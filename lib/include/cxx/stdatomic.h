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
#include <__cxxdefault.dee>

#include <atomic>
#include <cxx/__deemon_atomic_api.dee>

#ifndef ATOMIC_VAR_INIT
#define ATOMIC_VAR_INIT(value)  value
#endif
#ifndef ATOMIC_FLAG_INIT
#define ATOMIC_FLAG_INIT        0
#endif

// TODO: What if they aren't?
#define ATOMIC_BOOL_LOCK_FREE     1
#define ATOMIC_CHAR_LOCK_FREE     1
#define ATOMIC_CHAR16_T_LOCK_FREE 1
#define ATOMIC_CHAR32_T_LOCK_FREE 1
#define ATOMIC_WCHAR_T_LOCK_FREE  1
#define ATOMIC_SHORT_LOCK_FREE    1
#define ATOMIC_INT_LOCK_FREE      1
#define ATOMIC_LONG_LOCK_FREE     1
#define ATOMIC_LLONG_LOCK_FREE    1
#define ATOMIC_POINTER_LOCK_FREE  1

#ifndef kill_dependency
#define kill_dependency(x) x
#endif

#pragma push_macro("atomic_thread_fence")
#undef atomic_thread_fence
constexpr function atomic_thread_fence(__order) -> none; // TODO ?
#pragma pop_macro("atomic_thread_fence")

#pragma push_macro("atomic_signal_fence")
#undef atomic_signal_fence
constexpr function atomic_signal_fence(__order) -> none; // TODO ?
#pragma pop_macro("atomic_signal_fence")

__if_not_exists(memory_order_relaxed) {
	const memory_order_relaxed = __MEMORY_ORDER_RELAXED;
	const memory_order_consume = __MEMORY_ORDER_CONSUME;
	const memory_order_acquire = __MEMORY_ORDER_ACQUIRE;
	const memory_order_release = __MEMORY_ORDER_RELEASE;
	const memory_order_acq_rel = __MEMORY_ORDER_ACQ_REL;
	const memory_order_seq_cst = __MEMORY_ORDER_SEQ_CST;
}

const atomic_bool = _Atomic __unsigned__ __int8;
const atomic_char = _Atomic char;
const atomic_schar = _Atomic __signed__ char;
const atomic_uchar = _Atomic __unsigned__ char;
const atomic_short = _Atomic short;
const atomic_ushort = _Atomic __unsigned__ short;
const atomic_int = _Atomic int;
const atomic_uint = _Atomic __unsigned__ int;
const atomic_long = _Atomic long;
const atomic_ulong = _Atomic __unsigned__ long;
const atomic_llong = _Atomic long long;
const atomic_ullong = _Atomic __unsigned__ long long;
const atomic_wchar_t = _Atomic wchar_t;
const atomic_char16_t = _Atomic char16_t;
const atomic_char32_t = _Atomic char32_t;
const atomic_intmax_t = _Atomic __INTMAX_TYPE__;
const atomic_uintmax_t = _Atomic __UINTMAX_TYPE__;
#ifndef __STDC_STRICT__
const atomic_int8_t = _Atomic __INT8_TYPE__;
const atomic_int16_t = _Atomic __INT16_TYPE__;
const atomic_int32_t = _Atomic __INT32_TYPE__;
const atomic_int64_t = _Atomic __INT64_TYPE__;
const atomic_uint8_t = _Atomic __UINT8_TYPE__;
const atomic_uint16_t = _Atomic __UINT16_TYPE__;
const atomic_uint32_t = _Atomic __UINT32_TYPE__;
const atomic_uint64_t = _Atomic __UINT64_TYPE__;
#endif
const atomic_int_least8_t = _Atomic __INT_LEAST8_TYPE__;
const atomic_int_least16_t = _Atomic __INT_LEAST16_TYPE__;
const atomic_int_least32_t = _Atomic __INT_LEAST32_TYPE__;
const atomic_int_least64_t = _Atomic __INT_LEAST64_TYPE__;
const atomic_uint_least8_t = _Atomic __UINT_LEAST8_TYPE__;
const atomic_uint_least16_t = _Atomic __UINT_LEAST16_TYPE__;
const atomic_uint_least32_t = _Atomic __UINT_LEAST32_TYPE__;
const atomic_uint_least64_t = _Atomic __UINT_LEAST64_TYPE__;
const atomic_int_fast8_t = _Atomic __INT_FAST8_TYPE__;
const atomic_int_fast16_t = _Atomic __INT_FAST16_TYPE__;
const atomic_int_fast32_t = _Atomic __INT_FAST32_TYPE__;
const atomic_int_fast64_t = _Atomic __INT_FAST64_TYPE__;
const atomic_uint_fast8_t = _Atomic __UINT_FAST8_TYPE__;
const atomic_uint_fast16_t = _Atomic __UINT_FAST16_TYPE__;
const atomic_uint_fast32_t = _Atomic __UINT_FAST32_TYPE__;
const atomic_uint_fast64_t = _Atomic __UINT_FAST64_TYPE__;
const atomic_intptr_t = _Atomic __INTPTR_TYPE__;
const atomic_uintptr_t = _Atomic __UINTPTR_TYPE__;
const atomic_size_t = _Atomic __SIZE_TYPE__;
const atomic_ptrdiff_t = _Atomic __PTRDIFF_TYPE__;

const atomic_flag = _Atomic __unsigned__ __CAT(__int,__TPP_EVAL(8*__SIZEOF_BOOL__));
