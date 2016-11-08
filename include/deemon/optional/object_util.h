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
#ifndef GUARD_DEEMON_OPTIONAL_OBJECT_UTIL_H
#define GUARD_DEEMON_OPTIONAL_OBJECT_UTIL_H 1

#include <deemon/__conf.inl>

#if defined(__INTELLISENSE__) && defined(__cplusplus)
extern "C++" {
 template<class T> inline T *_Dee_____COMPILER_ERROR___REQUIRES_POINTER(T *);
 template<class T> inline T const *_Dee_____COMPILER_ERROR___REQUIRES_POINTER(T const *);
 template<class T> inline T volatile *_Dee_____COMPILER_ERROR___REQUIRES_POINTER(T volatile *);
 template<class T> inline T const volatile *_Dee_____COMPILER_ERROR___REQUIRES_POINTER(T const volatile *);
}
#define Dee_REQUIRES_POINTER _Dee_____COMPILER_ERROR___REQUIRES_POINTER
#elif defined(__INTELLISENSE__)/* && defined(__cplusplus)*/
// NOTE: When compiling in c, this check sometimes crashes the compiler... (idk why, though...)
// Just a small macro, that creates a compiler error if 'p' isn't a pointer
// NOTE: Only used to happen with the real compiler, not intellisense though
DEE_STATIC_INLINE(void) _Dee_____COMPILER_ERROR___REQUIRES_POINTER(void const volatile *);
#define Dee_REQUIRES_POINTER(p) (__noop(_Dee_____COMPILER_ERROR___REQUIRES_POINTER(p)),p)
#else /* DEE_DEBUG */
#define Dee_REQUIRES_POINTER    /* nothing */
#endif /* !DEE_DEBUG */

//////////////////////////////////////////////////////////////////////////
// Wrapper for doc strings
#ifndef Dee_DOCSTRING
#if DEE_CONFIG_RUNTIME_HAVE_DOCSTRINGS
# define Dee_DOCSTRING(x) x
#else
# define Dee_DOCSTRING(x) NULL
#endif
#endif


//////////////////////////////////////////////////////////////////////////
// Initial starting value for all hash functions
#ifndef DEE_HASH_START
#define DEE_HASH_START 0
#endif

#endif /* !GUARD_DEEMON_OPTIONAL_OBJECT_UTIL_H */
