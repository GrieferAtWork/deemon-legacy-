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
#include "string.c"
#define UNSIGNED       1
#define BITS           32
#define DEE_CHAR       char
#define DeeString_F(x) DEE_PP_CAT_2(DeeUtf8String_,x)
#endif

#ifndef UNSIGNED
#error Must define "UNSIGNED" as 0 / 1
#endif

#ifndef BITS
#error Must define "BITS" as 8 / 16 / 32 / 64
#endif

#ifndef DEE_CHAR
#error Must define "DEE_CHAR"
#endif

#ifndef DeeString_F
#error Must define "DeeString_F"
#endif

#define UINT_T    DEE_PP_CAT_3(uint,BITS,_t)
#define SINT_T    DEE_PP_CAT_3(int,BITS,_t)
#if UNSIGNED
#define INT_T     UINT_T
#else
#define INT_T     SINT_T
#endif

#ifndef DEE_STOI_UNEXPECTED_STRING_END_DEFINED
#define DEE_STOI_UNEXPECTED_STRING_END_DEFINED
static void _deeerror_stoi_unexpected_string_end(void) {
 DeeError_SET_STRING(&DeeErrorType_ValueError,"Unexpected string end");
}
#endif

#ifndef DEE_STOI_INVALID_CHARACTER_DEFINED
#define DEE_STOI_INVALID_CHARACTER_DEFINED
static void _deeerror_stoi_invalid_string_char(char ch) {
 DeeError_SetStringf(&DeeErrorType_ValueError,"Invalid character in integer: %.1q",&ch);
}
#endif

#ifndef DEE_STOI_INTEGER_OVERFLOW_DEFINED
#define DEE_STOI_INTEGER_OVERFLOW_DEFINED
static void _deeerror_stoi_integer_overflow(void) {
 DeeError_SET_STRING(&DeeErrorType_ValueError,"Integer overflow");
}
#endif

#if UNSIGNED
DEE_A_RET_EXCEPT(-1) int DEE_PP_CAT_3(DeeString_F(ToUInt),BITS,WithLength)
#else
DEE_A_RET_EXCEPT(-1) int DEE_PP_CAT_3(DeeString_F(ToInt),BITS,WithLength)
#endif
(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) DEE_CHAR const *str, DEE_A_OUT INT_T *result) {
 DEE_CHAR ch;
 INT_T new_result;
 int sign = 0;
 int numsys,more;
 if (!len) {
inv_str_end:
  _deeerror_stoi_unexpected_string_end();
  return -1;
 }
 DEE_ASSERT(str && result);
 *result = 0;
 if (*str == '+') { if (!--len) goto inv_str_end; ++str; }
 else if (*str == '-') { if (!--len) goto inv_str_end; sign = 1; ++str; }
 if (*str == '0') {
  if (!--len) return 0;
  ++str;
  if (*str == 'x' || *str == 'X') {
   if (!--len) goto inv_str_end;
   ++str;
   numsys = 16;
  } else if (*str == 'b' || *str == 'B') {
   if (!--len) goto inv_str_end;
   ++str;
   numsys = 2;
  } else numsys = 8;
 } else if (DEE_UNLIKELY(!*str)) {
  goto inv_chr;
 } else {
  numsys = 10;
 }
 DEE_ASSERT(len);
 while (1) {
  if (!len--) return 0;
  switch ((ch = *str++)) {
   case '0': case '1': case '2': case '3':
   case '4': case '5': case '6': case '7':
   case '8': case '9': more = (int)(ch-'0'); break;
   case 'a': case 'b': case 'c': case 'd':
   case 'e': case 'f': more = (int)(10+ch-'a'); break;
   case 'A': case 'B': case 'C': case 'D':
   case 'E': case 'F': more = (int)(10+ch-'A'); break;
   default: goto inv_chr;
  }
  if (DEE_UNLIKELY(more >= numsys)) {
inv_chr: --str; /*inv_chr2:*/
   _deeerror_stoi_invalid_string_char((char)*str);
   return -1;
  }
  new_result = (INT_T)(*result*numsys+more);
  if (DEE_UNLIKELY((UINT_T)new_result < *(UINT_T *)result)) {
   _deeerror_stoi_integer_overflow();
   return -1;
  }
  *result = new_result;
 }
 if (sign) *(SINT_T *)result = -*(SINT_T *)result;
 return 0;
}

#if UNSIGNED
DEE_A_RET_EXCEPT(-1) int DEE_PP_CAT_2(DeeString_F(ToUInt),BITS)
#else
DEE_A_RET_EXCEPT(-1) int DEE_PP_CAT_2(DeeString_F(ToInt),BITS)
#endif
(DEE_A_IN_Z DEE_CHAR const *str, DEE_A_OUT INT_T *result) {
 DEE_CHAR ch;
 INT_T new_result;
 int numsys,more;
 int sign = 0;
 DEE_ASSERT(str && result);
 *result = 0;
 if (*str == '+') ++str;
 else if (*str == '-') sign = 1,++str;
 if (*str == '0') {
  if (!*++str) return 0;
  if (*str == 'x' || *str == 'X') {
   if (DEE_UNLIKELY(!*++str)) goto inv_chr2;
   numsys = 16;
  } else if (*str == 'b' || *str == 'B') {
   if (DEE_UNLIKELY(!*++str)) goto inv_chr2;
   numsys = 2;
  } else numsys = 8;
 } else if (DEE_UNLIKELY(!*str)) {
  goto inv_chr;
 } else {
  numsys = 10;
 }
 DEE_ASSERT(*str);
 while (1) {
  switch ((ch = *str++)) {
   case '0': case '1': case '2': case '3':
   case '4': case '5': case '6': case '7':
   case '8': case '9': more = (int)(ch-'0'); break;
   case 'a': case 'b': case 'c': case 'd':
   case 'e': case 'f': more = (int)(10+ch-'a'); break;
   case 'A': case 'B': case 'C': case 'D':
   case 'E': case 'F': more = (int)(10+ch-'A'); break;
   case '\0': return 0;
   default: goto inv_chr;
  }
  if (DEE_UNLIKELY(more >= numsys)) {
inv_chr: --str; inv_chr2:
   if (*str) _deeerror_stoi_invalid_string_char((char)*str);
   else _deeerror_stoi_unexpected_string_end();
   return -1;
  }
  new_result = (INT_T)(*result*numsys+more);
  if (DEE_UNLIKELY((UINT_T)new_result < *(UINT_T *)result)) {
   _deeerror_stoi_integer_overflow();
   return -1;
  }
  *result = new_result;
 }
 if (sign) *(SINT_T *)result = -*(SINT_T *)result;
 return 0;
}


#undef INT_T
#undef SINT_T
#undef UINT_T
#undef BITS
