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
#ifndef GUARD_DEEMON_MEMBERDEF_C
#define GUARD_DEEMON_MEMBERDEF_C 1
#define DEE_LIMITED_API 1


// /include/*
#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/float.h>
#include <deemon/integer.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/string.h>
#include <deemon/structured.h>
#include <deemon/type.h>
#include <deemon/weakref.h>

// /src/*
#include <deemon/__xconf.inl>

// */ (nano...)

DEE_DECL_BEGIN

static DeeTypeObject *_DeeMember_GetBasicType(DeeMemberType mtype) {
 DeeTypeObject *result;
 switch (mtype) {
  case DeeMemberType_ldouble:
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
   return DeeObject_TYPE(ldouble);
#endif
  case DeeMemberType_double:
#ifdef DEE_TYPES_SIZEOF_DOUBLE
   return DeeObject_TYPE(double);
#endif
  case DeeMemberType_float:
#ifdef DEE_TYPES_SIZEOF_FLOAT
   return DeeObject_TYPE(float);
#endif
  case DeeMemberType_ATOMIC|DeeMemberType_none:
  case DeeMemberType_none: return DeeObject_TYPE(none);
  case DeeMemberType_int8_t: return DeeObject_TYPE(Dee_int8_t);
  case DeeMemberType_int16_t: return DeeObject_TYPE(Dee_int16_t);
  case DeeMemberType_int32_t: return DeeObject_TYPE(Dee_int32_t);
  case DeeMemberType_int64_t: return DeeObject_TYPE(Dee_int64_t);
  case DeeMemberType_uint8_t: return DeeObject_TYPE(Dee_uint8_t);
  case DeeMemberType_uint16_t: return DeeObject_TYPE(Dee_uint16_t);
  case DeeMemberType_uint32_t: return DeeObject_TYPE(Dee_uint32_t);
  case DeeMemberType_uint64_t: return DeeObject_TYPE(Dee_uint64_t);
  case DeeMemberType_ATOMIC|DeeMemberType_int8_t: return DeeObject_TYPE(atomic(Dee_int8_t));
  case DeeMemberType_ATOMIC|DeeMemberType_int16_t: return DeeObject_TYPE(atomic(Dee_int16_t));
  case DeeMemberType_ATOMIC|DeeMemberType_int32_t: return DeeObject_TYPE(atomic(Dee_int32_t));
  case DeeMemberType_ATOMIC|DeeMemberType_int64_t: return DeeObject_TYPE(atomic(Dee_int64_t));
  case DeeMemberType_ATOMIC|DeeMemberType_uint8_t: return DeeObject_TYPE(atomic(Dee_uint8_t));
  case DeeMemberType_ATOMIC|DeeMemberType_uint16_t: return DeeObject_TYPE(atomic(Dee_uint16_t));
  case DeeMemberType_ATOMIC|DeeMemberType_uint32_t: return DeeObject_TYPE(atomic(Dee_uint32_t));
  case DeeMemberType_ATOMIC|DeeMemberType_uint64_t: return DeeObject_TYPE(atomic(Dee_uint64_t));

  case DeeMemberType_widechar:
  case DeeMemberType_widestring:
  case DeeMemberType_inplace_widestring:
  case DeeMemberType_ATOMIC|DeeMemberType_widechar:
  case DeeMemberType_ATOMIC|DeeMemberType_widestring:
#if DEE_CONFIG_HAVE_ENCODING_WIDE
   return &DeeWideString_Type;
#else
   return &DeeString_Type;
#endif
  case DeeMemberType_utf8char:
  case DeeMemberType_utf8string:
  case DeeMemberType_inplace_utf8string:
  case DeeMemberType_ATOMIC|DeeMemberType_utf8char:
  case DeeMemberType_ATOMIC|DeeMemberType_utf8string:
#if DEE_CONFIG_HAVE_ENCODING_UTF8
   return &DeeUtf8String_Type;
#else
   return &DeeString_Type;
#endif
  case DeeMemberType_utf16char:
  case DeeMemberType_utf16string:
  case DeeMemberType_inplace_utf16string:
  case DeeMemberType_ATOMIC|DeeMemberType_utf16char:
  case DeeMemberType_ATOMIC|DeeMemberType_utf16string:
#if DEE_CONFIG_HAVE_ENCODING_UTF16
   return &DeeUtf16String_Type;
#else
   return &DeeString_Type;
#endif
  case DeeMemberType_utf32char:
  case DeeMemberType_utf32string:
  case DeeMemberType_inplace_utf32string:
  case DeeMemberType_ATOMIC|DeeMemberType_utf32char:
  case DeeMemberType_ATOMIC|DeeMemberType_utf32string:
#if DEE_CONFIG_HAVE_ENCODING_UTF32
   return &DeeUtf32String_Type;
#else
   return &DeeString_Type;
#endif

  case DeeMemberType_object:
  case DeeMemberType_object_null:
  case DeeMemberType_weak_object:
  case DeeMemberType_weak_object_null:
  case DeeMemberType_ATOMIC|DeeMemberType_object:
  case DeeMemberType_ATOMIC|DeeMemberType_object_null:
  case DeeMemberType_ATOMIC|DeeMemberType_weak_object:
  case DeeMemberType_ATOMIC|DeeMemberType_weak_object_null:
   return &DeeObject_Type;

  default:
   DEE_ASSERT((mtype & DeeMemberType_POINTER_MASK) != 0);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   if ((result = _DeeMember_GetBasicType(mtype-DeeMemberType_POINTER_X1)) != NULL) {
#if DEE_CONFIG_HAVE_ENCODING_WIDE
    if (result == &DeeWideString_Type) result = DeeObject_TYPE(Dee_WideChar);
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
    if (result == &DeeUtf8String_Type) result = DeeObject_TYPE(Dee_Utf8Char);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
    if (result == &DeeUtf16String_Type) result = DeeObject_TYPE(Dee_Utf16Char);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
    if (result == &DeeUtf32String_Type) result = DeeObject_TYPE(Dee_Utf32Char);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
    result = DeeType_Pointer(result);
   }
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   DeeError_NotImplemented_str("DEE_CONFIG_RUNTIME_HAVE_POINTERS");
   result = NULL;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   break;
 }
 return result;
}
DEE_A_RET_EXCEPT(NULL) DeeTypeObject *DeeMember_Type(
 DEE_A_IN struct DeeMemberDef const *def) {
 DEE_ASSERT(def);
 return _DeeMember_GetBasicType(def->d_type&~DeeMemberType_CONST);
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeMember_Get(
 DEE_A_IN DeeObject const *base_ob, DEE_A_IN struct DeeMemberDef const *def) {
 DeeObject *result;
 DEE_ASSERT(base_ob && def);
 *(Dee_uintptr_t *)&base_ob += def->d_offset;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#if !DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS
 if ((def->d_access&DeeMemberAccess_WRITE)!=0)
#endif /* !DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS */
 if ((def->d_type&DeeMemberType_CONST)==0) {
  // Return an lvalue
  DeeTypeObject *lv_type = _DeeMember_GetBasicType(def->d_type&~DeeMemberType_ATOMIC);
  if (lv_type && DeeStructuredType_Check(lv_type)) {
   lv_type = DeeType_LValue(lv_type);
   return lv_type ? DeeLValue_New(lv_type,(void *)base_ob) : NULL;
  }
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 switch (def->d_type) {
  case DeeMemberType_CONST|DeeMemberType_int8_t: result = DeeObject_New(Dee_int8_t,(Dee_int8_t)def->d_offset); break;
  case DeeMemberType_CONST|DeeMemberType_int16_t: result = DeeObject_New(Dee_int16_t,(Dee_int16_t)def->d_offset); break;
  case DeeMemberType_CONST|DeeMemberType_int32_t: result = DeeObject_New(Dee_int32_t,(Dee_int32_t)def->d_offset); break;
  case DeeMemberType_CONST|DeeMemberType_int64_t: result = DeeObject_New(Dee_int64_t,(Dee_int64_t)def->d_offset); break;
  case DeeMemberType_CONST|DeeMemberType_uint8_t: result = DeeObject_New(Dee_uint8_t,(Dee_uint8_t)def->d_offset); break;
  case DeeMemberType_CONST|DeeMemberType_uint16_t: result = DeeObject_New(Dee_uint16_t,(Dee_uint16_t)def->d_offset); break;
  case DeeMemberType_CONST|DeeMemberType_uint32_t: result = DeeObject_New(Dee_uint32_t,(Dee_uint32_t)def->d_offset); break;
  case DeeMemberType_CONST|DeeMemberType_uint64_t: result = DeeObject_New(Dee_uint64_t,(Dee_uint64_t)def->d_offset); break;
  case DeeMemberType_CONST|DeeMemberType_ATOMIC|DeeMemberType_int8_t: result = DeeObject_New(atomic(Dee_int8_t),(Dee_int8_t)def->d_offset); break;
  case DeeMemberType_CONST|DeeMemberType_ATOMIC|DeeMemberType_int16_t: result = DeeObject_New(atomic(Dee_int16_t),(Dee_int16_t)def->d_offset); break;
  case DeeMemberType_CONST|DeeMemberType_ATOMIC|DeeMemberType_int32_t: result = DeeObject_New(atomic(Dee_int32_t),(Dee_int32_t)def->d_offset); break;
  case DeeMemberType_CONST|DeeMemberType_ATOMIC|DeeMemberType_int64_t: result = DeeObject_New(atomic(Dee_int64_t),(Dee_int64_t)def->d_offset); break;
  case DeeMemberType_CONST|DeeMemberType_ATOMIC|DeeMemberType_uint8_t: result = DeeObject_New(atomic(Dee_uint8_t),(Dee_uint8_t)def->d_offset); break;
  case DeeMemberType_CONST|DeeMemberType_ATOMIC|DeeMemberType_uint16_t: result = DeeObject_New(atomic(Dee_uint16_t),(Dee_uint16_t)def->d_offset); break;
  case DeeMemberType_CONST|DeeMemberType_ATOMIC|DeeMemberType_uint32_t: result = DeeObject_New(atomic(Dee_uint32_t),(Dee_uint32_t)def->d_offset); break;
  case DeeMemberType_CONST|DeeMemberType_ATOMIC|DeeMemberType_uint64_t: result = DeeObject_New(atomic(Dee_uint64_t),(Dee_uint64_t)def->d_offset); break;
  case DeeMemberType_CONST|DeeMemberType_object:
  case DeeMemberType_CONST|DeeMemberType_weak_object:
   Dee_INCREF(result = (DeeObject *)def->d_offset);
   break;
  case DeeMemberType_CONST|DeeMemberType_object_null:
  case DeeMemberType_CONST|DeeMemberType_weak_object_null:
   Dee_INCREF(result = def->d_offset ? (DeeObject *)def->d_offset : Dee_None);
   break;
  case DeeMemberType_CONST|DeeMemberType_widechar:
  case DeeMemberType_CONST|DeeMemberType_ATOMIC|DeeMemberType_widechar:
#if DEE_CONFIG_HAVE_ENCODING_WIDE
   result = DeeWideString_NewSized(1);
   if (result) *DeeWideString_STR(result) = (Dee_WideChar)def->d_offset;
#else
   Dee_INCREF(result = DeeString_Character((char)(Dee_WideChar)def->d_offset));
#endif
   break;
  case DeeMemberType_CONST|DeeMemberType_utf8char:
  case DeeMemberType_CONST|DeeMemberType_ATOMIC|DeeMemberType_utf8char:
#if DEE_CONFIG_HAVE_ENCODING_UTF8
   Dee_INCREF(result = DeeUtf8String_Character((Dee_Utf8Char)def->d_offset));
#else
   Dee_INCREF(result = DeeString_Character((char)(Dee_Utf8Char)def->d_offset));
#endif
   break;
  case DeeMemberType_CONST|DeeMemberType_utf16char:
  case DeeMemberType_CONST|DeeMemberType_ATOMIC|DeeMemberType_utf16char:
#if DEE_CONFIG_HAVE_ENCODING_UTF16
   result = DeeUtf16String_NewSized(1);
   if (result) *DeeUtf16String_STR(result) = (Dee_Utf16Char)def->d_offset;
#else
   Dee_INCREF(result = DeeString_Character((char)(Dee_Utf16Char)def->d_offset));
#endif
   break;
  case DeeMemberType_CONST|DeeMemberType_utf32char:
  case DeeMemberType_CONST|DeeMemberType_ATOMIC|DeeMemberType_utf32char:
#if DEE_CONFIG_HAVE_ENCODING_UTF32
   result = DeeUtf32String_NewSized(1);
   if (result) *DeeUtf32String_STR(result) = (Dee_Utf32Char)def->d_offset;
#else
   Dee_INCREF(result = DeeString_Character((char)(Dee_Utf32Char)def->d_offset));
#endif
   break;

  case DeeMemberType_CONST|DeeMemberType_ldouble:
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
   result = DeeObject_New(ldouble,(long double)def->d_offset); break;
#endif
  case DeeMemberType_CONST|DeeMemberType_double:
#ifdef DEE_TYPES_SIZEOF_DOUBLE
   result = DeeObject_New(double,(double)def->d_offset); break;
#endif
  case DeeMemberType_CONST|DeeMemberType_float:
#ifdef DEE_TYPES_SIZEOF_FLOAT
   result = DeeObject_New(float,(float)def->d_offset); break;
#endif
  case DeeMemberType_CONST|DeeMemberType_none:
  case DeeMemberType_none: result = DeeNone_New(); break;
  case DeeMemberType_int8_t: result = DeeObject_New(Dee_int8_t,*(Dee_int8_t *)base_ob); break;
  case DeeMemberType_int16_t: result = DeeObject_New(Dee_int16_t,*(Dee_int16_t *)base_ob); break;
  case DeeMemberType_int32_t: result = DeeObject_New(Dee_int32_t,*(Dee_int32_t *)base_ob); break;
  case DeeMemberType_int64_t: result = DeeObject_New(Dee_int64_t,*(Dee_int64_t *)base_ob); break;
  case DeeMemberType_uint8_t: result = DeeObject_New(Dee_uint8_t,*(Dee_uint8_t *)base_ob); break;
  case DeeMemberType_uint16_t: result = DeeObject_New(Dee_uint16_t,*(Dee_uint16_t *)base_ob); break;
  case DeeMemberType_uint32_t: result = DeeObject_New(Dee_uint32_t,*(Dee_uint32_t *)base_ob); break;
  case DeeMemberType_uint64_t: result = DeeObject_New(Dee_uint64_t,*(Dee_uint64_t *)base_ob); break;
  case DeeMemberType_ATOMIC|DeeMemberType_int8_t: result = DeeObject_New(Dee_int8_t,(Dee_int8_t)DeeAtomic8_Load(*(Dee_int8_t *)base_ob,memory_order_seq_cst)); break;
  case DeeMemberType_ATOMIC|DeeMemberType_int16_t: result = DeeObject_New(Dee_int16_t,(Dee_int16_t)DeeAtomic16_Load(*(Dee_int16_t *)base_ob,memory_order_seq_cst)); break;
  case DeeMemberType_ATOMIC|DeeMemberType_int32_t: result = DeeObject_New(Dee_int32_t,(Dee_int32_t)DeeAtomic32_Load(*(Dee_int32_t *)base_ob,memory_order_seq_cst)); break;
  case DeeMemberType_ATOMIC|DeeMemberType_int64_t: result = DeeObject_New(Dee_int64_t,(Dee_int64_t)DeeAtomic64_Load(*(Dee_int64_t *)base_ob,memory_order_seq_cst)); break;
  case DeeMemberType_ATOMIC|DeeMemberType_uint8_t: result = DeeObject_New(Dee_uint8_t,(Dee_uint8_t)DeeAtomic8_Load(*(Dee_uint8_t *)base_ob,memory_order_seq_cst)); break;
  case DeeMemberType_ATOMIC|DeeMemberType_uint16_t: result = DeeObject_New(Dee_uint16_t,(Dee_uint16_t)DeeAtomic16_Load(*(Dee_uint16_t *)base_ob,memory_order_seq_cst)); break;
  case DeeMemberType_ATOMIC|DeeMemberType_uint32_t: result = DeeObject_New(Dee_uint32_t,(Dee_uint32_t)DeeAtomic32_Load(*(Dee_uint32_t *)base_ob,memory_order_seq_cst)); break;
  case DeeMemberType_ATOMIC|DeeMemberType_uint64_t: result = DeeObject_New(Dee_uint64_t,(Dee_uint64_t)DeeAtomic64_Load(*(Dee_uint64_t *)base_ob,memory_order_seq_cst)); break;
  case DeeMemberType_ldouble:
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
   result = DeeObject_New(ldouble,*(long double *)base_ob); break;
#endif
  case DeeMemberType_double:
#ifdef DEE_TYPES_SIZEOF_DOUBLE
   result = DeeObject_New(double,*(double *)base_ob); break;
#endif
  case DeeMemberType_float:
#ifdef DEE_TYPES_SIZEOF_FLOAT
   result = DeeObject_New(float,*(float *)base_ob); break;
#endif
#if !defined(DEE_TYPES_SIZEOF_LDOUBLE) && \
    !defined(DEE_TYPES_SIZEOF_DOUBLE) && \
    !defined(DEE_TYPES_SIZEOF_FLOAT)
   result = DeeNone_New(); break;
#endif


  case DeeMemberType_widechar:
#if DEE_CONFIG_HAVE_ENCODING_WIDE
   result = DeeWideString_NewSized(1);
   if (result) *DeeWideString_STR(result) = *(Dee_WideChar *)base_ob;
#else
   Dee_INCREF(result = DeeString_Character((char)*(Dee_WideChar *)base_ob));
#endif
   break;
  case DeeMemberType_ATOMIC|DeeMemberType_widechar:
#if DEE_CONFIG_HAVE_ENCODING_WIDE
   result = DeeWideString_NewSized(1);
   if (result) *DeeWideString_STR(result) = (Dee_WideChar)DeeAtomicN_Load(
    DEE_TYPES_SIZEOF_WCHAR_T,*(Dee_WideChar *)base_ob,memory_order_seq_cst);
#else
   Dee_INCREF(result = DeeString_Character((char)DeeAtomicN_Load(
    DEE_TYPES_SIZEOF_WCHAR_T,*(Dee_WideChar *)base_ob,memory_order_seq_cst)));
#endif
   break;
  case DeeMemberType_utf8char:
#if DEE_CONFIG_HAVE_ENCODING_UTF8
   Dee_INCREF(result = DeeUtf8String_Character(*(Dee_Utf8Char *)base_ob));
#else
   Dee_INCREF(result = DeeString_Character(*(char *)base_ob));
#endif
   break;
  case DeeMemberType_ATOMIC|DeeMemberType_utf8char:
#if DEE_CONFIG_HAVE_ENCODING_UTF8
   Dee_INCREF(result = DeeUtf8String_Character((Dee_Utf8Char)DeeAtomicChar_Load(*(Dee_Utf8Char *)base_ob,memory_order_seq_cst)));
#else
   Dee_INCREF(result = DeeString_Character((char)DeeAtomicChar_Load(*(char *)base_ob,memory_order_seq_cst)));
#endif
   break;
  case DeeMemberType_utf16char:
#if DEE_CONFIG_HAVE_ENCODING_WIDE
   result = DeeUtf16String_NewSized(1);
   if (result) *DeeUtf16String_STR(result) = *(Dee_Utf16Char *)base_ob;
#else
   Dee_INCREF(result = DeeString_Character((char)*(Dee_Utf16Char *)base_ob));
#endif
   break;
  case DeeMemberType_ATOMIC|DeeMemberType_utf16char:
#if DEE_CONFIG_HAVE_ENCODING_WIDE
   result = DeeUtf16String_NewSized(1);
   if (result) *DeeUtf16String_STR(result) = (Dee_Utf16Char)DeeAtomicN_Load(
    DEE_TYPES_SIZEOF_CHAR16_T,*(Dee_Utf16Char *)base_ob,memory_order_seq_cst);
#else
   Dee_INCREF(result = DeeString_Character((char)DeeAtomicN_Load(
    DEE_TYPES_SIZEOF_CHAR16_T,*(Dee_Utf16Char *)base_ob,memory_order_seq_cst)));
#endif
   break;
  case DeeMemberType_utf32char:
#if DEE_CONFIG_HAVE_ENCODING_WIDE
   result = DeeUtf32String_NewSized(1);
   if (result) *DeeUtf32String_STR(result) = *(Dee_Utf32Char *)base_ob;
#else
   Dee_INCREF(result = DeeString_Character((char)*(Dee_Utf32Char *)base_ob));
#endif
   break;
  case DeeMemberType_ATOMIC|DeeMemberType_utf32char:
#if DEE_CONFIG_HAVE_ENCODING_WIDE
   result = DeeUtf32String_NewSized(1);
   if (result) *DeeUtf32String_STR(result) = (Dee_Utf32Char)DeeAtomicN_Load(
    DEE_TYPES_SIZEOF_CHAR32_T,*(Dee_Utf32Char *)base_ob,memory_order_seq_cst);
#else
   Dee_INCREF(result = DeeString_Character((char)DeeAtomicN_Load(
    DEE_TYPES_SIZEOF_CHAR32_T,*(Dee_Utf32Char *)base_ob,memory_order_seq_cst)));
#endif
   break;
  case DeeMemberType_widestring: {
   Dee_WideChar *temp = *(Dee_WideChar **)base_ob;
#if DEE_CONFIG_HAVE_ENCODING_UTF8
   if (temp) result = DeeWideString_New(temp);
   else Dee_INCREF(result = Dee_EmptyWideString);
#else
   if (temp) result = DeeString_FromWideString(temp);
   else Dee_INCREF(result = Dee_EmptyString);
#endif
  } break;
  case DeeMemberType_ATOMIC|DeeMemberType_widestring: {
   Dee_WideChar *temp = (Dee_WideChar *)DeeAtomicPtr_Load(*(void **)base_ob,memory_order_seq_cst);
#if DEE_CONFIG_HAVE_ENCODING_UTF8
   if (temp) result = DeeWideString_New(temp);
   else Dee_INCREF(result = Dee_EmptyWideString);
#else
   if (temp) result = DeeString_FromWideString(temp);
   else Dee_INCREF(result = Dee_EmptyString);
#endif
  } break;
  case DeeMemberType_inplace_widestring:
#if DEE_CONFIG_HAVE_ENCODING_UTF8
   result = DeeWideString_New((Dee_WideChar *)base_ob);
#else
   result = DeeString_FromWideString((Dee_WideChar *)base_ob);
#endif
   break;
  case DeeMemberType_utf8string: {
   Dee_Utf8Char *temp = *(Dee_Utf8Char **)base_ob;
#if DEE_CONFIG_HAVE_ENCODING_UTF8
   if (temp) result = DeeUtf8String_New(temp);
   else Dee_INCREF(result = Dee_EmptyUtf8String);
#else
   if (temp) result = DeeString_FromUtf8String(temp);
   else Dee_INCREF(result = Dee_EmptyString);
#endif
  } break;
  case DeeMemberType_ATOMIC|DeeMemberType_utf8string: {
   Dee_Utf8Char *temp = (Dee_Utf8Char *)DeeAtomicPtr_Load(*(void **)base_ob,memory_order_seq_cst);
#if DEE_CONFIG_HAVE_ENCODING_UTF8
   if (temp) result = DeeUtf8String_New(temp);
   else Dee_INCREF(result = Dee_EmptyUtf8String);
#else
   if (temp) result = DeeString_FromUtf8String(temp);
   else Dee_INCREF(result = Dee_EmptyString);
#endif
  } break;
  case DeeMemberType_inplace_utf8string:
#if DEE_CONFIG_HAVE_ENCODING_UTF8
   result = DeeUtf8String_New((Dee_Utf8Char *)base_ob);
#else
   result = DeeString_FromUtf8String((Dee_Utf8Char *)base_ob);
#endif
   break;
  case DeeMemberType_utf16string: {
   Dee_Utf16Char *temp = *(Dee_Utf16Char **)base_ob;
#if DEE_CONFIG_HAVE_ENCODING_UTF16
   if (temp) result = DeeUtf16String_New(temp);
   else Dee_INCREF(result = Dee_EmptyUtf16String);
#else
   if (temp) result = DeeString_FromUtf16String(temp);
   else Dee_INCREF(result = Dee_EmptyString);
#endif
  } break;
  case DeeMemberType_ATOMIC|DeeMemberType_utf16string: {
   Dee_Utf16Char *temp = (Dee_Utf16Char *)DeeAtomicPtr_Load(*(void **)base_ob,memory_order_seq_cst);
#if DEE_CONFIG_HAVE_ENCODING_UTF16
   if (temp) result = DeeUtf16String_New(temp);
   else Dee_INCREF(result = Dee_EmptyUtf16String);
#else
   if (temp) result = DeeString_FromUtf16String(temp);
   else Dee_INCREF(result = Dee_EmptyString);
#endif
  } break;
  case DeeMemberType_inplace_utf16string:
#if DEE_CONFIG_HAVE_ENCODING_UTF16
   result = DeeUtf16String_New((Dee_Utf16Char *)base_ob);
#else
   result = DeeString_FromUtf16String((Dee_Utf16Char *)base_ob);
#endif
   break;
  case DeeMemberType_utf32string: {
   Dee_Utf32Char *temp = *(Dee_Utf32Char **)base_ob;
#if DEE_CONFIG_HAVE_ENCODING_UTF32
   if (temp) result = DeeUtf32String_New(temp);
   else Dee_INCREF(result = Dee_EmptyUtf32String);
#else
   if (temp) result = DeeString_FromUtf32String(temp);
   else Dee_INCREF(result = Dee_EmptyString);
#endif
  } break;
  case DeeMemberType_ATOMIC|DeeMemberType_utf32string: {
   Dee_Utf32Char *temp = (Dee_Utf32Char *)DeeAtomicPtr_Load(*(void **)base_ob,memory_order_seq_cst);
#if DEE_CONFIG_HAVE_ENCODING_UTF32
   if (temp) result = DeeUtf32String_New(temp);
   else Dee_INCREF(result = Dee_EmptyUtf32String);
#else
   if (temp) result = DeeString_FromUtf32String(temp);
   else Dee_INCREF(result = Dee_EmptyString);
#endif
  } break;
  case DeeMemberType_inplace_utf32string:
#if DEE_CONFIG_HAVE_ENCODING_UTF32
   result = DeeUtf32String_New((Dee_Utf32Char *)base_ob);
#else
   result = DeeString_FromUtf32String((Dee_Utf32Char *)base_ob);
#endif
   break;

  case DeeMemberType_object:
   result = *(DeeObject **)base_ob;
   if (result) Dee_INCREF(result); else DeeError_SetStringf(
    &DeeErrorType_AttributeError,"Missing attribute: %s",def->d_name);
   break;
  case DeeMemberType_weak_object:
   result = *(DeeObject **)base_ob;
wref2:
   if (result) {
wref:
    if ((result = DeeObject_LockWeakref(result)) == NULL)
     DeeError_Throw((DeeObject *)&_DeeError_DeadWeakReference);
   } else DeeError_SetStringf(&DeeErrorType_AttributeError,"Missing attribute: %s",def->d_name);
   break;
  case DeeMemberType_ATOMIC|DeeMemberType_weak_object:
   result = (DeeObject *)DeeAtomicPtr_Load(*(void **)base_ob,memory_order_seq_cst);
   goto wref2;
  case DeeMemberType_ATOMIC|DeeMemberType_object:
   result = (DeeObject *)DeeAtomicPtr_Load(*(void **)base_ob,memory_order_seq_cst);
   if (result) Dee_INCREF(result); else DeeError_SetStringf(
    &DeeErrorType_AttributeError,"Missing attribute: %s",def->d_name);
   break;
  case DeeMemberType_object_null:
   result = *(DeeObject **)base_ob;
   if (!result) result = Dee_None;
   Dee_INCREF(result);
   break;
  case DeeMemberType_ATOMIC|DeeMemberType_object_null:
   result = (DeeObject *)DeeAtomicPtr_Load(*(void **)base_ob,memory_order_seq_cst);
   if (!result) result = Dee_None;
   Dee_INCREF(result);
   break;
  case DeeMemberType_weak_object_null:
   result = *(DeeObject **)base_ob;
wref_n:
   if (result) goto wref;
   result = Dee_None;
   Dee_INCREF(result);
   break;
  case DeeMemberType_ATOMIC|DeeMemberType_weak_object_null:
   result = (DeeObject *)DeeAtomicPtr_Load(*(void **)base_ob,memory_order_seq_cst);
   goto wref_n;
  default: {
   DeeTypeObject *ptr_type;
   DEE_ASSERTF((def->d_type & DeeMemberType_POINTER_MASK) != 0,"Expected pointer type");
   ptr_type = _DeeMember_GetBasicType((def->d_type&~DeeMemberType_ATOMIC)-DeeMemberType_POINTER_X1);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   if (ptr_type) ptr_type = DeeType_Pointer(ptr_type);
   if (!ptr_type) return NULL;
   if ((def->d_type&DeeMemberType_ATOMIC)!=0) {
    return DeePointer_New(ptr_type,DeeAtomicPtr_Load(
     *(void **)base_ob,memory_order_seq_cst));
   } else {
    return DeePointer_New(ptr_type,*(void **)base_ob);
   }
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   DeeError_NotImplemented_str("DEE_CONFIG_RUNTIME_HAVE_POINTERS");
   return NULL;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  } break;
 }
 return result;
}

#ifdef DEE_TYPES_SIZEOF_FLOAT
DEE_STATIC_ASSERT(DEE_TYPES_SIZEOF_FLOAT == sizeof(float));
#endif
#ifdef DEE_TYPES_SIZEOF_DOUBLE
DEE_STATIC_ASSERT(DEE_TYPES_SIZEOF_DOUBLE == sizeof(double));
#endif
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
DEE_STATIC_ASSERT(DEE_TYPES_SIZEOF_LDOUBLE == sizeof(long double));
#endif


DEE_A_RET_EXCEPT(-1) int DeeMember_TSet(
 DEE_A_IN DeeObject *base_ob, DEE_A_IN struct DeeMemberDef const *def,
 DEE_A_IN DeeTypeObject const *tp_v, DEE_A_INOUT DeeObject *v) {
 DEE_ASSERT(DeeObject_Check(v));
 DEE_ASSERT(DeeObject_Check(tp_v) && DeeType_Check(tp_v));
 DEE_ASSERT(DeeObject_InstanceOf(v,tp_v));
 DEE_ASSERT(base_ob && def);
 *(Dee_uintptr_t *)&base_ob += def->d_offset;
 switch (def->d_type) {
#define DEF_VALUE(T,get)\
 {T temp;\
  if (get(tp_v,v,&temp) != 0) return -1;\
  *(T *)base_ob = temp; }
#define DEF_ATOMIC_VALUE(T,get,store,atomic_t)\
 {T temp;\
  if (get(tp_v,v,&temp) != 0) return -1;\
  store(*(atomic_t *)base_ob,*(atomic_t *)&temp,memory_order_seq_cst);}
  case DeeMemberType_int8_t: DEF_VALUE(Dee_int8_t,DeeObject_Cast_F(Dee_int8_t)) break;
  case DeeMemberType_int16_t: DEF_VALUE(Dee_int16_t,DeeObject_Cast_F(Dee_int16_t)) break;
  case DeeMemberType_int32_t: DEF_VALUE(Dee_int32_t,DeeObject_Cast_F(Dee_int32_t)) break;
  case DeeMemberType_int64_t: DEF_VALUE(Dee_int64_t,DeeObject_Cast_F(Dee_int64_t)) break;
  case DeeMemberType_uint8_t: DEF_VALUE(Dee_uint8_t,DeeObject_Cast_F(Dee_uint8_t)) break;
  case DeeMemberType_uint16_t: DEF_VALUE(Dee_uint16_t,DeeObject_Cast_F(Dee_uint16_t)) break;
  case DeeMemberType_uint32_t: DEF_VALUE(Dee_uint32_t,DeeObject_Cast_F(Dee_uint32_t)) break;
  case DeeMemberType_uint64_t: DEF_VALUE(Dee_uint64_t,DeeObject_Cast_F(Dee_uint64_t)) break;
  case DeeMemberType_float: DEF_VALUE(float,DeeObject_Cast_F(float)) break;
  case DeeMemberType_double: DEF_VALUE(double,DeeObject_Cast_F(double)) break;
  case DeeMemberType_ldouble: DEF_VALUE(long double,DeeObject_Cast_F(ldouble)) break;
  case DeeMemberType_ATOMIC|DeeMemberType_int8_t: DEF_ATOMIC_VALUE(Dee_int8_t,DeeObject_Cast_F(Dee_int8_t),DeeAtomic8_Store,Dee_int8_t) break;
  case DeeMemberType_ATOMIC|DeeMemberType_int16_t: DEF_ATOMIC_VALUE(Dee_int16_t,DeeObject_Cast_F(Dee_int16_t),DeeAtomic16_Store,Dee_int16_t) break;
  case DeeMemberType_ATOMIC|DeeMemberType_int32_t: DEF_ATOMIC_VALUE(Dee_int32_t,DeeObject_Cast_F(Dee_int32_t),DeeAtomic32_Store,Dee_int32_t) break;
  case DeeMemberType_ATOMIC|DeeMemberType_int64_t: DEF_ATOMIC_VALUE(Dee_int64_t,DeeObject_Cast_F(Dee_int64_t),DeeAtomic64_Store,Dee_int64_t) break;
  case DeeMemberType_ATOMIC|DeeMemberType_uint8_t: DEF_ATOMIC_VALUE(Dee_uint8_t,DeeObject_Cast_F(Dee_uint8_t),DeeAtomic8_Store,Dee_uint8_t) break;
  case DeeMemberType_ATOMIC|DeeMemberType_uint16_t: DEF_ATOMIC_VALUE(Dee_uint16_t,DeeObject_Cast_F(Dee_uint16_t),DeeAtomic16_Store,Dee_uint16_t) break;
  case DeeMemberType_ATOMIC|DeeMemberType_uint32_t: DEF_ATOMIC_VALUE(Dee_uint32_t,DeeObject_Cast_F(Dee_uint32_t),DeeAtomic32_Store,Dee_uint32_t) break;
  case DeeMemberType_ATOMIC|DeeMemberType_uint64_t: DEF_ATOMIC_VALUE(Dee_uint64_t,DeeObject_Cast_F(Dee_uint64_t),DeeAtomic64_Store,Dee_uint64_t) break;
  case DeeMemberType_widechar: DEF_VALUE(Dee_WideChar,DeeObject_Cast_F(Dee_WideChar)) break;
  case DeeMemberType_utf8char: DEF_VALUE(Dee_Utf8Char,DeeObject_Cast_F(Dee_Utf8Char)) break;
  case DeeMemberType_utf16char: DEF_VALUE(Dee_Utf16Char,DeeObject_Cast_F(Dee_Utf16Char)) break;
  case DeeMemberType_utf32char: DEF_VALUE(Dee_Utf32Char,DeeObject_Cast_F(Dee_Utf32Char)) break;
  case DeeMemberType_ATOMIC|DeeMemberType_widechar: DEF_ATOMIC_VALUE(Dee_WideChar,DeeObject_Cast_F(Dee_WideChar),DeeAtomicN_Store_F(DEE_TYPES_SIZEOF_WCHAR_T),DEE_TYPES_UINT(DEE_TYPES_SIZEOF_WCHAR_T)) break;
  case DeeMemberType_ATOMIC|DeeMemberType_utf8char: DEF_ATOMIC_VALUE(Dee_Utf8Char,DeeObject_Cast_F(Dee_Utf8Char),DeeAtomicN_Store_F(DEE_TYPES_SIZEOF_CHAR),DEE_TYPES_UINT(DEE_TYPES_SIZEOF_CHAR)) break;
  case DeeMemberType_ATOMIC|DeeMemberType_utf16char: DEF_ATOMIC_VALUE(Dee_Utf16Char,DeeObject_Cast_F(Dee_Utf16Char),DeeAtomicN_Store_F(DEE_TYPES_SIZEOF_CHAR16_T),DEE_TYPES_UINT(DEE_TYPES_SIZEOF_CHAR16_T)) break;
  case DeeMemberType_ATOMIC|DeeMemberType_utf32char: DEF_ATOMIC_VALUE(Dee_Utf32Char,DeeObject_Cast_F(Dee_Utf32Char),DeeAtomicN_Store_F(DEE_TYPES_SIZEOF_CHAR32_T),DEE_TYPES_UINT(DEE_TYPES_SIZEOF_CHAR32_T)) break;

#if defined(DEE_TYPES_SIZEOF_FLOAT) && DEE_TYPES_SIZEOF_FLOAT <= 8
  case DeeMemberType_ATOMIC|DeeMemberType_float:
   DEF_ATOMIC_VALUE(float,DeeObject_Cast_F(float),
                    DEE_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(DEE_TYPES_SIZEOF_FLOAT),_Store),
                    DEE_TYPES_UINT(DEE_TYPES_SIZEOF_FLOAT))
                    break;
#endif
#if defined(DEE_TYPES_SIZEOF_DOUBLE) && DEE_TYPES_SIZEOF_DOUBLE <= 8
  case DeeMemberType_ATOMIC|DeeMemberType_double:
   DEF_ATOMIC_VALUE(double,DeeObject_Cast_F(double),
                    DEE_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(DEE_TYPES_SIZEOF_DOUBLE),_Store),
                    DEE_TYPES_UINT(DEE_TYPES_SIZEOF_DOUBLE))
                    break;
#endif
#if defined(DEE_TYPES_SIZEOF_LDOUBLE) && DEE_TYPES_SIZEOF_LDOUBLE <= 8
  case DeeMemberType_ATOMIC|DeeMemberType_ldouble:
   DEF_ATOMIC_VALUE(long double,DeeObject_Cast_F(ldouble),
                    DEE_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(DEE_TYPES_SIZEOF_LDOUBLE),_Store),
                    DEE_TYPES_UINT(DEE_TYPES_SIZEOF_LDOUBLE))
                    break;
#endif
#undef DEF_ATOMIC_VALUE
#undef DEF_VALUE
  case DeeMemberType_object: Dee_DECREF(*((DeeObject **)base_ob)); goto obn;
  case DeeMemberType_object_null: Dee_XDECREF(*((DeeObject **)base_ob));
obn:
   Dee_INCREF(*((DeeObject **)base_ob) = v);
   break;
  case DeeMemberType_ATOMIC|DeeMemberType_object:
   Dee_INCREF(v);
   v = (DeeObject *)DeeAtomicPtr_Exchange(
    *((void **)base_ob),v,memory_order_seq_cst);
   Dee_DECREF(v);
   break;
  case DeeMemberType_ATOMIC|DeeMemberType_object_null:
   Dee_INCREF(v);
   v = (DeeObject *)DeeAtomicPtr_Exchange(
    *((void **)base_ob),v,memory_order_seq_cst);
   Dee_XDECREF(v);
   break;
  case DeeMemberType_weak_object: Dee_WEAKDECREF(*((DeeObject **)base_ob)); goto wobn;
  case DeeMemberType_weak_object_null: Dee_WEAKXDECREF(*((DeeObject **)base_ob));
wobn:
   Dee_WEAKINCREF(*((DeeObject **)base_ob) = v);
   break;
  case DeeMemberType_ATOMIC|DeeMemberType_weak_object:
   Dee_WEAKINCREF(v);
   v = (DeeObject *)DeeAtomicPtr_Exchange(
    *((void **)base_ob),v,memory_order_seq_cst);
   Dee_WEAKDECREF(v);
   break;
  case DeeMemberType_ATOMIC|DeeMemberType_weak_object_null:
   Dee_WEAKINCREF(v);
   v = (DeeObject *)DeeAtomicPtr_Exchange(
    *((void **)base_ob),v,memory_order_seq_cst);
   Dee_WEAKXDECREF(v);
   break;

  default: {
   DeeTypeObject *ptr_base_tp;
   DEE_ASSERT((def->d_type & DeeMemberType_POINTER_MASK) != 0 && "Expected pointer type");
   if ((ptr_base_tp = _DeeMember_GetBasicType(def->d_type-DeeMemberType_POINTER_X1)) == NULL) return -1;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   {
    void *p;
    if (DeeObject_TGetPointerEx(tp_v,v,ptr_base_tp,&p) != 0) return -1;
    if ((def->d_type&DeeMemberType_ATOMIC)!=0) {
     DeeAtomicPtr_Store(*(void **)base_ob,p,memory_order_seq_cst);
    } else {
     *(void **)base_ob = p;
    }
   }
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   DeeError_NotImplemented_str("DEE_CONFIG_RUNTIME_HAVE_POINTERS");
   return -1;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  } break;

 }
 return 0;
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_MEMBERDEF_C */
