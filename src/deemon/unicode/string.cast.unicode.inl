/* Copyright (c) 2016 Cyder aka. Griefer@Work (https://sourceforge.net/users/grieferatwork) *
 *                                                                                          *
 * This software is provided 'as-is', without any express or implied                        *
 * warranty. In no event will the authors be held liable for any damages                    *
 * arising from the use of this software.                                                   *
 *                                                                                          *
 * Permission is granted to anyone to use this software for any purpose,                    *
 * including commercial applications, and to alter it and redistribute it                   *
 * freely, subject to the following restrictions:                                           *
 *                                                                                          *
 * 1. The origin of this software must not be misrepresented; you must not                  *
 *    claim that you wrote the original software. If you use this software                  *
 *    in a product, an acknowledgement in the product documentation would be                *
 *    appreciated but is not required.                                                      *
 * 2. Altered s versions must be plainly marked as such, and must not be               *
 *    misrepresented as being the original software.                                        *
 * 3. This notice may not be removed or altered from any s distribution.               *
 */

#ifdef __INTELLISENSE__
#include <deemon/__conf.inl>
#include "string.c"
#include <deemon/__string_encoding.inl>
DEE_DECL_BEGIN
#define SRC_ENCODING Utf8
#define DST_ENCODING Utf8
#endif

// NOTE: #define UNICODE_RELAXED to disable some of the encode/decode errors

#ifndef SRC_ENCODING
#error "Must #define SRC_ENCODING before including"
#endif
#ifndef DST_ENCODING
#error "Must #define DST_ENCODING before including"
#endif

// Stuff in here is partially based on that unicode file.
// So here's the disclaimer from the thing that taught me how to do the utf-conversions:
/*
 * Copyright 2001-2004 Unicode, Inc.
 * 
 * Disclaimer
 * 
 * This source code is provided as is by Unicode, Inc. No claims are
 * made as to fitness for any particular purpose. No warranties of any
 * kind are expressed or implied. The recipient agrees to determine
 * applicability of information provided. If this file has been
 * purchased on magnetic or optical media from Unicode, Inc., the
 * sole remedy for any claim will be exchange of defective media
 * within 90 days of receipt.
 * 
 * Limitations on Rights to Redistribute This Code
 * 
 * Unicode, Inc. hereby grants the right to freely use the information
 * supplied in this file in the creation of products supporting the
 * Unicode Standard, and to make copies of this file in any form
 * for internal or external distribution as long as this notice
 * remains attached.
 */

#define DST_CHAR            DEE_PP_CAT_3(Dee_,DST_ENCODING,Char)
#define SRC_CHAR            DEE_PP_CAT_3(Dee_,SRC_ENCODING,Char)
#define DST_WRITER_REQUIRE  DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter_Require)
#define DST_WRITER_WRITE    DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_Write,DST_ENCODING,StringWithLength)
#define DST_WRITER_WRITECH  DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_Write,DST_ENCODING,Char)

#define DEE_CONVERSION(src,dst)\
 (DEE_STRING_ENCODING(SRC_ENCODING) == DEE_STRING_ENCODING(src) && \
  DEE_STRING_ENCODING(DST_ENCODING) == DEE_STRING_ENCODING(dst))

#ifndef DEE_STRINGWRITER_SET_LOCALE_DEFINED
#define DEE_STRINGWRITER_SET_LOCALE_DEFINED
#ifndef DEE_STRING_USE_WIN32_API
#define _dee_setlocal _dee_setlocal
DEE_STATIC_INLINE(void) _dee_setlocal(void) {
 DEE_ATOMIC_ONCE({
  // Use default locale for everything
  if (!setlocale(LC_ALL,"")) DEE_LDEBUG("Failed to initialize default locale\n");
 });
}
#endif
#endif

#ifndef DEE_STRINGWRITER_UNICODE_CONSTANTS_DEFINED
#define DEE_STRINGWRITER_UNICODE_CONSTANTS_DEFINED
static Dee_uint8_t const dee_uni_bytemarks[7] = {0x00,0x00,0xC0,0xE0,0xF0,0xF8,0xFC};
static Dee_uint8_t const dee_utf8_trailing_bytes[256] = {
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
 2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5};
static Dee_uint32_t const dee_utf8_offsets[6] = {
 DEE_UINT32_C(0x00000000),DEE_UINT32_C(0x00003080),
 DEE_UINT32_C(0x000E2080),DEE_UINT32_C(0x03C82080),
 DEE_UINT32_C(0xFA082080),DEE_UINT32_C(0x82082080)};
#define DEE_UNI_HALF_BASE            DEE_UINT32_C(0x0010000)
#define DEE_UNI_HALF_MASK            DEE_UINT32_C(0x3FF)
#define DEE_UNI_HALF_SHIFT           10
#define DEE_UNI_MAX_BMP              0x0000FFFF
#define DEE_UNI_MAX_LEGAL_UTF32      0x0010FFFF
#define DEE_UNI_MAX_UTF16            0x0010FFFF
#define DEE_UNI_MAX_UTF32            0x7FFFFFFF
#define DEE_UNI_REPLACEMENT_CHAR     '?'
#define DEE_UNI_SURROGATE_HIGH_END   0xDBFF
#define DEE_UNI_SURROGATE_HIGH_BEGIN 0xD800
#define DEE_UNI_SURROGATE_LOW_END    0xDFFF
#define DEE_UNI_SURROGATE_LOW_BEGIN  0xDC00
DEE_STATIC_INLINE(int) _Dee_CheckUTF8StringWithLength(
 DEE_A_IN Dee_size_t l, DEE_A_IN_R(l) Dee_uint8_t const *s) {
 const Dee_uint8_t *s_end = s+l;
 Dee_uint8_t ch;
 switch (l) {
  case 4: if ((ch = (*--s_end)) < 0x80 || ch > 0xBF) return 0; DEE_ATTRIBUTE_FALLTHROUGH
  case 3: if ((ch = (*--s_end)) < 0x80 || ch > 0xBF) return 0; DEE_ATTRIBUTE_FALLTHROUGH
  case 2: if ((ch = (*--s_end)) < 0x80 || ch > 0xBF) return 0; DEE_ATTRIBUTE_FALLTHROUGH
   switch (*s) {
    case 0xE0: if (ch < 0xA0) return 0; break;
    case 0xED: if (ch > 0x9F) return 0; break;
    case 0xF0: if (ch < 0x90) return 0; break;
    case 0xF4: if (ch > 0x8F) return 0; break;
    default:   if (ch < 0x80) return 0; break;
   }
   DEE_ATTRIBUTE_FALLTHROUGH
  case 1:
   if (*s >= 0x80 && *s < 0xC2) return 0;
   break;
  default: return 0;
 }
 if (*s > 0xF4) return 0;
 return 1;
}
#endif

DEE_A_RET_EXCEPT(-1) int
DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_Write,SRC_ENCODING,StringWithLengthEx)(
 DEE_A_INOUT DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter) *self,
 DEE_A_IN Dee_size_t l, DEE_A_IN_R(l) SRC_CHAR const *s, DEE_A_IN Dee_uint32_t flags) {
#define UNICODE_STRICT ((flags&DEE_STRING_DECODE_FLAG_RELAXED)==0)
 if (!l) return 0; // Nothing to do
 {
#if DEE_STRING_ENCODING(SRC_ENCODING) == DEE_STRING_ENCODING(DST_ENCODING)
  DST_CHAR *buffer; (void)flags;
  // Simple case: Same encoding - copy from src --> dst
  if ((buffer = DST_WRITER_REQUIRE(self,l)) == NULL) return -1;
  memcpy(buffer,s,l*sizeof(DST_CHAR));
  return 0;
#elif DEE_CONVERSION(Utf8,Wide)
#ifdef DEE_STRING_USE_WIN32_API
  DST_CHAR *buffer;
  int temp;
  temp = MultiByteToWideChar(CP_UTF8,(DWORD)(
   UNICODE_STRICT ? MB_ERR_INVALID_CHARS : 0),s,(int)l,NULL,0);
  if DEE_UNLIKELY(!temp && l) {
   temp = (int)GetLastError(); SetLastError(0);
   if (temp == ERROR_NO_UNICODE_TRANSLATION && !UNICODE_STRICT) {
    if DEE_UNLIKELY((buffer = DST_WRITER_REQUIRE(self,(Dee_size_t)l)) == NULL) return -1;
    while (l--) *buffer++ = (DST_CHAR)*s++;
    return 0;
   }
   DeeError_SetStringf(&DeeErrorType_UnicodeError,
                       "MultiByteToWideChar(%.*q) : %#.8x : %K",
                       (int)(l*sizeof(SRC_CHAR)),s,
                       temp,DeeSystemError_ToString(temp));
   return -1;
  }
  if DEE_UNLIKELY((buffer = DST_WRITER_REQUIRE(self,(Dee_size_t)temp)) == NULL) return -1;
  MultiByteToWideChar(CP_UTF8,0,s,(int)l,buffer,temp);
  return 0;
#else
  mbstate_t state;
  Dee_WideChar wc;
  Dee_size_t len;
#ifdef _dee_setlocal
  _dee_setlocal();
#endif
  memset(&state,0,sizeof(state));
  while (1) {
   len = mbrtowc(&wc,s,l,&state);
   if DEE_UNLIKELY(len == (Dee_size_t)-1) {
    if DEE_LIKELY(UNICODE_STRICT) {
     int err = errno; errno = 0;
     DeeError_SetStringf(&DeeErrorType_UnicodeError,
                         "mbrtowc(%.*q) : %#.8x : %s",
                         (int)l,s,err,strerror(err));
    } else {
     if DEE_UNLIKELY(DST_WRITER_WRITECH(self,(DST_CHAR)DEE_UNI_REPLACEMENT_CHAR) == -1) return -1;
    }
   } else if DEE_UNLIKELY(len == (Dee_size_t)-2) {
    if DEE_UNLIKELY(DST_WRITER_WRITE(self,(l*sizeof(SRC_CHAR))/sizeof(DST_CHAR),(DST_CHAR *)s) == -1) return -1;
    break;
   } else {
    if DEE_UNLIKELY(DST_WRITER_WRITECH(self,(DST_CHAR)wc) == -1) return -1;
   }
   if (len >= l) break;
   l -= len;
   s += len;
  }
  return 0;
#endif
#elif DEE_CONVERSION(Wide,Utf8)
#ifdef DEE_STRING_USE_WIN32_API
  DST_CHAR *buffer; int temp;
#ifndef WC_ERR_INVALID_CHARS
#define WC_ERR_INVALID_CHARS 0x00000080
#endif
  static DWORD wincall_flags = WC_ERR_INVALID_CHARS;
again:
  temp = WideCharToMultiByte(CP_UTF8,(DWORD)(
   UNICODE_STRICT ? wincall_flags : 0),s,(int)l,NULL,0,NULL,NULL);
  if DEE_UNLIKELY(!temp) {
   temp = (int)GetLastError(); SetLastError(0);
   // This flag wasn't available before windows vista.
   // But since I don't have access to a windows XP machine,
   // I can only assume that this is the correct error.
   // (I got this one when I accidentally called WideCharToMultiByte with 'MB_ERR_INVALID_CHARS')
   if DEE_UNLIKELY(temp == ERROR_INVALID_FLAGS && wincall_flags)
   { wincall_flags = 0; goto again; }
   if (temp == ERROR_NO_UNICODE_TRANSLATION && !UNICODE_STRICT) {
    if DEE_UNLIKELY((buffer = DST_WRITER_REQUIRE(self,(Dee_size_t)l)) == NULL) return -1;
    while (l--) *buffer++ = (DST_CHAR)*s++;
    return 0;
   }
   DeeError_SetStringf(&DeeErrorType_UnicodeError,
                       "WideCharToMultiByte(%.*q) : %#.8x : %K",
                       (int)(l*sizeof(SRC_CHAR)),s,
                       temp,DeeSystemError_ToString(temp));
   return -1;
  }
  if DEE_UNLIKELY((buffer = DST_WRITER_REQUIRE(self,(Dee_size_t)temp)) == NULL) return -1;
  WideCharToMultiByte(CP_UTF8,0,s,(int)l,buffer,temp,NULL,NULL);
  return 0;
#else
  char *buf; Dee_size_t buflen;
  mbstate_t state;
#ifdef _dee_setlocal
  _dee_setlocal();
#endif
  if DEE_UNLIKELY(DEE_UNLIKELY((buflen = (Dee_size_t)MB_CUR_MAX) == 0)) return 0; // WHAT?!?!
#if DEE_HAVE_ALLOCA
  buf = (char *)alloca(buflen);
#else
  while DEE_UNLIKELY((buf = (char *)malloc_nz(buflen)) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
#endif
  memset(&state,0,sizeof(state));
  do {
   buflen = wcrtomb(buf,*s++,&state);
   if DEE_UNLIKELY(buflen == (Dee_size_t)-1) {
    if DEE_LIKELY(UNICODE_STRICT) {
     int err = errno; errno = 0;
     DeeError_SetStringf(&DeeErrorType_UnicodeError,
                         "wcrtomb(%#I"
                         DEE_PP_STR(DEE_PP_MUL8(DEE_STRING_ENCODING_SIZE(SRC_ENCODING)))
                         "x) : %#.8x : %s",
                         s[-1],err,strerror(err));
err_buf:
#if !DEE_HAVE_ALLOCA
     free_nn(buf);
#endif
     return -1;
    } else {
     errno = 0;
     buf[0] = (DST_CHAR)DEE_UNI_REPLACEMENT_CHAR;
     buflen = 1;
    }
   }
   if DEE_UNLIKELY(DST_WRITER_WRITE(self,buflen,buf) == -1) goto err_buf;
  } while DEE_LIKELY(--l);
  return 0;
#endif
#elif DEE_CONVERSION(Utf32,Utf8)
  Dee_Utf8Char *dst;
  Dee_Utf32Char ch;
  Dee_size_t dst_size;
  do {
   ch = *s++;
   if DEE_UNLIKELY(UNICODE_STRICT && ch >= DEE_UNI_SURROGATE_HIGH_BEGIN && ch <= DEE_UNI_SURROGATE_LOW_END) {
    DeeError_SetStringf(&DeeErrorType_UnicodeError,
                        "Illegal character (Utf16 surrogate): %#I32x",ch);
    return -1;
   }
   if DEE_LIKELY(ch < (Dee_Utf32Char)0x80) dst_size = 1;
   else if (ch < (Dee_Utf32Char)0x800) dst_size = 2;
   else if (ch < (Dee_Utf32Char)0x10000) dst_size = 3;
   else if (ch <= DEE_UNI_MAX_LEGAL_UTF32) dst_size = 4;
   else {
    if DEE_LIKELY(UNICODE_STRICT) {
     DeeError_SetStringf(&DeeErrorType_UnicodeError,
                         "Illegal Utf32 character (greater than max): %#I32x",ch);
     return -1;
    } else {
     if DEE_UNLIKELY(DST_WRITER_WRITECH(self,(DST_CHAR)DEE_UNI_REPLACEMENT_CHAR) == -1) return -1;
     goto skip;
    }
   }
   if DEE_UNLIKELY((dst = DST_WRITER_REQUIRE(self,dst_size)) == NULL) return -1;
   switch (dst_size) {
    case 4: *dst++ = (Dee_Utf8Char)((ch|0x80)&0xBF); ch >>= 6; DEE_ATTRIBUTE_FALLTHROUGH
    case 3: *dst++ = (Dee_Utf8Char)((ch|0x80)&0xBF); ch >>= 6; DEE_ATTRIBUTE_FALLTHROUGH
    case 2: *dst++ = (Dee_Utf8Char)((ch|0x80)&0xBF); ch >>= 6; DEE_ATTRIBUTE_FALLTHROUGH
    case 1: *dst   = (Dee_Utf8Char) (ch|dee_uni_bytemarks[dst_size]);
   }
skip:;
  } while DEE_LIKELY(--l);
  return 0;
#elif DEE_CONVERSION(Utf8,Utf32)
  SRC_CHAR const *s_end = s+l;
  do {
   Dee_Utf32Char ch = 0;
   Dee_size_t src_size = dee_utf8_trailing_bytes[*s];
   if DEE_UNLIKELY(src_size >= (Dee_size_t)(s_end-s)) {
    if DEE_LIKELY(UNICODE_STRICT) {
     DeeError_SetStringf(&DeeErrorType_UnicodeError,
                         "Source string too short (Missing %Iu bytes)",
                         (Dee_size_t)(src_size-(s_end-s)));
     return -1;
    } else {
     if DEE_UNLIKELY(DST_WRITER_WRITECH(self,DEE_UNI_REPLACEMENT_CHAR) == -1) return -1;
     break;
    }
   }
   if DEE_UNLIKELY(!_Dee_CheckUTF8StringWithLength(src_size+1,(Dee_uint8_t const *)s)) {
    if DEE_LIKELY(UNICODE_STRICT) {
     DeeError_SetStringf(&DeeErrorType_UnicodeError,
                         "Illegal source string: %.*q",
                         (int)(src_size+1),s);
     return -1;
    } else goto w_rep;
   }
   switch (src_size) {
    case 5: ch += *s++; ch <<= 6; DEE_ATTRIBUTE_FALLTHROUGH
    case 4: ch += *s++; ch <<= 6; DEE_ATTRIBUTE_FALLTHROUGH
    case 3: ch += *s++; ch <<= 6; DEE_ATTRIBUTE_FALLTHROUGH
    case 2: ch += *s++; ch <<= 6; DEE_ATTRIBUTE_FALLTHROUGH
    case 1: ch += *s++; ch <<= 6; DEE_ATTRIBUTE_FALLTHROUGH
    case 0: ch += *s++; break;
   }
   ch -= dee_utf8_offsets[src_size];
   if DEE_LIKELY(ch <= DEE_UNI_MAX_LEGAL_UTF32) {
    if DEE_UNLIKELY(ch >= DEE_UNI_SURROGATE_HIGH_BEGIN && ch <= DEE_UNI_SURROGATE_LOW_END) {
     if DEE_LIKELY(UNICODE_STRICT) {
      DeeError_SetStringf(&DeeErrorType_UnicodeError,
                          "Illegal character (Utf16 surrogate): %#I32x",ch);
      return -1;
     } else goto w_rep;
    }
   } else {
    if DEE_LIKELY(UNICODE_STRICT) {
     DeeError_SetStringf(&DeeErrorType_UnicodeError,
                         "Illegal Utf32 character: %#I32x",ch);
     return -1;
    } else {
w_rep:
     ch = (Dee_Utf32Char)DEE_UNI_REPLACEMENT_CHAR;
    }
   }
   if DEE_UNLIKELY(DST_WRITER_WRITECH(self,ch) == -1) return -1;
  } while DEE_LIKELY(s != s_end);
  return 0;
#elif DEE_CONVERSION(Utf16,Utf8)
  SRC_CHAR const *s_end = s+l;
  DST_CHAR *buf;
  Dee_Utf32Char ch;
  Dee_size_t dst_size;
  do {
   ch = *s++;
   // Convert surrogate pair to Utf32
   if (ch >= DEE_UNI_SURROGATE_HIGH_BEGIN && ch <= DEE_UNI_SURROGATE_HIGH_END) {
    if DEE_LIKELY(s < s_end) {
     Dee_Utf32Char ch2 = *s;
     if DEE_LIKELY(ch2 >= DEE_UNI_SURROGATE_LOW_BEGIN && ch2 <= DEE_UNI_SURROGATE_LOW_END) {
      ch = ((ch-DEE_UNI_SURROGATE_HIGH_BEGIN) << DEE_UNI_HALF_SHIFT)
          + (ch2-DEE_UNI_SURROGATE_LOW_BEGIN)+DEE_UNI_HALF_BASE;
      ++s;
     } else if DEE_LIKELY(UNICODE_STRICT) {
      DeeError_SetStringf(&DeeErrorType_UnicodeError,
                          "Illegal Utf16 character (Unpaired high surrogate): %#I16x",s[-1]);
      return -1;
     }
    } else if DEE_LIKELY(UNICODE_STRICT) {
     DeeError_SET_STRING(&DeeErrorType_UnicodeError,
                         "Source string too short (Missing 2 bytes)");
     return -1;
    } else {
     if DEE_UNLIKELY(DST_WRITER_WRITECH(self,(DST_CHAR)DEE_UNI_REPLACEMENT_CHAR) == -1) return -1;
     break;
    }
   } else if DEE_UNLIKELY(UNICODE_STRICT && ch >= DEE_UNI_SURROGATE_LOW_BEGIN && ch <= DEE_UNI_SURROGATE_LOW_END) {
    DeeError_SetStringf(&DeeErrorType_UnicodeError,
                        "Illegal Utf32 character (Utf16 surrogate in Utf32): %#I32x",ch);
    return -1;
   }
   if DEE_LIKELY(ch < (Dee_Utf32Char)0x80) dst_size = 1;
   else if (ch < (Dee_Utf32Char)0x800) dst_size = 2;
   else if (ch < (Dee_Utf32Char)0x10000) dst_size = 3;
   else if (ch < (Dee_Utf32Char)0x110000) dst_size = 4;
   else {
    if DEE_UNLIKELY(DST_WRITER_WRITECH(self,(DST_CHAR)DEE_UNI_REPLACEMENT_CHAR) == -1) return -1;
    goto skip;
   }
   if DEE_UNLIKELY((buf = DST_WRITER_REQUIRE(self,dst_size)) == NULL) return -1;
   buf += dst_size;
   switch (dst_size) {
    case 4: *--buf = (Dee_Utf8Char)((ch|0x80)&0xBF); ch >>= 6; DEE_ATTRIBUTE_FALLTHROUGH
    case 3: *--buf = (Dee_Utf8Char)((ch|0x80)&0xBF); ch >>= 6; DEE_ATTRIBUTE_FALLTHROUGH
    case 2: *--buf = (Dee_Utf8Char)((ch|0x80)&0xBF); ch >>= 6; DEE_ATTRIBUTE_FALLTHROUGH
    case 1: *--buf = (Dee_Utf8Char)(ch|dee_uni_bytemarks[dst_size]);
   }
skip:;
  } while DEE_LIKELY(s != s_end);
  return 0;
#elif DEE_CONVERSION(Utf8,Utf16)
  Dee_Utf32Char ch;
  SRC_CHAR const *s_end = s+l;
  do {
   Dee_size_t src_size = dee_utf8_trailing_bytes[*s];
   if DEE_UNLIKELY(src_size >= (Dee_size_t)(s_end-s)) {
    if DEE_LIKELY(UNICODE_STRICT) {
     DeeError_SetStringf(&DeeErrorType_UnicodeError,
                         "Source string too short (Missing %Iu bytes)",
                         (Dee_size_t)(src_size-(s_end-s)));
     return -1;
    } else {
     if DEE_UNLIKELY(DST_WRITER_WRITECH(self,(DST_CHAR)DEE_UNI_REPLACEMENT_CHAR) == -1) return -1;
     break;
    }
   }
   /* Do this check whether lenient or strict */
   if DEE_UNLIKELY(!_Dee_CheckUTF8StringWithLength(src_size+1,(Dee_uint8_t const *)s)) {
    if DEE_LIKELY(UNICODE_STRICT) {
     DeeError_SetStringf(&DeeErrorType_UnicodeError,
                         "Illegal source string: %.*q",
                         (int)(src_size+1),s);
     return -1;
    } else goto w_rep;
   }
   ch = 0;
   switch (src_size) {
    case 5: ch += *s++; ch <<= 6; DEE_ATTRIBUTE_FALLTHROUGH
    case 4: ch += *s++; ch <<= 6; DEE_ATTRIBUTE_FALLTHROUGH
    case 3: ch += *s++; ch <<= 6; DEE_ATTRIBUTE_FALLTHROUGH
    case 2: ch += *s++; ch <<= 6; DEE_ATTRIBUTE_FALLTHROUGH
    case 1: ch += *s++; ch <<= 6; DEE_ATTRIBUTE_FALLTHROUGH
    case 0: ch += *s++; break;
   }
   ch -= dee_utf8_offsets[src_size];
   if DEE_LIKELY(ch <= DEE_UNI_MAX_BMP) {
    if DEE_UNLIKELY(ch >= DEE_UNI_SURROGATE_HIGH_BEGIN && ch <= DEE_UNI_SURROGATE_LOW_END) {
     if DEE_LIKELY(UNICODE_STRICT) {
      DeeError_SetStringf(&DeeErrorType_UnicodeError,
                          "Illegal Utf32 character (Utf16 surrogate in Utf32): %#I32x",ch);
      return -1;
     } else if DEE_UNLIKELY(DST_WRITER_WRITECH(self,(Dee_Utf16Char)DEE_UNI_REPLACEMENT_CHAR) == -1) return -1;
    } else if DEE_UNLIKELY(DST_WRITER_WRITECH(self,(Dee_Utf16Char)ch) == -1) return -1;
   } else if DEE_UNLIKELY(ch > DEE_UNI_MAX_UTF16) {
    if DEE_LIKELY(UNICODE_STRICT) {
     DeeError_SetStringf(&DeeErrorType_UnicodeError,
                         "Illegal Utf32 character (greater than max): %#I32x",ch);
     return -1;
    } else {
w_rep:
     if DEE_UNLIKELY(DST_WRITER_WRITECH(self,(Dee_Utf16Char)DEE_UNI_REPLACEMENT_CHAR) == -1) return -1;
    }
   } else { // Range: 0xFFFF - 0x10FFFF.
    ch -= DEE_UNI_HALF_BASE;
    if DEE_UNLIKELY(
     DST_WRITER_WRITECH(self,(Dee_Utf16Char)((ch >> DEE_UNI_HALF_SHIFT)+DEE_UNI_SURROGATE_HIGH_BEGIN)) == -1 ||
     DST_WRITER_WRITECH(self,(Dee_Utf16Char)((ch & DEE_UNI_HALF_MASK)+DEE_UNI_SURROGATE_LOW_BEGIN)) == -1) return -1;
   }
  } while (s != s_end);
  return 0;
#elif DEE_CONVERSION(Utf32,Utf16)
  SRC_CHAR const *s_end = s+l;
  SRC_CHAR ch;
  do {
   ch = *s++;
   if DEE_LIKELY(ch <= DEE_UNI_MAX_BMP) {
    /* UTF-16 surrogate values are illegal in UTF-32; 0xffff or 0xfffe are both reserved values */
    if DEE_UNLIKELY(ch >= DEE_UNI_SURROGATE_HIGH_BEGIN && ch <= DEE_UNI_SURROGATE_LOW_END) {
     if DEE_LIKELY(UNICODE_STRICT) {
      DeeError_SetStringf(&DeeErrorType_UnicodeError,
                          "Illegal Utf32 character (Utf16 surrogate in Utf32): %#I32x",ch);
      return -1;
     } else if DEE_UNLIKELY(DST_WRITER_WRITECH(self,(Dee_Utf16Char)DEE_UNI_REPLACEMENT_CHAR) == -1) return -1;
    } else if DEE_UNLIKELY(DST_WRITER_WRITECH(self,(Dee_Utf16Char)ch) == -1) return -1;
   } else if DEE_UNLIKELY(ch > DEE_UNI_MAX_LEGAL_UTF32) {
    if DEE_LIKELY(UNICODE_STRICT) {
     DeeError_SetStringf(&DeeErrorType_UnicodeError,
                         "Illegal Utf32 character (greater than max): %#I32x",ch);
     return -1;
    } else if DEE_UNLIKELY(DST_WRITER_WRITECH(self,(Dee_Utf16Char)DEE_UNI_REPLACEMENT_CHAR) == -1) return -1;
   } else {
    ch -= DEE_UNI_HALF_BASE;
    if DEE_UNLIKELY(DST_WRITER_WRITECH(self,(Dee_Utf16Char)((ch >> DEE_UNI_HALF_SHIFT)+DEE_UNI_SURROGATE_HIGH_BEGIN)) == -1 ||
                    DST_WRITER_WRITECH(self,(Dee_Utf16Char)((ch & DEE_UNI_HALF_MASK)+DEE_UNI_SURROGATE_LOW_BEGIN)) == -1) return -1;
   }
  } while DEE_LIKELY(s != s_end);
  return 0;
#elif DEE_CONVERSION(Utf16,Utf32)
  SRC_CHAR const *s_end = s+l;
  Dee_Utf32Char ch,ch2;
  do {
   ch = *s++;
   if DEE_UNLIKELY(ch >= DEE_UNI_SURROGATE_HIGH_BEGIN && ch <= DEE_UNI_SURROGATE_HIGH_END) {
    // Surrogate pair
    if DEE_LIKELY(s != s_end) {
     ch2 = *s;
     if DEE_LIKELY(ch2 >= DEE_UNI_SURROGATE_LOW_BEGIN && ch2 <= DEE_UNI_SURROGATE_LOW_END) {
      ch = ((ch - DEE_UNI_SURROGATE_HIGH_BEGIN) << DEE_UNI_HALF_SHIFT)
          + (ch2 - DEE_UNI_SURROGATE_LOW_BEGIN) + DEE_UNI_HALF_BASE;
      ++s;
     } else if DEE_LIKELY(UNICODE_STRICT) {
      DeeError_SetStringf(&DeeErrorType_UnicodeError,
                          "Illegal Utf16 character (Unpaired high surrogate): %#I16x",ch2);
      return -1;
     }
    } else if DEE_LIKELY(UNICODE_STRICT) {
     DeeError_SET_STRING(&DeeErrorType_UnicodeError,
                         "Source string too short (Missing 2 bytes)");
     return -1;
    } else ch = (Dee_Utf32Char)DEE_UNI_REPLACEMENT_CHAR;
   } else if DEE_UNLIKELY(UNICODE_STRICT && ch >= DEE_UNI_SURROGATE_LOW_BEGIN && ch <= DEE_UNI_SURROGATE_LOW_END) {
    DeeError_SetStringf(&DeeErrorType_UnicodeError,
                        "Illegal Utf32 character (Utf16 surrogate in Utf32): %#I32x",ch);
    return -1;
   }
   if DEE_UNLIKELY(DST_WRITER_WRITECH(self,ch) == -1) return -1;
  } while DEE_LIKELY(s != s_end);
  return 0;
#elif DEE_STRING_ENCODING(SRC_ENCODING) == DEE_STRING_ENCODING(Wide)
  int result;
  // Fallback when converting from wide --> utf16/utf32
  // NOTE: For these we must use an intermediate utf8 buffer
  DeeUtf8StringWriter temp_writer = DeeUtf8StringWriter_INIT();
  result = DeeUtf8StringWriter_WriteWideStringWithLengthEx(&temp_writer,l,s,flags);
  if DEE_LIKELY(result == 0 && DeeUtf8StringWriter_SIZE(&temp_writer) != 0) {
   result = DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter_WriteUtf8StringWithLengthEx)(
    self,DeeUtf8StringWriter_SIZE(&temp_writer),DeeUtf8StringWriter_ELEM(&temp_writer),flags);
  }
  DeeUtf8StringWriter_Quit(&temp_writer);
  return result;
#elif DEE_STRING_ENCODING(DST_ENCODING) == DEE_STRING_ENCODING(Wide)
  int result;
  // Fallback when converting from utf16/utf32 --> wide
  // NOTE: For these we must use an intermediate utf8 buffer
  DeeUtf8StringWriter temp_writer = DeeUtf8StringWriter_INIT();
  result = DEE_PP_CAT_3(DeeUtf8StringWriter_Write,SRC_ENCODING,StringWithLengthEx)(&temp_writer,l,s,flags);
  if DEE_LIKELY(result == 0 && DeeUtf8StringWriter_SIZE(&temp_writer) != 0) {
   result = DeeWideStringWriter_WriteUtf8StringWithLengthEx(
    self,DeeUtf8StringWriter_SIZE(&temp_writer),DeeUtf8StringWriter_ELEM(&temp_writer),flags);
  }
  DeeUtf8StringWriter_Quit(&temp_writer);
  return result;
#else
  // Fallback implementation (no conversion)
#ifndef DEE_ENCODE_DECODE_NO_WARNINGS
#error Unsupported Encoding: SRC_ENCODING --> DST_ENCODING -- You can safely comment this error out, but be aware that there is an unsupported encoding
#endif
  DST_CHAR *buffer; (void)flags;
  if DEE_UNLIKELY((buffer = DST_WRITER_REQUIRE(self,l)) == NULL) return -1;
  do *buffer++ = (DST_CHAR)*s++; while DEE_LIKELY(--l);
  return 0;
#endif
 }
}

DEE_A_RET_EXCEPT(-1) int
DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_Write,SRC_ENCODING,CharEx)(
 DEE_A_INOUT DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter) *self,
 DEE_A_IN SRC_CHAR ch, DEE_A_IN Dee_uint32_t flags) {
#if DEE_STRING_ENCODING(SRC_ENCODING) == DEE_STRING_ENCODING(DST_ENCODING)
 DST_CHAR *buffer; (void)flags;
 // Simple case: Copy from src --> dst
 if DEE_UNLIKELY((buffer = DST_WRITER_REQUIRE(self,1)) == NULL) return -1;
 *buffer = ch;
 return 0;
#else
 return DEE_PP_CAT_5(
  Dee,DST_ENCODING,StringWriter_Write,
  SRC_ENCODING,StringWithLengthEx)(self,1,&ch,flags);
#endif
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStrObject) *
DEE_PP_CAT_5(Dee,DST_ENCODING,String_From,SRC_ENCODING,StringWithLengthEx)(
 DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) SRC_CHAR const *s, DEE_A_IN Dee_uint32_t flags) {
 DeeObject *result;
 if DEE_UNLIKELY(!len) DEE_PP_CAT_3(DeeReturn_Empty,DST_ENCODING,String);
 {
#if DEE_STRING_ENCODING(SRC_ENCODING) == DEE_STRING_ENCODING(DST_ENCODING)
 (void)flags;
 if DEE_LIKELY((result = (DeeObject *)DEE_PP_CAT_3(Dee,DST_ENCODING,String_NewSized)(len)) != NULL) {
  memcpy(DEE_PP_CAT_3(Dee,DST_ENCODING,String_STR)(result),s,len*sizeof(SRC_CHAR));
 }
 return result;
#else
 DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter) writer = DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter_INIT)();
 if DEE_LIKELY(DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_Write,SRC_ENCODING,StringWithLengthEx)(&writer,len,s,flags) != -1)
  result = DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter_Pack)(&writer);
 else result = NULL;
 DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter_Quit)(&writer);
 return result;
#endif
 }
}

DEE_A_RET_EXCEPT(-1) int DEE_PP_CAT_5(
 Dee,DST_ENCODING,StringWriter_SpecWrite,SRC_ENCODING,StringWithLengthEx)(
 DEE_A_INOUT DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter) *self,
 DEE_A_IN Dee_size_t s_len, DEE_A_IN_R(s_len) SRC_CHAR const *s,
 DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) {
 DEE_ASSERT(self),DEE_ASSERT(spec);
 if DEE_UNLIKELY(spec->has_width && !spec->left_just && spec->width > s_len)
  if DEE_UNLIKELY(DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter_RepeatChar)(self,(DST_CHAR)' ',spec->width-s_len) == -1) return -1;
#if DEE_STRING_ENCODING(SRC_ENCODING) == DEE_STRING_ENCODING(DST_ENCODING)
 if (spec->instab) {
  DST_CHAR const *iter,*begin,*end;
  end = (iter = begin = s)+s_len;
  while (iter != end) {
   if (*iter++ == '\n') {
    if DEE_UNLIKELY(DEE_PP_CAT_5(Dee,DST_ENCODING,
     StringWriter_Write,SRC_ENCODING,StringWithLengthEx)(
     self,(Dee_size_t)(iter-begin),begin,flags) != 0) return -1;
    if DEE_UNLIKELY(DEE_PP_CAT_3(Dee,DST_ENCODING,
     StringWriter_RepeatChar)(
     self,(DST_CHAR)'\t',(Dee_size_t)spec->instab) != 0) return -1;
    begin = iter;
   }
  }
  if DEE_UNLIKELY(DEE_PP_CAT_5(Dee,DST_ENCODING,
   StringWriter_Write,SRC_ENCODING,StringWithLengthEx)(
   self,(Dee_size_t)(end-begin),begin,flags) != 0) return -1;
 } else
#endif
 if DEE_UNLIKELY(DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_Write,SRC_ENCODING,StringWithLengthEx)(self,s_len,s,flags) == -1) return -1;
 if DEE_UNLIKELY(spec->has_width && spec->left_just && spec->width > s_len)
  if DEE_UNLIKELY(DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter_RepeatChar)(self,(DST_CHAR)' ',spec->width-s_len) == -1) return -1;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DEE_PP_CAT_5(
 Dee,DST_ENCODING,StringWriter_SpecWrite,SRC_ENCODING,CharEx)(
 DEE_A_INOUT DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter) *self, DEE_A_IN SRC_CHAR ch,
 DEE_A_IN DeeStringWriterFormatSpec const *spec, DEE_A_IN Dee_uint32_t flags) {
 DEE_ASSERT(self),DEE_ASSERT(spec);
 if DEE_UNLIKELY(spec->has_width && !spec->left_just && spec->width > 1)
  if DEE_UNLIKELY(DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter_RepeatChar)(self,(DST_CHAR)' ',spec->width-1) == -1) return -1;
 if DEE_UNLIKELY(DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_Write,SRC_ENCODING,CharEx)(self,ch,flags) == -1) return -1;
 if DEE_UNLIKELY(spec->has_width && spec->left_just && spec->width > 1)
  if DEE_UNLIKELY(DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter_RepeatChar)(self,(DST_CHAR)' ',spec->width-1) == -1) return -1;
 return 0;
}



#undef DEE_CONVERSION
#undef DST_WRITER_WRITECH
#undef DST_WRITER_WRITE
#undef DST_WRITER_REQUIRE
#undef DST_CHAR
#undef SRC_CHAR


#ifdef __INTELLISENSE__
DEE_DECL_END
#endif

