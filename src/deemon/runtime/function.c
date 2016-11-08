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
#ifndef GUARD_DEEMON_FUNCTION_C
#define GUARD_DEEMON_FUNCTION_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/bool.h>
#include <deemon/compiler/code.h>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/gc.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/mp/thread.h>
#include <deemon/none.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_visit.h>
#include <deemon/runtime/execute.h>
#include <deemon/runtime/function.h>
#include <deemon/string.h>
#include <deemon/tuple.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/compiler/__opcode.inl>
#include <deemon/marshal_data.h>

// */ (nano...)

#include DEE_INCLUDE_MEMORY_API()

#ifndef __INTELLISENSE__
#define FLAGS  DEE_FUNCTION_FLAG_NONE
#include "function.call.inl"
#define FLAGS  DEE_FUNCTION_FLAG_VARG
#include "function.call.inl"
#define FLAGS  DEE_FUNCTION_FLAG_YILD
#include "function.call.inl"
#define FLAGS  DEE_FUNCTION_FLAG_THIS
#include "function.call.inl"
#define FLAGS (DEE_FUNCTION_FLAG_VARG|DEE_FUNCTION_FLAG_YILD)
#include "function.call.inl"
#define FLAGS (DEE_FUNCTION_FLAG_VARG|DEE_FUNCTION_FLAG_THIS)
#include "function.call.inl"
#define FLAGS (DEE_FUNCTION_FLAG_YILD|DEE_FUNCTION_FLAG_THIS)
#include "function.call.inl"
#define FLAGS (DEE_FUNCTION_FLAG_VARG|DEE_FUNCTION_FLAG_YILD|DEE_FUNCTION_FLAG_THIS)
#include "function.call.inl"
#endif

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT_REF DeeObject *DeeFunction_New(
 DEE_A_INOUT_OBJECT(DeeCodeObject) *code,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *refs,
 DEE_A_IN Dee_size_t argc, DEE_A_IN Dee_uint32_t flags) {
 DeeFunctionObject *result;
 DEE_ASSERT(DeeObject_Check(code) && DeeCode_Check(code));
 DEE_ASSERT(DeeObject_Check(refs) && DeeTuple_Check(refs));
 if DEE_LIKELY((result = DeeGC_MALLOC(DeeFunctionObject)) != NULL) {
  DeeObject_INIT(result,&DeeFunction_Type);
  DeeAtomicMutex_Init(&result->fn_lock);
  Dee_INCREF(result->fn_code = (DeeCodeObject *)code);
  Dee_INCREF(result->fn_refs = (DeeTupleObject *)refs);
  result->fn_argc = argc;
  result->fn_flags = flags;
  DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}

DEE_A_RET_OBJECT_REF(DeeCodeObject) *DeeFunction_Code(
 DEE_A_IN_OBJECT(DeeFunctionObject) const *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeFunction_Check(self));
 DeeFunction_ACQUIRE(self);
 Dee_INCREF(result = DeeFunction_CODE(self));
 DeeFunction_RELEASE(self);
 return result;
}
DEE_A_RET_OBJECT_REF(DeeTupleObject) *DeeFunction_Refs(
 DEE_A_IN_OBJECT(DeeFunctionObject) const *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeFunction_Check(self));
 DeeFunction_ACQUIRE(self);
 Dee_INCREF(result = DeeFunction_REFS(self));
 DeeFunction_RELEASE(self);
 return result;
}
DEE_A_RET_WUNUSED Dee_size_t DeeFunction_Argc(
 DEE_A_IN_OBJECT(DeeFunctionObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFunction_Check(self));
 return DeeFunction_ARGC(self);
}
DEE_A_RET_WUNUSED Dee_uint32_t DeeFunction_Flags(
 DEE_A_IN_OBJECT(DeeFunctionObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFunction_Check(self));
 return DeeFunction_FLAGS(self);
}

DEE_A_RET_EXCEPT_REF DeeObject *DeeFunction_Call(
 DEE_A_IN_OBJECT(DeeFunctionObject) const *self,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFunction_Check(self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 switch (DeeFunction_FLAGS(self)) {
#ifndef __INTELLISENSE__
  case DEE_FUNCTION_FLAG_NONE: return DeeFunction_CallImpl(self,args);
  case DEE_FUNCTION_FLAG_VARG: return DeeFunction_CallImplV(self,args);
  case DEE_FUNCTION_FLAG_YILD: return DeeFunction_CallImplY(self,args);
  case DEE_FUNCTION_FLAG_THIS: return DeeFunction_CallImplT(self,args);
  case DEE_FUNCTION_FLAG_VARG|DEE_FUNCTION_FLAG_YILD: return DeeFunction_CallImplVY(self,args);
  case DEE_FUNCTION_FLAG_VARG|DEE_FUNCTION_FLAG_THIS: return DeeFunction_CallImplVT(self,args);
  case DEE_FUNCTION_FLAG_YILD|DEE_FUNCTION_FLAG_THIS: return DeeFunction_CallImplYT(self,args);
  case DEE_FUNCTION_FLAG_VARG|DEE_FUNCTION_FLAG_YILD|DEE_FUNCTION_FLAG_THIS: return DeeFunction_CallImplVYT(self,args);
#endif
  default: DEE_BUILTIN_UNREACHABLE();
 }
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeFunction_ThisCall(
 DEE_A_IN_OBJECT(DeeFunctionObject) const *self, DEE_A_INOUT DeeObject *this_arg,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFunction_Check(self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 switch (DeeFunction_FLAGS(self)) {
#ifndef __INTELLISENSE__
  case DEE_FUNCTION_FLAG_NONE: return DeeFunction_ThisCallImpl(self,this_arg,args);
  case DEE_FUNCTION_FLAG_VARG: return DeeFunction_ThisCallImplV(self,this_arg,args);
  case DEE_FUNCTION_FLAG_YILD: return DeeFunction_ThisCallImplY(self,this_arg,args);
  case DEE_FUNCTION_FLAG_THIS: return DeeFunction_ThisCallImplT(self,this_arg,args);
  case DEE_FUNCTION_FLAG_VARG|DEE_FUNCTION_FLAG_YILD: return DeeFunction_ThisCallImplVY(self,this_arg,args);
  case DEE_FUNCTION_FLAG_VARG|DEE_FUNCTION_FLAG_THIS: return DeeFunction_ThisCallImplVT(self,this_arg,args);
  case DEE_FUNCTION_FLAG_YILD|DEE_FUNCTION_FLAG_THIS: return DeeFunction_ThisCallImplYT(self,this_arg,args);
  case DEE_FUNCTION_FLAG_VARG|DEE_FUNCTION_FLAG_YILD|DEE_FUNCTION_FLAG_THIS: return DeeFunction_ThisCallImplVYT(self,this_arg,args);
#endif
  default: DEE_BUILTIN_UNREACHABLE();
 }
}




DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeYieldFunctionIterator_Init(
 DEE_A_INOUT DeeYieldFunctionIteratorObject *self, DEE_A_IN DeeYieldFunctionObject const *yfunc) {
 DeeCodeObject *code; char *wbuf;
 DEE_ASSERT(DeeObject_Check(self) && DeeYieldFunctionIterator_Check(self));
 DEE_ASSERT(DeeObject_Check(yfunc) && DeeYieldFunction_Check(yfunc));
 code = yfunc->yf_func->fn_code;
 while DEE_UNLIKELY((wbuf = (char *)mallocf_nz(code->co_wsize,
  "Workspace buffer for yield_function %p",yfunc)) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 // Make sure that locals are zero-initialized
 memset(wbuf+code->co_woff_loc,0,code->co_nlocals*sizeof(DeeObject *));

 Dee_INCREF(self->yi_func = yfunc->yf_func);
 Dee_INCREF(self->yi_args = yfunc->yf_args);
 //result->yi_frame.f_wbuf = wbuf;
 self->yi_frame.f_prev = NULL;
 self->yi_frame.f_stackv = (DeeObject **)wbuf;
 self->yi_frame.f_localv = (DeeObject **)(wbuf+code->co_woff_loc);
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 self->yi_frame.f_exceptv_end = 
 self->yi_frame.f_exceptv = (struct DeeExceptionHandlerRef *)(wbuf+code->co_woff_except);
 self->yi_frame.f_finally_return_address_end = (Dee_uint8_t const **)(wbuf+code->co_woff_finally);
#endif
 Dee_XINCREF(self->yi_frame.f_this = yfunc->yf_this);
 Dee_INCREF(self->yi_frame.f_code = code);
 self->yi_frame.f_argv = DeeTuple_ELEM(self->yi_args);
 DeeAtomicMutex_Init(&self->yi_frame.f_lock);
 self->yi_frame.f_addr = code->co_code;
 self->yi_frame.f_constv = DeeTuple_ELEM(code->co_consts);
 self->yi_frame.f_refv = DeeTuple_ELEM(self->yi_func->fn_refs);
#ifdef DEE_DEBUG
 self->yi_frame.f_argc = DeeTuple_SIZE(self->yi_args);
 self->yi_frame.f_refc = DeeTuple_SIZE(self->yi_func->fn_refs);
#endif
 self->yi_frame.f_stackv_end = self->yi_frame.f_stackv;
 self->yi_frame.f_result = NULL;
 self->yi_frame.f_stored_exceptions = NULL;
 DeeAtomicMutex_Init(&self->yi_lock);
 return 0;
}

static DeeYieldFunctionIteratorObject *DEE_CALL
_deeyieldfunction_tp_seq_iter_self(DeeYieldFunctionObject *self) {
 DeeYieldFunctionIteratorObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeYieldFunction_Check(self));
 if DEE_LIKELY((result = (DeeYieldFunctionIteratorObject *)DeeGC_MALLOCF(
  DeeYieldFunctionIteratorObject,"yield function iterator for %p",self)) != NULL) {
  DeeObject_INIT(result,&DeeYieldFunctionIterator_Type);
  if DEE_UNLIKELY(_DeeYieldFunctionIterator_Init(result,self) != 0) {
   _DeeObject_DELETE(&DeeYieldFunctionIterator_Type,result);
   result = NULL;
  } else DeeGC_TrackedAdd((DeeObject *)result);
 }
 return result;
}

static int _deeyieldfunction_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeYieldFunctionObject *self, DeeYieldFunctionObject *right) {
 Dee_INCREF(self->yf_func = right->yf_func);
 Dee_INCREF(self->yf_args = right->yf_args);
 Dee_XINCREF(self->yf_this = right->yf_this);
 return 0;
}
static int _deeyieldfunction_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeYieldFunctionObject *self, DeeObject *args) {
 DeeObject *func_,*args_,*this_ = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo|o:yield_function",&func_,&args_,&this_) != 0 ||
                 DeeError_TypeError_CheckTypeExact(func_,&DeeFunction_Type) != 0 ||
                 DeeError_TypeError_CheckTypeExact(args_,&DeeTuple_Type) != 0) return -1;
 Dee_INCREF(self->yf_func = (DeeFunctionObject *)func_);
 Dee_INCREF(self->yf_args = (DeeTupleObject *)args_);
 Dee_XINCREF(self->yf_this = this_);
 return 0;
}
static void _deeyieldfunction_tp_dtor(DeeYieldFunctionObject *self) {
 Dee_DECREF(self->yf_func);
 Dee_DECREF(self->yf_args);
 Dee_XDECREF(self->yf_this);
}
static DeeObject *_deeyieldfunction_tp_str(DeeYieldFunctionObject *self) {
 return DeeString_Newf("%k%k",self->yf_func,self->yf_args);
}
static DeeObject *_deeyieldfunction_tp_repr(DeeYieldFunctionObject *self) {
 return DeeString_Newf("<yield_function %k%k>",
                       self->yf_func,self->yf_args);
}
DEE_VISIT_PROC(_deeyieldfunction_tp_visit,DeeYieldFunctionObject *self) {
 Dee_VISIT(self->yf_func);
 Dee_VISIT(self->yf_args);
 Dee_XVISIT(self->yf_this);
}




static void _deeyieldfunctioniterator_tp_dtor(DeeYieldFunctionIteratorObject *self) {
 Dee_DECREF(self->yi_func);
 Dee_DECREF(self->yi_args);

 // Since we allocated all the execution space on the heap, we have to free it here
 free_nn(self->yi_frame.f_wbuf);

 // yield_function iterators store a reference to their code
 Dee_DECREF(self->yi_frame.f_code);
}

static DeeFunctionObject _Dee_EmptyFunction = {
 DEE_OBJECT_HEAD_INIT(&DeeFunction_Type),
 (struct DeeCodeObject *)Dee_EmptyCode,   // fn_code
 (struct DeeTupleObject *)Dee_EmptyTuple, // fn_refs
 DeeAtomicMutex_INIT(),                   // fn_lock
 DEE_FUNCTION_FLAG_NONE,                  // fn_flags
 0                                        // fn_argc
};

static void _deeyieldfunctioniterator_tp_clear(DeeYieldFunctionIteratorObject *self) {
 DeeThreadObject *thread_self; int error;
 DeeObject **old_stackv,**iter,**end,*old_this,*old_result;
 DeeFunctionObject *old_func; DeeCodeObject *old_code;
 DeeTupleObject *old_args;
 Dee_size_t old_stackc;
start_over:
 DEE_ASSERTF(!self->yi_frame.f_prev,
             "Yield-function iterator stackframe still in use during tp_clear");
 // Crazy complicated and hacky code to execute any remaining finally blocks
 DeeAtomicMutex_Acquire(&self->yi_lock);
 if DEE_LIKELY((thread_self = DeeThread_SELF()) != NULL) {
  struct DeeRaisedException *stored_exceptions,*next_stored_exceptions;
  DeeThread_PUSH_EXCEPTIONS(thread_self);
#define frame self->yi_frame
  // There can still be some exceptions stored if we stopped after
  // a yield within a finally block after an exception was thrown.
  // (boy, that sounds just as complicated as it is...):
  // >> try {
  // >>   throw x;
  // >> } finally {
  // >>   try {
  // >>     yield y; // After this yield, iteration is aborted
  // >>   } finally { // When this finally gets executed, 'x' is still in the stored exception list >> just dump it
  // >>     ...
  // >>   }
  // >> }
  // And yes: as you can see, you have to work really hard to
  //          actually get an exception to show up here...
  DeeAtomicMutex_Acquire(&frame.f_lock);
  while DEE_UNLIKELY((stored_exceptions = frame.f_stored_exceptions) != NULL) {
   // Clear stored exceptions
   frame.f_stored_exceptions = NULL;
   DeeAtomicMutex_Release(&frame.f_lock);
   DeeAtomicMutex_Release(&self->yi_lock);
   while (1) {
    next_stored_exceptions = stored_exceptions->re_prev;
    DeeError_Display("[Ignored] Unhandled stored exception in yield_function.iterator destructor\n",
                     (DeeObject *)stored_exceptions->re_error,
                     (DeeObject *)stored_exceptions->re_traceback);
    _DeeRaisedException_Quit(stored_exceptions);
    _DeeRaisedException_Free(stored_exceptions);
    if DEE_LIKELY(!next_stored_exceptions) break;
    stored_exceptions = next_stored_exceptions;
   }
   DeeAtomicMutex_Acquire(&self->yi_lock);
   DeeAtomicMutex_Acquire(&frame.f_lock);
  }
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  // Execute remaining exception handlers
exec_except:
  if DEE_UNLIKELY(frame.f_exceptv_end != frame.f_exceptv) {
   struct DeeExceptionHandlerEntry *handler;
   DeeObject **required_stack_end;
   static Dee_uint8_t stop_code[] = {OP_STOP};
   handler = (--frame.f_exceptv_end)->r_handler; // Consume one handler
   if (handler->e_kind != DeeExceptionHandleKind_FINALLY) goto exec_except; // Inappropriate handler
   // Align the stack
   required_stack_end = frame.f_stackv+handler->e_stack;
   DEE_ASSERT(frame.f_stackv_end >= required_stack_end);
   if (frame.f_stackv_end != required_stack_end) {
    // Must re-align the stack.
    // NOTE: We can't just call decref on the entires right now.
    //    >> We have to do that after calling the finally-block
    old_stackc = (Dee_size_t)(frame.f_stackv_end-required_stack_end);
    if ((old_stackv = (DeeObject **)malloc_nz(old_stackc*sizeof(DeeObject *))) == NULL) {
     if (Dee_CollectMemory()) goto start_over;
     // This is bad...
     DeeError_NoMemory();
     goto exec_error;
    }
    // Move the dropped entires into our temporary cache
    memcpy(old_stackv,frame.f_stackv_end,
           old_stackc*sizeof(DeeObject *));
#ifdef DEE_DEBUG
    DEE_LVERBOSE1("Aligning stack for finally-block after destruction of "
                  "yield_function.iterator (dropping %Iu slot(s))\n",
                  (Dee_size_t)(frame.f_stackv_end-required_stack_end));
#endif /* DEE_DEBUG */
    frame.f_stackv_end = required_stack_end;
   } else {
    old_stackc = 0;
    old_stackv = NULL;
   }
   // Navigate to the exception handler
   frame.f_addr = frame.f_code->co_code+handler->e_handler;
   // There can still be some finally return addresses hanging around,
   // if a yield statement was encountered within a nested finally block
   // >> try {
   // >>   ...
   // >> } finally { // The return address of this finally is left dangling once we reach the yield.
   // >>   try {
   // >>     ...
   // >>   } finally {
   // >>     yield x; // this yield...
   // >>   }
   // >> }
   frame.f_finally_return_address_end = _DeeStackFrame_FINALLY_RETURN_ADDRESS_BASE(frame);
   // Hide additional exceptions to simulate an exception-free
   // environment while executing exception handler code
   // NOTE: These exceptions are restored once the handler is finished
   // NOTE2: Unless it's a finally handler, we delete one exception
   // This is another very rare case where the destructor was called
   // from a C callback after an error was thrown.
   if DEE_UNLIKELY(thread_self->t_exception) {
    frame.f_stored_exceptions = thread_self->t_exception;
    DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
    thread_self->t_exception = NULL;
    DeeAtomicMutex_Release(&thread_self->t_exception_lock);
   }
   *frame.f_finally_return_address_end++ = stop_code;
   error = _DeeStackFrame_RunLocked_AndUnlock(thread_self,&frame);
   DeeAtomicMutex_Release(&self->yi_lock);

   // All the locks have been released, and the finally-block was executed
   // >> Time to clear the stack buffer and check for errors
   DEE_ASSERT((old_stackc != 0) == (old_stackv != NULL));
   while (old_stackc--) Dee_DECREF(old_stackv[old_stackc]);
   free(old_stackv);

   // Check for an error
   if DEE_UNLIKELY(error != 0) {
exec_error:
    while (!DeeError_Print("[Ignored] Unhandled exception in yield_function.iterator destructor\n",1));
   }

   // Since everything can have already changed again, we need to start over
   goto start_over;
  } else {
   DeeAtomicMutex_Release(&frame.f_lock);
  }
#endif
  DeeThread_POP_EXCEPTIONS_NO_NEW(thread_self);
#undef frame
 }
 // NOTE: At this point, the frame has no more exception handlers set,
 //       and our thread has a lock to 'self->yi_lock', but NOT 'self->yi_frame.f_lock'
 old_func = self->yi_func;
 Dee_INCREF(self->yi_func = &_Dee_EmptyFunction);
 old_args = self->yi_args;
 Dee_INCREF(self->yi_args = (DeeTupleObject *)Dee_EmptyTuple);

 // Clear the last remainders out of the frame
 DeeAtomicMutex_AcquireRelaxed(&self->yi_frame.f_lock);
 DEE_ASSERT(!self->yi_frame.f_prev);
 old_code = self->yi_frame.f_code;
 Dee_INCREF(self->yi_frame.f_code = (struct DeeCodeObject *)Dee_EmptyCode);
 self->yi_frame.f_argv = NULL;
#ifdef DEE_DEBUG
 self->yi_frame.f_argc = 0;
#endif
 self->yi_frame.f_addr = DeeCode_CODE(Dee_EmptyCode);
 self->yi_frame.f_constv = DeeTuple_ELEM(Dee_EmptyTuple);
 self->yi_frame.f_refv = DeeTuple_ELEM(Dee_EmptyTuple);
#ifdef DEE_DEBUG
 self->yi_frame.f_refc = 0;
#endif

 self->yi_frame.f_localv = (DeeObject **)self->yi_frame.f_wbuf;
 //DEE_ASSERT(self->yi_frame.f_stackv_end == self->yi_frame.f_stackv);
 old_this = self->yi_frame.f_this;
 self->yi_frame.f_this = NULL;
 old_result = self->yi_frame.f_result;
 self->yi_frame.f_result = NULL;
 DEE_ASSERT(self->yi_frame.f_stored_exceptions == NULL);
#if DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS
 self->yi_frame.f_exceptv = NULL;
 self->yi_frame.f_exceptv_end = NULL;
#endif

 // Clear the stack
 if DEE_UNLIKELY(self->yi_frame.f_stackv_end != self->yi_frame.f_stackv) {
  DEE_ASSERTF(_DeeStackFrame_StackSize(&self->yi_frame) <= old_code->co_stacksize,
              "Frame exceeds maximum stack size specified in code (%Iu > %Iu)",
              _DeeStackFrame_StackSize(&self->yi_frame),old_code->co_stacksize);
  iter = self->yi_frame.f_stackv,end = self->yi_frame.f_stackv_end;
  self->yi_frame.f_stackv_end = self->yi_frame.f_stackv;
  // NOTE: Since we've already overwritten the code, anything using this
  //       stackframe (except for us) will think that there isn't a stack
  DeeAtomicMutex_Release(&self->yi_frame.f_lock);
  DeeAtomicMutex_Release(&self->yi_lock);
  // Cleanup in reverse order
  DEE_ASSERT(iter != end);
  do Dee_DECREF(*--end); while (iter != end);
  DeeAtomicMutex_AcquireRelaxed(&self->yi_lock);
  DeeAtomicMutex_AcquireRelaxed(&self->yi_frame.f_lock);
  DEE_ASSERTF(self->yi_frame.f_stackv == self->yi_frame.f_stackv_end,
              "With no code, it shouldn't have been possible to add new stack entires...\n"
              "And even then, this should have already been detected by prior assertions about too-large stack checks");
  DEE_ASSERT(self->yi_frame.f_code == (struct DeeCodeObject *)Dee_EmptyCode);
 }

 // Cleanup Local variables
 if (old_code->co_nlocals != 0) {
  // NOTE: Since we've already overwritten the code, anything using
  //       this stackframe (except for us) will think that there aren't any locals
  end = (iter = (DeeObject **)(self->yi_frame.f_wbuf+old_code->co_woff_loc))+old_code->co_nlocals;
  DeeAtomicMutex_Release(&self->yi_frame.f_lock);
  DeeAtomicMutex_Release(&self->yi_lock);
  DEE_ASSERT(iter != end);
  do Dee_XDECREF(*iter++); while DEE_LIKELY(iter != end);
  DeeAtomicMutex_AcquireRelaxed(&self->yi_lock);
  DeeAtomicMutex_AcquireRelaxed(&self->yi_frame.f_lock);
  DEE_ASSERT(self->yi_frame.f_code == (struct DeeCodeObject *)Dee_EmptyCode);
 }

 DeeAtomicMutex_Release(&self->yi_frame.f_lock);
 DeeAtomicMutex_Release(&self->yi_lock);

 // Destroy everything we have yet to clean up
 Dee_DECREF(old_func);
 Dee_DECREF(old_args);
 Dee_DECREF(old_code);
 Dee_XDECREF(old_this);
 Dee_XDECREF(old_result);
}

static int _deeyieldfunctioniterator_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeYieldFunctionIteratorObject *self, DeeYieldFunctionIteratorObject *right) {
 DeeTupleObject *rhs_args; int error;
 // Create a full copy of the right stackframe
 if (DeeAtomicMutex_TryAcquire(&right->yi_lock)) {
do_locked:
  Dee_INCREF(self->yi_func = right->yi_func);
  Dee_INCREF(rhs_args = right->yi_args);
  // NOTE: The locking of the frame is done by '_DeeStackFrame_InitCopy'
  error = _DeeStackFrame_InitCopy(&self->yi_frame,&right->yi_frame,
                                  DEE_STACKFRAME_COPY_FLAG_LOCALS_DEEPCOPY|
                                  DEE_STACKFRAME_COPY_FLAG_STACK_DEEPCOPY);
  DeeAtomicMutex_Release(&right->yi_lock);
 } else if (!_DeeThread_HasFrame(DeeThread_SELF(),&self->yi_frame)) {
  DeeAtomicMutex_AcquireRelaxed(&right->yi_lock);
  goto do_locked;
 } else {
  Dee_INCREF(self->yi_func = right->yi_func);
  Dee_INCREF(rhs_args = right->yi_args);
  // NOTE: The locking of the frame is done by '_DeeStackFrame_InitCopy'
  error = _DeeStackFrame_InitCopy(&self->yi_frame,&right->yi_frame,
                                  DEE_STACKFRAME_COPY_FLAG_LOCALS_DEEPCOPY|
                                  DEE_STACKFRAME_COPY_FLAG_STACK_DEEPCOPY);
 }
 if DEE_UNLIKELY(error != 0) {
  Dee_DECREF(rhs_args);
err_yi_func:
  Dee_DECREF(self->yi_func);
  return -1;
 }
 self->yi_args = (DeeTupleObject *)DeeTuple_DeepCopy((DeeObject *)rhs_args);
 Dee_DECREF(rhs_args);
 if DEE_UNLIKELY(!self->yi_args) goto err_yi_func;
 // Make sure the correct version of the arguments tuple is used
 self->yi_frame.f_argv = DeeTuple_ELEM(self->yi_args);
 DeeAtomicMutex_Init(&self->yi_lock);
 // Reminder: yield_function iterators own a reference to their code
 Dee_INCREF(self->yi_frame.f_code);
 return 0;
}
static int _deeyieldfunctioniterator_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeYieldFunctionIteratorObject *self, DeeObject *args) {
 DeeYieldFunctionObject *yfunc;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:yield_function.iterator",&yfunc) != 0) return -1;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((
  DeeObject *)yfunc,&DeeYieldFunction_Type) != 0) return -1;
 return _DeeYieldFunctionIterator_Init(self,yfunc);
}

DEE_VISIT_PROC(_deeyieldfunctioniterator_tp_visit,DeeYieldFunctionIteratorObject *self) {
 if (DeeAtomicMutex_TryAcquire(&self->yi_lock)) {
do_locked:
  Dee_VISIT(self->yi_func);
  Dee_VISIT(self->yi_args);
  _DeeStackFrame_Visit(&self->yi_frame,visit,closure);
  DeeAtomicMutex_Release(&self->yi_lock);
 } else if (!_DeeThread_HasFrame(DeeThread_SELF(),&self->yi_frame)) {
  DeeAtomicMutex_AcquireRelaxed(&self->yi_lock);
  goto do_locked;
 } else {
  Dee_VISIT(self->yi_func);
  Dee_VISIT(self->yi_args);
  _DeeStackFrame_Visit(&self->yi_frame,visit,closure);
 }
}
static DeeObject *_deeyieldfunctioniterator_tp_str(DeeYieldFunctionIteratorObject *self) {
 DeeObject *result; DeeFunctionObject *func; DeeTupleObject *args;
 if (DeeAtomicMutex_TryAcquire(&self->yi_lock)) {
do_locked:
  Dee_INCREF(func = self->yi_func);
  Dee_INCREF(args = self->yi_args);
  DeeAtomicMutex_Release(&self->yi_lock);
 } else if (!_DeeThread_HasFrame(DeeThread_SELF(),&self->yi_frame)) {
  DeeAtomicMutex_AcquireRelaxed(&self->yi_lock);
  goto do_locked;
 } else {
  Dee_INCREF(func = self->yi_func);
  Dee_INCREF(args = self->yi_args);
 }
 result = DeeString_Newf("yield_function.iterator(%k%k)",
                         func,args);
 Dee_DECREF(args);
 Dee_DECREF(func);
 return result;
}
static DeeObject *_deeyieldfunctioniterator_tp_repr(DeeYieldFunctionIteratorObject *self) {
 DeeObject *result; DeeFunctionObject *func; DeeTupleObject *args;
 if (DeeAtomicMutex_TryAcquire(&self->yi_lock)) {
do_locked:
  Dee_INCREF(func = self->yi_func);
  Dee_INCREF(args = self->yi_args);
  DeeAtomicMutex_Release(&self->yi_lock);
 } else if (!_DeeThread_HasFrame(DeeThread_SELF(),&self->yi_frame)) {
  DeeAtomicMutex_AcquireRelaxed(&self->yi_lock);
  goto do_locked;
 } else {
  Dee_INCREF(func = self->yi_func);
  Dee_INCREF(args = self->yi_args);
 }
 result = DeeString_Newf("<yield_function.iterator(%k%k)>",
                         func,args);
 Dee_DECREF(args);
 Dee_DECREF(func);
 return result;
}
static void const *_DeeYieldFunctionIterator_GetRealAddress(DeeYieldFunctionIteratorObject *self) {
 void const *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeYieldFunctionIterator_Check(self));
 if (DeeAtomicMutex_TryAcquire(&self->yi_lock)) {
do_locked:
  DeeAtomicMutex_AcquireRelaxed(&self->yi_frame.f_lock);
  result = (void const *)self->yi_frame.f_addr;
  DeeAtomicMutex_Release(&self->yi_frame.f_lock);
  DeeAtomicMutex_Release(&self->yi_lock);
 } else if (!_DeeThread_HasFrame(DeeThread_SELF(),&self->yi_frame)) {
  DeeAtomicMutex_AcquireRelaxed(&self->yi_lock);
  goto do_locked;
 } else {
  DeeAtomicMutex_AcquireRelaxed(&self->yi_frame.f_lock);
  result = (void const *)self->yi_frame.f_addr;
  DeeAtomicMutex_Release(&self->yi_frame.f_lock);
 }
 return result;
}

static DeeObject *_deeyieldfunctioniterator_tp_cmp_lo(DeeYieldFunctionIteratorObject *self, DeeYieldFunctionIteratorObject *right) { if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)right,&DeeYieldFunctionIterator_Type) != 0) return NULL; DeeReturn_Bool(_DeeYieldFunctionIterator_GetRealAddress(self) <  _DeeYieldFunctionIterator_GetRealAddress(right)); }
static DeeObject *_deeyieldfunctioniterator_tp_cmp_le(DeeYieldFunctionIteratorObject *self, DeeYieldFunctionIteratorObject *right) { if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)right,&DeeYieldFunctionIterator_Type) != 0) return NULL; DeeReturn_Bool(_DeeYieldFunctionIterator_GetRealAddress(self) <= _DeeYieldFunctionIterator_GetRealAddress(right)); }
static DeeObject *_deeyieldfunctioniterator_tp_cmp_eq(DeeYieldFunctionIteratorObject *self, DeeYieldFunctionIteratorObject *right) { if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)right,&DeeYieldFunctionIterator_Type) != 0) return NULL; DeeReturn_Bool(_DeeYieldFunctionIterator_GetRealAddress(self) == _DeeYieldFunctionIterator_GetRealAddress(right)); }
static DeeObject *_deeyieldfunctioniterator_tp_cmp_ne(DeeYieldFunctionIteratorObject *self, DeeYieldFunctionIteratorObject *right) { if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)right,&DeeYieldFunctionIterator_Type) != 0) return NULL; DeeReturn_Bool(_DeeYieldFunctionIterator_GetRealAddress(self) != _DeeYieldFunctionIterator_GetRealAddress(right)); }
static DeeObject *_deeyieldfunctioniterator_tp_cmp_gr(DeeYieldFunctionIteratorObject *self, DeeYieldFunctionIteratorObject *right) { if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)right,&DeeYieldFunctionIterator_Type) != 0) return NULL; DeeReturn_Bool(_DeeYieldFunctionIterator_GetRealAddress(self) >  _DeeYieldFunctionIterator_GetRealAddress(right)); }
static DeeObject *_deeyieldfunctioniterator_tp_cmp_ge(DeeYieldFunctionIteratorObject *self, DeeYieldFunctionIteratorObject *right) { if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)right,&DeeYieldFunctionIterator_Type) != 0) return NULL; DeeReturn_Bool(_DeeYieldFunctionIterator_GetRealAddress(self) >= _DeeYieldFunctionIterator_GetRealAddress(right)); }

static int DEE_CALL _deeyieldfunctioniterator_tp_seq_iter_next(
 DeeYieldFunctionIteratorObject *self, DeeObject **result) {
 struct DeeStackFrame *frame_; DeeThreadObject *thread_self; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeYieldFunctionIterator_Check(self));
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL) return 1; // Iterator end? (not really...)
 frame_ = &self->yi_frame;
 // Try to acquire a lock to the iterator
 if (!DeeAtomicMutex_TryAcquire(&self->yi_lock)) {
  if DEE_UNLIKELY(frame_->f_prev || thread_self->t_frame_last == frame_) {
   // The frame is already being executed... (in this thread, and trying to call itself)
   DeeError_SetStringf(&DeeErrorType_RuntimeError,
                       "yield_function.iterator %k is already being executed",
                       self);
   return -1;
  }
  // Another thread is currently using this iterator (wait for that thread)
  DeeAtomicMutex_Acquire(&self->yi_lock);
 }
 // Run the stack frame
 DeeAtomicMutex_AcquireRelaxed(&frame_->f_lock);
 error = _DeeStackFrame_RunLocked_AndUnlock(thread_self,frame_);
 *result = frame_->f_result; // inherit/transfer reference
 frame_->f_result = NULL;
 DeeAtomicMutex_Release(&self->yi_lock);
 if DEE_UNLIKELY(error != 0) { DEE_ASSERT(error < 0); return error; }
 if DEE_UNLIKELY(!*result) return 1; // End of iterator
 return 0;
}












static void _deefunction_tp_dtor(DeeFunctionObject *self) {
 Dee_DECREF(self->fn_code);
 Dee_DECREF(self->fn_refs);
}
static void _deefunction_tp_clear(DeeFunctionObject *self) {
 DeeCodeObject *old_code;
 DeeTupleObject *old_refs;
 DeeFunction_ACQUIRE(self);
 old_code = self->fn_code; Dee_INCREF(self->fn_code = (DeeCodeObject *)Dee_EmptyCode);
 old_refs = self->fn_refs; Dee_INCREF(self->fn_refs = (DeeTupleObject *)Dee_EmptyTuple);
 DeeFunction_RELEASE(self);
 Dee_DECREF(old_code);
 Dee_DECREF(old_refs);
}
static int _deefunction_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFunctionObject *self, DeeObject *args) {
 self->fn_code = (DeeCodeObject *)Dee_EmptyCode;
 self->fn_refs = (DeeTupleObject *)Dee_EmptyTuple;
 self->fn_argc = 0;
 self->fn_flags = DEE_FUNCTION_FLAG_NONE;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|ooIuI32u:function",
  &self->fn_code,&self->fn_refs,&self->fn_argc,&self->fn_flags) != 0) return -1;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)self->fn_refs,&DeeTuple_Type) != 0 ||
                 DeeError_TypeError_CheckTypeExact((DeeObject *)self->fn_code,&DeeCode_Type) != 0) return -1;
 DeeAtomicMutex_Init(&self->fn_lock);
 Dee_INCREF(self->fn_code);
 Dee_INCREF(self->fn_refs);
 return 0;
}
static int _deefunction_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFunctionObject *self, DeeFunctionObject *right) {
 DeeFunction_ACQUIRE(right);
 Dee_INCREF(self->fn_code = right->fn_code);
 Dee_INCREF(self->fn_refs = right->fn_refs);
 DeeFunction_RELEASE(right);
 DeeAtomicMutex_Init(&self->fn_lock);
 self->fn_argc = right->fn_argc;
 self->fn_flags = right->fn_flags;
 return 0;
}
DEE_VISIT_PROC(_deefunction_tp_visit,DeeFunctionObject *self) {
 Dee_VISIT(self->fn_code);
 Dee_VISIT(self->fn_refs);
}

static DeeObject *_deefunctionclass_compile(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *file_or_code,*disp_filename = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:compile",&file_or_code,&disp_filename) != 0) return NULL;
 if DEE_UNLIKELY(disp_filename && DeeError_TypeError_CheckTypeExact(disp_filename,&DeeString_Type) != 0) return NULL;
 if (DeeFile_Check(file_or_code)) return disp_filename
   ? DeeCompile_FileStreamEx(file_or_code,DeeString_STR(disp_filename),NULL)
   : DeeCompile_FileStream(file_or_code,NULL);
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(file_or_code,&DeeString_Type) != 0) return NULL;
 return disp_filename
  ? DeeCompile_StringObjectEx(file_or_code,DeeString_STR(disp_filename),NULL)
  : DeeCompile_StringObject(file_or_code,NULL);
}
static DeeObject *_deefunction_tp_str(DeeFunctionObject *self) {
 DeeObject *result,*code;
 code = DeeFunction_Code((DeeObject *)self);
 if DEE_LIKELY((result = DeeCode_NAME(code)) != NULL) {
  Dee_INCREF(result);
  Dee_DECREF(code);
  return result;
 }
 Dee_DECREF(code);
 DeeReturn_STATIC_STRING("__anonymous__");
}
static DeeObject *_deefunction_tp_repr(DeeFunctionObject *self) {
 return DeeString_Newf("<function %K>",_deefunction_tp_str(self));
}
static int _deefunction_tp_copy_assign(
 DeeFunctionObject *self, DeeFunctionObject *right) {
 DeeFunction_ACQUIRE(self);
 if DEE_UNLIKELY(self->fn_code != (DeeCodeObject *)Dee_EmptyCode ||
                 self->fn_refs != (DeeTupleObject *)Dee_EmptyTuple) {
  DeeFunction_RELEASE(self);
  DeeError_SET_STRING(&DeeErrorType_ValueError,
                      "Can only assign to forward-declared functions (aka. without a body)");
  return -1;
 }
 Dee_INCREF(self->fn_code = right->fn_code);
 Dee_INCREF(self->fn_refs = right->fn_refs);
 self->fn_argc = right->fn_argc;
 self->fn_flags = right->fn_flags;
 DeeFunction_RELEASE(self);
 Dee_DECREF(Dee_EmptyCode);
 Dee_DECREF(Dee_EmptyTuple);
 return 0;
}

static DeeObject *_deefunction_code_get(
 DeeFunctionObject *self, void *DEE_UNUSED(closure)) {
 return DeeFunction_Code((DeeObject *)self);
}
static DeeObject *_deefunction_name_get(
 DeeFunctionObject *self, void *DEE_UNUSED(closure)) {
 DeeObject *result,*code;
 code = DeeFunction_Code((DeeObject *)self);
 if DEE_LIKELY((result = DeeCode_NAME(code)) != NULL) {
  Dee_INCREF(result);
  Dee_DECREF(code);
  return result;
 }
 Dee_DECREF(code);
 DeeReturn_EmptyString;
}
static DeeObject *_deefunction_asmrepr(
 DeeFunctionObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*code;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":asmrepr") != 0) return NULL;
 code = DeeFunction_Code((DeeObject *)self);
 result = DeeCode_AsmRepr(code);
 Dee_DECREF(code);
 return result;
}
static DeeObject *_deeyieldfunction_asmrepr(
 DeeYieldFunctionObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*code;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":asmrepr") != 0) return NULL;
 code = DeeFunction_Code((DeeObject *)self->yf_func);
 result = DeeCode_AsmRepr(code);
 Dee_DECREF(code);
 return result;
}
static DeeObject *_deeyieldfunctioniterator_asmrepr(
 DeeYieldFunctionIteratorObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*code;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":asmrepr") != 0) return NULL;
 code = DeeFunction_Code((DeeObject *)self->yi_func);
 result = DeeCode_AsmRepr(code);
 Dee_DECREF(code);
 return result;
}
static DeeObject *_deeyieldfunctioniterator_file_get(
 DeeYieldFunctionIteratorObject *self, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 DeeAtomicMutex_Acquire(&self->yi_frame.f_lock);
 Dee_XINCREF(result = _DeeStackFrame_File(&self->yi_frame));
 DeeAtomicMutex_Release(&self->yi_frame.f_lock);
 if DEE_LIKELY(result) return result;
 DeeReturn_EmptyString;
}
static DeeObject *_deeyieldfunctioniterator_line_get(
 DeeYieldFunctionIteratorObject *self, void *DEE_UNUSED(closure)) {
 Dee_int32_t result;
 DeeAtomicMutex_Acquire(&self->yi_frame.f_lock);
 result = _DeeStackFrame_Line(&self->yi_frame);
 DeeAtomicMutex_Release(&self->yi_frame.f_lock);
 return DeeObject_New(Dee_int32_t,result);
}
static DeeObject *_deeyieldfunctioniterator_func_get(
 DeeYieldFunctionIteratorObject *self, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 DeeAtomicMutex_Acquire(&self->yi_frame.f_lock);
 Dee_XINCREF(result = _DeeStackFrame_Func(&self->yi_frame));
 DeeAtomicMutex_Release(&self->yi_frame.f_lock);
 if DEE_LIKELY(result) return result;
 DeeReturn_EmptyString;
}
static struct DeeMethodDef const _deefunction_tp_methods[] = {
 DEE_METHODDEF_v100("asmrepr",member(&_deefunction_asmrepr),DEE_DOC_AUTO),
 DEE_METHODDEF_END_v100
};
static struct DeeMethodDef const _deeyieldfunction_tp_methods[] = {
 DEE_METHODDEF_CONST_v100("asmrepr",member(&_deeyieldfunction_asmrepr),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("at",member(&_deegenericiterable_at),NULL),
 DEE_METHODDEF_CONST_v100("back",member(&_deegenericiterable_back),NULL),
 DEE_METHODDEF_CONST_v100("contains",member(&_deegenericiterable_contains),NULL),
 DEE_METHODDEF_CONST_v100("count",member(&_deegenericiterable_count),NULL),
 DEE_METHODDEF_CONST_v100("empty",member(&_deegenericiterable_empty),NULL),
 DEE_METHODDEF_CONST_v100("find",member(&_deegenericiterable_find),NULL),
 DEE_METHODDEF_CONST_v100("front",member(&_deegenericiterable_front),NULL),
 DEE_METHODDEF_CONST_v100("index",member(&_deegenericiterable_index),NULL),
 DEE_METHODDEF_CONST_v100("locate",member(&_deegenericiterable_locate),NULL),
 DEE_METHODDEF_CONST_v100("locate_all",member(&_deegenericiterable_locate_all),NULL),
 DEE_METHODDEF_CONST_v100("non_empty",member(&_deegenericiterable_non_empty),NULL),
 DEE_METHODDEF_CONST_v100("reversed",member(&_deegenericiterable_reversed),NULL),
 DEE_METHODDEF_CONST_v100("rfind",member(&_deegenericiterable_rfind),NULL),
 DEE_METHODDEF_CONST_v100("rindex",member(&_deegenericiterable_rindex),NULL),
 DEE_METHODDEF_CONST_v100("rlocate",member(&_deegenericiterable_rlocate),NULL),
 DEE_METHODDEF_CONST_v100("segments",member(&_deegenericiterable_segments),NULL),
 DEE_METHODDEF_CONST_v100("sorted",member(&_deegenericiterable_sorted),NULL),
 DEE_METHODDEF_CONST_v100("transform",member(&_deegenericiterable_transform),NULL),
 DEE_METHODDEF_END_v100
};
static struct DeeMethodDef const _deeyieldfunctioniterator_tp_methods[] = {
 DEE_METHODDEF_v100("asmrepr",member(&_deeyieldfunctioniterator_asmrepr),DEE_DOC_AUTO),
 DEE_METHODDEF_END_v100
};
static struct DeeGetSetDef const _deeyieldfunctioniterator_tp_getsets[] = {
 DEE_GETSETDEF_v100("file",member(&_deeyieldfunctioniterator_file_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("line",member(&_deeyieldfunctioniterator_line_get),null,null,"-> int32_t"),
 DEE_GETSETDEF_v100("func",member(&_deeyieldfunctioniterator_func_get),null,null,"-> string"),
 DEE_GETSETDEF_END_v100
};


#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deefunction_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deefunction_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__fn_code",DeeFunctionObject,fn_code,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__fn_refs",DeeFunctionObject,fn_refs,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__fn_lock",DeeFunctionObject,fn_lock,DeeAtomicMutex),
 DEE_MEMBERDEF_NAMED_RO_v100("__fn_flags",DeeFunctionObject,fn_flags,Dee_uint32_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__fn_argc",DeeFunctionObject,fn_argc,Dee_size_t),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeGetSetDef const _deefunction_tp_getsets[] = {
 DEE_GETSETDEF_v100("code",member(&_deefunction_code_get),null,null,"-> code\n"
  "\tThe code of the function."),
 DEE_GETSETDEF_v100("name",member(&_deefunction_name_get),null,null,"-> string\n"
  "\tName of the function, or an empty string if unknown."),
 DEE_GETSETDEF_END_v100
};
#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deeyieldfunction_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deeyieldfunction_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__yf_func",DeeYieldFunctionObject,yf_func,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__yf_args",DeeYieldFunctionObject,yf_args,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__yf_this",DeeYieldFunctionObject,yf_this,object_null),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMethodDef const _deefunction_tp_class_methods[] = {
 DEE_METHODDEF_CONST_v100("compile",member(&_deefunctionclass_compile),
  "(string code) -> function\n"
  "(file code) -> function\n"
  "(string code, string disp_filename) -> function\n"
  "(file code, string disp_filename) -> function\n"
  "\tCompiles a function from a given string or file with an optional filename used in tracebacks and for __FILE__"),
 DEE_METHODDEF_END_v100
};
static struct DeeMemberDef const _deefunction_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("FLAG_NONE",Dee_uint32_t,DEE_FUNCTION_FLAG_NONE),
 DEE_MEMBERDEF_CONST_v100("FLAG_VARG",Dee_uint32_t,DEE_FUNCTION_FLAG_VARG),
 DEE_MEMBERDEF_CONST_v100("FLAG_YILD",Dee_uint32_t,DEE_FUNCTION_FLAG_YILD),
 DEE_MEMBERDEF_CONST_v100("FLAG_THIS",Dee_uint32_t,DEE_FUNCTION_FLAG_THIS),
 DEE_MEMBERDEF_CONST_v100("yield",object,&DeeYieldFunction_Type),
 DEE_MEMBERDEF_CONST_v100("code",object,&DeeCode_Type),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const _deeyieldfunction_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeYieldFunctionIterator_Type),
 DEE_MEMBERDEF_END_v100
};

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deeyieldfunctioniterator_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deeyieldfunctioniterator_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_func",DeeYieldFunctionIteratorObject,yi_func,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_args",DeeYieldFunctionIteratorObject,yi_args,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_prev",DeeYieldFunctionIteratorObject,yi_frame.f_prev,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_code",DeeYieldFunctionIteratorObject,yi_frame.f_code,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_argv",DeeYieldFunctionIteratorObject,yi_frame.f_code,p2(void)),
#ifdef DEE_DEBUG
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_argc",DeeYieldFunctionIteratorObject,yi_frame.f_argc,Dee_size_t),
#endif
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_lock",DeeYieldFunctionIteratorObject,yi_frame.f_lock,DeeAtomicMutex),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_padding",DeeYieldFunctionIteratorObject,yi_frame.f_padding,Dee_uint8_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_flags",DeeYieldFunctionIteratorObject,yi_frame.f_flags,Dee_uint16_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_addr",DeeYieldFunctionIteratorObject,yi_frame.f_addr,p(Dee_uint8_t)),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_constv",DeeYieldFunctionIteratorObject,yi_frame.f_constv,p2(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_refv",DeeYieldFunctionIteratorObject,yi_frame.f_refv,p2(void)),
#ifdef DEE_DEBUG
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_refc",DeeYieldFunctionIteratorObject,yi_frame.f_refc,Dee_size_t),
#endif
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_localv",DeeYieldFunctionIteratorObject,yi_frame.f_localv,p2(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_stackv",DeeYieldFunctionIteratorObject,yi_frame.f_stackv,p2(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_wbuf",DeeYieldFunctionIteratorObject,yi_frame.f_wbuf,p(char)),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_stackv_end",DeeYieldFunctionIteratorObject,yi_frame.f_stackv_end,p2(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_this",DeeYieldFunctionIteratorObject,yi_frame.f_this,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_result",DeeYieldFunctionIteratorObject,yi_frame.f_this,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_file",DeeYieldFunctionIteratorObject,yi_frame.f_this,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_stored_exceptions",DeeYieldFunctionIteratorObject,yi_frame.f_stored_exceptions,p(void)),
#if DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_exceptv",DeeYieldFunctionIteratorObject,yi_frame.f_exceptv,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_exceptv_end",DeeYieldFunctionIteratorObject,yi_frame.f_exceptv_end,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_frame_f_finally_return_address_end",DeeYieldFunctionIteratorObject,yi_frame.f_finally_return_address_end,p2(Dee_uint8_t)),
#endif
#ifndef DEE_WITHOUT_THREADS
 DEE_MEMBERDEF_NAMED_RO_v100("__yi_lock",DeeYieldFunctionIteratorObject,yi_lock,DeeAtomicMutex),
#endif
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */

static int DEE_CALL _deefunction_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFunctionObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_uint64_t read_argc;
 if (DeeFile_GetLe(infile,self->fn_flags) != 0) return -1;
 if ((self->fn_flags&~(DEE_FUNCTION_FLAG_MASK))!=0) {
  // Unknown/Unsupported function flags
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Function contains flags unsupported by this version of deemon: "
                      "0x%.8I32x",(Dee_uint32_t)(self->fn_flags&~(DEE_FUNCTION_FLAG_MASK)));
  return -1;
 }
 // NOTE: Since we ensure that only known flags are parsed, we can re-use the
 //       flag for serializing additional future features, such as function defaults.
 if (DeeFile_GetLeSmall64(infile,&read_argc) != 0) return -1;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_argc > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Function argument count too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_argc,(Dee_size_t)-1);
  return -1;
 }
#endif
 self->fn_argc = (Dee_size_t)read_argc;
 if ((self->fn_code = (DeeCodeObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if (DeeNone_Check(self->fn_code)) { Dee_DECREF(self->fn_code); Dee_INCREF(self->fn_code = (DeeCodeObject *)Dee_EmptyCode); }
 else if (DeeError_TypeError_CheckTypeExact((DeeObject *)self->fn_code,&DeeCode_Type) != 0) {err_code: Dee_DECREF(self->fn_code); return -1; }
 if ((self->fn_refs = (DeeTupleObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) goto err_code;
 if (DeeNone_Check(self->fn_refs)) { Dee_DECREF(self->fn_refs); Dee_INCREF(self->fn_refs = (DeeTupleObject *)Dee_EmptyTuple); }
 else if (DeeError_TypeError_CheckTypeExact((DeeObject *)self->fn_refs,&DeeTuple_Type) != 0) {/*err_refs:*/ Dee_DECREF(self->fn_refs); goto err_code; }
 DeeAtomicMutex_Init(&self->fn_lock);
 return 0;
}

static int DEE_CALL _deefunction_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFunctionObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 DeeCodeObject *fun_code; DeeTupleObject *fun_refs;
 Dee_size_t fun_argc; Dee_uint32_t fun_flags;
 DeeFunction_ACQUIRE(self);
 Dee_INCREF(fun_code = self->fn_code);
 Dee_INCREF(fun_refs = self->fn_refs);
 fun_argc = self->fn_argc;
 fun_flags = self->fn_flags;
 DeeFunction_RELEASE(self);
 DEE_ASSERTF((fun_flags&~(DEE_FUNCTION_FLAG_MASK)) == 0,
             "Tried to serialize function %r with unrecognized flags",self);
 // NOTE: Function defaults will get a special flag that's
 //       going to indicate the presence of a defaults-tuple.
 if (DeeFile_PutLe(outfile,fun_flags) != 0) goto err;
 if (DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)fun_argc) != 0) goto err;
 if (fun_code == (DeeCodeObject *)Dee_EmptyCode) { Dee_DECREF(fun_code); fun_code = (DeeCodeObject *)DeeNone_New(); }
 if (DeeTuple_CheckEmpty(fun_refs)) { Dee_DECREF(fun_refs); fun_refs = (DeeTupleObject *)DeeNone_New(); }
 if (DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)fun_code) != 0) goto err;
 if (DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)fun_refs) != 0) goto err;
 Dee_DECREF(fun_refs);
 Dee_DECREF(fun_code);
 return 0;
err: Dee_DECREF(fun_refs);
 Dee_DECREF(fun_code);
 return -1;
}

static struct DeeTypeMarshal _deefunction_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(function)),
 member(&_deefunction_tp_marshal_ctor),
 member(&_deefunction_tp_marshal_put));


DeeTypeObject DeeFunction_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("function"),member(
  "(code c, tuple refs = pack(), size_t argc = 0, uint32_t flags = function.FLAG_NONE)\n"
  "\tCreates a new function object call calls 'code'. Note that 'argc' must be corrent\n"
  "\tand that an incorrect argc-value can cause access violations even when deemon is compiled in debug mode"),
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_HAS_GC|DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(
  sizeof(DeeFunctionObject),member(&_DeeGC_TpAlloc),null,
  member(&_deefunction_tp_copy_ctor),null,
  member(&_deefunction_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(member(&_DeeGC_TpFree),
  member(&_deefunction_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deefunction_tp_copy_assign),null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deefunction_tp_str),
  member(&_deefunction_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(
  member(&DeeFunction_Call),
  member(&_deefunction_tp_visit),
  member(&_deefunction_tp_clear)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deefunction_tp_members),member(_deefunction_tp_getsets),member(_deefunction_tp_methods),
  member(_deefunction_tp_class_members),null,member(_deefunction_tp_class_methods)),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deefunction_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};

static int DEE_CALL _deeyieldfunction_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeYieldFunctionObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 if ((self->yf_func = (DeeFunctionObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if (DeeError_TypeError_CheckTypeExact((DeeObject *)self->yf_func,&DeeFunction_Type) != 0) {err_func: Dee_DECREF(self->yf_func); return -1; }
 if ((self->yf_args = (DeeTupleObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) goto err_func;
 if (DeeNone_Check(self->yf_args)) { Dee_DECREF(self->yf_args); self->yf_args = (DeeTupleObject *)DeeNone_New(); }
 else if (DeeError_TypeError_CheckTypeExact((DeeObject *)self->yf_args,&DeeTuple_Type) != 0) {err_args: Dee_DECREF(self->yf_args); goto err_func; }
 if ((self->yf_func->fn_flags&DEE_FUNCTION_FLAG_THIS)!=0) {
  if ((self->yf_this = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) goto err_args;
 } else self->yf_this = NULL;
 return 0;
}
static int DEE_CALL _deeyieldfunction_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeYieldFunctionObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 if (DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)self->yf_func) != 0) return -1;
 if (DeeMarshal_WriteObjectWithMap(outfile,map,DeeTuple_CheckEmpty(self->yf_args) ? Dee_None : (DeeObject *)self->yf_args) != 0) return -1;
 if ((self->yf_func->fn_flags&DEE_FUNCTION_FLAG_THIS)!=0) {
  DEE_ASSERT(self->yf_this);
  if (DeeMarshal_WriteObjectWithMap(outfile,map,self->yf_this) != 0) return -1;
 } else DEE_ASSERT(!self->yf_this);
 return 0;
}

static struct DeeTypeMarshal _deeyieldfunction_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(yield_function)),
 member(&_deeyieldfunction_tp_marshal_ctor),
 member(&_deeyieldfunction_tp_marshal_put));


DeeTypeObject DeeYieldFunction_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("yield_function"),member(
  "(function func, tuple args)\n"
  "(function func, tuple args, object this)\n"
  "@param func: The given function\n"
  "@param args: Arguments used to call the given function\n"
  "@param this: The this argument used during [[thiscall]] calls\n"
  "\tCreates a new yield_function from a given function and arguments.\n"
  "\tNOTE: Using this constructor has the same effect as calling 'func.operator ()(args)'"),
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeYieldFunctionObject),null,null,
  member(&_deeyieldfunction_tp_copy_ctor),null,
  member(&_deeyieldfunction_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deeyieldfunction_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deeyieldfunction_tp_str),
  member(&_deeyieldfunction_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deeyieldfunction_tp_visit),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deegenericiterable_tp_cmp_lo),
  member(&_deegenericiterable_tp_cmp_le),
  member(&_deegenericiterable_tp_cmp_eq),
  member(&_deegenericiterable_tp_cmp_ne),
  member(&_deegenericiterable_tp_cmp_gr),
  member(&_deegenericiterable_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&_deegenericiterable_tp_seq_get),null,null,
  member(&_deegenericiterable_tp_seq_size),
  member(&_deegenericiterable_tp_seq_contains),null,null,null,
  member(&_deeyieldfunction_tp_seq_iter_self),null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deeyieldfunction_tp_members),null,member(_deeyieldfunction_tp_methods),
  member(_deeyieldfunction_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deeyieldfunction_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};


static int DEE_CALL _deeyieldfunctioniterator_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeYieldFunctionIteratorObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 if ((self->yi_func = (DeeFunctionObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if (DeeError_TypeError_CheckTypeExact((DeeObject *)self->yi_func,&DeeFunction_Type) != 0) {err_func: Dee_DECREF(self->yi_func); return -1; }
 if ((self->yi_args = (DeeTupleObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) goto err_func;
 if (DeeNone_Check(self->yi_args)) { Dee_DECREF(self->yi_args); Dee_INCREF(self->yi_args = (DeeTupleObject *)Dee_EmptyTuple); }
 else if (DeeError_TypeError_CheckTypeExact((DeeObject *)self->yi_args,&DeeTuple_Type) != 0) {err_args: Dee_DECREF(self->yi_args); goto err_func; }
 if (_DeeStackFrame_InitFromMarshal(&self->yi_frame,self->yi_func->fn_code,
#ifdef DEE_DEBUG
  self->yi_func->fn_argc,DeeTuple_ELEM(self->yi_args),
  DeeTuple_SIZE(self->yi_func->fn_refs),DeeTuple_ELEM(self->yi_func->fn_refs),
#else
  DeeTuple_ELEM(self->yi_args),DeeTuple_ELEM(self->yi_func->fn_refs),
#endif
  infile,map) != 0) goto err_args;
 return 0;
}
static int DEE_CALL _deeyieldfunctioniterator_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeYieldFunctionIteratorObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 DEE_ASSERT(DeeObject_Check(self->yi_frame.f_code) && DeeCode_Check(self->yi_frame.f_code));
 if ((self->yi_frame.f_code->co_flags&DEE_CODE_FLAG_COPY) == 0) {
  // Yield-function iterators not marked with [[copyable]] can't be serialized either!
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Can't serialize code %k stackframe not marked with [[copyable]]",
                      self->yi_frame.f_code);
  return -1;
 }
 if (DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)self->yi_func) != 0) return -1;
 if (DeeMarshal_WriteObjectWithMap(outfile,map,DeeTuple_CheckEmpty(self->yi_args) ? Dee_None : (DeeObject *)self->yi_args) != 0) return -1;
 return _DeeStackFrame_PutToMarshal(&self->yi_frame,outfile,map);
}

static struct DeeTypeMarshal _deeyieldfunctioniterator_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(yield_function_iterator)),
 member(&_deeyieldfunctioniterator_tp_marshal_ctor),
 member(&_deeyieldfunctioniterator_tp_marshal_put));

DeeTypeObject DeeYieldFunctionIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("yield_function.iterator"),member(
  "(yield_function func)\n"
  "\tCreates a new yield_function iterator from a given yield_function.\n"
  "\tUsing this constructor has the same effect as calling 'func.operator __iterself__()'"
  "\tNOTE: Copying a yield_function iterator requires the given iterator to not have been used yet, or the code to be marked with [[copyable]]"),
  member(DEE_TYPE_FLAG_NO_SUBCLASS|DEE_TYPE_FLAG_ALWAYS_CLEAR|DEE_TYPE_FLAG_HAS_GC),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeYieldFunctionIteratorObject),member(&_DeeGC_TpAlloc),
  null,member(&_deeyieldfunctioniterator_tp_copy_ctor),
  null,member(&_deeyieldfunctioniterator_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(member(&_DeeGC_TpFree),
  member(&_deeyieldfunctioniterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deeyieldfunctioniterator_tp_str),
  member(&_deeyieldfunctioniterator_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deeyieldfunctioniterator_tp_visit),
  member(&_deeyieldfunctioniterator_tp_clear)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deeyieldfunctioniterator_tp_cmp_lo),
  member(&_deeyieldfunctioniterator_tp_cmp_le),
  member(&_deeyieldfunctioniterator_tp_cmp_eq),
  member(&_deeyieldfunctioniterator_tp_cmp_ne),
  member(&_deeyieldfunctioniterator_tp_cmp_gr),
  member(&_deeyieldfunctioniterator_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,
  null,member(&_deeyieldfunctioniterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deeyieldfunctioniterator_tp_members),
  member(_deeyieldfunctioniterator_tp_getsets),
  member(_deeyieldfunctioniterator_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deeyieldfunctioniterator_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};

DEE_DECL_END

#endif /* !GUARD_DEEMON_FUNCTION_C */
