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
#ifndef GUARD_DEEMON_OPTIONAL_HASHMAP_H
#define GUARD_DEEMON_OPTIONAL_HASHMAP_H 1

#include <deemon/__conf.inl>

DEE_DECL_BEGIN

#if defined(DEE_LIMITED_API) || \
    DEE_CONFIG_API_HAVE_HASHMAP_EXPORTS
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
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT_ITERATE_FUNC
DEE_PRIVATE_DECL_DEE_OBJECT_ITERATE_FUNC
#undef DEE_PRIVATE_DECL_DEE_OBJECT_ITERATE_FUNC
#endif

#if DEE_CONFIG_API_HAVE_HASHMAP_EXPORTS
#define DEE_HASHMAP_FUNC_DECL    DEE_FUNC_DECL
#else
#define DEE_HASHMAP_FUNC_DECL(T) extern T
#endif
#ifdef DEE_LIMITED_API
struct _DeeHashMapNode {
 struct _DeeHashMapNode *hmn_prev; /*< [0..1] Previous node with the same internal hash. */
 struct _DeeHashMapNode *hmn_next; /*< [0..1] Next node with the same internal hash. */
 Dee_hash_t              hmn_hash; /*< Hash of the key (not interned! / == DeeObject_HashEx(hmn_key,DEE_HASH_START)). */
#ifdef __INTELLISENSE__
           DeeObject    *hmn_key;  /*< [1..1][const] key associated with this node. */
           DeeObject    *hmn_item; /*< [1..1] Item associated with the key. */
#else
 DEE_A_REF DeeObject    *hmn_key;  /*< [1..1][const] key associated with this node. */
 DEE_A_REF DeeObject    *hmn_item; /*< [1..1] Item associated with the key. */
#endif
};
#define _DeeHashMapNode_KEY(ob)  (ob)->hmn_key
#define _DeeHashMapNode_ITEM(ob) (ob)->hmn_item

struct DeeHashMapIterator {
 struct _DeeHashMapNode **hmi_buckets_begin; /*< [0..1] Pointer to the first bucket. */
 struct _DeeHashMapNode **hmi_buckets_end;   /*< [0..1] Pointer to the bucket after the last valid. */
 struct _DeeHashMapNode  *hmi_hash_curr;     /*< [0..1] Current node in the current bucket (reachable from hmi_buckets_begin). */
};
extern DEE_A_RET_WUNUSED int DeeHashMapIterator_CompareLo(DEE_A_IN struct DeeHashMapIterator const *lhs, DEE_A_IN struct DeeHashMapIterator const *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_WUNUSED int DeeHashMapIterator_CompareLe(DEE_A_IN struct DeeHashMapIterator const *lhs, DEE_A_IN struct DeeHashMapIterator const *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_WUNUSED int DeeHashMapIterator_CompareEq(DEE_A_IN struct DeeHashMapIterator const *lhs, DEE_A_IN struct DeeHashMapIterator const *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
#define DeeHashMapIterator_CompareNe !DeeHashMapIterator_CompareEq
#define DeeHashMapIterator_CompareGr !DeeHashMapIterator_CompareLe
#define DeeHashMapIterator_CompareGe !DeeHashMapIterator_CompareLo
#endif /* DEE_LIMITED_API */

#ifdef DEE_LIMITED_API
#define DEE_HASHMAP_INITIAL_BUCKET_COUNT 8
struct DeeHashMap {
 // A dict is a vector<double_linked_list<pair<object,object>>>
 //  - The vector grows with the combined sizes of the linked lists
 //  - Any pair can be found in the linked list at vector index 'hash(pair.first) % size(vector)'
 //  - 'bucket' refers to one of the linked lists at index 'bucket % size(vector)'
 //  - 'bucket_count' refers to the size of the vector; aka. 'size(vector)'
 //  - 'map_size' refers to the combined amount of pairs in all linked lists; aka. total amount of pairs
 //  - 'max_load_factor' refers to the relation between 'bucket_count' and 'map_size'
 struct _DeeHashMapNode **hm_valid_buckets_begin; /*< [0..1] First non-empty bucket (hm_buckets+n | n >= 0 && n < hm_bucket_count) */
 struct _DeeHashMapNode **hm_valid_buckets_end;   /*< [0..1] Bucket after the last non-empty bucket (hm_buckets+hm_bucket_count-n | n >= 0 && n < hm_bucket_count) */
 struct _DeeHashMapNode **hm_buckets;             /*< [0..hm_bucket_count][owned] Actual map of buckets (index == hash % hm_bucket_count) */
 Dee_size_t               hm_bucket_count;        /*< Amount of allocated buckets (any 2^n number; based on hm_map_size and hm_max_load_factor) */
 Dee_size_t               hm_map_size;            /*< Amount of existing nodes. */
 double                   hm_max_load_factor;     /*< Load factor (affects map_size vs bucket_count ratio; default: 1.0). */
};
#define DeeHashMap_INIT()                    {NULL,NULL,NULL,0,0,1.0}
#define DeeHashMap_SIZE(ob)                         (ob)->hm_map_size
#define DeeHashMap_BUCKET_COUNT(ob)                 (ob)->hm_bucket_count
#define DeeHashMap_GET_MAX_LOAD_FACTOR(ob)          (ob)->hm_max_load_factor
#define DeeHashMap_SET_MAX_LOAD_FACTOR(ob,v) (void)((ob)->hm_max_load_factor=(v))
#else /* DEE_LIMITED_API */
#define DeeHashMap_SIZE                DeeHashMap_Size
#define DeeHashMap_BUCKET_COUNT        DeeHashMap_BucketCount
#define DeeHashMap_GET_MAX_LOAD_FACTOR DeeHashMap_GetMaxLoadFactor
#define DeeHashMap_SET_MAX_LOAD_FACTOR DeeHashMap_SetMaxLoadFactor
#endif /* !DEE_LIMITED_API */


#ifdef DEE_LIMITED_API
#define DeeHashMap_TRAVERSE_VARS\
 struct _DeeHashMapNode **_trav_buckets_begin,**_trav_buckets_end
#define DeeHashMap_TRAVERSE(node,map)\
 for (_trav_buckets_begin = (map)->hm_valid_buckets_begin,\
      _trav_buckets_end = (map)->hm_valid_buckets_end;\
      _trav_buckets_begin != _trav_buckets_end;)\
  for ((node) = *_trav_buckets_begin++; (node); (node) = (node)->hmn_next)
#define DeeHashMap_TRAVERSE_RANGE_VARS\
 struct _DeeHashMapNode **_trav_buckets_begin,**_trav_buckets_end;\
 Dee_size_t _trav_pre,_trav_n
#define DeeHashMap_TRAVERSE_RANGE(node,map,lo,hi)\
 for (_trav_buckets_begin = (map)->hm_valid_buckets_begin,\
      _trav_buckets_end = (map)->hm_valid_buckets_end,\
      _trav_n = (hi)-(_trav_pre=(lo));\
      _trav_buckets_begin != _trav_buckets_end && _trav_n;)\
  for ((node) = *_trav_buckets_begin++; (node)&&_trav_n; (node) = (node)->hmn_next)\
  if (_trav_pre) --_trav_pre; else if ((--_trav_n,0)){}else
#define DeeHashMap_TRAVERSE_SAFE_VARS\
 struct DeeHashMapIterator _trav_iterator
#define DeeHashMap_TRAVERSE_SAFE(node,map)\
 for (DeeHashMapIterator_Init(&_trav_iterator,map);\
     ((node)=DeeHashMapIterator_Yield(&_trav_iterator,map))!=NULL;)
#endif


//////////////////////////////////////////////////////////////////////////
// Allocate / Free a hash-map
// >> NOTE: Alloc doesn't initialize the object and free doesn't finalize it
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct DeeHashMap *) DeeHashMap_Alloc(void);
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct DeeHashMap *) DeeHashMap_New(void);
DEE_HASHMAP_FUNC_DECL(           void) DeeHashMap_Free(DEE_A_IN struct DeeHashMap *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC void) DeeHashMap_Delete(DEE_A_IN DEE_A_OUT_INVALID struct DeeHashMap *self) DEE_ATTRIBUTE_NONNULL((1));


DEE_HASHMAP_FUNC_DECL(           void) DeeHashMap_Init(DEE_A_OUT struct DeeHashMap *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHMAP_FUNC_DECL(           void) DeeHashMap_InitMove(DEE_A_OUT struct DeeHashMap *self, DEE_A_INOUT struct DeeHashMap *right) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_InitCopy(DEE_A_OUT struct DeeHashMap *self, DEE_A_IN struct DeeHashMap const *right) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_InitDeepCopy(DEE_A_OUT struct DeeHashMap *self, DEE_A_IN struct DeeHashMap const *right) DEE_ATTRIBUTE_NONNULL((1,2));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_InitCopyWithLock(DEE_A_OUT struct DeeHashMap *self, DEE_A_IN struct DeeHashMap const *right, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_InitDeepCopyWithLock(DEE_A_OUT struct DeeHashMap *self, DEE_A_IN struct DeeHashMap const *right, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
#else
#define /*DEE_A_EXEC*/ DeeHashMap_InitCopyWithLock(ob,right,lock)     DeeHashMap_InitCopy(ob,right)
#define /*DEE_A_EXEC*/ DeeHashMap_InitDeepCopyWithLock(ob,right,lock) DeeHashMap_InitDeepCopy(ob,right)
#endif
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_InitFromIterator(DEE_A_OUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,2));
#define DeeHashMap_InitFromSequence(ob,sequence) DeeHashMap_TInitFromSequence(ob,Dee_TYPE(sequence),sequence)
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_TInitFromSequence(DEE_A_OUT struct DeeHashMap *self, DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_InitFromKeyValuePairs(DEE_A_OUT struct DeeHashMap *self, DEE_A_IN Dee_size_t pairc, DEE_A_IN_R(pairc*2) DeeObject *const *pairv) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC void) DeeHashMap_AssignMove(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT struct DeeHashMap *right) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_AssignCopy(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN struct DeeHashMap const *right) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC void) DeeHashMap_Quit(DEE_A_IN DEE_A_OUT_INVALID struct DeeHashMap *self) DEE_ATTRIBUTE_NONNULL((1));
#define /* DEE_A_EXEC int */DeeHashMap_InsertSequence(ob,sequence)              DeeHashMap_TInsertSequence(ob,Dee_TYPE(sequence),sequence)
#define /* DEE_A_EXEC int */DeeHashMap_InsertSequenceWithLock(ob,sequence,lock) DeeHashMap_TInsertSequenceWithLock(ob,Dee_TYPE(sequence),sequence,lock)
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_TInsertSequence(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_TInsertSequenceWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_InsertIterator(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_InsertIteratorWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *iterator, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_HashKeys(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_HashItems(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_HashValues(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result) DEE_ATTRIBUTE_NONNULL((1,3));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_HashKeysWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_HashItemsWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_HashValuesWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,3,4));
#else
#define /* DEE_A_EXEC */DeeHashMap_HashKeysWithLock(ob,start,result,lock)   DeeHashMap_HashKeys(ob,start,result)
#define /* DEE_A_EXEC */DeeHashMap_HashItemsWithLock(ob,start,result,lock)  DeeHashMap_HashItems(ob,start,result)
#define /* DEE_A_EXEC */DeeHashMap_HashValuesWithLock(ob,start,result,lock) DeeHashMap_HashValues(ob,start,result)
#endif

DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct DeeHashMapIterator *) DeeHashMapIterator_Alloc(void);
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct DeeHashMapIterator *) DeeHashMapIterator_New(DEE_A_IN struct DeeHashMap const *map) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHMAP_FUNC_DECL(void) DeeHashMapIterator_Free(
 DEE_A_IN DEE_A_OUT_INVALID struct DeeHashMapIterator *self) DEE_ATTRIBUTE_NONNULL((1));
#define DeeHashMapIterator_Quit (void)Dee_REQUIRES_POINTER
#define DeeHashMapIterator_Delete DeeHashMapIterator_Free
DEE_HASHMAP_FUNC_DECL(void) DeeHashMapIterator_Init(
 DEE_A_OUT struct DeeHashMapIterator *self,
 DEE_A_IN struct DeeHashMap const *map) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(void) DeeHashMapIterator_InitCopy(
 DEE_A_OUT struct DeeHashMapIterator *self,
 DEE_A_IN struct DeeHashMapIterator const *right) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_RET_MAYBE_NULL struct _DeeHashMapNode *) DeeHashMapIterator_Yield(
 DEE_A_INOUT struct DeeHashMapIterator *self, DEE_A_IN struct DeeHashMap const *map) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Pop 'key' from 'self' and return 'def' or throw an 'Error.KeyError' if 'key' wasn't found
//  - Same as dict.pop()
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_Pop(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT_OPT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_PopWithKnownBucket(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT_OPT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_PopString(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_INOUT_OPT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_PopStringWithKnownBucket(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT_OPT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_PopWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT_OPT DeeObject *def, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_PopWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT_OPT DeeObject *def, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,5));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_PopStringWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_INOUT_OPT DeeObject *def, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_PopStringWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT_OPT DeeObject *def, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,5));
#else
#define /*DEE_A_EXEC*/ DeeHashMap_PopWithLock(ob,key,def,lock)                            DeeHashMap_Pop(ob,key,def)
#define /*DEE_A_EXEC*/ DeeHashMap_PopWithKnownBucketAndLock(ob,key,bucket,def,lock)       DeeHashMap_PopWithKnownBucket(ob,key,bucket,def)
#define /*DEE_A_EXEC*/ DeeHashMap_PopStringWithLock(ob,key,def,lock)                      DeeHashMap_PopString(ob,key,def)
#define /*DEE_A_EXEC*/ DeeHashMap_PopStringWithKnownBucketAndLock(ob,key,bucket,def,lock) DeeHashMap_PopStringWithKnownBucket(ob,key,bucket,def)
#endif

//////////////////////////////////////////////////////////////////////////
// Returns the item associated with 'key' or sets the 'item' to 'def' if 'key' didn't exist
//  - Same as dict.setdefault()
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_SetDefault(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_SetDefaultWithKnownBucket(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_SetDefaultString(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_SetDefaultStringWithKnownBucket(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,4));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_SetDefaultWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *def, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_SetDefaultWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_SetDefaultStringWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *def, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_SetDefaultStringWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
#else
#define /*DEE_A_EXEC*/ DeeHashMap_SetDefaultWithLock(ob,key,def,lock)                            DeeHashMap_SetDefault(ob,key,def)
#define /*DEE_A_EXEC*/ DeeHashMap_SetDefaultWithKnownBucketAndLock(ob,key,bucket,def,lock)       DeeHashMap_SetDefaultWithKnownBucket(ob,key,bucket,def)
#define /*DEE_A_EXEC*/ DeeHashMap_SetDefaultStringWithLock(ob,key,def,lock)                      DeeHashMap_SetDefaultString(ob,key,def)
#define /*DEE_A_EXEC*/ DeeHashMap_SetDefaultStringWithKnownBucketAndLock(ob,key,bucket,def,lock) DeeHashMap_SetDefaultStringWithKnownBucket(ob,key,bucket,def)
#endif

//////////////////////////////////////////////////////////////////////////
// Returns the item associated with 'key' or 'def' if 'key' didn't exist
//  - Same as dict.get()
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_GetDefault(DEE_A_INOUT struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_GetDefaultWithKnownBucket(DEE_A_INOUT struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_GetDefaultString(DEE_A_INOUT struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_GetDefaultStringWithKnownBucket(DEE_A_INOUT struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def) DEE_ATTRIBUTE_NONNULL((1,2,4));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_GetDefaultWithLock(DEE_A_INOUT struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *def, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_GetDefaultWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_GetDefaultStringWithLock(DEE_A_INOUT struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *def, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_GetDefaultStringWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
#else
#define /*DEE_A_EXEC*/ DeeHashMap_GetDefaultWithLock(ob,key,def,lock)                            DeeHashMap_GetDefault(ob,key,def)
#define /*DEE_A_EXEC*/ DeeHashMap_GetDefaultWithKnownBucketAndLock(ob,key,bucket,def,lock)       DeeHashMap_GetDefaultWithKnownBucket(ob,key,bucket,def)
#define /*DEE_A_EXEC*/ DeeHashMap_GetDefaultStringWithLock(ob,key,def,lock)                      DeeHashMap_GetDefaultString(ob,key,def)
#define /*DEE_A_EXEC*/ DeeHashMap_GetDefaultStringWithKnownBucketAndLock(ob,key,bucket,def,lock) DeeHashMap_GetDefaultStringWithKnownBucket(ob,key,bucket,def)
#endif

//////////////////////////////////////////////////////////////////////////
// Pop some key-item pair from 'self' and store it in '*key' and '*item'
// Throws an 'Error.ValueError' if 'self' is empty
//  - Same as dict.popitem()
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_PopSomething(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_REF DEE_A_OUT DeeObject **key,
 DEE_A_REF DEE_A_OUT DeeObject **item) DEE_ATTRIBUTE_NONNULL((1));

DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindKeyIndex(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindKeyIndexWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindKeyIndexString(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindKeyIndexStringWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindItemIndex(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindKeyItemIndex(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindKeyItemIndexWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindKeyItemIndexString(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindKeyItemIndexStringWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexKeyIndex(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexKeyIndexWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexKeyIndexString(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexKeyIndexStringWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexItemIndex(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexKeyItemIndex(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexKeyItemIndexWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexKeyItemIndexString(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexKeyItemIndexStringWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_CountItem(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2));

#ifndef DEE_WITHOUT_THREADS
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindKeyIndexWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindKeyIndexWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindKeyIndexStringWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindKeyIndexStringWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindItemIndexWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindKeyItemIndexWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindKeyItemIndexWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindKeyItemIndexStringWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeHashMap_FindKeyItemIndexStringWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexKeyIndexWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexKeyIndexWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexKeyIndexStringWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexKeyIndexStringWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexItemIndexWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexKeyItemIndexWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexKeyItemIndexWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexKeyItemIndexStringWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_IndexKeyItemIndexStringWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeHashMap_CountItemWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
#else
#define /*DEE_A_EXEC*/ DeeHashMap_FindKeyIndexWithLock(ob,key,lock)                                      DeeHashMap_FindKeyIndex(ob,key)
#define /*DEE_A_EXEC*/ DeeHashMap_FindKeyIndexWithKnownBucketAndLock(ob,key,bucket,lock)                 DeeHashMap_FindKeyIndexWithKnownBucket(ob,key,bucket)
#define /*DEE_A_EXEC*/ DeeHashMap_FindKeyIndexStringWithLock(ob,key,lock)                                DeeHashMap_FindKeyIndexString(ob,key)
#define /*DEE_A_EXEC*/ DeeHashMap_FindKeyIndexStringWithKnownBucketAndLock(ob,key,bucket,lock)           DeeHashMap_FindKeyIndexStringWithKnownBucket(ob,key,bucket)
#define /*DEE_A_EXEC*/ DeeHashMap_FindItemIndexWithLock(ob,item,lock)                                    DeeHashMap_FindItemIndex(ob,item)
#define /*DEE_A_EXEC*/ DeeHashMap_FindKeyItemIndexWithLock(ob,key,item,lock)                             DeeHashMap_FindKeyItemIndex(ob,key,item)
#define /*DEE_A_EXEC*/ DeeHashMap_FindKeyItemIndexWithKnownBucketAndLock(ob,key,bucket,item,lock)        DeeHashMap_FindKeyItemIndexWithKnownBucket(ob,key,bucket,item)
#define /*DEE_A_EXEC*/ DeeHashMap_FindKeyItemIndexStringWithLock(ob,key,item,lock)                       DeeHashMap_FindKeyItemIndexString(ob,key,item)
#define /*DEE_A_EXEC*/ DeeHashMap_FindKeyItemIndexStringWithKnownBucketAndLock(ob,key,bucket,item,lock)  DeeHashMap_FindKeyItemIndexStringWithKnownBucket(ob,key,bucket,item)
#define /*DEE_A_EXEC*/ DeeHashMap_IndexKeyIndexWithLock(ob,key,lock)                                     DeeHashMap_IndexKeyIndex(ob,key)
#define /*DEE_A_EXEC*/ DeeHashMap_IndexKeyIndexWithKnownBucketAndLock(ob,key,bucket,lock)                DeeHashMap_IndexKeyIndexWithKnownBucket(ob,key,bucket)
#define /*DEE_A_EXEC*/ DeeHashMap_IndexKeyIndexStringWithLock(ob,key,lock)                               DeeHashMap_IndexKeyIndexString(ob,key)
#define /*DEE_A_EXEC*/ DeeHashMap_IndexKeyIndexStringWithKnownBucketAndLock(ob,key,bucket,lock)          DeeHashMap_IndexKeyIndexStringWithKnownBucket(ob,key,bucket)
#define /*DEE_A_EXEC*/ DeeHashMap_IndexItemIndexWithLock(ob,item,lock)                                   DeeHashMap_IndexItemIndex(ob,item)
#define /*DEE_A_EXEC*/ DeeHashMap_IndexKeyItemIndexWithLock(ob,key,item,lock)                            DeeHashMap_IndexKeyItemIndex(ob,key,item)
#define /*DEE_A_EXEC*/ DeeHashMap_IndexKeyItemIndexWithKnownBucketAndLock(ob,key,bucket,item,lock)       DeeHashMap_IndexKeyItemIndexWithKnownBucket(ob,key,bucket,item)
#define /*DEE_A_EXEC*/ DeeHashMap_IndexKeyItemIndexStringWithLock(ob,key,item,lock)                      DeeHashMap_IndexKeyItemIndexString(ob,key,item)
#define /*DEE_A_EXEC*/ DeeHashMap_IndexKeyItemIndexStringWithKnownBucketAndLock(ob,key,bucket,item,lock) DeeHashMap_IndexKeyItemIndexStringWithKnownBucket(ob,key,bucket,item)
#define /*DEE_A_EXEC*/ DeeHashMap_CountItemWithLock(ob,item,lock)                                        DeeHashMap_CountItem(ob,item)
#endif


//////////////////////////////////////////////////////////////////////////
// Returns the item associated with a given key
// It is recommended to always use the '*Ex' or '*WithError' functions to reduce the amount of thrown exceptions
// NOTE: If a *WithError function is used, a KeyError is set if NULL would have been returned without an error
// NOTE: Use the GetItemEx functions to always get informed if an error occurred or the item could not be found (because GetItem can return NULL without an error)
// NOTE: NotImplemented Errors in compare functions are always ignored and count as not_equal
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_GetItemEx(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_REF DEE_A_OUT DeeObject **result) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_GetItemExWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_REF DEE_A_OUT DeeObject **result) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_GetItemExString(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_REF DEE_A_OUT DeeObject **result) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_GetItemExStringWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_REF DEE_A_OUT DeeObject **result) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_GetItem(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_GetItemWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_GetItemString(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_GetItemStringWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_GetItemExWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_OUT DEE_A_REF DeeObject **result, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_GetItemExWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_OUT DEE_A_REF DeeObject **result, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_GetItemExStringWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_OUT DEE_A_REF DeeObject **result, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_GetItemExStringWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_OUT DEE_A_REF DeeObject **result, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_GetItemWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_GetItemWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_GetItemStringWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_GetItemStringWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
#else
#define /*DEE_A_EXEC*/ DeeHashMap_GetItemExWithLock(ob,key,result,lock)                            DeeHashMap_GetItemEx(ob,key,result)
#define /*DEE_A_EXEC*/ DeeHashMap_GetItemExWithKnownBucketAndLock(ob,key,bucket,result,lock)       DeeHashMap_GetItemExWithKnownBucket(ob,key,bucket,result)
#define /*DEE_A_EXEC*/ DeeHashMap_GetItemExStringWithLock(ob,key,result,lock)                      DeeHashMap_GetItemExString(ob,key,result)
#define /*DEE_A_EXEC*/ DeeHashMap_GetItemExStringWithKnownBucketAndLock(ob,key,bucket,result,lock) DeeHashMap_GetItemExStringWithKnownBucket(ob,key,bucket,result)
#define /*DEE_A_EXEC*/ DeeHashMap_GetItemWithLock(ob,key,lock)                                     DeeHashMap_GetItem(ob,key)
#define /*DEE_A_EXEC*/ DeeHashMap_GetItemWithKnownBucketAndLock(ob,key,bucket,lock)                DeeHashMap_GetItemWithKnownBucket(ob,key,bucket)
#define /*DEE_A_EXEC*/ DeeHashMap_GetItemStringWithLock(ob,key,lock)                               DeeHashMap_GetItemString(ob,key)
#define /*DEE_A_EXEC*/ DeeHashMap_GetItemStringWithKnownBucketAndLock(ob,key,bucket,lock)          DeeHashMap_GetItemStringWithKnownBucket(ob,key,bucket)
#endif

//////////////////////////////////////////////////////////////////////////
// Checks for the existence of a key / item / key-item pair
// >> Returns -1: On error (caused if 'DeeObject_HashEx' throws)
// >> Returns  0: Key doesn't exist
// >> Returns  1: Key does exist
// NOTE: NotImplemented Errors in compare functions are always ignored and count as not_equal
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasKey(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasKeyWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasKeyString(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasKeyStringWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasItem(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasKeyItem(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasKeyItemWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasKeyItemString(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasKeyItemStringWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,4));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasKeyWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasKeyWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasKeyStringWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasKeyStringWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasItemWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasKeyItemWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasKeyItemWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasKeyItemStringWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeHashMap_HasKeyItemStringWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
#else
#define /*DEE_A_EXEC*/ DeeHashMap_HasKeyWithLock(ob,key,lock)                                     DeeHashMap_HasKey(ob,key)
#define /*DEE_A_EXEC*/ DeeHashMap_HasKeyWithKnownBucketAndLock(ob,key,bucket,lock)                DeeHashMap_HasKeyWithKnownBucket(ob,key,bucket)
#define /*DEE_A_EXEC*/ DeeHashMap_HasKeyStringWithLock(ob,key,lock)                               DeeHashMap_HasKeyString(ob,key)
#define /*DEE_A_EXEC*/ DeeHashMap_HasKeyStringWithKnownBucketAndLock(ob,key,bucket,lock)          DeeHashMap_HasKeyStringWithKnownBucket(ob,key,bucket)
#define /*DEE_A_EXEC*/ DeeHashMap_HasItemWithLock(ob,item,lock)                                   DeeHashMap_HasItem(ob,item)
#define /*DEE_A_EXEC*/ DeeHashMap_HasKeyItemWithLock(ob,key,item,lock)                            DeeHashMap_HasKeyItem(ob,key,item)
#define /*DEE_A_EXEC*/ DeeHashMap_HasKeyItemWithKnownBucketAndLock(ob,key,bucket,item,lock)       DeeHashMap_HasKeyItemWithKnownBucket(ob,key,bucket,item)
#define /*DEE_A_EXEC*/ DeeHashMap_HasKeyItemStringWithLock(ob,key,item,lock)                      DeeHashMap_HasKeyItemString(ob,key,item)
#define /*DEE_A_EXEC*/ DeeHashMap_HasKeyItemStringWithKnownBucketAndLock(ob,key,bucket,item,lock) DeeHashMap_HasKeyItemStringWithKnownBucket(ob,key,bucket,item)
#endif

//////////////////////////////////////////////////////////////////////////
// Deletes a key
// >> Returns -1: On error (reallocation failure / compare exception)
// >> Returns  0: On success
// >> Returns  1: Key not found (no error set; never returned if *WithError is used)
// NOTE: If a *WithError function is used, -1 will be returned instead of 1, alongside raising a KeyError
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_DelItem(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_DelItemWithKnownBucket(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_DelItemString(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_DelItemStringWithKnownBucket(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_DelItemWithError(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_DelItemWithErrorAndKnownBucket(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_DelItemStringWithError(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_DelItemStringWithErrorAndKnownBucket(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_DelItemWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_DelItemWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_DelItemStringWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_DelItemStringWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_DelItemWithErrorAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_DelItemWithErrorAndKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_DelItemStringWithErrorAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_DelItemStringWithErrorAndKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
#else
#define /*DEE_A_EXEC*/ DeeHashMap_DelItemWithLock(ob,key,lock)                                    DeeHashMap_DelItem(ob,key)
#define /*DEE_A_EXEC*/ DeeHashMap_DelItemWithKnownBucketAndLock(ob,key,bucket,lock)               DeeHashMap_DelItemWithKnownBucket(ob,key,bucket)
#define /*DEE_A_EXEC*/ DeeHashMap_DelItemStringWithLock(ob,key,lock)                              DeeHashMap_DelItemString(ob,key)
#define /*DEE_A_EXEC*/ DeeHashMap_DelItemStringWithKnownBucketAndLock(ob,key,bucket,lock)         DeeHashMap_DelItemStringWithKnownBucket(ob,key,bucket)
#define /*DEE_A_EXEC*/ DeeHashMap_DelItemWithErrorAndLock(ob,key,lock)                            DeeHashMap_DelItemWithError(ob,key)
#define /*DEE_A_EXEC*/ DeeHashMap_DelItemWithErrorAndKnownBucketAndLock(ob,key,bucket,lock)       DeeHashMap_DelItemWithErrorAndKnownBucket(ob,key,bucket)
#define /*DEE_A_EXEC*/ DeeHashMap_DelItemStringWithErrorAndLock(ob,key,lock)                      DeeHashMap_DelItemStringWithError(ob,key)
#define /*DEE_A_EXEC*/ DeeHashMap_DelItemStringWithErrorAndKnownBucketAndLock(ob,key,bucket,lock) DeeHashMap_DelItemStringWithErrorAndKnownBucket(ob,key,bucket)
#endif

//////////////////////////////////////////////////////////////////////////
// Sets an item
// >> Returns -1: On error
// >> Returns  0: On success
// >> Returns  1: Key already existed (only returned by DeeDict_SetNewItem*; doesn't set an error)
// NOTE: DeeDict_SetItem* will overwrite existing items
// NOTE: DeeDict_SetNewItem* will not overwrite existing items
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_SetItem(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_SetItemWithKnownBucket(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_SetItemString(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_SetItemStringWithKnownBucket(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_SetNewItem(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_SetNewItemWithKnownBucket(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_SetNewItemString(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_SetNewItemStringWithKnownBucket(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_SetOldItem(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_SetOldItemWithKnownBucket(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_SetOldItemString(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_SetOldItemStringWithKnownBucket(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) DEE_ATTRIBUTE_NONNULL((1,2,4));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_SetItemWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_SetItemWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_SetItemStringWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_SetItemStringWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
// The following functions return -1 on error, 0 if the old item was overwritten (and either stored in 'old_item' or decref'ed) or 1, if the key didn't exist
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_SetOldItemWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,5));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_SetOldItemWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,6));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_SetOldItemStringWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,5));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_SetOldItemStringWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,6));
// The following functions return -1 on error, 0 if the new item was set, or 1 if an old item existed (a reference to which is now stored in 'old_item', if that was set)
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_SetNewItemWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,5));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_SetNewItemWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,6));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_SetNewItemStringWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,5));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_SetNewItemStringWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,6));
#else
#define /*DEE_A_EXEC*/ DeeHashMap_SetItemWithLock(ob,key,item,lock)                                        DeeHashMap_SetItem(ob,key,item)
#define /*DEE_A_EXEC*/ DeeHashMap_SetItemWithKnownBucketAndLock(ob,key,bucket,item,lock)                   DeeHashMap_SetItemWithKnownBucket(ob,key,bucket,item)
#define /*DEE_A_EXEC*/ DeeHashMap_SetItemStringWithLock(ob,key,item,lock)                                  DeeHashMap_SetItemString(ob,key,item)
#define /*DEE_A_EXEC*/ DeeHashMap_SetItemStringWithKnownBucketAndLock(ob,key,bucket,item,lock)             DeeHashMap_SetItemStringWithKnownBucket(ob,key,bucket,item)
#define /*DEE_A_EXEC*/ DeeHashMap_SetOldItemWithLock(ob,key,item,old_item,lock)                            DeeHashMap_SetOldItem(ob,key,item,old_item)
#define /*DEE_A_EXEC*/ DeeHashMap_SetOldItemWithKnownBucketAndLock(ob,key,bucket,item,old_item,lock)       DeeHashMap_SetOldItemWithKnownBucket(ob,key,bucket,item,old_item)
#define /*DEE_A_EXEC*/ DeeHashMap_SetOldItemStringWithLock(ob,key,item,old_item,lock)                      DeeHashMap_SetOldItemString(ob,key,item,old_item)
#define /*DEE_A_EXEC*/ DeeHashMap_SetOldItemStringWithKnownBucketAndLock(ob,key,bucket,item,old_item,lock) DeeHashMap_SetOldItemStringWithKnownBucket(ob,key,bucket,item,old_item)
#define /*DEE_A_EXEC*/ DeeHashMap_SetNewItemWithLock(ob,key,item,old_item,lock)                            DeeHashMap_SetNewItem(ob,key,item,old_item)
#define /*DEE_A_EXEC*/ DeeHashMap_SetNewItemWithKnownBucketAndLock(ob,key,bucket,item,old_item,lock)       DeeHashMap_SetNewItemWithKnownBucket(ob,key,bucket,item,old_item)
#define /*DEE_A_EXEC*/ DeeHashMap_SetNewItemStringWithLock(ob,key,item,old_item,lock)                      DeeHashMap_SetNewItemString(ob,key,item,old_item)
#define /*DEE_A_EXEC*/ DeeHashMap_SetNewItemStringWithKnownBucketAndLock(ob,key,bucket,item,old_item,lock) DeeHashMap_SetNewItemStringWithKnownBucket(ob,key,bucket,item,old_item)
#endif

//////////////////////////////////////////////////////////////////////////
// Index-based dict functions (used to simulate array operators for proxy types)
// NOTE: Requires "index < DeeHashMap_SIZE(self)" / "lo <= hi <= DeeHashMap_SIZE(self)"
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_NEVER_NULL DeeObject *) DeeHashMap_GetKeyIndex(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_size_t index) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_NEVER_NULL DeeObject *) DeeHashMap_GetItemIndex(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_size_t index) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeHashMap_GetValueIndex(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_size_t index) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeHashMap_GetKeyRangeIndex(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeHashMap_GetItemRangeIndex(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeHashMap_GetValueRangeIndex(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_DelItemIndex(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN Dee_ssize_t index) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_DelItemRangeIndex(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_EXEC void) DeeHashMap_SetItemIndex(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN Dee_size_t index, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_REF DeeObject *) DeeHashMap_ExchangeItemIndex(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN Dee_size_t index, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,3));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeHashMap_DelItemIndexWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN Dee_ssize_t index, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_DelItemRangeIndexWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,4));
#else
#define /*DEE_A_EXEC*/ DeeHashMap_DelItemIndexWithLock(ob,index,lock)      DeeHashMap_DelItemIndex(ob,index)
#define /*DEE_A_EXEC*/ DeeHashMap_DelItemRangeIndexWithLock(ob,lo,hi,lock) DeeHashMap_DelItemRangeIndex(ob,lo,hi)
#endif

//////////////////////////////////////////////////////////////////////////
// HashMap functionality
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC void) DeeHashMap_Clear(DEE_A_INOUT struct DeeHashMap *self) DEE_ATTRIBUTE_NONNULL((1));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC void) DeeHashMap_ClearWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2));
#else
#define /*DEE_A_EXEC*/ DeeHashMap_ClearWithLock(ob,lock) DeeHashMap_Clear(ob)
#endif
DEE_HASHMAP_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeHashMap_Size(DEE_A_IN struct DeeHashMap const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHMAP_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeHashMap_BucketSize(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_size_t bucket) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHMAP_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeHashMap_BucketCount(DEE_A_IN struct DeeHashMap const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// HashMap max_load_factor access
DEE_HASHMAP_FUNC_DECL(DEE_A_RET_WUNUSED double) DeeHashMap_GetMaxLoadFactor(DEE_A_IN struct DeeHashMap const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHMAP_FUNC_DECL(void) DeeHashMap_SetMaxLoadFactor(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN double v) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Traverse a hash-map
//  - If the "trav" function returns a non-zero value, the iteration stops and returns that value
//  - Otherwise the function will call "trav" for all its key-item pairs and return 0
// NOTE: 'DeeHashMap_Traverse' never throws an exception on its own; It only yields what your function does
typedef DEE_A_RET_EXCEPT(-1) int (*DeeHashMapTraverseFunc)(DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item, DEE_A_IN_OPT void *closure) /*DEE_ATTRIBUTE_NONNULL((1,2))*/;
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC_IF((*trav)(...,closure)) DEE_A_RET_EXCEPT(-1) int) DeeHashMap_Traverse(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN DeeHashMapTraverseFunc trav, DEE_A_IN_OPT void *closure) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC_IF((*trav)(...,closure)) DEE_A_RET_EXCEPT(-1) int) DeeHashMap_TraverseKeys(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN DeeObjectIterateFunc trav, DEE_A_IN_OPT void *closure) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC_IF((*trav)(...,closure)) DEE_A_RET_EXCEPT(-1) int) DeeHashMap_TraverseItems(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN DeeObjectIterateFunc trav, DEE_A_IN_OPT void *closure) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC_IF((*trav)(...,closure)) DEE_A_RET_EXCEPT(-1) int) DeeHashMap_TraverseValues(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN DeeObjectIterateFunc trav, DEE_A_IN_OPT void *closure) DEE_ATTRIBUTE_NONNULL((1,2));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_TraverseWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN DeeHashMapTraverseFunc trav, DEE_A_IN_OPT void *closure, DEE_A_INOUT_LOCKED struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_TraverseKeysWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN DeeObjectIterateFunc trav, DEE_A_IN_OPT void *closure, DEE_A_INOUT_LOCKED struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_TraverseItemsWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN DeeObjectIterateFunc trav, DEE_A_IN_OPT void *closure, DEE_A_INOUT_LOCKED struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_TraverseValuesWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN DeeObjectIterateFunc trav, DEE_A_IN_OPT void *closure, DEE_A_INOUT_LOCKED struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
#else
#define /*DEE_A_EXEC_IF((*trav)(...,closure))*/ DeeHashMap_TraverseWithLock(ob,trav,closure,lock)       DeeHashMap_Traverse(ob,trav,closure)
#define /*DEE_A_EXEC_IF((*trav)(...,closure))*/ DeeHashMap_TraverseKeysWithLock(ob,trav,closure,lock)   DeeHashMap_TraverseKeys(ob,trav,closure)
#define /*DEE_A_EXEC_IF((*trav)(...,closure))*/ DeeHashMap_TraverseItemsWithLock(ob,trav,closure,lock)  DeeHashMap_TraverseItems(ob,trav,closure)
#define /*DEE_A_EXEC_IF((*trav)(...,closure))*/ DeeHashMap_TraverseValuesWithLock(ob,trav,closure,lock) DeeHashMap_TraverseValues(ob,trav,closure)
#endif

//////////////////////////////////////////////////////////////////////////
// Locate functions for HashMaps
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_LocateKey(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_LocateItem(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeHashMap_LocateValue(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_LocateAll(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT struct DeeHashMap *dst, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeHashMap_LocateAllKeys(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeHashMap_LocateAllItems(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeHashMap_LocateAllValues(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));

#ifndef DEE_WITHOUT_THREADS
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_LocateKeyWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeHashMap_LocateItemWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) DeeHashMap_LocateValueWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_LocateAllWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT struct DeeHashMap *dst, DEE_A_INOUT DeeObject *pred, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeHashMap_LocateAllKeysWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeHashMap_LocateAllItemsWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeHashMap_LocateAllValuesWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
#else
#define /*DEE_A_EXEC*/ DeeHashMap_LocateKeyWithLock(ob,pred,lock)       DeeHashMap_LocateKey(ob,pred)
#define /*DEE_A_EXEC*/ DeeHashMap_LocateItemWithLock(ob,pred,lock)      DeeHashMap_LocateItem(ob,pred)
#define /*DEE_A_EXEC*/ DeeHashMap_LocateValueWithLock(ob,pred,lock)     DeeHashMap_LocateValue(ob,pred)
#define /*DEE_A_EXEC*/ DeeHashMap_LocateAllWithLock(ob,dst,pred,lock)   DeeHashMap_LocateAll(ob,dst,pred)
#define /*DEE_A_EXEC*/ DeeHashMap_LocateAllKeysWithLock(ob,pred,lock)   DeeHashMap_LocateAllKeys(ob,pred)
#define /*DEE_A_EXEC*/ DeeHashMap_LocateAllItemsWithLock(ob,pred,lock)  DeeHashMap_LocateAllItems(ob,pred)
#define /*DEE_A_EXEC*/ DeeHashMap_LocateAllValuesWithLock(ob,pred,lock) DeeHashMap_LocateAllValues(ob,pred)
#endif

//////////////////////////////////////////////////////////////////////////
// Transform functions for HashMaps
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_Transform(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT struct DeeHashMap *dst, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeHashMap_TransformKeys(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeHashMap_TransformItems(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeHashMap_TransformValues(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
#ifndef DEE_WITHOUT_THREADS
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeHashMap_TransformWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT struct DeeHashMap *dst, DEE_A_INOUT DeeObject *pred, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeHashMap_TransformKeysWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeHashMap_TransformItemsWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_HASHMAP_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeHashMap_TransformValuesWithLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
#else
#define /*DEE_A_EXEC*/ DeeHashMap_TransformWithLock(ob,dst,pred,lock)   DeeHashMap_Transform(ob,dst,pred)
#define /*DEE_A_EXEC*/ DeeHashMap_TransformKeysWithLock(ob,pred,lock)   DeeHashMap_TransformKeys(ob,pred)
#define /*DEE_A_EXEC*/ DeeHashMap_TransformItemsWithLock(ob,pred,lock)  DeeHashMap_TransformItems(ob,pred)
#define /*DEE_A_EXEC*/ DeeHashMap_TransformValuesWithLock(ob,pred,lock) DeeHashMap_TransformValues(ob,pred)
#endif

//////////////////////////////////////////////////////////////////////////
// Front/Back functions for HashMaps
DEE_HASHMAP_FUNC_DECL(void) DeeHashMap_Front(DEE_A_IN struct DeeHashMap const *self, DEE_A_OUT_OPT DeeObject **key, DEE_A_OUT_OPT Dee_hash_t *bucket, DEE_A_OUT_OPT DeeObject **item) DEE_ATTRIBUTE_NONNULL((1));
DEE_HASHMAP_FUNC_DECL(void) DeeHashMap_Back(DEE_A_IN struct DeeHashMap const *self, DEE_A_OUT_OPT DeeObject **key, DEE_A_OUT_OPT Dee_hash_t *bucket, DEE_A_OUT_OPT DeeObject **item) DEE_ATTRIBUTE_NONNULL((1));


#ifdef DEE_LIMITED_API
// Internal node access functions (limited API)
extern DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeHashMap_GetNodeExWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_OUT struct _DeeHashMapNode **result) DEE_ATTRIBUTE_NONNULL((1,2,4));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeHashMap_GetNodeExStringWithKnownBucket(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_OUT struct _DeeHashMapNode **result) DEE_ATTRIBUTE_NONNULL((1,2,4));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct _DeeHashMapNode *_DeeHashMap_GetOrCreateNode(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct _DeeHashMapNode *_DeeHashMap_GetOrCreateNodeString(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct _DeeHashMapNode *_DeeHashMap_GetOrCreateNodeWithKnownBucket(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct _DeeHashMapNode *_DeeHashMap_GetOrCreateNodeStringWithKnownBucket(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) DEE_ATTRIBUTE_NONNULL((1,2));
extern            DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL struct _DeeHashMapNode *_DeeHashMap_GetNodeIndex(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_size_t index) DEE_ATTRIBUTE_NONNULL((1));
extern            DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL struct _DeeHashMapNode *_DeeHashMap_FrontNode(DEE_A_IN struct DeeHashMap const *self) DEE_ATTRIBUTE_NONNULL((1));
extern            DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL struct _DeeHashMapNode *_DeeHashMap_BackNode(DEE_A_IN struct DeeHashMap const *self) DEE_ATTRIBUTE_NONNULL((1));
#ifndef DEE_WITHOUT_THREADS
extern DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeHashMap_GetNodeExWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_OUT struct _DeeHashMapNode **result, DEE_A_INOUT_LOCKED struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeHashMap_GetNodeExStringWithKnownBucketAndLock(DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_OUT struct _DeeHashMapNode **result, DEE_A_INOUT_LOCKED struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct _DeeHashMapNode *_DeeHashMap_GetOrCreateNodeWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT_LOCKED struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct _DeeHashMapNode *_DeeHashMap_GetOrCreateNodeStringWithLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_INOUT_LOCKED struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct _DeeHashMapNode *_DeeHashMap_GetOrCreateNodeWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT_LOCKED struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct _DeeHashMapNode *_DeeHashMap_GetOrCreateNodeStringWithKnownBucketAndLock(DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT_LOCKED struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,4));
#else
#define /*DEE_A_EXEC*/ _DeeHashMap_GetNodeExWithKnownBucketAndLock(ob,key,bucket,result,lock)       _DeeHashMap_GetNodeExWithKnownBucket(ob,key,bucket,result)
#define /*DEE_A_EXEC*/ _DeeHashMap_GetNodeExStringWithKnownBucketAndLock(ob,key,bucket,result,lock) _DeeHashMap_GetNodeExStringWithKnownBucket(ob,key,bucket,result)
#define /*DEE_A_EXEC*/ _DeeHashMap_GetOrCreateNodeWithLock(ob,key,lock)                             _DeeHashMap_GetOrCreateNode(ob,key)
#define /*DEE_A_EXEC*/ _DeeHashMap_GetOrCreateNodeStringWithLock(ob,key,lock)                       _DeeHashMap_GetOrCreateNodeString(ob,key)
#define /*DEE_A_EXEC*/ _DeeHashMap_GetOrCreateNodeWithKnownBucketAndLock(ob,key,bucket,lock)        _DeeHashMap_GetOrCreateNodeWithKnownBucket(ob,key,bucket)
#define /*DEE_A_EXEC*/ _DeeHashMap_GetOrCreateNodeStringWithKnownBucketAndLock(ob,key,bucket,lock)  _DeeHashMap_GetOrCreateNodeStringWithKnownBucket(ob,key,bucket)
#endif
#endif /* DEE_LIMITED_API */
#undef DEE_HASHMAP_FUNC_DECL
#endif



#ifdef DEE_LIMITED_OPTIMIZATIONS
// Proxy versions of dict functions (implemented as macros, to speedup internal calls)
#define DeeHashMap_GetKeyIndex(ob,index)  _DeeHashMap_GetNodeIndex(ob,index)->hmn_key
#define DeeHashMap_GetItemIndex(ob,index) _DeeHashMap_GetNodeIndex(ob,index)->hmn_item
#endif /* DEE_LIMITED_OPTIMIZATIONS */

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_HASHMAP_H */
