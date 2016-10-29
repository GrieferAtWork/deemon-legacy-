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
#ifndef TPP_WHILE
#include "__TPP_STDINC.h"

#define __TPP_PRIVATE_WHILE_0(c,b,s)     s // Final state
#define __TPP_PRIVATE_WHILE_1(c,b,s)\
 __TPP_PRIVATE_WHILE_E(c,b,s,__TPP_FORCE_EXPAND(b(s)))
#define __TPP_PRIVATE_WHILE_E(c,b,s,ns)\
 __TPP_PRIVATE_WHILE_E2(c,b,s,ns)
#define __TPP_PRIVATE_WHILE_E2(c,b,s,ns)\
 __TPP_BASIC_CAT(__TPP_PRIVATE_WHILE_,__TPP_EVAL(!!(c(ns))))(c,b,ns)

//////////////////////////////////////////////////////////////////////////
// State TPP_WHILE(Integral c(State), State b(State), State s)
//  - You can think of it like this:
//    >> while (c(s)) { s = b(s); } return s;
// NOTE: This macro return the last state
// >> #define COND(s)  TPP_TUPLE_AT(s,0) != TPP_TUPLE_AT(s,1)
// >> #define BLOCK(s) (__TPP_EVAL(TPP_TUPLE_AT(s,0)+1),TPP_TUPLE_AT(s,1))
// >> TPP_WHILE(COND,BLOCK,(10,20)) // expands to [(][20][,][20][)]
#define TPP_WHILE(c,b,s) __TPP_BASIC_CAT(__TPP_PRIVATE_WHILE_,__TPP_EVAL(!!(c(s))))(c,b,s)

#endif /* !TPP_WHILE */
