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

//////////////////////////////////////////////////////////////////////////
// === Win32 ===
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

struct DeeWin32SysThread {
 HANDLE w32_handle;
 DWORD  w32_tid;
};
#define DeeWin32SysThread_return_t   DWORD
#define DeeWin32SysThread_priority_t int

#define DeeWin32SysThread_THREADMAIN(name,closure)  DWORD WINAPI name(closure)
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

#define DeeWin32SysThread_InitSuspended(self,callback,closure,...) \
 DeeWin32Sys_HThreadInitSuspended(&(self)->w32_handle,&(self)->w32_tid,callback,closure,__VA_ARGS__)
#define DeeWin32SysThread_Init(self,callback,closure,...) \
 DeeWin32Sys_HThreadInit(&(self)->w32_handle,&(self)->w32_tid,callback,closure,__VA_ARGS__)

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
 if ((pretval) && DEE_UNLIKELY(!GetExitCodeThread(hThread,pretval))) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "GetExitCodeThread(%p,...) : %K",hThread,\
                      DeeSystemError_Win32ToString(GetLastError()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeWin32Sys_HThreadDetach(hThread,...) do{}while(0)
#define DeeWin32Sys_HThreadDetachNoexcept(hThread,...) do{}while(0)
#define DeeWin32Sys_HThreadQuit(hThread) do{ CloseHandle(hThread); }while(0)

#define DeeWin32SysThread_Join(self,pretval,...)   DeeWin32Sys_HThreadJoin((self)->w32_handle,pretval,__VA_ARGS__)
#define DeeWin32SysThread_Detach(self,...)         DeeWin32Sys_HThreadDetach((self)->w32_handle,__VA_ARGS__)
#define DeeWin32SysThread_DetachNoexcept(self,...) DeeWin32Sys_HThreadDetachNoexcept((self)->w32_handle,__VA_ARGS__)
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

#define DeeWin32SysThread_GetPriority(self,result,...)




#define DeeSysThread_THREADMAIN     DeeWin32SysThread_THREADMAIN
#define DeeSysThread_return_t       DeeWin32SysThread_return_t
#define DeeSysThread_Init           DeeWin32SysThread_Init
#define DeeSysThread_InitSuspended  DeeWin32SysThread_InitSuspended
#define DeeSysThread_Suspend        DeeWin32SysThread_Suspend
#define DeeSysThread_Resume         DeeWin32SysThread_Resume
#define DEE_SYSTHREAD_SUSPENDRESUME_RECURSIVE
#define DeeSysThread_Join           DeeWin32SysThread_Join
#define DeeSysThread_Detach         DeeWin32SysThread_Detach
#define DeeSysThread_DetachNoexcept DeeWin32SysThread_DetachNoexcept
#define DeeSysThread_Quit           DeeWin32SysThread_Quit
#define DeeSysThread_GetPriority    DeeWin32SysThread_GetPriority




DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS_WIN32_SYSTHREAD_H */
