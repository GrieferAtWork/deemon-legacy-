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
#ifndef GUARD_DEEMON_DEX_DEQUE_C_INL
#define GUARD_DEEMON_DEX_DEQUE_C_INL 1

#include "deque.h.inl"
#include "_sequence.h"
#include <deemon/optional/object_refcnt.h>
#include <deemon/memberdef.h>
#include <deemon/bool.h>
#include <deemon/gc.h>
#include <deemon/deemonrun.h>
#include <deemon/generic_vtable.h>
#include <deemon/none.h>
#include <deemon/type.h>
#include <deemon/error.h>
#include <deemon/tuple.h>
#include <deemon/string.h>
#include <deemon/optional/type_decl.h>
#include <deemon/optional/type_flags.h>
#include <deemon/optional/type_slots.h>
#include <deemon/optional/type_util.h>
#include <deemon/optional/object_visit.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/tuple.h>
#include <deemon/super.h>
#include <deemon/list.h>
#include <deemon/runtime/builtin_functions.h>

DEE_DECL_BEGIN


DEE_A_RET_EXCEPT(-1) int DeeDeque_InitFromVectorEx(
 DEE_A_OUT struct DeeDeque *self, DEE_A_IN Dee_size_t bucket_size,
 DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(elemc) DeeObject **elemv) {
 struct DeeDequeBucket *bucket_iter,*bucket_end;
 DeeObject **copy_iter,**copy_end,**copy_src;
 Dee_size_t used_begin_storage; /*< Amount of bucket-slots used in the front slot. */
 Dee_size_t used_end_storage; /*< Amount of bucket-slots used in the back slot. */
 Dee_size_t n_overallocated_slots;
 Dee_size_t unused_begin_storage;
 Dee_size_t elemv_byte_size;
 DEE_ASSERT(self);
 DEE_ASSERT(!elemc || elemv);
 DEE_ASSERTF(bucket_size != 0,"Invalid bucket_size");
 if (!elemc) { DeeDeque_InitEx(self,bucket_size); return 0; }
 copy_src = elemv;
 self->d_bucketsize = bucket_size;
 self->d_bucketc = elemc/bucket_size;
 if ((elemc%bucket_size)!=0) ++self->d_bucketc;
 DEE_ASSERT(self->d_bucketc*bucket_size >= elemc);
 if (self->d_bucketc == 1) {
  DEE_ASSERT(elemc <= bucket_size);
  while DEE_UNLIKELY((self->d_bucketv = (struct DeeDequeBucket *)
   DEE_DEQUE_DCALLOC_NZ(sizeof(struct DeeDequeBucket))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
  while DEE_UNLIKELY((self->d_bucketv[0].db_elemv = (DeeObject **)
   DEE_DEQUE_DCALLOC_NZ(bucket_size*sizeof(DeeObject *))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   free_nn(self->d_bucketv);
   DeeError_NoMemory();
   return -1;
  }
  // Special case: Only one bucket
  // >> Here we try to center the actual data within the bucket,
  //    thus keeping the same (+-1) amount of free slots before
  //    and after the generated block.
  copy_iter = self->d_bucketv[0].db_elemv+((bucket_size-elemc)/2);
  copy_end = copy_iter+elemc;
  self->d_begin = copy_iter;
  self->d_end = copy_end;
  DEE_ASSERT(copy_iter != copy_end);
  do Dee_INCREF(*copy_iter++ = *copy_src++);
  while (copy_iter != copy_end);
  // Set the buffers to match the actual size
  self->d_bucketa = self->d_bucketc;
  self->d_bucketelema = self->d_bucketc;
  self->d_bucketelemv = self->d_bucketv;
  self->d_bucketroot = self->d_bucketv;
  self->d_elemc = elemc;
  return 0;
 }

 DEE_ASSERT(elemc > bucket_size);
 n_overallocated_slots = (self->d_bucketc*bucket_size)-elemc;
 unused_begin_storage = n_overallocated_slots/2;
 used_begin_storage = bucket_size-unused_begin_storage;
 used_end_storage = used_begin_storage;
 if ((n_overallocated_slots%2)!=0) --used_end_storage;
 DEE_ASSERT(bucket_size != 2 || used_begin_storage+used_end_storage == elemc);
 DEE_ASSERT(used_begin_storage <= bucket_size);
 DEE_ASSERT(used_end_storage <= bucket_size);
 // Allocate the bucket-vector
 while DEE_UNLIKELY((self->d_bucketv = (struct DeeDequeBucket *)
  DEE_DEQUE_DCALLOC_NZ(self->d_bucketc*sizeof(struct DeeDequeBucket))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 bucket_end = (bucket_iter = self->d_bucketv)+self->d_bucketc;
 // Allocate all the vectors stored in all the buckets
 elemv_byte_size = sizeof(DeeObject *)*bucket_size;
 DEE_ASSERT(elemv_byte_size);
 while (bucket_iter != bucket_end) {
  while DEE_UNLIKELY((bucket_iter->db_elemv = (DeeObject **)
   DEE_DEQUE_DCALLOC_NZ(elemv_byte_size)) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   while (bucket_iter != self->d_bucketv) { --bucket_iter; free_nn(bucket_iter->db_elemv); }
   free_nn(self->d_bucketv);
   DeeError_NoMemory();
   return -1;
  }
  ++bucket_iter;
 }

 // Set the buffers to match the actual size
 self->d_bucketa = self->d_bucketc;
 self->d_bucketelema = self->d_bucketc;
 self->d_bucketelemv = self->d_bucketv;
 self->d_bucketroot = self->d_bucketv;
 self->d_elemc = elemc;

 // Begin filling in the allocated memory
 copy_iter = self->d_bucketv[0].db_elemv+(bucket_size-used_begin_storage);
 self->d_begin = copy_iter;
 DEE_ASSERT(self->d_bucketc >= 2);
 copy_end = copy_iter+used_begin_storage;
 DEE_ASSERT(copy_iter != copy_end);
 do Dee_INCREF(*copy_iter++ = *copy_src++);
 while (copy_iter != copy_end);
 // Copy all full buckets
 bucket_end = (bucket_iter = self->d_bucketv+1)+(self->d_bucketc-2);
 while (bucket_iter != bucket_end) {
  copy_end = (copy_iter = bucket_iter->db_elemv)+bucket_size;
  DEE_ASSERT(copy_iter != copy_end);
  do Dee_INCREF(*copy_iter++ = *copy_src++);
  while (copy_iter != copy_end);
  ++bucket_iter;
 }
 // Copy the last, partially full bucket
 copy_iter = self->d_bucketv[self->d_bucketc-1].db_elemv;
 copy_end = copy_iter+used_end_storage;
 self->d_end = copy_end;
 DEE_ASSERT(copy_iter != copy_end);
 do Dee_INCREF(*copy_iter++ = *copy_src++);
 while (copy_iter != copy_end);
 DEE_ASSERTF(copy_src == elemv+elemc,"Too many/not enough object inserted");
 DeeDeque_AssertIntegrity(self);
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeDeque_InitFromIteratorEx(
 DEE_A_OUT struct DeeDeque *self, DEE_A_IN Dee_size_t bucket_size,
 DEE_A_INOUT DeeObject *iterator) {
 DeeObject *temp; int error;
 // TODO: Optimized version
 if ((temp = DeeList_NewFromIterator(iterator)) == NULL) return -1;
 error = DeeDeque_InitFromVectorEx(self,bucket_size,
                                   DeeList_SIZE(temp),
                                   DeeList_ELEM(temp));
 Dee_DECREF(temp);
 return error;
}
DEE_A_RET_EXCEPT(-1) int DeeDeque_TInitFromSequenceEx(
 DEE_A_OUT struct DeeDeque *self, DEE_A_IN Dee_size_t bucket_size,
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) {
 DeeObject *iterator; int error;
 if (tp_sequence == &DeeTuple_Type) return DeeDeque_InitFromVectorEx(
  self,bucket_size,DeeTuple_SIZE(sequence),DeeTuple_ELEM(sequence));
 if (tp_sequence == &DeeSuper_Type) return DeeDeque_TInitFromSequenceEx(
  self,bucket_size,DeeSuper_TYPE(sequence),DeeSuper_SELF(sequence));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_sequence,sequence)) == NULL) return -1;
 error = DeeDeque_InitFromIteratorEx(self,bucket_size,iterator);
 Dee_DECREF(iterator);
 return error;
}

void DeeDeque_Quit(DEE_A_IN struct DeeDeque *self) {
 struct DeeDequeBucket *iter,*end;
 DeeObject **elem_iter,**elem_end;
 DeeDeque_AssertIntegrity(self);
 if (self->d_elemc) {
  // Clear the first bucket
  elem_iter = self->d_begin;
  DEE_ASSERT(elem_iter >= self->d_bucketv->db_elemv &&
             elem_iter < self->d_bucketv->db_elemv+self->d_bucketsize);
  if (self->d_bucketc == 1) {
   // Special case: Only need to delete a single bucket partially
   elem_end = self->d_end;
   DEE_ASSERT(elem_end > self->d_bucketv->db_elemv &&
              elem_end <= self->d_bucketv->db_elemv+self->d_bucketsize);
   DEE_ASSERT(elem_iter < elem_end);
   DEE_ASSERT(elem_end == elem_iter+self->d_elemc);
   do Dee_DECREF(*elem_iter);
   while (++elem_iter != elem_end);
  } else {
   elem_end = self->d_bucketv->db_elemv+self->d_bucketsize;
   DEE_ASSERT(elem_iter != elem_end);
   do Dee_DECREF(*elem_iter);
   while (++elem_iter != elem_end);
   // Clear all intermediate buckets
   DEE_ASSERT(self->d_bucketc >= 2);
   end = (iter = self->d_bucketv+1)+(self->d_bucketc-2);
   while (iter != end) {
    DEE_ASSERT(iter->db_elemv);
    DEE_ASSERT(self->d_bucketsize);
    elem_end = (elem_iter = iter->db_elemv)+self->d_bucketsize;
    do Dee_DECREF(*elem_iter);
    while (++elem_iter != elem_end);
    ++iter;
   }
   // Clear the end bucket
   DEE_ASSERT(end == self->d_bucketv+self->d_bucketc-1);
   elem_end = self->d_end;
   DEE_ASSERT(elem_end > end->db_elemv &&
              elem_end <= end->db_elemv+self->d_bucketsize);
   elem_iter = end->db_elemv;
   DEE_ASSERT(elem_iter != elem_end);
   do Dee_DECREF(*elem_iter);
   while (++elem_iter != elem_end);
  }
 }

 // Free all allocated element vectors
 end = (iter = self->d_bucketelemv)+self->d_bucketelema;
 while (iter != end) { free_nn(iter->db_elemv); ++iter; }

 // Free the root bucket list
 free(self->d_bucketroot);
}


void _DeeDeque_ShiftRight(
 DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t free_start,
 DEE_A_IN Dee_size_t free_count) {
 struct _DeeDequeFastIterator src_iter,dst_iter;
 Dee_size_t shift_count;
 DeeDeque_AssertIntegrity(self);
 shift_count = self->d_elemc-free_start;
 // Consume elements on the right
 DeeDeque_CONSUME_RIGHT_N(self,free_count);
 DeeDeque_AssertIntegrity(self);
 _DeeDequeFastIterator_InitReverse(&src_iter,self,self->d_elemc-free_count);
 _DeeDequeFastIterator_InitReverse(&dst_iter,self,self->d_elemc);
 // Must Iterate downwards to shift everything to the right
 // without overriding anything we'll still need (aka. the memmove rule)
 while (shift_count--) {
  _DeeDequeFastIterator_Prev(&src_iter,self);
  _DeeDequeFastIterator_Prev(&dst_iter,self);
  *_DeeDequeFastIterator_ELEM(&dst_iter) = *_DeeDequeFastIterator_ELEM(&src_iter);
#ifdef DEE_DEBUG
  *_DeeDequeFastIterator_ELEM(&src_iter) = NULL;
#endif
 }
}
void _DeeDeque_ShiftLeft(
 DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t free_start,
 DEE_A_IN Dee_size_t free_count) {
 struct _DeeDequeFastIterator src_iter,dst_iter;
 Dee_size_t shift_count;
 DeeDeque_AssertIntegrity(self);
 shift_count = free_start;
 // Consume elements on the left
 DeeDeque_CONSUME_LEFT_N(self,free_count);
 _DeeDequeFastIterator_InitForward(&src_iter,self,free_count);
 _DeeDequeFastIterator_InitFront(&dst_iter,self);
 // Must Iterate upwards to shift everything to the right
 // without overriding anything we'll still need (aka. the memmove rule)
 while (shift_count--) {
  *_DeeDequeFastIterator_ELEM(&dst_iter) = *_DeeDequeFastIterator_ELEM(&src_iter);
#ifdef DEE_DEBUG
  *_DeeDequeFastIterator_ELEM(&src_iter) = NULL;
#endif
  _DeeDequeFastIterator_Next(&src_iter,self);
  _DeeDequeFastIterator_Next(&dst_iter,self);
 }
}



#ifndef __INTELLISENSE__
#define WITH_LOCK
#include "deque.impl.inl"
#include "deque.impl.inl"
#endif


//////////////////////////////////////////////////////////////////////////
// VTable: deque
static int DEE_CALL _deedeque_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeDequeObject *self) {
 DeeDeque_Init(&self->d_deq);
 DeeAtomicMutex_Init(&self->d_lock);
 return 0;
}
static int DEE_CALL _deedeque_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeDequeObject *self, DeeDequeObject *right) {
 if (DeeDeque_InitCopyWithLock(&self->d_deq,&right->d_deq,&right->d_lock) != 0) return -1;
 DeeAtomicMutex_Init(&self->d_lock);
 return 0;
}
static int DEE_CALL _deedeque_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeDequeObject *self, DeeDequeObject *right) {
 DeeDeque_InitMoveWithLock(&self->d_deq,&right->d_deq,&right->d_lock);
 DeeAtomicMutex_Init(&self->d_lock);
 return 0;
}
static int DEE_CALL _deedeque_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeDequeObject *self, DeeObject *args) {
 DeeObject *sequence; Dee_size_t bucket_size = DEE_DEQUE_DEFUALT_BUCKET_SIZE;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|Iu:deque",&sequence,&bucket_size) != 0) return -1;
 DeeAtomicMutex_Init(&self->d_lock);
 return DeeDeque_InitFromSequenceEx(&self->d_deq,bucket_size,sequence);
}
static int DEE_CALL _deedeque_tp_copy_assign(DeeDequeObject *self, DeeDequeObject *right) {
 struct DeeDeque new_deque,old_deque;
 if DEE_UNLIKELY(DeeDeque_InitCopyWithLock(&new_deque,&right->d_deq,&right->d_lock) != 0) return -1;
 DeeAtomicMutex_AcquireRelaxed(&self->d_lock);
 memcpy(&old_deque,&self->d_deq,sizeof(struct DeeDeque));
 memcpy(&self->d_deq,&new_deque,sizeof(struct DeeDeque));
 DeeAtomicMutex_Release(&self->d_lock);
 DeeDeque_Quit(&old_deque);
 return 0;
}
static int DEE_CALL _deedeque_tp_move_assign(DeeDequeObject *self, DeeDequeObject *right) {
 struct DeeDeque new_deque,old_deque;
 DeeDeque_InitMoveWithLock(&new_deque,&right->d_deq,&right->d_lock);
 DeeAtomicMutex_AcquireRelaxed(&self->d_lock);
 memcpy(&old_deque,&self->d_deq,sizeof(struct DeeDeque));
 memcpy(&self->d_deq,&new_deque,sizeof(struct DeeDeque));
 DeeAtomicMutex_Release(&self->d_lock);
 DeeDeque_Quit(&old_deque);
 return 0;
}
static int DEE_CALL _deedeque_tp_any_assign(DeeDequeObject *self, DeeObject *sequence) {
 struct DeeDeque new_deque,old_deque;
 if (DeeDeque_InitFromSequence(&new_deque,sequence) != 0) return -1;
 DeeAtomicMutex_AcquireRelaxed(&self->d_lock);
 memcpy(&old_deque,&self->d_deq,sizeof(struct DeeDeque));
 memcpy(&self->d_deq,&new_deque,sizeof(struct DeeDeque));
 DeeAtomicMutex_Release(&self->d_lock);
 DeeDeque_Quit(&old_deque);
 return 0;
}
static void DEE_CALL _deedeque_tp_dtor(DeeDequeObject *self) { DeeDeque_Quit(&self->d_deq); }
static void DEE_CALL _deedeque_tp_clear(DeeDequeObject *self) { DeeDeque_Clear(&self->d_deq); }
static int DEE_CALL _deedeque_tp_bool(DeeDequeObject *self) { return !DeeDeque_EMPTY(&self->d_deq); }
static DeeObject *DEE_CALL _deedeque_tp_not(DeeDequeObject *self) { DeeReturn_Bool(DeeDeque_EMPTY(&self->d_deq)); }
DEE_VISIT_PROC(_deedeque_tp_visit,DeeDequeObject *self) {
 DeeDeque_TRAVERSE_VARS; DeeObject *elem;
 DeeDeque_ACQUIRE(self);
 DeeDeque_TRAVERSE(elem,&self->d_deq) Dee_VISIT(elem);
 DeeDeque_RELEASE(self);
}
static DeeObject *DEE_CALL _deedeque_tp_str(DeeDequeObject *self) {
 int first = 1; DeeObject *elem,*result; DeeDeque_TRAVERSE_SAFE_VARS;
 DeeStringWriter writer = DeeStringWriter_INIT();
 if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,"{ ") != 0) goto err;
 DeeDeque_ACQUIRE(self);
 DeeDeque_TRAVERSE_SAFE(elem,&self->d_deq) {
  Dee_INCREF(elem);
  DeeDeque_RELEASE(self);
  if DEE_UNLIKELY(first) first = 0;
  else if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,",") != 0) {
err_elem: Dee_DECREF(elem); goto err;
  }
  if DEE_UNLIKELY(DeeStringWriter_WriteObjectStr(&writer,elem) != 0) goto err_elem;
  Dee_DECREF(elem);
  DeeDeque_ACQUIRE(self);
 }
 DeeDeque_RELEASE(self);
 if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer," }") != 0) goto err;
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}
static DeeObject *DEE_CALL _deedeque_tp_repr(DeeDequeObject *self) {
 int first = 1; DeeObject *elem,*result; DeeDeque_TRAVERSE_SAFE_VARS;
 DeeStringWriter writer = DeeStringWriter_INIT();
 if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,"deque({ ") != 0) goto err;
 DeeDeque_ACQUIRE(self);
 DeeDeque_TRAVERSE_SAFE(elem,&self->d_deq) {
  Dee_INCREF(elem);
  DeeDeque_RELEASE(self);
  if DEE_UNLIKELY(first) first = 0;
  else if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,",") != 0) {
err_elem: Dee_DECREF(elem); goto err;
  }
  if DEE_UNLIKELY(DeeStringWriter_WriteObjectRepr(&writer,elem) != 0) goto err_elem;
  Dee_DECREF(elem);
  DeeDeque_ACQUIRE(self);
 }
 DeeDeque_RELEASE(self);
 if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer," })") != 0) goto err;
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}
static DeeObject *DEE_CALL _deedeque_tp_seq_get(
 DeeDequeObject *self, DeeObject *index) {
 DeeObject *result; Dee_ssize_t i;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,index,&i) != 0) return NULL;
 DeeDeque_ACQUIRE(self);
 if (DeeDeque_EMPTY(&self->d_deq)) {
  DeeDeque_RELEASE(self);
  _sequence_emptyerror();
  return NULL;
 }
 DEE_PRIVATE_CLAMP_INDEX(i,DeeDeque_SIZE(&self->d_deq));
 Dee_INCREF(result = DeeDeque_GET_NZ(&self->d_deq,(Dee_size_t)i));
 DeeDeque_RELEASE(self);
 return result;
}
static int DEE_CALL _deedeque_tp_seq_del(
 DeeDequeObject *self, DeeObject *index) {
 Dee_ssize_t i;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,index,&i) != 0) return -1;
 DeeDeque_ACQUIRE(self);
 if (DeeDeque_EMPTY(&self->d_deq)) {
  DeeDeque_RELEASE(self);
  _sequence_emptyerror();
  return -1;
 }
 DEE_PRIVATE_CLAMP_INDEX(i,DeeDeque_SIZE(&self->d_deq));
 return DeeDeque_EraseReleaseLock(&self->d_deq,(Dee_size_t)i,1,&self->d_lock);
}
static int DEE_CALL _deedeque_tp_seq_set(
 DeeDequeObject *self, DeeObject *index, DeeObject *v) {
 DeeObject **elemp,*old_elem; Dee_ssize_t i;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,index,&i) != 0) return -1;
 DeeDeque_ACQUIRE(self);
 if (DeeDeque_EMPTY(&self->d_deq)) {
  DeeDeque_RELEASE(self);
  _sequence_emptyerror();
  return -1;
 }
 DEE_PRIVATE_CLAMP_INDEX(i,DeeDeque_SIZE(&self->d_deq));
 elemp = DeeDeque_ELEM_NZ(&self->d_deq,(Dee_size_t)i);
 old_elem = *elemp;
 DEE_ASSERT(old_elem);
 Dee_INCREF(*elemp = v);
 DeeDeque_RELEASE(self);
 Dee_DECREF(old_elem);
 return 0;
}
static DeeObject *DEE_CALL _deedeque_tp_seq_range_get(
 DeeDequeObject *self, DeeObject *begino, DeeObject *endo) {
 Dee_ssize_t begin,end;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,begino,&begin) != 0) return -1;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,endo,&end) != 0) return -1;
 return DeeDeque_GetRangeWithLock(&self->d_deq,begin,end,&self->d_lock);
}
static int DEE_CALL _deedeque_tp_seq_range_del(
 DeeDequeObject *self, DeeObject *begino, DeeObject *endo) {
 Dee_ssize_t begin,end;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,begino,&begin) != 0) return -1;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,endo,&end) != 0) return -1;
 if (begin < 0) begin = 0; if (end < 0) end = 0;
 if (end >= begin) return 0;
 return DeeDeque_EraseWithLock(&self->d_deq,(Dee_size_t)begin,(Dee_size_t)(end-begin),&self->d_lock);
}
static DeeObject *DEE_CALL _deedeque_tp_seq_size(DeeDequeObject *self) {
 Dee_size_t result;
 DeeDeque_ACQUIRE(self);
 result = DeeDeque_SIZE(&self->d_deq);
 DeeDeque_RELEASE(self);
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *DEE_CALL _deedeque_tp_seq_contains(
 DeeDequeObject *self, DeeObject *elem) {
 int error;
 if DEE_UNLIKELY((error = DeeDeque_ContainsWithLock(
  &self->d_deq,elem,&self->d_lock)) < 0) return NULL;
 DeeReturn_Bool(error);
}

static DeeDequeIteratorObject *DEE_CALL
_deedeque_tp_seq_iter_self(DeeDequeObject *self) {
 DeeDequeIteratorObject *result;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeDequeIteratorObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeDequeIterator_Type);
 Dee_INCREF(result->di_deq = self);
 DeeDeque_ACQUIRE(self);
 DeeDequeIterator_InitBegin(&result->di_iter,&self->d_deq);
 DeeDeque_RELEASE(self);
 DeeAtomicMutex_Init(&result->di_lock);
 return result;
}
static DeeObject *DEE_CALL _deedeque_clear(
 DeeDequeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":clear") != 0) return NULL;
 DeeDeque_ClearWithLock(&self->d_deq,&self->d_lock);
 DeeReturn_None;
}
static DeeDequeIteratorObject *DEE_CALL _deedeque_begin(
 DeeDequeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeDequeIteratorObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":begin") != 0) return NULL;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeDequeIteratorObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeDequeIterator_Type);
 Dee_INCREF(result->di_deq = self);
 DeeDeque_ACQUIRE(self);
 DeeDequeIterator_InitBegin(&result->di_iter,&self->d_deq);
 DeeDeque_RELEASE(self);
 DeeAtomicMutex_Init(&result->di_lock);
 return result;
}
static DeeDequeIteratorObject *DEE_CALL _deedeque_end(
 DeeDequeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeDequeIteratorObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":end") != 0) return NULL;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeDequeIteratorObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeDequeIterator_Type);
 Dee_INCREF(result->di_deq = self);
 DeeDeque_ACQUIRE(self);
 DeeDequeIterator_InitEnd(&result->di_iter,&self->d_deq);
 DeeDeque_RELEASE(self);
 DeeAtomicMutex_Init(&result->di_lock);
 return result;
}
static DeeObject *DEE_CALL _deedeque_push_front(
 DeeDequeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(((DeeTuple_SIZE(args) == 1)
  ? DeeDeque_PushFrontWithLock(&self->d_deq,DeeTuple_GET(args,0),&self->d_lock)
  : DeeDeque_PushFrontVectorWithLock(&self->d_deq,DeeTuple_SIZE(args),DeeTuple_ELEM(args),&self->d_lock)
  ) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deedeque_push_back(
 DeeDequeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(((DeeTuple_SIZE(args) == 1)
  ? DeeDeque_PushBackWithLock(&self->d_deq,DeeTuple_GET(args,0),&self->d_lock)
  : DeeDeque_PushBackVectorWithLock(&self->d_deq,DeeTuple_SIZE(args),DeeTuple_ELEM(args),&self->d_lock)
  ) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deedeque_pop_front(
 DeeDequeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":pop_front") != 0) return NULL;
 if DEE_UNLIKELY((result = DeeDeque_PopFrontWithLock(&self->d_deq,&self->d_lock)) == NULL) return NULL;
 return result;
}
static DeeObject *DEE_CALL _deedeque_pop_back(
 DeeDequeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":pop_back") != 0) return NULL;
 if DEE_UNLIKELY((result = DeeDeque_PopBackWithLock(&self->d_deq,&self->d_lock)) == NULL) return NULL;
 return result;
}
static DeeObject *DEE_CALL _deedeque_insert(
 DeeDequeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t i; DeeObject *elem;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iuo:insert",&i,&elem) != 0) return NULL;
 if DEE_UNLIKELY(DeeDeque_InsertWithLock(&self->d_deq,i,elem,&self->d_lock) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deedeque_insert_list(
 DeeDequeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t i; DeeObject *seq;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iuo:insert_list",&i,&seq) != 0) return NULL;
 if DEE_UNLIKELY(DeeDeque_InsertSequenceWithLock(&self->d_deq,i,seq,&self->d_lock) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deedeque_insert_iter(
 DeeDequeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t i; DeeObject *iter;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iuo:insert_iter",&i,&iter) != 0) return NULL;
 if DEE_UNLIKELY(DeeDeque_InsertIteratorWithLock(&self->d_deq,i,iter,&self->d_lock) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deedeque_erase(
 DeeDequeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t i,n = 1;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu|Iu:erase",&i,&n) != 0) return NULL;
 if DEE_UNLIKELY(DeeDeque_EraseWithLock(&self->d_deq,i,n,&self->d_lock) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deedeque_remove(
 DeeDequeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem,*pred = Dee_None; int error;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:remove",&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((error = ((DeeNone_Check(pred) && pred != &DeeBuiltinFunction___eq__)
  ? DeeDeque_RemoveWithLock(&self->d_deq,elem,&self->d_lock)
  : DeeDeque_RemovePredWithLock(&self->d_deq,elem,pred,&self->d_lock)
  )) < 0) return NULL;
 DeeReturn_Bool(error);
}
static DeeObject *DEE_CALL _deedeque_remove_if(
 DeeDequeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred = Dee_None; Dee_size_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:remove",&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeDeque_RemoveIfWithLock(
  &self->d_deq,pred,&self->d_lock)) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *DEE_CALL _deedeque_shrink_to_fit(
 DeeDequeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint32_t flags = DEE_DEQUE_SHRINKTOFIT_FLAG_DEFAULT;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|I32u:shrink_to_fit",&flags) != 0) return NULL;
 DeeDeque_ShrinkToFitWithLock(&self->d_deq,flags,&self->d_lock);
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deedeque_tp_bucketrepr(
 DeeDequeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t n_buckets,bucket_size,front_unused,back_unused,digmax,i,full_max;
 DeeObject *result; DeeStringWriter writer = DeeStringWriter_INIT();
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":bucketrepr") != 0) return NULL;
 DeeDeque_ACQUIRE(self);
 DeeDeque_AssertIntegrity(&self->d_deq);
 if (DeeDeque_EMPTY(&self->d_deq)) {
  DeeDeque_RELEASE(self);
 } else {
  n_buckets = self->d_deq.d_bucketc;
  bucket_size = self->d_deq.d_bucketsize;
  front_unused = DeeDeque_FRONT_UNUSED_NZ(&self->d_deq);
  back_unused = DeeDeque_BACK_UNUSED_NZ(&self->d_deq);
  DeeDeque_RELEASE(self);
  if (n_buckets == 1) {
   if (DeeStringWriter_WRITE_STRING(&writer,"0 |") != 0) goto err;
   if (DeeStringWriter_RepeatChar(&writer,' ',front_unused) != 0) goto err;
   if (DeeStringWriter_RepeatChar(&writer,'O',bucket_size-(front_unused+back_unused)) != 0) goto err;
   if (DeeStringWriter_RepeatChar(&writer,' ',back_unused) != 0) goto err;
   if (DeeStringWriter_WRITE_STRING(&writer,"|\n") != 0) goto err;
  } else {
   i = n_buckets,digmax = 1;
   while ((i /= 10) != 0) ++digmax;
   if (DeeStringWriter_RepeatChar(&writer,'0',digmax) != 0) goto err;
   if (DeeStringWriter_WRITE_STRING(&writer," |") != 0) goto err;
   if (DeeStringWriter_RepeatChar(&writer,' ',front_unused) != 0) goto err;
   if (DeeStringWriter_RepeatChar(&writer,'O',bucket_size-front_unused) != 0) goto err;
   if (DeeStringWriter_WRITE_STRING(&writer,"|\n") != 0) goto err;
   full_max = Dee_MIN((n_buckets-1),16);
   for (i = 1; i < full_max; ++i) {
    if (digmax >= 2) {
     if (DeeStringWriter_RepeatChar(&writer,'0',digmax-2) != 0) goto err;
     if (DeeStringWriter_Writef(&writer,"%.2Iu",i) != 0) goto err;
    } else {
     if (DeeStringWriter_Writef(&writer,"%.1Iu",i) != 0) goto err;
    }
    if (DeeStringWriter_WRITE_STRING(&writer," |") != 0) goto err;
    if (DeeStringWriter_RepeatChar(&writer,'O',bucket_size) != 0) goto err;
    if (DeeStringWriter_WRITE_STRING(&writer,"|\n") != 0) goto err;
   }
   if (full_max != n_buckets-1) {
    if (DeeStringWriter_RepeatChar(&writer,'.',digmax) != 0) goto err;
    if (DeeStringWriter_WRITE_STRING(&writer," |") != 0) goto err;
    if (DeeStringWriter_RepeatChar(&writer,'O',bucket_size) != 0) goto err;
    if (DeeStringWriter_WRITE_STRING(&writer,"|\n") != 0) goto err;
   }
   if (DeeStringWriter_Writef(&writer,"%Iu |",n_buckets-1) != 0) goto err;
   if (DeeStringWriter_RepeatChar(&writer,'O',bucket_size-back_unused) != 0) goto err;
   if (DeeStringWriter_RepeatChar(&writer,' ',back_unused) != 0) goto err;
   if (DeeStringWriter_WRITE_STRING(&writer,"|\n") != 0) goto err;
  }
 }
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}




//////////////////////////////////////////////////////////////////////////
// VTable: deque.iterator
static void DEE_CALL _deedequeiterator_tp_dtor(DeeDequeIteratorObject *self) { Dee_DECREF(self->di_deq); }
DEE_VISIT_PROC(_deedequeiterator_tp_visit,DeeDequeIteratorObject *self) { Dee_VISIT(self->di_deq); }
static int DEE_CALL _deedequeiterator_tp_seq_iter_next(
 DeeDequeIteratorObject *self, DeeObject **result) {
 DeeAtomicMutex_AcquireRelaxed(&self->di_lock);
 DeeAtomicMutex_AcquireRelaxed(&self->di_deq->d_lock);
 Dee_XINCREF(*result = DeeDequeIterator_YieldSafe(&self->di_iter,&self->di_deq->d_deq));
 DeeAtomicMutex_Release(&self->di_deq->d_lock);
 DeeAtomicMutex_Release(&self->di_lock);
 return DEE_LIKELY(*result != NULL) ? 0 : 1;
}
static DeeObject *DEE_CALL _deedequeiterator_tp_str(DeeDequeIteratorObject *self) {
 DeeObject *elem;
 DeeAtomicMutex_AcquireRelaxed(&self->di_lock);
 Dee_XINCREF(elem = DeeDequeIterator_GET(&self->di_iter));
 DeeAtomicMutex_Release(&self->di_lock);
 if (!elem) DeeReturn_STATIC_STRING("<deque.iterator>");
 return DeeString_Newf("<deque.iterator -> %K>",elem);
}
static DeeObject *DEE_CALL _deedequeiterator_tp_repr(DeeDequeIteratorObject *self) {
 DeeObject *elem;
 DeeAtomicMutex_AcquireRelaxed(&self->di_lock);
 Dee_XINCREF(elem = DeeDequeIterator_GET(&self->di_iter));
 DeeAtomicMutex_Release(&self->di_lock);
 if (!elem) DeeReturn_STATIC_STRING("<deque.iterator>");
 return DeeString_Newf("<deque.iterator -> %R>",elem);
}




static struct DeeMethodDef const _deedeque_tp_methods[] = {
 // TODO: Intrinsic versions of all functions below
 DEE_METHODDEF_CONST_v100("at",member(&_deegenericiterable_at),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("back",member(&_deegenericiterable_back),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("contains",member(&_deegenericiterable_contains),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("count",member(&_deegenericiterable_count),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("empty",member(&_deegenericiterable_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("find",member(&_deegenericiterable_find),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("front",member(&_deegenericiterable_front),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("index",member(&_deegenericiterable_index),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("locate",member(&_deegenericiterable_locate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("locate_all",member(&_deegenericiterable_locate_all),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("non_empty",member(&_deegenericiterable_non_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rfind",member(&_deegenericiterable_rfind),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rindex",member(&_deegenericiterable_rindex),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rlocate",member(&_deegenericiterable_rlocate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("segments",member(&_deegenericiterable_segments),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("sorted",member(&_deegenericiterable_sorted),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("transform",member(&_deegenericiterable_transform),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("clear",member(&_deedeque_clear),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("begin",member(&_deedeque_begin),"() -> deque.iterator"),
 DEE_METHODDEF_v100("end",member(&_deedeque_end),"() -> deque.iterator"),
 DEE_METHODDEF_v100("push_front",member(&_deedeque_push_front),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("push_back",member(&_deedeque_push_back),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("pop_front",member(&_deedeque_pop_front),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("pop_back",member(&_deedeque_pop_back),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("insert",member(&_deedeque_insert),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("insert_list",member(&_deedeque_insert_list),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("insert_iter",member(&_deedeque_insert_iter),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("shrink_to_fit",member(&_deedeque_shrink_to_fit),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("bucketrepr",member(&_deedeque_tp_bucketrepr),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("erase",member(&_deedeque_erase),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("append",member(&_deedeque_push_back),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("remove",member(&_deedeque_remove),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("remove_if",member(&_deedeque_remove_if),DEE_DOC_AUTO),

 //TODO: DEE_METHODDEF_v100("sorted_insert",member(&_deelist_sorted_insert),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("extend",member(&_deelist_extend),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("erase",member(&_deelist_erase),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("pop",member(&_deelist_pop),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("resize",member(&_deelist_resize),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("reserve",member(&_deelist_reserve),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("reverse",member(&_deelist_reverse),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("sort",member(&_deelist_sort),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_CONST_v100("reversed",member(&_deelist_reversed),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_CONST_v100("allocated",member(&_deelist_allocated),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("fill",member(&_deelist_fill),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("unique",member(&_deelist_unique),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_CONST_v100("tounique",member(&_deelist_tounique),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_CONST_v100("extend_unique",member(&_deelist_extend_unique),DEE_DOC_AUTO),
 DEE_METHODDEF_END_v100
};

static struct DeeMemberDef const _deedeque_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeDequeIterator_Type),
 DEE_MEMBERDEF_END_v100
};

DeeTypeObject DeeDeque_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("deque"),null,
  member(DEE_TYPE_FLAG_HAS_GC),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeDequeObject),
  member(&_DeeGC_TpAlloc),
  member(&_deedeque_tp_ctor),
  member(&_deedeque_tp_copy_ctor),
  member(&_deedeque_tp_move_ctor),
  member(&_deedeque_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(
  member(&_DeeGC_TpFree),
  member(&_deedeque_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deedeque_tp_copy_assign),
  member(&_deedeque_tp_move_assign),
  member(&_deedeque_tp_any_assign)),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deedeque_tp_str),
  member(&_deedeque_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deedeque_tp_visit),
  member(&_deedeque_tp_clear)),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deedeque_tp_not),member(&_deedeque_tp_bool),
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deegenericiterable_tp_cmp_lo),
  member(&_deegenericiterable_tp_cmp_le),
  member(&_deegenericiterable_tp_cmp_eq),
  member(&_deegenericiterable_tp_cmp_ne),
  member(&_deegenericiterable_tp_cmp_gr),
  member(&_deegenericiterable_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&_deedeque_tp_seq_get),
  member(&_deedeque_tp_seq_del),
  member(&_deedeque_tp_seq_set),
  member(&_deedeque_tp_seq_size),
  member(&_deedeque_tp_seq_contains),
  member(&_deedeque_tp_seq_range_get),
  member(&_deedeque_tp_seq_range_del),null,
  member(&_deedeque_tp_seq_iter_self),null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,null,member(_deedeque_tp_methods),
  member(_deedeque_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(null),
 DEE_TYPE_OBJECT_FOOTER_v101
};
DeeTypeObject DeeDequeIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("deque.iterator"),null,null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeDequeIteratorObject),null,null,null,null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deedequeiterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deedequeiterator_tp_str),
  member(&_deedequeiterator_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deedequeiterator_tp_visit),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,
  member(&_deedequeiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(null),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DEQUE_C_INL */
