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
#ifndef GUARD_DEEMON_UNICODE_C
#define GUARD_DEEMON_UNICODE_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#if 1 // TODO (only here for sprintf)
#include <stdio.h>
#endif

#ifdef DEE_PLATFORM_WINDOWS
#define DEE_STRING_USE_WIN32_API
#endif
// IFDYNAMICGETCACHEDFUNCTION

#ifdef DEE_STRING_USE_WIN32_API
#include <Windows.h>
#else
#include <errno.h>
#include <deemon/optional/std/string.h>
#include <wchar.h>
#include <locale.h>
#endif
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

// /include/*
#include <deemon/__bswap_intrin.inl>
#include <deemon/bool.h>
#include <deemon/cell.h>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/float.h>
#include <deemon/gc.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/list.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/optional/atomic_once.h>
#include <deemon/optional/hash.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/type_util.h>
#include <deemon/string.h>
#include <deemon/structured.h>
#include <deemon/super.h>
#include <deemon/tuple.h>
#include <deemon/type.h>
#include DEE_INCLUDE_MEMORY_API()

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/__alloca.inl>
#include <deemon/marshal_data.h>
#include <deemon/unicode/char_traits.inl>

#ifndef __INTELLISENSE__
#include <deemon/__string_encoding.inl>
#include <deemon/unicode/string.cast.escapecode.previewsize.inl>
#if DEE_CONFIG_HAVE_ENCODING_WIDE
#define ENC Wide
#include "string.unicode_impl.inl"
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF8
#define ENC Utf8
#include "string.unicode_impl.inl"
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
#define ENC Utf16
#include "string.unicode_impl.inl"
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
#define ENC Utf32
#include "string.unicode_impl.inl"
#endif
#endif

DEE_DECL_BEGIN

// The correlations are required for compatibility with user code!
DEE_STATIC_ASSERT(sizeof(Dee_Utf8Char) == sizeof(Dee_rt_char));
DEE_STATIC_ASSERT(sizeof(Dee_WideChar) == sizeof(Dee_rt_wchar_t));
DEE_STATIC_ASSERT(sizeof(Dee_Utf16Char) == sizeof(Dee_rt_char16_t));
DEE_STATIC_ASSERT(sizeof(Dee_Utf32Char) == sizeof(Dee_rt_char32_t));

#undef Dee_WideStrLen
#undef Dee_Utf8StrLen
#undef Dee_Utf16StrLen
#undef Dee_Utf32StrLen
DEE_A_RET_WUNUSED Dee_size_t Dee_WideStrLen(
 DEE_A_IN_Z Dee_WideChar const *str_) {
#ifdef _MSC_VER
 return wcslen(str_);
#else
 Dee_WideChar const *end = str_;
 while DEE_LIKELY(*end) ++end;
 return (Dee_size_t)(end-str_);
#endif
}
DEE_A_RET_WUNUSED Dee_size_t Dee_Utf8StrLen(
 DEE_A_IN_Z Dee_Utf8Char const *str_) {
 return strlen(str_);
}
DEE_A_RET_WUNUSED Dee_size_t Dee_Utf16StrLen(
 DEE_A_IN_Z Dee_Utf16Char const *str_) {
 Dee_Utf16Char const *end = str_;
 while DEE_LIKELY(*end) ++end;
 return (Dee_size_t)(end-str_);
}
DEE_A_RET_WUNUSED Dee_size_t Dee_Utf32StrLen(
 DEE_A_IN_Z Dee_Utf32Char const *str_) {
 Dee_Utf32Char const *end = str_;
 while DEE_LIKELY(*end) ++end;
 return (Dee_size_t)(end-str_);
}

#undef Dee_WideStrCmp
#undef Dee_Utf8StrCmp
#undef Dee_Utf16StrCmp
#undef Dee_Utf32StrCmp
DEE_A_RET_WUNUSED int Dee_WideStrCmp(
 DEE_A_IN_Z Dee_WideChar const *a,
 DEE_A_IN_Z Dee_WideChar const *b) {
#ifdef _MSC_VER
 return wcscmp(a,b);
#else
 Dee_WideChar ca,cb; DEE_ASSERT(a && b);
 do ca = *a++,cb = *b++;
 while DEE_LIKELY(ca && ca == cb);
 return (int)(cb-ca);
#endif
}
DEE_A_RET_WUNUSED int Dee_Utf8StrCmp(
 DEE_A_IN_Z Dee_Utf8Char const *a,
 DEE_A_IN_Z Dee_Utf8Char const *b) {
 return strcmp(a,b);
}
DEE_A_RET_WUNUSED int Dee_Utf16StrCmp(
 DEE_A_IN_Z Dee_Utf16Char const *a,
 DEE_A_IN_Z Dee_Utf16Char const *b) {
 Dee_Utf16Char ca,cb; DEE_ASSERT(a && b);
 do ca = *a++,cb = *b++;
 while DEE_LIKELY(ca && ca == cb);
 return (int)(cb-ca);
}
DEE_A_RET_WUNUSED int Dee_Utf32StrCmp(
 DEE_A_IN_Z Dee_Utf32Char const *a,
 DEE_A_IN_Z Dee_Utf32Char const *b) {
 Dee_Utf32Char ca,cb; DEE_ASSERT(a && b);
 do ca = *a++,cb = *b++;
 while DEE_LIKELY(ca && ca == cb);
 return (int)(cb-ca);
}


#undef Dee_WideStrNLen
#undef Dee_Utf8StrNLen
#undef Dee_Utf16StrNLen
#undef Dee_Utf32StrNLen
DEE_A_RET_WUNUSED Dee_size_t Dee_WideStrNLen(
 DEE_A_IN_R(n) Dee_WideChar const *str_, DEE_A_IN Dee_size_t n) {
 Dee_WideChar const *end = str_;
 while DEE_LIKELY(*end && n--) ++end;
 return (Dee_size_t)(end-str_);
}
DEE_A_RET_WUNUSED Dee_size_t Dee_Utf8StrNLen(
 DEE_A_IN_R(n) Dee_Utf8Char const *str_, DEE_A_IN Dee_size_t n) {
 Dee_Utf8Char const *end = str_;
 while DEE_LIKELY(*end && n--) ++end;
 return (Dee_size_t)(end-str_);
}
DEE_A_RET_WUNUSED Dee_size_t Dee_Utf16StrNLen(
 DEE_A_IN_R(n) Dee_Utf16Char const *str_, DEE_A_IN Dee_size_t n) {
 Dee_Utf16Char const *end = str_;
 while DEE_LIKELY(*end && n--) ++end;
 return (Dee_size_t)(end-str_);
}
DEE_A_RET_WUNUSED Dee_size_t Dee_Utf32StrNLen(
 DEE_A_IN_R(n) Dee_Utf32Char const *str_, DEE_A_IN Dee_size_t n) {
 Dee_Utf32Char const *end = str_;
 while DEE_LIKELY(*end && n--) ++end;
 return (Dee_size_t)(end-str_);
}

#undef Dee_WideStrCaseCmp
#undef Dee_Utf8StrCaseCmp
#undef Dee_Utf16StrCaseCmp
#undef Dee_Utf32StrCaseCmp
#define DEE_PRIVATE_MAKE_STRCASECMP(enc)\
DEE_A_RET_WUNUSED int Dee_##enc##StrCaseCmp(\
 DEE_A_IN_Z Dee_##enc##Char const *a, DEE_A_IN_Z Dee_##enc##Char const *b) {\
 Dee_##enc##Char ca,cb; DEE_ASSERT(a && b);\
 do ca = (Dee_##enc##Char)DEE_CH_TO_LOWER((int)*a++),\
    cb = (Dee_##enc##Char)DEE_CH_TO_LOWER((int)*b++);\
 while DEE_LIKELY(ca && ca == cb);\
 return (int)(cb-ca);\
}
DEE_PRIVATE_MAKE_STRCASECMP(Wide)
DEE_PRIVATE_MAKE_STRCASECMP(Utf8)
DEE_PRIVATE_MAKE_STRCASECMP(Utf16)
DEE_PRIVATE_MAKE_STRCASECMP(Utf32)
#undef DEE_PRIVATE_MAKE_STRCASECMP

#undef Dee_WideStrCmpLen
#undef Dee_Utf8StrCmpLen
#undef Dee_Utf16StrCmpLen
#undef Dee_Utf32StrCmpLen
#define DEE_PRIVATE_MAKE_STRCMPLEN(enc)\
DEE_A_RET_WUNUSED int Dee_##enc##StrCmpLen(\
 DEE_A_IN Dee_size_t a_len, DEE_A_IN_R(a_len) Dee_##enc##Char const *a,\
 DEE_A_IN Dee_size_t b_len, DEE_A_IN_R(b_len) Dee_##enc##Char const *b) {\
 int temp; Dee_##enc##Char const *a_end = a+a_len,*b_end = b+b_len;\
 while DEE_LIKELY(a != a_end && b != b_end) \
  if DEE_UNLIKELY((temp = ((int)*a++-(int)*b++)) != 0) return temp;\
 return (int)((a_end-a)-(b_end-b));\
}\
DEE_A_RET_WUNUSED int Dee_##enc##StrCaseCmpLen(\
 DEE_A_IN Dee_size_t a_len, DEE_A_IN_R(a_len) Dee_##enc##Char const *a,\
 DEE_A_IN Dee_size_t b_len, DEE_A_IN_R(b_len) Dee_##enc##Char const *b) {\
 int temp; Dee_##enc##Char const *a_end = a+a_len,*b_end = b+b_len;\
 while DEE_LIKELY(a != a_end && b != b_end) \
  if DEE_UNLIKELY((temp = (DEE_CH_TO_LOWER((int)*a++)-DEE_CH_TO_LOWER((int)*b++))) != 0) return temp;\
 return (int)((a_end-a)-(b_end-b));\
}
DEE_PRIVATE_MAKE_STRCMPLEN(Wide)
DEE_PRIVATE_MAKE_STRCMPLEN(Utf8)
DEE_PRIVATE_MAKE_STRCMPLEN(Utf16)
DEE_PRIVATE_MAKE_STRCMPLEN(Utf32)
#undef DEE_PRIVATE_MAKE_STRCMPLEN


DEE_A_RET_WUNUSED enum DeeStringEncoding DeeAnyString_DetermineEncoding(
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s, DEE_A_OUT_OPT void const **start) {
 Dee_uint8_t const *end,*iter; int found_non_ascii;
#define CH(i) ((Dee_uint8_t *)p)[i]
 if (s >= 2) {
  if (CH(0) == 0xFE && CH(1) == 0xFF) { if (start) *start = &CH(2); return DEE_STRING_ENCODING_UTF16_BE; }
  if (CH(0) == 0xFF && CH(1) == 0xFE) { if (start) *start = &CH(2); return DEE_STRING_ENCODING_UTF16_LE; }
  if (s >= 3) {
   if (CH(0) == 0xEF && CH(1) == 0xBB && CH(2) == 0xBF) { if (start) *start = &CH(3); return DEE_STRING_ENCODING_UTF8; }
   if (s >= 4) {
    if (CH(0) == 0xFF && CH(1) == 0xFE && CH(2) == 0x00 && CH(3) == 0x00) { if (start) *start = &CH(4); return DEE_STRING_ENCODING_UTF32_LE; }
    if (CH(0) == 0x00 && CH(1) == 0x00 && CH(2) == 0xFE && CH(3) == 0xFF) { if (start) *start = &CH(4); return DEE_STRING_ENCODING_UTF32_BE; }
   }
  }
 }
 if (start) *start = p;
 // No BOM found --> search for recurring \0 characters
 end = (iter = (Dee_uint8_t const *)p)+s;
 found_non_ascii = 0;
 while (iter != end) {
  if (end-iter > 4) {
   if ( iter[0] && !iter[1] && !iter[2] && !iter[3] &&  iter[4]) return DEE_STRING_ENCODING_UTF32_LE;
   if (!iter[0] && !iter[1] && !iter[2] &&  iter[3] && !iter[4]) return DEE_STRING_ENCODING_UTF32_BE;
  } else if (end-iter == 4) {
   if ( iter[0] && !iter[1] && !iter[2] && !iter[3]) return DEE_STRING_ENCODING_UTF32_LE;
   if (!iter[0] && !iter[1] && !iter[2] &&  iter[3]) return DEE_STRING_ENCODING_UTF32_BE;
  }
  if (end-iter > 2) {
   if ( iter[0] && !iter[1] &&  iter[2]) return DEE_STRING_ENCODING_UTF16_LE;
   if (!iter[0] &&  iter[1] && !iter[2]) return DEE_STRING_ENCODING_UTF16_BE;
  } else if (end-iter == 2) {
   if ( iter[0] && !iter[1]) return DEE_STRING_ENCODING_UTF16_LE;
   if (!iter[0] &&  iter[1]) return DEE_STRING_ENCODING_UTF16_BE;
  }
  if (*iter > 127) found_non_ascii = 1;
  ++iter;
 }
 return found_non_ascii
  ? DEE_STRING_ENCODING_UTF8
  : DEE_STRING_ENCODING_UTF7;
#undef CH
}

DEE_COMPILER_MSVC_WARNING_PUSH(4061)
DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *DeeAnyString_FromEncodingAndData(
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s, DEE_A_IN enum DeeStringEncoding encoding) {
 switch (encoding) {

  case DEE_STRING_ENCODING_UTF7: {
   DeeObject *result; Dee_Utf8Char *iter,*end;
   if DEE_UNLIKELY((result = DeeUtf8String_NewWithLength(
    s/sizeof(Dee_Utf8Char),(Dee_Utf8Char *)p)) == NULL) return NULL;
   end = (iter = DeeUtf8String_STR(result))+DeeUtf8String_SIZE(result);
   // Make sure only valid utf7 characters are part of this string.
   while DEE_LIKELY(iter != end) if (*iter++ > 127) {
    DeeError_SetStringf(&DeeErrorType_UnicodeError,
                        "Invalid character in utf7 string: %#I8x",iter[-1]);
    Dee_DECREF(result);
    return NULL;
   }
   return result;
  } break;


  case DEE_STRING_ENCODING_UTF8:
   return DeeUtf8String_NewWithLength(s/sizeof(Dee_Utf8Char),(Dee_Utf8Char *)p);

#ifdef DEE_PLATFORM_BIG_ENDIAN
  case DEE_STRING_ENCODING_UTF16_BE:
#else
  case DEE_STRING_ENCODING_UTF16_LE:
#endif
   return DeeUtf16String_NewWithLength(s/sizeof(Dee_Utf16Char),(Dee_Utf16Char *)p);

#ifdef DEE_PLATFORM_BIG_ENDIAN
  case DEE_STRING_ENCODING_UTF32_BE:
#else
  case DEE_STRING_ENCODING_UTF32_LE:
#endif
   return DeeUtf32String_NewWithLength(s/sizeof(Dee_Utf32Char),(Dee_Utf32Char *)p);

#ifdef DEE_PLATFORM_BIG_ENDIAN
  case DEE_STRING_ENCODING_UTF16_LE:
#else
  case DEE_STRING_ENCODING_UTF16_BE:
#endif
  {
   DeeObject *result; Dee_Utf16Char *iter,*end;
   if DEE_UNLIKELY((result = DeeUtf16String_NewWithLength(s/sizeof(
    Dee_Utf16Char),(Dee_Utf16Char *)p)) == NULL) return NULL;
   end = (iter = DeeUtf16String_STR(result))+DeeUtf16String_SIZE(result);
   while DEE_LIKELY(iter != end) *iter = DEE_BUILTIN_BESWAP16(*iter),++iter;
   return result;
  }

#ifdef DEE_PLATFORM_BIG_ENDIAN
  case DEE_STRING_ENCODING_UTF32_LE:
#else
  case DEE_STRING_ENCODING_UTF32_BE:
#endif
  {
   DeeObject *result; Dee_Utf32Char *iter,*end;
   if DEE_UNLIKELY((result = DeeUtf32String_NewWithLength(s/sizeof(
    Dee_Utf32Char),(Dee_Utf32Char *)p)) == NULL) return NULL;
   end = (iter = DeeUtf32String_STR(result))+DeeUtf32String_SIZE(result);
   while DEE_LIKELY(iter != end) *iter = DEE_BUILTIN_BESWAP32(*iter),++iter;
   return result;
  }

  default:
   DeeError_SetStringf(&DeeErrorType_UnicodeError,
                       "Unsupported encoding: %d",encoding);
   break;
 }
 return NULL;
}
DEE_COMPILER_MSVC_WARNING_POP

DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *DeeAnyString_FromData(
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s) {
 void const *data_start;
 enum DeeStringEncoding encoding;
 encoding = DeeAnyString_DetermineEncoding(p,s,&data_start);
 if (encoding == DEE_STRING_ENCODING_UTF7)
  encoding = DEE_STRING_ENCODING_UTF8; // skip the check
 return DeeAnyString_FromEncodingAndData(data_start,s-((
  Dee_uintptr_t)data_start-(Dee_uintptr_t)p),encoding);
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_UNICODE_C */
