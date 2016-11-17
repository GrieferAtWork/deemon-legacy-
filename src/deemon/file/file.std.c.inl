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
#ifndef GUARD_DEEMON_FILE_STD_C_INL
#define GUARD_DEEMON_FILE_STD_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/file.h>
#include <deemon/file/file.fd.h>
#include <deemon/sys/sysfd.h>
#include <deemon/optional/atomic_mutex.h>
#ifdef DEE_PLATFORM_WINDOWS
#include <deemon/file/file.win32_debug.h.inl>
#endif

DEE_DECL_BEGIN

#if (defined(DeeSysFD_INIT_STDOUT) || !defined(DeeSysFD_GET_STDOUT))\
 && (defined(DeeSysFD_INIT_STDERR) || !defined(DeeSysFD_GET_STDERR))\
 && (defined(DeeSysFD_INIT_STDIN)  || !defined(DeeSysFD_GET_STDIN))
#define DEE_SYSFD_HAVE_STATIC_STDFD_INITIALIZER
#endif

static struct {
#if defined(DeeSysFD_INIT_STDOUT) || defined(DeeSysFD_GET_STDOUT)
 DeeFileFDObject       dp_stdout;
#endif
#if defined(DeeSysFD_INIT_STDERR) || defined(DeeSysFD_GET_STDERR)
 DeeFileFDObject       dp_stderr;
#endif
#if defined(DeeSysFD_INIT_STDIN) || defined(DeeSysFD_GET_STDIN)
 DeeFileFDObject       dp_stdin;
#endif
 DeeFileObject         dp_stdnull;
#ifdef DEE_PLATFORM_WINDOWS
 DeeFileWin32DbgObject dp_stddbg;
#endif
}
#ifdef DEE_SYSFD_HAVE_STATIC_STDFD_INITIALIZER
const /*< Only make this constant if all initializers are static. */
#endif
_dee_default_printers_data = {
 // NOTE: All printers are initialized with two references, because
 //       the second one is stored in the vector of active printers below.
#ifdef DeeSysFD_INIT_STDOUT
 DeeFileFDObject_INIT_REF_VALID(&DeeFileFD_Type,2,DeeSysFD_INIT_STDOUT()), /*< dp_stdout. */
#elif defined(DeeSysFD_GET_STDOUT)
 DeeFileFDObject_INIT_REF_INVALID(&DeeFileFD_Type,2), /*< dp_stdout. */
#endif
#ifdef DeeSysFD_INIT_STDERR
 DeeFileFDObject_INIT_REF_VALID(&DeeFileFD_Type,2,DeeSysFD_INIT_STDERR()), /*< dp_stderr. */
#elif defined(DeeSysFD_GET_STDERR)
 DeeFileFDObject_INIT_REF_INVALID(&DeeFileFD_Type,2), /*< dp_stderr. */
#endif
#ifdef DeeSysFD_INIT_STDIN
 DeeFileFDObject_INIT_REF_VALID(&DeeFileFD_Type,2,DeeSysFD_INIT_STDIN()), /*< dp_stdin. */
#elif defined(DeeSysFD_GET_STDIN)
 DeeFileFDObject_INIT_REF_INVALID(&DeeFileFD_Type,2), /*< dp_stdin. */
#endif
 {DEE_FILE_OBJECT_HEAD_INIT_REF(&DeeFile_Type,2)} /*< dp_stdnull. */
#ifdef DEE_PLATFORM_WINDOWS
 ,{DEE_FILE_OBJECT_HEAD_INIT_REF(&DeeFileWin32Dbg_Type,2)}
#endif
};

static DeeFileObject *const _dee_default_printers[_DEE_STDMAX+1] = {
#if defined(DeeSysFD_INIT_STDOUT) || defined(DeeSysFD_GET_STDOUT)
 (DeeFileObject *)&_dee_default_printers_data.dp_stdout,
#else
 (DeeFileObject *)&_dee_default_printers_data.dp_stdnull,
#endif
#if defined(DeeSysFD_INIT_STDERR) || defined(DeeSysFD_GET_STDERR)
 (DeeFileObject *)&_dee_default_printers_data.dp_stderr,
#else
 (DeeFileObject *)&_dee_default_printers_data.dp_stdnull,
#endif
#if defined(DeeSysFD_INIT_STDIN) || defined(DeeSysFD_GET_STDIN)
 (DeeFileObject *)&_dee_default_printers_data.dp_stdin,
#else
 (DeeFileObject *)&_dee_default_printers_data.dp_stdnull,
#endif
 (DeeFileObject *)&_dee_default_printers_data.dp_stdnull,
#ifdef DEE_PLATFORM_WINDOWS
 (DeeFileObject *)&_dee_default_printers_data.dp_stddbg,
#elif defined(DeeSysFD_INIT_STDERR) || defined(DeeSysFD_GET_STDERR)
 (DeeFileObject *)&_dee_default_printers_data.dp_stderr,
#else
 (DeeFileObject *)&_dee_default_printers_data.dp_stdnull,
#endif
};


DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_OBJECT(DeeFileObject) *
DeeFile_DefaultStd(DEE_A_IN enum DeeStdPrinter printer) {
 DEE_ASSERTF(printer >= 0 && printer <= _DEE_STDMAX,
             "Invalid printer: %d",(int)printer);
#ifdef DEE_SYSFD_HAVE_STATIC_STDFD_INITIALIZER
 /* Simply return the staticly allocated printer. */
 return (DeeObject *)_dee_default_printers[printer];
#else
 /* A bit more difficult: We need to initialize the printers manually (and only once). */
 switch (printer) {

#define LOAD_PRINTER(fd,getter)\
 /* NOTE: Since the std-handle getters don't allow calls to quit functions,\
          there is no need to make sure only one thread is calling them. */\
 if ((DeeAtomic32_Load(fd.fo_flags,\
  memory_order_seq_cst)&DEE_PRIVATE_FILEFLAG_STDINIT)==0) {\
  getter(&fd.fd_descr);\
  DeeAtomic32_FetchOr(fd.fo_flags,DEE_PRIVATE_FILEFLAG_STDINIT,memory_order_seq_cst);\
 }
#ifdef DeeSysFD_GET_STDOUT
  case DEE_STDOUT:
   LOAD_PRINTER(_dee_default_printers_data.dp_stdout,DeeSysFD_GET_STDOUT)
   return (DeeObject *)&_dee_default_printers_data.dp_stdout;
#endif /* DeeSysFD_GET_STDOUT */
#ifdef DeeSysFD_GET_STDERR
#ifndef DEE_PLATFORM_WINDOWS
  case DEE_STDDBG:
#endif /* !DEE_PLATFORM_WINDOWS */
  case DEE_STDERR:
   LOAD_PRINTER(_dee_default_printers_data.dp_stderr,DeeSysFD_GET_STDERR)
   return (DeeObject *)&_dee_default_printers_data.dp_stderr;
#endif /* DeeSysFD_GET_STDERR */
#ifdef DeeSysFD_GET_STDIN
  case DEE_STDIN:
   LOAD_PRINTER(_dee_default_printers_data.dp_stdin,DeeSysFD_GET_STDIN)
   return (DeeObject *)&_dee_default_printers_data.dp_stdin;
#endif /* DeeSysFD_GET_STDIN */
#undef LOAD_PRINTER
#ifdef DEE_PLATFORM_WINDOWS
  case DEE_STDDBG: return (DeeObject *)&_dee_default_printers_data.dp_stddbg;
#endif /* DEE_PLATFORM_WINDOWS */
  default: return (DeeObject *)&_dee_default_printers_data.dp_stdnull;
 }
#endif
}




static struct DeeAtomicMutex _dee_active_printers_lock = DeeAtomicMutex_INIT();

//////////////////////////////////////////////////////////////////////////
// NOTE: This vector doesn't contain any NULL-entries if it was
//       possible to define 'DEE_SYSFD_HAVE_STATIC_STDFD_INITIALIZER'.
static DeeFileObject *_dee_active_printers[_DEE_STDMAX+1] = {
#ifdef DeeSysFD_INIT_STDOUT
 (DeeFileObject *)&_dee_default_printers_data.dp_stdout,
#elif defined(DeeSysFD_GET_STDOUT)
 NULL,
#else
 (DeeFileObject *)&_dee_default_printers_data.dp_stdnull,
#endif
#ifdef DeeSysFD_INIT_STDERR
 (DeeFileObject *)&_dee_default_printers_data.dp_stderr,
#elif defined(DeeSysFD_GET_STDERR)
 NULL,
#else
 (DeeFileObject *)&_dee_default_printers_data.dp_stdnull,
#endif
#ifdef DeeSysFD_INIT_STDIN
 (DeeFileObject *)&_dee_default_printers_data.dp_stdin,
#elif defined(DeeSysFD_GET_STDIN)
 NULL,
#else
 (DeeFileObject *)&_dee_default_printers_data.dp_stdnull,
#endif
 (DeeFileObject *)&_dee_default_printers_data.dp_stdnull,
#ifdef DEE_PLATFORM_WINDOWS
 (DeeFileObject *)&_dee_default_printers_data.dp_stddbg,
#elif defined(DeeSysFD_INIT_STDERR)
 (DeeFileObject *)&_dee_default_printers_data.dp_stderr,
#elif defined(DeeSysFD_GET_STDERR)
 NULL,
#else
 (DeeFileObject *)&_dee_default_printers_data.dp_stdnull,
#endif
};

DEE_A_RET_OBJECT_REF(DeeFileObject) *
DeeFile_Std(DEE_A_IN enum DeeStdPrinter printer) {
 DeeFileObject *result;
 DEE_ASSERTF(printer >= 0 && printer <= _DEE_STDMAX,
             "Invalid printer: %d",(int)printer);
 DeeAtomicMutex_Acquire(&_dee_active_printers_lock);
#ifdef DEE_SYSFD_HAVE_STATIC_STDFD_INITIALIZER
 // No need to check for NULL, if all printers were statically initialized
 Dee_INCREF(result = _dee_active_printers[printer]);
#else
 if DEE_LIKELY((result = _dee_active_printers[printer]) != NULL) {
  Dee_INCREF(result);
 } else {
  // First time initialization
  // NOTE: We inherit the second reference here!
  // NOTE: This is allowed, as not even 'DeeFile_ResetStd' resets the printers to NULL
  result = (DeeFileObject *)DeeFile_DefaultStd(printer);
  _dee_active_printers[printer] = result;
 }
#endif
 DeeAtomicMutex_Release(&_dee_active_printers_lock);
 return (DeeObject *)result;
}
DEE_A_EXEC void DeeFile_SetStd(DEE_A_IN enum DeeStdPrinter printer,
                               DEE_A_INOUT_OBJECT(DeeFileObject) *fp) {
 DeeFileObject *old_printer;
 DEE_ASSERTF(printer >= 0 && printer <= _DEE_STDMAX,
             "Invalid printer: %d",(int)printer);
 DEE_ASSERT(DeeObject_Check(fp) && DeeFile_Check(fp));
 Dee_INCREF(fp);
 DeeAtomicMutex_Acquire(&_dee_active_printers_lock);
 old_printer = _dee_active_printers[printer];
#ifndef DEE_SYSFD_HAVE_STATIC_STDFD_INITIALIZER
 if DEE_UNLIKELY(!old_printer) {
  // Must still account for uninitialized printers
  Dee_INCREF(old_printer = (DeeFileObject *)DeeFile_DefaultStd(printer));
 }
#endif
 _dee_active_printers[printer] = (DeeFileObject *)fp;
 DeeAtomicMutex_Release(&_dee_active_printers_lock);
 // Drop a reference to the old printer
 Dee_DECREF(old_printer);
}
DEE_A_EXEC void DeeFile_DelStd(DEE_A_IN enum DeeStdPrinter printer) {
 DeeFileObject *old_printer,*default_printer;
 DEE_ASSERTF(printer >= 0 && printer <= _DEE_STDMAX,
             "Invalid printer: %d",(int)printer);
 default_printer = _dee_default_printers[printer];
 DeeAtomicMutex_Acquire(&_dee_active_printers_lock);
 old_printer = _dee_active_printers[printer];
 if (old_printer == default_printer
#ifndef DEE_SYSFD_HAVE_STATIC_STDFD_INITIALIZER
  || !old_printer
#endif
  ) { // Never initialized/Initialized with the default printer
  DeeAtomicMutex_Release(&_dee_active_printers_lock);
  return;
 }
 Dee_INCREF(_dee_active_printers[printer] = default_printer);
 DeeAtomicMutex_Release(&_dee_active_printers_lock);
 // Drop a reference to the old printer
 Dee_DECREF(old_printer);
}

DEE_A_EXEC void DeeFile_ResetStd(void) {
 DeeFileObject *old_printers[_DEE_STDMAX+1];
 DeeAtomicMutex_Acquire(&_dee_active_printers_lock);
 memcpy(old_printers,_dee_active_printers,
        (_DEE_STDMAX+1)*sizeof(DeeFileObject *));
#define PRINTER_ASSIGNED(printer)\
   (_dee_active_printers[printer] != _dee_default_printers[printer]\
 && _dee_active_printers[printer] != NULL)
#define RESET_PRINTER(printer) \
   (_dee_active_printers[printer] = _dee_default_printers[printer])
#ifndef DeeSysFD_INIT_STDOUT
 if (PRINTER_ASSIGNED(DEE_STDOUT)) RESET_PRINTER(DEE_STDOUT);
#else
 RESET_PRINTER(DEE_STDOUT);
#endif
#ifndef DeeSysFD_INIT_STDERR
 if (PRINTER_ASSIGNED(DEE_STDERR)) RESET_PRINTER(DEE_STDERR);
#else
 RESET_PRINTER(DEE_STDERR);
#endif
#ifndef DeeSysFD_INIT_STDIN
 if (PRINTER_ASSIGNED(DEE_STDIN)) RESET_PRINTER(DEE_STDIN);
#else
 RESET_PRINTER(DEE_STDIN);
#endif
 RESET_PRINTER(DEE_STDNULL);
 RESET_PRINTER(DEE_STDDBG);
#undef RESET_PRINTER
#undef PRINTER_ASSIGNED
 DeeAtomicMutex_Release(&_dee_active_printers_lock);
#ifdef DeeSysFD_INIT_STDOUT
 Dee_DECREF(old_printers[DEE_STDOUT]);
#else
 Dee_XDECREF(old_printers[DEE_STDOUT]);
#endif
#ifdef DeeSysFD_INIT_STDERR
 Dee_DECREF(old_printers[DEE_STDERR]);
#else
 Dee_XDECREF(old_printers[DEE_STDERR]);
#endif
#ifdef DeeSysFD_INIT_STDIN
 Dee_DECREF(old_printers[DEE_STDIN]);
#else
 Dee_XDECREF(old_printers[DEE_STDIN]);
#endif
 Dee_DECREF(old_printers[DEE_STDNULL]);
 Dee_DECREF(old_printers[DEE_STDDBG]);
}


DEE_DECL_END

#endif /* !GUARD_DEEMON_FILE_STD_C_INL */
