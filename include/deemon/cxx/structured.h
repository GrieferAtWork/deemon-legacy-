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
#ifndef GUARD_DEEMON_CXX_STRUCTURED_H
#define GUARD_DEEMON_CXX_STRUCTURED_H 1

#include <deemon/__conf.inl>
#include <deemon/cxx/__features.inl>
#include <deemon/structured.h>
#include <deemon/cxx/object.h>
#include <deemon/cxx/type.h>

#if DEE_HAVE_CXX_API
DEE_DECL_CXX_BEGIN
namespace deemon {
namespace detail {
template<class T, Dee_size_t N> struct _typeof_impl<T[N]>: _typeof_impl<T> {
 // Array types
 static inline DEE_A_RET_EXCEPT(NULL) DeeTypeObject *type_() DEE_CXX11_NOEXCEPT {
  DeeTypeObject *result;
  if ((result = _typeof_impl<T>::type_()) != NULL)
   result = DeeType_Array(result,N);
  return result;
 }
 static inline DEE_A_RET_EXCEPT_REF DeeObject *new_(DEE_A_IN T const (&ob)[N]) DEE_CXX11_NOEXCEPT {
  DeeTypeObject *tp_result = type_();
  return tp_result ? DeeStructured_NewFromData(tp_result,(void *)ob) : NULL;
 }
};
template<class ...Args> struct _arg_types_fill;
template<> struct _arg_types_fill<> {
 static inline DEE_A_RET_EXCEPT(-1) int fill(DeeTypeObject **) DEE_CXX11_NOEXCEPT { return 0; }
};
template<class Last> struct _arg_types_fill<Last> {
 static inline DEE_A_RET_EXCEPT(-1) int fill(DeeTypeObject **dst) DEE_CXX11_NOEXCEPT {
  if ((*dst = ::deemon::typeof<Last>()) == NULL) return -1;
  return 0;
 }
};
template<class First, class ...Args> struct _arg_types_fill<First,Args...> {
 static inline DEE_A_RET_EXCEPT(-1) int fill(DeeTypeObject **dst) DEE_CXX11_NOEXCEPT {
  if ((*dst = ::deemon::typeof<First>()) == NULL) return -1;
  return ::deemon::detail::_arg_types_fill<Args...>::fill(dst+1);
 }
};

template<class ReturnType, class ...Args>
struct _typeof_impl<ReturnType(Args...)>: _typeof_impl<ReturnType> {
 // Function types
 static inline DEE_A_RET_EXCEPT(NULL) DeeTypeObject *type_() DEE_CXX11_NOEXCEPT {
  DeeTypeObject *result;
  if ((result = _typeof_impl<T>::type_()) != NULL) {
   DeeTypeObject *arg_types[sizeof...(Args)];
   if (::deemon::detail::_arg_types_fill<Args...>::fill(arg_types) == -1) return NULL;
   result = ::DeeType_ForeignFunction(result,DeeFunctionFlags_NONE,sizeof...(Args),arg_types);
  }
  return result;
 }
 static inline DEE_A_RET_EXCEPT_REF DeeObject *new_(ReturnType(&ob)(Args...)) DEE_CXX11_NOEXCEPT {
  DeeTypeObject *tp_result = type_();
  return tp_result ? DeeStructured_NewFromData(tp_result,(void *)&ob) : NULL;
 }
};
template<class T> struct _typeof_impl<T&>: _typeof_impl<T> {
 // LValue types
 static inline DEE_A_RET_EXCEPT(NULL) DeeTypeObject *type_() DEE_CXX11_NOEXCEPT {
  DeeTypeObject *result;
  if ((result = _typeof_impl<T>::type_()) != NULL)
   result = DeeType_LValue(result);
  return result;
 }
 static inline DEE_A_RET_EXCEPT_REF DeeObject *new_(T &ob) DEE_CXX11_NOEXCEPT {
  DeeTypeObject *tp_result = type_();
  return tp_result ? DeeLValue_New(tp_result,(void *)&ob) : NULL;
 }
};
template<class T> struct _typeof_impl<T*>: _typeof_impl<T> {
 // Pointer types
 static inline DEE_A_RET_EXCEPT(NULL) DeeTypeObject *type_() DEE_CXX11_NOEXCEPT {
  DeeTypeObject *result;
  if ((result = _typeof_impl<T>::type_()) != NULL)
   result = DeeType_Pointer(result);
  return result;
 }
 static inline DEE_A_RET_EXCEPT_REF DeeObject *new_(T *ob) DEE_CXX11_NOEXCEPT {
  DeeTypeObject *tp_result = type_();
  return tp_result ? DeePointer_New(tp_result,(void *)ob) : NULL;
 }
};
}

inline DEE_A_RET_WUNUSED object object::operator * () const { return object(::DeeObject_Deref(this->ob_ptr),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED object object::operator & () const { return object(::DeeObject_Ref(this->ob_ptr),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED type type::pointer() const { return type(::DeeType_Pointer((::DeeTypeObject *)this->ob_ptr),detail::tag_check_error()); }
inline DEE_A_RET_WUNUSED type type::lvalue() const { return type(::DeeType_LValue((::DeeTypeObject *)this->ob_ptr),detail::tag_check_error()); }
inline DEE_A_RET_WUNUSED type type::array(DEE_A_IN Dee_size_t n_elem) const { return type(::DeeType_Array((::DeeTypeObject *)this->ob_ptr,n_elem),detail::tag_check_error()); }
inline DEE_A_RET_WUNUSED type type::foreign_function(DEE_A_IN Dee_size_t n_args, DEE_A_IN_R(n_args) type const *arg_types) const { return type(::DeeType_ForeignFunction((::DeeTypeObject *)this->ob_ptr,DeeFunctionFlags_DEFAULT,n_args,reinterpret_cast< ::DeeTypeObject *const *>(arg_types)),detail::tag_check_error()); }
inline DEE_A_RET_WUNUSED type type::foreign_function(DEE_A_IN Dee_size_t n_args, DEE_A_IN_R(n_args) ::DeeTypeObject *const *arg_types) const { return type(::DeeType_ForeignFunction((::DeeTypeObject *)this->ob_ptr,DeeFunctionFlags_DEFAULT,n_args,arg_types),detail::tag_check_error()); }
inline DEE_A_RET_WUNUSED type type::foreign_function(DEE_A_IN ::DeeFunctionFlags flags, DEE_A_IN Dee_size_t n_args, DEE_A_IN_R(n_args) type const *arg_types) const { return type(::DeeType_ForeignFunction((::DeeTypeObject *)this->ob_ptr,flags,n_args,reinterpret_cast< ::DeeTypeObject *const *>(arg_types)),detail::tag_check_error()); }
inline DEE_A_RET_WUNUSED type type::foreign_function(DEE_A_IN ::DeeFunctionFlags flags, DEE_A_IN Dee_size_t n_args, DEE_A_IN_R(n_args) ::DeeTypeObject *const *arg_types) const { return type(::DeeType_ForeignFunction((::DeeTypeObject *)this->ob_ptr,flags,n_args,arg_types),detail::tag_check_error()); }
#ifdef GUARD_DEEMON_CXX_TUPLE_H
inline DEE_A_RET_WUNUSED type type::foreign_function(DEE_A_IN tuple const &arg_types) const { return type(::DeeType_ForeignFunction((::DeeTypeObject *)this->ob_ptr,DeeFunctionFlags_DEFAULT,DeeTuple_SIZE(arg_types.priv_get()),(DeeTypeObject *const *)DeeTuple_ELEM(arg_types.priv_get())),detail::tag_check_error()); }
inline DEE_A_RET_WUNUSED type type::foreign_function(DEE_A_IN ::DeeFunctionFlags flags, DEE_A_IN tuple const &arg_types) const { return type(::DeeType_ForeignFunction((::DeeTypeObject *)this->ob_ptr,flags,DeeTuple_SIZE(arg_types.priv_get()),(DeeTypeObject *const *)DeeTuple_ELEM(arg_types.priv_get())),detail::tag_check_error()); }
#endif

}
DEE_DECL_CXX_END
#endif

#endif /* !GUARD_DEEMON_CXX_STRUCTURED_H */
