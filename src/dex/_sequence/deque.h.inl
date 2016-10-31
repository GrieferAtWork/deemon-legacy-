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
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeDeque_PopFront(DEE_A_INOUT struct DeeDeque *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeDeque_PopFrontWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeDeque_PopBack(DEE_A_INOUT struct DeeDeque *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeDeque_PopBackWithLock(DEE_A_INOUT struct DeeDeque *self, DEE_A_INOUT struct DeeAtomicMutex *lock) DEE_ATTRIBUTE_NONNULL((1,2));

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
  /* Relocate the offsets of the elemv-start and buckets-start. */\
  (ob)->d_bucketelemv = new_buckets+((ob)->d_bucketelemv-(ob)->d_bucketroot);\
  /* Shift the elemv-buffers to make space on the left */\
  memmove((ob)->d_bucketelemv+new_bucketc,(ob)->d_bucketelemv,\
          (ob)->d_bucketelema*sizeof(struct DeeDequeBucket));\
  (ob)->d_bucketelemv += new_bucketc;\
  (ob)->d_bucketv = new_buckets+((ob)->d_bucketv-(ob)->d_bucketroot)+new_bucketc;\
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
#define DeeDeque_EMPBUCKETCACHE_RIGHT(ob) \
 ((ob)->d_bucketv+(ob)->d_bucketc == (ob)->d_bucketroot+(ob)->d_bucketa)
#define DeeDeque_INCBUCKETCACHE_RIGHT(ob,...)\
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
  /* Relocate the offsets of the elemv-start and buckets-start. */\
  (ob)->d_bucketelemv = new_buckets+((ob)->d_bucketelemv-(ob)->d_bucketroot);\
  (ob)->d_bucketv = new_buckets+((ob)->d_bucketv-(ob)->d_bucketroot);\
  (ob)->d_bucketroot = new_buckets;\
  (ob)->d_bucketa = new_bucket_cachesize;\
 } else {\
  /* Unused buckets available on the left --> Use them. */\
  DEE_ASSERT((ob)->d_bucketelemv > (ob)->d_bucketroot);\
  memmove((ob)->d_bucketelemv-1,(ob)->d_bucketelemv,\
          (ob)->d_bucketelema*sizeof(struct DeeDequeBucket));\
  --(ob)->d_bucketelemv;\
 }\
}while(0)
#define DeeDeque_EMPELEMVCACHE_LEFT(ob)  ((ob)->d_bucketv == (ob)->d_bucketelemv)
#define DeeDeque_ADDELEMVCACHE_LEFT(ob,...)\
do{\
 DeeObject **new_elemv;\
 DEE_ASSERTF((ob)->d_bucketelemv > (ob)->d_bucketroot,\
             "No bucketcache available; call 'DeeDeque_INCBUCKETCACHE_LEFT' first");\
 if DEE_UNLIKELY((new_elemv = (DeeObject **)malloc_nz(\
  (ob)->d_bucketsize*sizeof(DeeObject *))) == NULL) {__VA_ARGS__;}\
 --(ob)->d_bucketelemv;\
 (ob)->d_bucketelemv->db_elemv = new_elemv;\
 ++(ob)->d_bucketelema;\
}while(0)
#define DeeDeque_EMPELEMVCACHE_RIGHT(ob)\
 ((ob)->d_bucketv+(ob)->d_bucketc == (ob)->d_bucketelemv+(ob)->d_bucketelema)
#define DeeDeque_ADDELEMVCACHE_RIGHT(ob,...)\
do{\
 DeeObject **new_elemv;\
 DEE_ASSERTF((ob)->d_bucketelemv+(ob)->d_bucketelema < (ob)->d_bucketroot+(ob)->d_bucketa,\
             "No bucketcache available; call 'DeeDeque_INCBUCKETCACHE_RIGHT' first");\
 if DEE_UNLIKELY((new_elemv = (DeeObject **)malloc_nz(\
  (ob)->d_bucketsize*sizeof(DeeObject *))) == NULL) {__VA_ARGS__;}\
 (ob)->d_bucketelemv[(ob)->d_bucketelema++].db_elemv = new_elemv;\
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
   malloc_nz(sizeof(struct DeeDequeBucket))) == NULL) {__VA_ARGS__;}\
  (ob)->d_bucketa = 1;\
  DEE_ASSERT((ob)->d_bucketelema == 0);\
 }\
 if (!(ob)->d_bucketelema) {\
  DEE_ASSERT(!(ob)->d_bucketelemv);\
  /* Must actually allocate the bucket elements */\
  if DEE_UNLIKELY(((ob)->d_bucketroot[0].db_elemv = (DeeObject **)\
   malloc_nz((ob)->d_bucketsize*sizeof(DeeObject *))) == NULL) {__VA_ARGS__;}\
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

#define DeeDeque_FRONT_BUCKET(ob)         ((ob)->d_bucketv)
#define DeeDeque_BACK_BUCKET(ob)          ((ob)->d_bucketv+((ob)->d_bucketc-1))
#define DeeDeque_END_BUCKET(ob)           ((ob)->d_bucketv+(ob)->d_bucketc)
#define DeeDeque_FRONT_BUCKET_FULL_NZ(ob) ((ob)->d_begin == DeeDeque_FRONT_BUCKET(ob)->db_elemv)
#define DeeDeque_BACK_BUCKET_FULL_NZ(ob)  ((ob)->d_end == DeeDeque_BACK_BUCKET(ob)->db_elemv+(ob)->d_bucketsize)
#define DeeDeque_FRONT_UNUSED_NZ(ob)      ((ob)->d_begin-DeeDeque_FRONT_BUCKET(ob)->db_elemv)
#define DeeDeque_FRONT_USED_NZ(ob)        ((ob)->d_bucketsize-DeeDeque_FRONT_UNUSED_NZ(ob))
#define DeeDeque_BACK_USED_NZ(ob)         ((ob)->d_end-DeeDeque_BACK_BUCKET(ob)->db_elemv)
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
 Dee_size_t used_front,new_i;
 DEE_ASSERT(self);
 DEE_ASSERT(i < self->d_elemc);
 DEE_ASSERT(self->d_bucketc);
 used_front = DeeDeque_FRONT_USED_NZ(self);
 // Special handling for small indexes (The first bucket has a special start)
 if (i < used_front) return self->d_begin+i;
 new_i = i-used_front; // Input index, aligned to full buckets
 DEE_ASSERT(new_i/self->d_bucketsize < self->d_bucketc);
 return self->d_bucketv[new_i/self->d_bucketsize].db_elemv+(new_i%self->d_bucketsize);
}

struct DeeDequeIterator {
 struct DeeDequeBucket *di_bucket_iter;
 struct DeeDequeBucket *di_bucket_end;
 DeeObject            **di_elem_iter;
 DeeObject            **di_elem_end;
};
#define DeeDequeIterator_InitBegin(ob,deq)\
(DeeDeque_AssertIntegrity(deq),\
 (ob)->di_bucket_end = ((ob)->di_bucket_iter = (deq)->d_bucketv)+(deq)->d_bucketc,\
 (ob)->di_elem_iter = (deq)->d_begin,(ob)->di_elem_end = (\
 (deq)->d_bucketc == 0 ? NULL : \
 (deq)->d_bucketc == 1 ? (deq)->d_end :\
 (deq)->d_bucketv->db_elemv+(deq)->d_bucketsize))
#define DeeDequeIterator_InitEnd(ob,deq)\
(DeeDeque_AssertIntegrity(deq),\
 (ob)->di_bucket_end = (ob)->di_bucket_iter = ((deq)->d_bucketv+(deq)->d_bucketc),\
 (ob)->di_elem_iter = (deq)->d_begin,(ob)->di_elem_end = (deq)->d_end)

#define DeeDequeIterator_Yield(ob,deq)\
((ob)->di_elem_iter != (ob)->di_elem_end\
 ? (DEE_ASSERT((ob)->di_elem_iter),*(ob)->di_elem_iter++) : (\
 (++(ob)->di_bucket_iter < (ob)->di_bucket_end)\
 ? *((ob)->di_elem_iter = (ob)->di_bucket_iter->db_elemv,\
     (ob)->di_elem_end = ((ob)->di_bucket_iter == (ob)->di_bucket_end-1)\
         ? (deq)->d_end : (ob)->di_elem_iter+(deq)->d_bucketsize,\
     (ob)->di_elem_iter)++\
 : (--(ob)->di_bucket_iter,NULL)\
))
#define DeeDequeIterator_Validate(ob,deq)\
((ob)->di_bucket_end == DeeDeque_END_BUCKET(deq))
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

DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DEQUE_H_INL */
