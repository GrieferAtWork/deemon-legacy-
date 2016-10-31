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
  DEE_ASSERT(elem_end >= end[1].db_elemv &&
             elem_end < end[1].db_elemv+old_bucketsize);
  elem_iter = end[1].db_elemv;
  DEE_ASSERT(elem_iter != elem_end);
  do Dee_DECREF(*elem_iter);
  while (++elem_iter != elem_end);
 }

 if (TRY_ACQUIRE) {
  // Try to return the pre-allocated buffers
  // NOTE: can't restore the buffer if a new one was allocated,
  //       or if the bucketsize got changed in the mean-time.
  if (!self->d_bucketc && self->d_bucketsize == old_bucketsize) {
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
