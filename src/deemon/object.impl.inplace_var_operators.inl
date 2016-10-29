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
#endif

#include <deemon/__conf.inl>
#include <deemon/integer.h>
#include <deemon/object.h>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/type_slots.h>
#include <deemon/type.h>


DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeObject_InplaceIncVar(DEE_A_INOUT DeeObject **self) {
 DeeTypeObject *tp; DeeObject *result,*int_one;
 DeeType_SLOT_TYPE(tp_inc) inc_func;
 DeeType_SLOT_TYPE(tp_add) add_func;
 DEE_ASSERT(self && DeeObject_Check(*self));
 tp = Dee_TYPE(*self);
 while ((inc_func = DeeType_GET_SLOT(tp,tp_inc)) == NULL) tp = DeeType_BASE(tp);
 if DEE_LIKELY(inc_func != DeeType_GET_SLOT(&DeeObject_Type,tp_inc)) {
inc_call: result = (*inc_func)(*self); goto end;
 }
 tp = Dee_TYPE(*self);
 while ((add_func = DeeType_GET_SLOT(tp,tp_iadd)) == NULL) tp = DeeType_BASE(tp);
 if DEE_UNLIKELY(add_func == DeeType_GET_SLOT(&DeeObject_Type,tp_iadd)) {
  tp = Dee_TYPE(*self);
  while ((add_func = DeeType_GET_SLOT(tp,tp_add)) == NULL) tp = DeeType_BASE(tp);
  if DEE_UNLIKELY(add_func == DeeType_GET_SLOT(&DeeObject_Type,tp_add)) goto inc_call;
 }
 if ((int_one = DeeObject_New(int,1)) == NULL) return -1;
 result = (*add_func)(*self,int_one);
 Dee_DECREF(int_one);
end:
 if DEE_UNLIKELY(!result) return -1;
 tp = (DeeTypeObject *)*self;
 Dee_INHERIT_REF(*self,result);
 Dee_DECREF(tp);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_InplaceDecVar(DEE_A_INOUT DeeObject **self) {
 DeeTypeObject *tp; DeeObject *result,*int_one;
 DeeType_SLOT_TYPE(tp_dec) dec_func;
 DeeType_SLOT_TYPE(tp_sub) sub_func;
 DEE_ASSERT(self && DeeObject_Check(*self));
 tp = Dee_TYPE(*self);
 while ((dec_func = DeeType_GET_SLOT(tp,tp_dec)) == NULL) tp = DeeType_BASE(tp);
 if DEE_LIKELY(dec_func != DeeType_GET_SLOT(&DeeObject_Type,tp_dec)) {
inc_call: result = (*dec_func)(*self); goto end;
 }
 tp = Dee_TYPE(*self);
 while ((sub_func = DeeType_GET_SLOT(tp,tp_isub)) == NULL) tp = DeeType_BASE(tp);
 if DEE_UNLIKELY(sub_func == DeeType_GET_SLOT(&DeeObject_Type,tp_isub)) {
  tp = Dee_TYPE(*self);
  while ((sub_func = DeeType_GET_SLOT(tp,tp_sub)) == NULL) tp = DeeType_BASE(tp);
  if DEE_UNLIKELY(sub_func == DeeType_GET_SLOT(&DeeObject_Type,tp_sub)) goto inc_call;
 }
 if ((int_one = DeeObject_New(int,1)) == NULL) return -1;
 result = (*sub_func)(*self,int_one);
 Dee_DECREF(int_one);
end:
 if DEE_UNLIKELY(!result) return -1;
 tp = (DeeTypeObject *)*self;
 Dee_INHERIT_REF(*self,result);
 Dee_DECREF(tp);
 return 0;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_InplaceIncPostVar(DEE_A_INOUT DeeObject **self) {
 DeeObject *result;
 DEE_ASSERT(self && DeeObject_Check(*self));
 if ((result = DeeObject_Copy(*self)) == NULL) return NULL;
 if (DeeObject_InplaceIncVar(self) != 0) Dee_CLEAR(result);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_InplaceDecPostVar(DEE_A_INOUT DeeObject **self) {
 DeeObject *result;
 DEE_ASSERT(self && DeeObject_Check(*self));
 if ((result = DeeObject_Copy(*self)) == NULL) return NULL;
 if (DeeObject_InplaceDecVar(self) != 0) Dee_CLEAR(result);
 return result;
}

#define DEE_PRIVATE_MAKE_BINARY_INPLACE_VAR_FUNCTOR(name,tp_xxx,tp_ixxx)\
DEE_A_RET_EXCEPT(-1) int name(\
 DEE_A_INOUT DeeObject **lhs, DEE_A_INOUT DeeObject *rhs) {\
 DeeTypeObject *tp; DeeObject *result;\
 DeeType_SLOT_TYPE(tp_ixxx) ifunc;\
 DeeType_SLOT_TYPE(tp_xxx) func;\
 DEE_ASSERT(lhs && DeeObject_Check(*lhs));\
 DEE_ASSERT(DeeObject_Check(rhs));\
 tp = Dee_TYPE(*lhs);\
 while ((ifunc = DeeType_GET_SLOT(tp,tp_ixxx)) == NULL)\
  tp = DeeType_BASE(tp);\
 if DEE_LIKELY(ifunc != DeeType_GET_SLOT(&DeeObject_Type,tp_ixxx)) {\
calli:\
  result = (*ifunc)(*lhs,rhs);\
 } else {\
  tp = Dee_TYPE(*lhs);\
  while ((func = DeeType_GET_SLOT(tp,tp_xxx)) == NULL)\
   tp = DeeType_BASE(tp);\
  if DEE_LIKELY(func != DeeType_GET_SLOT(&DeeObject_Type,tp_xxx)) {\
   result = (*func)(*lhs,rhs);\
  } else goto calli;\
 }\
 if DEE_UNLIKELY(!result) return -1;\
 tp = (DeeTypeObject *)*lhs;\
 Dee_INHERIT_REF(*lhs,result);\
 Dee_DECREF(tp);\
 return 0;\
}
DEE_PRIVATE_MAKE_BINARY_INPLACE_VAR_FUNCTOR(DeeObject_InplaceAddVar,tp_add,tp_iadd)
DEE_PRIVATE_MAKE_BINARY_INPLACE_VAR_FUNCTOR(DeeObject_InplaceSubVar,tp_sub,tp_isub);
DEE_PRIVATE_MAKE_BINARY_INPLACE_VAR_FUNCTOR(DeeObject_InplaceMulVar,tp_mul,tp_imul);
DEE_PRIVATE_MAKE_BINARY_INPLACE_VAR_FUNCTOR(DeeObject_InplaceDivVar,tp_div,tp_idiv);
DEE_PRIVATE_MAKE_BINARY_INPLACE_VAR_FUNCTOR(DeeObject_InplaceModVar,tp_mod,tp_imod);
DEE_PRIVATE_MAKE_BINARY_INPLACE_VAR_FUNCTOR(DeeObject_InplaceShlVar,tp_shl,tp_ishl);
DEE_PRIVATE_MAKE_BINARY_INPLACE_VAR_FUNCTOR(DeeObject_InplaceShrVar,tp_shr,tp_ishr);
DEE_PRIVATE_MAKE_BINARY_INPLACE_VAR_FUNCTOR(DeeObject_InplaceAndVar,tp_and,tp_iand);
DEE_PRIVATE_MAKE_BINARY_INPLACE_VAR_FUNCTOR(DeeObject_InplaceOrVar, tp_or, tp_ior);
DEE_PRIVATE_MAKE_BINARY_INPLACE_VAR_FUNCTOR(DeeObject_InplaceXorVar,tp_xor,tp_ixor);
DEE_PRIVATE_MAKE_BINARY_INPLACE_VAR_FUNCTOR(DeeObject_InplacePowVar,tp_pow,tp_ipow);
#undef DEE_PRIVATE_MAKE_BINARY_INPLACE_VAR_FUNCTOR

DEE_DECL_END

