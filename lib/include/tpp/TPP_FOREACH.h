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
#ifndef TPP_FOREACH
#include "__TPP_STDINC.h"

#define __TPP_PRIVATE_FOREACH_NEMPTY2(p,r,...) r
#define __TPP_PRIVATE_FOREACH_0(m,d,...) /* break */
#define __TPP_PRIVATE_FOREACH_1(m,d,first,...)\
m(first,d)__TPP_PRIVATE_FOREACH_E(m,d,__VA_ARGS__)
#define __TPP_PRIVATE_FOREACH_E(m,d,...)\
__TPP_BASIC_CAT(__TPP_PRIVATE_FOREACH_,__TPP_PRIVATE_FOREACH_NEMPTY2(__VA_COMMA__ 1,0))(m,d,__VA_ARGS__)
#define __TPP_PRIVATE_FOREACH_C(m,d,...)\
__TPP_PRIVATE_FOREACH_E(m,d,__VA_ARGS__)


//////////////////////////////////////////////////////////////////////////
// Param tpl: Tuple
// Param m:   Macro(Data e, Data d)
// Param d:   Data // Second argument, when calling "m"
// >> #define ITER(e,d) __pragma(message("ITER: " #e))
// >> TPP_FOREACH((),ITER,~) // prints nothing
// >> TPP_FOREACH((10,20,30,40),ITER,~)
// >> // prints: "ITER: 10", "ITER: 20", "ITER: 30", "ITER: 40"
#define TPP_FOREACH(tpl,m,d) __TPP_PRIVATE_FOREACH_C(m,d,__TPP_FORCE_EXPAND tpl)

#endif /* !TPP_FOREACH */
