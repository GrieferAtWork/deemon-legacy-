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
#ifndef GUARD_DEEMON_LIST_H
#define GUARD_DEEMON_LIST_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/object_sequence.h>
#ifdef DEE_LIMITED_API
#include <deemon/optional/atomic_mutex.h>
#include <deemon/object.h>
#endif

#if DEE_DEPRECATED_API_VERSION(100,101,102)
#include <deemon/object.h> // Required for deprecated functions
#include <deemon/type.h>   // Required for deprecated functions
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif

DEE_OBJECT_DEF(DeeListObject);
DEE_OBJECT_DEF(DeeListIteratorObject);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// List (A mutable dynamic-vector)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifdef DEE_LIMITED_API
struct DeeListObject {
 DEE_OBJECT_HEAD
 struct DeeAtomicMutex l_lock;  /*< Lock for this list. */
           Dee_size_t  l_len;   /*< Amount of elements. */
           Dee_size_t  l_alloc; /*< Allocated amount of elements. */
 DEE_A_REF DeeObject **l_elem;  /*< [1..1][l_len] Elements. */
};
#define DeeList_INIT() \
 {DEE_OBJECT_HEAD_INIT(&DeeList_Type),\
  DeeAtomicMutex_INIT(),0,0,NULL}
#define _DeeList_InitCommon(ob) \
 DeeAtomicMutex_Init(DeeList_LOCK(ob))
#define DeeList_ACQUIRE(ob)          DeeAtomicMutex_AcquireRelaxed(DeeList_LOCK(ob))
#define DeeList_RELEASE(ob)          DeeAtomicMutex_Release(DeeList_LOCK(ob))
#define DeeList_LOCK(ob)         (&((DeeListObject*)Dee_REQUIRES_POINTER(ob))->l_lock)
#define DeeList_SIZE(ob)           ((DeeListObject*)Dee_REQUIRES_POINTER(ob))->l_len
#define DeeList_ALLOC(ob)          ((DeeListObject*)Dee_REQUIRES_POINTER(ob))->l_alloc
#define DeeList_ELEM(ob)           ((DeeListObject*)Dee_REQUIRES_POINTER(ob))->l_elem
#define DeeList_GET(ob,i)          ((DeeListObject*)Dee_REQUIRES_POINTER(ob))->l_elem[i]
#define DeeList_SET(ob,i,v) (void)(((DeeListObject*)Dee_REQUIRES_POINTER(ob))->l_elem[i]=(v))
// v thread-safe and OK to do. (s.a.: explanation for 'DeeCell_EMPTY')
#define DeeList_EMPTY(ob)         (((DeeListObject*)Dee_REQUIRES_POINTER(ob))->l_len==0)
#else
#define DeeList_SIZE(ob)             DeeList_Size((DeeObject*)Dee_REQUIRES_POINTER(ob))
#define DeeList_ALLOC(ob)            DeeList_Alloc((DeeObject*)Dee_REQUIRES_POINTER(ob))
#define DeeList_EMPTY(ob)           (DeeList_SIZE(ob)==0)
#endif

DEE_DATA_DECL(DeeTypeObject) DeeList_Type;
#define DeeList_Check(ob)      DeeObject_InstanceOf(ob,&DeeList_Type)
#define DeeList_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeList_Type)
#define DeeReturn_EmptyList    return DeeList_New()
#define DeeList_NewEmpty       DeeList_New

#ifdef DEE_LIMITED_API
struct DeeListIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeListObject *li_list; /*< [1..1] List that is being iterated. */
 Dee_size_t               li_pos;  /*< Iteration position (protected by the list lock). */
};
#define DeeListIterator_LIST(ob)  ((DeeListIteratorObject *)Dee_REQUIRES_POINTER(ob))->li_list
#define DeeListIterator_POS(ob)   ((DeeListIteratorObject *)Dee_REQUIRES_POINTER(ob))->li_pos
#endif
DEE_DATA_DECL(DeeTypeObject) DeeListIterator_Type;
#define DeeListIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeListIterator_Type)
#define DeeListIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeListIterator_Type)
struct DeeTupleObject;


#ifdef DEE_LIMITED_API
//////////////////////////////////////////////////////////////////////////
// Unsafe List API
// >> Careful with these functions, as they break the guaranty of list elements never being NULL.
//    They are only meant for custom list initializers where the size of the list is already known.
extern DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *_DeeList_NewUnsafe(DEE_A_IN Dee_size_t size);
extern DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeListObject) *_DeeList_TryWeakNewUnsafe(DEE_A_IN Dee_size_t size);
extern DEE_A_RET_EXCEPT(-1) int _DeeList_ResizeUnsafe(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t new_size) DEE_ATTRIBUTE_NONNULL((1));
#define _DeeList_FreeUnsafe(ob)\
do{\
 DEE_ASSERT(DeeObject_Check(ob) && DeeList_Check(ob));\
 free(((DeeListObject *)(ob))->l_elem);\
 DeeGC_TrackedRem(ob);\
 _DeeObject_DELETE(&DeeList_Type,ob);\
}while(0)
#endif

//////////////////////////////////////////////////////////////////////////
// Public list API
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_New(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_NewFromVector(DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p);
#define /* DEE_A_EXEC DeeListObject * */DeeList_NewFromSequence(sequence) DeeList_TNewFromSequence(Dee_TYPE(sequence),sequence)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_TNewFromSequence(DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_NewFromIterator(DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_NewWithSizeHint(DEE_A_IN Dee_size_t hint);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_CopyAssign(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN_OBJECT(DeeListObject) const *right) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC void) DeeList_MoveAssign(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT_OBJECT(DeeListObject) *right) DEE_ATTRIBUTE_NONNULL((1,2));
#define /* DEE_A_EXEC int */                       DeeList_AssignSequence(ob,sequence) DeeList_TAssignSequence(ob,Dee_TYPE(sequence),sequence)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_TAssignSequence(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_AssignIterator(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_AssignVector(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_DeepCopy(DEE_A_IN_OBJECT(DeeListObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeList_Back(DEE_A_IN_OBJECT(DeeListObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeList_Front(DEE_A_IN_OBJECT(DeeListObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeList_Get(DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_IN Dee_ssize_t i) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeList_At(DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_IN Dee_size_t i) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_Set(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_ssize_t i, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_SetAt(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t i, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_Del(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_ssize_t i) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_DelAt(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t i) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(           DEE_A_RET_WUNUSED Dee_size_t) DeeList_Size(DEE_A_IN_OBJECT(DeeListObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(           DEE_A_RET_WUNUSED Dee_size_t) DeeList_Alloc(DEE_A_IN_OBJECT(DeeListObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *) DeeList_AsTuple(DEE_A_IN_OBJECT(DeeListObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_GetRange(DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) DEE_ATTRIBUTE_NONNULL((1));
#define /* DEE_A_EXEC int */DeeList_SetRange(ob,begin,end,v) DeeList_TSetRange(ob,begin,end,Dee_TYPE(v),v)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_TSetRange(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end, DEE_A_IN DeeTypeObject const *tp_v, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,4,5));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_SetRangeVector(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end, DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_SetRangeIterator(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,4));

#if DEE_DEPRECATED_API_VERSION(101,102,104)
#define DeeList_SetRangeIteartor  DeeList_SetRangeIterator
#endif

#if DEE_DEPRECATED_API_VERSION(100,101,103)
DEE_STATIC_INLINE(DEE_A_EXEC DEE_ATTRIBUTE_NONNULL((1))
 DEE_ATTRIBUTE_DEPRECATED("'DeeList_Copy()' is limited. Use 'DeeObject_Copy' instead")
 DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_Copy(
 DEE_A_IN_OBJECT(DeeListObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 return DeeObject_Copy((DeeObject *)self);
}
#elif defined(DEE_LIMITED_API)
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *
 DeeList_Copy(DEE_A_IN_OBJECT(DeeListObject) const *self);
#endif


#ifdef DEE_LIMITED_API
extern DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListIteratorObject) *DeeList_IterSelf(
 DEE_A_INOUT_OBJECT(DeeListObject) *self) DEE_ATTRIBUTE_NONNULL((1));
#endif

#if DEE_DEPRECATED_API_VERSION(100,101,102)
#ifdef DEE_PRIVATE_DECL_DEE_HASH_T
DEE_PRIVATE_DECL_DEE_HASH_T
#undef DEE_PRIVATE_DECL_DEE_HASH_T
#endif
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1))
 DEE_ATTRIBUTE_DEPRECATED("'DeeList_Hash()' isn't exception safe. Use 'DeeObject_HashEx' instead")
 DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_hash_t) DeeList_Hash(
 DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_hash_t start) {
 Dee_hash_t result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 if (DeeObject_HashEx(self,start,&result) != 0)
  return (Dee_hash_t)-1; // Hash might naturally return -1; Error may have occurred before
 return result;
}
#else
#ifdef DEE_LIMITED_API
#ifdef DEE_PRIVATE_DECL_DEE_HASH_T
DEE_PRIVATE_DECL_DEE_HASH_T
#undef DEE_PRIVATE_DECL_DEE_HASH_T
#endif
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeList_Hash(
 DEE_A_IN_OBJECT(DeeListObject) const *self,
 DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result) DEE_ATTRIBUTE_NONNULL((1,3));
#endif
#endif


DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_Insert(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t i, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,3));
#define /* DEE_A_EXEC int */DeeList_InsertSequence(ob,i,sequence) DeeList_TInsertSequence(ob,i,Dee_TYPE(sequence),sequence)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_TInsertSequence(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t i, DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_InsertVector(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t i, DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_InsertIterator(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t i, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_Append(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
#define /* DEE_A_EXEC int */DeeList_AppendSequence(ob,sequence) DeeList_TAppendSequence(ob,Dee_TYPE(sequence),sequence)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_TAppendSequence(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_AppendVector(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_AppendIterator(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,2));

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_Repeat(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t n) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_Repeated(DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_IN Dee_size_t n) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_Erase(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t i, DEE_A_IN Dee_size_t n) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_NOEXCEPT_REF DeeObject *) DeeList_Pop(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t i) DEE_ATTRIBUTE_NONNULL((1)); // Returns removed object
DEE_FUNC_DECL(DEE_A_EXEC void) DeeList_Clear(DEE_A_INOUT_OBJECT(DeeListObject) *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC void) DeeList_ClearAndShrink(DEE_A_INOUT_OBJECT(DeeListObject) *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_Reserve(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t reserved_size) DEE_ATTRIBUTE_NONNULL((1)); // Reserves memory for at least reserved_size elements
DEE_FUNC_DECL(DEE_A_EXEC void) DeeList_Reverse(DEE_A_INOUT_OBJECT(DeeListObject) *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_Reversed(DEE_A_IN_OBJECT(DeeListObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC void) DeeList_Fill(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *filler) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_FillAssign(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *filler, DEE_A_IN Dee_size_t size) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_Resize(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t new_size, DEE_A_INOUT DeeObject *filler) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_FUNC_DECL(DEE_A_EXEC void) DeeList_ShrinkToFit(DEE_A_INOUT_OBJECT(DeeListObject) *self) DEE_ATTRIBUTE_NONNULL((1));
#define /* DEE_A_EXEC DeeListObject * */DeeList_Concat(lhs,rhs) DeeList_TConcat(lhs,Dee_TYPE(rhs),rhs)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_TConcat(
 DEE_A_IN_OBJECT(DeeListObject) const *lhs, DEE_A_IN DeeTypeObject const *tp_rhs,
 DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
#define /* DEE_A_EXEC int */DeeList_InplaceConcat  DeeList_AppendSequence
#define /* DEE_A_EXEC int */DeeList_TInplaceConcat DeeList_TAppendSequence
#define /* DEE_A_EXEC DeeListObject * */DeeList_Cast(ob) DeeList_TCast(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_TCast(DEE_A_IN DeeTypeObject const *tp_seq, DEE_A_INOUT DeeObject *seq) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Sorts the elements of a given list 'self' using a merge-sort algorithm ('cause dose are da best!)
// You can use 'DeeList_SortPred' to specify a predicate '[](a,b) -> a < b'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_Sort(DEE_A_INOUT_OBJECT(DeeListObject) *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_SortPred(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *pred_lo) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_Sorted(DEE_A_IN_OBJECT(DeeListObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_SortedPred(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *pred_lo) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Removes all consecutive elements that are equivalent to each other.
// You can use 'DeeList_UniquePred' to specify a predicate '[](a,b) -> a == b'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_Unique(DEE_A_INOUT_OBJECT(DeeListObject) *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_UniquePred(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *pred_eq) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_ToUnique(DEE_A_IN_OBJECT(DeeListObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_ToUniquePred(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *pred_eq) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Appends elements from an iterator, skipping those equivalent to the previously element.
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_AppendUniqueIterator(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_AppendUniqueIteratorPred(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *iterator, DEE_A_INOUT DeeObject *pred_eq) DEE_ATTRIBUTE_NONNULL((1,2,3));
#if DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS
#define DeeList_AppendUniqueSequence(ob,sequence)             DEE_GCC_EXTENSION({int _temp; DeeObject *const _iterator = DeeObject_IterSelf(sequence); if (_iterator) { _temp = DeeList_AppendUniqueIterator    (ob,sequence);         Dee_DECREF(sequence); } else _temp = -1; _temp; })
#define DeeList_AppendUniqueSequencePred(ob,sequence,pred_eq) DEE_GCC_EXTENSION({int _temp; DeeObject *const _iterator = DeeObject_IterSelf(sequence); if (_iterator) { _temp = DeeList_AppendUniqueIteratorPred(ob,sequence,pred_eq); Dee_DECREF(sequence); } else _temp = -1; _temp; })
#else /* DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_AppendUniqueSequence(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *sequence) { int temp; if ((sequence = DeeObject_IterSelf(sequence)) == NULL) return -1; temp = DeeList_AppendUniqueIterator(self,sequence); Dee_DECREF(sequence); return temp; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_AppendUniqueSequencePred(DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *sequence, DEE_A_INOUT DeeObject *pred_eq) { int temp; if ((sequence = DeeObject_IterSelf(sequence)) == NULL) return -1; temp = DeeList_AppendUniqueIteratorPred(self,sequence,pred_eq); Dee_DECREF(sequence); return temp; }
#endif /* !DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */

DEE_FUNC_DECL(DEE_A_EXEC_IF((*func)(...,closure)) DEE_A_RET_EXCEPT(-1) int) DeeList_Traverse(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_IN DeeObjectIterateFunc func,
 DEE_A_IN_OPT void *closure) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Inserts 'elem' into 'self', keeping an already sorted list still sorted.
// You can use 'DeeList_SortedInsertPred' to specify a predicate '[](a,b) -> a < b'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_SortedInsert(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_SortedInsertPred(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));

//////////////////////////////////////////////////////////////////////////
// Merge a any iterable object with a list
//  - If both lists are sorted, the returned list will be sorted as well.
//  - Available from user code as the '|' operator for lists
//  - examples:
//   >> [1,4,6,7,9] | [2,3,5,8] == [1,2,3,4,5,6,7,8,9]
//   >> [4,1,7,3]   | [2,1,4,6] == [2,1,4,1,4,6,7,3]
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_Merge(
 DEE_A_INOUT_OBJECT(DeeListObject) *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_InplaceMerge(
 DEE_A_INOUT_OBJECT(DeeListObject) *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Returns a list containing only objects included in both lists
//  - Available from user code as the '&' operator for lists
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_Common(
 DEE_A_INOUT_OBJECT(DeeListObject) *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeList_InplaceCommon(
 DEE_A_INOUT_OBJECT(DeeListObject) *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2));

// v returns 1 if found; 0 if not found; -1 on error
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeList_Contains(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeList_ContainsPred(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));

// Returns the index of "elem" (return -2 on error / -1 if not found)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeList_Find(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeList_RFind(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeList_FindPred(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *elem,
 DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeList_RFindPred(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *elem,
 DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));

// Returns the index of "elem" (return (Dee_size_t)-1 and raises an error if not found)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeList_Index(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeList_RIndex(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeList_IndexPred(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *elem,
 DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeList_RIndexPred(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *elem,
 DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));

// Returns (Dee_size_t)-1 if an error occurred
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeList_Count(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeList_CountPred(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *elem,
 DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));

//////////////////////////////////////////////////////////////////////////
// Locate the first element for which 'pred(elem)' is true
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeList_Locate(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeList_RLocate(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_LocateAll(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Returns a new list with all elements of 'self' transformed with 'pred(elem)'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_Transform(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Combination of DeeList_Find+DeeList_Erase
// >> Returns -1: Error
// >> Returns  0: Element removed
// >> Returns  1: Element not found
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeList_Remove(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Removes all elements for which 'pred(elem)' is true
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeList_RemoveIf(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeList_Segments(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_IN Dee_size_t n) DEE_ATTRIBUTE_NONNULL((1));

DEE_DECL_END

#endif /* !GUARD_DEEMON_LIST_H */
