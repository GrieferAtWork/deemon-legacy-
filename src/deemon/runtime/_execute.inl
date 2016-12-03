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
#ifdef __INTELLISENSE__
#define DEE_LIMITED_API 1
#include "execute.c"

//#define SANDBOX
//#define YIELD_FUNCTION

#ifdef SANDBOX
#include <deemon/runtime/sandbox.h>
#endif

DEE_DECL_BEGIN
static int _intellisense_rt_execute(
 DeeThreadObject *thread_self)
#endif

#ifdef DEE_DEBUG
#define STACK_DEBUG
#endif

#ifdef SANDBOX
#define DeeSand_Violation(msg)  DeeError_SET_STRING(&DeeErrorType_IllegalInstruction,msg)
#define DeeSand_Violationf(...) DeeError_SetStringf(&DeeErrorType_IllegalInstruction,__VA_ARGS__)
#endif

{
#ifdef __INTELLISENSE__
#ifdef SANDBOX
 struct DeeSandbox const *sbx;
#endif
 struct DeeStackFrame frame;
#endif
#if DEE_XCONFIG_RUNTIME_USE_COMPUTED_GOTOS
#if defined(_MSC_VER)
#undef DEE_XCONFIG_RUNTIME_USE_COMPUTED_GOTOS
#define DEE_XCONFIG_RUNTIME_USE_COMPUTED_GOTOS 0
#endif
#endif

 static char const _deert_unexpected_unpack_size_msg[] =
  "Unpack: unexpected sequence size (Expected: %I16u, Got: %Iu)";
#define _deert_unexpected_unpack_size(wanted,got) \
 DeeError_RuntimeErrorf(_deert_unexpected_unpack_size_msg,wanted,got);
#ifdef __INTELLISENSE__
#ifdef YIELD_FUNCTION
 int result;
#endif
#endif
#define code       frame.f_addr
#ifdef OPTIMIZE_STACKEND
 DeeObject **stack_end;
#else
#define stack_end  frame.f_stackv_end
#endif
#define stack      frame.f_stackv
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 struct DeeExceptionHandlerEntry *handler;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
#if DEE_CONFIG_RUNTIME_HAVE_BUILTIN_ALLOCA && !DEE_HAVE_ALLOCA
 Dee_size_t alloca_c = 0;
 void **alloca_v = NULL;
#endif

#ifdef OPTIMIZE_STACKEND
 stack_end = frame.f_stackv_end;
#endif
#ifdef YIELD_FUNCTION
 result = 0;
#endif

#define ERROR_OCCURRED (thread_self->t_exception!=NULL)

 // Only need to lock the frame lock for writing
 frame.f_prev = thread_self->t_frame_last;
 DeeAtomicMutex_Release(&frame.f_lock);
 DeeAtomicMutex_Acquire(&thread_self->t_frame_lock);
#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
 if DEE_UNLIKELY(thread_self->t_frame_size >= Dee_GetStackLimit()) {
  // Stack overflow
  DeeAtomicMutex_Release(&thread_self->t_frame_lock);
  DeeAtomicMutex_AcquireRelaxed(&frame.f_lock);
#if defined(YIELD_FUNCTION) || defined(DEE_DEBUG)
  frame.f_prev = NULL;
#endif
#ifdef YIELD_FUNCTION
  Dee_XCLEAR(frame.f_result);
#endif
  DeeAtomicMutex_Release(&frame.f_lock);
#ifdef YIELD_FUNCTION
  result = -1;
#endif
  DeeError_Throw(DeeErrorInstance_StackOverflow);
  goto end_always_2;
 }
 ++thread_self->t_frame_size;
#endif /* DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS */
 thread_self->t_frame_last = &frame;
 DeeAtomicMutex_Release(&thread_self->t_frame_lock);
 DeeAtomicMutex_AcquireRelaxed(&frame.f_lock);
 
 if (thread_self->t_exception) {
#ifdef YIELD_FUNCTION
  // Yield functions may already have some stored exceptions
  if (frame.f_stored_exceptions) {
   struct DeeRaisedException *iter;
   iter = frame.f_stored_exceptions;
   while (iter->re_prev) iter = iter->re_prev;
   // Prepend current exceptions to the list of stored exceptions
   iter->re_prev = thread_self->t_exception;
  } else
#endif
  {
   frame.f_stored_exceptions = thread_self->t_exception;
  }
  DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
  thread_self->t_exception = NULL;
  DeeAtomicMutex_Release(&thread_self->t_exception_lock);
 }

#ifdef DEE_WITHOUT_THREADS
#define EXTERN_BEGIN (void)0
#define EXTERN_END   (void)0
#else
#define EXTERN_BEGIN DeeAtomicMutex_Release(&frame.f_lock)
#define EXTERN_END   DeeAtomicMutex_AcquireRelaxed(&frame.f_lock)
#endif

#ifdef DEE_DEBUG
#define RT_DEBUG_CHECK_VALID_ARG(id)\
 DEE_ASSERTF((id) < frame.f_argc,\
             "%O(%I32d) : %O : +%.4Ix : Invalid argument id: %I16u",\
             _DeeStackFrame_File(&frame),\
             _DeeStackFrame_Line(&frame)+1,\
             _DeeStackFrame_Func(&frame),\
             (Dee_size_t)((code-1)-frame.f_code->co_code),(id))
#define RT_DEBUG_CHECK_VALID_ARG32(id)\
 DEE_ASSERTF((id) < frame.f_argc,\
             "%O(%I32d) : %O : +%.4Ix : Invalid argument id: %I32u",\
             _DeeStackFrame_File(&frame),\
             _DeeStackFrame_Line(&frame)+1,\
             _DeeStackFrame_Func(&frame),\
             (Dee_size_t)((code-1)-frame.f_code->co_code),(id))
#define RT_DEBUG_CHECK_VALID_REF(id)\
 DEE_ASSERTF((id) < frame.f_refc,\
             "%O(%I32d) : %O : +%.4Ix : Invalid reference id: %I16u",\
             _DeeStackFrame_File(&frame),\
             _DeeStackFrame_Line(&frame)+1,\
             _DeeStackFrame_Func(&frame),\
             (Dee_size_t)((code-1)-frame.f_code->co_code),(id))
#define RT_DEBUG_CHECK_VALID_REF32(id)\
 DEE_ASSERTF((id) < frame.f_refc,\
             "%O(%I32d) : %O : +%.4Ix : Invalid reference id: %I32u",\
             _DeeStackFrame_File(&frame),\
             _DeeStackFrame_Line(&frame)+1,\
             _DeeStackFrame_Func(&frame),\
             (Dee_size_t)((code-1)-frame.f_code->co_code),(id))
#else
#define RT_DEBUG_CHECK_VALID_ARG(id)   (void)0
#define RT_DEBUG_CHECK_VALID_ARG32(id) (void)0
#define RT_DEBUG_CHECK_VALID_REF(id)   (void)0
#define RT_DEBUG_CHECK_VALID_REF32(id) (void)0
#endif

#define RT_DEBUG_CHECK_VALID_LOCAL(id)\
 DEE_ASSERTF((id) < frame.f_code->co_nlocals,\
             "%O(%I32d) : %O : +%.4Ix : Invalid local id: %I16u",\
             _DeeStackFrame_File(&frame),\
             _DeeStackFrame_Line(&frame)+1,\
             _DeeStackFrame_Func(&frame),\
             (Dee_size_t)((code-1)-frame.f_code->co_code),(id))
#define RT_DEBUG_CHECK_VALID_LOCAL32(id)\
 DEE_ASSERTF((id) < frame.f_code->co_nlocals,\
             "%O(%I32d) : %O : +%.4Ix : Invalid local id: %I32u",\
             _DeeStackFrame_File(&frame),\
             _DeeStackFrame_Line(&frame)+1,\
             _DeeStackFrame_Func(&frame),\
             (Dee_size_t)((code-1)-frame.f_code->co_code),(id))
#define RT_DEBUG_CHECK_VALID_CONST(id)\
 DEE_ASSERTF((id) < DeeTuple_SIZE(frame.f_code->co_consts),\
             "%O(%I32d) : %O : +%.4Ix : Invalid constant/static id: %I16u",\
             _DeeStackFrame_File(&frame),\
             _DeeStackFrame_Line(&frame)+1,\
             _DeeStackFrame_Func(&frame),\
             (Dee_size_t)((code-1)-frame.f_code->co_code),(id))
#define RT_DEBUG_CHECK_VALID_CONST32(id)\
 DEE_ASSERTF((id) < DeeTuple_SIZE(frame.f_code->co_consts),\
             "%O(%I32d) : %O : +%.4Ix : Invalid constant/static id: %I32u",\
             _DeeStackFrame_File(&frame),\
             _DeeStackFrame_Line(&frame)+1,\
             _DeeStackFrame_Func(&frame),\
             (Dee_size_t)((code-1)-frame.f_code->co_code),(id))


#define RT_DEBUG_CHECK_STACK(n) \
 DEE_ASSERTF((Dee_size_t)(stack_end-stack)>=(Dee_size_t)(n),\
             "%O(%I32d) : %O : +%.4Ix : Stack is too small",\
             _DeeStackFrame_File(&frame),\
             _DeeStackFrame_Line(&frame)+1,\
             _DeeStackFrame_Func(&frame),\
             (Dee_size_t)((code-1)-frame.f_code->co_code))
#define RT_DEBUG_CHECK_STACK32 RT_DEBUG_CHECK_STACK
#define RT_STACK_SLOT(n)        stack_end[-((n)+1)]
#define RT_TOP                  stack_end[-1]
#define RT_SECOND               stack_end[-2]
#define RT_THIRD                stack_end[-3]
#define RT_FOURTH               stack_end[-4]
#ifdef STACK_DEBUG
#define RT_POP_1() \
 (DEE_ASSERT(stack_end != stack),(void)(*--stack_end=NULL))
#define RT_POP_N(n) \
do{\
 Dee_ssize_t const _n=(Dee_ssize_t)(n);\
 DEE_ASSERT((Dee_ssize_t)(stack_end-frame.f_stackv) >= _n);\
 stack_end -= _n;\
 memset(stack_end,0,_n*sizeof(DeeObject *));\
}while(0)
#else /* DEE_DEBUG */
#define RT_POP_1()              (void)--stack_end
#define RT_POP_N(n)             (void)(stack_end-=(n))
#endif /* !DEE_DEBUG */
#define RT_PUSH_INHERIT(ob)     Dee_INHERIT_REF(*stack_end++,ob)
#define RT_PUSH(ob)             Dee_INCREF(*stack_end++=(ob))
#define RT_PEEK_ARG()           (*(Dee_uint16_t *)code)
#define RT_READ_ARG()           (*(*(Dee_uint16_t **)&code)++)
#define RT_LAST_ARG()           (((Dee_uint16_t *)code)[-1])
#define RT_PEEK_SARG()          (*(Dee_int16_t *)code)
#define RT_READ_SARG()          (*(*(Dee_int16_t **)&code)++)
#define RT_LAST_SARG()          (((Dee_int16_t *)code)[-1])
#define RT_SKIP_ARG()           (void)(code+=sizeof(Dee_uint16_t))
#define RT_PREV_ARG()           (void)(--*(Dee_int16_t **)&code)
#define RT_PEEK_OP()            (*code)
#define RT_PREV_OP()            (void)(--code)
#define RT_READ_OP()            (*code++)
#define RT_SKIP_OP()            (void)(++code)
#define RT_STACK_ADDR(n)        (stack_end-(n))
#define RT_STACK_ADDR32(n)      (stack_end-(n))
#define RT_CODE_ADDR()          code
#ifdef DEE_DEBUG
#define RT_ASSERT_EXCEPTION_OCCURRED() \
 DEE_ASSERTF(ERROR_OCCURRED,"%O(%I32d) : %O : +%.4Ix : No exception set",\
            _DeeStackFrame_File(&frame),_DeeStackFrame_Line(&frame)+1,\
            _DeeStackFrame_Func(&frame),_DeeStackFrame_Addr(&frame))
#endif
#if DEE_CONFIG_HAVE_MSVC_MEMORY_DEBUG && defined(RT_ASSERT_EXCEPTION_OCCURRED)
#define RT_HANDLE_EXCEPTION     do{RT_ASSERT_EXCEPTION_OCCURRED();_CrtCheckMemory();goto handle_exception;}while(0)
#elif DEE_CONFIG_HAVE_MSVC_MEMORY_DEBUG
#define RT_HANDLE_EXCEPTION     do{_CrtCheckMemory();goto handle_exception;}while(0)
#elif defined(RT_ASSERT_EXCEPTION_OCCURRED)
#define RT_HANDLE_EXCEPTION     do{RT_ASSERT_EXCEPTION_OCCURRED();goto handle_exception;}while(0)
#else
#define RT_HANDLE_EXCEPTION     goto handle_exception
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
exec:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 // Override the internal decref function to call
 // the destroy function inside of an extern block.
 // >> This improves response time when another
 //    thread tries to access our stackframe and
 //    prevents deadlocks if we lock our own frame
 //    from the outside.
#ifdef DEE_DEBUG
#define _DeeExecuteObject_DECREF(ob)\
do{if(!_DeeRefCounter_DecFetch(ob->__ob_refcnt)){\
 EXTERN_BEGIN; _DeeObject_InternalDestroy_d(ob,__FILE__,__LINE__); EXTERN_END;\
}}while(0)
#else /* DEE_DEBUG */
#define _DeeExecuteObject_DECREF(ob)\
do{if(!_DeeRefCounter_DecFetch(ob->__ob_refcnt)){\
 EXTERN_BEGIN; _DeeObject_InternalDestroy(ob); EXTERN_END;\
}}while(0)
#endif /* !DEE_DEBUG */
#undef _DeeObject_DECREF
#define _DeeObject_DECREF      _DeeExecuteObject_DECREF

{
 DeeObject *rt_temp;
 int rt_itemp;
 Dee_uint16_t rt_arg;
next_op:
#if 0 /*< For those REALLY hard to find bugs... */
 DEE_LDEBUG("%O(%I32d) : %O : +%.4Ix : TRACE\n",
            _DeeStackFrame_File(&frame),
            _DeeStackFrame_Line(&frame)+1,
            _DeeStackFrame_Func(&frame),
            _DeeStackFrame_Addr(&frame));
#endif
#if defined(DEE_DEBUG) && 0
#define TARGET_DEBUG(op_str,op)\
 EXTERN_BEGIN; DEE_LDEBUG("Stack %d [" op_str ":" #op "]\n",(int)(frame.f_stackv_end-frame.f_stackv)); EXTERN_END;
#else
#define TARGET_DEBUG(op_str,op)
#endif

#if DEE_XCONFIG_RUNTIME_USE_COMPUTED_GOTOS
#define TARGET(op) TARGET_##op: case op: TARGET_DEBUG(#op,op)
#else
#define TARGET(op)              case op: TARGET_DEBUG(#op,op)
#endif
#ifdef DEE_DEBUG
#define DISPATCH() \
do{\
 if (!_DeeFlag_NoAssert && ERROR_OCCURRED) {\
  DeeObject **_stack_iter = stack_end;\
  while (_stack_iter-- != stack) DEE_LDEBUG("[STACK %Id] %r\n",_stack_iter-stack,*_stack_iter);\
  DEE_ASSERTF(0,"%O(%I32d) : %O : +%.4Ix : DISPATCH() reached with exception set",\
              _DeeStackFrame_File(&frame),\
              _DeeStackFrame_Line(&frame)+1,\
              _DeeStackFrame_Func(&frame),\
              (Dee_size_t)((code-1)-frame.f_code->co_code));\
 }\
 goto next_op;\
}while(0)
#else
#define DISPATCH() goto next_op
#endif

#define PREDICT(op) (void)0 // Nothing (yet)

 // Read an opcode and jump to its target
 switch (RT_READ_OP()) {

#ifdef OP_STOP
  TARGET(OP_STOP) {
   goto end;
  }
#endif

#ifdef OP_RETVAREXIT
  TARGET(OP_RETVAREXIT) {
   // If no result has been set, just return 'none'
   if (!frame.f_result) Dee_INCREF(frame.f_result = Dee_None);
   goto end;
  }
#endif

#ifdef OP_RETNONE
  TARGET(OP_RETNONE) {
   rt_temp = frame.f_result;
   Dee_INCREF(frame.f_result = Dee_None);
   Dee_XDECREF(rt_temp);
   goto end;
  }
#endif

#ifdef OP_YIELD
  TARGET(OP_YIELD) {
   RT_DEBUG_CHECK_STACK(1);
   DEE_ASSERT(!frame.f_result && "Return value already set in yield function");
   Dee_INHERIT_REF(frame.f_result,RT_TOP);
   RT_POP_1();
   goto end_yield; // Skip finally resolution and stack checks (we will continue later)
  }
#endif

#ifdef OP_YIELDALL
  TARGET(OP_YIELDALL) {
   RT_DEBUG_CHECK_STACK(1);
   DEE_ASSERTF(!frame.f_result,"Return value already set in yield function");
   if DEE_UNLIKELY((rt_itemp = DeeObject_IterNextEx(RT_TOP,&rt_temp)) != 0) {
    Dee_DECREF(RT_TOP); RT_POP_1();
    if DEE_UNLIKELY(rt_itemp < 0) RT_HANDLE_EXCEPTION;
   } else {
    --code; // yieldall must be executed again once we return
    Dee_INHERIT_REF(frame.f_result,rt_temp);
    goto end_yield; // Skip finally resolution and stack checks (we will continue later)
   }
   DISPATCH();
  }
#endif

#ifdef OP_YIELDSTOP
  TARGET(OP_YIELDSTOP) {
   frame.f_result = NULL; // return NULL without an error set
   goto end;
  }
#endif

#ifdef OP_RET
  TARGET(OP_RET) {
   RT_DEBUG_CHECK_STACK(1);
   // Result may already be assigned if we have
   // a second return from within a finally block
   rt_temp = frame.f_result;
   Dee_INHERIT_REF(frame.f_result,RT_TOP);
   Dee_XDECREF(rt_temp);
   RT_POP_1();
   goto end;
  }
#endif

#ifdef OP_NOOP
  TARGET(OP_NOOP) {
   DISPATCH();
  }
#endif

#ifdef OP_LOAD_RET
  TARGET(OP_LOAD_RET) {
   if DEE_UNLIKELY(!frame.f_result) {
EXTERN_BEGIN;
    DeeError_Throw((DeeObject *)&_DeeRuntimeError_UnboundLocal);
EXTERN_END;
    RT_PUSH(Dee_None);
    RT_HANDLE_EXCEPTION;
   }
   RT_PUSH(frame.f_result);
   DISPATCH();
  }
#endif

#ifdef OP_LOAD_THIS
  TARGET(OP_LOAD_THIS) {
#ifdef DEE_DEBUG
   DEE_ASSERTF(frame.f_this != NULL,"Can't load 'this' from non-[[thiscall]] code");
#endif
   RT_PUSH(frame.f_this);
   DISPATCH();
  }
#endif

#ifdef OP_DUP
  TARGET(OP_DUP) {
   RT_DEBUG_CHECK_STACK(1);
   rt_temp = RT_TOP;
   RT_PUSH(rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_STORE_RET
  TARGET(OP_STORE_RET) {
   RT_DEBUG_CHECK_STACK(1);
   rt_temp = frame.f_result;
   Dee_INCREF(frame.f_result = RT_TOP);
   Dee_XDECREF(rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_STORE_RET_POP
  TARGET(OP_STORE_RET_POP) {
   RT_DEBUG_CHECK_STACK(1);
   rt_temp = frame.f_result;
   Dee_INHERIT_REF(frame.f_result,RT_TOP);
   Dee_XDECREF(rt_temp);
   RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_POP
  TARGET(OP_POP) {
   RT_DEBUG_CHECK_STACK(1);
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_LOAD_NONE
  TARGET(OP_LOAD_NONE) {
   RT_PUSH(Dee_None);
   DISPATCH();
  }
#endif

#ifdef OP_TYPE
  TARGET(OP_TYPE) {
   RT_DEBUG_CHECK_STACK(1);
   rt_temp = RT_TOP;
   Dee_INCREF(RT_TOP = (DeeObject *)Dee_TYPE(rt_temp));
   Dee_DECREF(rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_BOOL
  TARGET(OP_BOOL) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_itemp = DeeObject_Bool(RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp < 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP);
   Dee_INCREF(RT_TOP = rt_itemp ? Dee_True : Dee_False);
   DISPATCH();
  }
#endif

#ifdef OP_IS
  TARGET(OP_IS) {
   RT_DEBUG_CHECK_STACK(2);
   // No need to call this in an extern block, as
   // this function isn't a DEE_A_EXEC callback.
   rt_itemp = DeeObject_Is(RT_SECOND,(DeeTypeObject *)RT_TOP);
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP);
   Dee_INCREF(RT_TOP = rt_itemp ? Dee_True : Dee_False);
   DISPATCH();
  }
#endif

#ifdef OP_IN
  TARGET(OP_IN) {
   RT_DEBUG_CHECK_STACK(2);
   // Swapped order, because this function takes the list as argument 0
EXTERN_BEGIN;
   rt_temp = DeeObject_ContainsObject(RT_TOP,RT_SECOND);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_CALL
  TARGET(OP_CALL) {
   RT_DEBUG_CHECK_STACK(2);
   DEE_ASSERT(DeeTuple_Check(RT_TOP));
   // TODO: if we are calling a deemon function object,
   //       we can inline the call to never cause a real stack overflow
EXTERN_BEGIN;
   rt_temp = DeeObject_Call(RT_SECOND,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_NEW_INSTANCE
  TARGET(OP_NEW_INSTANCE) {
   RT_DEBUG_CHECK_STACK(2);
   DEE_ASSERT(DeeType_Check(RT_SECOND));
   DEE_ASSERT(DeeTuple_Check(RT_TOP));
EXTERN_BEGIN;
   rt_temp = DeeType_NewInstance((DeeTypeObject *)RT_SECOND,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_MOVE_ASSIGN
  TARGET(OP_MOVE_ASSIGN) {
   RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
   rt_itemp = DeeObject_MoveAssign(RT_SECOND,RT_TOP);
EXTERN_END;
   Dee_DECREF(RT_TOP); RT_POP_1();
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   // RT_TOP is now the lhs operand of the assign op
   DISPATCH();
  }
#endif

#ifdef OP_COPY_ASSIGN
  TARGET(OP_COPY_ASSIGN) {
   RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
   rt_itemp = DeeObject_CopyAssign(RT_SECOND,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   // RT_TOP is now the lhs operand of the assign op
   DISPATCH();
  }
#endif

#if 1
#define MAKE_UNARY_OP(func)\
   RT_DEBUG_CHECK_STACK(1);\
EXTERN_BEGIN;\
   rt_temp = func(RT_TOP);\
   goto unary_end
#else
#define MAKE_UNARY_OP(func)\
   RT_DEBUG_CHECK_STACK(1);\
EXTERN_BEGIN;\
   rt_temp = func(RT_TOP);\
EXTERN_END;\
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;\
   Dee_DECREF(RT_TOP);\
   Dee_INHERIT_REF(RT_TOP,rt_temp);\
   DISPATCH()
#endif

#ifdef OP_STR
  TARGET(OP_STR) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_temp = DeeObject_Str(RT_TOP);
unary_end:
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP);
   Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_REPR
  TARGET(OP_REPR) {
   MAKE_UNARY_OP(DeeObject_Repr);
  }
#endif

#ifdef OP_NOT
  TARGET(OP_NOT) {
   MAKE_UNARY_OP(DeeObject_Not);
  }
#endif

#ifdef OP_POS
  TARGET(OP_POS) {
   MAKE_UNARY_OP(DeeObject_Pos);
  }
#endif

#ifdef OP_NEG
  TARGET(OP_NEG) {
   MAKE_UNARY_OP(DeeObject_Neg);
  }
#endif

#ifdef OP_REF
  TARGET(OP_REF) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   MAKE_UNARY_OP(DeeObject_Ref);
#else
   goto on_unreachable;
#endif
  }
#endif

#ifdef OP_DEREF
  TARGET(OP_DEREF) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   MAKE_UNARY_OP(DeeObject_Deref);
#else
   goto on_unreachable;
#endif
  }
#endif

#ifdef OP_INV
  TARGET(OP_INV) {
   MAKE_UNARY_OP(DeeObject_Inv);
  }
#endif

#ifdef OP_INC
  TARGET(OP_INC) {
   MAKE_UNARY_OP(DeeObject_Inc);
  }
#endif

#ifdef OP_DEC
  TARGET(OP_DEC) {
   MAKE_UNARY_OP(DeeObject_Dec);
  }
#endif

#ifdef OP_INC_POST
  TARGET(OP_INC_POST) {
   MAKE_UNARY_OP(DeeObject_IncPost);
  }
#endif

#ifdef OP_DEC_POST
  TARGET(OP_DEC_POST) {
   MAKE_UNARY_OP(DeeObject_DecPost);
  }
#endif

#ifdef OP_MOVE
  TARGET(OP_MOVE) {
   MAKE_UNARY_OP(DeeObject_Move);
  }
#endif

#ifdef OP_COPY
  TARGET(OP_COPY) {
   MAKE_UNARY_OP(DeeObject_Copy);
  }
#endif

#ifdef OP_WEAK
  TARGET(OP_WEAK) {
   MAKE_UNARY_OP(DeeWeakref_New);
  }
#endif

#undef MAKE_UNARY_OP
#if 1
#define MAKE_BIARY_OP(func)\
   RT_DEBUG_CHECK_STACK(2);\
EXTERN_BEGIN;\
   rt_temp = func(RT_SECOND,RT_TOP);\
   goto binary_end
#else
#define MAKE_BIARY_OP(func)\
   RT_DEBUG_CHECK_STACK(2);\
EXTERN_BEGIN;\
   rt_temp = func(RT_SECOND,RT_TOP);\
EXTERN_END;\
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;\
   Dee_DECREF(RT_TOP); RT_POP_1();\
   Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);\
   DISPATCH()
#endif

#ifdef OP_ADD
  TARGET(OP_ADD) {
   RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
   rt_temp = DeeObject_Add(RT_SECOND,RT_TOP);
binary_end:
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_INPLACE_ADD
  TARGET(OP_INPLACE_ADD) {
   MAKE_BIARY_OP(DeeObject_InplaceAdd);
  }
#endif

#ifdef OP_SUB
  TARGET(OP_SUB) {
   MAKE_BIARY_OP(DeeObject_Sub);
  }
#endif

#ifdef OP_INPLACE_SUB
  TARGET(OP_INPLACE_SUB) {
   MAKE_BIARY_OP(DeeObject_InplaceSub);
  }
#endif

#ifdef OP_MUL
  TARGET(OP_MUL) {
   MAKE_BIARY_OP(DeeObject_Mul);
  }
#endif

#ifdef OP_INPLACE_MUL
  TARGET(OP_INPLACE_MUL) {
   MAKE_BIARY_OP(DeeObject_InplaceMul);
  }
#endif

#ifdef OP_DIV
  TARGET(OP_DIV) {
   MAKE_BIARY_OP(DeeObject_Div);
  }
#endif

#ifdef OP_INPLACE_DIV
  TARGET(OP_INPLACE_DIV) {
   MAKE_BIARY_OP(DeeObject_InplaceDiv);
  }
#endif

#ifdef OP_MOD
  TARGET(OP_MOD) {
   MAKE_BIARY_OP(DeeObject_Mod);
  }
#endif

#ifdef OP_INPLACE_MOD
  TARGET(OP_INPLACE_MOD) {
   MAKE_BIARY_OP(DeeObject_InplaceMod);
  }
#endif

#ifdef OP_SHL
  TARGET(OP_SHL) {
   MAKE_BIARY_OP(DeeObject_Shl);;
  }
#endif

#ifdef OP_INPLACE_SHL
  TARGET(OP_INPLACE_SHL) {
   MAKE_BIARY_OP(DeeObject_InplaceShl);;
  }
#endif

#ifdef OP_SHR
  TARGET(OP_SHR) {
   MAKE_BIARY_OP(DeeObject_Shr);;
  }
#endif

#ifdef OP_INPLACE_SHR
  TARGET(OP_INPLACE_SHR) {
   MAKE_BIARY_OP(DeeObject_InplaceShr);
  }
#endif

#ifdef OP_AND
  TARGET(OP_AND) {
   MAKE_BIARY_OP(DeeObject_And);
  }
#endif

#ifdef OP_INPLACE_AND
  TARGET(OP_INPLACE_AND) {
   MAKE_BIARY_OP(DeeObject_InplaceAnd);
  }
#endif

#ifdef OP_OR
  TARGET(OP_OR) {
   MAKE_BIARY_OP(DeeObject_Or);
  }
#endif

#ifdef OP_INPLACE_OR
  TARGET(OP_INPLACE_OR) {
   MAKE_BIARY_OP(DeeObject_InplaceOr);
  }
#endif

#ifdef OP_XOR
  TARGET(OP_XOR) {
   MAKE_BIARY_OP(DeeObject_Xor);
  }
#endif

#ifdef OP_INPLACE_XOR
  TARGET(OP_INPLACE_XOR) {
   MAKE_BIARY_OP(DeeObject_InplaceXor);
  }
#endif

#ifdef OP_POW
  TARGET(OP_POW) {
   MAKE_BIARY_OP(DeeObject_Pow);
  }
#endif

#ifdef OP_INPLACE_POW
  TARGET(OP_INPLACE_POW) {
   MAKE_BIARY_OP(DeeObject_InplacePow);
  }
#endif

#ifdef OP_CMP_LO
  TARGET(OP_CMP_LO) {
   MAKE_BIARY_OP(DeeObject_CompareLoObject);
  }
#endif

#ifdef OP_CMP_LE
  TARGET(OP_CMP_LE) {
   MAKE_BIARY_OP(DeeObject_CompareLeObject);
  }
#endif

#ifdef OP_CMP_EQ
  TARGET(OP_CMP_EQ) {
   MAKE_BIARY_OP(DeeObject_CompareEqObject);
  }
#endif

#ifdef OP_CMP_NE
  TARGET(OP_CMP_NE) {
   MAKE_BIARY_OP(DeeObject_CompareNeObject);
  }
#endif

#ifdef OP_CMP_GR
  TARGET(OP_CMP_GR) {
   MAKE_BIARY_OP(DeeObject_CompareGrObject);
  }
#endif

#ifdef OP_CMP_GE
  TARGET(OP_CMP_GE) {
   MAKE_BIARY_OP(DeeObject_CompareGeObject);
  }
#endif
#undef MAKE_BIARY_OP

#ifdef OP_SEQ_GET
  TARGET(OP_SEQ_GET) {
   RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
   rt_temp = DeeObject_GetItem(RT_SECOND,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_SEQ_DEL
  TARGET(OP_SEQ_DEL) {
   RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
   rt_itemp = DeeObject_DelItem(RT_SECOND,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_SEQ_SET
  TARGET(OP_SEQ_SET) {
   RT_DEBUG_CHECK_STACK(3);
EXTERN_BEGIN;
   rt_itemp = DeeObject_SetItem(RT_THIRD,RT_SECOND,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   rt_temp = RT_TOP;   RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);
   // the assigned value is now at RT_TOP
   DISPATCH();
  }
#endif

#ifdef OP_SEQ_SIZE
  TARGET(OP_SEQ_SIZE) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_temp = DeeObject_SizeObject(RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP);
   Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_SEQ_RANGE_GET
  TARGET(OP_SEQ_RANGE_GET) {
   RT_DEBUG_CHECK_STACK(3);
EXTERN_BEGIN;
   rt_temp = DeeObject_GetRangeItem(RT_THIRD,RT_SECOND,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP);
   Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_SEQ_RANGE_DEL
  TARGET(OP_SEQ_RANGE_DEL) {
   RT_DEBUG_CHECK_STACK(3);
EXTERN_BEGIN;
   rt_itemp = DeeObject_DelRangeItem(RT_THIRD,RT_SECOND,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_SEQ_RANGE_SET
  TARGET(OP_SEQ_RANGE_SET) {
   RT_DEBUG_CHECK_STACK(4);
EXTERN_BEGIN;
   rt_itemp = DeeObject_SetRangeItem(RT_FOURTH,RT_THIRD,RT_SECOND,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   rt_temp = RT_TOP;   RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);
   // the assigned value is now RT_TOP
   DISPATCH();
  }
#endif

#ifdef OP_SEQ_ITER_SELF
  TARGET(OP_SEQ_ITER_SELF) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_temp = DeeObject_IterSelf(RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP);
   Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_ATTR_GET
  TARGET(OP_ATTR_GET) {
   RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
   rt_temp = DeeObject_GetAttr(RT_SECOND,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP);
   Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_ATTR_HAS
  TARGET(OP_ATTR_HAS) {
   RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
   rt_itemp = DeeObject_HasAttr(RT_SECOND,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp < 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); RT_TOP = DeeBool_New(rt_itemp);
   DISPATCH();
  }
#endif

#ifdef OP_ATTR_DEL
  TARGET(OP_ATTR_DEL) {
   RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
   rt_itemp = DeeObject_DelAttr(RT_SECOND,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_ATTR_SET
  TARGET(OP_ATTR_SET) {
   RT_DEBUG_CHECK_STACK(3);
EXTERN_BEGIN;
   rt_itemp = DeeObject_SetAttr(RT_THIRD,RT_TOP,RT_SECOND);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   rt_temp = RT_TOP;   RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);
   // the assigned value is now in RT_TOP
   DISPATCH();
  }
#endif

#ifdef OP_ROT_2
  TARGET(OP_ROT_2) {
   // ab -> ba
   RT_DEBUG_CHECK_STACK(2);
   rt_temp = RT_TOP;
   RT_TOP = RT_SECOND;
   RT_SECOND = rt_temp;
   DISPATCH();
  }
#endif

#ifdef OP_LROT_3
  TARGET(OP_LROT_3) {
   // abc -> bca
   RT_DEBUG_CHECK_STACK(3);
   rt_temp = RT_TOP;
   RT_TOP = RT_THIRD;
   RT_THIRD = RT_SECOND;
   RT_SECOND = rt_temp;
   DISPATCH();
  }
#endif

#ifdef OP_RROT_3
  TARGET(OP_RROT_3) {
   // abc -> cab
   RT_DEBUG_CHECK_STACK(3);
   rt_temp = RT_TOP;
   RT_TOP = RT_SECOND;
   RT_SECOND = RT_THIRD;
   RT_THIRD = rt_temp;
   DISPATCH();
  }
#endif

#ifdef OP_LROT_4
  TARGET(OP_LROT_4) {
   // abcd -> bcda
   RT_DEBUG_CHECK_STACK(4);
   rt_temp = RT_TOP;
   RT_TOP = RT_FOURTH;
   RT_FOURTH = RT_THIRD;
   RT_THIRD = RT_SECOND;
   RT_SECOND = rt_temp;
   DISPATCH();
  }
#endif

#ifdef OP_RROT_4
  TARGET(OP_RROT_4) {
   // abcd -> dabc
   RT_DEBUG_CHECK_STACK(4);
   rt_temp = RT_TOP;
   RT_TOP = RT_SECOND;
   RT_SECOND = RT_THIRD;
   RT_THIRD = RT_FOURTH;
   RT_FOURTH = rt_temp;
   DISPATCH();
  }
#endif

#ifdef OP_LROT_N
  TARGET(OP_LROT_N) {
   // ab...yz -> b...yza
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_STACK(rt_arg+1);
   rt_temp = *(stack_end-(rt_arg+1));
   memmove(stack_end-(rt_arg+1),stack_end-rt_arg,rt_arg*sizeof(DeeObject *));
   stack_end[-1] = rt_temp;
   DISPATCH();
  }
#endif

#ifdef OP_RROT_N
  TARGET(OP_RROT_N) {
   // ab...yz -> zab...y
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_STACK(rt_arg+1);
   rt_temp = stack_end[-1];
   memmove(stack_end-rt_arg,stack_end-(rt_arg+1),rt_arg*sizeof(DeeObject *));
   *(stack_end-(rt_arg+1)) = rt_temp;
   DISPATCH();
  }
#endif

#ifdef OP_LOAD_LOC
  TARGET(OP_LOAD_LOC) {
   // Read-only access (no need to lock)
#ifdef DEE_DEBUG
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_LOCAL(rt_arg);
   rt_temp = frame.f_localv[rt_arg];
#else
   rt_temp = frame.f_localv[RT_READ_ARG()];
#endif
   if DEE_UNLIKELY(!rt_temp) {
EXTERN_BEGIN;
    // v this would probably be likely, but lets
    //   optimize this for code without debug informations
    if DEE_UNLIKELY(frame.f_code->co_debug) {
     rt_arg = RT_LAST_ARG();
unbound_local_rtarg:
     rt_temp = (DeeObject *)frame.f_code->co_debug->di_local_names;
     if DEE_UNLIKELY(!rt_temp) goto unbound_local_default;
     if DEE_UNLIKELY(rt_arg >= DeeTuple_SIZE(frame.f_code->co_debug->di_local_names)) goto unbound_local_default;
     rt_temp = DeeTuple_GET(rt_temp,rt_arg);
     if DEE_UNLIKELY(!DeeString_Check(rt_temp)) goto unbound_local_default;
     DeeError_SetStringf(&DeeErrorType_UnboundLocal,"Unbound local: %q",DeeString_STR(rt_temp));
    } else {
unbound_local_default:
     DeeError_Throw((DeeObject *)&_DeeRuntimeError_UnboundLocal);
    }
EXTERN_END;
    RT_PUSH(Dee_None);
    RT_HANDLE_EXCEPTION;
   }
   RT_PUSH(rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_LOAD_REF
  TARGET(OP_LOAD_REF) {
#ifdef DEE_DEBUG
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_REF(rt_arg);
   RT_PUSH(frame.f_refv[rt_arg]);
#else
   RT_PUSH(frame.f_refv[RT_READ_ARG()]);
#endif
   DISPATCH();
  }
#endif

#ifdef OP_LOAD_CST_COPY
  TARGET(OP_LOAD_CST_COPY) {
#ifdef DEE_DEBUG
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_CONST(rt_arg);
   rt_temp = frame.f_constv[rt_arg];
#else
   rt_temp = frame.f_constv[RT_READ_ARG()];
#endif
push_copy_rt_temp:
EXTERN_BEGIN;
   rt_temp = DeeObject_Copy(rt_temp);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   RT_PUSH_INHERIT(rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_LOAD_CST_DCOPY
  TARGET(OP_LOAD_CST_DCOPY) {
#ifdef DEE_DEBUG
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_CONST(rt_arg);
   rt_temp = frame.f_constv[rt_arg];
#else
   rt_temp = frame.f_constv[RT_READ_ARG()];
#endif
push_deep_copy_rt_temp:
EXTERN_BEGIN;
   rt_temp = DeeObject_DeepCopy(rt_temp);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   RT_PUSH_INHERIT(rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_LOAD_CST
  TARGET(OP_LOAD_CST) {
#ifdef DEE_DEBUG
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_CONST(rt_arg);
   RT_PUSH(frame.f_constv[rt_arg]);
#else
   RT_PUSH(frame.f_constv[RT_READ_ARG()]);
#endif
   DISPATCH();
  }
#endif

#ifdef OP_LOAD_CST_LOCKED
  TARGET(OP_LOAD_CST_LOCKED) {
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_CONST(rt_arg);
EXTERN_BEGIN;
   DeeAtomicMutex_Acquire(&frame.f_code->co_consts_lock);
   Dee_INCREF(rt_temp = frame.f_constv[rt_arg]);
   DeeAtomicMutex_Release(&frame.f_code->co_consts_lock);
EXTERN_END;
   RT_PUSH_INHERIT(rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_LOAD_BLTIN
  TARGET(OP_LOAD_BLTIN) {
#ifdef DEE_DEBUG
   rt_arg = RT_READ_ARG();
   rt_temp = DeeBuiltin_Get(rt_arg);
   DEE_ASSERTF(DeeObject_Check(rt_temp),
               "Tried to load invalid builtin-id: %I16u",
               rt_arg);
   RT_PUSH(rt_temp);
#else
   RT_PUSH(DeeBuiltin_GET(RT_READ_ARG()));
#endif
   DISPATCH();
  }
#endif

#ifdef OP_STORE_LOC
  TARGET(OP_STORE_LOC) {
   RT_DEBUG_CHECK_STACK(1);
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_LOCAL(rt_arg);
   rt_temp = frame.f_localv[rt_arg];
   Dee_INCREF(frame.f_localv[rt_arg] = RT_TOP);
   Dee_XDECREF(rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_LOAD_ARG
  TARGET(OP_LOAD_ARG) {
   // Read-only access (no need to lock)
#ifdef DEE_DEBUG
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_ARG(rt_arg);
   RT_PUSH(frame.f_argv[rt_arg]);
#else
   RT_PUSH(frame.f_argv[RT_READ_ARG()]);
#endif
   DISPATCH();
  }
#endif

#ifdef OP_STORE_CST
  TARGET(OP_STORE_CST) {
   RT_DEBUG_CHECK_STACK(1);
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_CONST(rt_arg);
   Dee_INCREF(RT_TOP);
EXTERN_BEGIN;
   DeeAtomicMutex_Acquire(&frame.f_code->co_consts_lock);
   rt_temp = frame.f_constv[rt_arg];
   frame.f_constv[rt_arg] = RT_TOP;
   DeeAtomicMutex_Release(&frame.f_code->co_consts_lock);
EXTERN_END;
   Dee_DECREF(rt_temp); // Always assigned
   DISPATCH();
  }
#endif

#ifdef OP_TUPLE
  TARGET(OP_TUPLE) {
   rt_arg = RT_READ_ARG();
   DEE_ASSERTF(rt_arg != 0,"OP_TUPLE can't create empty tuples");
   RT_DEBUG_CHECK_STACK(rt_arg);
EXTERN_BEGIN;
   rt_temp = _DeeTuple_NewUnsafe(rt_arg);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   memcpy(DeeTuple_ELEM(rt_temp),RT_STACK_ADDR(rt_arg),
          rt_arg*sizeof(DeeObject *)); // inherit references
   RT_POP_N(rt_arg-1);
   Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_LIST
  TARGET(OP_LIST) {
   rt_arg = RT_READ_ARG();
   DEE_ASSERTF(rt_arg != 0,"OP_LIST can't create empty lists");
   RT_DEBUG_CHECK_STACK(rt_arg);
EXTERN_BEGIN;
   rt_temp = _DeeList_NewUnsafe(rt_arg);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   memcpy(DeeList_ELEM(rt_temp),RT_STACK_ADDR(rt_arg),
          rt_arg*sizeof(DeeObject *)); // inherit references
   RT_POP_N(rt_arg-1);
   Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_DICT
  TARGET(OP_DICT) {
   rt_arg = RT_READ_ARG();
   DEE_ASSERTF(rt_arg != 0,"OP_DICT can't create empty dicts");
   RT_DEBUG_CHECK_STACK(rt_arg*2);
EXTERN_BEGIN;
   rt_temp = DeeDict_NewFromKeyValuePairs(rt_arg,RT_STACK_ADDR(rt_arg*2));
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   do {
    Dee_DECREF(RT_TOP); RT_POP_1();
    Dee_DECREF(RT_TOP); RT_POP_1();
   } while (--rt_arg);
   RT_PUSH_INHERIT(rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_SET
  TARGET(OP_SET) {
   rt_arg = RT_READ_ARG();
   DEE_ASSERTF(rt_arg != 0,"OP_SET can't create empty sets");
   RT_DEBUG_CHECK_STACK(rt_arg);
EXTERN_BEGIN;
   rt_temp = DeeSet_NewFromKeys(rt_arg,RT_STACK_ADDR(rt_arg));
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   do {
    Dee_DECREF(RT_TOP); RT_POP_1();
   } while (--rt_arg);
   RT_PUSH_INHERIT(rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_JUMP_DYN_REL
  TARGET(OP_JUMP_DYN_REL) {
   Dee_ssize_t addroff;
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_itemp = DeeObject_Cast(Dee_ssize_t,RT_TOP,&addroff);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   RT_POP_1();
   RT_CODE_ADDR() += (addroff-sizeof(Dee_uint8_t));
   DEE_ASSERTF(RT_CODE_ADDR() >= frame.f_code->co_code &&
               RT_CODE_ADDR() < frame.f_code->co_code+frame.f_code->co_size,
               "%s(%I32d) : %k : Code pointer is out-of-bounds after OP_JUMP_DYN_REL:\n"
               "\tbegin: %#I." DEE_PP_STR(DEE_PP_MUL8(DEE_TYPES_SIZEOF_POINTER)) "x\n"
               "\tend:   %#I." DEE_PP_STR(DEE_PP_MUL8(DEE_TYPES_SIZEOF_POINTER)) "x\n"
               "\taddr:  %#I." DEE_PP_STR(DEE_PP_MUL8(DEE_TYPES_SIZEOF_POINTER)) "x\n",
               _DeeStackFrame_File(&frame),_DeeStackFrame_Line(&frame)+1,_DeeStackFrame_Func(&frame),
               frame.f_code->co_code,frame.f_code->co_code+frame.f_code->co_size,RT_CODE_ADDR());
   DISPATCH();
  }
#endif

#ifdef OP_JUMP_DYN_ABS
  TARGET(OP_JUMP_DYN_ABS) {
   Dee_size_t newaddr;
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_itemp = DeeObject_Cast(Dee_size_t,RT_TOP,&newaddr);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   RT_POP_1();
   RT_CODE_ADDR() = frame.f_code->co_code+newaddr;
   DEE_ASSERTF(RT_CODE_ADDR() >= frame.f_code->co_code &&
               RT_CODE_ADDR() < frame.f_code->co_code+frame.f_code->co_size,
               "%s(%I32d) : %k : Code pointer is out-of-bounds after OP_JUMP_DYN_ABS:\n"
               "\tbegin: %#I." DEE_PP_STR(DEE_PP_MUL8(DEE_TYPES_SIZEOF_POINTER)) "x\n"
               "\tend:   %#I." DEE_PP_STR(DEE_PP_MUL8(DEE_TYPES_SIZEOF_POINTER)) "x\n"
               "\taddr:  %#I." DEE_PP_STR(DEE_PP_MUL8(DEE_TYPES_SIZEOF_POINTER)) "x\n",
               _DeeStackFrame_File(&frame),_DeeStackFrame_Line(&frame)+1,_DeeStackFrame_Func(&frame),
               frame.f_code->co_code,frame.f_code->co_code+frame.f_code->co_size,RT_CODE_ADDR());
   DISPATCH();
  }
#endif

#ifdef OP_JUMP
  TARGET(OP_JUMP) {
   RT_CODE_ADDR() += (RT_PEEK_SARG()-1);
   DISPATCH();
  }
#endif

#ifdef OP_JUMP_IF_TT_POP
  TARGET(OP_JUMP_IF_TT_POP) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_itemp = DeeObject_Bool(RT_TOP);
EXTERN_END;
   Dee_DECREF(RT_TOP); RT_POP_1();
   if (rt_itemp) {
    if DEE_UNLIKELY(rt_itemp < 0) {
skip_arg_err: RT_SKIP_ARG(); RT_HANDLE_EXCEPTION;
    }
    RT_CODE_ADDR() += (RT_PEEK_SARG()-1);
   } else RT_SKIP_ARG();
   DISPATCH();
  }
#endif

#ifdef OP_JUMP_IF_FF_POP
  TARGET(OP_JUMP_IF_FF_POP) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_itemp = DeeObject_Bool(RT_TOP);
EXTERN_END;
   Dee_DECREF(RT_TOP); RT_POP_1();
   if (!rt_itemp) RT_CODE_ADDR() += (RT_PEEK_SARG()-1);
   else {
    if DEE_UNLIKELY(rt_itemp < 0) goto skip_arg_err;
    RT_SKIP_ARG();
   }
   DISPATCH();
  }
#endif

#ifdef OP_JUMP_IF_CASE_COMPARE
  TARGET(OP_JUMP_IF_CASE_COMPARE) {
   RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
   rt_itemp = DeeObject_CompareEq(RT_TOP,RT_SECOND);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp < 0 && !DeeError_Catch(
    &DeeErrorType_NotImplemented)) goto skip_arg_err;
   Dee_DECREF(RT_TOP); RT_POP_1();
   if DEE_UNLIKELY(rt_itemp > 0) RT_CODE_ADDR() += (RT_PEEK_SARG()-1);
   else RT_SKIP_ARG();
   DISPATCH();
  }
#endif

#ifdef OP_JUMP_IF_RCASE_COMPARE
  TARGET(OP_JUMP_IF_RCASE_COMPARE) {
   RT_DEBUG_CHECK_STACK(3);
EXTERN_BEGIN;
   rt_itemp = DeeObject_CompareLe(RT_SECOND,RT_THIRD);
   if (rt_itemp == 1) rt_itemp = DeeObject_CompareGe(RT_TOP,RT_THIRD);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp < 0 && !DeeError_Catch(
    &DeeErrorType_NotImplemented)) goto skip_arg_err;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   if DEE_UNLIKELY(rt_itemp > 0) RT_CODE_ADDR() += (RT_PEEK_SARG()-1);
   else RT_SKIP_ARG();
   DISPATCH();
  }
#endif

#ifdef OP_ATTR_GET_C
  TARGET(OP_ATTR_GET_C) {
   RT_DEBUG_CHECK_STACK(1);
#ifdef DEE_DEBUG
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_CONST(rt_arg);
   rt_temp = frame.f_constv[rt_arg];
#else
   rt_temp = frame.f_constv[RT_READ_ARG()];
#endif
do_get_attr_c_rt_temp:
EXTERN_BEGIN;
   rt_temp = DeeObject_GetAttr(RT_TOP,rt_temp);
EXTERN_END;
   if DEE_UNLIKELY(rt_temp == NULL) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP);
   Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_ATTR_HAS_C
  TARGET(OP_ATTR_HAS_C) {
   RT_DEBUG_CHECK_STACK(1);
#ifdef DEE_DEBUG
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_CONST(rt_arg);
   rt_temp = frame.f_constv[rt_arg];
#else
   rt_temp = frame.f_constv[RT_READ_ARG()];
#endif
do_has_attr_c_rt_temp:
EXTERN_BEGIN;
   rt_itemp = DeeObject_HasAttr(RT_TOP,rt_temp);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp < 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_TOP = DeeBool_New(rt_itemp);
   DISPATCH();
  }
#endif

#ifdef OP_ATTR_DEL_C
  TARGET(OP_ATTR_DEL_C) {
   RT_DEBUG_CHECK_STACK(1);
#ifdef DEE_DEBUG
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_CONST(rt_arg);
   rt_temp = frame.f_constv[rt_arg];
#else
   rt_temp = frame.f_constv[RT_READ_ARG()];
#endif
do_del_attr_c_rt_temp:
EXTERN_BEGIN;
   rt_itemp = DeeObject_DelAttr(RT_TOP,rt_temp);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_ATTR_SET_C
  TARGET(OP_ATTR_SET_C) {
   RT_DEBUG_CHECK_STACK(2);
#ifdef DEE_DEBUG
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_CONST(rt_arg);
   rt_temp = frame.f_constv[rt_arg];
#else
   rt_temp = frame.f_constv[RT_READ_ARG()];
#endif
do_set_attr_c_rt_temp:
EXTERN_BEGIN;
   rt_itemp = DeeObject_SetAttr(RT_SECOND,rt_temp,RT_TOP);
EXTERN_END;
   rt_temp = RT_TOP;   RT_POP_1();
   Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   // the assigned value is now RT_TOP
   DISPATCH();
  }
#endif

#ifdef OP_DEL_LOCAL
  TARGET(OP_DEL_LOCAL) {
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_LOCAL(rt_arg);
   if DEE_LIKELY((rt_temp = frame.f_localv[rt_arg]) != NULL) {
    frame.f_localv[rt_arg] = NULL;
    Dee_DECREF(rt_temp);
   }
   DISPATCH();
  }
#endif

#ifdef OP_SEQ_ITER_WALK
  TARGET(OP_SEQ_ITER_WALK) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_itemp = DeeObject_IterNextEx(RT_TOP,&rt_temp);
EXTERN_END;
   if DEE_LIKELY(rt_itemp == 0) {
    RT_PUSH_INHERIT(rt_temp);
    RT_SKIP_ARG();
   } else {
    if DEE_UNLIKELY(rt_itemp < 0) RT_HANDLE_EXCEPTION;
    RT_CODE_ADDR() += (RT_PEEK_SARG()-1);
   }
   DISPATCH();
  }
#endif

#ifdef OP_CALL_MEMBER_C
  TARGET(OP_CALL_MEMBER_C) {
   RT_DEBUG_CHECK_STACK(2);
#ifdef DEE_DEBUG
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_CONST(rt_arg);
   rt_temp = frame.f_constv[rt_arg];
#else
   rt_temp = frame.f_constv[RT_READ_ARG()];
#endif
do_call_member_c_rt_temp:
   DEE_ASSERT(DeeTuple_Check(RT_TOP));
EXTERN_BEGIN;
   rt_temp = DeeObject_CallAttr(RT_SECOND,rt_temp,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP);
   Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

//////////////////////////////////////////////////////////////////////////
// Sequence concat opcodes
#ifdef OP_CONCAT_TUPLE
  TARGET(OP_CONCAT_TUPLE) {
   RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
   rt_temp = DeeTuple_Concat(RT_SECOND,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP);
   Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_CONCAT_LIST
  TARGET(OP_CONCAT_LIST) {
   RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
   rt_temp = DeeList_Concat(RT_SECOND,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP);
   Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_CAST_TUPLE
  TARGET(OP_CAST_TUPLE) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_temp = DeeTuple_Cast(RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP);
   Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_CAST_LIST
  TARGET(OP_CAST_LIST) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_temp = DeeList_Cast(RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP);
   Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_CAST_SET
  TARGET(OP_CAST_SET) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_temp = DeeSet_Cast(RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP);
   Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif


//////////////////////////////////////////////////////////////////////////
// Print opcodes
#ifdef OP_PRINT_ONE
  TARGET(OP_PRINT_ONE) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_temp = DeeFile_Std(DEE_STDOUT);
   rt_itemp = DeeFile_PrintOne(rt_temp,RT_TOP);
   Dee_DECREF(rt_temp);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_PRINT_ONE_SEP
  TARGET(OP_PRINT_ONE_SEP) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_temp = DeeFile_Std(DEE_STDOUT);
   rt_itemp = DeeFile_PrintOne(rt_temp,RT_TOP);
   if DEE_LIKELY(rt_itemp == 0) rt_itemp = DeeFile_PRINT(rt_temp," ");
   Dee_DECREF(rt_temp);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_PRINT_ONE_END
  TARGET(OP_PRINT_ONE_END) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_temp = DeeFile_Std(DEE_STDOUT);
   rt_itemp = DeeFile_PrintOne(rt_temp,RT_TOP);
   if DEE_LIKELY(rt_itemp == 0) rt_itemp = DeeFile_PRINT(rt_temp,"\n");
   Dee_DECREF(rt_temp);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_PRINT_ONE_F
  TARGET(OP_PRINT_ONE_F) {
   RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
   rt_itemp = DeeFile_PrintOne(RT_SECOND,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_PRINT_ONE_F_SEP
  TARGET(OP_PRINT_ONE_F_SEP) {
   RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
   rt_itemp = DeeFile_PrintOne(RT_SECOND,RT_TOP);
   if DEE_LIKELY(rt_itemp == 0 && DeeFile_Check(RT_SECOND)) rt_itemp = DeeFile_PRINT(RT_SECOND," ");
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_PRINT_ONE_F_END
  TARGET(OP_PRINT_ONE_F_END) {
   RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
   rt_itemp = DeeFile_PrintOne(RT_SECOND,RT_TOP);
   if DEE_LIKELY(rt_itemp == 0 && DeeFile_Check(RT_SECOND)) rt_itemp = DeeFile_PRINT(RT_SECOND,"\n");
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_PRINT_ALL
  TARGET(OP_PRINT_ALL) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_temp = DeeFile_Std(DEE_STDOUT);
   rt_itemp = DeeFile_PrintAll(rt_temp,RT_TOP);
   Dee_DECREF(rt_temp);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_PRINT_ALL_SEP
  TARGET(OP_PRINT_ALL_SEP) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_temp = DeeFile_Std(DEE_STDOUT);
   rt_itemp = DeeFile_PrintAll(rt_temp,RT_TOP);
   if DEE_LIKELY(rt_itemp == 0) rt_itemp = DeeFile_PRINT(rt_temp," ");
   Dee_DECREF(rt_temp);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_PRINT_ALL_END
  TARGET(OP_PRINT_ALL_END) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_temp = DeeFile_Std(DEE_STDOUT);
   rt_itemp = DeeFile_PrintAll(rt_temp,RT_TOP);
   if DEE_LIKELY(rt_itemp == 0) rt_itemp = DeeFile_PRINT(rt_temp,"\n");
   Dee_DECREF(rt_temp);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_PRINT_ALL_F
  TARGET(OP_PRINT_ALL_F) {
   RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
   rt_itemp = DeeFile_PrintAll(RT_SECOND,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_PRINT_ALL_F_SEP
  TARGET(OP_PRINT_ALL_F_SEP) {
   RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
   rt_itemp = DeeFile_PrintAll(RT_SECOND,RT_TOP);
   if DEE_LIKELY(rt_itemp == 0 && DeeFile_Check(RT_SECOND)) rt_itemp = DeeFile_PRINT(RT_SECOND," ");
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_PRINT_ALL_F_END
  TARGET(OP_PRINT_ALL_F_END) {
   RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
   rt_itemp = DeeFile_PrintAll(RT_SECOND,RT_TOP);
   if DEE_LIKELY(rt_itemp == 0 && DeeFile_Check(RT_SECOND)) rt_itemp = DeeFile_PRINT(RT_SECOND,"\n");
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_PRINT_END
  TARGET(OP_PRINT_END) {
EXTERN_BEGIN;
   rt_temp = DeeFile_Std(DEE_STDOUT);
   rt_itemp = DeeFile_PRINT(rt_temp,"\n");
   Dee_DECREF(rt_temp);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   DISPATCH();
  }
#endif

#ifdef OP_PRINT_END_F
  TARGET(OP_PRINT_END_F) {
   RT_DEBUG_CHECK_STACK(1);
   if DEE_UNLIKELY(DeeNone_Check(RT_TOP)) rt_itemp = 0;
   else {
EXTERN_BEGIN;
    if DEE_UNLIKELY(DeeError_TypeError_CheckType(RT_TOP,(
     DeeTypeObject *)&DeeFile_Type) != 0) rt_itemp = -1;
    else rt_itemp = DeeFile_PRINT(RT_TOP,"\n");
EXTERN_END;
   }
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
  }
#endif

#ifdef OP_CELL
  TARGET(OP_CELL) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   rt_temp = DeeCell_New(RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP);
   Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

//////////////////////////////////////////////////////////////////////////
// Sequence unpacking
#ifdef OP_UNPACK_TUPLE
  TARGET(OP_UNPACK_TUPLE) {
   DeeObject **elem;
   rt_arg = RT_READ_ARG();
   DEE_ASSERT(rt_arg != 0);
   RT_DEBUG_CHECK_STACK(1);
   rt_temp = RT_TOP;
   DEE_ASSERT(DeeTuple_Check(rt_temp));
unpack_tuple:
   if DEE_UNLIKELY(DeeTuple_SIZE(rt_temp) != rt_arg) {
EXTERN_BEGIN;
    _deert_unexpected_unpack_size(rt_arg,DeeTuple_SIZE(rt_temp));
EXTERN_END;
    RT_HANDLE_EXCEPTION;
   }
   Dee_INCREF(RT_TOP = *(elem = DeeTuple_ELEM(rt_temp)));
   while (--rt_arg) RT_PUSH(*++elem);
   Dee_DECREF(rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_UNPACK_LIST
  TARGET(OP_UNPACK_LIST) {
   DeeObject **elem;
   rt_arg = RT_READ_ARG();
   DEE_ASSERT(rt_arg != 0);
   RT_DEBUG_CHECK_STACK(1);
   rt_temp = RT_TOP;
unpack_list:
   DEE_ASSERT(DeeObject_Check(rt_temp) && DeeList_Check(rt_temp));
   // NOTE: This isn't a deadlock, since nothing
   //       that could potentially try to lock our frame
   //       could also hold a lock on the list.
   //       >> So we are safe to just go ahead and hold locks on both!
   DeeList_ACQUIRE(rt_temp);
   if DEE_UNLIKELY(DeeList_SIZE(rt_temp) != rt_arg) {
    Dee_size_t list_size;
    list_size = DeeList_SIZE(rt_temp);
    DeeList_RELEASE(rt_temp);
EXTERN_BEGIN;
    _deert_unexpected_unpack_size(rt_arg,list_size);
EXTERN_END;
    RT_HANDLE_EXCEPTION;
   }
   Dee_INCREF(RT_TOP = *(elem = DeeList_ELEM(rt_temp)));
   while (--rt_arg) RT_PUSH(*++elem);
   DeeList_RELEASE(rt_temp);
   Dee_DECREF(rt_temp);
   DISPATCH();
  }
#endif

#ifdef OP_UNPACK_SEQUENCE
  TARGET(OP_UNPACK_SEQUENCE) {
   rt_arg = RT_READ_ARG();
   DEE_ASSERT(rt_arg != 0);
   RT_DEBUG_CHECK_STACK(1);
   rt_temp = RT_TOP; // Optimizations for expected types
   if (DeeTuple_Check(rt_temp)) goto unpack_tuple;
   if (DeeList_Check(rt_temp)) goto unpack_list;
   if (DeeNone_Check(rt_temp)) {
    // 'none' can always be unpacked into more 'none' - yay!
#if 0
    Dee_DECREF(rt_temp);
    Dee_INCREF(RT_TOP = Dee_None);
#endif
    while (--rt_arg) RT_PUSH(Dee_None);
   } else {
    DeeObject *seq_iter,*seq_elem;
    Dee_uint16_t req_len;
    Dee_size_t seq_len_real = 0;
EXTERN_BEGIN;
    seq_iter = DeeObject_IterSelf(rt_temp);
    if DEE_UNLIKELY(seq_iter == NULL) {
EXTERN_END;
     RT_HANDLE_EXCEPTION;
    }
    if DEE_UNLIKELY((rt_itemp = DeeObject_IterNextEx(seq_iter,&seq_elem)) != 0) {
err_seq_inv_len:
     _DeeInternalObject_DECREF(seq_iter);
     if DEE_LIKELY(rt_itemp > 0) _deert_unexpected_unpack_size(rt_arg,seq_len_real);
EXTERN_END;
     RT_HANDLE_EXCEPTION;
    }
    _DeeInternalObject_DECREF(RT_TOP);
EXTERN_END;
    Dee_INHERIT_REF(RT_TOP,seq_elem);
    ++seq_len_real;
    req_len = rt_arg;
    while (--rt_arg) {
     ++seq_len_real;
EXTERN_BEGIN;
     rt_itemp = DeeObject_IterNextEx(seq_iter,&seq_elem);
EXTERN_END;
     if DEE_UNLIKELY(rt_itemp != 0) { rt_arg = req_len; goto err_seq_inv_len; }
     RT_PUSH_INHERIT(seq_elem);
    }
    // We got all we need, but to ensure integrity, we have to check
    // if there is actually more than we need (and if there is)
EXTERN_BEGIN;
    rt_itemp = DeeObject_IterNextEx(seq_iter,&seq_elem);
    if DEE_UNLIKELY(rt_itemp != 1) {
     if DEE_LIKELY(rt_itemp == 0) {
      _DeeInternalObject_DECREF(seq_elem);
      DeeError_RuntimeErrorf("Unpack: unexpected %q iterator size (Expected: %I16u, Got: >= %Iu)",
                             DeeType_NAME(Dee_TYPE(seq_iter)),
                             req_len,seq_len_real+1);
     }
     _DeeInternalObject_DECREF(seq_iter);
EXTERN_END;
     RT_HANDLE_EXCEPTION;
    }
    _DeeInternalObject_DECREF(seq_iter);
EXTERN_END;
    DEE_ASSERT(rt_itemp >= 0); // annotation...
   }
   DISPATCH();
  }
#endif


#ifdef OP_FOREIGN_FUNCTION
  TARGET(OP_FOREIGN_FUNCTION) {
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_STACK(1+rt_arg); // return_type+argv
EXTERN_BEGIN;
   rt_temp = (DeeObject *)DeeType_ForeignFunction((DeeTypeObject *)RT_STACK_SLOT(rt_arg),
                                                  (Dee_funflags_t)0,(Dee_size_t)rt_arg,
                                                  (DeeTypeObject const *const *)RT_STACK_ADDR(rt_arg));
EXTERN_END;
push_foreign_function:
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   while (rt_arg--) { Dee_DECREF(RT_TOP); RT_POP_1(); }
   Dee_DECREF(RT_TOP); // return type
   Dee_INCREF(RT_TOP = rt_temp); // function type
   DISPATCH();
#else /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
   RT_SKIP_ARG();
   goto on_unreachable;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
  }
#endif

//////////////////////////////////////////////////////////////////////////
// Function opcodes
#ifdef OP_FUNCTION
  TARGET(OP_FUNCTION) {
   rt_arg = RT_READ_ARG();
EXTERN_BEGIN;
   rt_temp = DeeFunction_New(RT_TOP,RT_SECOND,
                             (Dee_size_t)rt_arg,
                             DEE_FUNCTION_FLAG_NONE);
EXTERN_END;
rt_push_fun:
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP);
   Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
  }
#endif

//////////////////////////////////////////////////////////////////////////
// Class opcodes
#ifdef OP_CLASS_SET_SLOT
  TARGET(OP_CLASS_SET_SLOT) {
   RT_DEBUG_CHECK_STACK(2);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
   DEE_ASSERT(DeeObject_Check(RT_SECOND) && DeeClass_Check(RT_SECOND) && "Not a class");
   rt_arg = RT_READ_ARG();
EXTERN_BEGIN;
   rt_itemp = DeeClass_SetSlot((DeeTypeObject *)RT_SECOND,rt_arg,RT_TOP);
EXTERN_END;
   if (rt_itemp == -1) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
#else
   RT_SKIP_ARG();
   goto on_unreachable;
#endif
  }
#endif

#ifdef OP_CLASS_ADD_METHOD
  TARGET(OP_CLASS_ADD_METHOD) {
   RT_DEBUG_CHECK_STACK(2);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_CONST(rt_arg);
   rt_temp = frame.f_constv[rt_arg];
do_class_add_method_rt_temp:
EXTERN_BEGIN;
   DEE_ASSERTF(DeeObject_Check(RT_SECOND) && DeeClass_Check(RT_SECOND),
               "Slot 2 (instance of %q) is not a class",
               DeeType_NAME(Dee_TYPE(RT_SECOND)));
   rt_itemp = DeeClass_AddMethod((DeeTypeObject *)RT_SECOND,rt_temp,RT_TOP);
EXTERN_END;
   if (rt_itemp == -1) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
#else
   RT_SKIP_ARG();
   goto on_unreachable;
#endif
  }
#endif

#ifdef OP_CLASS_ADD_GETSET
  TARGET(OP_CLASS_ADD_GETSET) {
   RT_DEBUG_CHECK_STACK(4);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_CONST(rt_arg);
   rt_temp = frame.f_constv[rt_arg];
do_class_add_getset_rt_temp:
EXTERN_BEGIN;
   DEE_ASSERTF(DeeObject_Check(RT_FOURTH) && DeeClass_Check(RT_FOURTH),
               "Slot 4 (instance of %q) is not a class",
               DeeType_NAME(Dee_TYPE(RT_FOURTH)));
   rt_itemp = DeeClass_AddProperty((DeeTypeObject *)RT_FOURTH,rt_temp,
                                   DEE_LIKELY(RT_THIRD != Dee_EmptyString) ? RT_THIRD : NULL,
                                   DEE_LIKELY(RT_SECOND != Dee_EmptyString) ? RT_SECOND : NULL,
                                   DEE_LIKELY(RT_TOP != Dee_EmptyString) ? RT_TOP : NULL);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
#else
   RT_SKIP_ARG();
   goto on_unreachable;
#endif
  }
#endif

#ifdef OP_CLASS_ADD_CLASS_METHOD
  TARGET(OP_CLASS_ADD_CLASS_METHOD) {
   RT_DEBUG_CHECK_STACK(2);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_CONST(rt_arg);
   rt_temp = frame.f_constv[rt_arg];
do_class_add_class_method_rt_temp:
EXTERN_BEGIN;
   DEE_ASSERTF(DeeObject_Check(RT_SECOND) && DeeClass_Check(RT_SECOND),
               "Slot 2 (instance of %q) is not a class",
               DeeType_NAME(Dee_TYPE(RT_SECOND)));
   rt_itemp = DeeClass_AddClassMethod((DeeTypeObject *)RT_SECOND,
                                      rt_temp,RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
#else
   RT_SKIP_ARG();
   goto on_unreachable;
#endif
  }
#endif

#ifdef OP_CLASS_ADD_CLASS_GETSET
  TARGET(OP_CLASS_ADD_CLASS_GETSET) {
   RT_DEBUG_CHECK_STACK(4);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_VALID_CONST(rt_arg);
   rt_temp = frame.f_constv[rt_arg];
do_class_add_class_getset_rt_temp:
EXTERN_BEGIN;
   DEE_ASSERTF(DeeObject_Check(RT_FOURTH) && DeeClass_Check(RT_FOURTH),
               "Slot 4 (instance of %q) is not a class",
               DeeType_NAME(Dee_TYPE(RT_FOURTH)));
   rt_itemp = DeeClass_AddClassProperty((DeeTypeObject *)RT_FOURTH,rt_temp,
                                        DEE_LIKELY(RT_THIRD != Dee_EmptyString) ? RT_THIRD : NULL,
                                        DEE_LIKELY(RT_SECOND != Dee_EmptyString) ? RT_SECOND : NULL,
                                        DEE_LIKELY(RT_TOP != Dee_EmptyString) ? RT_TOP : NULL);
EXTERN_END;
   if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   DISPATCH();
#else
   RT_SKIP_ARG();
   goto on_unreachable;
#endif
  }
#endif

#ifdef OP_STACKALIGN
  TARGET(OP_STACKALIGN) {
#ifdef DEE_DEBUG
   rt_arg = RT_READ_ARG();
   RT_DEBUG_CHECK_STACK(rt_arg);
   rt_arg = (Dee_uint16_t)((stack_end-stack)-rt_arg);
#else
   rt_arg = (Dee_uint16_t)((stack_end-stack)-RT_READ_ARG());
#endif
   while (rt_arg--) { Dee_DECREF(RT_TOP); RT_POP_1(); }
   DISPATCH();
  }
#endif


//////////////////////////////////////////////////////////////////////////
// Exception handling
  //////////////////////////////////////////////////////////////////////////
  // Mockup of opcodes generated for a standard try - catch block
  //////////////////////////////////////////////////////////////////////////
  // try
  // @OP_TRY_BEGIN h_addr // Push a handler that leads to 'h_addr'
  // {
  //   ...
  // }
  // @OP_TRY_END          // Executes finally handlers / removes active exception handlers
  // @OP_JUMP tend_addr   // Jump to the end of all exception blocks (here only one)
  // catch (...)
  // {
  //h_addr:
  // @OP_TRY_BEGIN c_pop  // Begin a finally block to always execute OP_EXCEPT_END to remove the exception for the handling-list
  //
  //   ...
  //
  // @OP_TRY_END          // End the try block of user-code (only reached if no )
  // @OP_JUMP c_end       // Jump behind the handling-remove code
  //c_pop:
  // @OP_EXCEPT_END       // Remove the exception for the handling-list
  // @OP_FINALLY_END      // End the finally block
  //c_end:
  // }
  //tend_addr:
  //////////////////////////////////////////////////////////////////////////
#ifdef OP_TRY_BEGIN
  TARGET(OP_TRY_BEGIN) {
   DEE_ASSERTF(frame.f_exceptv_end < frame.f_exceptv+frame.f_code->co_exceptsize,
               "%O(%I32d) : +%.4Ix : Can't push more exception handlers (Maximum of %Iu has been reached)",
               _DeeStackFrame_File(&frame),_DeeStackFrame_Line(&frame)+1,
               _DeeStackFrame_Addr(&frame),frame.f_code->co_exceptsize);
#ifdef DEE_DEBUG
   rt_arg = RT_READ_ARG();
   DEE_ASSERTF(rt_arg < frame.f_code->co_exceptc,
               "%O(%I32d) : +%.4Ix : Invalid exception handler ID (%I16u exceeds maximum of %Iu)",
               _DeeStackFrame_File(&frame),_DeeStackFrame_Line(&frame)+1,
               _DeeStackFrame_Addr(&frame),rt_arg,frame.f_code->co_exceptc);
   (frame.f_exceptv_end++)->r_handler = frame.f_code->co_exceptv+rt_arg;
#else
   (frame.f_exceptv_end++)->r_handler = frame.f_code->co_exceptv+RT_READ_ARG();
#endif
   DISPATCH();
  }
#endif

#ifdef OP_TRY_END
  TARGET(OP_TRY_END) {
   DEE_ASSERTF(frame.f_exceptv_end != frame.f_exceptv,
               "%O(%I32d) : Can't pop exception handler (no handlers have been pushed)",
               _DeeStackFrame_File(&frame),_DeeStackFrame_Line(&frame)+1);
   handler = (--frame.f_exceptv_end)->r_handler;
   if (handler->e_kind == DeeExceptionHandleKind_FINALLY) {
    DEE_ASSERTF(_DeeStackFrame_FINALLY_RETURN_ADDRESS_MAX_SIZE(frame) != 
                _DeeStackFrame_FINALLY_RETURN_ADDRESS_SIZE(frame),
                "%O(%I32d) : Finally return address buffer is full",
                _DeeStackFrame_File(&frame),_DeeStackFrame_Line(&frame)+1);
    // Store the address of the next opcode as
    // the return address to the finally block.
    // >> That way, once finally returns, code will continue to execute normally
    *frame.f_finally_return_address_end++ = code;
    goto fast_exec_handler_novar; // Execute the handler
   }
   DISPATCH();
  }
#endif

#ifdef OP_FINALLY_END
  TARGET(OP_FINALLY_END) {
   // End of exception block (finally)
   // Continue executing where the finally ended (code end / OP_TRY_END)
   DEE_ASSERTF(_DeeStackFrame_FINALLY_RETURN_ADDRESS_SIZE(frame),
               "%O(%I32d) : In OP_FINALLY_END without return address",
               _DeeStackFrame_File(&frame),_DeeStackFrame_Line(&frame)+1);
#if 1
   // - Restore stored exceptions and handle them if they exist
   // -> This is required for code that throws multiple exceptions at once
   // NOTE: We can't restore stored exceptions before this point (like in 'OP_EXCEPT_END'),
   //       because then the finally block wouldn't get torn down, leaving
   //       a dangling finally-return-address that would sooner or later cause some major problems.
   //       -> By restoring exceptions at the end of a finally block (aka. now),
   //          we basically do the right thing and say that executing a
   //          finally block might not return, if an exception was thrown.
   //       -> And that is exactly what should be happening!
   if DEE_UNLIKELY(frame.f_stored_exceptions) {
    // NOTE: Still consume the finally return address though
    --frame.f_finally_return_address_end;
    goto retore_exceptions;
   }
#endif
   // Jump to the finally return address
   code = *--frame.f_finally_return_address_end;
   DISPATCH();
  }
#endif

#ifdef OP_EXCEPT_END
  TARGET(OP_EXCEPT_END) {
   // End of exception block (catch)
EXTERN_BEGIN;
   // Drop the exception we were handling before
   _DeeError_DropOneHandlingException((DeeObject *)thread_self);
EXTERN_END;
   // v Very likely case, as except is followed by finally if
   //   usercode that could have thrown an exception was executed.
   //   >> try {
   //   >>   throws_an_error();
   //   >> } catch (...) {
   //   >>   // Since 'print' can throw another error, there is a
   //   >>   // hidden try-finally-block around this print, that will
   //   >>   // tear down the exception handler if it fails as well.
   //   >>   print "Error!";
   //   >> }
   PREDICT(OP_FINALLY_END);
   DISPATCH();
  }
#endif

#ifdef OP_THROW
  TARGET(OP_THROW) {
   RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
   DeeError_Throw(RT_TOP);
EXTERN_END;
   Dee_DECREF(RT_TOP); RT_POP_1();
   RT_HANDLE_EXCEPTION;
  }
#endif

#ifdef OP_RETHROW
  TARGET(OP_RETHROW) {
   struct DeeRaisedException *old_exception;
EXTERN_BEGIN;
   old_exception = thread_self->t_handling_exception;
   if DEE_LIKELY(old_exception) {
    struct DeeRaisedException *new_exception;
    if DEE_LIKELY((new_exception = _DeeRaisedException_Alloc()) != NULL) {
     new_exception->re_prev = thread_self->t_exception;
     _DeeRaisedException_InitCopy(new_exception,old_exception);
     DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
     thread_self->t_exception = new_exception;
     DeeAtomicMutex_Release(&thread_self->t_exception_lock);
    }
   } else {
    // Nothing is wrong? Then let's make it be wrong (raise an error if there is no error... XD)
    // But seriously: This is just so we can simply say that rethrow always throws an exception.
    // Because having rethrow return under special circumstances would break a lot of assumptions.
    DeeError_Throw((DeeObject *)&_DeeError_RuntimeError_NoActiveException);
   }
EXTERN_END;
   RT_HANDLE_EXCEPTION;
  }
#endif

#ifdef OP_LOAD_EXTERN
  TARGET(OP_LOAD_EXTERN) {
   RT_DEBUG_CHECK_STACK(3);
#if DEE_CONFIG_RUNTIME_HAVE_EXTERN
EXTERN_BEGIN;
   rt_temp = DeeSharedLib_LoadExtern(RT_THIRD,RT_SECOND,(DeeTypeObject *)RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
#else /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */
   goto on_unreachable;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_EXTERN */
  }
#endif

#ifdef OP_LOAD_AUTOEXTERN
  TARGET(OP_LOAD_AUTOEXTERN) {
   RT_DEBUG_CHECK_STACK(2);
#if DEE_CONFIG_RUNTIME_HAVE_EXTERN
EXTERN_BEGIN;
   rt_temp = DeeSharedLib_LoadAutoExtern(RT_SECOND,(DeeTypeObject *)RT_TOP);
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
#else /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */
   goto on_unreachable;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_EXTERN */
  }
#endif

#ifdef OP_FF_CLOSURE
  TARGET(OP_FF_CLOSURE) {
   RT_DEBUG_CHECK_STACK(2);
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
EXTERN_BEGIN;
   if (DeeError_TypeError_CheckType(RT_SECOND,&DeeForeignFunctionType_Type) == 0) {
    rt_temp = DeeForeignFunctionClosure_New((DeeTypeObject *)RT_SECOND,RT_TOP);
   } else rt_temp = NULL;
EXTERN_END;
   if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
   Dee_DECREF(RT_TOP); RT_POP_1();
   Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);
   DISPATCH();
#else /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */
   goto on_unreachable;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */
  }
#endif

#ifdef OP_UNREACHABLE
  TARGET(OP_UNREACHABLE) {
on_unreachable:
   // Step #0: unlock our frame and throw the unreachable exception
   DeeAtomicMutex_Release(&frame.f_lock);
   DeeError_Throw((DeeObject *)&_DeeRuntimeError_Unreachable);

   // Now for the hard of manually cleaning everything
   // Step #1: Unlink our frame (so we can start cleaning up)
   DEE_ASSERTF(thread_self->t_frame_last == &frame,
               "%O(%I32d) : Invalid 't_frame_last' in thread_self",
               _DeeStackFrame_File(&frame),_DeeStackFrame_Line(&frame)+1);
   DeeAtomicMutex_Acquire(&thread_self->t_frame_lock);
   thread_self->t_frame_last = frame.f_prev;
#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
   // Release one from the frame size counter
   --thread_self->t_frame_size;
#endif /* DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS */
   DeeAtomicMutex_Release(&thread_self->t_frame_lock);

   // Step #2: Restore stored exceptions
   if (frame.f_stored_exceptions) {
    struct DeeRaisedException *except_iter;
    except_iter = thread_self->t_exception;
    if (except_iter) {
     while (except_iter->re_prev) except_iter = except_iter->re_prev;
     DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
     except_iter->re_prev = frame.f_stored_exceptions;
     DeeAtomicMutex_Release(&thread_self->t_exception_lock);
    } else {
     DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
     thread_self->t_exception = frame.f_stored_exceptions;
     DeeAtomicMutex_Release(&thread_self->t_exception_lock);
    }
   }
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
   // Step #3: Check if the unreachable was encountered inside of a noexcept catch block
   while (1) {
    if (code >= frame.f_code->co_code+frame.f_code->co_size) break;
    switch (*code++) {
     case OP_RETNONE:
     case OP_RET:
     case OP_STOP:
     case OP_RETVAREXIT:
     case OP_YIELDSTOP:
     case OP_FINALLY_END:
      goto unreachable_step_4;
     case OP_EXCEPT_END:
      // Yes, it did happen in a noexcept catch block
      // Drop the exception we were handling before
      // TODO: We assume a little bit too much about how bytecode
      //       is structured here:
      //       - What if there's a condition jump that would skip the end of a except block?
      //         Sure: That's impossible to write in usercode, but most definitely
      //               possible to produce with bytecode. And with the introduction
      //               of a working marshal, quite possible to hack together.
      //         --> Not really a problem now, but once the sandbox is done, it
      //             will need some way of safely tearing down handling-exceptions.
      // TODO: Nevermind... This one's just totally broken...
      _DeeError_DropOneHandlingException((DeeObject *)thread_self);
      goto unreachable_step_4;
     case OP_JUMP:
      if (RT_PEEK_SARG() > 0) {
       RT_CODE_ADDR() += (RT_PEEK_SARG()-1);
       break;
      }
     default:
      if ((code[-1]&OPFLAG_ARG)!=0)RT_SKIP_ARG();
    }
   }
unreachable_step_4:

   // Step #4: Cleanup additional exception handlers
   while (frame.f_exceptv_end != frame.f_exceptv) {
    Dee_uint8_t *handler_code;
    handler = (--frame.f_exceptv_end)->r_handler; // Consume one handler
    DEE_ASSERTF(frame.f_exceptv_end >= frame.f_exceptv &&
                frame.f_exceptv_end < frame.f_exceptv+frame.f_code->co_exceptsize,
                "Frame exception handlers end is out-of-bounds.\n"
                "Active handler address %p isn't in the range of %p...%p",
                frame.f_exceptv_end,frame.f_exceptv,frame.f_exceptv+frame.f_code->co_exceptsize);
    if (handler->e_kind != DeeExceptionHandleKind_FINALLY) continue; // Inappropriate handler
    handler_code = frame.f_code->co_code+handler->e_handler;
    // This finally block was supposed to clean a 
    if (*handler_code == OP_EXCEPT_END) {
     // Drop additional exceptions that were supposed to be removed by finally blocks
     _DeeError_DropOneHandlingException((DeeObject *)thread_self);
    }
   }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */

   // Step #5: Cleanup the stack
   while (stack_end != stack) { --stack_end; _DeeInternalObject_DECREF((*stack_end)); }

   // Step #6: Local variables are cleaned at the other end of this goto!
   goto end_local_clean;
  }
#endif
   

#ifdef OP_PREFIX
  TARGET(OP_PREFIX) {
   Dee_uint16_t specarg = RT_READ_ARG();
   switch (RT_READ_OP()) {

#ifdef OP_FUNCTION
    case OP_FUNCTION:
     rt_arg = RT_READ_ARG();
EXTERN_BEGIN;
     rt_temp = DeeFunction_New(RT_TOP,RT_SECOND,
                               (Dee_size_t)rt_arg,
                               (Dee_uint32_t)specarg);
EXTERN_END;
     goto rt_push_fun;
#endif

#ifdef OP_FOREIGN_FUNCTION
    case OP_FOREIGN_FUNCTION:
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
     rt_arg = RT_READ_ARG();
     RT_DEBUG_CHECK_STACK(1+rt_arg); // return_type+argv
EXTERN_BEGIN;
     rt_temp = (DeeObject *)DeeType_ForeignFunction(
      (DeeTypeObject *)RT_STACK_SLOT(rt_arg),(Dee_funflags_t)specarg,
      (Dee_size_t)rt_arg,(DeeTypeObject const *const *)RT_STACK_ADDR(rt_arg));
EXTERN_END;
     goto push_foreign_function;
#else /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
     goto on_unreachable;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#endif

//////////////////////////////////////////////////////////////////////////
// Extended local/const/ref/arg slot access
//////////////////////////////////////////////////////////////////////////
#define RT_READ_ARG32()  (((Dee_uint32_t)specarg << 16) | (Dee_uint32_t)RT_READ_ARG())
#define RT_PEEK_ARG32()  (((Dee_uint32_t)specarg << 16) | (Dee_uint32_t)RT_PEEK_ARG())
#define RT_LAST_ARG32()  (((Dee_uint32_t)specarg << 16) | (Dee_uint32_t)RT_LAST_ARG())
#define RT_READ_SARG32() (Dee_int32_t)(((Dee_uint32_t)specarg << 16) | (Dee_uint32_t)RT_READ_ARG())
#define RT_PEEK_SARG32() (Dee_int32_t)(((Dee_uint32_t)specarg << 16) | (Dee_uint32_t)RT_PEEK_ARG())
#define RT_LAST_SARG32() (Dee_int32_t)(((Dee_uint32_t)specarg << 16) | (Dee_uint32_t)RT_LAST_ARG())
#define RT_SKIP_ARG32    RT_SKIP_ARG
#define RT_PREV_ARG32    RT_PREV_ARG

     {
     Dee_uint32_t rt_arg32;
#ifdef OP_LOAD_LOC
    case OP_LOAD_LOC:
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_LOCAL32(rt_arg32);
     rt_temp = frame.f_localv[rt_arg32];
     if DEE_UNLIKELY(!rt_temp) {
EXTERN_BEGIN;
      // v this would probably be likely, but lets
      //   optimize this for code without debug informations
      if DEE_UNLIKELY(frame.f_code->co_debug) {
unbound_local_rtarg32:
       rt_temp = (DeeObject *)frame.f_code->co_debug->di_local_names;
       if DEE_UNLIKELY(!rt_temp) goto unbound_local_default;
       if DEE_UNLIKELY(rt_arg32 >= DeeTuple_SIZE(frame.f_code->co_debug->di_local_names)) goto unbound_local_default;
       rt_temp = DeeTuple_GET(rt_temp,rt_arg32);
       if DEE_UNLIKELY(!DeeString_Check(rt_temp)) goto unbound_local_default;
       DeeError_SetStringf(&DeeErrorType_UnboundLocal,"Unbound local: %q",DeeString_STR(rt_temp));
      } else goto unbound_local_default;
EXTERN_END;
      RT_PUSH(Dee_None);
      RT_HANDLE_EXCEPTION;
     }
     RT_PUSH(rt_temp);
     break;
#endif

#ifdef OP_LOAD_REF
    case OP_LOAD_REF:
#ifdef DEE_DEBUG
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_REF32(rt_arg32);
     RT_PUSH(frame.f_refv[rt_arg32]);
#else
     RT_PUSH(frame.f_refv[RT_READ_ARG32()]);
#endif
     break;
#endif

#ifdef OP_LOAD_CST
    case OP_LOAD_CST:
#ifdef DEE_DEBUG
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_CONST32(rt_arg32);
     RT_PUSH(frame.f_constv[rt_arg32]);
#else
     RT_PUSH(frame.f_constv[RT_READ_ARG32()]);
#endif
     break;
#endif

#ifdef OP_LOAD_CST_LOCKED
    case OP_LOAD_CST_LOCKED:
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_CONST(rt_arg32);
EXTERN_BEGIN;
     DeeAtomicMutex_Acquire(&frame.f_code->co_consts_lock);
     Dee_INCREF(rt_temp = frame.f_constv[rt_arg32]);
     DeeAtomicMutex_Release(&frame.f_code->co_consts_lock);
EXTERN_END;
     RT_PUSH_INHERIT(rt_temp);
     break;
#endif

#ifdef OP_LOAD_CST_COPY
    case OP_LOAD_CST_COPY: {
#ifdef DEE_DEBUG
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_CONST(rt_arg32);
     rt_temp = frame.f_constv[rt_arg32];
#else
     rt_temp = frame.f_constv[RT_READ_ARG32()];
#endif
     goto push_copy_rt_temp;
    } break;
#endif

#ifdef OP_LOAD_CST_DCOPY
    case OP_LOAD_CST_DCOPY: {
#ifdef DEE_DEBUG
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_CONST(rt_arg32);
     rt_temp = frame.f_constv[rt_arg32];
#else
     rt_temp = frame.f_constv[RT_READ_ARG32()];
#endif
     goto push_deep_copy_rt_temp;
    } break;
#endif

#ifdef OP_LOAD_ARG
    case OP_LOAD_ARG:
#ifdef DEE_DEBUG
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_ARG32(rt_arg32);
     RT_PUSH(frame.f_argv[rt_arg32]);
#else
     RT_PUSH(frame.f_argv[RT_READ_ARG32()]);
#endif
     break;
#endif

#ifdef OP_STORE_LOC
    case OP_STORE_LOC:
     RT_DEBUG_CHECK_STACK(1);
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_LOCAL32(rt_arg32);
     rt_temp = frame.f_localv[rt_arg32];
     Dee_INCREF(frame.f_localv[rt_arg32] = RT_TOP);
     Dee_XDECREF(rt_temp);
     break;
#endif

#ifdef OP_STORE_CST
    case OP_STORE_CST:
     RT_DEBUG_CHECK_STACK(1);
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_CONST32(rt_arg32);
     rt_temp = frame.f_constv[rt_arg32];
     Dee_INCREF(frame.f_constv[rt_arg32] = RT_TOP);
     Dee_XDECREF(rt_temp);
     break;
#endif

#ifdef OP_DEL_LOCAL
    case OP_DEL_LOCAL:
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_LOCAL32(rt_arg32);
     if DEE_LIKELY((rt_temp = frame.f_localv[rt_arg32]) != NULL) {
      frame.f_localv[rt_arg32] = NULL;
      Dee_DECREF(rt_temp);
     }
     break;
#endif

#ifdef OP_ATTR_GET_C
    case OP_ATTR_GET_C:
     RT_DEBUG_CHECK_STACK(1);
#ifdef DEE_DEBUG
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_CONST32(rt_arg32);
     rt_temp = frame.f_constv[rt_arg32];
#else
     rt_temp = frame.f_constv[RT_READ_ARG32()];
#endif
     goto do_get_attr_c_rt_temp;
#endif

#ifdef OP_ATTR_DEL_C
    case OP_ATTR_DEL_C:
     RT_DEBUG_CHECK_STACK(1);
#ifdef DEE_DEBUG
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_CONST32(rt_arg32);
     rt_temp = frame.f_constv[rt_arg32];
#else
     rt_temp = frame.f_constv[RT_READ_ARG32()];
#endif
     goto do_del_attr_c_rt_temp;
#endif

#ifdef OP_ATTR_SET_C
    case OP_ATTR_SET_C:
     RT_DEBUG_CHECK_STACK(2);
#ifdef DEE_DEBUG
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_CONST32(rt_arg32);
     rt_temp = frame.f_constv[rt_arg32];
#else
     rt_temp = frame.f_constv[RT_READ_ARG32()];
#endif
     goto do_set_attr_c_rt_temp;
#endif

#ifdef OP_ATTR_HAS_C
    case OP_ATTR_HAS_C:
     RT_DEBUG_CHECK_STACK(1);
#ifdef DEE_DEBUG
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_CONST32(rt_arg32);
     rt_temp = frame.f_constv[rt_arg32];
#else
     rt_temp = frame.f_constv[RT_READ_ARG32()];
#endif
     goto do_has_attr_c_rt_temp;
#endif

#ifdef OP_CALL_MEMBER_C
    case OP_CALL_MEMBER_C:
     RT_DEBUG_CHECK_STACK(2);
#ifdef DEE_DEBUG
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_CONST32(rt_arg32);
     rt_temp = frame.f_constv[rt_arg32];
#else
     rt_temp = frame.f_constv[RT_READ_ARG32()];
#endif
     goto do_call_member_c_rt_temp;
#endif

#ifdef OP_TUPLE
    case OP_TUPLE:
     rt_arg32 = RT_READ_ARG32();
     DEE_ASSERTF(rt_arg32 != 0,"OP_TUPLE can't create empty tuples");
     RT_DEBUG_CHECK_STACK32(rt_arg32);
EXTERN_BEGIN;
     rt_temp = _DeeTuple_NewUnsafe(rt_arg32);
EXTERN_END;
     if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
     memcpy(DeeTuple_ELEM(rt_temp),RT_STACK_ADDR32(rt_arg32),
            rt_arg32*sizeof(DeeObject *)); // inherit references
     RT_POP_N(rt_arg32-1);
     Dee_INHERIT_REF(RT_TOP,rt_temp);
     break;
#endif

#ifdef OP_LIST
    case OP_LIST:
     rt_arg32 = RT_READ_ARG32();
     DEE_ASSERTF(rt_arg32 != 0,"OP_LIST can't create empty lists");
     RT_DEBUG_CHECK_STACK32(rt_arg32);
EXTERN_BEGIN;
     rt_temp = _DeeList_NewUnsafe(rt_arg32);
EXTERN_END;
     if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
     memcpy(DeeList_ELEM(rt_temp),RT_STACK_ADDR32(rt_arg32),
            rt_arg32*sizeof(DeeObject *)); // inherit references
     RT_POP_N(rt_arg32-1);
     Dee_INHERIT_REF(RT_TOP,rt_temp);
     break;
#endif

#ifdef OP_DICT
    case OP_DICT:
     rt_arg32 = RT_READ_ARG32();
     DEE_ASSERTF(rt_arg32 != 0,"OP_DICT can't create empty dicts");
     RT_DEBUG_CHECK_STACK32(rt_arg32*2);
EXTERN_BEGIN;
     rt_temp = DeeDict_NewFromKeyValuePairs(rt_arg32,RT_STACK_ADDR32(rt_arg32*2));
EXTERN_END;
     if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
     do {
      Dee_DECREF(RT_TOP); RT_POP_1();
      Dee_DECREF(RT_TOP); RT_POP_1();
     } while (--rt_arg32);
     RT_PUSH_INHERIT(rt_temp);
     break;
#endif

#ifdef OP_SET
    case OP_SET:
     rt_arg32 = RT_READ_ARG32();
     DEE_ASSERTF(rt_arg32 != 0,"OP_SET can't create empty sets");
     RT_DEBUG_CHECK_STACK32(rt_arg32);
EXTERN_BEGIN;
     rt_temp = DeeSet_NewFromKeys(rt_arg32,RT_STACK_ADDR32(rt_arg32));
EXTERN_END;
     if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
     do {
      Dee_DECREF(RT_TOP); RT_POP_1();
     } while (--rt_arg32);
     RT_PUSH_INHERIT(rt_temp);
     break;
#endif

#ifdef OP_CLASS_ADD_METHOD
    case OP_CLASS_ADD_METHOD:
     RT_DEBUG_CHECK_STACK(2);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_CONST32(rt_arg32);
     rt_temp = frame.f_constv[rt_arg32];
     goto do_class_add_method_rt_temp;
#else
     RT_SKIP_ARG();
     goto on_unreachable;
#endif
#endif

#ifdef OP_CLASS_ADD_GETSET
    case OP_CLASS_ADD_GETSET:
     RT_DEBUG_CHECK_STACK(4);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_CONST32(rt_arg32);
     rt_temp = frame.f_constv[rt_arg32];
     goto do_class_add_getset_rt_temp;
#else
     RT_SKIP_ARG();
     goto on_unreachable;
#endif
#endif

#ifdef OP_CLASS_ADD_CLASS_METHOD
    case OP_CLASS_ADD_CLASS_METHOD:
     RT_DEBUG_CHECK_STACK(2);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_CONST32(rt_arg32);
     rt_temp = frame.f_constv[rt_arg32];
     goto do_class_add_class_method_rt_temp;
#else
     RT_SKIP_ARG();
     goto on_unreachable;
#endif
#endif

#ifdef OP_CLASS_ADD_CLASS_GETSET
    case OP_CLASS_ADD_CLASS_GETSET:
     RT_DEBUG_CHECK_STACK(4);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_VALID_CONST32(rt_arg32);
     rt_temp = frame.f_constv[rt_arg32];
     goto do_class_add_class_getset_rt_temp;
#else
     RT_SKIP_ARG();
     goto on_unreachable;
#endif
#endif

#ifdef OP_JUMP
    case OP_JUMP:
     RT_CODE_ADDR() += (RT_PEEK_SARG32()-4);
     break;
#endif

#ifdef OP_JUMP_IF_TT_POP
    case OP_JUMP_IF_TT_POP:
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     rt_itemp = DeeObject_Bool(RT_TOP);
EXTERN_END;
     Dee_DECREF(RT_TOP); RT_POP_1();
     if (rt_itemp) {
      if DEE_UNLIKELY(rt_itemp < 0) {
skip_arg32_err: RT_SKIP_ARG32(); RT_HANDLE_EXCEPTION;
      }
      RT_CODE_ADDR() += (RT_PEEK_SARG32()-4);
     } else RT_SKIP_ARG32();
     break;
#endif

#ifdef OP_JUMP_IF_FF_POP
    case OP_JUMP_IF_FF_POP:
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     rt_itemp = DeeObject_Bool(RT_TOP);
EXTERN_END;
     Dee_DECREF(RT_TOP); RT_POP_1();
     if (!rt_itemp) RT_CODE_ADDR() += (RT_PEEK_SARG32()-4);
     else {
      if DEE_UNLIKELY(rt_itemp < 0) goto skip_arg32_err;
      RT_SKIP_ARG32();
     }
     break;
#endif

#ifdef OP_JUMP_IF_CASE_COMPARE
    case OP_JUMP_IF_CASE_COMPARE:
     RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
     rt_itemp = DeeObject_CompareEq(RT_TOP,RT_SECOND);
EXTERN_END;
     if DEE_UNLIKELY(rt_itemp < 0 && !DeeError_Catch(
      &DeeErrorType_NotImplemented)) goto skip_arg32_err;
     Dee_DECREF(RT_TOP); RT_POP_1();
     if DEE_UNLIKELY(rt_itemp > 0) RT_CODE_ADDR() += (RT_PEEK_SARG32()-4);
     else RT_SKIP_ARG32();
     break;
#endif

#ifdef OP_JUMP_IF_RCASE_COMPARE
    case OP_JUMP_IF_RCASE_COMPARE:
     RT_DEBUG_CHECK_STACK(3);
EXTERN_BEGIN;
     rt_itemp = DeeObject_CompareLe(RT_SECOND,RT_THIRD);
     if (rt_itemp == 1) rt_itemp = DeeObject_CompareGe(RT_TOP,RT_THIRD);
EXTERN_END;
     if DEE_UNLIKELY(rt_itemp < 0 && !DeeError_Catch(
      &DeeErrorType_NotImplemented)) goto skip_arg32_err;
     Dee_DECREF(RT_TOP); RT_POP_1();
     Dee_DECREF(RT_TOP); RT_POP_1();
     if DEE_UNLIKELY(rt_itemp > 0) RT_CODE_ADDR() += (RT_PEEK_SARG32()-4);
     else RT_SKIP_ARG();
     break;
#endif

#ifdef OP_SEQ_ITER_WALK
    case OP_SEQ_ITER_WALK:
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     rt_itemp = DeeObject_IterNextEx(RT_TOP,&rt_temp);
EXTERN_END;
     if DEE_LIKELY(rt_itemp == 0) {
      RT_PUSH_INHERIT(rt_temp);
      RT_SKIP_ARG32();
     } else {
      if DEE_UNLIKELY(rt_itemp < 0) RT_HANDLE_EXCEPTION;
      RT_CODE_ADDR() += (RT_PEEK_SARG32()-4);
     }
     break;
#endif

#ifdef OP_LROT_N
   // ab...yz -> b...yza
    case OP_LROT_N:
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_STACK32(rt_arg32+1);
     rt_temp = *(stack_end-(rt_arg32+1));
     memmove(stack_end-(rt_arg32+1),stack_end-rt_arg32,rt_arg32*sizeof(DeeObject *));
     stack_end[-1] = rt_temp;
     break;
#endif

#ifdef OP_RROT_N
    case OP_RROT_N:
     // ab...yz -> zab...y
     rt_arg32 = RT_READ_ARG32();
     RT_DEBUG_CHECK_STACK(rt_arg32+1);
     rt_temp = stack_end[-1];
     memmove(stack_end-rt_arg32,stack_end-(rt_arg32+1),rt_arg32*sizeof(DeeObject *));
     *(stack_end-(rt_arg32+1)) = rt_temp;
     break;
#endif

#ifdef OP_TRY_BEGIN
    case OP_TRY_BEGIN:
     DEE_ASSERTF(frame.f_exceptv_end < frame.f_exceptv+frame.f_code->co_exceptsize,
                 "%O(%I32d) : Can't push more exception handlers (Maximum of %Iu has been reached)",
                 _DeeStackFrame_File(&frame),_DeeStackFrame_Line(&frame)+1,
                 frame.f_code->co_exceptsize);
#ifdef DEE_DEBUG
     rt_arg32 = RT_READ_ARG32();
     DEE_ASSERTF(rt_arg32 < frame.f_code->co_exceptc,
                 "%O(%I32d) : Invalid exception handler ID (%I32u exceeds maximum of %Iu)",
                 _DeeStackFrame_File(&frame),_DeeStackFrame_Line(&frame)+1,
                 rt_arg32,frame.f_code->co_exceptc);
     (frame.f_exceptv_end++)->r_handler = frame.f_code->co_exceptv+rt_arg32;
#else
     (frame.f_exceptv_end++)->r_handler = frame.f_code->co_exceptv+RT_READ_ARG32();
#endif
     break;
#endif

     // OP_PREFIX+OP_PREFIX
     case OP_PREFIX: {
      rt_arg32 = RT_READ_ARG32();
      switch (RT_READ_OP()) {
#ifdef OP_EXTENDED
       case OP_EXTENDED: {
        rt_arg = RT_READ_ARG();
        switch (rt_arg) {
         case OPEXT_BOUND_LOCAL:
#if DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BOUND
          RT_DEBUG_CHECK_VALID_LOCAL32(rt_arg32);
          RT_PUSH(DeeBool_Const(frame.f_localv[rt_arg32] != NULL));
#else /* DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BOUND */
          goto on_unreachable;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BOUND */
          break;

         default:
          RT_PREV_ARG();
          goto ill_instr;
        }
       } break;
#endif
       default: goto ill_instr;
      }
     } break;

    }
#undef RT_READ_ARG32
#undef RT_PEEK_ARG32
#undef RT_LAST_ARG32
#undef RT_PEEK_SARG32
#undef RT_READ_SARG32
#undef RT_LAST_SARG32
#undef RT_SKIP_ARG32
#undef RT_PREV_ARG32

#ifdef OP_EXTENDED
    case OP_EXTENDED: {
     rt_arg = RT_READ_ARG();
     switch (rt_arg) {

#ifdef OPEXT_BOUND_LOCAL
      case OPEXT_BOUND_LOCAL:
#if DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BOUND
       RT_DEBUG_CHECK_VALID_LOCAL(specarg);
       RT_PUSH(DeeBool_Const(frame.f_localv[specarg] != NULL));
#else /* DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BOUND */
       goto on_unreachable;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BOUND */
       break;
#endif

#ifdef OPEXT_TYPEFLAGS_LO
     case OPEXT_TYPEFLAGS_LO:
      RT_DEBUG_CHECK_STACK(1);
      DEE_ASSERT(DeeType_Check(RT_TOP));
      DeeType_GET_SLOT(RT_TOP,tp_flags) |= (((Dee_uint32_t)
       specarg) & DEE_UINT32_C(0x0000FFFF))&DEE_TYPE_FLAG_PUBLIC_MASK;
      break;
#endif

#ifdef OPEXT_TYPEFLAGS_HI
     case OPEXT_TYPEFLAGS_HI:
      RT_DEBUG_CHECK_STACK(1);
      DEE_ASSERT(DeeType_Check(RT_TOP));
      DeeType_GET_SLOT(RT_TOP,tp_flags) |= (((Dee_uint32_t)
       specarg << 16) & DEE_UINT32_C(0xFFFF0000))&DEE_TYPE_FLAG_PUBLIC_MASK;
      break;
#endif

      default:
       RT_PREV_ARG();
       goto ill_instr;
     }
    } break;
#endif

    default: goto ill_instr;
   }
   DISPATCH();
  }
#endif

#ifdef OP_EXTENDED
  TARGET(OP_EXTENDED) {
   switch (RT_READ_ARG()) {

#ifdef OPEXT_TRY_INPLACE_ADD
    {
     DeeObject *temp_var;
    case OPEXT_TRY_INPLACE_ADD:
     RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
     Dee_INCREF(temp_var = RT_SECOND);
     rt_itemp = DeeObject_InplaceAddVar(&temp_var,RT_TOP);
EXTERN_END;
check_err_and_set_tempvar_x2:
     if DEE_UNLIKELY(rt_itemp != 0) { Dee_DECREF(temp_var); RT_HANDLE_EXCEPTION; }
     rt_temp = RT_TOP;
     Dee_INCREF(RT_TOP = temp_var);
     Dee_DECREF(rt_temp);
     rt_temp = RT_SECOND;
     Dee_INHERIT_REF(RT_SECOND,temp_var);
     Dee_DECREF(rt_temp);
     break;
#define MAKE_INPLACE_TRY_CODE(opfunc)\
     RT_DEBUG_CHECK_STACK(2);\
EXTERN_BEGIN;\
     Dee_INCREF(temp_var = RT_SECOND);\
     rt_itemp = opfunc(&temp_var,RT_TOP);\
EXTERN_END;\
     goto check_err_and_set_tempvar_x2
    case OPEXT_TRY_INPLACE_SUB: MAKE_INPLACE_TRY_CODE(DeeObject_InplaceSubVar);
    case OPEXT_TRY_INPLACE_MUL: MAKE_INPLACE_TRY_CODE(DeeObject_InplaceMulVar);
    case OPEXT_TRY_INPLACE_DIV: MAKE_INPLACE_TRY_CODE(DeeObject_InplaceDivVar);
    case OPEXT_TRY_INPLACE_MOD: MAKE_INPLACE_TRY_CODE(DeeObject_InplaceModVar);
    case OPEXT_TRY_INPLACE_SHL: MAKE_INPLACE_TRY_CODE(DeeObject_InplaceShlVar);
    case OPEXT_TRY_INPLACE_SHR: MAKE_INPLACE_TRY_CODE(DeeObject_InplaceShrVar);
    case OPEXT_TRY_INPLACE_AND: MAKE_INPLACE_TRY_CODE(DeeObject_InplaceAndVar);
    case OPEXT_TRY_INPLACE_OR : MAKE_INPLACE_TRY_CODE(DeeObject_InplaceOrVar);
    case OPEXT_TRY_INPLACE_XOR: MAKE_INPLACE_TRY_CODE(DeeObject_InplaceXorVar);
    case OPEXT_TRY_INPLACE_POW: MAKE_INPLACE_TRY_CODE(DeeObject_InplacePowVar);
#undef MAKE_INPLACE_TRY_CODE
    case OPEXT_TRY_INPLACE_INC:
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     Dee_INCREF(temp_var = RT_TOP);
     rt_itemp = DeeObject_InplaceIncVar(&temp_var);
EXTERN_END;
check_err_and_setpush_tempvar:
     if DEE_UNLIKELY(rt_itemp != 0) { Dee_DECREF(temp_var); RT_HANDLE_EXCEPTION; }
     rt_temp = RT_TOP;
     Dee_INCREF(RT_TOP = temp_var);
     Dee_DECREF(rt_temp);
     RT_PUSH_INHERIT(temp_var);
     break;
    case OPEXT_TRY_INPLACE_DEC:
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     Dee_INCREF(temp_var = RT_TOP);
     rt_itemp = DeeObject_InplaceDecVar(&temp_var);
EXTERN_END;
     goto check_err_and_setpush_tempvar;
    {
     DeeObject *inplace_result;
    case OPEXT_TRY_INPLACE_INC_POST:
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     Dee_INCREF(temp_var = RT_TOP);
     inplace_result = DeeObject_InplaceIncPostVar(&temp_var);
EXTERN_END;
check_err_and_push_tempvarir:
     if DEE_UNLIKELY(inplace_result == NULL) { Dee_DECREF(temp_var); RT_HANDLE_EXCEPTION; }
     rt_temp = RT_TOP;
     Dee_INHERIT_REF(RT_TOP,inplace_result);
     Dee_DECREF(rt_temp);
     RT_PUSH_INHERIT(temp_var);
     break;
    case OPEXT_TRY_INPLACE_DEC_POST:
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     Dee_INCREF(temp_var = RT_TOP);
     inplace_result = DeeObject_InplaceDecPostVar(&temp_var);
EXTERN_END;
     goto check_err_and_push_tempvarir;
    }
    }
#endif

#ifdef OPEXT_ALLOCA
    case OPEXT_ALLOCA: {
#if DEE_CONFIG_RUNTIME_HAVE_BUILTIN_ALLOCA
     Dee_size_t alloc_size;
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     if DEE_UNLIKELY((DeeObject_Cast(Dee_size_t,RT_TOP,&alloc_size) != 0) ||
                     (rt_temp = DeeVoidPointer_New(NULL)) == NULL) {
EXTERN_END;
      RT_HANDLE_EXCEPTION;
     }
EXTERN_END;
     Dee_DECREF(RT_TOP);
     RT_TOP = rt_temp; // Inherit reference
     if DEE_LIKELY(alloc_size) {
#if DEE_HAVE_ALLOCA
      // As the last thing: actually do the alloca
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6255)
#endif
      DeePointer_ADDR(rt_temp) = alloca(alloc_size);
#ifdef _PREFAST_
#pragma warning(pop)
#endif
#else
      {void **new_alloca_list;
EXTERN_BEGIN;
       if ((DeePointer_ADDR(rt_temp) = malloc_nz(alloc_size)) == NULL) {
alloca_nomem:
        DeeError_NoMemory();
EXTERN_END;
        RT_HANDLE_EXCEPTION;
       }
       if ((new_alloca_list = (void **)realloc_nz(alloca_v,(++alloca_c)*sizeof(void *))) == NULL) {
        free_nn(DeePointer_ADDR(rt_temp));
        DeePointer_ADDR(rt_temp) = NULL;
        goto alloca_nomem;
       }
EXTERN_END;
       new_alloca_list[alloca_c-1] = DeePointer_ADDR(rt_temp);
       alloca_v = new_alloca_list;
      }
#endif
     }
#else /* DEE_CONFIG_RUNTIME_HAVE_BUILTIN_ALLOCA */
     goto on_unreachable;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_BUILTIN_ALLOCA */
    } break;
#endif

#ifdef OPEXT_BREAKPOINT
    case OPEXT_BREAKPOINT: {
#if DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BREAKPOINT
EXTERN_BEGIN;
     rt_itemp = _DeeStackFrame_InteractiveDebugger_impl((DeeObject *)thread_self,&frame);
EXTERN_END;
     if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
#else /* DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BREAKPOINT */
#if defined(DEE_DEVEL_BUILD) && \
    defined(DEE_BUILTIN_BREAKPOINT)
#ifdef DEE_PLATFORM_WINDOWS
     if (IsDebuggerPresent())
#endif /* DEE_PLATFORM_WINDOWS */
      DEE_BUILTIN_BREAKPOINT();
#ifdef DEE_PLATFORM_WINDOWS
     else
#endif /* DEE_PLATFORM_WINDOWS */
#endif /* ... */
     {
      goto on_unreachable;
     }
#endif /* !DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BREAKPOINT */
    } break;
#endif

#ifdef OPEXT_DOC
    case OPEXT_DOC: {
     RT_DEBUG_CHECK_STACK(1);
#if DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP
EXTERN_BEGIN;
     rt_temp = DeeObject_Doc(RT_TOP);
EXTERN_END;
     if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP);
     Dee_INHERIT_REF(RT_TOP,rt_temp);
#else /* DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP */
     goto on_unreachable;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP */
    } break;
#endif

#ifdef OPEXT_DOC_ATTR
    case OPEXT_DOC_ATTR: {
     RT_DEBUG_CHECK_STACK(2);
#if DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP
EXTERN_BEGIN;
     rt_temp = DeeObject_DocAttr(RT_SECOND,RT_TOP);
EXTERN_END;
     if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP); RT_POP_1();
     Dee_DECREF(RT_TOP);
     Dee_INHERIT_REF(RT_TOP,rt_temp);
#else /* DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP */
     goto on_unreachable;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP */
    } break;
#endif

#ifdef OPEXT_EMPTY_TUPLE
    case OPEXT_EMPTY_TUPLE: {
     RT_PUSH(Dee_EmptyTuple);
    } break;
#endif

#ifdef OPEXT_EMPTY_LIST
    case OPEXT_EMPTY_LIST: {
EXTERN_BEGIN;
     rt_temp = DeeList_NewEmpty();
EXTERN_END;
     if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
     RT_PUSH_INHERIT(rt_temp);
    } break;
#endif

#ifdef OPEXT_EMPTY_DICT
    case OPEXT_EMPTY_DICT: {
EXTERN_BEGIN;
     rt_temp = DeeDict_NewEmpty();
EXTERN_END;
     if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
     RT_PUSH_INHERIT(rt_temp);
    } break;
#endif

#ifdef OPEXT_EMPTY_SET
    case OPEXT_EMPTY_SET: {
EXTERN_BEGIN;
     rt_temp = DeeSet_NewEmpty();
EXTERN_END;
     if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
     RT_PUSH_INHERIT(rt_temp);
    } break;
#endif

#ifdef OPEXT_CLASS_NEW
    case OPEXT_CLASS_NEW: {
     RT_DEBUG_CHECK_STACK(2);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
EXTERN_BEGIN;
     if DEE_UNLIKELY(DeeError_TypeError_CheckType(RT_SECOND,&DeeType_Type) != 0) rt_temp = NULL;
     else rt_temp = (DeeObject *)DeeClass_NewObject((DeeTypeObject *)RT_SECOND,RT_TOP);
EXTERN_END;
     if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP); RT_POP_1();
     Dee_DECREF(RT_TOP);
     Dee_INHERIT_REF(RT_TOP,rt_temp);
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES */
     goto on_unreachable;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES */
    } break;
#endif

#ifdef OPEXT_CLASS_NEW_UUID
    case OPEXT_CLASS_NEW_UUID: {
     RT_DEBUG_CHECK_STACK(3);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
EXTERN_BEGIN;
     if DEE_UNLIKELY(DeeError_TypeError_CheckType(RT_THIRD,&DeeType_Type) != 0) rt_temp = NULL;
     else if DEE_UNLIKELY(DeeError_TypeError_CheckType(RT_TOP,&DeeString_Type) != 0) rt_temp = NULL;
     else { struct DeeUUID uuid;
      if (DeeUUID_FromStringWithError(&uuid,DeeString_STR(RT_TOP)) == 0)
       rt_temp = (DeeObject *)DeeClass_NewObjectEx((DeeTypeObject *)RT_THIRD,RT_SECOND,&uuid);
      else rt_temp = NULL;
     }
EXTERN_END;
     if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP); RT_POP_1();
     Dee_DECREF(RT_TOP); RT_POP_1();
     Dee_DECREF(RT_TOP);
     Dee_INHERIT_REF(RT_TOP,rt_temp);
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES */
     goto on_unreachable;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES */
    } break;
#endif

#ifdef OPEXT_SUPER_AT
    case OPEXT_SUPER_AT: {
     RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
     if DEE_UNLIKELY(DeeError_TypeError_CheckType(RT_SECOND,&DeeType_Type) != 0) rt_temp = NULL;
     else if DEE_UNLIKELY(DeeError_TypeError_CheckType(RT_TOP,(DeeTypeObject *)RT_SECOND) != 0) rt_temp = NULL;
     else rt_temp = (DeeObject *)DeeSuper_New((DeeTypeObject *)RT_SECOND,RT_TOP);
EXTERN_END;
     if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP); RT_POP_1();
     Dee_DECREF(RT_TOP);
     Dee_INHERIT_REF(RT_TOP,rt_temp);
    } break;
#endif

#ifdef OPEXT_EMPTY_CELL
    case OPEXT_EMPTY_CELL: {
EXTERN_BEGIN;
     rt_temp = DeeCell_NewEmpty();
EXTERN_END;
     if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
     RT_PUSH_INHERIT(rt_temp);
    } break;
#endif

#ifdef OPEXT_BOUND_RESULT
    case OPEXT_BOUND_RESULT: {
#if DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BOUND
     RT_PUSH(frame.f_result ? Dee_True : Dee_False);
#else /* DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BOUND */
     goto on_unreachable;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_BUILTIN_BOUND */
    } break;
#endif

#ifdef OPEXT_DEL_RESULT
    case OPEXT_DEL_RESULT: {
     rt_temp = frame.f_result;
     frame.f_result = NULL;
     Dee_XDECREF(rt_temp);
    } break;
#endif

#ifdef OPEXT_SUPEROF
    case OPEXT_SUPEROF: {
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     rt_temp = DeeSuper_Of(RT_TOP);
EXTERN_END;
     if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP);
     RT_TOP = rt_temp;
    } break;
#endif

#ifdef OPEXT_CLASSOF
    case OPEXT_CLASSOF: {
     RT_DEBUG_CHECK_STACK(1);
     rt_temp = (DeeObject *)DeeObject_ClassOf(RT_TOP);
     Dee_DECREF(RT_TOP);
     Dee_INCREF(RT_TOP = rt_temp);
    } break;
#endif

#ifdef OPEXT_SEQ_ITERALL
    case OPEXT_SEQ_ITERALL: {
     RT_DEBUG_CHECK_STACK(1);
     // Blacklist of common types where not
     // iterating doesn't have any side-effects.
     if ((DeeType_FLAGS(Dee_TYPE(RT_TOP))&DEE_TYPE_FLAG_NO_ITERATE_EFFECT)==0) {
      DeeObject *elem,*iter;
EXTERN_BEGIN;
      iter = DeeObject_IterSelf(RT_TOP);
      if DEE_UNLIKELY(!iter) {
EXTERN_END;
       RT_HANDLE_EXCEPTION;
      }
      // Consume and drop all elements
      while ((rt_itemp = DeeObject_IterNextEx(iter,&elem)) == 0) _DeeInternalObject_DECREF(elem);
      _DeeInternalObject_DECREF(iter);
EXTERN_END;
      // Check for a possible exception in the last call to 'DeeObject_IterNextEx'
      if DEE_UNLIKELY(rt_itemp < 0) RT_HANDLE_EXCEPTION;
     }
     Dee_DECREF(RT_TOP); RT_POP_1();
    } break;
#endif

#ifdef OPEXT_SEQ_ANY
    case OPEXT_SEQ_ANY: {
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     rt_itemp = DeeSequence_Any(RT_TOP);
EXTERN_END;
     if DEE_UNLIKELY(rt_itemp < 0) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP);
     RT_TOP = DeeBool_New(rt_itemp);
    } break;
#endif

#ifdef OPEXT_SEQ_ALL
    case OPEXT_SEQ_ALL: {
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     rt_itemp = DeeSequence_All(RT_TOP);
EXTERN_END;
     if DEE_UNLIKELY(rt_itemp < 0) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP);
     RT_TOP = DeeBool_New(rt_itemp);
    } break;
#endif

#ifdef OPEXT_SEQ_SUM
    case OPEXT_SEQ_SUM: {
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     rt_temp = DeeSequence_Sum(RT_TOP);
EXTERN_END;
     if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP);
     Dee_INHERIT_REF(RT_TOP,rt_temp);
    } break;
#endif

#ifdef OPEXT_SEQ_MIN
    case OPEXT_SEQ_MIN: {
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     rt_temp = DeeSequence_Min(RT_TOP);
EXTERN_END;
     if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP);
     Dee_INHERIT_REF(RT_TOP,rt_temp);
    } break;
#endif

#ifdef OPEXT_SEQ_MAX
    case OPEXT_SEQ_MAX: {
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     rt_temp = DeeSequence_Max(RT_TOP);
EXTERN_END;
     if DEE_UNLIKELY(!rt_temp) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP);
     Dee_INHERIT_REF(RT_TOP,rt_temp);
    } break;
#endif

#ifdef OPEXT_SEQ_ITER_NEXT
    case OPEXT_SEQ_ITER_NEXT: {
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     rt_itemp = DeeObject_IterNextEx(RT_TOP,&rt_temp);
EXTERN_END;
     if DEE_UNLIKELY(rt_itemp != 0) {
      if DEE_LIKELY(rt_itemp > 0)
       DeeError_Throw(DeeErrorInstance_IterDone);
      RT_HANDLE_EXCEPTION;
     }
     Dee_DECREF(RT_TOP);
     Dee_INHERIT_REF(RT_TOP,rt_temp);
    } break;
#endif

#ifdef OPEXT_TRY_CMP_EQ
    case OPEXT_TRY_CMP_EQ: {
     RT_DEBUG_CHECK_STACK(2);
EXTERN_BEGIN;
     rt_itemp = DeeObject_CompareEq(RT_SECOND,RT_TOP);
EXTERN_END;
     if DEE_UNLIKELY(rt_itemp < 0) {
      if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) RT_HANDLE_EXCEPTION;
      rt_itemp = 0;
     }
     Dee_DECREF(RT_TOP); RT_POP_1();
     Dee_DECREF(RT_TOP); RT_TOP = DeeBool_New(rt_itemp);
    } break;
#endif

#ifdef OPEXT_IO_WRITEP
    case OPEXT_IO_WRITEP: {
     RT_DEBUG_CHECK_STACK(3);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
     {
      void *p; Dee_size_t s,ws;
EXTERN_BEGIN;
      rt_itemp = DeeObject_GetVoidPointerEx(RT_SECOND,&p);
#ifdef DEE_DEBUG
      rt_temp = NULL;
#endif
      if DEE_LIKELY(rt_itemp == 0) {rt_itemp = DeeObject_Cast(Dee_size_t,RT_TOP,&s);
      if DEE_LIKELY(rt_itemp == 0) {rt_itemp = DeeObject_IOWrite(RT_THIRD,p,s,&ws);
      if DEE_LIKELY(rt_itemp == 0) {rt_temp = DeeObject_New(Dee_size_t,ws);
      }}}
EXTERN_END;
     }
     if DEE_UNLIKELY(rt_itemp != 0 || !rt_temp) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP); RT_POP_1();
     Dee_DECREF(RT_TOP); RT_POP_1();
     Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
     goto on_unreachable;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
    } break;
#endif

#ifdef OPEXT_IO_WRITENP
    case OPEXT_IO_WRITENP: {
     RT_DEBUG_CHECK_STACK(2);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
     {
      Dee_size_t ws;
EXTERN_BEGIN;
#ifdef DEE_DEBUG
      rt_temp = NULL;
#endif
      rt_itemp = DeeError_TypeError_CheckType(RT_TOP,&DeeString_Type);
      if DEE_LIKELY(rt_itemp == 0) {rt_itemp = DeeObject_IOWrite(
       RT_SECOND,DeeString_STR(RT_TOP),DeeString_SIZE(RT_TOP)*sizeof(char),&ws);
      if DEE_LIKELY(rt_itemp == 0) {rt_temp = DeeObject_New(Dee_size_t,ws);
      }}
EXTERN_END;
     }
     if DEE_UNLIKELY(rt_itemp != 0 || !rt_temp) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP); RT_POP_1();
     Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
     goto on_unreachable;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
    } break;
#endif

#ifdef OPEXT_IO_READP
    case OPEXT_IO_READP: {
     RT_DEBUG_CHECK_STACK(3);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
     {
      void *p; Dee_size_t s,rs;
EXTERN_BEGIN;
#ifdef DEE_DEBUG
      rt_temp = NULL;
#endif
      rt_itemp = DeeObject_GetVoidPointerEx(RT_SECOND,&p);
      if DEE_LIKELY(rt_itemp == 0) {rt_itemp = DeeObject_Cast(Dee_size_t,RT_TOP,&s);
      if DEE_LIKELY(rt_itemp == 0) {rt_itemp = DeeObject_IORead(RT_THIRD,p,s,&rs);
      if DEE_LIKELY(rt_itemp == 0) {rt_temp = DeeObject_New(Dee_size_t,rs);
      }}}
EXTERN_END;
     }
     if DEE_UNLIKELY(rt_itemp != 0 || !rt_temp) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP); RT_POP_1();
     Dee_DECREF(RT_TOP); RT_POP_1();
     Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
     goto on_unreachable;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
    } break;
#endif

#ifdef OPEXT_IO_READNP
    case OPEXT_IO_READNP: {
     RT_DEBUG_CHECK_STACK(2);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
     {
      Dee_size_t s,rs;
EXTERN_BEGIN;
      rt_itemp = DeeObject_Cast(Dee_size_t,RT_TOP,&s);
      if DEE_LIKELY(rt_itemp == 0) {
       if DEE_UNLIKELY((rt_temp = DeeString_NewSized(s/sizeof(char))) == NULL) rt_itemp = -1;
       else {
        rt_itemp = DeeObject_IORead(RT_SECOND,DeeString_STR(rt_temp),s,&rs);
        if (DEE_LIKELY(rt_itemp) && DEE_UNLIKELY(s > rs)) {
         if DEE_UNLIKELY((rt_itemp = DeeString_Resize(&rt_temp,rs)) != 0)
          Dee_DECREF(rt_temp);
        }
       }
      }
#ifdef DEE_DEBUG
      else rt_temp = NULL;
#endif
EXTERN_END;
     }
     if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP); RT_POP_1();
     Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
     goto on_unreachable;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
    } break;
#endif

#ifdef OPEXT_IO_SEEK
    case OPEXT_IO_SEEK: {
     RT_DEBUG_CHECK_STACK(3);
     {
      Dee_int64_t off; int whence;
EXTERN_BEGIN;
      rt_itemp = DeeObject_Cast(Dee_int64_t,RT_SECOND,&off);
      if DEE_LIKELY(rt_itemp == 0) {rt_itemp = DeeObject_Cast(int,RT_TOP,&whence);
      if DEE_LIKELY(rt_itemp == 0) {rt_itemp = DeeObject_IOSeek(RT_THIRD,off,whence,NULL);
      }}
EXTERN_END;
     }
     if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP); RT_POP_1();
     Dee_DECREF(RT_TOP); RT_POP_1();
     Dee_DECREF(RT_TOP); RT_POP_1();
    } break;
#endif

#ifdef OPEXT_IO_SEEK_USED
    case OPEXT_IO_SEEK_USED: {
     RT_DEBUG_CHECK_STACK(3);
     {
      Dee_int64_t off; int whence; Dee_uint64_t new_pos;
EXTERN_BEGIN;
#ifdef DEE_DEBUG
      rt_temp = NULL;
#endif
      rt_itemp = DeeObject_Cast(Dee_int64_t,RT_SECOND,&off);
      if DEE_LIKELY(rt_itemp == 0) {rt_itemp = DeeObject_Cast(int,RT_TOP,&whence);
      if DEE_LIKELY(rt_itemp == 0) {rt_itemp = DeeObject_IOSeek(RT_THIRD,off,whence,&new_pos);
      if DEE_LIKELY(rt_itemp == 0) {rt_temp = DeeObject_New(Dee_uint64_t,new_pos);
      }}}
EXTERN_END;
     }
     if DEE_UNLIKELY(rt_itemp != 0 || !rt_temp) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP); RT_POP_1();
     Dee_DECREF(RT_TOP); RT_POP_1();
     Dee_DECREF(RT_TOP); Dee_INHERIT_REF(RT_TOP,rt_temp);
    } break;
#endif

#ifdef OPEXT_IO_TRUNC
    case OPEXT_IO_TRUNC: {
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     rt_itemp = DeeObject_IOTrunc(RT_TOP);
EXTERN_END;
     if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP); RT_POP_1();
    } break;
#endif

#ifdef OPEXT_IO_FLUSH
    case OPEXT_IO_FLUSH: {
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     rt_itemp = DeeObject_IOFlush(RT_TOP);
EXTERN_END;
     if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP); RT_POP_1();
    } break;
#endif

#ifdef OPEXT_IO_CLOSE
    case OPEXT_IO_CLOSE: {
     RT_DEBUG_CHECK_STACK(1);
EXTERN_BEGIN;
     rt_itemp = DeeObject_IOClose(RT_TOP);
EXTERN_END;
     if DEE_UNLIKELY(rt_itemp != 0) RT_HANDLE_EXCEPTION;
     Dee_DECREF(RT_TOP); RT_POP_1();
    } break;
#endif

    default:
     DeeError_SetStringf(&DeeErrorType_IllegalInstruction,
                         "Illegal extended instruction: %I16x(%I16u)",
                         ((Dee_uint16_t*)code)[-1],((Dee_uint16_t*)code)[-1]);
     RT_HANDLE_EXCEPTION;
   }
   DISPATCH();
  }
#endif

  default: {
ill_instr:
   // Unknown opcode (skip arg if necessary)
   if ((code[-1]&OPFLAG_ARG)!=0) {
    DeeError_SetStringf(&DeeErrorType_IllegalInstruction,
                        "Illegal instruction: %I8x(%I8u) + %I16x(%I16u)",
                        ((Dee_uint8_t*)code)[-1],((Dee_uint8_t*)code)[-1],
                        ((Dee_uint16_t*)code)[0],((Dee_uint16_t*)code)[0]);
    RT_SKIP_ARG();
   } else {
    DeeError_SetStringf(&DeeErrorType_IllegalInstruction,
                        "Illegal instruction: %I8x(%I8u)",
                        ((Dee_uint8_t*)code)[-1],((Dee_uint8_t*)code)[-1]);
   }
   RT_HANDLE_EXCEPTION;
  }
 }
}
#undef DISPATCH
#undef TARGET
#undef RT_CODE_ADDR
#undef RT_STACK_ADDR
#undef RT_STACK_ADDR32
#undef RT_CONST
#undef RT_REF
#undef RT_SKIP_OP
#undef RT_READ_OP
#undef RT_PREV_OP
#undef RT_PEEK_OP
#undef RT_SKIP_ARG
#undef RT_LAST_SARG
#undef RT_READ_SARG
#undef RT_PEEK_SARG
#undef RT_LAST_ARG
#undef RT_READ_ARG
#undef RT_PEEK_ARG
#undef RT_PUSH
#undef RT_PUSH_INHERIT
#undef RT_POP_N
#undef RT_POP_1
#undef RT_FOURTH
#undef RT_THIRD
#undef RT_SECOND
#undef RT_TOP
#undef RT_STACK_SLOT
#undef RT_DEBUG_CHECK_STACK
#undef RT_DEBUG_CHECK_VALID_CONST
#undef RT_DEBUG_CHECK_VALID_LOCAL
#undef RT_DEBUG_CHECK_VALID_REF
#undef RT_DEBUG_CHECK_VALID_ARG
#undef RT_DEBUG_CHECK_STACK32
#undef RT_DEBUG_CHECK_VALID_CONST32
#undef RT_DEBUG_CHECK_VALID_LOCAL32
#undef RT_DEBUG_CHECK_VALID_REF32
#undef RT_DEBUG_CHECK_VALID_ARG32
#undef RT_ASSERT_EXCEPTION_OCCURRED
#undef RT_HANDLE_EXCEPTION
end:
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 // We may still have unhandled exceptions that were hidden because of finally handlers
 if (frame.f_stored_exceptions) goto retore_exceptions;
 // Execute finally blocks
 while (frame.f_exceptv_end != frame.f_exceptv) {
  handler = (--frame.f_exceptv_end)->r_handler; // Consume one handler
  DEE_ASSERTF(frame.f_exceptv_end >= frame.f_exceptv &&
              frame.f_exceptv_end < frame.f_exceptv+frame.f_code->co_exceptsize,
              "Frame exception handlers end is out-of-bounds.\n"
              "Active handler address %p isn't in the range of %p...%p",
              frame.f_exceptv_end,frame.f_exceptv,frame.f_exceptv+frame.f_code->co_exceptsize);
  if (handler->e_kind != DeeExceptionHandleKind_FINALLY) continue; // Inappropriate handler
  goto exec_handler_novar; // Execute the handler
 }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 // No more finally handlers (so we can ~finally~ get out of here...)
end_now:
 // The frame is going to disappear and not come back!
#if 0
 // This is allowed to happen (and does happen) when a finally block causes another exception
 DEE_ASSERT(!_DeeStackFrame_FINALLY_RETURN_ADDRESS_SIZE(frame) &&
            "Reached end before all finally handlers were executed");
#endif
 // Clear the execution frame and restore the old
 // stack frame in the thread-local context.
 // Unlock the frame to allow the gc to visit it before we destroy it
 DeeAtomicMutex_Release(&frame.f_lock);
 // Restore the old thread frame
 // Only need to lock the frame lock for writing
 DeeAtomicMutex_Acquire(&thread_self->t_frame_lock);
 thread_self->t_frame_last = frame.f_prev;
#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
 // Release one from the frame size counter
 --thread_self->t_frame_size;
#endif /* DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS */
 DeeAtomicMutex_Release(&thread_self->t_frame_lock);
 // The frame is now unlinked, unlocked and invisible to everyone
#if defined(YIELD_FUNCTION) || defined(DEE_DEBUG)
 frame.f_prev = NULL;
#endif

#if !DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 // Restore stored exceptions
 if (frame.f_stored_exceptions) {
  struct DeeRaisedException *except_iter;
  except_iter = thread_self->t_exception;
  if (except_iter) {
   while (except_iter->ob_prev) except_iter = except_iter->ob_prev;
   DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
   except_iter->ob_prev = frame.f_stored_exceptions;
   DeeAtomicMutex_Release(&thread_self->t_exception_lock);
  } else {
   DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
   thread_self->t_exception = frame.f_stored_exceptions;
   DeeAtomicMutex_Release(&thread_self->t_exception_lock);
  }
 }
#endif /* !DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */

end_local_clean:
 // Cleanup Local variables
 {DeeObject **local_iter,**local_end,*temp;
  local_end = (local_iter = frame.f_localv)+frame.f_code->co_nlocals;
  while (local_iter != local_end) {
   // Make sure to use the real decref macro
   // (we can't execute code as extern at this point)
   if ((temp = *local_iter) != NULL) {
#ifdef YIELD_FUNCTION
    *local_iter = NULL;
#endif
    _DeeInternalObject_DECREF(temp);
   }
   ++local_iter;
  }
 }
#ifdef DEE_DEBUG
 if (stack != stack_end) {
  // Make sure the stack got cleaned correctly
  DEE_LDEBUG("%O(%I32d) : %O : Wrong stack alignment\n",
             _DeeStackFrame_File(&frame),_DeeStackFrame_Line(&frame)+1,
             _DeeStackFrame_Func(&frame));
  {DeeObject **debug_iter;
   debug_iter = stack;
   while (debug_iter != stack_end)
    DEE_LDEBUG("stack %#.4x : %r\n",(unsigned)(debug_iter-stack),*debug_iter++);
  }
  DEE_ASSERT(0);
 }
#endif
end_always:
#if DEE_CONFIG_RUNTIME_HAVE_BUILTIN_ALLOCA && !DEE_HAVE_ALLOCA
 if (alloca_c) {
  do free_nn(alloca_v[--alloca_c]); while (alloca_c);
  free_nn(alloca_v);
 }
#endif
#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
end_always_2:
#endif /* DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS */
 // Store final values in the frame
 // >> Required to stabilize local optimizations
#ifdef OPTIMIZE_STACKEND
 frame.f_stackv_end = stack_end;
#endif
#ifdef __INTELLISENSE__
#ifdef YIELD_FUNCTION
 return result;
#endif
 DEE_BUILTIN_UNREACHABLE();
#else
 goto exec_end;
#endif
handle_exception:
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 if (frame.f_stored_exceptions) {
  // Restore additional exceptions we know about
retore_exceptions:
  DEE_ASSERTF(frame.f_stored_exceptions,"No exceptions to restore");
  // Prepend the stored exceptions onto the list of actual exceptions
  {struct DeeRaisedException *except_iter;
EXTERN_BEGIN;
   except_iter = thread_self->t_exception;
   if (except_iter) {
    while (except_iter->re_prev) except_iter = except_iter->re_prev;
    DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
    except_iter->re_prev = frame.f_stored_exceptions;
    DeeAtomicMutex_Release(&thread_self->t_exception_lock);
   } else {
    DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
    thread_self->t_exception = frame.f_stored_exceptions;
    DeeAtomicMutex_Release(&thread_self->t_exception_lock);
   }
EXTERN_END;
  }
  frame.f_stored_exceptions = NULL;
 }
 // TODO: This is where we should generate the traceback for lazy initialization
 // >> thread_self->t_exception->ob_traceback

 // Error occurred
 if (frame.f_exceptv_end != frame.f_exceptv) {
  DeeObject *error_ob;
  DeeObject **required_stack_end;
  DEE_ASSERTF(thread_self->t_exception,
              "Exception handling code is being executed, but no exception was set");
  error_ob = thread_self->t_exception->re_error;
  DEE_ASSERTF(DeeObject_Check(error_ob),
              "The raised exception (object at %p) isn't a valid object",
              error_ob);
  // Exception handling
  while (frame.f_exceptv_end != frame.f_exceptv) {
   handler = (--frame.f_exceptv_end)->r_handler; // Consume one handler
   DEE_ASSERTF(frame.f_exceptv_end >= frame.f_exceptv &&
               frame.f_exceptv_end < frame.f_exceptv+frame.f_code->co_exceptsize,
               "Frame exception handlers end is out-of-bounds.\n"
               "Active handler address %p isn't in the range of %p...%p",
               frame.f_exceptv_end,frame.f_exceptv,frame.f_exceptv+frame.f_code->co_exceptsize);
   // After having troubles with this once, lets be really picky about
   // doing every possible assertion for checking the validity of the handler!
   DEE_ASSERTF(handler,"Invalid handler");
   DEE_ASSERTF(handler >= frame.f_code->co_exceptv &&
               handler < frame.f_code->co_exceptv+frame.f_code->co_exceptc,
               "Found exception handler is not an acceptable handler for this code.\n"
               "Handler address %p isn't in the range of %p...%p",handler,
               frame.f_code->co_exceptv,frame.f_code->co_exceptv+frame.f_code->co_exceptc);
   DEE_ASSERTF((handler->e_kind&DeeExceptionHandleKind_FLAG_VAR)!=0
               ? handler->e_store < frame.f_code->co_nlocals : !handler->e_store,
               "Invalid local id in exception handler: %Iu",handler->e_store);
   DEE_ASSERTF(handler->e_stack <= frame.f_code->co_stacksize,
               "Exception handler stack alignment is impossible (%Iu exceeds maximum of %Iu)",
               handler->e_stack,frame.f_code->co_stacksize);
   DEE_ASSERTF(!handler->e_type || (DeeObject_Check(handler->e_type) && DeeType_Check(handler->e_type)),
               "Exception handler type mask (object at %p) is neither NULL, nor a valid type",handler->e_type);
   DEE_ASSERTF(handler->e_handler < DeeCode_SIZE(frame.f_code),
               "Exception handler code address is out of bounds (%Iu exceeds maximum of %Iu)",
               handler->e_handler,DeeCode_SIZE(frame.f_code));
   if ((handler->e_kind&DeeExceptionHandleKind_FLAG_TYPED)!=0 &&
       !DeeObject_InstanceOf(error_ob,handler->e_type)
       ) continue; // Inappropriate handler (typed handler mask doesn't apply)
   // Yes! use this handler.
   if ((handler->e_kind&DeeExceptionHandleKind_FLAG_VAR)!=0) {
    DeeObject *old_val;
    // Store the exception object is required
    old_val = frame.f_localv[handler->e_store];
    Dee_INCREF(frame.f_localv[handler->e_store] = error_ob);
    Dee_XDECREF(old_val);
   }
exec_handler_novar:
   // Hide additional exceptions to simulate an exception-free
   // environment while executing exception handler code
   // NOTE: These exceptions are restored once the handler is finished
   // NOTE2: Unless it's a finally handler, we delete one exception
   frame.f_stored_exceptions = thread_self->t_exception;
EXTERN_BEGIN;
   DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
   thread_self->t_exception = NULL;
   DeeAtomicMutex_Release(&thread_self->t_exception_lock);
EXTERN_END;
   if (handler->e_kind != DeeExceptionHandleKind_FINALLY) {
    // Remove one exception from the list we will restore
    struct DeeRaisedException *handled_exception = frame.f_stored_exceptions;
    frame.f_stored_exceptions = frame.f_stored_exceptions->re_prev;
    handled_exception->re_prev = thread_self->t_handling_exception;
EXTERN_BEGIN;
    DeeAtomicMutex_Acquire(&thread_self->t_handling_exception_lock);
    thread_self->t_handling_exception = handled_exception;
    DeeAtomicMutex_Release(&thread_self->t_handling_exception_lock);
EXTERN_END;
   } else {
    static Dee_uint8_t const stop_code[] = {OP_STOP};
    frame.f_finally_return_address_end = _DeeStackFrame_FINALLY_RETURN_ADDRESS_BASE(frame);
    *frame.f_finally_return_address_end++ = stop_code; // push a faked stop code address
   }
fast_exec_handler_novar:
   // Align the stack
   required_stack_end = stack+handler->e_stack;
   DEE_ASSERT(stack_end >= required_stack_end);
#ifdef DEE_DEBUG
   if (stack_end != required_stack_end)
    DEE_LVERBOSE1("Aligning stack for exception handler (dropping %Iu slot(s))\n",
                  (Dee_size_t)(stack_end-required_stack_end));
#endif /* DEE_DEBUG */
   while (stack_end != required_stack_end) Dee_DECREF(*--stack_end);
   // Navigate to the exception handler
   code = frame.f_code->co_code+handler->e_handler;
   goto exec; // Execute more code!
  }
 }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 // No (appropriate) handlers found

 // --> cleanup the stack and return NULL
 // >> 'we've failed; the ship is sinking!!!'
 while (stack != stack_end) {
#ifdef DEE_DEBUG
  --stack_end;
  DEE_LVERBOSE1("stack %#.4x : %r\n",(unsigned)(stack_end-stack),*stack_end);
  Dee_DECREF(*stack_end);
#else /* DEE_DEBUG */
  Dee_DECREF(*--stack_end);
#endif /* !DEE_DEBUG */
 }
 // Clear the result value if it was already assigned
#ifdef YIELD_FUNCTION
 result = -1;
#endif
 Dee_XCLEAR(frame.f_result);
 goto end_now;
end_yield:
 // Clear the execution frame and restore the old
 // stack frame in the thread-local context
 // Unlock the frame to allow the gc to visit it before we destroy it
 DeeAtomicMutex_Release(&frame.f_lock);
 // Restore the old thread frame
 // Only need to lock the frame lock for writing
 DeeAtomicMutex_Acquire(&thread_self->t_frame_lock);
 thread_self->t_frame_last = frame.f_prev;
#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
 // Release one from the frame size counter
 --thread_self->t_frame_size;
#endif /* DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS */
 DeeAtomicMutex_Release(&thread_self->t_frame_lock);
#if defined(YIELD_FUNCTION) || defined(DEE_DEBUG)
 frame.f_prev = NULL;
#endif
 goto end_always;
#undef _deert_unexpected_unpack_size
}
#undef ERROR_OCCURRED
#ifdef code
#undef code
#endif
#ifdef stack
#undef stack
#endif
#ifdef stack_end
#undef stack_end
#endif
#ifdef OPTIMIZE_CODEADDR
#undef OPTIMIZE_CODEADDR
#endif
#ifdef OPTIMIZE_STACKEND
#undef OPTIMIZE_STACKEND
#endif
#ifdef YIELD_FUNCTION
#undef YIELD_FUNCTION
#endif
#ifdef SANDBOX
#undef SANDBOX
#endif
#undef _DeeObject_DECREF
#define _DeeObject_DECREF  _DeeInternalObject_DECREF
#undef _DeeExecuteObject_DECREF
#ifdef STACK_DEBUG
#undef STACK_DEBUG
#endif
#ifdef SANDBOX
#undef DeeSand_Violation
#undef DeeSand_Violationf
#endif

#ifdef __INTELLISENSE__
#undef frame
DEE_DECL_END
#endif
