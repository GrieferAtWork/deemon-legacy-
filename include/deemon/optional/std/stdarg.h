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
#ifndef GUARD_DEEMON_OPTIONAL_STD_STDARG_H
#define GUARD_DEEMON_OPTIONAL_STD_STDARG_H 1

#include <deemon/__conf.inl>
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#include <stdarg.h>
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */


#ifdef DEE_LIMITED_API
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
//////////////////////////////////////////////////////////////////////////
// >> va_list *DEE_VA_ARGPTR(va_list &arg);
// Returns the usable address of a va_list from the argument list:
// >> void use_arg(va_list *args) {
// >>   printf("%d\n",va_arg(*args,int));
// >> }
// >> void call_args(va_list args) {
// >>   use_arg(DEE_VA_ARGPTR(args));
// >>   use_arg(DEE_VA_ARGPTR(args));
// >> }
// NOTE: This is what caused deemon v101:1 to crash on x86-64 linux all the time...
// fix_ref: http://stackoverflow.com/questions/8047362/is-gcc-mishandling-a-pointer-to-a-va-list-passed-to-a-function
#if DEE_PLATFORM_VA_LIST_IS_ARRAY
# define DEE_VA_ARGPTR(arg) ((va_list *)(arg))
#else
# define DEE_VA_ARGPTR(arg) (&(arg))
#endif

#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */
#endif

#endif /* !GUARD_DEEMON_OPTIONAL_STD_STDARG_H */
