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
#ifndef GUARD_DEEMON_SYS_SYSTHREAD_H
#define GUARD_DEEMON_SYS_SYSTHREAD_H 1

#include <deemon/__conf.inl>

//////////////////////////////////////////////////////////////////////////
//
// >> struct DeeSysThread { ... };
// >> #define DeeSysThread_THREADMAIN(name,closure)
//          - Use to declare the main function of a thread.
//          -> 'closure' must be a pointer variable declaration
//              that is passed to the thread upon creation.
//          >> static DeeSysThread_THREADMAIN(mythread_main) {
//          >>   int *my_data;
//          >> #ifdef DeeSysThread_THREADMAIN_BEGIN
//          >>   DeeSysThread_THREADMAIN_BEGIN();
//          >> #endif
//          >>   my_data = DeeSysThread_CLOSURE(int);
//          >>   DeeSysThread_RETURN(*my_data * 2);
//          >> #ifdef DeeSysThread_THREADMAIN_END
//          >>   DeeSysThread_THREADMAIN_END();
//          >> #endif
//          >> }
// >> [[optional]] void DeeSysThread_THREADMAIN_BEGIN(void);
// >> [[optional]] void DeeSysThread_THREADMAIN_END(void);
//          - Callbacks that must be executed at the start and end of any thread callback
// >> typedef ... DeeSysThread_return_t;
// >> void DeeSysThread_RETURN(DeeSysThread_return_t retval);
//          - Return a value and end the current thread
//            NOTE: May only be used within 'DeeSysThread_THREADMAIN'
//            NOTE: The given 'retval' may, or may not be ignored
// >> void DeeSysThread_Init(DEE_A_OUT struct DeeSysThread *self, <?> *thread_main, void *closure, CODE on_error);
//          - Initialize and start a given thread
// [[optional /* Functions for suspending/resuming threads (must not be supported) */
// >> [[optional]] void DeeSysThread_InitSuspended(DEE_A_OUT struct DeeSysThread *self, <?> *thread_main, void *closure, CODE on_error);
//          - NOTE: A thread initialized as suspended must be started with 'DeeSysThread_Resume'
// >> [[optional]] void DeeSysThread_Suspend(DEE_A_INOUT struct DeeSysThread *self, CODE on_error);
// >>              void DeeSysThread_Resume(DEE_A_INOUT struct DeeSysThread *self, CODE on_error);
// >> [[optional]] #define DEE_SYSTHREAD_SUSPENDRESUME_RECURSIVE
// ]]
//
//////////////////////////////////////////////////////////////////////////
//
// >> void DeeSysThread_GetSelf(DEE_A_OUT struct DeeSysThread *tdescr);
//          - Stores a thread descriptor for the calling in '*tdescr'
//            WARNING: A thread should not join, detach, or quit itself! (undefined behavior, jaddi jaddi jadda...)
// >> void DeeSysThread_Equals(DEE_A_IN struct DeeSysThread const *a, DEE_A_IN struct DeeSysThread const *b, int *result);
//          - Returns true for two threads equal to each other
// >> void DeeSysThread_IsSelf(DEE_A_IN struct DeeSysThread const *tdescr, int *result);
//          - Returns true if 'tdescr' is equal to the descriptor returned by 'DeeSysThread_GetSelf'
//////////////////////////////////////////////////////////////////////////
//
// >> [[optional]] void DeeSysThread_SetSelfName(DEE_A_IN_Z char const *name);
//          - Sets the name of the current thread for use in debugging
//
//////////////////////////////////////////////////////////////////////////
// 
// === Thread IDs ===
// >> [[optional
// >>              void DeeSysThread_GetTID(DEE_A_IN struct DeeSysThread const *self, DEE_A_OUT Dee_tid_t *result);
// >> [[optional]] void DeeSysThread_GetSelfTID(DEE_A_OUT Dee_tid_t *result);
// >> ]]
// 
//////////////////////////////////////////////////////////////////////////
//
// === Thread destructors ===
// >> void DeeSysThread_Join(DEE_A_INOUT struct DeeSysThread *self, DEE_A_OUT_OPT DeeSysThread_return_t *retval, CODE on_error);
// >> void DeeSysThread_Detach(DEE_A_INOUT struct DeeSysThread *self, CODE on_error);
// >> void DeeSysThread_DetachNoexcept(DEE_A_INOUT struct DeeSysThread *self);
// >> [[optional]] void DeeSysThread_Quit(DEE_A_INOUT struct DeeSysThread *self);
// -> To destroy a thread, the following must be done:
//   1. Either call 'DeeSysThread_Join', 'DeeSysThread_Detach' or 'DeeSysThread_DetachNoexcept'
//   2. If defined, call 'DeeSysThread_Quit' (must always be the last call)
//   NOTE: Some platforms may implement a set of special functions that can
//         be called between joining a thread and destroying it. (e.g.: Win32 GetThreadTimes)
//   HINT: To destroy a thread without the risk of errors, use 'DeeSysThread_DetachNoexcept'
//
//////////////////////////////////////////////////////////////////////////
//
// === Thread priority ===
// >> [[optional
// >> typedef ... DeeSysThread_priority_t;
// >> [[optional]] static DeeSysThread_priority_t const DeeSysThread_PRIORITY_MIN = ...;
// >> [[optional]] static DeeSysThread_priority_t const DeeSysThread_PRIORITY_DEF = ...;
// >> [[optional]] static DeeSysThread_priority_t const DeeSysThread_PRIORITY_MAX = ...;
// >> double DeeSysThread_PRIORITY2FLOAT(DeeSysThread_priority_t p);
// >> DeeSysThread_priority_t DeeSysThread_FLOAT2PRIORITY(double p);
// >> void DeeSysThread_GetPriority(DEE_A_INOUT struct DeeSysThread *self, DEE_A_OUT DeeSysThread_priority_t *result, CODE on_error);
// >> void DeeSysThread_SetPriority(DEE_A_INOUT struct DeeSysThread *self, DEE_A_IN DeeSysThread_priority_t value, CODE on_error);
// >> ]]
//
//////////////////////////////////////////////////////////////////////////

#if defined(DEE_PLATFORM_WINDOWS)
# include <deemon/sys/win32/systhread.h>
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_PTHREAD_H
# include <deemon/sys/pthread/systhread.h>
#else
# error "No thread implementation for this platform (Compile with -DDEE_WITHOUT_THREADS)"
#endif


//////////////////////////////////////////////////////////////////////////
// Fallback/Automatic implementations

#if !defined(DeeSysThread_GetSelfTID)\
  && defined(DeeSysThread_GetTID)
#define DeeSysThread_GetSelfTID(result) \
do{\
 struct DeeSysThread _tid_self;\
 DeeSysThread_GetSelf(&_tid_self);\
 DeeSysThread_GetTID(&_tid_self,result);\
}while(0)
#endif

#if !defined(DeeSysThread_Equals)\
  && defined(DeeSysThread_GetTID)
#define DeeSysThread_Equals(a,b,result) \
do{\
 Dee_tid_t _tid_a,_tid_b;\
 DeeSysThread_GetTID(a,&_tid_a);\
 DeeSysThread_GetTID(b,&_tid_b);\
 *(result) = _tid_a == _tid_b;\
}while(0)
#endif

#ifndef DeeSysThread_IsSelf
#if defined(DeeSysThread_GetTID)\
 && defined(DeeSysThread_GetSelfTID)
#define DeeSysThread_IsSelf(tdescr,result) \
do{\
 Dee_tid_t _tid_self,_tid_descr;\
 DeeSysThread_GetSelfTID(&_tid_self);\
 DeeSysThread_GetTID(tdescr,&_tid_descr);\
 *(result) = _tid_self == _tid_descr;\
}while(0)
#elif defined(DeeSysThread_Equals)\
   && defined(DeeSysThread_GetSelf)
#define DeeSysThread_IsSelf(tdescr,result) \
do{\
 struct DeeSysThread _tself;\
 DeeSysThread_GetSelf(&_tself);\
 DeeSysThread_Equals(&_tself,tdescr,result);\
}while(0)
#endif
#endif

#if defined(DeeSysThread_PRIORITY_MIN)\
 && defined(DeeSysThread_PRIORITY_MAX)\
 && defined(DeeSysThread_priority_t)
#ifndef DeeSysThread_PRIORITY2FLOAT
#define DeeSysThread_PRIORITY2FLOAT(p) \
 ((double)((p)-DeeSysThread_PRIORITY_MIN)/\
  (DeeSysThread_PRIORITY_MAX-DeeSysThread_PRIORITY_MIN))
#endif
#ifndef DeeSysThread_FLOAT2PRIORITY
#define DeeSysThread_FLOAT2PRIORITY(p) \
 ((DeeWin32SysThread_priority_t)((double)(p)*\
  (DeeSysThread_PRIORITY_MAX-DeeSysThread_PRIORITY_MIN)\
                           )+DeeSysThread_PRIORITY_MIN)
#endif
#endif

#ifndef DeeSysThread_priority_t
#undef DeeSysThread_PRIORITY_MIN
#undef DeeSysThread_PRIORITY_DEF
#undef DeeSysThread_PRIORITY_MAX
#undef DeeSysThread_PRIORITY2FLOAT
#undef DeeSysThread_FLOAT2PRIORITY
#undef DeeSysThread_GetPriority
#undef DeeSysThread_SetPriority
#endif


#endif /* !GUARD_DEEMON_SYS_SYSTHREAD_H */
