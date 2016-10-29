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
#ifndef GUARD_DEEMON___TYPECOMPATIBLE_INL
#define GUARD_DEEMON___TYPECOMPATIBLE_INL 1

#include <deemon/__conf.inl>

#if defined(__INTELLISENSE__) && defined(__cplusplus) && (\
   (defined(_MSC_VER) && _MSC_VER >= 1800))
namespace deemon { namespace detail {

template<class T> struct cxx_novc2 { typedef T type; };
template<class T> struct cxx_novc2<T const> { typedef T type; };
template<class T> struct cxx_novc2<T volatile> { typedef T type; };
template<class T> struct cxx_novc2<T const volatile> { typedef T type; };
template<class T> struct cxx_novc: cxx_novc2<T> {};
template<class T> struct cxx_novc<T&>: cxx_novc2<T> {};
//template<class T> struct cxx_novc<T&&>: cxx_novc2<T> {};
template<class T, class S> struct require_typecompatible_base2 { enum{value=false}; };
template<class T> struct require_typecompatible_base2<T,T> { enum{value=true}; };
template<class T, class S> struct require_typecompatible_base: require_typecompatible_base2<T,S> {};
template<class T> struct require_typecompatible_base<T,void *> { enum{value=true}; };
template<class T> struct require_typecompatible_base<T,int> { enum{value=true}; };
template<class T> struct require_typecompatible_base<T,decltype(nullptr)> { enum{value=true}; };
template<class T, class S> struct require_typecompatible_impl: require_typecompatible_base<T,S> {};
template<class T, class S> struct require_typecompatible_pointer { enum{value=true}; };
#if 1
template<class T> struct require_typecompatible_impl<T *,T *> { enum{value=true}; };
template<class T, class S> struct require_typecompatible_impl<T *,S *>:
 require_typecompatible_pointer<typename cxx_novc<T>::type,typename cxx_novc<S>::type> {};
template<bool ...Value> struct tpl_and { enum{value = true}; };
template<bool ...Value> struct tpl_and<true,Value...>: tpl_and<Value...> {};
template<bool ...Value> struct tpl_and<false,Value...> { enum{value=false}; };
#if !defined(__INTELLISENSE__)
template<class RetT, class ...ArgT, class RetS, class ...ArgS>
struct require_typecompatible_pointer<RetT(ArgT...),RetS(ArgS...)>: tpl_and<
 sizeof...(ArgT) == sizeof...(ArgS) && require_typecompatible_impl<RetT,RetS>::value &&
 require_typecompatible_impl<ArgT,ArgS>::value...> {};
#else
template<class RetT, class ...ArgT, class RetS, class ...ArgS>
struct require_typecompatible_pointer<RetT(ArgT...),RetS(ArgS...)>: tpl_and<
 sizeof...(ArgT) == sizeof...(ArgS) && require_typecompatible_impl<RetT,RetS>::value,
 require_typecompatible_impl<ArgT,ArgS>::value...> {};
#endif
template<class T, class S> struct require_typecompatible_pointer<T *,S *>:
 require_typecompatible_pointer<typename cxx_novc<T>::type,typename cxx_novc<S>::type> {};
template<class T, class S> struct require_typecompatible_pointer<T *,S> { enum{value=false}; };
template<class T, class S> struct require_typecompatible_pointer<T,S *> { enum{value=false}; };
#endif
template<class T, bool enabled> struct require_typecompatible2 { typedef T type; };
template<class T> struct require_typecompatible2<T,false> {};
#if 1
template<class T, class S> struct require_typecompatible:
 require_typecompatible2<T,require_typecompatible_impl<
  typename cxx_novc<T>::type,
  typename cxx_novc<S>::type
 >::value> {};
#else
template<class T, class S> struct require_typecompatible {typedef T type;};
#endif

}}

// Type-safety checks for callbacks in types
// >> DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_str),&my_callback)
// >> // Returns 'my_callback' castet to 'DeeType_SLOT_TYPE(tp_str)', but causes a compiler
// >> // error if it isn't compatible with the current typing of 'DeeType_SLOT_TYPE(tp_str)'
// >> // NOTE: Also enforces the '&' before 'my_callback'
#define DEE_REQUIRE_TYPECOMPATIBLE(T,ob) \
 (deemon::detail::require_typecompatible< T,decltype(ob)>::type)(ob)
#else
#define DEE_REQUIRE_TYPECOMPATIBLE(T,ob) (T)(ob)
#endif



#endif /* !GUARD_DEEMON___TYPECOMPATIBLE_INL */
