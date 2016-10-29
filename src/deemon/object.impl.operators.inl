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
#include <deemon/error.h>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/type_decl.h>
#include <deemon/optional/type_flags.h>
#include <deemon/optional/type_slots.h>
#include <deemon/structured.h>
#include <deemon/type.h>


DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeObject_TCopyAssign(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *right) {
 DeeType_SLOT_TYPE(tp_any_assign) func;
 DeeTypeObject const *tp_iter,*rtp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(right));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; rtp = Dee_TYPE(right); while (1) {
  if DEE_LIKELY(DeeType_IsSameOrDerived(rtp,tp_iter)) {
   while (1) {
    if ((func = DeeType_GET_SLOT(tp_iter,tp_copy_assign)) != NULL ||
        (func = DeeType_GET_SLOT(tp_iter,tp_any_assign)) != NULL
        ) return (*func)(self,right);
    tp_iter = DeeType_BASE(tp_iter);
   }
  }
  if ((func = DeeType_GET_SLOT(tp_iter,tp_any_assign)) != NULL)
   return (*func)(self,right);
  tp_iter = DeeType_BASE(tp_iter);
 }
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TMoveAssign(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *right) {
 DeeType_SLOT_TYPE(tp_any_assign) func;
 DeeTypeObject const *tp_iter,*rtp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(right));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; rtp = Dee_TYPE(right); while (1) {
  if DEE_LIKELY(DeeType_IsSameOrDerived(rtp,tp_iter)) {
   while (1) {
    if ((func = DeeType_GET_SLOT(tp_iter,tp_move_assign)) != NULL ||
      //(func = DeeType_GET_SLOT(tp_iter,tp_copy_assign)) != NULL || // would have been assigned
        (func = DeeType_GET_SLOT(tp_iter,tp_any_assign)) != NULL
        ) return (*func)(self,right);
    tp_iter = DeeType_BASE(tp_iter);
   }
  }
  if ((func = DeeType_GET_SLOT(tp_iter,tp_any_assign)) != NULL)
   return (*func)(self,right);
  tp_iter = DeeType_BASE(tp_iter);
 }
}


DEE_A_RET_EXCEPT(-1) int DeeObject_TBool(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(self));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_bool);
 return (*DeeType_GET_SLOT(tp_iter,tp_bool))(self);
}
#define DEE_PRIVATE_MAKE_TUNARY_FUNCTOR(name,slot)\
DEE_A_RET_EXCEPT_REF DeeObject *name(\
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {\
 DeeTypeObject const *tp_iter;\
 DEE_ASSERT(DeeObject_Check(self));\
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));\
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));\
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,slot);\
 return (*DeeType_GET_SLOT(tp_iter,slot))(self);\
}
#define DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(name,slot)\
DEE_A_RET_EXCEPT_REF DeeObject *name(\
 DEE_A_IN DeeTypeObject const *tp_lhs,\
 DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) {\
 DeeTypeObject const *tp_iter;\
 DEE_ASSERT(DeeObject_Check(lhs));\
 DEE_ASSERT(DeeObject_Check(rhs));\
 DEE_ASSERT(DeeObject_Check(tp_lhs) && DeeType_Check(tp_lhs));\
 DEE_ASSERT(DeeObject_InstanceOf(lhs,tp_lhs));\
 tp_iter = tp_lhs; DeeType_FIND_SLOT(tp_iter,slot);\
 return (*DeeType_GET_SLOT(tp_iter,slot))(lhs,rhs);\
}
DEE_PRIVATE_MAKE_TUNARY_FUNCTOR(DeeObject_TNot,tp_not)
DEE_PRIVATE_MAKE_TUNARY_FUNCTOR(DeeObject_TInv,tp_inv)
DEE_PRIVATE_MAKE_TUNARY_FUNCTOR(DeeObject_TPos,tp_pos)
DEE_PRIVATE_MAKE_TUNARY_FUNCTOR(DeeObject_TNeg,tp_neg)
DEE_PRIVATE_MAKE_TUNARY_FUNCTOR(DeeObject_TInc,tp_inc)
DEE_PRIVATE_MAKE_TUNARY_FUNCTOR(DeeObject_TDec,tp_dec)
DEE_PRIVATE_MAKE_TUNARY_FUNCTOR(DeeObject_TIncPost,tp_incpost)
DEE_PRIVATE_MAKE_TUNARY_FUNCTOR(DeeObject_TDecPost,tp_decpost)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TAdd,tp_add)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TSub,tp_sub)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TMul,tp_mul)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TDiv,tp_div)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TMod,tp_mod)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TShl,tp_shl)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TShr,tp_shr)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TAnd,tp_and)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TOr,tp_or)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TXor,tp_xor)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TPow,tp_pow)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TInplaceAdd,tp_iadd)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TInplaceSub,tp_isub)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TInplaceMul,tp_imul)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TInplaceDiv,tp_idiv)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TInplaceMod,tp_imod)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TInplaceShl,tp_ishl)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TInplaceShr,tp_ishr)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TInplaceAnd,tp_iand)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TInplaceOr,tp_ior)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TInplaceXor,tp_ixor)
DEE_PRIVATE_MAKE_TBINARY_FUNCTOR(DeeObject_TInplacePow,tp_ipow)
#undef DEE_PRIVATE_MAKE_TBINARY_FUNCTOR
#undef DEE_PRIVATE_MAKE_TUNARY_FUNCTOR

DEE_A_RET_EXCEPT(-1) int DeeObject_THashEx(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(result);
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_hash);
 return (*DeeType_GET_SLOT(tp_iter,tp_hash))(self,start,result);
}

DEE_A_RET_EXCEPT(-1) int DeeObject_PHash(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result) {
 DeeStructuredTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_hash))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_hash))(
    (DeeStructuredTypeObject *)tp_iter,base_addr,start,result);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__hash__()",
                     DeeType_NAME(tp_self));
 return -1;
}

DEE_A_RET_EXCEPT(-1) int DeeObject_PBool(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) {
 DeeStructuredTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_bool))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_bool))(
    (DeeStructuredTypeObject *)tp_iter,base_addr);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__bool__()",
                     DeeType_NAME(tp_self));
 return -1;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PInc(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) {
 DeeStructuredTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_inc))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_inc))(
    (DeeStructuredTypeObject *)tp_iter,base_addr);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__inc__()",
                     DeeType_NAME(tp_self));
 return -1;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PDec(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) {
 DeeStructuredTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_dec))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_dec))(
    (DeeStructuredTypeObject *)tp_iter,base_addr);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__dec__()",
                     DeeType_NAME(tp_self));
 return -1;
}

#define DEE_PRIVATE_MAKE_PUNARY_FUNCTOR(name,slot,name_str)\
DEE_A_RET_EXCEPT_REF DeeObject *name(\
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) {\
 DeeStructuredTypeObject *tp_iter;\
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));\
 tp_iter = (DeeStructuredTypeObject *)tp_self;\
 while (1) {\
  if (DeeType_HAS_SLOT(tp_iter,slot))\
   return (*DeeType_GET_SLOT(tp_iter,slot))(\
    (DeeStructuredTypeObject *)tp_iter,base_addr);\
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);\
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;\
 }\
 DeeError_SetStringf(&DeeErrorType_NotImplemented,\
                     "Not implemented: %s." name_str "()",\
                     DeeType_NAME(tp_self));\
 return NULL;\
}
#define DEE_PRIVATE_MAKE_PBINARY_FUNCTOR(name,slot,name_str)\
DEE_A_RET_EXCEPT_REF DeeObject *name(\
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, \
 DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) {\
 DeeStructuredTypeObject const *tp_iter;\
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));\
 tp_iter = (DeeStructuredTypeObject const *)tp_self;\
 while (1) {\
  if (DeeType_HAS_SLOT(tp_iter,slot))\
   return (*DeeType_GET_SLOT(tp_iter,slot))(\
    (DeeStructuredTypeObject *)tp_iter,lhs_base_addr,rhs);\
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);\
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;\
 }\
 DeeError_SetStringf(&DeeErrorType_NotImplemented,\
                     "Not implemented: %s." name_str "(%s)",\
                     DeeType_NAME(tp_self),\
                     DeeType_NAME(Dee_TYPE(rhs)));\
 return NULL;\
}
#define DEE_PRIVATE_MAKE_PBINARY_IFUNCTOR(name,slot,name_str)\
DEE_A_RET_EXCEPT(-1) int name(\
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, \
 DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) {\
 DeeStructuredTypeObject const *tp_iter;\
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));\
 tp_iter = (DeeStructuredTypeObject const *)tp_self;\
 while (1) {\
  if (DeeType_HAS_SLOT(tp_iter,slot))\
   return (*DeeType_GET_SLOT(tp_iter,slot))(\
    (DeeStructuredTypeObject *)tp_iter,lhs_base_addr,rhs);\
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);\
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;\
 }\
 DeeError_SetStringf(&DeeErrorType_NotImplemented,\
                     "Not implemented: %s." name_str "(%s)",\
                     DeeType_NAME(tp_self),\
                     DeeType_NAME(Dee_TYPE(rhs)));\
 return -1;\
}
DEE_PRIVATE_MAKE_PUNARY_FUNCTOR(DeeObject_PNot,tp_p_not,"__not__")
DEE_PRIVATE_MAKE_PUNARY_FUNCTOR(DeeObject_PInv,tp_p_inv,"__inv__")
DEE_PRIVATE_MAKE_PUNARY_FUNCTOR(DeeObject_PPos,tp_p_pos,"__pos__")
DEE_PRIVATE_MAKE_PUNARY_FUNCTOR(DeeObject_PNeg,tp_p_neg,"__neg__")
DEE_PRIVATE_MAKE_PUNARY_FUNCTOR(DeeObject_PIncPost,tp_p_incpost,"__incpost__")
DEE_PRIVATE_MAKE_PUNARY_FUNCTOR(DeeObject_PDecPost,tp_p_decpost,"__decpost__")
DEE_PRIVATE_MAKE_PBINARY_FUNCTOR(DeeObject_PAdd,tp_p_add,"__add__")
DEE_PRIVATE_MAKE_PBINARY_FUNCTOR(DeeObject_PSub,tp_p_sub,"__sub__")
DEE_PRIVATE_MAKE_PBINARY_FUNCTOR(DeeObject_PMul,tp_p_mul,"__mul__")
DEE_PRIVATE_MAKE_PBINARY_FUNCTOR(DeeObject_PDiv,tp_p_div,"__div__")
DEE_PRIVATE_MAKE_PBINARY_FUNCTOR(DeeObject_PMod,tp_p_mod,"__mod__")
DEE_PRIVATE_MAKE_PBINARY_FUNCTOR(DeeObject_PShl,tp_p_shl,"__shl__")
DEE_PRIVATE_MAKE_PBINARY_FUNCTOR(DeeObject_PShr,tp_p_shr,"__shr__")
DEE_PRIVATE_MAKE_PBINARY_FUNCTOR(DeeObject_PAnd,tp_p_and,"__and__")
DEE_PRIVATE_MAKE_PBINARY_FUNCTOR(DeeObject_POr,tp_p_or,"__or__")
DEE_PRIVATE_MAKE_PBINARY_FUNCTOR(DeeObject_PXor,tp_p_xor,"__xor__")
DEE_PRIVATE_MAKE_PBINARY_FUNCTOR(DeeObject_PPow,tp_p_pow,"__pow__")
DEE_PRIVATE_MAKE_PBINARY_IFUNCTOR(DeeObject_PInplaceAdd,tp_p_iadd,"__iadd__")
DEE_PRIVATE_MAKE_PBINARY_IFUNCTOR(DeeObject_PInplaceSub,tp_p_isub,"__isub__")
DEE_PRIVATE_MAKE_PBINARY_IFUNCTOR(DeeObject_PInplaceMul,tp_p_imul,"__imul__")
DEE_PRIVATE_MAKE_PBINARY_IFUNCTOR(DeeObject_PInplaceDiv,tp_p_idiv,"__idiv__")
DEE_PRIVATE_MAKE_PBINARY_IFUNCTOR(DeeObject_PInplaceMod,tp_p_imod,"__imod__")
DEE_PRIVATE_MAKE_PBINARY_IFUNCTOR(DeeObject_PInplaceShl,tp_p_ishl,"__ishl__")
DEE_PRIVATE_MAKE_PBINARY_IFUNCTOR(DeeObject_PInplaceShr,tp_p_ishr,"__ishr__")
DEE_PRIVATE_MAKE_PBINARY_IFUNCTOR(DeeObject_PInplaceAnd,tp_p_iand,"__iand__")
DEE_PRIVATE_MAKE_PBINARY_IFUNCTOR(DeeObject_PInplaceOr,tp_p_ior,"__ior__")
DEE_PRIVATE_MAKE_PBINARY_IFUNCTOR(DeeObject_PInplaceXor,tp_p_ixor,"__ixor__")
DEE_PRIVATE_MAKE_PBINARY_IFUNCTOR(DeeObject_PInplacePow,tp_p_ipow,"__ipow__")
#undef DEE_PRIVATE_MAKE_PBINARY_IFUNCTOR
#undef DEE_PRIVATE_MAKE_PBINARY_FUNCTOR
#undef DEE_PRIVATE_MAKE_PUNARY_FUNCTOR

DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TIterSelf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_seq_iter_self);
 return (*DeeType_GET_SLOT(tp_iter,tp_seq_iter_self))(self);
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeObject_TIterNextEx(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_REF DEE_A_OUT DeeObject **elem) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(elem);
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_seq_iter_next);
#ifdef DEE_DEBUG
 {
  int error;
  *elem = NULL;
  error = (*DeeType_GET_SLOT(tp_iter,tp_seq_iter_next))(self,elem);
  if DEE_LIKELY(error == 0) DEE_ASSERT(DeeObject_Check(*elem));
  return error;
 }
#else
 return (*DeeType_GET_SLOT(tp_iter,tp_seq_iter_next))(self,elem);
#endif
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TSizeObject(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_seq_size);
 return (*DeeType_GET_SLOT(tp_iter,tp_seq_size))(self);
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TContainsObject(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *elem) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(elem));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_seq_contains);
 return (*DeeType_GET_SLOT(tp_iter,tp_seq_contains))(self,elem);
}

DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PIterSelf(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_seq_iter_self))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_seq_iter_self))(
    (DeeStructuredTypeObject *)tp_iter,base_addr);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__iterself__()",
                     DeeType_NAME(tp_self));
 return NULL;
}

DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PSizeObject(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_seq_size))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_seq_size))(
    (DeeStructuredTypeObject *)tp_iter,base_addr);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__size__()",
                     DeeType_NAME(tp_self));
 return NULL;
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PContainsObject(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *elem) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_seq_contains))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_seq_contains))(
    (DeeStructuredTypeObject *)tp_iter,base_addr,elem);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__contains__(%s)",
                     DeeType_NAME(tp_self),
                     DeeType_NAME(Dee_TYPE(elem)));
 return NULL;
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TGetItem(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *key) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(key));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_seq_get);
 return (*DeeType_GET_SLOT(tp_iter,tp_seq_get))(self,key);
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TDelItem(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *key) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(key));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_seq_del);
 return (*DeeType_GET_SLOT(tp_iter,tp_seq_del))(self,key);
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TSetItem(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(key));
 DEE_ASSERT(DeeObject_Check(item));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_seq_set);
 return (*DeeType_GET_SLOT(tp_iter,tp_seq_set))(self,key,item);
}

DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TGetRangeItem(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *lo, DEE_A_INOUT DeeObject *hi) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(lo));
 DEE_ASSERT(DeeObject_Check(hi));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_seq_range_get);
 return (*DeeType_GET_SLOT(tp_iter,tp_seq_range_get))(self,lo,hi);
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TDelRangeItem(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *lo, DEE_A_INOUT DeeObject *hi) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(lo));
 DEE_ASSERT(DeeObject_Check(hi));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_seq_range_del);
 return (*DeeType_GET_SLOT(tp_iter,tp_seq_range_del))(self,lo,hi);
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TSetRangeItem(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *lo,
 DEE_A_INOUT DeeObject *hi, DEE_A_INOUT DeeObject *v) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(lo));
 DEE_ASSERT(DeeObject_Check(hi));
 DEE_ASSERT(DeeObject_Check(v));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_seq_range_set);
 return (*DeeType_GET_SLOT(tp_iter,tp_seq_range_set))(self,lo,hi,v);
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PGetItem(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *key) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_seq_get))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_seq_get))(
    (DeeStructuredTypeObject *)tp_iter,base_addr,key);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__getitem__(%s)",
                     DeeType_NAME(tp_self),
                     DeeType_NAME(Dee_TYPE(key)));
 return NULL;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PDelItem(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *key) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_seq_del))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_seq_del))(
    (DeeStructuredTypeObject *)tp_iter,base_addr,key);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__delitem__(%s)",
                     DeeType_NAME(tp_self),
                     DeeType_NAME(Dee_TYPE(key)));
 return -1;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PSetItem(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_seq_set))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_seq_set))(
    (DeeStructuredTypeObject *)tp_iter,base_addr,key,item);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__setitem__(%s,%s)",
                     DeeType_NAME(tp_self),
                     DeeType_NAME(Dee_TYPE(key)),
                     DeeType_NAME(Dee_TYPE(item)));
 return -1;
}

DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PGetRangeItem(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *lo, DEE_A_INOUT DeeObject *hi) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_seq_range_get))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_seq_range_get))(
    (DeeStructuredTypeObject *)tp_iter,base_addr,lo,hi);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__getrange__(%s,%s)",
                     DeeType_NAME(tp_self),
                     DeeType_NAME(Dee_TYPE(lo)),
                     DeeType_NAME(Dee_TYPE(hi)));
 return NULL;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PDelRangeItem(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *lo, DEE_A_INOUT DeeObject *hi) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_seq_range_del))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_seq_range_del))(
    (DeeStructuredTypeObject *)tp_iter,base_addr,lo,hi);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__delrange__(%s,%s)",
                     DeeType_NAME(tp_self),
                     DeeType_NAME(Dee_TYPE(lo)),
                     DeeType_NAME(Dee_TYPE(hi)));
 return -1;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PSetRangeItem(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *lo,
 DEE_A_INOUT DeeObject *hi, DEE_A_INOUT DeeObject *v) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_seq_range_set))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_seq_range_set))(
    (DeeStructuredTypeObject *)tp_iter,base_addr,lo,hi,v);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__setrange__(%s,%s,%s)",
                     DeeType_NAME(tp_self),
                     DeeType_NAME(Dee_TYPE(lo)),
                     DeeType_NAME(Dee_TYPE(hi)),
                     DeeType_NAME(Dee_TYPE(v)));
 return -1;
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PGetAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_attr_get))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_attr_get))(tp_iter,base_addr,(DeeObject *)attr);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 return DeeObject_PGenericGetAttrString(tp_self,base_addr,DeeString_STR(attr));
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PDelAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_attr_del))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_attr_del))(tp_iter,base_addr,(DeeObject *)attr);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 return DeeObject_PGenericDelAttr(tp_self,base_addr,attr);
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PSetAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_attr_set))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_attr_set))(tp_iter,base_addr,(DeeObject *)attr,v);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 return DeeObject_PGenericSetAttr(tp_self,base_addr,attr,v);
}


DEE_DECL_END

