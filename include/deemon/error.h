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
#ifndef GUARD_DEEMON_ERROR_H
#define GUARD_DEEMON_ERROR_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/std/stdarg.h>
#ifdef DEE_LIMITED_DEX
#include <deemon/object.h>
#include <deemon/tuple.h>
#include <deemon/string.h>
#include <deemon/mp/thread.h>
#ifdef DEE_WITHOUT_THREADS
#include <deemon/optional/raised_exception.h>
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
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Error Apis
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

DEE_OBJECT_DEF(DeeErrorObject);
DEE_OBJECT_DEF(DeeSignalObject);
DEE_OBJECT_DEF(DeeThreadCrashErrorObject);

typedef int Dee_errno_t; // Works under windows too, since 'sizeof(DWORD) == sizeof(int)'


#ifdef DEE_LIMITED_DEX
#define DEE_SIGNAL_OBJECT_HEAD \
 DEE_OBJECT_HEAD
#define DEE_EXCEPTION_OBJECT_HEAD \
 DEE_OBJECT_HEAD \
 DEE_A_REF struct DeeStringObject *ob_message; /*< [1..1] Exception message. */
struct DeeThreadObject;
struct DeeTracebackObject;
struct DeeErrorObject { DEE_EXCEPTION_OBJECT_HEAD };
struct DeeSignalObject { DEE_SIGNAL_OBJECT_HEAD };
struct DeeThreadCrashErrorObject {
 DEE_EXCEPTION_OBJECT_HEAD
 DEE_A_REF struct DeeThreadObject    *ob_thread;   /*< [1..1] The thread that crashed. */
 DEE_A_REF DeeObject                 *ob_inner;    /*< [1..1] The Inner exception that caused the thread to crash. */
 DEE_A_REF struct DeeTracebackObject *ob_inner_tb; /*< [1..1] Traceback of the inner exception. */
};
#endif

//struct DeeSystemErrorObject {
// DEE_EXCEPTION_OBJECT_HEAD // Message was returned by 'DeeSystemError_ToString'
// Dee_errno_t                       ob_code; /*< Error code that occurred. */
// DEE_A_REF struct DeeStringObject *ob_func; /*< [1..1] Function that caused the error. */
//};


#ifdef DEE_LIMITED_DEX
#define DeeError_NEW_EXTERN(name,ty,msg)\
DeeString_NEW_STATIC(_##name##_msg,msg);\
DeeErrorObject name = {\
 DEE_OBJECT_HEAD_INIT(ty),\
 (DeeStringObject *)&_##name##_msg\
}
#define DeeError_NEW_STATIC(name,ty,msg)\
DeeString_NEW_STATIC(_##name##_msg,msg);\
static DeeErrorObject name = {\
 DEE_OBJECT_HEAD_INIT(ty),\
 (DeeStringObject *)&_##name##_msg\
}
#endif

DEE_DATA_DECL(DeeSignalObject) _DeeErrorInstance_IterDone;
DEE_DATA_DECL(DeeErrorObject)  _DeeErrorInstance_OutOfMemory;
DEE_DATA_DECL(DeeErrorObject)  _DeeErrorInstance_NotImplemented;
DEE_DATA_DECL(DeeErrorObject)  _DeeErrorInstance_DivByZero;
DEE_DATA_DECL(DeeErrorObject)  _DeeErrorInstance_StackOverflow;
#define DeeErrorInstance_IterDone        ((DeeObject *)&_DeeErrorInstance_IterDone)
#define DeeErrorInstance_OutOfMemory     ((DeeObject *)&_DeeErrorInstance_OutOfMemory)
#define DeeErrorInstance_NotImplemented  ((DeeObject *)&_DeeErrorInstance_NotImplemented)
#define DeeErrorInstance_DivByZero       ((DeeObject *)&_DeeErrorInstance_DivByZero)
#define DeeErrorInstance_StackOverflow   ((DeeObject *)&_DeeErrorInstance_StackOverflow)

#ifdef DEE_LIMITED_DEX
DEE_DATA_DECL(DeeErrorObject) _DeeErrorInstance_FileCantRead;  // in "file.c"
DEE_DATA_DECL(DeeErrorObject) _DeeErrorInstance_FileCantWrite; // in "file.c"
#define DeeErrorInstance_FileCantRead  ((DeeObject *)&_DeeErrorInstance_FileCantRead)
#define DeeErrorInstance_FileCantWrite ((DeeObject *)&_DeeErrorInstance_FileCantWrite)
#endif


DEE_DATA_DECL(DeeTypeObject) DeeSignalType_Signal; // Base class for all signals
DEE_DATA_DECL(DeeTypeObject)  DeeSignalType_Interrupt;
// Internal signal that an iterator is done
// (used to signal iterator end to deemon user code)
// NOTE: Internally a simple NULL return value without an error set is used for this
DEE_DATA_DECL(DeeTypeObject)  DeeSignalType_StopIteration;

//////////////////////////////////////////////////////////////////////////
// Builtin exceptions that may be thrown by the runtime
// NOTE: Indentation and order are documenting bases
// The runtime name of an exception if composed of the portion behind the '_':
// A '.'-separated attribute-chain using all the shorter names.
// e.g.: 'DeeErrorType_NotImplemented' is named 'Error.RuntimeError.NotImplemented'
DEE_DATA_DECL(DeeTypeObject) DeeErrorType_Error; // Base class for all errors
DEE_DATA_DECL(DeeTypeObject)  DeeErrorType_AttributeError;
DEE_DATA_DECL(DeeTypeObject)  DeeErrorType_CompilerError;
DEE_DATA_DECL(DeeTypeObject)  DeeErrorType_ThreadCrash; // Thrown by join if the associated thread crashed
DEE_DATA_DECL(DeeTypeObject)  DeeErrorType_NoMemory;
DEE_DATA_DECL(DeeTypeObject)  DeeErrorType_RuntimeError;
DEE_DATA_DECL(DeeTypeObject)   DeeErrorType_NotImplemented;
DEE_DATA_DECL(DeeTypeObject)   DeeErrorType_AssertionError;
DEE_DATA_DECL(DeeTypeObject)   DeeErrorType_UnboundLocal;
DEE_DATA_DECL(DeeTypeObject)   DeeErrorType_StackOverflow;
DEE_DATA_DECL(DeeTypeObject)   DeeErrorType_SegFault;
DEE_DATA_DECL(DeeTypeObject)   DeeErrorType_IllegalInstruction;
DEE_DATA_DECL(DeeTypeObject)   DeeErrorType_DexError;
DEE_DATA_DECL(DeeTypeObject)  DeeErrorType_TypeError;
DEE_DATA_DECL(DeeTypeObject)  DeeErrorType_ValueError;
DEE_DATA_DECL(DeeTypeObject)   DeeErrorType_DivideByZero;
DEE_DATA_DECL(DeeTypeObject)   DeeErrorType_KeyError;
DEE_DATA_DECL(DeeTypeObject)   DeeErrorType_IndexError;
DEE_DATA_DECL(DeeTypeObject)   DeeErrorType_UnicodeError;
DEE_DATA_DECL(DeeTypeObject)   DeeErrorType_ReferenceError;
DEE_DATA_DECL(DeeTypeObject)  DeeErrorType_SystemError;
DEE_DATA_DECL(DeeTypeObject)  DeeErrorType_NetError; // TODO: Various sub-classes (e.g.: DeeErrorType_NetConnectRefused)
DEE_DATA_DECL(DeeTypeObject)  DeeErrorType_IOError;
DEE_DATA_DECL(DeeTypeObject)   DeeErrorType_FileNotFound;


//////////////////////////////////////////////////////////////////////////
// Returns an additional error object referring to an inner error of 'err'
// This function doesn't throw an error
// >> Internally, this simply returns 'err.inner()',
//    with some special optimizations for known types with inner errors (such as Error.ThreadCrash)
// NOTE: 'err' doesn't have to be derived from 'DeeErrorType_Error'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_NOEXCEPT_REF DeeObject *)
 DeeError_Inner(DEE_A_IN DeeObject *err) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_REQ_INIT DEE_A_RET_OBJECT_NOEXCEPT(DeeStringObject) *)
 DeeError_Message(DEE_A_IN_OBJECT(DeeErrorObject) *err) DEE_ATTRIBUTE_NONNULL((1));



//////////////////////////////////////////////////////////////////////////
// Raises "error" within the current thread
// >> Equivalent to what 'throw error;' does in usercode
// NOTE: 'error' can be an instance of anything
// NOTE: While this could technically fail, we'd have no way of
//       reporting the error if it does (DeeThread_Self() == NULL problem.)
//       Because of that, this function will do anything in its
//       power to throw the given error before giving up and
//       flipping the table (aborting with 'sys::_exit(1)')
//       Though you shouldn't worry about that. If it fails,
//       it is due to no more memory being available.
DEE_FUNC_DECL(DEE_A_EXEC void) DeeError_Throw(
 DEE_A_INOUT DeeObject *error) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Clears the internal cache of free raised_exception objects.
DEE_FUNC_DECL(DEE_A_REQ_INIT Dee_size_t) DeeError_ClearFreeExceptions(void);


//////////////////////////////////////////////////////////////////////////
// === Error Handling: Handle vs. HandleOne ===
//////////////////////////////////////////////////////////////////////////
//
// Both versions are usually safe to use in most situations, leaving code like...
// >> if (!DeeError_Handle(&DeeErrorType_Error)) return -1;
// ... to be just as standard-conforming and functional as:
// >> if (DeeError_HandleOne(&DeeErrorType_Error) == 0) return -1;
// The difference here is, that...
//
// CASE A (DeeError_Handle):
//   This version checks if the latest error is derived from 'Error',
//   returning '0' if it isn't before moving on to silently discarding
//   any errors that way also be thrown.
//   NOTE: Remember the difference between Occurred and Handling here... (this is about Occurred)
//
// CASE B (DeeError_HandleOne):
//   This version will start out just like the first one,
//   checking whether or not the latest error is derived from 'Error',
//   only featuring different return values, indicating a failure in
//   handling with '-1'.
//   If the occurred error does match, only the entry of that specific
//   error is removed, causing the function to either return '0' or '1'.
//   '0' is returned if everything's nice and dandy and no additional
//   error were thrown, as is possible with code like:
//   >> try {
//   >>   throw "A";
//   >> } finally {
//   >>   throw "B";
//   >> }
//   But if code like that above has caused more than
//   one error to appear, the function return '1'.
//   Note through, that in both cases one error has been handled, a fact
//   that the caller may choose to somewhat ignore, but never discard.
//   Because as long as there are unhandled exceptions, any function
//   with the capability of handling them may not signal success
//   to its caller until all errors are handled.
//   AGAIN: This does not interfere with calls to such functions
//          from exception handlers, or finally blocks, as
//          those will temporarily hide active exceptions and restore
//          them once they come to a close.
//
// For a practical example of usercode capable of detecting this
// difference, see '/lib/test/tests/test_multiexception_rt_handle.dee'
// 
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Signal all errors as handled
// - Returns 1: if an error was handled
// - Returns 0: if no error was handled
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_SUCCESS(return != 0) int) DeeError_Handled(void);

//////////////////////////////////////////////////////////////////////////
// Signal the top error as handled
// - Returns  1: if more error can be handled
// - Returns  0: if no more errors must be handled
// - Returns -1: if no error was handled
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_SUCCESS(return >= 0) int) DeeError_HandledOne(void);


//////////////////////////////////////////////////////////////////////////
// Handles a specific type of error
// - Returns  1: if an error with the given type was handled, but more errors are set and active
// - Returns  0: if an error with the given type was handled
// - Returns -1: if the top error didn't watch the given type
// >> Same behavior as a leaving 'catch()' with 'tp' as typing
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_SUCCESS(return >= 0) int) DeeError_HandleOne(
 DEE_A_IN DeeTypeObject const *tp) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Handles a specific type of error
// - Returns 1: if an error with the given type was handled
//             (Any other active errors will be silently discarded)
// - Returns 0: if the top error didn't watch the given type
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_SUCCESS(return != 0) int) DeeError_Handle(
 DEE_A_IN DeeTypeObject const *tp) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_REQ_INIT DEE_A_SUCCESS(return != 0) int) DeeError_CanHandle(
 DEE_A_IN DeeTypeObject const *tp) DEE_ATTRIBUTE_NONNULL((1));


//////////////////////////////////////////////////////////////////////////
// Simplified version of drop-in replacement for handling exceptions.
// Toggling this switches how exceptions are handled by the core (s.a.: 'Handle vs. HandleOne')
// NOTE: This does NOT affect how exceptions are handled in usercode. The standard
//       specifies that catch-blocks in usercode shall only ever handle one exception.
//       This is about the undefined behavior of how exceptions are handled by the runtime.
//        -> Choose the first version to have the core behave in legacy mode.
//        -> Choose the second version to mirror exception handling from usercode.
//       Since introduction in v102, the default behavior is mirroring usercode and not
//       dropping unnecessary exceptions while still handling the errors meant to be handled.
#if 0
#define DeeError_Catch(tp) (DeeError_Handle(tp))
#else
#define DeeError_Catch(tp) (DeeError_HandleOne(tp) == 0)
#endif


//////////////////////////////////////////////////////////////////////////
// Fills in 'exc' and 'tb' with information about the currently raised exception
// - Returns 0 if no exception was previously raised
// - Returns 1 otherwise
// NOTE: The values written to exc / tb are not references, so don't decref them!
// NOTE: The difference between occurred and handling is that handling returns
//       the error after user code started handling it in a catch block, while
//       occurred returns it before user code started handling it.
// NOTE: If user exception handling is disabled, both functions do the same.
// Difference 'DeeError_GetOccurred' / 'DeeError_GetHandling':
//  -> 'DeeError_GetOccurred' returns an error that
//      was just thrown and hasn't been caught yet.
//  -> 'DeeError_GetHandling' returns an error that was caught,
//      with code meant to handle that error currently being executed.
DEE_FUNC_DECL(DEE_A_REQ_INIT DEE_A_SUCCESS(return != 0) int) DeeError_GetOccurred(
 DEE_A_OUT_OPT DeeObject **exc, DEE_A_OUT_OPT DeeObject **tb);
DEE_FUNC_DECL(DEE_A_REQ_INIT DEE_A_SUCCESS(return != 0) int) DeeError_GetHandling(
 DEE_A_OUT_OPT DeeObject **exc, DEE_A_OUT_OPT DeeObject **tb);
DEE_FUNC_DECL(DEE_A_REQ_INIT DEE_A_SUCCESS(return != 0) int) DeeError_PopOccurred(
 DEE_A_REF DEE_A_OUT_OPT DeeObject **exc, DEE_A_REF DEE_A_OUT_OPT DeeObject **tb);


//////////////////////////////////////////////////////////////////////////
// Signal the top error as handled and store its values in '*exc' and '*tb'
// - Returns  1: if more error can be handled
// - Returns  0: if no more errors must be handled
// - Returns -1: if no error was handled
DEE_FUNC_DECL(DEE_A_REQ_INIT DEE_A_SUCCESS(return >= 0) int) DeeError_PopOccurredOne(
 DEE_A_REF DEE_A_OUT_OPT DeeObject **exc, DEE_A_REF DEE_A_OUT_OPT DeeObject **tb);

#ifdef DEE_LIMITED_API
extern DEE_A_EXEC DEE_A_REF DEE_A_RET_NEVER_NULL DeeTypeObject *_DeeError_HandleAndReturnType(void);
#endif

//////////////////////////////////////////////////////////////////////////
// Prints the latest error to stderr and handles it if 'handle_error' is non-zero
//  - Returns  0 on success
//  - Returns  1 if no error occurred that could have been printed
// If 'reason' is NULL, display "Unhandled exception\n" as reason instead
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_SUCCESS(return != 1) int) DeeError_Print(
 DEE_A_IN_Z_OPT char const *reason, DEE_A_IN int handle_error);
DEE_FUNC_DECL(DEE_A_EXEC void) DeeError_Display(
 DEE_A_IN_Z_OPT char const *reason, DEE_A_INOUT DeeObject *exc,
 DEE_A_IN_OBJECT(DeeTracebackObject) *tb) DEE_ATTRIBUTE_NONNULL((1,2,3));

//////////////////////////////////////////////////////////////////////////
// Returns the active, per-thread error object (NULL if no error occurred)
// NOTE: Try not to call this too often (it involves a TLS lookup and more stuff...)
DEE_FUNC_DECL(DEE_A_REQ_INIT DEE_A_RET_MAYBE_NULL DEE_A_RET_WUNUSED DeeObject *) DeeError_Occurred(void);

#ifndef DEE_PRIVATE_ERROR_OCCURRED_DEFINED
#define DEE_PRIVATE_ERROR_OCCURRED_DEFINED
//////////////////////////////////////////////////////////////////////////
// Returns true if an error occurred in the calling thread
// NOTE: Try not to call this too often (it involves a TLS lookup and more stuff...)
DEE_FUNC_DECL(DEE_A_REQ_INIT DEE_A_RET_WUNUSED int) _DeeError_Occurred(void);

#if defined(DEE_WITHOUT_THREADS) && defined(DEE_LIMITED_API)
#ifndef DEE_PRIVATE_THREAD_SELF_DEFINED
#define DEE_PRIVATE_THREAD_SELF_DEFINED 1
extern DeeThreadObject _DeeThread_Self;
#define DeeThread_Self() (void*)&_DeeThread_Self
#define DeeThread_SELF() (&_DeeThread_Self)
#endif
#define DeeError_Occurred() (_DeeThread_Self.ob_exception?_DeeThread_Self.ob_exception->ob_error:NULL)
#define DeeError_OCCURRED() (_DeeThread_Self.ob_exception!=NULL)
#else
#define DeeError_OCCURRED    _DeeError_Occurred
#endif
#endif


struct DeeRaisedException;
struct DeeThreadObject;
struct DeeErrorStateData {
 struct DeeThreadObject    *esd_threadself;        /*< [0..1] Calling thread control block. */
 struct DeeRaisedException *esd_stored_exceptions; /*< [0..1] List of stored exceptions. */
};

//////////////////////////////////////////////////////////////////////////
// Push/Pop the thread-local error state, temporarily
// creating an environment where no errors have occurred.
// NOTE: Unless you really know that you need this, and what this does, you probably won't need this...
extern void DeeError_PushState(DEE_A_OUT struct DeeErrorStateData *state) DEE_ATTRIBUTE_NONNULL((1));
extern void DeeError_PopState(DEE_A_IN struct DeeErrorStateData const *state) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Push the error-state around a block where exceptions need to be preserved:
// >> DeeError_PUSH_STATE() {
// >>   // If this call fails, handle all errors,
// >>   // except for those that already existed before.
// >>   if (SomeDangerousCall() != 0) {
// >>     DeeError_Handled();
// >>   }
// >> } DeeError_POP_STATE();
#ifndef DEE_LIMITED_DEX
#define DeeError_PUSH_STATE()\
do{\
 struct DeeErrorStateData _temp_state;\
 DeeError_PushState(&_temp_state)
#define DeeError_BREAK_STATE()\
 DeeError_PopState(&_temp_state)
#define DeeError_POP_STATE()\
 DeeError_BREAK_STATE();\
}while(0)
#else
#define DeeError_PUSH_STATE()\
do{\
 struct DeeErrorStateData _temp_state;\
 if DEE_LIKELY((_temp_state.esd_threadself = DeeThread_SELF()) != NULL) {\
  if ((_temp_state.esd_stored_exceptions = _temp_state.esd_threadself->t_exception) != NULL) {\
   DeeAtomicMutex_AcquireRelaxed(&_temp_state.esd_threadself->t_exception_lock);\
   _temp_state.esd_threadself->t_exception = NULL;\
   DeeAtomicMutex_Release(&_temp_state.esd_threadself->t_exception_lock);\
  }\
 } else {\
  _temp_state.esd_stored_exceptions = NULL;\
 } do
#define DeeError_BREAK_STATE()\
do{\
 if (_temp_state.esd_stored_exceptions) {\
  struct DeeRaisedException *_exc_end;\
  if ((_exc_end = _temp_state.esd_threadself->t_exception) != NULL) {\
   /* Prefix to existing exception chain. */\
   while (_exc_end->re_prev) _exc_end = _exc_end->re_prev;\
   DeeAtomicMutex_AcquireRelaxed(&_temp_state.esd_threadself->t_exception_lock);\
   _exc_end->re_prev = _temp_state.esd_stored_exceptions;\
   DeeAtomicMutex_Release(&_temp_state.esd_threadself->t_exception_lock);\
  } else {\
   /* Insert thread-local. */\
   DeeAtomicMutex_AcquireRelaxed(&_temp_state.esd_threadself->t_exception_lock);\
   _temp_state.esd_threadself->t_exception = _temp_state.esd_stored_exceptions;\
   DeeAtomicMutex_Release(&_temp_state.esd_threadself->t_exception_lock);\
  }\
 }\
}while(0)
#define DeeError_POP_STATE()\
 while(0); DeeError_BREAK_STATE();\
}while(0)
#endif

// Better syntax highlighting
#ifdef __INTELLISENSE__
//#undef DeeError_PUSH_STATE
//#undef DeeError_BREAK_STATE
//#undef DeeError_POP_STATE
//#define DeeError_PUSH_STATE()  if(1)do
//#define DeeError_BREAK_STATE() break
//#define DeeError_POP_STATE()   while(0)
#endif



#ifdef DEE_LIMITED_DEX
#ifdef __INTELLISENSE__
#define DeeError_SET_STRING(type_,message)\
do{\
 static char const _msg[] = (message);\
 static DeeTypeObject *const _tp = (type_);\
}while(0)
#else
#define DeeError_SET_STRING(type_,message)\
do{\
 DEE_COMPILER_MSVC_WARNING_PUSH(4640)\
 DeeString_NEW_STATIC(_error_message,message);\
 static DeeErrorObject _error_object = {\
  DEE_OBJECT_HEAD_INIT(type_),\
 (DeeStringObject *)&_error_message};\
 DeeError_Throw((DeeObject *)&_error_object);\
 DEE_COMPILER_MSVC_WARNING_POP\
}while(0)
#endif
#else
#define DeeError_SET_STRING(type_,message)\
 do{DeeError_SetString(type_,message);}while(0)
#endif

//////////////////////////////////////////////////////////////////////////
// Helper functions for throwing a given error:
//  - DeeError_SetNone(x):        Same as calling 'DeeError_Throw(DeeType_NewInstanceDefault(x))' (And also decref-ing the error instance and checking it for an error)
//  - DeeError_SetString(x,msg):  Same as calling 'DeeError_Throw(DeeType_NewInstancef(x,"s",msg))' (And also decref-ing the error instance and checking it for an error)
//  - DeeError_SetStringf(x,...): Same as calling 'DeeError_Throw(DeeType_NewInstancef(x,__VA_ARGS__))' (And also decref-ing the error instance and checking it for an error)
DEE_FUNC_DECL(DEE_A_EXEC void) DeeError_SetNone(
 DEE_A_IN DeeTypeObject *type_) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC void) DeeError_SetString(
 DEE_A_IN DeeTypeObject *type_, DEE_A_IN_Z char const *message) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC void) DeeError_SetStringf(
 DEE_A_IN DeeTypeObject *type_, DEE_A_IN_PRINTF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2));
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
DEE_FUNC_DECL(DEE_A_EXEC void) DeeError_VSetStringf(
 DEE_A_IN DeeTypeObject *type_, DEE_A_IN_PRINTF char const *fmt,
 DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */


#define DeeError_RuntimeError(message)\
 DeeError_SET_STRING(&DeeErrorType_RuntimeError,message)
#define DeeError_RuntimeErrorf(...)\
 DeeError_SetStringf(&DeeErrorType_RuntimeError,__VA_ARGS__)

#define DeeError_NoMemory()           DeeError_Throw(DeeErrorInstance_OutOfMemory)
#if defined(DEE_DEBUG) && defined(DEE_LIMITED_DEX)
// Add some more debug information in internal+dex source files 
#define DeeError_NotImplemented()     DeeError_SET_STRING(&DeeErrorType_NotImplemented,__FILE__ "(" DEE_PP_STR(__LINE__) ") : Not implemented")
#else
#define DeeError_NotImplemented()     DeeError_Throw(DeeErrorInstance_NotImplemented)
#endif
#define DeeError_NotImplementedf(...) DeeError_SetStringf(&DeeErrorType_NotImplemented,__VA_ARGS__)
#define DeeError_NotImplemented_str(name) \
 DeeError_SET_STRING(&DeeErrorType_NotImplemented,name)

#ifdef DEE_LIMITED_API
#if DEE_DEVEL_BUILD
extern void _DeeError_TODONotImplemented_impl(
 DEE_A_IN_Z char const *file_, DEE_A_IN int line_,
 DEE_A_IN_Z char const *func_);
#if defined(__GNUC__) || __has_builtin(__builtin_FUNCTION)
#define DeeError_TODONotImplemented() \
 _DeeError_TODONotImplemented_impl(__FILE__,__LINE__,__builtin_FUNCTION())
#elif defined(__FUNCTION__) || defined(_MSC_VER)
#define DeeError_TODONotImplemented() \
 _DeeError_TODONotImplemented_impl(__FILE__,__LINE__,__FUNCTION__)
#elif 0 /*< C11? */
#define DeeError_TODONotImplemented() \
 _DeeError_TODONotImplemented_impl(__FILE__,__LINE__,__func__)
#else
#define DeeError_TODONotImplemented() \
 _DeeError_TODONotImplemented_impl(__FILE__,__LINE__,"?")
#endif
#else
#define DeeError_TODONotImplemented  DeeError_NotImplemented
#endif
#endif

#ifdef DEE_LIMITED_DEX
#ifdef DEE_PLATFORM_WINDOWS
#define _DeeError_SystemErrorExplicit(type_,func_name,error) \
 DeeError_SetStringf(type_,"%k : %K",func_name,DeeSystemError_Win32ToString(error))
#define _DeeError_SystemError(type_,func_name) \
 _DeeError_SystemErrorExplicit(type_,func_name,DeeSystemError_Win32Consume())
#else
#define _DeeError_SystemErrorExplicit(type_,func_name,error) \
 DeeError_SetStringf(type_,"%k : %K",func_name,DeeSystemError_ToString(error))
#define _DeeError_SystemError(type_,func_name) \
 _DeeError_SystemErrorExplicit(type_,func_name,DeeSystemError_Consume())
#endif
#define DeeError_SystemError(func_name)              do{ DeeString_NEW_STATIC(_system_error_funcname,func_name); _DeeError_SystemError(&DeeErrorType_SystemError,(DeeObject *)&_system_error_funcname); }while(0)
#define DeeError_SystemErrorExplicit(func_name,code) do{ DeeString_NEW_STATIC(_system_error_funcname,func_name); _DeeError_SystemErrorExplicit(&DeeErrorType_SystemError,(DeeObject *)&_system_error_funcname,(code)); }while(0)
#define DeeError_IOError(func_name)                  do{ DeeString_NEW_STATIC(_system_error_funcname,func_name); _DeeError_SystemError(&DeeErrorType_IOError,(DeeObject *)&_system_error_funcname); }while(0)
#define DeeError_IOErrorExplicit(func_name,code)     do{ DeeString_NEW_STATIC(_system_error_funcname,func_name);_DeeError_SystemErrorExplicit(&DeeErrorType_IOError,(DeeObject *)&_system_error_funcname,(code)); }while(0)
#endif


//////////////////////////////////////////////////////////////////////////
// Convert an operating system error code into a string
//  - Used with 'errno' / 'DeeSystemError_Consume()'
// The returned string contains both the errno name, code and message
// NOTE: Function function requires the '_errno' dex to fully function
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *)
 DeeSystemError_ToString(DEE_A_IN Dee_errno_t code);

//////////////////////////////////////////////////////////////////////////
// Returns the current 'errno' system error, and sets it to 0
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_errno_t) DeeSystemError_Consume(void);

#ifdef DEE_PLATFORM_WINDOWS
//////////////////////////////////////////////////////////////////////////
// Convert an operating system error code into a string
//  - Used with 'GetLastError()' / 'DeeSystemError_Win32Consume()'
// The returned string contains both the errno name, code and message
// NOTE: Function function requires the '_errno' dex to fully function
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *)
 DeeSystemError_Win32ToString(DEE_A_IN unsigned long code);

//////////////////////////////////////////////////////////////////////////
// Returns the current 'GetLastError()' system error, and sets it to 0
DEE_FUNC_DECL(DEE_A_RET_WUNUSED unsigned long) DeeSystemError_Win32Consume(void);
#endif /* DEE_PLATFORM_WINDOWS */


#ifdef DEE_LIMITED_API
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
extern DEE_A_EXEC void _DeeError_DropOneHandlingException(
 DEE_A_IN_OBJECT(DeeThreadObject) *thread_self) DEE_ATTRIBUTE_NONNULL((1));
#endif
extern DeeErrorObject _DeeError_RuntimeError_NoActiveException;

#ifndef DEE_WITHOUT_THREADS
extern DEE_A_EXEC void DeeError_ThreadCrash(
 DEE_A_IN_OBJECT(DeeThreadObject) *thread) DEE_ATTRIBUTE_NONNULL((1));
#endif

extern DEE_A_EXEC void DeeError_AttributeError_NoDel(DEE_A_IN DeeTypeObject const *tp, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC void DeeError_AttributeError_NoRead(DEE_A_IN DeeTypeObject const *tp, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC void DeeError_AttributeError_NoWrite(DEE_A_IN DeeTypeObject const *tp, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC void DeeError_AttributeError_NotFound(DEE_A_IN DeeTypeObject const *tp, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
#define DeeError_AttributeError_NotFound_InType DeeError_AttributeError_NotFound
extern DEE_A_EXEC void DeeError_KeyError_NotFound(DEE_A_INOUT DeeObject *map, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC void DeeError_KeyError_NotFound_str(DEE_A_INOUT DeeObject *map, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC void DeeError_KeyError_NotFound_T(DEE_A_IN DeeTypeObject const *map, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC void DeeError_ValueError_NotFound(DEE_A_INOUT DeeObject *list, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2)); // Raised by .index functions
extern DEE_A_EXEC void DeeError_ValueError_NotFound_T(DEE_A_IN DeeTypeObject const *list, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2)); // Raised by .index functions
extern DEE_A_EXEC void DeeError_ValueError_NotFound_str_T(DEE_A_IN DeeTypeObject const *list, DEE_A_IN_Z char const *elem) DEE_ATTRIBUTE_NONNULL((1,2)); // Raised by .index functions
extern DEE_A_EXEC void DeeError_ValueError_NotFound_Pred(DEE_A_INOUT DeeObject *list, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2)); // Raised by .locate functions
extern DEE_A_EXEC void DeeError_ValueError_NotFound_Pred_N(DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1)); // Raised by .locate functions
extern DEE_A_EXEC void DeeError_ValueError_NotFound_Pred_T(DEE_A_IN DeeTypeObject const *list_type, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2)); // Raised by .locate functions
extern DEE_A_EXEC void DeeError_ValueError_EmptyList(DEE_A_IN DeeTypeObject const *list_type) DEE_ATTRIBUTE_NONNULL((1)); // Raised by .pop functions and tp_seq_get
extern DEE_A_EXEC void DeeError_IndexError(DEE_A_INOUT DeeObject *list, DEE_A_IN Dee_size_t index) DEE_ATTRIBUTE_NONNULL((1)); // Raised by .at functions
extern DEE_A_EXEC void DeeError_IndexError_T(DEE_A_IN DeeTypeObject const *tp, DEE_A_IN Dee_size_t index) DEE_ATTRIBUTE_NONNULL((1)); // Raised by .at functions
extern DEE_A_EXEC void DeeError_TypeError_TupleSizeExpected(DEE_A_IN Dee_size_t expected, DEE_A_IN Dee_size_t given);
extern DEE_A_EXEC void DeeError_TypeError_ArgCountExpected(DEE_A_IN Dee_size_t expected, DEE_A_IN Dee_size_t given);
extern DEE_A_EXEC void DeeError_TypeError_ArgCountExpectedNamed(DEE_A_IN_Z char const *fun_name, DEE_A_IN Dee_size_t expected, DEE_A_IN Dee_size_t given) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_EXEC void DeeError_TypeError_ArgCountExpectedNamedMin(DEE_A_IN_Z char const *fun_name, DEE_A_IN Dee_size_t min_expected, DEE_A_IN Dee_size_t given) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_EXEC void DeeError_TypeError_ArgCountExpectedNamedMax(DEE_A_IN_Z char const *fun_name, DEE_A_IN Dee_size_t max_expected, DEE_A_IN Dee_size_t given) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_EXEC void DeeError_TypeError_ArgCountExpectedNamedMinMax(DEE_A_IN_Z char const *fun_name, DEE_A_IN Dee_size_t min_expected, DEE_A_IN Dee_size_t max_expected, DEE_A_IN Dee_size_t given) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_EXEC void DeeError_TypeError_ArgCountExpectedMin(DEE_A_IN Dee_size_t min_expected, DEE_A_IN Dee_size_t given);
extern DEE_A_EXEC void DeeError_TypeError_ArgCountExpectedMax(DEE_A_IN Dee_size_t max_expected, DEE_A_IN Dee_size_t given);
extern DEE_A_EXEC void DeeError_TypeError_ArgCountExpectedMinMax(DEE_A_IN Dee_size_t min_expected, DEE_A_IN Dee_size_t max_expected, DEE_A_IN Dee_size_t given);
#define DeeError_TypeError_CheckArgSize(ob,s)    (DeeTuple_SIZE(ob)==(s)?0:(DeeError_TypeError_ArgCountExpected(s,DeeTuple_SIZE(ob)),-1))
extern DEE_A_EXEC void DeeError_TypeError_UnexpectedTypeT(
 DEE_A_IN DeeTypeObject const *expected_type,
 DEE_A_IN DeeTypeObject const *found_type) DEE_ATTRIBUTE_NONNULL((1,2));
#endif

#ifdef DEE_LIMITED_DEX
DEE_FUNC_DECL(DEE_A_EXEC void) DeeError_TypeError_UnexpectedType(
 DEE_A_INOUT DeeObject *ob, DEE_A_IN DeeTypeObject const *expected_type) DEE_ATTRIBUTE_NONNULL((1,2));
#define DeeError_TypeError_CheckType(ob,ty)      (DeeObject_InstanceOf(ob,ty)?0:(DeeError_TypeError_UnexpectedType(ob,ty),-1))
#define DeeError_TypeError_CheckTypeExact(ob,ty) (DeeObject_InstanceOfExact(ob,ty)?0:(DeeError_TypeError_UnexpectedType(ob,ty),-1))
#endif

#ifdef DEE_LIMITED_API
struct TPPLexerObject;
struct TPPTokenObject;
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeError_CompilerError(DEE_A_IN int code, DEE_A_IN_OBJECT(DeeLexerObject) *lexer, DEE_A_IN_OBJECT(DeeTokenObject) *token, DEE_A_IN_Z char const *message) DEE_ATTRIBUTE_NONNULL((2,3,4));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeError_CompilerErrorf(DEE_A_IN int code, DEE_A_IN_OBJECT(DeeLexerObject) *lexer, DEE_A_IN_OBJECT(DeeTokenObject) *token, DEE_A_IN_PRINTF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((2,3,4));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeError_TPPCompilerError(DEE_A_IN int code, DEE_A_IN struct TPPLexerObject *lexer, DEE_A_IN struct TPPTokenObject *token, DEE_A_IN_Z char const *message) DEE_ATTRIBUTE_NONNULL((2,3,4));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeError_TPPCompilerErrorf(DEE_A_IN int code, DEE_A_IN struct TPPLexerObject *lexer, DEE_A_IN struct TPPTokenObject *token, DEE_A_IN_PRINTF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((2,3,4));
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeError_VCompilerErrorf(DEE_A_IN int code, DEE_A_IN_OBJECT(DeeLexerObject) *lexer, DEE_A_IN_OBJECT(DeeTokenObject) *token, DEE_A_IN_PRINTF char const *fmt, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((2,3,4));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeError_VTPPCompilerErrorf(DEE_A_IN int code, DEE_A_IN struct TPPLexerObject *lexer, DEE_A_IN struct TPPTokenObject *token, DEE_A_IN_PRINTF char const *fmt, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((2,3,4));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_ERROR_H */
