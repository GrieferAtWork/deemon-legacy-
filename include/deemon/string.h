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
#ifndef GUARD_DEEMON_STRING_H
#define GUARD_DEEMON_STRING_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/string_forward.h>
#include <deemon/__ctypes.inl>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/object_refcnt.h>
#include <deemon/optional/std/stdarg.h>

#ifdef DEE_LIMITED_DEX
#include <deemon/object.h>
#endif
#ifndef __INTELLISENSE__
#include <deemon/optional/std/string.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif


DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) Dee_WideStrLen(
 DEE_A_IN_Z Dee_WideChar const *str_) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) Dee_Utf8StrLen(
 DEE_A_IN_Z Dee_Utf8Char const *str_) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) Dee_Utf16StrLen(
 DEE_A_IN_Z Dee_Utf16Char const *str_) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) Dee_Utf32StrLen(
 DEE_A_IN_Z Dee_Utf32Char const *str_) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) Dee_WideStrNLen(
 DEE_A_IN_R(n) Dee_WideChar const *str_, DEE_A_IN Dee_size_t n) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) Dee_Utf8StrNLen(
 DEE_A_IN_R(n) Dee_Utf8Char const *str_, DEE_A_IN Dee_size_t n) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) Dee_Utf16StrNLen(
 DEE_A_IN_R(n) Dee_Utf16Char const *str_, DEE_A_IN Dee_size_t n) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) Dee_Utf32StrNLen(
 DEE_A_IN_R(n) Dee_Utf32Char const *str_, DEE_A_IN Dee_size_t n) DEE_ATTRIBUTE_NONNULL((1));

DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_WideStrCmp(
 DEE_A_IN_Z Dee_WideChar const *a, DEE_A_IN_Z Dee_WideChar const *b) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf8StrCmp(
 DEE_A_IN_Z Dee_Utf8Char const *a, DEE_A_IN_Z Dee_Utf8Char const *b) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf16StrCmp(
 DEE_A_IN_Z Dee_Utf16Char const *a, DEE_A_IN_Z Dee_Utf16Char const *b) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf32StrCmp(
 DEE_A_IN_Z Dee_Utf32Char const *a, DEE_A_IN_Z Dee_Utf32Char const *b) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_WideStrCaseCmp(
 DEE_A_IN_Z Dee_WideChar const *a, DEE_A_IN_Z Dee_WideChar const *b) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf8StrCaseCmp(
 DEE_A_IN_Z Dee_Utf8Char const *a, DEE_A_IN_Z Dee_Utf8Char const *b) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf16StrCaseCmp(
 DEE_A_IN_Z Dee_Utf16Char const *a, DEE_A_IN_Z Dee_Utf16Char const *b) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf32StrCaseCmp(
 DEE_A_IN_Z Dee_Utf32Char const *a, DEE_A_IN_Z Dee_Utf32Char const *b) DEE_ATTRIBUTE_NONNULL((1,2));

#define Dee_WideStrNCmp(a,b,s)  memcmp(a,b,s*sizeof(Dee_WideChar))
#define Dee_Utf8StrNCmp(a,b,s)  memcmp(a,b,s*sizeof(Dee_Utf8Char))
#define Dee_Utf16StrNCmp(a,b,s) memcmp(a,b,s*sizeof(Dee_Utf16Char))
#define Dee_Utf32StrNCmp(a,b,s) memcmp(a,b,s*sizeof(Dee_Utf32Char))

DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_WideStrCmpLen(
 DEE_A_IN Dee_size_t a_size, DEE_A_IN_R(a_size) Dee_WideChar const *a,
 DEE_A_IN Dee_size_t b_size, DEE_A_IN_R(b_size) Dee_WideChar const *b);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf8StrCmpLen(
 DEE_A_IN Dee_size_t a_size, DEE_A_IN_R(a_size) Dee_Utf8Char const *a,
 DEE_A_IN Dee_size_t b_size, DEE_A_IN_R(b_size) Dee_Utf8Char const *b);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf16StrCmpLen(
 DEE_A_IN Dee_size_t a_size, DEE_A_IN_R(a_size) Dee_Utf16Char const *a,
 DEE_A_IN Dee_size_t b_size, DEE_A_IN_R(b_size) Dee_Utf16Char const *b);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf32StrCmpLen(
 DEE_A_IN Dee_size_t a_size, DEE_A_IN_R(a_size) Dee_Utf32Char const *a,
 DEE_A_IN Dee_size_t b_size, DEE_A_IN_R(b_size) Dee_Utf32Char const *b);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_WideStrCaseCmpLen(
 DEE_A_IN Dee_size_t a_size, DEE_A_IN_R(a_size) Dee_WideChar const *a,
 DEE_A_IN Dee_size_t b_size, DEE_A_IN_R(b_size) Dee_WideChar const *b);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf8StrCaseCmpLen(
 DEE_A_IN Dee_size_t a_size, DEE_A_IN_R(a_size) Dee_Utf8Char const *a,
 DEE_A_IN Dee_size_t b_size, DEE_A_IN_R(b_size) Dee_Utf8Char const *b);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf16StrCaseCmpLen(
 DEE_A_IN Dee_size_t a_size, DEE_A_IN_R(a_size) Dee_Utf16Char const *a,
 DEE_A_IN Dee_size_t b_size, DEE_A_IN_R(b_size) Dee_Utf16Char const *b);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf32StrCaseCmpLen(
 DEE_A_IN Dee_size_t a_size, DEE_A_IN_R(a_size) Dee_Utf32Char const *a,
 DEE_A_IN Dee_size_t b_size, DEE_A_IN_R(b_size) Dee_Utf32Char const *b);

//////////////////////////////////////////////////////////////////////////
// Wildcard match functions
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_WideStrWMatch(DEE_A_IN_Z Dee_WideChar const *str_, DEE_A_IN_Z Dee_WideChar const *pat_) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf8StrWMatch(DEE_A_IN_Z Dee_Utf8Char const *str_, DEE_A_IN_Z Dee_Utf8Char const *pat_) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf16StrWMatch(DEE_A_IN_Z Dee_Utf16Char const *str_, DEE_A_IN_Z Dee_Utf16Char const *pat_) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf32StrWMatch(DEE_A_IN_Z Dee_Utf32Char const *str_, DEE_A_IN_Z Dee_Utf32Char const *pat_) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_WideStrCaseWMatch(DEE_A_IN_Z Dee_WideChar const *str_, DEE_A_IN_Z Dee_WideChar const *pat_) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf8StrCaseWMatch(DEE_A_IN_Z Dee_Utf8Char const *str_, DEE_A_IN_Z Dee_Utf8Char const *pat_) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf16StrCaseWMatch(DEE_A_IN_Z Dee_Utf16Char const *str_, DEE_A_IN_Z Dee_Utf16Char const *pat_) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf32StrCaseWMatch(DEE_A_IN_Z Dee_Utf32Char const *str_, DEE_A_IN_Z Dee_Utf32Char const *pat_) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_WideStrWMatchLen(DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) Dee_WideChar const *str_, DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) Dee_WideChar const *pat_);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf8StrWMatchLen(DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) Dee_Utf8Char const *str_, DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) Dee_Utf8Char const *pat_);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf16StrWMatchLen(DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) Dee_Utf16Char const *str_, DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) Dee_Utf16Char const *pat_);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf32StrWMatchLen(DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) Dee_Utf32Char const *str_, DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) Dee_Utf32Char const *pat_);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_WideStrCaseWMatchLen(DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) Dee_WideChar const *str_, DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) Dee_WideChar const *pat_);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf8StrCaseWMatchLen(DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) Dee_Utf8Char const *str_, DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) Dee_Utf8Char const *pat_);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf16StrCaseWMatchLen(DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) Dee_Utf16Char const *str_, DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) Dee_Utf16Char const *pat_);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_Utf32StrCaseWMatchLen(DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) Dee_Utf32Char const *str_, DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) Dee_Utf32Char const *pat_);


#define Dee_MemCmpLen(a_len,a,b_len,b) Dee_Utf8StrCmpLen(\
 a_len,(Dee_Utf8Char const *)Dee_REQUIRES_POINTER(a),\
 b_len,(Dee_Utf8Char const *)Dee_REQUIRES_POINTER(b))


#ifndef __INTELLISENSE__
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STRING_H
#ifdef _MSC_VER
#define Dee_WideStrCmp  wcscmp
#define Dee_WideStrLen  wcslen
#define Dee_WideStrNLen wcsnlen
#endif
#define Dee_Utf8StrCmp  strcmp
#define Dee_Utf8StrLen  strlen
#define Dee_Utf8StrNLen strnlen
#ifdef _MSC_VER
#pragma intrinsic(strcmp)
#pragma intrinsic(strlen)
#pragma intrinsic(wcscmp)
#pragma intrinsic(wcslen)
#endif
#endif
#endif


#if DEE_CONFIG_HAVE_ENCODING_WIDE
#define DeeObject_WideStr(ob)  DeeObject_TWideStr(Dee_TYPE(ob),ob)
#define DeeObject_WideRepr(ob) DeeObject_TWideRepr(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeObject_TWideStr(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeObject_TWideRepr(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeObject_PWideStr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeObject_PWideRepr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
#endif

#if DEE_CONFIG_HAVE_ENCODING_UTF8
#define DeeObject_Utf8Str(ob)  DeeObject_TUtf8Str(Dee_TYPE(ob),ob)
#define DeeObject_Utf8Repr(ob) DeeObject_TUtf8Repr(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeObject_TUtf8Str(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeObject_TUtf8Repr(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeObject_PUtf8Str(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeObject_PUtf8Repr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
#endif

#if DEE_CONFIG_HAVE_ENCODING_UTF16
#define DeeObject_Utf16Str(ob)  DeeObject_TUtf16Str(Dee_TYPE(ob),ob)
#define DeeObject_Utf16Repr(ob) DeeObject_TUtf16Repr(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeObject_TUtf16Str(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeObject_TUtf16Repr(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeObject_PUtf16Str(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeObject_PUtf16Repr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
#endif

#if DEE_CONFIG_HAVE_ENCODING_UTF32
#define DeeObject_Utf32Str(ob)  DeeObject_TUtf32Str(Dee_TYPE(ob),ob)
#define DeeObject_Utf32Repr(ob) DeeObject_TUtf32Repr(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeObject_TUtf32Str(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeObject_TUtf32Repr(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeObject_PUtf32Str(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeObject_PUtf32Repr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
#endif

#ifndef DEE_EMPTY_CHARS
#define DEE_EMPTY_CHARS_STR  "\x09\x0A\x0B\x0C\x0D\x20"
#define DEE_EMPTY_CHARS     {'\x09','\x0A','\x0B','\x0C','\x0D','\x20','\0'}
#endif


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// String
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifdef DEE_LIMITED_DEX
// NOTE: This array size has __NOTHING__ to do with the actual size of a string
#ifdef _PREFAST_
#define DEE_PRIVATE_STRING_DEBUG_ARRAY_SIZE 1
#else
#define DEE_PRIVATE_STRING_DEBUG_ARRAY_SIZE 8192
#endif
#if DEE_CONFIG_HAVE_ENCODING_WIDE
struct _DeeEmptyWideStringObject { DEE_OBJECT_HEAD Dee_size_t s_len; Dee_WideChar s_str[1]; };
struct DeeWideStringObject { DEE_OBJECT_HEAD Dee_size_t s_len; Dee_WideChar s_str[DEE_PRIVATE_STRING_DEBUG_ARRAY_SIZE]; };
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF8
struct _DeeEmptyUtf8StringObject { DEE_OBJECT_HEAD Dee_size_t s_len; Dee_Utf8Char s_str[1]; };
struct DeeUtf8StringObject { DEE_OBJECT_HEAD Dee_size_t s_len; Dee_Utf8Char s_str[DEE_PRIVATE_STRING_DEBUG_ARRAY_SIZE]; };
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
struct _DeeEmptyUtf16StringObject { DEE_OBJECT_HEAD Dee_size_t s_len; Dee_Utf16Char s_str[1]; };
struct DeeUtf16StringObject { DEE_OBJECT_HEAD Dee_size_t s_len; Dee_Utf16Char s_str[DEE_PRIVATE_STRING_DEBUG_ARRAY_SIZE]; };
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
struct _DeeEmptyUtf32StringObject { DEE_OBJECT_HEAD Dee_size_t s_len; Dee_Utf32Char s_str[1]; };
struct DeeUtf32StringObject { DEE_OBJECT_HEAD Dee_size_t s_len; Dee_Utf32Char s_str[DEE_PRIVATE_STRING_DEBUG_ARRAY_SIZE]; };
#endif
#undef DEE_PRIVATE_STRING_DEBUG_ARRAY_SIZE
#endif

#if DEE_CONFIG_HAVE_ENCODING_UTF8
// Required because of the static character table
struct _DeeUtf8StringCharacterObject { DEE_OBJECT_HEAD Dee_size_t s_len; Dee_Utf8Char s_str[2]; };
#endif


#if DEE_CONFIG_HAVE_ENCODING_WIDE
#ifdef DEE_LIMITED_DEX
#define DeeWideString_STR(ob)         ((Dee_WideChar*)((DeeWideStringObject*)Dee_REQUIRES_POINTER(ob))->s_str)
#define DeeWideString_SIZE(ob)        ((Dee_size_t)((DeeWideStringObject*)Dee_REQUIRES_POINTER(ob))->s_len)
#else
#define DeeWideString_STR(ob)         ((Dee_WideChar*)DeeWideString_Str((DeeObject*)Dee_REQUIRES_POINTER(ob)))
#define DeeWideString_SIZE(ob)        DeeWideString_Size((DeeObject*)Dee_REQUIRES_POINTER(ob))
#endif
#define DeeWideString_EMPTY           DeeWideString_CheckEmpty
#define DeeWideString_EQUALS(ob,str_) \
 ((DeeWideString_SIZE(ob)==(sizeof(str_)/sizeof(*str_))-1)&&\
   memcmp(DeeWideString_STR(ob),str_,sizeof(str_)-sizeof(*str_))==0)
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */

#if DEE_CONFIG_HAVE_ENCODING_UTF8
#ifdef DEE_LIMITED_DEX
#define DeeUtf8String_STR(ob)         ((Dee_Utf8Char*)((DeeUtf8StringObject*)Dee_REQUIRES_POINTER(ob))->s_str)
#define DeeUtf8String_SIZE(ob)        ((Dee_size_t)((DeeUtf8StringObject*)Dee_REQUIRES_POINTER(ob))->s_len)
#else
#define DeeUtf8String_STR(ob)         ((Dee_Utf8Char*)DeeUtf8String_Str((DeeObject*)Dee_REQUIRES_POINTER(ob)))
#define DeeUtf8String_SIZE(ob)        DeeUtf8String_Size((DeeObject*)Dee_REQUIRES_POINTER(ob))
#endif
#define DeeUtf8String_EMPTY           DeeUtf8String_CheckEmpty
#define DeeUtf8String_EQUALS(ob,str_) \
 ((DeeUtf8String_SIZE(ob)==(sizeof(str_)/sizeof(*str_))-1)&&\
   memcmp(DeeUtf8String_STR(ob),str_,sizeof(str_)-sizeof(*str_))==0)
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */

#if DEE_CONFIG_HAVE_ENCODING_UTF16
#ifdef DEE_LIMITED_DEX
#define DeeUtf16String_STR(ob)         ((Dee_Utf16Char*)((DeeUtf16StringObject*)Dee_REQUIRES_POINTER(ob))->s_str)
#define DeeUtf16String_SIZE(ob)        ((Dee_size_t)((DeeUtf16StringObject*)Dee_REQUIRES_POINTER(ob))->s_len)
#else
#define DeeUtf16String_STR(ob)         ((Dee_Utf16Char*)DeeUtf16String_Str((DeeObject*)Dee_REQUIRES_POINTER(ob)))
#define DeeUtf16String_SIZE(ob)        DeeUtf16String_Size((DeeObject*)Dee_REQUIRES_POINTER(ob))
#endif
#define DeeUtf16String_EMPTY           DeeUtf16String_CheckEmpty
#define DeeUtf16String_EQUALS(ob,str_) \
 ((DeeUtf16String_SIZE(ob)==(sizeof(str_)/sizeof(*str_))-1)&&\
   memcmp(DeeUtf16String_STR(ob),str_,sizeof(str_)-sizeof(*str_))==0)
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */

#if DEE_CONFIG_HAVE_ENCODING_UTF32
#ifdef DEE_LIMITED_DEX
#define DeeUtf32String_STR(ob)         ((Dee_Utf32Char*)((DeeUtf32StringObject*)Dee_REQUIRES_POINTER(ob))->s_str)
#define DeeUtf32String_SIZE(ob)        ((Dee_size_t)((DeeUtf32StringObject*)Dee_REQUIRES_POINTER(ob))->s_len)
#else
#define DeeUtf32String_STR(ob)         ((Dee_Utf32Char*)DeeUtf32String_Str((DeeObject*)Dee_REQUIRES_POINTER(ob)))
#define DeeUtf32String_SIZE(ob)        DeeUtf32String_Size((DeeObject*)Dee_REQUIRES_POINTER(ob))
#endif
#define DeeUtf32String_EMPTY           DeeUtf32String_CheckEmpty
#define DeeUtf32String_EQUALS(ob,str_) \
 ((DeeUtf32String_SIZE(ob)==(sizeof(str_)/sizeof(*str_))-1)&&\
   memcmp(DeeUtf32String_STR(ob),str_,sizeof(str_)-sizeof(*str_))==0)
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */


#ifdef DEE_LIMITED_DEX
#if DEE_CONFIG_HAVE_ENCODING_WIDE
struct DeeWideStringIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeWideStringObject *si_string; /*< [0..1] Keep-alive reference. */
 /*atomic*/Dee_WideChar const  *si_pos;    /*< [0..1] Current character pointer. */
 Dee_WideChar const            *si_end;    /*< [0..1] End character pointer. */
};
#define DeeWideStringIterator_STRING(ob) ((DeeObject *)((DeeWideStringIteratorObject*)Dee_REQUIRES_POINTER(ob))->si_string)
#define DeeWideStringIterator_POS(ob)    ((DeeWideStringIteratorObject*)Dee_REQUIRES_POINTER(ob))->si_pos
#define DeeWideStringIterator_END(ob)    ((DeeWideStringIteratorObject*)Dee_REQUIRES_POINTER(ob))->si_end
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */

#if DEE_CONFIG_HAVE_ENCODING_UTF8
struct DeeUtf8StringIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeUtf8StringObject *si_string; /*< [0..1] Keep-alive reference. */
 /*atomic*/Dee_Utf8Char const  *si_pos;    /*< [0..1] Current character pointer. */
 Dee_Utf8Char const            *si_end;    /*< [0..1] End character pointer. */
};
#define DeeUtf8StringIterator_STRING(ob) ((DeeObject *)((DeeUtf8StringIteratorObject*)Dee_REQUIRES_POINTER(ob))->si_string)
#define DeeUtf8StringIterator_POS(ob)    ((DeeUtf8StringIteratorObject*)Dee_REQUIRES_POINTER(ob))->si_pos
#define DeeUtf8StringIterator_END(ob)    ((DeeUtf8StringIteratorObject*)Dee_REQUIRES_POINTER(ob))->si_end
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */

#if DEE_CONFIG_HAVE_ENCODING_UTF16
struct DeeUtf16StringIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeUtf16StringObject *si_string; /*< [0..1] Keep-alive reference. */
 /*atomic*/Dee_Utf16Char const  *si_pos;    /*< [0..1] Current character pointer. */
 Dee_Utf16Char const            *si_end;    /*< [0..1] End character pointer. */
};
#define DeeUtf16StringIterator_STRING(ob) ((DeeObject *)((DeeUtf16StringIteratorObject*)Dee_REQUIRES_POINTER(ob))->si_string)
#define DeeUtf16StringIterator_POS(ob)    ((DeeUtf16StringIteratorObject*)Dee_REQUIRES_POINTER(ob))->si_pos
#define DeeUtf16StringIterator_END(ob)    ((DeeUtf16StringIteratorObject*)Dee_REQUIRES_POINTER(ob))->si_end
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */

#if DEE_CONFIG_HAVE_ENCODING_UTF32
struct DeeUtf32StringIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeUtf32StringObject *si_string; /*< [0..1] Keep-alive reference. */
 /*atomic*/Dee_Utf32Char const  *si_pos;    /*< [0..1] Current character pointer. */
 Dee_Utf32Char const            *si_end;    /*< [0..1] End character pointer. */
};
#define DeeUtf32StringIterator_STRING(ob) ((DeeObject *)((DeeUtf32StringIteratorObject*)Dee_REQUIRES_POINTER(ob))->si_string)
#define DeeUtf32StringIterator_POS(ob)    ((DeeUtf32StringIteratorObject*)Dee_REQUIRES_POINTER(ob))->si_pos
#define DeeUtf32StringIterator_END(ob)    ((DeeUtf32StringIteratorObject*)Dee_REQUIRES_POINTER(ob))->si_end
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
#endif /* DEE_LIMITED_DEX */


#ifdef DEE_LIMITED_DEX
#ifdef _MSC_VER
#pragma warning(disable: 4820)
#endif
#if DEE_CONFIG_HAVE_ENCODING_WIDE
#ifdef __INTELLISENSE__
#define DeeWideString_NEW_STATIC(name,value)\
static struct { DEE_OBJECT_HEAD Dee_WideChar s_str[sizeof(value)/sizeof(Dee_WideChar)]; }\
 name = {DEE_OBJECT_HEAD_INIT(0),value}
#define DeeWideString_NEW_STATIC_EX(name,size,...)\
static struct { DEE_OBJECT_HEAD Dee_WideChar s_str[size]; Dee_WideChar s_end; }\
 name = {DEE_OBJECT_HEAD_INIT(0),__VA_ARGS__,0}
#else
#define DeeWideString_NEW_STATIC(name,value)\
 DEE_COMPILER_MSVC_WARNING_PUSH(4640)\
 static struct {\
  DEE_OBJECT_HEAD Dee_size_t s_len;\
  Dee_WideChar s_str[sizeof(value)/sizeof(Dee_WideChar)];\
 } name = {DEE_OBJECT_HEAD_INIT(&DeeWideString_Type),\
 (sizeof(value)/sizeof(Dee_WideChar))-1,value}\
 DEE_COMPILER_MSVC_WARNING_POP
#define DeeWideString_NEW_STATIC_EX(name,size,...)\
 DEE_COMPILER_MSVC_WARNING_PUSH(4640)\
 static struct {\
  DEE_OBJECT_HEAD Dee_size_t s_len;\
  Dee_WideChar s_str[size];\
  Dee_WideChar s_end;\
 } name = {DEE_OBJECT_HEAD_INIT(&DeeWideString_Type),size,__VA_ARGS__,0}\
 DEE_COMPILER_MSVC_WARNING_POP
#endif
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
#ifdef __INTELLISENSE__
#define DeeUtf8String_NEW_STATIC(name,value)\
static struct { DEE_OBJECT_HEAD Dee_Utf8Char s_str[sizeof(value)/sizeof(Dee_Utf8Char)]; }\
 name = {DEE_OBJECT_HEAD_INIT(0),value}
#define DeeUtf8String_NEW_STATIC_EX(name,size,...)\
static struct { DEE_OBJECT_HEAD Dee_Utf8Char s_str[size]; Dee_Utf8Char s_end; }\
 name = {DEE_OBJECT_HEAD_INIT(0),__VA_ARGS__,0}
#else
#define DeeUtf8String_NEW_STATIC(name,value)\
 DEE_COMPILER_MSVC_WARNING_PUSH(4640)\
 static struct {\
  DEE_OBJECT_HEAD Dee_size_t s_len;\
  Dee_Utf8Char s_str[sizeof(value)/sizeof(Dee_Utf8Char)];\
 } name = {DEE_OBJECT_HEAD_INIT(&DeeUtf8String_Type),\
 (sizeof(value)/sizeof(Dee_Utf8Char))-1,value}\
 DEE_COMPILER_MSVC_WARNING_POP
#define DeeUtf8String_NEW_STATIC_EX(name,size,...)\
 DEE_COMPILER_MSVC_WARNING_PUSH(4640)\
 static struct {\
  DEE_OBJECT_HEAD Dee_size_t s_len;\
  Dee_Utf8Char s_str[size];\
  Dee_Utf8Char s_end;\
 } name = {DEE_OBJECT_HEAD_INIT(&DeeUtf8String_Type),size,__VA_ARGS__,0}\
 DEE_COMPILER_MSVC_WARNING_POP
#endif
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
#ifdef __INTELLISENSE__
#define DeeUtf16String_NEW_STATIC(name,value)\
static struct { DEE_OBJECT_HEAD Dee_Utf16Char s_str[sizeof(value)/sizeof(Dee_Utf16Char)]; }\
 name = {DEE_OBJECT_HEAD_INIT(0),value}
#define DeeUtf16String_NEW_STATIC_EX(name,size,...)\
static struct { DEE_OBJECT_HEAD Dee_Utf16Char s_str[size]; Dee_Utf16Char s_end; }\
 name = {DEE_OBJECT_HEAD_INIT(0),__VA_ARGS__,0}
#else
#define DeeUtf16String_NEW_STATIC(name,value)\
 DEE_COMPILER_MSVC_WARNING_PUSH(4640)\
 static struct {\
  DEE_OBJECT_HEAD Dee_size_t s_len;\
  Dee_Utf16Char s_str[sizeof(value)/sizeof(Dee_Utf16Char)];\
 } name = {DEE_OBJECT_HEAD_INIT(&DeeUtf16String_Type),\
 (sizeof(value)/sizeof(Dee_Utf16Char))-1,value}\
 DEE_COMPILER_MSVC_WARNING_POP
#define DeeUtf16String_NEW_STATIC_EX(name,size,...)\
 DEE_COMPILER_MSVC_WARNING_PUSH(4640)\
 static struct {\
  DEE_OBJECT_HEAD Dee_size_t s_len;\
  Dee_Utf16Char s_str[size];\
  Dee_Utf16Char s_end;\
 } name = {DEE_OBJECT_HEAD_INIT(&DeeUtf16String_Type),size,__VA_ARGS__,0}\
 DEE_COMPILER_MSVC_WARNING_POP
#endif
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
#ifdef __INTELLISENSE__
#define DeeUtf32String_NEW_STATIC(name,value)\
static struct { DEE_OBJECT_HEAD Dee_Utf32Char s_str[sizeof(value)/sizeof(Dee_Utf32Char)]; }\
 name = {DEE_OBJECT_HEAD_INIT(0),value}
#define DeeUtf32String_NEW_STATIC_EX(name,size,...)\
static struct { DEE_OBJECT_HEAD Dee_Utf32Char s_str[size]; Dee_Utf32Char s_end; }\
 name = {DEE_OBJECT_HEAD_INIT(0),__VA_ARGS__,0}
#else
#define DeeUtf32String_NEW_STATIC(name,value)\
 DEE_COMPILER_MSVC_WARNING_PUSH(4640)\
 static struct {\
  DEE_OBJECT_HEAD Dee_size_t s_len;\
  Dee_Utf32Char s_str[sizeof(value)/sizeof(Dee_Utf32Char)];\
 } name = {DEE_OBJECT_HEAD_INIT(&DeeUtf32String_Type),\
 (sizeof(value)/sizeof(Dee_Utf32Char))-1,value}\
 DEE_COMPILER_MSVC_WARNING_POP
#define DeeUtf32String_NEW_STATIC_EX(name,size,...)\
 DEE_COMPILER_MSVC_WARNING_PUSH(4640)\
 static struct {\
  DEE_OBJECT_HEAD Dee_size_t s_len;\
  Dee_Utf32Char s_str[size];\
  Dee_Utf32Char s_end;\
 } name = {DEE_OBJECT_HEAD_INIT(&DeeUtf32String_Type),size,__VA_ARGS__,0}\
 DEE_COMPILER_MSVC_WARNING_POP
#endif
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
#endif /* DEE_LIMITED_DEX */


#define DEE_PRIVATE_STRING_CAST_TO_EX_0_0(enc,N8) Dee##enc##String_ToUInt##N8
#define DEE_PRIVATE_STRING_CAST_TO_EX_0_1(enc,N8) Dee##enc##String_ToInt##N8
#define DEE_PRIVATE_STRING_CAST_TO_EX_02(signed) DEE_PRIVATE_STRING_CAST_TO_EX_0_##signed
#define DEE_PRIVATE_STRING_CAST_TO_EX_0(enc,signed,N8) DEE_PRIVATE_STRING_CAST_TO_EX_02(signed)(enc,N8)
#define DEE_PRIVATE_STRING_CAST_TO_EX_1(enc,signed,N8) Dee##enc##String_ToFloat
#define DEE_PRIVATE_STRING_CAST_TO_EX_2(enc,signed,N8) Dee##enc##String_ToDouble
#define DEE_PRIVATE_STRING_CAST_TO_EX_3(enc,signed,N8) Dee##enc##String_ToLDouble
#define DEE_PRIVATE_STRING_CAST_TO_EX2(float) DEE_PRIVATE_STRING_CAST_TO_EX_##float
#define DEE_PRIVATE_STRING_CAST_TO_EX(enc,float,signed,N8) DEE_PRIVATE_STRING_CAST_TO_EX2(float)(enc,signed,N8)
#define DEE_PRIVATE_STRING_CAST_TO_F(enc,T) DEE_PRIVATE_STRING_CAST_TO_EX(enc,DEE_TYPEDEF_FLOATID(T),DEE_TYPEDEF_SIGNED(T),DEE_PP_MUL8(DEE_TYPEDEF_SIZEOF(T)))
#define DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_EX_0_0(enc,N8) Dee##enc##String_ToUInt##N8##WithLength
#define DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_EX_0_1(enc,N8) Dee##enc##String_ToInt##N8##WithLength
#define DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_EX_02(signed) DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_EX_0_##signed
#define DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_EX_0(enc,signed,N8) DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_EX_02(signed)(enc,N8)
#define DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_EX_1(enc,signed,N8) Dee##enc##String_ToFloatWithLength
#define DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_EX_2(enc,signed,N8) Dee##enc##String_ToDoubleWithLength
#define DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_EX_3(enc,signed,N8) Dee##enc##String_ToLDoubleWithLength
#define DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_EX2(float) DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_EX_##float
#define DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_EX(enc,float,signed,N8) DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_EX2(float)(enc,signed,N8)
#define DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_F(enc,T) DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_EX(enc,DEE_TYPEDEF_FLOATID(T),DEE_TYPEDEF_SIGNED(T),DEE_PP_MUL8(DEE_TYPEDEF_SIZEOF(T)))


//////////////////////////////////////////////////////////////////////////
// Flags for creating new strings from character arrays
#define DEE_STRING_DECODE_FLAG_NONE    0x00000000
#define DEE_STRING_DECODE_FLAG_RELAXED 0x00000001 /*< Relaxed unicode encode/decode rules. */
#define DEE_STRING_DECODE_FLAG_DEFAULT DEE_STRING_DECODE_FLAG_NONE


#if DEE_CONFIG_HAVE_ENCODING_WIDE
DEE_DATA_DECL(struct _DeeEmptyWideStringObject)       _Dee_EmptyWideString;
#define Dee_EmptyWideString                          ((DeeObject *)&_Dee_EmptyWideString)
#define DeeWideString_NewEmpty()                     (_DeeObject_INCREF(Dee_EmptyWideString),Dee_EmptyWideString)
#define DeeReturn_EmptyWideString                    return _DeeObject_INCREF(Dee_EmptyWideString),Dee_EmptyWideString
#define DeeReturn_STATIC_WIDE_STRING(s)              do{DeeWideString_NEW_STATIC(_static_return_str,s);_DeeObject_INCREF(((DeeObject*)&_static_return_str));return(DeeObject*)&_static_return_str;}while(0)
#define DeeReturn_STATIC_WIDE_STRING_EX(s,...)       do{DeeWideString_NEW_STATIC_EX(_static_return_str,s,__VA_ARGS__);_DeeObject_INCREF(((DeeObject*)&_static_return_str));return(DeeObject*)&_static_return_str;}while(0)
#define DeeReturn_STATIC_WIDE_STRING_NOREF(s)        do{DeeWideString_NEW_STATIC(_static_return_str,s);return(DeeObject*)&_static_return_str;}while(0)
#define DeeReturn_STATIC_WIDE_STRING_EX_NOREF(s,...) do{DeeWideString_NEW_STATIC_EX(_static_return_str,s,__VA_ARGS__);return(DeeObject*)&_static_return_str;}while(0)
#define DeeWideString_CheckExact(ob)         DeeObject_InstanceOfExact(ob,&DeeWideString_Type)
#define DeeWideString_Check                  DeeWideString_CheckExact
#define DeeWideString_CheckEmpty(ob)       ((DeeObject *)Dee_REQUIRES_POINTER(ob)==Dee_EmptyWideString)
#define DeeWideStringIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeWideStringIterator_Type)
#define DeeWideStringIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeWideStringIterator_Type)
#define DeeWideString_Cast(ob)               DeeWideString_CastEx(ob,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideString_InplaceCast(ob)        DeeWideString_InplaceCastEx(ob,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideString_New                    DeeWideString_FromWideString
#define DeeWideString_NewWithLength          DeeWideString_FromWideStringWithLength
#define DeeWideString_NewQuoted              DeeWideString_FromQuotedWideString
#define DeeWideString_NewQuotedWithLength    DeeWideString_FromQuotedWideStringWithLength
#define DeeWideString_NEW(s)                 DeeWideString_NewWithLength((sizeof(s)/sizeof(Dee_WideChar))-1,s)
#define DeeWideString_Repr                   DeeWideString_Utf8Repr
#define DeeWideString_MyRepr                 DeeWideString_WideRepr
#define DeeWideString_Join(sep,sequence)     DeeWideString_TJoin(sep,Dee_TYPE(sequence),sequence)
#define DeeWideString_ZFill(self,width)      DeeWideString_RJust(self,width,DEE_WIDECHAR_C('0'))
#define DeeWideString_FromStringWithLengthEx                 DeeWideString_FromUtf8StringWithLengthEx
#define DeeWideString_FromMyStringWithLengthEx               DeeWideString_FromWideStringWithLengthEx
#define DeeWideString_FromStringWithLength(len,s)            DeeWideString_FromStringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideString_FromMyStringWithLength(len,s)          DeeWideString_FromMyStringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideString_FromWideStringWithLength(len,s)        DeeWideString_FromWideStringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideString_FromUtf8StringWithLength(len,s)        DeeWideString_FromUtf8StringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideString_FromUtf16StringWithLength(len,s)       DeeWideString_FromUtf16StringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideString_FromUtf32StringWithLength(len,s)       DeeWideString_FromUtf32StringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideString_FromStringEx                           DeeWideString_FromUtf8StringEx
#define DeeWideString_FromMyStringEx                         DeeWideString_FromWideStringEx
#define DeeWideString_FromString(s)                          DeeWideString_FromStringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideString_FromMyString(s)                        DeeWideString_FromMyStringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideString_FromWideString(s)                      DeeWideString_FromWideStringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideString_FromUtf8String(s)                      DeeWideString_FromUtf8StringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideString_FromUtf16String(s)                     DeeWideString_FromUtf16StringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideString_FromUtf32String(s)                     DeeWideString_FromUtf32StringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideString_FromQuotedStringWithLength             DeeWideString_FromQuotedUtf8StringWithLength
#define DeeWideString_FromQuotedMyStringWithLength           DeeWideString_FromQuotedWideStringWithLength
#define DeeWideString_FromQuotedString                       DeeWideString_FromQuotedUtf8String
#define DeeWideString_FromQuotedMyString                     DeeWideString_FromQuotedWideString
#define DeeWideString_ToNumber_F(T) DEE_PRIVATE_STRING_CAST_TO_F(Wide,T)
#define DeeWideString_ToNumber(T,str_,result) DeeWideString_ToNumber_F(T)(str_,result)
#define DeeWideString_ToNumberWithLength_F(T) DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_F(Wide,T)
#define DeeWideString_ToNumberWithLength(T,len,str_,result) DeeWideString_ToNumberWithLength_F(T)(len,str_,result)
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */

#if DEE_CONFIG_HAVE_ENCODING_UTF8
DEE_DATA_DECL(struct _DeeEmptyUtf8StringObject)       _Dee_EmptyUtf8String;
DEE_DATA_DECL(struct _DeeUtf8StringCharacterObject)   _DeeUtf8String_Characters[256];
#define Dee_EmptyUtf8String                          ((DeeObject *)&_Dee_EmptyUtf8String)
#define DeeUtf8String_NewEmpty()                     (_DeeObject_INCREF(Dee_EmptyUtf8String),Dee_EmptyUtf8String)
#define DeeUtf8String_Character(c)                   ((DeeObject *)(_DeeUtf8String_Characters+(Dee_uint8_t)(c)))
#define DeeReturn_Utf8Character(c)                   do{Dee_uint8_t const _c=(Dee_uint8_t)(c);return _DeeObject_INCREF(((DeeObject *)(_DeeUtf8String_Characters+_c))),(DeeObject *)(_DeeUtf8String_Characters+_c);}while(0)
#define DeeReturn_EmptyUtf8String                    return _DeeObject_INCREF(Dee_EmptyUtf8String),Dee_EmptyUtf8String
#define DeeReturn_STATIC_UTF8_STRING(s)              do{DeeUtf8String_NEW_STATIC(_static_return_str,s);_DeeObject_INCREF(((DeeObject*)&_static_return_str));return(DeeObject*)&_static_return_str;}while(0)
#define DeeReturn_STATIC_UTF8_STRING_EX(s,...)       do{DeeUtf8String_NEW_STATIC_EX(_static_return_str,s,__VA_ARGS__);_DeeObject_INCREF(((DeeObject*)&_static_return_str));return(DeeObject*)&_static_return_str;}while(0)
#define DeeReturn_STATIC_UTF8_STRING_NOREF(s)        do{DeeUtf8String_NEW_STATIC(_static_return_str,s);return(DeeObject*)&_static_return_str;}while(0)
#define DeeReturn_STATIC_UTF8_STRING_EX_NOREF(s,...) do{DeeUtf8String_NEW_STATIC_EX(_static_return_str,s,__VA_ARGS__);return(DeeObject*)&_static_return_str;}while(0)
#define DeeUtf8String_CheckExact(ob)         DeeObject_InstanceOfExact(ob,&DeeUtf8String_Type)
#define DeeUtf8String_Check                  DeeUtf8String_CheckExact
#define DeeUtf8String_CheckEmpty(ob)       ((DeeObject *)Dee_REQUIRES_POINTER(ob)==Dee_EmptyUtf8String)
#define DeeUtf8StringIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeUtf8StringIterator_Type)
#define DeeUtf8StringIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeUtf8StringIterator_Type)
#define DeeUtf8String_Cast(ob)            DeeUtf8String_CastEx(ob,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8String_InplaceCast(ob)     DeeUtf8String_InplaceCastEx(ob,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8String_New                 DeeUtf8String_FromUtf8String
#define DeeUtf8String_NewWithLength       DeeUtf8String_FromUtf8StringWithLength
#define DeeUtf8String_NewQuoted           DeeUtf8String_FromQuotedUtf8String
#define DeeUtf8String_NewQuotedWithLength DeeUtf8String_FromQuotedUtf8StringWithLength
#define DeeUtf8String_NEW(s)              DeeUtf8String_NewWithLength((sizeof(s)/sizeof(Dee_Utf8Char))-1,s)
#define DeeUtf8String_Repr                DeeUtf8String_Utf8Repr
#define DeeUtf8String_MyRepr              DeeUtf8String_Utf8Repr
#define DeeUtf8String_Join(sep,sequence)  DeeUtf8String_TJoin(sep,Dee_TYPE(sequence),sequence)
#define DeeUtf8String_ZFill(self,width)   DeeUtf8String_RJust(self,width,DEE_CHAR8_C('0'))
#define DeeUtf8String_FromStringWithLengthEx                 DeeUtf8String_FromUtf8StringWithLengthEx
#define DeeUtf8String_FromMyStringWithLengthEx               DeeUtf8String_FromUtf8StringWithLengthEx
#define DeeUtf8String_FromStringWithLength(len,s)            DeeUtf8String_FromStringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8String_FromMyStringWithLength(len,s)          DeeUtf8String_FromMyStringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8String_FromWideStringWithLength(len,s)        DeeUtf8String_FromWideStringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8String_FromUtf8StringWithLength(len,s)        DeeUtf8String_FromUtf8StringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8String_FromUtf16StringWithLength(len,s)       DeeUtf8String_FromUtf16StringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8String_FromUtf32StringWithLength(len,s)       DeeUtf8String_FromUtf32StringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8String_FromStringEx                           DeeUtf8String_FromUtf8StringEx
#define DeeUtf8String_FromMyStringEx                         DeeUtf8String_FromUtf8StringEx
#define DeeUtf8String_FromString(s)                          DeeUtf8String_FromStringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8String_FromMyString(s)                        DeeUtf8String_FromMyStringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8String_FromWideString(s)                      DeeUtf8String_FromWideStringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8String_FromUtf8String(s)                      DeeUtf8String_FromUtf8StringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8String_FromUtf16String(s)                     DeeUtf8String_FromUtf16StringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8String_FromUtf32String(s)                     DeeUtf8String_FromUtf32StringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8String_FromQuotedStringWithLength             DeeUtf8String_FromQuotedUtf8StringWithLength
#define DeeUtf8String_FromQuotedMyStringWithLength           DeeUtf8String_FromQuotedUtf8StringWithLength
#define DeeUtf8String_FromQuotedString                       DeeUtf8String_FromQuotedUtf8String
#define DeeUtf8String_FromQuotedMyString                     DeeUtf8String_FromQuotedUtf8String
#define DeeUtf8String_ToNumber_F(T) DEE_PRIVATE_STRING_CAST_TO_F(Utf8,T)
#define DeeUtf8String_ToNumber(T,str_,result) DeeUtf8String_ToNumber_F(T)(str_,result)
#define DeeUtf8String_ToNumberWithLength_F(T) DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_F(Utf8,T)
#define DeeUtf8String_ToNumberWithLength(T,len,str_,result) DeeUtf8String_ToNumberWithLength_F(T)(len,str_,result)
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */

#if DEE_CONFIG_HAVE_ENCODING_UTF16
DEE_DATA_DECL(struct _DeeEmptyUtf16StringObject)       _Dee_EmptyUtf16String;
#define Dee_EmptyUtf16String                          ((DeeObject *)&_Dee_EmptyUtf16String)
#define DeeUtf16String_NewEmpty()                     (_DeeObject_INCREF(Dee_EmptyUtf16String),Dee_EmptyUtf16String)
#define DeeReturn_EmptyUtf16String                    return _DeeObject_INCREF(Dee_EmptyUtf16String),Dee_EmptyUtf16String
#define DeeReturn_STATIC_UTF16_STRING(s)              do{DeeUtf16String_NEW_STATIC(_static_return_str,s);_DeeObject_INCREF(((DeeObject*)&_static_return_str));return(DeeObject*)&_static_return_str;}while(0)
#define DeeReturn_STATIC_UTF16_STRING_EX(s,...)       do{DeeUtf16String_NEW_STATIC_EX(_static_return_str,s,__VA_ARGS__);_DeeObject_INCREF(((DeeObject*)&_static_return_str));return(DeeObject*)&_static_return_str;}while(0)
#define DeeReturn_STATIC_UTF16_STRING_NOREF(s)        do{DeeUtf16String_NEW_STATIC(_static_return_str,s);return(DeeObject*)&_static_return_str;}while(0)
#define DeeReturn_STATIC_UTF16_STRING_EX_NOREF(s,...) do{DeeUtf16String_NEW_STATIC_EX(_static_return_str,s,__VA_ARGS__);return(DeeObject*)&_static_return_str;}while(0)
#define DeeUtf16String_CheckExact(ob)         DeeObject_InstanceOfExact(ob,&DeeUtf16String_Type)
#define DeeUtf16String_Check                  DeeUtf16String_CheckExact
#define DeeUtf16String_CheckEmpty(ob)       ((DeeObject *)Dee_REQUIRES_POINTER(ob)==Dee_EmptyUtf16String)
#define DeeUtf16StringIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeUtf16StringIterator_Type)
#define DeeUtf16StringIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeUtf16StringIterator_Type)
#define DeeUtf16String_Cast(ob)               DeeUtf16String_CastEx(ob,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16String_InplaceCast(ob)        DeeUtf16String_InplaceCastEx(ob,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16String_New                    DeeUtf16String_FromUtf16String
#define DeeUtf16String_NewWithLength          DeeUtf16String_FromUtf16StringWithLength
#define DeeUtf16String_NewQuoted              DeeUtf16String_FromQuotedUtf16String
#define DeeUtf16String_NewQuotedWithLength    DeeUtf16String_FromQuotedUtf16StringWithLength
#define DeeUtf16String_NEW(s)                 DeeUtf16String_NewWithLength((sizeof(s)/sizeof(Dee_Utf16Char))-1,s)
#define DeeUtf16String_Repr                   DeeUtf16String_Utf8Repr
#define DeeUtf16String_MyRepr                 DeeUtf16String_Utf16Repr
#define DeeUtf16String_Join(sep,sequence)     DeeUtf16String_TJoin(sep,Dee_TYPE(sequence),sequence)
#define DeeUtf16String_ZFill(self,width)      DeeUtf16String_RJust(self,width,DEE_CHAR16_C('0'))
#define DeeUtf16String_FromStringWithLengthEx                 DeeUtf16String_FromUtf8StringWithLengthEx
#define DeeUtf16String_FromMyStringWithLengthEx               DeeUtf16String_FromUtf16StringWithLengthEx
#define DeeUtf16String_FromStringWithLength(len,s)            DeeUtf16String_FromStringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16String_FromMyStringWithLength(len,s)          DeeUtf16String_FromMyStringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16String_FromWideStringWithLength(len,s)        DeeUtf16String_FromWideStringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16String_FromUtf8StringWithLength(len,s)        DeeUtf16String_FromUtf8StringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16String_FromUtf16StringWithLength(len,s)       DeeUtf16String_FromUtf16StringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16String_FromUtf32StringWithLength(len,s)       DeeUtf16String_FromUtf32StringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16String_FromStringEx                           DeeUtf16String_FromUtf8StringEx
#define DeeUtf16String_FromMyStringEx                         DeeUtf16String_FromUtf16StringEx
#define DeeUtf16String_FromString(s)                          DeeUtf16String_FromStringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16String_FromMyString(s)                        DeeUtf16String_FromMyStringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16String_FromWideString(s)                      DeeUtf16String_FromWideStringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16String_FromUtf8String(s)                      DeeUtf16String_FromUtf8StringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16String_FromUtf16String(s)                     DeeUtf16String_FromUtf16StringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16String_FromUtf32String(s)                     DeeUtf16String_FromUtf32StringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16String_FromQuotedStringWithLength             DeeUtf16String_FromQuotedUtf8StringWithLength
#define DeeUtf16String_FromQuotedMyStringWithLength           DeeUtf16String_FromQuotedUtf16StringWithLength
#define DeeUtf16String_FromQuotedString                       DeeUtf16String_FromQuotedUtf8String
#define DeeUtf16String_FromQuotedMyString                     DeeUtf16String_FromQuotedUtf16String
#define DeeUtf16String_ToNumber_F(T) DEE_PRIVATE_STRING_CAST_TO_F(Utf16,T)
#define DeeUtf16String_ToNumber(T,str_,result) DeeUtf16String_ToNumber_F(T)(str_,result)
#define DeeUtf16String_ToNumberWithLength_F(T) DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_F(Utf16,T)
#define DeeUtf16String_ToNumberWithLength(T,len,str_,result) DeeUtf16String_ToNumberWithLength_F(T)(len,str_,result)
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */

#if DEE_CONFIG_HAVE_ENCODING_UTF32
DEE_DATA_DECL(struct _DeeEmptyUtf32StringObject)       _Dee_EmptyUtf32String;
#define Dee_EmptyUtf32String                          ((DeeObject *)&_Dee_EmptyUtf32String)
#define DeeUtf32String_NewEmpty()                     (_DeeObject_INCREF(Dee_EmptyUtf32String),Dee_EmptyUtf32String)
#define DeeReturn_EmptyUtf32String                    return _DeeObject_INCREF(Dee_EmptyUtf32String),Dee_EmptyUtf32String
#define DeeReturn_STATIC_UTF32_STRING(s)              do{DeeUtf32String_NEW_STATIC(_static_return_str,s);_DeeObject_INCREF(((DeeObject*)&_static_return_str));return(DeeObject*)&_static_return_str;}while(0)
#define DeeReturn_STATIC_UTF32_STRING_EX(s,...)       do{DeeUtf32String_NEW_STATIC_EX(_static_return_str,s,__VA_ARGS__);_DeeObject_INCREF(((DeeObject*)&_static_return_str));return(DeeObject*)&_static_return_str;}while(0)
#define DeeReturn_STATIC_UTF32_STRING_NOREF(s)        do{DeeUtf32String_NEW_STATIC(_static_return_str,s);return(DeeObject*)&_static_return_str;}while(0)
#define DeeReturn_STATIC_UTF32_STRING_EX_NOREF(s,...) do{DeeUtf32String_NEW_STATIC_EX(_static_return_str,s,__VA_ARGS__);return(DeeObject*)&_static_return_str;}while(0)
#define DeeUtf32String_CheckExact(ob)         DeeObject_InstanceOfExact(ob,&DeeUtf32String_Type)
#define DeeUtf32String_Check                  DeeUtf32String_CheckExact
#define DeeUtf32String_CheckEmpty(ob)       ((DeeObject *)Dee_REQUIRES_POINTER(ob)==Dee_EmptyUtf32String)
#define DeeUtf32StringIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeUtf32StringIterator_Type)
#define DeeUtf32StringIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeUtf32StringIterator_Type)
#define DeeUtf32String_Cast(ob)               DeeUtf32String_CastEx(ob,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32String_InplaceCast(ob)        DeeUtf32String_InplaceCastEx(ob,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32String_New                    DeeUtf32String_FromUtf32String
#define DeeUtf32String_NewWithLength          DeeUtf32String_FromUtf32StringWithLength
#define DeeUtf32String_NewQuoted              DeeUtf32String_FromQuotedUtf32String
#define DeeUtf32String_NewQuotedWithLength    DeeUtf32String_FromQuotedUtf32StringWithLength
#define DeeUtf32String_NEW(s)                 DeeUtf32String_NewWithLength((sizeof(s)/sizeof(Dee_Utf32Char))-1,s)
#define DeeUtf32String_Repr                   DeeUtf32String_Utf8Repr
#define DeeUtf32String_MyRepr                 DeeUtf32String_Utf32Repr
#define DeeUtf32String_Join(sep,sequence)     DeeUtf32String_TJoin(sep,Dee_TYPE(sequence),sequence)
#define DeeUtf32String_ZFill(self,width)      DeeUtf32String_RJust(self,width,DEE_CHAR32_C('0'))
#define DeeUtf32String_FromStringWithLengthEx                 DeeUtf32String_FromUtf8StringWithLengthEx
#define DeeUtf32String_FromMyStringWithLengthEx               DeeUtf32String_FromUtf32StringWithLengthEx
#define DeeUtf32String_FromStringWithLength(len,s)            DeeUtf32String_FromStringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32String_FromMyStringWithLength(len,s)          DeeUtf32String_FromMyStringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32String_FromWideStringWithLength(len,s)        DeeUtf32String_FromWideStringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32String_FromUtf8StringWithLength(len,s)        DeeUtf32String_FromUtf8StringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32String_FromUtf16StringWithLength(len,s)       DeeUtf32String_FromUtf16StringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32String_FromUtf32StringWithLength(len,s)       DeeUtf32String_FromUtf32StringWithLengthEx(len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32String_FromStringEx                           DeeUtf32String_FromUtf8StringEx
#define DeeUtf32String_FromMyStringEx                         DeeUtf32String_FromUtf32StringEx
#define DeeUtf32String_FromString(s)                          DeeUtf32String_FromStringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32String_FromMyString(s)                        DeeUtf32String_FromMyStringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32String_FromWideString(s)                      DeeUtf32String_FromWideStringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32String_FromUtf8String(s)                      DeeUtf32String_FromUtf8StringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32String_FromUtf16String(s)                     DeeUtf32String_FromUtf16StringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32String_FromUtf32String(s)                     DeeUtf32String_FromUtf32StringEx(s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32String_FromQuotedStringWithLength             DeeUtf32String_FromQuotedUtf8StringWithLength
#define DeeUtf32String_FromQuotedMyStringWithLength           DeeUtf32String_FromQuotedUtf32StringWithLength
#define DeeUtf32String_FromQuotedString                       DeeUtf32String_FromQuotedUtf8String
#define DeeUtf32String_FromQuotedMyString                     DeeUtf32String_FromQuotedUtf32String
#define DeeUtf32String_ToNumber_F(T) DEE_PRIVATE_STRING_CAST_TO_F(Utf32,T)
#define DeeUtf32String_ToNumber(T,str_,result) DeeUtf32String_ToNumber_F(T)(str_,result)
#define DeeUtf32String_ToNumberWithLength_F(T) DEE_PRIVATE_STRING_WITH_LENGTH_CAST_TO_F(Utf32,T)
#define DeeUtf32String_ToNumberWithLength(T,len,str_,result) DeeUtf32String_ToNumberWithLength_F(T)(len,str_,result)
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// StringWriter
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#if DEE_CONFIG_HAVE_ENCODING_WIDE
#ifdef DEE_LIMITED_DEX
#define DeeWideStringWriter_INIT()        {NULL,0}
#define DeeWideStringWriter_STR(ob)       ((ob)->sw_str?DeeWideString_STR((ob)->sw_str):NULL)
#define DeeWideStringWriter_EMPTY(ob)     ((ob)->sw_str==NULL)
#define DeeWideStringWriter_ELEM(ob)      DeeWideString_STR((ob)->sw_str)
#define DeeWideStringWriter_SIZE(ob)      (ob)->sw_len
#define DeeWideStringWriter_ALLOCATED(ob) ((ob)->sw_str?DeeWideString_SIZE((ob)->sw_str):0)
#else
#define DeeWideStringWriter_STR           DeeWideStringWriter_Str
#define DeeWideStringWriter_EMPTY         DeeWideStringWriter_Empty
#define DeeWideStringWriter_ELEM          DeeWideStringWriter_Elem
#define DeeWideStringWriter_SIZE          DeeWideStringWriter_Size
#define DeeWideStringWriter_ALLOCATED     DeeWideStringWriter_Allocated
#endif
#define DeeWideStringWriter_WRITE_STRING                      DeeWideStringWriter_WRITE_UTF8_STRING
#define DeeWideStringWriter_WRITE_MY_STRING                   DeeWideStringWriter_WRITE_WIDE_STRING
#define DeeWideStringWriter_WRITE_WIDE_STRING(self,s)         DeeWideStringWriter_WriteWideStringWithLength(self,(sizeof(s)/sizeof(Dee_WideChar))-1,s)
#define DeeWideStringWriter_WRITE_UTF8_STRING(self,s)         DeeWideStringWriter_WriteUtf8StringWithLength(self,(sizeof(s)/sizeof(Dee_Utf8Char))-1,s)
#define DeeWideStringWriter_WRITE_UTF16_STRING(self,s)        DeeWideStringWriter_WriteUtf16StringWithLength(self,(sizeof(s)/sizeof(Dee_Utf16Char))-1,s)
#define DeeWideStringWriter_WRITE_UTF32_STRING(self,s)        DeeWideStringWriter_WriteUtf32StringWithLength(self,(sizeof(s)/sizeof(Dee_Utf32Char))-1,s)
#define DeeWideStringWriter_WriteString                       DeeWideStringWriter_WriteUtf8String
#define DeeWideStringWriter_WriteMyString                     DeeWideStringWriter_WriteWideString
#define DeeWideStringWriter_WriteStringWithLength             DeeWideStringWriter_WriteUtf8StringWithLength
#define DeeWideStringWriter_WriteMyStringWithLength           DeeWideStringWriter_WriteWideStringWithLength
#define DeeWideStringWriter_WriteChar                         DeeWideStringWriter_WriteUtf8Char
#define DeeWideStringWriter_WriteMyChar                       DeeWideStringWriter_WriteWideChar
#define DeeWideStringWriter_WriteStringEx                     DeeWideStringWriter_WriteUtf8StringEx
#define DeeWideStringWriter_WriteMyStringEx                   DeeWideStringWriter_WriteWideStringEx
#define DeeWideStringWriter_WriteStringWithLengthEx           DeeWideStringWriter_WriteUtf8StringWithLengthEx
#define DeeWideStringWriter_WriteMyStringWithLengthEx         DeeWideStringWriter_WriteWideStringWithLengthEx
#define DeeWideStringWriter_WriteCharEx                       DeeWideStringWriter_WriteUtf8CharEx
#define DeeWideStringWriter_WriteMyCharEx                     DeeWideStringWriter_WriteWideCharEx
#define DeeWideStringWriter_WriteQuotedString                 DeeWideStringWriter_WriteQuotedUtf8String
#define DeeWideStringWriter_WriteQuotedMyString               DeeWideStringWriter_WriteQuotedWideString
#define DeeWideStringWriter_WriteQuotedStringWithLength       DeeWideStringWriter_WriteQuotedUtf8StringWithLength
#define DeeWideStringWriter_WriteQuotedMyStringWithLength     DeeWideStringWriter_WriteQuotedWideStringWithLength
#define DeeWideStringWriter_SpecWriteString                   DeeWideStringWriter_SpecWriteUtf8String
#define DeeWideStringWriter_SpecWriteMyString                 DeeWideStringWriter_SpecWriteWideString
#define DeeWideStringWriter_SpecWriteStringWithLength         DeeWideStringWriter_SpecWriteUtf8StringWithLength
#define DeeWideStringWriter_SpecWriteMyStringWithLength       DeeWideStringWriter_SpecWriteWideStringWithLength
#define DeeWideStringWriter_SpecWriteChar                     DeeWideStringWriter_SpecWriteUtf8Char
#define DeeWideStringWriter_SpecWriteMyChar                   DeeWideStringWriter_SpecWriteWideChar
#define DeeWideStringWriter_SpecWriteStringEx                 DeeWideStringWriter_SpecWriteUtf8StringEx
#define DeeWideStringWriter_SpecWriteMyStringEx               DeeWideStringWriter_SpecWriteWideStringEx
#define DeeWideStringWriter_SpecWriteStringWithLengthEx       DeeWideStringWriter_SpecWriteUtf8StringWithLengthEx
#define DeeWideStringWriter_SpecWriteMyStringWithLengthEx     DeeWideStringWriter_SpecWriteWideStringWithLengthEx
#define DeeWideStringWriter_SpecWriteCharEx                   DeeWideStringWriter_SpecWriteUtf8CharEx
#define DeeWideStringWriter_SpecWriteMyCharEx                 DeeWideStringWriter_SpecWriteWideCharEx
#define DeeWideStringWriter_SpecWriteQuotedString             DeeWideStringWriter_SpecWriteQuotedUtf8String
#define DeeWideStringWriter_SpecWriteQuotedMyString           DeeWideStringWriter_SpecWriteQuotedWideString
#define DeeWideStringWriter_SpecWriteQuotedStringWithLength   DeeWideStringWriter_SpecWriteQuotedUtf8StringWithLength
#define DeeWideStringWriter_SpecWriteQuotedMyStringWithLength DeeWideStringWriter_SpecWriteQuotedWideStringWithLength
#define DeeWideStringWriter_WriteWideStringWithLength(ob,len,s)           DeeWideStringWriter_WriteWideStringWithLengthEx(ob,len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_WriteUtf8StringWithLength(ob,len,s)           DeeWideStringWriter_WriteUtf8StringWithLengthEx(ob,len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_WriteUtf16StringWithLength(ob,len,s)          DeeWideStringWriter_WriteUtf16StringWithLengthEx(ob,len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_WriteUtf32StringWithLength(ob,len,s)          DeeWideStringWriter_WriteUtf32StringWithLengthEx(ob,len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_WriteWideChar(ob,ch)                          DeeWideStringWriter_WriteWideCharEx(ob,ch,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_WriteUtf8Char(ob,ch)                          DeeWideStringWriter_WriteUtf8CharEx(ob,ch,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_WriteUtf16Char(ob,ch)                         DeeWideStringWriter_WriteUtf16CharEx(ob,ch,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_WriteUtf32Char(ob,ch)                         DeeWideStringWriter_WriteUtf32CharEx(ob,ch,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_WriteWideString(ob,s)                         DeeWideStringWriter_WriteWideStringEx(ob,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_WriteUtf8String(ob,s)                         DeeWideStringWriter_WriteUtf8StringEx(ob,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_WriteUtf16String(ob,s)                        DeeWideStringWriter_WriteUtf16StringEx(ob,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_WriteUtf32String(ob,s)                        DeeWideStringWriter_WriteUtf32StringEx(ob,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_SpecWriteWideStringWithLength(ob,len,s,spec)  DeeWideStringWriter_SpecWriteWideStringWithLengthEx(ob,len,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_SpecWriteUtf8StringWithLength(ob,len,s,spec)  DeeWideStringWriter_SpecWriteUtf8StringWithLengthEx(ob,len,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_SpecWriteUtf16StringWithLength(ob,len,s,spec) DeeWideStringWriter_SpecWriteUtf16StringWithLengthEx(ob,len,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_SpecWriteUtf32StringWithLength(ob,len,s,spec) DeeWideStringWriter_SpecWriteUtf32StringWithLengthEx(ob,len,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_SpecWriteWideChar(ob,ch,spec)                 DeeWideStringWriter_SpecWriteWideCharEx(ob,ch,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_SpecWriteUtf8Char(ob,ch,spec)                 DeeWideStringWriter_SpecWriteUtf8CharEx(ob,ch,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_SpecWriteUtf16Char(ob,ch,spec)                DeeWideStringWriter_SpecWriteUtf16CharEx(ob,ch,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_SpecWriteUtf32Char(ob,ch,spec)                DeeWideStringWriter_SpecWriteUtf32CharEx(ob,ch,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_SpecWriteWideString(ob,s,spec)                DeeWideStringWriter_SpecWriteWideStringEx(ob,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_SpecWriteUtf8String(ob,s,spec)                DeeWideStringWriter_SpecWriteUtf8StringEx(ob,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_SpecWriteUtf16String(ob,s,spec)               DeeWideStringWriter_SpecWriteUtf16StringEx(ob,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeWideStringWriter_SpecWriteUtf32String(ob,s,spec)               DeeWideStringWriter_SpecWriteUtf32StringEx(ob,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */

#if DEE_CONFIG_HAVE_ENCODING_UTF8
#ifdef DEE_LIMITED_DEX
#define DeeUtf8StringWriter_INIT()        {NULL,0}
#define DeeUtf8StringWriter_STR(ob)       ((ob)->sw_str?DeeUtf8String_STR((ob)->sw_str):NULL)
#define DeeUtf8StringWriter_EMPTY(ob)     ((ob)->sw_str==NULL)
#define DeeUtf8StringWriter_ELEM(ob)      DeeUtf8String_STR((ob)->sw_str)
#define DeeUtf8StringWriter_SIZE(ob)      (ob)->sw_len
#define DeeUtf8StringWriter_ALLOCATED(ob) ((ob)->sw_str?DeeUtf8String_SIZE((ob)->sw_str):0)
#else
#define DeeUtf8StringWriter_STR           DeeUtf8StringWriter_Str
#define DeeUtf8StringWriter_EMPTY         DeeUtf8StringWriter_Empty
#define DeeUtf8StringWriter_ELEM          DeeUtf8StringWriter_Elem
#define DeeUtf8StringWriter_SIZE          DeeUtf8StringWriter_Size
#define DeeUtf8StringWriter_ALLOCATED     DeeUtf8StringWriter_Allocated
#endif
#define DeeUtf8StringWriter_WRITE_STRING                      DeeUtf8StringWriter_WRITE_UTF8_STRING
#define DeeUtf8StringWriter_WRITE_MY_STRING                   DeeUtf8StringWriter_WRITE_UTF8_STRING
#define DeeUtf8StringWriter_WRITE_WIDE_STRING(self,s)         DeeUtf8StringWriter_WriteWideStringWithLength(self,(sizeof(s)/sizeof(Dee_WideChar))-1,s)
#define DeeUtf8StringWriter_WRITE_UTF8_STRING(self,s)         DeeUtf8StringWriter_WriteUtf8StringWithLength(self,(sizeof(s)/sizeof(Dee_Utf8Char))-1,s)
#define DeeUtf8StringWriter_WRITE_UTF16_STRING(self,s)        DeeUtf8StringWriter_WriteUtf16StringWithLength(self,(sizeof(s)/sizeof(Dee_Utf16Char))-1,s)
#define DeeUtf8StringWriter_WRITE_UTF32_STRING(self,s)        DeeUtf8StringWriter_WriteUtf32StringWithLength(self,(sizeof(s)/sizeof(Dee_Utf32Char))-1,s)
#define DeeUtf8StringWriter_WriteString                       DeeUtf8StringWriter_WriteUtf8String
#define DeeUtf8StringWriter_WriteMyString                     DeeUtf8StringWriter_WriteUtf8String
#define DeeUtf8StringWriter_WriteStringWithLength             DeeUtf8StringWriter_WriteUtf8StringWithLength
#define DeeUtf8StringWriter_WriteMyStringWithLength           DeeUtf8StringWriter_WriteUtf8StringWithLength
#define DeeUtf8StringWriter_WriteChar                         DeeUtf8StringWriter_WriteUtf8Char
#define DeeUtf8StringWriter_WriteMyChar                       DeeUtf8StringWriter_WriteUtf8Char
#define DeeUtf8StringWriter_WriteStringEx                     DeeUtf8StringWriter_WriteUtf8StringEx
#define DeeUtf8StringWriter_WriteMyStringEx                   DeeUtf8StringWriter_WriteUtf8StringEx
#define DeeUtf8StringWriter_WriteStringWithLengthEx           DeeUtf8StringWriter_WriteUtf8StringWithLengthEx
#define DeeUtf8StringWriter_WriteMyStringWithLengthEx         DeeUtf8StringWriter_WriteUtf8StringWithLengthEx
#define DeeUtf8StringWriter_WriteCharEx                       DeeUtf8StringWriter_WriteUtf8CharEx
#define DeeUtf8StringWriter_WriteMyCharEx                     DeeUtf8StringWriter_WriteUtf8CharEx
#define DeeUtf8StringWriter_WriteQuotedString                 DeeUtf8StringWriter_WriteQuotedUtf8String
#define DeeUtf8StringWriter_WriteQuotedMyString               DeeUtf8StringWriter_WriteQuotedUtf8String
#define DeeUtf8StringWriter_WriteQuotedStringWithLength       DeeUtf8StringWriter_WriteQuotedUtf8StringWithLength
#define DeeUtf8StringWriter_WriteQuotedMyStringWithLength     DeeUtf8StringWriter_WriteQuotedUtf8StringWithLength
#define DeeUtf8StringWriter_SpecWriteString                   DeeUtf8StringWriter_SpecWriteUtf8String
#define DeeUtf8StringWriter_SpecWriteMyString                 DeeUtf8StringWriter_SpecWriteUtf8String
#define DeeUtf8StringWriter_SpecWriteStringWithLength         DeeUtf8StringWriter_SpecWriteUtf8StringWithLength
#define DeeUtf8StringWriter_SpecWriteMyStringWithLength       DeeUtf8StringWriter_SpecWriteUtf8StringWithLength
#define DeeUtf8StringWriter_SpecWriteChar                     DeeUtf8StringWriter_SpecWriteUtf8Char
#define DeeUtf8StringWriter_SpecWriteMyChar                   DeeUtf8StringWriter_SpecWriteUtf8Char
#define DeeUtf8StringWriter_SpecWriteStringEx                 DeeUtf8StringWriter_SpecWriteUtf8StringEx
#define DeeUtf8StringWriter_SpecWriteMyStringEx               DeeUtf8StringWriter_SpecWriteUtf8StringEx
#define DeeUtf8StringWriter_SpecWriteStringWithLengthEx       DeeUtf8StringWriter_SpecWriteUtf8StringWithLengthEx
#define DeeUtf8StringWriter_SpecWriteMyStringWithLengthEx     DeeUtf8StringWriter_SpecWriteUtf8StringWithLengthEx
#define DeeUtf8StringWriter_SpecWriteCharEx                   DeeUtf8StringWriter_SpecWriteUtf8CharEx
#define DeeUtf8StringWriter_SpecWriteMyCharEx                 DeeUtf8StringWriter_SpecWriteUtf8CharEx
#define DeeUtf8StringWriter_SpecWriteQuotedString             DeeUtf8StringWriter_SpecWriteQuotedUtf8String
#define DeeUtf8StringWriter_SpecWriteQuotedMyString           DeeUtf8StringWriter_SpecWriteQuotedUtf8String
#define DeeUtf8StringWriter_SpecWriteQuotedStringWithLength   DeeUtf8StringWriter_SpecWriteQuotedUtf8StringWithLength
#define DeeUtf8StringWriter_SpecWriteQuotedMyStringWithLength DeeUtf8StringWriter_SpecWriteQuotedUtf8StringWithLength
#define DeeUtf8StringWriter_WriteWideStringWithLength(ob,len,s)           DeeUtf8StringWriter_WriteWideStringWithLengthEx(ob,len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_WriteUtf8StringWithLength(ob,len,s)           DeeUtf8StringWriter_WriteUtf8StringWithLengthEx(ob,len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_WriteUtf16StringWithLength(ob,len,s)          DeeUtf8StringWriter_WriteUtf16StringWithLengthEx(ob,len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_WriteUtf32StringWithLength(ob,len,s)          DeeUtf8StringWriter_WriteUtf32StringWithLengthEx(ob,len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_WriteWideChar(ob,ch)                          DeeUtf8StringWriter_WriteWideCharEx(ob,ch,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_WriteUtf8Char(ob,ch)                          DeeUtf8StringWriter_WriteUtf8CharEx(ob,ch,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_WriteUtf16Char(ob,ch)                         DeeUtf8StringWriter_WriteUtf16CharEx(ob,ch,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_WriteUtf32Char(ob,ch)                         DeeUtf8StringWriter_WriteUtf32CharEx(ob,ch,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_WriteWideString(ob,s)                         DeeUtf8StringWriter_WriteWideStringEx(ob,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_WriteUtf8String(ob,s)                         DeeUtf8StringWriter_WriteUtf8StringEx(ob,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_WriteUtf16String(ob,s)                        DeeUtf8StringWriter_WriteUtf16StringEx(ob,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_WriteUtf32String(ob,s)                        DeeUtf8StringWriter_WriteUtf32StringEx(ob,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_SpecWriteWideStringWithLength(ob,len,s,spec)  DeeUtf8StringWriter_SpecWriteWideStringWithLengthEx(ob,len,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_SpecWriteUtf8StringWithLength(ob,len,s,spec)  DeeUtf8StringWriter_SpecWriteUtf8StringWithLengthEx(ob,len,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_SpecWriteUtf16StringWithLength(ob,len,s,spec) DeeUtf8StringWriter_SpecWriteUtf16StringWithLengthEx(ob,len,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_SpecWriteUtf32StringWithLength(ob,len,s,spec) DeeUtf8StringWriter_SpecWriteUtf32StringWithLengthEx(ob,len,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_SpecWriteWideChar(ob,ch,spec)                 DeeUtf8StringWriter_SpecWriteWideCharEx(ob,ch,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_SpecWriteUtf8Char(ob,ch,spec)                 DeeUtf8StringWriter_SpecWriteUtf8CharEx(ob,ch,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_SpecWriteUtf16Char(ob,ch,spec)                DeeUtf8StringWriter_SpecWriteUtf16CharEx(ob,ch,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_SpecWriteUtf32Char(ob,ch,spec)                DeeUtf8StringWriter_SpecWriteUtf32CharEx(ob,ch,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_SpecWriteWideString(ob,s,spec)                DeeUtf8StringWriter_SpecWriteWideStringEx(ob,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_SpecWriteUtf8String(ob,s,spec)                DeeUtf8StringWriter_SpecWriteUtf8StringEx(ob,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_SpecWriteUtf16String(ob,s,spec)               DeeUtf8StringWriter_SpecWriteUtf16StringEx(ob,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf8StringWriter_SpecWriteUtf32String(ob,s,spec)               DeeUtf8StringWriter_SpecWriteUtf32StringEx(ob,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */

#if DEE_CONFIG_HAVE_ENCODING_UTF16
#ifdef DEE_LIMITED_DEX
#define DeeUtf16StringWriter_INIT()        {NULL,0}
#define DeeUtf16StringWriter_STR(ob)       ((ob)->sw_str?DeeUtf16String_STR((ob)->sw_str):NULL)
#define DeeUtf16StringWriter_EMPTY(ob)     ((ob)->sw_str==NULL)
#define DeeUtf16StringWriter_ELEM(ob)      DeeUtf16String_STR((ob)->sw_str)
#define DeeUtf16StringWriter_SIZE(ob)      (ob)->sw_len
#define DeeUtf16StringWriter_ALLOCATED(ob) ((ob)->sw_str?DeeUtf16String_SIZE((ob)->sw_str):0)
#else
#define DeeUtf16StringWriter_STR           DeeUtf16StringWriter_Str
#define DeeUtf16StringWriter_EMPTY         DeeUtf16StringWriter_Empty
#define DeeUtf16StringWriter_ELEM          DeeUtf16StringWriter_Elem
#define DeeUtf16StringWriter_SIZE          DeeUtf16StringWriter_Size
#define DeeUtf16StringWriter_ALLOCATED     DeeUtf16StringWriter_Allocated
#endif
#define DeeUtf16StringWriter_WRITE_STRING                      DeeUtf16StringWriter_WRITE_UTF8_STRING
#define DeeUtf16StringWriter_WRITE_MY_STRING                   DeeUtf16StringWriter_WRITE_UTF16_STRING
#define DeeUtf16StringWriter_WRITE_WIDE_STRING(self,s)         DeeUtf16StringWriter_WriteWideStringWithLength(self,(sizeof(s)/sizeof(Dee_WideChar))-1,s)
#define DeeUtf16StringWriter_WRITE_UTF8_STRING(self,s)         DeeUtf16StringWriter_WriteUtf8StringWithLength(self,(sizeof(s)/sizeof(Dee_Utf8Char))-1,s)
#define DeeUtf16StringWriter_WRITE_UTF16_STRING(self,s)        DeeUtf16StringWriter_WriteUtf16StringWithLength(self,(sizeof(s)/sizeof(Dee_Utf16Char))-1,s)
#define DeeUtf16StringWriter_WRITE_UTF32_STRING(self,s)        DeeUtf16StringWriter_WriteUtf32StringWithLength(self,(sizeof(s)/sizeof(Dee_Utf32Char))-1,s)
#define DeeUtf16StringWriter_WriteString                       DeeUtf16StringWriter_WriteUtf8String
#define DeeUtf16StringWriter_WriteMyString                     DeeUtf16StringWriter_WriteUtf16String
#define DeeUtf16StringWriter_WriteStringWithLength             DeeUtf16StringWriter_WriteUtf8StringWithLength
#define DeeUtf16StringWriter_WriteMyStringWithLength           DeeUtf16StringWriter_WriteUtf16StringWithLength
#define DeeUtf16StringWriter_WriteChar                         DeeUtf16StringWriter_WriteUtf8Char
#define DeeUtf16StringWriter_WriteMyChar                       DeeUtf16StringWriter_WriteUtf16Char
#define DeeUtf16StringWriter_WriteStringEx                     DeeUtf16StringWriter_WriteUtf8StringEx
#define DeeUtf16StringWriter_WriteMyStringEx                   DeeUtf16StringWriter_WriteUtf16StringEx
#define DeeUtf16StringWriter_WriteStringWithLengthEx           DeeUtf16StringWriter_WriteUtf8StringWithLengthEx
#define DeeUtf16StringWriter_WriteMyStringWithLengthEx         DeeUtf16StringWriter_WriteUtf16StringWithLengthEx
#define DeeUtf16StringWriter_WriteCharEx                       DeeUtf16StringWriter_WriteUtf8CharEx
#define DeeUtf16StringWriter_WriteMyCharEx                     DeeUtf16StringWriter_WriteUtf16CharEx
#define DeeUtf16StringWriter_WriteQuotedString                 DeeUtf16StringWriter_WriteQuotedUtf8String
#define DeeUtf16StringWriter_WriteQuotedMyString               DeeUtf16StringWriter_WriteQuotedUtf16String
#define DeeUtf16StringWriter_WriteQuotedStringWithLength       DeeUtf16StringWriter_WriteQuotedUtf8StringWithLength
#define DeeUtf16StringWriter_WriteQuotedMyStringWithLength     DeeUtf16StringWriter_WriteQuotedUtf16StringWithLength
#define DeeUtf16StringWriter_SpecWriteString                   DeeUtf16StringWriter_SpecWriteUtf8String
#define DeeUtf16StringWriter_SpecWriteMyString                 DeeUtf16StringWriter_SpecWriteUtf16String
#define DeeUtf16StringWriter_SpecWriteStringWithLength         DeeUtf16StringWriter_SpecWriteUtf8StringWithLength
#define DeeUtf16StringWriter_SpecWriteMyStringWithLength       DeeUtf16StringWriter_SpecWriteUtf16StringWithLength
#define DeeUtf16StringWriter_SpecWriteChar                     DeeUtf16StringWriter_SpecWriteUtf8Char
#define DeeUtf16StringWriter_SpecWriteMyChar                   DeeUtf16StringWriter_SpecWriteUtf16Char
#define DeeUtf16StringWriter_SpecWriteStringEx                 DeeUtf16StringWriter_SpecWriteUtf8StringEx
#define DeeUtf16StringWriter_SpecWriteMyStringEx               DeeUtf16StringWriter_SpecWriteUtf16StringEx
#define DeeUtf16StringWriter_SpecWriteStringWithLengthEx       DeeUtf16StringWriter_SpecWriteUtf8StringWithLengthEx
#define DeeUtf16StringWriter_SpecWriteMyStringWithLengthEx     DeeUtf16StringWriter_SpecWriteUtf16StringWithLengthEx
#define DeeUtf16StringWriter_SpecWriteCharEx                   DeeUtf16StringWriter_SpecWriteUtf8CharEx
#define DeeUtf16StringWriter_SpecWriteMyCharEx                 DeeUtf16StringWriter_SpecWriteUtf16CharEx
#define DeeUtf16StringWriter_SpecWriteQuotedString             DeeUtf16StringWriter_SpecWriteQuotedUtf8String
#define DeeUtf16StringWriter_SpecWriteQuotedMyString           DeeUtf16StringWriter_SpecWriteQuotedUtf16String
#define DeeUtf16StringWriter_SpecWriteQuotedStringWithLength   DeeUtf16StringWriter_SpecWriteQuotedUtf8StringWithLength
#define DeeUtf16StringWriter_SpecWriteQuotedMyStringWithLength DeeUtf16StringWriter_SpecWriteQuotedUtf16StringWithLength
#define DeeUtf16StringWriter_WriteWideStringWithLength(ob,len,s)           DeeUtf16StringWriter_WriteWideStringWithLengthEx(ob,len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_WriteUtf8StringWithLength(ob,len,s)           DeeUtf16StringWriter_WriteUtf8StringWithLengthEx(ob,len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_WriteUtf16StringWithLength(ob,len,s)          DeeUtf16StringWriter_WriteUtf16StringWithLengthEx(ob,len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_WriteUtf32StringWithLength(ob,len,s)          DeeUtf16StringWriter_WriteUtf32StringWithLengthEx(ob,len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_WriteWideChar(ob,ch)                          DeeUtf16StringWriter_WriteWideCharEx(ob,ch,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_WriteUtf8Char(ob,ch)                          DeeUtf16StringWriter_WriteUtf8CharEx(ob,ch,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_WriteUtf16Char(ob,ch)                         DeeUtf16StringWriter_WriteUtf16CharEx(ob,ch,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_WriteUtf32Char(ob,ch)                         DeeUtf16StringWriter_WriteUtf32CharEx(ob,ch,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_WriteWideString(ob,s)                         DeeUtf16StringWriter_WriteWideStringEx(ob,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_WriteUtf8String(ob,s)                         DeeUtf16StringWriter_WriteUtf8StringEx(ob,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_WriteUtf16String(ob,s)                        DeeUtf16StringWriter_WriteUtf16StringEx(ob,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_WriteUtf32String(ob,s)                        DeeUtf16StringWriter_WriteUtf32StringEx(ob,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_SpecWriteWideStringWithLength(ob,len,s,spec)  DeeUtf16StringWriter_SpecWriteWideStringWithLengthEx(ob,len,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_SpecWriteUtf8StringWithLength(ob,len,s,spec)  DeeUtf16StringWriter_SpecWriteUtf8StringWithLengthEx(ob,len,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_SpecWriteUtf16StringWithLength(ob,len,s,spec) DeeUtf16StringWriter_SpecWriteUtf16StringWithLengthEx(ob,len,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_SpecWriteUtf32StringWithLength(ob,len,s,spec) DeeUtf16StringWriter_SpecWriteUtf32StringWithLengthEx(ob,len,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_SpecWriteWideChar(ob,ch,spec)                 DeeUtf16StringWriter_SpecWriteWideCharEx(ob,ch,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_SpecWriteUtf8Char(ob,ch,spec)                 DeeUtf16StringWriter_SpecWriteUtf8CharEx(ob,ch,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_SpecWriteUtf16Char(ob,ch,spec)                DeeUtf16StringWriter_SpecWriteUtf16CharEx(ob,ch,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_SpecWriteUtf32Char(ob,ch,spec)                DeeUtf16StringWriter_SpecWriteUtf32CharEx(ob,ch,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_SpecWriteWideString(ob,s,spec)                DeeUtf16StringWriter_SpecWriteWideStringEx(ob,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_SpecWriteUtf8String(ob,s,spec)                DeeUtf16StringWriter_SpecWriteUtf8StringEx(ob,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_SpecWriteUtf16String(ob,s,spec)               DeeUtf16StringWriter_SpecWriteUtf16StringEx(ob,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf16StringWriter_SpecWriteUtf32String(ob,s,spec)               DeeUtf16StringWriter_SpecWriteUtf32StringEx(ob,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */

#if DEE_CONFIG_HAVE_ENCODING_UTF32
#ifdef DEE_LIMITED_DEX
#define DeeUtf32StringWriter_INIT()        {NULL,0}
#define DeeUtf32StringWriter_STR(ob)       ((ob)->sw_str?DeeUtf32String_STR((ob)->sw_str):NULL)
#define DeeUtf32StringWriter_EMPTY(ob)     ((ob)->sw_str==NULL)
#define DeeUtf32StringWriter_ELEM(ob)      DeeUtf32String_STR((ob)->sw_str)
#define DeeUtf32StringWriter_SIZE(ob)      (ob)->sw_len
#define DeeUtf32StringWriter_ALLOCATED(ob) ((ob)->sw_str?DeeUtf32String_SIZE((ob)->sw_str):0)
#else
#define DeeUtf32StringWriter_STR           DeeUtf32StringWriter_Str
#define DeeUtf32StringWriter_EMPTY         DeeUtf32StringWriter_Empty
#define DeeUtf32StringWriter_ELEM          DeeUtf32StringWriter_Elem
#define DeeUtf32StringWriter_SIZE          DeeUtf32StringWriter_Size
#define DeeUtf32StringWriter_ALLOCATED     DeeUtf32StringWriter_Allocated
#endif
#define DeeUtf32StringWriter_WRITE_STRING                      DeeUtf32StringWriter_WRITE_UTF8_STRING
#define DeeUtf32StringWriter_WRITE_MY_STRING                   DeeUtf32StringWriter_WRITE_UTF32_STRING
#define DeeUtf32StringWriter_WRITE_WIDE_STRING(self,s)         DeeUtf32StringWriter_WriteWideStringWithLength(self,(sizeof(s)/sizeof(Dee_WideChar))-1,s)
#define DeeUtf32StringWriter_WRITE_UTF8_STRING(self,s)         DeeUtf32StringWriter_WriteUtf8StringWithLength(self,(sizeof(s)/sizeof(Dee_Utf8Char))-1,s)
#define DeeUtf32StringWriter_WRITE_UTF16_STRING(self,s)        DeeUtf32StringWriter_WriteUtf16StringWithLength(self,(sizeof(s)/sizeof(Dee_Utf16Char))-1,s)
#define DeeUtf32StringWriter_WRITE_UTF32_STRING(self,s)        DeeUtf32StringWriter_WriteUtf32StringWithLength(self,(sizeof(s)/sizeof(Dee_Utf32Char))-1,s)
#define DeeUtf32StringWriter_WriteString                       DeeUtf32StringWriter_WriteUtf8String
#define DeeUtf32StringWriter_WriteMyString                     DeeUtf32StringWriter_WriteUtf32String
#define DeeUtf32StringWriter_WriteStringWithLength             DeeUtf32StringWriter_WriteUtf8StringWithLength
#define DeeUtf32StringWriter_WriteMyStringWithLength           DeeUtf32StringWriter_WriteUtf32StringWithLength
#define DeeUtf32StringWriter_WriteChar                         DeeUtf32StringWriter_WriteUtf8Char
#define DeeUtf32StringWriter_WriteMyChar                       DeeUtf32StringWriter_WriteUtf32Char
#define DeeUtf32StringWriter_WriteStringEx                     DeeUtf32StringWriter_WriteUtf8StringEx
#define DeeUtf32StringWriter_WriteMyStringEx                   DeeUtf32StringWriter_WriteUtf32StringEx
#define DeeUtf32StringWriter_WriteStringWithLengthEx           DeeUtf32StringWriter_WriteUtf8StringWithLengthEx
#define DeeUtf32StringWriter_WriteMyStringWithLengthEx         DeeUtf32StringWriter_WriteUtf32StringWithLengthEx
#define DeeUtf32StringWriter_WriteCharEx                       DeeUtf32StringWriter_WriteUtf8CharEx
#define DeeUtf32StringWriter_WriteMyCharEx                     DeeUtf32StringWriter_WriteUtf32CharEx
#define DeeUtf32StringWriter_WriteQuotedString                 DeeUtf32StringWriter_WriteQuotedUtf8String
#define DeeUtf32StringWriter_WriteQuotedMyString               DeeUtf32StringWriter_WriteQuotedUtf32String
#define DeeUtf32StringWriter_WriteQuotedStringWithLength       DeeUtf32StringWriter_WriteQuotedUtf8StringWithLength
#define DeeUtf32StringWriter_WriteQuotedMyStringWithLength     DeeUtf32StringWriter_WriteQuotedUtf32StringWithLength
#define DeeUtf32StringWriter_SpecWriteString                   DeeUtf32StringWriter_SpecWriteUtf8String
#define DeeUtf32StringWriter_SpecWriteMyString                 DeeUtf32StringWriter_SpecWriteUtf32String
#define DeeUtf32StringWriter_SpecWriteStringWithLength         DeeUtf32StringWriter_SpecWriteUtf8StringWithLength
#define DeeUtf32StringWriter_SpecWriteMyStringWithLength       DeeUtf32StringWriter_SpecWriteUtf32StringWithLength
#define DeeUtf32StringWriter_SpecWriteChar                     DeeUtf32StringWriter_SpecWriteUtf8Char
#define DeeUtf32StringWriter_SpecWriteMyChar                   DeeUtf32StringWriter_SpecWriteUtf32Char
#define DeeUtf32StringWriter_SpecWriteStringEx                 DeeUtf32StringWriter_SpecWriteUtf8StringEx
#define DeeUtf32StringWriter_SpecWriteMyStringEx               DeeUtf32StringWriter_SpecWriteUtf32StringEx
#define DeeUtf32StringWriter_SpecWriteStringWithLengthEx       DeeUtf32StringWriter_SpecWriteUtf8StringWithLengthEx
#define DeeUtf32StringWriter_SpecWriteMyStringWithLengthEx     DeeUtf32StringWriter_SpecWriteUtf32StringWithLengthEx
#define DeeUtf32StringWriter_SpecWriteCharEx                   DeeUtf32StringWriter_SpecWriteUtf8CharEx
#define DeeUtf32StringWriter_SpecWriteMyCharEx                 DeeUtf32StringWriter_SpecWriteUtf32CharEx
#define DeeUtf32StringWriter_SpecWriteQuotedString             DeeUtf32StringWriter_SpecWriteQuotedUtf8String
#define DeeUtf32StringWriter_SpecWriteQuotedMyString           DeeUtf32StringWriter_SpecWriteQuotedUtf32String
#define DeeUtf32StringWriter_SpecWriteQuotedStringWithLength   DeeUtf32StringWriter_SpecWriteQuotedUtf8StringWithLength
#define DeeUtf32StringWriter_SpecWriteQuotedMyStringWithLength DeeUtf32StringWriter_SpecWriteQuotedUtf32StringWithLength
#define DeeUtf32StringWriter_WriteWideStringWithLength(ob,len,s)           DeeUtf32StringWriter_WriteWideStringWithLengthEx(ob,len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_WriteUtf8StringWithLength(ob,len,s)           DeeUtf32StringWriter_WriteUtf8StringWithLengthEx(ob,len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_WriteUtf16StringWithLength(ob,len,s)          DeeUtf32StringWriter_WriteUtf16StringWithLengthEx(ob,len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_WriteUtf32StringWithLength(ob,len,s)          DeeUtf32StringWriter_WriteUtf32StringWithLengthEx(ob,len,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_WriteWideChar(ob,ch)                          DeeUtf32StringWriter_WriteWideCharEx(ob,ch,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_WriteUtf8Char(ob,ch)                          DeeUtf32StringWriter_WriteUtf8CharEx(ob,ch,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_WriteUtf16Char(ob,ch)                         DeeUtf32StringWriter_WriteUtf16CharEx(ob,ch,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_WriteUtf32Char(ob,ch)                         DeeUtf32StringWriter_WriteUtf32CharEx(ob,ch,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_WriteWideString(ob,s)                         DeeUtf32StringWriter_WriteWideStringEx(ob,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_WriteUtf8String(ob,s)                         DeeUtf32StringWriter_WriteUtf8StringEx(ob,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_WriteUtf16String(ob,s)                        DeeUtf32StringWriter_WriteUtf16StringEx(ob,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_WriteUtf32String(ob,s)                        DeeUtf32StringWriter_WriteUtf32StringEx(ob,s,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_SpecWriteWideStringWithLength(ob,len,s,spec)  DeeUtf32StringWriter_SpecWriteWideStringWithLengthEx(ob,len,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_SpecWriteUtf8StringWithLength(ob,len,s,spec)  DeeUtf32StringWriter_SpecWriteUtf8StringWithLengthEx(ob,len,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_SpecWriteUtf16StringWithLength(ob,len,s,spec) DeeUtf32StringWriter_SpecWriteUtf16StringWithLengthEx(ob,len,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_SpecWriteUtf32StringWithLength(ob,len,s,spec) DeeUtf32StringWriter_SpecWriteUtf32StringWithLengthEx(ob,len,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_SpecWriteWideChar(ob,ch,spec)                 DeeUtf32StringWriter_SpecWriteWideCharEx(ob,ch,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_SpecWriteUtf8Char(ob,ch,spec)                 DeeUtf32StringWriter_SpecWriteUtf8CharEx(ob,ch,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_SpecWriteUtf16Char(ob,ch,spec)                DeeUtf32StringWriter_SpecWriteUtf16CharEx(ob,ch,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_SpecWriteUtf32Char(ob,ch,spec)                DeeUtf32StringWriter_SpecWriteUtf32CharEx(ob,ch,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_SpecWriteWideString(ob,s,spec)                DeeUtf32StringWriter_SpecWriteWideStringEx(ob,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_SpecWriteUtf8String(ob,s,spec)                DeeUtf32StringWriter_SpecWriteUtf8StringEx(ob,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_SpecWriteUtf16String(ob,s,spec)               DeeUtf32StringWriter_SpecWriteUtf16StringEx(ob,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#define DeeUtf32StringWriter_SpecWriteUtf32String(ob,s,spec)               DeeUtf32StringWriter_SpecWriteUtf32StringEx(ob,s,spec,DEE_STRING_DECODE_FLAG_DEFAULT)
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */


//////////////////////////////////////////////////////////////////////////
// DeeAnyStringObject API
#if DEE_CONFIG_HAVE_ENCODING_WIDE && DEE_CONFIG_HAVE_ENCODING_UTF8 && DEE_CONFIG_HAVE_ENCODING_UTF16 && DEE_CONFIG_HAVE_ENCODING_UTF32
#define DeeAnyString_Check(ob) (DeeWideString_Check(ob)||DeeUtf8String_Check(ob)||DeeUtf16String_Check(ob)||DeeUtf32String_Check(ob))
#elif DEE_CONFIG_HAVE_ENCODING_WIDE && DEE_CONFIG_HAVE_ENCODING_UTF8 && DEE_CONFIG_HAVE_ENCODING_UTF16 && !DEE_CONFIG_HAVE_ENCODING_UTF32
#define DeeAnyString_Check(ob) (DeeWideString_Check(ob)||DeeUtf8String_Check(ob)||DeeUtf16String_Check(ob))
#elif DEE_CONFIG_HAVE_ENCODING_WIDE && DEE_CONFIG_HAVE_ENCODING_UTF8 && !DEE_CONFIG_HAVE_ENCODING_UTF16 && DEE_CONFIG_HAVE_ENCODING_UTF32
#define DeeAnyString_Check(ob) (DeeWideString_Check(ob)||DeeUtf8String_Check(ob)||DeeUtf32String_Check(ob))
#elif DEE_CONFIG_HAVE_ENCODING_WIDE && DEE_CONFIG_HAVE_ENCODING_UTF8 && !DEE_CONFIG_HAVE_ENCODING_UTF16 && !DEE_CONFIG_HAVE_ENCODING_UTF32
#define DeeAnyString_Check(ob) (DeeWideString_Check(ob)||DeeUtf8String_Check(ob))
#elif DEE_CONFIG_HAVE_ENCODING_WIDE && !DEE_CONFIG_HAVE_ENCODING_UTF8 && DEE_CONFIG_HAVE_ENCODING_UTF16 && DEE_CONFIG_HAVE_ENCODING_UTF32
#define DeeAnyString_Check(ob) (DeeWideString_Check(ob)||DeeUtf16String_Check(ob)||DeeUtf32String_Check(ob))
#elif DEE_CONFIG_HAVE_ENCODING_WIDE && !DEE_CONFIG_HAVE_ENCODING_UTF8 && DEE_CONFIG_HAVE_ENCODING_UTF16 && !DEE_CONFIG_HAVE_ENCODING_UTF32
#define DeeAnyString_Check(ob) (DeeWideString_Check(ob)||DeeUtf16String_Check(ob))
#elif DEE_CONFIG_HAVE_ENCODING_WIDE && !DEE_CONFIG_HAVE_ENCODING_UTF8 && !DEE_CONFIG_HAVE_ENCODING_UTF16 && DEE_CONFIG_HAVE_ENCODING_UTF32
#define DeeAnyString_Check(ob) (DeeWideString_Check(ob)||DeeUtf32String_Check(ob))
#elif DEE_CONFIG_HAVE_ENCODING_WIDE && !DEE_CONFIG_HAVE_ENCODING_UTF8 && !DEE_CONFIG_HAVE_ENCODING_UTF16 && !DEE_CONFIG_HAVE_ENCODING_UTF32
#define DeeAnyString_Check(ob)  DeeWideString_Check(ob)
#elif !DEE_CONFIG_HAVE_ENCODING_WIDE && DEE_CONFIG_HAVE_ENCODING_UTF8 && DEE_CONFIG_HAVE_ENCODING_UTF16 && DEE_CONFIG_HAVE_ENCODING_UTF32
#define DeeAnyString_Check(ob) (DeeUtf8String_Check(ob)||DeeUtf16String_Check(ob)||DeeUtf32String_Check(ob))
#elif !DEE_CONFIG_HAVE_ENCODING_WIDE && DEE_CONFIG_HAVE_ENCODING_UTF8 && DEE_CONFIG_HAVE_ENCODING_UTF16 && !DEE_CONFIG_HAVE_ENCODING_UTF32
#define DeeAnyString_Check(ob) (DeeUtf8String_Check(ob)||DeeUtf16String_Check(ob))
#elif !DEE_CONFIG_HAVE_ENCODING_WIDE && DEE_CONFIG_HAVE_ENCODING_UTF8 && !DEE_CONFIG_HAVE_ENCODING_UTF16 && DEE_CONFIG_HAVE_ENCODING_UTF32
#define DeeAnyString_Check(ob) (DeeUtf8String_Check(ob)||DeeUtf32String_Check(ob))
#elif !DEE_CONFIG_HAVE_ENCODING_WIDE && DEE_CONFIG_HAVE_ENCODING_UTF8 && !DEE_CONFIG_HAVE_ENCODING_UTF16 && !DEE_CONFIG_HAVE_ENCODING_UTF32
#define DeeAnyString_Check(ob) (DeeUtf8String_Check(ob))
#elif !DEE_CONFIG_HAVE_ENCODING_WIDE && !DEE_CONFIG_HAVE_ENCODING_UTF8 && DEE_CONFIG_HAVE_ENCODING_UTF16 && DEE_CONFIG_HAVE_ENCODING_UTF32
#define DeeAnyString_Check(ob) (DeeUtf16String_Check(ob)||DeeUtf32String_Check(ob))
#elif !DEE_CONFIG_HAVE_ENCODING_WIDE && !DEE_CONFIG_HAVE_ENCODING_UTF8 && DEE_CONFIG_HAVE_ENCODING_UTF16 && !DEE_CONFIG_HAVE_ENCODING_UTF32
#define DeeAnyString_Check(ob) (DeeUtf16String_Check(ob))
#elif !DEE_CONFIG_HAVE_ENCODING_WIDE && !DEE_CONFIG_HAVE_ENCODING_UTF8 && !DEE_CONFIG_HAVE_ENCODING_UTF16 && DEE_CONFIG_HAVE_ENCODING_UTF32
#define DeeAnyString_Check(ob) (DeeUtf32String_Check(ob))
#else /* !DEE_CONFIG_HAVE_ENCODING_WIDE && !DEE_CONFIG_HAVE_ENCODING_UTF8 && !DEE_CONFIG_HAVE_ENCODING_UTF16 && !DEE_CONFIG_HAVE_ENCODING_UTF32 */
#define DeeAnyString_Check(ob)  0
#endif
#define DeeAnyString_CheckExact DeeAnyString_Check

#ifdef DEE_LIMITED_DEX
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeAnyStringObject {
 DEE_OBJECT_HEAD
 Dee_size_t as_len;
 union{
  char          as_str[8192];    /*< [s_len] String. */
#if DEE_CONFIG_HAVE_ENCODING_WIDE
  Dee_WideChar  as_wstr[8192];   /*< [s_len] Wide String */
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
  Dee_Utf8Char  as_u8str[8192];  /*< [s_len] Utf-8 String */
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
  Dee_Utf16Char as_u16str[8192]; /*< [s_len] Utf-16 String */
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
  Dee_Utf32Char as_u32str[8192]; /*< [s_len] Utf-32 String */
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define as_str      _as_strdata.as_str
#if DEE_CONFIG_HAVE_ENCODING_WIDE
#define as_wstr     _as_strdata.as_wstr
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
#define as_u8str    _as_strdata.as_u8str
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
#define as_u16str   _as_strdata.as_u16str
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
#define as_u32str   _as_strdata.as_u32str
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 _as_strdata
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
 ;
};
DEE_COMPILER_MSVC_WARNING_POP
#endif

#define DeeAnyString_STR(ob)  ((void *)DeeString_STR(ob))
#define DeeAnyString_SIZE       DeeString_SIZE

//////////////////////////////////////////////////////////////////////////
// Returns a pointer to the character data of any string type
#define DeeAnyString_DATAV      DeeAnyString_STR

//////////////////////////////////////////////////////////////////////////
// Returns a size (in bytes) of the data portion of any string type
// NOTE: String types store their character's size in their tp_instance_size type field
#define DeeAnyString_DATAC(ob) (DeeAnyString_SIZE(ob)*DeeType_GET_SLOT(Dee_TYPE(ob),tp_instance_size))



//////////////////////////////////////////////////////////////////////////
// String encodings
enum DeeStringEncoding {
 DEE_STRING_ENCODING_NONE     = 0,
 DEE_STRING_ENCODING_UTF8     = 1,
 DEE_STRING_ENCODING_UTF16_LE = 2,
 DEE_STRING_ENCODING_UTF16_BE = 3,
 DEE_STRING_ENCODING_UTF32_LE = 4,
 DEE_STRING_ENCODING_UTF32_BE = 5,
 DEE_STRING_ENCODING_UTF7     = 6,
 // TODO: DEE_STRING_ENCODING_UTF1       = 7,
 // TODO: DEE_STRING_ENCODING_UTF_EBCDIC = 8,
 // TODO: DEE_STRING_ENCODING_SCSU       = 9,
 // TODO: DEE_STRING_ENCODING_BOCU_1     = 10,
 // TODO: DEE_STRING_ENCODING_GB_18030   = 11,
};

//////////////////////////////////////////////////////////////////////////
// Try to determine the encoding of given raw-memory
//  - Returns 'DEE_STRING_ENCODING_NONE' if the encoding could not be determined.
//  - Optionally stores the actual start of the memory '*start' (in case of a prefix)
DEE_FUNC_DECL(DEE_A_RET_WUNUSED enum DeeStringEncoding) DeeAnyString_DetermineEncoding(
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s, DEE_A_OUT_OPT void const **start);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeAnyString_FromEncodingAndData(
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s, DEE_A_IN enum DeeStringEncoding encoding);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeAnyString_FromData(
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s);


//////////////////////////////////////////////////////////////////////////
// Specifications for writing data into a StringWriter with custom formatting
typedef struct DeeStringWriterFormatSpec {
 unsigned int numsys;            // Number system to output in (2 / 8 / 10 / 16)
 unsigned int width;             // Minimum width: %5d
 unsigned int precision;         // Minimum number of digits / absolute number of exponents: %.5d
#ifndef __DEEMON__ /* bitfields aren't supported (yet) */
 unsigned int has_width     : 1; // Enables the width attribute
 unsigned int has_precision : 1; // Enables the precision attribute
 unsigned int left_just     : 1; // Left justify: %-d
 unsigned int sign_pos      : 1; // Output '+' before positive numbers: %+d
 unsigned int sign_space    : 1; // Output ' ' before positive numbers: % d
 unsigned int prefix        : 1; // Output the numsys prefix between sign and number: %#d
 unsigned int pad_zero      : 1; // Output '0' in unused space, instead of ' ' (requires width): %05d
 unsigned int upper_hex     : 1; // Use uppercase for hexadecimal
 unsigned int instab        : 8; // Insert tabs ('\t') after every '\n'
 unsigned int padding       :16;
#else
 Dee_uint32_t padding; /* Still force the correct size, though. */
#endif
} DeeStringWriterFormatSpec;
#ifndef __DEEMON__ /* bitfields... */
#define DeeStringWriterFormatSpec_INIT_EXTENDED(numsys,width,precision) \
 {numsys,width,precision,(width)!=0,(precision)!=0,0,0,0,0,0,0,0,0}
#define DeeStringWriterFormatSpec_INIT_BASIC(numsys) {numsys,0,0,0,0,0,0,0,0,0,0,0,0}
#endif

#ifdef DEE_LIMITED_API
extern DeeStringWriterFormatSpec const dee_decimal_default_spec;
#endif /* DEE_LIMITED_API */

DEE_DECL_END

#ifdef __cplusplus
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_WUNUSED Dee_size_t) Dee_StrLen(DEE_A_IN_Z Dee_WideChar const *str_) { return Dee_WideStrLen(str_); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_WUNUSED Dee_size_t) Dee_StrLen(DEE_A_IN_Z Dee_Utf8Char const *str_) { return Dee_Utf8StrLen(str_); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_WUNUSED Dee_size_t) Dee_StrLen(DEE_A_IN_Z Dee_Utf16Char const *str_) { return Dee_Utf16StrLen(str_); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_WUNUSED Dee_size_t) Dee_StrLen(DEE_A_IN_Z Dee_Utf32Char const *str_) { return Dee_Utf32StrLen(str_); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_WUNUSED Dee_size_t) Dee_StrNLen(DEE_A_IN_R(n) Dee_WideChar const *str_, DEE_A_IN Dee_size_t n) { return Dee_WideStrNLen(str_,n); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_WUNUSED Dee_size_t) Dee_StrNLen(DEE_A_IN_R(n) Dee_Utf8Char const *str_, DEE_A_IN Dee_size_t n) { return Dee_Utf8StrNLen(str_,n); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_WUNUSED Dee_size_t) Dee_StrNLen(DEE_A_IN_R(n) Dee_Utf16Char const *str_, DEE_A_IN Dee_size_t n) { return Dee_Utf16StrNLen(str_,n); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1)) DEE_A_RET_WUNUSED Dee_size_t) Dee_StrNLen(DEE_A_IN_R(n) Dee_Utf32Char const *str_, DEE_A_IN Dee_size_t n) { return Dee_Utf32StrNLen(str_,n); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_WUNUSED int) Dee_StrCmp(DEE_A_IN_Z Dee_WideChar const *a, DEE_A_IN_Z Dee_WideChar const *b) { return Dee_WideStrCmp(a,b); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_WUNUSED int) Dee_StrCmp(DEE_A_IN_Z Dee_Utf8Char const *a, DEE_A_IN_Z Dee_Utf8Char const *b) { return Dee_Utf8StrCmp(a,b); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_WUNUSED int) Dee_StrCmp(DEE_A_IN_Z Dee_Utf16Char const *a, DEE_A_IN_Z Dee_Utf16Char const *b) { return Dee_Utf16StrCmp(a,b); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_WUNUSED int) Dee_StrCmp(DEE_A_IN_Z Dee_Utf32Char const *a, DEE_A_IN_Z Dee_Utf32Char const *b) { return Dee_Utf32StrCmp(a,b); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_WUNUSED int) Dee_StrCaseCmp(DEE_A_IN_Z Dee_WideChar const *a, DEE_A_IN_Z Dee_WideChar const *b) { return Dee_WideStrCaseCmp(a,b); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_WUNUSED int) Dee_StrCaseCmp(DEE_A_IN_Z Dee_Utf8Char const *a, DEE_A_IN_Z Dee_Utf8Char const *b) { return Dee_Utf8StrCaseCmp(a,b); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_WUNUSED int) Dee_StrCaseCmp(DEE_A_IN_Z Dee_Utf16Char const *a, DEE_A_IN_Z Dee_Utf16Char const *b) { return Dee_Utf16StrCaseCmp(a,b); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_WUNUSED int) Dee_StrCaseCmp(DEE_A_IN_Z Dee_Utf32Char const *a, DEE_A_IN_Z Dee_Utf32Char const *b) { return Dee_Utf32StrCaseCmp(a,b); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) Dee_StrCmpLen(DEE_A_IN Dee_size_t a_size, DEE_A_IN_R(a_size) Dee_WideChar const *a, DEE_A_IN Dee_size_t b_size, DEE_A_IN_R(b_size) Dee_WideChar const *b) { return Dee_WideStrCmpLen(a_size,a,b_size,b); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) Dee_StrCmpLen(DEE_A_IN Dee_size_t a_size, DEE_A_IN_R(a_size) Dee_Utf8Char const *a, DEE_A_IN Dee_size_t b_size, DEE_A_IN_R(b_size) Dee_Utf8Char const *b) { return Dee_Utf8StrCmpLen(a_size,a,b_size,b); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) Dee_StrCmpLen(DEE_A_IN Dee_size_t a_size, DEE_A_IN_R(a_size) Dee_Utf16Char const *a, DEE_A_IN Dee_size_t b_size, DEE_A_IN_R(b_size) Dee_Utf16Char const *b) { return Dee_Utf16StrCmpLen(a_size,a,b_size,b); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) Dee_StrCmpLen(DEE_A_IN Dee_size_t a_size, DEE_A_IN_R(a_size) Dee_Utf32Char const *a, DEE_A_IN Dee_size_t b_size, DEE_A_IN_R(b_size) Dee_Utf32Char const *b) { return Dee_Utf32StrCmpLen(a_size,a,b_size,b); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) Dee_StrCaseCmpLen(DEE_A_IN Dee_size_t a_size, DEE_A_IN_R(a_size) Dee_WideChar const *a, DEE_A_IN Dee_size_t b_size, DEE_A_IN_R(b_size) Dee_WideChar const *b) { return Dee_WideStrCaseCmpLen(a_size,a,b_size,b); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) Dee_StrCaseCmpLen(DEE_A_IN Dee_size_t a_size, DEE_A_IN_R(a_size) Dee_Utf8Char const *a, DEE_A_IN Dee_size_t b_size, DEE_A_IN_R(b_size) Dee_Utf8Char const *b) { return Dee_Utf8StrCaseCmpLen(a_size,a,b_size,b); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) Dee_StrCaseCmpLen(DEE_A_IN Dee_size_t a_size, DEE_A_IN_R(a_size) Dee_Utf16Char const *a, DEE_A_IN Dee_size_t b_size, DEE_A_IN_R(b_size) Dee_Utf16Char const *b) { return Dee_Utf16StrCaseCmpLen(a_size,a,b_size,b); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) Dee_StrCaseCmpLen(DEE_A_IN Dee_size_t a_size, DEE_A_IN_R(a_size) Dee_Utf32Char const *a, DEE_A_IN Dee_size_t b_size, DEE_A_IN_R(b_size) Dee_Utf32Char const *b) { return Dee_Utf32StrCaseCmpLen(a_size,a,b_size,b); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_WUNUSED int) Dee_StrWMatch(DEE_A_IN_Z Dee_WideChar const *str_, DEE_A_IN_Z Dee_WideChar const *pat_) { return Dee_WideStrWMatch(str_,pat_); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_WUNUSED int) Dee_StrWMatch(DEE_A_IN_Z Dee_Utf8Char const *str_, DEE_A_IN_Z Dee_Utf8Char const *pat_) { return Dee_Utf8StrWMatch(str_,pat_); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_WUNUSED int) Dee_StrWMatch(DEE_A_IN_Z Dee_Utf16Char const *str_, DEE_A_IN_Z Dee_Utf16Char const *pat_) { return Dee_Utf16StrWMatch(str_,pat_); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_WUNUSED int) Dee_StrWMatch(DEE_A_IN_Z Dee_Utf32Char const *str_, DEE_A_IN_Z Dee_Utf32Char const *pat_) { return Dee_Utf32StrWMatch(str_,pat_); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_WUNUSED int) Dee_StrCaseWMatch(DEE_A_IN_Z Dee_WideChar const *str_, DEE_A_IN_Z Dee_WideChar const *pat_) { return Dee_WideStrCaseWMatch(str_,pat_); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_WUNUSED int) Dee_StrCaseWMatch(DEE_A_IN_Z Dee_Utf8Char const *str_, DEE_A_IN_Z Dee_Utf8Char const *pat_) { return Dee_Utf8StrCaseWMatch(str_,pat_); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_WUNUSED int) Dee_StrCaseWMatch(DEE_A_IN_Z Dee_Utf16Char const *str_, DEE_A_IN_Z Dee_Utf16Char const *pat_) { return Dee_Utf16StrCaseWMatch(str_,pat_); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_RET_WUNUSED int) Dee_StrCaseWMatch(DEE_A_IN_Z Dee_Utf32Char const *str_, DEE_A_IN_Z Dee_Utf32Char const *pat_) { return Dee_Utf32StrCaseWMatch(str_,pat_); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) Dee_StrWMatchLen(DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) Dee_WideChar const *str_, DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) Dee_WideChar const *pat_) { return Dee_WideStrWMatchLen(str_size,str_,pat_size,pat_); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) Dee_StrWMatchLen(DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) Dee_Utf8Char const *str_, DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) Dee_Utf8Char const *pat_) { return Dee_Utf8StrWMatchLen(str_size,str_,pat_size,pat_); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) Dee_StrWMatchLen(DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) Dee_Utf16Char const *str_, DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) Dee_Utf16Char const *pat_) { return Dee_Utf16StrWMatchLen(str_size,str_,pat_size,pat_); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) Dee_StrWMatchLen(DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) Dee_Utf32Char const *str_, DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) Dee_Utf32Char const *pat_) { return Dee_Utf32StrWMatchLen(str_size,str_,pat_size,pat_); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) Dee_StrCaseWMatchLen(DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) Dee_WideChar const *str_, DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) Dee_WideChar const *pat_) { return Dee_WideStrCaseWMatchLen(str_size,str_,pat_size,pat_); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) Dee_StrCaseWMatchLen(DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) Dee_Utf8Char const *str_, DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) Dee_Utf8Char const *pat_) { return Dee_Utf8StrCaseWMatchLen(str_size,str_,pat_size,pat_); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) Dee_StrCaseWMatchLen(DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) Dee_Utf16Char const *str_, DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) Dee_Utf16Char const *pat_) { return Dee_Utf16StrCaseWMatchLen(str_size,str_,pat_size,pat_); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) Dee_StrCaseWMatchLen(DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) Dee_Utf32Char const *str_, DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) Dee_Utf32Char const *pat_) { return Dee_Utf32StrCaseWMatchLen(str_size,str_,pat_size,pat_); }
#else
#define Dee_StrLen           Dee_Utf8StrLen
#define Dee_StrNLen          Dee_Utf8StrNLen
#define Dee_StrCmp           Dee_Utf8StrCmp
#define Dee_StrCaseCmp       Dee_Utf8StrCaseCmp
#define Dee_StrCmpLen        Dee_Utf8StrCmpLen
#define Dee_StrCaseCmpLen    Dee_Utf8StrCaseCmpLen
#define Dee_StrWMatch        Dee_Utf8StrWMatch
#define Dee_StrCaseWMatch    Dee_Utf8StrCaseWMatch
#define Dee_StrWMatchLen     Dee_Utf8StrWMatchLen
#define Dee_StrCaseWMatchLen Dee_Utf8StrCaseWMatchLen
#endif

#if !defined(__INTELLISENSE__) || 1
#if DEE_CONFIG_HAVE_ENCODING_WIDE
#define DEE_STRING_ENC Wide
#include "__string_api.inl"
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF8
#define DEE_STRING_ENC Utf8
#include "__string_api.inl"
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
#define DEE_STRING_ENC Utf16
#include "__string_api.inl"
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
#define DEE_STRING_ENC Utf32
#include "__string_api.inl"
#endif
#endif

//////////////////////////////////////////////////////////////////////////
// C++ Wrapper for overloading function functions
//////////////////////////////////////////////////////////////////////////
/*[[[deemon
encodings = ["Wide","Utf8","Utf16","Utf32"];
print "#ifdef __cplusplus";
for (local enc: encodings) {
  print "#undef Dee"+enc+"String_FromStringEx";
  print "#undef Dee"+enc+"String_FromStringWithLengthEx";
  print "#undef Dee"+enc+"String_FromQuotedString";
  print "#undef Dee"+enc+"String_FromQuotedStringWithLength";
  print "#undef Dee"+enc+"StringWriter_WRITE_STRING";
  print "#undef Dee"+enc+"StringWriter_WriteStringEx";
  print "#undef Dee"+enc+"StringWriter_WriteStringWithLengthEx";
  print "#undef Dee"+enc+"StringWriter_WriteCharEx";
  print "#undef Dee"+enc+"StringWriter_WriteQuotedString";
  print "#undef Dee"+enc+"StringWriter_WriteQuotedStringWithLength";
  print "#undef Dee"+enc+"StringWriter_SpecWriteStringEx";
  print "#undef Dee"+enc+"StringWriter_SpecWriteStringWithLengthEx";
  print "#undef Dee"+enc+"StringWriter_SpecWriteCharEx";
  print "#undef Dee"+enc+"StringWriter_SpecWriteQuotedString";
  print "#undef Dee"+enc+"StringWriter_SpecWriteQuotedStringWithLength";
  for (local enc2: encodings) print "DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(Dee"+enc+"StringObject) *) Dee"+enc+"String_FromStringEx(DEE_A_IN_Z Dee_"+enc2+"Char const *s, DEE_A_IN Dee_uint32_t flags) { return Dee"+enc+"String_From"+enc2+"StringEx(s,flags); }";
  for (local enc2: encodings) print "DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(Dee"+enc+"StringObject) *) Dee"+enc+"String_FromStringWithLengthEx(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_"+enc2+"Char const *s, DEE_A_IN Dee_uint32_t flags) { return Dee"+enc+"String_From"+enc2+"StringWithLengthEx(len,s,flags); }";
  for (local enc2: encodings) print "DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(Dee"+enc+"StringObject) *) Dee"+enc+"String_FromQuotedString(DEE_A_IN_Z Dee_"+enc2+"Char const *s) { return Dee"+enc+"String_FromQuoted"+enc2+"String(s); }";
  for (local enc2: encodings) print "DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(Dee"+enc+"StringObject) *) Dee"+enc+"String_FromQuotedStringWithLength(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_"+enc2+"Char const *s) { return Dee"+enc+"String_FromQuoted"+enc2+"StringWithLength(len,s); }";
  for (local enc2: encodings) print "template<Dee_size_t len> DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) Dee"+enc+"StringWriter_WRITE_STRING(DEE_A_INOUT Dee"+enc+"StringWriter *self, Dee_"+enc2+"Char const (&s)[len]) { return Dee"+enc+"StringWriter_WRITE_"+enc2.upper()+"_STRING(self,s); }";
  for (local enc2: encodings) print "DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) Dee"+enc+"StringWriter_WriteStringEx(DEE_A_INOUT Dee"+enc+"StringWriter *self, DEE_A_IN_Z Dee_"+enc2+"Char const *s, DEE_A_IN Dee_uint32_t flags) { return Dee"+enc+"StringWriter_Write"+enc2+"StringEx(self,s,flags); }";
  for (local enc2: encodings) print "DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) Dee"+enc+"StringWriter_WriteStringWithLengthEx(DEE_A_INOUT Dee"+enc+"StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_"+enc2+"Char const *s, DEE_A_IN Dee_uint32_t flags) { return Dee"+enc+"StringWriter_Write"+enc2+"StringWithLengthEx(self,len,s,flags); }";
  for (local enc2: encodings) print "DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) Dee"+enc+"StringWriter_WriteCharEx(DEE_A_INOUT Dee"+enc+"StringWriter *self, DEE_A_IN Dee_"+enc2+"Char ch, DEE_A_IN Dee_uint32_t flags) { return Dee"+enc+"StringWriter_Write"+enc2+"CharEx(self,ch,flags); }";
  for (local enc2: encodings) print "DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) Dee"+enc+"StringWriter_WriteQuotedString(DEE_A_INOUT Dee"+enc+"StringWriter *self, DEE_A_IN_Z Dee_"+enc2+"Char const *s) { return Dee"+enc+"StringWriter_WriteQuoted"+enc2+"String(self,s); }";
  for (local enc2: encodings) print "DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) Dee"+enc+"StringWriter_WriteQuotedStringWithLength(DEE_A_INOUT Dee"+enc+"StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_"+enc2+"Char const *s) { return Dee"+enc+"StringWriter_WriteQuoted"+enc2+"StringWithLength(self,len,s); }";
  for (local enc2: encodings) print "DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) Dee"+enc+"StringWriter_SpecWriteStringEx(DEE_A_INOUT Dee"+enc+"StringWriter *self, DEE_A_IN_Z Dee_"+enc2+"Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return Dee"+enc+"StringWriter_SpecWrite"+enc2+"StringEx(self,s,spec,flags); }";
  for (local enc2: encodings) print "DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) Dee"+enc+"StringWriter_SpecWriteStringWithLengthEx(DEE_A_INOUT Dee"+enc+"StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_"+enc2+"Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return Dee"+enc+"StringWriter_SpecWrite"+enc2+"StringWithLengthEx(self,len,s,spec,flags); }";
  for (local enc2: encodings) print "DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) Dee"+enc+"StringWriter_SpecWriteCharEx(DEE_A_INOUT Dee"+enc+"StringWriter *self, DEE_A_IN Dee_"+enc2+"Char ch, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return Dee"+enc+"StringWriter_SpecWrite"+enc2+"CharEx(self,ch,spec,flags); }";
  for (local enc2: encodings) print "DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) Dee"+enc+"StringWriter_SpecWriteQuotedString(DEE_A_INOUT Dee"+enc+"StringWriter *self, DEE_A_IN_Z Dee_"+enc2+"Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return Dee"+enc+"StringWriter_SpecWriteQuoted"+enc2+"String(self,s,spec); }";
  for (local enc2: encodings) print "DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) Dee"+enc+"StringWriter_SpecWriteQuotedStringWithLength(DEE_A_INOUT Dee"+enc+"StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_"+enc2+"Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return Dee"+enc+"StringWriter_SpecWriteQuoted"+enc2+"StringWithLength(self,len,s,spec); }";
}
print "#endif";
]]]*/
#ifdef __cplusplus
#undef DeeWideString_FromStringEx
#undef DeeWideString_FromStringWithLengthEx
#undef DeeWideString_FromQuotedString
#undef DeeWideString_FromQuotedStringWithLength
#undef DeeWideStringWriter_WRITE_STRING
#undef DeeWideStringWriter_WriteStringEx
#undef DeeWideStringWriter_WriteStringWithLengthEx
#undef DeeWideStringWriter_WriteCharEx
#undef DeeWideStringWriter_WriteQuotedString
#undef DeeWideStringWriter_WriteQuotedStringWithLength
#undef DeeWideStringWriter_SpecWriteStringEx
#undef DeeWideStringWriter_SpecWriteStringWithLengthEx
#undef DeeWideStringWriter_SpecWriteCharEx
#undef DeeWideStringWriter_SpecWriteQuotedString
#undef DeeWideStringWriter_SpecWriteQuotedStringWithLength
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWideString_FromStringEx(DEE_A_IN_Z Dee_WideChar const *s, DEE_A_IN Dee_uint32_t flags) { return DeeWideString_FromWideStringEx(s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWideString_FromStringEx(DEE_A_IN_Z Dee_Utf8Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeWideString_FromUtf8StringEx(s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWideString_FromStringEx(DEE_A_IN_Z Dee_Utf16Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeWideString_FromUtf16StringEx(s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWideString_FromStringEx(DEE_A_IN_Z Dee_Utf32Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeWideString_FromUtf32StringEx(s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWideString_FromStringWithLengthEx(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s, DEE_A_IN Dee_uint32_t flags) { return DeeWideString_FromWideStringWithLengthEx(len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWideString_FromStringWithLengthEx(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeWideString_FromUtf8StringWithLengthEx(len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWideString_FromStringWithLengthEx(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeWideString_FromUtf16StringWithLengthEx(len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWideString_FromStringWithLengthEx(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeWideString_FromUtf32StringWithLengthEx(len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWideString_FromQuotedString(DEE_A_IN_Z Dee_WideChar const *s) { return DeeWideString_FromQuotedWideString(s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWideString_FromQuotedString(DEE_A_IN_Z Dee_Utf8Char const *s) { return DeeWideString_FromQuotedUtf8String(s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWideString_FromQuotedString(DEE_A_IN_Z Dee_Utf16Char const *s) { return DeeWideString_FromQuotedUtf16String(s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWideString_FromQuotedString(DEE_A_IN_Z Dee_Utf32Char const *s) { return DeeWideString_FromQuotedUtf32String(s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWideString_FromQuotedStringWithLength(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s) { return DeeWideString_FromQuotedWideStringWithLength(len,s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWideString_FromQuotedStringWithLength(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s) { return DeeWideString_FromQuotedUtf8StringWithLength(len,s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWideString_FromQuotedStringWithLength(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s) { return DeeWideString_FromQuotedUtf16StringWithLength(len,s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeWideString_FromQuotedStringWithLength(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s) { return DeeWideString_FromQuotedUtf32StringWithLength(len,s); }
template<Dee_size_t len> DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WRITE_STRING(DEE_A_INOUT DeeWideStringWriter *self, Dee_WideChar const (&s)[len]) { return DeeWideStringWriter_WRITE_WIDE_STRING(self,s); }
template<Dee_size_t len> DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WRITE_STRING(DEE_A_INOUT DeeWideStringWriter *self, Dee_Utf8Char const (&s)[len]) { return DeeWideStringWriter_WRITE_UTF8_STRING(self,s); }
template<Dee_size_t len> DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WRITE_STRING(DEE_A_INOUT DeeWideStringWriter *self, Dee_Utf16Char const (&s)[len]) { return DeeWideStringWriter_WRITE_UTF16_STRING(self,s); }
template<Dee_size_t len> DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WRITE_STRING(DEE_A_INOUT DeeWideStringWriter *self, Dee_Utf32Char const (&s)[len]) { return DeeWideStringWriter_WRITE_UTF32_STRING(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteStringEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN_Z Dee_WideChar const *s, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_WriteWideStringEx(self,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteStringEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN_Z Dee_Utf8Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_WriteUtf8StringEx(self,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteStringEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN_Z Dee_Utf16Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_WriteUtf16StringEx(self,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteStringEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN_Z Dee_Utf32Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_WriteUtf32StringEx(self,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteStringWithLengthEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_WriteWideStringWithLengthEx(self,len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteStringWithLengthEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_WriteUtf8StringWithLengthEx(self,len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteStringWithLengthEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_WriteUtf16StringWithLengthEx(self,len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteStringWithLengthEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_WriteUtf32StringWithLengthEx(self,len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteCharEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_WideChar ch, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_WriteWideCharEx(self,ch,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteCharEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_Utf8Char ch, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_WriteUtf8CharEx(self,ch,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteCharEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_Utf16Char ch, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_WriteUtf16CharEx(self,ch,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteCharEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_Utf32Char ch, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_WriteUtf32CharEx(self,ch,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteQuotedString(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN_Z Dee_WideChar const *s) { return DeeWideStringWriter_WriteQuotedWideString(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteQuotedString(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN_Z Dee_Utf8Char const *s) { return DeeWideStringWriter_WriteQuotedUtf8String(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteQuotedString(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN_Z Dee_Utf16Char const *s) { return DeeWideStringWriter_WriteQuotedUtf16String(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteQuotedString(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN_Z Dee_Utf32Char const *s) { return DeeWideStringWriter_WriteQuotedUtf32String(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteQuotedStringWithLength(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s) { return DeeWideStringWriter_WriteQuotedWideStringWithLength(self,len,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteQuotedStringWithLength(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s) { return DeeWideStringWriter_WriteQuotedUtf8StringWithLength(self,len,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteQuotedStringWithLength(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s) { return DeeWideStringWriter_WriteQuotedUtf16StringWithLength(self,len,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_WriteQuotedStringWithLength(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s) { return DeeWideStringWriter_WriteQuotedUtf32StringWithLength(self,len,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteStringEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN_Z Dee_WideChar const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_SpecWriteWideStringEx(self,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteStringEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN_Z Dee_Utf8Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_SpecWriteUtf8StringEx(self,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteStringEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN_Z Dee_Utf16Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_SpecWriteUtf16StringEx(self,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteStringEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN_Z Dee_Utf32Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_SpecWriteUtf32StringEx(self,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteStringWithLengthEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_SpecWriteWideStringWithLengthEx(self,len,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteStringWithLengthEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_SpecWriteUtf8StringWithLengthEx(self,len,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteStringWithLengthEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_SpecWriteUtf16StringWithLengthEx(self,len,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteStringWithLengthEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_SpecWriteUtf32StringWithLengthEx(self,len,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteCharEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_WideChar ch, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_SpecWriteWideCharEx(self,ch,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteCharEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_Utf8Char ch, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_SpecWriteUtf8CharEx(self,ch,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteCharEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_Utf16Char ch, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_SpecWriteUtf16CharEx(self,ch,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteCharEx(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_Utf32Char ch, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeWideStringWriter_SpecWriteUtf32CharEx(self,ch,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteQuotedString(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN_Z Dee_WideChar const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeWideStringWriter_SpecWriteQuotedWideString(self,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteQuotedString(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN_Z Dee_Utf8Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeWideStringWriter_SpecWriteQuotedUtf8String(self,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteQuotedString(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN_Z Dee_Utf16Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeWideStringWriter_SpecWriteQuotedUtf16String(self,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteQuotedString(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN_Z Dee_Utf32Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeWideStringWriter_SpecWriteQuotedUtf32String(self,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteQuotedStringWithLength(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeWideStringWriter_SpecWriteQuotedWideStringWithLength(self,len,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteQuotedStringWithLength(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeWideStringWriter_SpecWriteQuotedUtf8StringWithLength(self,len,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteQuotedStringWithLength(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeWideStringWriter_SpecWriteQuotedUtf16StringWithLength(self,len,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeWideStringWriter_SpecWriteQuotedStringWithLength(DEE_A_INOUT DeeWideStringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeWideStringWriter_SpecWriteQuotedUtf32StringWithLength(self,len,s,spec); }
#undef DeeUtf8String_FromStringEx
#undef DeeUtf8String_FromStringWithLengthEx
#undef DeeUtf8String_FromQuotedString
#undef DeeUtf8String_FromQuotedStringWithLength
#undef DeeUtf8StringWriter_WRITE_STRING
#undef DeeUtf8StringWriter_WriteStringEx
#undef DeeUtf8StringWriter_WriteStringWithLengthEx
#undef DeeUtf8StringWriter_WriteCharEx
#undef DeeUtf8StringWriter_WriteQuotedString
#undef DeeUtf8StringWriter_WriteQuotedStringWithLength
#undef DeeUtf8StringWriter_SpecWriteStringEx
#undef DeeUtf8StringWriter_SpecWriteStringWithLengthEx
#undef DeeUtf8StringWriter_SpecWriteCharEx
#undef DeeUtf8StringWriter_SpecWriteQuotedString
#undef DeeUtf8StringWriter_SpecWriteQuotedStringWithLength
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUtf8String_FromStringEx(DEE_A_IN_Z Dee_WideChar const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8String_FromWideStringEx(s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUtf8String_FromStringEx(DEE_A_IN_Z Dee_Utf8Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8String_FromUtf8StringEx(s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUtf8String_FromStringEx(DEE_A_IN_Z Dee_Utf16Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8String_FromUtf16StringEx(s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUtf8String_FromStringEx(DEE_A_IN_Z Dee_Utf32Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8String_FromUtf32StringEx(s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUtf8String_FromStringWithLengthEx(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8String_FromWideStringWithLengthEx(len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUtf8String_FromStringWithLengthEx(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8String_FromUtf8StringWithLengthEx(len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUtf8String_FromStringWithLengthEx(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8String_FromUtf16StringWithLengthEx(len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUtf8String_FromStringWithLengthEx(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8String_FromUtf32StringWithLengthEx(len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUtf8String_FromQuotedString(DEE_A_IN_Z Dee_WideChar const *s) { return DeeUtf8String_FromQuotedWideString(s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUtf8String_FromQuotedString(DEE_A_IN_Z Dee_Utf8Char const *s) { return DeeUtf8String_FromQuotedUtf8String(s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUtf8String_FromQuotedString(DEE_A_IN_Z Dee_Utf16Char const *s) { return DeeUtf8String_FromQuotedUtf16String(s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUtf8String_FromQuotedString(DEE_A_IN_Z Dee_Utf32Char const *s) { return DeeUtf8String_FromQuotedUtf32String(s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUtf8String_FromQuotedStringWithLength(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s) { return DeeUtf8String_FromQuotedWideStringWithLength(len,s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUtf8String_FromQuotedStringWithLength(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s) { return DeeUtf8String_FromQuotedUtf8StringWithLength(len,s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUtf8String_FromQuotedStringWithLength(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s) { return DeeUtf8String_FromQuotedUtf16StringWithLength(len,s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeUtf8String_FromQuotedStringWithLength(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s) { return DeeUtf8String_FromQuotedUtf32StringWithLength(len,s); }
template<Dee_size_t len> DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WRITE_STRING(DEE_A_INOUT DeeUtf8StringWriter *self, Dee_WideChar const (&s)[len]) { return DeeUtf8StringWriter_WRITE_WIDE_STRING(self,s); }
template<Dee_size_t len> DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WRITE_STRING(DEE_A_INOUT DeeUtf8StringWriter *self, Dee_Utf8Char const (&s)[len]) { return DeeUtf8StringWriter_WRITE_UTF8_STRING(self,s); }
template<Dee_size_t len> DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WRITE_STRING(DEE_A_INOUT DeeUtf8StringWriter *self, Dee_Utf16Char const (&s)[len]) { return DeeUtf8StringWriter_WRITE_UTF16_STRING(self,s); }
template<Dee_size_t len> DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WRITE_STRING(DEE_A_INOUT DeeUtf8StringWriter *self, Dee_Utf32Char const (&s)[len]) { return DeeUtf8StringWriter_WRITE_UTF32_STRING(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteStringEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN_Z Dee_WideChar const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_WriteWideStringEx(self,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteStringEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN_Z Dee_Utf8Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_WriteUtf8StringEx(self,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteStringEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN_Z Dee_Utf16Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_WriteUtf16StringEx(self,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteStringEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN_Z Dee_Utf32Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_WriteUtf32StringEx(self,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteStringWithLengthEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_WriteWideStringWithLengthEx(self,len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteStringWithLengthEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_WriteUtf8StringWithLengthEx(self,len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteStringWithLengthEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_WriteUtf16StringWithLengthEx(self,len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteStringWithLengthEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_WriteUtf32StringWithLengthEx(self,len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteCharEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_WideChar ch, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_WriteWideCharEx(self,ch,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteCharEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_Utf8Char ch, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_WriteUtf8CharEx(self,ch,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteCharEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_Utf16Char ch, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_WriteUtf16CharEx(self,ch,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteCharEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_Utf32Char ch, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_WriteUtf32CharEx(self,ch,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteQuotedString(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN_Z Dee_WideChar const *s) { return DeeUtf8StringWriter_WriteQuotedWideString(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteQuotedString(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN_Z Dee_Utf8Char const *s) { return DeeUtf8StringWriter_WriteQuotedUtf8String(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteQuotedString(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN_Z Dee_Utf16Char const *s) { return DeeUtf8StringWriter_WriteQuotedUtf16String(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteQuotedString(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN_Z Dee_Utf32Char const *s) { return DeeUtf8StringWriter_WriteQuotedUtf32String(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteQuotedStringWithLength(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s) { return DeeUtf8StringWriter_WriteQuotedWideStringWithLength(self,len,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteQuotedStringWithLength(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s) { return DeeUtf8StringWriter_WriteQuotedUtf8StringWithLength(self,len,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteQuotedStringWithLength(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s) { return DeeUtf8StringWriter_WriteQuotedUtf16StringWithLength(self,len,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_WriteQuotedStringWithLength(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s) { return DeeUtf8StringWriter_WriteQuotedUtf32StringWithLength(self,len,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteStringEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN_Z Dee_WideChar const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_SpecWriteWideStringEx(self,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteStringEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN_Z Dee_Utf8Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_SpecWriteUtf8StringEx(self,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteStringEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN_Z Dee_Utf16Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_SpecWriteUtf16StringEx(self,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteStringEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN_Z Dee_Utf32Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_SpecWriteUtf32StringEx(self,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteStringWithLengthEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_SpecWriteWideStringWithLengthEx(self,len,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteStringWithLengthEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_SpecWriteUtf8StringWithLengthEx(self,len,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteStringWithLengthEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_SpecWriteUtf16StringWithLengthEx(self,len,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteStringWithLengthEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_SpecWriteUtf32StringWithLengthEx(self,len,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteCharEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_WideChar ch, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_SpecWriteWideCharEx(self,ch,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteCharEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_Utf8Char ch, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_SpecWriteUtf8CharEx(self,ch,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteCharEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_Utf16Char ch, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_SpecWriteUtf16CharEx(self,ch,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteCharEx(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_Utf32Char ch, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf8StringWriter_SpecWriteUtf32CharEx(self,ch,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteQuotedString(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN_Z Dee_WideChar const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf8StringWriter_SpecWriteQuotedWideString(self,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteQuotedString(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN_Z Dee_Utf8Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf8StringWriter_SpecWriteQuotedUtf8String(self,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteQuotedString(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN_Z Dee_Utf16Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf8StringWriter_SpecWriteQuotedUtf16String(self,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteQuotedString(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN_Z Dee_Utf32Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf8StringWriter_SpecWriteQuotedUtf32String(self,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteQuotedStringWithLength(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf8StringWriter_SpecWriteQuotedWideStringWithLength(self,len,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteQuotedStringWithLength(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf8StringWriter_SpecWriteQuotedUtf8StringWithLength(self,len,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteQuotedStringWithLength(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf8StringWriter_SpecWriteQuotedUtf16StringWithLength(self,len,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf8StringWriter_SpecWriteQuotedStringWithLength(DEE_A_INOUT DeeUtf8StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf8StringWriter_SpecWriteQuotedUtf32StringWithLength(self,len,s,spec); }
#undef DeeUtf16String_FromStringEx
#undef DeeUtf16String_FromStringWithLengthEx
#undef DeeUtf16String_FromQuotedString
#undef DeeUtf16String_FromQuotedStringWithLength
#undef DeeUtf16StringWriter_WRITE_STRING
#undef DeeUtf16StringWriter_WriteStringEx
#undef DeeUtf16StringWriter_WriteStringWithLengthEx
#undef DeeUtf16StringWriter_WriteCharEx
#undef DeeUtf16StringWriter_WriteQuotedString
#undef DeeUtf16StringWriter_WriteQuotedStringWithLength
#undef DeeUtf16StringWriter_SpecWriteStringEx
#undef DeeUtf16StringWriter_SpecWriteStringWithLengthEx
#undef DeeUtf16StringWriter_SpecWriteCharEx
#undef DeeUtf16StringWriter_SpecWriteQuotedString
#undef DeeUtf16StringWriter_SpecWriteQuotedStringWithLength
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeUtf16String_FromStringEx(DEE_A_IN_Z Dee_WideChar const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16String_FromWideStringEx(s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeUtf16String_FromStringEx(DEE_A_IN_Z Dee_Utf8Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16String_FromUtf8StringEx(s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeUtf16String_FromStringEx(DEE_A_IN_Z Dee_Utf16Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16String_FromUtf16StringEx(s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeUtf16String_FromStringEx(DEE_A_IN_Z Dee_Utf32Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16String_FromUtf32StringEx(s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeUtf16String_FromStringWithLengthEx(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16String_FromWideStringWithLengthEx(len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeUtf16String_FromStringWithLengthEx(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16String_FromUtf8StringWithLengthEx(len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeUtf16String_FromStringWithLengthEx(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16String_FromUtf16StringWithLengthEx(len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeUtf16String_FromStringWithLengthEx(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16String_FromUtf32StringWithLengthEx(len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeUtf16String_FromQuotedString(DEE_A_IN_Z Dee_WideChar const *s) { return DeeUtf16String_FromQuotedWideString(s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeUtf16String_FromQuotedString(DEE_A_IN_Z Dee_Utf8Char const *s) { return DeeUtf16String_FromQuotedUtf8String(s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeUtf16String_FromQuotedString(DEE_A_IN_Z Dee_Utf16Char const *s) { return DeeUtf16String_FromQuotedUtf16String(s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeUtf16String_FromQuotedString(DEE_A_IN_Z Dee_Utf32Char const *s) { return DeeUtf16String_FromQuotedUtf32String(s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeUtf16String_FromQuotedStringWithLength(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s) { return DeeUtf16String_FromQuotedWideStringWithLength(len,s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeUtf16String_FromQuotedStringWithLength(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s) { return DeeUtf16String_FromQuotedUtf8StringWithLength(len,s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeUtf16String_FromQuotedStringWithLength(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s) { return DeeUtf16String_FromQuotedUtf16StringWithLength(len,s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *) DeeUtf16String_FromQuotedStringWithLength(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s) { return DeeUtf16String_FromQuotedUtf32StringWithLength(len,s); }
template<Dee_size_t len> DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WRITE_STRING(DEE_A_INOUT DeeUtf16StringWriter *self, Dee_WideChar const (&s)[len]) { return DeeUtf16StringWriter_WRITE_WIDE_STRING(self,s); }
template<Dee_size_t len> DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WRITE_STRING(DEE_A_INOUT DeeUtf16StringWriter *self, Dee_Utf8Char const (&s)[len]) { return DeeUtf16StringWriter_WRITE_UTF8_STRING(self,s); }
template<Dee_size_t len> DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WRITE_STRING(DEE_A_INOUT DeeUtf16StringWriter *self, Dee_Utf16Char const (&s)[len]) { return DeeUtf16StringWriter_WRITE_UTF16_STRING(self,s); }
template<Dee_size_t len> DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WRITE_STRING(DEE_A_INOUT DeeUtf16StringWriter *self, Dee_Utf32Char const (&s)[len]) { return DeeUtf16StringWriter_WRITE_UTF32_STRING(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteStringEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN_Z Dee_WideChar const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_WriteWideStringEx(self,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteStringEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN_Z Dee_Utf8Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_WriteUtf8StringEx(self,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteStringEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN_Z Dee_Utf16Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_WriteUtf16StringEx(self,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteStringEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN_Z Dee_Utf32Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_WriteUtf32StringEx(self,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteStringWithLengthEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_WriteWideStringWithLengthEx(self,len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteStringWithLengthEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_WriteUtf8StringWithLengthEx(self,len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteStringWithLengthEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_WriteUtf16StringWithLengthEx(self,len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteStringWithLengthEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_WriteUtf32StringWithLengthEx(self,len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteCharEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_WideChar ch, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_WriteWideCharEx(self,ch,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteCharEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_Utf8Char ch, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_WriteUtf8CharEx(self,ch,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteCharEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_Utf16Char ch, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_WriteUtf16CharEx(self,ch,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteCharEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_Utf32Char ch, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_WriteUtf32CharEx(self,ch,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteQuotedString(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN_Z Dee_WideChar const *s) { return DeeUtf16StringWriter_WriteQuotedWideString(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteQuotedString(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN_Z Dee_Utf8Char const *s) { return DeeUtf16StringWriter_WriteQuotedUtf8String(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteQuotedString(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN_Z Dee_Utf16Char const *s) { return DeeUtf16StringWriter_WriteQuotedUtf16String(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteQuotedString(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN_Z Dee_Utf32Char const *s) { return DeeUtf16StringWriter_WriteQuotedUtf32String(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteQuotedStringWithLength(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s) { return DeeUtf16StringWriter_WriteQuotedWideStringWithLength(self,len,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteQuotedStringWithLength(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s) { return DeeUtf16StringWriter_WriteQuotedUtf8StringWithLength(self,len,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteQuotedStringWithLength(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s) { return DeeUtf16StringWriter_WriteQuotedUtf16StringWithLength(self,len,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_WriteQuotedStringWithLength(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s) { return DeeUtf16StringWriter_WriteQuotedUtf32StringWithLength(self,len,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteStringEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN_Z Dee_WideChar const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_SpecWriteWideStringEx(self,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteStringEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN_Z Dee_Utf8Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_SpecWriteUtf8StringEx(self,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteStringEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN_Z Dee_Utf16Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_SpecWriteUtf16StringEx(self,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteStringEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN_Z Dee_Utf32Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_SpecWriteUtf32StringEx(self,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteStringWithLengthEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_SpecWriteWideStringWithLengthEx(self,len,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteStringWithLengthEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_SpecWriteUtf8StringWithLengthEx(self,len,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteStringWithLengthEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_SpecWriteUtf16StringWithLengthEx(self,len,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteStringWithLengthEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_SpecWriteUtf32StringWithLengthEx(self,len,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteCharEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_WideChar ch, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_SpecWriteWideCharEx(self,ch,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteCharEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_Utf8Char ch, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_SpecWriteUtf8CharEx(self,ch,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteCharEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_Utf16Char ch, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_SpecWriteUtf16CharEx(self,ch,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteCharEx(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_Utf32Char ch, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf16StringWriter_SpecWriteUtf32CharEx(self,ch,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteQuotedString(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN_Z Dee_WideChar const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf16StringWriter_SpecWriteQuotedWideString(self,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteQuotedString(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN_Z Dee_Utf8Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf16StringWriter_SpecWriteQuotedUtf8String(self,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteQuotedString(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN_Z Dee_Utf16Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf16StringWriter_SpecWriteQuotedUtf16String(self,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteQuotedString(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN_Z Dee_Utf32Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf16StringWriter_SpecWriteQuotedUtf32String(self,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteQuotedStringWithLength(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf16StringWriter_SpecWriteQuotedWideStringWithLength(self,len,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteQuotedStringWithLength(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf16StringWriter_SpecWriteQuotedUtf8StringWithLength(self,len,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteQuotedStringWithLength(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf16StringWriter_SpecWriteQuotedUtf16StringWithLength(self,len,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf16StringWriter_SpecWriteQuotedStringWithLength(DEE_A_INOUT DeeUtf16StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf16StringWriter_SpecWriteQuotedUtf32StringWithLength(self,len,s,spec); }
#undef DeeUtf32String_FromStringEx
#undef DeeUtf32String_FromStringWithLengthEx
#undef DeeUtf32String_FromQuotedString
#undef DeeUtf32String_FromQuotedStringWithLength
#undef DeeUtf32StringWriter_WRITE_STRING
#undef DeeUtf32StringWriter_WriteStringEx
#undef DeeUtf32StringWriter_WriteStringWithLengthEx
#undef DeeUtf32StringWriter_WriteCharEx
#undef DeeUtf32StringWriter_WriteQuotedString
#undef DeeUtf32StringWriter_WriteQuotedStringWithLength
#undef DeeUtf32StringWriter_SpecWriteStringEx
#undef DeeUtf32StringWriter_SpecWriteStringWithLengthEx
#undef DeeUtf32StringWriter_SpecWriteCharEx
#undef DeeUtf32StringWriter_SpecWriteQuotedString
#undef DeeUtf32StringWriter_SpecWriteQuotedStringWithLength
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeUtf32String_FromStringEx(DEE_A_IN_Z Dee_WideChar const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32String_FromWideStringEx(s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeUtf32String_FromStringEx(DEE_A_IN_Z Dee_Utf8Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32String_FromUtf8StringEx(s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeUtf32String_FromStringEx(DEE_A_IN_Z Dee_Utf16Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32String_FromUtf16StringEx(s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeUtf32String_FromStringEx(DEE_A_IN_Z Dee_Utf32Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32String_FromUtf32StringEx(s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeUtf32String_FromStringWithLengthEx(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32String_FromWideStringWithLengthEx(len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeUtf32String_FromStringWithLengthEx(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32String_FromUtf8StringWithLengthEx(len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeUtf32String_FromStringWithLengthEx(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32String_FromUtf16StringWithLengthEx(len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeUtf32String_FromStringWithLengthEx(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32String_FromUtf32StringWithLengthEx(len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeUtf32String_FromQuotedString(DEE_A_IN_Z Dee_WideChar const *s) { return DeeUtf32String_FromQuotedWideString(s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeUtf32String_FromQuotedString(DEE_A_IN_Z Dee_Utf8Char const *s) { return DeeUtf32String_FromQuotedUtf8String(s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeUtf32String_FromQuotedString(DEE_A_IN_Z Dee_Utf16Char const *s) { return DeeUtf32String_FromQuotedUtf16String(s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeUtf32String_FromQuotedString(DEE_A_IN_Z Dee_Utf32Char const *s) { return DeeUtf32String_FromQuotedUtf32String(s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeUtf32String_FromQuotedStringWithLength(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s) { return DeeUtf32String_FromQuotedWideStringWithLength(len,s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeUtf32String_FromQuotedStringWithLength(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s) { return DeeUtf32String_FromQuotedUtf8StringWithLength(len,s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeUtf32String_FromQuotedStringWithLength(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s) { return DeeUtf32String_FromQuotedUtf16StringWithLength(len,s); }
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *) DeeUtf32String_FromQuotedStringWithLength(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s) { return DeeUtf32String_FromQuotedUtf32StringWithLength(len,s); }
template<Dee_size_t len> DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WRITE_STRING(DEE_A_INOUT DeeUtf32StringWriter *self, Dee_WideChar const (&s)[len]) { return DeeUtf32StringWriter_WRITE_WIDE_STRING(self,s); }
template<Dee_size_t len> DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WRITE_STRING(DEE_A_INOUT DeeUtf32StringWriter *self, Dee_Utf8Char const (&s)[len]) { return DeeUtf32StringWriter_WRITE_UTF8_STRING(self,s); }
template<Dee_size_t len> DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WRITE_STRING(DEE_A_INOUT DeeUtf32StringWriter *self, Dee_Utf16Char const (&s)[len]) { return DeeUtf32StringWriter_WRITE_UTF16_STRING(self,s); }
template<Dee_size_t len> DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WRITE_STRING(DEE_A_INOUT DeeUtf32StringWriter *self, Dee_Utf32Char const (&s)[len]) { return DeeUtf32StringWriter_WRITE_UTF32_STRING(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteStringEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN_Z Dee_WideChar const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_WriteWideStringEx(self,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteStringEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN_Z Dee_Utf8Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_WriteUtf8StringEx(self,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteStringEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN_Z Dee_Utf16Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_WriteUtf16StringEx(self,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteStringEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN_Z Dee_Utf32Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_WriteUtf32StringEx(self,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteStringWithLengthEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_WriteWideStringWithLengthEx(self,len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteStringWithLengthEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_WriteUtf8StringWithLengthEx(self,len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteStringWithLengthEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_WriteUtf16StringWithLengthEx(self,len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteStringWithLengthEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_WriteUtf32StringWithLengthEx(self,len,s,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteCharEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_WideChar ch, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_WriteWideCharEx(self,ch,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteCharEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_Utf8Char ch, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_WriteUtf8CharEx(self,ch,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteCharEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_Utf16Char ch, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_WriteUtf16CharEx(self,ch,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteCharEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_Utf32Char ch, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_WriteUtf32CharEx(self,ch,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteQuotedString(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN_Z Dee_WideChar const *s) { return DeeUtf32StringWriter_WriteQuotedWideString(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteQuotedString(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN_Z Dee_Utf8Char const *s) { return DeeUtf32StringWriter_WriteQuotedUtf8String(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteQuotedString(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN_Z Dee_Utf16Char const *s) { return DeeUtf32StringWriter_WriteQuotedUtf16String(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteQuotedString(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN_Z Dee_Utf32Char const *s) { return DeeUtf32StringWriter_WriteQuotedUtf32String(self,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteQuotedStringWithLength(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s) { return DeeUtf32StringWriter_WriteQuotedWideStringWithLength(self,len,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteQuotedStringWithLength(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s) { return DeeUtf32StringWriter_WriteQuotedUtf8StringWithLength(self,len,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteQuotedStringWithLength(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s) { return DeeUtf32StringWriter_WriteQuotedUtf16StringWithLength(self,len,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_WriteQuotedStringWithLength(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s) { return DeeUtf32StringWriter_WriteQuotedUtf32StringWithLength(self,len,s); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteStringEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN_Z Dee_WideChar const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_SpecWriteWideStringEx(self,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteStringEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN_Z Dee_Utf8Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_SpecWriteUtf8StringEx(self,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteStringEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN_Z Dee_Utf16Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_SpecWriteUtf16StringEx(self,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteStringEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN_Z Dee_Utf32Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_SpecWriteUtf32StringEx(self,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteStringWithLengthEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_SpecWriteWideStringWithLengthEx(self,len,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteStringWithLengthEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_SpecWriteUtf8StringWithLengthEx(self,len,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteStringWithLengthEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_SpecWriteUtf16StringWithLengthEx(self,len,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteStringWithLengthEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_SpecWriteUtf32StringWithLengthEx(self,len,s,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteCharEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_WideChar ch, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_SpecWriteWideCharEx(self,ch,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteCharEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_Utf8Char ch, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_SpecWriteUtf8CharEx(self,ch,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteCharEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_Utf16Char ch, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_SpecWriteUtf16CharEx(self,ch,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteCharEx(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_Utf32Char ch, DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) { return DeeUtf32StringWriter_SpecWriteUtf32CharEx(self,ch,spec,flags); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteQuotedString(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN_Z Dee_WideChar const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf32StringWriter_SpecWriteQuotedWideString(self,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteQuotedString(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN_Z Dee_Utf8Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf32StringWriter_SpecWriteQuotedUtf8String(self,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteQuotedString(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN_Z Dee_Utf16Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf32StringWriter_SpecWriteQuotedUtf16String(self,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteQuotedString(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN_Z Dee_Utf32Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf32StringWriter_SpecWriteQuotedUtf32String(self,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteQuotedStringWithLength(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf32StringWriter_SpecWriteQuotedWideStringWithLength(self,len,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteQuotedStringWithLength(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf32StringWriter_SpecWriteQuotedUtf8StringWithLength(self,len,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteQuotedStringWithLength(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf32StringWriter_SpecWriteQuotedUtf16StringWithLength(self,len,s,spec); }
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeUtf32StringWriter_SpecWriteQuotedStringWithLength(DEE_A_INOUT DeeUtf32StringWriter *self, DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) { return DeeUtf32StringWriter_SpecWriteQuotedUtf32StringWithLength(self,len,s,spec); }
#endif
//[[[end]]]

#endif /* !GUARD_DEEMON_STRING_H */
