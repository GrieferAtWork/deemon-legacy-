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
#ifndef GUARD_DEEMON_SIGNALHANDLER_H
#define GUARD_DEEMON_SIGNALHANDLER_H 1

#include <deemon/__conf.inl>
#include <deemon/__xconf.inl>

#if DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#ifdef DEE_PLATFORM_WINDOWS
#if DEE_XCONFIG_SIGNALHANDLER_ALLOW_WIN32_SEH
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
#include <Winternl.h>
DEE_COMPILER_MSVC_WARNING_POP
#endif
#endif
#if (!defined(DEE_PLATFORM_WINDOWS) || \
    (DEE_XCONFIG_SIGNALHANDLER_ALLOW_WIN32_SEH && \
    !DEE_COMPILER_HAVE_MSVC_SEH)) && \
     DEE_ENVIRONMENT_HAVE_INCLUDE_SETJMP_H
#include <setjmp.h>
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

#ifdef DEE_PLATFORM_WINDOWS
#if DEE_XCONFIG_SIGNALHANDLER_ALLOW_WIN32_SEH
extern LONG WINAPI DeeSignalHandler_Win32SEH(
 LPEXCEPTION_POINTERS lpPointers, DWORD dwCode);
#endif
#endif

#ifdef DEE_PLATFORM_WINDOWS
#if DEE_XCONFIG_SIGNALHANDLER_ALLOW_WIN32_SEH
#if DEE_COMPILER_HAVE_MSVC_SEH
// Implementation of WIN32 SEH using the msvc intrinsic syntax
#define DEE_SIGNALHANDLER_BEGIN __try
#define DEE_SIGNALHANDLER_DONE  (void)0
#define DEE_SIGNALHANDLER_END(on_failure) \
 __except(DeeSignalHandler_Win32SEH(\
  GetExceptionInformation(),GetExceptionCode()\
  )) {on_failure;}
#elif defined(__GNUC__) && DEE_ENVIRONMENT_HAVE_INCLUDE_SETJMP_H
// Implementation of WIN32 SEH using GCC assembly
struct _DEE_SEH_EXCEPTION_REGISTRATION;
typedef EXCEPTION_DISPOSITION (*LPDEE_EXCEPTION_HANDLER)(
 LPEXCEPTION_RECORD pRecord, struct _DEE_SEH_EXCEPTION_REGISTRATION *pReg,
 LPCONTEXT pContext, LPEXCEPTION_RECORD pRecord2);
typedef struct _DEE_SEH_HANDLER {
 jmp_buf context;
} DEE_SEH_HANDLER;
typedef struct _DEE_SEH_EXCEPTION_REGISTRATION {
 struct _DEE_SEH_EXCEPTION_REGISTRATION *prev;
 LPDEE_EXCEPTION_HANDLER                 handler;
 DEE_SEH_HANDLER                        *exthandler;
} DEE_SEH_EXCEPTION_REGISTRATION;
#define DEE_PRIVATE_SIGNALHANDLER_HAVE_WIN32_EXCEPTION_ROUTER
extern EXCEPTION_DISPOSITION _DeeSignalHandler_Win32ExceptionRouter(
 LPEXCEPTION_RECORD pRecord, DEE_SEH_EXCEPTION_REGISTRATION *pReg,
 LPCONTEXT pContext, LPEXCEPTION_RECORD pRecord2);
#define DEE_SEH_HANDLER_GET(phandler) __asm__ __volatile__("movl %%fs:0, %0" : "=r" (phandler))
#define DEE_SEH_HANDLER_SET(phandler) __asm__ __volatile__("movl %0, %%fs:0" : : "r" (phandler))
#define DEE_SIGNALHANDLER_BEGIN \
if(1){int _seh_setjmp_res;\
 DEE_SEH_EXCEPTION_REGISTRATION _seh_er;\
 DEE_SEH_HANDLER _seh_handler;\
 _seh_er.handler=&_DeeSignalHandler_Win32ExceptionRouter;\
 _seh_er.exthandler=&_seh_handler;\
 DEE_SEH_HANDLER_GET(_seh_er.prev);\
 DEE_SEH_HANDLER_SET(&_seh_er);\
 _seh_setjmp_res=setjmp(_seh_handler.context);\
 while(1){if(_seh_setjmp_res!=0)break;else
#define DEE_SIGNALHANDLER_DONE \
 DEE_SEH_HANDLER_SET(_seh_er.prev)
#define DEE_SIGNALHANDLER_END(on_failure) \
 break;}DEE_SIGNALHANDLER_DONE;\
 if(_seh_setjmp_res!=0){on_failure;}\
}else(void)0
#endif
#endif /* DEE_XCONFIG_SIGNALHANDLER_ALLOW_WIN32_SEH */
#endif


// Linux signal handlers
#ifdef DEE_PLATFORM_UNIX
#if DEE_XCONFIG_SIGNALHANDLER_ALLOW_POSIX_SIGNALS
struct DeeSignalHandler_PosixRegData {
 struct DeeSignalHandler_PosixRegData *prev;     /* [0..1] Previous handler. */
 jmp_buf                               context;  /* Handler context. */
 int                                   foundsig; /* Found signal. */
};
extern void DeeSignalHandler_PosixBegin(DEE_A_IN struct DeeSignalHandler_PosixRegData *reg);
extern void DeeSignalHandler_PosixEnd(void);
extern void DeeSignalHandler_PosixInstall(void);
extern void DeeSignalHandler_PosixUninstall(void);
extern void DeeSignalHandler_PosixThrowSignal(int signo);
#define DEE_INSTALL_SIGNALHANDLERS   DeeSignalHandler_PosixInstall
#define DEE_UNINSTALL_SIGNALHANDLERS DeeSignalHandler_PosixUninstall
#define DEE_SIGNALHANDLER_BEGIN \
if(1){int _sig_setjmp_res;\
 struct DeeSignalHandler_PosixRegData _sig_reg;\
 if((_sig_setjmp_res=setjmp(_sig_reg.context)\
 )==0)DeeSignalHandler_PosixBegin(&_sig_reg);\
 while(1){if(_sig_setjmp_res!=0)break;else
#define DEE_SIGNALHANDLER_DONE \
 DeeSignalHandler_PosixEnd()
#define DEE_SIGNALHANDLER_END(on_failure) \
 break;}DEE_SIGNALHANDLER_DONE;\
 if(_sig_setjmp_res!=0){\
 DeeSignalHandler_PosixThrowSignal(_sig_reg.foundsig);\
 {on_failure;}}\
}else(void)0
#endif /* DEE_XCONFIG_SIGNALHANDLER_ALLOW_POSIX_SIGNALS */
#endif


// NOTE: Don't define unless they actually do something
//#define DEE_INSTALL_SIGNALHANDLERS()   (void)0
//#define DEE_UNINSTALL_SIGNALHANDLERS() (void)0

DEE_DECL_END

#endif /* DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS */


//////////////////////////////////////////////////////////////////////////
// Signal handler
//////////////////////////////////////////////////////////////////////////
// Turns system/os exceptions into deemon exceptions
// Usage:
// >> DEE_SIGNALHANDLER_BEGIN {
// >>
// >>  ...
// >>
// >>  dangerous_function_call();
// >>
// >>  ...
// >>
// >>  if (condition) {
// >>   DEE_SIGNALHANDLER_DONE;
// >>   return 42;
// >>  }
// >>
// >>  ...
// >>
// >> } DEE_SIGNALHANDLER_END({
// >>  return -1; // Deemon exception already got thrown
// >> });
#ifndef DEE_SIGNALHANDLER_BEGIN
#define DEE_WITHOUT_SIGNALHANDLERS 1 // Ups...
#define DEE_SIGNALHANDLER_BEGIN           if(1)
#define DEE_SIGNALHANDLER_DONE           (void)0
#define DEE_SIGNALHANDLER_END(on_failure) else(void)0
#endif


// For better syntax highlighting
#ifdef __INTELLISENSE__
#undef DEE_SIGNALHANDLER_BEGIN
#undef DEE_SIGNALHANDLER_DONE
#undef DEE_SIGNALHANDLER_END
#define DEE_SIGNALHANDLER_BEGIN           if(1)do
#define DEE_SIGNALHANDLER_DONE            break
#define DEE_SIGNALHANDLER_END(on_failure)\
  while(0);else if(1){on_failure;}else(void)0
#endif

//////////////////////////////////////////////////////////////////////////
// Overrides for signal-handlers in different situations
#if DEE_CONFIG_RUNTIME_HAVE_LVALUE_OPS_SIGNAL_PROTECTED
#define DEE_LVALUE_SIGNALHANDLER_BEGIN           DEE_SIGNALHANDLER_BEGIN
#define DEE_LVALUE_SIGNALHANDLER_DONE            DEE_SIGNALHANDLER_DONE
#define DEE_LVALUE_SIGNALHANDLER_END             DEE_SIGNALHANDLER_END
#else
#define DEE_LVALUE_SIGNALHANDLER_BEGIN           if(1)
#define DEE_LVALUE_SIGNALHANDLER_DONE           (void)0
#define DEE_LVALUE_SIGNALHANDLER_END(on_failure) else(void)0
#endif

#define DEE_BUILTINMEM_SIGNALHANDLER_BEGIN       DEE_SIGNALHANDLER_BEGIN
#define DEE_BUILTINMEM_SIGNALHANDLER_DONE        DEE_SIGNALHANDLER_DONE
#define DEE_BUILTINMEM_SIGNALHANDLER_END         DEE_SIGNALHANDLER_END


#ifdef DEE_WITHOUT_SIGNALHANDLERS
#if DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS
#define DEE_PRIVATE_SIGNALHANDLER_HAVE_HEADER_FLAG 1
#endif
// Signal handlers aren't available...
#undef DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS
#undef DEE_CONFIG_RUNTIME_HAVE_LVALUE_OPS_SIGNAL_PROTECTED
#define DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS 0
#define DEE_CONFIG_RUNTIME_HAVE_LVALUE_OPS_SIGNAL_PROTECTED 0
#endif


#endif /* !GUARD_DEEMON_SIGNALHANDLER_H */
