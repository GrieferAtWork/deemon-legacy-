/* Copyright (c) 2016 - deemon by Griefer@Work                                    *
 *                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy   *
 * of this software and associated documentation files (the "Software"), to deal  *
 * in the Software without restriction, including without limitation the rights   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
 * copies of the Software, and to permit persons to whom the Software is          *
 * furnished to do so, subject to the following conditions:                       *
 *                                                                                *
 * The above copyright notice and this permission notice shall be included in all *
 * copies or substantial portions of the Software.                                *
 *                                                                                *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
 * SOFTWARE.                                                                      *
 */
#ifndef GUARD_DEEMON_DEX_C
#define GUARD_DEEMON_DEX_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

#include DEE_INCLUDE_MEMORY_API()

// /include/*
#include <deemon/deemonrun.h>
#include <deemon/dex.h>
#include <deemon/error.h>
#include <deemon/fs_api.h>
#include <deemon/mp/mutex.h>
#include <deemon/mp/thread.h>
#include <deemon/memberdef.h>
#include <deemon/generic_vtable.h>
#include <deemon/list.h>
#include <deemon/optional/enum_attributes.h>
#include <deemon/optional/atomic_mutex.h>
#include <deemon/optional/hashmap.h>
#include <deemon/optional/object_hash.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_visit.h>
#include <deemon/type.h>
#include <deemon/value.h>

// /src/*
#include <deemon/__alloca.inl>
#include <deemon/marshal_data.h>
#include <deemon/sys/sysdynlib.h>
#include <deemon/runtime/builtin_functions.h>

// */ (nano...)

DEE_DECL_BEGIN

typedef DEE_A_RET_EXCEPT(-1) int (DEE_CALL *PDeeDexMain)(DEE_A_INOUT DeeDexContext *context);

struct DeeDexModuleObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeAnyStringObject    *dm_name;      /*< [1..1][const] Name used for this dex. */
 struct DeeDexExportDef          *dm_exports;   /*< [1..1] List of dex exports. */
 /*atomic*/Dee_uint16_t           dm_state;     /*< Dex State. */
 /*atomic*/Dee_uint16_t           dm_tickets;   /*< Amount of tickets preventing dex finalization. */
 PDeeDexMain                      dm_main;      /*< [0..1][const] Dex event callback. */
 struct DeeSysDynlibN             dm_dynlib;    /*< [?..?][owned][const] .so/.dll library. */
 struct DeeNativeMutex            dm_cachelock; /*< Lock for all the cached portions of the dex. */
 struct DeeDexModuleObject       *dm_initnext;  /*< [0..1][lock(:_dex_init_list_lock)] Dex initialized after this one. */
 struct DeeDexModuleObject       *dm_initprev;  /*< [0..1][lock(:_dex_init_list_lock)] Dex initialized before this one. */
};

//////////////////////////////////////////////////////////////////////////
// Linked list of dex modules currently initialized
// -> This list stores the order in which all loaded dex modules were initialized in.
//    During shutdown, all modules are finalized in the reserve order of being initialized.
// NOTE: This list does not own any real references (those are stored in the dex map)
static struct DeeDexModuleObject *_dex_init_list = NULL; /*< End of the list (this->dm_initnext == NULL) */
static struct DeeAtomicMutex _dex_init_list_lock = DeeAtomicMutex_INIT();
#define DeeDexModule_REGISTER_INITIALIZED(ob)\
do{\
 DEE_ASSERT((ob)->dm_initprev == NULL);\
 DEE_ASSERT((ob)->dm_initnext == NULL);\
 DeeAtomicMutex_AcquireRelaxed(&_dex_init_list_lock);\
 if (((ob)->dm_initprev = _dex_init_list) != NULL)\
  _dex_init_list->dm_initnext = (ob);\
 _dex_init_list = (ob);\
 DeeAtomicMutex_Release(&_dex_init_list_lock);\
}while(0)
#define DeeDexModule_UNREGISTER_INITIALIZED(ob)\
do{\
 DeeAtomicMutex_AcquireRelaxed(&_dex_init_list_lock);\
 if ((ob)->dm_initprev) (ob)->dm_initprev->dm_initnext = (ob)->dm_initnext;\
 if ((ob)->dm_initnext) (ob)->dm_initnext->dm_initprev = (ob)->dm_initprev;\
 else if ((ob) == _dex_init_list) _dex_init_list = (ob)->dm_initprev;\
 (ob)->dm_initprev = NULL;\
 (ob)->dm_initnext = NULL;\
 DeeAtomicMutex_Release(&_dex_init_list_lock);\
}while(0)


#ifdef DEE_DEBUG
static void _DeeDexModule_DebugCheckIntegrity(DeeDexModuleObject *self) {
 struct DeeDexExportDef *iter,*iter2;
 for (iter = self->dm_exports; iter->dxe_name; ++iter) {
  for (iter2 = iter+1; iter2->dxe_name; ++iter2) {
   if (strcmp(iter->dxe_name,iter2->dxe_name) == 0) {
    if (DeeFile_StdPrintf(DEE_STDERR,
     "[dex %r][export %q] Export listed more than once\n",
     self->dm_name,iter->dxe_name) != 0) DeeError_Print(NULL,1);
   }
  }
  switch (iter->dxe_kind) {
   case DEE_DEX_EXPORT_TYPE_OBJECT:
    if (!DeeObject_Check(iter->dxe_object)) {
     if (DeeFile_StdPrintf(DEE_STDERR,
      "[dex %r][export %q][DEE_DEX_EXPORT_TYPE_OBJECT] Invalid object at %p\n",
      self->dm_name,iter->dxe_name,iter->dxe_object) != 0) DeeError_Print(NULL,1);
    }
    break;
   case DEE_DEX_EXPORT_TYPE_GENERATOR:
    if (iter->dxe_generator.dxe_gn_cache != NULL) {
     if (DeeFile_StdPrintf(DEE_STDERR,
      "[dex %r][export %q][DEE_DEX_EXPORT_TYPE_GENERATOR] "
      "Generator cache incorrectly initialized to %p instead of NULL "
      "(this would cause a crash in non-debug builds)\n",
      self->dm_name,iter->dxe_name,iter->dxe_generator.dxe_gn_cache) != 0) DeeError_Print(NULL,1);
     iter->dxe_generator.dxe_gn_cache = NULL;
    }
    if (iter->dxe_generator.dxe_gn_ctor == NULL) {
     if (DeeFile_StdPrintf(DEE_STDERR,
      "[dex %r][export %q][DEE_DEX_EXPORT_TYPE_GENERATOR] "
      "Invalid generator constructor (may not be NULL)\n",
      self->dm_name,iter->dxe_name) != 0) DeeError_Print(NULL,1);
     iter->dxe_generator.dxe_gn_cache = NULL;
    }
    break;
   case DEE_DEX_EXPORT_TYPE_GETSET: break;
   case DEE_DEX_EXPORT_TYPE_OBJECTINL:
    if (!DeeObject_Check(&iter->dxe_objectinl)) {
     if (DeeFile_StdPrintf(DEE_STDERR,
      "[dex %r][export %q][DEE_DEX_EXPORT_TYPE_OBJECTINL] Invalid inlined object at %p\n",
      self->dm_name,iter->dxe_name,&iter->dxe_objectinl) != 0) DeeError_Print(NULL,1);
    }
    break;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS || 1
   case DEE_DEX_EXPORT_TYPE_NATIVE_FUNCTION: {
    if (!iter->dxe_native_function.dxe_nf_function) {
     if (DeeFile_StdPrintf(DEE_STDERR,
      "[dex %r][export %q][DEE_DEX_EXPORT_TYPE_NATIVE_FUNCTION] Invalid native function pointer: NULL\n",
      self->dm_name,iter->dxe_name) != 0) DeeError_Print(NULL,1);
    }
    if (!iter->dxe_native_function.dxe_nf_type) {
     if (DeeFile_StdPrintf(DEE_STDERR,
      "[dex %r][export %q][DEE_DEX_EXPORT_TYPE_NATIVE_FUNCTION] Invalid native function type: NULL\n",
      self->dm_name,iter->dxe_name) != 0) DeeError_Print(NULL,1);
    } else if (!DeeType_Structf(iter->dxe_native_function.dxe_nf_type)) {
     DeeError_Print("Error while testing dex native function export type",1);
     if (DeeFile_StdPrintf(DEE_STDERR,
      "[dex %r][export %q][DEE_DEX_EXPORT_TYPE_NATIVE_FUNCTION] Failed to create native function export type from %q\n",
      self->dm_name,iter->dxe_name,iter->dxe_native_function.dxe_nf_type) != 0) DeeError_Print(NULL,1);
    }
   } break;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   case DEE_DEX_EXPORT_TYPE_INT8: case DEE_DEX_EXPORT_TYPE_INT16:
   case DEE_DEX_EXPORT_TYPE_INT32: case DEE_DEX_EXPORT_TYPE_INT64:
   case DEE_DEX_EXPORT_TYPE_UINT8: case DEE_DEX_EXPORT_TYPE_UINT16:
   case DEE_DEX_EXPORT_TYPE_UINT32: case DEE_DEX_EXPORT_TYPE_UINT64: break;
   default:
    if (DeeFile_StdPrintf(DEE_STDERR,
     "[dex %r][export %q] Invalid/Unsupported kind of export: 0x%.8I32x\n",
     self->dm_name,iter->dxe_name,iter->dxe_kind) != 0) DeeError_Print(NULL,1);
    break;
  }
 }
}
#endif

static DEE_A_RET_EXCEPT(-1) int DeeDexModule_Acquire(
 DEE_A_INOUT DeeDexModuleObject *self, DEE_A_IN Dee_uint32_t init_reason) {
 Dee_uint16_t old_flags;
 Dee_uint32_t old_access,new_access;
 // Start by simultaneously checking for shutdown, as well as adding a dex ticket
 do {
  old_access = (Dee_uint32_t)DeeAtomic32_Load(*(Dee_uint32_t *)&self->dm_state,memory_order_seq_cst);
  old_flags = ((Dee_uint16_t *)&old_access)[0];
  if ((old_flags & DEE_DEXMODULE_FLAGS_STATE_SHUTDOWN)!=0) {
dex_shutdown:
   DeeError_SET_STRING(&DeeErrorType_ValueError,"Dex was shut down");
   return -1;
  }
  new_access = old_access;
  ++((Dee_uint16_t *)&new_access)[1]; // Add a new ticket
 } while (!DeeAtomic32_CompareExchangeStrong(
  *(Dee_uint32_t *)&self->dm_state,old_access,new_access,
  memory_order_seq_cst,memory_order_seq_cst));
wait_init:
 if ((old_flags &
  (DEE_DEXMODULE_FLAGS_STATE_INITIALIZED|DEE_DEXMODULE_FLAGS_STATE_INITIALIZING))!=
  (DEE_DEXMODULE_FLAGS_STATE_INITIALIZED|DEE_DEXMODULE_FLAGS_STATE_INITIALIZING)) {
  // Dex must be initialized / is already being initialized
  old_flags = (Dee_uint16_t)DeeAtomic16_FetchOr(self->dm_state,DEE_DEXMODULE_FLAGS_STATE_INITIALIZING,memory_order_seq_cst);
  if ((old_flags&DEE_DEXMODULE_FLAGS_STATE_INITIALIZING)==0) {
   // It's our job to initialize it!
   // And we already got exclusive access for calling init.
#ifdef DEE_DEBUG
   // Do some checks on the integrity of the module and warn about inconsistencies
   _DeeDexModule_DebugCheckIntegrity(self);
#endif
   DEE_LVERBOSE1("Loading extension %r...\n",self->dm_name);
   if (self->dm_main) {
    DeeDexContext ctx; int temp;
    ctx.dc_kind = DEE_DEXCONTEXTKIND_INITIALIZE;
    ctx.dc_initialize.ci_reason = init_reason;
    ctx.dc_initialize.dc_self = (DeeObject *)self;
    temp = (*self->dm_main)(&ctx);
    if (temp != 0) { // Initialization failed...
     DeeAtomic16_FetchDec(self->dm_tickets,memory_order_seq_cst);
     DeeAtomic16_FetchAnd(self->dm_state,~(DEE_DEXMODULE_FLAGS_STATE_INITIALIZING),memory_order_seq_cst);
     return temp;
    }
   }
   DEE_LVERBOSE1("Loaded extension %r\n",self->dm_name);
   DeeAtomic16_FetchOr(self->dm_state,DEE_DEXMODULE_FLAGS_STATE_INITIALIZED,memory_order_seq_cst);
   DeeDexModule_REGISTER_INITIALIZED(self);
  } else if ((old_flags&DEE_DEXMODULE_FLAGS_STATE_INITIALIZED)==0) {
   // Wait a bit and check again
   if (DeeThread_Sleep(1) != 0) return -1;
   old_flags = (Dee_uint16_t)DeeAtomic16_Load(self->dm_state,memory_order_seq_cst);
   if ((old_flags&DEE_DEXMODULE_FLAGS_STATE_SHUTDOWN)!=0) {
    // Dex was shut down in the mean time
    DeeAtomic16_FetchDec(self->dm_tickets,memory_order_seq_cst);
    goto dex_shutdown;
   }
   goto wait_init;
  } // else { /* Dex is now initialized. */ }
 }
 // We now have concurrent access to the dex that
 // will prevent it from being closed on us.
 return 0;
}

// Tries to acquire a ticket, but fails if the isn't initialized, or was shutdown
#define DEE_DEX_TRYACQUIRE_SUCCESS    0
#define DEE_DEX_TRYACQUIRE_SHUTDOWN (-1) /*< Can't acquire ticket: the dex was shut down. */
#define DEE_DEX_TRYACQUIRE_NOTINIT  (-2) /*< Can't acquire ticket: the dex wasn't initialized. */
static DEE_A_RET_NOEXCEPT(0) int DeeDexModule_TryAcquire(DEE_A_INOUT DeeDexModuleObject *self) {
 Dee_uint16_t old_flags;
 Dee_uint32_t old_access,new_access;
 // Start by simultaniously checking for shutdown, as well as adding a dex ticket
 do {
  old_access = (Dee_uint32_t)DeeAtomic32_Load(*(Dee_uint32_t *)&self->dm_state,memory_order_seq_cst);
  old_flags = ((Dee_uint16_t *)&old_access)[0];
  if ((old_flags&DEE_DEXMODULE_FLAGS_STATE_SHUTDOWN)!=0) return DEE_DEX_TRYACQUIRE_SHUTDOWN;
  new_access = old_access;
  ++((Dee_uint16_t *)&new_access)[1]; // Add a new ticket
 } while (!DeeAtomic32_CompareExchangeStrong(
  *(Dee_uint32_t *)&self->dm_state,old_access,new_access,
  memory_order_seq_cst,memory_order_seq_cst));
 while ((old_flags&(DEE_DEXMODULE_FLAGS_STATE_INITIALIZED|
  DEE_DEXMODULE_FLAGS_STATE_INITIALIZING)) == (DEE_DEXMODULE_FLAGS_STATE_INITIALIZING)) {
  DeeThread_SleepNoInterrupt(1); // Wait for the dex to finish initialization
  old_flags = (Dee_uint16_t)DeeAtomic16_Load(self->dm_state,memory_order_seq_cst);
  if ((old_flags&DEE_DEXMODULE_FLAGS_STATE_SHUTDOWN)!=0) {
   DeeAtomic16_FetchDec(self->dm_tickets,memory_order_seq_cst);
   return DEE_DEX_TRYACQUIRE_SHUTDOWN;
  }
 }
 if ((old_flags&DEE_DEXMODULE_FLAGS_STATE_INITIALIZED)==0) {
  DeeAtomic16_FetchDec(self->dm_tickets,memory_order_seq_cst);
  return DEE_DEX_TRYACQUIRE_NOTINIT;
 }
 // We now have concurrent access to the dex that
 // will prevent it from being closed on us.
 return 0;
}
#define DeeDexModule_Release(ob)\
 DeeAtomic16_FetchDec((ob)->dm_tickets,memory_order_seq_cst)


// Shutdown a dex module, preventing any following calls to get/del/set from succeeding
static void DeeDexModule_Shutdown(
 DEE_A_INOUT DeeDexModuleObject *self, DEE_A_IN Dee_uint32_t quit_reason) {
 struct DeeDexExportDef *xiter; Dee_uint16_t old_flags;
 old_flags = (Dee_uint16_t)DeeAtomic16_FetchOr(self->dm_state,DEE_DEXMODULE_FLAGS_STATE_SHUTDOWN,memory_order_seq_cst);
 if ((old_flags&DEE_DEXMODULE_FLAGS_STATE_SHUTDOWN)!=0) return; // No-op if the dex was already shut down
 // Wait until all users disappear (new ones can't pop up because the dex is now in shutdown mode)
 while (DeeAtomic16_Load(self->dm_tickets,memory_order_seq_cst)) DeeThread_SleepNoInterrupt(1);
 // Now the dex can no longer be loaded, and all users are gone.
 // And we have exclusive rights for calling the destructor
 // NOTE: Reload the flags, as an initialization might have been in progress
 old_flags = (Dee_uint16_t)DeeAtomic16_Load(self->dm_state,memory_order_seq_cst);
 // Now comes a little bit complicated part:
 //   We need to go though all of the dex'es exports and destroy generator caches
 // NOTE: Since the dex was shut down, we have exclusive access to the export table
 xiter = self->dm_exports;
 while (xiter->dxe_name) {
  if (xiter->dxe_kind == DEE_DEX_EXPORT_TYPE_GENERATOR && xiter->dxe_generator.dxe_gn_cache) {
   DeeObject *cache_entry = xiter->dxe_generator.dxe_gn_cache;
   xiter->dxe_generator.dxe_gn_cache = NULL;
   if (xiter->dxe_generator.dxe_gn_dtor)
    (*xiter->dxe_generator.dxe_gn_dtor)(cache_entry);
   else Dee_DECREF(cache_entry);
  }
  ++xiter;
 }

 // Finally, call the actual finalizer of the dex
 DEE_LVERBOSE1("Unloading extension %r...\n",self->dm_name);
 if ((old_flags&DEE_DEXMODULE_FLAGS_STATE_INITIALIZED)!=0 && self->dm_main) {
  DeeDexContext ctx; int temp;
  // The dex was in fact initialized (time to finalize it)
  ctx.dc_kind = DEE_DEXCONTEXTKIND_FINALIZE;
  ctx.dc_finalize.cf_reason = quit_reason;
  ctx.dc_finalize.dc_self = (DeeObject *)self;
  temp = (*self->dm_main)(&ctx);
  if DEE_UNLIKELY(temp != 0) DeeError_Print("[ignored] Error occurred during dex finalization",1);
 }
 DEE_LVERBOSE1("Unloaded extension %r\n",self->dm_name);
 // Add the finalized flag for anything insterested
 DeeAtomic16_FetchOr(self->dm_state,DEE_DEXMODULE_FLAGS_STATE_FINALIZED,memory_order_seq_cst);
 DeeDexModule_UNREGISTER_INITIALIZED(self);
}






DeeListObject _DeeDex_SearchPath = DeeList_INIT();
static struct DeeAtomicMutex _dex_modules_lock = DeeAtomicMutex_INIT();
static struct DeeHashMap _dex_modules_map = DeeHashMap_INIT();


DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *
DeeDex_GetDefaultSearchPath(void) {
 DeeObject *result,*env_paths,*new_path; int error;
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) return NULL;
 if ((env_paths = DeeFS_TryGetEnv(DEE_AUTOCONF_VARNAME_DEEMON_DEXPATH)) != NULL) {
  char const *begin,*iter,*end;
  end = (begin = iter = DeeString_STR(env_paths))+DeeString_SIZE(env_paths);
  while (iter != end) {
   if (*iter == DEE_FS_DELIM) {
    if (iter != begin) {
     if DEE_UNLIKELY((new_path = DeeString_NewWithLength((
      size_t)(iter-begin),begin)) == NULL) {
err_renv: Dee_DECREF(env_paths);
/*err_r:*/ Dee_DECREF(result); return NULL;
     }
     error = DeeList_Append(result,new_path);
     Dee_DECREF(new_path);
     if DEE_UNLIKELY(error != 0) goto err_renv;
    }
    begin = iter+1;
   }
   ++iter;
  }
  // Append the rest
  if (begin != end) {
   if (begin == DeeString_STR(env_paths)) {
    // Optimization for a single path
    if DEE_UNLIKELY(DeeList_Append(result,env_paths) != 0) goto err_renv;
   } else {
    if DEE_UNLIKELY((new_path = DeeString_NewWithLength((
     size_t)(iter-begin),begin)) == NULL) goto err_renv;
    error = DeeList_Append(result,new_path);
    Dee_DECREF(new_path);
    if DEE_UNLIKELY(error != 0) goto err_renv;
   }
  }
  Dee_DECREF(env_paths);
 }
 return result;
}

DEE_STATIC_INLINE(void) _dex_searchorder_enable(void);
DEE_STATIC_INLINE(void) _dex_searchorder_disable(void);

DEE_A_RET_EXCEPT(-1) int _DeeDex_InitApi(void) {
 DeeObject *new_paths; int error;
 if DEE_UNLIKELY((new_paths = DeeDex_GetDefaultSearchPath()) == NULL) return -1;
#if 0 /* Extensions must use deemon for communicating with each other!. */
#ifdef DEE_PLATFORM_WINDOWS
 { // In order for the extensions to see each other, we need to add 'new_paths' to $PATH
  DeeString_NEW_STATIC_EX(_sepstring,1,{DEE_FS_DELIM});
  DeeObject *additional_paths,*path_env;
  additional_paths = DeeString_Join((DeeObject *)&_sepstring,new_paths);
  if DEE_LIKELY((path_env = DeeFS_TryGetEnv("PATH")) != NULL) {
   path_env = DeeString_Newf("%K" DEE_FS_DELIM_S "%K",path_env,additional_paths);
   if DEE_UNLIKELY(path_env == NULL) { Dee_DECREF(new_paths); return -1; }
  }
  error = DeeFS_SetEnv("PATH",DeeString_STR(path_env));
  Dee_DECREF(path_env);
  if DEE_UNLIKELY(error != 0) return error;
 }
#endif
#endif
 error = DeeList_AssignVector(DeeDex_SearchPath,DeeList_SIZE(new_paths),DeeList_ELEM(new_paths));
 Dee_DECREF(new_paths);
 if (error != 0) return error;
 // Enable the actual API
 _dex_searchorder_enable();
 return 0;
}

void _DeeDex_ShutdownApi(void) {
 struct DeeDexModuleObject *load_order,*load_next;
 // Disable the dex
 // -> This call will prevent the loading of new dex modules
 //    by clearing the search path and making it read-only.
 _dex_searchorder_disable();
 // Clear all user-defined search paths
 // >> Only here in case the list contained a self-reference,
 //    something that it really shouldn't, but cannot be enforced.
 // NOTE: Remember that this function is repeatedly
 //       called from the shutdown-gc-loop.
 DeeList_ClearAndShrink(DeeDex_SearchPath);

 DeeAtomicMutex_AcquireRelaxed(&_dex_init_list_lock);
 load_order = _dex_init_list; _dex_init_list = NULL;
 DeeAtomicMutex_Release(&_dex_init_list_lock);
 while (load_order) {
  load_next = load_order->dm_initprev;
  load_order->dm_initprev = NULL;
  load_order->dm_initnext = NULL;
  DeeDexModule_Shutdown(load_order,DEE_DEXCONTEXT_FINALIZE_REASON_SHUTDOWN);
  DEE_ASSERT(!_dex_init_list);
  load_order = load_next;
 }

}
void _DeeDex_QuitApi(void) {
 // Here, we clear the search path one last time
 DeeList_ClearAndShrink(DeeDex_SearchPath);
 // The following like will do the actual unloading of all dex
 // modules, actually freeing all of their library handles.
 DeeHashMap_Clear(&_dex_modules_map);
 //DeeHashMap_ClearWithLock(&_dex_modules_map,&_dex_modules_lock);
}

#ifdef DEE_PLATFORM_WINDOWS
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDexModuleObject) *)
 DeeDex_Open_impl(DEE_A_IN_Z char const *name);
DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDexModuleObject) *
 DeeDex_Open(DEE_A_IN_Z char const *name) {
 char ch; char const *name_check;
 DEE_ASSERT(name);
 name_check = name;
 do { // Make sure the name is lowercase
  ch = *name_check++;
  if (ch >= 'A' && ch <= 'Z') goto use_lower;
 } while (ch);
#if DEE_HAVE_ALLOCA
use_name:
#endif
 return DeeDex_Open_impl(name);
use_lower: {
#if !DEE_HAVE_ALLOCA
  DeeObject *result;
#endif
  Dee_size_t name_len;
  char *lower_name,*iter;
  name_len = strlen(name);
#if DEE_HAVE_ALLOCA
  lower_name = (char *)alloca((name_len+1)*sizeof(char));
#else
  while ((lower_name = (char *)malloc_nz((
   name_len+1)*sizeof(char))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return NULL;
  }
#endif
  iter = lower_name;
  name_check = name;
  do {
   ch = *name_check++;
   if (ch >= 'A' && ch <= 'Z') ch += 'a'-'A';
   *iter++ = ch;
  } while (ch);
#if DEE_HAVE_ALLOCA
  goto use_name;
#else
  result = DeeDex_Open_impl(lower_name);
  free_nn(lower_name);
  return result;
#endif
 }
}
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDexModuleObject) *)
 DeeDex_Open_impl(DEE_A_IN_Z char const *name)
#else
DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDexModuleObject) *
 DeeDex_Open(DEE_A_IN_Z char const *name)
#endif
{
 DeeDexModuleObject *result,*old_module; Dee_hash_t name_hash; int temp;
 DEE_ASSERT(name);
 name_hash = DeeHash_String(name,DEE_HASH_START);
 temp = DeeHashMap_GetItemExStringWithKnownBucketAndLock(&_dex_modules_map,name,name_hash,
                                                         (DeeObject **)&result,&_dex_modules_lock);
 if (temp == 0) return (DeeObject *)result; // Already found it (fast pass)
 if DEE_UNLIKELY(temp < 0) return NULL; // Error
 // Dex not loaded (time to create/search it)
 result = (DeeDexModuleObject *)DeeDexModule_New(name);
 if DEE_UNLIKELY(!result) return NULL; // Error
#ifdef DEE_DEBUG
 old_module = NULL;
#endif
 temp = DeeHashMap_SetNewItemStringWithKnownBucketAndLock(&_dex_modules_map,name,name_hash,
                                                          (DeeObject *)result,(DeeObject **)&old_module,
                                                          &_dex_modules_lock);
 if (temp == 0) return (DeeObject *)result; // Module has been set!
 if DEE_UNLIKELY(temp < 0) { Dee_DECREF(result); return NULL; } // Error
 // The module was loaded by another thread, and we were slower
 // >> Delete our's and return the other threads (currently stored in 'old_module')
 DEE_ASSERT(old_module != NULL);
 DEE_ASSERT(old_module != result);
 DEE_ASSERT(result);
 DEE_ASSERT(DeeObject_IS_UNIQUE(result));
 Dee_DECREF(result);
 return (DeeObject *)old_module;
}


DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) void *DeeDex_ImportNativeEx(
 DEE_A_IN_Z char const *dex_name, DEE_A_IN_Z char const *import_name) {
 DeeObject *dex; void *result;
 DEE_ASSERT(dex_name);
 DEE_ASSERT(import_name);
 if DEE_UNLIKELY((dex = DeeDex_Open(dex_name)) == NULL) return NULL;
 result = DeeDexModule_GetNativeAddress(dex,import_name);
 Dee_DECREF(dex);
 return result;
}
DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *
DeeDex_ImportEx(DEE_A_IN_Z char const *dex_name, DEE_A_IN_Z char const *import_name) {
 DeeObject *dex,*result;
 DEE_ASSERT(dex_name);
 DEE_ASSERT(import_name);
 if DEE_UNLIKELY((dex = DeeDex_Open(dex_name)) == NULL) return NULL;
 result = DeeDexModule_GetAttrString(dex,import_name);
 Dee_DECREF(dex);
 return result;
}

DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *
DeeDex_VCallf(DEE_A_IN_Z char const *name_and_fmt, DEE_A_INOUT va_list args) {
 char const *name_begin; Dee_size_t name_size; char *name_copy;
 DeeObject *result,*import_ob,*obargs;
 name_begin = name_and_fmt;
 while (*name_and_fmt && *name_and_fmt != '(') ++name_and_fmt;
 if ((obargs = Dee_VBuildValue(name_and_fmt,args)) == NULL) return NULL;
 name_size = (Dee_size_t)(name_and_fmt-name_begin);
#if DEE_HAVE_ALLOCA
 name_copy = (char *)alloca((name_size+1)*sizeof(char));
#else
 while ((name_copy = (char *)malloc_nz((name_size+1)*sizeof(char))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
#endif
 memcpy(name_copy,name_begin,name_size*sizeof(char));
 name_copy[name_size] = 0;
 import_ob = DeeDex_Import(name_copy);
#if !DEE_HAVE_ALLOCA
 free_nn(name_copy);
#endif
 if DEE_UNLIKELY(!import_ob) { Dee_DECREF(obargs); return NULL; }
 result = DeeObject_Call(import_ob,obargs);
 Dee_DECREF(import_ob);
 Dee_DECREF(obargs);
 return result;
}
DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *
DeeDex_Callf(DEE_A_IN_Z char const *name_and_fmt, ...) {
 va_list args; DeeObject *result;
 DEE_VA_START(args,name_and_fmt);
 result = DeeDex_VCallf(name_and_fmt,args);
 DEE_VA_END(args);
 return result;
}

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDex_VCallAndCastf(
 DEE_A_OUT void *result, DEE_A_IN_Z char const *name_and_fmt_and_result, DEE_A_INOUT va_list args) {
 DeeObject *retval; char const *fmt_end;
 char *fmt_begin; Dee_size_t fmt_len; int error;
 DEE_ASSERT(name_and_fmt_and_result);
 fmt_end = name_and_fmt_and_result;
 while (*fmt_end != '!') {
  if (!*fmt_end) {
   if DEE_UNLIKELY((retval = DeeDex_VCallf(name_and_fmt_and_result,args)) == NULL) return -1;
   Dee_DECREF(retval);
   return 0;
  }
  ++fmt_end;
 }
 // At this point, 'result' isn't allowed to be NULL
 DEE_ASSERT(result);
 fmt_len = (Dee_size_t)(fmt_end-name_and_fmt_and_result);
#if DEE_HAVE_ALLOCA
 fmt_begin = (char *)alloca((fmt_len+1)*sizeof(char));
#else /* DEE_HAVE_ALLOCA */
 while DEE_UNLIKELY((fmt_begin = (char *)malloc_nz((fmt_len+1)*sizeof(char))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
#endif /* !DEE_HAVE_ALLOCA */
 memcpy(fmt_begin,name_and_fmt_and_result,fmt_len*sizeof(char));
 fmt_begin[fmt_len] = '\0';
 retval = DeeDex_VCallf(fmt_begin,args);
#if !DEE_HAVE_ALLOCA
 free_nn(fmt_begin);
#endif
 if DEE_UNLIKELY(!retval) return -1;
 error = DeeObject_Castf(retval,result,fmt_end+1);
 Dee_DECREF(retval);
 return error;
}

DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeDex_CallAndCastf(
 DEE_A_OUT void *result, DEE_A_IN_Z char const *name_and_fmt_and_result, ...) {
 va_list args; int error;
 DEE_VA_START(args,name_and_fmt_and_result);
 error = DeeDex_VCallAndCastf(result,name_and_fmt_and_result,args);
 DEE_VA_END(args);
 return error;
}




// v currently, there are only 6 different modes, so this is enough (+1 for \0 end marker)
#define DEEDEX_SEARCHORDER_INTERNAL_MAXSIZE 7
static char const _dex_searchorder_default[DEEDEX_SEARCHORDER_INTERNAL_MAXSIZE] = DEEDEX_SEARCHORDER_DEFAULT;
static char _dex_searchorder[DEEDEX_SEARCHORDER_INTERNAL_MAXSIZE] = "\0";
static int _dex_searchorder_ro = 1; // Search order is read-only
static struct DeeAtomicMutex _dex_searchorder_lock = DeeAtomicMutex_INIT();


#define DEE_DEX_SEARCHORDER_SUCCESS           0
#define DEE_DEX_SEARCHORDER_INVALID_MARKER  (-1)
#define DEE_DEX_SEARCHORDER_REUSED_MARKER   (-2)
static int _dex_searchorder_validate(
 DEE_A_IN_Z char const *order, DEE_A_OUT char const **error_or_end_pos) {
 char const *check_iter,*walk; char ch;
 DEE_ASSERT(order); DEE_ASSERT(error_or_end_pos);
 check_iter = order; // Validate the given order string
 while (1) {
  ch = *check_iter++;
  switch (ch) {
   case 'V': case 'D':
   case 'C': case 'X':
   case 'P': case 'S': break;
   case 0: goto check_ok;
   default:
    *error_or_end_pos = check_iter-1;
    return DEE_DEX_SEARCHORDER_INVALID_MARKER;
  }
  walk = order;
  while (walk != check_iter) {
   DEE_ASSERT(walk < check_iter);
   if (*walk == ch) {
    // Order marker was already used.
    *error_or_end_pos = check_iter-1;
    return DEE_DEX_SEARCHORDER_REUSED_MARKER;
   }
  }
 }
check_ok:
 DEE_ASSERT(strlen(order) == (Dee_size_t)(check_iter-order));
 DEE_ASSERT((Dee_size_t)(check_iter-order) < DEEDEX_SEARCHORDER_INTERNAL_MAXSIZE);
 *error_or_end_pos = check_iter;
 return DEE_DEX_SEARCHORDER_SUCCESS;
}


DEE_STATIC_INLINE(void) _dex_searchorder_enable(void) {
 DeeObject *used_order; char const *error_pos;
 if ((used_order = DeeFS_TryGetEnv(DEE_AUTOCONF_VARNAME_DEEMON_DEXORDER)) != NULL) {
  switch (_dex_searchorder_validate(DeeString_STR(used_order),&error_pos)) {
   case DEE_DEX_SEARCHORDER_INVALID_MARKER:
    if (DeeFile_StdPrintf(DEE_STDERR,"Invalid order maker in dex search order: %.1q",error_pos) != 0) DeeError_Handled();
    Dee_DECREF(used_order);
    goto set_default_order;
   case DEE_DEX_SEARCHORDER_REUSED_MARKER:
    if (DeeFile_StdPrintf(DEE_STDERR,"Order marker reused in dex search order: %.1q",error_pos) != 0) DeeError_Handled();
    Dee_DECREF(used_order);
    goto set_default_order;
   default: break;
  }
  DeeAtomicMutex_Acquire(&_dex_searchorder_lock);
  memcpy(_dex_searchorder,DeeString_STR(used_order),
         ((Dee_size_t)((error_pos-DeeString_STR(used_order))+1))*sizeof(char));
  _dex_searchorder_ro = 0;
  DeeAtomicMutex_Release(&_dex_searchorder_lock);
  Dee_DECREF(used_order);
 } else {
set_default_order:
  DeeAtomicMutex_Acquire(&_dex_searchorder_lock);
  memcpy(_dex_searchorder,_dex_searchorder_default,sizeof(_dex_searchorder));
  _dex_searchorder_ro = 0;
  DeeAtomicMutex_Release(&_dex_searchorder_lock);
 }
}
DEE_STATIC_INLINE(void) _dex_searchorder_disable(void) {
 DeeAtomicMutex_Acquire(&_dex_searchorder_lock);
 _dex_searchorder[0] = 0;
 _dex_searchorder_ro = 1;
 DeeAtomicMutex_Release(&_dex_searchorder_lock);
}

Dee_size_t DeeDex_GetSearchMode(DEE_A_OUT_W(s) char *order, Dee_size_t s) {
 Dee_size_t result;
 DEE_ASSERT(!s || order);
 DeeAtomicMutex_Acquire(&_dex_searchorder_lock);
 result = strlen(_dex_searchorder)+1;
 DEE_ASSERTF(result <= DEEDEX_SEARCHORDER_INTERNAL_MAXSIZE,
             "Overflow: The dex search order is too long");
 memcpy(order,_dex_searchorder,result < s ? result : s);
 DeeAtomicMutex_Release(&_dex_searchorder_lock);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeDex_SetSearchOrder(DEE_A_IN_Z_OPT char const *order) {
 char const *error_or_end;
 if (!order) {
  DeeAtomicMutex_Acquire(&_dex_searchorder_lock);
  if DEE_UNLIKELY(_dex_searchorder_ro) {
   DeeAtomicMutex_Release(&_dex_searchorder_lock);
   return 0; /*< Silently ignore after the dex was shut down. */
  }
  memcpy(_dex_searchorder,_dex_searchorder_default,sizeof(_dex_searchorder));
  DeeAtomicMutex_Release(&_dex_searchorder_lock);
  return 0;
 }
 // Validate the given order string
 switch (_dex_searchorder_validate(order,&error_or_end)) {
  case DEE_DEX_SEARCHORDER_INVALID_MARKER:
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Invalid order maker in dex search order: %.1q",
                       error_or_end);
   return -1;
  case DEE_DEX_SEARCHORDER_REUSED_MARKER:
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Order marker reused in dex search order: %.1q",
                       error_or_end);
   return -1;
  default: break;
 }
 DeeAtomicMutex_Acquire(&_dex_searchorder_lock);
 if DEE_UNLIKELY(_dex_searchorder_ro) {
  DeeAtomicMutex_Release(&_dex_searchorder_lock);
  return 0; /*< Silently ignore after the dex was shut down. */
 }
 memcpy(_dex_searchorder,order,((Dee_size_t)(error_or_end-order)+1)*sizeof(char));
 DeeAtomicMutex_Release(&_dex_searchorder_lock);
 return 0;
}



#define DEE_DEXMODULE_INIT_SUCCESS    0
#define DEE_DEXMODULE_INIT_NOTFOUND (-1) /*< The specified dex file wasn't found. */
#define DEE_DEXMODULE_INIT_BROKEN   (-3) /*< The specified dex file is broken (missing non-optional exports) */
#define DEE_DEXMODULE_INIT_ERROR    (-4) /*< Other, miscellaneous deemon error (s.a.: DeeError_Occurred()) */

static DEE_A_RET_NOEXCEPT(DEE_DEXMODULE_INIT_NOTFOUND) int DeeDexModule_Utf8InitFromName(
 DEE_A_INOUT DeeDexModuleObject *self, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dex_name);
static DEE_A_RET_NOEXCEPT(DEE_DEXMODULE_INIT_NOTFOUND) int DeeDexModule_WideInitFromName(
 DEE_A_INOUT DeeDexModuleObject *self, DEE_A_IN_OBJECT(DeeWideStringObject) const *dex_name);

static DEE_A_RET_EXCEPT(-1) int DeeDexModule_Init(
 DEE_A_INOUT DeeDexModuleObject *self,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *dex_name) {
 DeeObject *u8dexname; int error;
#ifdef DEE_PLATFORM_WINDOWS
 if (DeeWideString_Check(dex_name)) {
  error = DeeDexModule_WideInitFromName(self,dex_name);
 } else
#endif
 {
  if DEE_UNLIKELY((u8dexname = DeeUtf8String_Cast(dex_name)) == NULL) return -1;
  error = DeeDexModule_Utf8InitFromName(self,u8dexname);
  Dee_DECREF(u8dexname);
 }
 switch (error) {
  case DEE_DEXMODULE_INIT_SUCCESS: return 0;
  case DEE_DEXMODULE_INIT_ERROR: return -1;
  case DEE_DEXMODULE_INIT_BROKEN:
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Broken dex: %r",dex_name);
   return -1;
  default: break;
 }
 DeeError_SetStringf(&DeeErrorType_DexError,
                     "Dex not found: %r",dex_name);
 return -1;
}

DEE_A_EXEC DEE_A_REF DeeDexModuleObject *
DeeDexModule_New(DEE_A_IN_Z char const *name) {
 DeeDexModuleObject *result; DeeObject *dex_name; int temp;
 if DEE_UNLIKELY((dex_name = DeeString_New(name)) == NULL) return NULL;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeDexModuleObject)) == NULL) { Dee_DECREF(dex_name); return NULL; }
 DeeObject_INIT(result,&DeeDexModule_Type);
 temp = DeeDexModule_Init(result,dex_name);
 Dee_DECREF(dex_name); 
 if DEE_UNLIKELY(temp != 0) { _DeeObject_DELETE(&DeeDexModule_Type,result); return NULL; }
 return result;
}







DEE_A_RET_EXCEPT(-1) int DeeDexModule_EnumAttr(
 DEE_A_INOUT DeeDexModuleObject *self,
 DEE_A_IN DeeEnumAttrFunc enum_attr, void *closure) {
 struct DeeDexExportDef *xiter; int result;
 if DEE_UNLIKELY(DeeDexModule_Acquire(self,
  DEE_DEXCONTEXT_INITIALIZE_REASON_ENUMXP) != 0) return -1;
 xiter = self->dm_exports,result = 0;
 while (xiter->dxe_name) {
  switch (xiter->dxe_kind) {
   case DEE_DEX_EXPORT_TYPE_OBJECT:
    DEE_ASSERT(DeeObject_Check(xiter->dxe_object));
    result = (*enum_attr)(&DeeDexModule_Type,(DeeObject *)self,xiter->dxe_name,
                          DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET,
                          Dee_TYPE(xiter->dxe_object),closure);
    break;
   case DEE_DEX_EXPORT_TYPE_GENERATOR: {
    DeeTypeObject *attr_type;
    if DEE_UNLIKELY(DeeNativeMutex_Acquire(&self->dm_cachelock) != 0) result = -1;
    else {
     if (xiter->dxe_generator.dxe_gn_cache) {
      Dee_INCREF(attr_type = Dee_TYPE(xiter->dxe_generator.dxe_gn_cache));
     } else attr_type = NULL;
     result = DeeNativeMutex_Release(&self->dm_cachelock);
     if DEE_LIKELY(result == 0) result = (*enum_attr)(
      &DeeDexModule_Type,(DeeObject *)self,xiter->dxe_name,
      DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET|
      DEE_ENUMATTRIBUTE_FLAG_PERMISSION_DEL,attr_type,closure);
     Dee_XDECREF(attr_type);
    }
   } break;
   case DEE_DEX_EXPORT_TYPE_GETSET: {
    Dee_uint32_t flags = DEE_ENUMATTRIBUTE_FLAG_NONE;
    if (xiter->dxe_getset.dxe_gs_get) flags |= DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET;
    if (xiter->dxe_getset.dxe_gs_del) flags |= DEE_ENUMATTRIBUTE_FLAG_PERMISSION_DEL;
    if (xiter->dxe_getset.dxe_gs_set) flags |= DEE_ENUMATTRIBUTE_FLAG_PERMISSION_SET;
    result = (*enum_attr)(&DeeDexModule_Type,(DeeObject *)self,xiter->dxe_name,
                          flags,Dee_TYPE(&xiter->dxe_objectinl),closure);
   } break;
   case DEE_DEX_EXPORT_TYPE_OBJECTINL:
    DEE_ASSERT(DeeObject_Check(&xiter->dxe_objectinl));
    result = (*enum_attr)(&DeeDexModule_Type,(DeeObject *)self,xiter->dxe_name,
                          DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET,
                          Dee_TYPE(&xiter->dxe_objectinl),closure);
    break;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   case DEE_DEX_EXPORT_TYPE_NATIVE_FUNCTION: {
    DeeTypeObject *attr_type;
    attr_type = DeeType_Structf(xiter->dxe_native_function.dxe_nf_type);
    if DEE_LIKELY(attr_type) attr_type = DeeType_LValue(attr_type);
    result = DEE_LIKELY(attr_type) ? (*enum_attr)(&DeeDexModule_Type,(DeeObject *)self,
                                                  xiter->dxe_name,DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET,
                                                  attr_type,closure) : -1;
   } break;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   {
    DeeTypeObject *attr_type;
    if (0) { case DEE_DEX_EXPORT_TYPE_INT8:   attr_type = (DeeTypeObject *)&DeeInt8_Type; }
    if (0) { case DEE_DEX_EXPORT_TYPE_INT16:  attr_type = (DeeTypeObject *)&DeeInt16_Type; }
    if (0) { case DEE_DEX_EXPORT_TYPE_INT32:  attr_type = (DeeTypeObject *)&DeeInt32_Type; }
    if (0) { case DEE_DEX_EXPORT_TYPE_INT64:  attr_type = (DeeTypeObject *)&DeeInt64_Type; }
    if (0) { case DEE_DEX_EXPORT_TYPE_UINT8:  attr_type = (DeeTypeObject *)&DeeUInt8_Type; }
    if (0) { case DEE_DEX_EXPORT_TYPE_UINT16: attr_type = (DeeTypeObject *)&DeeUInt16_Type; }
    if (0) { case DEE_DEX_EXPORT_TYPE_UINT32: attr_type = (DeeTypeObject *)&DeeUInt32_Type; }
    if (0) { case DEE_DEX_EXPORT_TYPE_UINT64: attr_type = (DeeTypeObject *)&DeeUInt64_Type; }
    result = (*enum_attr)(&DeeDexModule_Type,(DeeObject *)self,xiter->dxe_name,
                          DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET,attr_type,closure);
   } break;

   default: // Unknown attribute...
    result = (*enum_attr)(&DeeDexModule_Type,(DeeObject *)self,xiter->dxe_name,
                          DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET,
                          NULL,closure);
    break;
  }
  if (result != 0) break;
  ++xiter;
 }
 DeeDexModule_Release(self);
 return result;
}





DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) void *DeeDexModule_GetNativeAddress(
 DEE_A_INOUT_OBJECT(DeeDexModuleObject) *self, DEE_A_IN_Z char const *name) {
 void *result; DEE_ASSERT(name);
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 if DEE_UNLIKELY(DeeDexModule_Acquire((DeeDexModuleObject *)self,
  DEE_DEXCONTEXT_INITIALIZE_REASON_NATIVE) != 0) return NULL;
DEE_COMPILER_MSVC_WARNING_PUSH(4152)
 if DEE_UNLIKELY(!DeeSysDynlib_TryImport(&((DeeDexModuleObject *)self)->dm_dynlib,name,result)) result = NULL;
DEE_COMPILER_MSVC_WARNING_POP
 DeeDexModule_Release((DeeDexModuleObject *)self);
 if DEE_UNLIKELY(!result) {
  DeeError_SetStringf(&DeeErrorType_AttributeError,
                     "Native export %q not found in dex %k",
                      name,self);
 }
 return result;
}

DEE_A_EXEC DEE_A_RET_NOEXCEPT(NULL) void *DeeDexModule_TryGetNativeAddress(
 DEE_A_INOUT_OBJECT(DeeDexModuleObject) *self, DEE_A_IN_Z char const *name) {
 void *result; DEE_ASSERT(name);
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 if DEE_UNLIKELY(DeeDexModule_Acquire((DeeDexModuleObject *)self,
  DEE_DEXCONTEXT_INITIALIZE_REASON_NATIVE) != 0) { DeeError_Handled(); return NULL; }
 if DEE_UNLIKELY(!DeeSysDynlib_TryImport(&((DeeDexModuleObject *)self)->dm_dynlib,name,result)) result = NULL;
 DeeDexModule_Release((DeeDexModuleObject *)self);
 return result;
}



DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeDexModule_GetAttrString(
 DEE_A_INOUT_OBJECT(DeeDexModuleObject) *self, DEE_A_IN_Z char const *name) {
 DeeObject *result; struct DeeDexExportDef *xiter;
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 DEE_ASSERT(name);
 if DEE_UNLIKELY(DeeDexModule_Acquire((DeeDexModuleObject *)self,
  DEE_DEXCONTEXT_INITIALIZE_REASON_GETTER) != 0) return NULL;
 xiter = ((DeeDexModuleObject *)self)->dm_exports;
 while (xiter->dxe_name) {
  if (strcmp(xiter->dxe_name,name) == 0) {
   switch (xiter->dxe_kind) {
    // found it
    case DEE_DEX_EXPORT_TYPE_OBJECT: {
     DEE_ASSERT(DeeObject_Check(xiter->dxe_object));
     Dee_INCREF(result = xiter->dxe_object);
    } goto end;
    case DEE_DEX_EXPORT_TYPE_GENERATOR: {
     if DEE_UNLIKELY(DeeNativeMutex_Acquire(&((DeeDexModuleObject *)self)->dm_cachelock) != 0) goto err;
     if (xiter->dxe_generator.dxe_gn_cache) {
      // Use existing cache
      Dee_INCREF(result = xiter->dxe_generator.dxe_gn_cache);
     } else {
      DEE_ASSERT(xiter->dxe_generator.dxe_gn_ctor);
      result = (*xiter->dxe_generator.dxe_gn_ctor)();
      Dee_XINCREF(xiter->dxe_generator.dxe_gn_cache = result);
     }
     DeeNativeMutex_ReleaseNoexcept(&((DeeDexModuleObject *)self)->dm_cachelock);
    } goto end;
    case DEE_DEX_EXPORT_TYPE_GETSET: {
     if DEE_LIKELY(xiter->dxe_getset.dxe_gs_get) {
      result = (*xiter->dxe_getset.dxe_gs_get)();
     } else goto noread;
    } goto end;
    case DEE_DEX_EXPORT_TYPE_OBJECTINL: {
     DEE_ASSERT(DeeObject_Check(&xiter->dxe_objectinl));
     Dee_INCREF(result = &xiter->dxe_objectinl);
    } goto end;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
    case DEE_DEX_EXPORT_TYPE_NATIVE_FUNCTION: {
     result = DeeLValue_Newf(xiter->dxe_native_function.dxe_nf_function,
                             xiter->dxe_native_function.dxe_nf_type);
    } goto end;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
    case DEE_DEX_EXPORT_TYPE_INT8: result = DeeInt8_New(xiter->dxe_const.dxe_c_i8); goto end;
    case DEE_DEX_EXPORT_TYPE_INT16: result = DeeInt16_New(xiter->dxe_const.dxe_c_i16); goto end;
    case DEE_DEX_EXPORT_TYPE_INT32: result = DeeInt32_New(xiter->dxe_const.dxe_c_i32); goto end;
    case DEE_DEX_EXPORT_TYPE_INT64: result = DeeInt64_New(xiter->dxe_const.dxe_c_i64); goto end;
    case DEE_DEX_EXPORT_TYPE_UINT8: result = DeeUInt8_New(xiter->dxe_const.dxe_c_ui8); goto end;
    case DEE_DEX_EXPORT_TYPE_UINT16: result = DeeUInt16_New(xiter->dxe_const.dxe_c_ui16); goto end;
    case DEE_DEX_EXPORT_TYPE_UINT32: result = DeeUInt32_New(xiter->dxe_const.dxe_c_ui32); goto end;
    case DEE_DEX_EXPORT_TYPE_UINT64: result = DeeUInt64_New(xiter->dxe_const.dxe_c_ui64); goto end;
    default:noread: {
     DeeError_SetStringf(&DeeErrorType_AttributeError,
                         "Export %q of dex %k cannot be read",
                         name,self);
     result = NULL;
    } goto end;
   }
  }
  ++xiter;
 }
 DeeError_SetStringf(&DeeErrorType_AttributeError,
                     "Export %q not found in dex %k",
                     name,self);
err: result = NULL;
end: DeeDexModule_Release((DeeDexModuleObject *)self);
 return result;
}

DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDexModule_HasAttrString(
 DEE_A_INOUT_OBJECT(DeeDexModuleObject) const *self, DEE_A_IN_Z char const *name) {
 int result; struct DeeDexExportDef *xiter;
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 DEE_ASSERT(name);
 if DEE_UNLIKELY(DeeDexModule_Acquire((DeeDexModuleObject *)self,
  DEE_DEXCONTEXT_INITIALIZE_REASON_HASXPT) != 0) return -1;
 xiter = ((DeeDexModuleObject *)self)->dm_exports;
 while (xiter->dxe_name) {
  if (strcmp(xiter->dxe_name,name) == 0) { result = 1; goto end; }
  ++xiter;
 }
 result = 0;
end: DeeDexModule_Release((DeeDexModuleObject *)self);
 return result;
}


DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDexModule_DelAttrString(
 DEE_A_INOUT_OBJECT(DeeDexModuleObject) *self, DEE_A_IN_Z char const *name) {
 int result; struct DeeDexExportDef *xiter;
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 DEE_ASSERT(name);
 if DEE_UNLIKELY(DeeDexModule_Acquire((DeeDexModuleObject *)self,
  DEE_DEXCONTEXT_INITIALIZE_REASON_DELETE) != 0) return -1;
 xiter = ((DeeDexModuleObject *)self)->dm_exports;
 while (xiter->dxe_name) {
  if (strcmp(xiter->dxe_name,name) == 0) {
   switch (xiter->dxe_kind) {
    // found it
    case DEE_DEX_EXPORT_TYPE_GENERATOR: {
     DeeObject *existing_cache;
     if DEE_UNLIKELY(DeeNativeMutex_Acquire(&((DeeDexModuleObject *)self)->dm_cachelock) != 0) goto err;
     if ((existing_cache = xiter->dxe_generator.dxe_gn_cache) != NULL) {
      xiter->dxe_generator.dxe_gn_cache = NULL;
      // Delete existing cache
      if (xiter->dxe_generator.dxe_gn_dtor) {
       (*xiter->dxe_generator.dxe_gn_dtor)(existing_cache);
      } else Dee_DECREF(existing_cache);
      result = 0;
     } else result = 1;
     DeeNativeMutex_ReleaseNoexcept(&((DeeDexModuleObject *)self)->dm_cachelock);
    } goto end;
    case DEE_DEX_EXPORT_TYPE_GETSET: {
     if DEE_LIKELY(xiter->dxe_getset.dxe_gs_del) {
      result = (*xiter->dxe_getset.dxe_gs_del)();
     } else goto nodel;
    } goto end;
    default:nodel: {
     DeeError_SetStringf(&DeeErrorType_AttributeError,
                         "Export %q of dex %k cannot be deleted",
                         name,self);
     result = -1;
    } goto end;
   }
  }
  ++xiter;
 }
 DeeError_SetStringf(&DeeErrorType_AttributeError,
                     "Export %q not found in dex %k",
                     name,self);
err: result = -1;
end: DeeDexModule_Release((DeeDexModuleObject *)self);
 return result;
}

DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeDexModule_SetAttrString(
 DEE_A_INOUT_OBJECT(DeeDexModuleObject) *self, DEE_A_IN_Z char const *name, DEE_A_INOUT DeeObject *v) {
 int result; struct DeeDexExportDef *xiter;
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 DEE_ASSERT(name);
 if DEE_UNLIKELY(DeeDexModule_Acquire((DeeDexModuleObject *)self,
  DEE_DEXCONTEXT_INITIALIZE_REASON_DELETE) != 0) return -1;
 xiter = ((DeeDexModuleObject *)self)->dm_exports;
 while (xiter->dxe_name) {
  if (strcmp(xiter->dxe_name,name) == 0) {
   switch (xiter->dxe_kind) {
    // found it
    case DEE_DEX_EXPORT_TYPE_GETSET: {
     if DEE_LIKELY(xiter->dxe_getset.dxe_gs_set) {
      result = (*xiter->dxe_getset.dxe_gs_set)(v);
     } else goto noset;
    } goto end;
    default:noset: {
     DeeError_SetStringf(&DeeErrorType_AttributeError,
                         "Export %q of dex %k cannot be set",
                         name,self);
     result = -1;
    } goto end;
   }
  }
  ++xiter;
 }
 DeeError_SetStringf(&DeeErrorType_AttributeError,
                     "Export %q not found in dex %k",
                     name,self);
/*err:*/ result = -1;
end: DeeDexModule_Release((DeeDexModuleObject *)self);
 return result;
}

//////////////////////////////////////////////////////////////////////////
// Returns the address of a natively exported
// variable/function from a given dex (GetProcAddress / dlsym)

DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeDexModule_Name(DEE_A_IN_OBJECT(DeeDexModuleObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 DeeReturn_NEWREF((DeeObject *)((DeeDexModuleObject *)self)->dm_name);
}
DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeDexModule_File(DEE_A_IN_OBJECT(DeeDexModuleObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 return DeeSysDynlibN_Name(&((DeeDexModuleObject *)self)->dm_dynlib);
}
DEE_A_RET_WUNUSED Dee_uint16_t DeeDexModule_Flags(
 DEE_A_IN_OBJECT(DeeDexModuleObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 return (Dee_uint16_t)DeeAtomic16_Load(((DeeDexModuleObject *)self)->dm_state,memory_order_seq_cst);
}

DEE_A_EXEC Dee_size_t DeeDexModule_CollectMemoryEx(
 DEE_A_INOUT DeeDexModuleObject *self, DEE_A_IN Dee_uint32_t reason) {
 DeeDexContext ctx;
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 if (DeeDexModule_TryAcquire(self) != DEE_DEX_TRYACQUIRE_SUCCESS) return 0;
 if (!self->dm_main) { ctx.dc_collectmem.ccm_total = 0; goto end; }
 ctx.dc_kind = DEE_DEXCONTEXTKIND_COLLECTMEM;
 ctx.dc_collectmem.ccm_reason = reason;
 ctx.dc_collectmem.dc_self = (DeeObject *)self;
 ctx.dc_collectmem.ccm_total = 0;
 if ((*self->dm_main)(&ctx) != 0) {
  DeeError_Print("[ignored] Error occurred during collecting memory from dex",1);
  ctx.dc_collectmem.ccm_total = 0; // Don't trust 'ctx.dc_collectmem.ccm_total'
 }
end:
 DeeDexModule_Release(self);
 return ctx.dc_collectmem.ccm_total;
}

DEE_A_EXEC Dee_size_t DeeDex_CollectMemoryEx(DEE_A_IN Dee_uint32_t reason) {
 DeeDexModuleObject *dex; DeeHashMap_TRAVERSE_SAFE_VARS;
 struct _DeeHashMapNode *node; Dee_size_t result; result = 0;
 DeeAtomicMutex_Acquire(&_dex_modules_lock);
 DeeHashMap_TRAVERSE_SAFE(node,&_dex_modules_map) {
  Dee_INCREF(dex = (DeeDexModuleObject *)node->hmn_item);
  DeeAtomicMutex_Release(&_dex_modules_lock);
  result += DeeDexModule_CollectMemoryEx(dex,reason);
  Dee_DECREF(dex);
  DeeAtomicMutex_Acquire(&_dex_modules_lock);
 }
 DeeAtomicMutex_Release(&_dex_modules_lock);
 return result;
}









//////////////////////////////////////////////////////////////////////////
// DexModule VTable
DeeObject *DEE_CALL _deedexmodule_tp_attr_get(DeeDexModuleObject *self, DeeObject *attr) {
 return DeeDexModule_GetAttrString((DeeObject *)self,DeeString_STR(attr));
}
int DEE_CALL _deedexmodule_tp_attr_del(DeeDexModuleObject *self, DeeObject *attr) {
 return DeeDexModule_DelAttrString((DeeObject *)self,DeeString_STR(attr));
}
int DEE_CALL _deedexmodule_tp_attr_set(DeeDexModuleObject *self, DeeObject *attr, DeeObject *v) {
 return DeeDexModule_SetAttrString((DeeObject *)self,DeeString_STR(attr),v);
}

static void DEE_CALL _deedexmodule_tp_dtor(DeeDexModuleObject *self) {
 DeeDexModule_Shutdown(self,DEE_DEXCONTEXT_FINALIZE_REASON_DESTROY);
 DeeSysDynlibN_Quit(&self->dm_dynlib);
 Dee_DECREF(self->dm_name);
 DeeNativeMutex_Quit(&self->dm_cachelock);
}

DEE_VISIT_PROC(_deedexmodule_tp_visit,DeeDexModuleObject *self) {
 struct DeeDexExportDef *xiter; DeeDexContext ctx;
 DEE_ASSERT(DeeObject_Check(self) && DeeDexModule_Check(self));
 Dee_VISIT(self->dm_name);
 if (DeeDexModule_TryAcquire(self) != DEE_DEX_TRYACQUIRE_SUCCESS) return;
 if (!self->dm_main) goto end;
 ctx.dc_kind = DEE_DEXCONTEXTKIND_VISIT;
 ctx.dc_visit.dc_self = (DeeObject *)self;
 ctx.dc_visit.cv_visit = visit;
 ctx.dc_visit.cv_closure = closure;
 if ((*self->dm_main)(&ctx) != 0) {
  DeeError_Print("Error occurred in dex visit callback.\n"
                 "Even though it (apparently) worked this time,\n"
                 "you can very easily cause a deadlock if you keep doing this.\n"
                 ">> Please change your code to not cause an error when called\n"
                 "   with 'DEE_DEXCONTEXTKIND_VISIT'",1);
 }
end:
 xiter = self->dm_exports;
 while (xiter->dxe_name) {
  if (xiter->dxe_kind == DEE_DEX_EXPORT_TYPE_GENERATOR)
   Dee_XVISIT(xiter->dxe_generator.dxe_gn_cache);
  ++xiter;
 }

 DeeDexModule_Release(self);
}


static DeeObject *DEE_CALL _deedexmodule_tp_str(DeeDexModuleObject *self) {
 return DeeString_Cast((DeeObject *)self->dm_name);
}
static DeeObject *DEE_CALL _deedexmodule_tp_repr(DeeDexModuleObject *self) {
 return DeeString_Newf("<dex(%r,%R)>",self->dm_name,
                       DeeDexModule_File((DeeObject *)self));
}


static DeeObject *DEE_CALL _deedexmoduleclass_search_order_get(
 DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) {
 char buffer[DEEDEX_SEARCHORDER_INTERNAL_MAXSIZE];
 return DeeString_NewWithLength(DeeDex_GetSearchMode(buffer,sizeof(buffer)),buffer);
}
static int DEE_CALL _deedexmoduleclass_search_order_del(
 DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) {
 return DeeDex_SetSearchOrder(NULL);
}
static int DEE_CALL _deedexmoduleclass_search_order_set(
 DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure), DeeObject *v) {
 if (DeeError_TypeError_CheckTypeExact(v,&DeeString_Type) != 0) return -1;
 return DeeDex_SetSearchOrder(DeeString_STR(v));
}
static DeeObject *DEE_CALL _deedexmoduleclass_nameof(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeDexModuleObject *dex;
 if (DeeTuple_Unpack(args,"o:nameof",&dex) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&dex,&DeeDexModule_Type) != 0) return NULL;
 return DeeDexModule_Name((DeeObject *)dex);
}
static DeeObject *DEE_CALL _deedexmoduleclass_fileof(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeDexModuleObject *dex;
 if (DeeTuple_Unpack(args,"o:fileof",&dex) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&dex,&DeeDexModule_Type) != 0) return NULL;
 return DeeDexModule_File((DeeObject *)dex);
}

static struct DeeMemberDef _deedexmodule_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("open",object,&DeeBuiltinFunction___builtin_dex),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMethodDef _deedexmodule_tp_class_methods[] = {
 DEE_METHODDEF_CONST_v100("nameof",member(&_deedexmoduleclass_nameof),
  "(dexmodule dex) -> string\n"
  "@return: The name of a given dex module @dex"),
 DEE_METHODDEF_CONST_v100("fileof",member(&_deedexmoduleclass_fileof),
  "(dexmodule dex) -> string\n"
  "(dexmodule dex) -> string.wide\n"
  "@return: The filename of a given dex module @dex"),
 DEE_METHODDEF_END_v100
};

static DeeObject *DEE_CALL _deedexmoduleclass_search_path_get(
 DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) {
 DeeReturn_NEWREF(DeeDex_SearchPath);
}
static int DEE_CALL _deedexmoduleclass_search_path_del(
 DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) {
 DeeObject *default_path;
 if ((default_path = DeeDex_GetDefaultSearchPath()) == NULL) return -1;
 DeeList_MoveAssign(DeeDex_SearchPath,default_path);
 Dee_DECREF(default_path);
 return 0;
}
static int DEE_CALL _deedexmoduleclass_search_path_set(
 DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure), DeeObject *v) {
 return DeeList_AssignSequence(DeeDex_SearchPath,v);
}

static struct DeeGetSetDef _deedexmodule_tp_class_getsets[] = {
 DEE_GETSETDEF_v100("search_path",
  member(&_deedexmoduleclass_search_order_get),
  member(&_deedexmoduleclass_search_order_del),
  member(&_deedexmoduleclass_search_order_set),"-> list\n"
  "\tConfigure the custom search path, as enabled by #('D') in #dexmodule.search_order\n"
  "\tDeleting this attribute will restore the default search path.\n"
  "\tNOTE: The returned list can be modified directly and changes will be applied immediatly.\n"),
#ifdef DEE_PLATFORM_WINDOWS
 DEE_GETSETDEF_v100("search_order",
  member(&_deedexmoduleclass_search_order_get),
  member(&_deedexmoduleclass_search_order_del),
  member(&_deedexmoduleclass_search_order_set),"-> string\n"
  "\tConfigure the search order for dex modules.\n"
  "\tThe order is a string marker characters, using one of each at most:\n"
  "\t#('V'): Ignored under windows\n"
  "\t#('D'): Searches all paths from #dexmodule.search_path\n"
  "\t#('C'): Searches #(fs::getcwd())\n"
  "\t#('X'): Searches #(fs::path::head(fs::readlink(\"/proc/self/exe\")))\n"
  "\t#('P'): Searches all paths from #($PATH)\n"
  "\tThe default search order is #(\"" DEEDEX_SEARCHORDER_DEFAULT "\").\n"
  "\tDeleting this attribute will restore the default search order."),
#else
 DEE_GETSETDEF_v100("search_order",
  member(&_deedexmoduleclass_search_order_get),
  member(&_deedexmoduleclass_search_order_del),
  member(&_deedexmoduleclass_search_order_set),"-> string\n"
  "\tConfigure the search order for dex modules.\n"
  "\tThe order is a string marker characters, using one of each at most:\n"
  "\t#('V'): Searches #(\"" DEE_DEFAULT_HOMEPATH "/dex." DEE_PP_STR(DEE_VERSION_API) "\")\n"
  "\t#('D'): Searches all paths from #dexmodule.search_path\n"
  "\t#('C'): Searches #(fs::getcwd())\n"
  "\t#('X'): Searches #(fs::path::head(fs::readlink(\"/proc/self/exe\")))\n"
  "\t#('P'): Searches all paths from #($PATH)\n"
  "\tThe default search order is #(\"" DEEDEX_SEARCHORDER_DEFAULT "\").\n"
  "\tDeleting this attribute will restore the default search order."),
#endif
 DEE_GETSETDEF_END_v100
};


static struct DeeTypeMarshal _deedexmodule_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALID_DEXMODULE),member(NULL),member(NULL)); // Special marshal type

DeeTypeObject DeeDexModule_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("dexmodule"),member(
  "\tDexmodules are used to extend deemon dynamically without the need of everything being implemented by the core.\n"
  "\tA builtin function #__builtin_dex is provided by the language, that can be used to load a dex module.\n"
  "\t'Dex' is an abbreviation that stands for deemon-extension."),
  // 'DEE_TYPE_FLAG_NO_LIFETIME_EFFECT' is required for unused variable optimization
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(
  sizeof(DeeDexModuleObject),null,null,null,null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deedexmodule_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deedexmodule_tp_str),
  member(&_deedexmodule_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deedexmodule_tp_visit),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,
  member(&_deegenericsameob_tp_hash)),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deegenericsameob_tp_cmp_lo),
  member(&_deegenericsameob_tp_cmp_le),
  member(&_deegenericsameob_tp_cmp_eq),
  member(&_deegenericsameob_tp_cmp_ne),
  member(&_deegenericsameob_tp_cmp_gr),
  member(&_deegenericsameob_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(
  member(&_deedexmodule_tp_attr_get),
  member(&_deedexmodule_tp_attr_del),
  member(&_deedexmodule_tp_attr_set),null,null,null,
  member(_deedexmodule_tp_class_members),
  member(_deedexmodule_tp_class_getsets),
  member(_deedexmodule_tp_class_methods)),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deedexmodule_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};



DEE_DECL_END

#ifndef __INTELLISENSE__
#define NATIVE
#include "dex.import.inl"
#include "dex.import.inl"
#ifdef DEE_PLATFORM_WINDOWS
#define WIDE
#include "dex.locate.inl"
#endif
#include "dex.locate.inl"
#endif

#endif /* !GUARD_DEEMON_DEX_C */
