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
#ifndef GUARD_DEEMON_CXX_TYPE_H
#define GUARD_DEEMON_CXX_TYPE_H 1

#include <deemon/__conf.inl>
#include <deemon/cxx/__features.inl>
#include <deemon/cxx/object.h>
#include <deemon/optional/type_slots.h>
#include <deemon/optional/functionflags.h>

#if DEE_HAVE_CXX_API
DEE_DECL_CXX_BEGIN
namespace deemon {

struct type: object {
 public: /* public operators */
  inline DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL operator ::DeeTypeObject *() const DEE_CXX11_NOEXCEPT { return (::DeeTypeObject *)this->ob_ptr; }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED object new_() const { return object(::DeeType_NewInstanceDefault((DeeTypeObject *)this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object new_(tuple const &args) const;
  inline type &base() DEE_CXX11_NOEXCEPT { return reinterpret_cast<type &>(DeeType_GET_SLOT(this->ob_ptr,tp_base)); }
  inline type const &base() const DEE_CXX11_NOEXCEPT { return reinterpret_cast<type const &>(DeeType_GET_SLOT(this->ob_ptr,tp_base)); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED bool derived_from(DEE_A_IN ::DeeTypeObject *base) const DEE_CXX11_NOEXCEPT { return !!::DeeType_IsDerived((::DeeTypeObject *)this->ob_ptr,base); }
  inline DEE_A_RET_WUNUSED bool same_or_derived_from(DEE_A_IN ::DeeTypeObject *base) const DEE_CXX11_NOEXCEPT { return !!::DeeType_IsSameOrDerived((::DeeTypeObject *)this->ob_ptr,base); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED Dee_size_t instance_size() const DEE_CXX11_NOEXCEPT { return DeeType_GET_SLOT(this->ob_ptr,tp_instance_size); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED DEE_A_RET_Z char const *get_name() const DEE_CXX11_NOEXCEPT { char const *result = DeeType_NAME(this->ob_ptr); return result ? result : ""; }
  inline DEE_A_RET_WUNUSED DEE_A_RET_Z char const *get_doc() const DEE_CXX11_NOEXCEPT { char const *result = DeeType_GET_SLOT(this->ob_ptr,tp_doc); return result ? result : ""; }
  inline void set_name(DEE_A_IN_Z char const *name) const { if (::DeeType_SetName((::DeeTypeObject *)this->ob_ptr,name) == -1) detail::throw_last_error(); }
  inline void set_doc(DEE_A_IN_Z char const *doc) const { if (::DeeType_SetDoc((::DeeTypeObject *)this->ob_ptr,doc) == -1) detail::throw_last_error(); }
  inline void set_static_name(DEE_A_IN_Z char const *name) const DEE_CXX11_NOEXCEPT { ::DeeType_SetStaticName((::DeeTypeObject *)this->ob_ptr,name); }
  inline void set_static_doc(DEE_A_IN_Z char const *doc) const DEE_CXX11_NOEXCEPT { ::DeeType_SetStaticDoc((::DeeTypeObject *)this->ob_ptr,doc); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED type pointer() const;
  inline DEE_A_RET_WUNUSED type lvalue() const;
  inline DEE_A_RET_WUNUSED type array(DEE_A_IN Dee_size_t n_elem) const;
  inline DEE_A_RET_WUNUSED type foreign_function(DEE_A_IN tuple const &arg_types) const;
  inline DEE_A_RET_WUNUSED type foreign_function(DEE_A_IN Dee_size_t n_args, DEE_A_IN_R(n_args) type const *arg_types) const;
  inline DEE_A_RET_WUNUSED type foreign_function(DEE_A_IN Dee_size_t n_args, DEE_A_IN_R(n_args) ::DeeTypeObject *const *arg_types) const;
  inline DEE_A_RET_WUNUSED type foreign_function(DEE_A_IN ::Dee_funflags_t flags, DEE_A_IN tuple const &arg_types) const;
  inline DEE_A_RET_WUNUSED type foreign_function(DEE_A_IN ::Dee_funflags_t flags, DEE_A_IN Dee_size_t n_args, DEE_A_IN_R(n_args) type const *arg_types) const;
  inline DEE_A_RET_WUNUSED type foreign_function(DEE_A_IN ::Dee_funflags_t flags, DEE_A_IN Dee_size_t n_args, DEE_A_IN_R(n_args) ::DeeTypeObject *const *arg_types) const;
 public: /* public constructor / destructor */
  DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(type,object,DeeType_Check)
  inline type &operator = (DEE_A_IN ::DeeTypeObject *ob) DEE_CXX11_NOEXCEPT { DEE_ASSERT(DeeObject_Check(ob) && DeeType_Check(ob)); this->priv_assign_ptr((::DeeObject *)ob); return *this; }
  inline explicit type(DEE_A_IN ::DeeTypeObject *ob) DEE_CXX11_NOEXCEPT: object((::DeeObject *)ob) { DEE_ASSERT(!ob || DeeType_Check(ob)); }
  inline explicit type(DEE_A_IN_OPT ::DeeTypeObject *ob, detail::tag_check_error): object((::DeeObject *)ob,detail::tag_check_error()) { DEE_ASSERT(!ob || DeeType_Check(ob)); }
  inline explicit type(DEE_A_IN ::DeeTypeObject *ob, detail::tag_inherit_ref) DEE_CXX11_NOEXCEPT: object((::DeeObject *)ob,detail::tag_inherit_ref()) { DEE_ASSERT(!ob || DeeType_Check(ob)); }
  inline explicit type(DEE_A_IN_OPT ::DeeTypeObject *ob, detail::tag_ref_or_err): object((::DeeObject *)ob,detail::tag_ref_or_err()) { DEE_ASSERT(!ob || DeeType_Check(ob)); }
};

#ifdef GUARD_DEEMON_CXX_TUPLE_H
inline DEE_A_RET_WUNUSED object type::new_(DEE_A_IN tuple const &args) const { return object(::DeeType_NewInstance((::DeeTypeObject *)this->ob_ptr,args),detail::tag_ref_or_err()); }
#endif
inline DEE_A_RET_WUNUSED ::deemon::type object::class_() const DEE_CXX11_NOEXCEPT { return ::deemon::type(::DeeObject_ClassOf(this->ob_ptr)); }
inline DEE_A_RET_WUNUSED bool object::is(DEE_A_IN ::DeeTypeObject const *ty) const DEE_CXX11_NOEXCEPT { return !!::DeeObject_Is(this->ob_ptr,ty); }
inline DEE_A_RET_WUNUSED bool object::is(DEE_A_IN ::deemon::type const &ty) const DEE_CXX11_NOEXCEPT { return !!::DeeObject_Is(this->ob_ptr,ty); }
inline DEE_A_RET_WUNUSED bool object::instanceof(DEE_A_IN ::DeeTypeObject const *ty) const DEE_CXX11_NOEXCEPT { return !!DeeObject_InstanceOf(this->ob_ptr,ty); }
inline DEE_A_RET_WUNUSED bool object::instanceof(DEE_A_IN ::deemon::type const &ty) const DEE_CXX11_NOEXCEPT { return !!DeeObject_InstanceOf(this->ob_ptr,ty); }
inline DEE_A_RET_WUNUSED bool object::instanceof_exact(DEE_A_IN ::DeeTypeObject const *ty) const DEE_CXX11_NOEXCEPT { return !!DeeObject_InstanceOfExact(this->ob_ptr,ty); }
inline DEE_A_RET_WUNUSED bool object::instanceof_exact(DEE_A_IN ::deemon::type const &ty) const DEE_CXX11_NOEXCEPT { return !!DeeObject_InstanceOfExact(this->ob_ptr,ty); }

}
DEE_DECL_CXX_END
#endif

#endif /* !GUARD_DEEMON_CXX_TYPE_H */
