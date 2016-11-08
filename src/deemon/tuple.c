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
#ifndef GUARD_DEEMON_TUPLE_C
#define GUARD_DEEMON_TUPLE_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include DEE_INCLUDE_MEMORY_API()

// /include/*
#include <deemon/bool.h>
#include <deemon/cell.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/gc.h>
#include <deemon/generic_vtable.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/type_util.h>
#include <deemon/optional/object_visit.h>
#include <deemon/integer.h>
#include <deemon/iterator.h>
#include <deemon/list.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/string.h>
#include <deemon/structured.h>
#include <deemon/tuple.h>

// /src/*
#include <deemon/marshal_data.h>
#include <deemon/runtime/builtin_functions.h>

DEE_DECL_BEGIN

#define DEE_PRIVATE_SIZEOF_TUPLE(n_elem)\
 (Dee_OFFSETOF(DeeTupleObject,t_elem)+(n_elem)*sizeof(DeeObject*))

// 8 is already really high, as most tuples hang around 1-5
#define DEE_TUPLE_POOL_MAX_TRACK_LENGTH 8

#ifndef DEE_TUPLE_POOL_MAX_TRACK_LENGTH
#define DEE_TUPLE_POOL_MAX_TRACK_LENGTH 0
#endif

#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
static struct DeeAtomicMutex _DeeTuplePool_Lock = DeeAtomicMutex_INIT();
union _DeeTuplePool { union _DeeTuplePool *tp_next; };
static union _DeeTuplePool *_DeeTuplePool_Pool[DEE_TUPLE_POOL_MAX_TRACK_LENGTH];
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */


Dee_size_t DeeTuple_ClearPool(void) {
 Dee_size_t result = 0;
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 union _DeeTuplePool **iter,*elem,*tp_next;
#define pool_end  (_DeeTuplePool_Pool+DEE_TUPLE_POOL_MAX_TRACK_LENGTH)
 DeeAtomicMutex_Acquire(&_DeeTuplePool_Lock);
 DEE_ASSERT(_DeeTuplePool_Pool[0] == NULL); // no free empty tuples
 iter = _DeeTuplePool_Pool+1;
 while DEE_LIKELY(iter != pool_end) {
#ifdef DEE_DEBUG
  Dee_size_t pool_size = 0;
#endif
  if DEE_LIKELY((elem = *iter) != NULL) {
   *iter = NULL;
   while (1) {
    tp_next = elem->tp_next;
    free_nn(elem);
#ifdef DEE_DEBUG
    ++pool_size;
#endif
    ++result;
    if DEE_UNLIKELY(!tp_next) break;
    elem = tp_next;
   }
  }
  ++iter;
#ifdef DEE_DEBUG
  DEE_LVERBOSE3("[TuplePool %Id] Cleared: %Iu\n",iter-_DeeTuplePool_Pool,pool_size);
#endif
 }
 DeeAtomicMutex_Release(&_DeeTuplePool_Lock);
#undef pool_end
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
 return result;
}

#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
static void _deetuple_tp_free(DeeTypeObject *DEE_UNUSED(tp), DeeTupleObject *self) {
 Dee_size_t tuple_size;
 DEE_ASSERT(DeeTuple_SIZE(self) != 0);
 if DEE_LIKELY((tuple_size = DeeTuple_SIZE(self)) < DEE_TUPLE_POOL_MAX_TRACK_LENGTH) {
  DeeAtomicMutex_Acquire(&_DeeTuplePool_Lock);
  ((union _DeeTuplePool *)self)->tp_next = _DeeTuplePool_Pool[tuple_size];
  _DeeTuplePool_Pool[tuple_size] = ((union _DeeTuplePool *)self);
  DeeAtomicMutex_Release(&_DeeTuplePool_Lock);
 } else free_nn(self);
}
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */









DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *
_DeeTuple_NewUnsafe(DEE_A_IN Dee_size_t n) {
 DeeTupleObject *result;
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 union _DeeTuplePool **pool;
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
 if DEE_UNLIKELY(!n) DeeReturn_EmptyTuple;
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 if DEE_LIKELY(n < DEE_TUPLE_POOL_MAX_TRACK_LENGTH) {
  DeeAtomicMutex_Acquire(&_DeeTuplePool_Lock);
  if DEE_LIKELY((*(pool = _DeeTuplePool_Pool+n)) != NULL) {
   result = (DeeTupleObject *)*pool;
   *pool = (*pool)->tp_next;
   DeeAtomicMutex_Release(&_DeeTuplePool_Lock);
  } else {
   DeeAtomicMutex_Release(&_DeeTuplePool_Lock);
   goto alloc_new;
  }
 } else
 {
alloc_new:
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
  if DEE_UNLIKELY((result = (DeeTupleObject *)DeeObject_Mallocf(DEE_PRIVATE_SIZEOF_TUPLE(n),
   "tuple(size: %lu)",(unsigned long)n)) == NULL) return NULL;
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 }
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
 DeeObject_INIT(result,&DeeTuple_Type);
 result->t_size = n;
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *
_DeeTuple_WeakNewUnsafe(DEE_A_IN Dee_size_t n) {
 DeeTupleObject *result;
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 union _DeeTuplePool **pool;
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
 if DEE_UNLIKELY(!n) DeeReturn_EmptyTuple;
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 if DEE_LIKELY(n < DEE_TUPLE_POOL_MAX_TRACK_LENGTH) {
  DeeAtomicMutex_Acquire(&_DeeTuplePool_Lock);
  if DEE_LIKELY((*(pool = _DeeTuplePool_Pool+n)) != NULL) {
   result = (DeeTupleObject *)*pool;
   *pool = (*pool)->tp_next;
   DeeAtomicMutex_Release(&_DeeTuplePool_Lock);
  } else {
   DeeAtomicMutex_Release(&_DeeTuplePool_Lock);
   goto alloc_new;
  }
 } else
 {
alloc_new:
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
  if DEE_UNLIKELY((result = (DeeTupleObject *)DeeObject_WeakMalloc(
   DEE_PRIVATE_SIZEOF_TUPLE(n))) == NULL) return NULL;
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 }
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
 DeeObject_INIT(result,&DeeTuple_Type);
 result->t_size = DEE_PRIVATE_SIZEOF_TUPLE(n);
 return (DeeObject *)result;
}

DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeTupleObject) *
_DeeTuple_TryNewUnsafe(DEE_A_IN Dee_size_t n) {
 DeeTupleObject *result;
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 union _DeeTuplePool **pool;
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
 if DEE_UNLIKELY(!n) DeeReturn_EmptyTuple;
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 if DEE_LIKELY(n < DEE_TUPLE_POOL_MAX_TRACK_LENGTH) {
  DeeAtomicMutex_Acquire(&_DeeTuplePool_Lock);
  if DEE_LIKELY((*(pool = _DeeTuplePool_Pool+n)) != NULL) {
   result = (DeeTupleObject *)*pool;
   *pool = (*pool)->tp_next;
   DeeAtomicMutex_Release(&_DeeTuplePool_Lock);
  } else {
   DeeAtomicMutex_Release(&_DeeTuplePool_Lock);
   goto alloc_new;
  }
 } else
 {
alloc_new:
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
  if DEE_UNLIKELY((result = (DeeTupleObject *)DeeObject_TryMallocf(
   DEE_PRIVATE_SIZEOF_TUPLE(n),"tuple(size: %u)",
   (unsigned int)n)) == NULL) return NULL;
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 }
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
 DeeObject_INIT(result,&DeeTuple_Type);
 result->t_size = n;
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeTupleObject) *
_DeeTuple_TryWeakNewUnsafe(DEE_A_IN Dee_size_t n) {
 DeeTupleObject *result;
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 union _DeeTuplePool **pool;
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
 if DEE_UNLIKELY(!n) DeeReturn_EmptyTuple;
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 if DEE_LIKELY(n < DEE_TUPLE_POOL_MAX_TRACK_LENGTH) {
  DeeAtomicMutex_Acquire(&_DeeTuplePool_Lock);
  if DEE_LIKELY((*(pool = _DeeTuplePool_Pool+n)) != NULL) {
   result = (DeeTupleObject *)*pool;
   *pool = (*pool)->tp_next;
   DeeAtomicMutex_Release(&_DeeTuplePool_Lock);
  } else {
   DeeAtomicMutex_Release(&_DeeTuplePool_Lock);
   goto alloc_new;
  }
 } else
 {
alloc_new:
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
  if DEE_UNLIKELY((result = (DeeTupleObject *)DeeObject_TryWeakMalloc(
   DEE_PRIVATE_SIZEOF_TUPLE(n))) == NULL) return NULL;
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 }
#endif /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
 DeeObject_INIT(result,&DeeTuple_Type);
 result->t_size = n;
 return (DeeObject *)result;
}





DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *
DeeTuple_NewFromVector(DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p) {
 DeeObject *result;
 if DEE_LIKELY((result = _DeeTuple_NewUnsafe(n)) != NULL) {
  DeeObject **dst = DeeTuple_ELEM(result);
  while DEE_LIKELY(n--) Dee_INCREF(*dst++ = *p++);
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *
DeeTuple_Pack(DEE_A_IN Dee_size_t n, ...) {
 va_list args;
 DeeObject *result;
 DEE_VA_START(args,n);
 result = _DeeTuple_VPack(n,args);
 DEE_VA_END(args);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *
_DeeTuple_VPack(DEE_A_IN Dee_size_t n, DEE_A_IN va_list args) {
 DeeObject *result,**elem;
 if DEE_UNLIKELY(!n) DeeReturn_EmptyTuple;
 if DEE_LIKELY((result = _DeeTuple_NewUnsafe(n)) != NULL) {
  elem = DeeTuple_ELEM(result);
  do Dee_INCREF(*elem++ = DEE_VA_ARG(args,DeeObject *));
  while DEE_LIKELY(--n);
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int _DeeTuple_ResizeUnsafe(
 DEE_A_INOUT_OBJECT(DeeTupleObject) **self, DEE_A_IN Dee_size_t n) {
 DEE_ASSERT(self);
 if DEE_LIKELY(*self) {
  DeeTupleObject *new_tuple,*old_tuple = (DeeTupleObject *)*self;
  Dee_size_t i,old_size;
  DEE_ASSERT(DeeObject_Check(old_tuple) && DeeTuple_Check(old_tuple));
  old_size = DeeTuple_SIZE(old_tuple);
  if DEE_UNLIKELY(n == old_size) return 0;
  if DEE_LIKELY(DeeObject_IS_UNIQUE(old_tuple) &&
                old_tuple != (DeeTupleObject *)Dee_EmptyTuple) {
   if (n < old_size) {
    for (i = n; DEE_LIKELY(i < old_size); ++i) {
     Dee_DECREF(DeeTuple_GET(old_tuple,i));
    }
   }
   if DEE_UNLIKELY((new_tuple = (DeeTupleObject *)DeeObject_Realloc(
    old_tuple,DEE_PRIVATE_SIZEOF_TUPLE(n))
    ) == NULL) return -1;
   new_tuple->t_size = n;
#if 0
   if (n > old_size) for (i = old_size; i < n; ++i)
    DeeTuple_SET(new_tuple,i,NULL);
#endif
  } else {
   if DEE_UNLIKELY((new_tuple = (DeeTupleObject *)_DeeTuple_NewUnsafe(n)) == NULL) return -1;
   if (old_size < n) n = old_size;
   for (i = 0; DEE_LIKELY(i < n); ++i) {
    DeeObject *elem = DeeTuple_GET(old_tuple,i);
    Dee_INCREF(elem);
    DeeTuple_SET(new_tuple,i,elem);
   }
   Dee_DECREF(*self);
  }
  *self = (DeeObject *)new_tuple;
  return 0;
 } else {
  if DEE_UNLIKELY((*self = _DeeTuple_NewUnsafe(n)) == NULL) return -1;
  return 0;
 }
}
DEE_A_RET_EXCEPT(-1) int DeeTuple_Resize(
 DEE_A_INOUT_OBJECT(DeeTupleObject) **self,
 DEE_A_IN Dee_size_t n, DEE_A_INOUT DeeObject *filler) {
 DEE_ASSERT(self);
 if DEE_LIKELY(*self) {
  DeeTupleObject *new_tuple,*old_tuple = (DeeTupleObject *)*self;
  Dee_size_t i,old_size;
  DEE_ASSERT(DeeObject_Check(old_tuple) && DeeTuple_Check(old_tuple));
  old_size = DeeTuple_SIZE(old_tuple);
  if DEE_UNLIKELY(n == old_size) return 0;
  if DEE_LIKELY(DeeObject_IS_UNIQUE(old_tuple) &&
                old_tuple != (DeeTupleObject *)Dee_EmptyTuple) {
   if (n < old_size) for (i = n; i < old_size; ++i)
    Dee_DECREF(DeeTuple_GET(old_tuple,i));
   if DEE_UNLIKELY((new_tuple = (DeeTupleObject *)DeeObject_Realloc(
    old_tuple,DEE_PRIVATE_SIZEOF_TUPLE(n))
    ) == NULL) return -1;
   new_tuple->t_size = n;
   if (n > old_size) {
    for (i = old_size; DEE_LIKELY(i < n); ++i) {
     Dee_INCREF(filler);
     DeeTuple_SET(new_tuple,i,filler);
    }
   }
  } else {
   if DEE_UNLIKELY((new_tuple = (DeeTupleObject *)_DeeTuple_NewUnsafe(n)) == NULL) return -1;
   if (old_size < n) n = old_size;
   for (i = 0; DEE_LIKELY(i < n); ++i) {
    DeeObject *elem = DeeTuple_GET(old_tuple,i);
    Dee_INCREF(elem);
    DeeTuple_SET(new_tuple,i,elem);
   }
   Dee_DECREF(*self);
  }
  *self = (DeeObject *)new_tuple;
  return 0;
 } else {
  if DEE_UNLIKELY((*self = _DeeTuple_NewUnsafe(n)) == NULL) return -1;
  return 0;
 }
}
DEE_A_RET_EXCEPT(-1) int DeeTuple_Append(
 DEE_A_INOUT_OBJECT(DeeTupleObject) **self, DEE_A_INOUT DeeObject *ob) {
 Dee_size_t old_size;
 old_size = DEE_LIKELY(*self) ? DeeTuple_SIZE(*self) : 0;
 if DEE_UNLIKELY(_DeeTuple_ResizeUnsafe(self,old_size+1) != 0) return -1;
 DeeTuple_SET(*self,old_size,ob);
 Dee_INCREF(ob);
 return 0;
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeTuple_AppendVector(
 DEE_A_INOUT_OBJECT(DeeTupleObject) **self,
 DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p) {
 DeeObject **dst;
 Dee_size_t old_size; if (!n) return 0;
 old_size = *self ? DeeTuple_SIZE(*self) : 0;
 if DEE_UNLIKELY(_DeeTuple_ResizeUnsafe(self,old_size+n) != 0) return -1;
 dst = DeeTuple_ELEM(*self)+old_size;
 do Dee_INCREF(*dst++ = *p++); while DEE_LIKELY(--n);
 return 0;
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeTuple_TAppendIterator(
 DEE_A_INOUT_OBJECT(DeeTupleObject) **self,
 DEE_A_IN DeeTypeObject const *tp_iterator, DEE_A_INOUT DeeObject *iterator) {
 DeeObject *elem; int temp;
 while DEE_LIKELY((temp = DeeObject_TIterNextEx(tp_iterator,iterator,&elem)) == 0) {
  temp = DeeTuple_Append(self,elem);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(temp != 0) return temp;
 }
 return DEE_UNLIKELY(temp < 0) ? temp : 0;
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeTuple_TAppendSequence(
 DEE_A_INOUT_OBJECT(DeeTupleObject) **self,
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) {
 DeeObject *iterator; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 DEE_ASSERT(DeeObject_Check(sequence));
 DEE_ASSERT(DeeObject_Check(tp_sequence) && DeeType_Check(tp_sequence));
 DEE_ASSERT(DeeObject_InstanceOf(sequence,tp_sequence));
 if (tp_sequence == &DeeTuple_Type) return DeeTuple_AppendVector(
  self,DeeTuple_SIZE(sequence),DeeTuple_ELEM(sequence));
 // TODO: Optimizations for list and cell
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_sequence,sequence)) == NULL) return -1;
 result = DeeTuple_AppendIterator(self,iterator);
 Dee_DECREF(iterator);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *
DeeTuple_DeepCopy(DEE_A_IN_OBJECT(DeeTupleObject) const *self) {
 Dee_size_t tuple_size;
 DeeObject *result,**dst_elem,**src_elem,**end;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 tuple_size = DeeTuple_SIZE(self);
 if DEE_LIKELY((result = _DeeTuple_NewUnsafe(tuple_size)) != NULL) {
  dst_elem = DeeTuple_ELEM(result);
  src_elem = DeeTuple_ELEM(self);
  end = src_elem+tuple_size;
  while DEE_LIKELY(src_elem != end) {
   DeeObject *copy;
   if DEE_UNLIKELY((copy = DeeObject_DeepCopy(*src_elem)) == NULL) {
    while DEE_LIKELY(dst_elem != DeeTuple_ELEM(result)) Dee_DECREF(*--dst_elem);
    _DeeTuple_FreeUnsafe(result);
    return NULL;
   }
   *dst_elem++ = copy;
   ++src_elem;
  }
 }
 return result;
}


DEE_A_RET_EXCEPT(NULL) DeeObject *DeeTuple_Front(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 if DEE_UNLIKELY(DeeTuple_EMPTY(self)) { DeeError_ValueError_EmptyList(&DeeTuple_Type); return NULL; }
 DEE_ASSERT(DeeTuple_SIZE(self) != 0);
 return DeeTuple_GET(self,0);
}
DEE_A_RET_EXCEPT(NULL) DeeObject *DeeTuple_Back(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 if DEE_UNLIKELY(DeeTuple_EMPTY(self)) { DeeError_ValueError_EmptyList(&DeeTuple_Type); return NULL; }
 DEE_ASSERT(DeeTuple_SIZE(self) != 0);
 return DeeTuple_GET(self,DeeTuple_SIZE(self)-1);
}
DEE_A_RET_EXCEPT(NULL) DeeObject *DeeTuple_Get(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_IN Dee_ssize_t i) {
 Dee_size_t size;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 if DEE_UNLIKELY((size = DeeTuple_SIZE(self)) == 0) {
  DeeError_ValueError_EmptyList(&DeeTuple_Type);
  return NULL;
 }
 DEE_PRIVATE_CLAMP_INDEX(i,size);
 return DeeTuple_GET(self,i);
}
DEE_A_RET_EXCEPT(NULL) DeeObject *DeeTuple_At(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_IN Dee_size_t i) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 if DEE_UNLIKELY(i >= DeeTuple_SIZE(self)) {
  DeeError_IndexError((DeeObject *)self,i);
  return NULL;
 }
 return DeeTuple_GET(self,i);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *DeeTuple_GetRangeIndex(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) {
 Dee_size_t size;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 if ((size = DeeTuple_SIZE(self)) == 0) DeeReturn_EmptyTuple;
 DEE_PRIVATE_CLAMP_URANGE(lo,hi,size);
 if DEE_UNLIKELY(hi <= lo) DeeReturn_EmptyTuple;
 return DeeTuple_NewFromVector((Dee_size_t)(hi-lo),DeeTuple_ELEM(self)+lo);
}
DEE_A_RET_EXCEPT(-1) int DeeTuple_Traverse(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self,
 DEE_A_IN DeeObjectIterateFunc func, DEE_A_IN_OPT void *closure) {
 DeeObject **iter,**end; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 end = (iter = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
 while DEE_LIKELY(iter != end) {
  if DEE_UNLIKELY((result = (*func)(*iter,closure)) != 0) return result;
  ++iter;
 }
 return 0;
}


DEE_A_RET_WUNUSED Dee_size_t DeeTuple_Size(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 return DeeTuple_SIZE(self);
}
DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL DeeObject **
_DeeTuple_Elem(DEE_A_IN_OBJECT(DeeTupleObject) *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 return DeeTuple_ELEM(self);
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeTuple_Contains(
 DEE_A_IN_OBJECT(DeeTupleObject) *self, DEE_A_INOUT DeeObject *ob) {
 DeeObject **elem,**end; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 DEE_ASSERT(DeeObject_Check(ob));
 end = (elem = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
 while DEE_LIKELY(elem != end) {
  temp = DeeObject_CompareEq(*elem,ob);
  if DEE_UNLIKELY(temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return -1;
   } else {
    DEE_ASSERT(temp > 0);
    return 1;
   }
  }
  ++elem;
 }
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeTuple_ContainsPred(
 DEE_A_IN_OBJECT(DeeTupleObject) *self,
 DEE_A_INOUT DeeObject *ob, DEE_A_INOUT DeeObject *pred) {
 DeeObject **elem,**end,*pred_args,*pred_result; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 DEE_ASSERT(DeeObject_Check(ob));
 DEE_ASSERT(DeeObject_Check(pred));
 end = (elem = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
 while DEE_LIKELY(elem != end) {
  if DEE_UNLIKELY((pred_args = DeeTuple_Pack(2,*elem,elem)) == NULL) return -1;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) {
check_ni:
   if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return -1;
  } else {
   temp = DeeObject_Bool(pred_result);
   Dee_DECREF(pred_result);
   if DEE_UNLIKELY(temp != 0) {
    if DEE_UNLIKELY(temp < 0) goto check_ni;
    DEE_ASSERT(temp > 0);
    return 1;
   }
  }
  ++elem;
 }
 return 0;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleIteratorObject) *
DeeTuple_IterSelf(DEE_A_IN_OBJECT(DeeTupleObject) *self) {
 DeeTupleIteratorObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeTupleIteratorObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeTupleIterator_Type);
 Dee_INCREF(result->ti_tuple = (DeeTupleObject *)self);
 result->ti_end = (result->ti_pos = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
 return (DeeObject *)result;
}

DEE_A_RET_EXCEPT(-1) int DeeTuple_Hash(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self,
 DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result) {
 DeeObject *const *elem,*const *end;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 end = (elem = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
 while DEE_LIKELY(elem != end) { if DEE_UNLIKELY(DeeObject_HashInplace(*elem,start) != 0) return -1; ++elem; }
 *result = start;
 return 0;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeListObject) *
DeeTuple_AsList(DEE_A_IN_OBJECT(DeeTupleObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 return DeeList_NewFromVector(DeeTuple_SIZE(self),DeeTuple_ELEM(self));
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *DeeTuple_TConcat(
 DEE_A_IN_OBJECT(DeeTupleObject) const *lhs,
 DEE_A_IN DeeTypeObject const *tp_rhs, DEE_A_INOUT DeeObject *rhs) {
 DeeObject *result; Dee_size_t lhs_size,rhs_size;
 DEE_ASSERT(DeeObject_Check(lhs) && DeeTuple_Check(lhs));
 DEE_ASSERT(DeeObject_Check(rhs));
 DEE_ASSERT(DeeObject_Check(tp_rhs) && DeeType_Check(tp_rhs));
 DEE_ASSERT(DeeObject_InstanceOf(rhs,tp_rhs));
 if (tp_rhs == &DeeTuple_Type) {
  if ((lhs_size = DeeTuple_SIZE(lhs)) == 0) DeeReturn_NEWREF(rhs);
  if ((rhs_size = DeeTuple_SIZE(rhs)) == 0) DeeReturn_NEWREF(lhs);
  if DEE_LIKELY((result = _DeeTuple_NewUnsafe(lhs_size+rhs_size)) != NULL) {
   DeeObject **dst,**src;
   dst = DeeTuple_ELEM(result);
   src = DeeTuple_ELEM(lhs); do Dee_INCREF(*dst++ = *src++); while(--lhs_size);
   src = DeeTuple_ELEM(rhs); do Dee_INCREF(*dst++ = *src++); while(--rhs_size);
  }
 } else {
  if DEE_UNLIKELY((result = DeeObject_Copy((DeeObject *)lhs)) == NULL) return NULL;
  if DEE_UNLIKELY(_DeeTuple_TAppendNonTupleSeq(&result,tp_rhs,rhs) != 0) {
   Dee_DECREF(result);
   result = NULL;
  }
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *DeeTuple_TCast(
 DEE_A_IN DeeTypeObject const *tp_seq, DEE_A_INOUT DeeObject *sequence) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(sequence));
 DEE_ASSERT(DeeObject_Check(tp_seq) && DeeType_Check(tp_seq));
 DEE_ASSERT(DeeObject_InstanceOf(sequence,tp_seq));
 if (tp_seq == &DeeTuple_Type) DeeReturn_NEWREF(sequence);
 if (tp_seq == &DeeList_Type) {
  Dee_size_t size; DeeObject **iter,**end,**src;
  DeeList_ACQUIRE(sequence);
  size = DeeList_SIZE(sequence);
  DeeList_RELEASE(sequence);
  if (!size) DeeReturn_EmptyTuple;
  if DEE_UNLIKELY((result = _DeeTuple_NewUnsafe(size)) == NULL) return NULL;
list_again:
  DeeList_ACQUIRE(sequence);
  // Make sure the list didn't change in the meantime
  if DEE_UNLIKELY(size != DeeList_SIZE(sequence)) {
   size = DeeList_SIZE(sequence);
   DeeList_RELEASE(sequence);
   if (!size) { _DeeTuple_FreeUnsafe(result); DeeReturn_EmptyTuple; }
   if DEE_UNLIKELY(_DeeTuple_ResizeUnsafe(&result,size) != 0) {
/*err_r_unsafe:*/ _DeeTuple_FreeUnsafe(result);
    return NULL;
   }
   goto list_again;
  }
  end = (iter = DeeTuple_ELEM(result))+size;
  src = DeeList_ELEM(sequence);
  do Dee_INCREF(*iter++ = *src++); while (iter != end);
  DeeList_RELEASE(sequence);
  return result;
 }
 if (tp_seq == &DeeCell_Type) {
  DeeObject *cellval;
  if ((cellval = DeeCell_TryGet(sequence)) == NULL) return DeeCell_NewEmpty();
  result = DeeTuple_Pack(1,cellval);
  Dee_DECREF(cellval);
  return result;
 }
 if DEE_UNLIKELY((sequence = DeeObject_TIterSelf(tp_seq,sequence)) == NULL) return NULL;
 result = DeeTuple_NewFromIterator(sequence);
 Dee_DECREF(sequence);
 return result;
}
DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *
DeeTuple_NewFromIterator(DEE_A_INOUT DeeObject *iterator) {
 DeeObject *result,*elem; int error;
 DEE_ASSERT(DeeObject_Check(iterator));
 if DEE_UNLIKELY((error = DeeObject_IterNextEx(iterator,&elem)) != 0)
  return DEE_UNLIKELY(error < 0) ? NULL : DeeTuple_NewEmpty();
 if DEE_UNLIKELY((result = _DeeTuple_NewUnsafe(1)) == NULL) return NULL;
 DeeTuple_SET(result,0,elem); // inherit reference
 while DEE_LIKELY((error = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  error = DeeTuple_Append(&result,elem);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(error != 0) { Dee_DECREF(result); return NULL; }
 }
 if DEE_UNLIKELY(error < 0) Dee_CLEAR(result);
 return result;
}

DEE_A_RET_EXCEPT(-1) int _DeeTuple_TAppendNonTupleSeq(
 DEE_A_INOUT_OBJECT(DeeTupleObject) **self,
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) {
 DeeObject *elem,*iter; int error;
 DEE_ASSERT(DeeObject_Check(sequence));
 DEE_ASSERT(DeeObject_Check(tp_sequence) && DeeType_Check(tp_sequence));
 DEE_ASSERT(DeeObject_InstanceOf(sequence,tp_sequence));
 if DEE_UNLIKELY((iter = DeeObject_TIterSelf(tp_sequence,sequence)) == NULL) return -1;
 while DEE_LIKELY((error = DeeObject_IterNextEx(iter,&elem)) == 0) {
  error = DeeTuple_Append(self,elem);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(error != 0) return error;
 }
 Dee_DECREF(iter);
 return DEE_UNLIKELY(error < 0) ? error : 0;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *DeeTuple_Types(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self) {
 DeeObject *result,**dst_elem,**src_elem,**src_end; Dee_size_t size;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 size = DeeTuple_SIZE(self);
 if DEE_LIKELY((result = _DeeTuple_NewUnsafe(size)) != NULL) {
  dst_elem = DeeTuple_ELEM(result);
  src_end = (src_elem = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
  while DEE_LIKELY(src_elem != src_end)
   Dee_INCREF(*dst_elem++ = (DeeObject *)Dee_TYPE(*src_elem++));
 }
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *
_DeeTuple_NewObjArgs(DEE_A_IN int prefix, ...) {
 DeeObject *result; va_list args;
 DEE_VA_START(args,prefix);
 result = DeeTuple_VNewObjArgs(args);
 DEE_VA_END(args);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *
DeeTuple_VNewObjArgs(DEE_A_IN va_list args) {
 DeeObject **elem,*result; Dee_size_t n = 0;
 va_list count_copy;
 DEE_VA_COPY(count_copy,args);
 while (DEE_VA_ARG(count_copy,DeeObject *)) ++n;
 DEE_VA_END(count_copy);
 if DEE_UNLIKELY((result = _DeeTuple_NewUnsafe(n)) == NULL) return NULL;
 elem = DeeTuple_ELEM(result);
#if defined(DEE_DEBUG) || defined(_PREFAST_)
 while DEE_LIKELY(n--) {
  DeeObject *temp = DEE_VA_ARG(args,DeeObject *);
  DEE_ASSERT(temp);
  Dee_INCREF(*elem++ = temp);
 }
#else
 while (n--) Dee_INCREF(*elem++ = DEE_VA_ARG(args,DeeObject *));
#endif
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeTuple_Find(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *elem) {
 DeeObject *const *iter,*const *end; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 end = (iter = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
 while DEE_LIKELY(iter != end) {
  if DEE_UNLIKELY((temp = DeeObject_CompareEq(*iter,elem)) != 0) {
   if DEE_UNLIKELY(temp < 0) {
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return (Dee_ssize_t)-2;
   } else {
    DEE_ASSERT(temp > 0);
    return (Dee_ssize_t)(iter-DeeTuple_ELEM(self));
   }
  }
  ++iter;
 }
 return (Dee_ssize_t)-1;
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeTuple_FindPred(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self,
 DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) {
 DeeObject *const *iter,*const *end; int temp;
 DeeObject *pred_args,*pred_result;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 DEE_ASSERT(DeeObject_Check(pred));
 end = (iter = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
 while (iter != end) {
  if DEE_UNLIKELY((pred_args = DeeTuple_Pack(2,*iter,elem)) == NULL) return (Dee_ssize_t)-2;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) goto check_ni;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if DEE_UNLIKELY(temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
check_ni:
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return (Dee_ssize_t)-2;
   } else {
    DEE_ASSERT(temp > 0);
    return (Dee_ssize_t)(iter-DeeTuple_ELEM(self));
   }
  }
  ++iter;
 }
 return (Dee_ssize_t)-1;
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeTuple_RFind(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *elem) {
 DeeObject *const *iter,*const *begin; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 iter = (begin = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
 while DEE_LIKELY(iter != begin) {
  --iter;
  if DEE_UNLIKELY((temp = DeeObject_CompareEq(*iter,elem)) != 0) {
   if DEE_UNLIKELY(temp < 0) {
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return (Dee_ssize_t)-2;
   } else {
    DEE_ASSERT(temp > 0);
    return (Dee_ssize_t)(iter-DeeTuple_ELEM(self));
   }
  }
 }
 return (Dee_ssize_t)-1;
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeTuple_RFindPred(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self,
 DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) {
 DeeObject *const *iter,*const *begin; int temp;
 DeeObject *pred_args,*pred_result;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 DEE_ASSERT(DeeObject_Check(pred));
 iter = (begin = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
 while DEE_LIKELY(iter != begin) {
  if DEE_UNLIKELY((pred_args = DeeTuple_Pack(2,*--iter,elem)) == NULL) return (Dee_ssize_t)-2;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) goto check_ni;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if DEE_UNLIKELY(temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
check_ni:
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return (Dee_ssize_t)-2;
   } else {
    DEE_ASSERT(temp > 0);
    return (Dee_ssize_t)(iter-DeeTuple_ELEM(self));
   }
  }
 }
 return (Dee_ssize_t)-1;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeTuple_Index(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *elem) {
 Dee_ssize_t result = DeeTuple_Find(self,elem);
 if DEE_UNLIKELY(result == -2) return (Dee_size_t)-1;
 if DEE_UNLIKELY(result == -1) { DeeError_ValueError_NotFound((DeeObject *)self,elem); return (Dee_size_t)-1; }
 return (Dee_size_t)result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeTuple_IndexPred(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self,
 DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) {
 Dee_ssize_t result = DeeTuple_FindPred(self,elem,pred);
 if DEE_UNLIKELY(result == -2) return (Dee_size_t)-1;
 if DEE_UNLIKELY(result == -1) { DeeError_ValueError_NotFound((DeeObject *)self,elem); return (Dee_size_t)-1; }
 return (Dee_size_t)result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeTuple_RIndex(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *elem) {
 Dee_ssize_t result = DeeTuple_RFind(self,elem);
 if DEE_UNLIKELY(result == -2) return (Dee_size_t)-1;
 if DEE_UNLIKELY(result == -1) { DeeError_ValueError_NotFound((DeeObject *)self,elem); return (Dee_size_t)-1; }
 return (Dee_size_t)result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeTuple_RIndexPred(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self,
 DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) {
 Dee_ssize_t result = DeeTuple_RFindPred(self,elem,pred);
 if DEE_UNLIKELY(result == -2) return (Dee_size_t)-1;
 if DEE_UNLIKELY(result == -1) { DeeError_ValueError_NotFound((DeeObject *)self,elem); return (Dee_size_t)-1; }
 return (Dee_size_t)result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeTuple_Count(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *elem) {
 Dee_size_t result = 0; DeeObject **iter,**end; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 end = (iter = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
 while DEE_LIKELY(iter != end) {
  if DEE_UNLIKELY((temp = DeeObject_CompareEq(*iter,elem)) != 0) {
   if DEE_UNLIKELY(temp < 0) {
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return (Dee_size_t)-1;
   } else {
    DEE_ASSERT(temp > 0);
    ++result;
   }
  }
  ++iter;
 }
 DEE_ASSERTF(result != (Dee_size_t)-1,
             "How can this be possible?");
 return result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeTuple_CountPred(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self,
 DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) {
 Dee_size_t result = 0; DeeObject **iter,**end; int temp;
 DeeObject *pred_args,*pred_result;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 DEE_ASSERT(DeeObject_Check(pred));
 end = (iter = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
 while DEE_LIKELY(iter != end) {
  if DEE_UNLIKELY((pred_args = DeeTuple_Pack(2,*iter,elem)) == NULL) return (Dee_size_t)-1;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) goto check_ni;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if DEE_UNLIKELY(temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
check_ni:
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return (Dee_size_t)-1;
   } else {
    DEE_ASSERT(temp > 0);
    ++result;
   }
  }
  ++iter;
 }
 DEE_ASSERTF(result != (Dee_size_t)-1,
             "How can this be possible?");
 return result;
}



DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *DeeTuple_Reversed(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self) {
 DeeObject *result,**dst,**iter,**end; Dee_size_t size;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 size = DeeTuple_SIZE(self);
 if DEE_LIKELY((result = _DeeTuple_NewUnsafe(size)) != NULL) {
  dst = DeeTuple_ELEM(result)+size;
  end = (iter = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
  while DEE_LIKELY(iter != end) Dee_INCREF(*--dst = *iter++);
 }
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *DeeTuple_Sorted(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self) {
 DeeObject *result,**iter,**end; Dee_size_t size;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 size = DeeTuple_SIZE(self);
 if DEE_LIKELY((result = _DeeTuple_NewUnsafe(size)) != NULL) {
  end = (iter = DeeTuple_ELEM(self))+size;
  while DEE_LIKELY(iter != end) Dee_INCREF(*iter++); // Reserve references
  if DEE_UNLIKELY(_DeeGeneric_MergeSort(size,DeeTuple_ELEM(self),
   DeeTuple_ELEM(result),&_DeeGeneric_SortPredCmpLo,NULL
   ) != 0) Dee_CLEAR(result);
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *DeeTuple_SortedPred(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *pred) {
 DeeObject *result,**iter,**end; Dee_size_t size;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 size = DeeTuple_SIZE(self);
 if DEE_LIKELY((result = _DeeTuple_NewUnsafe(size)) != NULL) {
  end = (iter = DeeTuple_ELEM(self))+size;
  while DEE_LIKELY(iter != end) Dee_INCREF(*iter++); // Reserve references
  if DEE_UNLIKELY(_DeeGeneric_MergeSort(size,DeeTuple_ELEM(self),
   DeeTuple_ELEM(result),&_DeeGeneric_SortPredUserFunc,pred
   ) != 0) Dee_CLEAR(result);
 }
 return result;
}


DEE_A_RET_EXCEPT(NULL) DeeObject *DeeTuple_Locate(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *pred) {
 DeeObject **iter,**end,*args,*temp; int itemp;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 DEE_ASSERT(DeeObject_Check(pred));
 end = (iter = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
 while DEE_LIKELY(iter != end) {
  if DEE_UNLIKELY((args = DeeTuple_Pack(1,*iter)) == NULL) return NULL;
  temp = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if DEE_UNLIKELY(!temp) return NULL;
  itemp = DeeObject_Bool(temp);
  Dee_DECREF(temp);
  if DEE_UNLIKELY(temp != 0) {
   if DEE_UNLIKELY(itemp < 0) return NULL;
   DEE_ASSERT(itemp > 0);
   return *iter;
  }
  ++iter;
 }
 DeeError_ValueError_NotFound_Pred((DeeObject *)self,pred);
 return NULL;
}
DEE_A_RET_EXCEPT(NULL) DeeObject *DeeTuple_RLocate(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *pred) {
 DeeObject **iter,**begin,*args,*temp; int itemp;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 DEE_ASSERT(DeeObject_Check(pred));
 iter = (begin = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
 while DEE_LIKELY(iter != begin) {
  if DEE_UNLIKELY((args = DeeTuple_Pack(1,*--iter)) == NULL) return NULL;
  temp = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if DEE_UNLIKELY(!temp) return NULL;
  itemp = DeeObject_Bool(temp);
  Dee_DECREF(temp);
  if DEE_UNLIKELY(itemp != 0) {
   if DEE_UNLIKELY(itemp < 0) return NULL;
   DEE_ASSERT(itemp > 0);
   return *iter;
  }
 }
 DeeError_ValueError_NotFound_Pred((DeeObject *)self,pred);
 return NULL;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeTuple_LocateAll(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *pred) {
 DeeObject *result,**iter,**end,*args,*temp; int itemp;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 DEE_ASSERT(DeeObject_Check(pred));
 end = (iter = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
 Dee_INCREF(result = Dee_EmptyTuple);
 while DEE_LIKELY(iter != end) {
  if DEE_UNLIKELY((args = DeeTuple_Pack(1,*iter)) == NULL) goto err_r;
  temp = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if DEE_UNLIKELY(!temp) goto err_r;
  itemp = DeeObject_Bool(temp);
  Dee_DECREF(temp);
  if DEE_UNLIKELY(itemp != 0) {
   if DEE_UNLIKELY(itemp < 0) goto err_r;
   DEE_ASSERT(itemp > 0);
   if DEE_UNLIKELY(DeeTuple_Append(&result,*iter) != 0) {
err_r:
    Dee_DECREF(result);
    return NULL;
   }
  }
  ++iter;
 }
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *DeeTuple_Transform(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_INOUT DeeObject *pred) {
 DeeObject *result,**dst,**iter,**end,*args,*elem;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 DEE_ASSERT(DeeObject_Check(pred));
 if DEE_LIKELY((result = _DeeTuple_NewUnsafe(DeeTuple_SIZE(self))) != NULL) {
  dst = DeeTuple_ELEM(result);
  end = (iter = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
  while DEE_LIKELY(iter != end) {
   if DEE_UNLIKELY((args = DeeTuple_Pack(1,*iter)) == NULL) {
err_r:
    while DEE_LIKELY(dst != DeeTuple_ELEM(result)) Dee_DECREF(*--dst);
    _DeeTuple_FreeUnsafe(result);
    return NULL;
   }
   elem = DeeObject_Call(pred,args);
   Dee_DECREF(args);
   if (!elem) goto err_r;
   Dee_INHERIT_REF(*dst,elem);
   ++dst,++iter;
  }
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeTuple_Segments(
 DEE_A_IN_OBJECT(DeeTupleObject) const *self, DEE_A_IN Dee_size_t n) {
 DeeObject **dst_iter,**dst_end,**src_iter,*result;
 Dee_size_t my_segment_size,src_size,segment_size;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 if DEE_UNLIKELY(!n) return DeeList_NewEmpty();
 if DEE_UNLIKELY((result = _DeeList_NewUnsafe(n)) == NULL) return NULL;
 src_size = DeeTuple_SIZE(self),segment_size = src_size/n;
 if (segment_size*n != src_size) ++segment_size;
 dst_end = (dst_iter = DeeList_ELEM(result))+n;
 src_iter = DeeTuple_ELEM(self);
 while DEE_LIKELY(dst_iter != dst_end) {
  my_segment_size = segment_size <= src_size ? segment_size : src_size;
  if DEE_UNLIKELY((*dst_iter = DeeTuple_NewFromVector(segment_size,src_iter)) == NULL) {
   while DEE_LIKELY(dst_iter != DeeList_ELEM(result)) Dee_DECREF(*--dst_iter);
   _DeeList_FreeUnsafe(result);
   return NULL;
  }
  ++dst_iter;
  src_iter += my_segment_size;
  src_size -= my_segment_size;
 }
 return result;
}









static DeeObject *_deetuple_tp_new(DeeTypeObject *DEE_UNUSED(tp)) {
 DeeReturn_EmptyTuple;
}
static DeeObject *_deetuple_tp_copy_new(
 DeeTypeObject *DEE_UNUSED(tp), DeeObject *self) {
 Dee_size_t tuple_size;
 DeeObject *result,**dst_elem,**src_elem,**end;
 tuple_size = DeeTuple_SIZE(self);
 if DEE_LIKELY((result = _DeeTuple_NewUnsafe(tuple_size)) != NULL) {
  dst_elem = DeeTuple_ELEM(result);
  src_elem = DeeTuple_ELEM(self);
  end = src_elem+tuple_size;
  while DEE_LIKELY(src_elem != end)
   Dee_INCREF(*dst_elem++ = *src_elem++);
 }
 return result;
}
static DeeObject *_deetuple_tp_any_new(
 DeeTypeObject *DEE_UNUSED(tp), DeeObject *args) {
 DeeObject *arg0;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:tuple",&arg0) != 0) return NULL;
 return DeeTuple_Cast(arg0);
}
static void _deetuple_tp_dtor(DeeTupleObject *self) {
 DeeObject **elem,**end;
 end = (elem = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
 while DEE_LIKELY(elem != end) Dee_DECREF(*elem++);
}
#define DEE_PRIVATE_MAKE_TUPLE_STR_FUN(name,elem_str,prefix)\
static DeeObject *name(DeeTupleObject *self) {\
 DeeObject **elem,**end,*str;\
 DeeStringWriter writer = DeeStringWriter_INIT();\
 if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,prefix) != 0) goto err;\
 end = (elem = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);\
 if DEE_LIKELY(elem != end) {\
  str = elem_str(*elem); ++elem;\
  if DEE_UNLIKELY(!str || DeeStringWriter_WriteObjectStr(&writer,str) != 0) goto err2;\
  Dee_DECREF(str);\
  while DEE_LIKELY(elem != end) {\
   if DEE_UNLIKELY(DeeStringWriter_WriteChar(&writer,',') != 0) goto err;\
   str = elem_str(*elem); ++elem;\
   if DEE_UNLIKELY(!str || DeeStringWriter_WriteObjectStr(&writer,str) != 0) goto err2;\
   Dee_DECREF(str);\
  }\
 }\
 if DEE_UNLIKELY(DeeStringWriter_WriteChar(&writer,')') != 0) goto err;\
 str = DeeStringWriter_Pack(&writer);\
 DeeStringWriter_Quit(&writer);\
 return str;\
err2:\
 Dee_XDECREF(str);\
err:\
 DeeStringWriter_Quit(&writer);\
 return NULL;\
}
DEE_PRIVATE_MAKE_TUPLE_STR_FUN(_deetuple_tp_str,DeeObject_Str,"(")
DEE_PRIVATE_MAKE_TUPLE_STR_FUN(_deetuple_tp_repr,DeeObject_Repr,"pack(")
#undef DEE_PRIVATE_MAKE_TUPLE_STR_FUN

DEE_VISIT_PROC(_deetuple_tp_visit,DeeTupleObject *self) {
 DeeObject **elem,**end;
 end = (elem = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
 while DEE_LIKELY(elem != end) Dee_VISIT(*elem++);
}

static DeeObject *_deetuple_tp_seq_get(DeeTupleObject *self, DeeObject *i_ob) {
 Dee_ssize_t i;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 DEE_ASSERT(DeeObject_Check(i_ob));
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,i_ob,&i) != 0) return NULL;
 DeeReturn_XNEWREF(DeeTuple_Get((DeeObject *)self,i));
}
static int _deetuple_tp_bool(DeeTupleObject *self) {
 return !DeeTuple_EMPTY(self);
}
static DeeObject *_deetuple_tp_not(DeeTupleObject *self) {
 DeeReturn_Bool(DeeTuple_EMPTY(self));
}
static DeeObject *_deetuple_tp_seq_size(DeeTupleObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 return DeeObject_New(Dee_size_t,DeeTuple_SIZE(self));
}
static DeeObject *_deetuple_tp_seq_contains(DeeTupleObject *self, DeeObject *ob) {
 int temp;
 if DEE_UNLIKELY((temp = DeeTuple_Contains((DeeObject *)self,ob)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *_deetuple_tp_seq_range_get(
 DeeTupleObject *self, DeeObject *lo, DeeObject *hi) {
 Dee_ssize_t loi,hii; Dee_size_t size;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 DEE_ASSERT(DeeObject_Check(lo) && DeeObject_Check(hi));
 size = DeeTuple_SIZE(self);
 if (lo != Dee_None) { if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,lo,&loi) != 0) return NULL; } else loi = 0;
 if (hi != Dee_None) { if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,hi,&hii) != 0) return NULL; } else hii = (Dee_ssize_t)size;
 if DEE_UNLIKELY(!size) DeeReturn_EmptyTuple;
 DEE_PRIVATE_CLAMP_RANGE(loi,hii,size);
 if DEE_UNLIKELY(hii <= loi) DeeReturn_EmptyTuple;
 return DeeTuple_NewFromVector((Dee_size_t)(hii-loi),DeeTuple_ELEM(self)+loi);
}
static DeeObject *_deetuple_tp_cmp_lo(DeeTupleObject *lhs, DeeObject *rhs) {
 DeeObject *rhs_iterator; int result;
 if (DeeTuple_Check(rhs)) result = DeeGeneric_CompareVVLo(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),DeeTuple_SIZE(rhs),DeeTuple_ELEM(rhs));
 //else if (DeeList_Check(rhs)) result = DeeGeneric_CompareVVLo(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),DeeList_SIZE(rhs),DeeList_ELEM(rhs));
 else {
  if DEE_UNLIKELY((rhs_iterator = DeeObject_IterSelf(rhs)) == NULL) return NULL;
  result = DeeGeneric_CompareVILo(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),rhs_iterator);
  Dee_DECREF(rhs_iterator);
 }
 if DEE_UNLIKELY(result < 0) return NULL;
 DeeReturn_Bool(result);
}
static DeeObject *_deetuple_tp_cmp_le(DeeTupleObject *lhs, DeeObject *rhs) {
 DeeObject *rhs_iterator; int result;
 if (DeeTuple_Check(rhs)) result = DeeGeneric_CompareVVLe(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),DeeTuple_SIZE(rhs),DeeTuple_ELEM(rhs));
 //else if (DeeList_Check(rhs)) result = DeeGeneric_CompareVVLe(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),DeeList_SIZE(rhs),DeeList_ELEM(rhs));
 else {
  if DEE_UNLIKELY((rhs_iterator = DeeObject_IterSelf(rhs)) == NULL) return NULL;
  result = DeeGeneric_CompareVILe(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),rhs_iterator);
  Dee_DECREF(rhs_iterator);
 }
 if DEE_UNLIKELY(result < 0) return NULL;
 DeeReturn_Bool(result);
}
static DeeObject *_deetuple_tp_cmp_eq(DeeTupleObject *lhs, DeeObject *rhs) {
 DeeObject *rhs_iterator; int result;
 if (DeeTuple_Check(rhs)) result = DeeGeneric_CompareVVEq(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),DeeTuple_SIZE(rhs),DeeTuple_ELEM(rhs));
 //else if (DeeList_Check(rhs)) result = DeeGeneric_CompareVVEq(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),DeeList_SIZE(rhs),DeeList_ELEM(rhs));
 else {
  if DEE_UNLIKELY((rhs_iterator = DeeObject_IterSelf(rhs)) == NULL) return NULL;
  result = DeeGeneric_CompareVIEq(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),rhs_iterator);
  Dee_DECREF(rhs_iterator);
 }
 if DEE_UNLIKELY(result < 0) return NULL;
 DeeReturn_Bool(result);
}
static DeeObject *_deetuple_tp_cmp_ne(DeeTupleObject *lhs, DeeObject *rhs) {
 DeeObject *rhs_iterator; int result;
 if (DeeTuple_Check(rhs)) result = DeeGeneric_CompareVVNe(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),DeeTuple_SIZE(rhs),DeeTuple_ELEM(rhs));
 //else if (DeeList_Check(rhs)) result = DeeGeneric_CompareVVNe(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),DeeList_SIZE(rhs),DeeList_ELEM(rhs));
 else {
  if DEE_UNLIKELY((rhs_iterator = DeeObject_IterSelf(rhs)) == NULL) return NULL;
  result = DeeGeneric_CompareVINe(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),rhs_iterator);
  Dee_DECREF(rhs_iterator);
 }
 if DEE_UNLIKELY(result < 0) return NULL;
 DeeReturn_Bool(result);
}
static DeeObject *_deetuple_tp_cmp_gr(DeeTupleObject *lhs, DeeObject *rhs) {
 DeeObject *rhs_iterator; int result;
 if (DeeTuple_Check(rhs)) result = DeeGeneric_CompareVVGr(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),DeeTuple_SIZE(rhs),DeeTuple_ELEM(rhs));
 //else if (DeeList_Check(rhs)) result = DeeGeneric_CompareVVGr(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),DeeList_SIZE(rhs),DeeList_ELEM(rhs));
 else {
  if DEE_UNLIKELY((rhs_iterator = DeeObject_IterSelf(rhs)) == NULL) return NULL;
  result = DeeGeneric_CompareVIGr(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),rhs_iterator);
  Dee_DECREF(rhs_iterator);
 }
 if DEE_UNLIKELY(result < 0) return NULL;
 DeeReturn_Bool(result);
}
static DeeObject *_deetuple_tp_cmp_ge(DeeTupleObject *lhs, DeeObject *rhs) {
 DeeObject *rhs_iterator; int result;
 if (DeeTuple_Check(rhs)) result = DeeGeneric_CompareVVGe(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),DeeTuple_SIZE(rhs),DeeTuple_ELEM(rhs));
 //else if (DeeList_Check(rhs)) result = DeeGeneric_CompareVVGe(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),DeeList_SIZE(rhs),DeeList_ELEM(rhs));
 else {
  if DEE_UNLIKELY((rhs_iterator = DeeObject_IterSelf(rhs)) == NULL) return NULL;
  result = DeeGeneric_CompareVIGe(DeeTuple_SIZE(lhs),DeeTuple_ELEM(lhs),rhs_iterator);
  Dee_DECREF(rhs_iterator);
 }
 if DEE_UNLIKELY(result < 0) return NULL;
 DeeReturn_Bool(result);
}



static DeeObject *_deetuple_find(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem,*pred = Dee_None; Dee_ssize_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:find",&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeTuple_Find((DeeObject *)self,elem)
  : DeeTuple_FindPred((DeeObject *)self,elem,pred)) == -2) return NULL;
 return DeeObject_New(Dee_ssize_t,result);
}
static DeeObject *_deetuple_index(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem,*pred = Dee_None; Dee_size_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:index",&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeTuple_Index((DeeObject *)self,elem)
  : DeeTuple_IndexPred((DeeObject *)self,elem,pred)) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *_deetuple_rfind(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem,*pred = Dee_None; Dee_ssize_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:rfind",&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeTuple_RFind((DeeObject *)self,elem)
  : DeeTuple_RFindPred((DeeObject *)self,elem,pred)) == -2) return NULL;
 return DeeObject_New(Dee_ssize_t,result);
}
static DeeObject *_deetuple_rindex(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem,*pred = Dee_None; Dee_size_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:rindex",&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeTuple_RIndex((DeeObject *)self,elem)
  : DeeTuple_RIndexPred((DeeObject *)self,elem,pred)) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *_deetuple_count(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem,*pred = Dee_None; Dee_size_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:count",&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeTuple_Count((DeeObject *)self,elem)
  : DeeTuple_CountPred((DeeObject *)self,elem,pred)) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *_deetuple_contains(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem,*pred = Dee_None; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:contains",&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeTuple_Contains((DeeObject *)self,elem)
  : DeeTuple_ContainsPred((DeeObject *)self,elem,pred)) < 0) return NULL;
 DeeReturn_Bool(result);
}
static DeeObject *_deetuple_empty(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":empty") != 0) return NULL;
 DeeReturn_Bool(DeeTuple_SIZE(self) == 0);
}
static DeeObject *_deetuple_non_empty(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":non_empty") != 0) return NULL;
 DeeReturn_Bool(DeeTuple_SIZE(self) != 0);
}
static DeeObject *_deetuple_front(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":front") != 0) return NULL;
 DeeReturn_XNEWREF(DeeTuple_Front((DeeObject *)self));
}
static DeeObject *_deetuple_back(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":back") != 0) return NULL;
 DeeReturn_XNEWREF(DeeTuple_Back((DeeObject *)self));
}
static DeeObject *_deetuple_at(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t index;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu:at",&index) != 0) return NULL;
 DeeReturn_XNEWREF(DeeTuple_At((DeeObject *)self,index));
}
static DeeObject *_deetuple_locate(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:locate",&pred) != 0) return NULL;
 DeeReturn_XNEWREF(DeeTuple_Locate((DeeObject *)self,pred));
}
static DeeObject *_deetuple_rlocate(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:rlocate",&pred) != 0) return NULL;
 DeeReturn_XNEWREF(DeeTuple_RLocate((DeeObject *)self,pred));
}
static DeeObject *_deetuple_locate_all(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:locate_all",&pred) != 0) return NULL;
 return DeeTuple_LocateAll((DeeObject *)self,pred);
}
static DeeObject *_deetuple_transform(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:transform",&pred) != 0) return NULL;
 return DeeTuple_Transform((DeeObject *)self,pred);
}
static DeeObject *_deetuple_segments(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t n;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu:segments",&n) != 0) return NULL;
 return DeeTuple_Segments((DeeObject *)self,n);
}
static DeeObject *_deetuple_sorted(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:sorted",&pred) != 0) return NULL;
 return DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___lo__)
  ? DeeTuple_Sorted((DeeObject *)self)
  : DeeTuple_SortedPred((DeeObject *)self,pred);
}
static DeeObject *_deetuple_reversed(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":reversed") != 0) return NULL;
 return DeeTuple_Reversed((DeeObject *)self);
}

static struct DeeMethodDef const _deetuple_tp_methods[] = {
 DEE_METHODDEF_CONST_v100("at",member(&_deetuple_at),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("back",member(&_deetuple_back),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("contains",member(&_deetuple_contains),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("count",member(&_deetuple_count),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("empty",member(&_deetuple_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("find",member(&_deetuple_find),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("front",member(&_deetuple_front),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("index",member(&_deetuple_index),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("locate",member(&_deetuple_locate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("locate_all",member(&_deetuple_locate_all),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("non_empty",member(&_deetuple_non_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("reversed",member(&_deetuple_reversed),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rfind",member(&_deetuple_rfind),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rindex",member(&_deetuple_rindex),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rlocate",member(&_deetuple_rlocate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("segments",member(&_deetuple_segments),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("sorted",member(&_deetuple_sorted),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("transform",member(&_deetuple_transform),DEE_DOC_AUTO),
 DEE_METHODDEF_END_v100
};


static DeeObject *DEE_CALL _deetuple_tp_marshal_new(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *infile, struct DeeMarshalReadMap *map,
 DEE_A_IN PDeeMashalNewAfterAlloc after_alloc, void *after_alloc_data) {
 DeeObject *result; Dee_size_t used_size; Dee_uint64_t read_size;
 if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&read_size) != 0) return NULL;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_size > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Tuple is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_size,(Dee_size_t)-1);
  return NULL;
 }
#endif
 used_size = (Dee_size_t)read_size;
 if DEE_UNLIKELY((result = _DeeTuple_NewUnsafe(used_size)) != NULL) {
  DEE_ASSERTF(DeeObject_IS_UNIQUE(result),
              "Marshalled tuple at %p (size: %Iu) isn't unique before alloc notification (ref: %u; wref: %u)\n",
              result,used_size,result->__ob_refcnt,result->__ob_weakcnt);
  (*after_alloc)(result,after_alloc_data);
  DEE_ASSERTF(DeeObject_IS_UNIQUE(result),
              "Marshalled tuple at %p (size: %Iu) isn't unique after notifying about its allocation (ref: %u; wref: %u)\n",
              result,used_size,result->__ob_refcnt,result->__ob_weakcnt);
  if DEE_UNLIKELY(DeeMarshal_ReadObjectsWithMap(infile,map,used_size,DeeTuple_ELEM(result)) != 0) {
   DEE_ASSERTF(DeeObject_IS_UNIQUE(result),
               "Marshalled tuple at %p (size: %Iu) isn't unique after reading its elements failed (ref: %u; wref: %u)\n",
               result,used_size,result->__ob_refcnt,result->__ob_weakcnt);
   _DeeTuple_FreeUnsafe(result);
   result = NULL;
  }
 }
 return result;
}

static int DEE_CALL _deetuple_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,DeeTuple_SIZE(self)) != 0) return -1;
 return DeeMarshal_WriteObjectsWithMap(outfile,map,DeeTuple_SIZE(self),DeeTuple_ELEM(self));
}

static struct DeeTypeMarshal _deetuple_tp_marshal = DEE_TYPE_MARSHAL_VAR_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(tuple)),
 member(&_deetuple_tp_marshal_new),member(&_deetuple_tp_marshal_put));


struct _DeeEmptyTupleObject _Dee_EmptyTuple = {
 DEE_OBJECT_HEAD_INIT(&DeeTuple_Type),0};
DeeTypeObject DeeTuple_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("tuple"),null,member(
  DEE_TYPE_FLAG_VAR_OBJECT|DEE_TYPE_FLAG_CONST_CTOR|DEE_TYPE_FLAG_MUST_COPY|
  DEE_TYPE_FLAG_NO_ITERATE_EFFECT|DEE_TYPE_FLAG_NO_SUBCLASS|
  DEE_TYPE_FLAG_FUNDAMENTAL),null),
 DEE_TYPE_OBJECT_VAR_CONSTRUCTOR_v101(
  member(&_deetuple_tp_new),
  member(&_deetuple_tp_copy_new),null,
  member(&_deetuple_tp_any_new)),
#if DEE_TUPLE_POOL_MAX_TRACK_LENGTH
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(
  member(&_deetuple_tp_free),member(&_deetuple_tp_dtor)),
#else /* DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deetuple_tp_dtor)),
#endif /* !DEE_TUPLE_POOL_MAX_TRACK_LENGTH */
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deetuple_tp_str),member(&_deetuple_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deetuple_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deetuple_tp_not),member(&_deetuple_tp_bool),
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,
  member(&DeeTuple_Hash)),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deetuple_tp_cmp_lo),member(&_deetuple_tp_cmp_le),
  member(&_deetuple_tp_cmp_eq),member(&_deetuple_tp_cmp_ne),
  member(&_deetuple_tp_cmp_gr),member(&_deetuple_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&_deetuple_tp_seq_get),null,null,member(&_deetuple_tp_seq_size),
  member(&_deetuple_tp_seq_contains),member(&_deetuple_tp_seq_range_get),
  null,null,member(&DeeTuple_IterSelf),null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,null,member(_deetuple_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deetuple_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};













//////////////////////////////////////////////////////////////////////////
// GenericPointerIterator VTable
static void _deetupleiterator_tp_dtor(DeeTupleIteratorObject *self) { Dee_DECREF(self->ti_tuple); }
DEE_VISIT_PROC(_deetupleiterator_tp_visit,DeeTupleIteratorObject *self) { Dee_VISIT(self->ti_tuple); }
static int _deetupleiterator_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeTupleIteratorObject *self, DeeTupleIteratorObject *right) {
 Dee_INCREF(self->ti_tuple = right->ti_tuple);
 self->ti_pos = (DeeObject **)DeeAtomicPtr_Load(right->ti_pos,memory_order_seq_cst);
 self->ti_end = right->ti_end;
 DEE_ASSERT(self->ti_pos >= DeeTuple_ELEM(self->ti_tuple));
 DEE_ASSERT(self->ti_end == DeeTuple_ELEM(self->ti_tuple)+DeeTuple_SIZE(self->ti_tuple));
 return 0;
}
#define DEE_PRIVATE_MAKE_TUPLEITER_COMPARE_FUN(name,op)\
static DeeObject *name(DeeTupleIteratorObject *self, DeeObject *right) {\
 DeeObject **this_pos,**that_pos;\
 if DEE_UNLIKELY((right = DeeObject_GetInstance(right,&DeeTupleIterator_Type)) == NULL) return NULL;\
 this_pos = (DeeObject **)DeeAtomicPtr_Load(DeeTupleIterator_POS(self),memory_order_seq_cst);\
 that_pos = (DeeObject **)DeeAtomicPtr_Load(DeeTupleIterator_POS(right),memory_order_seq_cst);\
 DeeReturn_Bool(this_pos op that_pos);\
}
DEE_PRIVATE_MAKE_TUPLEITER_COMPARE_FUN(_deetupleiterator_tp_cmp_lo,<)
DEE_PRIVATE_MAKE_TUPLEITER_COMPARE_FUN(_deetupleiterator_tp_cmp_le,<=)
DEE_PRIVATE_MAKE_TUPLEITER_COMPARE_FUN(_deetupleiterator_tp_cmp_eq,==)
DEE_PRIVATE_MAKE_TUPLEITER_COMPARE_FUN(_deetupleiterator_tp_cmp_ne,!=)
DEE_PRIVATE_MAKE_TUPLEITER_COMPARE_FUN(_deetupleiterator_tp_cmp_gr,>)
DEE_PRIVATE_MAKE_TUPLEITER_COMPARE_FUN(_deetupleiterator_tp_cmp_ge,>=)
#undef DEE_PRIVATE_MAKE_TUPLEITER_COMPARE_FUN
static int _deetupleiterator_tp_seq_iter_next(
DeeTupleIteratorObject *self, DeeObject **result) {
 DeeObject **elem_pos;
 DEE_ASSERT(DeeObject_Check(self) && DeeTupleIterator_Check(self));
 elem_pos = (DeeObject **)DeeAtomicPtr_FetchAdd(
  self->ti_pos,sizeof(DeeObject *),memory_order_seq_cst);
 DEE_ASSERT(elem_pos >= DeeTuple_ELEM(self->ti_tuple));
 if DEE_UNLIKELY(elem_pos >= self->ti_end) return 1; // End of iterator
 Dee_INCREF(*result = *elem_pos);
 return 0;
}


static int DEE_CALL _deetupleiterator_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTupleIteratorObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_size_t used_position; Dee_uint64_t read_position;
 if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&read_position) != 0) return -1;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_position > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Tuple iterator position is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_position,(Dee_size_t)-1);
  return -1;
 }
#endif
 used_position = (Dee_size_t)read_position;
 if DEE_UNLIKELY((self->ti_tuple = (DeeTupleObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if DEE_UNLIKELY(DeeNone_Check(self->ti_tuple)) { Dee_DECREF(self->ti_tuple); Dee_INCREF(self->ti_tuple = (DeeTupleObject *)Dee_EmptyTuple); }
 else if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)self->ti_tuple,&DeeTuple_Type) != 0) { Dee_DECREF(self->ti_tuple); return -1; }
 self->ti_pos = DeeTuple_ELEM(self->ti_tuple)+used_position;
 self->ti_end = DeeTuple_ELEM(self->ti_tuple)+DeeTuple_SIZE(self->ti_tuple);
 return 0;
}

static int DEE_CALL _deetupleiterator_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTupleIteratorObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 DeeObject **iterator_pos; Dee_uint64_t written_pos;
 iterator_pos = (DeeObject **)DeeAtomicPtr_Load(self->ti_pos,memory_order_seq_cst);
 written_pos = (Dee_uint64_t)((Dee_size_t)(iterator_pos-DeeTuple_ELEM(self->ti_tuple)));
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,written_pos) != 0) return -1;
 // NOTE: The empty-tuple situation is unlikely, since empty
 //       tuples should be written as builtin objects.
 return DeeMarshal_WriteObjectWithMap(outfile,map,DEE_UNLIKELY(DeeTuple_CheckEmpty(self->ti_tuple))
                                      ? Dee_None : (DeeObject *)self->ti_tuple);
}

static struct DeeTypeMarshal _deetupleiterator_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(tuple_iterator)),
 member(&_deetupleiterator_tp_marshal_ctor),member(&_deetupleiterator_tp_marshal_put));


DeeTypeObject DeeTupleIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("tuple_iterator"),null,null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeTupleIteratorObject),null,
  null,member(&_deetupleiterator_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deetupleiterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deetupleiterator_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deetupleiterator_tp_cmp_lo),
  member(&_deetupleiterator_tp_cmp_le),
  member(&_deetupleiterator_tp_cmp_eq),
  member(&_deetupleiterator_tp_cmp_ne),
  member(&_deetupleiterator_tp_cmp_gr),
  member(&_deetupleiterator_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,
  member(&_deetupleiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deetupleiterator_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_DECL_END

#ifndef __INTELLISENSE__
#include "iterator.zip_iterator.inl"
#include "tuple.unpack.inl"
#endif

#endif /* !GUARD_DEEMON_TUPLE_C */
