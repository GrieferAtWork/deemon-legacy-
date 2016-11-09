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
#ifndef GUARD_DEEMON_CXX_TUPLE_H
#define GUARD_DEEMON_CXX_TUPLE_H 1

#include <deemon/__conf.inl>
#include <deemon/cxx/__features.inl>
#include <deemon/tuple.h>
#include <deemon/cxx/object.h>
#include <deemon/optional/std/stdarg.h>

#if DEE_HAVE_CXX_API
DEE_DECL_CXX_BEGIN
namespace deemon {

struct list;
struct tuple: object {
 public: /* public functions */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
  static inline DEE_A_RET_WUNUSED tuple pack(DEE_A_IN Dee_size_t n, ...) {
   DeeObject *result; va_list args;
   DEE_VA_START(args,n);
   result = _DeeTuple_VPack(n,args);
   DEE_VA_END(args);
   return tuple(result,detail::tag_ref_or_err());
  }
  static inline DEE_A_RET_WUNUSED tuple vpack(DEE_A_IN Dee_size_t n, DEE_A_INOUT va_list args) {
   return tuple(_DeeTuple_VPack(n,args),detail::tag_ref_or_err());
  }
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */
 public: /* public operators */
  inline DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL object *elem() const DEE_CXX11_NOEXCEPT { return reinterpret_cast<object *>(DeeTuple_ELEM(this->ob_ptr)); }
  inline DEE_A_RET_WUNUSED Dee_size_t size() const DEE_CXX11_NOEXCEPT { return DeeTuple_SIZE(this->ob_ptr); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED tuple types() const { return tuple(::DeeTuple_Types(this->ob_ptr),detail::tag_ref_or_err()); }
 public: /* public methods */
#ifdef DEE_LIMITED_API
  inline DEE_A_RET_WUNUSED ::Dee_hash_t __hash__() const { ::Dee_hash_t result; if (::DeeTuple_HashEx(this->ob_ptr,DEE_HASH_START,&result) != 0) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED ::Dee_hash_t __hash__(DEE_A_IN ::Dee_hash_t start) const { ::Dee_hash_t result; if (::DeeTuple_HashEx(this->ob_ptr,start,&result) != 0) detail::throw_last_error(); return result; }
#else
  using object::__hash__;
#endif
 public: /* public methods */
  inline void _append(DEE_A_INOUT ::DeeObject *ob) { if ((::DeeTuple_Append(&this->ob_ptr,ob)) == -1) detail::throw_last_error(); }
  inline DEE_A_RET_WUNUSED bool contains(DEE_A_INOUT ::DeeObject *ob) const { int result; if ((result = ::DeeTuple_Contains(this->ob_ptr,ob)) == -1) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED bool contains(DEE_A_INOUT ::DeeObject *ob, DEE_A_INOUT ::DeeObject *pred) const { int result; if ((result = ::DeeTuple_ContainsPred(this->ob_ptr,ob,pred)) == -1) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED ::Dee_ssize_t find(DEE_A_INOUT ::DeeObject *ob) const { ::Dee_ssize_t result; if ((result = ::DeeTuple_Find(this->ob_ptr,ob)) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_INOUT ::DeeObject *ob) const { Dee_size_t result; if ((result = ::DeeTuple_Index(this->ob_ptr,ob)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_INOUT ::DeeObject *ob) const { Dee_size_t result; if ((result = ::DeeTuple_Count(this->ob_ptr,ob)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED tuple concat(DEE_A_INOUT ::DeeObject *right) const { return tuple(::DeeTuple_Concat(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object locate(DEE_A_INOUT ::DeeObject *pred) const { return object(::DeeTuple_Locate(this->ob_ptr,pred),detail::tag_check_error()); }
  inline DEE_A_RET_WUNUSED object rlocate(DEE_A_INOUT ::DeeObject *pred) const { return object(::DeeTuple_RLocate(this->ob_ptr,pred),detail::tag_check_error()); }
  inline DEE_A_RET_WUNUSED tuple locate_all(DEE_A_INOUT ::DeeObject *pred) const { return tuple(::DeeTuple_LocateAll(this->ob_ptr,pred),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED list as_list() const;
  inline DEE_A_RET_WUNUSED tuple reversed() const { return tuple(::DeeTuple_Reversed(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED tuple sorted() const { return tuple(::DeeTuple_Sorted(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED tuple sorted(DEE_A_INOUT ::DeeObject *pred) const { return tuple(::DeeTuple_SortedPred(this->ob_ptr,pred),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED tuple deep_copy() const { return tuple(::DeeTuple_DeepCopy(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED list segments(DEE_A_IN Dee_size_t n) const;
 public: /* public constructor / destructor */
  inline tuple() DEE_CXX11_NOEXCEPT: object(Dee_EmptyTuple) {}
  inline tuple(DEE_A_IN object const &ob): object(::DeeTuple_Cast(ob),detail::tag_ref_or_err()) {}
  inline tuple(DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) object const *elem): object(::DeeTuple_NewFromVector(n,reinterpret_cast< ::DeeObject *const*>(elem)),detail::tag_ref_or_err()) {}
  inline tuple(DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *elem): object(::DeeTuple_NewFromVector(n,elem),detail::tag_ref_or_err()) {}
  DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(tuple,object,DeeTuple_Check)
};

inline DEE_A_RET_WUNUSED object object::callattr(DEE_A_IN_Z char const *attr, DEE_A_IN tuple const &args) const { return object(::DeeObject_CallAttrString(this->ob_ptr,attr,args),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED object object::__call__(DEE_A_IN tuple const &args) const { return object(::DeeObject_Call(this->ob_ptr,args),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED object object::__thiscall__(DEE_A_INOUT ::DeeObject *this_arg, DEE_A_IN tuple const &args) const { return object(::DeeObject_ThisCall(this->ob_ptr,this_arg,args),detail::tag_ref_or_err()); }
//inline DEE_A_RET_WUNUSED object object::operator () (DEE_A_IN tuple const &args) const { return object(::DeeObject_Call(this->ob_ptr,args),detail::tag_ref_or_err()); }
#ifdef GUARD_DEEMON_CXX_TYPE_H
inline DEE_A_RET_WUNUSED object type::new_(DEE_A_IN tuple const &args) const { return object(::DeeType_NewInstance((::DeeTypeObject *)this->ob_ptr,args),detail::tag_ref_or_err()); }
#endif
#ifdef GUARD_DEEMON_CXX_STRING_H
inline DEE_A_RET_WUNUSED object object::callattr(DEE_A_IN string const &attr, DEE_A_IN tuple const &args) const { return object(DeeObject_CallAttr(this->ob_ptr,attr,args),detail::tag_ref_or_err()); }
#if DEE_CONFIG_HAVE_ENCODING_UTF8
inline DEE_A_RET_WUNUSED string::utf8 string::utf8::format(DEE_A_IN tuple const &args) const { return string::utf8(DeeUtf8String_Format(this->ob_ptr,args),detail::tag_ref_or_err()); }
#endif
#if DEE_CONFIG_HAVE_ENCODING_WIDE
inline DEE_A_RET_WUNUSED string::wide string::wide::format(DEE_A_IN tuple const &args) const { return string::wide(DeeWideString_Format(this->ob_ptr,args),detail::tag_ref_or_err()); }
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
inline DEE_A_RET_WUNUSED string::utf16 string::utf16::format(DEE_A_IN tuple const &args) const { return string::utf16(DeeUtf16String_Format(this->ob_ptr,args),detail::tag_ref_or_err()); }
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
inline DEE_A_RET_WUNUSED string::utf32 string::utf32::format(DEE_A_IN tuple const &args) const { return string::utf32(DeeUtf32String_Format(this->ob_ptr,args),detail::tag_ref_or_err()); }
#endif
#endif
#ifdef GUARD_DEEMON_CXX_STRUCTURED_H
inline DEE_A_RET_WUNUSED type type::foreign_function(DEE_A_IN tuple const &arg_types) const { return type(::DeeType_ForeignFunction((::DeeTypeObject *)this->ob_ptr,DeeFunctionFlags_DEFAULT,DeeTuple_SIZE(arg_types.priv_get()),(::DeeTypeObject *const *)DeeTuple_ELEM(arg_types.priv_get())),detail::tag_check_error()); }
inline DEE_A_RET_WUNUSED type type::foreign_function(DEE_A_IN ::DeeFunctionFlags flags, DEE_A_IN tuple const &arg_types) const { return type(::DeeType_ForeignFunction((::DeeTypeObject *)this->ob_ptr,flags,DeeTuple_SIZE(arg_types.priv_get()),(::DeeTypeObject *const *)DeeTuple_ELEM(arg_types.priv_get())),detail::tag_check_error()); }
#endif
#ifdef GUARD_DEEMON_CXX_LIST_H
inline DEE_A_RET_WUNUSED tuple list::as_tuple() const { return tuple(::DeeList_AsTuple(this->ob_ptr),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list tuple::as_list() const { return list(::DeeTuple_AsList(this->ob_ptr),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list tuple::segments(DEE_A_IN Dee_size_t n) const { return list(::DeeTuple_Segments(this->ob_ptr,n),detail::tag_ref_or_err()); }
#endif

}
DEE_DECL_CXX_END
#endif

#endif /* !GUARD_DEEMON_CXX_TUPLE_H */
