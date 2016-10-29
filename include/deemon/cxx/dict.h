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
#ifndef GUARD_DEEMON_CXX_DICT_H
#define GUARD_DEEMON_CXX_DICT_H 1

#include <deemon/__conf.inl>
#include <deemon/none.h>
#include <deemon/dict.h>
#include <deemon/cxx/tuple.h>
#include <deemon/cxx/list.h>
#include <deemon/cxx/object.h>

#if DEE_HAVE_CXX_API
DEE_DECL_CXX_BEGIN
namespace deemon {

struct dict: object {
 private: /* private types */
  struct _proxy_max_load_factor {
   private: DeeObject *m_ob; public:
   inline operator double () const DEE_CXX11_NOEXCEPT { return DeeDict_GET_MAX_LOAD_FACTOR(this->m_ob); }
   inline _proxy_max_load_factor const &operator = (DEE_A_IN double v) const DEE_CXX11_NOEXCEPT { DeeDict_SET_MAX_LOAD_FACTOR(this->m_ob,v); return *this; }
  };
  struct _proxy_dictitem_object;
  struct _proxy_dictitem_string;
 public: /* public types */
  friend inline void del(_proxy_dictitem_object const &ob);
  friend inline void del(_proxy_dictitem_string const &ob);
 public: /* public types */
  struct _keys;
  struct _items;
  struct _values;
 public: /* public methods */
  inline _proxy_max_load_factor const &max_load_factor() const { return reinterpret_cast< _proxy_max_load_factor const &>(*this); }
  inline _proxy_max_load_factor const &ob_max_load_factor() const { return reinterpret_cast< _proxy_max_load_factor const &>(*this); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED _keys keys() const;
  inline DEE_A_RET_WUNUSED _items items() const;
  inline DEE_A_RET_WUNUSED _values values() const;
 public: /* public methods */
  inline DEE_A_RET_WUNUSED Dee_size_t size() const DEE_CXX11_NOEXCEPT { return DeeDict_SIZE(this->ob_ptr); }
  inline DEE_A_RET_WUNUSED bool empty() const DEE_CXX11_NOEXCEPT { return DeeDict_SIZE(this->ob_ptr) == 0; }
  inline DEE_A_RET_WUNUSED bool non_empty() const DEE_CXX11_NOEXCEPT { return DeeDict_SIZE(this->ob_ptr) != 0; }
  inline void clear() const DEE_CXX11_NOEXCEPT { ::DeeDict_Clear(this->ob_ptr); }
  inline DEE_A_RET_WUNUSED Dee_size_t bucket_count() const DEE_CXX11_NOEXCEPT { return DeeDict_BUCKET_COUNT(this->ob_ptr); }
  inline DEE_A_RET_WUNUSED Dee_size_t bucket_size(DEE_A_IN Dee_ssize_t bucket) const DEE_CXX11_NOEXCEPT { return DeeDict_BucketSize(this->ob_ptr,bucket); }
 public: /* public methods */
  inline object pop(DEE_A_IN_Z char const *key) const { return object(::DeeDict_PopString(this->ob_ptr,key,NULL),detail::tag_ref_or_err()); }
  inline object pop(DEE_A_IN_Z char const *key, DEE_A_INOUT_OPT ::DeeObject *def) const { return object(::DeeDict_PopString(this->ob_ptr,key,def),detail::tag_ref_or_err()); }
  inline object pop(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) const { return object(::DeeDict_PopStringWithKnownBucket(this->ob_ptr,key,bucket,NULL),detail::tag_ref_or_err()); }
  inline object pop(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT_OPT ::DeeObject *def) const { return object(::DeeDict_PopStringWithKnownBucket(this->ob_ptr,key,bucket,def),detail::tag_ref_or_err()); }
  inline object pop(DEE_A_INOUT ::DeeObject *key) const { return object(::DeeDict_Pop(this->ob_ptr,key,NULL),detail::tag_ref_or_err()); }
  inline object pop(DEE_A_INOUT ::DeeObject *key, DEE_A_INOUT_OPT ::DeeObject *def) const { return object(::DeeDict_Pop(this->ob_ptr,key,def),detail::tag_ref_or_err()); }
  inline object pop(DEE_A_INOUT ::DeeObject *key, DEE_A_IN Dee_hash_t bucket) const { return object(::DeeDict_PopWithKnownBucket(this->ob_ptr,key,bucket,NULL),detail::tag_ref_or_err()); }
  inline object pop(DEE_A_INOUT ::DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT_OPT ::DeeObject *def) const { return object(::DeeDict_PopWithKnownBucket(this->ob_ptr,key,bucket,def),detail::tag_ref_or_err()); }
 public: /* public methods */
  inline object setdefault(DEE_A_IN_Z char const *key) const { return object(::DeeDict_SetDefaultString(this->ob_ptr,key,Dee_None),detail::tag_ref_or_err()); }
  inline object setdefault(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) const { return object(::DeeDict_SetDefaultStringWithKnownBucket(this->ob_ptr,key,bucket,Dee_None),detail::tag_ref_or_err()); }
  inline object setdefault(DEE_A_IN_Z char const *key, DEE_A_INOUT ::DeeObject *def) const { return object(::DeeDict_SetDefaultString(this->ob_ptr,key,def),detail::tag_ref_or_err()); }
  inline object setdefault(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT ::DeeObject *def) const { return object(::DeeDict_SetDefaultStringWithKnownBucket(this->ob_ptr,key,bucket,def),detail::tag_ref_or_err()); }
  inline object setdefault(DEE_A_INOUT ::DeeObject *key) const { return object(::DeeDict_SetDefault(this->ob_ptr,key,Dee_None),detail::tag_ref_or_err()); }
  inline object setdefault(DEE_A_INOUT ::DeeObject *key, DEE_A_IN Dee_hash_t bucket) const { return object(::DeeDict_SetDefaultWithKnownBucket(this->ob_ptr,key,bucket,Dee_None),detail::tag_ref_or_err()); }
  inline object setdefault(DEE_A_INOUT ::DeeObject *key, DEE_A_INOUT ::DeeObject *def) const { return object(::DeeDict_SetDefault(this->ob_ptr,key,def),detail::tag_ref_or_err()); }
  inline object setdefault(DEE_A_INOUT ::DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT ::DeeObject *def) const { return object(::DeeDict_SetDefaultWithKnownBucket(this->ob_ptr,key,bucket,def),detail::tag_ref_or_err()); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED object get(DEE_A_IN_Z char const *key) const { return object(::DeeDict_GetDefaultString(this->ob_ptr,key,Dee_None),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object get(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) const { return object(::DeeDict_GetDefaultStringWithKnownBucket(this->ob_ptr,key,bucket,Dee_None),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object get(DEE_A_IN_Z char const *key, DEE_A_INOUT ::DeeObject *def) const { return object(::DeeDict_GetDefaultString(this->ob_ptr,key,def),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object get(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT ::DeeObject *def) const { return object(::DeeDict_GetDefaultStringWithKnownBucket(this->ob_ptr,key,bucket,def),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object get(DEE_A_INOUT ::DeeObject *key) const { return object(::DeeDict_GetDefault(this->ob_ptr,key,Dee_None),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object get(DEE_A_INOUT ::DeeObject *key, DEE_A_IN Dee_hash_t bucket) const { return object(::DeeDict_GetDefaultWithKnownBucket(this->ob_ptr,key,bucket,Dee_None),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object get(DEE_A_INOUT ::DeeObject *key, DEE_A_INOUT ::DeeObject *def) const { return object(::DeeDict_GetDefault(this->ob_ptr,key,def),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object get(DEE_A_INOUT ::DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT ::DeeObject *def) const { return object(::DeeDict_GetDefaultWithKnownBucket(this->ob_ptr,key,bucket,def),detail::tag_ref_or_err()); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED tuple front() const { DeeObject *key,*item; DeeDict_Front(this->ob_ptr,&key,NULL,&item); return tuple(::DeeTuple_Pack(2,key,item),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED tuple back() const { DeeObject *key,*item; DeeDict_Back(this->ob_ptr,&key,NULL,&item); return tuple(::DeeTuple_Pack(2,key,item),detail::tag_ref_or_err()); }
 public: /* public methods */
  inline tuple popitem() const {
   ::DeeObject *key,*item,*result;
   if (::DeeDict_PopSomething(this->ob_ptr,&key,&item) != 0) detail::throw_last_error();
   result = ::DeeTuple_Pack(2,key,item);
   Dee_DECREF(key);
   Dee_DECREF(item);
   return tuple(result,detail::tag_ref_or_err());
  }
  inline void popitem(DEE_A_OUT DeeObject *&key, DEE_A_OUT DeeObject *&item) const { if (::DeeDict_PopSomething(this->ob_ptr,&key,&item) != 0) detail::throw_last_error(); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_IN_Z char const *key, DEE_A_INOUT ::DeeObject *item) const { Dee_ssize_t result; if ((result = ::DeeDict_FindKeyItemIndexString(this->ob_ptr,key,item)) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT ::DeeObject *item) const { Dee_ssize_t result; if ((result = ::DeeDict_FindKeyItemIndexStringWithKnownBucket(this->ob_ptr,key,bucket,item)) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_INOUT ::DeeObject *key, DEE_A_INOUT ::DeeObject *item) const { Dee_ssize_t result; if ((result = ::DeeDict_FindKeyItemIndex(this->ob_ptr,key,item)) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_INOUT ::DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT ::DeeObject *item) const { Dee_ssize_t result; if ((result = ::DeeDict_FindKeyItemIndexWithKnownBucket(this->ob_ptr,key,bucket,item)) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_IN tuple const &key_item) const { Dee_ssize_t result; if ((result = ::DeeDict_FindKeyItemIndex(this->ob_ptr,DeeTuple_GET(key_item.priv_get(),0),DeeTuple_GET(key_item.priv_get(),1))) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_IN_Z char const *key, DEE_A_INOUT ::DeeObject *item) const { Dee_size_t result; if ((result = ::DeeDict_IndexKeyItemIndexString(this->ob_ptr,key,item)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT ::DeeObject *item) const { Dee_size_t result; if ((result = ::DeeDict_IndexKeyItemIndexStringWithKnownBucket(this->ob_ptr,key,bucket,item)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_INOUT ::DeeObject *key, DEE_A_INOUT ::DeeObject *item) const { Dee_size_t result; if ((result = ::DeeDict_IndexKeyItemIndex(this->ob_ptr,key,item)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_INOUT ::DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT ::DeeObject *item) const { Dee_size_t result; if ((result = ::DeeDict_IndexKeyItemIndexWithKnownBucket(this->ob_ptr,key,bucket,item)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_IN tuple const &key_item) const { Dee_size_t result; if ((result = ::DeeDict_IndexKeyItemIndex(this->ob_ptr,DeeTuple_GET(key_item.priv_get(),0),DeeTuple_GET(key_item.priv_get(),1))) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_IN_Z char const *key, DEE_A_INOUT ::DeeObject *item) const { int result; if ((result = ::DeeDict_HasKeyItemString(this->ob_ptr,key,item)) < 0) detail::throw_last_error(); return (Dee_size_t)result; }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT ::DeeObject *item) const { int result; if ((result = ::DeeDict_HasKeyItemStringWithKnownBucket(this->ob_ptr,key,bucket,item)) < 0) detail::throw_last_error(); return (Dee_size_t)result; }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_INOUT ::DeeObject *key, DEE_A_INOUT ::DeeObject *item) const { int result; if ((result = ::DeeDict_HasKeyItem(this->ob_ptr,key,item)) < 0) detail::throw_last_error(); return (Dee_size_t)result; }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_INOUT ::DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT ::DeeObject *item) const { int result; if ((result = ::DeeDict_HasKeyItemWithKnownBucket(this->ob_ptr,key,bucket,item)) < 0) detail::throw_last_error(); return (Dee_size_t)result; }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_IN tuple const &key_item) const { int result; if ((result = ::DeeDict_HasKeyItem(this->ob_ptr,DeeTuple_GET(key_item.priv_get(),0),DeeTuple_GET(key_item.priv_get(),1))) < 0) detail::throw_last_error(); return (Dee_size_t)result; }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED object locate(DEE_A_INOUT ::DeeObject *pred) const { return object(::DeeDict_LocateValue(this->ob_ptr,pred),detail::tag_ref_or_err()); }
  using object::rlocate;
  inline DEE_A_RET_WUNUSED dict locate_all(DEE_A_INOUT ::DeeObject *pred) const { return dict(::DeeDict_LocateAll(this->ob_ptr,pred),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED dict transform(DEE_A_INOUT ::DeeObject *pred) const { return dict(::DeeDict_Transform(this->ob_ptr,pred),detail::tag_ref_or_err()); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED dict copy() const { return dict(::DeeDict_Copy(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED dict deep_copy() const { return dict(::DeeDict_DeepCopy(this->ob_ptr),detail::tag_ref_or_err()); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED bool hasitem(DEE_A_IN_Z char const *key) const { int result; if ((result = ::DeeDict_HasKeyString(this->ob_ptr,key)) < 0) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED bool hasitem(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) const { int result; if ((result = ::DeeDict_HasKeyStringWithKnownBucket(this->ob_ptr,key,bucket)) < 0) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED bool hasitem(DEE_A_IN DeeObject *key) const { int result; if ((result = ::DeeDict_HasKey(this->ob_ptr,key)) < 0) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED bool hasitem(DEE_A_IN DeeObject *key, DEE_A_IN Dee_hash_t bucket) const { int result; if ((result = ::DeeDict_HasKeyWithKnownBucket(this->ob_ptr,key,bucket)) < 0) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED object getitem(DEE_A_IN_Z char const *key) const { return object(::DeeDict_GetItemString(this->ob_ptr,key),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object getitem(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) const { return object(::DeeDict_GetItemStringWithKnownBucket(this->ob_ptr,key,bucket),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object getitem(DEE_A_IN DeeObject *key) const { return object(::DeeDict_GetItem(this->ob_ptr,key),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object getitem(DEE_A_IN DeeObject *key, DEE_A_IN Dee_hash_t bucket) const { return object(::DeeDict_GetItemWithKnownBucket(this->ob_ptr,key,bucket),detail::tag_ref_or_err()); }
  inline DEE_A_RET_SUCCESS(true) bool try_getitem(DEE_A_IN_Z char const *key, DEE_A_REF DEE_A_OUT DeeObject *&result) const { int error; if ((error = ::DeeDict_GetItemExString(this->ob_ptr,key,&result)) < 0) detail::throw_last_error(); return !error; }
  inline DEE_A_RET_SUCCESS(true) bool try_getitem(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_REF DEE_A_OUT DeeObject *&result) const { int error; if ((error = ::DeeDict_GetItemExStringWithKnownBucket(this->ob_ptr,key,bucket,&result)) < 0) detail::throw_last_error(); return !error; }
  inline DEE_A_RET_SUCCESS(true) bool try_getitem(DEE_A_IN DeeObject *key, DEE_A_REF DEE_A_OUT DeeObject *&result) const { int error; if ((error = ::DeeDict_GetItemEx(this->ob_ptr,key,&result)) < 0) detail::throw_last_error(); return !error; }
  inline DEE_A_RET_SUCCESS(true) bool try_getitem(DEE_A_IN DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_REF DEE_A_OUT DeeObject *&result) const { int error; if ((error = ::DeeDict_GetItemExWithKnownBucket(this->ob_ptr,key,bucket,&result)) < 0) detail::throw_last_error(); return !error; }
  inline void delitem(DEE_A_IN_Z char const *key) const { if (::DeeDict_DelItemStringWithError(this->ob_ptr,key) < 0) detail::throw_last_error(); }
  inline void delitem(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) const { if (::DeeDict_DelItemStringWithErrorAndKnownBucket(this->ob_ptr,key,bucket) < 0) detail::throw_last_error(); }
  inline void setitem(DEE_A_IN_Z char const *key, DEE_A_IN DeeObject *v) const { if (::DeeDict_SetItemString(this->ob_ptr,key,v) < 0) detail::throw_last_error(); }
  inline void setitem(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_IN DeeObject *v) const { if (::DeeDict_SetItemStringWithKnownBucket(this->ob_ptr,key,bucket,v) < 0) detail::throw_last_error(); }
  inline void delitem(DEE_A_IN DeeObject *key) const { if (::DeeDict_DelItemWithError(this->ob_ptr,key) < 0) detail::throw_last_error(); }
  inline void delitem(DEE_A_IN DeeObject *key, DEE_A_IN Dee_hash_t bucket) const { if (::DeeDict_DelItemWithErrorAndKnownBucket(this->ob_ptr,key,bucket) < 0) detail::throw_last_error(); }
  inline void setitem(DEE_A_IN DeeObject *key, DEE_A_IN DeeObject *v) const { if (::DeeDict_SetItem(this->ob_ptr,key,v) < 0) detail::throw_last_error(); }
  inline void setitem(DEE_A_IN DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_IN DeeObject *v) const { if (::DeeDict_SetItemWithKnownBucket(this->ob_ptr,key,bucket,v) < 0) detail::throw_last_error(); }
  inline bool setnew(DEE_A_IN_Z char const *key, DEE_A_IN DeeObject *v) const { int result; if ((result = ::DeeDict_SetNewItemString(this->ob_ptr,key,v,NULL)) < 0) detail::throw_last_error(); return !result; }
  inline bool setnew(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_IN DeeObject *v) const { int result; if ((result = ::DeeDict_SetNewItemStringWithKnownBucket(this->ob_ptr,key,bucket,v,NULL)) < 0) detail::throw_last_error(); return !result; }
  inline bool setnew(DEE_A_IN DeeObject *key, DEE_A_IN DeeObject *v) const { int result; if ((result = ::DeeDict_SetNewItem(this->ob_ptr,key,v,NULL)) < 0) detail::throw_last_error(); return !result; }
  inline bool setnew(DEE_A_IN DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_IN DeeObject *v) const { int result; if ((result = ::DeeDict_SetNewItemWithKnownBucket(this->ob_ptr,key,bucket,v,NULL)) < 0) detail::throw_last_error(); return !result; }
  inline bool setold(DEE_A_IN_Z char const *key, DEE_A_IN DeeObject *v) const { int result; if ((result = ::DeeDict_SetOldItemString(this->ob_ptr,key,v,NULL)) < 0) detail::throw_last_error(); return !result; }
  inline bool setold(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_IN DeeObject *v) const { int result; if ((result = ::DeeDict_SetOldItemStringWithKnownBucket(this->ob_ptr,key,bucket,v,NULL)) < 0) detail::throw_last_error(); return !result; }
  inline bool setold(DEE_A_IN DeeObject *key, DEE_A_IN DeeObject *v) const { int result; if ((result = ::DeeDict_SetOldItem(this->ob_ptr,key,v,NULL)) < 0) detail::throw_last_error(); return !result; }
  inline bool setold(DEE_A_IN DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_IN DeeObject *v) const { int result; if ((result = ::DeeDict_SetOldItemWithKnownBucket(this->ob_ptr,key,bucket,v,NULL)) < 0) detail::throw_last_error(); return !result; }
 public: /* public operators (deemon-style) */
  inline DEE_A_RET_WUNUSED _proxy_dictitem_object operator [] (DEE_A_INOUT ::DeeObject *key) const DEE_CXX11_NOEXCEPT;
  inline DEE_A_RET_WUNUSED _proxy_dictitem_string operator [] (DEE_A_IN_Z char const *key) const DEE_CXX11_NOEXCEPT;
 public: /* public constructor / destructor */
  inline dict(): object(::DeeDict_New(),detail::tag_ref_or_err()) {}
  inline explicit dict(DEE_A_IN object const &seq): object(::DeeDict_NewFromSequence(seq.priv_get()),detail::tag_ref_or_err()) {}
  inline explicit dict(DEE_A_IN Dee_size_t pairc, DEE_A_IN_R(pairc*2) ::DeeObject *const *pairv): object(::DeeDict_NewFromKeyValuePairs(pairc,pairv),detail::tag_ref_or_err()) {}
  inline explicit dict(DEE_A_IN Dee_size_t pairc, DEE_A_IN_R(pairc*2) object const *pairv): object(::DeeDict_NewFromKeyValuePairs(pairc,reinterpret_cast< ::DeeObject *const *>(pairv)),detail::tag_ref_or_err()) {}
  DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(dict,object,DeeDict_Check)
};

struct dict::_keys: object {
 public: /* public methods */
  inline DEE_A_RET_WUNUSED Dee_size_t size() const DEE_CXX11_NOEXCEPT { return DeeDict_SIZE(DeeDictProxy_DICT(this->ob_ptr)); }
  inline DEE_A_RET_WUNUSED bool empty() const DEE_CXX11_NOEXCEPT { return DeeDict_SIZE(DeeDictProxy_DICT(this->ob_ptr)) == 0; }
  inline DEE_A_RET_WUNUSED bool non_empty() const DEE_CXX11_NOEXCEPT { return DeeDict_SIZE(DeeDictProxy_DICT(this->ob_ptr)) != 0; }
  inline void clear() const DEE_CXX11_NOEXCEPT { DeeDict_Clear(DeeDictProxy_DICT(this->ob_ptr)); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_IN_Z char const *key) const { Dee_ssize_t result; if ((result = ::DeeDict_FindKeyIndexString(DeeDictProxy_DICT(this->ob_ptr),key)) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) const { Dee_ssize_t result; if ((result = ::DeeDict_FindKeyIndexStringWithKnownBucket(DeeDictProxy_DICT(this->ob_ptr),key,bucket)) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_INOUT ::DeeObject *key) const { Dee_ssize_t result; if ((result = ::DeeDict_FindKeyIndex(DeeDictProxy_DICT(this->ob_ptr),key)) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_INOUT ::DeeObject *key, DEE_A_IN Dee_hash_t bucket) const { Dee_ssize_t result; if ((result = ::DeeDict_FindKeyIndexWithKnownBucket(DeeDictProxy_DICT(this->ob_ptr),key,bucket)) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_IN_Z char const *key) const { Dee_size_t result; if ((result = ::DeeDict_IndexKeyIndexString(DeeDictProxy_DICT(this->ob_ptr),key)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) const { Dee_size_t result; if ((result = ::DeeDict_IndexKeyIndexStringWithKnownBucket(DeeDictProxy_DICT(this->ob_ptr),key,bucket)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_IN_Z char const *key) const { int result; if ((result = ::DeeDict_HasKeyString(DeeDictProxy_DICT(this->ob_ptr),key)) < 0) detail::throw_last_error(); return (Dee_size_t)result; }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) const { int result; if ((result = ::DeeDict_HasKeyStringWithKnownBucket(DeeDictProxy_DICT(this->ob_ptr),key,bucket)) < 0) detail::throw_last_error(); return (Dee_size_t)result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_INOUT ::DeeObject *key) const { Dee_size_t result; if ((result = ::DeeDict_IndexKeyIndex(DeeDictProxy_DICT(this->ob_ptr),key)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_INOUT ::DeeObject *key, DEE_A_IN Dee_hash_t bucket) const { Dee_size_t result; if ((result = ::DeeDict_IndexKeyIndexWithKnownBucket(DeeDictProxy_DICT(this->ob_ptr),key,bucket)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_INOUT ::DeeObject *key) const { int result; if ((result = ::DeeDict_HasKey(DeeDictProxy_DICT(this->ob_ptr),key)) < 0) detail::throw_last_error(); return (Dee_size_t)result; }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_INOUT ::DeeObject *key, DEE_A_IN Dee_hash_t bucket) const { int result; if ((result = ::DeeDict_HasKeyWithKnownBucket(DeeDictProxy_DICT(this->ob_ptr),key,bucket)) < 0) detail::throw_last_error(); return (Dee_size_t)result; }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED object front() const DEE_CXX11_NOEXCEPT { DeeObject *result; DeeDict_Front(DeeDictProxy_DICT(this->ob_ptr),&result,NULL,NULL); return object(result); }
  inline DEE_A_RET_WUNUSED object back() const DEE_CXX11_NOEXCEPT { DeeObject *result; DeeDict_Back(DeeDictProxy_DICT(this->ob_ptr),&result,NULL,NULL); return object(result); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED object locate(DEE_A_INOUT ::DeeObject *pred) const { return object(::DeeDict_LocateKey(DeeDictProxy_DICT(this->ob_ptr),pred),detail::tag_ref_or_err()); }
  using object::rlocate;
  inline DEE_A_RET_WUNUSED list locate_all(DEE_A_INOUT ::DeeObject *pred) const { return list(::DeeDict_LocateAllKeys(DeeDictProxy_DICT(this->ob_ptr),pred),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED list transform(DEE_A_INOUT ::DeeObject *pred) const { return list(::DeeDict_TransformKeys(DeeDictProxy_DICT(this->ob_ptr),pred),detail::tag_ref_or_err()); }
 public: /* public constructor / destructor */
  DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(_keys,object,DeeDictKeys_Check)
};
struct dict::_items: object {
 public: /* public methods */
  inline DEE_A_RET_WUNUSED Dee_size_t size() const DEE_CXX11_NOEXCEPT { return DeeDict_SIZE(DeeDictProxy_DICT(this->ob_ptr)); }
  inline DEE_A_RET_WUNUSED bool empty() const DEE_CXX11_NOEXCEPT { return DeeDict_SIZE(DeeDictProxy_DICT(this->ob_ptr)) == 0; }
  inline DEE_A_RET_WUNUSED bool non_empty() const DEE_CXX11_NOEXCEPT { return DeeDict_SIZE(DeeDictProxy_DICT(this->ob_ptr)) != 0; }
  inline void clear() const DEE_CXX11_NOEXCEPT { ::DeeDict_Clear(DeeDictProxy_DICT(this->ob_ptr)); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_INOUT ::DeeObject *item) const { Dee_ssize_t result; if ((result = ::DeeDict_FindItemIndex(DeeDictProxy_DICT(this->ob_ptr),item)) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_INOUT ::DeeObject *item) const { Dee_size_t result; if ((result = ::DeeDict_IndexItemIndex(DeeDictProxy_DICT(this->ob_ptr),item)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_INOUT ::DeeObject *item) const { Dee_size_t result; if ((result = ::DeeDict_CountItem(DeeDictProxy_DICT(this->ob_ptr),item)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED object front() const DEE_CXX11_NOEXCEPT { DeeObject *result; DeeDict_Front(DeeDictProxy_DICT(this->ob_ptr),NULL,NULL,&result); return object(result); }
  inline DEE_A_RET_WUNUSED object back() const DEE_CXX11_NOEXCEPT { DeeObject *result; DeeDict_Back(DeeDictProxy_DICT(this->ob_ptr),NULL,NULL,&result); return object(result); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED object locate(DEE_A_INOUT ::DeeObject *pred) const { return object(::DeeDict_LocateItem(DeeDictProxy_DICT(this->ob_ptr),pred),detail::tag_ref_or_err()); }
  using object::rlocate;
  inline DEE_A_RET_WUNUSED list locate_all(DEE_A_INOUT ::DeeObject *pred) const { return list(::DeeDict_LocateAllItems(DeeDictProxy_DICT(this->ob_ptr),pred),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED list transform(DEE_A_INOUT ::DeeObject *pred) const { return list(::DeeDict_TransformItems(DeeDictProxy_DICT(this->ob_ptr),pred),detail::tag_ref_or_err()); }
 public: /* public constructor / destructor */
  DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(_items,object,DeeDictItems_Check)
};
struct dict::_values: object {
 public: /* public methods */
  inline DEE_A_RET_WUNUSED Dee_size_t size() const DEE_CXX11_NOEXCEPT { return DeeDict_SIZE(DeeDictProxy_DICT(this->ob_ptr)); }
  inline DEE_A_RET_WUNUSED bool empty() const DEE_CXX11_NOEXCEPT { return DeeDict_SIZE(DeeDictProxy_DICT(this->ob_ptr)) == 0; }
  inline DEE_A_RET_WUNUSED bool non_empty() const DEE_CXX11_NOEXCEPT { return DeeDict_SIZE(DeeDictProxy_DICT(this->ob_ptr)) != 0; }
  inline void clear() const DEE_CXX11_NOEXCEPT { ::DeeDict_Clear(DeeDictProxy_DICT(this->ob_ptr)); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_IN_Z char const *key, DEE_A_INOUT ::DeeObject *item) const { Dee_ssize_t result; if ((result = ::DeeDict_FindKeyItemIndexString(DeeDictProxy_DICT(this->ob_ptr),key,item)) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT ::DeeObject *item) const { Dee_ssize_t result; if ((result = ::DeeDict_FindKeyItemIndexStringWithKnownBucket(DeeDictProxy_DICT(this->ob_ptr),key,bucket,item)) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_INOUT ::DeeObject *key, DEE_A_INOUT ::DeeObject *item) const { Dee_ssize_t result; if ((result = ::DeeDict_FindKeyItemIndex(DeeDictProxy_DICT(this->ob_ptr),key,item)) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_INOUT ::DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT ::DeeObject *item) const { Dee_ssize_t result; if ((result = ::DeeDict_FindKeyItemIndexWithKnownBucket(DeeDictProxy_DICT(this->ob_ptr),key,bucket,item)) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_IN tuple const &key_item) const { Dee_ssize_t result; if ((result = ::DeeDict_FindKeyItemIndex(DeeDictProxy_DICT(this->ob_ptr),DeeTuple_GET(key_item.priv_get(),0),DeeTuple_GET(key_item.priv_get(),1))) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_IN_Z char const *key, DEE_A_INOUT ::DeeObject *item) const { Dee_size_t result; if ((result = ::DeeDict_IndexKeyItemIndexString(DeeDictProxy_DICT(this->ob_ptr),key,item)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT ::DeeObject *item) const { Dee_size_t result; if ((result = ::DeeDict_IndexKeyItemIndexStringWithKnownBucket(DeeDictProxy_DICT(this->ob_ptr),key,bucket,item)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_INOUT ::DeeObject *key, DEE_A_INOUT ::DeeObject *item) const { Dee_size_t result; if ((result = ::DeeDict_IndexKeyItemIndex(DeeDictProxy_DICT(this->ob_ptr),key,item)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_INOUT ::DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT ::DeeObject *item) const { Dee_size_t result; if ((result = ::DeeDict_IndexKeyItemIndexWithKnownBucket(DeeDictProxy_DICT(this->ob_ptr),key,bucket,item)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_IN tuple const &key_item) const { Dee_size_t result; if ((result = ::DeeDict_IndexKeyItemIndex(DeeDictProxy_DICT(this->ob_ptr),DeeTuple_GET(key_item.priv_get(),0),DeeTuple_GET(key_item.priv_get(),1))) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_IN_Z char const *key, DEE_A_INOUT ::DeeObject *item) const { int result; if ((result = ::DeeDict_HasKeyItemString(DeeDictProxy_DICT(this->ob_ptr),key,item)) < 0) detail::throw_last_error(); return (Dee_size_t)result; }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT ::DeeObject *item) const { int result; if ((result = ::DeeDict_HasKeyItemStringWithKnownBucket(DeeDictProxy_DICT(this->ob_ptr),key,bucket,item)) < 0) detail::throw_last_error(); return (Dee_size_t)result; }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_INOUT ::DeeObject *key, DEE_A_INOUT ::DeeObject *item) const { int result; if ((result = ::DeeDict_HasKeyItem(DeeDictProxy_DICT(this->ob_ptr),key,item)) < 0) detail::throw_last_error(); return (Dee_size_t)result; }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_INOUT ::DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT ::DeeObject *item) const { int result; if ((result = ::DeeDict_HasKeyItemWithKnownBucket(DeeDictProxy_DICT(this->ob_ptr),key,bucket,item)) < 0) detail::throw_last_error(); return (Dee_size_t)result; }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_IN tuple const &key_item) const { int result; if ((result = ::DeeDict_HasKeyItem(DeeDictProxy_DICT(this->ob_ptr),DeeTuple_GET(key_item.priv_get(),0),DeeTuple_GET(key_item.priv_get(),1))) < 0) detail::throw_last_error(); return (Dee_size_t)result; }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED tuple front() const { DeeObject *key,*item; DeeDict_Front(DeeDictProxy_DICT(this->ob_ptr),&key,NULL,&item); return tuple::pack(2,key,item); }
  inline DEE_A_RET_WUNUSED tuple back() const { DeeObject *key,*item; DeeDict_Back(DeeDictProxy_DICT(this->ob_ptr),&key,NULL,&item); return tuple::pack(2,key,item); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED object locate(DEE_A_INOUT ::DeeObject *pred) const { return object(::DeeDict_LocateValue(DeeDictProxy_DICT(this->ob_ptr),pred),detail::tag_ref_or_err()); }
  using object::rlocate;
  inline DEE_A_RET_WUNUSED list locate_all(DEE_A_INOUT ::DeeObject *pred) const { return list(::DeeDict_LocateAllValues(DeeDictProxy_DICT(this->ob_ptr),pred),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED list transform(DEE_A_INOUT ::DeeObject *pred) const { return list(::DeeDict_TransformValues(DeeDictProxy_DICT(this->ob_ptr),pred),detail::tag_ref_or_err()); }
 public: /* public constructor / destructor */
  DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(_values,object,DeeDictValues_Check)
};

struct dict::_proxy_dictitem_object {
 friend struct dict;
 DEE_CXX_DELETE_CLASSDEFAULT(_proxy_dictitem_object);
 protected: /* protected vars */
  DEE_A_REF DeeObject *m_ob;
  DEE_A_REF DeeObject *m_key;
 public: /* public operators */
  inline operator object () const { return object(::DeeDict_GetItem(this->m_ob,this->m_key),detail::tag_ref_or_err()); }
  inline _proxy_dictitem_object const &operator = (DEE_A_INOUT ::DeeObject *ob) const { if (::DeeDict_SetItem(this->m_ob,this->m_key,ob) != 0) detail::throw_last_error(); return *this; }
  inline void del() const { if (::DeeDict_DelItemWithError(this->m_ob,this->m_key) != 0) detail::throw_last_error(); }
 private: /* private constructor / destructor */
  inline explicit _proxy_dictitem_object(DEE_A_INOUT DeeObject *ob, DEE_A_INOUT DeeObject *key) DEE_CXX11_NOEXCEPT: m_ob(ob), m_key(key) { Dee_INCREF(key); }
 public: /* public constructor / destructor */
  inline ~_proxy_dictitem_object() DEE_CXX11_NOEXCEPT { Dee_DECREF(this->m_key); }
};
struct dict::_proxy_dictitem_string {
 friend struct dict;
 DEE_CXX_DELETE_CLASSDEFAULT(_proxy_dictitem_string);
 protected: /* protected vars */
  DEE_A_REF DeeObject *m_ob;
           char const *m_key;
 public: /* public operators */
  inline operator object () const { return object(::DeeDict_GetItemString(this->m_ob,this->m_key),detail::tag_ref_or_err()); }
  inline _proxy_dictitem_string const &operator = (DEE_A_INOUT ::DeeObject *ob) const { if (::DeeDict_SetItemString(this->m_ob,this->m_key,ob) != 0) detail::throw_last_error(); return *this; }
  inline void del() const { if (::DeeDict_DelItemStringWithError(this->m_ob,this->m_key) != 0) detail::throw_last_error(); }
 private: /* private constructor / destructor */
  inline explicit _proxy_dictitem_string(DEE_A_INOUT DeeObject *ob, DEE_A_IN_Z char const *key) DEE_CXX11_NOEXCEPT: m_ob(ob), m_key(key) {}
 public: /* public constructor / destructor */
  inline ~_proxy_dictitem_string() DEE_CXX11_NOEXCEPT {}
};

inline DEE_A_RET_WUNUSED dict::_keys dict::keys() const { return dict::_keys(::DeeDict_Keys(this->ob_ptr),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED dict::_items dict::items() const { return dict::_items(::DeeDict_Items(this->ob_ptr),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED dict::_values dict::values() const { return dict::_values(::DeeDict_Values(this->ob_ptr),detail::tag_ref_or_err()); }
#ifndef __INTELLISENSE__
inline DEE_A_RET_WUNUSED dict::_proxy_dictitem_object dict::operator [] (DEE_A_INOUT ::DeeObject *key) const DEE_CXX11_NOEXCEPT { return _proxy_dictitem_object(this->ob_ptr,key); }
inline DEE_A_RET_WUNUSED dict::_proxy_dictitem_string dict::operator [] (DEE_A_IN_Z char const *key) const DEE_CXX11_NOEXCEPT { return _proxy_dictitem_string(this->ob_ptr,key); }
#endif

// Global 'del' callbacks for dict items
inline void del(dict::_proxy_dictitem_object const &ob) { ob.del(); }
inline void del(dict::_proxy_dictitem_string const &ob) { ob.del(); }

}
DEE_DECL_CXX_END
#endif

#endif /* !GUARD_DEEMON_CXX_DICT_H */
