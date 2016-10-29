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
#ifndef TPP_REPEAT
#include "__TPP_STDINC.h"

#define __TPP_PRIVATE_REPEAT_NEXT(b,s,c,m,d) __TPP_BASIC_CAT2(__TPP_PRIVATE_REPEAT_,b)(s,c,m,d)
#define __TPP_PRIVATE_REPEAT_0(s,c,m,d) /* break; */
#define __TPP_PRIVATE_REPEAT_1(s,c,m,d) m(s,d)__TPP_PRIVATE_REPEAT_NEXT(__TPP_EVAL(s+1 < (c)),__TPP_EVAL(s+1),c,m,d)

//////////////////////////////////////////////////////////////////////////
// Repeats a given macro "m" "c" times
// >> Param c: Integer
//    >> Constant expression for the amount of times, to repeat "m"
// >> Param m: macro(Integer i, Data d)
//    >> Macro to be repeated
// >> Param d: Data
//    >> Second parameter for calls to "m"
#define TPP_REPEAT(c,m,d) __TPP_PRIVATE_REPEAT_NEXT(__TPP_EVAL(c > 0),0,c,m,d)

#endif /* !TPP_REPEAT */
