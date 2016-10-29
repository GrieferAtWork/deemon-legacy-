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
#include "xast.c"
#endif

#include <deemon/time.h>
#include <deemon/traceback.h>
#include <deemon/memberdef.h>
#include <deemon/struct.h>
#include <deemon/__string_encoding.inl>

DEE_DECL_BEGIN

DEE_A_RET_WUNUSED int DeeType_SlotIdFromXAstKind(DEE_A_IN DeeXAstKind kind) {
 switch (kind) {
  case DEE_XASTKIND_STR: return DeeType_SLOT_ID(tp_str);
  case DEE_XASTKIND_REPR: return DeeType_SLOT_ID(tp_repr);
  case DEE_XASTKIND_COPY: return DeeType_SLOT_ID(tp_copy_ctor);
  case DEE_XASTKIND_MOVE: return DeeType_SLOT_ID(tp_move_ctor);
  case DEE_XASTKIND_BOOL: return DeeType_SLOT_ID(tp_bool);
  case DEE_XASTKIND_NOT: return DeeType_SLOT_ID(tp_not);
  case DEE_XASTKIND_NEG: return DeeType_SLOT_ID(tp_neg);
  case DEE_XASTKIND_POS: return DeeType_SLOT_ID(tp_pos);
  case DEE_XASTKIND_INV: return DeeType_SLOT_ID(tp_inv);
  case DEE_XASTKIND_INC: return DeeType_SLOT_ID(tp_inc);
  case DEE_XASTKIND_DEC: return DeeType_SLOT_ID(tp_dec);
  case DEE_XASTKIND_INCPOST: return DeeType_SLOT_ID(tp_incpost);
  case DEE_XASTKIND_DECPOST: return DeeType_SLOT_ID(tp_decpost);
  case DEE_XASTKIND_SEQ_SIZE: return DeeType_SLOT_ID(tp_seq_size);
  case DEE_XASTKIND_ITERSELF: return DeeType_SLOT_ID(tp_seq_iter_self);
  case DEE_XASTKIND_ITERNEXT: return DeeType_SLOT_ID(tp_seq_iter_next);
  case DEE_XASTKIND_CALL: return DeeType_SLOT_ID(tp_call);
  case DEE_XASTKIND_COMPARE_LO: return DeeType_SLOT_ID(tp_cmp_lo);
  case DEE_XASTKIND_COMPARE_LE: return DeeType_SLOT_ID(tp_cmp_le);
  case DEE_XASTKIND_COMPARE_EQ: return DeeType_SLOT_ID(tp_cmp_eq);
  case DEE_XASTKIND_COMPARE_NE: return DeeType_SLOT_ID(tp_cmp_ne);
  case DEE_XASTKIND_COMPARE_GR: return DeeType_SLOT_ID(tp_cmp_gr);
  case DEE_XASTKIND_COMPARE_GE: return DeeType_SLOT_ID(tp_cmp_ge);
  case DEE_XASTKIND_ADD: return DeeType_SLOT_ID(tp_add);
  case DEE_XASTKIND_SUB: return DeeType_SLOT_ID(tp_sub);
  case DEE_XASTKIND_MUL: return DeeType_SLOT_ID(tp_mul);
  case DEE_XASTKIND_DIV: return DeeType_SLOT_ID(tp_div);
  case DEE_XASTKIND_MOD: return DeeType_SLOT_ID(tp_mod);
  case DEE_XASTKIND_SHL: return DeeType_SLOT_ID(tp_shl);
  case DEE_XASTKIND_SHR: return DeeType_SLOT_ID(tp_shr);
  case DEE_XASTKIND_AND: return DeeType_SLOT_ID(tp_and);
  case DEE_XASTKIND_OR: return DeeType_SLOT_ID(tp_or);
  case DEE_XASTKIND_XOR: return DeeType_SLOT_ID(tp_xor);
  case DEE_XASTKIND_POW: return DeeType_SLOT_ID(tp_pow);
  case DEE_XASTKIND_IADD: return DeeType_SLOT_ID(tp_iadd);
  case DEE_XASTKIND_ISUB: return DeeType_SLOT_ID(tp_isub);
  case DEE_XASTKIND_IMUL: return DeeType_SLOT_ID(tp_imul);
  case DEE_XASTKIND_IDIV: return DeeType_SLOT_ID(tp_idiv);
  case DEE_XASTKIND_IMOD: return DeeType_SLOT_ID(tp_imod);
  case DEE_XASTKIND_ISHL: return DeeType_SLOT_ID(tp_ishl);
  case DEE_XASTKIND_ISHR: return DeeType_SLOT_ID(tp_ishr);
  case DEE_XASTKIND_IAND: return DeeType_SLOT_ID(tp_iand);
  case DEE_XASTKIND_IOR: return DeeType_SLOT_ID(tp_ior);
  case DEE_XASTKIND_IXOR: return DeeType_SLOT_ID(tp_ixor);
  case DEE_XASTKIND_IPOW: return DeeType_SLOT_ID(tp_ipow);
  case DEE_XASTKIND_SEQ_GET: return DeeType_SLOT_ID(tp_seq_get);
  case DEE_XASTKIND_SEQ_DEL: return DeeType_SLOT_ID(tp_seq_del);
  case DEE_XASTKIND_ATTR_GET:
  case DEE_XASTKIND_ATTR_GET_C: return DeeType_SLOT_ID(tp_attr_get);
  case DEE_XASTKIND_ATTR_DEL:
  case DEE_XASTKIND_ATTR_DEL_C: return DeeType_SLOT_ID(tp_attr_del);
  case DEE_XASTKIND_IN: return DeeType_SLOT_ID(tp_seq_contains);
  case DEE_XASTKIND_MOVE_ASSIGN: return DeeType_SLOT_ID(tp_move_assign);
  case DEE_XASTKIND_COPY_ASSIGN: return DeeType_SLOT_ID(tp_copy_assign);
  case DEE_XASTKIND_SEQ_SET: return DeeType_SLOT_ID(tp_seq_set);
  case DEE_XASTKIND_ATTR_SET:
  case DEE_XASTKIND_ATTR_SET_C: return DeeType_SLOT_ID(tp_attr_set);
  default: break;
 }
 return 0;
}

#define T_IS_INT8(ty)            ((ty)==(DeeTypeObject*)&DeeInt8_Type||(ty)==(DeeTypeObject*)&DeeUInt8_Type||(ty)==(DeeTypeObject*)&DeeAtomicInt8_Type||(ty)==(DeeTypeObject*)&DeeAtomicUInt8_Type)
#define T_IS_INT16(ty)           ((ty)==(DeeTypeObject*)&DeeInt16_Type||(ty)==(DeeTypeObject*)&DeeUInt16_Type||(ty)==(DeeTypeObject*)&DeeAtomicInt16_Type||(ty)==(DeeTypeObject*)&DeeAtomicUInt16_Type)
#define T_IS_INT32(ty)           ((ty)==(DeeTypeObject*)&DeeInt32_Type||(ty)==(DeeTypeObject*)&DeeUInt32_Type||(ty)==(DeeTypeObject*)&DeeAtomicInt32_Type||(ty)==(DeeTypeObject*)&DeeAtomicUInt32_Type)
#define T_IS_INT64(ty)           ((ty)==(DeeTypeObject*)&DeeInt64_Type||(ty)==(DeeTypeObject*)&DeeUInt64_Type||(ty)==(DeeTypeObject*)&DeeAtomicInt64_Type||(ty)==(DeeTypeObject*)&DeeAtomicUInt64_Type)
#define T_IS_SIGNED_NOATOM(ty)   ((ty)==(DeeTypeObject*)&DeeInt8_Type||(ty)==(DeeTypeObject*)&DeeInt16_Type||(ty)==(DeeTypeObject*)&DeeInt32_Type||(ty)==(DeeTypeObject*)&DeeInt64_Type)
#define T_IS_SIGNED_ATOM(ty)     ((ty)==(DeeTypeObject*)&DeeAtomicInt8_Type||(ty)==(DeeTypeObject*)&DeeAtomicInt16_Type||(ty)==(DeeTypeObject*)&DeeAtomicInt32_Type||(ty)==(DeeTypeObject*)&DeeAtomicInt64_Type)
#define T_IS_UNSIGNED_NOATOM(ty) ((ty)==(DeeTypeObject*)&DeeUInt8_Type||(ty)==(DeeTypeObject*)&DeeUInt16_Type||(ty)==(DeeTypeObject*)&DeeUInt32_Type||(ty)==(DeeTypeObject*)&DeeUInt64_Type)
#define T_IS_UNSIGNED_ATOM(ty)   ((ty)==(DeeTypeObject*)&DeeAtomicUInt8_Type||(ty)==(DeeTypeObject*)&DeeAtomicUInt16_Type||(ty)==(DeeTypeObject*)&DeeAtomicUInt32_Type||(ty)==(DeeTypeObject*)&DeeAtomicUInt64_Type)
#define T_IS_SIGNED(ty)          (T_IS_SIGNED_NOATOM(ty)||T_IS_SIGNED_ATOM(ty))
#define T_IS_UNSIGNED(ty)        (T_IS_UNSIGNED_NOATOM(ty)||T_IS_UNSIGNED_ATOM(ty))
#define T_IS_INTEGER_NOATOM(ty)  (T_IS_SIGNED_NOATOM(ty)||T_IS_UNSIGNED_NOATOM(ty))
#define T_IS_INTEGER_ATOM(ty)    (T_IS_SIGNED_ATOM(ty)||T_IS_UNSIGNED_ATOM(ty))
#define T_IS_INTEGER(ty)         (T_IS_SIGNED(ty)||T_IS_UNSIGNED(ty))
#define T_IS_FLOAT(ty)           ((ty)==(DeeTypeObject*)&DeeFloat_Type||(ty)==(DeeTypeObject*)&DeeDouble_Type||(ty)==(DeeTypeObject*)&DeeLDouble_Type)
#define T_IS_BOOL(ty)            ((ty)==(DeeTypeObject*)&DeeBool_Type)
#define T_IS_INTEGRAL(ty)        (T_IS_INTEGER(ty)||T_IS_FLOAT(ty)||T_IS_BOOL(ty))

#define SET_RESULT_TYPE(ty) do{if(result_type)*result_type=(ty);}while(0)



#define T_TYPES_SIGNED_NOATOM   (DeeTypeObject*)&DeeInt8_Type,(DeeTypeObject*)&DeeInt16_Type,(DeeTypeObject*)&DeeInt32_Type,(DeeTypeObject*)&DeeInt64_Type
#define T_TYPES_SIGNED_ATOM     (DeeTypeObject*)&DeeAtomicInt8_Type,(DeeTypeObject*)&DeeAtomicInt16_Type,(DeeTypeObject*)&DeeAtomicInt32_Type,(DeeTypeObject*)&DeeAtomicInt64_Type
#define T_TYPES_UNSIGNED_NOATOM (DeeTypeObject*)&DeeUInt8_Type,(DeeTypeObject*)&DeeUInt16_Type,(DeeTypeObject*)&DeeUInt32_Type,(DeeTypeObject*)&DeeUInt64_Type
#define T_TYPES_UNSIGNED_ATOM   (DeeTypeObject*)&DeeAtomicUInt8_Type,(DeeTypeObject*)&DeeAtomicUInt16_Type,(DeeTypeObject*)&DeeAtomicUInt32_Type,(DeeTypeObject*)&DeeAtomicUInt64_Type
#define T_TYPES_SIGNED          T_TYPES_SIGNED_NOATOM,T_TYPES_SIGNED_ATOM
#define T_TYPES_UNSIGNED        T_TYPES_UNSIGNED_NOATOM,T_TYPES_UNSIGNED_ATOM
#define T_TYPES_INTEGER_NOATOM  T_TYPES_SIGNED_NOATOM,T_TYPES_UNSIGNED_NOATOM
#define T_TYPES_INTEGER_ATOM    T_TYPES_SIGNED_ATOM,T_TYPES_UNSIGNED_ATOM
#define T_TYPES_INTEGER         T_TYPES_SIGNED,T_TYPES_UNSIGNED
#define T_TYPES_FLOAT           (DeeTypeObject*)&DeeFloat_Type,(DeeTypeObject*)&DeeDouble_Type,(DeeTypeObject*)&DeeLDouble_Type
#define T_TYPES_BOOL            (DeeTypeObject*)&DeeBool_Type
#define T_TYPES_INTEGRAL        T_TYPES_INTEGER,T_TYPES_FLOAT,T_TYPES_BOOL
#define T_TYPES_STRING          &DeeWideString_Type,&DeeUtf8String_Type,&DeeUtf16String_Type,&DeeUtf32String_Type
#define T_TYPES_CONTAINER       &DeeList_Type,&DeeTuple_Type,&DeeDict_Type,&DeeCell_Type,&DeeSet_Type,&DeeDictKeys_Type,&DeeDictItems_Type,&DeeDictValues_Type

DEE_STATIC_INLINE(int) _DeeType_InList(
 DeeTypeObject const *tp, DeeTypeObject const *const *v, Dee_size_t n) {
 while (n--) if (v[n] == tp) return 1;
 return 0;
}

#define T_SUPPORTS(tp,feature) _DeeType_InList(tp,feature,sizeof(feature)/sizeof(DeeTypeObject const *))
static DeeTypeObject const *const t_noeffect_copy[] = { T_TYPES_INTEGRAL,T_TYPES_STRING,&DeeTime_Type };
static DeeTypeObject const *const t_noeffect_str[] = { T_TYPES_INTEGRAL,&DeeString_Type };
static DeeTypeObject const *const t_noeffect_bool[] = { T_TYPES_INTEGRAL,T_TYPES_STRING,T_TYPES_CONTAINER,&DeeTime_Type };
static DeeTypeObject const *const t_add_returns_same_type[] = { T_TYPES_INTEGRAL,T_TYPES_STRING,&DeeList_Type,&DeeTime_Type };
#define t_noeffect_copyassign  t_noexcept_inc
#define t_noeffect_int         t_integral
static DeeTypeObject const *const t_noexcept_inc[] = { T_TYPES_INTEGRAL,&DeeTime_Type };
static DeeTypeObject const *const t_noexcept_hash[] = { T_TYPES_INTEGRAL,T_TYPES_STRING };
static DeeTypeObject const *const t_integral[] = { T_TYPES_INTEGRAL };
static DeeTypeObject const *const t_string[] = { T_TYPES_STRING };
#define t_noeffect_size t_container
static DeeTypeObject const *const t_container[] = { T_TYPES_CONTAINER };
static DeeTypeObject const *const t_transform_returns_same_type[] = { &DeeTuple_Type,&DeeList_Type,&DeeDict_Type,&DeeCell_Type };
static DeeTypeObject const *const t_transform_returns_list[] = { &DeeSet_Type,&DeeYieldFunction_Type };
static DeeTypeObject const *const t_segments_returns_list[] = { T_TYPES_CONTAINER,&DeeFunction_Type };
static DeeTypeObject const *const t_noeffect_empty[] = { T_TYPES_STRING,T_TYPES_CONTAINER };


//////////////////////////////////////////////////////////////////////////
DeeAstAttribute DeeType_UnaryAstAttr( // Unary
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN int typeslot,
 DEE_A_IN Dee_uint32_t flags, DEE_A_OUT_OPT DeeTypeObject const **result_type) {
 DeeObject *(DEE_CALL *slot_callback)(DeeObject *);
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 switch (typeslot) {
  case DeeType_SLOT_ID(tp_copy_new):
  case DeeType_SLOT_ID(tp_move_new):
  case DeeType_SLOT_ID(tp_copy_ctor):
  case DeeType_SLOT_ID(tp_move_ctor):
   SET_RESULT_TYPE(self);
   return (DeeAstAttribute)(T_SUPPORTS(self,t_noeffect_copy)
                            ? DEE_AST_ATTRIBUTE_NOOP
                            : DEE_AST_ATTRIBUTE_NONE);
  {
   if (0) { case DeeType_SLOT_ID(tp_int32):  SET_RESULT_TYPE((DeeTypeObject *)&DeeInt32_Type); }
   if (0) { case DeeType_SLOT_ID(tp_int64):  SET_RESULT_TYPE((DeeTypeObject *)&DeeInt64_Type); }
   if (0) { case DeeType_SLOT_ID(tp_double): SET_RESULT_TYPE((DeeTypeObject *)&DeeDouble_Type); }
   return (DeeAstAttribute)(T_SUPPORTS(self,t_noeffect_int)
                            ? DEE_AST_ATTRIBUTE_NOOP
                            : DEE_AST_ATTRIBUTE_NONE);
  } break;
  case DeeType_SLOT_ID(tp_str):
  case DeeType_SLOT_ID(tp_repr):
   SET_RESULT_TYPE(&DeeString_Type);
   return (DeeAstAttribute)(T_SUPPORTS(self,t_noeffect_str)
                            ? DEE_AST_ATTRIBUTE_NOOP
                            : DEE_AST_ATTRIBUTE_NONE);
  case DeeType_SLOT_ID(tp_bool):
  case DeeType_SLOT_ID(tp_not):
   SET_RESULT_TYPE(&DeeBool_Type);
   return (DeeAstAttribute)(T_SUPPORTS(self,t_noeffect_bool)
                            ? DEE_AST_ATTRIBUTE_NOOP
                            : DEE_AST_ATTRIBUTE_NONE);
  case DeeType_SLOT_ID(tp_inc):
  case DeeType_SLOT_ID(tp_dec):
  case DeeType_SLOT_ID(tp_incpost):
  case DeeType_SLOT_ID(tp_decpost):
   if (T_SUPPORTS(self,t_noexcept_inc)) {
    SET_RESULT_TYPE(self);
    return DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOJUMP;
   }
   break;
  case DeeType_SLOT_ID(tp_inv):
   if (self == &DeeBool_Type) { SET_RESULT_TYPE(&DeeBool_Type); return DEE_AST_ATTRIBUTE_NOOP; }
  case DeeType_SLOT_ID(tp_pos):
  case DeeType_SLOT_ID(tp_neg):
   if (0
#if DEE_CONFIG_RT_SIZEOF_INT > 1
    || T_IS_INT8(self)
#endif
#if DEE_CONFIG_RT_SIZEOF_INT > 2
    || T_IS_INT16(self)
#endif
#if DEE_CONFIG_RT_SIZEOF_INT > 4
    || T_IS_INT32(self)
#endif
#if DEE_CONFIG_RT_SIZEOF_INT > 8
    || T_IS_INT64(self)
#endif
#if DEE_CONFIG_RT_SIZEOF_INT > DEE_CONFIG_RT_SIZEOF_BOOL
    || self == (DeeTypeObject *)&DeeBool_Type
#endif
    ) { SET_RESULT_TYPE(DeeObject_TYPE(Dee_rt_int)); return DEE_AST_ATTRIBUTE_NOOP; }
   if (T_IS_FLOAT(self)
#if DEE_CONFIG_RT_SIZEOF_INT <= 1
    || T_IS_INT8(self)
#endif
#if DEE_CONFIG_RT_SIZEOF_INT <= 2
    || T_IS_INT16(self)
#endif
#if DEE_CONFIG_RT_SIZEOF_INT <= 4
    || T_IS_INT32(self)
#endif
#if DEE_CONFIG_RT_SIZEOF_INT <= 8
    || T_IS_INT64(self)
#endif
#if DEE_CONFIG_RT_SIZEOF_INT <= DEE_CONFIG_RT_SIZEOF_BOOL
    || self == (DeeTypeObject *)&DeeBool_Type
#endif
    ) { SET_RESULT_TYPE(self); return DEE_AST_ATTRIBUTE_NOOP; }
   break;

  case DeeType_SLOT_ID(tp_seq_size):
   if (T_SUPPORTS(self,t_noeffect_size)) {
    SET_RESULT_TYPE(DeeObject_TYPE(Dee_size_t));
    return DEE_AST_ATTRIBUTE_NOOP;
   }
   break;

  case DeeType_SLOT_ID(tp_seq_iter_self):
   SET_RESULT_TYPE(_DeeType_IteratorType(self));
   return (DeeAstAttribute)((flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS)!=0
    ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT))
    : (DEE_AST_ATTRIBUTE_NOOP));

  case DeeType_SLOT_ID(tp_seq_iter_next):
   if (self == &DeeStringIterator_Type) {
    // This one is always noexcept, since the characters
    // for 1-byte strings are allocated statically.
    SET_RESULT_TYPE(&DeeString_Type);
    // NOTE: __iternext__ can still throw a Signal.IterDone
    return DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT);
   }
#if DEE_CONFIG_HAVE_ENCODING_WIDE && \
    DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING) != DEE_STRING_ENCODING(Wide)
   if (self == &DeeWideStringIterator_Type) {
    SET_RESULT_TYPE(&DeeWideString_Type);
    return DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT);
   }
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF8 && \
    DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING) != DEE_STRING_ENCODING(Utf8)
   if (self == &DeeUtf8StringIterator_Type) {
    SET_RESULT_TYPE(&DeeUtf8String_Type);
    return DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT);
   }
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16 && \
    DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING) != DEE_STRING_ENCODING(Utf16)
   if (self == &DeeUtf16StringIterator_Type) {
    SET_RESULT_TYPE(&DeeUtf16String_Type);
    return DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT);
   }
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32 && \
    DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING) != DEE_STRING_ENCODING(Utf32)
   if (self == &DeeUtf32StringIterator_Type) {
    SET_RESULT_TYPE(&DeeUtf32String_Type);
    return DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT);
   }
#endif
   break;

  case DeeType_SLOT_ID(tp_io_flush):
   SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type);
   return DEE_AST_ATTRIBUTE_NONE;

  case DeeType_SLOT_ID(tp_io_trunc):
   SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type);
   return DEE_AST_ATTRIBUTE_NONE;

  case DeeType_SLOT_ID(tp_io_close):
   SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type);
   return DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEFFECT);


  default: break;
 }

 if (self == (DeeTypeObject *)&DeeNone_Type) {
  SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type); // Always a no-op
  return DEE_AST_ATTRIBUTE_NOOP;
 }
 DEE_ASSERTF(DeeType_GetSlot(self,typeslot) != NULL,"Invalid typeslot");
DEE_COMPILER_PREFAST_WARNING_PUSH(6011)
 slot_callback = *(DeeObject *(DEE_CALL **)(DeeObject *))DeeType_GetSlot(self,typeslot);
DEE_COMPILER_PREFAST_WARNING_POP
 if (slot_callback == &_DeeGeneric_ReturnSelf) {
  SET_RESULT_TYPE(self); // no-op
  return DEE_AST_ATTRIBUTE_NOOP;
 } 
 if (slot_callback == &DeeObject_Copy) // copy-op
  return DeeType_UnaryAstAttr(self,DeeType_SLOT_ID(tp_copy_ctor),flags,result_type);
 if (slot_callback == &DeeObject_Move) // move-op
  return DeeType_UnaryAstAttr(self,DeeType_SLOT_ID(tp_move_ctor),flags,result_type);
 if (slot_callback == &_deegenericiterable_tp_seq_size) {
  SET_RESULT_TYPE(DeeObject_TYPE(Dee_size_t));
  return DEE_AST_ATTRIBUTE_NONE;
 }
 SET_RESULT_TYPE(NULL);
 return DEE_AST_ATTRIBUTE_NONE;
}







//////////////////////////////////////////////////////////////////////////
DeeAstAttribute DeeType_BinaryAstAttr( // Binary
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN int typeslot,
 DEE_A_IN Dee_uint32_t flags, DEE_A_IN_OPT DeeTypeObject const *tb,
 DEE_A_OUT_OPT DeeTypeObject const **result_type) {
 DeeAstAttribute result;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT(!tb || (DeeObject_Check(tb) && DeeType_Check(tb)));
 (void)flags;
 switch (typeslot) {

  case DeeType_SLOT_ID(tp_move_assign):
  case DeeType_SLOT_ID(tp_copy_assign):
   SET_RESULT_TYPE(self);
   return (DeeAstAttribute)((tb && T_SUPPORTS(tb,t_noeffect_copyassign))
                            ? DEE_AST_ATTRIBUTE_NOOP
                            : DEE_AST_ATTRIBUTE_NONE);

  if (0) {
  case DeeType_SLOT_ID(tp_and):
  case DeeType_SLOT_ID(tp_or):
  case DeeType_SLOT_ID(tp_xor):
   if (self == (DeeTypeObject *)&DeeBool_Type) {
    // and, or and xor behave different in bool
    SET_RESULT_TYPE((DeeTypeObject *)&DeeBool_Type);
    return (DeeAstAttribute)((tb && T_SUPPORTS(tb,t_noeffect_bool))
                             ? DEE_AST_ATTRIBUTE_NOOP
                             : DEE_AST_ATTRIBUTE_NONE);
   }
   DEE_ATTRIBUTE_FALLTHROUGH
  case DeeType_SLOT_ID(tp_add):
  case DeeType_SLOT_ID(tp_sub):
  case DeeType_SLOT_ID(tp_mul):
  case DeeType_SLOT_ID(tp_shr):
  case DeeType_SLOT_ID(tp_shl):
  case DeeType_SLOT_ID(tp_pow):
   result = (DeeAstAttribute)((tb && T_SUPPORTS(tb,t_noeffect_int))
                              ? DEE_AST_ATTRIBUTE_NOOP
                              : DEE_AST_ATTRIBUTE_NONE);
  }
  if (0) {
  case DeeType_SLOT_ID(tp_div):
  case DeeType_SLOT_ID(tp_mod):
   result = (DeeAstAttribute)((tb && T_SUPPORTS(tb,t_noeffect_int))
                              ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT))
                              : DEE_AST_ATTRIBUTE_NONE);
  }
   if (0
#if DEE_CONFIG_RT_SIZEOF_INT > 1
    || T_IS_INT8(self)
#endif
#if DEE_CONFIG_RT_SIZEOF_INT > 2
    || T_IS_INT16(self)
#endif
#if DEE_CONFIG_RT_SIZEOF_INT > 4
    || T_IS_INT32(self)
#endif
#if DEE_CONFIG_RT_SIZEOF_INT > 8
    || T_IS_INT64(self)
#endif
#if DEE_CONFIG_RT_SIZEOF_INT > DEE_CONFIG_RT_SIZEOF_BOOL
    || self == (DeeTypeObject *)&DeeBool_Type
#endif
    ) { SET_RESULT_TYPE(DeeObject_TYPE(Dee_rt_int)); return result; }
   if (T_IS_FLOAT(self)
#if DEE_CONFIG_RT_SIZEOF_INT <= 1
    || T_IS_INT8(self)
#endif
#if DEE_CONFIG_RT_SIZEOF_INT <= 2
    || T_IS_INT16(self)
#endif
#if DEE_CONFIG_RT_SIZEOF_INT <= 4
    || T_IS_INT32(self)
#endif
#if DEE_CONFIG_RT_SIZEOF_INT <= 8
    || T_IS_INT64(self)
#endif
#if DEE_CONFIG_RT_SIZEOF_INT <= DEE_CONFIG_RT_SIZEOF_BOOL
    || self == (DeeTypeObject *)&DeeBool_Type
#endif
    ) { SET_RESULT_TYPE(self); return result; }
   break;

  case DeeType_SLOT_ID(tp_iadd):
  case DeeType_SLOT_ID(tp_isub):
  case DeeType_SLOT_ID(tp_imul):
  case DeeType_SLOT_ID(tp_ishl):
  case DeeType_SLOT_ID(tp_ishr):
  case DeeType_SLOT_ID(tp_iand):
  case DeeType_SLOT_ID(tp_ior):
  case DeeType_SLOT_ID(tp_ixor):
  case DeeType_SLOT_ID(tp_ipow):
   if (T_SUPPORTS(self,t_integral)) {
    SET_RESULT_TYPE(self);
    return (DeeAstAttribute)((tb && T_SUPPORTS(tb,t_noeffect_int))
                             ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEFFECT))
                             : DEE_AST_ATTRIBUTE_NONE);
   }
   break;
  case DeeType_SLOT_ID(tp_idiv):
  case DeeType_SLOT_ID(tp_imod):
   if (T_SUPPORTS(self,t_integral)) {
    SET_RESULT_TYPE(self);
    return (DeeAstAttribute)((tb && T_SUPPORTS(tb,t_noeffect_int))
                             ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEFFECT|DEE_AST_ATTRIBUTE_NOEXCEPT))
                             : DEE_AST_ATTRIBUTE_NONE);
   }
   break;
  case DeeType_SLOT_ID(tp_cmp_lo):
  case DeeType_SLOT_ID(tp_cmp_le):
  case DeeType_SLOT_ID(tp_cmp_eq):
  case DeeType_SLOT_ID(tp_cmp_ne):
  case DeeType_SLOT_ID(tp_cmp_gr):
  case DeeType_SLOT_ID(tp_cmp_ge):
   if (T_SUPPORTS(self,t_integral)) {
    SET_RESULT_TYPE(&DeeBool_Type);
    return (DeeAstAttribute)((tb && T_SUPPORTS(tb,t_noeffect_int))
                             ? DEE_AST_ATTRIBUTE_NOOP
                             : DEE_AST_ATTRIBUTE_NONE);
   } else if (T_SUPPORTS(self,t_string)) {
    SET_RESULT_TYPE(&DeeBool_Type);
    return (DeeAstAttribute)((tb && T_SUPPORTS(tb,t_string))
                             ? DEE_AST_ATTRIBUTE_NOOP
                             : DEE_AST_ATTRIBUTE_NONE);
   } else if (T_SUPPORTS(self,t_container)) {
    SET_RESULT_TYPE(&DeeBool_Type);
    return DEE_AST_ATTRIBUTE_NONE;
   }
   break;

  case DeeType_SLOT_ID(tp_seq_contains):
   if (T_SUPPORTS(self,t_string)) {
    SET_RESULT_TYPE(&DeeBool_Type); // String contains
    return (DeeAstAttribute)((tb && T_SUPPORTS(tb,t_string))
                             ? DEE_AST_ATTRIBUTE_NOOP
                             : DEE_AST_ATTRIBUTE_NONE);
   } else if (T_SUPPORTS(self,t_container)) {
    SET_RESULT_TYPE(&DeeBool_Type); // Container contains
    return DEE_AST_ATTRIBUTE_NONE;
   }
   break;

  case DeeType_SLOT_ID(tp_seq_get):
   if (T_SUPPORTS(self,t_string)) {
    SET_RESULT_TYPE(self); // Character lookup
    return (DeeAstAttribute)((tb && T_SUPPORTS(tb,t_integral))
                             ? DEE_AST_ATTRIBUTE_NOOP
                             : DEE_AST_ATTRIBUTE_NONE);
   } else if (self == &DeeList_Type || self == &DeeTuple_Type ||
              self == &DeeCell_Type || self == &DeeDictKeys_Type ||
              self == &DeeDictItems_Type || self == &DeeDictValues_Type) {
    // Element lookup (we can't predict the type,
    // but we can same something about the attributes)
    SET_RESULT_TYPE(NULL);
    return (DeeAstAttribute)((tb && T_SUPPORTS(tb,t_integral))
                             ? DEE_AST_ATTRIBUTE_NOOP
                             : DEE_AST_ATTRIBUTE_NONE);
   } else if (self == &DeeList_Type || self == &DeeTuple_Type || self == &DeeCell_Type ||
              self == &DeeDictKeys_Type || self == &DeeDictItems_Type || self == &DeeDictValues_Type) {
    // Element lookup by index (we can't predict the type,
    // but we can same something about the attributes)
    SET_RESULT_TYPE(NULL);
    return (DeeAstAttribute)((tb && T_SUPPORTS(tb,t_integral))
                             ? DEE_AST_ATTRIBUTE_NOOP
                             : DEE_AST_ATTRIBUTE_NONE);
   }
   break;

  case DeeType_SLOT_ID(tp_io_read): // non-pointer-version
  case DeeType_SLOT_ID(tp_io_write): // non-pointer-version
   SET_RESULT_TYPE(DeeObject_TYPE(Dee_size_t));
   return DEE_AST_ATTRIBUTE_NONE;

  case DeeType_SLOT_ID(tp_seq_del):
   SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type);
   if (self == &DeeCell_Type) {
    // For cells, we can predict that __delitem__ is noexcept
    return (DeeAstAttribute)((tb && T_SUPPORTS(tb,t_integral))
                             ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEFFECT))
                             : DEE_AST_ATTRIBUTE_NONE);
   }
   return DEE_AST_ATTRIBUTE_NONE;

  default: break;
 }
 if (self == (DeeTypeObject *)&DeeNone_Type) {
  SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type); // no-op
  return DEE_AST_ATTRIBUTE_NOOP;
 }
 SET_RESULT_TYPE(NULL);
 return DEE_AST_ATTRIBUTE_NONE;
}










DeeAstAttribute DeeType_TrinaryAstAttr(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN int typeslot,
 DEE_A_IN Dee_uint32_t flags, DEE_A_IN_OPT DeeTypeObject const *tb,
 DEE_A_IN_OPT DeeTypeObject const *tc, DEE_A_OUT_OPT DeeTypeObject const **result_type) {
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT(!tb || (DeeObject_Check(tb) && DeeType_Check(tb)));
 DEE_ASSERT(!tc || (DeeObject_Check(tc) && DeeType_Check(tc)));
 (void)flags;
 switch (typeslot) {

#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case DeeType_SLOT_ID(tp_io_read): // pointer-version
  case DeeType_SLOT_ID(tp_io_write): // pointer-version
   SET_RESULT_TYPE(DeeObject_TYPE(Dee_size_t));
   return DEE_AST_ATTRIBUTE_NONE;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */

  case DeeType_SLOT_ID(tp_io_seek):
   SET_RESULT_TYPE((DeeTypeObject *)&DeeUInt64_Type);
   return DEE_AST_ATTRIBUTE_NONE;

  case DeeType_SLOT_ID(tp_seq_range_get):
   if (T_SUPPORTS(self,t_string) || self == &DeeList_Type ||
       self == &DeeTuple_Type || self == &DeeCell_Type) {
    SET_RESULT_TYPE(self); // Substring/Subrange lookup
    return (DeeAstAttribute)((tb && tc &&
     (tb == (DeeTypeObject *)&DeeNone_Type || T_SUPPORTS(tb,t_integral)) &&
     (tc == (DeeTypeObject *)&DeeNone_Type || T_SUPPORTS(tc,t_integral)))
     ? DEE_AST_ATTRIBUTE_NOOP : DEE_AST_ATTRIBUTE_NONE);
   } else if (self == &DeeDictKeys_Type || self == &DeeDictItems_Type || self == &DeeDictValues_Type) {
    SET_RESULT_TYPE(&DeeList_Type); // Subrange lookup in a dict
    return (DeeAstAttribute)((tb && tc &&
     (tb == (DeeTypeObject *)&DeeNone_Type || T_SUPPORTS(tb,t_integral)) &&
     (tc == (DeeTypeObject *)&DeeNone_Type || T_SUPPORTS(tc,t_integral)))
     ? DEE_AST_ATTRIBUTE_NOOP : DEE_AST_ATTRIBUTE_NONE);
   }
   if (DeeType_GET_SLOT(self,tp_seq_range_get) == &_deegenericiterable_tp_seq_range_get) {
    SET_RESULT_TYPE(&DeeList_Type);
    return DEE_AST_ATTRIBUTE_NONE;
   }
   break;

  case DeeType_SLOT_ID(tp_seq_range_del):
   SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type); // Subrange delete
   if (self == &DeeCell_Type) {
    // For cells, we can predict that __delrange__ is noexcept
    return (DeeAstAttribute)((tb && tc &&
     (tb == (DeeTypeObject *)&DeeNone_Type || T_SUPPORTS(tb,t_integral)) &&
     (tc == (DeeTypeObject *)&DeeNone_Type || T_SUPPORTS(tc,t_integral)))
     ? DEE_AST_ATTRIBUTE_NOOP : DEE_AST_ATTRIBUTE_NONE);
   }
   break;

  case DeeType_SLOT_ID(tp_seq_set):
   SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type);
   if (self == &DeeCell_Type) {
    // For cells, we can predict that __setitem__ is noexcept
    return (DeeAstAttribute)((tb && (tb == (DeeTypeObject *)&DeeNone_Type || T_SUPPORTS(tb,t_integral)))
                             ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEFFECT))
                             : DEE_AST_ATTRIBUTE_NONE);
   }
   return DEE_AST_ATTRIBUTE_NONE;

  default: break;
 }
 if (self == (DeeTypeObject *)&DeeNone_Type) {
  SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type); // no-op
  return DEE_AST_ATTRIBUTE_NOOP;
 }
 SET_RESULT_TYPE(NULL);
 return DEE_AST_ATTRIBUTE_NONE;
}


DEE_A_RET_WUNUSED DeeAstAttribute DeeType_SetRangeAstAttr(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN_OPT DeeTypeObject const *tbegin, DEE_A_IN_OPT DeeTypeObject const *tend,
 DEE_A_IN_OPT DeeTypeObject const *tvalue) {
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT(!tbegin || (DeeObject_Check(tbegin) && DeeType_Check(tbegin)));
 DEE_ASSERT(!tend || (DeeObject_Check(tend) && DeeType_Check(tend)));
 DEE_ASSERT(!tvalue || (DeeObject_Check(tvalue) && DeeType_Check(tvalue)));
 (void)flags,tvalue;
 if (self == (DeeTypeObject *)&DeeNone_Type) return DEE_AST_ATTRIBUTE_NOOP;
 if (self == &DeeCell_Type) {
  // For cells, we can predict that __delrange__ is noexcept
  return (DeeAstAttribute)((tbegin && tend &&
   (tbegin == (DeeTypeObject *)&DeeNone_Type || T_SUPPORTS(tbegin,t_integral)) &&
   (tend == (DeeTypeObject *)&DeeNone_Type || T_SUPPORTS(tend,t_integral)))
   ? DEE_AST_ATTRIBUTE_NOOP : DEE_AST_ATTRIBUTE_NONE);
 }
 return DEE_AST_ATTRIBUTE_NONE;
}


DeeAstAttribute DeeType_CallAstAttr(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN Dee_size_t targc, DEE_A_IN_R_OPT(targc) DeeTypeObject const *const *targv,
 DEE_A_OUT_OPT DeeTypeObject const **result_type) {
 if (self == (DeeTypeObject *)&DeeNone_Type) {
  SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type); // no-op
  return DEE_AST_ATTRIBUTE_NOOP;
 }
 (void)self,flags,targc,targv; // TODO
 // Fallback: Use the binary tp_call operator
 return DeeType_BinaryAstAttr(self,DeeType_SLOT_ID(tp_call),flags,&DeeTuple_Type,result_type);
}

DEE_STATIC_INLINE(DeeTypeObject *) _try_get_void_pointer_type(void) {
 DeeTypeObject *result;
 if ((result = DeeStructuredType_Pointer(DeeObject_TYPE(void))) == NULL)
  DeeError_Handled();
 return result;
}
DEE_STATIC_INLINE(DeeTypeObject *) _try_get_char_pointer_type(void) {
 DeeTypeObject *result;
 if ((result = DeeStructuredType_Pointer(DeeObject_TYPE(char))) == NULL)
  DeeError_Handled();
 return result;
}

DeeAstAttribute DeeObject_CallAstAttr(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeObject *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN Dee_size_t targc, DEE_A_IN_R_OPT(targc) DeeTypeObject const *const *targv,
 DEE_A_OUT_OPT DeeTypeObject const **result_type) {
 DeeAstAttribute result;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeCFunction_Type) { // C-function call
  // Builtin functions
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_dex) {
   SET_RESULT_TYPE(&DeeDexModule_Type);
   // NOTE: __builtin_dex is considered to be noeffect!
   return DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT);
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_object) {
   SET_RESULT_TYPE(NULL); /* This can literally be anything... */
   // NOTE: __builtin_object is considered to be noeffect!
   return DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT);
  }
#if DEE_CONFIG_RUNTIME_HAVE_EXTERN
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_extern) {
   SET_RESULT_TYPE((DeeTypeObject *)&DeeLValue_Type); /*< Extern objects are loaded as lvalues. */
   // NOTE: __builtin_extern is considered to be noeffect!
   return DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT);
  }
#endif /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_ff_closure) {
   // NOTE: __builtin_ff_closure is considered to be noeffect!
   SET_RESULT_TYPE((DeeTypeObject *)&DeeForeignFunctionClosure_Type);
   return DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT);
  }
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_assertion_failed) {
   SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type);
   // '__builtin_assertion_failed' does not return, but throws an error
   return DEE_AST_ATTRIBUTE_ALWAYSTHROW(flags);
  }
  // NOTE: The dual-operator predicates are not listed here, as they would
  //       have been optimized away earlier if their argument counter was known.
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_last_traceback) {
   SET_RESULT_TYPE(&DeeTraceback_Type);
   return DEE_AST_ATTRIBUTE_NONE;
  }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_sum) {
   // In __builtin_sum, we can deduce the resulting type if the first one is known
   if (targc == 0) { SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type); return DEE_AST_ATTRIBUTE_NOOP; }
   if (targc == 1) { SET_RESULT_TYPE(targv[0]); return DEE_AST_ATTRIBUTE_NOOP; }
   if (targv[0]) {
    DeeTypeObject const *ta = targv[0];
    SET_RESULT_TYPE(T_SUPPORTS(ta,t_add_returns_same_type) ? ta : NULL);
    return DEE_AST_ATTRIBUTE_NONE;
   }
   SET_RESULT_TYPE(NULL);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_abs) {
   // In __builtin_sum, we can deduce the resulting type if the first one is known
   if (targc == 1 && targv[0]) {
    DeeTypeObject const *compare_result;
    // If
    result = DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_neg),flags,result_type);
    if (result_type && *result_type != targv[0]) *result_type = NULL;
    // __builtin_abs will compare against an integer: 'ob < 0'
    result = DeeAstAttribute_MergeSequential(result,DeeType_BinaryAstAttr(
     targv[0],DeeType_SLOT_ID(tp_cmp_lo),flags,DeeObject_TYPE(Dee_rt_int),&compare_result),flags);
    // Make sure the result of 'ob < 0' can be interpreted as bool without causing an exception.
    // >> If we can't be sure, don't signal any special attributes (such as noeffect, or noexcept).
    if (!compare_result || !T_SUPPORTS(compare_result,t_noeffect_bool)) return DEE_AST_ATTRIBUTE_NONE;
    return result;
   }
   SET_RESULT_TYPE(NULL);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_ceil ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin_floor ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin_round) {
   if (targc == 1) {
    SET_RESULT_TYPE(DeeObject_TYPE(Dee_rt_int));
    return (DeeAstAttribute)(targv[0] ? DeeType_UnaryAstAttr(
     targv[0],DeeType_SLOT_ID(tp_double),flags,NULL) : DEE_AST_ATTRIBUTE_NONE);
   }
   SET_RESULT_TYPE(NULL);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_range) {
   if (targc == 1) {
    if (targv[0]) {
     // Special range types
          if (targv[0] == (DeeTypeObject *)&DeeInt8_Type) SET_RESULT_TYPE(&DeeRangeInt8Iterator_Type);
     else if (targv[0] == (DeeTypeObject *)&DeeInt16_Type) SET_RESULT_TYPE(&DeeRangeInt16Iterator_Type);
     else if (targv[0] == (DeeTypeObject *)&DeeInt32_Type) SET_RESULT_TYPE(&DeeRangeInt32Iterator_Type);
     else if (targv[0] == (DeeTypeObject *)&DeeInt64_Type) SET_RESULT_TYPE(&DeeRangeInt64Iterator_Type);
     else if (targv[0] == (DeeTypeObject *)&DeeUInt8_Type) SET_RESULT_TYPE(&DeeRangeUInt8Iterator_Type);
     else if (targv[0] == (DeeTypeObject *)&DeeUInt16_Type) SET_RESULT_TYPE(&DeeRangeUInt16Iterator_Type);
     else if (targv[0] == (DeeTypeObject *)&DeeUInt32_Type) SET_RESULT_TYPE(&DeeRangeUInt32Iterator_Type);
     else if (targv[0] == (DeeTypeObject *)&DeeUInt64_Type) SET_RESULT_TYPE(&DeeRangeUInt64Iterator_Type);
     else SET_RESULT_TYPE(NULL);
    } else {
     SET_RESULT_TYPE(NULL);
    }
    // This one can still cause an Error.NoMemory...
    return (DeeAstAttribute)((flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS)!=0
     ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT))
     : (DEE_AST_ATTRIBUTE_NOOP));
   }
   SET_RESULT_TYPE(NULL);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_id) {
   SET_RESULT_TYPE(DeeObject_TYPE(Dee_uintptr_t));
   return (DeeAstAttribute)(
    (targc != 1 || (flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS)!=0)
    ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT))
    : (DEE_AST_ATTRIBUTE_NOOP));
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_argv) {
   SET_RESULT_TYPE(&DeeList_Type);
zeroop_noexcept_call_attr:
   // NOTE: This one can't throw an error unless the argument count is incorrect
   return (DeeAstAttribute)((targc != 0)
    ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT))
    : (DEE_AST_ATTRIBUTE_NOOP));
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_swap ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin_enumattr) {
   SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_enumerate) {
   SET_RESULT_TYPE((DeeTypeObject *)&DeeEnumIterator_Type);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_sqr) {
   // '__builtin_sqr(x)' is literally just a fancy way of writing 'x*x'
   if (targc == 1 && targv[0]) return DeeType_BinaryAstAttr(
    targv[0],DeeType_SLOT_ID(tp_mul),flags,targv[0],result_type);
   SET_RESULT_TYPE(NULL);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_find ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_rfind) {
   SET_RESULT_TYPE(DeeObject_TYPE(Dee_ssize_t));
   if (targc == 2 && targv[0] && T_SUPPORTS(targv[0],t_string) && targv[0] == targv[1]) {
    // find("foo","bar"). // 2 arguments; same type; some-kind-of-string
    return (DeeAstAttribute)((flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS)!=0
     ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT))
     : (DEE_AST_ATTRIBUTE_NOOP));
   }
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_count) {
   SET_RESULT_TYPE(DeeObject_TYPE(Dee_size_t));
   if (targc == 2 && targv[0] && T_SUPPORTS(targv[0],t_string) && targv[0] == targv[1]) {
    // count("foo","bar"). // 2 arguments; same type; some-kind-of-string
    return (DeeAstAttribute)((flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS)!=0
     ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT))
     : (DEE_AST_ATTRIBUTE_NOOP));
   }
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_index ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_rindex) {
   SET_RESULT_TYPE(DeeObject_TYPE(Dee_size_t));
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_contains) {
   SET_RESULT_TYPE(&DeeBool_Type);
   if (targc == 2 && targv[0] && T_SUPPORTS(targv[0],t_string) && targv[0] == targv[1]) {
    // contains("foo","bar"). // 2 arguments; same type; some-kind-of-string
    return (DeeAstAttribute)((flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS)!=0
     ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT))
     : (DEE_AST_ATTRIBUTE_NOOP));
   }
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_locate ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_rlocate) {
   SET_RESULT_TYPE((targc >= 1 && targv[0] && T_SUPPORTS(targv[0],t_string)) ? targv[0] : NULL);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_transform ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_locate_all ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_reversed ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_sorted) {
   SET_RESULT_TYPE((targc >= 1 && targv[0]) ? (
    T_SUPPORTS(targv[0],t_transform_returns_same_type) ? targv[0] :
    T_SUPPORTS(targv[0],t_transform_returns_list) ? &DeeList_Type : NULL) : NULL);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_empty ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_non_empty) {
   SET_RESULT_TYPE(&DeeBool_Type);
   return (DeeAstAttribute)((targc == 1 && targv[0] && T_SUPPORTS(targv[0],t_noeffect_empty))
                            ? DEE_AST_ATTRIBUTE_NOOP : DEE_AST_ATTRIBUTE_NONE);
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_front ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_back ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_get ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_at) {
   SET_RESULT_TYPE((targc >= 1 && targv[0] && T_SUPPORTS(targv[0],t_string)) ? targv[0] : NULL);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_front ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_back ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_get ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_at) {
   SET_RESULT_TYPE((targc >= 1 && targv[0] && T_SUPPORTS(targv[0],t_string)) ? targv[0] : NULL);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_seq_segments) {
   SET_RESULT_TYPE((targc >= 1 && targv[0] && T_SUPPORTS(
    targv[0],t_segments_returns_list)) ? &DeeList_Type : NULL);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_gc_collect_now) {
   SET_RESULT_TYPE(DeeObject_TYPE(Dee_size_t));
   return (DeeAstAttribute)((targc == 0 && (flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS)==0)
                            ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEFFECT))
                            : (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEFFECT|DEE_AST_ATTRIBUTE_NOEXCEPT)));
  }
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_class) {
   SET_RESULT_TYPE(&DeeType_Type);
   return DEE_AST_ATTRIBUTE_NONE;
  }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
#if DEE_CONFIG_RUNTIME_HAVE_EXIT
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_exit ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin__exit ||
      self == (DeeObject *)&DeeBuiltinFunction___builtin_fatal) {
   SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type);
   return DEE_AST_ATTRIBUTE_NORETURN;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_atexit) {
   SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type);
   return DEE_AST_ATTRIBUTE_NONE;
  }
#endif /* DEE_CONFIG_RUNTIME_HAVE_EXIT */
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_sin
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_cos
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_tan
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_asin
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_acos
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_atan
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_sinh
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_cosh
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_tanh
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_asinh
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_acosh
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_atanh
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_exp
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_log
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_log10
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_exp2
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_expm1
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_log1p
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_log2
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_logb
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_sqrt
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_cbrt
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_erf
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_tgamma
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_lgamma
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_ceilf
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_floorf
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_truncf
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_roundf
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_rint
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_nearbyint) {
   SET_RESULT_TYPE(targc >= 1 && T_IS_FLOAT(targv[0]) ? targv[0] : (DeeTypeObject *)&DeeDouble_Type);
unary_integral_call_attr:
   return (DeeAstAttribute)(
    (targc == 1 && targv[0] && T_SUPPORTS(targv[0],t_noeffect_int) &&
    (flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS)==0)
    ? (DEE_AST_ATTRIBUTE_NOOP)
    : (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT)));
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_atan2 
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_hypot
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fmod
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_remainder
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_copysign
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_nextafter
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_nexttoward
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fdim
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_scalbn
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_scalbi32n) {
   SET_RESULT_TYPE(targc >= 1 && T_IS_FLOAT(targv[0]) ? targv[0] : (DeeTypeObject *)&DeeDouble_Type);
binary_integral_call_attr:
   return (DeeAstAttribute)((targc == 2 &&
     targv[0] && T_SUPPORTS(targv[0],t_noeffect_int) &&
     targv[1] && T_SUPPORTS(targv[1],t_noeffect_int) &&
    (flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS)==0)
    ? (DEE_AST_ATTRIBUTE_NOOP)
    : (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT)));
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_sincos
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_asincos
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_sincosh
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_asincosh
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_frexp
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_modf) {
   SET_RESULT_TYPE(&DeeTuple_Type);
   goto unary_integral_call_attr;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_ilogb) {
   SET_RESULT_TYPE(DeeObject_TYPE(int));
   goto unary_integral_call_attr;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_i32round
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_i32rint) {
   SET_RESULT_TYPE(DeeObject_TYPE(Dee_int32_t));
   goto unary_integral_call_attr;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_i64round
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_i64rint) {
   SET_RESULT_TYPE(DeeObject_TYPE(Dee_int64_t));
   goto unary_integral_call_attr;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_remquo) {
   SET_RESULT_TYPE(&DeeTuple_Type);
   goto binary_integral_call_attr;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_isfinite
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_isinf
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_isnan
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_isnormal
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_signbit) {
   SET_RESULT_TYPE(&DeeBool_Type);
   goto unary_integral_call_attr;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_isgreater
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_isgreaterequal
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_isless
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_islessequal
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_islessgreater
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_isunordered) {
   SET_RESULT_TYPE(&DeeBool_Type);
   goto binary_integral_call_attr;
  }

#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  // Raw memory / Pointer related
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_memcpy
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_mempcpy
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_memccpy
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_rawmemchr
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_memmove
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_memchr
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_memrchr
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_memmem
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_memset
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_malloc
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_calloc
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_realloc
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_aligned_malloc
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_aligned_calloc
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_aligned_realloc) {
   SET_RESULT_TYPE(_try_get_void_pointer_type());
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_memcmp
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strcoll
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strcmp
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strncmp
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strcasecmp
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strncasecmp) {
   SET_RESULT_TYPE(DeeObject_TYPE(int));
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_strxfrm
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strcspn
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strspn
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strlen
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strnlen) {
   SET_RESULT_TYPE(DeeObject_TYPE(Dee_size_t));
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_strcpy
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strncpy
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_stpcpy
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_stpncpy
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strcat
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strncat
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strchr
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strchrnul
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strcasestr
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strpbrk
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strrchr
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strstr
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strtok
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strerror
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strdup
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_strndup) {
   SET_RESULT_TYPE(_try_get_char_pointer_type());
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_free
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_aligned_free) {
   SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type);
   return DEE_AST_ATTRIBUTE_NONE;
  }
#endif
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_FILE
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_FUNCTION) {
   SET_RESULT_TYPE(&DeeString_Type);
   goto zeroop_noexcept_call_attr;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_LINE) {
   SET_RESULT_TYPE(DeeObject_TYPE(Dee_int32_t));
   return (DeeAstAttribute)((targc == 0 && (flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS)==0)
    ? (DEE_AST_ATTRIBUTE_NOOP) : (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT)));
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_code) {
   SET_RESULT_TYPE(&DeeCode_Type);
   return DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT);
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_get_stacklimit) {
   SET_RESULT_TYPE(DeeObject_TYPE(Dee_size_t));
   goto zeroop_noexcept_call_attr;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_set_stacklimit) {
   SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type);
   goto unary_integral_call_attr;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_bswap16) { SET_RESULT_TYPE((DeeTypeObject *)&DeeUInt16_Type); goto unary_integral_call_attr; }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_bswap32) { SET_RESULT_TYPE((DeeTypeObject *)&DeeUInt32_Type); goto unary_integral_call_attr; }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_bswap64) { SET_RESULT_TYPE((DeeTypeObject *)&DeeUInt64_Type); goto unary_integral_call_attr; }
  // Stuff from the fs api
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_user
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_machine
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_getcwd
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_gettmp
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_gettmpname
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_gethome
#ifdef DEE_PLATFORM_WINDOWS
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_win32modulename
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_win32dlldirectory
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_win32systemdirectory
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_win32windowsdirectory
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_win32systemwindowsdirectory
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_win32syswow64directory
#endif /* DEE_PLATFORM_WINDOWS */
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_uidname
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_gidname) {
   SET_RESULT_TYPE(&DeeUtf8String_Type);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_wuser
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_wmachine
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_wgetcwd
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_wgettmp
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_wgettmpname
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_wgethome
#ifdef DEE_PLATFORM_WINDOWS
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_win32modulename
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_win32dlldirectory
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_win32systemdirectory
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_win32windowsdirectory
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_win32systemwindowsdirectory
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_win32syswow64directory
#endif /* DEE_PLATFORM_WINDOWS */
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_wuidname
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_wgidname) {
   SET_RESULT_TYPE(&DeeWideString_Type);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_chdir) {
   SET_RESULT_TYPE(&DeeWideString_Type);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_listenv
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_wlistenv
#ifdef DEE_PLATFORM_WINDOWS
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_win32drives
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_win32wdrives
#endif /* DEE_PLATFORM_WINDOWS */
   ) {
   SET_RESULT_TYPE(&DeeList_Type);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_hasenv
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_isabs
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_isfile
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_isdir
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_islink
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_isdrive
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_ismount
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_ishidden
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_isxfile /* --Insert X-Files Theme here-- */
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_ischrdev
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_isblkdev
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_isfifo
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_issocket
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_exists) {
   SET_RESULT_TYPE(&DeeBool_Type);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_getenv
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_head
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_tail
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_file
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_ext
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_drive
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_inctrail
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_exctrail
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_abs
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_rel
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_join
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_expanduser
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_path_expandvars
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_readlink) {
   SET_RESULT_TYPE((targc >= 1 && targv[0] && T_SUPPORTS(targv[0],t_string)) ? targv[0] : NULL);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_delenv
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_setenv
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_setatime
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_setctime
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_setmtime
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_settimes
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_remove
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_rmfile
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_mkdir
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_rmdir
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_copy
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_move
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_link
#ifdef DEE_PLATFORM_WINDOWS
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_win32hardlink
#endif /* DEE_PLATFORM_WINDOWS */
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_chmod
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_chown
   ) {
   SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_getatime
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_getctime
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_getmtime) {
   SET_RESULT_TYPE(&DeeTime_Type);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_gettimes
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_getown) {
   SET_RESULT_TYPE(&DeeTuple_Type);
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_getmod) {
   SET_RESULT_TYPE(DeeObject_TYPE(Dee_mode_t));
   return DEE_AST_ATTRIBUTE_NONE;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_uidof
   || self == (DeeObject *)&DeeBuiltinFunction___builtin_fs_gidof) {
#ifdef DEE_PLATFORM_WINDOWS
   SET_RESULT_TYPE(&DeeFSWin32SID_Type);
#else
   DEE_ASSERT(DeeObject_TYPE(Dee_uid_t) == DeeObject_TYPE(Dee_gid_t));
   SET_RESULT_TYPE(DeeObject_TYPE(Dee_uid_t));
#endif
   return DEE_AST_ATTRIBUTE_NONE;
  }
  // Builtin operator predicates
  if (self == (DeeObject *)&DeeBuiltinFunction___bool__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_bool),flags,result_type); SET_RESULT_TYPE(&DeeBool_Type); return DEE_AST_ATTRIBUTE_NONE; }
  if (self == (DeeObject *)&DeeBuiltinFunction___repr__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_repr),flags,result_type); SET_RESULT_TYPE(&DeeString_Type); return DEE_AST_ATTRIBUTE_NONE; }
  if (self == (DeeObject *)&DeeBuiltinFunction___copy__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_copy_ctor), flags,result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___type__) { SET_RESULT_TYPE(targc == 1 && targv[0] ? Dee_TYPE(targv[0]) : &DeeType_Type); return DEE_AST_ATTRIBUTE_NOOP; }
  if (self == (DeeObject *)&DeeBuiltinFunction___move__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_move_ctor),flags,result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___copyassign__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_copy_assign),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___moveassign__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_move_assign),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___not__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_not),flags,result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___neg__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_neg),flags,result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___pos__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_pos),flags,result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___inv__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_inv),flags,result_type); goto after_cfunction; }
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  if (self == (DeeObject *)&DeeBuiltinFunction___ref__) {
   if (targc == 1) {
    DeeTypeObject const *tb = targv[0];
    SET_RESULT_TYPE((DeeTypeObject *)&DeePointer_Type);
    return (DeeAstAttribute)((tb && (DeeStructuredType_Check(tb) || DeeLValueType_Check(tb)))
                             ? ((flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS) != 0
                               ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT))
                               : (DEE_AST_ATTRIBUTE_NOOP))
                             : (DEE_AST_ATTRIBUTE_NOEFFECT));
   }
   goto after_cfunction;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___deref__) {
   if (targc == 1) {
    DeeTypeObject const *tb = targv[0];
    SET_RESULT_TYPE((DeeTypeObject *)&DeePointer_Type);
    return (DeeAstAttribute)((tb && (DeePointerType_Check(tb)))
                             ? ((flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS) != 0
                               ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT))
                               : (DEE_AST_ATTRIBUTE_NOOP))
                             : (DEE_AST_ATTRIBUTE_NOEFFECT));
   }
   goto after_cfunction;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_pointer_add) {
   if (targc == 1) {
    DeeTypeObject const *tb = targv[0];
    SET_RESULT_TYPE((DeeTypeObject *)&DeePointerType_Type);
    return (DeeAstAttribute)((tb && DeeType_IsSameOrDerived(tb,&DeeStructuredType_Type))
                             ? ((flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS) != 0
                               ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT))
                               : (DEE_AST_ATTRIBUTE_NOOP))
                             : (DEE_AST_ATTRIBUTE_NOEFFECT));
   }
   goto after_cfunction;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_pointer_rem) {
   if (targc == 1) {
    DeeTypeObject const *tb = targv[0];
    SET_RESULT_TYPE((DeeTypeObject *)&DeeStructuredType_Type);
    return (DeeAstAttribute)((tb && DeeType_IsSameOrDerived(tb,&DeePointerType_Type))
                             ? ((flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS) != 0
                               ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT))
                               : (DEE_AST_ATTRIBUTE_NOOP))
                             : (DEE_AST_ATTRIBUTE_NOEFFECT));
   }
   goto after_cfunction;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_lvalue_add) {
   if (targc == 1) {
    DeeTypeObject const *tb = targv[0];
    SET_RESULT_TYPE((DeeTypeObject *)&DeeLValueType_Type);
    return (DeeAstAttribute)((tb && DeeType_IsSameOrDerived(tb,&DeeStructuredType_Type))
                             ? ((flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS) != 0
                               ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT))
                               : (DEE_AST_ATTRIBUTE_NOOP))
                             : (DEE_AST_ATTRIBUTE_NOEFFECT));
   }
   goto after_cfunction;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___builtin_lvalue_rem) {
   if (targc == 1) {
    DeeTypeObject const *tb = targv[0];
    SET_RESULT_TYPE((DeeTypeObject *)&DeeStructuredType_Type);
    return (DeeAstAttribute)((tb && DeeType_IsSameOrDerived(tb,&DeeLValueType_Type))
                             ? ((flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS) != 0
                               ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT))
                               : (DEE_AST_ATTRIBUTE_NOOP))
                             : (DEE_AST_ATTRIBUTE_NOEFFECT));
   }
   goto after_cfunction;
  }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
  if (self == (DeeObject *)&DeeBuiltinFunction___inc__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_inc),flags,result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___dec__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_dec),flags,result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___incpost__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_incpost),flags,result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___decpost__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_decpost),flags,result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___lo__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_cmp_lo),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___le__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_cmp_le),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___eq__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_cmp_eq),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___ne__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_cmp_ne),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___gr__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_cmp_gr),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___ge__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_cmp_ge),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___add__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_add),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___sub__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_sub),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___mul__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_mul),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___div__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_div),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___mod__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_mod),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___shl__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_shl),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___shr__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_shr),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___and__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_and),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___or__)  { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_or), flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___xor__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_xor),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___pow__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_pow),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___iadd__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_iadd),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___isub__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_isub),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___imul__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_imul),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___idiv__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_idiv),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___imod__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_imod),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___ishl__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_ishl),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___ishr__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_ishr),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___iand__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_iand),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___ior__)  { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_ior), flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___ixor__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_ixor),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___ipow__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_ipow),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___call__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_call),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___callv__) { if (targc >= 1 && targv[0]) return DeeType_CallAstAttr(targv[0],flags,targc-1,targv+1,result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___size__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_seq_size),flags,result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___getitem__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_seq_get),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___delitem__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_seq_del),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___setitem__) { if (targc == 3 && targv[0]) return DeeType_TrinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_seq_set),flags,targv[1],targv[2],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___getrange__) { if (targc == 3 && targv[0]) return DeeType_TrinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_seq_range_get),flags,targv[1],targv[2],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___delrange__) { if (targc == 3 && targv[0]) return DeeType_TrinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_seq_range_del),flags,targv[1],targv[2],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___setrange__) { if (targc == 4 && targv[0]) { SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type); return DeeType_SetRangeAstAttr(targv[0],flags,targv[1],targv[2],targv[3]); } goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___is__) { SET_RESULT_TYPE(&DeeBool_Type); return (DeeAstAttribute)((targc == 2) ? DEE_AST_ATTRIBUTE_NOOP : DEE_AST_ATTRIBUTE_NONE); }
  if (self == (DeeObject *)&DeeBuiltinFunction___contains__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_seq_contains),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___getattr__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_attr_get),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___hasattr__) {
   SET_RESULT_TYPE(&DeeBool_Type);
   return (DeeAstAttribute)((targc == 2 && targv[1] && targv[1] == &DeeString_Type)
                            // v __hasattr__ will try to call __getattr__, and can cause an error because of that!
                            ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT))
                            : DEE_AST_ATTRIBUTE_NOEFFECT);
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___delattr__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_attr_del),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___setattr__) { if (targc == 3 && targv[0]) return DeeType_TrinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_attr_set),flags,targv[1],targv[2],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___iterself__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_seq_iter_self),flags,result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___iternext__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_seq_iter_next),flags,result_type); goto after_cfunction; }
//todo: if (self == (DeeObject *)&DeeBuiltinFunction___hash__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_hash),flags,result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___read__) {
   if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_io_read),flags,targv[1],result_type);
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
   if (targc == 3 && targv[0]) return DeeType_TrinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_io_read),flags,targv[1],targv[2],result_type);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
   goto after_cfunction;
  }
  if (self == (DeeObject *)&DeeBuiltinFunction___write__) {
   if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_io_write),flags,targv[1],result_type);
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
   if (targc == 3 && targv[0]) return DeeType_TrinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_io_write),flags,targv[1],targv[2],result_type);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
   goto after_cfunction;
  }
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  if (self == (DeeObject *)&DeeBuiltinFunction___readp__) { if (targc == 3 && targv[0]) return DeeType_TrinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_io_write),flags,targv[1],targv[2],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___writep__) { if (targc == 3 && targv[0]) return DeeType_TrinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_io_write),flags,targv[1],targv[2],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___readnp__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_io_read),flags,targv[1],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___writenp__) { if (targc == 2 && targv[0]) return DeeType_BinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_io_write),flags,targv[1],result_type); goto after_cfunction; }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
  if (self == (DeeObject *)&DeeBuiltinFunction___seek__) { if (targc == 3 && targv[0]) return DeeType_TrinaryAstAttr(targv[0],DeeType_SLOT_ID(tp_io_seek),flags,targv[1],targv[2],result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___flush__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_io_flush),flags,result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___trunc__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_io_trunc),flags,result_type); goto after_cfunction; }
  if (self == (DeeObject *)&DeeBuiltinFunction___close__) { if (targc == 1 && targv[0]) return DeeType_UnaryAstAttr(targv[0],DeeType_SLOT_ID(tp_io_close),flags,result_type); goto after_cfunction; }
 }
after_cfunction:
 if (tp_self == &DeeSuper_Type) return DeeObject_CallAstAttr(
  DeeSuper_TYPE(self),DeeSuper_SELF(self),flags,targc,targv,result_type);

 return DeeType_CallAstAttr(tp_self,flags,targc,targv,result_type);
}




static DeeAstAttribute DeeMemberDef_GenericGetAttrAstAttr(
 DEE_A_IN struct DeeMemberDef const *memberdef, DEE_A_IN Dee_uint32_t flags,
 DEE_A_OUT_OPT DeeTypeObject const **result_type) {
 DEE_ASSERT(memberdef);
 if ((memberdef->d_access&DeeMemberAccess_READ)==0) {
  SET_RESULT_TYPE(NULL); // Can't access this member --> Using it will cause an error
  return DEE_AST_ATTRIBUTE_ALWAYSTHROW(flags);
 }
 if (result_type) {
  if ((*result_type = DeeMember_Type(memberdef)) == NULL) DeeError_Handled();
 }
 return (DeeAstAttribute)(
  (flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS) != 0
  ? (DEE_AST_ATTRIBUTE_NOOP & ~(DEE_AST_ATTRIBUTE_NOEXCEPT))
  : (DEE_AST_ATTRIBUTE_NOOP));
}
static DeeAstAttribute DeeGetSetDef_GenericGetAttrAstAttr(
 DEE_A_IN struct DeeGetSetDef const *getsetdef, DEE_A_IN Dee_uint32_t flags,
 DEE_A_OUT_OPT DeeTypeObject const **result_type) {
 DEE_ASSERT(getsetdef);
 if (!getsetdef->d_get) {
  SET_RESULT_TYPE(NULL); // Can't access this member --> Using it will cause an error
  return DEE_AST_ATTRIBUTE_ALWAYSTHROW(flags);
 }
 if (result_type) {
  //if (getsetdef->d_get == ...) {
  //  TODO
  //}
  SET_RESULT_TYPE(NULL);
 }
 return DEE_AST_ATTRIBUTE_NONE;
}
static DeeAstAttribute DeeMethodDef_GenericGetAttrAstAttr(
 DEE_A_IN struct DeeMethodDef const *methoddef, DEE_A_IN Dee_uint32_t flags,
 DEE_A_OUT_OPT DeeTypeObject const **result_type) {
 DEE_ASSERT(methoddef);
 (void)flags;
 if (result_type) {
  //if (methoddef->d_func == ...) {
  //  TODO
  //}
  SET_RESULT_TYPE(NULL);
 }
 return DEE_AST_ATTRIBUTE_NONE;
}
static DeeAstAttribute DeeObject_GenericGetAttrAstAttr(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN_OBJECT(DeeStringObject) *member,
 DEE_A_OUT_OPT DeeTypeObject const **result_type) {
 DeeTypeObject const *tp_iter;
 struct DeeMemberDef const *members;
 struct DeeGetSetDef const *getsets;
 struct DeeMethodDef const *methods;
 char const *member_str;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT(DeeObject_Check(member) && DeeString_Check(member));
 member_str = DeeString_STR(member);
 tp_iter = self;
 while (1) {
  // TODO: Use the attribute cache
  members = DeeType_GET_SLOT(tp_iter,tp_members);
  DEE_ASSERT(members);
  while (members->d_name) {
   if (strcmp(members->d_name,member_str) == 0) {
    // Found it!
    return DeeMemberDef_GenericGetAttrAstAttr(members,flags,result_type);
   }
   ++members;
  }
  getsets = DeeType_GET_SLOT(tp_iter,tp_getsets);
  DEE_ASSERT(getsets);
  while (getsets->d_name) {
   if (strcmp(getsets->d_name,member_str) == 0) {
    // Found it!
    return DeeGetSetDef_GenericGetAttrAstAttr(getsets,flags,result_type);
   }
   ++getsets;
  }
  methods = DeeType_GET_SLOT(tp_iter,tp_methods);
  DEE_ASSERT(methods);
  while (methods->d_name) {
   if (strcmp(methods->d_name,member_str) == 0) {
    // Found it!
    return DeeMethodDef_GenericGetAttrAstAttr(methods,flags,result_type);
   }
   ++methods;
  }
  if ((tp_iter = DeeType_BASE(tp_iter)) == NULL) break;
 }
 SET_RESULT_TYPE(NULL); // Unknown attribute
 return DEE_AST_ATTRIBUTE_ALWAYSTHROW(flags);
}

DeeAstAttribute DeeType_GetAttrAstAttr(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN_OBJECT(DeeStringObject) *member,
 DEE_A_OUT_OPT DeeTypeObject const **result_type) {
 DeeTypeObject const *operator_type;
 DeeType_SLOT_TYPE(tp_attr_get) callback;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT(DeeObject_Check(member) && DeeString_Check(member));
 (void)flags,member;
 if (self == (DeeTypeObject *)&DeeNone_Type) {
  SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type); // no-op
  return DEE_AST_ATTRIBUTE_NOOP;
 }
 operator_type = self;
 DeeType_FIND_SLOT(operator_type,tp_attr_get);
 callback = DeeType_GET_SLOT(operator_type,tp_attr_get);
 // Detect common attribute protocols
 if (callback == (DeeType_SLOT_TYPE(tp_attr_get))&DeeObject_GenericGetAttr
  || callback == (DeeType_SLOT_TYPE(tp_attr_get))&DeeType_GetAttr
  || callback == (DeeType_SLOT_TYPE(tp_attr_get))&DeeClass_GetAttr
  || callback == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStructured_GetAttr) {
  // NOTE: tp_class_members, ... can't be detected, as those are only
  //       available one level below (aka. in instances of 'self')
  return DeeObject_GenericGetAttrAstAttr(self,flags,member,result_type);
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_get))&DeeInstance_GetAttr) {
  DEE_ASSERT(DeeClass_Check(self));
  // TODO: 'self' is a class type, so we can iterate user-defined methods/getsets
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStruct_GetAttr) {
  DEE_ASSERT(DeeStructType_Check(self));
  // TODO: iterate struct members
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_get))&_deelvalue_tp_attr_get) {
  DEE_ASSERT(DeeLValueType_Check(self)); // Scan the base of the lvalue type
  return DeeType_GetAttrAstAttr(DeeLValueType_BASE(self),flags,member,result_type);
 }
 SET_RESULT_TYPE(NULL);
 return DEE_AST_ATTRIBUTE_NONE;
}

DEE_A_RET_WUNUSED DeeAstAttribute DeeType_HasAttrAstAttr(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN_OBJECT(DeeStringObject) *member) {
 DeeTypeObject const *operator_type;
 DeeType_SLOT_TYPE(tp_attr_get) callback;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT(DeeObject_Check(member) && DeeString_Check(member));
 (void)member,flags;
 if (self == (DeeTypeObject *)&DeeNone_Type) return DEE_AST_ATTRIBUTE_NOOP;
 operator_type = self;
 DeeType_FIND_SLOT(operator_type,tp_attr_get);
 callback = DeeType_GET_SLOT(operator_type,tp_attr_get);
 // Detect common attribute protocols
 // >> These are the smae protocols for which 'DeeObject_HasAttr' is noexcept, making these no-op
 if (callback == (DeeType_SLOT_TYPE(tp_attr_get))&DeeObject_GenericGetAttr)  return DEE_AST_ATTRIBUTE_NOOP;
 if (callback == (DeeType_SLOT_TYPE(tp_attr_get))&DeeType_GetAttr)           return DEE_AST_ATTRIBUTE_NOOP;
 if (callback == (DeeType_SLOT_TYPE(tp_attr_get))&_deesuper_tp_attr_get)     return DEE_AST_ATTRIBUTE_NOOP;
 if (callback == (DeeType_SLOT_TYPE(tp_attr_get))&DeeInstance_GetAttr)       return DEE_AST_ATTRIBUTE_NOOP;
 if (callback == (DeeType_SLOT_TYPE(tp_attr_get))&DeeClass_GetAttr)          return DEE_AST_ATTRIBUTE_NOOP;
 if (callback == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStructured_GetAttr)     return DEE_AST_ATTRIBUTE_NOOP;
 if (callback == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStruct_GetAttr)         return DEE_AST_ATTRIBUTE_NOOP;
 if (callback == (DeeType_SLOT_TYPE(tp_attr_get))&_deelvalue_tp_attr_get)    return DEE_AST_ATTRIBUTE_NOOP;
 if (callback == (DeeType_SLOT_TYPE(tp_attr_get))&_deedexmodule_tp_attr_get) return DEE_AST_ATTRIBUTE_NOOP;
 return DEE_AST_ATTRIBUTE_NONE;
}

DEE_A_RET_WUNUSED DeeAstAttribute DeeType_DelAttrAstAttr(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN_OBJECT(DeeStringObject) *member) {
 DeeTypeObject const *operator_type;
 DeeType_SLOT_TYPE(tp_attr_del) callback;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT(DeeObject_Check(member) && DeeString_Check(member));
 if (self == (DeeTypeObject *)&DeeNone_Type) return DEE_AST_ATTRIBUTE_NOOP;
 (void)flags,member;
 operator_type = self;
 DeeType_FIND_SLOT(operator_type,tp_attr_del);
 callback = DeeType_GET_SLOT(operator_type,tp_attr_del);
 // Detect common attribute protocols
 if (callback == (DeeType_SLOT_TYPE(tp_attr_del))&DeeObject_GenericDelAttr) {
  // TODO
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_del))&DeeType_DelAttr) {
  // TODO
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_del))&_deesuper_tp_attr_del) {
  // TODO
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_del))&DeeInstance_DelAttr) {
  // TODO
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_del))&DeeClass_DelAttr) {
  // TODO
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_del))&DeeStructured_DelAttr) {
  // TODO
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_del))&DeeStruct_DelAttr) {
  // TODO
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_del))&_deelvalue_tp_attr_del) {
  // TODO
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_del))&_deedexmodule_tp_attr_del) {
  // TODO
 }
 return DEE_AST_ATTRIBUTE_NONE;
}

DEE_A_RET_WUNUSED DeeAstAttribute DeeType_SetAttrAstAttr(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN_OBJECT(DeeStringObject) *member, DEE_A_IN_OPT DeeTypeObject const *tvalue) {
 DeeTypeObject const *operator_type;
 DeeType_SLOT_TYPE(tp_attr_set) callback;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT(DeeObject_Check(member) && DeeString_Check(member));
 DEE_ASSERT(!tvalue || (DeeObject_Check(tvalue) && DeeType_Check(tvalue)));
 if (self == (DeeTypeObject *)&DeeNone_Type) return DEE_AST_ATTRIBUTE_NOOP;
 (void)flags,member,tvalue;
 operator_type = self;
 DeeType_FIND_SLOT(operator_type,tp_attr_set);
 callback = DeeType_GET_SLOT(operator_type,tp_attr_set);
 // Detect common attribute protocols
 if (callback == (DeeType_SLOT_TYPE(tp_attr_set))&DeeObject_GenericSetAttr) {
  // TODO
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_set))&DeeType_SetAttr) {
  // TODO
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_set))&_deesuper_tp_attr_set) {
  // TODO
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_set))&DeeInstance_SetAttr) {
  // TODO
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_set))&DeeClass_SetAttr) {
  // TODO
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_set))&DeeStructured_SetAttr) {
  // TODO
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_set))&DeeStruct_SetAttr) {
  // TODO
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_set))&_deelvalue_tp_attr_set) {
  // TODO
 }
 if (callback == (DeeType_SLOT_TYPE(tp_attr_set))&_deedexmodule_tp_attr_set) {
  // TODO
 }
 return DEE_AST_ATTRIBUTE_NONE;
}

DeeAstAttribute DeeType_CallAttrAstAttr(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN_OBJECT(DeeStringObject) *member, DEE_A_IN Dee_size_t targc,
 DEE_A_IN_R_OPT(targc) DeeTypeObject const *const *targv,
 DEE_A_OUT_OPT DeeTypeObject const **result_type) {
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT(DeeObject_Check(member) && DeeString_Check(member));
 DEE_ASSERT(!targc || targv);
 if (self == (DeeTypeObject *)&DeeNone_Type) {
  SET_RESULT_TYPE((DeeTypeObject *)&DeeNone_Type); // no-op
  return DEE_AST_ATTRIBUTE_NOOP;
 }
 (void)self,flags,member,targc,targv; // TODO
 SET_RESULT_TYPE(NULL);
 return DEE_AST_ATTRIBUTE_NONE;
}
DeeAstAttribute DeeObject_CallAttrAstAttr(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeObject *self,
 DEE_A_IN Dee_uint32_t flags, DEE_A_IN_OBJECT(DeeStringObject) *member,
 DEE_A_IN Dee_size_t targc, DEE_A_IN_R_OPT(targc) DeeTypeObject const *const *targv,
 DEE_A_OUT_OPT DeeTypeObject const **result_type) {
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(member) && DeeString_Check(member));
 if (tp_self == &DeeSuper_Type) return DeeObject_CallAttrAstAttr(
  DeeSuper_TYPE(self),DeeSuper_SELF(self),flags,member,targc,targv,result_type);
 // TODO
 return DeeType_CallAttrAstAttr(tp_self,flags,member,targc,targv,result_type);
}

#undef SET_RESULT_TYPE

DEE_DECL_END
