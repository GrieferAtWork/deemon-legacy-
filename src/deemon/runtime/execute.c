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
#ifndef GUARD_DEEMON_EXECUTE_C
#define GUARD_DEEMON_EXECUTE_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/array.h>
#include <deemon/bool.h>
#include <deemon/cell.h>
#include <deemon/class.h>
#include <deemon/compiler/code.h>
#include <deemon/deemonrun.h>
#include <deemon/dex.h>
#include <deemon/dict.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/integer.h>
#include <deemon/list.h>
#include <deemon/mp/thread.h>
#include <deemon/none.h>
#include <deemon/optional/object_doc.h>
#include <deemon/optional/object_io.h>
#include <deemon/optional/object_visit.h>
#include <deemon/runtime/builtins.h>
#include <deemon/runtime/execute.h>
#include <deemon/runtime/extern.h>
#include <deemon/runtime/function.h>
#include <deemon/runtime_flags.h>
#include <deemon/sequence.h>
#include <deemon/set.h>
#include <deemon/structured.h>
#include <deemon/super.h>
#include <deemon/traceback.h>
#include <deemon/tuple.h>
#include <deemon/weakref.h>

// /src/*
#include DEE_INCLUDE_MEMORY_API()
#include <deemon/__xconf.inl>
#include <deemon/__alloca.inl>
#include <deemon/compiler/__opcode.inl>

// */ (nano...)

#ifdef DEE_PLATFORM_WINDOWS
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif


DEE_DECL_BEGIN


DEE_STATIC_INLINE(void) _Dee_XInitObjectVector(
 Dee_size_t n, DeeObject **dst, DeeObject *const *src) {
 while (n--) Dee_XINCREF(*dst++ = *src++);
}
DEE_STATIC_INLINE(int) _Dee_XInitObjectVectorCopy(
 Dee_size_t n, DeeObject **dst, DeeObject *const *src) {
 DeeObject *elem,**iter = dst;
 while (n--) {
  if ((elem = *src++) != NULL) {
   if ((*iter = DeeObject_Copy(elem)) == NULL) {
    while (iter != dst) Dee_XDECREF(*--iter);
    return -1;
   }
  } else *iter = NULL;
  ++iter;
 }
 return 0;
}
DEE_STATIC_INLINE(int) _Dee_XInitObjectVectorDeepCopy(
 Dee_size_t n, DeeObject **dst, DeeObject *const *src) {
 DeeObject *elem,**iter = dst;
 while (n--) {
  if ((elem = *src++) != NULL) {
   if ((*iter = DeeObject_DeepCopy(elem)) == NULL) {
    while (iter != dst) Dee_XDECREF(*--iter);
    return -1;
   }
  } else *iter = NULL;
  ++iter;
 }
 return 0;
}
DEE_STATIC_INLINE(void) _Dee_InitObjectVector(
 Dee_size_t n, DeeObject **dst, DeeObject *const *src) {
 while (n--) Dee_INCREF(*dst++ = *src++);
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _Dee_InitObjectVectorCopy(
 Dee_size_t n, DeeObject **dst, DeeObject *const *src) {
 DeeObject **iter = dst;
 while (n--) {
  if DEE_UNLIKELY((*iter = DeeObject_Copy(*src)) == NULL) {
   while (iter != dst) Dee_DECREF(*--iter);
   return -1;
  }
  ++iter,++src;
 }
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _Dee_InitObjectVectorDeepCopy(
 Dee_size_t n, DeeObject **dst, DeeObject *const *src) {
 DeeObject **iter = dst;
 while (n--) {
  if DEE_UNLIKELY((*iter = DeeObject_DeepCopy(*src)) == NULL) {
   while (iter != dst) Dee_DECREF(*--iter);
   return -1;
  }
  ++iter,++src;
 }
 return 0;
}

DEE_STATIC_INLINE(void) _Dee_XQuitObjectVector(Dee_size_t n, DeeObject **v) { while (n--) Dee_XDECREF(*v++); }
DEE_STATIC_INLINE(void) _Dee_QuitObjectVector(Dee_size_t n, DeeObject **v) { while (n--) Dee_DECREF(*v++); }

DEE_A_RET_EXCEPT(-1) int _DeeStackFrame_InitCopy(
 DEE_A_OUT struct DeeStackFrame *self,
 DEE_A_IN struct DeeStackFrame const *right,
 DEE_A_IN Dee_uint32_t copy_flags) {
 DeeCodeObject *code; Dee_size_t stack_size;
 DEE_ASSERT(self && right);
 code = right->f_code;
 DEE_ASSERT(DeeObject_Check(code) && DeeCode_Check(code));
 // Make sure we can actually copy code of the given stackframe.
 // NOTE: We allow the copying of any frame if code hasn't been executed, yet
 if DEE_UNLIKELY((code->co_flags&DEE_CODE_FLAG_COPY)==0 && _DeeStackFrame_WasExecuted(right)) {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Can't copy code %k stackframe not marked with [[copyable]]",code);
  return -1;
 }
 // Allocate the work buffer (on the heap)
 if DEE_UNLIKELY((self->f_wbuf = (char *)malloc_nz(code->co_wsize)) == NULL) {
  DeeError_NoMemory();
  return -1;
 }

 // Initialize stuff that doesn't require a lock to 'right->f_lock'
 DeeAtomicMutex_Init(&self->f_lock);
 self->f_prev = NULL;
 self->f_flags = DEE_STACKFRAME_FLAG_NONE;

 // Acquire a lock to the right stackframe
 DeeAtomicMutex_AcquireRelaxed(&right->f_lock);

 // Copy locals
 self->f_localv = (DeeObject **)((Dee_uintptr_t)self->f_wbuf+code->co_woff_loc);
 if ((copy_flags&DEE_STACKFRAME_COPY_FLAG_LOCALS_DEEPCOPY)!=0) {
  if DEE_UNLIKELY(_Dee_XInitObjectVectorDeepCopy(code->co_nlocals,self->f_localv,right->f_localv) != 0) {
err_0: free_nn(self->f_wbuf); DeeAtomicMutex_Release(&right->f_lock); return -1;
  }
 } else if ((copy_flags&DEE_STACKFRAME_COPY_FLAG_LOCALS_COPY)!=0) {
  if DEE_UNLIKELY(_Dee_XInitObjectVectorCopy(code->co_nlocals,self->f_localv,right->f_localv) != 0) goto err_0;
 } else {
  _Dee_XInitObjectVector(code->co_nlocals,self->f_localv,right->f_localv);
 }

 // Copy the stack
 stack_size = (Dee_size_t)(right->f_stackv_end-right->f_stackv);
 self->f_stackv_end = self->f_stackv+stack_size;
 if ((copy_flags&DEE_STACKFRAME_COPY_FLAG_STACK_DEEPCOPY)!=0) {
  if DEE_UNLIKELY(_Dee_InitObjectVectorDeepCopy(stack_size,self->f_stackv,right->f_stackv) != 0) {
err_1: _Dee_XQuitObjectVector(code->co_nlocals,self->f_localv); goto err_0;
  }
 } else if ((copy_flags&DEE_STACKFRAME_COPY_FLAG_STACK_COPY)!=0) {
  if DEE_UNLIKELY(_Dee_InitObjectVectorCopy(stack_size,self->f_stackv,right->f_stackv) != 0) goto err_1;
 } else {
  _Dee_InitObjectVector(stack_size,self->f_stackv,right->f_stackv);
 }

 // Copy stored exceptions
 if (right->f_stored_exceptions) {
  if DEE_UNLIKELY((self->f_stored_exceptions = _DeeRaisedException_CopyChain(
   right->f_stored_exceptions)) == NULL) {
/*err_2:*/ _Dee_QuitObjectVector(stack_size,self->f_stackv); goto err_1;
  }
 } else {
  self->f_stored_exceptions = NULL;
 }

 // Initialize everything that can be done noexcept
 self->f_code = code;
 self->f_addr = right->f_addr;
 self->f_constv = right->f_constv;
 self->f_refv = right->f_refv;
 self->f_argv = right->f_argv;
#ifdef DEE_DEBUG
 self->f_argc = right->f_argc;
 self->f_refc = right->f_refc;
#endif
 Dee_XINCREF(self->f_this = right->f_this);
 Dee_XINCREF(self->f_result = right->f_result);
#if DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS
 // Copy active exception handlers
 {Dee_size_t right_exception_handler_size;
  right_exception_handler_size = (Dee_size_t)(right->f_exceptv_end-right->f_exceptv);
  self->f_exceptv = (struct DeeExceptionHandlerRef *)((Dee_uintptr_t)self->f_wbuf+code->co_woff_except);
  self->f_exceptv_end = self->f_exceptv+right_exception_handler_size;
  memcpy(self->f_exceptv,right->f_exceptv,right_exception_handler_size*sizeof(struct DeeExceptionHandlerRef));
 }
#endif
#if DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS
 // Initialize the finally address memory
 self->f_finally_return_address_end = (Dee_uint8_t const **)((Dee_uintptr_t)self->f_wbuf+code->co_woff_finally);
 {Dee_uint8_t const **finally_right_begin;
  Dee_size_t finally_size;
  finally_right_begin = (Dee_uint8_t const **)((Dee_uintptr_t)right->f_wbuf+code->co_woff_finally);
  finally_size = (Dee_size_t)(right->f_finally_return_address_end-finally_right_begin);
  memcpy((void *)self->f_finally_return_address_end,(void *)finally_right_begin,finally_size*sizeof(Dee_uint8_t const *));
  self->f_finally_return_address_end += finally_size;
 }
#endif
 DeeAtomicMutex_Release(&right->f_lock);
 return 0;
}
void _DeeStackFrame_Quit(DEE_A_IN struct DeeStackFrame *self) {
 Dee_XDECREF(self->f_result);
 Dee_XDECREF(self->f_this);
 _Dee_QuitObjectVector((Dee_size_t)(self->f_stackv_end-self->f_stackv),self->f_stackv);
 _Dee_XQuitObjectVector(self->f_code->co_nlocals,self->f_localv);
 if (self->f_stored_exceptions) _DeeRaisedException_DeleteChain(self->f_stored_exceptions);
 Dee_DECREF(self->f_code);
 free_nn(self->f_wbuf);
}



#define DEE_STACKFRAME_MARSHAL_FLAG_NONE     DEE_UINT16_C(0x0000)
#define DEE_STACKFRAME_MARSHAL_FLAG_RESULT   DEE_UINT16_C(0x0001) /*< The frame contains a result variable. */
#define DEE_STACKFRAME_MARSHAL_FLAG_STACK    DEE_UINT16_C(0x0002) /*< The frame has a non-empty stack. */
#define DEE_STACKFRAME_MARSHAL_FLAG_THIS     DEE_UINT16_C(0x0004) /*< The frame contains a this variable. */
#define DEE_STACKFRAME_MARSHAL_FLAG_FILENAME DEE_UINT16_C(0x0008) /*< The frame has a filename set. */
#if DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS
#define DEE_STACKFRAME_MARSHAL_FLAG_SEXCEPT  DEE_UINT16_C(0x0010) /*< The frame contains stored exceptions. */
#define DEE_STACKFRAME_MARSHAL_FLAG_HEXCEPT  DEE_UINT16_C(0x0020) /*< The frame contains active exception handlers. */
#define DEE_STACKFRAME_MARSHAL_FLAG_FINALLY  DEE_UINT16_C(0x0040) /*< The frame contains finally return addresses. */
#endif
// struct DeeMarshalStackFrame {
//  Dee_uint8_t msf_flags; /*< Set of 'DEE_STACKFRAME_MARSHAL_FLAG_*' */
// };


DEE_A_RET_EXCEPT(-1) int _DeeStackFrame_InitFromMarshal(
 DEE_A_OUT struct DeeStackFrame *self, DEE_A_INOUT struct DeeCodeObject *code,
#ifdef DEE_DEBUG
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeObject **argv,
 DEE_A_IN Dee_size_t refc, DEE_A_IN_R(refc) DeeObject **refv,
#else
 DEE_A_INOUT DeeObject **argv, DEE_A_INOUT DeeObject **refv,
#endif
 DEE_A_INOUT DeeObject *infile, DEE_A_INOUT struct DeeMarshalReadMap *map) {
 DEE_ASSERT(self); DEE_ASSERT(map);
 DEE_ASSERT(DeeObject_Check(infile) && DeeFile_Check(infile));
 DEE_ASSERT(DeeObject_Check(code) && DeeCode_Check(code));
#ifdef DEE_DEBUG
 DEE_ASSERT(!argc || argv);
 DEE_ASSERT(!refc || refv);
#endif
#if 1
 DeeError_TODONotImplemented(); // TODO
 return -1;
#else
 while ((self->f_wbuf = (char *)malloc_nz(code->co_wsize)) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 struct DeeStringObject *f_file;        /*< [0..1] Current filename (based on debug information). */

 // TODO: Read the stack
 self->f_stackv_end = self->f_stackv;
 self->f_addr;
 // TODO: Read f_this
 // TODO: Read f_result

 self->f_prev = NULL;
 self->f_code = code;
 self->f_argv = argv;
 self->f_refv = refv;
#ifdef DEE_DEBUG
 self->f_argc = argc;
 self->f_refc = refc;
#endif
 DeeAtomicMutex_Init(&self->f_lock);
 self->f_padding = 0;
 self->f_constv = DeeTuple_ELEM(code->co_consts);
 self->f_flags = DEE_STACKFRAME_FLAG_NONE;
 self->f_localv = (DeeObject **)((Dee_uintptr_t)self->f_wbuf+code->co_woff_loc);

#if DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS
 struct DeeExceptionHandlerRef      *f_exceptv;     /*< [1..1][this..f_exceptv_end] List of exception handlers. */
 struct DeeExceptionHandlerRef      *f_exceptv_end; /*< [?..?][0..1] End of the exception handler list. */
 Dee_uint8_t const **f_finally_return_address_end;
#define _DeeStackFrame_FINALLY_RETURN_ADDRESS_BASE(ob)\
 ((Dee_uint8_t const **)((ob).f_wbuf+(ob).f_code->co_woff_finally))
#define _DeeStackFrame_FINALLY_RETURN_ADDRESS_MAX_SIZE(ob)\
 ((ob).f_code->co_finallyc)
#define _DeeStackFrame_FINALLY_RETURN_ADDRESS_SIZE(ob)\
 ((Dee_size_t)((ob).f_finally_return_address_end-_DeeStackFrame_FINALLY_RETURN_ADDRESS_BASE(ob)))
#endif
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int _DeeStackFrame_PutToMarshal(
 DEE_A_IN struct DeeStackFrame const *self, DEE_A_INOUT DeeObject *infile,
 DEE_A_INOUT struct DeeMarshalWriteMap *map) {
 DEE_ASSERT(self); DEE_ASSERT(map);
 DEE_ASSERT(DeeObject_Check(infile) && DeeFile_Check(infile));
 DEE_ASSERT(DeeObject_Check(self->f_code) && DeeCode_Check(self->f_code));
 DEE_ASSERTF((self->f_code->co_flags&DEE_CODE_FLAG_COPY)!=0,"Can't serialize non-copyable code");
 DeeError_TODONotImplemented(); // TODO
 return -1;
}



void _DeeStackFrame_VisitLocked(
 DEE_A_IN struct DeeStackFrame const *self,
 DEE_A_IN DeeVisitProc visit, void *closure) {
 DeeObject **iter,**end;
 struct DeeRaisedException *exc;
 DEE_ASSERT(self && visit);
 //Dee_VISIT(self->f_code); // Not a true reference
 Dee_XVISIT(self->f_this);
 Dee_XVISIT(self->f_result);
 end = (iter = self->f_localv)+self->f_code->co_nlocals;
 while (iter != end) Dee_XVISIT(*iter++); // Visit locals
 iter = self->f_stackv,end = self->f_stackv_end;
 while (iter != end) Dee_VISIT(*iter++); // Visit the stack
 for (exc = self->f_stored_exceptions; exc; exc = exc->re_prev) {
  _DeeRaisedException_Visit(exc); // Visit stored exceptions
 }
}

DEE_A_RET_WUNUSED int _DeeStackFrame_WasExecuted(DEE_A_IN struct DeeStackFrame const *self) {
 DeeObject **iter,**end;
 DEE_ASSERT(self);
 if ((self->f_addr != self->f_code->co_code)  // Code pointer not at code start
  || (self->f_stackv != self->f_stackv_end)   // stack alignment != 0
  || (self->f_result != NULL)                 // result value has been set
#if DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS
  || (self->f_exceptv != self->f_exceptv_end) // Exception handlers have been set
  || (self->f_finally_return_address_end != _DeeStackFrame_FINALLY_RETURN_ADDRESS_BASE(*self)) // Finally return addresses are available
#endif
     ) return 1;
 end = (iter = self->f_localv)+self->f_code->co_nlocals;
 while (iter != end) if (*iter++ != NULL) return 1; // Locals have been modified
 return 0;
}


DeeError_NEW_EXTERN(_DeeRuntimeError_UnboundLocal,&DeeErrorType_UnboundLocal,"Unbound local");
DeeError_NEW_EXTERN(_DeeRuntimeError_Unreachable,&DeeErrorType_IllegalInstruction,"Unreachable code reached"); // todo: Should this get its own error type?
#ifdef DEE_DEBUG
DEE_COMPILER_MSVC_WARNING_PUSH(4102)
#else
DEE_COMPILER_MSVC_WARNING_PUSH(4102 4701 4703)
#endif
DEE_A_RET_EXCEPT(-1) int _DeeStackFrame_RunLocked_AndUnlock(
 DEE_A_INOUT struct DeeThreadObject *thread_self,
 DEE_A_INOUT struct DeeStackFrame *_frame) {
 int result;
 DEE_ASSERTF(DeeObject_Check(thread_self) && DeeThread_Check(thread_self),"Not a valid thread");
 DEE_ASSERTF(thread_self == DeeThread_SELF(),"The given thread isn't thread.self()");
 DEE_ASSERTF(_frame,"Invalid frame");

#define frame (*_frame)

 // Signal, that this is code for a yield function
#define YIELD_FUNCTION

 // Enable optimizations for localizing frame variables
#define OPTIMIZE_STACKEND // localize stack end (removes a lot of indirections)

 // Include the execute code
#ifndef __INTELLISENSE__
#include "_execute.inl"
#endif
#undef frame
exec_end:
 return result;
}
DEE_COMPILER_MSVC_WARNING_POP


#ifdef DEE_DEBUG
DEE_COMPILER_MSVC_WARNING_PUSH(4102 6255)
#else
DEE_COMPILER_MSVC_WARNING_PUSH(4102 6255 4701 4703)
#endif
#ifdef DEE_DEBUG
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeCode_RunDebug(
 DEE_A_IN_OBJECT(DeeCodeObject) *self,
 DEE_A_IN Dee_size_t refc, DEE_A_IN_OPT DeeObject **refv,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_OPT DeeObject **argv,
 DEE_A_IN_OPT DeeObject *this_arg)
#else
DEE_A_RET_EXCEPT_REF DeeObject *DeeCode_Run(
 DEE_A_IN_OBJECT(DeeCodeObject) *self,
 DEE_A_IN_OPT DeeObject **refv,
 DEE_A_IN_OPT DeeObject **argv,
 DEE_A_IN_OPT DeeObject *this_arg)
#endif
{
 struct DeeStackFrame frame;
 DeeThreadObject *thread_self;
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
#if DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS
 frame.f_exceptv_end =
 frame.f_exceptv = (struct DeeExceptionHandlerRef *)(frame.f_wbuf+((DeeCodeObject *)self)->co_woff_except);
 frame.f_finally_return_address_end = (Dee_uint8_t const **)(frame.f_wbuf+((DeeCodeObject *)self)->co_woff_finally);
#endif

 // Initialize the frame lock as already locked
 DeeAtomicMutex_InitLocked(&frame.f_lock);

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


static int _DeeCode_IsStaticID(DeeCodeObject *code, DeeVarID const_id) {
 Dee_uint8_t *code_iter,*code_end,op;
 code_end = (code_iter = DeeCode_CODE(code))+DeeCode_SIZE(code);
 while (code_iter < code_end) switch ((op = *code_iter++)) {
  case OP_LOAD_CST_LOCKED:
  case OP_STORE_CST:
  case OP_STORE_CST_POP: {
   Dee_uint16_t var16 = *(Dee_uint16_t *)code_iter;
   if ((const_id&DEE_UINT32_C(0xFFFF0000))==0 &&
       var16 == (Dee_uint16_t)const_id) return 1;
   code_iter += 2;
  } break;
  case OP_PREFIX: {
   Dee_uint32_t var32;
   Dee_uint16_t significant_work = *(Dee_uint16_t *)code_iter;
   code_iter += 2;
   op = *code_iter++;
   switch (op) {
    case OP_LOAD_CST_LOCKED:
    case OP_STORE_CST:
    case OP_STORE_CST_POP:
     var32 = ((Dee_uint32_t)significant_work << 16) |
              (Dee_uint32_t)*(Dee_uint16_t *)code_iter;
     if (var32 == const_id) return 1;
     code_iter += 2;
     break;
    default: if ((op&OPFLAG_ARG)!=0) code_iter += 2; break;
   }
  } break;
  default: if ((op&OPFLAG_ARG)!=0) code_iter += 2; break;
 }
 return 0;
}
static Dee_size_t _DeeCode_GetArgc(DeeCodeObject *code) {
 Dee_uint8_t *code_iter,*code_end,op;
 Dee_size_t result = 0;
 code_end = (code_iter = DeeCode_CODE(code))+DeeCode_SIZE(code);
 while (code_iter < code_end) switch ((op = *code_iter++)) {
  case OP_LOAD_ARG: {
   Dee_uint16_t var16 = *(Dee_uint16_t *)code_iter;
   if ((Dee_size_t)var16 >= result) result = (Dee_size_t)var16+1;
   code_iter += 2;
  } break;
  case OP_PREFIX: {
   Dee_uint32_t var32;
   Dee_uint16_t significant_work = *(Dee_uint16_t *)code_iter;
   code_iter += 2;
   op = *code_iter++;
   switch (op) {
    case OP_LOAD_ARG:
     var32 = ((Dee_uint32_t)significant_work << 16) |
              (Dee_uint32_t)*(Dee_uint16_t *)code_iter;
     if ((Dee_size_t)var32 >= result) result = (Dee_size_t)var32;
     code_iter += 2;
     break;
    default: if ((op&OPFLAG_ARG)!=0) code_iter += 2; break;
   }
  } break;
  default:
   if ((op&OPFLAG_ARG)!=0) code_iter += 2;
   break;
 }
 return result;
}
static Dee_size_t _DeeCode_GetRefc(DeeCodeObject *code) {
 Dee_uint8_t *code_iter,*code_end,op;
 Dee_size_t result = 0;
 code_end = (code_iter = DeeCode_CODE(code))+DeeCode_SIZE(code);
 while (code_iter < code_end) switch ((op = *code_iter++)) {
  case OP_LOAD_REF: {
   Dee_uint16_t var16 = *(Dee_uint16_t *)code_iter;
   if ((Dee_size_t)var16 >= result) result = (Dee_size_t)var16+1;
   code_iter += 2;
  } break;
  case OP_PREFIX: {
   Dee_uint32_t var32;
   Dee_uint16_t significant_work = *(Dee_uint16_t *)code_iter;
   code_iter += 2;
   op = *code_iter++;
   switch (op) {
    case OP_LOAD_REF:
     var32 = ((Dee_uint32_t)significant_work << 16) |
              (Dee_uint32_t)*(Dee_uint16_t *)code_iter;
     if ((Dee_size_t)var32 >= result) result = (Dee_size_t)var32;
     code_iter += 2;
     break;
    default: if ((op&OPFLAG_ARG)!=0) code_iter += 2; break;
   }
  } break;
  default:
   if ((op&OPFLAG_ARG)!=0) code_iter += 2;
   break;
 }
 return result;
}

#if !defined(OP_LOAD_THIS) && !defined(OPEXT_SUPER_THIS)
#define _DeeCode_HasThis(code) 0
#else
static int _DeeCode_HasThis(DeeCodeObject *code) {
 Dee_uint8_t *code_iter,*code_end,op;
 code_end = (code_iter = DeeCode_CODE(code))+DeeCode_SIZE(code);
 while (code_iter < code_end) switch ((op = *code_iter++)) {
#ifdef OP_LOAD_THIS
  case OP_LOAD_THIS: return 1;
#endif
  default:
   if ((op&OPFLAG_ARG)!=0) code_iter += 2;
   break;
 }
 return 0;
}
#endif


DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL
struct DeeStackFrame *DeeStackFrame_ThreadLocal(void) {
 DeeThreadObject *thread_self;
 if ((thread_self = DeeThread_SELF()) == NULL) return NULL;
 return thread_self->t_frame_last;
}
DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL struct DeeStackFrame *
 DeeStackFrame_Prev(DEE_A_IN struct DeeStackFrame const *self) {
 DEE_ASSERT(self);
 return self->f_prev;
}
DEE_A_RET_WUNUSED Dee_size_t DeeStackFrame_Addr(
 DEE_A_IN struct DeeStackFrame const *self) {
 DEE_ASSERT(self);
 return _DeeStackFrame_Addr(self);
}
DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_OBJECT_NOEXCEPT(DeeStringObject) *
DeeStackFrame_File(DEE_A_IN struct DeeStackFrame const *self) {
 DEE_ASSERT(self);
 return _DeeStackFrame_File(self);
}
DEE_A_RET_WUNUSED Dee_int32_t DeeStackFrame_Line(
 DEE_A_IN struct DeeStackFrame const *self) {
 DEE_ASSERT(self);
 return _DeeStackFrame_Line(self);
}
DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_OBJECT_NOEXCEPT(DeeStringObject) *
DeeStackFrame_Func(DEE_A_IN struct DeeStackFrame const *self) {
 DEE_ASSERT(self);
 return _DeeStackFrame_Func(self);
}
DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_OBJECT(DeeCodeObject) *
DeeStackFrame_Code(DEE_A_IN struct DeeStackFrame const *self) {
 DEE_ASSERT(self);
 return (DeeObject *)self->f_code;
}


DEE_COMPILER_PREFAST_WARNING_PUSH(6101)
DEE_A_RET_EXCEPT(-1) int DeeStackFrame_EnumLocalsEx(
 DEE_A_INOUT struct DeeStackFrame const *self,
 DEE_A_IN DeeStackFrame_LocalsIterFunc func, void *closure,
 DEE_A_OUT_OPT Dee_size_t *total_count, DEE_A_IN Dee_uint16_t valid_types) {
 DeeCodeObject *code; struct DeeCodeDebugInfoObject *debuginfo;
 DeeObject *local_value,*const *begin,*const *iter,*const *end,**namev,*name;
 DeeTupleObject *names; Dee_size_t id,namec,count = 0; int error;
 DEE_ASSERT(self && func);
 code = self->f_code;
 DEE_ASSERT(DeeObject_Check(code) && DeeCode_Check(code));
 debuginfo = code->co_debug;
 DEE_ASSERT(!debuginfo || (DeeObject_Check(debuginfo) && DeeCodeDebugInfo_Check(debuginfo)));
 if ((valid_types&DEE_STACKFRAME_VARTYPE_LOCALS)!=0) {
  end = (iter = begin = self->f_localv)+code->co_nlocals;
  names = debuginfo ? debuginfo->di_local_names : NULL;
  namev = names ? DeeTuple_ELEM(names) : NULL;
  namec = names ? DeeTuple_SIZE(names) : 0;
  while (iter != end) {
   id = (Dee_size_t)(iter-begin);
   DeeAtomicMutex_Acquire(&self->f_lock);
   Dee_XINCREF(local_value = *iter);
   DeeAtomicMutex_Release(&self->f_lock);
   if (id < namec) {
    name = namev[id];
    DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
    if (!DeeString_SIZE(name)) name = NULL;
   } else name = NULL;
   error = (*func)(DEE_STACKFRAME_VARUID_MAKE(DEE_STACKFRAME_VARTYPE_LOCALS,(Dee_uint16_t)id),
                   name ? DeeString_STR(name) : NULL,
                   local_value,closure);
   Dee_XDECREF(local_value);
   if (error != 0)  return error;
   ++count;
   ++iter;
  }
 }
 if ((valid_types&DEE_STACKFRAME_VARTYPE_STATIC)!=0) {
  end = (iter = begin = DeeTuple_ELEM(code->co_consts)
         )+DeeTuple_SIZE(code->co_consts);
  names = debuginfo ? debuginfo->di_const_names : NULL;
  namev = names ? DeeTuple_ELEM(names) : NULL;
  namec = names ? DeeTuple_SIZE(names) : 0;
  while (iter != end) {
   id = (Dee_size_t)(iter-begin);
   if (_DeeCode_IsStaticID(code,(Dee_uint16_t)id)) {
    DeeAtomicMutex_Acquire(&code->co_consts_lock);
    Dee_INCREF(local_value = *iter);
    DeeAtomicMutex_Release(&code->co_consts_lock);
    if (id < namec) {
     name = namev[id];
     DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
     if (!DeeString_SIZE(name)) name = NULL;
    } else name = NULL;
    error = (*func)(DEE_STACKFRAME_VARUID_MAKE(DEE_STACKFRAME_VARTYPE_STATIC,(Dee_uint16_t)id),
                    name ? DeeString_STR(name) : NULL,
                    local_value,closure);
    Dee_DECREF(local_value);
    if (error != 0)  return error;
    ++count;
   }
   ++iter;
  }
 }
 if ((valid_types&DEE_STACKFRAME_VARTYPE_ARGS)!=0) {
  Dee_size_t codeargc;
  names = debuginfo ? debuginfo->di_arg_names : NULL;
  iter = begin = self->f_argv;
  codeargc = _DeeCode_GetArgc(code);
  namev = names ? DeeTuple_ELEM(names) : NULL;
  namec = names ? DeeTuple_SIZE(names) : 0;
  end = begin+Dee_MAX(namec,codeargc);
  while (iter != end) {
   id = (Dee_size_t)(iter-begin);
   DeeAtomicMutex_Acquire(&self->f_lock);
   Dee_INCREF(local_value = *iter);
   DeeAtomicMutex_Release(&self->f_lock);
   if (id < namec) {
    name = namev[id];
    DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
    if (!DeeString_SIZE(name)) name = NULL;
   } else name = NULL;
   error = (*func)(DEE_STACKFRAME_VARUID_MAKE(DEE_STACKFRAME_VARTYPE_ARGS,(Dee_uint16_t)id),
                   name ? DeeString_STR(name) : NULL,
                   local_value,closure);
   Dee_DECREF(local_value);
   if (error != 0)  return error;
   ++count;
   ++iter;
  }
 }
 if ((valid_types&DEE_STACKFRAME_VARTYPE_REFS)!=0) {
  Dee_size_t coderefc;
  names = debuginfo ? debuginfo->di_ref_names : NULL;
  iter = begin = self->f_refv;
  coderefc = _DeeCode_GetRefc(code);
  namev = names ? DeeTuple_ELEM(names) : NULL;
  namec = names ? DeeTuple_SIZE(names) : 0;
  end = begin+Dee_MAX(namec,coderefc);
  while (iter != end) {
   id = (Dee_size_t)(iter-begin);
   DeeAtomicMutex_Acquire(&self->f_lock);
   Dee_INCREF(local_value = *iter);
   DeeAtomicMutex_Release(&self->f_lock);
   if (id < namec) {
    name = namev[id];
    DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
    if (!DeeString_SIZE(name)) name = NULL;
   } else name = NULL;
   error = (*func)(DEE_STACKFRAME_VARUID_MAKE(DEE_STACKFRAME_VARTYPE_REFS,(Dee_uint16_t)id),
                   name ? DeeString_STR(name) : NULL,
                   local_value,closure);
   Dee_DECREF(local_value);
   if (error != 0)  return error;
   ++count;
   ++iter;
  }
 }
 if ((valid_types&DEE_STACKFRAME_VARTYPE_THIS)!=0) {
  static char const this_name[] = "this";
  DeeAtomicMutex_Acquire(&self->f_lock);
  Dee_XINCREF(local_value = self->f_this);
  DeeAtomicMutex_Release(&self->f_lock);
  if (local_value) {
   error = (*func)(DEE_STACKFRAME_VARUID_MAKE(DEE_STACKFRAME_VARTYPE_THIS,0),
                   this_name,local_value,closure);
   Dee_DECREF(local_value);
   ++count;
  } else if (_DeeCode_HasThis(code)) {
   error = (*func)(DEE_STACKFRAME_VARUID_MAKE(DEE_STACKFRAME_VARTYPE_THIS,0),
                   this_name,NULL,closure);
   ++count;
  } else error = 0;
  if (error != 0) return error;
 }
 if (total_count) *total_count = count;
 return 0;
}
DEE_COMPILER_PREFAST_WARNING_POP


DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,1) int) _DeeStackFrame_NameToIDNoMissing(
 DEE_A_INOUT struct DeeStackFrame const *self, DEE_A_IN_Z char const *name,
 DEE_A_IN Dee_uint16_t valid_types, DEE_A_OUT DeeVarUID *uid);
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeStackFrame_NameToID(
 DEE_A_INOUT struct DeeStackFrame const *self, DEE_A_IN_Z char const *name,
 DEE_A_IN Dee_uint16_t valid_types, DEE_A_OUT DeeVarUID *uid) {
 int result;
 if ((result = _DeeStackFrame_NameToIDNoMissing(self,name,valid_types,uid)) == 1) {
  DeeError_SetStringf(&DeeErrorType_AttributeError,
                      "Unknown variable: %q",name);
  result = -1;
 }
 return result;
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,1) int) _DeeStackFrame_NameToIDNoMissing(
 DEE_A_INOUT struct DeeStackFrame const *self, DEE_A_IN_Z char const *name,
 DEE_A_IN Dee_uint16_t valid_types, DEE_A_OUT DeeVarUID *uid) {
 DeeCodeObject *code; struct DeeCodeDebugInfoObject *debuginfo;
 DeeObject **namev; DeeTupleObject *names; Dee_size_t i,s; Dee_uint16_t id;
 DEE_ASSERT(self);
 code = self->f_code;
 DEE_ASSERT(DeeObject_Check(code) && DeeCode_Check(code));
 debuginfo = code->co_debug;
 DEE_ASSERT(!debuginfo || (DeeObject_Check(debuginfo) && DeeCodeDebugInfo_Check(debuginfo)));
 if (*name && name[1] == ':') {
  switch (*name) {
   case 'L': case 'l': valid_types &= DEE_STACKFRAME_VARTYPE_LOCALS; break;
   case 'C': case 'c': valid_types &= DEE_STACKFRAME_VARTYPE_STATIC; break;
   case 'A': case 'a': valid_types &= DEE_STACKFRAME_VARTYPE_ARGS; break;
   case 'R': case 'r': valid_types &= DEE_STACKFRAME_VARTYPE_REFS; break;
   default:
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        "Unknown name prefix in %q",
                        name);
    return -1;
   // Custom prefix
  }
  name += 2;
 } else valid_types &= DEE_STACKFRAME_VARTYPE_ANY;
#define RETURN_UID(uid_) do{*uid=(uid_);return 0;}while(0)
 if (debuginfo) {
  if ((valid_types&DEE_STACKFRAME_VARTYPE_LOCALS)!=0 &&
      (names = debuginfo->di_local_names) != NULL) {
   namev = DeeTuple_ELEM(names);
   s = Dee_MIN(DeeTuple_SIZE(names),code->co_nlocals);
   for (i = 0; i < s; ++i)
    if (strcmp(DeeString_STR(namev[i]),name) == 0)
     RETURN_UID(DEE_STACKFRAME_VARUID_MAKE(DEE_STACKFRAME_VARTYPE_LOCALS,i));
  }
  if ((valid_types&DEE_STACKFRAME_VARTYPE_STATIC)!=0 &&
      (names = debuginfo->di_const_names) != NULL) {
   namev = DeeTuple_ELEM(names);
   s = Dee_MIN(DeeTuple_SIZE(names),DeeTuple_SIZE(code->co_consts));
   for (i = 0; i < s; ++i)
    if (strcmp(DeeString_STR(namev[i]),name) == 0)
     RETURN_UID(DEE_STACKFRAME_VARUID_MAKE(DEE_STACKFRAME_VARTYPE_STATIC,i));
  }
  if ((valid_types&DEE_STACKFRAME_VARTYPE_ARGS)!=0 &&
      (names = debuginfo->di_arg_names) != NULL) {
   namev = DeeTuple_ELEM(names);
   s = DeeTuple_SIZE(names);
   for (i = 0; i < s; ++i)
    if (strcmp(DeeString_STR(namev[i]),name) == 0)
     RETURN_UID(DEE_STACKFRAME_VARUID_MAKE(DEE_STACKFRAME_VARTYPE_ARGS,i));
  }
  if ((valid_types&DEE_STACKFRAME_VARTYPE_REFS)!=0 &&
      (names = debuginfo->di_ref_names) != NULL) {
   namev = DeeTuple_ELEM(names);
   s = DeeTuple_SIZE(names);
   for (i = 0; i < s; ++i)
    if (strcmp(DeeString_STR(namev[i]),name) == 0)
     RETURN_UID(DEE_STACKFRAME_VARUID_MAKE(DEE_STACKFRAME_VARTYPE_ARGS,i));
  }
 }
 if ((valid_types&DEE_STACKFRAME_VARTYPE_THIS)!=0) {
  if (strcmp(name,"this") == 0)
   RETURN_UID(DEE_STACKFRAME_VARUID_MAKE(DEE_STACKFRAME_VARTYPE_THIS,0));
 }
#undef RETURN_UID
 // Last chance: Is it an integer
 if (DeeString_ToUInt16(name,&id) == 0) {
  *uid = DEE_STACKFRAME_VARUID_MAKE(valid_types,id);
  return 0;
 }
 return -1;
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeStackFrame_GetLocalEx(
 DEE_A_INOUT struct DeeStackFrame const *self,
 DEE_A_IN_Z char const *name, DEE_A_IN Dee_uint16_t valid_types) {
 DeeVarUID uid;
 if (_DeeStackFrame_NameToID(self,name,valid_types,&uid) != 0) return NULL;
 return DeeStackFrame_GetLocalID(self,uid);
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeStackFrame_DelLocalEx(
 DEE_A_INOUT struct DeeStackFrame *self,
 DEE_A_IN_Z char const *name, DEE_A_IN Dee_uint16_t valid_types) {
 DeeVarUID uid;
 if (_DeeStackFrame_NameToID(self,name,valid_types,&uid) != 0) return -1;
 return DeeStackFrame_DelLocalID(self,uid);
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeStackFrame_HasLocalEx(
 DEE_A_INOUT struct DeeStackFrame const *self,
 DEE_A_IN_Z char const *name, DEE_A_IN Dee_uint16_t valid_types) {
 DeeVarUID uid;
 if (_DeeStackFrame_NameToIDNoMissing(self,name,valid_types,&uid) != 0) return 1;
 return DeeStackFrame_HasLocalID(self,uid);
}
DEE_A_RET_EXCEPT(-1) int DeeStackFrame_SetLocalEx(
 DEE_A_INOUT struct DeeStackFrame *self, DEE_A_IN_Z char const *name,
 DEE_A_INOUT DeeObject *value, DEE_A_IN Dee_uint16_t valid_types) {
 DeeVarUID uid;
 if (_DeeStackFrame_NameToID(self,name,valid_types,&uid) != 0) return -1;
 return DeeStackFrame_SetLocalID(self,uid,value);
}




DEE_A_RET_EXCEPT_REF DeeObject *DeeStackFrame_GetLocalID(
 DEE_A_INOUT struct DeeStackFrame const *self, DEE_A_IN DeeVarUID uid) {
 DeeObject *result; DeeVarID id;
 DEE_ASSERT(self);
 id = DEE_STACKFRAME_VARUID_ID(uid);
 switch (DEE_STACKFRAME_VARUID_TYPE(uid)) {
  case DEE_STACKFRAME_VARTYPE_LOCALS:
   if (id >= self->f_code->co_nlocals) {
    DeeError_SetStringf(&DeeErrorType_AttributeError,
                        "Unknown Variable: l:%I16u",id);
    return NULL;
   }
   DeeAtomicMutex_Acquire(&self->f_lock);
   Dee_XINCREF(result = self->f_localv[id]);
   DeeAtomicMutex_Release(&self->f_lock);
   if (!result) {
    DeeError_SetStringf(&DeeErrorType_UnboundLocal,
                        "Unbound Variable: l:%I16u",id);
   }
   break;

  case DEE_STACKFRAME_VARTYPE_STATIC:
   if (!_DeeCode_IsStaticID(self->f_code,id)) {
    DeeError_SetStringf(&DeeErrorType_AttributeError,
                        "Unknown Variable: c:%I16u",id);
    return NULL;
   }
   DeeAtomicMutex_Acquire(&self->f_code->co_consts_lock);
   Dee_INCREF(result = DeeTuple_GET(self->f_code->co_consts,id));
   DeeAtomicMutex_Release(&self->f_code->co_consts_lock);
   break;

  case DEE_STACKFRAME_VARTYPE_ARGS:
   if (id >= _DeeCode_GetArgc(self->f_code)) {
    DeeError_SetStringf(&DeeErrorType_AttributeError,
                        "Unknown Variable: a:%I16u",id);
    return NULL;
   }
   DeeAtomicMutex_Acquire(&self->f_lock);
   Dee_INCREF(result = self->f_argv[id]);
   DeeAtomicMutex_Release(&self->f_lock);
   break;

  case DEE_STACKFRAME_VARTYPE_REFS:
   if (id >= _DeeCode_GetRefc(self->f_code)) {
    DeeError_SetStringf(&DeeErrorType_AttributeError,
                        "Unknown Variable: r:%I16u",id);
    return NULL;
   }
   DeeAtomicMutex_Acquire(&self->f_lock);
   Dee_INCREF(result = self->f_refv[id]);
   DeeAtomicMutex_Release(&self->f_lock);
   break;

  case DEE_STACKFRAME_VARTYPE_THIS:
   if (id != 0) {
    DeeError_SetStringf(&DeeErrorType_AttributeError,
                        "Unknown Variable: 'this' has id 0 (not: %I16u)",id);
    return NULL;
   }
   DeeAtomicMutex_Acquire(&self->f_lock);
   Dee_XINCREF(result = self->f_this);
   DeeAtomicMutex_Release(&self->f_lock);
   if (!result) {
    DeeError_SET_STRING(&DeeErrorType_UnboundLocal,
                        "Unbound Variable: this");
   }
   break;

  default:
   DeeError_SET_STRING(&DeeErrorType_AttributeError,
                       "Unsupported Variable Type for 'get'");
   return NULL;
 }
 return result;
}


DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeStackFrame_DelLocalID(
 DEE_A_INOUT struct DeeStackFrame *self, DEE_A_IN DeeVarUID uid) {
 DeeObject *old_value; DeeVarID id;
 DEE_ASSERT(self);
 id = DEE_STACKFRAME_VARUID_ID(uid);
 switch (DEE_STACKFRAME_VARUID_TYPE(uid)) {
  case DEE_STACKFRAME_VARTYPE_LOCALS:
   if (id >= self->f_code->co_nlocals) {
    DeeError_SetStringf(&DeeErrorType_AttributeError,
                        "Unknown Variable: l:%I16u",id);
    return -1;
   }
   DeeAtomicMutex_Acquire(&self->f_lock);
   old_value = self->f_localv[id];
   self->f_localv[id] = NULL;
   DeeAtomicMutex_Release(&self->f_lock);
   if (!old_value) return 1;
   Dee_DECREF(old_value);
   break;

  default:
   DeeError_SET_STRING(&DeeErrorType_AttributeError,
                       "Unsupported Variable Type for 'del'");
   return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeStackFrame_HasLocalID(
 DEE_A_INOUT struct DeeStackFrame const *self, DEE_A_IN DeeVarUID uid) {
 DeeObject *result; DeeVarID id;
 DEE_ASSERT(self);
 id = DEE_STACKFRAME_VARUID_ID(uid);
 switch (DEE_STACKFRAME_VARUID_TYPE(uid)) {
  case DEE_STACKFRAME_VARTYPE_LOCALS:
   if (id >= self->f_code->co_nlocals) return 1;
   DeeAtomicMutex_Acquire(&self->f_lock);
   result = self->f_localv[id];
   DeeAtomicMutex_Release(&self->f_lock);
   return result ? 0 : 1;

  case DEE_STACKFRAME_VARTYPE_STATIC:
   return _DeeCode_IsStaticID(self->f_code,id);

  case DEE_STACKFRAME_VARTYPE_ARGS:
   return id < _DeeCode_GetArgc(self->f_code);

  case DEE_STACKFRAME_VARTYPE_REFS:
   return id < _DeeCode_GetRefc(self->f_code);

  case DEE_STACKFRAME_VARTYPE_THIS:
   return id == 0;

  default:
   DeeError_SET_STRING(&DeeErrorType_AttributeError,
                       "Unsupported Variable Type for 'has'");
   return -1;
 }
}
DEE_A_RET_EXCEPT(-1) int DeeStackFrame_SetLocalID(
 DEE_A_INOUT struct DeeStackFrame *self,
 DEE_A_IN DeeVarUID uid, DEE_A_INOUT DeeObject *value) {
 DeeObject *old_value; DeeVarID id;
 DEE_ASSERT(self);
 id = DEE_STACKFRAME_VARUID_ID(uid);
 switch (DEE_STACKFRAME_VARUID_TYPE(uid)) {
  case DEE_STACKFRAME_VARTYPE_LOCALS:
   if (id >= self->f_code->co_nlocals) {
    DeeError_SetStringf(&DeeErrorType_AttributeError,
                        "Unknown Variable: l:%I16u",id);
    return -1;
   }
   Dee_INCREF(value);
   DeeAtomicMutex_Acquire(&self->f_lock);
   old_value = self->f_localv[id];
   self->f_localv[id] = value;
   DeeAtomicMutex_Release(&self->f_lock);
   Dee_XDECREF(old_value);
   break;

  case DEE_STACKFRAME_VARTYPE_STATIC:
   if (!_DeeCode_IsStaticID(self->f_code,id)) {
    DeeError_SetStringf(&DeeErrorType_AttributeError,
                        "Unknown Variable: c:%I16u",id);
    return -1;
   }
   Dee_INCREF(value);
   DeeAtomicMutex_Acquire(&self->f_code->co_consts_lock);
   old_value = DeeTuple_GET(self->f_code->co_consts,id);
   DeeTuple_SET(self->f_code->co_consts,id,value);
   DeeAtomicMutex_Release(&self->f_code->co_consts_lock);
   Dee_XDECREF(old_value);
   break;

  case DEE_STACKFRAME_VARTYPE_REFS:
   if (id >= _DeeCode_GetRefc(self->f_code)) {
    DeeError_SetStringf(&DeeErrorType_AttributeError,
                        "Unknown Variable: r:%I16u",id);
    return -1;
   }
   Dee_INCREF(value);
   DeeAtomicMutex_Acquire(&self->f_lock);
   old_value = self->f_refv[id];
   self->f_refv[id] = value;
   DeeAtomicMutex_Release(&self->f_lock);
   Dee_DECREF(old_value);
   break;

  case DEE_STACKFRAME_VARTYPE_THIS:
   if (id != 0) {
    DeeError_SetStringf(&DeeErrorType_AttributeError,
                        "Unknown Variable: 'this' has id 0 (not: %I16u)",id);
    return -1;
   }
   Dee_INCREF(value);
   DeeAtomicMutex_Acquire(&self->f_lock);
   old_value = self->f_this;
   self->f_this = value;
   DeeAtomicMutex_Release(&self->f_lock);
   Dee_DECREF(old_value);
   break;

  default:
   DeeError_SET_STRING(&DeeErrorType_AttributeError,
                       "Unsupported Variable Type for 'set'");
   return -1;
 }
 return 0;
}




DEE_A_RET_EXCEPT(-1) int DeeStackFrame_InteractiveDebugger(void) {
 DeeThreadObject *thread_self;
 if ((thread_self = DeeThread_SELF()) == NULL) return -1;
 if (!thread_self->t_frame_last) {
  DeeError_SET_STRING(&DeeErrorType_ValueError,
                      "Can't enter debugger: No execution frame found");
  return -1;
 }
 return DeeStackFrame_InteractiveDebuggerEx((
  DeeObject *)thread_self,thread_self->t_frame_last);
}

DEE_A_RET_EXCEPT(-1) int DeeStackFrame_InteractiveDebuggerEx(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *thread_self,
 DEE_A_INOUT struct DeeStackFrame *_frame) {
 typedef int (DEE_CALL *PDEEDEBUGGERRUN)(
  DEE_A_INOUT DeeThreadObject *thread_self,
  DEE_A_INOUT struct DeeStackFrame *frame_);
 DeeObject *debugger_dex,*result; int error;
 PDEEDEBUGGERRUN PDeeDebugger_Run;
 DEE_ASSERTF(DeeObject_Check(thread_self) && DeeThread_Check(thread_self),
             "thread_self is not a thread");
 DEE_ASSERTF((DeeThreadObject *)thread_self == DeeThread_SELF(),
             "thread_self must be equal to DeeThread_SELF()");
 if DEE_UNLIKELY((debugger_dex = DeeDex_Open("_debugger")) == NULL) return -1;
DEE_COMPILER_MSVC_WARNING_PUSH(4055)
 PDeeDebugger_Run = (PDEEDEBUGGERRUN)DeeDexModule_GetNativeAddress(debugger_dex,"DeeDebugger_Run");
DEE_COMPILER_MSVC_WARNING_POP
 if DEE_LIKELY(PDeeDebugger_Run) {
  error = (*PDeeDebugger_Run)((DeeThreadObject *)thread_self,_frame);
  Dee_DECREF(debugger_dex);
  return error;
 }
 DeeError_HandledOne();
 // So far, so bad... (This might be a custom debugger that doesn't implement the native run function)
 // >> Lets try again by calling the deemon-style version that should also be present
 result = DeeObject_CallAttrStringf(debugger_dex,"breakpoint","");
 Dee_DECREF(debugger_dex);
 if DEE_UNLIKELY(!result) return -1; // Nope! Either that one was missing too, or it failed.
 Dee_DECREF(result);
 return 0;
}


DEE_DECL_END

#endif /* !GUARD_DEEMON_EXECUTE_C */
