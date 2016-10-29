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
#include <__cdefault.dee>

#ifndef CHAR_BIT
#define CHAR_BIT   __TPP_EVAL(__SIZEOF_CHAR__*8)
#endif
#ifndef SCHAR_MIN
#define SCHAR_MIN  __SCHAR_MIN__
#endif
#ifndef SCHAR_MAX
#define SCHAR_MAX  __SCHAR_MAX__
#endif
#ifndef UCHAR_MAX
#define UCHAR_MAX  __UCHAR_MAX__
#endif
#ifndef CHAR_MIN
#define CHAR_MIN   __CHAR_MIN__
#endif
#ifndef CHAR_MAX
#define CHAR_MAX   __CHAR_MAX__
#endif
#ifndef SHRT_MIN
#define SHRT_MIN   __SHORT_MIN__
#endif
#ifndef SHRT_MAX
#define SHRT_MAX   __SHORT_MAX__
#endif
#ifndef USHRT_MAX
#define USHRT_MAX  __USHORT_MAX__
#endif
#ifndef INT_MIN
#define INT_MIN    __INT_MIN__
#endif
#ifndef INT_MAX
#define INT_MAX    __INT_MAX__
#endif
#ifndef UINT_MAX
#define UINT_MAX   __UINT_MAX__
#endif
#ifndef LONG_MIN
#define LONG_MIN   __LONG_MIN__
#endif
#ifndef LONG_MAX
#define LONG_MAX   __LONG_MAX__
#endif
#ifndef ULONG_MAX
#define ULONG_MAX  __ULONG_MAX__
#endif
#ifndef LLONG_MIN
#define LLONG_MIN  __LLONG_MIN__
#endif
#ifndef LLONG_MAX
#define LLONG_MAX  __LLONG_MAX__
#endif
#ifndef ULLONG_MAX
#define ULLONG_MAX __ULLONG_MAX__
#endif

#ifndef SIZE_MAX
#define SIZE_MAX    __SIZE_MAX__
#endif

#ifndef __STDC__
// MSVC Extensions

#ifndef _I8_MIN
#define _I8_MIN     __INT8_MIN__
#endif
#ifndef _I8_MAX
#define _I8_MAX     __INT8_MAX__
#endif
#ifndef _UI8_MIN
#define _UI8_MIN    __UINT8_MIN__
#endif
#ifndef _UI8_MAX
#define _UI8_MAX    __UINT8_MAX__
#endif
#ifndef _I16_MIN
#define _I16_MIN    __INT16_MIN__
#endif
#ifndef _I16_MAX
#define _I16_MAX    __INT16_MAX__
#endif
#ifndef _UI16_MIN
#define _UI16_MIN   __UINT16_MIN__
#endif
#ifndef _UI16_MAX
#define _UI16_MAX   __UINT16_MAX__
#endif
#ifndef _I32_MIN
#define _I32_MIN    __INT32_MIN__
#endif
#ifndef _I32_MAX
#define _I32_MAX    __INT32_MAX__
#endif
#ifndef _UI32_MIN
#define _UI32_MIN   __UINT32_MIN__
#endif
#ifndef _UI32_MAX
#define _UI32_MAX   __UINT32_MAX__
#endif
#ifndef _I64_MIN
#define _I64_MIN    __INT64_MIN__
#endif
#ifndef _I64_MAX
#define _I64_MAX    __INT64_MAX__
#endif
#ifndef _UI64_MIN
#define _UI64_MIN   __UINT64_MIN__
#endif
#ifndef _UI64_MAX
#define _UI64_MAX   __UINT64_MAX__
#endif

#endif
