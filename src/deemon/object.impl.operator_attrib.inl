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
#include <deemon/bool.h>
#include <deemon/error.h>
#include <deemon/integer.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/object.h>
#include <deemon/optional/memberdef_decl.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/type_decl.h>
#include <deemon/optional/type_flags.h>
#include <deemon/optional/type_slots.h>
#include <deemon/string.h>
#include <deemon/structured.h>
#include <deemon/super.h>
#include <deemon/tuple.h>
#include <deemon/type.h>


DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// Generic functions
DeeObject *_DeeGenericFallback_tp_new(DeeTypeObject *tp) { DeeError_SetStringf(&DeeErrorType_NotImplemented,"Not implemented: %s.__constructor__()",DeeType_NAME(tp)); return NULL; }
DeeObject *_DeeGenericFallback_tp_copy_new(DeeTypeObject *tp, DeeObject *DEE_UNUSED(self)) { DeeError_SetStringf(&DeeErrorType_NotImplemented,"Not implemented: %s.__copy__()",DeeType_NAME(tp)); return NULL; }
DeeObject *_DeeGenericFallback_tp_move_new(DeeTypeObject *tp, DeeObject *DEE_UNUSED(self)) { DeeError_SetStringf(&DeeErrorType_NotImplemented,"Not implemented: %s.__move__()",DeeType_NAME(tp)); return NULL; }
DeeObject *_DeeGenericFallback_tp_any_new(DeeTypeObject *tp, DeeObject *args) { DeeError_SetStringf(&DeeErrorType_NotImplemented,"Not implemented: %s.__constructor__%K",DeeType_NAME(tp),DeeTuple_Types(args)); return NULL; }

DeeObject *_DeeGenericFallback_tp_var_alloc(DeeTypeObject *tp) {
 DeeError_SetStringf(&DeeErrorType_TypeError,
                     "tp_alloc isn't supported by var types like %s",
                     DeeType_NAME(tp));
 return NULL;
}
DeeObject *_DeeGenericFallback_tp_alloc(DeeTypeObject *tp) {
 DeeObject *result; DEE_ASSERT(DeeObject_Check(tp));
 DEE_ASSERT((DeeType_FLAGS(tp)&DEE_TYPE_FLAG_VAR_OBJECT)==0 &&
            "Var object with regular alloc function");
 DEE_ASSERT((DeeType_FLAGS(tp)&DEE_TYPE_FLAG_HAS_GC)==0 &&
            "GC Type with regular alloc function");
 if (DEE_LIKELY((result = (DeeObject *)DeeObject_Mallocf(
  DeeType_GET_SLOT(tp,tp_instance_size),"Instance of %s",DeeType_NAME(tp))
  ) != NULL)) DeeObject_INIT(result,tp);
 return result;
}

void _DeeGenericFallback_tp_free(
#ifdef DEE_DEBUG
 DeeTypeObject *tp,
#else
 DeeTypeObject *DEE_UNUSED(tp),
#endif
 DeeObject *self) {
#ifdef DEE_DEBUG
 DEE_ASSERT(DeeObject_Check(tp) && DeeType_Check(tp));
 DEE_ASSERT((DeeType_FLAGS(tp)&DEE_TYPE_FLAG_HAS_GC)==0 &&
            "GC Type with regular free function");
#endif
 // NOTE: DeeObject_Check(self) fails at this point!
 DEE_ASSERT(self);
 DeeObject_Free(self);
}
DeeObject *_DeeGenericFallback_tp_new_callany(DeeTypeObject *tp) {
 return (*DeeType_GET_SLOT(tp,tp_any_new))(tp,Dee_EmptyTuple);
}
DeeObject *_DeeGenericFallback_tp_copy_new_callany(DeeTypeObject *tp, DeeObject *arg0) {
 DeeObject *args,*result;
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,arg0)) == NULL) return NULL;
 result = (*DeeType_GET_SLOT(tp,tp_any_new))(tp,args);
 Dee_DECREF(args);
 return result;
}
int _DeeGenericFallback_tp_ctor_callany(DeeTypeObject *tp, DeeObject *self) {
 DEE_ASSERT(DeeObject_Check(self));
 return (*DeeType_GET_SLOT(tp,tp_any_ctor))(tp,self,Dee_EmptyTuple);
}
int _DeeGenericFallback_tp_copy_ctor_callany(
 DeeTypeObject *tp, DeeObject *self, DeeObject *arg0) {
 int result; DeeObject *args;
 DEE_ASSERT(DeeObject_Check(self));
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,arg0)) == NULL) return -1;
 result = (*DeeType_GET_SLOT(tp,tp_any_ctor))(tp,self,args);
 Dee_DECREF(args);
 return result;
}

DeeObject *_DeeGenericContext_tp_incpost_from_tp_inc(DeeObject *self) {
 DeeObject *ob_copy;
 if DEE_UNLIKELY((ob_copy = DeeObject_Copy(self)) == NULL) return NULL;
 if DEE_UNLIKELY((self = DeeObject_Inc(self)) == NULL) Dee_CLEAR(ob_copy);
 else Dee_DECREF(self);
 return ob_copy;
}
DeeObject *_DeeGenericContext_tp_decpost_from_tp_dec(DeeObject *self) {
 DeeObject *ob_copy;
 if DEE_UNLIKELY((ob_copy = DeeObject_Copy(self)) == NULL) return NULL;
 if DEE_UNLIKELY((self = DeeObject_Dec(self)) == NULL) Dee_CLEAR(ob_copy);
 else Dee_DECREF(self);
 return ob_copy;
}
int _DeeGenericContext_tp_int32_from_tp_int64(DeeObject *self, Dee_int32_t *result) { Dee_int64_t result2; if ((*DeeType_GET_SLOT(Dee_TYPE(self),tp_int64))(self,&result2) != 0) return -1; *result = (Dee_int32_t)result2; return 0; }
int _DeeGenericContext_tp_int32_from_tp_double(DeeObject *self, Dee_int32_t *result) { double result2; if ((*DeeType_GET_SLOT(Dee_TYPE(self),tp_double))(self,&result2) != 0) return -1; *result = (Dee_int32_t)result2; return 0; }
int _DeeGenericContext_tp_int64_from_tp_int32(DeeObject *self, Dee_int64_t *result) { Dee_int32_t result2; if ((*DeeType_GET_SLOT(Dee_TYPE(self),tp_int32))(self,&result2) != 0) return -1; *result = (Dee_int64_t)result2; return 0; }
int _DeeGenericContext_tp_int64_from_tp_double(DeeObject *self, Dee_int64_t *result) { double result2; if ((*DeeType_GET_SLOT(Dee_TYPE(self),tp_double))(self,&result2) != 0) return -1; *result = (Dee_int64_t)result2; return 0; }
int _DeeGenericContext_tp_double_from_tp_int32(DeeObject *self, double *result) { Dee_int32_t result2; if ((*DeeType_GET_SLOT(Dee_TYPE(self),tp_int32))(self,&result2) != 0) return -1; *result = (double)result2; return 0; }
int _DeeGenericContext_tp_double_from_tp_int64(DeeObject *self, double *result) { Dee_int64_t result2; if ((*DeeType_GET_SLOT(Dee_TYPE(self),tp_int64))(self,&result2) != 0) return -1; *result = (double)result2; return 0; }
DeeObject *_DeeGeneric_ReturnSelf(DeeObject *self) { DeeReturn_NEWREF(self); }
int _DeeGeneric_Noop(DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *DEE_UNUSED(self)) { return 0; }
struct DeeMemberDef const _DeeGenericFallback_MemberDef[1] = {DEE_MEMBERDEF_END_v100};
struct DeeGetSetDef const _DeeGenericFallback_GetSetDef[1] = {DEE_GETSETDEF_END_v100};
struct DeeMethodDef const _DeeGenericFallback_MethodDef[1] = {DEE_METHODDEF_END_v100};






//////////////////////////////////////////////////////////////////////////
// Fallback implementations for missing constructors
int _DeeGenericMissing_tp_ctor(DeeTypeObject *tp, DeeObject *self) {
 tp = DeeType_BASE(tp);
 return (*DeeType_GET_SLOT(tp,tp_ctor))(tp,self);
}
int _DeeGenericMissing_tp_copy_ctor(
 DeeTypeObject *tp, DeeObject *self, DeeObject *right) {
 tp = DeeType_BASE(tp);
 return (*DeeType_GET_SLOT(tp,tp_copy_ctor))(tp,self,right);
}
int _DeeGenericMissing_tp_move_ctor(
 DeeTypeObject *tp, DeeObject *self, DeeObject *right) {
 tp = DeeType_BASE(tp);
 return (*DeeType_GET_SLOT(tp,tp_move_ctor))(tp,self,right);
}
int _DeeGenericMissing_tp_any_ctor(DeeTypeObject *tp, DeeObject *self, DeeObject *args) {
 tp = DeeType_BASE(tp);
 return (*DeeType_GET_SLOT(tp,tp_any_ctor))(tp,self,args);
}
DeeObject *_DeeGenericContext_tp_not_from_bool(DeeObject *self) {
 int temp;
 if (DEE_UNLIKELY((temp = DeeObject_Bool(self)) < 0)) return NULL;
 DeeReturn_Bool(!temp);
}













//////////////////////////////////////////////////////////////////////////
// Virtual object functions
static DeeObject *_deeobject_tp_str(DeeObject *self) {
 DEE_ASSERT(DeeObject_Check(self));
 return DeeString_Newf("<%#q>",DeeType_NAME(Dee_TYPE(self)));
}
static DeeObject *_deeobject_tp_repr(DeeObject *self) {
 DEE_ASSERT(DeeObject_Check(self));
 if (DeeObject_IS_VAR(self)) {
  // TODO: Add the id value, as returned by 'util::id()'
  return DeeString_Newf("<%#q>",DeeType_NAME(Dee_TYPE(self)));
 }
 return DeeString_Newf("<%k: %K>",self,DeeString_NewHexDump(
  self+1,DeeType_GET_SLOT(Dee_TYPE(self),tp_instance_size)-sizeof(DeeObject)));
}
#define NOTIMPLEMENTED_OPERATOR(self,name) \
 (DEE_ASSERT(DeeObject_Check(self)),\
  DeeError_SetStringf(&DeeErrorType_NotImplemented,\
  "Not implemented: %s." name "()",DeeType_NAME(Dee_TYPE(self))))
#define NOTIMPLEMENTED_OPERATOR_2(self,right,name) \
 (DEE_ASSERT(DeeObject_Check(self)),DEE_ASSERT(DeeObject_Check(right)),\
  DeeError_SetStringf(&DeeErrorType_NotImplemented,\
  "Not implemented: %s." name "(%s)",\
  DeeType_NAME(Dee_TYPE(self)),\
  DeeType_NAME(Dee_TYPE(right))))
#define NOTIMPLEMENTED_OPERATOR_3(self,a,b,name) \
 (DEE_ASSERT(DeeObject_Check(self)),\
  DEE_ASSERT(DeeObject_Check(a)),\
  DEE_ASSERT(DeeObject_Check(b)),\
  DeeError_SetStringf(&DeeErrorType_NotImplemented,\
  "Not implemented: %s." name "(%s,%s)",\
  DeeType_NAME(Dee_TYPE(self)),\
  DeeType_NAME(Dee_TYPE(a)),\
  DeeType_NAME(Dee_TYPE(b))))
static DeeObject *_deeobject_tp_call(DeeObject *self, DeeObject *args) {
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
  "Not implemented: %s.__call__%K",
  DeeType_NAME(Dee_TYPE(self)),DeeTuple_Types(args));
 return NULL;
}
static DeeObject *_deeobject_tp_not(DeeObject *self) {
 int result; DeeTypeObject *tp = Dee_TYPE(self);
 DeeType_FIND_SLOT(tp,tp_bool);
 if DEE_LIKELY(tp != &DeeObject_Type) {
  if DEE_UNLIKELY((result = (*DeeType_GET_SLOT(tp,tp_bool))(self)) < 0) {
   if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return NULL;
  } else DeeReturn_Bool(!result);
 }
 NOTIMPLEMENTED_OPERATOR(self,"__not__");
 return NULL;
}
static int _deeobject_tp_bool(DeeObject *self) {
 DeeObject *result; DeeTypeObject *tp = Dee_TYPE(self);
 DeeType_FIND_SLOT(tp,tp_not);
 if DEE_LIKELY(tp != &DeeObject_Type) {
  if DEE_UNLIKELY((result = (*DeeType_GET_SLOT(tp,tp_not))(self)) == NULL) {
   if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return -1;
  } else {
   // v be careful not to cause an infinite recursion with the bool function
   //   >> If 'self is type(__not__(...))' is true, we don't call
   //      __bool__ recursively, because we'd just end up here again!
   if DEE_LIKELY(!DeeType_IsSameOrDerived(Dee_TYPE(self),Dee_TYPE(result))) {
    int temp = DeeObject_Bool(result);
    Dee_DECREF(result);
    if DEE_UNLIKELY(temp < 0) {
     // We set our own unimplemented error, to keep stuff easier to understand
     if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return -1;
    } else return !temp;
   } else {
    Dee_DECREF(result);
    // Fallthrough to unimplemented
   }
  }
 }
 NOTIMPLEMENTED_OPERATOR(self,"__bool__");
 return -1;
}
static DeeObject *_deeobject_tp_cmp_lo(DeeObject *lhs, DeeObject *rhs) { DeeReturn_Bool(lhs < rhs); }
static DeeObject *_deeobject_tp_cmp_le(DeeObject *lhs, DeeObject *rhs) { DeeReturn_Bool(lhs <= rhs); }
static DeeObject *_deeobject_tp_cmp_gr(DeeObject *lhs, DeeObject *rhs) { DeeReturn_Bool(lhs > rhs); }
static DeeObject *_deeobject_tp_cmp_ge(DeeObject *lhs, DeeObject *rhs) { DeeReturn_Bool(lhs >= rhs); }
static DeeObject *_deeobject_tp_cmp_eq(DeeObject *lhs, DeeObject *rhs) { DeeReturn_Bool(lhs == rhs); }
static DeeObject *_deeobject_tp_cmp_ne(DeeObject *lhs, DeeObject *rhs) { DeeReturn_Bool(lhs != rhs); }

static int _deeobject_tp_ctor(DeeTypeObject *DEE_UNUSED(tp), DeeObject *DEE_UNUSED(self)) { return 0; }
static int _deeobject_tp_ctor_arg(DeeTypeObject *DEE_UNUSED(tp), DeeObject *DEE_UNUSED(self), DeeObject *DEE_UNUSED(right)) { return 0; }
static int _deeobject_tp_ctor_args(DeeTypeObject *tp, DeeObject *DEE_UNUSED(self), DeeObject *args) {
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s%K",
                     DeeType_NAME(tp),DeeTuple_Types(args));
 return -1;
}
static int _deeobject_tp_copy_assign(DeeObject *self, DeeObject *DEE_UNUSED(right)) { NOTIMPLEMENTED_OPERATOR(self,"__copyassign__"); return -1; }
static int _deeobject_tp_move_assign(DeeObject *self, DeeObject *DEE_UNUSED(right)) { NOTIMPLEMENTED_OPERATOR(self,"__moveassign__"); return -1; }
static int _deeobject_tp_any_assign(DeeObject *self, DeeObject *right) { NOTIMPLEMENTED_OPERATOR_2(self,right,"__assign__"); return -1; }
static int _deeobject_tp_int32(DeeObject *self, Dee_int32_t *DEE_UNUSED(result)) { NOTIMPLEMENTED_OPERATOR(self,"__int32__"); return -1; }
static int _deeobject_tp_int64(DeeObject *self, Dee_int64_t *DEE_UNUSED(result)) { NOTIMPLEMENTED_OPERATOR(self,"__int64__"); return -1; }
static int _deeobject_tp_double(DeeObject *self, double *DEE_UNUSED(result)) { NOTIMPLEMENTED_OPERATOR(self,"__double__"); return -1; }
static DeeObject *_deeobject_tp_inv(DeeObject *self) { NOTIMPLEMENTED_OPERATOR(self,"__inv__"); return NULL; }
static DeeObject *_deeobject_tp_pos(DeeObject *self) { NOTIMPLEMENTED_OPERATOR(self,"__pos__"); return NULL; }
static DeeObject *_deeobject_tp_neg(DeeObject *self) { NOTIMPLEMENTED_OPERATOR(self,"__neg__"); return NULL; }
static DeeObject *_deeobject_tp_inc(DeeObject *self) { NOTIMPLEMENTED_OPERATOR(self,"__inc__"); return NULL; }
static DeeObject *_deeobject_tp_incpost(DeeObject *self) { NOTIMPLEMENTED_OPERATOR(self,"__incpost__"); return NULL; }
static DeeObject *_deeobject_tp_dec(DeeObject *self) { NOTIMPLEMENTED_OPERATOR(self,"__dec__"); return NULL; }
static DeeObject *_deeobject_tp_decpost(DeeObject *self) { NOTIMPLEMENTED_OPERATOR(self,"__decpost__"); return NULL; }
static DeeObject *_deeobject_tp_add(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__add__"); return NULL; }
static DeeObject *_deeobject_tp_sub(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__sub__"); return NULL; }
static DeeObject *_deeobject_tp_mul(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__mul__"); return NULL; }
static DeeObject *_deeobject_tp_div(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__div__"); return NULL; }
static DeeObject *_deeobject_tp_mod(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__mod__"); return NULL; }
static DeeObject *_deeobject_tp_shl(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__shl__"); return NULL; }
static DeeObject *_deeobject_tp_shr(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__shr__"); return NULL; }
static DeeObject *_deeobject_tp_and(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__and__"); return NULL; }
static DeeObject *_deeobject_tp_or(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__or__"); return NULL; }
static DeeObject *_deeobject_tp_xor(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__xor__"); return NULL; }
static DeeObject *_deeobject_tp_pow(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__pow__"); return NULL; }
static DeeObject *_deeobject_tp_iadd(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__iadd__"); return NULL; }
static DeeObject *_deeobject_tp_isub(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__isub__"); return NULL; }
static DeeObject *_deeobject_tp_imul(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__imul__"); return NULL; }
static DeeObject *_deeobject_tp_idiv(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__idiv__"); return NULL; }
static DeeObject *_deeobject_tp_imod(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__imod__"); return NULL; }
static DeeObject *_deeobject_tp_ishl(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__ishl__"); return NULL; }
static DeeObject *_deeobject_tp_ishr(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__ishr__"); return NULL; }
static DeeObject *_deeobject_tp_iand(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__iand__"); return NULL; }
static DeeObject *_deeobject_tp_ior(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__ior__"); return NULL; }
static DeeObject *_deeobject_tp_ixor(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__ixor__"); return NULL; }
static DeeObject *_deeobject_tp_ipow(DeeObject *lhs, DeeObject *rhs) { NOTIMPLEMENTED_OPERATOR_2(lhs,rhs,"__ipow__"); return NULL; }
static int _deeobject_tp_hash(DeeObject *self, Dee_hash_t DEE_UNUSED(start), Dee_hash_t *DEE_UNUSED(result)) { NOTIMPLEMENTED_OPERATOR(self,"__hash__"); return -1; }
static DeeObject *_deeobject_tp_seq_get(DeeObject *self, DeeObject *i) { NOTIMPLEMENTED_OPERATOR_2(self,i,"__getitem__"); return NULL; }
static int _deeobject_tp_seq_del(DeeObject *self, DeeObject *i) { NOTIMPLEMENTED_OPERATOR_2(self,i,"__delitem__"); return -1; }
static int _deeobject_tp_seq_set(DeeObject *self, DeeObject *i, DeeObject *v) { NOTIMPLEMENTED_OPERATOR_3(self,i,v,"__setitem__"); return -1; }
static DeeObject *_deeobject_tp_seq_size(DeeObject *self) { NOTIMPLEMENTED_OPERATOR(self,"__size__"); return NULL; }
static DeeObject *_deeobject_tp_seq_contains(DeeObject *self, DeeObject *ob) { NOTIMPLEMENTED_OPERATOR_2(self,ob,"__contains__"); return NULL; }
static DeeObject *_deeobject_tp_seq_range_get(DeeObject *self, DeeObject *lo, DeeObject *hi) { NOTIMPLEMENTED_OPERATOR_3(self,lo,hi,"__getrange__"); return NULL; }
static int _deeobject_tp_seq_range_del(DeeObject *self, DeeObject *lo, DeeObject *hi) { NOTIMPLEMENTED_OPERATOR_3(self,lo,hi,"__delrange__"); return -1; }
static int _deeobject_tp_seq_range_set(DeeObject *self, DeeObject *lo, DeeObject *hi, DeeObject *v) {
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
  "Not implemented: %s.__setrange__(%s,%s,%s)",
  DeeType_NAME(Dee_TYPE(self)),
  DeeType_NAME(Dee_TYPE(lo)),
  DeeType_NAME(Dee_TYPE(hi)),
  DeeType_NAME(Dee_TYPE(v)));
 return -1;
}
static DeeObject *_deeobject_tp_seq_iter_self(DeeObject *self) {
 NOTIMPLEMENTED_OPERATOR(self,"__iterself__");
 return NULL;
}
static int _deeobject_tp_seq_iter_next(
 DeeObject *self, DeeObject **DEE_UNUSED(result)) {
 NOTIMPLEMENTED_OPERATOR(self,"__iternext__");
 return -1;
}


#define DEE_PRIVATE_OBJECT_METHOD(name)\
static DeeObject *_deeobject_##name(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure))
DEE_PRIVATE_OBJECT_METHOD(__copy__) { return DeeTuple_Unpack(args,":__copy__") != 0 ? NULL : DeeObject_Copy(self); }
DEE_PRIVATE_OBJECT_METHOD(__move__) { return DeeTuple_Unpack(args,":__move__") != 0 ? NULL : DeeObject_Move(self); }
DEE_PRIVATE_OBJECT_METHOD(__deepcopy__) { return DeeTuple_Unpack(args,":__deepcopy__") != 0 ? NULL : DeeObject_DeepCopy(self); }
DEE_PRIVATE_OBJECT_METHOD(__deepequals__) {
 DeeObject *right; int temp;
 if (DeeTuple_Unpack(args,"o:__deepequals__",&right) != 0) return NULL;
 if ((temp = DeeObject_DeepEquals(self,right)) < 0) return NULL;
 return DeeObject_New(int,temp);
}
DEE_PRIVATE_OBJECT_METHOD(__assign__) {
 DeeObject *right;
 if (DeeTuple_Unpack(args,"o:__assign__",&right) != 0 ||
     DeeObject_Assign(self,right) != 0) return NULL;
 DeeReturn_NEWREF(self);
}
DEE_PRIVATE_OBJECT_METHOD(__bool__) {
 int temp;
 if (DeeTuple_Unpack(args,":__bool__") != 0 ||
    (temp = DeeObject_Bool(self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
#define MAKE_UNARY(name,cfunc)\
DEE_PRIVATE_OBJECT_METHOD(name) {\
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":" #name) != 0) return NULL;\
 return cfunc(self);\
}
#define MAKE_BINARY(name,cfunc)\
DEE_PRIVATE_OBJECT_METHOD(name) {\
 DeeObject *right;\
 if (DeeTuple_Unpack(args,"o:" #name,&right) != 0) return NULL;\
 return cfunc(self,right);\
}
#define MAKE_TRINARY(name,cfunc)\
DEE_PRIVATE_OBJECT_METHOD(name) {\
 DeeObject *a,*b;\
 if (DeeTuple_Unpack(args,"oo:" #name,&a,&b) != 0) return NULL;\
 return cfunc(self,a,b);\
}
MAKE_UNARY(__str__,DeeObject_Str)
MAKE_UNARY(__repr__,DeeObject_Repr)
MAKE_UNARY(__not__,DeeObject_Not)
MAKE_UNARY(__neg__,DeeObject_Neg)
MAKE_UNARY(__pos__,DeeObject_Pos)
MAKE_UNARY(__inv__,DeeObject_Inv)
MAKE_UNARY(__inc__,DeeObject_Inc)
MAKE_UNARY(__dec__,DeeObject_Dec)
MAKE_UNARY(__incpost__,DeeObject_IncPost)
MAKE_UNARY(__decpost__,DeeObject_DecPost)
#define _deeobject___lt__ _deeobject___lo__
#define _deeobject___gt__ _deeobject___gr__
MAKE_BINARY(__lo__,DeeObject_CompareLoObject)
MAKE_BINARY(__le__,DeeObject_CompareLeObject)
MAKE_BINARY(__eq__,DeeObject_CompareEqObject)
MAKE_BINARY(__ne__,DeeObject_CompareNeObject)
MAKE_BINARY(__gr__,DeeObject_CompareGrObject)
MAKE_BINARY(__ge__,DeeObject_CompareGeObject)
MAKE_BINARY(__add__,DeeObject_Add);
MAKE_BINARY(__iadd__,DeeObject_InplaceAdd);
MAKE_BINARY(__sub__,DeeObject_Sub);
MAKE_BINARY(__isub__,DeeObject_InplaceSub);
MAKE_BINARY(__mul__,DeeObject_Mul);
MAKE_BINARY(__imul__,DeeObject_InplaceMul);
MAKE_BINARY(__div__,DeeObject_Div);
MAKE_BINARY(__idiv__,DeeObject_InplaceDiv);
MAKE_BINARY(__mod__,DeeObject_Mod);
MAKE_BINARY(__imod__,DeeObject_InplaceMod);
MAKE_BINARY(__shl__,DeeObject_Shl);
MAKE_BINARY(__ishl__,DeeObject_InplaceShl);
MAKE_BINARY(__shr__,DeeObject_Shr);
MAKE_BINARY(__ishr__,DeeObject_InplaceShr);
MAKE_BINARY(__and__,DeeObject_And);
MAKE_BINARY(__iand__,DeeObject_InplaceAnd);
MAKE_BINARY(__or__,DeeObject_Or);
MAKE_BINARY(__ior__,DeeObject_InplaceOr);
MAKE_BINARY(__xor__,DeeObject_Xor);
MAKE_BINARY(__ixor__,DeeObject_InplaceXor);
MAKE_BINARY(__pow__,DeeObject_Pow);
MAKE_BINARY(__ipow__,DeeObject_InplacePow);
MAKE_BINARY(__call__,DeeObject_Call);
DEE_PRIVATE_OBJECT_METHOD(__thiscall__) {
 DeeObject *this_arg,*call_args;
 if (DeeTuple_Unpack(args,"oo:__thiscall__",&this_arg,&call_args) != 0) return NULL;
 return DeeObject_ThisCall(self,this_arg,call_args);
}
MAKE_UNARY(__size__,DeeObject_SizeObject);
MAKE_BINARY(__getitem__,DeeObject_GetItem);
DEE_PRIVATE_OBJECT_METHOD(__setitem__) {
 DeeObject *i,*v;
 if (DeeTuple_Unpack(args,"oo:__setitem__",&i,&v) != 0 ||
     DeeObject_SetItem(self,i,v) != 0) return NULL;
 DeeReturn_NEWREF(v);
}
DEE_PRIVATE_OBJECT_METHOD(__delitem__) {
 DeeObject *i;
 if (DeeTuple_Unpack(args,"o:__delitem__",&i) != 0 ||
     DeeObject_DelItem(self,i) != 0) return NULL;
 DeeReturn_None;
}
MAKE_TRINARY(__getrange__,DeeObject_GetRangeItem);
DEE_PRIVATE_OBJECT_METHOD(__setrange__) {
 DeeObject *lo,*hi,*v;
 if (DeeTuple_Unpack(args,"ooo:__setrange__",&lo,&hi,&v) != 0 ||
     DeeObject_SetRangeItem(self,lo,hi,v) != 0) return NULL;
 DeeReturn_NEWREF(v);
}
DEE_PRIVATE_OBJECT_METHOD(__delrange__) {
 DeeObject *lo,*hi;
 if (DeeTuple_Unpack(args,"oo:__delrange__",&lo,&hi) != 0 ||
     DeeObject_DelRangeItem(self,lo,hi) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_OBJECT_METHOD(__is__) {
 DeeObject *ty;
 if (DeeTuple_Unpack(args,"o:__is__",&ty) != 0) return NULL;
 DeeReturn_Bool(DeeObject_Is(self,(DeeTypeObject *)ty));
}
MAKE_BINARY(__contains__,DeeObject_ContainsObject);
DEE_PRIVATE_OBJECT_METHOD(__getattr__) {
 DeeObject *attr;
 if (DeeTuple_Unpack(args,"o:__getattr__",&attr) != 0) return NULL;
 if (DeeError_TypeError_CheckTypeExact(attr,&DeeString_Type) != 0) return NULL;
 return DeeObject_GetAttr(self,attr);
}
DEE_PRIVATE_OBJECT_METHOD(__delattr__) {
 DeeObject *attr;
 if (DeeTuple_Unpack(args,"o:__delattr__",&attr) != 0) return NULL;
 if (DeeError_TypeError_CheckTypeExact(attr,&DeeString_Type) != 0) return NULL;
 if (DeeObject_DelAttr(self,attr) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_OBJECT_METHOD(__setattr__) {
 DeeObject *attr,*v;
 if (DeeTuple_Unpack(args,"oo:__setattr__",&attr,&v) != 0) return NULL;
 if (DeeError_TypeError_CheckTypeExact(attr,&DeeString_Type) != 0) return NULL;
 if (DeeObject_SetAttr(self,attr,v) != 0) return NULL;
 DeeReturn_NEWREF(v);
}
MAKE_UNARY(__iterself__,DeeObject_IterSelf);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
MAKE_UNARY(__ref__,DeeObject_Ref);
MAKE_UNARY(__deref__,DeeObject_Deref);
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
DEE_PRIVATE_OBJECT_METHOD(__iternext__) {
 int error; DeeObject *result;
 if (DeeTuple_Unpack(args,":__iternext__") != 0) return NULL;
 if ((error = DeeObject_IterNextEx(self,&result)) != 0) {
  if (error == 1) DeeError_Throw(DeeErrorInstance_IterDone);
  return NULL;
 }
 return result;
}
DEE_PRIVATE_OBJECT_METHOD(__hash__) {
 Dee_hash_t result,hash = DEE_HASH_START;
 if (DeeTuple_Unpack(args,"|" DEE_TUPLE_UNPACKFMT_UINT(DEE_CONFIG_SIZEOF_DEE_HASH_T) ":__hash__",&hash) != 0) return NULL;
 if (DeeObject_HashEx(self,hash,&result) != 0) return NULL;
 return DeeObject_New(Dee_hash_t,result);
}
#undef MAKE_TRINARY
#undef MAKE_BINARY
#undef MAKE_UNARY
#undef DEE_PRIVATE_OBJECT_METHOD


static DeeObject *_deeobject_class(DeeObject *self, void *DEE_UNUSED(closure)) {
 DeeReturn_NEWREF((DeeObject *)DeeObject_ClassOf(self));
}
static DeeObject *_deeobject_super(DeeObject *self, void *DEE_UNUSED(closure)) {
#if 1
 // Since there should be no way of 'self' being a 'super' object,
 // we can optimize this by bypassing the special handling for super-objects
 return DeeSuper_New(DeeType_BASE(Dee_TYPE(self)),self);
#else
 return DeeSuper_Of(self);
#endif
}
static DeeObject *_deeobject_this(DeeObject *self, void *DEE_UNUSED(closure)) {
 DeeReturn_NEWREF(self);
}

struct DeeMemberDef const _deeobject_tp_members[] = {
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__type__",DeeObject,__ob_type,object,"-> type object\n@return: The technical type of @this"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__refcnt__",DeeObject,__ob_refcnt,atomic(uint),"@return: The amount of references to @this object"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__weakcnt__",DeeObject,__ob_weakcnt,atomic(uint),"@return: The amount of weak references to @this object"),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__ob_refcnt",DeeObject,__ob_refcnt,atomic(uint)),
 DEE_MEMBERDEF_NAMED_RO_v100("__ob_weakcnt",DeeObject,__ob_weakcnt,atomic(uint)),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};
struct DeeGetSetDef const _deeobject_tp_getsets[] = {
 // Note, that these are never called by the runtime as
 // these attributes are optimized away during parsing.
 // >> These are only here so you don't scratch your head
 //    when calling something like 'DeeObject_GetAttrString(my_ob,"super")',
 //    which would otherwise give you an Error.AttributeError
 DEE_GETSETDEF_CONST_v100("this",member(&_deeobject_this),null,null,
  "-> object\n@return: @this\n"
  "\tThis member should cannot be overwritten, as it is handled by the compiler."),
 DEE_GETSETDEF_CONST_v100("super",member(&_deeobject_super),null,null,"-> super\n"
  "@return: A proxy describing the immediate super-instance of @this\n"
  "\tThis member should cannot be overwritten, as it is handled by the compiler."),
 DEE_GETSETDEF_CONST_v100("class",member(&_deeobject_class),null,null,"-> type object\n"
  "@return: The logical class of @this\n"
  "\tThis member should cannot be overwritten, as it is handled by the compiler."),
 DEE_GETSETDEF_END_v100
};
struct DeeMethodDef const _deeobject_tp_methods[] = {
 DEE_METHODDEF_CONST_v100("__deepcopy__",member(&_deeobject___deepcopy__),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__deepequals__",member(&_deeobject___deepequals__),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__copy__",    member(&_deeobject___copy__),    DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__move__",    member(&_deeobject___move__),    DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__assign__",  member(&_deeobject___assign__),  DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__bool__",    member(&_deeobject___bool__),    DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__str__",     member(&_deeobject___str__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__repr__",    member(&_deeobject___repr__),    DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__not__",     member(&_deeobject___not__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__neg__",     member(&_deeobject___neg__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__pos__",     member(&_deeobject___pos__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__inv__",     member(&_deeobject___inv__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__inc__",     member(&_deeobject___inc__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__dec__",     member(&_deeobject___dec__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__incpost__", member(&_deeobject___incpost__), DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__decpost__", member(&_deeobject___decpost__), DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__lo__",      member(&_deeobject___lo__),      DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__lt__",      member(&_deeobject___lt__),      DEE_DOC_AUTO_OF("__lo__")),
 DEE_METHODDEF_CONST_v100("__le__",      member(&_deeobject___le__),      DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__eq__",      member(&_deeobject___eq__),      DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__ne__",      member(&_deeobject___ne__),      DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__gr__",      member(&_deeobject___gr__),      DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__gt__",      member(&_deeobject___gt__),      DEE_DOC_AUTO_OF("__gr__")),
 DEE_METHODDEF_CONST_v100("__ge__",      member(&_deeobject___ge__),      DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__add__",     member(&_deeobject___add__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__iadd__",    member(&_deeobject___iadd__),    DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__sub__",     member(&_deeobject___sub__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__isub__",    member(&_deeobject___isub__),    DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__mul__",     member(&_deeobject___mul__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__imul__",    member(&_deeobject___imul__),    DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__div__",     member(&_deeobject___div__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__idiv__",    member(&_deeobject___idiv__),    DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__mod__",     member(&_deeobject___mod__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__imod__",    member(&_deeobject___imod__),    DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__shl__",     member(&_deeobject___shl__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__ishl__",    member(&_deeobject___ishl__),    DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__shr__",     member(&_deeobject___shr__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__ishr__",    member(&_deeobject___ishr__),    DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__and__",     member(&_deeobject___and__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__iand__",    member(&_deeobject___iand__),    DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__or__",      member(&_deeobject___or__),      DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__ior__",     member(&_deeobject___ior__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__xor__",     member(&_deeobject___xor__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__ixor__",    member(&_deeobject___ixor__),    DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__pow__",     member(&_deeobject___pow__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__ipow__",    member(&_deeobject___ipow__),    DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__call__",    member(&_deeobject___call__),    DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__thiscall__",member(&_deeobject___thiscall__),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__size__",    member(&_deeobject___size__),    DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__getitem__", member(&_deeobject___getitem__), DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__setitem__", member(&_deeobject___setitem__), DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__delitem__", member(&_deeobject___delitem__), DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__getrange__",member(&_deeobject___getrange__),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__setrange__",member(&_deeobject___setrange__),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__delrange__",member(&_deeobject___delrange__),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__is__",      member(&_deeobject___is__),      DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__contains__",member(&_deeobject___contains__),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__getattr__", member(&_deeobject___getattr__), DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__delattr__", member(&_deeobject___delattr__), DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__setattr__", member(&_deeobject___setattr__), DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__iterself__",member(&_deeobject___iterself__),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__iternext__",member(&_deeobject___iternext__),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__hash__",    member(&_deeobject___hash__),    DEE_DOC_AUTO),

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DEE_METHODDEF_CONST_v100("__ref__",     member(&_deeobject___ref__),     DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("__deref__",   member(&_deeobject___deref__),   DEE_DOC_AUTO),
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DEE_METHODDEF_END_v100
};


DEE_DECL_END

