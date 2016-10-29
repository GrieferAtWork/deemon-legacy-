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
#ifndef GUARD_DEEMON_ARRAY_C
#define GUARD_DEEMON_ARRAY_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS

// /include/*
#include <deemon/array.h>
#include <deemon/bool.h>
#include <deemon/error.h>
#include <deemon/gc.h>
#include <deemon/generic_vtable.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/type_util.h>
#include <deemon/optional/object_visit.h>
#include <deemon/integer.h>
#include <deemon/list.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/sequence.h>
#include <deemon/string.h>
#include <deemon/tuple.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>
#include <deemon/optional/std/string.h>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN


DEE_STATIC_INLINE(int) DeeArrayType_Init(
 DeeArrayTypeObject *self, DeeStructuredTypeObject *base, Dee_size_t n);
DEE_A_RET_TYPEOBJECT_EXCEPT_REF(DeeArrayTypeObject) *_DeeArrayType_New(
 DEE_A_INOUT DeeStructuredTypeObject *base, DEE_A_IN Dee_size_t n) {
 DeeArrayTypeObject *result;
 DEE_ASSERT(DeeObject_Check(base) && DeeStructuredType_Check(base));
 if ((result = DeeObject_MALLOCF(DeeArrayTypeObject,
  "Array type: %s[%u]",DeeType_NAME(base),(unsigned)n)) != NULL) {
  DeeObject_INIT(result,&DeeArrayType_Type);
  if (DeeArrayType_Init(result,base,n) != 0) {
   _DeeObject_DELETE(&DeeArrayType_Type,result);
   result = NULL;
  }
 }
 return (DeeTypeObject *)result;
}


DEE_STATIC_INLINE(char *) _DeeArrayType_AllocName(
 char const *base_name, Dee_size_t n_elem) {
 char *result; DeeStringWriter writer = DeeStringWriter_INIT();
 if (DeeStringWriter_WriteString(&writer,base_name) != 0 ||
     DeeStringWriter_WRITE_STRING(&writer,"[") != 0 ||
     DEE_PP_CAT_2(DeeStringWriter_SpecWriteUInt,DEE_PP_MUL8(DEE_TYPES_SIZEOF_SIZE_T))
     (&writer,n_elem,&dee_decimal_default_spec) != 0 ||
     DeeStringWriter_WRITE_STRING(&writer,"]") != 0) goto err;
 result = _DeeStringWriter_PackCString(&writer,NULL);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}

DEE_STATIC_INLINE(int) _DeeArray_AssignRangeFromIterator(
 DeeStructuredTypeObject *elem_type, Dee_size_t elem_count, Dee_uint8_t *dst, DeeObject *iterator) {
 Dee_size_t p_instance_size; DeeObject *elem;
 Dee_uint8_t *dst_end,*dst_iter; int error;
 p_instance_size = DeeType_GET_SLOT(elem_type,tp_p_instance_size);
 dst_end = (dst_iter = dst)+(elem_count*DeeType_GET_SLOT(elem_type,tp_p_instance_size));
 while (1) {
  if ((error = DeeObject_IterNextEx(iterator,&elem)) != 0) {
   if (error < 0) return error;
   if (dst_iter == dst_end) break;
   DeeError_SetStringf(&DeeErrorType_ValueError,
    "%k[%Iu] expected an iterable of size %Iu (got: %Iu)",
    elem_type,elem_count,elem_count,
    (Dee_size_t)((dst_iter-dst)/DeeType_GET_SLOT(elem_type,tp_p_instance_size)));
   return -1;
  }
  if (dst_iter == dst_end) {
   DeeError_SetStringf(
    &DeeErrorType_ValueError,
    "%k[%Iu] expected an iterable of size %Iu (got: >= %Iu)",
    elem_type,elem_count,elem_count,elem_count+1);
   Dee_DECREF(elem);
   return -1;
  }
  DEE_ASSERT(dst_iter < dst_end);
  error = DeeStructured_SetData((DeeTypeObject *)elem_type,dst_iter,elem);
  Dee_DECREF(elem);
  if (error != 0) return error;
  dst_iter += p_instance_size;
 }
 return 0;
}
static int _DeeArray_AssignRangeFromSequence(
 DeeStructuredTypeObject *elem_type, Dee_size_t elem_count, Dee_uint8_t *dst, DeeObject *seq) {
 DeeObject *iter; int temp;
 if (DeeNone_Check(seq)) {
  memset(dst,0,elem_count*DeeType_GET_SLOT(elem_type,tp_p_instance_size));
  return 0;
 }
 if ((iter = DeeObject_IterSelf(seq)) == NULL) return -1;
 temp = _DeeArray_AssignRangeFromIterator(elem_type,elem_count,dst,iter);
 Dee_DECREF(iter);
 return temp;
}
static int _DeeArray_AssignIntegralRangeFromSequence(
 DeeStructuredTypeObject *elem_type, Dee_size_t elem_count, Dee_uint8_t *dst, DeeObject *seq) {
 DEE_ASSERT(DeeType_GET_SLOT(elem_type,tp_p_instance_size) == 1 ||
            DeeType_GET_SLOT(elem_type,tp_p_instance_size) == 2 ||
            DeeType_GET_SLOT(elem_type,tp_p_instance_size) == 4 ||
            DeeType_GET_SLOT(elem_type,tp_p_instance_size) == 8);
 if (DeeString_Check(seq)) {
  Dee_uint8_t const *src; Dee_size_t elem_size;
  if (DeeString_SIZE(seq) != elem_count) {
   DeeError_SetStringf(&DeeErrorType_ValueError,
    "%k[%Iu] expected an iterable of size %Iu (got: %Iu)",
    elem_type,elem_count,elem_count,DeeString_SIZE(seq));
   return -1;
  }
  elem_size = DeeType_GET_SLOT(elem_type,tp_p_instance_size);
  src = (Dee_uint8_t const *)DeeAnyString_DATAV(seq);
  switch (elem_size) {
   case 1: memcpy(dst,src,elem_count); break;
   case 2: while (elem_count--) *(Dee_uint16_t *)dst++ = (Dee_uint16_t)*src++; break;
   case 4: while (elem_count--) *(Dee_uint32_t *)dst++ = (Dee_uint32_t)*src++; break;
   case 8: while (elem_count--) *(Dee_uint64_t *)dst++ = (Dee_uint64_t)*src++; break;
   default: DEE_BUILTIN_UNREACHABLE();
  }
  return 0;
 }
 // TODO: Other string encodings
 return _DeeArray_AssignRangeFromSequence(elem_type,elem_count,dst,seq);
}

static int _deearray_tp_p_any_ctor(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *args) {
 DeeObject *seq;
 if (DeeTuple_Unpack(args,"o:array",&seq) != 0) return -1;
 return _DeeArray_AssignRangeFromSequence(
  tp_self->at_array_base,tp_self->at_array_elem_count,self,seq);
}
static int _deearray_tp_p_any_assign(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *right) {
 return _DeeArray_AssignRangeFromSequence(
  tp_self->at_array_base,tp_self->at_array_elem_count,self,right);
}
static int _deeintarray_tp_p_any_ctor(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *args) {
 DeeObject *seq;
 if (DeeTuple_Unpack(args,"o:array",&seq) != 0) return -1;
 return _DeeArray_AssignIntegralRangeFromSequence(
  tp_self->at_array_base,tp_self->at_array_elem_count,self,seq);
}
static int _deeintarray_tp_p_any_assign(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *right) {
 return _DeeArray_AssignIntegralRangeFromSequence(
  tp_self->at_array_base,tp_self->at_array_elem_count,self,right);
}
static DeeObject *_deearray_tp_p_str(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self) {
 DeeObject *temp_ob,*result; Dee_uint8_t *self_end; int temp;
 DeeStringWriter writer = DeeStringWriter_INIT();
 if (DeeStringWriter_WRITE_STRING(&writer,"[") != 0) goto err;
 self_end = self+DeeType_GET_SLOT(tp_self,tp_p_instance_size);
 if (self != self_end) while (1) {
  DEE_ASSERT(self < self_end);
  if ((temp_ob = DeeObject_PStr((DeeTypeObject *)tp_self->at_array_base,self)) == NULL) goto err;
  temp = DeeStringWriter_WriteStringWithLength(&writer,DeeString_SIZE(temp_ob),DeeString_STR(temp_ob));
  Dee_DECREF(temp_ob);
  if (temp != 0) goto err;
  if ((self += tp_self->at_array_elem_size) == self_end) break;
  if (DeeStringWriter_WRITE_STRING(&writer,",") != 0) goto err;
 }
 if (DeeStringWriter_WRITE_STRING(&writer,"]") != 0) goto err;
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
static DeeObject *_deearray_tp_p_add(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *right) {
 Dee_rt_ptrdiff_t diff; DeeTypeObject *pointer_type;
 if (DeeObject_Cast(Dee_rt_ptrdiff_t,right,&diff) != 0) return NULL;
 if ((pointer_type = DeeType_Pointer((
  DeeTypeObject *)tp_self->at_array_base)) == NULL) return NULL;
 return DeePointer_New(pointer_type,self+(diff*tp_self->at_array_elem_size));
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
static DeeObject *_deearray_tp_p_repr(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self) {
 DeeObject *temp_ob,*result; Dee_uint8_t *self_end; int temp;
 DeeStringWriter writer = DeeStringWriter_INIT();
 if (DeeStringWriter_WriteObjectStr(&writer,(DeeObject *)tp_self) != 0 ||
     DeeStringWriter_WRITE_STRING(&writer," { ") != 0) goto err;
 self_end = self+DeeType_GET_SLOT(tp_self,tp_p_instance_size);
 if (self != self_end) while (1) {
  DEE_ASSERT(self < self_end);
  if ((temp_ob = DeeObject_PRepr((DeeTypeObject *)tp_self->at_array_base,self)) == NULL) goto err;
  temp = DeeStringWriter_WriteStringWithLength(&writer,DeeString_SIZE(temp_ob),DeeString_STR(temp_ob));
  Dee_DECREF(temp_ob);
  if (temp != 0) goto err;
  if ((self += tp_self->at_array_elem_size) == self_end) break;
  if (DeeStringWriter_WRITE_STRING(&writer,",") != 0) goto err;
 }
 if (DeeStringWriter_WRITE_STRING(&writer," }") != 0) goto err;
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
static DeeObject *_deearray_tp_p_sub_zero(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *right) {
 void *ptr_right; int temp;
 Dee_rt_ptrdiff_t diff; DeeTypeObject *pointer_type;
 if ((temp = DeeObject_TryGetPointer(right,(DeeTypeObject *)
  tp_self->at_array_base,&ptr_right)) < 0) return NULL;
 if (temp == 0) return DeeObject_New(Dee_rt_ptrdiff_t,0);
 if (DeeObject_Cast(Dee_rt_ptrdiff_t,right,&diff) != 0) return NULL;
 if ((pointer_type = DeeType_Pointer((DeeTypeObject *)tp_self->at_array_base)) == NULL) return NULL;
 return DeePointer_New(pointer_type,self);
}
static DeeObject *_deearray_tp_p_sub(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *right) {
 void *ptr_right; int temp;
 Dee_rt_ptrdiff_t diff; DeeTypeObject *pointer_type;
 if ((temp = DeeObject_TryGetPointer(right,(DeeTypeObject *)
  tp_self->at_array_base,&ptr_right)) < 0) return NULL;
 if (temp == 0) { // ptr diff
  return DeeObject_New(Dee_rt_ptrdiff_t,(Dee_rt_ptrdiff_t)(
   ((Dee_ssize_t)self-(Dee_ssize_t)ptr_right)/(Dee_ssize_t)tp_self->at_array_elem_size));
 }
 if (DeeObject_Cast(Dee_rt_ptrdiff_t,right,&diff) != 0) return NULL;
 if ((pointer_type = DeeType_Pointer((DeeTypeObject *)tp_self->at_array_base)) == NULL) return NULL;
 return DeePointer_New(pointer_type,self-(diff*tp_self->at_array_elem_size));
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DEE_PRIVATE_MAKE_ARRAY_CMP(name,op)\
static DeeObject *name(\
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *right) {\
 if (DeeLValue_Check(right) && DeeLValueType_BASE(right) == (DeeTypeObject *)tp_self) {\
  DeeReturn_Bool(memcmp(self,DeeLValue_ADDR(right),DeeType_GET_SLOT(tp_self,tp_p_instance_size)) op 0);\
 } else if (DeeError_TypeError_CheckTypeExact(right,(DeeTypeObject *)tp_self) != 0) return NULL;\
 DeeReturn_Bool(memcmp(self,DeeArray_ELEM(right),DeeType_GET_SLOT(tp_self,tp_p_instance_size)) op 0);\
}\
static DeeObject *name##_empty(\
 DeeArrayTypeObject *DEE_UNUSED(tp_self), Dee_uint8_t *DEE_UNUSED(self), DeeObject *right) {\
 int temp;\
 if ((temp = DeeSequence_Empty(right)) < 0) return NULL;\
 DeeReturn_Bool(0 op temp);\
}
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#define DEE_PRIVATE_MAKE_ARRAY_CMP(name,op)\
static DeeObject *name(\
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *right) {\
 if (DeeError_TypeError_CheckTypeExact(right,(DeeTypeObject *)tp_self) != 0) return NULL;\
 DeeReturn_Bool(memcmp(self,DeeArray_ELEM(right),DeeType_GET_SLOT(tp_self,tp_p_instance_size)) op 0);\
}\
static DeeObject *name##_empty(\
 DeeArrayTypeObject *DEE_UNUSED(tp_self), Dee_uint8_t *DEE_UNUSED(self), DeeObject *right) {\
 int temp;\
 if ((temp = DeeSequence_Empty(right)) < 0) return NULL;\
 DeeReturn_Bool(0 op temp);\
}
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */

DEE_PRIVATE_MAKE_ARRAY_CMP(_deearray_tp_p_cmp_lo,<)
DEE_PRIVATE_MAKE_ARRAY_CMP(_deearray_tp_p_cmp_le,<=)
DEE_PRIVATE_MAKE_ARRAY_CMP(_deearray_tp_p_cmp_eq,==)
DEE_PRIVATE_MAKE_ARRAY_CMP(_deearray_tp_p_cmp_ne,!=)
DEE_PRIVATE_MAKE_ARRAY_CMP(_deearray_tp_p_cmp_gr,>)
DEE_PRIVATE_MAKE_ARRAY_CMP(_deearray_tp_p_cmp_ge,>=)
#undef DEE_PRIVATE_MAKE_ARRAY_CMP

static int _deearray_tp_p_bool(DeeArrayTypeObject *DEE_UNUSED(tp_self), Dee_uint8_t *DEE_UNUSED(self)) { return 1; }
static int _deearray_tp_p_bool_empty(DeeArrayTypeObject *DEE_UNUSED(tp_self), Dee_uint8_t *DEE_UNUSED(self)) { return 0; }
static DeeObject *_deearray_tp_p_not(DeeArrayTypeObject *DEE_UNUSED(tp_self), Dee_uint8_t *DEE_UNUSED(self)) { DeeReturn_False; }
static DeeObject *_deearray_tp_p_not_empty(DeeArrayTypeObject *DEE_UNUSED(tp_self), Dee_uint8_t *DEE_UNUSED(self)) { DeeReturn_True; }
static DeeObject *_deearray_tp_p_seq_contains_empty(
 DeeArrayTypeObject *DEE_UNUSED(tp_self), Dee_uint8_t *DEE_UNUSED(self), DeeObject *DEE_UNUSED(i)) {
 DeeReturn_False;
}
static DeeObject *_deearray_tp_p_seq_get_empty(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *DEE_UNUSED(self), DeeObject *DEE_UNUSED(i)) {
 DeeError_ValueError_EmptyList((DeeTypeObject *)tp_self);
 return NULL;
}
static int _deearray_tp_p_seq_del_empty(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *DEE_UNUSED(self), DeeObject *DEE_UNUSED(i)) {
 DeeError_ValueError_EmptyList((DeeTypeObject *)tp_self);
 return -1;
}
static int _deearray_tp_p_seq_set_empty(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *DEE_UNUSED(self),
 DeeObject *DEE_UNUSED(i), DeeObject *DEE_UNUSED(v)) {
 DeeError_ValueError_EmptyList((DeeTypeObject *)tp_self);
 return -1;
}
static DeeObject *_deearray_tp_p_seq_range_get_empty(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *lo, DeeObject *hi) {
 Dee_ssize_t tempidx;
 if (DeeObject_Cast(Dee_ssize_t,lo,&tempidx) != 0) return NULL;
 if (DeeObject_Cast(Dee_ssize_t,hi,&tempidx) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 {
  DeeTypeObject *lvalue_type;
  if ((lvalue_type = DeeType_LValue((DeeTypeObject *)tp_self)) == NULL) return NULL;
  return DeeLValue_New(lvalue_type,(void *)self);
 }
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 return DeeStructured_NewFromData((DeeTypeObject *)tp_self,(void *)self);
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
}
static int _deearray_tp_p_seq_range_del_empty(
 DeeArrayTypeObject *DEE_UNUSED(tp_self),
 Dee_uint8_t *DEE_UNUSED(self), DeeObject *lo, DeeObject *hi) {
 Dee_ssize_t tempidx;
 if (DeeObject_Cast(Dee_ssize_t,lo,&tempidx) != 0) return -1;
 if (DeeObject_Cast(Dee_ssize_t,hi,&tempidx) != 0) return -1;
 return 0;
}
static int _deearray_tp_p_seq_range_set_empty(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *DEE_UNUSED(self),
 DeeObject *lo, DeeObject *hi, DeeObject *v) {
 int temp; Dee_ssize_t tempidx;
 if (DeeObject_Cast(Dee_ssize_t,lo,&tempidx) != 0) return -1;
 if (DeeObject_Cast(Dee_ssize_t,hi,&tempidx) != 0) return -1;
 if ((temp = DeeSequence_Empty(v)) < 0) return -1;
 if (!temp) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "%k expected an iterable of size 0 (got: >= 1)",
                      tp_self);
  return -1;
 }
 return 0;
}

static DeeObject *_deearray_tp_p_seq_contains(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *elem) {
 int temp; Dee_uint8_t *self_end;
 self_end = self+DeeType_GET_SLOT(tp_self,tp_p_instance_size);
 while (self != self_end) {
  if ((temp = DeeObject_PCompareEq((DeeTypeObject *)tp_self->at_array_base,self,elem)) < 0) {
   if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return NULL;
  } else if (temp) DeeReturn_True;
  self += tp_self->at_array_elem_size;
 }
 DeeReturn_False;
}
static DeeObject *_deearray_tp_p_seq_get(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *i) {
 Dee_ssize_t ii;
 DEE_ASSERT(tp_self->at_array_elem_count);
 if (DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return NULL;
 DEE_PRIVATE_CLAMP_INDEX(ii,tp_self->at_array_elem_count);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 {
  DeeTypeObject *tp_result;
  if ((tp_result = DeeType_LValue((DeeTypeObject *)tp_self->at_array_base)) == NULL) return NULL;
  return DeeLValue_New(tp_result,self+(ii*tp_self->at_array_elem_size));
 }
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 return DeeStructured_NewFromData((DeeTypeObject *)tp_self->at_array_base,
                                  self+(ii*tp_self->at_array_elem_size));
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
}
static int _deearray_tp_p_seq_del(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *i) {
 Dee_ssize_t ii;
 DEE_ASSERT(tp_self->at_array_elem_count);
 if (DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return -1;
 DEE_PRIVATE_CLAMP_INDEX(ii,tp_self->at_array_elem_count);
 memset(self+ii*tp_self->at_array_elem_size,0,tp_self->at_array_elem_size);
 return 0;
}
static int _deearray_tp_p_seq_set(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *i, DeeObject *v) {
 Dee_ssize_t ii;
 DEE_ASSERT(tp_self->at_array_elem_count);
 if (DeeNone_Check(v)) return _deearray_tp_p_seq_del(tp_self,self,i);
 if (DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return -1;
 DEE_PRIVATE_CLAMP_INDEX(ii,tp_self->at_array_elem_count);
 return DeeStructured_SetData((DeeTypeObject *)tp_self->at_array_base,
                              self+ii*tp_self->at_array_elem_size,v);
}
static DeeObject *_deearray_tp_p_seq_range_get(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *lo, DeeObject *hi) {
 Dee_ssize_t loi,hii; DeeTypeObject *tp_result;
 DEE_ASSERT(tp_self->at_array_elem_count);
 if (!DeeNone_Check(lo)) { if (DeeObject_Cast(Dee_ssize_t,lo,&loi) != 0) return NULL; } else loi = 0;
 if (!DeeNone_Check(hi)) { if (DeeObject_Cast(Dee_ssize_t,hi,&hii) != 0) return NULL; } else hii = (Dee_ssize_t)tp_self->at_array_elem_count;
 DEE_PRIVATE_CLAMP_RANGE(loi,hii,tp_self->at_array_elem_count);
 if (hii < loi) hii = loi;
 if ((tp_result = DeeType_Array((DeeTypeObject *)tp_self->at_array_base,(Dee_size_t)(hii-loi))) == NULL) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if ((tp_result = DeeType_LValue(tp_result)) == NULL) return NULL;
 return DeeLValue_New(tp_result,self+(loi*tp_self->at_array_elem_size));
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 return DeeStructured_NewFromData(tp_result,self+(loi*tp_self->at_array_elem_size));
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
}
static int _deearray_tp_p_seq_range_del(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *lo, DeeObject *hi) {
 Dee_ssize_t loi,hii;
 DEE_ASSERT(tp_self->at_array_elem_count);
 if (!DeeNone_Check(lo)) { if (DeeObject_Cast(Dee_ssize_t,lo,&loi) != 0) return -1; } else loi = 0;
 if (!DeeNone_Check(hi)) { if (DeeObject_Cast(Dee_ssize_t,hi,&hii) != 0) return -1; } else hii = (Dee_ssize_t)tp_self->at_array_elem_count;
 DEE_PRIVATE_CLAMP_RANGE(loi,hii,tp_self->at_array_elem_count);
 if (hii < loi) return 0;
 memset(self+(loi*tp_self->at_array_elem_size),0,(Dee_size_t)(hii-loi)*tp_self->at_array_elem_size);
 return 0;
}
static int _deearray_tp_p_seq_range_set(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self,
 DeeObject *lo, DeeObject *hi, DeeObject *v) {
 Dee_ssize_t loi,hii;
 DEE_ASSERT(tp_self->at_array_elem_count);
 if (!DeeNone_Check(lo)) { if (DeeObject_Cast(Dee_ssize_t,lo,&loi) != 0) return -1; } else loi = 0;
 if (!DeeNone_Check(hi)) { if (DeeObject_Cast(Dee_ssize_t,hi,&hii) != 0) return -1; } else hii = (Dee_ssize_t)tp_self->at_array_elem_count;
 DEE_PRIVATE_CLAMP_RANGE(loi,hii,tp_self->at_array_elem_count);
 if (hii < loi) hii = loi;
 return _DeeArray_AssignRangeFromSequence(
  tp_self->at_array_base,(Dee_size_t)(hii-loi),
  self+(loi*tp_self->at_array_elem_size),v);
}
static int _deeintarray_tp_p_seq_range_set(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self,
 DeeObject *lo, DeeObject *hi, DeeObject *v) {
 Dee_ssize_t loi,hii;
 DEE_ASSERT(tp_self->at_array_elem_count);
 if (!DeeNone_Check(lo)) { if (DeeObject_Cast(Dee_ssize_t,lo,&loi) != 0) return -1; } else loi = 0;
 if (!DeeNone_Check(hi)) { if (DeeObject_Cast(Dee_ssize_t,hi,&hii) != 0) return -1; } else hii = (Dee_ssize_t)tp_self->at_array_elem_count;
 DEE_PRIVATE_CLAMP_RANGE(loi,hii,tp_self->at_array_elem_count);
 if (hii < loi) hii = loi;
 return _DeeArray_AssignIntegralRangeFromSequence(
  tp_self->at_array_base,(Dee_size_t)(hii-loi),
  self+(loi*tp_self->at_array_elem_size),v);
}
static DeeObject *_deearray_tp_p_seq_size(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *DEE_UNUSED(self)) {
 return DeeObject_New(Dee_size_t,tp_self->at_array_elem_count);
}
static DeeObject *_deearray_tp_p_seq_iter_self(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self) {
 return DeeArrayIterator_New((
  DeeTypeObject *)tp_self->at_array_base,self,tp_self->at_array_elem_count);
}
static DeeObject *_deearray_tp_p_seq_iter_self_empty(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self) {
 return DeeArrayIterator_New((DeeTypeObject *)tp_self->at_array_base,self,0);
}




static DeeObject *_deearray_empty(
 DeeArrayTypeObject *DEE_UNUSED(tp_self), Dee_uint8_t *DEE_UNUSED(self),
 DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":empty") != 0) return NULL;
 DeeReturn_False;
}
static DeeObject *_deearray_empty_empty(
 DeeArrayTypeObject *DEE_UNUSED(tp_self), Dee_uint8_t *DEE_UNUSED(self),
 DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":empty") != 0) return NULL;
 DeeReturn_True;
}
static DeeObject *_deearray_non_empty(
 DeeArrayTypeObject *DEE_UNUSED(tp_self), Dee_uint8_t *DEE_UNUSED(self),
 DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":non_empty") != 0) return NULL;
 DeeReturn_True;
}
static DeeObject *_deearray_non_empty_empty(
 DeeArrayTypeObject *DEE_UNUSED(tp_self), Dee_uint8_t *DEE_UNUSED(self),
 DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":non_empty") != 0) return NULL;
 DeeReturn_False;
}
DEE_STATIC_INLINE(Dee_ssize_t) _DeeArray_Find(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *elem) {
 Dee_size_t elem_size = tp_self->at_array_elem_size;
 Dee_uint8_t *iter = self,*end = self+DeeType_GET_SLOT(tp_self,tp_p_instance_size);
 DeeTypeObject *elem_type = (DeeTypeObject *)tp_self->at_array_base;
 do { int temp;
  DEE_ASSERT(iter <= end-elem_size);
  if ((temp = DeeObject_PCompareEq(elem_type,iter,elem)) < 0) {
   if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return (Dee_ssize_t)-2;
  } else if (temp) return (Dee_ssize_t)((iter-self)/elem_size);
 } while ((iter += elem_size) != end);
 return (Dee_ssize_t)-1;
}
DEE_STATIC_INLINE(Dee_size_t) _DeeArray_Count(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self, DeeObject *elem) {
 Dee_size_t result = 0,elem_size = tp_self->at_array_elem_size;
 Dee_uint8_t *end = self+DeeType_GET_SLOT(tp_self,tp_p_instance_size);
 DeeTypeObject *elem_type = (DeeTypeObject *)tp_self->at_array_base;
 do { int temp;
  DEE_ASSERT(self <= end-elem_size);
  if ((temp = DeeObject_PCompareEq(elem_type,self,elem)) < 0) {
   if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return (Dee_size_t)-1;
  } else if (temp) ++result;
 } while ((self += elem_size) != end);
 return result;
}

static DeeObject *_deearray_find(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self,
 DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem; Dee_ssize_t result;
 if (DeeTuple_Unpack(args,"o:find",&elem) != 0) return NULL;
 if ((result = _DeeArray_Find(tp_self,self,elem)) == (Dee_ssize_t)-2) return NULL;
 return DeeObject_New(Dee_ssize_t,result);
}
static DeeObject *_deearray_find_empty(
 DeeArrayTypeObject *DEE_UNUSED(tp_self), Dee_uint8_t *DEE_UNUSED(self),
 DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem;
 if (DeeTuple_Unpack(args,"o:find",&elem) != 0) return NULL;
 return DeeObject_New(Dee_ssize_t,(Dee_ssize_t)-1);
}
static DeeObject *_deearray_index(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self,
 DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem; Dee_ssize_t result;
 if (DeeTuple_Unpack(args,"o:index",&elem) != 0) return NULL;
 if ((result = _DeeArray_Find(tp_self,self,elem)) == (Dee_ssize_t)-2) return NULL;
 if (result == (Dee_ssize_t)-1) { DeeError_ValueError_NotFound_T((DeeTypeObject *)tp_self,elem); return NULL; }
 return DeeObject_New(Dee_size_t,(Dee_size_t)result);
}
static DeeObject *_deearray_index_empty(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *DEE_UNUSED(self),
 DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem;
 if (DeeTuple_Unpack(args,"o:index",&elem) != 0) return NULL;
 DeeError_ValueError_NotFound_T((DeeTypeObject *)tp_self,elem);
 return NULL;
}
static DeeObject *_deearray_count(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self,
 DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem; Dee_size_t result;
 if (DeeTuple_Unpack(args,"o:count",&elem) != 0) return NULL;
 if ((result = _DeeArray_Count(tp_self,self,elem)) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *_deearray_count_empty(
 DeeArrayTypeObject *DEE_UNUSED(tp_self), Dee_uint8_t *DEE_UNUSED(self),
 DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem;
 if (DeeTuple_Unpack(args,"o:count",&elem) != 0) return NULL;
 return DeeObject_New(Dee_size_t,0);
}
static DeeObject *_deearray_front(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self,
 DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":front") != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 {
  DeeTypeObject *lvalue_type;
  if ((lvalue_type = DeeType_LValue((DeeTypeObject *)tp_self->at_array_base)) == NULL) return NULL;
  return DeeLValue_New(lvalue_type,self);
 }
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 return DeeStructured_NewFromData((DeeTypeObject *)tp_self->at_array_base,self);
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
}
static DeeObject *_deearray_front_empty(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *DEE_UNUSED(self),
 DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":front") != 0) return NULL;
 DeeError_ValueError_EmptyList((DeeTypeObject *)tp_self);
 return NULL;
}
static DeeObject *_deearray_back(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self,
 DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":back") != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 {
  DeeTypeObject *lvalue_type;
  if ((lvalue_type = DeeType_LValue((DeeTypeObject *)tp_self->at_array_base)) == NULL) return NULL;
  return DeeLValue_New(lvalue_type,self+(DeeType_GET_SLOT(tp_self,tp_p_instance_size)-tp_self->at_array_elem_size));
 }
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 return DeeStructured_NewFromData((DeeTypeObject *)tp_self->at_array_base,self+(
  DeeType_GET_SLOT(tp_self,tp_p_instance_size)-tp_self->at_array_elem_size));
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
}
static DeeObject *_deearray_back_empty(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *DEE_UNUSED(self),
 DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":back") != 0) return NULL;
 DeeError_ValueError_EmptyList((DeeTypeObject *)tp_self);
 return NULL;
}
static DeeObject *_deearray_at(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self,
 DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t index;
 if (DeeTuple_Unpack(args,"Iu:at",&index) != 0) return NULL;
 if (index >= tp_self->at_array_elem_count) { DeeError_IndexError_T((DeeTypeObject *)tp_self,index); return NULL; }
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 {
  DeeTypeObject *lvalue_type;
  if ((lvalue_type = DeeType_LValue((DeeTypeObject *)tp_self->at_array_base)) == NULL) return NULL;
  return DeeLValue_New(lvalue_type,self+(index*tp_self->at_array_elem_size));
 }
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 return DeeStructured_NewFromData((DeeTypeObject *)tp_self->at_array_base,
                                  self+(index*tp_self->at_array_elem_size));
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
}
static DeeObject *_deearray_at_empty(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *DEE_UNUSED(self),
 DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t index;
 if (DeeTuple_Unpack(args,"Iu:back",&index) != 0) return NULL;
 DeeError_IndexError_T((DeeTypeObject *)tp_self,index);
 return NULL;
}
static DeeObject *_deearray_fill(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self,
 DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem; Dee_size_t elem_size; Dee_uint8_t *iter,*self_end;
 if (DeeTuple_Unpack(args,"o:fill",&elem) != 0) return NULL;
 if (DeeStructured_SetData((DeeTypeObject *)
  tp_self->at_array_base,self,elem) != 0) return NULL;
 self_end = self+DeeType_GET_SLOT(tp_self,tp_p_instance_size);
 iter = self+(elem_size = tp_self->at_array_elem_size);
 while (iter != self_end) { memcpy(iter,self,elem_size); iter += elem_size; }
 DeeReturn_None;
}
static DeeObject *_deearray_fill_empty(
 DeeArrayTypeObject *DEE_UNUSED(tp_self), Dee_uint8_t *DEE_UNUSED(self),
 DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem;
 if (DeeTuple_Unpack(args,"o:fill",&elem) != 0) return NULL;
 DeeReturn_None;
}


static DeeObject *_deearray_locate(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self,
 DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *temp,*pred,*pred_args,*elem; int itemp; Dee_size_t elem_size;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeTypeObject *elem_lvalue;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DeeTypeObject *elem_type;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 Dee_uint8_t *end;
 if (DeeTuple_Unpack(args,"o:locate",&pred) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if ((elem_lvalue = DeeType_LValue((DeeTypeObject *)tp_self->at_array_base)) == NULL) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 elem_type = (DeeTypeObject *)tp_self->at_array_base;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 elem_size = DeeType_GET_SLOT(tp_self->at_array_base,tp_p_instance_size);
 end = (self+DeeType_GET_SLOT(tp_self,tp_p_instance_size));
 while (self != end) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  if ((elem = DeeLValue_New(elem_lvalue,self)) == NULL) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  if ((elem = DeeStructured_NewFromData(elem_type,self)) == NULL) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  pred_args = DeeTuple_Pack(1,elem);
  if (!pred_args) {err_elem: Dee_DECREF(elem); return NULL; }
  temp = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if (!temp) goto err_elem;
  itemp = DeeObject_Bool(temp);
  Dee_DECREF(temp);
  if (itemp < 0) goto err_elem;
  if (itemp) return elem;
  Dee_DECREF(elem);
  self += elem_size;
 }
 DeeError_ValueError_NotFound_Pred_T((DeeTypeObject *)tp_self,pred);
 return NULL; 
}
static DeeObject *_deearray_locate_empty(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *DEE_UNUSED(self),
 DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if (DeeTuple_Unpack(args,"o:locate",&pred) != 0) return NULL;
 DeeError_ValueError_NotFound_Pred_T((DeeTypeObject *)tp_self,pred);
 return NULL; 
}
static DeeObject *_deearray_rlocate(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self,
 DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *temp,*pred,*pred_args,*elem; int itemp; Dee_size_t elem_size;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeTypeObject *elem_lvalue;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DeeTypeObject *elem_type;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 Dee_uint8_t *end;
 if (DeeTuple_Unpack(args,"o:rlocate",&pred) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if ((elem_lvalue = DeeType_LValue((DeeTypeObject *)tp_self->at_array_base)) == NULL) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 elem_type = (DeeTypeObject *)tp_self->at_array_base;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 elem_size = DeeType_GET_SLOT(tp_self->at_array_base,tp_p_instance_size);
 end = (self+DeeType_GET_SLOT(tp_self,tp_p_instance_size));
 while (end != self) {
  end -= elem_size;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  if ((elem = DeeLValue_New(elem_lvalue,end)) == NULL) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  if ((elem = DeeStructured_NewFromData(elem_type,end)) == NULL) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  pred_args = DeeTuple_Pack(1,elem);
  if (!pred_args) {err_elem: Dee_DECREF(elem); return NULL; }
  temp = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if (!temp) goto err_elem;
  itemp = DeeObject_Bool(temp);
  Dee_DECREF(temp);
  if (itemp < 0) goto err_elem;
  if (itemp) return elem;
  Dee_DECREF(elem);
 }
 DeeError_ValueError_NotFound_Pred_T((DeeTypeObject *)tp_self,pred);
 return NULL; 
}
static DeeObject *_deearray_rlocate_empty(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *DEE_UNUSED(self),
 DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if (DeeTuple_Unpack(args,"o:rlocate",&pred) != 0) return NULL;
 DeeError_ValueError_NotFound_Pred_T((DeeTypeObject *)tp_self,pred);
 return NULL; 
}
static DeeObject *_deearray_locate_all(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self,
 DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *temp,*pred,*pred_args,*elem,*result; int itemp; Dee_size_t elem_size;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeTypeObject *elem_lvalue;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DeeTypeObject *elem_type;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 Dee_uint8_t *end;
 if (DeeTuple_Unpack(args,"o:locate_all",&pred) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if ((elem_lvalue = DeeType_LValue((DeeTypeObject *)tp_self->at_array_base)) == NULL) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 elem_type = (DeeTypeObject *)tp_self->at_array_base;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 elem_size = DeeType_GET_SLOT(tp_self->at_array_base,tp_p_instance_size);
 end = (self+DeeType_GET_SLOT(tp_self,tp_p_instance_size));
 if ((result = DeeList_New()) == NULL) return NULL;
 while (self != end) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  if ((elem = DeeLValue_New(elem_lvalue,self)) == NULL) {err_r: Dee_DECREF(result); return NULL; }
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  if ((elem = DeeStructured_NewFromData(elem_type,self)) == NULL) {err_r: Dee_DECREF(result); return NULL; }
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  pred_args = DeeTuple_Pack(1,elem);
  if (!pred_args) {err_elem: Dee_DECREF(elem); goto err_r; }
  temp = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if (!temp) goto err_elem;
  itemp = DeeObject_Bool(temp);
  Dee_DECREF(temp);
  if (itemp < 0) goto err_elem;
  if (itemp && DeeList_Append(result,elem) != 0) goto err_elem;
  Dee_DECREF(elem);
  self += elem_size;
 }
 return result; 
}
static DeeObject *_deearray_locate_all_empty(
 DeeArrayTypeObject *DEE_UNUSED(tp_self), Dee_uint8_t *DEE_UNUSED(self),
 DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if (DeeTuple_Unpack(args,"o:locate_all",&pred) != 0) return NULL;
 DeeReturn_EmptyList;
}
static DeeObject *_deearray_transform(
 DeeArrayTypeObject *tp_self, Dee_uint8_t *self,
 DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*pred,**dst,*pred_args,*elem; Dee_uint8_t *end;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeTypeObject *elem_lvalue;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DeeTypeObject *elem_type;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (DeeTuple_Unpack(args,"o:transform",&pred) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if ((elem_lvalue = DeeType_LValue((DeeTypeObject *)tp_self->at_array_base)) == NULL) return NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 elem_type = (DeeTypeObject *)tp_self->at_array_base;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if ((result = _DeeList_NewUnsafe(tp_self->at_array_elem_count)) != NULL) {
  dst = DeeList_ELEM(result);
  end = self+DeeType_GET_SLOT(tp_self,tp_p_instance_size);
  while (self != end) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   if ((elem = DeeLValue_New(elem_lvalue,self)) == NULL)
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   if ((elem = DeeStructured_NewFromData(elem_type,self)) == NULL)
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   {
err_r:
    while (dst != DeeList_ELEM(result)) Dee_DECREF(*--dst);
    _DeeList_FreeUnsafe(result);
    return NULL;
   }
   pred_args = DeeTuple_Pack(1,elem);
   Dee_DECREF(elem);
   if (!pred_args) goto err_r;
   elem = DeeObject_Call(pred,pred_args);
   Dee_DECREF(pred_args);
   if (!elem) goto err_r;
   Dee_INHERIT_REF(*dst++,elem);
  }
 }
 return result;
}
static DeeObject *_deearray_transform_empty(
 DeeArrayTypeObject *DEE_UNUSED(tp_self), Dee_uint8_t *DEE_UNUSED(self),
 DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if (DeeTuple_Unpack(args,"o:transform",&pred) != 0) return NULL;
 DeeReturn_EmptyList;
}

static struct DeeMethodDef const _deearray_tp_methods[] = {
 DEE_METHODDEF_STRUCT_v100("empty",member(&_deearray_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("non_empty",member(&_deearray_non_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("find",member(&_deearray_find),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("index",member(&_deearray_index),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("count",member(&_deearray_count),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("front",member(&_deearray_front),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("back",member(&_deearray_back),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("at",member(&_deearray_at),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("fill",member(&_deearray_fill),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("locate",member(&_deearray_locate),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("rlocate",member(&_deearray_rlocate),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("locate_all",member(&_deearray_locate_all),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("transform",member(&_deearray_transform),DEE_DOC_AUTO),
 // TODO: Missing standard functions: at(), find(), index(), empty(), non_empty(), count(), front(), back(), fill()
 DEE_METHODDEF_END_v100
};
static struct DeeMethodDef const _deearray_tp_methods_empty[] = {
 DEE_METHODDEF_STRUCT_v100("empty",member(&_deearray_empty_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("non_empty",member(&_deearray_non_empty_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("find",member(&_deearray_find_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("index",member(&_deearray_index_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("count",member(&_deearray_count_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("front",member(&_deearray_front_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("back",member(&_deearray_back_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("at",member(&_deearray_at_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("fill",member(&_deearray_fill_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("locate",member(&_deearray_locate_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("rlocate",member(&_deearray_rlocate_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("locate_all",member(&_deearray_locate_all_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_STRUCT_v100("transform",member(&_deearray_transform_empty),DEE_DOC_AUTO),
 // TODO: Missing standard functions: at(), find(), index(), empty(), non_empty(), count(), front(), back(), fill()
 DEE_METHODDEF_END_v100
};
static struct DeeMemberDef const _deearray_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeArrayIterator_Type),
 DEE_MEMBERDEF_END_v100
};

DEE_STATIC_INLINE(int) _DeeType_IsIntegral(DeeTypeObject *tp) {
 return
  tp == DeeObject_TYPE(Dee_rt_int8_t) ||
  tp == DeeObject_TYPE(Dee_rt_int16_t) ||
  tp == DeeObject_TYPE(Dee_rt_int32_t) ||
  tp == DeeObject_TYPE(Dee_rt_int64_t) ||
  tp == DeeObject_TYPE(Dee_rt_uint8_t) ||
  tp == DeeObject_TYPE(Dee_rt_uint16_t) ||
  tp == DeeObject_TYPE(Dee_rt_uint32_t) ||
  tp == DeeObject_TYPE(Dee_rt_uint64_t) ||
  tp == DeeObject_TYPE(atomic(Dee_rt_int8_t)) ||
  tp == DeeObject_TYPE(atomic(Dee_rt_int16_t)) ||
  tp == DeeObject_TYPE(atomic(Dee_rt_int32_t)) ||
  tp == DeeObject_TYPE(atomic(Dee_rt_int64_t)) ||
  tp == DeeObject_TYPE(atomic(Dee_rt_uint8_t)) ||
  tp == DeeObject_TYPE(atomic(Dee_rt_uint16_t)) ||
  tp == DeeObject_TYPE(atomic(Dee_rt_uint32_t)) ||
  tp == DeeObject_TYPE(atomic(Dee_rt_uint64_t))
 ;
}

DEE_STATIC_INLINE(int) DeeArrayType_Init(
 DeeArrayTypeObject *self, DeeStructuredTypeObject *elem_type, Dee_size_t n) {
 int is_integral_elem;
 DEE_ASSERT(DeeObject_Check(self) && DeeArrayType_Check(self));
 DEE_ASSERT(DeeObject_Check(elem_type) && DeeStructuredType_Check(elem_type));
 if ((*DeeType_GET_SLOT(&DeeStructuredType_Type,tp_ctor))(&DeeStructuredType_Type,(DeeObject *)self) != 0) return -1;
 if ((DeeType_GET_SLOT(self,tp_name) = _DeeArrayType_AllocName(
  DeeType_NAME(elem_type) ? DeeType_NAME(elem_type) : "<unnamed>",n)) == NULL) {
  (*DeeType_GET_SLOT(&DeeStructuredType_Type,tp_dtor))((DeeObject *)self);
  (*DeeType_GET_SLOT(&DeeType_Type,tp_dtor))((DeeObject *)self);
  return -1;
 }
 DeeType_GET_SLOT(self,tp_flags) &= ~DEE_TYPE_FLAG_STATIC_NAME;
 Dee_INCREF(self->at_array_base = elem_type);
 DeeType_GET_SLOT(self,tp_instance_size) = sizeof(DeeStructuredObject)+(
  DeeType_GET_SLOT(self,tp_p_instance_size) = (self->at_array_elem_count = n)*
  (self->at_array_elem_size = DeeType_GET_SLOT(elem_type,tp_p_instance_size)));
 DeeType_GET_SLOT(self,tp_class_members) = _deearray_tp_class_members;
 is_integral_elem = _DeeType_IsIntegral((DeeTypeObject *)elem_type);
 if (is_integral_elem) {
  DeeType_GET_SLOT(self,tp_p_any_ctor) = (int(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deeintarray_tp_p_any_ctor;
  DeeType_GET_SLOT(self,tp_p_any_assign) = (int(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deeintarray_tp_p_any_assign;
 } else {
  DeeType_GET_SLOT(self,tp_p_any_ctor) = (int(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_any_ctor;
  DeeType_GET_SLOT(self,tp_p_any_assign) = (int(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_any_assign;
 }
 DeeType_GET_SLOT(self,tp_p_str) = (DeeObject*(*)(DeeStructuredTypeObject*,void*))&_deearray_tp_p_str;
 DeeType_GET_SLOT(self,tp_p_repr) = (DeeObject*(*)(DeeStructuredTypeObject*,void*))&_deearray_tp_p_repr;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeType_GET_SLOT(self,tp_p_add) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_add;
 DeeType_GET_SLOT(self,tp_p_sub) = DeeType_GET_SLOT(elem_type,tp_p_instance_size)
  ? (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_sub
  : (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_sub_zero;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DeeType_GET_SLOT(self,tp_p_seq_size) = (DeeObject*(*)(DeeStructuredTypeObject*,void*))&_deearray_tp_p_seq_size;
 if (n) {
  DeeType_GET_SLOT(self,tp_p_bool) = (int(*)(DeeStructuredTypeObject*,void*))&_deearray_tp_p_bool;
  DeeType_GET_SLOT(self,tp_p_not) = (DeeObject*(*)(DeeStructuredTypeObject*,void*))&_deearray_tp_p_not;
  DeeType_GET_SLOT(self,tp_p_cmp_lo) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_cmp_lo;
  DeeType_GET_SLOT(self,tp_p_cmp_le) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_cmp_le;
  DeeType_GET_SLOT(self,tp_p_cmp_eq) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_cmp_eq;
  DeeType_GET_SLOT(self,tp_p_cmp_ne) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_cmp_ne;
  DeeType_GET_SLOT(self,tp_p_cmp_gr) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_cmp_gr;
  DeeType_GET_SLOT(self,tp_p_cmp_ge) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_cmp_ge;
  DeeType_GET_SLOT(self,tp_p_seq_get) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_seq_get;
  DeeType_GET_SLOT(self,tp_p_seq_del) = (int(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_seq_del;
  DeeType_GET_SLOT(self,tp_p_seq_set) = (int(*)(DeeStructuredTypeObject*,void*,DeeObject*,DeeObject*))&_deearray_tp_p_seq_set;
  DeeType_GET_SLOT(self,tp_p_seq_range_get) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*,DeeObject*))&_deearray_tp_p_seq_range_get;
  DeeType_GET_SLOT(self,tp_p_seq_range_del) = (int(*)(DeeStructuredTypeObject*,void*,DeeObject*,DeeObject*))&_deearray_tp_p_seq_range_del;
  DeeType_GET_SLOT(self,tp_p_seq_range_set) = is_integral_elem
   ? (int(*)(DeeStructuredTypeObject*,void*,DeeObject*,DeeObject*,DeeObject*))&_deeintarray_tp_p_seq_range_set
   : (int(*)(DeeStructuredTypeObject*,void*,DeeObject*,DeeObject*,DeeObject*))&_deearray_tp_p_seq_range_set;
  DeeType_GET_SLOT(self,tp_p_seq_contains) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_seq_contains;
  DeeType_GET_SLOT(self,tp_methods) = _deearray_tp_methods;
 } else {
  DeeType_GET_SLOT(self,tp_p_bool) = (int(*)(DeeStructuredTypeObject*,void*))&_deearray_tp_p_bool_empty;
  DeeType_GET_SLOT(self,tp_p_not) = (DeeObject*(*)(DeeStructuredTypeObject*,void*))&_deearray_tp_p_not_empty;
  DeeType_GET_SLOT(self,tp_p_cmp_lo) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_cmp_lo_empty;
  DeeType_GET_SLOT(self,tp_p_cmp_le) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_cmp_le_empty;
  DeeType_GET_SLOT(self,tp_p_cmp_eq) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_cmp_eq_empty;
  DeeType_GET_SLOT(self,tp_p_cmp_ne) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_cmp_ne_empty;
  DeeType_GET_SLOT(self,tp_p_cmp_gr) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_cmp_gr_empty;
  DeeType_GET_SLOT(self,tp_p_cmp_ge) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_cmp_ge_empty;
  DeeType_GET_SLOT(self,tp_p_seq_get) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_seq_get_empty;
  DeeType_GET_SLOT(self,tp_p_seq_del) = (int(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_seq_del_empty;
  DeeType_GET_SLOT(self,tp_p_seq_set) = (int(*)(DeeStructuredTypeObject*,void*,DeeObject*,DeeObject*))&_deearray_tp_p_seq_set_empty;
  DeeType_GET_SLOT(self,tp_p_seq_range_get) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*,DeeObject*))&_deearray_tp_p_seq_range_get_empty;
  DeeType_GET_SLOT(self,tp_p_seq_range_del) = (int(*)(DeeStructuredTypeObject*,void*,DeeObject*,DeeObject*))&_deearray_tp_p_seq_range_del_empty;
  DeeType_GET_SLOT(self,tp_p_seq_range_set) = (int(*)(DeeStructuredTypeObject*,void*,DeeObject*,DeeObject*,DeeObject*))&_deearray_tp_p_seq_range_set_empty;
  DeeType_GET_SLOT(self,tp_p_seq_contains) = (DeeObject*(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deearray_tp_p_seq_contains_empty;
  DeeType_GET_SLOT(self,tp_methods) = _deearray_tp_methods_empty;
 }
 DeeType_GET_SLOT(self,tp_p_seq_iter_self) = (n && DeeType_GET_SLOT(elem_type,tp_p_instance_size))
  ? (DeeObject*(*)(DeeStructuredTypeObject*,void*))&_deearray_tp_p_seq_iter_self
  : (DeeObject*(*)(DeeStructuredTypeObject*,void*))&_deearray_tp_p_seq_iter_self_empty;
 return 0;
}



static int _deearraytype_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeArrayTypeObject *self) {
 return DeeArrayType_Init(self,&DeeNone_Type,0);
}
static void _deearraytype_tp_dtor(DeeArrayTypeObject *self) {
 Dee_DECREF(self->at_array_base);
}
DEE_VISIT_PROC(_deearraytype_tp_visit,DeeArrayTypeObject *self) {
 Dee_VISIT(self->at_array_base);
}
static int _deearraytype_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeArrayTypeObject *self, DeeObject *args) {
 DeeStructuredTypeObject *base; Dee_size_t size;
 if (DeeTuple_Unpack(args,"oIu:array",&base,&size) != 0) return -1;
 if (DeeError_TypeError_CheckType((DeeObject *)base,&DeeStructuredType_Type) != 0) return -1;
 return DeeArrayType_Init(self,base,size);
}

static struct DeeMemberDef const _deearraytype_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("elem",DeeArrayTypeObject,at_array_base,object),
 DEE_MEMBERDEF_NAMED_RO_v100("count",DeeArrayTypeObject,at_array_elem_count,Dee_size_t),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("at_array_elem_count",DeeArrayTypeObject,at_array_elem_count,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("at_array_elem_size",DeeArrayTypeObject,at_array_elem_size,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("at_array_base",DeeArrayTypeObject,at_array_base,object),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};

DEE_A_RET_OBJECT_EXCEPT_REF(DeeArrayIteratorObject) *DeeArrayIterator_New(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) *elem_type,
 DEE_A_IN void *begin, DEE_A_IN Dee_size_t n_elem) {
 DeeArrayIteratorObject *result;
 DEE_ASSERT(DeeObject_Check(elem_type) && DeeStructuredType_Check(elem_type));
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if ((elem_type = DeeType_LValue(elem_type)) == NULL) return NULL;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if ((result = DeeObject_MALLOCF(
  DeeArrayIteratorObject,"array_iterator (%s: %p*%u)",
  DeeType_NAME(elem_type),begin,(unsigned)n_elem)) != NULL) {
  DeeObject_INIT(result,&DeeArrayIterator_Type);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  Dee_INCREF(result->ai_elem_lv_type = (DeeLValueTypeObject *)elem_type);
  result->ai_elem_size = DeeType_GET_SLOT(DeeLValueType_BASE(elem_type),tp_p_instance_size);
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  Dee_INCREF(result->ai_elem_type = (DeeStructuredTypeObject *)elem_type);
  result->ai_elem_size = DeeType_GET_SLOT(elem_type,tp_p_instance_size);
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  result->ai_end = (result->ai_pos = result->ai_begin = (Dee_uint8_t *)begin)+(n_elem*result->ai_elem_size);
 }
 return (DeeObject *)result;
}

static void _deearrayiterator_tp_dtor(DeeArrayIteratorObject *self) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 Dee_DECREF(self->ai_elem_lv_type);
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 Dee_DECREF(self->ai_elem_type);
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
}
DEE_VISIT_PROC(_deearrayiterator_tp_visit,DeeArrayIteratorObject *self) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 Dee_VISIT(self->ai_elem_lv_type);
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 Dee_VISIT(self->ai_elem_type);
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
}
#define DEE_PRIVATE_MAKE_ARRAY_ITERATOR_COMPARE_OPERATOR(name,op)\
static DeeObject *name(DeeArrayIteratorObject *self, DeeArrayIteratorObject *right) {\
 if (DeeError_TypeError_CheckType((DeeObject *)right,&DeeArrayIterator_Type) != 0) return NULL;\
 DeeReturn_Bool(DeeAtomicPtr_Load(self->ai_pos,memory_order_seq_cst) op \
                DeeAtomicPtr_Load(right->ai_pos,memory_order_seq_cst));\
}
DEE_PRIVATE_MAKE_ARRAY_ITERATOR_COMPARE_OPERATOR(_deearrayiterator_tp_cmp_lo,<)
DEE_PRIVATE_MAKE_ARRAY_ITERATOR_COMPARE_OPERATOR(_deearrayiterator_tp_cmp_le,<=)
DEE_PRIVATE_MAKE_ARRAY_ITERATOR_COMPARE_OPERATOR(_deearrayiterator_tp_cmp_eq,==)
DEE_PRIVATE_MAKE_ARRAY_ITERATOR_COMPARE_OPERATOR(_deearrayiterator_tp_cmp_ne,!=)
DEE_PRIVATE_MAKE_ARRAY_ITERATOR_COMPARE_OPERATOR(_deearrayiterator_tp_cmp_gr,>)
DEE_PRIVATE_MAKE_ARRAY_ITERATOR_COMPARE_OPERATOR(_deearrayiterator_tp_cmp_ge,>=)
#undef DEE_PRIVATE_MAKE_ARRAY_ITERATOR_COMPARE_OPERATOR

static int _deearrayiterator_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeArrayIteratorObject *self, DeeObject *args) {
 DeeTypeObject *elem_type; void *p; Dee_size_t s;
 if (DeeTuple_Unpack(args,"opIu:array.iterator",&elem_type,&p,&s) != 0) return -1;
 if (DeeError_TypeError_CheckType((DeeObject *)elem_type,&DeeStructuredType_Type) != 0) return -1;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if ((self->ai_elem_lv_type = (DeeLValueTypeObject *)DeeType_LValue(elem_type)) == NULL) return -1;
 Dee_INCREF(self->ai_elem_lv_type);
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 Dee_INCREF(self->ai_elem_type = (DeeStructuredTypeObject *)elem_type);
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 self->ai_elem_size = DeeType_GET_SLOT(elem_type,tp_p_instance_size);
 self->ai_end = (self->ai_pos = self->ai_begin = (Dee_uint8_t *)p)+s*self->ai_elem_size;
 return 0;
}
static int _deearrayiterator_tp_seq_iter_next(
 DeeArrayIteratorObject *self, DeeObject **elem) {
 Dee_uint8_t *my_elemaddr;
 my_elemaddr = (Dee_uint8_t *)DeeAtomicPtr_FetchAdd(
  self->ai_pos,self->ai_elem_size,memory_order_seq_cst);
 if (my_elemaddr < self->ai_begin || my_elemaddr >= self->ai_end) return 1; // Out-of-bounds means end
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if ((*elem = DeeLValue_New((DeeTypeObject *)
  self->ai_elem_lv_type,my_elemaddr)) == NULL) return -1;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if ((*elem = DeeStructured_NewFromData((
  DeeTypeObject *)self->ai_elem_type,my_elemaddr)) == NULL) return -1;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 return 0;
}

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deearrayiterator_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */    
static struct DeeMemberDef const _deearrayiterator_tp_members[] = {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DEE_MEMBERDEF_NAMED_RO_v100("__ai_elem_lv_type",DeeArrayIteratorObject,ai_elem_lv_type,object),
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DEE_MEMBERDEF_NAMED_RO_v100("__ai_elem_type",DeeArrayIteratorObject,ai_elem_type,object),
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DEE_MEMBERDEF_NAMED_RO_v100("__ai_elem_size",DeeArrayIteratorObject,ai_elem_size,Dee_size_t),
 DEE_MEMBERDEF_NAMED_v100("__ai_pos",DeeArrayIteratorObject,ai_pos,atomic(p(Dee_uint8_t))),
 DEE_MEMBERDEF_NAMED_RO_v100("__ai_end",DeeArrayIteratorObject,ai_end,p(Dee_uint8_t)),
 DEE_MEMBERDEF_NAMED_RO_v100("__ai_begin",DeeArrayIteratorObject,ai_begin,p(Dee_uint8_t)),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */


static struct DeeTypeMarshal _deearraytype_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALID_ARRAYTYPE),member(NULL),member(NULL)); // Special marshaling

DeeTypeObject DeeArrayType_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("array_type"),null,member(
  DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_CONST_CTOR|
  DEE_TYPE_FLAG_NO_ITERATE_EFFECT|DEE_TYPE_FLAG_FUNDAMENTAL|
  DEE_TYPE_FLAG_NO_SUBCLASS),member(&DeeStructuredType_Type)),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeArrayTypeObject),null,
  member(&_deearraytype_tp_ctor),null,null,
  member(&_deearraytype_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deearraytype_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deearraytype_tp_visit),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deearraytype_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deearraytype_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};
DeeTypeObject DeeArrayIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("array.iterator"),null,member(
  DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_CONST_CTOR|DEE_TYPE_FLAG_FUNDAMENTAL),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeArrayTypeObject),null,null,null,null,
  member(&_deearrayiterator_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deearrayiterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deearrayiterator_tp_visit),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deearrayiterator_tp_cmp_lo),
  member(&_deearrayiterator_tp_cmp_le),
  member(&_deearrayiterator_tp_cmp_eq),
  member(&_deearrayiterator_tp_cmp_ne),
  member(&_deearrayiterator_tp_cmp_gr),
  member(&_deearrayiterator_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,
  member(&_deearrayiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deearrayiterator_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// VarArray 
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DEE_A_RET_EXCEPT_REF DeeVarArrayObject *DEE_CALL DeeVarArray_NewSized(
 DEE_A_INOUT DeeVarArrayTypeObject *tp_self, DEE_A_IN Dee_size_t n_elem) {
 DeeVarArrayObject *result;
 if ((result = (DeeVarArrayObject *)DeeObject_Malloc(Dee_OFFSETOF(
  DeeVarArrayObject,va_data)+(tp_self->vat_array_elem_size*n_elem))) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)tp_self);
  result->va_size = n_elem;
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeVarArray_Resize(
 DEE_A_INOUT DeeVarArrayObject **self, DEE_A_IN Dee_size_t n_elem) {
 DeeVarArrayObject *new_array;
 DeeVarArrayTypeObject *tp_self;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(*self) && DeeVarArray_Check(*self));
 tp_self = (DeeVarArrayTypeObject *)Dee_TYPE(new_array = *self);
 if ((new_array = (DeeVarArrayObject *)DeeObject_Realloc(new_array,Dee_OFFSETOF(
  DeeVarArrayObject,va_data)+(tp_self->vat_array_elem_size*n_elem))) == NULL) return -1;
 *self = new_array;
 new_array->va_size = n_elem;
 return 0;
}


DEE_A_RET_EXCEPT_REF DeeVarArrayObject *DeeVarArray_NewFromSequence(
 DEE_A_INOUT DeeVarArrayTypeObject *tp_self, DEE_A_INOUT DeeObject *sequence) {
 DeeVarArrayObject *result;
 DEE_ASSERT(DeeObject_Check(sequence));
 if ((sequence = DeeObject_IterSelf(sequence)) == NULL) return NULL;
 result = DeeVarArray_NewFromIterator(tp_self,sequence);
 Dee_DECREF(sequence);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeVarArrayObject *DeeVarArray_NewFromIterator(
 DEE_A_INOUT DeeVarArrayTypeObject *tp_self, DEE_A_INOUT DeeObject *iterator) {
 DeeVarArrayObject *result; int temp; DeeObject *elem;
 Dee_size_t used_size,alloc_size,init_pos,elem_size; Dee_uint8_t *init_dst;
 DeeStructuredTypeObject *tp_elem;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeVarArrayType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(iterator));
 if ((temp = DeeObject_IterNextEx(iterator,&elem)) != 0) {
  if (temp < 0) return NULL;
  return DeeVarArray_NewEmpty(tp_self);
 }
 if ((result = DeeVarArray_NewSized(tp_self,2)) == NULL) { Dee_DECREF(elem); return NULL; }
 init_dst = (Dee_uint8_t *)DeeVarArray_DATA(result);
 tp_elem = tp_self->vat_array_base;
 elem_size = DeeType_GET_SLOT(tp_elem,tp_p_instance_size);
 temp = DeeStructured_SetData((DeeTypeObject *)tp_elem,init_dst,elem);
 Dee_DECREF(elem);
 if (temp != 0) {err_r: Dee_DECREF(result); return NULL; }
 alloc_size = 2; used_size = 1;
 // Iterate the full iterator and fill the array
 while ((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  init_dst += elem_size;
  if (used_size == alloc_size) {
   DEE_ASSERT(alloc_size);
   alloc_size *= 2;
   init_pos = (Dee_size_t)(init_dst-(Dee_uint8_t *)DeeVarArray_DATA(result));
   if (DeeVarArray_Resize(&result,alloc_size) != 0) { Dee_DECREF(elem); goto err_r; }
   init_dst = (Dee_uint8_t *)DeeVarArray_DATA(result)+init_pos;
  }
  DEE_ASSERT(used_size < alloc_size);
  temp = DeeStructured_SetData((DeeTypeObject *)tp_elem,init_dst,elem);
  Dee_DECREF(elem);
  if (temp != 0) goto err_r;
  ++used_size;
 }
 if (temp < 0) goto err_r;
 if (used_size != alloc_size) {
  // Flush some memory
  if (DeeVarArray_Resize(&result,used_size) != 0) goto err_r;
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeVarArrayObject *DeeVarArray_NewFrom8BitString(
 DEE_A_INOUT DeeVarArrayTypeObject *tp_self, DEE_A_IN Dee_size_t len,
 DEE_A_IN_R(len) Dee_uint8_t const *data) {
 DeeVarArrayObject *result; Dee_uint8_t *dst;
 if ((result = (DeeVarArrayObject *)DeeObject_Malloc(Dee_OFFSETOF(
  DeeVarArrayObject,va_data)+(len*tp_self->vat_array_elem_size))) == NULL) return NULL;
 DeeObject_INIT(result,(DeeTypeObject *)tp_self);
 result->va_size = len;
 dst = (Dee_uint8_t *)DeeVarArray_DATA(result);
 switch (tp_self->vat_array_elem_size) {
  case sizeof(Dee_uint8_t):  memcpy(dst,data,len*sizeof(Dee_uint8_t)); break;
  case sizeof(Dee_uint16_t): while (len--) *(*(Dee_uint16_t **)&dst)++ = (Dee_uint16_t)*data++; break;
  case sizeof(Dee_uint32_t): while (len--) *(*(Dee_uint32_t **)&dst)++ = (Dee_uint32_t)*data++; break;
  case sizeof(Dee_uint64_t): while (len--) *(*(Dee_uint64_t **)&dst)++ = (Dee_uint64_t)*data++; break;
  default: DEE_BUILTIN_UNREACHABLE();
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeVarArrayObject *DeeVarArray_NewFrom16BitString(
 DEE_A_INOUT DeeVarArrayTypeObject *tp_self, DEE_A_IN Dee_size_t len,
 DEE_A_IN_R(len) Dee_uint16_t const *data) {
 DeeVarArrayObject *result; Dee_uint8_t *dst;
 if DEE_UNLIKELY((result = (DeeVarArrayObject *)DeeObject_Malloc(Dee_OFFSETOF(
  DeeVarArrayObject,va_data)+(len*tp_self->vat_array_elem_size))) == NULL) return NULL;
 DeeObject_INIT(result,(DeeTypeObject *)tp_self);
 result->va_size = len;
 dst = (Dee_uint8_t *)DeeVarArray_DATA(result);
 switch (tp_self->vat_array_elem_size) {
  case sizeof(Dee_uint8_t):  while (len--) *(*(Dee_uint8_t  **)&dst)++ = (Dee_uint8_t )*data++; break;
  case sizeof(Dee_uint16_t): memcpy(dst,data,len*sizeof(Dee_uint16_t)); break;
  case sizeof(Dee_uint32_t): while (len--) *(*(Dee_uint32_t **)&dst)++ = (Dee_uint32_t)*data++; break;
  case sizeof(Dee_uint64_t): while (len--) *(*(Dee_uint64_t **)&dst)++ = (Dee_uint64_t)*data++; break;
  default: DEE_BUILTIN_UNREACHABLE();
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeVarArrayObject *DeeVarArray_NewFrom32BitString(
 DEE_A_INOUT DeeVarArrayTypeObject *tp_self, DEE_A_IN Dee_size_t len,
 DEE_A_IN_R(len) Dee_uint32_t const *data) {
 DeeVarArrayObject *result; Dee_uint8_t *dst;
 if DEE_UNLIKELY((result = (DeeVarArrayObject *)DeeObject_Malloc(Dee_OFFSETOF(
  DeeVarArrayObject,va_data)+(len*tp_self->vat_array_elem_size))) == NULL) return NULL;
 DeeObject_INIT(result,(DeeTypeObject *)tp_self);
 result->va_size = len;
 dst = (Dee_uint8_t *)DeeVarArray_DATA(result);
 switch (tp_self->vat_array_elem_size) {
  case sizeof(Dee_uint8_t):  while (len--) *(*(Dee_uint8_t  **)&dst)++ = (Dee_uint8_t )*data++; break;
  case sizeof(Dee_uint16_t): while (len--) *(*(Dee_uint16_t **)&dst)++ = (Dee_uint16_t)*data++; break;
  case sizeof(Dee_uint32_t): memcpy(dst,data,len*sizeof(Dee_uint32_t)); break;
  case sizeof(Dee_uint64_t): while (len--) *(*(Dee_uint64_t **)&dst)++ = (Dee_uint64_t)*data++; break;
  default: DEE_BUILTIN_UNREACHABLE();
 }
 return result;
}


static DeeVarArrayObject *DEE_CALL _deevararray_tp_new(DeeVarArrayTypeObject *tp_self) {
 return DeeVarArray_NewEmpty(tp_self);
}
static DeeVarArrayObject *DEE_CALL _deevararray_tp_copy_new(
 DeeVarArrayTypeObject *tp_self, DeeVarArrayObject *right) {
 DeeVarArrayObject *result; Dee_size_t elem_size;
 elem_size = right->va_size*tp_self->vat_array_elem_size;
 if DEE_LIKELY((result = (DeeVarArrayObject *)DeeObject_Malloc(
  Dee_OFFSETOF(DeeVarArrayObject,va_data)+elem_size)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)tp_self);
  result->va_size = right->va_size;
  memcpy(DeeVarArray_BYTEV(result),DeeVarArray_BYTEV(right),elem_size);
 }
 return result;
}
static DeeVarArrayObject *DEE_CALL _deevararray_tp_any_new(
 DeeVarArrayTypeObject *tp_self, DeeObject *args) {
 DeeObject *sequence;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:vararray",&sequence) != 0) return NULL;
 return DeeVarArray_NewFromSequence(tp_self,sequence);
}
static DeeVarArrayObject *DEE_CALL _deeintvararray_tp_any_new(
 DeeVarArrayTypeObject *tp_self, DeeObject *args) {
 DeeObject *sequence;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:vararray",&sequence) != 0) return NULL;
#if DEE_CONFIG_HAVE_ENCODING_WIDE
#if DEE_TYPES_SIZEOF_WCHAR_T == 1
 if (DeeWideString_Check(sequence)) return DeeVarArray_NewFrom8BitString(tp_self,DeeWideString_SIZE(sequence),(Dee_uint8_t *)DeeWideString_STR(sequence));
#elif DEE_TYPES_SIZEOF_WCHAR_T == 2
 if (DeeWideString_Check(sequence)) return DeeVarArray_NewFrom16BitString(tp_self,DeeWideString_SIZE(sequence),(Dee_uint16_t *)DeeWideString_STR(sequence));
#elif DEE_TYPES_SIZEOF_WCHAR_T == 4
 if (DeeWideString_Check(sequence)) return DeeVarArray_NewFrom32BitString(tp_self,DeeWideString_SIZE(sequence),(Dee_uint32_t *)DeeWideString_STR(sequence));
#endif /* ... */
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(sequence)) return DeeVarArray_NewFrom8BitString(tp_self,DeeUtf8String_SIZE(sequence),(Dee_uint8_t *)DeeUtf8String_STR(sequence));
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(sequence)) return DeeVarArray_NewFrom16BitString(tp_self,DeeUtf16String_SIZE(sequence),(Dee_uint16_t *)DeeUtf16String_STR(sequence));
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(sequence)) return DeeVarArray_NewFrom32BitString(tp_self,DeeUtf32String_SIZE(sequence),(Dee_uint32_t *)DeeUtf32String_STR(sequence));
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 return DeeVarArray_NewFromSequence(tp_self,sequence);
}
static int DEE_CALL _deeintvararray_tp_any_assign(
 DeeVarArrayObject *self, DeeObject *right) {
 return _DeeArray_AssignIntegralRangeFromSequence(
  (DeeStructuredTypeObject *)DeeVarArrayType_BASE(Dee_TYPE(self)),DeeVarArray_SIZE(self),
  (Dee_uint8_t *)DeeVarArray_ELEM(self),right);
}
static int DEE_CALL _deevararray_tp_any_assign(
 DeeVarArrayObject *self, DeeObject *right) {
 return _DeeArray_AssignRangeFromSequence(
  (DeeStructuredTypeObject *)DeeVarArrayType_BASE(Dee_TYPE(self)),DeeVarArray_SIZE(self),
  (Dee_uint8_t *)DeeVarArray_ELEM(self),right);
}

static DeeObject *DEE_CALL _deevararray_tp_str(DeeVarArrayObject *self) {
 DeeStructuredTypeObject *tp_elem; DeeObject *temp_str,*result;
 Dee_uint8_t *iter,*end; Dee_size_t elem_size; int temp;
 DeeStringWriter writer = DeeStringWriter_INIT();
 end = (iter = (Dee_uint8_t *)DeeVarArray_BYTEV(self))+DeeVarArray_BYTEC(self);
 tp_elem = (DeeStructuredTypeObject *)DeeVarArrayType_BASE(Dee_TYPE(self));
 elem_size = DeeType_GET_SLOT(tp_elem,tp_p_instance_size);
 if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,"[") != 0) goto err;
 if DEE_LIKELY(iter != end) while (1) {
  DEE_ASSERT(iter < end);
  if DEE_UNLIKELY((temp_str = DeeObject_PStr((DeeTypeObject *)tp_elem,iter)) == NULL) goto err;
  temp = DeeStringWriter_WriteStringWithLength(&writer,DeeString_SIZE(temp_str),DeeString_STR(temp_str));
  Dee_DECREF(temp_str);
  if DEE_UNLIKELY(temp != 0) goto err;
  if DEE_UNLIKELY((iter += elem_size) == end) break;
  if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,",") != 0) goto err;
 }
 if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,"]") != 0) goto err;
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}
static DeeObject *DEE_CALL _deevararray_tp_repr(DeeVarArrayObject *self) {
 DeeStructuredTypeObject *tp_elem; DeeObject *temp_str,*result;
 Dee_uint8_t *iter,*end; Dee_size_t elem_size; int temp;
 DeeStringWriter writer = DeeStringWriter_INIT();
 end = (iter = (Dee_uint8_t *)DeeVarArray_BYTEV(self))+DeeVarArray_BYTEC(self);
 tp_elem = (DeeStructuredTypeObject *)DeeVarArrayType_BASE(Dee_TYPE(self));
 elem_size = DeeType_GET_SLOT(tp_elem,tp_p_instance_size);
 if DEE_UNLIKELY(DeeStringWriter_Writef(&writer,"%s[] { ",DeeType_NAME(tp_elem)) != 0) goto err;
 if DEE_LIKELY(iter != end) while (1) {
  DEE_ASSERT(iter < end);
  if DEE_UNLIKELY((temp_str = DeeObject_PStr((DeeTypeObject *)tp_elem,iter)) == NULL) goto err;
  temp = DeeStringWriter_WriteStringWithLength(&writer,DeeString_SIZE(temp_str),DeeString_STR(temp_str));
  Dee_DECREF(temp_str);
  if DEE_UNLIKELY(temp != 0) goto err;
  if DEE_UNLIKELY((iter += elem_size) == end) break;
  if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,",") != 0) goto err;
 }
 if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer," }") != 0) goto err;
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
static DeeObject *DEE_CALL _deevararray_tp_add(DeeVarArrayObject *self, DeeObject *right) {
 Dee_rt_ptrdiff_t diff; DeeTypeObject *pointer_type,*tp_self = Dee_TYPE(self);
 if DEE_UNLIKELY(DeeObject_Cast(Dee_rt_ptrdiff_t,right,&diff) != 0) return NULL;
 if DEE_UNLIKELY((pointer_type = DeeType_Pointer(DeeVarArrayType_BASE(tp_self))) == NULL) return NULL;
 return DeePointer_New(pointer_type,self+(diff*DeeVarArrayType_SELEM(tp_self)));
}
static DeeObject *DEE_CALL _deevararray_tp_sub_zero(
 DeeVarArrayObject *self, DeeObject *right) {
 void *ptr_right; int temp;
 Dee_rt_ptrdiff_t diff; DeeTypeObject *pointer_type,*tp_self = Dee_TYPE(self);
 if DEE_UNLIKELY((temp = DeeObject_TryGetPointer(right,
  DeeVarArrayType_BASE(tp_self),&ptr_right)) < 0) return NULL;
 if DEE_LIKELY(temp == 0) return DeeObject_New(Dee_rt_ptrdiff_t,0);
 if DEE_UNLIKELY(DeeObject_Cast(Dee_rt_ptrdiff_t,right,&diff) != 0) return NULL;
 if DEE_UNLIKELY((pointer_type = DeeType_Pointer(DeeVarArrayType_BASE(tp_self))) == NULL) return NULL;
 return DeePointer_New(pointer_type,self);
}
static DeeObject *DEE_CALL _deevararray_tp_sub(
 DeeVarArrayObject *self, DeeObject *right) {
 void *ptr_right; int temp;
 Dee_rt_ptrdiff_t diff; DeeTypeObject *pointer_type,*tp_self = Dee_TYPE(self);
 if DEE_UNLIKELY((temp = DeeObject_TryGetPointer(right,
  DeeVarArrayType_BASE(tp_self),&ptr_right)) < 0) return NULL;
 if DEE_LIKELY(temp == 0) { // ptr diff
  return DeeObject_New(Dee_rt_ptrdiff_t,(Dee_rt_ptrdiff_t)(
   ((Dee_ssize_t)DeeVarArray_BYTEV(self)-(Dee_ssize_t)ptr_right)/
    (Dee_ssize_t)DeeVarArrayType_SELEM(tp_self)));
 }
 if DEE_UNLIKELY(DeeObject_Cast(Dee_rt_ptrdiff_t,right,&diff) != 0) return NULL;
 if DEE_UNLIKELY((pointer_type = DeeType_Pointer(DeeVarArrayType_BASE(tp_self))) == NULL) return NULL;
 return DeePointer_New(pointer_type,self-(diff*DeeVarArrayType_SELEM(tp_self)));
}
#endif
static DeeObject *DEE_CALL _deevararray_tp_seq_size(DeeVarArrayObject *self) { return DeeObject_New(Dee_size_t,DeeVarArray_SIZE(self)); }
static int DEE_CALL _deevararray_tp_bool(DeeVarArrayObject *self) { return !DeeVarArray_EMPTY(self); }
static DeeObject *DEE_CALL _deevararray_tp_not(DeeVarArrayObject *self) { DeeReturn_Bool(DeeVarArray_EMPTY(self)); }
static DeeObject *DEE_CALL _deevararray_tp_seq_iter_self(DeeVarArrayObject *self) {
 return DeeArrayIterator_New(DeeVarArrayType_BASE(Dee_TYPE(self)),
                             DeeVarArray_ELEM(self),DeeVarArray_SIZE(self));
}
static DeeObject *DEE_CALL _deevararray_tp_cmp_lo(
 DeeVarArrayObject *lhs, DeeVarArrayObject *rhs) {
 if (Dee_TYPE(rhs) == Dee_TYPE(lhs))
  DeeReturn_Bool(Dee_MemCmpLen(
   DeeVarArray_BYTEC(lhs),DeeVarArray_BYTEV(lhs),
   DeeVarArray_BYTEC(rhs),DeeVarArray_BYTEV(rhs)) < 0);
 // todo: better intrinsic support for this:
 return _deegenericiterable_tp_cmp_lo((DeeObject *)lhs,(DeeObject *)rhs);
}
static DeeObject *DEE_CALL _deevararray_tp_cmp_le(
 DeeVarArrayObject *lhs, DeeVarArrayObject *rhs) {
 if (Dee_TYPE(rhs) == Dee_TYPE(lhs))
  DeeReturn_Bool(Dee_MemCmpLen(
   DeeVarArray_BYTEC(lhs),DeeVarArray_BYTEV(lhs),
   DeeVarArray_BYTEC(rhs),DeeVarArray_BYTEV(rhs)) <= 0);
 // todo: better intrinsic support for this:
 return _deegenericiterable_tp_cmp_le((DeeObject *)lhs,(DeeObject *)rhs);
}
static DeeObject *DEE_CALL _deevararray_tp_cmp_eq(
 DeeVarArrayObject *lhs, DeeVarArrayObject *rhs) {
 if (Dee_TYPE(rhs) == Dee_TYPE(lhs))
  DeeReturn_Bool(Dee_MemCmpLen(
   DeeVarArray_BYTEC(lhs),DeeVarArray_BYTEV(lhs),
   DeeVarArray_BYTEC(rhs),DeeVarArray_BYTEV(rhs)) == 0);
 // todo: better intrinsic support for this:
 return _deegenericiterable_tp_cmp_eq((DeeObject *)lhs,(DeeObject *)rhs);
}
static DeeObject *DEE_CALL _deevararray_tp_cmp_ne(
 DeeVarArrayObject *lhs, DeeVarArrayObject *rhs) {
 if (Dee_TYPE(rhs) == Dee_TYPE(lhs))
  DeeReturn_Bool(Dee_MemCmpLen(
   DeeVarArray_BYTEC(lhs),DeeVarArray_BYTEV(lhs),
   DeeVarArray_BYTEC(rhs),DeeVarArray_BYTEV(rhs)) != 0);
 // todo: better intrinsic support for this:
 return _deegenericiterable_tp_cmp_ne((DeeObject *)lhs,(DeeObject *)rhs);
}
static DeeObject *DEE_CALL _deevararray_tp_cmp_gr(
 DeeVarArrayObject *lhs, DeeVarArrayObject *rhs) {
 if (Dee_TYPE(rhs) == Dee_TYPE(lhs))
  DeeReturn_Bool(Dee_MemCmpLen(
   DeeVarArray_BYTEC(lhs),DeeVarArray_BYTEV(lhs),
   DeeVarArray_BYTEC(rhs),DeeVarArray_BYTEV(rhs)) > 0);
 // todo: better intrinsic support for this:
 return _deegenericiterable_tp_cmp_gr((DeeObject *)lhs,(DeeObject *)rhs);
}
static DeeObject *DEE_CALL _deevararray_tp_cmp_ge(
 DeeVarArrayObject *lhs, DeeVarArrayObject *rhs) {
 if (Dee_TYPE(rhs) == Dee_TYPE(lhs))
  DeeReturn_Bool(Dee_MemCmpLen(
   DeeVarArray_BYTEC(lhs),DeeVarArray_BYTEV(lhs),
   DeeVarArray_BYTEC(rhs),DeeVarArray_BYTEV(rhs)) >= 0);
 // todo: better intrinsic support for this:
 return _deegenericiterable_tp_cmp_ge((DeeObject *)lhs,(DeeObject *)rhs);
}
static DeeObject *DEE_CALL _deevararray_tp_seq_get(
 DeeVarArrayObject *self, DeeObject *i) {
 Dee_ssize_t ii; Dee_size_t my_size;
 DeeTypeObject *tp_self = Dee_TYPE(self);
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return NULL;
 if DEE_UNLIKELY(DeeVarArray_EMPTY(self)) { DeeError_ValueError_EmptyList(tp_self); return NULL; }
 my_size = DeeVarArray_SIZE(self);
 DEE_PRIVATE_CLAMP_INDEX(ii,my_size);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 {
  DeeTypeObject *elem_lval;
  if DEE_UNLIKELY((elem_lval = DeeType_LValue(DeeVarArrayType_BASE(tp_self))) == NULL) return NULL;
  return DeeLValue_New(elem_lval,DeeVarArray_GET(self,(Dee_size_t)ii));
 }
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 return DeeStructured_NewFromData(DeeVarArrayType_BASE(tp_self),DeeVarArray_GET(self,(Dee_size_t)ii));
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
}
static int DEE_CALL _deevararray_tp_seq_del(
 DeeVarArrayObject *self, DeeObject *i) {
 Dee_ssize_t ii; Dee_size_t my_size;
 DeeTypeObject *tp_self = Dee_TYPE(self);
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return -1;
 if DEE_UNLIKELY(DeeVarArray_EMPTY(self)) { DeeError_ValueError_EmptyList(tp_self); return -1; }
 my_size = DeeVarArray_SIZE(self);
 DEE_PRIVATE_CLAMP_INDEX(ii,my_size);
 DeeVarArray_DEL(self,(Dee_size_t)ii);
 return 0;
}
static int DEE_CALL _deevararray_tp_seq_set(
 DeeVarArrayObject *self, DeeObject *i, DeeObject *v) {
 Dee_ssize_t ii; Dee_size_t my_size;
 DeeTypeObject *tp_self = Dee_TYPE(self);
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return -1;
 if DEE_UNLIKELY(DeeVarArray_EMPTY(self)) { DeeError_ValueError_EmptyList(tp_self); return -1; }
 my_size = DeeVarArray_SIZE(self);
 DEE_PRIVATE_CLAMP_INDEX(ii,my_size);
 return DeeStructured_SetData(DeeVarArrayType_BASE(tp_self),
                              DeeVarArray_GET(self,(Dee_size_t)ii),v);
}
static DeeVarArrayObject *DEE_CALL _deevararray_tp_seq_range_get(
 DeeVarArrayObject *self, DeeObject *lo, DeeObject *hi) {
 DeeVarArrayObject *result;
 Dee_ssize_t temp; Dee_size_t used_lo,used_hi,max_size;
 DeeVarArrayTypeObject *tp_self = (DeeVarArrayTypeObject *)Dee_TYPE(self);
 if (lo == Dee_None) used_lo = 0; else {
  if (DeeObject_Cast(Dee_ssize_t,lo,&temp) != 0) return NULL;
  used_lo = temp < 0 ? (Dee_size_t)0 : (Dee_size_t)temp;
 }
 if (hi == Dee_None) used_hi = (Dee_size_t)-1; else {
  if (DeeObject_Cast(Dee_ssize_t,hi,&temp) != 0) return NULL;
  used_hi = temp < 0 ? (Dee_size_t)0 : (Dee_size_t)temp;
 }
 max_size = DeeVarArray_SIZE(self);
 if DEE_UNLIKELY(used_lo > max_size) used_lo = max_size;
 if DEE_UNLIKELY(used_hi > max_size) used_hi = max_size;
 if DEE_UNLIKELY(used_lo > used_hi) used_lo = used_hi;
 max_size = (Dee_size_t)(used_hi-used_lo);
 if DEE_UNLIKELY((result = DeeVarArray_NewSized(tp_self,max_size)) == NULL) return NULL;
 memcpy(DeeVarArray_BYTEV(result),DeeVarArray_BYTEV(self),
        max_size*tp_self->vat_array_elem_size);
 return result;
}
static int DEE_CALL _deevararray_tp_seq_range_del(
 DeeVarArrayObject *self, DeeObject *lo, DeeObject *hi) {
 Dee_ssize_t temp; Dee_size_t used_lo,used_hi,max_size;
 DeeVarArrayTypeObject *tp_self = (DeeVarArrayTypeObject *)Dee_TYPE(self);
 if (lo == Dee_None) used_lo = 0; else {
  if (DeeObject_Cast(Dee_ssize_t,lo,&temp) != 0) return -1;
  used_lo = temp < 0 ? (Dee_size_t)0 : (Dee_size_t)temp;
 }
 if (hi == Dee_None) used_hi = (Dee_size_t)-1; else {
  if (DeeObject_Cast(Dee_ssize_t,hi,&temp) != 0) return -1;
  used_hi = temp < 0 ? (Dee_size_t)0 : (Dee_size_t)temp;
 }
 max_size = DeeVarArray_SIZE(self);
 if DEE_UNLIKELY(used_lo > max_size) used_lo = max_size;
 if DEE_UNLIKELY(used_hi > max_size) used_hi = max_size;
 if DEE_UNLIKELY(used_lo > used_hi) used_lo = used_hi;
 memset(DeeVarArray_GET(self,used_lo),0,
        (Dee_size_t)(used_hi-used_lo)*
        tp_self->vat_array_elem_size);
 return 0;
}
static int DEE_CALL _deevararray_tp_seq_range_set(
 DeeVarArrayObject *self, DeeObject *lo, DeeObject *hi, DeeObject *v) {
 Dee_ssize_t temp; Dee_size_t used_lo,used_hi,max_size;
 DeeVarArrayTypeObject *tp_self = (DeeVarArrayTypeObject *)Dee_TYPE(self);
 if (lo == Dee_None) used_lo = 0; else {
  if (DeeObject_Cast(Dee_ssize_t,lo,&temp) != 0) return -1;
  used_lo = temp < 0 ? (Dee_size_t)0 : (Dee_size_t)temp;
 }
 if (hi == Dee_None) used_hi = (Dee_size_t)-1; else {
  if (DeeObject_Cast(Dee_ssize_t,hi,&temp) != 0) return -1;
  used_hi = temp < 0 ? (Dee_size_t)0 : (Dee_size_t)temp;
 }
 max_size = DeeVarArray_SIZE(self);
 if DEE_UNLIKELY(used_lo > max_size) used_lo = max_size;
 if DEE_UNLIKELY(used_hi > max_size) used_hi = max_size;
 if DEE_UNLIKELY(used_lo > used_hi) used_lo = used_hi;
 return _DeeArray_AssignRangeFromSequence(
  (DeeStructuredTypeObject *)DeeVarArrayType_BASE(tp_self),
  (Dee_size_t)(used_hi-used_lo),(Dee_uint8_t *)DeeVarArray_GET(self,used_lo),v);
}
static int DEE_CALL _deeintvararray_tp_seq_range_set(
 DeeVarArrayObject *self, DeeObject *lo, DeeObject *hi, DeeObject *v) {
 Dee_ssize_t temp; Dee_size_t used_lo,used_hi,max_size;
 DeeVarArrayTypeObject *tp_self = (DeeVarArrayTypeObject *)Dee_TYPE(self);
 if (lo == Dee_None) used_lo = 0; else {
  if (DeeObject_Cast(Dee_ssize_t,lo,&temp) != 0) return -1;
  used_lo = temp < 0 ? (Dee_size_t)0 : (Dee_size_t)temp;
 }
 if (hi == Dee_None) used_hi = (Dee_size_t)-1; else {
  if (DeeObject_Cast(Dee_ssize_t,hi,&temp) != 0) return -1;
  used_hi = temp < 0 ? (Dee_size_t)0 : (Dee_size_t)temp;
 }
 max_size = DeeVarArray_SIZE(self);
 if DEE_UNLIKELY(used_lo > max_size) used_lo = max_size;
 if DEE_UNLIKELY(used_hi > max_size) used_hi = max_size;
 if DEE_UNLIKELY(used_lo > used_hi) used_lo = used_hi;
 return _DeeArray_AssignIntegralRangeFromSequence(
  (DeeStructuredTypeObject *)DeeVarArrayType_BASE(tp_self),
  (Dee_size_t)(used_hi-used_lo),(Dee_uint8_t *)DeeVarArray_GET(self,used_lo),v);
}


static struct DeeMemberDef const _deevararray_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeArrayIterator_Type),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMethodDef const _deevararray_tp_methods[] = {
 // TODO: Intrinsic versions of these
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




DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(NULL) char *)
_DeeVarArrayType_AllocName(DEE_A_IN_Z char const *base_name) {
 char *result; DeeStringWriter writer = DeeStringWriter_INIT();
 if DEE_UNLIKELY(DeeStringWriter_WriteString(&writer,base_name) != 0 ||
                 DeeStringWriter_WRITE_STRING(&writer,"[]") != 0) goto err;
 result = _DeeStringWriter_PackCString(&writer,NULL);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeVarArrayType_Init(
 DeeVarArrayTypeObject *self, DeeStructuredTypeObject *base) {
 int is_integral_elem;
 DEE_ASSERT(DeeObject_Check(self) && DeeVarArrayType_Check(self));
 DEE_ASSERT(DeeObject_Check(base) && DeeStructuredType_Check(base));
 if DEE_UNLIKELY((*DeeType_GET_SLOT(&DeeType_Type,tp_ctor))(&DeeStructuredType_Type,(DeeObject *)self) != 0) return -1;
 if DEE_UNLIKELY((DeeType_GET_SLOT(self,tp_name) = _DeeVarArrayType_AllocName(DeeType_NAME(base))) == NULL) {
  (*DeeType_GET_SLOT(&DeeStructuredType_Type,tp_dtor))((DeeObject *)self);
  (*DeeType_GET_SLOT(&DeeType_Type,tp_dtor))((DeeObject *)self);
  return -1;
 }
 DeeType_GET_SLOT(self,tp_flags) &= ~DEE_TYPE_FLAG_STATIC_NAME;
 DeeType_GET_SLOT(self,tp_flags) |= DEE_TYPE_FLAG_VAR_OBJECT;
 self->vat_prev_structured_object_type = NULL;
 self->vat_array_elem_size = DeeType_GET_SLOT(base,tp_p_instance_size);
 Dee_INCREF(self->vat_array_base = base);
 DeeType_GET_SLOT(self,tp_instance_size) = 0;
 DeeType_GET_SLOT(self,tp_class_members) = _deevararray_tp_class_members;
 is_integral_elem = _DeeType_IsIntegral((DeeTypeObject *)base);
 DeeType_GET_SLOT(self,tp_new) = (DeeObject*(DEE_CALL*)(DeeTypeObject*))&_deevararray_tp_new;
 DeeType_GET_SLOT(self,tp_copy_new) = (DeeObject*(DEE_CALL*)(DeeTypeObject*,DeeObject*))&_deevararray_tp_copy_new;
 DeeType_GET_SLOT(self,tp_move_new) = (DeeObject*(DEE_CALL*)(DeeTypeObject*,DeeObject*))&_deevararray_tp_copy_new;
 if (is_integral_elem) {
  DeeType_GET_SLOT(self,tp_any_new) = (DeeObject*(DEE_CALL*)(DeeTypeObject*,DeeObject*))&_deeintvararray_tp_any_new;
  DeeType_GET_SLOT(self,tp_any_assign) = (int(*)(DeeObject*,DeeObject*))&_deeintvararray_tp_any_assign;
 } else {
  DeeType_GET_SLOT(self,tp_any_new) = (DeeObject*(DEE_CALL*)(DeeTypeObject*,DeeObject*))&_deevararray_tp_any_new;
  DeeType_GET_SLOT(self,tp_any_assign) = (int(*)(DeeObject*,DeeObject*))&_deevararray_tp_any_assign;
 }
 DeeType_GET_SLOT(self,tp_str) = (DeeObject*(DEE_CALL*)(DeeObject*))&_deevararray_tp_str;
 DeeType_GET_SLOT(self,tp_repr) = (DeeObject*(DEE_CALL*)(DeeObject*))&_deevararray_tp_repr;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeType_GET_SLOT(self,tp_add) = (DeeObject*(DEE_CALL*)(DeeObject*,DeeObject*))&_deevararray_tp_add;
 DeeType_GET_SLOT(self,tp_sub) = DeeType_GET_SLOT(base,tp_p_instance_size)
  ? (DeeObject*(DEE_CALL*)(DeeObject*,DeeObject*))&_deevararray_tp_sub
  : (DeeObject*(DEE_CALL*)(DeeObject*,DeeObject*))&_deevararray_tp_sub_zero;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DeeType_GET_SLOT(self,tp_seq_size) = (DeeObject*(DEE_CALL*)(DeeObject*))&_deevararray_tp_seq_size;
 DeeType_GET_SLOT(self,tp_bool) = (int(DEE_CALL*)(DeeObject*))&_deevararray_tp_bool;
 DeeType_GET_SLOT(self,tp_not) = (DeeObject*(DEE_CALL*)(DeeObject*))&_deevararray_tp_not;
 DeeType_GET_SLOT(self,tp_cmp_lo) = (DeeObject*(DEE_CALL*)(DeeObject*,DeeObject*))&_deevararray_tp_cmp_lo;
 DeeType_GET_SLOT(self,tp_cmp_le) = (DeeObject*(DEE_CALL*)(DeeObject*,DeeObject*))&_deevararray_tp_cmp_le;
 DeeType_GET_SLOT(self,tp_cmp_eq) = (DeeObject*(DEE_CALL*)(DeeObject*,DeeObject*))&_deevararray_tp_cmp_eq;
 DeeType_GET_SLOT(self,tp_cmp_ne) = (DeeObject*(DEE_CALL*)(DeeObject*,DeeObject*))&_deevararray_tp_cmp_ne;
 DeeType_GET_SLOT(self,tp_cmp_gr) = (DeeObject*(DEE_CALL*)(DeeObject*,DeeObject*))&_deevararray_tp_cmp_gr;
 DeeType_GET_SLOT(self,tp_cmp_ge) = (DeeObject*(DEE_CALL*)(DeeObject*,DeeObject*))&_deevararray_tp_cmp_ge;
 DeeType_GET_SLOT(self,tp_seq_get) = (DeeObject*(DEE_CALL*)(DeeObject*,DeeObject*))&_deevararray_tp_seq_get;
 DeeType_GET_SLOT(self,tp_seq_del) = (int(DEE_CALL*)(DeeObject*,DeeObject*))&_deevararray_tp_seq_del;
 DeeType_GET_SLOT(self,tp_seq_set) = (int(DEE_CALL*)(DeeObject*,DeeObject*,DeeObject*))&_deevararray_tp_seq_set;
 DeeType_GET_SLOT(self,tp_seq_range_get) = (DeeObject*(DEE_CALL*)(DeeObject*,DeeObject*,DeeObject*))&_deevararray_tp_seq_range_get;
 DeeType_GET_SLOT(self,tp_seq_range_del) = (int(DEE_CALL*)(DeeObject*,DeeObject*,DeeObject*))&_deevararray_tp_seq_range_del;
 DeeType_GET_SLOT(self,tp_seq_range_set) = is_integral_elem
  ? (int(DEE_CALL*)(DeeObject*,DeeObject*,DeeObject*,DeeObject*))&_deeintvararray_tp_seq_range_set
  : (int(DEE_CALL*)(DeeObject*,DeeObject*,DeeObject*,DeeObject*))&_deevararray_tp_seq_range_set;
 DeeType_GET_SLOT(self,tp_seq_contains) = (DeeObject*(*)(DeeObject*,DeeObject*))&_deegenericiterable_tp_seq_contains; // TODO: intrinsic version
 DeeType_GET_SLOT(self,tp_methods) = _deevararray_tp_methods;
 DeeType_GET_SLOT(self,tp_seq_iter_self) = (DeeObject*(DEE_CALL*)(DeeObject*))&_deevararray_tp_seq_iter_self;
 return 0;
}

DEE_A_RET_EXCEPT_REF DeeVarArrayTypeObject *_DeeVarArrayType_New(
 DEE_A_INOUT DeeStructuredTypeObject *base) {
 DeeVarArrayTypeObject *result;
 DEE_ASSERT(DeeObject_Check(base) && DeeStructuredType_Check(base));
 if DEE_LIKELY((result = DeeObject_MALLOC(DeeVarArrayTypeObject)) != NULL) {
  DeeObject_INIT(result,&DeeVarArrayType_Type);
  if (DeeVarArrayType_Init(result,base) != 0) {
   _DeeObject_DELETE(&DeeVarArrayType_Type,result);
   result = NULL;
  }
 }
 return result;
}

static int _deevararraytype_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeVarArrayTypeObject *self) {
 return DeeVarArrayType_Init(self,&DeeNone_Type);
}
static int _deevararraytype_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeVarArrayTypeObject *self, DeeObject *args) {
 DeeStructuredTypeObject *base;
 if (DeeTuple_Unpack(args,"o:vararray_type",&base) != 0) return -1;
 if (DeeError_TypeError_CheckType((DeeObject *)base,&DeeStructuredType_Type) != 0) return -1;
 return DeeVarArrayType_Init(self,base);
}
static void _deevararraytype_tp_dtor(DeeVarArrayTypeObject *self) {
 Dee_DECREF(self->vat_array_base);
}
DEE_VISIT_PROC(_deevararraytype_tp_visit,DeeVarArrayTypeObject *self) {
 Dee_VISIT(self->vat_array_base);
}


static struct DeeTypeMarshal _deevararraytype_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALID_VARARRAYTYPE),member(NULL),member(NULL)); // Special marshaling

static struct DeeMemberDef const _deevararraytype_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("elem",DeeVarArrayTypeObject,vat_array_base,object),
#ifdef DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__vat_array_elem_size",DeeVarArrayTypeObject,vat_array_elem_size,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__vat_array_base",DeeVarArrayTypeObject,vat_array_base,object),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};

DeeTypeObject DeeVarArrayType_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("vararray_type"),null,member(
  DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_CONST_CTOR|
  DEE_TYPE_FLAG_NO_ITERATE_EFFECT|DEE_TYPE_FLAG_FUNDAMENTAL|
  DEE_TYPE_FLAG_NO_SUBCLASS),
  member(&DeeType_Type)),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeVarArrayTypeObject),null,
  member(&_deevararraytype_tp_ctor),null,null,
  member(&_deevararraytype_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deevararraytype_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deevararraytype_tp_visit),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deevararraytype_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deevararraytype_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};



DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */

#endif /* !GUARD_DEEMON_ARRAY_C */
