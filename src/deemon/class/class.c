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
#ifndef GUARD_DEEMON_CLASS_C
#define GUARD_DEEMON_CLASS_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES

// /include/*
#include <deemon/bool.h>
#include <deemon/class.h>
#include <deemon/optional/object_pool.h>
#include <deemon/optional/object_visit.h>
#include <deemon/compiler/code.h>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/gc.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/iterator.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/memberfunction.h>
#include <deemon/mp/thread.h>
#include <deemon/none.h>
#include <deemon/object.h>
#include <deemon/optional/object_hash.h>
#include <deemon/runtime/function.h>
#include <deemon/structured.h>
#include <deemon/tuple.h>
#include <deemon/type.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

#if !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
DEE_A_RET_NEVER_NULL DeeObject *_DeeClassDynamicVTableList_GetKnownOperator(
 DEE_A_IN struct _DeeClassDynamicVTableList const *self, DEE_A_IN int slot_id) {
 struct _DeeClassDynamicVTableEntry *iter;
 DEE_ASSERT(self);
 iter = self->dvt_tablev;
 DEE_ASSERT(iter && self->dvt_tablec);
 while (iter->dvte_slot != slot_id) {
  ++iter;
  DEE_ASSERT(iter != self->dvt_tablev+self->dvt_tablec);
 }
 return iter->dvte_callback;
}
DEE_A_RET_MAYBE_NULL DeeObject *_DeeClassDynamicVTableList_GetOperator(
 DEE_A_IN struct _DeeClassDynamicVTableList const *self, DEE_A_IN int slot_id) {
 struct _DeeClassDynamicVTableEntry *iter,*end;
 DEE_ASSERT(self);
 end = (iter = self->dvt_tablev)+self->dvt_tablec;
 while (1) {
  if (iter == end) return NULL;
  if (iter->dvte_slot == slot_id) return iter->dvte_callback;
  ++iter;
 }
}
DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeClassDynamicVTableList_DelOperator(
 DEE_A_INOUT struct _DeeClassDynamicVTableList *self, DEE_A_IN int slot_id) {
 struct _DeeClassDynamicVTableEntry *iter,*end;
 DeeObject *old_callback;
 Dee_size_t index,new_size;
 DEE_ASSERT(self);
 end = (iter = self->dvt_tablev)+self->dvt_tablec;
 while (1) {
  if (iter == end) return 1;
  if (iter->dvte_slot == slot_id) {
   index = (Dee_size_t)(iter-self->dvt_tablev);
   DEE_ASSERT(index < self->dvt_tablec);
   new_size = self->dvt_tablec-1;
   old_callback = iter->dvte_callback; // inherit reference
   if (new_size) {
    while DEE_UNLIKELY((iter = (struct _DeeClassDynamicVTableEntry *)realloc_nnz(
     self->dvt_tablev,new_size*sizeof(struct _DeeClassDynamicVTableEntry))) == NULL) {
     if DEE_LIKELY(Dee_CollectMemory()) continue;
     DeeError_NoMemory();
     return -1;
    }
    memcpy(iter+index,iter+(index+1),(
     new_size-index)*sizeof(struct _DeeClassDynamicVTableEntry));
    self->dvt_tablec = new_size;
    self->dvt_tablev = iter;
   } else {
    free_nn(self->dvt_tablev);
   }
   Dee_DECREF(old_callback);
   return 0;
  }
  ++iter;
 }
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int _DeeClassDynamicVTableList_SetOperator(
 DEE_A_INOUT struct _DeeClassDynamicVTableList *self,
 DEE_A_IN int slot_id, DEE_A_IN DeeObject *callback) {
 struct _DeeClassDynamicVTableEntry *iter,*end;
 DeeObject *old_callback; Dee_size_t new_size;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(callback));
 end = (iter = self->dvt_tablev)+self->dvt_tablec;
 while (1) {
  if (iter == end) break;
  if (iter->dvte_slot == slot_id) {
   old_callback = iter->dvte_callback;
   Dee_INCREF(iter->dvte_callback = callback);
   Dee_DECREF(old_callback);
   return 0;
  }
  ++iter;
 }
 new_size = self->dvt_tablec+1;
 while DEE_UNLIKELY((iter = (struct _DeeClassDynamicVTableEntry *)realloc_nz(
  self->dvt_tablev,new_size*sizeof(struct _DeeClassDynamicVTableEntry))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 self->dvt_tablec = new_size;
 self->dvt_tablev = iter;
 iter += new_size-1;
 Dee_INCREF(iter->dvte_callback = callback);
 iter->dvte_slot = slot_id;
 return 0;
}
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */


DEE_OBJECTPOOL_DEFINE(instance_method,DeeInstanceMethodObject)
Dee_size_t DeeInstanceMethod_ClearPool(void) {
 return DEE_OBJECTPOOL_CLEAR(instance_method);
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeInstanceMethodObject) *DeeInstanceMethod_New(
 DEE_A_INOUT DeeObject *this_arg, DEE_A_INOUT DeeObject *func) {
 DeeInstanceMethodObject *result;
 DEE_ASSERT(DeeObject_Check(this_arg));
 DEE_ASSERT(DeeObject_Check(func));
 if DEE_UNLIKELY((result = DEE_OBJECTPOOL_ALLOC(instance_method)) != NULL) {
  DeeObject_INIT(result,&DeeInstanceMethod_Type);
  Dee_INCREF(result->im_this = this_arg);
  Dee_INCREF(result->im_func = func);
 }
 return (DeeObject *)result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeInstanceMethod_Call(
 DEE_A_INOUT_OBJECT(DeeInstanceMethodObject) *self,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DEE_ASSERT(DeeObject_Check(self) && DeeInstanceMethod_Check(self));
 return DeeInstanceMethod_CALL(self,args);
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeePropertyObject) *DeeProperty_New(
 DEE_A_IN_OPT DeeObject *getter_, DEE_A_IN_OPT DeeObject *delete_,
 DEE_A_IN_OPT DeeObject *setter_) {
 DeePropertyObject *result;
 if DEE_UNLIKELY((result = DeeGC_MALLOC(DeePropertyObject)) != NULL) {
  DeeObject_INIT(result,&DeeProperty_Type);
  Dee_XINCREF(result->pr_get = getter_);
  Dee_XINCREF(result->pr_del = delete_);
  Dee_XINCREF(result->pr_set = setter_);
  DeeGC_TrackedAdd((DeeObject *)result);
 }
 return (DeeObject *)result;
}
DEE_A_RET_MAYBE_NULL DEE_A_RET_WUNUSED DeeObject *
DeeProperty_Getter(DEE_A_IN_OBJECT(DeePropertyObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeProperty_Check(self));
 return DeeProperty_GETTER(self);
}
DEE_A_RET_MAYBE_NULL DEE_A_RET_WUNUSED DeeObject *
DeeProperty_Delete(DEE_A_IN_OBJECT(DeePropertyObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeProperty_Check(self));
 return DeeProperty_DELETE(self);
}
DEE_A_RET_MAYBE_NULL DEE_A_RET_WUNUSED DeeObject *
DeeProperty_Setter(DEE_A_IN_OBJECT(DeePropertyObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeProperty_Check(self));
 return DeeProperty_SETTER(self);
}



#define DEE_CLASSTYPE_CACHE_SIZE 16
struct dee_classtype_cache_entry {
 struct dee_classtype_cache_entry       *ctce_next;       /*< [0..1][owned] Next cache entry. */
                    DeeTypeObject const *ctce_base_type;  /*< [1..1] The address of the base_type (this is a dangling pointer to a type-type). */
     DEE_A_WEAK_REF DeeClassTypeObject  *ctce_class_type; /*< [1..1] A weak reference to the class type associated with 'ob_base_type'. */
};
// Index == (Dee_uintptr_t)base_type % DEE_CLASSTYPE_CACHE_SIZE
static struct dee_classtype_cache_entry *dee_classtype_cache[DEE_CLASSTYPE_CACHE_SIZE] = {NULL,};
static struct DeeAtomicMutex dee_classtype_cache_lock = DeeAtomicMutex_INIT();

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeClassTypeObject *)
_DeeClassType_New(DEE_A_IN DeeTypeObject const *base_type);
DEE_A_RET_EXCEPT_REF DeeClassTypeObject *
DeeClassType_TypeOf(DEE_A_IN DeeTypeObject const *base) {
 DeeClassTypeObject *result,*found_type;
 struct dee_classtype_cache_entry **bucket,*entry,*prev,*next;
 DEE_ASSERT(DeeObject_Check(base) && DeeType_Check(base));
 base = Dee_TYPE(base);
 if (DeeClassType_Check(base)) {
  Dee_INCREF(base);
  return (DeeClassTypeObject *)base;
 }
 DeeAtomicMutex_Acquire(&dee_classtype_cache_lock);
 entry = *(bucket = dee_classtype_cache+((Dee_uintptr_t)base%DEE_CLASSTYPE_CACHE_SIZE));
 prev = NULL; while (entry) {
  if ((found_type = (DeeClassTypeObject *)DeeObject_LockWeakref((DeeObject *)entry->ctce_class_type)) != NULL) {
   if (entry->ctce_base_type == base) {
    DeeAtomicMutex_Release(&dee_classtype_cache_lock);
    return found_type; // Inherit reference
   }
   prev = entry;
   Dee_DECREF(found_type);
   entry = entry->ctce_next;
  } else {
   // Dead entry (clean it up)
   next = entry->ctce_next;
   if (prev) prev->ctce_next = next;
   else *bucket = next;
   Dee_WEAKDECREF(entry->ctce_class_type);
   free_nn(entry);
   entry = next;
  }
 }
 DeeAtomicMutex_Release(&dee_classtype_cache_lock);
 // Must create a new classtype
 if ((result = _DeeClassType_New(base)) == NULL) return NULL;
aqr_and_search_faster:
 found_type = NULL;
 DeeAtomicMutex_Acquire(&dee_classtype_cache_lock);
 entry = *(bucket = dee_classtype_cache+((Dee_uintptr_t)base%DEE_CLASSTYPE_CACHE_SIZE));
 while (entry) {
  if (entry->ctce_base_type == base) {
   if ((found_type = (DeeClassTypeObject *)DeeObject_LockWeakref((DeeObject *)entry->ctce_class_type)) != NULL) {
    // Rare case: Someone was faster than us
    DeeAtomicMutex_Release(&dee_classtype_cache_lock);
    Dee_DECREF(result);
    return found_type;
   }
  }
  entry = entry->ctce_next;
 }
 if ((entry = (struct dee_classtype_cache_entry *)malloc_nz(
  sizeof(struct dee_classtype_cache_entry))) == NULL) {
  DeeAtomicMutex_Release(&dee_classtype_cache_lock);
  if DEE_LIKELY(Dee_CollectMemory()) goto aqr_and_search_faster;
  DeeError_NoMemory();
  Dee_DECREF(result);
  return NULL;
 }
 entry->ctce_next = *bucket;
 entry->ctce_base_type = base; // Dangling pointer
 Dee_WEAKINCREF(entry->ctce_class_type = result); // Weak reference
 *bucket = entry;
 DeeAtomicMutex_Release(&dee_classtype_cache_lock);
 return result;
}
void _DeeClassType_Finalize(void) {
 struct dee_classtype_cache_entry **iter,**end,*entry,*next;
 DeeAtomicMutex_AcquireRelaxed(&dee_classtype_cache_lock);
 for (end = (iter = dee_classtype_cache)+DEE_CLASSTYPE_CACHE_SIZE; iter != end; ++iter) {
  entry = *iter;
  while (entry) {
   next = entry->ctce_next;
   Dee_WEAKDECREF(entry->ctce_class_type);
   free_nn(entry);
   entry = next;
  }
 }
 DeeAtomicMutex_Release(&dee_classtype_cache_lock);
}





//////////////////////////////////////////////////////////////////////////
// Instance Attribute functions
#ifndef __INTELLISENSE__
#define STRING
#include "class.instance_attributes.inl"
#include "class.instance_attributes.inl"
#define STRING
#include "class.class_attributes.inl"
#include "class.class_attributes.inl"
#endif

#include "class.operators.inl"

// Slot: tp_seq_iter_next
static int _deeinstance_tp_seq_iter_next(DeeInstanceObject *self, DeeObject **result) {
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERTF(DeeObject_Check(DeeClass_SUFFIX(Dee_TYPE(self))->cs_seq.ctp_seq_iter_next),"Missing slot: tp_seq_iter_next");
 *result = DeeObject_ThisCall(DeeClass_SUFFIX(Dee_TYPE(self))->cs_seq.ctp_seq_iter_next,(DeeObject *)self,Dee_EmptyTuple);
#else
 *result = DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(DeeClass_SUFFIX(Dee_TYPE(self)),DeeType_SLOT_ID(tp_seq_iter_next)),(DeeObject *)self,Dee_EmptyTuple);
#endif
 return *result ? 0 : DeeError_Catch(&DeeSignalType_StopIteration) ? 1 : -1;
}

// Slot: tp_str
static DeeObject * _deeinstance_tp_str(DeeInstanceObject *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERTF(DeeObject_Check(DeeClass_SUFFIX(Dee_TYPE(self))->cs_cast.ctp_str),"Missing slot: tp_str");
 result = DeeObject_ThisCall(DeeClass_SUFFIX(Dee_TYPE(self))->cs_cast.ctp_str,(DeeObject *)self,Dee_EmptyTuple);
#else
 result = DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(DeeClass_SUFFIX(Dee_TYPE(self)),DeeType_SLOT_ID(tp_str)),(DeeObject *)self,Dee_EmptyTuple);
#endif
 if DEE_UNLIKELY(result && !DeeString_Check(result)) {
  DeeError_SetStringf(&DeeErrorType_TypeError,
                      "%#q.operator __str__() returned an instance of %q instead of a string",
                      DeeType_NAME(Dee_TYPE(self)),DeeType_NAME(Dee_TYPE(result)));
  Dee_DECREF(result);
  return NULL;
 }
 return result;
}

// Slot: tp_repr
static DeeObject * _deeinstance_tp_repr(DeeInstanceObject *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERTF(DeeObject_Check(DeeClass_SUFFIX(Dee_TYPE(self))->cs_cast.ctp_repr),"Missing slot: tp_repr");
 result = DeeObject_ThisCall(DeeClass_SUFFIX(Dee_TYPE(self))->cs_cast.ctp_repr,(DeeObject *)self,Dee_EmptyTuple);
#else
 result = DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(DeeClass_SUFFIX(Dee_TYPE(self)),DeeType_SLOT_ID(tp_repr)),(DeeObject *)self,Dee_EmptyTuple);
#endif
 if DEE_UNLIKELY(result && !DeeString_Check(result)) {
  DeeError_SetStringf(&DeeErrorType_TypeError,
                      "%#q.operator __repr__() returned an instance of %q instead of a string",
                      DeeType_NAME(Dee_TYPE(self)),DeeType_NAME(Dee_TYPE(result)));
  Dee_DECREF(result);
  return NULL;
 }
 return result;
}


static int _deeinstance_tp_hash(
 DeeInstanceObject *self, Dee_hash_t start, Dee_hash_t *result) {
 DeeObject *args,*temp,*start_ob,*callback; Dee_hash_t newhash;
 DeeTypeObject *tp_iter; struct DeeInstanceLevel *level; int error;
 DeeType_SLOT_TYPE(tp_hash) func;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 tp_iter = Dee_TYPE(self); do {
  func = DeeType_GET_SLOT(tp_iter,tp_hash);
  if (func == (DeeType_SLOT_TYPE(tp_hash))&_deeinstance_tp_hash) {
   // Class implementation of hashing >> Try to call a user callback or simple hash variables
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
   if DEE_LIKELY((callback = DeeClass_SUFFIX(tp_iter)->cs_math.ctp_hash) != NULL)
#else
   if DEE_LIKELY((callback = DeeClassSuffix_GetVirtOperator(
    DeeClass_SUFFIX(tp_iter),DeeType_SLOT_ID(tp_hash))) != NULL)
#endif
   {
    DEE_ASSERT(DeeObject_Check(callback) && "Invalid slot: tp_hash");
    if ((start_ob = DeeObject_New(Dee_hash_t,start)) == NULL) return -1;
    args = DeeTuple_Pack(1,start_ob);
    Dee_DECREF(start_ob);
    if DEE_UNLIKELY(!args) return -1;
    temp = DeeObject_ThisCall(callback,(DeeObject *)self,args);
    Dee_DECREF(args);
    if DEE_UNLIKELY(!temp) return -1;
    error = DeeObject_Cast(Dee_hash_t,temp,&start);
    Dee_DECREF(temp);
    if DEE_UNLIKELY(error != 0) return -1;
   } else {
    level = DeeInstance_MEMBERS_EX(tp_iter,self);
    if DEE_UNLIKELY((error = DeeHashMap_HashValuesWithLock(
     &level->il_members,start,&newhash,&level->il_lock)
     ) != 0) return error;
    start = newhash;
   }
  } else {
   if DEE_UNLIKELY((error = (*func)((DeeObject *)self,start,&newhash)) != 0) return error;
   start = newhash;
  }
 } while ((tp_iter = DeeType_BASE(tp_iter)) != NULL);
 *result = start;
 return 0;
}

static int _deeinstance_tp_int32(DeeInstanceObject *self, Dee_int32_t *result) {
 DeeObject *temp; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(DeeClass_SUFFIX(Dee_TYPE(self))->cs_cast.ctp_int32) && "Missing slot: tp_int32");
 temp = DeeObject_ThisCall(
  DeeClass_SUFFIX(Dee_TYPE(self))->cs_cast.ctp_int32,
  (DeeObject *)self,Dee_EmptyTuple);
#else
 temp = DeeObject_ThisCall(
  DeeClassSuffix_GetKnownVirtOperator(DeeClass_SUFFIX(Dee_TYPE(self)),
  DeeType_SLOT_ID(tp_int32)),(DeeObject *)self,Dee_EmptyTuple);
#endif
 if DEE_UNLIKELY(!temp) return -1;
 error = DeeObject_Cast(Dee_int32_t,temp,result);
 Dee_DECREF(temp);
 return error;
}
static int _deeinstance_tp_int64(DeeInstanceObject *self, Dee_int64_t *result) {
 DeeObject *temp; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(DeeClass_SUFFIX(Dee_TYPE(self))->
  cs_cast.ctp_int64) && "Missing slot: tp_int64");
 temp = DeeObject_ThisCall(
  DeeClass_SUFFIX(Dee_TYPE(self))->cs_cast.ctp_int64,
  (DeeObject *)self,Dee_EmptyTuple);
#else
 temp = DeeObject_ThisCall(
  DeeClassSuffix_GetKnownVirtOperator(DeeClass_SUFFIX(Dee_TYPE(self)),
  DeeType_SLOT_ID(tp_int64)),(DeeObject *)self,Dee_EmptyTuple);
#endif
 if DEE_UNLIKELY(!temp) return -1;
 error = DeeObject_Cast(Dee_int64_t,temp,result);
 Dee_DECREF(temp);
 return error;
}

// Slot: tp_double
static int _deeinstance_tp_double(DeeInstanceObject *self, double *result) {
 DeeObject *temp; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(DeeClass_SUFFIX(Dee_TYPE(self))->
  cs_cast.ctp_double) && "Missing slot: tp_double");
#endif
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 temp = DeeObject_ThisCall(
  DeeClass_SUFFIX(Dee_TYPE(self))->cs_cast.ctp_double,
  (DeeObject *)self,Dee_EmptyTuple);
#else
 temp = DeeObject_ThisCall(
  DeeClassSuffix_GetKnownVirtOperator(DeeClass_SUFFIX(Dee_TYPE(self)),
  DeeType_SLOT_ID(tp_double)),(DeeObject *)self,Dee_EmptyTuple);
#endif
 if DEE_UNLIKELY(!temp) return -1;
 error = DeeObject_Cast(double,temp,result);
 Dee_DECREF(temp);
 return error;
}




DEE_A_RET_EXCEPT_REF DeeObject *DeeInstance_GetAttr(
 DEE_A_INOUT_OBJECT(DeeInstanceObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DEE_ASSERT(DeeObject_Check(self));
 return DeeInstance_TGetAttr(Dee_TYPE(self),self,attr);
}
DEE_A_RET_EXCEPT(-1) int DeeInstance_DelAttr(
 DEE_A_INOUT_OBJECT(DeeInstanceObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DEE_ASSERT(DeeObject_Check(self));
 return DeeInstance_TDelAttr(Dee_TYPE(self),self,attr);
}
DEE_A_RET_EXCEPT(-1) int DeeInstance_SetAttr(
 DEE_A_INOUT_OBJECT(DeeInstanceObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr,
 DEE_A_INOUT DeeObject *v) {
 DEE_ASSERT(DeeObject_Check(self));
 return DeeInstance_TSetAttr(Dee_TYPE(self),self,attr,v);
}



#define DeeClass_EXEC_BASE_COPY_CTOR(base,self,right)\
 (*DeeType_GET_SLOT(base,tp_copy_ctor))(base,(DeeObject *)(self),(DeeObject *)(right))
#define DeeClass_EXEC_BASE_MOVE_CTOR(base,self,right)\
 (*DeeType_GET_SLOT(base,tp_move_ctor))(base,(DeeObject *)(self),(DeeObject *)(right))
#if 1
#define DeeClass_EXEC_BASE_CTOR(base,self)\
 (*DeeType_GET_SLOT(base,tp_ctor))(base,(DeeObject *)(self))
#define DeeClass_EXEC_BASE_ANY_CTOR(base,self,args)\
 (*DeeType_GET_SLOT(base,tp_any_ctor))(base,(DeeObject *)(self),args)
#else
#define DeeClass_SHOULD_EXEC_BASE_CONSTRUCTOR(base)\
 ((base)!=&DeeObject_Type)
#define DeeClass_EXEC_BASE_CTOR(base,self)\
 (DeeClass_SHOULD_EXEC_BASE_CONSTRUCTOR(base)\
  ? (*DeeType_GET_SLOT(base,tp_ctor))(base,(DeeObject *)(self)) : 0)
#define DeeClass_EXEC_BASE_ANY_CTOR(base,self,args)\
 (DeeClass_SHOULD_EXEC_BASE_CONSTRUCTOR(base)\
  ? (*DeeType_GET_SLOT(base,tp_any_ctor))(base,(DeeObject *)(self),args) : 0)
#endif

#define DeeClass_IsValidBase(class_base) ((class_base)!=&DeeObject_Type)

#define DeeClass_GetNewInstance_tp_ctor_builtin(class_base)          (DeeClass_IsValidBase(class_base) ? &_deeinstance_tp_ctor_builtin          : &_deeinstance_tp_ctor_builtin_nobase)
#define DeeClass_GetNewInstance_tp_ctor_default(class_base)          (DeeClass_IsValidBase(class_base) ? &_deeinstance_tp_ctor_default          : &_deeinstance_tp_ctor_default_nobase)
#define DeeClass_GetNewInstance_tp_ctor_callany(class_base)          (DeeClass_IsValidBase(class_base) ? &_deeinstance_tp_ctor_callany          : &_deeinstance_tp_ctor_callany_nobase)
#define DeeClass_GetNewInstance_tp_any_ctor_builtin(class_base)      (DeeClass_IsValidBase(class_base) ? &_deeinstance_tp_any_ctor_builtin      : &_deeinstance_tp_any_ctor_builtin_nobase)
#define DeeClass_GetNewInstance_tp_any_ctor_default(class_base)      (DeeClass_IsValidBase(class_base) ? &_deeinstance_tp_any_ctor_default      : &_deeinstance_tp_any_ctor_default_nobase)
#define DeeClass_GetNewInstance_tp_any_ctor_callctor(class_base)     (DeeClass_IsValidBase(class_base) ? &_deeinstance_tp_any_ctor_callctor     : &_deeinstance_tp_any_ctor_callctor_nobase)
#define DeeClass_GetNewInstance_tp_any_ctor_callcopy(class_base)     (DeeClass_IsValidBase(class_base) ? &_deeinstance_tp_any_ctor_callcopy     : &_deeinstance_tp_any_ctor_callcopy_nobase)
#define DeeClass_GetNewInstance_tp_any_ctor_callctorcopy(class_base) (DeeClass_IsValidBase(class_base) ? &_deeinstance_tp_any_ctor_callctorcopy : &_deeinstance_tp_any_ctor_callctorcopy_nobase)

//////////////////////////////////////////////////////////////////////////
// Instance VTable
static int _deeinstance_tp_ctor_builtin(
 DeeClassObject *tp, DeeInstanceObject *self) {
 struct DeeInstanceLevel *level; DeeTypeObject *base;
 level = DeeInstance_MEMBERS_EX(tp,self);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 if DEE_UNLIKELY(DeeClass_EXEC_BASE_CTOR(base,self) != 0) {
  level->il_flags |= DEE_INSTANCELEVEL_FLAG_NODTOR;
  DeeHashMap_Quit(&level->il_members);
  return -1;
 }
 return 0;
}
static int _deeinstance_tp_ctor_builtin_nobase(
 DeeClassObject *tp, DeeInstanceObject *self) {
 struct DeeInstanceLevel *level;
 level = DeeInstance_MEMBERS_EX(tp,self);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 return 0;
}
static int _deeinstance_tp_ctor_builtin_superargs(
 DeeClassObject *tp, DeeInstanceObject *self) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeObject *superargs; int error; DeeTypeObject *base;
 suffix = DeeClass_SUFFIX(tp);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_ctor_superargs));
 if DEE_UNLIKELY((superargs = DeeObject_Call(
  suffix->cs_constructor.ctp_ctor_superargs,Dee_EmptyTuple)) == NULL) return -1;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY((superargs = DeeObject_Call(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DEE_CLASS_SLOTID_SUPERARGS_CTOR),Dee_EmptyTuple)) == NULL) return -1;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 error = DeeTuple_CheckEmpty(superargs)
  ? DeeClass_EXEC_BASE_CTOR(base,self)
  : DeeClass_EXEC_BASE_ANY_CTOR(base,self,superargs);
 Dee_DECREF(superargs);
 if DEE_UNLIKELY(error != 0) {
  level->il_flags |= DEE_INSTANCELEVEL_FLAG_NODTOR;
  DeeHashMap_Quit(&level->il_members);
  return error;
 }
 return 0;
}
static int _deeinstance_tp_ctor_builtin_anysuperargs(
 DeeClassObject *tp, DeeInstanceObject *self) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeObject *superargs; int error; DeeTypeObject *base;
 suffix = DeeClass_SUFFIX(tp);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_any_ctor_superargs));
 if DEE_UNLIKELY((superargs = DeeObject_Call(
  suffix->cs_constructor.ctp_any_ctor_superargs,Dee_EmptyTuple)) == NULL) return -1;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY((superargs = DeeObject_Call(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DEE_CLASS_SLOTID_SUPERARGS_ANY_CTOR),Dee_EmptyTuple)) == NULL) return -1;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 error = DeeTuple_CheckEmpty(superargs)
  ? DeeClass_EXEC_BASE_CTOR(base,self)
  : DeeClass_EXEC_BASE_ANY_CTOR(base,self,superargs);
 Dee_DECREF(superargs);
 if DEE_UNLIKELY(error != 0) {
  level->il_flags |= DEE_INSTANCELEVEL_FLAG_NODTOR;
  DeeHashMap_Quit(&level->il_members);
  return error;
 }
 return 0;
}
static int _deeinstance_tp_copy_ctor_builtin(
 DeeClassObject *tp, DeeInstanceObject *self, DeeInstanceObject *right) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *my_level,*rhs_level;
 DeeTypeObject *base;
 suffix = DeeClass_SUFFIX(tp);
 my_level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 rhs_level = DeeInstance_MEMBERS_AT(right,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(my_level);
 if (DeeHashMap_InitDeepCopyWithLock(&my_level->il_members,
  &rhs_level->il_members,&rhs_level->il_lock) != 0) return -1;
 base = DeeType_BASE(tp);
 if DEE_UNLIKELY(DeeClass_EXEC_BASE_COPY_CTOR(base,self,right) != 0) {
  my_level->il_flags |= DEE_INSTANCELEVEL_FLAG_NODTOR;
  DeeHashMap_Quit(&my_level->il_members);
  return -1;
 }
 return 0;
}
static int _deeinstance_tp_copy_ctor_builtin_anysuperargs(
 DeeClassObject *tp, DeeInstanceObject *self, DeeInstanceObject *right) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *my_level,*rhs_level;
 DeeTypeObject *base; DeeObject *args,*superargs; int error;
 suffix = DeeClass_SUFFIX(tp);
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,right)) == NULL) return -1;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_any_ctor_superargs));
 superargs = DeeObject_Call(suffix->cs_constructor.ctp_any_ctor_superargs,args);
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 superargs = DeeObject_Call(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DEE_CLASS_SLOTID_SUPERARGS_ANY_CTOR),args);
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 Dee_DECREF(args);
 if DEE_UNLIKELY(!superargs) return -1;
 my_level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 rhs_level = DeeInstance_MEMBERS_AT(right,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(my_level);
 if (DeeHashMap_InitDeepCopyWithLock(&my_level->il_members,
  &rhs_level->il_members,&rhs_level->il_lock) != 0) { Dee_DECREF(superargs); return -1; }
 base = DeeType_BASE(tp);
 error = DeeTuple_CheckEmpty(superargs)
  ? DeeClass_EXEC_BASE_CTOR(base,self)
  : DeeClass_EXEC_BASE_ANY_CTOR(base,self,superargs);
 Dee_DECREF(superargs);
 if DEE_UNLIKELY(error != 0) {
  my_level->il_flags |= DEE_INSTANCELEVEL_FLAG_NODTOR;
  DeeHashMap_Quit(&my_level->il_members);
  return -1;
 }
 return 0;
}
#define _deeinstance_tp_move_ctor_builtin_anysuperargs \
        _deeinstance_tp_copy_ctor_builtin_anysuperargs
static int _deeinstance_tp_move_ctor_builtin(
 DeeClassObject *tp, DeeInstanceObject *self, DeeInstanceObject *right) {
 struct DeeInstanceLevel *my_level,*rhs_level;
 DeeTypeObject *base = DeeType_BASE(tp);
 my_level = DeeInstance_MEMBERS_EX(tp,self);
 rhs_level = DeeInstance_MEMBERS_EX(tp,right);
 _DeeInstanceLevel_Init(my_level);
 DeeAtomicMutex_Acquire(&rhs_level->il_lock);
 DeeHashMap_InitMove(&my_level->il_members,&rhs_level->il_members);
 DeeAtomicMutex_Release(&rhs_level->il_lock);
 if DEE_UNLIKELY(DeeClass_EXEC_BASE_MOVE_CTOR(base,self,right) != 0) {
  my_level->il_flags |= DEE_INSTANCELEVEL_FLAG_NODTOR;
  DeeHashMap_Quit(&my_level->il_members);
  return -1;
 }
 return 0;
}
static int _deeinstance_tp_any_ctor_builtin(
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *args) {
 struct DeeInstanceLevel *level; DeeTypeObject *base;
 level = DeeInstance_MEMBERS_EX(tp,self);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 if DEE_UNLIKELY(DeeClass_EXEC_BASE_ANY_CTOR(base,self,args) != 0) {
  level->il_flags |= DEE_INSTANCELEVEL_FLAG_NODTOR;
  DeeHashMap_Quit(&level->il_members);
  return -1;
 }
 return 0;
}
#if 1
static int _deeinstance_tp_any_ctor_builtin_nobase(
 DeeClassObject *tp, DeeInstanceObject *DEE_UNUSED(self), DeeObject *args) {
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %#q%K",
                     DeeType_NAME(tp),
                     DeeTuple_Types(args));
 return -1;
}
#else
static int _deeinstance_tp_any_ctor_builtin_nobase(
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *DEE_UNUSED(args)) {
 struct DeeInstanceLevel *level;
 level = DeeInstance_MEMBERS_EX(tp,self);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 return 0;
}
#endif
static int _deeinstance_tp_any_ctor_builtin_superargs(
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *args) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeTypeObject *base; int error; DeeObject *superargs;
 if DEE_UNLIKELY(!DeeTuple_CheckEmpty(args)) {
  if DEE_LIKELY(DeeType_NAME(tp))
   DeeError_TypeError_ArgCountExpectedNamed(DeeType_NAME(tp),0,DeeTuple_SIZE(args));
  else DeeError_TypeError_ArgCountExpected(0,DeeTuple_SIZE(args));
  return -1;
 }
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_ctor_superargs));
 if DEE_UNLIKELY((superargs = DeeObject_Call(
  suffix->cs_constructor.ctp_ctor_superargs,Dee_EmptyTuple)) == NULL) return -1;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY((superargs = DeeObject_Call(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DEE_CLASS_SLOTID_SUPERARGS_CTOR),Dee_EmptyTuple)) == NULL) return -1;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 error = DeeTuple_CheckEmpty(superargs)
  ? DeeClass_EXEC_BASE_CTOR(base,self)
  : DeeClass_EXEC_BASE_ANY_CTOR(base,self,superargs);
 Dee_DECREF(superargs);
 if DEE_UNLIKELY(error != 0) {
  level->il_flags |= DEE_INSTANCELEVEL_FLAG_NODTOR;
  DeeHashMap_Quit(&level->il_members);
  return error;
 }
 return 0;
}
static int _deeinstance_tp_any_ctor_builtin_anysuperargs( // (superargs from any)
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *args) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeTypeObject *base; int error; DeeObject *superargs;
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_any_ctor_superargs));
 if DEE_UNLIKELY((superargs = DeeObject_Call(
  suffix->cs_constructor.ctp_any_ctor_superargs,args)) == NULL) return -1;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY((superargs = DeeObject_Call(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DEE_CLASS_SLOTID_SUPERARGS_ANY_CTOR),args)) == NULL) return -1;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 error = DeeTuple_CheckEmpty(superargs)
  ? DeeClass_EXEC_BASE_CTOR(base,self)
  : DeeClass_EXEC_BASE_ANY_CTOR(base,self,superargs);
 Dee_DECREF(superargs);
 if DEE_UNLIKELY(error != 0) {
  level->il_flags |= DEE_INSTANCELEVEL_FLAG_NODTOR;
  DeeHashMap_Quit(&level->il_members);
  return error;
 }
 return 0;
}
static int _deeinstance_tp_any_ctor_builtin_superargsanysuperargs( // (superargs from any/any)
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *args) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeTypeObject *base; int error; DeeObject *superargs;
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_ctor_superargs));
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_any_ctor_superargs));
 if DEE_UNLIKELY((superargs = DeeObject_Call(DeeTuple_CheckEmpty(args)
  ? suffix->cs_constructor.ctp_ctor_superargs
  : suffix->cs_constructor.ctp_any_ctor_superargs,args)) == NULL) return -1;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY((superargs = DeeObject_Call(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeTuple_CheckEmpty(args) ? DEE_CLASS_SLOTID_SUPERARGS_CTOR
  : DEE_CLASS_SLOTID_SUPERARGS_ANY_CTOR),args)) == NULL) return -1;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 error = DeeTuple_CheckEmpty(superargs)
  ? DeeClass_EXEC_BASE_CTOR(base,self)
  : DeeClass_EXEC_BASE_ANY_CTOR(base,self,superargs);
 Dee_DECREF(superargs);
 if DEE_UNLIKELY(error != 0) {
  level->il_flags |= DEE_INSTANCELEVEL_FLAG_NODTOR;
  DeeHashMap_Quit(&level->il_members);
  return error;
 }
 return 0;
}
static void _deeinstance_tp_dtor(DeeInstanceObject *self) {
 DeeHashMap_Quit(&DeeInstance_MEMBERS(self)->il_members);
}
static void _deeinstance_tp_clear_builtin(DeeInstanceObject *self) {
 struct DeeInstanceLevel *level;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 level = DeeInstance_MEMBERS(self);
 DeeHashMap_ClearWithLock(&level->il_members,&level->il_lock);
}
static void _deeinstance_tp_clear_default(DeeInstanceObject *self) {
 struct DeeInstanceLevel *level;
 struct DeeClassSuffix *suffix; DeeObject *result;
 DeeThreadObject *thread_self = DeeThread_SELF();
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 DEE_LVERBOSE1("HERE: %q\n",DeeType_NAME(Dee_TYPE(self)));
 if DEE_UNLIKELY(!thread_self) return;
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
 level = DeeInstance_MEMBERS(self);
 if DEE_LIKELY((level->il_flags&DEE_INSTANCELEVEL_FLAG_NODTOR)==0) {
  DEE_LVERBOSE3("Executing user class destructor for %q\n",
                DeeType_NAME(Dee_TYPE(self)));
  DeeThread_PUSH_EXCEPTIONS(thread_self);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
  result = DeeObject_ThisCall(suffix->cs_destructor.ctp_dtor,(DeeObject *)self,Dee_EmptyTuple);
#else
  result = DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
   suffix,DeeType_SLOT_ID(tp_dtor)),(DeeObject *)self,Dee_EmptyTuple);
#endif
  if DEE_UNLIKELY(!result) {
   while (!DeeError_Print("[Ignored] Unhandled exception in class_instance destructor\n",1));
  } else Dee_DECREF(result);
  DeeThread_POP_EXCEPTIONS_NO_NEW(thread_self);
 } else {
  DEE_LVERBOSE2("Skipping user destructor in %q because constructor failed\n",
                DeeType_NAME(Dee_TYPE(self)));
 }
 DeeHashMap_ClearWithLock(&level->il_members,&level->il_lock);
}
DEE_VISIT_PROC_EX(_deeinstance_tp_visit,DeeClassObject *tp_self,DeeInstanceObject *self) {
 DeeHashMap_TRAVERSE_VARS; struct _DeeHashMapNode *node;
 struct DeeInstanceLevel *level;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 level = DeeInstance_MEMBERS_EX(tp_self,self);
 DeeAtomicMutex_AcquireRelaxed(&level->il_lock);
 DeeHashMap_TRAVERSE(node,&level->il_members) {
  Dee_VISIT(_DeeHashMapNode_KEY(node));
  Dee_VISIT(_DeeHashMapNode_ITEM(node));
 }
 DeeAtomicMutex_Release(&level->il_lock);
}

#if 1
#define DeeInstance_CALL_CTOR(result,ctor_call) ((result) = (ctor_call))
#else
static void _deeinstance_constructor_failed_but_object_used(void) {
 DeeError_Print("[ignored] Class instance constructor caused an exception after "
                "the object has already started being used elsewhere.",1);
}
#define DeeInstance_CALL_CTOR(result,ctor_call)\
do{\
 unsigned int refcnt_before;\
 refcnt_before = _DeeRefCounter_Fetch((self)->__ob_refcnt);\
 (result) = (ctor_call);\
 if (!(result) && refcnt_before < _DeeRefCounter_Fetch((self)->__ob_refcnt)) {\
  _deeinstance_constructor_failed_but_object_used();\
  (result) = DeeNone_New();\
 }\
}while(0)
#endif

static int _DeeInstance_HandleConstructorResult(
 struct DeeInstanceLevel *level, DeeTypeObject *base,
 DeeInstanceObject *self, DeeObject *result) {
 int temp;
 if DEE_UNLIKELY(!result) {
  if DEE_LIKELY(DeeObject_IS_UNIQUE(self)) {
   temp = _DeeObject_DestructChain(base,(DeeObject *)self);
   DeeHashMap_Quit(&level->il_members);
   return temp;
  } else {
   while (!DeeError_Print("[ignored] Error occurred during construction of externally used object\n",1));
   level->il_flags |= DEE_INSTANCELEVEL_FLAG_NODTOR;
   DeeHashMap_Clear(&level->il_members);
   return 0;
  }
 }
 Dee_DECREF(result);
 return 0;
}
static int _DeeInstance_HandleConstructorResultNoBase(
 struct DeeInstanceLevel *level, DeeInstanceObject *self, DeeObject *result) {
 if DEE_UNLIKELY(!result) {
  if DEE_LIKELY(DeeObject_IS_UNIQUE(self)) {
   DeeHashMap_Quit(&level->il_members);
   return -1;
  } else {
   while (!DeeError_Print("[ignored] Error occurred during construction of externally used object\n",1));
   level->il_flags |= DEE_INSTANCELEVEL_FLAG_NODTOR;
   DeeHashMap_Clear(&level->il_members);
   return 0;
  }
 }
 Dee_DECREF(result);
 return 0;
}

static int _deeinstance_tp_ctor_default(
 DeeClassObject *tp, DeeInstanceObject *self) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeTypeObject *base; DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_EX(tp,self);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 if DEE_UNLIKELY(DeeClass_EXEC_BASE_CTOR(base,self) != 0) {
  DeeHashMap_Quit(&level->il_members);
  return -1;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_ctor,(DeeObject *)self,Dee_EmptyTuple));
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_ctor)),(DeeObject *)self,Dee_EmptyTuple));
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_ctor_default_nobase(
 DeeClassObject *tp, DeeInstanceObject *self) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_EX(tp,self);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_ctor,(DeeObject *)self,Dee_EmptyTuple));
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_ctor)),(DeeObject *)self,Dee_EmptyTuple));
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 return _DeeInstance_HandleConstructorResultNoBase(level,self,result);
}
static int _deeinstance_tp_ctor_default_superargs(
 DeeClassObject *tp, DeeInstanceObject *self) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeObject *result,*superargs; DeeTypeObject *base; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(tp);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_ctor_superargs));
 if DEE_UNLIKELY((superargs = DeeObject_Call(
  suffix->cs_constructor.ctp_ctor_superargs,Dee_EmptyTuple)) == NULL) return -1;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY((superargs = DeeObject_Call(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DEE_CLASS_SLOTID_SUPERARGS_CTOR),Dee_EmptyTuple)) == NULL) return -1;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 error = DeeTuple_CheckEmpty(superargs)
  ? DeeClass_EXEC_BASE_CTOR(base,self)
  : DeeClass_EXEC_BASE_ANY_CTOR(base,self,superargs);
 Dee_DECREF(superargs);
 if DEE_UNLIKELY(error != 0) {
  DeeHashMap_Quit(&level->il_members);
  return error;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_ctor,(DeeObject *)self,Dee_EmptyTuple));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_ctor)),(DeeObject *)self,Dee_EmptyTuple));
#endif
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_ctor_default_anysuperargs(
 DeeClassObject *tp, DeeInstanceObject *self) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeObject *result,*superargs; DeeTypeObject *base; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(tp);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_any_ctor_superargs));
 if DEE_UNLIKELY((superargs = DeeObject_Call(
  suffix->cs_constructor.ctp_any_ctor_superargs,Dee_EmptyTuple)) == NULL) return -1;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY((superargs = DeeObject_Call(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DEE_CLASS_SLOTID_SUPERARGS_ANY_CTOR),Dee_EmptyTuple)) == NULL) return -1;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 error = DeeTuple_CheckEmpty(superargs)
  ? DeeClass_EXEC_BASE_CTOR(base,self)
  : DeeClass_EXEC_BASE_ANY_CTOR(base,self,superargs);
 Dee_DECREF(superargs);
 if DEE_UNLIKELY(error != 0) {
  DeeHashMap_Quit(&level->il_members);
  return error;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_ctor,(DeeObject *)self,Dee_EmptyTuple));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_ctor)),(DeeObject *)self,Dee_EmptyTuple));
#endif
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_ctor_callany(
 DeeClassObject *tp, DeeInstanceObject *self) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeTypeObject *base; DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 if DEE_UNLIKELY(DeeClass_EXEC_BASE_CTOR(base,self) != 0) {
  DeeHashMap_Quit(&level->il_members);
  return -1;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_any_ctor,(DeeObject *)self,Dee_EmptyTuple));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_any_ctor)),(DeeObject *)self,Dee_EmptyTuple));
#endif
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_ctor_callany_nobase(
 DeeClassObject *tp, DeeInstanceObject *self) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_any_ctor,(DeeObject *)self,Dee_EmptyTuple));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_any_ctor)),(DeeObject *)self,Dee_EmptyTuple));
#endif
 return _DeeInstance_HandleConstructorResultNoBase(level,self,result);
}
static int _deeinstance_tp_ctor_callany_superargs(
 DeeClassObject *tp, DeeInstanceObject *self) {
 DeeTypeObject *base = DeeType_BASE(tp);
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeObject *result,*superargs; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(tp);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_ctor_superargs));
 if DEE_UNLIKELY((superargs = DeeObject_Call(
  suffix->cs_constructor.ctp_ctor_superargs,Dee_EmptyTuple)) == NULL) return -1;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY((superargs = DeeObject_Call(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DEE_CLASS_SLOTID_SUPERARGS_CTOR),Dee_EmptyTuple)) == NULL) return -1;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 error = DeeTuple_CheckEmpty(superargs)
  ? DeeClass_EXEC_BASE_CTOR(base,self)
  : DeeClass_EXEC_BASE_ANY_CTOR(base,self,superargs);
 Dee_DECREF(superargs);
 if DEE_UNLIKELY(error != 0) {
  DeeHashMap_Quit(&level->il_members);
  return error;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_any_ctor,(DeeObject *)self,Dee_EmptyTuple));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_any_ctor)),(DeeObject *)self,Dee_EmptyTuple));
#endif
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_ctor_callany_anysuperargs(
 DeeClassObject *tp, DeeInstanceObject *self) {
 DeeTypeObject *base = DeeType_BASE(tp);
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeObject *result,*superargs; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(tp);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_any_ctor_superargs));
 if DEE_UNLIKELY((superargs = DeeObject_Call(
  suffix->cs_constructor.ctp_any_ctor_superargs,Dee_EmptyTuple)) == NULL) return -1;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY((superargs = DeeObject_Call(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DEE_CLASS_SLOTID_SUPERARGS_ANY_CTOR),Dee_EmptyTuple)) == NULL) return -1;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 error = DeeTuple_CheckEmpty(superargs)
  ? DeeClass_EXEC_BASE_CTOR(base,self)
  : DeeClass_EXEC_BASE_ANY_CTOR(base,self,superargs);
 Dee_DECREF(superargs);
 if DEE_UNLIKELY(error != 0) {
  DeeHashMap_Quit(&level->il_members);
  return error;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_any_ctor,(DeeObject *)self,Dee_EmptyTuple));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_any_ctor)),(DeeObject *)self,Dee_EmptyTuple));
#endif
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_copy_ctor_default(
 DeeClassObject *tp, DeeInstanceObject *self, DeeInstanceObject *right) {
 struct DeeInstanceLevel *level;
 DeeTypeObject *base = DeeType_BASE(tp);
 struct DeeClassSuffix *suffix; DeeObject *result,*args;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,right)) == NULL) return -1;
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 if DEE_UNLIKELY(DeeClass_EXEC_BASE_COPY_CTOR(base,self,right) != 0) {
  DeeHashMap_Quit(&level->il_members);
  Dee_DECREF(args);
  return -1;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_copy_ctor,(DeeObject *)self,args));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_copy_ctor)),(DeeObject *)self,args));
#endif
 Dee_DECREF(args);
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_copy_ctor_callany(
 DeeClassObject *tp, DeeInstanceObject *self, DeeInstanceObject *right) {
 struct DeeInstanceLevel *level;
 DeeTypeObject *base = DeeType_BASE(tp);
 struct DeeClassSuffix *suffix; DeeObject *result,*args;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,right)) == NULL) return -1;
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 if DEE_UNLIKELY(DeeClass_EXEC_BASE_COPY_CTOR(base,self,right) != 0) {
  DeeHashMap_Quit(&level->il_members);
  Dee_DECREF(args);
  return -1;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_any_ctor,(DeeObject *)self,args));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_any_ctor)),(DeeObject *)self,args));
#endif
 Dee_DECREF(args);
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_copy_ctor_callany_anysuperargs(
 DeeClassObject *tp, DeeInstanceObject *self, DeeInstanceObject *right) {
 struct DeeInstanceLevel *level; DeeTypeObject *base; int error;
 struct DeeClassSuffix *suffix; DeeObject *result,*args,*superargs;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,right)) == NULL) return -1;
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_any_ctor_superargs));
 if DEE_UNLIKELY((superargs = DeeObject_Call(
  suffix->cs_constructor.ctp_any_ctor_superargs,args)) == NULL) goto err_args;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY((superargs = DeeObject_Call(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DEE_CLASS_SLOTID_SUPERARGS_ANY_CTOR),args)) == NULL) goto err_args;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 error = DeeTuple_CheckEmpty(superargs)
  ? DeeClass_EXEC_BASE_CTOR(base,self)
  : DeeClass_EXEC_BASE_ANY_CTOR(base,self,superargs);
 Dee_DECREF(superargs);
 if DEE_UNLIKELY(error != 0) {
/*err_members:*/ DeeHashMap_Quit(&level->il_members);
err_args: Dee_DECREF(args);
  return -1;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_any_ctor,(DeeObject *)self,args));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_any_ctor)),(DeeObject *)self,args));
#endif
 Dee_DECREF(args);
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_move_ctor_default(
 DeeClassObject *tp, DeeInstanceObject *self, DeeInstanceObject *right) {
 struct DeeInstanceLevel *level;
 DeeTypeObject *base = DeeType_BASE(tp);
 struct DeeClassSuffix *suffix; DeeObject *result,*args;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,right)) == NULL) return -1;
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 if DEE_UNLIKELY(DeeClass_EXEC_BASE_MOVE_CTOR(base,self,right) != 0) {
  DeeHashMap_Quit(&level->il_members);
  Dee_DECREF(args);
  return -1;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_move_ctor,(DeeObject *)self,args));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_move_ctor)),(DeeObject *)self,args));
#endif
 Dee_DECREF(args);
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_move_ctor_callany(
 DeeClassObject *tp, DeeInstanceObject *self, DeeInstanceObject *right) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeTypeObject *base; DeeObject *result,*args;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,right)) == NULL) return -1;
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 if DEE_UNLIKELY(DeeClass_EXEC_BASE_MOVE_CTOR(base,self,right) != 0) {
  DeeHashMap_Quit(&level->il_members);
  Dee_DECREF(args);
  return -1;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_any_ctor,(DeeObject *)self,args));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_any_ctor)),(DeeObject *)self,args));
#endif
 Dee_DECREF(args);
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_move_ctor_callany_anysuperargs(
 DeeClassObject *tp, DeeInstanceObject *self, DeeInstanceObject *right) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeTypeObject *base; DeeObject *result,*args,*superargs; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,right)) == NULL) return -1;
 suffix = DeeClass_SUFFIX(tp);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_any_ctor_superargs));
 if DEE_UNLIKELY((superargs = DeeObject_Call(
  suffix->cs_constructor.ctp_any_ctor_superargs,args)) == NULL) goto err_args;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY((superargs = DeeObject_Call(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DEE_CLASS_SLOTID_SUPERARGS_ANY_CTOR),args)) == NULL) goto err_args;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 error = DeeTuple_CheckEmpty(superargs)
  ? DeeClass_EXEC_BASE_CTOR(base,self)
  : DeeClass_EXEC_BASE_ANY_CTOR(base,self,superargs);
 Dee_DECREF(superargs);
 if DEE_UNLIKELY(error != 0) {
/*err_members:*/ DeeHashMap_Quit(&level->il_members);
err_args: Dee_DECREF(args);
  return -1;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_any_ctor,(DeeObject *)self,args));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_any_ctor)),(DeeObject *)self,args));
#endif
 Dee_DECREF(args);
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_move_ctor_callcopy(
 DeeClassObject *tp, DeeInstanceObject *self, DeeInstanceObject *right) {
 struct DeeInstanceLevel *level;
 DeeTypeObject *base = DeeType_BASE(tp);
 struct DeeClassSuffix *suffix; DeeObject *result,*args;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,right)) == NULL) return -1;
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 if DEE_UNLIKELY(DeeClass_EXEC_BASE_MOVE_CTOR(base,self,right) != 0) {
  DeeHashMap_Quit(&level->il_members);
  Dee_DECREF(args);
  return -1;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(suffix->cs_constructor.ctp_copy_ctor,(DeeObject *)self,args));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_copy_ctor)),(DeeObject *)self,args));
#endif
 Dee_DECREF(args);
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_any_ctor_default(
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *args) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeTypeObject *base; DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 if DEE_UNLIKELY(DeeClass_EXEC_BASE_ANY_CTOR(base,self,args) != 0) {
  DeeHashMap_Quit(&level->il_members);
  return -1;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(suffix->cs_constructor.ctp_any_ctor,(DeeObject *)self,args));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_any_ctor)),(DeeObject *)self,args));
#endif
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_any_ctor_default_nobase(
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *args) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(suffix->cs_constructor.ctp_any_ctor,(DeeObject *)self,args));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_any_ctor)),(DeeObject *)self,args));
#endif
 return _DeeInstance_HandleConstructorResultNoBase(level,self,result);
}
static int _deeinstance_tp_any_ctor_default_anysuperargs(
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *args) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeTypeObject *base; DeeObject *result,*superargs; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(tp);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_any_ctor_superargs));
 if DEE_UNLIKELY((superargs = DeeObject_Call(
  suffix->cs_constructor.ctp_any_ctor_superargs,args)) == NULL) return -1;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY((superargs = DeeObject_Call(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DEE_CLASS_SLOTID_SUPERARGS_ANY_CTOR),args)) == NULL) return -1;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 error = DeeTuple_CheckEmpty(superargs)
  ? DeeClass_EXEC_BASE_CTOR(base,self)
  : DeeClass_EXEC_BASE_ANY_CTOR(base,self,superargs);
 Dee_DECREF(superargs);
 if DEE_UNLIKELY(error != 0) {
  DeeHashMap_Quit(&level->il_members);
  return -1;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(suffix->cs_constructor.ctp_any_ctor,(DeeObject *)self,args));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_any_ctor)),(DeeObject *)self,args));
#endif
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}


static int _deeinstance_tp_any_ctor_callctor(
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *args) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeTypeObject *base; DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 if DEE_UNLIKELY(!DeeTuple_CheckEmpty(args)) {
  if DEE_LIKELY(DeeType_NAME(tp))
   DeeError_TypeError_ArgCountExpectedNamed(DeeType_NAME(tp),0,DeeTuple_SIZE(args));
  else DeeError_TypeError_ArgCountExpected(0,DeeTuple_SIZE(args));
  return -1;
 }
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 if DEE_UNLIKELY(DeeClass_EXEC_BASE_ANY_CTOR(base,self,Dee_EmptyTuple) != 0) {
  DeeHashMap_Quit(&level->il_members);
  return -1;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_ctor,(DeeObject *)self,Dee_EmptyTuple));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_ctor)),(DeeObject *)self,Dee_EmptyTuple));
#endif
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_any_ctor_callctor_nobase(
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *args) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 if DEE_UNLIKELY(!DeeTuple_CheckEmpty(args)) {
  if DEE_LIKELY(DeeType_NAME(tp))
   DeeError_TypeError_ArgCountExpectedNamed(DeeType_NAME(tp),0,DeeTuple_SIZE(args));
  else DeeError_TypeError_ArgCountExpected(0,DeeTuple_SIZE(args));
  return -1;
 }
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_ctor,(DeeObject *)self,Dee_EmptyTuple));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_ctor)),(DeeObject *)self,Dee_EmptyTuple));
#endif
 return _DeeInstance_HandleConstructorResultNoBase(level,self,result);
}
static int _deeinstance_tp_any_ctor_callctor_superargs(
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *args) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeTypeObject *base; DeeObject *result,*superargs; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 if DEE_UNLIKELY(!DeeTuple_CheckEmpty(args)) {
  if DEE_LIKELY(DeeType_NAME(tp))
   DeeError_TypeError_ArgCountExpectedNamed(DeeType_NAME(tp),0,DeeTuple_SIZE(args));
  else DeeError_TypeError_ArgCountExpected(0,DeeTuple_SIZE(args));
  return -1;
 }
 suffix = DeeClass_SUFFIX(tp);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_ctor_superargs));
 if DEE_UNLIKELY((superargs = DeeObject_Call(
  suffix->cs_constructor.ctp_ctor_superargs,Dee_EmptyTuple)) == NULL) return -1;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY((superargs = DeeObject_Call(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DEE_CLASS_SLOTID_SUPERARGS_CTOR),Dee_EmptyTuple)) == NULL) return -1;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 error = DeeTuple_CheckEmpty(superargs)
  ? DeeClass_EXEC_BASE_CTOR(base,self)
  : DeeClass_EXEC_BASE_ANY_CTOR(base,self,superargs);
 Dee_DECREF(superargs);
 if DEE_UNLIKELY(error != 0) {
  DeeHashMap_Quit(&level->il_members);
  return error;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_ctor,(DeeObject *)self,Dee_EmptyTuple));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_ctor)),(DeeObject *)self,Dee_EmptyTuple));
#endif
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_any_ctor_callctor_or_anysuperargs(
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *args) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeTypeObject *base; DeeObject *result,*superargs; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 if (DeeTuple_CheckEmpty(args))
  return _deeinstance_tp_ctor_default(tp,self);
 suffix = DeeClass_SUFFIX(tp);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_any_ctor_superargs));
 if DEE_UNLIKELY((superargs = DeeObject_Call(
  suffix->cs_constructor.ctp_any_ctor_superargs,Dee_EmptyTuple)) == NULL) return -1;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY((superargs = DeeObject_Call(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DEE_CLASS_SLOTID_SUPERARGS_ANY_CTOR),Dee_EmptyTuple)) == NULL) return -1;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 error = DeeTuple_CheckEmpty(superargs)
  ? DeeClass_EXEC_BASE_CTOR(base,self)
  : DeeClass_EXEC_BASE_ANY_CTOR(base,self,superargs);
 Dee_DECREF(superargs);
 if DEE_UNLIKELY(error != 0) {
  DeeHashMap_Quit(&level->il_members);
  return error;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_ctor,(DeeObject *)self,Dee_EmptyTuple));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_ctor)),(DeeObject *)self,Dee_EmptyTuple));
#endif
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_any_ctor_callctor_superargs_or_anysuperargs(
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *args) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeTypeObject *base; DeeObject *result,*superargs; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 if (DeeTuple_CheckEmpty(args))
  return _deeinstance_tp_ctor_default_superargs(tp,self);
 suffix = DeeClass_SUFFIX(tp);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_any_ctor_superargs));
 if DEE_UNLIKELY((superargs = DeeObject_Call(
  suffix->cs_constructor.ctp_any_ctor_superargs,Dee_EmptyTuple)) == NULL) return -1;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY((superargs = DeeObject_Call(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DEE_CLASS_SLOTID_SUPERARGS_ANY_CTOR),Dee_EmptyTuple)) == NULL) return -1;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 error = DeeTuple_CheckEmpty(superargs)
  ? DeeClass_EXEC_BASE_CTOR(base,self)
  : DeeClass_EXEC_BASE_ANY_CTOR(base,self,superargs);
 Dee_DECREF(superargs);
 if DEE_UNLIKELY(error != 0) {
  DeeHashMap_Quit(&level->il_members);
  return error;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_ctor,(DeeObject *)self,Dee_EmptyTuple));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_ctor)),(DeeObject *)self,Dee_EmptyTuple));
#endif
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_any_ctor_callcopy(
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *args) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeTypeObject *base; DeeObject *result,*right;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 if DEE_UNLIKELY(DeeTuple_SIZE(args) != 1) {
  if DEE_LIKELY(DeeType_NAME(tp))
   DeeError_TypeError_ArgCountExpectedNamed(DeeType_NAME(tp),1,DeeTuple_SIZE(args));
  else DeeError_TypeError_ArgCountExpected(1,DeeTuple_SIZE(args));
  return -1;
 }
 right = DeeTuple_GET(args,0);
 if DEE_UNLIKELY((right = DeeObject_GetInstance(right,(DeeTypeObject *)tp)) == NULL) return -1;
 if (right != DeeTuple_GET(args,0)) {
  if DEE_UNLIKELY((args = DeeTuple_Pack(1,right)) == NULL) return -1;
 } else Dee_INCREF(args);
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 if DEE_UNLIKELY(DeeClass_EXEC_BASE_ANY_CTOR(base,self,args) != 0) {
  DeeHashMap_Quit(&level->il_members);
  Dee_DECREF(args);
  return -1;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_copy_ctor,(DeeObject *)self,args));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_copy_ctor)),(DeeObject *)self,args));
#endif
 Dee_DECREF(args);
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_any_ctor_callcopy_nobase(
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *args) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeObject *result,*right;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 if DEE_UNLIKELY(DeeTuple_SIZE(args) != 1) {
  if DEE_LIKELY(DeeType_NAME(tp))
   DeeError_TypeError_ArgCountExpectedNamed(DeeType_NAME(tp),1,DeeTuple_SIZE(args));
  else DeeError_TypeError_ArgCountExpected(1,DeeTuple_SIZE(args));
  return -1;
 }
 right = DeeTuple_GET(args,0);
 if DEE_UNLIKELY((right = DeeObject_GetInstance(right,(DeeTypeObject *)tp)) == NULL) return -1;
 if (right != DeeTuple_GET(args,0)) {
  if DEE_UNLIKELY((args = DeeTuple_Pack(1,right)) == NULL) return -1;
 } else Dee_INCREF(args);
 suffix = DeeClass_SUFFIX(tp);
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
  suffix->cs_constructor.ctp_copy_ctor,(DeeObject *)self,args));
#else
 DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_copy_ctor)),(DeeObject *)self,args));
#endif
 Dee_DECREF(args);
 return _DeeInstance_HandleConstructorResultNoBase(level,self,result);
}
static int _deeinstance_tp_any_ctor_callctorcopy(
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *args) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeTypeObject *base; DeeObject *result,*right;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(tp);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_ctor));
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_copy_ctor));
#endif
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 if (DeeTuple_CheckEmpty(args)) {
  if DEE_UNLIKELY(DeeClass_EXEC_BASE_CTOR(base,self) != 0) goto err_member;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
  DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
   suffix->cs_constructor.ctp_ctor,(DeeObject *)self,Dee_EmptyTuple));
#else
  DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
   suffix,DeeType_SLOT_ID(tp_ctor)),(DeeObject *)self,Dee_EmptyTuple));
#endif
 } else if DEE_UNLIKELY(DeeTuple_SIZE(args) > 1) {
  if DEE_LIKELY(DeeType_NAME(tp))
   DeeError_TypeError_ArgCountExpectedNamedMax(DeeType_NAME(tp),1,DeeTuple_SIZE(args));
  else DeeError_TypeError_ArgCountExpectedMax(1,DeeTuple_SIZE(args));
  goto err_member;
 } else {
  if DEE_UNLIKELY(DeeClass_EXEC_BASE_ANY_CTOR(base,self,args) != 0) {
err_member: DeeHashMap_Quit(&level->il_members);
   return -1;
  }
  right = DeeTuple_GET(args,0);
  if DEE_UNLIKELY((right = DeeObject_GetInstance(right,(DeeTypeObject *)tp)) == NULL) result = NULL;
  else {
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
   DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
    suffix->cs_constructor.ctp_copy_ctor,(DeeObject *)self,args));
#else
   DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
    suffix,DeeType_SLOT_ID(tp_copy_ctor)),(DeeObject *)self,args));
#endif
  }
 }
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
static int _deeinstance_tp_any_ctor_callctorcopy_nobase(
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *args) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeObject *result,*right;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(tp);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_ctor));
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_copy_ctor));
#endif
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 if (DeeTuple_CheckEmpty(args)) {
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
  DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
   suffix->cs_constructor.ctp_ctor,(DeeObject *)self,Dee_EmptyTuple));
#else
  DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
   suffix,DeeType_SLOT_ID(tp_ctor)),(DeeObject *)self,Dee_EmptyTuple));
#endif
 } else if DEE_UNLIKELY(DeeTuple_SIZE(args) > 1) {
  if DEE_LIKELY(DeeType_NAME(tp))
   DeeError_TypeError_ArgCountExpectedNamedMax(DeeType_NAME(tp),1,DeeTuple_SIZE(args));
  else DeeError_TypeError_ArgCountExpectedMax(1,DeeTuple_SIZE(args));
  DeeHashMap_Quit(&level->il_members);
  return -1;
 } else {
  right = DeeTuple_GET(args,0);
  if DEE_UNLIKELY((right = DeeObject_GetInstance(right,(DeeTypeObject *)tp)) == NULL) result = NULL;
  else {
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
   DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
    suffix->cs_constructor.ctp_copy_ctor,(DeeObject *)self,args));
#else
   DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
    suffix,DeeType_SLOT_ID(tp_copy_ctor)),(DeeObject *)self,args));
#endif
  }
 }
 return _DeeInstance_HandleConstructorResultNoBase(level,self,result);
}
static int _deeinstance_tp_any_ctor_callctorcopy_superargs(
 DeeClassObject *tp, DeeInstanceObject *self, DeeObject *args) {
 struct DeeClassSuffix *suffix; struct DeeInstanceLevel *level;
 DeeTypeObject *base; DeeObject *result,*right,*superargs; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(tp);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_ctor));
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_copy_ctor));
 DEE_ASSERT(DeeObject_Check(suffix->cs_constructor.ctp_ctor_superargs));
#endif
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 _DeeInstanceLevel_Init(level);
 DeeHashMap_Init(&level->il_members);
 base = DeeType_BASE(tp);
 if (DeeTuple_CheckEmpty(args)) {
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
  if DEE_UNLIKELY((superargs = DeeObject_Call(
   suffix->cs_constructor.ctp_ctor_superargs,Dee_EmptyTuple)) == NULL) return -1;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
  if DEE_UNLIKELY((superargs = DeeObject_Call(DeeClassSuffix_GetKnownVirtOperator(
   suffix,DEE_CLASS_SLOTID_SUPERARGS_CTOR),Dee_EmptyTuple)) == NULL) return -1;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
  error = DeeTuple_CheckEmpty(superargs)
   ? DeeClass_EXEC_BASE_CTOR(base,self)
   : DeeClass_EXEC_BASE_ANY_CTOR(base,self,superargs);
  Dee_DECREF(superargs);
  if DEE_UNLIKELY(error != 0) goto err_member;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
  DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
   suffix->cs_constructor.ctp_ctor,(DeeObject *)self,Dee_EmptyTuple));
#else
  DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
   suffix,DeeType_SLOT_ID(tp_ctor)),(DeeObject *)self,Dee_EmptyTuple));
#endif
 } else if DEE_UNLIKELY(DeeTuple_SIZE(args) > 1) {
  if DEE_LIKELY(DeeType_NAME(tp))
   DeeError_TypeError_ArgCountExpectedNamedMax(DeeType_NAME(tp),1,DeeTuple_SIZE(args));
  else DeeError_TypeError_ArgCountExpectedMax(1,DeeTuple_SIZE(args));
  goto err_member;
 } else {
  if DEE_UNLIKELY(DeeClass_EXEC_BASE_ANY_CTOR(base,self,args) != 0) {
err_member: DeeHashMap_Quit(&level->il_members);
   return -1;
  }
  right = DeeTuple_GET(args,0);
  if DEE_UNLIKELY((right = DeeObject_GetInstance(right,(DeeTypeObject *)tp)) == NULL) result = NULL;
  else {
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
   DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(
    suffix->cs_constructor.ctp_copy_ctor,(DeeObject *)self,args));
#else
   DeeInstance_CALL_CTOR(result,DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
    suffix,DeeType_SLOT_ID(tp_copy_ctor)),(DeeObject *)self,args));
#endif
  }
 }
 return _DeeInstance_HandleConstructorResult(level,base,self,result);
}
#undef DeeClass_EXEC_BASE_ANY_CTOR
#undef DeeClass_EXEC_BASE_MOVE_CTOR
#undef DeeClass_EXEC_BASE_COPY_CTOR
#undef DeeClass_EXEC_BASE_CTOR

static int _deeinstance_tp_any_assign_default(
 DeeInstanceObject *self, DeeObject *right) {
 struct DeeClassSuffix *suffix; DeeObject *result,*args;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,right)) == NULL) return -1;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 result = DeeObject_ThisCall(suffix->cs_assign.ctp_any_assign,(DeeObject *)self,args);
#else
 result = DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_any_assign)),(DeeObject *)self,args);
#endif
 Dee_DECREF(args);
 if DEE_UNLIKELY(!result) return -1;
 Dee_DECREF(result);
 return 0;
}
static int _deeinstance_tp_any_assign_callcopy(
 DeeInstanceObject *self, DeeObject *right) {
 struct DeeClassSuffix *suffix; DeeObject *result,*args;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 if DEE_UNLIKELY((right = DeeObject_GetInstance(right,Dee_TYPE(self))) == NULL) return -1;
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,right)) == NULL) return -1;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 result = DeeObject_ThisCall(suffix->cs_assign.ctp_copy_assign,(DeeObject *)self,args);
#else
 result = DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_copy_assign)),(DeeObject *)self,args);
#endif
 Dee_DECREF(args);
 if DEE_UNLIKELY(!result) return -1;
 Dee_DECREF(result);
 return 0;
}
#define _deeinstance_tp_copy_assign_callany _deeinstance_tp_any_assign_default
static int _deeinstance_tp_copy_assign_default(
 DeeInstanceObject *self, DeeObject *right) {
 struct DeeClassSuffix *suffix; DeeObject *result,*args;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,right)) == NULL) return -1;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 result = DeeObject_ThisCall(suffix->cs_assign.ctp_copy_assign,(DeeObject *)self,args);
#else
 result = DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_copy_assign)),(DeeObject *)self,args);
#endif
 Dee_DECREF(args);
 if DEE_UNLIKELY(!result) return -1;
 Dee_DECREF(result);
 return 0;
}
#define _deeinstance_tp_move_assign_callcopy _deeinstance_tp_copy_assign_default
#define _deeinstance_tp_move_assign_callany _deeinstance_tp_any_assign_default
static int _deeinstance_tp_move_assign_default(
 DeeInstanceObject *self, DeeObject *right) {
 struct DeeClassSuffix *suffix; DeeObject *result,*args;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,right)) == NULL) return -1;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 result = DeeObject_ThisCall(suffix->cs_assign.ctp_move_assign,(DeeObject *)self,args);
#else
 result = DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_move_assign)),(DeeObject *)self,args);
#endif
 Dee_DECREF(args);
 if DEE_UNLIKELY(!result) return -1;
 Dee_DECREF(result);
 return 0;
}


static DeeObject *_deeinstance_tp_not_callbool(DeeInstanceObject *self) {
 struct DeeClassSuffix *suffix; DeeObject *result_ob; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 if DEE_UNLIKELY((result_ob = DeeObject_ThisCall(suffix->cs_math.ctp_bool,
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return NULL;
#else
 if DEE_UNLIKELY((result_ob = DeeObject_ThisCall(
  DeeClassSuffix_GetKnownVirtOperator(suffix,DeeType_SLOT_ID(tp_bool)),
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return NULL;
#endif
 result = DeeObject_Bool(result_ob);
 Dee_DECREF(result_ob);
 if DEE_UNLIKELY(result < 0) return NULL;
 DeeReturn_Bool(!result);
}
static int _deeinstance_tp_bool_callnot(DeeInstanceObject *self) {
 struct DeeClassSuffix *suffix; DeeObject *result_ob; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 if DEE_UNLIKELY((result_ob = DeeObject_ThisCall(suffix->cs_math.ctp_not,
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return -1;
#else
 if DEE_UNLIKELY((result_ob = DeeObject_ThisCall(
  DeeClassSuffix_GetKnownVirtOperator(suffix,DeeType_SLOT_ID(tp_not)),
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return -1;
#endif
 result = DeeObject_Bool(result_ob);
 Dee_DECREF(result_ob);
 return result < 0 ? result : !result;
}
static int _deeinstance_tp_int32_callint64(DeeInstanceObject *self, Dee_int32_t *result) {
 struct DeeClassSuffix *suffix; DeeObject *result_ob; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 if DEE_UNLIKELY((result_ob = DeeObject_ThisCall(suffix->cs_cast.ctp_int64,
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return -1;
#else
 if DEE_UNLIKELY((result_ob = DeeObject_ThisCall(
  DeeClassSuffix_GetKnownVirtOperator(suffix,DeeType_SLOT_ID(tp_int64)),
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return -1;
#endif
 error = DeeObject_GetInt32(result_ob,result);
 Dee_DECREF(result_ob);
 return error;
}
static int _deeinstance_tp_int32_calldouble(DeeInstanceObject *self, Dee_int32_t *result) {
 struct DeeClassSuffix *suffix; DeeObject *result_ob; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 if DEE_UNLIKELY((result_ob = DeeObject_ThisCall(suffix->cs_cast.ctp_double,
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return -1;
#else
 if DEE_UNLIKELY((result_ob = DeeObject_ThisCall(
  DeeClassSuffix_GetKnownVirtOperator(suffix,DeeType_SLOT_ID(tp_double)),
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return -1;
#endif
 error = DeeObject_GetInt32(result_ob,result);
 Dee_DECREF(result_ob);
 return error;
}
static int _deeinstance_tp_int64_callint32(DeeInstanceObject *self, Dee_int64_t *result) {
 struct DeeClassSuffix *suffix; DeeObject *result_ob; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 if DEE_UNLIKELY((result_ob = DeeObject_ThisCall(suffix->cs_cast.ctp_int32,
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return -1;
#else
 if DEE_UNLIKELY((result_ob = DeeObject_ThisCall(
  DeeClassSuffix_GetKnownVirtOperator(suffix,DeeType_SLOT_ID(tp_int32)),
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return -1;
#endif
 error = DeeObject_GetInt64(result_ob,result);
 Dee_DECREF(result_ob);
 return error;
}
static int _deeinstance_tp_int64_calldouble(DeeInstanceObject *self, Dee_int64_t *result) {
 struct DeeClassSuffix *suffix; DeeObject *result_ob; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 if DEE_UNLIKELY((result_ob = DeeObject_ThisCall(suffix->cs_cast.ctp_double,
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return -1;
#else
 if DEE_UNLIKELY((result_ob = DeeObject_ThisCall(
  DeeClassSuffix_GetKnownVirtOperator(suffix,DeeType_SLOT_ID(tp_double)),
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return -1;
#endif
 error = DeeObject_GetInt64(result_ob,result);
 Dee_DECREF(result_ob);
 return error;
}
static int _deeinstance_tp_double_callint32(DeeInstanceObject *self, double *result) {
 struct DeeClassSuffix *suffix; DeeObject *result_ob; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 if DEE_UNLIKELY((result_ob = DeeObject_ThisCall(suffix->cs_cast.ctp_int32,
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return -1;
#else
 if DEE_UNLIKELY((result_ob = DeeObject_ThisCall(
  DeeClassSuffix_GetKnownVirtOperator(suffix,DeeType_SLOT_ID(tp_int32)),
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return -1;
#endif
 error = DeeObject_GetDouble(result_ob,result);
 Dee_DECREF(result_ob);
 return error;
}
static int _deeinstance_tp_double_callint64(DeeInstanceObject *self, double *result) {
 struct DeeClassSuffix *suffix; DeeObject *result_ob; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 if DEE_UNLIKELY((result_ob = DeeObject_ThisCall(suffix->cs_cast.ctp_int64,
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return -1;
#else
 if DEE_UNLIKELY((result_ob = DeeObject_ThisCall(
  DeeClassSuffix_GetKnownVirtOperator(suffix,DeeType_SLOT_ID(tp_int64)),
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return -1;
#endif
 error = DeeObject_GetDouble(result_ob,result);
 Dee_DECREF(result_ob);
 return error;
}
static DeeObject *_deeinstance_tp_incpost_callinc(DeeInstanceObject *self) {
 struct DeeClassSuffix *suffix; DeeObject *result,*temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 if DEE_UNLIKELY((result = DeeObject_Copy((DeeObject *)self)) == NULL) return NULL;
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 if DEE_UNLIKELY((temp = DeeObject_ThisCall(suffix->cs_math.ctp_inc,
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) Dee_CLEAR(result);
#else
 if DEE_UNLIKELY((temp = DeeObject_ThisCall(
  DeeClassSuffix_GetKnownVirtOperator(suffix,DeeType_SLOT_ID(tp_inc)),
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) Dee_CLEAR(result);
#endif
 else Dee_DECREF(temp);
 return result;
}
static DeeObject *_deeinstance_tp_decpost_calldec(DeeInstanceObject *self) {
 struct DeeClassSuffix *suffix; DeeObject *result,*temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 if DEE_UNLIKELY((result = DeeObject_Copy((DeeObject *)self)) == NULL) return NULL;
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 if DEE_UNLIKELY((temp = DeeObject_ThisCall(suffix->cs_math.ctp_dec,
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) Dee_CLEAR(result);
#else
 if DEE_UNLIKELY((temp = DeeObject_ThisCall(
  DeeClassSuffix_GetKnownVirtOperator(suffix,DeeType_SLOT_ID(tp_dec)),
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) Dee_CLEAR(result);
#endif
 else Dee_DECREF(temp);
 return result;
}


static DeeObject *_deeinstance_tp_cmp_lo_callge(
 DeeInstanceObject *self, DeeObject *right) {
 struct DeeClassSuffix *suffix; DeeObject *temp,*result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
 if DEE_UNLIKELY((temp = DeeTuple_Pack(1,right)) == NULL) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 result = DeeObject_ThisCall(suffix->cs_compare.ctp_cmp_ge,(DeeObject *)self,temp);
#else
 result = DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_cmp_ge)),(DeeObject *)self,temp);
#endif
 Dee_DECREF(temp);
 if DEE_UNLIKELY(!result) return NULL;
 temp = DeeObject_Not(result);
 Dee_DECREF(result);
 return temp;
}
static DeeObject *_deeinstance_tp_cmp_le_callgr(
 DeeInstanceObject *self, DeeObject *right) {
 struct DeeClassSuffix *suffix; DeeObject *temp,*result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
 if DEE_UNLIKELY((temp = DeeTuple_Pack(1,right)) == NULL) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 result = DeeObject_ThisCall(suffix->cs_compare.ctp_cmp_gr,(DeeObject *)self,temp);
#else
 result = DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_cmp_gr)),(DeeObject *)self,temp);
#endif
 Dee_DECREF(temp);
 if DEE_UNLIKELY(!result) return NULL;
 temp = DeeObject_Not(result);
 Dee_DECREF(result);
 return temp;
}
static DeeObject *_deeinstance_tp_cmp_eq_callne(
 DeeInstanceObject *self, DeeObject *right) {
 struct DeeClassSuffix *suffix; DeeObject *temp,*result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
 if DEE_UNLIKELY((temp = DeeTuple_Pack(1,right)) == NULL) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 result = DeeObject_ThisCall(suffix->cs_compare.ctp_cmp_ne,(DeeObject *)self,temp);
#else
 result = DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_cmp_ne)),(DeeObject *)self,temp);
#endif
 Dee_DECREF(temp);
 if DEE_UNLIKELY(!result) return NULL;
 temp = DeeObject_Not(result);
 Dee_DECREF(result);
 return temp;
}
static DeeObject *_deeinstance_tp_cmp_ne_calleq(
 DeeInstanceObject *self, DeeObject *right) {
 struct DeeClassSuffix *suffix; DeeObject *temp,*result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
 if DEE_UNLIKELY((temp = DeeTuple_Pack(1,right)) == NULL) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 result = DeeObject_ThisCall(suffix->cs_compare.ctp_cmp_eq,(DeeObject *)self,temp);
#else
 result = DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_cmp_eq)),(DeeObject *)self,temp);
#endif
 Dee_DECREF(temp);
 if DEE_UNLIKELY(!result) return NULL;
 temp = DeeObject_Not(result);
 Dee_DECREF(result);
 return temp;
}
static DeeObject *_deeinstance_tp_cmp_gr_callle(
 DeeInstanceObject *self, DeeObject *right) {
 struct DeeClassSuffix *suffix; DeeObject *temp,*result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
 if DEE_UNLIKELY((temp = DeeTuple_Pack(1,right)) == NULL) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 result = DeeObject_ThisCall(suffix->cs_compare.ctp_cmp_le,(DeeObject *)self,temp);
#else
 result = DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_cmp_le)),(DeeObject *)self,temp);
#endif
 Dee_DECREF(temp);
 if DEE_UNLIKELY(!result) return NULL;
 temp = DeeObject_Not(result);
 Dee_DECREF(result);
 return temp;
}
static DeeObject *_deeinstance_tp_cmp_ge_calllo(
 DeeInstanceObject *self, DeeObject *right) {
 struct DeeClassSuffix *suffix; DeeObject *temp,*result;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
 if DEE_UNLIKELY((temp = DeeTuple_Pack(1,right)) == NULL) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 result = DeeObject_ThisCall(suffix->cs_compare.ctp_cmp_lo,(DeeObject *)self,temp);
#else
 result = DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_cmp_lo)),(DeeObject *)self,temp);
#endif
 Dee_DECREF(temp);
 if DEE_UNLIKELY(!result) return NULL;
 temp = DeeObject_Not(result);
 Dee_DECREF(result);
 return temp;
}
static DeeObject *_deeinstance_tp_seq_iter_self_callfor(DeeInstanceObject *self) {
 struct DeeClassSuffix *suffix; DeeObject *result,*iterator;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 suffix = DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 if DEE_UNLIKELY((result = DeeObject_ThisCall(suffix->cs_seq.ctp_seq_iter_self,
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return NULL;
#else
 if DEE_UNLIKELY((result = DeeObject_ThisCall(
  DeeClassSuffix_GetKnownVirtOperator(suffix,DeeType_SLOT_ID(tp_seq_iter_self)),
  (DeeObject *)self,Dee_EmptyTuple)) == NULL) return NULL;
#endif
 iterator = DeeObject_IterSelf(result);
 Dee_DECREF(result);
 return iterator;
}

//////////////////////////////////////////////////////////////////////////
// FileClass operator wrappers
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
static int DEE_CALL _deefileinstance_tp_io_read(
 struct DeeFileObject *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 struct DeeFileClassSuffix *suffix; DeeObject *result; int temp;
 DEE_ASSERT(DeeFileClass_Check(Dee_TYPE(self)));
 suffix = (struct DeeFileClassSuffix *)DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->tp_file.ctp_io_read));
 result = DeeObject_ThisCallf(suffix->tp_file.ctp_io_read,(DeeObject *)self,"pIu",p,s);
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 result = DeeObject_ThisCallf(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_io_read)),(DeeObject *)self,"pIu",p,s);
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY(!result) return -1;
 temp = DeeObject_Cast(Dee_size_t,result,rs);
 if DEE_UNLIKELY(*rs > s) *rs = s; // Silently ignore impossible values...
 Dee_DECREF(result);
 return temp;
}
static int DEE_CALL _deefileinstance_tp_io_write(
 struct DeeFileObject *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 struct DeeFileClassSuffix *suffix; DeeObject *result; int temp;
 DEE_ASSERT(DeeFileClass_Check(Dee_TYPE(self)));
 suffix = (struct DeeFileClassSuffix *)DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->tp_file.ctp_io_write));
 result = DeeObject_ThisCallf(suffix->tp_file.ctp_io_write,(DeeObject *)self,"pIu",p,s);
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 result = DeeObject_ThisCallf(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_io_write)),(DeeObject *)self,"pIu",p,s);
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY(!result) return -1;
 temp = DeeObject_Cast(Dee_size_t,result,ws);
 if DEE_UNLIKELY(*ws > s) *ws = s; // Silently ignore impossible values...
 Dee_DECREF(result);
 return temp;
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
static int DEE_CALL _deefileinstance_tp_io_read_np(
 struct DeeFileObject *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 struct DeeFileClassSuffix *suffix; DeeStringObject *result;
 Dee_size_t read_size;
 DEE_ASSERT(DeeFileClass_Check(Dee_TYPE(self)));
 suffix = (struct DeeFileClassSuffix *)DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->tp_file.ctp_io_read));
 result = (DeeStringObject *)DeeObject_ThisCallf(
  suffix->tp_file.ctp_io_read,(DeeObject *)self,"Iu",s);
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 result = (DeeStringObject *)DeeObject_ThisCallf(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_io_read)),(DeeObject *)self,"Iu",s);
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)result,&DeeString_Type) != 0) { Dee_DECREF(result); return -1; }
 read_size = DeeString_SIZE(result) >= s ? s : DeeString_SIZE(result);
 memcpy(p,DeeString_STR(result),read_size);
 *rs = read_size;
 Dee_DECREF(result);
 return 0;
}
static int DEE_CALL _deefileinstance_tp_io_write_np(
 struct DeeFileObject *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 struct DeeFileClassSuffix *suffix; DeeObject *result;
 int temp; DeeStringObject *bufstring;
 DEE_ASSERT(DeeFileClass_Check(Dee_TYPE(self)));
 suffix = (struct DeeFileClassSuffix *)DeeClass_SUFFIX(Dee_TYPE(self));
 if DEE_UNLIKELY((bufstring = (DeeStringObject *)DeeString_NewWithLength(s/sizeof(char),(char *)p)) == NULL) return -1;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->tp_file.ctp_io_write));
 result = DeeObject_ThisCallf(suffix->tp_file.ctp_io_write,(DeeObject *)self,"o",bufstring);
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 result = DeeObject_ThisCallf(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_io_write)),(DeeObject *)self,"o",bufstring);
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 Dee_DECREF(bufstring);
 if DEE_UNLIKELY(!result) return -1;
 temp = DeeObject_Cast(Dee_size_t,result,ws);
 Dee_DECREF(result);
 if (DEE_LIKELY(temp == 0) && DEE_UNLIKELY(*ws > s)) *ws = s;
 return temp;
}
static int DEE_CALL _deefileinstance_tp_io_seek(
 struct DeeFileObject *self, Dee_int64_t off, int whence, Dee_uint64_t *pos) {
 struct DeeFileClassSuffix *suffix; DeeObject *result; int temp;
 DEE_ASSERT(DeeFileClass_Check(Dee_TYPE(self)));
 suffix = (struct DeeFileClassSuffix *)DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->tp_file.ctp_io_seek));
 result = DeeObject_ThisCallf(suffix->tp_file.ctp_io_seek,(DeeObject *)self,"I64dd",off,whence);
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 result = DeeObject_ThisCallf(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_io_seek)),(DeeObject *)self,"I64dd",off,whence);
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY(!result) return -1;
 if (pos) temp = DeeObject_Cast(Dee_uint64_t,result,pos); else temp = 0;
 Dee_DECREF(result);
 return temp;
}
static int DEE_CALL _deefileinstance_tp_io_flush(struct DeeFileObject *self) {
 struct DeeFileClassSuffix *suffix; DeeObject *result;
 DEE_ASSERT(DeeFileClass_Check(Dee_TYPE(self)));
 suffix = (struct DeeFileClassSuffix *)DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->tp_file.ctp_io_flush));
 result = DeeObject_ThisCall(suffix->tp_file.ctp_io_flush,(DeeObject *)self,Dee_EmptyTuple);
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 result = DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_io_flush)),(DeeObject *)self,Dee_EmptyTuple);
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY(!result) return -1;
 Dee_DECREF(result);
 return 0;
}
static int DEE_CALL _deefileinstance_tp_io_trunc(struct DeeFileObject *self) {
 struct DeeFileClassSuffix *suffix; DeeObject *result;
 DEE_ASSERT(DeeFileClass_Check(Dee_TYPE(self)));
 suffix = (struct DeeFileClassSuffix *)DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->tp_file.ctp_io_trunc));
 result = DeeObject_ThisCall(suffix->tp_file.ctp_io_trunc,(DeeObject *)self,Dee_EmptyTuple);
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 result = DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_io_trunc)),(DeeObject *)self,Dee_EmptyTuple);
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY(!result) return -1;
 Dee_DECREF(result);
 return 0;
}
static void DEE_CALL _deefileinstance_tp_io_close(struct DeeFileObject *self) {
 struct DeeFileClassSuffix *suffix; DeeObject *result;
 DEE_ASSERT(DeeFileClass_Check(Dee_TYPE(self)));
 suffix = (struct DeeFileClassSuffix *)DeeClass_SUFFIX(Dee_TYPE(self));
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
 DEE_ASSERT(DeeObject_Check(suffix->tp_file.ctp_io_close));
 result = DeeObject_ThisCall(suffix->tp_file.ctp_io_close,(DeeObject *)self,Dee_EmptyTuple);
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 result = DeeObject_ThisCall(DeeClassSuffix_GetKnownVirtOperator(
  suffix,DeeType_SLOT_ID(tp_io_close)),(DeeObject *)self,Dee_EmptyTuple);
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 if DEE_UNLIKELY(!result) {
  while (!DeeError_Print("[ignored] Unhandled error in __close__ callback of file class",1));
 }
 else Dee_DECREF(result);
}



//////////////////////////////////////////////////////////////////////////
static struct _DeeClassTypeMethodDef _deeclass_empty_methods[] = {{NULL,NULL}};
static struct _DeeClassTypeGetSetDef _deeclass_empty_getsets[] = {{NULL,NULL,NULL,NULL}};

DEE_A_RET_EXCEPT(-1) int DeeClass_AddMethod(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name,
 DEE_A_INOUT DeeObject *method) {
 struct _DeeClassTypeMethodDef *new_list; Dee_size_t old_len = 0;
 struct DeeClassSuffix *suffix;
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(DeeObject_Check(method));
 DEE_ASSERT(!DeeClass_HasMethod(self,name));
 suffix = DeeClass_SUFFIX(self);
 new_list = suffix->cs_methods;
 while (new_list->ctmd_name) ++old_len,++new_list;
 while (1) {
  if (old_len) {
   new_list = (struct _DeeClassTypeMethodDef *)realloc_nnz(
    suffix->cs_methods,(old_len+2)*sizeof(struct _DeeClassTypeMethodDef));
  } else {
   new_list = (struct _DeeClassTypeMethodDef *)malloc_nz(
    2*sizeof(struct _DeeClassTypeMethodDef));
  }
  if (new_list) break;
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 suffix->cs_methods = new_list;
 new_list += old_len;
 Dee_INCREF(new_list->ctmd_name = (DeeStringObject *)name);
 Dee_INCREF(new_list->ctmd_func = method);
 new_list[1].ctmd_name = NULL;
#ifdef DEE_DEBUG
 new_list[1].ctmd_func = NULL;
#endif
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeClass_AddProperty(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *name,
 DEE_A_INOUT_OPT DeeObject *getter_, DEE_A_INOUT_OPT DeeObject *delete_, DEE_A_INOUT_OPT DeeObject *setter_) {
 struct _DeeClassTypeGetSetDef *new_list; Dee_size_t old_len = 0;
 struct DeeClassSuffix *suffix;
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(!getter_ || DeeObject_Check(getter_));
 DEE_ASSERT(!delete_ || DeeObject_Check(delete_));
 DEE_ASSERT(!setter_ || DeeObject_Check(setter_));
 DEE_ASSERT(!DeeClass_HasProperty(self,name));
 suffix = DeeClass_SUFFIX(self);
 new_list = suffix->cs_getsets;
 while (new_list->ctgd_name) ++old_len,++new_list;
 while (1) {
  if (old_len) {
   new_list = (struct _DeeClassTypeGetSetDef *)realloc_nnz(
    suffix->cs_getsets,(old_len+2)*sizeof(struct _DeeClassTypeGetSetDef));
  } else {
   new_list = (struct _DeeClassTypeGetSetDef *)malloc_nz(
    2*sizeof(struct _DeeClassTypeGetSetDef));
  }
  if (new_list) break;
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 suffix->cs_getsets = new_list;
 new_list += old_len;
 Dee_INCREF(new_list->ctgd_name = (DeeStringObject *)name);
 Dee_XINCREF(new_list->ctgd_get = getter_);
 Dee_XINCREF(new_list->ctgd_del = delete_);
 Dee_XINCREF(new_list->ctgd_set = setter_);
 new_list[1].ctgd_name = NULL;
#ifdef DEE_DEBUG
 new_list[1].ctgd_get = NULL;
 new_list[1].ctgd_del = NULL;
 new_list[1].ctgd_set = NULL;
#endif
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeClass_AddClassMethod(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *name,
 DEE_A_INOUT DeeObject *method) {
 struct _DeeClassTypeMethodDef *new_list; Dee_size_t old_len = 0;
 struct DeeClassSuffix *suffix;
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(DeeObject_Check(method));
 DEE_ASSERT(!DeeClass_HasClassMethod(self,name));
 suffix = DeeClass_SUFFIX(self);
 new_list = suffix->cs_class_methods;
 while (new_list->ctmd_name) ++old_len,++new_list;
 while (1) {
  if (old_len) {
   new_list = (struct _DeeClassTypeMethodDef *)realloc_nnz(
    suffix->cs_class_methods,(old_len+2)*sizeof(struct _DeeClassTypeMethodDef));
  } else {
   new_list = (struct _DeeClassTypeMethodDef *)malloc_nz(
    2*sizeof(struct _DeeClassTypeMethodDef));
  }
  if (new_list) break;
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 suffix->cs_class_methods = new_list;
 new_list += old_len;
 Dee_INCREF(new_list->ctmd_name = (DeeStringObject *)name);
 Dee_INCREF(new_list->ctmd_func = method);
 new_list[1].ctmd_name = NULL;
#ifdef DEE_DEBUG
 new_list[1].ctmd_func = NULL;
#endif
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeClass_AddClassProperty(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *name,
 DEE_A_INOUT_OPT DeeObject *getter_, DEE_A_INOUT_OPT DeeObject *delete_, DEE_A_INOUT_OPT DeeObject *setter_) {
 struct _DeeClassTypeGetSetDef *new_list; Dee_size_t old_len = 0;
 struct DeeClassSuffix *suffix;
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(!getter_ || DeeObject_Check(getter_));
 DEE_ASSERT(!delete_ || DeeObject_Check(delete_));
 DEE_ASSERT(!setter_ || DeeObject_Check(setter_));
 DEE_ASSERT(!DeeClass_HasClassProperty(self,name));
 suffix = DeeClass_SUFFIX(self);
 new_list = suffix->cs_class_getsets;
 while (new_list->ctgd_name) ++old_len,++new_list;
 while (1) {
  if (old_len) {
   new_list = (struct _DeeClassTypeGetSetDef *)realloc_nnz(
    suffix->cs_class_getsets,(old_len+2)*sizeof(struct _DeeClassTypeGetSetDef));
  } else {
   new_list = (struct _DeeClassTypeGetSetDef *)malloc_nz(
    2*sizeof(struct _DeeClassTypeGetSetDef));
  }
  if (new_list) break;
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 suffix->cs_class_getsets = new_list;
 new_list += old_len;
 Dee_INCREF(new_list->ctgd_name = (DeeStringObject *)name);
 Dee_XINCREF(new_list->ctgd_get = getter_);
 Dee_XINCREF(new_list->ctgd_del = delete_);
 Dee_XINCREF(new_list->ctgd_set = setter_);
 new_list[1].ctgd_name = NULL;
#ifdef DEE_DEBUG
 new_list[1].ctgd_get = NULL;
 new_list[1].ctgd_del = NULL;
 new_list[1].ctgd_set = NULL;
#endif
 return 0;
}


DEE_A_RET_NOEXCEPT(0) int DeeClass_HasMethodString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self, DEE_A_IN_Z char const *name) {
 struct _DeeClassTypeMethodDef *iter; Dee_size_t name_len;
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(name);
 iter = DeeClass_SUFFIX(self)->cs_methods;
 name_len = strlen(name);
 while (iter->ctmd_name) {
  if (DeeString_SIZE(iter->ctmd_name) == name_len &&
      memcmp(DeeString_STR(iter->ctmd_name),name,name_len) == 0
      ) return 1;
  ++iter;
 }
 return 0;
}
DEE_A_RET_NOEXCEPT(0) int DeeClass_HasPropertyString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self, DEE_A_IN_Z char const *name) {
 struct _DeeClassTypeGetSetDef *iter; Dee_size_t name_len;
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(name);
 iter = DeeClass_SUFFIX(self)->cs_getsets;
 name_len = strlen(name);
 while (iter->ctgd_name) {
  if (DeeString_SIZE(iter->ctgd_name) == name_len &&
      memcmp(DeeString_STR(iter->ctgd_name),name,name_len) == 0
      ) return 1;
  ++iter;
 }
 return 0;
}
DEE_A_RET_NOEXCEPT(0) int DeeClass_HasClassMethodString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self, DEE_A_IN_Z char const *name) {
 struct _DeeClassTypeMethodDef *iter; Dee_size_t name_len;
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(name);
 iter = DeeClass_SUFFIX(self)->cs_class_methods;
 name_len = strlen(name);
 while (iter->ctmd_name) {
  if (DeeString_SIZE(iter->ctmd_name) == name_len &&
      memcmp(DeeString_STR(iter->ctmd_name),name,name_len) == 0
      ) return 1;
  ++iter;
 }
 return 0;
}
DEE_A_RET_NOEXCEPT(0) int DeeClass_HasClassPropertyString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self, DEE_A_IN_Z char const *name) {
 struct _DeeClassTypeGetSetDef *iter; Dee_size_t name_len;
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(name);
 iter = DeeClass_SUFFIX(self)->cs_class_getsets;
 name_len = strlen(name);
 while (iter->ctgd_name) {
  if (DeeString_SIZE(iter->ctgd_name) == name_len &&
      memcmp(DeeString_STR(iter->ctgd_name),name,name_len) == 0
      ) return 1;
  ++iter;
 }
 return 0;
}


DEE_A_RET_NOEXCEPT(NULL) DeeObject *DeeClass_GetMethod(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name) {
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 return DeeClass_GetMethodString(self,DeeString_STR(name));
}
DEE_A_RET_NOEXCEPT(0) int DeeClass_GetProperty(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name,
 DEE_A_OUT_OPT DeeObject **getter_, DEE_A_OUT_OPT DeeObject **delete_,
 DEE_A_OUT_OPT DeeObject **setter_) {
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 return DeeClass_GetPropertyString(self,DeeString_STR(name),getter_,delete_,setter_);
}
DEE_A_RET_NOEXCEPT(NULL) DeeObject *DeeClass_GetClassMethod(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name) {
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 return DeeClass_GetClassMethodString(self,DeeString_STR(name));
}
DEE_A_RET_NOEXCEPT(0) int DeeClass_GetClassProperty(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name,
 DEE_A_OUT_OPT DeeObject **getter_, DEE_A_OUT_OPT DeeObject **delete_,
 DEE_A_OUT_OPT DeeObject **setter_) {
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 return DeeClass_GetClassPropertyString(self,DeeString_STR(name),getter_,delete_,setter_);
}
DEE_A_RET_NOEXCEPT(NULL) DeeObject *DeeClass_GetMethodString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self, DEE_A_IN_Z char const *name) {
 struct _DeeClassTypeMethodDef *iter; Dee_size_t name_len;
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(name);
 iter = DeeClass_SUFFIX(self)->cs_methods;
 name_len = strlen(name);
 while (iter->ctmd_name) {
  if (DeeString_SIZE(iter->ctmd_name) == name_len &&
      memcmp(DeeString_STR(iter->ctmd_name),name,name_len) == 0
      ) return iter->ctmd_func;
  ++iter;
 }
 return NULL;
}
DEE_A_RET_NOEXCEPT(0) int DeeClass_GetPropertyString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self, DEE_A_IN_Z char const *name,
 DEE_A_OUT_OPT DeeObject **getter_, DEE_A_OUT_OPT DeeObject **delete_,
 DEE_A_OUT_OPT DeeObject **setter_)  {
 struct _DeeClassTypeGetSetDef *iter; Dee_size_t name_len;
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(name);
 iter = DeeClass_SUFFIX(self)->cs_getsets;
 name_len = strlen(name);
 while (iter->ctgd_name) {
  if (DeeString_SIZE(iter->ctgd_name) == name_len &&
      memcmp(DeeString_STR(iter->ctgd_name),name,name_len) == 0) {
   if (getter_) *getter_ = iter->ctgd_get;
   if (delete_) *delete_ = iter->ctgd_del;
   if (setter_) *setter_ = iter->ctgd_set;
   return 1;
  }
  ++iter;
 }
 return 0;
}
DEE_A_RET_NOEXCEPT(NULL) DeeObject *DeeClass_GetClassMethodString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self, DEE_A_IN_Z char const *name) {
 struct _DeeClassTypeMethodDef *iter; Dee_size_t name_len;
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(name);
 iter = DeeClass_SUFFIX(self)->cs_class_methods;
 name_len = strlen(name);
 while (iter->ctmd_name) {
  if (DeeString_SIZE(iter->ctmd_name) == name_len &&
      memcmp(DeeString_STR(iter->ctmd_name),name,name_len) == 0
      ) return iter->ctmd_func;
  ++iter;
 }
 return NULL;
}
DEE_A_RET_NOEXCEPT(0) int DeeClass_GetClassPropertyString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self, DEE_A_IN_Z char const *name,
 DEE_A_OUT_OPT DeeObject **getter_, DEE_A_OUT_OPT DeeObject **delete_, DEE_A_OUT_OPT DeeObject **setter_) {
 struct _DeeClassTypeGetSetDef *iter; Dee_size_t name_len;
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(name);
 iter = DeeClass_SUFFIX(self)->cs_class_getsets;
 name_len = strlen(name);
 while (iter->ctgd_name) {
  if (DeeString_SIZE(iter->ctgd_name) == name_len &&
      memcmp(DeeString_STR(iter->ctgd_name),name,name_len) == 0) {
   if (getter_) *getter_ = iter->ctgd_get;
   if (delete_) *delete_ = iter->ctgd_del;
   if (setter_) *setter_ = iter->ctgd_set;
   return 1;
  }
  ++iter;
 }
 return 0;
}



DEE_A_RET_EXCEPT(-1) int DeeClass_AddMethodString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self,
 DEE_A_IN_Z char const *name, DEE_A_INOUT DeeObject *method) {
 DeeObject *name_ob; int result;
 if ((name_ob = DeeString_New(name)) == NULL) return -1;
 result = DeeClass_AddMethod(self,name_ob,method);
 Dee_DECREF(name_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeClass_AddPropertyString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_Z char const *name,
 DEE_A_INOUT_OPT DeeObject *getter_, DEE_A_INOUT_OPT DeeObject *delete_,
 DEE_A_INOUT_OPT DeeObject *setter_) {
 DeeObject *name_ob; int result;
 if ((name_ob = DeeString_New(name)) == NULL) return -1;
 result = DeeClass_AddProperty(self,name_ob,getter_,delete_,setter_);
 Dee_DECREF(name_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeClass_AddClassMethodString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self,
 DEE_A_IN_Z char const *name, DEE_A_INOUT DeeObject *method) {
 DeeObject *name_ob; int result;
 if ((name_ob = DeeString_New(name)) == NULL) return -1;
 result = DeeClass_AddClassMethod(self,name_ob,method);
 Dee_DECREF(name_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeClass_AddClassPropertyString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_Z char const *name,
 DEE_A_INOUT_OPT DeeObject *getter_, DEE_A_INOUT_OPT DeeObject *delete_,
 DEE_A_INOUT_OPT DeeObject *setter_) {
 DeeObject *name_ob; int result;
 if ((name_ob = DeeString_New(name)) == NULL) return -1;
 result = DeeClass_AddClassProperty(self,name_ob,getter_,delete_,setter_);
 Dee_DECREF(name_ob);
 return result;
}

DEE_A_RET_NOEXCEPT(0) int DeeClass_HasMethod(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name) {
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 return DeeClass_HasMethodString(self,DeeString_STR(name));
}
DEE_A_RET_NOEXCEPT(0) int DeeClass_HasProperty(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name) {
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 return DeeClass_HasPropertyString(self,DeeString_STR(name));
}
DEE_A_RET_NOEXCEPT(0) int DeeClass_HasClassMethod(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name) {
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 return DeeClass_HasClassMethodString(self,DeeString_STR(name));
}
DEE_A_RET_NOEXCEPT(0) int DeeClass_HasClassProperty(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name) {
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 return DeeClass_HasClassPropertyString(self,DeeString_STR(name));
}


DEE_A_RET_NOEXCEPT(NULL) DeeObject *DeeClass_GetSlot(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self, DEE_A_IN int slot) {
 struct DeeClassSuffix *suffix;
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 suffix = DeeClass_SUFFIX(self);
 switch (slot) {
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
  case DeeType_SLOT_ID(tp_dtor):
  case DeeType_SLOT_ID(tp_clear):
   return suffix->cs_destructor.ctp_dtor;
  case DeeType_SLOT_ID(tp_ctor):
   return suffix->cs_constructor.ctp_ctor;
  case DeeType_SLOT_ID(tp_copy_ctor):
   return suffix->cs_constructor.ctp_copy_ctor;
  case DeeType_SLOT_ID(tp_move_ctor):
   return suffix->cs_constructor.ctp_move_ctor;
  case DeeType_SLOT_ID(tp_any_ctor):
   return suffix->cs_constructor.ctp_any_ctor;
  case DeeType_SLOT_ID(tp_any_assign):
   return suffix->cs_assign.ctp_any_assign;
  case DeeType_SLOT_ID(tp_copy_assign):
   return suffix->cs_assign.ctp_copy_assign;
  case DeeType_SLOT_ID(tp_move_assign):
   return suffix->cs_assign.ctp_move_assign;
  case DeeType_SLOT_ID(tp_not):
   return suffix->cs_math.ctp_not;
  case DeeType_SLOT_ID(tp_bool):
   return suffix->cs_math.ctp_bool;
  case DeeType_SLOT_ID(tp_int32):
   return suffix->cs_cast.ctp_int32;
  case DeeType_SLOT_ID(tp_int64):
   return suffix->cs_cast.ctp_int64;
  case DeeType_SLOT_ID(tp_double):
   return suffix->cs_cast.ctp_double;
  case DeeType_SLOT_ID(tp_inc):
   return suffix->cs_math.ctp_inc;
  case DeeType_SLOT_ID(tp_incpost):
   return suffix->cs_math.ctp_incpost;
  case DeeType_SLOT_ID(tp_dec):
   return suffix->cs_math.ctp_dec;
  case DeeType_SLOT_ID(tp_decpost):
   return suffix->cs_math.ctp_decpost;
#define OPERATOR_SLOT(tp_name,tp_c_name)\
  case DeeType_SLOT_ID(tp_name): return suffix->tp_c_name
  OPERATOR_SLOT(tp_str,     cs_cast.ctp_str);
  OPERATOR_SLOT(tp_repr,    cs_cast.ctp_repr);
  OPERATOR_SLOT(tp_call,    cs_object.ctp_call);
  OPERATOR_SLOT(tp_inv,     cs_math.ctp_inv);
  OPERATOR_SLOT(tp_pos,     cs_math.ctp_pos);
  OPERATOR_SLOT(tp_neg,     cs_math.ctp_neg);
  OPERATOR_SLOT(tp_add,     cs_math.ctp_add);
  OPERATOR_SLOT(tp_iadd,    cs_math.ctp_iadd);
  OPERATOR_SLOT(tp_sub,     cs_math.ctp_sub);
  OPERATOR_SLOT(tp_isub,    cs_math.ctp_isub);
  OPERATOR_SLOT(tp_mul,     cs_math.ctp_mul);
  OPERATOR_SLOT(tp_imul,    cs_math.ctp_imul);
  OPERATOR_SLOT(tp_div,     cs_math.ctp_div);
  OPERATOR_SLOT(tp_idiv,    cs_math.ctp_idiv);
  OPERATOR_SLOT(tp_mod,     cs_math.ctp_mod);
  OPERATOR_SLOT(tp_imod,    cs_math.ctp_imod);
  OPERATOR_SLOT(tp_shl,     cs_math.ctp_shl);
  OPERATOR_SLOT(tp_ishl,    cs_math.ctp_ishl);
  OPERATOR_SLOT(tp_shr,     cs_math.ctp_shr);
  OPERATOR_SLOT(tp_ishr,    cs_math.ctp_ishr);
  OPERATOR_SLOT(tp_and,     cs_math.ctp_and);
  OPERATOR_SLOT(tp_iand,    cs_math.ctp_iand);
  OPERATOR_SLOT(tp_or,      cs_math.ctp_or);
  OPERATOR_SLOT(tp_ior,     cs_math.ctp_ior);
  OPERATOR_SLOT(tp_xor,     cs_math.ctp_xor);
  OPERATOR_SLOT(tp_ixor,    cs_math.ctp_ixor);
  OPERATOR_SLOT(tp_pow,     cs_math.ctp_pow);
  OPERATOR_SLOT(tp_ipow,    cs_math.ctp_ipow);
  OPERATOR_SLOT(tp_hash,    cs_math.ctp_hash);
  OPERATOR_SLOT(tp_cmp_lo,  cs_compare.ctp_cmp_lo);
  OPERATOR_SLOT(tp_cmp_le,  cs_compare.ctp_cmp_le);
  OPERATOR_SLOT(tp_cmp_eq,  cs_compare.ctp_cmp_eq);
  OPERATOR_SLOT(tp_cmp_ne,  cs_compare.ctp_cmp_ne);
  OPERATOR_SLOT(tp_cmp_gr,  cs_compare.ctp_cmp_gr);
  OPERATOR_SLOT(tp_cmp_ge,  cs_compare.ctp_cmp_ge);
  OPERATOR_SLOT(tp_seq_get, cs_seq.ctp_seq_get);
  OPERATOR_SLOT(tp_seq_del, cs_seq.ctp_seq_del);
  OPERATOR_SLOT(tp_seq_set, cs_seq.ctp_seq_set);
  OPERATOR_SLOT(tp_seq_size,cs_seq.ctp_seq_size);
  OPERATOR_SLOT(tp_seq_contains,cs_seq.ctp_seq_contains);
  OPERATOR_SLOT(tp_seq_range_get,cs_seq.ctp_seq_range_get);
  OPERATOR_SLOT(tp_seq_range_del,cs_seq.ctp_seq_range_del);
  OPERATOR_SLOT(tp_seq_range_set,cs_seq.ctp_seq_range_set);
  OPERATOR_SLOT(tp_seq_iter_next,cs_seq.ctp_seq_iter_next);
#undef OPERATOR_SLOT
  case DeeType_SLOT_ID(tp_attr_get): return suffix->cs_attr.ctp_attr_get;
  case DeeType_SLOT_ID(tp_attr_del): return suffix->cs_attr.ctp_attr_del;
  case DeeType_SLOT_ID(tp_attr_set): return suffix->cs_attr.ctp_attr_set;
#endif /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
  case DeeType_SLOT_ID(tp_seq_iter_self):
   if (DeeType_GET_SLOT(self,tp_seq_iter_self) == (DeeObject*(*)(
    DeeObject*))&_deeinstance_tp_seq_iter_self)
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
    return suffix->cs_seq.ctp_seq_iter_self;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
    return DeeClassSuffix_GetVirtOperator(suffix,DeeType_SLOT_ID(tp_seq_iter_self));
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
   break;
  case DEE_CLASS_SLOTID_OPERATOR_FOR:
   if (DeeType_GET_SLOT(self,tp_seq_iter_self) == (DeeObject*(*)(
    DeeObject*))&_deeinstance_tp_seq_iter_self_callfor)
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
    return suffix->cs_seq.ctp_seq_iter_self;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
    return DeeClassSuffix_GetVirtOperator(suffix,DeeType_SLOT_ID(tp_seq_iter_self));
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
   break;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  case DeeType_SLOT_ID(tp_io_read):
   if (!DeeFileClass_Check(self)) return NULL;
   if (DeeType_GET_SLOT(self,tp_io_read) != &_deefileinstance_tp_io_read) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
   return ((struct DeeFileClassSuffix *)suffix)->tp_file.ctp_io_read;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
   return DeeClassSuffix_GetVirtOperator(suffix,DeeType_SLOT_ID(tp_io_read));
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
  case DeeType_SLOT_ID(tp_io_write):
   if (!DeeFileClass_Check(self)) return NULL;
   if (DeeType_GET_SLOT(self,tp_io_write) != &_deefileinstance_tp_io_write) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
   return ((struct DeeFileClassSuffix *)suffix)->tp_file.ctp_io_write;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
   return DeeClassSuffix_GetVirtOperator(suffix,DeeType_SLOT_ID(tp_io_write));
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */

  case DEE_CLASS_SLOTID_OPERATOR_READNP:
   if (!DeeFileClass_Check(self)) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   if (DeeType_GET_SLOT(self,tp_io_read) != &_deefileinstance_tp_io_read_np) return NULL;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
   return ((struct DeeFileClassSuffix *)suffix)->tp_file.ctp_io_read;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
   return DeeClassSuffix_GetVirtOperator(suffix,DeeType_SLOT_ID(tp_io_read));
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
  case DEE_CLASS_SLOTID_OPERATOR_WRITENP:
   if (!DeeFileClass_Check(self)) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   if (DeeType_GET_SLOT(self,tp_io_write) != &_deefileinstance_tp_io_write_np) return NULL;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
   return ((struct DeeFileClassSuffix *)suffix)->tp_file.ctp_io_write;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
   return DeeClassSuffix_GetVirtOperator(suffix,DeeType_SLOT_ID(tp_io_write));
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
#define FILE_OPERATOR_SLOT(tp_name,tp_c_name)\
  case DeeType_SLOT_ID(tp_name):\
   if (!DeeFileClass_Check(self)) return NULL;\
   return ((struct DeeFileClassSuffix *)suffix)->tp_c_name
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
#define FILE_OPERATOR_SLOT(tp_name,tp_c_name)\
  case DeeType_SLOT_ID(tp_name):\
   if (!DeeFileClass_Check(self)) return NULL;\
   return DeeClassSuffix_GetVirtOperator(suffix,DeeType_SLOT_ID(tp_name));
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
  FILE_OPERATOR_SLOT(tp_io_seek,tp_file.ctp_io_seek);
  FILE_OPERATOR_SLOT(tp_io_flush,tp_file.ctp_io_flush);
  FILE_OPERATOR_SLOT(tp_io_trunc,tp_file.ctp_io_trunc);
  FILE_OPERATOR_SLOT(tp_io_close,tp_file.ctp_io_close);
#undef FILE_OPERATOR_SLOT
  default:
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
   break;
#else
   return DeeClassSuffix_GetVirtOperator(suffix,slot);
#endif
 }
 return NULL;
}
#ifndef __INTELLISENSE__
#define VALUE
#include "class.slot_linker.inl"
#include "class.slot_linker.inl"
#endif


static int DEE_CALL _deeinstance_tp_marshal_ctor(
 DeeClassObject *tp_self, DeeInstanceObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 DeeTypeObject *self_base; struct DeeInstanceLevel *my_level;
 Dee_uint64_t read_memberc; Dee_size_t memberc; int temp;
 my_level = DeeInstance_MEMBERS_EX(tp_self,self);
 _DeeInstanceLevel_Init(my_level);
 DeeHashMap_Init(&my_level->il_members);
 self_base = DeeType_BASE(tp_self);
 if (self_base != &DeeObject_Type) {
  struct DeeTypeMarshal *start_marshal;
  start_marshal = DeeType_GET_SLOT(self_base,tp_marshal);
  if (!start_marshal) {
   DeeError_SetStringf(&DeeErrorType_NotImplemented,
                       "Instance base %q cannot be marshaled",
                       DeeType_NAME(self_base));
err_base_init:
   DeeHashMap_Quit(&my_level->il_members);
   return -1;
  }
  DEE_ASSERT(start_marshal->tp_marshal_ctor_);
  if ((*start_marshal->tp_marshal_ctor_)(self_base,(
   DeeObject *)self,infile,map) != 0) goto err_base_init;
 }
 if (DeeFile_GetLeSmall64(infile,&read_memberc) != 0) {
err: temp = _DeeObject_DestructChain(self_base,(DeeObject *)self);
  DeeHashMap_Quit(&my_level->il_members);
  return temp;
 }
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_memberc > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Class member count is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_memberc,(Dee_size_t)-1);
  goto err;
 }
#endif
 memberc = (Dee_size_t)read_memberc;
 while (memberc--) {
  DeeObject *key,*item; int temp;
  if ((key = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) goto err;
  if ((item = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {
/*err_self_key:*/ Dee_DECREF(key); goto err;
  }
  temp = DeeHashMap_SetItem(&my_level->il_members,key,item);
  Dee_DECREF(item);
  Dee_DECREF(key);
  if (temp != 0) goto err;
 }
 return 0;
}

static int DEE_CALL _deeinstance_tp_marshal_put(
 DeeClassObject *tp_self, DeeInstanceObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 struct DeeInstanceLevel *my_level; DeeObject *key,*item;
 struct _DeeHashMapNode *node; DeeHashMap_TRAVERSE_SAFE_VARS;
 Dee_size_t memberc; int temp;
 // v recursively write data from all underlying levels / objects
 if (DeeType_BASE(tp_self) != &DeeObject_Type) {
  if (DeeMarshal_TWriteObjectWithMap(outfile,map,
   DeeType_BASE(tp_self),(DeeObject *)self) != 0) return -1;
 }
 my_level = DeeInstance_MEMBERS_EX(tp_self,self);
 DeeAtomicMutex_Acquire(&my_level->il_lock);
 memberc = DeeHashMap_SIZE(&my_level->il_members);
 DeeAtomicMutex_Release(&my_level->il_lock);
 if (DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)memberc) != 0) return -1;
 DeeAtomicMutex_Acquire(&my_level->il_lock);
 DeeHashMap_TRAVERSE_SAFE(node,&my_level->il_members) {
  Dee_INCREF(key = node->hmn_key);
  Dee_INCREF(item = node->hmn_item);
  DeeAtomicMutex_Release(&my_level->il_lock);
  temp = DeeMarshal_WriteObjectWithMap(outfile,map,key);
  if (temp == 0) temp = DeeMarshal_WriteObjectWithMap(outfile,map,item);
  Dee_DECREF(item);
  Dee_DECREF(key);
  if (temp != 0) return temp;
  --memberc;
  DeeAtomicMutex_Acquire(&my_level->il_lock);
 }
 DeeAtomicMutex_Release(&my_level->il_lock);
 // Write members that have gone missing (due to thread-safety as 'none')
 while (memberc--) {
  if (DeeMarshal_WriteObjectWithMap(outfile,map,Dee_None) != 0 ||
      DeeMarshal_WriteObjectWithMap(outfile,map,Dee_None) != 0) return -1;
 }
 return 0;
}

static struct DeeTypeMarshal _deeinstance_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALID_INSTANCE),
 member(&_deeinstance_tp_marshal_ctor),member(&_deeinstance_tp_marshal_put));

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeClass_Init(
 DEE_A_INOUT DeeClassTypeObject *class_type, DEE_A_INOUT DeeClassObject *self,
 DEE_A_IN DeeTypeObject const *base, DEE_A_IN_Z_OPT char const *name,
 DEE_A_IN_OPT struct DeeUUID const *uuid) {
 struct DeeClassSuffix *suffix;
 char const *error_name = name ? name : "<unnamed>";
 DeeTypeObject *class_type_base;
 DEE_ASSERT(DeeType_IsSameOrDerived(
  (DeeTypeObject *)class_type,Dee_TYPE(base)) && "Incompatible class type");
 // Make sure that 'base' can be used as a base-class
 if (DeeType_IsFinal(base)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Can't use final type %q as base for class %s",
                      DeeType_NAME(base),error_name);
  return -1;
 }
 if (DeeType_IsVarType(base)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Cannot use %k as base for %s: base describes variable objects",
                      base,error_name);
  return -1;
 }
 if (DeeStructuredType_Check(base)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Cannot use %k as base for %s: base is a structured type",
                      base,error_name);
  return -1;
 }
 if ((DeeType_GET_SLOT(base,tp_alloc) != &_DeeGC_TpAlloc &&
      DeeType_GET_SLOT(base,tp_alloc) != DeeType_DEFAULT_SLOT(tp_alloc)) ||
     (DeeType_GET_SLOT(base,tp_free) != &_DeeGC_TpFree &&
      DeeType_GET_SLOT(base,tp_free) != DeeType_DEFAULT_SLOT(tp_free))) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Cannot use %k as base for %s: base has incompatible alloc/free functions",
                      base,error_name);
  return -1;
 }
 // Initialize the type base
 class_type_base = DeeType_BASE(class_type);
 if ((*DeeType_GET_SLOT(class_type_base,tp_ctor))(
  (DeeTypeObject *)class_type_base,(DeeObject *)self) != 0) return -1;
 if (DeeType_SetName((DeeTypeObject *)self,name) != 0) {
err_base_dtor:
  return _DeeObject_DestructChain(class_type_base,(DeeObject *)self);
 }
 suffix = DeeClass_SUFFIX(self);
 DEE_ASSERTF(DeeType_GET_SLOT(class_type,tp_instance_size) >=
             DeeType_GET_SLOT(class_type_base,tp_instance_size),
             "class_type base is bigger than the class_type");
 DeeAnyClassSuffix_Init(Dee_TYPE(base),suffix);
 suffix->cs_methods = _deeclass_empty_methods;
 suffix->cs_getsets = _deeclass_empty_getsets;
 suffix->cs_class_methods = _deeclass_empty_methods;
 suffix->cs_class_getsets = _deeclass_empty_getsets;
 DeeType_GET_SLOT(self,tp_flags) |= DEE_TYPE_FLAG_CLASS_TYPE|DEE_TYPE_FLAG_HAS_GC;
 DeeType_GET_SLOT(self,tp_alloc) = &_DeeGC_TpAlloc;
 DeeType_GET_SLOT(self,tp_free) = &_DeeGC_TpFree;
 DeeType_GET_SLOT(self,tp_instance_size) = (
  suffix->cs_member_offset = DeeType_GET_SLOT(base,tp_instance_size))+sizeof(struct DeeInstanceLevel);
 DeeType_GET_SLOT(self,tp_ctor) = (int(*)(DeeTypeObject*,DeeObject*))DeeClass_GetNewInstance_tp_ctor_builtin(base);
 DeeType_GET_SLOT(self,tp_copy_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_copy_ctor_builtin;
 DeeType_GET_SLOT(self,tp_move_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_move_ctor_builtin;
 DeeType_GET_SLOT(self,tp_any_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_builtin;
 DeeType_GET_SLOT(self,tp_clear) = (void(*)(DeeObject*))&_deeinstance_tp_clear_builtin;
 DeeType_GET_SLOT(self,tp_dtor) = (void(*)(DeeObject*))&_deeinstance_tp_dtor;
 DeeType_GET_SLOT(self,tp_visit) = (DeeType_SLOT_TYPE(tp_visit))&_deeinstance_tp_visit;
 DeeType_GET_SLOT(self,tp_hash) = (int(*)(DeeObject*,Dee_hash_t,Dee_hash_t*))&_deeinstance_tp_hash;
 DeeType_GET_SLOT(self,tp_attr_get) = (DeeObject*(*)(DeeObject*,DeeObject*))&DeeInstance_GetAttr;
 DeeType_GET_SLOT(self,tp_attr_del) = (int(*)(DeeObject*,DeeObject*))&DeeInstance_DelAttr;
 DeeType_GET_SLOT(self,tp_attr_set) = (int(*)(DeeObject*,DeeObject*,DeeObject*))&DeeInstance_SetAttr;
 Dee_XDECREF(DeeType_GET_SLOT(self,tp_base));
 Dee_INCREF(DeeType_GET_SLOT(self,tp_base) = (DeeTypeObject *)base);
 if (uuid) {
  // Register a new type by uuid
  struct DeeTypeMarshal *type_marshal;
  while ((type_marshal = (struct DeeTypeMarshal *)
   malloc_nz(sizeof(struct DeeTypeMarshal))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
err_name:
   free((void *)DeeType_GET_SLOT(self,tp_name));
   goto err_base_dtor;
  }
  type_marshal->tp_uuid = *uuid;
  type_marshal->tp_marshal_ctor_ = (int(DEE_CALL *)(DeeTypeObject*,DeeObject*,DeeObject*,struct DeeMarshalReadMap *))&_deeinstance_tp_marshal_ctor;
  type_marshal->tp_marshal_put = (int(DEE_CALL *)(DeeTypeObject*,DeeObject*,DeeObject*,struct DeeMarshalWriteMap *))&_deeinstance_tp_marshal_put;
  type_marshal->tp_alias_uuids = NULL;
  DeeType_GET_SLOT(self,tp_marshal) = type_marshal;
  if (DeeMarshal_RegisterType((DeeTypeObject *)self) != 0) {
   free_nn(type_marshal);
   goto err_name;
  }
 } else {
  // Generic class type (without uuid, the type will be written with every instance)
  DeeType_GET_SLOT(self,tp_marshal) = &_deeinstance_tp_marshal;
 }
 return 0;
}
DEE_FUNC_DECL(DEE_A_RET_TYPEOBJECT_EXCEPT_REF(DeeClassObject) *) DeeClass_NewEx(
 DEE_A_IN DeeTypeObject const *base, DEE_A_IN_Z_OPT char const *name,
 DEE_A_IN_OPT struct DeeUUID const *uuid) {
 DeeClassTypeObject *class_type; DeeClassObject *result;
 DEE_ASSERT(DeeObject_Check(base) && DeeType_Check(base));
 if ((DeeType_FLAGS(base)&DEE_TYPE_FLAG_INCOMPLETE)!=0) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Can't use incomplete type %q as class base",
                      DeeType_NAME(base));
  return NULL;
 }
 if ((class_type = DeeClassType_TypeOf(base)) == NULL) return NULL;
 result = (DeeClassObject *)(*DeeType_GET_SLOT(class_type,tp_alloc))((DeeTypeObject *)class_type);
 Dee_DECREF(class_type);
 if (_DeeClass_Init(class_type,result,base,name,uuid) != 0) {
  _DeeObject_DELETE((DeeTypeObject *)class_type,(DeeObject *)result);
  result = NULL;
 } else DeeGC_TrackedAdd((DeeObject *)result);
 return (DeeTypeObject *)result;
}
DEE_FUNC_DECL(DEE_A_RET_TYPEOBJECT_EXCEPT_REF(DeeClassObject) *) DeeClass_NewObjectEx(
 DEE_A_IN DeeTypeObject const *base, DEE_A_IN_OBJECT_OPT(DeeAnyStringObject) const *name,
 DEE_A_IN_OPT struct DeeUUID const *uuid) {
 DeeTypeObject *result;
 if (name) {
  DEE_ASSERT(DeeObject_Check(name));
  if DEE_UNLIKELY((name = DeeUtf8String_Cast(name)) == NULL) return NULL;
  result = DeeClass_NewEx(base,DeeUtf8String_STR(name),uuid);
  Dee_DECREF(name);
  return result;
 }
 return DeeClass_NewEx(base,NULL,uuid);
}




//////////////////////////////////////////////////////////////////////////
// Class VTable
static int _deeclass_tp_any_ctor(
 DeeClassTypeObject *tp, DeeClassObject *self, DeeObject *args) {
 DeeTypeObject *base = &DeeObject_Type;
 DeeObject *name = Dee_EmptyString; int result;
 if (DeeTuple_Unpack(args,"o|o",&base,&name) != 0) return -1;
 if (!DeeType_IsSameOrDerived((DeeTypeObject *)tp,Dee_TYPE(base))) {
  // Invalid class-type for this kind of base class
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "classtype %k cannot be used for base %k of type %k",
                      tp,base,Dee_TYPE(base));
  return -1;
 }
 if ((name = DeeString_Cast(name)) == NULL) return -1;
 result = _DeeClass_Init(tp,self,base,DeeString_STR(name),NULL);
 Dee_DECREF(name);
 return result;
}
static int _deeclass_tp_ctor(DeeClassTypeObject *tp, DeeClassObject *self) {
 return _deeclass_tp_any_ctor(tp,self,Dee_EmptyTuple);
}
static void _deeclass_delete_methods(
 struct _DeeClassTypeMethodDef *methods) {
 struct _DeeClassTypeMethodDef *iter;
 DEE_ASSERT(methods);
 iter = methods;
 while (iter->ctmd_name) {
  Dee_DECREF(iter->ctmd_name);
  Dee_DECREF(iter->ctmd_func);
  ++iter;
 }
 free_nn(methods);
}
static void _deeclass_delete_getsets(
 struct _DeeClassTypeGetSetDef *getsets) {
 struct _DeeClassTypeGetSetDef *iter;
 DEE_ASSERT(getsets);
 iter = getsets;
 while (iter->ctgd_name) {
  Dee_DECREF(iter->ctgd_name);
  Dee_XDECREF(iter->ctgd_get);
  Dee_XDECREF(iter->ctgd_del);
  Dee_XDECREF(iter->ctgd_set);
  ++iter;
 }
 free_nn(getsets);
}
static void DEE_CALL _deeclass_tp_dtor(DeeClassObject *self) {
 struct DeeClassSuffix *suffix;
 struct DeeTypeMarshal *marshal;
 marshal = DeeType_GET_SLOT(self,tp_marshal);
 if (marshal && marshal != &_deeinstance_tp_marshal) {
  // v must unregister the marshal before we try to free it!
  //   NOTE: If we don't do it here, it'll be done later (in the type destructor)
  DeeMarshal_UnregisterType((DeeTypeObject *)self);
  free_nn(marshal); // Free a special marshal declaration
 }
 suffix = DeeClass_SUFFIX(self);
 if (suffix->cs_methods != _deeclass_empty_methods) _deeclass_delete_methods(suffix->cs_methods);
 if (suffix->cs_getsets != _deeclass_empty_getsets) _deeclass_delete_getsets(suffix->cs_getsets);
 if (suffix->cs_class_methods != _deeclass_empty_methods) _deeclass_delete_methods(suffix->cs_class_methods);
 if (suffix->cs_class_getsets != _deeclass_empty_getsets) _deeclass_delete_getsets(suffix->cs_class_getsets);
 DeeAnyClassSuffix_Quit(Dee_TYPE(self),suffix);
}
static void DEE_CALL _deeclass_tp_clear(DeeClassObject *self) {
 struct DeeClassSuffix *suffix;
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 suffix = DeeClass_SUFFIX(self);
 if (suffix->cs_methods != _deeclass_empty_methods) {
  _deeclass_delete_methods(suffix->cs_methods);
  suffix->cs_methods = _deeclass_empty_methods;
 }
 if (suffix->cs_getsets != _deeclass_empty_getsets) {
  _deeclass_delete_getsets(suffix->cs_getsets);
  suffix->cs_getsets = _deeclass_empty_getsets;
 }
 if (suffix->cs_class_methods != _deeclass_empty_methods) {
  _deeclass_delete_methods(suffix->cs_class_methods);
  suffix->cs_class_methods = _deeclass_empty_methods;
 }
 if (suffix->cs_class_getsets != _deeclass_empty_getsets) {
  _deeclass_delete_getsets(suffix->cs_class_getsets);
  suffix->cs_class_getsets = _deeclass_empty_getsets;
 }
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
#define CLEAR_SLOT(name,slot)\
do{\
 if (slot) {\
  Dee_CLEAR(slot);\
  DeeType_GET_SLOT(self,name) = DeeType_DEFAULT_SLOT(name);\
 }\
}while(0)

 if (suffix->cs_constructor.ctp_ctor) {
  Dee_CLEAR(suffix->cs_constructor.ctp_ctor);
  DeeType_GET_SLOT(self,tp_ctor) = (int(*)(DeeTypeObject*,DeeObject*))&_deeinstance_tp_copy_ctor_builtin;
  Dee_XCLEAR(suffix->cs_constructor.ctp_ctor_superargs);
 } else if (suffix->cs_constructor.ctp_ctor_superargs) {
  Dee_CLEAR(suffix->cs_constructor.ctp_ctor_superargs);
  DeeType_GET_SLOT(self,tp_ctor) = (int(*)(DeeTypeObject*,DeeObject*))&_deeinstance_tp_copy_ctor_builtin;
 }
 if (suffix->cs_constructor.ctp_copy_ctor) {
  Dee_CLEAR(suffix->cs_constructor.ctp_copy_ctor);
  DeeType_GET_SLOT(self,tp_copy_ctor) = (int(*)(
   DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_copy_ctor_builtin;
 }
 if (suffix->cs_constructor.ctp_move_ctor) {
  Dee_CLEAR(suffix->cs_constructor.ctp_move_ctor);
  DeeType_GET_SLOT(self,tp_move_ctor) = (int(*)(
   DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_move_ctor_builtin;
 }
 if (suffix->cs_constructor.ctp_any_ctor) {
  Dee_CLEAR(suffix->cs_constructor.ctp_any_ctor);
  DeeType_GET_SLOT(self,tp_any_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_builtin;
  Dee_XCLEAR(suffix->cs_constructor.ctp_any_ctor_superargs);
 } else if (suffix->cs_constructor.ctp_any_ctor_superargs) {
  Dee_CLEAR(suffix->cs_constructor.ctp_any_ctor_superargs);
  DeeType_GET_SLOT(self,tp_any_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_builtin;
 }
 if (suffix->cs_destructor.ctp_dtor) {
  Dee_CLEAR(suffix->cs_destructor.ctp_dtor);
  DeeType_GET_SLOT(self,tp_clear) = (void(*)(DeeObject*))&_deeinstance_tp_clear_builtin;
  DeeType_GET_SLOT(self,tp_flags) &= ~DEE_TYPE_FLAG_ALWAYS_CLEAR;
 }
 CLEAR_SLOT(tp_copy_assign,suffix->cs_assign.ctp_copy_assign);
 CLEAR_SLOT(tp_move_assign,suffix->cs_assign.ctp_move_assign);
 CLEAR_SLOT(tp_any_assign,suffix->cs_assign.ctp_any_assign);
 CLEAR_SLOT(tp_str,suffix->cs_cast.ctp_str);
 CLEAR_SLOT(tp_repr,suffix->cs_cast.ctp_repr);
 CLEAR_SLOT(tp_int32,suffix->cs_cast.ctp_int32);
 CLEAR_SLOT(tp_int64,suffix->cs_cast.ctp_int64);
 CLEAR_SLOT(tp_double,suffix->cs_cast.ctp_double);
 CLEAR_SLOT(tp_call,suffix->cs_object.ctp_call);
 CLEAR_SLOT(tp_not,suffix->cs_math.ctp_not);
 CLEAR_SLOT(tp_bool,suffix->cs_math.ctp_bool);
 CLEAR_SLOT(tp_inv,suffix->cs_math.ctp_inv);
 CLEAR_SLOT(tp_pos,suffix->cs_math.ctp_pos);
 CLEAR_SLOT(tp_neg,suffix->cs_math.ctp_neg);
 CLEAR_SLOT(tp_inc,suffix->cs_math.ctp_inc);
 CLEAR_SLOT(tp_incpost,suffix->cs_math.ctp_incpost);
 CLEAR_SLOT(tp_dec,suffix->cs_math.ctp_dec);
 CLEAR_SLOT(tp_decpost,suffix->cs_math.ctp_decpost);
 CLEAR_SLOT(tp_add,suffix->cs_math.ctp_add);
 CLEAR_SLOT(tp_iadd,suffix->cs_math.ctp_iadd);
 CLEAR_SLOT(tp_sub,suffix->cs_math.ctp_sub);
 CLEAR_SLOT(tp_isub,suffix->cs_math.ctp_isub);
 CLEAR_SLOT(tp_mul,suffix->cs_math.ctp_mul);
 CLEAR_SLOT(tp_imul,suffix->cs_math.ctp_imul);
 CLEAR_SLOT(tp_div,suffix->cs_math.ctp_div);
 CLEAR_SLOT(tp_idiv,suffix->cs_math.ctp_idiv);
 CLEAR_SLOT(tp_mod,suffix->cs_math.ctp_mod);
 CLEAR_SLOT(tp_imod,suffix->cs_math.ctp_imod);
 CLEAR_SLOT(tp_shl,suffix->cs_math.ctp_shl);
 CLEAR_SLOT(tp_ishl,suffix->cs_math.ctp_ishl);
 CLEAR_SLOT(tp_shr,suffix->cs_math.ctp_shr);
 CLEAR_SLOT(tp_ishr,suffix->cs_math.ctp_ishr);
 CLEAR_SLOT(tp_and,suffix->cs_math.ctp_and);
 CLEAR_SLOT(tp_iand,suffix->cs_math.ctp_iand);
 CLEAR_SLOT(tp_or,suffix->cs_math.ctp_or);
 CLEAR_SLOT(tp_ior,suffix->cs_math.ctp_ior);
 CLEAR_SLOT(tp_xor,suffix->cs_math.ctp_xor);
 CLEAR_SLOT(tp_ixor,suffix->cs_math.ctp_ixor);
 CLEAR_SLOT(tp_pow,suffix->cs_math.ctp_pow);
 CLEAR_SLOT(tp_ipow,suffix->cs_math.ctp_ipow);
 CLEAR_SLOT(tp_hash,suffix->cs_math.ctp_hash);
 CLEAR_SLOT(tp_cmp_lo,suffix->cs_compare.ctp_cmp_lo);
 CLEAR_SLOT(tp_cmp_le,suffix->cs_compare.ctp_cmp_le);
 CLEAR_SLOT(tp_cmp_eq,suffix->cs_compare.ctp_cmp_eq);
 CLEAR_SLOT(tp_cmp_ne,suffix->cs_compare.ctp_cmp_ne);
 CLEAR_SLOT(tp_cmp_gr,suffix->cs_compare.ctp_cmp_gr);
 CLEAR_SLOT(tp_cmp_ge,suffix->cs_compare.ctp_cmp_ge);
 CLEAR_SLOT(tp_seq_get,suffix->cs_seq.ctp_seq_get);
 CLEAR_SLOT(tp_seq_del,suffix->cs_seq.ctp_seq_del);
 CLEAR_SLOT(tp_seq_set,suffix->cs_seq.ctp_seq_set);
 CLEAR_SLOT(tp_seq_size,suffix->cs_seq.ctp_seq_size);
 CLEAR_SLOT(tp_seq_contains,suffix->cs_seq.ctp_seq_contains);
 CLEAR_SLOT(tp_seq_range_get,suffix->cs_seq.ctp_seq_range_get);
 CLEAR_SLOT(tp_seq_range_del,suffix->cs_seq.ctp_seq_range_del);
 CLEAR_SLOT(tp_seq_range_set,suffix->cs_seq.ctp_seq_range_set);
 CLEAR_SLOT(tp_seq_iter_self,suffix->cs_seq.ctp_seq_iter_self);
 CLEAR_SLOT(tp_seq_iter_next,suffix->cs_seq.ctp_seq_iter_next);
 CLEAR_SLOT(tp_attr_get,suffix->cs_attr.ctp_attr_get);
 CLEAR_SLOT(tp_attr_del,suffix->cs_attr.ctp_attr_del);
 CLEAR_SLOT(tp_attr_set,suffix->cs_attr.ctp_attr_set);
 if (DeeType_IsDerived(Dee_TYPE(self),&DeeFileType_Type)) {
  CLEAR_SLOT(tp_io_read,((struct DeeFileClassSuffix *)suffix)->tp_file.ctp_io_read);
  CLEAR_SLOT(tp_io_write,((struct DeeFileClassSuffix *)suffix)->tp_file.ctp_io_write);
  CLEAR_SLOT(tp_io_seek,((struct DeeFileClassSuffix *)suffix)->tp_file.ctp_io_seek);
  CLEAR_SLOT(tp_io_flush,((struct DeeFileClassSuffix *)suffix)->tp_file.ctp_io_flush);
  CLEAR_SLOT(tp_io_trunc,((struct DeeFileClassSuffix *)suffix)->tp_file.ctp_io_trunc);
  CLEAR_SLOT(tp_io_close,((struct DeeFileClassSuffix *)suffix)->tp_file.ctp_io_close);
 }
#undef CLEAR_SLOT
#else
 _DeeClassDynamicVTableList_Clear(&suffix->cs_vtable);
 DeeType_GET_SLOT(self,tp_ctor) = (int(*)(DeeTypeObject*,DeeObject*))&_deeinstance_tp_ctor_builtin;
 DeeType_GET_SLOT(self,tp_copy_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_copy_ctor_builtin;
 DeeType_GET_SLOT(self,tp_move_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_move_ctor_builtin;
 DeeType_GET_SLOT(self,tp_any_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeinstance_tp_any_ctor_builtin;
 DeeType_GET_SLOT(self,tp_clear) = (void(*)(DeeObject*))&_deeinstance_tp_clear_builtin;
 DeeType_GET_SLOT(self,tp_flags) &= ~DEE_TYPE_FLAG_ALWAYS_CLEAR;
 DeeType_GET_SLOT(self,tp_copy_assign) = DeeType_DEFAULT_SLOT(tp_copy_assign);
 DeeType_GET_SLOT(self,tp_move_assign) = DeeType_DEFAULT_SLOT(tp_move_assign);
 DeeType_GET_SLOT(self,tp_any_assign) = DeeType_DEFAULT_SLOT(tp_any_assign);
 DeeType_GET_SLOT(self,tp_str) = DeeType_DEFAULT_SLOT(tp_str);
 DeeType_GET_SLOT(self,tp_repr) = DeeType_DEFAULT_SLOT(tp_repr);
 DeeType_GET_SLOT(self,tp_int32) = DeeType_DEFAULT_SLOT(tp_int32);
 DeeType_GET_SLOT(self,tp_int64) = DeeType_DEFAULT_SLOT(tp_int64);
 DeeType_GET_SLOT(self,tp_double) = DeeType_DEFAULT_SLOT(tp_double);
 DeeType_GET_SLOT(self,tp_call) = DeeType_DEFAULT_SLOT(tp_call);
 DeeType_GET_SLOT(self,tp_not) = DeeType_DEFAULT_SLOT(tp_not);
 DeeType_GET_SLOT(self,tp_bool) = DeeType_DEFAULT_SLOT(tp_bool);
 DeeType_GET_SLOT(self,tp_inv) = DeeType_DEFAULT_SLOT(tp_inv);
 DeeType_GET_SLOT(self,tp_pos) = DeeType_DEFAULT_SLOT(tp_pos);
 DeeType_GET_SLOT(self,tp_neg) = DeeType_DEFAULT_SLOT(tp_neg);
 DeeType_GET_SLOT(self,tp_inc) = DeeType_DEFAULT_SLOT(tp_inc);
 DeeType_GET_SLOT(self,tp_incpost) = DeeType_DEFAULT_SLOT(tp_incpost);
 DeeType_GET_SLOT(self,tp_dec) = DeeType_DEFAULT_SLOT(tp_dec);
 DeeType_GET_SLOT(self,tp_decpost) = DeeType_DEFAULT_SLOT(tp_decpost);
 DeeType_GET_SLOT(self,tp_add) = DeeType_DEFAULT_SLOT(tp_add);
 DeeType_GET_SLOT(self,tp_iadd) = DeeType_DEFAULT_SLOT(tp_iadd);
 DeeType_GET_SLOT(self,tp_sub) = DeeType_DEFAULT_SLOT(tp_sub);
 DeeType_GET_SLOT(self,tp_isub) = DeeType_DEFAULT_SLOT(tp_isub);
 DeeType_GET_SLOT(self,tp_mul) = DeeType_DEFAULT_SLOT(tp_mul);
 DeeType_GET_SLOT(self,tp_imul) = DeeType_DEFAULT_SLOT(tp_imul);
 DeeType_GET_SLOT(self,tp_div) = DeeType_DEFAULT_SLOT(tp_div);
 DeeType_GET_SLOT(self,tp_idiv) = DeeType_DEFAULT_SLOT(tp_idiv);
 DeeType_GET_SLOT(self,tp_mod) = DeeType_DEFAULT_SLOT(tp_mod);
 DeeType_GET_SLOT(self,tp_imod) = DeeType_DEFAULT_SLOT(tp_imod);
 DeeType_GET_SLOT(self,tp_shl) = DeeType_DEFAULT_SLOT(tp_shl);
 DeeType_GET_SLOT(self,tp_ishl) = DeeType_DEFAULT_SLOT(tp_ishl);
 DeeType_GET_SLOT(self,tp_shr) = DeeType_DEFAULT_SLOT(tp_shr);
 DeeType_GET_SLOT(self,tp_ishr) = DeeType_DEFAULT_SLOT(tp_ishr);
 DeeType_GET_SLOT(self,tp_and) = DeeType_DEFAULT_SLOT(tp_and);
 DeeType_GET_SLOT(self,tp_iand) = DeeType_DEFAULT_SLOT(tp_iand);
 DeeType_GET_SLOT(self,tp_or) = DeeType_DEFAULT_SLOT(tp_or);
 DeeType_GET_SLOT(self,tp_ior) = DeeType_DEFAULT_SLOT(tp_ior);
 DeeType_GET_SLOT(self,tp_xor) = DeeType_DEFAULT_SLOT(tp_xor);
 DeeType_GET_SLOT(self,tp_ixor) = DeeType_DEFAULT_SLOT(tp_ixor);
 DeeType_GET_SLOT(self,tp_pow) = DeeType_DEFAULT_SLOT(tp_pow);
 DeeType_GET_SLOT(self,tp_ipow) = DeeType_DEFAULT_SLOT(tp_ipow);
 DeeType_GET_SLOT(self,tp_hash) = DeeType_DEFAULT_SLOT(tp_hash);
 DeeType_GET_SLOT(self,tp_cmp_lo) = DeeType_DEFAULT_SLOT(tp_cmp_lo);
 DeeType_GET_SLOT(self,tp_cmp_le) = DeeType_DEFAULT_SLOT(tp_cmp_le);
 DeeType_GET_SLOT(self,tp_cmp_eq) = DeeType_DEFAULT_SLOT(tp_cmp_eq);
 DeeType_GET_SLOT(self,tp_cmp_ne) = DeeType_DEFAULT_SLOT(tp_cmp_ne);
 DeeType_GET_SLOT(self,tp_cmp_gr) = DeeType_DEFAULT_SLOT(tp_cmp_gr);
 DeeType_GET_SLOT(self,tp_cmp_ge) = DeeType_DEFAULT_SLOT(tp_cmp_ge);
 DeeType_GET_SLOT(self,tp_seq_get) = DeeType_DEFAULT_SLOT(tp_seq_get);
 DeeType_GET_SLOT(self,tp_seq_del) = DeeType_DEFAULT_SLOT(tp_seq_del);
 DeeType_GET_SLOT(self,tp_seq_set) = DeeType_DEFAULT_SLOT(tp_seq_set);
 DeeType_GET_SLOT(self,tp_seq_size) = DeeType_DEFAULT_SLOT(tp_seq_size);
 DeeType_GET_SLOT(self,tp_seq_contains) = DeeType_DEFAULT_SLOT(tp_seq_contains);
 DeeType_GET_SLOT(self,tp_seq_range_get) = DeeType_DEFAULT_SLOT(tp_seq_range_get);
 DeeType_GET_SLOT(self,tp_seq_range_del) = DeeType_DEFAULT_SLOT(tp_seq_range_del);
 DeeType_GET_SLOT(self,tp_seq_range_set) = DeeType_DEFAULT_SLOT(tp_seq_range_set);
 DeeType_GET_SLOT(self,tp_seq_iter_self) = DeeType_DEFAULT_SLOT(tp_seq_iter_self);
 DeeType_GET_SLOT(self,tp_seq_iter_next) = DeeType_DEFAULT_SLOT(tp_seq_iter_next);
 DeeType_GET_SLOT(self,tp_attr_get) = DeeType_DEFAULT_SLOT(tp_attr_get);
 DeeType_GET_SLOT(self,tp_attr_del) = DeeType_DEFAULT_SLOT(tp_attr_del);
 DeeType_GET_SLOT(self,tp_attr_set) = DeeType_DEFAULT_SLOT(tp_attr_set);
 if (DeeType_IsDerived(Dee_TYPE(self),&DeeFileType_Type)) {
  DeeType_GET_SLOT(self,tp_io_read) = DeeType_DEFAULT_SLOT(tp_io_read);
  DeeType_GET_SLOT(self,tp_io_write) = DeeType_DEFAULT_SLOT(tp_io_write);
  DeeType_GET_SLOT(self,tp_io_seek) = DeeType_DEFAULT_SLOT(tp_io_seek);
  DeeType_GET_SLOT(self,tp_io_flush) = DeeType_DEFAULT_SLOT(tp_io_flush);
  DeeType_GET_SLOT(self,tp_io_trunc) = DeeType_DEFAULT_SLOT(tp_io_trunc);
  DeeType_GET_SLOT(self,tp_io_close) = DeeType_DEFAULT_SLOT(tp_io_close);
 }
#endif
}
DEE_VISIT_PROC_EX(_deeclass_tp_visit,DeeClassTypeObject *tp_self,DeeClassObject *self) {
 struct DeeClassSuffix *suffix;
 struct _DeeClassTypeMethodDef *methods;
 struct _DeeClassTypeGetSetDef *getsets;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeClassType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 (void)tp_self;
 suffix = DeeClass_SUFFIX(self);
 methods = suffix->cs_methods;
 while (methods->ctmd_name) {
  Dee_VISIT(methods->ctmd_name);
  Dee_VISIT(methods->ctmd_func);
  ++methods;
 }
 methods = suffix->cs_class_methods;
 while (methods->ctmd_name) {
  Dee_VISIT(methods->ctmd_name);
  Dee_VISIT(methods->ctmd_func);
  ++methods;
 }
 getsets = suffix->cs_getsets;
 while (getsets->ctgd_name) {
  Dee_VISIT(getsets->ctgd_name);
  Dee_XVISIT(getsets->ctgd_get);
  Dee_XVISIT(getsets->ctgd_del);
  Dee_XVISIT(getsets->ctgd_set);
  ++getsets;
 }
 getsets = suffix->cs_class_getsets;
 while (getsets->ctgd_name) {
  Dee_VISIT(getsets->ctgd_name);
  Dee_XVISIT(getsets->ctgd_get);
  Dee_XVISIT(getsets->ctgd_del);
  Dee_XVISIT(getsets->ctgd_set);
  ++getsets;
 }
 DeeAnyClassSuffix_Visit((DeeTypeObject *)tp_self,suffix);
}


static struct _DeeClassTypeMethodDef *_DeeClassTypeMethodDef_GetFromFile(
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 struct _DeeClassTypeMethodDef *result,*iter;
 Dee_uint64_t read_size; Dee_size_t size,name_size;
 if (DeeFile_GetLeSmall64(infile,&read_size) != 0) return NULL;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_size > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Class method count is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_size,(Dee_size_t)-1);
  return NULL;
 }
#endif
 if (!read_size) return _deeclass_empty_methods;
 size = (Dee_size_t)read_size;
 while DEE_UNLIKELY((result = (struct _DeeClassTypeMethodDef *)malloc_nz(
  (size+1)*sizeof(struct _DeeClassTypeMethodDef))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 iter = result;
 while (size--) {
  if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&read_size) != 0) {
err_r:
   while (iter-- != result) {
    Dee_DECREF(iter->ctmd_name);
    Dee_DECREF(iter->ctmd_func);
   }
   free_nn(result);
   return NULL;
  }
#if DEE_TYPES_SIZEOF_SIZE_T < 8
  if DEE_UNLIKELY(read_size > (Dee_uint64_t)((Dee_size_t)-1)) {
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Class method name is too long for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                       read_size,(Dee_size_t)-1);
   return NULL;
  }
#endif
  name_size = (Dee_size_t)read_size;
  if DEE_UNLIKELY((iter->ctmd_name = (DeeStringObject *)DeeString_NewSized(name_size)) == NULL) goto err_r;
  if DEE_UNLIKELY(DeeFile_ReadAll(infile,DeeString_STR(iter->ctmd_name),name_size) != 0) {
err_iter_name_r: Dee_DECREF(iter->ctmd_name); goto err_r;
  }
  if DEE_UNLIKELY((iter->ctmd_func = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) goto err_iter_name_r;
  ++iter;
 }
 iter->ctmd_name = NULL;
#ifdef DEE_DEBUG
 iter->ctmd_func = NULL;
#endif
 return result;
}

static int _DeeClassTypeMethodDef_PutInFile(
 struct _DeeClassTypeMethodDef const *self, DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 struct _DeeClassTypeMethodDef const *iter; Dee_size_t size;
 size = 0; iter = self;
 while (iter->ctmd_name) ++size,++iter;
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)size) != 0) return -1;
 iter = self; while (iter->ctmd_name) {
  DEE_ASSERT(DeeObject_Check(iter->ctmd_name) && DeeString_Check(iter->ctmd_name));
  if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)DeeString_SIZE(iter->ctmd_name)) != 0) return -1;
  if DEE_UNLIKELY(DeeFile_WriteAll(outfile,DeeString_STR(iter->ctmd_name),DeeString_SIZE(iter->ctmd_name)*sizeof(char)) != 0) return -1;
  if DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,iter->ctmd_func) != 0) return -1;
  ++iter;
 }
 return 0;
}

static struct _DeeClassTypeGetSetDef *_DeeClassTypeGetSetDef_GetFromFile(
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 struct _DeeClassTypeGetSetDef *result,*iter;
 Dee_uint64_t read_size; Dee_size_t size,name_size;
 if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&read_size) != 0) return NULL;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_size > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Class method count is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_size,(Dee_size_t)-1);
  return NULL;
 }
#endif
 if (!read_size) return _deeclass_empty_getsets;
 size = (Dee_size_t)read_size;
 while DEE_UNLIKELY((result = (struct _DeeClassTypeGetSetDef *)malloc_nz(
  (size+1)*sizeof(struct _DeeClassTypeGetSetDef))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 iter = result;
 while (size--) {
  if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&read_size) != 0) {
err_r:
   while (iter-- != result) {
    Dee_DECREF(iter->ctgd_name);
    Dee_XDECREF(iter->ctgd_get);
    Dee_XDECREF(iter->ctgd_del);
    Dee_XDECREF(iter->ctgd_set);
   }
   free_nn(result);
   return NULL;
  }
#if DEE_TYPES_SIZEOF_SIZE_T < 8
  if DEE_UNLIKELY(read_size > (Dee_uint64_t)((Dee_size_t)-1)) {
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Class getset name is too long for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                       read_size,(Dee_size_t)-1);
   return NULL;
  }
#endif
  name_size = (Dee_size_t)read_size;
  if DEE_UNLIKELY((iter->ctgd_name = (DeeStringObject *)DeeString_NewSized(name_size)) == NULL) goto err_r;
  if DEE_UNLIKELY(DeeFile_ReadAll(infile,DeeString_STR(iter->ctgd_name),name_size) != 0) {
err_iter_name_r: Dee_DECREF(iter->ctgd_name); goto err_r;
  }
  if DEE_UNLIKELY((iter->ctgd_get = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) goto err_iter_name_r;
  if DEE_UNLIKELY((iter->ctgd_del = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {
err_iter_get_r: Dee_DECREF(iter->ctgd_get); goto err_iter_name_r;
  }
  if DEE_UNLIKELY((iter->ctgd_set = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {
/*err_iter_del_r:*/ Dee_DECREF(iter->ctgd_del); goto err_iter_get_r;
  }
  if (DeeString_CheckEmpty(iter->ctgd_get)) { Dee_DECREF(iter->ctgd_get); iter->ctgd_get = DeeNone_New(); }
  if (DeeString_CheckEmpty(iter->ctgd_del)) { Dee_DECREF(iter->ctgd_del); iter->ctgd_del = DeeNone_New(); }
  if (DeeString_CheckEmpty(iter->ctgd_set)) { Dee_DECREF(iter->ctgd_set); iter->ctgd_set = DeeNone_New(); }
  ++iter;
 }
 iter->ctgd_name = NULL;
#ifdef DEE_DEBUG
 iter->ctgd_get = NULL;
 iter->ctgd_del = NULL;
 iter->ctgd_set = NULL;
#endif
 return result;
}
static int _DeeClassTypeGetSetDef_PutInFile(
 struct _DeeClassTypeGetSetDef const *self, DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 struct _DeeClassTypeGetSetDef const *iter; Dee_size_t size;
 size = 0; iter = self;
 while (iter->ctgd_name) ++size,++iter;
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)size) != 0) return -1;
 iter = self;
 while (iter->ctgd_name) {
  DEE_ASSERT(DeeObject_Check(iter->ctgd_name) && DeeString_Check(iter->ctgd_name));
  if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)DeeString_SIZE(iter->ctgd_name)) != 0) return -1;
  if DEE_UNLIKELY(DeeFile_WriteAll(outfile,DeeString_STR(iter->ctgd_name),DeeString_SIZE(iter->ctgd_name)*sizeof(char)) != 0) return -1;
  if DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,iter->ctgd_get ? iter->ctgd_get : Dee_EmptyString) != 0) return -1;
  if DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,iter->ctgd_del ? iter->ctgd_del : Dee_EmptyString) != 0) return -1;
  if DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,iter->ctgd_set ? iter->ctgd_set : Dee_EmptyString) != 0) return -1;
  ++iter;
 }
 return 0;
}

static int DEE_CALL _deeclass_tp_marshal_ctor(
 DeeClassTypeObject *tp_self, DeeClassObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 struct DeeClassSuffix *suffix; int error;
 Dee_uint16_t slot_id; DeeObject *callback;
 suffix = DeeClass_TSUFFIX(tp_self,self);
 // NOTE: This marshal callback is special, in that 'self'
 //       will already be a valid class type, that is simply
 //       missing its type slots.
 //       This is required because there is no such type as a 'DeeClassType_Type',
 //       that would be required for detecting a class type,
 //       which is response is required because a class can
 //       derive from something like 'DeeFile_Type'.
 //       (an object who's type is larger than 'DeeType_Type')
 if DEE_UNLIKELY((suffix->cs_methods = _DeeClassTypeMethodDef_GetFromFile(infile,map)) == NULL) { suffix->cs_methods = _deeclass_empty_methods; return -1; }
 if DEE_UNLIKELY((suffix->cs_getsets = _DeeClassTypeGetSetDef_GetFromFile(infile,map)) == NULL) {
err_methods: if (suffix->cs_methods != _deeclass_empty_methods) { _deeclass_delete_methods(suffix->cs_methods); suffix->cs_methods = _deeclass_empty_methods; } return -1;
 }
 if DEE_UNLIKELY((suffix->cs_class_methods = _DeeClassTypeMethodDef_GetFromFile(infile,map)) == NULL) {
err_getsets: if (suffix->cs_getsets != _deeclass_empty_getsets) { _deeclass_delete_getsets(suffix->cs_getsets); suffix->cs_getsets = _deeclass_empty_getsets; } goto err_methods;
 }
 if DEE_UNLIKELY((suffix->cs_class_getsets = _DeeClassTypeGetSetDef_GetFromFile(infile,map)) == NULL) {
err_class_methods: if (suffix->cs_class_methods != _deeclass_empty_methods) { _deeclass_delete_methods(suffix->cs_class_methods); suffix->cs_class_methods = _deeclass_empty_methods; } goto err_getsets;
 }
 while (1) {
  if DEE_UNLIKELY(DeeFile_GetLe(infile,slot_id) != 0) {
err_class_getsets: if (suffix->cs_class_getsets != _deeclass_empty_getsets) { _deeclass_delete_getsets(suffix->cs_class_getsets); suffix->cs_class_getsets = _deeclass_empty_getsets; } goto err_class_methods;
  }
  if (!slot_id) break; // End marker
  if DEE_UNLIKELY((callback = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) goto err_class_getsets;
  error = DeeClass_SetSlot((DeeTypeObject *)self,slot_id,callback);
  Dee_DECREF(callback);
  if DEE_UNLIKELY(error != 0) goto err_class_getsets;
 }
 return 0;
}

#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
static int _DeeClass_XWriteSlot(
 int slot, DeeObject *callback,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 Dee_uint16_t written_slot;
 if (!callback) return 0;
 written_slot = (Dee_uint16_t)slot;
 if DEE_UNLIKELY(DeeFile_PutLe(outfile,written_slot) != 0) return -1;
 return DeeMarshal_WriteObjectWithMap(outfile,map,callback);
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */

static int DEE_CALL _deeclass_tp_marshal_put(
 DeeClassTypeObject *tp_self, DeeClassObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 struct DeeClassSuffix *suffix;
 static Dee_uint16_t const end_marker = DEE_BUILTIN_LESWAP16_M(0);
 suffix = DeeClass_TSUFFIX(tp_self,self);
 if DEE_UNLIKELY(_DeeClassTypeMethodDef_PutInFile(suffix->cs_methods,outfile,map) != 0) return -1;
 if DEE_UNLIKELY(_DeeClassTypeGetSetDef_PutInFile(suffix->cs_getsets,outfile,map) != 0) return -1;
 if DEE_UNLIKELY(_DeeClassTypeMethodDef_PutInFile(suffix->cs_class_methods,outfile,map) != 0) return -1;
 if DEE_UNLIKELY(_DeeClassTypeGetSetDef_PutInFile(suffix->cs_class_getsets,outfile,map) != 0) return -1;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
#define WRITE_SLOT_ID(id,val) if DEE_UNLIKELY(_DeeClass_XWriteSlot(id,val,outfile,map)!=0)return -1
#define WRITE_SLOT(id,val) WRITE_SLOT_ID(DeeType_SLOT_ID(id),val)
 WRITE_SLOT(tp_ctor,suffix->cs_constructor.ctp_ctor);
 WRITE_SLOT_ID(DEE_CLASS_SLOTID_SUPERARGS_CTOR,suffix->cs_constructor.ctp_ctor_superargs);
 WRITE_SLOT(tp_copy_ctor,suffix->cs_constructor.ctp_copy_ctor);
 WRITE_SLOT(tp_move_ctor,suffix->cs_constructor.ctp_move_ctor);
 WRITE_SLOT(tp_any_ctor,suffix->cs_constructor.ctp_any_ctor);
 WRITE_SLOT_ID(DEE_CLASS_SLOTID_SUPERARGS_ANY_CTOR,suffix->cs_constructor.ctp_any_ctor_superargs);
 WRITE_SLOT(tp_dtor,suffix->cs_destructor.ctp_dtor);
 WRITE_SLOT(tp_copy_assign,suffix->cs_assign.ctp_copy_assign);
 WRITE_SLOT(tp_move_assign,suffix->cs_assign.ctp_move_assign);
 WRITE_SLOT(tp_any_assign,suffix->cs_assign.ctp_any_assign);
 WRITE_SLOT(tp_str,suffix->cs_cast.ctp_str);
 WRITE_SLOT(tp_repr,suffix->cs_cast.ctp_repr);
 WRITE_SLOT(tp_int32,suffix->cs_cast.ctp_int32);
 WRITE_SLOT(tp_int64,suffix->cs_cast.ctp_int64);
 WRITE_SLOT(tp_double,suffix->cs_cast.ctp_double);
 WRITE_SLOT(tp_call,suffix->cs_object.ctp_call);
 WRITE_SLOT(tp_not,suffix->cs_math.ctp_not);
 WRITE_SLOT(tp_bool,suffix->cs_math.ctp_bool);
 WRITE_SLOT(tp_inv,suffix->cs_math.ctp_inv);
 WRITE_SLOT(tp_pos,suffix->cs_math.ctp_pos);
 WRITE_SLOT(tp_neg,suffix->cs_math.ctp_neg);
 WRITE_SLOT(tp_inc,suffix->cs_math.ctp_inc);
 WRITE_SLOT(tp_incpost,suffix->cs_math.ctp_incpost);
 WRITE_SLOT(tp_dec,suffix->cs_math.ctp_dec);
 WRITE_SLOT(tp_decpost,suffix->cs_math.ctp_decpost);
 WRITE_SLOT(tp_add,suffix->cs_math.ctp_add);
 WRITE_SLOT(tp_iadd,suffix->cs_math.ctp_iadd);
 WRITE_SLOT(tp_sub,suffix->cs_math.ctp_sub);
 WRITE_SLOT(tp_isub,suffix->cs_math.ctp_isub);
 WRITE_SLOT(tp_mul,suffix->cs_math.ctp_mul);
 WRITE_SLOT(tp_imul,suffix->cs_math.ctp_imul);
 WRITE_SLOT(tp_div,suffix->cs_math.ctp_div);
 WRITE_SLOT(tp_idiv,suffix->cs_math.ctp_idiv);
 WRITE_SLOT(tp_mod,suffix->cs_math.ctp_mod);
 WRITE_SLOT(tp_imod,suffix->cs_math.ctp_imod);
 WRITE_SLOT(tp_shl,suffix->cs_math.ctp_shl);
 WRITE_SLOT(tp_ishl,suffix->cs_math.ctp_ishl);
 WRITE_SLOT(tp_shr,suffix->cs_math.ctp_shr);
 WRITE_SLOT(tp_ishr,suffix->cs_math.ctp_ishr);
 WRITE_SLOT(tp_and,suffix->cs_math.ctp_and);
 WRITE_SLOT(tp_iand,suffix->cs_math.ctp_iand);
 WRITE_SLOT(tp_or,suffix->cs_math.ctp_or);
 WRITE_SLOT(tp_ior,suffix->cs_math.ctp_ior);
 WRITE_SLOT(tp_xor,suffix->cs_math.ctp_xor);
 WRITE_SLOT(tp_ixor,suffix->cs_math.ctp_ixor);
 WRITE_SLOT(tp_pow,suffix->cs_math.ctp_pow);
 WRITE_SLOT(tp_ipow,suffix->cs_math.ctp_ipow);
 WRITE_SLOT(tp_hash,suffix->cs_math.ctp_hash);
 WRITE_SLOT(tp_cmp_lo,suffix->cs_compare.ctp_cmp_lo);
 WRITE_SLOT(tp_cmp_le,suffix->cs_compare.ctp_cmp_le);
 WRITE_SLOT(tp_cmp_eq,suffix->cs_compare.ctp_cmp_eq);
 WRITE_SLOT(tp_cmp_ne,suffix->cs_compare.ctp_cmp_ne);
 WRITE_SLOT(tp_cmp_gr,suffix->cs_compare.ctp_cmp_gr);
 WRITE_SLOT(tp_cmp_ge,suffix->cs_compare.ctp_cmp_ge);
 WRITE_SLOT(tp_seq_get,suffix->cs_seq.ctp_seq_get);
 WRITE_SLOT(tp_seq_del,suffix->cs_seq.ctp_seq_del);
 WRITE_SLOT(tp_seq_set,suffix->cs_seq.ctp_seq_set);
 WRITE_SLOT(tp_seq_size,suffix->cs_seq.ctp_seq_size);
 WRITE_SLOT(tp_seq_contains,suffix->cs_seq.ctp_seq_contains);
 WRITE_SLOT(tp_seq_range_get,suffix->cs_seq.ctp_seq_range_get);
 WRITE_SLOT(tp_seq_range_del,suffix->cs_seq.ctp_seq_range_del);
 WRITE_SLOT(tp_seq_range_set,suffix->cs_seq.ctp_seq_range_set);
 WRITE_SLOT(tp_seq_iter_self,suffix->cs_seq.ctp_seq_iter_self);
 WRITE_SLOT(tp_seq_iter_next,suffix->cs_seq.ctp_seq_iter_next);
 WRITE_SLOT(tp_attr_get,suffix->cs_attr.ctp_attr_get);
 WRITE_SLOT(tp_attr_del,suffix->cs_attr.ctp_attr_del);
 WRITE_SLOT(tp_attr_set,suffix->cs_attr.ctp_attr_set);
#undef WRITE_SLOT
#undef WRITE_SLOT_ID
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 {
  struct _DeeClassDynamicVTableEntry *iter,*end;
  end = (iter = suffix->cs_vtable.dvt_tablev)+suffix->cs_vtable.dvt_tablec;
  while (iter != end) {
   Dee_uint16_t written_slot;
   DEE_ASSERT(DeeObject_Check(iter->dvte_callback));
   written_slot = (Dee_uint16_t)iter->dvte_slot;
   if DEE_UNLIKELY(DeeFile_PutLe(outfile,written_slot) != 0) return -1;
   if DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,iter->dvte_callback) != 0) return -1;
   ++iter;
  }
 }
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
 return DeeFile_WriteAll(outfile,&end_marker,sizeof(end_marker));
}

static struct DeeTypeMarshal _deeclass_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALID_CLASS),
 member(&_deeclass_tp_marshal_ctor),member(&_deeclass_tp_marshal_put));


//////////////////////////////////////////////////////////////////////////
// ClassType VTable
static DeeObject *DEE_CALL _deeclass___get_slot(
 DeeClassObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int slot; DeeObject *result;
 if (DeeTuple_Unpack(args,"d:__get_slot",&slot) != 0) return NULL;
 if ((result = DeeClass_GetSlot((DeeTypeObject *)self,slot)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_ValueError,"Invalid/Unsupported slot id: %d",slot);
  return NULL;
 }
 DeeReturn_NEWREF(result);
}
static DeeObject *DEE_CALL _deeclass___get_method(
 DeeClassObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *name,*result;
 if (DeeTuple_Unpack(args,"o:__get_method",&name) != 0) return NULL;
 if (DeeError_TypeError_CheckTypeExact(name,&DeeString_Type) != 0) return NULL;
 if ((result = DeeClass_GetMethod((DeeTypeObject *)self,name)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_ValueError,"Class %k doesn't have a method %k",self,name);
  return NULL;
 }
 DeeReturn_NEWREF(result);
}
static DeeObject *DEE_CALL _deeclass___get_class_method(
 DeeClassObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *name,*result;
 if (DeeTuple_Unpack(args,"o:__get_class_method",&name) != 0) return NULL;
 if (DeeError_TypeError_CheckTypeExact(name,&DeeString_Type) != 0) return NULL;
 if ((result = DeeClass_GetClassMethod((DeeTypeObject *)self,name)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_ValueError,"Class %k doesn't have a class method %k",self,name);
  return NULL;
 }
 DeeReturn_NEWREF(result);
}
static DeeObject *DEE_CALL _deeclass___get_property(
 DeeClassObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *name,*getter_,*delete_,*setter_;
 if (DeeTuple_Unpack(args,"o:__get_property",&name) != 0) return NULL;
 if (DeeError_TypeError_CheckTypeExact(name,&DeeString_Type) != 0) return NULL;
 if (!DeeClass_GetProperty((DeeTypeObject *)self,name,&getter_,&delete_,&setter_)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,"Class %k doesn't have a property %k",self,name);
  return NULL;
 }
 return DeeTuple_Pack(3,
  getter_ ? getter_ : Dee_None,
  delete_ ? delete_ : Dee_None,
  setter_ ? setter_ : Dee_None);
}
static DeeObject *DEE_CALL _deeclass___get_class_property(
 DeeClassObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *name,*getter_,*delete_,*setter_;
 if (DeeTuple_Unpack(args,"o:__get_class_property",&name) != 0) return NULL;
 if (DeeError_TypeError_CheckTypeExact(name,&DeeString_Type) != 0) return NULL;
 if (!DeeClass_GetClassProperty((DeeTypeObject *)self,name,&getter_,&delete_,&setter_)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,"Class %k doesn't have a class property %k",self,name);
  return NULL;
 }
 return DeeTuple_Pack(3,
  getter_ ? getter_ : Dee_None,
  delete_ ? delete_ : Dee_None,
  setter_ ? setter_ : Dee_None);
}
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
static DeeObject *DEE_CALL _deeclass___del_slot(
 DeeClassObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int slot;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"d:__set_slot",&slot) != 0) return NULL;
 if DEE_UNLIKELY(DeeClass_DelSlot((DeeTypeObject *)self,slot) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deeclass___set_slot(
 DeeClassObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int slot; DeeObject *v;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"do:__set_slot",&slot,&v) != 0) return NULL;
 if DEE_UNLIKELY(DeeClass_SetSlot((DeeTypeObject *)self,slot,v) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deeclass___add_method(
 DeeClassObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *name,*callback = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:__add_method",&name,&callback) != 0) return NULL;
 if (!callback) {
  callback = name;
  if DEE_LIKELY(DeeFunction_Check(callback)) name = DeeFunction_NAME(callback);
  else {missing_name: DeeError_TypeError_ArgCountExpected(2,1); return NULL; }
  if DEE_UNLIKELY(!name) goto missing_name;
 }
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(name,&DeeString_Type) != 0) return NULL;
 if DEE_UNLIKELY(DeeClass_HasMethod((DeeTypeObject *)self,name)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,"Class %k already has a method %k",self,name);
  return NULL;
 }
 if DEE_UNLIKELY(DeeClass_AddMethod((DeeTypeObject *)self,name,callback) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deeclass___add_class_method(
 DeeClassObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *name,*callback = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:__add_class_method",&name,&callback) != 0) return NULL;
 if (!callback) {
  callback = name;
  if DEE_LIKELY(DeeFunction_Check(callback)) name = DeeFunction_NAME(callback);
  else {missing_name: DeeError_TypeError_ArgCountExpected(2,1); return NULL; }
  if DEE_UNLIKELY(!name) goto missing_name;
 }
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(name,&DeeString_Type) != 0) return NULL;
 if DEE_UNLIKELY(DeeClass_HasClassMethod((DeeTypeObject *)self,name)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,"Class %k already has a class method %k",self,name);
  return NULL;
 }
 if DEE_UNLIKELY(DeeClass_AddClassMethod((DeeTypeObject *)self,name,callback) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deeclass___add_property(
 DeeClassObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *name,*getter_ = Dee_None,*delete_ = Dee_None,*setter_ = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|ooo:__add_property",&name,&getter_,&delete_,&setter_) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(name,&DeeString_Type) != 0) return NULL;
 if DEE_UNLIKELY(DeeClass_HasProperty((DeeTypeObject *)self,name)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,"Class %k already has a property %k",self,name);
  return NULL;
 }
 if DEE_UNLIKELY(DeeClass_AddProperty((DeeTypeObject *)self,name,
  DeeNone_Check(getter_) ? NULL : getter_,
  DeeNone_Check(delete_) ? NULL : delete_,
  DeeNone_Check(setter_) ? NULL : setter_) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deeclass___add_class_property(
 DeeClassObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *name,*getter_ = Dee_None,*delete_ = Dee_None,*setter_ = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|ooo:__add_class_property",&name,&getter_,&delete_,&setter_) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(name,&DeeString_Type) != 0) return NULL;
 if DEE_UNLIKELY(DeeClass_HasClassProperty((DeeTypeObject *)self,name)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,"Class %k already has a class property %k",self,name);
  return NULL;
 }
 if DEE_UNLIKELY(DeeClass_AddClassProperty((DeeTypeObject *)self,name,
  DeeNone_Check(getter_) ? NULL : getter_,
  DeeNone_Check(delete_) ? NULL : delete_,
  DeeNone_Check(setter_) ? NULL : setter_) != 0) return NULL;
 DeeReturn_None;
}
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMethodDef const _deeclass_tp_methods[] = {
 DEE_METHODDEF_v100("__get_slot",member(&_deeclass___get_slot),"__get_slot(int slot) -> object"),
 DEE_METHODDEF_v100("__get_method",member(&_deeclass___get_method),"__get_method(string name) -> object"),
 DEE_METHODDEF_v100("__get_property",member(&_deeclass___get_property),"__get_property(string name) -> (object,object,object)"),
 DEE_METHODDEF_v100("__get_class_method",member(&_deeclass___get_class_method),"__get_class_method(string name) -> object"),
 DEE_METHODDEF_v100("__get_class_property",member(&_deeclass___get_class_property),"__get_class_property(string name) -> (object,object,object)"),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 // Pshhht... Classes are supposed to be immutable...
 // >> These are only available in development builds
 //    and are merely meant to be aid in debugging.
 DEE_METHODDEF_v100("__del_slot",member(&_deeclass___del_slot),"__del_slot(int slot) -> none"),
 DEE_METHODDEF_v100("__set_slot",member(&_deeclass___set_slot),"__set_slot(int slot, object v) -> none"),
 DEE_METHODDEF_v100("__add_method",member(&_deeclass___add_method),"__add_method(string name, object callback) -> none"),
 DEE_METHODDEF_v100("__add_property",member(&_deeclass___add_property),"__add_property(string name, object get = none, object del = none, object set = none) -> none"),
 DEE_METHODDEF_v100("__add_class_method",member(&_deeclass___add_class_method),"__add_class_method(string name, object callback) -> none"),
 DEE_METHODDEF_v100("__add_class_property",member(&_deeclass___add_class_property),"__add_class_property(string name, object get = none, object del = none, object set = none) -> none"),
#endif
 DEE_METHODDEF_END_v100
};

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeClassTypeObject *)
_DeeClassType_New(DEE_A_IN DeeTypeObject const *base_type) {
 DeeClassTypeObject *result;
 DEE_ASSERT(DeeObject_Check(base_type) && DeeType_Check(base_type));
 DEE_ASSERT(DeeType_GET_SLOT(base_type,tp_instance_size) >= sizeof(DeeTypeObject) &&
            "'base_type' can't possibly be a type object");
 if DEE_LIKELY((result = (DeeClassTypeObject *)DeeType_NewInstanceDefault(&DeeType_Type)) != NULL) {
  DeeType_GET_SLOT(result,tp_flags) |=
   DEE_TYPE_FLAG_STATIC_NAME|DEE_TYPE_FLAG_CLASS_TYPE|DEE_TYPE_FLAG_HAS_GC;
  if DEE_LIKELY(DeeType_HAS_SLOT(base_type,tp_name)) {
   char *class_name; char const *base_name; Dee_size_t base_name_len;
   base_name = DeeType_GET_SLOT(base_type,tp_name);
   DEE_ASSERT(base_name);
   base_name_len = strlen(base_name);
   // 'class_type' + '<' + base_name + '>' + '\0'
   while DEE_UNLIKELY((class_name = (char *)malloc_nz((
    10+1+base_name_len+1+1)*sizeof(char))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
    Dee_DECREF(result);
    return NULL;
   }
   strcpy(class_name,"class_type<");
   memcpy(class_name+10+1,base_name,base_name_len*sizeof(char));
   class_name[10+1+base_name_len] = '>';
   class_name[10+1+base_name_len+1] = '\0';
   DeeType_GET_SLOT(result,tp_flags) &= ~(DEE_TYPE_FLAG_STATIC_NAME);
   DeeType_GET_SLOT(result,tp_name) = class_name;
  } else {
   DeeType_GET_SLOT(result,tp_name) = "class_type";
  }
  DeeType_GET_SLOT(result,tp_ctor) = (int(*)(DeeTypeObject*,DeeObject*))&_deeclass_tp_ctor;
  DeeType_GET_SLOT(result,tp_any_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deeclass_tp_any_ctor;
  DeeType_GET_SLOT(result,tp_alloc) = &_DeeGC_TpAlloc;
  DeeType_GET_SLOT(result,tp_free) = &_DeeGC_TpFree;
  DeeType_GET_SLOT(result,tp_dtor) = (void(*)(DeeObject*))&_deeclass_tp_dtor;
  DeeType_GET_SLOT(result,tp_clear) = (void(*)(DeeObject*))&_deeclass_tp_clear;
  Dee_XDECREF(DeeType_GET_SLOT(result,tp_base));
  Dee_INCREF(DeeType_GET_SLOT(result,tp_base) = (DeeTypeObject *)base_type);
  DeeType_GET_SLOT(result,tp_visit) = (DeeType_SLOT_TYPE(tp_visit))&_deeclass_tp_visit;
  // This space is required for the class suffix
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
  if DEE_UNLIKELY(DeeType_IsSameOrDerived(base_type,&DeeFileType_Type)) {
   DeeType_GET_SLOT(result,tp_instance_size) = DeeType_GET_SLOT(
    base_type,tp_instance_size)+sizeof(struct DeeFileClassSuffix);
  } else
#endif /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */
  {
   DeeType_GET_SLOT(result,tp_instance_size) = DeeType_GET_SLOT(
    base_type,tp_instance_size)+sizeof(struct DeeClassSuffix);
  }
  DeeType_GET_SLOT(result,tp_attr_get) = (DeeObject*(*)(DeeObject*,DeeObject*))&DeeClass_GetAttr;
  DeeType_GET_SLOT(result,tp_attr_del) = (int(*)(DeeObject*,DeeObject*))&DeeClass_DelAttr;
  DeeType_GET_SLOT(result,tp_attr_set) = (int(*)(DeeObject*,DeeObject*,DeeObject*))&DeeClass_SetAttr;
  DeeType_GET_SLOT(result,tp_methods) = _deeclass_tp_methods;
  DeeType_GET_SLOT(result,tp_marshal) = &_deeclass_tp_marshal;
 }
 // Just make sure we did everything correct
 DEE_ASSERT(DeeObject_Check(result) && DeeClassType_Check(result));
 return result;
}






//////////////////////////////////////////////////////////////////////////
// InstanceMethod VTable
static DeeInstanceMethodObject *DEE_CALL _deeinstancemethod_tp_alloc(DeeTypeObject *tp) {
 DeeInstanceMethodObject *result = DEE_OBJECTPOOL_ALLOC(instance_method);
 if (result) DeeObject_INIT(result,tp);
 return result;
}
static void DEE_CALL _deeinstancemethod_tp_free(
 DeeTypeObject *DEE_UNUSED(tp), DeeInstanceMethodObject *ob) {
 DEE_OBJECTPOOL_FREE(instance_method,ob);
}
static void DEE_CALL _deeinstancemethod_tp_dtor(DeeInstanceMethodObject *self) {
 Dee_DECREF(self->im_this);
 Dee_DECREF(self->im_func);
}
DEE_VISIT_PROC(_deeinstancemethod_tp_visit,DeeInstanceMethodObject *self) {
 Dee_VISIT(self->im_this);
 Dee_VISIT(self->im_func);
}
static int DEE_CALL _deeinstancemethod_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp), DeeInstanceMethodObject *self, DeeObject *args) {
 DeeObject *this_arg,*func;
 if (DeeTuple_Unpack(args,"oo:instance_method",&this_arg,&func) != 0) return -1;
 Dee_INCREF(self->im_this = this_arg);
 Dee_INCREF(self->im_func = func);
 return 0;
}
static int DEE_CALL _deeinstancemethod_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp), DeeInstanceMethodObject *self, DeeInstanceMethodObject *right) {
 Dee_INCREF(self->im_this = right->im_this);
 Dee_INCREF(self->im_func = right->im_func);
 return 0;
}
static DeeObject *DEE_CALL _deeinstancemethod_tp_str(DeeInstanceMethodObject *self) {
 return DeeString_Newf("%k.%k",Dee_TYPE(self->im_this),self->im_func);
}
static DeeObject *DEE_CALL _deeinstancemethod_tp_repr(DeeInstanceMethodObject *self) {
 return DeeString_Newf("<instance_method: %s(%r) -> %k>",
                       DeeType_NAME(Dee_TYPE(self->im_this)),
                       self->im_this,self->im_func);
}

static struct DeeMemberDef const _deeinstancemethod_tp_members[] = {
 DEE_MEMBERDEF_RO_v100(DeeInstanceMethodObject,im_this,object),
 DEE_MEMBERDEF_RO_v100(DeeInstanceMethodObject,im_func,object),
 DEE_MEMBERDEF_END_v100
};


static int DEE_CALL _instancemethod_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeInstanceMethodObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 if DEE_UNLIKELY((self->im_this = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if DEE_UNLIKELY((self->im_func = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) { Dee_DECREF(self->im_this); return -1; }
 return 0;
}

static int DEE_CALL _instancemethod_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeInstanceMethodObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 if DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,self->im_this) != 0) return -1;
 return DeeMarshal_WriteObjectWithMap(outfile,map,self->im_func);
}

static struct DeeTypeMarshal _instancemethod_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(instance_method)),
 member(&_instancemethod_tp_marshal_ctor),
 member(&_instancemethod_tp_marshal_put));

DeeTypeObject DeeInstanceMethod_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("instance_method"),null,null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeInstanceMethodObject),
  member(&_deeinstancemethod_tp_alloc),null,
  member(&_deeinstancemethod_tp_copy_ctor),null,
  member(&_deeinstancemethod_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(
  member(&_deeinstancemethod_tp_free),
  member(&_deeinstancemethod_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deeinstancemethod_tp_str),
  member(&_deeinstancemethod_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(
  member(&DeeInstanceMethod_Call),
  member(&_deeinstancemethod_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deeinstancemethod_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_instancemethod_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};


static int DEE_CALL _deeproperty_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeePropertyObject *self) {
 self->pr_get = NULL;
 self->pr_del = NULL;
 self->pr_set = NULL;
 return 0;
}
static int DEE_CALL _deeproperty_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeePropertyObject *self, DeePropertyObject *right) {
 Dee_XINCREF(self->pr_get = right->pr_get);
 Dee_XINCREF(self->pr_del = right->pr_del);
 Dee_XINCREF(self->pr_set = right->pr_set);
 return 0;
}
static int DEE_CALL _deeproperty_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeePropertyObject *self, DeePropertyObject *right) {
 self->pr_get = right->pr_get;
 self->pr_del = right->pr_del;
 self->pr_set = right->pr_set;
 right->pr_get = NULL;
 right->pr_del = NULL;
 right->pr_set = NULL;
 return 0;
}
static int DEE_CALL _deeproperty_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeePropertyObject *self, DeeObject *args) {
 self->pr_get = NULL;
 self->pr_del = NULL;
 self->pr_set = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|ooo:property",
  &self->pr_get,&self->pr_del,&self->pr_set) != 0) return -1;
 Dee_XINCREF(self->pr_get);
 Dee_XINCREF(self->pr_del);
 Dee_XINCREF(self->pr_set);
 return 0;
}
static DeeObject *DEE_CALL _deeproperty_tp_str(DeePropertyObject *self) {
 return DeeString_Newf("property(%k,%k,%k)",
                       self->pr_get ? self->pr_get : Dee_None,
                       self->pr_del ? self->pr_del : Dee_None,
                       self->pr_set ? self->pr_set : Dee_None);
}
static DeeObject *DEE_CALL _deeproperty_tp_repr(DeePropertyObject *self) {
 return DeeString_Newf("property(%r,%r,%r)",
                       self->pr_get ? self->pr_get : Dee_None,
                       self->pr_del ? self->pr_del : Dee_None,
                       self->pr_set ? self->pr_set : Dee_None);
}
static void DEE_CALL _deeproperty_tp_dtor(DeePropertyObject *self) {
 Dee_XDECREF(self->pr_get);
 Dee_XDECREF(self->pr_del);
 Dee_XDECREF(self->pr_set);
}
static void DEE_CALL _deeproperty_tp_clear(DeePropertyObject *self) {
 Dee_XCLEAR(self->pr_get);
 Dee_XCLEAR(self->pr_del);
 Dee_XCLEAR(self->pr_set);
}
DEE_VISIT_PROC(_deeproperty_tp_visit,DeePropertyObject *self) {
 Dee_XVISIT(self->pr_get);
 Dee_XVISIT(self->pr_del);
 Dee_XVISIT(self->pr_set);
}
static struct DeeMemberDef const _deeproperty_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("get",DeePropertyObject,pr_get,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("del",DeePropertyObject,pr_del,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("set",DeePropertyObject,pr_set,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__get__",DeePropertyObject,pr_get,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__del__",DeePropertyObject,pr_del,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__set__",DeePropertyObject,pr_set,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("read",DeePropertyObject,pr_get,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("write",DeePropertyObject,pr_set,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("delete",DeePropertyObject,pr_del,object_null),
 DEE_MEMBERDEF_END_v100
};


#define DEE_PROPERTY_FILE_FLAG_HAVE_GET 0x01
#define DEE_PROPERTY_FILE_FLAG_HAVE_DEL 0x02
#define DEE_PROPERTY_FILE_FLAG_HAVE_SET 0x04
static int DEE_CALL _deeproperty_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeePropertyObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_uint8_t flags;
 if DEE_UNLIKELY(DeeFile_Getc(infile,&flags) != 0) return -1;
 if ((flags&DEE_PROPERTY_FILE_FLAG_HAVE_GET)!=0) {
  if DEE_UNLIKELY((self->pr_get = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 } else self->pr_get = NULL;
 if ((flags&DEE_PROPERTY_FILE_FLAG_HAVE_DEL)!=0) {
  if DEE_UNLIKELY((self->pr_del = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {
err_get: Dee_XDECREF(self->pr_get); return -1;
  }
 } else self->pr_del = NULL;
 if ((flags&DEE_PROPERTY_FILE_FLAG_HAVE_SET)!=0) {
  if DEE_UNLIKELY((self->pr_set = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {
/*err_del:*/ Dee_XDECREF(self->pr_del); goto err_get;
  }
 } else self->pr_set = NULL;
 return 0;
}

static int DEE_CALL _deeproperty_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeePropertyObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 Dee_uint8_t flags = 0;
 if (self->pr_get) flags |= DEE_PROPERTY_FILE_FLAG_HAVE_GET;
 if (self->pr_del) flags |= DEE_PROPERTY_FILE_FLAG_HAVE_DEL;
 if (self->pr_set) flags |= DEE_PROPERTY_FILE_FLAG_HAVE_SET;
 if (DeeFile_Putc(outfile,flags) != 0) return -1;
 if (self->pr_get && DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,self->pr_get) != 0)) return -1;
 if (self->pr_del && DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,self->pr_del) != 0)) return -1;
 if (self->pr_set && DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,self->pr_set) != 0)) return -1;
 return 0;
}

static struct DeeTypeMarshal _deeproperty_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(property)),
 member(&_deeproperty_tp_marshal_ctor),
 member(&_deeproperty_tp_marshal_put));

DeeTypeObject DeeProperty_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("property"),null,
  member(DEE_TYPE_FLAG_HAS_GC|DEE_TYPE_FLAG_MUST_COPY|
         DEE_TYPE_FLAG_CONST_CTOR|DEE_TYPE_FLAG_FUNDAMENTAL),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeePropertyObject),member(&_DeeGC_TpAlloc),
  member(&_deeproperty_tp_ctor),
  member(&_deeproperty_tp_copy_ctor),
  member(&_deeproperty_tp_move_ctor),
  member(&_deeproperty_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(member(&_DeeGC_TpFree),
  member(&_deeproperty_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deeproperty_tp_str),
  member(&_deeproperty_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deeproperty_tp_visit),
  member(&_deeproperty_tp_clear)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deeproperty_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deeproperty_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES */

#endif /* !GUARD_DEEMON_CLASS_H */
