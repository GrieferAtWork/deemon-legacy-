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
#ifndef GUARD_DEEMON_BOOL_INL
#define GUARD_DEEMON_BOOL_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include <deemon/bool.h>
#include <deemon/string.h>
#include <deemon/tuple.h>
#include <deemon/memberdef.h>
#include <deemon/integer.h>

DEE_DECL_BEGIN

DeeBoolObject _Dee_True = {DEE_OBJECT_HEAD_INIT(&DeeBool_Type),1};
DeeBoolObject _Dee_False = {DEE_OBJECT_HEAD_INIT(&DeeBool_Type),0};

static int _deebool_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeBoolObject *self, DeeBoolObject *right) {
 self->b_value = right->b_value;
 return 0;
}
static int _deebool_tp_copy_assign(
 DeeBoolObject *DEE_UNUSED(self),
 DeeBoolObject *DEE_UNUSED(right)) {
 DeeError_NotImplemented();
 return -1;
}

DEE_STATIC_INLINE(int) _deestring_to_bool(
 char const *str, Dee_rt_bool *result) {
 Dee_rt_bool temp;
 if (Dee_StrCaseCmp(str,"true") == 0 ||
     Dee_StrCaseCmp(str,"yes") == 0 ||
     Dee_StrCaseCmp(str,"on") == 0) {
  *result = 1;
  return 0;
 }
 if (Dee_StrCaseCmp(str,"false") == 0 ||
     Dee_StrCaseCmp(str,"no") == 0 ||
     Dee_StrCaseCmp(str,"off") == 0) {
  *result = 0;
  return 0;
 } 
#if DEE_CONFIG_RT_SIZEOF_BOOL == 1
 if (DeeString_ToUInt8(str,&temp) == -1) return -1;
#elif DEE_CONFIG_RT_SIZEOF_BOOL == 2
 if (DeeString_ToUInt16(str,&temp) == -1) return -1;
#elif DEE_CONFIG_RT_SIZEOF_BOOL == 4
 if (DeeString_ToUInt32(str,&temp) == -1) return -1;
#elif DEE_CONFIG_RT_SIZEOF_BOOL == 8
 if (DeeString_ToUInt64(str,&temp) == -1) return -1;
#else
#error "Unsupported DEE_CONFIG_RT_SIZEOF_BOOL"
#endif
 *result = (Dee_rt_bool)!!temp;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeBool_FromUserObject(DEE_A_INOUT DeeObject *ob) {
 if (DeeString_Check(ob)) {
  Dee_rt_bool result;
  if (_deestring_to_bool(DeeString_STR(ob),&result) == -1) return -1;
  return (int)result;
 }
 return DeeObject_Bool(ob);
}
DEE_A_RET_WUNUSED int DeeBool_Value(
 DEE_A_IN_OBJECT(DeeBoolObject) const *ob) {
 DEE_ASSERT(DeeObject_Check(ob) && DeeBool_Check(ob));
 return DeeBool_VALUE(ob);
}

static int _deebool_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeBoolObject *self, DeeObject *args) {
 DeeObject *ob; int result;
 if (DeeTuple_Unpack(args,"o:bool",&ob) == -1) return -1;
 if (DeeString_Check(ob)) return _deestring_to_bool(DeeString_STR(ob),&self->b_value);
 if ((result = DeeObject_Bool(ob)) == -1) return -1;
 self->b_value = (Dee_rt_bool)result;
 return 0;
}
static int _deebool_tp_any_assign(DeeBoolObject *self, DeeObject *right) {
 int result;
 if (DeeString_Check(right)) return _deestring_to_bool(DeeString_STR(right),&self->b_value);
 if ((result = DeeObject_Bool(right)) == -1) return -1;
 self->b_value = (Dee_rt_bool)result;
 return 0;
}
DeeString_NEW_STATIC(_deebool_str_true,"true");
DeeString_NEW_STATIC(_deebool_str_false,"false");
static DeeObject *_deebool_tp_str(DeeBoolObject *self) {
 DeeReturn_NEWREF(self->b_value
  ?(DeeObject *)&_deebool_str_true
  :(DeeObject *)&_deebool_str_false);
}
static int _deebool_tp_bool(DeeBoolObject *self) { return (int)self->b_value; }
static int _deebool_tp_int32(DeeBoolObject *self, Dee_int32_t *result) { *result = (Dee_int32_t)self->b_value; return 0; }
static int _deebool_tp_int64(DeeBoolObject *self, Dee_int64_t *result) { *result = (Dee_int64_t)self->b_value; return 0; }
static int _deebool_tp_double(DeeBoolObject *self, double *result) { *result = (double)self->b_value; return 0; }
static DeeObject *_deebool_tp_not(DeeBoolObject *self) { DeeReturn_Bool(!self->b_value); }
static DeeObject *_deebool_tp_and(DeeBoolObject *lhs, DeeObject *rhs) {
 int rhs_b = DeeObject_Bool(rhs);
 if (rhs_b == -1) return NULL;
 DeeReturn_Bool(lhs->b_value && rhs_b); 
}
static DeeObject *_deebool_tp_or(DeeBoolObject *lhs, DeeObject *rhs) {
 int rhs_b = DeeObject_Bool(rhs);
 if (rhs_b == -1) return NULL;
 DeeReturn_Bool(lhs->b_value || rhs_b); 
}
static DeeObject *_deebool_tp_xor(DeeBoolObject *lhs, DeeObject *rhs) {
 int rhs_b = DeeObject_Bool(rhs);
 if (rhs_b == -1) return NULL;
 DeeReturn_Bool(lhs->b_value ^ rhs_b); 
}
#if 0
static DeeObject *_deebool_tp_iand(DeeBoolObject *lhs, DeeObject *rhs) {
 int rhs_b = DeeObject_Bool(rhs);
 if (rhs_b == -1) return NULL;
 if (!rhs_b) lhs->b_value = 0;
 DeeReturn_NEWREF(lhs);
}
static DeeObject *_deebool_tp_ior(DeeBoolObject *lhs, DeeObject *rhs) {
 int rhs_b = DeeObject_Bool(rhs);
 if (rhs_b == -1) return NULL;
 if (rhs_b) lhs->b_value = 1;
 DeeReturn_NEWREF(lhs);
}
static DeeObject *_deebool_tp_ixor(DeeBoolObject *lhs, DeeObject *rhs) {
 int rhs_b = DeeObject_Bool(rhs);
 if (rhs_b == -1) return NULL;
 lhs->b_value ^= !!rhs_b; // Make sure rhs is either 0 or 1
 DeeReturn_NEWREF(lhs);
}
#endif
static int _deebool_tp_hash(DeeBoolObject *self, Dee_hash_t start, Dee_hash_t *result) {
 *result = start ^ (Dee_hash_t)self->b_value;
 return 0;
}
static DeeObject *_deebool_tp_cmp_lo(DeeBoolObject *lhs, DeeObject *rhs) { int rhs_b = DeeObject_Bool(rhs); if (rhs_b == -1) return NULL; DeeReturn_Bool(lhs->b_value <  (Dee_rt_bool)rhs_b); }
static DeeObject *_deebool_tp_cmp_le(DeeBoolObject *lhs, DeeObject *rhs) { int rhs_b = DeeObject_Bool(rhs); if (rhs_b == -1) return NULL; DeeReturn_Bool(lhs->b_value <= (Dee_rt_bool)rhs_b); }
static DeeObject *_deebool_tp_cmp_eq(DeeBoolObject *lhs, DeeObject *rhs) { int rhs_b = DeeObject_Bool(rhs); if (rhs_b == -1) return NULL; DeeReturn_Bool(lhs->b_value == (Dee_rt_bool)rhs_b); }
static DeeObject *_deebool_tp_cmp_ne(DeeBoolObject *lhs, DeeObject *rhs) { int rhs_b = DeeObject_Bool(rhs); if (rhs_b == -1) return NULL; DeeReturn_Bool(lhs->b_value != (Dee_rt_bool)rhs_b); }
static DeeObject *_deebool_tp_cmp_gr(DeeBoolObject *lhs, DeeObject *rhs) { int rhs_b = DeeObject_Bool(rhs); if (rhs_b == -1) return NULL; DeeReturn_Bool(lhs->b_value >  (Dee_rt_bool)rhs_b); }
static DeeObject *_deebool_tp_cmp_ge(DeeBoolObject *lhs, DeeObject *rhs) { int rhs_b = DeeObject_Bool(rhs); if (rhs_b == -1) return NULL; DeeReturn_Bool(lhs->b_value >= (Dee_rt_bool)rhs_b); }

#if defined(__INTELLISENSE__)
#define DEE_BOOL_INT_SLOT(name) DeeType_DEFAULT_SLOT(name)
#elif 1
#define DEE_BOOL_INT_SLOT(name) \
 ((DeeType_SLOT_TYPE(name))&DEE_PP_CAT_4(_deeuint,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_BOOL),_,name))
#else
#define DEE_BOOL_INT_SLOT(name) \
 ((DeeType_SLOT_TYPE(name))&DEE_PP_CAT_4(_deeint,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_BOOL),_,name))
#endif

static struct DeeMemberDef const _deebool_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("true",object,Dee_True),
 DEE_MEMBERDEF_CONST_v100("false",object,Dee_False),
 DEE_MEMBERDEF_END_v100
};


DeeTypeObject DeeBool_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("bool"),member(
  "bool(object ob) -> bool\n"
  "bool(string ob) -> bool\n"
  "@param ob: The object to be converted into a boolean\n"
  "\tCast any given object to bool.\n"
  "\tIf 'object' is a string, the string is parsed for text that would indicate #true and #false."),
  member(/*DEE_TYPE_FLAG_MUST_COPY|*/DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|
         DEE_TYPE_FLAG_CONST_CTOR|DEE_TYPE_FLAG_FUNDAMENTAL),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeBoolObject),null,
  member(&_DeeGeneric_Noop),
  member(&_deebool_tp_copy_ctor),null,
  member(&_deebool_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deebool_tp_copy_assign),null,
  member(&_deebool_tp_any_assign)),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deebool_tp_str),
  member(&_deebool_tp_str),
  member(&_deebool_tp_int32),
  member(&_deebool_tp_int64),
  member(&_deebool_tp_double)),
 DEE_TYPE_OBJECT_OBJECT_v101(null,null,null),
#if DEE_CONFIG_RT_SIZEOF_INT > DEE_CONFIG_RT_SIZEOF_BOOL
#define SLOT_TP_POS  DEE_BOOL_INT_SLOT(tp_pos)
#else
#define SLOT_TP_POS  &_DeeGeneric_ReturnSelf
#endif
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deebool_tp_not),member(&_deebool_tp_bool),member(&_deebool_tp_not),
  member(SLOT_TP_POS),member(DEE_BOOL_INT_SLOT(tp_neg)),null,null,null,null,
  member(DEE_BOOL_INT_SLOT(tp_add)),null,
  member(DEE_BOOL_INT_SLOT(tp_sub)),null,
  member(DEE_BOOL_INT_SLOT(tp_mul)),null,
  member(DEE_BOOL_INT_SLOT(tp_div)),null,
  member(DEE_BOOL_INT_SLOT(tp_mod)),null,
  member(DEE_BOOL_INT_SLOT(tp_shl)),null,
  member(DEE_BOOL_INT_SLOT(tp_shr)),null,
  // We can't enable inplace versions, since true / false must be immutable
  member(&_deebool_tp_and),null,//member(&_deebool_tp_iand),
  member(&_deebool_tp_or),null,//member(&_deebool_tp_ior),
  member(&_deebool_tp_xor),null,//member(_deebool_tp_ixor),
  member(DEE_BOOL_INT_SLOT(tp_pow)),null,
  member(&_deebool_tp_hash)),
#undef SLOT_TP_POS
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deebool_tp_cmp_lo),member(&_deebool_tp_cmp_le),
  member(&_deebool_tp_cmp_eq),member(&_deebool_tp_cmp_ne),
  member(&_deebool_tp_cmp_gr),member(&_deebool_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,
  member(_deebool_tp_class_members),null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DEE_DECL_END

#endif /* !GUARD_DEEMON_BOOL_INL */
