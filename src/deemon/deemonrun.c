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
#ifndef GUARD_DEEMON_DEEMONRUN_C
#define GUARD_DEEMON_DEEMONRUN_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
#ifdef DEE_DEBUG
#ifdef DEE_PLATFORM_WINDOWS
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#endif
#endif /* DEE_DEBUG */
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#include DEE_INCLUDE_MEMORY_API()

// include/*
#include <deemon/bool.h>
#include <deemon/cfunction.h>
#include <deemon/class.h>
#include <deemon/compiler/compiler.h>
#include <deemon/compiler/lexer.h>
#include <deemon/deemonrun.h>
#include <deemon/dex.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/float.h>
#include <deemon/fs_api.h>
#include <deemon/gc.h>
#include <deemon/integer.h>
#include <deemon/list.h>
#include <deemon/marshal.h>
#include <deemon/memberfunction.h>
#include <deemon/mp/process.h>
#include <deemon/mp/thread.h>
#include <deemon/none.h>
#include <deemon/runtime/builtins.h>
#include <deemon/runtime/extern.h>
#include <deemon/runtime/function.h>
#include <deemon/runtime_flags.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/signalhandler.h>
#include <deemon/socket.h>
#include <deemon/string.h>
#include <deemon/structured.h>
#include <deemon/super.h>
#include <deemon/tuple.h>
#include <deemon/value.h>
#include <deemon/weakref.h>

// src/*
#include <deemon/__xconf.inl>
#include <deemon/compiler/xast.h>
#include <deemon/compiler/sast.h>

DEE_DECL_BEGIN

static DeeCompilerConfig const _DeeCompilerConfig_Default = DeeCompilerConfig_INIT();
DeeCompilerConfig DeeCompilerConfig_Default = DeeCompilerConfig_INIT();


static struct DeeAtomicMutex _dee_argv_lock = DeeAtomicMutex_INIT();
static DeeObject *_dee_argv = NULL;
DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeListObject) *Dee_GetArgv(void) {
 DeeObject *result;
 DeeAtomicMutex_Acquire(&_dee_argv_lock);
 Dee_XINCREF(result = _dee_argv);
 DeeAtomicMutex_Release(&_dee_argv_lock);
 return result;
}
void Dee_SetArgv(DEE_A_INOUT_OBJECT_OPT(DeeListObject) *ob) {
 DeeObject *old_argv;
 DEE_ASSERT(!ob || (DeeObject_Check(ob) && DeeList_Check(ob)));
 DeeAtomicMutex_Acquire(&_dee_argv_lock);
 old_argv = _dee_argv; _dee_argv = ob;
 DeeAtomicMutex_Release(&_dee_argv_lock);
 Dee_XDECREF(old_argv);
}


#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
Dee_size_t Dee_StackLimit = DEE_XCONFIG_RUNTIME_DEFAULT_STACKLIMIT;
extern void _Dee_SetStackLimitString(char const *value);
void _Dee_SetStackLimitString(char const *value) {
 Dee_size_t new_limit;
 if (!*value) new_limit = DEE_XCONFIG_RUNTIME_DEFAULT_STACKLIMIT;
 else if (DeeString_ToNumber(Dee_size_t,value,&new_limit) != 0) {
  DeeError_Print("[ignored] Invalid integer value in '$" DEE_AUTOCONF_VARNAME_DEEMON_STACKLIMIT "' "
                 "(defaulting to '" DEE_PP_STR(DEE_XCONFIG_RUNTIME_DEFAULT_STACKLIMIT) "')\n",1);
  new_limit = DEE_XCONFIG_RUNTIME_DEFAULT_STACKLIMIT;
 }
 DEE_LVERBOSE1("Setting: 'Dee_StackLimit = %Iu;'\n",new_limit);
 Dee_SetStackLimit(new_limit);
}
#endif /* !DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS */

#ifdef DEE_DEBUG
extern void _DeeFlag_SetNoAssertString(char const *value);
extern void _DeeFlag_SetVerboseString(char const *value);
void _DeeFlag_SetNoAssertString(char const *value) {
 int new_flag;
 if (!*value) new_flag = DEE_XCONFIG_RUNTIME_DEFAULT_STACKLIMIT;
 else if (DeeString_ToNumber(int,value,&new_flag) != 0) {
  DeeError_Print("[ignored] Invalid integer value in '$" DEE_AUTOCONF_VARNAME_DEEMON_NOASSERT "' (defaulting to '0')\n",1);
  new_flag = 0;
 }
 DEE_LVERBOSE1("Setting: '_DeeFlag_NoAssert = %d;'\n",new_flag);
 _DeeFlag_NoAssert = new_flag;
}
void _DeeFlag_SetVerboseString(char const *value) {
 int new_flag;
 if (!*value) new_flag = DEE_XCONFIG_RUNTIME_DEFAULT_STACKLIMIT;
 else if (DeeString_ToNumber(int,value,&new_flag) != 0) {
  DeeError_Print("[ignored] Invalid integer value in '$" DEE_AUTOCONF_VARNAME_DEEMON_VERBOSE "' (defaulting to '0')\n",1);
  new_flag = 0;
 }
 DEE_LVERBOSE1("Setting: '_DeeFlag_Verbose = %d;'\n",new_flag);
 _DeeFlag_Verbose = new_flag;
}
#endif



// The debug_new init/quit functions already do this
// >> No need to dump leaks twice!
#define DEE_CONFIG_HAVE_DEBUG_NEW_MEMORY_STATE 0


#ifndef DEE_CONFIG_HAVE_DEBUG_NEW_MEMORY_STATE
#ifdef DEE_LINK_DLL_EXPORT
#define DEE_CONFIG_HAVE_DEBUG_NEW_MEMORY_STATE 1
#endif
#endif

#ifndef DEE_CONFIG_HAVE_DEBUG_NEW_MEMORY_STATE
#define DEE_CONFIG_HAVE_DEBUG_NEW_MEMORY_STATE 0
#endif

#if DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS
extern void _DeeFlag_SetNoSignalHandlersString(char const *value);
#endif /* DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS */
#if DEE_CONFIG_RUNTIME_HAVE_VFS
extern void _DeeFlag_SetNoVFSString(char const *value);
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#if DEE_XCONFIG_RUNTIME_HAVE_INIT_QUIT_RECURSION
static /*atomic*/unsigned int _dee_initquit_recursion = 0;
static /*atomic*/unsigned int _dee_initialized = 0;
#endif

DEE_A_RET_EXCEPT(-1) int Dee_InitializeEx(DEE_A_IN Dee_uint32_t flags) {
 (void)flags;

#if DEE_XCONFIG_RUNTIME_HAVE_INIT_QUIT_RECURSION
 if (DeeAtomicInt_FetchInc(_dee_initquit_recursion,memory_order_seq_cst) != 0) {
  // Wait for the library to be initialized
  while (!DeeAtomicInt_Load(_dee_initialized,memory_order_seq_cst)) DeeThread_SleepNoInterrupt(1);
  return 0;
 }
#endif

#if DEE_USE_DEBUG_NEW
#ifdef Dn_Init
#if DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION || \
    defined(DEE_LINK_DLL_EXPORT)
 // if recursion is enabled, lets make sure debug_new is initialized
 // NOTE: If we are building a DLL, we are responsible for initializing it ourselves.
 Dn_Init();
#endif
#endif
#if DEBUG_NEW_CONFIG_ENABLED
 DEE_ASSERT(DEBUG_NEW_CXX_NAMESPACE_ROOT Dn_Initialized() &&
            "debug_new must be initialized before Deemon can be initialized");
#if DEE_CONFIG_HAVE_DEBUG_NEW_MEMORY_STATE
 // If we are exporting as a dll, we know that we
 // have our own version of debug_new.
 // >> So we can create our own little memory state,
 //    to make sure that no leaks occur between
 //    the initialization and finalization of deemon
 // >> This allows for memory leaks to be faster detected
 //    if deemon is running as an embedded application
 DEBUG_NEW_API_MSTATE_GLOBAL_PUSH();
#endif
#endif /* DEBUG_NEW_CONFIG_ENABLED */
#endif
 //DEBUG_NEW_API_ALLOC_BREAKPOINT(10288);

#ifdef DEE_INSTALL_SIGNALHANDLERS
 // Install signal handlers
 DEE_INSTALL_SIGNALHANDLERS();
#endif

 // Restore the default compiler config
 DeeCompilerConfig_Default = _DeeCompilerConfig_Default;

#ifndef DEE_WITHOUT_THREADS
 if (DeeThread_Initialize() != 0) {
#ifdef DEE_UNINSTALL_SIGNALHANDLERS
  DEE_UNINSTALL_SIGNALHANDLERS();
#endif
  return -1;
 }
#endif
#if DEE_CONFIG_RUNTIME_HAVE_PROCESS
 if (DeeProcess_Initialize() != 0) {
#ifndef DEE_WITHOUT_THREADS
  DeeThread_Finalize();
#endif
#ifdef DEE_UNINSTALL_SIGNALHANDLERS
  DEE_UNINSTALL_SIGNALHANDLERS();
#endif
  return -1;
 }
#endif

 if (DeeGC_Initialize() != 0) {
#if DEE_CONFIG_RUNTIME_HAVE_PROCESS
  DeeProcess_Finalize();
#endif
#ifndef DEE_WITHOUT_THREADS
  DeeThread_Finalize();
#endif
#ifdef DEE_UNINSTALL_SIGNALHANDLERS
  DEE_UNINSTALL_SIGNALHANDLERS();
#endif
  return -1;
 }

#define DEE_INIT_ENV(name,setter,default)\
 {DeeObject *env_var;\
  if ((env_var = DeeFS_TryGetEnv(name)) != NULL) {\
   setter(DeeString_STR(env_var));\
   Dee_DECREF(env_var);\
  } else default;\
 }
#ifdef DEE_DEBUG
 DEE_INIT_ENV(DEE_AUTOCONF_VARNAME_DEEMON_VERBOSE,
              _DeeFlag_SetVerboseString,_DeeFlag_Verbose = 0);
 DEE_INIT_ENV(DEE_AUTOCONF_VARNAME_DEEMON_NOASSERT,
              _DeeFlag_SetNoAssertString,_DeeFlag_NoAssert = 0);
#endif
#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
 // Initialize the stack limit based on the environment config
 // NOTE: No need for atomics in here because usercode hasn't been executed yet
 DEE_INIT_ENV(DEE_AUTOCONF_VARNAME_DEEMON_STACKLIMIT,
              _Dee_SetStackLimitString,Dee_StackLimit = DEE_XCONFIG_RUNTIME_DEFAULT_STACKLIMIT);
#endif
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 // Enable/Disable the vfs based on the environment config
 DEE_INIT_ENV(DEE_AUTOCONF_VARNAME_DEEMON_NOVFS,
              _DeeFlag_SetNoVFSString,DeeFlag_NoVFS = 0);
#endif
#if DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS
 // Initialize the no-signal-handler flag based on environment config
 DEE_INIT_ENV(DEE_AUTOCONF_VARNAME_DEEMON_NOSIGHANDLERS,
              _DeeFlag_SetNoSignalHandlersString,DeeFlag_NoSignalHandlers = 0);
 if ((flags&DEE_INITIALIZE_FLAGS_NOSIGNAL)!=0)
  DeeFlag_NoSignalHandlers = 1; // Always disable signal handlers
#endif
#undef DEE_INIT_ENV

 if (_DeeDex_InitApi() != 0) {
  DeeGC_Finalize();
#if DEE_CONFIG_RUNTIME_HAVE_PROCESS
  DeeProcess_Finalize();
#endif
#ifndef DEE_WITHOUT_THREADS
  DeeThread_Finalize();
#endif
#ifdef DEE_UNINSTALL_SIGNALHANDLERS
  DEE_UNINSTALL_SIGNALHANDLERS();
#endif
  return -1;
 }

#if DEE_XCONFIG_RUNTIME_HAVE_INIT_QUIT_RECURSION
 // Set the flag that the library is now initialized
 DeeAtomicInt_Store(_dee_initialized,1,memory_order_seq_cst);
#endif
 return 0;
}

Dee_size_t Dee_ClearInstancePools(void) {
 Dee_size_t result = 0;
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
 result += DeeInstanceMethod_ClearPool();
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
 result += DeeInt_ClearPool();
 result += DeeFloat_ClearPool();
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 result += DeePointer_ClearPool();
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 result += DeeTuple_ClearPool();
 result += DeeWeakref_ClearPool();
 result += DeeToken_ClearPool();
 result += DeeXAst_ClearPool();
 result += DeeSAst_ClearPool();
 result += DeeSuper_ClearPool();
 result += DeeMemberFunction_ClearPool();
 return result;
}
DEE_A_EXEC Dee_size_t Dee_CollectMemory(void) {
 Dee_size_t result = 0;
 result += Dee_ClearInstancePools();
 result += DeeError_ClearFreeExceptions();
 result += DeeDex_CollectMemory();
 result += DeeGC_CollectNow();
 return result;
}


#if DEE_CONFIG_RUNTIME_HAVE_EXIT
DEE_STATIC_INLINE(void) _Dee_RunAtExit(void);
DEE_STATIC_INLINE(void) _Dee_ClearAtExit(void);
#endif /* DEE_CONFIG_RUNTIME_HAVE_EXIT */

void Dee_FinalizeEx(DEE_A_IN Dee_uint32_t flags) {
 Dee_size_t gc_cycles;
 (void)flags;

#if DEE_XCONFIG_RUNTIME_HAVE_INIT_QUIT_RECURSION
 if (DeeAtomicInt_DecFetch(_dee_initquit_recursion,memory_order_seq_cst) != 0)
  return; // Don't wait! Otherwise we get a deadlock
#endif

#if DEE_CONFIG_RUNTIME_HAVE_EXIT
 // Start by executing user exit callbacks
 // >> At this point, user code can probably still be executed.
 if ((flags&DEE_FINALIZE_FLAGS_NOCALLBACK)==0) _Dee_RunAtExit();
 else _Dee_ClearAtExit();
#endif

 // Start by collecting some initial garbage
 // - This should clean up any dangling instances of user types
 // - We continue doing this until the gc is finished.
 //   And while doing that, we are already cleaning all the pools.
 // - The only reason why the gc could still contain stuff
 //   would be user types that abort their own destruction.
 DeeGC_CollectNow();
 gc_cycles = 0;
 do {
  _DeeDex_CleanupApi();

  // Reset std file handles
  DeeFile_ResetStd(); // Reset 

  if (++gc_cycles >= 1024) {
   if (DeeFile_STDPRINT(DEE_STDERR,
    "Too deep recursion in garbage collector; switching to aggressive mode\n"
    ) != 0) DeeError_Handled();
   break;
  }

#ifndef DEE_WITHOUT_THREADS
  // Finalize the thread api (and wait for any threads still running)
  // >> This has to be done first, so we don't have other threads
  //    filling the global object pools after we cleared them
  DeeThread_PrepareFinalize();
#endif

  // Clear instance pools
  Dee_ClearInstancePools();
  
  // Cleanup marshal types
  _DeeMarshal_ClearRegisteredTypes();

  // Delete 'sys.argv'
  Dee_SetArgv(NULL);

  // Clear free exceptions after they were raised
  DeeError_ClearFreeExceptions();
 } while (DeeGC_CollectNow());

#ifdef DEE_DEBUG
 if (gc_cycles > 1) DEE_LVERBOSE1("[shutdown] Garbage collector satisfied after %Iu cycles\n",gc_cycles);
#endif
 
 _DeeDex_QuitApi();


 // From this point on, no user code can be executed!

#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
 // Clear the cache of class types
 // NOTE: The cache only consists of weak references,
 //       so no user code should be able to run here!
 _DeeClassType_Finalize();
#endif

 // Clear all structured type caches (pointer types, lvalue types, foreign_function types, ...)
 DeeStructuredType_Shutdown();
 // NOTE: Clearing all cached structured types can release some more gc objects in some cases.
 //       Those objects include c-structures that 
 DeeGC_CollectNow();

 // De-initialize all dlls (NOTE: This can't execute user code)
 DeeSharedLib_Shutdown();

 // Shutdown the gc pool
 DeeGC_Finalize();

#ifndef DEE_WITHOUT_THREADS
 // Finalize the thread api again, in case someone used it
 DeeThread_PrepareFinalize();
#endif

 // We are done!
#ifdef DEE_DEBUG
 // Now that everything is deallocated, lets make sure
 // all the references to static objects are consistent
 {Dee_uint16_t i = 0; DeeObject *ob;
  while ((ob = DeeBuiltin_Get(i++)) != NULL) {
   if (ob->__ob_refcnt != 1) {
    DEE_LDEBUG("[shutdown][builtin %I16x][%R] "
    "Inconsistent reference count (%u too many)\n",
    i-1,DeeObject_Str(ob),ob->__ob_refcnt-1u);
#ifndef DEE_WITHOUT_THREADS
    // 'DeeObject_Str' caused the thread api to re-initializie
    // >> Let's finalize it (again... *sigh*)
    DeeThread_PrepareFinalize();
#endif
   }
  }
 }
#endif

 // Destroy the thread.self() tls (NOTE: Only the system call; not a decref)
 DeeThread_Finalize();

#if DEE_CONFIG_RUNTIME_HAVE_PROCESS
 // Finalize the process API
 DeeProcess_Finalize();
#endif

#ifdef DEE_UNINSTALL_SIGNALHANDLERS
  DEE_UNINSTALL_SIGNALHANDLERS();
#endif

 // Undo all the stuff we did to debug_new
#if DEE_USE_DEBUG_NEW
#if DEBUG_NEW_CONFIG_ENABLED
#if DEE_CONFIG_HAVE_DEBUG_NEW_MEMORY_STATE
 DEBUG_NEW_API_MSTATE_GLOBAL_POP();
#endif
#endif
#ifdef Dn_Quit
#if DEBUG_NEW_CONFIG_HAVE_INIT_QUIT_RECURSION || \
    defined(DEE_LINK_DLL_EXPORT)
 Dn_Quit();
#endif
#endif
#endif
#if DEE_XCONFIG_RUNTIME_HAVE_INIT_QUIT_RECURSION
 // Set the flag that the library is no longer initialized
 DeeAtomicInt_Store(_dee_initialized,0,memory_order_seq_cst);
#endif
}


#ifdef DEE_DEBUG
int _DeeFlag_NoAssert = 0;
int _DeeFlag_Verbose = 0;
DEE_STATIC_INLINE(void) _DeeVDebugOut(char const *fmt, va_list args) {
 DeeStringWriter writer = DeeStringWriter_INIT();
 if (DeeStringWriter_VWritef(&writer,fmt,args) == -1) goto end;
 if (!DeeStringWriter_EMPTY(&writer)) {
#ifdef DEE_PLATFORM_WINDOWS
  if (DeeStringWriter_WriteChar(&writer,'\0') == -1) goto end;
  OutputDebugStringA(DeeStringWriter_STR(&writer));
  fwrite(DeeStringWriter_ELEM(&writer),sizeof(char),DeeStringWriter_SIZE(&writer)-1,stderr);
#else /* DEE_PLATFORM_WINDOWS */
  fwrite(DeeStringWriter_ELEM(&writer),sizeof(char),DeeStringWriter_SIZE(&writer),stderr);
#endif /* DEE_PLATFORM_WINDOWS */
 }
 //if (DeeFile_StdWriteAll(DEE_STDERR,
 // DeeStringWriter_STR(&writer),
 // DeeStringWriter_SIZE(&writer)-1) != 0) DeeError_Handled();
end:
 DeeStringWriter_Quit(&writer);
}

void _Dee_VerboseOut(char const *fmt, ...) {
 va_list args;
#if 0
#ifdef DEE_PLATFORM_WINDOWS
 HANDLE hConsole;
#endif /* DEE_PLATFORM_WINDOWS */
#endif
 va_start(args,fmt);
#if 0
#ifdef DEE_PLATFORM_WINDOWS
 hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
 SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE|FOREGROUND_GREEN);
#endif /* DEE_PLATFORM_WINDOWS */
#endif
 _DeeVDebugOut(fmt,args);
#if 0
#ifdef DEE_PLATFORM_WINDOWS
 SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
#endif /* DEE_PLATFORM_WINDOWS */
#endif
 va_end(args);
}
void _Dee_DebugOut(char const *fmt, ...) {
 va_list args;
 va_start(args,fmt);
 _DeeVDebugOut(fmt,args);
 va_end(args);
}
DEE_ATTRIBUTE_NORETURN void _Dee_AbnormalTermination(void) {
 // We use this function to instantly terminate the application
 // It is called, if a fatal error was encountered
 // NOTE: Only available in debug mode and used if an assertion fails,
 //       or other undefined behavior is executed.
#ifdef DEE_PLATFORM_WINDOWS
 // Doing it this way skips c++ finalization code.
 // >> So the error output doesn't get spammed with memory leaks...
 TerminateProcess(GetCurrentProcess(),1);
#elif 1
 unsigned int *ptr = (unsigned int *)0xDEADBEEF;
 while (1) *ptr++ *= 2; // Cause a core dump
#else
 exit(1);
#endif
 DEE_BUILTIN_UNREACHABLE();
}

#ifndef DEE_BUILTIN_BREAKPOINT
DEE_ATTRIBUTE_NORETURN
#endif /* !DEE_BUILTIN_BREAKPOINT */
void _Dee_AssertionFailed(
 char const *expr, char const *file_, int line_) {
 DEE_LDEBUG("%s(%d) : Assertion failed : %s\n",file_,line_,expr);
#ifndef DEE_BUILTIN_BREAKPOINT
 _Dee_AbnormalTermination();
#endif /* !DEE_BUILTIN_BREAKPOINT */
}
void _Dee_AssertionFailedf(
 char const *expr, char const *file_, int line_, char const *fmt, ...) {
 va_list args;
 DEE_LDEBUG("%s(%d) : Assertion failed : %s\n>> ",file_,line_,expr);
 va_start(args,fmt);
 _DeeVDebugOut(fmt,args);
 va_end(args);
 _Dee_DebugOut("\n");
#ifndef DEE_BUILTIN_BREAKPOINT
 _Dee_AbnormalTermination();
#endif /* !DEE_BUILTIN_BREAKPOINT */
}
#endif /* DEE_DEBUG */



#if DEE_CONFIG_RUNTIME_HAVE_EXIT
struct _dee_atexit_entry {
 DEE_A_REF DeeObject *func; /*< [1..1] Function callback. */
 DEE_A_REF DeeObject *args; /*< [1..1] Function arguments. */
};
static struct DeeAtomicMutex     _dee_atexit_lock = DeeAtomicMutex_INIT();
static Dee_size_t                    _dee_atexit_c = 0;
static struct _dee_atexit_entry *_dee_atexit_v = NULL;

DEE_A_RET_EXCEPT(-1) int Dee_AtExit(
 DEE_A_INOUT DeeObject *func, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 struct _dee_atexit_entry *new_list;
again:
 DEE_ASSERT(DeeObject_Check(func));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 DeeAtomicMutex_Acquire(&_dee_atexit_lock);
 if ((new_list = (struct _dee_atexit_entry *)realloc_nz(
  _dee_atexit_v,(_dee_atexit_c+1)*sizeof(struct _dee_atexit_entry))
  ) == NULL) {
  DeeAtomicMutex_Release(&_dee_atexit_lock);
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return -1;
 }
 _dee_atexit_v = new_list;
 new_list += _dee_atexit_c++;
 Dee_INCREF(new_list->func = func);
 Dee_INCREF(new_list->args = args);
 DeeAtomicMutex_Release(&_dee_atexit_lock);
 return 0;
}

DEE_STATIC_INLINE(void) _Dee_RunAtExit(void) {
 struct _dee_atexit_entry *list,*end;
 DeeAtomicMutex_Acquire(&_dee_atexit_lock);
 end = (list = _dee_atexit_v)+_dee_atexit_c;
 _dee_atexit_v = NULL,_dee_atexit_c = 0;
 DeeAtomicMutex_Release(&_dee_atexit_lock);
 while (end-- != list) {
  DeeObject *temp = DeeObject_Call(end->func,end->args);
  // Dump unhandled errors in here
  if (temp) Dee_DECREF(temp); else {
   DeeError_Print("[Ignored] Exception in __builtin_atexit callback\n",1);
  }
  Dee_DECREF(end->func);
  Dee_DECREF(end->args);
 }
 free(list);
}

DEE_STATIC_INLINE(void) _Dee_ClearAtExit(void) {
 struct _dee_atexit_entry *list,*end;
 DeeAtomicMutex_Acquire(&_dee_atexit_lock);
 end = (list = _dee_atexit_v)+_dee_atexit_c;
 _dee_atexit_v = NULL,_dee_atexit_c = 0;
 DeeAtomicMutex_Release(&_dee_atexit_lock);
 while (end-- != list) {
  Dee_DECREF(end->func);
  Dee_DECREF(end->args);
 }
 free(list);
}

#if DEBUG_NEW_CONFIG_ENABLED && defined(__cplusplus)
static void _dee_debug_new_void_printfunc(char const *DEE_UNUSED(msg)) {}
#endif

DEE_ATTRIBUTE_NORETURN void Dee_Exit(DEE_A_IN int exitcode) {
 _Dee_RunAtExit(); // Execute on-exit callbacks
#if DEBUG_NEW_CONFIG_ENABLED && defined(__cplusplus)
 // Set an empty print functions for debug_new, since calling
 // exit() will execute c++ static finalization and dump a
 // whole bunch of memory leaks.
 // But those leaks don't matter since everything
 // not freed will be cleaned by the OS.
 DEBUG_NEW_API_PRINTFUNC_PUSH(&_dee_debug_new_void_printfunc);
#endif
 Dee__Exit(exitcode);
}
DEE_ATTRIBUTE_NORETURN void Dee__Exit(DEE_A_IN int exitcode) {
 exit(exitcode);
}
#endif


#if DEE_CONFIG_RUNTIME_HAVE_UPDATE_CHECK
// Insert update server data here...
#define DEE_UPDATE_SERVER_HOST "I wish I could trust you..."
#define DEE_UPDATE_SERVER_PORT 0
#define DEE_UPDATE_SERVER_PAGE ":("

DEE_A_RET_WUNUSED Dee_uint32_t Dee_GetVersionLatest(void) {
 static char const _message[] = 
  "GET " DEE_UPDATE_SERVER_PAGE "?os="
#ifdef DEE_PLATFORM_UNIX
  "posix"
#elif defined(DEE_PLATFORM_WINDOWS)
  "windows"
#else
  "unknown"
#endif
#if DEE_DEVEL_BUILD
  "&dev=1"
#endif
  "&ps=" DEE_PP_STR(DEE_TYPES_SIZEOF_POINTER)
  "&me=" DEE_PP_STR(DEE_VERSION_API) "/"
         DEE_PP_STR(DEE_VERSION_COMPILER) ":"
         DEE_PP_STR(DEE_VERSION_REVISION)
  "\r\n"
  "\r\n";
 struct DeeSockAddr addr; DeeObject *sock;
 char return_buffer[256]; Dee_size_t return_bufsize;
 unsigned int result,api,compiler,revision; char const *api_end,*compiler_end;
 if (DeeSockAddr_InitFromStringAndPort(
  &addr,DEE_UPDATE_SERVER_HOST,DEE_UPDATE_SERVER_PORT) == -1) goto err;
 if ((sock = DeeTCPSocket_NewClient(&addr)) == NULL) goto err;
 if (DeeTCPSocket_SendAll(sock,_message,sizeof(_message)) == -1) {err_sock: Dee_DECREF(socket); goto err; }
 if (DeeTCPSocket_Recv(sock,return_buffer,sizeof(
  return_buffer)-sizeof(char),&return_bufsize) == -1) goto err_sock;
 Dee_DECREF(sock); // we're done with you
 DEE_LVERBOSE1("update-check: wget returned %.*q\n",
               (unsigned)return_bufsize,return_buffer);
 // >> Parse the output
 return_buffer[return_bufsize] = 0; // no overflows here!
 api_end = return_buffer;
 while (*api_end && *api_end != '/') ++api_end;
 if (DeeUtf8String_ToUInt32WithLength((Dee_size_t)(
  api_end-return_buffer),return_buffer,&api) == -1) goto err;
 compiler_end = ++api_end;
 while (*compiler_end && *compiler_end != ':') ++compiler_end;
 if (DeeUtf8String_ToUInt32WithLength((Dee_size_t)(
  compiler_end-api_end),api_end,&compiler) == -1) goto err;
 ++compiler_end;
 if (DeeUtf8String_ToUInt32WithLength(return_bufsize-(Dee_size_t)(
  compiler_end-return_buffer),compiler_end,&revision) == -1) goto err;
 result = DEE_VERSION_MAKE(api,compiler,revision);
 DEE_LVERBOSE1("update-check deduced latest version: %d/%d:%d (%d)\n",
               api,compiler,revision,result);
 return result;
err:
#ifdef DEE_DEBUG
 {DeeObject *exception;
  if (DeeError_GetOccurred(&exception,NULL))
   DEE_LVERBOSE1("Failed to check for updates:\n>> %k\n",exception);
 }
#endif
 DeeError_Handled();
 return 0;
}


static DeeObject *_Dee_DoCheckForUpdates_F(DeeObject *args);
static struct DeeCFunctionObject _Dee_DoCheckForUpdates = DeeCFunction_INIT(
 "do_check_updates","() -> none",_Dee_DoCheckForUpdates_F);
static DeeObject *_Dee_DoCheckForUpdates_F(DeeObject *DEE_UNUSED(args)) {
 Dee_uint32_t latest;
 latest = Dee_GetVersionLatest();
 if (latest > DEE_VERSION_FULL) {
  if (DeeFile_StdPrintf(DEE_STDERR,
   "[UpdateNotification] deemon %d/%d:%d is now available "
   "(you're using: " DEE_PP_STR(DEE_VERSION_API) "/"
                     DEE_PP_STR(DEE_VERSION_COMPILER) ":"
                     DEE_PP_STR(DEE_VERSION_REVISION) ")\n"
   "[UpdateNotification] Get it at 'https://sourceforge.net/projects/deemon/'\n"
   "[UpdateNotification] Skip this check with 'deemon -nu' or 'setenv(\""
                         DEE_AUTOCONF_VARNAME_DEEMON_NOUPDATE "\",\"1\")'\n",
   DEE_VERSION_GET_API(latest),DEE_VERSION_GET_COMPILER(latest),DEE_VERSION_GET_REVISION(latest)
   ) != 0) DeeError_Handled(); // ignore errors
 }
 DeeReturn_None;
}
static void _Dee_CheckForUpdatesNow(void) {
 DeeObject *check_thread;
 // TODO: The runtime shouldn't have to wait for this thread to finish in small apps
 if ((check_thread = DeeThread_New((DeeObject *)&_Dee_DoCheckForUpdates,Dee_EmptyTuple)) == NULL) goto err;
 if (DeeThread_Start(check_thread) == -1) {err_thread: Dee_DECREF(check_thread); goto err; }
 if (DeeThread_Detach(check_thread) == -1) goto err_thread;
 Dee_DECREF(check_thread);
 return;
err:
#ifdef DEE_DEBUG
 {DeeObject *err;
  if (!DeeError_GetOccurred(&err,NULL))
   DEE_LVERBOSE1("Failed to check for updates:\n>> %k\n",err);
 }
#endif
 DeeError_Handled();
}
static void _Dee_CheckForUpdates(void) {
 DeeObject *env_var; int skip;
 if ((env_var = DeeFS_TryGetEnv(DEE_AUTOCONF_VARNAME_DEEMON_NOUPDATE)) != NULL) {
  skip = !DeeUtf8String_EQUALS(env_var,"0");
  Dee_DECREF(env_var);
  if (skip) return;
 }
 _Dee_CheckForUpdatesNow();
}
#endif



DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *DeeCompile_FileStream(
 DEE_A_INOUT_OBJECT(DeeFileObject) *filestream,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 DeeObject *result,*lexer = DeeLexer_NewWithStdInc();
 if (!lexer) return NULL;
 if (DeeLexer_IncludeFileStream(lexer,filestream) == -1) result = NULL;
 else result = DeeLexer_ParseAndCompile(lexer,config);
 Dee_DECREF(lexer);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *DeeCompile_FileStreamEx(
 DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 DeeObject *result,*lexer = DeeLexer_NewWithStdInc();
 if (!lexer) return NULL;
 if (DeeLexer_IncludeFileStreamEx(lexer,filestream,disp_filename) == -1) result = NULL;
 else result = DeeLexer_ParseAndCompile(lexer,config);
 Dee_DECREF(lexer);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *DeeCompile_Filename(
 DEE_A_IN_Z char const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return DeeCompile_FilenameEx(filename,filename,config);
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *DeeCompile_FilenameEx(
 DEE_A_IN_Z char const *filename, DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 DeeObject *result,*lexer = DeeLexer_NewWithStdInc();
 if (!lexer) return NULL;
 if (DeeLexer_IncludeFilenameEx(lexer,filename,disp_filename) == -1) result = NULL;
 else result = DeeLexer_ParseAndCompile(lexer,config);
 Dee_DECREF(lexer);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *DeeCompile_FilenameObject(
 DEE_A_IN_OBJECT(DeeStringObject) const *filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 DEE_ASSERT(DeeObject_Check(filename) && DeeString_Check(filename));
 return DeeCompile_FilenameObjectEx(filename,DeeString_STR(filename),config);
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *DeeCompile_FilenameObjectEx(
 DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 DeeObject *result,*lexer = DeeLexer_NewWithStdInc();
 if (!lexer) return NULL;
 if (DeeLexer_IncludeFilenameObjectEx(lexer,filename,disp_filename) == -1) result = NULL;
 else result = DeeLexer_ParseAndCompile(lexer,config);
 Dee_DECREF(lexer);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *DeeCompile_String(
 DEE_A_IN_Z char const *code, DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return DEE_ASSERT(code),DeeCompile_StringWithLength(code,strlen(code),config);
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *DeeCompile_StringObject(
 DEE_A_IN_OBJECT(DeeStringObject) const *code, DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 DeeObject *result,*lexer = DeeLexer_NewWithStdInc();
 if (!lexer) return NULL;
 if (DeeLexer_IncludeStringObject(lexer,code) == -1) result = NULL;
 else result = DeeLexer_ParseAndCompile(lexer,config);
 Dee_DECREF(lexer);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *DeeCompile_StringWithLength(
 DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 DeeObject *result,*lexer = DeeLexer_NewWithStdInc();
 if (!lexer) return NULL;
 if (DeeLexer_IncludeStringWithLength(lexer,code,code_length) == -1) result = NULL;
 else result = DeeLexer_ParseAndCompile(lexer,config);
 Dee_DECREF(lexer);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *DeeCompile_StringEx(
 DEE_A_IN_Z char const *code, DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return DEE_ASSERT(code),DeeCompile_StringWithLengthEx(code,strlen(code),disp_filename,config);
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *DeeCompile_StringObjectEx(
 DEE_A_IN_OBJECT(DeeStringObject) const *code, DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 DeeObject *result,*lexer = DeeLexer_NewWithStdInc();
 if (!lexer) return NULL;
 if (DeeLexer_IncludeStringObjectEx(lexer,code,disp_filename) == -1) result = NULL;
 else result = DeeLexer_ParseAndCompile(lexer,config);
 Dee_DECREF(lexer);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *DeeCompile_StringWithLengthEx(
 DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length,
 DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 DeeObject *result,*lexer = DeeLexer_NewWithStdInc();
 if (!lexer) return NULL;
 if (DeeLexer_IncludeStringWithLengthEx(lexer,code,code_length,disp_filename) == -1) result = NULL;
 else result = DeeLexer_ParseAndCompile(lexer,config);
 Dee_DECREF(lexer);
 return result;
}



DEE_STATIC_INLINE(DeeObject *) _dee_run_and_consume(DeeObject *func, DeeObject *args) {
 DeeObject *result;
 if (!func) return NULL; // Error during compilation
 DEE_ASSERT(DeeObject_Check(func) && DeeFunction_Check(func));
 result = DeeFunction_Call(func,args);
 Dee_DECREF(func);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_FileStreamArgs(
 DEE_A_INOUT_OBJECT(DeeFileObject) *filestream,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 return _dee_run_and_consume(DeeCompile_FileStream(filestream,config),args);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_FileStreamExArgs(
 DEE_A_INOUT_OBJECT(DeeFileObject) *filestream,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 return _dee_run_and_consume(DeeCompile_FileStreamEx(filestream,disp_filename,config),args);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_FilenameArgs(
 DEE_A_IN_Z char const *filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 return _dee_run_and_consume(DeeCompile_Filename(filename,config),args);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_FilenameExArgs(
 DEE_A_IN_Z char const *filename,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 return _dee_run_and_consume(DeeCompile_FilenameEx(filename,disp_filename,config),args);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_FilenameObjectArgs(
 DEE_A_IN_OBJECT(DeeStringObject) const *filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 return _dee_run_and_consume(DeeCompile_FilenameObject(filename,config),args);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_FilenameObjectExArgs(
 DEE_A_IN_OBJECT(DeeStringObject) const *filename,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 return _dee_run_and_consume(DeeCompile_FilenameObjectEx(filename,disp_filename,config),args);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_StringArgs(
 DEE_A_IN_Z char const *code,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 return _dee_run_and_consume(DeeCompile_String(code,config),args);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_StringObjectArgs(
 DEE_A_IN_OBJECT(DeeStringObject) const *code,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 return _dee_run_and_consume(DeeCompile_StringObject(code,config),args);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_StringWithLengthArgs(
 DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 return _dee_run_and_consume(DeeCompile_StringWithLength(code,code_length,config),args);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_StringExArgs(
 DEE_A_IN_Z char const *code, DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 return _dee_run_and_consume(DeeCompile_StringEx(code,disp_filename,config),args);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_StringObjectExArgs(
 DEE_A_IN_OBJECT(DeeStringObject) const *code,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 return _dee_run_and_consume(DeeCompile_StringObjectEx(code,disp_filename,config),args);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_StringWithLengthExArgs(
 DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 return _dee_run_and_consume(DeeCompile_StringWithLengthEx(code,code_length,disp_filename,config),args);
}

#define DEE_PRIVATE_EXPAND_TUPLE(...) __VA_ARGS__
#define MAKE_ARGS_FORMAT_FUNCTIONS(prefix,name,arg_types,arg_vars)\
DEE_A_RET_EXCEPT_REF DeeObject *Dee##prefix##_##name##f(\
 DEE_PRIVATE_EXPAND_TUPLE arg_types,\
 DEE_A_IN_BUILDTUPLEF char const *fmt, ...) {\
 va_list args; DeeObject *result,*args_ob;\
 va_start(args,fmt);\
 args_ob = Dee_VBuildTuple(fmt,args);\
 va_end(args);\
 if (!args_ob) return NULL;\
 result = Dee##prefix##_##name(DEE_PRIVATE_EXPAND_TUPLE arg_vars,args_ob);\
 Dee_DECREF(args_ob);\
 return result;\
}\
DEE_A_RET_EXCEPT_REF DeeObject *Dee##prefix##_V##name##f(\
 DEE_PRIVATE_EXPAND_TUPLE arg_types,\
 DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) {\
 DeeObject *result,*args_ob;\
 if ((args_ob = Dee_VBuildTuple(fmt,args)) == NULL) return NULL;\
 result = Dee##prefix##_##name(DEE_PRIVATE_EXPAND_TUPLE arg_vars,args_ob);\
 Dee_DECREF(args_ob);\
 return result;\
}
MAKE_ARGS_FORMAT_FUNCTIONS(Run,FileStreamArgs,(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(filestream,config))
MAKE_ARGS_FORMAT_FUNCTIONS(Run,FileStreamExArgs,(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(filestream,disp_filename,config))
MAKE_ARGS_FORMAT_FUNCTIONS(Run,FilenameArgs,(DEE_A_IN_Z char const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(filename,config))
MAKE_ARGS_FORMAT_FUNCTIONS(Run,FilenameExArgs,(DEE_A_IN_Z char const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(filename,disp_filename,config))
MAKE_ARGS_FORMAT_FUNCTIONS(Run,FilenameObjectArgs,(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(filename,config))
MAKE_ARGS_FORMAT_FUNCTIONS(Run,FilenameObjectExArgs,(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(filename,disp_filename,config))
MAKE_ARGS_FORMAT_FUNCTIONS(Run,StringArgs,(DEE_A_IN_Z char const *code, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(code,config))
MAKE_ARGS_FORMAT_FUNCTIONS(Run,StringObjectArgs,(DEE_A_IN_OBJECT(DeeStringObject) const *code, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(code,config))
MAKE_ARGS_FORMAT_FUNCTIONS(Run,StringWithLengthArgs,(DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(code,code_length,config))
MAKE_ARGS_FORMAT_FUNCTIONS(Run,StringExArgs,(DEE_A_IN_Z char const *code, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(code,disp_filename,config))
MAKE_ARGS_FORMAT_FUNCTIONS(Run,StringObjectExArgs,(DEE_A_IN_OBJECT(DeeStringObject) const *code, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(code,disp_filename,config))
MAKE_ARGS_FORMAT_FUNCTIONS(Run,StringWithLengthExArgs,(DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(code,code_length,disp_filename,config))
MAKE_ARGS_FORMAT_FUNCTIONS(Exec,FileStreamArgs,(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(filestream,config))
MAKE_ARGS_FORMAT_FUNCTIONS(Exec,FileStreamExArgs,(DEE_A_INOUT_OBJECT(DeeFileObject) *filestream, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(filestream,disp_filename,config))
MAKE_ARGS_FORMAT_FUNCTIONS(Exec,FilenameArgs,(DEE_A_IN_Z char const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(filename,config))
MAKE_ARGS_FORMAT_FUNCTIONS(Exec,FilenameExArgs,(DEE_A_IN_Z char const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(filename,disp_filename,config))
MAKE_ARGS_FORMAT_FUNCTIONS(Exec,FilenameObjectArgs,(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(filename,config))
MAKE_ARGS_FORMAT_FUNCTIONS(Exec,FilenameObjectExArgs,(DEE_A_IN_OBJECT(DeeStringObject) const *filename, DEE_A_IN_Z char const *disp_filename, DEE_A_IN_OPT struct DeeCompilerConfig const *config),(filename,disp_filename,config))
#undef MAKE_ARGS_FORMAT_FUNCTIONS
#undef DEE_PRIVATE_EXPAND_TUPLE



#undef DeeRun_FileStream
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_FileStream(
 DEE_A_INOUT_OBJECT(DeeFileObject) *filestream,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return _dee_run_and_consume(DeeCompile_FileStream(filestream,config),Dee_EmptyTuple);
}
#undef DeeRun_FileStreamEx
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_FileStreamEx(
 DEE_A_INOUT_OBJECT(DeeFileObject) *filestream,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return _dee_run_and_consume(DeeCompile_FileStreamEx(filestream,disp_filename,config),Dee_EmptyTuple);
}
#undef DeeRun_Filename
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_Filename(
 DEE_A_IN_Z char const *filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return _dee_run_and_consume(DeeCompile_Filename(filename,config),Dee_EmptyTuple);
}
#undef DeeRun_FilenameEx
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_FilenameEx(
 DEE_A_IN_Z char const *filename,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return _dee_run_and_consume(DeeCompile_FilenameEx(filename,disp_filename,config),Dee_EmptyTuple);
}
#undef DeeRun_FilenameObject
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_FilenameObject(
 DEE_A_IN_OBJECT(DeeStringObject) const *filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return _dee_run_and_consume(DeeCompile_FilenameObject(filename,config),Dee_EmptyTuple);
}
#undef DeeRun_FilenameObjectEx
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_FilenameObjectEx(
 DEE_A_IN_OBJECT(DeeStringObject) const *filename,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return _dee_run_and_consume(DeeCompile_FilenameObjectEx(filename,disp_filename,config),Dee_EmptyTuple);
}
#undef DeeRun_String
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_String(
 DEE_A_IN_Z char const *code,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return _dee_run_and_consume(DeeCompile_String(code,config),Dee_EmptyTuple);
}
#undef DeeRun_StringObject
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_StringObject(
 DEE_A_IN_OBJECT(DeeStringObject) const *code,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return _dee_run_and_consume(DeeCompile_StringObject(code,config),Dee_EmptyTuple);
}
#undef DeeRun_StringWithLength
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_StringWithLength(
 DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return _dee_run_and_consume(DeeCompile_StringWithLength(code,code_length,config),Dee_EmptyTuple);
}
#undef DeeRun_StringEx
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_StringEx(
 DEE_A_IN_Z char const *code, DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return _dee_run_and_consume(DeeCompile_StringEx(code,disp_filename,config),Dee_EmptyTuple);
}
#undef DeeRun_StringObjectEx
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_StringObjectEx(
 DEE_A_IN_OBJECT(DeeStringObject) const *code,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return _dee_run_and_consume(DeeCompile_StringObjectEx(code,disp_filename,config),Dee_EmptyTuple);
}
#undef DeeRun_StringWithLengthEx
DEE_A_RET_EXCEPT_REF DeeObject *DeeRun_StringWithLengthEx(
 DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return _dee_run_and_consume(DeeCompile_StringWithLengthEx(code,code_length,disp_filename,config),Dee_EmptyTuple);
}




DEE_A_RET_EXCEPT_REF DeeObject *DeeLoadCompiled_FileStream(
 DEE_A_INOUT_OBJECT(DeeFileObject) *filestream) {
 Dee_uint32_t version; int temp; DeeMarshalVersion marshal_ver;
 if ((temp = DeeMarshal_IsCompiledFile(filestream,&version,&marshal_ver)) == -1) return NULL;
 if (!temp) {
  DeeError_SET_STRING(&DeeErrorType_ValueError,"Not a compiled deemon file");
  return NULL;
 }
 (void)version; // Unused (for now)
 // Read the function object
 return DeeMarshal_ReadObject(filestream,marshal_ver);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeLoadCompiled_Filename(
 DEE_A_IN_Z char const *filename) {
 DeeObject *filestream,*result;
 if ((filestream = DeeFileIO_New(filename,"r")) == NULL) return NULL;
 result = DeeLoadCompiled_FileStream(filestream);
 Dee_DECREF(filestream);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeLoadCompiled_FilenameObject(
 DEE_A_IN_OBJECT(DeeStringObject) const *filename) {
 DeeObject *filestream,*result;
 if ((filestream = DeeFileIO_NewObject((DeeObject *)filename,"r")) == NULL) return NULL;
 result = DeeLoadCompiled_FileStream(filestream);
 Dee_DECREF(filestream);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeSaveCompiled_FileStream(
 DEE_A_INOUT DeeObject *compiled_object,
 DEE_A_INOUT_OBJECT(DeeFileObject) *filestream) {
 if (DeeMarshal_WriteCompiledFileHeader(filestream) == -1) return -1;
 return DeeMarshal_WriteObjectEx(
  filestream,compiled_object,(
  DEE_MARSHAL_WRITEFLAG_DEFAULT
  // TODO: v deemon sometimes writes invalid ids under linux?
  |DEE_MARSHAL_WRITEFLAG_REFERENCE_ALL // Enabling this can really reduce code size
  )&~DEE_MARSHAL_WRITEFLAG_UUID_REFS);
}
DEE_A_RET_EXCEPT(-1) int DeeSaveCompiled_Filename(
 DEE_A_INOUT DeeObject *compiled_object,
 DEE_A_IN_Z char const *filename) {
 DeeObject *filename_ob; int result;
 if ((filename_ob = DeeString_New(filename)) == NULL) return -1;
 result = DeeSaveCompiled_FilenameObject(compiled_object,filename_ob);
 Dee_DECREF(filename_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeSaveCompiled_FilenameObject(
 DEE_A_INOUT DeeObject *compiled_object,
 DEE_A_IN_OBJECT(DeeStringObject) const *filename) {
 DeeObject *file_stream; int result;
 if ((file_stream = DeeFileIO_NewObject((DeeObject *)filename,"w")) == NULL) return -1;
 result = DeeSaveCompiled_FileStream(compiled_object,file_stream);
 Dee_DECREF(file_stream);
#ifdef DEE_PLATFORM_UNIX // Make the file executable
 if (result != -1 && DeeFS_ChmodObject((DeeObject *)filename,0777) == -1) result = -1;
#endif
 return result;
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeLoad_FileStream(
 DEE_A_INOUT_OBJECT(DeeFileObject) *filestream,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 Dee_uint32_t version; int temp; DeeMarshalVersion marshal_ver;
 if ((temp = DeeMarshal_IsCompiledFile(filestream,&version,&marshal_ver)) == -1) return NULL;
 if (temp) { // Read the function object
  (void)version; // Unused (for now)
  return DeeMarshal_ReadObject(filestream,marshal_ver);
 } else {
  return DeeCompile_FileStream(filestream,config);
 }
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeLoad_FileStreamEx(
 DEE_A_INOUT_OBJECT(DeeFileObject) *filestream,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 Dee_uint32_t version; int temp; DeeMarshalVersion marshal_ver;
 if ((temp = DeeMarshal_IsCompiledFile(filestream,&version,&marshal_ver)) == -1) return NULL;
 if (temp) { // Read the function object
  (void)version; // Unused (for now)
  return DeeMarshal_ReadObject(filestream,marshal_ver);
 } else {
  return DeeCompile_FileStreamEx(filestream,disp_filename,config);
 }
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeLoad_Filename(
 DEE_A_IN_Z char const *filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 DeeObject *filestream,*result;
 if ((filestream = DeeFileIO_New(filename,"r")) == NULL) return NULL;
 result = DeeLoad_FileStreamEx(filestream,filename,config);
 Dee_DECREF(filestream);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeLoad_FilenameEx(
 DEE_A_IN_Z char const *filename,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 DeeObject *filestream,*result;
 if ((filestream = DeeFileIO_New(filename,"r")) == NULL) return NULL;
 result = DeeLoad_FileStreamEx(filestream,disp_filename,config);
 Dee_DECREF(filestream);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeLoad_FilenameObject(
 DEE_A_IN_OBJECT(DeeStringObject) const *filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 DeeObject *filestream,*result;
 if ((filestream = DeeFileIO_NewObject((DeeObject *)filename,"r")) == NULL) return NULL;
 result = DeeLoad_FileStreamEx(filestream,DeeString_STR(filename),config);
 Dee_DECREF(filestream);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeLoad_FilenameObjectEx(
 DEE_A_IN_OBJECT(DeeStringObject) const *filename,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 DeeObject *filestream,*result;
 if ((filestream = DeeFileIO_NewObject((DeeObject *)filename,"r")) == NULL) return NULL;
 result = DeeLoad_FileStreamEx(filestream,disp_filename,config);
 Dee_DECREF(filestream);
 return result;
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeExec_FileStreamArgs(
 DEE_A_INOUT_OBJECT(DeeFileObject) *filestream,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 DeeObject *callable,*result;
 if ((callable = DeeLoad_FileStream(filestream,config)) == NULL) return NULL;
 result = DeeObject_Call(callable,args);
 Dee_DECREF(callable);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeExec_FileStreamExArgs(
 DEE_A_INOUT_OBJECT(DeeFileObject) *filestream,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 DeeObject *callable,*result;
 if ((callable = DeeLoad_FileStreamEx(
  filestream,disp_filename,config)) == NULL) return NULL;
 result = DeeObject_Call(callable,args);
 Dee_DECREF(callable);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeExec_FilenameArgs(
 DEE_A_IN_Z char const *filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 DeeObject *filestream,*result;
 if ((filestream = DeeFileIO_New(filename,"r")) == NULL) return NULL;
 result = DeeExec_FileStreamExArgs(filestream,filename,config,args);
 Dee_DECREF(filestream);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeExec_FilenameExArgs(
 DEE_A_IN_Z char const *filename,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 DeeObject *filestream,*result;
 if ((filestream = DeeFileIO_New(filename,"r")) == NULL) return NULL;
 result = DeeExec_FileStreamExArgs(filestream,disp_filename,config,args);
 Dee_DECREF(filestream);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeExec_FilenameObjectArgs(
 DEE_A_IN_OBJECT(DeeStringObject) const *filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 DeeObject *filestream,*result;
 if ((filestream = DeeFileIO_NewObject((DeeObject *)filename,"r")) == NULL) return NULL;
 result = DeeExec_FileStreamExArgs(filestream,DeeString_STR(filename),config,args);
 Dee_DECREF(filestream);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeExec_FilenameObjectExArgs(
 DEE_A_IN_OBJECT(DeeStringObject) const *filename,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config,
 DEE_A_IN_OBJECT(DeeTupleObject) *args) {
 DeeObject *filestream,*result;
 if ((filestream = DeeFileIO_NewObject((DeeObject *)filename,"r")) == NULL) return NULL;
 result = DeeExec_FileStreamExArgs(filestream,disp_filename,config,args);
 Dee_DECREF(filestream);
 return result;
}

#undef DeeExec_FileStream
DEE_A_RET_EXCEPT_REF DeeObject *DeeExec_FileStream(
 DEE_A_INOUT_OBJECT(DeeFileObject) *filestream,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return DeeExec_FileStreamArgs(filestream,config,Dee_EmptyTuple);
}
#undef DeeExec_FileStreamEx
DEE_A_RET_EXCEPT_REF DeeObject *DeeExec_FileStreamEx(
 DEE_A_INOUT_OBJECT(DeeFileObject) *filestream,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return DeeExec_FileStreamExArgs(filestream,disp_filename,config,Dee_EmptyTuple);
}
#undef DeeExec_Filename
DEE_A_RET_EXCEPT_REF DeeObject *DeeExec_Filename(
 DEE_A_IN_Z char const *filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return DeeExec_FilenameArgs(filename,config,Dee_EmptyTuple);
}
#undef DeeExec_FilenameEx
DEE_A_RET_EXCEPT_REF DeeObject *DeeExec_FilenameEx(
 DEE_A_IN_Z char const *filename, DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return DeeExec_FilenameExArgs(filename,disp_filename,config,Dee_EmptyTuple);
}
#undef DeeExec_FilenameObject
DEE_A_RET_EXCEPT_REF DeeObject *DeeExec_FilenameObject(
 DEE_A_IN_OBJECT(DeeStringObject) const *filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return DeeExec_FilenameObjectArgs(filename,config,Dee_EmptyTuple);
}
#undef DeeExec_FilenameObjectEx
DEE_A_RET_EXCEPT_REF DeeObject *DeeExec_FilenameObjectEx(
 DEE_A_IN_OBJECT(DeeStringObject) const *filename,
 DEE_A_IN_Z char const *disp_filename,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 return DeeExec_FilenameObjectExArgs(filename,disp_filename,config,Dee_EmptyTuple);
}

#if DEBUG_NEW_CONFIG_ENABLED
static void _debugnew_hushfunc(char const *DEE_UNUSED(msg)) {}
#endif /* DEBUG_NEW_CONFIG_ENABLED */

















#ifdef __cplusplus
#define DEE_COMPILER_CXX "++"
#else
#define DEE_COMPILER_CXX ""
#endif
#if defined(_MSC_FULL_VER)
#if _MSC_VER == 1600
#define DEE_VC_VERSION " 2010"
#elif _MSC_VER == 1800
#define DEE_VC_VERSION " 2013"
#else
#define DEE_VC_VERSION ""
#endif
#define DEE_COMPILER "VC" DEE_COMPILER_CXX DEE_VC_VERSION " (" DEE_PP_STR(_MSC_FULL_VER) ")"
#elif defined(__clang__)
# define DEE_COMPILER "clang" DEE_COMPILER_CXX " " DEE_PP_STR(__clang__)
#elif defined(__GNUC__) && defined(__cplusplus)
# define DEE_COMPILER "g++ " DEE_PP_STR(__GNUC__) "." DEE_PP_STR(__GNUC_MINOR__) "." DEE_PP_STR(__GNUC_PATCHLEVEL__)
#elif defined(__GNUC__) && !defined(__cplusplus)
# define DEE_COMPILER "gcc " DEE_PP_STR(__GNUC__) "." DEE_PP_STR(__GNUC_MINOR__) "." DEE_PP_STR(__GNUC_PATCHLEVEL__)
#elif defined(__TINYC__)
# define DEE_COMPILER "tcc " DEE_PP_STR(__TINYC__)
#else
# define DEE_COMPILER "Unknown c" DEE_COMPILER_CXX " compiler"
#endif


static char const _dee_help_string_short[] =
"Usage: deemon [options...] <infile> [args...]\n"
"       deemon [options...] -i <code>...\n"
"See 'deemon --help' for more help\n"
;

static char const _dee_help_string[] =
#if DEE_DEVEL_BUILD
"\t-- DEVELOPMENT BUILD --\n"
#endif
#ifdef DEE_DEBUG
"\t-- DEBUG BUILD --\n"
#endif
"Usage: deemon [options...] <infile> [args...]\n"
"       deemon [options...] -i <code>...\n"
//"       deemon [options...] -E <infile>\n"
//"       deemon [options...] -F <infile>\n"
"Options:\n"
"\t--help [subject]        Display this help\n"
"\t-version|--version      Displays version information\n"
"\t--features              Displays information about available features\n"
#ifdef DEE_PLATFORM_WINDOWS
"\t--has-feature <feature> Stores 0|1 in '%ERRORLEVEL%' if <feature> is available (wildcards can be used)\n"
#else
"\t--has-feature <feature> Stores 0|1 in '$?' if <feature> is available (wildcards can be used)\n"
#endif
"\t-Dsym[=val=1]           Defines 'sym' as 'val'\n"
"\t-i <code>...            Use the remainder of the command line as code input\n"
"\t-C[no-]flag             Enable/Disable compiler flags (for a list: --help compiler-flags)\n"
"\t-in=auto|--input=auto   infile type is automatically detected (default)\n"
"\t-in=code|--input=code   infile is a source code file\n"
"\t-in=data|--input=data   infile is a compiled deemon file\n"
"\t-ndebug|--no-debug      Reconfigures flags to enable optimizations,\n"
"\t                        disable debug information and define NDEBUG\n"
"\t-O[0..3|min|def|max]    Set the level of optimization\n"
"\t-o|--out <outfile>      Sets an output file for the compiled source code\n"
"\t-c|--compile            Compile only (Don't execute code)\n"
"\t-nostdinc|--nostdinc    Don't include standard library search paths ('lib/include/...')\n"
#if DEE_CONFIG_RUNTIME_HAVE_UPDATE_CHECK
"\t-nu|--no-update         Disable checks for updates upon startup\n"
#endif
"\n"
"File format options:\n"
"\t-F|--format             Enable file formatting\n"
"\t-o|--out <outfile>      Sets the output file for -F (default: infile)\n"
"\n"
"Preprocessor options:\n"
"\t-E                      Preprocess only (Only invoke tpp)\n"
"\t-I <dir>                Adds 'dir' to the list of #include <...> paths\n"
"\t-o|--out <outfile>      Sets the output file for tpp (default: stdout)\n"
"\t-no-line|--no-line      Don't emit #line directives\n"
"\t--keep-comments         Re-emit comments\n"
"\t-tok|--token            Outline tokens using the [bracket] notation\n"
"\n"
#if defined(DEE_DEBUG) && 0 /*< Still works, but no point in advertising it... */
"Debug build flags (Only available in debug builds):\n"
"\t--no-assert             Disable internal assertions (Workaround for faulty assertions)\n"
"\t-V[V[V[V]]]|--verbose   Enable verbose output\n"
"\n"
#endif
"Environment Variables:\n"
#ifdef DEE_AUTOCONF_VARNAME_DEEMON_HOME
"\t$" DEE_AUTOCONF_VARNAME_DEEMON_HOME "         \tFolder describing the deemon library home directory.\n"
                         "\t                     \tUsed as base for builtin system include paths,\n"
                         "\t                     \talongside \"$(dirname $(readlink /proc/self/exe))/lib\".\n"
                         "\t                     \tMay contain '" DEE_FS_DELIM_S "' to specify multiple include paths\n"
#ifdef DEE_PLATFORM_UNIX
                         "\t                     \tdefault: \"/usr/lib/deemon\"\n"
#else
                         "\t                     \tdefault: <unset>\n"
#endif
#endif
#ifdef DEE_AUTOCONF_VARNAME_DEEMON_NOVFS
"\t$" DEE_AUTOCONF_VARNAME_DEEMON_NOVFS "        \tDisable the virtual file-system used to simulate\n"
                         "\t                     \ta minimal posix-style environment under windows.\n"
                         "\t                     \tDefine as something other than '0' to disable.\n"
#endif
#ifdef DEE_AUTOCONF_VARNAME_DEEMON_STACKLIMIT
"\t$" DEE_AUTOCONF_VARNAME_DEEMON_STACKLIMIT "   \tOverwrite the default stack limit for function recursion\n"
                         "\t                     \tMust be defined as an integer value and defaults to " DEE_PP_STR(DEE_XCONFIG_RUNTIME_DEFAULT_STACKLIMIT) "\n"
#endif
#ifdef DEE_AUTOCONF_VARNAME_DEEMON_NOUPDATE
"\t$" DEE_AUTOCONF_VARNAME_DEEMON_NOUPDATE "     \tDisable checks for updates upon startup (same as '-nu')\n"
#endif
#ifdef DEE_AUTOCONF_VARNAME_DEEMON_UDPPACKETSIZE
"\t$" DEE_AUTOCONF_VARNAME_DEEMON_UDPPACKETSIZE "\tSets the maximum allowed size for UDP packet in the socket API (default: " DEE_PP_STR(DEE_XCONFIG_SOCKET_DEFAULT_UDP_MAXPACKETSIZE) ")\n"
#endif
#ifdef DEE_AUTOCONF_VARNAME_DEEMON_MAXBACKLOG
"\t$" DEE_AUTOCONF_VARNAME_DEEMON_MAXBACKLOG "   \tSets the default backlog size for listen sockets (default: " DEE_PP_STR(DEE_XCONFIG_SOCKET_DEFAULT_MAX_BACKLOG) ")\n"
#endif
#ifdef DEE_AUTOCONF_VARNAME_DEEMON_NOSIGHANDLERS
"\t$" DEE_AUTOCONF_VARNAME_DEEMON_NOSIGHANDLERS "\tDefine as non-zero to disable signal handlers\n"
#endif
#ifdef DEE_AUTOCONF_VARNAME_DEEMON_NOSIGHANDLERS
"\t$" DEE_AUTOCONF_VARNAME_DEEMON_NOSIGHANDLERS "\tDefine as non-zero to disable signal handlers\n"
#endif
#ifdef DEE_AUTOCONF_VARNAME_DEEMON_NOASSERT
"\t$" DEE_AUTOCONF_VARNAME_DEEMON_NOASSERT "     \tDefine as non-zero to disable internal assertions (used for temporary workarounds)\n"
#endif
#ifdef DEE_AUTOCONF_VARNAME_DEEMON_VERBOSE
"\t$" DEE_AUTOCONF_VARNAME_DEEMON_VERBOSE "      \tDefine as non-zero to set the level of verbosity (0-4)\n"
#endif
""
;

static char const _dee_version_string[] =
"["
#ifdef DEE_DEBUG
"DEBUG|"
#endif /* DEE_DEBUG */
#if DEE_DEVEL_BUILD
"DEVEL_BUILD|"
#endif
#if TPP_USE_DEBUG_NEW
"DEBUG_NEW|"
#endif /* TPP_USE_DEBUG_NEW */
#ifdef __TPP_VERSION__
"TPP " DEE_PP_STR(__TPP_VERSION__) "|"
#endif /* __TPP_VERSION__ */
DEE_COMPILER "|"
#ifdef __TIME__
__TIME__ "|"
#endif /* __TIME__ */
#ifdef __DATE__
__DATE__ "|"
#endif /* __DATE__ */
#ifdef DEE_PLATFORM_UNIX
"posix|"
#elif defined(DEE_PLATFORM_WINDOWS)
"windows|"
#endif
#ifdef DEE_PLATFORM_64_BIT
"x64|"
#else
"x86|"
#endif
#if DEE_PLATFORM_ENDIAN == 1234
"little-endian"
#elif DEE_PLATFORM_ENDIAN == 4321
"big-endian"
#else
DEE_PP_STR(DEE_PLATFORM_ENDIAN) "-endian"
#endif
"]\n"
"deemon    version "
DEE_PP_STR(DEE_VERSION_API) "/"
DEE_PP_STR(DEE_VERSION_COMPILER) ":"
DEE_PP_STR(DEE_VERSION_REVISION)       " -    Deemon Compiler    - "
"Copyright (C) 2016 Cyder aka Griefer@Work\n"
"tpp       version " "  "
DEE_PP_STR(TPP_API_VERSION) "/"
DEE_PP_STR(TPP_PREPROCESSOR_VERSION)   " -   Tiny PreProcessor   - "
"Copyright (C) 2015-2016 Cyder aka Griefer@Work\n"
"debug_new version " "  " "    "
DEE_PP_STR(DEBUG_NEW_VERSION)          " - c/c++ Memory Debugger - "
"Copyright (C) 2015-2016 Cyder aka Griefer@Work\n"
"\tGriefer@Work: https://sourceforge.net/users/grieferatwork\n"
"\n\n"
;

static char const _dee_features[] =
#include "deemonrun.active_features.inl"
"\0\0"
;


enum dee_compiler_flag_kind {
 DEE_CFLAG_PARSER,
 DEE_CFLAG_OPTIMIZER,
 DEE_CFLAG_COMPILER,
};
struct dee_compiler_flag_def {
 char const                 *name;
 char const                 *doc;
 enum dee_compiler_flag_kind kind;
 Dee_uint32_t                flag;
};

// v lowest version that introduced compiler flags now deprecated (will forever be 100)
#define DEE_MIN_DEPRECATED_COMPILER_FLAGS 100

#if DEE_XCONFIG_DEEMONRUN_HAVE_DEPRECATED_COMPILER_FLAGS < DEE_MIN_DEPRECATED_COMPILER_FLAGS
#define DEPR_FLAG(replacement)         "!" replacement
#define DeeCompilerFlagDef_ISDEPR(ob)  ((ob)->doc&&(ob)->doc[0]=='!')
#define DeeCompilerFlagDef_REPLACE(ob) ((ob)->doc+1)
#define DeeCompilerFlagDef_DOC(ob)     (((ob)->doc&&(ob)->doc[0]=='!')?((ob)->doc+1):(ob)->doc)
#else
#define DEPR_FLAG(replacement)        "alias for '" replacement "'"
#define DeeCompilerFlagDef_DOC(ob)    (ob)->doc
#endif

#define dee_compiler_flags_end (dee_compiler_flags+(sizeof(dee_compiler_flags)/sizeof(*dee_compiler_flags)))
static struct dee_compiler_flag_def const dee_compiler_flags[] = {
 {"p-OPTIMIZE_CONST_SEQUENCE",    "Optimize constant lists",DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE},
 {"p-OPTIMIZE_CONST_OPERATORS",   "Optimize constant operators",DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS},
 {"p-OPTIMIZE_DEAD_BRANCH",       "Optimize away dead branches",DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH},
 {"p-OPTIMIZE_PRINT_STDOUT",      "Optimize print to stdout",DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_PRINT_STDOUT},
 {"p-OPTIMIZE_LOOPS",             "Optimize loops",DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_LOOPS},
 {"p-OPTIMIZE_EXCEPTIONS",        "Optimize exceptions and try statements",DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_EXCEPTIONS},
 {"p-OPTIMIZE_UNWIND_LOOPS",      "Unwind constant loops",DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_UNWIND_LOOPS},
 {"p-OPTIMIZE_PREDICATES",        "Optimize known predicates",DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_PREDICATES},
 {"p-OPTIMIZE_INTRINSIC_CALLS",   "Optimize calls to intrinsic functions",DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_INTRINSIC_CALLS},
 {"p-OPTIMIZE_MULTIVARDECL",      "Optimize multi-var-decl expressions",DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_MULTIVARDECL},
 {"p-OPTIMIZE_MERGE_OPERATORS",   "Merge/shift operators to form more optimized code",DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_MERGE_OPERATORS},
 {"p-CONST_FUNCTION",             "Generate constant functions during the parser phase",DEE_CFLAG_PARSER,DEE_PARSER_FLAG_CONST_FUNCTION},
 {"p-NO_ASSERTIONS",              "Disable the assert statement",DEE_CFLAG_PARSER,DEE_PARSER_FLAG_NO_ASSERTIONS},
 {"o-UNUSED_VAR",                 "Optimize away unused variables",DEE_CFLAG_OPTIMIZER,DEE_OPTIMIZE_FLAG_UNUSED_VAR},
 {"o-DEL_NOEFFECT",               "Remove statements without side-effects",DEE_CFLAG_OPTIMIZER,DEE_OPTIMIZE_FLAG_DEL_NOEFFECT},
 {"o-DEL_UNREACHABLE",            "Remove unreachable code",DEE_CFLAG_OPTIMIZER,DEE_OPTIMIZE_FLAG_DEL_UNREACHABLE},
 {"o-UNINIT_BOUND",               "Optimize __builtin_bound queries on uninitialized variables",DEE_CFLAG_OPTIMIZER,DEE_OPTIMIZE_FLAG_UNINIT_BOUND},
 {"o-MERGE_CONSTS",               "Merge constants",DEE_CFLAG_OPTIMIZER,DEE_OPTIMIZE_FLAG_MERGE_CONSTS},
 {"o-ASSUMPTIONS",                "Do assumptions about the value of local variables",DEE_CFLAG_OPTIMIZER,DEE_OPTIMIZE_FLAG_ASSUMPTIONS},
 {"o-CONST_FUNCTION",             "Optimize functions without references into constants",DEE_CFLAG_OPTIMIZER,DEE_OPTIMIZE_FLAG_CONST_FUNCTION},
 {"c-DEBUG_FILE",                 "Enables file information in tracebacks",DEE_CFLAG_COMPILER,DEE_COMPILER_FLAG_DEBUG_FILE},
 {"c-DEBUG_LINE",                 "Enables line information in tracebacks",DEE_CFLAG_COMPILER,DEE_COMPILER_FLAG_DEBUG_LINE},
 {"c-DEBUG_FUNC",                 "Enables function name information in tracebacks",DEE_CFLAG_COMPILER,DEE_COMPILER_FLAG_DEBUG_FUNC},
 {"c-DEBUG_FILE_AND_LINE",        "Enables file and line information in tracebacks",DEE_CFLAG_COMPILER,DEE_COMPILER_FLAG_DEBUG_FILE_AND_LINE},
 {"c-DEBUG_EXPR",                 "Adds debug information to every statement (quite excessive)",DEE_CFLAG_COMPILER,DEE_COMPILER_FLAG_DEBUG_EXPR},
 {"c-DEBUG_LOCALS",               "Store the names of local variables",DEE_CFLAG_COMPILER,DEE_COMPILER_FLAG_DEBUG_LOCALS},
 {"c-GEN_UNREACHABLE",            "Generate OP_UNREACHABLE opcodes",DEE_CFLAG_COMPILER,DEE_COMPILER_FLAG_GEN_UNREACHABLE},
 {"c-OPTIMIZE_CONSTRUCTOR_CALLS", "Optimizes constructor calls, using a dedicated opcode",DEE_CFLAG_COMPILER,DEE_COMPILER_FLAG_OPTIMIZE_CONSTRUCTOR_CALLS},
 {"c-OPTIMIZE_PEEPHOLE",          "Perform peephole optimizations on generated code",DEE_CFLAG_COMPILER,DEE_COMPILER_FLAG_OPTIMIZE_PEEPHOLE},
 {"c-OPTIMIZE_DELNOOP",           "Remove OP_NOOP opcodes (sometimes used as placeholders)",DEE_CFLAG_COMPILER,DEE_COMPILER_FLAG_OPTIMIZE_DELNOOP},
 {"c-OPTIMIZE_REUSE_LOCALS",      "Re-use local variable ids (partially breaks 'c-DEBUG_LOCALS')",DEE_CFLAG_COMPILER,DEE_COMPILER_FLAG_OPTIMIZE_REUSE_LOCALS},
#if DEE_XCONFIG_DEEMONRUN_HAVE_DEPRECATED_COMPILER_FLAGS < 100
 {"assert",            DEPR_FLAG("p-no-NO_ASSERTIONS"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_NO_ASSERTIONS},
 {"file-and-line",     DEPR_FLAG("c-DEBUG_FILE_AND_LINE"),DEE_CFLAG_COMPILER,DEE_COMPILER_FLAG_DEBUG_FILE_AND_LINE},
 {"funcname",          DEPR_FLAG("c-DEBUG_FUNC"),DEE_CFLAG_COMPILER,DEE_COMPILER_FLAG_DEBUG_FUNC},
 {"intrin",            DEPR_FLAG("p-OPTIMIZE_INTRINSIC_CALLS"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_INTRINSIC_CALLS},
 {"intrin-calls",      DEPR_FLAG("p-OPTIMIZE_INTRINSIC_CALLS"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_INTRINSIC_CALLS},
 {"const-intrin-calls",DEPR_FLAG("p-OPTIMIZE_INTRINSIC_CALLS"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_INTRINSIC_CALLS},
 {"gen-unreachable",   DEPR_FLAG("c-GEN_UNREACHABLE"),DEE_CFLAG_COMPILER,DEE_COMPILER_FLAG_GEN_UNREACHABLE},
 {"o-const-list",      DEPR_FLAG("p-OPTIMIZE_CONST_SEQUENCE"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE},
 {"o-const-tuple",     DEPR_FLAG("p-OPTIMIZE_CONST_SEQUENCE"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE},
 {"o-const-dict",      DEPR_FLAG("p-OPTIMIZE_CONST_SEQUENCE"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE},
 {"o-const-set",       DEPR_FLAG("p-OPTIMIZE_CONST_SEQUENCE"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE},
 {"o-const-operators", DEPR_FLAG("p-OPTIMIZE_CONST_OPERATORS"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS},
 {"o-const-subscript", DEPR_FLAG("p-OPTIMIZE_CONST_OPERATORS"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS},
 {"o-emptystr-add",    DEPR_FLAG("p-OPTIMIZE_MERGE_OPERATORS"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_MERGE_OPERATORS},
 {"o-dead-branch",     DEPR_FLAG("p-OPTIMIZE_DEAD_BRANCH"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH},
 {"o-print-stdout",    DEPR_FLAG("p-OPTIMIZE_PRINT_STDOUT"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_PRINT_STDOUT},
 {"o-stmt-for",        DEPR_FLAG("p-OPTIMIZE_LOOPS"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_LOOPS},
 {"o-stmt-for-in",     DEPR_FLAG("p-OPTIMIZE_LOOPS"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_LOOPS},
 {"o-stmt-if",         DEPR_FLAG("p-OPTIMIZE_DEAD_BRANCH"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH},
 {"o-stmt-while",      DEPR_FLAG("p-OPTIMIZE_LOOPS"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_LOOPS},
 {"o-stmt-try",        DEPR_FLAG("p-OPTIMIZE_TRY"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_EXCEPTIONS},
 {"o-unwind-loops",    DEPR_FLAG("p-OPTIMIZE_UNWIND_LOOPS"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_UNWIND_LOOPS},
 {"o-all",             DEPR_FLAG("p-OPTIMIZE_ALL"),DEE_CFLAG_PARSER,DEE_PARSER_FLAG_OPTIMIZE_ALL(0)},
#endif
};
#undef DEPR_FLAG

static int _dee_print_compiler_flags(struct DeeCompilerConfig *config, DeeObject *out) {
 static char const space[] = "                                          ";
 struct dee_compiler_flag_def const *iter;
 char const *flag_doc; int enabled;
 Dee_size_t temp,longest_name = 0;
 if (DeeFile_PRINT(out,"Compiler flags:\n") == -1) return -1;
 iter = dee_compiler_flags;
 while (iter != dee_compiler_flags_end) {
  temp = strlen(iter->name);
  if (temp > longest_name) longest_name = temp;
  ++iter;
 }
 iter = dee_compiler_flags;
 while (iter != dee_compiler_flags_end) {
#if DEE_XCONFIG_DEEMONRUN_HAVE_DEPRECATED_COMPILER_FLAGS < DEE_MIN_DEPRECATED_COMPILER_FLAGS
  // Don't show deprecated flags (they're still there and work, but aren't listed)
  if (!DeeCompilerFlagDef_ISDEPR(iter))
#endif
  {
   switch (iter->kind) {
    case DEE_CFLAG_PARSER:    enabled = (config->parser_flags&iter->flag) == iter->flag; break;
    case DEE_CFLAG_OPTIMIZER: enabled = (config->optimize_flags&iter->flag) == iter->flag; break;
    case DEE_CFLAG_COMPILER:  enabled = (config->compiler_flags&iter->flag) == iter->flag; break;
    default: DEE_BUILTIN_UNREACHABLE();
   }
   temp = strlen(iter->name);
   if ((flag_doc = DeeCompilerFlagDef_DOC(iter)) != NULL) {
    if (DeeFile_Printf(out,"[%d]\t%s",enabled,iter->name) == -1) return -1;
    if (DeeFile_WriteAll(out,space,1+(longest_name-temp)) == -1) return -1;
    if (DeeFile_Printf(out,"%s\n",flag_doc) == -1) return -1;
   }
  }
  ++iter;
 }
 if (DeeFile_PRINT(out,
  "NOTE: All flags can be prefixed with 'no-' to disable them\n") == -1) return -1;
 return 0;
}

#define PRINT_HELP(fp)       DeeFile_PRINT(fp,_dee_help_string)
#define PRINT_VERSION(fp)    DeeFile_PRINT(fp,_dee_version_string)
#define PRINT_HELP_SHORT(fp) DeeFile_PRINT(fp,_dee_help_string_short)

static int _dee_pack_args(int argc, char **argv) {
 DeeObject *result,**dst;
 DEE_ASSERT(argc >= 0);
 if ((result = _DeeList_NewUnsafe((Dee_size_t)argc)) == NULL) return -1;
 dst = DeeList_ELEM(result);
 while (argc--) {
  if ((*dst = DeeString_New(*argv++)) == NULL) {
   while (dst != DeeList_ELEM(result)) Dee_DECREF(*--dst);
   _DeeList_FreeUnsafe(result);
   return -1;
  }
  ++dst;
 }
 Dee_SetArgv(result);
 return 0;
}
static int _dee_parsearg_I(DeeObject *lexer, char const *arg) {
 DeeObject *path_obj,*cwd,*real_path; int temp;
 if ((path_obj = DeeString_New(arg)) == NULL) return -1;
 if ((cwd = DeeFS_GetCwd()) == NULL) { Dee_DECREF(path_obj); return -1; }
 // Make the path absolute, if it isn't
 real_path = DeeFS_PathAbsObject(path_obj,cwd);
 Dee_DECREF(path_obj);
 Dee_DECREF(cwd);
 if (!real_path) return -1;
 temp = DeeLexer_AddSysIncludePathObject(lexer,real_path);
 Dee_DECREF(real_path);
 return temp;
}
static int _dee_parsearg_D(DeeObject *lexer, char const *arg) {
 DeeObject *def_name; char const *equal; int temp;
 equal = arg; while (*equal && *equal != '=') ++equal;
 if (*equal) {
  if ((def_name = DeeString_NewWithLength((Dee_size_t)(equal-arg),arg)) == NULL) return -1;
  temp = TPPLexer_FastDefine(DeeLexer_LEXER(lexer),DeeString_STR(def_name),equal+1);
  Dee_DECREF(def_name);
  return temp;
 } else return TPPLexer_FastDefine(DeeLexer_LEXER(lexer),arg,"1");
}


static int _dee_parsearg_C(
 struct DeeCompilerConfig *config, char const *arg) {
 int enable = 1,found = 0;
 struct dee_compiler_flag_def const *iter;
 if (memcmp(arg,"no-",3) == 0) { enable = 0; arg += 3; }
 iter = dee_compiler_flags;
 while (iter != dee_compiler_flags_end) {
  if (Dee_Utf8StrWMatch(iter->name,arg)) {
#if DEE_XCONFIG_DEEMONRUN_HAVE_DEPRECATED_COMPILER_FLAGS < DEE_MIN_DEPRECATED_COMPILER_FLAGS
   if (DeeCompilerFlagDef_ISDEPR(iter)) {
    // Warn about deprecated compiler flags
    if (DeeFile_StdPrintf(DEE_STDERR,"Deprecated flag: %q; use: %q\n",
     iter->name,DeeCompilerFlagDef_REPLACE(iter)) != 0) return -1;
   }
#endif
   switch (iter->kind) {
    case DEE_CFLAG_PARSER:
     if (enable) config->parser_flags |= iter->flag;
     else config->parser_flags &= ~iter->flag;
     break;
    case DEE_CFLAG_OPTIMIZER:
     if (enable) config->optimize_flags |= iter->flag;
     else config->optimize_flags &= ~iter->flag;
     break;
    case DEE_CFLAG_COMPILER:
     if (enable) config->compiler_flags |= iter->flag;
     else config->compiler_flags &= ~iter->flag;
     break;
    default: DEE_BUILTIN_UNREACHABLE();
   }
   found = 1;
  }
  ++iter;
 }
 if (!found) {
  if (!enable) arg -= 3;
  if (DeeFile_StdPrintf(DEE_STDERR,
   "CMD: Unknown compiler flag: %q\n",arg) != 0) return -1;
 }
 return 0;
}
static int _dee_print_features(void) {
 Dee_size_t feature_len;
 char const *iter; DeeObject *out_file;
 iter = _dee_features;
 out_file = DeeFile_Std(DEE_STDOUT);
 while (*iter) {
  feature_len = strlen(iter);
  if (DeeFile_WriteAll(out_file,iter,feature_len) != 0 ||
      DeeFile_PRINT(out_file,"\n") != 0) {
   Dee_DECREF(out_file);
   return -1;
  }
  iter += (feature_len+1);
 }
 Dee_DECREF(out_file);
 return 0;
}
DEE_A_RET_WUNUSED Dee_uint32_t Dee_GetVersion(void) {
 return DEE_VERSION_FULL;
}
DEE_A_RET_WUNUSED DEE_A_RET_Z
char const *Dee_GetVersionString(void) {
 return _dee_version_string;
}
DEE_A_RET_WUNUSED DEE_A_RET_Z
char const *Dee_GetFeatures(void) {
 return _dee_features;
}
DEE_A_RET_NOEXCEPT(0) int Dee_HasFeatures(
 DEE_A_IN_Z char const *name) {
 char const *iter = _dee_features;
 DEE_ASSERT(name);
 while (*iter)
  if (Dee_StrCmp(iter,name) == 0) return 1;
 else iter += strlen(iter)+1;
 return 0;
}
DEE_A_RET_NOEXCEPT(0) int Dee_HasWildFeatures(
 DEE_A_IN_Z char const *wildname) {
 char const *iter = _dee_features;
 DEE_ASSERT(wildname);
 while (*iter)
  if (Dee_StrWMatch(iter,wildname)) return 1;
 else iter += strlen(iter)+1;
 return 0;
}


DEE_A_RET_EXCEPT(-1) int Dee_Main(
 DEE_A_IN int argc, DEE_A_IN_R(argc) char **argv, DEE_A_OUT_OPT int *retval) {
#define DEE_CMD_MODE_DEFAULT      0
#define DEE_CMD_MODE_PREPROCESS   1
#define DEE_CMD_MODE_FORMAT       2
#define DEE_CMD_INKIND_AUTO       0
#define DEE_CMD_INKIND_CODE       1
#define DEE_CMD_INKIND_DATA       2
#define DEE_CMD_FLAG_NONE         0x00000000
#define DEE_CMD_FLAG_HELP_PRINTED 0x00000001
#define DEE_CMD_FLAG_NODEBUG      0x00000002
#define DEE_CMD_FLAG_NOSTDINC     0x00000004
#define DEE_CMD_FLAG_COMPILE_ONLY 0x00000008
#if DEE_CONFIG_RUNTIME_HAVE_UPDATE_CHECK
#define DEE_CMD_FLAG_NOUPDATE     0x00000010
#endif
 int temp,result,
  mode = DEE_CMD_MODE_DEFAULT,
  flags = DEE_CMD_FLAG_NONE,
  in_kind = DEE_CMD_INKIND_AUTO;
 DeeObject *fp,*lexer,*func,*script_file;
 struct DeeCompilerConfig config;
 struct DeePreprocessConfig pp_config = {NULL,1,0,0,0,0};
 ++argv,--argc; // consume the exe file argument
 // Setup the compiler
 if ((lexer = DeeLexer_New()) == NULL) goto err_always;
 config = DeeCompilerConfig_Default;

 while (argc && **argv == '-') {
  char *arg = (*argv++)+1; --argc;
#define CHECK_S(short)    (strcmp(arg,short)==0)
#define CHECK_L(long)     (strcmp(arg,"-" long)==0)
#define CHECK(short,long) (CHECK_S(short)||CHECK_L(long))
#if defined(DEE_DEBUG)
  if (CHECK("V","verbose")) _DeeFlag_Verbose = 1; else
  if (CHECK_S("VV")) _DeeFlag_Verbose = 2; else
  if (CHECK_S("VVV")) _DeeFlag_Verbose = 3; else
  if (CHECK_S("VVVV")) _DeeFlag_Verbose = 4; else
  if (CHECK("NAS","no-assert")) _DeeFlag_NoAssert = 1; else
#endif
  if (CHECK("h","help")) {
   fp = DeeFile_Std(DEE_STDERR);
   if (argc && strcmp(*argv,"compiler-flags") == 0) {
    --argc,++argv;
    temp = _dee_print_compiler_flags(&config,fp);
   } else {
    temp = PRINT_HELP(fp);
   }
   Dee_DECREF(fp);
   if (temp != 0) goto err_0;
   flags |= DEE_CMD_FLAG_HELP_PRINTED;
  } else
  if (*arg == 'O' && arg[1] >= '0' && arg[1] <= '3') {
   Dee_uint8_t opt_level = (Dee_uint8_t)(arg[1]-'0');
   switch (opt_level) {
    case 0:opt_min:
     config.compiler_flags = DEE_COMPILER_FLAG_FASTEST((flags&DEE_CMD_FLAG_NODEBUG)==0);
     config.optimize_flags = DEE_OPTIMIZE_FLAG_FASTEST((flags&DEE_CMD_FLAG_NODEBUG)==0);
     config.parser_flags   = DEE_PARSER_FLAG_FASTEST((flags&DEE_CMD_FLAG_NODEBUG)==0);
     break;
    case 2:
     config.compiler_flags = DEE_COMPILER_FLAG_OPTIMIZE((flags&DEE_CMD_FLAG_NODEBUG)==0);
     config.optimize_flags = DEE_OPTIMIZE_FLAG_OPTIMIZE((flags&DEE_CMD_FLAG_NODEBUG)==0);
     config.parser_flags   = DEE_PARSER_FLAG_OPTIMIZE((flags&DEE_CMD_FLAG_NODEBUG)==0);
     break;
    case 3:opt_max:
     config.compiler_flags = DEE_COMPILER_FLAG_OPTIMIZE_ALL((flags&DEE_CMD_FLAG_NODEBUG)==0);
     config.optimize_flags = DEE_OPTIMIZE_FLAG_OPTIMIZE_ALL((flags&DEE_CMD_FLAG_NODEBUG)==0);
     config.parser_flags   = DEE_PARSER_FLAG_OPTIMIZE_ALL((flags&DEE_CMD_FLAG_NODEBUG)==0);
     break;
    default:opt_def:
     config.compiler_flags = DEE_COMPILER_FLAG_DEFAULT((flags&DEE_CMD_FLAG_NODEBUG)==0);
     config.optimize_flags = DEE_OPTIMIZE_FLAG_DEFAULT((flags&DEE_CMD_FLAG_NODEBUG)==0);
     config.parser_flags   = DEE_PARSER_FLAG_DEFAULT((flags&DEE_CMD_FLAG_NODEBUG)==0);
     break;
   }
  } else if (CHECK_S("Omin")) { goto opt_min;
  } else if (CHECK_S("Odef")) { goto opt_def;
  } else if (CHECK_S("Omax")) { goto opt_max;
  } else if (CHECK("ndebug","no-debug")) {
   config.compiler_flags &= ~DEE_COMPILER_FLAG_DEBUG(1);
   config.optimize_flags &= ~DEE_OPTIMIZE_FLAG_DEBUG(1);
   config.parser_flags   &= ~DEE_PARSER_FLAG_DEBUG(1);
   config.compiler_flags |= DEE_COMPILER_FLAG_OPTIMIZE_ALL(0);
   config.optimize_flags |= DEE_OPTIMIZE_FLAG_OPTIMIZE_ALL(0);
   config.parser_flags   |= DEE_PARSER_FLAG_OPTIMIZE_ALL(0);
   if (TPPLexer_FastDefine(DeeLexer_LEXER(lexer),"NDEBUG","1") == -1) goto err_0;
   flags |= DEE_CMD_FLAG_NODEBUG;
  } else
  if (CHECK("nu","no-update")) {
#if DEE_CONFIG_RUNTIME_HAVE_UPDATE_CHECK
   flags |= DEE_CMD_FLAG_NOUPDATE;
#endif
  } else if (CHECK_L("update")) {
#if DEE_CONFIG_RUNTIME_HAVE_UPDATE_CHECK
   Dee_DECREF(_Dee_DoCheckForUpdates_F(Dee_EmptyTuple));
#endif
   flags |= DEE_CMD_FLAG_HELP_PRINTED;
  } else
#if DEBUG_NEW_CONFIG_ENABLED
  if (CHECK_L("debug-new-breakpoint")) { if (argc) {
   Dee_size_t break_id;
   if (DeeString_ToNumber(Dee_size_t,*argv,&break_id) != 0) {
    DeeError_Handled();
   } else DEBUG_NEW_API_ALLOC_BREAKPOINT(break_id);
   --argc,++argv;
  } } else
  if (CHECK_L("debug-new-hush")) {
   DEBUG_NEW_CXX_NAMESPACE_ROOT Dn_SetDefaultPrintFunc(&_debugnew_hushfunc);
  } else
#endif
  if (CHECK("version","version")) {
   fp = DeeFile_Std(DEE_STDOUT);
   temp = PRINT_VERSION(fp);
   Dee_DECREF(fp);
   if (temp != 0) goto err_0;
   flags |= DEE_CMD_FLAG_HELP_PRINTED;
  } else if (CHECK_L("features")) {
   if (_dee_print_features() != 0) goto err_0;
   flags |= DEE_CMD_FLAG_HELP_PRINTED;
  } else if (CHECK_L("has-feature")) {
   if (argc) {
    if (retval) *retval = Dee_HasWildFeatures(*argv) ? 0 : 1;
    Dee_DECREF(lexer);
    return 0;
   }
  } else if (*arg == 'I') {
   if (arg[1]) {
    if (_dee_parsearg_I(lexer,arg+1) == -1)
     goto err_0;
   } else if (argc) {
    --argc;
    if (_dee_parsearg_I(lexer,*argv++) == -1)
     goto err_0;
   }
  } else
  if (CHECK_S("E")) { mode = DEE_CMD_MODE_PREPROCESS; in_kind = DEE_CMD_INKIND_CODE; } else
  if (CHECK("F","format")) { mode = DEE_CMD_MODE_FORMAT; in_kind = DEE_CMD_INKIND_CODE; } else
  if (CHECK("in=auto","input=auto")) { in_kind = DEE_CMD_INKIND_AUTO; } else
  if (CHECK("in=code","input=code")) { in_kind = DEE_CMD_INKIND_CODE; } else
  if (CHECK("in=data","input=data")) { in_kind = DEE_CMD_INKIND_DATA; } else
  if (CHECK("c","compile")) { mode = DEE_CMD_MODE_DEFAULT; flags |= DEE_CMD_FLAG_COMPILE_ONLY; } else
  if (CHECK("nostdinc","nostdinc")) { flags |= DEE_CMD_FLAG_NOSTDINC; } else
  if (CHECK("no-line","no-line")) { pp_config.output_line_directives = 0; } else
  if (CHECK("tok","token")) { pp_config.output_tok_brackets = 1; } else
  if (CHECK_L("keep-comments")) { DeeLexer_LEXER(lexer)->l_flags |= TPPLexer_FLAG_TOK_COMMENTS; } else
  if (CHECK("o","out")) { if (argc) { --argc;
   Dee_XDECREF(pp_config.output_file);
   if ((pp_config.output_file = DeeFileIO_New(*argv++,"w")) == NULL) goto err_0;
  } } else
  if (*arg == 'C') { if (_dee_parsearg_C(&config,arg+1) == -1) goto err_0; } else
  if (CHECK_S("i")) {
   DeeObject *code_str; int temp;
   DeeStringWriter writer = DeeStringWriter_INIT();
   while (argc) {
    if (DeeStringWriter_WriteString(&writer,*argv++) == -1 ||
        DeeStringWriter_WriteChar(&writer,' ') == -1) {
     DeeStringWriter_Quit(&writer); goto err_0;
    }
    --argc;
   }
   code_str = DeeStringWriter_Pack(&writer);
   DeeStringWriter_Quit(&writer);
   if (!code_str) goto err_0;
   Dee_INCREF(script_file = Dee_None); // satisfy the decref below
   temp = DeeLexer_IncludeStringObjectEx(lexer,code_str,"<cmd>");
   Dee_DECREF(code_str);
   if (temp == -1) goto err_1;
   goto parse_and_compile;
  } else if (*arg == 'D') {
   if (_dee_parsearg_D(lexer,arg+1) == -1) goto err_0;
  } else {
   if (DeeFile_StdPrintf(DEE_STDERR,"CMD: Unknown argument: %q\n",arg-1) == -1) goto err_0;
  }
#undef CHECK
#undef CHECK_L
#undef CHECK_S
 }
 if (!argc) {
  if ((flags&DEE_CMD_FLAG_HELP_PRINTED)!=0) result = 0;
  else {
   fp = DeeFile_Std(DEE_STDERR);
   result = PRINT_HELP_SHORT(fp);
   Dee_DECREF(fp);
  }
  Dee_DECREF(lexer);
  if (retval) *retval = 0;
  goto end_always;
 }

#if DEE_CONFIG_RUNTIME_HAVE_UPDATE_CHECK
 // Check for updates if requested
 if ((flags&DEE_CMD_FLAG_NOUPDATE)==0) {
  _Dee_CheckForUpdates();
 }
#endif

 // Fix flag configuration
 if ((mode == DEE_CMD_MODE_PREPROCESS || mode == DEE_CMD_MODE_FORMAT) && in_kind != DEE_CMD_INKIND_CODE) {
  if (DeeFile_PRINT(DeeFile_Std(DEE_STDERR),"CMD: -E, -F requires -in=code\n") == -1) goto err_0;
  in_kind = DEE_CMD_INKIND_CODE;
 }

 // Include the STD folders
 if ((flags&DEE_CMD_FLAG_NOSTDINC) == 0 &&
     DeeLexer_AddStdInc(lexer) == -1) goto err_0;

 // Always use an absolute path for input filenames
 if ((script_file = DeeFS_PathAbs(*argv,".")) == NULL) goto err_0;
 *argv = DeeString_STR(script_file);

 if (mode != DEE_CMD_MODE_FORMAT) {
  DeeObject *input_stream;
  Dee_uint32_t version;
  DeeMarshalVersion marshal_ver;
  int temp;
  if ((input_stream = DeeFileIO_New(*argv,"r")) == NULL) {
err_io:
   Dee_DECREF(lexer);
   goto err_1;
  }
  if (in_kind == DEE_CMD_INKIND_AUTO) {
   if ((temp = DeeMarshal_IsCompiledFile(
    input_stream,&version,&marshal_ver)) == -1) {
    Dee_DECREF(input_stream);
    goto err_io;
   }
  } else if (in_kind == DEE_CMD_INKIND_DATA) {
   if ((temp = DeeMarshal_IsCompiledFile(
    input_stream,&version,&marshal_ver)) != 1) {
    if (temp == 0) {
     DeeError_SetStringf(&DeeErrorType_ValueError,
                         "Expected compiled deemon file in %k",
                         input_stream);
    }
    Dee_DECREF(input_stream);
    goto err_io;
   }
   goto inc_data;
  } else goto inc_code;
  if (temp) {
inc_data:
   // Compiled deemon file
   func = DeeMarshal_ReadObject(input_stream,marshal_ver);
   Dee_DECREF(input_stream);
   goto exec_func2;
  } else {
inc_code:
   // Assume deemon source file
   temp = DeeLexer_IncludeFileStreamEx(lexer,input_stream,*argv);
   Dee_DECREF(input_stream);
   if (temp == -1) {err_lex1: Dee_DECREF(lexer); goto err_1; }
  }
 }
parse_and_compile:
 if ((flags&DEE_CMD_FLAG_NODEBUG)==0) {
  if (TPPLexer_FastDefine(DeeLexer_LEXER(lexer),"_DEBUG","1") == -1) {
   Dee_DECREF(lexer);
   goto err_1;
  }
 }
 if (mode == DEE_CMD_MODE_PREPROCESS) {
  if (!pp_config.output_file)
   pp_config.output_file = DeeFile_Std(DEE_STDOUT);
  result = DeeLexer_PreprocessEx(lexer,&pp_config);
  Dee_DECREF(lexer);
  if (retval) *retval = 0;
  goto end_script;
 } else if (mode == DEE_CMD_MODE_FORMAT) {
  DeeObject *input_stream;
  if ((input_stream = DeeFileIO_New(
   *argv,pp_config.output_file ? "r" : "r+")) == NULL) goto err_lex1;
  result = DeeLexer_FormatFileStream(
   lexer,input_stream,pp_config.output_file
   ? pp_config.output_file : input_stream);
  Dee_DECREF(lexer);
  Dee_DECREF(input_stream);
  if (retval) *retval = 0;
  goto end_script;
 } else {
  DeeObject *old_args,*result_ob;
  // Parse and compile the function
  func = DeeLexer_ParseAndCompile(lexer,&config);
exec_func2:
  Dee_DECREF(lexer);
  if (func == NULL) goto err_1;
  Dee_ClearInstancePools(); // Do some clean-up
  DeeGC_CollectNow(); // Do some more clean-up

  // Output compiled code to file
  if (pp_config.output_file) {
   if (DeeSaveCompiled_FileStream(func,pp_config.output_file) == -1) goto err_2;
#ifdef DEE_PLATFORM_UNIX
   // v should always be the case, but might not be forever...
   if (DeeFileIO_Check(pp_config.output_file)) {
    if (DeeFileIO_Chmod(pp_config.output_file,0777) == -1) goto err_2;
   }
#endif
   Dee_CLEAR(pp_config.output_file);
  }

  // Execute the compiled code
  if ((flags&DEE_CMD_FLAG_COMPILE_ONLY)==0) {
   int usedretval;
   // Pack the rest of the cmd as arguments
   old_args = Dee_GetArgv();
   if (_dee_pack_args(argc,argv) == -1) goto err_2;
   // Call the generated function
   if ((result_ob = DeeObject_Call(func,Dee_EmptyTuple)) == NULL) goto err_3;
   // We simply cast the code return value to int,
   // because none converts to 0, which is what we want for no-error
   if (DeeObject_Cast(int,result_ob,&usedretval) != 0) { Dee_DECREF(result_ob); goto err_3; }
   Dee_DECREF(result_ob);
   if (retval) *retval = usedretval;

   Dee_SetArgv(old_args);
   Dee_XDECREF(old_args);
  } else {
   if (retval) *retval = 0;
  }
  result = 0;

  Dee_DECREF(func);
end_script:
  Dee_DECREF(script_file);
  goto end_always;
err_3: Dee_XDECREF(old_args);
err_2: Dee_DECREF(func);
 }
err_1:
 Dee_DECREF(script_file);
 goto err_always;
err_0:
 Dee_DECREF(lexer);
 //fallthrough
err_always:
 result = -1;
end_always:
 Dee_XDECREF(pp_config.output_file);
 return result;
#undef DEE_CMD_FLAG_COMPILE_ONLY
#undef DEE_CMD_FLAG_NOSTDINC
#undef DEE_CMD_FLAG_NODEBUG
#undef DEE_CMD_FLAG_HELP_PRINTED
#undef DEE_CMD_FLAG_NONE
#undef DEE_CMD_INKIND_DATA
#undef DEE_CMD_INKIND_CODE
#undef DEE_CMD_INKIND_AUTO
#undef DEE_CMD_MODE_FORMAT
#undef DEE_CMD_MODE_PREPROCESS
#undef DEE_CMD_MODE_DEFAULT
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_DEEMONRUN_C */
