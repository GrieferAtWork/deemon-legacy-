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
#ifndef GUARD_DEEMON_CXX___FEATURES_INL
#define GUARD_DEEMON_CXX___FEATURES_INL 1

#include <deemon/__conf.inl>

#ifndef DEE_CXX_DELETE_CLASSDEFAULT
#if __has_feature(deleted_functions) || (defined(__cplusplus) &&\
   (defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 180020827))
#define DEE_CXX_DELETE_CLASSDEFAULT(T)\
 T() = delete;\
 T(T const&) = delete;\
 T &operator = (T const&) = delete;
#else
#define DEE_CXX_DELETE_CLASSDEFAULT(T)\
private:\
 T();\
 T(T const&);\
 T &operator = (T const&);\
public:
#endif
#endif

#ifndef DEE_STATIC_CONST
#if __has_feature(cxx_constexpr) || \
   (defined(__cpp_constexpr) && __cpp_constexpr >= 200704) || (defined(__cplusplus) && \
   (defined(__IBMCPP__) && defined(__IBMCPP_CONSTEXPR) && __IBMCPP_CONSTEXPR) || \
   (defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x5130) || \
   (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)) && \
   (defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L)) || \
   (defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 190023026))
# define DEE_STATIC_CONST(T,decl) static constexpr T decl
#elif defined(__cplusplus) && (\
   (!defined(__SUNPRO_CC) || __SUNPRO_CC > 0x520) &&\
   (!defined(__IBMCPP__) || __IBMCPP__ > 502) &&\
   (!defined(MPW_CPLUS) || MPW_CPLUS > 0x890))
# define DEE_STATIC_CONST(T,decl) static T const decl
#else
# define DEE_STATIC_CONST(T,decl) enum{decl}
#endif
#endif

#ifndef DEE_CXX11_NOEXCEPT
#if defined(__cplusplus) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS) && \
   (__has_feature(cxx_noexcept) || \
   (defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46) || \
   (defined(_MSC_FULL_VER) && (_MSC_FULL_VER >= 190023026)))
# define DEE_CXX11_NOEXCEPT noexcept
#elif defined(__cplusplus) && (!defined(_HAS_EXCEPTIONS) || _HAS_EXCEPTIONS)
# define DEE_CXX11_NOEXCEPT throw()
#else
# define DEE_CXX11_NOEXCEPT /* nothing */
#endif
#endif /* !DEE_CXX11_NOEXCEPT */


#endif /* !GUARD_DEEMON_CXX___FEATURES_INL */
