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
#ifndef GUARD_DEEMON_ERROR_C
#define GUARD_DEEMON_ERROR_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#ifdef DEE_PLATFORM_WINDOWS
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#else
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#include <errno.h>
#endif
#include <deemon/optional/std/string.h>
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()

#include <deemon/compiler/lexer.h>
#include <deemon/error.h>
#include <deemon/dex.h>
#include <deemon/deemonrun.h>
#include <deemon/mp/thread.h>
#include <deemon/file.h>
#include <deemon/none.h>
#include <deemon/memberdef.h>
#include <deemon/optional/atomic_once.h>
#include <deemon/optional/object_visit.h>
#include <deemon/runtime/execute.h>
#include <deemon/string.h>
#include <deemon/traceback.h>
#include <deemon/tuple.h>
#include <deemon/unicode/char_traits.inl>
#include <deemon/value.h>

DEE_DECL_BEGIN

DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT_REF DeeObject *)
DeeError_InnerImpl(DEE_A_IN DeeObject *err) {
 DEE_ASSERT(DeeObject_Check(err));
 if DEE_UNLIKELY(DeeObject_InstanceOf(err,&DeeErrorType_ThreadCrash))
  DeeReturn_NEWREF(((DeeThreadCrashErrorObject *)err)->ob_inner);
 return NULL;
}
DEE_A_RET_NOEXCEPT_REF DeeObject *
DeeError_Inner(DEE_A_IN DeeObject *err) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(err));
 if DEE_UNLIKELY((result = DeeError_InnerImpl(err)) != NULL) return result;
 result = DeeObject_CallAttrStringf(err,"inner","");
 if DEE_UNLIKELY(!result) DeeError_Handled();
 return result;
}
DEE_A_RET_OBJECT_NOEXCEPT(DeeStringObject) *
DeeError_Message(DEE_A_IN_OBJECT(DeeErrorObject) *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeObject_InstanceOf(self,&DeeErrorType_Error));
 return (DeeObject *)((DeeErrorObject *)self)->ob_message;
}














DEE_A_RET_WUNUSED Dee_errno_t DeeSystemError_Consume(void) {
 Dee_errno_t result;
#if 1
 Dee_errno_t *errno_p = &errno;
 result = *errno_p;
 *errno_p = 0;
#else
 result = (Dee_errno_t)errno;
 errno = 0;
#endif
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeSystemError_ToString(DEE_A_IN Dee_errno_t code) {
 DeeObject *result,*errno_dex,*errno_name,*errno_text,*args;
 if DEE_UNLIKELY((errno_dex = DeeDex_Open("_errno")) == NULL) {
err:
  // Fallback if the dex is missing / defect
  if DEE_UNLIKELY(DeeError_Catch(&DeeErrorType_AttributeError))
   return DeeString_Newf("0x%.8X(%d)",code,code);
  return NULL;
 }
 if DEE_UNLIKELY((args = Dee_BuildTuple("d",code)) == NULL) { result = NULL; goto end_0; }
 if DEE_UNLIKELY((errno_name = DeeObject_CallAttrString(errno_dex,"errnoname",args)) == NULL) {err2: Dee_DECREF(errno_dex); Dee_DECREF(args); goto err; }
 if DEE_UNLIKELY((errno_text = DeeObject_CallAttrString(errno_dex,"errnotext",args)) == NULL) { Dee_DECREF(errno_name); goto err2; }
 result = DeeString_Newf("%k : 0x%.8X(%d) : %k",errno_name,code,code,errno_text);
 Dee_DECREF(errno_text);
 Dee_DECREF(errno_name);
 Dee_DECREF(args);
end_0: Dee_DECREF(errno_dex);
 return result;
}

#ifdef DEE_PLATFORM_WINDOWS
DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeSystemError_Win32ToString(DEE_A_IN unsigned long code) {
 DeeObject *result,*errno_dex,*errno_name,*errno_text,*args;
 if DEE_UNLIKELY((errno_dex = DeeDex_Open("_errno")) == NULL) {
err:
  // Fallback if the dex is missing / defect
  if (DeeError_Catch(&DeeErrorType_AttributeError))
   return DeeString_Newf("0x%.8X(%d)",code,code);
  return NULL;
 }
 if DEE_UNLIKELY((args = Dee_BuildTuple("lu",code)) == NULL) { result = NULL; goto end_0; }
 if DEE_UNLIKELY((errno_name = DeeObject_CallAttrString(errno_dex,"win32_errnoname",args)) == NULL) {err2: Dee_DECREF(errno_dex); Dee_DECREF(args); goto err; }
 if DEE_UNLIKELY((errno_text = DeeObject_CallAttrString(errno_dex,"win32_errnotext",args)) == NULL) { Dee_DECREF(errno_name); goto err2; }
 result = DeeString_Newf("%k : 0x%.8X(%d) : %k",errno_name,code,code,errno_text);
 Dee_DECREF(errno_text);
 Dee_DECREF(errno_name);
 Dee_DECREF(args);
end_0: Dee_DECREF(errno_dex);
 return result;
}
DEE_A_RET_WUNUSED unsigned long DeeSystemError_Win32Consume(void) {
 unsigned long result;
 result = GetLastError();
 SetLastError(0);
 return result;
}
#endif




static struct DeeAtomicMutex _dee_free_exceptions_lock = DeeAtomicMutex_INIT();
static struct DeeRaisedException *_dee_free_exceptions_pool = NULL;

DEE_A_RET_EXCEPT(NULL) struct DeeRaisedException *_DeeRaisedException_Alloc(void) {
 struct DeeRaisedException *result;
 DeeAtomicMutex_Acquire(&_dee_free_exceptions_lock);
 if DEE_LIKELY((result = _dee_free_exceptions_pool) != NULL) {
  _dee_free_exceptions_pool = result->re_prev;
  DeeAtomicMutex_Release(&_dee_free_exceptions_lock);
  return result;
 }
 DeeAtomicMutex_Release(&_dee_free_exceptions_lock);
 while DEE_UNLIKELY((result = (struct DeeRaisedException *)mallocf_nz(
  sizeof(struct DeeRaisedException),"Raised exception")) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 return result;
}
DEE_A_RET_NOEXCEPT(NULL) struct DeeRaisedException *
_DeeRaisedException_TryAlloc(void) {
 struct DeeRaisedException *result;
 DeeAtomicMutex_Acquire(&_dee_free_exceptions_lock);
 if DEE_LIKELY((result = _dee_free_exceptions_pool) != NULL) {
  _dee_free_exceptions_pool = result->re_prev;
  DeeAtomicMutex_Release(&_dee_free_exceptions_lock);
  return result;
 }
 DeeAtomicMutex_Release(&_dee_free_exceptions_lock);
 while DEE_UNLIKELY((result = (struct DeeRaisedException *)mallocf_nz(
  sizeof(struct DeeRaisedException),"Raised exception")) == NULL) {
  static /*atomic*/Dee_uint8_t collect_recursion = 0;
  // Make sure we don't get an infinite recursion in here
  if DEE_LIKELY(DeeAtomic8_CompareExchangeStrong(collect_recursion,
   0,1,memory_order_seq_cst,memory_order_seq_cst)) {
   Dee_size_t temp = Dee_CollectMemory();
   DeeAtomic8_Store(collect_recursion,0,memory_order_seq_cst);
   if (temp) continue;
  }
  return NULL;
 }
 return result;
}
DEE_A_RET_EXCEPT(NULL) struct DeeRaisedException *
_DeeRaisedException_CopyChain(DEE_A_IN struct DeeRaisedException *chain) {
 struct DeeRaisedException *result,*iter;
 DEE_ASSERT(chain);
 if DEE_LIKELY((result = _DeeRaisedException_Alloc()) != NULL) {
  iter = result; while (1) {
   _DeeRaisedException_InitCopy(iter,chain);
   if ((chain = chain->re_prev) == NULL) { iter->re_prev = NULL; break; }
   if DEE_UNLIKELY((iter->re_prev = _DeeRaisedException_Alloc()) == NULL) {
    _DeeRaisedException_DeleteChain(result);
    return NULL;
   }
   iter = iter->re_prev;
  }
 }
 return result;
}

void _DeeRaisedException_Free(
 DEE_A_IN DEE_A_OUT_INVALID struct DeeRaisedException *e) {
 DEE_ASSERT(e);
 DeeAtomicMutex_Acquire(&_dee_free_exceptions_lock);
 e->re_prev = _dee_free_exceptions_pool;
 _dee_free_exceptions_pool = e;
 DeeAtomicMutex_Release(&_dee_free_exceptions_lock);
}
void _DeeRaisedException_DeleteChain(
 DEE_A_IN DEE_A_OUT_INVALID struct DeeRaisedException *e) {
 struct DeeRaisedException *next;
 DEE_ASSERT(e);
 do {
  next = e->re_prev;
  _DeeRaisedException_Quit(e);
  _DeeRaisedException_Free(e);
 } while ((e = next) != NULL);
}


Dee_size_t DeeError_ClearFreeExceptions(void) {
 Dee_size_t result = 0;
 struct DeeRaisedException *iter,*next;
 DeeAtomicMutex_Acquire(&_dee_free_exceptions_lock);
 if DEE_LIKELY((iter = _dee_free_exceptions_pool) != NULL) {
  _dee_free_exceptions_pool = NULL;
  while (1) {
   next = iter->re_prev;
   free_nn(iter);
   ++result;
   if (!next) break;
   iter = next;
  }
 }
 DeeAtomicMutex_Release(&_dee_free_exceptions_lock);
 return result;
}


#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
void _DeeError_DropOneHandlingException(
 DEE_A_IN_OBJECT(DeeThreadObject) *thread_self) {
 struct DeeRaisedException *drop_exception;
#ifndef DEE_WITHOUT_THREADS
 DEE_ASSERT(DeeObject_Check(thread_self) && DeeThread_Check(thread_self));
#endif
 drop_exception = ((DeeThreadObject *)thread_self)->t_handling_exception;
 DEE_ASSERTF(drop_exception,"Not handling any exceptions right now");
 DeeAtomicMutex_Acquire(&((DeeThreadObject *)thread_self)->t_handling_exception_lock);
 ((DeeThreadObject *)thread_self)->t_handling_exception = drop_exception->re_prev;
 DeeAtomicMutex_Release(&((DeeThreadObject *)thread_self)->t_handling_exception_lock);
 // Drop 1 exception from the list of currently handled exceptions
 DEE_LVERBOSE2("Destroying handling_exception: %k\n",drop_exception->re_error);
 _DeeRaisedException_Quit(drop_exception);
 _DeeRaisedException_Free(drop_exception);
}
#endif

void DeeError_Throw(DEE_A_INOUT DeeObject *error) {
 struct DeeRaisedException *entry;
 DeeThreadObject *thread_self;
 DEE_LVERBOSE1("Throwing error: %k\n",error);
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL) return;
 // TODO: Recursion on out-of-memory
 if DEE_UNLIKELY((entry = _DeeRaisedException_TryAlloc()) == NULL) {
  // Even getting here is really hard:
  //  1. Practically no heap memory must be left
  //  2. No stored exceptions can be available
  if (DeeObject_InstanceOf(error,&DeeErrorType_NoMemory)) {
   // This is really bad!
   // >> The only chance we have at fixing this, is to
   //    overwrite an existing exception. ~cringes...~
   // But if you look below, it's still better than just flipping the table immediately.
   if DEE_LIKELY((entry = thread_self->t_exception) != NULL) {
    DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
    thread_self->t_exception = entry->re_prev;
    DeeAtomicMutex_Release(&thread_self->t_exception_lock);
    DEE_ATOMIC_ONCE({
     DEE_ATTRIBUTE_UNUSED int unused;
     unused = DeeFile_STDPRINT(DEE_STDERR,
                               "!!!WARNING!!! Re-using existing exception to "
                               "throw out-of-memory (this could end badly)\n");
    });
    _DeeRaisedException_Quit(entry);
   } else {
    // $H17! What can we try now?
    // Like seriously! We f*ed up bad enough to not even
    // be able to allocate a like 12-24 byte structure.
    // Let's hope we can still print this to stderr...
    // NOTE: The atomic_once is here in case the print
    //       somehow gets the idea of raising another error.
    //       So by making sure we only yell this once,
    //       we prevent an infinite loop ending in a stack overflow.
    DEE_ATOMIC_ONCE({
     DEE_ATTRIBUTE_UNUSED int unused;
     unused = DeeFile_STDPRINT(DEE_STDERR,
                               "!!!CRITICAL!!! Out-of-memory so bad that "
                               "we can't even raise an error about it\n");
    });
    // Screw dis! (╯°□°）╯︵ ┻━┻
    Dee__Exit(1);
   }
  } else {
   DeeError_NoMemory();
   return;
  }
 }
 // v "_DeeTraceback_New" doesn't try to raise an exception if it fails
 entry->re_traceback = (DeeTracebackObject *)_DeeTraceback_New(thread_self);
 if DEE_UNLIKELY(!entry->re_traceback) Dee_INCREF(
  entry->re_traceback = (DeeTracebackObject *)Dee_EmptyTraceback);
 Dee_INCREF(entry->re_error = error);
 entry->re_prev = thread_self->t_exception;
 DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
 thread_self->t_exception = entry;
 DeeAtomicMutex_Release(&thread_self->t_exception_lock);
}

DEE_A_SUCCESS(return == 0) int DeeError_HandledOne(void) {
 int result;
 struct DeeRaisedException *entry;
 DeeThreadObject *thread_self;
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL) return -1;
 entry = thread_self->t_exception;
 if DEE_UNLIKELY(!entry) return -1;
 DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
 thread_self->t_exception = entry->re_prev;
 DeeAtomicMutex_Release(&thread_self->t_exception_lock);
 result = entry->re_prev ? 1 : 0;
 DEE_LVERBOSE1("Handling generic error: %k\n%k",
               entry->re_error,entry->re_traceback);
 _DeeRaisedException_Quit(entry);
 _DeeRaisedException_Free(entry);
 return result;
}

DEE_A_SUCCESS(return != 0) int DeeError_Handled(void) {
 struct DeeRaisedException *entry,*next;
 DeeThreadObject *thread_self;
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL) return 0;
 entry = thread_self->t_exception;
 if DEE_UNLIKELY(!entry) return 0;
 DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
 thread_self->t_exception = NULL;
 DeeAtomicMutex_Release(&thread_self->t_exception_lock);
 while (1) {
  next = entry->re_prev;
  DEE_LVERBOSE1("Handling generic error: %k\n%k",
                entry->re_error,entry->re_traceback);
  _DeeRaisedException_Quit(entry);
  _DeeRaisedException_Free(entry);
  if (!next) break;
  entry = next;
 }
 return 1;
}


DEE_A_SUCCESS(return == 0) int DeeError_HandleOne(
 DEE_A_IN DeeTypeObject const *tp) {
 struct DeeRaisedException *entry,*next;
 DeeThreadObject *thread_self;
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL) return -1;
 entry = thread_self->t_exception;
 if DEE_UNLIKELY(!entry) return -1;
 if DEE_LIKELY(DeeObject_InstanceOf(entry->re_error,tp)) {
  next = entry->re_prev;
  DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
  thread_self->t_exception = next;
  DeeAtomicMutex_Release(&thread_self->t_exception_lock);
  _DeeRaisedException_Quit(entry);
  _DeeRaisedException_Free(entry);
  return DEE_UNLIKELY(next) ? 1 : 0;
 }
 return -1;
}
DEE_A_SUCCESS(return != 0) int DeeError_Handle(
 DEE_A_IN DeeTypeObject const *tp) {
 struct DeeRaisedException *entry,*next;
 DeeThreadObject *thread_self;
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL) return 0;
 entry = thread_self->t_exception;
 if DEE_UNLIKELY(!entry) return 0;
 if DEE_LIKELY(DeeObject_InstanceOf(entry->re_error,tp)) {
  // Discard all errors
  DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
  thread_self->t_exception = NULL;
  DeeAtomicMutex_Release(&thread_self->t_exception_lock);
  while (1) {
   next = entry->re_prev;
   _DeeRaisedException_Quit(entry);
   _DeeRaisedException_Free(entry);
   if DEE_LIKELY(!next) break;
   entry = next;
  }
  return 1;
 }
 return 0;
}
DEE_A_SUCCESS(return != 0) int DeeError_CanHandle(
 DEE_A_IN DeeTypeObject const *tp) {
 struct DeeRaisedException *entry;
 DeeThreadObject *thread_self;
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL) return 0;
 entry = thread_self->t_exception;
 return entry && DeeObject_InstanceOf(entry->re_error,tp);
}

DEE_A_SUCCESS(return >= 0) int DeeError_PopOccurredOne(
 DEE_A_REF DEE_A_OUT_OPT DeeObject **exc,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **tb) {
 struct DeeRaisedException *entry;
 DeeThreadObject *thread_self; int result;
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL) return -1;
 entry = thread_self->t_exception;
 if DEE_UNLIKELY(!entry) return -1;
 DEE_ASSERT(DeeObject_Check(entry->re_error));
 DEE_ASSERT(DeeObject_Check(entry->re_traceback));
 if (exc) Dee_INHERIT_REF(*exc,entry->re_error);
 else Dee_DECREF(entry->re_error);
 if (tb) Dee_INHERIT_REF(*tb,*(DeeObject **)&entry->re_traceback);
 else Dee_DECREF(entry->re_traceback);
 result = entry->re_prev ? 1 : 0;
 DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
 thread_self->t_exception = entry->re_prev;
 DeeAtomicMutex_Release(&thread_self->t_exception_lock);
 _DeeRaisedException_Free(entry);
 return result;
}
DEE_A_SUCCESS(return != 0) int DeeError_PopOccurred(
 DEE_A_REF DEE_A_OUT_OPT DeeObject **exc,
 DEE_A_REF DEE_A_OUT_OPT DeeObject **tb) {
 struct DeeRaisedException *entry,*next;
 DeeThreadObject *thread_self;
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL) return 0;
 entry = thread_self->t_exception;
 if DEE_UNLIKELY(!entry) return 0;
 DEE_ASSERT(DeeObject_Check(entry->re_error));
 DEE_ASSERT(DeeObject_Check(entry->re_traceback));
 if (exc) Dee_INHERIT_REF(*exc,entry->re_error);
 else Dee_DECREF(entry->re_error);
 if (tb) Dee_INHERIT_REF(*tb,*(DeeObject **)&entry->re_traceback);
 else Dee_DECREF(entry->re_traceback);
 DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
 thread_self->t_exception = NULL;
 DeeAtomicMutex_Release(&thread_self->t_exception_lock);
 while (1) {
  next = entry->re_prev;
  _DeeRaisedException_Free(entry);
  if (!next) break;
  entry = next;
  _DeeRaisedException_Quit(entry);
 }
 return 1;
}
DEE_A_SUCCESS(return != 0) int DeeError_GetOccurred(
 DEE_A_OUT_OPT DeeObject **exc, DEE_A_OUT_OPT DeeObject **tb) {
 struct DeeRaisedException *entry;
 DeeThreadObject *thread_self;
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL) return 0;
 entry = thread_self->t_exception;
 if DEE_UNLIKELY(!entry) return 0;
 DEE_ASSERT(DeeObject_Check(entry->re_error));
 DEE_ASSERT(DeeObject_Check(entry->re_traceback));
 if (exc) *exc = entry->re_error;
 if (tb) *tb = (DeeObject *)entry->re_traceback;
 return 1;
}
DEE_A_SUCCESS(return != 0) int DeeError_GetHandling(
 DEE_A_OUT_OPT DeeObject **exc, DEE_A_OUT_OPT DeeObject **tb) {
 struct DeeRaisedException *entry;
 DeeThreadObject *thread_self;
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL) return 0;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 entry = thread_self->t_handling_exception;
#else
 entry = thread_self->t_exception;
#endif
 if DEE_UNLIKELY(!entry) return 0;
 DEE_ASSERT(DeeObject_Check(entry->re_error));
 DEE_ASSERT(DeeObject_Check(entry->re_traceback));
 if (exc) *exc = entry->re_error;
 if (tb) *tb = (DeeObject *)entry->re_traceback;
 return 1;
}
DEE_A_REF DEE_A_RET_NEVER_NULL DeeTypeObject *
_DeeError_HandleAndReturnType(void) {
 DeeTypeObject *result;
 struct DeeRaisedException *entry;
 DeeThreadObject *thread_self;
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL ||
                 (entry = thread_self->t_exception) == NULL) {
  Dee_INCREF(&DeeNone_Type);
  return (DeeTypeObject *)&DeeNone_Type;
 }
 DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
 thread_self->t_exception = entry->re_prev;
 DeeAtomicMutex_Release(&thread_self->t_exception_lock);
 Dee_INCREF(result = Dee_TYPE(entry->re_error));
 _DeeRaisedException_Quit(entry);
 _DeeRaisedException_Free(entry);
 return result;
}


DEE_A_SUCCESS(return == 0) int DeeError_Print(
 DEE_A_IN_Z_OPT char const *reason, DEE_A_IN int handle_error) {
 struct DeeRaisedException *entry;
 DeeThreadObject *thread_self;
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL)
  return 1; // this should be an error, but we couldn't raise it in this state
 if DEE_UNLIKELY((entry = thread_self->t_exception) == NULL) return 1; // no error here...
 DeeError_Display(reason,entry->re_error,(DeeObject *)entry->re_traceback);
 if (handle_error) {
  DeeAtomicMutex_Acquire(&thread_self->t_exception_lock);
  thread_self->t_exception = entry->re_prev;
  DeeAtomicMutex_Release(&thread_self->t_exception_lock);
  _DeeRaisedException_Quit(entry);
  _DeeRaisedException_Free(entry);
 }
 return 0;
}
void DeeError_Display(DEE_A_IN_Z_OPT char const *reason, DEE_A_INOUT DeeObject *exc,
                      DEE_A_IN_OBJECT(DeeTracebackObject) *tb) {
 DeeStringObject *error_str;
 DEE_ASSERT(DeeObject_Check(exc));
 DEE_ASSERT(DeeObject_Check(tb) && DeeTraceback_Check(tb));
 if DEE_UNLIKELY((error_str = (DeeStringObject *)DeeObject_Str(exc)) == NULL) {
handle_0: DeeError_Handled(); return;
 }
 if (!reason) reason = "Unhandled exception\n";
 fwrite(reason,sizeof(char),strlen(reason),stderr);
 fprintf(stderr,">> %s: %s\n",DeeType_NAME(Dee_TYPE(exc)),DeeString_STR(error_str));
#if defined(DEE_DEBUG) && defined(DEE_PLATFORM_WINDOWS)
 // Output the error text into the debugger as well
 OutputDebugStringA(reason);
 OutputDebugStringA(">> ");
 OutputDebugStringA(DeeType_NAME(Dee_TYPE(exc)));
 OutputDebugStringA(": ");
 OutputDebugStringA(DeeString_STR(error_str));
 OutputDebugStringA("\n");
#endif /* DEE_DEBUG && DEE_PLATFORM_WINDOWS */
 Dee_DECREF(error_str);
 if DEE_UNLIKELY((error_str = (DeeStringObject *)DeeObject_Repr(tb)) == NULL) goto handle_0;
 fprintf(stderr,"%s\n",DeeString_STR(error_str));
#if defined(DEE_DEBUG) && defined(DEE_PLATFORM_WINDOWS)
 OutputDebugStringA(DeeString_STR(error_str));
 OutputDebugStringA("\n");
#endif /* DEE_DEBUG && DEE_PLATFORM_WINDOWS */
 Dee_DECREF(error_str);
}



#undef DeeError_Occurred
DEE_A_RET_MAYBE_NULL DEE_A_RET_WUNUSED
DeeObject *DeeError_Occurred(void) {
#ifndef DEE_WITHOUT_THREADS
 DeeThreadObject *thread_self;
 struct DeeRaisedException *entry;
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL) return NULL;
 if DEE_UNLIKELY((entry = thread_self->t_exception) == NULL) return NULL;
 return entry->re_error;
#else /* !DEE_WITHOUT_THREADS */
 return _DeeThread_Self.t_exception
  ? _DeeThread_Self.t_exception->re_error
  : NULL;
#endif /* DEE_WITHOUT_THREADS */
}

DEE_A_RET_WUNUSED int _DeeError_Occurred(void) {
#ifndef DEE_WITHOUT_THREADS
 DeeThreadObject *thread_self;
 return DEE_UNLIKELY((thread_self = DeeThread_SELF()) != NULL) &&
                     (thread_self->t_exception != NULL);
#else /* !DEE_WITHOUT_THREADS */
 return _DeeThread_Self.t_exception != NULL;
#endif /* DEE_WITHOUT_THREADS */
}


void DeeError_PushState(DEE_A_OUT struct DeeErrorStateData *state) {
 if DEE_LIKELY((state->esd_threadself = DeeThread_SELF()) != NULL) {
  if ((state->esd_stored_exceptions = state->esd_threadself->t_exception) != NULL) {
   DeeAtomicMutex_AcquireRelaxed(&state->esd_threadself->t_exception_lock);
   state->esd_threadself->t_exception = NULL;
   DeeAtomicMutex_Release(&state->esd_threadself->t_exception_lock);
  }
 } else {
  state->esd_stored_exceptions = NULL;
 }
}
void DeeError_PopState(DEE_A_IN struct DeeErrorStateData const *state) {
#ifndef __INTELLISENSE__
#define _temp_state (*state)
 DeeError_BREAK_STATE();
#undef _temp_state
#endif
}





void DeeError_SetNone(DEE_A_INOUT DeeTypeObject *type_) {
 DeeObject *exc;
 if DEE_LIKELY((exc = DeeType_NewInstanceDefault(type_)) != NULL) {
  DeeError_Throw(exc);
  Dee_DECREF(exc);
 }
}

void DeeError_SetString(DEE_A_INOUT DeeTypeObject *type_, DEE_A_IN_Z char const *message) {
 DeeObject *error,*ctor_args,*msg_ob;
 if DEE_UNLIKELY((msg_ob = DeeString_New(message)) == NULL) return; // Out of memory
 if DEE_UNLIKELY((ctor_args = _DeeTuple_NewUnsafe(1)) == NULL) { Dee_DECREF(msg_ob); return; }
 Dee_INHERIT_REF(DeeTuple_ELEM(ctor_args)[0],msg_ob);
 if DEE_UNLIKELY((error = DeeType_NewInstance(type_,ctor_args)) == NULL) goto end1;
 DeeError_Throw(error);
 Dee_DECREF(error);
end1: Dee_DECREF(ctor_args);
}

void DeeError_VSetStringf(
 DEE_A_INOUT DeeTypeObject *type_,
 DEE_A_IN_PRINTF char const *fmt,
 DEE_A_INOUT va_list args) {
 DeeObject *error,*ctor_args,*msg_ob;
 if DEE_UNLIKELY((msg_ob = DeeString_VNewf(fmt,args)) == NULL) return; // Out of memory
 if DEE_UNLIKELY((ctor_args = _DeeTuple_NewUnsafe(1)) == NULL) { Dee_DECREF(msg_ob); return; }
 Dee_INHERIT_REF(DeeTuple_ELEM(ctor_args)[0],msg_ob);
 if DEE_UNLIKELY((error = DeeType_NewInstance(type_,ctor_args)) == NULL) goto end1;
 DeeError_Throw(error);
 Dee_DECREF(error);
end1:
 Dee_DECREF(ctor_args);
}

void DeeError_SetStringf(
 DEE_A_INOUT DeeTypeObject *type_,
 DEE_A_IN_PRINTF char const *fmt, ...) {
 va_list args;
 DEE_VA_START(args,fmt);
 DeeError_VSetStringf(type_,fmt,args);
 DEE_VA_END(args);
}

#if DEE_DEVEL_BUILD
void _DeeError_TODONotImplemented_impl(
 DEE_A_IN_Z char const *file_, DEE_A_IN int line_,
 DEE_A_IN_Z char const *func_) {
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     //               v Prevent this todo from showing up in searches
                     "%s(%d) : %s : TO" "DO : Griefer@Work hasn't gotten around to implementing this...",
                     file_,func_,line_);
}
#endif


DeeError_NEW_EXTERN(_DeeError_RuntimeError_NoActiveException,
                    &DeeErrorType_RuntimeError,"No active exception");
DEE_A_EXEC void DeeError_TypeError_UnexpectedTypeT(
 DEE_A_IN DeeTypeObject const *expected_type,
 DEE_A_IN DeeTypeObject const *found_type) {
 DeeError_SetStringf(&DeeErrorType_TypeError,"Unexpected type (expected: %s; got: %s)",
                     DeeType_NAME(expected_type),DeeType_NAME(found_type));
}
void DeeError_TypeError_UnexpectedType(
 DEE_A_INOUT DeeObject *ob, DEE_A_IN DeeTypeObject const *expected_type) {
 DeeError_TypeError_UnexpectedTypeT(expected_type,Dee_TYPE(ob));
}
#ifndef DEE_WITHOUT_THREADS
void DeeError_ThreadCrash(DEE_A_IN_OBJECT(DeeThreadObject) *thread) {
 DeeObject *error,*err,*tb; int temp;
 if DEE_UNLIKELY((temp = DeeThread_GetError(thread,&err,&tb)) < 0) return;
 if DEE_UNLIKELY(!temp) { err = DeeNone_New(); Dee_INCREF(tb = Dee_EmptyTraceback); }
 error = DeeType_NewInstancef(&DeeErrorType_ThreadCrash,"ooo",thread,err,tb);
 Dee_DECREF(err);
 Dee_DECREF(tb);
 if DEE_LIKELY(error != NULL) {
  DeeError_Throw(error);
  Dee_DECREF(error);
 }
}
#endif
void DeeError_AttributeError_NoDel(DEE_A_IN DeeTypeObject const *tp, DEE_A_IN_Z char const *attr) {
 DeeError_SetStringf(&DeeErrorType_AttributeError,
                     "Can't delete attribute \"%#q.%#q\"",
                     DeeType_NAME(tp),attr);
}
void DeeError_AttributeError_NoRead(DEE_A_IN DeeTypeObject const *tp, DEE_A_IN_Z char const *attr) {
 DeeError_SetStringf(&DeeErrorType_AttributeError,
                     "Can't read attribute \"%#q.%#q\"",
                     DeeType_NAME(tp),attr);
}
void DeeError_AttributeError_NoWrite(DEE_A_IN DeeTypeObject const *tp, DEE_A_IN_Z char const *attr) {
 DeeError_SetStringf(&DeeErrorType_AttributeError,
                     "Can't write attribute \"%#q.%#q\"",
                     DeeType_NAME(tp),attr);
}
void DeeError_AttributeError_NotFound(DEE_A_IN DeeTypeObject const *tp, DEE_A_IN_Z char const *attr) {
 DeeError_SetStringf(&DeeErrorType_AttributeError,
                     "Didn't find attribute \"%#q.%#q\"",
                     DeeType_NAME(tp),attr);
}
void DeeError_KeyError_NotFound(DEE_A_INOUT DeeObject *map, DEE_A_INOUT DeeObject *key) {
 DeeError_SetStringf(&DeeErrorType_KeyError,
                     "Key %s:%r not found in %s:%k",
                     DeeType_NAME(Dee_TYPE(key)),key,
                     DeeType_NAME(Dee_TYPE(map)),map);
}
void DeeError_KeyError_NotFound_str(DEE_A_INOUT DeeObject *map, DEE_A_IN_Z char const *key) {
 DeeError_SetStringf(&DeeErrorType_KeyError,
                     "Key %q not found in %s:%k",
                     key,DeeType_NAME(Dee_TYPE(map)),map);
}
void DeeError_KeyError_NotFound_T(DEE_A_IN DeeTypeObject const *map, DEE_A_INOUT DeeObject *key) {
 DeeError_SetStringf(&DeeErrorType_KeyError,
                     "Key %s:%r not found in %s",
                     DeeType_NAME(Dee_TYPE(key)),key,DeeType_NAME(map));
}
void DeeError_ValueError_NotFound(DEE_A_INOUT DeeObject *list, DEE_A_INOUT DeeObject *elem) {
 DeeError_SetStringf(&DeeErrorType_ValueError,
                     "Element %s:%k not found in %s:%k",
                     DeeType_NAME(Dee_TYPE(elem)),elem,
                     DeeType_NAME(Dee_TYPE(list)),list);
}
void DeeError_ValueError_NotFound_T(DEE_A_IN DeeTypeObject const *list, DEE_A_INOUT DeeObject *elem) {
 DeeError_SetStringf(&DeeErrorType_ValueError,
                     "Element %s:%k not found in %s",
                     DeeType_NAME(Dee_TYPE(elem)),elem,DeeType_NAME(list));
}
void DeeError_ValueError_NotFound_str_T(DEE_A_IN DeeTypeObject const *list, DEE_A_IN_Z char const *elem) {
 DeeError_SetStringf(&DeeErrorType_ValueError,
                     "Element %s:%s not found in %s",
                     DeeType_NAME(Dee_TYPE(elem)),elem,DeeType_NAME(list));
}
void DeeError_ValueError_NotFound_Pred(DEE_A_INOUT DeeObject *list, DEE_A_INOUT DeeObject *pred) {
 DeeError_SetStringf(&DeeErrorType_ValueError,
                     "No element conforming to %s:%k found in %s:%k",
                     DeeType_NAME(Dee_TYPE(pred)),pred,
                     DeeType_NAME(Dee_TYPE(list)),list);
}
void DeeError_ValueError_NotFound_Pred_T(DEE_A_IN DeeTypeObject const *list_type, DEE_A_INOUT DeeObject *pred) {
 DeeError_SetStringf(&DeeErrorType_ValueError,
                     "No element conforming to %s:%k found in %s",
                     DeeType_NAME(Dee_TYPE(pred)),pred,DeeType_NAME(list_type));
}
void DeeError_ValueError_EmptyList(DEE_A_IN DeeTypeObject const *list_type) {
 DeeError_SetStringf(&DeeErrorType_ValueError,
                     "container %q is empty",
                     DeeType_NAME(list_type));
}
void DeeError_IndexError(DEE_A_INOUT DeeObject *list, DEE_A_IN Dee_size_t index) {
 DeeError_SetStringf(&DeeErrorType_IndexError,
                     "Index %Iu is greater than or equal to %K",
                     index,DeeObject_SizeObject(list));
}
void DeeError_IndexError_T(DEE_A_IN DeeTypeObject const *tp, DEE_A_IN Dee_size_t index) {
 DeeError_SetStringf(&DeeErrorType_IndexError,
                     "Index %Iu is greater than the size of %s",
                     index,DeeType_NAME(tp));
}
void DeeError_TypeError_TupleSizeExpected(DEE_A_IN Dee_size_t expected, DEE_A_IN Dee_size_t given) {
 DeeError_SetStringf(&DeeErrorType_TypeError,
                     "Expected a %Iu elemental tuple (%Iu elements found)",
                     expected,given);
}
void DeeError_TypeError_ArgCountExpected(DEE_A_IN Dee_size_t expected, DEE_A_IN Dee_size_t given) {
 DeeError_SetStringf(&DeeErrorType_TypeError,
                     "This function requires %Iu argument(s) (%Iu given)",
                     expected,given);
}
void DeeError_TypeError_ArgCountExpectedNamed(
 DEE_A_IN_Z char const *fun_name, DEE_A_IN Dee_size_t expected, DEE_A_IN Dee_size_t given) {
 DeeError_SetStringf(&DeeErrorType_TypeError,
                     "%q requires %Iu argument(s) (%Iu given)",
                     fun_name,expected,given);
}
void DeeError_TypeError_ArgCountExpectedNamedMinMax(
 DEE_A_IN_Z char const *fun_name, DEE_A_IN Dee_size_t min_expected,
 DEE_A_IN Dee_size_t max_expected, DEE_A_IN Dee_size_t given) {
 if (min_expected == max_expected) {
  DeeError_TypeError_ArgCountExpectedNamed(fun_name,min_expected,given);
 } else {
  DeeError_SetStringf(&DeeErrorType_TypeError,
                      "%q requires %Iu ... %Iu argument(s) (%Iu given)",
                      fun_name,min_expected,max_expected,given);
 }
}
void DeeError_TypeError_ArgCountExpectedNamedMin(
 DEE_A_IN_Z char const *fun_name, DEE_A_IN Dee_size_t min_expected, DEE_A_IN Dee_size_t given) {
 DeeError_SetStringf(&DeeErrorType_TypeError,
                     "%q requires at least %Iu argument(s) (%Iu given)",
                     fun_name,min_expected,given);
}
void DeeError_TypeError_ArgCountExpectedNamedMax(
 DEE_A_IN_Z char const *fun_name, DEE_A_IN Dee_size_t max_expected, DEE_A_IN Dee_size_t given) {
 DeeError_SetStringf(&DeeErrorType_TypeError,
                     "%q requires at most %Iu argument(s) (%Iu given)",
                     fun_name,max_expected,given);
}
void DeeError_TypeError_ArgCountExpectedMin(
 DEE_A_IN Dee_size_t min_expected, DEE_A_IN Dee_size_t given) {
 DeeError_SetStringf(&DeeErrorType_TypeError,
                     "This function requires at least %Iu argument(s) (%Iu given)",
                     min_expected,given);
}
void DeeError_TypeError_ArgCountExpectedMax(
 DEE_A_IN Dee_size_t max_expected, DEE_A_IN Dee_size_t given) {
 DeeError_SetStringf(&DeeErrorType_TypeError,
                     "This function requires max %Iu argument(s) (%Iu given)",
                     max_expected,given);
}
DEE_A_EXEC void DeeError_TypeError_ArgCountExpectedMinMax(
 DEE_A_IN Dee_size_t min_expected, DEE_A_IN Dee_size_t max_expected, DEE_A_IN Dee_size_t given) {
 if (min_expected == max_expected) {
  DeeError_TypeError_ArgCountExpected(min_expected,given);
 } else {
  DeeError_SetStringf(&DeeErrorType_TypeError,
                      "This function requires %Iu ... %Iu argument(s) (%Iu given)",
                      min_expected,max_expected,given);
 }
}



DEE_STATIC_INLINE(int) _deewarn_suppressed(DeeLexerObject *lexer, int code) {
 if (lexer->l_suppress_warnings != 0) return 1;
 if (lexer->l_suppress_Wcop != 0) {
  switch (code) {
#define WARNING(x,default) /* nothing */
#define W_NOCOP(x,default) case x:
#include <deemon/compiler/__warnings.inl>
#undef W_NOCOP
#undef WARNING
    return 1;
   default: break;
  }
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeError_TPPCompilerError(
 DEE_A_IN int code, DEE_A_IN struct TPPLexerObject *lexer,
 DEE_A_IN struct TPPTokenObject *token, DEE_A_IN_Z char const *message) {
 if (TPPLexer_Warn(lexer,code,token,message) == -1) {
#if 0
  DeeError_SetStringf(
   &DeeErrorType_CompilerError,"%s(%d) : %c%.4d : %K : %s",
   TPPToken_File(token),TPPToken_Line(token)+1,
   code >= TPPWarnings_COUNT_WITH_RESERVED ? 'C' : 'P',code,
   DeeToken_StrTPPToken(token),message);
#endif
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeError_TPPCompilerErrorf(
 DEE_A_IN int code, DEE_A_IN struct TPPLexerObject *lexer,
 DEE_A_IN struct TPPTokenObject *token, DEE_A_IN_PRINTF char const *fmt, ...) {
 va_list args; int result;
 DEE_VA_START(args,fmt);
 result = DeeError_VTPPCompilerErrorf(code,lexer,token,fmt,args);
 DEE_VA_END(args);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeError_VTPPCompilerErrorf(
 DEE_A_IN int code, DEE_A_IN struct TPPLexerObject *lexer,
 DEE_A_IN struct TPPTokenObject *token,
 DEE_A_IN_PRINTF char const *fmt, DEE_A_INOUT va_list args) {
 DeeObject *message; int result;
 DEE_ASSERT(lexer && token);
 if ((message = DeeString_VNewf(fmt,args)) == NULL) return -1;
 result = DeeError_TPPCompilerError(code,lexer,token,DeeString_STR(message));
 Dee_DECREF(message);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeError_CompilerError(
 DEE_A_IN int code,
 DEE_A_IN_OBJECT(DeeLexerObject) *lexer,
 DEE_A_IN_OBJECT(DeeTokenObject) *token,
 DEE_A_IN_Z char const *message) {
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(token) && DeeToken_Check(token));
 if (_deewarn_suppressed((DeeLexerObject *)lexer,code)) return 0;
 return DeeError_TPPCompilerError(
  code,DeeLexer_LEXER(lexer),DeeToken_TOKEN(token),message);
}
DEE_A_RET_EXCEPT(-1) int DeeError_CompilerErrorf(
 DEE_A_IN int code,
 DEE_A_IN_OBJECT(DeeLexerObject) *lexer,
 DEE_A_IN_OBJECT(DeeTokenObject) *token,
 DEE_A_IN_PRINTF char const *fmt, ...) {
 va_list args; int result;
 DEE_VA_START(args,fmt);
 result = DeeError_VCompilerErrorf(code,lexer,token,fmt,args);
 DEE_VA_END(args);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeError_VCompilerErrorf(
 DEE_A_IN int code,
 DEE_A_IN_OBJECT(DeeLexerObject) *lexer,
 DEE_A_IN_OBJECT(DeeTokenObject) *token,
 DEE_A_IN_PRINTF char const *fmt,
 DEE_A_INOUT va_list args) {
 DeeObject *message; int result;
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(token) && DeeToken_Check(token));
 if ((message = DeeString_VNewf(fmt,args)) == NULL) return -1;
 result = _deewarn_suppressed((DeeLexerObject *)lexer,code)?0:
  DeeError_TPPCompilerError(code,DeeLexer_LEXER(lexer),DeeToken_TOKEN(token),DeeString_STR(message));
 Dee_DECREF(message);
 return result;
}




















DeeSignalObject _DeeErrorInstance_IterDone = {
 DEE_OBJECT_HEAD_INIT(&DeeSignalType_StopIteration)};
DeeString_NEW_STATIC(_outofmemory_message,"Out of memory");
DeeErrorObject _DeeErrorInstance_OutOfMemory = {
 DEE_OBJECT_HEAD_INIT(&DeeErrorType_NoMemory),
 (DeeStringObject *)&_outofmemory_message};

DeeString_NEW_STATIC(_notimplemented_message,"Not implemented");
DeeErrorObject _DeeErrorInstance_NotImplemented = {
 DEE_OBJECT_HEAD_INIT(&DeeErrorType_NotImplemented),
 (DeeStringObject *)&_notimplemented_message};

DeeString_NEW_STATIC(_divbyzero_message,"Divide by zero");
DeeErrorObject _DeeErrorInstance_DivByZero = {
 DEE_OBJECT_HEAD_INIT(&DeeErrorType_DivideByZero),
 (DeeStringObject *)&_divbyzero_message};
DeeString_NEW_STATIC(_stackoverflow_message,"Stack overflow");
DeeErrorObject _DeeErrorInstance_StackOverflow = {
 DEE_OBJECT_HEAD_INIT(&DeeErrorType_StackOverflow),
 (DeeStringObject *)&_stackoverflow_message};



static DeeObject *_deesignal_tp_str(DeeObject *self) { return DeeString_New(DeeType_NAME(Dee_TYPE(self))); }
static int _deesignal_tp_ctor(DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *DEE_UNUSED(self)) { return 0; }
static int _deesignal_tp_copy_ctor(DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *DEE_UNUSED(self), DeeObject *DEE_UNUSED(right)) { return 0; }
#ifndef DEE_WITHOUT_THREADS
extern /*atomic*/Dee_uint16_t _deethread_itrpcounter;
static void _deeiterruptsignal_tp_dtor(DeeSignalObject *self) {
 DeeThreadObject *thread_self; Dee_uint8_t state;
 if ((thread_self = DeeThread_SELF()) == NULL) return;
 if (self != thread_self->t_sys_interrupt) return; // not the internal interrupt signal
 thread_self->t_sys_interrupt = NULL;
 do { // Must restore the interrupt flag in the current thread, so it get thrown again
  state = (Dee_uint8_t)DeeAtomic8_Load(thread_self->t_state,memory_order_seq_cst);
  if ((state&DeeThreadState_TERMINATED) != 0) return; // Thread has already terminated (don't restore interrupt!)
 } while (!DeeAtomic8_CompareExchangeWeak(
  thread_self->t_state,state,state&~DeeThreadState_INTERRUPT_HANDLED,
  memory_order_seq_cst,memory_order_seq_cst));
 DEE_THREADITRP_COUNTER_INC(); // +1 threads with unhandled interrupt signals
 DEE_LVERBOSE2("Restored interrupt signal in thread: %k\n",thread_self);
}

#else
#define _deeiterruptsignal_tp_dtor  DeeType_DEFAULT_SLOT(tp_dtor)
#endif

static int _deeerror_tp_ctor(DeeTypeObject *DEE_UNUSED(tp_self), DeeErrorObject *self) {
 Dee_INCREF(self->ob_message = (DeeStringObject *)Dee_EmptyString);
 return 0;
}
static int _deeerror_tp_any_ctor(DeeTypeObject *DEE_UNUSED(tp_self), DeeErrorObject *self, DeeObject *args) {
 DeeObject *arg;
 if (DeeTuple_Unpack(args,"o:Error",&arg) != 0) return -1;
 if ((self->ob_message = (DeeStringObject *)DeeObject_Str(arg)) == NULL) return -1;
 return 0;
}
static void _deeerror_tp_dtor(DeeErrorObject *self) {
 Dee_DECREF(self->ob_message);
}
static DeeObject *_deeerror_tp_str(DeeErrorObject *self) {
 DeeReturn_NEWREF(self->ob_message);
}
static DeeObject *_deeerror_tp_repr(DeeErrorObject *self) {
 return DeeString_Newf("%s: %k",DeeType_NAME(Dee_TYPE(self)),self->ob_message);
}

static struct DeeMemberDef const _deesignal_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("Interrupt",object,&DeeSignalType_Interrupt),
 DEE_MEMBERDEF_CONST_v100("StopIteration",object,&DeeSignalType_StopIteration),
 DEE_MEMBERDEF_END_v100
};

static struct DeeMemberDef const _deeerror_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("AttributeError",object,&DeeErrorType_AttributeError),
 DEE_MEMBERDEF_CONST_v100("CompilerError",object,&DeeErrorType_CompilerError),
 DEE_MEMBERDEF_CONST_v100("ThreadCrash",object,&DeeErrorType_ThreadCrash),
 DEE_MEMBERDEF_CONST_v100("NoMemory",object,&DeeErrorType_NoMemory),
 DEE_MEMBERDEF_CONST_v100("RuntimeError",object,&DeeErrorType_RuntimeError),
 DEE_MEMBERDEF_CONST_v100("TypeError",object,&DeeErrorType_TypeError),
 DEE_MEMBERDEF_CONST_v100("ValueError",object,&DeeErrorType_ValueError),
 DEE_MEMBERDEF_CONST_v100("NetError",object,&DeeErrorType_NetError),
 DEE_MEMBERDEF_CONST_v100("SystemError",object,&DeeErrorType_SystemError),
 DEE_MEMBERDEF_CONST_v100("IOError",object,&DeeErrorType_IOError),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const _deeruntimeerror_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("NotImplemented",object,&DeeErrorType_NotImplemented),
 DEE_MEMBERDEF_CONST_v100("UnboundLocal",object,&DeeErrorType_UnboundLocal),
 DEE_MEMBERDEF_CONST_v100("AssertionError",object,&DeeErrorType_AssertionError),
 DEE_MEMBERDEF_CONST_v100("StackOverflow",object,&DeeErrorType_StackOverflow),
 DEE_MEMBERDEF_CONST_v100("SegFault",object,&DeeErrorType_SegFault),
 DEE_MEMBERDEF_CONST_v100("IllegalInstruction",object,&DeeErrorType_IllegalInstruction),
 DEE_MEMBERDEF_CONST_v100("DexError",object,&DeeErrorType_DexError),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const _deevalueerror_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("DivideByZero",object,&DeeErrorType_DivideByZero),
 DEE_MEMBERDEF_CONST_v100("KeyError",object,&DeeErrorType_KeyError),
 DEE_MEMBERDEF_CONST_v100("IndexError",object,&DeeErrorType_IndexError),
 DEE_MEMBERDEF_CONST_v100("UnicodeError",object,&DeeErrorType_UnicodeError),
 DEE_MEMBERDEF_CONST_v100("ReferenceError",object,&DeeErrorType_ReferenceError),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const _deeioerror_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("FileNotFound",object,&DeeErrorType_FileNotFound),
 DEE_MEMBERDEF_END_v100
};


DeeTypeObject DeeSignalType_Signal = {
 DEE_TYPE_OBJECT_HEAD_v100(member("Signal"),null,null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeSignalObject),null,
  member(&_deesignal_tp_ctor),
  member(&_deesignal_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deesignal_tp_str),
  member(&_deesignal_tp_str),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,
 member(_deesignal_tp_class_members),null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};
DeeTypeObject DeeSignalType_Interrupt = {
 DEE_TYPE_OBJECT_HEAD_v100(
  member("Signal.Interrupt"),null,null,
  member(&DeeSignalType_Signal)),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeSignalObject),
  null,member(&_deesignal_tp_ctor),
  member(&_deesignal_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deeiterruptsignal_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deesignal_tp_str),
  member(&_deesignal_tp_str),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,
 member(_deesignal_tp_class_members),null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};
#define DEE_SIGNAL_TYPE_INIT(T,base,name,dtor,members)\
{\
 DEE_TYPE_OBJECT_HEAD_v100(name,null,null,base),\
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(T),null,\
  member(&_deesignal_tp_ctor),member(&_deesignal_tp_copy_ctor),null,null),\
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,dtor),\
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),\
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),\
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),\
 DEE_TYPE_OBJECT_MATH_v101(\
  null,null,null,null,null,null,null,null,null,null,null,\
  null,null,null,null,null,null,null,null,null,null,null,\
  null,null,null,null,null,null,null,null,null,null),\
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),\
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),\
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,members,null,null),\
 DEE_TYPE_OBJECT_FOOTER_v100\
}
DeeTypeObject DeeSignalType_StopIteration =
 DEE_SIGNAL_TYPE_INIT(DeeSignalObject,
  member(&DeeSignalType_Signal),
  member("Signal.StopIteration"),null,null);
#undef DEE_SIGNAL_TYPE_INIT


static DeeObject *_deeerror_inner(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if (DeeTuple_Unpack(args,":inner") != 0) return NULL;
 if ((result = DeeError_InnerImpl(self)) == NULL)
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "%s doesn't have an inner error",
                      DeeType_NAME(Dee_TYPE(self)));
 return result;
}

static struct DeeMethodDef const _deeerror_tp_methods[] = {
 DEE_METHODDEF_v100("inner",member(&_deeerror_inner),"() -> object\n"
  "@return: The inner error object"),
 DEE_METHODDEF_END_v100
};

DeeTypeObject DeeErrorType_Error = {
 DEE_TYPE_OBJECT_HEAD_v100(member("Error"),null,null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeErrorObject),null,
  member(&_deeerror_tp_ctor),null,null,
  member(&_deeerror_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deeerror_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deeerror_tp_str),
  member(&_deeerror_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,null,member(_deeerror_tp_methods),
  member(_deeerror_tp_class_members),null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};

#define DEE_EXCEPTION_TYPE_INIT(T,base,name,dtor,members)\
{\
 DEE_TYPE_OBJECT_HEAD_v100(name,null,null,base),\
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(T),null,\
  member(&_deeerror_tp_ctor),null,null,\
  member(&_deeerror_tp_any_ctor)),\
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,dtor),\
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),\
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),\
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),\
 DEE_TYPE_OBJECT_MATH_v101(\
  null,null,null,null,null,null,null,null,null,null,null,\
  null,null,null,null,null,null,null,null,null,null,null,\
  null,null,null,null,null,null,null,null,null,null),\
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),\
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),\
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,members,null,null),\
 DEE_TYPE_OBJECT_FOOTER_v100\
}
DeeTypeObject DeeErrorType_AttributeError      = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_Error),       member("Error.AttributeError"),                 null,null);
DeeTypeObject DeeErrorType_CompilerError       = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_Error),       member("Error.CompilerError"),                  null,null);
DeeTypeObject DeeErrorType_NoMemory            = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_Error),       member("Error.NoMemory"),                       null,null);
DeeTypeObject DeeErrorType_RuntimeError        = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_Error),       member("Error.RuntimeError"),                   null,member(_deeruntimeerror_tp_class_members));
DeeTypeObject  DeeErrorType_NotImplemented     = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_RuntimeError),member("Error.RuntimeError.NotImplemented"),    null,null);
DeeTypeObject  DeeErrorType_AssertionError     = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_RuntimeError),member("Error.RuntimeError.AssertionError"),    null,null);
DeeTypeObject  DeeErrorType_UnboundLocal       = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_RuntimeError),member("Error.RuntimeError.UnboundLocal"),      null,null);
DeeTypeObject  DeeErrorType_StackOverflow      = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_RuntimeError),member("Error.RuntimeError.StackOverflow"),     null,null);
DeeTypeObject  DeeErrorType_SegFault           = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_RuntimeError),member("Error.RuntimeError.SegFault"),          null,null);
DeeTypeObject  DeeErrorType_IllegalInstruction = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_RuntimeError),member("Error.RuntimeError.IllegalInstruction"),null,null);
DeeTypeObject  DeeErrorType_DexError           = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_RuntimeError),member("Error.RuntimeError.DexError"),          null,null);
DeeTypeObject DeeErrorType_TypeError           = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_Error),       member("Error.TypeError"),                      null,null);
DeeTypeObject DeeErrorType_ValueError          = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_Error),       member("Error.ValueError"),                     null,member(_deevalueerror_tp_class_members));
DeeTypeObject  DeeErrorType_DivideByZero       = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_ValueError),  member("Error.ValueError.DivideByZero"),        null,null);
DeeTypeObject  DeeErrorType_KeyError           = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_ValueError),  member("Error.ValueError.KeyError"),            null,null);
DeeTypeObject  DeeErrorType_IndexError         = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_ValueError),  member("Error.ValueError.IndexError"),          null,null);
DeeTypeObject  DeeErrorType_UnicodeError       = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_ValueError),  member("Error.ValueError.UnicodeError"),        null,null);
DeeTypeObject  DeeErrorType_ReferenceError     = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_ValueError),  member("Error.ValueError.ReferenceError"),      null,null);
DeeTypeObject DeeErrorType_SystemError         = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_Error),       member("Error.SystemError"),                    null,null);
DeeTypeObject DeeErrorType_NetError            = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_Error),       member("Error.NetError"),                       null,null);
DeeTypeObject DeeErrorType_IOError             = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_Error),       member("Error.IOError"),                        null,member(_deeioerror_tp_class_members));
DeeTypeObject  DeeErrorType_FileNotFound       = DEE_EXCEPTION_TYPE_INIT(DeeErrorObject,member(&DeeErrorType_IOError),     member("Error.IOError.FileNotFound"),           null,null);
#undef DEE_EXCEPTION_TYPE_INIT




static int _deethreadcrash_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeThreadCrashErrorObject *self,
 DeeThreadCrashErrorObject *right) {
 Dee_INCREF(self->ob_message = right->ob_message);
 Dee_INCREF(self->ob_inner = right->ob_inner);
 Dee_INCREF(self->ob_inner_tb = right->ob_inner_tb);
 return 0;
}
static int _deethreadcrash_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeThreadCrashErrorObject *self, DeeObject *args) {
 DeeObject *thread = NULL,*inner = Dee_None,*inner_tb = Dee_EmptyTraceback;
 if (DeeTuple_Unpack(args,"|ooo:Error.ThreadCrash",&thread,&inner,&inner_tb) != 0) return -1;
 if (!thread) thread = DeeThread_Self();
 else if (DeeObject_InplaceGetInstance(&thread,&DeeThread_Type) != 0) return -1;
 if (DeeError_TypeError_CheckType(inner_tb,&DeeTraceback_Type) != 0) return -1;
 if ((self->ob_message = (DeeStringObject *)DeeString_Newf(
  "Thread %k crashed:\n>> %r\n%k",thread,inner,inner_tb)) == NULL) return -1;
 Dee_INCREF(self->ob_thread = (DeeThreadObject *)thread);
 Dee_INCREF(self->ob_inner = inner);
 Dee_INCREF(self->ob_inner_tb = (DeeTracebackObject *)inner_tb);
 return 0;
}
static void _deethreadcrash_tp_dtor(
 DeeThreadCrashErrorObject *self) {
 Dee_DECREF(self->ob_thread);
 Dee_DECREF(self->ob_inner);
 Dee_DECREF(self->ob_inner_tb);
}

DEE_VISIT_PROC(_deethreadcrash_tp_visit,DeeThreadCrashErrorObject *self) {
 Dee_VISIT(self->ob_thread);
 Dee_VISIT(self->ob_inner);
 Dee_VISIT(self->ob_inner_tb);
}

static struct DeeMemberDef const _deethreadcrash_tp_members[] = {
 DEE_MEMBERDEF_NAMED_v100("thread",DeeThreadCrashErrorObject,ob_thread,object),
 DEE_MEMBERDEF_NAMED_v100("error",DeeThreadCrashErrorObject,ob_inner,object),
 DEE_MEMBERDEF_NAMED_v100("traceback",DeeThreadCrashErrorObject,ob_inner_tb,object),
 DEE_MEMBERDEF_END_v100
};

DeeTypeObject DeeErrorType_ThreadCrash = {
 DEE_TYPE_OBJECT_HEAD_v100(member("Error.ThreadCrash"),null,null,member(&DeeErrorType_Error)),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeThreadCrashErrorObject),null,null,
  member(&_deethreadcrash_tp_copy_ctor),null,
  member(&_deethreadcrash_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deethreadcrash_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deethreadcrash_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deethreadcrash_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DEE_DECL_END

#endif /* !GUARD_DEEMON_ERROR_C */
