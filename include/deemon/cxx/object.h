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
#ifndef GUARD_DEEMON_CXX_OBJECT_H
#define GUARD_DEEMON_CXX_OBJECT_H 1

#include <deemon/__conf.inl>
#include <deemon/object.h>
#include <deemon/sequence.h>
#include <deemon/none.h>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/traceback.h>

#if DEE_HAVE_CXX_API
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#include <exception>
#include <type_traits>
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#ifdef __INTELLISENSE__
;
#endif

DEE_DECL_CXX_BEGIN
namespace deemon {
struct object;

namespace detail {
 
template<class T, bool baseof_object> struct _typeof_baseimpl { typedef int missing; };
template<class T> struct _typeof_baseimpl<T,true> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(object); }
 static inline DeeObject *new_(T const &v) DEE_CXX11_NOEXCEPT { DeeReturn_NEWREF(v.priv_get()); }
};
template<class T> struct _typeof_impl: _typeof_baseimpl<T,std::is_base_of<object,T>::value> {};
template<class T> struct _typeof_impl<T const>: _typeof_impl<T> {};
template<class T> struct _typeof_impl<T volatile>: _typeof_impl<T> {};
template<class T> struct _typeof_impl<T const volatile>: _typeof_impl<T> {};
template<class T, ::Dee_size_t N> struct _typeof_impl<T const[N]>: _typeof_impl<T[N]> {};
template<class T, ::Dee_size_t N> struct _typeof_impl<T volatile[N]>: _typeof_impl<T[N]> {};
template<class T, ::Dee_size_t N> struct _typeof_impl<T const volatile[N]>: _typeof_impl<T[N]> {};
typedef int (&_supported_yes)[1];
typedef int (&_supported_no)[2];
template<class T> _supported_no _supported_impl(typename _typeof_impl<T>::missing*);
template<class T> _supported_yes _supported_impl(...);
}
template<class T> struct supported {
 enum{value = sizeof(::deemon::detail::_supported_impl<T>(0))==sizeof(int)};
};

#ifdef __INTELLISENSE__
// Just so use of 'typeof' and 'new_' gets marked red for unsupported types.
namespace detail {
template<class T, class R, bool = ::deemon::supported<T>::value>
struct _supported_enableif{typedef R type;};
template<class T, class R> struct _supported_enableif<T,R,false>{};
}
#define DEE_PRIVATE_CXX_ENABLEIF_SUPPORTED(T,R) \
 typename ::deemon::detail::_supported_enableif< T,R >::type
#else
#define DEE_PRIVATE_CXX_ENABLEIF_SUPPORTED(T,R) R
#endif

namespace detail {
struct tag_ref_or_err{};
struct tag_check_error{};
struct tag_inherit_ref{};
struct tag_init_later{};

class deemon_exception {
 private:
  ::DeeObject *ob_object;
  ::DeeObject *ob_traceback;
 public:
  inline operator ::DeeObject *() const DEE_CXX11_NOEXCEPT { return this->ob_object; }
  virtual inline const char *what() const DEE_CXX11_NOEXCEPT {
   if (DeeObject_InstanceOf(this->ob_object,&DeeErrorType_Error))
    return DeeString_STR(DeeError_Message(this->ob_object));
   if (DeeString_Check(this->ob_object)) return DeeString_STR(this->ob_object);
   return DeeType_NAME(Dee_TYPE(this->ob_object));
  }
  inline deemon_exception(DEE_A_IN ::DeeObject *err, DEE_A_IN ::DeeObject *traceback) DEE_CXX11_NOEXCEPT:
   ob_object(err),ob_traceback(traceback) { DEE_ASSERT(DeeTraceback_Check(traceback)); }
  inline deemon_exception(deemon_exception const &right) DEE_CXX11_NOEXCEPT:
   ob_object(right.ob_object),ob_traceback(right.ob_traceback) {
   Dee_INCREF(this->ob_object);
   Dee_INCREF(this->ob_traceback);
  }
  virtual inline ~deemon_exception() DEE_CXX11_NOEXCEPT {
   Dee_DECREF(this->ob_object);
   Dee_DECREF(this->ob_traceback);
  }
};
}

#define DEE_ERROR_CLASS_IMPL(name,base) public:\
 inline name(DEE_A_IN DeeObject *err, DEE_A_IN ::DeeObject *traceback) \
  DEE_CXX11_NOEXCEPT: base(err,traceback) {}\
 inline name(name const &right) DEE_CXX11_NOEXCEPT: base(right) {}

class Error: public detail::deemon_exception {
 DEE_ERROR_CLASS_IMPL(Error,deemon_exception)
 public: // Error sub-classes
  class AttributeError;
  class CompilerError;
  class ThreadCrash;
  class NoMemory;
  class RuntimeError;
  class TypeError;
  class ValueError;
  class SystemError;
  class NetError;
  class IOError;
};
class Error::RuntimeError: public Error {
 DEE_ERROR_CLASS_IMPL(RuntimeError,Error)
 class NotImplemented;
 class AssertionError;
 class UnboundLocal;
 class StackOverflow;
 class SegFault;
 class IllegalInstruction;
};
class Error::ValueError: public Error {
 DEE_ERROR_CLASS_IMPL(ValueError,Error)
 class DivideByZero;
 class KeyError;
 class IndexError;
 class UnicodeError;
 class ReferenceError;
};
class Error::IOError: public Error {
 DEE_ERROR_CLASS_IMPL(IOError,Error)
 class FileNotFound;
};
class Error::AttributeError: public Error {DEE_ERROR_CLASS_IMPL(AttributeError,Error)};
class Error::CompilerError: public Error {DEE_ERROR_CLASS_IMPL(CompilerError,Error)};
class Error::ThreadCrash: public Error {DEE_ERROR_CLASS_IMPL(ThreadCrash,Error)};
class Error::NoMemory: public Error {DEE_ERROR_CLASS_IMPL(NoMemory,Error)};
class Error::TypeError: public Error {DEE_ERROR_CLASS_IMPL(TypeError,Error)};
class Error::SystemError: public Error {DEE_ERROR_CLASS_IMPL(SystemError,Error)};
class Error::NetError: public Error {DEE_ERROR_CLASS_IMPL(NetError,Error)};
class Error::RuntimeError::NotImplemented: public Error::RuntimeError {DEE_ERROR_CLASS_IMPL(NotImplemented,Error::RuntimeError)};
class Error::RuntimeError::AssertionError: public Error::RuntimeError {DEE_ERROR_CLASS_IMPL(AssertionError,Error::RuntimeError)};
class Error::RuntimeError::UnboundLocal: public Error::RuntimeError {DEE_ERROR_CLASS_IMPL(UnboundLocal,Error::RuntimeError)};
class Error::RuntimeError::StackOverflow: public Error::RuntimeError {DEE_ERROR_CLASS_IMPL(StackOverflow,Error::RuntimeError)};
class Error::RuntimeError::SegFault: public Error::RuntimeError {DEE_ERROR_CLASS_IMPL(SegFault,Error::RuntimeError)};
class Error::RuntimeError::IllegalInstruction: public Error::RuntimeError {DEE_ERROR_CLASS_IMPL(IllegalInstruction,Error::RuntimeError)};
class Error::ValueError::DivideByZero: public Error::ValueError {DEE_ERROR_CLASS_IMPL(DivideByZero,Error::ValueError)};
class Error::ValueError::KeyError: public Error::ValueError {DEE_ERROR_CLASS_IMPL(KeyError,Error::ValueError)};
class Error::ValueError::IndexError: public Error::ValueError {DEE_ERROR_CLASS_IMPL(IndexError,Error::ValueError)};
class Error::ValueError::UnicodeError: public Error::ValueError {DEE_ERROR_CLASS_IMPL(UnicodeError,Error::ValueError)};
class Error::ValueError::ReferenceError: public Error::ValueError {DEE_ERROR_CLASS_IMPL(ReferenceError,Error::ValueError)};
class Error::IOError::FileNotFound: public Error::IOError {DEE_ERROR_CLASS_IMPL(FileNotFound,Error::IOError)};


class Signal: public detail::deemon_exception {
 public: // Public constructor
  DEE_ERROR_CLASS_IMPL(Signal,deemon_exception)
 public:
  class Interrupt;
  class StopIteration;
};
class Signal::Interrupt: public Signal {DEE_ERROR_CLASS_IMPL(Interrupt,Signal)};
class Signal::StopIteration: public Signal {DEE_ERROR_CLASS_IMPL(StopIteration,Signal)};
#undef DEE_ERROR_CLASS_IMPL

namespace detail {
static DEE_ATTRIBUTE_NORETURN void throw_last_error() {
 DeeObject *exc,*tb;
 // Pop one occurred exception
 if (!DeeError_PopOccurred(&exc,&tb)) {
  // No exception set (silently ignore...)
  Dee_INCREF(exc = Dee_None);
  Dee_INCREF(tb = Dee_EmptyTraceback);
 }
#define DEE_CHECK_THROW(ty,cls)\
 if (DeeObject_InstanceOf(exc,ty)) throw cls(exc,tb)
 if (DeeObject_InstanceOf(exc,&DeeErrorType_Error)) {
  if (DeeObject_InstanceOf(exc,&DeeErrorType_RuntimeError)) {
   DEE_CHECK_THROW(&DeeErrorType_NotImplemented,Error::RuntimeError::NotImplemented);
   DEE_CHECK_THROW(&DeeErrorType_AssertionError,Error::RuntimeError::AssertionError);
   DEE_CHECK_THROW(&DeeErrorType_UnboundLocal,Error::RuntimeError::UnboundLocal);
   DEE_CHECK_THROW(&DeeErrorType_StackOverflow,Error::RuntimeError::StackOverflow);
   DEE_CHECK_THROW(&DeeErrorType_SegFault,Error::RuntimeError::SegFault);
   DEE_CHECK_THROW(&DeeErrorType_IllegalInstruction,Error::RuntimeError::IllegalInstruction);
   throw Error::RuntimeError(exc,tb);
  }
  if (DeeObject_InstanceOf(exc,&DeeErrorType_ValueError)) {
   DEE_CHECK_THROW(&DeeErrorType_DivideByZero,Error::ValueError::DivideByZero);
   DEE_CHECK_THROW(&DeeErrorType_KeyError,Error::ValueError::KeyError);
   DEE_CHECK_THROW(&DeeErrorType_IndexError,Error::ValueError::IndexError);
   DEE_CHECK_THROW(&DeeErrorType_UnicodeError,Error::ValueError::UnicodeError);
   DEE_CHECK_THROW(&DeeErrorType_ReferenceError,Error::ValueError::ReferenceError);
   throw Error::ValueError(exc,tb);
  }
  if (DeeObject_InstanceOf(exc,&DeeErrorType_IOError)) {
   DEE_CHECK_THROW(&DeeErrorType_FileNotFound,Error::IOError::FileNotFound);
   throw Error::IOError(exc,tb);
  }
  if (DeeObject_InstanceOf(exc,&DeeErrorType_NetError)) {
   // TODO: Various sub-classes (e.g.: DeeErrorType_NetConnectRefused)
   throw Error::NetError(exc,tb);
  }
  DEE_CHECK_THROW(&DeeErrorType_AttributeError,Error::AttributeError);
  DEE_CHECK_THROW(&DeeErrorType_CompilerError,Error::CompilerError);
  DEE_CHECK_THROW(&DeeErrorType_ThreadCrash,Error::ThreadCrash);
  DEE_CHECK_THROW(&DeeErrorType_NoMemory,Error::NoMemory);
  DEE_CHECK_THROW(&DeeErrorType_TypeError,Error::TypeError);
  DEE_CHECK_THROW(&DeeErrorType_SystemError,Error::SystemError);
  throw Error(exc,tb);
 }
 if (DeeObject_InstanceOf(exc,&DeeSignalType_Signal)) {
  DEE_CHECK_THROW(&DeeSignalType_Interrupt,Signal::Interrupt);
  DEE_CHECK_THROW(&DeeSignalType_StopIteration,Signal::StopIteration);
  throw Signal(exc,tb);
 }
 // Fallback: throw a managed deemon exception
 throw detail::deemon_exception(exc,tb);
#undef DEE_CHECK_THROW
}
static DEE_ATTRIBUTE_NORETURN void throw_iterdone() {
 Dee_INCREF(DeeErrorInstance_IterDone);
 Dee_INCREF(Dee_EmptyTraceback);
 throw Signal::StopIteration(DeeErrorInstance_IterDone,Dee_EmptyTraceback);
}
}



struct string;
struct object;
struct type;
struct tuple;


struct object {
 protected: /* protected types */
  struct _proxy_seq_item_object;
  struct _proxy_seq_item_string;
  struct _proxy_seq_item_index;
  friend inline void del(_proxy_seq_item_object const &ob);
  friend inline void del(_proxy_seq_item_string const &ob);
  friend inline void del(_proxy_seq_item_index const &ob);
 public: /* public types */
 struct iterator {
  friend struct ::deemon::object;
  protected: /* protected vars */
   DEE_A_REF ::DeeObject *ob_elem;     /*< [0..1] Iterator element. */
   DEE_A_REF ::DeeObject *ob_iterator; /*< [0..1] Iterator object. */
  protected: /* protected methods */
  public: /* public operators */
   inline DEE_A_RET_WUNUSED object operator *() const {
    if (!this->ob_elem) detail::throw_iterdone();
    return object(this->ob_elem);
   }
   inline iterator &operator ++() {
    int error;
    Dee_XDECREF(this->ob_elem);
    if ((error = ::DeeObject_IterNextEx(this->ob_iterator,&this->ob_elem)) != 0) {
     if (error < 0) detail::throw_last_error();
     this->ob_elem = NULL;
    }
    return *this;
   }
   inline DEE_A_RET_WUNUSED bool operator == (iterator const &right) const DEE_CXX11_NOEXCEPT { return this->ob_elem == right.ob_elem; }
   inline DEE_A_RET_WUNUSED bool operator != (iterator const &right) const DEE_CXX11_NOEXCEPT { return this->ob_elem != right.ob_elem; }
  public: /* public operators */
   inline iterator &operator = (iterator &&right) DEE_CXX11_NOEXCEPT {
    DEE_ASSERT(this != &right);
    Dee_XDECREF(this->ob_iterator);
    this->ob_iterator = right.ob_iterator;
    right.ob_iterator = NULL;
    return *this;
   }
   inline iterator &operator = (iterator const &right) {
    DeeObject *old_iter,*old_elem;
    old_iter = this->ob_iterator;
    old_elem = this->ob_elem;
    Dee_XINCREF(this->ob_iterator = right.ob_iterator);
    Dee_XINCREF(this->ob_elem = right.ob_elem);
    Dee_XDECREF(old_iter);
    Dee_XDECREF(ob_elem);
    return *this;
   }
  public: /* public constructor / destructor */
   inline iterator() DEE_CXX11_NOEXCEPT: ob_iterator(NULL),ob_elem(NULL) {}
   inline iterator(iterator &&right) DEE_CXX11_NOEXCEPT:
    ob_iterator(right.ob_iterator),ob_elem(right.ob_elem) { right.ob_iterator = NULL; right.ob_elem = NULL; }
   inline iterator(iterator const &right) DEE_CXX11_NOEXCEPT:
    ob_iterator(right.ob_iterator),ob_elem(right.ob_elem) { Dee_XINCREF(this->ob_iterator); Dee_XINCREF(this->ob_elem); }
   inline ~iterator() DEE_CXX11_NOEXCEPT { Dee_XDECREF(this->ob_iterator); Dee_XDECREF(this->ob_elem); }
  private:
   inline explicit iterator(DeeObject *iter): ob_iterator(iter) {
    int error;
    if ((error = ::DeeObject_IterNextEx(iter,&this->ob_elem)) != 0) {
     if (error < 0) detail::throw_last_error();
     this->ob_elem = NULL;
    }
   }
 };

 private: object(); // deleted
 public: /* public methods (iterator support) */
DEE_COMPILER_MSVC_WARNING_PUSH(4571)
  inline DEE_A_RET_WUNUSED iterator begin() const {
   DeeObject *iter_self;
   if ((iter_self = ::DeeObject_IterSelf(this->ob_ptr)) == NULL)
    detail::throw_last_error();
   try {
    return iterator(iter_self);
   } catch (...) {
    Dee_DECREF(iter_self);
    throw;
   }
  }
  inline DEE_A_RET_WUNUSED iterator end() const DEE_CXX11_NOEXCEPT { return iterator(); }
DEE_COMPILER_MSVC_WARNING_POP
 protected: /* protected vars */
  ::DeeObject *ob_ptr; /*< [1..1] Object pointer. */
 public: /* public methods (internal) */
  inline DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL ::DeeObject *priv_get() const DEE_CXX11_NOEXCEPT { return this->ob_ptr; }
  inline void priv_assign_ptr(DEE_A_IN object const &ob) DEE_CXX11_NOEXCEPT { Dee_DECREF(this->ob_ptr); Dee_INCREF(this->ob_ptr = ob.ob_ptr); }
  inline void priv_assign_ptr(DEE_A_INOUT ::DeeObject *ob) DEE_CXX11_NOEXCEPT { Dee_DECREF(this->ob_ptr); Dee_INCREF(this->ob_ptr = ob); }
  inline void priv_assign_ptr(DEE_A_IN_OPT ::DeeObject *ob, detail::tag_check_error) { if (!ob) detail::throw_last_error(); Dee_DECREF(this->ob_ptr); Dee_INCREF(this->ob_ptr = ob); }
  inline void priv_assign_ptr(DEE_A_INOUT ::DeeObject *ob, detail::tag_inherit_ref) DEE_CXX11_NOEXCEPT { DEE_ASSERT(ob); Dee_DECREF(this->ob_ptr); this->ob_ptr = ob; }
  inline void priv_assign_ptr(DEE_A_IN_OPT ::DeeObject *ob, detail::tag_ref_or_err) { if (!ob) detail::throw_last_error(); Dee_DECREF(this->ob_ptr); this->ob_ptr = ob; }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED ::Dee_hash_t __hash__() const { ::Dee_hash_t result; if (::DeeObject_HashEx(this->ob_ptr,DEE_HASH_START,&result) != 0) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED ::Dee_hash_t __hash__(DEE_A_IN ::Dee_hash_t start) const { ::Dee_hash_t result; if (::DeeObject_HashEx(this->ob_ptr,start,&result) != 0) detail::throw_last_error(); return result; }
 public: /* public methods */
  inline void make_unique() { if (::DeeObject_ForceUnique(&this->ob_ptr) != 0) detail::throw_last_error(); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED object copy() const { return object(::DeeObject_Copy(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object move() const { return object(::DeeObject_Move(this->ob_ptr),detail::tag_ref_or_err()); }
  inline void assign(DEE_A_INOUT ::DeeObject *ob) const { if (::DeeObject_Assign(this->ob_ptr,ob) != 0) detail::throw_last_error(); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED object in(::DeeObject *right) const { return object(::DeeObject_ContainsObject(right,this->ob_ptr),detail::tag_ref_or_err()); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED ::deemon::type class_() const DEE_CXX11_NOEXCEPT;
 public: /* public methods */
  inline DEE_A_RET_WUNUSED bool is(::DeeTypeObject const *ty) const DEE_CXX11_NOEXCEPT;
  inline DEE_A_RET_WUNUSED bool is(::deemon::type const &ty) const DEE_CXX11_NOEXCEPT;
  inline DEE_A_RET_WUNUSED bool instanceof(::DeeTypeObject const *ty) const DEE_CXX11_NOEXCEPT;
  inline DEE_A_RET_WUNUSED bool instanceof(::deemon::type const &ty) const DEE_CXX11_NOEXCEPT;
  inline DEE_A_RET_WUNUSED bool instanceof_exact(::DeeTypeObject const *ty) const DEE_CXX11_NOEXCEPT;
  inline DEE_A_RET_WUNUSED bool instanceof_exact(::deemon::type const &ty) const DEE_CXX11_NOEXCEPT;
 public: /* public methods */
  inline DEE_A_RET_WUNUSED object __iterself__() const { return object(::DeeObject_IterSelf(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object __iternext__() const {
   int error; ::DeeObject *result;
   if ((error = ::DeeObject_IterNextEx(this->ob_ptr,&result)) != 0) {
    if (error < 0) detail::throw_last_error();
    detail::throw_iterdone(); // End of iterator
   }
   return object(result,detail::tag_ref_or_err());
  }
  inline DEE_A_RET_WUNUSED object __size__() const { return object(::DeeObject_SizeObject(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED string str();  // implemented in <deemon/string.h>
  inline DEE_A_RET_WUNUSED string repr(); // implemented in <deemon/string.h>
  inline DEE_A_RET_WUNUSED deemon::type &type() const DEE_CXX11_NOEXCEPT { return reinterpret_cast< deemon::type &>(Dee_TYPE(this->ob_ptr)); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED object getitem(DEE_A_IN ::Dee_size_t i) const { return object(::DeeObject_GetIndex(this->ob_ptr,i),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object getitem(DEE_A_IN ::Dee_size_t lo, ::Dee_size_t hi) const { return object(::DeeObject_GetRangeIndex(this->ob_ptr,lo,hi),detail::tag_ref_or_err()); }
  inline void delitem(DEE_A_IN ::Dee_size_t i) const { if (::DeeObject_DelIndex(this->ob_ptr,i) != 0) detail::throw_last_error(); }
  inline void delitem(DEE_A_IN ::Dee_size_t lo, DEE_A_IN ::Dee_size_t hi) const { if (::DeeObject_DelRangeIndex(this->ob_ptr,lo,hi) != 0) detail::throw_last_error(); }
  inline void setitem(DEE_A_IN ::Dee_size_t i, DEE_A_IN object const &v) const { if (::DeeObject_SetIndex(this->ob_ptr,i,v) != 0) detail::throw_last_error(); }
  inline void setitem(DEE_A_IN ::Dee_size_t lo, DEE_A_IN ::Dee_size_t hi, DEE_A_IN object const &v) const { if (::DeeObject_SetRangeIndex(this->ob_ptr,lo,hi,v) != 0) detail::throw_last_error(); }
  inline DEE_A_RET_WUNUSED bool hasitem(DEE_A_IN_Z char const *key) const { int result; if ((result = ::DeeObject_HasItemString(this->ob_ptr,key)) < 0) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED bool hasitem(DEE_A_IN object const &key) const { int result; if ((result = ::DeeObject_HasItem(this->ob_ptr,key)) < 0) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED object getitem(DEE_A_IN_Z char const *key) const { return object(::DeeObject_GetItemString(this->ob_ptr,key),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object getitem(DEE_A_IN object const &index_or_key) const { return object(::DeeObject_GetItem(this->ob_ptr,index_or_key),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object getitem(DEE_A_IN object const &lo, DEE_A_IN object const &hi) const { return object(::DeeObject_GetRangeItem(this->ob_ptr,lo,hi),detail::tag_ref_or_err()); }
  inline void delitem(DEE_A_IN_Z char const *key) const { if (::DeeObject_DelItemString(this->ob_ptr,key) != 0) detail::throw_last_error(); }
  inline void delitem(DEE_A_IN object const &index_or_key) const { if (::DeeObject_DelItem(this->ob_ptr,index_or_key) != 0) detail::throw_last_error(); }
  inline void delitem(DEE_A_IN object const &lo, DEE_A_IN object const &hi) const { if (::DeeObject_DelRangeItem(this->ob_ptr,lo,hi) != 0) detail::throw_last_error(); }
  inline void setitem(DEE_A_IN_Z char const *key, DEE_A_IN object const &v) const { if (::DeeObject_SetItemString(this->ob_ptr,key,v) != 0) detail::throw_last_error(); }
  inline void setitem(DEE_A_IN object const &index_or_key, DEE_A_IN object const &v) const { if (::DeeObject_SetItem(this->ob_ptr,index_or_key,v) != 0) detail::throw_last_error(); }
  inline void setitem(DEE_A_IN object const &lo, DEE_A_IN object const &hi, DEE_A_IN object const &v) const { if (::DeeObject_SetRangeItem(this->ob_ptr,lo,hi,v) != 0) detail::throw_last_error(); }
 public: /* public methods (doc access) */
  inline DEE_A_RET_WUNUSED string doc() const;
  inline DEE_A_RET_WUNUSED string docattr(DEE_A_IN string const &attr) const;
  inline DEE_A_RET_WUNUSED string docattr(DEE_A_IN_Z char const *attr) const;
 public: /* public methods (call) */
  inline DEE_A_RET_WUNUSED object __call__(DEE_A_IN tuple const &args) const;
  inline DEE_A_RET_WUNUSED object __thiscall__(DEE_A_INOUT ::DeeObject *this_arg, DEE_A_IN tuple const &args) const;
 public: /* public methods (attribute access) */
  inline DEE_A_RET_WUNUSED object callattr(DEE_A_IN string const &attr, DEE_A_IN tuple const &args) const;
  inline DEE_A_RET_WUNUSED object getattr(DEE_A_IN string const &attr) const;
  inline DEE_A_RET_WUNUSED bool hasattr(DEE_A_IN string const &attr) const;
  inline DEE_A_RET_WUNUSED object getattr_const(DEE_A_IN string const &attr) const;
  inline void delattr(DEE_A_IN string const &attr) const;
  inline void setattr(DEE_A_IN string const &attr, DEE_A_IN object const &v) const;
  inline DEE_A_RET_WUNUSED object callattr(DEE_A_IN_Z char const *attr, DEE_A_IN tuple const &args) const;
  inline DEE_A_RET_WUNUSED object getattr(DEE_A_IN_Z char const *attr) const { return object(::DeeObject_GetAttrString(this->ob_ptr,attr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED bool hasattr(DEE_A_IN_Z char const *attr) const { int temp; if ((temp = ::DeeObject_HasAttrString(this->ob_ptr,attr)) < 0) detail::throw_last_error(); return !!temp; }
  inline DEE_A_RET_WUNUSED object getattr_const(DEE_A_IN_Z char const *attr) const { return object(::DeeObject_GetAttrConstString(this->ob_ptr,attr),detail::tag_ref_or_err()); }
  inline void delattr(DEE_A_IN_Z char const *attr) const { if (::DeeObject_DelAttrString(this->ob_ptr,attr) != 0) detail::throw_last_error(); }
  inline void setattr(DEE_A_IN_Z char const *attr, DEE_A_IN object const &v) const { if (::DeeObject_SetAttrString(this->ob_ptr,attr,v) != 0) detail::throw_last_error(); }
 public: /* public operators (deemon-style) */
#define DEE_PRIVATE_CXX_DEFINE_CAST(T,name)\
  inline DEE_A_RET_WUNUSED operator T () const {\
   T result;\
   if (DeeObject_Cast(name,this->ob_ptr,&result) != 0) detail::throw_last_error();\
   return result;\
  }
  DEE_PRIVATE_CXX_DEFINE_CAST(float,float)
  DEE_PRIVATE_CXX_DEFINE_CAST(double,double)
  DEE_PRIVATE_CXX_DEFINE_CAST(long double,ldouble)
  DEE_PRIVATE_CXX_DEFINE_CAST(char,char)
  DEE_PRIVATE_CXX_DEFINE_CAST(signed char,schar)
  DEE_PRIVATE_CXX_DEFINE_CAST(unsigned char,uchar)
  DEE_PRIVATE_CXX_DEFINE_CAST(short,short)
  DEE_PRIVATE_CXX_DEFINE_CAST(unsigned short,ushort)
  DEE_PRIVATE_CXX_DEFINE_CAST(int,int)
  DEE_PRIVATE_CXX_DEFINE_CAST(unsigned int,uint)
  DEE_PRIVATE_CXX_DEFINE_CAST(long,long)
  DEE_PRIVATE_CXX_DEFINE_CAST(unsigned long,ulong)
#if DEE_COMPILER_HAVE_LONG_LONG
  DEE_PRIVATE_CXX_DEFINE_CAST(long long,llong)
  DEE_PRIVATE_CXX_DEFINE_CAST(unsigned long long,ullong)
#endif /* DEE_COMPILER_HAVE_LONG_LONG */
#undef DEE_PRIVATE_CXX_DEFINE_CAST
  inline DEE_A_RET_WUNUSED operator bool () const { int result; if ((result = ::DeeObject_Bool(this->ob_ptr)) < 0) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED object operator ! () const { return object(::DeeObject_Not(this->ob_ptr),detail::tag_ref_or_err()); }
  //inline DEE_A_RET_WUNUSED object operator () (DEE_A_IN tuple const &args) const;
 public: /* public operators (deemon-style) */
  inline DEE_A_RET_WUNUSED _proxy_seq_item_index operator [] (DEE_A_IN ::Dee_size_t index) const DEE_CXX11_NOEXCEPT;
  inline DEE_A_RET_WUNUSED _proxy_seq_item_object operator [] (DEE_A_INOUT ::DeeObject *index_or_key) const DEE_CXX11_NOEXCEPT;
  inline DEE_A_RET_WUNUSED _proxy_seq_item_string operator [] (DEE_A_IN_Z char const *key) const DEE_CXX11_NOEXCEPT;
 public: /* public operators (deemon-style) */
  inline DEE_A_RET_WUNUSED object operator * () const; // implemented in <deemon/structured.h>
  inline DEE_A_RET_WUNUSED object operator & () const; // implemented in <deemon/structured.h>
  inline DEE_A_RET_WUNUSED object operator + () const { return object(::DeeObject_Pos(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator - () const { return object(::DeeObject_Neg(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator ~ () const { return object(::DeeObject_Inv(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator ++ () const { return object(::DeeObject_Inc(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator -- () const { return object(::DeeObject_Dec(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator ++ (int) const { return object(::DeeObject_IncPost(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator -- (int) const { return object(::DeeObject_DecPost(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator + (DEE_A_INOUT ::DeeObject *right) const { return object(::DeeObject_Add(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator - (DEE_A_INOUT ::DeeObject *right) const { return object(::DeeObject_Sub(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator * (DEE_A_INOUT ::DeeObject *right) const { return object(::DeeObject_Mul(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator / (DEE_A_INOUT ::DeeObject *right) const { return object(::DeeObject_Div(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator % (DEE_A_INOUT ::DeeObject *right) const { return object(::DeeObject_Mod(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator << (DEE_A_INOUT ::DeeObject *right) const { return object(::DeeObject_Shl(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator >> (DEE_A_INOUT ::DeeObject *right) const { return object(::DeeObject_Shr(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator & (DEE_A_INOUT ::DeeObject *right) const { return object(::DeeObject_And(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator | (DEE_A_INOUT ::DeeObject *right) const { return object(::DeeObject_Or(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator ^ (DEE_A_INOUT ::DeeObject *right) const { return object(::DeeObject_Xor(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator < (DEE_A_INOUT ::DeeObject *right) const { return object(::DeeObject_CompareLoObject(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator <= (DEE_A_INOUT ::DeeObject *right) const { return object(::DeeObject_CompareLeObject(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator == (DEE_A_INOUT ::DeeObject *right) const { return object(::DeeObject_CompareEqObject(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator != (DEE_A_INOUT ::DeeObject *right) const { return object(::DeeObject_CompareNeObject(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator > (DEE_A_INOUT ::DeeObject *right) const { return object(::DeeObject_CompareGrObject(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object operator >= (DEE_A_INOUT ::DeeObject *right) const { return object(::DeeObject_CompareGeObject(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline object &operator += (DEE_A_INOUT ::DeeObject *right) { if (::DeeObject_InplaceAddVar(&this->ob_ptr,right) != 0) detail::throw_last_error(); return *this; }
  inline object &operator -= (DEE_A_INOUT ::DeeObject *right) { if (::DeeObject_InplaceSubVar(&this->ob_ptr,right) != 0) detail::throw_last_error(); return *this; }
  inline object &operator *= (DEE_A_INOUT ::DeeObject *right) { if (::DeeObject_InplaceMulVar(&this->ob_ptr,right) != 0) detail::throw_last_error(); return *this; }
  inline object &operator /= (DEE_A_INOUT ::DeeObject *right) { if (::DeeObject_InplaceDivVar(&this->ob_ptr,right) != 0) detail::throw_last_error(); return *this; }
  inline object &operator %= (DEE_A_INOUT ::DeeObject *right) { if (::DeeObject_InplaceModVar(&this->ob_ptr,right) != 0) detail::throw_last_error(); return *this; }
  inline object &operator <<= (DEE_A_INOUT ::DeeObject *right) { if (::DeeObject_InplaceShlVar(&this->ob_ptr,right) != 0) detail::throw_last_error(); return *this; }
  inline object &operator >>= (DEE_A_INOUT ::DeeObject *right) { if (::DeeObject_InplaceShrVar(&this->ob_ptr,right) != 0) detail::throw_last_error(); return *this; }
  inline object &operator &= (DEE_A_INOUT ::DeeObject *right) { if (::DeeObject_InplaceAndVar(&this->ob_ptr,right) != 0) detail::throw_last_error(); return *this; }
  inline object &operator |= (DEE_A_INOUT ::DeeObject *right) { if (::DeeObject_InplaceOrVar(&this->ob_ptr,right) != 0) detail::throw_last_error(); return *this; }
  inline object &operator ^= (DEE_A_INOUT ::DeeObject *right) { if (::DeeObject_InplaceXorVar(&this->ob_ptr,right) != 0) detail::throw_last_error(); return *this; }
 public: /* public operators (generic sequencing) */
  inline DEE_A_RET_WUNUSED ::Dee_ssize_t find(DEE_A_INOUT ::DeeObject *findob) const { ::Dee_ssize_t result; if ((result = ::DeeSequence_Find(this->ob_ptr,findob)) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED ::Dee_ssize_t rfind(DEE_A_INOUT ::DeeObject *findob) const { ::Dee_ssize_t result; if ((result = ::DeeSequence_RFind(this->ob_ptr,findob)) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED ::Dee_size_t index(DEE_A_INOUT ::DeeObject *findob) const { ::Dee_size_t result; if ((result = ::DeeSequence_Index(this->ob_ptr,findob)) == (::Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED ::Dee_size_t rindex(DEE_A_INOUT ::DeeObject *findob) const { ::Dee_size_t result; if ((result = ::DeeSequence_RIndex(this->ob_ptr,findob)) == (::Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED ::Dee_size_t count(DEE_A_INOUT ::DeeObject *findob) const { ::Dee_size_t result; if ((result = ::DeeSequence_Count(this->ob_ptr,findob)) == (::Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED object locate(DEE_A_INOUT ::DeeObject *pred) const { return object(::DeeSequence_Locate(this->ob_ptr,pred),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object rlocate(DEE_A_INOUT ::DeeObject *pred) const { return object(::DeeSequence_RLocate(this->ob_ptr,pred),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object locate_all(DEE_A_INOUT ::DeeObject *pred) const { return object(::DeeSequence_LocateAll(this->ob_ptr,pred),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object transform(DEE_A_INOUT ::DeeObject *pred) const { return object(::DeeSequence_Transform(this->ob_ptr,pred),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED bool empty() const { int result; if ((result = ::DeeSequence_Empty(this->ob_ptr)) < 0) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED bool non_empty() const { int result; if ((result = ::DeeSequence_NonEmpty(this->ob_ptr)) < 0) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED object front() const { return object(::DeeSequence_Front(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object back() const { return object(::DeeSequence_Back(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object at(DEE_A_IN ::Dee_size_t idx) const { return object(::DeeSequence_At(this->ob_ptr,idx),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object reversed() const { return object(::DeeSequence_Reversed(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object sorted() const { return object(::DeeSequence_Sorted(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object sorted(DEE_A_INOUT ::DeeObject *pred) const { return object(::DeeSequence_SortedPred(this->ob_ptr,pred),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object segments(DEE_A_IN ::Dee_size_t n) const { return object(::DeeSequence_Segments(this->ob_ptr,n),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED bool contains(DEE_A_INOUT ::DeeObject *elem) const { int result; if ((result = ::DeeSequence_Contains(this->ob_ptr,elem)) < 0) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED bool contains(DEE_A_INOUT ::DeeObject *elem, DEE_A_INOUT ::DeeObject *pred) const { int result; if ((result = ::DeeSequence_ContainsPred(this->ob_ptr,elem,pred)) < 0) detail::throw_last_error(); return !!result; }
 public: /* public operators (cxx-style) */
  inline DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL operator DeeObject *() const DEE_CXX11_NOEXCEPT { return this->ob_ptr; }
  inline object &operator = (DEE_A_IN object const &right) DEE_CXX11_NOEXCEPT { this->priv_assign_ptr(right.ob_ptr); return *this; }
  inline object &operator = (DEE_A_INOUT ::DeeObject *ob) DEE_CXX11_NOEXCEPT { this->priv_assign_ptr(ob); return *this; }
 public: /* public constructor / destructor */
  inline object(DEE_A_IN object const &right) DEE_CXX11_NOEXCEPT: ob_ptr(right.ob_ptr) { Dee_INCREF(this->ob_ptr); }
#ifdef DEE_DEBUG
  inline explicit object(::deemon::detail::tag_init_later) DEE_CXX11_NOEXCEPT: ob_ptr(NULL) {}
#else
  inline explicit object(::deemon::detail::tag_init_later) DEE_CXX11_NOEXCEPT {}
#endif
  inline explicit object(DEE_A_INOUT ::DeeObject *ob) DEE_CXX11_NOEXCEPT { DEE_ASSERT(DeeObject_Check(ob)); Dee_INCREF(this->ob_ptr = ob); }
  inline explicit object(DEE_A_IN_OPT ::DeeObject *ob, detail::tag_check_error) { if (!ob) detail::throw_last_error(); DEE_ASSERT(DeeObject_Check(ob)); Dee_INCREF(this->ob_ptr = ob); }
  inline explicit object(DEE_A_INOUT ::DeeObject *ob, detail::tag_inherit_ref) DEE_CXX11_NOEXCEPT: ob_ptr(ob) { DEE_ASSERT(DeeObject_Check(ob)); }
  inline explicit object(DEE_A_IN_OPT ::DeeObject *ob, detail::tag_ref_or_err) { if (!ob) detail::throw_last_error(); DEE_ASSERT(DeeObject_Check(ob)); this->ob_ptr = ob; }
  inline ~object() DEE_CXX11_NOEXCEPT { Dee_DECREF(this->ob_ptr); }
#define DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(T,base,check)\
  inline T &operator = (DEE_A_IN T const &right) DEE_CXX11_NOEXCEPT { this->priv_assign_ptr(right.priv_get()); return *this; }\
  inline T &operator = (DEE_A_INOUT ::DeeObject *ob) DEE_CXX11_NOEXCEPT { DEE_ASSERT(DeeObject_Check(ob) && check(ob)); this->priv_assign_ptr(ob); return *this; }\
  inline T(DEE_A_IN T const &right) DEE_CXX11_NOEXCEPT: base(right) {}\
  inline explicit T(::deemon::detail::tag_init_later) DEE_CXX11_NOEXCEPT: base(::deemon::detail::tag_init_later()) {}\
  inline explicit T(DEE_A_INOUT ::DeeObject *ob) DEE_CXX11_NOEXCEPT: base(ob) { DEE_ASSERT(!ob || check(ob)); }\
  inline explicit T(DEE_A_IN_OPT ::DeeObject *ob, ::deemon::detail::tag_check_error): base(ob,::deemon::detail::tag_check_error()) { DEE_ASSERT(!ob || check(ob)); }\
  inline explicit T(DEE_A_INOUT ::DeeObject *ob, ::deemon::detail::tag_inherit_ref) DEE_CXX11_NOEXCEPT: base(ob,::deemon::detail::tag_inherit_ref()) { DEE_ASSERT(!ob || check(ob)); }\
  inline explicit T(DEE_A_IN_OPT ::DeeObject *ob, ::deemon::detail::tag_ref_or_err): base(ob,::deemon::detail::tag_ref_or_err()) { DEE_ASSERT(!ob || check(ob)); }
};
struct object::_proxy_seq_item_object {
 friend struct object;
 DEE_CXX_DELETE_CLASSDEFAULT(_proxy_seq_item_object);
 protected: /* protected vars */
  DEE_A_REF DeeObject *m_ob;
  DEE_A_REF DeeObject *m_key;
 public: /* public operators */
  inline operator object () const { return object(::DeeObject_GetItem(this->m_ob,this->m_key),detail::tag_ref_or_err()); }
  inline _proxy_seq_item_object const &operator = (DEE_A_INOUT ::DeeObject *ob) const { if (::DeeObject_SetItem(this->m_ob,this->m_key,ob) != 0) detail::throw_last_error(); return *this; }
  inline void del() const { if (::DeeObject_DelItem(this->m_ob,this->m_key) != 0) detail::throw_last_error(); }
 private: /* private constructor / destructor */
  inline explicit _proxy_seq_item_object(DEE_A_INOUT DeeObject *ob, DEE_A_INOUT DeeObject *key) DEE_CXX11_NOEXCEPT: m_ob(ob), m_key(key) { Dee_INCREF(key); }
 public: /* public constructor / destructor */
  inline ~_proxy_seq_item_object() DEE_CXX11_NOEXCEPT { Dee_DECREF(this->m_key); }
};
struct object::_proxy_seq_item_string {
 friend struct object;
 DEE_CXX_DELETE_CLASSDEFAULT(_proxy_seq_item_string);
 protected: /* protected vars */
  DEE_A_REF DeeObject *m_ob;
           char const *m_key;
 public: /* public operators */
  inline operator object () const { return object(::DeeObject_GetItemString(this->m_ob,this->m_key),detail::tag_ref_or_err()); }
  inline _proxy_seq_item_string const &operator = (DEE_A_INOUT ::DeeObject *ob) const { if (::DeeObject_SetItemString(this->m_ob,this->m_key,ob) != 0) detail::throw_last_error(); return *this; }
  inline void del() const { if (::DeeObject_DelItemString(this->m_ob,this->m_key) != 0) detail::throw_last_error(); }
 private: /* private constructor / destructor */
  inline explicit _proxy_seq_item_string(DEE_A_INOUT DeeObject *ob, DEE_A_IN_Z char const *key) DEE_CXX11_NOEXCEPT: m_ob(ob), m_key(key) {}
 public: /* public constructor / destructor */
  inline ~_proxy_seq_item_string() DEE_CXX11_NOEXCEPT {}
};
struct object::_proxy_seq_item_index {
 friend struct object;
 DEE_CXX_DELETE_CLASSDEFAULT(_proxy_seq_item_index);
 protected: /* protected vars */
  DEE_A_REF DeeObject *m_ob;
          ::Dee_size_t m_idx;
 public: /* public operators */
  inline operator object () const { return object(::DeeObject_GetIndex(this->m_ob,this->m_idx),detail::tag_ref_or_err()); }
  inline _proxy_seq_item_index const &operator = (DEE_A_INOUT ::DeeObject *ob) const { if (::DeeObject_SetIndex(this->m_ob,this->m_idx,ob) != 0) detail::throw_last_error(); return *this; }
  inline void del() const { if (::DeeObject_DelIndex(this->m_ob,this->m_idx) != 0) detail::throw_last_error(); }
 private: /* private constructor / destructor */
  inline explicit _proxy_seq_item_index(DEE_A_INOUT DeeObject *ob, DEE_A_IN_Z ::Dee_size_t idx) DEE_CXX11_NOEXCEPT: m_ob(ob), m_idx(idx) {}
 public: /* public constructor / destructor */
  inline ~_proxy_seq_item_index() DEE_CXX11_NOEXCEPT {}
};

#ifndef __INTELLISENSE__
inline DEE_A_RET_WUNUSED object::_proxy_seq_item_index  object::operator [] (DEE_A_IN ::Dee_size_t index) const DEE_CXX11_NOEXCEPT { return _proxy_seq_item_index(this->ob_ptr,index); }
inline DEE_A_RET_WUNUSED object::_proxy_seq_item_object object::operator [] (DEE_A_INOUT ::DeeObject *index_or_key) const DEE_CXX11_NOEXCEPT { return _proxy_seq_item_object(this->ob_ptr,index_or_key); }
inline DEE_A_RET_WUNUSED object::_proxy_seq_item_string object::operator [] (DEE_A_IN_Z char const *key) const DEE_CXX11_NOEXCEPT { return _proxy_seq_item_string(this->ob_ptr,key); }
#endif


inline void del(object::_proxy_seq_item_object const &ob) { ob.del(); }
inline void del(object::_proxy_seq_item_string const &ob) { ob.del(); }
inline void del(object::_proxy_seq_item_index const &ob) { ob.del(); }

namespace detail {
template<> struct _typeof_impl<DeeObject *> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(object); }
 static inline DeeObject *new_(DeeObject *v) DEE_CXX11_NOEXCEPT { DeeReturn_NEWREF(v); }
};
template<> struct _typeof_impl<object> {
 static inline DeeTypeObject *type_() DEE_CXX11_NOEXCEPT { return DeeObject_TYPE(object); }
 static inline DeeObject *new_(object const &v) DEE_CXX11_NOEXCEPT { DeeReturn_NEWREF(v.priv_get()); }
};
}

//////////////////////////////////////////////////////////////////////////
// Returns the object type that deemon uses to implement 'T'
// Returns NULL if it failed to retrieve the type
template<class T> inline DEE_A_RET_EXCEPT(NULL) DEE_PRIVATE_CXX_ENABLEIF_SUPPORTED(T,::DeeTypeObject *) typeof() DEE_CXX11_NOEXCEPT { return ::deemon::detail::_typeof_impl<T>::type_(); }

//////////////////////////////////////////////////////////////////////////
// Returns a deemon object containing the data of 'ob'
// >> If 'ob' is a non-constant lvalue, this function will return an lvalue as well
template<class T> inline DEE_A_RET_EXCEPT(NULL) DEE_PRIVATE_CXX_ENABLEIF_SUPPORTED(T&,::DeeObject *) new_(T &ob) DEE_CXX11_NOEXCEPT { return ::deemon::detail::_typeof_impl<T&>::new_(ob); }
template<class T> inline DEE_A_RET_EXCEPT(NULL) DEE_PRIVATE_CXX_ENABLEIF_SUPPORTED(T,::DeeObject *) new_(T const &ob) DEE_CXX11_NOEXCEPT { return ::deemon::detail::_typeof_impl<T>::new_(ob); }

}
DEE_DECL_CXX_END
#endif


#endif /* !GUARD_DEEMON_CXX_OBJECT_H */
