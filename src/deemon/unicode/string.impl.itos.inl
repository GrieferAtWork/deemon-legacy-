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
#include <deemon/optional/std/string.h>
#define UNSIGNED             1
#define BITS                 32
#define DEE_CHAR             char
#define DEE_STRINGWRITER     DeeUtf8StringWriter
#define DeeStringWriter_F(x) DEE_PP_CAT_2(DeeUtf8StringWriter_,x)
#endif

#ifndef UNSIGNED
#error Must define "UNSIGNED" as 0 / 1
#endif

#ifndef BITS
#error Must define "BITS" as 8 / 16 / 32 / 64
#endif

#ifndef DeeStringWriter_F
#error Must define "DeeStringWriter_F"
#endif

#ifndef DEE_CHAR
#error Must define "DEE_CHAR"
#endif

#ifndef DEE_STRINGWRITER
#error Must define "DEE_STRINGWRITER"
#endif

#define UINT_T    DEE_PP_CAT_3(uint,BITS,_t)
#define SINT_T    DEE_PP_CAT_3(int,BITS,_t)
#if UNSIGNED
#define INT_T     UINT_T
#else
#define INT_T     SINT_T
#endif

#if UNSIGNED
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED unsigned int)
DEE_PP_CAT_2(DeeStringWriter_F(_dee_itos_),BITS)(
 DEE_A_OUT DEE_CHAR *buffer_end, DEE_A_IN UINT_T i,
 DEE_A_IN DeeStringWriterFormatSpec const *spec) {
 // This is the heart of the actual integer writer
 // Here we generate the string repr of an integer
 unsigned int result; DEE_CHAR digit;
 if (!i) return *--buffer_end = '0',1u;
 result = 0;
 while (i) {
  digit = (DEE_CHAR)(i % spec->numsys);
  if (digit >= 10) {
   *--buffer_end = (DEE_CHAR)((spec->upper_hex ? 'A' : 'a')+(digit-10));
  } else {
   *--buffer_end = (DEE_CHAR)('0'+digit);
  }
  ++result;
  i /= (UINT_T)spec->numsys;
 }
 return result;
}
#endif


#if UNSIGNED
DEE_A_RET_EXCEPT(-1) int DEE_PP_CAT_2(DeeStringWriter_F(SpecWriteUInt),BITS)
#else
DEE_A_RET_EXCEPT(-1) int DEE_PP_CAT_2(DeeStringWriter_F(SpecWriteInt),BITS)
#endif
(DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_IN INT_T v,
 DEE_A_IN DeeStringWriterFormatSpec const *spec) {
 // Binary number take up to BITS characters
 DEE_CHAR buffer[BITS];
 unsigned int i;
 unsigned int real_number_len;     // Length of the number in the buffer
 unsigned int min_real_number_len; // Amount of digits to be written (based on precision)
 unsigned int number_len;          // Length of the number in output
 unsigned int final_number_len;    // Length of the number (considering the width attribute)
 DEE_CHAR *number_dst;                 // Output pointer to write the number to (in writer)
 DEE_ASSERT(self),DEE_ASSERT(spec);
#if UNSIGNED
 real_number_len = DEE_PP_CAT_2(DeeStringWriter_F(_dee_itos_),BITS)(buffer+BITS,v,spec);
#else
 real_number_len = DEE_PP_CAT_2(DeeStringWriter_F(_dee_itos_),BITS)(buffer+BITS,(UINT_T)(v<0?-v:v),spec);
#endif
 min_real_number_len = real_number_len;
 if (spec->has_precision && spec->precision > min_real_number_len)
  min_real_number_len = spec->precision;
 number_len = min_real_number_len;
 if (spec->prefix) number_len += spec->numsys == 8 ? 1 : (spec->numsys == 10 ? 0 : 2);
 if (spec->sign_pos||spec->sign_space
#if !UNSIGNED
     || v < 0
#endif
     ) ++number_len;
 final_number_len = number_len;
 if (spec->has_width && spec->width > final_number_len)
  final_number_len = spec->width;
 // Allocate the buffer
 number_dst = DeeStringWriter_F(Require)(self,final_number_len);
 if (!number_dst) return -1;
 if (final_number_len > number_len && !spec->left_just && !spec->pad_zero) {
  // Fill whitespace on the left side
  i = final_number_len-number_len;
  do *number_dst++ = ' '; while(--i);
 }
 // Write the sign
#if !UNSIGNED
 if (v < 0)            *number_dst++ = '-'; else
#endif
 if (spec->sign_pos)   *number_dst++ = '+'; else
 if (spec->sign_space) *number_dst++ = ' ';
 // Write the prefix
 if (spec->prefix) switch (spec->numsys) {
  case 2: *number_dst++ = '0'; *number_dst++ = 'b'; break;
  case 8: *number_dst++ = '0'; break;
  case 16: *number_dst++ = '0'; *number_dst++ = (DEE_CHAR)(spec->upper_hex?'X':'x'); break;
  default: break;
 }
 // Write padding zeros
 if (final_number_len > number_len && spec->pad_zero) {
  // Fill space left over by width with zeros
  i = final_number_len-number_len;
pz:
  do *number_dst++ = '0'; while(--i);
 } else if (spec->has_precision && spec->precision > real_number_len) {
  // Write precision zeros
  i = spec->precision-real_number_len;
  goto pz;
 }
 // Actually write the number
 memcpy(number_dst,buffer+(BITS-real_number_len),real_number_len*sizeof(DEE_CHAR));
 number_dst += real_number_len;

 // Write space at the end
 if (final_number_len > number_len && spec->left_just && !spec->pad_zero) {
  // Fill space on the right with ' '
  i = final_number_len-number_len;
  do *number_dst++ = ' '; while(--i);
 }
 return 0;
}


#undef INT_T
#undef SINT_T
#undef UINT_T
#undef BITS
