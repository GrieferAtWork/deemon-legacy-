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
#ifndef GUARD_DEEMON_ALLOCA_INL
#define GUARD_DEEMON_ALLOCA_INL 1

#include <deemon/__conf.inl>

#if !defined(DEE_HAVE_ALLOCA) || DEE_HAVE_ALLOCA
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#if DEE_ENVIRONMENT_HAVE_INCLUDE_MALLOC_H
DEE_COMPILER_MSVC_WARNING_PUSH(4820)
#include <malloc.h> // alloca
DEE_COMPILER_MSVC_WARNING_POP
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_ALLOCA_H
#include <alloca.h> // alloca
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif

#ifndef DEE_HAVE_ALLOCA
#ifdef alloca
#define DEE_HAVE_ALLOCA 1
#endif
#endif

#if DEE_ENVIRONMENT_HAVE_INCLUDE_ALLOCA_H
#undef DEE_HAVE_ALLOCA
#define DEE_HAVE_ALLOCA 1
#endif

#ifndef DEE_HAVE_ALLOCA
#define DEE_HAVE_ALLOCA 0
#endif


#if !DEE_HAVE_ALLOCA
#if defined(__GNUC__) || \
    __has_builtin(__builtin_alloca)
#undef DEE_HAVE_ALLOCA
#define DEE_HAVE_ALLOCA 1
#define alloca   __builtin_alloca
#endif
#endif

#endif /* !GUARD_DEEMON_ALLOCA_INL */
