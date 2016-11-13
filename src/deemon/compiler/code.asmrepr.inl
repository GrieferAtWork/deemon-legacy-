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
#ifndef GUARD_DEEMON_CODE_ASMREPR_INL
#define GUARD_DEEMON_CODE_ASMREPR_INL  1
#define DEE_LIMITED_API 1

// /include/*
#include <deemon/__conf.inl>
#include <deemon/compiler/code.h>
#include <deemon/runtime/builtins.h>
#include <deemon/runtime/execute.h>
#include <deemon/string.h>
#include <deemon/tuple.h>

// /src/*
#include <deemon/compiler/__opcode.inl>

// */ (nano...)

DEE_DECL_BEGIN

DEE_STATIC_INLINE(char const *) _DeeCode_LocalName(
 DEE_A_IN_OBJECT(DeeCodeObject) const *self, DEE_A_IN Dee_size_t id) {
 DeeObject *local_name;
 DeeCodeDebugInfoObject *debug_info;
 debug_info = ((DeeCodeObject *)self)->co_debug;
 if (debug_info && debug_info->di_local_names &&
     id < DeeTuple_SIZE(debug_info->di_local_names)) {
  local_name = DeeTuple_GET(debug_info->di_local_names,id);
  if (DeeString_Check(local_name)) return DeeString_STR(local_name);
 }
 return "?";
}
DEE_STATIC_INLINE(char const *) _DeeCode_RefName(
 DEE_A_IN_OBJECT(DeeCodeObject) const *self, DEE_A_IN Dee_size_t id) {
 DeeObject *local_name;
 DeeCodeDebugInfoObject *debug_info;
 debug_info = ((DeeCodeObject *)self)->co_debug;
 if (debug_info && debug_info->di_ref_names &&
     id < DeeTuple_SIZE(debug_info->di_ref_names)) {
  local_name = DeeTuple_GET(debug_info->di_ref_names,id);
  if (DeeString_Check(local_name)) return DeeString_STR(local_name);
 }
 return "?";
}
DEE_STATIC_INLINE(char const *) _DeeCode_ArgName(
 DEE_A_IN_OBJECT(DeeCodeObject) const *self, DEE_A_IN Dee_size_t id) {
 DeeObject *local_name;
 DeeCodeDebugInfoObject *debug_info;
 debug_info = ((DeeCodeObject *)self)->co_debug;
 if (debug_info && debug_info->di_arg_names &&
     id < DeeTuple_SIZE(debug_info->di_arg_names)) {
  local_name = DeeTuple_GET(debug_info->di_arg_names,id);
  if (DeeString_Check(local_name)) return DeeString_STR(local_name);
 }
 return "?";
}
DEE_STATIC_INLINE(char const *) _DeeCode_ConstName(
 DEE_A_IN_OBJECT(DeeCodeObject) const *self, DEE_A_IN Dee_size_t id) {
 DeeObject *local_name;
 DeeCodeDebugInfoObject *debug_info;
 debug_info = ((DeeCodeObject *)self)->co_debug;
 if (debug_info && debug_info->di_const_names &&
     id < DeeTuple_SIZE(debug_info->di_const_names)) {
  local_name = DeeTuple_GET(debug_info->di_const_names,id);
  if (DeeString_Check(local_name)) return DeeString_STR(local_name);
 }
 return "?";
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeCode_AsmRepr(DEE_A_IN_OBJECT(DeeCodeObject) const *self) {
 Dee_uint8_t const *code,*end; Dee_int16_t arg;
 Dee_uint16_t uarg;
 DeeObject *result,*temp;
 DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(DeeObject_Check(self) && DeeCode_Check(self));
 end = (code = DeeCode_CODE(self))+DeeCode_SIZE(self);
#define put(s)    do{if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,s)!=0)goto err;}while(0)
#define putf(...) do{if DEE_UNLIKELY(DeeStringWriter_Writef(&writer,__VA_ARGS__)!=0)goto err;}while(0)
 while (code != end) {
  DEE_ASSERT(code < end);
  putf("+%.4Ix ",code-DeeCode_CODE(self));
#define RT_READ_ARG()           *(*(Dee_uint16_t **)&code)++
#define RT_READ_SARG()          *(*(Dee_int16_t **)&code)++
  switch (*code++) {
   case OP_RETNONE          : put("ret none"); break;
   case OP_NOOP             : put("noop"); break;
   case OP_POP              : put("pop 1"); break;
   case OP_RET              : put("ret pop 1"); break;
   case OP_STOP             : put("stop"); break;
   case OP_RETVAREXIT       : put("exit"); break;
   case OP_YIELD            : put("yield pop 1"); break;
   case OP_YIELDALL         : put("yield_all pop 1"); break;
   case OP_YIELDSTOP        : put("yield_stop"); break;
   case OP_LOAD_NONE        : put("push none"); break;
   case OP_COPY             : put("push @op __copy__ pop 1"); break;
   case OP_MOVE             : put("push @op __move__ pop 1"); break;
   case OP_TYPE             : put("push @op __type__ pop 1"); break;
   case OP_BOOL             : put("push @op __bool__ pop 1"); break;
   case OP_WEAK             : put("push @op __weak__ pop 1"); break;
   case OP_IS               : put("push @op __is__ pop 2"); break;
   case OP_IN               : put("push @op __in__ pop 2"); break;
   case OP_CALL             : put("push @op __call__ pop 2"); break;
   case OP_COPY_ASSIGN      : put("push @op __copyassign__ pop 2"); break;
   case OP_STR              : put("push @op __str__ pop 1"); break;
   case OP_REPR             : put("push @op __repr__ pop 1"); break;
   case OP_NOT              : put("push @op __not__ pop 1"); break;
   case OP_POS              : put("push @op __pos__ pop 1"); break;
   case OP_NEG              : put("push @op __neg__ pop 1"); break;
   case OP_REF              : put("push @op __ref__ pop 1"); break;
   case OP_DEREF            : put("push @op __deref__ pop 1"); break;
   case OP_INV              : put("push @op __inv__ pop 1"); break;
   case OP_INC              : put("push @op __inc__ pop 1"); break;
   case OP_DEC              : put("push @op __dec__ pop 1"); break;
   case OP_INC_POST         : put("push @op __incpost__ pop 1"); break;
   case OP_DEC_POST         : put("push @op __decpost__ pop 1"); break;
   case OP_ADD              : put("push @op __add__ pop 2"); break;
   case OP_INPLACE_ADD      : put("push @op __iadd__ pop 2"); break;
   case OP_SUB              : put("push @op __sub__ pop 2"); break;
   case OP_INPLACE_SUB      : put("push @op __isub__ pop 2"); break;
   case OP_MUL              : put("push @op __mul__ pop 2"); break;
   case OP_INPLACE_MUL      : put("push @op __imul__ pop 2"); break;
   case OP_DIV              : put("push @op __div__ pop 2"); break;
   case OP_INPLACE_DIV      : put("push @op __idiv__ pop 2"); break;
   case OP_MOD              : put("push @op __mod__ pop 2"); break;
   case OP_INPLACE_MOD      : put("push @op __imod__ pop 2"); break;
   case OP_SHL              : put("push @op __shl__ pop 2"); break;
   case OP_INPLACE_SHL      : put("push @op __ishl__ pop 2"); break;
   case OP_SHR              : put("push @op __shr__ pop 2"); break;
   case OP_INPLACE_SHR      : put("push @op __ishr__ pop 2"); break;
   case OP_AND              : put("push @op __and__ pop 2"); break;
   case OP_INPLACE_AND      : put("push @op __iand__ pop 2"); break;
   case OP_OR               : put("push @op __or__ pop 2"); break;
   case OP_INPLACE_OR       : put("push @op __ior__ pop 2"); break;
   case OP_XOR              : put("push @op __xor__ pop 2"); break;
   case OP_INPLACE_XOR      : put("push @op __ixor__ pop 2"); break;
   case OP_POW              : put("push @op __pow__ pop 2"); break;
   case OP_INPLACE_POW      : put("push @op __ipow__ pop 2"); break;
   case OP_CMP_LO           : put("push @op __lo__ pop 2"); break;
   case OP_CMP_LE           : put("push @op __le__ pop 2"); break;
   case OP_CMP_EQ           : put("push @op __eq__ pop 2"); break;
   case OP_CMP_NE           : put("push @op __ne__ pop 2"); break;
   case OP_CMP_GR           : put("push @op __gr__ pop 2"); break;
   case OP_CMP_GE           : put("push @op __ge__ pop 2"); break;
   case OP_SEQ_GET          : put("push @op __getitem__ pop 2"); break;
   case OP_SEQ_DEL          : put("@op __delitem__ pop 2"); break;
   case OP_SEQ_SET          : put("push @op __setitem__ pop 3"); break;
   case OP_SEQ_SIZE         : put("push @op __size__ pop 1"); break;
   case OP_SEQ_RANGE_GET    : put("push @op __getrange__ pop 3"); break;
   case OP_SEQ_RANGE_DEL    : put("@op __delrange__ pop 3"); break;
   case OP_SEQ_RANGE_SET    : put("push @op __setrange__ pop 4"); break;
   case OP_SEQ_ITER_SELF    : put("push @op __iterself__ pop 1"); break;
   case OP_ATTR_GET         : put("push @op __getattr__ pop 2"); break;
   case OP_ATTR_DEL         : put("@op __delattr__ pop 2"); break;
   case OP_ATTR_SET         : put("push @op __setattr__ pop 3"); break;
   case OP_ROT_2            : put("rot 2"); break;
   case OP_LROT_3           : put("rot << 3"); break;
   case OP_TRY_END          : put("try @end"); break;
   case OP_EXCEPT_END       : put("try except @end"); break;
   case OP_FINALLY_END      : put("try finally @end"); break;
   case OP_THROW            : put("throw pop 1"); break;
   case OP_RETHROW          : put("throw @last_exception"); break;
   case OP_CONCAT_TUPLE     : put("push @tuple.concat pop 2"); break;
   case OP_CONCAT_LIST      : put("push @list.concat pop 2"); break;
   case OP_CAST_LIST        : put("push @list pop 1"); break;
   case OP_CAST_TUPLE       : put("push @tuple pop 1"); break;
   case OP_LOAD_RET         : put("push @return"); break;
   case OP_STORE_RET        : put("store @return"); break;
   case OP_STORE_RET_POP    : put("pop store @return"); break;
   case OP_UNREACHABLE      : put("UNREACHABLE"); break;
   case OP_LOAD_THIS        : put("push @this"); break;
   case OP_DUP              : put("dup top"); break;
   case OP_JUMP_DYN_REL     : put("jmp +pop(0)"); break;
   case OP_JUMP_DYN_ABS     : put("jmp @pop(0)"); break;
   case OP_PRINT_ONE        : put("print stdout: pop(0)"); break;
   case OP_PRINT_ONE_SEP    : put("print stdout: pop(0),\" \""); break;
   case OP_PRINT_ONE_END    : put("print stdout: pop(0),\"\\n\""); break;
   case OP_PRINT_ONE_F      : put("print pop(1): pop(0)"); break;
   case OP_PRINT_ONE_F_SEP  : put("print second: pop(0),\" \""); break;
   case OP_PRINT_ONE_F_END  : put("print pop(1): pop(0),\"\\n\""); break;
   case OP_PRINT_ALL        : put("print stdout: pop(0)..."); break;
   case OP_PRINT_ALL_SEP    : put("print stdout: pop(0)...,\" \""); break;
   case OP_PRINT_ALL_END    : put("print stdout: pop(0)...,\"\\n\""); break;
   case OP_PRINT_ALL_F      : put("print pop(1): pop(0)..."); break;
   case OP_PRINT_ALL_F_SEP  : put("print second: pop(0)...,\" \""); break;
   case OP_PRINT_ALL_F_END  : put("print pop(1): pop(0)...,\"\\n\""); break;
   case OP_PRINT_END        : put("print stdout: \"\\n\""); break;
   case OP_PRINT_END_F      : put("print pop(0): \"\\n\""); break;
   case OP_NEW_INSTANCE     : put("push @op __new_instance__ pop 2"); break;
   case OP_CELL             : put("push @op __cell__ pop 1"); break;
   case OP_LROT_4           : put("rot << 4"); break;
   case OP_ATTR_HAS         : put("push @op __hasattr__ pop 2"); break;
   case OP_MOVE_ASSIGN      : put("push @op __moveassign__ pop 2"); break;
   case OP_RROT_3           : put("rot >> 3"); break;
   case OP_RROT_4           : put("rot >> 4"); break;
   case OP_CAST_SET         : put("push @set pop 1"); break;
   case OP_LOAD_EXTERN      : put("push @op __builtin_extern pop 3"); break;
   case OP_LOAD_AUTOEXTERN  : put("push @op __builtin_extern pop 2"); break;
   case OP_FF_CLOSURE       : put("push @op __builtin_ff_closure pop 2"); break;

   case OP_LOAD_LOC         : uarg = RT_READ_ARG(); putf("push @local %s@%u",_DeeCode_LocalName(self,uarg),(unsigned)uarg); break;
   case OP_LOAD_REF         : uarg = RT_READ_ARG(); putf("push @ref %s@%u",_DeeCode_RefName(self,uarg),(unsigned)uarg); break;
   {
    char const *prefix;
    if (0) { case OP_LOAD_CST:        prefix = "";           }
    if (0) { case OP_LOAD_CST_COPY:   prefix = "copy ";      }
    if (0) { case OP_LOAD_CST_DCOPY:  prefix = "deep_copy "; }
    if (0) { case OP_LOAD_CST_LOCKED: prefix = "locked ";    }
    uarg = RT_READ_ARG();
    temp = DeeTuple_GET(((DeeCodeObject *)self)->co_consts,uarg);
    if (0) { case OP_LOAD_BLTIN: temp = DeeBuiltin_GET(RT_READ_ARG()); prefix = ""; }
    putf("push %s{%r}",prefix,temp);
   } break;
   case OP_LOAD_ARG         : uarg = RT_READ_ARG(); putf("push @arg %s@%u",_DeeCode_ArgName(self,uarg),(unsigned)uarg); break;
   case OP_STORE_LOC        : uarg = RT_READ_ARG(); putf("store @local %s@%u",_DeeCode_LocalName(self,uarg),(unsigned)uarg); break;
   case OP_STORE_LOC_POP    : uarg = RT_READ_ARG(); putf("pop store @local %s@%u",_DeeCode_LocalName(self,uarg),(unsigned)uarg); break;
   case OP_STORE_CST        : uarg = RT_READ_ARG(); putf("locked store @const %s@%u",_DeeCode_ConstName(self,uarg),(unsigned)uarg); break;
   case OP_STORE_CST_POP    : uarg = RT_READ_ARG(); putf("pop store @const %s@%u",_DeeCode_ConstName(self,uarg),(unsigned)uarg); break;
   case OP_DEL_LOCAL        : uarg = RT_READ_ARG(); putf("del @local %s@%u",_DeeCode_LocalName(self,uarg),(unsigned)uarg); break;
   case OP_TUPLE            : uarg = RT_READ_ARG(); putf("push tuple(pop %I16u)",uarg); break;
   case OP_LIST             : uarg = RT_READ_ARG(); putf("push list(pop %I16u)",uarg); break;
   case OP_DICT             : uarg = RT_READ_ARG(); putf("push dict(pop %u)",(unsigned)(uarg*2)); break;
   case OP_JUMP             : arg = RT_READ_SARG(); putf("jmp %.4x",(unsigned)((unsigned)((code-DeeCode_CODE(self))-3)+(arg))); break;
   case OP_JUMP_IF_TT       : arg = RT_READ_SARG(); putf("if peek 1: jmp %.4x",(unsigned)((unsigned)((code-DeeCode_CODE(self))-3)+(arg))); break;
   case OP_JUMP_IF_FF       : arg = RT_READ_SARG(); putf("if !peek 1: jmp %.4x",(unsigned)((unsigned)((code-DeeCode_CODE(self))-3)+(arg))); break;
   case OP_JUMP_IF_TT_POP   : arg = RT_READ_SARG(); putf("if pop 1: jmp %.4x",(unsigned)((unsigned)((code-DeeCode_CODE(self))-3)+(arg))); break;
   case OP_JUMP_IF_FF_POP   : arg = RT_READ_SARG(); putf("if !pop 1: jmp %.4x",(unsigned)((unsigned)((code-DeeCode_CODE(self))-3)+(arg))); break;
   case OP_ATTR_GET_C       : uarg = RT_READ_ARG(); putf("push @op __getattr__ pop 1 %r",DeeTuple_GET(((DeeCodeObject *)self)->co_consts,uarg)); break;
   case OP_ATTR_DEL_C       : uarg = RT_READ_ARG(); putf("@op __delattr__ pop 1 %r",DeeTuple_GET(((DeeCodeObject *)self)->co_consts,uarg)); break;
   case OP_ATTR_SET_C       : uarg = RT_READ_ARG(); putf("push @op __setattr__ pop 2 %r",DeeTuple_GET(((DeeCodeObject *)self)->co_consts,uarg)); break;
   case OP_SEQ_ITER_WALK    : arg = RT_READ_SARG(); putf("if @op __iternext__: push ret_op; else: jump %.4x",(unsigned)((unsigned)((code-DeeCode_CODE(self))-3)+(arg))); break;
   case OP_ATTR_HAS_C       : uarg = RT_READ_ARG(); putf("push @op __hasattr__ pop 1 %r",DeeTuple_GET(((DeeCodeObject *)self)->co_consts,uarg)); break;
   case OP_STACKALIGN       : uarg = RT_READ_ARG(); putf("stack align %I16u",uarg); break;
   case OP_LROT_N           : uarg = RT_READ_ARG(); putf("rot << %u",(unsigned)(uarg+1)); break;
   case OP_RROT_N           : uarg = RT_READ_ARG(); putf("rot >> %u",(unsigned)(uarg+1)); break;
   case OP_FOREIGN_FUNCTION : uarg = RT_READ_ARG(); putf("push foreign_function(pop %u)",(unsigned)(uarg+1)); break;
   case OP_FUNCTION         : uarg = RT_READ_ARG(); putf("push function(%I16u)(pop 2)",uarg); break;
   case OP_TRY_BEGIN: {
    uarg = RT_READ_ARG();
    switch (((DeeCodeObject *)self)->co_exceptv[uarg].e_kind) {
     case DeeExceptionHandleKind_ALL:       put("try @begin(*): "); break;
     case DeeExceptionHandleKind_ALL_VAR:
      putf("try @begin(*): @last_exception store @local %s@%u; ",
           _DeeCode_LocalName(self,((DeeCodeObject *)self)->co_exceptv[uarg].e_store),
           ((DeeCodeObject *)self)->co_exceptv[uarg].e_store); break;
     case DeeExceptionHandleKind_TYPED:     putf("try @begin(%k): ",((DeeCodeObject *)self)->co_exceptv[uarg].e_type); break;
     case DeeExceptionHandleKind_TYPED_VAR:
      putf("try @begin(%k): @last_exception store @local %s@%u; ",
           ((DeeCodeObject *)self)->co_exceptv[uarg].e_type,
           _DeeCode_LocalName(self,((DeeCodeObject *)self)->co_exceptv[uarg].e_store),
           ((DeeCodeObject *)self)->co_exceptv[uarg].e_store); break;
     case DeeExceptionHandleKind_FINALLY:   put("try @begin_finally: "); break;
     default: DEE_BUILTIN_UNREACHABLE();
    }
    putf("align %Iu; jmp %.4Ix",
         ((DeeCodeObject *)self)->co_exceptv[uarg].e_stack,
         ((DeeCodeObject *)self)->co_exceptv[uarg].e_handler);
   } break;
   case OP_CALL_MEMBER_C    : putf("push @op __call__ pop 2: %r",DeeTuple_GET(((DeeCodeObject *)self)->co_consts,RT_READ_ARG())); break;
   case OP_UNPACK_TUPLE     : putf("push %u @tuple.unpack pop 1",(unsigned)RT_READ_ARG()); break;
   case OP_UNPACK_LIST      : putf("push %u @list.unpack pop 1",(unsigned)RT_READ_ARG()); break;
   case OP_UNPACK_SEQUENCE  : putf("push %u @sequence.unpack pop 1",(unsigned)RT_READ_ARG()); break;
   case OP_JUMP_IF_CASE_COMPARE: arg = RT_READ_SARG(); putf("if pop(0) == second: jmp %.4x",(unsigned)((unsigned)((code-DeeCode_CODE(self))-3)+(arg))); break;
   case OP_JUMP_IF_RCASE_COMPARE: arg = RT_READ_SARG(); putf("if pop(1) <= third && pop(0) >= third: jmp %.4x",(unsigned)((unsigned)((code-DeeCode_CODE(self))-3)+(arg))); break;
   case OP_CLASS_SET_SLOT   : uarg = RT_READ_ARG(); putf("class second @set_slot %u pop 1",(unsigned)uarg); break;
   case OP_CLASS_ADD_METHOD : uarg = RT_READ_ARG(); putf("class second @add_method %r pop 1",DeeTuple_GET(((DeeCodeObject *)self)->co_consts,uarg)); break;
   case OP_CLASS_ADD_GETSET : uarg = RT_READ_ARG(); putf("class fourth @add_getset %r pop 3",DeeTuple_GET(((DeeCodeObject *)self)->co_consts,uarg)); break;
   case OP_CLASS_ADD_CLASS_METHOD: uarg = RT_READ_ARG(); putf("class second @add_class_method %r pop 1",DeeTuple_GET(((DeeCodeObject *)self)->co_consts,uarg)); break;
   case OP_CLASS_ADD_CLASS_GETSET: uarg = RT_READ_ARG(); putf("class fourth @add_class_getset %r pop 3",DeeTuple_GET(((DeeCodeObject *)self)->co_consts,uarg)); break;
   case OP_SET              : putf("push set(pop %u)",(unsigned)RT_READ_ARG()); break;
   case OP_PREFIX           : uarg = RT_READ_ARG(); putf("@prefix %.4x",(unsigned)uarg); break;
   case OP_EXTENDED:
    uarg = RT_READ_ARG();
    switch (uarg) {
     case OPEXT_TRY_INPLACE_ADD: put("try{push @op __iadd__ second,pop 1} or {push @op __add__ pop 2; dup}"); break;
     case OPEXT_TRY_INPLACE_SUB: put("try{push @op __isub__ second,pop 1} or {push @op __sub__ pop 2; dup}"); break;
     case OPEXT_TRY_INPLACE_MUL: put("try{push @op __imul__ second,pop 1} or {push @op __mul__ pop 2; dup}"); break;
     case OPEXT_TRY_INPLACE_DIV: put("try{push @op __idiv__ second,pop 1} or {push @op __div__ pop 2; dup}"); break;
     case OPEXT_TRY_INPLACE_MOD: put("try{push @op __imod__ second,pop 1} or {push @op __mod__ pop 2; dup}"); break;
     case OPEXT_TRY_INPLACE_SHL: put("try{push @op __ishl__ second,pop 1} or {push @op __shl__ pop 2; dup}"); break;
     case OPEXT_TRY_INPLACE_SHR: put("try{push @op __ishr__ second,pop 1} or {push @op __shr__ pop 2; dup}"); break;
     case OPEXT_TRY_INPLACE_AND: put("try{push @op __iand__ second,pop 1} or {push @op __and__ pop 2; dup}"); break;
     case OPEXT_TRY_INPLACE_OR:  put("try{push @op __ior__ second,pop 1} or {push @op __or__ pop 2; dup}"); break;
     case OPEXT_TRY_INPLACE_XOR: put("try{push @op __ixor__ second,pop 1} or {push @op __xor__ pop 2; dup}"); break;
     case OPEXT_TRY_INPLACE_POW: put("try{push @op __ipow__ second,pop 1} or {push @op __pow__ pop 2; dup}"); break;
     case OPEXT_ALLOCA      : put("push @op alloca pop 1"); break;
     case OPEXT_BREAKPOINT  : put("breakpoint"); break;
     case OPEXT_DOC         : put("push @op __doc__ pop 1"); break;
     case OPEXT_DOC_ATTR    : put("push @op __doc__ __getattr__ pop 2"); break;
     case OPEXT_EMPTY_TUPLE : put("push @tuple empty"); break;
     case OPEXT_EMPTY_LIST  : put("push @list empty"); break;
     case OPEXT_EMPTY_DICT  : put("push @dict empty"); break;
     case OPEXT_TRY_INPLACE_INC: put("try{push @op __inc__ top; rot 2} or {push @op __iadd__ top,int 1; rot 2} or {push @op __add__ pop 1,int 1; dup}"); break;
     case OPEXT_TRY_INPLACE_DEC: put("try{push @op __dec__ top; rot 2} or {push @op __isub__ top,int 1; rot 2} or {push @op __sub__ pop 1,int 1; dup}"); break;
     case OPEXT_TRY_INPLACE_INC_POST: put("try{push @op __incpost__ top; rot 2} or {push copy top; rot 2; @op __iadd__ pop 1,int 1; rot 2} or {push copy top; rot 2; push @op __add__ pop 1,int 1; rot 2}"); break;
     case OPEXT_TRY_INPLACE_DEC_POST: put("try{push @op __decpost__ top; rot 2} or {push copy top; rot 2; @op __isub__ pop 1,int 1; rot 2} or {push copy top; rot 2; push @op __sub__ pop 1,int 1; rot 2}"); break;
     case OPEXT_CLASS_NEW   : put("push @class.new pop 2"); break;
     case OPEXT_CLASS_NEW_UUID: put("push @class.uuid_new pop 3"); break;
     case OPEXT_EMPTY_SET   : put("push @set empty"); break;
     case OPEXT_SUPER_AT    : put("push @super pop 2"); break;
     case OPEXT_EMPTY_CELL  : put("push @cell empty"); break;
     case OPEXT_BOUND_RESULT: put("push is_bound @result"); break;
     case OPEXT_DEL_RESULT  : put("del @result"); break;
     case OPEXT_SUPEROF     : put("push @op (pop 1).super"); break;
     case OPEXT_CLASSOF     : put("push @op (pop 1).class"); break;
     case OPEXT_SEQ_ITERALL : put("seq.iterall pop 1"); break;
     case OPEXT_SEQ_ANY     : put("push __any__ pop 1"); break;
     case OPEXT_SEQ_ALL     : put("push __all__ pop 1"); break;
     case OPEXT_SEQ_SUM     : put("push __sum__ pop 1"); break;
     case OPEXT_SEQ_ITER_NEXT: put("push @op __iternext__ pop 1"); break;
     case OPEXT_TRY_CMP_EQ  : put("try{push @op __eq__ pop 2} or {push false}"); break;
     case OPEXT_IO_WRITEP   : put("push @op __writep__ pop 3"); break;
     case OPEXT_IO_WRITENP  : put("push @op __writenp__ pop 2"); break;
     case OPEXT_IO_READP    : put("push @op __readp__ pop 3"); break;
     case OPEXT_IO_READNP   : put("push @op __readnp__ pop 2"); break;
     case OPEXT_IO_SEEK     : put("@op __seek__ pop 2"); break;
     case OPEXT_IO_SEEK_USED: put("push @op __seek__ pop 2"); break;
     case OPEXT_IO_TRUNC    : put("@op __trunc__ pop 1"); break;
     case OPEXT_IO_FLUSH    : put("@op __flush__ pop 1"); break;
     case OPEXT_IO_CLOSE    : put("@op __close__ pop 1"); break;
     case OPEXT_SEQ_MIN     : put("push __min__ pop 1"); break;
     case OPEXT_SEQ_MAX     : put("push __max__ pop 1"); break;
     default                : putf("extended %.4x",(unsigned)uarg); break;
    }
    break;
   default:
    if ((code[-1] & OPFLAG_ARG) != 0) {
     uarg = code[-1];
     putf("UNKNOWN (%#.2x: %#.4x)",(unsigned)((Dee_uint8_t)uarg),(unsigned)RT_READ_ARG());
    } else {
     putf("UNKNOWN (%#.2x)",(unsigned)(code[-1]));
    }
    break;
  }
  if (DeeStringWriter_WriteChar(&writer,'\n') == -1) goto err;
 }
#undef putf
#undef put
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_CODE_ASMREPR_INL */
