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
#ifndef GUARD_DEEMON_OPTIONAL_HASHSET_H
#define GUARD_DEEMON_OPTIONAL_HASHSET_H 1

#include <deemon/__conf.inl>

DEE_DECL_BEGIN

#if defined(DEE_LIMITED_API) || \
    DEE_CONFIG_API_HAVE_HASHSET_EXPORTS
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_HASH_T
DEE_PRIVATE_DECL_DEE_HASH_T
#undef DEE_PRIVATE_DECL_DEE_HASH_T
#endif

#if DEE_CONFIG_API_HAVE_HASHSET_EXPORTS
#define DEE_HASHSET_FUNC_DECL    DEE_FUNC_DECL
#else
#define DEE_HASHSET_FUNC_DECL(T) extern T
#endif

#ifdef DEE_LIMITED_DEX
struct _DeeHashSetBucketEntry {
 Dee_hash_t           sbe_hash; /*< Hash associated with the entry. */
 DEE_A_REF DeeObject *sbe_key;  /*< [1..1] Key of this entry. */
};
#define _DeeHashSetBucketEntry_HASH(ob) (ob)->sbe_hash
#define _DeeHashSetBucketEntry_KEY(ob)  (ob)->sbe_key
struct _DeeHashSetBucket {
 Dee_size_t                     hsb_entryc; /*< Amount of entries in this bucket. */
 struct _DeeHashSetBucketEntry *hsb_entryv; /*< [1..1][0..b_keyc][owned] Vector of entries in this bucket. */
};
#ifdef DEE_LIMITED_API
extern void _DeeHashSetBucket_Init(DEE_A_OUT struct _DeeHashSetBucket *self);
extern void _DeeHashSetBucket_Quit(DEE_A_IN struct _DeeHashSetBucket *self);
extern DEE_A_RET_NOEXCEPT(-1) int _DeeHashSetBucket_TryAddNew(
 DEE_A_INOUT struct _DeeHashSetBucket *self, DEE_A_IN Dee_hash_t hash,
 DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,3));
extern DEE_A_RET_NOEXCEPT_REF DeeObject *_DeeHashSetBucket_TryPopSomething(
 DEE_A_INOUT struct _DeeHashSetBucket *self, DEE_A_OUT Dee_hash_t *hash) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_NOEXCEPT(-1) int _DeeHashSetBucket_TryTransfer(
 DEE_A_INOUT struct _DeeHashSetBucket *dst,
 DEE_A_INOUT struct _DeeHashSetBucket *src,
 DEE_A_IN Dee_size_t src_index) DEE_ATTRIBUTE_NONNULL((1,2));
#endif

struct DeeHashSetIterator {
 struct _DeeHashSetBucket      *hsi_buckets_begin; /*< [0..1] Pointer to the first bucket. */
 struct _DeeHashSetBucket      *hsi_buckets_end;   /*< [0..1] Pointer to the bucket after the last valid. */
 struct _DeeHashSetBucketEntry *hsi_hash_curr;     /*< [0..1] Current node in the current bucket (element of hsi_buckets_begin). */
 struct _DeeHashSetBucketEntry *hsi_hash_curr_end; /*< [0..1] End of 'hsi_buckets_begin' == (hsi_buckets_begin->hsb_entryv+hsi_buckets_begin->hsb_entryc). */
};

#ifdef DEE_LIMITED_API
extern DEE_A_RET_WUNUSED int DeeHashSetIterator_CompareLo(DEE_A_IN struct DeeHashSetIterator const *lhs, DEE_A_IN struct DeeHashSetIterator const *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_WUNUSED int DeeHashSetIterator_CompareLe(DEE_A_IN struct DeeHashSetIterator const *lhs, DEE_A_IN struct DeeHashSetIterator const *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_WUNUSED int DeeHashSetIterator_CompareEq(DEE_A_IN struct DeeHashSetIterator const *lhs, DEE_A_IN struct DeeHashSetIterator const *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
#define DeeHashSetIterator_CompareNe !DeeHashSetIterator_CompareEq
#define DeeHashSetIterator_CompareGr !DeeHashSetIterator_CompareLe
#define DeeHashSetIterator_CompareGe !DeeHashSetIterator_CompareLo
#endif

#endif /* DEE_LIMITED_DEX */


#ifdef DEE_LIMITED_DEX
#define DEE_HASHSET_INITIAL_BUCKET_COUNT 8
struct DeeHashSet {
 struct _DeeHashSetBucket *hs_valid_buckets_begin; /*< [0..1] First non-empty bucket (hs_buckets+n | n >= 0 && n < hs_bucket_count) */
 struct _DeeHashSetBucket *hs_valid_buckets_end;   /*< [0..1] Bucket after the last non-empty bucket (hs_buckets+hs_bucket_count-n | n >= 0 && n < hs_bucket_count) */
 struct _DeeHashSetBucket *hs_buckets;             /*< [0..hs_bucket_count][owned] Actual map of buckets (index == hash % hs_bucket_count) */
 Dee_size_t                hs_bucket_count;        /*< Amount of allocated buckets (any 2^n number; based on hs_map_size and hs_max_load_factor) */
 Dee_size_t                hs_map_size;            /*< Amount of existing nodes. */
 double                    hs_max_load_factor;     /*< Load factor (affects map_size vs bucket_count ratio; default: 1.0). */
};
#define DeeHashSet_INIT()                    {NULL,NULL,NULL,0,0,1.0}
#define DeeHashSet_SIZE(ob)                         (ob)->hs_map_size
#define DeeHashSet_BUCKET_COUNT(ob)                 (ob)->hs_bucket_count
#define DeeHashSet_BUCKET_SIZE(ob,bucket)           (ob)->hs_buckets[bucket].hsb_entryc
#define DeeHashSet_GET_MAX_LOAD_FACTOR(ob)          (ob)->hs_max_load_factor
#define DeeHashSet_SET_MAX_LOAD_FACTOR(ob,v) (void)((ob)->hs_max_load_factor=(v))
#else
#define DeeHashSet_SIZE                DeeHashSet_Size
#define DeeHashSet_BUCKET_COUNT        DeeHashSet_BucketCount
#define DeeHashSet_BUCKET_SIZE         DeeHashSet_BucketSize
#define DeeHashSet_GET_MAX_LOAD_FACTOR DeeHashSet_GetMaxLoadFactor
#define DeeHashSet_SET_MAX_LOAD_FACTOR DeeHashSet_SetMaxLoadFactor
#endif

#ifdef DEE_LIMITED_DEX
#define DeeHashSet_TRAVERSE_VARS\
 struct _DeeHashSetBucket *_set_buckets_begin,*_set_buckets_end;\
 struct _DeeHashSetBucketEntry *_set_bucket_end
#define DeeHashSet_TRAVERSE(entry,set)\
 for (_set_buckets_begin = (set)->hs_valid_buckets_begin,\
      _set_buckets_end = (set)->hs_valid_buckets_end;\
      _set_buckets_begin != _set_buckets_end;)\
  for ((_set_bucket_end) = ((entry) = _set_buckets_begin->hsb_entryv)+\
                                      _set_buckets_begin->hsb_entryc,\
      ++_set_buckets_begin;(entry) != _set_bucket_end; ++(entry))
#define DeeHashSet_TRAVERSE_SAFE_VARS\
 struct DeeHashSetIterator _set_iterator
#define DeeHashSet_TRAVERSE_SAFE(entry,set)\
 for (DeeHashSetIterator_Init(&_set_iterator,set);\
     ((entry)=DeeHashSetIterator_Yield(&_set_iterator,set))!=NULL;)
#define DeeHashSet_TRAVERSE_INF_VARS\
 struct _DeeHashSetBucket *_set_buckets_begin;\
 struct _DeeHashSetBucketEntry *_set_bucket_end
#define DeeHashSet_TRAVERSE_INF(entry,set)\
 for (_set_buckets_begin = (set)->hs_valid_buckets_begin;;)\
  for ((_set_bucket_end) = ((entry) = _set_buckets_begin->hsb_entryv)+\
                                      _set_buckets_begin->hsb_entryc,\
      ++_set_buckets_begin;(entry) != _set_bucket_end; ++(entry))
#define DeeHashSet_TRAVERSE_RANGE_VARS\
 struct _DeeHashSetBucket *_set_buckets_begin,*_set_buckets_end;\
 struct _DeeHashSetBucketEntry *_set_bucket_end;\
 Dee_size_t _set_pre,_set_n
#define DeeHashSet_TRAVERSE_RANGE(entry,set,lo,hi)\
 for (_set_buckets_begin = (set)->hs_valid_buckets_begin,\
      _set_buckets_end = (set)->hs_valid_buckets_end,\
      _set_n = (hi)-(_set_pre=(lo));\
      _set_buckets_begin != _set_buckets_end && _set_n;)\
  for ((_set_bucket_end) = ((entry) = _set_buckets_begin->hsb_entryv)+\
                                      _set_buckets_begin->hsb_entryc,\
      ++_set_buckets_begin;(entry) != _set_bucket_end && _set_n; ++(entry))\
  if (_set_pre) --_set_pre; else if ((--_set_n,0)){}else
#endif

DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct DeeHashSet *) DeeHashSet_Alloc(void);
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct DeeHashSet *) DeeHashSet_New(void);
DEE_HASHSET_FUNC_DECL(           void) DeeHashSet_Free(DEE_A_IN DEE_A_OUT_INVALID struct DeeHashSet *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC void) DeeHashSet_Delete(DEE_A_IN DEE_A_OUT_INVALID struct DeeHashSet *self) DEE_ATTRIBUTE_NONNULL((1));

DEE_HASHSET_FUNC_DECL(void) DeeHashSet_Init(DEE_A_OUT struct DeeHashSet *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHSET_FUNC_DECL(void) DeeHashSet_InitMove(DEE_A_OUT struct DeeHashSet *self, DEE_A_INOUT struct DeeHashSet *right) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashSet_InitCopy(DEE_A_OUT struct DeeHashSet *self, DEE_A_IN struct DeeHashSet const *right) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashSet_InitDeepCopy(DEE_A_OUT struct DeeHashSet *self, DEE_A_IN struct DeeHashSet const *right) DEE_ATTRIBUTE_NONNULL((1,2));
#define /* DEE_A_EXEC int */DeeHashSet_InitFromSequence(ob,sequence) DeeHashSet_TInitFromSequence(ob,Dee_TYPE(sequence),sequence)
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashSet_TInitFromSequence(DEE_A_OUT struct DeeHashSet *self, DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashSet_InitFromIterator(DEE_A_OUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashSet_InitFromKeys(DEE_A_OUT struct DeeHashSet *self, DEE_A_IN Dee_size_t keyc, DEE_A_IN_R(keyc) DeeObject *const *keyv) DEE_ATTRIBUTE_NONNULL((1));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashSet_InitCopyWithLock(DEE_A_OUT struct DeeHashSet *self, DEE_A_IN struct DeeHashSet const *right, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashSet_InitDeepCopyWithLock(DEE_A_OUT struct DeeHashSet *self, DEE_A_IN struct DeeHashSet const *right, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
#else
#define /*DEE_A_EXEC*/ DeeHashSet_InitCopyWithLock(self,right,lock)     DeeHashSet_InitCopy(self,right)
#define /*DEE_A_EXEC*/ DeeHashSet_InitDeepCopyWithLock(self,right,lock) DeeHashSet_InitDeepCopy(self,right)
#endif
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashSet_AssignCopy(DEE_A_INOUT struct DeeHashSet *self, DEE_A_IN struct DeeHashSet const *right) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC void) DeeHashSet_Quit(DEE_A_IN DEE_A_OUT_INVALID struct DeeHashSet *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashSet_Hash(DEE_A_IN struct DeeHashSet const *self, DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result) DEE_ATTRIBUTE_NONNULL((1,3));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashSet_HashWithLock(DEE_A_IN struct DeeHashSet const *self, DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,3,4));
#else
#define /*DEE_A_EXEC*/ DeeHashSet_HashWithLock(self,start,result,lock) DeeHashSet_Hash(self,start,result)
#endif

#define /* DEE_A_EXEC int */DeeHashSet_InsertSequence(ob,sequence)              DeeHashSet_TInsertSequence(ob,Dee_TYPE(sequence),sequence)
#define /* DEE_A_EXEC int */DeeHashSet_InsertSequenceWithLock(ob,sequence,lock) DeeHashSet_TInsertSequenceWithLock(ob,Dee_TYPE(sequence),sequence,lock)
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashSet_TInsertSequence(DEE_A_INOUT struct DeeHashSet *self, DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashSet_TInsertSequenceWithLock(DEE_A_INOUT struct DeeHashSet *self, DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashSet_InsertIterator(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashSet_InsertIteratorWithLock(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *iterator, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));

//////////////////////////////////////////////////////////////////////////
// Arguably the most importable methods of any set object...
// - 'DeeHashSet_Insert' will fail and return 1, if 'ob' is already part of the set
// - 'DeeHashSet_Contains' returns positive, non-zero if 'ob' is contained
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashSet_Insert(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashSet_InsertWithKnownBucket(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashSet_Remove(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashSet_RemoveWithKnownBucket(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashSet_Contains(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashSet_ContainsWithKnownBucket(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashSet_ContainsPred(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashSet_InsertWithLock(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashSet_InsertWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashSet_RemoveWithLock(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashSet_RemoveWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashSet_ContainsWithLock(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashSet_ContainsWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashSet_ContainsPredWithLock(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob, DEE_A_INOUT DeeObject *pred, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashSet_ContainsPredWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *pred, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
#else
#define /* DEE_A_EXEC int */DeeHashSet_InsertWithLock(self,ob,lock)                                 DeeHashSet_Insert(self,ob)
#define /* DEE_A_EXEC int */DeeHashSet_InsertWithKnownBucketAndLock(self,ob,bucket,lock)            DeeHashSet_InsertWithKnownBucket(self,ob,bucket)
#define /* DEE_A_EXEC int */DeeHashSet_RemoveWithLock(self,ob,lock)                                 DeeHashSet_Remove(self,ob)
#define /* DEE_A_EXEC int */DeeHashSet_RemoveWithKnownBucketAndLock(self,ob,bucket,lock)            DeeHashSet_RemoveWithKnownBucket(self,ob,bucket)
#define /* DEE_A_EXEC int */DeeHashSet_ContainsWithLock(self,ob,lock)                               DeeHashSet_Contains(self,ob)
#define /* DEE_A_EXEC int */DeeHashSet_ContainsWithKnownBucketAndLock(self,ob,bucket,lock)          DeeHashSet_ContainsWithKnownBucket(self,ob,bucket)
#define /* DEE_A_EXEC int */DeeHashSet_ContainsPredWithLock(self,ob,pred,lock)                      DeeHashSet_ContainsPred(self,ob,pred)
#define /* DEE_A_EXEC int */DeeHashSet_ContainsPredWithKnownBucketAndLock(self,ob,bucket,pred,lock) DeeHashSet_ContainsPredWithKnownBucke(self,ob,bucket,pred)
#endif

//////////////////////////////////////////////////////////////////////////
// Clears all keys from 'self'
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC void) DeeHashSet_Clear(DEE_A_INOUT struct DeeHashSet *self) DEE_ATTRIBUTE_NONNULL((1));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC void) DeeHashSet_ClearWithLock(DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2));
#else
#define /* DEE_A_EXEC void */DeeHashSet_ClearWithLock(self,lock) DeeHashSet_Clear(self)
#endif

//////////////////////////////////////////////////////////////////////////
// HashSet functionality
DEE_HASHSET_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeHashSet_Size(DEE_A_IN struct DeeHashSet const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHSET_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeHashSet_BucketSize(DEE_A_IN struct DeeHashSet const *self, DEE_A_IN Dee_size_t bucket) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHSET_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeHashSet_BucketCount(DEE_A_IN struct DeeHashSet const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// HashSet max_load_factor access
DEE_HASHSET_FUNC_DECL(DEE_A_RET_WUNUSED double) DeeHashSet_GetMaxLoadFactor(DEE_A_IN struct DeeHashSet const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHSET_FUNC_DECL(void) DeeHashSet_SetMaxLoadFactor(DEE_A_INOUT struct DeeHashSet *self, DEE_A_IN double v) DEE_ATTRIBUTE_NONNULL((1));


DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct DeeHashSetIterator *) DeeHashSetIterator_Alloc(void);
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct DeeHashSetIterator *) DeeHashSetIterator_New(DEE_A_IN struct DeeHashSet const *set) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHSET_FUNC_DECL(void) DeeHashSetIterator_Free(DEE_A_IN DEE_A_OUT_INVALID struct DeeHashSetIterator *self) DEE_ATTRIBUTE_NONNULL((1));
#define DeeHashSetIterator_Delete DeeHashSetIterator_Free
DEE_HASHSET_FUNC_DECL(void) DeeHashSetIterator_Init(DEE_A_OUT struct DeeHashSetIterator *self, DEE_A_IN struct DeeHashSet const *set) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHSET_FUNC_DECL(DEE_A_RET_MAYBE_NULL struct _DeeHashSetBucketEntry *) DeeHashSetIterator_Yield(
 DEE_A_INOUT struct DeeHashSetIterator *self, DEE_A_IN struct DeeHashSet const *set) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Pops a key from 'self' and returns it
// -> Throws an 'Error.ValueError' if the set is empty
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashSet_PopSomething(
 DEE_A_INOUT struct DeeHashSet *self) DEE_ATTRIBUTE_NONNULL((1));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashSet_PopSomethingWithLock(
 DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2));
#else
#define /* DEE_A_EXEC DeeObject * */DeeHashSet_PopSomethingWithLock(self,lock) DeeHashSet_PopSomething(self)
#endif

//////////////////////////////////////////////////////////////////////////
// Returns the key of a given index
// >> Causes undefined behavior if 'index >= DeeHashSet_SIZE(self)'
DEE_HASHSET_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_NEVER_NULL DEE_A_RET_WUNUSED DeeObject *)
 DeeHashSet_GetKeyIndex(DEE_A_IN struct DeeHashSet const *self, DEE_A_IN Dee_size_t index) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashSet_GetKeyRangeIndex(
 DEE_A_IN struct DeeHashSet const *self, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) DEE_ATTRIBUTE_NONNULL((1));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashSet_GetKeyRangeIndexWithLock(
 DEE_A_IN struct DeeHashSet const *self, DEE_A_IN Dee_size_t lo,
 DEE_A_IN Dee_size_t hi, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,4));
#else
#define /* DEE_A_EXEC DeeObject * */DeeHashSet_GetKeyRangeIndexWithLock(self,lo,hi,lock) DeeHashSet_GetKeyRangeIndex(self,lo,hi)
#endif

//////////////////////////////////////////////////////////////////////////
// Searches for 'key' and returns its index
// >> Returns -1 if the key wasn't found (no error set)
// >> Returns -2 if an error occurred
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashSet_FindKeyIndex(
 DEE_A_IN struct DeeHashSet const *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashSet_FindKeyIndexWithKnownBucket(
 DEE_A_IN struct DeeHashSet const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashSet_FindKeyIndexWithLock(
 DEE_A_IN struct DeeHashSet const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashSet_FindKeyIndexWithKnownBucketAndLock(
 DEE_A_IN struct DeeHashSet const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket,
 DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
#else
#define /* DEE_A_EXEC Dee_ssize_t */DeeHashSet_FindKeyIndexWithLock(self,key,lock) DeeHashSet_FindKeyIndex(self,key)
#define /* DEE_A_EXEC Dee_ssize_t */DeeHashSet_FindKeyIndexWithKnownBucketAndLock(self,key,bucket,lock) DeeHashSet_FindKeyIndexWithKnownBucket(self,key,bucket) 
#endif

//////////////////////////////////////////////////////////////////////////
// Traverse a hash-set
// >> If the "trav" function returns a non-zero value, the function stops and returns that value
// >> Otherwise the function will call "trav" for all its keys and return 0
// NOTE: 'DeeHashSet_Traverse' never throws an exception itself
typedef DEE_A_RET_EXCEPT(-1) int (*DeeHashSetTraverseFunc)(
 DEE_A_INOUT DeeObject *key, DEE_A_IN_OPT void *closure);
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC_IF((*trav)(...,closure)) DEE_A_RET_EXCEPT(-1) int) DeeHashSet_Traverse(
 DEE_A_IN struct DeeHashSet const *self, DEE_A_IN DeeHashSetTraverseFunc trav,
 DEE_A_IN_OPT void *closure) DEE_ATTRIBUTE_NONNULL((1,2));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHSET_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashSet_TraverseWithLock(
 DEE_A_IN struct DeeHashSet const *self, DEE_A_IN DeeHashSetTraverseFunc trav,
 DEE_A_IN_OPT void *closure, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
#else
#define /* DEE_A_EXEC_IF((*trav)(...,closure)) int */DeeHashSet_TraverseWithLock(self,trav,closure,lock) DeeHashSet_Traverse(self,trav,closure)
#endif


#ifdef DEE_LIMITED_API
extern DEE_A_RET_NEVER_NULL struct _DeeHashSetBucketEntry *_DeeHashSet_FrontEntry(DEE_A_IN struct DeeHashSet const *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_NEVER_NULL struct _DeeHashSetBucketEntry *_DeeHashSet_BackEntry(DEE_A_IN struct DeeHashSet const *self) DEE_ATTRIBUTE_NONNULL((1));
#endif
#undef DEE_HASHSET_FUNC_DECL
#endif

                                          
DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_HASHSET_H */
