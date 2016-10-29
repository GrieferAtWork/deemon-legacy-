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
#ifndef GUARD_DEEMON_DICT_C
#define GUARD_DEEMON_DICT_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/bool.h>
#include <deemon/deemonrun.h>
#include <deemon/dict.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/gc.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_visit.h>
#include <deemon/optional/hash.h>
#include <deemon/optional/type_util.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/list.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/string.h>
#include <deemon/super.h>
#include <deemon/tuple.h>
#include <deemon/value.h>
#include DEE_INCLUDE_MEMORY_API()

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>
#include <deemon/runtime/builtin_functions.h>

// */ (nano...)

DEE_DECL_BEGIN

///////////////////////////////////////////////////////////////////////////
// Ceils the given value to its most significant bit (+1)
// NOTE: "x" may not be 0
// >> 2 --> (ceil to 3) +1 --> 4
// >> 5 --> (ceil to 7) +1 --> 8
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_size_t)
_DeeDictCapacityOf(DEE_A_IN Dee_size_t x) {
 Dee_size_t iter;
 if (!x) return 0;
 iter = 1; while (1) {
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

// Returns true if 'node' is reachable from 'from[->hmn_next]...'
DEE_STATIC_INLINE(int) _DeeHashMapNode_Reachable(
 DEE_A_IN_OPT struct _DeeHashMapNode const *from,
 DEE_A_IN struct _DeeHashMapNode const *node) {
 while (from) {
  if (from == node) return 1;
  from = from->hmn_next;
 }
 return 0;
}



DEE_A_RET_WUNUSED int DeeHashMapIterator_CompareLo(
 DEE_A_IN struct DeeHashMapIterator const *lhs,
 DEE_A_IN struct DeeHashMapIterator const *rhs) {
 struct _DeeHashMapNode *p,*d;
 DEE_ASSERT(lhs && rhs);
 if (lhs->hmi_buckets_begin == rhs->hmi_buckets_begin) {
  p = lhs->hmi_hash_curr;
  d = rhs->hmi_hash_curr;
  if (p == d) return 0;
  if (!p) return 1;
  while (1) {
   if ((p = p->hmn_next) == NULL) break;
   if (p == d) return 1;
  }
  return 0;
 } else if (lhs->hmi_buckets_begin > rhs->hmi_buckets_begin) {
  return 0;
 } else {
  DEE_ASSERT(lhs->hmi_buckets_begin < rhs->hmi_buckets_begin);
  return 1;
 }
}
DEE_A_RET_WUNUSED int DeeHashMapIterator_CompareLe(
 DEE_A_IN struct DeeHashMapIterator const *lhs,
 DEE_A_IN struct DeeHashMapIterator const *rhs) {
 struct _DeeHashMapNode *p,*d;
 DEE_ASSERT(lhs && rhs);
 if (lhs->hmi_buckets_begin == rhs->hmi_buckets_begin) {
  p = lhs->hmi_hash_curr;
  d = rhs->hmi_hash_curr;
  if (!p) return d == NULL;
  while (1) {
   if (p == d) return 1;
   if ((p = p->hmn_next) == NULL) break;
  }
  return 0;
 } else if (lhs->hmi_buckets_begin > rhs->hmi_buckets_begin) {
  return 0;
 } else {
  DEE_ASSERT(lhs->hmi_buckets_begin < rhs->hmi_buckets_begin);
  return 1;
 }
}
DEE_A_RET_WUNUSED int DeeHashMapIterator_CompareEq(
 DEE_A_IN struct DeeHashMapIterator const *lhs,
 DEE_A_IN struct DeeHashMapIterator const *rhs) {
 DEE_ASSERT(lhs && rhs);
 return lhs->hmi_buckets_begin == rhs->hmi_buckets_begin &&
        lhs->hmi_hash_curr == rhs->hmi_hash_curr;
}


DEE_A_RET_EXCEPT(NULL) struct DeeHashMap *DeeHashMap_Alloc(void) {
 struct DeeHashMap *result;
 while ((result = (struct DeeHashMap *)
  malloc_nz(sizeof(struct DeeHashMap))
  ) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 return result;
}
DEE_A_RET_EXCEPT(NULL) struct DeeHashMap *DeeHashMap_New(void) {
 struct DeeHashMap *result;
 while ((result = (struct DeeHashMap *)
  malloc_nz(sizeof(struct DeeHashMap))
  ) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 DeeHashMap_Init(result);
 return result;
}
void DeeHashMap_Free(DEE_A_IN struct DeeHashMap *self) {
 DEE_ASSERT(self);
 free_nn(self);
}
void DeeHashMap_Delete(DEE_A_IN DEE_A_OUT_INVALID struct DeeHashMap *self) {
 DEE_ASSERT(self);
 DeeHashMap_Quit(self);
 free_nn(self);
}
void DeeHashMap_Init(DEE_A_OUT struct DeeHashMap *self) {
 DEE_ASSERT(self);
 self->hm_valid_buckets_begin = NULL;
 self->hm_valid_buckets_end = NULL;
 self->hm_buckets = NULL;
 self->hm_bucket_count = 0;
 self->hm_map_size = 0;
 self->hm_max_load_factor = 1.0;
}
void DeeHashMap_InitMove(
 DEE_A_OUT struct DeeHashMap *self,
 DEE_A_INOUT struct DeeHashMap *right) {
 DEE_ASSERT(self && right);
 self->hm_valid_buckets_begin = right->hm_valid_buckets_begin;
 self->hm_valid_buckets_end = right->hm_valid_buckets_end;
 self->hm_buckets = right->hm_buckets;
 self->hm_bucket_count = right->hm_bucket_count;
 self->hm_map_size = right->hm_map_size;
 self->hm_max_load_factor = right->hm_max_load_factor;
 right->hm_valid_buckets_begin = NULL;
 right->hm_valid_buckets_end = NULL;
 right->hm_buckets = NULL;
 right->hm_bucket_count = 0;
 right->hm_map_size = 0;
}

static void _deehashmap_itersize(DEE_A_IN_Z char const *sizetext) {
 DeeError_SetStringf(&DeeErrorType_ValueError,
                     "Expected 2-element sequence for hashmap element (got: %s)",
                     sizetext);
}

DEE_A_RET_EXCEPT(-1) int DeeHashMap_InitFromIterator(
 DEE_A_OUT struct DeeHashMap *self, DEE_A_INOUT DeeObject *iterator) {
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(iterator));
 DeeHashMap_Init(self);
 return DeeHashMap_InsertIterator(self,iterator);
}
DEE_A_RET_EXCEPT(-1) int DeeHashMap_TInitFromSequence(
 DEE_A_OUT struct DeeHashMap *self,
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) {
 if (tp_sequence == &DeeDict_Type) return DeeHashMap_InitCopyWithLock(
  self,DeeDict_MAP(sequence),DeeDict_LOCK(sequence));
 if (tp_sequence == &DeeSuper_Type) return DeeHashMap_TInitFromSequence(
  self,DeeSuper_TYPE(sequence),DeeSuper_SELF(sequence));
 DeeHashMap_Init(self);
 return DeeHashMap_TInsertSequence(self,tp_sequence,sequence);
}
DEE_A_RET_EXCEPT(-1) int DeeHashMap_InitFromKeyValuePairs(
 DEE_A_OUT struct DeeHashMap *self, DEE_A_IN Dee_size_t pairc,
 DEE_A_IN_R(pairc*2) DeeObject *const *pairv) {
 DeeObject *key,*item;
 DEE_ASSERT(self);
 DeeHashMap_Init(self);
 while (pairc--) {
  key = *pairv++,item = *pairv++;
  if (DeeHashMap_SetItem(self,key,item) != 0) {
   DeeHashMap_Quit(self);
   return -1;
  }
 }
 return 0;
}
void DeeHashMap_AssignMove(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_INOUT struct DeeHashMap *right) {
 DEE_ASSERTF(self != right,"Can't move a hashmap into itself");
 DeeHashMap_Quit(self);
 DeeHashMap_InitMove(self,right);
}
DEE_A_RET_EXCEPT(-1) int DeeHashMap_AssignCopy(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN struct DeeHashMap const *right) {
 struct DeeHashMap temp;
 if (DeeHashMap_InitCopy(&temp,right) != 0) return -1;
 DeeHashMap_Quit(self);
 memcpy(self,&temp,sizeof(struct DeeHashMap));
 return 0;
}

void DeeHashMap_Quit(DEE_A_IN DEE_A_OUT_INVALID struct DeeHashMap *self) {
 struct _DeeHashMapNode **buckets_begin,**buckets_end,*node,*next;
 buckets_begin = self->hm_valid_buckets_begin;
 buckets_end = self->hm_valid_buckets_end;
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
 free(self->hm_buckets);
}



DEE_A_RET_EXCEPT(NULL) struct DeeHashMapIterator *
DeeHashMapIterator_Alloc(void) {
 struct DeeHashMapIterator *result;
 while ((result = (struct DeeHashMapIterator *)
  malloc_nz(sizeof(struct DeeHashMapIterator))
  ) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 return result;
}
DEE_A_RET_EXCEPT(NULL) struct DeeHashMapIterator *
DeeHashMapIterator_New(DEE_A_IN struct DeeHashMap const *map) {
 struct DeeHashMapIterator *result;
 while ((result = (struct DeeHashMapIterator *)
  malloc_nz(sizeof(struct DeeHashMapIterator))
  ) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 DeeHashMapIterator_Init(result,map);
 return result;
}
void DeeHashMapIterator_Free(
 DEE_A_IN DEE_A_OUT_INVALID struct DeeHashMapIterator *self) {
 DEE_ASSERT(self);
 free_nn(self);
}
void DeeHashMapIterator_Init(
 DEE_A_OUT struct DeeHashMapIterator *self,
 DEE_A_IN struct DeeHashMap const *map) {
 DEE_ASSERT(self && map);
 self->hmi_hash_curr = (
  (self->hmi_buckets_begin = map->hm_valid_buckets_begin) !=
  (self->hmi_buckets_end = map->hm_valid_buckets_end))
  ? *self->hmi_buckets_begin : NULL;
}
void DeeHashMapIterator_InitCopy(
 DEE_A_OUT struct DeeHashMapIterator *self,
 DEE_A_IN struct DeeHashMapIterator const *right) {
 self->hmi_buckets_begin = right->hmi_buckets_begin;
 self->hmi_buckets_end = right->hmi_buckets_end;
 self->hmi_hash_curr = right->hmi_hash_curr;
}
DEE_A_RET_MAYBE_NULL struct _DeeHashMapNode *DeeHashMapIterator_Yield(
 DEE_A_INOUT struct DeeHashMapIterator *self, DEE_A_IN struct DeeHashMap const *map) {
 struct _DeeHashMapNode *result;
 DEE_ASSERT(self);
 if (self->hmi_buckets_begin < map->hm_valid_buckets_begin ||
     self->hmi_buckets_begin >= map->hm_valid_buckets_end ||
     !_DeeHashMapNode_Reachable(*self->hmi_buckets_begin,self->hmi_hash_curr)) {
  // TODO: Repair invalid iterator (somehow...)
  self->hmi_buckets_begin = NULL;
  self->hmi_buckets_end = NULL;
  self->hmi_hash_curr = NULL;
 }
 result = self->hmi_hash_curr;
 if (result) {
  DEE_ASSERT(self->hmi_buckets_begin < self->hmi_buckets_end);
  if ((self->hmi_hash_curr = self->hmi_hash_curr->hmn_next) == NULL) {
   struct _DeeHashMapNode **search,**search_end;
   /* Next bucket */
   search = self->hmi_buckets_begin;
   search_end = self->hmi_buckets_end;
   DEE_ASSERT(search),DEE_ASSERT(search_end);
   while (!(++search == search_end || *search));
   self->hmi_buckets_begin = search;
   if ((self->hmi_buckets_begin = search) != self->hmi_buckets_end)
    self->hmi_hash_curr = *self->hmi_buckets_begin;
  }
 }
 return result;
}


static void _deehashmap_error_not_found(DeeObject *key) {
 DeeError_SetStringf(&DeeErrorType_KeyError,"Key not found: %R",DeeObject_Str(key));
}
static void _deehashmap_error_not_found_s(char const *key) {
 DeeError_SetStringf(&DeeErrorType_KeyError,"Key not found: %q",key);
}









DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *DeeHashMap_GetValueIndex(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_size_t index) {
 struct _DeeHashMapNode *node = _DeeHashMap_GetNodeIndex(self,index);
 return DeeTuple_Pack(2,node->hmn_key,node->hmn_item);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeHashMap_GetKeyRangeIndex(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) {
 DeeHashMap_TRAVERSE_RANGE_VARS;
 struct _DeeHashMapNode *node;
 DeeObject *result,**result_iter;
 DEE_ASSERT(self && lo <= hi && hi <= DeeHashMap_SIZE(self));
 if ((result = _DeeList_NewUnsafe(hi-lo)) != NULL) {
  result_iter = DeeList_ELEM(result);
  DeeHashMap_TRAVERSE_RANGE(node,self,lo,hi)
   Dee_INCREF(*result_iter++ = node->hmn_key);
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeHashMap_GetItemRangeIndex(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) {
 DeeHashMap_TRAVERSE_RANGE_VARS;
 struct _DeeHashMapNode *node;
 DeeObject *result,**result_iter;
 DEE_ASSERT(self && lo <= hi && hi <= DeeHashMap_SIZE(self));
 if ((result = _DeeList_NewUnsafe(hi-lo)) != NULL) {
  result_iter = DeeList_ELEM(result);
  DeeHashMap_TRAVERSE_RANGE(node,self,lo,hi)
   Dee_INCREF(*result_iter++ = node->hmn_item);
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeHashMap_GetValueRangeIndex(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) {
 DeeHashMap_TRAVERSE_RANGE_VARS;
 struct _DeeHashMapNode *node;
 DeeObject *result,**result_iter;
 DEE_ASSERT(self && lo <= hi && hi <= DeeHashMap_SIZE(self));
 if ((result = _DeeList_NewUnsafe(hi-lo)) != NULL) {
  result_iter = DeeList_ELEM(result);
  DeeHashMap_TRAVERSE_RANGE(node,self,lo,hi) {
   DeeObject *temp = DeeTuple_Pack(2,node->hmn_key,node->hmn_item);
   if (DEE_UNLIKELY(!temp)) {
    while (result_iter != DeeList_ELEM(result))
     Dee_DECREF(*--result_iter);
    _DeeList_FreeUnsafe(result);
    return NULL;
   }
   Dee_INHERIT_REF(*result_iter++,temp);
  }
 }
 return result;
}
void DeeHashMap_SetItemIndex(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN Dee_size_t index, DEE_A_INOUT DeeObject *item) {
 struct _DeeHashMapNode *node;
 DEE_ASSERT(self && index <= DeeHashMap_SIZE(self));
 node = _DeeHashMap_GetNodeIndex(self,index);
 Dee_DECREF(node->hmn_item);
 Dee_INCREF(node->hmn_item = item);
}
DEE_A_RET_REF DeeObject *DeeHashMap_ExchangeItemIndex(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN Dee_size_t index,
 DEE_A_INOUT DeeObject *item) {
 struct _DeeHashMapNode *node; DeeObject *result;
 DEE_ASSERT(self && index <= DeeHashMap_SIZE(self));
 node = _DeeHashMap_GetNodeIndex(self,index);
 result = node->hmn_item;
 Dee_INCREF(node->hmn_item = item);
 return result;
}

DEE_A_RET_WUNUSED Dee_size_t DeeHashMap_Size(
 DEE_A_IN struct DeeHashMap const *self) {
 DEE_ASSERT(self);
 return DeeHashMap_SIZE(self);
}
DEE_A_RET_WUNUSED Dee_size_t DeeHashMap_BucketSize(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_size_t bucket) {
 struct _DeeHashMapNode const *node; Dee_size_t result = 0;
 DEE_ASSERT(self);
 DEE_ASSERT(bucket < DeeHashMap_BUCKET_COUNT(self));
 for (node = self->hm_buckets[bucket]; node; node = node->hmn_next) ++result;
 return result;
}
DEE_A_RET_WUNUSED Dee_size_t DeeHashMap_BucketCount(
 DEE_A_IN struct DeeHashMap const *self) {
 DEE_ASSERT(self);
 return DeeHashMap_BUCKET_COUNT(self);
}

DEE_A_RET_WUNUSED double DeeHashMap_GetMaxLoadFactor(
 DEE_A_IN struct DeeHashMap const *self) {
 DEE_ASSERT(self);
 return DeeHashMap_GET_MAX_LOAD_FACTOR(self);
}
void DeeHashMap_SetMaxLoadFactor(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN double v) {
 DEE_ASSERT(self);
 DeeHashMap_SET_MAX_LOAD_FACTOR(self,v);
}



void DeeHashMap_Front(
 DEE_A_IN struct DeeHashMap const *self,
 DEE_A_OUT_OPT DeeObject **key,
 DEE_A_OUT_OPT Dee_hash_t *bucket,
 DEE_A_OUT_OPT DeeObject **item) {
 struct _DeeHashMapNode *node;
 DEE_ASSERT(self);
 node = _DeeHashMap_FrontNode(self);
 DEE_ASSERT(node);
 if (key) *key = node->hmn_key;
 if (bucket) *bucket = node->hmn_hash;
 if (item) *item = node->hmn_item;
}
void DeeHashMap_Back(
 DEE_A_IN struct DeeHashMap const *self,
 DEE_A_OUT_OPT DeeObject **key,
 DEE_A_OUT_OPT Dee_hash_t *bucket,
 DEE_A_OUT_OPT DeeObject **item) {
 struct _DeeHashMapNode *node;
 DEE_ASSERT(self);
 node = _DeeHashMap_BackNode(self);
 DEE_ASSERT(node);
 if (key) *key = node->hmn_key;
 if (bucket) *bucket = node->hmn_hash;
 if (item) *item = node->hmn_item;
}






DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,1) int) _DeeHashMapNodeFindKeyStringOnly(
 DEE_A_IN_OPT struct _DeeHashMapNode *begin, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t hash, DEE_A_OUT struct _DeeHashMapNode **result) {
 struct _DeeHashMapNode *iter = begin;
 DEE_ASSERT(key);
 while (iter) {
  if (DeeString_Check(iter->hmn_key) && iter->hmn_hash == hash &&
      strcmp(DeeString_STR(iter->hmn_key),key) == 0) {
   *result = iter;
   return 0; // Found it!
  }
  iter = iter->hmn_next;
 }
 return 1;
}

//////////////////////////////////////////////////////////////////////////
// Update the valid bucket range of a dict
// >> NOTE: "self" must be non-empty
DEE_STATIC_INLINE(void) _DeeHashMap_UpdateValidBuckets(
 DEE_A_INOUT struct DeeHashMap *self) {
 struct _DeeHashMapNode **iter;
 DEE_ASSERT(self && self->hm_buckets);
 // Iterate to the first valid bucket
 iter = self->hm_buckets;
 while (!*iter) ++iter;
 self->hm_valid_buckets_begin = iter;
 // Iterator to the first invalid bucket
 iter = self->hm_buckets+self->hm_bucket_count;
 while (!*--iter);
 self->hm_valid_buckets_end = iter+1;
}

//////////////////////////////////////////////////////////////////////////
// Rehash a dict to use "new_bucket_count" buckets
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeHashMap_Rehash(
 DEE_A_INOUT struct DeeHashMap *self, DEE_A_IN Dee_size_t new_bucket_count) {
 struct _DeeHashMapNode **dst,**new_buckets,**old_buckets,**old_buckets_end,*iter,*next;
 Dee_hash_t new_hash,old_hash;
 DEE_ASSERT(self);
 if (!new_bucket_count) {
  DEE_ASSERT(!self->hm_map_size && "Can't rehash non-empty map to empty bucket size");
  if (self->hm_buckets) {
   free_nn(self->hm_buckets);
   self->hm_buckets = NULL;
   self->hm_valid_buckets_begin = NULL;
   self->hm_valid_buckets_end = NULL;
  } else {
   DEE_ASSERT(self->hm_valid_buckets_begin == NULL);
   DEE_ASSERT(self->hm_valid_buckets_end == NULL);
  }
  self->hm_bucket_count = new_bucket_count;
  return 0;
 }
 DEE_ASSERT(self->hm_buckets);
 if ((new_buckets = (struct _DeeHashMapNode **)callocf_nz(
  new_bucket_count,sizeof(struct _DeeHashMapNode *),
  "rehashed dict buckets (%u)",(unsigned)new_bucket_count)) == NULL) {
  // can't collect memory here... (locks and what not...)
  DeeError_NoMemory();
  return -1;
 }
 old_buckets = self->hm_valid_buckets_begin;
 old_buckets_end = self->hm_valid_buckets_end;
 // Actually start rehashing this shit
 while (1) {
  iter = *old_buckets;
  old_hash = (Dee_size_t)(old_buckets-self->hm_buckets);
  while (iter) {
   next = iter->hmn_next;
   new_hash = iter->hmn_hash % new_bucket_count;
   DEE_ASSERT(old_hash == iter->hmn_hash % self->hm_bucket_count);
#ifdef DEE_DEBUG
   if (_DeeFlag_Verbose && old_hash != new_hash) {
    DEE_LVERBOSE2("[dict::rehash] key = %O; item = %O; hash = %Iu/%Iu/%Iu\n",
                  iter->hmn_key,iter->hmn_item,old_hash,new_hash,iter->hmn_hash);
   }
#endif
   dst = new_buckets+new_hash;
   if (*dst == NULL) {
    iter->hmn_prev = NULL;
    iter->hmn_next = NULL;
   } else {
    if ((iter->hmn_prev = (*dst)->hmn_prev) != NULL)
     iter->hmn_prev->hmn_next = iter;
    ((*dst)->hmn_prev = iter)->hmn_next = *dst;
   }
   *dst = iter;
   iter = next;
  }
  if (++old_buckets == old_buckets_end) break;
 }
 // Exchange the bucket maps + update bucket count
 free_nn(self->hm_buckets);
 self->hm_buckets = new_buckets;
 self->hm_bucket_count = new_bucket_count;
 // Update valid buckets
 _DeeHashMap_UpdateValidBuckets(self);
 return 0;
}


DEE_A_RET_EXCEPT(-1) int DeeHashMap_PopSomething(
 DEE_A_INOUT struct DeeHashMap *self,
 DEE_A_REF DEE_A_OUT DeeObject **key,
 DEE_A_REF DEE_A_OUT DeeObject **item) {
 struct _DeeHashMapNode **b,*node;
 Dee_size_t new_bucket_count;
 DEE_ASSERT(self && key && item);
 if (!self->hm_buckets) { // empty self doesn't contain anything!
  DeeError_SET_STRING(&DeeErrorType_ValueError,"Empty mapping");
  return -1;
 }
 node = *(b = self->hm_buckets);
 DEE_ASSERT(node);
 // Step #1: Remove the node
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
   return -1;
  }
  // NOTE: At this point, "b" was invalidated
 } else {
  DEE_ASSERT(self->hm_map_size && "This would have been detected by 'new_bucket_count < self->hm_bucket_count'");
  DEE_ASSERT(b == self->hm_buckets);
  if (!*b) {
   if (b == self->hm_valid_buckets_end-1) {
    while (!self->hm_valid_buckets_end[-1]) --self->hm_valid_buckets_end;
   } else if (b == self->hm_valid_buckets_begin) {
    while (!*self->hm_valid_buckets_begin) ++self->hm_valid_buckets_begin;
   }
  }
 }
 // Step #3: Free the node
 Dee_INHERIT_REF(*key,node->hmn_key);
 Dee_INHERIT_REF(*item,node->hmn_item);
 free_nn(node);
 return 0;
}








DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL struct _DeeHashMapNode *_DeeHashMap_GetNodeIndex(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_size_t index) {
 struct _DeeHashMapNode **buckets,*result;
 DEE_ASSERT(self);
 DEE_ASSERT(index < DeeHashMap_SIZE(self));
 buckets = self->hm_valid_buckets_begin;
 DEE_ASSERT(buckets);
 result = *buckets; // Must be assigned (because "index" must exist)
 DEE_ASSERT(result);
 while (1) {
  if (!index--) break;
  // Goto the next node
  if ((result = result->hmn_next) == NULL)
   while ((result = *++buckets) == NULL)
    DEE_ASSERT(buckets != self->hm_valid_buckets_end);
 }
 return result;
}
DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL struct _DeeHashMapNode *
_DeeHashMap_FrontNode(DEE_A_IN struct DeeHashMap const *self) {
 DEE_ASSERT(self && DeeHashMap_SIZE(self));
 DEE_ASSERT(*self->hm_valid_buckets_begin);
 return *self->hm_valid_buckets_begin;
}
DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL struct _DeeHashMapNode *
_DeeHashMap_BackNode(DEE_A_IN struct DeeHashMap const *self) {
 struct _DeeHashMapNode **bucket,*result;
 DEE_ASSERT(self && DeeHashMap_SIZE(self));
 bucket = self->hm_valid_buckets_end-1;
 while ((result = *bucket) == 0)
  DEE_ASSERT(bucket != self->hm_valid_buckets_begin),--bucket;
 while (result->hmn_next) result = result->hmn_next;
 return result;
}



























































DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictObject) *DeeDict_New(void) {
 DeeDictObject *result;
 if ((result = DeeGC_MALLOC(DeeDictObject)) != NULL) {
  DeeObject_INIT(result,&DeeDict_Type);
  _DeeDict_InitCommon(result);
  DeeHashMap_Init(&result->d_map);
  DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictObject) *
DeeDict_NewFromIterator(DEE_A_INOUT DeeObject *iterator) {
 DeeDictObject *result;
 if ((result = DeeGC_MALLOC(DeeDictObject)) != NULL) {
  DeeObject_INIT(result,&DeeDict_Type);
  _DeeDict_InitCommon(result);
  if (DeeHashMap_InitFromIterator(&result->d_map,iterator) != 0) {
   _DeeObject_DELETE(&DeeDict_Type,result);
   result = NULL;
  } else {
   DeeGC_TrackedAdd((DeeObject *)result);
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictObject) *DeeDict_TNewFromSequence(
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) {
 DeeDictObject *result;
 if ((result = DeeGC_MALLOC(DeeDictObject)) != NULL) {
  DeeObject_INIT(result,&DeeDict_Type);
  _DeeDict_InitCommon(result);
  if (DeeHashMap_TInitFromSequence(&result->d_map,tp_sequence,sequence) != 0) {
   _DeeObject_DELETE(&DeeDict_Type,result);
   result = NULL;
  } else {
   DeeGC_TrackedAdd((DeeObject *)result);
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictObject) *DeeDict_Copy(
 DEE_A_IN_OBJECT(DeeDictObject) const *self) {
 DeeDictObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 if ((result = DeeGC_MALLOC(DeeDictObject)) != NULL) {
  DeeObject_INIT(result,&DeeDict_Type);
  _DeeDict_InitCommon(result);
  if (DeeHashMap_InitCopyWithLock(
   &result->d_map,DeeDict_MAP(self),DeeDict_LOCK(self)) != 0) {
   _DeeObject_DELETE(&DeeDict_Type,result);
   result = NULL;
  } else {
   DeeGC_TrackedAdd((DeeObject *)result);
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictObject) *DeeDict_DeepCopy(
 DEE_A_IN_OBJECT(DeeDictObject) const *self) {
 DeeDictObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 if ((result = DeeGC_MALLOC(DeeDictObject)) != NULL) {
  DeeObject_INIT(result,&DeeDict_Type);
  _DeeDict_InitCommon(result);
  if (DeeHashMap_InitDeepCopyWithLock(
   &result->d_map,DeeDict_MAP(self),DeeDict_LOCK(self)) != 0) {
   _DeeObject_DELETE(&DeeDict_Type,result);
   result = NULL;
  } else {
   DeeGC_TrackedAdd((DeeObject *)result);
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictObject) *DeeDict_NewFromKeyValuePairs(
 DEE_A_IN Dee_size_t pairc, DEE_A_IN_R(pairc*2) DeeObject *const *pairv) {
 DeeDictObject *result;
 if ((result = DeeGC_MALLOC(DeeDictObject)) != NULL) {
  DeeObject_INIT(result,&DeeDict_Type);
  _DeeDict_InitCommon(result);
  if (DeeHashMap_InitFromKeyValuePairs(&result->d_map,pairc,pairv) != 0) {
   _DeeObject_DELETE(&DeeDict_Type,result);
   result = NULL;
  } else {
   DeeGC_TrackedAdd((DeeObject *)result);
  }
 }
 return (DeeObject *)result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeDict_Str(
 DEE_A_IN_OBJECT(DeeDictObject) const *self) {
 DeeObject *repr_ob,*result; int temp,first = 1;
 DeeObject *node_key,*node_item;
 DeeHashMap_TRAVERSE_SAFE_VARS;
 struct _DeeHashMapNode *node;
 DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 if (DeeStringWriter_WRITE_STRING(&writer,"dict { ") != 0) goto err;
 DeeDict_ACQUIRE(self);
 DeeHashMap_TRAVERSE_SAFE(node,DeeDict_MAP(self)) {
  Dee_INCREF(node_key = node->hmn_key);
  Dee_INCREF(node_item = node->hmn_item);
  DeeDict_RELEASE(self);
  if (first)first = 0; else if (DeeStringWriter_WRITE_STRING(&writer,", ") != 0) {
err_ik: Dee_DECREF(node_item); Dee_DECREF(node_key); goto err;
  }
  if ((repr_ob = DeeObject_Str(node_key)) == NULL) goto err_ik;
  temp = DeeStringWriter_WriteObjectStr(&writer,repr_ob);
  Dee_DECREF(repr_ob);
  if (temp != 0) goto err_ik;
  if (DeeStringWriter_WRITE_STRING(&writer,": ") != 0) goto err_ik;
  if ((repr_ob = DeeObject_Str(node_item)) == NULL) goto err_ik;
  temp = DeeStringWriter_WriteObjectStr(&writer,repr_ob);
  Dee_DECREF(repr_ob);
  if (temp != 0) goto err_ik;
  Dee_DECREF(node_item);
  Dee_DECREF(node_key);
  DeeDict_ACQUIRE(self);
 }
 DeeDict_RELEASE(self);
 if (DeeStringWriter_WRITE_STRING(&writer," }") != 0) goto err;
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeDict_Repr(
 DEE_A_IN_OBJECT(DeeDictObject) const *self) {
 DeeObject *repr_ob,*result; int temp,first = 1;
 DeeObject *node_key,*node_item;
 DeeHashMap_TRAVERSE_SAFE_VARS;
 struct _DeeHashMapNode *node;
 DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 if (DeeStringWriter_WRITE_STRING(&writer,"dict { ") != 0) goto err;
 DeeDict_ACQUIRE(self);
 DeeHashMap_TRAVERSE_SAFE(node,DeeDict_MAP(self)) {
  Dee_INCREF(node_key = node->hmn_key);
  Dee_INCREF(node_item = node->hmn_item);
  DeeDict_RELEASE(self);
  if (first)first = 0; else if (DeeStringWriter_WRITE_STRING(&writer,", ") != 0) {
err_ik: Dee_DECREF(node_item); Dee_DECREF(node_key); goto err;
  }
  if ((repr_ob = DeeObject_Repr(node_key)) == NULL) goto err_ik;
  temp = DeeStringWriter_WriteObjectStr(&writer,repr_ob);
  Dee_DECREF(repr_ob);
  if (temp != 0) goto err_ik;
  if (DeeStringWriter_WRITE_STRING(&writer,": ") != 0) goto err_ik;
  if ((repr_ob = DeeObject_Repr(node_item)) == NULL) goto err_ik;
  temp = DeeStringWriter_WriteObjectStr(&writer,repr_ob);
  Dee_DECREF(repr_ob);
  if (temp != 0) goto err_ik;
  Dee_DECREF(node_item);
  Dee_DECREF(node_key);
  DeeDict_ACQUIRE(self);
 }
 DeeDict_RELEASE(self);
 if (DeeStringWriter_WRITE_STRING(&writer," }") != 0) goto err;
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}


DEE_A_RET_OBJECT(DeeDictObject) *DeeDictProxy_Dict(
 DEE_A_IN_OBJECT(DeeDictProxyObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDictProxy_Check(self));
 return DeeDictProxy_DICT(self);
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictProxyObject) *
DeeDict_Keys(DEE_A_INOUT_OBJECT(DeeDictObject) const *self) {
 DeeDictProxyObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 result = DeeObject_MALLOCF(DeeDictProxyObject,"dict.keys for %p",self);
 if (result) {
  DeeObject_INIT(result,&DeeDictKeys_Type);
  Dee_INCREF(result->dp_dict = (DeeDictObject *)self);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictProxyObject) *
DeeDict_Items(DEE_A_INOUT_OBJECT(DeeDictObject) const *self) {
 DeeDictProxyObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 result = DeeObject_MALLOCF(DeeDictProxyObject,"dict.items for %p",self);
 if (result) {
  DeeObject_INIT(result,&DeeDictItems_Type);
  Dee_INCREF(result->dp_dict = (DeeDictObject *)self);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictProxyObject) *
DeeDict_Values(DEE_A_INOUT_OBJECT(DeeDictObject) const *self) {
 DeeDictProxyObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 result = DeeObject_MALLOCF(DeeDictProxyObject,"dict.values for %p",self);
 if (result) {
  DeeObject_INIT(result,&DeeDictValues_Type);
  Dee_INCREF(result->dp_dict = (DeeDictObject *)self);
 }
 return (DeeObject *)result;
}





DEE_STATIC_INLINE(DeeDictIteratorObject *) _DeeDictIterator_New(
 DEE_A_IN DeeTypeObject *iter_type, DEE_A_IN DeeDictObject *dict) {
 DeeDictIteratorObject *result;
 DEE_ASSERT(DeeObject_Check(iter_type) && DeeType_Check(iter_type));
 DEE_ASSERT(DeeObject_Check(dict) && DeeDict_Check(dict));
 if ((result = DeeObject_MALLOCF(
  DeeDictIteratorObject,"%s.iterator for %p",DeeType_NAME(iter_type),dict)) != NULL) {
  DeeObject_INIT(result,iter_type);
  Dee_INCREF(result->di_dict = dict);
  DeeDict_ACQUIRE(dict);
  DeeHashMapIterator_Init(&result->di_iter,&dict->d_map);
  DeeDict_RELEASE(dict);
 }
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictIteratorObject) *DeeDict_IterSelf(
 DEE_A_INOUT_OBJECT(DeeDictObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return (DeeObject *)_DeeDictIterator_New(&DeeDictIterator_Type,(DeeDictObject *)self);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictIteratorObject) *DeeDictKeys_IterSelf(
 DEE_A_IN_OBJECT(DeeDictProxyObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDictKeys_Check(self));
 return (DeeObject *)_DeeDictIterator_New(&DeeDictKeysIterator_Type,(DeeDictObject *)DeeDictProxy_DICT(self));
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictIteratorObject) *DeeDictItems_IterSelf(
 DEE_A_IN_OBJECT(DeeDictProxyObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDictItems_Check(self));
 return (DeeObject *)_DeeDictIterator_New(&DeeDictItemsIterator_Type,(DeeDictObject *)DeeDictProxy_DICT(self));
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictIteratorObject) *DeeDictValues_IterSelf(
 DEE_A_IN_OBJECT(DeeDictProxyObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDictValues_Check(self));
 return (DeeObject *)_DeeDictIterator_New(&DeeDictIterator_Type,(DeeDictObject *)DeeDictProxy_DICT(self));
}









//////////////////////////////////////////////////////////////////////////
// Virtual dict functions
static int _deedict_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeDictObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 _DeeDict_InitCommon(self);
 DeeHashMap_Init(&self->d_map);
 return 0;
}
static void _deedict_tp_dtor(DeeDictObject *self) {
 //DeeAtomicMutex_Quit(&self->d_lock);
 DeeHashMap_Quit(&self->d_map);
}
DEE_VISIT_PROC(_deedict_tp_visit,DeeDictObject *self) {
 DeeHashMap_TRAVERSE_VARS;
 struct _DeeHashMapNode *node;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 DeeHashMap_TRAVERSE(node,&self->d_map) {
  // NOTE: No need to unlock the map, as dicts are gc objects.
  //       Meaning that a visit call isn't allowed to be recursive
  //       (no deadlock as we should not be allowed to re-visit ourselves)
  Dee_VISIT(node->hmn_key);
  Dee_VISIT(node->hmn_item);
 }
 DeeDict_RELEASE(self);
}
static int _deedict_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeDictObject *self, DeeDictObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeDict_Check(right));
 _DeeDict_InitCommon(self);
 return DeeHashMap_InitCopyWithLock(&self->d_map,&right->d_map,DeeDict_LOCK(right));
}
static int _deedict_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeDictObject *self, DeeDictObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeDict_Check(right));
 DeeDict_ACQUIRE(right);
 DeeHashMap_InitMove(&self->d_map,&right->d_map);
 DeeDict_RELEASE(right);
 return 0;
}
static int _deedict_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeDictObject *self, DeeObject *args) {
 DeeObject *seq;
 if (DeeTuple_Unpack(args,"o:dict",&seq) != 0) return -1;
 _DeeDict_InitCommon(self);
 return DeeHashMap_InitFromSequence(&self->d_map,seq);
}
static int _deedict_tp_copy_assign(DeeDictObject *self, DeeDictObject *right) {
 if (self != right) {
  struct DeeHashMap fake_map = DeeHashMap_INIT();
  fake_map.hm_max_load_factor = right->d_map.hm_max_load_factor;
  // v we copy-initialize a fake, local dict so we don't
  //   fail and have already cleared the original dict
  if (DeeHashMap_InitCopyWithLock(&fake_map,&right->d_map,DeeDict_LOCK(right)) != 0) return -1;
  DeeDict_ACQUIRE(self);
  DeeHashMap_Quit(&self->d_map);
  self->d_map = fake_map;
  DeeDict_RELEASE(self);
 }
 return 0;
}
static int _deedict_tp_move_assign(DeeDictObject *self, DeeDictObject *right) {
 if (self != right) {
  struct DeeHashMap temp_map;
  DeeDict_ACQUIRE(right);
  temp_map = right->d_map;
  right->d_map.hm_valid_buckets_begin = NULL;
  right->d_map.hm_valid_buckets_end = NULL;
  right->d_map.hm_buckets = NULL;
  right->d_map.hm_bucket_count = 0;
  right->d_map.hm_map_size = 0;
  DeeDict_RELEASE(right);
  DeeDict_ACQUIRE(self);
  DeeHashMap_Quit(&self->d_map);
  self->d_map = temp_map;
  DeeDict_RELEASE(self);
 }
 return 0;
}
static int _deedict_tp_any_assign(DeeDictObject *self, DeeObject *right) {
 DEE_ASSERT((DeeObject *)self != right);
 {// Same deal as with the regular copy assign
  struct DeeHashMap fake_map = DeeHashMap_INIT();
  fake_map.hm_max_load_factor = self->d_map.hm_max_load_factor;
  // v we copy-initialize a fake, local dict so we don't
  //   fail and have already cleared the original dict
  if (DeeHashMap_InitFromSequence(&fake_map,right) != 0) return -1;
  DeeDict_ACQUIRE(self);
  DeeHashMap_Quit(&self->d_map);
  self->d_map = fake_map;
  DeeDict_RELEASE(self);
 }
 return 0;
}
static int _deedict_tp_hash(DeeDictObject *self, Dee_hash_t start, Dee_hash_t *result) {
 return DeeHashMap_HashValuesWithLock(&self->d_map,start,result,&self->d_lock);
}
static DeeObject *_deedict_tp_not(DeeDictObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeReturn_Bool(DeeDict_Size((DeeObject *)self) == 0);
}
static int _deedict_tp_bool(DeeDictObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeDict_Size((DeeObject *)self) != 0;
}
static DeeObject *_deedict_tp_seq_size(DeeDictObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeObject_New(Dee_size_t,DeeDict_Size((DeeObject *)self));
}
static DeeObject *_deedict_tp_seq_contains(DeeDictObject *self, DeeObject *key) {
 int temp = DeeDict_HasKey((DeeObject *)self,key);
 if (temp < 0) return NULL;
 DeeReturn_Bool(temp);
}








//////////////////////////////////////////////////////////////////////////
// Virtual Dict Proxy functions
static void _deedictproxy_tp_dtor(DeeDictProxyObject *self) {
 Dee_DECREF(self->dp_dict);
}
DEE_VISIT_PROC(_deedictproxy_tp_visit,DeeDictProxyObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDictProxy_Check(self));
 Dee_VISIT(self->dp_dict);
}
static int _deedictproxy_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeDictProxyObject *self, DeeDictProxyObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDictProxy_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeDictProxy_Check(right));
 Dee_INCREF(self->dp_dict = right->dp_dict);
 return 0;
}
static int _deedictproxy_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeDictProxyObject *self, DeeObject *args) {
 DeeDictObject *right;
 DEE_ASSERT(DeeObject_Check(self) && DeeDictProxy_Check(self));
 if (DeeTuple_Unpack(args,"o:__dictproxy__",&right) != 0) return -1;
 if (DeeObject_InplaceGetInstance(&right,&DeeDict_Type) != 0) return -1;
 Dee_INCREF(self->dp_dict = right);
 return 0;
}
static int _deedictproxy_tp_bool(DeeDictProxyObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDictProxy_Check(self));
 return DeeDict_Size((DeeObject *)self->dp_dict) != 0;
}
static DeeObject *_deedictproxy_tp_not(DeeDictProxyObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDictProxy_Check(self));
 DeeReturn_Bool(DeeDict_Size((DeeObject *)self->dp_dict) == 0);
}
static int _deedictkeys_tp_hash(DeeDictProxyObject *self, Dee_hash_t start, Dee_hash_t *result) {
 return DeeHashMap_HashKeysWithLock(&self->dp_dict->d_map,start,result,&self->dp_dict->d_lock);
}
static int _deedictitems_tp_hash(DeeDictProxyObject *self, Dee_hash_t start, Dee_hash_t *result) {
 return DeeHashMap_HashItemsWithLock(&self->dp_dict->d_map,start,result,&self->dp_dict->d_lock);
}
static int _deedictvalues_tp_hash(DeeDictProxyObject *self, Dee_hash_t start, Dee_hash_t *result) {
 return DeeHashMap_HashValuesWithLock(&self->dp_dict->d_map,start,result,&self->dp_dict->d_lock);
}
static DeeObject *_deedictproxy_tp_seq_size(DeeDictProxyObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDictProxy_Check(self));
 return DeeObject_New(Dee_size_t,DeeDict_Size((DeeObject *)self->dp_dict));
}
static int _deedictproxy_tp_seq_del(
 DeeDictProxyObject *self, DeeObject *i) {
 Dee_ssize_t ii;
 DEE_ASSERT(DeeObject_Check(self) && DeeDictProxy_Check(self));
 if (DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return -1;
 return DeeDict_DelItemIndex((DeeObject *)self->dp_dict,ii);
}
static int _deedictproxy_tp_seq_range_del(
 DeeDictProxyObject *self, DeeObject *lo, DeeObject *hi) {
 Dee_ssize_t loi,hii;
 DEE_ASSERT(DeeObject_Check(self) && DeeDictProxy_Check(self));
 if (DeeObject_Cast(Dee_ssize_t,lo,&loi) != 0) return -1;
 if (DeeObject_Cast(Dee_ssize_t,hi,&hii) != 0) return -1;
 if (loi < 0) loi = 0; if (hii < 0) hii = 0;
 return loi < hii ? DeeDict_DelItemRangeIndex((DeeObject *)self->dp_dict,(Dee_size_t)loi,(Dee_size_t)hii) : 0;
}
static DeeObject *_deedictkeys_tp_seq_get(
 DeeDictProxyObject *self, DeeObject *i) {
 Dee_ssize_t ii;
 DEE_ASSERT(DeeObject_Check(self) && DeeDictKeys_Check(self));
 if (DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return NULL;
 return DeeDict_GetKeyIndex((DeeObject *)self->dp_dict,ii);
}
static DeeObject *_deedictitems_tp_seq_get(
 DeeDictProxyObject *self, DeeObject *i) {
 Dee_ssize_t ii;
 DEE_ASSERT(DeeObject_Check(self) && DeeDictItems_Check(self));
 if (DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return NULL;
 return DeeDict_GetItemIndex((DeeObject *)self->dp_dict,ii);
}
static DeeObject *_deedictvalues_tp_seq_get(
 DeeDictProxyObject *self, DeeObject *i) {
 Dee_ssize_t ii;
 DEE_ASSERT(DeeObject_Check(self) && DeeDictValues_Check(self));
 if (DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return NULL;
 return DeeDict_GetValueIndex((DeeObject *)self->dp_dict,ii);
}
static DeeObject *_deedictkeys_tp_seq_range_get(
 DeeDictProxyObject *self, DeeObject *lo, DeeObject *hi) {
 Dee_ssize_t loi,hii;
 DEE_ASSERT(DeeObject_Check(self) && DeeDictKeys_Check(self));
 if (DeeObject_Cast(Dee_ssize_t,lo,&loi) != 0) return NULL;
 if (DeeObject_Cast(Dee_ssize_t,hi,&hii) != 0) return NULL;
 return DeeDict_GetKeyRangeIndex((DeeObject *)self->dp_dict,loi,hii);
}
static DeeObject *_deedictitems_tp_seq_range_get(
 DeeDictProxyObject *self, DeeObject *lo, DeeObject *hi) {
 Dee_ssize_t loi,hii;
 DEE_ASSERT(DeeObject_Check(self) && DeeDictItems_Check(self));
 if (DeeObject_Cast(Dee_ssize_t,lo,&loi) != 0) return NULL;
 if (DeeObject_Cast(Dee_ssize_t,hi,&hii) != 0) return NULL;
 return DeeDict_GetItemRangeIndex((DeeObject *)self->dp_dict,loi,hii);
}
static DeeObject *_deedictvalues_tp_seq_range_get(
 DeeDictProxyObject *self, DeeObject *lo, DeeObject *hi) {
 Dee_ssize_t loi,hii;
 DEE_ASSERT(DeeObject_Check(self) && DeeDictValues_Check(self));
 if (DeeObject_Cast(Dee_ssize_t,lo,&loi) != 0) return NULL;
 if (DeeObject_Cast(Dee_ssize_t,hi,&hii) != 0) return NULL;
 return DeeDict_GetValueRangeIndex((DeeObject *)self->dp_dict,loi,hii);
}
static int _deedictitems_tp_seq_set(
 DeeDictProxyObject *self, DeeObject *i, DeeObject *v) {
 Dee_ssize_t ii;
 DEE_ASSERT(DeeObject_Check(self) && DeeDictItems_Check(self));
 if (DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return -1;
 return DeeDict_SetItemIndex((DeeObject *)self->dp_dict,ii,v);
}
static DeeObject *_deedictkeys_tp_seq_contains(
 DeeDictProxyObject *self, DeeObject *key) {
 int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeDictKeys_Check(self));
 if ((temp = DeeDict_HasKey((DeeObject *)self->dp_dict,key)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *_deedictitems_tp_seq_contains(
 DeeDictProxyObject *self, DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDictItems_Check(self));
 DeeReturn_Bool(DeeDict_HasItem((DeeObject *)self->dp_dict,item));
}
static DeeObject *_deedictvalues_tp_seq_contains(
 DeeDictProxyObject *self, DeeObject *key_item) {
 int temp;
 if (!DeeTuple_Check(key_item) || DeeTuple_SIZE(key_item) != 2)
  return DeeType_GET_SLOT(&DeeObject_Type,tp_seq_contains)((DeeObject *)self,key_item);
 temp = DeeDict_HasKeyItem((DeeObject *)self->dp_dict,
  DeeTuple_GET(key_item,0),DeeTuple_GET(key_item,1));
 if (temp < 0) return NULL;
 DeeReturn_Bool(temp);
}




//////////////////////////////////////////////////////////////////////////
// Virtual Dict Iterator functions
static void _deedictiterator_tp_dtor(DeeDictIteratorObject *self) {
 DeeDict_ACQUIRE(self->di_dict);
 DeeHashMapIterator_Quit(&self->di_iter);
 DeeDict_RELEASE(self->di_dict);
 Dee_DECREF(self->di_dict);
}
static int _deedictiterator_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeDictIteratorObject *self, DeeDictIteratorObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDictAnyIterator_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeDictAnyIterator_Check(right));
 Dee_INCREF(self->di_dict = right->di_dict);
 DeeDict_ACQUIRE(self->di_dict);
 DeeHashMapIterator_InitCopy(&self->di_iter,&right->di_iter);
 DeeDict_RELEASE(self->di_dict);
 return 0;
}
DEE_VISIT_PROC(_deedictiterator_tp_visit,DeeDictIteratorObject *self) {
 Dee_VISIT(self->di_dict);
}
static int _deedictiterator_tp_copy_assign(
 DeeDictIteratorObject *self, DeeDictIteratorObject *right) {
 if (self != right) {
  _deedictiterator_tp_dtor(self);
  _deedictiterator_tp_copy_ctor(Dee_TYPE(self),self,right);
 }
 return 0;
}

static DeeObject *_deedictiterator_tp_cmp_lo(
 DeeDictIteratorObject *lhs, DeeDictIteratorObject *rhs) {
 int result;
 DEE_ASSERT(DeeObject_Check(lhs) && DeeDictAnyIterator_Check(lhs));
 DEE_ASSERT(DeeObject_Check(rhs));
 if (DeeObject_InplaceGetInstance(&rhs,Dee_TYPE(lhs)) != 0) return NULL;
 if (lhs->di_dict != rhs->di_dict) DeeReturn_Bool(lhs->di_dict < rhs->di_dict);
 DeeDict_ACQUIRE(lhs->di_dict);
 result = DeeDictIterator_CompareLo((DeeObject *)lhs,(DeeObject *)rhs);
 DeeDict_RELEASE(lhs->di_dict);
 DeeReturn_Bool(result);
}
static DeeObject *_deedictiterator_tp_cmp_le(
 DeeDictIteratorObject *lhs, DeeDictIteratorObject *rhs) {
 int result;
 DEE_ASSERT(DeeObject_Check(lhs) && DeeDictAnyIterator_Check(lhs));
 DEE_ASSERT(DeeObject_Check(rhs));
 if (DeeObject_InplaceGetInstance(&rhs,Dee_TYPE(lhs)) != 0) return NULL;
 if (lhs->di_dict != rhs->di_dict) DeeReturn_Bool(lhs->di_dict < rhs->di_dict);
 DeeDict_ACQUIRE(lhs->di_dict);
 result = DeeDictIterator_CompareLe((DeeObject *)lhs,(DeeObject *)rhs);
 DeeDict_RELEASE(lhs->di_dict);
 DeeReturn_Bool(result);
}
static DeeObject *_deedictiterator_tp_cmp_eq(
 DeeDictIteratorObject *lhs, DeeDictIteratorObject *rhs) {
 int result;
 DEE_ASSERT(DeeObject_Check(lhs) && DeeDictAnyIterator_Check(lhs));
 DEE_ASSERT(DeeObject_Check(rhs));
 if (DeeObject_InplaceGetInstance(&rhs,Dee_TYPE(lhs)) != 0) return NULL;
 if (lhs->di_dict != rhs->di_dict) DeeReturn_False;
 DeeDict_ACQUIRE(lhs->di_dict);
 result = DeeDictIterator_CompareEq((DeeObject *)lhs,(DeeObject *)rhs);
 DeeDict_RELEASE(lhs->di_dict);
 DeeReturn_Bool(result);
}
static DeeObject *_deedictiterator_tp_cmp_ne(
 DeeDictIteratorObject *lhs, DeeDictIteratorObject *rhs) {
 int result;
 DEE_ASSERT(DeeObject_Check(lhs) && DeeDictAnyIterator_Check(lhs));
 DEE_ASSERT(DeeObject_Check(rhs));
 if (DeeObject_InplaceGetInstance(&rhs,Dee_TYPE(lhs)) != 0) return NULL;
 if (lhs->di_dict != rhs->di_dict) DeeReturn_True;
 DeeDict_ACQUIRE(lhs->di_dict);
 result = DeeDictIterator_CompareNe((DeeObject *)lhs,(DeeObject *)rhs);
 DeeDict_RELEASE(lhs->di_dict);
 DeeReturn_Bool(result);
}
static DeeObject *_deedictiterator_tp_cmp_gr(
 DeeDictIteratorObject *lhs, DeeDictIteratorObject *rhs) {
 int result;
 DEE_ASSERT(DeeObject_Check(lhs) && DeeDictAnyIterator_Check(lhs));
 DEE_ASSERT(DeeObject_Check(rhs));
 if (DeeObject_InplaceGetInstance(&rhs,Dee_TYPE(lhs)) != 0) return NULL;
 if (lhs->di_dict != rhs->di_dict) DeeReturn_Bool(lhs->di_dict > rhs->di_dict);
 DeeDict_ACQUIRE(lhs->di_dict);
 result = DeeDictIterator_CompareGr((DeeObject *)lhs,(DeeObject *)rhs);
 DeeDict_RELEASE(lhs->di_dict);
 DeeReturn_Bool(result);
}
static DeeObject *_deedictiterator_tp_cmp_ge(
 DeeDictIteratorObject *lhs, DeeDictIteratorObject *rhs) {
 int result;
 DEE_ASSERT(DeeObject_Check(lhs) && DeeDictAnyIterator_Check(lhs));
 DEE_ASSERT(DeeObject_Check(rhs));
 if (DeeObject_InplaceGetInstance(&rhs,Dee_TYPE(lhs)) != 0) return NULL;
 if (lhs->di_dict != rhs->di_dict) DeeReturn_Bool(lhs->di_dict > rhs->di_dict);
 DeeDict_ACQUIRE(lhs->di_dict);
 result = DeeDictIterator_CompareGe((DeeObject *)lhs,(DeeObject *)rhs);
 DeeDict_RELEASE(lhs->di_dict);
 DeeReturn_Bool(result);
}

static int _deedictkeysiterator_tp_seq_iter_next(DeeDictIteratorObject *self, DeeObject **result) {
 struct _DeeHashMapNode *node; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeDictKeysIterator_Check(self));
 DeeDict_ACQUIRE(self->di_dict);
 node = DeeHashMapIterator_Yield(&self->di_iter,DeeDict_MAP(self->di_dict));
 if (node) { Dee_INCREF(*result = node->hmn_key); error = 0; } else error = 1;
 DeeDict_RELEASE(self->di_dict);
 return error;
}
static int _deedictitemsiterator_tp_seq_iter_next(DeeDictIteratorObject *self, DeeObject **result) {
 struct _DeeHashMapNode *node; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeDictItemsIterator_Check(self));
 DeeDict_ACQUIRE(self->di_dict);
 node = DeeHashMapIterator_Yield(&self->di_iter,DeeDict_MAP(self->di_dict));
 if (node) { Dee_INCREF(*result = node->hmn_item); error = 0; } else error = 1;
 DeeDict_RELEASE(self->di_dict);
 return error;
}
DEE_COMPILER_MSVC_WARNING_PUSH(4701 4703)
static int _deedictvaluesiterator_tp_seq_iter_next(DeeDictIteratorObject *self, DeeObject **result) {
 struct _DeeHashMapNode *node; DeeObject *key,*item; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeDictValuesIterator_Check(self));
 DeeDict_ACQUIRE(self->di_dict);
 node = DeeHashMapIterator_Yield(&self->di_iter,DeeDict_MAP(self->di_dict));
 if (node) { Dee_INCREF(key = node->hmn_key); Dee_INCREF(item = node->hmn_item); error = 0; } else error = 1;
 DeeDict_RELEASE(self->di_dict);
 if (error == 0) {
  *result = DeeTuple_Pack(2,key,item);
  Dee_DECREF(item);
  Dee_DECREF(key);
  if (!*result) return -1;
 }
 return error;
}
DEE_COMPILER_MSVC_WARNING_POP




//////////////////////////////////////////////////////////////////////////
// Dict member functions
static DeeObject *_deedict_keys(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":keys") != 0) return NULL;
 return DeeDict_Keys((DeeObject *)self);
}
static DeeObject *_deedict_items(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":items") != 0) return NULL;
 return DeeDict_Items((DeeObject *)self);
}
static DeeObject *_deedict_values(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":values") != 0) return NULL;
 return DeeDict_Values((DeeObject *)self);
}
static DeeObject *_deedict_clear(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":clear") != 0) return NULL;
 DeeDict_Clear((DeeObject *)self);
 DeeReturn_None;
}
static DeeObject *_deedict_empty(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":empty") != 0) return NULL;
 DeeReturn_Bool(DeeDict_SIZE(self) == 0);
}
static DeeObject *_deedict_non_empty(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":non_empty") != 0) return NULL;
 DeeReturn_Bool(DeeDict_SIZE(self) != 0);
}
static DeeObject *_deedict_bucket_size(DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_ssize_t bucket; Dee_size_t result;
 if (DeeTuple_Unpack(args,"Id:bucket_size",&bucket) != 0) return NULL;
 if ((result = DeeDict_BucketSize((DeeObject *)self,bucket)) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *_deedict_bucket_count(DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":bucket_count") != 0) return NULL;
 return DeeObject_New(Dee_size_t,DeeDict_BucketCount((DeeObject *)self));
}
static DeeObject *_deedict_insert_all(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *sequence;
 if (DeeTuple_Unpack(args,"o:insert_all",&sequence) != 0) return NULL;
 if (DeeDict_InsertSequence((DeeObject *)self,sequence) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deedict_pop(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *key,*def = NULL;
 if (DeeTuple_Unpack(args,"o|o:pop",&key,&def) != 0) return NULL;
 return DeeDict_Pop((DeeObject *)self,key,def);
}
static DeeObject *_deedict_setdefault(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *key,*def = Dee_None;
 if (DeeTuple_Unpack(args,"o|o:setdefault",&key,&def) != 0) return NULL;
 return DeeDict_SetDefault((DeeObject *)self,key,def);
}
static DeeObject *_deedict_setnew(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *key,*item; int result;
 if (DeeTuple_Unpack(args,"oo:setnew",&key,&item) != 0) return NULL;
 if ((result = DeeDict_SetNewItem((DeeObject *)self,key,item,NULL)) < 0) return NULL;
 DeeReturn_Bool(result == 0);
}
static DeeObject *_deedict_setold(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *key,*item; int result;
 if (DeeTuple_Unpack(args,"oo:setold",&key,&item) != 0) return NULL;
 if ((result = DeeDict_SetOldItem((DeeObject *)self,key,item,NULL)) < 0) return NULL;
 DeeReturn_Bool(result == 0);
}
static DeeObject *_deedict_setnew_ex(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *key,*item,*old_item; int result;
 if (DeeTuple_Unpack(args,"oo:setnew_ex",&key,&item) != 0) return NULL;
 if ((result = DeeDict_SetNewItem((DeeObject *)self,key,item,&old_item)) < 0) return NULL;
 return Dee_BuildValue("(bO)",result == 0,result == 0 ? DeeNone_New() : old_item);
}
static DeeObject *_deedict_setold_ex(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *key,*item,*old_item; int result;
 if (DeeTuple_Unpack(args,"oo:setold_ex",&key,&item) != 0) return NULL;
 if ((result = DeeDict_SetOldItem((DeeObject *)self,key,item,&old_item)) < 0) return NULL;
 return Dee_BuildValue("(bO)",result == 0,result == 0 ? old_item : DeeNone_New());
}
static DeeObject *_deedict_get(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *key,*def = Dee_None;
 if (DeeTuple_Unpack(args,"o|o:get",&key,&def) != 0) return NULL;
 return DeeDict_GetDefault((DeeObject *)self,key,def);
}
static DeeObject *_deedict_popitem(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*key,*item;
 if (DeeTuple_Unpack(args,":popitem") != 0) return NULL;
 if (DeeDict_PopSomething((DeeObject *)self,&key,&item) != 0) return NULL;
 result = DeeTuple_Pack(2,key,item);
 Dee_DECREF(key);
 Dee_DECREF(item);
 return result;
}

static DeeObject *_deedictproxy_clear(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":clear") != 0) return NULL;
 DeeDict_Clear((DeeObject *)self->dp_dict);
 DeeReturn_None;
}
static DeeObject *_deedictproxy_empty(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":empty") != 0) return NULL;
 DeeReturn_Bool(DeeDict_Size((DeeObject *)self->dp_dict) == 0);
}
static DeeObject *_deedictproxy_non_empty(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":empty") != 0) return NULL;
 DeeReturn_Bool(DeeDict_Size((DeeObject *)self->dp_dict) == 0);
}
static DeeObject *_deedictkeys_find(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem; Dee_ssize_t index;
 if (DeeTuple_Unpack(args,"o:find",&elem) != 0) return NULL;
 if ((index = DeeDict_FindKeyIndex((DeeObject *)self->dp_dict,elem)) == -2) return NULL;
 DEE_ASSERT(index >= -1);
 return DeeObject_New(Dee_ssize_t,index);
}
static DeeObject *_deedictitems_find(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem; Dee_ssize_t index;
 if (DeeTuple_Unpack(args,"o:find",&elem) != 0) return NULL;
 if ((index = DeeDict_FindItemIndex((DeeObject *)self->dp_dict,elem)) == -2) return NULL;
 DEE_ASSERT(index >= -1);
 return DeeObject_New(Dee_ssize_t,index);
}
static DeeObject *_deedictvalues_find(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *key_item; Dee_ssize_t index;
 if (DeeTuple_Unpack(args,"o:find",&key_item) != 0) return NULL;
 if (!DeeTuple_Check(key_item) || DeeTuple_SIZE(key_item) != 2) return DeeObject_New(Dee_ssize_t,-1);
 if ((index = DeeDict_FindKeyItemIndex((DeeObject *)self->dp_dict,
  DeeTuple_GET(key_item,0),DeeTuple_GET(key_item,1))) == -2) return NULL;
 DEE_ASSERT(index >= -1);
 return DeeObject_New(Dee_ssize_t,index);
}
static DeeObject *_deedict_find(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *key_item; Dee_ssize_t index;
 if (DeeTuple_Unpack(args,"o:find",&key_item) != 0) return NULL;
 if (!DeeTuple_Check(key_item) || DeeTuple_SIZE(key_item) != 2) return DeeObject_New(Dee_ssize_t,-1);
 if ((index = DeeDict_FindKeyItemIndex((DeeObject *)self,
  DeeTuple_GET(key_item,0),DeeTuple_GET(key_item,1))) == -2) return NULL;
 DEE_ASSERT(index >= -1);
 return DeeObject_New(Dee_ssize_t,index);
}
static DeeObject *_deedictkeys_index(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem; Dee_size_t index;
 if (DeeTuple_Unpack(args,"o:index",&elem) != 0) return NULL;
 if ((index = DeeDict_IndexKeyIndex((DeeObject *)self->dp_dict,elem)) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,index);
}
static DeeObject *_deedictitems_index(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem; Dee_size_t index;
 if (DeeTuple_Unpack(args,"o:index",&elem) != 0) return NULL;
 if ((index = DeeDict_IndexItemIndex((DeeObject *)self->dp_dict,elem)) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,index);
}
static DeeObject *_deedictvalues_index(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *key_item; Dee_size_t index;
 if (DeeTuple_Unpack(args,"o:index",&key_item) != 0) return NULL;
 if (!DeeTuple_Check(key_item) || DeeTuple_SIZE(key_item) != 2) {
  DeeError_ValueError_NotFound((DeeObject *)self,key_item);
  return NULL;
 }
 if ((index = DeeDict_IndexKeyItemIndex((DeeObject *)self->dp_dict,
  DeeTuple_GET(key_item,0),DeeTuple_GET(key_item,1))) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,index);
}
static DeeObject *_deedict_index(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *key_item; Dee_size_t index;
 if (DeeTuple_Unpack(args,"o:index",&key_item) != 0) return NULL;
 if (!DeeTuple_Check(key_item) || DeeTuple_SIZE(key_item) != 2) {
  DeeError_ValueError_NotFound((DeeObject *)self,key_item);
  return NULL;
 }
 if ((index = DeeDict_IndexKeyItemIndex((DeeObject *)self,
  DeeTuple_GET(key_item,0),DeeTuple_GET(key_item,1))) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,index);
}
static DeeObject *_deedictkeys_count(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem; int temp;
 if (DeeTuple_Unpack(args,"o:count",&elem) != 0) return NULL;
 if ((temp = DeeDict_HasKey((DeeObject *)self->dp_dict,elem)) < 0) return NULL;
 DEE_ASSERT(temp == 1 || temp == 0);
 return DeeObject_New(Dee_size_t,(Dee_size_t)temp);
}
static DeeObject *_deedictitems_count(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem; Dee_size_t result;
 if (DeeTuple_Unpack(args,"o:count",&elem) != 0) return NULL;
 if ((result = DeeDict_CountItem((DeeObject *)self->dp_dict,elem)) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *_deedictvalues_count(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *key_item; int temp;
 if (DeeTuple_Unpack(args,"o:count",&key_item) != 0) return NULL;
 if (!DeeTuple_Check(key_item) || DeeTuple_SIZE(key_item) != 2) return DeeObject_New(Dee_size_t,0);
 if ((temp = DeeDict_HasKeyItem((DeeObject *)self->dp_dict,
  DeeTuple_GET(key_item,0),DeeTuple_GET(key_item,1))) < 0) return NULL;
 DEE_ASSERT(temp == 0 || temp == 1);
 return DeeObject_New(Dee_size_t,(Dee_size_t)temp);
}
static DeeObject *_deedict_count(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *key_item; int temp;
 if (DeeTuple_Unpack(args,"o:count",&key_item) != 0) return NULL;
 if (!DeeTuple_Check(key_item) || DeeTuple_SIZE(key_item) != 2) return DeeObject_New(Dee_size_t,0);
 if ((temp = DeeDict_HasKeyItem((DeeObject *)self,
  DeeTuple_GET(key_item,0),DeeTuple_GET(key_item,1))) < 0) return NULL;
 DEE_ASSERT(temp == 0 || temp == 1);
 return DeeObject_New(Dee_size_t,(Dee_size_t)temp);
}
static DeeObject *_deedict_contains(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *keyitem,*pred = Dee_None; int result;
 if (DeeTuple_Unpack(args,"o|o:contains",&keyitem,&pred) != 0) return NULL;
 if (!DeeNone_Check(pred) && pred != (DeeObject *)&DeeBuiltinFunction___eq__)
  result = DeeDict_HasKeyItemPred((DeeObject *)self,keyitem,pred);
 if (!DeeTuple_Check(keyitem) || DeeTuple_SIZE(keyitem) != 2) DeeReturn_False;
 result = DeeDict_HasKeyItem((DeeObject *)self,
                             DeeTuple_GET(keyitem,0),
                             DeeTuple_GET(keyitem,1));
 if (result < 0) return NULL;
 DeeReturn_Bool(result);
}
static DeeObject *_deedictkeys_contains(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *key,*pred = Dee_None; int result;
 if (DeeTuple_Unpack(args,"o|o:contains",&key,&pred) != 0) return NULL;
 result = (DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeDict_HasKey((DeeObject *)self->dp_dict,key)
  : DeeDict_HasKeyPred((DeeObject *)self->dp_dict,key,pred);
 if (result < 0) return NULL;
 DeeReturn_Bool(result);
}
static DeeObject *_deedictitems_contains(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *item,*pred = Dee_None; int result;
 if (DeeTuple_Unpack(args,"o|o:contains",&item,&pred) != 0) return NULL;
 result = (DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeDict_HasItem((DeeObject *)self->dp_dict,item)
  : DeeDict_HasItemPred((DeeObject *)self->dp_dict,item,pred);
 if (result < 0) return NULL;
 DeeReturn_Bool(result);
}
static DeeObject *_deedictvalues_contains(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *keyitem,*pred = Dee_None; int result;
 if (DeeTuple_Unpack(args,"o|o:contains",&keyitem,&pred) != 0) return NULL;
 if (!DeeNone_Check(pred) && pred != (DeeObject *)&DeeBuiltinFunction___eq__)
  result = DeeDict_HasKeyItemPred((DeeObject *)self->dp_dict,keyitem,pred);
 if (!DeeTuple_Check(keyitem) || DeeTuple_SIZE(keyitem) != 2) DeeReturn_False;
 result = DeeDict_HasKeyItem((DeeObject *)self->dp_dict,
                             DeeTuple_GET(keyitem,0),
                             DeeTuple_GET(keyitem,1));
 if (result < 0) return NULL;
 DeeReturn_Bool(result);
}
static DeeObject *_deedict_front(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*key,*item;
 if (DeeTuple_Unpack(args,":front") != 0) return NULL;
 if (!DeeDict_Front((DeeObject *)self,&key,NULL,&item)) {
  DeeError_ValueError_EmptyList(&DeeDict_Type);
  return NULL;
 }
 result = DeeTuple_Pack(2,key,item);
 Dee_DECREF(key);
 Dee_DECREF(item);
 return result;
}
static DeeObject *_deedict_back(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*key,*item;
 if (DeeTuple_Unpack(args,":back") != 0) return NULL;
 if (!DeeDict_Back((DeeObject *)self,&key,NULL,&item)) {
  DeeError_ValueError_EmptyList(&DeeDict_Type);
  return NULL;
 }
 result = DeeTuple_Pack(2,key,item);
 Dee_DECREF(key);
 Dee_DECREF(item);
 return result;
}
static DeeObject *_deedictkeys_front(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if (DeeTuple_Unpack(args,":front") != 0) return NULL;
 if (!DeeDict_Front((DeeObject *)self->dp_dict,&result,NULL,NULL)) {
  DeeError_ValueError_EmptyList(&DeeDictKeys_Type);
  return NULL;
 }
 return result;
}
static DeeObject *_deedictkeys_back(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if (DeeTuple_Unpack(args,":back") != 0) return NULL;
 if (!DeeDict_Back((DeeObject *)self->dp_dict,&result,NULL,NULL)) {
  DeeError_ValueError_EmptyList(&DeeDictKeys_Type);
  return NULL;
 }
 return result;
}
static DeeObject *_deedictitems_front(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if (DeeTuple_Unpack(args,":front") != 0) return NULL;
 if (!DeeDict_Front((DeeObject *)self->dp_dict,NULL,NULL,&result)) {
  DeeError_ValueError_EmptyList(&DeeDictItems_Type);
  return NULL;
 }
 return result;
}
static DeeObject *_deedictitems_back(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if (DeeTuple_Unpack(args,":back") != 0) return NULL;
 if (!DeeDict_Back((DeeObject *)self->dp_dict,NULL,NULL,&result)) {
  DeeError_ValueError_EmptyList(&DeeDictItems_Type);
  return NULL;
 }
 return result;
}
static DeeObject *_deedictvalues_front(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*key,*item;
 if (DeeTuple_Unpack(args,":front") != 0) return NULL;
 if (!DeeDict_Front((DeeObject *)self->dp_dict,&key,NULL,&item)) {
  DeeError_ValueError_EmptyList(&DeeDictValues_Type);
  return NULL;
 }
 result = DeeTuple_Pack(2,key,item);
 Dee_DECREF(key);
 Dee_DECREF(item);
 return result;
}
static DeeObject *_deedictvalues_back(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*key,*item;
 if (DeeTuple_Unpack(args,":back") != 0) return NULL;
 if (!DeeDict_Front((DeeObject *)self->dp_dict,&key,NULL,&item)) {
  DeeError_ValueError_EmptyList(&DeeDictValues_Type);
  return NULL;
 }
 result = DeeTuple_Pack(2,key,item);
 Dee_DECREF(key);
 Dee_DECREF(item);
 return result;
}


static DeeObject *_deedict_locate(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if (DeeTuple_Unpack(args,"o:locate",&pred) != 0) return NULL;
 return DeeDict_LocateValue((DeeObject *)self,pred);
}
static DeeObject *_deedict_locate_all(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if (DeeTuple_Unpack(args,"o:locate_all",&pred) != 0) return NULL;
 return DeeDict_LocateAll((DeeObject *)self,pred);
}
static DeeObject *_deedictkeys_locate(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if (DeeTuple_Unpack(args,"o:locate",&pred) != 0) return NULL;
 return DeeDict_LocateKey((DeeObject *)self->dp_dict,pred);
}
static DeeObject *_deedictkeys_locate_all(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if (DeeTuple_Unpack(args,"o:locate_all",&pred) != 0) return NULL;
 return DeeDict_LocateAllKeys((DeeObject *)self->dp_dict,pred);
}
static DeeObject *_deedictitems_locate(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if (DeeTuple_Unpack(args,"o:locate",&pred) != 0) return NULL;
 return DeeDict_LocateItem((DeeObject *)self->dp_dict,pred);
}
static DeeObject *_deedictitems_locate_all(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if (DeeTuple_Unpack(args,"o:locate_all",&pred) != 0) return NULL;
 return DeeDict_LocateAllItems((DeeObject *)self->dp_dict,pred);
}
static DeeObject *_deedictvalues_locate(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if (DeeTuple_Unpack(args,"o:locate",&pred) != 0) return NULL;
 return DeeDict_LocateValue((DeeObject *)self->dp_dict,pred);
}
static DeeObject *_deedictvalues_locate_all(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if (DeeTuple_Unpack(args,"o:locate_all",&pred) != 0) return NULL;
 return DeeDict_LocateAllValues((DeeObject *)self->dp_dict,pred);
}

static DeeObject *_deedict_transform(
 DeeDictObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if (DeeTuple_Unpack(args,"o:transform",&pred) != 0) return NULL;
 return DeeDict_Transform((DeeObject *)self,pred);
}
static DeeObject *_deedictkeys_transform(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if (DeeTuple_Unpack(args,"o:transform",&pred) != 0) return NULL;
 return DeeDict_TransformKeys((DeeObject *)self->dp_dict,pred);
}
static DeeObject *_deedictitems_transform(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if (DeeTuple_Unpack(args,"o:transform",&pred) != 0) return NULL;
 return DeeDict_TransformItems((DeeObject *)self->dp_dict,pred);
}
static DeeObject *_deedictvalues_transform(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if (DeeTuple_Unpack(args,"o:transform",&pred) != 0) return NULL;
 return DeeDict_TransformValues((DeeObject *)self->dp_dict,pred);
}


static DeeObject *_deedictkeys_at(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t index;
 if (DeeTuple_Unpack(args,"Iu:at",&index) != 0) return NULL;
 return DeeDict_GetKeyIndexAt((DeeObject *)self->dp_dict,index);
}
static DeeObject *_deedictitems_at(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t index;
 if (DeeTuple_Unpack(args,"Iu:at",&index) != 0) return NULL;
 return DeeDict_GetItemIndexAt((DeeObject *)self->dp_dict,index);
}
static DeeObject *_deedictvalues_at(
 DeeDictProxyObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t index;
 if (DeeTuple_Unpack(args,"Iu:at",&index) != 0) return NULL;
 return DeeDict_GetValueIndexAt((DeeObject *)self->dp_dict,index);
}


static struct DeeMethodDef const _deedict_tp_methods[] = {
 DEE_METHODDEF_CONST_v100("back",member(&_deedict_back),"() -> (object,object)"),
 DEE_METHODDEF_CONST_v100("bucket_count",member(&_deedict_bucket_count),"() -> size_t\n\tReturns the amount of active buckets in this dict"),
 DEE_METHODDEF_CONST_v100("bucket_size",member(&_deedict_bucket_size),"(size_t index) -> size_t\n\tReturns the size of a given bucket by index"),
 DEE_METHODDEF_CONST_v100("count",member(&_deedict_count),"(object elem) -> size_t"),
 DEE_METHODDEF_CONST_v100("empty",member(&_deedict_empty),"() -> bool\n\tReturns true if the dict is empty"),
 DEE_METHODDEF_CONST_v100("find",member(&_deedict_find),"(object elem) -> ssize_t"),
 DEE_METHODDEF_CONST_v100("front",member(&_deedict_front),"() -> (object,object)"),
 DEE_METHODDEF_CONST_v100("get",member(&_deedict_get),"(object key, object def = none) -> object\n\tReturns the item associated with 'key' or 'def' if 'key' wasn't found"),
 DEE_METHODDEF_CONST_v100("index",member(&_deedict_index),"(object elem) -> ssize_t"),
 DEE_METHODDEF_CONST_v100("items",member(&_deedict_items),"() -> dict.items\n\tReturns a representation of the dict describing all items"),
 DEE_METHODDEF_CONST_v100("keys",member(&_deedict_keys),"() -> dict.keys\n\tReturns a representation of the dict describing all keys"),
 DEE_METHODDEF_CONST_v100("locate",member(&_deedict_locate),"(callable pred) -> (object,object)"),
 DEE_METHODDEF_CONST_v100("locate_all",member(&_deedict_locate_all),"(callable pred) -> dict"),
 DEE_METHODDEF_CONST_v100("non_empty",member(&_deedict_non_empty),"() -> bool\n\tReturns true if the dict is not empty"),
 DEE_METHODDEF_CONST_v100("rlocate",member(&_deegenericiterable_rlocate),"(callable pred) -> (object,object)"),
 DEE_METHODDEF_CONST_v100("transform",member(&_deedict_transform),"(callable pred) -> dict"),
 DEE_METHODDEF_CONST_v100("values",member(&_deedict_values),
  "() -> dict.values\n"
  "\tReturns a representation of the dict describing all key-item pairs"),
 DEE_METHODDEF_v100("clear",member(&_deedict_clear),
  "() -> none\n"
  "\tCleans all key-item pairs from the dict, leaving this dict as an empty dict"),
 DEE_METHODDEF_v100("insert_all",member(&_deedict_insert_all),
  "(iterable sequence) -> none\n"
  "\tInserts a given sequence of key-item pairs"),
 DEE_METHODDEF_v100("pop",member(&_deedict_pop),
  "(object key) -> object\n"
  "(object key, object def) -> object\n"
  "\tPops and returns the items of 'key' and return 'def' or raises an 'Error.KeyError' if 'key' wasn't found"),
 DEE_METHODDEF_v100("setdefault",member(&_deedict_setdefault),
  "(object key, object def = none) -> object\n"
  "\tReturns the item associated with 'key' and sets it to 'def' if 'key' didn't exist before"),
 DEE_METHODDEF_v100("setold",member(&_deedict_setold),
  "(object key, object item) -> bool\n"
  "\tOverrides an existing item, but fails (returns false) if the key didn't exist"),
 DEE_METHODDEF_v100("setnew",member(&_deedict_setnew),
  "(object key, object item) -> bool\n"
  "\tInserts a new item, but fails (returns false) if the key already existed"),
 DEE_METHODDEF_v100("setold_ex",member(&_deedict_setold_ex),
  "(object key, object item) -> (bool,object)\n"
  "\tOverrides an existing item and returns it, but fails (returns false) if the key didn't exist"),
 DEE_METHODDEF_v100("setnew_ex",member(&_deedict_setnew_ex),
  "(object key, object item) -> (bool,object)\n"
  "\tInserts a new item, but fails (returns false and the old item) if the key already existed"),
 DEE_METHODDEF_v100("popitem",member(&_deedict_popitem),
  "() -> (object,object)\n"
  "\tReturns some key-item pair from this dict after removing said pair from the dict"),
 DEE_METHODDEF_CONST_v100("contains",member(&_deedict_contains),NULL),
 // TODO: intrinsic version
 DEE_METHODDEF_CONST_v100("reversed",member(&_deegenericiterable_reversed),NULL),
 DEE_METHODDEF_CONST_v100("rfind",member(&_deegenericiterable_rfind),NULL),
 DEE_METHODDEF_CONST_v100("rindex",member(&_deegenericiterable_rindex),NULL),
 DEE_METHODDEF_CONST_v100("segments",member(&_deegenericiterable_segments),NULL),
 DEE_METHODDEF_CONST_v100("sorted",member(&_deegenericiterable_sorted),NULL),
 DEE_METHODDEF_END_v100
};
static struct DeeMethodDef const _deedictkeys_tp_methods[] = {
 DEE_METHODDEF_CONST_v100("at",member(&_deedictkeys_at),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("back",member(&_deedictkeys_back),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("count",member(&_deedictkeys_count),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("empty",member(&_deedictproxy_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("find",member(&_deedictkeys_find),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("front",member(&_deedictkeys_front),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("index",member(&_deedictkeys_index),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("locate",member(&_deedictkeys_locate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("locate_all",member(&_deedictkeys_locate_all),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("non_empty",member(&_deedictproxy_non_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rlocate",member(&_deegenericiterable_rlocate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("transform",member(&_deedictkeys_transform),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("clear",member(&_deedictproxy_clear),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("contains",member(&_deedictkeys_contains),NULL),
 // TODO: intrinsic version
 DEE_METHODDEF_CONST_v100("reversed",member(&_deegenericiterable_reversed),NULL),
 DEE_METHODDEF_CONST_v100("rfind",member(&_deegenericiterable_rfind),NULL),
 DEE_METHODDEF_CONST_v100("rindex",member(&_deegenericiterable_rindex),NULL),
 DEE_METHODDEF_CONST_v100("segments",member(&_deegenericiterable_segments),NULL),
 DEE_METHODDEF_CONST_v100("sorted",member(&_deegenericiterable_sorted),NULL),
 DEE_METHODDEF_END_v100
};
static struct DeeMethodDef const _deedictitems_tp_methods[] = {
 DEE_METHODDEF_CONST_v100("at",member(&_deedictitems_at),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("back",member(&_deedictitems_back),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("count",member(&_deedictitems_count),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("empty",member(&_deedictproxy_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("find",member(&_deedictitems_find),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("front",member(&_deedictitems_front),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("index",member(&_deedictitems_index),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("locate",member(&_deedictitems_locate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("locate_all",member(&_deedictitems_locate_all),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("non_empty",member(&_deedictproxy_non_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rlocate",member(&_deegenericiterable_rlocate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("transform",member(&_deedictitems_transform),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("clear",member(&_deedictproxy_clear),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("contains",member(&_deedictitems_contains),NULL),
 // TODO: intrinsic version
 DEE_METHODDEF_CONST_v100("reversed",member(&_deegenericiterable_reversed),NULL),
 DEE_METHODDEF_CONST_v100("rfind",member(&_deegenericiterable_rfind),NULL),
 DEE_METHODDEF_CONST_v100("rindex",member(&_deegenericiterable_rindex),NULL),
 DEE_METHODDEF_CONST_v100("segments",member(&_deegenericiterable_segments),NULL),
 DEE_METHODDEF_CONST_v100("sorted",member(&_deegenericiterable_sorted),NULL),
 DEE_METHODDEF_END_v100
};
static struct DeeMethodDef const _deedictvalues_tp_methods[] = {
 DEE_METHODDEF_CONST_v100("at",member(&_deedictvalues_at),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("back",member(&_deedictvalues_back),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("count",member(&_deedictvalues_count),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("empty",member(&_deedictproxy_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("find",member(&_deedictvalues_find),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("front",member(&_deedictvalues_front),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("index",member(&_deedictvalues_index),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("locate",member(&_deedictvalues_locate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("locate_all",member(&_deedictvalues_locate_all),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("non_empty",member(&_deedictproxy_non_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rlocate",member(&_deegenericiterable_rlocate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("transform",member(&_deedictvalues_transform),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("clear",member(&_deedictproxy_clear),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("contains",member(&_deedictvalues_contains),NULL),
 // TODO: intrinsic version
 DEE_METHODDEF_CONST_v100("reversed",member(&_deegenericiterable_reversed),NULL),
 DEE_METHODDEF_CONST_v100("rfind",member(&_deegenericiterable_rfind),NULL),
 DEE_METHODDEF_CONST_v100("rindex",member(&_deegenericiterable_rindex),NULL),
 DEE_METHODDEF_CONST_v100("segments",member(&_deegenericiterable_segments),NULL),
 DEE_METHODDEF_CONST_v100("sorted",member(&_deegenericiterable_sorted),NULL),
 DEE_METHODDEF_END_v100
};
static struct DeeMemberDef const _deedict_tp_members[] = {
 DEE_MEMBERDEF_NAMED_v100("max_load_factor",DeeDictObject,d_map.hm_max_load_factor,double),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#ifndef DEE_WITHOUT_THREADS
 DEE_MEMBERDEF_NAMED_RO_v100("__d_lock",DeeDictObject,d_lock,DeeAtomicMutex),
#endif
 DEE_MEMBERDEF_NAMED_RO_v100("__hm_valid_buckets_begin",DeeDictObject,d_map.hm_valid_buckets_begin,p2(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__hm_valid_buckets_end",DeeDictObject,d_map.hm_valid_buckets_end,p2(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__hm_buckets",DeeDictObject,d_map.hm_buckets,p2(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__hm_bucket_count",DeeDictObject,d_map.hm_bucket_count,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__hm_map_size",DeeDictObject,d_map.hm_map_size,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__hm_max_load_factor",DeeDictObject,d_map.hm_max_load_factor,double),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const _deedictiterator_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("dict",DeeDictIteratorObject,di_dict,object),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__di_dict",DeeDictIteratorObject,di_dict,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__hmi_buckets_begin",DeeDictIteratorObject,di_iter.hmi_buckets_begin,p2(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__hmi_buckets_end",DeeDictIteratorObject,di_iter.hmi_buckets_end,p2(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__hmi_hash_curr",DeeDictIteratorObject,di_iter.hmi_hash_curr,p(void)),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const _deedictproxy_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("dict",DeeDictProxyObject,dp_dict,object),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__dp_dict",DeeDictProxyObject,dp_dict,object),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};

static struct DeeMemberDef const _deedict_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("keys",object,&DeeDictKeys_Type),
 DEE_MEMBERDEF_CONST_v100("items",object,&DeeDictItems_Type),
 DEE_MEMBERDEF_CONST_v100("values",object,&DeeDictValues_Type),
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeDictIterator_Type),
 DEE_MEMBERDEF_END_v100
};

static struct DeeMemberDef const _deedictkeys_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeDictKeysIterator_Type),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const _deedictitems_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeDictItemsIterator_Type),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const _deedictvalues_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeDictIterator_Type),
 DEE_MEMBERDEF_END_v100
};















static int DEE_CALL _deedict_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeDictObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_uint64_t read_size; DeeObject *key,*item; int temp;
 if (DeeFile_GetLeSmall64(infile,&read_size) != 0) return -1;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_size > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Dict is too big for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_size,(Dee_size_t)-1);
  return -1;
 }
#endif
 DeeHashMap_Init(&self->d_map);
 while (read_size--) {
  if ((key = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {
err_self: DeeHashMap_Quit(&self->d_map); return -1;
  }
  if ((item = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {
   Dee_DECREF(key); goto err_self;
  }
  temp = DeeHashMap_SetItem(&self->d_map,key,item);
  Dee_DECREF(item);
  Dee_DECREF(key);
  if (temp != 0) return -1;
 }
 DeeAtomicMutex_Init(&self->d_lock);
 return 0;
}

static int DEE_CALL _deedict_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeDictObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 Dee_size_t size; int temp;
 DeeHashMap_TRAVERSE_SAFE_VARS; DeeObject *key,*item;
 struct _DeeHashMapNode *node;
 DeeDict_ACQUIRE(self);
 size = self->d_map.hm_map_size;
 DeeDict_RELEASE(self);
 if (DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)size) != 0) return -1;
 DeeDict_ACQUIRE(self);
 DeeHashMap_TRAVERSE_SAFE(node,&self->d_map) {
  Dee_INCREF(key = node->hmn_key);
  Dee_INCREF(item = node->hmn_item);
  DeeDict_RELEASE(self);
  temp = DeeMarshal_WriteObjectWithMap(outfile,map,key);
  if (temp == 0) temp = DeeMarshal_WriteObjectWithMap(outfile,map,item);
  Dee_DECREF(item);
  Dee_DECREF(key);
  if (temp != 0) return temp;
  --size;
  DeeDict_ACQUIRE(self);
 }
 DeeDict_RELEASE(self);
 // Write any objects that we weren't able to
 // retrieve due to another thread modifying the dict.
 while (size--) {
  if (DeeMarshal_WriteObjectWithMap(outfile,map,Dee_None) != 0 ||
      DeeMarshal_WriteObjectWithMap(outfile,map,Dee_None) != 0
      ) return -1;
 }
 return 0;
}

static struct DeeTypeMarshal _deedict_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(dict)),
 member(&_deedict_tp_marshal_ctor),member(&_deedict_tp_marshal_put));



static int DEE_CALL _deedictproxy_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeDictProxyObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 if ((self->dp_dict = (DeeDictObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if (DeeError_TypeError_CheckType((DeeObject *)self->dp_dict,&DeeDict_Type) != 0) { Dee_DECREF(self->dp_dict); return -1; }
 return 0;
}
static int DEE_CALL _deedictproxy_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeDictProxyObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 return DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)self->dp_dict);
}
static struct DeeTypeMarshal _deedictkeys_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(dict_keys)),
 member(&_deedictproxy_tp_marshal_ctor),member(&_deedictproxy_tp_marshal_put));
static struct DeeTypeMarshal _deedictitems_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(dict_items)),
 member(&_deedictproxy_tp_marshal_ctor),member(&_deedictproxy_tp_marshal_put));
static struct DeeTypeMarshal _deedictvalues_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(dict_values)),
 member(&_deedictproxy_tp_marshal_ctor),member(&_deedictproxy_tp_marshal_put));

DeeTypeObject DeeDict_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("dict"),null,
  member(DEE_TYPE_FLAG_HAS_GC|DEE_TYPE_FLAG_CONST_CTOR|DEE_TYPE_FLAG_MUST_COPY|
         DEE_TYPE_FLAG_NO_ITERATE_EFFECT|DEE_TYPE_FLAG_FUNDAMENTAL),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(
  sizeof(DeeDictObject),member(&_DeeGC_TpAlloc),
  member(&_deedict_tp_ctor),member(&_deedict_tp_copy_ctor),
  member(&_deedict_tp_move_ctor),member(&_deedict_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(member(&_DeeGC_TpFree),
  member(&_deedict_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deedict_tp_copy_assign),member(&_deedict_tp_move_assign),
  member(&_deedict_tp_any_assign)),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&DeeDict_Str),member(&DeeDict_Repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deedict_tp_visit),member(&DeeDict_Clear)),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deedict_tp_not),member(&_deedict_tp_bool),
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,
  member(&_deedict_tp_hash)),
 DEE_TYPE_OBJECT_COMPARE_v100(
  // TODO: Optimized version for dict
  member(&_deegenericiterable_tp_cmp_lo),
  member(&_deegenericiterable_tp_cmp_le),
  member(&_deegenericiterable_tp_cmp_eq),
  member(&_deegenericiterable_tp_cmp_ne),
  member(&_deegenericiterable_tp_cmp_gr),
  member(&_deegenericiterable_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&DeeDict_GetItem),member(&DeeDict_DelItem),
  member(&DeeDict_SetItem),member(&_deedict_tp_seq_size),
  member(&_deedict_tp_seq_contains),null,null,null,
  member(&DeeDict_IterSelf),null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deedict_tp_members),null,member(_deedict_tp_methods),
  member(_deedict_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deedict_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};
DeeTypeObject DeeDictKeys_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("dict.keys"),null,
  member(DEE_TYPE_FLAG_NO_ITERATE_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeDictProxyObject),null,null,
  member(&_deedictproxy_tp_copy_ctor),null,
  member(&_deedictproxy_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deedictproxy_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deegenericlist_tp_str),
  member(&_deegenericlist_tp_str),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deedictproxy_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deedictproxy_tp_not),
  member(&_deedictproxy_tp_bool),
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,
  member(&_deedictkeys_tp_hash)),
 DEE_TYPE_OBJECT_COMPARE_v100(
  // TODO: Optimized version for dict.keys
  member(&_deegenericiterable_tp_cmp_lo),
  member(&_deegenericiterable_tp_cmp_le),
  member(&_deegenericiterable_tp_cmp_eq),
  member(&_deegenericiterable_tp_cmp_ne),
  member(&_deegenericiterable_tp_cmp_gr),
  member(&_deegenericiterable_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&_deedictkeys_tp_seq_get),
  member(&_deedictproxy_tp_seq_del),null,
  member(&_deedictproxy_tp_seq_size),
  member(&_deedictkeys_tp_seq_contains),
  member(&_deedictkeys_tp_seq_range_get),
  member(&_deedictproxy_tp_seq_range_del),null,
  member(&DeeDictKeys_IterSelf),null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deedictproxy_tp_members),null,member(_deedictkeys_tp_methods),
  member(_deedictkeys_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deedictkeys_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};
DeeTypeObject DeeDictItems_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("dict.items"),null,
  member(DEE_TYPE_FLAG_NO_ITERATE_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeDictProxyObject),null,null,
  member(&_deedictproxy_tp_copy_ctor),null,
  member(&_deedictproxy_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deedictproxy_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deegenericlist_tp_str),
  member(&_deegenericlist_tp_str),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deedictproxy_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deedictproxy_tp_not),
  member(&_deedictproxy_tp_bool),
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,
  member(&_deedictitems_tp_hash)),
 DEE_TYPE_OBJECT_COMPARE_v100(
  // TODO: optimized version for dict.items
  member(&_deegenericiterable_tp_cmp_lo),
  member(&_deegenericiterable_tp_cmp_le),
  member(&_deegenericiterable_tp_cmp_eq),
  member(&_deegenericiterable_tp_cmp_ne),
  member(&_deegenericiterable_tp_cmp_gr),
  member(&_deegenericiterable_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&_deedictitems_tp_seq_get),
  member(&_deedictproxy_tp_seq_del),
  member(&_deedictitems_tp_seq_set),
  member(&_deedictproxy_tp_seq_size),
  member(&_deedictitems_tp_seq_contains),
  member(&_deedictitems_tp_seq_range_get),
  member(&_deedictproxy_tp_seq_range_del),null,
  member(&DeeDictItems_IterSelf),null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deedictproxy_tp_members),null,member(_deedictitems_tp_methods),
  member(_deedictitems_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deedictitems_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};
DeeTypeObject DeeDictValues_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("dict.values"),null,
  member(DEE_TYPE_FLAG_NO_ITERATE_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeDictProxyObject),null,null,
  member(&_deedictproxy_tp_copy_ctor),null,
  member(&_deedictproxy_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deedictproxy_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deegenericlist_tp_str),
  member(&_deegenericlist_tp_str),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deedictproxy_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deedictproxy_tp_not),
  member(&_deedictproxy_tp_bool),
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,
  member(&_deedictvalues_tp_hash)),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deegenericiterable_tp_cmp_lo),
  member(&_deegenericiterable_tp_cmp_le),
  member(&_deegenericiterable_tp_cmp_eq),
  member(&_deegenericiterable_tp_cmp_ne),
  member(&_deegenericiterable_tp_cmp_gr),
  member(&_deegenericiterable_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&_deedictvalues_tp_seq_get),
  member(&_deedictproxy_tp_seq_del),null,
  member(&_deedictproxy_tp_seq_size),
  member(&_deedictvalues_tp_seq_contains),
  member(&_deedictvalues_tp_seq_range_get),
  member(&_deedictproxy_tp_seq_range_del),null,
  member(&DeeDictValues_IterSelf),null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deedictproxy_tp_members),null,member(_deedictvalues_tp_methods),
  member(_deedictvalues_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deedictvalues_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DeeTypeObject DeeDictIterator_Type = {
 // NOTE: Also used as dict.values.iterator
 DEE_TYPE_OBJECT_HEAD_v100(member("dict.iterator"),null,null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeDictIteratorObject),null,null,
  member(&_deedictiterator_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deedictiterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deedictiterator_tp_copy_assign),null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deedictiterator_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deedictiterator_tp_cmp_lo),
  member(&_deedictiterator_tp_cmp_le),
  member(&_deedictiterator_tp_cmp_eq),
  member(&_deedictiterator_tp_cmp_ne),
  member(&_deedictiterator_tp_cmp_gr),
  member(&_deedictiterator_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,
  member(&_deedictvaluesiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deedictiterator_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};
DeeTypeObject DeeDictKeysIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("dict.keys.iterator"),null,null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeDictIteratorObject),null,null,
  member(&_deedictiterator_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deedictiterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deedictiterator_tp_copy_assign),null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deedictiterator_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deedictiterator_tp_cmp_lo),
  member(&_deedictiterator_tp_cmp_le),
  member(&_deedictiterator_tp_cmp_eq),
  member(&_deedictiterator_tp_cmp_ne),
  member(&_deedictiterator_tp_cmp_gr),
  member(&_deedictiterator_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,
  member(&_deedictkeysiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deedictiterator_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};
DeeTypeObject DeeDictItemsIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("dict.items.iterator"),null,null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeDictIteratorObject),null,null,
  member(&_deedictiterator_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deedictiterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deedictiterator_tp_copy_assign),null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deedictiterator_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deedictiterator_tp_cmp_lo),
  member(&_deedictiterator_tp_cmp_le),
  member(&_deedictiterator_tp_cmp_eq),
  member(&_deedictiterator_tp_cmp_ne),
  member(&_deedictiterator_tp_cmp_gr),
  member(&_deedictiterator_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,
  member(&_deedictitemsiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deedictiterator_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};












DEE_A_RET_EXCEPT(-1) int DeeDict_TInsertSequence(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self,
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DEE_ASSERT(DeeObject_Check(sequence));
 DEE_ASSERT(DeeObject_Check(tp_sequence) && DeeType_Check(tp_sequence));
 DEE_ASSERT(DeeObject_InstanceOf(sequence,tp_sequence));
 return DeeHashMap_TInsertSequenceWithLock(
  DeeDict_MAP(self),tp_sequence,sequence,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) int DeeDict_InsertIterator(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *iterator) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DEE_ASSERT(DeeObject_Check(iterator));
 return DeeHashMap_InsertIteratorWithLock(
  DeeDict_MAP(self),iterator,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) int DeeDict_Traverse(
 DEE_A_IN_OBJECT(DeeDictObject) const *self,
 DEE_A_IN DeeDictTraverseFunc trav,
 DEE_A_IN_OPT void *closure) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 result = DeeHashMap_TraverseWithLock(
  DeeDict_MAP(self),trav,closure,DeeDict_LOCK(self));
 DeeDict_RELEASE(self);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeDict_TraverseKeys(
 DEE_A_IN_OBJECT(DeeDictObject) const *self,
 DEE_A_IN DeeObjectIterateFunc trav, DEE_A_IN_OPT void *closure) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 result = DeeHashMap_TraverseKeysWithLock(
  DeeDict_MAP(self),trav,closure,DeeDict_LOCK(self));
 DeeDict_RELEASE(self);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeDict_TraverseItems(
 DEE_A_IN_OBJECT(DeeDictObject) const *self,
 DEE_A_IN DeeObjectIterateFunc trav, DEE_A_IN_OPT void *closure) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 result = DeeHashMap_TraverseItemsWithLock(
  DeeDict_MAP(self),trav,closure,DeeDict_LOCK(self));
 DeeDict_RELEASE(self);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeDict_TraverseValues(
 DEE_A_IN_OBJECT(DeeDictObject) const *self,
 DEE_A_IN DeeObjectIterateFunc trav, DEE_A_IN_OPT void *closure) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 result = DeeHashMap_TraverseValuesWithLock(
  DeeDict_MAP(self),trav,closure,DeeDict_LOCK(self));
 DeeDict_RELEASE(self);
 return result;
}
void DeeDict_Clear(DEE_A_INOUT_OBJECT(DeeDictObject) *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeHashMap_ClearWithLock(DeeDict_MAP(self),DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeDict_BucketSize(
 DEE_A_IN_OBJECT(DeeDictObject) const *self,
 DEE_A_IN Dee_ssize_t bucket) {
 Dee_size_t result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 if (!DeeDict_SIZE(self)) {
  DeeDict_RELEASE(self);
  DeeError_ValueError_EmptyList(&DeeDict_Type);
  return (Dee_size_t)-1;
 }
 DEE_PRIVATE_CLAMP_INDEX(bucket,DeeDict_SIZE(self));
 result = DeeHashMap_BucketSize(DeeDict_MAP(self),(Dee_size_t)bucket);
 DeeDict_RELEASE(self);
 return result;
}
DEE_A_RET_WUNUSED Dee_size_t DeeDict_Size(
 DEE_A_IN_OBJECT(DeeDictObject) const *self) {
 Dee_size_t result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 result = DeeDict_SIZE(self);
 DeeDict_RELEASE(self);
 return result;
}
DEE_A_RET_WUNUSED Dee_size_t DeeDict_BucketCount(
 DEE_A_IN_OBJECT(DeeDictObject) const *self) {
 Dee_size_t result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 result = DeeDict_BUCKET_COUNT(self);
 DeeDict_RELEASE(self);
 return result;
}
DEE_A_RET_WUNUSED double DeeDict_GetMaxLoadFactor(
 DEE_A_IN_OBJECT(DeeDictObject) const *self) {
 double result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 result = DeeDict_GET_MAX_LOAD_FACTOR(self);
 DeeDict_RELEASE(self);
 return result;
}
void DeeDict_SetMaxLoadFactor(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self,
 DEE_A_IN double v) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 DeeDict_SET_MAX_LOAD_FACTOR(self,v);
 DeeDict_RELEASE(self);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_Pop(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT_OPT DeeObject *def) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_PopWithLock(DeeDict_MAP(self),key,def,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_PopWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT_OPT DeeObject *def) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_PopWithKnownBucketAndLock(DeeDict_MAP(self),key,bucket,def,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_PopString(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key,
 DEE_A_INOUT_OPT DeeObject *def) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_PopStringWithLock(DeeDict_MAP(self),key,def,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_PopStringWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT_OPT DeeObject *def) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_PopStringWithKnownBucketAndLock(DeeDict_MAP(self),key,bucket,def,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_SetDefault(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT DeeObject *def) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_SetDefaultWithLock(DeeDict_MAP(self),key,def,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_SetDefaultWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_SetDefaultWithKnownBucketAndLock(DeeDict_MAP(self),key,bucket,def,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_SetDefaultString(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key,
 DEE_A_INOUT DeeObject *def) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_SetDefaultStringWithLock(DeeDict_MAP(self),key,def,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_SetDefaultStringWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_SetDefaultStringWithKnownBucketAndLock(DeeDict_MAP(self),key,bucket,def,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_GetDefault(
 DEE_A_INOUT_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT DeeObject *def) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_GetDefaultWithLock(DeeDict_MAP(self),key,def,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_GetDefaultWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_GetDefaultWithKnownBucketAndLock(
  DeeDict_MAP(self),key,bucket,def,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_GetDefaultString(
 DEE_A_INOUT_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key,
 DEE_A_INOUT DeeObject *def) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_GetDefaultStringWithLock(DeeDict_MAP(self),key,def,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_GetDefaultStringWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *def) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_GetDefaultStringWithKnownBucketAndLock(
  DeeDict_MAP(self),key,bucket,def,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) int DeeDict_PopSomething(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self,
 DEE_A_REF DEE_A_OUT DeeObject **key,
 DEE_A_REF DEE_A_OUT DeeObject **item) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DEE_ASSERT(key && item);
 DeeDict_ACQUIRE(self);
 result = DeeHashMap_PopSomething(DeeDict_MAP(self),key,item);
 DeeDict_RELEASE(self);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeDict_FindKeyIndex(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_FindKeyIndexWithLock(DeeDict_MAP(self),key,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeDict_FindKeyIndexWithKnownBucket(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_FindKeyIndexWithKnownBucketAndLock(DeeDict_MAP(self),key,bucket,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeDict_FindKeyIndexString(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_FindKeyIndexStringWithLock(DeeDict_MAP(self),key,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeDict_FindKeyIndexStringWithKnownBucket(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_FindKeyIndexStringWithKnownBucketAndLock(
  DeeDict_MAP(self),key,bucket,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeDict_FindItemIndex(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_FindItemIndexWithLock(DeeDict_MAP(self),item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeDict_FindKeyItemIndex(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_FindKeyItemIndexWithLock(DeeDict_MAP(self),key,item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeDict_FindKeyItemIndexWithKnownBucket(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_FindKeyItemIndexWithKnownBucketAndLock(
  DeeDict_MAP(self),key,bucket,item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeDict_FindKeyItemIndexString(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_FindKeyItemIndexStringWithLock(DeeDict_MAP(self),key,item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeDict_FindKeyItemIndexStringWithKnownBucket(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_FindKeyItemIndexStringWithKnownBucketAndLock(
  DeeDict_MAP(self),key,bucket,item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeDict_IndexKeyIndex(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_IndexKeyIndexWithLock(DeeDict_MAP(self),key,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeDict_IndexKeyIndexWithKnownBucket(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_IndexKeyIndexWithKnownBucketAndLock(
  DeeDict_MAP(self),key,bucket,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeDict_IndexKeyIndexString(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_IndexKeyIndexStringWithLock(DeeDict_MAP(self),key,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeDict_IndexKeyIndexStringWithKnownBucket(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_IndexKeyIndexStringWithKnownBucketAndLock(
  DeeDict_MAP(self),key,bucket,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeDict_IndexItemIndex(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_IndexItemIndexWithLock(DeeDict_MAP(self),item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeDict_IndexKeyItemIndex(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_IndexKeyItemIndexWithLock(DeeDict_MAP(self),key,item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeDict_IndexKeyItemIndexWithKnownBucket(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_IndexKeyItemIndexWithKnownBucketAndLock(
  DeeDict_MAP(self),key,bucket,item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeDict_IndexKeyItemIndexString(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_IndexKeyItemIndexStringWithLock(
  DeeDict_MAP(self),key,item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeDict_IndexKeyItemIndexStringWithKnownBucket(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_IndexKeyItemIndexStringWithKnownBucketAndLock(
  DeeDict_MAP(self),key,bucket,item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeDict_CountItem(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_CountItemWithLock(DeeDict_MAP(self),item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_GetItem(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_GetItemWithLock(DeeDict_MAP(self),key,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_GetItemWithKnownBucket(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_GetItemWithKnownBucketAndLock(DeeDict_MAP(self),key,bucket,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_GetItemEx(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_REF DEE_A_OUT DeeObject **result) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_GetItemExWithLock(DeeDict_MAP(self),key,result,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_GetItemExWithKnownBucket(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_REF DEE_A_OUT DeeObject **result) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_GetItemExWithKnownBucketAndLock(
  DeeDict_MAP(self),key,bucket,result,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_GetItemExString(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key,
 DEE_A_REF DEE_A_OUT DeeObject **result) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_GetItemExStringWithLock(
  DeeDict_MAP(self),key,result,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_GetItemExStringWithKnownBucket(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_REF DEE_A_OUT DeeObject **result) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_GetItemExStringWithKnownBucketAndLock(
  DeeDict_MAP(self),key,bucket,result,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_GetItemString(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_GetItemStringWithLock(DeeDict_MAP(self),key,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_GetItemStringWithKnownBucket(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_GetItemStringWithKnownBucketAndLock(
  DeeDict_MAP(self),key,bucket,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDict_HasKey(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_HasKeyWithLock(DeeDict_MAP(self),key,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDict_HasKeyWithKnownBucket(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_HasKeyWithKnownBucketAndLock(DeeDict_MAP(self),key,bucket,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDict_HasKeyString(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_HasKeyStringWithLock(DeeDict_MAP(self),key,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDict_HasKeyStringWithKnownBucket(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_HasKeyStringWithKnownBucketAndLock(DeeDict_MAP(self),key,bucket,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDict_HasItem(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_HasItemWithLock(DeeDict_MAP(self),item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDict_HasKeyItem(
 DEE_A_IN_OBJECT(DeeDictObject) const *self,
 DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_HasKeyItemWithLock(DeeDict_MAP(self),key,item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDict_HasKeyItemWithKnownBucket(
 DEE_A_IN_OBJECT(DeeDictObject) const *self,
 DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_HasKeyItemWithKnownBucketAndLock(DeeDict_MAP(self),key,bucket,item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDict_HasKeyItemString(
 DEE_A_IN_OBJECT(DeeDictObject) const *self,
 DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_HasKeyItemStringWithLock(DeeDict_MAP(self),key,item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDict_HasKeyItemStringWithKnownBucket(
 DEE_A_IN_OBJECT(DeeDictObject) const *self,
 DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_HasKeyItemStringWithKnownBucketAndLock(
  DeeDict_MAP(self),key,bucket,item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_DelItem(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_DelItemWithLock(DeeDict_MAP(self),key,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_DelItemWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_DelItemWithKnownBucketAndLock(DeeDict_MAP(self),key,bucket,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_DelItemWithError(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_DelItemWithErrorAndLock(DeeDict_MAP(self),key,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_DelItemWithErrorAndKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_DelItemWithErrorAndKnownBucketAndLock(DeeDict_MAP(self),key,bucket,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_DelItemString(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_DelItemStringWithLock(DeeDict_MAP(self),key,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_DelItemStringWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_DelItemStringWithKnownBucketAndLock(DeeDict_MAP(self),key,bucket,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_DelItemStringWithError(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_DelItemStringWithErrorAndLock(DeeDict_MAP(self),key,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_DelItemStringWithErrorAndKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_DelItemStringWithErrorAndKnownBucketAndLock(DeeDict_MAP(self),key,bucket,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) int DeeDict_SetItem(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_SetItemWithLock(DeeDict_MAP(self),key,item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) int DeeDict_SetItemWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_SetItemWithKnownBucketAndLock(DeeDict_MAP(self),key,bucket,item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) int DeeDict_SetItemString(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_SetItemStringWithLock(DeeDict_MAP(self),key,item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) int DeeDict_SetItemStringWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key,
 DEE_A_IN Dee_hash_t bucket, DEE_A_INOUT DeeObject *item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_SetItemStringWithKnownBucketAndLock(DeeDict_MAP(self),key,bucket,item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_SetNewItem(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_SetNewItemWithLock(DeeDict_MAP(self),key,item,old_item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_SetNewItemWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket,
 DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_SetNewItemWithKnownBucketAndLock(DeeDict_MAP(self),key,bucket,item,old_item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_SetNewItemString(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key,
 DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_SetNewItemStringWithLock(DeeDict_MAP(self),key,item,old_item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_SetNewItemStringWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket,
 DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_SetNewItemStringWithKnownBucketAndLock(DeeDict_MAP(self),key,bucket,item,old_item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_SetOldItem(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key,
 DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_SetOldItemWithLock(DeeDict_MAP(self),key,item,old_item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_SetOldItemWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_INOUT DeeObject *key, DEE_A_IN Dee_hash_t bucket,
 DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_SetOldItemWithKnownBucketAndLock(DeeDict_MAP(self),key,bucket,item,old_item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_SetOldItemString(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key,
 DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_SetOldItemStringWithLock(DeeDict_MAP(self),key,item,old_item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDict_SetOldItemStringWithKnownBucket(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN_Z char const *key, DEE_A_IN Dee_hash_t bucket,
 DEE_A_INOUT DeeObject *item, DEE_A_REF DEE_A_OUT_OPT DeeObject **old_item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_SetOldItemStringWithKnownBucketAndLock(DeeDict_MAP(self),key,bucket,item,old_item,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_GetKeyIndex(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_ssize_t index) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 if (!DeeDict_SIZE(self)) {
  DeeDict_RELEASE(self);
  DeeError_ValueError_EmptyList(&DeeDictKeys_Type);
  return NULL;
 } else {
  DEE_PRIVATE_CLAMP_INDEX(index,DeeDict_SIZE(self));
  result = DeeHashMap_GetKeyIndex(DeeDict_MAP(self),(Dee_size_t)index);
  Dee_INCREF(result);
 }
 DeeDict_RELEASE(self);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_GetItemIndex(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_ssize_t index) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 if (!DeeDict_SIZE(self)) {
  DeeDict_RELEASE(self);
  DeeError_ValueError_EmptyList(&DeeDictItems_Type);
  return NULL;
 } else {
  DEE_PRIVATE_CLAMP_INDEX(index,DeeDict_SIZE(self));
  result = DeeHashMap_GetItemIndex(DeeDict_MAP(self),(Dee_size_t)index);
  Dee_INCREF(result);
 }
 DeeDict_RELEASE(self);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *DeeDict_GetValueIndex(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_ssize_t index) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 if (!DeeDict_SIZE(self)) {
  DeeDict_RELEASE(self);
  DeeError_ValueError_EmptyList(&DeeDictValues_Type);
  return NULL;
 } else {
  DEE_PRIVATE_CLAMP_INDEX(index,DeeDict_SIZE(self));
  result = DeeHashMap_GetValueIndex(DeeDict_MAP(self),(Dee_size_t)index);
 }
 DeeDict_RELEASE(self);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_GetKeyIndexAt(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_size_t index) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeObject *result;
 DeeDict_ACQUIRE(self);
 if (index >= DeeDict_SIZE(self)) {
  DeeDict_RELEASE(self);
  DeeError_IndexError((DeeObject *)self,index);
  return NULL;
 } else {
  Dee_INCREF(result = _DeeHashMap_GetNodeIndex(DeeDict_MAP(self),index)->hmn_key);
 }
 DeeDict_RELEASE(self);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_GetItemIndexAt(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_size_t index) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeObject *result;
 DeeDict_ACQUIRE(self);
 if (index >= DeeDict_SIZE(self)) {
  DeeDict_RELEASE(self);
  DeeError_IndexError((DeeObject *)self,index);
  return NULL;
 } else {
  Dee_INCREF(result = _DeeHashMap_GetNodeIndex(DeeDict_MAP(self),index)->hmn_item);
 }
 DeeDict_RELEASE(self);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *DeeDict_GetValueIndexAt(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_size_t index) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 if (index >= DeeDict_SIZE(self)) {
  DeeDict_RELEASE(self);
  DeeError_IndexError((DeeObject *)self,index);
  return NULL;
 } else result = DeeHashMap_GetValueIndex(DeeDict_MAP(self),index);
 DeeDict_RELEASE(self);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeDict_GetKeyRangeIndex(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_ssize_t lo, DEE_A_IN Dee_ssize_t hi) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 DEE_PRIVATE_CLAMP_RANGE(lo,hi,DeeDict_SIZE(self));
 result = DeeHashMap_GetKeyRangeIndex(DeeDict_MAP(self),(Dee_size_t)lo,(Dee_size_t)hi);
 DeeDict_RELEASE(self);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeDict_GetItemRangeIndex(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_ssize_t lo, DEE_A_IN Dee_ssize_t hi) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 DEE_PRIVATE_CLAMP_RANGE(lo,hi,DeeDict_SIZE(self));
 result = DeeHashMap_GetItemRangeIndex(DeeDict_MAP(self),(Dee_size_t)lo,(Dee_size_t)hi);
 DeeDict_RELEASE(self);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeDict_GetValueRangeIndex(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_IN Dee_ssize_t lo, DEE_A_IN Dee_ssize_t hi) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 DEE_PRIVATE_CLAMP_RANGE(lo,hi,DeeDict_SIZE(self));
 result = DeeHashMap_GetValueRangeIndex(DeeDict_MAP(self),(Dee_size_t)lo,(Dee_size_t)hi);
 DeeDict_RELEASE(self);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeDict_DelItemIndex(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN Dee_ssize_t index) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 if ((result = DeeHashMap_DelItemIndexWithLock(
  DeeDict_MAP(self),index,DeeDict_LOCK(self))) == 1) {
  DeeError_ValueError_EmptyList(&DeeDict_Type);
  result = -1;
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeDict_DelItemRangeIndex(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_DelItemRangeIndexWithLock(DeeDict_MAP(self),lo,hi,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT(-1) int DeeDict_SetItemIndex(
 DEE_A_INOUT_OBJECT(DeeDictObject) *self,
 DEE_A_IN Dee_ssize_t index, DEE_A_INOUT DeeObject *item) {
 DeeObject *old_value;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 if (!DeeDict_SIZE(self)) {
  DeeDict_RELEASE(self);
  DeeError_ValueError_EmptyList(&DeeDict_Type);
  return -1;
 }
 DEE_PRIVATE_CLAMP_INDEX(index,DeeDict_SIZE(self));
 old_value = DeeHashMap_ExchangeItemIndex(
  DeeDict_MAP(self),(Dee_size_t)index,item);
 DeeDict_RELEASE(self);
 Dee_DECREF(old_value);
 return 0;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_LocateKey(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_LocateKeyWithLock(DeeDict_MAP(self),pred,DeeDict_LOCK(self));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeDict_LocateItem(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_LocateItemWithLock(DeeDict_MAP(self),pred,DeeDict_LOCK(self));
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *DeeDict_LocateValue(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_LocateValueWithLock(DeeDict_MAP(self),pred,DeeDict_LOCK(self));
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictObject) *DeeDict_LocateAll(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 if ((result = DeeDict_New()) == NULL) return NULL;
 if (DeeHashMap_LocateAllWithLock(DeeDict_MAP(self),
  DeeDict_MAP(result),pred,DeeDict_LOCK(self)) != 0) Dee_CLEAR(result);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeDict_LocateAllKeys(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_LocateAllKeysWithLock(DeeDict_MAP(self),pred,DeeDict_LOCK(self));
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeDict_LocateAllItems(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_LocateAllItemsWithLock(DeeDict_MAP(self),pred,DeeDict_LOCK(self));
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeDict_LocateAllValues(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_LocateAllValuesWithLock(DeeDict_MAP(self),pred,DeeDict_LOCK(self));
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeDictObject) *DeeDict_Transform(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 if ((result = DeeDict_New()) == NULL) return NULL;
 if (DeeHashMap_TransformWithLock(DeeDict_MAP(self),
  DeeDict_MAP(result),pred,DeeDict_LOCK(self)) != 0) Dee_CLEAR(result);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeDict_TransformKeys(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_TransformKeysWithLock(DeeDict_MAP(self),pred,DeeDict_LOCK(self));
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeDict_TransformItems(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_TransformItemsWithLock(DeeDict_MAP(self),pred,DeeDict_LOCK(self));
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeDict_TransformValues(
 DEE_A_IN_OBJECT(DeeDictObject) const *self, DEE_A_INOUT DeeObject *pred) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 return DeeHashMap_TransformValuesWithLock(DeeDict_MAP(self),pred,DeeDict_LOCK(self));
}
DEE_A_RET_NOEXCEPT(0) int DeeDict_Front(
 DEE_A_IN_OBJECT(DeeDictObject) const *self,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **key,
 DEE_A_OUT_OPT Dee_hash_t *bucket,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 if (!DeeDict_SIZE(self)) { DeeDict_RELEASE(self); return 0; }
 DeeHashMap_Front(DeeDict_MAP(self),key,bucket,item);
 if (key) Dee_INCREF(*key);
 if (item) Dee_INCREF(*item);
 DeeDict_RELEASE(self);
 return 1;
}
DEE_A_RET_NOEXCEPT(0) int DeeDict_Back(
 DEE_A_IN_OBJECT(DeeDictObject) const *self,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **key,
 DEE_A_OUT_OPT Dee_hash_t *bucket,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **item) {
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DeeDict_ACQUIRE(self);
 if (!DeeDict_SIZE(self)) { DeeDict_RELEASE(self); return 0; }
 DeeHashMap_Back(DeeDict_MAP(self),key,bucket,item);
 if (key) Dee_INCREF(*key);
 if (item) Dee_INCREF(*item);
 DeeDict_RELEASE(self);
 return 1;
}


DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDict_HasKeyPred(
 DEE_A_IN_OBJECT(DeeDictObject) const *self,
 DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *pred) {
 DeeHashMap_TRAVERSE_SAFE_VARS;
 struct _DeeHashMapNode *node; int error;
 DeeObject *node_key,*pred_args,*pred_result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DEE_ASSERT(DeeObject_Check(key));
 DEE_ASSERT(DeeObject_Check(pred));
 DeeDict_ACQUIRE(self);
 DeeHashMap_TRAVERSE_SAFE(node,DeeDict_MAP(self)) {
  Dee_INCREF(node_key = node->hmn_key);
  DeeDict_RELEASE(self);
  pred_args = DeeTuple_Pack(2,key,node_key);
  Dee_DECREF(node_key);
  if DEE_UNLIKELY(!pred_args) return -1;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) return -1;
  error = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if DEE_UNLIKELY(error != 0) return error; // Error, or found
  DeeDict_ACQUIRE(self);
 }
 DeeDict_RELEASE(self);
 return 0;
}
DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDict_HasItemPred(
 DEE_A_IN_OBJECT(DeeDictObject) const *self,
 DEE_A_INOUT DeeObject *item, DEE_A_INOUT DeeObject *pred) {
 DeeHashMap_TRAVERSE_SAFE_VARS;
 struct _DeeHashMapNode *node; int error;
 DeeObject *node_item,*pred_args,*pred_result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DEE_ASSERT(DeeObject_Check(item));
 DEE_ASSERT(DeeObject_Check(pred));
 DeeDict_ACQUIRE(self);
 DeeHashMap_TRAVERSE_SAFE(node,DeeDict_MAP(self)) {
  Dee_INCREF(node_item = node->hmn_item);
  DeeDict_RELEASE(self);
  pred_args = DeeTuple_Pack(2,item,node_item);
  Dee_DECREF(node_item);
  if DEE_UNLIKELY(!pred_args) return -1;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) return -1;
  error = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if DEE_UNLIKELY(error != 0) return error; // Error, or found
  DeeDict_ACQUIRE(self);
 }
 DeeDict_RELEASE(self);
 return 0;
}
DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeDict_HasKeyItemPred(
 DEE_A_IN_OBJECT(DeeDictObject) const *self,
 DEE_A_INOUT DeeObject *keyitem, DEE_A_INOUT DeeObject *pred) {
 DeeHashMap_TRAVERSE_SAFE_VARS;
 struct _DeeHashMapNode *node; int error;
 DeeObject *node_key,*node_item,*node_keyitem,*pred_args,*pred_result;
 DEE_ASSERT(DeeObject_Check(self) && DeeDict_Check(self));
 DEE_ASSERT(DeeObject_Check(keyitem));
 DEE_ASSERT(DeeObject_Check(pred));
 DeeDict_ACQUIRE(self);
 DeeHashMap_TRAVERSE_SAFE(node,DeeDict_MAP(self)) {
  Dee_INCREF(node_key = node->hmn_key);
  Dee_INCREF(node_item = node->hmn_item);
  DeeDict_RELEASE(self);
  node_keyitem = DeeTuple_Pack(2,node_key,node_item);
  Dee_DECREF(node_item);
  Dee_DECREF(node_key);
  if DEE_UNLIKELY(!node_keyitem) return -1;
  pred_args = DeeTuple_Pack(2,node_keyitem,node_item);
  Dee_DECREF(node_keyitem);
  if DEE_UNLIKELY(!pred_args) return -1;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) return -1;
  error = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if DEE_UNLIKELY(error != 0) return error; // Error, or found
  DeeDict_ACQUIRE(self);
 }
 DeeDict_RELEASE(self);
 return 0;
}




#ifndef __INTELLISENSE__
#ifndef DEE_WITHOUT_THREADS
#define WITH_LOCK
#include "dict.impl.hashmap.inl"
#endif
#include "dict.impl.hashmap.inl"
#endif

#undef DeeHashMap_GetKeyIndex
DEE_A_RET_NEVER_NULL DeeObject *DeeHashMap_GetKeyIndex(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_size_t index) {
 DEE_ASSERT(self);
 return _DeeHashMap_GetNodeIndex(self,index)->hmn_key;
}
#undef DeeHashMap_GetItemIndex
DEE_A_RET_NEVER_NULL DeeObject *DeeHashMap_GetItemIndex(
 DEE_A_IN struct DeeHashMap const *self, DEE_A_IN Dee_size_t index) {
 DEE_ASSERT(self);
 return _DeeHashMap_GetNodeIndex(self,index)->hmn_item;
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_DICT_C */
