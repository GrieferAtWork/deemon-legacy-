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
#ifndef TPP_CAT
#include "__TPP_STDINC.h"
#include "TPP_VA_NARGS.h"

#define TPP_CAT_0()                  /* nothing */
#define TPP_CAT_1(a)                 a
#define TPP_CAT_2(a,b)               a ## b
#define TPP_CAT_3(a,b,c)             a ## b ## c
#define TPP_CAT_4(a,b,c,d)           a ## b ## c ## d
#define TPP_CAT_5(a,b,c,d,e)         a ## b ## c ## d ## e
#define TPP_CAT_6(a,b,c,d,e,f)       a ## b ## c ## d ## e ## f
#define TPP_CAT_7(a,b,c,d,e,f,g)     a ## b ## c ## d ## e ## f ## g
#define TPP_CAT_8(a,b,c,d,e,f,g,h)   a ## b ## c ## d ## e ## f ## g ## h
#define TPP_CAT_9(a,b,c,d,e,f,g,h,i) a ## b ## c ## d ## e ## f ## g ## h ## i

#define TPP_CAT(...) __TPP_BASIC_CAT(TPP_CAT_,TPP_VA_NARGS(__VA_ARGS__))(__VA_ARGS__)

#endif /* !TPP_CAT */
