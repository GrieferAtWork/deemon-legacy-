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

#ifndef DEE_MININT_T
#include <deemon/__conf.inl>
#if DEE_TYPES_SIZEOF_INT > 1
#define DEE_PRIVATE_MININT_T_1  int
#define DEE_PRIVATE_MINUINT_T_1 int
#else
#define DEE_PRIVATE_MININT_T_1  DEE_TYPES_INT8_T
#define DEE_PRIVATE_MINUINT_T_1 DEE_TYPES_UINT8_T
#endif
#if DEE_TYPES_SIZEOF_INT > 2
#define DEE_PRIVATE_MININT_T_2  int
#define DEE_PRIVATE_MINUINT_T_2 int
#else
#define DEE_PRIVATE_MININT_T_2  DEE_TYPES_INT16_T
#define DEE_PRIVATE_MINUINT_T_2 DEE_TYPES_UINT16_T
#endif
#if DEE_TYPES_SIZEOF_INT > 4
#define DEE_PRIVATE_MININT_T_4  int
#define DEE_PRIVATE_MINUINT_T_4 int
#else
#define DEE_PRIVATE_MININT_T_4  DEE_TYPES_INT32_T
#define DEE_PRIVATE_MINUINT_T_4 DEE_TYPES_UINT32_T
#endif
#if DEE_TYPES_SIZEOF_INT > 8
#define DEE_PRIVATE_MININT_T_8  int
#define DEE_PRIVATE_MINUINT_T_8 int
#else
#define DEE_PRIVATE_MININT_T_8  DEE_TYPES_INT64_T
#define DEE_PRIVATE_MINUINT_T_8 DEE_TYPES_UINT64_T
#endif
#define DEE_PRIVATE_MININT_T(n)  DEE_PRIVATE_MININT_T_##n
#define DEE_PRIVATE_MINUINT_T(n) DEE_PRIVATE_MINUINT_T_##n
#define DEE_MININT_T(n)  DEE_PRIVATE_MININT_T(n)
#define DEE_MINUINT_T(n) DEE_PRIVATE_MINUINT_T(n)
#endif

