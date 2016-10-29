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
#ifndef TPP_IF
#include "__TPP_STDINC.h"

#define __TPP_PRIVATE_MIF_0(mt,dt,mf,df) mf(df)
#define __TPP_PRIVATE_MIF_1(mt,dt,mf,df) mt(dt)
#define __TPP_WRITE_COMMA(d) ,

//////////////////////////////////////////////////////////////////////////
// Param c:   Integral    // If-condition
// Param mt:  Macro(Data) // Macro called, with dt, if "c" is true
// Param dt:  Data        // Argument for mt
// Param mf:  Macro(Data) // Macro called, with dt, if "c" is false
// Param df:  Data        // Argument for mf
#define TPP_MIF(c,mt,dt,mf,df)\
 __TPP_BASIC_CAT(__TPP_PRIVATE_MIF_,__TPP_EVAL(!!(c)))(mt,dt,mf,df)

//////////////////////////////////////////////////////////////////////////
// Param c:   Integral // If-condition
// Param tt:  Tuple    // Tuple expanded, if "c" is true
// Param ft:  Tuple    // Tuple expanded, if "c" is false
#define TPP_IF(c,tt,ft) \
 __TPP_BASIC_CAT(__TPP_PRIVATE_MIF_,__TPP_EVAL(!!(c)))(__TPP_BASIC_EXPAND_TUPLE,tt,__TPP_BASIC_EXPAND_TUPLE,ft)

//////////////////////////////////////////////////////////////////////////
// Param c:   Integral // If-condition
// Param tt:  Code     // Code written, if "c" is true
// Param ft:  Code     // Code written, if "c" is false
#define TPP_IIF(c,t,f) \
 __TPP_BASIC_CAT(__TPP_PRIVATE_MIF_,__TPP_EVAL(!!(c)))(__TPP_FORCE_EXPAND,t,__TPP_FORCE_EXPAND,f)


//////////////////////////////////////////////////////////////////////////
// Param c:   Integral // If-condition
// Expands to a comma token [,] if "c" is true
#define TPP_COMMA_IF(c) TPP_MIF(c,__TPP_WRITE_COMMA,~,__TPP_EAT_ARGS,~)

#endif /* !TPP_IF */
