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
#ifndef GUARD_DEEMON_MP_MUTEX_H
#define GUARD_DEEMON_MP_MUTEX_H 1

#include <deemon/__conf.inl>
#ifndef DEE_WITHOUT_THREADS
#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#endif

#ifndef DEE_PLATFORM_WINDOWS
#if DEE_ENVIRONMENT_HAVE_INCLUDE_SEMAPHORE_H
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#include <semaphore.h>
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_SEMAPHORE_H */
#endif

//////////////////////////////////////////////////////////////////////////
//
// WARNING:
//    -- The mutex API will be moved into a Dex sooner or later --
//   This header will remain, and it will still be possible to use the API,
//   but additional code may be required to load the API before use,
//   as well as binary compatibility with any function/object declared
//   by this header being broken.
//   NOTE: This will only affect the object-implementation, meaning
//         that functions like 'DeeMutex_Acquire' will be moved,
//         while functions like 'DeeNativeMutex_Acquire' will stay.
//         (only the ones without 'Native' in their name will be moved)
//
//////////////////////////////////////////////////////////////////////////

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_THREAD_ID
DEE_PRIVATE_DECL_DEE_THREAD_ID
#undef DEE_PRIVATE_DECL_DEE_THREAD_ID
#endif

DEE_OBJECT_DEF(DeeSemaphoreObject);
DEE_OBJECT_DEF(DeeMutexObject);

//////////////////////////////////////////////////////////////////////////
// Native Semaphore object
#ifndef DEE_TYPES_SIZEOF_SEMPAHORE_COUNT
#define DEE_TYPES_SIZEOF_SEMPAHORE_COUNT DEE_TYPES_SIZEOF_LONG
typedef unsigned long DeeSempahoreCount;
#endif

struct DeeNativeSemaphore {
#ifdef DEE_PLATFORM_WINDOWS
 /*HANDLE*/void *s_handle;
#else
 sem_t           s_handle;
#endif
};

DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeNativeSemaphore_Init(DEE_A_OUT struct DeeNativeSemaphore *self, DEE_A_IN DeeSempahoreCount initial) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeNativeSemaphore_Quit(DEE_A_IN struct DeeNativeSemaphore *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeNativeSemaphore_TryAcquire(DEE_A_INOUT struct DeeNativeSemaphore *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) DeeNativeSemaphore_Acquire(DEE_A_INOUT struct DeeNativeSemaphore *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeNativeSemaphore_AcquireTimed(DEE_A_INOUT struct DeeNativeSemaphore *self, DEE_A_IN unsigned int msecs) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeNativeSemaphore_Release(DEE_A_INOUT struct DeeNativeSemaphore *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeNativeSemaphore_ReleaseMultiple(DEE_A_INOUT struct DeeNativeSemaphore *self, DEE_A_IN DeeSempahoreCount count) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeNativeSemaphore_GetValue(DEE_A_INOUT struct DeeNativeSemaphore *self, DEE_A_OUT DeeSempahoreCount *value) DEE_ATTRIBUTE_NONNULL((1,2));

DEE_FUNC_DECL(void) DeeNativeSemaphore_AcquireNoexcept(DEE_A_INOUT struct DeeNativeSemaphore *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeNativeSemaphore_ReleaseNoexcept(DEE_A_INOUT struct DeeNativeSemaphore *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(1) int) DeeNativeSemaphore_TryAcquireNoExcept(DEE_A_INOUT struct DeeNativeSemaphore *self) DEE_ATTRIBUTE_NONNULL((1));


//////////////////////////////////////////////////////////////////////////
// Native Mutex object
struct DeeNativeMutex {
 /*atomic*/ unsigned int   m_counter;
            DeeThreadID    m_owner;
 unsigned int              m_recursion;
 struct DeeNativeSemaphore m_semaphore;
};

DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeNativeMutex_Init(DEE_A_OUT struct DeeNativeMutex *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeNativeMutex_Quit(DEE_A_IN struct DeeNativeMutex *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(1) int) DeeNativeMutex_TryAcquireNoexcept(DEE_A_INOUT struct DeeNativeMutex *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeNativeMutex_AcquireNoexcept(DEE_A_INOUT struct DeeNativeMutex *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeNativeMutex_ReleaseNoexcept(DEE_A_INOUT struct DeeNativeMutex *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) DeeNativeMutex_Acquire(DEE_A_INOUT struct DeeNativeMutex *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeNativeMutex_AcquireTimed(DEE_A_INOUT struct DeeNativeMutex *self, DEE_A_IN unsigned int msecs) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeNativeMutex_Release(DEE_A_INOUT struct DeeNativeMutex *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeNativeMutex_ReleaseWithError(DEE_A_INOUT struct DeeNativeMutex *self) DEE_ATTRIBUTE_NONNULL((1));
// v When using this, please still check for errors signaled with -1 (in case I add some in the future)
#define DeeNativeMutex_TryAcquire DeeNativeMutex_TryAcquireNoexcept



#ifdef DEE_LIMITED_API
struct DeeSemaphoreObject {
 DEE_OBJECT_HEAD
 struct DeeNativeSemaphore s_semaphore;
};
#define DeeSemaphore_SEMAPHORE(ob) (&((DeeSemaphoreObject *)Dee_REQUIRES_POINTER(ob))->s_semaphore)
#else
#define DeeSemaphore_SEMAPHORE(ob) DeeSemaphore_Semaphore((DeeObject *)Dee_REQUIRES_POINTER(ob))
#endif

#define DeeSemaphore_Check(ob)      DeeObject_InstanceOf(ob,&DeeSemaphore_Type)
#define DeeSemaphore_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeSemaphore_Type)
DEE_DATA_DECL(DeeTypeObject) DeeSemaphore_Type;
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeSemaphoreObject) *) DeeSemaphore_New(DEE_A_IN DeeSempahoreCount initial);
DEE_FUNC_DECL(DEE_A_RET_NEVER_NULL struct DeeNativeSemaphore  *) DeeSemaphore_Semaphore(DEE_A_IN_OBJECT(DeeSemaphoreObject) *self) DEE_ATTRIBUTE_NONNULL((1));

#define DeeSemaphore_Acquire(ob)               DeeNativeSemaphore_Acquire(DeeSemaphore_SEMAPHORE(ob))
#define DeeSemaphore_AcquireTimed(ob,msecs)    DeeNativeSemaphore_AcquireTimed(DeeSemaphore_SEMAPHORE(ob),msecs)
#define DeeSemaphore_TryAcquire(ob)            DeeNativeSemaphore_TryAcquire(DeeSemaphore_SEMAPHORE(ob))
#define DeeSemaphore_Release(ob)               DeeNativeSemaphore_Release(DeeSemaphore_SEMAPHORE(ob))
#define DeeSemaphore_ReleaseMultiple(ob,count) DeeNativeSemaphore_ReleaseMultiple(DeeSemaphore_SEMAPHORE(ob),count)
#define DeeSemaphore_GetValue(ob,value)        DeeNativeSemaphore_GetValue(DeeSemaphore_SEMAPHORE(ob),value)



#ifdef DEE_LIMITED_API
struct DeeMutexObject {
 DEE_OBJECT_HEAD
 struct DeeNativeMutex m_mutex;
};
#define DeeMutex_MUTEX(ob)               (&((DeeMutexObject *)Dee_REQUIRES_POINTER(ob))->m_mutex)
#define DeeMutex_ACQUIRE(ob)             DeeNativeMutex_Acquire(DeeMutex_MUTEX(ob))
#define DeeMutex_ACQUIRE_TIMED(ob,msecs) DeeNativeMutex_AcquireTimed(DeeMutex_MUTEX(ob),msecs)
#define DeeMutex_TRY_ACQUIRE(ob)         DeeNativeMutex_TryAcquire(DeeMutex_MUTEX(ob))
#define DeeMutex_RELEASE(ob)             DeeNativeMutex_Release(DeeMutex_MUTEX(ob))
#else
#define DeeMutex_ACQUIRE(ob)             DeeMutex_Acquire((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeMutex_ACQUIRE_TIMED(ob,msecs) DeeMutex_AcquireTimed((DeeObject *)Dee_REQUIRES_POINTER(ob),msecs)
#define DeeMutex_TRY_ACQUIRE(ob)         DeeMutex_TryAcquire((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeMutex_RELEASE(ob)             DeeMutex_Release((DeeObject *)Dee_REQUIRES_POINTER(ob))
#endif

#define DeeMutex_Check(ob)      DeeObject_InstanceOf(ob,&DeeMutex_Type)
#define DeeMutex_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeMutex_Type)
DEE_DATA_DECL(DeeTypeObject) DeeMutex_Type;

DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeMutexObject) *) DeeMutex_New(void);
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) DeeMutex_Acquire(DEE_A_INOUT_OBJECT(DeeMutexObject) *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeMutex_AcquireTimed(DEE_A_INOUT_OBJECT(DeeMutexObject) *self, DEE_A_IN unsigned int msecs) DEE_ATTRIBUTE_NONNULL((1)); // Returns same as Semaphore
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeMutex_TryAcquire(DEE_A_INOUT_OBJECT(DeeMutexObject) *self) DEE_ATTRIBUTE_NONNULL((1)); // Returns same as NativeSemaphore
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeMutex_Release(DEE_A_INOUT_OBJECT(DeeMutexObject) *self) DEE_ATTRIBUTE_NONNULL((1));

DEE_DECL_END
#endif /* !DEE_WITHOUT_THREADS */

#endif /* !GUARD_DEEMON_MP_MUTEX_H */
