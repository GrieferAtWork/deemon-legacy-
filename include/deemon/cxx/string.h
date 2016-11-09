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
#ifndef GUARD_DEEMON_CXX_STRING_H
#define GUARD_DEEMON_CXX_STRING_H 1

#include <deemon/__conf.inl>
#include <deemon/cxx/__features.inl>
#include <deemon/string.h>
#include <deemon/optional/object_doc.h>
#include <deemon/cxx/object.h>
#include <deemon/__string_encoding.inl>

#if DEE_HAVE_CXX_API

DEE_DECL_CXX_BEGIN
namespace deemon {

struct tuple;
struct list;

struct string: ::deemon::object {
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 public: /* public types */
#if DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING) == DEE_STRING_ENCODING(Utf8)
 typedef ::deemon::string utf8;
#else
 struct utf8: ::deemon::object {
#define DEE_STRING_TYPE_NAME utf8
#define DEE_STRING_ENC       Utf8
#include "__string_impl.inl"
 };
#endif
#endif

#if DEE_CONFIG_HAVE_ENCODING_WIDE
 public: /* public types */
#if DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING) == DEE_STRING_ENCODING(Wide)
 typedef ::deemon::string wide;
#else
 struct wide: ::deemon::object {
#define DEE_STRING_TYPE_NAME wide
#define DEE_STRING_ENC       Wide
#include "__string_impl.inl"
 };
#endif
#endif

#if DEE_CONFIG_HAVE_ENCODING_UTF16
 public: /* public types */
#if DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING) == DEE_STRING_ENCODING(Utf16)
 typedef ::deemon::string utf16;
#else
 struct utf16: ::deemon::object {
#define DEE_STRING_TYPE_NAME utf16
#define DEE_STRING_ENC       Utf16
#include "__string_impl.inl"
 };
#endif
#endif

#if DEE_CONFIG_HAVE_ENCODING_UTF32
 public: /* public types */
#if DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING) == DEE_STRING_ENCODING(Utf32)
 typedef ::deemon::string utf32;
#else
 struct utf32: ::deemon::object {
#define DEE_STRING_TYPE_NAME utf32
#define DEE_STRING_ENC       Utf32
#include "__string_impl.inl"
 };
#endif
#endif

#define DEE_STRING_TYPE_NAME string
#define DEE_STRING_ENC       DEE_CONFIG_DEFAULT_ENCODING
#include "__string_impl.inl"
};


inline DEE_A_RET_WUNUSED string object::str() { return string(::DeeObject_Str(this->ob_ptr),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED string object::repr() { return string(::DeeObject_Repr(this->ob_ptr),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED string object::doc() const { return string(::DeeObject_Doc(this->ob_ptr),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED string object::docattr(DEE_A_IN string const &attr) const { return string(::DeeObject_DocAttr(this->ob_ptr,attr),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED string object::docattr(DEE_A_IN_Z char const *attr) const { return string(::DeeObject_DocAttrString(this->ob_ptr,attr),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED object object::getattr_const(DEE_A_IN string const &attr) const { return object(::DeeObject_GetAttrConst(this->ob_ptr,attr),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED object object::getattr(DEE_A_IN string const &attr) const { return object(::DeeObject_GetAttr(this->ob_ptr,attr),detail::tag_ref_or_err()); }
#ifdef GUARD_DEEMON_CXX_TUPLE_H
inline DEE_A_RET_WUNUSED object object::callattr(DEE_A_IN string const &attr, DEE_A_IN tuple const &args) const { return object(DeeObject_CallAttr(this->ob_ptr,attr,args),detail::tag_ref_or_err()); }
#endif
inline DEE_A_RET_WUNUSED bool object::hasattr(DEE_A_IN string const &attr) const { int result; if ((result = !!DeeObject_HasAttr(this->ob_ptr,attr)) == -1) detail::throw_last_error(); return !!result; }
inline void object::delattr(DEE_A_IN string const &attr) const { if (DeeObject_DelAttr(this->ob_ptr,attr) == -1) detail::throw_last_error(); }
inline void object::setattr(DEE_A_IN string const &attr, DEE_A_IN object const &v) const { if (DeeObject_SetAttr(this->ob_ptr,attr,v) == -1) detail::throw_last_error(); }

#ifdef GUARD_DEEMON_CXX_TUPLE_H
inline DEE_A_RET_WUNUSED string string::format(DEE_A_IN tuple const &args) const { return string(DeeString_Format(this->ob_ptr,args),detail::tag_ref_or_err()); }
#endif
#ifdef GUARD_DEEMON_CXX_LIST_H
inline DEE_A_RET_WUNUSED list string::rsplit(DEE_A_IN string const &sep) const { return list(::DeeString_RSplit(this->ob_ptr,sep),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list string::split(DEE_A_IN string const &sep) const { return list(::DeeString_Split(this->ob_ptr,sep),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list string::splitlines() const { return list(::DeeString_SplitLines(this->ob_ptr,0),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list string::splitlines(bool keepends) const { return list(::DeeString_SplitLines(this->ob_ptr,(int)keepends),detail::tag_ref_or_err()); }
#endif

namespace detail {
#if DEE_CONFIG_HAVE_ENCODING_UTF8
template<Dee_size_t N> struct _typeof_impl<Dee_Utf8Char[N]> {
 static inline DEE_A_RET_EXCEPT(NULL) DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return &DeeUtf8String_Type; }
 static inline DEE_A_RET_EXCEPT_REF DeeObject *new_(DEE_A_IN Dee_Utf8Char const (&ob)[N]) DEE_CXX11_NOEXCEPT { return DeeUtf8String_NewWithLength(N-1,ob); }
};
#endif
#if DEE_CONFIG_HAVE_ENCODING_WIDE
template<Dee_size_t N> struct _typeof_impl<Dee_WideChar[N]> {
 static inline DEE_A_RET_EXCEPT(NULL) DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return &DeeWideString_Type; }
 static inline DEE_A_RET_EXCEPT_REF DeeObject *new_(DEE_A_IN Dee_WideChar const (&ob)[N]) DEE_CXX11_NOEXCEPT { return DeeWideString_NewWithLength(N-1,ob); }
};
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
template<Dee_size_t N> struct _typeof_impl<Dee_Utf16Char[N]> {
 static inline DEE_A_RET_EXCEPT(NULL) DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return &DeeUtf16String_Type; }
 static inline DEE_A_RET_EXCEPT_REF DeeObject *new_(DEE_A_IN Dee_Utf16Char const (&ob)[N]) DEE_CXX11_NOEXCEPT { return DeeUtf16String_NewWithLength(N-1,ob); }
};
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
template<Dee_size_t N> struct _typeof_impl<Dee_Utf32Char[N]> {
 static inline DEE_A_RET_EXCEPT(NULL) DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return &DeeUtf32String_Type; }
 static inline DEE_A_RET_EXCEPT_REF DeeObject *new_(DEE_A_IN Dee_Utf32Char const (&ob)[N]) DEE_CXX11_NOEXCEPT { return DeeUtf32String_NewWithLength(N-1,ob); }
};
#endif
}

}
DEE_DECL_CXX_END
#endif

#endif /* !GUARD_DEEMON_CXX_STRING_H */
