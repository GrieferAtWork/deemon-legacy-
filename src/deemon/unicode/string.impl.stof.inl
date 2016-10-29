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
#include <deemon/string.h>
#include <deemon/error.h>
#include "char_traits.inl"
#define KIND           0
#define ENC            Utf8
#define DEE_CHAR       char
#define DeeString_F(x) DEE_PP_CAT_2(DeeUtf8String_,x)
#endif

#ifndef ENC
#error "Must #define 'ENC'"
#endif

#ifndef DeeString_F
#error "Must #define 'DeeString_F'"
#endif

#ifndef DEE_CHAR
#error "Must #define 'DEE_CHAR'"
#endif

#if KIND == 0
#define FLOAT_T   float
#elif KIND == 1
#define FLOAT_T   double
#elif KIND == 2
#define FLOAT_T   long double
#else
#error "Invalid float kind"
#endif

#if KIND == 0
DEE_A_RET_EXCEPT(-1) int DeeString_F(ToFloat)
#elif KIND == 1
DEE_A_RET_EXCEPT(-1) int DeeString_F(ToDouble)
#elif KIND == 2
DEE_A_RET_EXCEPT(-1) int DeeString_F(ToLDouble)
#endif
(DEE_A_IN_Z DEE_CHAR const *str, DEE_A_OUT FLOAT_T *result) {
 DEE_CHAR ch;
 int numsys,more,float_extension_mult,sign = 0;
 DEE_ASSERT(str && result);
 if DEE_UNLIKELY(!*str) goto inv_chr2;
 if DEE_UNLIKELY(*str == (DEE_CHAR)'+') { if (!*++str) goto inv_chr2; }
 else if (*str == (DEE_CHAR)'-') { if (!*++str) goto inv_chr2; sign = 1; }
 if (*str == (DEE_CHAR)'0') {
  if (!*++str) { *result = 0.0; return 0; }
  if (*str == (DEE_CHAR)'x' || *str == (DEE_CHAR)'X') {
   if DEE_UNLIKELY(!*++str) goto inv_chr2;
   numsys = 16;
  } else if (*str == (DEE_CHAR)'b' || *str == (DEE_CHAR)'B') {
   if DEE_UNLIKELY(!*++str) goto inv_chr2;
   numsys = 2;
  } else if (*str == (DEE_CHAR)'.') {
   numsys = 10;
  } else numsys = 8;
 } else numsys = 10;
 float_extension_mult = 0;
 *result = (FLOAT_T)0;
 while (1) {
next:
  switch ((ch = *str++)) {
   case (DEE_CHAR)'p': case (DEE_CHAR)'P': goto flt_ext;
   case (DEE_CHAR)'0': case (DEE_CHAR)'1': case (DEE_CHAR)'2': case (DEE_CHAR)'3':
   case (DEE_CHAR)'4': case (DEE_CHAR)'5': case (DEE_CHAR)'6': case (DEE_CHAR)'7':
   case (DEE_CHAR)'8': case (DEE_CHAR)'9': more = (int)(ch-'0'); break;
   case (DEE_CHAR)'e': if DEE_UNLIKELY(numsys < (10+'e'-'a')) goto flt_ext; DEE_ATTRIBUTE_FALLTHROUGH
   case (DEE_CHAR)'a': case (DEE_CHAR)'b': case (DEE_CHAR)'c':
   case (DEE_CHAR)'d': case (DEE_CHAR)'f': more = (int)(10+ch-'a'); break;
   case (DEE_CHAR)'E': if DEE_UNLIKELY(numsys < (10+'E'-'A')) goto flt_ext; DEE_ATTRIBUTE_FALLTHROUGH
   case (DEE_CHAR)'A': case (DEE_CHAR)'B': case (DEE_CHAR)'C':
   case (DEE_CHAR)'D': case (DEE_CHAR)'F': more = (int)(10+ch-'A'); break;
   case (DEE_CHAR)'.': float_extension_mult = numsys; goto next;
   case (DEE_CHAR)'\0': return 0;
   default: goto inv_chr;
  }
  if (DEE_UNLIKELY(more >= numsys)) {
inv_chr: --str; inv_chr2:
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Invalid character in float: %#I"
                       DEE_PP_STR(DEE_PP_MUL8(DEE_STRING_ENCODING_SIZE(ENC)))
                       "x",*str);
   return -1;
  }
  if (float_extension_mult != 0) {
   *result += ((FLOAT_T)more/(FLOAT_T)float_extension_mult);
   float_extension_mult *= numsys;
  } else {
   *result = (*result*numsys)+(FLOAT_T)more;
  }
 }
 goto end;
flt_ext:
 // Read the Float extension: E[+/-][int]
#define float_extension_pos numsys
#define float_extension_off more
 float_extension_pos = 1;
 float_extension_off = 0;
 if DEE_UNLIKELY(!*str) goto inv_chr2;
 ch = *str++;
 if (ch == (DEE_CHAR)'-' || ch == (DEE_CHAR)'+') {
  float_extension_pos = (ch == (DEE_CHAR)'+');
  if ((ch = *str++) == (DEE_CHAR)'\0') goto inv_chr;
 }
 while (DEE_CH_IS_DIGIT(ch)) {
  float_extension_off = float_extension_off*10+(int)(ch-'0');
  if ((ch = *str++) == (DEE_CHAR)'\0') break;
 }
 float_extension_mult = 1;
 while (float_extension_off != 0) {
  float_extension_mult *= 10;
  --float_extension_off;
 }
 if (float_extension_pos) {
  *result *= float_extension_mult;
 } else {
  *result /= float_extension_mult;
 }
#undef float_extension_pos
#undef float_extension_off
end:
 if (sign) *result = -*result;
 return 0;
}

#if KIND == 0
DEE_A_RET_EXCEPT(-1) int DeeString_F(ToFloatWithLength)
#elif KIND == 1
DEE_A_RET_EXCEPT(-1) int DeeString_F(ToDoubleWithLength)
#elif KIND == 2
DEE_A_RET_EXCEPT(-1) int DeeString_F(ToLDoubleWithLength)
#endif
(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) DEE_CHAR const *str, DEE_A_OUT FLOAT_T *result) {
 DEE_CHAR ch;
 int numsys,more,float_extension_mult,sign = 0;
 DEE_ASSERT(str && result);
 if DEE_UNLIKELY(!len || !*str) goto inv_chr2;
 if DEE_UNLIKELY(*str == (DEE_CHAR)'+') { if (!--len) goto inv_chr2; ++str; }
 else if (*str == (DEE_CHAR)'-') { if (!--len) goto inv_chr2; ++str; sign = 1; }
 if (*str == (DEE_CHAR)'0') {
  if (!--len) { *result = 0.0; return 0; }
  ++str;
  if DEE_UNLIKELY(*str == (DEE_CHAR)'x' || *str == (DEE_CHAR)'X') {
   if DEE_UNLIKELY(!--len) goto inv_chr2;
   ++str,numsys = 16;
  } else if DEE_UNLIKELY(*str == (DEE_CHAR)'b' || *str == (DEE_CHAR)'B') {
   if DEE_UNLIKELY(!--len) goto inv_chr2;
   ++str,numsys = 2;
  } else if (*str == (DEE_CHAR)'.') {
   numsys = 10;
  } else numsys = 8;
 } else numsys = 10;
 float_extension_mult = 0;
 *result = (FLOAT_T)0;
 while (1) {
next:
  if DEE_UNLIKELY(!len--) return 0;
  switch ((ch = *str++)) {
   case (DEE_CHAR)'p': case (DEE_CHAR)'P': goto flt_ext;
   case (DEE_CHAR)'0': case (DEE_CHAR)'1': case (DEE_CHAR)'2': case (DEE_CHAR)'3':
   case (DEE_CHAR)'4': case (DEE_CHAR)'5': case (DEE_CHAR)'6': case (DEE_CHAR)'7':
   case (DEE_CHAR)'8': case (DEE_CHAR)'9': more = (int)(ch-'0'); break;
   case (DEE_CHAR)'e': if DEE_UNLIKELY(numsys < (10+'e'-'a')) goto flt_ext; DEE_ATTRIBUTE_FALLTHROUGH
   case (DEE_CHAR)'a': case (DEE_CHAR)'b': case (DEE_CHAR)'c':
   case (DEE_CHAR)'d': case (DEE_CHAR)'f': more = (int)(10+ch-'a'); break;
   case (DEE_CHAR)'E': if DEE_UNLIKELY(numsys < (10+'E'-'A')) goto flt_ext; DEE_ATTRIBUTE_FALLTHROUGH
   case (DEE_CHAR)'A': case (DEE_CHAR)'B': case (DEE_CHAR)'C':
   case (DEE_CHAR)'D': case (DEE_CHAR)'F': more = (int)(10+ch-'A'); break;
   case (DEE_CHAR)'.': float_extension_mult = numsys; goto next;
   default: goto inv_chr;
  }
  if (DEE_UNLIKELY(more >= numsys)) {
inv_chr: --str; inv_chr2:
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Invalid character in float: %#I"
                       DEE_PP_STR(DEE_PP_MUL8(DEE_STRING_ENCODING_SIZE(ENC)))
                       "x",*str);
   return -1;
  }
  if (float_extension_mult != 0) {
   *result += ((FLOAT_T)more/(FLOAT_T)float_extension_mult);
   float_extension_mult *= numsys;
  } else {
   *result = (*result*numsys)+(FLOAT_T)more;
  }
 }
 goto end;
flt_ext:
 // Read the Float extension: E[+/-][int]
#define float_extension_pos numsys
#define float_extension_off more
 float_extension_pos = 1;
 float_extension_off = 0;
 if DEE_UNLIKELY(!len) goto inv_chr2;
 --len,ch = *str++;
 if (ch == (DEE_CHAR)'-' || ch == (DEE_CHAR)'+') {
  float_extension_pos = (ch == (DEE_CHAR)'+');
  if (!--len) goto inv_chr;
  ch = *str++;
 }
 while (DEE_CH_IS_DIGIT(ch)) {
  float_extension_off = float_extension_off*10+(int)(ch-'0');
  if (!--len) break;
  ch = *str++;
 }
 float_extension_mult = 1;
 while (float_extension_off) {
  float_extension_mult *= 10;
  --float_extension_off;
 }
 if (float_extension_pos) {
  *result *= float_extension_mult;
 } else {
  *result /= float_extension_mult;
 }
#undef float_extension_pos
#undef float_extension_off
end:
 if (sign) *result = -*result;
 return 0;
}

#undef FLOAT_T
#ifndef __INTELLISENSE__
#undef KIND
#endif
