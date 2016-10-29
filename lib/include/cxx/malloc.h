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

#ifdef __STDC__
#warning <malloc.h> is not STDC (use <stdlib.h> instead)
#endif

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

#pragma push_macro("cfree")
#undef cfree
const cfree = __builtin_object(0x00DD); // void cfree(void *p);
#pragma pop_macro("cfree")
