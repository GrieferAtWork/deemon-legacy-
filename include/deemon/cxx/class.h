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
#ifndef GUARD_DEEMON_CXX_CLASS_H
#define GUARD_DEEMON_CXX_CLASS_H 1

#include <deemon/__conf.inl>
#include <deemon/cxx/object.h>
#include <deemon/cxx/object.h>
#include <deemon/cxx/type.h>
#include <deemon/class.h>

#if DEE_HAVE_CXX_API
DEE_DECL_CXX_BEGIN
namespace deemon {

struct class_: type {
 public: /* public methods (operator/method/property access) */
  inline bool __has_slot(DEE_A_IN int slot) DEE_CXX11_NOEXCEPT { return DeeClass_HasSlot((DeeTypeObject *)this->ob_ptr,slot); }
  inline void __set_slot(DEE_A_IN int slot, DEE_A_INOUT DeeObject *callback) DEE_CXX11_NOEXCEPT { if (DeeClass_SetSlot((DeeTypeObject *)this->ob_ptr,slot,callback) != 0) detail::throw_last_error(); }
  inline void __del_slot(DEE_A_IN int slot) DEE_CXX11_NOEXCEPT { if (DeeClass_DelSlot((DeeTypeObject *)this->ob_ptr,slot) != 0) detail::throw_last_error(); }
  inline bool __has_method(DEE_A_IN_Z char const *name) DEE_CXX11_NOEXCEPT { return !!DeeClass_HasMethodString((DeeTypeObject *)this->ob_ptr,name); }
  inline bool __has_method(DEE_A_IN_OBJECT(DeeStringObject) const *name) DEE_CXX11_NOEXCEPT { return !!DeeClass_HasMethod((DeeTypeObject *)this->ob_ptr,name); }
  inline bool __has_class_method(DEE_A_IN_Z char const *name) DEE_CXX11_NOEXCEPT { return !!DeeClass_HasClassMethodString((DeeTypeObject *)this->ob_ptr,name); }
  inline bool __has_class_method(DEE_A_IN_OBJECT(DeeStringObject) const *name) DEE_CXX11_NOEXCEPT { return !!DeeClass_HasClassMethod((DeeTypeObject *)this->ob_ptr,name); }
  inline void __add_method(DEE_A_IN_Z char const *name, DEE_A_INOUT DeeObject *callback) DEE_CXX11_NOEXCEPT { if (DeeClass_AddMethodString((DeeTypeObject *)this->ob_ptr,name,callback) != 0) detail::throw_last_error(); }
  inline void __add_method(DEE_A_IN_OBJECT(DeeStringObject) const *name, DEE_A_INOUT DeeObject *callback) DEE_CXX11_NOEXCEPT { if (DeeClass_AddMethod((DeeTypeObject *)this->ob_ptr,name,callback) != 0) detail::throw_last_error(); }
  inline void __add_class_method(DEE_A_IN_Z char const *name, DEE_A_INOUT DeeObject *callback) DEE_CXX11_NOEXCEPT { if (DeeClass_AddClassMethodString((DeeTypeObject *)this->ob_ptr,name,callback) != 0) detail::throw_last_error(); }
  inline void __add_class_method(DEE_A_IN_OBJECT(DeeStringObject) const *name, DEE_A_INOUT DeeObject *callback) DEE_CXX11_NOEXCEPT { if (DeeClass_AddClassMethod((DeeTypeObject *)this->ob_ptr,name,callback) != 0) detail::throw_last_error(); }
  inline bool __has_property(DEE_A_IN_Z char const *name) DEE_CXX11_NOEXCEPT { return !!DeeClass_HasPropertyString((DeeTypeObject *)this->ob_ptr,name); }
  inline bool __has_property(DEE_A_IN_OBJECT(DeeStringObject) const *name) DEE_CXX11_NOEXCEPT { return !!DeeClass_HasProperty((DeeTypeObject *)this->ob_ptr,name); }
  inline bool __has_class_property(DEE_A_IN_Z char const *name) DEE_CXX11_NOEXCEPT { return !!DeeClass_HasClassPropertyString((DeeTypeObject *)this->ob_ptr,name); }
  inline bool __has_class_property(DEE_A_IN_OBJECT(DeeStringObject) const *name) DEE_CXX11_NOEXCEPT { return !!DeeClass_HasClassProperty((DeeTypeObject *)this->ob_ptr,name); }
  inline void __add_property(DEE_A_IN_Z char const *name, DEE_A_INOUT_OPT DeeObject *getter_ = NULL, DEE_A_INOUT_OPT DeeObject *delete_ = NULL, DEE_A_INOUT_OPT DeeObject *setter_ = NULL) DEE_CXX11_NOEXCEPT { if (DeeClass_AddPropertyString((DeeTypeObject *)this->ob_ptr,name,getter_,delete_,setter_) != 0) detail::throw_last_error(); }
  inline void __add_property(DEE_A_IN_OBJECT(DeeStringObject) const *name, DEE_A_INOUT_OPT DeeObject *getter_ = NULL, DEE_A_INOUT_OPT DeeObject *delete_ = NULL, DEE_A_INOUT_OPT DeeObject *setter_ = NULL) DEE_CXX11_NOEXCEPT { if (DeeClass_AddProperty((DeeTypeObject *)this->ob_ptr,name,getter_,delete_,setter_) != 0) detail::throw_last_error(); }
  inline void __add_class_property(DEE_A_IN_Z char const *name, DEE_A_INOUT_OPT DeeObject *getter_ = NULL, DEE_A_INOUT_OPT DeeObject *delete_ = NULL, DEE_A_INOUT_OPT DeeObject *setter_ = NULL) DEE_CXX11_NOEXCEPT { if (DeeClass_AddClassPropertyString((DeeTypeObject *)this->ob_ptr,name,getter_,delete_,setter_) != 0) detail::throw_last_error(); }
  inline void __add_class_property(DEE_A_IN_OBJECT(DeeStringObject) const *name, DEE_A_INOUT_OPT DeeObject *getter_ = NULL, DEE_A_INOUT_OPT DeeObject *delete_ = NULL, DEE_A_INOUT_OPT DeeObject *setter_ = NULL) DEE_CXX11_NOEXCEPT { if (DeeClass_AddClassProperty((DeeTypeObject *)this->ob_ptr,name,getter_,delete_,setter_) != 0) detail::throw_last_error(); }
 public: /* public constructor / destructor */
  inline explicit class_(): type((DeeObject *)::DeeClass_New(&::DeeObject_Type,NULL),detail::tag_ref_or_err()) {}
  inline explicit class_(DEE_A_IN DeeTypeObject const *base): type((DeeObject *)::DeeClass_New(base,NULL),detail::tag_ref_or_err()) {}
  inline explicit class_(DEE_A_IN DeeTypeObject const *base, DEE_A_IN_Z_OPT char const *name): type((DeeObject *)::DeeClass_New(base,name),detail::tag_ref_or_err()) {}
  inline explicit class_(DEE_A_IN DeeTypeObject const *base, DEE_A_IN_OBJECT_OPT(DeeAnyStringObject) const *name): type((DeeObject *)::DeeClass_NewObject(base,name),detail::tag_ref_or_err()) {}
  inline explicit class_(DEE_A_IN DeeTypeObject const *base, DEE_A_IN_Z_OPT char const *name, DEE_A_IN_OPT struct DeeUUID const *uuid): type((DeeObject *)::DeeClass_NewEx(base,name,uuid),detail::tag_ref_or_err()) {}
  inline explicit class_(DEE_A_IN DeeTypeObject const *base, DEE_A_IN_OBJECT_OPT(DeeAnyStringObject) const *name, DEE_A_IN_OPT struct DeeUUID const *uuid): type((DeeObject *)::DeeClass_NewObjectEx(base,name,uuid),detail::tag_ref_or_err()) {}
  inline explicit class_(DEE_A_IN_Z_OPT char const *name): type((DeeObject *)::DeeClass_New(&::DeeObject_Type,name),detail::tag_ref_or_err()) {}
  inline explicit class_(DEE_A_IN_OBJECT_OPT(DeeAnyStringObject) const *name): type((DeeObject *)::DeeClass_NewObject(&::DeeObject_Type,name),detail::tag_ref_or_err()) {}
  inline explicit class_(DEE_A_IN_Z_OPT char const *name, DEE_A_IN_OPT struct DeeUUID const *uuid): type((DeeObject *)::DeeClass_NewEx(&::DeeObject_Type,name,uuid),detail::tag_ref_or_err()) {}
  inline explicit class_(DEE_A_IN_OBJECT_OPT(DeeAnyStringObject) const *name, DEE_A_IN_OPT struct DeeUUID const *uuid): type((DeeObject *)::DeeClass_NewObjectEx(&::DeeObject_Type,name,uuid),detail::tag_ref_or_err()) {}
  DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(class_,type,DeeClass_Check)
};

}
DEE_DECL_CXX_END
#endif

#endif /* !GUARD_DEEMON_CXX_CLASS_H */
