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
#include <cxx/__deemon_stdint.dee>

__if_not_exists(int_least8_t) {
	const int_least8_t = __INT_LEAST8_TYPE__;
	const int_least16_t = __INT_LEAST16_TYPE__;
	const int_least32_t = __INT_LEAST32_TYPE__;
	const int_least64_t = __INT_LEAST64_TYPE__;
	const uint_least8_t = __UINT_LEAST8_TYPE__;
	const uint_least16_t = __UINT_LEAST16_TYPE__;
	const uint_least32_t = __UINT_LEAST32_TYPE__;
	const uint_least64_t = __UINT_LEAST64_TYPE__;
}

__if_not_exists(int_fast8_t) {
	const int_fast8_t = __INT_FAST8_TYPE__;
	const int_fast16_t = __INT_FAST16_TYPE__;
	const int_fast32_t = __INT_FAST32_TYPE__;
	const int_fast64_t = __INT_FAST64_TYPE__;
	const uint_fast8_t = __UINT_FAST8_TYPE__;
	const uint_fast16_t = __UINT_FAST16_TYPE__;
	const uint_fast32_t = __UINT_FAST32_TYPE__;
	const uint_fast64_t = __UINT_FAST64_TYPE__;
}

__if_not_exists(intmax_t) {
	const intmax_t = __int64;
	const uintmax_t = __unsigned__ __int64;
}

__if_not_exists(intptr_t) {
	const intptr_t = __INTPTR_TYPE__;
	const uintptr_t = __UINTPTR_TYPE__;
}
