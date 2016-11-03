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
#ifndef GUARD_DEEMON_NONE_C
#define GUARD_DEEMON_NONE_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/bool.h>
#include <deemon/error.h>
#include <deemon/integer.h>
#include <deemon/none.h>
#include <deemon/string.h>
#include <deemon/structured.h>

// /src/*
#include <deemon/optional/type_decl.h>
#include <deemon/marshal_data.h>

// */ (nano...)

DEE_DECL_BEGIN

DeeString_NEW_STATIC(dee_none_str,"none");
static int DEE_CALL _deenone_tp_int32(DeeObject *DEE_UNUSED(self), Dee_int32_t *result) { *result = (Dee_int32_t)0; return 0; }
static int DEE_CALL _deenone_tp_int64(DeeObject *DEE_UNUSED(self), Dee_int64_t *result) { *result = (Dee_int64_t)0; return 0; }
static int DEE_CALL _deenone_tp_double(DeeObject *DEE_UNUSED(self), double *result) { *result = (double)0.0; return 0; }
static DEE_A_REF DeeObject *DEE_CALL _deenone_tp_not(DeeObject *DEE_UNUSED(self)) { DeeReturn_True; }
static int DEE_CALL _deenone_tp_bool(DeeObject *DEE_UNUSED(self)) { return 0; }
static int DEE_CALL _deenone_tp_hash(DeeObject *DEE_UNUSED(self), Dee_hash_t start, Dee_hash_t *result) { *result = start; return 0; }
//static int DEE_CALL _deenone_unary(DeeObject *DEE_UNUSED(self)) { return 0; }
static int DEE_CALL _deenone_binary(DeeObject *DEE_UNUSED(self), DeeObject *DEE_UNUSED(a)) { return 0; }
static int DEE_CALL _deenone_trinary(DeeObject *DEE_UNUSED(self), DeeObject *DEE_UNUSED(a), DeeObject *DEE_UNUSED(b)) { return 0; }
static int DEE_CALL _deenone_quadary(DeeObject *DEE_UNUSED(self), DeeObject *DEE_UNUSED(a), DeeObject *DEE_UNUSED(b), DeeObject *DEE_UNUSED(c)) { return 0; }
static DeeObject *DEE_CALL _deenone_tp_seq_len(DeeObject *DEE_UNUSED(self)) { DeeReturn_False; }
static DeeObject *DEE_CALL _deenone_tp_seq_contains(DeeObject *DEE_UNUSED(self), DeeObject *DEE_UNUSED(ob)) { DeeReturn_False; }
static DeeObject *DEE_CALL _deenone_unary_ret(DeeObject *DEE_UNUSED(self)) { DeeReturn_None; }
static DeeObject *DEE_CALL _deenone_binary_ret(DeeObject *DEE_UNUSED(self), DeeObject *DEE_UNUSED(a)) { DeeReturn_None; }
static DeeObject *DEE_CALL _deenone_trinary_ret(DeeObject *DEE_UNUSED(self), DeeObject *DEE_UNUSED(a), DeeObject *DEE_UNUSED(b)) { DeeReturn_None; }
static DeeObject *DEE_CALL _deenone_tp_str(DeeObject *DEE_UNUSED(self)) { DeeReturn_NEWREF(&dee_none_str); }
static int DEE_CALL _deenone_tp_seq_iter_next(DeeObject *DEE_UNUSED(self), DeeObject **DEE_UNUSED(result)) { return 1; }
static DeeObject *DEE_CALL _deenone_tp_cmp_lo(DeeObject *DEE_UNUSED(lhs), DeeObject *rhs) { DeeReturn_Bool(!DeeNone_Check(rhs)); }
static DeeObject *DEE_CALL _deenone_tp_cmp_le(DeeObject *DEE_UNUSED(lhs), DeeObject *DEE_UNUSED(rhs)) { DeeReturn_True; }
static DeeObject *_deenone_tp_cmp_eq(DeeObject *DEE_UNUSED(lhs), DeeObject *rhs) { DeeReturn_Bool(DeeNone_Check(rhs)); }
static DeeObject *_deenone_tp_cmp_ne(DeeObject *DEE_UNUSED(lhs), DeeObject *rhs) { DeeReturn_Bool(!DeeNone_Check(rhs)); }
static DeeObject *_deenone_tp_cmp_gr(DeeObject *DEE_UNUSED(lhs), DeeObject *DEE_UNUSED(rhs)) { DeeReturn_False; }
static DeeObject *_deenone_tp_cmp_ge(DeeObject *DEE_UNUSED(lhs), DeeObject *rhs) { DeeReturn_Bool(DeeNone_Check(rhs)); }

// Custom alloc/free functions, that don't really do anything
static DeeObject *_deenone_tp_alloc(DeeTypeObject *DEE_UNUSED(lhs)) { DeeReturn_None; }
static void _deenone_tp_free(DeeTypeObject *DEE_UNUSED(lhs), DeeObject *DEE_UNUSED(ob)) { }

static int _deenone_tp_p_int32(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(self), Dee_int32_t *result) { *result = (Dee_int32_t)0; return 0; }
static int _deenone_tp_p_int64(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(self), Dee_int64_t *result) { *result = (Dee_int64_t)0; return 0; }
static int _deenone_tp_p_double(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(self), double *result) { *result = (double)0.0; return 0; }
static DEE_A_REF DeeObject *_deenone_tp_p_not(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(self)) { DeeReturn_True; }
static int _deenone_tp_p_bool(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(self)) { return 0; }
static int _deenone_tp_p_hash(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(self), Dee_hash_t start, Dee_hash_t *result) { *result = start; return 0; }
static int _deenone_p_unary(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(self)) { return 0; }
static int _deenone_p_binary(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(self), DeeObject *DEE_UNUSED(a)) { return 0; }
static int _deenone_p_trinary(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(self), DeeObject *DEE_UNUSED(a), DeeObject *DEE_UNUSED(b)) { return 0; }
static int _deenone_p_quadary(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(self), DeeObject *DEE_UNUSED(a), DeeObject *DEE_UNUSED(b), DeeObject *DEE_UNUSED(c)) { return 0; }
static DeeObject *_deenone_tp_p_seq_len(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(self)) { DeeReturn_False; }
static DeeObject *_deenone_tp_p_seq_contains(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(self), DeeObject *DEE_UNUSED(ob)) { DeeReturn_False; }
static DeeObject *_deenone_p_unary_ret(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(self)) { DeeReturn_None; }
static DeeObject *_deenone_p_binary_ret(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(self), DeeObject *DEE_UNUSED(a)) { DeeReturn_None; }
static DeeObject *_deenone_p_trinary_ret(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(self), DeeObject *DEE_UNUSED(a), DeeObject *DEE_UNUSED(b)) { DeeReturn_None; }
static DeeObject *_deenone_tp_p_str(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(self)) { DeeReturn_NEWREF(&dee_none_str); }
static DeeObject *_deenone_tp_p_cmp_lo(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(lhs), DeeObject *rhs) { DeeReturn_Bool(!DeeNone_Check(rhs)); }
static DeeObject *_deenone_tp_p_cmp_le(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(lhs), DeeObject *DEE_UNUSED(rhs)) { DeeReturn_True; }
static DeeObject *_deenone_tp_p_cmp_eq(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(lhs), DeeObject *rhs) { DeeReturn_Bool(DeeNone_Check(rhs)); }
static DeeObject *_deenone_tp_p_cmp_ne(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(lhs), DeeObject *rhs) { DeeReturn_Bool(!DeeNone_Check(rhs)); }
static DeeObject *_deenone_tp_p_cmp_gr(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(lhs), DeeObject *DEE_UNUSED(rhs)) { DeeReturn_False; }
static DeeObject *_deenone_tp_p_cmp_ge(DeeStructuredTypeObject *DEE_UNUSED(tp_self), void *DEE_UNUSED(lhs), DeeObject *rhs) { DeeReturn_Bool(DeeNone_Check(rhs)); }

static int DEE_CALL _deenone_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *DEE_UNUSED(self),
 DeeObject *DEE_UNUSED(infile), struct DeeMarshalReadMap *DEE_UNUSED(map)) {
 return 0;
}
static int DEE_CALL _deenone_tp_marshal_put(
 DEE_A_INOUT DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *DEE_UNUSED(self),
 DeeObject *DEE_UNUSED(outfile), struct DeeMarshalWriteMap *DEE_UNUSED(map)) {
 return 0;
}

static struct DeeTypeMarshal _deenone_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALID_NONE),
 member(&_deenone_tp_marshal_ctor),member(&_deenone_tp_marshal_put));


DeeStructuredObject _Dee_None = {
 DEE_STRUCTURED_OBJECT_HEAD_INIT(&DeeNone_Type)};
DeeStructuredTypeObject DeeNone_Type = {
 { // tp_type
  DEE_TYPE_OBJECT_HEAD_EX_v100(
   member((DeeTypeObject *)&DeeStructuredType_Type),member("none"),member(
   "none(...) -> none\n"
   "\tA placeholder object, implementing everything as a no-op.\n"
   "\tThe void-type, having a size of 0 and indicating no return value.\n"
   "\tThe generic NULL-pointer, being castable to any pointer and always yielding (T *)0.\n"
   "\tNone is a singleton.\n"),
   member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_CONST_CTOR|
          DEE_TYPE_FLAG_NO_SUBCLASS|DEE_TYPE_FLAG_FUNDAMENTAL),
   member((DeeTypeObject *)&DeeStructured_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeObject),
   member(&_deenone_tp_alloc),member(&_deenone_binary),
   member(&_deenone_trinary),member(&_deenone_trinary),
   member(&_deenone_trinary)),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(
   member(&_deenone_tp_free),null),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(&_deenone_binary),member(&_deenone_binary),
   member(&_deenone_binary)),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&_deenone_tp_str),member(&_deenone_tp_str),
   member(&_deenone_tp_int32),member(&_deenone_tp_int64),
   member(&_deenone_tp_double)),
  DEE_TYPE_OBJECT_OBJECT_v101(
   member(&_deenone_binary_ret),null,null),
  DEE_TYPE_OBJECT_MATH_v101(
   member(&_deenone_tp_not),member(&_deenone_tp_bool),member(&_deenone_unary_ret),
   member(&_deenone_unary_ret),member(&_deenone_unary_ret),
   member(&_deenone_unary_ret),member(&_deenone_unary_ret),
   member(&_deenone_unary_ret),member(&_deenone_unary_ret),
   member(&_deenone_binary_ret),member(&_deenone_binary_ret),
   member(&_deenone_binary_ret),member(&_deenone_binary_ret),
   member(&_deenone_binary_ret),member(&_deenone_binary_ret),
   member(&_deenone_binary_ret),member(&_deenone_binary_ret),
   member(&_deenone_binary_ret),member(&_deenone_binary_ret),
   member(&_deenone_binary_ret),member(&_deenone_binary_ret),
   member(&_deenone_binary_ret),member(&_deenone_binary_ret),
   member(&_deenone_binary_ret),member(&_deenone_binary_ret),
   member(&_deenone_binary_ret),member(&_deenone_binary_ret),
   member(&_deenone_binary_ret),member(&_deenone_binary_ret),
   member(&_deenone_binary_ret),member(&_deenone_binary_ret),member(&_deenone_tp_hash)),
  DEE_TYPE_OBJECT_COMPARE_v100(
   member(&_deenone_tp_cmp_lo),member(&_deenone_tp_cmp_le),
   member(&_deenone_tp_cmp_eq),member(&_deenone_tp_cmp_ne),
   member(&_deenone_tp_cmp_gr),member(&_deenone_tp_cmp_ge)),
  DEE_TYPE_OBJECT_SEQ_v101(
   member(&_deenone_binary_ret),member(&_deenone_binary),
   member(&_deenone_trinary),member(&_deenone_tp_seq_len),
   member(&_deenone_tp_seq_contains),member(&_deenone_trinary_ret),
   member(&_deenone_trinary),member(&_deenone_quadary),
   member(&_deenone_unary_ret),member(&_deenone_tp_seq_iter_next)),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(
   member(&_deenone_binary_ret),member(&_deenone_binary),
   member(&_deenone_trinary),null,null,null,null,null,null),
  DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deenone_tp_marshal)),
  DEE_TYPE_OBJECT_FOOTER_v101
 },
 DEE_STRUCTURED_TYPE_OBJECT_HEADER_v100
 DEE_STRUCTURED_TYPE_OBJECT_CONSTRUCTOR_v100(
  member(0),member(&_deenone_p_binary)),
 DEE_STRUCTURED_TYPE_OBJECT_ASSIGN_v100(
  member(&_deenone_p_binary)),
 DEE_STRUCTURED_TYPE_OBJECT_CAST_v100(
  member(&_deenone_tp_p_str),member(&_deenone_tp_p_str),
  member(&_deenone_tp_p_int32),member(&_deenone_tp_p_int64),
  member(&_deenone_tp_p_double)),
 DEE_STRUCTURED_TYPE_OBJECT_OBJECT_v100(
  member(&_deenone_p_binary_ret)),
 DEE_STRUCTURED_TYPE_OBJECT_MATH_v100(
  member(&_deenone_tp_p_not),member(&_deenone_tp_p_bool),member(&_deenone_p_unary_ret),
  member(&_deenone_p_unary_ret),member(&_deenone_p_unary_ret),
  member(&_deenone_p_unary),member(&_deenone_p_unary_ret),
  member(&_deenone_p_unary),member(&_deenone_p_unary_ret),
  member(&_deenone_p_binary_ret),member(&_deenone_p_binary),
  member(&_deenone_p_binary_ret),member(&_deenone_p_binary),
  member(&_deenone_p_binary_ret),member(&_deenone_p_binary),
  member(&_deenone_p_binary_ret),member(&_deenone_p_binary),
  member(&_deenone_p_binary_ret),member(&_deenone_p_binary),
  member(&_deenone_p_binary_ret),member(&_deenone_p_binary),
  member(&_deenone_p_binary_ret),member(&_deenone_p_binary),
  member(&_deenone_p_binary_ret),member(&_deenone_p_binary),
  member(&_deenone_p_binary_ret),member(&_deenone_p_binary),
  member(&_deenone_p_binary_ret),member(&_deenone_p_binary),
  member(&_deenone_p_binary_ret),member(&_deenone_p_binary),member(&_deenone_tp_p_hash)),
 DEE_STRUCTURED_TYPE_OBJECT_COMPARE_v100(
  member(&_deenone_tp_p_cmp_lo),member(&_deenone_tp_p_cmp_le),
  member(&_deenone_tp_p_cmp_eq),member(&_deenone_tp_p_cmp_ne),
  member(&_deenone_tp_p_cmp_gr),member(&_deenone_tp_p_cmp_ge)),
 DEE_STRUCTURED_TYPE_OBJECT_SEQ_v100(
  member(&_deenone_p_binary_ret),member(&_deenone_p_binary),
  member(&_deenone_p_trinary),member(&_deenone_tp_p_seq_len),
  member(&_deenone_tp_p_seq_contains),member(&_deenone_p_trinary_ret),
  member(&_deenone_p_trinary),member(&_deenone_p_quadary),
  member(&_deenone_p_unary_ret)),
 DEE_STRUCTURED_TYPE_OBJECT_ATTRIBUTE_v100(
  member(&_deenone_p_binary_ret),member(&_deenone_p_binary),
  member(&_deenone_p_trinary)),
 DEE_STRUCTURED_TYPE_OBJECT_FOOTER_v100
};

DEE_DECL_END

#endif /* !GUARD_DEEMON_NONE_C */
