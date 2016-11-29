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
// >> struct DeeSysThread { ... };
// >> #define DeeSysThread_THREADMAIN(name,closure)
//          - Use to declare the main function of a thread.
//          -> 'closure' must be a pointer variable declaration
//              that is passed to the thread upon creation.
//          >> static DeeSysThread_THREADMAIN(mythread_main,void *arg) {
//          >> #ifdef DeeSysThread_THREADMAIN_BEGIN
//          >>   DeeSysThread_THREADMAIN_BEGIN();
//          >> #endif
//          >>   DeeSysThread_RETURN(42);
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
//


#if defined(DEE_PLATFORM_WINDOWS)
# include <deemon/sys/win32/systhread.h>
#else
# error "No thread implementation for this platform"
#endif


#endif /* !GUARD_DEEMON_SYS_SYSTHREAD_H */
