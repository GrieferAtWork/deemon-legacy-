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
#ifndef GUARD_DEEMON_MUTEX_C
#define GUARD_DEEMON_MUTEX_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

#ifndef DEE_WITHOUT_THREADS
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#ifdef DEE_PLATFORM_WINDOWS
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#else
#include <errno.h>
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()

// /include/*
#include <deemon/__atomic_intrin.inl>
#include <deemon/bool.h>
#include <deemon/error.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/memberdef.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/mp/mutex.h>
#include <deemon/mp/thread.h>
#include <deemon/none.h>
#include <deemon/time.h> // DeeTime_GetClock1000

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/posix_features.inl>

// */ (nano...)

DEE_DECL_BEGIN

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeNativeSemaphore_AcquireTimed_Nointerrupt(
 DEE_A_INOUT struct DeeNativeSemaphore *self, DEE_A_IN unsigned int msecs) {
#ifdef DEE_PLATFORM_WINDOWS
 switch (WaitForSingleObject(self->s_handle,msecs)) {
  case WAIT_ABANDONED:
  case WAIT_OBJECT_0: return 0;
  case WAIT_TIMEOUT: return 1;
  default: break;
 }
 DeeError_SetStringf(&DeeErrorType_SystemError,
                     "WaitForSingleObject(%p,%u) : %K",self->s_handle,msecs,
                     DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
 return -1;
#elif DEE_HAVE_SEM_TIMEDWAIT
 struct timespec wait_time = {msecs,0};
 if DEE_UNLIKELY(sem_timedwait(&self->s_handle,&wait_time) != 0) {
  int error = DeeSystemError_Consume();
  if DEE_LIKELY(error == EAGAIN) return 1;
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "sem_timedwait(%u) : %K",msecs,
                      DeeSystemError_ToString(error));
  return -1;
 }
 return 0;
#else
 int result; Dee_uint64_t end_time = DeeTime_GetClock1000()+(Dee_uint64_t)msecs;
 while ((result = DeeNativeSemaphore_TryAcquire(self)) == 1 &&
        DeeTime_GetClock1000() < end_time
        ) if DEE_UNLIKELY((result = DeeThread_Sleep(1)) != 0) break;
 return result;
#endif
}

DEE_A_RET_EXCEPT(-1) int DeeNativeSemaphore_Init(
 DEE_A_OUT struct DeeNativeSemaphore *self, DEE_A_IN DeeSempahoreCount initial) {
 DEE_ASSERT(self);
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if DEE_UNLIKELY((self->s_handle = CreateSemaphoreW(NULL,(
  LONG)initial,(LONG)0x00008000l,NULL)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "CreateSemaphoreW(%lu) : %K",(unsigned long)initial,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
#else
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 if DEE_UNLIKELY(sem_init(&self->s_handle,0,initial) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "sem_init(%lu) : %K",(unsigned long)initial,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
#endif
 return 0;
}
void DeeNativeSemaphore_Quit(DEE_A_IN struct DeeNativeSemaphore *self) {
 DEE_ASSERT(self);
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(!CloseHandle(self->s_handle)) SetLastError(0);
#else
 if DEE_UNLIKELY(sem_destroy(&self->s_handle) != 0) errno = 0;
#endif
}
void DeeNativeSemaphore_AcquireNoexcept(DEE_A_INOUT struct DeeNativeSemaphore *self) {
#ifdef DEE_PLATFORM_WINDOWS
 WaitForSingleObject(self->s_handle,INFINITE);
#else
 DEE_ASSERT(self);
 if DEE_UNLIKELY(sem_wait(&self->s_handle) != 0) errno = 0;
#endif
}
void DeeNativeSemaphore_ReleaseNoexcept(DEE_A_INOUT struct DeeNativeSemaphore *self) {
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(!ReleaseSemaphore((HANDLE)self->s_handle,(LONG)1,NULL)) SetLastError(0);
#else
 DEE_ASSERT(self);
 if DEE_UNLIKELY(sem_post(&self->s_handle) != 0) errno = 0;
#endif
}

DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1)
int DeeNativeSemaphore_Acquire(DEE_A_INOUT struct DeeNativeSemaphore *self) {
#ifdef DEE_PLATFORM_WINDOWS
 DEE_ASSERT(self);
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0)  return -1;
 return _DeeNativeSemaphore_AcquireTimed_Nointerrupt(self,INFINITE);
#else
 DEE_ASSERT(self);
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0)  return -1;
 if DEE_UNLIKELY(sem_wait(&self->s_handle) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "sem_wait() : %K",
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
#endif
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeNativeSemaphore_AcquireTimed(
 DEE_A_INOUT struct DeeNativeSemaphore *self, DEE_A_IN unsigned int msecs) {
 DEE_ASSERT(self);
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 return _DeeNativeSemaphore_AcquireTimed_Nointerrupt(self,msecs);
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeNativeSemaphore_TryAcquire(
 DEE_A_INOUT struct DeeNativeSemaphore *self) {
 DEE_ASSERT(self);
#ifdef DEE_PLATFORM_WINDOWS
 return _DeeNativeSemaphore_AcquireTimed_Nointerrupt(self,0);
#else
 if DEE_UNLIKELY(sem_trywait(&self->s_handle) != 0) {
  int error = DeeSystemError_Consume();
  if DEE_LIKELY(error == EAGAIN) return 1;
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "sem_trywait() : %K",
                      DeeSystemError_ToString(error));
  return -1;
 }
 return 0;
#endif
}
DEE_A_RET_NOEXCEPT(1) int DeeNativeSemaphore_TryAcquireNoExcept(
 DEE_A_INOUT struct DeeNativeSemaphore *self) {
 DEE_ASSERT(self);
#ifdef DEE_PLATFORM_WINDOWS
 switch (WaitForSingleObject(self->s_handle,0)) {
  case WAIT_ABANDONED:
  case WAIT_OBJECT_0: return 0;
  case WAIT_TIMEOUT: return 1;
  default: break;
 }
 SetLastError(0);
 return 1;
#else
 if DEE_UNLIKELY(sem_trywait(&self->s_handle) != 0) {
  errno = 0;
  return 1;
 }
 return 0;
#endif
}

DEE_A_RET_EXCEPT(-1) int DeeNativeSemaphore_Release(DEE_A_INOUT struct DeeNativeSemaphore *self) {
#ifdef DEE_PLATFORM_WINDOWS
 return DeeNativeSemaphore_ReleaseMultiple(self,1);
#else
 DEE_ASSERT(self);
 if DEE_UNLIKELY(sem_post(&self->s_handle) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "sem_post(%p) : %K",self->s_handle,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNativeSemaphore_ReleaseMultiple(
 DEE_A_INOUT struct DeeNativeSemaphore *self, DEE_A_IN DeeSempahoreCount count) {
 DEE_ASSERT(self);
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(!ReleaseSemaphore((HANDLE)self->s_handle,(LONG)count,NULL)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "ReleaseSemaphore(%p,%lu) : %K",self->s_handle,count,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
#else
 while (count--) if DEE_UNLIKELY(DeeNativeSemaphore_Release(self) != 0) return -1;
#endif
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeNativeSemaphore_GetValue(
 DEE_A_INOUT struct DeeNativeSemaphore *self, DEE_A_OUT DeeSempahoreCount *value) {
#ifdef DEE_PLATFORM_WINDOWS
 DeeSempahoreCount prev_value;
 DEE_ASSERT(self); DEE_ASSERT(value);
 switch (WaitForSingleObject(self->s_handle,0)) {
  case WAIT_OBJECT_0:
   // Restore the ticket we just grabbed (and while doing this, we can figure out the value)
   if DEE_UNLIKELY(!ReleaseSemaphore(self->s_handle,1,(PLONG)&prev_value)) {
    DeeError_SetStringf(&DeeErrorType_SystemError,
                        "ReleaseSemaphore(%p,1) : %K",self,
                        DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
    return -1;
   }
   *value = prev_value+1;
   break;
  case WAIT_TIMEOUT:
   // Nothing to grab --> The value must be 0
   *value = 0;
   break;
  default:
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "WaitForSingleObject(%p,0) : %K",self,
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
   return -1;
 }
 return 0;
#else
 DEE_ASSERT(self);
 if DEE_UNLIKELY(sem_getvalue(&self->s_handle,(int *)value) < 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "sem_getvalue(%p) : %K",self->s_handle,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
#endif
}






DEE_A_RET_EXCEPT(-1) int DeeNativeMutex_Init(DEE_A_OUT struct DeeNativeMutex *self) {
 DEE_ASSERT(self);
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY((self->m_semaphore.s_handle = CreateSemaphoreW(NULL,0,1,NULL)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "CreateSemaphoreW(1) : %K",
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
#else
 if DEE_UNLIKELY(DeeNativeSemaphore_Init(&self->m_semaphore,0) != 0) return -1;
#endif
 self->m_counter = 0;
 self->m_owner = 0;
 self->m_recursion = 0;
 return 0;
}
void DeeNativeMutex_Quit(DEE_A_IN struct DeeNativeMutex *self) {
 DEE_ASSERT(self);
 DeeNativeSemaphore_Quit(&self->m_semaphore);
}
DEE_A_RET_NOEXCEPT(1) int DeeNativeMutex_TryAcquireNoexcept(DEE_A_INOUT struct DeeNativeMutex *self) {
 Dee_tid_t tid;
 DEE_ASSERT(self);
 tid = DeeThread_SelfID();
 if (self->m_owner == tid) {
  // Already inside the lock
  DeeAtomicInt_FetchInc(self->m_counter,memory_order_acquire);
 } else {
  if (!DeeAtomicInt_CompareExchangeStrong(
   self->m_counter,0,1,
   memory_order_seq_cst,memory_order_seq_cst
   )) return 1; // Enter failed
  // --- We are now inside the Lock ---
  self->m_owner = tid;
 }
 ++self->m_recursion;
 return 0;
}
void DeeNativeMutex_AcquireNoexcept(DEE_A_INOUT struct DeeNativeMutex *self) {
 Dee_tid_t tid;
 DEE_ASSERT(self);
 tid = DeeThread_SelfID();
 if (DeeAtomicInt_IncFetch(self->m_counter,memory_order_acquire) > 1) {
  if (tid != self->m_owner) DeeNativeSemaphore_AcquireNoexcept(&self->m_semaphore);
 }
 //--- We are now inside the Lock ---
 self->m_owner = tid;
 ++self->m_recursion;
}
void DeeNativeMutex_ReleaseNoexcept(DEE_A_INOUT struct DeeNativeMutex *self) {
 unsigned int recur;
 DEE_ASSERT(self);
 DEE_ASSERTF(self->m_owner == DeeThread_SelfID(),"Calling thread is not the owner");
 recur = --self->m_recursion;
 if (recur == 0) self->m_owner = 0;
 if (DeeAtomicInt_DecFetch(self->m_counter,memory_order_release) > 0) {
  if (recur == 0) DeeNativeSemaphore_ReleaseNoexcept(&self->m_semaphore);
 }
 //--- We are now outside the Lock ---
}

DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1)
int DeeNativeMutex_Acquire(DEE_A_INOUT struct DeeNativeMutex *self) {
 Dee_tid_t tid;
 DEE_ASSERT(self);
 tid = DeeThread_SelfID();
 if (DeeAtomicInt_IncFetch(self->m_counter,memory_order_acquire) > 1) {
  if (tid != self->m_owner) {
   if DEE_UNLIKELY(DeeNativeSemaphore_Acquire(&self->m_semaphore) != 0) {
    DeeAtomicInt_FetchDec(self->m_counter,memory_order_release);
    return -1;
   }
  }
 }
 //--- We are now inside the Lock ---
 self->m_owner = tid;
 ++self->m_recursion;
 return 0;
}
DEE_A_INTERRUPT DEE_A_RET_EXCEPT_FAIL(-1,1)
int DeeNativeMutex_AcquireTimed(DEE_A_INOUT struct DeeNativeMutex *self, DEE_A_IN unsigned int msecs) {
 Dee_tid_t tid; int error;
 DEE_ASSERT(self);
 tid = DeeThread_SelfID();
 if (DeeAtomicInt_IncFetch(self->m_counter,memory_order_acquire) > 1) {
  if (tid != self->m_owner) {
   if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) {
    // Interrupt received
    DeeAtomicInt_FetchDec(self->m_counter,memory_order_release);
    return -1;
   }
   if DEE_UNLIKELY((error = _DeeNativeSemaphore_AcquireTimed_Nointerrupt(&self->m_semaphore,msecs)) != 0) {
    DeeAtomicInt_FetchDec(self->m_counter,memory_order_release);
    return error;
   }
  }
 }
 //--- We are now inside the Lock ---
 self->m_owner = tid;
 ++self->m_recursion;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeNativeMutex_Release(DEE_A_INOUT struct DeeNativeMutex *self) {
 unsigned int recur;
 DEE_ASSERT(self);
 DEE_ASSERTF(self->m_owner == DeeThread_SelfID(),"Calling thread is not the owner");
 recur = --self->m_recursion;
 if (recur == 0) self->m_owner = 0;
 if (DeeAtomicInt_DecFetch(self->m_counter,memory_order_release) > 0) {
  if (recur == 0) {
   if DEE_UNLIKELY(DeeNativeSemaphore_Release(&self->m_semaphore) != 0) {
    DeeAtomicInt_FetchInc(self->m_counter,memory_order_acquire);
    self->m_owner = DeeThread_SelfID();
    self->m_recursion = 1;
    return -1;
   }
  }
 }
 //--- We are now outside the Lock ---
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeNativeMutex_ReleaseWithError(DEE_A_INOUT struct DeeNativeMutex *self) {
 unsigned int recur; Dee_tid_t tid;
 DEE_ASSERT(self); tid = DeeThread_SelfID();
 if (self->m_owner != tid) {
  DeeError_NEW_STATIC(_not_owner,&DeeErrorType_ValueError,"Calling thread is not the owner");
  DeeError_Throw((DeeObject *)&_not_owner);
  return -1;
 }
 recur = --self->m_recursion;
 if (recur == 0) self->m_owner = 0;
 if (DeeAtomicInt_DecFetch(self->m_counter,memory_order_release) > 0) {
  if (recur == 0) {
   if DEE_UNLIKELY(DeeNativeSemaphore_Release(&self->m_semaphore) != 0) {
    DeeAtomicInt_FetchInc(self->m_counter,memory_order_acquire);
    self->m_owner = tid;
    self->m_recursion = 1;
    return -1;
   }
  }
 }
 //--- We are now outside the Lock ---
 return 0;
}






DEE_A_RET_OBJECT_EXCEPT_REF(DeeSemaphoreObject) *
DeeSemaphore_New(DEE_A_IN DeeSempahoreCount initial) {
 DeeSemaphoreObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeSemaphoreObject,
  "Semaphore (%d initial)",(int)initial)) != NULL) {
  DeeObject_INIT(result,&DeeSemaphore_Type);
  if DEE_UNLIKELY(DeeNativeSemaphore_Init(&result->s_semaphore,initial) != 0) {
   _DeeObject_DELETE(&DeeSemaphore_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_NEVER_NULL struct DeeNativeSemaphore *
DeeSemaphore_Semaphore(DEE_A_IN_OBJECT(DeeSemaphoreObject) *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSemaphore_Check(self));
 return DeeSemaphore_SEMAPHORE(self);
}




DEE_A_RET_OBJECT_EXCEPT_REF(DeeMutexObject) *DeeMutex_New(void) {
 DeeMutexObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeMutexObject,"Mutex")) != NULL) {
  DeeObject_INIT(result,&DeeMutex_Type);
  if DEE_UNLIKELY(DeeNativeMutex_Init(&result->m_mutex) != 0) {
   _DeeObject_DELETE(&DeeMutex_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int
DeeMutex_Acquire(DEE_A_INOUT_OBJECT(DeeMutexObject) *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeMutex_Check(self));
 return DeeMutex_ACQUIRE(self);
}
DEE_A_INTERRUPT DEE_A_RET_EXCEPT_FAIL(-1,1) int
DeeMutex_AcquireTimed(DEE_A_INOUT_OBJECT(DeeMutexObject) *self, DEE_A_IN unsigned int msecs) {
 DEE_ASSERT(DeeObject_Check(self) && DeeMutex_Check(self));
 return DeeMutex_ACQUIRE_TIMED(self,msecs);
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeMutex_TryAcquire(DEE_A_INOUT_OBJECT(DeeMutexObject) *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeMutex_Check(self));
 return DeeMutex_TRY_ACQUIRE(self);
}
DEE_A_RET_EXCEPT(-1) int DeeMutex_Release(DEE_A_INOUT_OBJECT(DeeMutexObject) *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeMutex_Check(self));
 return DeeMutex_RELEASE(self);
}













int DEE_CALL _deesemaphore_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeSemaphoreObject *self, DeeTupleObject *args) {
 DeeSempahoreCount initial;
 DEE_ASSERT(DeeObject_Check(self) && DeeSemaphore_Check(self));
 if (DeeTuple_Unpack((DeeObject *)args,
  DEE_TUPLE_UNPACKFMT_UINT(DEE_TYPES_SIZEOF_SEMPAHORE_COUNT)
  ":semaphore",&initial) != 0) return -1;
 return DeeNativeSemaphore_Init(&self->s_semaphore,initial);
}
void DEE_CALL _deesemaphore_tp_dtor(DeeSemaphoreObject *self) {
 DeeNativeSemaphore_Quit(&self->s_semaphore);
}



int DEE_CALL _deemutex_tp_ctor(DeeTypeObject *DEE_UNUSED(tp_self), DeeMutexObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeMutex_Check(self));
 return DeeNativeMutex_Init(&self->m_mutex);
}
void DEE_CALL _deemutex_tp_dtor(DeeMutexObject *self) {
 DeeNativeMutex_Quit(&self->m_mutex);
}

static DEE_A_REF DeeObject *DEE_CALL _deesemaphore_acquire(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSemaphore_Check(self));
 if (DeeTuple_Unpack(args,":acquire") != 0 ||
     DeeSemaphore_Acquire(self) != 0) return NULL;
 DeeReturn_None;
}
static DEE_A_REF DeeObject *DEE_CALL _deesemaphore_acquire_timed(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 unsigned int msecs; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeSemaphore_Check(self));
 if (DeeTuple_Unpack(args,"u:acquire_timed",&msecs) != 0 ||
    (result = DeeSemaphore_AcquireTimed(self,msecs)) < 0) return NULL;
 DeeReturn_Bool(!result);
}
static DEE_A_REF DeeObject *DEE_CALL _deesemaphore_try_acquire(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeSemaphore_Check(self));
 if (DeeTuple_Unpack(args,":try_acquire") != 0 ||
    (result = DeeSemaphore_TryAcquire(self)) < 0) return NULL;
 DeeReturn_Bool(!result);
}
static DEE_A_REF DeeObject *DEE_CALL _deesemaphore_release(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeSempahoreCount count = 1;
 DEE_ASSERT(DeeObject_Check(self) && DeeSemaphore_Check(self));
 if (DeeTuple_Unpack(args,"|" DEE_TUPLE_UNPACKFMT_UINT(
  DEE_TYPES_SIZEOF_SEMPAHORE_COUNT) ":release",&count) != 0) return NULL;
 if ((count == 1 ? DeeSemaphore_Release(self)
  : DeeSemaphore_ReleaseMultiple(self,count)) != 0
  ) return NULL;
 DeeReturn_None;
}
static DEE_A_REF DeeObject *DEE_CALL _deesemaphore_value_get(
 DeeSemaphoreObject *self, void *DEE_UNUSED(closure)) {
 DeeSempahoreCount result;
 DEE_ASSERT(DeeObject_Check(self) && DeeSemaphore_Check(self));
 if (DeeSemaphore_GetValue(self,&result) != 0) return NULL;
 return DeeObject_New(DeeSempahoreCount,result);
}

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deesemaphore_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef _deesemaphore_tp_members[] = {
#ifdef DEE_PLATFORM_WINDOWS
 DEE_MEMBERDEF_NAMED_RO_v100("__s_semaphore_s_handle",DeeSemaphoreObject,s_semaphore.s_handle,HANDLE),
#endif
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */

static struct DeeMethodDef _deesemaphore_tp_methods[] = {
 DEE_METHODDEF_v100("acquire",member(&_deesemaphore_acquire),"() -> none\n"
  "@throws Signal.Interrupt: The calling thread was interrupted\n"
  "\tAcquires an ticket to @this semaphore.\n"
  "\tWARNING: If no more tickets are available and no running thread is able to provide some, a deadlock will occurr."),
 DEE_METHODDEF_v100("acquire_timed",member(&_deesemaphore_acquire_timed),"(unsigned int msecs) -> bool\n"
  "@param msecs: Timeout when waiting for a ticket to become available\n"
  "@return: True/false indicating a successfully acquired ticket\n"
  "@throws Signal.Interrupt: The calling thread was interrupted\n"
  "\tTries to acquire a ticket to #this semaphore, similar to #(this.acquire).\n"
  "\tIf the ticket could not be acquired after @msecs milliseconds have passed, return #false without acquiring a ticket.\n"
  "\tWARNING: If the calling thread is already holding a ticket to this semaphore, @msecs must pass before the function can return #(false)."),
 DEE_METHODDEF_v100("try_acquire",member(&_deesemaphore_try_acquire),"() -> bool\n"
  "@return: True/false indicating a successfully acquired ticket\n"
  "\tTries to acquire a ticket to #this semaphore, similar to #(this.acquire).\n"
  "\tIf the ticket could not immediatly be acquired, return #false without acquiring a ticket."),
 DEE_METHODDEF_v100("release",member(&_deesemaphore_release),"(semaphore_count n = 1) -> none\n"
  "@param n: The amount of tickets to be released\n"
  "@throws Error.ValueError: The calling thread isn't holding a ticket to #this mutex\n"
  "\tReleases a ticket previously acquired with #this.acquire, #this.acquire_timed or #this.try_acquire\n"
  "\tIf the calling thread isn't holding a ticket, an #Error.ValueError is thrown.\n"
  "\tThe process of releasing tickets is repeated #n times.\n"
  "\tWARNING: A thread must not hold a ticket in order to release one."),
 DEE_METHODDEF_END_v100
};
static struct DeeGetSetDef _deesemaphore_tp_getsets[] = {
 DEE_GETSETDEF_v100("value",member(&_deesemaphore_value_get),null,null,
  "-> semaphore_count\n@return: The amount of tickets available from this semaphore"),
 DEE_GETSETDEF_END_v100
};


DeeTypeObject DeeSemaphore_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("semaphore"),member(
  "(semaphore_count initial) -> semaphore\n"
  "\tInitializes a new semaphore containing @initial tickets available to be acquired."),null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeSemaphoreObject),null,null,null,null,
  member(&_deesemaphore_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deesemaphore_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,
  member(&_deegenericsameob_tp_hash)),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deegenericsameob_tp_cmp_lo),
  member(&_deegenericsameob_tp_cmp_le),
  member(&_deegenericsameob_tp_cmp_eq),
  member(&_deegenericsameob_tp_cmp_ne),
  member(&_deegenericsameob_tp_cmp_gr),
  member(&_deegenericsameob_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deesemaphore_tp_members),member(_deesemaphore_tp_getsets),
  member(_deesemaphore_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};

static DEE_A_REF DeeObject *DEE_CALL _deemutex_acquire(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DEE_ASSERT(DeeObject_Check(self) && DeeMutex_Check(self));
 if (DeeTuple_Unpack(args,":acquire") != 0 ||
     DeeMutex_ACQUIRE(self) != 0) return NULL;
 DeeReturn_None;
}
static DEE_A_REF DeeObject *DEE_CALL _deemutex_acquire_timed(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 unsigned int msecs; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeMutex_Check(self));
 if (DeeTuple_Unpack(args,"u:acquire_timed",&msecs) != 0 ||
    (result = DeeMutex_ACQUIRE_TIMED(self,msecs)) < 0) return NULL;
 DeeReturn_Bool(!result);
}
static DEE_A_REF DeeObject *DEE_CALL _deemutex_try_acquire(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeMutex_Check(self));
 if (DeeTuple_Unpack(args,":try_acquire") != 0 ||
    (result = DeeMutex_TRY_ACQUIRE(self)) < 0) return NULL;
 DeeReturn_Bool(!result);
}
static DEE_A_REF DeeObject *DEE_CALL _deemutex_release(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DEE_ASSERT(DeeObject_Check(self) && DeeMutex_Check(self));
 if (DeeTuple_Unpack(args,":release") != 0 ||
     DeeNativeMutex_ReleaseWithError(DeeMutex_MUTEX(self)) != 0
     ) return NULL;
 DeeReturn_None;
}

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deemutex_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef _deemutex_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__m_mutex_m_counter",DeeMutexObject,m_mutex.m_counter,atomic(uint)),
 DEE_MEMBERDEF_NAMED_RO_v100("__m_mutex_m_owner",DeeMutexObject,m_mutex.m_owner,Dee_tid_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__m_mutex_m_recursion",DeeMutexObject,m_mutex.m_recursion,uint),
#ifdef DEE_PLATFORM_WINDOWS
 DEE_MEMBERDEF_NAMED_RO_v100("__m_mutex_m_semaphore_s_handle",DeeMutexObject,m_mutex.m_semaphore.s_handle,HANDLE),
#endif
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */

static struct DeeMethodDef _deemutex_tp_methods[] = {
 DEE_METHODDEF_v100("acquire",member(&_deemutex_acquire),"() -> none\n"
  "@throws Signal.Interrupt: The calling thread was interrupted\n"
  "\tAcquires an exclusive lock to @this mutex.\n"
  "\tIf the calling thread was already holding an exclusive lock, this function returns "
    "immediatly and the calling thread must run #this.release twice to allow "
    "another thread to acquire the lock."),
 DEE_METHODDEF_v100("acquire_timed",member(&_deemutex_acquire_timed),
  "(unsigned int msecs) -> bool\n"
  "@param msecs: Timeout when waiting for an exclusive lock to become available\n"
  "@return: True/false indicating a successfully acquired lock\n"
  "@throws Signal.Interrupt: The calling thread was interrupted\n"
  "\tTries to acquire an exclusive lock to #this mutex, similar to #(this.acquire).\n"
  "\tIf the lock could not be acquired after @msecs milliseconds have passed, return #false without acquiring a lock.\n"
  "\tIf the calling thread was already holding an exclusive lock, return #true immediatly."),
 DEE_METHODDEF_v100("try_acquire",member(&_deemutex_try_acquire),"() -> bool\n"
  "@return: True/false indicating a successfully acquired lock\n"
  "\tTries to acquire an exclusive lock to #this mutex, similar to #(this.acquire).\n"
  "\tIf the lock could not immediatly be acquired, return #false without acquiring a lock.\n"
  "\tIf the calling thread was already holding an exclusive lock, return #true immediatly."),
 DEE_METHODDEF_v100("release",member(&_deemutex_release),"() -> none\n"
  "@throws Error.ValueError: The calling thread isn't holding a lock to #this mutex\n"
  "\tReleases a lock previously acquired with #this.acquire, #this.acquire_timed or #this.try_acquire\n"
  "\tIf the calling thread isn't holding a lock, an #Error.ValueError is thrown."),
 DEE_METHODDEF_END_v100
};



DeeTypeObject DeeMutex_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("mutex"),null,null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeMutexObject),null,
  member(&_deemutex_tp_ctor),null,null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deemutex_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,
  member(&_deegenericsameob_tp_hash)),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deegenericsameob_tp_cmp_lo),
  member(&_deegenericsameob_tp_cmp_le),
  member(&_deegenericsameob_tp_cmp_eq),
  member(&_deegenericsameob_tp_cmp_ne),
  member(&_deegenericsameob_tp_cmp_gr),
  member(&_deegenericsameob_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deemutex_tp_members),null,
  member(_deemutex_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DEE_DECL_END
#endif /* !DEE_WITHOUT_THREADS */

#endif /* !GUARD_DEEMON_MUTEX_C */
