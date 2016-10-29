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
#include <deemon/__conf.inl>
#include <deemon/__string_encoding.inl>
#include "string.cast.escapecode.previewsize.inl"
#include "unicode.c"
DEE_DECL_BEGIN
#define SRC_ENCODING Utf8
#define DST_ENCODING Utf8
#endif

#ifndef SRC_ENCODING
#error "Must #define SRC_ENCODING before including"
#endif
#ifndef DST_ENCODING
#error "Must #define DST_ENCODING before including"
#endif

#define DST_CHAR            DEE_PP_CAT_3(Dee_,DST_ENCODING,Char)
#define SRC_CHAR            DEE_PP_CAT_3(Dee_,SRC_ENCODING,Char)
#define DST_WRITER_REQUIRE  DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter_Require)

#ifndef DEE_PRIVATE_QUOTE_SPECIAL_STRINGS_DEFINED
#define DEE_PRIVATE_QUOTE_SPECIAL_STRINGS_DEFINED 1
static Dee_WideChar const _Dee_WideCharQuoteX0[2][3]  = {{'\\','x','\0'},{'\\','X','\0'}};
static Dee_WideChar const _Dee_WideCharQuoteX0E[2][4] = {{'\\','x','e','\0'},{'\\','X','E','\0'}};
static Dee_WideChar const _Dee_WideCharQuoteX0F[2][4] = {{'\\','x','f','\0'},{'\\','X','F','\0'}};
static Dee_Utf8Char const _Dee_Utf8CharQuoteX0[2][3]  = {{'\\','x','\0'},{'\\','X','\0'}};
static Dee_Utf8Char const _Dee_Utf8CharQuoteX0E[2][4] = {{'\\','x','e','\0'},{'\\','X','E','\0'}};
static Dee_Utf8Char const _Dee_Utf8CharQuoteX0F[2][4] = {{'\\','x','f','\0'},{'\\','X','F','\0'}};
static Dee_Utf16Char const _Dee_Utf16CharQuoteX0[2][3]  = {{'\\','x','\0'},{'\\','X','\0'}};
static Dee_Utf16Char const _Dee_Utf16CharQuoteX0E[2][4] = {{'\\','x','e','\0'},{'\\','X','E','\0'}};
static Dee_Utf16Char const _Dee_Utf16CharQuoteX0F[2][4] = {{'\\','x','f','\0'},{'\\','X','F','\0'}};
static Dee_Utf32Char const _Dee_Utf32CharQuoteX0[2][3]  = {{'\\','x','\0'},{'\\','X','\0'}};
static Dee_Utf32Char const _Dee_Utf32CharQuoteX0E[2][4] = {{'\\','x','e','\0'},{'\\','X','E','\0'}};
static Dee_Utf32Char const _Dee_Utf32CharQuoteX0F[2][4] = {{'\\','x','f','\0'},{'\\','X','F','\0'}};
#endif

#if DEE_STRING_ENCODING_SIZE(SRC_ENCODING) >= 2
static void 
DEE_PP_CAT_5(_Dee_Fast,DST_ENCODING,WriteHex,SRC_ENCODING,Char_Lower)
(DST_CHAR *dst, SRC_CHAR ch) {
 while (ch) {
  Dee_uint8_t temp = (Dee_uint8_t)(ch&0x0F);
  *--dst = (DST_CHAR)(temp < 10 ? ('0'+temp) : 'a'+(temp-10));
  ch >>= 4;
 }
}
static void
DEE_PP_CAT_5(_Dee_Fast,DST_ENCODING,WriteHex,SRC_ENCODING,Char_Upper)
(DST_CHAR *dst, SRC_CHAR ch) {
 while (ch) {
  Dee_uint8_t temp = (Dee_uint8_t)(ch&0x0F);
  *--dst = (DST_CHAR)(temp < 10 ? ('0'+temp) : 'A'+(temp-10));
  ch >>= 4;
 }
}
#endif


DEE_COMPILER_MSVC_WARNING_PUSH(4701)
DEE_A_RET_EXCEPT(-1) int
DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_SpecWriteQuoted,SRC_ENCODING,StringWithLength)(
 DEE_A_INOUT DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter) *self, DEE_A_IN Dee_size_t l,
 DEE_A_IN_R(l) SRC_CHAR const *s, DEE_A_IN DeeStringWriterFormatSpec const *spec) {
 SRC_CHAR ch; SRC_CHAR const *s_end = s+l;
 Dee_size_t expanded_length;
 if (spec->has_width) {
  expanded_length = DEE_PP_CAT_3(_Dee_,SRC_ENCODING,PreviewQuotedLength)(s,s_end);
  if (!spec->precision) expanded_length += 2; // Required for the surrounding quotes
  if (!spec->left_just && spec->width > expanded_length) {
   if (DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter_RepeatChar)(
    self,(DST_CHAR)' ',spec->width-expanded_length) == -1) return -1;
  }
 }
 if (!spec->prefix && 
     DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_Write,DST_ENCODING,Char)
     (self,(DST_CHAR)'\"') == -1)  return -1;
 while (s != s_end) {
  switch ((ch = *s++)) {
   case (SRC_CHAR)'\x07': ch = (SRC_CHAR)'a';   goto ch1;
   case (SRC_CHAR)'\x08': ch = (SRC_CHAR)'b';   goto ch1;
   case (SRC_CHAR)'\x09': ch = (SRC_CHAR)'t';   goto ch1;
   case (SRC_CHAR)'\x0A': ch = (SRC_CHAR)'n';   goto ch1;
   case (SRC_CHAR)'\x0B': ch = (SRC_CHAR)'v';   goto ch1;
   case (SRC_CHAR)'\x0C': ch = (SRC_CHAR)'c';   goto ch1;
   case (SRC_CHAR)'\x0D': ch = (SRC_CHAR)'r';   goto ch1;
   case (SRC_CHAR)'\x1B': ch = (SRC_CHAR)'e';   goto ch1;
// case (SRC_CHAR)'\'': /*ch = (SRC_CHAR)'\'';  goto ch1;*/
   case (SRC_CHAR)'\"': /*ch = (SRC_CHAR)'\"';  goto ch1;*/
   case (SRC_CHAR)'\\': /*ch = (SRC_CHAR)'\\';*/goto ch1;
   case (SRC_CHAR)'\x7F': ch = (SRC_CHAR)'d';   goto ch1;
   case (SRC_CHAR)'\0': case (SRC_CHAR)'\1': case (SRC_CHAR)'\2':
   case (SRC_CHAR)'\3': case (SRC_CHAR)'\4': case (SRC_CHAR)'\5':
   case (SRC_CHAR)'\6': ch += (SRC_CHAR)('0'-'\0');
ch1:
    if (DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_Write,DST_ENCODING,Char)(self,(DST_CHAR)'\\') == -1 ||
        DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_Write,DST_ENCODING,Char)(self,(DST_CHAR)ch) == -1
        ) return -1;
    break;
   case (SRC_CHAR)'\x0E':
    if (DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_Write,DST_ENCODING,StringWithLength)(
     self,3,DEE_PP_CAT_3(_Dee_,DST_ENCODING,CharQuoteX0E)[spec->upper_hex]) == -1) return -1;
   case (SRC_CHAR)'\x0F':
    if (DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_Write,DST_ENCODING,StringWithLength)(
     self,3,DEE_PP_CAT_3(_Dee_,DST_ENCODING,CharQuoteX0F)[spec->upper_hex]) == -1) return -1;
    break;
   case (SRC_CHAR)'\x10': case (SRC_CHAR)'\x11': case (SRC_CHAR)'\x12': case (SRC_CHAR)'\x13':
   case (SRC_CHAR)'\x14': case (SRC_CHAR)'\x15': case (SRC_CHAR)'\x16': case (SRC_CHAR)'\x17':
   case (SRC_CHAR)'\x18': case (SRC_CHAR)'\x19': case (SRC_CHAR)'\x1A': case (SRC_CHAR)'\x1C':
   case (SRC_CHAR)'\x1D': case (SRC_CHAR)'\x1E': case (SRC_CHAR)'\x1F': case (SRC_CHAR)'\x80':
   case (SRC_CHAR)'\x81': case (SRC_CHAR)'\x82': case (SRC_CHAR)'\x83': case (SRC_CHAR)'\x84':
   case (SRC_CHAR)'\x85': case (SRC_CHAR)'\x86': case (SRC_CHAR)'\x87': case (SRC_CHAR)'\x88':
   case (SRC_CHAR)'\x89': case (SRC_CHAR)'\x8A': case (SRC_CHAR)'\x8B': case (SRC_CHAR)'\x8C': {
    Dee_uint8_t temp;
    if (DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_Write,DST_ENCODING,StringWithLength)(
     self,2,DEE_PP_CAT_3(_Dee_,DST_ENCODING,CharQuoteX0)[spec->upper_hex]) == -1) return -1;
    temp = (Dee_uint8_t)((ch >> 4) & '\x0F');
    if (DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_Write,DST_ENCODING,Char)(self,
     (DST_CHAR)(temp < 10 ? ('0'+temp) : ((spec->upper_hex ? 'A' : 'a')+(temp-10)))) == -1) return -1;
    temp = (Dee_uint8_t)(ch & '\x0F');
    if (DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_Write,DST_ENCODING,Char)(self,
     (DST_CHAR)(temp < 10 ? ('0'+temp) : ((spec->upper_hex ? 'A' : 'a')+(temp-10)))) == -1) return -1;
   } break;
   default:
#if DEE_STRING_ENCODING_SIZE(SRC_ENCODING) >= 2
    if (ch > 0xFF) {
     Dee_size_t hex_len;
     DST_CHAR *dst;
#if DEE_STRING_ENCODING_SIZE(SRC_ENCODING) >= 4
     if (ch > 0xFFFFFFF) hex_len = 10; else
     if (ch > 0xFFFFFF) hex_len = 9; else
     if (ch > 0xFFFFF) hex_len = 8; else
     if (ch > 0xFFFF) hex_len = 7; else
#endif
     if (ch > 0xFFF) hex_len = 6; else
#if 1
     hex_len = 5;
#else
     if (ch > 0xFF) hex_len = 5; else
     hex_len = 4;
#endif
     if ((dst = DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter_Require)(self,hex_len)) == NULL) return -1;
     dst[0] = (DST_CHAR)(DST_CHAR)'\\';
     if (spec->upper_hex) {
      dst[1] = (DST_CHAR)'X',DEE_PP_CAT_5(_Dee_Fast,DST_ENCODING,WriteHex,SRC_ENCODING,Char_Upper)(dst+hex_len,ch);
     } else {
      dst[1] = (DST_CHAR)'x',DEE_PP_CAT_5(_Dee_Fast,DST_ENCODING,WriteHex,SRC_ENCODING,Char_Lower)(dst+hex_len,ch);
     }
    } else
#endif
    if DEE_UNLIKELY(DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_Write,
     DST_ENCODING,Char)(self,(DST_CHAR)ch) == -1) return -1;
    break;
  }
 }
 if (!spec->prefix && DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_Write,DST_ENCODING,Char)(self,'\"') == -1)  return -1;
 if (spec->has_width && spec->left_just && spec->width > expanded_length) {
  if DEE_UNLIKELY(DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter_RepeatChar)(self,(
   DST_CHAR)' ',spec->width-expanded_length) == -1) return -1;
 }
 return 0;
}
DEE_COMPILER_MSVC_WARNING_POP

DEE_A_RET_EXCEPT(-1) int
DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_WriteQuoted,SRC_ENCODING,Char)(
 DEE_A_INOUT DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter) *self, DEE_A_IN SRC_CHAR ch) {
 return DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_WriteQuoted,SRC_ENCODING,StringWithLength)(self,1,&ch);
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStrObject) *
DEE_PP_CAT_5(Dee,DST_ENCODING,String_FromQuoted,SRC_ENCODING,StringWithLength)(
 DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) SRC_CHAR const *s) {
 DeeObject *result; DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter) writer = DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter_INIT)();
 if (DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_WriteQuoted,SRC_ENCODING,StringWithLength)(&writer,len,s) != -1)
  result = DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter_Pack)(&writer);
 else result = NULL;
 DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter_Quit)(&writer);
 return result;
}
DEE_A_RET_EXCEPT(-1) int
DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_WriteQuoted,SRC_ENCODING,StringWithLength)(
 DEE_A_INOUT DEE_PP_CAT_3(Dee,DST_ENCODING,StringWriter) *self, DEE_A_IN Dee_size_t s_len, DEE_A_IN_R(s_len) SRC_CHAR const *s) {
 return DEE_PP_CAT_5(Dee,DST_ENCODING,StringWriter_SpecWriteQuoted,SRC_ENCODING,StringWithLength)(self,s_len,s,&dee_decimal_default_spec);
}


#undef DST_WRITER_REQUIRE
#undef DST_CHAR
#undef SRC_CHAR


#ifdef __INTELLISENSE__
DEE_DECL_END
#endif

