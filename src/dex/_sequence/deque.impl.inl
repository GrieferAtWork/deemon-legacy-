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
#ifdef __INTELLISENSE__
#include "deque.c.inl"
DEE_DECL_BEGIN
#define WITH_LOCK
#endif


#ifdef WITH_LOCK
#define FUNC(name)             name##WithLock
#define AND_FUNC(name)         name##AndLock
#define LOCK_ARG(annotation)   , annotation struct DeeAtomicMutex *lock
#define LOCK_PAR               ,lock
#define TRY_ACQUIRE            DeeAtomicMutex_TryAcquire(lock)
#define ACQUIRE                DeeAtomicMutex_AcquireRelaxed(lock)
#define RELEASE                DeeAtomicMutex_Release(lock)
#else
#define FUNC(name)             name
#define AND_FUNC(name)         name
#define LOCK_ARG(annotation)   /* nothing */
#define LOCK_PAR               /* nothing */
#define TRY_ACQUIRE            1
#define ACQUIRE                (void)0
#define RELEASE                (void)0
#endif


DEE_A_RET_EXCEPT(-1) int FUNC(DeeDeque_InitCopy)(
 DEE_A_OUT struct DeeDeque *self,
 DEE_A_IN struct DeeDeque const *right LOCK_ARG(DEE_A_INOUT)) {
 struct DeeDequeBucket *iter,*end,*src;
 DeeObject **elem_iter,**elem_end,**elem_src;
again:
 DEE_ASSERT(self);
 ACQUIRE;
again_locked:
 DeeDeque_AssertIntegrity(right);
 if (DeeDeque_EMPTY(right)) {
  DeeDeque_InitEx(self,right->d_bucketsize);
 } else {
  DEE_ASSERT(right->d_bucketc != 0);
  self->d_bucketc = right->d_bucketc;
  self->d_bucketsize = right->d_bucketsize;
  if DEE_UNLIKELY((self->d_bucketroot = (struct DeeDequeBucket *)malloc_nz(
   self->d_bucketc*sizeof(struct DeeDequeBucket))) == NULL) {
   RELEASE;
   if (Dee_CollectMemory()) goto again;
   DeeError_NoMemory();
  }
  end = (iter = self->d_bucketroot)+self->d_bucketc;
  DEE_ASSERT(iter != end);
  do {
   if DEE_UNLIKELY((iter->db_elemv = (DeeObject **)malloc_nz(
    self->d_bucketsize*sizeof(DeeObject *))) == NULL) {
    RELEASE;
    if (Dee_CollectMemory()) {
     ACQUIRE;
     if (self->d_bucketc != right->d_bucketc || 
         self->d_bucketsize != right->d_bucketsize) {
      // The other deque changed in a way that forces us to start over
      while (iter != self->d_bucketroot) { --iter; free_nn(iter->db_elemv); }
      free_nn(self->d_bucketroot);
      goto again_locked;
     }
    }
    while (iter != self->d_bucketroot) { --iter; free_nn(iter->db_elemv); }
    free_nn(self->d_bucketroot);
    DeeError_NoMemory();
    return -1;
   }
  } while (++iter != end);
  DEE_ASSERT(self->d_bucketc == right->d_bucketc);
  DEE_ASSERT(self->d_bucketsize == right->d_bucketsize);
  // Buckets & elemv-buffers are now allocated.
  // >> From here on, everything is noexcept
  self->d_elemc = right->d_elemc;
  self->d_bucketv = self->d_bucketroot;
  self->d_bucketelemv = self->d_bucketroot;
  self->d_bucketa = right->d_bucketc;
  self->d_bucketelema = right->d_bucketc;
  self->d_begin = self->d_bucketv[0].db_elemv+(right->d_begin-right->d_bucketv[0].db_elemv);
  self->d_end = self->d_bucketv[self->d_bucketc-1].db_elemv+(
   right->d_end-right->d_bucketv[right->d_bucketc-1].db_elemv);
  // Last thing to do: Actually copy all the stored elements (by-reference)
  if (self->d_bucketc == 1) {
   // Special case: Only one bucket (begin & end share the same vector)
   elem_src = right->d_begin;
   elem_iter = self->d_begin;
   elem_end = self->d_end;
   do Dee_INCREF(*elem_iter++ = *elem_src++);
   while (elem_iter != elem_end);
  } else {
   // Special case: First bucket
   elem_src = right->d_begin;
   elem_iter = self->d_begin;
   elem_end = self->d_bucketv[0].db_elemv+self->d_bucketsize;
   do Dee_INCREF(*elem_iter++ = *elem_src++);
   while (elem_iter != elem_end);
   // Copy all full buckets
   end = (iter = self->d_bucketv+1)+(self->d_bucketc-2);
   src = right->d_bucketv+1;
   do {
    elem_end = (elem_iter = iter->db_elemv)+self->d_bucketsize;
    elem_src = src->db_elemv;
    do Dee_INCREF(*elem_iter++ = *elem_src++);
    while (elem_iter != elem_end);
    ++iter,++src;
   } while (iter != end);
   // Sepcial case: Last bucket
   DEE_ASSERT(end == self->d_bucketv+(self->d_bucketc-1));
   DEE_ASSERT(src == right->d_bucketv+(right->d_bucketc-1));
   elem_iter = end->db_elemv;
   elem_end = self->d_end;
   elem_src = src->db_elemv;
   do Dee_INCREF(*elem_iter++ = *elem_src++);
   while (elem_iter != elem_end);
  }
 }
 RELEASE;
 DeeDeque_AssertIntegrity(self);
 return 0;
}
void FUNC(DeeDeque_InitMove)(
 DEE_A_OUT struct DeeDeque *self,
 DEE_A_INOUT struct DeeDeque *right LOCK_ARG(DEE_A_INOUT)) {
 ACQUIRE;
 DeeDeque_AssertIntegrity(right);
 self->d_elemc       = right->d_elemc;
 self->d_bucketsize  = right->d_bucketsize;
 self->d_bucketelema = right->d_bucketelema;
 self->d_bucketelemv = right->d_bucketelemv;
 self->d_bucketa     = right->d_bucketa;
 self->d_bucketroot  = right->d_bucketroot;
 self->d_bucketc     = right->d_bucketc;
 self->d_bucketv     = right->d_bucketv;
 self->d_begin       = right->d_begin;
 self->d_end         = right->d_end;
 right->d_elemc = 0;
 right->d_bucketelema = 0;
 right->d_bucketelemv = NULL;
 right->d_bucketa = 0;
 right->d_bucketroot = NULL;
 right->d_bucketc = 0;
 right->d_bucketv = NULL;
 right->d_begin = NULL;
 right->d_end = NULL;
 DeeDeque_AssertIntegrity(right);
 RELEASE;
 DeeDeque_AssertIntegrity(self);
}



void FUNC(DeeDeque_Clear)(DEE_A_INOUT struct DeeDeque *self LOCK_ARG(DEE_A_INOUT)) {
 struct DeeDequeBucket *iter,*end,*old_bucketelemv,*old_bucketroot,*old_bucketv;
 DeeObject **old_begin,**old_end,**elem_iter,**elem_end;
 Dee_size_t old_bucketelema,old_bucketc,old_bucketsize,old_bucketa;
 ACQUIRE;
 DeeDeque_AssertIntegrity(self);
 if (!self->d_elemc) { RELEASE; return; }
 self->d_elemc = 0;
 old_bucketsize = self->d_bucketsize;
 old_bucketelema = self->d_bucketelema;
 old_bucketelemv = self->d_bucketelemv;
 old_bucketroot = self->d_bucketroot;
 old_bucketc = self->d_bucketc; self->d_bucketc = 0;
 old_bucketv = self->d_bucketv; self->d_bucketv = NULL;
 old_begin = self->d_begin; self->d_begin = NULL;
 old_end = self->d_end; self->d_end = NULL;
 self->d_bucketelema = 0;
 self->d_bucketelemv = NULL;
 old_bucketa = self->d_bucketa; self->d_bucketa = 0;
 self->d_bucketroot = NULL;
 RELEASE;
 // Clear the first bucket
 elem_iter = old_begin;
 DEE_ASSERT(elem_iter >= old_bucketv->db_elemv &&
            elem_iter < old_bucketv->db_elemv+old_bucketsize);
 if (old_bucketc == 1) {
  // Special case: Only need to clear a single bucket partially
  elem_end = old_end;
  DEE_ASSERT(elem_end >= old_bucketv->db_elemv &&
             elem_end < old_bucketv->db_elemv+old_bucketsize);
  DEE_ASSERT(elem_iter != elem_end);
  do Dee_DECREF(*elem_iter);
  while (++elem_iter != elem_end);
 } else {
  elem_end = old_bucketv->db_elemv+old_bucketsize;
  DEE_ASSERT(elem_iter != elem_end);
  do Dee_DECREF(*elem_iter);
  while (++elem_iter != elem_end);
  // Clear all intermediate buckets
  DEE_ASSERT(old_bucketc >= 2);
  end = (iter = old_bucketv+1)+(old_bucketc-2);
  while (iter != end) {
   DEE_ASSERT(iter->db_elemv);
   DEE_ASSERT(old_bucketsize);
   elem_end = (elem_iter = iter->db_elemv)+old_bucketsize;
   do Dee_DECREF(*elem_iter);
   while (++elem_iter != elem_end);
   ++iter;
  }
  // Clear the end bucket
  DEE_ASSERT(end == old_bucketv+old_bucketc-1);
  elem_end = old_end;
  DEE_ASSERT(elem_end >= end->db_elemv &&
             elem_end < end->db_elemv+old_bucketsize);
  elem_iter = end->db_elemv;
  DEE_ASSERT(elem_iter != elem_end);
  do Dee_DECREF(*elem_iter);
  while (++elem_iter != elem_end);
 }

 if (TRY_ACQUIRE) {
  // Try to return the pre-allocated buffers
  // NOTE: can't restore the buffer if a new one was allocated,
  //       or if the bucketsize got changed in the mean-time.
  if (!self->d_bucketroot && self->d_bucketsize == old_bucketsize) {
   DEE_ASSERT(!self->d_bucketelema);
   DEE_ASSERT(!self->d_bucketelemv);
   DEE_ASSERT(!self->d_bucketa);
   DEE_ASSERT(!self->d_bucketroot);
   self->d_bucketelema = old_bucketelema;
   self->d_bucketelemv = old_bucketelemv;
   self->d_bucketa = old_bucketa;
   self->d_bucketroot = old_bucketroot;
   DEE_ASSERT(!self->d_elemc);
   DEE_ASSERT(!self->d_bucketv);
   DEE_ASSERT(!self->d_bucketc);
   DEE_ASSERT(!self->d_begin);
   DEE_ASSERT(!self->d_end);
   RELEASE;
   return;
  }
  RELEASE;
 }
 // Free all allocated element vectors
 end = (iter = old_bucketelemv)+old_bucketelema;
 while (iter != end) { free_nn(iter->db_elemv); ++iter; }

 // Free the root bucket list
 free(old_bucketroot);
}


DEE_A_RET_EXCEPT_FAIL(-1,0) int FUNC(DeeDeque_Contains)(
 DEE_A_INOUT struct DeeDeque const *self,
 DEE_A_INOUT DeeObject *elem LOCK_ARG(DEE_A_INOUT)) {
 DeeObject *deq_elem; int error;
 DeeDeque_TRAVERSE_SAFE_VARS;
 ACQUIRE;
 DeeDeque_AssertIntegrity(self);
 DeeDeque_TRAVERSE_SAFE(deq_elem,self) {
  Dee_INCREF(deq_elem);
  RELEASE;
  error = DeeObject_CompareEq(deq_elem,elem);
  Dee_DECREF(deq_elem);
  if (error != 0) return error; // Yes, or error
  ACQUIRE;
 }
 RELEASE;
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int FUNC(DeeDeque_ContainsPred)(
 DEE_A_INOUT struct DeeDeque const *self, DEE_A_INOUT DeeObject *elem,
 DEE_A_INOUT DeeObject *pred LOCK_ARG(DEE_A_INOUT)) {
 DeeObject *deq_elem,*pred_args,*pred_result; int error;
 DeeDeque_TRAVERSE_SAFE_VARS;
 ACQUIRE;
 DeeDeque_AssertIntegrity(self);
 DeeDeque_TRAVERSE_SAFE(deq_elem,self) {
  Dee_INCREF(deq_elem);
  RELEASE;
  pred_args = DeeTuple_Pack(2,deq_elem,elem);
  Dee_DECREF(deq_elem);
  if DEE_UNLIKELY(!pred_args) return -1;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) return -1;
  error = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if (error != 0) return error; // Yes, or error
  ACQUIRE;
 }
 RELEASE;
 return 0;
}


DEE_A_RET_EXCEPT(-1) int FUNC(DeeDeque_PushFront)(
 DEE_A_INOUT struct DeeDeque *self,
 DEE_A_INOUT DeeObject *elem LOCK_ARG(DEE_A_INOUT)) {
 DEE_ASSERT(DeeObject_Check(elem));
again:
 ACQUIRE;
 DeeDeque_AssertIntegrity(self);
 if DEE_UNLIKELY(DeeDeque_EMPTY(self)) {
  // Empty deque --> Special code for allocating the first element
  DeeDeque_ALLOC_FIRST(self,elem,goto err_nomem);
  RELEASE;
  return 0;
 }
 if DEE_UNLIKELY(DeeDeque_FRONT_BUCKET_FULL_NZ(self)) {
  if DEE_UNLIKELY(DeeDeque_EMPBUCKETCACHE_LEFT(self)) {
   // Allocate a new bucket
   DeeDeque_INCBUCKETCACHE_LEFT(self,goto err_nomem);
  }
  if DEE_UNLIKELY(DeeDeque_EMPELEMVCACHE_LEFT(self)) {
   // Allocate a new elemv-cache entry
   DeeDeque_INCELEMVCACHE_LEFT(self,goto err_nomem);
  }
  // Begin using a new bucket on the left
  self->d_begin = (--self->d_bucketv)->db_elemv+self->d_bucketsize;
  ++self->d_bucketc;
 }
 Dee_INCREF(*--self->d_begin = elem);
 ++self->d_elemc;
 RELEASE;
 return 0;
err_nomem:
 RELEASE;
 if DEE_LIKELY(Dee_CollectMemory()) goto again;
 DeeError_NoMemory();
 return -1;
}
DEE_A_RET_EXCEPT(-1) int FUNC(DeeDeque_PushBack)(
 DEE_A_INOUT struct DeeDeque *self,
 DEE_A_INOUT DeeObject *elem LOCK_ARG(DEE_A_INOUT)) {
 DEE_ASSERT(DeeObject_Check(elem));
again:
 ACQUIRE;
 DeeDeque_AssertIntegrity(self);
 if DEE_UNLIKELY(DeeDeque_EMPTY(self)) {
  // Empty deque --> Special code for allocating the first element
  DeeDeque_ALLOC_FIRST(self,elem,goto err_nomem);
  RELEASE;
  return 0;
 }
 if DEE_UNLIKELY(DeeDeque_BACK_BUCKET_FULL_NZ(self)) {
  if DEE_UNLIKELY(DeeDeque_EMPBUCKETCACHE_RIGHT(self)) {
   DeeDeque_INCBUCKETCACHE_RIGHT(self,goto err_nomem); // Allocate a new bucket
  }
  if DEE_UNLIKELY(DeeDeque_EMPELEMVCACHE_RIGHT(self)) {
   DeeDeque_INCELEMVCACHE_RIGHT(self,goto err_nomem); // Allocate a new elemv-cache entry
  }
  // Begin using a new bucket on the left
  self->d_end = self->d_bucketv[self->d_bucketc++].db_elemv;
 }
 Dee_INCREF(*self->d_end++ = elem);
 ++self->d_elemc;
 RELEASE;
 return 0;
err_nomem:
 RELEASE;
 if DEE_LIKELY(Dee_CollectMemory()) goto again;
 DeeError_NoMemory();
 return -1;
}

DEE_A_RET_EXCEPT(-1) int FUNC(DeeDeque_PushFrontVector)(
 DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t elemc,
 DEE_A_IN_R(n) DeeObject *const *elemv LOCK_ARG(DEE_A_INOUT)) {
 DeeObject **iter,**end;
 Dee_size_t req_freebucketc,free_slots,uncovered_elemc;
 struct _DeeDequeFastIterator fill_iter;
 DEE_ASSERT(!elemc || elemv);
 if (!elemc) return 0;
again:
 ACQUIRE;
 DeeDeque_AssertIntegrity(self);
 if (DeeDeque_EMPTY(self)) {
  DeeDeque_ALLOC_FIRST_N(self,elemc,(DeeObject **)elemv,goto err_nomem);
  RELEASE;
  return 0;
 }
 // Insert on the left
 free_slots = DeeDeque_FRONT_UNUSED_NZ(self);
 if (elemc > free_slots) {
  // Must allocate more buckets in the right
  uncovered_elemc = elemc-free_slots;
  req_freebucketc = uncovered_elemc/self->d_bucketsize;
  if ((uncovered_elemc%self->d_bucketsize)!=0) ++req_freebucketc;
  DeeDeque_INCBUCKETCACHE_LEFT_N(self,req_freebucketc,goto err_nomem);
  DeeDeque_INCELEMVCACHE_LEFT_N(self,req_freebucketc,goto err_nomem);
 }
 DeeDeque_CONSUME_LEFT_N(self,elemc);
 DeeDeque_AssertIntegrity(self);

 // Fill the now free memory
 _DeeDequeFastIterator_InitFront(&fill_iter,self);
 end = (iter = (DeeObject **)elemv)+elemc;
 while (iter != end) {
  Dee_INCREF(*_DeeDequeFastIterator_ELEM(&fill_iter) = *iter++);
  _DeeDequeFastIterator_Next(&fill_iter,self);
 }
 DeeDeque_AssertIntegrity(self);
 RELEASE;
 return 0;
err_nomem:
 DeeDeque_AssertIntegrity(self);
 RELEASE;
 if DEE_LIKELY(Dee_CollectMemory()) goto again;
 DeeError_NoMemory();
 return -1;
}
DEE_A_RET_EXCEPT(-1) int FUNC(DeeDeque_PushBackVector)(
 DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t elemc,
 DEE_A_IN_R(n) DeeObject *const *elemv LOCK_ARG(DEE_A_INOUT)) {
 DeeObject **iter,**end; Dee_size_t start_index;
 Dee_size_t req_freebucketc,free_slots,uncovered_elemc;
 struct _DeeDequeFastIterator fill_iter;
 DEE_ASSERT(!elemc || elemv);
 if (!elemc) return 0;
again:
 ACQUIRE;
 DeeDeque_AssertIntegrity(self);
 if (DeeDeque_EMPTY(self)) {
  DeeDeque_ALLOC_FIRST_N(self,elemc,(DeeObject **)elemv,goto err_nomem);
  RELEASE;
  return 0;
 }
 start_index = self->d_elemc;
 // Insert on the right
 free_slots = DeeDeque_BACK_UNUSED_NZ(self);
 if (elemc > free_slots) {
  // Must allocate more buckets in the right
  uncovered_elemc = elemc-free_slots;
  req_freebucketc = uncovered_elemc/self->d_bucketsize;
  if ((uncovered_elemc%self->d_bucketsize)!=0) ++req_freebucketc;
  DeeDeque_INCBUCKETCACHE_RIGHT_N(self,req_freebucketc,goto err_nomem);
  DeeDeque_INCELEMVCACHE_RIGHT_N(self,req_freebucketc,goto err_nomem);
 }
 DeeDeque_CONSUME_RIGHT_N(self,elemc);
 DeeDeque_AssertIntegrity(self);

 // Fill the now free memory
 _DeeDequeFastIterator_InitForward(&fill_iter,self,start_index);
 end = (iter = (DeeObject **)elemv)+elemc;
 while (iter != end) {
  Dee_INCREF(*_DeeDequeFastIterator_ELEM(&fill_iter) = *iter++);
  _DeeDequeFastIterator_Next(&fill_iter,self);
 }

 DeeDeque_AssertIntegrity(self);
 RELEASE;
 return 0;
err_nomem:
 DeeDeque_AssertIntegrity(self);
 RELEASE;
 if DEE_LIKELY(Dee_CollectMemory()) goto again;
 DeeError_NoMemory();
 return -1;
}



DEE_A_RET_EXCEPT_REF DeeObject *FUNC(DeeDeque_PopFront)(
 DEE_A_INOUT struct DeeDeque *self LOCK_ARG(DEE_A_INOUT)) {
 DeeObject *result;
 ACQUIRE;
 DeeDeque_AssertIntegrity(self);
 if DEE_UNLIKELY(DeeDeque_EMPTY(self)) {
  RELEASE;
  _sequence_emptyerror();
  return NULL; // Error: Empty deque
 }
 // Transfer reference
 --self->d_elemc;
#ifdef DEE_DEBUG
 result = *self->d_begin;
 *self->d_begin = NULL;
 ++self->d_begin;
#else
 result = *self->d_begin++;
#endif
 if DEE_UNLIKELY(self->d_begin == DeeDeque_FRONT_BUCKET(self)->db_elemv+self->d_bucketsize) {
  // Front bucket now empty --> mark it as such, and update the begin pointer
  ++self->d_bucketv;
  if DEE_LIKELY(--self->d_bucketc != 0) {
   self->d_begin = DeeDeque_FRONT_BUCKET(self)->db_elemv;
  } else {
   // Deque is now empty
now_empty:
   DEE_ASSERT(!self->d_elemc);
   self->d_begin = NULL;
   self->d_end = NULL;
   self->d_bucketv = NULL;
  }
 } else if DEE_UNLIKELY(self->d_begin == self->d_end) {
  DEE_ASSERT(self->d_bucketc == 1);
  self->d_bucketc = 0;
  goto now_empty;
 }
 RELEASE;
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *FUNC(DeeDeque_PopBack)(
 DEE_A_INOUT struct DeeDeque *self LOCK_ARG(DEE_A_INOUT)) {
 DeeObject *result;
 ACQUIRE;
 DeeDeque_AssertIntegrity(self);
 if DEE_UNLIKELY(DeeDeque_EMPTY(self)) {
  RELEASE;
  _sequence_emptyerror();
  return NULL; // Error: Empty deque
 }
 // Transfer reference
 --self->d_elemc;
 result = *--self->d_end;
#ifdef DEE_DEBUG
 *self->d_end = NULL;
#endif
 if DEE_UNLIKELY(self->d_end == DeeDeque_BACK_BUCKET(self)->db_elemv) {
  // Back bucket now empty --> mark it as such
  if DEE_LIKELY(--self->d_bucketc != 0) {
   self->d_end = DeeDeque_BACK_BUCKET(self)->db_elemv+self->d_bucketsize;
  } else {
   // Deque is now empty
now_empty:
   DEE_ASSERT(!self->d_elemc);
   self->d_begin = NULL;
   self->d_end = NULL;
   self->d_bucketv = NULL;
  }
 } else if DEE_UNLIKELY(self->d_begin == self->d_end) {
  DEE_ASSERT(self->d_bucketc == 1);
  self->d_bucketc = 0;
  goto now_empty;
 }
 RELEASE;
 return result;
}


DEE_A_RET_EXCEPT(-1) int FUNC(DeeDeque_Insert)(
 DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t i,
 DEE_A_INOUT DeeObject *elem LOCK_ARG(DEE_A_INOUT)) {
 Dee_size_t used_i; DeeObject **elem_slot;
 DEE_ASSERT(DeeObject_Check(elem));
again:
 used_i = i;
 ACQUIRE;
 DeeDeque_AssertIntegrity(self);
 if (DeeDeque_EMPTY(self)) {
  DeeDeque_ALLOC_FIRST(self,elem,goto err_nomem);
  RELEASE;
  return 0;
 }
 if (used_i > DeeDeque_SIZE(self)) used_i = DeeDeque_SIZE(self);
 if (used_i >= DeeDeque_SIZE(self)/2) {
  // Closer to the right --> Insert on the right
  if DEE_UNLIKELY(DeeDeque_EMPBUCKETCACHE_RIGHT(self)) {
   DeeDeque_INCBUCKETCACHE_RIGHT(self,goto err_nomem); // Allocate a new bucket
  }
  if DEE_UNLIKELY(DeeDeque_EMPELEMVCACHE_RIGHT(self)) {
   DeeDeque_INCELEMVCACHE_RIGHT(self,goto err_nomem); // Allocate a new elemv-cache entry
  }
  _DeeDeque_ShiftRight(self,used_i,1);
 } else {
  // Closer to the left --> Insert on the left
  if DEE_UNLIKELY(DeeDeque_EMPBUCKETCACHE_LEFT(self)) {
   DeeDeque_INCBUCKETCACHE_LEFT(self,goto err_nomem); // Allocate a new bucket
  }
  if DEE_UNLIKELY(DeeDeque_EMPELEMVCACHE_LEFT(self)) {
   DeeDeque_INCELEMVCACHE_LEFT(self,goto err_nomem); // Allocate a new elemv-cache entry
  }
  _DeeDeque_ShiftLeft(self,used_i,1);
 }
 DeeDeque_AssertIntegrity(self);
 elem_slot = DeeDeque_ELEM_NZ(self,used_i);
 Dee_INCREF(*elem_slot = elem);
 RELEASE;
 return 0;
err_nomem:
 DeeDeque_AssertIntegrity(self);
 RELEASE;
 if DEE_LIKELY(Dee_CollectMemory()) goto again;
 DeeError_NoMemory();
 return -1;
}

DEE_A_RET_EXCEPT(-1) int FUNC(DeeDeque_InsertVector)(
 DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t i,
 DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(n) DeeObject *const *elemv LOCK_ARG(DEE_A_INOUT)) {
 Dee_size_t used_i; DeeObject **iter,**end;
 Dee_size_t req_freebucketc,free_slots,uncovered_elemc;
 struct _DeeDequeFastIterator fill_iter;
 DEE_ASSERT(!elemc || elemv);
 if (!elemc) return 0;
again:
 used_i = i;
 ACQUIRE;
 DeeDeque_AssertIntegrity(self);
 if (DeeDeque_EMPTY(self)) {
#if 1
  DeeDeque_ALLOC_FIRST_N(self,elemc,(DeeObject **)elemv,goto err_nomem);
  RELEASE;
  return 0;
#else
  // NOTE: This code was just a placeholder and lacks in integrity when if comes to multi-threading
  RELEASE;
  if (FUNC(DeeDeque_PushFront)(self,*elemv LOCK_PAR) != 0) return -1;
  return FUNC(DeeDeque_InsertVector)(self,1,elemc-1,elemv+1 LOCK_PAR);
#endif
 }
 if (used_i > DeeDeque_SIZE(self)) used_i = DeeDeque_SIZE(self);
 if (used_i >= DeeDeque_SIZE(self)/2) {
  // Closer to the right --> Insert on the right
  free_slots = DeeDeque_BACK_UNUSED_NZ(self);
  if (elemc > free_slots) {
   // Must allocate more buckets in the right
   uncovered_elemc = elemc-free_slots;
   req_freebucketc = uncovered_elemc/self->d_bucketsize;
   if ((uncovered_elemc%self->d_bucketsize)!=0) ++req_freebucketc;
   DeeDeque_INCBUCKETCACHE_RIGHT_N(self,req_freebucketc,goto err_nomem);
   DeeDeque_INCELEMVCACHE_RIGHT_N(self,req_freebucketc,goto err_nomem);
  }
  _DeeDeque_ShiftRight(self,used_i,elemc);
 } else {
  // Closer to the left --> Insert on the left
  free_slots = DeeDeque_FRONT_UNUSED_NZ(self);
  if (elemc > free_slots) {
   // Must allocate more buckets in the right
   uncovered_elemc = elemc-free_slots;
   req_freebucketc = uncovered_elemc/self->d_bucketsize;
   if ((uncovered_elemc%self->d_bucketsize)!=0) ++req_freebucketc;
   DeeDeque_INCBUCKETCACHE_LEFT_N(self,req_freebucketc,goto err_nomem);
   DeeDeque_INCELEMVCACHE_LEFT_N(self,req_freebucketc,goto err_nomem);
  }
  _DeeDeque_ShiftLeft(self,used_i,elemc);
 }
 DeeDeque_AssertIntegrity(self);

 // Fill the now free memory
 _DeeDequeFastIterator_InitForward(&fill_iter,self,used_i);
 end = (iter = (DeeObject **)elemv)+elemc;
 while (iter != end) {
  Dee_INCREF(*_DeeDequeFastIterator_ELEM(&fill_iter) = *iter++);
  _DeeDequeFastIterator_Next(&fill_iter,self);
 }

 DeeDeque_AssertIntegrity(self);
 RELEASE;
 return 0;
err_nomem:
 DeeDeque_AssertIntegrity(self);
 RELEASE;
 if DEE_LIKELY(Dee_CollectMemory()) goto again;
 DeeError_NoMemory();
 return -1;
}


DEE_A_RET_EXCEPT(-1) int FUNC(DeeDeque_TInsertSequence)(
 DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t i,
 DEE_A_IN DeeTypeObject const *tp_sequence,
 DEE_A_INOUT DeeObject *sequence LOCK_ARG(DEE_A_INOUT)) {
 DeeObject *temp; int error;
 if (tp_sequence == &DeeTuple_Type) return FUNC(DeeDeque_InsertVector)(
  self,i,DeeTuple_SIZE(sequence),DeeTuple_ELEM(sequence) LOCK_PAR);
 if (tp_sequence == &DeeSuper_Type) return FUNC(DeeDeque_TInsertSequence)(
  self,i,DeeSuper_TYPE(sequence),DeeSuper_SELF(sequence) LOCK_PAR);
 if (tp_sequence == &DeeList_Type) {
  if (DeeObject_IS_UNIQUE(sequence)) {
   return FUNC(DeeDeque_InsertVector)(self,i,DeeList_SIZE(sequence),
                                      DeeList_ELEM(sequence) LOCK_PAR);
  }
  if DEE_UNLIKELY((temp = DeeObject_Copy(sequence)) == NULL) return -1;
  error = FUNC(DeeDeque_InsertVector)(self,i,DeeList_SIZE(temp),
                                      DeeList_ELEM(temp) LOCK_PAR);
  Dee_DECREF(temp);
  return error;
 }
 if DEE_UNLIKELY((temp = DeeObject_TIterSelf(tp_sequence,sequence)) == NULL) return -1;
 error = FUNC(DeeDeque_InsertIterator)(self,i,temp LOCK_PAR);
 Dee_DECREF(temp);
 return error;
}
DEE_A_RET_EXCEPT(-1) int FUNC(DeeDeque_InsertIterator)(
 DEE_A_INOUT struct DeeDeque *self, DEE_A_IN Dee_size_t i,
 DEE_A_INOUT DeeObject *iterator LOCK_ARG(DEE_A_INOUT)) {
 DeeObject *temp; int error; // todo: Optimize?
 if DEE_UNLIKELY((temp = DeeList_NewFromIterator(iterator)) == NULL) return -1;
 error = FUNC(DeeDeque_InsertVector)(self,i,DeeList_SIZE(temp),DeeList_ELEM(temp) LOCK_PAR);
 Dee_DECREF(temp);
 return error;
}



void FUNC(DeeDeque_ShrinkToFit)(
 DEE_A_INOUT struct DeeDeque *self,
 DEE_A_IN Dee_uint32_t flags LOCK_ARG(DEE_A_INOUT)) {
 ACQUIRE;
 DeeDeque_AssertIntegrity(self);
 if (DeeDeque_EMPTY(self)) { RELEASE; return; }
 if ((flags&DEE_DEQUE_SHRINKTOFIT_FLAG_ELEMV)!=0) {
  Dee_size_t used_size,avail_size,overflow,old_start_offset,new_start_offset,shift;
  // Step #1: Shift elements to reduce the needed bucket count by up to one
  used_size = DeeDeque_SIZE(self);
  avail_size = self->d_bucketc*self->d_bucketsize;
  DEE_ASSERT(avail_size >= used_size);
  overflow = avail_size-used_size;
  DEE_ASSERT(overflow < self->d_bucketsize*2);
  if (overflow >= self->d_bucketsize) {
   struct _DeeDequeFastIterator src_iter,dst_iter;
   // Yes: We can shift the elements to free up one more bucket
   // NOTE: The new start of the deque will be at 
   //       >> self->d_bucketelemv[0].db_elemv+new_start_offset;
   new_start_offset = (overflow-self->d_bucketsize)/2;
   old_start_offset = (Dee_size_t)(self->d_begin-self->d_bucketv[0].db_elemv);
   DEE_ASSERT(old_start_offset > new_start_offset);
   shift = old_start_offset-new_start_offset;
   _DeeDequeFastIterator_InitFront(&src_iter,self);
   self->d_begin -= shift;
   // Underflow the previous end into the bucket preceding it
   // >> Now the last bucket is no longer in use
   DEE_ASSERTF(self->d_end-shift <= self->d_bucketv[self->d_bucketc-1].db_elemv,
               "End pointer to last bucket can't shift into previous bucket");
   self->d_end = self->d_bucketv[self->d_bucketc-2].db_elemv+(self->d_bucketsize-(
    self->d_bucketv[self->d_bucketc-1].db_elemv-(self->d_end-shift)));
   --self->d_bucketc;
   _DeeDequeFastIterator_InitFront(&dst_iter,self);
   // Shift the elements from 'src_iter' to 'dst_iter'
   while (used_size--) {
    *_DeeDequeFastIterator_ELEM(&dst_iter) = *_DeeDequeFastIterator_ELEM(&src_iter);
#ifdef DEE_DEBUG
    *_DeeDequeFastIterator_ELEM(&src_iter) = NULL;
#endif
    _DeeDequeFastIterator_Next(&dst_iter,self);
    _DeeDequeFastIterator_Next(&src_iter,self);
   }
  }
  DeeDeque_AssertIntegrity(self);
 }
 if ((flags&DEE_DEQUE_SHRINKTOFIT_FLAG_ELEMV)!=0) {
  // Free unused elemv-entries
  DEE_ASSERT(self->d_bucketelemv <= self->d_bucketv);
  while (self->d_bucketelemv != self->d_bucketv) {
   // Free one elemv-entry at the front
   free_nn(self->d_bucketelemv->db_elemv);
   --self->d_bucketelema;
   ++self->d_bucketelemv;
  }
  DEE_ASSERT(self->d_bucketelemv == self->d_bucketv);
  DEE_ASSERT(self->d_bucketelema >= self->d_bucketc);
  while (self->d_bucketelema != self->d_bucketc) {
   // Free one elemv-entry at the back
   --self->d_bucketelema;
   free_nn(self->d_bucketelemv[self->d_bucketelema].db_elemv);
  }
  DEE_ASSERT(self->d_bucketelema == self->d_bucketc);
  DeeDeque_AssertIntegrity(self);
 }
 if ((flags&DEE_DEQUE_SHRINKTOFIT_FLAG_BUCKETS)!=0 &&
     self->d_bucketelema != self->d_bucketa) {
  struct DeeDequeBucket *new_root;
  // Relocate the bucket vector to only take as much memory as we need
  DEE_ASSERT(self->d_bucketelema <= self->d_bucketa);
  DEE_ASSERT(self->d_bucketelemv >= self->d_bucketroot);
  if (self->d_bucketroot != self->d_bucketelemv) {
   Dee_size_t shift_offset;
   // Shift the elemv-start to match the root pointer
   memmove(self->d_bucketroot,self->d_bucketelemv,
           self->d_bucketelema*sizeof(struct DeeDequeBucket));
   shift_offset = (Dee_size_t)(self->d_bucketelemv-self->d_bucketroot);
   self->d_bucketelemv = self->d_bucketroot;
   self->d_bucketv -= shift_offset;
  }
  DEE_ASSERT(self->d_bucketelemv == self->d_bucketroot);
  DEE_ASSERT(self->d_bucketv >= self->d_bucketroot);
  // With the elemv-cache matching the root pointer, we can easily reallocate
  // the bucket root to have 'self->d_bucketa' match 'self->d_bucketelema'
  if ((new_root = (struct DeeDequeBucket *)realloc_nnz(
   self->d_bucketroot,self->d_bucketelema*
   sizeof(struct DeeDequeBucket))) != NULL) {
   self->d_bucketv = new_root+(self->d_bucketv-self->d_bucketroot);
   self->d_bucketa = self->d_bucketelema;
   self->d_bucketelemv = new_root;
   self->d_bucketroot = new_root;
  } // else { /* Ignore out-of-memory here */ }
  DeeDeque_AssertIntegrity(self);
 }
 RELEASE;
}




#undef FUNC
#undef AND_FUNC
#undef LOCK_ARG
#undef LOCK_PAR
#undef TRY_ACQUIRE
#undef ACQUIRE
#undef RELEASE
#ifdef WITH_LOCK
#undef WITH_LOCK
#endif

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
