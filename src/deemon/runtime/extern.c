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
#ifndef GUARD_DEEMON_EXTERN_C
#define GUARD_DEEMON_EXTERN_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/runtime/extern.h>
#include <deemon/string.h>
#include <deemon/memberdef.h>
#include <deemon/integer.h>
#include <deemon/none.h>
#include <deemon/error.h>
#include <deemon/fs_api.h>
#include <deemon/bool.h>
#include <deemon/optional/hash.h>
#include <deemon/optional/hashmap.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_visit.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>
#include <deemon/sys/sysdynlib.h>
#include DEE_INCLUDE_MEMORY_API()

// */ (nano...)


DEE_DECL_BEGIN

struct DeeSharedLibObject {
 DEE_OBJECT_HEAD
 struct DeeSysDynlibN sl_lib; /*< System synlib. */
};


#if DEE_CONFIG_RUNTIME_HAVE_EXTERN
DEE_A_RET_OBJECT_EXCEPT_REF(DeeLValueObject) *DeeSharedLib_LoadExtern(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *object_name,
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *object_type) {
 void *import_addr;
 if DEE_UNLIKELY((object_type = DeeType_LValue(object_type)) == NULL) return NULL;
 if DEE_UNLIKELY((object_name = DeeString_Cast(object_name)) == NULL) return NULL;
 import_addr = DeeSharedLib_PoolImportObject(filename,DeeString_STR(object_name));
 Dee_DECREF(object_name);
 if DEE_UNLIKELY(!import_addr) return NULL;
 return DeeLValue_New(object_type,import_addr);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeLValueObject) *DeeSharedLib_LoadAutoExtern(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *object_name,
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *object_type) {
 // TODO: In the future, this is supposed to implement
 //       automatic linkage with external libraries.
 (void)object_name,object_type;
 DeeError_NotImplemented_str("DeeSharedLib_LoadAutoExtern()");
 return NULL;
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */


DEE_A_RET_OBJECT_EXCEPT_REF(DeeSharedLibObject) *DeeSharedLib_Utf8New(
 DEE_A_IN_Z Dee_Utf8Char const *filename) {
 DeeSharedLibObject *result; DEE_ASSERT(filename);
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeSharedLibObject,"dll(%s)",filename)) != NULL) {
  if (*filename) DeeSysDynlibN_Utf8InitFromFilename(&result->sl_lib,filename,goto err_free);
  else DeeSysDynlibN_InitFromSelf(&result->sl_lib,goto err_free);
  DeeObject_INIT(result,&DeeSharedLib_Type);
 }
 return (DeeObject *)result;
err_free: DeeObject_Free(result);
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeSharedLibObject) *DeeSharedLib_WideNew(
 DEE_A_IN_Z Dee_WideChar const *filename) {
 DeeSharedLibObject *result; DEE_ASSERT(filename);
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeSharedLibObject,"dll(%ls)",filename)) != NULL) {
  if (*filename) DeeSysDynlibN_WideInitFromFilename(&result->sl_lib,filename,goto err_free);
  else DeeSysDynlibN_InitFromSelf(&result->sl_lib,goto err_free);
  DeeObject_INIT(result,&DeeSharedLib_Type);
 }
 return (DeeObject *)result;
err_free: DeeObject_Free(result);
 return NULL;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeSharedLibObject) *DeeSharedLib_NewObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename) {
 DeeSharedLibObject *result; DeeUtf8StringObject *u8filename;
 DEE_ASSERT(DeeObject_Check(filename));
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeSharedLibObject,"dll(%p)",filename)) != NULL) {
  if (DeeWideString_Check(filename)) {
   if (DeeWideString_SIZE(filename)) DeeSysDynlibN_WideInitFromFilenameObject(&result->sl_lib,filename,goto err_free);
   else DeeSysDynlibN_InitFromSelf(&result->sl_lib,goto err_free);
  } else {
   if ((u8filename = (DeeUtf8StringObject *)DeeUtf8String_Cast(filename)) == NULL) goto err_free;
   if (DeeUtf8String_SIZE(u8filename)) DeeSysDynlibN_Utf8InitFromFilenameObject(&result->sl_lib,u8filename,goto err_freeu8);
   else DeeSysDynlibN_InitFromSelf(&result->sl_lib,goto err_freeu8);
   Dee_DECREF(u8filename);
  }
  DeeObject_INIT(result,&DeeSharedLib_Type);
 }
 return (DeeObject *)result;
err_freeu8: Dee_DECREF(u8filename);
err_free: DeeObject_Free(result);
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeSharedLib_Name(DEE_A_IN_OBJECT(DeeSharedLibObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSharedLib_Check(self));
 return DeeSysDynlibN_Name(&((DeeSharedLibObject *)self)->sl_lib);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeSharedLib_Utf8Name(DEE_A_IN_OBJECT(DeeSharedLibObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSharedLib_Check(self));
 return DeeSysDynlibN_Utf8Name(&((DeeSharedLibObject *)self)->sl_lib);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeSharedLib_WideName(DEE_A_IN_OBJECT(DeeSharedLibObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSharedLib_Check(self));
 return DeeSysDynlibN_WideName(&((DeeSharedLibObject *)self)->sl_lib);
}

DEE_A_RET_EXCEPT(NULL) void *DeeSharedLib_Import(
 DEE_A_IN_OBJECT(DeeSharedLibObject) const *self,
 DEE_A_IN_Z char const *name) {
 void *result;
 DeeSysDynlib_Import(&((DeeSharedLibObject *)self)->sl_lib,name,result,{return NULL;});
 return result;
}
DEE_COMPILER_MSVC_WARNING_PUSH(4054)
DEE_A_RET_NOEXCEPT(NULL) void *DeeSharedLib_TryImport(
 DEE_A_IN_OBJECT(DeeSharedLibObject) const *self, DEE_A_IN_Z char const *name) {
 void *result; DEE_ASSERT(DeeObject_Check(self) && DeeSharedLib_Check(self));
 if (!DeeSysDynlib_TryImport(&((DeeSharedLibObject *)self)->sl_lib,name,result)) return NULL;
 return result;
}
DEE_COMPILER_MSVC_WARNING_POP
DEE_A_RET_WUNUSED void *DeeSharedLib_Handle(
 DEE_A_IN_OBJECT(DeeSharedLibObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSharedLib_Check(self));
#ifdef DEE_SYSDYNLIB_USES_WIN32
 return (void *)((DeeSharedLibObject *)self)->sl_lib.sd_lib;
#elif defined(DEE_SYSDYNLIB_USES_DLFCN)
 return         ((DeeSharedLibObject *)self)->sl_lib.sd_lib;
#else
 //DeeError_NotImplemented_str("Can't retrieve dynlib handle on this platform");
 return NULL;
#endif
}




static struct DeeAtomicMutex _dll_lock = DeeAtomicMutex_INIT();
static struct DeeHashMap _dll_pool = DeeHashMap_INIT();

DEE_A_RET_NOEXCEPT(NULL) void *DeeSharedLib_Utf8PoolTryImport(
 DEE_A_IN_Z Dee_Utf8Char const *filename, DEE_A_IN_Z char const *name) {
 DeeObject *dll;
 if DEE_UNLIKELY((dll = DeeSharedLib_Utf8PoolLoad(filename)) == NULL) { DeeError_Handled(); return NULL; }
 return DeeSharedLib_TryImport(dll,name);
}
DEE_A_RET_NOEXCEPT(NULL) void *DeeSharedLib_WidePoolTryImport(
 DEE_A_IN_Z Dee_WideChar const *filename, DEE_A_IN_Z char const *name) {
 DeeObject *dll;
 if DEE_UNLIKELY((dll = DeeSharedLib_WidePoolLoad(filename)) == NULL) { DeeError_Handled(); return NULL; }
 return DeeSharedLib_TryImport(dll,name);
}
DEE_A_RET_NOEXCEPT(NULL) void *DeeSharedLib_PoolTryImportObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename, DEE_A_IN_Z char const *name) {
 DeeObject *dll;
 if DEE_UNLIKELY((dll = DeeSharedLib_PoolLoadObject(filename)) == NULL) { DeeError_Handled(); return NULL; }
 return DeeSharedLib_TryImport(dll,name);
}
DEE_A_RET_EXCEPT(NULL) void *DeeSharedLib_Utf8PoolImport(
 DEE_A_IN_Z Dee_Utf8Char const *filename, DEE_A_IN_Z char const *name) {
 DeeObject *dll;
 if DEE_UNLIKELY((dll = DeeSharedLib_Utf8PoolLoad(filename)) == NULL) return NULL;
 return DeeSharedLib_Import(dll,name);
}
DEE_A_RET_EXCEPT(NULL) void *DeeSharedLib_WidePoolImport(
 DEE_A_IN_Z Dee_WideChar const *filename, DEE_A_IN_Z char const *name) {
 DeeObject *dll;
 if DEE_UNLIKELY((dll = DeeSharedLib_WidePoolLoad(filename)) == NULL) return NULL;
 return DeeSharedLib_Import(dll,name);
}

DEE_A_RET_EXCEPT(NULL) void *DeeSharedLib_PoolImportObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename, DEE_A_IN_Z char const *name) {
 DeeObject *dll;
 if DEE_UNLIKELY((dll = DeeSharedLib_PoolLoadObject(filename)) == NULL) return NULL;
 return DeeSharedLib_Import(dll,name);
}

DEE_A_RET_OBJECT_EXCEPT(DeeSharedLibObject) *DeeSharedLib_PoolLoadObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename) {
 DeeObject *result,*utf8_filename;
 Dee_hash_t filename_hash; int temp;
 DEE_ASSERT(DeeObject_Check(filename));
 if DEE_UNLIKELY((utf8_filename = DeeUtf8String_Cast(filename)) == NULL) return NULL;
 filename_hash = DeeHash_Memory(DeeUtf8String_STR(utf8_filename),DeeUtf8String_SIZE(utf8_filename)*sizeof(Dee_Utf8Char),DEE_HASH_START);
 DeeAtomicMutex_Acquire(&_dll_lock);
 if DEE_UNLIKELY((temp = DeeHashMap_GetItemExStringWithKnownBucket(
  &_dll_pool,DeeUtf8String_STR(utf8_filename),filename_hash,&result)) < 0) {
  DeeAtomicMutex_Release(&_dll_lock);
  return NULL;
 }
 if DEE_LIKELY(DEE_LIKELY(temp == 0)) {
  DeeAtomicMutex_Release(&_dll_lock);
  DEE_ASSERT(!DeeObject_IS_UNIQUE(result));
  Dee_DECREF(result);
 } else {
  DeeAtomicMutex_Release(&_dll_lock);
  result = DeeSharedLib_NewObject(filename);
  if DEE_LIKELY(result != NULL) {
   DeeObject *old;
   DeeAtomicMutex_Acquire(&_dll_lock);
   if DEE_UNLIKELY((temp = DeeHashMap_GetItemExStringWithKnownBucket(
    &_dll_pool,DeeString_STR(utf8_filename),filename_hash,&old)) < 0) {
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    return NULL;
   }
   if DEE_UNLIKELY(temp == 0) {
    DEE_ASSERT(DeeObject_Check(old) && DeeSharedLib_Check(old));
    // Someone must have been faster than us and did
    // everything while we weren't locking the dll
    // >> Drop out new dll and return the old one
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    Dee_DECREF(utf8_filename);
    DEE_ASSERT(!DeeObject_IS_UNIQUE(old));
    Dee_DECREF(old);
    return old;
   } else if DEE_UNLIKELY(DeeHashMap_SetItemStringWithKnownBucket(
    &_dll_pool,DeeString_STR(utf8_filename),filename_hash,result) != 0) {
    // The insertion into the dict failed
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    Dee_DECREF(utf8_filename);
    return NULL;
   }
   DeeAtomicMutex_Release(&_dll_lock);
   Dee_DECREF(result); // Object is kep alive through the dict
  }
 }
 Dee_DECREF(utf8_filename);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT(DeeSharedLibObject) *DeeSharedLib_Utf8PoolLoad(
 DEE_A_IN_Z Dee_Utf8Char const *filename) {
 DeeObject *result;
 Dee_hash_t filename_hash; int temp;
 DEE_ASSERT(filename);
 filename_hash = DeeHash_String(filename,DEE_HASH_START);
 DeeAtomicMutex_Acquire(&_dll_lock);
 if DEE_UNLIKELY((temp = DeeHashMap_GetItemExStringWithKnownBucket(
  &_dll_pool,filename,filename_hash,&result)) < 0) {
  DeeAtomicMutex_Release(&_dll_lock);
  return NULL;
 }
 if DEE_LIKELY(temp == 0) {
  DeeAtomicMutex_Release(&_dll_lock);
  DEE_ASSERT(!DeeObject_IS_UNIQUE(result));
  Dee_DECREF(result);
 } else {
  DeeAtomicMutex_Release(&_dll_lock);
  result = DeeSharedLib_Utf8New(filename);
  if DEE_LIKELY(result != NULL) {
   DeeObject *old;
   DeeAtomicMutex_Acquire(&_dll_lock);
   if DEE_UNLIKELY((temp = DeeHashMap_GetItemExStringWithKnownBucket(
    &_dll_pool,filename,filename_hash,&old)) < 0) {
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    return NULL;
   }
   if DEE_UNLIKELY(temp == 0) {
    DEE_ASSERT(DeeObject_Check(old) && DeeSharedLib_Check(old));
    // Someone must have been faster than us and did
    // everything while we weren't locking the dll
    // >> Drop out new dll and return the old one
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    DEE_ASSERT(!DeeObject_IS_UNIQUE(old));
    Dee_DECREF(old);
    return old;
   } else if DEE_UNLIKELY(DeeHashMap_SetItemStringWithKnownBucket(
    &_dll_pool,filename,filename_hash,result) != 0) {
    // The insertion into the dict failed
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    return NULL;
   }
   DeeAtomicMutex_Release(&_dll_lock);
   Dee_DECREF(result); // Object is kept alive through the dict
  }
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT(DeeSharedLibObject) *DeeSharedLib_WidePoolLoad(
 DEE_A_IN_Z Dee_WideChar const *filename) {
 DeeObject *result,*utf8_filename;
 Dee_hash_t filename_hash; int temp;
 DEE_ASSERT(DeeObject_Check(filename));
 if DEE_UNLIKELY((utf8_filename = DeeUtf8String_FromWideString(filename)) == NULL) return NULL;
 filename_hash = DeeHash_Memory(DeeUtf8String_STR(utf8_filename),
                                DeeUtf8String_SIZE(utf8_filename)*
                                sizeof(Dee_Utf8Char),DEE_HASH_START);
 DeeAtomicMutex_Acquire(&_dll_lock);
 if DEE_UNLIKELY((temp = DeeHashMap_GetItemExStringWithKnownBucket(
  &_dll_pool,DeeUtf8String_STR(utf8_filename),filename_hash,&result)) < 0) {
  DeeAtomicMutex_Release(&_dll_lock);
  return NULL;
 }
 if DEE_LIKELY(temp == 0) {
  DeeAtomicMutex_Release(&_dll_lock);
  DEE_ASSERT(!DeeObject_IS_UNIQUE(result));
  Dee_DECREF(result);
 } else {
  DeeAtomicMutex_Release(&_dll_lock);
#ifdef DEE_PLATFORM_WINDOWS
  // Prefer widestring on windows
  result = DeeSharedLib_WideNew(filename);
#else
  result = DeeSharedLib_Utf8New(DeeUtf8String_STR(utf8_filename));
#endif
  if DEE_LIKELY(result != NULL) {
   DeeObject *old;
   DeeAtomicMutex_Acquire(&_dll_lock);
   if DEE_UNLIKELY((temp = DeeHashMap_GetItemExStringWithKnownBucket(
    &_dll_pool,DeeString_STR(utf8_filename),filename_hash,&old)) < 0) {
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    return NULL;
   }
   if DEE_UNLIKELY(temp == 0) {
    DEE_ASSERT(DeeObject_Check(old) && DeeSharedLib_Check(old));
    // Someone must have been faster than us and did
    // everything while we weren't locking the dll
    // >> Drop out new dll and return the old one
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    Dee_DECREF(utf8_filename);
    DEE_ASSERT(!DeeObject_IS_UNIQUE(old));
    Dee_DECREF(old);
    return old;
   } else if DEE_UNLIKELY(DeeHashMap_SetItemStringWithKnownBucket(
    &_dll_pool,DeeString_STR(utf8_filename),filename_hash,result) != 0) {
    // The insertion into the dict failed
    DeeAtomicMutex_Release(&_dll_lock);
    Dee_DECREF(result);
    Dee_DECREF(utf8_filename);
    return NULL;
   }
   DeeAtomicMutex_Release(&_dll_lock);
   Dee_DECREF(result); // Object is kep alive through the dict
  }
 }
 Dee_DECREF(utf8_filename);
 return result;
}

void DeeSharedLib_Shutdown(void) {
 DeeAtomicMutex_Acquire(&_dll_lock);
 DeeHashMap_Clear(&_dll_pool);
 DeeAtomicMutex_Release(&_dll_lock);
}

static void _deesharedlib_tp_dtor(DeeSharedLibObject *self) { DeeSysDynlibN_Quit(&self->sl_lib); }
static DeeObject *_deesharedlib_tp_cmp_lo(DeeSharedLibObject *self, DeeSharedLibObject *right) { if DEE_UNLIKELY((right = (DeeSharedLibObject *)DeeObject_GetInstance((DeeObject *)right,&DeeSharedLib_Type)) == NULL) return NULL; DeeReturn_Bool(DeeSysDynlib_CompareLo(&self->sl_lib,&right->sl_lib)); }
static DeeObject *_deesharedlib_tp_cmp_le(DeeSharedLibObject *self, DeeSharedLibObject *right) { if DEE_UNLIKELY((right = (DeeSharedLibObject *)DeeObject_GetInstance((DeeObject *)right,&DeeSharedLib_Type)) == NULL) return NULL; DeeReturn_Bool(DeeSysDynlib_CompareLe(&self->sl_lib,&right->sl_lib)); }
static DeeObject *_deesharedlib_tp_cmp_eq(DeeSharedLibObject *self, DeeSharedLibObject *right) { if DEE_UNLIKELY((right = (DeeSharedLibObject *)DeeObject_GetInstance((DeeObject *)right,&DeeSharedLib_Type)) == NULL) return NULL; DeeReturn_Bool(DeeSysDynlib_CompareEq(&self->sl_lib,&right->sl_lib)); }
static DeeObject *_deesharedlib_tp_cmp_ne(DeeSharedLibObject *self, DeeSharedLibObject *right) { if DEE_UNLIKELY((right = (DeeSharedLibObject *)DeeObject_GetInstance((DeeObject *)right,&DeeSharedLib_Type)) == NULL) return NULL; DeeReturn_Bool(DeeSysDynlib_CompareNe(&self->sl_lib,&right->sl_lib)); }
static DeeObject *_deesharedlib_tp_cmp_gr(DeeSharedLibObject *self, DeeSharedLibObject *right) { if DEE_UNLIKELY((right = (DeeSharedLibObject *)DeeObject_GetInstance((DeeObject *)right,&DeeSharedLib_Type)) == NULL) return NULL; DeeReturn_Bool(DeeSysDynlib_CompareGr(&self->sl_lib,&right->sl_lib)); }
static DeeObject *_deesharedlib_tp_cmp_ge(DeeSharedLibObject *self, DeeSharedLibObject *right) { if DEE_UNLIKELY((right = (DeeSharedLibObject *)DeeObject_GetInstance((DeeObject *)right,&DeeSharedLib_Type)) == NULL) return NULL; DeeReturn_Bool(DeeSysDynlib_CompareGe(&self->sl_lib,&right->sl_lib)); }


#if DEE_CONFIG_RUNTIME_HAVE_EXTERN
static DeeObject *_deesharedlib_tp_call(DeeSharedLibObject *self, DeeObject *args) {
 DeeObject *import_name; void *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:dll.__call__",&import_name) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(import_name,&DeeString_Type) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeSharedLib_Import((DeeObject *)self,DeeString_STR(import_name))) == NULL) return NULL;
 return DeeVoidPointer_New(result);
}
static DeeObject *_deesharedlib_tp_seq_get(DeeSharedLibObject *self, DeeObject *import_name) {
 char const *import_name_str; void *result;
 if DEE_UNLIKELY(DeeObject_GetPointerEx(import_name,DeeObject_TYPE(char),(void **)&import_name_str) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeSharedLib_TryImport((DeeObject *)self,import_name_str)) == NULL) return NULL;
 return DeeVoidPointer_New(result);
}
static DeeObject *_deesharedlib_tp_seq_contains(DeeSharedLibObject *self, DeeObject *import_name) {
 char const *import_name_str;
 if DEE_UNLIKELY(DeeObject_GetPointerEx(import_name,DeeObject_TYPE(char),(void **)&import_name_str) != 0) return NULL;
 DeeReturn_Bool(DeeSharedLib_TryImport((DeeObject *)self,import_name_str) != NULL);
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */

static DeeObject *_deesharedlib_tp_str(DeeSharedLibObject *self) {
#if defined(DEE_SYSDYNLIB_USES_WIN32) || defined(DEE_SYSDYNLIB_USES_DLFCN)
 return DeeString_Newf("<shared_lib(%R): %p>",
                       DeeSharedLib_Name((DeeObject *)self),
                       self->sl_lib.sd_lib);
#else
 return DeeString_Newf("<shared_lib(%R)>",
                       DeeSharedLib_Name((DeeObject *)self));
#endif
}
static DeeObject *_deesharedlib_tp_repr(DeeSharedLibObject *self) {
 return DeeString_Newf("shared_lib(%R)",DeeSharedLib_Name((DeeObject *)self));
}
static int _deesharedlib_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSharedLibObject *self, DeeObject *args) {
 DeeObject *filename;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:shared_lib",&filename) != 0) return -1;
 if (DeeWideString_Check(filename)) {
  if (DeeWideString_SIZE(filename)) DeeSysDynlibN_WideInitFromFilenameObject(&self->sl_lib,filename,return -1);
  else DeeSysDynlibN_InitFromSelf(&self->sl_lib,return -1);
 } else {
  if ((filename = DeeUtf8String_Cast(filename)) == NULL) return -1;
  if (DeeUtf8String_SIZE(filename)) DeeSysDynlibN_Utf8InitFromFilenameObject(&self->sl_lib,filename,goto err_filename);
  else DeeSysDynlibN_InitFromSelf(&self->sl_lib,goto err_filename);
  Dee_DECREF(filename);
 }
 return 0;
err_filename:
 Dee_DECREF(filename);
 return -1;
}

static DeeObject *_deedll_name_get(
 DeeSharedLibObject *self, void *DEE_UNUSED(closure)) {
 return DeeSharedLib_Name((DeeObject *)self);
}
static DeeObject *_deedllclass_open(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *dll_name;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:open",&dll_name) != 0) return NULL;
 DeeReturn_XNEWREF(DeeSharedLib_PoolLoadObject(dll_name));
}

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deesharedlib_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deesharedlib_tp_members[] = {
#ifdef DEE_SYSDYNLIB_USES_WIN32
 DEE_MEMBERDEF_NAMED_RO_v100("__sl_lib_sd_lib",DeeSharedLibObject,sl_lib.sd_lib,HMODULE),
#elif defined(DEE_SYSDYNLIB_USES_DLFCN)
 DEE_MEMBERDEF_NAMED_RO_v100("__sl_lib_sd_lib",DeeSharedLibObject,sl_lib.sd_lib,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__sl_lib_sd_name",DeeSharedLibObject,sl_lib.sd_name,object),
#endif
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeGetSetDef const _deesharedlib_tp_getsets[] = {
 DEE_GETSETDEF_v100("name",member(&_deedll_name_get),null,null,"-> string"),
 DEE_GETSETDEF_END_v100
};
static struct DeeMethodDef const _deesharedlib_tp_class_methods[] = {
 DEE_METHODDEF_v100("open",member(&_deedllclass_open),"(string name) -> shared_lib"),
 DEE_METHODDEF_END_v100
};


static struct DeeTypeMarshal _deesharedlib_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALID_SHAREDLIB),member(NULL),member(NULL)); // Special marshal type

DeeTypeObject DeeSharedLib_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("shared_lib"),member(
  "Wrapper for externally loadable shared libraries."),null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeSharedLibObject),null,null,null,null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deesharedlib_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&DeeSharedLib_Name),
  member(&_deesharedlib_tp_repr),null,null,null),
#if DEE_CONFIG_RUNTIME_HAVE_EXTERN
 DEE_TYPE_OBJECT_OBJECT_v101(member(&_deesharedlib_tp_call),null,null),
#else /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */
 DEE_TYPE_OBJECT_OBJECT_v101(null,null,null),
#endif /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null), // TODO: tp_hash
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deesharedlib_tp_cmp_lo),member(&_deesharedlib_tp_cmp_le),
  member(&_deesharedlib_tp_cmp_eq),member(&_deesharedlib_tp_cmp_ne),
  member(&_deesharedlib_tp_cmp_gr),member(&_deesharedlib_tp_cmp_ge)),
#if DEE_CONFIG_RUNTIME_HAVE_EXTERN
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&_deesharedlib_tp_seq_get),null,null,null,
  member(&_deesharedlib_tp_seq_contains),null,null,null,null,null),
#else /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */
 DEE_TYPE_OBJECT_SEQ_v101(
  null,null,null,null,null,null,null,null,null,null),
#endif /* !DEE_CONFIG_RUNTIME_HAVE_EXTERN */
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deesharedlib_tp_members),member(_deesharedlib_tp_getsets),
  null,null,null,member(_deesharedlib_tp_class_methods)),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deesharedlib_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};

DEE_DECL_END

#endif /* !GUARD_DEEMON_EXTERN_C */
