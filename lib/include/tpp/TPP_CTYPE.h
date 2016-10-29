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
#pragma once

#include "__TPP_STDINC.h"
#if __has_feature(__tpp_pragma_tpp_set_keyword_flags__)
// Enable the macros below as builtins
// >> __has_builtin(__builtin_tpp_isalnum) >> 1
#pragma tpp_set_keyword_flags("builtin_tpp_isalnum",0x02)
#pragma tpp_set_keyword_flags("builtin_tpp_isalpha",0x02)
#pragma tpp_set_keyword_flags("builtin_tpp_isblank",0x02)
#pragma tpp_set_keyword_flags("builtin_tpp_iscntrl",0x02)
#pragma tpp_set_keyword_flags("builtin_tpp_isdigit",0x02)
#pragma tpp_set_keyword_flags("builtin_tpp_isgraph",0x02)
#pragma tpp_set_keyword_flags("builtin_tpp_islower",0x02)
#pragma tpp_set_keyword_flags("builtin_tpp_isprint",0x02)
#pragma tpp_set_keyword_flags("builtin_tpp_ispunct",0x02)
#pragma tpp_set_keyword_flags("builtin_tpp_isspace",0x02)
#pragma tpp_set_keyword_flags("builtin_tpp_isupper",0x02)
#pragma tpp_set_keyword_flags("builtin_tpp_isxdigit",0x02)
#endif

// Builtin character function equivalents from <ctype.h>
#define __builtin_tpp_isalnum(c) __TPP_EVAL(\
((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') || \
((c) >= '0' && (c) <= '9') || ((c) == '_'))
#define __builtin_tpp_isalpha(c)  __TPP_EVAL(\
((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') || ((c) == '_'))
#define __builtin_tpp_isblank(c)  __TPP_EVAL((c) == '\x09' || (c) == '\x20')
#define __builtin_tpp_iscntrl(c)  __TPP_EVAL((c) <= '\x1F' || (c) == '\x7F')
#define __builtin_tpp_isdigit(c)  __TPP_EVAL((c) >= '0' && (c) <= '9')
#define __builtin_tpp_isgraph(c)  __TPP_EVAL((c) >= '\x21' && (c) <= '\x7E')
#define __builtin_tpp_islower(c)  __TPP_EVAL((c) >= 'a' && (c) <= 'z')
#define __builtin_tpp_isprint(c)  __TPP_EVAL((c) >= '\x20' && (c) <= '\x7E')
#define __builtin_tpp_ispunct(c)  __TPP_EVAL(\
((c) >= '\x21' && (c) <= '\x2F') ||/* !"#$%&'()*+,-./ */\
((c) >= '\x3A' && (c) <= '\x40') ||/* :;<=>?@ */\
((c) >= '\x5C' && (c) <= '\x60') ||/* [\]^_` */\
((c) >= '\x7B' && (c) <= '\x7E')   /* {|}~ */)
#define __builtin_tpp_isspace(c)  __TPP_EVAL(\
((c) >= '\x09' && (c) <= '\x0D') || (c) == '\x20')
#define __builtin_tpp_isupper(c)  __TPP_EVAL((c) >= 'A' && (c) <= 'Z')
#define __builtin_tpp_isxdigit(c) __TPP_EVAL(\
((c) >= '0' && (c) <= '9') || \
((c) >= 'A' && (c) <= 'F') || \
((c) >= 'a' && (c) <= 'f'))

#define __builtin_tpp_tolower(c) __TPP_EVAL(\
((c) >= 'A' && (c) <= 'Z') ? ((c)-('A'-'a')) : (c))
#define __builtin_tpp_toupper(c) __TPP_EVAL(\
((c) >= 'a' && (c) <= 'z') ? ((c)-('a'-'A')) : (c))
#define __builtin_tpp_swapcase(c) __TPP_EVAL(\
__builtin_tpp_islower(c)?__builtin_tpp_toupper(c):__builtin_tpp_tolower(c))


