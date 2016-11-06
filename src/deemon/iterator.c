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
#ifndef GUARD_DEEMON_ITERATOR_C
#define GUARD_DEEMON_ITERATOR_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/bool.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/generic_vtable.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_visit.h>
#include <deemon/integer.h>
#include <deemon/iterator.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>

// /src/*
#include <deemon/marshal_data.h>

// */ (nano...)

DEE_DECL_BEGIN

static struct DeeMethodDef const _deegenericiterable_tp_methods[] = {
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


DEE_A_RET_OBJECT_EXCEPT_REF(DeeGenericRangeIteratorObject) *DeeGenericRangeIterator_New(
 DEE_A_INOUT DeeObject *low, DEE_A_INOUT DeeObject *high, DEE_A_IN_OPT DeeObject *step) {
 DeeGenericRangeIteratorObject *result;
 if ((low = DeeObject_Copy(low)) == NULL) return NULL;
 if ((result = DeeObject_MALLOCF(DeeGenericRangeIteratorObject,"generic_range_iterator (%p - %p @ %p)",low,high,step)) != NULL) {
  DeeObject_INIT(result,&DeeGenericRangeIterator_Type);
  Dee_INHERIT_REF(result->ri_iter,low);
  Dee_INCREF(result->ri_end = high);
  Dee_XINCREF(result->ri_step = step);
 } else Dee_DECREF(low);
 return (DeeObject *)result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeGenericSequenceIteratorObject) *
DeeGenericSequenceIterator_New(DEE_A_INOUT DeeObject *seq) {
 DeeGenericSequenceIteratorObject *result; Dee_size_t seqsize;
 DEE_ASSERT(DeeObject_Check(seq));
 if (DeeObject_Size(seq,&seqsize) != 0) return NULL;
 if ((result = DeeObject_MALLOCF(DeeGenericSequenceIteratorObject,
  "generic_sequence_iterator(%p,%lu)",seq,(unsigned long)seqsize)) != NULL) {
  DeeObject_INIT(result,&DeeGenericSequenceIterator_Type);
  Dee_INCREF(result->si_seq = seq);
  result->si_pos = 0;
  result->si_size = seqsize;
 }
 return (DeeObject *)result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeSubRangeIteratorObject) *DeeSubRangeIterator_New(
 DEE_A_INOUT DeeObject *iter, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) {
 DeeSubRangeIteratorObject *result;
 DEE_ASSERT(DeeObject_Check(iter));
 if ((result = DeeObject_MALLOCF(DeeSubRangeIteratorObject,
  "subrange_iterator(%p,%lu,%lu)",iter,(unsigned long)begin,(unsigned long)end)) != NULL) {
  DeeObject_INIT(result,&DeeSubRangeIterator_Type);
  result->sri_begin = begin;
  result->sri_end = end;
  result->sri_pos = 0;
  Dee_INCREF(result->sri_iter = iter);
 }
 return (DeeObject *)result;
}




//////////////////////////////////////////////////////////////////////////
// GenericRangeIterator VTable
static void DEE_CALL _deegenericrangeiterator_tp_dtor(DeeGenericRangeIteratorObject *self) {
 Dee_DECREF(self->ri_iter);
 Dee_DECREF(self->ri_end);
 Dee_XDECREF(self->ri_step);
}
DEE_VISIT_PROC(_deegenericrangeiterator_tp_visit,DeeGenericRangeIteratorObject *self) {
 Dee_VISIT(self->ri_iter);
 Dee_VISIT(self->ri_end);
 Dee_XVISIT(self->ri_step);
}
static int DEE_CALL _deegenericrangeiterator_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeGenericRangeIteratorObject *self, DeeGenericRangeIteratorObject *right) {
 if ((self->ri_iter = DeeObject_Copy(right->ri_iter)) == NULL) return -1;
 Dee_INCREF(self->ri_end = right->ri_end);
 Dee_XINCREF(self->ri_step = right->ri_step);
 return 0;
}
static int DEE_CALL _deegenericrangeiterator_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeGenericRangeIteratorObject *self, DeeGenericRangeIteratorObject *right) {
 Dee_INCREF(self->ri_iter = right->ri_iter);
 Dee_INCREF(self->ri_end = right->ri_end);
 self->ri_step = right->ri_step;
 right->ri_step = NULL;
 return 0;
}
static int DEE_CALL _deegenericrangeiterator_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeGenericRangeIteratorObject *self, DeeObject *args) {
 self->ri_step = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo|o:generic_range_iterator",
  &self->ri_iter,&self->ri_end,&self->ri_step) != 0) return -1;
 if DEE_UNLIKELY((self->ri_iter = DeeObject_Copy(self->ri_iter)) == NULL) return -1;
 Dee_INCREF(self->ri_end);
 Dee_XINCREF(self->ri_step);
 return 0;
}
static int DEE_CALL _deegenericrangeiterator_tp_bool(DeeGenericRangeIteratorObject *self) {
 return DeeObject_CompareLo(self->ri_iter,self->ri_end);
}
static DeeObject *DEE_CALL _deegenericrangeiterator_tp_not(DeeGenericRangeIteratorObject *self) {
 return DeeObject_CompareGeObject(self->ri_iter,self->ri_end);
}
static DeeObject *DEE_CALL _deegenericrangeiterator_tp_seq_contains(
 DeeGenericRangeIteratorObject *self, DeeObject *ob) {
 int temp;
 if DEE_UNLIKELY((temp = DeeObject_CompareGe(ob,self->ri_iter)) < 0) return NULL;
 if (!temp) DeeReturn_False;
 return DeeObject_CompareLoObject(ob,self->ri_end);
}
static DeeObject *DEE_CALL _deegenericrangeiterator_tp_str(DeeGenericRangeIteratorObject *self) {
 return self->ri_step
  ? DeeString_Newf("[%k...%k]",self->ri_iter,self->ri_end)
  : DeeString_Newf("[%k...%k / %k]",self->ri_iter,self->ri_end,self->ri_step);
}
static DeeObject *DEE_CALL _deegenericrangeiterator_tp_seq_size(DeeGenericRangeIteratorObject *self) {
 int temp; DeeObject *result,*newresult;
 if DEE_UNLIKELY((temp = DeeObject_CompareLo(self->ri_iter,self->ri_end)) < 0) return NULL;
 if (!temp) return DeeObject_Sub(self->ri_iter,self->ri_iter);
 result = DeeObject_Sub(self->ri_end,self->ri_iter);
 if (result && self->ri_step) {
  newresult = DeeObject_Div(result,self->ri_step);
  Dee_DECREF(result);
  Dee_INHERIT_REF(result,newresult);
 }
 return result;
}
static int DEE_CALL _deegenericrangeiterator_tp_seq_iter_next(
 DeeGenericRangeIteratorObject *self, DeeObject **result) {
 DeeObject *temp2; int temp;
 if DEE_UNLIKELY((temp = DeeObject_CompareLo(self->ri_iter,self->ri_end)) < 0) return temp;
 if DEE_UNLIKELY(!temp) return 1; // Iterator end
 if DEE_UNLIKELY((*result = DeeObject_Copy(self->ri_iter)) == NULL) return -1;
 if DEE_LIKELY((temp2 = self->ri_step
  ? DeeObject_InplaceAdd(self->ri_iter,self->ri_step)
  : DeeObject_Inc(self->ri_iter)) == NULL) {
  Dee_DECREF(*result);
  return -1;
 }
 Dee_DECREF(temp2);
 return 0;
}
static DeeObject *DEE_CALL _deegenericrangeiterator_tp_inc(
 DeeGenericRangeIteratorObject *self) {
 DeeObject *ob; int temp;
 if DEE_UNLIKELY((temp = DeeObject_CompareLo(self->ri_iter,self->ri_end)) < 0) return NULL;
 if DEE_LIKELY(temp) {
  if DEE_UNLIKELY((ob = self->ri_step
   ? DeeObject_InplaceAdd(self->ri_iter,self->ri_step)
   : DeeObject_Inc(self->ri_iter)) == NULL) return NULL;
  Dee_DECREF(ob);
 }
 DeeReturn_NEWREF(self);
}
static DeeObject *DEE_CALL _deegenericrangeiterator_tp_dec(
 DeeGenericRangeIteratorObject *self) {
 DeeObject *ob;
 if DEE_UNLIKELY((ob = self->ri_step
  ? DeeObject_InplaceSub(self->ri_iter,self->ri_step)
  : DeeObject_Dec(self->ri_iter)) == NULL) return NULL;
 Dee_DECREF(ob);
 DeeReturn_NEWREF(self);
}
static DeeObject *DEE_CALL _deegenericrangeiterator_tp_inc_post(
 DeeGenericRangeIteratorObject *self) {
 DeeObject *result,*ob; int temp;
 if ((temp = DeeObject_CompareLo(self->ri_iter,self->ri_end)) < 0) return NULL;
 if (temp) {
  if ((result = DeeObject_Copy((DeeObject *)self)) == NULL) return NULL;
  ob = self->ri_step
   ? DeeObject_InplaceAdd(self->ri_iter,self->ri_step)
   : DeeObject_Inc(self->ri_iter);
  if (!ob) { Dee_DECREF(result); return NULL; }
  Dee_DECREF(ob);
  return result;
 }
 DeeReturn_NEWREF(self);
}
static DeeObject *DEE_CALL _deegenericrangeiterator_tp_dec_post(
 DeeGenericRangeIteratorObject *self) {
 DeeObject *result,*ob;
 if ((result = DeeObject_Copy((DeeObject *)self)) == NULL) return NULL;
 ob = self->ri_step
  ? DeeObject_InplaceSub(self->ri_iter,self->ri_step)
  : DeeObject_Dec(self->ri_iter);
 if (!ob) { Dee_DECREF(result); return NULL; }
 Dee_DECREF(ob);
 return result;
}
static int DEE_CALL _deegenericrangeiterator_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeGenericRangeIteratorObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_uint8_t has_step;
 if (DeeFile_Getc(infile,&has_step) != 0) return -1;
 if ((self->ri_iter = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if ((self->ri_end = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {err_iter: Dee_DECREF(self->ri_iter); return -1; }
 if (!has_step) self->ri_step = NULL;
 else if ((self->ri_step = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) { Dee_DECREF(self->ri_end); goto err_iter; }
 return 0;
}
static int DEE_CALL _deegenericrangeiterator_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeGenericRangeIteratorObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 if (DeeFile_Putc(outfile,(Dee_uint8_t)(self->ri_step ? 1 : 0)) != 0) return -1;
 if (DeeMarshal_WriteObjectWithMap(outfile,map,self->ri_iter) != 0) return -1;
 return DeeMarshal_WriteObjectWithMap(outfile,map,self->ri_end);
}
static struct DeeTypeMarshal _deegenericrangeiterator_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(generic_range_iterator)),
 member(&_deegenericrangeiterator_tp_marshal_ctor),
 member(&_deegenericrangeiterator_tp_marshal_put));



//////////////////////////////////////////////////////////////////////////
// GenericSequenceIterator VTable
static int DEE_CALL _deegenericsequenceiterator_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeGenericSequenceIteratorObject *self,
 DeeGenericSequenceIteratorObject *right) {
 Dee_INCREF(self->si_seq = right->si_seq);
 self->si_pos = (Dee_size_t)DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,right->si_pos,memory_order_seq_cst);
 self->si_size = right->si_size;
 return 0;
}
static int DEE_CALL _deegenericsequenceiterator_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeGenericSequenceIteratorObject *self, DeeObject *args) {
 self->si_pos = 0;
 if (DeeTuple_Unpack(args,"o|Iu:generic_sequence_iterator",&self->si_seq,&self->si_pos) != 0) return -1;
 if (DeeObject_Size(self->si_seq,&self->si_size) != 0) return -1;
 Dee_INCREF(self->si_seq);
 return 0;
}
static void DEE_CALL _deegenericsequenceiterator_tp_dtor(DeeGenericSequenceIteratorObject *self) {
 Dee_DECREF(self->si_seq);
}
DEE_VISIT_PROC(_deegenericsequenceiterator_tp_visit,DeeGenericSequenceIteratorObject *self) {
 Dee_VISIT(self->si_seq);
}
static DeeObject *DEE_CALL _deegenericsequenceiterator_tp_str(DeeGenericSequenceIteratorObject *self) {
 return DeeString_Newf("generic_sequence_iterator(%Iu..%Iu)",
                       DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,self->si_pos,memory_order_seq_cst),
                       self->si_size);
}
static DeeObject *DEE_CALL _deegenericsequenceiterator_tp_repr(DeeGenericSequenceIteratorObject *self) {
 return DeeString_Newf("generic_sequence_iterator(%Iu..%Iu,%k)",
                       DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,self->si_pos,memory_order_seq_cst),
                       self->si_size,self->si_seq);
}
static int DEE_CALL _deegenericsequenceiterator_tp_bool(DeeGenericSequenceIteratorObject *self) {
 return (Dee_size_t)DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,self->si_pos,memory_order_seq_cst) < self->si_size;
}
static DeeObject *DEE_CALL _deegenericsequenceiterator_tp_not(DeeGenericSequenceIteratorObject *self) {
 DeeReturn_Bool((Dee_size_t)DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,self->si_pos,memory_order_seq_cst) >= self->si_size);
}
static int DEE_CALL _deegenericsequenceiterator_tp_seq_iter_next(
 DeeGenericSequenceIteratorObject *self, DeeObject **result) {
 Dee_size_t myindex = (Dee_size_t)DeeAtomicN_FetchInc(
  DEE_TYPES_SIZEOF_SIZE_T,self->si_pos,memory_order_seq_cst);
 if (myindex >= self->si_size) return 1;
 return (*result = DeeObject_GetIndex(self->si_seq,myindex)) != NULL ? 0 : -1;
}
static DeeObject *DEE_CALL _deegenericsequenceiterator_tp_cmp_lo(
 DeeGenericSequenceIteratorObject *lhs, DeeGenericSequenceIteratorObject *rhs) {
 int temp;
 if (DeeObject_InplaceGetInstance(&rhs,&DeeGenericSequenceIterator_Type) != 0) return NULL;
 if ((temp = DeeObject_CompareLo(lhs->si_seq,rhs->si_seq)) < 0) return NULL;
 if (temp) DeeReturn_True;
 if ((temp = DeeObject_CompareEq(lhs->si_seq,rhs->si_seq)) < 0) return NULL;
 if (!temp) DeeReturn_False;
 DeeReturn_Bool(DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,lhs->si_pos,memory_order_seq_cst) <
                DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,rhs->si_pos,memory_order_seq_cst));
}
static DeeObject *DEE_CALL _deegenericsequenceiterator_tp_cmp_le(
 DeeGenericSequenceIteratorObject *lhs, DeeGenericSequenceIteratorObject *rhs) {
 int temp;
 if (DeeObject_InplaceGetInstance(&rhs,&DeeGenericSequenceIterator_Type) != 0) return NULL;
 if ((temp = DeeObject_CompareLo(lhs->si_seq,rhs->si_seq)) < 0) return NULL;
 if (temp) DeeReturn_True;
 if ((temp = DeeObject_CompareEq(lhs->si_seq,rhs->si_seq)) < 0) return NULL;
 if (!temp) DeeReturn_False;
 DeeReturn_Bool(DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,lhs->si_pos,memory_order_seq_cst) <=
                DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,rhs->si_pos,memory_order_seq_cst));
}
static DeeObject *DEE_CALL _deegenericsequenceiterator_tp_cmp_eq(
 DeeGenericSequenceIteratorObject *lhs, DeeGenericSequenceIteratorObject *rhs) {
 int temp;
 if (DeeObject_InplaceGetInstance(&rhs,&DeeGenericSequenceIterator_Type) != 0) return NULL;
 if ((temp = DeeObject_CompareEq(lhs->si_seq,rhs->si_seq)) < 0) return NULL;
 if (!temp) DeeReturn_False;
 DeeReturn_Bool(DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,lhs->si_pos,memory_order_seq_cst) ==
                DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,rhs->si_pos,memory_order_seq_cst));
}
static DeeObject *DEE_CALL _deegenericsequenceiterator_tp_cmp_ne(
 DeeGenericSequenceIteratorObject *lhs, DeeGenericSequenceIteratorObject *rhs) {
 int temp;
 if (DeeObject_InplaceGetInstance(&rhs,&DeeGenericSequenceIterator_Type) != 0) return NULL;
 if ((temp = DeeObject_CompareNe(lhs->si_seq,rhs->si_seq)) < 0) return NULL;
 if (!temp) DeeReturn_False;
 DeeReturn_Bool(DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,lhs->si_pos,memory_order_seq_cst) !=
                DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,rhs->si_pos,memory_order_seq_cst));
}
static DeeObject *DEE_CALL _deegenericsequenceiterator_tp_cmp_gr(
 DeeGenericSequenceIteratorObject *lhs, DeeGenericSequenceIteratorObject *rhs) {
 int temp;
 if (DeeObject_InplaceGetInstance(&rhs,&DeeGenericSequenceIterator_Type) != 0) return NULL;
 if ((temp = DeeObject_CompareGr(lhs->si_seq,rhs->si_seq)) < 0) return NULL;
 if (temp) DeeReturn_True;
 if ((temp = DeeObject_CompareEq(lhs->si_seq,rhs->si_seq)) < 0) return NULL;
 if (!temp) DeeReturn_False;
 DeeReturn_Bool(DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,lhs->si_pos,memory_order_seq_cst) >
                DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,rhs->si_pos,memory_order_seq_cst));
}
static DeeObject *DEE_CALL _deegenericsequenceiterator_tp_cmp_ge(
 DeeGenericSequenceIteratorObject *lhs, DeeGenericSequenceIteratorObject *rhs) {
 int temp;
 if (DeeObject_InplaceGetInstance(&rhs,&DeeGenericSequenceIterator_Type) != 0) return NULL;
 if ((temp = DeeObject_CompareGr(lhs->si_seq,rhs->si_seq)) < 0) return NULL;
 if (temp) DeeReturn_True;
 if ((temp = DeeObject_CompareEq(lhs->si_seq,rhs->si_seq)) < 0) return NULL;
 if (!temp) DeeReturn_False;
 DeeReturn_Bool(DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,lhs->si_pos,memory_order_seq_cst) >=
                DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,rhs->si_pos,memory_order_seq_cst));
}
static int DEE_CALL _deegenericsequenceiterator_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeGenericSequenceIteratorObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_uint64_t read_pos,read_size;
 if (DeeFile_GetLeSmall64(infile,&read_pos) != 0) return -1;
 if (DeeFile_GetLeSmall64(infile,&read_size) != 0) return -1;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_pos > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "generic_sequence_iterator position is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_pos,(Dee_size_t)-1);
  return -1;
 }
 if DEE_UNLIKELY(read_size > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "generic_sequence_iterator sequence size is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_size,(Dee_size_t)-1);
  return -1;
 }
#endif
 self->si_pos = (Dee_size_t)read_pos;
 self->si_size = (Dee_size_t)read_size;
 if ((self->si_seq = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 return 0;
}
static int DEE_CALL _deegenericsequenceiterator_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeGenericSequenceIteratorObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 Dee_size_t used_pos;
 used_pos = (Dee_size_t)DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,self->si_pos,memory_order_seq_cst);
 if (DeeFile_PutLeSmall64(outfile,used_pos) != 0) return -1;
 if (DeeFile_PutLeSmall64(outfile,self->si_size) != 0) return -1;
 return DeeMarshal_WriteObjectWithMap(outfile,map,self->si_seq);
}
static struct DeeTypeMarshal _deegenericsequenceiterator_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(generic_sequence_iterator)),
 member(&_deegenericsequenceiterator_tp_marshal_ctor),
 member(&_deegenericsequenceiterator_tp_marshal_put));


//////////////////////////////////////////////////////////////////////////
// SubRangeIterator VTable
static int DEE_CALL _deesubrangeiterator_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSubRangeIteratorObject *self,
 DeeSubRangeIteratorObject *right) {
 if ((self->sri_iter = DeeObject_Copy(right->sri_iter)) == NULL) return -1;
 self->sri_pos = (Dee_size_t)DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,right->sri_pos,memory_order_seq_cst);
 self->sri_begin = right->sri_begin;
 self->sri_end = right->sri_end;
 return 0;
}
static int DEE_CALL _deesubrangeiterator_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeSubRangeIteratorObject *self, DeeObject *args) {
 self->sri_begin = 0;
 self->sri_end = (Dee_size_t)-1;
 if (DeeTuple_Unpack(args,"o|IuIu:subrange_iterator",
  &self->sri_iter,&self->sri_begin,&self->sri_end) != 0) return -1;
 Dee_INCREF(self->sri_iter);
 self->sri_pos = 0;
 return 0;
}
void DEE_CALL _deesubrangeiterator_tp_dtor(DeeGenericSequenceIteratorObject *self) {
 Dee_DECREF(self->si_seq);
}
DEE_VISIT_PROC(_deesubrangeiterator_tp_visit,DeeGenericSequenceIteratorObject *self) {
 Dee_VISIT(self->si_seq);
}
static DeeObject *DEE_CALL _deesubrangeiterator_tp_str(DeeSubRangeIteratorObject *self) {
 return DeeString_Newf("subrange_iterator(%Iu,%Iu..%Iu)",
                       DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,self->sri_pos,memory_order_seq_cst),
                       self->sri_begin,self->sri_end);
}
static DeeObject *DEE_CALL _deesubrangeiterator_tp_repr(DeeSubRangeIteratorObject *self) {
 return DeeString_Newf("subrange_iterator(%Iu,%Iu..%Iu,%k)",
                       DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,self->sri_pos,memory_order_seq_cst),
                       self->sri_begin,self->sri_end,self->sri_iter);
}
static int DEE_CALL _deesubrangeiterator_tp_bool(DeeSubRangeIteratorObject *self) {
 return (Dee_size_t)DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,self->sri_pos,memory_order_seq_cst) < self->sri_end;
}
static DeeObject *DEE_CALL _deesubrangeiterator_tp_not(DeeSubRangeIteratorObject *self) {
 DeeReturn_Bool((Dee_size_t)DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,self->sri_pos,memory_order_seq_cst) >= self->sri_end);
}
static int DEE_CALL _deesubrangeiterator_tp_seq_iter_next(
 DeeSubRangeIteratorObject *self, DeeObject **result) {
 int error; Dee_size_t myindex; while (1) {
  myindex = (Dee_size_t)DeeAtomicN_FetchInc(DEE_TYPES_SIZEOF_SIZE_T,self->sri_pos,memory_order_seq_cst);
  if (myindex >= self->sri_end) return 1;
  if ((error = DeeObject_IterNextEx(self->sri_iter,result)) != 0) return error;
  if (myindex >= self->sri_begin) break;
  Dee_DECREF(*result);
 }
 return 0;
}

static DeeObject *DEE_CALL _deesubrangeiterator_tp_cmp_lo(
 DeeSubRangeIteratorObject *lhs, DeeSubRangeIteratorObject *rhs) {
 int temp;
 if (DeeObject_InplaceGetInstance(&rhs,&DeeSubRangeIterator_Type) != 0) return NULL;
 if ((temp = DeeObject_CompareLo(lhs->sri_iter,rhs->sri_iter)) < 0) return NULL;
 if (temp) DeeReturn_True;
 if ((temp = DeeObject_CompareEq(lhs->sri_iter,rhs->sri_iter)) < 0) return NULL;
 if (!temp) DeeReturn_False;
 DeeReturn_Bool(DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,lhs->sri_pos,memory_order_seq_cst) <
                DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,rhs->sri_pos,memory_order_seq_cst));
}
static DeeObject *DEE_CALL _deesubrangeiterator_tp_cmp_le(
 DeeSubRangeIteratorObject *lhs, DeeSubRangeIteratorObject *rhs) {
 int temp;
 if (DeeObject_InplaceGetInstance(&rhs,&DeeSubRangeIterator_Type) != 0) return NULL;
 if ((temp = DeeObject_CompareLo(lhs->sri_iter,rhs->sri_iter)) < 0) return NULL;
 if (temp) DeeReturn_True;
 if ((temp = DeeObject_CompareEq(lhs->sri_iter,rhs->sri_iter)) < 0) return NULL;
 if (!temp) DeeReturn_False;
 DeeReturn_Bool(DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,lhs->sri_pos,memory_order_seq_cst) <=
                DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,rhs->sri_pos,memory_order_seq_cst));
}
static DeeObject *DEE_CALL _deesubrangeiterator_tp_cmp_eq(
 DeeSubRangeIteratorObject *lhs, DeeSubRangeIteratorObject *rhs) {
 int temp;
 if (DeeObject_InplaceGetInstance(&rhs,&DeeSubRangeIterator_Type) != 0) return NULL;
 if ((temp = DeeObject_CompareEq(lhs->sri_iter,rhs->sri_iter)) < 0) return NULL;
 if (!temp) DeeReturn_False;
 DeeReturn_Bool(DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,lhs->sri_pos,memory_order_seq_cst) ==
                DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,rhs->sri_pos,memory_order_seq_cst));
}
static DeeObject *DEE_CALL _deesubrangeiterator_tp_cmp_ne(
 DeeSubRangeIteratorObject *lhs, DeeSubRangeIteratorObject *rhs) {
 int temp;
 if (DeeObject_InplaceGetInstance(&rhs,&DeeSubRangeIterator_Type) != 0) return NULL;
 if ((temp = DeeObject_CompareNe(lhs->sri_iter,rhs->sri_iter)) < 0) return NULL;
 if (!temp) DeeReturn_False;
 DeeReturn_Bool(DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,lhs->sri_pos,memory_order_seq_cst) !=
                DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,rhs->sri_pos,memory_order_seq_cst));
}
static DeeObject *DEE_CALL _deesubrangeiterator_tp_cmp_gr(
 DeeSubRangeIteratorObject *lhs, DeeSubRangeIteratorObject *rhs) {
 int temp;
 if (DeeObject_InplaceGetInstance(&rhs,&DeeSubRangeIterator_Type) != 0) return NULL;
 if ((temp = DeeObject_CompareGr(lhs->sri_iter,rhs->sri_iter)) < 0) return NULL;
 if (temp) DeeReturn_True;
 if ((temp = DeeObject_CompareEq(lhs->sri_iter,rhs->sri_iter)) < 0) return NULL;
 if (!temp) DeeReturn_False;
 DeeReturn_Bool(DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,lhs->sri_pos,memory_order_seq_cst) >
                DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,rhs->sri_pos,memory_order_seq_cst));
}
static DeeObject *DEE_CALL _deesubrangeiterator_tp_cmp_ge(
 DeeSubRangeIteratorObject *lhs, DeeSubRangeIteratorObject *rhs) {
 int temp;
 if (DeeObject_InplaceGetInstance(&rhs,&DeeSubRangeIterator_Type) != 0) return NULL;
 if ((temp = DeeObject_CompareGr(lhs->sri_iter,rhs->sri_iter)) < 0) return NULL;
 if (temp) DeeReturn_True;
 if ((temp = DeeObject_CompareEq(lhs->sri_iter,rhs->sri_iter)) < 0) return NULL;
 if (!temp) DeeReturn_False;
 DeeReturn_Bool(DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,lhs->sri_pos,memory_order_seq_cst) >=
                DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,rhs->sri_pos,memory_order_seq_cst));
}
static int DEE_CALL _deesubrangeiterator_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSubRangeIteratorObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_uint64_t read_begin,read_end,read_pos;
 if (DeeFile_GetLeSmall64(infile,&read_begin) != 0) return -1;
 if (DeeFile_GetLeSmall64(infile,&read_end) != 0) return -1;
 if (DeeFile_GetLeSmall64(infile,&read_pos) != 0) return -1;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_begin > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "subrange_iterator begin is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_begin,(Dee_size_t)-1);
  return -1;
 }
 if DEE_UNLIKELY(read_end > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "subrange_iterator end is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_end,(Dee_size_t)-1);
  return -1;
 }
 if DEE_UNLIKELY(read_pos > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "subrange_iterator position is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_pos,(Dee_size_t)-1);
  return -1;
 }
#endif
 self->sri_begin = (Dee_size_t)read_begin;
 self->sri_end = (Dee_size_t)read_end;
 self->sri_pos = (Dee_size_t)read_pos;
 if ((self->sri_iter = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 return 0;
}
static int DEE_CALL _deesubrangeiterator_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSubRangeIteratorObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 Dee_size_t used_pos;
 used_pos = (Dee_size_t)DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,self->sri_pos,memory_order_seq_cst);
 if (DeeFile_PutLeSmall64(outfile,self->sri_begin) != 0) return -1;
 if (DeeFile_PutLeSmall64(outfile,self->sri_end) != 0) return -1;
 if (DeeFile_PutLeSmall64(outfile,used_pos) != 0) return -1;
 return DeeMarshal_WriteObjectWithMap(outfile,map,self->sri_iter);
}
static struct DeeTypeMarshal _deesubrangeiterator_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(subrange_iterator)),
 member(&_deesubrangeiterator_tp_marshal_ctor),
 member(&_deesubrangeiterator_tp_marshal_put));







static struct DeeMemberDef const _deegenericrangeiterator_tp_members[] = {
 DEE_MEMBERDEF_RO_v100(DeeGenericRangeIteratorObject,ri_iter,object),
 DEE_MEMBERDEF_RO_v100(DeeGenericRangeIteratorObject,ri_end,object),
 DEE_MEMBERDEF_RO_v100(DeeGenericRangeIteratorObject,ri_step,object_null),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const _deegenericsequenceiterator_tp_members[] = {
 DEE_MEMBERDEF_RO_v100(DeeGenericSequenceIteratorObject,si_seq,object),
 DEE_MEMBERDEF_RO_v100(DeeGenericSequenceIteratorObject,si_size,Dee_size_t),
 DEE_MEMBERDEF_v100(DeeGenericSequenceIteratorObject,si_pos,atomic(Dee_size_t)),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const _deesubrangeiterator_tp_members[] = {
 DEE_MEMBERDEF_RO_v100(DeeSubRangeIteratorObject,sri_iter,object),
 DEE_MEMBERDEF_RO_v100(DeeSubRangeIteratorObject,sri_begin,Dee_size_t),
 DEE_MEMBERDEF_RO_v100(DeeSubRangeIteratorObject,sri_end,Dee_size_t),
 DEE_MEMBERDEF_v100(DeeSubRangeIteratorObject,sri_pos,atomic(Dee_size_t)),
 DEE_MEMBERDEF_END_v100
};


DeeTypeObject DeeGenericRangeIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("generic_range_iterator"),member(
  "(object begin, object end) -> generic_range_iterator\n"
  "(object begin, object end, object step) -> generic_range_iterator"),
  null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeGenericRangeIteratorObject),null,null,
  member(&_deegenericrangeiterator_tp_copy_ctor),
  member(&_deegenericrangeiterator_tp_move_ctor),
  member(&_deegenericrangeiterator_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deegenericrangeiterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deegenericrangeiterator_tp_str),
  null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deegenericrangeiterator_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deegenericrangeiterator_tp_not),
  member(&_deegenericrangeiterator_tp_bool),null,null,null,
  member(&_deegenericrangeiterator_tp_inc),
  member(&_deegenericrangeiterator_tp_inc_post),
  member(&_deegenericrangeiterator_tp_dec),
  member(&_deegenericrangeiterator_tp_dec_post),
  null,null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deegenericiterable_tp_cmp_lo), // TODO: Dedicated compare function
  member(&_deegenericiterable_tp_cmp_le),
  member(&_deegenericiterable_tp_cmp_eq),
  member(&_deegenericiterable_tp_cmp_ne),
  member(&_deegenericiterable_tp_cmp_gr),
  member(&_deegenericiterable_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,
  member(&_deegenericrangeiterator_tp_seq_size),
  member(&_deegenericrangeiterator_tp_seq_contains),
  null,null,null,member(&DeeObject_Copy),
  member(&_deegenericrangeiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deegenericrangeiterator_tp_members),null,
  member(_deegenericiterable_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deegenericrangeiterator_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};

DeeTypeObject DeeGenericSequenceIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("generic_sequence_iterator"),member(
  "(object seq) -> generic_sequence_iterator"),null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeGenericSequenceIteratorObject),null,null,
  member(&_deegenericsequenceiterator_tp_copy_ctor),null,
  member(&_deegenericsequenceiterator_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deegenericsequenceiterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deegenericsequenceiterator_tp_str),
  member(&_deegenericsequenceiterator_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deegenericsequenceiterator_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deegenericsequenceiterator_tp_not),
  member(&_deegenericsequenceiterator_tp_bool),
  null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deegenericsequenceiterator_tp_cmp_lo),
  member(&_deegenericsequenceiterator_tp_cmp_le),
  member(&_deegenericsequenceiterator_tp_cmp_eq),
  member(&_deegenericsequenceiterator_tp_cmp_ne),
  member(&_deegenericsequenceiterator_tp_cmp_gr),
  member(&_deegenericsequenceiterator_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  null,null,null,null,null,null,null,null,member(&DeeObject_Copy),
  member(&_deegenericsequenceiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deegenericsequenceiterator_tp_members),null,
  member(_deegenericiterable_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deegenericsequenceiterator_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};

DeeTypeObject DeeSubRangeIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("subrange_iterator"),member(
  "(object iter, size_t begin = 0, size_t end = size_t(-1)) -> subrange_iterator"),null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeSubRangeIteratorObject),null,null,
  member(&_deesubrangeiterator_tp_copy_ctor),null,
  member(&_deesubrangeiterator_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deesubrangeiterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deesubrangeiterator_tp_str),
  member(&_deesubrangeiterator_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deesubrangeiterator_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deesubrangeiterator_tp_not),
  member(&_deesubrangeiterator_tp_bool),
  null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deesubrangeiterator_tp_cmp_lo),
  member(&_deesubrangeiterator_tp_cmp_le),
  member(&_deesubrangeiterator_tp_cmp_eq),
  member(&_deesubrangeiterator_tp_cmp_ne),
  member(&_deesubrangeiterator_tp_cmp_gr),
  member(&_deesubrangeiterator_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  null,null,null,null,null,null,null,null,member(&DeeObject_Copy),
  member(&_deesubrangeiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deesubrangeiterator_tp_members),null,
  member(_deegenericiterable_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deesubrangeiterator_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeEnumIteratorObject) *DeeEnumIterator_NewEx(
 DEE_A_IN Dee_size_t start_index, DEE_A_INOUT DeeObject *iterator) {
 DeeEnumIteratorObject *result;
 DEE_ASSERT(DeeObject_Check(iterator));
 if DEE_LIKELY((result = DeeObject_MALLOC(DeeEnumIteratorObject)) != NULL) {
  DeeObject_INIT(result,&DeeEnumIterator_Type);
  result->ei_index = start_index;
  Dee_INCREF(result->ei_iterator = iterator);
 }
 return (DeeObject *)result;
}

static int DEE_CALL _deeenumiterator_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeEnumIteratorObject *self,
 DeeEnumIteratorObject *right) {
 self->ei_index = (Dee_size_t)DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,right->ei_index,memory_order_seq_cst);
 Dee_INCREF(self->ei_iterator = right->ei_iterator);
 return 0;
}
static int DEE_CALL _deeenumiterator_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeEnumIteratorObject *self, DeeObject *args) {
 DeeObject *index,*iter = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:enumiterator",&index,&iter) != 0) return -1;
 if (iter) {
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,index,&self->ei_index) != 0) return -1;
  Dee_INCREF(self->ei_iterator = iter);
 } else {
  self->ei_index = 0;
  Dee_INCREF(self->ei_iterator = index);
 }
 return 0;
}

static void DEE_CALL _deeenumiterator_tp_dtor(DeeEnumIteratorObject *self) { Dee_DECREF(self->ei_iterator); }
DEE_VISIT_PROC(_deeenumiterator_tp_visit,DeeEnumIteratorObject *self) { Dee_VISIT(self->ei_iterator); }
static DeeObject *_deeenumiterator_tp_str(DeeEnumIteratorObject *self) {
 return DeeString_Newf("<enum_iterator -> %Iu,%k>",
                       (Dee_size_t)DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,self->ei_index,memory_order_seq_cst),
                       self->ei_iterator);
}
static DeeObject *DEE_CALL _deeenumiterator_tp_repr(DeeEnumIteratorObject *self) {
 return DeeString_Newf("enum_iterator(%Iu,%r)",
                       (Dee_size_t)DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,self->ei_index,memory_order_seq_cst),
                       self->ei_iterator);
}
static int DEE_CALL _deeenumiterator_tp_bool(DeeEnumIteratorObject *self) { return DeeObject_Bool(self->ei_iterator); }
static DeeObject *DEE_CALL _deeenumiterator_tp_not(DeeEnumIteratorObject *self) { return DeeObject_Not(self->ei_iterator); }
static DeeObject *DEE_CALL _deeenumiterator_tp_cmp_lo(DeeEnumIteratorObject *self, DeeEnumIteratorObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeEnumIterator_Type) != 0) return NULL; return DeeObject_CompareLoObject(self->ei_iterator,right->ei_iterator); }
static DeeObject *DEE_CALL _deeenumiterator_tp_cmp_le(DeeEnumIteratorObject *self, DeeEnumIteratorObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeEnumIterator_Type) != 0) return NULL; return DeeObject_CompareLeObject(self->ei_iterator,right->ei_iterator); }
static DeeObject *DEE_CALL _deeenumiterator_tp_cmp_eq(DeeEnumIteratorObject *self, DeeEnumIteratorObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeEnumIterator_Type) != 0) return NULL; return DeeObject_CompareEqObject(self->ei_iterator,right->ei_iterator); }
static DeeObject *DEE_CALL _deeenumiterator_tp_cmp_ne(DeeEnumIteratorObject *self, DeeEnumIteratorObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeEnumIterator_Type) != 0) return NULL; return DeeObject_CompareNeObject(self->ei_iterator,right->ei_iterator); }
static DeeObject *DEE_CALL _deeenumiterator_tp_cmp_gr(DeeEnumIteratorObject *self, DeeEnumIteratorObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeEnumIterator_Type) != 0) return NULL; return DeeObject_CompareGrObject(self->ei_iterator,right->ei_iterator); }
static DeeObject *DEE_CALL _deeenumiterator_tp_cmp_ge(DeeEnumIteratorObject *self, DeeEnumIteratorObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeEnumIterator_Type) != 0) return NULL; return DeeObject_CompareGeObject(self->ei_iterator,right->ei_iterator); }

static int DEE_CALL _deeenumiterator_tp_seq_iter_next(DeeEnumIteratorObject *self, DeeObject **result) {
 Dee_size_t result_index; int error; DeeObject *index_object,*result_elem;
 result_index = (Dee_size_t)DeeAtomicN_FetchInc(DEE_TYPES_SIZEOF_SIZE_T,self->ei_index,memory_order_seq_cst);
 if DEE_UNLIKELY((error = DeeObject_IterNextEx(self->ei_iterator,&result_elem)) != 0) return error; // Error or iterator end
 if DEE_UNLIKELY((index_object = DeeObject_New(Dee_size_t,result_index)) == NULL) { Dee_DECREF(result_elem); return -1; }
 *result = DeeTuple_Pack(2,index_object,result_elem);
 Dee_DECREF(index_object);
 Dee_DECREF(result_elem);
 return DEE_LIKELY(*result) ? 0 : -1;
}

static int DEE_CALL _deeenumiterator_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeEnumIteratorObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_uint64_t read_index;
 if (DeeFile_GetLeSmall64(infile,&read_index) != 0) return -1;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_index > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "enum_iterator index is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_index,(Dee_size_t)-1);
  return -1;
 }
#endif
 self->ei_index = (Dee_size_t)read_index;
 if ((self->ei_iterator = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 return 0;
}
static int DEE_CALL _deeenumiterator_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeEnumIteratorObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 Dee_size_t used_pos;
 used_pos = (Dee_size_t)DeeAtomicN_Load(DEE_TYPES_SIZEOF_SIZE_T,self->ei_index,memory_order_seq_cst);
 if (DeeFile_PutLeSmall64(outfile,used_pos) != 0) return -1;
 return DeeMarshal_WriteObjectWithMap(outfile,map,self->ei_iterator);
}
static struct DeeTypeMarshal _deeenumiterator_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(enumiterator)),
 member(&_deeenumiterator_tp_marshal_ctor),
 member(&_deeenumiterator_tp_marshal_put));
static struct DeeMemberDef const _deeenumiterator_tp_members[] = {
 DEE_MEMBERDEF_RO_v100(DeeEnumIteratorObject,ei_index,atomic(Dee_size_t)),
 DEE_MEMBERDEF_RO_v100(DeeEnumIteratorObject,ei_iterator,object),
 DEE_MEMBERDEF_END_v100
};


DeeTypeObject DeeEnumIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("enumiterator"),member(
  "(object iterator) -> enumiterator\n"
  "(size_t start_index, object iterator) -> enumiterator"),null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeEnumIteratorObject),null,null,
  member(&_deeenumiterator_tp_copy_ctor),null,
  member(&_deeenumiterator_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deeenumiterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deeenumiterator_tp_str),
  member(&_deeenumiterator_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deeenumiterator_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deeenumiterator_tp_not),
  member(&_deeenumiterator_tp_bool),
  null,null,null,null,null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deeenumiterator_tp_cmp_lo),
  member(&_deeenumiterator_tp_cmp_le),
  member(&_deeenumiterator_tp_cmp_eq),
  member(&_deeenumiterator_tp_cmp_ne),
  member(&_deeenumiterator_tp_cmp_gr),
  member(&_deeenumiterator_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  null,null,null,null,null,null,null,null,member(&DeeObject_Copy),
  member(&_deeenumiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deeenumiterator_tp_members),null,
  member(_deegenericiterable_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deeenumiterator_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};



DEE_DECL_END

#endif /* !GUARD_DEEMON_ITERATOR_C */
