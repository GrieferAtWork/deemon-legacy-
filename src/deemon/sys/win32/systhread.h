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
#ifndef GUARD_DEEMON_SYS_WIN32_SYSTHREAD_H
#define GUARD_DEEMON_SYS_WIN32_SYSTHREAD_H 1

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/runtime/extern.h>
#include <deemon/optional/atomic_once.h>
#include <deemon/optional/string_forward.h>

//////////////////////////////////////////////////////////////////////////
// === Win32 ===
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// While getting kind-of complicated on XP, we don't
// necessarily need to store the TID of a thread.
// >> Though since I currently don't have a machine running XP,
//    or better yet: something even older than that,
//    I decided to leave this in, as it might allow for
//    backwards compatibility with stuff that's even older...
#ifndef DEE_WIN32SYSTHREAD_STORE_TID
#define DEE_WIN32SYSTHREAD_STORE_TID 0
#endif /* !DEE_WIN32SYSTHREAD_STORE_TID */

struct DeeWin32SysThread {
 HANDLE w32_handle;
#if DEE_WIN32SYSTHREAD_STORE_TID
 DWORD  w32_tid;
#endif /* DEE_WIN32SYSTHREAD_STORE_TID */
};
#define DeeWin32SysThread_return_t    DWORD
#define DeeWin32SysThread_RETURN(val) return (DWORD)(val)

#define DeeWin32SysThread_THREADMAIN(name)  DWORD WINAPI name(LPVOID _w32closure)
#define DeeWin32SysThread_CLOSURE(T)        ((T *)_w32closure)
#define _DeeWin32Sys_HThreadInitWithKnownFlags(phThread,phTid,callback,closure,flags,...) \
do{\
 if DEE_UNLIKELY((*(phThread) = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)\
  (callback),closure,flags,phTid)) == NULL) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "CreateThread(NULL,0,%p,%p," #flags ",...) : %K",\
                      (LPTHREAD_START_ROUTINE)(callback),closure,\
                      DeeSystemError_Win32ToString(GetLastError()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeWin32Sys_HThreadInitSuspended(phThread,phTid,callback,closure,...) \
 _DeeWin32Sys_HThreadInitWithKnownFlags(phThread,phTid,callback,closure,CREATE_SUSPENDED,__VA_ARGS__)
#define DeeWin32Sys_HThreadInit(phThread,phTid,callback,closure,...) \
 _DeeWin32Sys_HThreadInitWithKnownFlags(phThread,phTid,callback,closure,0,__VA_ARGS__)

#if DEE_WIN32SYSTHREAD_STORE_TID
#define DeeWin32SysThread_InitSuspended(self,callback,closure,...) \
 DeeWin32Sys_HThreadInitSuspended(&(self)->w32_handle,&(self)->w32_tid,callback,closure,__VA_ARGS__)
#define DeeWin32SysThread_Init(self,callback,closure,...) \
 DeeWin32Sys_HThreadInit(&(self)->w32_handle,&(self)->w32_tid,callback,closure,__VA_ARGS__)
#else /* DEE_WIN32SYSTHREAD_STORE_TID */
#define DeeWin32SysThread_InitSuspended(self,callback,closure,...) \
 DeeWin32Sys_HThreadInitSuspended(&(self)->w32_handle,NULL,callback,closure,__VA_ARGS__)
#define DeeWin32SysThread_Init(self,callback,closure,...) \
 DeeWin32Sys_HThreadInit(&(self)->w32_handle,NULL,callback,closure,__VA_ARGS__)
#endif /* !DEE_WIN32SYSTHREAD_STORE_TID */

#define DeeWin32Sys_HThreadSuspend(hThread,...) \
do{\
 if DEE_UNLIKELY(SuspendThread(hThread) == (DWORD)-1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "SuspendThread(%p) : %K",hThread,\
                      DeeSystemError_Win32ToString(GetLastError()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeWin32Sys_HThreadResume(hThread,...) \
do{\
 if DEE_UNLIKELY(ResumeThread(hThread) == (DWORD)-1) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "ResumeThread(%p) : %K",hThread,\
                      DeeSystemError_Win32ToString(GetLastError()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DEE_WIN32SYSTHREAD_SUSPENDRESUME_RECURSIVE
#define DeeWin32SysThread_Suspend(self,...) DeeWin32Sys_HThreadSuspend((self)->w32_handle,__VA_ARGS__)
#define DeeWin32SysThread_Resume(self,...)  DeeWin32Sys_HThreadResume((self)->w32_handle,__VA_ARGS__)

#define DeeWin32Sys_HThreadJoin(hThread,pretval,...) \
do{\
 switch (WaitForSingleObject(hThread,INFINITE)) {\
  case WAIT_FAILED:\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "WaitForSingleObject(%p,INFINITE) : %K",hThread,\
                       DeeSystemError_Win32ToString(GetLastError()));\
   {__VA_ARGS__;}\
  default: break;\
 }\
 if (((pretval)!=NULL) && DEE_UNLIKELY(!GetExitCodeThread(hThread,pretval))) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "GetExitCodeThread(%p,...) : %K",hThread,\
                      DeeSystemError_Win32ToString(GetLastError()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeWin32Sys_HThreadDetach(hThread,...) do{}while(0)
#define DeeWin32Sys_HThreadDetachNoexcept(hThread) do{}while(0)
#define DeeWin32Sys_HThreadQuit(hThread) do{ CloseHandle(hThread); }while(0)

#define DeeWin32SysThread_Join(self,pretval,...)   DeeWin32Sys_HThreadJoin((self)->w32_handle,pretval,__VA_ARGS__)
#define DeeWin32SysThread_Detach(self,...)         DeeWin32Sys_HThreadDetach((self)->w32_handle,__VA_ARGS__)
#define DeeWin32SysThread_DetachNoexcept(self)     DeeWin32Sys_HThreadDetachNoexcept((self)->w32_handle)
#define DeeWin32SysThread_Quit(self)               DeeWin32Sys_HThreadQuit((self)->w32_handle)

#define DeeWin32Sys_HThreadTerminate(hThread,retval,...) \
do{\
 if DEE_UNLIKELY(!TerminateThread(hThread,retval)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "TerminateThread(%p,%lu) : %K",hThread,(DWORD)(retval),\
                      DeeSystemError_Win32ToString(GetLastError()));\
  {__VA_ARGS__;}\
 }\
}while(0)

#define DeeWin32Sys_HThreadGetPriority(hThread,result,...) \
do{\
 if DEE_UNLIKELY((*(result) = GetThreadPriority(hThread)) == THREAD_PRIORITY_ERROR_RETURN) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "GetThreadPriority(%p) : %K",hThread,\
                      DeeSystemError_Win32ToString(GetLastError()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeWin32Sys_HThreadSetPriority(hThread,value,...) \
do{\
 if DEE_UNLIKELY(!SetThreadPriority(hThread,value)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "SetThreadPriority(%p,%d) : %K",hThread,value,\
                      DeeSystemError_Win32ToString(GetLastError()));\
  {__VA_ARGS__;}\
 }\
}while(0)

#define DeeWin32SysThread_priority_t    int
#define DeeWin32SysThread_PRIORITY_MIN  THREAD_PRIORITY_LOWEST
#define DeeWin32SysThread_PRIORITY_DEF  THREAD_PRIORITY_NORMAL
#define DeeWin32SysThread_PRIORITY_MAX  THREAD_PRIORITY_HIGHEST
#define DeeWin32SysThread_GetPriority(self,result,...) \
 DeeWin32Sys_HThreadGetPriority((self)->w32_handle,result,__VA_ARGS__)
#define DeeWin32SysThread_SetPriority(self,value,...) \
 DeeWin32Sys_HThreadSetPriority((self)->w32_handle,value,__VA_ARGS__)


#define DeeWin32Sys_HThreadGetSelf(phThread) \
do{ *(phThread) = GetCurrentThread(); }while(0)
#define DeeWin32Sys_HThreadIsSelf(hThread,result) \
do{\
 DWORD _hthread_tid;\
 DeeWin32Sys_HThreadGetTID(hThread,&_hthread_tid);\
 *(result) = GetCurrentThreadId() == _hthread_tid;\
}while(0)
#define DeeWin32Sys_HThreadEquals(hThreadA,hThreadB,result) \
do{\
 DWORD _hthreada_tid,_hthreadb_tid;\
 DeeWin32Sys_HThreadGetTID(hThreadA,&_hthreada_tid);\
 DeeWin32Sys_HThreadGetTID(hThreadB,&_hthreadb_tid);\
 *(result) = _hthreada_tid == _hthreadb_tid;\
}while(0)
#define DeeWin32SysThread_GetSelf(tdescr)       DeeWin32Sys_HThreadGetSelf(&(tdescr)->w32_handle)
#define DeeWin32SysThread_IsSelf(tdescr,result) DeeWin32Sys_HThreadIsSelf((tdescr)->w32_handle,result)
#define DeeWin32SysThread_Equals(a,b,result)    DeeWin32Sys_HThreadEquals((a)->w32_handle,(b)->w32_handle,result)


#if DEE_WIN32SYSTHREAD_STORE_TID
#define DeeWin32SysThread_GetTID(self,result) do{ *(result) = (self)->w32_tid; }while(0)
#else /* DEE_WIN32SYSTHREAD_STORE_TID */
typedef DWORD (WINAPI *LPGETTHREADID)(HANDLE hThread);
static LPGETTHREADID DeeWin32Sys_PGetThreadId = NULL;
static struct DeeAtomicOnceFlag DeeWin32Sys_PGetThreadId_loaded = DeeAtomicOnceFlag_INIT();
#ifndef DeeWin32Sys_sKERNEL32
#define DeeWin32Sys_sKERNEL32 DeeWin32Sys_sKERNEL32
static Dee_WideChar const DeeWin32Sys_sKERNEL32[] = {'K','E','R','N','E','L','3','2',0};
#endif

typedef struct _DEE_WIN32_CLIENT_ID {
 PVOID UniqueProcess;
 PVOID UniqueThread;
} DEE_WIN32_CLIENT_ID,*PDEE_WIN32_CLIENT_ID;
typedef struct _DEE_WIN32_THREAD_BASIC_INFORMATION {
 NTSTATUS            ExitStatus;
 PVOID               TebBaseAddress;
 DEE_WIN32_CLIENT_ID ClientId;
 KAFFINITY           AffinityMask;
 LONG /*KPRIORITY*/  Priority;
 LONG /*KPRIORITY*/  BasePriority;
} DEE_WIN32_THREAD_BASIC_INFORMATION,*DEE_WIN32_PTHREAD_BASIC_INFORMATION;
typedef /*NTSTATUS*/ LONG (NTAPI *LPNTQUERYINFORMATIONTHREAD)(
 HANDLE ThreadHandle, /*THREADINFOCLASS*/ int ThreadInformationClass,
 PVOID ThreadInformation,ULONG ThreadInformationLength,PULONG ReturnLength);
static LPNTQUERYINFORMATIONTHREAD pNtQueryInformationThread = NULL;


static DWORD WINAPI DeeWin32Sys_FallbackGetThreadId(HANDLE hThread) {
 // NOTE: While meant for windows xp, this function also works on windows 8.1 (x64)
 DEE_WIN32_THREAD_BASIC_INFORMATION thread_info;
 DEE_ATOMIC_ONCE({
  *(void **)&pNtQueryInformationThread =
  DeeSharedLib_Utf8PoolTryImport("ntdll.dll","NtQueryInformationThread");
 });
 if (!pNtQueryInformationThread) {
fallback:
  return (DWORD)(Dee_uintptr_t)hThread; // damn...
 }
 // NOTE: '!NT_SUCCESS' --> '< 0'
 if ((*pNtQueryInformationThread)(hThread,/*ThreadBasicInformation*/0,
  &thread_info,sizeof(thread_info),NULL) < 0) goto fallback;
 return (DWORD)(Dee_uintptr_t)thread_info.ClientId.UniqueThread;
}

#define DeeWin32Sys_PGetThreadId_load() \
 DeeAtomicOnceFlag_RUN(&DeeWin32Sys_PGetThreadId_loaded,{\
  *(FARPROC *)&DeeWin32Sys_PGetThreadId = GetProcAddress(\
   GetModuleHandleW(DeeWin32Sys_sKERNEL32),"GetThreadId");\
  if (!DeeWin32Sys_PGetThreadId) DeeWin32Sys_PGetThreadId = &DeeWin32Sys_FallbackGetThreadId;\
 })

#define DeeWin32Sys_HThreadGetTID(hThread,result) \
do{\
 DeeWin32Sys_PGetThreadId_load();\
 *(result) = (*DeeWin32Sys_PGetThreadId)(hThread);\
}while(0)
#define DeeWin32SysThread_GetTID(self,result) DeeWin32Sys_HThreadGetTID((self)->w32_handle,result)
#endif /* !DEE_WIN32SYSTHREAD_STORE_TID */

#define DeeWin32SysThread_GetSelfTID(result)  do{*(result) = GetCurrentThreadId();}while(0)


DEE_COMPILER_PACK_PUSH(8)
typedef struct THREADNAME_INFO {
 DWORD  dwType;     // Must be 0x1000.
 LPCSTR szName;     // Pointer to name (in user addr space).
 DWORD  dwThreadID; // Thread ID (-1=caller thread).
 DWORD  dwFlags;    // Reserved for future use, must be zero.
} THREADNAME_INFO;
DEE_COMPILER_PACK_POP

#if DEE_COMPILER_HAVE_MSVC_SEH
#define DeeWin32Sys_TIDSetName(tid,name) \
do{\
 THREADNAME_INFO _tname_info;\
 _tname_info.dwType = 0x1000;\
 _tname_info.szName = (name);\
 _tname_info.dwThreadID = (tid);\
 _tname_info.dwFlags = 0;\
 __try {\
  RaiseException(0x406D1388,0,sizeof(_tname_info)/sizeof(void *),(ULONG_PTR const *)&_tname_info);\
 } DEE_COMPILER_PREFAST_WARNING_PUSH(6320 6322) __except(1) {}\
   DEE_COMPILER_PREFAST_WARNING_POP\
}while(0)
#elif defined(__GNUC__)
typedef EXCEPTION_DISPOSITION (WINAPI *LPDEE_IGNORE_EXCEPTION_HANDLER)(
 LPEXCEPTION_RECORD pRecord, struct _DEE_SEH_EXCEPTION_REGISTRATION *pReg,
 LPCONTEXT pContext, LPEXCEPTION_RECORD pRecord2);
typedef struct _DEE_IGNORESEH_EXCEPTION_REGISTRATION {
 struct _DEE_IGNORESEH_EXCEPTION_REGISTRATION *prev;
 LPDEE_IGNORE_EXCEPTION_HANDLER                handler;
} _DEE_IGNORESEH_EXCEPTION_REGISTRATION;
static EXCEPTION_DISPOSITION WINAPI _DeeWin32Sys_IgnoreExceptionRouter(
 LPEXCEPTION_RECORD DEE_UNUSED(pRecord), void *DEE_UNUSED(pReg),
 LPCONTEXT DEE_UNUSED(pContext), LPEXCEPTION_RECORD DEE_UNUSED(pRecord2)) {
 return ExceptionContinueExecution;
}
#define DeeWin32Sys_TIDSetName(tid,name) \
do{\
 THREADNAME_INFO _tname_info;\
 _DEE_IGNORESEH_EXCEPTION_REGISTRATION _exceptreg;\
 _tname_info.dwType = 0x1000;\
 _tname_info.szName = (name);\
 _tname_info.dwThreadID = (tid);\
 _tname_info.dwFlags = 0;\
 _exceptreg.handler=&_DeeWin32Sys_IgnoreExceptionRouter;\
 __asm__ __volatile__("movl %%fs:0, %0" : "=r" (_exceptreg.prev));\
 __asm__ __volatile__("movl %0, %%fs:0" : : "r" (&_exceptreg));\
 RaiseException(0x406D1388,0,sizeof(_tname_info)/sizeof(void *),(ULONG_PTR const *)&_tname_info);\
 __asm__ __volatile__("movl %0, %%fs:0" : : "r" (_exceptreg.prev));\
}while(0)
#endif

#ifdef DeeWin32Sys_TIDSetName
#define DeeWin32Sys_TSelfSetName(name) \
 DeeWin32Sys_TIDSetName(GetCurrentThreadId(),name)
#endif
#ifdef DeeWin32Sys_TSelfSetName
#define DeeWin32SysThread_SetSelfName DeeWin32Sys_TSelfSetName
#endif


#define DeeSysThread                DeeWin32SysThread
#define DeeSysThread_THREADMAIN     DeeWin32SysThread_THREADMAIN
#define DeeSysThread_CLOSURE        DeeWin32SysThread_CLOSURE
#define DeeSysThread_RETURN         DeeWin32SysThread_RETURN
#define DeeSysThread_return_t       DeeWin32SysThread_return_t
#define DeeSysThread_Init           DeeWin32SysThread_Init
#define DeeSysThread_InitSuspended  DeeWin32SysThread_InitSuspended
#define DeeSysThread_Suspend        DeeWin32SysThread_Suspend
#define DeeSysThread_Resume         DeeWin32SysThread_Resume
#define DeeSysThread_GetSelf        DeeWin32SysThread_GetSelf
#define DeeSysThread_Equals         DeeWin32SysThread_Equals
#define DeeSysThread_IsSelf         DeeWin32SysThread_IsSelf
#ifdef DeeWin32SysThread_SetSelfName
#define DeeSysThread_SetSelfName    DeeWin32SysThread_SetSelfName
#endif
#define DeeSysThread_GetTID         DeeWin32SysThread_GetTID
#define DeeSysThread_GetSelfTID     DeeWin32SysThread_GetSelfTID
#define DEE_SYSTHREAD_SUSPENDRESUME_RECURSIVE
#define DeeSysThread_Join           DeeWin32SysThread_Join
#define DeeSysThread_Detach         DeeWin32SysThread_Detach
#define DeeSysThread_DetachNoexcept DeeWin32SysThread_DetachNoexcept
#define DeeSysThread_Quit           DeeWin32SysThread_Quit
#define DeeSysThread_priority_t     DeeWin32SysThread_priority_t
#define DeeSysThread_PRIORITY_MIN   DeeWin32SysThread_PRIORITY_MIN
#define DeeSysThread_PRIORITY_DEF   DeeWin32SysThread_PRIORITY_DEF
#define DeeSysThread_PRIORITY_MAX   DeeWin32SysThread_PRIORITY_MAX
#define DeeSysThread_GetPriority    DeeWin32SysThread_GetPriority
#define DeeSysThread_SetPriority    DeeWin32SysThread_SetPriority


DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS_WIN32_SYSTHREAD_H */
