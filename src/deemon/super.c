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
#ifndef GUARD_DEEMON_SUPER_C
#define GUARD_DEEMON_SUPER_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/super.h>
#include <deemon/tuple.h>
#include <deemon/optional/object_pool.h>
#include <deemon/optional/object_visit.h>

DEE_DECL_BEGIN

DEE_OBJECTPOOL_DEFINE(super,DeeSuperObject)

Dee_size_t DeeSuper_ClearPool(void) {
 return DEE_OBJECTPOOL_CLEAR(super);
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeSuperObject) *DeeSuper_New(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeSuperObject *result;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if ((result = DEE_OBJECTPOOL_ALLOC(super)) != NULL) {
  DeeObject_INIT(result,&DeeSuper_Type);
  Dee_INCREF(result->s_self = self);
  result->s_type = tp_self;
 }
 return (DeeObject *)result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeSuperObject) *DeeSuper_TOf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeTypeObject const *super_type; DeeSuperObject *result;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeSuper_Type) {
  if ((super_type = DeeType_BASE(DeeSuper_TYPE(self))) == NULL) {
   super_type = DeeSuper_TYPE(self);
   goto no_super;
  }
  self = DeeSuper_SELF(self);
 } else {
  if ((super_type = DeeType_BASE(tp_self)) == NULL) {
   super_type = Dee_TYPE(self);
no_super:
   DeeError_SetStringf(&DeeErrorType_TypeError,
                       "Type %q does not have a super class",
                       DeeType_NAME(super_type));
   return NULL;
  }
 }
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(super_type) && DeeType_Check(super_type));
 DEE_ASSERT(DeeObject_InstanceOf(self,super_type));
 if ((result = DEE_OBJECTPOOL_ALLOC(super)) != NULL) {
  DeeObject_INIT(result,&DeeSuper_Type);
  Dee_INCREF(result->s_self = self);
  result->s_type = super_type;
 }
 return (DeeObject *)result;
}

DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeObject *
DeeSuper_GetSelf(DEE_A_IN_OBJECT(DeeSuperObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSuper_Check(self));
 return DeeSuper_SELF(self);
}
DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeTypeObject const *
DeeSuper_GetType(DEE_A_IN_OBJECT(DeeSuperObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSuper_Check(self));
 return DeeSuper_TYPE(self);
}



static DeeSuperObject *DEE_CALL _deesuper_tp_alloc(DeeTypeObject *tp_self) {
 DeeSuperObject *result;
 if ((result = DEE_OBJECTPOOL_ALLOC(super)) != NULL)
  DeeObject_INIT(result,tp_self);
 return result;
}
static void DEE_CALL _deesuper_tp_free(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSuperObject *ob) {
 DEE_OBJECTPOOL_FREE(super,ob);
}
static void DEE_CALL _deesuper_tp_dtor(DeeSuperObject *self) {
 Dee_DECREF(self->s_self);
}
DEE_VISIT_PROC(_deesuper_tp_visit,DeeSuperObject *self) {
 Dee_VISIT(self->s_self);
}
static int DEE_CALL _deesuper_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeSuperObject *self, DeeSuperObject *right) {
 Dee_INCREF(self->s_self = right->s_self);
 self->s_type = right->s_type;
 return 0;
}
static int DEE_CALL _deesuper_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSuperObject *self, DeeObject *args) {
 DeeObject *ob; DeeTypeObject const *super_tp = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:super",&ob,&super_tp) != 0) return -1;
 if (super_tp) {
  if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&ob,(DeeTypeObject *)super_tp) != 0) return -1;
 } else {
  if (DeeSuper_Check(ob)) {
   if DEE_UNLIKELY((super_tp = DeeType_BASE(DeeSuper_TYPE(ob))) == NULL) {
    super_tp = DeeSuper_TYPE(ob);
    goto no_super;
   }
   ob = DeeSuper_SELF(ob);
  } else {
   if DEE_UNLIKELY((super_tp = DeeType_BASE(Dee_TYPE(ob))) == NULL) {
    super_tp = Dee_TYPE(ob);
no_super:
    DeeError_SetStringf(&DeeErrorType_TypeError,
                        "Type %q does not have a super class",
                        DeeType_NAME(super_tp));
    return -1;
   }
  }
 }
 DEE_ASSERT(DeeObject_InstanceOf(ob,super_tp));
 self->s_type = super_tp;
 Dee_INCREF(self->s_self = ob);
 return 0;
}


DeeObject *DEE_CALL _deesuper_tp_attr_get(DeeSuperObject *self, DeeObject *attr) { return DeeObject_TGetAttr(self->s_type,self->s_self,attr); }
int DEE_CALL _deesuper_tp_attr_del(DeeSuperObject *self, DeeObject *attr) { return DeeObject_TDelAttr(self->s_type,self->s_self,attr); }
int DEE_CALL _deesuper_tp_attr_set(DeeSuperObject *self, DeeObject *attr, DeeObject *v) { return DeeObject_TSetAttr(self->s_type,self->s_self,attr,v); }
static int DEE_CALL _deesuper_tp_copy_assign(DeeSuperObject *self, DeeObject *v) { return DeeObject_TCopyAssign(self->s_type,self->s_self,v); }
static int DEE_CALL _deesuper_tp_move_assign(DeeSuperObject *self, DeeObject *v) { return DeeObject_TMoveAssign(self->s_type,self->s_self,v); }
static int DEE_CALL _deesuper_tp_any_assign(DeeSuperObject *self, DeeObject *v) { return DeeObject_TAssign(self->s_type,self->s_self,v); }
static DeeObject *DEE_CALL _deesuper_tp_str(DeeSuperObject *self) { return DeeObject_TStr(self->s_type,self->s_self); }
static DeeObject *DEE_CALL _deesuper_tp_repr(DeeSuperObject *self) { return DeeObject_TRepr(self->s_type,self->s_self); }
static int DEE_CALL _deesuper_tp_double(DeeSuperObject *self, double *result) { return DeeObject_TGetDouble(self->s_type,self->s_self,result); }
static int DEE_CALL _deesuper_tp_int32(DeeSuperObject *self, Dee_int32_t *result) { return DeeObject_TGetInt32(self->s_type,self->s_self,result); }
static int DEE_CALL _deesuper_tp_int64(DeeSuperObject *self, Dee_int64_t *result) { return DeeObject_TGetInt64(self->s_type,self->s_self,result); }
static DeeObject *DEE_CALL _deesuper_tp_call(DeeSuperObject *self, DeeObject *args) { return DeeObject_TCall(self->s_type,self->s_self,args); }
static DeeObject *DEE_CALL _deesuper_tp_cmp_lo(DeeSuperObject *self, DeeObject *right) { return DeeObject_TCompareLoObject(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_cmp_le(DeeSuperObject *self, DeeObject *right) { return DeeObject_TCompareLeObject(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_cmp_eq(DeeSuperObject *self, DeeObject *right) { return DeeObject_TCompareEqObject(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_cmp_ne(DeeSuperObject *self, DeeObject *right) { return DeeObject_TCompareNeObject(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_cmp_gr(DeeSuperObject *self, DeeObject *right) { return DeeObject_TCompareGrObject(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_cmp_ge(DeeSuperObject *self, DeeObject *right) { return DeeObject_TCompareGeObject(self->s_type,self->s_self,right); }
static int DEE_CALL _deesuper_tp_bool(DeeSuperObject *self) { return DeeObject_TBool(self->s_type,self->s_self); }
static DeeObject *DEE_CALL _deesuper_tp_not(DeeSuperObject *self) { return DeeObject_TNot(self->s_type,self->s_self); }
static DeeObject *DEE_CALL _deesuper_tp_inv(DeeSuperObject *self) { return DeeObject_TInv(self->s_type,self->s_self); }
static DeeObject *DEE_CALL _deesuper_tp_pos(DeeSuperObject *self) { return DeeObject_TPos(self->s_type,self->s_self); }
static DeeObject *DEE_CALL _deesuper_tp_neg(DeeSuperObject *self) { return DeeObject_TNeg(self->s_type,self->s_self); }
static DeeObject *DEE_CALL _deesuper_tp_inc(DeeSuperObject *self) { return DeeObject_TInc(self->s_type,self->s_self); }
static DeeObject *DEE_CALL _deesuper_tp_dec(DeeSuperObject *self) { return DeeObject_TDec(self->s_type,self->s_self); }
static DeeObject *DEE_CALL _deesuper_tp_inc_post(DeeSuperObject *self) { return DeeObject_TIncPost(self->s_type,self->s_self); }
static DeeObject *DEE_CALL _deesuper_tp_dec_post(DeeSuperObject *self) { return DeeObject_TDecPost(self->s_type,self->s_self); }
static DeeObject *DEE_CALL _deesuper_tp_add(DeeSuperObject *self, DeeObject *right) { return DeeObject_TAdd(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_sub(DeeSuperObject *self, DeeObject *right) { return DeeObject_TSub(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_mul(DeeSuperObject *self, DeeObject *right) { return DeeObject_TMul(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_div(DeeSuperObject *self, DeeObject *right) { return DeeObject_TDiv(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_mod(DeeSuperObject *self, DeeObject *right) { return DeeObject_TMod(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_shl(DeeSuperObject *self, DeeObject *right) { return DeeObject_TShl(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_shr(DeeSuperObject *self, DeeObject *right) { return DeeObject_TShr(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_and(DeeSuperObject *self, DeeObject *right) { return DeeObject_TAnd(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_or (DeeSuperObject *self, DeeObject *right) { return DeeObject_TOr (self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_xor(DeeSuperObject *self, DeeObject *right) { return DeeObject_TXor(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_pow(DeeSuperObject *self, DeeObject *right) { return DeeObject_TPow(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_iadd(DeeSuperObject *self, DeeObject *right) { return DeeObject_TInplaceAdd(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_isub(DeeSuperObject *self, DeeObject *right) { return DeeObject_TInplaceSub(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_imul(DeeSuperObject *self, DeeObject *right) { return DeeObject_TInplaceMul(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_idiv(DeeSuperObject *self, DeeObject *right) { return DeeObject_TInplaceDiv(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_imod(DeeSuperObject *self, DeeObject *right) { return DeeObject_TInplaceMod(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_ishl(DeeSuperObject *self, DeeObject *right) { return DeeObject_TInplaceShl(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_ishr(DeeSuperObject *self, DeeObject *right) { return DeeObject_TInplaceShr(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_iand(DeeSuperObject *self, DeeObject *right) { return DeeObject_TInplaceAnd(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_ior (DeeSuperObject *self, DeeObject *right) { return DeeObject_TInplaceOr (self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_ixor(DeeSuperObject *self, DeeObject *right) { return DeeObject_TInplaceXor(self->s_type,self->s_self,right); }
static DeeObject *DEE_CALL _deesuper_tp_ipow(DeeSuperObject *self, DeeObject *right) { return DeeObject_TInplacePow(self->s_type,self->s_self,right); }
static int DEE_CALL _deesuper_tp_hash(DeeSuperObject *self, Dee_hash_t start, Dee_hash_t *result) { return DeeObject_THashEx(self->s_type,self->s_self,start,result); }
static DeeObject *DEE_CALL _deesuper_tp_seq_get(DeeSuperObject *self, DeeObject *item) { return DeeObject_TGetItem(self->s_type,self->s_self,item); }
static int DEE_CALL _deesuper_tp_seq_del(DeeSuperObject *self, DeeObject *item) { return DeeObject_TDelItem(self->s_type,self->s_self,item); }
static int DEE_CALL _deesuper_tp_seq_set(DeeSuperObject *self, DeeObject *item, DeeObject *v) { return DeeObject_TSetItem(self->s_type,self->s_self,item,v); }
static DeeObject *DEE_CALL _deesuper_tp_seq_range_get(DeeSuperObject *self, DeeObject *lo, DeeObject *hi) { return DeeObject_TGetRangeItem(self->s_type,self->s_self,lo,hi); }
static int DEE_CALL _deesuper_tp_seq_range_del(DeeSuperObject *self, DeeObject *lo, DeeObject *hi) { return DeeObject_TDelRangeItem(self->s_type,self->s_self,lo,hi); }
static int DEE_CALL _deesuper_tp_seq_range_set(DeeSuperObject *self, DeeObject *lo, DeeObject *hi, DeeObject *v) { return DeeObject_TSetRangeItem(self->s_type,self->s_self,lo,hi,v); }
static DeeObject *DEE_CALL _deesuper_tp_seq_size(DeeSuperObject *self) { return DeeObject_TSizeObject(self->s_type,self->s_self); }
static DeeObject *DEE_CALL _deesuper_tp_seq_contains(DeeSuperObject *self, DeeObject *ob) { return DeeObject_TContainsObject(self->s_type,self->s_self,ob); }
static DeeObject *DEE_CALL _deesuper_tp_seq_iter_self(DeeSuperObject *self) { return DeeObject_TIterSelf(self->s_type,self->s_self); }
static int DEE_CALL _deesuper_tp_seq_iter_next(DeeSuperObject *self, DeeObject **result) { return DeeObject_TIterNextEx(self->s_type,self->s_self,result); }

DeeTypeObject DeeSuper_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("super"),member(
  "(super ob) -> super\n"
  "(object ob) -> super\n"
  "(object ob, type object tp_ob) -> super\n"
  "\tReturns a symbolic proxy objects with limited access to only a given super-class of an object.\n"
  "\tNote that the parameter order is the reverse of the '__super' builtin keyword."),
  member(DEE_TYPE_FLAG_DONT_COPY|DEE_TYPE_FLAG_CONST_CTOR|
         DEE_TYPE_FLAG_NO_SUBCLASS|DEE_TYPE_FLAG_FUNDAMENTAL),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeSuperObject),
  member(&_deesuper_tp_alloc),null,
  member(&_deesuper_tp_copy_ctor),null,
  member(&_deesuper_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(
  member(&_deesuper_tp_free),
  member(&_deesuper_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deesuper_tp_copy_assign),
  member(&_deesuper_tp_move_assign),
  member(&_deesuper_tp_any_assign)),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deesuper_tp_str),member(&_deesuper_tp_repr),
  member(&_deesuper_tp_int32),member(&_deesuper_tp_int64),
  member(&_deesuper_tp_double)),
 DEE_TYPE_OBJECT_OBJECT_v100(
  member(&_deesuper_tp_call),member(&_deesuper_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deesuper_tp_not),member(&_deesuper_tp_bool),member(&_deesuper_tp_inv),
  member(&_deesuper_tp_pos),member(&_deesuper_tp_neg),
  member(&_deesuper_tp_inc),member(&_deesuper_tp_inc_post),
  member(&_deesuper_tp_dec),member(&_deesuper_tp_dec_post),
  member(&_deesuper_tp_add),member(&_deesuper_tp_iadd),
  member(&_deesuper_tp_sub),member(&_deesuper_tp_isub),
  member(&_deesuper_tp_mul),member(&_deesuper_tp_imul),
  member(&_deesuper_tp_div),member(&_deesuper_tp_idiv),
  member(&_deesuper_tp_mod),member(&_deesuper_tp_imod),
  member(&_deesuper_tp_shl),member(&_deesuper_tp_ishl),
  member(&_deesuper_tp_shr),member(&_deesuper_tp_ishr),
  member(&_deesuper_tp_and),member(&_deesuper_tp_iand),
  member(&_deesuper_tp_or),member(&_deesuper_tp_ior),
  member(&_deesuper_tp_xor),member(&_deesuper_tp_ixor),
  member(&_deesuper_tp_pow),member(&_deesuper_tp_ipow),
  member(&_deesuper_tp_hash)),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deesuper_tp_cmp_lo),member(&_deesuper_tp_cmp_le),
  member(&_deesuper_tp_cmp_eq),member(&_deesuper_tp_cmp_ne),
  member(&_deesuper_tp_cmp_gr),member(&_deesuper_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&_deesuper_tp_seq_get),member(&_deesuper_tp_seq_del),
  member(&_deesuper_tp_seq_set),member(&_deesuper_tp_seq_size),
  member(&_deesuper_tp_seq_contains),member(&_deesuper_tp_seq_range_get),
  member(&_deesuper_tp_seq_range_del),member(&_deesuper_tp_seq_range_set),
  member(&_deesuper_tp_seq_iter_self),member(&_deesuper_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(
  member(&_deesuper_tp_attr_get),member(&_deesuper_tp_attr_del),
  member(&_deesuper_tp_attr_set),null,null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_SUPER_C */
