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
#ifndef GUARD_DEEMON_MP_THREAD_H
#define GUARD_DEEMON_MP_THREAD_H 1

#include <deemon/__conf.inl>

#ifdef DEE_LIMITED_DEX
#include <deemon/optional/atomic_mutex.h>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/string_forward.h>
#include <deemon/optional/raised_exception.h>
#include <deemon/random.h>
#include <deemon/tuple.h>
#endif
#ifdef DEE_LIMITED_API
#ifndef GUARD_DEEMON_ERROR_H
#include <deemon/error.h>
#endif
#endif
#ifndef DEE_WITHOUT_THREADS
#if DEE_ENVIRONMENT_HAVE_INCLUDE_PTHREAD_H
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#include <pthread.h>
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif

DEE_OBJECT_DEF(DeeThreadObject);

#ifndef DEE_WITHOUT_THREADS
typedef Dee_uint8_t DeeThreadState; enum{
 DeeThreadState_INITIAL           = 0x00,
 DeeThreadState_STARTED           = 0x01, // Thread was started
 DeeThreadState_INTERRUPTED       = 0x02, // Interrupt signal was send
 DeeThreadState_INTERRUPT_HANDLED = 0x04, // Interrupt signal was received
 DeeThreadState_DETACHED          = 0x08, // Thread was detached / joined
 DeeThreadState_NOINTERRUPT       = 0x10, // Thread may not be interrupted right now
 DeeThreadState_TERMINATED        = 0x20, // Thread has run its course
 DeeThreadState_DEBUGGING         = 0x40, // The interactive debugger is running on this thread
};
#endif /* !DEE_WITHOUT_THREADS */

#ifdef DEE_LIMITED_DEX
struct DeeRaisedException;
struct DeeStackFrame;
struct DeeRecursionListEntry {
 DeeTypeObject const *rle_type; /*< [?..?] Object type. */
 DeeObject     const *rle_self; /*< [?..?] Object. */
};
struct DeeRecursionList {
 Dee_size_t                    rl_a; /*< Allocated size of the list. */
 Dee_size_t                    rl_c; /*< Size of the list. */
 struct DeeRecursionListEntry *rl_v; /*< [0..ob_size] Elements of the list. */
};
#ifdef DEE_LIMITED_API
#define DeeRecursionList_INIT() {0,0,NULL}
#define _DeeRecursionList_Init(ob)\
 ((ob)->rl_a=0,(ob)->rl_c=0,(ob)->rl_v=NULL)
#define _DeeRecursionList_Quit(ob) free((ob)->rl_v)
#define _DeeRecursionList_Pop(ob)    (--(ob)->rl_c)
#define _DeeRecursionList_Clear(ob)\
do{\
 _DeeRecursionList_Quit(ob);\
 _DeeRecursionList_Init(ob);\
}while(0)

extern DEE_A_RET_EXCEPT(-1) int _DeeRecursionList_Push(
 DEE_A_INOUT struct DeeRecursionList *self,
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_IN DeeObject const *ob) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_NOEXCEPT(0) int _DeeRecursionList_Contains(
 DEE_A_IN struct DeeRecursionList const *self,
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_IN DeeObject const *ob) DEE_ATTRIBUTE_NONNULL((1,2,3));
#endif /* DEE_LIMITED_API */


struct DeeThreadObject {
#ifndef DEE_WITHOUT_THREADS
 // HINT: This structure itself is the argument for the system-specific callback
 DEE_OBJECT_HEAD

 // Atomic variable describing the current state of the thread
 // NOTE: Flags are never removed and kept throughout the various stages
 // Exception: DeeThreadState_NOINTERRUPT|DeeThreadState_INTERRUPT_HANDLED
 // >> Before start():                 |=  DeeThreadState_INITIAL
 // >> After start():                  |=  DeeThreadState_STARTED
 // >> After join()/detach():          |=  DeeThreadState_DETACHED
 // >> After interrupt():              |=  DeeThreadState_INTERRUPTED
 // >> After check_interrupt():        |=  DeeThreadState_INTERRUPT_HANDLED
 // >> After return-from-callback:     |=  DeeThreadState_TERMINATED
 // >> After nointerrupt_begin():      |=  DeeThreadState_NOINTERRUPT
 // >> After nointerrupt_end():        &= ~DeeThreadState_NOINTERRUPT
 // >> After Signal.Interrupt.tp_dtor: &= ~DeeThreadState_INTERRUPT_HANDLED  // NOTE: Only if the signal was thrown by 'thread.check_interrupt()'
 /*atomic*/DeeThreadState   t_state;   /*< State of this thread. */

 // Member variables used at all times during the lifetime of a thread
 // >> These include the callback, name and system-dependent thread-handle
 struct DeeAtomicMutex      t_lock;
#ifdef DEE_PLATFORM_WINDOWS
 /*HANDLE*/void            *t_handle;        /*< [0..1][lock(t_lock)] System-specific thread handle. */ 
 /*DWORD*/ unsigned long    t_id;            /*< [lock(t_lock)] System-specific thread id. */ 
#else
 pthread_t                  t_handle;        /*< [lock(t_lock)] System-specific thread handle. */ 
#endif
 DEE_A_REF DeeObject       *t_callback_func; /*< [0..1][lock(t_lock)] Function to call. */
 DEE_A_REF DeeTupleObject  *t_callback_args; /*< [0..1][lock(t_lock)] Arguments to use for function call. */
 DEE_A_REF DeeStringObject *t_name;          /*< [0..1][lock(t_lock)] Optional name of the thread. */
 double                     t_priority;      /*< [lock(t_lock)] Priority before the thread was started / Initial priority. */
#endif /* !DEE_WITHOUT_THREADS */

 // Member variables associated with running deemon code in this thread
#ifndef DEE_WITHOUT_THREADS
 struct DeeAtomicMutex      t_frame_lock;    /*< Write-exclusive lock for 't_frame_last'. */
#endif /* !DEE_WITHOUT_THREADS */
#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
 Dee_size_t                 t_frame_size;    /*< Amount of recursively existing stack frames. */
#endif /* DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS */
 struct DeeStackFrame      *t_frame_last;    /*< [0..1][lock(t_frame_lock)] Top deemon code execution frame within the running thread. */

 // Support for recursive operators
 // NOTE: Technically these should be 'ob_rt_*' members,
 //       but since there isn't need to an interface for
 //       other threads to look at these, they are
 //       read/write exclusive to the running thread.
 struct DeeRecursionList    t_recursion_str;   /*< Recursion support for executing 'str(...)' */
 struct DeeRecursionList    t_recursion_repr;  /*< Recursion support for executing 'repr(...)' */
 unsigned int               t_nointerrupt_rec; /*< Recursion for nointerrupt. */

 // Thread-local runtime objects (TLS / thread-local random, thread return value)
#ifndef DEE_WITHOUT_THREADS
 struct DeeAtomicMutex      t_rt_lock;     /*< Lock for the following runtime-variables. */
 Dee_size_t                 t_rt_tls_size; /*< [lock(t_rt_lock)] Size of the following list. (Initially 0, grows as TLS vars are requested from within the thread). */
 DEE_A_REF DeeObject      **t_rt_tls_list; /*< [0..1][0..t_rt_tls_size][owned][lock(t_rt_lock)] List of thread TLS variables. */
 DEE_A_REF DeeObject       *t_rt_return;   /*< [0..1][lock(t_rt_lock)] Thread return value. */
 DEE_A_REF DeeRandomObject *t_rt_rand;     /*< [0..1][lock(t_rt_lock)] Thread-local random number generator (lazy_init). */
#endif /* !DEE_WITHOUT_THREADS */

 // Occurred exceptions in the thread
 // >> This is where we take our information about crash-errors from.
#ifndef DEE_WITHOUT_THREADS
 struct DeeAtomicMutex t_exception_lock;
#endif /* !DEE_WITHOUT_THREADS */
 /*  [0..1][lock(t_exception)]
            Linked list of raised exceptions
            in this thread (unhandled errors only). */
 // Write-exclusive for the running thread (until termination)
 struct DeeRaisedException  *t_exception;

#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
#ifndef DEE_WITHOUT_THREADS
 struct DeeAtomicMutex t_handling_exception_lock;
#endif /* !DEE_WITHOUT_THREADS */
 /* [0..1][lock(t_handling_exception_lock)]
           Linked list of exceptions currently being handled
           (most recently handled exception first).
           If this non-NULL it means that execution within the
           current thread is coming from a catch() block. */
 // Write-exclusive for the running thread (until termination)
 struct DeeRaisedException *t_handling_exception;
#endif

#ifndef DEE_WITHOUT_THREADS
 /* [0..1] Internally thrown interrupt signal (by 'check_interrupt()')
           (This one can update thread flags and interrupt cache on destruction). */
 struct DeeSignalObject *t_sys_interrupt;
#endif /* !DEE_WITHOUT_THREADS */
 // --end--
};
extern DEE_A_RET_NOEXCEPT(0) int _DeeThread_HasFrame(
 DEE_A_IN DeeThreadObject const *self,
 DEE_A_IN struct DeeStackFrame const *frame_) DEE_ATTRIBUTE_NONNULL((1,2));

#define DeeThread_PUSH_EXCEPTIONS(ob)\
do{ struct DeeRaisedException *_stored_exceptions; \
 if ((_stored_exceptions = ((DeeThreadObject *)Dee_REQUIRES_POINTER(ob))->t_exception) != NULL) {\
  DeeAtomicMutex_AcquireRelaxed(&((DeeThreadObject *)Dee_REQUIRES_POINTER(ob))->t_exception_lock);\
  ((DeeThreadObject *)(ob))->t_exception = NULL;\
  DeeAtomicMutex_Release(&((DeeThreadObject *)(ob))->t_exception_lock);\
 }if(0);else
#define DeeThread_BREAK_EXCEPTIONS(ob)\
do{\
 struct DeeRaisedException *_exc_end;\
 if (_stored_exceptions) {\
  if ((_exc_end = (ob)->t_exception) != NULL) {\
   while (_exc_end->re_prev) _exc_end = _exc_end->re_prev;\
   DeeAtomicMutex_AcquireRelaxed(&(ob)->t_exception_lock);\
   _exc_end->re_prev = _stored_exceptions;\
   DeeAtomicMutex_Release(&(ob)->t_exception_lock);\
  } else {\
   DeeAtomicMutex_AcquireRelaxed(&(ob)->t_exception_lock);\
   (ob)->t_exception = _stored_exceptions;\
   DeeAtomicMutex_Release(&(ob)->t_exception_lock);\
  }\
 }\
}while(0)
#define DeeThread_BREAK_EXCEPTIONS_NO_NEW(ob)\
do{\
 DEE_ASSERTF(!(ob)->t_exception,"New exceptions did occurr");\
 if (_stored_exceptions) {\
  DeeAtomicMutex_AcquireRelaxed(&(ob)->t_exception_lock);\
  (ob)->t_exception = _stored_exceptions;\
  DeeAtomicMutex_Release(&(ob)->t_exception_lock);\
 }\
}while(0)
#define DeeThread_POP_EXCEPTIONS(ob)\
 DeeThread_BREAK_EXCEPTIONS(ob);\
}while(0)
#define DeeThread_POP_EXCEPTIONS_NO_NEW(ob)\
 DeeThread_BREAK_EXCEPTIONS_NO_NEW(ob);\
}while(0)


#ifndef DEE_WITHOUT_THREADS
#define DeeThread_STATE(ob)       ((DeeThreadObject *)Dee_REQUIRES_POINTER(ob))->t_state
#define DeeThread_STATE_A(ob)     (DeeThreadState)DeeAtomic8_Load(DeeThread_STATE(ob),memory_order_acquire)
#define DeeThread_STARTED(ob)     ((DeeThread_STATE_A(ob)&DeeThreadState_STARTED)!=0)
#define DeeThread_DETACHED(ob)    ((DeeThread_STATE_A(ob)&DeeThreadState_DETACHED)!=0)
#define DeeThread_TERMINATED(ob)  ((DeeThread_STATE_A(ob)&DeeThreadState_TERMINATED)!=0)
#define DeeThread_INTERRUPTED(ob) ((DeeThread_STATE_A(ob)&DeeThreadState_INTERRUPTED)!=0)
#define DeeThread_NAME(ob)        (DeeObject *)((DeeThreadObject *)Dee_REQUIRES_POINTER(ob))->t_name
#define DeeThread_HANDLE(ob)      ((DeeThreadObject *)Dee_REQUIRES_POINTER(ob))->t_handle
#endif /* !DEE_WITHOUT_THREADS */
#else /* DEE_LIMITED_API */
#ifndef DEE_WITHOUT_THREADS
#define DeeThread_STATE_A(ob)     DeeThread_State((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeThread_STARTED(ob)     DeeThread_Started((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeThread_DETACHED(ob)    DeeThread_Detached((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeThread_TERMINATED(ob)  DeeThread_Terminated((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeThread_INTERRUPTED(ob) DeeThread_Interrupted((DeeObject *)Dee_REQUIRES_POINTER(ob))
#endif /* !DEE_WITHOUT_THREADS */
#endif /* !DEE_LIMITED_API */

#ifndef DEE_WITHOUT_THREADS
#define DeeThread_Check(ob)      DeeObject_InstanceOf(ob,&DeeThread_Type)
#define DeeThread_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeThread_Type)
DEE_DATA_DECL(DeeTypeObject) DeeThread_Type;


//////////////////////////////////////////////////////////////////////////
// Creates a new thread controller that, once started,
// will executed 'callback.operator () (args...)'.
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeThreadObject) *) DeeThread_New(
 DEE_A_INOUT DeeObject *callback, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2));


//////////////////////////////////////////////////////////////////////////
// Start the current thread
// >> returns -1: on error
// >> returns  0: on success
// >> returns  1: thread was already started
// NOTE: Can only be called once for every thread object
// NOTE: Thread objects cannot be recycled; you need a new thread object for that!
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeThread_Start(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self) DEE_ATTRIBUTE_NONNULL((1));
#endif /* !DEE_WITHOUT_THREADS */

#ifdef DEE_PRIVATE_DECL_DEE_THREAD_ID
DEE_PRIVATE_DECL_DEE_THREAD_ID
#undef DEE_PRIVATE_DECL_DEE_THREAD_ID
#endif

#ifndef DEE_WITHOUT_THREADS
//////////////////////////////////////////////////////////////////////////
// noexcept: Returns a unique (preferable meaningful) thread-id for "self"
// Returns (DeeThreadID)-1, but doesn't set an error if the thread wasn't started or is detached
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DeeThreadID) DeeThread_ID(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// noexcept: Returns a unique (preferable meaningful) thread-id for the calling thread
// - Same as "DeeThread_ID(DeeThread_Self())", but is normally faster
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DeeThreadID) DeeThread_SelfID(void);

//////////////////////////////////////////////////////////////////////////
// Check is the thread was already started
// - noexcept function (return 0 / 1)
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeThread_Started(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeThread_Detached(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeThread_Terminated(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeThread_Interrupted(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeThread_Crashed(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DeeThreadState) DeeThread_State(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Get exception information from a crashed thread
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeThread_GetError(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **exc,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **tb) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Interrupt a thread's execution
//  - Threads periodically check for interruptions
//    before they enter a blocking call.
//    If they are interrupted at that point, they will raise an
//    interrupt signal and unwind their calling stack using an exception
// >> returns -1: on error (e.g. thread wasn't started)
// >> returns  0: on success
// >> returns  1: thread was already interrupted
// NOTE: The thread must already be running
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeThread_Interrupt(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Detach from a running thread
// >> returns -1: on error (e.g. thread wasn't started)
// >> returns  0: on success
// >> returns  1: thread was already detached / joined
// NOTE: noop if thread was already detached
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeThread_Detach(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Joins the thread (the thread's return value is stored in *return_value if not NULL)
// Note: An Error.ThreadCrash is thrown if the thread terminated with an unhandled exception
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeThread_Join(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **return_value) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
//   returns -1: on error
//   returns  0: on success
//   returns  1: couldn't join the thread (timeout + it's still running)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeThread_TryJoin(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **return_value) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeThread_JoinTimed(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self, DEE_A_IN unsigned int msecs,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **return_value) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns the thread priority
// >> The priority is a floating point value clamped between 0.0 and 1.0
//   >> 0.0 is the lowest possible priority on the current platform
//   >> 1.0 is the highest possible priority on the current platform
//   >> 0.5 is the default priority
// >> Returns -1.0f on error
// NOTE: This function can be called before and after the start of the thread
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) double) DeeThread_GetPriority(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeThread_SetPriority(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self, DEE_A_IN double priority) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Thread name Interface
// NOTE: Threads cannot be renamed once started, and 'DeeThread_SetName'
//       will throw an 'Error.ValueError' it you attempt to.
DEE_FUNC_DECL(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeStringObject) *)
 DeeThread_GetName(DEE_A_IN_OBJECT(DeeThreadObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeThread_SetName(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1,2));


//////////////////////////////////////////////////////////////////////////
// Returns the Thread object of the current thread
// >> Returns NULL if the thread could not be retrieved (but doesn't
//    set an error, as exceptions require this function to work)
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE
              DEE_A_RET_OBJECT_MAYBE(DeeThreadObject) *) DeeThread_Self(void);
#define DeeThread_SELF   (DeeThreadObject *)DeeThread_Self
#endif

#if defined(DEE_WITHOUT_THREADS) && defined(DEE_LIMITED_API)
#ifndef DEE_PRIVATE_THREAD_SELF_DEFINED
#define DEE_PRIVATE_THREAD_SELF_DEFINED
extern DeeThreadObject _DeeThread_Self;
#define DeeThread_Self() (void*)&_DeeThread_Self
#define DeeThread_SELF() (&_DeeThread_Self)
#endif
#endif


#ifndef DEE_WITHOUT_THREADS
//////////////////////////////////////////////////////////////////////////
// Returns -1 and raises an error if an interrupt was received
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeThread_CheckInterrupt(void);
#else /* !DEE_WITHOUT_THREADS */
#define DeeThread_CheckInterrupt() 0
#endif /* DEE_WITHOUT_THREADS */

#ifndef DEE_PRIVATE_THREAD_YIELD_DEFINED
#define DEE_PRIVATE_THREAD_YIELD_DEFINED 1
//////////////////////////////////////////////////////////////////////////
// Yield for other threads
#ifndef DEE_WITHOUT_THREADS
DEE_FUNC_DECL(void) DeeThread_Yield(void);
#else /* !DEE_WITHOUT_THREADS */
#define DeeThread_Yield() (void)0
#endif /* DEE_WITHOUT_THREADS */
#endif /* !DEE_PRIVATE_THREAD_YIELD_DEFINED */

#ifndef DEE_PRIVATE_THREAD_SLEEP_DEFINED
#define DEE_PRIVATE_THREAD_SLEEP_DEFINED 1
DEE_FUNC_DECL(void) DeeThread_SleepNoInterrupt(DEE_A_IN unsigned int msecs);
#endif
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeThread_Sleep(DEE_A_IN unsigned int msecs);


#ifndef DEE_WITHOUT_THREADS
//////////////////////////////////////////////////////////////////////////
// Must be called if the calling thread is about to shut down
//  - Destroys all existing TLS instance in the calling thread
//  - Required for proper decref of existing TLS objects
//  - Until this is called, the calling thread owns a reference to every initialized TLS object
//  - If you have your own thread system, you have to call this at the end of your thread's lifetime
// NOTES:
//  - No-op if called multiple times
//  - Automatically called in deemon threads
//  - The main thread must call this, too (that's why this one's also called from the library finalizer)
// Returns 1 if the thread was shut down; returns 0 otherwise
DEE_FUNC_DECL(int) DeeThread_Shutdown(void);
#endif /* !DEE_WITHOUT_THREADS */

#ifndef DEE_WITHOUT_THREADS
//////////////////////////////////////////////////////////////////////////
// Begin / end a no_interrupt block in the current thread
DEE_FUNC_DECL(void) DeeThread_NoInterruptBegin(void);
DEE_FUNC_DECL(void) DeeThread_NoInterruptEnd(void);
#else /* !DEE_WITHOUT_THREADS */
#define DeeThread_NoInterruptBegin() (void)0
#define DeeThread_NoInterruptEnd() (void)0
#endif /* DEE_WITHOUT_THREADS */


#ifndef DEE_WITHOUT_THREADS
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(-1) int) DeeThread_Initialize(void);
DEE_FUNC_DECL(void) DeeThread_PrepareFinalize(void);
#endif /* !DEE_WITHOUT_THREADS */
DEE_FUNC_DECL(void) DeeThread_Finalize(void);


#ifndef DEE_WITHOUT_THREADS
//////////////////////////////////////////////////////////////////////////
// Platform-specific Thread functions
#ifdef DEE_PLATFORM_WINDOWS
struct DeeTimeObject;
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeThread_Win32Times(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **creation_time,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **exit_time,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **kernel_time,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **user_time)
 DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeThread_Win32PendingIO(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeThread_Win32GetPriorityBoost(
 DEE_A_IN_OBJECT(DeeThreadObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeThread_Win32SetPriorityBoost(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *self, DEE_A_IN int enabled) DEE_ATTRIBUTE_NONNULL((1));
#endif
#endif /* !DEE_WITHOUT_THREADS */


DEE_DECL_END

#ifndef DEE_WITHOUT_THREADS
#if defined(_INC_WINDOWS) && !defined(__INTELLISENSE__)
// Some optimizations, by inlining calls to windows functions
#define DeeThread_Yield             (void)SwitchToThread
#define DeeThread_SleepNoInterrupt  Sleep
#define DeeThread_SelfID            (DeeThreadID)GetCurrentThreadId
#endif
#endif /* !DEE_WITHOUT_THREADS */

#ifdef DEE_LIMITED_API
#ifndef DEE_WITHOUT_THREADS
extern /*atomic*/Dee_uint16_t _deethread_itrpcounter;
#ifdef DEE_DEBUG
#define DEE_THREADITRP_COUNTER_INC() (DeeAtomic16_FetchInc(_deethread_itrpcounter,memory_order_seq_cst)==0?DEE_LVERBOSE2("[Thread][+] Enable interrupt checks\n"):(void)0)
#define DEE_THREADITRP_COUNTER_DEC() (DeeAtomic16_DecFetch(_deethread_itrpcounter,memory_order_seq_cst)==0?DEE_LVERBOSE2("[Thread][-] Disable interrupt checks\n"):(void)0)
#else /* DEE_DEBUG */
#define DEE_THREADITRP_COUNTER_INC() (void)DeeAtomic16_IncFetch(_deethread_itrpcounter,memory_order_seq_cst)
#define DEE_THREADITRP_COUNTER_DEC() (void)DeeAtomic16_DecFetch(_deethread_itrpcounter,memory_order_seq_cst)
#endif /* !DEE_DEBUG */
#endif /* !DEE_WITHOUT_THREADS */
#endif /* DEE_LIMITED_API */


#endif /* !GUARD_DEEMON_MP_THREAD_H */
