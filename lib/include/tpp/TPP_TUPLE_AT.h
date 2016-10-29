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
#ifndef TPP_TUPLE_AT
#include "__TPP_STDINC.h"

#define __TPP_PRIVATE_TUPLE_AT_0(i,first,...) TPP_TUPLE_AT((__VA_ARGS__),i-1)
#define __TPP_PRIVATE_TUPLE_AT_1(i,first,...) first
#define __TPP_PRIVATE_TUPLE_AT(i,...)\
__TPP_BASIC_CAT(__TPP_PRIVATE_TUPLE_AT_,__TPP_EVAL((i) <= 0))(i,__VA_ARGS__)

//////////////////////////////////////////////////////////////////////////
// Returns the i'th element of "tpl"
// - Expands to nothing, if "i" is too large
// >> TPP_TUPLE_AT((10,20),0) // Expands to [10]
// >> TPP_TUPLE_AT((10,20),1) // Expands to [20]
// >> TPP_TUPLE_AT((10,20),2) // Expands to <nothing>
#define TPP_TUPLE_AT(tpl,i) __TPP_PRIVATE_TUPLE_AT(i,__TPP_FORCE_EXPAND tpl)

#endif /* !TPP_TUPLE_AT */
