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
#ifndef GUARD_DEEMON_SANDBOX_C
#define GUARD_DEEMON_SANDBOX_C 1
#define DEE_LIMITED_API 1

#if 0
#include <deemon/__conf.inl>
#include <deemon/mp/thread.h>
#include <deemon/runtime/sandbox.h>
#include <deemon/deemonrun.h>
#include <deemon/class.h>
#include <deemon/sequence.h>
#include <deemon/super.h>
#include <deemon/runtime/extern.h>
#include <deemon/runtime/function.h>
#include <deemon/cell.h>
#include <deemon/file.h>
#include <deemon/dict.h>
#include <deemon/set.h>
#include <deemon/tuple.h>
#include <deemon/list.h>
#include <deemon/runtime/builtins.h>
#include <deemon/weakref.h>
#include <deemon/runtime_flags.h>
#include <deemon/structured.h>
#include <deemon/bool.h>
#include <deemon/compiler/code.h>
#include <deemon/none.h>
#include <deemon/object.h>
#include <deemon/__alloca.inl>
#include <deemon/compiler/__opcode.inl>

DEE_DECL_BEGIN


DEE_COMPILER_MSVC_WARNING_PUSH(4102)
DEE_A_RET_EXCEPT_REF DeeObject *DeeSandbox_RunCode(
 DEE_A_IN struct DeeSandbox const *sbx, DEE_A_IN_OBJECT(DeeCodeObject) *self,
 DEE_A_IN Dee_size_t refc, DEE_A_IN_R(refc) DeeObject **refv,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeObject **argv,
 DEE_A_IN_OPT DeeObject *this_arg) {
 struct DeeStackFrame frame;
 DeeThreadObject *thread_self;
 (void)sbx;
 DEE_ASSERT(DeeObject_Check(self) && DeeCode_Check(self));
#ifdef DEE_DEBUG
 DEE_ASSERT(!refc || refv);
 DEE_ASSERT(!argc || argv);
#endif
 if ((thread_self = DeeThread_SELF()) == NULL) return NULL;
#if DEE_HAVE_ALLOCA
 frame.f_wbuf = (char *)alloca(((DeeCodeObject *)self)->co_wsize);
#else
 if ((frame.f_wbuf = (char *)mallocf_nz(((DeeCodeObject *)self)->co_wsize,
  "Workspace buffer for code %p",self)) == NULL) {
  DeeError_NoMemory();
  return NULL;
}
#endif
 // Make sure that locals are zero-initialized
 memset(frame.f_wbuf+((DeeCodeObject *)self)->co_woff_loc,0,
        ((DeeCodeObject *)self)->co_nlocals*sizeof(DeeObject *));
 DEE_ASSERT(DeeObject_Check(self) && DeeCode_Check(self));

#ifdef DEE_DEBUG
 // In debug mode, initialize the stack to ZERO, too
 memset(frame.f_stackv,0,((DeeCodeObject *)self)->co_stacksize*sizeof(DeeObject *));
#endif

 // Initialize the stack frame
#ifdef DEE_DEBUG
 frame.f_argc = argc;
 frame.f_refc = refc;
#endif
 frame.f_code = (DeeCodeObject *)self;
 frame.f_file = NULL;
 frame.f_flags = DEE_STACKFRAME_FLAG_NONE;
 // We must still reference 'this' because of tp_visit integrity
 Dee_XINCREF(frame.f_this = this_arg);
 frame.f_refv = refv;
 //frame.f_stackv = (DeeObject **)frame.f_wbuf; // Same memory address
 frame.f_stackv_end = frame.f_stackv;
 frame.f_localv = (DeeObject **)(frame.f_wbuf+((DeeCodeObject *)self)->co_woff_loc);
 frame.f_argv = argv;
 frame.f_addr = DeeCode_CODE(self);
 frame.f_constv = DeeTuple_ELEM(DeeCode_CONSTS(self));
 frame.f_result = NULL;
 frame.f_stored_exceptions = NULL;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 frame.f_exceptv_end =
 frame.f_exceptv = (struct DeeExceptionHandlerRef *)(frame.f_wbuf+((DeeCodeObject *)self)->co_woff_except);
 frame.f_finally_return_address_end = (Dee_uint8_t const **)(frame.f_wbuf+((DeeCodeObject *)self)->co_woff_finally);
#endif

 // Initialize the frame lock as already locked
 DeeAtomicMutex_InitLocked(&frame.f_lock);

 // Enable sandbox mode
#define SANDBOX

#ifndef __INTELLISENSE__
#include "_execute.inl"
#endif
exec_end:
 Dee_XDECREF(frame.f_this);
#if !DEE_HAVE_ALLOCA
 free_nn(frame.f_wbuf);
#endif
 return frame.f_result;
}
DEE_COMPILER_MSVC_WARNING_POP


DEE_DECL_END
#endif

#endif /* !GUARD_DEEMON_SANDBOX_C */
