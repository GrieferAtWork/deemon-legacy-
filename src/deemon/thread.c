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
#ifndef GUARD_DEEMON_THREAD_C
#define GUARD_DEEMON_THREAD_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

#ifndef DEE_WITHOUT_THREADS
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#ifdef DEE_PLATFORM_WINDOWS
#include <deemon/time.h>
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#else
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#include <errno.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_PTHREAD_H
#include <pthread.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SCHED_H
#include <sched.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_TIME_H
#include <time.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_SELECT_H
#include <sys/select.h>
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TIME_H && \
      DEE_ENVIRONMENT_HAVE_INCLUDE_SYS_TYPES_H && \
      DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif /* !DEE_WITHOUT_THREADS */

#include <deemon/__atomic_intrin.inl>
#include <deemon/bool.h>
#include <deemon/cfunction.h>
#include <deemon/compiler/code.h>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/float.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/memberdef.h>
#include <deemon/time.h>
#include <deemon/mp/thread.h>
#include <deemon/mp/tls.h>
#include <deemon/optional/atomic_once.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/runtime/execute.h>
#include <deemon/runtime/function.h>
#include <deemon/string.h>
#include <deemon/optional/object_visit.h>
#include DEE_INCLUDE_MEMORY_API()

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/posix_features.inl>
#include <deemon/sys/systls.h>


// */ (nano...)


DEE_DECL_BEGIN

//static
// Thread32First();
#ifndef DEE_WITHOUT_THREADS
#ifdef DEE_PLATFORM_WINDOWS
#if 0
// This <could> work under XP, but it doesn't under win8.
// And since I have no xp lying around to test it, I have disabled it.
// Based on this:
// http://undocumented.ntinternals.net/index.html?page=UserMode%2FUndocumented%20Functions%2FNT%20Objects%2FThread%2FTHREAD_INFORMATION_CLASS.html
DEE_COMPILER_MSVC_WARNING_PUSH(4701 4703)
static DWORD _win32_GetThreadId_XP(HANDLE Thread) {
 typedef struct _THREAD_BASIC_INFORMATION {
  NTSTATUS  ExitStatus;
  PVOID     TebBaseAddress;
  DWORD ClientId;
  KAFFINITY AffinityMask;
  LONG Priority;
  LONG BasePriority;
 } THREAD_BASIC_INFORMATION,*PTHREAD_BASIC_INFORMATION;
 THREAD_BASIC_INFORMATION info;
 typedef NTSTATUS (*NTAPI LPNTQUERYINFORMATIONTHREAD)(
  HANDLE ThreadHandle, int ThreadInformationClass,
  PVOID ThreadInformation, ULONG ThreadInformationLength, PULONG ReturnLength);
 LPNTQUERYINFORMATIONTHREAD p_NtQueryInformationThread;
 DEE_ATOMIC_ONCE({
  HMODULE hNtdll = GetModuleHandle(TEXT("NTDLL"));
  p_NtQueryInformationThread = (LPNTQUERYINFORMATIONTHREAD)
   GetProcAddress(hNtdll,"NtQueryInformationThread");
 });
 if (!p_NtQueryInformationThread) return (DWORD)Thread; // Ups...
 (*p_NtQueryInformationThread)(Thread,0,&info,sizeof(info),NULL);
 return info.ClientId;
}
DEE_COMPILER_MSVC_WARNING_POP
#endif
typedef DWORD (WINAPI *LPGETTHREADID)(HANDLE Thread);
static LPGETTHREADID p_GetThreadId = NULL;
#ifdef DEE_PLATFORM_64_BIT
static DWORD _win32_threadid_cast_handle(HANDLE h) {
 return (DWORD)(
  ((((Dee_uint64_t)h) & UINT64_C(0x00000000FFFFFFFF))      ) ^
  ((((Dee_uint64_t)h) & UINT64_C(0xFFFFFFFF00000000)) >> 32));
}
#else
#define _win32_threadid_cast_handle(h) (DWORD)(h)
#endif

static void _win32_load_GetThreadId(void) {
 DEE_ATOMIC_ONCE({
  HMODULE hKernel32 = GetModuleHandle(TEXT("KERNEL32"));
  p_GetThreadId = hKernel32 ? (LPGETTHREADID)GetProcAddress(hKernel32,"GetThreadId") : NULL;
 });
}
static DWORD _win32_GetThreadId(DEE_A_IN HANDLE Thread) {
 _win32_load_GetThreadId();
 // Vista and up
 if (p_GetThreadId) return (*p_GetThreadId)(Thread);
 return _win32_threadid_cast_handle(Thread); // Well $H17!
}
#endif
#endif /* !DEE_WITHOUT_THREADS */


DEE_A_RET_EXCEPT(-1) int _DeeRecursionList_Push(
 DEE_A_INOUT struct DeeRecursionList *self,
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeObject const *ob) {
 DEE_ASSERT(self);
 if DEE_UNLIKELY(self->rl_c == self->rl_a) {
  struct DeeRecursionListEntry *new_list;
  if DEE_LIKELY(self->rl_a) self->rl_a *= 2; else self->rl_a = 2;
  while DEE_UNLIKELY((new_list = (struct DeeRecursionListEntry *)realloc_nz(
   self->rl_v,self->rl_a*sizeof(struct DeeRecursionListEntry))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
  self->rl_v = new_list;
 }
 self->rl_v[self->rl_c].rle_type = tp_self;
 self->rl_v[self->rl_c].rle_self = ob;
 ++self->rl_c;
 return 0;
}
DEE_A_RET_NOEXCEPT(0) int _DeeRecursionList_Contains(
 DEE_A_IN struct DeeRecursionList const *self,
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeObject const *ob) {
 struct DeeRecursionListEntry *iter,*end;
 end = (iter = self->rl_v)+self->rl_c;
 while (iter != end) {
  if DEE_UNLIKELY(iter->rle_self == ob &&
                  iter->rle_type == tp_self) return 1;
  ++iter;
 }
 return 0;
}

#ifndef DEE_WITHOUT_THREADS
#if defined(DEE_PLATFORM_WINDOWS) && defined(_MSC_VER)
#define _dee_thread_set_name _dee_thread_set_name
DEE_STATIC_INLINE(void) _dee_thread_set_name(LPCSTR name) {
#pragma pack(push,8)
 typedef struct THREADNAME_INFO {
  DWORD  dwType;     // Must be 0x1000.
  LPCSTR szName;     // Pointer to name (in user addr space).
  DWORD  dwThreadID; // Thread ID (-1=caller thread).
  DWORD  dwFlags;    // Reserved for future use, must be zero.
 } THREADNAME_INFO;
#pragma pack(pop)
 THREADNAME_INFO info;
 info.dwType = 0x1000;
 info.szName = name;
 info.dwThreadID = GetCurrentThreadId();
 info.dwFlags = 0;
 __try {
  RaiseException(0x406D1388,0,sizeof(info)/sizeof(void *),(ULONG_PTR const *)&info);
 } 
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6320)
#pragma warning(disable: 6322)
#endif
 __except(1) {}
#ifdef _PREFAST_
#pragma warning(pop)
#endif
}
#elif DEE_HAVE_PTHREAD_SETNAME_NP
#define _dee_thread_set_name(name) \
 (pthread_setname_np(pthread_self(),name)?(void)(errno=0):(void)0)
#endif



#ifdef DEE_PLATFORM_WINDOWS
#define DeeThread_Win32PriorityToFloat(x)   ((float)((x)-THREAD_PRIORITY_LOWEST)/(THREAD_PRIORITY_HIGHEST-THREAD_PRIORITY_LOWEST))
#define DeeThread_Win32PriorityFromFloat(x)  (THREAD_PRIORITY_LOWEST+(int)((x)*(THREAD_PRIORITY_HIGHEST-THREAD_PRIORITY_LOWEST)))
#endif



// v The TLS that owns a reference to the thread-local thread control object (aka. thread.self())
static struct DeeSysTLS _dee_thread_threadself_tls;


#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
#define _DeeThread_INIT_COMMON_EXCEPT(ob)\
 (ob)->t_handling_exception = NULL,\
 DeeAtomicMutex_Init(&(ob)->t_handling_exception_lock),
#else
#define _DeeThread_INIT_COMMON_EXCEPT(ob) 
#endif
#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
#define _DeeThread_INIT_COMMON_FRAMESIZE(ob)\
 (ob)->t_frame_size = 0,
#else /* DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS */
#define _DeeThread_INIT_COMMON_FRAMESIZE(ob)
#endif /* DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS */

#define _DeeThread_INIT_COMMON(ob) (\
 _DeeThread_INIT_COMMON_EXCEPT(ob)\
 (ob)->t_priority = 0.5f,\
 (ob)->t_nointerrupt_rec = 0,\
 (ob)->t_name = NULL,\
 DeeAtomicMutex_Init(&(ob)->t_lock),\
 DeeAtomicMutex_Init(&(ob)->t_frame_lock),\
 _DeeThread_INIT_COMMON_FRAMESIZE(ob)\
 (ob)->t_frame_last = NULL,\
 DeeAtomicMutex_Init(&(ob)->t_exception_lock),\
 (ob)->t_exception = NULL,\
 _DeeRecursionList_Init(&(ob)->t_recursion_str),\
 _DeeRecursionList_Init(&(ob)->t_recursion_repr),\
 DeeAtomicMutex_Init(&(ob)->t_rt_lock),\
 (ob)->t_rt_tls_size = 0,\
 (ob)->t_rt_tls_list = NULL,\
 (ob)->t_rt_return = NULL,\
 (ob)->t_rt_rand = NULL,\
 (ob)->t_sys_interrupt = NULL\
)
#define _DeeThread_QUIT_COMMON(ob) (\
 _DeeRecursionList_Quit(&(ob)->t_recursion_str),\
 _DeeRecursionList_Quit(&(ob)->t_recursion_repr)\
)


//////////////////////////////////////////////////////////////////////////
// Clears out all references to ob_rt_* members
void _DeeThread_ClearRuntime(DeeThreadObject *thread, DeeObject *return_value) {
 DeeObject **old_tls,**iter,**end; Dee_size_t old_tls_size;
 DeeObject *old_return,*old_rand;
 DeeAtomicMutex_Acquire(&thread->t_rt_lock);
 old_tls_size = thread->t_rt_tls_size;
 old_tls = thread->t_rt_tls_list;
 old_return = thread->t_rt_return;
 old_rand = (DeeObject *)thread->t_rt_rand;
 thread->t_rt_tls_size = 0;
 thread->t_rt_tls_list = NULL;
 Dee_XINCREF(thread->t_rt_return = return_value);
 thread->t_rt_rand = NULL;
 DeeAtomicMutex_Release(&thread->t_rt_lock);
 Dee_XDECREF(old_rand);
 Dee_XDECREF(old_return);
 end = (iter = old_tls)+old_tls_size;
 while (iter != end) Dee_XDECREF(*iter++);
 free(old_tls);
}
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
void _DeeThread_ClearHandlingExceptions(DeeThreadObject *thread) {
 struct DeeRaisedException *old_exceptions,*ex_next;
 old_exceptions = thread->t_handling_exception;
 if (!old_exceptions) return;
 DeeAtomicMutex_Acquire(&thread->t_handling_exception_lock);
 thread->t_handling_exception = NULL;
 DeeAtomicMutex_Release(&thread->t_handling_exception_lock);
 // Free currently handled exceptions (should always be empty...)
 while (old_exceptions) {
  ex_next = old_exceptions->re_prev;
  _DeeRaisedException_Quit(old_exceptions);
  _DeeRaisedException_Free(old_exceptions);
  old_exceptions = ex_next;
 }
 DEE_ASSERT(thread->t_handling_exception == NULL &&
            "Dangling 't_handling_exception' in thread (even after cleanup?)");
}
#endif

DEE_A_RET_NOEXCEPT(0) int _DeeThread_HasFrame(
 DEE_A_IN DeeThreadObject const *self,
 DEE_A_IN struct DeeStackFrame const *frame_) {
 struct DeeStackFrame const *iter;
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 DEE_ASSERT(frame_);
 DeeAtomicMutex_AcquireRelaxed(&self->t_frame_lock);
 iter = self->t_frame_last;
 while (iter) {
  if (iter == frame_) {
   DeeAtomicMutex_Release(&self->t_frame_lock);
   return 1;
  }
  iter = iter->f_prev;
 }
 DeeAtomicMutex_Release(&self->t_frame_lock);
 return 0;
}




static /*atomic*/Dee_uint16_t _deethread_counter = 0;
       /*atomic*/Dee_uint16_t _deethread_itrpcounter = 0;
#define DEE_THREAD_COUNTER_INC()     DeeAtomic16_IncFetch(_deethread_counter,memory_order_seq_cst)
#define DEE_THREAD_COUNTER_DEC()     DeeAtomic16_DecFetch(_deethread_counter,memory_order_seq_cst)
#define DEE_THREADITRP_CHECK()      (DeeAtomic16_Load(_deethread_itrpcounter,memory_order_seq_cst)!=0)
#if 1
#define DEE_THREAD_COUNTER_WAIT()\
do{while(DeeAtomic16_Load(_deethread_counter,memory_order_seq_cst))DeeThread_SleepNoInterrupt(1);\
}while(0)
#else
#define DEE_THREAD_COUNTER_WAIT()\
do{while(DeeAtomic16_Load(_deethread_counter,memory_order_seq_cst))DeeThread_Yield();\
}while(0)
#endif



// Thread main() function (executed in own thread)
DEE_COMPILER_MSVC_WARNING_PUSH(4701 4703)
#ifdef DEE_PLATFORM_WINDOWS
DWORD WINAPI DeeThreadMain(DeeThreadObject *self)
#else
void *DeeThreadMain(DeeThreadObject *self)
#endif
{
 DeeObject *result,*callback,*args;
#ifdef _dee_thread_set_name
 DeeStringObject *name;
#endif /* _dee_thread_set_name */
#if defined(_MSC_VER) && defined(DEE_PLATFORM_WINDOWS)
 __try {
#endif
  // Setup the current thread TLS value
  // NOTE: Ignore errors in this call
  DeeSysTLS_TrySet(&_dee_thread_threadself_tls,self);
  DeeAtomicMutex_Acquire(&self->t_lock);
#ifdef _dee_thread_set_name
  Dee_XINCREF(name = self->t_name);
#endif /* _dee_thread_set_name */
  callback = self->t_callback_func;
  self->t_callback_func = NULL;
  args = (DeeObject *)self->t_callback_args;
  self->t_callback_args = NULL;
  DeeAtomicMutex_Release(&self->t_lock);
#ifdef _dee_thread_set_name
  // Notify the system of our custom name
  if (name) { _dee_thread_set_name(DeeString_STR(name)); Dee_DECREF(name); }
#endif /* _dee_thread_set_name */
  if DEE_LIKELY(callback) {
   // Actually execute the thread-function
   result = DeeObject_Call(callback,args ? args : Dee_EmptyTuple);
   Dee_DECREF(callback);
  } else {
   result = DeeNone_New(); // Just return 'none'
  }
  Dee_XDECREF(args);
  // Add the terminated flag & check if we were interrupted
  // NOTE: The terminated flag prevents new TLS variables from being assigned
  if ((DeeAtomic8_FetchOr(self->t_state,
   DeeThreadState_TERMINATED,memory_order_seq_cst)&(
   DeeThreadState_INTERRUPTED|DeeThreadState_INTERRUPT_HANDLED
   ))==DeeThreadState_INTERRUPTED) {
   // This thread had its interrupt flag set, but not its interrupt_handled
   DEE_LVERBOSE2("Semi-Interrupted thread has terminated: %r\n",self);
   // Notify the runtime that an interrupted thread has terminated
   // NOTE: We can't just do this in the part where we handle interrupt
   //       exceptions, as those could also be thrown by the user.
   DEE_THREADITRP_COUNTER_DEC();
  }
  // NOTE: We don't have to worry about any real exceptions remaining,
  //       as everything of the following is used for cleanup.
  //       >> And since destructors are noexcept, there won't be any
  //          exceptions dangling in this thread.
  // Check for (and handle) interrupt signals (internal & external)
  DEE_ASSERT((result != NULL) == (self->t_exception == NULL));
  while DEE_UNLIKELY(self->t_exception && DeeObject_InstanceOf(
   self->t_exception->re_error,&DeeSignalType_Interrupt)) {
   struct DeeRaisedException *exc;
   exc = self->t_exception;
   DeeAtomicMutex_Acquire(&self->t_exception_lock);
   self->t_exception = exc->re_prev;
   DeeAtomicMutex_Release(&self->t_exception_lock);
   _DeeRaisedException_Quit(exc);
   _DeeRaisedException_Free(exc);
   if DEE_UNLIKELY(!result) result = DeeNone_New(); // Interrupted threads return 'none'
  }
  _DeeThread_ClearRuntime(self,result);
  Dee_XDECREF(result);
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  _DeeThread_ClearHandlingExceptions(self);
#endif
  Dee_DECREF(self); // Finally release a reference from the thread itself
  // Notify the system that we are done
  // We try to do this with a finally block, because this really
  // __HAS__ to be called. - Otherwise the library shutdown hook freezes
#if defined(_MSC_VER) && defined(DEE_PLATFORM_WINDOWS)
 } __finally {
  DEE_THREAD_COUNTER_DEC();
 }
#else
 DEE_THREAD_COUNTER_DEC();
#endif
#if defined(DEE_PLATFORM_WINDOWS)
 return (DWORD)(result ? 0 : 1);
#else
 return (void *)((Dee_size_t)(result ? 0 : 1));
#endif
}
DEE_COMPILER_MSVC_WARNING_POP















DEE_A_RET_NOEXCEPT(-1) int DeeThread_Initialize(void) {
 DeeSysTLS_Init(&_dee_thread_threadself_tls,{return -1;});
 return 0;
}
void DeeThread_PrepareFinalize(void) {
 // Shutdown our own thread information block
 DeeThread_Shutdown();
 // Wait for all other threads to finish
 DEE_THREAD_COUNTER_WAIT();
}
void DeeThread_Finalize(void) {
 DeeThread_PrepareFinalize();
 // Destroy the tls we used to implement 'DeeThread_Self()'
 DeeSysTLS_Quit(&_dee_thread_threadself_tls);
}

int DeeThread_Shutdown(void) {
 DeeThreadObject *thread;
 DeeSysTLS_TryGetNofail(&_dee_thread_threadself_tls,thread);
 if DEE_LIKELY(thread != NULL) {
  DeeSysTLS_TrySet(&_dee_thread_threadself_tls,NULL);
  _DeeThread_ClearRuntime(thread,NULL);
  Dee_DECREF(thread);
  return 1;
 }
 return 0;
}



DEE_A_RET_OBJECT_MAYBE(DeeThreadObject) *DeeThread_Self(void) {
 // NOTE: This function can't raise any exceptions, because
 //       the exception system requires a working DeeThread_Self()
 DeeThreadObject *result;
 DeeSysTLS_TryGetNofail(&_dee_thread_threadself_tls,result);
 if DEE_UNLIKELY(result == NULL) {
  if DEE_LIKELY((result = (DeeThreadObject *)DeeObject_TryWeakMalloc(sizeof(DeeThreadObject))) != NULL) {
   DeeObject_INIT(result,&DeeThread_Type);
   // Detached, because we don't have control over this thread
   result->t_state = (DeeThreadState_STARTED|DeeThreadState_DETACHED);
#ifdef DEE_PLATFORM_WINDOWS
   result->t_id = GetCurrentThreadId();
   result->t_handle = GetCurrentThread();
#else
   result->t_handle = pthread_self();
#endif
   result->t_callback_func = NULL;
   result->t_callback_args = NULL;
   _DeeThread_INIT_COMMON(result);
   if DEE_UNLIKELY(!DeeSysTLS_TrySet(&_dee_thread_threadself_tls,result)) Dee_CLEAR(result);
  }
 }
 return (DeeObject *)result;
}






#define DeeThread_Init(ob,callback,args)\
do{\
 DEE_ASSERT(DeeObject_Check(ob) && DeeThread_Check(ob));\
 DEE_ASSERT(DeeObject_Check(callback));\
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));\
 DEE_LVERBOSE2("Initializing thread calling %k with %k\n",callback,args);\
 (ob)->t_state = DeeThreadState_INITIAL;\
 Dee_INCREF((ob)->t_callback_func = callback);\
 Dee_INCREF((ob)->t_callback_args = (DeeTupleObject *)args);\
 _DeeThread_INIT_COMMON(ob);\
}while(0)

DEE_A_RET_OBJECT_EXCEPT_REF(DeeThreadObject) *DeeThread_New(
 DEE_A_INOUT DeeObject *callback, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DeeThreadObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeThreadObject,
  "thread (calling %p with %p)",callback,args)) != NULL) {
  DeeObject_INIT(result,&DeeThread_Type);
  DeeThread_Init(result,callback,args);
 }
 return (DeeObject *)result;
}

DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeThread_Start(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self) {
 DeeThreadObject *thread;
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 thread = (DeeThreadObject *)self;
 DeeAtomicMutex_Acquire(&thread->t_lock);
 if DEE_UNLIKELY((DeeAtomic8_FetchOr(thread->t_state,DeeThreadState_STARTED,
  memory_order_seq_cst)&DeeThreadState_STARTED) != 0) {
  DeeAtomicMutex_Release(&thread->t_lock);
  return 1; // Thread already started/starting
 }
 Dee_INCREF(self); // reference owned by the thread (or removed later on error)
 DEE_THREAD_COUNTER_INC(); // Runtime information for running threads
 {
#ifdef DEE_PLATFORM_WINDOWS
#define COMMON_ERROR_CLEANUP() \
do{\
 Dee_DECREF(self);\
 DEE_THREAD_COUNTER_DEC();\
 DeeAtomic8_FetchAdd(thread->t_state,~DeeThreadState_STARTED,memory_order_seq_cst);\
 DeeAtomicMutex_Release(&thread->t_lock);\
}while(0)
  HANDLE new_handle;
  if DEE_UNLIKELY((new_handle = CreateThread(NULL,0,(
   LPTHREAD_START_ROUTINE)&DeeThreadMain,self,CREATE_SUSPENDED,
   &((DeeThreadObject *)self)->t_id)) == NULL) {
   COMMON_ERROR_CLEANUP();
   DeeError_SystemError("CreateThread");
   return -1;
  }
  if (thread->t_priority != 0.5) {
   if DEE_UNLIKELY(!SetThreadPriority(new_handle,DeeThread_Win32PriorityFromFloat(thread->t_priority))) {
    COMMON_ERROR_CLEANUP();
    DeeError_SetStringf(&DeeErrorType_SystemError,
                        "SetThreadPriority(%p,%lu) : %K",new_handle,
                        DeeThread_Win32PriorityFromFloat(thread->t_priority),
                        DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
cleanup_handle:
    if DEE_UNLIKELY(!CloseHandle(new_handle)) SetLastError(0);
    return -1;
   }
  }
  if DEE_UNLIKELY(ResumeThread(new_handle) == (DWORD)-1) {
   COMMON_ERROR_CLEANUP();
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "ResumeThread(%p) : %K",new_handle,
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
   goto cleanup_handle;
  }
#undef COMMON_ERROR_CLEANUP
  thread->t_handle = new_handle;
#else
  pthread_attr_t attr;
  int error = pthread_attr_init(&attr);
  if DEE_UNLIKELY(error != 0) {
   DeeError_SystemErrorExplicit("pthread_attr_init",error);
err_0: Dee_DECREF(self);
   DEE_THREAD_COUNTER_DEC();
   DeeAtomic8_FetchAdd(thread->t_state,~DeeThreadState_STARTED,memory_order_seq_cst);
   DeeAtomicMutex_Release(&thread->t_lock);
   return -1;
  }
  error = pthread_create(&thread->t_handle,&attr,(void*(*)(void *))&DeeThreadMain,self);
  pthread_attr_destroy(&attr);
  if DEE_UNLIKELY(error != 0) {
   DeeError_SystemErrorExplicit("pthread_create",error);
   goto err_0;
  }
  if (thread->t_priority != 0.5) {
   // TODO: Set priority
  }
#endif
 }
 DeeAtomicMutex_Release(&thread->t_lock);
 DEE_LVERBOSE2("Started Thread %r\n",self);
 return 0;
}


DEE_A_RET_WUNUSED DeeThreadID DeeThread_ID(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 DeeAtomicMutex_Acquire(&((DeeThreadObject *)self)->t_lock);
 if DEE_UNLIKELY((DeeThread_STATE_A(self)&DeeThreadState_STARTED) != DeeThreadState_STARTED) {
  DeeAtomicMutex_Release(&((DeeThreadObject *)self)->t_lock);
  return (DeeThreadID)-1;
 }
 {
#ifdef DEE_PLATFORM_WINDOWS
  DeeThreadID result;
#if 1
  result = ((DeeThreadObject *)self)->t_id;
#else
  result = (DeeThreadID)_win32_GetThreadId(DeeThread_HANDLE(self));
#endif
  DeeAtomicMutex_Release(&((DeeThreadObject *)self)->t_lock);
  return result;
#elif DEE_HAVE_PTHREAD_GETUNIQUE_NP
  pthread_id_np_t tid;
  pthread_getunique_np(&((DeeThreadObject *)self)->t_handle,&tid);
  DeeAtomicMutex_Release(&((DeeThreadObject *)self)->t_lock);
  return *(DeeThreadID*)&tid;
#else
  DeeThreadID result;
  // Not really an ID, but can still be used in output
  result = (DeeThreadID)((Dee_size_t)&((DeeThreadObject *)self)->t_handle);
  DeeAtomicMutex_Release(&((DeeThreadObject *)self)->t_lock);
  return result;
#endif
 }
}


#undef DeeThread_SelfID
DEE_A_RET_WUNUSED DeeThreadID DeeThread_SelfID(void) {
#ifdef DEE_PLATFORM_WINDOWS
 _win32_load_GetThreadId();
 // Keep it consistently incorrect
 return DEE_LIKELY(p_GetThreadId)
  ? (DeeThreadID)GetCurrentThreadId()
  : (DeeThreadID)_win32_threadid_cast_handle(GetCurrentThread());
#elif DEE_HAVE_PTHREAD_GETUNIQUE_NP
 pthread_id_np_t tid = pthread_getthreadid_np();
 return *(DeeThreadID*)&tid;
#else
 return DeeThread_ID(DeeThread_Self());
#endif
}


DEE_A_RET_WUNUSED int DeeThread_Started(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 return DeeThread_STARTED(self);
}
DEE_A_RET_WUNUSED int DeeThread_Detached(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 return DeeThread_DETACHED(self);
}
DEE_A_RET_WUNUSED int DeeThread_Terminated(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 return DeeThread_TERMINATED(self);
}
DEE_A_RET_WUNUSED int DeeThread_Interrupted(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 return DeeThread_INTERRUPTED(self);
}
DEE_A_RET_WUNUSED int DeeThread_Crashed(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 if DEE_UNLIKELY(!DeeThread_TERMINATED(self)) return 0;
 return ((DeeThreadObject *)self)->t_exception != NULL;
}
DEE_A_RET_WUNUSED DeeThreadState DeeThread_State(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 return DeeThread_STATE_A(self);
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeThread_GetError(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self,
 DEE_A_OUT_OPT DeeObject **exc, DEE_A_OUT_OPT DeeObject **tb) {
 struct DeeRaisedException *entry;
 if DEE_UNLIKELY(!DeeThread_TERMINATED(self)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,"Thread %k not terminated",self);
  return -1;
 }
 DeeAtomicMutex_AcquireRelaxed(&((DeeThreadObject *)self)->t_exception_lock);
 if DEE_UNLIKELY((entry = ((DeeThreadObject *)self)->t_exception) == NULL) {
  DeeAtomicMutex_Release(&((DeeThreadObject *)self)->t_exception_lock);
  return 0;
 }
 DEE_ASSERT(DeeObject_Check(entry->re_error));
 DEE_ASSERT(DeeObject_Check(entry->re_traceback));
 if (exc) Dee_INCREF(*exc = entry->re_error);
 if (tb) Dee_INCREF(*tb = (DeeObject *)entry->re_traceback);
 DeeAtomicMutex_Release(&((DeeThreadObject *)self)->t_exception_lock);
 return 1;
}

DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeThread_Interrupt(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self) {
 DeeThreadObject *thread;
 DeeThreadState state;
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 thread = (DeeThreadObject *)self;
 do {
  state = DeeThread_STATE_A(thread);
  if DEE_UNLIKELY((state & (DeeThreadState_STARTED|
   DeeThreadState_DETACHED))!=DeeThreadState_STARTED) {
   DeeError_SetStringf(&DeeErrorType_ValueError,"Thread %k not started or detached",thread);
   return -1;
  }
  if DEE_UNLIKELY((state & DeeThreadState_INTERRUPTED)!=0) return 1; // already interrupted
 } while DEE_UNLIKELY(!DeeAtomic8_CompareExchangeWeak(
  thread->t_state,state,state|DeeThreadState_INTERRUPTED, // Set the interrupted flag
  memory_order_seq_cst,memory_order_seq_cst));
 // Notify the runtime of the new interrupted thread
 DEE_THREADITRP_COUNTER_INC();
 DEE_LVERBOSE2("Interrupted Thread %r\n",self);
 return 0;
}

DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeThread_Detach(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self) {
 DeeThreadObject *thread; DeeThreadState state;
#ifndef DEE_PLATFORM_WINDOWS
 int error;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 thread = (DeeThreadObject *)self;
 do {
  state = DeeThread_STATE_A(thread);
  if DEE_UNLIKELY((state & DeeThreadState_STARTED)==0) {
   DeeError_SetStringf(&DeeErrorType_ValueError,"Thread %k not started",thread);
   return -1;
  }
  if DEE_UNLIKELY((state & DeeThreadState_DETACHED)!=0) return 1; // already detached
 } while DEE_UNLIKELY(!DeeAtomic8_CompareExchangeWeak(
  thread->t_state,state,state|DeeThreadState_DETACHED, // Set the detached flag
  memory_order_seq_cst,memory_order_seq_cst));
 {
#ifdef DEE_PLATFORM_WINDOWS
  HANDLE old_thread;
  DeeAtomicMutex_Acquire(&thread->t_lock);
  old_thread = thread->t_handle;
  thread->t_handle = NULL;
  thread->t_id = 0;
  DeeAtomicMutex_Release(&thread->t_lock);
  if DEE_UNLIKELY(!CloseHandle(old_thread)) {
   DeeError_SystemError("CloseHandle");
   return -1;
  }
#else
  DeeAtomicMutex_Acquire(&thread->t_lock);
  error = pthread_detach(thread->t_handle);
  DeeAtomicMutex_Release(&thread->t_lock);
  if DEE_UNLIKELY(error != 0) {
   DeeError_SystemErrorExplicit("pthread_detach",error);
   return -1;
  }
#endif
 }
 DEE_LVERBOSE2("Detached Thread %r\n",self);
 return 0;
}


static int _deethread_setup_join_or_detach(DeeThreadObject *self) {
 DeeThreadState state;
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 do {
  state = DeeThread_STATE_A(self);
  if DEE_UNLIKELY((state & (DeeThreadState_STARTED|
   DeeThreadState_DETACHED))!=DeeThreadState_STARTED) {
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Thread %k not started or detached/joined",
                       self);
   return -1;
  }
 } while DEE_UNLIKELY(!DeeAtomic8_CompareExchangeWeak(
  self->t_state,state,state|DeeThreadState_DETACHED, // Set the detached flag
  memory_order_seq_cst,memory_order_seq_cst));
 return 0;
}
#define _deethread_undo_join_or_detach(self)\
 DeeAtomic8_FetchAnd((self)->t_state,~DeeThreadState_DETACHED,memory_order_seq_cst);

#define _DeeThread_StoreReturn(self,return_value)\
do{\
 DeeObject *tmp_retval;\
 DeeAtomicMutex_Acquire(&((DeeThreadObject *)(self))->t_rt_lock);\
 tmp_retval = ((DeeThreadObject *)(self))->t_rt_return;\
 ((DeeThreadObject *)(self))->t_rt_return = NULL;\
 DeeAtomicMutex_Release(&((DeeThreadObject *)(self))->t_rt_lock);\
 if (return_value) {\
  /* Transfer reference */\
  /* - no incref in *return_value here, because the reference is inherited */\
  if DEE_UNLIKELY((*(return_value) = tmp_retval) == NULL) {\
   DeeError_ThreadCrash(self);\
   return -1;\
  }\
 } else {\
  if (tmp_retval) Dee_DECREF(tmp_retval);\
 }\
}while(0)

#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6103)
#endif
DEE_A_RET_EXCEPT(-1) int DeeThread_Join(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **return_value) {
#ifdef DEE_PLATFORM_WINDOWS
 return DeeThread_JoinTimed(self,INFINITE,return_value);
#else
 DeeThreadState state; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if DEE_UNLIKELY(_deethread_setup_join_or_detach((DeeThreadObject *)self) != 0) return -1;
 error = pthread_join(((DeeThreadObject *)self)->t_handle,NULL);
 if DEE_UNLIKELY(error != 0) {
  DeeError_SystemErrorExplicit("pthread_join",error);
  _deethread_undo_join_or_detach((DeeThreadObject *)self);
  return -1;
 }
 _DeeThread_StoreReturn(self,return_value);
 return 0;
#endif
}
#ifdef _PREFAST_
#pragma warning(pop)
#endif

DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeThread_TryJoin(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **return_value) {
#ifdef DEE_PLATFORM_WINDOWS
 return DeeThread_JoinTimed(self,0,return_value);
#else
 int error;
 if DEE_UNLIKELY(_deethread_setup_join_or_detach((DeeThreadObject *)self) != 0) return -1;
#if DEE_HAVE_PTHREAD_TRYJOIN_NP
 error = pthread_tryjoin_np(((DeeThreadObject *)self)->t_handle,NULL);
 if DEE_UNLIKELY(error != 0) {
  _deethread_undo_join_or_detach((DeeThreadObject *)self);
  if (error == EBUSY) return 1; // Still running
  DeeError_SystemErrorExplicit("pthread_tryjoin_np",error);
  return -1;
 }
 _DeeThread_StoreReturn(self,return_value);
 return 0;
#else
 if ((DeeThread_STATE_A(self)&DeeThreadState_TERMINATED)!=0) {
  // Thread has terminated (we can simply join to simulate a try_join)
  error = pthread_join(((DeeThreadObject *)self)->t_handle,NULL);
  if DEE_UNLIKELY(error != 0) {
   _deethread_undo_join_or_detach((DeeThreadObject *)self);
   DeeError_SystemErrorExplicit("pthread_join",error);
   return -1;
  }
  _DeeThread_StoreReturn(self,return_value);
  return 0;
 } else {
  _deethread_undo_join_or_detach((DeeThreadObject *)self);
  return 1; // still running
 }
#endif
#endif
}

DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeThread_JoinTimed(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self, DEE_A_IN unsigned int msecs,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **return_value) {
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if DEE_UNLIKELY(_deethread_setup_join_or_detach((DeeThreadObject *)self) != 0) return -1;
#ifdef DEE_PLATFORM_WINDOWS
 switch (WaitForSingleObject(((DeeThreadObject *)self)->t_handle,msecs)) {
  case WAIT_ABANDONED:
  case WAIT_OBJECT_0:
   break;
  case WAIT_TIMEOUT:
   _deethread_undo_join_or_detach((DeeThreadObject *)self);
   return 1; // Couldn't join
  default: break;
 }
 _DeeThread_StoreReturn(self,return_value);
 return 0;
#elif DEE_HAVE_PTHREAD_TIMEDJOIN_NP
 {int error;
  struct timespec join_time;
  join_time.tv_nsec = ((long)msecs%1000L)*1000000L;
  join_time.tv_sec = (time_t)(msecs/1000u);
  DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
  if (_deethread_setup_join_or_detach((DeeThreadObject *)self) == -1) return -1;
  error = pthread_timedjoin_np(((DeeThreadObject *)self)->t_handle,NULL,&join_time);
  if DEE_UNLIKELY(error != 0) {
   _deethread_undo_join_or_detach((DeeThreadObject *)self);
   if (error == ETIMEDOUT) return 1; // Timeout
   DeeError_SystemErrorExplicit("pthread_timedjoin_np",error);
   return -1;
  }
  _DeeThread_StoreReturn(self,return_value);
  return 0;
 }
#else
 int result; Dee_uint64_t end_time = DeeTime_GetClock1000()+(Dee_uint64_t)msecs;
 while ((result = DeeThread_TryJoin(self,return_value)) == 1 &&
        DeeTime_GetClock1000() < end_time
        ) if DEE_UNLIKELY((result = DeeThread_Sleep(1)) != 0) break;
 return result;
#endif
}
#undef _DeeThread_StoreReturn


DEE_A_RET_EXCEPT(-1) double DeeThread_GetPriority(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) {
 double result;
 DeeThreadObject *thread; DeeThreadState state;
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 thread = (DeeThreadObject *)self;
 DeeAtomicMutex_Acquire(&thread->t_lock);
 state = DeeThread_STATE_A(thread);
 // Make sure the thread isn't detached
 if DEE_UNLIKELY((state&DeeThreadState_DETACHED)!=0) {
  DeeAtomicMutex_Release(&thread->t_lock);
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Thread %k detached/joined",
                      thread);
  return -1.0;
 }
 if ((state&DeeThreadState_STARTED)!=0) {
#ifdef DEE_PLATFORM_WINDOWS
  int error = GetThreadPriority(thread->t_handle);
  if DEE_UNLIKELY(error == THREAD_PRIORITY_ERROR_RETURN) {
   DeeAtomicMutex_Release(&thread->t_lock);
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "GetThreadPriority(%p) : %K",thread->t_handle,
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
   return -1.0;
  }
  result = DeeThread_Win32PriorityToFloat(error);
#else
  result = 0.5; // TODO: Get priority
#endif
 } else {
  result = thread->t_priority; // Priority before launch
 }
 DeeAtomicMutex_Release(&thread->t_lock);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeThread_SetPriority(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self, DEE_A_IN double priority) {
 DeeThreadObject *thread; DeeThreadState state;
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 thread = (DeeThreadObject *)self;
 if DEE_UNLIKELY(priority < 0.0f) priority = 0.0f; else
 if DEE_UNLIKELY(priority > 1.0f) priority = 1.0f;
 DeeAtomicMutex_Acquire(&thread->t_lock);
 state = DeeThread_STATE_A(thread);
 // Make sure the thread isn't detached
 if DEE_UNLIKELY((state&DeeThreadState_DETACHED)!=0) {
  DeeAtomicMutex_Release(&thread->t_lock);
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Thread %k detached/joined",
                      thread);
  return -1;
 }
 if ((state & DeeThreadState_STARTED) != 0) {
#ifdef DEE_PLATFORM_WINDOWS
  if DEE_UNLIKELY(!SetThreadPriority(thread->t_handle,DeeThread_Win32PriorityFromFloat(priority))) {
   DWORD error = DeeSystemError_Win32Consume();
   DeeAtomicMutex_Release(&thread->t_lock);
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "SetThreadPriority(%p,%lu) : %K",
                       thread->t_handle,DeeThread_Win32PriorityFromFloat(priority),
                       DeeSystemError_Win32ToString(error));
   return -1;
  }
  DeeAtomicMutex_Release(&thread->t_lock);
#else
  // TODO: Set priority
#endif
  return 0;
 }
 thread->t_priority = priority; // Priority before launch
 DeeAtomicMutex_Release(&thread->t_lock);
 return 0;
}


DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeStringObject) *DeeThread_GetName(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 DeeAtomicMutex_Acquire(&((DeeThreadObject *)self)->t_lock);
 Dee_INCREF(result = DeeThread_NAME(self));
 DeeAtomicMutex_Release(&((DeeThreadObject *)self)->t_lock);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeThread_SetName(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name) {
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 // Make sure the thread isn't started
 DeeAtomicMutex_Acquire(&((DeeThreadObject *)self)->t_lock);
 if DEE_UNLIKELY((DeeThread_STATE_A(self) & DeeThreadState_STARTED)!=0) {
  DeeAtomicMutex_Release(&((DeeThreadObject *)self)->t_lock);
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Thread %k already started",
                      self);
  return -1;
 }
 Dee_XDECREF(((DeeThreadObject *)self)->t_name);
 Dee_INCREF(((DeeThreadObject *)self)->t_name = (DeeStringObject *)name);
 DeeAtomicMutex_Release(&((DeeThreadObject *)self)->t_lock);
 return 0;
}


DEE_A_RET_EXCEPT(-1) int _DeeThread_DoCheckInterrupt(void) {
 DeeThreadObject *thread; DeeObject *interrupt_signal;
 DeeSysTLS_TryGetNofail(&_dee_thread_threadself_tls,thread);
 if DEE_LIKELY(thread != NULL) {
  DeeThreadState state;
  do {
   state = DeeThread_STATE_A(thread);
   // Only allow interrupt if:
   //  - we're not inside of a nointerrupt block
   //  - we actually are being interrupted
   //  - we haven't already handled the interrupt
   if DEE_LIKELY((state & (
    DeeThreadState_NOINTERRUPT|
    DeeThreadState_INTERRUPTED|
    DeeThreadState_INTERRUPT_HANDLED
    )) != DeeThreadState_INTERRUPTED) return 0;
  } while DEE_UNLIKELY(!DeeAtomic8_CompareExchangeWeak(
  thread->t_state,state,state|DeeThreadState_INTERRUPT_HANDLED, // Set the interrupted_handled flag
  memory_order_seq_cst,memory_order_seq_cst));
  // This thread is no longer interrupted
  // >> Disable the optimized lookup for interrupted threads
  DEE_THREADITRP_COUNTER_DEC();
  if DEE_LIKELY((interrupt_signal = DeeType_NewInstanceDefault(&DeeSignalType_Interrupt)) != NULL) {
   thread->t_sys_interrupt = (struct DeeSignalObject *)interrupt_signal;
   DeeError_Throw(interrupt_signal);
   Dee_DECREF(interrupt_signal);
  }
  return -1;
 }
 return 0;
}

#ifndef __INTELLISENSE__
#undef DeeThread_CheckInterrupt
#endif
DEE_A_RET_EXCEPT(-1) int DeeThread_CheckInterrupt(void)
#ifndef __INTELLISENSE__
#define DeeThread_CheckInterrupt  DeeThread_CHECKINTERRUPT
#endif
{
 // Optimization: No need to load and check our TLS block,
 // if there aren't even any interrupted threads to begin with.
 if DEE_LIKELY(!DEE_THREADITRP_CHECK()) return 0;
 // At this point we know that at least something was interrupted.
 return _DeeThread_DoCheckInterrupt();
}

void DeeThread_NoInterruptBegin(void) {
 DeeThreadObject *thread_self = DeeThread_SELF();
 if (DEE_LIKELY(thread_self) && ++thread_self->t_nointerrupt_rec == 1)
  DeeAtomic8_FetchOr(thread_self->t_state,DeeThreadState_NOINTERRUPT,memory_order_seq_cst);
}
void DeeThread_NoInterruptEnd(void) {
 DeeThreadObject *thread_self = DeeThread_SELF();
 if (DEE_LIKELY(thread_self) && --thread_self->t_nointerrupt_rec == 0)
  DeeAtomic8_FetchAnd(thread_self->t_state,~DeeThreadState_NOINTERRUPT,memory_order_seq_cst);
}
#ifdef DEE_PLATFORM_WINDOWS
DEE_A_RET_EXCEPT(-1) int DeeThread_Win32Times(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **creation_time,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **exit_time,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **kernel_time,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **user_time) {
 FILETIME tc,te,tk,tu; BOOL temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 DeeAtomicMutex_Acquire(&((DeeThreadObject *)self)->t_lock);
 if DEE_UNLIKELY((DeeThread_STATE_A(self)&(DeeThreadState_STARTED|
              DeeThreadState_DETACHED)) != DeeThreadState_STARTED) {
  DeeAtomicMutex_Release(&((DeeThreadObject *)self)->t_lock);
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Thread %k not started or detached",
                      self);
  return -1;
 }
 temp = GetThreadTimes(DeeThread_HANDLE(self),&tc,&te,&tk,&tu);
 DeeAtomicMutex_Release(&((DeeThreadObject *)self)->t_lock);
 if DEE_UNLIKELY(!temp) { DeeError_SystemError("GetThreadTimes"); return -1; }
 if (creation_time && (*creation_time = DeeTime_NewFromWin32FileTime(&tc)) == NULL) return -1;
 if (exit_time && (*exit_time = DeeTime_NewFromWin32FileTime(&te)) == NULL) goto err_0;
 if (kernel_time && (*kernel_time = DeeTime_NewFromWin32FileTime0(&tk)) == NULL) goto err_1;
 if (user_time && (*user_time = DeeTime_NewFromWin32FileTime0(&tu)) == NULL) goto err_2;
 return 0;
err_2: if (kernel_time) Dee_DECREF(*kernel_time);
err_1: if (exit_time) Dee_DECREF(*exit_time);
err_0: if (creation_time) Dee_DECREF(*creation_time);
 return -1;
}
DEE_A_RET_EXCEPT(-1) int DeeThread_Win32PendingIO(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) {
 BOOL result,temp;
 DeeAtomicMutex_Acquire(&((DeeThreadObject *)self)->t_lock);
 if DEE_UNLIKELY((DeeThread_STATE_A(self)&(DeeThreadState_STARTED|
              DeeThreadState_DETACHED)) != DeeThreadState_STARTED) {
  DeeAtomicMutex_Release(&((DeeThreadObject *)self)->t_lock);
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Thread %k not started or detached",
                      self);
  return -1;
 }
 temp = GetThreadIOPendingFlag(DeeThread_HANDLE(self),&result);
 DeeAtomicMutex_Release(&((DeeThreadObject *)self)->t_lock);
 if DEE_UNLIKELY(!temp) { DeeError_SystemError("GetThreadIOPendingFlag"); return -1; }
 return (int)result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeThread_Win32GetPriorityBoost(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) {
 BOOL result,temp;
 DeeAtomicMutex_Acquire(&((DeeThreadObject *)self)->t_lock);
 if DEE_UNLIKELY((DeeThread_STATE_A(self)&(DeeThreadState_STARTED|
              DeeThreadState_DETACHED)) != DeeThreadState_STARTED) {
  DeeAtomicMutex_Release(&((DeeThreadObject *)self)->t_lock);
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Thread %k not started or detached",
                      self);
  return -1;
 }
 temp = GetThreadPriorityBoost(DeeThread_HANDLE(self),&result);
 DeeAtomicMutex_Release(&((DeeThreadObject *)self)->t_lock);
 if DEE_UNLIKELY(!temp) { DeeError_SystemError("GetThreadPriorityBoost"); return -1; }
 return !result;
}
DEE_A_RET_EXCEPT(-1) int DeeThread_Win32SetPriorityBoost(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self, DEE_A_IN int enabled) {
 BOOL temp;
 DeeAtomicMutex_Acquire(&((DeeThreadObject *)self)->t_lock);
 if DEE_UNLIKELY((DeeThread_STATE_A(self)&(DeeThreadState_STARTED|
              DeeThreadState_DETACHED)) != DeeThreadState_STARTED) {
  DeeAtomicMutex_Release(&((DeeThreadObject *)self)->t_lock);
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Thread %k not started or detached",
                      self);
  return -1;
 }
 temp = SetThreadPriorityBoost(DeeThread_HANDLE(self),enabled ? FALSE : TRUE);
 DeeAtomicMutex_Release(&((DeeThreadObject *)self)->t_lock);
 if DEE_UNLIKELY(!temp) { DeeError_SystemError("SetThreadPriorityBoost"); return -1; }
 return 0;
}
#endif







int _deethread_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeThreadObject *self, DeeTupleObject *args) {
 DeeObject *callback,*callback_args,*elem,*name; Dee_size_t i,argc;
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 argc = DeeTuple_SIZE(args);
 if DEE_UNLIKELY(argc < 1) { DeeError_TypeError_ArgCountExpectedMin(1,argc); return -1; }
 callback = DeeTuple_GET(args,0);
 if (DeeString_Check(callback)) {
  // First argument is the thread name
  if DEE_UNLIKELY(argc == 1) { DeeError_TypeError_ArgCountExpectedMin(2,argc); return -1; }
  name = callback,--argc;
  callback = DeeTuple_GET(args,1);
 } else name = NULL;
 --argc;
 if DEE_UNLIKELY((callback_args = _DeeTuple_NewUnsafe(argc)) == NULL) return -1;
 for (i = 0; i < argc; ++i) {
  Dee_INCREF(elem = DeeTuple_GET(args,i+1));
  DeeTuple_SET(callback_args,i,elem);
 }
 DeeThread_Init(self,callback,callback_args);
 if (!name) { // Try some more stuff to get a function name
  if (DeeFunction_Check(callback)) {
   Dee_XINCREF(name = DeeFunction_NAME(callback));
  } else if (DeeCFunction_Check(callback)) {
   if ((name = DeeString_New(DeeCFunction_NAME(callback))) == NULL) {
    _DeeThread_QUIT_COMMON(self);
    Dee_DECREF(self->t_callback_func);
    Dee_DECREF(self->t_callback_args);
    return -1;
   }
  }
 } else Dee_INCREF(name);
 self->t_name = (DeeStringObject *)name;
 Dee_DECREF(callback_args);
 return 0;
}

void _deethread_tp_clear(DeeThreadObject *self) {
 DeeObject *old_func,*old_args,*old_name;
 DeeAtomicMutex_Acquire(&self->t_lock);
 old_func = (DeeObject *)self->t_callback_func;
 old_args = (DeeObject *)self->t_callback_args;
 old_name = (DeeObject *)self->t_name;
 self->t_callback_func = NULL;
 self->t_callback_args = NULL;
 self->t_name = NULL;
 DeeAtomicMutex_Release(&self->t_lock);
 Dee_XDECREF(old_func);
 Dee_XDECREF(old_args);
 Dee_XDECREF(old_name);
 _DeeThread_ClearRuntime(self,NULL);
}
void _deethread_tp_dtor(DeeThreadObject *self) {
 struct DeeRaisedException *exc,*exc_next;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 DEE_ASSERT(!self->t_handling_exception);
#endif
 _DeeThread_QUIT_COMMON(self);
 if ((exc = self->t_exception) != NULL) {
  while (1) {
   exc_next = exc->re_prev;
   _DeeRaisedException_Quit(exc);
   _DeeRaisedException_Free(exc);
   if (!exc_next) break;
   exc = exc_next;
  }
#ifdef DEE_DEBUG
  self->t_exception = NULL;
#endif
 }
 // NOTE: All of these *should* already be NULL
 Dee_XDECREF(self->t_callback_func);
 Dee_XDECREF(self->t_callback_args);
 Dee_XDECREF(self->t_name);
 Dee_XDECREF(self->t_rt_return);
 Dee_XDECREF(self->t_rt_rand);
 if ((DeeThread_STATE(self)&(DeeThreadState_STARTED|
  DeeThreadState_DETACHED))==DeeThreadState_STARTED) {
#if defined(DEE_PLATFORM_WINDOWS)
  if (!CloseHandle(self->t_handle)) SetLastError(0);
#else
  if (pthread_detach(self->t_handle) != 0) errno = 0;
#endif
 }
}


static void _deevisit_raised_exception(
 struct DeeRaisedException *iter, DeeVisitProc visit, void *closure) {
 Dee_VISIT(iter->re_error);
 Dee_VISIT(iter->re_traceback);
 if (iter->re_prev) _deevisit_raised_exception(iter->re_prev,visit,closure);
}

DEE_VISIT_PROC(_deethread_tp_visit,DeeThreadObject *self) {
 struct DeeStackFrame *stack; DeeObject **tls_iter,**tls_end;
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 Dee_XVISIT(self->t_callback_func);
 Dee_XVISIT(self->t_callback_args);
 Dee_XVISIT(self->t_name);
 DeeAtomicMutex_AcquireRelaxed(&self->t_frame_lock);
 stack = self->t_frame_last;
 while (stack) {
  _DeeStackFrame_Visit(stack,visit,closure);
  stack = stack->f_prev;
 }
 DeeAtomicMutex_Release(&self->t_frame_lock);
 // Visit exceptions
 // - This is a bit complicated because we
 //   must visit them while not holding any locks
 DeeAtomicMutex_AcquireRelaxed(&self->t_exception_lock);
 if (self->t_exception) _deevisit_raised_exception(self->t_exception,visit,closure);
 DeeAtomicMutex_Release(&self->t_exception_lock);
 DeeAtomicMutex_AcquireRelaxed(&self->t_rt_lock);
 Dee_XVISIT(self->t_rt_return);
 tls_end = (tls_iter = self->t_rt_tls_list)+self->t_rt_tls_size;
 while (tls_iter != tls_end) Dee_XVISIT(*tls_iter++);
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 if (self->t_handling_exception)
  _deevisit_raised_exception(self->t_handling_exception,visit,closure);
#endif
 DeeAtomicMutex_Release(&self->t_rt_lock);
}

DeeString_NEW_STATIC(_deethread_unnamed_name,"__unnamed__");
DeeObject *_deethread_tp_str(DeeThreadObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeThread_Check(self));
 if (DeeThread_STARTED(self)) {
  return DeeString_Newf("<thread(%r," DEE_TYPES_IUPRINTF(DEE_TYPES_SIZEOF_THREADID) ")>",self->t_name
                        ? (DeeObject *)self->t_name
                        : (DeeObject *)&_deethread_unnamed_name,
                        DeeThread_ID((DeeObject *)self));
 } else {
  return DeeString_Newf("<thread(%r)>",self->t_name
                        ? (DeeObject *)self->t_name
                        : (DeeObject *)&_deethread_unnamed_name);
 }
}

DeeObject *_deethread_tp_repr(DeeThreadObject *self) {
 DeeObject *result; int first_tag = 1;
 DeeObject *name = (DeeObject *)self->t_name;
 DeeStringWriter writer = DeeStringWriter_INIT();
 DeeThreadState state = (DeeThreadState)DeeAtomic8_Load(self->t_state,memory_order_seq_cst);
 if (!name) name = (DeeObject *)&_deethread_unnamed_name;
 if ((state&DeeThreadState_STARTED)!=0) {
  if DEE_UNLIKELY(DeeStringWriter_Writef(&writer,"<thread(%r," DEE_TYPES_IUPRINTF(
   DEE_TYPES_SIZEOF_THREADID) ") [",name,DeeThread_ID((DeeObject *)self)) != 0) goto err;
 } else {
  if DEE_UNLIKELY(DeeStringWriter_Writef(&writer,"<thread(%r) [",name) != 0) goto err;
 }
 if ((state&DeeThreadState_STARTED)!=0) {
  if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,"STARTED") != 0) goto err;
  first_tag = 0;
 }
#define CHECK_FLAG(flag,str)\
 if ((state&flag)!=0) {\
  if DEE_UNLIKELY(!first_tag)  {\
   if DEE_UNLIKELY(DeeStringWriter_WriteChar(&writer,'|') != 0) goto err;\
  } else first_tag = 0;\
  if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,str) != 0) goto err;\
 }
 CHECK_FLAG(DeeThreadState_INTERRUPTED,"INTERRUPTED")
 CHECK_FLAG(DeeThreadState_INTERRUPT_HANDLED,"INTERRUPTED_HANDLED")
 CHECK_FLAG(DeeThreadState_DETACHED,"DETACHED")
 CHECK_FLAG(DeeThreadState_NOINTERRUPT,"NOINTERRUPT")
 CHECK_FLAG(DeeThreadState_TERMINATED,"TERMINATED")
#undef CHECK_FLAG
 if (DeeStringWriter_WRITE_STRING(&writer,"]>") == -1) goto err;
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}





static DeeObject *_deethread_start(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":start") != 0) return NULL;
 if DEE_UNLIKELY((result = DeeThread_Start(self)) < 0) return NULL;
 DeeReturn_Bool(result == 0);
}
static DeeObject *_deethread_id(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":id") != 0) return NULL;
 return DeeUInt32_New(DeeThread_ID(self));
}
static DeeObject *_deethread_started(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":started") != 0) return NULL;
 DeeReturn_Bool(DeeThread_STARTED(self));
}
static DeeObject *_deethread_detached(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":detached") != 0) return NULL;
 DeeReturn_Bool(DeeThread_DETACHED(self));
}
static DeeObject *_deethread_terminated(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":terminated") != 0) return NULL;
 DeeReturn_Bool(DeeThread_TERMINATED(self));
}
static DeeObject *_deethread_interrupted(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":interrupted") != 0) return NULL;
 DeeReturn_Bool(DeeThread_INTERRUPTED(self));
}
static DeeObject *_deethread_crashed(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":crashed") != 0) return NULL;
 DeeReturn_Bool(DeeThread_Crashed(self));
}
static DeeObject *_deethread_crash_error(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result; int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":last_error") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeThread_GetError(self,&result,NULL)) < 0) return NULL;
 if DEE_UNLIKELY(!temp) {
  DeeError_Throw((DeeObject *)&_DeeError_RuntimeError_NoActiveException);
  return NULL;
 }
 return result;
}
static DeeObject *_deethread_crash_traceback(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result; int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":last_traceback") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeThread_GetError(self,NULL,&result)) < 0) return NULL;
 if DEE_UNLIKELY(!temp) {
  DeeError_Throw((DeeObject *)&_DeeError_RuntimeError_NoActiveException);
  return NULL;
 }
 return result;
}
static DeeObject *_deethread_interrupt(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":interrupt") != 0) return NULL;
 if DEE_UNLIKELY((result = DeeThread_Interrupt(self)) < 0) return NULL;
 DeeReturn_Bool(result == 0);
}
static DeeObject *_deethread_detach(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":detach") != 0) return NULL;
 if DEE_UNLIKELY((result = DeeThread_Detach(self)) < 0) return NULL;
 DeeReturn_Bool(result == 0);
}
static DeeObject *_deethread_join(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *retval;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":join") != 0) return NULL;
 if DEE_UNLIKELY(DeeThread_Join(self,&retval) != 0) return NULL;
 return retval;
}
static DeeObject *_deethread_try_join(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int success; DeeObject *retval,*result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":try_join") != 0) return NULL;
 if DEE_UNLIKELY((success = DeeThread_TryJoin(self,&retval)) < 0) return NULL;
 if (success) retval = Dee_None;
 // We return a 2-elemental tuple: (success,retval)
 result = DeeTuple_Pack(2,success == 0 ? Dee_True : Dee_False,retval);
 if (!success) Dee_DECREF(retval);
 return result;
}
static DeeObject *_deethread_join_timed(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 unsigned int msecs; int success; DeeObject *retval,*result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"u:join_timed",&msecs) != 0) return NULL;
 if DEE_UNLIKELY((success = DeeThread_JoinTimed(self,msecs,&retval)) < 0) return NULL;
 if (success) retval = Dee_None;
 // We return a 2-elemental tuple: (success,retval)
 result = DeeTuple_Pack(2,success == 0 ? Dee_True : Dee_False,retval);
 if DEE_UNLIKELY(!success) Dee_DECREF(retval);
 return result;
}
static DeeObject *_deethread_priority_get(DeeObject *self, void *DEE_UNUSED(closure)) {
 return DeeObject_New(double,DeeThread_GetPriority(self));
}
static int _deethread_priority_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 double pv;
 if DEE_UNLIKELY(DeeObject_Cast(double,v,&pv) != 0) return -1;
 return DeeThread_SetPriority(self,pv);
}
static DeeObject *_deethread_name_get(DeeObject *self, void *DEE_UNUSED(closure)) {
 DeeObject *result = DeeThread_GetName(self);
 if (!result) Dee_INCREF(result = (DeeObject *)&_deethread_unnamed_name);
 return result;
}
static int _deethread_name_set(
 DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 int result;
 if DEE_UNLIKELY((v = DeeString_Cast(v)) == NULL) return -1;
 result = DeeThread_SetName(self,v);
 Dee_DECREF(v);
 return result;
}



// Static thread functions
static DeeObject *_deethreadclass_selfid(
 DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":self_id") != 0) return NULL;
 return DeeUInt32_New(DeeThread_SelfID());
}
static DeeObject *_deethreadclass_self(
 DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":self") != 0) return NULL;
 DeeReturn_XNEWREF(DeeThread_Self());
}
static DeeObject *_deethreadclass_check_interrupt(
 DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":check_interrupt") != 0) return NULL;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deethreadclass_yield(
 DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":yield") != 0) return NULL;
 DeeThread_Yield();
 DeeReturn_None;
}
static DeeObject *_deethreadclass_nointerrupt_begin(
 DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":nointerrupt_begin") != 0) return NULL;
 DeeThread_NoInterruptBegin();
 DeeReturn_None;
}
static DeeObject *_deethreadclass_nointerrupt_end(
 DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":nointerrupt_end") != 0) return NULL;
 DeeThread_NoInterruptEnd();
 DeeReturn_None;
}
static DeeObject *_deethreadclass_sleep(
 DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 unsigned int msecs;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"u:sleep",&msecs) != 0) return NULL;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return NULL;
 DeeThread_SleepNoInterrupt(msecs);
 DeeReturn_None;
}
#ifdef DEE_PLATFORM_WINDOWS
static DeeObject *_deethread_win32_times(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *ct,*et,*kt,*ut,*result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":win32_times") != 0) return NULL;
 if DEE_UNLIKELY(DeeThread_Win32Times(self,&ct,&et,&kt,&ut) != 0) return NULL;
 result = DeeTuple_Pack(4,ct,et,kt,ut);
 Dee_DECREF(ct);
 Dee_DECREF(et);
 Dee_DECREF(kt);
 Dee_DECREF(ut);
 return result;
}
static DeeObject *_deethread_win32_creation_time(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":win32_creation_time") != 0) return NULL;
 if DEE_UNLIKELY(DeeThread_Win32Times(self,&result,NULL,NULL,NULL) != 0) return NULL;
 return result;
}
static DeeObject *_deethread_win32_exit_time(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":win32_exit_time") != 0) return NULL;
 if DEE_UNLIKELY(DeeThread_Win32Times(self,NULL,&result,NULL,NULL) != 0) return NULL;
 return result;
}
static DeeObject *_deethread_win32_kernel_time(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":win32_kernel_time") != 0) return NULL;
 if DEE_UNLIKELY(DeeThread_Win32Times(self,NULL,NULL,&result,NULL) != 0) return NULL;
 return result;
}
static DeeObject *_deethread_win32_user_time(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":win32_user_time") != 0) return NULL;
 if DEE_UNLIKELY(DeeThread_Win32Times(self,NULL,NULL,NULL,&result) != 0) return NULL;
 return result;
}
static DeeObject *_deethread_win32_pending_io(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":win32_pending_io") != 0) return NULL;
 if DEE_UNLIKELY((result = DeeThread_Win32PendingIO(self)) < 0) return NULL;
 DeeReturn_Bool(result);
}
static DeeObject *_deethread_win32_priority_boost_get(
 DeeThreadObject *self, void *DEE_UNUSED(closure)) {
 int result;
 if DEE_UNLIKELY((result = DeeThread_Win32GetPriorityBoost((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(result);
}
static int _deethread_win32_priority_boost_del(
 DeeThreadObject *self, void *DEE_UNUSED(closure)) {
 return DeeThread_Win32SetPriorityBoost((DeeObject *)self,1);
}
static int _deethread_win32_priority_boost_set(
 DeeThreadObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 int mode;
 if DEE_UNLIKELY((mode = DeeObject_Bool(v)) < 0) return -1;
 return DeeThread_Win32SetPriorityBoost((DeeObject *)self,mode);
}
#endif



static struct DeeMemberDef const _deethread_tp_members[] =  {
#ifndef DEE_WITHOUT_THREADS
 DEE_MEMBERDEF_NAMED_RO_v100("state",DeeThreadObject,t_state,atomic(DeeThreadState)),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__t_state",DeeThreadObject,t_state,atomic(DeeThreadState)),
#ifndef DEE_WITHOUT_THREADS
 DEE_MEMBERDEF_NAMED_RO_v100("__t_lock",DeeThreadObject,t_lock,DeeAtomicMutex),
#endif /* !DEE_WITHOUT_THREADS */
#ifdef DEE_PLATFORM_WINDOWS
 DEE_MEMBERDEF_NAMED_RO_v100("__t_handle",DeeThreadObject,t_handle,HANDLE),
 DEE_MEMBERDEF_NAMED_RO_v100("__t_id",DeeThreadObject,t_id,DWORD),
#endif
 DEE_MEMBERDEF_NAMED_RO_v100("__t_callback_func",DeeThreadObject,t_callback_func,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__t_callback_args",DeeThreadObject,t_callback_args,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__t_name",DeeThreadObject,t_name,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__t_priority",DeeThreadObject,t_priority,double),
#endif
#ifndef DEE_WITHOUT_THREADS
 DEE_MEMBERDEF_NAMED_RO_v100("__t_frame_lock",DeeThreadObject,t_frame_lock,DeeAtomicMutex),
#endif /* !DEE_WITHOUT_THREADS */
 DEE_MEMBERDEF_NAMED_RO_v100("__t_frame_size",DeeThreadObject,t_frame_size,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__t_frame_last",DeeThreadObject,t_frame_last,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__t_recursion_str_a",DeeThreadObject,t_recursion_str.rl_a,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__t_recursion_str_c",DeeThreadObject,t_recursion_str.rl_c,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__t_recursion_str_p",DeeThreadObject,t_recursion_str.rl_v,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__t_recursion_repr_a",DeeThreadObject,t_recursion_repr.rl_a,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__t_recursion_repr_c",DeeThreadObject,t_recursion_repr.rl_c,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__t_recursion_repr_p",DeeThreadObject,t_recursion_repr.rl_v,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__t_nointerrupt_rec",DeeThreadObject,t_nointerrupt_rec,uint),
#ifndef DEE_WITHOUT_THREADS
 DEE_MEMBERDEF_NAMED_RO_v100("__t_rt_lock",DeeThreadObject,t_rt_lock,DeeAtomicMutex),
#endif /* !DEE_WITHOUT_THREADS */
 DEE_MEMBERDEF_NAMED_RO_v100("__t_rt_tls_size",DeeThreadObject,t_rt_tls_size,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__t_rt_tls_list",DeeThreadObject,t_rt_tls_list,p2(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__t_rt_return",DeeThreadObject,t_rt_return,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__t_rt_rand",DeeThreadObject,t_rt_rand,object_null),
#ifndef DEE_WITHOUT_THREADS
 DEE_MEMBERDEF_NAMED_RO_v100("__t_exception_lock",DeeThreadObject,t_exception_lock,DeeAtomicMutex),
#endif /* !DEE_WITHOUT_THREADS */
 DEE_MEMBERDEF_NAMED_RO_v100("__t_exception",DeeThreadObject,t_exception,p(void)),
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
#ifndef DEE_WITHOUT_THREADS
 DEE_MEMBERDEF_NAMED_RO_v100("__t_handling_exception_lock",DeeThreadObject,t_handling_exception_lock,DeeAtomicMutex),
#endif /* !DEE_WITHOUT_THREADS */
 DEE_MEMBERDEF_NAMED_RO_v100("__t_handling_exception",DeeThreadObject,t_handling_exception,p(void)),
#endif
 DEE_MEMBERDEF_NAMED_RO_v100("__t_sys_interrupt",DeeThreadObject,t_sys_interrupt,object_null),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const _deethreadclass_tp_members[] =  {
 DEE_MEMBERDEF_CONST_v100("STATE_INITIAL",          DeeThreadState,DeeThreadState_INITIAL),
 DEE_MEMBERDEF_CONST_v100("STATE_STARTED",          DeeThreadState,DeeThreadState_STARTED),
 DEE_MEMBERDEF_CONST_v100("STATE_INTERRUPTED",      DeeThreadState,DeeThreadState_INTERRUPTED),
 DEE_MEMBERDEF_CONST_v100("STATE_INTERRUPT_HANDLED",DeeThreadState,DeeThreadState_INTERRUPT_HANDLED),
 DEE_MEMBERDEF_CONST_v100("STATE_DETACHED",         DeeThreadState,DeeThreadState_DETACHED),
 DEE_MEMBERDEF_CONST_v100("STATE_NOINTERRUPT",      DeeThreadState,DeeThreadState_NOINTERRUPT),
 DEE_MEMBERDEF_CONST_v100("STATE_TERMINATED",       DeeThreadState,DeeThreadState_TERMINATED),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMethodDef const _deethread_tp_methods[] =  {
 DEE_METHODDEF_v100("start",member(&_deethread_start),"() -> bool"),
 DEE_METHODDEF_v100("id",member(&_deethread_id),"() -> uint32_t"),
 DEE_METHODDEF_v100("started",member(&_deethread_started),"() -> bool"),
 DEE_METHODDEF_v100("detached",member(&_deethread_detached),"() -> bool"),
 DEE_METHODDEF_v100("terminated",member(&_deethread_terminated),"() -> bool"),
 DEE_METHODDEF_v100("interrupted",member(&_deethread_interrupted),"() -> bool"),
 DEE_METHODDEF_v100("crashed",member(&_deethread_crashed),"() -> bool\n"
  "@return: #true if the thread has crashed, #false otherwise"),
 DEE_METHODDEF_v100("crash_error",member(&_deethread_crash_error),"() -> object\n"
  "@return: The error that caused the thread to crash"),
 DEE_METHODDEF_v100("crash_traceback",member(&_deethread_crash_traceback),"() -> traceback\n"
  "@return: A traceback pointing to the source of the thread's crash"),
 DEE_METHODDEF_v100("interrupt",member(&_deethread_interrupt),"() -> bool"),
 DEE_METHODDEF_v100("detach",member(&_deethread_detach),"() -> bool"),
 DEE_METHODDEF_v100("join",member(&_deethread_join),"() -> object"),
 DEE_METHODDEF_v100("try_join",member(&_deethread_try_join),"() -> (bool,object)"),
 DEE_METHODDEF_v100("join_timed",member(&_deethread_join_timed),"(unsigned int msec) -> (bool,object)"),
#ifdef DEE_PLATFORM_WINDOWS
 DEE_METHODDEF_v100("win32_times",member(&_deethread_win32_times),"() -> (time,time,time,time)"),
 DEE_METHODDEF_v100("win32_creation_times",member(&_deethread_win32_creation_time),"() -> time"),
 DEE_METHODDEF_v100("win32_exit_time",member(&_deethread_win32_exit_time),"() -> time"),
 DEE_METHODDEF_v100("win32_kernel_time",member(&_deethread_win32_kernel_time),"() -> time"),
 DEE_METHODDEF_v100("win32_user_time",member(&_deethread_win32_user_time),"() -> time"),
 DEE_METHODDEF_v100("win32_pending_io",member(&_deethread_win32_pending_io),"() -> bool"),
#endif
 DEE_METHODDEF_END_v100
};
static struct DeeGetSetDef const _deethread_tp_getsets[] =  {
 DEE_GETSETDEF_v100("priority",member(&_deethread_priority_get),null,member(&_deethread_priority_set),"-> double"),
 DEE_GETSETDEF_v100("name",member(&_deethread_name_get),null,member(&_deethread_name_set),"-> string"),
#ifdef DEE_PLATFORM_WINDOWS
 DEE_GETSETDEF_v100("win32_priority_boost",
  member(&_deethread_win32_priority_boost_get),
  member(&_deethread_win32_priority_boost_del),
  member(&_deethread_win32_priority_boost_set),"-> bool"),
#endif
 DEE_GETSETDEF_END_v100
};


static struct DeeMethodDef const _deethread_tp_class_methods[] =  {
 DEE_METHODDEF_v100("self",member(&_deethreadclass_self),"() -> thread\n"
  "@return: A thread object representive of the calling thread."),
 DEE_METHODDEF_v100("selfid",member(&_deethreadclass_selfid),"() -> uint32_t\n"
  "@return: The id of the calling thread.\n"
  "\tSame as calling #(thread.self().id()), but may be faster."),
 DEE_METHODDEF_v100("check_interrupt",member(&_deethreadclass_check_interrupt),"() -> none\n"
  "@throws Signal.Interrupt: The calling thread was interrupted\n"
  "\tManually checks if an interrupt signal was sent to the calling thread.\n"
  "\tThis function is called automatically when a blocking system call like #(file.io.stdout.write(...)) is called."),
 DEE_METHODDEF_v100("yield",member(&_deethreadclass_yield),"() -> none\n"
  "\tYields execution in the calling thread, and hints the scheduler to continue in another thread."),
 DEE_METHODDEF_v100("nointerrupt_begin",member(&_deethreadclass_nointerrupt_begin),"() -> none\n"
  "\tRecursively begins a section of code during which interrupt signals are not checked.\n"
  "\tSuch blocks are automatically activated when executing destructors or finally blocks, where exceptions and less than welcome.\n"),
 DEE_METHODDEF_v100("nointerrupt_end",member(&_deethreadclass_nointerrupt_end),"() -> none\n"
  "\tRecursively ends a nointerrupt block."),
 DEE_METHODDEF_v100("sleep",member(&_deethreadclass_sleep),"(unsigned int msec) -> none\n"
  "@throws Signal.Interrupt: The calling thread was interrupted\n"
  "\tSuspends execution for @msecs milliseconds, not using any system resources and allowing other threads to run."),
 DEE_METHODDEF_END_v100
};


DeeTypeObject DeeThread_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("thread"),member(
  "(callable callback, object args...) -> thread\n"
  "(string name, callable callback, object args...) -> thread\n"
  "\tCreates a new thread, that when run will execute @callback with the given arguments"),
  // v 'DEE_TYPE_FLAG_ALWAYS_CLEAR' is required to ensure thread-locals and what
  //   not being cleared before the thread is being destroyed.
  //   >> If we wouldn't do this, user-code might still be able to execute
  //      through calls to decref from the real thread destructor,
  //      which (if it's the current thread that's being destroyed)
  //      could try (and succeed) in reviving the thread.
  member(DEE_TYPE_FLAG_ALWAYS_CLEAR),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeThreadObject),null,null,null,null,
  member(&_deethread_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deethread_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deethread_tp_str),
  member(&_deethread_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deethread_tp_visit),
  member(&_deethread_tp_clear)),
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
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deethread_tp_members),member(_deethread_tp_getsets),member(_deethread_tp_methods),
  member(_deethreadclass_tp_members),null,member(_deethread_tp_class_methods)),
 DEE_TYPE_OBJECT_FOOTER_v100
};
#else /* !DEE_WITHOUT_THREADS */

DeeThreadObject _DeeThread_Self = {
 0,    // t_nointerrupt_rec
#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
 0,    // t_frame_size
#endif /* DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS */
 NULL, // t_frame_last
 NULL, // t_exception
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 NULL, // t_handling_exception
#endif
 DeeRecursionList_INIT(), // t_recursion_str
 DeeRecursionList_INIT(), // t_recursion_repr
 NULL, // ob_rand
};

void DeeThread_Finalize(void) {
 struct DeeRaisedException *exc,*exc_next;
#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
 DEE_ASSERT(!_DeeThread_Self.t_frame_size && "Thread is still executing code!");
#endif /* DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS */
 DEE_ASSERT(!_DeeThread_Self.t_frame_last && "Thread is still executing code!");
 _DeeRecursionList_Clear(&_DeeThread_Self.t_recursion_str);
 _DeeRecursionList_Clear(&_DeeThread_Self.t_recursion_repr);
 Dee_XCLEAR(_DeeThread_Self.ob_rand);

#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 // Free currently handled exceptions (should always be empty...)
 if ((exc = _DeeThread_Self.t_handling_exception) != NULL) {
  while (exc) {
   exc_next = exc->re_prev;
   _DeeRaisedException_Quit(exc);
   _DeeRaisedException_Free(exc);
   exc = exc_next;
  }
  _DeeThread_Self.t_handling_exception = NULL;
 }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
}


#endif /* !DEE_WITHOUT_THREADS */


#ifndef DEE_WITHOUT_THREADS
#undef DeeThread_Yield
void DeeThread_Yield(void) {
#ifdef DEE_PLATFORM_WINDOWS
 SwitchToThread();
#elif DEE_HAVE_SCHED_YIELD
 sched_yield();
#else
 pthread_yield();
#endif
}
#endif

DEE_A_RET_EXCEPT(-1) int DeeThread_Sleep(DEE_A_IN unsigned int msecs) {
#ifdef DEE_PLATFORM_WINDOWS
 if (DeeThread_CheckInterrupt() != 0) return -1;
 Sleep(msecs);
#elif DEE_HAVE_NANOSLEEP
 struct timespec wait_tm,rem;
 if (DeeThread_CheckInterrupt() != 0) return -1;
 wait_tm.tv_sec = (time_t)(msecs/1000);
 wait_tm.tv_nsec = (unsigned long)(msecs%1000)*1000000ul;
 while (nanosleep(&wait_tm,&rem) != 0) {
  errno = 0;
  if (DeeThread_CheckInterrupt() != 0) return -1;
  wait_tm = rem;
 }
#elif DEE_HAVE_CLOCK_NANOSLEEP
 struct timespec wait_tm,rem;
 if (DeeThread_CheckInterrupt() != 0) return -1;
 wait_tm.tv_sec = (time_t)(msecs/1000);
 wait_tm.tv_nsec = (unsigned long)(msecs%1000)*1000000ul;
 while (clock_nanosleep(CLOCK_MONOTONIC,0,&wait_tm,&rem) != 0) {
  errno = 0;
  if (DeeThread_CheckInterrupt() != 0) return -1;
  wait_tm = rem;
 }
#elif DEE_HAVE_USLEEP
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (usleep((useconds_t)(msecs*1000ul)) != 0) errno = 0;
#elif DEE_HAVE_SELECT
 struct timeval wait_tm;
 if (DeeThread_CheckInterrupt() != 0) return -1;
 wait_tm.tv_sec = msecs/1000ul;
 wait_tm.tv_usec = (msecs%1000ul)*1000ul;
 if (select(0,0,0,0,&tv) != 0) errno = 0;
#else
 Dee_uint64_t end; if (!msecs) return;
 if (DeeThread_CheckInterrupt() != 0) return -1;
 end = DeeTime_GetClock1000()+msecs; // *cringes*
 while (end < DeeTime_GetClock1000()) {
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
  DeeThread_Yield();
 }
#endif
 return 0;
}
#undef DeeThread_SleepNoInterrupt
void DeeThread_SleepNoInterrupt(DEE_A_IN unsigned int msecs) {
#ifdef DEE_PLATFORM_WINDOWS
 Sleep(msecs);
#elif DEE_HAVE_NANOSLEEP
 struct timespec wait_tm,rem;
 wait_tm.tv_sec = (time_t)(msecs/1000);
 wait_tm.tv_nsec = (unsigned long)(msecs%1000)*1000000ul;
 while (nanosleep(&wait_tm,&rem) != 0) errno = 0,wait_tm = rem;
#elif DEE_HAVE_CLOCK_NANOSLEEP
 struct timespec wait_tm,rem;
 wait_tm.tv_sec = (time_t)(msecs/1000);
 wait_tm.tv_nsec = (unsigned long)(msecs%1000)*1000000ul;
 while (clock_nanosleep(CLOCK_MONOTONIC,0,&wait_tm,&rem) != 0) errno = 0,wait_tm = rem;
#elif DEE_HAVE_USLEEP
 if (usleep((useconds_t)(msecs*1000ul)) != 0) errno = 0;
#elif DEE_HAVE_SELECT
 struct timeval wait_tm;
 wait_tm.tv_sec = msecs/1000ul;
 wait_tm.tv_usec = (msecs%1000ul)*1000ul;
 if (select(0,0,0,0,&tv) != 0) errno = 0;
#else
 Dee_uint64_t end; if (!msecs) return;
 end = DeeTime_GetClock1000()+msecs; // *cringes*
 while (end < DeeTime_GetClock1000()) DeeThread_Yield();
#endif
}


DEE_DECL_END

#endif /* !GUARD_DEEMON_THREAD_C */
