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

#ifndef offsetof
#if __has_builtin(__builtin_offsetof)
#	define offsetof          __builtin_offsetof
#else
#	define offsetof(__t,__m) __SIZE_TYPE__(&__builtin_object(0x0086)(__t)(none)->__m)
#endif
#endif

__if_not_exists(ptrdiff_t) {
	const ptrdiff_t = __PTRDIFF_TYPE__;
}
__if_not_exists(size_t) {
	const size_t = __SIZE_TYPE__;
}

//__if_not_exists(max_align_t) {
//	const max_align_t = ?;
//}

__if_not_exists(ssize_t) {
	const ssize_t = ptrdiff_t;
}

#ifndef NULL
#define NULL none
#endif

__if_not_exists(nullptr_t) {
	const nullptr_t = __builtin_typeof(NULL);
}
