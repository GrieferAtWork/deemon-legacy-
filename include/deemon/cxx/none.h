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
#ifndef GUARD_DEEMON_CXX_NONE_H
#define GUARD_DEEMON_CXX_NONE_H 1

#include <deemon/__conf.inl>
#include <deemon/cxx/__features.inl>
#include <deemon/none.h>
#include <deemon/cxx/object.h>

#if DEE_HAVE_CXX_API
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#include <cstddef>
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_CXX_BEGIN
namespace deemon {
namespace detail {
template<> struct _typeof_impl<void> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(void); }
 static inline DeeObject *new_(void) DEE_CXX11_NOEXCEPT { DeeReturn_None; }
};
template<> struct _typeof_impl< ::std::nullptr_t > {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(nullptr_t); }
 static inline DeeObject *new_(::std::nullptr_t) DEE_CXX11_NOEXCEPT { DeeReturn_None; }
};
}

namespace detail {
template<class T> struct _none_enableif{};
template<class T> struct _none_enableif<T const>: _none_enableif<T>{};
template<class T> struct _none_enableif<T volatile>: _none_enableif<T>{};
template<class T> struct _none_enableif<T const volatile>: _none_enableif<T>{};
template<> struct _none_enableif<void>{typedef DeeObject *type;};
template<> struct _none_enableif< ::std::nullptr_t>{typedef DeeObject *type;};
}
// Special handling for empty parameter list on void and nullptr_t
template<class T> inline DEE_A_RET_EXCEPT_REF typename ::deemon::detail::
 _none_enableif<T>::type new_(void) DEE_CXX11_NOEXCEPT { DeeReturn_None; }
inline DEE_A_RET_EXCEPT_REF DeeObject *new_(void) DEE_CXX11_NOEXCEPT { DeeReturn_None; }

struct none: object {
 public: /* public operators */
  inline DEE_A_RET_WUNUSED operator bool () const DEE_CXX11_NOEXCEPT { return false; }
  inline DEE_A_RET_WUNUSED bool operator ! () const DEE_CXX11_NOEXCEPT { return true; }
  inline none &operator ()(...) DEE_CXX11_NOEXCEPT { return *this; }
  inline none const &operator ()(...) const DEE_CXX11_NOEXCEPT { return *this; }
  inline DEE_A_RET_WUNUSED bool operator < (DeeObject const *rhs) const DEE_CXX11_NOEXCEPT { return !DeeNone_Check(rhs); }
  inline DEE_A_RET_WUNUSED bool operator <= (DeeObject const *DEE_UNUSED(rhs)) const DEE_CXX11_NOEXCEPT { return true; }
  inline DEE_A_RET_WUNUSED bool operator == (DeeObject const *rhs) const DEE_CXX11_NOEXCEPT { return DeeNone_Check(rhs); }
  inline DEE_A_RET_WUNUSED bool operator != (DeeObject const *rhs) const DEE_CXX11_NOEXCEPT { return !DeeNone_Check(rhs); }
  inline DEE_A_RET_WUNUSED bool operator > (DeeObject const *DEE_UNUSED(rhs)) const DEE_CXX11_NOEXCEPT { return false; }
  inline DEE_A_RET_WUNUSED bool operator >= (DeeObject const *rhs) const DEE_CXX11_NOEXCEPT { return DeeNone_Check(rhs); }
#define DEE_DECL_NONE_OPERATOR(symb)\
  inline none &operator symb() DEE_CXX11_NOEXCEPT { return *this; }\
  inline none const &operator symb() const DEE_CXX11_NOEXCEPT { return *this; }
  DEE_DECL_NONE_OPERATOR(+)
  DEE_DECL_NONE_OPERATOR(-)
  DEE_DECL_NONE_OPERATOR(++)
  DEE_DECL_NONE_OPERATOR(--)
#undef DEE_DECL_NONE_OPERATOR
#define DEE_DECL_NONE_OPERATOR(symb)\
  inline none &operator symb(int) DEE_CXX11_NOEXCEPT { return *this; }\
  inline none const &operator symb(int) const DEE_CXX11_NOEXCEPT { return *this; }
  DEE_DECL_NONE_OPERATOR(++)
  DEE_DECL_NONE_OPERATOR(--)
#undef DEE_DECL_NONE_OPERATOR
#define DEE_DECL_NONE_OPERATOR(symb)\
  template<class T> inline none &operator symb(T const&) DEE_CXX11_NOEXCEPT { return *this; }\
  template<class T> inline none const &operator symb(T const&) const DEE_CXX11_NOEXCEPT { return *this; }
  DEE_DECL_NONE_OPERATOR(+)  DEE_DECL_NONE_OPERATOR(+=)
  DEE_DECL_NONE_OPERATOR(-)  DEE_DECL_NONE_OPERATOR(-=)
  DEE_DECL_NONE_OPERATOR(*)  DEE_DECL_NONE_OPERATOR(*=)
  DEE_DECL_NONE_OPERATOR(/)  DEE_DECL_NONE_OPERATOR(/=)
  DEE_DECL_NONE_OPERATOR(%)  DEE_DECL_NONE_OPERATOR(%=)
  DEE_DECL_NONE_OPERATOR(<<) DEE_DECL_NONE_OPERATOR(<<=)
  DEE_DECL_NONE_OPERATOR(>>) DEE_DECL_NONE_OPERATOR(>>=)
  DEE_DECL_NONE_OPERATOR(&)  DEE_DECL_NONE_OPERATOR(&=)
  DEE_DECL_NONE_OPERATOR(|)  DEE_DECL_NONE_OPERATOR(|=)
  DEE_DECL_NONE_OPERATOR(^)  DEE_DECL_NONE_OPERATOR(^=)
  DEE_DECL_NONE_OPERATOR([])
#undef DEE_DECL_NONE_OPERATOR
 public: /* public constructor / destructor */
  inline none(...) DEE_CXX11_NOEXCEPT: object(Dee_None) {}
  DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(none,object,DeeNone_Check)
};

}
DEE_DECL_CXX_END
#endif

#endif /* !GUARD_DEEMON_CXX_NONE_H */
