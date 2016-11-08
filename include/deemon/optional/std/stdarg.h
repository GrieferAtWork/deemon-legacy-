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


#ifdef DEE_LIMITED_DEX
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H

#ifndef DEE_PLATFORM_VA_LIST_IS_ARRAY
// formerly: DEE_COMPILER_HAVE_VA_LIST_IS_ARRAY
// >> Confirmed on amd64/x86-64 linux
#if defined(DEE_PLATFORM_UNIX) && defined(DEE_PLATFORM_64_BIT)
# define DEE_PLATFORM_VA_LIST_IS_ARRAY 1
#else
# define DEE_PLATFORM_VA_LIST_IS_ARRAY 0
#endif
#endif /* !DEE_PLATFORM_VA_LIST_IS_ARRAY */


//////////////////////////////////////////////////////////////////////////
// >> va_list *DEE_VA_ARGPTR(va_list &arg);
// Returns the usable address of a va_list from the argument list:
// >> void use_arg(va_list *args) {
// >>   printf("%d\n",DEE_VA_ARG(*args,int));
// >> }
// >> void call_args(va_list args) {
// >>   use_arg(DEE_VA_ARGPTR(args));
// >>   use_arg(DEE_VA_ARGPTR(args));
// >> }
// NOTE: This is what caused deemon v101:1 to crash on x86-64 linux all the time...
// fix_ref: http://stackoverflow.com/questions/8047362/is-gcc-mishandling-a-pointer-to-a-va-list-passed-to-a-function
#ifndef DEE_VA_ARGPTR
#if DEE_PLATFORM_VA_LIST_IS_ARRAY
# define DEE_VA_ARGPTR(arg) ((va_list *)(arg))
#else
# define DEE_VA_ARGPTR(arg) (&(arg))
#endif
#endif /* !DEE_VA_ARGPTR */

//////////////////////////////////////////////////////////////////////////
// >> void DEE_VA_START(va_list &args, T &before_start);
#ifndef DEE_VA_START
#define DEE_VA_START va_start
#endif /* !DEE_VA_START */

//////////////////////////////////////////////////////////////////////////
// >> void DEE_VA_END(va_list &ap);
#ifndef DEE_VA_END
#ifdef _MSC_VER
# define DEE_VA_END(ap) (void)__noop(ap)
#else
# define DEE_VA_END     va_end
#endif
#endif /* !DEE_VA_END */

//////////////////////////////////////////////////////////////////////////
// >> T DEE_VA_ARG(va_list &args, typename T);
#ifndef DEE_VA_ARG
#define DEE_VA_ARG   va_arg
#endif /* !DEE_VA_ARG */

//////////////////////////////////////////////////////////////////////////
// >> void DEE_VA_COPY(va_list &apdst, va_list const &apsrc);
#ifndef DEE_VA_COPY
#ifdef _MSC_VER
// In msvc this is just an external function containing a single assign
// Like seriously: Why not implement that in a macro?
// OH! Forward compatibility? I dare you to change that and break
// code of everyone trying to be as cutting edge as I am!
// hash-tag: #savage
# define DEE_VA_COPY(apdst,apsrc) (void)((apdst)=(apsrc))
#elif defined(va_copy)
# define DEE_VA_COPY va_copy
#elif defined(__va_copy)
# define DEE_VA_COPY __va_copy
#elif DEE_PLATFORM_VA_LIST_IS_ARRAY
# define DEE_VA_COPY(apdst,apsrc) memcpy(apdst,apsrc,sizeof(va_list));
#else
# define DEE_VA_COPY(apdst,apsrc) (void)((apdst)=(apsrc))
#endif
#endif /* !DEE_VA_COPY */

#if defined(_MSC_VER) && (defined(_M_CEE_PURE) ||\
   (defined(_M_CEE) && !defined(_M_ARM)))
// This one uses unspecific *real* external function calls
// >> So lets try not to mess with it (this time...)
#ifdef va_end
#undef DEE_VA_END
#define DEE_VA_END  va_end
#endif /* va_end */
#ifdef va_copy
#undef DEE_VA_COPY
#define DEE_VA_COPY va_copy
#endif /* va_copy */
#endif

#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */
#endif /* DEE_LIMITED_DEX */

#endif /* !GUARD_DEEMON_OPTIONAL_STD_STDARG_H */
