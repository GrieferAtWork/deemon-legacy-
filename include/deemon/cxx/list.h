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
#ifndef GUARD_DEEMON_CXX_LIST_H
#define GUARD_DEEMON_CXX_LIST_H 1

#include <deemon/__conf.inl>
#include <deemon/cxx/__features.inl>
#include <deemon/list.h>
#include <deemon/none.h>
#include <deemon/cxx/object.h>

#if DEE_HAVE_CXX_API
DEE_DECL_CXX_BEGIN
namespace deemon {

struct tuple;
struct list: object {
 public: /* public operators */
  inline Dee_size_t size() const DEE_CXX11_NOEXCEPT { return DeeList_Size(this->ob_ptr); }
 public: /* public methods */
#ifdef DEE_LIMITED_API
  inline DEE_A_RET_WUNUSED ::Dee_hash_t __hash__() const { ::Dee_hash_t result; if (::DeeList_HashEx(this->ob_ptr,DEE_HASH_START,&result) != 0) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED ::Dee_hash_t __hash__(DEE_A_IN ::Dee_hash_t start) const { ::Dee_hash_t result; if (::DeeList_HashEx(this->ob_ptr,start,&result) != 0) detail::throw_last_error(); return result; }
#else
  using object::__hash__;
#endif
 public: /* public methods */
  inline DEE_A_RET_WUNUSED tuple as_tuple() const;
 public: /* public operators */
  inline DEE_A_RET_WUNUSED list operator + (DEE_A_INOUT ::DeeObject *right) const { return list(::DeeList_Concat(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED list operator | (DEE_A_INOUT ::DeeObject *right) const { return list(::DeeList_Merge(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED list operator & (DEE_A_INOUT ::DeeObject *right) const { return list(::DeeList_Common(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED list operator * (DEE_A_IN Dee_size_t n) const { return list(::DeeList_Repeated(this->ob_ptr,n),detail::tag_ref_or_err()); }
  inline list &operator += (DEE_A_INOUT ::DeeObject *right) { if (::DeeList_InplaceConcat(this->ob_ptr,right) == -1) detail::throw_last_error(); return *this; }
  inline list &operator |= (DEE_A_INOUT ::DeeObject *right) { if (::DeeList_InplaceMerge(this->ob_ptr,right) == -1) detail::throw_last_error(); return *this; }
  inline list &operator &= (DEE_A_INOUT ::DeeObject *right) { if (::DeeList_InplaceCommon(this->ob_ptr,right) == -1) detail::throw_last_error(); return *this; }
  inline list &operator *= (DEE_A_IN Dee_size_t n) { if (::DeeList_Repeat(this->ob_ptr,n) == -1) detail::throw_last_error(); return *this; }
  inline list const &operator += (DEE_A_INOUT ::DeeObject *right) const { if (::DeeList_InplaceConcat(this->ob_ptr,right) == -1) detail::throw_last_error(); return *this; }
  inline list const &operator |= (DEE_A_INOUT ::DeeObject *right) const { if (::DeeList_InplaceMerge(this->ob_ptr,right) == -1) detail::throw_last_error(); return *this; }
  inline list const &operator &= (DEE_A_INOUT ::DeeObject *right) const { if (::DeeList_InplaceCommon(this->ob_ptr,right) == -1) detail::throw_last_error(); return *this; }
  inline list const &operator *= (DEE_A_IN Dee_size_t n) const { if (::DeeList_Repeat(this->ob_ptr,n) == -1) detail::throw_last_error(); return *this; }
 public: /* public methods */
  inline void clear() const DEE_CXX11_NOEXCEPT { ::DeeList_Clear(this->ob_ptr); }
  inline DEE_A_RET_WUNUSED object pop(DEE_A_IN Dee_size_t i) const { return object(::DeeList_Pop(this->ob_ptr,i),detail::tag_ref_or_err()); }
  inline void erase(DEE_A_IN Dee_size_t i) const { if (::DeeList_Erase(this->ob_ptr,i,1) == -1) detail::throw_last_error(); }
  inline void erase(DEE_A_IN Dee_size_t i, DEE_A_IN Dee_size_t n) const { if (::DeeList_Erase(this->ob_ptr,i,n) == -1) detail::throw_last_error(); }
  inline void append(DEE_A_INOUT ::DeeObject *elem) const { if (::DeeList_Append(this->ob_ptr,elem) == -1) detail::throw_last_error(); }
  inline void sorted_insert(DEE_A_INOUT ::DeeObject *elem) const { if (::DeeList_SortedInsert(this->ob_ptr,elem) == -1) detail::throw_last_error(); }
  inline void sorted_insert(DEE_A_INOUT ::DeeObject *elem, DEE_A_INOUT ::DeeObject *pred) const { if (::DeeList_SortedInsertPred(this->ob_ptr,elem,pred) == -1) detail::throw_last_error(); }
  inline void insert(DEE_A_IN Dee_size_t i, DEE_A_INOUT ::DeeObject *elem) const { if (::DeeList_Insert(this->ob_ptr,i,elem) == -1) detail::throw_last_error(); }
  inline void insert(DEE_A_IN Dee_size_t i, DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p) const { if (::DeeList_InsertVector(this->ob_ptr,i,n,p) == -1) detail::throw_last_error(); }
  inline void insert(DEE_A_IN Dee_size_t i, DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) object const *p) const { if (::DeeList_InsertVector(this->ob_ptr,i,n,reinterpret_cast< ::DeeObject *const *>(p)) == -1) detail::throw_last_error(); }
  inline void insert_iter(DEE_A_IN Dee_size_t i, DEE_A_INOUT ::DeeObject *ob) const { if (::DeeList_InsertIterator(this->ob_ptr,i,ob) == -1) detail::throw_last_error(); }
  inline void insert_list(DEE_A_IN Dee_size_t i, DEE_A_INOUT ::DeeObject *seq) const { if (::DeeList_InsertSequence(this->ob_ptr,i,seq) == -1) detail::throw_last_error(); }
  inline void extend(DEE_A_INOUT ::DeeObject *seq) const { if (::DeeList_AppendSequence(this->ob_ptr,seq) == -1) detail::throw_last_error(); }
  inline void extend(DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p) const { if (::DeeList_AppendVector(this->ob_ptr,n,p) == -1) detail::throw_last_error(); }
  inline void extend(DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) object const *p) const { if (::DeeList_AppendVector(this->ob_ptr,n,reinterpret_cast< ::DeeObject *const *>(p)) == -1) detail::throw_last_error(); }
  inline void extend_iter(DEE_A_INOUT ::DeeObject *ob) const { if (::DeeList_AppendIterator(this->ob_ptr,ob) == -1) detail::throw_last_error(); }
  inline void reserve(DEE_A_IN Dee_size_t n) const { if (::DeeList_Reserve(this->ob_ptr,n) == -1) detail::throw_last_error(); }
  inline void reverse() const DEE_CXX11_NOEXCEPT { ::DeeList_Reverse(this->ob_ptr); }
  inline void shrink_to_fit() const DEE_CXX11_NOEXCEPT { ::DeeList_ShrinkToFit(this->ob_ptr); }
  inline void resize(DEE_A_IN Dee_size_t new_size) const { if (::DeeList_Resize(this->ob_ptr,new_size,Dee_None) == -1) detail::throw_last_error(); }
  inline void resize(DEE_A_IN Dee_size_t new_size, DEE_A_INOUT ::DeeObject *filler) const { if (::DeeList_Resize(this->ob_ptr,new_size,filler) == -1) detail::throw_last_error(); }
  inline void fill(DEE_A_INOUT ::DeeObject *filler) const DEE_CXX11_NOEXCEPT { ::DeeList_Fill(this->ob_ptr,filler); }
  inline void fill(DEE_A_INOUT ::DeeObject *filler, DEE_A_IN Dee_size_t n) const { if (::DeeList_FillAssign(this->ob_ptr,filler,n) == -1) detail::throw_last_error(); }
  inline void sort() const { if (::DeeList_Sort(this->ob_ptr) == -1) detail::throw_last_error(); }
  inline void sort(DEE_A_INOUT ::DeeObject *pred) const { if (::DeeList_SortPred(this->ob_ptr,pred) == -1) detail::throw_last_error(); }
  inline DEE_A_RET_WUNUSED list sorted() const { return list(::DeeList_Sorted(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED list sorted(DEE_A_INOUT ::DeeObject *pred) const { return list(::DeeList_SortedPred(this->ob_ptr,pred),detail::tag_ref_or_err()); }
  inline void push_front(DEE_A_INOUT ::DeeObject *ob) const { if (::DeeList_Insert(this->ob_ptr,0,ob) == -1) detail::throw_last_error(); }
  inline void push_back(DEE_A_INOUT ::DeeObject *ob) const { if (::DeeList_Append(this->ob_ptr,ob) == -1) detail::throw_last_error(); }
  inline object pop_front() const { return object(::DeeList_Pop(this->ob_ptr,0),detail::tag_ref_or_err()); }
  inline object pop_back() const { return object(::DeeList_Pop(this->ob_ptr,(Dee_size_t)-1),detail::tag_ref_or_err()); }
  inline Dee_size_t remove_if(DEE_A_INOUT ::DeeObject *pred) const { if (::DeeList_RemoveIf(this->ob_ptr,pred) == -1) detail::throw_last_error(); }
  inline DEE_A_RET_WUNUSED bool contains(DEE_A_INOUT ::DeeObject *ob) const { int result; if ((result = ::DeeList_Contains(this->ob_ptr,ob)) == -1) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED bool contains(DEE_A_INOUT ::DeeObject *ob, DEE_A_INOUT ::DeeObject *pred) const { int result; if ((result = ::DeeList_ContainsPred(this->ob_ptr,ob,pred)) == -1) detail::throw_last_error(); return !!result; }
  inline DEE_A_RET_WUNUSED ::Dee_ssize_t find(DEE_A_INOUT ::DeeObject *ob) const { ::Dee_ssize_t result; if ((result = ::DeeList_Find(this->ob_ptr,ob)) == -2) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_INOUT ::DeeObject *ob) const { Dee_size_t result; if ((result = ::DeeList_Index(this->ob_ptr,ob)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_INOUT ::DeeObject *ob) const { Dee_size_t result; if ((result = ::DeeList_Count(this->ob_ptr,ob)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED list concat(DEE_A_INOUT ::DeeObject *right) const { return list(::DeeList_Concat(this->ob_ptr,right),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object locate(DEE_A_INOUT ::DeeObject *pred) const { return object(::DeeList_Locate(this->ob_ptr,pred),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED object rlocate(DEE_A_INOUT ::DeeObject *pred) const { return object(::DeeList_RLocate(this->ob_ptr,pred),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED list locate_all(DEE_A_INOUT ::DeeObject *pred) const { return list(::DeeList_LocateAll(this->ob_ptr,pred),detail::tag_ref_or_err()); }
#ifdef DEE_LIMITED_API
  inline DEE_A_RET_WUNUSED list copy() const { return list(::DeeList_Copy(this->ob_ptr),detail::tag_ref_or_err()); }
#endif
  inline DEE_A_RET_WUNUSED list deep_copy() const { return list(::DeeList_DeepCopy(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED list segments(DEE_A_IN Dee_size_t n) const { return list(::DeeList_Segments(this->ob_ptr,n),detail::tag_ref_or_err()); }
 public: /* public constructor / destructor */
  inline list() DEE_CXX11_NOEXCEPT: object(::DeeList_NewEmpty(),detail::tag_ref_or_err()) {}
  inline list(object const &ob): object(::DeeList_Cast(ob),detail::tag_ref_or_err()) {}
  inline list(DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) object const *elem): object(::DeeList_NewFromVector(n,reinterpret_cast< ::DeeObject *const*>(elem)),detail::tag_ref_or_err()) {}
  inline list(DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *elem): object(::DeeList_NewFromVector(n,elem),detail::tag_ref_or_err()) {}
  DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(list,object,DeeList_Check)
};

#ifdef GUARD_DEEMON_CXX_TUPLE_H
inline DEE_A_RET_WUNUSED tuple list::as_tuple() const { return tuple(::DeeList_AsTuple(this->ob_ptr),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list tuple::as_list() const { return list(::DeeTuple_AsList(this->ob_ptr),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list tuple::segments(DEE_A_IN Dee_size_t n) const { return list(::DeeTuple_Segments(this->ob_ptr,n),detail::tag_ref_or_err()); }
#endif
#ifdef GUARD_DEEMON_CXX_STRING_H
#if DEE_CONFIG_HAVE_ENCODING_UTF8
inline DEE_A_RET_WUNUSED list string::utf8::rsplit(DEE_A_IN string::utf8 const &sep) const { return list(::DeeUtf8String_RSplit(this->ob_ptr,sep),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list string::utf8::split(DEE_A_IN string::utf8 const &sep) const { return list(::DeeUtf8String_Split(this->ob_ptr,sep),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list string::utf8::splitlines() const { return list(::DeeUtf8String_SplitLines(this->ob_ptr,0),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list string::utf8::splitlines(DEE_A_IN bool keepends) const { return list(::DeeUtf8String_SplitLines(this->ob_ptr,(int)keepends),detail::tag_ref_or_err()); }
#endif
#if DEE_CONFIG_HAVE_ENCODING_WIDE
inline DEE_A_RET_WUNUSED list string::wide::rsplit(DEE_A_IN string::wide const &sep) const { return list(::DeeWideString_RSplit(this->ob_ptr,sep),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list string::wide::split(DEE_A_IN string::wide const &sep) const { return list(::DeeWideString_Split(this->ob_ptr,sep),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list string::wide::splitlines() const { return list(::DeeWideString_SplitLines(this->ob_ptr,0),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list string::wide::splitlines(DEE_A_IN bool keepends) const { return list(::DeeWideString_SplitLines(this->ob_ptr,(int)keepends),detail::tag_ref_or_err()); }
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
inline DEE_A_RET_WUNUSED list string::utf16::rsplit(DEE_A_IN string::utf16 const &sep) const { return list(::DeeUtf16String_RSplit(this->ob_ptr,sep),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list string::utf16::split(DEE_A_IN string::utf16 const &sep) const { return list(::DeeUtf16String_Split(this->ob_ptr,sep),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list string::utf16::splitlines() const { return list(::DeeUtf16String_SplitLines(this->ob_ptr,0),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list string::utf16::splitlines(DEE_A_IN bool keepends) const { return list(::DeeUtf16String_SplitLines(this->ob_ptr,(int)keepends),detail::tag_ref_or_err()); }
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
inline DEE_A_RET_WUNUSED list string::utf32::rsplit(DEE_A_IN string::utf32 const &sep) const { return list(::DeeUtf32String_RSplit(this->ob_ptr,sep),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list string::utf32::split(DEE_A_IN string::utf32 const &sep) const { return list(::DeeUtf32String_Split(this->ob_ptr,sep),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list string::utf32::splitlines() const { return list(::DeeUtf32String_SplitLines(this->ob_ptr,0),detail::tag_ref_or_err()); }
inline DEE_A_RET_WUNUSED list string::utf32::splitlines(DEE_A_IN bool keepends) const { return list(::DeeUtf32String_SplitLines(this->ob_ptr,(int)keepends),detail::tag_ref_or_err()); }
#endif
#endif

}
DEE_DECL_CXX_END
#endif

#endif /* !GUARD_DEEMON_CXX_LIST_H */
