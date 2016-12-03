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
#ifndef GUARD_DEEMON_SYS_PTHREAD_SYSTHREAD_H
#define GUARD_DEEMON_SYS_PTHREAD_SYSTHREAD_H 1

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/posix_features.inl>

//////////////////////////////////////////////////////////////////////////
// === PThread ===
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#include <errno.h>
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_PTHREAD_H
#include <pthread.h>
#elif defined(__INTELLISENSE__)
#include "highlight_pthread.h"
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_TIME_H
#include <time.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SCHED_H
#include <sched.h>
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
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN


struct DeePThreadSysThread { pthread_t pt_thread; };
typedef void *DeePThreadSysThread_return_t;
#define DeePThreadSysThread_return_t DeePThreadSysThread_return_t

#define DeePThreadSysThread_THREADMAIN(name)  void *name(void *_ptclosure)
#define DeePThreadSysThread_CLOSURE(T)        ((T *)_ptclosure)
#define DeePThreadSysThread_RETURN(val)       return (void *)(Dee_uintptr_t)(val)
#define DeePThreadSys_PThreadCreate(ppthread,callback,closure,...) \
do{\
 int _pt_error = pthread_create(ppthread,NULL,callback,(void *)(closure));\
 if DEE_UNLIKELY(_pt_error != 0) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "pthread_create(...,NULL,%p,%p) : %K",\
                      (void *)(callback),(void *)(closure),\
                      DeeSystemError_ToString(_pt_error));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeePThreadSys_PThreadJoin(pthread,pretval,...) \
do{\
 int _pt_error = pthread_join(pthread,pretval);\
 if DEE_UNLIKELY(_pt_error != 0) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "pthread_join(...) : %K",\
                      DeeSystemError_ToString(_pt_error));\
  {__VA_ARGS__;}\
 }\
}while(0)

#define DeePThreadSys_PThreadDetach(pthread,...) \
do{\
 int _pt_error = pthread_detach(pthread);\
 if DEE_UNLIKELY(_pt_error != 0) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "pthread_detach(...) : %K",\
                      DeeSystemError_ToString(_pt_error));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeePThreadSys_PThreadDetachNoexcept(pthread) \
do{ pthread_detach(pthread); }while(0)

#define DeePThreadSysThread_Init(self,callback,closure,...) DeePThreadSys_PThreadCreate(&(self)->pt_thread,callback,closure,__VA_ARGS__)
#define DeePThreadSysThread_Join(self,pretval,...)          DeePThreadSys_PThreadJoin((self)->pt_thread,pretval,__VA_ARGS__)
#define DeePThreadSysThread_Detach(self,...)                DeePThreadSys_PThreadDetach((self)->pt_thread,__VA_ARGS__)
#define DeePThreadSysThread_DetachNoexcept(self)            DeePThreadSys_PThreadDetachNoexcept((self)->pt_thread)


#if 0
#define DeePThreadSys_PThreadGetPriority(pthread,result,...) do{}while(0)
#define DeePThreadSys_PThreadSetPriority(pthread,value,...)  do{}while(0)
#define DeePThreadSysThread_priority_t                       int
#define DeePThreadSysThread_PRIORITY_MIN                     ?
#define DeePThreadSysThread_PRIORITY_DEF                     ?
#define DeePThreadSysThread_PRIORITY_MAX                     ?
#endif

#ifdef DeePThreadSys_PThreadGetPriority
#define DeePThreadSysThread_GetPriority(self,result,...) DeePThreadSys_PThreadGetPriority((self)->pt_thread,result,__VA_ARGS__)
#endif
#ifdef DeePThreadSys_PThreadSetPriority
#define DeePThreadSysThread_SetPriority(self,value,...)  DeePThreadSys_PThreadSetPriority((self)->pt_thread,value,__VA_ARGS__)
#endif


#define DeePthreadSys_PThreadGetSelf(ppthread) do{ *(ppthread) = pthread_self(); }while(0)
#define DeePThreadSys_PThreadEquals(pthreadA,pthreadB,result) do{*(result) = pthread_equal(pthreadA,pthreadB);}while(0)
#define DeePThreadSysThread_GetSelf(tdescr)    DeePthreadSys_PThreadGetSelf(&(tdescr)->pt_thread)
#define DeePThreadSysThread_Equals(a,b,result) DeePThreadSys_PThreadEquals((a)->pt_thread,(b)->pt_thread,result)

#if DEE_HAVE_PTHREAD_SETNAME_NP
#define DeePThreadSys_PThreadSetSelfName(name) \
do{ pthread_setname_np(pthread_self(),name); }while(0)
#endif

#ifdef DeePThreadSys_PThreadSetSelfName
#define DeePThreadSysThread_SetSelfName DeePThreadSys_PThreadSetSelfName
#endif


#define DeeSysThread                DeePThreadSysThread
#define DeeSysThread_THREADMAIN     DeePThreadSysThread_THREADMAIN
#define DeeSysThread_CLOSURE        DeePThreadSysThread_CLOSURE
#define DeeSysThread_RETURN         DeePThreadSysThread_RETURN
#define DeeSysThread_return_t       DeePThreadSysThread_return_t
#define DeeSysThread_Init           DeePThreadSysThread_Init
#define DeeSysThread_GetSelf        DeePThreadSysThread_GetSelf
#define DeeSysThread_Equals         DeePThreadSysThread_Equals
#ifdef DeePThreadSysThread_SetSelfName
#define DeeSysThread_SetSelfName    DeePThreadSysThread_SetSelfName
#endif
#define DeeSysThread_Join           DeePThreadSysThread_Join
#define DeeSysThread_Detach         DeePThreadSysThread_Detach
#define DeeSysThread_DetachNoexcept DeePThreadSysThread_DetachNoexcept

#ifdef DeePThreadSysThread_priority_t
#define DeeSysThread_priority_t     DeePThreadSysThread_priority_t
#ifdef DeePThreadSysThread_PRIORITY_MIN
#define DeeSysThread_PRIORITY_MIN   DeePThreadSysThread_PRIORITY_MIN
#endif
#ifdef DeePThreadSysThread_PRIORITY_DEF
#define DeeSysThread_PRIORITY_DEF   DeePThreadSysThread_PRIORITY_DEF
#endif
#ifdef DeePThreadSysThread_PRIORITY_MAX
#define DeeSysThread_PRIORITY_MAX   DeePThreadSysThread_PRIORITY_MAX
#endif
#ifdef DeePThreadSysThread_GetPriority
#define DeeSysThread_GetPriority    DeePThreadSysThread_GetPriority
#endif
#ifdef DeePThreadSysThread_SetPriority
#define DeeSysThread_SetPriority    DeePThreadSysThread_SetPriority
#endif
#endif /* DeePThreadSysThread_priority_t */


DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS_PTHREAD_SYSTHREAD_H */
