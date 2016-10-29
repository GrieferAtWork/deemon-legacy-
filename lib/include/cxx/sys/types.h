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

#if 0
__if_not_exists(u_char) { const u_char = __unsigned__ char; }
__if_not_exists(u_short) { local const u_short = __unsigned__ short; }
__if_not_exists(u_int) { local const u_int = __unsigned__ int; }
__if_not_exists(u_long) { local const u_long = __unsigned__ long; }
__if_not_exists(quad_t) { local const quad_t = __signed__ __int64; }
__if_not_exists(u_quad_t) { local const u_quad_t = __unsigned__ __int64; }
//__if_not_exists(fsid_t) { struct fsid_t { int __val[2]; }; }
#endif

__if_not_exists(time_t) { const time_t = __CAT(__int,__TPP_EVAL(__SIZEOF_TIME_T__*8)); }
__if_not_exists(ino_t) { const ino_t = __unsigned__ short; }
__if_not_exists(dev_t) { const dev_t = __unsigned__ int; }
__if_not_exists(off_t) { const off_t = __signed__ __int64; }
