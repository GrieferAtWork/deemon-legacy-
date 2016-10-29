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
#include "unicode.c"
#include <deemon/__string_encoding.inl>
#define PREVIEW_ENC Utf8
#endif

#ifndef PREVIEW_ENC
#error "Must define PREVIEW_ENC before including this file"
#endif

#define ENC_CHAR  DEE_PP_CAT_3(Dee_,PREVIEW_ENC,Char)

DEE_STATIC_INLINE(DEE_A_RET_WUNUSED Dee_size_t)
DEE_PP_CAT_3(_Dee_,PREVIEW_ENC,PreviewQuotedLength)(
 DEE_A_IN ENC_CHAR const *s, DEE_A_IN ENC_CHAR const *end) {
 ENC_CHAR ch; Dee_size_t result = 0;
 while (s != end) {
  switch ((ch = *s++)) {
   case (ENC_CHAR)'\x07': // '\\' 'a'
   case (ENC_CHAR)'\x08': // '\\' 'b'
   case (ENC_CHAR)'\x09': // '\\' 't'
   case (ENC_CHAR)'\x0A': // '\\' 'n'
   case (ENC_CHAR)'\x0B': // '\\' 'v'
   case (ENC_CHAR)'\x0C': // '\\' 'c'
   case (ENC_CHAR)'\x0D': // '\\' 'r'
   case (ENC_CHAR)'\x1B': // '\\' 'e'
// case (ENC_CHAR)'\'':   // '\\' '\''
   case (ENC_CHAR)'\"':   // '\\' '\"'
   case (ENC_CHAR)'\\':   // '\\' '\\'
   case (ENC_CHAR)'\x7F': // '\\' 'd'
   case (ENC_CHAR)'\0':   // '\\' '0'
   case (ENC_CHAR)'\1':   // '\\' '1'
   case (ENC_CHAR)'\2':   // '\\' '2'
   case (ENC_CHAR)'\3':   // '\\' '3'
   case (ENC_CHAR)'\4':   // '\\' '4'
   case (ENC_CHAR)'\5':   // '\\' '5'
   case (ENC_CHAR)'\6':   // '\\' '6'
    result += 2;
    break;
   case (ENC_CHAR)'\x0E': // '\\' 'x' 'E'
   case (ENC_CHAR)'\x0F': // '\\' 'x' 'F'
    result += 3;
    break;
   case (ENC_CHAR)'\x10': case (ENC_CHAR)'\x11': case (ENC_CHAR)'\x12': case (ENC_CHAR)'\x13':
   case (ENC_CHAR)'\x14': case (ENC_CHAR)'\x15': case (ENC_CHAR)'\x16': case (ENC_CHAR)'\x17':
   case (ENC_CHAR)'\x18': case (ENC_CHAR)'\x19': case (ENC_CHAR)'\x1A': case (ENC_CHAR)'\x1C':
   case (ENC_CHAR)'\x1D': case (ENC_CHAR)'\x1E': case (ENC_CHAR)'\x1F': case (ENC_CHAR)'\x80':
   case (ENC_CHAR)'\x81': case (ENC_CHAR)'\x82': case (ENC_CHAR)'\x83': case (ENC_CHAR)'\x84':
   case (ENC_CHAR)'\x85': case (ENC_CHAR)'\x86': case (ENC_CHAR)'\x87': case (ENC_CHAR)'\x88':
   case (ENC_CHAR)'\x89': case (ENC_CHAR)'\x8A': case (ENC_CHAR)'\x8B': case (ENC_CHAR)'\x8C':
    result += 4; // '\\' 'x' '?' '?'
    break;
   default: 
#if DEE_STRING_ENCODING_SIZE(PREVIEW_ENC) >= 2
#if DEE_STRING_ENCODING_SIZE(PREVIEW_ENC) >= 4
    if (ch > 0xFFFFFFF) result += 10; else
    if (ch > 0xFFFFFF) result += 9; else
    if (ch > 0xFFFFF) result += 8; else
    if (ch > 0xFFFF) result += 7; else
#endif
    if (ch > 0xFFF) result += 6; else
    if (ch > 0xFF) result += 5; else
#endif
    ++result;
    break;
  }
 }
 return result;
}

#undef PREVIEW_ENC
#undef ENC_CHAR
