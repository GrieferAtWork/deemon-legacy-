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
#ifndef GUARD_DEEMON_THREAD_TLS_C
#define GUARD_DEEMON_THREAD_TLS_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include <deemon/__atomic_intrin.inl>
#include <deemon/bool.h>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/integer.h>
#include <deemon/memberdef.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_visit.h>
#include <deemon/mp/thread.h>
#include <deemon/mp/tls.h>
#include <deemon/tuple.h>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

#ifndef DEE_WITHOUT_THREADS
static struct DeeAtomicMutex _dee_tls_id_lock = DeeAtomicMutex_INIT();
#define DEE_TLS_ID_LOCK_ACQUIRE() DeeAtomicMutex_Acquire(&_dee_tls_id_lock)
#define DEE_TLS_ID_LOCK_RELEASE() DeeAtomicMutex_Release(&_dee_tls_id_lock)

static Dee_size_t  _dee_tls_id_next_new = 0;

// Static buffer for free TLS ids
enum{_dee_tls_id_free_static=256};
static Dee_size_t _dee_tls_id_free[_dee_tls_id_free_static];
static Dee_size_t _dee_tls_id_free_size = 0;
// Dynamic buffer for free TLS ids
// >> Used if _dee_tls_id_free_size > _dee_tls_id_free_static
static Dee_size_t *_dee_tls_id_free_more = NULL;

DEE_A_RET_WUNUSED Dee_size_t DeeTls_InstanceCount(void) {
 Dee_size_t result;
 DEE_TLS_ID_LOCK_ACQUIRE();
 result = _dee_tls_id_free_size;
 DEE_TLS_ID_LOCK_RELEASE();
 return result;
}

//////////////////////////////////////////////////////////////////////////
// Searches for "id" and removes it from the free list
// >> Returns -1: on error (bad reallocation)
// >> Returns  0: if found and removed
// >> Returns  1: if not found
DEE_COMPILER_MSVC_WARNING_PUSH(6385)
DEE_STATIC_INLINE(int) _DeeRemoveFreeTlsID_AlreadyLocked(Dee_size_t id) {
 Dee_size_t *new_list; Dee_size_t i,search_size = _dee_tls_id_free_size < _dee_tls_id_free_static
  ? _dee_tls_id_free_size : _dee_tls_id_free_static;
 for (i = 0; i < search_size; ++i) {
  if (_dee_tls_id_free[i] == id) {
   Dee_size_t transfer_id;
   memmove(_dee_tls_id_free+i,_dee_tls_id_free+(i+1),
           (search_size-i)*sizeof(Dee_size_t));
   if (--_dee_tls_id_free_size >= _dee_tls_id_free_static) {
    transfer_id = *_dee_tls_id_free_more;
    memmove(_dee_tls_id_free_more,_dee_tls_id_free_more+1,_dee_tls_id_free_size-_dee_tls_id_free_static);
    if (_dee_tls_id_free_size == _dee_tls_id_free_static) {
     free_nn(_dee_tls_id_free_more);
#ifdef DEE_DEBUG
     _dee_tls_id_free_more = NULL;
#endif
    } else {
     new_list = (Dee_size_t *)realloc_nnz(
      _dee_tls_id_free_more,(_dee_tls_id_free_size-_dee_tls_id_free_static)*sizeof(Dee_size_t));
     if (!new_list) {
      memmove(_dee_tls_id_free_more+1,_dee_tls_id_free_more,_dee_tls_id_free_size-_dee_tls_id_free_static);
      *_dee_tls_id_free_more = transfer_id;
      return -1; // Reallocation error
     }
     _dee_tls_id_free_more = new_list;
    }
    _dee_tls_id_free[_dee_tls_id_free_static-1] = transfer_id;
   }
   return 0;
  }
 }
 if (_dee_tls_id_free_size > _dee_tls_id_free_static) {
  // Search the dynamic list
  search_size = _dee_tls_id_free_size-_dee_tls_id_free_static;
  for (i = 0; i < search_size; ++i) {
   if (_dee_tls_id_free_more[i] == id) {
    if (--_dee_tls_id_free_size == _dee_tls_id_free_static) {
     free_nn(_dee_tls_id_free_more);
#ifdef DEE_DEBUG
     _dee_tls_id_free_more = NULL;
#endif
    } else {
     memmove(_dee_tls_id_free_more+i,_dee_tls_id_free_more+i+1,search_size-i);
     new_list = (Dee_size_t *)realloc_nnz(
      _dee_tls_id_free_more,(_dee_tls_id_free_size-_dee_tls_id_free_static)*sizeof(Dee_size_t));
     if (!new_list) {
      memmove(_dee_tls_id_free_more+i+1,_dee_tls_id_free_more+i,search_size-i);
      _dee_tls_id_free_more[i] = id;
      return -1; // Reallocation error
     }
     _dee_tls_id_free_more = new_list;
    }
    return 0;
   }
  }
 }
 return 1;
}
DEE_COMPILER_MSVC_WARNING_POP

DEE_STATIC_INLINE(Dee_size_t) _DeeAllocTlsID(void);
DEE_STATIC_INLINE(Dee_size_t) _DeeAllocTlsID_internal(void) {
 Dee_size_t result;
 DEE_TLS_ID_LOCK_ACQUIRE();
 if (_dee_tls_id_free_size) {
  DEE_ASSERT(_dee_tls_id_free_more);
  if (_dee_tls_id_free_size >= _dee_tls_id_free_static) {
   result = _dee_tls_id_free_more[(--_dee_tls_id_free_size)-_dee_tls_id_free_static];
   if (_dee_tls_id_free_size != _dee_tls_id_free_static) {
    Dee_size_t *new_list;
    DEE_COMPILER_ASSUME(_dee_tls_id_free_size > _dee_tls_id_free_static);
    if ((new_list = (Dee_size_t *)realloc_nnz(_dee_tls_id_free_more,(
     _dee_tls_id_free_size-_dee_tls_id_free_static)*sizeof(Dee_size_t))) == NULL) {
     result = (Dee_size_t)-1; // bad reallocation
     ++_dee_tls_id_free_size;
    } else _dee_tls_id_free_more = new_list;
   } else {
    free_nn(_dee_tls_id_free_more);
#ifdef DEE_DEBUG
    _dee_tls_id_free_more = NULL;
#endif
   }
  } else result = _dee_tls_id_free[--_dee_tls_id_free_size];
 } else result = _dee_tls_id_next_new++;
 DEE_TLS_ID_LOCK_RELEASE();
 DEE_LVERBOSE2("Allocated Tls id: %Iu\n",result);
 return result;
}
DEE_STATIC_INLINE(Dee_size_t) _DeeAllocTlsID(void) {
 Dee_size_t result;
 while ((result = _DeeAllocTlsID_internal()) == (Dee_size_t)-1) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return (Dee_size_t)-1;
 }
 return result;
}

DEE_STATIC_INLINE(void) _DeeFreeTlsID(Dee_size_t id) {
 DEE_LVERBOSE2("Releasing Tls id: %Iu\n",id);
 DEE_TLS_ID_LOCK_ACQUIRE();
 if (id == (_dee_tls_id_next_new-1)) {
  // Last id removed
  //   >> decrease the the next id value
  --_dee_tls_id_next_new;
  // Release addition free ids
  while (_DeeRemoveFreeTlsID_AlreadyLocked(_dee_tls_id_next_new-1) == 0)
   --_dee_tls_id_next_new;
 } else {
  if (_dee_tls_id_free_size < _dee_tls_id_free_static) {
   // Use the static buffer
   _dee_tls_id_free[_dee_tls_id_free_size++] = id;
  } else {
   // Need to use dynamic list
   // NOTE: realloc_nz, because the list may be NULL if this is the first entry
   Dee_size_t *new_list = (Dee_size_t *)realloc_nz(
    _dee_tls_id_free_more,(++_dee_tls_id_free_size-_dee_tls_id_free_static)*sizeof(Dee_size_t));
   if (new_list) {
    new_list[(_dee_tls_id_free_size-_dee_tls_id_free_static)-1] = id;
    _dee_tls_id_free_more = new_list;
   } else {
    // We can't recover at this point; the given id is lost...
   }
  }
 }
 DEE_TLS_ID_LOCK_RELEASE();
}
#else /* !DEE_WITHOUT_THREADS */
DEE_A_RET_WUNUSED Dee_size_t DeeTls_InstanceCount(void) {
 return 0;
}
#endif /* DEE_WITHOUT_THREADS */




DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeTls_InitWithFactory(
 DEE_A_INOUT DeeTlsObject *self, DEE_A_INOUT DeeObject *factory) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTls_Check(self));
 DEE_ASSERT(DeeObject_Check(factory));
 Dee_INCREF(self->tls_factory = factory);
#ifndef DEE_WITHOUT_THREADS
 if ((self->tls_handle = _DeeAllocTlsID()) == (Dee_size_t)-1) {
  DeeError_NoMemory();
  return -1;
 }
#else /* !DEE_WITHOUT_THREADS */
 self->tls_value = NULL;
#endif /* DEE_WITHOUT_THREADS */
 return 0;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTlsObject) *
DeeTls_NewWithFactory(DEE_A_INOUT DeeObject *factory) {
 DeeTlsObject *result;
 DEE_ASSERT(DeeObject_Check(factory));
 if ((result = DeeObject_MALLOCF(DeeTlsObject,"tls (factory = %p)",factory)) != NULL) {
  DeeObject_INIT(result,&DeeTls_Type);
  if (_DeeTls_InitWithFactory(result,factory) == -1) {
   _DeeObject_DELETE(&DeeTls_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_WUNUSED DeeObject *DeeTls_Factory(DEE_A_IN DeeObject const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTls_Check(self));
 return DeeTls_FACTORY(self);
}


DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeTls_Delete(DEE_A_INOUT_OBJECT(DeeTlsObject) *self) {
#ifndef DEE_WITHOUT_THREADS
 Dee_size_t handle;
 DeeThreadObject *thread_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeTls_Check(self));
 thread_self = DeeThread_SELF();
 if (thread_self) {
  handle = DeeTls_HANDLE(self);
  DeeAtomicMutex_Acquire(&thread_self->t_rt_lock);
  //if (DeeThread_TERMINATED(thread_self)) { ... } // Allow because it's a delete
  if (handle >= thread_self->t_rt_tls_size ||
     !thread_self->t_rt_tls_list[handle]) {
   DeeAtomicMutex_Release(&thread_self->t_rt_lock);
   return 1; // Tls not allocated
  }
  Dee_DECREF(thread_self->t_rt_tls_list[handle]);
  thread_self->t_rt_tls_list[handle] = NULL;
  DeeAtomicMutex_Release(&thread_self->t_rt_lock);
  return 0;
 } else {
  return 1; // We can't set errors without DeeThread_Self()
 }
#else /* !DEE_WITHOUT_THREADS */
 DEE_ASSERT(DeeObject_Check(self) && DeeTls_Check(self));
 if (DeeTls_VALUE(self)) {
  Dee_DECREF(DeeTls_VALUE(self));
  ((DeeTlsObject *)self)->tls_value = NULL;
  return 0;
 }
 return 1;
#endif /* DEE_WITHOUT_THREADS */
}

DEE_A_RET_EXCEPT_REF DeeObject *
DeeTls_Get(DEE_A_INOUT_OBJECT(DeeTlsObject) *self) {
#ifndef DEE_WITHOUT_THREADS
 DeeObject *result,*factory_result,**new_list; Dee_size_t handle,new_size;
 DeeThreadObject *thread_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeTls_Check(self));
 if ((thread_self = DeeThread_SELF()) == NULL) return NULL;
 handle = DeeTls_HANDLE(self);
again:
 DeeAtomicMutex_Acquire(&thread_self->t_rt_lock);
 if (handle >= thread_self->t_rt_tls_size ||
    (result = thread_self->t_rt_tls_list[handle]) == NULL) {
  if (DeeThread_TERMINATED(thread_self)) {
   // Get is still allowed so long as the factory isn't invoked
   DeeAtomicMutex_Release(&thread_self->t_rt_lock);
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Can't initialize TLS of terminated thread %k",
                       thread_self);
   return NULL;
  }
  // Call the factory to create the default value
  DeeAtomicMutex_Release(&thread_self->t_rt_lock);
  if ((factory_result = DeeObject_Call(DeeTls_FACTORY(
   self),Dee_EmptyTuple)) == NULL) return NULL;
  DeeAtomicMutex_Acquire(&thread_self->t_rt_lock);
  if (handle >= thread_self->t_rt_tls_size) {
   new_size = handle+1; // Must reallocate TLS list
   if ((new_list = (DeeObject **)realloc_nz(
    thread_self->t_rt_tls_list,new_size*sizeof(DeeObject *))) == NULL) {
    DeeAtomicMutex_Release(&thread_self->t_rt_lock);
    if DEE_LIKELY(Dee_CollectMemory()) goto again; // start over
    DeeError_NoMemory();
    return NULL;
   }
   memset(new_list+thread_self->t_rt_tls_size,0,
         (new_size-thread_self->t_rt_tls_size)*sizeof(DeeObject *));
   thread_self->t_rt_tls_size = new_size;
   thread_self->t_rt_tls_list = new_list;
  } else if ((result = thread_self->t_rt_tls_list[handle]) != NULL) {
   // This shouldn't happen, but the contract doesn't prevent this...
   Dee_INCREF(result);
   DeeAtomicMutex_Release(&thread_self->t_rt_lock);
   Dee_DECREF(factory_result);
   return result;
  } 
  Dee_INCREF(thread_self->t_rt_tls_list[handle] = factory_result);
  DeeAtomicMutex_Release(&thread_self->t_rt_lock);
  return factory_result;
 } else {
  Dee_INCREF(result);
  DeeAtomicMutex_Release(&thread_self->t_rt_lock);
  return result;
 }
#else /* !DEE_WITHOUT_THREADS */
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeTls_Check(self));
 if ((result = DeeTls_VALUE(self)) == NULL) {
  result = DeeObject_Call(DeeTls_FACTORY(self),Dee_EmptyTuple);
  if (!result) return NULL;
  ((DeeTlsObject *)self)->tls_value = result; // Inherit reference
 }
 return result;
#endif /* DEE_WITHOUT_THREADS */
}
DEE_A_RET_NOEXCEPT_REF DeeObject *
_DeeTls_LazyGet(DEE_A_IN_OBJECT(DeeTlsObject) const *self) {
#ifndef DEE_WITHOUT_THREADS
 Dee_size_t handle; DeeThreadObject *thread_self; DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeTls_Check(self));
 if ((thread_self = DeeThread_SELF()) == NULL) return NULL;
 DeeAtomicMutex_Acquire(&thread_self->t_rt_lock);
 if ((handle = DeeTls_HANDLE(self)) >= thread_self->t_rt_tls_size) result = NULL;
 else Dee_XINCREF(result = thread_self->t_rt_tls_list[handle]);
 DeeAtomicMutex_Release(&thread_self->t_rt_lock);
 return result;
#else /* !DEE_WITHOUT_THREADS */
 DEE_ASSERT(DeeObject_Check(self) && DeeTls_Check(self));
 DeeReturn_NEWREF(((DeeTlsObject *)self)->tls_value);
#endif /* DEE_WITHOUT_THREADS */
}
DEE_A_RET_WUNUSED int DeeTls_Initialized(
 DEE_A_IN_OBJECT(DeeTlsObject) const *self) {
#ifndef DEE_WITHOUT_THREADS
 Dee_size_t handle; DeeThreadObject *thread_self; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeTls_Check(self));
 if ((thread_self = DeeThread_SELF()) == NULL) return 0;
 DeeAtomicMutex_Acquire(&thread_self->t_rt_lock);
 if ((handle = DeeTls_HANDLE(self)) >= thread_self->t_rt_tls_size) result = 0;
 else result = thread_self->t_rt_tls_list[handle] != NULL;
 DeeAtomicMutex_Release(&thread_self->t_rt_lock);
 return result;
#else /* !DEE_WITHOUT_THREADS */
 DEE_ASSERT(DeeObject_Check(self) && DeeTls_Check(self));
 return ((DeeTlsObject *)self)->tls_value != NULL;
#endif /* DEE_WITHOUT_THREADS */
}

DEE_A_RET_EXCEPT(-1) int DeeTls_Set(
 DEE_A_INOUT_OBJECT(DeeTlsObject) *self, DEE_A_INOUT DeeObject *v) {
#ifndef DEE_WITHOUT_THREADS
 DeeObject *old_value,**new_list; Dee_size_t handle,new_size;
 DeeThreadObject *thread_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeTls_Check(self));
 DEE_ASSERT(DeeObject_Check(v));
 if ((thread_self = DeeThread_SELF()) == NULL) return -1;
 handle = DeeTls_HANDLE(self);
again:
 DeeAtomicMutex_Acquire(&thread_self->t_rt_lock);
 if (DeeThread_TERMINATED(thread_self)) {
  // Setting a TLS of a terminated thread is illegal
  // >> By disallowing this, we can prevent TLS destructors
  //    from trying to revive a terminated thread
  //    (which could otherwise cause problems)
  DeeAtomicMutex_Release(&thread_self->t_rt_lock);
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Can't set TLS of terminated thread %k",
                      thread_self);
  return -1;
 }
 if (handle >= thread_self->t_rt_tls_size) {
  new_size = handle+1; // Must reallocate TLS list
  if ((new_list = (DeeObject **)realloc_nz(
   thread_self->t_rt_tls_list,new_size*sizeof(DeeObject *))) == NULL) {
   DeeAtomicMutex_Release(&thread_self->t_rt_lock);
   if DEE_LIKELY(Dee_CollectMemory()) goto again; // start over
   DeeError_NoMemory();
   return -1;
  }
  // No need to zero-initialize our own handle (we set that anyways)
  memset(new_list+thread_self->t_rt_tls_size,0,
        (handle-thread_self->t_rt_tls_size)*sizeof(DeeObject *));
  thread_self->t_rt_tls_size = new_size;
  thread_self->t_rt_tls_list = new_list;
  // No need to deallocate an old value (it didn't exit)
  Dee_INCREF(new_list[handle] = v);
  DeeAtomicMutex_Release(&thread_self->t_rt_lock);
  return 0;
 }
 // Get the old value (we need to decref it later)
 old_value = thread_self->t_rt_tls_list[handle];
 Dee_INCREF(thread_self->t_rt_tls_list[handle] = v); // Store the new value
 DeeAtomicMutex_Release(&thread_self->t_rt_lock);
 Dee_XDECREF(old_value);
 return 0;
#else /* !DEE_WITHOUT_THREADS */
 DeeObject *old_value;
 DEE_ASSERT(DeeObject_Check(self) && DeeTls_Check(self));
 old_value = DeeTls_VALUE(self);
 Dee_INCREF(((DeeTlsObject *)self)->tls_value = v);
 Dee_XDECREF(old_value);
 return 0;
#endif /* DEE_WITHOUT_THREADS */
}
// Exchange this thread's value for 'self' with 'v'
DEE_A_RET_EXCEPT_REF DeeObject *DeeTls_Exchange(
 DEE_A_INOUT_OBJECT(DeeTlsObject) *self, DEE_A_INOUT DeeObject *v) {
#ifndef DEE_WITHOUT_THREADS
 DeeObject *old_value,**new_list; Dee_size_t handle,new_size;
 DeeThreadObject *thread_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeTls_Check(self));
 DEE_ASSERT(DeeObject_Check(v));
 if ((thread_self = DeeThread_SELF()) == NULL) return NULL;
 handle = DeeTls_HANDLE(self);
again:
 DeeAtomicMutex_Acquire(&thread_self->t_rt_lock);
 if (DeeThread_TERMINATED(thread_self)) {
  // Setting a TLS of a terminated thread is illegal
  // >> By disallowing this, we can prevent TLS destructors
  //    from trying to revive a terminated thread
  //    (which could otherwise cause problems)
  DeeAtomicMutex_Release(&thread_self->t_rt_lock);
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Can't set TLS of terminated thread %k",
                      thread_self);
  return NULL;
 }
 if (handle >= thread_self->t_rt_tls_size) {
  new_size = handle+1; // Must reallocate TLS list
  if ((new_list = (DeeObject **)realloc_nz(
   thread_self->t_rt_tls_list,new_size*sizeof(DeeObject *))) == NULL) {
   DeeAtomicMutex_Release(&thread_self->t_rt_lock);
   if DEE_LIKELY(Dee_CollectMemory()) goto again; // start over
   DeeError_NoMemory();
   return NULL;
  }
  // No need to zero-initialize our own handle (we set that anyways)
  memset(new_list+thread_self->t_rt_tls_size,0,
        (handle-thread_self->t_rt_tls_size)*sizeof(DeeObject *));
  thread_self->t_rt_tls_size = new_size;
  thread_self->t_rt_tls_list = new_list;
  // No need to deallocate an old value (it didn't exit)
  Dee_INCREF(new_list[handle] = v);
  DeeAtomicMutex_Release(&thread_self->t_rt_lock);
  goto ret_factory;
 }
 // Get the old value (we need to decref it later)
 old_value = thread_self->t_rt_tls_list[handle];
 Dee_INCREF(thread_self->t_rt_tls_list[handle] = v); // Store the new value
 DeeAtomicMutex_Release(&thread_self->t_rt_lock);
#else /* !DEE_WITHOUT_THREADS */
 DeeObject *old_value;
 DEE_ASSERT(DeeObject_Check(self) && DeeTls_Check(self));
 old_value = DeeTls_VALUE(self);
 Dee_INCREF(((DeeTlsObject *)self)->tls_value = v);
#endif /* DEE_WITHOUT_THREADS */
 if (!old_value) {
#ifndef DEE_WITHOUT_THREADS
ret_factory:
#endif
  old_value = DeeObject_Call(DeeTls_FACTORY(self),Dee_EmptyTuple);
 }
 return old_value;
}





static void _deetls_tp_dtor(DeeTlsObject *self) {
#ifndef DEE_WITHOUT_THREADS
 DEE_ASSERT(self->tls_handle != (Dee_size_t)-1);
 _DeeFreeTlsID(self->tls_handle);
#else /* !DEE_WITHOUT_THREADS */
 Dee_XDECREF(self->tls_value);
#endif /* DEE_WITHOUT_THREADS */
 Dee_DECREF(self->tls_factory);
}
static int _deetls_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTlsObject *self) {
 return _DeeTls_InitWithFactory(self,Dee_None);
}
static int _deetls_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTlsObject *self, DeeTlsObject *right) {
 return _DeeTls_InitWithFactory(self,DeeTls_FACTORY(right));
}
static int _deetls_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTlsObject *self, DeeObject *args) {
 DeeObject *factory;
 if (DeeTuple_Unpack(args,"o:tls",&factory) == -1) return -1;
 return _DeeTls_InitWithFactory(self,factory);
}
static DeeObject *_deetls_tp_str(DeeTlsObject *self) {
 return DeeString_Newf("<tls: %O>",_DeeTls_LazyGet((DeeObject *)self));
}
static DeeObject *_deetls_tp_repr(DeeTlsObject *self) {
 return DeeString_Newf("<tls: %k -> %O>",
                       DeeTls_FACTORY(self),
                       _DeeTls_LazyGet((DeeObject *)self));
}
DEE_VISIT_PROC(_deetls_tp_visit,DeeTlsObject *self) {
 Dee_VISIT(self->tls_factory);
 Dee_XVISIT(_DeeTls_LazyGet((DeeObject *)self));
}
static DeeObject *_deetls_tp_not(DeeTlsObject *self) {
 DeeReturn_Bool(!DeeTls_Initialized((DeeObject *)self));
}
static int _deetls_tp_copy_assign(DeeTlsObject *self, DeeTlsObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTls_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeTls_Check(right));
 if (self != right) {
#ifndef DEE_WITHOUT_THREADS
  Dee_size_t new_handle = _DeeAllocTlsID();
  if (new_handle == (Dee_size_t)-1) return -1;
#endif /* !DEE_WITHOUT_THREADS */
  Dee_DECREF(self->tls_factory);
  Dee_INCREF(self->tls_factory = right->tls_factory);
#ifndef DEE_WITHOUT_THREADS
  self->tls_handle = new_handle;
#else /* !DEE_WITHOUT_THREADS */
  Dee_XDECREF(self->tls_value);
  Dee_XINCREF(self->tls_value = right->tls_value);
#endif /* DEE_WITHOUT_THREADS */
 }
 return 0;
}


#ifndef DEE_WITHOUT_THREADS
static DeeObject *_deetls_tp_cmp_lo(DeeTlsObject *lhs, DeeTlsObject *rhs) { if (DeeObject_InplaceGetInstance(&rhs,&DeeTls_Type) != 0) return NULL; DeeReturn_Bool(DeeTls_HANDLE(lhs) < DeeTls_HANDLE(rhs)); }
static DeeObject *_deetls_tp_cmp_le(DeeTlsObject *lhs, DeeTlsObject *rhs) { if (DeeObject_InplaceGetInstance(&rhs,&DeeTls_Type) != 0) return NULL; DeeReturn_Bool(DeeTls_HANDLE(lhs) <= DeeTls_HANDLE(rhs)); }
static DeeObject *_deetls_tp_cmp_eq(DeeTlsObject *lhs, DeeTlsObject *rhs) { if (DeeObject_InplaceGetInstance(&rhs,&DeeTls_Type) != 0) return NULL; DeeReturn_Bool(DeeTls_HANDLE(lhs) == DeeTls_HANDLE(rhs)); }
static DeeObject *_deetls_tp_cmp_ne(DeeTlsObject *lhs, DeeTlsObject *rhs) { if (DeeObject_InplaceGetInstance(&rhs,&DeeTls_Type) != 0) return NULL; DeeReturn_Bool(DeeTls_HANDLE(lhs) != DeeTls_HANDLE(rhs)); }
static DeeObject *_deetls_tp_cmp_gr(DeeTlsObject *lhs, DeeTlsObject *rhs) { if (DeeObject_InplaceGetInstance(&rhs,&DeeTls_Type) != 0) return NULL; DeeReturn_Bool(DeeTls_HANDLE(lhs) > DeeTls_HANDLE(rhs)); }
static DeeObject *_deetls_tp_cmp_ge(DeeTlsObject *lhs, DeeTlsObject *rhs) { if (DeeObject_InplaceGetInstance(&rhs,&DeeTls_Type) != 0) return NULL; DeeReturn_Bool(DeeTls_HANDLE(lhs) >= DeeTls_HANDLE(rhs)); }
#else /* !DEE_WITHOUT_THREADS */
static DeeObject *_deetls_tp_cmp_lo(DeeTlsObject *lhs, DeeTlsObject *rhs) { if (DeeObject_InplaceGetInstance(&rhs,&DeeTls_Type) != 0) return NULL; DeeReturn_Bool(lhs < rhs); }
static DeeObject *_deetls_tp_cmp_le(DeeTlsObject *lhs, DeeTlsObject *rhs) { if (DeeObject_InplaceGetInstance(&rhs,&DeeTls_Type) != 0) return NULL; DeeReturn_Bool(lhs <= rhs); }
static DeeObject *_deetls_tp_cmp_eq(DeeTlsObject *lhs, DeeTlsObject *rhs) { if (DeeObject_InplaceGetInstance(&rhs,&DeeTls_Type) != 0) return NULL; DeeReturn_Bool(lhs == rhs); }
static DeeObject *_deetls_tp_cmp_ne(DeeTlsObject *lhs, DeeTlsObject *rhs) { if (DeeObject_InplaceGetInstance(&rhs,&DeeTls_Type) != 0) return NULL; DeeReturn_Bool(lhs != rhs); }
static DeeObject *_deetls_tp_cmp_gr(DeeTlsObject *lhs, DeeTlsObject *rhs) { if (DeeObject_InplaceGetInstance(&rhs,&DeeTls_Type) != 0) return NULL; DeeReturn_Bool(lhs > rhs); }
static DeeObject *_deetls_tp_cmp_ge(DeeTlsObject *lhs, DeeTlsObject *rhs) { if (DeeObject_InplaceGetInstance(&rhs,&DeeTls_Type) != 0) return NULL; DeeReturn_Bool(lhs >= rhs); }
#endif /* DEE_WITHOUT_THREADS */




static DeeObject *_deetls_get(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":get") != 0) return NULL;
 return DeeTls_Get(self);
}
static DeeObject *_deetls_set(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *v;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:set",&v) != 0) return NULL;
 return DEE_UNLIKELY(DeeTls_Set(self,v) != 0) ? NULL : DeeNone_New();
}
static DeeObject *_deetls_del(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":del") != 0) return NULL;
 if DEE_UNLIKELY((result = DeeTls_Delete(self)) < 0) return NULL;
 DeeReturn_Bool(result == 0);
}
static DeeObject *_deetls_exchange(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *v;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:exchange",&v) != 0) return NULL;
 return DeeTls_Exchange(self,v);
}


static struct DeeMemberDef const _deetls_tp_members[] =  {
 DEE_MEMBERDEF_NAMED_RO_v100("factory",DeeTlsObject,tls_factory,object),
 DEE_MEMBERDEF_RO_v100(DeeTlsObject,tls_factory,object),
#ifdef DEE_WITHOUT_THREADS
 DEE_MEMBERDEF_RO_v100(DeeTlsObject,tls_value,object_null),
#else /* DEE_WITHOUT_THREADS */
 DEE_MEMBERDEF_RO_v100(DeeTlsObject,tls_handle,Dee_size_t),
#endif /* !DEE_WITHOUT_THREADS */
 DEE_MEMBERDEF_END_v100
};

static DeeObject *_deetls_value_get(
 DeeTlsObject *self, void *DEE_UNUSED(closure)) {
 return DeeTls_Get((DeeObject *)self);
}
static int _deetls_value_del(
 DeeTlsObject *self, void *DEE_UNUSED(closure)) {
 return DEE_UNLIKELY(DeeTls_Delete((DeeObject *)self) < 0) ? -1 : 0;
}
static int _deetls_value_set(
 DeeTlsObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 return DeeTls_Set((DeeObject *)self,v);
}

static struct DeeGetSetDef const _deetls_tp_getsets[] =  {
 DEE_GETSETDEF_v100("value",
  member(&_deetls_value_get),
  member(&_deetls_value_del),
  member(&_deetls_value_set),
  "value -> object"),
 DEE_GETSETDEF_END_v100
};
static struct DeeMethodDef const _deetls_tp_methods[] =  {
 DEE_METHODDEF_v100("get",member(&_deetls_get),"() -> object\n"
  "@throws object: same as factory\n"
  "\tReturns the object currently stored in the calling thread.\n"
  "\tIf no object was stored before a call to this function, "
    "the factory function set in the constructor is called to "
    "create a new instance, that will be returned"),
 DEE_METHODDEF_v100("set",member(&_deetls_set),"(object v) -> none\n"
  "\tSets the thread-locally stored value to 'v', where v can be any object.\n"
  "\tNOTE: The object will be overwritten, meaning that the __assign__ operator of any possible old objects will not be called"),
 DEE_METHODDEF_v100("del",member(&_deetls_del),"() -> bool\n"
  "@return: true if an object was stored prior to the call; false otherwise.\n"
  "\tDeletes the object stored in the calling thread and resets the state of the tls back to its original, uninitialized state.\n"
  "\tThe next call to 'get()' will call the factory as if the tls was never initialized in the current thread.\n"
  "\tNOTE: Calling this function has no effect if the value was already deleted or didn't exist.\n"),
 DEE_METHODDEF_v100("exchange",member(&_deetls_exchange),"(object v) -> object\n"
  "@throws object: same as factory\n"
  "@return: The object previously stored object\n"
  "\tExchanges the stored object with the same effect, as calling 'get', then 'set' would have."),
 DEE_METHODDEF_END_v100
};


static DEE_A_REF DeeObject *_deetlsclass_get_instance_count(
 DeeObject *DEE_UNUSED(self), DEE_A_INOUT_OBJECT(DeeTupleObject) *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":get_instance_count") != 0) return NULL;
 return DeeObject_New(Dee_size_t,DeeTls_InstanceCount());
}
static struct DeeMethodDef const _deetls_tp_class_methods[] =  {
 DEE_METHODDEF_CONST_v100("get_instance_count",member(&_deetlsclass_get_instance_count),
  "() -> size_t\n"
  "@returns: The amount of assigned TLS handles\n"
  "\tOnly meant for profiling: returns the amount of existing tls objects.\n"
  "\tThe returned value describes <tls-handles>, not <objects-stored-in-tls-containers>\n"),
 DEE_METHODDEF_END_v100
};



DeeTypeObject DeeTls_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("tls"),member(
  "(callable factory = none) -> tls\n"
  "\tCreate a thread_local_storage variable, to store any given object thread-local, weaning that every thread using the variable can assign its own value to be used.\n"
  "\t'factory' can be a callable object 'function() -> ...', that returns the initial object once requested.\n"
  "\tIf 'factory' is not specified, 'none' will be used as initial object\n"),
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeTlsObject),null,
  member(&_deetls_tp_ctor),
  member(&_deetls_tp_copy_ctor),null,
  member(&_deetls_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deetls_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deetls_tp_copy_assign),null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deetls_tp_str),
  member(&_deetls_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deetls_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deetls_tp_not),
  member(&DeeTls_Initialized),
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deetls_tp_cmp_lo),member(&_deetls_tp_cmp_le),
  member(&_deetls_tp_cmp_eq),member(&_deetls_tp_cmp_ne),
  member(&_deetls_tp_cmp_gr),member(&_deetls_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deetls_tp_members),member(_deetls_tp_getsets),member(_deetls_tp_methods),
  null,null,member(_deetls_tp_class_methods)),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DEE_DECL_END

#endif /* !GUARD_DEEMON_THREAD_TLS_C */
