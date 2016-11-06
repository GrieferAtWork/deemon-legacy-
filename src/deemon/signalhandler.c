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
#ifndef GUARD_DEEMON_SIGNALHANDLER_C
#define GUARD_DEEMON_SIGNALHANDLER_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include <deemon/signalhandler.h>
#include <deemon/runtime_flags.h>

#if DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS
#include <deemon/string.h>
#include <deemon/error.h>
#ifdef DEE_PLATFORM_UNIX
#if DEE_XCONFIG_SIGNALHANDLER_ALLOW_POSIX_SIGNALS
#include <deemon/sys/systls.h>
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SIGNAL_H
#include <signal.h>
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif /* DEE_XCONFIG_SIGNALHANDLER_ALLOW_POSIX_SIGNALS */
#endif

DEE_DECL_BEGIN

// s.a.: <deemon/runtime_flags.h>
int DeeFlag_NoSignalHandlers = 0;
extern void _DeeFlag_SetNoSignalHandlersString(char const *value);
void _DeeFlag_SetNoSignalHandlersString(char const *value) {
 int new_flag;
 if (DeeString_ToNumber(int,value,&new_flag) != 0) {
  DeeError_Print("[ignored] Invalid integer value in '$" DEE_AUTOCONF_VARNAME_DEEMON_NOSIGHANDLERS "' (defaulting to '0')\n",1);
  new_flag = 0;
 } else new_flag = !!new_flag;
 DEE_LVERBOSE1("Setting: 'DeeFlag_NoSignalHandlers = %d;'\n",new_flag);
 DeeFlag_NoSignalHandlers = new_flag;
}


#ifdef DEE_PLATFORM_WINDOWS
#if DEE_XCONFIG_SIGNALHANDLER_ALLOW_WIN32_SEH

#ifndef EXCEPTION_EXECUTE_HANDLER
#define EXCEPTION_EXECUTE_HANDLER  1
#endif

LONG WINAPI DeeSignalHandler_Win32SEH(
 LPEXCEPTION_POINTERS lpPointers, DWORD dwCode) {
 if (DeeFlag_NoSignalHandlers) return EXCEPTION_CONTINUE_SEARCH;
 switch (dwCode) {
#if defined(EXCEPTION_ACCESS_VIOLATION) || defined(STATUS_ACCESS_VIOLATION)
#ifdef EXCEPTION_ACCESS_VIOLATION
  case EXCEPTION_ACCESS_VIOLATION:
#else
  case STATUS_ACCESS_VIOLATION:
#endif
   if (lpPointers->ExceptionRecord->NumberParameters >= 1) {
    DeeError_SetStringf(&DeeErrorType_SegFault,
                        "EXCEPTION_ACCESS_VIOLATION : Code at %#p tried to %s %#p",
                        lpPointers->ExceptionRecord->ExceptionAddress,
                        lpPointers->ExceptionRecord->ExceptionInformation[0] == 0 ? "read from" :
                        lpPointers->ExceptionRecord->ExceptionInformation[0] == 1 ? "write to" :
                        lpPointers->ExceptionRecord->ExceptionInformation[0] == 8 ? "execute" : "??" "?",
                        lpPointers->ExceptionRecord->NumberParameters >= 2 ?
                        lpPointers->ExceptionRecord->ExceptionInformation[1] : (ULONG_PTR)0);
   } else DeeError_SET_STRING(&DeeErrorType_SegFault,"Access Violation");
   break;
#endif
#if defined(EXCEPTION_STACK_OVERFLOW) || defined(STATUS_STACK_OVERFLOW)
#ifdef EXCEPTION_STACK_OVERFLOW
  case EXCEPTION_STACK_OVERFLOW:
#else
  case STATUS_STACK_OVERFLOW:
#endif
   // This one might still break...
   DeeError_SET_STRING(&DeeErrorType_StackOverflow,
                       "EXCEPTION_STACK_OVERFLOW : Stack Overflow");
   break;
#endif
#if defined(EXCEPTION_FLT_DIVIDE_BY_ZERO) || defined(STATUS_FLOAT_DIVIDE_BY_ZERO)
#ifdef EXCEPTION_FLT_DIVIDE_BY_ZERO
  case EXCEPTION_FLT_DIVIDE_BY_ZERO:
#else
  case STATUS_FLOAT_DIVIDE_BY_ZERO:
#endif
   DeeError_SET_STRING(&DeeErrorType_DivideByZero,
                       "EXCEPTION_FLT_DIVIDE_BY_ZERO : Divide by 0");
   break;
#endif
#if defined(EXCEPTION_INT_DIVIDE_BY_ZERO) || defined(STATUS_INTEGER_DIVIDE_BY_ZERO)
#ifdef EXCEPTION_INT_DIVIDE_BY_ZERO
  case EXCEPTION_INT_DIVIDE_BY_ZERO:
#else
  case STATUS_INTEGER_DIVIDE_BY_ZERO:
#endif
   DeeError_SET_STRING(&DeeErrorType_DivideByZero,
                       "EXCEPTION_INT_DIVIDE_BY_ZERO : Divide by 0");
   break;
#endif
#if defined(EXCEPTION_ILLEGAL_INSTRUCTION) || defined(STATUS_ILLEGAL_INSTRUCTION)
#ifdef EXCEPTION_ILLEGAL_INSTRUCTION
  case EXCEPTION_ILLEGAL_INSTRUCTION:
#else
  case STATUS_ILLEGAL_INSTRUCTION:
#endif
   DeeError_SET_STRING(&DeeErrorType_IllegalInstruction,
                       "EXCEPTION_ILLEGAL_INSTRUCTION : Illegal Instruction");
   break;
#endif
  default: return EXCEPTION_CONTINUE_SEARCH;
 }
 return EXCEPTION_EXECUTE_HANDLER;
}

#ifdef DEE_PRIVATE_SIGNALHANDLER_HAVE_WIN32_EXCEPTION_ROUTER
EXCEPTION_DISPOSITION WINAPI _DeeSignalHandler_Win32ExceptionRouter(
 LPEXCEPTION_RECORD pRecord, DEE_SEH_EXCEPTION_REGISTRATION *pReg,
 LPCONTEXT pContext, LPEXCEPTION_RECORD pRecord2) {
 EXCEPTION_DISPOSITION result;
 EXCEPTION_POINTERS pointers;
 pointers.ContextRecord = pContext;
 pointers.ExceptionRecord = pRecord;
 result = (EXCEPTION_DISPOSITION)DeeSignalHandler_Win32SEH(&pointers,pRecord->ExceptionCode);
 if (result == ExceptionContinueExecution) longjmp(pReg->exthandler->context,1);
 return result;
}
#endif
#endif /* DEE_XCONFIG_SIGNALHANDLER_ALLOW_WIN32_SEH */
#endif

// Linux signal handlers
#ifdef DEE_PLATFORM_UNIX
#if DEE_XCONFIG_SIGNALHANDLER_ALLOW_POSIX_SIGNALS
#ifdef DEE_PLATFORM_WINDOWS
typedef void (__cdecl *_deesigh_handler_t)(int signo);
#else
typedef void (*_deesigh_handler_t)(int signo);
#endif

static int _deesigh_posix_installed = 0;
static struct DeeSysTLS _deesigh_posix_regs;
#define DeeSignalHandler_PosixGetRegs(result) DeeSysTLS_TryGetNofail(&_deesigh_posix_regs,result)
#define DeeSignalHandler_PosixSetRegs(value)  DeeSysTLS_TrySet(&_deesigh_posix_regs,value)\

static struct {
 struct sigaction old_SIGSEGV;
} _deesign_old_handlers;


static int register_signal(int sig, _deesigh_handler_t h, struct sigaction *old_handler) {
 struct sigaction action;
 action.sa_handler = h;
 sigemptyset(&action.sa_mask);
 action.sa_flags = SA_RESTART|SA_NODEFER;
 if (sigaction(sig,&action,old_handler) < 0) return 0;
 return 1;
} 
static void unregister_signal(int sig, struct sigaction *old_handler) {
 struct sigaction _old_action;
 sigaction(sig,old_handler,&_old_action);
} 



static void _deesigh_posix_handler(int signo) {
 struct DeeSignalHandler_PosixRegData *reg;
 // Yes, doing a lookup to a thread-local does cause undefined behavior...
 // That's why you can always disable signal handlers with "$DEEMON_NOSIGHANDLERS"
 DeeSignalHandler_PosixGetRegs(reg);
 if (reg != NULL) {
  reg->foundsig = signo;
  longjmp(reg->context,1);
  //DEE_LDEBUG("longjmp() returned?\n");
 }
 // Install the old signal handler
 switch (signo) {
  case SIGSEGV: unregister_signal(SIGSEGV,&_deesign_old_handlers.old_SIGSEGV); break;
  default: break;
 }
 raise(signo);
}

void DeeSignalHandler_PosixThrowSignal(int signo) {
 struct sigaction action,old_action;
 action.sa_handler = &_deesigh_posix_handler;
 sigemptyset(&action.sa_mask);
 action.sa_flags = SA_RESTART|SA_NODEFER;
 if (sigaction(signo,&action,&old_action) < 0) DEE_LDEBUG("NOT GOOD!\n");
 switch (signo) {
  case SIGSEGV:
   DeeError_SET_STRING(&DeeErrorType_SegFault,"SIGSEGV");
   break;
  default: // Shouldn't happen, but lets be careful
   DeeError_SetStringf(&DeeErrorType_Error,"Signal: %d",signo);
   break;
 }
}


void DeeSignalHandler_PosixInstall(void) {
 if (!_deesigh_posix_installed) {
  DeeSysTLS_Init(&_deesigh_posix_regs,return);
  if (!register_signal(SIGSEGV,&_deesigh_posix_handler,&_deesign_old_handlers.old_SIGSEGV)) {
   DeeSysTLS_Quit(&_deesigh_posix_regs);
  }
  _deesigh_posix_installed = 1;
 }
}
void DeeSignalHandler_PosixUninstall(void) {
 if (_deesigh_posix_installed) {
  _deesigh_posix_installed = 0;
  unregister_signal(SIGSEGV,&_deesign_old_handlers.old_SIGSEGV);
  DeeSysTLS_Quit(&_deesigh_posix_regs);
 }
}


void DeeSignalHandler_PosixBegin(
 DEE_A_IN struct DeeSignalHandler_PosixRegData *reg) {
 struct DeeSignalHandler_PosixRegData *old_reg;
 if (_deesigh_posix_installed && !DeeFlag_NoSignalHandlers) {
  DeeSignalHandler_PosixGetRegs(old_reg);
  reg->prev = old_reg;
  DeeSignalHandler_PosixSetRegs(reg);
 }
}
void DeeSignalHandler_PosixEnd(void) {
 struct DeeSignalHandler_PosixRegData *old_reg;
 if (_deesigh_posix_installed && !DeeFlag_NoSignalHandlers) {
  DeeSignalHandler_PosixGetRegs(old_reg);
  if (old_reg) DeeSignalHandler_PosixSetRegs(old_reg->prev);
 }
}
#endif /* DEE_XCONFIG_SIGNALHANDLER_ALLOW_POSIX_SIGNALS */
#endif

DEE_DECL_END
#else /* DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS */

#ifdef DEE_PRIVATE_SIGNALHANDLER_HAVE_HEADER_FLAG
#undef DEE_PRIVATE_SIGNALHANDLER_HAVE_HEADER_FLAG
DEE_DECL_BEGIN

// s.a.: <deemon/runtime_flags.h>
int DeeFlag_NoSignalHandlers = 1;
DEE_DECL_END
#endif

#endif /* !DEE_CONFIG_RUNTIME_HAVE_SIGNAL_HANDLERS */

#endif /* !GUARD_DEEMON_SIGNALHANDLER_C */
