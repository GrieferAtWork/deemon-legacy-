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
#ifndef GUARD_DEEMON_DICT_H
#define GUARD_DEEMON_DICT_H 1

#include <deemon/__conf.inl>
#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#include <deemon/optional/atomic_mutex.h>
#include <deemon/optional/hashmap.h>
#elif DEE_CONFIG_API_HAVE_HASHMAP_EXPORTS
#include <deemon/optional/hashmap.h>
#endif
#if DEE_DEPRECATED_API_VERSION(100,101,103)
#include <deemon/object.h> // Required for deprecation-replacement functions
#include <deemon/type.h>   // Required for deprecation-replacement functions
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Dict
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

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
#ifdef DEE_PRIVATE_DECL_DEE_HASH_T
DEE_PRIVATE_DECL_DEE_HASH_T
#undef DEE_PRIVATE_DECL_DEE_HASH_T
#endif

//////////////////////////////////////////////////////////////////////////
// Iterator behavior
//  - dict.iterator        --> iterator<tuple<key,item>>
//  - dict.keys.iterator   --> iterator<key>
//  - dict.items.iterator  --> iterator<item>
//  - dict.values.iterator --> iterator<tuple<key,item>> (same as dict.iterator)
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Usage rules
// - 'key' can be anything that supports 'tp_hash' and 'tp_cmp_eq'
// - 'item' can be anything (no exceptions)
// - For convenience, functions that use strings for keys are provided.
//   Please note that those function behave the same as if the regular
//   counterpart would have been called with the 'DeeString_New'
//   version of the key.
// - 'bucket' is the hashed value of the key when DEE_HASH_START is
//   used as second parameter in 'DeeObject_HashEx' / 'DeeHash_String'
//   - If multiple operations with the same key are performed, it
//     can be used to speedup access time by reducing the amount of
//     required hash operations.
//   - The correctness of the bucket is enforced in debug mode

DEE_OBJECT_DEF(DeeDictObject);
DEE_OBJECT_DEF(DeeDictProxyObject);
DEE_OBJECT_DEF(DeeDictIteratorObject);

#ifdef DEE_LIMITED_API
struct DeeDictObject {
 DEE_OBJECT_HEAD
#ifndef DEE_WITHOUT_THREADS
 struct DeeAtomicMutex d_lock; /*< Lock for this dict. */
#endif
 struct DeeHashMap     d_map;  /*< Internal map object. */
};
#define _DeeDict_InitCommon(ob) DeeAtomicMutex_Init(&(ob)->d_lock)
#ifndef DEE_WITHOUT_THREADS
#define DeeDict_LOCK(ob)                  (&((DeeDictObject*)Dee_REQUIRES_POINTER(ob))->d_lock)
#define DeeDict_ACQUIRE(ob)               DeeAtomicMutex_Acquire(DeeDict_LOCK(ob))
#define DeeDict_RELEASE(ob)               DeeAtomicMutex_Release(DeeDict_LOCK(ob))
#else
#define DeeDict_ACQUIRE(ob)               (void)0
#define DeeDict_RELEASE(ob)               (void)0
#endif
#define DeeDict_MAP(ob)                   (&((DeeDictObject*)Dee_REQUIRES_POINTER(ob))->d_map)
#define DeeDict_SIZE(ob)                  DeeHashMap_SIZE(DeeDict_MAP(ob))
#define DeeDict_BUCKET_COUNT(ob)          DeeHashMap_BUCKET_COUNT(DeeDict_MAP(ob))
#define DeeDict_GET_MAX_LOAD_FACTOR(ob)   DeeHashMap_GET_MAX_LOAD_FACTOR(DeeDict_MAP(ob))
#define DeeDict_SET_MAX_LOAD_FACTOR(ob,v) DeeHashMap_SET_MAX_LOAD_FACTOR(DeeDict_MAP(ob),v)
// v thread-safe and OK to do. (s.a.: explanation for 'DeeCell_EMPTY')
#define DeeDict_EMPTY(ob)                 (DeeHashMap_SIZE(DeeDict_MAP(ob))==0)
#else /* DEE_LIMITED_API */
#define DeeDict_SIZE(ob)                  DeeDict_Size((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeDict_BUCKET_COUNT(ob)          DeeDict_BucketCount((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeDict_GET_MAX_LOAD_FACTOR(ob)   DeeDict_GetMaxLoadFactor((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeDict_SET_MAX_LOAD_FACTOR(ob,v) DeeDict_SetMaxLoadFactor((DeeObject *)Dee_REQUIRES_POINTER(ob),v)
#define DeeDict_EMPTY(ob)                (DeeDict_SIZE(ob)==0)
#endif /* !DEE_LIMITED_API */

#ifdef DEE_LIMITED_API
struct DeeDictIteratorObject {
 DEE_OBJECT_HEAD // NOTE: The same data structure is used for .keys() and .items()
 DEE_A_REF DeeDictObject  *di_dict; /*< [1..1] Keep-alive reference. */
 struct DeeHashMapIterator di_iter; /*< Internal iterator object. */
};
#define DeeDictIterator_DICT(ob)           ((DeeObject *)((DeeDictIteratorObject *)Dee_REQUIRES_POINTER(ob))->di_dict)
#define DeeDictIterator_ITER(ob)           (&((DeeDictIteratorObject *)Dee_REQUIRES_POINTER(ob))->di_iter)
#define DeeDictIterator_CompareLo(lhs,rhs) DeeHashMapIterator_CompareLo(DeeDictIterator_ITER(lhs),DeeDictIterator_ITER(rhs))
#define DeeDictIterator_CompareLe(lhs,rhs) DeeHashMapIterator_CompareLe(DeeDictIterator_ITER(lhs),DeeDictIterator_ITER(rhs))
#define DeeDictIterator_CompareEq(lhs,rhs) DeeHashMapIterator_CompareEq(DeeDictIterator_ITER(lhs),DeeDictIterator_ITER(rhs))
#define DeeDictIterator_CompareNe(lhs,rhs) DeeHashMapIterator_CompareNe(DeeDictIterator_ITER(lhs),DeeDictIterator_ITER(rhs))
#define DeeDictIterator_CompareGr(lhs,rhs) DeeHashMapIterator_CompareGr(DeeDictIterator_ITER(lhs),DeeDictIterator_ITER(rhs))
#define DeeDictIterator_CompareGe(lhs,rhs) DeeHashMapIterator_CompareGe(DeeDictIterator_ITER(lhs),DeeDictIterator_ITER(rhs))
#endif /* DEE_LIMITED_API */

#ifdef DEE_LIMITED_API
struct DeeDictProxyObject {
 /* Iterable<tuple<object>>
    keys:   (get+del)
    items:  (get+put+del)
    values: (get+del) */
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDictObject *dp_dict; /*< [1..1] The dict we are referencing. */
};
#define DeeDictProxy_DICT(ob) ((DeeObject *)((DeeDictProxyObject *)Dee_REQUIRES_POINTER(ob))->dp_dict)
#else
#define DeeDictProxy_DICT(ob) DeeDictProxy_Dict((DeeObject *)Dee_REQUIRES_POINTER(ob))
#endif

DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_OBJECT(DeeDictObject) *)
 DeeDictProxy_Dict(DEE_A_IN_OBJECT(DeeDictProxyObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

#define DeeDict_Check(ob)                   DeeObject_InstanceOf(ob,&DeeDict_Type)
#define DeeDictKeys_Check(ob)               DeeObject_InstanceOf(ob,&DeeDictKeys_Type)
#define DeeDictItems_Check(ob)              DeeObject_InstanceOf(ob,&DeeDictItems_Type)
#define DeeDictValues_Check(ob)             DeeObject_InstanceOf(ob,&DeeDictValues_Type)
#define DeeDictIterator_Check(ob)           DeeObject_InstanceOf(ob,&DeeDictIterator_Type)
#define DeeDictKeysIterator_Check(ob)       DeeObject_InstanceOf(ob,&DeeDictKeysIterator_Type)
#define DeeDictItemsIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeDictItemsIterator_Type)
#define DeeDictValuesIterator_Check         DeeDictIterator_Check
#define DeeDict_CheckExact(ob)              DeeObject_InstanceOfExact(ob,&DeeDict_Type)
#define DeeDictKeys_CheckExact(ob)          DeeObject_InstanceOfExact(ob,&DeeDictKeys_Type)
#define DeeDictItems_CheckExact(ob)         DeeObject_InstanceOfExact(ob,&DeeDictItems_Type)
#define DeeDictValues_CheckExact(ob)        DeeObject_InstanceOfExact(ob,&DeeDictValues_Type)
#define DeeDictIterator_CheckExact(ob)      DeeObject_InstanceOfExact(ob,&DeeDictIterator_Type)
#define DeeDictKeysIterator_CheckExact(ob)  DeeObject_InstanceOfExact(ob,&DeeDictKeysIterator_Type)
#define DeeDictItemsIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeDictItemsIterator_Type)
#define DeeDictValuesIterator_CheckExact    DeeDictIterator_CheckExact

// v any object with the "DeeDictProxyObject" data structure
#define DeeDictProxy_Check(ob) (DeeDictKeys_Check(ob)||DeeDictItems_Check(ob)||DeeDictValues_Check(ob))
#define DeeDictProxy_CheckExact(ob) (DeeDictKeys_CheckExact(ob)||DeeDictItems_CheckExact(ob)||DeeDictValues_CheckExact(ob))

// v any object with the "DeeDictIteratorObject" data structure
#define DeeDictAnyIterator_Check(ob) (DeeDictIterator_Check(ob)||DeeDictKeysIterator_Check(ob)||DeeDictItemsIterator_Check(ob))
#define DeeDictAnyIterator_CheckExact(ob) (DeeDictIterator_CheckExact(ob)||DeeDictKeysIterator_CheckExact(ob)||DeeDictItemsIterator_CheckExact(ob))

DEE_DATA_DECL(DeeTypeObject) DeeDict_Type;
DEE_DATA_DECL(DeeTypeObject) DeeDictKeys_Type;
DEE_DATA_DECL(DeeTypeObject) DeeDictItems_Type;
DEE_DATA_DECL(DeeTypeObject) DeeDictValues_Type;
DEE_DATA_DECL(DeeTypeObject) DeeDictIterator_Type;
DEE_DATA_DECL(DeeTypeObject) DeeDictKeysIterator_Type;
DEE_DATA_DECL(DeeTypeObject) DeeDictItemsIterator_Type;

//////////////////////////////////////////////////////////////////////////
// Allocate a new dict object
#define                                                                DeeDict_NewEmpty DeeDict_New
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictObject) *) DeeDict_New(void);
#define /* DEE_A_EXEC DeeDictObject * */DeeDict_NewFromSequence(sequence) DeeDict_TNewFromSequence(Dee_TYPE(sequence),sequence)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictObject) *) DeeDict_TNewFromSequence(DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictObject) *) DeeDict_NewFromIterator(DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictObject) *) DeeDict_Copy(DEE_A_IN_OBJECT(DeeDictObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictObject) *) DeeDict_DeepCopy(DEE_A_IN_OBJECT(DeeDictObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Create a new dict with key-value pairs
// - 'pairv' contains 'pairc*2' objects where every first is the key and every second is the item
// - Used to initialize static dicts in user code ('a = {"foo" : "bar", 10 : 20, };')
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictObject) *) DeeDict_NewFromKeyValuePairs(
 DEE_A_IN Dee_size_t pairc, DEE_A_IN_R(pairc*2) DeeObject *const *pairv);

//////////////////////////////////////////////////////////////////////////
// Inserts element pairs from a given iterator into a given dict.
//  -> Sequences and iterators are converted same as by 'DeeDict_NewFromSequence' / 'DeeDict_NewFromIterator'
//  -> In usercode, 'DeeDict_InsertSequence' is available through 'dict.insert_all'
#define /* DEE_A_EXEC int */DeeDict_InsertSequence(ob,sequence) DeeDict_TInsertSequence(ob,Dee_TYPE(sequence),sequence)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDict_TInsertSequence(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN DeeTypeObject const *tp_sequence,
 DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDict_InsertIterator(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Pop 'key' from 'self' and return 'def' or throw an 'Error.KeyError' if 'key' wasn't found
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_Pop(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT_OPT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_PopWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT_OPT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_PopString(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key,
 DEE_A_INOUT_OPT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_PopStringWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT_OPT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,4));

//////////////////////////////////////////////////////////////////////////
// Returns the item associated with 'key' or sets the 'item' to 'def' if 'key' didn't exist
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_SetDefault(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_SetDefaultWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_SetDefaultString(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key,
 DEE_A_INOUT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_SetDefaultStringWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,4));

//////////////////////////////////////////////////////////////////////////
// Returns the item associated with 'key' or 'def' if 'key' didn't exist
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_GetDefault(
 DEE_A_INOUT_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_GetDefaultWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_GetDefaultString(
 DEE_A_INOUT_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key,
 DEE_A_INOUT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_GetDefaultStringWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,4));

//////////////////////////////////////////////////////////////////////////
// Pop some key-item pair from 'self' and store it in '*key' and '*item'
// Throws an 'Error.ValueError' if 'self' is empty
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDict_PopSomething(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_REF DEE_A_OUT DeeObject **key,
 DEE_A_REF DEE_A_OUT DeeObject **item) DEE_ATTRIBUTE_NONNULL((1));

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeDict_FindKeyIndex(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeDict_FindKeyIndexWithKnownBucket(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeDict_FindKeyIndexString(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeDict_FindKeyIndexStringWithKnownBucket(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeDict_FindItemIndex(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeDict_FindKeyItemIndex(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeDict_FindKeyItemIndexWithKnownBucket(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeDict_FindKeyItemIndexString(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeDict_FindKeyItemIndexStringWithKnownBucket(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeDict_IndexKeyIndex(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeDict_IndexKeyIndexWithKnownBucket(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeDict_IndexKeyIndexString(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeDict_IndexKeyIndexStringWithKnownBucket(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeDict_IndexItemIndex(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeDict_IndexKeyItemIndex(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeDict_IndexKeyItemIndexWithKnownBucket(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeDict_IndexKeyItemIndexString(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeDict_IndexKeyItemIndexStringWithKnownBucket(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeDict_CountItem(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Returns the item associated with a given key
// NOTE: 'DeeDict_GetItem' can raise an error if 'tp_hash' throws an error
// NOTE: If a *WithError function is used, a KeyError is set if NULL would have been returned without an error
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_GetItem(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_GetItemWithKnownBucket(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_GetItemString(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_GetItemStringWithKnownBucket(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int)  DeeDict_GetItemEx(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_REF DEE_A_OUT DeeObject **result) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int)  DeeDict_GetItemExWithKnownBucket(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_REF DEE_A_OUT DeeObject **result) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int)  DeeDict_GetItemExString(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_REF DEE_A_OUT DeeObject **result) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int)  DeeDict_GetItemExStringWithKnownBucket(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_REF DEE_A_OUT DeeObject **result) DEE_ATTRIBUTE_NONNULL((1,2,4));

//////////////////////////////////////////////////////////////////////////
// Checks for the existence of a key / item / key-item pair
// >> Returns -1: On error (caused if 'DeeObject_HashEx' throws)
// >> Returns  0: Key doesn't exist
// >> Returns  1: Key does exist
// NOTE: some functions can return -1 if 'DeeObject_HashEx' throws an error (s.a. annotations)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeDict_HasKey(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeDict_HasKeyWithKnownBucket(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeDict_HasKeyString(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeDict_HasKeyStringWithKnownBucket(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeDict_HasItem(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeDict_HasKeyItem(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeDict_HasKeyItemWithKnownBucket(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeDict_HasKeyItemString(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeDict_HasKeyItemStringWithKnownBucket(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,4));

#ifdef DEE_LIMITED_API
extern DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDict_HasKeyPred(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDict_HasItemPred(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *item,
 DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDict_HasKeyItemPred(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *keyitem,
 DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
#endif


//////////////////////////////////////////////////////////////////////////
// Deletes a key
// >> Returns -1: On error (can always be caused by reallocation failure)
// >> Returns  0: On success
// >> Returns  1: Key not found (no error set; never returned if *WithError is used)
// NOTE: If a *WithError function is used, -1 will be returned instead of 1, alongside raising a KeyError
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDict_DelItem(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDict_DelItemWithKnownBucket(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDict_DelItemWithError(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDict_DelItemWithErrorAndKnownBucket(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDict_DelItemString(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDict_DelItemStringWithKnownBucket(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDict_DelItemStringWithError(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDict_DelItemStringWithErrorAndKnownBucket(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Sets an item
// >> Returns -1: On error
// >> Returns  0: On success
// >> Returns  1: Key already existed (only returned by DeeDict_SetNewItem* / DeeDict_SetOldItem*)
// NOTE: DeeDict_SetItem* will overwrite existing items
// NOTE: DeeDict_SetNewItem* will not overwrite existing items (returns 1 if 'key' already existed)
// NOTE: DeeDict_SetOldItem* will only overwrite existing items (returns 1 if 'key' didn't exist)
// NOTE: If 'old_item' is non-NULL, 'DeeDict_SetNewItem' will store a reference to the existing item inside when returning '1'
// NOTE: If 'old_item' is non-NULL, 'DeeDict_SetOldItem' will store the old item instead of destroying it when returning '0'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDict_SetItem(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDict_SetItemWithKnownBucket(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDict_SetItemString(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDict_SetItemStringWithKnownBucket(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDict_SetNewItem(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDict_SetNewItemWithKnownBucket(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDict_SetNewItemString(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDict_SetNewItemStringWithKnownBucket(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDict_SetOldItem(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDict_SetOldItemWithKnownBucket(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDict_SetOldItemString(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDict_SetOldItemStringWithKnownBucket(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) DEE_ATTRIBUTE_NONNULL((1,2,4));

//////////////////////////////////////////////////////////////////////////
// Index-based dict functions (used to simulate array operators for proxy types)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_GetKeyIndex(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_ssize_t index) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_GetKeyIndexAt(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_size_t index) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_GetItemIndex(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_ssize_t index) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_GetItemIndexAt(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_size_t index) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeDict_GetValueIndex(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_ssize_t index) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeDict_GetValueIndexAt(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_size_t index) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeDict_GetKeyRangeIndex(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_ssize_t lo, DEE_A_IN Dee_ssize_t hi) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeDict_GetItemRangeIndex(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_ssize_t lo, DEE_A_IN Dee_ssize_t hi) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeDict_GetValueRangeIndex(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_ssize_t lo, DEE_A_IN Dee_ssize_t hi) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDict_DelItemIndex(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN Dee_ssize_t index) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDict_DelItemRangeIndex(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDict_SetItemIndex(DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN Dee_ssize_t index, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,3));

//////////////////////////////////////////////////////////////////////////
// Dict proxy objects
// >> Returns a new Dict proxy object or NULL and raises an exception
// >> All lists share the same indices for the same key / item combination
// >> 'DeeDict_Keys(d)'   >> 'd.keys()'   // List of keys
// >> 'DeeDict_Items(d)'  >> 'd.items()'  // List of items
// >> 'DeeDict_Values(d)' >> 'd.values()' // List of (key,item) tuples
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictProxyObject) *) DeeDict_Keys(DEE_A_INOUT_OBJECT(DeeDictObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictProxyObject) *) DeeDict_Items(DEE_A_INOUT_OBJECT(DeeDictObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictProxyObject) *) DeeDict_Values(DEE_A_INOUT_OBJECT(DeeDictObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT_ITERATE_FUNC
DEE_PRIVATE_DECL_DEE_OBJECT_ITERATE_FUNC
#undef DEE_PRIVATE_DECL_DEE_OBJECT_ITERATE_FUNC
#endif

//////////////////////////////////////////////////////////////////////////
// Traverse a dictionary
// >> If the "trav" function returns a non-zero value, the function stops and returns that value
// >> Otherwise the function will call "trav" for all its key-item pairs and return 0
// NOTE: 'DeeDict_Traverse' never throws an exception itself, only forwarding
//       exceptions thrown by the given 'trav' function.
//       Though 'DeeDict_TraverseValues' can in fact throw exceptions.
typedef DEE_A_RET_EXCEPT(-1) int (*DeeDictTraverseFunc)(
 DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item, DEE_A_IN_OPT void *closure) /*DEE_ATTRIBUTE_NONNULL((1,2))*/;
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDict_Traverse(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN DeeDictTraverseFunc trav, DEE_A_IN_OPT void *closure) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDict_TraverseKeys(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN DeeObjectIterateFunc trav, DEE_A_IN_OPT void *closure) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDict_TraverseItems(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN DeeObjectIterateFunc trav, DEE_A_IN_OPT void *closure) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDict_TraverseValues(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN DeeObjectIterateFunc trav, DEE_A_IN_OPT void *closure) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Locate functions for Dicts
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_LocateKey(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDict_LocateItem(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeDict_LocateValue(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictObject) *) DeeDict_LocateAll(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeDict_LocateAllKeys(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeDict_LocateAllItems(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeDict_LocateAllValues(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Transform functions for Dicts
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictObject) *) DeeDict_Transform(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeDict_TransformKeys(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeDict_TransformItems(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeDict_TransformValues(DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Front/Back functions for Dicts
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) DeeDict_Front(
           DEE_A_IN_OBJECT(DeeDictObject) const *self,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **key,
           DEE_A_OUT_OPT Dee_hash_t *bucket,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **item) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) DeeDict_Back(
           DEE_A_IN_OBJECT(DeeDictObject) const *self,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **key,
           DEE_A_OUT_OPT Dee_hash_t *bucket,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **item) DEE_ATTRIBUTE_NONNULL((1));


#ifdef DEE_LIMITED_API
extern DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeDict_Str(
 DEE_A_IN_OBJECT(DeeDictObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeDict_Repr(
 DEE_A_IN_OBJECT(DeeDictObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#elif DEE_DEPRECATED_API_VERSION(100,101,103)
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1))
 DEE_ATTRIBUTE_DEPRECATED("DeeDict_Str() is a limited function. Use 'DeeObject_Str()' instead")
 DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeDict_Str(
 DEE_A_IN_OBJECT(DeeDictObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeObject_Str((DeeObject *)self);
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1))
 DEE_ATTRIBUTE_DEPRECATED("DeeDict_Repr() is a limited function. Use 'DeeObject_Repr()' instead")
 DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeDict_Repr(
 DEE_A_IN_OBJECT(DeeDictObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeObject_Repr((DeeObject *)self);
}
#endif

//////////////////////////////////////////////////////////////////////////
// Dict functionality
DEE_FUNC_DECL(DEE_A_EXEC void) DeeDict_Clear(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeDict_Size(
 DEE_A_IN_OBJECT(DeeDictObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeDict_BucketSize(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_ssize_t bucket) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeDict_BucketCount(
 DEE_A_IN_OBJECT(DeeDictObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// max_load_factor access
DEE_FUNC_DECL(DEE_A_RET_WUNUSED double) DeeDict_GetMaxLoadFactor(
 DEE_A_IN_OBJECT(DeeDictObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeDict_SetMaxLoadFactor(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN double v) DEE_ATTRIBUTE_NONNULL((1));

#ifdef DEE_LIMITED_API
// Iterator creations functions
extern DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictIteratorObject) *DeeDict_IterSelf(
 DEE_A_INOUT_OBJECT(DeeDictObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictIteratorObject) *DeeDictKeys_IterSelf(
 DEE_A_IN_OBJECT(DeeDictProxyObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictIteratorObject) *DeeDictItems_IterSelf(
 DEE_A_IN_OBJECT(DeeDictProxyObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictIteratorObject) *DeeDictValues_IterSelf(
 DEE_A_IN_OBJECT(DeeDictProxyObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_DICT_H */
