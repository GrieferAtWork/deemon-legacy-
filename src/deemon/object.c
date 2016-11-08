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
#ifndef GUARD_DEEMON_OBJECT_C
#define GUARD_DEEMON_OBJECT_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include <deemon/array.h>
#include <deemon/bool.h>
#include <deemon/cell.h>
#include <deemon/cfunction.h>
#include <deemon/class.h>
#include <deemon/deemonrun.h>
#include <deemon/dex.h>
#include <deemon/dict.h>
#include <deemon/error.h>
#include <deemon/gc.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/list.h>
#include <deemon/marshal_data.h>
#include <deemon/memberdef.h>
#include <deemon/memberfunction.h>
#include <deemon/mp/thread.h>
#include <deemon/none.h>
#include <deemon/object.h>
#include <deemon/optional/atomic_once.h>
#include <deemon/optional/object_doc.h>
#include <deemon/optional/object_io.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/hash.h>
#include <deemon/optional/type_decl.h>
#include <deemon/optional/std/string.h>
#include <deemon/runtime/function.h>
#include <deemon/set.h>
#include <deemon/struct.h>
#include <deemon/structured.h>
#include <deemon/super.h>
#include <deemon/tuple.h>
#include <deemon/type.h>
#include <deemon/value.h>
#include <deemon/signalhandler.h>
#include DEE_INCLUDE_MEMORY_API()


// >>> !!!!!!!!! <<<
// !!! IMPORTANT !!!
// >>> !!!!!!!!! <<<
// If any of these assertions fail, you have to adjust the
// specific type size macro in <deemon/__features.inl>
// DON'T disable the assertion, because they're here for a reason!
DEE_STATIC_ASSERT(sizeof(char) == DEE_TYPES_SIZEOF_CHAR);
DEE_STATIC_ASSERT(sizeof(signed char) == DEE_TYPES_SIZEOF_CHAR);
DEE_STATIC_ASSERT(sizeof(unsigned char) == DEE_TYPES_SIZEOF_CHAR);
DEE_STATIC_ASSERT(sizeof(short) == DEE_TYPES_SIZEOF_SHORT);
DEE_STATIC_ASSERT(sizeof(unsigned short) == DEE_TYPES_SIZEOF_SHORT);
DEE_STATIC_ASSERT(sizeof(int) == DEE_TYPES_SIZEOF_INT);
DEE_STATIC_ASSERT(sizeof(unsigned int) == DEE_TYPES_SIZEOF_INT);
DEE_STATIC_ASSERT(sizeof(long) == DEE_TYPES_SIZEOF_LONG);
DEE_STATIC_ASSERT(sizeof(unsigned long) == DEE_TYPES_SIZEOF_LONG);
#ifdef DEE_TYPES_SIZEOF_LLONG
DEE_STATIC_ASSERT(sizeof(long long) == DEE_TYPES_SIZEOF_LLONG);
DEE_STATIC_ASSERT(sizeof(unsigned long long) == DEE_TYPES_SIZEOF_LLONG);
#endif
DEE_STATIC_ASSERT(sizeof(void *) == DEE_TYPES_SIZEOF_POINTER);
#ifdef DEE_TYPES_SIZEOF_FLOAT
DEE_STATIC_ASSERT(sizeof(float) == DEE_TYPES_SIZEOF_FLOAT);
#endif
#ifdef DEE_TYPES_SIZEOF_DOUBLE
DEE_STATIC_ASSERT(sizeof(double) == DEE_TYPES_SIZEOF_DOUBLE);
#endif
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
DEE_STATIC_ASSERT(sizeof(long double) == DEE_TYPES_SIZEOF_LDOUBLE);
#endif

// Make sure all our type sizes are correct
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDINT_H
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#include <stdint.h>
#include DEE_INCLUDE_MEMORY_API_ENABLE()
DEE_STATIC_ASSERT(sizeof(int_fast8_t) == DEE_TYPES_SIZEOF_INT_FAST8_T);
DEE_STATIC_ASSERT(sizeof(int_fast16_t) == DEE_TYPES_SIZEOF_INT_FAST16_T);
DEE_STATIC_ASSERT(sizeof(int_fast32_t) == DEE_TYPES_SIZEOF_INT_FAST32_T);
DEE_STATIC_ASSERT(sizeof(int_fast64_t) == DEE_TYPES_SIZEOF_INT_FAST64_T);
DEE_STATIC_ASSERT(sizeof(int_least8_t) == DEE_TYPES_SIZEOF_INT_LEAST8_T);
DEE_STATIC_ASSERT(sizeof(int_least16_t) == DEE_TYPES_SIZEOF_INT_LEAST16_T);
DEE_STATIC_ASSERT(sizeof(int_least32_t) == DEE_TYPES_SIZEOF_INT_LEAST32_T);
DEE_STATIC_ASSERT(sizeof(int_least64_t) == DEE_TYPES_SIZEOF_INT_LEAST64_T);
DEE_STATIC_ASSERT(sizeof(uint_fast8_t) == DEE_TYPES_SIZEOF_INT_FAST8_T);
DEE_STATIC_ASSERT(sizeof(uint_fast16_t) == DEE_TYPES_SIZEOF_INT_FAST16_T);
DEE_STATIC_ASSERT(sizeof(uint_fast32_t) == DEE_TYPES_SIZEOF_INT_FAST32_T);
DEE_STATIC_ASSERT(sizeof(uint_fast64_t) == DEE_TYPES_SIZEOF_INT_FAST64_T);
DEE_STATIC_ASSERT(sizeof(uint_least8_t) == DEE_TYPES_SIZEOF_INT_LEAST8_T);
DEE_STATIC_ASSERT(sizeof(uint_least16_t) == DEE_TYPES_SIZEOF_INT_LEAST16_T);
DEE_STATIC_ASSERT(sizeof(uint_least32_t) == DEE_TYPES_SIZEOF_INT_LEAST32_T);
DEE_STATIC_ASSERT(sizeof(uint_least64_t) == DEE_TYPES_SIZEOF_INT_LEAST64_T);
#endif

#include "object.impl.call_operators.inl"
#include "object.impl.castf.inl"
#include "object.impl.inplace_var_operators.inl"
#include "object.impl.integer_operators.inl"
#include "object.impl.marshal.inl"
#include "object.impl.operator_attrib.inl"
#include "object.impl.operators.inl"
#include "object.impl.string_operators.inl"
#include "object.util.iterate.inl"


DEE_DECL_BEGIN

DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeTypeObject *
Dee_Type(DEE_A_IN DeeObject const *self) {
 DEE_ASSERT(DeeObject_Check(self));
 return Dee_TYPE(self);
}

#ifndef __INTELLISENSE__
#include "object.core.reference.inl"
#include "object.core.destroy.inl"
#include "object.generic_attribute.inl"
#include "object.deep_equals.inl"
#endif



DEE_A_RET_WUNUSED int DeeObject_Is(
 DEE_A_IN DeeObject const *ob,
 DEE_A_IN DeeTypeObject const *tp) {
 DeeTypeObject const *ob_type;
 DEE_ASSERT(DeeObject_Check(ob));
 DEE_ASSERT(DeeObject_Check(tp));
 if DEE_UNLIKELY(!DeeType_Check(tp)) {
  if DEE_LIKELY(DeeNone_Check(tp)) return DeeNone_Check(ob); // Allow for 'x is none' checks
  return 0; // 'tp' isn't a type
 }
 ob_type = Dee_TYPE(ob);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (DeeLValueType_Check(ob_type) && !DeeLValueType_Check(tp))
  ob_type = DeeLValueType_BASE(ob_type);
 else
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (ob_type == &DeeSuper_Type && tp != &DeeSuper_Type)
  ob_type = DeeSuper_TYPE(ob);
 return DeeType_IsSameOrDerived(ob_type,tp);
}


DEE_A_RET_EXCEPT(NULL) DeeObject *DeeObject_TGetInstance(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeObject *self,
 DEE_A_IN DeeTypeObject const *tp_instance) {
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(tp_instance) && DeeType_Check(tp_instance));
 // NOTE: Perform the tp_self == tp_instance check inline to make this call cheaper
 if DEE_LIKELY(DEE_LIKELY(tp_self == tp_instance) ||
               DeeType_IsDerived(tp_self,tp_instance)
               ) return self; // Everything's already OK
 if DEE_LIKELY(tp_self == &DeeSuper_Type) return DeeObject_TGetInstance(
  DeeSuper_TYPE(self),DeeSuper_SELF(self),tp_instance);
 // Unexpected type
 DeeError_TypeError_UnexpectedType(self,(DeeTypeObject *)tp_instance);
 return NULL;
}




DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TDeepCopy(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeTuple_Type) return DeeTuple_DeepCopy(self);
 if (tp_self == &DeeList_Type) return DeeList_DeepCopy(self);
 if (tp_self == &DeeDict_Type) return DeeDict_DeepCopy(self);
 if (tp_self == &DeeSet_Type) return DeeSet_DeepCopy(self);
 if (tp_self == &DeeCell_Type) return DeeCell_DeepCopy(self);
 if (DeeType_MustCopy(tp_self)) return DeeObject_TCopy(tp_self,self);
 DeeReturn_NEWREF(self);
}


void _DeeObject_Delete(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_IN DEE_A_OUT_INVALID DeeObject *self) {
 _DeeObject_DELETE(tp_self,self);
}



DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeTypeObject *DeeObject_TClassOf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeObject const *self) {
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeSuper_Type) {
  do {
   tp_self = DeeSuper_TYPE(self);
   self = DeeSuper_SELF(self);
  } while DEE_UNLIKELY(tp_self == &DeeSuper_Type);
  return (DeeTypeObject *)tp_self;
 }
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (DeeLValueType_Check(tp_self)) return DeeLValueType_BASE(tp_self);
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 return (DeeTypeObject *)tp_self;
}

#define DEE_PRIVATE_MAKE_TCOMPARE_FUNC(name,slot)\
DEE_A_RET_EXCEPT(-1) int name(\
 DEE_A_IN DeeTypeObject const *tp_lhs,\
 DEE_A_INOUT DeeObject *lhs,\
 DEE_A_INOUT DeeObject *rhs) {\
 int result; DeeObject *result_ob;\
 result_ob = name##Object(tp_lhs,lhs,rhs);\
 if DEE_LIKELY(result_ob) {\
  result = DeeObject_Bool(result_ob);\
  Dee_DECREF(result_ob);\
 } else result = -1;\
 return result;\
}\
DEE_A_RET_EXCEPT_REF DeeObject *name##Object(\
 DEE_A_IN DeeTypeObject const *tp_lhs,\
 DEE_A_INOUT DeeObject *lhs,\
 DEE_A_INOUT DeeObject *rhs) {\
 DeeTypeObject const *tp_iter;\
 DEE_ASSERT(DeeObject_Check(lhs));\
 DEE_ASSERT(DeeObject_Check(rhs));\
 DEE_ASSERT(DeeObject_Check(tp_lhs) && DeeType_Check(tp_lhs));\
 DEE_ASSERT(DeeObject_InstanceOf(lhs,tp_lhs));\
 tp_iter = tp_lhs; DeeType_FIND_SLOT(tp_iter,slot);\
 return (*DeeType_GET_SLOT(tp_iter,slot))(lhs,rhs);\
}
DEE_PRIVATE_MAKE_TCOMPARE_FUNC(DeeObject_TCompareLo,tp_cmp_lo)
DEE_PRIVATE_MAKE_TCOMPARE_FUNC(DeeObject_TCompareLe,tp_cmp_le)
DEE_PRIVATE_MAKE_TCOMPARE_FUNC(DeeObject_TCompareEq,tp_cmp_eq)
DEE_PRIVATE_MAKE_TCOMPARE_FUNC(DeeObject_TCompareNe,tp_cmp_ne)
DEE_PRIVATE_MAKE_TCOMPARE_FUNC(DeeObject_TCompareGr,tp_cmp_gr)
DEE_PRIVATE_MAKE_TCOMPARE_FUNC(DeeObject_TCompareGe,tp_cmp_ge)
#undef DEE_PRIVATE_MAKE_TCOMPARE_FUNC

#define DEE_PRIVATE_MAKE_PCOMPARE_FUNC(name,slot)\
DEE_A_RET_EXCEPT(-1) int name(\
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self,\
 DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) {\
 int result; DeeObject *result_ob;\
 result_ob = name##Object(lhs_tp_self,lhs_base_addr,rhs);\
 if DEE_LIKELY(result_ob) {\
  result = DeeObject_Bool(result_ob);\
  Dee_DECREF(result_ob);\
 } else result = -1;\
 return result;\
}\
DEE_A_RET_EXCEPT_REF DeeObject *name##Object(\
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self,\
 DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) {\
 DEE_ASSERT(DeeObject_Check(lhs_tp_self) && DeeStructuredType_Check(lhs_tp_self));\
 DEE_ASSERT(DeeObject_Check(rhs));\
 DeeType_FIND_SLOT(lhs_tp_self,slot);\
 return DeeType_GET_SLOT(lhs_tp_self,slot)(\
  (DeeStructuredTypeObject *)lhs_tp_self,lhs_base_addr,rhs);\
}
DEE_PRIVATE_MAKE_PCOMPARE_FUNC(DeeObject_PCompareLo,tp_p_cmp_lo)
DEE_PRIVATE_MAKE_PCOMPARE_FUNC(DeeObject_PCompareLe,tp_p_cmp_le)
DEE_PRIVATE_MAKE_PCOMPARE_FUNC(DeeObject_PCompareEq,tp_p_cmp_eq)
DEE_PRIVATE_MAKE_PCOMPARE_FUNC(DeeObject_PCompareNe,tp_p_cmp_ne)
DEE_PRIVATE_MAKE_PCOMPARE_FUNC(DeeObject_PCompareGr,tp_p_cmp_gr)
DEE_PRIVATE_MAKE_PCOMPARE_FUNC(DeeObject_PCompareGe,tp_p_cmp_ge)
#undef DEE_PRIVATE_MAKE_PCOMPARE_FUNC



DEE_A_RET_EXCEPT(-1) int DeeObject_TSize(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_OUT Dee_size_t *result) {
 DeeObject *result_ob; int error;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeList_Type) { *result = DeeList_Size(self); return 0; }
 if (tp_self == &DeeTuple_Type) { *result = DeeTuple_SIZE(self); return 0; }
 if (tp_self == &DeeDict_Type) { *result = DeeDict_Size(self); return 0; }
 if (tp_self == &DeeSet_Type) { *result = DeeSet_Size(self); return 0; }
 if (tp_self == &DeeCell_Type) { *result = (Dee_size_t)(DeeCell_EMPTY(self) ? 0 : 1); return 0; }
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (tp_self == &DeeUtf8String_Type) { *result = DeeUtf8String_SIZE(self); return 0; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (tp_self == &DeeWideString_Type) { *result = DeeWideString_SIZE(self); return 0; }
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (tp_self == &DeeUtf16String_Type) { *result = DeeUtf16String_SIZE(self); return 0; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (tp_self == &DeeUtf32String_Type) { *result = DeeUtf32String_SIZE(self); return 0; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 if (DeeArrayType_Check(tp_self)) { *result = DeeArray_SIZE(self); return 0; }
 if (DeeVarArrayType_Check(tp_self)) { *result = DeeVarArray_SIZE(self); return 0; }
#endif /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) { *result = 0; return 0; }
 if DEE_UNLIKELY((result_ob = DeeObject_SizeObject(self)) == NULL) return -1;
 error = DeeObject_Cast(Dee_size_t,result_ob,result);
 Dee_DECREF(result_ob);
 return error;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TGetIndex(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t i) {
 DeeObject *result,*iob;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeTuple_Type) DeeReturn_XNEWREF(DeeTuple_At(self,i));
 if (tp_self == &DeeList_Type) return DeeList_At(self,i);
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) DeeReturn_None;
 if DEE_UNLIKELY((iob = DeeObject_New(Dee_size_t,i)) == NULL) return NULL;
 result = DeeObject_GetItem(self,iob);
 Dee_DECREF(iob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TDelIndex(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t i) {
 int result; DeeObject *iob;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeList_Type) return DeeList_DelAt(self,i);
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) return 0;
 if DEE_UNLIKELY((iob = DeeObject_New(Dee_size_t,i)) == NULL) return -1;
 result = DeeObject_TDelItem(tp_self,self,iob);
 Dee_DECREF(iob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TSetIndex(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN Dee_size_t i, DEE_A_INOUT DeeObject *v) {
 int result; DeeObject *iob;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(v));
 if (tp_self == &DeeList_Type) return DeeList_SetAt(self,i,v);
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) return 0;
 if DEE_UNLIKELY((iob = DeeObject_New(Dee_size_t,i)) == NULL) return -1;
 result = DeeObject_TSetItem(tp_self,self,iob,v);
 Dee_DECREF(iob);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TGetRangeIndex(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) {
 DeeObject *result,*iob_lo,*iob_hi;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeTuple_Type) return DeeTuple_GetRangeIndex(self,lo,hi);
 if (tp_self == &DeeList_Type) return DeeList_GetRange(self,lo,hi);
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) DeeReturn_None;
 if DEE_UNLIKELY((iob_lo = DeeObject_New(Dee_size_t,lo)) == NULL) return NULL;
 if DEE_UNLIKELY((iob_hi = DeeObject_New(Dee_size_t,hi)) == NULL) { Dee_DECREF(iob_lo); return NULL; }
 result = DeeObject_TGetRangeItem(tp_self,self,iob_lo,iob_hi);
 Dee_DECREF(iob_hi);
 Dee_DECREF(iob_lo);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TDelRangeIndex(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self,
 DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) {
 int result; DeeObject *iob_lo,*iob_hi;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeList_Type) return hi > lo ? DeeList_Erase(self,lo,(Dee_size_t)(hi-lo)) : 0;
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) return 0;
 if DEE_UNLIKELY((iob_lo = DeeObject_New(Dee_size_t,lo)) == NULL) return -1;
 if DEE_UNLIKELY((iob_hi = DeeObject_New(Dee_size_t,hi)) == NULL) { Dee_DECREF(iob_lo); return -1; }
 result = DeeObject_TDelRangeItem(tp_self,self,iob_lo,iob_hi);
 Dee_DECREF(iob_hi);
 Dee_DECREF(iob_lo);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TSetRangeIndex(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t lo,
 DEE_A_IN Dee_size_t hi, DEE_A_INOUT DeeObject *v) {
 int result; DeeObject *iob_lo,*iob_hi;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(v));
 if (tp_self == &DeeList_Type) return DeeList_SetRange(self,lo,hi,v);
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) return 0;
 if DEE_UNLIKELY((iob_lo = DeeObject_New(Dee_size_t,lo)) == NULL) return -1;
 if DEE_UNLIKELY((iob_hi = DeeObject_New(Dee_size_t,hi)) == NULL) { Dee_DECREF(iob_lo); return -1; }
 result = DeeObject_TSetRangeItem(tp_self,self,iob_lo,iob_hi,v);
 Dee_DECREF(iob_hi);
 Dee_DECREF(iob_lo);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TContains(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *elem) {
 int result; DeeObject *result_ob;
 if DEE_UNLIKELY((result_ob = DeeObject_TContainsObject(tp_self,self,elem)) == NULL) return -1;
 result = DeeObject_Bool(result_ob);
 Dee_DECREF(result_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PSize(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_OUT Dee_size_t *result) {
 DeeObject *result_ob; int error;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 if (DeeArrayType_Check(tp_self)) { *result = DeeArrayType_NELEM(tp_self); return 0; }
#endif /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) { *result = 0; return 0; }
 if DEE_UNLIKELY((result_ob = DeeObject_PSizeObject(tp_self,base_addr)) == NULL) return -1;
 error = DeeObject_Cast(Dee_size_t,result_ob,result);
 Dee_DECREF(result_ob);
 return error;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PGetIndex(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN Dee_size_t i) {
 DeeObject *index_ob,*result;
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) DeeReturn_None;
 if DEE_UNLIKELY((index_ob = DeeObject_New(Dee_size_t,i)) == NULL) return NULL;
 result = DeeObject_PGetItem(tp_self,base_addr,index_ob);
 Dee_DECREF(index_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PDelIndex(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN Dee_size_t i) {
 DeeObject *index_ob; int result;
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) return 0;
 if DEE_UNLIKELY((index_ob = DeeObject_New(Dee_size_t,i)) == NULL) return -1;
 result = DeeObject_PDelItem(tp_self,base_addr,index_ob);
 Dee_DECREF(index_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PSetIndex(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN Dee_size_t i, DEE_A_INOUT DeeObject *v) {
 DeeObject *index_ob; int result;
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) return 0;
 if DEE_UNLIKELY((index_ob = DeeObject_New(Dee_size_t,i)) == NULL) return -1;
 result = DeeObject_PSetItem(tp_self,base_addr,index_ob,v);
 Dee_DECREF(index_ob);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PGetRangeIndex(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) {
 DeeObject *lo_ob,*hi_ob,*result;
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) DeeReturn_None;
 if DEE_UNLIKELY((lo_ob = DeeObject_New(Dee_size_t,lo)) == NULL) return NULL;
 if DEE_UNLIKELY((hi_ob = DeeObject_New(Dee_size_t,hi)) == NULL) { Dee_DECREF(lo_ob); return NULL; }
 result = DeeObject_PGetRangeItem(tp_self,base_addr,lo_ob,hi_ob);
 Dee_DECREF(lo_ob);
 Dee_DECREF(hi_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PDelRangeIndex(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) {
 DeeObject *lo_ob,*hi_ob; int result;
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) return 0;
 if DEE_UNLIKELY((lo_ob = DeeObject_New(Dee_size_t,lo)) == NULL) return -1;
 if DEE_UNLIKELY((hi_ob = DeeObject_New(Dee_size_t,hi)) == NULL) { Dee_DECREF(lo_ob); return -1; }
 result = DeeObject_PDelRangeItem(tp_self,base_addr,lo_ob,hi_ob);
 Dee_DECREF(lo_ob);
 Dee_DECREF(hi_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PSetRangeIndex(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi, DEE_A_INOUT DeeObject *v) {
 DeeObject *lo_ob,*hi_ob; int result;
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) return 0;
 if DEE_UNLIKELY((lo_ob = DeeObject_New(Dee_size_t,lo)) == NULL) return -1;
 if DEE_UNLIKELY((hi_ob = DeeObject_New(Dee_size_t,hi)) == NULL) { Dee_DECREF(lo_ob); return -1; }
 result = DeeObject_PSetRangeItem(tp_self,base_addr,lo_ob,hi_ob,v);
 Dee_DECREF(lo_ob);
 Dee_DECREF(hi_ob);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeObject_PContains(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *elem) {
 DeeObject *result_ob; int result;
 if DEE_UNLIKELY((result_ob = DeeObject_PContainsObject(tp_self,base_addr,elem)) == NULL) return -1;
 result = DeeObject_Bool(result_ob);
 Dee_DECREF(result_ob);
 return result;
}


DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeObject_THasItem(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *key) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(key));
 if (tp_self == &DeeDict_Type) return DeeDict_HasKey(self,key);
 if (tp_self == &DeeSet_Type) return DeeSet_Contains(self,key);
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) return 1;
 result = DeeObject_TGetItem(tp_self,self,key);
 if DEE_UNLIKELY(!result) return DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_KeyError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  ) ? 0 : -1;
 Dee_DECREF(result);
 return 1;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeObject_THasItemString(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *key) {
 DeeObject *result,*key_ob;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(key);
 if (tp_self == &DeeDict_Type) return DeeDict_HasKeyString(self,key);
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) return 1;
 if DEE_UNLIKELY((key_ob = DeeString_New(key)) == NULL) return -1;
 result = DeeObject_TGetItem(tp_self,self,key_ob);
 Dee_DECREF(key_ob);
 if DEE_UNLIKELY(!result) return DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_KeyError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  ) ? 0 : -1;
 Dee_DECREF(result);
 return 1;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TGetItemString(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *key) {
 DeeObject *result,*key_ob;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(key);
 if (tp_self == &DeeDict_Type) return DeeDict_GetItemString(self,key);
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) DeeReturn_None;
 if DEE_UNLIKELY((key_ob = DeeString_New(key)) == NULL) return NULL;
 result = DeeObject_TGetItem(tp_self,self,key_ob);
 Dee_DECREF(key_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TDelItemString(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *key) {
 int result; DeeObject *key_ob;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(key);
 if (tp_self == &DeeDict_Type) return DeeDict_DelItemStringWithError(self,key);
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) return 0;
 if DEE_UNLIKELY((key_ob = DeeString_New(key)) == NULL) return -1;
 result = DeeObject_TDelItem(tp_self,self,key_ob);
 Dee_DECREF(key_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TSetItemString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item) {
 int result; DeeObject *key_ob;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(key);
 DEE_ASSERT(DeeObject_Check(item));
 if (tp_self == &DeeDict_Type) return DeeDict_SetItemString(self,key,item);
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) return 0;
 if DEE_UNLIKELY((key_ob = DeeString_New(key)) == NULL) return -1;
 result = DeeObject_TSetItem(tp_self,self,key_ob,item);
 Dee_DECREF(key_ob);
 return result;
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeObject_PHasItem(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *key) {
 DeeObject *item_val;
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) return 1;
 item_val = DeeObject_PGetItem(tp_self,base_addr,key);
 if DEE_UNLIKELY(!item_val) return DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  ) ? 0 : -1;
 Dee_DECREF(item_val);
 return 1;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeObject_PHasItemString(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *key) {
 DeeObject *item_val,*key_ob;
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) return 1;
 if DEE_UNLIKELY((key_ob = DeeString_New(key)) == NULL) return -1;
 item_val = DeeObject_PGetItem(tp_self,base_addr,key_ob);
 Dee_DECREF(key_ob);
 if DEE_UNLIKELY(!item_val) return DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  ) ? 0 : -1;
 Dee_DECREF(item_val);
 return 1;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PGetItemString(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *key) {
 DeeObject *result,*key_ob;
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) DeeReturn_None;
 if DEE_UNLIKELY((key_ob = DeeString_New(key)) == NULL) return NULL;
 result = DeeObject_PGetItem(tp_self,base_addr,key_ob);
 Dee_DECREF(key_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PDelItemString(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *key) {
 DeeObject *key_ob; int result;
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) return 0;
 if DEE_UNLIKELY((key_ob = DeeString_New(key)) == NULL) return -1;
 result = DeeObject_PDelItem(tp_self,base_addr,key_ob);
 Dee_DECREF(key_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PSetItemString(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item) {
 DeeObject *key_ob; int result;
 if DEE_UNLIKELY(tp_self == (DeeTypeObject *)&DeeNone_Type) return 0;
 if DEE_UNLIKELY((key_ob = DeeString_New(key)) == NULL) return -1;
 result = DeeObject_PSetItem(tp_self,base_addr,key_ob,item);
 Dee_DECREF(key_ob);
 return result;
}



DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_TGetAttrAndCast(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_OUT_OPT void *result,
 DEE_A_IN_Z char const *result_fmt, DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DeeObject *attr_value; int error;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(result_fmt);
 if DEE_UNLIKELY((attr_value = DeeObject_TGetAttr(tp_self,self,attr)) == NULL) return -1;
 error = DeeObject_Castf(attr_value,result,result_fmt);
 Dee_DECREF(attr_value);
 return error;
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_TGetAttrStringAndCast(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_OUT_OPT void *result,
 DEE_A_IN_Z char const *result_fmt, DEE_A_IN_Z char const *attr) {
 DeeObject *attr_value; int error;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(attr);
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(result_fmt);
 if DEE_UNLIKELY((attr_value = DeeObject_TGetAttrString(tp_self,self,attr)) == NULL) return -1;
 error = DeeObject_Castf(attr_value,result,result_fmt);
 Dee_DECREF(attr_value);
 return error;
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TGetAttr(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DeeType_SLOT_TYPE(tp_attr_get) func;
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; while ((func = DeeType_GET_SLOT(tp_iter,tp_attr_get)) == NULL) tp_iter = DeeType_BASE(tp_iter);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeObject_GenericGetAttr) return DeeObject_TGenericGetAttr(tp_self,self,attr);
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeInstance_GetAttr) return DeeInstance_TGetAttr(tp_self,self,attr);
#endif
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStructured_GetAttr) return DeeStructured_TGetAttr(tp_self,self,attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStruct_GetAttr) return DeeStruct_TGetAttr(tp_self,self,attr);
 return (*func)(self,(DeeObject *)attr);
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeObject_THasAttr(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DeeTypeObject const *tp_iter; DeeObject *result;
 DeeType_SLOT_TYPE(tp_attr_get) func;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; while ((func = DeeType_GET_SLOT(tp_iter,tp_attr_get)) == NULL) tp_iter = DeeType_BASE(tp_iter);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeObject_GenericGetAttr) return _DeeObject_TGenericHasAttrString(tp_self,self,DeeString_STR(attr));
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeInstance_GetAttr) return DeeInstance_THasAttr(tp_self,self,attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeClass_GetAttr) return DeeClass_HasAttr((DeeTypeObject *)self,attr);
#endif
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&_deesuper_tp_attr_get) return DeeObject_THasAttr(DeeSuper_TYPE(self),DeeSuper_SELF(self),attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeType_GetAttr) return _DeeType_HasAttrString((DeeTypeObject *)self,DeeString_STR(attr));
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStructured_GetAttr) return DeeStructured_THasAttr(tp_self,self,attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStruct_GetAttr) return DeeStruct_THasAttr(tp_self,self,attr);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&_deelvalue_tp_attr_get) {
  int error; DEE_LVALUE_SIGNALHANDLER_BEGIN {
   error = DeeObject_PHasAttr(DeeLValueType_BASE(Dee_TYPE(self)),DeeLValue_ADDR(self),attr);
  } DEE_LVALUE_SIGNALHANDLER_END({ error = -1; });
  return error;
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&_deedexmodule_tp_attr_get) return DeeDexModule_HasAttrString(self,DeeString_STR(attr));
 result = (*func)(self,(DeeObject *)attr);
 if DEE_UNLIKELY(!result) return DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  ) ? 0 : -1;
 Dee_DECREF(result);
 return 1;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TDelAttr(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DeeType_SLOT_TYPE(tp_attr_del) func;
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; while ((func = DeeType_GET_SLOT(tp_iter,tp_attr_del)) == NULL) tp_iter = DeeType_BASE(tp_iter);
 if (func == (DeeType_SLOT_TYPE(tp_attr_del))&DeeObject_GenericDelAttr) return DeeObject_TGenericDelAttr(tp_self,self,attr);
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
 if (func == (DeeType_SLOT_TYPE(tp_attr_del))&DeeInstance_DelAttr) return DeeInstance_TDelAttr(tp_self,self,attr);
#endif
 if (func == (DeeType_SLOT_TYPE(tp_attr_del))&DeeStructured_DelAttr) return DeeStructured_TDelAttr(tp_self,self,attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_del))&DeeStruct_DelAttr) return DeeStruct_TDelAttr(tp_self,self,attr);
 return (*func)(self,(DeeObject *)attr);
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TSetAttr(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v) {
 DeeType_SLOT_TYPE(tp_attr_set) func;
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; while ((func = DeeType_GET_SLOT(tp_iter,tp_attr_set)) == NULL) tp_iter = DeeType_BASE(tp_iter);
 if (func == (DeeType_SLOT_TYPE(tp_attr_set))&DeeObject_GenericSetAttr) return DeeObject_TGenericSetAttr(tp_self,self,attr,v);
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
 if (func == (DeeType_SLOT_TYPE(tp_attr_set))&DeeInstance_SetAttr) return DeeInstance_TSetAttr(tp_self,self,attr,v);
#endif
 if (func == (DeeType_SLOT_TYPE(tp_attr_set))&DeeStructured_SetAttr) return DeeStructured_TSetAttr(tp_self,self,attr,v);
 if (func == (DeeType_SLOT_TYPE(tp_attr_set))&DeeStruct_SetAttr) return DeeStruct_TSetAttr(tp_self,self,attr,v);
 return (*func)(self,(DeeObject *)attr,v);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TCallAttr(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DeeTypeObject const *tp_iter; DeeObject *callback,*result;
 DeeType_SLOT_TYPE(tp_attr_get) func;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; while ((func = DeeType_GET_SLOT(tp_iter,tp_attr_get)) == NULL) tp_iter = DeeType_BASE(tp_iter);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeObject_GenericGetAttr) return DeeObject_TGenericCallAttr(tp_self,self,attr,args);
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeInstance_GetAttr) return DeeInstance_TCallAttr(tp_self,self,attr,args);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeClass_GetAttr) return DeeClass_CallAttr((DeeTypeObject *)self,attr,args);
#endif
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&_deesuper_tp_attr_get) return DeeObject_TCallAttr(DeeSuper_TYPE(self),DeeSuper_SELF(self),attr,args);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeType_GetAttr) return DeeType_CallAttr((DeeTypeObject *)self,attr,args);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStructured_GetAttr) return DeeStructured_TCallAttr(tp_self,self,attr,args);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStruct_GetAttr) return DeeStruct_TCallAttr(tp_self,self,attr,args);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&_deelvalue_tp_attr_get) {
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   result = DeeObject_PCallAttr(DeeLValueType_BASE(Dee_TYPE(self)),DeeLValue_ADDR(self),attr,args);
  } DEE_LVALUE_SIGNALHANDLER_END({ result = NULL; });
  return result;
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY((callback = (*func)(self,(DeeObject *)attr)) == NULL) return NULL;
 result = DeeObject_Call(callback,args);
 Dee_DECREF(callback);
 return result;
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TGetAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_Z char const *attr) {
 DeeTypeObject const *tp_iter; DeeObject *result,*attr_ob;
 DeeType_SLOT_TYPE(tp_attr_get) func;
 DEE_ASSERT(DeeObject_Check(self));
 tp_iter = tp_self; while ((func = DeeType_GET_SLOT(tp_iter,tp_attr_get)) == NULL) tp_iter = DeeType_BASE(tp_iter);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeObject_GenericGetAttr) return DeeObject_TGenericGetAttrString(tp_self,self,attr);
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeInstance_GetAttr) return DeeInstance_TGetAttrString(tp_self,self,attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeClass_GetAttr) return DeeClass_GetAttrString((DeeTypeObject *)self,attr);
#endif
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&_deesuper_tp_attr_get) return DeeObject_TGetAttrString(DeeSuper_TYPE(self),DeeSuper_SELF(self),attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeType_GetAttr) return DeeType_GetAttrString((DeeTypeObject *)self,attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStructured_GetAttr) return DeeStructured_TGetAttrString(tp_self,self,attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStruct_GetAttr) return DeeStruct_TGetAttrString(tp_self,self,attr);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&_deelvalue_tp_attr_get) {
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   result = DeeObject_PGetAttrString(DeeLValueType_BASE(Dee_TYPE(self)),DeeLValue_ADDR(self),attr);
  } DEE_LVALUE_SIGNALHANDLER_END({ result = NULL; });
  return result;
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&_deedexmodule_tp_attr_get) return DeeDexModule_GetAttrString(self,attr);
 if DEE_UNLIKELY((attr_ob = DeeString_New(attr)) == NULL) return NULL;
 result = (*func)(self,attr_ob);
 Dee_DECREF(attr_ob);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeObject_THasAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_Z char const *attr) {
 DeeTypeObject const *tp_iter; DeeObject *result,*attr_ob;
 DeeType_SLOT_TYPE(tp_attr_get) func;
 DEE_ASSERT(DeeObject_Check(self));
 tp_iter = tp_self; while ((func = DeeType_GET_SLOT(tp_iter,tp_attr_get)) == NULL) tp_iter = DeeType_BASE(tp_iter);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeObject_GenericGetAttr) return _DeeObject_TGenericHasAttrString(tp_self,self,attr);
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeInstance_GetAttr) return DeeInstance_THasAttrString(tp_self,self,attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeClass_GetAttr) return DeeClass_HasAttrString((DeeTypeObject *)self,attr);
#endif
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&_deesuper_tp_attr_get) return DeeObject_THasAttrString(DeeSuper_TYPE(self),DeeSuper_SELF(self),attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeType_GetAttr) return _DeeType_HasAttrString((DeeTypeObject *)self,attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStructured_GetAttr) return DeeStructured_THasAttrString(tp_self,self,attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStruct_GetAttr) return DeeStruct_THasAttrString(tp_self,self,attr);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&_deelvalue_tp_attr_get) {
  int error; DEE_LVALUE_SIGNALHANDLER_BEGIN {
   error = DeeObject_PHasAttrString(DeeLValueType_BASE(Dee_TYPE(self)),DeeLValue_ADDR(self),attr);
  } DEE_LVALUE_SIGNALHANDLER_END({ error = -1; });
  return error;
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&_deedexmodule_tp_attr_get) return DeeDexModule_HasAttrString(self,attr);
 if DEE_UNLIKELY((attr_ob = DeeString_New(attr)) == NULL) return -1;
 result = (*func)(self,attr_ob);
 Dee_DECREF(attr_ob);
 if DEE_UNLIKELY(!result) return DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  ) ? 0 : -1;
 Dee_DECREF(result);
 return 1;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TDelAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_Z char const *attr) {
 DeeTypeObject const *tp_iter; int result; DeeObject *attr_ob;
 DeeType_SLOT_TYPE(tp_attr_del) func;
 DEE_ASSERT(DeeObject_Check(self));
 tp_iter = tp_self; while ((func = DeeType_GET_SLOT(tp_iter,tp_attr_del)) == NULL) tp_iter = DeeType_BASE(tp_iter);
 if (func == (DeeType_SLOT_TYPE(tp_attr_del))&DeeObject_GenericDelAttr) return DeeObject_TGenericDelAttrString(tp_self,self,attr);
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
 if (func == (DeeType_SLOT_TYPE(tp_attr_del))&DeeInstance_DelAttr) return DeeInstance_TDelAttrString(tp_self,self,attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_del))&DeeClass_DelAttr) return DeeClass_DelAttrString((DeeTypeObject *)self,attr);
#endif
 if (func == (DeeType_SLOT_TYPE(tp_attr_del))&_deesuper_tp_attr_del) return DeeObject_TDelAttrString(DeeSuper_TYPE(self),DeeSuper_SELF(self),attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_del))&DeeType_DelAttr) return DeeType_DelAttrString((DeeTypeObject *)self,attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_del))&DeeStructured_DelAttr) return DeeStructured_TDelAttrString(tp_self,self,attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_del))&DeeStruct_DelAttr) return DeeStruct_TDelAttrString(tp_self,self,attr);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (func == (DeeType_SLOT_TYPE(tp_attr_del))&_deelvalue_tp_attr_del) return DeeObject_PDelAttrString(DeeLValueType_BASE(Dee_TYPE(self)),DeeLValue_ADDR(self),attr);
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (func == (DeeType_SLOT_TYPE(tp_attr_del))&_deedexmodule_tp_attr_del) return DeeDexModule_DelAttrString(self,attr);
 if DEE_UNLIKELY((attr_ob = DeeString_New(attr)) == NULL) return -1;
 result = (*func)(self,attr_ob);
 Dee_DECREF(attr_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TSetAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_Z char const *attr, DEE_A_INOUT DeeObject *v) {
 DeeTypeObject const *tp_iter; int result; DeeObject *attr_ob;
 DeeType_SLOT_TYPE(tp_attr_set) func;
 DEE_ASSERT(DeeObject_Check(self));
 tp_iter = tp_self; while ((func = DeeType_GET_SLOT(tp_iter,tp_attr_set)) == NULL) tp_iter = DeeType_BASE(tp_iter);
 if (func == (DeeType_SLOT_TYPE(tp_attr_set))&DeeObject_GenericSetAttr) return DeeObject_TGenericSetAttrString(tp_self,self,attr,v);
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
 if (func == (DeeType_SLOT_TYPE(tp_attr_set))&DeeInstance_SetAttr) return DeeInstance_TSetAttrString(tp_self,self,attr,v);
 if (func == (DeeType_SLOT_TYPE(tp_attr_set))&DeeClass_SetAttr) return DeeClass_SetAttrString((DeeTypeObject *)self,attr,v);
#endif
 if (func == (DeeType_SLOT_TYPE(tp_attr_set))&_deesuper_tp_attr_set) return DeeObject_TSetAttrString(DeeSuper_TYPE(self),DeeSuper_SELF(self),attr,v);
 if (func == (DeeType_SLOT_TYPE(tp_attr_set))&DeeType_SetAttr) return DeeType_SetAttrString((DeeTypeObject *)self,attr,v);
 if (func == (DeeType_SLOT_TYPE(tp_attr_set))&DeeStructured_SetAttr) return DeeStructured_TSetAttrString(tp_self,self,attr,v);
 if (func == (DeeType_SLOT_TYPE(tp_attr_set))&DeeStruct_SetAttr) return DeeStruct_TSetAttrString(tp_self,self,attr,v);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (func == (DeeType_SLOT_TYPE(tp_attr_set))&_deelvalue_tp_attr_set) return DeeObject_PSetAttrString(DeeLValueType_BASE(Dee_TYPE(self)),DeeLValue_ADDR(self),attr,v);
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (func == (DeeType_SLOT_TYPE(tp_attr_set))&_deedexmodule_tp_attr_set) return DeeDexModule_SetAttrString(self,attr,v);
 if DEE_UNLIKELY((attr_ob = DeeString_New(attr)) == NULL) return -1;
 result = (*func)(self,attr_ob,v);
 Dee_DECREF(attr_ob);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TCallAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_Z char const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DeeTypeObject const *tp_iter; DeeObject *result,*member;
 DeeType_SLOT_TYPE(tp_attr_get) func;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 tp_iter = tp_self; while ((func = DeeType_GET_SLOT(tp_iter,tp_attr_get)) == NULL) tp_iter = DeeType_BASE(tp_iter);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeObject_GenericGetAttr) return DeeObject_TGenericCallAttrString(tp_self,self,attr,args);
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeInstance_GetAttr) return DeeInstance_TCallAttrString(tp_self,self,attr,args);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeClass_GetAttr) return DeeClass_CallAttrString((DeeTypeObject *)self,attr,args);
#endif
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&_deesuper_tp_attr_get) return DeeObject_TCallAttrString(DeeSuper_TYPE(self),DeeSuper_SELF(self),attr,args);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeType_GetAttr) return DeeType_CallAttrString((DeeTypeObject *)self,attr,args);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStructured_GetAttr) return DeeStructured_TCallAttrString(tp_self,self,attr,args);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStruct_GetAttr) return DeeStruct_TCallAttrString(tp_self,self,attr,args);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&_deelvalue_tp_attr_get) {
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   result = DeeObject_PCallAttrString(DeeLValueType_BASE(Dee_TYPE(self)),DeeLValue_ADDR(self),attr,args);
  } DEE_LVALUE_SIGNALHANDLER_END({ result = NULL; });
  return result;
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&_deedexmodule_tp_attr_get) {
  if ((member = DeeDexModule_GetAttrString(self,attr)) == NULL) return NULL;
  result = DeeObject_Call(member,args);
  Dee_DECREF(member);
  return result;
 }
 // Fallback: Get member + call it
 if DEE_UNLIKELY((result = DeeString_New(attr)) == NULL) return NULL;
 member = (*func)(self,result);
 Dee_DECREF(result);
 if DEE_UNLIKELY(!member) return NULL;
 result = DeeObject_Call(member,args);
 Dee_DECREF(member);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TCallAttrStringObjArgs(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_Z char const *attr, ...) {
 DeeObject *result; va_list args;
 DEE_VA_START(args,attr);
 result = DeeObject_VTCallAttrStringObjArgs(tp_self,self,attr,args);
 DEE_VA_END(args);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TCallAttrObjArgs(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr, ...) {
 DeeObject *result; va_list args;
 DEE_VA_START(args,attr);
 result = DeeObject_VTCallAttrObjArgs(tp_self,self,attr,args);
 DEE_VA_END(args);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_VTCallAttrStringObjArgs(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_Z char const *attr, DEE_A_IN va_list args) {
 DeeObject *result,*args_tuple;
 if DEE_UNLIKELY((args_tuple = DeeTuple_VNewObjArgs(args)) == NULL) return NULL;
 result = DeeObject_TCallAttrString(tp_self,self,attr,args_tuple);
 Dee_DECREF(args_tuple);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_VTCallAttrObjArgs(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_IN va_list args) {
 DeeObject *result,*args_tuple;
 if DEE_UNLIKELY((args_tuple = DeeTuple_VNewObjArgs(args)) == NULL) return NULL;
 result = DeeObject_TCallAttr(tp_self,self,attr,args_tuple);
 Dee_DECREF(args_tuple);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TCallAttrf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr,
 DEE_A_IN_BUILDTUPLEF char const *fmt, ...) {
 va_list args; DeeObject *result,*args_ob;
 DEE_VA_START(args,fmt);
 args_ob = Dee_VBuildTuple(fmt,args);
 DEE_VA_END(args);
 if DEE_UNLIKELY(!args_ob) return NULL;
 result = DeeObject_TCallAttr(tp_self,self,attr,args_ob);
 Dee_DECREF(args_ob);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TCallAttrStringf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_Z char const *attr, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) {
 va_list args; DeeObject *result,*args_ob;
 DEE_VA_START(args,fmt);
 args_ob = Dee_VBuildTuple(fmt,args);
 DEE_VA_END(args);
 if DEE_UNLIKELY(!args_ob) return NULL;
 result = DeeObject_TCallAttrString(tp_self,self,attr,args_ob);
 Dee_DECREF(args_ob);
 return result;
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_TCallAttrAndCastf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_OUT_OPT void *result, DEE_A_IN_Z char const *result_fmt,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) {
 va_list args; int error;
 DEE_VA_START(args,fmt);
 error = DeeObject_VTCallAttrAndCastf(tp_self,self,result,result_fmt,attr,fmt,args);
 DEE_VA_END(args);
 return error;
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_TCallAttrStringAndCastf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_OUT_OPT void *result, DEE_A_IN_Z char const *result_fmt,
 DEE_A_IN_Z char const *attr, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) {
 va_list args; int error;
 DEE_VA_START(args,fmt);
 error = DeeObject_VTCallAttrStringAndCastf(tp_self,self,result,result_fmt,attr,fmt,args);
 DEE_VA_END(args);
 return error;
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_VTCallAttrf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr,
 DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) {
 DeeObject *result,*args_ob;
 if DEE_UNLIKELY((args_ob = Dee_VBuildTuple(fmt,args)) == NULL) return NULL;
 result = DeeObject_TCallAttr(tp_self,self,attr,args_ob);
 Dee_DECREF(args_ob);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_VTCallAttrStringf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_Z char const *attr, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) {
 DeeObject *result,*args_ob;
 if DEE_UNLIKELY((args_ob = Dee_VBuildTuple(fmt,args)) == NULL) return NULL;
 result = DeeObject_TCallAttrString(tp_self,self,attr,args_ob);
 Dee_DECREF(args_ob);
 return result;
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_VTCallAttrAndCastf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_OUT_OPT void *result, DEE_A_IN_Z char const *result_fmt,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) {
 DeeObject *retval; int error;
 if DEE_UNLIKELY((retval = DeeObject_VTCallAttrf(tp_self,self,attr,fmt,args)) == NULL) return -1;
 error = DeeObject_Castf(retval,result,result_fmt);
 Dee_DECREF(retval);
 return error;
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_VTCallAttrStringAndCastf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_OUT_OPT void *result, DEE_A_IN_Z char const *result_fmt,
 DEE_A_IN_Z char const *attr, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) {
 DeeObject *retval; int error;
 if DEE_UNLIKELY((retval = DeeObject_VTCallAttrStringf(tp_self,self,attr,fmt,args)) == NULL) return -1;
 error = DeeObject_Castf(retval,result,result_fmt);
 Dee_DECREF(retval);
 return error;
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TGetAttrConst(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 return DeeObject_TGetAttrConstString(tp_self,self,DeeString_STR(attr));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TGetAttrConstString(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr) {
 DeeTypeObject const *tp_iter;
 DeeType_SLOT_TYPE(tp_attr_get) func;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(attr);
 tp_iter = tp_self;
 while ((func = DeeType_GET_SLOT(tp_iter,tp_attr_get)) == NULL) tp_iter = DeeType_BASE(tp_iter);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeObject_GenericGetAttr) return DeeObject_TGenericGetConstAttrString(tp_self,self,attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeType_GetAttr) return DeeType_GetConstAttrString((DeeTypeObject *)self,attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&_deesuper_tp_attr_get) return DeeObject_TGenericGetConstAttrString(DeeSuper_TYPE(self),DeeSuper_SELF(self),attr);
#if 0
 // TODO: Add some restrictions to these attribute lookup functions.
 //    -> They were only added to allow for compile-time offsetof lookup
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStructured_GetAttr) return DeeStructured_TGetAttrString(tp_self,self,attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStruct_GetAttr) return DeeStruct_TGetAttrString(tp_self,self,attr);
 if (func == (DeeType_SLOT_TYPE(tp_attr_get))&_deelvalue_tp_attr_get) {
  DeeObject *result;
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   result = DeeObject_PGetAttrString(DeeLValueType_BASE(tp_self),DeeLValue_ADDR(self),attr);
  } DEE_LVALUE_SIGNALHANDLER_END({ result = NULL; });
  return result;
 }
#endif
 DeeError_AttributeError_NotFound(tp_self,attr);
 return NULL;
}



DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PCallAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DeeTypeObject const *tp_first; DeeObject *callback,*result;
 DeeType_SLOT_TYPE(tp_p_attr_get) func;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_first = tp_self;
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_get)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return DeeObject_PGenericCallAttrString(tp_first,base_addr,attr,args);
 }
 if (func == (DeeType_SLOT_TYPE(tp_p_attr_get))&DeeStruct_PGetAttr)
  return DeeStruct_PCallAttrString(tp_first,base_addr,attr,args);
 if DEE_UNLIKELY((result = DeeString_New(attr)) == NULL) return NULL;
 callback = (*func)((DeeStructuredTypeObject *)tp_first,base_addr,result);
 Dee_DECREF(result);
 if DEE_UNLIKELY(!callback) return NULL;
 result = DeeObject_Call(callback,args);
 Dee_DECREF(callback);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PCallAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DeeTypeObject const *tp_first; DeeObject *result,*callback;
 DeeType_SLOT_TYPE(tp_p_attr_get) func;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_first = tp_self;
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_get)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return DeeObject_PGenericCallAttr(tp_first,base_addr,attr,(DeeObject *)args);
 }
 if (func == (DeeType_SLOT_TYPE(tp_p_attr_get))&DeeStruct_PGetAttr)
  return DeeStruct_PCallAttr(tp_first,base_addr,attr,args);
 if DEE_UNLIKELY((callback = (*func)((DeeStructuredTypeObject *)tp_first,
  base_addr,(DeeObject *)attr)) == NULL) return NULL;
 result = DeeObject_Call(callback,args);
 Dee_DECREF(callback);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PCallAttrStringObjArgs(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr,
 DEE_A_IN_Z char const *attr, ...) {
 DeeObject *result; va_list args;
 DEE_VA_START(args,attr);
 result = DeeObject_VPCallAttrStringObjArgs(tp_self,base_addr,attr,args);
 DEE_VA_END(args);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PCallAttrObjArgs(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr, ...) {
 DeeObject *result; va_list args;
 DEE_VA_START(args,attr);
 result = DeeObject_VPCallAttrObjArgs(tp_self,base_addr,attr,args);
 DEE_VA_END(args);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_VPCallAttrStringObjArgs(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr,
 DEE_A_IN_Z char const *attr, DEE_A_IN va_list args) {
 DeeObject *result,*args_tuple;
 if DEE_UNLIKELY((args_tuple = DeeTuple_VNewObjArgs(args)) == NULL) return NULL;
 result = DeeObject_PCallAttrString(tp_self,base_addr,attr,args_tuple);
 Dee_DECREF(args_tuple);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_VPCallAttrObjArgs(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_IN va_list args) {
 DeeObject *result,*args_tuple;
 if DEE_UNLIKELY((args_tuple = DeeTuple_VNewObjArgs(args)) == NULL) return NULL;
 result = DeeObject_PCallAttr(tp_self,base_addr,attr,args_tuple);
 Dee_DECREF(args_tuple);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PCallAttrf(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr,
 DEE_A_IN_BUILDTUPLEF char const *fmt, ...) {
 va_list args; DeeObject *result,*args_ob;
 DEE_VA_START(args,fmt);
 args_ob = Dee_VBuildTuple(fmt,args);
 DEE_VA_END(args);
 if DEE_UNLIKELY(!args_ob) return NULL;
 result = DeeObject_PCallAttr(tp_self,base_addr,attr,args_ob);
 Dee_DECREF(args_ob);
 return result;
}
DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_VPCallAttrf(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr,
 DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) {
 DeeObject *result,*args_ob;
 if DEE_UNLIKELY((args_ob = Dee_VBuildTuple(fmt,args)) == NULL) return NULL;
 result = DeeObject_PCallAttr(tp_self,base_addr,attr,args_ob);
 Dee_DECREF(args_ob);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PCallAttrStringf(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr,
 DEE_A_IN_BUILDTUPLEF char const *fmt, ...) {
 va_list args; DeeObject *result,*args_ob;
 DEE_VA_START(args,fmt);
 args_ob = Dee_VBuildTuple(fmt,args);
 DEE_VA_END(args);
 if DEE_UNLIKELY(!args_ob) return NULL;
 result = DeeObject_PCallAttrString(tp_self,base_addr,attr,args_ob);
 Dee_DECREF(args_ob);
 return result;
}
DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_VPCallAttrStringf(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr,
 DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) {
 DeeObject *result,*args_ob;
 if DEE_UNLIKELY((args_ob = Dee_VBuildTuple(fmt,args)) == NULL) return NULL;
 result = DeeObject_PCallAttrString(tp_self,base_addr,attr,args_ob);
 Dee_DECREF(args_ob);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PTCallAttrf(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr,
 DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) {
 DeeObject *result,*args_ob;
 if DEE_UNLIKELY((args_ob = Dee_VBuildTuple(fmt,args)) == NULL) return NULL;
 result = DeeObject_PCallAttr(tp_self,base_addr,attr,args_ob);
 Dee_DECREF(args_ob);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PTCallAttrStringf(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr,
 DEE_A_IN_Z char const *attr, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) {
 DeeObject *result,*args_ob;
 if DEE_UNLIKELY((args_ob = Dee_VBuildTuple(fmt,args)) == NULL) return NULL;
 result = DeeObject_PCallAttrString(tp_self,base_addr,attr,args_ob);
 Dee_DECREF(args_ob);
 return result;
}



DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PGetAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr) {
 DeeTypeObject const *tp_first; DeeObject *result,*attr_ob;
 DeeType_SLOT_TYPE(tp_p_attr_get) func;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_first = tp_self;
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_get)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return DeeObject_PGenericGetAttrString(tp_first,base_addr,attr);
 }
 if (func == (DeeType_SLOT_TYPE(tp_p_attr_get))&DeeStruct_PGetAttr)
  return DeeStruct_PGetAttrString(tp_first,base_addr,attr);
 if DEE_UNLIKELY((attr_ob = DeeString_New(attr)) == NULL) return NULL;
 result = (*func)((DeeStructuredTypeObject *)tp_first,base_addr,attr_ob);
 Dee_DECREF(attr_ob);
 return result;
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeObject_PHasAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DeeTypeObject const *tp_first; DeeObject *attr_value;
 DeeType_SLOT_TYPE(tp_p_attr_get) func;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_first = tp_self;
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_get)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return _DeeObject_PGenericHasAttrString(tp_first,DeeString_STR(attr));
 }
 if (func == (DeeType_SLOT_TYPE(tp_p_attr_get))&DeeStruct_PGetAttr)
  return DeeStruct_PHasAttr(tp_first,attr);
 attr_value = (*func)((DeeStructuredTypeObject *)tp_first,base_addr,(DeeObject *)attr);
 if DEE_UNLIKELY(!attr_value) return DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  ) ? 0 : -1;
 Dee_DECREF(attr_value);
 return 1;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeObject_PHasAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr) {
 DeeTypeObject const *tp_first; DeeObject *attr_ob,*attr_value;
 DeeType_SLOT_TYPE(tp_p_attr_get) func;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_first = tp_self;
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_get)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return _DeeObject_PGenericHasAttrString(tp_self,attr);
 }
 if (func == (DeeType_SLOT_TYPE(tp_p_attr_get))&DeeStruct_PGetAttr)
  return DeeStruct_PHasAttrString(tp_first,attr);
 if DEE_UNLIKELY((attr_ob = DeeString_New(attr)) == NULL) return -1;
 attr_value = (*func)((DeeStructuredTypeObject *)tp_first,base_addr,attr_ob);
 Dee_DECREF(attr_ob);
 if DEE_UNLIKELY(!attr_value) return DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  ) ? 0 : -1;
 Dee_DECREF(attr_value);
 return 1;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PDelAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr) {
 DeeTypeObject const *tp_first; DeeObject *attr_ob; int result;
 DeeType_SLOT_TYPE(tp_p_attr_del) func;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_first = tp_self;
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_del)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return DeeObject_PGenericDelAttrString(tp_first,base_addr,attr);
 }
 if (func == (DeeType_SLOT_TYPE(tp_p_attr_del))&DeeStruct_PDelAttr)
  return DeeStruct_PDelAttrString(tp_first,base_addr,attr);
 if DEE_UNLIKELY((attr_ob = DeeString_New(attr)) == NULL) return -1;
 result = (*func)((DeeStructuredTypeObject *)tp_first,base_addr,attr_ob);
 Dee_DECREF(attr_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PSetAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr, DEE_A_INOUT DeeObject *v) {
 DeeTypeObject const *tp_first; DeeObject *attr_ob; int result;
 DeeType_SLOT_TYPE(tp_p_attr_set) func;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_first = tp_self;
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_set)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return DeeObject_PGenericSetAttrString(tp_first,base_addr,attr,v);
 }
 if (func == (DeeType_SLOT_TYPE(tp_p_attr_set))&DeeStruct_PSetAttr)
  return DeeStruct_PSetAttrString(tp_first,base_addr,attr,v);
 if DEE_UNLIKELY((attr_ob = DeeString_New(attr)) == NULL) return -1;
 result = (*func)((DeeStructuredTypeObject *)tp_first,base_addr,attr_ob,v);
 Dee_DECREF(attr_ob);
 return result;
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TGenericCallAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_Z char const *name, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DeeObject *result; int temp;
 if DEE_UNLIKELY((temp = _DeeObject_TGenericCallAttrString(tp_self,self,name,args,&result)) < 0) result = NULL;
 else if DEE_UNLIKELY(temp == 1) { DeeError_AttributeError_NotFound(tp_self,name); result = NULL; }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PGenericCallAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT void *base_addr,
 DEE_A_IN_Z char const *name, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DeeObject *result; int temp;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 if DEE_UNLIKELY((temp = _DeeObject_PGenericCallAttrString(tp_self,base_addr,name,args,&result)) < 0) result = NULL;
 else if DEE_UNLIKELY(temp == 1) { DeeError_AttributeError_NotFound(tp_self,name); result = NULL; }
 return result;
}



DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_TIORead(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s, DEE_A_OUT Dee_size_t *rs) {
 DeeTypeObject const *tp_iter;
 DeeType_SLOT_TYPE(tp_io_read) callback;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(rs); DEE_ASSERT(!s || p);
 if (tp_self == &DeeSuper_Type) return DeeObject_TIORead(
  DeeSuper_TYPE(self),DeeSuper_SELF(self),p,s,rs);
 if (tp_self == (DeeTypeObject *)&DeeNone_Type) { memset(p,0,s); *rs = s; return 0; }
 if DEE_LIKELY(DeeFileType_Check(tp_self)) {
  tp_iter = tp_self; do {
   callback = DeeType_GET_SLOT(tp_iter,tp_io_read);
   if (callback) return (*callback)((DeeFileObject *)self,p,s,rs);
   tp_iter = DeeType_BASE(tp_iter);
  } while (DeeFileType_Check(tp_iter));
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__read__()",
                     DeeType_NAME(tp_self));
 return -1;
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_TIOWrite(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s, DEE_A_OUT Dee_size_t *ws) {
 DeeTypeObject const *tp_iter;
 DeeType_SLOT_TYPE(tp_io_write) callback;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(ws); DEE_ASSERT(!s || p);
 if (tp_self == &DeeSuper_Type) return DeeObject_TIOWrite(
  DeeSuper_TYPE(self),DeeSuper_SELF(self),p,s,ws);
 if (tp_self == (DeeTypeObject *)&DeeNone_Type) { *ws = s; return 0; }
 if DEE_LIKELY(DeeFileType_Check(tp_self)) {
  tp_iter = tp_self; do {
   callback = DeeType_GET_SLOT(tp_iter,tp_io_write);
   if (callback) return (*callback)((DeeFileObject *)self,p,s,ws);
   tp_iter = DeeType_BASE(tp_iter);
  } while (DeeFileType_Check(tp_iter));
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__write__()",
                     DeeType_NAME(tp_self));
 return -1;
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_TIOSeek(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN Dee_int64_t off, DEE_A_IN int whence, DEE_A_OUT_OPT Dee_uint64_t *new_pos) {
 DeeTypeObject const *tp_iter;
 DeeType_SLOT_TYPE(tp_io_seek) callback;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeSuper_Type) return DeeObject_TIOSeek(
  DeeSuper_TYPE(self),DeeSuper_SELF(self),off,whence,new_pos);
 if (tp_self == (DeeTypeObject *)&DeeNone_Type) return 0;
 if DEE_LIKELY(DeeFileType_Check(tp_self)) {
  tp_iter = tp_self; do {
   callback = DeeType_GET_SLOT(tp_iter,tp_io_seek);
   if (callback) return (*callback)((DeeFileObject *)self,off,whence,new_pos);
   tp_iter = DeeType_BASE(tp_iter);
  } while (DeeFileType_Check(tp_iter));
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__seek__()",
                     DeeType_NAME(tp_self));
 return -1;
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_TIOFlush(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeTypeObject const *tp_iter;
 DeeType_SLOT_TYPE(tp_io_flush) callback;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeSuper_Type) return DeeObject_TIOFlush(
  DeeSuper_TYPE(self),DeeSuper_SELF(self));
 if (tp_self == (DeeTypeObject *)&DeeNone_Type) return 0;
 if DEE_LIKELY(DeeFileType_Check(tp_self)) {
  tp_iter = tp_self; do {
   callback = DeeType_GET_SLOT(tp_iter,tp_io_flush);
   if (callback) return (*callback)((DeeFileObject *)self);
   tp_iter = DeeType_BASE(tp_iter);
  } while (DeeFileType_Check(tp_iter));
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__flush__()",
                     DeeType_NAME(tp_self));
 return -1;
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_TIOTrunc(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeTypeObject const *tp_iter;
 DeeType_SLOT_TYPE(tp_io_trunc) callback;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeSuper_Type) return DeeObject_TIOTrunc(
  DeeSuper_TYPE(self),DeeSuper_SELF(self));
 if (tp_self == (DeeTypeObject *)&DeeNone_Type) return 0;
 if DEE_LIKELY(DeeFileType_Check(tp_self)) {
  tp_iter = tp_self; do {
   callback = DeeType_GET_SLOT(tp_iter,tp_io_trunc);
   if (callback) return (*callback)((DeeFileObject *)self);
   tp_iter = DeeType_BASE(tp_iter);
  } while (DeeFileType_Check(tp_iter));
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__trunc__()",
                     DeeType_NAME(tp_self));
 return -1;
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_TIOClose(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeTypeObject const *tp_iter;
 DeeType_SLOT_TYPE(tp_io_close) callback;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeSuper_Type) return DeeObject_TIOClose(
  DeeSuper_TYPE(self),DeeSuper_SELF(self));
 if (tp_self == (DeeTypeObject *)&DeeNone_Type) return 0;
 if DEE_LIKELY(DeeFileType_Check(tp_self)) {
  tp_iter = tp_self; do {
   callback = DeeType_GET_SLOT(tp_iter,tp_io_close);
   if (callback) { (*callback)((DeeFileObject *)self); return 0; }
   tp_iter = DeeType_BASE(tp_iter);
  } while (DeeFileType_Check(tp_iter));
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__close__()",
                     DeeType_NAME(tp_self));
 return -1;
}







DEE_A_RET_EXCEPT(-1) int DeeObject_Swap(
 DEE_A_INOUT DeeObject *a, DEE_A_INOUT DeeObject *b) {
 DeeObject *temp;
 DEE_ASSERT(DeeObject_Check(a));
 DEE_ASSERT(DeeObject_Check(b));
 DEE_ASSERT(Dee_TYPE(a) == Dee_TYPE(b));
 // Move 'a' into a temporary object
 if DEE_UNLIKELY((temp = DeeObject_Move(a)) == NULL) return -1;
 // Move 'b' into 'a'
 if DEE_UNLIKELY(DeeObject_MoveAssign(a,b) != 0) {
restore_a: // Try to restore 'a'
  if DEE_UNLIKELY(DeeObject_MoveAssign(a,temp) != 0) DeeError_Handled();
  Dee_DECREF(temp);
  return -1;
 }
 // Move the temporary into 'b'
 if DEE_UNLIKELY(DeeObject_MoveAssign(b,temp) != 0) goto restore_a;
 Dee_DECREF(temp);
 return 0;
}


DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeObject_TIsImmutable(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 int temp; DeeObject *elem;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (!DeeType_IsImmutable(tp_self)) return 0; // Not an immutable type
 if (tp_self == &DeeBool_Type // Don't try to iterate this one...
#if DEE_CONFIG_HAVE_ENCODING_WIDE
  || tp_self == &DeeWideString_Type
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
  || tp_self == &DeeUtf8String_Type
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
  || tp_self == &DeeUtf16String_Type
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
  || tp_self == &DeeUtf32String_Type
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
     ) return 1; // Don't check recursively on string types (would end in an infinite loop)
 if (tp_self == &DeeTuple_Type) {
  DeeObject **iter,**end;
  end = (iter = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
  while (iter != end) {
   elem = *iter++;
   if ((temp = DeeObject_IsImmutable(elem)) <= 0) return temp; // Error, or not immutable
  }
  return 1; // Immutable tuple
 }
 if (tp_self == &DeeSuper_Type) return DeeObject_TIsImmutable(DeeSuper_TYPE(self),DeeSuper_SELF(self));
 // Fallback: Check recursively
 if DEE_UNLIKELY((self = DeeObject_TIterSelf(tp_self,self)) == NULL)
  return DeeError_Catch(&DeeErrorType_NotImplemented) ? 0 : -1;
 while DEE_LIKELY((temp = DeeObject_IterNextEx(self,&elem)) == 0) {
  temp = DeeObject_IsImmutable(elem);
  Dee_DECREF(elem);
  if (temp <= 0) break; // Error, or not immutable
 }
 Dee_DECREF(self);
 return temp <= 0 ? temp : 1; // Error, not immutable or immutable
}



DEE_A_RET_EXCEPT(-1) int DeeObject_ForceUnique(
 DEE_A_INOUT DeeObject **self) {
 DeeObject *copy;
 DEE_ASSERT(self && DeeObject_Check(*self));
 if DEE_UNLIKELY(!DeeObject_IS_UNIQUE(*self)) {
  if DEE_UNLIKELY((copy = DeeObject_Copy(*self)) == NULL) return -1;
  *self = copy;
 }
 return 0;
}


DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TCopy(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *result; Dee_uint32_t flags;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 flags = DeeType_FLAGS(tp_self);
 if DEE_UNLIKELY((flags&DEE_TYPE_FLAG_DONT_COPY)!=0) DeeReturn_NEWREF(self);
 if DEE_UNLIKELY((flags&DEE_TYPE_FLAG_VAR_OBJECT)!=0)
  return (*DeeType_GET_SLOT(tp_self,tp_copy_new))((DeeTypeObject *)tp_self,self);
 if DEE_LIKELY((result = (*DeeType_GET_SLOT(tp_self,tp_alloc))((DeeTypeObject *)tp_self)) != NULL) {
  if DEE_UNLIKELY((*DeeType_GET_SLOT(tp_self,tp_copy_ctor))((DeeTypeObject *)tp_self,result,self) != 0) {
   _DeeObject_DELETE(tp_self,result);
   return NULL;
  }
  if DEE_UNLIKELY((flags&DEE_TYPE_FLAG_HAS_GC)!=0) DeeGC_TrackedAdd(result);
 }
 return result;
}
DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TMove(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *result; Dee_uint32_t flags;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 flags = DeeType_FLAGS(tp_self);
 if DEE_UNLIKELY((flags&DEE_TYPE_FLAG_DONT_COPY)!=0) DeeReturn_NEWREF(self);
 if DEE_UNLIKELY((flags&DEE_TYPE_FLAG_VAR_OBJECT)!=0)
  return (*DeeType_GET_SLOT(tp_self,tp_move_new))((DeeTypeObject *)tp_self,self);
 if DEE_LIKELY((result = (*DeeType_GET_SLOT(tp_self,tp_alloc))((DeeTypeObject *)tp_self)) != NULL) {
  if DEE_UNLIKELY((*DeeType_GET_SLOT(tp_self,tp_move_ctor))((DeeTypeObject *)tp_self,result,self) != 0) {
   _DeeObject_DELETE(tp_self,result);
   return NULL;
  }
  if DEE_UNLIKELY((flags&DEE_TYPE_FLAG_HAS_GC)!=0) DeeGC_TrackedAdd(result);
 }
 return result;
}


#ifndef __INTELLISENSE__
#if DEE_CONFIG_SIZEOF_DEE_HASH_T == 4
#if 0
#include "object.hash.murmur3_32.inl"
#else
#include "object.hash.something32.inl"
#endif
#elif DEE_CONFIG_SIZEOF_DEE_HASH_T == 8
#include "object.hash.something64.inl"
#else
#error "No known hash algorithms for the configured hash size"
#endif
#endif


DEE_A_RET_WUNUSED Dee_hash_t DeeHash_String(
 DEE_A_IN_Z char const *s, DEE_A_IN Dee_hash_t start) {
 return DeeHash_Memory(s,strlen(s),start);
}





DeeTypeObject DeeObject_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("object"),member("(...) -> object\n"
  "\tBase class of anything and everything in deemon.\n"
  "\tEverything in deemon is a sub-class of this type."),
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_CONST_CTOR|
         DEE_TYPE_FLAG_FUNDAMENTAL),
  member(NULL)), // no base!
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(
  sizeof(DeeObject),
  member(&_DeeGenericFallback_tp_alloc),
  member(&_deeobject_tp_ctor),
  member(&_deeobject_tp_ctor_arg),
  member(&_deeobject_tp_ctor_arg),
  member(&_deeobject_tp_ctor_args)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(
  member(&_DeeGenericFallback_tp_free),null),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deeobject_tp_copy_assign),
  member(&_deeobject_tp_move_assign),
  member(&_deeobject_tp_any_assign)),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deeobject_tp_str),
  member(&_deeobject_tp_repr),
  member(&_deeobject_tp_int32),
  member(&_deeobject_tp_int64),
  member(&_deeobject_tp_double)),
 DEE_TYPE_OBJECT_OBJECT_v101(
  member(&_deeobject_tp_call),null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deeobject_tp_not),member(&_deeobject_tp_bool),member(&_deeobject_tp_inv),
  member(&_deeobject_tp_pos),member(&_deeobject_tp_neg),
  member(&_deeobject_tp_inc),member(&_deeobject_tp_incpost),
  member(&_deeobject_tp_dec),member(&_deeobject_tp_decpost),
  member(&_deeobject_tp_add),member(&_deeobject_tp_iadd),
  member(&_deeobject_tp_sub),member(&_deeobject_tp_isub),
  member(&_deeobject_tp_mul),member(&_deeobject_tp_imul),
  member(&_deeobject_tp_div),member(&_deeobject_tp_idiv),
  member(&_deeobject_tp_mod),member(&_deeobject_tp_imod),
  member(&_deeobject_tp_shl),member(&_deeobject_tp_ishl),
  member(&_deeobject_tp_shr),member(&_deeobject_tp_ishr),
  member(&_deeobject_tp_and),member(&_deeobject_tp_iand),
  member(&_deeobject_tp_or),member(&_deeobject_tp_ior),
  member(&_deeobject_tp_xor),member(&_deeobject_tp_ixor),
  member(&_deeobject_tp_pow),member(&_deeobject_tp_ipow),
  member(&_deeobject_tp_hash)),
 DEE_TYPE_OBJECT_COMPARE_v100(
  // We allow comparing of all objects by default!
  member(&_deeobject_tp_cmp_lo),member(&_deeobject_tp_cmp_le),
  member(&_deeobject_tp_cmp_eq),member(&_deeobject_tp_cmp_ne),
  member(&_deeobject_tp_cmp_gr),member(&_deeobject_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&_deeobject_tp_seq_get),
  member(&_deeobject_tp_seq_del),
  member(&_deeobject_tp_seq_set),
  member(&_deeobject_tp_seq_size),
  member(&_deeobject_tp_seq_contains),
  member(&_deeobject_tp_seq_range_get),
  member(&_deeobject_tp_seq_range_del),
  member(&_deeobject_tp_seq_range_set),
  member(&_deeobject_tp_seq_iter_self),
  member(&_deeobject_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(
  member(&DeeObject_GenericGetAttr),
  member(&DeeObject_GenericDelAttr),
  member(&DeeObject_GenericSetAttr),
  member(_deeobject_tp_members),
  member(_deeobject_tp_getsets),
  member(_deeobject_tp_methods),
  member(_DeeGenericFallback_MemberDef),
  member(_DeeGenericFallback_GetSetDef),
  member(_DeeGenericFallback_MethodDef)),
 DEE_TYPE_OBJECT_MARSHAL_v101(
  member(&_deeobject_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};



#ifndef __INTELLISENSE__
#define OB_NOCOLLECT
#define OB_TRYMALLOC
#define OB_DBGMALLOC
#include "object.core.ob_malloc.inl"

#define OB_NOCOLLECT
#define OB_TRYMALLOC
#include "object.core.ob_malloc.inl"

#define OB_NOCOLLECT
#define OB_DBGMALLOC
#include "object.core.ob_malloc.inl"

#define OB_NOCOLLECT
#include "object.core.ob_malloc.inl"

#define OB_TRYMALLOC
#define OB_DBGMALLOC
#include "object.core.ob_malloc.inl"

#define OB_TRYMALLOC
#include "object.core.ob_malloc.inl"

#define OB_DBGMALLOC
#include "object.core.ob_malloc.inl"

#include "object.core.ob_malloc.inl"
#endif


#undef DeeObject_Copy
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_Copy(DEE_A_INOUT DeeObject *self) {
 DEE_ASSERT(DeeObject_Check(self));
 return DeeObject_TCopy(Dee_TYPE(self),self);
}
#undef DeeObject_Move
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_Move(DEE_A_INOUT DeeObject *self) {
 DEE_ASSERT(DeeObject_Check(self));
 return DeeObject_TMove(Dee_TYPE(self),self);
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_OBJECT_C */
