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
#include "set.c"
DEE_DECL_BEGIN
#define WITH_LOCK
#endif

#ifdef WITH_LOCK
#define FUNC(name)             name##WithLock
#define AND_FUNC(name)         name##AndLock
#define LOCK_ARG(annotation)   , annotation struct DeeAtomicMutex *lock
#define LOCK_PAR               ,lock
#define ACQUIRE                DeeAtomicMutex_Acquire(lock)
#define RELEASE                DeeAtomicMutex_Release(lock)
#else
#define FUNC(name)             name
#define AND_FUNC(name)         name
#define LOCK_ARG(annotation)   /* nothing */
#define LOCK_PAR               /* nothing */
#define ACQUIRE                (void)0
#define RELEASE                (void)0
#endif


DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashSet_InitCopy)(
 DEE_A_OUT struct DeeHashSet *self,
 DEE_A_IN struct DeeHashSet const *right LOCK_ARG(DEE_A_INOUT)) {
 DeeHashSet_TRAVERSE_SAFE_VARS; int temp;
 struct _DeeHashSetBucketEntry *entry;
 Dee_hash_t bucket; DeeObject *key;
 DEE_ASSERT(self && right && self != right);
 DEE_ASSERT(self && right);
 DeeHashSet_Init(self);
 ACQUIRE;
 DeeHashSet_TRAVERSE_SAFE(entry,right) {
  bucket = entry->sbe_hash;
  Dee_INCREF(key = entry->sbe_key);
  RELEASE;
  temp = DeeHashSet_InsertWithKnownBucket(self,key,bucket);
  Dee_DECREF(key);
  if (temp == -1) { DeeHashSet_Quit(self); return -1; }
  ACQUIRE;
 }
 RELEASE;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashSet_InitDeepCopy)(
 DEE_A_OUT struct DeeHashSet *self,
 DEE_A_IN struct DeeHashSet const *right LOCK_ARG(DEE_A_INOUT)) {
 DeeHashSet_TRAVERSE_SAFE_VARS; int temp;
 struct _DeeHashSetBucketEntry *entry;
 Dee_hash_t bucket; DeeObject *key,*key_copy;
 DEE_ASSERT(self && right && self != right);
 DEE_ASSERT(self && right);
 DeeHashSet_Init(self);
 ACQUIRE;
 DeeHashSet_TRAVERSE_SAFE(entry,right) {
  bucket = entry->sbe_hash;
  Dee_INCREF(key = entry->sbe_key);
  RELEASE;
  key_copy = DeeObject_DeepCopy(key);
  Dee_DECREF(key);
  if (!key_copy) goto err;
  temp = DeeHashSet_InsertWithKnownBucket(self,key_copy,bucket);
  Dee_DECREF(key_copy);
  if (temp == -1) {err: DeeHashSet_Quit(self); return -1; }
  ACQUIRE;
 }
 RELEASE;
 return 0;
}

DEE_COMPILER_PREFAST_WARNING_PUSH(6101)
DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashSet_Hash)(
 DEE_A_IN struct DeeHashSet const *self,
 DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result LOCK_ARG(DEE_A_INOUT)) {
 DeeHashSet_TRAVERSE_SAFE_VARS; DeeObject *entry_key;
 struct _DeeHashSetBucketEntry *entry; int error;
 ACQUIRE;
 DeeHashSet_TRAVERSE_SAFE(entry,self) {
  Dee_INCREF(entry_key = entry->sbe_key);
  RELEASE;
  error = DeeObject_HashInplace(entry_key,start);
  Dee_DECREF(entry_key);
  if (error != 0) return error;
  ACQUIRE;
 }
 RELEASE;
 *result = start;
 return 0;
}
DEE_COMPILER_PREFAST_WARNING_POP

void FUNC(DeeHashSet_Clear)(
 DEE_A_INOUT struct DeeHashSet *self LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashSetBucket *old_buckets,*begin,*end;
 DEE_ASSERT(self);
 ACQUIRE;
 begin = self->hs_valid_buckets_begin;
 end = self->hs_valid_buckets_end;
 old_buckets = self->hs_buckets;
 self->hs_valid_buckets_begin = NULL;
 self->hs_valid_buckets_end = NULL;
 self->hs_buckets = NULL;
 self->hs_bucket_count = 0;
 self->hs_map_size = 0;
 RELEASE;
 while (begin != end) _DeeHashSetBucket_Quit(begin++);
 free(old_buckets);
}

DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashSet_TInsertSequence)(
 DEE_A_INOUT struct DeeHashSet *self, DEE_A_IN DeeTypeObject const *tp_sequence,
 DEE_A_INOUT DeeObject *sequence LOCK_ARG(DEE_A_INOUT)) {
 DeeObject *iterator,*elem; int error;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(sequence));
 DEE_ASSERT(DeeObject_Check(tp_sequence) && DeeType_Check(tp_sequence));
 DEE_ASSERT(DeeObject_InstanceOf(sequence,tp_sequence));
 if (tp_sequence == &DeeSet_Type) {
  DeeHashSet_TRAVERSE_SAFE_VARS;
  struct _DeeHashSetBucketEntry *entry;
  Dee_hash_t hash; DeeSet_ACQUIRE(sequence);
  DeeHashSet_TRAVERSE_SAFE(entry,DeeSet_SET(sequence)) {
   Dee_INCREF(elem = entry->sbe_key);
   hash = entry->sbe_hash;
   DeeSet_RELEASE(sequence);
   error = AND_FUNC(DeeHashSet_InsertWithKnownBucket)(self,elem,hash LOCK_PAR);
   Dee_DECREF(elem);
   if DEE_UNLIKELY(error != 0) return error;
   DeeSet_ACQUIRE(sequence);
  }
  DeeSet_RELEASE(sequence);
  return 0;
 }
 if (tp_sequence == &DeeTuple_Type) {
  DeeObject **iter,**end;
  end = (iter = DeeTuple_ELEM(sequence))+DeeTuple_SIZE(sequence);
  while DEE_LIKELY(iter != end) {
   if DEE_UNLIKELY(FUNC(DeeHashSet_Insert)(self,*iter LOCK_PAR) != 0) return -1;
   ++iter;
  }
  return 0;
 }
 if (tp_sequence == &DeeList_Type) {
  Dee_size_t i;
  DeeList_ACQUIRE(sequence);
  for (i = 0; DEE_LIKELY(i < DeeList_SIZE(sequence)); ++i) {
   Dee_INCREF(elem = DeeList_GET(sequence,i));
   DeeList_RELEASE(sequence);
   error = FUNC(DeeHashSet_Insert)(self,elem LOCK_PAR);
   Dee_DECREF(elem);
   if DEE_UNLIKELY(error != 0) return error;
   DeeList_ACQUIRE(sequence);
  }
  return 0;
 }
 if (tp_sequence == &DeeCell_Type) {
  if ((elem = DeeCell_TryGet(sequence)) != NULL) {
   error = FUNC(DeeHashSet_Insert)(self,elem LOCK_PAR);
   Dee_DECREF(elem);
   if DEE_UNLIKELY(error != 0) return error;
  }
  return 0;
 }
 if (tp_sequence == &DeeSuper_Type) return FUNC(DeeHashSet_TInsertSequence)(
  self,DeeSuper_TYPE(sequence),DeeSuper_SELF(sequence) LOCK_PAR);
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_sequence,sequence)) == NULL) return -1;
 error = FUNC(DeeHashSet_InsertIterator)(self,iterator LOCK_PAR);
 Dee_DECREF(iterator);
 return error;
}
DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashSet_InsertIterator)(
 DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *iterator LOCK_ARG(DEE_A_INOUT)) {
 DeeObject *elem; int error;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(iterator));
 while DEE_LIKELY((error = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  error = FUNC(DeeHashSet_Insert)(self,elem LOCK_PAR);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(error != 0) return error;
 }
 return DEE_UNLIKELY(error < 0) ? error : 0;
}


DEE_A_RET_EXCEPT_FAIL(-1,1) int AND_FUNC(DeeHashSet_InsertWithKnownBucket)(
 DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob,
 DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashSetBucket *bucket_ob; DeeObject *entry_key;
 struct _DeeHashSetBucketEntry *iter,*end;
 Dee_size_t new_bucket_count; int temp;
again:
 ACQUIRE;
again2:
 if (self->hs_buckets) {
  if (!self->hs_map_size) { RELEASE; return 0; }
  bucket_ob = self->hs_buckets+(bucket % self->hs_bucket_count);
  end = (iter = bucket_ob->hsb_entryv)+bucket_ob->hsb_entryc;
  while (iter != end) {
   if (iter->sbe_hash == bucket) {
    Dee_INCREF(entry_key = iter->sbe_key);
    RELEASE;
    temp = DeeObject_CompareEq(entry_key,ob);
    Dee_DECREF(entry_key);
    if (temp == -1) {
     if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return -1;
    } else if (temp) return 1;
    ACQUIRE;
    if (bucket_ob != self->hs_buckets+(bucket % self->hs_bucket_count)) goto again2;
    // At this point we have confirmed 'b'
    // >> Now to make sure that 'node' is still reachable from there
    if (!_DeeHashSetBucket_ContainsEntry(bucket_ob,iter)) goto again2;
    // OK! Everything has worked out
   }
   ++iter;
  }
  new_bucket_count = _DeeSetCapacityOf(_DeeCeil((
   ++self->hs_map_size)*self->hs_max_load_factor));
  if (new_bucket_count > self->hs_bucket_count) {
   // Actually rehash the map (This can cause a memory error)
   if (_DeeHashSet_TryRehash(self,new_bucket_count) == -1) goto no_mem;
  }
  bucket_ob = self->hs_buckets+(bucket % self->hs_bucket_count);
  if (_DeeHashSetBucket_TryAddNew(bucket_ob,bucket,ob) == -1) {
   // This is really bad...
   // Because in order to fix this properly, we would have to rehash again.
   // But we can't take the risk of causing another error to be thrown,
   // so the only way we can safely fix this, is by not performing the rehash.
   // Doing so is OK, as in: it doesn't cause any problems further down the line,
   // but it's still not perfect...
   --self->hs_map_size;
   goto no_mem;
  }
  if (bucket_ob >= self->hs_valid_buckets_end) self->hs_valid_buckets_end = bucket_ob+1; else
  if (bucket_ob < self->hs_valid_buckets_begin) self->hs_valid_buckets_begin = bucket_ob;
 } else { // First entry
  if (_DeeHashSet_TryInsertFirstEntry(self,ob,bucket) == -1) {
no_mem:
   RELEASE;
   if DEE_LIKELY(Dee_CollectMemory()) goto again;
   DeeError_NoMemory();
   return -1;
  }
 }
 RELEASE;
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int AND_FUNC(DeeHashSet_RemoveWithKnownBucket)(
 DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob,
 DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT)) {
 Dee_size_t new_bucket_count; int temp; DeeObject *entry_key;
 struct _DeeHashSetBucket *bucket_ob;
 struct _DeeHashSetBucketEntry *iter,*end,*new_list;
again:
 ACQUIRE;
again2:
 if (self->hs_buckets) {
  bucket_ob = self->hs_buckets+(bucket % self->hs_bucket_count);
  DEE_ASSERT(self);
  end = (iter = bucket_ob->hsb_entryv)+bucket_ob->hsb_entryc;
  while (iter != end) {
   if (iter->sbe_hash == bucket) {
    Dee_INCREF(entry_key = iter->sbe_key);
    RELEASE;
    temp = DeeObject_CompareEq(iter->sbe_key,ob);
    Dee_DECREF(entry_key);
    if (temp == -1) {
     if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return -1;
     temp = 0;
    }
    ACQUIRE;
    if (bucket_ob != self->hs_buckets+(bucket % self->hs_bucket_count)) goto again2;
    // At this point we have confirmed 'b'
    // >> Now to make sure that 'node' is still reachable from there
    if (!_DeeHashSetBucket_ContainsEntry(bucket_ob,iter)) goto again2;
    // OK! Everything has worked out
    if (temp) { // Found it!
     Dee_INHERIT_REF(entry_key,iter->sbe_key);
     memcpy(iter,iter+1,((end-iter)-1)*sizeof(struct _DeeHashSetBucketEntry));
     if ((--bucket_ob->hsb_entryc) != 0) {
      if ((new_list = (struct _DeeHashSetBucketEntry *)realloc_nnz(
       bucket_ob->hsb_entryv,bucket_ob->hsb_entryc*sizeof(struct _DeeHashSetBucketEntry))) == NULL) {
       // Restore old list
       memcpy(iter+1,iter,((end-iter)-1)*sizeof(struct _DeeHashSetBucketEntry));
       Dee_INHERIT_REF(iter->sbe_key,entry_key);
       iter->sbe_hash = bucket;
       ++bucket_ob->hsb_entryc;
       RELEASE;
       if DEE_LIKELY(Dee_CollectMemory()) goto again;
       DeeError_NoMemory();
       return -1;
      }
      bucket_ob->hsb_entryv = new_list;
     } else {
      DEE_ASSERT(bucket_ob->hsb_entryv);
      free_nn(bucket_ob->hsb_entryv);
      bucket_ob->hsb_entryv = NULL;
     }
     new_bucket_count = _DeeSetCapacityOf(_DeeCeil((
      --self->hs_map_size)*self->hs_max_load_factor));
     if (new_bucket_count < self->hs_bucket_count) {
      // Actually rehash the map (This can raise a memory error)
      if (_DeeHashSet_TryRehash(self,new_bucket_count) == -1) {
       int DEE_ATTRIBUTE_UNUSED unused;
       // Try to recover 'ob'. But if this fails, it will be lost...
       unused = _DeeHashSetBucket_TryAddNew(
        self->hs_buckets+(bucket % self->hs_bucket_count),
        bucket,entry_key);
       RELEASE;
       Dee_DECREF(entry_key);
       if DEE_LIKELY(Dee_CollectMemory()) goto again;
       DeeError_NoMemory();
       return -1;
      }
     } else {
      DEE_ASSERT(self->hs_map_size && "This would have been detected by 'new_bucket_count < self->hs_bucket_count'");
      DEE_ASSERT(bucket_ob == self->hs_buckets+(bucket % self->hs_bucket_count));
      if (!bucket_ob->hsb_entryc) {
       if (bucket_ob == self->hs_valid_buckets_end-1) {
        while (!self->hs_valid_buckets_end[-1].hsb_entryc) --self->hs_valid_buckets_end;
       } else if (bucket_ob == self->hs_valid_buckets_begin) {
        while (!self->hs_valid_buckets_begin->hsb_entryc) ++self->hs_valid_buckets_begin;
       }
      }
     }
     RELEASE;
     Dee_DECREF(entry_key);
     return 0;
    }
   }
   ++iter;
  }
  RELEASE;
  return 1;
 } else { // First entry
  if (_DeeHashSet_TryInsertFirstEntry(self,ob,bucket) == -1) {
   RELEASE;
   if DEE_LIKELY(Dee_CollectMemory()) goto again;
   DeeError_NoMemory();
   return -1;
  }
 }
 RELEASE;
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int AND_FUNC(DeeHashSet_ContainsWithKnownBucket)(
 DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob,
 DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashSetBucket *bucket_ob; DeeObject *entry_key;
 struct _DeeHashSetBucketEntry *iter,*end; int temp;
 ACQUIRE;
again2:
 if (!self->hs_map_size) { RELEASE; return 0; }
 bucket_ob = self->hs_buckets+(bucket % self->hs_bucket_count);
 end = (iter = bucket_ob->hsb_entryv)+bucket_ob->hsb_entryc;
 while (iter != end) {
  if (iter->sbe_hash == bucket) {
   Dee_INCREF(entry_key = iter->sbe_key);
   RELEASE;
   temp = DeeObject_CompareEq(entry_key,ob);
   Dee_DECREF(entry_key);
   if (temp == -1) {
    if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return -1;
   } else if (temp) return 1;
   ACQUIRE;
   if (bucket_ob != self->hs_buckets+(bucket % self->hs_bucket_count)) goto again2;
   // At this point we have confirmed 'b'
   // >> Now to make sure that 'node' is still reachable from there
   if (!_DeeHashSetBucket_ContainsEntry(bucket_ob,iter)) goto again2;
   // OK! Everything has worked out
  }
  ++iter;
 }
 RELEASE;
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int AND_FUNC(DeeHashSet_ContainsPredWithKnownBucket)(
 DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *pred LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashSetBucket *bucket_ob,*bucket_iter,*bucket_end;
 DeeObject *entry_key,*pred_args,*pred_result;
 struct _DeeHashSetBucketEntry *iter,*end; int temp;
 ACQUIRE;
again2:
 if (!self->hs_map_size) { RELEASE; return 0; }
 bucket_ob = self->hs_buckets+(bucket % self->hs_bucket_count);
 end = (iter = bucket_ob->hsb_entryv)+bucket_ob->hsb_entryc;
 while (iter != end) {
  if (iter->sbe_hash == bucket) {
   Dee_INCREF(entry_key = iter->sbe_key);
   RELEASE;
   pred_args = DeeTuple_Pack(2,entry_key,ob);
   Dee_DECREF(entry_key);
   if (!pred_args) return -1;
   pred_result = DeeObject_Call(pred,pred_args);
   Dee_DECREF(pred_args);
   if (!pred_result) goto check_ni;
   temp = DeeObject_Bool(pred_result);
   Dee_DECREF(pred_result);
   if (temp == -1) {
check_ni:
    if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return -1;
   } else if (temp) return 1;
   ACQUIRE;
   if (bucket_ob != self->hs_buckets+(bucket % self->hs_bucket_count)) goto again2;
   // At this point we have confirmed 'b'
   // >> Now to make sure that 'node' is still reachable from there
   if (!_DeeHashSetBucket_ContainsEntry(bucket_ob,iter)) goto again2;
   // OK! Everything has worked out
  }
  ++iter;
 }
 // Didn't find it in the bucket it was most likely to be located in!
 // -> Must still search all other buckets since we don't know what 'pred' actually does...
 bucket_end = (bucket_iter = self->hs_buckets)+self->hs_bucket_count;
 while (bucket_iter != bucket_end) {
  if (bucket_iter != bucket_ob) { // Skip the one we already checked
   end = (iter = bucket_iter->hsb_entryv)+bucket_iter->hsb_entryc;
   while (iter != end) {
    Dee_INCREF(entry_key = iter->sbe_key);
    RELEASE;
    pred_args = DeeTuple_Pack(2,entry_key,ob);
    Dee_DECREF(entry_key);
    if (!pred_args) return -1;
    pred_result = DeeObject_Call(pred,pred_args);
    Dee_DECREF(pred_args);
    if (!pred_result) goto check_ni2;
    temp = DeeObject_Bool(pred_result);
    Dee_DECREF(pred_result);
    if (temp == -1) {
check_ni2:
     if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return -1;
    } else if (temp) return 1;
    ACQUIRE;
    if (bucket_ob != self->hs_buckets+(bucket % self->hs_bucket_count)) goto again2;
    // At this point we have confirmed 'b'
    // >> Now to make sure that 'node' is still reachable from there
    if (!_DeeHashSetBucket_ContainsEntry(bucket_ob,iter)) goto again2;
    // OK! Everything has worked out
    ++iter;
   }
  }
  ++bucket_iter;
 }
 RELEASE;
 return 0;
}

DEE_A_RET_EXCEPT_FAIL(-1,1) int FUNC(DeeHashSet_Insert)(
 DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(ob,DEE_HASH_START,&bucket) != 0) return -1;
 return AND_FUNC(DeeHashSet_InsertWithKnownBucket)(self,ob,bucket LOCK_PAR);
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int FUNC(DeeHashSet_Remove)(
 DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(ob,DEE_HASH_START,&bucket) != 0) return -1;
 return AND_FUNC(DeeHashSet_RemoveWithKnownBucket)(self,ob,bucket LOCK_PAR);
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int FUNC(DeeHashSet_Contains)(
 DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(ob,DEE_HASH_START,&bucket) != 0) return -1;
 return AND_FUNC(DeeHashSet_ContainsWithKnownBucket)(self,ob,bucket LOCK_PAR);
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int FUNC(DeeHashSet_ContainsPred)(
 DEE_A_INOUT struct DeeHashSet *self, DEE_A_INOUT DeeObject *ob,
 DEE_A_INOUT DeeObject *pred LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(ob,DEE_HASH_START,&bucket) != 0) return -1;
 return AND_FUNC(DeeHashSet_ContainsPredWithKnownBucket)(self,ob,bucket,pred LOCK_PAR);
}





DEE_A_RET_EXCEPT_REF DeeObject *FUNC(DeeHashSet_PopSomething)(
 DEE_A_INOUT struct DeeHashSet *self LOCK_ARG(DEE_A_INOUT)) {
 Dee_size_t new_bucket_count;
 Dee_hash_t old_hash; DeeObject *result;
 DEE_ASSERT(self);
again:
 ACQUIRE;
 if DEE_UNLIKELY(!self->hs_buckets) { // empty self doesn't contain anything!
  RELEASE;
  DeeError_SET_STRING(&DeeErrorType_ValueError,"Empty mapping");
  return NULL;
 }
 if DEE_LIKELY((result = _DeeHashSetBucket_TryPopSomething(
  self->hs_valid_buckets_begin,&old_hash)) != NULL) {
  new_bucket_count = _DeeSetCapacityOf(_DeeCeil(
   (--self->hs_map_size)*self->hs_max_load_factor));
  if (new_bucket_count < self->hs_bucket_count) {
   // Actually rehash the map (This can raise a memory error)
   if (_DeeHashSet_TryRehash(self,new_bucket_count) == -1) {
    int DEE_ATTRIBUTE_UNUSED unused;
    // Try to recover 'result'. But if this fails, it will be lost...
    unused = _DeeHashSetBucket_TryAddNew(self->hs_buckets+(
     old_hash % self->hs_bucket_count),old_hash,result);
    RELEASE;
    Dee_DECREF(result);
    return NULL;
   }
  } else {
   DEE_ASSERT(self->hs_map_size && "This would have been detected by 'new_bucket_count < self->hs_bucket_count'");
   while (!self->hs_valid_buckets_begin->hsb_entryc) ++self->hs_valid_buckets_begin;
  }
 } else {
  RELEASE;
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return NULL;
 }
 RELEASE;
 return result;
}


DEE_A_RET_EXCEPT_REF DeeObject *FUNC(DeeHashSet_GetKeyRangeIndex)(
 DEE_A_IN struct DeeHashSet const *self, DEE_A_IN Dee_size_t lo,
 DEE_A_IN Dee_size_t hi LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashSetBucketEntry *entry;
 DeeObject *result,**dst; Dee_size_t used_hi;
 DeeHashSet_TRAVERSE_RANGE_VARS;
again:
 ACQUIRE;
 if ((used_hi = hi) > DeeHashSet_SIZE(self)) used_hi = DeeHashSet_SIZE(self);
 if (used_hi < lo) { RELEASE; return DeeList_New(); }
 if ((result = _DeeList_TryWeakNewUnsafe(used_hi-lo)) == NULL) {
  RELEASE;
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return NULL;
 }
 dst = DeeList_ELEM(result);
 DeeHashSet_TRAVERSE_RANGE(entry,self,lo,hi) {
  DEE_ASSERT(dst < DeeList_ELEM(result)+DeeList_SIZE(result));
  Dee_INCREF(*dst++ = entry->sbe_key);
 }
 RELEASE;
 return result;
}


DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t AND_FUNC(DeeHashSet_FindKeyIndexWithKnownBucket)(
 DEE_A_IN struct DeeHashSet const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT)) {
 Dee_ssize_t result; int temp; DeeObject *entry_key;
 struct _DeeHashSetBucket *bucket_ob,*bucket_iter;
 struct _DeeHashSetBucketEntry *iter,*end;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(key));
 ACQUIRE;
again2:
 if (!self->hs_map_size) return -1;
 bucket_ob = self->hs_buckets+(bucket % self->hs_bucket_count),result = 0;
 end = (iter = bucket_ob->hsb_entryv)+bucket_ob->hsb_entryc;
 while (iter != end) {
  if (iter->sbe_hash == bucket) {
   Dee_INCREF(entry_key = iter->sbe_key);
   RELEASE;
   temp = DeeObject_CompareEq(entry_key,key);
   Dee_DECREF(entry_key);
   if (temp == -1) {
    if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return -2;
    temp = 0;
   }
   ACQUIRE;
   if (bucket_ob != self->hs_buckets+(bucket % self->hs_bucket_count)) goto again2;
   // At this point we have confirmed 'b'
   // >> Now to make sure that 'node' is still reachable from there
   if (!_DeeHashSetBucket_ContainsEntry(bucket_ob,iter)) goto again2;
   // OK! Everything has worked out
   if (temp) { // found it!
    // Count all keys in all buckets before 'bucket_ob'
    bucket_iter = self->hs_valid_buckets_begin;
    while (bucket_iter != bucket_ob) {
     end = (iter = bucket_iter->hsb_entryv)+bucket_iter->hsb_entryc;
     while (iter != end) ++iter,++result;
     ++bucket_iter;
    }
    return result;
   }
  }
  ++iter,++result;
 }
 RELEASE;
 return -1; // Not found
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t FUNC(DeeHashSet_FindKeyIndex)(
 DEE_A_IN struct DeeHashSet const *self, DEE_A_INOUT DeeObject *key LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return -2;
 return AND_FUNC(DeeHashSet_FindKeyIndexWithKnownBucket)(self,key,bucket LOCK_PAR);
}

DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashSet_Traverse)(
 DEE_A_IN struct DeeHashSet const *self,
 DEE_A_IN DeeHashSetTraverseFunc trav,
 DEE_A_IN_OPT void *closure LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashSetBucketEntry *entry; DeeObject *entry_key;
 DeeHashSet_TRAVERSE_SAFE_VARS; int temp;
 DEE_ASSERT(self && trav);
 ACQUIRE;
 DeeHashSet_TRAVERSE_SAFE(entry,self) {
  Dee_INCREF(entry_key = entry->sbe_key);
  RELEASE;
  temp = (*trav)(entry_key,closure);
  Dee_DECREF(entry_key);
  if (temp != 0) return temp;
  ACQUIRE;
 }
 RELEASE;
 return 0;
}




#undef FUNC
#undef AND_FUNC
#undef LOCK_ARG
#undef LOCK_PAR
#undef ACQUIRE
#undef RELEASE

#ifdef WITH_LOCK
#undef WITH_LOCK
#endif
#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
