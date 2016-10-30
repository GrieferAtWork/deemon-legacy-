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
#ifndef GUARD_DEEMON_LIST_C
#define GUARD_DEEMON_LIST_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include DEE_INCLUDE_MEMORY_API()

// /include/*
#include <deemon/bool.h>
#include <deemon/cell.h>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/gc.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/type_util.h>
#include <deemon/optional/object_visit.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/list.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/super.h>
#include <deemon/string.h>
#include <deemon/tuple.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>
#include <deemon/runtime/builtin_functions.h>


/*
NOTE:
  There are a lot for for-loops in here that look like this:
  >> for (i = 0; i < DeeList_SIZE(self); ++i) 
  I'm fully aware that I continuously check the size of a given list,
  something I actually do on purpose as these loops always contain
  code that could potentially modify the list represented by 'self'.
  - So don't go and ~optimize~ those loops as you would break the runtime
*/

DEE_DECL_BEGIN

#define _DeeList_InitEmpty(ob)\
do{\
 ((DeeListObject *)(ob))->l_alloc = 0;\
 ((DeeListObject *)(ob))->l_len = 0;\
 ((DeeListObject *)(ob))->l_elem = NULL;\
 _DeeList_InitCommon(ob);\
}while(0)


static DEE_A_RET_EXCEPT(-1) int DeeList_InitCopy(
 DEE_A_IN DeeListObject *self,
 DEE_A_IN_OBJECT(DeeListObject) const *src) {
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(src) && DeeList_Check(src));
again:
 DeeList_ACQUIRE(src);
 if ((self->l_alloc = self->l_len = DeeList_SIZE(src)) != 0) {
  DeeObject **elem_dst,*const *elem_src,**elem_dst_end;
  if DEE_UNLIKELY((elem_dst = (DeeObject **)mallocf_nz(
   self->l_len*sizeof(DeeObject *),"List copy of %p in %p",src,self)) == NULL) {
   DeeList_RELEASE(src);
   if DEE_LIKELY(Dee_CollectMemory()) goto again;
   DeeError_NoMemory();
   return -1;
  }
  self->l_elem = elem_dst;
  elem_dst_end = elem_dst+self->l_len;
  elem_src = DeeList_ELEM(src);
  do Dee_INCREF(*elem_dst++ = *elem_src++);
  while(elem_dst != elem_dst_end);
 } else self->l_elem = NULL;
 DeeList_RELEASE(src);
 _DeeList_InitCommon(self);
 return 0;
}
static DEE_A_RET_EXCEPT(-1) int DeeList_InitDeepCopy(
 DEE_A_IN DeeListObject *self,
 DEE_A_IN_OBJECT(DeeListObject) const *src) {
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(src) && DeeList_Check(src));
again:
 DeeList_ACQUIRE(src);
 if ((self->l_alloc = self->l_len = DeeList_SIZE(src)) != 0) {
  DeeObject *ob_copy,**elem_dst,*const *elem_src,**elem_dst_end;
  if DEE_LIKELY((elem_dst = (DeeObject **)mallocf_nz(
   self->l_len*sizeof(DeeObject *),"List copy of %p in %p",src,self)
   ) == NULL) {
   DeeList_RELEASE(src);
   if DEE_LIKELY(Dee_CollectMemory()) goto again;
   DeeError_NoMemory();
   return -1;
  }
  self->l_elem = elem_dst;
  elem_dst_end = elem_dst+self->l_len;
  elem_src = DeeList_ELEM(src);
  // Step #1: Copy references into the new list
  do Dee_INCREF(*elem_dst++ = *elem_src++);
  while DEE_LIKELY(elem_dst != elem_dst_end);
  DeeList_RELEASE(src); // Unlock the source list
  elem_dst = self->l_elem;
  // Step #2: Inplace-copy all items of the new list
  do {
   if DEE_UNLIKELY((ob_copy = DeeObject_DeepCopy(*elem_dst)) == NULL) {
    elem_dst = self->l_elem+self->l_len;
    while DEE_LIKELY(elem_dst != self->l_elem) Dee_DECREF(*--elem_dst);
    free_nn(self->l_elem);
    return -1;
   }
   Dee_DECREF(*elem_dst);
   Dee_INHERIT_REF(*elem_dst,ob_copy);
  } while DEE_LIKELY(++elem_dst != elem_dst_end);
 } else {
  DeeList_RELEASE(src);
  self->l_elem = NULL;
 }
 _DeeList_InitCommon(self);
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeList_InitFromIterator(
 DEE_A_INOUT DeeListObject *self, DEE_A_INOUT DeeObject *iterator) {
 DeeObject *elem,**iter,**new_list,**new_list2; int error;
 Dee_size_t new_size,new_alloc;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(iterator));
 if DEE_UNLIKELY((error = DeeObject_IterNextEx(iterator,&elem)) != 0) {
  if DEE_UNLIKELY(error < 0) return error;
  _DeeList_InitEmpty(self);
  return 0;
 }
 new_alloc = 2,new_size = 1;
 if DEE_UNLIKELY((new_list = (DeeObject **)
  malloc_nz(2*sizeof(DeeObject *))) == NULL) {
no_mem:
  DeeError_NoMemory();
  Dee_DECREF(elem);
  return -1;
 }
 Dee_INHERIT_REF(*new_list,elem);
 while DEE_LIKELY((error = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  if DEE_UNLIKELY(new_size == new_alloc) { // reallocate new list
   if DEE_UNLIKELY((new_list2 = (DeeObject **)realloc_nnz(new_list,(
    new_alloc *= 2)*sizeof(DeeObject *))) == NULL) {
    iter = new_list+new_size;
    while DEE_LIKELY(iter != new_list) Dee_DECREF(*--iter);
    free_nn(new_list);
    goto no_mem;
   }
   new_list = new_list2;
  }
  Dee_INHERIT_REF(new_list[new_size++],elem);
 }
 if DEE_UNLIKELY(error < 0) {
  iter = new_list+new_size;
  while DEE_LIKELY(iter != new_list) Dee_DECREF(*--iter);
  free_nn(new_list);
  return error;
 }
 self->l_alloc = new_alloc;
 self->l_len = new_size;
 self->l_elem = new_list;
 _DeeList_InitCommon(self);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeList_InitFromVector(
 DEE_A_OUT DeeListObject *self,
 DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p) {
 DeeObject **new_list,**iter,**end;
again:
 if DEE_UNLIKELY((new_list = (DeeObject **)mallocf_nz(n*sizeof(DeeObject *),
  "Initialized list (size: %u; elem: %p...)",(unsigned int)n,p)) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return -1;
 }
 end = (iter = new_list)+n;
 while DEE_LIKELY(iter != end) Dee_INCREF(*iter++ = *p++);
 self->l_alloc =
 self->l_len = n;
 self->l_elem = new_list;
 _DeeList_InitCommon(self);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeList_WeakInitFromArray(
 DEE_A_OUT DeeListObject *self, DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p) {
 DeeObject **new_list,**iter,**end;
 if DEE_UNLIKELY((new_list = (DeeObject **)mallocf_nz(n*sizeof(DeeObject *),
  "Initialized list (size: %u; elem: %p...)",(unsigned int)n,p)) == NULL) {
  DeeError_NoMemory();
  return -1;
 }
 end = (iter = new_list)+n;
 while DEE_LIKELY(iter != end) Dee_INCREF(*iter++ = *p++);
 self->l_alloc =
 self->l_len = n;
 self->l_elem = new_list;
 _DeeList_InitCommon(self);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeList_TryWeakInitFromArray(
 DEE_A_OUT DeeListObject *self, DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p) {
 DeeObject **new_list,**iter,**end;
 if DEE_UNLIKELY((new_list = (DeeObject **)mallocf_nz(n*sizeof(DeeObject *),
  "Initialized list (size: %u; elem: %p...)",(unsigned int)n,p)
  ) == NULL) return -1;
 end = (iter = new_list)+n;
 while DEE_LIKELY(iter != end) Dee_INCREF(*iter++ = *p++);
 self->l_alloc =
 self->l_len = n;
 self->l_elem = new_list;
 _DeeList_InitCommon(self);
 return 0;
}
#define DeeList_InitFromSequence(self,sequence) \
 DeeList_TInitFromSequence(self,Dee_TYPE(sequence),sequence)
DEE_A_RET_EXCEPT(-1) int DeeList_TInitFromSequence(
 DEE_A_INOUT DeeListObject *self,
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) {
 int result;
 DEE_ASSERT(DeeObject_Check(sequence));
 DEE_ASSERT(DeeObject_Check(tp_sequence) && DeeType_Check(tp_sequence));
 DEE_ASSERT(DeeObject_InstanceOf(sequence,tp_sequence));
 if (tp_sequence == &DeeList_Type) return DeeList_InitCopy(self,sequence);
 if (tp_sequence == &DeeTuple_Type) return DeeList_InitFromVector(
  self,DeeTuple_SIZE(sequence),DeeTuple_ELEM(sequence));
 if (tp_sequence == &DeeCell_Type) {
  if ((sequence = DeeCell_TryGet(sequence)) == NULL) { _DeeList_InitEmpty(self); return 0; }
  result = DeeList_InitFromVector(self,1,&sequence);
  Dee_DECREF(sequence);
  return result;
 }
 if (tp_sequence == &DeeSuper_Type) return DeeList_TInitFromSequence(
  self,DeeSuper_TYPE(sequence),DeeSuper_SELF(sequence));
 if DEE_UNLIKELY((sequence = DeeObject_TIterSelf(tp_sequence,sequence)) == NULL) return -1;
 result = DeeList_InitFromIterator(self,sequence);
 Dee_DECREF(sequence);
 return result;
}





DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeList_New(void) {
 DeeListObject *result;
 if DEE_LIKELY((result = DeeGC_MALLOC(DeeListObject)) != NULL) {
  DeeObject_INIT(result,&DeeList_Type);
  _DeeList_InitCommon(result);
  result->l_len = 0;
  result->l_alloc = 0;
  result->l_elem = NULL;
  DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeList_NewFromVector(
 DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p) {
 DeeListObject *result;
 if DEE_LIKELY((result = DeeGC_MALLOC(DeeListObject)) != NULL) {
  DeeObject_INIT(result,&DeeList_Type);
  if DEE_UNLIKELY(DeeList_InitFromVector(result,n,p) != 0) {
   _DeeGCObject_Delete((DeeObject *)result);
   result = NULL;
  } else DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeList_TNewFromSequence(
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) {
 DeeListObject *result;
 DEE_ASSERT(DeeObject_Check(sequence));
 DEE_ASSERT(DeeObject_Check(tp_sequence) && DeeType_Check(tp_sequence));
 DEE_ASSERT(DeeObject_InstanceOf(sequence,tp_sequence));
 if DEE_LIKELY((result = DeeGC_MALLOC(DeeListObject)) != NULL) {
  DeeObject_INIT(result,&DeeList_Type);
  if DEE_UNLIKELY(DeeList_TInitFromSequence(result,tp_sequence,sequence) != 0) {
   _DeeGCObject_Delete((DeeObject *)result);
   result = NULL;
  } else DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *
DeeList_NewFromIterator(DEE_A_INOUT DeeObject *iterator) {
 DeeListObject *result;
 DEE_ASSERT(DeeObject_Check(iterator));
 if DEE_LIKELY((result = DeeGC_MALLOC(DeeListObject)) != NULL) {
  DeeObject_INIT(result,&DeeList_Type);
  if DEE_UNLIKELY(DeeList_InitFromIterator(result,iterator) != 0) {
   _DeeGCObject_Delete((DeeObject *)result);
   result = NULL;
  } else DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *
_DeeList_NewUnsafe(DEE_A_IN Dee_size_t size) {
 DeeListObject *result;
 if DEE_LIKELY((result = (DeeListObject *)DeeList_New()) != NULL && size) {
  // The unsafe part is, that list elements must be non-zero
  // >> Here we break that rule and rely on the caller to fill
  //    everything properly before releasing the list into the world...
  if DEE_LIKELY((result->l_elem = (DeeObject **)mallocf_nz(size*sizeof(DeeObject *),
   "Unsafe list (%u entries for list %p)",(unsigned)size,result)) != NULL) {
   result->l_len = result->l_alloc = size;
  } else {
   DeeError_NoMemory();
   Dee_DECREF(result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeListObject) *
_DeeList_TryWeakNewUnsafe(DEE_A_IN Dee_size_t size) {
 DeeListObject *result;
 if DEE_LIKELY((result = (DeeListObject *)DeeGC_TryWeakMalloc(sizeof(DeeListObject))) != NULL) {
  DeeObject_INIT(result,&DeeList_Type);
  _DeeList_InitCommon(result);
  if (size) {
   // The unsafe part is, that list elements must be non-zero
   // >> Here we break that rule and rely on the caller to fill
   //    everything properly before releasing the list into the world...
   if DEE_LIKELY((result->l_elem = (DeeObject **)mallocf_nz(size*sizeof(DeeObject *),
    "Unsafe list (%u entries for list %p)",(unsigned)size,result)) != NULL) {
    result->l_len = result->l_alloc = size;
    DeeGC_TrackedAdd((DeeObject *)result);
   } else {
    _DeeObject_DELETE(&DeeList_Type,result);
    result = NULL;
   }
  } else {
   result->l_len = 0;
   result->l_alloc = 0;
   result->l_elem = NULL;
   DeeGC_TrackedAdd((DeeObject *)result);
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_EXCEPT(-1) int _DeeList_ResizeUnsafe(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t new_size) {
 DeeObject **new_elem;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeList_SIZE(self) != new_size && "No change in size");
 if (new_size) {
  while DEE_UNLIKELY((new_elem = (DeeObject **)realloc_nz(
   ((DeeListObject *)self)->l_elem,new_size*sizeof(DeeObject *))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
  ((DeeListObject *)self)->l_elem = new_elem;
  ((DeeListObject *)self)->l_len =
  ((DeeListObject *)self)->l_alloc = new_size;
 } else {
  free(((DeeListObject *)self)->l_elem);
 }
 return 0;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *
DeeList_NewWithSizeHint(DEE_A_IN Dee_size_t hint) {
 DeeListObject *result;
 if DEE_LIKELY((result = (DeeListObject *)DeeList_New()) != NULL && hint) {
  result->l_elem = (DeeObject **)mallocf_nz(
   hint*sizeof(DeeObject *),"Sizehint %u for list %p",(unsigned)hint,result);
  if DEE_LIKELY(result->l_elem) result->l_alloc = hint;
 }
 return (DeeObject *)result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *
DeeList_Copy(DEE_A_IN_OBJECT(DeeListObject) const *self) {
 DeeListObject *result;
 if DEE_LIKELY((result = DeeGC_MALLOC(DeeListObject)) != NULL) {
  DeeObject_INIT(result,&DeeList_Type);
  if DEE_UNLIKELY(DeeList_InitCopy(result,self) != 0) {
   _DeeObject_DELETE(&DeeList_Type,result);
   result = NULL;
  } else DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *
DeeList_DeepCopy(DEE_A_IN_OBJECT(DeeListObject) const *self) {
 DeeListObject *result;
 if DEE_LIKELY((result = DeeGC_MALLOC(DeeListObject)) != NULL) {
  DeeObject_INIT(result,&DeeList_Type);
  if DEE_UNLIKELY(DeeList_InitDeepCopy(result,self) != 0) {
   _DeeObject_DELETE(&DeeList_Type,result);
   result = NULL;
  } else DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}
DEE_A_RET_EXCEPT(-1) int DeeList_CopyAssign(
 DEE_A_INOUT_OBJECT(DeeListObject) *self,
 DEE_A_IN_OBJECT(DeeListObject) const *right) {
 Dee_size_t new_size,old_size;
 DeeObject **new_list,**old_list,**iter,**end,**src;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeList_Check(right));
 DeeList_ACQUIRE(right);
 if DEE_LIKELY((new_size = DeeList_SIZE(right)) != 0) {
  new_list = (DeeObject **)mallocf_nz(
   new_size*sizeof(DeeObject *),"List copy of %p in %p",right,self);
  if DEE_UNLIKELY(!new_list) { DeeList_RELEASE(right); DeeError_NoMemory(); return -1; }
  end = (iter = new_list)+new_size,src = DeeList_ELEM(right);
  do Dee_INCREF(*iter++ = *src++);
  while DEE_LIKELY(iter != end);
 } else new_list = NULL;
 DeeList_RELEASE(right);
 // Replace the old list
 DeeList_ACQUIRE(self);
 old_size = DeeList_SIZE(self);
 old_list = DeeList_ELEM(self);
 ((DeeListObject *)self)->l_alloc =
 ((DeeListObject *)self)->l_len = new_size;
 ((DeeListObject *)self)->l_elem = new_list;
 DeeList_RELEASE(self);
 end = (iter = old_list)+old_size;
 while DEE_LIKELY(iter != end) Dee_DECREF(*iter++);
 free(old_list);
 return 0;
}
void DeeList_MoveAssign(
 DEE_A_INOUT_OBJECT(DeeListObject) *self,
 DEE_A_INOUT_OBJECT(DeeListObject) *right) {
 Dee_size_t new_alloc,new_size,old_size; DeeObject **iter,**end,**new_list,**old_list;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeList_Check(right));
 DeeList_ACQUIRE(right);
 new_list = DeeList_ELEM(right);
 new_size = DeeList_SIZE(right);
 new_alloc = DeeList_ALLOC(right);
 ((DeeListObject *)right)->l_alloc = 0;
 ((DeeListObject *)right)->l_len = 0;
 ((DeeListObject *)right)->l_elem = NULL;
 DeeList_RELEASE(right);
 DeeList_ACQUIRE(self);
 old_size = DeeList_SIZE(self);
 old_list = DeeList_ELEM(self);
 ((DeeListObject *)self)->l_alloc = new_alloc;
 ((DeeListObject *)self)->l_len = new_size;
 ((DeeListObject *)self)->l_elem = new_list;
 DeeList_RELEASE(self);
 end = (iter = old_list)+old_size;
 while DEE_LIKELY(iter != end) Dee_DECREF(*iter++);
 free(old_list);
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeList_TAssignSequence(
 DEE_A_INOUT_OBJECT(DeeListObject) *self,
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(sequence));
 DEE_ASSERT(DeeObject_Check(tp_sequence) && DeeType_Check(tp_sequence));
 DEE_ASSERT(DeeObject_InstanceOf(sequence,tp_sequence));
 if (tp_sequence == &DeeList_Type) return DeeList_CopyAssign(self,sequence);
 if (tp_sequence == &DeeTuple_Type) return DeeList_AssignVector(self,DeeTuple_SIZE(sequence),DeeTuple_ELEM(sequence));
 if (tp_sequence == &DeeCell_Type) {
  if ((sequence = DeeCell_TryGet(sequence)) == NULL) { DeeList_Clear(self); return 0; }
  result = DeeList_AssignVector(self,1,&sequence);
  Dee_DECREF(sequence);
  return result;
 }
 if (tp_sequence == &DeeSuper_Type) return DeeList_TAssignSequence(
  self,DeeSuper_TYPE(sequence),DeeSuper_SELF(sequence));
 if DEE_UNLIKELY((sequence = DeeObject_TIterSelf(tp_sequence,sequence)) == NULL) return -1;
 result = DeeList_AssignIterator(self,sequence);
 Dee_DECREF(sequence);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeList_AssignIterator(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *iterator) {
 DeeObject *elem,**iter,**new_list,**new_list2,**old_list,**end;
 Dee_size_t old_size,new_size,new_alloc; int error;
 if DEE_LIKELY((error = DeeObject_IterNextEx(iterator,&elem)) != 0) {
  if (error < 0) return error;
  DeeList_Clear(self);
  return 0;
 }
 new_alloc = 2,new_size = 1;
 if DEE_UNLIKELY((new_list = (DeeObject **)malloc_nz(2*sizeof(DeeObject *))) == NULL) {
no_mem:
  DeeError_NoMemory();
  Dee_DECREF(elem);
  return -1;
 }
 Dee_INHERIT_REF(*new_list,elem);
 while DEE_LIKELY((error = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  if DEE_UNLIKELY(new_size == new_alloc) { // reallocate new list
   if DEE_UNLIKELY((new_list2 = (DeeObject **)realloc_nnz(new_list,(
    new_alloc *= 2)*sizeof(DeeObject *))) == NULL) {
    iter = new_list+new_size;
    while (iter != new_list) Dee_DECREF(*--iter);
    free_nn(new_list);
    goto no_mem;
   }
   new_list = new_list2;
  }
  Dee_INHERIT_REF(new_list[new_size++],elem);
 }
 if DEE_UNLIKELY(error < 0) {
  iter = new_list+new_size;
  while DEE_LIKELY(iter != new_list) Dee_DECREF(*--iter);
  free_nn(new_list);
  return -1;
 }
 DeeList_ACQUIRE(self);
 old_list = DeeList_ELEM(self);
 old_size = DeeList_SIZE(self);
 ((DeeListObject *)self)->l_alloc = new_alloc;
 ((DeeListObject *)self)->l_len = new_size;
 ((DeeListObject *)self)->l_elem = new_list;
 DeeList_RELEASE(self);
 end = (iter = old_list)+old_size;
 while (iter != end) Dee_DECREF(*iter++);
 free(old_list);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeList_AssignVector(
 DEE_A_INOUT_OBJECT(DeeListObject) *self,
 DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p) {
 DeeObject **old_list,**new_list,**iter,**end; Dee_size_t old_size;
 while DEE_UNLIKELY((new_list = (DeeObject **)mallocf_nz(n*sizeof(DeeObject *),
  "Assigned list (size: %u; elem: %p...)",(unsigned int)n,p)) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 end = (iter = new_list)+n;
 while (iter != end) Dee_INCREF(*iter++ = *p++);
 DeeList_ACQUIRE(self);
 old_list = DeeList_ELEM(self);
 old_size = DeeList_SIZE(self);
 ((DeeListObject *)self)->l_alloc =
 ((DeeListObject *)self)->l_len = n;
 ((DeeListObject *)self)->l_elem = new_list;
 DeeList_RELEASE(self);
 end = (iter = old_list)+old_size;
 while (iter != end) Dee_DECREF(*iter++);
 free(old_list);
 return 0;
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeList_Back(
 DEE_A_IN_OBJECT(DeeListObject) const *self) {
 DeeObject *result; Dee_size_t size;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeList_ACQUIRE(self);
 if DEE_LIKELY((size = DeeList_SIZE(self)) != 0) {
  Dee_INCREF(result = DeeList_GET(self,size-1));
  DeeList_RELEASE(self);
 } else {
  DeeList_RELEASE(self);
  result = NULL;
  DeeError_ValueError_EmptyList(&DeeList_Type);
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeList_Front(
 DEE_A_IN_OBJECT(DeeListObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeObject *result; Dee_size_t size;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeList_ACQUIRE(self);
 if DEE_LIKELY((size = DeeList_SIZE(self)) != 0) {
  Dee_INCREF(result = DeeList_GET(self,0));
  DeeList_RELEASE(self);
 } else {
  DeeList_RELEASE(self);
  result = NULL;
  DeeError_ValueError_EmptyList(&DeeList_Type);
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeList_Get(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_IN Dee_ssize_t i) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeList_ACQUIRE(self);
 if DEE_UNLIKELY(DeeList_EMPTY(self)) {
  DeeList_RELEASE(self);
  DeeError_ValueError_EmptyList(&DeeList_Type);
  return NULL;
 }
 DEE_PRIVATE_CLAMP_INDEX(i,DeeList_SIZE(self));
 Dee_INCREF(result = DeeList_GET(self,(Dee_size_t)i));
 DeeList_RELEASE(self);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeList_At(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_IN Dee_size_t i) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeList_ACQUIRE(self);
 if DEE_UNLIKELY(i >= DeeList_SIZE(self)) {
  DeeList_RELEASE(self);
  DeeError_IndexError((DeeObject *)self,i);
  return NULL;
 }
 Dee_INCREF(result = DeeList_GET(self,i));
 DeeList_RELEASE(self);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeList_Set(
 DEE_A_INOUT_OBJECT(DeeListObject) *self,
 DEE_A_IN Dee_ssize_t i, DEE_A_INOUT DeeObject *v) {
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeObject *old_elem;
 DeeList_ACQUIRE(self);
 if DEE_UNLIKELY(DeeList_EMPTY(self)) {
  DeeList_RELEASE(self);
  DeeError_ValueError_EmptyList(&DeeList_Type);
  return -1;
 }
 DEE_PRIVATE_CLAMP_INDEX(i,DeeList_SIZE(self));
 old_elem = DeeList_GET(self,(Dee_size_t)i);
 DeeList_SET(self,(Dee_size_t)i,v);
 Dee_INCREF(v);
 DeeList_RELEASE(self);
 Dee_DECREF(old_elem);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeList_SetAt(
 DEE_A_INOUT_OBJECT(DeeListObject) *self,
 DEE_A_IN Dee_size_t i, DEE_A_INOUT DeeObject *v) {
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeObject *old_elem;
 DeeList_ACQUIRE(self);
 if DEE_UNLIKELY(i >= DeeList_SIZE(self)) {
  DeeList_RELEASE(self);
  DeeError_IndexError((DeeObject *)self,i);
  return -1;
 }
 old_elem = DeeList_GET(self,i);
 DeeList_SET(self,i,v);
 Dee_INCREF(v);
 DeeList_RELEASE(self);
 Dee_DECREF(old_elem);
 return 0;
}


DEE_A_RET_WUNUSED Dee_size_t DeeList_Size(
 DEE_A_IN_OBJECT(DeeListObject) const *self) {
 Dee_size_t result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeList_ACQUIRE(self);
 result = DeeList_SIZE(self);
 DeeList_RELEASE(self);
 return result;
}
DEE_A_RET_WUNUSED Dee_size_t DeeList_Alloc(
 DEE_A_IN_OBJECT(DeeListObject) const *self) {
 Dee_size_t result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeList_ACQUIRE(self);
 result = DeeList_ALLOC(self);
 DeeList_RELEASE(self);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *
DeeList_AsTuple(DEE_A_IN_OBJECT(DeeListObject) const *self) {
 DeeObject *result,**iter,**end,**src;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
again:
 DeeList_ACQUIRE(self);
 if DEE_LIKELY((result = _DeeTuple_TryWeakNewUnsafe(DeeList_SIZE(self))) != NULL) {
  end = (iter = DeeTuple_ELEM(result))+DeeList_SIZE(self);
  src = DeeList_ELEM(self);
  while DEE_LIKELY(iter != end) Dee_INCREF(*iter++ = *src++);
  DeeList_RELEASE(self);
 } else {
  DeeList_RELEASE(self);
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleIteratorObject) *
DeeList_IterSelf(DEE_A_INOUT_OBJECT(DeeListObject) *self) {
 DeeListIteratorObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeListIteratorObject,"list.iterator(%p)",self)) != NULL) {
  DeeObject_INIT(result,&DeeListIterator_Type);
  Dee_INCREF(result->li_list = (DeeListObject *)self);
  result->li_pos = 0;
 }
 return (DeeObject *)result;
}

DEE_A_RET_EXCEPT(-1) int DeeList_Del(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_ssize_t i) {
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeObject *result; Dee_size_t list_size;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeList_ACQUIRE(self);
 if DEE_UNLIKELY((list_size = DeeList_SIZE(self)) == 0) {
  DeeList_RELEASE(self);
  DeeError_ValueError_EmptyList(&DeeList_Type);
  return -1;
 }
 DEE_PRIVATE_CLAMP_INDEX(i,list_size);
 result = DeeList_GET(self,i);
 memmove(DeeList_ELEM(self)+i,DeeList_ELEM(self)+(i+1),
        (list_size-(i+1))*sizeof(DeeObject *));
 --((DeeListObject *)self)->l_len;
 DeeList_RELEASE(self);
 Dee_DECREF(result);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeList_DelAt(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t i) {
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeObject *result; Dee_size_t list_size;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeList_ACQUIRE(self);
 list_size = DeeList_SIZE(self);
 if DEE_UNLIKELY(i >= list_size) {
  DeeList_RELEASE(self);
  DeeError_IndexError(self,i);
  return -1;
 }
 result = DeeList_GET(self,i);
 memmove(DeeList_ELEM(self)+i,DeeList_ELEM(self)+(i+1),
        (list_size-(i+1))*sizeof(DeeObject *));
 --((DeeListObject *)self)->l_len;
 DeeList_RELEASE(self);
 Dee_DECREF(result);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeList_Traverse(
 DEE_A_IN_OBJECT(DeeListObject) const *self,
 DEE_A_IN DeeObjectIterateFunc func, DEE_A_IN_OPT void *closure) {
 Dee_size_t i; DeeObject *elem; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(func);
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  temp = (*func)(elem,closure);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(temp != 0) return temp;
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 return 0;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeList_GetRange(
 DEE_A_IN_OBJECT(DeeListObject) const *self,
 DEE_A_IN Dee_size_t lo, DEE_A_IN Dee_size_t hi) {
 DeeListObject *result; DeeObject **result_elem,**end,**src;
 Dee_size_t used_hi,result_size;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 if DEE_LIKELY((result = DeeGC_MALLOC(DeeListObject)) != NULL) {
  DeeObject_INIT(result,&DeeList_Type);
  _DeeList_InitCommon(result);
again: used_hi = hi;
  DeeList_ACQUIRE(self);
  if DEE_UNLIKELY(used_hi > DeeList_SIZE(self)) used_hi = DeeList_SIZE(self);
  if DEE_LIKELY(used_hi > lo) {
   result_size = (Dee_size_t)(used_hi-lo);
   if DEE_UNLIKELY((result_elem = (DeeObject **)malloc_nz(
    result_size*sizeof(DeeObject *))) == NULL) {
    DeeList_RELEASE(self);
    if DEE_LIKELY(Dee_CollectMemory()) goto again;
    _DeeGCObject_Delete((DeeObject *)result);
    DeeError_NoMemory();
    return NULL;
   }
   result->l_elem = result_elem;
   result->l_alloc = result->l_len = result_size;
   src = DeeList_ELEM(self)+lo;
   end = result_elem+result_size;
   while DEE_LIKELY(result_elem != end) Dee_INCREF(*result_elem++ = *src++);
  } else {
   result->l_elem = NULL;
   result->l_alloc = 0;
   result->l_len = 0;
  }
  DeeList_RELEASE(self);
  DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}
DEE_A_RET_EXCEPT(-1) int DeeList_TSetRange(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t lo,
 DEE_A_IN Dee_size_t hi, DEE_A_IN DeeTypeObject const *tp_v, DEE_A_INOUT DeeObject *v) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(v));
 DEE_ASSERT(DeeObject_Check(tp_v) && DeeType_Check(tp_v));
 DEE_ASSERT(DeeObject_InstanceOf(v,tp_v));
 if (tp_v == &DeeTuple_Type) return DeeList_SetRangeVector(
  self,lo,hi,DeeTuple_SIZE(v),DeeTuple_ELEM(v));
 if (tp_v == &DeeList_Type) {
  // Let's make this simple...
  if DEE_UNLIKELY((v = DeeList_Copy(v)) == NULL) return -1;
  result = DeeList_SetRangeVector(self,lo,hi,DeeList_SIZE(v),DeeList_ELEM(v));
  Dee_DECREF(v);
  return result;
 }
 if DEE_UNLIKELY((v = DeeObject_TIterSelf(tp_v,v)) == NULL) return -1;
 result = DeeList_SetRangeIterator(self,lo,hi,v);
 Dee_DECREF(v);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeList_SetRangeVector(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t lo,
 DEE_A_IN Dee_size_t hi, DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p) {
 DeeObject **new_elem,**old_elem,**iter,**end;
 Dee_size_t new_size,old_size,used_hi;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
again: used_hi = hi;
 DeeList_ACQUIRE(self);
 if DEE_UNLIKELY(used_hi > DeeList_SIZE(self)) used_hi = DeeList_SIZE(self);
 if ((old_size = (Dee_size_t)(used_hi-lo)) != 0) {
  // Temporary buffer for old elements
  if DEE_UNLIKELY((old_elem = (DeeObject **)malloc_nz(
   old_size*sizeof(DeeObject *))) == NULL) {
no_mem: DeeList_RELEASE(self);
   if DEE_LIKELY(Dee_CollectMemory()) goto again;
   DeeError_NoMemory();
   return -1;
  }
  // Preserve old elements (will be deallocated later)
  memcpy(old_elem,DeeList_ELEM(self)+lo,old_size*sizeof(DeeObject *));
 } else old_elem = NULL;
 if ((new_size = (DeeList_SIZE(self)-old_size)+n) != 0) {
  new_elem = DeeList_ELEM(self);
  if (new_size > DeeList_ALLOC(self)) { // Must reallocate list (not enough buffer)
   // Reallocate the list (Note how we use 'realloc_nz' instead of 'realloc_nnz')
   if DEE_UNLIKELY((new_elem = (DeeObject **)realloc_nz(new_elem,new_size*sizeof(DeeObject *))
    ) == NULL) { free(old_elem); goto no_mem; }
   // Update the list pointer and allocated size
   ((DeeListObject *)self)->l_elem = new_elem;
   ((DeeListObject *)self)->l_alloc = new_size;
  } // NOTE: If we are below the allocated size and wanted to realloc,
    //       we'd have to run the following memmove first.
  // Move the list portions above the high point to the end of the insert portion
  memmove(new_elem+(lo+n),new_elem+used_hi,(DeeList_SIZE(self)-used_hi)*sizeof(DeeObject *));
  // Fill the newly acquires space with the given array
  end = (iter = new_elem+lo)+n;
  while DEE_LIKELY(iter != end) Dee_INCREF(*iter++ = *p++);
 } // NOTE: If the list is now empty, that's ok (keep the preallocate cache)
 // Always update the size, though
 ((DeeListObject *)self)->l_len = new_size;
 DeeList_RELEASE(self);
 // Cleanup the old elements
 if (old_elem) {
  end = (iter = old_elem)+old_size;
  while DEE_LIKELY(iter != end) Dee_DECREF(*iter++);
  free_nn(old_elem);
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeList_SetRangeIterator(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t lo,
 DEE_A_IN Dee_size_t hi, DEE_A_INOUT DeeObject *iterator) {
 DeeObject *temp_cache; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(iterator));
 // Let's make this simple...
 if DEE_UNLIKELY((temp_cache = DeeList_NewFromIterator(iterator)) == NULL) return -1;
 result = DeeList_SetRangeVector(self,lo,hi,DeeList_SIZE(temp_cache),DeeList_ELEM(temp_cache));
 Dee_DECREF(temp_cache);
 return result;
}



DEE_A_RET_EXCEPT(-1) int DeeList_Insert(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t i,
 DEE_A_INOUT DeeObject *elem) {
 DeeObject **new_elem;
 Dee_size_t new_size,used_i;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
again: used_i = i;
 DeeList_ACQUIRE(self);
 if DEE_UNLIKELY(used_i > DeeList_SIZE(self)) used_i = DeeList_SIZE(self);
 new_size = DeeList_SIZE(self)+1;
 new_elem = DeeList_ELEM(self);
 if (new_size > DeeList_ALLOC(self)) { // Must reallocate list (not enough buffer)
  // Reallocate the list (Note how we use 'realloc_nz' instead of 'realloc_nnz')
  if DEE_UNLIKELY((new_elem = (DeeObject **)realloc_nz(
   new_elem,new_size*sizeof(DeeObject *))) == NULL) {
   DeeList_RELEASE(self);
   if DEE_LIKELY(Dee_CollectMemory()) goto again;
   DeeError_NoMemory();
   return -1;
  }
  // Update the list pointer and allocated size
  ((DeeListObject *)self)->l_elem = new_elem;
  ((DeeListObject *)self)->l_alloc = new_size;
 } // NOTE: If we are below the allocated size and wanted to realloc,
   //       we'd have to run the following memmove first.
 // Move the list portions above the high point to the end of the insert portion
 memmove(new_elem+(i+1),new_elem+i,(DeeList_SIZE(self)-i)*sizeof(DeeObject *));
 // Fill the newly acquires space with the given element
 Dee_INCREF(new_elem[i] = elem);
 // Always update the size
 ((DeeListObject *)self)->l_len = new_size;
 DeeList_RELEASE(self);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeList_InsertVector(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t i,
 DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p) {
 DeeObject **new_elem,**iter,**end;
 Dee_size_t new_size,used_i;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
again: used_i = i;
 DeeList_ACQUIRE(self);
 if DEE_UNLIKELY(used_i > DeeList_SIZE(self)) used_i = DeeList_SIZE(self);
 new_size = DeeList_SIZE(self)+n;
 new_elem = DeeList_ELEM(self);
 if (new_size > DeeList_ALLOC(self)) { // Must reallocate list (not enough buffer)
  // Reallocate the list (Note how we use 'realloc_nz' instead of 'realloc_nnz',
  // since we don't know whether or not we had a buffer to begin with)
  if DEE_UNLIKELY((new_elem = (DeeObject **)realloc_nz(
   new_elem,new_size*sizeof(DeeObject *))) == NULL) {
   DeeList_RELEASE(self);
   if DEE_LIKELY(Dee_CollectMemory()) goto again;
   DeeError_NoMemory();
   return -1;
  }
  // Update the list pointer and allocated size
  ((DeeListObject *)self)->l_elem = new_elem;
  ((DeeListObject *)self)->l_alloc = new_size;
 } // NOTE: If we are below the allocated size and wanted to realloc,
   //       we'd have to run the following memmove first.
 // Move the list portions above the high point to the end of the insert portion
 memmove(new_elem+(i+n),new_elem+i,(DeeList_SIZE(self)-i)*sizeof(DeeObject *));
 // Fill the newly acquires space with the given array
 end = (iter = new_elem+i)+n;
 while DEE_LIKELY(iter != end) Dee_INCREF(*iter++ = *p++);
 // Always update the size
 ((DeeListObject *)self)->l_len = new_size;
 DeeList_RELEASE(self);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeList_TInsertSequence(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t i,
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(sequence));
 DEE_ASSERT(DeeObject_Check(tp_sequence) && DeeType_Check(tp_sequence));
 DEE_ASSERT(DeeObject_InstanceOf(sequence,tp_sequence));
 if (tp_sequence == &DeeTuple_Type) return DeeList_InsertVector(
  self,i,DeeTuple_SIZE(sequence),DeeTuple_ELEM(sequence));
 if (tp_sequence == &DeeList_Type) {
  if DEE_UNLIKELY((sequence = DeeList_Copy(sequence)) == NULL) return -1;
  result = DeeList_InsertVector(self,i,DeeList_SIZE(sequence),DeeList_ELEM(sequence));
  Dee_DECREF(sequence);
  return result;
 }
 if (tp_sequence == &DeeCell_Type) {
  DeeObject *cell_val;
  if ((cell_val = DeeCell_TryGet(sequence)) == NULL) return 0;
  result = DeeList_Insert(self,i,cell_val);
  Dee_DECREF(cell_val);
  return result;
 }
 if (tp_sequence == &DeeSuper_Type) return DeeList_TInsertSequence(
  self,i,DeeSuper_TYPE(sequence),DeeSuper_SELF(sequence));
 if DEE_UNLIKELY((sequence = DeeObject_TIterSelf(tp_sequence,sequence)) == NULL) return -1;
 result = DeeList_InsertIterator(self,i,sequence);
 Dee_DECREF(sequence);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeList_InsertIterator(
 DEE_A_INOUT_OBJECT(DeeListObject) *self,
 DEE_A_IN Dee_size_t i, DEE_A_INOUT DeeObject *iterator) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(iterator));
 if DEE_UNLIKELY((iterator = DeeList_NewFromIterator(iterator)) == NULL) return -1;
 result = DeeList_InsertVector(self,i,DeeList_SIZE(iterator),DeeList_ELEM(iterator));
 Dee_DECREF(iterator);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeList_Append(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *elem) {
 DeeObject **new_elem; Dee_size_t new_size;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
again:
 DeeList_ACQUIRE(self);
 new_size = DeeList_SIZE(self)+1;
 new_elem = DeeList_ELEM(self);
 if (new_size > DeeList_ALLOC(self)) { // Must reallocate list (not enough buffer)
  // Reallocate the list (Note how we use 'realloc_nz' instead of 'realloc_nnz')
  if DEE_UNLIKELY((new_elem = (DeeObject **)realloc_nz(
   new_elem,new_size*sizeof(DeeObject *))) == NULL) {
   DeeList_RELEASE(self);
   if DEE_LIKELY(Dee_CollectMemory()) goto again;
   DeeError_NoMemory();
   return -1;
  }
  // Update the list pointer and allocated size
  ((DeeListObject *)self)->l_elem = new_elem;
  ((DeeListObject *)self)->l_alloc = new_size;
 }
 // Fill the newly acquires space with the given element
 Dee_INCREF(new_elem[new_size-1] = elem);
 // Always update the size
 ((DeeListObject *)self)->l_len = new_size;
 DeeList_RELEASE(self);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeList_AppendVector(
 DEE_A_INOUT_OBJECT(DeeListObject) *self,
 DEE_A_IN Dee_size_t n, DEE_A_IN_R(n) DeeObject *const *p) {
 DeeObject **new_elem,**iter,**end; Dee_size_t old_size,new_size;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
again:
 DeeList_ACQUIRE(self);
 new_size = (old_size = DeeList_SIZE(self))+n;
 new_elem = DeeList_ELEM(self);
 if (new_size > DeeList_ALLOC(self)) { // Must reallocate list (not enough buffer)
  // Reallocate the list (Note how we use 'realloc_nz' instead of 'realloc_nnz')
  if DEE_UNLIKELY((new_elem = (DeeObject **)realloc_nz(
   new_elem,new_size*sizeof(DeeObject *))) == NULL) {
   DeeList_RELEASE(self);
   if DEE_LIKELY(Dee_CollectMemory()) goto again;
   DeeError_NoMemory();
   return -1;
  }
  // Update the list pointer and allocated size
  ((DeeListObject *)self)->l_elem = new_elem;
  ((DeeListObject *)self)->l_alloc = new_size;
 }
 // Fill the newly acquires space with the given array
 end = (iter = new_elem+old_size)+n;
 while DEE_LIKELY(iter != end) Dee_INCREF(*iter++ = *p++);
 // Always update the size
 ((DeeListObject *)self)->l_len = new_size;
 DeeList_RELEASE(self);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeList_TAppendSequence(
 DEE_A_INOUT_OBJECT(DeeListObject) *self,
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(sequence));
 DEE_ASSERT(DeeObject_Check(tp_sequence) && DeeType_Check(tp_sequence));
 DEE_ASSERT(DeeObject_InstanceOf(sequence,tp_sequence));
 if (tp_sequence == &DeeTuple_Type) return DeeList_AppendVector(
  self,DeeTuple_SIZE(sequence),DeeTuple_ELEM(sequence));
 if (tp_sequence == &DeeList_Type) {
  if ((sequence = DeeList_Copy(sequence)) == NULL) return -1;
  result = DeeList_AppendVector(self,DeeList_SIZE(sequence),DeeList_ELEM(sequence));
  Dee_DECREF(sequence);
  return result;
 }
 if (tp_sequence == &DeeCell_Type) {
  DeeObject *cell_val;
  if ((cell_val = DeeCell_TryGet(sequence)) == NULL) return 0;
  result = DeeList_Append(self,cell_val);
  Dee_DECREF(cell_val);
  return result;
 }
 if (tp_sequence == &DeeSuper_Type) return DeeList_TAppendSequence(
  self,DeeSuper_TYPE(sequence),DeeSuper_SELF(sequence));
 if DEE_UNLIKELY((sequence = DeeObject_TIterSelf(tp_sequence,sequence)) == NULL) return -1;
 result = DeeList_AppendIterator(self,sequence);
 Dee_DECREF(sequence);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeList_AppendIterator(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *iterator) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(iterator));
 if DEE_UNLIKELY((iterator = DeeList_NewFromIterator(iterator)) == NULL) return -1;
 result = DeeList_AppendVector(self,DeeList_SIZE(iterator),DeeList_ELEM(iterator));
 Dee_DECREF(iterator);
 return result;
}

DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeList_Remove(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *elem) {
 Dee_size_t i; int temp; DeeObject *list_elem;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(list_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  temp = DeeObject_CompareEq(list_elem,elem);
  Dee_DECREF(list_elem);
  if DEE_UNLIKELY(temp < 0) {
   if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return -1;
   temp = 0;
  }
  DeeList_ACQUIRE(self);
  // v make extra sure that this is still the same object!
  if (temp && list_elem == DeeList_GET(self,i)) { // Found it!
   memmove(DeeList_ELEM(self)+i,DeeList_ELEM(self)+(i+1),
          (DeeList_SIZE(self)-i)*sizeof(DeeObject *));
   --((DeeListObject *)self)->l_len;
   DeeList_RELEASE(self);
   Dee_DECREF(list_elem); // Remove the second reference
   return 0;
  }
 }
 DeeList_RELEASE(self);
 return 1;
}

DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeList_RemovePred(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *elem,
 DEE_A_INOUT DeeObject *pred) {
 Dee_size_t i; int temp; DeeObject *list_elem,*pred_args,*pred_result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(list_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  pred_args = DeeTuple_Pack(2,list_elem,elem);
  Dee_DECREF(list_elem);
  if DEE_UNLIKELY(!pred_args) return -1;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) return -1;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if DEE_UNLIKELY(temp < 0) {
   if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return -1;
   temp = 0;
  }
  DeeList_ACQUIRE(self);
  // v make extra sure that this is still the same object!
  if (temp && list_elem == DeeList_GET(self,i)) { // Found it!
   memmove(DeeList_ELEM(self)+i,DeeList_ELEM(self)+(i+1),
          (DeeList_SIZE(self)-i)*sizeof(DeeObject *));
   --((DeeListObject *)self)->l_len;
   DeeList_RELEASE(self);
   Dee_DECREF(list_elem); // Remove the second reference
   return 0;
  }
 }
 DeeList_RELEASE(self);
 return 1;
}


DEE_A_RET_EXCEPT(-1) Dee_size_t DeeList_RemoveIf(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *pred) {
 Dee_size_t n_removed,i; int temp; DeeObject *list_elem,*args,*call_res;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(pred));
 n_removed = 0; DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(list_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  if DEE_UNLIKELY((args = DeeTuple_Pack(1,list_elem)) == NULL) {
err_ob: Dee_DECREF(list_elem); return (Dee_size_t)-1;
  }
  call_res = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if DEE_UNLIKELY(!call_res) goto err_ob;
  temp = DeeObject_Bool(call_res);
  Dee_DECREF(call_res);
  if DEE_UNLIKELY(temp < 0) goto err_ob;
  DeeList_ACQUIRE(self);
  Dee_DECREF(list_elem);
  // v make extra sure that this is still the same object!
  if (temp && list_elem == DeeList_GET(self,i)) { // Found it!
   Dee_DECREF(list_elem); // Decref the reference contained in the list
   memmove(DeeList_ELEM(self)+i,DeeList_ELEM(self)+(i+1),
          (DeeList_SIZE(self)-i)*sizeof(DeeObject *));
   --((DeeListObject *)self)->l_len;
   ++n_removed;
  }
 }
 DeeList_RELEASE(self);
 return n_removed;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeList_Segments(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_IN Dee_size_t n) {
 DeeObject **dst_iter,**dst_end,**src_iter,*result;
 Dee_size_t i,my_segment_size,src_size,segment_size;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 if (!n) return DeeList_NewEmpty();
 if DEE_UNLIKELY((result = _DeeList_NewUnsafe(n)) == NULL) return NULL;
again: i = 0;
 DeeList_ACQUIRE(self);
 src_size = DeeList_SIZE(self),segment_size = src_size/n;
 if (segment_size*n != src_size) ++segment_size;
 dst_end = (dst_iter = DeeList_ELEM(result))+n;
 src_iter = DeeList_ELEM(self);
 while DEE_LIKELY(dst_iter != dst_end) {
  DeeListObject *dst_list;
  my_segment_size = segment_size <= src_size ? segment_size : src_size;
  if DEE_UNLIKELY((dst_list = (DeeListObject *)DeeGC_TryWeakMalloc(sizeof(DeeListObject))) == NULL) {
   DeeList_RELEASE(self);
   while DEE_LIKELY(dst_iter != DeeList_ELEM(result)) Dee_DECREF(*--dst_iter);
   _DeeList_FreeUnsafe(result);
   if DEE_LIKELY(Dee_CollectMemory()) goto again;
   DeeError_NoMemory();
   return NULL;
  } else {
   DeeObject_INIT(dst_list,&DeeList_Type);
   if DEE_UNLIKELY(DeeList_TryWeakInitFromArray(dst_list,segment_size,src_iter) != 0) {
    DeeList_RELEASE(self);
    _DeeGCObject_Delete((DeeObject *)dst_list);
    while DEE_LIKELY(dst_iter != DeeList_ELEM(result)) Dee_DECREF(*--dst_iter);
    _DeeList_FreeUnsafe(result);
    if DEE_LIKELY(Dee_CollectMemory()) goto again;
    DeeError_NoMemory();
    return NULL;
   } else DeeGC_TrackedAdd((DeeObject *)dst_list);
  }
  *dst_iter = (DeeObject *)dst_iter;
  ++dst_iter;
  src_iter += my_segment_size;
  src_size -= my_segment_size;
 }
 DeeList_RELEASE(self);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeList_Erase(
 DEE_A_INOUT_OBJECT(DeeListObject) *self,
 DEE_A_IN Dee_size_t i, DEE_A_IN Dee_size_t n) {
 DeeObject **elem,**old_elem,**iter,**end;
 Dee_size_t new_size,used_n;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 if (!n) return 0;
again: used_n = n;
 DeeList_ACQUIRE(self);
 if (i >= DeeList_SIZE(self)) { DeeList_RELEASE(self); return 0; }
 if (i+used_n > DeeList_SIZE(self)) used_n = DeeList_SIZE(self)-i;
 // Temporary buffer for old elements
 if DEE_UNLIKELY((old_elem = (DeeObject **)malloc_nz(
  used_n*sizeof(DeeObject *))) == NULL) {
  DeeList_RELEASE(self);
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return -1;
 }
 // Preserve old elements (will be deallocated later)
 elem = DeeList_ELEM(self);
 memcpy(old_elem,elem+i,used_n*sizeof(DeeObject *));
 if ((new_size = DeeList_SIZE(self)-used_n) != 0) {
  // Move the list portions above the high point to the end of the insert portion
  memmove(elem+i,elem+(i+used_n),(DeeList_SIZE(self)-(i+used_n))*sizeof(DeeObject *));
 } // NOTE: If the list is now empty, that's ok (keep the preallocate cache)
 // Always update the size, though
 ((DeeListObject *)self)->l_len = new_size;
 DeeList_RELEASE(self);
 // Cleanup the old elements
 end = (iter = old_elem)+used_n;
 while (iter != end) Dee_DECREF(*iter++);
 free_nn(old_elem);
 return 0;
}
DEE_A_RET_NOEXCEPT_REF DeeObject *DeeList_Pop(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t i) {
 DeeObject *result; Dee_size_t list_size;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeList_ACQUIRE(self);
 if DEE_UNLIKELY((list_size = DeeList_SIZE(self)) == 0) {
  DeeList_RELEASE(self);
  DeeError_ValueError_EmptyList(&DeeList_Type);
  return NULL;
 }
 if DEE_LIKELY(i >= (list_size-1)) {
  result = DeeList_GET(self,list_size-1);
 } else {
  result = DeeList_GET(self,i);
  memmove(DeeList_ELEM(self)+i,DeeList_ELEM(self)+(i+1),
         (list_size-(i+1))*sizeof(DeeObject *));
 }
 --((DeeListObject *)self)->l_len;
 DeeList_RELEASE(self);
 return result; // Inherit reference
}
void DeeList_Clear(DEE_A_INOUT_OBJECT(DeeListObject) *self) {
 DeeObject **elem,**iter,**end; Dee_size_t old_alloc;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeList_ACQUIRE(self);
 // Steal the object list
 if ((elem = DeeList_ELEM(self)) == NULL) goto end;
 old_alloc = DeeList_ALLOC(self);
 end = elem+DeeList_SIZE(self);
 ((DeeListObject *)self)->l_elem = NULL;
 ((DeeListObject *)self)->l_len = 0;
 ((DeeListObject *)self)->l_alloc = 0;
 DeeList_RELEASE(self);
 // Decref everything within
 iter = elem; while (iter != end) Dee_DECREF(*iter++);
 DeeList_ACQUIRE(self);
 // Try to hand back the empty list
 if (DeeList_ELEM(self) == NULL) {
  DEE_ASSERT(DeeList_ALLOC(self) == 0);
  DEE_ASSERT(DeeList_SIZE(self) == 0);
  // Can re-use the element vector for pre-allocated list memory
  ((DeeListObject *)self)->l_elem = elem;
  ((DeeListObject *)self)->l_alloc = old_alloc;
end: DeeList_RELEASE(self);
 } else {
  // Some other thread already continued using
  // the list (drop the old element list)
  DeeList_RELEASE(self);
  free(elem);
 }
}
void DeeList_ClearAndShrink(DEE_A_INOUT_OBJECT(DeeListObject) *self) {
 DeeObject **elem,**iter,**end;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeList_ACQUIRE(self);
 end = (elem = DeeList_ELEM(self))+DeeList_SIZE(self);
 ((DeeListObject *)self)->l_elem = NULL;
 ((DeeListObject *)self)->l_len = 0;
 ((DeeListObject *)self)->l_alloc = 0;
 DeeList_RELEASE(self);
 // Decref everything within
 iter = elem; while DEE_LIKELY(iter != end) Dee_DECREF(*iter++);
 free(elem);
}

DEE_A_RET_EXCEPT(-1) int DeeList_Repeat(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_IN Dee_size_t n) {
 DeeObject **new_elem,**dst,**iter,**src,**end; Dee_size_t old_size,new_size;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 if (!n) { DeeList_Clear(self); return 0; }
 if (n == 1) return 0;
again:
 DeeList_ACQUIRE(self);
 if ((old_size = DeeList_SIZE(self)) != 0) {
  new_elem = DeeList_ELEM(self);
  new_size = old_size*n;
  if (new_size > DeeList_ALLOC(self)) {
   if DEE_UNLIKELY((new_elem = (DeeObject **)realloc_nnz(
    new_elem,new_size*sizeof(DeeObject *))) == NULL) {
    DeeList_RELEASE(self);
    // Collect garbage and try again
    if DEE_LIKELY(Dee_CollectMemory()) goto again;
    DeeError_NoMemory();
    return -1;
   }
   ((DeeListObject *)self)->l_alloc = new_size;
   ((DeeListObject *)self)->l_elem = new_elem;
  }
  ((DeeListObject *)self)->l_len = new_size;
  // Fill the new portion
  dst = end = (src = new_elem)+old_size;
  --n;
  do {
   iter = src;
   do Dee_INCREF(*dst++ = *iter++);
   while DEE_LIKELY(iter != end);
  } while DEE_LIKELY(--n);
 }
 DeeList_RELEASE(self);
 return 0;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeList_Repeated(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_IN Dee_size_t n) {
 DeeObject *result;
 if DEE_LIKELY((result = DeeList_Copy(self)) != NULL) {
  if DEE_UNLIKELY(DeeList_Repeat(result,n) != 0) Dee_CLEAR(result);
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeList_Reserve(
 DEE_A_INOUT_OBJECT(DeeListObject) *self,
 DEE_A_IN Dee_size_t reserved_size) {
 DeeObject **new_elem;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
again:
 DeeList_ACQUIRE(self);
 if (reserved_size > DeeList_ALLOC(self)) { // Acquire more memory
  if DEE_UNLIKELY((new_elem = (DeeObject **)realloc_nz(DeeList_ELEM(
   self),sizeof(DeeObject *)*reserved_size)) == NULL) {
   DeeList_RELEASE(self);
   if DEE_LIKELY(Dee_CollectMemory()) goto again;
   DeeError_NoMemory();
   return -1;
  }
  ((DeeListObject *)self)->l_elem = new_elem;
  ((DeeListObject *)self)->l_alloc = reserved_size;
 }
 DeeList_RELEASE(self);
 return 0;
}
void DeeList_ShrinkToFit(DEE_A_INOUT_OBJECT(DeeListObject) *self) {
 Dee_size_t size; DeeObject **new_elem;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
again:
 DeeList_ACQUIRE(self);
 if (DeeList_ALLOC(self) != (size = DeeList_SIZE(self))) {
  new_elem = DeeList_ELEM(self);
  DEE_ASSERT(new_elem != NULL);
  if (size) {
   if DEE_UNLIKELY((new_elem = (DeeObject **)realloc_nnz(
    new_elem,size*sizeof(DeeObject *))) == NULL) {
    DeeList_RELEASE(self);
    if DEE_LIKELY(Dee_CollectMemory()) goto again;
    return;
   }
   ((DeeListObject *)self)->l_elem = new_elem;
  } else {
   free_nn(new_elem);
   ((DeeListObject *)self)->l_elem = NULL;
  }
  ((DeeListObject *)self)->l_alloc = size;
 }
 DeeList_RELEASE(self);
}
DEE_A_RET_EXCEPT(-1) int DeeList_Resize(
 DEE_A_INOUT_OBJECT(DeeListObject) *self,
 DEE_A_IN Dee_size_t new_size, DEE_A_INOUT DeeObject *filler) {
 DeeObject **iter,**end; Dee_size_t old_size;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(filler));
 if (!new_size) { DeeList_Clear(self); return 0; }
again:
 DeeList_ACQUIRE(self);
 if (new_size > (old_size = DeeList_SIZE(self))) {
  // Increase size
  iter = DeeList_ELEM(self);
  if (new_size > DeeList_ALLOC(self)) {
   // Increase pre-allocated space
   if ((iter = (DeeObject **)realloc_nz(
    iter,new_size*sizeof(DeeObject *))) == NULL) {
no_mem:
    DeeList_RELEASE(self);
    if DEE_LIKELY(Dee_CollectMemory()) goto again;
    DeeError_NoMemory();
    return -1;
   }
   ((DeeListObject *)self)->l_elem = iter;
   ((DeeListObject *)self)->l_alloc = new_size;
  }
  // Set the new size
  ((DeeListObject *)self)->l_len = new_size;
  // Fill the new memory
  end = iter+new_size;
  iter += old_size;
  while DEE_LIKELY(iter != end) Dee_INCREF(*iter++ = filler);
  DeeList_RELEASE(self);
 } else if (new_size != old_size) {
  DeeObject **pop_elem; Dee_size_t pop_size;
  DEE_ASSERT(new_size < old_size);
  pop_size = (Dee_size_t)(old_size-new_size);
  if DEE_UNLIKELY((pop_elem = (DeeObject **)malloc_nz(
   pop_size*sizeof(DeeObject *))) == NULL) goto no_mem;
  // Set the new size
  ((DeeListObject *)self)->l_len = new_size;
  // Copy all elements that should be removed into the temporary buffer
  memcpy(pop_elem,DeeList_ELEM(self)+new_size,pop_size*sizeof(DeeObject *));
  DeeList_RELEASE(self);
  // Clear the temporary buffer after we unlocked the list
  end = (iter = pop_elem)+pop_size;
  while DEE_LIKELY(iter != end) Dee_DECREF(*iter++);
  free_nn(pop_elem);
 }
 return 0;
}
void DeeList_Reverse(DEE_A_INOUT_OBJECT(DeeListObject) *self) {
 DeeObject **iter,**end;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeList_ACQUIRE(self);
 end = (iter = DeeList_ELEM(self))+DeeList_SIZE(self);
 while DEE_LIKELY(iter < end) {
  DeeObject *temp = *iter;
  *iter++ = *--end;
  *end = temp;
 }
 DeeList_RELEASE(self);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *
DeeList_Reversed(DEE_A_IN_OBJECT(DeeListObject) const *self) {
 DeeObject *result;
 if DEE_UNLIKELY((result = DeeList_Copy(self)) != NULL) DeeList_Reverse(result);
 return result;
}
void DeeList_Fill(
 DEE_A_INOUT_OBJECT(DeeListObject) *self,
 DEE_A_INOUT DeeObject *filler) {
 Dee_size_t i; DeeObject *old_elem;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(filler));
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  old_elem = DeeList_GET(self,i); // inherit reference
  DeeList_SET(self,i,filler);
  Dee_INCREF(filler);
  DeeList_RELEASE(self);
  Dee_DECREF(old_elem); // This could do crazy stuff
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
}
DEE_A_RET_EXCEPT(-1) int DeeList_FillAssign(
 DEE_A_INOUT_OBJECT(DeeListObject) *self,
 DEE_A_INOUT DeeObject *filler, DEE_A_IN Dee_size_t size) {
 DeeObject **new_elem,**old_elem,**iter,**end; Dee_size_t old_size;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(filler));
 if DEE_UNLIKELY(!size) { DeeList_Clear(self); return 0; }
again:
 DeeList_ACQUIRE(self);
 old_size = DeeList_SIZE(self);
 if (old_size != 0) {
  if DEE_UNLIKELY((old_elem = (DeeObject **)malloc_nz(
   old_size*sizeof(DeeObject *))) == NULL) {
   DeeList_RELEASE(self);
no_mem:
   if DEE_LIKELY(Dee_CollectMemory()) goto again;
   DeeError_NoMemory();
   return -1;
  }
 } else old_elem = NULL;
 new_elem = DeeList_ELEM(self);
 memcpy(old_elem,new_elem,old_size*sizeof(DeeObject *));
 if DEE_UNLIKELY(size > DeeList_ALLOC(self)) {
  // Must reallocate the list
  if DEE_UNLIKELY((new_elem = (DeeObject **)realloc_nz(
   new_elem,size*sizeof(DeeObject *))) == NULL) {
   DeeList_RELEASE(self);
   free(old_elem);
   goto no_mem;
  }
  ((DeeListObject *)self)->l_elem = new_elem;
  ((DeeListObject *)self)->l_alloc = size;
 }
 ((DeeListObject *)self)->l_len = size;
 // Override the list
 end = (iter = DeeList_ELEM(self))+size;
 while DEE_LIKELY(iter != end) Dee_INCREF(*iter++ = filler);
 DeeList_RELEASE(self);
 // Free the old list
 end = (iter = old_elem)+old_size;
 while DEE_LIKELY(iter != end) Dee_DECREF(*iter++);
 free(old_elem);
 return 0;
}




DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeList_DoSort(
 DEE_A_INOUT_OBJECT(DeeListObject) *self,
 DEE_A_IN int (*pred)(DeeObject*,DeeObject*,void*), void *closure) {
 DeeObject **new_list,**old_list,**iter,**end,**src;
 Dee_size_t size; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
again:
 // TODO: Optimization: No need to copy the list if 'DeeObject_IS_UNIQUE(self)' is true
 DeeList_ACQUIRE(self);
 if DEE_UNLIKELY((size = DeeList_SIZE(self)) == 0) {
  DeeList_RELEASE(self);
  return 0;
 }
 if DEE_UNLIKELY((old_list = (DeeObject **)malloc_nz(
  size*sizeof(DeeObject *))) == NULL) {
  DeeList_RELEASE(self);
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return -1;
 }
 if DEE_UNLIKELY((new_list = (DeeObject **)malloc_nz(
  size*sizeof(DeeObject *))) == NULL) {
  DeeList_RELEASE(self);
  free_nn(old_list);
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return -1;
 }
 src = DeeList_ELEM(self),end = (iter = old_list)+size;
 // NOTE: The references stored here are transfered by '_DeeGeneric_MergeSort'
 do Dee_INCREF(*iter++ = *src++); while (iter != end);
 DeeList_RELEASE(self);
 temp = _DeeGeneric_MergeSort(size,old_list,new_list,pred,closure);
 free_nn(old_list); // Free 'old_list' (all references got moved to 'new_list')
 if DEE_UNLIKELY(temp != 0) {
  // Error occurred while sorting
  end = (iter = new_list)+size;
  while (iter != end) Dee_DECREF(*iter++);
  free_nn(new_list);
  return -1;
 }
 DeeList_ACQUIRE(self);
 // v make sure that no other thread has modified the list in the meantime
 if DEE_UNLIKELY((src == DeeList_ELEM(self)+size) && size == DeeList_SIZE(self)) {
  old_list = DeeList_ELEM(self);
  ((DeeListObject *)self)->l_elem = new_list;
  ((DeeListObject *)self)->l_len =
  ((DeeListObject *)self)->l_alloc = size;
  DeeList_RELEASE(self);
  new_list = old_list; // Free the correct list
 } else DeeList_RELEASE(self);
 // Free either the old list or the unused sorted list
 end = (iter = new_list)+size;
 while DEE_LIKELY(iter != end) Dee_DECREF(*iter++);
 free_nn(new_list);
 return temp;
}
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) _DeeList_DoSorted(
 DEE_A_INOUT_OBJECT(DeeListObject) const *self,
 DEE_A_IN int (*pred_lo)(DeeObject*,DeeObject*,void*), void *closure) {
 DeeObject **new_list,**old_list,**iter,**end,**src;
 DeeListObject *result;
 Dee_size_t size; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
again:
 // TODO: Optimization: No need to copy the list if 'DeeObject_IS_UNIQUE(self)' is true
 DeeList_ACQUIRE(self);
 if DEE_UNLIKELY((size = DeeList_SIZE(self)) == 0) {
  DeeList_RELEASE(self);
  return DeeList_New();
 }
 if DEE_UNLIKELY((old_list = (DeeObject **)malloc_nz(size*sizeof(DeeObject *))) == NULL) {
  DeeList_RELEASE(self);
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return NULL;
 }
 if DEE_UNLIKELY((new_list = (DeeObject **)malloc_nz(size*sizeof(DeeObject *))) == NULL) {
  DeeList_RELEASE(self);
  free_nn(old_list);
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return NULL;
 }
 src = DeeList_ELEM(self),end = (iter = old_list)+size;
 // NOTE: The references stored here are transfered by '_DeeGeneric_MergeSort'
 do Dee_INCREF(*iter++ = *src++); while (iter != end);
 DeeList_RELEASE(self);
 temp = _DeeGeneric_MergeSort(size,old_list,new_list,pred_lo,closure);
 free_nn(old_list); // Free 'old_list' (all references got moved to 'new_list')
 if DEE_UNLIKELY(temp != 0) {
late_err:
  // Error occurred while sorting
  end = (iter = new_list)+size;
  while (iter != end) Dee_DECREF(*iter++);
  free_nn(new_list);
  return NULL;
 }
 if DEE_UNLIKELY((result = DeeGC_MALLOC(DeeListObject)) == NULL) goto late_err;
 DeeObject_INIT(result,&DeeList_Type);
 _DeeList_InitCommon(result);
 result->l_len = result->l_alloc = size;
 result->l_elem = new_list; // Inherit vector
 DeeGC_TrackedAdd((DeeObject *)result);
 return (DeeObject *)result;
}


DEE_A_RET_EXCEPT(-1) int DeeList_Sort(
 DEE_A_INOUT_OBJECT(DeeListObject) *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 return _DeeList_DoSort(self,&_DeeGeneric_SortPredCmpLo,NULL);
}
DEE_A_RET_EXCEPT(-1) int DeeList_SortPred(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *pred_lo) {
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 return _DeeList_DoSort(self,&_DeeGeneric_SortPredUserFunc,pred_lo);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeList_Sorted(
 DEE_A_IN_OBJECT(DeeListObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 return _DeeList_DoSorted(self,&_DeeGeneric_SortPredCmpLo,NULL);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeList_SortedPred(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *pred_lo) {
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 return _DeeList_DoSorted(self,&_DeeGeneric_SortPredUserFunc,pred_lo);
}


DEE_A_RET_EXCEPT(-1) int DeeList_Unique(
 DEE_A_INOUT_OBJECT(DeeListObject) *self) {
 Dee_size_t i; DeeObject *first,*second; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 i = 0;
 DeeList_ACQUIRE(self);
 while (1) {
  if DEE_UNLIKELY(i+1 >= DeeList_SIZE(self)) break;
  Dee_INCREF(first = DeeList_GET(self,i));
  Dee_INCREF(second = DeeList_GET(self,i+1));
  DeeList_RELEASE(self);
  // Check if two consecutive elements are equal to each other
  temp = DeeObject_CompareEq(first,second);
  Dee_DECREF(second);
  Dee_DECREF(first);
  if DEE_UNLIKELY(temp < 0) return temp;
  DeeList_ACQUIRE(self);
  if DEE_UNLIKELY(i >= DeeList_SIZE(self)) break;
  if (temp && DeeList_GET(self,i) == first) {
   // Remove 'first' from 'DeeList_GET(self,i)'
   memmove(DeeList_ELEM(self)+i,DeeList_ELEM(self)+(i+1),
          (DeeList_SIZE(self)-(i-1))*sizeof(DeeObject *));
   --((DeeListObject *)self)->l_len;
   DeeList_RELEASE(self);
   Dee_DECREF(first);
   DeeList_ACQUIRE(self);
  } else ++i;
 }
 DeeList_RELEASE(self);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeList_UniquePred(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *pred_eq) {
 Dee_size_t i; DeeObject *first,*second,*args,*pred_result; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(pred_eq));
 i = 0;
 DeeList_ACQUIRE(self);
 while (1) {
  if (i+1 >= DeeList_SIZE(self)) break;
  Dee_INCREF(first = DeeList_GET(self,i));
  Dee_INCREF(second = DeeList_GET(self,i+1));
  DeeList_RELEASE(self);
  // Check if two consecutive elements are equal to each other
  args = DeeTuple_Pack(2,first,second);
  Dee_DECREF(second);
  Dee_DECREF(first);
  if (!args) return -1;
  pred_result = DeeObject_Call(pred_eq,args);
  Dee_DECREF(args);
  if (!pred_result) return -1;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if (temp < 0) return temp;
  DeeList_ACQUIRE(self);
  if (i >= DeeList_SIZE(self)) break;
  if (temp && DeeList_GET(self,i) == first) {
   // Remove 'first' from 'DeeList_GET(self,i)'
   memmove(DeeList_ELEM(self)+i,DeeList_ELEM(self)+(i+1),
          (DeeList_SIZE(self)-(i-1))*sizeof(DeeObject *));
   --((DeeListObject *)self)->l_len;
   DeeList_RELEASE(self);
   Dee_DECREF(first);
   DeeList_ACQUIRE(self);
  } else ++i;
 }
 DeeList_RELEASE(self);
 return 0;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeList_ToUnique(
 DEE_A_IN_OBJECT(DeeListObject) const *self) {
 DeeObject *result; Dee_size_t i,size; DeeObject *first,*second; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
again:
 DeeList_ACQUIRE(self);
 size = DeeList_SIZE(self);
 DeeList_RELEASE(self);
 switch (size) {
  case 0: return DeeList_NewEmpty();
  case 1:
   DeeList_ACQUIRE(self);
   if DEE_UNLIKELY(DeeList_SIZE(self) != 1) goto again;
   Dee_INCREF(first = DeeList_GET(self,0));
   DeeList_RELEASE(self);
   result = DeeList_NewFromVector(1,&first);
   Dee_DECREF(first);
   return result;
  default: break;
 }
 if DEE_UNLIKELY((result = DeeList_NewWithSizeHint(size)) == NULL) return NULL;
 DeeList_ACQUIRE(self);
 if DEE_UNLIKELY(DeeList_SIZE(self) < 2) {
  // The list is now too small (try again)
  DeeList_RELEASE(self);
  Dee_DECREF(result);
  goto again;
 }
 i = 0; while (1) {
  DEE_ASSERT(i+1 < DeeList_SIZE(self));
  Dee_INCREF(first = DeeList_GET(self,i));
  Dee_INCREF(second = DeeList_GET(self,i+1));
  DeeList_RELEASE(self);
  temp = DeeObject_CompareEq(first,second);
  Dee_DECREF(second);
  if DEE_UNLIKELY(temp < 0) {
err_first: Dee_DECREF(first);
/*err_r:*/ Dee_DECREF(result);
   return NULL;
  }
  // Append the first element to the result if it's not equal to the second
  if DEE_UNLIKELY(!temp && DeeList_Append(result,first) != 0) goto err_first;
  Dee_DECREF(first);
  ++i;
  DeeList_ACQUIRE(self);
  if DEE_UNLIKELY(i+1 >= DeeList_SIZE(self)) {
   if DEE_UNLIKELY(i == DeeList_SIZE(self)-1) {
    // Append the last element
    Dee_INCREF(first = DeeList_GET(self,i));
    DeeList_RELEASE(self);
    if (DeeList_Append(result,first) != 0) goto err_first;
    Dee_DECREF(first);
    return result;
   }
   break;
  }
 }
 DeeList_RELEASE(self);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeList_ToUniquePred(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *pred_eq) {
 DeeObject *result; Dee_size_t i,size;
 DeeObject *first,*second,*args,*pred_result; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
again:
 DeeList_ACQUIRE(self);
 size = DeeList_SIZE(self);
 DeeList_RELEASE(self);
 switch (size) {
  case 0: return DeeList_NewEmpty();
  case 1:
   DeeList_ACQUIRE(self);
   if DEE_UNLIKELY(DeeList_SIZE(self) != 1) goto again;
   Dee_INCREF(first = DeeList_GET(self,0));
   DeeList_RELEASE(self);
   result = DeeList_NewFromVector(1,&first);
   Dee_DECREF(first);
   return result;
  default: break;
 }
 if DEE_UNLIKELY((result = DeeList_NewWithSizeHint(size)) == NULL) return NULL;
 DeeList_ACQUIRE(self);
 if DEE_UNLIKELY(DeeList_SIZE(self) < 2) {
  // The list is now too small (try again)
  DeeList_RELEASE(self);
  Dee_DECREF(result);
  goto again;
 }
 i = 0; while (1) {
  DEE_ASSERT(i+1 < DeeList_SIZE(self));
  Dee_INCREF(first = DeeList_GET(self,i));
  Dee_INCREF(second = DeeList_GET(self,i+1));
  DeeList_RELEASE(self);
  args = DeeTuple_Pack(2,first,second);
  Dee_DECREF(second);
  if DEE_UNLIKELY(!args) goto err_first;
  pred_result = DeeObject_Call(pred_eq,args);
  Dee_DECREF(args);
  if DEE_UNLIKELY(!pred_result) goto err_first;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if DEE_UNLIKELY(temp < 0) {
err_first: Dee_DECREF(first);
/*err_r:*/ Dee_DECREF(result);
   return NULL;
  }
  // Append the first element to the result if it's not equal to the second
  if DEE_UNLIKELY(!temp && DeeList_Append(result,first) != 0) goto err_first;
  Dee_DECREF(first);
  ++i;
  DeeList_ACQUIRE(self);
  if DEE_UNLIKELY(i+1 >= DeeList_SIZE(self)) {
   if DEE_UNLIKELY(i == DeeList_SIZE(self)-1) {
    // Append the last element
    Dee_INCREF(first = DeeList_GET(self,i));
    DeeList_RELEASE(self);
    if (DeeList_Append(result,first) != 0) goto err_first;
    Dee_DECREF(first);
    return result;
   }
   break;
  }
 }
 DeeList_RELEASE(self);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeList_AppendUniqueIterator(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *iterator) {
 DeeObject *previous,*elem; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(iterator));
 if DEE_UNLIKELY((temp = DeeObject_IterNextEx(iterator,&previous)) != 0) return temp < 0 ? temp : 0;
 while DEE_LIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  temp = DeeObject_CompareEq(previous,elem);
  if DEE_UNLIKELY(temp < 0) {err_elem: Dee_DECREF(elem); break; }
  if DEE_UNLIKELY(!temp && DeeList_Append(self,previous) != 0) { temp = -1; goto err_elem; }
  Dee_DECREF(previous);
  Dee_INHERIT_REF(previous,elem);
 }
 if DEE_UNLIKELY(temp >= 0) temp = DeeList_Append(self,previous);
 Dee_DECREF(previous);
 return DEE_UNLIKELY(temp < 0) ? temp : 0;
}
DEE_A_RET_EXCEPT(-1) int DeeList_AppendUniqueIteratorPred(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *iterator,
 DEE_A_INOUT DeeObject *pred_eq) {
 DeeObject *previous,*elem,*args,*pred_result; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(iterator));
 if DEE_UNLIKELY((temp = DeeObject_IterNextEx(iterator,&previous)) != 0)
  return DEE_UNLIKELY(temp < 0) ? temp : 0;
 while DEE_LIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  if DEE_UNLIKELY((args = DeeTuple_Pack(2,previous,elem)) == NULL) goto err_elem2;
  pred_result = DeeObject_Call(pred_eq,args);
  Dee_DECREF(args);
  if DEE_UNLIKELY(!pred_result) goto err_elem2;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if DEE_UNLIKELY(temp < 0) {err_elem: Dee_DECREF(elem); break; }
  if DEE_UNLIKELY(!temp && DeeList_Append(self,previous) != 0) {err_elem2: temp = -1; goto err_elem; }
  Dee_DECREF(previous);
  Dee_INHERIT_REF(previous,elem);
 }
 if DEE_LIKELY(temp >= 0) temp = DeeList_Append(self,previous);
 Dee_DECREF(previous);
 return DEE_UNLIKELY(temp < 0) ? temp : 0;
}



DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeList_Find(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *elem) {
 Dee_size_t i; int temp; DeeObject *my_elem;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(my_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  temp = DeeObject_CompareEq(my_elem,elem);
  Dee_DECREF(my_elem);
  if DEE_UNLIKELY(temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return (Dee_ssize_t)-2;
   } else {
    DEE_ASSERT(temp > 0);
    return (Dee_ssize_t)i;
   }
  }
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 return (Dee_ssize_t)-1;
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeList_FindPred(
 DEE_A_IN_OBJECT(DeeListObject) const *self,
 DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) {
 Dee_size_t i; int temp; DeeObject *my_elem,*pred_args,*pred_result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 DEE_ASSERT(DeeObject_Check(pred));
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(my_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  pred_args = DeeTuple_Pack(2,my_elem,elem);
  Dee_DECREF(my_elem);
  if DEE_UNLIKELY(!pred_args) return (Dee_ssize_t)-2;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) goto check_ni;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if DEE_UNLIKELY(temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
check_ni:
    if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return (Dee_ssize_t)-2;
   } else {
    DEE_ASSERT(temp > 0);
    return (Dee_ssize_t)i;
   }
  }
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 return (Dee_ssize_t)-1;
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeList_RFind(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *elem) {
 Dee_size_t i; int temp; DeeObject *my_elem;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 i = (Dee_size_t)-1;
 DeeList_ACQUIRE(self);
 while (1) {
  if DEE_UNLIKELY(i >= DeeList_SIZE(self)) i = DeeList_SIZE(self);
  if DEE_UNLIKELY(!i--) break;
  Dee_INCREF(my_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  temp = DeeObject_CompareEq(my_elem,elem);
  Dee_DECREF(my_elem);
  if DEE_UNLIKELY(temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return (Dee_ssize_t)-2;
   } else {
    DEE_ASSERT(temp > 0);
    return (Dee_ssize_t)i;
   }
  }
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 return (Dee_ssize_t)-1;
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeList_RFindPred(
 DEE_A_IN_OBJECT(DeeListObject) const *self,
 DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) {
 Dee_size_t i; int temp; DeeObject *my_elem,*pred_args,*pred_result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 i = (Dee_size_t)-1;
 DeeList_ACQUIRE(self);
 while (1) {
  if DEE_UNLIKELY(i >= DeeList_SIZE(self)) i = DeeList_SIZE(self);
  if DEE_UNLIKELY(!i--) break;
  Dee_INCREF(my_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  pred_args = DeeTuple_Pack(2,my_elem,elem);
  Dee_DECREF(my_elem);
  if DEE_UNLIKELY(!pred_args) return (Dee_ssize_t)-2;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) goto check_ni;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if DEE_UNLIKELY(temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
check_ni:
    if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return (Dee_ssize_t)-2;
   } else {
    DEE_ASSERT(temp > 0);
    return (Dee_ssize_t)i;
   }
  }
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 return (Dee_ssize_t)-1;
}


DEE_A_RET_EXCEPT(-1) Dee_size_t DeeList_Index(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *elem) {
 Dee_ssize_t result = DeeList_Find(self,elem);
 if DEE_UNLIKELY(result == -2) return (Dee_size_t)-1;
 if DEE_UNLIKELY(result == -1) {
  DeeError_ValueError_NotFound((DeeObject *)self,elem);
  return (Dee_size_t)-1;
 }
 return (Dee_size_t)result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeList_IndexPred(
 DEE_A_IN_OBJECT(DeeListObject) const *self,
 DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) {
 Dee_ssize_t result = DeeList_FindPred(self,elem,pred);
 if DEE_UNLIKELY(result == -2) return (Dee_size_t)-1;
 if DEE_UNLIKELY(result == -1) {
  DeeError_ValueError_NotFound((DeeObject *)self,elem);
  return (Dee_size_t)-1;
 }
 return (Dee_size_t)result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeList_RIndex(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *elem) {
 Dee_ssize_t result = DeeList_RFind(self,elem);
 if DEE_UNLIKELY(result == -2) return (Dee_size_t)-1;
 if DEE_UNLIKELY(result == -1) {
  DeeError_ValueError_NotFound((DeeObject *)self,elem);
  return (Dee_size_t)-1;
 }
 return (Dee_size_t)result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeList_RIndexPred(
 DEE_A_IN_OBJECT(DeeListObject) const *self,
 DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) {
 Dee_ssize_t result = DeeList_RFindPred(self,elem,pred);
 if DEE_UNLIKELY(result == -2) return (Dee_size_t)-1;
 if DEE_UNLIKELY(result == -1) {
  DeeError_ValueError_NotFound((DeeObject *)self,elem);
  return (Dee_size_t)-1;
 }
 return (Dee_size_t)result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeList_Count(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *elem) {
 int temp; Dee_size_t i,result = 0; DeeObject *my_elem;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(my_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  temp = DeeObject_CompareEq(my_elem,elem);
  Dee_DECREF(my_elem);
  if DEE_UNLIKELY(temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
    if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return (Dee_size_t)-1;
   } else {
    DEE_ASSERT(temp > 0);
    ++result;
   }
  }
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 DEE_ASSERT(result != (Dee_size_t)-1 && "How can this be possible?");
 return result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeList_CountPred(
 DEE_A_IN_OBJECT(DeeListObject) const *self,
 DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) {
 int temp; Dee_size_t i,result = 0; DeeObject *my_elem,*pred_args,*pred_result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 DEE_ASSERT(DeeObject_Check(pred));
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(my_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  pred_args = DeeTuple_Pack(2,my_elem,elem);
  Dee_DECREF(my_elem);
  if DEE_UNLIKELY(!pred_args) return (Dee_size_t)-1;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) goto check_ni;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if DEE_UNLIKELY(temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
check_ni:
    if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return (Dee_size_t)-1;
   } else {
    DEE_ASSERT(temp > 0);
    ++result;
   }
  }
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 DEE_ASSERT(result != (Dee_size_t)-1 && "How can this be possible?");
 return result;
}

DEE_A_RET_EXCEPT_REF DeeObject *DeeList_Locate(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *pred) {
 Dee_size_t i; int temp; DeeObject *pred_result_ob,*args,*my_elem;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(pred));
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(my_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  if DEE_UNLIKELY((args = DeeTuple_Pack(1,my_elem)) == NULL) {
err_my_elem: Dee_DECREF(my_elem); return NULL;
  }
  pred_result_ob = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if DEE_UNLIKELY(!pred_result_ob) goto err_my_elem;
  temp = DeeObject_Bool(pred_result_ob);
  Dee_DECREF(pred_result_ob);
  if (temp != 0) {
   if (temp < 0) goto err_my_elem;
   DEE_ASSERT(temp > 0);
   return my_elem; // Inherit reference
  }
  Dee_DECREF(my_elem);
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 DeeError_ValueError_NotFound_Pred((DeeObject *)self,pred);
 return NULL;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeList_RLocate(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *pred) {
 Dee_size_t i; int temp; DeeObject *pred_result_ob,*args,*my_elem;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(pred));
 i = (Dee_size_t)-1;
 DeeList_ACQUIRE(self);
 while (1) {
  if DEE_UNLIKELY(i >= DeeList_SIZE(self)) i = DeeList_SIZE(self);
  if DEE_UNLIKELY(!i--) break;
  Dee_INCREF(my_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  if DEE_UNLIKELY((args = DeeTuple_Pack(1,my_elem)) == NULL) {
err_my_elem: Dee_DECREF(my_elem); return NULL;
  }
  pred_result_ob = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if DEE_UNLIKELY(!pred_result_ob) goto err_my_elem;
  temp = DeeObject_Bool(pred_result_ob);
  Dee_DECREF(pred_result_ob);
  if DEE_UNLIKELY(temp != 0) {
   if (temp < 0) goto err_my_elem;
   DEE_ASSERT(temp > 0);
   return my_elem; // Inherit reference
  }
  Dee_DECREF(my_elem);
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 DeeError_ValueError_NotFound_Pred((DeeObject *)self,pred);
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeList_LocateAll(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *pred) {
 Dee_size_t i; int temp;
 DeeObject *result,*pred_result_ob,*args,*my_elem;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(pred));
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) return NULL;
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(my_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  if DEE_UNLIKELY((args = DeeTuple_Pack(1,my_elem)) == NULL) {
err_my_elem: Dee_DECREF(my_elem);
err_r: Dee_DECREF(result); return NULL;
  }
  pred_result_ob = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if DEE_UNLIKELY(!pred_result_ob) goto err_my_elem;
  temp = DeeObject_Bool(pred_result_ob);
  Dee_DECREF(pred_result_ob);
  if (temp > 0) temp = DeeList_Append(result,my_elem);
  Dee_DECREF(my_elem);
  if DEE_UNLIKELY(temp != 0) goto err_r;
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeList_Transform(
 DEE_A_IN_OBJECT(DeeListObject) const *self, DEE_A_INOUT DeeObject *pred) {
 Dee_size_t i; int temp;
 DeeObject *result,*pred_result,*args,*my_elem;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(pred));
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) return NULL;
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(my_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  args = DeeTuple_Pack(1,my_elem);
  Dee_DECREF(my_elem);
  if DEE_UNLIKELY(!args) {
err_r: Dee_DECREF(result); return NULL;
  }
  pred_result = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if DEE_UNLIKELY(!pred_result) goto err_r;
  temp = DeeList_Append(result,pred_result);
  Dee_DECREF(pred_result);
  if DEE_UNLIKELY(temp != 0) goto err_r;
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeList_TConcat(
 DEE_A_IN_OBJECT(DeeListObject) const *lhs,
 DEE_A_IN DeeTypeObject const *tp_rhs, DEE_A_INOUT DeeObject *rhs) {
 DeeObject *result; // TODO: Optimization
 if DEE_UNLIKELY((result = DeeList_Copy(lhs)) == NULL) return NULL;
 if DEE_UNLIKELY(DeeList_TAppendSequence(result,tp_rhs,rhs) != 0) Dee_CLEAR(result);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeList_TCast(
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) {
 DEE_ASSERT(DeeObject_Check(sequence));
 DEE_ASSERT(DeeObject_Check(tp_sequence) && DeeType_Check(tp_sequence));
 DEE_ASSERT(DeeObject_InstanceOf(sequence,tp_sequence));
 if (tp_sequence == &DeeList_Type) DeeReturn_NEWREF(sequence);
 if (tp_sequence == &DeeSuper_Type) return DeeList_TCast(DeeSuper_TYPE(sequence),DeeSuper_SELF(sequence));
 return DeeList_TNewFromSequence(tp_sequence,sequence);
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeList_Merge(
 DEE_A_INOUT_OBJECT(DeeListObject) *lhs, DEE_A_INOUT DeeObject *rhs) {
 DeeObject *lhs_iter,*rhs_iter,*result; // TODO: Optimization
 if DEE_UNLIKELY((lhs_iter = DeeList_IterSelf(lhs)) == NULL) return NULL;
 if DEE_UNLIKELY((rhs_iter = DeeObject_IterSelf(rhs)) == NULL) { result = NULL; goto err_0; }
 result = DeeGeneric_MergeIterators(lhs_iter,rhs_iter);
 Dee_DECREF(rhs_iter);
err_0: Dee_DECREF(lhs_iter);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeList_InplaceMerge(
 DEE_A_INOUT_OBJECT(DeeListObject) *lhs, DEE_A_INOUT DeeObject *rhs) {
 DeeObject *new_list; // TODO: Optimization
 if DEE_UNLIKELY((new_list = DeeList_Merge(lhs,rhs)) == NULL) return -1;
 DeeList_MoveAssign(lhs,new_list);
 Dee_DECREF(new_list);
 return 0;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeList_Common(
 DEE_A_INOUT_OBJECT(DeeListObject) *lhs, DEE_A_INOUT DeeObject *rhs) {
 DeeObject *lhs_iter,*rhs_iter,*result; // TODO: Optimization
 if DEE_UNLIKELY((lhs_iter = DeeList_IterSelf(lhs)) == NULL) return NULL;
 if DEE_UNLIKELY((rhs_iter = DeeObject_IterSelf(rhs)) == NULL) { result = NULL; goto err_0; }
 result = DeeGeneric_CommonIterators(lhs_iter,rhs_iter);
 Dee_DECREF(rhs_iter);
err_0: Dee_DECREF(lhs_iter);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeList_InplaceCommon(
 DEE_A_INOUT_OBJECT(DeeListObject) *lhs, DEE_A_INOUT DeeObject *rhs) {
 DeeObject *new_list; // TODO: Optimization
 if DEE_UNLIKELY((new_list = DeeList_Common(lhs,rhs)) == NULL) return -1;
 DeeList_MoveAssign(lhs,new_list);
 Dee_DECREF(new_list);
 return 0;
}


DEE_A_RET_EXCEPT(-1) int DeeList_Hash(
 DEE_A_IN_OBJECT(DeeListObject) const *self,
 DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result) {
 Dee_size_t i; DeeObject *list_elem;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_UNLIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(list_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  if DEE_UNLIKELY(DeeObject_HashInplace(list_elem,start) != 0) {
   Dee_DECREF(list_elem);
   return -1;
  }
  Dee_DECREF(list_elem);
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 *result = start;
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeList_Contains(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *elem) {
 int temp; DeeObject *list_elem; Dee_size_t i;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(list_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  temp = DeeObject_CompareEq(list_elem,elem);
  Dee_DECREF(list_elem);
  if DEE_UNLIKELY(temp != 0) return temp; // Error or found
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 return 0; // not found
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeList_ContainsPred(
 DEE_A_INOUT_OBJECT(DeeListObject) *self,
 DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) {
 int temp; DeeObject *list_elem,*pred_args,*pred_result; Dee_size_t i;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(list_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  pred_args = DeeTuple_Pack(2,list_elem,elem);
  Dee_DECREF(list_elem);
  if DEE_UNLIKELY(!pred_args) return -1;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) {
check_ni:
   if DEE_LIKELY(DeeError_Catch(&DeeErrorType_NotImplemented)) temp = 0;
   else return -1;
  } else {
   temp = DeeObject_Bool(pred_result);
   Dee_DECREF(pred_result);
   if DEE_UNLIKELY(temp < 0) goto check_ni;
  }
  if DEE_UNLIKELY(temp != 0) return temp; // Error or found
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 return 0; // not found
}



DEE_A_RET_EXCEPT(-1) int DeeList_SortedInsert(
 DEE_A_INOUT_OBJECT(DeeListObject) *self, DEE_A_INOUT DeeObject *elem) {
 Dee_size_t i; int temp; DeeObject *my_elem;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(my_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  temp = DeeObject_CompareLo(my_elem,elem);
  Dee_DECREF(my_elem);
  if DEE_UNLIKELY(temp < 0) {
   if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return -1;
  } else if (!temp) goto end;
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
end:
 return DeeList_Insert(self,i,elem);
}
DEE_A_RET_EXCEPT(-1) int DeeList_SortedInsertPred(
 DEE_A_INOUT_OBJECT(DeeListObject) *self,
 DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) {
 Dee_size_t i; int temp; DeeObject *my_elem,*pred_args,*pred_result_ob;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(elem));
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(my_elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  pred_args = DeeTuple_Pack(2,my_elem,elem);
  Dee_DECREF(my_elem);
  if DEE_UNLIKELY(!pred_args) return -1;
  pred_result_ob = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result_ob) return -1;
  temp = DeeObject_Bool(pred_result_ob);
  Dee_DECREF(pred_result_ob);
  if DEE_UNLIKELY(temp < 0) return -1;
  if (!temp) goto end;
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
end:
 return DeeList_Insert(self,i,elem);
}















static int _deelist_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeListObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 _DeeList_InitEmpty(self);
 return 0;
}
static void _deelist_tp_dtor(DeeListObject *self) {
 DeeObject **begin,**end;
 end = (begin = DeeList_ELEM(self))+DeeList_SIZE(self);
 while DEE_LIKELY(end != begin) Dee_DECREF(*--end);
 DEE_ASSERT(begin == DeeList_ELEM(self));
 free(begin);
}
static void _DeeList_Move(
 DeeListObject *self, DeeListObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeList_Check(right));
 DeeList_ACQUIRE(right);
 self->l_len = right->l_len;
 self->l_alloc = right->l_alloc;
 self->l_elem = right->l_elem;
 right->l_len = 0;
 right->l_alloc = 0;
 right->l_elem = NULL;
 DeeList_RELEASE(right);
}
static int _deelist_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeListObject *self, DeeListObject *right) {
 return DeeList_InitCopy(self,(DeeObject *)right);
}
static int _deelist_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeListObject *self, DeeListObject *right) {
 _DeeList_InitCommon(self);
 _DeeList_Move(self,right);
 return 0;
}
static int _deelist_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeListObject *self, DeeObject *args) {
 DeeObject *arg;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:list",&arg) != 0) return -1;
 return DeeList_InitFromSequence(self,arg);
}
static int _deelist_tp_move_assign(
 DeeListObject *self, DeeListObject *right) {
 DeeList_MoveAssign((DeeObject *)self,(DeeObject *)right);
 return 0;
}
#ifdef DeeList_AssignSequence
static int _deelist_tp_any_assign(
 DeeListObject *self, DeeObject *right) {
 return DeeList_AssignSequence((DeeObject *)self,right);
}
#else
#define _deelist_tp_any_assign DeeList_AssignSequence
}
#endif
DEE_VISIT_PROC(_deelist_tp_visit,DeeListObject *self) {
 DeeObject **elem,**end;
 DeeList_ACQUIRE(self);
 end = (elem = DeeList_ELEM(self))+DeeList_SIZE(self);
 while DEE_LIKELY(elem != end) Dee_VISIT(*elem++);
 DeeList_RELEASE(self);
}
static DeeObject *_deelist_tp_add(DeeListObject *lhs, DeeObject *rhs) {
 return DeeList_Concat((DeeObject *)lhs,rhs);
}
static DeeObject *_deelist_tp_iadd(DeeListObject *lhs, DeeObject *rhs) {
 if DEE_UNLIKELY(DeeList_InplaceConcat((DeeObject *)lhs,rhs) != 0) return NULL;
 DeeReturn_NEWREF(lhs);
}
static DeeObject *_deelist_tp_mul(DeeListObject *lhs, DeeObject *rhs) {
 Dee_size_t n;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,rhs,&n) != 0) return NULL;
 return DeeList_Repeated((DeeObject *)lhs,n);
}
static DeeObject *_deelist_tp_imul(DeeListObject *lhs, DeeObject *rhs) {
 Dee_size_t n;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,rhs,&n) != 0) return NULL;
 if DEE_UNLIKELY(DeeList_Repeat((DeeObject *)lhs,n) != 0) return NULL;
 DeeReturn_NEWREF(lhs);
}
static DeeObject *_deelist_tp_iand(DeeListObject *lhs, DeeObject *rhs) {
 if DEE_UNLIKELY(DeeList_InplaceCommon((DeeObject *)lhs,rhs) != 0) return NULL;
 DeeReturn_NEWREF(lhs);
}
static DeeObject *_deelist_tp_ior(DeeListObject *lhs, DeeObject *rhs) {
 if DEE_UNLIKELY(DeeList_InplaceMerge((DeeObject *)lhs,rhs) != 0) return NULL;
 DeeReturn_NEWREF(lhs);
}
static DeeObject *_deelist_tp_str(DeeListObject *self) {
 DeeObject *result,*elem; Dee_size_t i;
 DeeStringWriter writer = DeeStringWriter_INIT();
 if DEE_UNLIKELY(DeeStringWriter_WriteChar(&writer,'[') != 0) goto err;
 for (i = 0;;++i) {
  DeeList_ACQUIRE(self);
  if DEE_UNLIKELY(i >= DeeList_SIZE(self)) { DeeList_RELEASE(self); break; }
  Dee_INCREF(elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  if DEE_UNLIKELY((i != 0 && DeeStringWriter_WriteChar(&writer,',') != 0) ||
                  DeeStringWriter_WriteObjectStr(&writer,elem) != 0)
  { Dee_DECREF(elem); goto err; }
  Dee_DECREF(elem);
 }
 if DEE_UNLIKELY(DeeStringWriter_WriteChar(&writer,']') != 0) goto err;
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}
static DeeObject *_deelist_tp_repr(DeeListObject *self) {
 DeeObject *result,*elem; Dee_size_t i;
 DeeStringWriter writer = DeeStringWriter_INIT();
 if DEE_UNLIKELY(DeeStringWriter_WriteChar(&writer,'[') != 0) goto err;
 for (i = 0;;++i) {
  DeeList_ACQUIRE(self);
  if DEE_UNLIKELY(i >= DeeList_SIZE(self)) { DeeList_RELEASE(self); break; }
  Dee_INCREF(elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  if DEE_UNLIKELY((i != 0 && DeeStringWriter_WriteChar(&writer,',') != 0) ||
                  DeeStringWriter_WriteObjectRepr(&writer,elem) != 0)
  { Dee_DECREF(elem); goto err; }
  Dee_DECREF(elem);
 }
 if DEE_UNLIKELY(DeeStringWriter_WriteChar(&writer,']') != 0) goto err;
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}

static int _deelist_tp_bool(DeeListObject *self) {
 int result;
 DeeList_ACQUIRE(self);
 result = DeeList_SIZE(self) != 0;
 DeeList_RELEASE(self);
 return result;
}
static DeeObject *_deelist_tp_not(DeeListObject *self) {
 int result;
 DeeList_ACQUIRE(self);
 result = DeeList_SIZE(self) != 0;
 DeeList_RELEASE(self);
 DeeReturn_Bool(!result);
}
static DeeObject *_deelist_tp_seq_size(DeeListObject *self) {
 Dee_size_t result;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DeeList_ACQUIRE(self);
 result = DeeList_SIZE(self);
 DeeList_RELEASE(self);
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *_deelist_tp_seq_contains(DeeListObject *self, DeeObject *ob) {
 int temp;
 if DEE_UNLIKELY((temp = DeeList_Contains((DeeObject *)self,ob)) < 0) return NULL;
 DeeReturn_Bool(temp != 0);
}
static DeeObject *_deelist_tp_seq_get(DeeListObject *self, DeeObject *i) {
 Dee_ssize_t ii;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(i));
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return NULL;
 return DeeList_Get((DeeObject *)self,ii);
}
static int _deelist_tp_seq_del(DeeListObject *self, DeeObject *i) {
 Dee_ssize_t ii;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return -1;
 return DeeList_Del((DeeObject *)self,ii);
}
static int _deelist_tp_seq_set(DeeListObject *self, DeeObject *i, DeeObject *v) {
 Dee_ssize_t ii;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 DEE_ASSERT(DeeObject_Check(i));
 DEE_ASSERT(DeeObject_Check(v));
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return -1;
 return DeeList_Set((DeeObject *)self,ii,v);
}

static DeeObject *_deelist_tp_seq_range_get(
 DeeListObject *self, DeeObject *lo, DeeObject *hi) {
 Dee_ssize_t temp; Dee_size_t used_lo,used_hi;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 if (lo == Dee_None) used_lo = 0; else {
  if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,lo,&temp) != 0) return NULL;
  used_lo = temp < 0 ? (Dee_size_t)0 : (Dee_size_t)temp;
 }
 if (hi == Dee_None) used_hi = (Dee_size_t)-1; else {
  if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,hi,&temp) != 0) return NULL;
  used_hi = temp < 0 ? (Dee_size_t)0 : (Dee_size_t)temp;
 }
 return DeeList_GetRange((DeeObject *)self,used_lo,used_hi);
}
static int _deelist_tp_seq_range_del(
 DeeListObject *self, DeeObject *lo, DeeObject *hi) {
 Dee_ssize_t temp; Dee_size_t used_lo,used_hi;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 if (lo == Dee_None) used_lo = 0; else {
  if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,lo,&temp) != 0) return -1;
  used_lo = temp < 0 ? (Dee_size_t)0 : (Dee_size_t)temp;
 }
 if (hi == Dee_None) used_hi = (Dee_size_t)-1; else {
  if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,hi,&temp) != 0) return -1;
  used_hi = temp < 0 ? (Dee_size_t)0 : (Dee_size_t)temp;
 }
 return DEE_LIKELY(used_hi > used_lo) ? DeeList_Erase((
  DeeObject *)self,used_lo,(Dee_size_t)(used_hi-used_lo)) : 0;
}
static int _deelist_tp_seq_range_set(
 DeeListObject *self, DeeObject *lo, DeeObject *hi, DeeObject *v) {
 Dee_ssize_t temp; Dee_size_t used_lo,used_hi;
 DEE_ASSERT(DeeObject_Check(self) && DeeList_Check(self));
 if (lo == Dee_None) used_lo = 0; else {
  if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,lo,&temp) != 0) return -1;
  used_lo = temp < 0 ? (Dee_size_t)0 : (Dee_size_t)temp;
 }
 if (hi == Dee_None) used_hi = (Dee_size_t)-1; else {
  if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,hi,&temp) != 0) return -1;
  used_hi = temp < 0 ? (Dee_size_t)0 : (Dee_size_t)temp;
 }
 return DeeList_SetRange((DeeObject *)self,used_lo,used_hi,v);
}


static int _deelistiterator_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeListIteratorObject *self) {
 if DEE_UNLIKELY((self->li_list = (DeeListObject *)DeeList_NewEmpty()) == NULL) return -1;
 self->li_pos = 0;
 return 0;
}
static void _deelistiterator_tp_dtor(DeeListIteratorObject *self) {
 Dee_DECREF(self->li_list);
}
DEE_VISIT_PROC(_deelistiterator_tp_visit,DeeListIteratorObject *self) {
 Dee_VISIT(self->li_list);
}
static int _deelistiterator_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeListIteratorObject *self, DeeListIteratorObject *right) {
 Dee_INCREF(self->li_list = right->li_list);
 self->li_pos = right->li_pos;
 return 0;
}
static int _deelistiterator_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeListIteratorObject *self, DeeObject *args) {
 self->li_pos = 0;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|Iu:list.iterator",&self->li_list,&self->li_pos) != 0) return -1;
 if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&self->li_list,&DeeList_Type) != 0) return -1;
 Dee_INCREF(self->li_list);
 return 0;
}
static DeeObject *_deelistiterator_tp_str(DeeListIteratorObject *self) {
 return DeeString_Newf("<list.iterator %Iu/%Iu>",
                       self->li_pos,DeeList_SIZE(self->li_list));
}
static DeeObject *_deelistiterator_tp_repr(DeeListIteratorObject *self) {
 return DeeString_Newf("<list.iterator %Iu/%Iu | %r>",
                       self->li_pos,DeeList_SIZE(self->li_list),self->li_list);
}
static DeeObject *_deelistiterator_tp_cmp_lo(DeeListIteratorObject *self, DeeListIteratorObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeListIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeListIterator_LIST(self) < DeeListIterator_LIST(right) || (DeeListIterator_LIST(self) == DeeListIterator_LIST(right) && DeeListIterator_POS(self) < DeeListIterator_POS(right))); }
static DeeObject *_deelistiterator_tp_cmp_le(DeeListIteratorObject *self, DeeListIteratorObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeListIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeListIterator_LIST(self) < DeeListIterator_LIST(right) || (DeeListIterator_LIST(self) == DeeListIterator_LIST(right) && DeeListIterator_POS(self) <= DeeListIterator_POS(right))); }
static DeeObject *_deelistiterator_tp_cmp_eq(DeeListIteratorObject *self, DeeListIteratorObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeListIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeListIterator_LIST(self) == DeeListIterator_LIST(right) && DeeListIterator_POS(self) == DeeListIterator_POS(right)); }
static DeeObject *_deelistiterator_tp_cmp_ne(DeeListIteratorObject *self, DeeListIteratorObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeListIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeListIterator_LIST(self) != DeeListIterator_LIST(right) || DeeListIterator_POS(self) != DeeListIterator_POS(right)); }
static DeeObject *_deelistiterator_tp_cmp_gr(DeeListIteratorObject *self, DeeListIteratorObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeListIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeListIterator_LIST(self) > DeeListIterator_LIST(right) || (DeeListIterator_LIST(self) == DeeListIterator_LIST(right) && DeeListIterator_POS(self) > DeeListIterator_POS(right))); }
static DeeObject *_deelistiterator_tp_cmp_ge(DeeListIteratorObject *self, DeeListIteratorObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeListIterator_Type) != 0) return NULL; DeeReturn_Bool(DeeListIterator_LIST(self) > DeeListIterator_LIST(right) || (DeeListIterator_LIST(self) == DeeListIterator_LIST(right) && DeeListIterator_POS(self) >= DeeListIterator_POS(right))); }
static int _deelistiterator_tp_seq_iter_next(DeeListIteratorObject *self, DeeObject **result) {
 DeeList_ACQUIRE(self->li_list);
 if DEE_UNLIKELY(self->li_pos >= DeeList_SIZE(self->li_list)) { DeeList_RELEASE(self->li_list); return 1; }
 Dee_INCREF(*result = DeeList_GET(self->li_list,self->li_pos));
 ++self->li_pos;
 DeeList_RELEASE(self->li_list);
 return 0;
}







//////////////////////////////////////////////////////////////////////////
// List member functions
static DeeObject *_deelist_sorted_insert(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem,*pred = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:sorted_insert",&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___lo__)
  ? DeeList_SortedInsert((DeeObject *)self,elem)
  : DeeList_SortedInsertPred((DeeObject *)self,elem,pred)
  ) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelist_remove(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem,*pred = Dee_None; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:remove",&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = ((DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeList_Remove((DeeObject *)self,elem)
  : DeeList_RemovePred((DeeObject *)self,elem,pred)
  )) != 0) return NULL;
 DeeReturn_Bool(result == 0);
}
static DeeObject *_deelist_insert(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_ssize_t i; Dee_size_t size; DeeObject *elem;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Ido:insert",&i,&elem) != 0) return NULL;
 if DEE_UNLIKELY((size = DeeList_SIZE(self)) != 0) DEE_PRIVATE_CLAMP_INDEX(i,size); else i = 0;
 if DEE_UNLIKELY(DeeList_Insert((DeeObject *)self,(Dee_size_t)i,elem) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelist_pop_front(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":pop_front") != 0) return NULL;
 return DeeList_Pop((DeeObject *)self,0);
}
static DeeObject *_deelist_pop_back(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":pop_back") != 0) return NULL;
 return DeeList_Pop((DeeObject *)self,(Dee_size_t)-1);
}
static DeeObject *_deelist_push_front(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeList_InsertVector((DeeObject *)self,0,
  DeeTuple_SIZE(args),DeeTuple_ELEM(args)) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelist_push_back(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeList_AppendVector((DeeObject *)self,
  DeeTuple_SIZE(args),DeeTuple_ELEM(args)) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelist_insert_list(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t i; DeeObject *seq;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iuo:insert_list",&i,&seq) != 0) return NULL;
 if DEE_UNLIKELY(DeeList_InsertSequence((DeeObject *)self,i,seq) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelist_insert_iter(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t i; DeeObject *iter;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iuo:insert_iter",&i,&iter) != 0) return NULL;
 if DEE_UNLIKELY(DeeList_InsertIterator((DeeObject *)self,i,iter) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelist_append(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeList_AppendVector((DeeObject *)self,
  DeeTuple_SIZE(args),DeeTuple_ELEM(args)) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelist_extend(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject **iter,**end;
 for (end = (iter = DeeTuple_ELEM(args))+DeeTuple_SIZE(args); DEE_UNLIKELY(iter != end); ++iter)
  if DEE_UNLIKELY(DeeList_AppendSequence((DeeObject *)self,*iter) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelist_erase(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t i,n = 1;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu|Iu:erase",&i,&n) != 0) return NULL;
 if DEE_UNLIKELY(DeeList_Erase((DeeObject *)self,i,n) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelist_pop(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t i = (Dee_size_t)-1;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|Iu:pop",&i) != 0) return NULL;
 return DeeList_Pop((DeeObject *)self,i);
}
static DeeObject *_deelist_clear(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":clear") != 0) return NULL;
 DeeList_Clear((DeeObject *)self);
 DeeReturn_None;
}
static DeeObject *_deelist_resize(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t new_size; DeeObject *filler = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu|o:resize",&new_size,&filler) != 0) return NULL;
 if DEE_UNLIKELY(DeeList_Resize((DeeObject *)self,new_size,filler) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelist_reserve(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t reserved_elems;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu:reserve",&reserved_elems) != 0) return NULL;
 if DEE_UNLIKELY(DeeList_Reserve((DeeObject *)self,reserved_elems) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelist_shrink_to_fit(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":shrink_to_fit") != 0) return NULL;
 DeeList_ShrinkToFit((DeeObject *)self);
 DeeReturn_None;
}
static DeeObject *_deelist_reverse(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":reverse") != 0) return NULL;
 DeeList_Reverse((DeeObject *)self);
 DeeReturn_None;
}
static DeeObject *_deelist_find(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem,*pred = Dee_None; Dee_ssize_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:find",&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeList_Find((DeeObject *)self,elem)
  : DeeList_FindPred((DeeObject *)self,elem,pred)) == -2) return NULL;
 return DeeObject_New(Dee_ssize_t,result);
}
static DeeObject *_deelist_index(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem,*pred = Dee_None; Dee_size_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:index",&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeList_Index((DeeObject *)self,elem)
  : DeeList_IndexPred((DeeObject *)self,elem,pred)) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *_deelist_rfind(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem,*pred = Dee_None; Dee_ssize_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:rfind",&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeList_RFind((DeeObject *)self,elem)
  : DeeList_RFindPred((DeeObject *)self,elem,pred)) == -2) return NULL;
 return DeeObject_New(Dee_ssize_t,result);
}
static DeeObject *_deelist_rindex(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem,*pred = Dee_None; Dee_size_t result;
 if (DeeTuple_Unpack(args,"o|o:rindex",&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeList_RIndex((DeeObject *)self,elem)
  : DeeList_RIndexPred((DeeObject *)self,elem,pred)) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *_deelist_count(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem,*pred = Dee_None; Dee_size_t result;
 if (DeeTuple_Unpack(args,"o|o:count",&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeList_Count((DeeObject *)self,elem)
  : DeeList_CountPred((DeeObject *)self,elem,pred)) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *_deelist_sort(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:sort",&pred) != 0) return NULL;
 if DEE_UNLIKELY((DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___lo__)
  ? DeeList_Sort((DeeObject *)self)
  : DeeList_SortPred((DeeObject *)self,pred)) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelist_sorted(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:sorted",&pred) != 0) return NULL;
 return DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___lo__)
  ? DeeList_Sorted((DeeObject *)self) : DeeList_SortedPred((DeeObject *)self,pred);
}
static DeeObject *_deelist_reversed(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":reversed") != 0) return NULL;
 return DeeList_Reversed((DeeObject *)self);
}
static DeeObject *_deelist_empty(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":empty") != 0) return NULL;
 DeeReturn_Bool(DeeList_Size((DeeObject *)self) == 0);
}
static DeeObject *_deelist_non_empty(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":non_empty") != 0) return NULL;
 DeeReturn_Bool(DeeList_Size((DeeObject *)self) != 0);
}
static DeeObject *_deelist_front(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":front") != 0) return NULL;
 return DeeList_Front((DeeObject *)self);
}
static DeeObject *_deelist_back(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":back") != 0) return NULL;
 return DeeList_Back((DeeObject *)self);
}
static DeeObject *_deelist_at(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t index;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu:at",&index) != 0) return NULL;
 return DeeList_At((DeeObject *)self,index);
}
static DeeObject *_deelist_locate(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:locate",&pred) != 0) return NULL;
 return DeeList_Locate((DeeObject *)self,pred);
}
static DeeObject *_deelist_rlocate(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:rlocate",&pred) != 0) return NULL;
 return DeeList_RLocate((DeeObject *)self,pred);
}
static DeeObject *_deelist_locate_all(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:locate_all",&pred) != 0) return NULL;
 return DeeList_LocateAll((DeeObject *)self,pred);
}
static DeeObject *_deelist_transform(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:transform",&pred) != 0) return NULL;
 return DeeList_Transform((DeeObject *)self,pred);
}
static DeeObject *_deelist_segments(
 DeeTupleObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t n;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu:segments",&n) != 0) return NULL;
 return DeeList_Segments((DeeObject *)self,n);
}
static DeeObject *_deelist_contains(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem,*pred = Dee_None; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:contains",&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeList_Contains((DeeObject *)self,elem)
  : DeeList_ContainsPred((DeeObject *)self,elem,pred)) < 0) return NULL;
 DeeReturn_Bool(result);
}
static DeeObject *_deelist_remove_if(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t result; DeeObject *pred;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:remove_if",&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeList_RemoveIf((DeeObject *)self,pred)) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *_deelist_allocated(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":allocated") != 0) return NULL;
 return DeeObject_New(Dee_size_t,DeeList_Alloc((DeeObject *)self));
}
static DeeObject *_deelist_fill(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *fill_ob; Dee_size_t fill_size = DeeList_SIZE(self);
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|Iu:fill",&fill_ob,&fill_size) != 0) return NULL;
 if DEE_UNLIKELY(DeeList_FillAssign((DeeObject *)self,fill_ob,fill_size) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelist_unique(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:unique",&pred) != 0) return NULL;
 if DEE_UNLIKELY((DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeList_Unique((DeeObject *)self)
  : DeeList_UniquePred((DeeObject *)self,pred)) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelist_tounique(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:tounique",&pred) != 0) return NULL;
 return DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeList_ToUnique((DeeObject *)self) : DeeList_ToUniquePred((DeeObject *)self,pred);
}
static DeeObject *_deelist_extend_unique(
 DeeListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *seq,*pred = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:extend_unique",&seq,&pred) != 0) return NULL;
 if DEE_UNLIKELY((DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeList_AppendUniqueSequence((DeeObject *)self,seq)
  : DeeList_AppendUniqueSequencePred((DeeObject *)self,seq,pred)) != 0) return NULL;
 DeeReturn_None;
}




static struct DeeMemberDef const _deelist_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeListIterator_Type),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMethodDef const _deelist_tp_methods[] = {
 DEE_METHODDEF_CONST_v100("at",member(&_deelist_at),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("back",member(&_deelist_back),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("contains",member(&_deelist_contains),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("count",member(&_deelist_count),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("empty",member(&_deelist_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("find",member(&_deelist_find),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("front",member(&_deelist_front),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("index",member(&_deelist_index),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("locate",member(&_deelist_locate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("locate_all",member(&_deelist_locate_all),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("non_empty",member(&_deelist_non_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rfind",member(&_deelist_rfind),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rindex",member(&_deelist_rindex),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rlocate",member(&_deelist_rlocate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("segments",member(&_deelist_segments),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("sorted",member(&_deelist_sorted),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("transform",member(&_deelist_transform),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("remove_if",member(&_deelist_remove_if),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("remove",member(&_deelist_remove),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("insert",member(&_deelist_insert),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("insert_list",member(&_deelist_insert_list),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("insert_iter",member(&_deelist_insert_iter),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("sorted_insert",member(&_deelist_sorted_insert),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("append",member(&_deelist_append),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("extend",member(&_deelist_extend),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("erase",member(&_deelist_erase),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("pop",member(&_deelist_pop),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("clear",member(&_deelist_clear),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("resize",member(&_deelist_resize),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("reserve",member(&_deelist_reserve),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("shrink_to_fit",member(&_deelist_shrink_to_fit),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("reverse",member(&_deelist_reverse),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("sort",member(&_deelist_sort),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("reversed",member(&_deelist_reversed),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("allocated",member(&_deelist_allocated),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("fill",member(&_deelist_fill),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("push_front",member(&_deelist_push_front),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("push_back",member(&_deelist_push_back),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("pop_front",member(&_deelist_pop_front),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("pop_back",member(&_deelist_pop_back),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("unique",member(&_deelist_unique),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("tounique",member(&_deelist_tounique),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("extend_unique",member(&_deelist_extend_unique),DEE_DOC_AUTO),
 DEE_METHODDEF_END_v100
};

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deelistiterator_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deelistiterator_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__li_list",DeeListIteratorObject,li_list,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__li_pos",DeeListIteratorObject,li_pos,atomic(Dee_size_t)),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */

static int DEE_CALL _deelist_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeListObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_uint64_t size;
 if (DeeFile_GetLeSmall64(infile,&size) != 0) return -1;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(size > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "List is too big for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      size,(Dee_size_t)-1);
  return -1;
 }
#endif
 self->l_alloc = self->l_len = (Dee_size_t)size;
 if (size) {
  DeeObject **iter,**end;
  while ((self->l_elem = (DeeObject **)malloc_nz(
   self->l_len*sizeof(DeeObject *))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
  end = (iter = self->l_elem)+self->l_len;
  do if ((*iter = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {
   while (iter != self->l_elem) Dee_DECREF(*--iter);
   free_nn(self->l_elem);
   return -1;
  } while (++iter != end);
 } else self->l_elem = NULL;
 DeeAtomicMutex_Init(&self->l_lock);
 return 0;
}

static int DEE_CALL _deelist_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeListObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 DeeObject *list_elem; int error;
 Dee_size_t i,size; Dee_uint64_t written_size;
 DeeList_ACQUIRE(self);
 written_size = (Dee_uint64_t)(size = self->l_len);
 DeeList_RELEASE(self);
 if (DeeFile_PutLeSmall64(outfile,written_size) != 0) return -1;
 i = 0;
 DeeList_ACQUIRE(self);
 for (; i < size; ++i) {
  if DEE_LIKELY(i < DeeList_SIZE(self))
   Dee_INCREF(list_elem = DeeList_GET(self,i));
  else list_elem = DeeNone_New();
  DeeList_RELEASE(self);
  error = DeeMarshal_WriteObjectWithMap(outfile,map,list_elem);
  Dee_DECREF(list_elem);
  if (error != 0) return error;
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 return 0;
}

static struct DeeTypeMarshal _deelist_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(list)),
 member(&_deelist_tp_marshal_ctor),member(&_deelist_tp_marshal_put));


DeeTypeObject DeeList_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("list"),member(
  "()\n"
  "(iterable sequence)\n"
  "@param sequence: A given sequence that should be casted to a #(list)."
  "\tA resizeable and mutable vector of objects."),member(
  DEE_TYPE_FLAG_CONST_CTOR|DEE_TYPE_FLAG_HAS_GC|DEE_TYPE_FLAG_MUST_COPY|
  DEE_TYPE_FLAG_NO_ITERATE_EFFECT|DEE_TYPE_FLAG_FUNDAMENTAL),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeListObject),
  member(&_DeeGC_TpAlloc),
  member(&_deelist_tp_ctor),
  member(&_deelist_tp_copy_ctor),
  member(&_deelist_tp_move_ctor),
  member(&_deelist_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(member(&_DeeGC_TpFree),
  member(&_deelist_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&DeeList_CopyAssign),
  member(&_deelist_tp_move_assign),
  member(&_deelist_tp_any_assign)),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deelist_tp_str),
  member(&_deelist_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deelist_tp_visit),
  member(&DeeList_Clear)),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deelist_tp_not),
  member(&_deelist_tp_bool),null,null,null,null,null,null,null,
  member(&_deelist_tp_add),member(&_deelist_tp_iadd),null,null,
  member(&_deelist_tp_mul),member(&_deelist_tp_imul),null,null,
  null,null,null,null,null,null,
  member(&DeeList_Common),member(&_deelist_tp_iand),
  member(&DeeList_Merge),member(&_deelist_tp_ior),
  null,null,null,null,member(&DeeList_Hash)),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deegenericiterable_tp_cmp_lo),
  member(&_deegenericiterable_tp_cmp_le),
  member(&_deegenericiterable_tp_cmp_eq),
  member(&_deegenericiterable_tp_cmp_ne),
  member(&_deegenericiterable_tp_cmp_gr),
  member(&_deegenericiterable_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&_deelist_tp_seq_get),
  member(&_deelist_tp_seq_del),
  member(&_deelist_tp_seq_set),
  member(&_deelist_tp_seq_size),
  member(&_deelist_tp_seq_contains),
  member(&_deelist_tp_seq_range_get),
  member(&_deelist_tp_seq_range_del),
  member(&_deelist_tp_seq_range_set),
  member(&DeeList_IterSelf),null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,null,member(_deelist_tp_methods),
  member(_deelist_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deelist_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};

static int DEE_CALL _deelistiterator_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeListIteratorObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_uint64_t iterator_pos;
 if (DeeFile_GetLeSmall64(infile,&iterator_pos) != 0) return -1;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if (iterator_pos > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "List iterator position too big for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      iterator_pos,(Dee_size_t)-1);
  return -1;
 }
#endif
 self->li_pos = (Dee_size_t)iterator_pos;
 if ((self->li_list = (DeeListObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL)
  return -1;
 if (DeeError_TypeError_CheckType((DeeObject *)self->li_list,&DeeList_Type) != 0) {
  Dee_DECREF(self->li_list);
  return -1;
 }
 return 0;
}

static int DEE_CALL _deelistiterator_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeListIteratorObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 Dee_uint64_t written_pos;
 DeeList_ACQUIRE(self->li_list);
 written_pos = (Dee_uint64_t)self->li_pos;
 DeeList_RELEASE(self->li_list);
 if (DeeFile_PutLeSmall64(outfile,written_pos) != 0) return -1;
 return DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)self->li_list);
}

static struct DeeTypeMarshal _deelistiterator_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(list_iterator)),
 member(&_deelistiterator_tp_marshal_ctor),member(&_deelistiterator_tp_marshal_put));


DeeTypeObject DeeListIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("list.iterator"),member(
  "()\n"
  "(list l, size_t start = 0)\n"
  "@param l: The list to create an iterator for\n"
  "@param start: The starting index, defaulting to 0 if not given\n"
  "\tCreates a new list iterator form a given @list, starting a given index @(start)."),null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeListIteratorObject),null,
  member(&_deelistiterator_tp_ctor),
  member(&_deelistiterator_tp_copy_ctor),null,
  member(&_deelistiterator_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deelistiterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deelistiterator_tp_str),
  member(&_deelistiterator_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deelistiterator_tp_visit),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deelistiterator_tp_cmp_lo),
  member(&_deelistiterator_tp_cmp_le),
  member(&_deelistiterator_tp_cmp_eq),
  member(&_deelistiterator_tp_cmp_ne),
  member(&_deelistiterator_tp_cmp_gr),
  member(&_deelistiterator_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,
  member(&_deelistiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deelistiterator_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deelistiterator_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_LIST_C */
