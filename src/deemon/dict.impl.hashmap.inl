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
#include "dict.c"
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


DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashMap_InitCopy)(
 DEE_A_OUT struct DeeHashMap *self,
 DEE_A_IN struct DeeHashMap const *right LOCK_ARG(DEE_A_INOUT)) {
 DeeHashMap_TRAVERSE_SAFE_VARS; int temp;
 struct _DeeHashMapNode *node;
 DeeObject *node_key,*node_item;
 Dee_hash_t node_bucket;
 DEE_ASSERT(self && right);
 DeeHashMap_Init(self);
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,right) {
  Dee_INCREF(node_key = node->hmn_key);
  Dee_INCREF(node_item = node->hmn_item);
  node_bucket = node->hmn_hash;
  RELEASE;
  temp = DeeHashMap_SetItemWithKnownBucket(self,node_key,node_bucket,node_item);
  Dee_DECREF(node_item);
  Dee_DECREF(node_key);
  if (temp != 0) { DeeHashMap_Quit(self); return -1; }
  ACQUIRE;
 }
 RELEASE;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashMap_InitDeepCopy)(
 DEE_A_OUT struct DeeHashMap *self,
 DEE_A_IN struct DeeHashMap const *right LOCK_ARG(DEE_A_INOUT)) {
 DeeHashMap_TRAVERSE_SAFE_VARS; int temp;
 struct _DeeHashMapNode *node;
 DeeObject *node_key,*node_item,*item_copy;
 Dee_hash_t node_bucket;
 DEE_ASSERT(self && right);
 DeeHashMap_Init(self);
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,right) {
  Dee_INCREF(node_key = node->hmn_key);
  Dee_INCREF(node_item = node->hmn_item);
  node_bucket = node->hmn_hash;
  RELEASE;
  item_copy = DeeObject_DeepCopy(node_item);
  Dee_DECREF(node_item);
  if (item_copy) {
   temp = DeeHashMap_SetItemWithKnownBucket(self,node_key,node_bucket,item_copy);
   Dee_DECREF(item_copy);
  } else temp = -1;
  Dee_DECREF(node_key);
  if (temp != 0) { DeeHashMap_Quit(self); return -1; }
  ACQUIRE;
 }
 RELEASE;
 return 0;
}

DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashMap_TInsertSequence)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN DeeTypeObject const *tp_sequence,
 DEE_A_INOUT DeeObject *sequence LOCK_ARG(DEE_A_INOUT)) {
 int error;
 // TODO: Optimization for dicts
 if (tp_sequence == &DeeSuper_Type) return FUNC(DeeHashMap_TInsertSequence)(
  self,DeeSuper_TYPE(sequence),DeeSuper_SELF(sequence) LOCK_PAR);
 if ((sequence = DeeObject_TIterSelf(tp_sequence,sequence)) == NULL) return -1;
 error = FUNC(DeeHashMap_InsertIterator)(self,sequence LOCK_PAR);
 Dee_DECREF(sequence);
 return error;
}

DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashMap_InsertIterator)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *iterator LOCK_ARG(DEE_A_INOUT)) {
 DeeObject *elem,*ki_iter,*key,*item; int error;
 while DEE_LIKELY((error = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  if (DeeTuple_Check(elem) && DeeTuple_SIZE(elem) >= 2) {
   if (DeeHashMap_SetItem(self,DeeTuple_GET(elem,0),DeeTuple_GET(elem,1)) != 0) {
err: Dee_DECREF(elem); DeeHashMap_Quit(self); return -1;
   }
  } else {
   if ((ki_iter = DeeObject_IterSelf(elem)) == NULL) goto err;
   if ((error = DeeObject_IterNextEx(ki_iter,&key)) != 0) {
    if (error > 0) _deehashmap_itersize("0");
err_kii_iter: Dee_DECREF(ki_iter); goto err;
   }
   if ((error = DeeObject_IterNextEx(ki_iter,&item)) != 0) {
    if (error > 0) _deehashmap_itersize("1");
err_kiik_iter: Dee_DECREF(key); goto err_kii_iter;
   }
   if (FUNC(DeeHashMap_SetItem)(self,key,item LOCK_PAR) != 0) goto err_kiik_iter;
   Dee_DECREF(item);
   Dee_DECREF(key);
   if ((error = DeeObject_IterNextEx(ki_iter,&key)) <= 0) {
    if (error == 0) { _deehashmap_itersize("> 2"); Dee_DECREF(key); }
    goto err_kii_iter;
   }
   Dee_DECREF(ki_iter);
  }
  Dee_DECREF(elem);
 }
 return DEE_UNLIKELY(error < 0) ? error : 0;
}


void FUNC(DeeHashMap_Clear)(DEE_A_INOUT struct DeeHashMap *self LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode **buckets,**buckets_begin,**buckets_end,*node,*next;
 ACQUIRE;
 buckets_begin = self->hm_valid_buckets_begin;
 buckets_end = self->hm_valid_buckets_end;
 buckets = self->hm_buckets;
 self->hm_valid_buckets_begin = NULL;
 self->hm_valid_buckets_end = NULL;
 self->hm_buckets = NULL;
 self->hm_bucket_count = 0;
 self->hm_map_size = 0;
 RELEASE;
 while (buckets_begin != buckets_end) {
  node = *buckets_begin++;
  while (node) {
   next = node->hmn_next;
   Dee_DECREF(node->hmn_key);
   Dee_DECREF(node->hmn_item);
   free_nn(node);
   node = next;
  }
 }
 // Free the underlying list of buckets
 free(buckets);
}

DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashMap_Traverse)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN DeeHashMapTraverseFunc trav,
 DEE_A_IN_OPT void *closure LOCK_ARG(DEE_A_INOUT_LOCKED)) {
 DeeHashMap_TRAVERSE_SAFE_VARS;
#ifdef WITH_LOCK
 DeeObject *key,*item;
#endif
 struct _DeeHashMapNode *node; int temp;
 DEE_ASSERT(self && trav);
 DeeHashMap_TRAVERSE_SAFE(node,self) {
#ifdef WITH_LOCK
  Dee_INCREF(key = node->hmn_key);
  Dee_INCREF(item = node->hmn_item);
  RELEASE;
  temp = (*trav)(key,item,closure);
  Dee_DECREF(item);
  Dee_DECREF(key);
#else
  temp = (*trav)(node->hmn_key,node->hmn_item,closure);
#endif
  if (temp != 0) return temp;
  ACQUIRE;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashMap_TraverseKeys)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN DeeObjectIterateFunc trav,
 DEE_A_IN_OPT void *closure LOCK_ARG(DEE_A_INOUT_LOCKED)) {
 DeeHashMap_TRAVERSE_SAFE_VARS;
#ifdef WITH_LOCK
 DeeObject *temp_ob;
#endif
 struct _DeeHashMapNode *node; int temp;
 DEE_ASSERT(self && trav);
 DeeHashMap_TRAVERSE_SAFE(node,self) {
#ifdef WITH_LOCK
  Dee_INCREF(temp_ob = node->hmn_key);
  RELEASE;
  temp = (*trav)(temp_ob,closure);
  Dee_DECREF(temp_ob);
#else
  temp = (*trav)(node->hmn_key,closure);
#endif
  ACQUIRE;
  if (temp != 0) return temp;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashMap_TraverseItems)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN DeeObjectIterateFunc trav,
 DEE_A_IN_OPT void *closure LOCK_ARG(DEE_A_INOUT_LOCKED)) {
 DeeHashMap_TRAVERSE_SAFE_VARS;
#ifdef WITH_LOCK
 DeeObject *temp_ob;
#endif
 struct _DeeHashMapNode *node; int temp;
 DEE_ASSERT(self && trav);
 DeeHashMap_TRAVERSE_SAFE(node,self) {
#ifdef WITH_LOCK
  Dee_INCREF(temp_ob = node->hmn_item);
  RELEASE;
  temp = (*trav)(temp_ob,closure);
  Dee_DECREF(temp_ob);
#else
  temp = (*trav)(node->hmn_item,closure);
#endif
  ACQUIRE;
  if (temp != 0) return temp;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashMap_TraverseValues)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN DeeObjectIterateFunc trav,
 DEE_A_IN_OPT void *closure LOCK_ARG(DEE_A_INOUT_LOCKED)) {
 DeeHashMap_TRAVERSE_SAFE_VARS;
#ifdef WITH_LOCK
 DeeObject *key,*item;
#endif
 DeeObject *value;
 struct _DeeHashMapNode *node; int temp;
 DEE_ASSERT(self); DEE_ASSERT(trav);
 DeeHashMap_TRAVERSE_SAFE(node,self) {
#ifdef WITH_LOCK
  Dee_INCREF(key = node->hmn_key);
  Dee_INCREF(item = node->hmn_item);
  RELEASE;
  value = DeeTuple_Pack(2,key,item);
  Dee_DECREF(item);
  Dee_DECREF(key);
#else
  value = DeeTuple_Pack(2,node->hmn_key,node->hmn_item);
#endif
  if (!value) return -1;
  temp = (*trav)(value,closure);
  Dee_DECREF(value);
  ACQUIRE;
  if (temp != 0) return temp;
 }
 return 0;
}





DEE_A_RET_EXCEPT_REF DeeObject *FUNC(DeeHashMap_LocateKey)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred LOCK_ARG(DEE_A_INOUT)) {
 DeeHashMap_TRAVERSE_SAFE_VARS; DeeObject *args,*temp; int itemp;
 struct _DeeHashMapNode *node; DeeObject *key;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(pred));
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,self) {
  Dee_INCREF(key = node->hmn_key);
  RELEASE;
  args = DeeTuple_Pack(1,key);
  if (!args) goto err;
  temp = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if (!temp) goto err;
  itemp = DeeObject_Bool(temp);
  Dee_DECREF(temp);
  if (itemp < 0) {
err: Dee_DECREF(key); return NULL;
  }
  if (itemp) return key; // inherit reference
  Dee_DECREF(key);
  ACQUIRE;
 }
 RELEASE;
 DeeError_ValueError_NotFound_Pred_T(&DeeDictKeys_Type,pred);
 return NULL;
}
DEE_A_RET_EXCEPT_REF DeeObject *FUNC(DeeHashMap_LocateItem)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred LOCK_ARG(DEE_A_INOUT)) {
 DeeHashMap_TRAVERSE_SAFE_VARS; DeeObject *args,*temp; int itemp;
 struct _DeeHashMapNode *node; DeeObject *item;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(pred));
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,self) {
  Dee_INCREF(item = node->hmn_item);
  RELEASE;
  args = DeeTuple_Pack(1,item);
  if (!args) goto err;
  temp = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if (!temp) goto err;
  itemp = DeeObject_Bool(temp);
  Dee_DECREF(temp);
  if (itemp < 0) {
err: Dee_DECREF(item); return NULL;
  }
  if (itemp) return item; // inherit reference
  Dee_DECREF(item);
  ACQUIRE;
 }
 RELEASE;
 DeeError_ValueError_NotFound_Pred_T(&DeeDictKeys_Type,pred);
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *FUNC(DeeHashMap_LocateValue)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred LOCK_ARG(DEE_A_INOUT)) {
 DeeHashMap_TRAVERSE_SAFE_VARS;
 DeeObject *value,*args,*temp; int itemp;
 struct _DeeHashMapNode *node;
#ifdef WITH_LOCK
 DeeObject *key,*item;
#endif
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(pred));
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,self) {
#ifdef WITH_LOCK
  Dee_INCREF(key = node->hmn_key);
  Dee_INCREF(item = node->hmn_item);
  RELEASE;
  value = DeeTuple_Pack(2,key,item);
  Dee_DECREF(key);
  Dee_DECREF(item);
#else
  value = DeeTuple_Pack(2,node->hmn_key,node->hmn_item);
#endif
  if (!value) goto err2;
  if ((args = DeeTuple_Pack(1,value)) == NULL) goto err;
  temp = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if (!temp) goto err;
  itemp = DeeObject_Bool(temp);
  Dee_DECREF(temp);
  if (itemp < 0) {
err: Dee_DECREF(value);
err2: return NULL;
  }
  if (itemp) return value; // inherit reference
  Dee_DECREF(value);
  ACQUIRE;
 }
 RELEASE;
 DeeError_ValueError_NotFound_Pred_T(&DeeDictKeys_Type,pred);
 return NULL;
}
DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashMap_LocateAll)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT struct DeeHashMap *dst,
 DEE_A_INOUT DeeObject *pred LOCK_ARG(DEE_A_INOUT)) {
 DeeHashMap_TRAVERSE_SAFE_VARS; DeeObject *args,*elem,*temp; int itemp;
 DeeObject *key,*item; Dee_hash_t bucket;
 struct _DeeHashMapNode *node;
 DEE_ASSERT(self && dst);
 DEE_ASSERT(DeeObject_Check(pred));
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,self) {
  bucket = node->hmn_hash;
  Dee_INCREF(key = node->hmn_key);
  Dee_INCREF(item = node->hmn_item);
  RELEASE;
  elem = DeeTuple_Pack(2,key,item);
  if (!elem) goto err2;
  args = DeeTuple_Pack(1,elem);
  Dee_DECREF(elem);
  if (!args) goto err2;
  temp = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if (!temp) {err2: Dee_DECREF(item); Dee_DECREF(key); return -1; }
  itemp = DeeObject_Bool(temp);
  Dee_DECREF(temp);
  if (itemp > 0) itemp = DeeHashMap_SetItemWithKnownBucket(dst,key,bucket,item);
  Dee_DECREF(item);
  Dee_DECREF(key);
  if (itemp != 0) return -1;
  ACQUIRE;
 }
 RELEASE;
 return 0;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *FUNC(DeeHashMap_LocateAllKeys)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred LOCK_ARG(DEE_A_INOUT)) {
 DeeHashMap_TRAVERSE_SAFE_VARS; DeeObject *args,*temp,*result; int itemp;
 struct _DeeHashMapNode *node; DeeObject *key;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(pred));
 if ((result = DeeList_New()) == NULL) return NULL;
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,self) {
  Dee_INCREF(key = node->hmn_key);
  RELEASE;
  args = DeeTuple_Pack(1,key);
  if (!args) {
err_kr: Dee_DECREF(key);
err_r: Dee_DECREF(result);
   return NULL;
  }
  temp = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if (!temp) goto err_kr;
  itemp = DeeObject_Bool(temp);
  Dee_DECREF(temp);
  if (itemp > 0) itemp = DeeList_Append(result,key);
  Dee_DECREF(key);
  if (itemp != 0) goto err_r;
  ACQUIRE;
 }
 RELEASE;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *FUNC(DeeHashMap_LocateAllItems)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred LOCK_ARG(DEE_A_INOUT)) {
 DeeHashMap_TRAVERSE_SAFE_VARS; DeeObject *args,*temp,*result; int itemp;
 struct _DeeHashMapNode *node; DeeObject *item;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(pred));
 if ((result = DeeList_New()) == NULL) return NULL;
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,self) {
  Dee_INCREF(item = node->hmn_item);
  RELEASE;
  args = DeeTuple_Pack(1,item);
  if (!args) {
err_kr: Dee_DECREF(item);
err_r: Dee_DECREF(result);
   return NULL;
  }
  temp = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if (!temp) goto err_kr;
  itemp = DeeObject_Bool(temp);
  Dee_DECREF(temp);
  if (itemp > 0) itemp = DeeList_Append(result,item);
  Dee_DECREF(item);
  if (itemp != 0) goto err_r;
  ACQUIRE;
 }
 RELEASE;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *FUNC(DeeHashMap_LocateAllValues)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred LOCK_ARG(DEE_A_INOUT)) {
 DeeHashMap_TRAVERSE_SAFE_VARS;
 DeeObject *value,*args,*temp,*result; int itemp;
 struct _DeeHashMapNode *node;
#ifdef WITH_LOCK
 DeeObject *key,*item;
#endif
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(pred));
 if ((result = DeeList_New()) == NULL) return NULL;
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,self) {
#ifdef WITH_LOCK
  Dee_INCREF(key = node->hmn_key);
  Dee_INCREF(item = node->hmn_item);
  RELEASE;
  value = DeeTuple_Pack(2,key,item);
  Dee_DECREF(item);
  Dee_DECREF(key);
#else
  value = DeeTuple_Pack(2,node->hmn_key,node->hmn_item);
#endif
  if (!value) goto err_r;
  args = DeeTuple_Pack(1,value);
  if (!args) {
err_vr: Dee_DECREF(value);
err_r: Dee_DECREF(result);
   return NULL;
  }
  temp = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if (!temp) goto err_vr;
  itemp = DeeObject_Bool(temp);
  Dee_DECREF(temp);
  if (itemp > 0) itemp = DeeList_Append(result,value);
  Dee_DECREF(value);
  if (itemp != 0) goto err_r;
  ACQUIRE;
 }
 RELEASE;
 return result;
}
DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashMap_Transform)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT struct DeeHashMap *dst,
 DEE_A_INOUT DeeObject *pred LOCK_ARG(DEE_A_INOUT)) {
 DeeHashMap_TRAVERSE_SAFE_VARS; DeeObject *args,*elem,*temp,*iterator,*key,*item; int itemp;
 struct _DeeHashMapNode *node; int error;
 DEE_ASSERT(self && dst);
 DEE_ASSERT(DeeObject_Check(pred));
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,self) {
#ifdef WITH_LOCK
  Dee_INCREF(key = node->hmn_key);
  Dee_INCREF(item = node->hmn_item);
  RELEASE;
  elem = DeeTuple_Pack(2,key,item);
  Dee_DECREF(item);
  Dee_DECREF(key);
#else
  RELEASE;
  elem = DeeTuple_Pack(2,node->hmn_key,node->hmn_item);
#endif
  if (!elem) goto err;
  args = DeeTuple_Pack(1,elem);
  Dee_DECREF(elem);
  if (!args) goto err;
  temp = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if (!temp) goto err;
  iterator = DeeObject_IterSelf(temp);
  Dee_DECREF(temp);
  if (!iterator) goto err;
  if ((error = DeeObject_IterNextEx(iterator,&key)) != 0) {
   if (error > 0) DeeError_SET_STRING(&DeeErrorType_ValueError,"Expected iterable of size 2 (got 0)");
err_iter:
   Dee_DECREF(iterator);
   goto err;
  }
  if ((error = DeeObject_IterNextEx(iterator,&item)) != 0) {
   if (error > 0) DeeError_SET_STRING(&DeeErrorType_ValueError,"Expected iterable of size 2 (got 1)");
err_key:
   Dee_DECREF(key);
   goto err_iter;
  }
  if ((error = DeeObject_IterNextEx(iterator,&temp)) == 0) {
   DeeError_SET_STRING(&DeeErrorType_ValueError,"Expected iterable of size 2 (got >= 3)");
   Dee_DECREF(temp);
err_item:
   Dee_DECREF(item);
   goto err_key;
  } else if (error < 0) goto err_item;
  Dee_DECREF(iterator);
  itemp = DeeHashMap_SetItem(dst,key,item);
  Dee_DECREF(key);
  Dee_DECREF(item);
  if (itemp != 0) {err: return -1; }
  ACQUIRE;
 }
 RELEASE;
 return 0;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *FUNC(DeeHashMap_TransformKeys)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred LOCK_ARG(DEE_A_INOUT)) {
 DeeHashMap_TRAVERSE_SAFE_VARS; DeeObject *args,*temp,*result; int itemp;
 struct _DeeHashMapNode *node;
#ifdef WITH_LOCK
 DeeObject *key;
#endif
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(pred));
 if ((result = DeeList_New()) == NULL) goto err;
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,self) {
#ifdef WITH_LOCK
  Dee_INCREF(key = node->hmn_key);
  RELEASE;
  args = DeeTuple_Pack(1,key);
  Dee_DECREF(key);
#else
  args = DeeTuple_Pack(1,node->hmn_key);
#endif
  if (!args) {
err_r: Dee_DECREF(result);
err: return NULL;
  }
  temp = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if (!temp) goto err_r;
  itemp = DeeList_Append(result,temp);
  Dee_DECREF(temp);
  if (itemp != 0) goto err_r;
  ACQUIRE;
 }
 RELEASE;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *FUNC(DeeHashMap_TransformItems)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred LOCK_ARG(DEE_A_INOUT)) {
 DeeHashMap_TRAVERSE_SAFE_VARS; DeeObject *args,*temp,*result; int itemp;
 struct _DeeHashMapNode *node;
#ifdef WITH_LOCK
 DeeObject *item;
#endif
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(pred));
 if ((result = DeeList_New()) == NULL) goto err;
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,self) {
#ifdef WITH_LOCK
  Dee_INCREF(item = node->hmn_item);
  RELEASE;
  args = DeeTuple_Pack(1,item);
  Dee_DECREF(item);
#else
  args = DeeTuple_Pack(1,node->hmn_item);
#endif
  if (!args) {
err_r: Dee_DECREF(result);
err: return NULL;
  }
  temp = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if (!temp) goto err_r;
  itemp = DeeList_Append(result,temp);
  Dee_DECREF(temp);
  if (itemp != 0) goto err_r;
  ACQUIRE;
 }
 RELEASE;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *FUNC(DeeHashMap_TransformValues)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *pred LOCK_ARG(DEE_A_INOUT)) {
 DeeHashMap_TRAVERSE_SAFE_VARS; DeeObject *args,*temp,*elem,*result; int itemp;
#ifdef WITH_LOCK
 DeeObject *key,*item;
#endif
 struct _DeeHashMapNode *node;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(pred));
 if ((result = DeeList_New()) == NULL) return NULL;
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,self) {
#ifdef WITH_LOCK
  Dee_INCREF(key = node->hmn_key);
  Dee_INCREF(item = node->hmn_item);
  RELEASE;
  elem = DeeTuple_Pack(2,key,item);
  Dee_DECREF(item);
  Dee_DECREF(key);
#else
  elem = DeeTuple_Pack(2,node->hmn_key,node->hmn_item);
#endif
  if (!elem) {
err_r: Dee_DECREF(result);
   return NULL;
  }
  args = DeeTuple_Pack(1,elem);
  Dee_DECREF(elem);
  if (!args) goto err_r;
  temp = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if (!temp) goto err_r;
  itemp = DeeList_Append(result,temp);
  Dee_DECREF(temp);
  if (itemp != 0) goto err_r;
  ACQUIRE;
 }
 RELEASE;
 return result;
}



DEE_A_RET_EXCEPT_REF DeeObject *FUNC(DeeHashMap_Pop)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT_OPT DeeObject *def LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 DEE_ASSERT(DeeObject_Check(key));
 if (DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return NULL;
 return AND_FUNC(DeeHashMap_PopWithKnownBucket)(self,key,bucket,def LOCK_PAR);
}
DEE_A_RET_EXCEPT_REF DeeObject *FUNC(DeeHashMap_PopString)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key,
 DEE_A_INOUT_OPT DeeObject *def LOCK_ARG(DEE_A_INOUT)) {
 return AND_FUNC(DeeHashMap_PopStringWithKnownBucket)(
  self,key,DeeHash_String(key,DEE_HASH_START),def LOCK_PAR);
}

DEE_A_RET_EXCEPT_REF DeeObject *AND_FUNC(DeeHashMap_PopWithKnownBucket)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT_OPT DeeObject *def LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode **b,*prev_node,*node; int temp;
 Dee_size_t new_bucket_count;
 DeeObject *result,*node_key;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(key));
#ifdef DEE_DEBUG
 {Dee_hash_t hash_check;
  if (DeeObject_HashEx(key,DEE_HASH_START,&hash_check) == 0)
   DEE_ASSERT(bucket == hash_check);
  else DeeError_Handled();}
#endif
 ACQUIRE;
 if (!self->hm_buckets) { // empty self doesn't contain anything!
not_found:
  RELEASE;
  if (!def) { _deehashmap_error_not_found(key); return NULL; }
  DeeReturn_NEWREF(def);
 }
again:
 node = *(b = self->hm_buckets+(bucket % self->hm_bucket_count));
 prev_node = NULL;
 while (node) {
  if (node->hmn_hash == bucket) {
   Dee_INCREF(node_key = node->hmn_key);
   RELEASE;
   temp = DeeObject_CompareEq(node_key,key);
   Dee_DECREF(node_key);
   if (temp < 0) { // Ignore not_implemented errors here
    if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return NULL;
    temp = 0;
   }
   ACQUIRE;
   if (b != self->hm_buckets+(bucket % self->hm_bucket_count)) goto again;
   // At this point we have confirmed 'b'
   // >> Now to make sure that 'node' is still reachable from there
   if (!_DeeHashMapNode_Reachable(*b,node)) goto again;
   // OK! Everything has worked out

   if (temp != 0) { // found it!
    // Step #1: Remove the node
    DEE_ASSERT((node->hmn_prev == NULL) == (node == *b));
    if (node->hmn_prev) {
     node->hmn_prev->hmn_next = node->hmn_next;
    } else *b = node->hmn_next;
    if (node->hmn_next) node->hmn_next->hmn_prev = node->hmn_prev;
    // Step #2: Rehash the map, if necessary
    new_bucket_count = _DeeDictCapacityOf(_DeeCeil(
     (--self->hm_map_size)*self->hm_max_load_factor));
    if (new_bucket_count < self->hm_bucket_count) {
     // Actually rehash the self (This can raise a memory error, yet isn't DEE_A_EXEC. So it's fine)
     if (_DeeHashMap_Rehash(self,new_bucket_count) != 0) {
      // Must re-insert the node into "*b"
      node->hmn_prev = NULL;
      if ((node->hmn_next = *b) != NULL) node->hmn_next->hmn_prev = node;
      *b = node;
      RELEASE;
      return NULL;
     }
     // NOTE: At this point, "b" was invalidated
    } else {
     DEE_ASSERT(self->hm_map_size && "This would have been detected by 'new_bucket_count < self->hm_bucket_count'");
     DEE_ASSERT(b == self->hm_buckets+(bucket % self->hm_bucket_count));
     if (!*b) {
      if (b == self->hm_valid_buckets_end-1) {
       while (!self->hm_valid_buckets_end[-1]) --self->hm_valid_buckets_end;
      } else if (b == self->hm_valid_buckets_begin) {
       while (!*self->hm_valid_buckets_begin) ++self->hm_valid_buckets_begin;
      }
     }
    }
    RELEASE;
    // Step #3: Free the node
    Dee_DECREF(node->hmn_key);
    Dee_INHERIT_REF(result,node->hmn_item);
    free_nn(node);
    DEE_ASSERT(DeeObject_Check(result));
    return result; // Done!
   }
  }
  node = node->hmn_next;
 }
 goto not_found;
}
DEE_A_RET_EXCEPT_REF DeeObject *AND_FUNC(DeeHashMap_PopStringWithKnownBucket)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT_OPT DeeObject *def LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode **b,*node;
 DeeObject *key_ob; DeeObject *result;
 Dee_size_t new_bucket_count;
 DEE_ASSERT(self && key);
 DEE_ASSERT(bucket == DeeHash_String(key,DEE_HASH_START));
 ACQUIRE;
 if (!self->hm_buckets) { // empty self doesn't contain anything!
  RELEASE;
  if (!def) { _deehashmap_error_not_found_s(key); return NULL; }
  DeeReturn_NEWREF(def);
 }
 node = *(b = self->hm_buckets+(bucket % self->hm_bucket_count));
 while (node) {
  if (node->hmn_hash == bucket &&
      DeeString_Check(node->hmn_key) &&
      strcmp(DeeString_STR(node->hmn_key),key) == 0) { // found it!
   // Step #1: Remove the node
   DEE_ASSERT((node->hmn_prev == NULL) == (node == *b));
   if (node->hmn_prev) {
    node->hmn_prev->hmn_next = node->hmn_next;
   } else *b = node->hmn_next;
   if (node->hmn_next) node->hmn_next->hmn_prev = node->hmn_prev;
   // Step #2: Rehash the map, if necessary
   new_bucket_count = _DeeDictCapacityOf(_DeeCeil(
    (--self->hm_map_size)*self->hm_max_load_factor));
   if (new_bucket_count < self->hm_bucket_count) {
    // Actually rehash the self (This can raise a memory error)
    if (_DeeHashMap_Rehash(self,new_bucket_count) != 0) {
     // Must re-insert the node into "*b"
     node->hmn_prev = NULL;
     if ((node->hmn_next = *b) != NULL) node->hmn_next->hmn_prev = node;
     *b = node;
     return NULL;
    }
    // NOTE: At this point, "b" was invalidated
   } else {
    DEE_ASSERT(self->hm_map_size && "This would have been detected by 'new_bucket_count < self->hm_bucket_count'");
    DEE_ASSERT(b == self->hm_buckets+(bucket % self->hm_bucket_count));
    if (!*b) {
     if (b == self->hm_valid_buckets_end-1) {
      while (!self->hm_valid_buckets_end[-1]) --self->hm_valid_buckets_end;
     } else if (b == self->hm_valid_buckets_begin) {
      while (!*self->hm_valid_buckets_begin) ++self->hm_valid_buckets_begin;
     }
    }
   }
   RELEASE;
   // Step #3: Free the node
   Dee_DECREF(node->hmn_key);
   Dee_INHERIT_REF(result,node->hmn_item);
   free_nn(node);
   DEE_ASSERT(DeeObject_Check(result));
   return result; // Done!
  }
  node = node->hmn_next;
 }
 RELEASE;
 // Search by object
 if ((key_ob = DeeString_New(key)) == NULL) return NULL;
 result = DeeHashMap_PopWithKnownBucket(self,key_ob,bucket,def);
 Dee_DECREF(key_ob);
 return result;
}


DEE_A_RET_EXCEPT_REF DeeObject *AND_FUNC(DeeHashMap_SetDefaultWithKnownBucket)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node; DeeObject *result;
 DEE_ASSERT(self),DEE_ASSERT(DeeObject_Check(def));
 ACQUIRE;
 if ((node = AND_FUNC(_DeeHashMap_GetOrCreateNodeWithKnownBucket)(
  self,key,bucket LOCK_PAR)) == NULL) {
  RELEASE;
  return NULL;
 }
 if (!node->hmn_item) Dee_INCREF(node->hmn_item = def);
 Dee_INCREF(result = node->hmn_item);
 RELEASE;
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *AND_FUNC(DeeHashMap_SetDefaultStringWithKnownBucket)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node; DeeObject *result;
 DEE_ASSERT(self),DEE_ASSERT(DeeObject_Check(def));
 ACQUIRE;
 if ((node = AND_FUNC(_DeeHashMap_GetOrCreateNodeStringWithKnownBucket)(
  self,key,bucket LOCK_PAR)) == NULL) {
  RELEASE;
  return NULL;
 }
 if (!node->hmn_item) Dee_INCREF(node->hmn_item = def);
 Dee_INCREF(result = node->hmn_item);
 RELEASE;
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *FUNC(DeeHashMap_SetDefault)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT DeeObject *def LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 DEE_ASSERT(DeeObject_Check(key));
 if DEE_UNLIKELY(DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return NULL;
 return AND_FUNC(DeeHashMap_SetDefaultWithKnownBucket)(self,key,bucket,def LOCK_PAR);
}
DEE_A_RET_EXCEPT_REF DeeObject *FUNC(DeeHashMap_SetDefaultString)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key,
 DEE_A_INOUT DeeObject *def LOCK_ARG(DEE_A_INOUT)) {
 return AND_FUNC(DeeHashMap_SetDefaultStringWithKnownBucket)(
  self,key,DeeHash_String(key,DEE_HASH_START),def LOCK_PAR);
}

DEE_A_RET_EXCEPT_REF DeeObject *AND_FUNC(DeeHashMap_GetDefaultWithKnownBucket)(
 DEE_A_INOUT struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node; int temp; DeeObject *result;
 DEE_ASSERT(self),DEE_ASSERT(DeeObject_Check(def));
 ACQUIRE;
 temp = AND_FUNC(_DeeHashMap_GetNodeExWithKnownBucket)(self,key,bucket,&node LOCK_PAR);
 if DEE_UNLIKELY(temp < 0) { RELEASE; return NULL; }
 Dee_INCREF(result = temp == 1 ? def : node->hmn_item);
 RELEASE;
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *AND_FUNC(DeeHashMap_GetDefaultStringWithKnownBucket)(
 DEE_A_INOUT struct DeeHashMap const *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node; int temp; DeeObject *result;
 DEE_ASSERT(self),DEE_ASSERT(DeeObject_Check(def));
 ACQUIRE;
 temp = AND_FUNC(_DeeHashMap_GetNodeExStringWithKnownBucket)(self,key,bucket,&node LOCK_PAR);
 if (temp < 0) { RELEASE; return NULL; }
 Dee_INCREF(result = temp == 1 ? def : node->hmn_item);
 RELEASE;
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *FUNC(DeeHashMap_GetDefault)(
 DEE_A_INOUT struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT DeeObject *def LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 DEE_ASSERT(DeeObject_Check(key));
 if (DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return NULL;
 return AND_FUNC(DeeHashMap_GetDefaultWithKnownBucket)(self,key,bucket,def LOCK_PAR);
}
DEE_A_RET_EXCEPT_REF DeeObject *FUNC(DeeHashMap_GetDefaultString)(
 DEE_A_INOUT struct DeeHashMap const *self, DEE_A_IN_Z char const *key,
 DEE_A_INOUT DeeObject *def LOCK_ARG(DEE_A_INOUT)) {
 return AND_FUNC(DeeHashMap_GetDefaultStringWithKnownBucket)(
  self,key,DeeHash_String(key,DEE_HASH_START),def LOCK_PAR);
}



DEE_A_RET_EXCEPT(NULL) struct _DeeHashMapNode *FUNC(_DeeHashMap_GetOrCreateNode)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key LOCK_ARG(DEE_A_INOUT_LOCKED)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return NULL;
 return AND_FUNC(_DeeHashMap_GetOrCreateNodeWithKnownBucket)(self,key,bucket LOCK_PAR);
}
DEE_A_RET_EXCEPT(NULL) struct _DeeHashMapNode *FUNC(_DeeHashMap_GetOrCreateNodeString)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key LOCK_ARG(DEE_A_INOUT_LOCKED)) {
 return AND_FUNC(_DeeHashMap_GetOrCreateNodeStringWithKnownBucket)(
  self,key,DeeHash_String(key,DEE_HASH_START) LOCK_PAR);
}


#define ALLOC_MAP_NODE(key,bucket) \
 (struct _DeeHashMapNode *)mallocf_nz(sizeof(struct _DeeHashMapNode),\
 "HashMapNode (key = %p; bucket = %d)",key,(int)(bucket))
DEE_A_RET_EXCEPT(NULL) struct _DeeHashMapNode *AND_FUNC(_DeeHashMap_GetOrCreateNodeWithKnownBucket)(
 DEE_A_INOUT struct  DeeHashMap *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT_LOCKED)) {
 struct _DeeHashMapNode *result,**b; int temp;
 Dee_size_t new_bucket_count; DeeObject *node_key;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(key));
#ifdef DEE_DEBUG
 {Dee_hash_t hash_check;
  if (DeeObject_HashEx(key,DEE_HASH_START,&hash_check) == 0)
   DEE_ASSERT(bucket == hash_check);
  else DeeError_Handled();}
#endif
again:
 if (self->hm_buckets) {
  b = self->hm_buckets+(bucket % self->hm_bucket_count);
  // Check of "*b" contains a key equivalent to ours
  result = *b;
  while (result) {
   if (result->hmn_hash == bucket) {
    Dee_INCREF(node_key = result->hmn_key);
    RELEASE;
    temp = DeeObject_CompareEq(node_key,key);
    Dee_DECREF(node_key);
    if (temp < 0) { // Ignore not implemented errors
     if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return NULL;
     temp = 0;
    }
    ACQUIRE;
    if (b != self->hm_buckets+(bucket % self->hm_bucket_count)) goto again;
    // At this point we have confirmed 'b'
    // >> Now to make sure that 'node' is still reachable from there
    if (!_DeeHashMapNode_Reachable(*b,result)) goto again;
    // OK! Everything has worked out
    if (DEE_UNLIKELY(temp != 0)) return result; // This is the node we were looking for!
   }
   result = result->hmn_next;
  }
  // Key not found --> must allocate new node
  result = ALLOC_MAP_NODE(key,bucket);
  if (!result) {
no_mem:
   RELEASE;
   if DEE_LIKELY(Dee_CollectMemory()) { ACQUIRE; goto again; }
   DeeError_NoMemory();
   ACQUIRE;
   return NULL;
  }
  // NOTE: This is the complicated part, where we need to rehash
  //       everything if the load factory commands us to do so...
  new_bucket_count = _DeeDictCapacityOf(_DeeCeil(
   (++self->hm_map_size)*self->hm_max_load_factor));
  if (new_bucket_count > self->hm_bucket_count) {
   // Actually rehash the dict (This can raise a memory error)
   if (_DeeHashMap_Rehash(self,new_bucket_count) != 0) {
    free_nn(result);
    return NULL;
   }
   // Update the destination bucket
   b = self->hm_buckets+(bucket % self->hm_bucket_count);
  } else {
   if (b >= self->hm_valid_buckets_end) self->hm_valid_buckets_end = b+1; else
   if (b < self->hm_valid_buckets_begin) self->hm_valid_buckets_begin = b;
  }
  // noexcept from here on
  Dee_INCREF(result->hmn_key = key);
  result->hmn_hash = bucket;
  result->hmn_item = NULL; // new node
  if (*b) {
   // Insert the result node before "*b"
   if ((result->hmn_prev = (*b)->hmn_prev) != NULL) {
    result->hmn_prev->hmn_next = result;
   }
   ((*b)->hmn_prev = result)->hmn_next = (*b);
  } else {
   // First node in new bucket
   result->hmn_prev = NULL;
   result->hmn_next = NULL;
   if (b >= self->hm_valid_buckets_end) {
    self->hm_valid_buckets_end = b+1;
   } else if (b < self->hm_valid_buckets_begin) {
    self->hm_valid_buckets_begin = b;
   }
  }
  *b = result;
 } else {
  if ((result = ALLOC_MAP_NODE(key,bucket)) == NULL) goto no_mem;
#if DEE_HASHMAP_INITIAL_BUCKET_COUNT <= 1
  b = (struct _DeeHashMapNode **)mallocf_nz(sizeof(struct _DeeHashMapNode *),"Initial map buckets");
#else
  b = (struct _DeeHashMapNode **)callocf_nz(
   DEE_HASHMAP_INITIAL_BUCKET_COUNT,sizeof(struct _DeeHashMapNode *),"Initial map buckets");
#endif
  if (!b) { free_nn(result); goto no_mem; }
  // noexcept from here on
  self->hm_bucket_count = DEE_HASHMAP_INITIAL_BUCKET_COUNT;
  self->hm_map_size = 1;
  self->hm_buckets = b;
#if DEE_HASHMAP_INITIAL_BUCKET_COUNT > 1
  b += bucket % DEE_HASHMAP_INITIAL_BUCKET_COUNT;
#endif
  result->hmn_prev = NULL;
  result->hmn_next = NULL;
  result->hmn_hash = bucket;
  *b = result;
  self->hm_valid_buckets_begin = b;
  self->hm_valid_buckets_end = b+1;
  Dee_INCREF(result->hmn_key = key);
  result->hmn_item = NULL;
 }
 return result;
}
#undef ALLOC_MAP_NODE
#define ALLOC_MAP_NODE(key,bucket) \
 (struct _DeeHashMapNode *)mallocf_nz(sizeof(struct _DeeHashMapNode),\
 "HashMapNode (key = %s; bucket = %d)",key,(int)(bucket))
DEE_A_RET_EXCEPT(NULL) struct _DeeHashMapNode *AND_FUNC(_DeeHashMap_GetOrCreateNodeStringWithKnownBucket)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT_LOCKED)) {
 struct _DeeHashMapNode *result,**b; int temp;
 DeeObject *key_ob,*node_key;
 Dee_size_t new_bucket_count;
 DEE_ASSERT(self && key);
 DEE_ASSERT(bucket == DeeHash_String(key,DEE_HASH_START));
again:
 if (self->hm_buckets) {
  b = self->hm_buckets+(bucket % self->hm_bucket_count);
  // Check of "*b" contains a key equivalent to ours
  if ((temp = _DeeHashMapNodeFindKeyStringOnly(*b,key,bucket,&result)) < 0) return NULL;
  if (temp == 0) { DEE_ASSERT(result); return result; }
  RELEASE;
  key_ob = DeeString_New(key);
  ACQUIRE;
  if (!key_ob) return NULL;
  if (b != self->hm_buckets+(bucket % self->hm_bucket_count)) {
   RELEASE;
   Dee_DECREF(key_ob);
   ACQUIRE;
   goto again;
  }
  // Retry finding the key with a string object (unlikely to succeed)
  result = *b;
  while (result) {
   if (result->hmn_hash == bucket) {
    Dee_INCREF(node_key = result->hmn_key);
    RELEASE;
    temp = DeeObject_CompareEq(node_key,key_ob);
    Dee_DECREF(node_key);
    if (temp < 0) { // Ignore not implemented errors
     if (!DeeError_Catch(&DeeErrorType_NotImplemented)) {
      Dee_DECREF(key_ob);
      ACQUIRE;
      return NULL;
     }
     temp = 0;
    }
    ACQUIRE;
    if (b != self->hm_buckets+(bucket % self->hm_bucket_count)) goto again;
    // At this point we have confirmed 'b'
    // >> Now to make sure that 'node' is still reachable from there
    if (!_DeeHashMapNode_Reachable(*b,result)) goto again;
    // OK! Everything has worked out
    if (DEE_UNLIKELY(temp != 0)) {
     RELEASE;
     Dee_DECREF(key_ob);
     ACQUIRE;
     if (b != self->hm_buckets+(bucket % self->hm_bucket_count)) goto again;
     // At this point we have confirmed 'b'
     // >> Now to make sure that 'node' is still reachable from there
     if (!_DeeHashMapNode_Reachable(*b,result)) goto again;
     return result; // This is the node we were looking for!
    }
   }
   result = result->hmn_next;
  }
  // Key not found --> must allocate new node
  result = ALLOC_MAP_NODE(key,bucket);
  if (!result) {
no_mem_keyob:
   RELEASE;
   Dee_DECREF(key_ob);
   if DEE_LIKELY(Dee_CollectMemory()) { ACQUIRE; goto again; }
   DeeError_NoMemory();
   ACQUIRE;
   return NULL;
  }
  // NOTE: This is the complicated part, where we need to rehash
  //       everything if the load factory commands us to do so...
  new_bucket_count = _DeeDictCapacityOf(_DeeCeil(
   (++self->hm_map_size)*self->hm_max_load_factor));
  if (new_bucket_count > self->hm_bucket_count) {
   // Actually rehash the dict (This can raise a memory error)
   if (_DeeHashMap_Rehash(self,new_bucket_count) != 0) {
    RELEASE;
    Dee_DECREF(key_ob);
    free_nn(result);
    ACQUIRE;
    return NULL;
   }
   // Update the destination bucket
   b = self->hm_buckets+(bucket % self->hm_bucket_count);
  } else {
   if (b >= self->hm_valid_buckets_end) self->hm_valid_buckets_end = b+1; else
   if (b < self->hm_valid_buckets_begin) self->hm_valid_buckets_begin = b;
  }
  // noexcept from here on
  Dee_INHERIT_REF(result->hmn_key,key_ob);
  result->hmn_hash = bucket;
  result->hmn_item = NULL; // new node
  if (*b) {
   // Insert the result node before "*b"
   if ((result->hmn_prev = (*b)->hmn_prev) != NULL) {
    result->hmn_prev->hmn_next = result;
   }
   ((*b)->hmn_prev = result)->hmn_next = (*b);
  } else {
   // First node in new bucket
   result->hmn_prev = NULL;
   result->hmn_next = NULL;
   if (b >= self->hm_valid_buckets_end) {
    self->hm_valid_buckets_end = b+1;
   } else if (b < self->hm_valid_buckets_begin) {
    self->hm_valid_buckets_begin = b;
   }
  }
  *b = result;
 } else {
  RELEASE;
  key_ob = DeeString_New(key);
  ACQUIRE;
  if (!key_ob) return NULL;
  if (self->hm_buckets) {
   RELEASE;
   Dee_DECREF(key_ob);
   ACQUIRE;
   goto again;
  }
  result = ALLOC_MAP_NODE(key,bucket);
  if (!result) goto no_mem_keyob;
#if DEE_HASHMAP_INITIAL_BUCKET_COUNT <= 1
  b = (struct _DeeHashMapNode **)mallocf_nz(sizeof(struct _DeeHashMapNode *),"Initial map buckets");
#else
  b = (struct _DeeHashMapNode **)callocf_nz(
   DEE_HASHMAP_INITIAL_BUCKET_COUNT,sizeof(struct _DeeHashMapNode *),"Initial map buckets");
#endif
  if (!b) { free_nn(result); goto no_mem_keyob; }
  // noexcept from here on
  self->hm_bucket_count = DEE_HASHMAP_INITIAL_BUCKET_COUNT;
  self->hm_map_size = 1;
  self->hm_buckets = b;
#if DEE_HASHMAP_INITIAL_BUCKET_COUNT > 1
  b += bucket % DEE_HASHMAP_INITIAL_BUCKET_COUNT;
#endif
  result->hmn_prev = NULL;
  result->hmn_next = NULL;
  result->hmn_hash = bucket;
  *b = result;
  self->hm_valid_buckets_begin = b;
  self->hm_valid_buckets_end = b+1;
  Dee_INHERIT_REF(result->hmn_key,key_ob);
  result->hmn_item = NULL;
 }
 return result;
}
#undef ALLOC_MAP_NODE


DEE_A_RET_EXCEPT_FAIL(-1,1) int AND_FUNC(_DeeHashMap_GetNodeExWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_OUT struct _DeeHashMapNode **result LOCK_ARG(DEE_A_INOUT_LOCKED)) {
 int temp; struct _DeeHashMapNode **b,*begin;
#ifdef DEE_DEBUG
 {Dee_hash_t hash_check;
  if (DeeObject_HashEx(key,DEE_HASH_START,&hash_check) == 0)
   DEE_ASSERT(bucket == hash_check);
  else DeeError_Handled();}
#endif
again:
 if (DEE_UNLIKELY(!self->hm_buckets)) return 1;
 begin = *(b = self->hm_buckets+(bucket % self->hm_bucket_count));
 while (begin) {
  if (begin->hmn_hash == bucket) {
   RELEASE;
   temp = DeeObject_CompareEq(begin->hmn_key,key);
   if (temp < 0) { // Ignore not implemented errors
    if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return -1;
    temp = 0;
   }
   ACQUIRE;
   if (b != self->hm_buckets+(bucket % self->hm_bucket_count)) goto again;
   // At this point we have confirmed 'b'
   // >> Now to make sure that 'node' is still reachable from there
   if (!_DeeHashMapNode_Reachable(*b,begin)) goto again;
   // OK! Everything has worked out
   if (DEE_UNLIKELY(temp)) {
    *result = begin;
    return 0;
   }
  }
  begin = begin->hmn_next;
 }
 return 1;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int AND_FUNC(_DeeHashMap_GetNodeExStringWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_OUT struct _DeeHashMapNode **result LOCK_ARG(DEE_A_INOUT_LOCKED)) {
 int temp; struct _DeeHashMapNode **b,*begin; DeeObject *node_key,*key_ob;
 DEE_ASSERT(bucket == DeeHash_String(key,DEE_HASH_START));
again:
 if (DEE_UNLIKELY(!self->hm_buckets)) return 1;
 b = self->hm_buckets+(bucket % self->hm_bucket_count);
 // Check of "*b" contains a key equivalent to ours
 if ((temp = _DeeHashMapNodeFindKeyStringOnly(*b,key,bucket,result)) != 1) return temp;
 if ((key_ob = DeeString_New(key)) == NULL) return -1;
 // Retry finding the key with a string object (unlikely to succeed)
 begin = *b;
 while (begin) {
  if (begin->hmn_hash == bucket) {
   Dee_INCREF(node_key = begin->hmn_key);
   RELEASE;
   temp = DeeObject_CompareEq(node_key,key_ob);
   Dee_DECREF(node_key);
   if (temp < 0) { // Ignore not implemented errors
    if (!DeeError_Catch(&DeeErrorType_NotImplemented)) {
     Dee_DECREF(key_ob);
     ACQUIRE;
     return -1;
    }
    temp = 0;
   }
   ACQUIRE;
   if (b != self->hm_buckets+(bucket % self->hm_bucket_count)) goto again;
   // At this point we have confirmed 'b'
   // >> Now to make sure that 'node' is still reachable from there
   if (!_DeeHashMapNode_Reachable(*b,begin)) goto again;
   // OK! Everything has worked out
   if (DEE_UNLIKELY(temp != 0)) {
    RELEASE;
    Dee_DECREF(key_ob);
    ACQUIRE;
    if (b != self->hm_buckets+(bucket % self->hm_bucket_count)) goto again;
    // At this point we have confirmed 'b'
    // >> Now to make sure that 'node' is still reachable from there
    if (!_DeeHashMapNode_Reachable(*b,begin)) goto again;
    // OK! Everything has worked out
    *result = begin;
    return 0; // This is the node we were looking for!
   }
  }
  begin = begin->hmn_next;
 }
 RELEASE;
 Dee_DECREF(key_ob);
 ACQUIRE;
 return 1;
}



#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6101)
#endif
DEE_A_RET_EXCEPT_FAIL(-1,1) int FUNC(DeeHashMap_GetItemEx)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_OUT DEE_A_REF DeeObject **result LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return -1;
 return AND_FUNC(DeeHashMap_GetItemExWithKnownBucket)(self,key,bucket,result LOCK_PAR);
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int FUNC(DeeHashMap_GetItemExString)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key,
 DEE_A_OUT DEE_A_REF DeeObject **result LOCK_ARG(DEE_A_INOUT)) {
 return AND_FUNC(DeeHashMap_GetItemExStringWithKnownBucket)(
  self,key,DeeHash_String(key,DEE_HASH_START),result LOCK_PAR);
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int AND_FUNC(DeeHashMap_GetItemExWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_OUT DEE_A_REF DeeObject **result LOCK_ARG(DEE_A_INOUT)) {
 int temp; struct _DeeHashMapNode *node; DEE_ASSERT(result);
 ACQUIRE;
 temp = AND_FUNC(_DeeHashMap_GetNodeExWithKnownBucket)(self,key,bucket,&node LOCK_PAR);
 if (temp == 0) Dee_INCREF(*result = node->hmn_item);
 RELEASE;
 return temp;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int AND_FUNC(DeeHashMap_GetItemExStringWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_OUT DEE_A_REF DeeObject **result LOCK_ARG(DEE_A_INOUT)) {
 int temp; struct _DeeHashMapNode *node; DEE_ASSERT(result);
 ACQUIRE;
 temp = AND_FUNC(_DeeHashMap_GetNodeExStringWithKnownBucket)(self,key,bucket,&node LOCK_PAR);
 if (temp == 0) Dee_INCREF(*result = node->hmn_item);
 RELEASE;
 return temp;
}
#ifdef _PREFAST_
#pragma warning(pop)
#endif
DEE_COMPILER_MSVC_WARNING_PUSH(4701 4703)
DEE_A_RET_EXCEPT_REF DeeObject *FUNC(DeeHashMap_GetItem)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return NULL;
 return AND_FUNC(DeeHashMap_GetItemWithKnownBucket)(self,key,bucket LOCK_PAR);
}
DEE_A_RET_EXCEPT_REF DeeObject *FUNC(DeeHashMap_GetItemString)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key LOCK_ARG(DEE_A_INOUT)) {
 return AND_FUNC(DeeHashMap_GetItemStringWithKnownBucket)(
  self,key,DeeHash_String(key,DEE_HASH_START) LOCK_PAR);
}
DEE_A_RET_EXCEPT_REF DeeObject *AND_FUNC(DeeHashMap_GetItemWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT)) {
 int temp; struct _DeeHashMapNode *node; DeeObject *result;
 ACQUIRE;
 temp = AND_FUNC(_DeeHashMap_GetNodeExWithKnownBucket)(self,key,bucket,&node LOCK_PAR);
 if (temp != 0) {
  RELEASE;
  if (temp == 1) _deehashmap_error_not_found(key);
  return NULL;
 }
 DEE_ASSERT(node);
 Dee_INCREF(result = node->hmn_item);
 RELEASE;
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *AND_FUNC(DeeHashMap_GetItemStringWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT)) {
 int temp; struct _DeeHashMapNode *node; DeeObject *result;
 ACQUIRE;
 temp = AND_FUNC(_DeeHashMap_GetNodeExStringWithKnownBucket)(self,key,bucket,&node LOCK_PAR);
 if (temp != 0) {
  RELEASE;
  if (temp == 1) _deehashmap_error_not_found_s(key);
  return NULL;
 }
 DEE_ASSERT(node);
 Dee_INCREF(result = node->hmn_item);
 RELEASE;
 return result;
}
DEE_COMPILER_MSVC_WARNING_POP



DEE_A_RET_EXCEPT_FAIL(-1,0) int FUNC(DeeHashMap_HasKey)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return -1;
 return AND_FUNC(DeeHashMap_HasKeyWithKnownBucket)(self,key,bucket LOCK_PAR);
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int FUNC(DeeHashMap_HasKeyString)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key LOCK_ARG(DEE_A_INOUT)) {
 return AND_FUNC(DeeHashMap_HasKeyStringWithKnownBucket)(
  self,key,DeeHash_String(key,DEE_HASH_START) LOCK_PAR);
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int AND_FUNC(DeeHashMap_HasKeyWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node; int temp;
 ACQUIRE;
 temp = AND_FUNC(_DeeHashMap_GetNodeExWithKnownBucket)(self,key,bucket,&node LOCK_PAR);
 RELEASE;
 if (temp < 0) return -1;
 return temp == 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int AND_FUNC(DeeHashMap_HasKeyStringWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node; int temp;
 ACQUIRE;
 temp = AND_FUNC(_DeeHashMap_GetNodeExStringWithKnownBucket)(self,key,bucket,&node LOCK_PAR);
 RELEASE;
 if (temp < 0) return -1;
 return temp == 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int FUNC(DeeHashMap_HasItem)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 DeeHashMap_TRAVERSE_SAFE_VARS;
 struct _DeeHashMapNode *node; int temp; DeeObject *node_item;
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,self) {
  Dee_INCREF(node_item = node->hmn_item);
  RELEASE;
  temp = DeeObject_CompareEq(node_item,item);
  Dee_DECREF(node_item);
  if (temp < 0) {
   if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return -1;
  } else if (temp) return 1; // Found it!
  ACQUIRE;
 }
 RELEASE;
 return 0; // Not found...
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int FUNC(DeeHashMap_HasKeyItem)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return -1;
 return AND_FUNC(DeeHashMap_HasKeyItemWithKnownBucket)(self,key,bucket,item LOCK_PAR);
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int AND_FUNC(DeeHashMap_HasKeyItemWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node; int temp; DeeObject *found_item;
 ACQUIRE;
 temp = AND_FUNC(_DeeHashMap_GetNodeExWithKnownBucket)(self,key,bucket,&node LOCK_PAR);
 if (temp != 0) {
  RELEASE;
  return temp < 0 ? temp : 0; // Not found or error
 }
 DEE_ASSERT(node);
 Dee_INCREF(found_item = node->hmn_item);
 RELEASE;
 temp = DeeObject_CompareEq(found_item,item);
 Dee_DECREF(found_item);
 return temp;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int FUNC(DeeHashMap_HasKeyItemString)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key,
 DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 return AND_FUNC(DeeHashMap_HasKeyItemStringWithKnownBucket)(
  self,key,DeeHash_String(key,DEE_HASH_START),item LOCK_PAR);
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int AND_FUNC(DeeHashMap_HasKeyItemStringWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node; int temp; DeeObject *found_item;
 ACQUIRE;
 temp = AND_FUNC(_DeeHashMap_GetNodeExStringWithKnownBucket)(self,key,bucket,&node LOCK_PAR);
 if (temp != 0) {
  RELEASE;
  return temp < 0 ? temp : 0; // Not found or error
 }
 DEE_ASSERT(node);
 Dee_INCREF(found_item = node->hmn_item);
 RELEASE;
 temp = DeeObject_CompareEq(found_item,item);
 Dee_DECREF(found_item);
 return temp;
}




DEE_A_RET_EXCEPT_FAIL(-1,1) int FUNC(DeeHashMap_DelItem)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return -1;
 return AND_FUNC(DeeHashMap_DelItemWithKnownBucket)(self,key,bucket LOCK_PAR);
}
DEE_A_RET_EXCEPT(-1) int AND_FUNC(DeeHashMap_DelItemWithError)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return -1;
 return AND_FUNC(DeeHashMap_DelItemWithErrorAndKnownBucket)(self,key,bucket LOCK_PAR);
}
DEE_A_RET_EXCEPT(-1) int AND_FUNC(DeeHashMap_DelItemWithErrorAndKnownBucket)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT)) {
 int temp = AND_FUNC(DeeHashMap_DelItemWithKnownBucket)(self,key,bucket LOCK_PAR);
 if (temp == 1) _deehashmap_error_not_found(key),temp = -1;
 return temp;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int FUNC(DeeHashMap_DelItemString)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key LOCK_ARG(DEE_A_INOUT)) {
 return AND_FUNC(DeeHashMap_DelItemStringWithKnownBucket)(self,key,DeeHash_String(key,DEE_HASH_START) LOCK_PAR);
}
DEE_A_RET_EXCEPT(-1) int AND_FUNC(DeeHashMap_DelItemStringWithError)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key LOCK_ARG(DEE_A_INOUT)) {
 return AND_FUNC(DeeHashMap_DelItemStringWithErrorAndKnownBucket)(self,key,DeeHash_String(key,DEE_HASH_START) LOCK_PAR);
}
DEE_A_RET_EXCEPT(-1) int AND_FUNC(DeeHashMap_DelItemStringWithErrorAndKnownBucket)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT)) {
 int temp = AND_FUNC(DeeHashMap_DelItemStringWithKnownBucket)(self,key,bucket LOCK_PAR);
 if (temp == 1) _deehashmap_error_not_found_s(key),temp = -1;
 return temp;
}


DEE_A_RET_EXCEPT_FAIL(-1,1) int AND_FUNC(DeeHashMap_DelItemWithKnownBucket)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode **b,*node; int temp;
 Dee_size_t new_bucket_count; DeeObject *node_key;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(key));
#ifdef DEE_DEBUG
 {Dee_hash_t hash_check;
  if (DeeObject_HashEx(key,DEE_HASH_START,&hash_check) == 0)
   DEE_ASSERT(bucket == hash_check);
  else DeeError_Handled();}
#endif
 ACQUIRE;
again:
 if (!self->hm_buckets) return 1; // empty self doesn't contain anything!
 node = *(b = self->hm_buckets+(bucket % self->hm_bucket_count));
 while (node) {
  if (node->hmn_hash == bucket) {
   Dee_INCREF(node_key = node->hmn_key);
   RELEASE;
   temp = DeeObject_CompareEq(node_key,key);
   Dee_DECREF(node_key);
   if DEE_UNLIKELY(temp < 0) { // Ignore not_implemented errors here
    if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return -1;
    temp = 0;
   }
   ACQUIRE;
   if (b != self->hm_buckets+(bucket % self->hm_bucket_count)) goto again;
   // At this point we have confirmed 'b'
   // >> Now to make sure that 'node' is still reachable from there
   if (!_DeeHashMapNode_Reachable(*b,node)) goto again;
   // OK! Everything has worked out
   if DEE_UNLIKELY(temp != 0) { // found it!
    // Step #1: Remove the node
    DEE_ASSERT((node->hmn_prev == NULL) == (node == *b));
    if (node->hmn_prev) {
     node->hmn_prev->hmn_next = node->hmn_next;
    } else *b = node->hmn_next;
    if (node->hmn_next) node->hmn_next->hmn_prev = node->hmn_prev;
    // Step #2: Rehash the map, if necessary
    new_bucket_count = _DeeDictCapacityOf(_DeeCeil(
     (--self->hm_map_size)*self->hm_max_load_factor));
    if (new_bucket_count < self->hm_bucket_count) {
     // Actually rehash the self (This can raise a memory error)
     if (_DeeHashMap_Rehash(self,new_bucket_count) != 0) {
      // Must re-insert the node into "*b"
      node->hmn_prev = NULL;
      if ((node->hmn_next = *b) != NULL) node->hmn_next->hmn_prev = node;
      *b = node;
      RELEASE;
      return -1;
     }
     // NOTE: At this point, "b" was invalidated
    } else {
     DEE_ASSERT(self->hm_map_size && "This would have been detected by 'new_bucket_count < self->hm_bucket_count'");
     DEE_ASSERT(b == self->hm_buckets+(bucket % self->hm_bucket_count));
     if (!*b) {
      if (b == self->hm_valid_buckets_end-1) {
       while (!self->hm_valid_buckets_end[-1]) --self->hm_valid_buckets_end;
      } else if (b == self->hm_valid_buckets_begin) {
       while (!*self->hm_valid_buckets_begin) ++self->hm_valid_buckets_begin;
      }
     }
    }
    RELEASE;
    // Step #3: Free the node
    Dee_DECREF(node->hmn_key);
    Dee_DECREF(node->hmn_item);
    free_nn(node);
    return 0; // Done!
   }
  }
  node = node->hmn_next;
 }
 RELEASE;
 return 1;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int AND_FUNC(DeeHashMap_DelItemStringWithKnownBucket)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode **b,*node;
 DeeObject *key_ob; int temp;
 Dee_size_t new_bucket_count;
 DEE_ASSERT(self && key);
 DEE_ASSERT(bucket == DeeHash_String(key,DEE_HASH_START));
 ACQUIRE;
 if (!self->hm_buckets) {
  RELEASE; // empty self doesn't contain anything!
  return 1;
 }
 node = *(b = self->hm_buckets+(bucket % self->hm_bucket_count));
 while (node) {
  if (node->hmn_hash == bucket &&
      DeeString_Check(node->hmn_key) &&
      strcmp(DeeString_STR(node->hmn_key),key) == 0) { // found it!
   // Step #1: Remove the node
   DEE_ASSERT((node->hmn_prev == NULL) == (node == *b));
   if (node->hmn_prev) {
    node->hmn_prev->hmn_next = node->hmn_next;
   } else *b = node->hmn_next;
   if (node->hmn_next) node->hmn_next->hmn_prev = node->hmn_prev;
   // Step #2: Rehash the map, if necessary
   new_bucket_count = _DeeDictCapacityOf(_DeeCeil(
    (--self->hm_map_size)*self->hm_max_load_factor));
   if (new_bucket_count < self->hm_bucket_count) {
    // Actually rehash the self (This can raise a memory error)
    if (_DeeHashMap_Rehash(self,new_bucket_count) != 0) {
     // Must re-insert the node into "*b"
     node->hmn_prev = NULL;
     if ((node->hmn_next = *b) != NULL) node->hmn_next->hmn_prev = node;
     *b = node;
     RELEASE;
     return -1;
    }
    // NOTE: At this point, "b" was invalidated
   } else {
    DEE_ASSERT(self->hm_map_size && "This would have been detected by 'new_bucket_count < self->hm_bucket_count'");
    DEE_ASSERT(b == self->hm_buckets+(bucket % self->hm_bucket_count));
    if (!*b) {
     if (b == self->hm_valid_buckets_end-1) {
      while (!self->hm_valid_buckets_end[-1]) --self->hm_valid_buckets_end;
     } else if (b == self->hm_valid_buckets_begin) {
      while (!*self->hm_valid_buckets_begin) ++self->hm_valid_buckets_begin;
     }
    }
   }
   RELEASE;
   // Step #3: Free the node
   Dee_DECREF(node->hmn_key);
   Dee_DECREF(node->hmn_item);
   free_nn(node);
   return 0; // Done!
  }
  node = node->hmn_next;
 }
 RELEASE;
 // Search by object
 if ((key_ob = DeeString_New(key)) == NULL) return -1;
 temp = DeeHashMap_DelItemWithKnownBucket(self,key_ob,bucket);
 Dee_DECREF(key_ob);
 return temp;
}


DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashMap_SetItem)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return -1;
 return AND_FUNC(DeeHashMap_SetItemWithKnownBucket)(self,key,bucket,item LOCK_PAR);
}
DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashMap_SetItemString)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key,
 DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 return AND_FUNC(DeeHashMap_SetItemStringWithKnownBucket)(
  self,key,DeeHash_String(key,DEE_HASH_START),item LOCK_PAR);
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int FUNC(DeeHashMap_SetNewItem)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return -1;
 return AND_FUNC(DeeHashMap_SetNewItemWithKnownBucket)(self,key,bucket,item,old_item LOCK_PAR);
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int FUNC(DeeHashMap_SetNewItemString)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key,
 DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item LOCK_ARG(DEE_A_INOUT)) {
 return AND_FUNC(DeeHashMap_SetNewItemStringWithKnownBucket)(
  self,key,DeeHash_String(key,DEE_HASH_START),item,old_item LOCK_PAR);
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int FUNC(DeeHashMap_SetOldItem)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return -1;
 return AND_FUNC(DeeHashMap_SetOldItemWithKnownBucket)(self,key,bucket,item,old_item LOCK_PAR);
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int FUNC(DeeHashMap_SetOldItemString)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key,
 DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item LOCK_ARG(DEE_A_INOUT)) {
 return AND_FUNC(DeeHashMap_SetOldItemStringWithKnownBucket)(
  self,key,DeeHash_String(key,DEE_HASH_START),item,old_item LOCK_PAR);
}
DEE_A_RET_EXCEPT(-1) int AND_FUNC(DeeHashMap_SetItemWithKnownBucket)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node; DeeObject *old_item;
 DEE_ASSERT(key && DeeObject_Check(item));
 ACQUIRE;
 node = AND_FUNC(_DeeHashMap_GetOrCreateNodeWithKnownBucket)(self,key,bucket LOCK_PAR);
 if (!node) { RELEASE; return -1; }
 old_item = node->hmn_item;
 Dee_INCREF(node->hmn_item = item);
 RELEASE;
 Dee_XDECREF(old_item);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int AND_FUNC(DeeHashMap_SetItemStringWithKnownBucket)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node; DeeObject *old_item;
 DEE_ASSERT(key && DeeObject_Check(item));
 ACQUIRE;
 node = AND_FUNC(_DeeHashMap_GetOrCreateNodeStringWithKnownBucket)(self,key,bucket LOCK_PAR);
 if (!node) { RELEASE; return -1; }
 old_item = node->hmn_item;
 Dee_INCREF(node->hmn_item = item);
 RELEASE;
 Dee_XDECREF(old_item);
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int AND_FUNC(DeeHashMap_SetNewItemWithKnownBucket)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node;
 DEE_ASSERT(DeeObject_Check(key));
 DEE_ASSERT(DeeObject_Check(item));
 ACQUIRE;
 node = AND_FUNC(_DeeHashMap_GetOrCreateNodeWithKnownBucket)(self,key,bucket LOCK_PAR);
 if (!node) { RELEASE; return -1; }
 if (node->hmn_item) { if (old_item) Dee_INCREF(*old_item = node->hmn_item); RELEASE; return 1; }
 Dee_INCREF(node->hmn_item = item);
 RELEASE;
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int AND_FUNC(DeeHashMap_SetNewItemStringWithKnownBucket)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node;
 DEE_ASSERT(key && DeeObject_Check(item));
 ACQUIRE;
 node = AND_FUNC(_DeeHashMap_GetOrCreateNodeStringWithKnownBucket)(self,key,bucket LOCK_PAR);
 if (!node) { RELEASE; return -1; }
 if (node->hmn_item) { if (old_item) Dee_INCREF(*old_item = node->hmn_item); RELEASE; return 1; }
 Dee_INCREF(node->hmn_item = item);
 RELEASE;
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int AND_FUNC(DeeHashMap_SetOldItemWithKnownBucket)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node; int result; DeeObject *found_old_item;
 DEE_ASSERT(DeeObject_Check(key));
 DEE_ASSERT(DeeObject_Check(item));
 ACQUIRE;
 if ((result = AND_FUNC(_DeeHashMap_GetNodeExWithKnownBucket)(
  self,key,bucket,&node LOCK_PAR)) != 0) { RELEASE; return result; }
 DEE_ASSERT(node && node->hmn_item);
 Dee_INHERIT_REF(found_old_item,node->hmn_item);
 Dee_INCREF(node->hmn_item = item);
 RELEASE;
 if (old_item) Dee_INHERIT_REF(*old_item,found_old_item);
 else Dee_DECREF(found_old_item);
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int AND_FUNC(DeeHashMap_SetOldItemStringWithKnownBucket)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node; int result; DeeObject *found_old_item;
 DEE_ASSERT(DeeObject_Check(key));
 DEE_ASSERT(DeeObject_Check(item));
 ACQUIRE;
 if ((result = AND_FUNC(_DeeHashMap_GetNodeExStringWithKnownBucket)(
  self,key,bucket,&node LOCK_PAR)) != 0) { RELEASE; return result; }
 DEE_ASSERT(node && node->hmn_item);
 Dee_INHERIT_REF(found_old_item,node->hmn_item);
 Dee_INCREF(node->hmn_item = item);
 RELEASE;
 if (old_item) Dee_INHERIT_REF(*old_item,found_old_item);
 else Dee_DECREF(found_old_item);
 return 0;
}



DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t FUNC(DeeHashMap_FindKeyIndex)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return -1;
 return AND_FUNC(DeeHashMap_FindKeyIndexWithKnownBucket)(self,key,bucket LOCK_PAR);
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t FUNC(DeeHashMap_FindKeyIndexString)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key LOCK_ARG(DEE_A_INOUT)) {
 return AND_FUNC(DeeHashMap_FindKeyIndexStringWithKnownBucket)(
  self,key,DeeHash_String(key,DEE_HASH_START) LOCK_PAR);
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t FUNC(DeeHashMap_FindKeyItemIndex)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return -1;
 return AND_FUNC(DeeHashMap_FindKeyItemIndexWithKnownBucket)(self,key,bucket,item LOCK_PAR);
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t FUNC(DeeHashMap_FindKeyItemIndexString)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key,
 DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 return AND_FUNC(DeeHashMap_FindKeyItemIndexStringWithKnownBucket)(
  self,key,DeeHash_String(key,DEE_HASH_START),item LOCK_PAR);
}


DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t AND_FUNC(DeeHashMap_FindKeyIndexWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node_iter,*node,**buckets_begin,**buckets_end;
 Dee_size_t index; int temp;
 ACQUIRE;
 temp = AND_FUNC(_DeeHashMap_GetNodeExWithKnownBucket)(self,key,bucket,&node LOCK_PAR);
 if (temp != 0) { RELEASE; return temp < 0 ? -2 : -1; }
 buckets_end = (buckets_begin = self->hm_valid_buckets_begin)+(bucket % self->hm_bucket_count);
 index = 0; while (buckets_begin != buckets_end)
  for (node_iter = *buckets_begin++; node_iter; node_iter = node_iter->hmn_next) ++index;
 for (node_iter = *buckets_end; node_iter != node; node_iter = node_iter->hmn_next) ++index;
 RELEASE;
 return (Dee_ssize_t)index;
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t AND_FUNC(DeeHashMap_FindKeyIndexStringWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node_iter,*node,**buckets_begin,**buckets_end;
 Dee_size_t index; int temp;
 ACQUIRE;
 temp = AND_FUNC(_DeeHashMap_GetNodeExStringWithKnownBucket)(self,key,bucket,&node LOCK_PAR);
 if (temp != 0) { RELEASE; return temp < 0 ? -2 : -1; }
 buckets_end = (buckets_begin = self->hm_valid_buckets_begin)+(bucket % self->hm_bucket_count);
 index = 0; while (buckets_begin != buckets_end)
  for (node_iter = *buckets_begin++; node_iter; node_iter = node_iter->hmn_next) ++index;
 for (node_iter = *buckets_end; node_iter != node; node_iter = node_iter->hmn_next) ++index;
 RELEASE;
 return (Dee_ssize_t)index;
}

DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t AND_FUNC(DeeHashMap_FindKeyItemIndexWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node_iter,*node,**buckets_begin,**buckets_end;
 DeeObject *node_item; Dee_size_t index; int temp;
 ACQUIRE;
 temp = AND_FUNC(_DeeHashMap_GetNodeExWithKnownBucket)(self,key,bucket,&node LOCK_PAR);
 if (temp != 0) { RELEASE; return temp < 0 ? -2 : -1; }
 buckets_end = (buckets_begin = self->hm_valid_buckets_begin)+(bucket % self->hm_bucket_count);
 index = 0; while (buckets_begin != buckets_end)
  for (node_iter = *buckets_begin++; node_iter; node_iter = node_iter->hmn_next) ++index;
 for (node_iter = *buckets_end; node_iter != node; node_iter = node_iter->hmn_next) ++index;
 Dee_INCREF(node_item = node->hmn_item);
 RELEASE;
 temp = DeeObject_CompareEq(node_item,item);
 Dee_DECREF(node_item);
 return temp > 0 ? (Dee_ssize_t)index : temp < 0 ? -2 : -1;
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t AND_FUNC(DeeHashMap_FindKeyItemIndexStringWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node_iter,*node,**buckets_begin,**buckets_end;
 DeeObject *node_item; Dee_size_t index; int temp;
 ACQUIRE;
 temp = AND_FUNC(_DeeHashMap_GetNodeExStringWithKnownBucket)(self,key,bucket,&node LOCK_PAR);
 if (temp != 0) { RELEASE; return temp < 0 ? -2 : -1; }
 buckets_end = (buckets_begin = self->hm_valid_buckets_begin)+(bucket % self->hm_bucket_count);
 index = 0; while (buckets_begin != buckets_end)
  for (node_iter = *buckets_begin++; node_iter; node_iter = node_iter->hmn_next) ++index;
 for (node_iter = *buckets_end; node_iter != node; node_iter = node_iter->hmn_next) ++index;
 Dee_INCREF(node_item = node->hmn_item);
 RELEASE;
 temp = DeeObject_CompareEq(node_item,item);
 Dee_DECREF(node_item);
 return temp > 0 ? (Dee_ssize_t)index : temp < 0 ? -2 : -1;
}


DEE_A_RET_EXCEPT(-1) Dee_size_t FUNC(DeeHashMap_IndexKeyIndex)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return (Dee_size_t)-1;
 return AND_FUNC(DeeHashMap_IndexKeyIndexWithKnownBucket)(self,key,bucket LOCK_PAR);
}
DEE_A_RET_EXCEPT(-1) Dee_size_t FUNC(DeeHashMap_IndexKeyIndexString)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key LOCK_ARG(DEE_A_INOUT)) {
 return AND_FUNC(DeeHashMap_IndexKeyIndexStringWithKnownBucket)(
  self,key,DeeHash_String(key,DEE_HASH_START) LOCK_PAR);
}
DEE_A_RET_EXCEPT(-1) Dee_size_t FUNC(DeeHashMap_IndexKeyItemIndex)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 Dee_hash_t bucket;
 if (DeeObject_HashEx(key,DEE_HASH_START,&bucket) != 0) return (Dee_size_t)-1;
 return AND_FUNC(DeeHashMap_IndexKeyItemIndexWithKnownBucket)(self,key,bucket,item LOCK_PAR);
}
DEE_A_RET_EXCEPT(-1) Dee_size_t FUNC(DeeHashMap_IndexKeyItemIndexString)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key,
 DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 return AND_FUNC(DeeHashMap_IndexKeyItemIndexStringWithKnownBucket)(
  self,key,DeeHash_String(key,DEE_HASH_START),item LOCK_PAR);
}

DEE_A_RET_EXCEPT(-1) Dee_size_t AND_FUNC(DeeHashMap_IndexKeyIndexWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT)) {
 Dee_ssize_t result;
 if ((result = AND_FUNC(DeeHashMap_FindKeyIndexWithKnownBucket)(
  self,key,bucket LOCK_PAR)) == (Dee_ssize_t)-2) return (Dee_size_t)-1;
 if (result == (Dee_ssize_t)-1) { DeeError_ValueError_NotFound_T(&DeeDictKeys_Type,key); return (Dee_size_t)-1; }
 return (Dee_size_t)result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t AND_FUNC(DeeHashMap_IndexKeyIndexStringWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket LOCK_ARG(DEE_A_INOUT)) {
 Dee_ssize_t result;
 if ((result = AND_FUNC(DeeHashMap_FindKeyIndexStringWithKnownBucket)(
  self,key,bucket LOCK_PAR)) == (Dee_ssize_t)-2) return (Dee_size_t)-1;
 if (result == (Dee_ssize_t)-1) { DeeError_ValueError_NotFound_str_T(&DeeDictKeys_Type,key); return (Dee_size_t)-1; }
 return (Dee_size_t)result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t AND_FUNC(DeeHashMap_IndexKeyItemIndexWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 Dee_ssize_t result;
 if ((result = AND_FUNC(DeeHashMap_FindKeyItemIndexWithKnownBucket)(
  self,key,bucket,item LOCK_PAR)) == (Dee_ssize_t)-2) return (Dee_size_t)-1;
 if (result == (Dee_ssize_t)-1) {
  DeeError_SetStringf(&DeeErrorType_ValueError,"Element tuple:(%k,%k) not found in dict.values",key,item);
  return (Dee_size_t)-1;
 }
 return (Dee_size_t)result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t FUNC(DeeHashMap_IndexItemIndex)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 Dee_ssize_t result;
 if ((result = FUNC(DeeHashMap_FindItemIndex)(self,item LOCK_PAR)) == (Dee_ssize_t)-2) return (Dee_size_t)-1;
 if (result == (Dee_ssize_t)-1) { DeeError_ValueError_NotFound_T(&DeeDictItems_Type,item); return (Dee_size_t)-1; }
 return (Dee_size_t)result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t AND_FUNC(DeeHashMap_IndexKeyItemIndexStringWithKnownBucket)(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 Dee_ssize_t result;
 if ((result = AND_FUNC(DeeHashMap_FindKeyItemIndexStringWithKnownBucket)(
  self,key,bucket,item LOCK_PAR)) == (Dee_ssize_t)-2) return (Dee_size_t)-1;
 if (result == (Dee_ssize_t)-1) {
  DeeError_SetStringf(&DeeErrorType_ValueError,"Element tuple:(%s,%k) not found in dict.values",key,item);
  return (Dee_size_t)-1;
 }
 return (Dee_size_t)result;
}


DEE_A_RET_EXCEPT(-1) Dee_size_t FUNC(DeeHashMap_CountItem)(
 DEE_A_IN struct DeeHashMap const *self,
 DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node; DeeObject *node_item;
 DeeHashMap_TRAVERSE_SAFE_VARS;
 int temp; Dee_size_t result = 0;
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,self) {
  Dee_INCREF(node_item = node->hmn_item);
  RELEASE;
  temp = DeeObject_CompareEq(node_item,item);
  Dee_DECREF(node_item);
  if (temp < 0 && !DeeError_Catch(&DeeErrorType_NotImplemented)) return (Dee_size_t)-1;
  if (temp) ++result;
  ACQUIRE;
 }
 RELEASE;
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t FUNC(DeeHashMap_FindItemIndex)(
 DEE_A_IN struct DeeHashMap const *self,
 DEE_A_INOUT DeeObject *item LOCK_ARG(DEE_A_INOUT)) {
 struct _DeeHashMapNode *node; DeeObject *node_item;
 DeeHashMap_TRAVERSE_SAFE_VARS;
 int temp; Dee_size_t index = 0;
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,self) {
  Dee_INCREF(node_item = node->hmn_item);
  RELEASE;
  temp = DeeObject_CompareEq(node_item,item);
  Dee_DECREF(node_item);
  if (temp < 0 && !DeeError_Catch(&DeeErrorType_NotImplemented)) return (Dee_size_t)-2;
  if (temp) return (Dee_ssize_t)index;
  ++index;
  ACQUIRE;
 }
 RELEASE;
 return -1;
}


DEE_A_RET_EXCEPT_FAIL(-1,1) int FUNC(DeeHashMap_DelItemIndex)(
 DEE_A_INOUT struct DeeHashMap *self,
 DEE_A_IN Dee_ssize_t index LOCK_ARG(DEE_A_INOUT)) {
 // NOTE: This could be optimized
 int result; DeeObject *delete_key;
 ACQUIRE;
 DEE_PRIVATE_CLAMP_INDEX(index,DeeHashMap_SIZE(self));
 Dee_INCREF(delete_key = DeeHashMap_GetKeyIndex(self,(Dee_size_t)index));
 RELEASE;
 result = DeeHashMap_DelItem(self,delete_key);
 Dee_DECREF(delete_key);
 return result;
}
DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashMap_DelItemRangeIndex)(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN Dee_size_t lo,
 DEE_A_IN Dee_size_t hi LOCK_ARG(DEE_A_INOUT)) {
 int error; DeeObject *delete_key; Dee_size_t n;
 n = hi-lo; while (n--) {
  ACQUIRE;
  if (lo >= DeeHashMap_SIZE(self)) { RELEASE; break; }
  Dee_INCREF(delete_key = DeeHashMap_GetKeyIndex(self,lo));
  RELEASE;
  error = DeeHashMap_DelItem(self,delete_key);
  if (error == 1) {
   DeeError_KeyError_NotFound_T(&DeeDict_Type,delete_key);
   error = -1;
  }
  Dee_DECREF(delete_key);
  if (error < 0) return -1;
 }
 return 0;
}


DEE_COMPILER_PREFAST_WARNING_PUSH(6101)
DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashMap_HashKeys)(
 DEE_A_IN struct DeeHashMap const *self,
 DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result LOCK_ARG(DEE_A_INOUT)) {
 int error; DeeHashMap_TRAVERSE_SAFE_VARS;
 struct _DeeHashMapNode *node; DeeObject *key_ob;
 DEE_ASSERT(self && result);
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,self) {
  Dee_INCREF(key_ob = node->hmn_key);
  RELEASE;
  error = DeeObject_HashInplace(key_ob,start);
  Dee_DECREF(key_ob);
  if (error != 0) return error;
  ACQUIRE;
 }
 RELEASE;
 *result = start;
 return 0;
}
DEE_COMPILER_PREFAST_WARNING_POP

DEE_COMPILER_PREFAST_WARNING_PUSH(6101)
DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashMap_HashItems)(
 DEE_A_IN struct DeeHashMap const *self,
 DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result LOCK_ARG(DEE_A_INOUT)) {
 int error; DeeHashMap_TRAVERSE_SAFE_VARS;
 struct _DeeHashMapNode *node; DeeObject *item_ob;
 DEE_ASSERT(self && result);
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,self) {
  Dee_INCREF(item_ob = node->hmn_item);
  RELEASE;
  error = DeeObject_HashInplace(item_ob,start);
  Dee_DECREF(item_ob);
  if (error != 0) return error;
  ACQUIRE;
 }
 RELEASE;
 *result = start;
 return 0;
}
DEE_COMPILER_PREFAST_WARNING_POP

DEE_COMPILER_PREFAST_WARNING_PUSH(6101)
DEE_A_RET_EXCEPT(-1) int FUNC(DeeHashMap_HashValues)(
 DEE_A_IN struct DeeHashMap const *self,
 DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result LOCK_ARG(DEE_A_INOUT)) {
 int error; DeeHashMap_TRAVERSE_SAFE_VARS;
 struct _DeeHashMapNode *node; DeeObject *item_ob,*key_ob;
 ACQUIRE;
 DeeHashMap_TRAVERSE_SAFE(node,self) {
  Dee_INCREF(key_ob = node->hmn_key);
  Dee_INCREF(item_ob = node->hmn_item);
  RELEASE;
  error = DeeObject_HashInplace(item_ob,start);
  Dee_DECREF(item_ob);
  if (error != 0) { Dee_DECREF(key_ob); return error; }
  error = DeeObject_HashInplace(key_ob,start);
  Dee_DECREF(key_ob);
  if (error != 0) return error;
  ACQUIRE;
 }
 RELEASE;
 *result = start;
 return 0;
}
DEE_COMPILER_PREFAST_WARNING_POP


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
