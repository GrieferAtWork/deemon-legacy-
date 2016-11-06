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
#ifndef GUARD_DEEMON_SET_C
#define GUARD_DEEMON_SET_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/bool.h>
#include <deemon/cell.h>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/gc.h>
#include <deemon/generic_vtable.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/type_util.h>
#include <deemon/optional/object_visit.h>
#include <deemon/integer.h>
#include <deemon/list.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/set.h>
#include <deemon/super.h>
#include <deemon/tuple.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>
#include <deemon/runtime/builtin_functions.h>
#include DEE_INCLUDE_MEMORY_API()

// */ (nano...)

DEE_DECL_BEGIN

DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_size_t)
_DeeSetCapacityOf(DEE_A_IN Dee_size_t x) {
 Dee_size_t iter = 1;
 DEE_ASSERT(x);
 while (1) {
  if ((x & iter) != 0) return (x|iter)+1;
  iter = (iter<<1)|0x1;
 }
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_size_t)
_DeeCeil(DEE_A_IN double x) {
 Dee_size_t result = (Dee_size_t)x;
 if (result < x) ++result;
 return result;
}
// Returns true if 'node' is reachable from 'from[->n_next]...'
#define _DeeHashSetBucket_ContainsEntry(bucket,entry)\
 ((entry)>=(bucket)->hsb_entryv&&(entry)<(bucket)->hsb_entryv+(bucket)->hsb_entryc)

DEE_A_RET_WUNUSED int DeeHashSetIterator_CompareLo(
 DEE_A_IN struct DeeHashSetIterator const *lhs,
 DEE_A_IN struct DeeHashSetIterator const *rhs) {
 struct _DeeHashSetBucketEntry *p,*d;
 DEE_ASSERT(lhs && rhs);
 if (lhs->hsi_buckets_begin == rhs->hsi_buckets_begin) {
  p = lhs->hsi_hash_curr;
  d = rhs->hsi_hash_curr;
  if (p == d) return 0;
  if (!p) return 1;
  while (++p != lhs->hsi_hash_curr_end)
   if (p == d) return 1;
  return 0;
 } else if (lhs->hsi_buckets_begin > rhs->hsi_buckets_begin) {
  return 0;
 } else {
  DEE_ASSERT(lhs->hsi_buckets_begin < rhs->hsi_buckets_begin);
  return 1;
 }
}
DEE_A_RET_WUNUSED int DeeHashSetIterator_CompareLe(
 DEE_A_IN struct DeeHashSetIterator const *lhs,
 DEE_A_IN struct DeeHashSetIterator const *rhs) {
 struct _DeeHashSetBucketEntry *p,*d;
 DEE_ASSERT(lhs && rhs);
 if (lhs->hsi_buckets_begin == rhs->hsi_buckets_begin) {
  p = lhs->hsi_hash_curr;
  d = rhs->hsi_hash_curr;
  if (!p) return d == NULL;
  do if (p == d) return 1;
  while (++p != lhs->hsi_hash_curr_end);
  return 0;
 } else if (lhs->hsi_buckets_begin > rhs->hsi_buckets_begin) {
  return 0;
 } else {
  DEE_ASSERT(lhs->hsi_buckets_begin < rhs->hsi_buckets_begin);
  return 1;
 }
}
DEE_A_RET_WUNUSED int DeeHashSetIterator_CompareEq(
 DEE_A_IN struct DeeHashSetIterator const *lhs,
 DEE_A_IN struct DeeHashSetIterator const *rhs) {
 DEE_ASSERT(lhs && rhs);
 return lhs->hsi_buckets_begin == rhs->hsi_buckets_begin &&
        lhs->hsi_hash_curr == rhs->hsi_hash_curr;
}
DEE_A_RET_EXCEPT(NULL) struct DeeHashSetIterator *
DeeHashSetIterator_Alloc(void) {
 struct DeeHashSetIterator *result;
 while ((result = (struct DeeHashSetIterator *)malloc_nz(
  sizeof(struct DeeHashSetIterator))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 return result;
}
DEE_A_RET_EXCEPT(NULL) struct DeeHashSetIterator *
DeeHashSetIterator_New(DEE_A_IN struct DeeHashSet const *set) {
 struct DeeHashSetIterator *result;
 while ((result = (struct DeeHashSetIterator *)malloc_nz(
  sizeof(struct DeeHashSetIterator))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 DeeHashSetIterator_Init(result,set);
 return result;
}
void DeeHashSetIterator_Free(
 DEE_A_IN DEE_A_OUT_INVALID struct DeeHashSetIterator *self) {
 DEE_ASSERT(self);
 free_nn(self);
}
void DeeHashSetIterator_Init(
 DEE_A_OUT struct DeeHashSetIterator *self,
 DEE_A_IN struct DeeHashSet const *set) {
 DEE_ASSERT(self && set);
 if ((self->hsi_buckets_begin = set->hs_valid_buckets_begin) !=
     (self->hsi_buckets_end = set->hs_valid_buckets_end)) {
  self->hsi_hash_curr_end = (
   self->hsi_hash_curr = self->hsi_buckets_begin->hsb_entryv)+
   self->hsi_buckets_begin->hsb_entryc;
 } else {
  self->hsi_hash_curr = NULL;
  self->hsi_hash_curr_end = NULL;
 }
}

DEE_A_RET_MAYBE_NULL struct _DeeHashSetBucketEntry *DeeHashSetIterator_Yield(
 DEE_A_INOUT struct DeeHashSetIterator *self, DEE_A_IN struct DeeHashSet const *set) {
 struct _DeeHashSetBucketEntry *result;
 DEE_ASSERT(self);
 if (self->hsi_buckets_begin < set->hs_valid_buckets_begin ||
     self->hsi_buckets_begin >= set->hs_valid_buckets_end ||
     !_DeeHashSetBucket_ContainsEntry(self->hsi_buckets_begin,self->hsi_hash_curr)) {
  // TODO: Repair invalid iterator (somehow...)
  self->hsi_buckets_begin = NULL;
  self->hsi_buckets_end = NULL;
  self->hsi_hash_curr = NULL;
 }
 if ((result = self->hsi_hash_curr) != NULL) {
  DEE_ASSERT(self->hsi_buckets_begin < self->hsi_buckets_end);
  if (++self->hsi_hash_curr == self->hsi_hash_curr_end) {
   struct _DeeHashSetBucket *search,*search_end;
   /* Next bucket */
   search = self->hsi_buckets_begin;
   search_end = self->hsi_buckets_end;
   DEE_ASSERT(search),DEE_ASSERT(search_end);
   while (!(++search == search_end || search->hsb_entryc));
   self->hsi_buckets_begin = search;
   if ((self->hsi_buckets_begin = search) != self->hsi_buckets_end) {
    self->hsi_hash_curr_end = (
     self->hsi_hash_curr = self->hsi_buckets_begin->hsb_entryv)+
     self->hsi_buckets_begin->hsb_entryc;
   } else {
    self->hsi_hash_curr = NULL;
    self->hsi_hash_curr_end = NULL;
   }
  }
 }
 return result;
}


void _DeeHashSetBucket_Init(DEE_A_OUT struct _DeeHashSetBucket *self) {
 DEE_ASSERT(self);
 self->hsb_entryc = 0;
 self->hsb_entryv = NULL;
}
void _DeeHashSetBucket_Quit(DEE_A_IN struct _DeeHashSetBucket *self) {
 struct _DeeHashSetBucketEntry *iter,*end;
 DEE_ASSERT(self);
 end = (iter = self->hsb_entryv)+self->hsb_entryc;
 while (iter != end) Dee_DECREF((iter++)->sbe_key);
 free(self->hsb_entryv);
}
DEE_A_RET_NOEXCEPT(-1) int _DeeHashSetBucket_TryAddNew(
 DEE_A_INOUT struct _DeeHashSetBucket *self,
 DEE_A_IN Dee_hash_t hash, DEE_A_INOUT DeeObject *key) {
 struct _DeeHashSetBucketEntry *new_list;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(key));
 if ((new_list = (struct _DeeHashSetBucketEntry *)realloc_nz(
  self->hsb_entryv,(self->hsb_entryc+1)*sizeof(struct _DeeHashSetBucketEntry)
  )) == NULL) return -1;
 self->hsb_entryv = new_list;
 new_list += self->hsb_entryc++;
 new_list->sbe_hash = hash;
 Dee_INCREF(new_list->sbe_key = key);
 return 0;
}
DEE_A_RET_NOEXCEPT_REF DeeObject *_DeeHashSetBucket_TryPopSomething(
 DEE_A_INOUT struct _DeeHashSetBucket *self, DEE_A_OUT Dee_hash_t *hash) {
 struct _DeeHashSetBucketEntry *temp;
 DeeObject *result;
 DEE_ASSERT(self && self->hsb_entryc && hash);
 temp = self->hsb_entryv+(--self->hsb_entryc);
 Dee_INHERIT_REF(result,temp->sbe_key);
 *hash = _DeeHashSetBucketEntry_HASH(temp);
 if (self->hsb_entryc) {
  if ((temp = (struct _DeeHashSetBucketEntry *)realloc_nnz(
   self->hsb_entryv,self->hsb_entryc*sizeof(struct _DeeHashSetBucketEntry))) == NULL) {
   ++self->hsb_entryc;
   return NULL;
  }
  self->hsb_entryv = temp;
 } else {
  free_nn(self->hsb_entryv);
  self->hsb_entryv = NULL;
 }
 return result;
}
DEE_A_RET_NOEXCEPT(-1) int _DeeHashSetBucket_TryTransfer(
 DEE_A_INOUT struct _DeeHashSetBucket *dst,
 DEE_A_INOUT struct _DeeHashSetBucket *src,
 DEE_A_IN Dee_size_t src_index) {
 struct _DeeHashSetBucketEntry *dst_slot,*src_slot;
 DEE_ASSERT(dst && src);
 DEE_ASSERT(src_index < src->hsb_entryc);
 if ((dst_slot = (struct _DeeHashSetBucketEntry *)realloc_nz(
  dst->hsb_entryv,(dst->hsb_entryc+1)*sizeof(struct _DeeHashSetBucketEntry))
  ) == NULL) return -1;
 dst->hsb_entryv = dst_slot;
 dst_slot += dst->hsb_entryc++;
 src_slot = src->hsb_entryv+src_index;
 *dst_slot = *src_slot; // Transfer reference
 memcpy(src_slot,src_slot+1,src->hsb_entryc-src_index);
 if (--src->hsb_entryc) {
  if ((src_slot = (struct _DeeHashSetBucketEntry *)realloc_nnz(
   src->hsb_entryv,src->hsb_entryc*sizeof(struct _DeeHashSetBucketEntry))) == NULL) {
   // Must undo everything so far...
   src_slot = src->hsb_entryv+src_index;
   memcpy(src_slot+1,src_slot,src->hsb_entryc-src_index);
   *src_slot = *dst_slot; // Transfer reference (back)
   if (--dst->hsb_entryc) {
    if ((dst_slot = (struct _DeeHashSetBucketEntry *)realloc_nnz(
     dst->hsb_entryv,dst->hsb_entryc*sizeof(struct _DeeHashSetBucketEntry))) != NULL) {
     dst->hsb_entryv = dst_slot;
    } else {
     // Couldn't remove entry from dst
     // >> Looks like we'll have to live with two instances
     Dee_INCREF(src_slot->sbe_key); // Stabilize the doubled reference
    }
   } else {
    free_nn(dst->hsb_entryv);
    dst->hsb_entryv = NULL;
   }
   return -1;
  }
  src->hsb_entryv = src_slot;
 } else {
  free_nn(src->hsb_entryv);
  src->hsb_entryv = NULL;
 }
 return 0;
}


DEE_A_RET_EXCEPT(NULL) struct DeeHashSet *DeeHashSet_Alloc(void) {
 struct DeeHashSet *result;
 while ((result = (struct DeeHashSet *)
  malloc_nz(sizeof(struct DeeHashSet))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 return result;
}
DEE_A_RET_EXCEPT(NULL) struct DeeHashSet *DeeHashSet_New(void) {
 struct DeeHashSet *result;
 while ((result = (struct DeeHashSet *)
  malloc_nz(sizeof(struct DeeHashSet))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 DeeHashSet_Init(result);
 return result;
}
void DeeHashSet_Free(DEE_A_IN DEE_A_OUT_INVALID struct DeeHashSet *self) {
 DEE_ASSERT(self);
 free_nn(self);
}
void DeeHashSet_Delete(DEE_A_IN DEE_A_OUT_INVALID struct DeeHashSet *self) {
 DEE_ASSERT(self);
 DeeHashSet_Quit(self);
 free_nn(self);
}

void DeeHashSet_Init(DEE_A_OUT struct DeeHashSet *self) {
 DEE_ASSERT(self);
 self->hs_valid_buckets_begin = NULL;
 self->hs_valid_buckets_end = NULL;
 self->hs_buckets = NULL;
 self->hs_bucket_count = 0;
 self->hs_map_size = 0;
 self->hs_max_load_factor = 1.0;
}

void DeeHashSet_InitMove(
 DEE_A_OUT struct DeeHashSet *self,
 DEE_A_INOUT struct DeeHashSet *right) {
 DEE_ASSERT(self && right && self != right);
 self->hs_valid_buckets_begin = right->hs_valid_buckets_begin;
 self->hs_valid_buckets_end = right->hs_valid_buckets_end;
 self->hs_buckets = right->hs_buckets;
 self->hs_bucket_count = right->hs_bucket_count;
 self->hs_map_size = right->hs_map_size;
 self->hs_max_load_factor = right->hs_max_load_factor;
 right->hs_valid_buckets_begin = NULL;
 right->hs_valid_buckets_end = NULL;
 right->hs_buckets = NULL;
 right->hs_bucket_count = 0;
 right->hs_map_size = 0;
 right->hs_max_load_factor = 1.0;
}

DEE_A_RET_EXCEPT(-1) int DeeHashSet_InitFromIterator(
 DEE_A_OUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *iterator) {
 DeeObject *elem; int temp;
 DeeHashSet_Init(self);
 while ((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  temp = DeeHashSet_Insert(self,elem);
  Dee_DECREF(elem);
  if (temp == -1) {err: DeeHashSet_Quit(self); return -1; }
 }
 if (temp < 0) goto err;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeHashSet_TInitFromSequence(
 DEE_A_OUT struct DeeHashSet *self,
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) {
 int temp;
 DEE_ASSERT(DeeObject_Check(sequence));
 DEE_ASSERT(DeeObject_Check(tp_sequence) && DeeType_Check(tp_sequence));
 DEE_ASSERT(DeeObject_InstanceOf(sequence,tp_sequence));
 if (tp_sequence == &DeeTuple_Type) return DeeHashSet_InitFromKeys(self,DeeTuple_SIZE(sequence),DeeTuple_ELEM(sequence));
 if (tp_sequence == &DeeSet_Type) return DeeHashSet_InitCopyWithLock(self,DeeSet_SET(sequence),DeeSet_LOCK(sequence));
 if (tp_sequence == &DeeList_Type) {
  Dee_size_t i; DeeObject *elem;
  DeeHashSet_Init(self);
  DeeList_ACQUIRE(sequence);
  for (i = 0; i < DeeList_SIZE(sequence); ++i) {
   Dee_INCREF(elem = DeeList_GET(sequence,i));
   DeeList_RELEASE(sequence);
   temp = DeeHashSet_Insert(self,elem);
   Dee_DECREF(elem);
   if (temp != 0) { DeeHashSet_Quit(self); return temp; }
   DeeList_ACQUIRE(sequence);
  }
  DeeList_RELEASE(sequence);
  return 0;
 }
 if (tp_sequence == &DeeCell_Type) {
  if ((sequence = DeeCell_TryGet(sequence)) != NULL) {
   temp = DeeHashSet_InitFromKeys(self,1,&sequence);
   Dee_DECREF(sequence);
   return temp;
  }
  DeeHashSet_Init(self);
  return 0;
 }
 if (tp_sequence == &DeeSuper_Type)
  return DeeHashSet_TInitFromSequence(self,DeeSuper_TYPE(self),DeeSuper_SELF(self));
 if ((sequence = DeeObject_TIterSelf(tp_sequence,sequence)) == NULL) return -1;
 temp = DeeHashSet_InitFromIterator(self,sequence);
 Dee_DECREF(sequence);
 return temp;
}

DEE_A_RET_EXCEPT(-1) int DeeHashSet_InitFromKeys(
 DEE_A_OUT struct DeeHashSet *self,
 DEE_A_IN Dee_size_t keyc, DEE_A_IN_R(keyc) DeeObject *const *keyv) {
 DeeObject *const *end;
 DeeHashSet_Init(self);
 end = keyv+keyc;
 while (keyv != end) {
  if (DeeHashSet_Insert(self,*keyv) != 0) {
   DeeHashSet_Quit(self);
   return -1;
  }
  ++keyv;
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeHashSet_AssignCopy(
 DEE_A_INOUT struct DeeHashSet *self,
 DEE_A_IN struct DeeHashSet const *right) {
 struct DeeHashSet temp;
 DEE_ASSERT(self);
 DEE_ASSERT(right);
 DEE_ASSERT(self != right);
 if (DeeHashSet_InitCopy(&temp,right) != 0) return -1;
 DeeHashSet_Quit(self);
 memcpy(self,&temp,sizeof(struct DeeHashSet));
 return 0;
}
void DeeHashSet_Quit(DEE_A_IN DEE_A_OUT_INVALID struct DeeHashSet *self) {
 struct _DeeHashSetBucket *begin,*end;
 DEE_ASSERT(self);
 begin = self->hs_valid_buckets_begin;
 end = self->hs_valid_buckets_end;
 while (begin != end) _DeeHashSetBucket_Quit(begin++);
 free(self->hs_buckets);
}




#define _DeeHashSet_UpdateValidBuckets(ob)\
do{\
 struct _DeeHashSetBucket *_val_iter;\
 DEE_ASSERT((ob) && (ob)->hs_buckets);\
 /* Iterate to the first valid bucket */\
 _val_iter = (ob)->hs_buckets;\
 while (!_val_iter->hsb_entryc) ++_val_iter;\
 (ob)->hs_valid_buckets_begin = _val_iter;\
 /* Iterator to the first invalid bucket */\
 _val_iter = (ob)->hs_buckets+(ob)->hs_bucket_count;\
 while (!(--_val_iter)->hsb_entryc);\
 (ob)->hs_valid_buckets_end = _val_iter+1;\
}while(0)

DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(-1) int) _DeeHashSet_TryRehash(
 DEE_A_INOUT struct DeeHashSet *self, DEE_A_IN Dee_size_t new_bucket_count) {
 struct _DeeHashSetBucket *new_buckets,*old_buckets,*old_buckets_end;
 struct _DeeHashSetBucketEntry *iter,*end;
 Dee_hash_t new_hash,old_hash;
 DEE_ASSERT(self);
 if (!new_bucket_count) {
  DEE_ASSERTF(!self->hs_map_size,"Can't rehash non-empty set to empty bucket size");
  if (self->hs_buckets) {
   free_nn(self->hs_buckets);
   self->hs_buckets = NULL;
   self->hs_valid_buckets_begin = NULL;
   self->hs_valid_buckets_end = NULL;
  } else {
   DEE_ASSERT(self->hs_valid_buckets_begin == NULL);
   DEE_ASSERT(self->hs_valid_buckets_end == NULL);
  }
  self->hs_bucket_count = new_bucket_count;
  return 0;
 }
 DEE_ASSERT(self->hs_buckets);
 // v we can use 'calloc', because '_DeeHashSetBucket_Init' simply initializes to zero
 if ((new_buckets = (struct _DeeHashSetBucket *)callocf_nz(
  new_bucket_count,sizeof(struct _DeeHashSetBucket),
  "rehashed set buckets (%u)",(unsigned)new_bucket_count)
  ) == NULL) return -1;
 old_buckets = self->hs_valid_buckets_begin;
 old_buckets_end = self->hs_valid_buckets_end;
 // Actually start rehashing this shit
 while (1) {
  end = (iter = old_buckets->hsb_entryv)+old_buckets->hsb_entryc;
  old_hash = (Dee_size_t)(old_buckets-self->hs_buckets);
  while (iter != end) {
   new_hash = iter->sbe_hash % new_bucket_count;
   DEE_ASSERT(old_hash == iter->sbe_hash % self->hs_bucket_count);
   if (new_hash != old_hash) {
    Dee_size_t src_index;
    DEE_LVERBOSE2("[set::rehash] key = %O; hash = %Iu/%Iu/%Iu\n",
                  iter->sbe_key,old_hash,new_hash,iter->sbe_hash);
    src_index = (Dee_size_t)(iter-old_buckets->hsb_entryv);
    if (_DeeHashSetBucket_TryTransfer(new_buckets+new_hash,old_buckets,src_index) == -1) {
     struct _DeeHashSetBucket *new_buckets_end;
     // We may lose some keys in here, but there's no point in
     // trying to move them back into the old list, since
     // attempting to move an entry was what caused the problem
     // in the first place.
     // And only a crazy person would try the same thing twice,
     // while expecting a different outcome...
     new_buckets_end = new_buckets+new_bucket_count;
     while (new_buckets_end != new_buckets) _DeeHashSetBucket_Quit(--new_buckets_end);
     free_nn(new_buckets);
     return -1;
    }
    end = old_buckets->hsb_entryv+old_buckets->hsb_entryc;
    iter = old_buckets->hsb_entryv+src_index;
   }
   ++iter;
  }
  if (++old_buckets == old_buckets_end) break;
 }
 // Exchange the bucket maps + update bucket count
 free_nn(self->hs_buckets);
 self->hs_buckets = new_buckets;
 self->hs_bucket_count = new_bucket_count;
 // Update valid buckets
 _DeeHashSet_UpdateValidBuckets(self);
 return 0;
}

DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(-1) int) _DeeHashSet_TryInsertFirstEntry(
 DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob, DEE_A_IN Dee_hash_t bucket) {
 struct _DeeHashSetBucket *buckets;
 DEE_ASSERTF(self && self->hs_map_size == 0,"Invalid, or non-empty map");
#if DEE_HASHSET_INITIAL_BUCKET_COUNT <= 1
 buckets = (struct _DeeHashSetBucket *)mallocf_nz(
  sizeof(struct _DeeHashSetBucket),"Initial set buckets");
#else
 buckets = (struct _DeeHashSetBucket *)callocf_nz(
  DEE_HASHSET_INITIAL_BUCKET_COUNT,sizeof(struct _DeeHashSetBucket),"Initial set buckets");
#endif
 if (!buckets) return -1;
 self->hs_bucket_count = DEE_HASHSET_INITIAL_BUCKET_COUNT;
 self->hs_map_size = 1;
 self->hs_buckets = buckets;
#if DEE_HASHSET_INITIAL_BUCKET_COUNT > 1
 buckets += bucket % DEE_HASHSET_INITIAL_BUCKET_COUNT;
#endif
 if ((buckets->hsb_entryv = (struct _DeeHashSetBucketEntry *)mallocf_nz(
  sizeof(struct _DeeHashSetBucketEntry),"First hash-set bucket")) == NULL) {
  free_nn(buckets);
  return -1;
 }
 buckets->hsb_entryc = 1;
 self->hs_valid_buckets_begin = buckets;
 self->hs_valid_buckets_end = buckets+1;
 buckets->hsb_entryv[0].sbe_hash = bucket;
 Dee_INCREF(buckets->hsb_entryv[0].sbe_key = ob);
 return 0;
}



DEE_A_RET_NEVER_NULL DEE_A_RET_WUNUSED DeeObject *DeeHashSet_GetKeyIndex(
 DEE_A_IN struct DeeHashSet const *self, DEE_A_IN Dee_size_t index) {
 struct _DeeHashSetBucketEntry *entry;
 DeeHashSet_TRAVERSE_INF_VARS;
 DEE_ASSERT(self);
 DEE_ASSERT(index < DeeHashSet_SIZE(self));
 DeeHashSet_TRAVERSE_INF(entry,self) {
  if (!index--) return _DeeHashSetBucketEntry_KEY(entry);
 }
#ifndef _MSC_VER
 DEE_BUILTIN_UNREACHABLE();
#endif
}






DEE_A_RET_WUNUSED Dee_size_t DeeHashSet_Size(
 DEE_A_IN struct DeeHashSet const *self) {
 DEE_ASSERT(self);
 return DeeHashSet_SIZE(self);
}
DEE_A_RET_WUNUSED Dee_size_t DeeHashSet_BucketSize(
 DEE_A_IN struct DeeHashSet const *self, DEE_A_IN Dee_size_t bucket) {
 DEE_ASSERT(self);
 return DeeHashSet_BUCKET_SIZE(self,bucket);
}
DEE_A_RET_WUNUSED Dee_size_t DeeHashSet_BucketCount(
 DEE_A_IN struct DeeHashSet const *self) {
 DEE_ASSERT(self);
 return DeeHashSet_BUCKET_COUNT(self);
}
DEE_A_RET_WUNUSED double DeeHashSet_GetMaxLoadFactor(
 DEE_A_IN struct DeeHashSet const *self) {
 DEE_ASSERT(self);
 return DeeHashSet_GET_MAX_LOAD_FACTOR(self);
}
void DeeHashSet_SetMaxLoadFactor(
 DEE_A_INOUT struct DeeHashSet *self, DEE_A_IN double v) {
 DEE_ASSERT(self);
 DeeHashSet_SET_MAX_LOAD_FACTOR(self,v);
}

DEE_A_RET_NEVER_NULL struct _DeeHashSetBucketEntry *
_DeeHashSet_FrontEntry(DEE_A_IN struct DeeHashSet const *self) {
 DEE_ASSERT(self && self->hs_map_size != 0);
 DEE_ASSERT(self->hs_valid_buckets_begin != NULL);
 DEE_ASSERT(self->hs_valid_buckets_begin->hsb_entryc != 0);
 return &self->hs_valid_buckets_begin->hsb_entryv[0];
}
DEE_A_RET_NEVER_NULL struct _DeeHashSetBucketEntry *
_DeeHashSet_BackEntry(DEE_A_IN struct DeeHashSet const *self) {
 struct _DeeHashSetBucket *bucket_last;
 DEE_ASSERT(self && self->hs_map_size != 0);
 DEE_ASSERT(self->hs_valid_buckets_end);
 bucket_last = self->hs_valid_buckets_end-1;
 DEE_ASSERT(bucket_last->hsb_entryc != 0);
 return &bucket_last->hsb_entryv[bucket_last->hsb_entryc-1];
}
























DEE_A_RET_OBJECT_EXCEPT_REF(DeeSetObject) *
DeeSet_New(void) {
 DeeSetObject *result;
 if ((result = DeeGC_MALLOC(DeeSetObject)) != NULL) {
  DeeObject_INIT(result,&DeeSet_Type);
  DeeAtomicMutex_Init(&result->s_lock);
  DeeHashSet_Init(&result->s_set);
  DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeSetObject) *DeeSet_TNewFromSequence(
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) {
 DeeSetObject *result;
 DEE_ASSERT(DeeObject_Check(sequence));
 DEE_ASSERT(DeeObject_Check(tp_sequence) && DeeType_Check(tp_sequence));
 DEE_ASSERT(DeeObject_InstanceOf(sequence,tp_sequence));
 if ((result = DeeGC_MALLOC(DeeSetObject)) != NULL) {
  DeeObject_INIT(result,&DeeSet_Type);
  DeeAtomicMutex_Init(&result->s_lock);
  if (DeeHashSet_TInitFromSequence(&result->s_set,tp_sequence,sequence) != 0) {
   _DeeGCObject_Delete((DeeObject *)result);
   result = NULL;
  } else DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeSetObject) *
DeeSet_NewFromIterator(DEE_A_INOUT DeeObject *iterator) {
 DeeSetObject *result;
 DEE_ASSERT(DeeObject_Check(iterator));
 if ((result = DeeGC_MALLOC(DeeSetObject)) != NULL) {
  DeeObject_INIT(result,&DeeSet_Type);
  DeeAtomicMutex_Init(&result->s_lock);
  if (DeeHashSet_InitFromIterator(&result->s_set,iterator) != 0) {
   _DeeGCObject_Delete((DeeObject *)result);
   result = NULL;
  } else DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeSetObject) *
DeeSet_Copy(DEE_A_IN_OBJECT(DeeSetObject) const *self) {
 DeeSetObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 if ((result = DeeGC_MALLOC(DeeSetObject)) != NULL) {
  DeeObject_INIT(result,&DeeSet_Type);
  DeeAtomicMutex_Init(&result->s_lock);
  if (DeeHashSet_InitCopy(&result->s_set,DeeSet_SET(self)) != 0) {
   _DeeGCObject_Delete((DeeObject *)result);
   result = NULL;
  } else DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeSetObject) *
DeeSet_DeepCopy(DEE_A_IN_OBJECT(DeeSetObject) const *self) {
 DeeSetObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 if ((result = DeeGC_MALLOC(DeeSetObject)) != NULL) {
  DeeObject_INIT(result,&DeeSet_Type);
  DeeAtomicMutex_Init(&result->s_lock);
  if (DeeHashSet_InitDeepCopy(&result->s_set,DeeSet_SET(self)) != 0) {
   _DeeGCObject_Delete((DeeObject *)result);
   result = NULL;
  } else DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeSetObject) *DeeSet_NewFromKeys(
 DEE_A_IN Dee_size_t keyc, DEE_A_IN_R(keyc) DeeObject *const *keyv) {
 DeeSetObject *result;
 if ((result = DeeGC_MALLOC(DeeSetObject)) != NULL) {
  DeeObject_INIT(result,&DeeSet_Type);
  DeeAtomicMutex_Init(&result->s_lock);
  if (DeeHashSet_InitFromKeys(&result->s_set,keyc,keyv) != 0) {
   _DeeGCObject_Delete((DeeObject *)result);
   result = NULL;
  } else DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}
DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeSetObject) *DeeSet_TCast(
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(tp_sequence) && DeeType_Check(tp_sequence));
 DEE_ASSERT(DeeObject_Check(sequence));
 DEE_ASSERT(DeeObject_InstanceOf(sequence,tp_sequence));
 if (tp_sequence == &DeeSet_Type) DeeReturn_NEWREF(sequence);
 if (tp_sequence == &DeeTuple_Type) return DeeSet_NewFromKeys(DeeTuple_SIZE(sequence),DeeTuple_ELEM(sequence));
 if (tp_sequence == &DeeList_Type) {
  Dee_size_t i; DeeObject *elem; int error;
  if ((result = DeeSet_New()) == NULL) return NULL;
  DeeList_ACQUIRE(sequence);
  for (i = 0; i < DeeList_SIZE(sequence); ++i) {
   Dee_INCREF(elem = DeeList_GET(sequence,i));
   DeeList_RELEASE(sequence);
   error = DeeSet_Insert(result,elem);
   Dee_DECREF(elem);
   if (error != 0) { Dee_DECREF(result); return NULL; }
   DeeList_ACQUIRE(sequence);
  }
  DeeList_RELEASE(sequence);
  return result;
 }
 if (tp_sequence == &DeeCell_Type) {
  DeeObject *cellval;
  if ((cellval = DeeCell_TryGet(sequence)) == NULL) return DeeSet_NewEmpty();
  result = DeeSet_NewFromKeys(1,&cellval);
  Dee_DECREF(cellval);
  return result;
 }
 if (tp_sequence == &DeeSuper_Type)
  return DeeSet_TCast(DeeSuper_TYPE(sequence),DeeSuper_SELF(sequence));
 if ((sequence = DeeObject_TIterSelf(tp_sequence,sequence)) == NULL) return NULL;
 result = DeeSet_NewFromIterator(sequence);
 Dee_DECREF(sequence);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeSet_TInsertSequence(
 DEE_A_INOUT_OBJECT(DeeSetObject) *self,
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_sequence) && DeeType_Check(tp_sequence));
 DEE_ASSERT(DeeObject_Check(sequence));
 DEE_ASSERT(DeeObject_InstanceOf(sequence,tp_sequence));
 return DeeHashSet_TInsertSequenceWithLock(DeeSet_SET(self),tp_sequence,self,DeeSet_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) int DeeSet_InsertIterator(
 DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *iterator) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 DEE_ASSERT(DeeObject_Check(iterator));
 return DeeHashSet_InsertIteratorWithLock(DeeSet_SET(self),iterator,DeeSet_LOCK(self));
}



void DeeSet_Clear(DEE_A_INOUT_OBJECT(DeeSetObject) *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 DeeHashSet_ClearWithLock(DeeSet_SET(self),DeeSet_LOCK(self));
}

DEE_A_RET_WUNUSED Dee_size_t DeeSet_Size(
 DEE_A_IN_OBJECT(DeeSetObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 return DeeSet_SIZE(self);
}

DEE_A_RET_WUNUSED Dee_size_t DeeSet_BucketSize(
 DEE_A_IN_OBJECT(DeeSetObject) const *self, DEE_A_IN Dee_size_t bucket) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 DEE_ASSERT(bucket < DeeSet_BUCKET_COUNT(self));
 return DeeSet_BUCKET_SIZE(self,bucket);
}

DEE_A_RET_WUNUSED Dee_size_t DeeSet_BucketCount(
 DEE_A_IN_OBJECT(DeeSetObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 return DeeSet_BUCKET_COUNT(self);
}

DEE_A_RET_WUNUSED double DeeSet_GetMaxLoadFactor(
 DEE_A_IN_OBJECT(DeeSetObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 return DeeSet_GET_MAX_LOAD_FACTOR(self);
}

void DeeSet_SetMaxLoadFactor(
 DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_IN double v) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 DeeSet_SET_MAX_LOAD_FACTOR(self,v);
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeSetIteratorObject) *
DeeSet_IterSelf(DEE_A_INOUT_OBJECT(DeeSetObject) *self) {
 DeeSetIteratorObject *result;
 if ((result = DeeObject_MALLOCF(DeeSetIteratorObject,"hashset.iterator(%p)",self)) != NULL) {
  DeeObject_INIT(result,&DeeSetIterator_Type);
  Dee_INCREF(result->si_set = (DeeSetObject *)self);
  DeeSet_ACQUIRE(self);
  DeeHashSetIterator_Init(DeeSetIterator_ITER(result),DeeSet_SET(self));
  DeeSet_RELEASE(self);
 }
 return (DeeObject *)result;
}



















static int DEE_CALL _deeset_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSetObject *self) {
 DeeAtomicMutex_Init(&self->s_lock);
 DeeHashSet_Init(&self->s_set);
 return 0;
}
static int DEE_CALL _deeset_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSetObject *self, DeeSetObject *right) {
 DeeAtomicMutex_Init(&self->s_lock);
 return DeeHashSet_InitCopyWithLock(&self->s_set,&right->s_set,DeeSet_LOCK(right));
}
static int DEE_CALL _deeset_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSetObject *self, DeeSetObject *right) {
 DeeAtomicMutex_Init(&self->s_lock);
 DeeSet_ACQUIRE(right);
 DeeHashSet_InitMove(&self->s_set,&right->s_set);
 DeeSet_RELEASE(right);
 return 0;
}
static int DEE_CALL _deeset_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSetObject *self, DeeObject *args) {
 DeeObject *sequence;
 if (DeeTuple_Unpack(args,"o:set",&sequence) != 0) return -1;
 DeeAtomicMutex_Init(&self->s_lock);
 return DeeHashSet_InitFromSequence(&self->s_set,sequence);
}
static void DEE_CALL _deeset_tp_dtor(DeeSetObject *self) {
 DeeHashSet_Quit(&self->s_set);
}
DEE_VISIT_PROC(_deeset_tp_visit,DeeSetObject *self) {
 struct _DeeHashSetBucketEntry *entry;
 DeeHashSet_TRAVERSE_VARS;
 DeeSet_ACQUIRE(self);
 DeeHashSet_TRAVERSE(entry,DeeSet_SET(self)) {
  Dee_VISIT(_DeeHashSetBucketEntry_KEY(entry));
 }
 DeeSet_RELEASE(self);
}
static int DEE_CALL _deeset_tp_bool(DeeSetObject *self) {
 int result;
 DeeSet_ACQUIRE(self);
 result = DeeSet_SIZE(self) != 0;
 DeeSet_RELEASE(self);
 return result;
}
static DeeObject *DEE_CALL _deeset_tp_not(DeeSetObject *self) {
 int result;
 DeeSet_ACQUIRE(self);
 result = DeeSet_SIZE(self) == 0;
 DeeSet_RELEASE(self);
 DeeReturn_Bool(result);
}
static DeeObject *DEE_CALL _deeset_tp_add(DeeSetObject *self, DeeObject *sequence) {
 DeeObject *result;
 if ((result = DeeSet_Copy((DeeObject *)self)) == NULL) return NULL;
 if (DeeSet_InsertSequence(result,sequence) != 0) Dee_CLEAR(result);
 return result;
}
static DeeObject *DEE_CALL _deeset_tp_inplace_add(DeeSetObject *self, DeeObject *sequence) {
 if (DeeSet_InsertSequence((DeeObject *)self,sequence) != 0) return NULL;
 DeeReturn_NEWREF(self);
}
static int DEE_CALL _deeset_tp_hash(DeeSetObject *self, Dee_hash_t start, Dee_hash_t *result) {
 return DeeHashSet_HashWithLock(DeeSet_SET(self),start,result,DeeSet_LOCK(self));
}
static int DEE_CALL _deeset_tp_move_assign(DeeSetObject *self, DeeSetObject *right) {
 if (self != right) {
  struct _DeeHashSetBucket *old_buckets,*begin,*end;
  struct DeeHashSet temp_set;
  DeeSet_ACQUIRE(right);
  temp_set = right->s_set;
  right->s_set.hs_valid_buckets_begin = NULL;
  right->s_set.hs_valid_buckets_end = NULL;
  right->s_set.hs_buckets = 0;
  right->s_set.hs_bucket_count = 0;
  right->s_set.hs_map_size = 0;
  DeeSet_RELEASE(right);
  DeeSet_ACQUIRE(self);
  begin = self->s_set.hs_valid_buckets_begin;
  end = self->s_set.hs_valid_buckets_end;
  old_buckets = self->s_set.hs_buckets;
  self->s_set = temp_set;
  DeeSet_RELEASE(self);
  while (begin != end) _DeeHashSetBucket_Quit(begin++);
  free(old_buckets);
 }
 return 0;
}
static int DEE_CALL _deeset_tp_copy_assign(DeeSetObject *self, DeeSetObject *right) {
 if (self != right) {
  struct _DeeHashSetBucket *old_buckets,*begin,*end;
  struct DeeHashSet temp_set;
  DeeSet_RELEASE(right);
  if (DeeHashSet_InitCopy(&temp_set,&right->s_set) != 0) { DeeSet_RELEASE(right); return -1; }
  DeeSet_RELEASE(right);
  DeeSet_ACQUIRE(self);
  begin = self->s_set.hs_valid_buckets_begin;
  end = self->s_set.hs_valid_buckets_end;
  old_buckets = self->s_set.hs_buckets;
  self->s_set = temp_set;
  DeeSet_RELEASE(self);
  while (begin != end) _DeeHashSetBucket_Quit(begin++);
  free(old_buckets);
 }
 return 0;
}
static int DEE_CALL _deeset_tp_any_assign(DeeSetObject *self, DeeObject *sequence) {
 struct _DeeHashSetBucket *old_buckets,*begin,*end;
 struct DeeHashSet temp_set;
 if (DeeHashSet_InitFromSequence(&temp_set,sequence) != 0) return -1;
 DeeSet_ACQUIRE(self);
 begin = self->s_set.hs_valid_buckets_begin;
 end = self->s_set.hs_valid_buckets_end;
 old_buckets = self->s_set.hs_buckets;
 self->s_set = temp_set;
 DeeSet_RELEASE(self);
 while (begin != end) _DeeHashSetBucket_Quit(begin++);
 free(old_buckets);
 return 0;
}

static DeeObject *DEE_CALL _deeset_tp_str(DeeSetObject const *self) {
 DeeObject *result,*entry_key; int temp,first = 1;
 DeeHashSet_TRAVERSE_SAFE_VARS;
 struct _DeeHashSetBucketEntry *entry;
 DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 if (DeeStringWriter_WRITE_STRING(&writer,"set { ") == -1) goto err;
 DeeSet_ACQUIRE(self);
 DeeHashSet_TRAVERSE_SAFE(entry,DeeSet_SET(self)) {
  Dee_INCREF(entry_key = _DeeHashSetBucketEntry_KEY(entry));
  DeeSet_RELEASE(self);
  if (first)first = 0;else if (DeeStringWriter_WRITE_STRING(
   &writer,", ") == -1) { Dee_DECREF(entry_key); goto err; }
  temp = DeeStringWriter_WriteObjectStr(&writer,entry_key);
  Dee_DECREF(entry_key);
  if (temp == -1) goto err;
  DeeSet_ACQUIRE(self);
 }
 DeeSet_RELEASE(self);
 if (DeeStringWriter_WRITE_STRING(&writer," }") == -1) goto err;
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}

static DeeObject *DEE_CALL _deeset_tp_repr(DeeSetObject const *self) {
 DeeObject *result,*entry_key; int temp,first = 1;
 DeeHashSet_TRAVERSE_SAFE_VARS;
 struct _DeeHashSetBucketEntry *entry;
 DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 if (DeeStringWriter_WRITE_STRING(&writer,"set { ") == -1) goto err;
 DeeSet_ACQUIRE(self);
 DeeHashSet_TRAVERSE_SAFE(entry,DeeSet_SET(self)) {
  Dee_INCREF(entry_key = _DeeHashSetBucketEntry_KEY(entry));
  DeeSet_RELEASE(self);
  if (first)first = 0;else if (DeeStringWriter_WRITE_STRING(
   &writer,", ") == -1) { Dee_DECREF(entry_key); goto err; }
  temp = DeeStringWriter_WriteObjectRepr(&writer,entry_key);
  Dee_DECREF(entry_key);
  if (temp == -1) goto err;
  DeeSet_ACQUIRE(self);
 }
 DeeSet_RELEASE(self);
 if (DeeStringWriter_WRITE_STRING(&writer," }") == -1) goto err;
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}

static DeeObject *DEE_CALL _deeset_tp_seq_get(DeeSetObject *self, DeeObject *i) {
 Dee_ssize_t ii;
 if (DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return NULL;
 return DeeSet_GetKeyIndex((DeeObject *)self,ii);
}
static DeeObject *DEE_CALL _deeset_tp_seq_range_get(DeeSetObject *self, DeeObject *lo, DeeObject *hi) {
 Dee_size_t loi,hii;
 if (DeeNone_Check(lo)) loi = 0; else if (DeeObject_Cast(Dee_size_t,lo,&loi) != 0) return NULL;
 if (DeeNone_Check(hi)) hii = (Dee_size_t)-1; else if (DeeObject_Cast(Dee_size_t,hi,&hii) != 0) return NULL;
 return DeeSet_GetKeyRangeIndex((DeeObject *)self,loi,hii);
}
static DeeObject *DEE_CALL _deeset_tp_seq_size(DeeSetObject *self) {
 Dee_size_t result;
 DeeSet_ACQUIRE(self);
 result = DeeSet_SIZE(self);
 DeeSet_RELEASE(self);
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *DEE_CALL _deeset_tp_seq_contains(DeeSetObject *self, DeeObject *ob) {
 int temp;
 if DEE_UNLIKELY((temp = DeeSet_Contains((DeeObject *)self,ob)) < 0) return NULL;
 DeeReturn_Bool(temp);
}

static DeeObject *DEE_CALL _deeset_clear(
 DeeSetObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":clear") != 0) return NULL;
 DeeSet_Clear((DeeObject *)self);
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deeset_insert(
 DeeSetObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *ob; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:insert",&ob) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeSet_Insert((DeeObject *)self,ob)) < 0) return NULL;
 DeeReturn_Bool(!result);
}
static DeeObject *DEE_CALL _deeset_insert_all(
 DeeSetObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *sequence;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:insert_all",&sequence) != 0) return NULL;
 if DEE_UNLIKELY(DeeSet_InsertSequence((DeeObject *)self,sequence) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deeset_remove(
 DeeSetObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *ob; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:remove",&ob) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeSet_Remove((DeeObject *)self,ob)) < 0) return NULL;
 DeeReturn_Bool(!result);
}
static DeeObject *DEE_CALL _deeset_contains(
 DeeSetObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem,*pred = Dee_None; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:contains",&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY(DEE_LIKELY(result = ((DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeSet_Contains((DeeObject *)self,elem)
  : DeeSet_ContainsPred((DeeObject *)self,elem,pred)
  )) < 0) return NULL;
 DeeReturn_Bool(!result);
}
static DeeObject *DEE_CALL _deeset_popitem(
 DeeSetObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":popitem") != 0) return NULL;
 return DeeSet_PopSomething((DeeObject *)self);
}
static DeeObject *DEE_CALL _deeset_find(
 DeeSetObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *find_ob; Dee_ssize_t index;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:find",&find_ob) != 0) return NULL;
 if DEE_UNLIKELY((index = DeeSet_FindKeyIndex((DeeObject *)self,find_ob)) == -2) return NULL;
 return DeeObject_New(Dee_ssize_t,index);
}
static DeeObject *DEE_CALL _deeset_index(
 DeeSetObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *find_ob; Dee_ssize_t index;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:index",&find_ob) != 0) return NULL;
 if DEE_UNLIKELY((index = DeeSet_FindKeyIndex((DeeObject *)self,find_ob)) == -2) return NULL;
 if DEE_UNLIKELY(index == -1) {
  DeeError_ValueError_NotFound((DeeObject *)self,find_ob);
  return NULL;
 }
 return DeeObject_New(Dee_size_t,(Dee_size_t)index);
}
static DeeObject *DEE_CALL _deeset_empty(
 DeeSetObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":empty") != 0) return NULL;
 DeeReturn_Bool(DeeSet_EMPTY(self));
}
static DeeObject *DEE_CALL _deeset_non_empty(
 DeeSetObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":non_empty") != 0) return NULL;
 DeeReturn_Bool(!DeeSet_EMPTY(self));
}
static DeeObject *DEE_CALL _deeset_at(
 DeeSetObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t index;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu:at",&index) != 0) return NULL;
 return DeeSet_GetKeyIndexAt((DeeObject *)self,index);
}
static DeeObject *DEE_CALL _deeset_front(
 DeeSetObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 struct _DeeHashSetBucketEntry *entry; DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":front") != 0) return NULL;
 DeeSet_ACQUIRE(self);
 if DEE_UNLIKELY(!DeeSet_SIZE(self)) {
  DeeSet_RELEASE(self);
  DeeError_ValueError_EmptyList(&DeeSet_Type);
  return NULL;
 }
 entry = _DeeHashSet_FrontEntry(&self->s_set);
 Dee_INCREF(result = _DeeHashSetBucketEntry_KEY(entry));
 DeeSet_RELEASE(self);
 return result;
}
static DeeObject *DEE_CALL _deeset_back(
 DeeSetObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 struct _DeeHashSetBucketEntry *entry; DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":back") != 0) return NULL;
 DeeSet_ACQUIRE(self);
 if DEE_UNLIKELY(!DeeSet_SIZE(self)) {
  DeeSet_RELEASE(self);
  DeeError_ValueError_EmptyList(&DeeSet_Type);
  return NULL;
 }
 entry = _DeeHashSet_BackEntry(&self->s_set);
 Dee_INCREF(result = _DeeHashSetBucketEntry_KEY(entry));
 DeeSet_RELEASE(self);
 return result;
}
static DeeObject *DEE_CALL _deeset_count(
 DeeSetObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *count_ob; int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:count",&count_ob) != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeSet_Contains((DeeObject *)self,count_ob)) < 0) return NULL;
 return DeeObject_New(Dee_size_t,(Dee_size_t)(temp ? 1 : 0));
}

static struct DeeMethodDef const _deeset_tp_methods[] = {
 DEE_METHODDEF_CONST_v100("at",member(&_deeset_at),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("find",member(&_deeset_find),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("index",member(&_deeset_index),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("empty",member(&_deeset_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("non_empty",member(&_deeset_non_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("count",member(&_deeset_count),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("front",member(&_deeset_front),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("back",member(&_deeset_back),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("clear",member(&_deeset_clear),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("insert",member(&_deeset_insert),"(object ob) -> bool"),
 DEE_METHODDEF_v100("insert_all",member(&_deeset_insert_all),"(iterable sequence) -> none\n\tInserts all elements from 'sequence'"),
 DEE_METHODDEF_v100("remove",member(&_deeset_remove),"(object ob) -> bool"),
 DEE_METHODDEF_v100("popitem",member(&_deeset_popitem),"() -> object"),
 DEE_METHODDEF_v100("pop",member(&_deeset_popitem),"() -> object\n\tSame as #(set.popitem)"),
 DEE_METHODDEF_CONST_v100("contains",member(&_deeset_contains),DEE_DOC_AUTO),
 // Alternate names for insert/remove
 DEE_METHODDEF_v100("add",member(&_deeset_insert),"(object ob) -> bool"),
 DEE_METHODDEF_v100("discard",member(&_deeset_remove),"(object ob) -> bool"),
 // TODO: Intrinsic versions of these:
 DEE_METHODDEF_CONST_v100("locate",member(&_deegenericiterable_locate),NULL),
 DEE_METHODDEF_CONST_v100("locate_all",member(&_deegenericiterable_locate_all),NULL),
 DEE_METHODDEF_CONST_v100("reversed",member(&_deegenericiterable_reversed),NULL),
 DEE_METHODDEF_CONST_v100("rfind",member(&_deegenericiterable_rfind),NULL),
 DEE_METHODDEF_CONST_v100("rindex",member(&_deegenericiterable_rindex),NULL),
 DEE_METHODDEF_CONST_v100("rlocate",member(&_deegenericiterable_rlocate),NULL),
 DEE_METHODDEF_CONST_v100("segments",member(&_deegenericiterable_segments),NULL),
 DEE_METHODDEF_CONST_v100("sorted",member(&_deegenericiterable_sorted),NULL),
 DEE_METHODDEF_CONST_v100("transform",member(&_deegenericiterable_transform),NULL),
 DEE_METHODDEF_END_v100
};
static struct DeeMemberDef const _deeset_tp_members[] = {
 DEE_MEMBERDEF_NAMED_v100("max_load_factor",DeeSetObject,s_set.hs_max_load_factor,double),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__s_lock",DeeSetObject,s_lock,DeeAtomicMutex),
 DEE_MEMBERDEF_NAMED_RO_v100("__s_set_hs_max_load_factor",DeeSetObject,s_set.hs_max_load_factor,double),
 DEE_MEMBERDEF_NAMED_RO_v100("__s_set_hs_valid_buckets_begin",DeeSetObject,s_set.hs_valid_buckets_begin,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__s_set_hs_valid_buckets_end",DeeSetObject,s_set.hs_valid_buckets_end,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__s_set_hs_buckets",DeeSetObject,s_set.hs_buckets,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__s_set_hs_bucket_count",DeeSetObject,s_set.hs_bucket_count,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__s_set_hs_map_size",DeeSetObject,s_set.hs_map_size,Dee_size_t),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const _deeset_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeSetIterator_Type),
 DEE_MEMBERDEF_END_v100
};




static int DEE_CALL _deesetiterator_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSetIteratorObject *self, DeeObject *args) {
 DeeSetObject *set;
 if (DeeTuple_Unpack(args,"o:set.iterator",&set) != 0) return -1;
 if (DeeObject_InplaceGetInstance(&set,&DeeSet_Type) != 0) return -1;
 Dee_INCREF(self->si_set = set);
 DeeSet_ACQUIRE(set);
 DeeHashSetIterator_Init(&self->si_iter,DeeSet_SET(set));
 DeeSet_RELEASE(set);
 return 0;
}
static void DEE_CALL _deesetiterator_tp_dtor(DeeSetIteratorObject *self) {
 Dee_DECREF(self->si_set);
}
DEE_VISIT_PROC(_deesetiterator_tp_visit,DeeSetIteratorObject *self) {
 Dee_VISIT(self->si_set);
}
static int DEE_CALL _deesetiterator_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSetIteratorObject *self, DeeSetIteratorObject *right) {
 Dee_INCREF(self->si_set = right->si_set);
 self->si_iter = right->si_iter;
 return 0;
}
static int DEE_CALL _deesetiterator_tp_copy_assign(
 DeeSetIteratorObject *self, DeeSetIteratorObject *right) {
 if (self != right) {
  Dee_DECREF(self->si_set);
  Dee_INCREF(self->si_set = right->si_set);
  self->si_iter = right->si_iter;
 }
 return 0;
}
static int DEE_CALL _deesetiterator_tp_seq_iter_next(
 DeeSetIteratorObject *self, DeeObject **result) {
 struct _DeeHashSetBucketEntry *entry;
 DeeSet_ACQUIRE(self->si_set);
 entry = DeeHashSetIterator_Yield(
  DeeSetIterator_ITER(self),
  DeeSet_SET(DeeSetIterator_SET(self)));
 if (entry) Dee_INCREF(*result = _DeeHashSetBucketEntry_KEY(entry)); else *result = NULL;
 DeeSet_RELEASE(self->si_set);
 return *result ? 0 : 1;
}
static DeeObject *DEE_CALL _deesetiterator_tp_cmp_lo(DeeSetIteratorObject *self, DeeSetIteratorObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeSetIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeHashSetIterator_CompareLo(&self->si_iter,&right->si_iter)); }
static DeeObject *DEE_CALL _deesetiterator_tp_cmp_le(DeeSetIteratorObject *self, DeeSetIteratorObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeSetIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeHashSetIterator_CompareLe(&self->si_iter,&right->si_iter)); }
static DeeObject *DEE_CALL _deesetiterator_tp_cmp_eq(DeeSetIteratorObject *self, DeeSetIteratorObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeSetIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeHashSetIterator_CompareEq(&self->si_iter,&right->si_iter)); }
static DeeObject *DEE_CALL _deesetiterator_tp_cmp_ne(DeeSetIteratorObject *self, DeeSetIteratorObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeSetIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeHashSetIterator_CompareNe(&self->si_iter,&right->si_iter)); }
static DeeObject *DEE_CALL _deesetiterator_tp_cmp_gr(DeeSetIteratorObject *self, DeeSetIteratorObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeSetIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeHashSetIterator_CompareGr(&self->si_iter,&right->si_iter)); }
static DeeObject *DEE_CALL _deesetiterator_tp_cmp_ge(DeeSetIteratorObject *self, DeeSetIteratorObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeSetIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeHashSetIterator_CompareGe(&self->si_iter,&right->si_iter)); }

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deesetiterator_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deesetiterator_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__si_iter_hsi_buckets_begin",DeeSetIteratorObject,si_iter.hsi_buckets_begin,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__si_iter_hsi_buckets_end",DeeSetIteratorObject,si_iter.hsi_buckets_end,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__si_iter_hsi_hash_curr",DeeSetIteratorObject,si_iter.hsi_hash_curr,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__si_iter_hsi_hash_curr_end",DeeSetIteratorObject,si_iter.hsi_hash_curr_end,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__si_set",DeeSetIteratorObject,si_set,object),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */

static int DEE_CALL _deeset_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSetObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_uint64_t read_size; DeeObject *key; int temp;
 if (DeeFile_GetLeSmall64(infile,&read_size) != 0) return -1;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_size > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Set is too big for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_size,(Dee_size_t)-1);
  return -1;
 }
#endif
 DeeHashSet_Init(&self->s_set);
 while (read_size--) {
  if ((key = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {
   DeeHashSet_Quit(&self->s_set);
   return -1;
  }
  temp = DeeHashSet_Insert(&self->s_set,key);
  Dee_DECREF(key);
  if (temp != 0) return -1;
 }
 DeeAtomicMutex_Init(&self->s_lock);
 return 0;
}

static int DEE_CALL _deeset_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSetObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 Dee_size_t size; int temp;
 DeeHashSet_TRAVERSE_SAFE_VARS; DeeObject *key;
 struct _DeeHashSetBucketEntry *entry;
 DeeSet_ACQUIRE(self);
 size = self->s_set.hs_map_size;
 DeeSet_RELEASE(self);
 if (DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)size) != 0) return -1;
 DeeSet_ACQUIRE(self);
 DeeHashSet_TRAVERSE_SAFE(entry,&self->s_set) {
  Dee_INCREF(key = entry->sbe_key);
  DeeSet_RELEASE(self);
  temp = DeeMarshal_WriteObjectWithMap(outfile,map,key);
  Dee_DECREF(key);
  if (temp != 0) return temp;
  --size;
  DeeSet_ACQUIRE(self);
 }
 DeeSet_RELEASE(self);
 // Write any objects that we weren't able to
 // retrieve due to another thread modifying the set.
 while (size--) if (DeeMarshal_WriteObjectWithMap(outfile,map,Dee_None) != 0) return -1;
 return 0;
}

static struct DeeTypeMarshal _deeset_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(set)),
 member(&_deeset_tp_marshal_ctor),member(&_deeset_tp_marshal_put));



DeeTypeObject DeeSet_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("set"),null,member(
 DEE_TYPE_FLAG_CONST_CTOR|DEE_TYPE_FLAG_HAS_GC|DEE_TYPE_FLAG_MUST_COPY|/*DEE_TYPE_FLAG_ALWAYS_CLEAR|*/
 DEE_TYPE_FLAG_NO_ITERATE_EFFECT|DEE_TYPE_FLAG_FUNDAMENTAL),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeSetObject),
  member(&_DeeGC_TpAlloc),member(&_deeset_tp_ctor),
  member(&_deeset_tp_copy_ctor),member(&_deeset_tp_move_ctor),
  member(&_deeset_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(member(&_DeeGC_TpFree),
  member(&_deeset_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deeset_tp_copy_assign),member(&_deeset_tp_move_assign),
  member(&_deeset_tp_any_assign)),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deeset_tp_str),member(&_deeset_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deeset_tp_visit),member(&DeeSet_Clear)),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deeset_tp_not),member(&_deeset_tp_bool),
  null,null,null,null,null,null,null,
  member(&_deeset_tp_add),member(&_deeset_tp_inplace_add),
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  member(&_deeset_tp_hash)),
 DEE_TYPE_OBJECT_COMPARE_v100(
  // TODO: intrinsic set operators
  member(&_deegenericiterable_tp_cmp_lo),member(&_deegenericiterable_tp_cmp_le),
  member(&_deegenericiterable_tp_cmp_eq),member(&_deegenericiterable_tp_cmp_ne),
  member(&_deegenericiterable_tp_cmp_gr),member(&_deegenericiterable_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&_deeset_tp_seq_get),null,null,member(&_deeset_tp_seq_size),
  member(&_deeset_tp_seq_contains),member(&_deeset_tp_seq_range_get),null,null,
  member(&DeeSet_IterSelf),null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deeset_tp_members),null,member(_deeset_tp_methods),
  member(_deeset_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deeset_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};
DeeTypeObject DeeSetIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("set.iterator"),null,null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeSetIteratorObject),null,null,
  member(&_deesetiterator_tp_copy_ctor),null,
  member(&_deesetiterator_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deesetiterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deesetiterator_tp_copy_assign),null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deesetiterator_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deesetiterator_tp_cmp_lo),member(&_deesetiterator_tp_cmp_le),
  member(&_deesetiterator_tp_cmp_eq),member(&_deesetiterator_tp_cmp_ne),
  member(&_deesetiterator_tp_cmp_gr),member(&_deesetiterator_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,
  member(&_deesetiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deesetiterator_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};



DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeSet_Insert(
 DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *ob) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 return DeeHashSet_InsertWithLock(DeeSet_SET(self),ob,DeeSet_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeSet_InsertWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *ob, DEE_A_IN Dee_hash_t bucket) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 return DeeHashSet_InsertWithKnownBucketAndLock(DeeSet_SET(self),ob,bucket,DeeSet_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeSet_Remove(
 DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *ob) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 return DeeHashSet_RemoveWithLock(DeeSet_SET(self),ob,DeeSet_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeSet_RemoveWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *ob, DEE_A_IN Dee_hash_t bucket) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 return DeeHashSet_RemoveWithKnownBucketAndLock(DeeSet_SET(self),ob,bucket,DeeSet_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeSet_Contains(
 DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *ob) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 return DeeHashSet_ContainsWithLock(DeeSet_SET(self),ob,DeeSet_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeSet_ContainsPred(
 DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *ob, DEE_A_INOUT DeeObject *pred) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 return DeeHashSet_ContainsPredWithLock(DeeSet_SET(self),ob,pred,DeeSet_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeSet_ContainsWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *ob, DEE_A_IN Dee_hash_t bucket) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 return DeeHashSet_ContainsWithKnownBucketAndLock(DeeSet_SET(self),ob,bucket,DeeSet_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeSet_ContainsPredWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeSetObject) *self, DEE_A_INOUT DeeObject *ob,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *pred) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 return DeeHashSet_ContainsPredWithKnownBucketAndLock(
  DeeSet_SET(self),ob,bucket,pred,DeeSet_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeSet_PopSomething(
 DEE_A_INOUT_OBJECT(DeeSetObject) *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 return DeeHashSet_PopSomethingWithLock(DeeSet_SET(self),DeeSet_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeSet_GetKeyIndex(
 DEE_A_IN_OBJECT(DeeSetObject) const *self, DEE_A_IN Dee_ssize_t index) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 DeeSet_ACQUIRE(self);
 if (!DeeSet_SIZE(self)) {
  DeeSet_RELEASE(self);
  DeeError_ValueError_EmptyList(&DeeSet_Type);
  return NULL;
 }
 DEE_PRIVATE_CLAMP_INDEX(index,DeeSet_SIZE(self));
 Dee_INCREF(result = DeeHashSet_GetKeyIndex(DeeSet_SET(self),(Dee_size_t)index));
 DeeSet_RELEASE(self);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeSet_GetKeyIndexAt(
 DEE_A_IN_OBJECT(DeeSetObject) const *self, DEE_A_IN Dee_size_t index) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 DeeSet_ACQUIRE(self);
 if (index >= DeeSet_SIZE(self)) {
  DeeSet_RELEASE(self);
  DeeError_IndexError((DeeObject *)self,index);
  return NULL;
 }
 Dee_INCREF(result = DeeHashSet_GetKeyIndex(DeeSet_SET(self),index));
 DeeSet_RELEASE(self);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeSet_GetKeyRangeIndex(
 DEE_A_IN_OBJECT(DeeSetObject) const *self, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 return DeeHashSet_GetKeyRangeIndexWithLock(DeeSet_SET(self),lo,hi,DeeSet_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeSet_FindKeyIndex(
 DEE_A_IN_OBJECT(DeeSetObject) const *self, DEE_A_INOUT DeeObject *key) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 return DeeHashSet_FindKeyIndexWithLock(DeeSet_SET(self),key,DeeSet_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeSet_FindKeyIndexWithKnownBucket(
 DEE_A_IN_OBJECT(DeeSetObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 return DeeHashSet_FindKeyIndexWithKnownBucketAndLock(DeeSet_SET(self),key,bucket,DeeSet_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) int DeeSet_Traverse(
 DEE_A_IN_OBJECT(DeeSetObject) const *self,
 DEE_A_IN DeeSetTraverseFunc trav,
 DEE_A_IN_OPT void *closure) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSet_Check(self));
 return DeeHashSet_TraverseWithLock(DeeSet_SET(self),trav,closure,DeeSet_LOCK(self));
}

#ifndef __INTELLISENSE__
#ifndef DEE_WITHOUT_THREADS
#define WITH_LOCK
#include "set.impl.hashset.inl"
#endif
#include "set.impl.hashset.inl"
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_SET_C */
