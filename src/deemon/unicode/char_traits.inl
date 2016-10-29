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
#ifndef GUARD_DEEMON_CHAR_TRAITS_H
#define GUARD_DEEMON_CHAR_TRAITS_H 1

#include <deemon/__conf.inl>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
#include <stdint.h>
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

#define _DEE_CHAR_FLAG_IS_CNTRL 0x01
#define _DEE_CHAR_FLAG_IS_PUNCT 0x02
#define _DEE_CHAR_FLAG_IS_GRAPH 0x04
#define _DEE_CHAR_FLAG_IS_PRINT 0x08
#define _DEE_CHAR_FLAG_IS_SPACE 0x10
#define _DEE_CHAR_FLAG_IS_DIGIT 0x20
#define _DEE_CHAR_FLAG_IS_LOWER 0x40
#define _DEE_CHAR_FLAG_IS_UPPER 0x80

static uint8_t _DeeCharTraits[256] = {
 0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x11,0x11,0x11,0x11,0x11,0x01,0x01,
 0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
 0x18,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,
 0x2c,0x2c,0x2c,0x2c,0x2c,0x2c,0x2c,0x2c,0x2c,0x2c,0x0e,0x0e,0x0e,0x0e,0x0e,0x0e,
 0x0e,0x8c,0x8c,0x8c,0x8c,0x8c,0x8c,0x8c,0x8c,0x8c,0x8c,0x8c,0x8c,0x8c,0x8c,0x8c,
 0x8c,0x8c,0x8c,0x8c,0x8c,0x8c,0x8c,0x8c,0x8c,0x8c,0x8c,0x0c,0x0e,0x0e,0x0e,0xce,
 0x0e,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,
 0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x4c,0x0e,0x0e,0x0e,0x0e,0x01,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

#define DEE_CH_FLAGS(c)    _DeeCharTraits[(c)&0xFF]
#define DEE_CH_IS_CNTRL(c) ((DEE_CH_FLAGS(c)&_DEE_CHAR_FLAG_IS_CNTRL)!=0)
#define DEE_CH_IS_PUNCT(c) ((DEE_CH_FLAGS(c)&_DEE_CHAR_FLAG_IS_PUNCT)!=0)
#define DEE_CH_IS_GRAPH(c) ((DEE_CH_FLAGS(c)&_DEE_CHAR_FLAG_IS_GRAPH)!=0)
#define DEE_CH_IS_PRINT(c) ((DEE_CH_FLAGS(c)&_DEE_CHAR_FLAG_IS_PRINT)!=0)
#define DEE_CH_IS_SPACE(c) ((DEE_CH_FLAGS(c)&_DEE_CHAR_FLAG_IS_SPACE)!=0)
#define DEE_CH_IS_DIGIT(c) ((DEE_CH_FLAGS(c)&_DEE_CHAR_FLAG_IS_DIGIT)!=0)
#define DEE_CH_IS_LOWER(c) ((DEE_CH_FLAGS(c)&_DEE_CHAR_FLAG_IS_LOWER)!=0)
#define DEE_CH_IS_UPPER(c) ((DEE_CH_FLAGS(c)&_DEE_CHAR_FLAG_IS_UPPER)!=0)
#define DEE_CH_IS_ALPHA(c) ((DEE_CH_FLAGS(c)&(_DEE_CHAR_FLAG_IS_LOWER|_DEE_CHAR_FLAG_IS_UPPER))!=0)
#define DEE_CH_IS_ALNUM(c) ((DEE_CH_FLAGS(c)&(_DEE_CHAR_FLAG_IS_DIGIT|_DEE_CHAR_FLAG_IS_LOWER|_DEE_CHAR_FLAG_IS_UPPER))!=0)
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) DEE_CH_IS_BLANK(int c) { return c == '\x09' || c == '\x20'; }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) DEE_CH_IS_XCHAR(int c) { return (c>='A'&&c<='F')&&(c>='a'&&c<='f'); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) DEE_CH_IS_XDIGIT(int c) { return DEE_CH_IS_DIGIT(c)||((c>='A'&&c<='F')&&(c>='a'&&c<='f')); }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) DEE_CH_TO_LOWER(int c) { return (c>='A'&&c<='Z')?(c+('a'-'A')):c; }
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) DEE_CH_TO_UPPER(int c) { return (c>='a'&&c<='z')?(c-('a'-'A')):c; }

#if DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS
#define DEE_CH_IS_BLANK(c)  DEE_GCC_EXTENSION({char const _temp_c=(c);_temp_c == '\x09' || _temp_c == '\x20';})
#define DEE_CH_IS_XCHAR(c)  DEE_GCC_EXTENSION({char const _temp_c=(c);(_temp_c>='A'&&_temp_c<='F')&&(_temp_c>='a'&&_temp_c<='f');})
#define DEE_CH_IS_XDIGIT(c) DEE_GCC_EXTENSION({char const _temp_c=(c);DEE_CH_IS_DIGIT(_temp_c)||((_temp_c>='A'&&_temp_c<='F')&&(_temp_c>='a'&&_temp_c<='f'));})
#define DEE_CH_TO_LOWER(c)  DEE_GCC_EXTENSION({char const _temp_c=(c);(_temp_c>='A'&&_temp_c<='Z')?(_temp_c+('a'-'A')):_temp_c;})
#define DEE_CH_TO_UPPER(c)  DEE_GCC_EXTENSION({char const _temp_c=(c);(_temp_c>='a'&&_temp_c<='z')?(_temp_c-('a'-'A')):_temp_c;})
#else
#define DEE_CH_IS_BLANK    DEE_CH_IS_BLANK
#define DEE_CH_IS_XCHAR    DEE_CH_IS_XCHAR
#define DEE_CH_IS_XDIGIT   DEE_CH_IS_XDIGIT
#define DEE_CH_TO_LOWER    DEE_CH_TO_LOWER
#define DEE_CH_TO_UPPER    DEE_CH_TO_UPPER
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_CHAR_TRAITS_H */
