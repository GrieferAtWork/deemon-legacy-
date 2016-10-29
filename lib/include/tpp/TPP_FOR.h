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
#ifndef TPP_FOR
#include "__TPP_STDINC.h"

#define __TPP_PRIVATE_FOR_0(s,p,n,m) /*break;*/
#define __TPP_PRIVATE_FOR_1(s,p,n,m)\
 m(s)__TPP_PRIVATE_FOR_E(s,p,n,m,n(s))
#define __TPP_PRIVATE_FOR_E(s,p,n,m,ns)\
 __TPP_PRIVATE_FOR_E2(s,p,n,m,ns)
#define __TPP_PRIVATE_FOR_E2(s,p,n,m,ns)\
 __TPP_BASIC_CAT(__TPP_PRIVATE_FOR_,__TPP_EVAL(!!(p(ns))))(ns,p,n,m)

//////////////////////////////////////////////////////////////////////////
// PP_FOR(State start, Bool pred(State s), State next(State s), Code macro(State s))
// Example:
// >> #define PRED(s) TPP_TUPLE_AT(s,0) != TPP_TUPLE_AT(s,1)
// >> #define NEXT(s) (__TPP_EVAL(TPP_TUPLE_AT(s,0)-1),TPP_TUPLE_AT(s,1))
// >> #define ITER(s) __pragma(message("for(" TPP_STR(TPP_TUPLE_AT(s,0)) ")"))
// >> // Prints "for(2)" "for(1)" "for(0)" "for(-1)"
// >> TPP_FOR((2,-2),PRED,NEXT,ITER)
#define TPP_FOR(s,p,n,m) __TPP_BASIC_CAT(__TPP_PRIVATE_FOR_,__TPP_EVAL(!!(p(s))))(s,p,n,m)

#endif /* !TPP_FOR */
