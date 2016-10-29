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
#ifndef GUARD_DEEMON_SET_H
#define GUARD_DEEMON_SET_H 1

#include <deemon/__conf.inl>

#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#include <deemon/optional/hashset.h>
#elif DEE_CONFIG_API_HAVE_HASHSET_EXPORTS
#include <deemon/optional/hashset.h>
#endif


DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Set
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

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
#ifdef DEE_PRIVATE_DECL_DEE_HASH_T
DEE_PRIVATE_DECL_DEE_HASH_T
#undef DEE_PRIVATE_DECL_DEE_HASH_T
#endif


DEE_OBJECT_DEF(DeeSetObject);
DEE_OBJECT_DEF(DeeSetIteratorObject);

#ifdef DEE_LIMITED_API
struct DeeSetObject {
 DEE_OBJECT_HEAD
 struct DeeAtomicMutex s_lock; /*< Synchronization. */
 struct DeeHashSet     s_set;  /*< Hash-set. */
};
#define DeeSet_SET(ob)     &((DeeSetObject *)Dee_REQUIRES_POINTER(ob))->s_set
#define DeeSet_LOCK(ob)    &((DeeSetObject *)Dee_REQUIRES_POINTER(ob))->s_lock
#define DeeSet_ACQUIRE(ob)    DeeAtomicMutex_Acquire(DeeSet_LOCK(ob))
#define DeeSet_RELEASE(ob)    DeeAtomicMutex_Release(DeeSet_LOCK(ob))
#define DeeSet_SIZE(ob)                  DeeHashSet_SIZE(DeeSet_SET(ob))
#define DeeSet_BUCKET_COUNT(ob)          DeeHashSet_BUCKET_COUNT(DeeSet_SET(ob))
#define DeeSet_BUCKET_SIZE(ob,bucket)    DeeHashSet_BUCKET_SIZE(DeeSet_SET(ob),bucket)
#define DeeSet_GET_MAX_LOAD_FACTOR(ob)   DeeHashSet_GET_MAX_LOAD_FACTOR(DeeSet_SET(ob))
#define DeeSet_SET_MAX_LOAD_FACTOR(ob,v) DeeHashSet_SET_MAX_LOAD_FACTOR(DeeSet_SET(ob),v)
// v thread-safe and OK to do. (s.a.: explanation for 'DeeCell_EMPTY')
#define DeeSet_EMPTY(ob)                (DeeHashSet_SIZE(DeeSet_SET(ob))==0)
#else
#define DeeSet_SIZE(ob)                  DeeSet_Size((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeSet_BUCKET_COUNT(ob)          DeeSet_BucketCount((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeSet_BUCKET_SIZE(ob,bucket)    DeeSet_BucketSize((DeeObject *)Dee_REQUIRES_POINTER(ob),bucket)
#define DeeSet_GET_MAX_LOAD_FACTOR(ob)   DeeSet_GetMaxLoadFactor((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeSet_SET_MAX_LOAD_FACTOR(ob,v) DeeSet_SetMaxLoadFactor((DeeObject *)Dee_REQUIRES_POINTER(ob),v)
#define DeeSet_EMPTY(ob)                (DeeSet_SIZE(ob)==0)
#endif



#ifdef DEE_LIMITED_API
struct DeeSetIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF struct DeeSetObject *si_set;  /*< [1..1] Set being iterated. */
 struct DeeHashSetIterator      si_iter; /*< Iterator data structure. */
};
#define DeeSetIterator_SET(ob)  ((DeeObject *)((DeeSetIteratorObject *)Dee_REQUIRES_POINTER(ob))->si_set)
#define DeeSetIterator_ITER(ob) (&((DeeSetIteratorObject *)Dee_REQUIRES_POINTER(ob))->si_iter)
#endif

DEE_DATA_DECL(DeeTypeObject) DeeSet_Type;
DEE_DATA_DECL(DeeTypeObject) DeeSetIterator_Type;
#define DeeSet_Check(ob)      DeeObject_InstanceOf(ob,&DeeSet_Type)
#define DeeSet_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeSet_Type)
#define DeeSetIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeSetIterator_Type)
#define DeeSetIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeSetIterator_Type)

//////////////////////////////////////////////////////////////////////////
// Allocate a new set object
#define /* DEE_A_EXEC DeeSetObject * */DeeSet_NewEmpty DeeSet_New
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeSetObject) *) DeeSet_New(void);
#define /* DEE_A_EXEC DeeSetObject * */DeeSet_NewFromSequence(sequence) DeeSet_TNewFromSequence(Dee_TYPE(sequence),sequence)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeSetObject) *) DeeSet_TNewFromSequence(DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeSetObject) *) DeeSet_NewFromIterator(DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeSetObject) *) DeeSet_Copy(DEE_A_IN_OBJECT(DeeSetObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeSetObject) *) DeeSet_DeepCopy(DEE_A_IN_OBJECT(DeeSetObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#define /* DEE_A_EXEC DeeSetObject * */DeeSet_Cast(ob) DeeSet_TCast(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeSetObject) *) DeeSet_TCast(DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Create a new set from a key vector
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeSetObject) *) DeeSet_NewFromKeys(
 DEE_A_IN Dee_size_t keyc, DEE_A_IN_R(keyc) DeeObject *const *keyv);

//////////////////////////////////////////////////////////////////////////
// Inserts a sequence of elements into a given set.
//  -> Sequences and iterators are converted same as by 'DeeSet_NewFromSequence' / 'DeeSet_NewFromIterator'
//  -> In usercode, 'DeeSet_InsertSequence' is available through 'set.insert_all'
#define /* DEE_A_EXEC int */DeeSet_InsertSequence(ob,sequence) DeeSet_TInsertSequence(ob,Dee_TYPE(sequence),sequence)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSet_TInsertSequence(DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSet_InsertIterator(DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Arguably the most importable methods of any set object...
// - 'DeeSet_Insert' will fail and return 1, if 'ob' is already part of the set
// - 'DeeSet_Contains' returns positive, non-zero if 'ob' is contained
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeSet_Insert(DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeSet_InsertWithKnownBucket(DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *ob, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeSet_Remove(DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeSet_RemoveWithKnownBucket(DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *ob, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeSet_Contains(DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeSet_ContainsWithKnownBucket(DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *ob, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeSet_ContainsPred(DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *ob, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeSet_ContainsPredWithKnownBucket(DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *ob, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,4));

#if DEE_DEPRECATED_API_VERSION(101,102,104)
#define DeeSet_InsertAllFromSeq      DeeSet_InsertSequence
#define DeeSet_InsertAllFromIterator DeeSet_InsertIterator
#endif

//////////////////////////////////////////////////////////////////////////
// Pops a key from 'self' and returns it
// -> Throws an 'Error.ValueError' if the set is empty
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSet_PopSomething(
 DEE_A_INOUT_OBJECT(DeeSetObject) *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns the key of a given index
// -> DeeSet_GetKeyIndex:   throws an Causes Error.ValueError if the set is empty
// -> DeeSet_GetKeyIndexAt: throws an Causes Error.ValueError if index > DeeSet_SIZE(self)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSet_GetKeyIndex(
 DEE_A_IN_OBJECT(DeeSetObject) const *self, DEE_A_IN Dee_ssize_t index) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSet_GetKeyIndexAt(
 DEE_A_IN_OBJECT(DeeSetObject) const *self, DEE_A_IN Dee_size_t index) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSet_GetKeyRangeIndex(
 DEE_A_IN_OBJECT(DeeSetObject) const *self, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Searches for 'key' and returns its index
// >> Returns -1 if the key wasn't found (no error set)
// >> Returns -2 if an error occurred
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeSet_FindKeyIndex(
 DEE_A_IN_OBJECT(DeeSetObject) const *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeSet_FindKeyIndexWithKnownBucket(
 DEE_A_IN_OBJECT(DeeSetObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Traverse a set
// >> If the "trav" function returns a non-zero value, the function stops and returns that value
// >> Otherwise the function will call "trav" for all its keys and return 0
// NOTE: 'DeeSet_Traverse' never throws an exception itself
typedef DEE_A_RET_EXCEPT(-1) int (*DeeSetTraverseFunc)(
 DEE_A_INOUT DeeObject *key, DEE_A_IN_OPT void *closure);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeSet_Traverse(
 DEE_A_IN_OBJECT(DeeSetObject) const *self, DEE_A_IN DeeSetTraverseFunc trav,
 DEE_A_IN_OPT void *closure) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Set functionality
DEE_FUNC_DECL(DEE_A_EXEC void) DeeSet_Clear(DEE_A_INOUT_OBJECT(DeeSetObject) *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeSet_Size(DEE_A_IN_OBJECT(DeeSetObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeSet_BucketSize(DEE_A_IN_OBJECT(DeeSetObject) const *self, DEE_A_IN Dee_size_t bucket) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeSet_BucketCount(DEE_A_IN_OBJECT(DeeSetObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// max_load_factor access
DEE_FUNC_DECL(DEE_A_RET_WUNUSED double) DeeSet_GetMaxLoadFactor(DEE_A_IN_OBJECT(DeeSetObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeSet_SetMaxLoadFactor(DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_IN double v) DEE_ATTRIBUTE_NONNULL((1));

#ifdef DEE_LIMITED_API
// Iterator functions
extern DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeSetIteratorObject) *DeeSet_IterSelf(
 DEE_A_INOUT_OBJECT(DeeSetObject) *self) DEE_ATTRIBUTE_NONNULL((1));
#endif



DEE_DECL_END

#endif /* !GUARD_DEEMON_SET_H */
