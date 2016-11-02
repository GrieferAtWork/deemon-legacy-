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
#ifndef GUARD_DEEMON_DEX_DEQUE_H_INL
#define GUARD_DEEMON_DEX_DEQUE_H_INL 1
 
#include <deemon/__conf.inl>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/atomic_mutex.h>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPE_OBJECT
DEE_PRIVATE_DECL_DEE_TYPE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif

//////////////////////////////////////////////////////////////////////////
// Debug helper to see uninitialized data appear as NULL (instead of undefined garbage)
#ifdef DEE_DEBUG
#define DEE_DEQUE_DCALLOC_NZ(s) calloc_nz(1,s)
#else
#define DEE_DEQUE_DCALLOC_NZ(s) malloc_nz(s)
#endif

struct DeeDequeBucket {
#ifdef __INTELLISENSE__
           DeeObject **db_elemv; /*< [0..1][:d_bucketsize|0..1][owned] Vector of elements (deref is only valid in full buckets, or if >= begin / < end). */
#else
 DEE_A_REF DeeObject **db_elemv; /*< [0..1][:d_bucketsize|0..1][owned] Vector of elements (deref is only valid in full buckets, or if >= begin / < end). */
#endif
};

#ifndef DEE_DEQUE_DEFUALT_BUCKET_SIZE
#define DEE_DEQUE_DEFUALT_BUCKET_SIZE 16
#endif
struct DeeDeque {
 Dee_size_t             d_elemc;       /*< Amount of elements stored in the deque. */
 Dee_size_t             d_bucketsize;  /*< [!0] Size of any given bucket. */
 Dee_size_t             d_bucketelema; /*< Amount of buckets with allocated elements. */
 struct DeeDequeBucket *d_bucketelemv; /*< [0..d_bucketelema] First bucket with allocated elements. */
 Dee_size_t             d_bucketa;     /*< Amount of allocated buckets. */
 struct DeeDequeBucket *d_bucketroot;  /*< [0..d_bucketa][owned] Allocated bucket root pointer. */
 Dee_size_t             d_bucketc;     /*< Amount of used buckets. */
 struct DeeDequeBucket *d_bucketv;     /*< [0..d_bucketc][in d_bucketroot] First used bucket. */
 DeeObject            **d_begin;       /*< [1..1][0..1][in d_bucketv[0].db_elemv] First used entry in the first used bucket. */
 DeeObject            **d_end;         /*< [?..?][0..1][in d_bucketv[d_bucketc-1].db_elemv] First unused entry in last used bucket (may exceed maximum index of last bucket by 1). */
};
#define DeeDeque_AssertIntegrity(ob)\
(DEE_ASSERTF(ob,"Invalid deque object (NULL pointer)")\
,DEE_ASSERTF(\
  (ob)->d_bucketsize != 0\
,"Broken deque: Invalid bucket size")\
,DEE_ASSERTF(\
   (((ob)->d_elemc != 0) == ((ob)->d_bucketc != 0))\
&& (((ob)->d_elemc != 0) == ((ob)->d_bucketv != NULL))\
&& (((ob)->d_elemc != 0) == ((ob)->d_begin != NULL))\
&& (((ob)->d_elemc != 0) == ((ob)->d_end != NULL))\
&& (((ob)->d_elemc != 0) == ((ob)->d_begin != (ob)->d_end))\
,"Broken deque: Contained pointers contradict each other")\
,DEE_ASSERTF(\
   (((ob)->d_bucketelema != 0) == ((ob)->d_bucketelemv != NULL))\
,"Broken deque: elemv-buffer size contradicts start-pointer")\
,DEE_ASSERTF(\
   (((ob)->d_bucketa != 0) == ((ob)->d_bucketroot != NULL))\
,"Broken deque: allocated bucket size contradicts start-pointer")\
,DEE_ASSERTF(\
  /* Allocated elements requires a valid elemv-cache. */\
  !(ob)->d_elemc || ((ob)->d_bucketelema != 0)\
,"Broken deque: Contains elements, but has no elemv-buffer")\
,DEE_ASSERTF(\
  /* The elemv-buffer range requires a valid bucket-cache. */\
  !(ob)->d_bucketelema || ((ob)->d_bucketa != 0)\
,"Broken deque: Contains an elemv-buffer, but no bucket-buffer")\
,DEE_ASSERTF(\
  !(ob)->d_begin || ((ob)->d_begin >= DeeDeque_FRONT_BUCKET(ob)->db_elemv && \
                     (ob)->d_begin < DeeDeque_FRONT_BUCKET(ob)->db_elemv+(ob)->d_bucketsize)\
,"Broken deque: Begin pointer isn't part of the first bucket")\
,DEE_ASSERTF(\
  !(ob)->d_end || ((ob)->d_end > DeeDeque_BACK_BUCKET(ob)->db_elemv && \
                   (ob)->d_end <= DeeDeque_BACK_BUCKET(ob)->db_elemv+(ob)->d_bucketsize)\
,"Broken deque: End pointer isn't part of the last bucket")\
,DEE_ASSERTF(\
  (ob)->d_bucketc != 1 || ((ob)->d_begin <= (ob)->d_end)\
,"Broken deque: Single-bucket deque's end pointer lies below its start")\
,DEE_ASSERTF(\
  (ob)->d_bucketc <= (ob)->d_bucketelema\
,"Broken deque: More buckets than there are allocated elemv-buckets")\
,DEE_ASSERTF(\
  (ob)->d_bucketelema <= (ob)->d_bucketa\
,"Broken deque: More elemv-buckets than there are allocated ones")\
,DEE_ASSERTF(\
  (ob)->d_bucketelemv >= (ob)->d_bucketroot\
,"Broken deque: elemv-start lies below bucket-root")\
,DEE_ASSERTF(\
  !(ob)->d_bucketv || ((ob)->d_bucketv >= (ob)->d_bucketelemv)\
,"Broken deque: bucket-start lies below elemv-start")\
,DEE_ASSERTF(\
  ((ob)->d_bucketelemv+(ob)->d_bucketelema) <= ((ob)->d_bucketroot+(ob)->d_bucketa)\
,"Broken deque: elemv-end lies aboce bucket-root-end")\
,DEE_ASSERTF(\
  !(ob)->d_bucketv || (((ob)->d_bucketv+(ob)->d_bucketc) <= ((ob)->d_bucketelemv+(ob)->d_bucketelema))\
,"Broken deque: elemv-end lies aboce bucket-root-end")\
)
#ifdef __INTELLISENSE__
// Make this easier for intellisense
#undef DeeDeque_AssertIntegrity
#define DeeDeque_AssertIntegrity(ob) (void)(ob)
#endif

#define DeeDeque_InitFromVector(ob,elemc,elemv) \
 DeeDeque_InitFromVectorEx(ob,DEE_DEQUE_DEFUALT_BUCKET_SIZE,elemc,elemv)
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_InitFromVectorEx(
 DEE_A_OUT struct DeeDeque *self, DEE_A_IN Dee_size_t bucket_size,
 DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(elemc) DeeObject **elemv) DEE_ATTRIBUTE_NONNULL((1));
#define DeeDeque_InitFromSequence(ob,sequence) \
 DeeDeque_InitFromSequenceEx(ob,DEE_DEQUE_DEFUALT_BUCKET_SIZE,sequence)
#define DeeDeque_InitFromSequenceEx(ob,bucket_size,sequence)\
 DeeDeque_TInitFromSequenceEx(ob,bucket_size,Dee_TYPE(sequence),sequence)
#define DeeDeque_TInitFromSequence(ob,tp_sequence,sequence) \
 DeeDeque_TInitFromSequenceEx(ob,DEE_DEQUE_DEFUALT_BUCKET_SIZE,tp_sequence,sequence)
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_TInitFromSequenceEx(
 DEE_A_OUT struct DeeDeque *self, DEE_A_IN Dee_size_t bucket_size,
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,3,4));
#define DeeDeque_InitFromIterator(ob,iterator) \
 DeeDeque_InitFromIteratorEx(ob,DEE_DEQUE_DEFUALT_BUCKET_SIZE,iterator)
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_InitFromIteratorEx(
 DEE_A_OUT struct DeeDeque *self, DEE_A_IN Dee_size_t bucket_size,
 DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,3));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_InitCopy(DEE_A_OUT struct DeeDeque *self, DEE_A_IN struct DeeDeque const *right) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_InitCopyWithLock(DEE_A_OUT struct DeeDeque *self, DEE_A_IN struct DeeDeque const *right, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern void DeeDeque_InitMove(DEE_A_OUT struct DeeDeque *self, DEE_A_INOUT struct DeeDeque *right) DEE_ATTRIBUTE_NONNULL((1,2));
extern void DeeDeque_InitMoveWithLock(DEE_A_OUT struct DeeDeque *self, DEE_A_INOUT struct DeeDeque *right, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern void DeeDeque_Quit(DEE_A_IN struct DeeDeque *self) DEE_ATTRIBUTE_NONNULL((1));
extern void DeeDeque_Clear(DEE_A_INOUT struct DeeDeque *self) DEE_ATTRIBUTE_NONNULL((1));
extern void DeeDeque_ClearWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDeque_Contains(DEE_A_INOUT struct DeeDeque const *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDeque_ContainsWithLock(DEE_A_INOUT struct DeeDeque const *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDeque_ContainsPred(DEE_A_INOUT struct DeeDeque const *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDeque_ContainsPredWithLock(DEE_A_INOUT struct DeeDeque const *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_PushFront(DEE_A_INOUT struct DeeDeque *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_PushFrontWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_PushBack(DEE_A_INOUT struct DeeDeque *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_PushBackWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_PushFrontVector(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(n) DeeObject *const *elemv) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_PushFrontVectorWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(n) DeeObject *const *elemv, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,4));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_PushBackVector(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(n) DeeObject *const *elemv) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_PushBackVectorWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(n) DeeObject *const *elemv, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,4));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeDeque_PopFront(DEE_A_INOUT struct DeeDeque *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeDeque_PopFrontWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeDeque_PopBack(DEE_A_INOUT struct DeeDeque *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeDeque_PopBackWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_Insert(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t i, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,3));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_InsertWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t i, DEE_A_INOUT DeeObject *elem, struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,3,4));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_InsertVector(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t i, DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(n) DeeObject *const *elemv) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_InsertVectorWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t i, DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(n) DeeObject *const *elemv, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,5));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_Erase(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t i, DEE_A_IN Dee_size_t n) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_EraseWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t i, DEE_A_IN Dee_size_t n, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,4));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_EraseReleaseLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t i, DEE_A_IN Dee_size_t n, DEE_A_IN_LOCKED struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,4));
#define DeeDeque_InsertSequence(self,i,sequence)              DeeDeque_TInsertSequence(self,i,Dee_TYPE(sequence),sequence)
#define DeeDeque_InsertSequenceWithLock(self,i,sequence,lock) DeeDeque_TInsertSequenceWithLock(self,i,Dee_TYPE(sequence),sequence,lock)
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_TInsertSequence(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t i, DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,3,4));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_TInsertSequenceWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t i, DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,3,4,5));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_InsertIterator(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t i, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,3));
extern DEE_A_RET_EXCEPT(-1) int DeeDeque_InsertIteratorWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t i, DEE_A_INOUT DeeObject *iterator, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,3,4));
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDeque_Remove(DEE_A_INOUT struct DeeDeque *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDeque_RemoveWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDeque_RemovePred(DEE_A_INOUT struct DeeDeque *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDeque_RemovePredWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
extern DEE_A_RET_EXCEPT(-1) Dee_size_t DeeDeque_RemoveIf(DEE_A_INOUT struct DeeDeque *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) Dee_size_t DeeDeque_RemoveIfWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_INOUT DeeObject *pred, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeDeque_GetRange(DEE_A_INOUT struct DeeDeque const *self, DEE_A_IN Dee_ssize_t begin, DEE_A_IN Dee_ssize_t end) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeDeque_GetRangeWithLock(DEE_A_INOUT struct DeeDeque const *self, DEE_A_IN Dee_ssize_t begin, DEE_A_IN Dee_ssize_t end, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,4));


#define DEE_DEQUE_SHRINKTOFIT_FLAG_ELEMV      DEE_UINT32_C(0x00000001) /*< Free unused elemv-entries. */
#define DEE_DEQUE_SHRINKTOFIT_FLAG_BUCKETS    DEE_UINT32_C(0x00000002) /*< Free unused buckets. */
#define DEE_DEQUE_SHRINKTOFIT_FLAG_SHIFT_ELEM DEE_UINT32_C(0x00000004) /*< Shift elements to free up to one more bucket. */
#define DEE_DEQUE_SHRINKTOFIT_FLAG_DEFAULT \
 (DEE_DEQUE_SHRINKTOFIT_FLAG_ELEMV\
 |DEE_DEQUE_SHRINKTOFIT_FLAG_BUCKETS\
 |DEE_DEQUE_SHRINKTOFIT_FLAG_SHIFT_ELEM)
extern void DeeDeque_ShrinkToFit(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1));
extern void DeeDeque_ShrinkToFitWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_uint32_t flags, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,3));

//////////////////////////////////////////////////////////////////////////
// Shifts the elements in the deque left/right, making the region
// from 'free_start' to 'free_start+free_count' contain invalid object
// that should be filled in by the caller
// NOTE: These functions will not reallocate any buffers, relying on
//       the caller to make sure that enough space is available through
//       previous calls to 'DeeDeque_INC(BUCKET|ELEMV)CACHE_(LEFT|RIGHT)_N'
extern void _DeeDeque_ShiftRight(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t free_start, DEE_A_IN Dee_size_t free_count) DEE_ATTRIBUTE_NONNULL((1));
extern void _DeeDeque_ShiftLeft(DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t free_start, DEE_A_IN Dee_size_t free_count) DEE_ATTRIBUTE_NONNULL((1));

#define DeeDeque_INIT()              DeeDeque_INIT_EX(DEE_DEQUE_DEFUALT_BUCKET_SIZE)
#define DeeDeque_INIT_EX(bucketsize) {0,bucketsize,0,NULL,0,NULL,0,NULL,NULL,NULL}
#define DeeDeque_Init(ob) DeeDeque_InitEx(ob,DEE_DEQUE_DEFUALT_BUCKET_SIZE)
#define DeeDeque_InitEx(ob,bucketsize)\
((ob)->d_elemc = 0,\
 (ob)->d_bucketsize = (bucketsize),\
 DEE_ASSERTF((ob)->d_bucketsize != 0,"Invalid bucketsize"),\
 (ob)->d_bucketelema = 0,\
 (ob)->d_bucketelemv = NULL,\
 (ob)->d_bucketa = 0,\
 (ob)->d_bucketroot = NULL,\
 (ob)->d_bucketc = 0,\
 (ob)->d_bucketv = NULL,\
 (ob)->d_begin = NULL,\
 (ob)->d_end = NULL)
#define DeeDeque_EMPBUCKETCACHE_LEFT(ob)  ((ob)->d_bucketv == (ob)->d_bucketroot)
#define DeeDeque_INCBUCKETCACHE_LEFT(ob,...)\
do{\
 Dee_size_t new_bucket_cachesize,new_bucketc;\
 struct DeeDequeBucket *new_buckets;\
 DEE_ASSERTF((ob)->d_bucketa,"Can't increase size of empty cache");\
 if ((ob)->d_bucketa == (ob)->d_bucketc) {\
  /* All buckets used --> Must really allocate new ones. */\
  if ((new_bucketc = (ob)->d_bucketa/2) == 0) new_bucketc = 1;\
  new_bucket_cachesize = (ob)->d_bucketa+new_bucketc;\
  if DEE_UNLIKELY((new_buckets = (struct DeeDequeBucket *)realloc_nnz(\
   self->d_bucketroot,new_bucket_cachesize*sizeof(struct DeeDequeBucket)\
   )) == NULL) {__VA_ARGS__;}\
  DEE_ASSERTF((ob)->d_bucketv == (ob)->d_bucketroot,"All buckets are used, but start of usage isn't root?");\
  DEE_ASSERTF((ob)->d_bucketelemv == (ob)->d_bucketroot,"Bucket-elemv isn't full, but all buckets are used?");\
  /* Relocate the offsets of the elemv-start and buckets-start. */\
  /* Shift the elemv-buffers to make space on the left */\
  memmove(new_buckets+new_bucketc,new_buckets,\
          (ob)->d_bucketelema*sizeof(struct DeeDequeBucket));\
  (ob)->d_bucketelemv = new_buckets+new_bucketc;\
  (ob)->d_bucketv = new_buckets+new_bucketc;\
  (ob)->d_bucketroot = new_buckets;\
  (ob)->d_bucketa = new_bucket_cachesize;\
 } else {\
  /* Unused buckets available on the right --> Use them. */\
  DEE_ASSERT((ob)->d_bucketelema < (ob)->d_bucketa);\
  DEE_ASSERT((ob)->d_bucketelemv < (ob)->d_bucketroot+((ob)->d_bucketa-(ob)->d_bucketelema));\
  memmove((ob)->d_bucketelemv+1,(ob)->d_bucketelemv,\
          (ob)->d_bucketelema*sizeof(struct DeeDequeBucket));\
  ++(ob)->d_bucketelemv;\
 }\
}while(0)
#define DeeDeque_INCBUCKETCACHE_LEFT_N(ob,min_freelhs,...)\
do{\
 Dee_size_t shift,free_bucketc,front_free,new_bucket_cachesize;\
 struct DeeDequeBucket *new_buckets;\
 DEE_ASSERTF((ob)->d_bucketa,"Can't increase size of empty cache");\
 free_bucketc = (ob)->d_bucketa-(ob)->d_bucketelema;\
 front_free = (Dee_size_t)((ob)->d_bucketelemv-(ob)->d_bucketroot);\
 if ((min_freelhs) > free_bucketc) {\
  /* All buckets used --> Must really allocate new ones. */\
  new_bucket_cachesize = (ob)->d_bucketa+((min_freelhs)-free_bucketc);\
  DEE_ASSERT(new_bucket_cachesize != 0);\
  if DEE_UNLIKELY((new_buckets = (struct DeeDequeBucket *)realloc_nnz(\
   self->d_bucketroot,new_bucket_cachesize*sizeof(struct DeeDequeBucket)\
   )) == NULL) {__VA_ARGS__;}\
  /* Relocate the offsets of the elemv-start and buckets-start. */\
  /* Shift the elemv-buffers to make space on the left */\
  (ob)->d_bucketa = new_bucket_cachesize;\
  (ob)->d_bucketelemv = new_buckets+((ob)->d_bucketelemv-(ob)->d_bucketroot);\
  (ob)->d_bucketv = new_buckets+((ob)->d_bucketv-(ob)->d_bucketroot);\
  (ob)->d_bucketroot = new_buckets;\
  DEE_ASSERT((min_freelhs) > front_free);\
 }\
 if ((min_freelhs) > front_free) {\
  /* Unused buckets available on the right --> Use them. */\
  DEE_ASSERT((ob)->d_bucketelema < (ob)->d_bucketa);\
  DEE_ASSERT((ob)->d_bucketelemv < (ob)->d_bucketroot+((ob)->d_bucketa-(ob)->d_bucketelema));\
  shift = (min_freelhs)-front_free;\
  memmove((ob)->d_bucketelemv+shift,(ob)->d_bucketelemv,\
          (ob)->d_bucketelema*sizeof(struct DeeDequeBucket));\
  (ob)->d_bucketelemv += shift;\
  (ob)->d_bucketv += shift;\
 }\
 DEE_ASSERT((Dee_size_t)((ob)->d_bucketelemv-(ob)->d_bucketroot) >= (min_freelhs));\
}while(0)
#define DeeDeque_EMPBUCKETCACHE_RIGHT(ob) \
 ((ob)->d_bucketv+(ob)->d_bucketc == (ob)->d_bucketroot+(ob)->d_bucketa)
#define DeeDeque_INCBUCKETCACHE_RIGHT(ob,...)\
do{\
 Dee_size_t new_bucket_cachesize,new_bucketc;\
 struct DeeDequeBucket *new_buckets;\
 DEE_ASSERTF((ob)->d_bucketa,"Can't increase size of empty cache");\
 if ((ob)->d_bucketa == (ob)->d_bucketelema) {\
  /* All buckets used --> Must really allocate new ones. */\
  if ((new_bucketc = (ob)->d_bucketa/2) == 0) new_bucketc = 1;\
  new_bucket_cachesize = (ob)->d_bucketa+new_bucketc;\
  if DEE_UNLIKELY((new_buckets = (struct DeeDequeBucket *)realloc_nnz(\
   self->d_bucketroot,new_bucket_cachesize*sizeof(struct DeeDequeBucket)\
   )) == NULL) {__VA_ARGS__;}\
  DEE_ASSERTF((ob)->d_bucketv == (ob)->d_bucketroot,"All buckets are used, but start of usage isn't root?");\
  DEE_ASSERTF((ob)->d_bucketelemv == (ob)->d_bucketroot,"Bucket-elemv isn't full, but all buckets are used?");\
  /* Relocate the offsets of the elemv-start and buckets-start. */\
  (ob)->d_bucketelemv = new_buckets;\
  (ob)->d_bucketv = new_buckets;\
  (ob)->d_bucketroot = new_buckets;\
  (ob)->d_bucketa = new_bucket_cachesize;\
 } else {\
  /* Unused buckets available on the left --> Use them. */\
  DEE_ASSERT((ob)->d_bucketelemv > (ob)->d_bucketroot);\
  memmove((ob)->d_bucketelemv-1,(ob)->d_bucketelemv,\
          (ob)->d_bucketelema*sizeof(struct DeeDequeBucket));\
  --(ob)->d_bucketelemv;\
 }\
 DEE_ASSERT(((ob)->d_bucketroot+(ob)->d_bucketa) > ((ob)->d_bucketelemv+(ob)->d_bucketelema));\
}while(0)
#define DeeDeque_INCBUCKETCACHE_RIGHT_N(ob,min_freerhs,...)\
do{\
 Dee_size_t used_from_left,back_free,free_bucketc,new_bucket_cachesize;\
 struct DeeDequeBucket *new_buckets;\
 DEE_ASSERTF((ob)->d_bucketa,"Can't increase size of empty cache");\
 DEE_ASSERT((ob)->d_bucketa >= (ob)->d_bucketelema);\
 free_bucketc = (ob)->d_bucketa-(ob)->d_bucketelema;\
 back_free = (Dee_size_t)(((ob)->d_bucketroot+(ob)->d_bucketa)-\
                          ((ob)->d_bucketelemv+(ob)->d_bucketelema));\
 DEE_ASSERT(back_free <= free_bucketc);\
 if ((min_freerhs) > free_bucketc) {\
  /* All buckets used --> Must really allocate new ones. */\
  new_bucket_cachesize = (ob)->d_bucketa+((min_freerhs)-free_bucketc);\
  DEE_ASSERT(new_bucket_cachesize != 0);\
  if DEE_UNLIKELY((new_buckets = (struct DeeDequeBucket *)realloc_nnz(\
   self->d_bucketroot,new_bucket_cachesize*sizeof(struct DeeDequeBucket)\
   )) == NULL) {__VA_ARGS__;}\
  /* Relocate the offsets of the elemv-start and buckets-start. */\
  /* Shift the elemv-buffers to make space on the left */\
  (ob)->d_bucketa = new_bucket_cachesize;\
  (ob)->d_bucketelemv = new_buckets+((ob)->d_bucketelemv-(ob)->d_bucketroot);\
  (ob)->d_bucketv = new_buckets+((ob)->d_bucketv-(ob)->d_bucketroot);\
  (ob)->d_bucketroot = new_buckets;\
  back_free = (Dee_size_t)((new_buckets+new_bucket_cachesize)-\
                           ((ob)->d_bucketelemv+(ob)->d_bucketelema));\
 }\
 if ((min_freerhs) > back_free) {\
  /* Unused buckets available on the left --> Use them. */\
  used_from_left = (min_freerhs)-back_free;\
  DEE_ASSERT((ob)->d_bucketelemv-used_from_left >= (ob)->d_bucketroot);\
  memmove((ob)->d_bucketelemv-used_from_left,(ob)->d_bucketelemv,\
          (ob)->d_bucketelema*sizeof(struct DeeDequeBucket));\
  (ob)->d_bucketelemv -= used_from_left;\
 }\
 DEE_ASSERT((Dee_size_t)(((ob)->d_bucketroot+(ob)->d_bucketa)-\
             ((ob)->d_bucketelemv+(ob)->d_bucketelema)) >= (min_freerhs));\
}while(0)

#define DeeDeque_EMPELEMVCACHE_LEFT(ob)  ((ob)->d_bucketv == (ob)->d_bucketelemv)
#define DeeDeque_INCELEMVCACHE_LEFT(ob,...)\
do{\
 DeeObject **new_elemv;\
 DEE_ASSERTF((ob)->d_bucketelemv > (ob)->d_bucketroot,\
             "No bucketcache available; call 'DeeDeque_INCBUCKETCACHE_LEFT' first");\
 if DEE_UNLIKELY((new_elemv = (DeeObject **)DEE_DEQUE_DCALLOC_NZ(\
  (ob)->d_bucketsize*sizeof(DeeObject *))) == NULL) {__VA_ARGS__;}\
 --(ob)->d_bucketelemv;\
 (ob)->d_bucketelemv->db_elemv = new_elemv;\
 ++(ob)->d_bucketelema;\
}while(0)
#define DeeDeque_INCELEMVCACHE_LEFT_N(ob,min_alloclhs,...)\
do{\
 Dee_size_t n_req_alloc,allocated_bucketelemc;\
 DEE_ASSERT((ob)->d_bucketelemv >= (ob)->d_bucketroot);\
 DEE_ASSERTF((Dee_size_t)((ob)->d_bucketelemv-(ob)->d_bucketroot) >= (min_alloclhs),\
             "No bucketcache available; call 'DeeDeque_INCBUCKETCACHE_LEFT_N' first");\
 DEE_ASSERT((ob)->d_bucketelemv >= (ob)->d_bucketroot);\
 DEE_ASSERT((ob)->d_bucketelemv <= (ob)->d_bucketv);\
 allocated_bucketelemc = (Dee_size_t)((ob)->d_bucketv-(ob)->d_bucketelemv);\
 if ((min_alloclhs) > allocated_bucketelemc) {\
  /* Must allocate more unused elemv-buckets */\
  n_req_alloc = (min_alloclhs)-allocated_bucketelemc;\
  DEE_LVERBOSE4("n_req_alloc = %Iu\n",n_req_alloc);\
  DEE_ASSERTF((ob)->d_bucketelemv-n_req_alloc >= (ob)->d_bucketroot,\
              "New elemv-base lies below the bucket root");\
  /* Allocate 'n_req_alloc' new elemv-buckets. */\
  while (n_req_alloc--) {\
   if DEE_UNLIKELY(((ob)->d_bucketelemv[-1].db_elemv = (DeeObject **)\
    DEE_DEQUE_DCALLOC_NZ((ob)->d_bucketsize*sizeof(DeeObject *))) == NULL) {__VA_ARGS__;}\
   --(ob)->d_bucketelemv;\
   ++(ob)->d_bucketelema;\
  }\
 }\
 DEE_ASSERT((Dee_size_t)((ob)->d_bucketv-(ob)->d_bucketelemv) >= (min_alloclhs));\
}while(0)
#define DeeDeque_EMPELEMVCACHE_RIGHT(ob)\
 ((ob)->d_bucketv+(ob)->d_bucketc == (ob)->d_bucketelemv+(ob)->d_bucketelema)
#define DeeDeque_INCELEMVCACHE_RIGHT(ob,...)\
do{\
 DeeObject **new_elemv;\
 DEE_ASSERTF(((ob)->d_bucketelemv+(ob)->d_bucketelema) < ((ob)->d_bucketroot+(ob)->d_bucketa),\
             "No bucketcache available; call 'DeeDeque_INCBUCKETCACHE_RIGHT' first");\
 if DEE_UNLIKELY((new_elemv = (DeeObject **)DEE_DEQUE_DCALLOC_NZ(\
  (ob)->d_bucketsize*sizeof(DeeObject *))) == NULL) {__VA_ARGS__;}\
 (ob)->d_bucketelemv[(ob)->d_bucketelema++].db_elemv = new_elemv;\
}while(0)
#define DeeDeque_INCELEMVCACHE_RIGHT_N(ob,min_allocrhs,...)\
do{\
 Dee_size_t n_req_alloc,allocated_bucketelemc;\
 DEE_ASSERTF(DeeDeque_BACK_FREE_BUCKETC(ob) >= (min_allocrhs),\
             "No bucketcache available; call 'DeeDeque_INCBUCKETCACHE_RIGHT_N' first");\
 DEE_ASSERT((ob)->d_bucketelemv >= (ob)->d_bucketroot);\
 DEE_ASSERT(((ob)->d_bucketroot+(ob)->d_bucketa) >= ((ob)->d_bucketelemv+(ob)->d_bucketelema));\
 allocated_bucketelemc = (Dee_size_t)(\
   ((ob)->d_bucketelemv+(ob)->d_bucketelema)-\
   ((ob)->d_bucketv+(ob)->d_bucketc));\
 if ((min_allocrhs) > allocated_bucketelemc) {\
  /* Must allocate more unused elemv-buckets */\
  n_req_alloc = (min_allocrhs)-allocated_bucketelemc;\
  DEE_ASSERTF((ob)->d_bucketelemv+((ob)->d_bucketelema+n_req_alloc) <=\
             ((ob)->d_bucketroot+(ob)->d_bucketa),\
              "New elemv-base lies above the bucket root-end");\
  /* Allocate 'n_req_alloc' new elemv-buckets. */\
  while (n_req_alloc--) {\
   if DEE_UNLIKELY(((ob)->d_bucketelemv[(ob)->d_bucketelema].db_elemv = (DeeObject **)\
    DEE_DEQUE_DCALLOC_NZ((ob)->d_bucketsize*sizeof(DeeObject *))) == NULL) {__VA_ARGS__;}\
   ++(ob)->d_bucketelema;\
  }\
 }\
 DEE_ASSERT((Dee_size_t)(((ob)->d_bucketelemv+(ob)->d_bucketelema)-\
                         ((ob)->d_bucketv+(ob)->d_bucketc)) >= (min_allocrhs));\
}while(0)
#define DeeDeque_CONSUME_LEFT(ob)\
do{\
 if DEE_UNLIKELY(DeeDeque_FRONT_BUCKET_FULL_NZ(ob)) {\
  DEE_ASSERT((ob)->d_bucketv > (ob)->d_bucketroot);\
  DEE_ASSERT((ob)->d_bucketc > (ob)->d_bucketa);\
  (ob)->d_begin = (--(ob)->d_bucketv)->db_elemv+(ob)->d_bucketsize;\
  ++(ob)->d_bucketc;\
 }\
 --(ob)->d_begin;\
 ++(ob)->d_elemc;\
}while(0)
#define DeeDeque_CONSUME_RIGHT(ob)\
do{\
 if DEE_UNLIKELY(DeeDeque_BACK_BUCKET_FULL_NZ(ob)) {\
  DEE_ASSERT((ob)->d_bucketv+(ob)->d_bucketc < (ob)->d_bucketroot+(ob)->d_bucketa);\
  DEE_ASSERT((ob)->d_bucketc > (ob)->d_bucketa);\
  (ob)->d_end = (ob)->d_bucketv[(ob)->d_bucketc++].db_elemv;\
 }\
 ++(ob)->d_end;\
 ++(ob)->d_elemc;\
}while(0)
#define DeeDeque_CONSUME_LEFT_N(ob,n)\
do{\
 Dee_size_t aligned_n,bucket_offset;\
 (ob)->d_elemc += (n);\
 if ((ob)->d_begin >= (ob)->d_bucketv[0].db_elemv+(n)) {\
  /* Enough free slots in the first bucket. --> No underflow. */\
  (ob)->d_begin -= (n);\
 } else {\
  aligned_n = (n)-((ob)->d_begin-(ob)->d_bucketv[0].db_elemv);\
  DEE_LVERBOSE4("aligned_n = %Iu\n",aligned_n);\
  bucket_offset = aligned_n/(ob)->d_bucketsize;\
  if ((aligned_n%(ob)->d_bucketsize)!=0) ++bucket_offset;\
  DEE_ASSERT(bucket_offset != 0);\
  (ob)->d_bucketv -= bucket_offset;\
  (ob)->d_bucketc += bucket_offset;\
  DEE_ASSERT((ob)->d_bucketc <= (ob)->d_bucketa);\
  (ob)->d_begin = (ob)->d_bucketv[0].db_elemv+((bucket_offset*(ob)->d_bucketsize)-aligned_n);\
  DEE_ASSERT((ob)->d_begin >= (ob)->d_bucketv[0].db_elemv);\
  DEE_ASSERT((ob)->d_begin < (ob)->d_bucketv[0].db_elemv+(ob)->d_bucketsize);\
 }\
}while(0)
#define DeeDeque_CONSUME_RIGHT_N(ob,n)\
do{\
 Dee_size_t aligned_n,bucket_offset;\
 (ob)->d_elemc += (n);\
 aligned_n = (n)+((ob)->d_end-(ob)->d_bucketv[(ob)->d_bucketc-1].db_elemv);\
 if (aligned_n < (ob)->d_bucketsize) {\
  /* Enough free slots in the last bucket. --> No overflow. */\
  (ob)->d_end += (n);\
 } else {\
  bucket_offset = aligned_n/(ob)->d_bucketsize;\
  if ((aligned_n%(ob)->d_bucketsize)==0) --bucket_offset;\
  DEE_LVERBOSE4("bucket_offset = %Iu\n",bucket_offset);\
  (ob)->d_bucketc += bucket_offset;\
  DEE_ASSERT((ob)->d_bucketc <= (ob)->d_bucketa);\
  (ob)->d_end = (ob)->d_bucketv[(ob)->d_bucketc-1].db_elemv+(aligned_n-(bucket_offset*(ob)->d_bucketsize));\
 }\
}while(0)
#define DeeDeque_RETURN_LEFT_N(ob,n)\
do{\
 Dee_size_t aligned_n,bucket_offset;\
 DEE_ASSERT((ob)->d_elemc >= (n));\
 if (((ob)->d_elemc -= (n)) == 0) {\
  (ob)->d_bucketc = 0;\
  (ob)->d_bucketv = NULL;\
  (ob)->d_begin = NULL;\
  (ob)->d_end = NULL;\
 } else if ((ob)->d_bucketc == 1) {\
  (ob)->d_begin += (n);\
  DEE_ASSERT((ob)->d_begin <= (ob)->d_end);\
 } else {\
  Dee_size_t index_front = (Dee_size_t)((ob)->d_begin-(ob)->d_bucketv[0].db_elemv);\
  aligned_n = index_front+(n);\
  if (aligned_n < (ob)->d_bucketsize) {\
   /* Enough used slots in the first bucket. --> No overflow. */\
   (ob)->d_begin += (n);\
  } else {\
   bucket_offset = aligned_n/(ob)->d_bucketsize;\
   DEE_ASSERT(bucket_offset != 0);\
   DEE_ASSERT((ob)->d_bucketc >= bucket_offset);\
   (ob)->d_bucketv += bucket_offset;\
   (ob)->d_bucketc -= bucket_offset;\
   DEE_ASSERT((ob)->d_bucketc != 0);\
   (ob)->d_begin = (ob)->d_bucketv[0].db_elemv+(aligned_n-(bucket_offset*(ob)->d_bucketsize));\
   DEE_ASSERT((ob)->d_begin >= (ob)->d_bucketv[0].db_elemv);\
   DEE_ASSERT((ob)->d_begin < (ob)->d_bucketv[0].db_elemv+(ob)->d_bucketsize);\
  }\
 }\
}while(0)
#define DeeDeque_RETURN_RIGHT_N(ob,n)\
do{\
 Dee_size_t aligned_n,bucket_offset;\
 DEE_ASSERT((ob)->d_elemc >= (n));\
 if (((ob)->d_elemc -= (n)) == 0) {\
  (ob)->d_bucketc = 0;\
  (ob)->d_bucketv = NULL;\
  (ob)->d_begin = NULL;\
  (ob)->d_end = NULL;\
 } else if ((ob)->d_bucketc == 1) {\
  (ob)->d_end -= (n);\
  DEE_ASSERT((ob)->d_end >= (ob)->d_begin);\
 } else {\
  Dee_size_t index_back = (Dee_size_t)((ob)->d_end-(ob)->d_bucketv[(ob)->d_bucketc-1].db_elemv);\
  if (index_back > (n)) {\
   /* Enough free slots in the last bucket. --> No overflow. */\
   (ob)->d_end -= (n);\
  } else {\
   aligned_n = (n)-index_back;\
   DEE_LVERBOSE4("aligned_n = %Iu\n",aligned_n);\
   if (!aligned_n) bucket_offset = 1;\
   else {\
    bucket_offset = aligned_n/(ob)->d_bucketsize;\
    if ((aligned_n%(ob)->d_bucketsize)!=0) ++bucket_offset;\
   }\
   DEE_LVERBOSE4("bucket_offset = %Iu\n",bucket_offset);\
   DEE_ASSERT((ob)->d_bucketc >= bucket_offset);\
   (ob)->d_bucketc -= bucket_offset;\
   DEE_ASSERT((ob)->d_bucketc != 0);\
   DEE_ASSERT((ob)->d_bucketc <= (ob)->d_bucketa);\
   (ob)->d_end = (ob)->d_bucketv[(ob)->d_bucketc-1].db_elemv+((bucket_offset*(ob)->d_bucketsize)-aligned_n);\
  }\
 }\
}while(0)


#define DeeDeque_ALLOC_FIRST(ob,elem,...)\
do{\
 DEE_ASSERT(!(ob)->d_elemc);\
 DEE_ASSERT(!(ob)->d_bucketc);\
 DEE_ASSERT(!(ob)->d_bucketv);\
 DEE_ASSERT(!(ob)->d_begin);\
 DEE_ASSERT(!(ob)->d_end);\
 if (!(ob)->d_bucketa) {\
  /* Must actually allocate the bucket */\
  if DEE_UNLIKELY(((ob)->d_bucketroot = (struct DeeDequeBucket *)\
   DEE_DEQUE_DCALLOC_NZ(sizeof(struct DeeDequeBucket))) == NULL) {__VA_ARGS__;}\
  (ob)->d_bucketa = 1;\
  DEE_ASSERT((ob)->d_bucketelema == 0);\
  DEE_ASSERT((ob)->d_bucketelemv == NULL);\
 }\
 if (!(ob)->d_bucketelema) {\
  DEE_ASSERT(!(ob)->d_bucketelemv);\
  /* Must actually allocate the bucket elements */\
  if DEE_UNLIKELY(((ob)->d_bucketroot[0].db_elemv = (DeeObject **)\
   DEE_DEQUE_DCALLOC_NZ((ob)->d_bucketsize*sizeof(DeeObject *))) == NULL) {__VA_ARGS__;}\
  (ob)->d_bucketelema = 1;\
  (ob)->d_bucketelemv = (ob)->d_bucketroot;\
 }\
 (ob)->d_bucketc = 1;\
 /* Choose the center-most bucket. */\
 (ob)->d_bucketv = (ob)->d_bucketroot+((ob)->d_bucketa/2);\
 (ob)->d_elemc = 1;\
 /* Choose the center-most elem-pointer. */\
 (ob)->d_begin = (ob)->d_bucketv[0].db_elemv+((ob)->d_bucketsize/2);\
 (ob)->d_end = (ob)->d_begin+1;\
 Dee_INCREF(*(ob)->d_begin = (elem));\
}while(0)

#define DeeDeque_ALLOC_FIRST_N(ob,elemc,elemv,...)\
do{\
 Dee_size_t min_bucketc,startend_elemc,overflow_elemc;\
 DeeObject **iter,**end,**src;\
 struct DeeDequeBucket *bucket_iter,*bucket_end;\
 DEE_ASSERT(!(ob)->d_elemc);\
 DEE_ASSERT(!(ob)->d_bucketc);\
 DEE_ASSERT(!(ob)->d_bucketv);\
 DEE_ASSERT(!(ob)->d_begin);\
 DEE_ASSERT(!(ob)->d_end);\
 DEE_ASSERT((elemc) != 0);\
 min_bucketc = (elemc)/(ob)->d_bucketsize;\
 if (((elemc)%(ob)->d_bucketsize)!=0) ++min_bucketc;\
 if ((ob)->d_bucketa < min_bucketc) {\
  struct DeeDequeBucket *new_root;\
  /* Must actually allocate the bucket */\
  if DEE_UNLIKELY((new_root = (struct DeeDequeBucket *)realloc_nz(\
   (ob)->d_bucketroot,min_bucketc*sizeof(struct DeeDequeBucket))) == NULL) {__VA_ARGS__;}\
  DEE_ASSERT(((ob)->d_bucketelema != 0) == ((ob)->d_bucketelemv != 0));\
  if ((ob)->d_bucketelema) { /* Relocate the elemv-cache */\
   (ob)->d_bucketelemv = new_root+(min_bucketc+((ob)->d_bucketelemv-(ob)->d_bucketroot));\
  }\
  /* NOTE: No need to relocate the d_bucketv-vector, as that one's NULL (aka. empty) */\
  (ob)->d_bucketroot = new_root;\
  (ob)->d_bucketa = min_bucketc;\
 }\
 if ((ob)->d_bucketelema < min_bucketc) {\
  Dee_size_t more_before,more_after;\
  /* Must actually allocate the bucket elements */\
  if (!(ob)->d_bucketelemv) (ob)->d_bucketelemv = (ob)->d_bucketroot;\
  /* Figure out how many additional elemv-buffers are required at the front and back. */\
  more_before = (Dee_size_t)((ob)->d_bucketelemv-(ob)->d_bucketroot);\
  if (more_before >= min_bucketc) {\
   more_before = min_bucketc,more_after = 0;\
  } else {\
   DEE_ASSERT(min_bucketc > more_before);\
   more_after = (Dee_size_t)(((ob)->d_bucketroot+(ob)->d_bucketa)-\
                             ((ob)->d_bucketelemv+(ob)->d_bucketelema));\
   if (more_after > min_bucketc-more_before) more_after = min_bucketc-more_before;\
  }\
  DEE_LVERBOSE3("more_before = %Iu\n",more_before);\
  DEE_LVERBOSE3("more_after = %Iu\n",more_after);\
  /* Allocate 'more_before' elemv-entries before '(ob)->d_bucketelemv' */\
  while (more_before--) {\
   DEE_ASSERT((ob)->d_bucketelemv > (ob)->d_bucketroot);\
   if DEE_UNLIKELY(((ob)->d_bucketelemv[-1].db_elemv = (DeeObject **)\
    DEE_DEQUE_DCALLOC_NZ((ob)->d_bucketsize*sizeof(DeeObject *))) == NULL) {__VA_ARGS__;}\
   --(ob)->d_bucketelemv;\
   ++(ob)->d_bucketelema;\
  }\
  /* Allocate 'more_after' elemv-entries starting at '(ob)->d_bucketelemv+(ob)->d_bucketelema' */\
  while (more_after--) {\
   DEE_ASSERT((ob)->d_bucketelemv+(ob)->d_bucketelema < (ob)->d_bucketroot+(ob)->d_bucketa);\
   if DEE_UNLIKELY(((ob)->d_bucketelemv[(ob)->d_bucketelema].db_elemv = (DeeObject **)\
    DEE_DEQUE_DCALLOC_NZ((ob)->d_bucketsize*sizeof(DeeObject *))) == NULL) {__VA_ARGS__;}\
   ++(ob)->d_bucketelema;\
  }\
 }\
 (ob)->d_bucketc = min_bucketc;\
 /* Choose the center-most bucket. */\
 DEE_ASSERT((ob)->d_bucketa >= min_bucketc);\
 (ob)->d_bucketv = (ob)->d_bucketroot+(((ob)->d_bucketa-min_bucketc)/2);\
 (ob)->d_elemc = (elemc);\
 /* Choose the center-most elem-pointer. */\
 src = (elemv);\
 if (min_bucketc == 1) {\
  DEE_ASSERT((elemc) <= (ob)->d_bucketsize);\
  overflow_elemc = (ob)->d_bucketsize-(elemc);\
  startend_elemc = overflow_elemc/2;\
  (ob)->d_begin = (ob)->d_bucketv[0].db_elemv+startend_elemc;\
  (ob)->d_end = (ob)->d_bucketv[0].db_elemv+((ob)->d_bucketsize-(overflow_elemc-startend_elemc));\
  iter = (ob)->d_begin,end = (ob)->d_end;\
  DEE_ASSERT((Dee_size_t)(end-iter) == (elemc));\
  do Dee_INCREF(*iter++ = *src++); while (iter != end);\
 } else {\
  DEE_ASSERT(min_bucketc >= 2);\
  overflow_elemc = ((ob)->d_bucketc*(ob)->d_bucketsize)-(elemc);\
  startend_elemc = overflow_elemc/2;\
  (ob)->d_begin = (ob)->d_bucketv[0].db_elemv+startend_elemc;\
  (ob)->d_end = (ob)->d_bucketv[(ob)->d_bucketc-1].db_elemv+((ob)->d_bucketsize-(overflow_elemc-startend_elemc));\
  /* Special case: First bucket */\
  iter = (ob)->d_begin,end = (ob)->d_bucketv[0].db_elemv+(ob)->d_bucketsize;\
  do Dee_INCREF(*iter++ = *src++); while (iter != end);\
  bucket_end = (bucket_iter = (ob)->d_bucketv+1)+((ob)->d_bucketc-2);\
  while (bucket_iter != bucket_end) {\
   end = (iter = bucket_iter->db_elemv)+(ob)->d_bucketsize;\
   do Dee_INCREF(*iter++ = *src++); while (iter != end);\
   ++bucket_iter;\
  }\
  DEE_ASSERT(bucket_iter == bucket_end);\
  DEE_ASSERT(bucket_end == (ob)->d_bucketv+((ob)->d_bucketc-1));\
  /* Special case: Last bucket */\
  iter = (ob)->d_bucketv[(ob)->d_bucketc-1].db_elemv,end = (ob)->d_end;\
  do Dee_INCREF(*iter++ = *src++); while (iter != end);\
 }\
 DEE_ASSERT(src == (elemv)+(elemc));\
}while(0)

#define DeeDeque_FREE_BUCKETC(ob)         ((ob)->d_bucketa-(ob)->d_bucketc)
#define DeeDeque_FRONT_FREE_BUCKETC(ob)   (Dee_size_t)((ob)->d_bucketv-(ob)->d_bucketroot)
#define DeeDeque_BACK_FREE_BUCKETC(ob)    (DeeDeque_FREE_BUCKETC(ob)-DeeDeque_FRONT_FREE_BUCKETC(ob))
#define DeeDeque_FRONT_BUCKET(ob)         ((ob)->d_bucketv)
#define DeeDeque_BACK_BUCKET(ob)          ((ob)->d_bucketv+((ob)->d_bucketc-1))
#define DeeDeque_END_BUCKET(ob)           ((ob)->d_bucketv+(ob)->d_bucketc)
#define DeeDeque_FRONT_BUCKET_FULL_NZ(ob) ((ob)->d_begin == DeeDeque_FRONT_BUCKET(ob)->db_elemv)
#define DeeDeque_BACK_BUCKET_FULL_NZ(ob)  ((ob)->d_end == DeeDeque_BACK_BUCKET(ob)->db_elemv+(ob)->d_bucketsize)
#define DeeDeque_FRONT_UNUSED_NZ(ob)      (Dee_size_t)((ob)->d_begin-DeeDeque_FRONT_BUCKET(ob)->db_elemv)
#define DeeDeque_FRONT_USED_NZ(ob)        ((ob)->d_bucketsize-DeeDeque_FRONT_UNUSED_NZ(ob))
#define DeeDeque_BACK_USED_NZ(ob)         (Dee_size_t)((ob)->d_end-DeeDeque_BACK_BUCKET(ob)->db_elemv)
#define DeeDeque_BACK_UNUSED_NZ(ob)       ((ob)->d_bucketsize-DeeDeque_BACK_USED_NZ(ob))
#define DeeDeque_FRONT_NZ(ob)             (*(ob)->d_begin)
#define DeeDeque_BACK_NZ(ob)              ((ob)->d_end[-1])
#define DeeDeque_ALLOCATED(ob)            ((ob)->d_bucketelema*(ob)->d_bucketsize)
#define DeeDeque_SIZE(ob)                 ((ob)->d_elemc)
#define DeeDeque_EMPTY(ob)                ((ob)->d_elemc==0)

#define DeeDeque_GET_NZ(ob,i)   (*DeeDeque_ELEM_NZ(ob,i))
#define DeeDeque_SET_NZ(ob,i,v) (*DeeDeque_ELEM_NZ(ob,i)=(v))
#define DeeDeque_ELEM_NZ        DeeDeque_ELEM_NZ
DEE_STATIC_INLINE(DeeObject **) DeeDeque_ELEM_NZ(struct DeeDeque *self, Dee_size_t i) {
 Dee_size_t aligned_i; DeeDeque_AssertIntegrity(self);
 aligned_i = i+DeeDeque_FRONT_UNUSED_NZ(self);
 return self->d_bucketv[aligned_i/self->d_bucketsize].db_elemv+(aligned_i%self->d_bucketsize);
}

struct DeeDequeIterator {
 struct DeeDequeBucket *di_bucket_iter; /*< [0..1] Current bucket. */
 struct DeeDequeBucket *di_bucket_end;  /*< [0..1] First invalid bucket. */
 DeeObject            **di_elem_iter;   /*< [1..1][0..1] Position in current bucket. */
 DeeObject            **di_elem_end;    /*< [1..1][0..1] End of the current bucket. */
};
#define DeeDequeIterator_DONE(ob)\
 ((ob)->di_bucket_iter == (ob)->di_bucket_end && \
  (ob)->di_elem_iter == (ob)->di_elem_end)
#define DeeDequeIterator_ELEM(ob)   ((DeeObject **)(ob)->di_elem_iter)
#define DeeDequeIterator_GET_NZ(ob) (*DeeDequeIterator_ELEM(ob))
#define DeeDequeIterator_GET(ob)    (DeeDequeIterator_DONE(ob) ? NULL : DeeDequeIterator_GET_NZ(ob))

#define DeeDequeIterator_GetIndex(ob,deq)\
 (Dee_size_t)((((ob)->di_bucket_iter-(deq)->d_bucketv)*(deq)->d_bucketsize+\
               ((ob)->di_elem_iter-(ob)->di_bucket_iter->db_elemv))-\
                DeeDeque_FRONT_UNUSED_NZ(deq))
#define DeeDequeIterator_InitBegin(ob,deq)\
(DeeDeque_AssertIntegrity(deq),\
 (ob)->di_bucket_end = ((ob)->di_bucket_iter = (deq)->d_bucketv)+(deq)->d_bucketc,\
 (ob)->di_elem_iter = (deq)->d_begin,(ob)->di_elem_end = (\
 (deq)->d_bucketc == 0 ? NULL : \
 (deq)->d_bucketc == 1 ? (deq)->d_end :\
 (deq)->d_bucketv->db_elemv+(deq)->d_bucketsize))
#define DeeDequeIterator_InitIndex(ob,deq,index)\
do{\
 Dee_size_t aligned_i,bucket_offset;\
 DeeDeque_AssertIntegrity(deq);\
 aligned_i = (Dee_size_t)((deq)->d_begin-(deq)->d_bucketv[0].db_elemv)+(index);\
 if (aligned_i < (deq)->d_bucketsize) {\
  (ob)->di_bucket_iter = (deq)->d_bucketv;\
  (ob)->di_elem_iter = (deq)->d_begin+(index);\
 } else {\
  bucket_offset = aligned_i/(deq)->d_bucketsize;\
  (ob)->di_bucket_iter = (deq)->d_bucketv+bucket_offset;\
  (ob)->di_elem_iter = (ob)->di_bucket_iter->db_elemv+(aligned_i-(bucket_offset*(deq)->d_bucketsize));\
 }\
 (ob)->di_elem_end = (ob)->di_bucket_iter->db_elemv+(deq)->d_bucketsize;\
 (ob)->di_bucket_end = (deq)->d_bucketv+(deq)->d_bucketc;\
}while(0)

#define DeeDequeIterator_InitEnd(ob,deq)\
(DeeDeque_AssertIntegrity(deq),\
 (ob)->di_bucket_end = (ob)->di_bucket_iter = ((deq)->d_bucketv+(deq)->d_bucketc),\
 (ob)->di_elem_iter = (deq)->d_begin,(ob)->di_elem_end = (deq)->d_end)

#define DeeDequeIterator_Get(ob) (*(ob)->di_elem_iter)
#define DeeDequeIterator_Done(ob)\
((ob)->di_elem_iter == (ob)->di_elem_end && \
 ((ob)->di_bucket_iter >= (ob)->di_bucket_end-1))
#define DeeDequeIterator_Next(ob,deq)\
(DEE_LIKELY((ob)->di_elem_iter != (ob)->di_elem_end)\
 ? (void)(DEE_ASSERT((ob)->di_elem_iter),++(ob)->di_elem_iter) : (void)(\
 ++(ob)->di_bucket_iter,DEE_ASSERT((ob)->di_bucket_iter < (ob)->di_bucket_end),\
 (ob)->di_elem_iter = (ob)->di_bucket_iter->db_elemv,\
 (ob)->di_elem_end = ((ob)->di_bucket_iter == (ob)->di_bucket_end-1)\
   ? (deq)->d_end : (ob)->di_elem_iter+(deq)->d_bucketsize,\
 ++(ob)->di_elem_iter))
#define DeeDequeIterator_Validate(ob,deq)\
((ob)->di_bucket_end == DeeDeque_END_BUCKET(deq) && \
 (ob)->di_bucket_iter >= (deq)->d_bucketv && \
 (ob)->di_elem_iter >= (ob)->di_bucket_iter->db_elemv && \
 (ob)->di_elem_end <= (ob)->di_bucket_iter->db_elemv+(deq)->d_bucketsize)

#define DeeDequeIterator_Yield(ob,deq)\
(DEE_LIKELY((ob)->di_elem_iter != (ob)->di_elem_end)\
 ? (DEE_ASSERT((ob)->di_elem_iter),*(ob)->di_elem_iter++) : (\
 (++(ob)->di_bucket_iter < (ob)->di_bucket_end)\
 ? ((ob)->di_elem_iter = (ob)->di_bucket_iter->db_elemv,\
    (ob)->di_elem_end = ((ob)->di_bucket_iter == (ob)->di_bucket_end-1)\
        ? (deq)->d_end : (ob)->di_elem_iter+(deq)->d_bucketsize,\
    *(ob)->di_elem_iter++)\
 : (--(ob)->di_bucket_iter,NULL)\
))
#define DeeDequeIterator_YieldSafe(ob,deq)\
(DeeDequeIterator_Validate(ob,deq)\
 ? DeeDequeIterator_Yield(ob,deq) : NULL)

#define DeeDeque_TRAVERSE_VARS \
 struct DeeDequeIterator _di_iter
#define DeeDeque_TRAVERSE(elem,deq)\
 if((DeeDequeIterator_InitBegin(&_di_iter,deq),0));else\
 while (((elem) = DeeDequeIterator_Yield(&_di_iter,deq)) != NULL)

#define DeeDeque_TRAVERSE_SAFE_VARS \
 struct DeeDequeIterator _di_iter
#define DeeDeque_TRAVERSE_SAFE(elem,deq)\
 if((DeeDequeIterator_InitBegin(&_di_iter,deq),0));else\
 while (((elem) = DeeDequeIterator_YieldSafe(&_di_iter,deq)) != NULL)
#define DeeDeque_TRAVERSE_INDEX(deq)         (DeeDequeIterator_GetIndex(&_di_iter,deq)-1)
#define DeeDeque_TRAVERSE_SAFE_INDEX(deq)    (DeeDequeIterator_GetIndex(&_di_iter,deq)-1)
#define DeeDeque_TRAVERSE_SAFE_VALIDATE(deq) DeeDequeIterator_Validate(&_di_iter,deq)


DEE_OBJECT_DEF(DeeDequeObject);
DEE_OBJECT_DEF(DeeDequeIteratorObject);
struct DeeDequeObject {
 DEE_OBJECT_HEAD
 struct DeeDeque       d_deq;  /*< Used deque. */
 struct DeeAtomicMutex d_lock; /*< Lock for this deque. */
};
#define DeeDeque_DEQUE(ob)   (&((DeeDequeObject *)Dee_REQUIRES_POINTER(ob))->d_deq)
#define DeeDeque_LOCK(ob)    (&((DeeDequeObject *)Dee_REQUIRES_POINTER(ob))->d_lock)
#define DeeDeque_ACQUIRE(ob) DeeAtomicMutex_AcquireRelaxed(DeeDeque_LOCK(ob))
#define DeeDeque_RELEASE(ob) DeeAtomicMutex_Release(DeeDeque_LOCK(ob))

struct DeeDequeIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDequeObject *di_deq;  /*< [1..1] Keep-alive reference. */
 struct DeeDequeIterator   di_iter; /*< Used iterator. */
 struct DeeAtomicMutex     di_lock; /*< Lock for this deque. */
};

extern DeeTypeObject DeeDeque_Type;
extern DeeTypeObject DeeDequeIterator_Type;










struct _DeeDequeFastIterator {
 struct DeeDequeBucket *dfi_bucket_iter; /*< [0..1] Current bucket. */
 DeeObject            **dfi_elem_iter;   /*< [1..1][0..1] Position in current bucket. */
union{
 DeeObject            **dfi_elem_end;    /*< [1..1][0..1] End of the current bucket (used by forward). */
 DeeObject            **dfi_elem_begin;  /*< [1..1][0..1] Begin of the current bucket (used by reverse). */
};
};
#define _DeeDequeFastIterator_ELEM(ob) ((DeeObject **)(ob)->dfi_elem_iter)
#define _DeeDequeFastIterator_InitFront(ob,deq)\
do{\
 (ob)->dfi_bucket_iter = (deq)->d_bucketv;\
 (ob)->dfi_elem_iter = (deq)->d_begin;\
 (ob)->dfi_elem_end = (deq)->d_bucketv[0].db_elemv+(deq)->d_bucketsize;\
}while(0)
#define _DeeDequeFastIterator_InitForward(ob,deq,i)\
do{\
 Dee_size_t aligned_i,bucket_offset;\
 aligned_i = (Dee_size_t)((deq)->d_begin-(deq)->d_bucketv[0].db_elemv)+(i);\
 DEE_LVERBOSE4("aligned_i = %Iu\n",aligned_i);\
 if (aligned_i < (deq)->d_bucketsize) {\
  (ob)->dfi_bucket_iter = (deq)->d_bucketv;\
  (ob)->dfi_elem_iter = (deq)->d_begin+(i);\
 } else {\
  bucket_offset = aligned_i/(deq)->d_bucketsize;\
  (ob)->dfi_bucket_iter = (deq)->d_bucketv+bucket_offset;\
  (ob)->dfi_elem_iter = (ob)->dfi_bucket_iter->db_elemv+(aligned_i-(bucket_offset*(deq)->d_bucketsize));\
 }\
 (ob)->dfi_elem_end = (ob)->dfi_bucket_iter->db_elemv+(deq)->d_bucketsize;\
}while(0)
#define _DeeDequeFastIterator_InitReverse(ob,deq,i)\
do{\
 Dee_size_t aligned_i,bucket_offset;\
 aligned_i = (Dee_size_t)((deq)->d_begin-(deq)->d_bucketv[0].db_elemv)+i;\
 DEE_LVERBOSE4("aligned_i = %Iu\n",aligned_i);\
 if (aligned_i < (deq)->d_bucketsize) {\
  (ob)->dfi_bucket_iter = (deq)->d_bucketv;\
  (ob)->dfi_elem_iter = (deq)->d_begin+(i);\
 } else {\
  bucket_offset = aligned_i/(deq)->d_bucketsize;\
  if ((aligned_i%(deq)->d_bucketsize)==0) --bucket_offset;\
  (ob)->dfi_bucket_iter = (deq)->d_bucketv+bucket_offset;\
  (ob)->dfi_elem_iter = (ob)->dfi_bucket_iter->db_elemv+(aligned_i-(bucket_offset*(deq)->d_bucketsize));\
 }\
 (ob)->dfi_elem_end = (ob)->dfi_bucket_iter->db_elemv;\
}while(0)
#define _DeeDequeFastIterator_Next(ob,deq)\
((++(ob)->dfi_elem_iter == (ob)->dfi_elem_end) ? (void)(\
 (ob)->dfi_elem_end = ((ob)->dfi_elem_iter = \
  (++(ob)->dfi_bucket_iter)->db_elemv)+(deq)->d_bucketsize\
) : (void)0)
#define _DeeDequeFastIterator_Prev(ob,deq)\
(((ob)->dfi_elem_iter-- == (ob)->dfi_elem_begin) ? (void)(\
 (ob)->dfi_elem_iter = ((ob)->dfi_elem_begin = \
  (--(ob)->dfi_bucket_iter)->db_elemv)+((deq)->d_bucketsize-1)\
) : (void)0)


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DEQUE_H_INL */
