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

// NOTE: Technically, we can produce up to 63-bits,
//       but rand() must return an int
#define RAND_MAX __INT_MAX__

#pragma push_macro("thread_local")
#undef thread_local
const function rand() -> __builtin_object(0x010A)(__builtin_object(0x0101).thread_local()());
const function srand(seed) { __builtin_object(0x0101).thread_local().seed = seed; }
#pragma pop_macro("thread_local")

#pragma push_macro("malloc")
#pragma push_macro("calloc")
#pragma push_macro("realloc")
#pragma push_macro("free")
#undef malloc
#undef calloc
#undef realloc
#undef free

__if_not_exists(malloc) {
	const malloc  = __builtin_object(0x00DA); // void *malloc(size_t s);
	const calloc  = __builtin_object(0x00DB); // void *calloc(size_t c, size_t s);
	const realloc = __builtin_object(0x00DC); // void *realloc(void *p, size_t s);
	const free    = __builtin_object(0x00DD); // void free(void *p);
}

#pragma pop_macro("malloc")
#pragma pop_macro("calloc")
#pragma pop_macro("realloc")
#pragma pop_macro("free")

#if __has_extension(__exit__)
#pragma push_macro("abort")
#pragma push_macro("atexit")
#pragma push_macro("at_quick_exit")
#pragma push_macro("exit")
#pragma push_macro("quick_exit")
#pragma push_macro("_Exit")
#undef abort
#undef atexit
#undef at_quick_exit
#undef exit
#undef quick_exit
#undef _Exit

const abort = __builtin_object(0x00F0);
const atexit = __builtin_object(0x00F2);
const at_quick_exit = __builtin_object(0x00F2);
const exit = __builtin_object(0x00EF);
const quick_exit = __builtin_object(0x00EF);
const _Exit = __builtin_object(0x00F0);

#pragma pop_macro("abort")
#pragma pop_macro("atexit")
#pragma pop_macro("at_quick_exit")
#pragma pop_macro("exit")
#pragma pop_macro("quick_exit")
#pragma pop_macro("_Exit")
#endif

#pragma push_macro("getenv")
#undef getenv
const getenv = __builtin_object(0x009E);
#pragma pop_macro("getenv")

#pragma push_macro("system")
#pragma push_macro("start")
#pragma push_macro("join")
#undef system
#undef start
#undef join
const function system(__command) {
#pragma warning(suppress: 679)
	__stack local __p = __builtin_object(0x0188)(__command);
	__p.start();
	return __p.join();
};
#pragma pop_macro("system")
#pragma pop_macro("start")
#pragma pop_macro("join")

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

constexpr function atof(s) -> (float)string.from_data(s);
constexpr function atoi(s) -> (int)string.from_data(s);
constexpr function atol(s) -> (long)string.from_data(s);
constexpr function atoll(s) -> (long long)string.from_data(s);
// TODO: bsearch
// TODO: strtod
// TODO: strtof
// TODO: strtol
// TODO: strtold
// TODO: strtoll
// TODO: strtoul
// TODO: strtoull
// TODO: MB_CUR_MAX
// TODO: mblen
// TODO: mbtowc
// TODO: mbstowcs
// TODO: wctomb
// TODO: wcstombs
// TODO: div
// TODO: ldiv
// TODO: lldiv

#pragma push_macro("abs")
#pragma push_macro("labs")
#pragma push_macro("llabs")
#pragma push_macro("size_t")
#undef abs
#undef labs
#undef llabs
#undef size_t

__if_not_exists(abs) {
	const abs = __builtin_object(0x010A);
}
const labs = __builtin_object(0x010A);
const llabs = __builtin_object(0x010A);

__if_not_exists(size_t) {
	const size_t = __SIZE_TYPE__;
}

#pragma pop_macro("abs")
#pragma pop_macro("labs")
#pragma pop_macro("llabs")
#pragma pop_macro("size_t")

#ifndef NULL
#define NULL none
#endif

