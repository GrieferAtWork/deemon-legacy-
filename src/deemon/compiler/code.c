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
#ifndef GUARD_DEEMON_CODE_C
#define GUARD_DEEMON_CODE_C  1
#define DEE_LIMITED_API      1
#define DEE_OUTDATED_OPCODES 101

// /include/*
#include <deemon/__conf.inl>
#include <deemon/bool.h>
#include <deemon/compiler/code.h>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/integer.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/object.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_visit.h>
#include <deemon/runtime/builtins.h>
#include <deemon/runtime/execute.h>
#include <deemon/type.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/compiler/__opcode.inl>
#include <deemon/marshal_data.h>
#include DEE_INCLUDE_MEMORY_API()

// */ (nano...)

DEE_DECL_BEGIN

DEE_A_RET_WUNUSED int _DeeCodeLnOffList_CompareLo(
 DEE_A_IN_OPT struct _DeeCodeLnOffList const *lhs,
 DEE_A_IN_OPT struct _DeeCodeLnOffList const *rhs) {
 if (!lhs) return rhs ? 1 : 0;
 if (!rhs) return 0;
 while (1) {
  if (_DeeCodeLnOffList_EMPTY(lhs)) return !_DeeCodeLnOffList_EMPTY(rhs);
  if (_DeeCodeLnOffList_EMPTY(rhs)) return 0;
  if (*(Dee_uint16_t *)lhs >= *(Dee_uint16_t *)rhs) return 0;
  ++lhs,++rhs;
 }
}
DEE_A_RET_WUNUSED int _DeeCodeLnOffList_CompareLe(
 DEE_A_IN_OPT struct _DeeCodeLnOffList const *lhs,
 DEE_A_IN_OPT struct _DeeCodeLnOffList const *rhs) {
 if (!lhs) return 1;
 if (!rhs) return 0;
 while (1) {
  if (_DeeCodeLnOffList_EMPTY(lhs)) return 1;
  if (_DeeCodeLnOffList_EMPTY(rhs)) return 0;
  if (*(Dee_uint16_t *)lhs > *(Dee_uint16_t *)rhs) return 0;
  ++lhs,++rhs;
 }
}
DEE_A_RET_WUNUSED int _DeeCodeLnOffList_CompareEq(
 DEE_A_IN_OPT struct _DeeCodeLnOffList const *lhs,
 DEE_A_IN_OPT struct _DeeCodeLnOffList const *rhs) {
 if (!lhs) return rhs ? 0 : 1;
 if (!rhs) return 0;
 while (1) {
  if (_DeeCodeLnOffList_EMPTY(lhs)) return _DeeCodeLnOffList_EMPTY(rhs);
  if (_DeeCodeLnOffList_EMPTY(rhs)) return 0;
  if (*(Dee_uint16_t *)lhs != *(Dee_uint16_t *)rhs) return 0;
  ++lhs,++rhs;
 }
}
DEE_A_RET_WUNUSED Dee_size_t _DeeCodeLnOffList_Size(
 DEE_A_IN struct _DeeCodeLnOffList const *self) {
 struct _DeeCodeLnOffList const *iter;
 DEE_ASSERT(self),iter = self;
 while (!_DeeCodeLnOffList_EMPTY(iter)) ++iter;
 DEE_ASSERT(iter != self && "Empty list should be NULL");
 return (Dee_size_t)(iter-self);
}
DEE_A_RET_EXCEPT(NULL) struct _DeeCodeLnOffList *
_DeeCodeLnOffList_Copy(DEE_A_IN struct _DeeCodeLnOffList const *self) {
 Dee_size_t size; struct _DeeCodeLnOffList *result;
 size = (_DeeCodeLnOffList_Size(self)+1)*sizeof(struct _DeeCodeLnOffList);
 while DEE_UNLIKELY((result = (struct _DeeCodeLnOffList *)malloc_nz(size)) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 memcpy(result,self,size);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeCodeDebugInfoObject) *
_DeeCodeDebugInfo_New(void) {
 DeeCodeDebugInfoObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOC(DeeCodeDebugInfoObject)) != NULL) {
  DeeObject_INIT(result,&DeeCodeDebugInfo_Type);
 }
 return (DeeObject *)result;
}



DEE_A_RET_OBJECT_NOEXCEPT(DeeStringObject) *DeeCode_Addr2File(
 DEE_A_IN_OBJECT(DeeCodeObject) const *self, DEE_A_IN Dee_size_t addr) {
 DEE_ASSERT(DeeObject_Check(self) && DeeCode_Check(self));
 return DeeCode_ADDR2FILE(self,addr);
}
DEE_A_RET_WUNUSED Dee_int64_t DeeCode_Addr2Line(
 DEE_A_IN_OBJECT(DeeCodeObject) const *self, DEE_A_IN Dee_size_t addr) {
 DEE_ASSERT(DeeObject_Check(self) && DeeCode_Check(self));
 return DeeCode_ADDR2LINE(self,addr);
}
DEE_A_RET_WUNUSED Dee_size_t DeeCode_Ptr2Addr(
 DEE_A_IN_OBJECT(DeeCodeObject) const *self, DEE_A_IN void const *p) {
 Dee_size_t result;
 if DEE_UNLIKELY(p < (void const *)DeeCode_CODE(self)) return 0;
 result = DeeCode_PTR2ADDR(self,p);
 if (result > DeeCode_SIZE(self)) result = DeeCode_SIZE(self);
 return result;
}

DEE_A_RET_WUNUSED Dee_size_t DeeCode_Size(
 DEE_A_IN_OBJECT(DeeCodeObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeCode_Check(self));
 return DeeCode_SIZE(self);
}
DEE_A_RET_WUNUSED Dee_size_t DeeCode_InstructionCount(
 DEE_A_IN_OBJECT(DeeCodeObject) const *self) {
 Dee_uint8_t *code,*end; Dee_size_t result = 0;
 DEE_ASSERT(DeeObject_Check(self) && DeeCode_Check(self));
 end = (code = DeeCode_CODE(self))+DeeCode_SIZE(self);
 while (code != end) {
  DEE_ASSERT(code < end);
  if ((*code++&OPFLAG_ARG)!=0) code += 2;
  ++result;
 }
 return result;
}
DEE_A_RET_OBJECT_MAYBE(DeeCodeDebugInfoObject) *DeeCode_Debug(
 DEE_A_IN_OBJECT(DeeCodeObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeCode_Check(self));
 return DeeCode_DEBUG(self);
}
DEE_A_RET_OBJECT_MAYBE(DeeStringObject) *DeeCode_Name(
 DEE_A_IN_OBJECT(DeeCodeObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeCode_Check(self));
 return DeeCode_NAME(self);
}

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
 char const *prefix;
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
#ifdef OP_RETNONE
   case OP_RETNONE          : put("ret none"); break;
#endif
#ifdef OP_STOP
   case OP_STOP             : put("stop"); break;
#endif
#ifdef OP_NOOP
   case OP_NOOP             : put("noop"); break;
#endif
#ifdef OP_POP
   case OP_POP              : put("pop 1"); break;
#endif
#ifdef OP_YIELD
   case OP_YIELD            : put("yield pop 1"); break;
#endif
#ifdef OP_YIELDSTOP
   case OP_YIELDSTOP        : put("yield_stop"); break;
#endif
#ifdef OP_RET
   case OP_RET              : put("ret pop 1"); break;
#endif
#ifdef OP_LOAD_NONE
   case OP_LOAD_NONE        : put("push none"); break;
#endif
#ifdef OP_MOVE
   case OP_MOVE             : put("push @op __move__ pop 1"); break;
#endif
#ifdef OP_COPY
   case OP_COPY             : put("push @op __copy__ pop 1"); break;
#endif
#ifdef OP_WEAK
   case OP_WEAK             : put("push @op __weak__ pop 1"); break;
#endif
#ifdef OP_TYPE
   case OP_TYPE             : put("push @op __type__ pop 1"); break;
#endif
#ifdef OP_BOOL
   case OP_BOOL             : put("push @op __bool__ pop 1"); break;
#endif
#ifdef OP_IS
   case OP_IS               : put("push @op __is__ pop 2"); break;
#endif
#ifdef OP_IN
   case OP_IN               : put("push @op __in__ pop 2"); break;
#endif
#ifdef OP_CALL
   case OP_CALL             : put("push @op __call__ pop 2"); break;
#endif
#ifdef OP_NEW_INSTANCE
   case OP_NEW_INSTANCE     : put("push @op __new_instance__ pop 2"); break;
#endif
#ifdef OP_COPY_ASSIGN
   case OP_COPY_ASSIGN      : put("push @op __assign__ pop 2"); break;
#endif
#ifdef OP_STR
   case OP_STR              : put("push @op __str__ pop 1"); break;
#endif
#ifdef OP_REPR
   case OP_REPR             : put("push @op __repr__ pop 1"); break;
#endif
#ifdef OP_NOT
   case OP_NOT              : put("push @op __not__ pop 1"); break;
#endif
#ifdef OP_POS
   case OP_POS              : put("push @op __pos__ pop 1"); break;
#endif
#ifdef OP_NEG
   case OP_NEG              : put("push @op __neg__ pop 1"); break;
#endif
#ifdef OP_REF
   case OP_REF              : put("push @op __ref__ pop 1"); break;
#endif
#ifdef OP_DEREF
   case OP_DEREF            : put("push @op __deref__ pop 1"); break;
#endif
#ifdef OP_INV
   case OP_INV              : put("push @op __inv__ pop 1"); break;
#endif
#ifdef OP_INC
   case OP_INC              : put("push @op __inc__ pop 1"); break;
#endif
#ifdef OP_DEC
   case OP_DEC              : put("push @op __dec__ pop 1"); break;
#endif
#ifdef OP_INC_POST
   case OP_INC_POST         : put("push @op __incpost__ pop 1"); break;
#endif
#ifdef OP_DEC_POST
   case OP_DEC_POST         : put("push @op __decpost__ pop 1"); break;
#endif
#ifdef OP_ADD
   case OP_ADD              : put("push @op __add__ pop 2"); break;
#endif
#ifdef OP_INPLACE_ADD
   case OP_INPLACE_ADD      : put("push @op __iadd__ pop 2"); break;
#endif
#ifdef OP_SUB
   case OP_SUB              : put("push @op __sub__ pop 2"); break;
#endif
#ifdef OP_INPLACE_SUB
   case OP_INPLACE_SUB      : put("push @op __isub__ pop 2"); break;
#endif
#ifdef OP_MUL
   case OP_MUL              : put("push @op __mul__ pop 2"); break;
#endif
#ifdef OP_INPLACE_MUL
   case OP_INPLACE_MUL      : put("push @op __imul__ pop 2"); break;
#endif
#ifdef OP_DIV
   case OP_DIV              : put("push @op __div__ pop 2"); break;
#endif
#ifdef OP_INPLACE_DIV
   case OP_INPLACE_DIV      : put("push @op __idiv__ pop 2"); break;
#endif
#ifdef OP_MOD
   case OP_MOD              : put("push @op __mod__ pop 2"); break;
#endif
#ifdef OP_INPLACE_MOD
   case OP_INPLACE_MOD      : put("push @op __imod__ pop 2"); break;
#endif
#ifdef OP_SHL
   case OP_SHL              : put("push @op __shl__ pop 2"); break;
#endif
#ifdef OP_INPLACE_SHL
   case OP_INPLACE_SHL      : put("push @op __ishl__ pop 2"); break;
#endif
#ifdef OP_SHR
   case OP_SHR              : put("push @op __shr__ pop 2"); break;
#endif
#ifdef OP_INPLACE_SHR
   case OP_INPLACE_SHR      : put("push @op __ishr__ pop 2"); break;
#endif
#ifdef OP_AND
   case OP_AND              : put("push @op __and__ pop 2"); break;
#endif
#ifdef OP_INPLACE_AND
   case OP_INPLACE_AND      : put("push @op __iand__ pop 2"); break;
#endif
#ifdef OP_OR
   case OP_OR               : put("push @op __or__ pop 2"); break;
#endif
#ifdef OP_INPLACE_OR
   case OP_INPLACE_OR       : put("push @op __ior__ pop 2"); break;
#endif
#ifdef OP_XOR
   case OP_XOR              : put("push @op __xor__ pop 2"); break;
#endif
#ifdef OP_INPLACE_XOR
   case OP_INPLACE_XOR      : put("push @op __ixor__ pop 2"); break;
#endif
#ifdef OP_POW
   case OP_POW              : put("push @op __pow__ pop 2"); break;
#endif
#ifdef OP_INPLACE_POW
   case OP_INPLACE_POW      : put("push @op __ipow__ pop 2"); break;
#endif
#ifdef OP_CMP_LO
   case OP_CMP_LO           : put("push @op __lo__ pop 2"); break;
#endif
#ifdef OP_CMP_LE
   case OP_CMP_LE           : put("push @op __le__ pop 2"); break;
#endif
#ifdef OP_CMP_EQ
   case OP_CMP_EQ           : put("push @op __eq__ pop 2"); break;
#endif
#ifdef OP_CMP_NE
   case OP_CMP_NE           : put("push @op __ne__ pop 2"); break;
#endif
#ifdef OP_CMP_GR
   case OP_CMP_GR           : put("push @op __gr__ pop 2"); break;
#endif
#ifdef OP_CMP_GE
   case OP_CMP_GE           : put("push @op __ge__ pop 2"); break;
#endif
#ifdef OP_SEQ_GET
   case OP_SEQ_GET          : put("push @op __getitem__ pop 2"); break;
#endif
#ifdef OP_SEQ_DEL
   case OP_SEQ_DEL          : put("@op __delitem__ pop 2"); break;
#endif
#ifdef OP_SEQ_SET
   case OP_SEQ_SET          : put("push @op __setitem__ pop 3"); break;
#endif
#ifdef OP_SEQ_SIZE
   case OP_SEQ_SIZE         : put("push @op __size__ pop 1"); break;
#endif
#ifdef OP_SEQ_RANGE_GET
   case OP_SEQ_RANGE_GET    : put("push @op __getrange__ pop 3"); break;
#endif
#ifdef OP_SEQ_RANGE_DEL
   case OP_SEQ_RANGE_DEL    : put("@op __delrange__ pop 3"); break;
#endif
#ifdef OP_SEQ_RANGE_SET
   case OP_SEQ_RANGE_SET    : put("push @op __setrange__ pop 4"); break;
#endif
#ifdef OP_SEQ_ITER_SELF
   case OP_SEQ_ITER_SELF    : put("push @op __iterself__ pop 1"); break;
#endif
#ifdef OP_LOAD_EXTERN
   case OP_LOAD_EXTERN      : put("push @op __builtin_extern pop 3"); break;
#endif
#ifdef OP_LOAD_AUTOEXTERN
   case OP_LOAD_AUTOEXTERN  : put("push @op __builtin_extern pop 2"); break;
#endif
#ifdef OP_FF_CLOSURE
   case OP_FF_CLOSURE       : put("push @op __builtin_ff_closure pop 2"); break;
#endif
#ifdef OP_ATTR_GET
   case OP_ATTR_GET         : put("push @op __getattr__ pop 2"); break;
#endif
#ifdef OP_ATTR_DEL
   case OP_ATTR_DEL         : put("@op __delattr__ pop 2"); break;
#endif
#ifdef OP_ATTR_SET
   case OP_ATTR_SET         : put("push @op __setattr__ pop 3"); break;
#endif
#ifdef OP_LOAD_LOC
   case OP_LOAD_LOC         : uarg = RT_READ_ARG(); putf("push @local %s@%u",_DeeCode_LocalName(self,uarg),(unsigned)uarg); break;
#endif
#ifdef OP_LOAD_ARG
   case OP_LOAD_ARG         : uarg = RT_READ_ARG(); putf("push @arg %s@%u",_DeeCode_ArgName(self,uarg),(unsigned)uarg); break;
#endif
#ifdef OP_CONCAT_TUPLE
   case OP_CONCAT_TUPLE     : put("push @tuple.concat pop 2"); break;
#endif
#ifdef OP_CONCAT_LIST
   case OP_CONCAT_LIST      : put("push @list.concat pop 2"); break;
#endif
#ifdef OP_CAST_LIST
   case OP_CAST_LIST        : put("push @list pop 1"); break;
#endif
#ifdef OP_CAST_TUPLE
   case OP_CAST_TUPLE       : put("push @tuple pop 1"); break;
#endif
#ifdef OP_CAST_SET
   case OP_CAST_SET         : put("push @set pop 1"); break;
#endif
#ifdef OP_UNREACHABLE
   case OP_UNREACHABLE      : put("__builtin_unreachable"); break;
#endif
#ifdef OP_DUP
   case OP_DUP              : put("dup top"); break;
#endif
#ifdef OP_LOAD_CST
   case OP_LOAD_CST:        prefix = ""; goto push_c;
#endif
#ifdef OP_LOAD_CST_LOCKED
   case OP_LOAD_CST_LOCKED: prefix = "locked "; goto push_c;
#endif
#ifdef OP_LOAD_CST_COPY
   case OP_LOAD_CST_COPY:   prefix = "copy "; goto push_c;
#endif
#ifdef OP_LOAD_CST_DCOPY
   case OP_LOAD_CST_DCOPY:  prefix = "deep_copy ";
#endif
#if defined(OP_LOAD_CST) || defined(OP_LOAD_CST_LOCKED) || \
    defined(OP_LOAD_CST_COPY) || defined(OP_LOAD_CST_DCOPY)
   push_c:
    temp = DeeTuple_GET(((DeeCodeObject *)self)->co_consts,RT_READ_ARG());
wrt_tmp: putf("push %s{%r}",prefix,temp);
    break;
#endif
#ifdef OP_LOAD_BLTIN
   case OP_LOAD_BLTIN         : temp = DeeBuiltin_GET(RT_READ_ARG()); prefix = ""; goto wrt_tmp;
#endif
#ifdef OP_LOAD_RET
   case OP_LOAD_RET           : put("push @return"); break;
#endif
#ifdef OP_LOAD_THIS
   case OP_LOAD_THIS          : put("push @this"); break;
#endif
#ifdef OP_STORE_LOC
   case OP_STORE_LOC          : uarg = RT_READ_ARG(); putf("store @local %s@%u",_DeeCode_LocalName(self,uarg),(unsigned)uarg); break;
#endif
#ifdef OP_STORE_LOC_POP
   case OP_STORE_LOC_POP      : uarg = RT_READ_ARG(); putf("pop store @local %s@%u",_DeeCode_LocalName(self,uarg),(unsigned)uarg); break;
#endif
#ifdef OP_STORE_ARG
   case OP_STORE_ARG          : uarg = RT_READ_ARG(); putf("store @arg %s@%u",_DeeCode_ArgName(self,uarg),(unsigned)uarg); break;
#endif
#ifdef OP_STORE_ARG_POP
   case OP_STORE_ARG_POP      : uarg = RT_READ_ARG(); putf("pop locked store @arg %s@%u",_DeeCode_ArgName(self,uarg),(unsigned)uarg); break;
#endif
#ifdef OP_STORE_CST
   case OP_STORE_CST          : uarg = RT_READ_ARG(); putf("locked store @const %s@%u",_DeeCode_ConstName(self,uarg),(unsigned)uarg); break;
#endif
#ifdef OP_STORE_CST_POP
   case OP_STORE_CST_POP      : uarg = RT_READ_ARG(); putf("pop store @const %s@%u",_DeeCode_ConstName(self,uarg),(unsigned)uarg); break;
#endif
#ifdef OP_STORE_RET
   case OP_STORE_RET          : put("store @return"); break;
#endif
#ifdef OP_STORE_RET_POP
   case OP_STORE_RET_POP      : put("pop store @return"); break;
#endif
#ifdef OP_TUPLE
   case OP_TUPLE              : putf("push tuple(pop %u)",(unsigned)RT_READ_ARG()); break;
#endif
#ifdef OP_LIST
   case OP_LIST               : putf("push list(pop %u)",(unsigned)RT_READ_ARG()); break;
#endif
#ifdef OP_JUMP
   case OP_JUMP               : arg = RT_READ_SARG(); putf("jmp %.4x",(unsigned)((unsigned)((code-DeeCode_CODE(self))-3)+(arg))); break;
#endif
#ifdef OP_JUMP_IF_TT
   case OP_JUMP_IF_TT         : arg = RT_READ_SARG(); putf("if peek 1: jmp %.4x",(unsigned)((unsigned)((code-DeeCode_CODE(self))-3)+(arg))); break;
#endif
#ifdef OP_JUMP_IF_FF
   case OP_JUMP_IF_FF         : arg = RT_READ_SARG(); putf("if !peek 1: jmp %.4x",(unsigned)((unsigned)((code-DeeCode_CODE(self))-3)+(arg))); break;
#endif
#ifdef OP_JUMP_IF_TT_POP
   case OP_JUMP_IF_TT_POP     : arg = RT_READ_SARG(); putf("if pop 1: jmp %.4x",(unsigned)((unsigned)((code-DeeCode_CODE(self))-3)+(arg))); break;
#endif
#ifdef OP_JUMP_IF_FF_POP
   case OP_JUMP_IF_FF_POP     : arg = RT_READ_SARG(); putf("if !pop 1: jmp %.4x",(unsigned)((unsigned)((code-DeeCode_CODE(self))-3)+(arg))); break;
#endif
#ifdef OP_ATTR_GET_C
   case OP_ATTR_GET_C         : putf("push @op __getattr__ pop 1 %r",DeeTuple_GET(((DeeCodeObject *)self)->co_consts,RT_READ_ARG())); break;
#endif
#ifdef OP_ATTR_DEL_C
   case OP_ATTR_DEL_C         : putf("@op __delattr__ pop 1 %r",DeeTuple_GET(((DeeCodeObject *)self)->co_consts,RT_READ_ARG())); break;
#endif
#ifdef OP_ATTR_SET_C
   case OP_ATTR_SET_C         : putf("push @op __setattr__ pop 2 %r",DeeTuple_GET(((DeeCodeObject *)self)->co_consts,RT_READ_ARG())); break;
#endif
#ifdef OP_DEL_LOCAL
   case OP_DEL_LOCAL          : uarg = RT_READ_ARG(); putf("del @local %s@%u",_DeeCode_LocalName(self,uarg),(unsigned)uarg); break;
#endif
#ifdef OP_SEQ_ITER_WALK
   case OP_SEQ_ITER_WALK      : arg = RT_READ_SARG(); putf("if @op __iternext__: push ret_op; else: jump %.4x",(unsigned)((unsigned)((code-DeeCode_CODE(self))-3)+(arg))); break;
#endif
#ifdef OP_CALL_MEMBER_C
   case OP_CALL_MEMBER_C        : putf("push @op __call__ pop 2: %r",DeeTuple_GET(((DeeCodeObject *)self)->co_consts,RT_READ_ARG())); break;
#endif
#ifdef OP_UNPACK_TUPLE
   case OP_UNPACK_TUPLE       : putf("push %u @tuple.unpack pop 1",(unsigned)RT_READ_ARG()); break;
#endif
#ifdef OP_UNPACK_LIST
   case OP_UNPACK_LIST        : putf("push %u @list.unpack pop 1",(unsigned)RT_READ_ARG()); break;
#endif
#ifdef OP_UNPACK_SEQUENCE
   case OP_UNPACK_SEQUENCE    : putf("push %u @sequence.unpack pop 1",(unsigned)RT_READ_ARG()); break;
#endif
#ifdef OP_JUMP_IF_CASE_COMPARE
   case OP_JUMP_IF_CASE_COMPARE: arg = RT_READ_SARG(); putf("if pop(0) == second: jmp %.4x",(unsigned)((unsigned)((code-DeeCode_CODE(self))-3)+(arg))); break;
#endif
#ifdef OP_JUMP_IF_RCASE_COMPARE
   case OP_JUMP_IF_RCASE_COMPARE: arg = RT_READ_SARG(); putf("if pop(1) <= third && pop(0) >= third: jmp %.4x",(unsigned)((unsigned)((code-DeeCode_CODE(self))-3)+(arg))); break;
#endif
#ifdef OP_PRINT_ONE
   case OP_PRINT_ONE          : put("print stdout: pop(0)"); break;
#endif
#ifdef OP_PRINT_ONE_SEP
   case OP_PRINT_ONE_SEP      : put("print stdout: pop(0),\" \""); break;
#endif
#ifdef OP_PRINT_ONE_END
   case OP_PRINT_ONE_END      : put("print stdout: pop(0),\"\\n\""); break;
#endif
#ifdef OP_PRINT_ONE_F
   case OP_PRINT_ONE_F        : put("print pop(1): pop(0)"); break;
#endif
#ifdef OP_PRINT_ONE_F_SEP
   case OP_PRINT_ONE_F_SEP    : put("print second: pop(0),\" \""); break;
#endif
#ifdef OP_PRINT_ONE_F_END
   case OP_PRINT_ONE_F_END    : put("print pop(1): pop(0),\"\\n\""); break;
#endif
#ifdef OP_PRINT_ALL
   case OP_PRINT_ALL          : put("print stdout: pop(0)..."); break;
#endif
#ifdef OP_PRINT_ALL_SEP
   case OP_PRINT_ALL_SEP      : put("print stdout: pop(0)...,\" \""); break;
#endif
#ifdef OP_PRINT_ALL_END
   case OP_PRINT_ALL_END      : put("print stdout: pop(0)...,\"\\n\""); break;
#endif
#ifdef OP_PRINT_ALL_F
   case OP_PRINT_ALL_F        : put("print pop(1): pop(0)..."); break;
#endif
#ifdef OP_PRINT_ALL_F_SEP
   case OP_PRINT_ALL_F_SEP    : put("print second: pop(0)...,\" \""); break;
#endif
#ifdef OP_PRINT_ALL_F_END
   case OP_PRINT_ALL_F_END    : put("print pop(1): pop(0)...,\"\\n\""); break;
#endif
#ifdef OP_PRINT_END
   case OP_PRINT_END          : put("print stdout: \"\\n\""); break;
#endif
#ifdef OP_PRINT_END_F
   case OP_PRINT_END_F        : put("print pop(0): \"\\n\""); break;
#endif
#ifdef OP_CELL
   case OP_CELL               : put("push @op __cell__ pop 1"); break;
#endif
#ifdef OP_DICT
   case OP_DICT               : putf("push dict(pop %u)",(unsigned)(RT_READ_ARG()*2)); break;
#endif
#ifdef OP_ROT_2
   case OP_ROT_2              : put("rot 2"); break;
#endif
#ifdef OP_LROT_3
   case OP_LROT_3             : put("rot << 3"); break;
#endif
#ifdef OP_LROT_4
   case OP_LROT_4             : put("rot << 4"); break;
#endif
#ifdef OP_RROT_3
   case OP_RROT_3             : put("rot >> 3"); break;
#endif
#ifdef OP_RROT_4
   case OP_RROT_4             : put("rot >> 4"); break;
#endif
#ifdef OP_LROT_N
   case OP_LROT_N             : putf("rot << %u",(unsigned)(RT_READ_ARG()+1)); break;
#endif
#ifdef OP_RROT_N
   case OP_RROT_N             : putf("rot >> %u",(unsigned)(RT_READ_ARG()+1)); break;
#endif
#ifdef OP_CLASS_SET_SLOT
   case OP_CLASS_SET_SLOT     : putf("class second @set_slot %u pop 1",(unsigned)RT_READ_ARG()); break;
#endif
#ifdef OP_CLASS_ADD_METHOD
   case OP_CLASS_ADD_METHOD   : putf("class second @add_method %u pop 1",(unsigned)RT_READ_ARG()); break;
#endif
#ifdef OP_CLASS_ADD_GETSET
   case OP_CLASS_ADD_GETSET   : putf("class fourth @add_getset %u pop 3",(unsigned)RT_READ_ARG()); break;
#endif
#ifdef OP_CLASS_ADD_CLASS_METHOD
   case OP_CLASS_ADD_CLASS_METHOD: putf("class second @add_class_method %u pop 1",(unsigned)RT_READ_ARG()); break;
#endif
#ifdef OP_CLASS_ADD_CLASS_GETSET
   case OP_CLASS_ADD_CLASS_GETSET: putf("class fourth @add_class_getset %u pop 3",(unsigned)RT_READ_ARG()); break;
#endif
#ifdef OP_SET
   case OP_SET                : putf("push set(pop %u)",(unsigned)RT_READ_ARG()); break;
#endif
#ifdef OP_EXTENDED
   case OP_EXTENDED:
    uarg = RT_READ_ARG();
    switch (uarg) {
#ifdef OPEXT_ALLOCA
     case OPEXT_ALLOCA: put("push @op alloca pop 1"); break;
#endif
#ifdef OPEXT_BREAKPOINT
     case OPEXT_BREAKPOINT:  put("breakpoint"); break;
#endif
#ifdef OPEXT_DOC
     case OPEXT_DOC:         put("push @op __doc__ pop 1"); break;
#endif
#ifdef OPEXT_DOC_ATTR
     case OPEXT_DOC_ATTR:    put("push @op __doc__ __getattr__ pop 2"); break;
#endif
#ifdef OPEXT_EMPTY_TUPLE
     case OPEXT_EMPTY_TUPLE: put("push @tuple empty"); break;
#endif
#ifdef OPEXT_EMPTY_LIST
     case OPEXT_EMPTY_LIST:  put("push @list empty"); break;
#endif
#ifdef OPEXT_EMPTY_DICT
     case OPEXT_EMPTY_DICT:  put("push @dict empty"); break;
#endif
#ifdef OPEXT_EMPTY_SET
     case OPEXT_EMPTY_SET:   put("push @set empty"); break;
#endif
#ifdef OPEXT_CLASS_NEW
     case OPEXT_CLASS_NEW:   put("push @class.new pop 2"); break;
#endif
#ifdef OPEXT_CLASS_NEW_UUID
     case OPEXT_CLASS_NEW_UUID: put("push @class.new pop 3"); break;
#endif
#ifdef OPEXT_SUPER_AT
     case OPEXT_SUPER_AT:    put("push @super pop 2"); break;
#endif
#ifdef OPEXT_EMPTY_CELL
     case OPEXT_EMPTY_CELL:  put("push @cell empty"); break;
#endif
#ifdef OPEXT_SUPEROF
     case OPEXT_SUPEROF:     put("push @op (pop 1).super"); break;
#endif
#ifdef OPEXT_CLASSOF
     case OPEXT_CLASSOF:     put("push @op (pop 1).class"); break;
#endif
#ifdef OPEXT_SEQ_ITERALL
     case OPEXT_SEQ_ITERALL: put("seq.iterall pop 1"); break;
#endif
#ifdef OPEXT_SEQ_ANY
     case OPEXT_SEQ_ANY:     put("push seq.any pop 1"); break;
#endif
#ifdef OPEXT_SEQ_ALL
     case OPEXT_SEQ_ALL:     put("push seq.all pop 1"); break;
#endif
     default: putf("ext %.4x",(unsigned)uarg); break;
    }
    break;
#endif
#ifdef OP_PREFIX
   case OP_PREFIX:
    uarg = RT_READ_ARG();
    putf("@prefix %.4x",(unsigned)uarg);
    break;
#endif
#ifdef OP_FUNCTION
   case OP_FUNCTION           : putf("push function(%u)(pop 2)",(unsigned)RT_READ_ARG()); break;
#endif
#ifdef OP_THROW
   case OP_THROW              : put("throw pop 1"); break;
#endif
#ifdef OP_RETHROW
   case OP_RETHROW            : put("throw @last_exception"); break;
#endif
#ifdef OP_TRY_BEGIN
   case OP_TRY_BEGIN:
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
    putf("align %u; jmp %.4x",
         (unsigned)((DeeCodeObject *)self)->co_exceptv[uarg].e_stack,
         (unsigned)((DeeCodeObject *)self)->co_exceptv[uarg].e_handler);
    break;
#endif
#ifdef OP_EXCEPT_END
   case OP_EXCEPT_END:  put("try except @end"); break;
#endif
#ifdef OP_FINALLY_END
   case OP_FINALLY_END: put("try finally @end"); break;
#endif
#ifdef OP_TRY_END
   case OP_TRY_END:     put("try @end"); break;
#endif
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
end:
 DeeStringWriter_Quit(&writer);
 return result;
err:
 result = NULL;
 goto end;
}



static int _deecodedebuginfo_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeCodeDebugInfoObject *self) {
 self->di_name = NULL;
 self->di_local_names = NULL;
 self->di_const_names = NULL;
 self->di_ref_names = NULL;
 self->di_arg_names = NULL;
 self->di_lno = NULL;
 self->di_fno = NULL;
 self->di_file_names = NULL;
 return 0;
}
static int _deecodedebuginfo_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeCodeDebugInfoObject *self, DeeCodeDebugInfoObject *right) {
 if (right->di_lno) {
  if DEE_UNLIKELY((self->di_lno = _DeeCodeLnOffList_Copy(right->di_lno)) == NULL) return -1;
 } else self->di_lno = NULL;
 if (right->di_fno) {
  if DEE_UNLIKELY((self->di_fno = _DeeCodeLnOffList_Copy(right->di_fno)) == NULL) {
   if (self->di_lno) _DeeCodeLnOffList_Free(self->di_lno);
   return -1;
  }
 } else self->di_fno = NULL;
 Dee_XINCREF(self->di_file_names = right->di_file_names);
 Dee_XINCREF(self->di_name = right->di_name);
 Dee_XINCREF(self->di_local_names = right->di_local_names);
 Dee_XINCREF(self->di_const_names = right->di_const_names);
 Dee_XINCREF(self->di_arg_names = right->di_arg_names);
 Dee_XINCREF(self->di_ref_names = right->di_ref_names);
 return 0;
}

static int _dee_check_string_tuple(DeeTupleObject *str_tuple) {
 DeeObject **iter,**end;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)str_tuple,&DeeTuple_Type) != 0) return -1;
 end = (iter = DeeTuple_ELEM(str_tuple))+DeeTuple_SIZE(str_tuple);
 while (iter != end) {
  if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(*iter,&DeeString_Type) != 0) return -1;
  ++iter;
 }
 return 0;
}
static int _deecodedebuginfo_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeCodeDebugInfoObject *self, DeeObject *args) {
 self->di_name = NULL; self->di_local_names = NULL;
 self->di_const_names = NULL; self->di_arg_names = NULL;
 self->di_ref_names = NULL;
 self->di_lno = NULL; self->di_fno = NULL;
 self->di_file_names = NULL;
 if (DeeTuple_Unpack(args,"|ooooo",
  &self->di_name,&self->di_local_names,
  &self->di_const_names,&self->di_arg_names,
  &self->di_ref_names) != 0) return -1;
 if (self->di_local_names) {
  if DEE_UNLIKELY(_dee_check_string_tuple(self->di_local_names) != 0) return -1;
  if DEE_UNLIKELY(DeeTuple_SIZE(self->di_local_names) == 0) self->di_local_names = NULL;
 }
 if (self->di_const_names) {
  if DEE_UNLIKELY(_dee_check_string_tuple(self->di_const_names) != 0) return -1;
  if DEE_UNLIKELY(DeeTuple_SIZE(self->di_const_names) == 0) self->di_const_names = NULL;
 }
 if (self->di_arg_names) {
  if DEE_UNLIKELY(_dee_check_string_tuple(self->di_arg_names) != 0) return -1;
  if DEE_UNLIKELY(DeeTuple_SIZE(self->di_arg_names) == 0) self->di_arg_names = NULL;
 }
 if (self->di_ref_names) {
  if DEE_UNLIKELY(_dee_check_string_tuple(self->di_ref_names) != 0) return -1;
  if DEE_UNLIKELY(DeeTuple_SIZE(self->di_ref_names) == 0) self->di_ref_names = NULL;
 }
 if (self->di_name) {
  if DEE_UNLIKELY((self->di_name = (DeeStringObject *)
   DeeString_Cast((DeeObject *)self->di_name)) == NULL) return -1;
  if DEE_UNLIKELY(DeeString_CheckEmpty(self->di_name)) Dee_CLEAR(self->di_name);
 }
 Dee_XINCREF(self->di_local_names);
 Dee_XINCREF(self->di_const_names);
 Dee_XINCREF(self->di_arg_names);
 Dee_XINCREF(self->di_ref_names);
 return 0;
}
static void _deecodedebuginfo_tp_dtor(DeeCodeDebugInfoObject *self) {
 Dee_XDECREF(self->di_name);
 Dee_XDECREF(self->di_local_names);
 Dee_XDECREF(self->di_const_names);
 Dee_XDECREF(self->di_arg_names);
 Dee_XDECREF(self->di_ref_names);
 Dee_XDECREF(self->di_file_names);
 if (self->di_lno) _DeeCodeLnOffList_Free(self->di_lno);
 if (self->di_fno) _DeeCodeLnOffList_Free(self->di_fno);
}
DEE_VISIT_PROC(_deecodedebuginfo_tp_visit,DeeCodeDebugInfoObject *self) {
 Dee_XVISIT(self->di_name);
 Dee_XVISIT(self->di_local_names);
 Dee_XVISIT(self->di_const_names);
 Dee_XVISIT(self->di_arg_names);
 Dee_XVISIT(self->di_ref_names);
 Dee_XVISIT(self->di_file_names);
}

DeeString_NEW_STATIC(_deecode_default_name,"<unnamed>");
static DeeObject *_deecodedebuginfo_tp_str(DeeCodeDebugInfoObject *self) {
 DeeReturn_NEWREF(self->di_name
  ? (DeeObject *)self->di_name
  : (DeeObject *)&_deecode_default_name);
}
static DeeObject *_deecodedebuginfo_tp_cmp_eq(
 DeeCodeDebugInfoObject *self, DeeCodeDebugInfoObject *right) {
 int temp;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((
  DeeObject *)right,&DeeCodeDebugInfo_Type) != 0) return NULL;
#define XCOMPARE_OBJECTS(T,a,b)\
 if (a || b) {\
  if (!a || !b) DeeReturn_False;\
  if DEE_UNLIKELY((temp = DeeObject_TCompareNe(T,(DeeObject *)a,(DeeObject *)b)) < 0) return NULL;\
  if (temp) DeeReturn_False;\
 }
 XCOMPARE_OBJECTS(&DeeString_Type,self->di_name,right->di_name);
 XCOMPARE_OBJECTS(&DeeTuple_Type,self->di_local_names,right->di_local_names);
 XCOMPARE_OBJECTS(&DeeTuple_Type,self->di_const_names,right->di_const_names);
 XCOMPARE_OBJECTS(&DeeTuple_Type,self->di_arg_names,right->di_arg_names);
 XCOMPARE_OBJECTS(&DeeTuple_Type,self->di_ref_names,right->di_ref_names);
 XCOMPARE_OBJECTS(&DeeTuple_Type,self->di_file_names,right->di_file_names);
#undef XCOMPARE_OBJECTS
 if (_DeeCodeLnOffList_CompareNe(self->di_lno,right->di_lno)) DeeReturn_False;
 if (_DeeCodeLnOffList_CompareNe(self->di_fno,right->di_fno)) DeeReturn_False;
 DeeReturn_True;
}
static DeeObject *_deecodedebuginfo_tp_cmp_ne(
 DeeCodeDebugInfoObject *self, DeeCodeDebugInfoObject *right) {
 int temp;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((
  DeeObject *)right,&DeeCodeDebugInfo_Type) != 0) return NULL;
#define XCOMPARE_OBJECTS(T,a,b)\
 if (a || b) {\
  if (!a || !b) DeeReturn_True;\
  if DEE_UNLIKELY((temp = DeeObject_TCompareNe(T,(DeeObject *)a,(DeeObject *)b)) < 0) return NULL;\
  if (temp) DeeReturn_True;\
 }
 XCOMPARE_OBJECTS(&DeeString_Type,self->di_name,right->di_name);
 XCOMPARE_OBJECTS(&DeeTuple_Type,self->di_local_names,right->di_local_names);
 XCOMPARE_OBJECTS(&DeeTuple_Type,self->di_const_names,right->di_const_names);
 XCOMPARE_OBJECTS(&DeeTuple_Type,self->di_arg_names,right->di_arg_names);
 XCOMPARE_OBJECTS(&DeeTuple_Type,self->di_ref_names,right->di_ref_names);
 XCOMPARE_OBJECTS(&DeeTuple_Type,self->di_file_names,right->di_file_names);
#undef XCOMPARE_OBJECTS
 if (_DeeCodeLnOffList_CompareNe(self->di_lno,right->di_lno)) DeeReturn_True;
 if (_DeeCodeLnOffList_CompareNe(self->di_fno,right->di_fno)) DeeReturn_True;
 DeeReturn_False;
}

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deecodedebuginfo_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deecodedebuginfo_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__di_name",DeeCodeDebugInfoObject,di_name,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__di_local_names",DeeCodeDebugInfoObject,di_local_names,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__di_const_names",DeeCodeDebugInfoObject,di_const_names,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__di_arg_names",DeeCodeDebugInfoObject,di_arg_names,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__di_ref_names",DeeCodeDebugInfoObject,di_ref_names,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__di_file_names",DeeCodeDebugInfoObject,di_file_names,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__di_lno",DeeCodeDebugInfoObject,di_lno,p(Dee_uint16_t)),
 DEE_MEMBERDEF_NAMED_RO_v100("__di_fno",DeeCodeDebugInfoObject,di_fno,p(Dee_uint16_t)),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */

static DeeObject *_deecodedebuginfo_localname(
 DeeCodeDebugInfoObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint16_t local_id;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I16u:localname",&local_id) != 0) return NULL;
 if (!self->di_local_names || local_id >= DeeTuple_SIZE(self->di_local_names) ||
     !DeeString_Check(DeeTuple_GET(self->di_local_names,local_id))
     ) DeeReturn_EmptyString;
 DeeReturn_NEWREF(DeeTuple_GET(self->di_local_names,local_id));
}
static DeeObject *_deecodedebuginfo_constname(
 DeeCodeDebugInfoObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint16_t const_id;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I16u:constname",&const_id) != 0) return NULL;
 if (!self->di_const_names || const_id >= DeeTuple_SIZE(self->di_const_names) ||
     !DeeString_Check(DeeTuple_GET(self->di_const_names,const_id))
     ) DeeReturn_EmptyString;
 DeeReturn_NEWREF(DeeTuple_GET(self->di_const_names,const_id));
}
static DeeObject *_deecodedebuginfo_refname(
 DeeCodeDebugInfoObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint16_t ref_id;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I16u:refname",&ref_id) != 0) return NULL;
 if (!self->di_ref_names || ref_id >= DeeTuple_SIZE(self->di_ref_names) ||
     !DeeString_Check(DeeTuple_GET(self->di_ref_names,ref_id))
     ) DeeReturn_EmptyString;
 DeeReturn_NEWREF(DeeTuple_GET(self->di_ref_names,ref_id));
}
static DeeObject *_deecodedebuginfo_argname(
 DeeCodeDebugInfoObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint16_t arg_id;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I16u:argname",&arg_id) != 0) return NULL;
 if (!self->di_arg_names || arg_id >= DeeTuple_SIZE(self->di_arg_names) ||
     !DeeString_Check(DeeTuple_GET(self->di_arg_names,arg_id))
     ) DeeReturn_EmptyString;
 DeeReturn_NEWREF(DeeTuple_GET(self->di_arg_names,arg_id));
}
static DeeObject *_deecodedebuginfo_addr2line(
 DeeCodeDebugInfoObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t addr;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu:addr2line",&addr) != 0) return NULL;
 return DeeObject_New(Dee_int64_t,DeeCodeDebugInfo_ADDR2LINE(self,addr));
}
static DeeObject *_deecodedebuginfo_addr2file(
 DeeCodeDebugInfoObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t addr;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu:addr2line",&addr) != 0) return NULL;
 DeeReturn_XNEWREF(DeeCodeDebugInfo_ADDR2FILE(self,addr));
}


static struct DeeMethodDef const _deecodedebuginfo_tp_methods[] = {
 DEE_METHODDEF_v100("addr2line",member(&_deecodedebuginfo_addr2line),
  "(size_t addr) -> int64_t\n\tReturns the source-code line associated with a given address, or -1 if no such data is available.\n"
  "\tNote: The first line is 0, meaning you should add +1 when presenting to the user"),
 DEE_METHODDEF_v100("addr2file",member(&_deecodedebuginfo_addr2file),
  "(size_t addr) -> string\n\tReturns the source-code filename associated with a given address, or an empty string if no such data is available.\n"),
 DEE_METHODDEF_v100("localname",member(&_deecodedebuginfo_localname),"(uint16_t id) -> string\n\tReturns the name of a given local variable id, or an empty string if its name is unknown"),
 DEE_METHODDEF_v100("constname",member(&_deecodedebuginfo_constname),"(uint16_t id) -> string\n\tReturns the name of a given constant id, or an empty string if its name is unknown"),
 DEE_METHODDEF_v100("refname",member(&_deecodedebuginfo_refname),"(uint16_t id) -> string\n\tReturns the name of a given reference id, or an empty string if its name is unknown"),
 DEE_METHODDEF_v100("argname",member(&_deecodedebuginfo_argname),"(uint16_t id) -> string\n\tReturns the name of a given argument id, or an empty string if its name is unknown"),
 DEE_METHODDEF_END_v100
};

static DeeObject *_deecodedebuginfo_name_get(
 DeeCodeDebugInfoObject *self, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(!self->di_name) DeeReturn_EmptyString;
 DEE_ASSERT(DeeString_Check(self->di_name));
 DeeReturn_NEWREF(self->di_name);
}

static struct DeeGetSetDef const _deecodedebuginfo_tp_getsets[] = {
 DEE_GETSETDEF_v100("name",member(&_deecodedebuginfo_name_get),null,null,
  "-> string\n"
  "\tReturns the name of the code, or an empty string if unknown"),
 DEE_GETSETDEF_END_v100
};




static int DEE_CALL _deecodedebuginfo_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeCodeDebugInfoObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_size_t lno_size,fno_size; Dee_uint64_t read_lno_size,read_fno_size;
 if DEE_UNLIKELY((self->di_name        = (struct DeeStringObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if DEE_UNLIKELY((self->di_local_names = (struct DeeTupleObject  *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {err_name: Dee_DECREF(self->di_name); return -1; }
 if DEE_UNLIKELY((self->di_const_names = (struct DeeTupleObject  *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {err_local: Dee_DECREF(self->di_local_names); goto err_name; }
 if DEE_UNLIKELY((self->di_ref_names   = (struct DeeTupleObject  *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {err_const: Dee_DECREF(self->di_const_names); goto err_local; }
 if DEE_UNLIKELY((self->di_arg_names   = (struct DeeTupleObject  *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {err_ref: Dee_DECREF(self->di_ref_names); goto err_const; }
 if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&read_lno_size) != 0) {err_arg: Dee_DECREF(self->di_arg_names); goto err_ref; }
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_lno_size > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Code debuginfo line-notab is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_lno_size,(Dee_size_t)-1);
  goto err_arg;
 }
#endif
 lno_size = (Dee_size_t)read_lno_size;
 if (lno_size) {
  while DEE_UNLIKELY((self->di_lno = (struct _DeeCodeLnOffList *)malloc_nz(
   (lno_size+1)*sizeof(struct _DeeCodeLnOffList))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   goto err_arg;
  }
  if DEE_UNLIKELY(DeeFile_ReadAll(infile,self->di_lno,lno_size*sizeof(struct _DeeCodeLnOffList)) != 0)
  {err_lno: free_nn(self->di_lno); goto err_arg; }
  ((Dee_uint16_t *)self->di_lno)[lno_size] = 0;
 } else self->di_lno = NULL;
 if (map->mrm_version >= DEE_MARSHAL_V102) {
  // Read the filename-tab
  if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&read_fno_size) != 0) goto err_lno;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
  if DEE_UNLIKELY(read_fno_size > (Dee_uint64_t)((Dee_size_t)-1)) {
   DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Code debuginfo file-notab is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                       read_fno_size,(Dee_size_t)-1);
   goto err_lno;
  }
#endif
  fno_size = (Dee_size_t)read_fno_size;
  if (fno_size) {
   while DEE_UNLIKELY((self->di_fno = (struct _DeeCodeLnOffList *)malloc_nz(
    (fno_size+1)*sizeof(struct _DeeCodeLnOffList))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
    goto err_lno;
   }
   if DEE_UNLIKELY(DeeFile_ReadAll(infile,self->di_fno,fno_size*sizeof(struct _DeeCodeLnOffList)) != 0)
   {err_fno: free_nn(self->di_fno); goto err_lno; }
   ((Dee_uint16_t *)self->di_fno)[fno_size] = 0;
   // Read the filename tab
   if DEE_UNLIKELY((self->di_file_names = (struct DeeTupleObject  *)
    DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) goto err_fno;
  } else {
   // Empty filename tab
   self->di_fno = NULL;
   self->di_file_names = NULL;
  }
 } else {
  // No filename tab prior to v102
  self->di_fno = NULL;
  self->di_file_names = NULL;
 }

 // Do some cleanup
 if (DeeNone_Check(self->di_arg_names) || DeeTuple_CheckEmpty(self->di_arg_names)) Dee_CLEAR(self->di_arg_names);
 if (DeeNone_Check(self->di_ref_names) || DeeTuple_CheckEmpty(self->di_ref_names)) Dee_CLEAR(self->di_ref_names);
 if (DeeNone_Check(self->di_const_names) || DeeTuple_CheckEmpty(self->di_const_names)) Dee_CLEAR(self->di_const_names);
 if (DeeNone_Check(self->di_local_names) || DeeTuple_CheckEmpty(self->di_local_names)) Dee_CLEAR(self->di_local_names);
 if (DeeNone_Check(self->di_file_names) || DeeTuple_CheckEmpty(self->di_file_names)) Dee_CLEAR(self->di_file_names);
 if (DeeNone_Check(self->di_name) || DeeString_CheckEmpty(self->di_name)) Dee_CLEAR(self->di_name);
 // Do some checks on the object types
 if DEE_UNLIKELY(self->di_arg_names && DeeError_TypeError_CheckTypeExact((DeeObject *)self->di_arg_names,&DeeTuple_Type) != 0) goto err;
 if DEE_UNLIKELY(self->di_ref_names && DeeError_TypeError_CheckTypeExact((DeeObject *)self->di_ref_names,&DeeTuple_Type) != 0) goto err;
 if DEE_UNLIKELY(self->di_const_names && DeeError_TypeError_CheckTypeExact((DeeObject *)self->di_const_names,&DeeTuple_Type) != 0) goto err;
 if DEE_UNLIKELY(self->di_local_names && DeeError_TypeError_CheckTypeExact((DeeObject *)self->di_local_names,&DeeTuple_Type) != 0) goto err;
 if DEE_UNLIKELY(self->di_file_names && DeeError_TypeError_CheckTypeExact((DeeObject *)self->di_file_names,&DeeTuple_Type) != 0) goto err;
 if DEE_UNLIKELY(self->di_name && DeeError_TypeError_CheckTypeExact((DeeObject *)self->di_name,&DeeString_Type) != 0) goto err;
 return 0;
err:
 Dee_XDECREF(self->di_name);
 Dee_XDECREF(self->di_local_names);
 Dee_XDECREF(self->di_const_names);
 Dee_XDECREF(self->di_ref_names);
 Dee_XDECREF(self->di_arg_names);
 Dee_XDECREF(self->di_file_names);
 if (self->di_lno) _DeeCodeLnOffList_Free(self->di_lno);
 if (self->di_fno) _DeeCodeLnOffList_Free(self->di_fno);
 return -1;
}

static int DEE_CALL _deecodedebuginfo_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeCodeDebugInfoObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 Dee_uint64_t lno_size,fno_size;
 if DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,self->di_name ? (DeeObject *)self->di_name : Dee_None) != 0) return -1;
 if DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,self->di_local_names ? (DeeObject *)self->di_local_names : Dee_None) != 0) return -1;
 if DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,self->di_const_names ? (DeeObject *)self->di_const_names : Dee_None) != 0) return -1;
 if DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,self->di_ref_names ? (DeeObject *)self->di_ref_names : Dee_None) != 0) return -1;
 if DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,self->di_arg_names ? (DeeObject *)self->di_arg_names : Dee_None) != 0) return -1;
 if (self->di_lno) {
  lno_size = (Dee_uint64_t)(_DeeCodeLnOffList_Size(self->di_lno));
  if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,lno_size) != 0) return -1;
  if DEE_UNLIKELY(DeeFile_WriteAll(outfile,self->di_lno,((Dee_size_t)lno_size)*sizeof(struct _DeeCodeLnOffList)) != 0) return -1;
 } else if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,0) != 0) return -1;
 if (self->di_fno) {
  fno_size = (Dee_uint64_t)(_DeeCodeLnOffList_Size(self->di_fno));
  if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,fno_size) != 0) return -1;
  if DEE_UNLIKELY(DeeFile_WriteAll(outfile,self->di_fno,((Dee_size_t)fno_size)*sizeof(struct _DeeCodeLnOffList)) != 0) return -1;
  if DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,self->di_file_names ? (DeeObject *)self->di_file_names : Dee_None) != 0) return -1;
 } else if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,0) != 0) return -1;
 return 0;
}

static struct DeeTypeMarshal _deecodedebuginfo_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(code_debuginfo)),
 member(&_deecodedebuginfo_tp_marshal_ctor),
 member(&_deecodedebuginfo_tp_marshal_put));


DeeTypeObject DeeCodeDebugInfo_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("code.debuginfo"),member(
  "()\n"
  "(string name)\n"
  "(string name, tuple local_names)\n"
  "(string name, tuple local_names, tuple const_names)\n"
  "(string name, tuple local_names, tuple const_names, tuple arg_names)\n"
  "(string name, tuple local_names, tuple const_names, tuple arg_names, tuple ref_names)"),
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeCodeDebugInfoObject),null,
  member(&_deecodedebuginfo_tp_ctor),
  member(&_deecodedebuginfo_tp_copy_ctor),null,
  member(&_deecodedebuginfo_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deecodedebuginfo_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deecodedebuginfo_tp_str),null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deecodedebuginfo_tp_visit),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,
  member(&_deecodedebuginfo_tp_cmp_eq),
  member(&_deecodedebuginfo_tp_cmp_ne),null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deecodedebuginfo_tp_members),member(_deecodedebuginfo_tp_getsets),
  member(_deecodedebuginfo_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deecodedebuginfo_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};



static void _deecode_tp_dtor(DeeCodeObject *self) {
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 struct DeeExceptionHandlerEntry *begin,*end;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 Dee_DECREF(self->co_consts);
 Dee_XDECREF(self->co_debug);
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 end = (begin = self->co_exceptv)+self->co_exceptc;
 while (begin != end) { _DeeExceptionHandlerEntry_Quit(begin); ++begin; }
 free(self->co_exceptv);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
}
static DeeCodeObject *_deecode_tp_copy_new(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeCodeObject *self) {
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 struct DeeExceptionHandlerEntry *begin,*end,*src;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 DeeCodeObject *result;
 if ((result = (DeeCodeObject *)DeeObject_Malloc(
  Dee_OFFSETOF(DeeCodeObject,co_code)+DeeCode_SIZE(self))) != NULL) return NULL;
 _DeeCode_InitCommon(result);
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 if ((result->co_exceptc = self->co_exceptc) != 0) {
  while ((begin = (struct DeeExceptionHandlerEntry *)malloc_nz(
   result->co_exceptc*sizeof(struct DeeExceptionHandlerEntry))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   DeeObject_Free(result);
   return NULL;
  }
  end = (result->co_exceptv = begin)+result->co_exceptc;
  src = self->co_exceptv;
  while (begin != end) {
   _DeeExceptionHandlerEntry_InitCopy(begin,src);
   ++begin,++src;
  }
 } else result->co_exceptv = NULL;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 Dee_XINCREF(result->co_debug = self->co_debug);
 Dee_INCREF(result->co_consts = self->co_consts);
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 result->co_exceptsize = self->co_exceptsize;
 result->co_finallyc = self->co_finallyc;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 result->co_nlocals = self->co_nlocals;
 result->co_stacksize = self->co_stacksize;
 result->co_size = self->co_size;
 // Copy the rest...
 memcpy(&result->co_wsize,&self->co_wsize,self->co_size);
 DeeObject_INIT(result,&DeeCode_Type);
 return result;
}
DEE_VISIT_PROC(_deecode_tp_visit,DeeCodeObject *self) {
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 struct DeeExceptionHandlerEntry *begin,*end;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 DEE_ASSERT(DeeObject_Check(self) && DeeCode_Check(self));
 // v technically only required for static variables,
 //   but it would be too complicated to figure out which are static.
 DeeAtomicMutex_Acquire(&self->co_consts_lock);
 Dee_VISIT(self->co_consts);
 DeeAtomicMutex_Release(&self->co_consts_lock);
 Dee_XVISIT(self->co_debug);
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 end = (begin = self->co_exceptv)+self->co_exceptc;
 while (begin != end) { _DeeExceptionHandlerEntry_Visit(begin); ++begin; }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
}
static DeeObject *_deecode_tp_cmp_eq(DeeCodeObject *self, DeeCodeObject *right) {
 int temp;
 if DEE_UNLIKELY((right = (DeeCodeObject *)DeeObject_GetInstance((
  DeeObject *)right,&DeeCode_Type)) == NULL) return NULL;
 if (Dee_MemCmpLen(
  DeeCode_SIZE(self),DeeCode_CODE(self),
  DeeCode_SIZE(right),DeeCode_CODE(right)) != 0) DeeReturn_False;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 if (self->co_exceptc != right->co_exceptc) DeeReturn_False;
 if (self->co_finallyc != right->co_finallyc) DeeReturn_False;
 if (self->co_exceptsize != right->co_exceptsize) DeeReturn_False;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 if (self->co_nlocals != right->co_nlocals) DeeReturn_False;
 if (self->co_stacksize != right->co_stacksize) DeeReturn_False;
 if (self->co_wsize != right->co_wsize) DeeReturn_False;
 if ((temp = DeeObject_CompareNe((DeeObject *)self->co_consts,(DeeObject *)right->co_consts)) == -1) return NULL;
 if (temp) DeeReturn_False;
 if (self->co_debug || right->co_debug) {
  if (!self->co_debug || !right->co_debug) DeeReturn_False;
  if ((temp = DeeObject_CompareNe(
   (DeeObject *)self->co_debug,
   (DeeObject *)right->co_debug)) != 0) return NULL;
  if (temp) DeeReturn_False;
 }
 DeeReturn_True;
}
static DeeObject *_deecode_tp_cmp_ne(DeeCodeObject *self, DeeCodeObject *right) {
 int temp;
 if DEE_UNLIKELY((right = (DeeCodeObject *)DeeObject_GetInstance((
  DeeObject *)right,&DeeCode_Type)) == NULL) return NULL;
 if (Dee_MemCmpLen(
  DeeCode_SIZE(self),DeeCode_CODE(self),
  DeeCode_SIZE(right),DeeCode_CODE(right)) != 0) DeeReturn_True;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 if (self->co_exceptc != right->co_exceptc) DeeReturn_True;
 if (self->co_finallyc != right->co_finallyc) DeeReturn_True;
 if (self->co_exceptsize != right->co_exceptsize) DeeReturn_True;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 if (self->co_nlocals != right->co_nlocals) DeeReturn_True;
 if (self->co_stacksize != right->co_stacksize) DeeReturn_True;
 if (self->co_wsize != right->co_wsize) DeeReturn_True;
 if ((temp = DeeObject_CompareNe((DeeObject *)self->co_consts,(DeeObject *)right->co_consts)) == -1) return NULL;
 if (temp) DeeReturn_True;
 if (self->co_debug || right->co_debug) {
  if (!self->co_debug || !right->co_debug) DeeReturn_False;
  if ((temp = DeeObject_CompareNe(
   (DeeObject *)self->co_debug,
   (DeeObject *)right->co_debug)) != 0) return NULL;
  if (temp) DeeReturn_True;
 }
 DeeReturn_False;
}


static DeeObject *_deecode_tp_str(DeeCodeObject *self) {
 DeeReturn_NEWREF(DeeCode_NAME(self) ? DeeCode_NAME(self) : (DeeObject *)&_deecode_default_name);
}
static DeeObject *_deecode_tp_repr(DeeCodeObject *self) {
 return DeeString_Newf("<code: %q>",DeeCode_NAME(self)
                       ? DeeString_STR(DeeCode_NAME(self))
                       : DeeString_STR(&_deecode_default_name));
}
static DeeObject *_deecode_name_get(DeeCodeObject *self, void *DEE_UNUSED(closure)) {
 if (DeeCode_NAME(self)) DeeReturn_NEWREF(DeeCode_NAME(self));
 DeeReturn_EmptyString;
}
static DeeObject *_deecode_asmrepr(
 DeeCodeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":asmrepr") != 0) return NULL;
 return DeeCode_AsmRepr((DeeObject *)self);
}
static DeeObject *_deecode_instruction_count(
 DeeCodeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":instruction_count") != 0) return NULL;
 return DeeObject_New(Dee_size_t,DeeCode_InstructionCount((DeeObject *)self));
}

static struct DeeMemberDef const _deecode_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("debuginfo",object,&DeeCodeDebugInfo_Type),
 DEE_MEMBERDEF_CONST_v100("empty_code",object,&_Dee_EmptyCode),
 DEE_MEMBERDEF_END_v100
};
#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deecode_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deecode_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__co_debug",DeeCodeObject,co_debug,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__co_consts_lock",DeeCodeObject,co_consts_lock,DeeAtomicMutex),
 DEE_MEMBERDEF_NAMED_RO_v100("__co_consts",DeeCodeObject,co_consts,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__co_flags",DeeCodeObject,co_flags,Dee_uint16_t),
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 DEE_MEMBERDEF_NAMED_RO_v100("__co_exceptv",DeeCodeObject,co_exceptv,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__co_exceptc",DeeCodeObject,co_exceptc,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__co_finallyc",DeeCodeObject,co_finallyc,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__co_exceptsize",DeeCodeObject,co_exceptsize,Dee_uint16_t),
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 DEE_MEMBERDEF_NAMED_RO_v100("__co_nlocals",DeeCodeObject,co_nlocals,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__co_stacksize",DeeCodeObject,co_stacksize,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__co_wsize",DeeCodeObject,co_wsize,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__co_woff_loc",DeeCodeObject,co_woff_loc,Dee_size_t),
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 DEE_MEMBERDEF_NAMED_RO_v100("__co_woff_except",DeeCodeObject,co_woff_except,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__co_woff_finally",DeeCodeObject,co_woff_finally,Dee_size_t),
#endif
 DEE_MEMBERDEF_NAMED_RO_v100("__co_size",DeeCodeObject,co_size,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__co_code",DeeCodeObject,co_code,Dee_uint8_t),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */

static struct DeeGetSetDef const _deecode_tp_getsets[] = {
 DEE_GETSETDEF_v100("name",member(&_deecode_name_get),null,null,"-> string\n"
  "@return: Name of the code, or an empty string if unknown."),
 DEE_GETSETDEF_END_v100
};
static struct DeeMethodDef const _deecode_tp_methods[] = {
 DEE_METHODDEF_v100("asmrepr",member(&_deecode_asmrepr),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("instruction_count",member(&_deecode_instruction_count),
  "() -> size_t\n"
  "@return: The total amount of individual instructions"),
 DEE_METHODDEF_END_v100
};

DEE_COMPILER_STRUCTURE_PACKED_BEGIN
//////////////////////////////////////////////////////////////////////////
// Old headers
struct _DeeExceptionHandlerEntryFileHeader101_ {
 Dee_uint8_t               ehefh_kind;    /*< s.a.: 'DeeExceptionHandlerEntry::e_kind'. */
 Dee_uint16_t              ehefh_stack;   /*< s.a.: 'DeeExceptionHandlerEntry::e_stack'. */
};
struct _DeeExceptionHandlerEntryFileHeader101 {
 Dee_uint8_t               ehefh_kind;    /*< s.a.: 'DeeExceptionHandlerEntry::e_kind'. */
 Dee_uint16_t              ehefh_stack;   /*< s.a.: 'DeeExceptionHandlerEntry::e_stack'. */
 /*small*/Dee_uint64_t     ehefh_handler; /*< s.a.: 'DeeExceptionHandlerEntry::e_handler'. */
 //[optional] Dee_uint16_t ehefh_store;   /*< [if(ehefh_kind&DeeExceptionHandleKind_FLAG_VAR)] s.a.: 'DeeExceptionHandlerEntry::e_store'. */
 //[optional] object       ehefh_type;    /*< [if(ehefh_kind&DeeExceptionHandleKind_FLAG_TYPED)] s.a.: 'DeeExceptionHandlerEntry::e_type'. */
};
struct _DeeCodeFileHeader101_ {
          Dee_uint16_t cfh_exceptsize;     /*< s.a.: DeeCodeObject::co_exceptsize. */
          Dee_uint16_t cfh_nlocals;        /*< s.a.: DeeCodeObject::co_nlocals. */
          Dee_uint16_t cfh_flags;          /*< s.a.: DeeCodeObject::co_flags. */
};
struct _DeeCodeFileHeader101 {
          Dee_uint16_t cfh_exceptsize;     /*< s.a.: DeeCodeObject::co_exceptsize. */
          Dee_uint16_t cfh_nlocals;        /*< s.a.: DeeCodeObject::co_nlocals. */
          Dee_uint16_t cfh_flags;          /*< s.a.: DeeCodeObject::co_flags. */
 /*small*/Dee_uint64_t cfh_size;           /*< Size of the bytecode (== DeeCode_SIZE(...)). */
 /*small*/Dee_uint64_t cfh_exceptc;        /*< s.a.: DeeCodeObject::co_exceptc. */
 /*small*/Dee_uint64_t cfh_finallyc;       /*< s.a.: DeeCodeObject::co_finallyc. */
 /*small*/Dee_uint64_t cfh_stacksize;      /*< s.a.: DeeCodeObject::co_stacksize. */
 //       object       cfh_debug;          /*< s.a.: DeeCodeObject::co_debug. (NOTE: 'none' is an alias for NULL/empty) */
 //       object       cfh_consts;         /*< s.a.: DeeCodeObject::co_consts. (NOTE: 'none' is an alias for empty) */
 //       struct _DeeExceptionHandlerEntryFileHeader cfh_exceptv[cfh_exceptc]; /*< s.a.: DeeCodeObject::co_exceptv. */
 //       Dee_uint8_t  cfh_code[cfh_size]; /*< s.a.: DeeCodeObject::co_code. (NOTE: opargs are written in lil-endian) */
};

struct _DeeExceptionHandlerEntryFileHeader_ {
 Dee_uint8_t               ehefh_kind;    /*< s.a.: 'DeeExceptionHandlerEntry::e_kind'. */
};
struct _DeeExceptionHandlerEntryFileHeader {
 Dee_uint8_t                        ehefh_kind;    /*< s.a.: 'DeeExceptionHandlerEntry::e_kind'. */
 /*small*/Dee_uint64_t              ehefh_stack;   /*< s.a.: 'DeeExceptionHandlerEntry::e_stack'. */
 /*small*/Dee_uint64_t              ehefh_handler; /*< s.a.: 'DeeExceptionHandlerEntry::e_handler'. */
 //[optional] /*small*/Dee_uint64_t ehefh_store;   /*< [if(ehefh_kind&DeeExceptionHandleKind_FLAG_VAR)] s.a.: 'DeeExceptionHandlerEntry::e_store'. */
 //[optional] object                ehefh_type;    /*< [if(ehefh_kind&DeeExceptionHandleKind_FLAG_TYPED)] s.a.: 'DeeExceptionHandlerEntry::e_type'. */
};
struct _DeeCodeFileHeader_ {
          Dee_uint16_t cfh_flags;          /*< s.a.: DeeCodeObject::co_flags. */
};
struct _DeeCodeFileHeader {
          Dee_uint16_t cfh_flags;          /*< s.a.: DeeCodeObject::co_flags. */
 /*small*/Dee_uint64_t cfh_size;           /*< Size of the bytecode (== DeeCode_SIZE(...)). */
 /*small*/Dee_uint64_t cfh_nlocals;        /*< s.a.: DeeCodeObject::co_nlocals. */
 /*small*/Dee_uint64_t cfh_exceptsize;     /*< s.a.: DeeCodeObject::co_exceptsize. */
 /*small*/Dee_uint64_t cfh_exceptc;        /*< s.a.: DeeCodeObject::co_exceptc. */
 /*small*/Dee_uint64_t cfh_finallyc;       /*< s.a.: DeeCodeObject::co_finallyc. */
 /*small*/Dee_uint64_t cfh_stacksize;      /*< s.a.: DeeCodeObject::co_stacksize. */
 /*small*/Dee_uint64_t cfh_stackdata;      /*< s.a.: DeeCodeObject::co_woff_sdata (size of the stack data in bytes). */
 //       object       cfh_debug;          /*< s.a.: DeeCodeObject::co_debug. (NOTE: 'none' is an alias for NULL/empty) */
 //       object       cfh_consts;         /*< s.a.: DeeCodeObject::co_consts. (NOTE: 'none' is an alias for empty) */
 //       struct _DeeExceptionHandlerEntryFileHeader cfh_exceptv[cfh_exceptc]; /*< s.a.: DeeCodeObject::co_exceptv. */
 //       Dee_uint8_t  cfh_code[cfh_size]; /*< s.a.: DeeCodeObject::co_code. (NOTE: opargs are written in lil-endian) */
};
DEE_COMPILER_STRUCTURE_PACKED_END

DEE_COMPILER_MSVC_WARNING_PUSH(4366)
static DeeCodeObject *DEE_CALL _deecode_tp_marshal_new(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *infile, struct DeeMarshalReadMap *map,
 PDeeMashalNewAfterAlloc after_alloc, void *after_alloc_data) {
 DeeCodeObject *result; struct _DeeCodeFileHeader header;
 struct DeeExceptionHandlerEntry *iter,*end;
 if (map->mrm_version <= DEE_MARSHAL_V101) {
  struct _DeeCodeFileHeader101_ header101; // Backwards compatibility with 101:1
  DEE_LVERBOSE3("[Backward] Reading code object from v101\n");
  if (DeeFile_ReadAll(infile,&header101,sizeof(header101)) != 0) return NULL;
  if (DeeFile_GetLeSmall64(infile,&header.cfh_size) != 0) return NULL;
  header.cfh_flags = header101.cfh_flags;
  header.cfh_nlocals = (Dee_uint64_t)DEE_BUILTIN_LESWAP16(header101.cfh_nlocals);
  header.cfh_exceptsize = (Dee_uint64_t)DEE_BUILTIN_LESWAP16(header101.cfh_exceptsize);
 } else {
  // The new way (nlocals is now a small64)
  if (DeeFile_ReadAll(infile,&header,Dee_OFFSETOF(struct _DeeCodeFileHeader,cfh_size)) != 0
   || DeeFile_GetLeSmall64(infile,&header.cfh_size) != 0
   || DeeFile_GetLeSmall64(infile,&header.cfh_nlocals) != 0
   || DeeFile_GetLeSmall64(infile,&header.cfh_exceptsize) != 0
   ) return NULL;
 }
#ifndef DEE_PLATFORM_LIL_ENDIAN
 header.cfh_flags = DEE_BUILTIN_LESWAP16(header.cfh_flags);
#endif
 if (DeeFile_GetLeSmall64(infile,&header.cfh_exceptc) != 0 ||
     DeeFile_GetLeSmall64(infile,&header.cfh_finallyc) != 0 ||
     DeeFile_GetLeSmall64(infile,&header.cfh_stacksize) != 0
     ) return NULL;
 if (map->mrm_version <= DEE_MARSHAL_V101) {
  header.cfh_stackdata = 0;
 } else {
  if (DeeFile_GetLeSmall64(infile,&header.cfh_stackdata) != 0) return NULL;
 }
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(header.cfh_size > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Code byte-size is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      header.cfh_size,(Dee_size_t)-1);
  return NULL;
 }
 if DEE_UNLIKELY(header.cfh_nlocals > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Code locals-size is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      header.cfh_nlocals,(Dee_size_t)-1);
  return NULL;
 }
 if DEE_UNLIKELY(header.cfh_exceptc > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Code exception-size is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      header.cfh_exceptc,(Dee_size_t)-1);
  return NULL;
 }
 if DEE_UNLIKELY(header.cfh_finallyc > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Code finally-size is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      header.cfh_finallyc,(Dee_size_t)-1);
  return NULL;
 }
 if DEE_UNLIKELY(header.cfh_stacksize > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Code stack-size is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      header.cfh_stacksize,(Dee_size_t)-1);
  return NULL;
 }
 if DEE_UNLIKELY(header.cfh_stackdata > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Code stack-data is too big for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      header.cfh_stackdata,(Dee_size_t)-1);
  return NULL;
 }
#endif
 result = (DeeCodeObject *)DeeObject_Malloc(Dee_OFFSETOF(DeeCodeObject,co_code)+
                                            ((Dee_size_t)header.cfh_size)*sizeof(Dee_uint8_t));
 if DEE_UNLIKELY(!result) return NULL;
 DeeObject_INIT(result,&DeeCode_Type);
 (*after_alloc)((DeeObject *)result,after_alloc_data);
 // Not for initializing the code object
 result->co_size = (Dee_size_t)header.cfh_size;
 result->co_flags = header.cfh_flags;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 result->co_exceptsize = (Dee_size_t)header.cfh_exceptsize;
 result->co_exceptc = (Dee_size_t)header.cfh_exceptc;
 result->co_finallyc = (Dee_size_t)header.cfh_finallyc;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 result->co_nlocals = (Dee_size_t)header.cfh_nlocals;
 result->co_stacksize = (Dee_size_t)header.cfh_stacksize;
 if ((result->co_debug = (DeeCodeDebugInfoObject *)
  DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {
err_r: _DeeObject_DELETE(&DeeCode_Type,result); return NULL;
 }
 if (DeeNone_Check(result->co_debug)) Dee_CLEAR(result->co_debug);
 else if (DeeError_TypeError_CheckTypeExact((DeeObject *)result->co_debug,&DeeCodeDebugInfo_Type) != 0) {
err_r_debug: Dee_XDECREF(result->co_debug); goto err_r;
 } else {
  if (!result->co_debug->di_name && !result->co_debug->di_local_names &&
      !result->co_debug->di_const_names && !result->co_debug->di_ref_names &&
      !result->co_debug->di_arg_names && !result->co_debug->di_lno &&
      !result->co_debug->di_fno && !result->co_debug->di_file_names) Dee_CLEAR(result->co_debug);
 }
 if ((result->co_consts = (DeeTupleObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) goto err_r_debug;
 if (DeeNone_Check(result->co_consts)) { Dee_DECREF(result->co_consts); Dee_INCREF(result->co_consts = (DeeTupleObject *)Dee_EmptyTuple); }
 else if (DeeError_TypeError_CheckTypeExact((DeeObject *)result->co_consts,&DeeTuple_Type) != 0) {
err_r_const: Dee_DECREF(result->co_consts); goto err_r_debug;
 }
 if (result->co_exceptc) {
  while ((result->co_exceptv = (struct DeeExceptionHandlerEntry *)malloc_nz(
   result->co_exceptc*sizeof(struct DeeExceptionHandlerEntry))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   goto err_r_const;
  }
  end = (iter = result->co_exceptv)+result->co_exceptc;
  do {
   int error;
   struct _DeeExceptionHandlerEntryFileHeader hheader;
   if (map->mrm_version <= DEE_MARSHAL_V101) {
    Dee_uint16_t ehefh_stack; // Backwards compatibility
    error = DeeFile_ReadAll(infile,&hheader,Dee_OFFSETOF(
     struct _DeeExceptionHandlerEntryFileHeader,ehefh_stack));
    if (error == 0) {
     error = DeeFile_ReadAll(infile,&ehefh_stack,2);
     hheader.ehefh_stack = (Dee_uint64_t)DEE_BUILTIN_LESWAP16(ehefh_stack);
    }
   } else {
    error = DeeFile_ReadAll(infile,&hheader,Dee_OFFSETOF(
     struct _DeeExceptionHandlerEntryFileHeader,ehefh_stack));
    if (error == 0) error = DeeFile_GetLeSmall64(infile,&hheader.ehefh_stack);
   }
   if DEE_UNLIKELY(error != 0) {
err_r_except_iter:
    while (iter != result->co_exceptv) { --iter; _DeeExceptionHandlerEntry_Quit(iter); }
    free_nn(result->co_exceptv);
    goto err_r_const;
   }
   if (DeeFile_GetLeSmall64(infile,&hheader.ehefh_handler) != 0) goto err_r_except_iter;
   if (
#if DEE_TYPES_SIZEOF_SIZE_T < 8
    hheader.ehefh_stack >= (Dee_uint64_t)((Dee_size_t)-1) ||
#endif
    (Dee_size_t)hheader.ehefh_stack >= (Dee_size_t)header.cfh_stacksize) {
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        "Exception handler stack alignment is too large: (%I64u exceeds maximum of %I64u)",
                        hheader.ehefh_stack,header.cfh_stacksize);
    goto err_r_except_iter;
   }
   if (
#if DEE_TYPES_SIZEOF_SIZE_T < 8
    hheader.ehefh_handler >= (Dee_uint64_t)((Dee_size_t)-1) ||
#endif
    (Dee_size_t)hheader.ehefh_handler >= (Dee_size_t)header.cfh_size) {
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        "Exception handler address is out of bounds: (%I64u exceeds maximum of %I64u)",
                        hheader.ehefh_handler,header.cfh_size);
    goto err_r_except_iter;
   }
   iter->e_kind = (DeeExceptionHandlerKind)hheader.ehefh_kind;
   iter->e_stack = (Dee_size_t)hheader.ehefh_stack;
   iter->e_handler = (Dee_size_t)hheader.ehefh_handler;
   if ((iter->e_kind&DeeExceptionHandleKind_FLAG_VAR)!=0) {
    Dee_uint64_t local_id;
    if (map->mrm_version <= DEE_MARSHAL_V101) {
     Dee_uint16_t local_id16;
     if DEE_UNLIKELY(DeeFile_GetLe(infile,local_id16) != 0) goto err_r_except_iter;
     local_id = (Dee_uint64_t)local_id16;
    } else {
     if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&local_id) != 0) goto err_r_except_iter;
    }
    if (
#if DEE_TYPES_SIZEOF_SIZE_T < 8
     local_id >= (Dee_uint64_t)((Dee_size_t)-1) ||
#endif
     (Dee_size_t)local_id > result->co_nlocals) {
     DeeError_SetStringf(&DeeErrorType_ValueError,
                         "Exception variable storage slot is out of bounds: (%I64u exceeds maximum of %Iu)",
                         local_id,result->co_nlocals);
     goto err_r_except_iter;
    }
    iter->e_store = (Dee_size_t)local_id;
   } else iter->e_store = 0;
   if ((iter->e_kind&DeeExceptionHandleKind_FLAG_TYPED)!=0) {
    if DEE_UNLIKELY((iter->e_type = (DeeTypeObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) goto err_r_except_iter;
    if DEE_UNLIKELY(DeeError_TypeError_CheckType((DeeObject *)iter->e_type,&DeeType_Type) != 0) { Dee_DECREF(iter->e_type); goto err_r_except_iter; }
   } else iter->e_type = NULL;
  } while (++iter != end);
 } else result->co_exceptv = NULL;


 if DEE_UNLIKELY(DeeFile_ReadAll(infile,result->co_code,(Dee_size_t)header.cfh_size) != 0) {
  if (!result->co_exceptc) goto err_r_const;
  iter = result->co_exceptv+result->co_exceptc;
  goto err_r_except_iter;
 }
#if DEE_PLATFORM_ENDIAN != DEE_MARSHAL_ENDIAN_BYTECODE
 { // Swap endian on opargs
  Dee_uint8_t *iter,*end;
  end = (iter = DeeCode_CODE(result))+DeeCode_SIZE(result);
  while (iter != end) {
   if ((*iter++&OPFLAG_ARG)!=0) {
    *(Dee_uint16_t *)iter = DEE_BUILTIN_BSWAP16(*(Dee_uint16_t *)iter);
    iter += 2;
   }
  }
 }
#endif
 if (map->mrm_version <= DEE_MARSHAL_V101) {
  Dee_uint8_t *iter,*end;
  // Delete OP_DEBUG_FILE opcodes (no longer used)
  // NOTE: By doing this, we will loose filename debug information,
  //       but the user simply has to recompile to get them back.
  end = (iter = DeeCode_CODE(result))+DeeCode_SIZE(result);
  while (iter != end) {
   if (*iter == v101_OP_DEBUG_FILE) {
    // Just delete it, and its argument
    iter[0] = OP_NOOP;
    iter[1] = OP_NOOP;
    iter[2] = OP_NOOP;
   } else {
    if ((*iter++&OPFLAG_ARG)!=0) iter += 2;
   }
  }
 }

 // Calculate the w-buffer sizes
 DeeCode_CALCULATE_WBUFFERSIZE(result,(Dee_size_t)header.cfh_stackdata);
 DeeAtomicMutex_Init(&result->co_consts_lock);
 return result;
}

static int DEE_CALL _deecode_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeCodeObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 struct DeeExceptionHandlerEntry *iter,*end;
 struct _DeeCodeFileHeader_ header;
 header.cfh_flags = DEE_BUILTIN_LESWAP16(self->co_flags);
 if DEE_UNLIKELY(DeeFile_WriteAll(outfile,&header,sizeof(header)) != 0) return -1;
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)DeeCode_SIZE(self)) != 0) return -1;
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)self->co_nlocals) != 0) return -1;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)self->co_exceptsize) != 0) return -1;
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)self->co_exceptc) != 0) return -1;
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)self->co_finallyc) != 0) return -1;
#else
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)0) != 0) return -1;
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)0) != 0) return -1;
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)0) != 0) return -1;
#endif
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)self->co_stacksize) != 0) return -1;
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)DeeCode_SDATASIZE(self)) != 0) return -1;
 if DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,self->co_debug ? (DeeObject *)self->co_debug : Dee_None) != 0) return -1;
 if DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,DeeTuple_CheckEmpty(self->co_consts) ? Dee_None: (DeeObject *)self->co_consts) != 0) return -1;
 end = (iter = self->co_exceptv)+self->co_exceptc;
 while (iter != end) {
  struct _DeeExceptionHandlerEntryFileHeader_ hheader;
  hheader.ehefh_kind = (Dee_uint8_t)iter->e_kind;
  if DEE_UNLIKELY(DeeFile_WriteAll(outfile,&hheader,sizeof(hheader)) != 0) return -1;
  if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)iter->e_stack) != 0) return -1;
  if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)iter->e_handler) != 0) return -1;
  if ((iter->e_kind&DeeExceptionHandleKind_FLAG_VAR)!=0 && DEE_UNLIKELY(
   DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)iter->e_store) != 0)) return -1;
  if ((iter->e_kind&DeeExceptionHandleKind_FLAG_TYPED)!=0 && DEE_UNLIKELY(
   DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)iter->e_type) != 0)) return -1;
  ++iter;
 }
#if DEE_PLATFORM_ENDIAN == DEE_MARSHAL_ENDIAN_BYTECODE
 if DEE_UNLIKELY(DeeFile_WriteAll(outfile,DeeCode_CODE(self),DeeCode_SIZE(self)) != 0) return -1;
#else
 { // Write opargs with swapped endian
  Dee_uint8_t *iter,*last_flush,*end;
  end = (iter = last_flush = DeeCode_CODE(self))+DeeCode_SIZE(self);
  while (iter != end) {
   if ((*iter++&OPFLAG_ARG)!=0) {
    Dee_uint16_t swapped_arg;
    // Flush unwritten data
    if DEE_UNLIKELY(DeeFile_WriteAll(outfile,last_flush,(Dee_size_t)(iter-last_flush)) != 0) return -1;
    swapped_arg = DEE_BUILTIN_BSWAP16(*(Dee_uint16_t *)iter);
    if DEE_UNLIKELY(DeeFile_WriteAll(outfile,&swapped_arg,2) != 0) return -1;
    last_flush = (iter += 2);
   }
  }
  if DEE_UNLIKELY(last_flush != end && DeeFile_WriteAll(
   outfile,last_flush,(Dee_size_t)(end-last_flush)) != 0) return -1;
 }
#endif

 return 0;
}
DEE_COMPILER_MSVC_WARNING_POP

static struct DeeTypeMarshal _deecode_tp_marshal = DEE_TYPE_MARSHAL_VAR_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(code)),
 member(&_deecode_tp_marshal_new),member(&_deecode_tp_marshal_put));


DeeTypeObject DeeCode_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("code"),null,member(
  DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_VAR_OBJECT|
  DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_VAR_CONSTRUCTOR_v101(null,
  member(&_deecode_tp_copy_new),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deecode_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deecode_tp_str),
  member(&_deecode_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deecode_tp_visit),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,
  member(&_deecode_tp_cmp_eq),
  member(&_deecode_tp_cmp_ne),null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deecode_tp_members),member(_deecode_tp_getsets),member(_deecode_tp_methods),
  member(_deecode_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deecode_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};

DeeCodeObject _Dee_EmptyCode = {
 DEE_OBJECT_HEAD_INIT(&DeeCode_Type),
 NULL,                             // co_debug
 DeeAtomicMutex_INIT(),
 (DeeTupleObject *)Dee_EmptyTuple, // co_consts
 DEE_CODE_FLAG_NONE,               // co_flags
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 NULL,                             // co_exceptv
 0,                                // co_exceptc
 0,                                // co_finallyc
 0,                                // co_exceptsize
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 0,                                // co_nlocals
 0,                                // co_stacksize
 1,                                // co_wsize
 0,                                // co_woff_loc
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 0,                                // co_woff_except
 0,                                // co_woff_finally
#endif
 0,                                // co_woff_sdata
 1,                                // co_size
 {OP_RETNONE}                      // co_code (just return 'none')
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_CODE_C */
