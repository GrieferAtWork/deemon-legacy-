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
#ifndef GUARD_DEEMON_FILE_IO_INL
#define GUARD_DEEMON_FILE_IO_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/bool.h>
#include <deemon/file.h>
#include <deemon/time.h>
#include <deemon/none.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/memberdef.h>
#include <deemon/fs_api.h>
#include <deemon/optional/object_visit.h>
#include <deemon/optional/object_malloc.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/sys/sysfd.h>
#include DEE_INCLUDE_MEMORY_API()

// */ (nano...)

DEE_DECL_BEGIN

#if DEE_CONFIG_RUNTIME_HAVE_VFS
struct DeeFileIOVFSType {
 int  (*vfs_read)(DeeFileIOObject *self, void *p, Dee_size_t s, Dee_size_t *rs);
 int  (*vfs_write)(DeeFileIOObject *self, void const *p, Dee_size_t s, Dee_size_t *ws);
 int  (*vfs_seek)(DeeFileIOObject *self, Dee_int64_t off, int whence, Dee_uint64_t *pos);
 int  (*vfs_flush)(DeeFileIOObject *self);
 int  (*vfs_trunc)(DeeFileIOObject *self);
 void (*vfs_close)(DeeFileIOObject *self);
};
struct DeeFileIOVFS {
 struct DeeFileIOVFSType *fio_type; /*< [1..1] VFS Type. */
 void                    *fio_data; /*< [?..?] VFS Data. */
};
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

DEE_COMPILER_MSVC_WARNING_PUSH(4201)
#define DeeFileIO_FD(ob)
struct DeeFileIOObject {
 DEE_FILE_OBJECT_HEAD
 // NOTE: This structure
 union{
  struct DeeSysFileFD fio_fd;  /*< System file descriptor. */
#if DEE_CONFIG_RUNTIME_HAVE_VFS
  struct DeeFileIOVFS fio_vfs; /*< Virtual filesystem file. */
#endif
 };
};
#if DEE_CONFIG_RUNTIME_HAVE_VFS
#define DeeFileIO_ISVFS(ob) ((DeeFile_FLAGS(ob)&DEE_PRIVATE_FILEFLAG_IO_VFSFILE)!=0)
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

DEE_COMPILER_MSVC_WARNING_POP


//////////////////////////////////////////////////////////////////////////
// File-fs hybrid functions
// - Both windows and unix have optimized functions to lookup
//   information on file paths from an open file descriptor.
// - These informations aren't always available, but even if they
//   aren't, we can still implement these functions since we store
//   the file name in the io object.
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) *
DeeFileIO_GetTimes(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DeeObject *a,*c,*m,*result;
 if DEE_UNLIKELY(DeeFileIO_GetTimes2(self,&a,&c,&m) != 0) return NULL;
 result = DeeTuple_Pack(3,a,c,m);
 Dee_DECREF(a);
 Dee_DECREF(c);
 Dee_DECREF(m);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
DeeFileIO_GetATime(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeFileIO_GetTimes2(self,&result,NULL,NULL) != 0) return NULL;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
DeeFileIO_GetCTime(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeFileIO_GetTimes2(self,NULL,&result,NULL) != 0) return NULL;
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTimeObject) *
DeeFileIO_GetMTime(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeFileIO_GetTimes2(self,NULL,NULL,&result) != 0) return NULL;
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFileIO_SetTimes2(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *self,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) *tm_access,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) *tm_creation,
 DEE_A_IN_OBJECT_OPT(struct DeeTimeObject) *tm_modification) {
#ifdef DEE_PLATFORM_WINDOWS
 DWORD error;
 FILETIME ftm_access,ftm_creation,ftm_modification;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 DEE_ASSERT(!tm_access || (DeeObject_Check(tm_access) && DeeTime_Check(tm_access)));
 DEE_ASSERT(!tm_creation || (DeeObject_Check(tm_creation) && DeeTime_Check(tm_creation)));
 DEE_ASSERT(!tm_modification || (DeeObject_Check(tm_modification) && DeeTime_Check(tm_modification)));
 if (tm_access && DeeTime_AsWin32FileTime(tm_access,&ftm_access) != 0) return -1;
 if (tm_creation && DeeTime_AsWin32FileTime(tm_creation,&ftm_creation) != 0) return -1;
 if (tm_modification && DeeTime_AsWin32FileTime(tm_modification,&ftm_modification) != 0) return -1;
 if DEE_UNLIKELY(!SetFileTime(DeeFileIO_HANDLE(self),
  tm_creation ? &ftm_creation : NULL,
  tm_access ? &ftm_access : NULL,
  tm_modification ? &ftm_modification : NULL)) {
  error = DeeSystemError_Win32Consume();
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "SetFileTime(%p:%r,<%O>,<%O>,<%O>) : %K",
                      DeeFileIO_HANDLE(self),DeeFileIO_FILE(self),
                      tm_creation,tm_access,tm_modification,
                      DeeSystemError_Win32ToString(error));
  return -1;
 }
 return 0;
#else
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 return _DeeFS_SetTimes2Object(DeeFileIO_FILE(self),tm_access,tm_creation,tm_modification);
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeFileIO_GetTimes2(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *self,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_access,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_creation,
 DEE_A_REF DEE_A_OUT_OBJECT_OPT(struct DeeTimeObject) **tm_modification) {
#ifdef DEE_PLATFORM_WINDOWS
 WIN32_FILE_ATTRIBUTE_DATA attribute_data;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 if DEE_UNLIKELY(!GetFileTime(DeeFileIO_HANDLE(self),
  tm_creation ? &attribute_data.ftCreationTime : NULL,
  tm_access ? &attribute_data.ftLastAccessTime : NULL,
  tm_modification ? &attribute_data.ftLastWriteTime : NULL)) {
  unsigned long error = DeeSystemError_Win32Consume();
  if DEE_LIKELY(DeeFileIO_HANDLE(self) == DEE_FILEIO_INVALID_HANDLE) {
   // This can happen if 'path' is not a file, but a directory
   if DEE_UNLIKELY(DeeWideString_Check(DeeFileIO_FILE(self))) {
    if DEE_UNLIKELY(!GetFileAttributesExW(DeeWideString_STR(
     DeeFileIO_FILE(self)),GetFileExInfoStandard,&attribute_data)) {
     SetLastError(0);
    } else goto put_data;
   } else {
    DeeObject *utf8_path; BOOL temp;
    if DEE_UNLIKELY((utf8_path = DeeUtf8String_Cast(DeeFileIO_FILE(self))) == NULL) return -1;
    temp = GetFileAttributesExA(DeeUtf8String_STR(utf8_path),GetFileExInfoStandard,&attribute_data);
    Dee_DECREF(utf8_path);
    if DEE_LIKELY(temp) goto put_data;
    SetLastError(0);
   }
  }
  DeeError_SetStringf(&DeeErrorType_SystemError,"GetFileTime(%r) : %K",
                      DeeFileIO_FILE(self),DeeSystemError_Win32ToString(error));
 }
put_data:
 if (tm_access && (*tm_access = DeeTime_NewFromWin32FileTime(&attribute_data.ftLastAccessTime)) == NULL) return -1;
 if (tm_creation && (*tm_creation = DeeTime_NewFromWin32FileTime(&attribute_data.ftCreationTime)) == NULL) goto err_0;
 if (tm_modification && (*tm_modification = DeeTime_NewFromWin32FileTime(&attribute_data.ftLastWriteTime)) == NULL) goto err_1;
 return 0;
err_1: if (tm_creation) Dee_DECREF(*tm_creation);
err_0: if (tm_access) Dee_DECREF(*tm_access);
 return -1;
#else // TODO: fstat
 // Fallback: Implement with file name
 return _DeeFS_GetTimes2Object(DeeFileIO_FILE(self),tm_access,tm_creation,tm_modification);
#endif
}


#ifdef DEE_PLATFORM_WINDOWS
struct _DeeFileWin32DbgObject { DEE_FILE_OBJECT_HEAD };
static DeeObject *_deefilewin32dbg_tp_str(struct _DeeFileWin32DbgObject *DEE_UNUSED(self)) {
 DeeReturn_STATIC_STRING("<file.io.stddbg: mode \"w\", pos 0/0>");
}
static int _deefilewin32dbg_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), struct _DeeFileWin32DbgObject *self) {
 _DeeFile_Init(self);
 return 0;
}
static int _deefilewin32dbg_tp_io_read(
 struct _DeeFileWin32DbgObject *DEE_UNUSED(self), void *DEE_UNUSED(p),
 Dee_size_t DEE_UNUSED(s), Dee_size_t *DEE_UNUSED(rs)) {
 DeeError_SET_STRING(&DeeErrorType_IOError,"Can't read file.io.stddbg");
 return -1;
}
static int _deefilewin32dbg_tp_io_write(
#ifndef DEE_WITHOUT_THREADS
 struct _DeeFileWin32DbgObject *self,
#else
 struct _DeeFileWin32DbgObject *DEE_UNUSED(self),
#endif
 void const *p, Dee_size_t s, Dee_size_t *ws) {
 enum{buffer_size=256};
 char buffer[buffer_size+1];
 *ws = s;
 DeeFile_ACQUIRE(self);
 while DEE_LIKELY(s >= buffer_size) {
  memcpy(buffer,p,buffer_size);
  buffer[buffer_size] = '\0';
  OutputDebugStringA(buffer);
  *(Dee_uintptr_t *)&p += buffer_size;
  s -= buffer_size;
 }
 if DEE_LIKELY(s) {
  memcpy(buffer,p,s);
  buffer[s] = '\0';
  OutputDebugStringA(buffer);
 }
 DeeFile_RELEASE(self);
 return 0;
}
static int _deefilewin32dbg_tp_io_seek(
 struct _DeeFileWin32DbgObject *DEE_UNUSED(self), Dee_int64_t DEE_UNUSED(off),
 int DEE_UNUSED(whence), Dee_uint64_t *pos) {
 if (pos) *pos = 0;
 return 0;
}
static int _deefilewin32dbg_tp_io_flush(struct _DeeFileWin32DbgObject *DEE_UNUSED(self)) { return 0; }
static int _deefilewin32dbg_tp_io_trunc(struct _DeeFileWin32DbgObject *DEE_UNUSED(self)) {
 DeeError_SET_STRING(&DeeErrorType_IOError,"Can't truncate file.io.stddbg");
 return -1;
}
#endif


DeeString_NEW_STATIC(_deefileio_stdout_name,"/dev/stdout");
DeeString_NEW_STATIC(_deefileio_stderr_name,"/dev/stderr");
DeeString_NEW_STATIC(_deefileio_stdin_name, "/dev/stdin");
struct _DeeFileStdObjects {
 DeeFileIOObject               ob_stdout;
 DeeFileIOObject               ob_stderr;
 DeeFileIOObject               ob_stdin;
 DeeFileObject                 ob_stdnull;
#ifdef DEE_PLATFORM_WINDOWS
 struct _DeeFileWin32DbgObject ob_stddbg;
#else
 DeeFileIOObject               ob_stddbg;
#endif
};
static Dee_size_t const _deefile_std_printers_offset[] = {
 Dee_OFFSETOF(struct _DeeFileStdObjects,ob_stdout),  // DEE_STDOUT
 Dee_OFFSETOF(struct _DeeFileStdObjects,ob_stderr),  // DEE_STDERR
 Dee_OFFSETOF(struct _DeeFileStdObjects,ob_stdin),   // DEE_STDIN
 Dee_OFFSETOF(struct _DeeFileStdObjects,ob_stdnull), // DEE_STDNULL
 Dee_OFFSETOF(struct _DeeFileStdObjects,ob_stddbg),  // DEE_STDDBG
};

#define _DeeFile_STD_DEFAULT(printer) \
 (DeeFileObject *)((Dee_uintptr_t)&_deefile_std_printers+_deefile_std_printers_offset[(int)(printer)])
static struct _DeeFileStdObjects _deefile_std_printers = {
 // NOTE: We initialize these with 2 references, as each stores a second one in '_deefile_std_overrides'
#if defined(DEE_PLATFORM_UNIX)
 {DEE_FILE_OBJECT_HEAD_INIT_REF(&DeeFileIO_Type,2),{(DeeStringObject *)&_deefileio_stdout_name},DEE_OPENMODE_APPEND,STDOUT_FILENO},
 {DEE_FILE_OBJECT_HEAD_INIT_REF(&DeeFileIO_Type,2),{(DeeStringObject *)&_deefileio_stderr_name},DEE_OPENMODE_APPEND,STDERR_FILENO},
 {DEE_FILE_OBJECT_HEAD_INIT_REF(&DeeFileIO_Type,2),{(DeeStringObject *)&_deefileio_stdin_name}, DEE_OPENMODE_READ,  STDIN_FILENO},
#else
 {DEE_FILE_OBJECT_HEAD_INIT_REF(&DeeFileIO_Type,2),{(DeeStringObject *)&_deefileio_stdout_name},DEE_OPENMODE_APPEND,DEE_FILEIO_INVALID_HANDLE},
 {DEE_FILE_OBJECT_HEAD_INIT_REF(&DeeFileIO_Type,2),{(DeeStringObject *)&_deefileio_stderr_name},DEE_OPENMODE_APPEND,DEE_FILEIO_INVALID_HANDLE},
 {DEE_FILE_OBJECT_HEAD_INIT_REF(&DeeFileIO_Type,2),{(DeeStringObject *)&_deefileio_stdin_name}, DEE_OPENMODE_READ,  DEE_FILEIO_INVALID_HANDLE},
#endif
 {DEE_FILE_OBJECT_HEAD_INIT_REF(&DeeFile_Type,2)},
#ifdef DEE_PLATFORM_WINDOWS
 {DEE_FILE_OBJECT_HEAD_INIT_REF(&DeeFileWin32Dbg_Type,2)},
#elif defined(DEE_PLATFORM_UNIX)
 {DEE_FILE_OBJECT_HEAD_INIT_REF(&DeeFileIO_Type,2),{(DeeStringObject *)&_deefileio_stderr_name},DEE_OPENMODE_APPEND,STDERR_FILENO},
#else
 {DEE_FILE_OBJECT_HEAD_INIT_REF(&DeeFileIO_Type,2),{(DeeStringObject *)&_deefileio_stderr_name},DEE_OPENMODE_APPEND,DEE_FILEIO_INVALID_HANDLE},
#endif
};

static struct DeeAtomicMutex _deefile_std_overrides_lock = DeeAtomicMutex_INIT();
static DeeFileObject *_deefile_std_overrides[_DEE_STDMAX+1] = {
 (DeeFileObject *)&_deefile_std_printers.ob_stdout,
 (DeeFileObject *)&_deefile_std_printers.ob_stderr,
 (DeeFileObject *)&_deefile_std_printers.ob_stdin,
 (DeeFileObject *)&_deefile_std_printers.ob_stdnull,
 (DeeFileObject *)&_deefile_std_printers.ob_stddbg
};

void DeeFile_SetStd(
 DEE_A_IN enum DeeStdPrinter printer,
 DEE_A_INOUT_OBJECT(DeeFileObject) *fp) {
 DeeFileObject *old;
 DEE_ASSERT((int)printer <= (int)_DEE_STDMAX);
 DEE_ASSERT(DeeObject_Check(fp) && DeeFile_Check(fp));
 DeeAtomicMutex_Acquire(&_deefile_std_overrides_lock);
 old = _deefile_std_overrides[printer];
 Dee_INCREF(_deefile_std_overrides[printer] = (DeeFileObject *)fp);
 DeeAtomicMutex_Release(&_deefile_std_overrides_lock);
 Dee_DECREF(old);
}

void DeeFile_DelStd(DEE_A_IN enum DeeStdPrinter printer) {
 DeeFileObject *reset,*old;
 DEE_ASSERT((int)printer <= (int)_DEE_STDMAX);
 reset = _DeeFile_STD_DEFAULT(printer);
 DeeAtomicMutex_Acquire(&_deefile_std_overrides_lock);
 if (_deefile_std_overrides[printer] != reset) {
  old = _deefile_std_overrides[printer];
  Dee_INCREF(_deefile_std_overrides[printer] = reset);
  DeeAtomicMutex_Release(&_deefile_std_overrides_lock);
  Dee_DECREF(old);
 } else {
  DeeAtomicMutex_Release(&_deefile_std_overrides_lock);
 }
}
#define RESET(n,id,default)\
 _old_files[n] = _deefile_std_overrides[id];\
 Dee_INCREF(_deefile_std_overrides[id] = default)
 

void DeeFile_ResetStd(void) {
 DeeFileObject *_old_files[5];
 DeeAtomicMutex_Acquire(&_deefile_std_overrides_lock);
 RESET(0,DEE_STDOUT,(DeeFileObject *)&_deefile_std_printers.ob_stdout);
 RESET(1,DEE_STDERR,(DeeFileObject *)&_deefile_std_printers.ob_stderr);
 RESET(2,DEE_STDIN,(DeeFileObject *)&_deefile_std_printers.ob_stdin);
 RESET(3,DEE_STDNULL,(DeeFileObject *)&_deefile_std_printers.ob_stdnull);
 RESET(4,DEE_STDDBG,(DeeFileObject *)&_deefile_std_printers.ob_stddbg);
 DeeAtomicMutex_Release(&_deefile_std_overrides_lock);
 Dee_DECREF(_old_files[4]); Dee_DECREF(_old_files[3]);
 Dee_DECREF(_old_files[2]); Dee_DECREF(_old_files[1]);
 Dee_DECREF(_old_files[0]);
}
#undef RESET

DEE_A_RET_OBJECT(DeeFileObject) *DeeFile_DefaultStd(DEE_A_IN enum DeeStdPrinter printer) {
 DEE_ASSERT((int)printer <= (int)_DEE_STDMAX);
#if defined(DEE_PLATFORM_WINDOWS)
 switch (printer) {
  case DEE_STDOUT:
   if (DeeAtomicPtr_Load(_deefile_std_printers.ob_stdout.fio_handle,memory_order_seq_cst) == DEE_FILEIO_INVALID_HANDLE) {
    HANDLE new_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DeeAtomicPtr_CompareExchangeStrong(_deefile_std_printers.ob_stdout.fio_handle,
                                       DEE_FILEIO_INVALID_HANDLE,new_handle,
                                       memory_order_seq_cst,memory_order_seq_cst);
   }
   return (DeeObject *)&_deefile_std_printers.ob_stdout;
  case DEE_STDERR:
   if (DeeAtomicPtr_Load(_deefile_std_printers.ob_stderr.fio_handle,memory_order_seq_cst) == DEE_FILEIO_INVALID_HANDLE) {
    HANDLE new_handle = GetStdHandle(STD_ERROR_HANDLE);
    DeeAtomicPtr_CompareExchangeStrong(_deefile_std_printers.ob_stderr.fio_handle,
                                       DEE_FILEIO_INVALID_HANDLE,new_handle,
                                       memory_order_seq_cst,memory_order_seq_cst);
   }
   return (DeeObject *)&_deefile_std_printers.ob_stderr;
  case DEE_STDIN:
   if (DeeAtomicPtr_Load(_deefile_std_printers.ob_stdin.fio_handle,memory_order_seq_cst) == DEE_FILEIO_INVALID_HANDLE) {
    HANDLE new_handle = GetStdHandle(STD_INPUT_HANDLE);
    DeeAtomicPtr_CompareExchangeStrong(_deefile_std_printers.ob_stdin.fio_handle,
                                       DEE_FILEIO_INVALID_HANDLE,new_handle,
                                       memory_order_seq_cst,memory_order_seq_cst);
   }
   return (DeeObject *)&_deefile_std_printers.ob_stdin;
  case DEE_STDNULL:
   return (DeeObject *)&_deefile_std_printers.ob_stdnull;
  case DEE_STDDBG:
   return (DeeObject *)&_deefile_std_printers.ob_stddbg;
  default: DEE_BUILTIN_UNREACHABLE();
 }
#elif defined(DEE_PLATFORM_UNIX)
 return (DeeObject *)_DeeFile_STD_DEFAULT(printer);
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
 switch (printer) {
  case DEE_STDOUT:
   if (DeeAtomicPtr_Load(_deefile_std_printers.ob_stdout.fio_handle,memory_order_seq_cst) == DEE_FILEIO_INVALID_HANDLE) {
    void *new_handle = (void *)stdout;
    DeeAtomicPtr_CompareExchangeStrong(_deefile_std_printers.ob_stdout.fio_handle,
                                       DEE_FILEIO_INVALID_HANDLE,new_handle,
                                       memory_order_seq_cst,memory_order_seq_cst);
   }
   return (DeeObject *)&_deefile_std_printers.ob_stdout;
  case DEE_STDERR:
   if (DeeAtomicPtr_Load(_deefile_std_printers.ob_stderr.fio_handle,memory_order_seq_cst) == DEE_FILEIO_INVALID_HANDLE) {
    void *new_handle = (void *)stderr;
    DeeAtomicPtr_CompareExchangeStrong(_deefile_std_printers.ob_stderr.fio_handle,
                                       DEE_FILEIO_INVALID_HANDLE,new_handle,
                                       memory_order_seq_cst,memory_order_seq_cst);
   }
   return (DeeObject *)&_deefile_std_printers.ob_stderr;
  case DEE_STDIN:
   if (DeeAtomicPtr_Load(_deefile_std_printers.ob_stdin.fio_handle,memory_order_seq_cst) == DEE_FILEIO_INVALID_HANDLE) {
    void *new_handle = (void *)stdin;
    DeeAtomicPtr_CompareExchangeStrong(_deefile_std_printers.ob_stdin.fio_handle,
                                       DEE_FILEIO_INVALID_HANDLE,new_handle,
                                       memory_order_seq_cst,memory_order_seq_cst);
   }
   return (DeeObject *)&_deefile_std_printers.ob_stdin;
  case DEE_STDNULL:
   return (DeeObject *)&_deefile_std_printers.ob_stdnull;
  case DEE_STDDBG:
   if (DeeAtomicPtr_Load(_deefile_std_printers.ob_stddbg.fio_handle,memory_order_seq_cst) == DEE_FILEIO_INVALID_HANDLE) {
    void *new_handle = (void *)stderr;
    DeeAtomicPtr_CompareExchangeStrong(_deefile_std_printers.ob_stddbg.fio_handle,
                                       DEE_FILEIO_INVALID_HANDLE,new_handle,
                                       memory_order_seq_cst,memory_order_seq_cst);
   }
   return (DeeObject *)&_deefile_std_printers.ob_stddbg;
  default: DEE_BUILTIN_UNREACHABLE();
 }
#else
#error "file.io not supported"
#endif
}
DEE_A_RET_OBJECT_REF(DeeFileObject) *
DeeFile_Std(DEE_A_IN enum DeeStdPrinter printer) {
 DeeObject *result;
 DEE_ASSERT((int)printer <= (int)_DEE_STDMAX);
#if defined(DEE_PLATFORM_WINDOWS)
 DeeAtomicMutex_Acquire(&_deefile_std_overrides_lock);
 if (_deefile_std_overrides[(int)printer] != _DeeFile_STD_DEFAULT(printer)) {
  Dee_INCREF(result = (DeeObject *)_deefile_std_overrides[(int)printer]);
  DeeAtomicMutex_Release(&_deefile_std_overrides_lock);
 } else {
  DeeAtomicMutex_Release(&_deefile_std_overrides_lock);
  Dee_INCREF(result = DeeFile_DefaultStd(printer));
 }
#elif defined(DEE_PLATFORM_UNIX)
 DeeAtomicMutex_Acquire(&_deefile_std_overrides_lock);
 Dee_INCREF(result = (DeeObject *)_deefile_std_overrides[(int)printer]);
 DeeAtomicMutex_Release(&_deefile_std_overrides_lock);
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
 DeeAtomicMutex_Acquire(&_deefile_std_overrides_lock);
 if (_deefile_std_overrides[(int)printer] != _DeeFile_STD_DEFAULT(printer)) {
  Dee_INCREF(result = (DeeObject *)_deefile_std_overrides[(int)printer]);
  DeeAtomicMutex_Release(&_deefile_std_overrides_lock);
 } else {
  DeeAtomicMutex_Release(&_deefile_std_overrides_lock);
  Dee_INCREF(result = DeeFile_DefaultStd(printer));
}
#else
#error "file.io not supported"
#endif
 return result;
}


#ifdef DEE_PLATFORM_WINDOWS
DeeFileTypeObject DeeFileWin32Dbg_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("file.io.stddbg"),null,
   null,member((DeeTypeObject *)&DeeFile_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(struct _DeeFileWin32DbgObject),null,
   member(&_deefilewin32dbg_tp_ctor),null,null,null),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
  DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&_deefilewin32dbg_tp_str),
   member(&_deefilewin32dbg_tp_str),null,null,null),
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
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_FOOTER_v100
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(
  member(&_deefilewin32dbg_tp_io_read),
  member(&_deefilewin32dbg_tp_io_write),
  member(&_deefilewin32dbg_tp_io_seek),
  member(&_deefilewin32dbg_tp_io_flush),
  member(&_deefilewin32dbg_tp_io_trunc),null),
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};
#endif




#define DeeError_IOFILESELF_Error(func) \
  DeeError_IOFileError(DeeFileIO_FILE(self),DeeString_STR(DeeFile_ModeToString(DeeFileIO_MODE(self))),func)
#define DeeError_IOFILESELF_ErrorExplicit(func,error) \
  DeeError_IOFileError_Explicit(DeeFileIO_FILE(self),DeeString_STR(DeeFile_ModeToString(DeeFileIO_MODE(self))),func,error)
#ifdef DEE_PLATFORM_WINDOWS
#define DeeError_IOFileError(filename,mode,func) \
 DeeError_IOFileError_Explicit(filename,mode,func,DeeSystemError_Win32Consume())
#else
#define DeeError_IOFileError(filename,mode,func) \
 DeeError_IOFileError_Explicit(filename,mode,func,DeeSystemError_Consume())
#endif
#ifdef DEE_PLATFORM_WINDOWS
static void DeeError_IOFileError_Explicit(
 DeeObject const *filename, char const *mode, char const *func, DWORD error) {
 DeeError_SetStringf(&DeeErrorType_IOError,"%s(%r,%q) : %K",
  func,filename,mode,DeeSystemError_Win32ToString(error));
}
#else
static void DeeError_IOFileError_Explicit(
 DeeObject const *filename, char const *mode, char const *func, int error) {
 DeeError_SetStringf(&DeeErrorType_IOError,"%s(%r,%q) : %K",
  func,filename,mode,DeeSystemError_ToString(error));
}
#endif


// v implemented in "src/deemon/fs_api.c"
extern DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int _DeeFileIO_Utf8InitObjectEx(DEE_A_IN DeeFileIOObject *self, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions);
extern DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int _DeeFileIO_WideInitObjectEx(DEE_A_IN DeeFileIOObject *self, DEE_A_IN_OBJECT(DeeWideStringObject) const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions);
DEE_STATIC_INLINE(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) DeeFileIO_Utf8InitObjectEx(DEE_A_IN DeeFileIOObject *self, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions);
DEE_STATIC_INLINE(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) DeeFileIO_WideInitObjectEx(DEE_A_IN DeeFileIOObject *self, DEE_A_IN_OBJECT(DeeWideStringObject) const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions);
DEE_STATIC_INLINE(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) DeeFileIO_Utf8InitEx(
 DEE_A_IN DeeFileIOObject *self, DEE_A_IN_Z Dee_Utf8Char const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 int result;  DeeObject *file_ob;
 if DEE_UNLIKELY((file_ob = DeeUtf8String_New(file)) == NULL) return -1;
 result = DeeFileIO_Utf8InitObjectEx(self,file_ob,mode,permissions);
 Dee_DECREF(file_ob);
 return result;
}
DEE_STATIC_INLINE(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) _DeeFileIO_Utf8InitEx(
 DEE_A_IN DeeFileIOObject *self, DEE_A_IN_Z Dee_Utf8Char const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 int result;  DeeObject *file_ob;
 if DEE_UNLIKELY((file_ob = DeeUtf8String_New(file)) == NULL) return -1;
 result = _DeeFileIO_Utf8InitObjectEx(self,file_ob,mode,permissions);
 Dee_DECREF(file_ob);
 return result;
}
DEE_STATIC_INLINE(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) DeeFileIO_WideInitEx(
 DEE_A_IN DeeFileIOObject *self, DEE_A_IN_Z Dee_WideChar const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 int result;  DeeObject *file_ob;
 if DEE_UNLIKELY((file_ob = DeeWideString_New(file)) == NULL) return -1;
 result = DeeFileIO_WideInitObjectEx(self,file_ob,mode,permissions);
 Dee_DECREF(file_ob);
 return result;
}
DEE_STATIC_INLINE(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) _DeeFileIO_WideInitEx(
 DEE_A_IN DeeFileIOObject *self, DEE_A_IN_Z Dee_WideChar const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 int result;  DeeObject *file_ob;
 if DEE_UNLIKELY((file_ob = DeeWideString_New(file)) == NULL) return -1;
 result = _DeeFileIO_WideInitObjectEx(self,file_ob,mode,permissions);
 Dee_DECREF(file_ob);
 return result;
}
DEE_A_INTERRUPT DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *
DeeFile_OpenTemporary(DEE_A_IN Dee_uint32_t flags) {
 DeeString_NEW_STATIC(deemon_prefix,"deemon_fileio_temp");
 DeeObject *filename,*tmp,*result;
 if DEE_UNLIKELY((tmp = DeeFS_GetTmp()) == NULL) return NULL;
 filename = DeeFS_GetTmpNameObject(tmp,(DeeObject *)&deemon_prefix);
 Dee_DECREF(tmp);
 if DEE_UNLIKELY(!filename) return NULL;
 result = _DeeFile_OpenObject(filename,"w");
 Dee_DECREF(filename);
 if ((flags&DEE_FILE_OPENTEMPORARY_FLAG_DELETE_WHEN_CLOSED)!=0 && result)
  ((DeeFileObject *)result)->fo_flags |= DEE_PRIVATE_FILEFLAG_IO_DELONCLOSE;
 return result;
}
DEE_A_INTERRUPT DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *DeeFile_Utf8OpenEx(
 DEE_A_IN_Z Dee_Utf8Char const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 DeeFileIOObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeFileIOObject,
  "file object (file: %s; mode: %s)",file,mode)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DeeFileIO_Type);
  if DEE_UNLIKELY(DeeFileIO_Utf8InitEx(result,file,mode,permissions) != 0) {
   _DeeObject_DELETE(&DeeFileIO_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_INTERRUPT DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *_DeeFile_Utf8OpenEx(
 DEE_A_IN_Z Dee_Utf8Char const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 DeeFileIOObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOCF(
  DeeFileIOObject,"file object (file: %s; mode: %s)",file,mode)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DeeFileIO_Type);
  if DEE_UNLIKELY(_DeeFileIO_Utf8InitEx(result,file,mode,permissions) != 0) {
   _DeeObject_DELETE(&DeeFileIO_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_INTERRUPT DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *DeeFile_WideOpenEx(
 DEE_A_IN_Z Dee_WideChar const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 DeeFileIOObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeFileIOObject,
  "file object (file: %ls; mode: %s)",file,mode)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DeeFileIO_Type);
  if DEE_UNLIKELY(DeeFileIO_WideInitEx(result,file,mode,permissions) != 0) {
   _DeeObject_DELETE(&DeeFileIO_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_INTERRUPT DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *_DeeFile_WideOpenEx(
 DEE_A_IN_Z Dee_WideChar const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 DeeFileIOObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOCF(DeeFileIOObject,
  "file object (file: %ls; mode: %s)",file,mode)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DeeFileIO_Type);
  if DEE_UNLIKELY(_DeeFileIO_WideInitEx(result,file,mode,permissions) != 0) {
   _DeeObject_DELETE(&DeeFileIO_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_INTERRUPT DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *DeeFile_OpenObjectEx(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 DeeFileIOObject *result;
 if DEE_UNLIKELY(DeeWideString_Check(file)) Dee_INCREF(file);
 else if DEE_UNLIKELY((file = DeeUtf8String_Cast(file)) == NULL) return NULL;
 if DEE_LIKELY((result = DeeObject_MALLOCF(
  DeeFileIOObject,"file object (file: %p; mode: %s)",file,mode)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DeeFileIO_Type);
  if DEE_UNLIKELY((DEE_UNLIKELY(DeeWideString_Check(file))
   ? DeeFileIO_WideInitObjectEx(result,file,mode,permissions)
   : DeeFileIO_Utf8InitObjectEx(result,file,mode,permissions)) != 0) {
   _DeeObject_DELETE(&DeeFileIO_Type,result);
   result = NULL;
  }
 }
 Dee_DECREF(file);
 return (DeeObject *)result;
}
DEE_A_INTERRUPT DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *_DeeFile_OpenObjectEx(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *file, DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 DeeFileIOObject *result;
 if DEE_UNLIKELY(DeeWideString_Check(file)) Dee_INCREF(file);
 else if DEE_UNLIKELY((file = DeeUtf8String_Cast(file)) == NULL) return NULL;
 if DEE_LIKELY((result = DeeObject_MALLOCF(
  DeeFileIOObject,"file object (file: %p; mode: %s)",file,mode)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DeeFileIO_Type);
  if DEE_UNLIKELY((DEE_UNLIKELY(DeeWideString_Check(file))
   ? _DeeFileIO_WideInitObjectEx(result,file,mode,permissions)
   : _DeeFileIO_Utf8InitObjectEx(result,file,mode,permissions)) != 0) {
   _DeeObject_DELETE(&DeeFileIO_Type,result);
   result = NULL;
  }
 }
 Dee_DECREF(file);
 return (DeeObject *)result;
}


DEE_STATIC_INLINE(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) DeeFileIO_Utf8InitObjectEx(
 DEE_A_IN DeeFileIOObject *self, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 int result;
 if DEE_UNLIKELY((file = DeeFS_PathExpandObject(file)) == NULL) return -1;
 result = _DeeFileIO_Utf8InitObjectEx(self,file,mode,permissions);
 Dee_DECREF(file);
 return result;
}
DEE_STATIC_INLINE(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) DeeFileIO_WideInitObjectEx(
 DEE_A_IN DeeFileIOObject *self, DEE_A_IN_OBJECT(DeeWideStringObject) const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 int result;
 if DEE_UNLIKELY((file = DeeFS_PathExpandObject(file)) == NULL) return -1;
 result = _DeeFileIO_WideInitObjectEx(self,file,mode,permissions);
 Dee_DECREF(file);
 return result;
}


DEE_A_RET_OBJECT(DeeAnyStringObject) *
DeeFileIO_File(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 DeeFile_ACQUIRE(self);
 Dee_INCREF(result = DeeFileIO_FILE(self));
 DeeFile_RELEASE(self);
 return result;
}
DEE_A_RET_WUNUSED Dee_openmode_t DeeFileIO_OpenMode(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
}
DEE_A_RET_OBJECT(DeeUtf8StringObject) *
DeeFileIO_Mode(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 return DeeFile_ModeToString(DeeFileIO_MODE(self));
}
DEE_STATIC_INLINE(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) _DeeFileIO_Utf8ReOpenObjectEx(
 DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions);
DEE_STATIC_INLINE(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) _DeeFileIO_WideReOpenObjectEx(
 DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_OBJECT(DeeWideStringObject) const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions);
DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int DeeFileIO_Utf8ReOpenEx(
 DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_Z Dee_Utf8Char const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 DeeObject *file_ob; int temp;
 if DEE_UNLIKELY((file_ob = DeeFS_Utf8PathExpand(file)) == NULL) return -1;
 temp = _DeeFileIO_Utf8ReOpenObjectEx(self,file_ob,mode,permissions);
 Dee_DECREF(file_ob);
 return temp;
}
DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int DeeFileIO_WideReOpenEx(
 DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_Z Dee_WideChar const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 DeeObject *file_ob; int temp;
 if DEE_UNLIKELY((file_ob = DeeFS_WidePathExpand(file)) == NULL) return -1;
 temp = _DeeFileIO_WideReOpenObjectEx(self,file_ob,mode,permissions);
 Dee_DECREF(file_ob);
 return temp;
}

DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int _DeeFileIO_Utf8ReOpenEx(
 DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_Z Dee_Utf8Char const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 DeeObject *file_ob; int temp;
 if DEE_UNLIKELY((file_ob = DeeUtf8String_New(file)) == NULL) return -1;
 temp = _DeeFileIO_Utf8ReOpenObjectEx(self,file_ob,mode,permissions);
 Dee_DECREF(file_ob);
 return temp;
}
DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int _DeeFileIO_WideReOpenEx(
 DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_Z Dee_WideChar const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 DeeObject *file_ob; int temp;
 if DEE_UNLIKELY((file_ob = DeeWideString_New(file)) == NULL) return -1;
 temp = _DeeFileIO_WideReOpenObjectEx(self,file_ob,mode,permissions);
 Dee_DECREF(file_ob);
 return temp;
}
DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int DeeFileIO_ReOpenObjectEx(
 DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_OBJECT(DeeAnyStringObject) const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 DEE_ASSERT(DeeObject_Check(file) && DeeString_Check(file));
 if DEE_UNLIKELY((file = DeeFS_PathExpandObject(file)) == NULL) return -1;
 result = _DeeFileIO_ReOpenObjectEx(self,file,mode,permissions);
 Dee_DECREF(file);
 return result;
}

static int _deefileio_tp_move_assign(DeeFileIOObject *self, DeeFileIOObject *right);
DEE_STATIC_INLINE(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) _DeeFileIO_Utf8ReOpenObjectEx(
 DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 DEE_ASSERT(DeeObject_Check(file) && DeeUtf8String_Check(file));
 {DeeFileIOObject new_file = {DEE_FILE_OBJECT_HEAD_INIT(&DeeFileIO_Type)};
  if DEE_UNLIKELY(_DeeFileIO_Utf8InitObjectEx(&new_file,file,mode,permissions) != 0) return -1;
  _deefileio_tp_move_assign((DeeFileIOObject *)self,&new_file);
  Dee_DECREF(new_file.fio_utf8file);
 }
 return 0;
}
DEE_STATIC_INLINE(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) _DeeFileIO_WideReOpenObjectEx(
 DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_OBJECT(DeeWideStringObject) const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 DEE_ASSERT(DeeObject_Check(file) && DeeWideString_Check(file));
 {DeeFileIOObject new_file = {DEE_FILE_OBJECT_HEAD_INIT(&DeeFileIO_Type)};
  if DEE_UNLIKELY(_DeeFileIO_WideInitObjectEx(&new_file,file,mode,permissions) != 0) return -1;
  _deefileio_tp_move_assign((DeeFileIOObject *)self,&new_file);
  Dee_DECREF(new_file.fio_utf8file);
 }
 return 0;
}
DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int _DeeFileIO_ReOpenObjectEx(
 DEE_A_INOUT_OBJECT(DeeFileIOObject) *self, DEE_A_IN_OBJECT(DeeAnyStringObject) const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 if DEE_UNLIKELY(DeeWideString_Check(file)) return _DeeFileIO_WideReOpenObjectEx(self,file,mode,permissions);
 if DEE_UNLIKELY((file = DeeUtf8String_Cast(file)) == NULL) return -1;
 result = _DeeFileIO_Utf8ReOpenObjectEx(self,file,mode,permissions);
 Dee_DECREF(file);
 return result;
}


#ifdef DEE_PLATFORM_WINDOWS
#ifndef SIZEOF_DWORD
#define SIZEOF_DWORD 4
#endif
#ifndef DWORD_MAX
#define DWORD_MAX 0xFFFFFFFF
#endif
#endif

static int DEE_CALL _deefileio_tp_io_read(DeeFileIOObject *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFile_ACQUIRE(self);
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if DEE_UNLIKELY((self->fo_flags&DEE_PRIVATE_FILEFLAG_IO_VFSFILE)!=0) {
  int result;
  if DEE_UNLIKELY(!self->fio_vfs.fio_type->vfs_read) {
   DeeFile_RELEASE(self);
   DeeError_NotImplemented_str("vfs_read");
   return -1;
  }
  result = (*self->fio_vfs.fio_type->vfs_read)(self,p,s,rs);
  DeeFile_RELEASE(self);
  return result;
 }
#endif
 {
#if defined(DEE_PLATFORM_WINDOWS)
#if DEE_TYPES_SIZEOF_SIZE_T > SIZEOF_DWORD
  DWORD temp; *rs = 0;
  while DEE_LIKELY(s) {
   if DEE_UNLIKELY(!ReadFile((HANDLE)self->fio_handle,p,s >= DWORD_MAX
    ?(DWORD)DWORD_MAX:(DWORD)s,(LPDWORD)&temp,NULL))
#else
   if DEE_UNLIKELY(!ReadFile((HANDLE)self->fio_handle,p,s,(LPDWORD)rs,NULL))
#endif
   {
    DWORD error;
    DeeFile_RELEASE(self);
    error = DeeSystemError_Win32Consume();
    if DEE_UNLIKELY(error == ERROR_BROKEN_PIPE) { *rs = 0; return 0; } // End of file
    DeeError_IOFILESELF_ErrorExplicit("ReadFile",error);
    return -1;
   }
#if DEE_TYPES_SIZEOF_SIZE_T > SIZEOF_DWORD
   if DEE_UNLIKELY(!temp) break; // can't read more
   s -= (Dee_size_t)temp;
   *(char const **)&p += (Dee_size_t)temp;
   *rs += (Dee_size_t)temp;
  }
#endif
#elif defined(DEE_PLATFORM_UNIX)
  Dee_ssize_t result;
  result = (Dee_ssize_t)read(self->fio_handle,p,s);
  if DEE_UNLIKELY(result == -1) {
   int error = errno;
   if DEE_LIKELY(error != 0) {
    errno = 0;
    DeeFile_RELEASE(self);
    DeeError_IOFILESELF_ErrorExplicit("read",error);
    return -1;
   }
  }
  *rs = *(Dee_size_t *)&result;
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
  if DEE_UNLIKELY((*rs = fread(p,sizeof(char),s,(FILE *)self->fio_handle)) != s) {
   int error;
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
   if DEE_LIKELY(ferror((FILE *)self->fio_handle) != 0)
#else /* DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H */
   if DEE_LIKELY((error = ferror((FILE *)self->fio_handle)) != 0)
#endif /* !DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H */
   {
    DeeFile_RELEASE(self);
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
    error = errno;
    errno = 0;
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H */
    DeeError_IOFILESELF_ErrorExplicit("fread",error);
   }
  }
#endif
  DeeFile_RELEASE(self);
  return 0;
 }
}

static int DEE_CALL _deefileio_tp_io_write(DeeFileIOObject *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFile_ACQUIRE(self);
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if DEE_UNLIKELY((self->fo_flags&DEE_PRIVATE_FILEFLAG_IO_VFSFILE)!=0) {
  int result;
  if DEE_UNLIKELY(!self->fio_vfs.fio_type->vfs_write) {
   DeeError_NotImplemented_str("vfs_write");
   DeeFile_RELEASE(self);
   return -1;
  }
  result = (*self->fio_vfs.fio_type->vfs_write)(self,p,s,ws);
  DeeFile_RELEASE(self);
  return result;
 }
#endif
 {
#if defined(DEE_PLATFORM_WINDOWS)
  // Move the file pointer to the end of the file in append mode
  if ((self->fio_mode&DEE_OPENMODE_MASKMODE)==DEE_OPENMODE_APPEND) {
   if DEE_UNLIKELY(SetFilePointer((HANDLE)self->fio_handle,0,NULL,DEE_SEEK_END
    ) == INVALID_SET_FILE_POINTER) SetLastError(0);
  }
#if DEE_TYPES_SIZEOF_SIZE_T > SIZEOF_DWORD
  DWORD temp; *ws = 0;
  while DEE_LIKELY(s) {
   if DEE_UNLIKELY(!WriteFile((HANDLE)self->fio_handle,p,s >= DWORD_MAX
    ?(DWORD)DWORD_MAX:(DWORD)s,(LPDWORD)&temp,NULL))
#else
   if DEE_UNLIKELY(!WriteFile(self->fio_handle,p,s,(LPDWORD)ws,NULL))
#endif
   {
    DeeFile_RELEASE(self);
    DeeError_IOFILESELF_Error("WriteFile");
    return -1;
   }
#if DEE_TYPES_SIZEOF_SIZE_T > SIZEOF_DWORD
   if DEE_UNLIKELY(!temp) break; // can't write more
   s -= (Dee_size_t)temp;
   *(char const **)&p += (Dee_size_t)temp;
   *ws += (Dee_size_t)temp;
  }
#endif
#elif defined(DEE_PLATFORM_UNIX)
  Dee_ssize_t result;
  result = (Dee_ssize_t)write(self->fio_handle,p,s);
  if DEE_UNLIKELY(result == -1) {
   int error = errno;
   if DEE_LIKELY(error != 0) {
    DeeFile_RELEASE(self);
    errno = 0;
    DeeError_IOFILESELF_ErrorExplicit("write",error);
    return -1;
   }
  }
  *ws = *(Dee_size_t *)&result;
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
  if DEE_UNLIKELY((*ws = fwrite(p,sizeof(char),s,(FILE *)self->fio_handle)) != s) {
   int error;
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
   if DEE_LIKELY(ferror((FILE *)self->fio_handle) != 0)
#else /* DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H */
   if DEE_LIKELY((error = ferror((FILE *)self->fio_handle)) != 0)
#endif /* !DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H */
   {
    DeeFile_RELEASE(self);
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
    error = errno;
    errno = 0;
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H */
    DeeError_IOFILESELF_ErrorExplicit("fwrite",error);
   }
  }
#endif
  DeeFile_RELEASE(self);
  return 0;
 }
}
static int DEE_CALL _deefileio_tp_io_seek(DeeFileIOObject *self, Dee_int64_t off, int whence, Dee_uint64_t *pos) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFile_ACQUIRE(self);
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if DEE_UNLIKELY((self->fo_flags&DEE_PRIVATE_FILEFLAG_IO_VFSFILE)!=0) {
  int result;
  if DEE_UNLIKELY(!self->fio_vfs.fio_type->vfs_seek) {
   DeeFile_RELEASE(self);
   DeeError_NotImplemented_str("vfs_seek");
   return -1;
  }
  result = (*self->fio_vfs.fio_type->vfs_seek)(self,off,whence,pos);
  DeeFile_RELEASE(self);
  return result;
 }
#endif
 {
  // Fix the seek offset if it doesn't match the specifications of the OS
  // NOTE: although I never heard of a platform that doesn't use 0-1-2,
  //       it doesn't hurt to be careful with this!
#if defined(DEE_PLATFORM_WINDOWS)
#if FILE_BEGIN != DEE_SEEK_SET
  if (whence == DEE_SEEK_SET) whence = FILE_BEGIN; else
#endif
#if FILE_CURRENT != DEE_SEEK_CUR
  if (whence == DEE_SEEK_CUR) whence = FILE_CURRENT; else
#endif
#if FILE_END != DEE_SEEK_END
  if (whence == DEE_SEEK_END) whence = FILE_END; else
#endif
   ;
#else
#if SEEK_SET != DEE_SEEK_SET
  if (whence == DEE_SEEK_SET) whence = SEEK_SET; else
#endif
#if SEEK_CUR != DEE_SEEK_CUR
  if (whence == DEE_SEEK_CUR) whence = SEEK_CUR; else
#endif
#if SEEK_END != DEE_SEEK_END
  if (whence == DEE_SEEK_END) whence = SEEK_END; else
#endif
   ;
#endif
#if defined(DEE_PLATFORM_WINDOWS)
  if DEE_UNLIKELY((((Dee_uint32_t *)&off)[0] = SetFilePointer((HANDLE)self->fio_handle,*(LONG *)&off,
   (LONG *)(((Dee_uint32_t *)&off)+1),(DWORD)whence)) == INVALID_SET_FILE_POINTER) {
   DWORD err = GetLastError(); SetLastError(0);
   switch (err) {
    case 0: break;

    case ERROR_INVALID_FUNCTION:
    case ERROR_SEEK_ON_DEVICE:
     DeeFile_RELEASE(self);
     DeeError_NotImplemented_str("seek");
     return -1;

    case ERROR_INVALID_HANDLE:
     if DEE_UNLIKELY(self->fio_handle == DEE_FILEIO_INVALID_HANDLE) {
      // This can happen if we tried to open a directory
      if (pos) *pos = 0;
      DeeFile_RELEASE(self);
      return 0;
     }
     DEE_ATTRIBUTE_FALLTHROUGH

    default:
     DeeFile_RELEASE(self);
     DeeError_IOFILESELF_ErrorExplicit("SetFilePointer",err);
     return -1;
   }
  }
  // Set the new file position
  if (pos) *pos = *(Dee_uint64_t *)&off;
#elif defined(DEE_PLATFORM_UNIX)
  Dee_int64_t new_pos; // TODO: _llseek ?
#if DEE_HAVE_LSEEK64
  new_pos = (Dee_int64_t)lseek64(self->fio_handle,off,whence);
#else
  new_pos = (Dee_int64_t)lseek(self->fio_handle,off,whence);
#endif
  if DEE_UNLIKELY(new_pos == -1) {
   int error = errno;
   if DEE_LIKELY(error != 0) {
    DeeFile_RELEASE(self);
    errno = 0;
#if DEE_HAVE_LSEEK64
    DeeError_IOFILESELF_ErrorExplicit("lseek64",error);
#else
    DeeError_IOFILESELF_ErrorExplicit("lseek",error);
#endif
    return -1;
   }
  }
  if (pos) *pos = *(Dee_uint64_t *)&new_pos;
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
#if DEE_HAVE_FSEEK64 && DEE_HAVE_FTELL64
#define DEE_NATIVE_FSEEK fseek64
#define DEE_NATIVE_FTELL ftell64
#define DEE_NATIVE_FSEEK_NAME "fseek64"
#define DEE_NATIVE_FTELL_NAME "ftell64"
#elif DEE_HAVE_FSEEKO && DEE_HAVE_FTELLO
#define DEE_NATIVE_FSEEK fseeko
#define DEE_NATIVE_FTELL ftello
#define DEE_NATIVE_FSEEK_NAME "fseeko"
#define DEE_NATIVE_FTELL_NAME "ftello"
#else
#define DEE_NATIVE_FSEEK(fp,off,whence) fseek(fp,(long)(off),whence)
#define DEE_NATIVE_FTELL                (Dee_int64_t)ftell
#define DEE_NATIVE_FSEEK_NAME "fseek"
#define DEE_NATIVE_FTELL_NAME "ftell"
#endif
  if DEE_UNLIKELY(DEE_NATIVE_FSEEK((FILE *)self->fio_handle,off,whence) != 0) {
   DeeFile_RELEASE(self);
   DeeError_IOFILESELF_Error(DEE_NATIVE_FSEEK_NAME);
   return -1;
  }
  if (pos) {
   Dee_int64_t new_pos;
   new_pos = DEE_NATIVE_FTELL((FILE *)self->fio_handle);
   if (DEE_UNLIKELY(new_pos == -1) && ferror((FILE *)self->fio_handle) != 0) {
    DeeFile_RELEASE(self);
    DeeError_IOFILESELF_Error(DEE_NATIVE_FTELL_NAME);
    return -1;
   }
   *pos = *(Dee_uint64_t *)&new_pos;
  }
#undef DEE_NATIVE_FTELL_NAME
#undef DEE_NATIVE_FSEEK_NAME
#undef DEE_NATIVE_FTELL
#undef DEE_NATIVE_FSEEK
#endif
  DeeFile_RELEASE(self);
  return 0;
 }
}
static int DEE_CALL _deefileio_tp_io_flush(DeeFileIOObject *self) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFile_ACQUIRE(self);
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if DEE_UNLIKELY((self->fo_flags&DEE_PRIVATE_FILEFLAG_IO_VFSFILE)!=0) {
  int result;
  if DEE_UNLIKELY(!self->fio_vfs.fio_type->vfs_flush) {
   DeeFile_RELEASE(self);
   DeeError_NotImplemented_str("vfs_flush");
   return -1;
  }
  result = (*self->fio_vfs.fio_type->vfs_flush)(self);
  DeeFile_RELEASE(self);
  return result;
 }
#endif
 {
#if defined(DEE_PLATFORM_WINDOWS)
  if DEE_UNLIKELY(!FlushFileBuffers((HANDLE)self->fio_handle)) {
   DeeFile_RELEASE(self);
   DeeError_IOFILESELF_Error("FlushFileBuffers");
   return -1;
  }
#elif defined(DEE_PLATFORM_UNIX)
#if DEE_HAVE_FSYNC || DEE_HAVE_FDATASYNC
  if DEE_UNLIKELY(
#if DEE_HAVE_FSYNC
   fsync(self->fio_handle) != 0
#elif DEE_HAVE_FDATASYNC
   fdatasync(self->fio_handle) != 0
#endif
   ) {
   DeeFile_RELEASE(self);
#if DEE_HAVE_FSYNC
   DeeError_IOFILESELF_Error("fsync");
#elif DEE_HAVE_FDATASYNC
   DeeError_IOFILESELF_Error("fdatasync");
#endif
   return -1;
  }
#else
  (void)self;
#endif
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
  if DEE_UNLIKELY(fflush((FILE *)self->fio_handle) != 0) {
   DeeFile_RELEASE(self);
   DeeError_IOFILESELF_Error("fflush");
   return -1;
  }
#endif
  DeeFile_RELEASE(self);
  return 0;
 }
}
static int DEE_CALL _deefileio_tp_io_trunc(DeeFileIOObject *self) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFile_ACQUIRE(self);
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if DEE_UNLIKELY((self->fo_flags&DEE_PRIVATE_FILEFLAG_IO_VFSFILE)!=0) {
  int result;
  if DEE_UNLIKELY(!self->fio_vfs.fio_type->vfs_trunc) {
   DeeFile_RELEASE(self);
   DeeError_NotImplemented_str("vfs_trunc");
   return -1;
  }
  result = (*self->fio_vfs.fio_type->vfs_trunc)(self);
  DeeFile_RELEASE(self);
  return result;
 }
#endif
 {
#if defined(DEE_PLATFORM_WINDOWS)
  if DEE_UNLIKELY(!SetEndOfFile((HANDLE)self->fio_handle)) {
   DeeFile_RELEASE(self);
   DeeError_IOFILESELF_Error("SetEndOfFile");
   return -1;
  }
#if 0 /*< Not a good idea... (data leakage and such) */
  {
   Dee_uint64_t file_size = 0;
   ((Dee_uint32_t *)&file_size)[0] = SetFilePointer(
    (HANDLE)self->fio_handle,*(LONG *)&file_size,
    (LONG *)(((Dee_uint32_t *)&file_size)+1),FILE_CURRENT);
   if DEE_UNLIKELY(!SetFileValidData((HANDLE)self->fio_handle,1ll*1024*1024)) {
    DeeFile_RELEASE(self);
    DeeError_IOFILESELF_Error("SetFileValidData");
    return -1;
   }
  }
#endif
#elif defined(DEE_PLATFORM_UNIX)
#if DEE_HAVE_FTRUNCATE
  Dee_uint64_t trunc_size; // TODO: _llseek ?
#if DEE_HAVE_LSEEK64
  trunc_size = (Dee_uint64_t)lseek64(self->fio_handle,0,SEEK_CUR);
#else
  trunc_size = (Dee_uint64_t)lseek(self->fio_handle,0,SEEK_CUR);
#endif
  if DEE_UNLIKELY(trunc_size == (Dee_uint64_t)-1) {
   int error = errno;
   if DEE_LIKELY(error != 0) {
    DeeFile_RELEASE(self);
    errno = 0;
#if DEE_HAVE_LSEEK64
    DeeError_IOFILESELF_ErrorExplicit("lseek64",error);
#else
    DeeError_IOFILESELF_ErrorExplicit("lseek",error);
#endif
    return -1;
   }
  }
  if DEE_UNLIKELY(ftruncate(self->fio_handle,(off_t)trunc_size) == -1) {
   DeeFile_RELEASE(self);
   DeeError_IOFILESELF_Error("ftruncate");
   return -1;
  }
#else
  (void)self;
#endif
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
  // If we can't use 'SetEndOfFile' and can't use 'ftruncate',
  // the only way we could do this would be by reading all
  // data from the file, closing it, then reopening it
  // and thereby clearing it, before writing all the old data.
  // But that is unpractical because:
  //  - Someone else might open the file before we get a chance to write the data back into it
  //  - The data might be really big, possibly more data than SIZE_MAX (so that would break)
  (void)self;
#endif
 }
 DeeFile_RELEASE(self);
 return 0;
}
DEE_STATIC_INLINE(void) _DeeFileIO_CloseAlreadyLocked(DeeFileIOObject *self) {
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if DEE_UNLIKELY((self->fo_flags&DEE_PRIVATE_FILEFLAG_IO_VFSFILE)!=0) {
  if DEE_UNLIKELY(self->fio_vfs.fio_type->vfs_close)
   (*self->fio_vfs.fio_type->vfs_close)(self);
 } else
#endif
 {
  if DEE_UNLIKELY(self->fio_handle != DEE_FILEIO_INVALID_HANDLE) {
#if defined(DEE_PLATFORM_WINDOWS)
   if DEE_UNLIKELY(!CloseHandle((HANDLE)self->fio_handle)) SetLastError(0);
#elif defined(DEE_PLATFORM_UNIX)
   if DEE_UNLIKELY(close(self->fio_handle) == -1) errno = 0;
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
   fclose((FILE *)self->fio_handle);
#endif
   self->fio_handle = DEE_FILEIO_INVALID_HANDLE;
   if ((self->fo_flags&DEE_PRIVATE_FILEFLAG_IO_DELONCLOSE)!=0) {
    // Try to delete the file
    if DEE_UNLIKELY(_DeeFS_RmFileObject(DeeFileIO_FILE(self)) != 0) DeeError_Handled();
    self->fo_flags &= ~DEE_PRIVATE_FILEFLAG_IO_DELONCLOSE;
   }
   Dee_DECREF(self->fio_utf8file);
   Dee_INCREF(self->fio_utf8file = (DeeUtf8StringObject *)Dee_EmptyUtf8String);
  }
 }
}
static void DEE_CALL _deefileio_tp_io_close(DeeFileIOObject *self) {
 DeeFile_ACQUIRE(self);
 _DeeFileIO_CloseAlreadyLocked(self);
 DeeFile_RELEASE(self);
}


static DeeObject *_deefileio_fileno(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":fileno") != 0) return NULL;
#if defined(DEE_PLATFORM_WINDOWS)
 return DeeObject_New(Dee_uintptr_t,(Dee_uintptr_t)DeeFileIO_HANDLE(self));
#elif defined(DEE_PLATFORM_UNIX)
 return DeeObject_New(int,DeeFileIO_HANDLE(self));
#endif
}

DeeString_NEW_STATIC_EX(_deefileio_default_mode,1,{'r'});
static DeeObject *_deefileio_reopen(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeAnyStringObject *filename; Dee_mode_t perms = DEE_FILEIO_DEFAULT_PERMISSIONS; int result;
 DeeStringObject *mode = (DeeStringObject *)&_deefileio_default_mode;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|oI" DEE_PP_STR(DEE_PP_MUL8(
  DEE_TYPES_SIZEOF_MODE_T)) "u:reopen",&filename,&mode,&perms) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)mode,&DeeString_Type) != 0) return NULL;
 result = DeeFileIO_ReOpenObjectEx((DeeObject *)self,(DeeObject *)filename,
                                   DeeString_STR(mode),perms);
 Dee_DECREF(filename);
 if (result != 0) return NULL;
 DeeReturn_None;
}
#ifdef DeeSysFileFD_GetSize
#define _deefileio_size _deefileio_size
static DeeObject *_deefileio_size(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint64_t fp_size;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":size") != 0) return NULL;
 DeeSysFileFD_GetSize(DeeFileIO_FD());
 if DEE_UNLIKELY(DeeFileIO_Size((DeeObject *)self,&fp_size) != 0) return NULL;
 return DeeUInt64_New(fp_size);
}
#endif /* DeeSysFileFD_GetSize */
static DeeObject *_deefileio_gettimes(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":gettimes") != 0) return NULL;
 return DeeFileIO_GetTimes((DeeObject *)self);
}
static DeeObject *_deefileio_getatime(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":getatime") != 0) return NULL;
 return DeeFileIO_GetATime((DeeObject *)self);
}
static DeeObject *_deefileio_getctime(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":getctime") != 0) return NULL;
 return DeeFileIO_GetCTime((DeeObject *)self);
}
static DeeObject *_deefileio_getmtime(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":getmtime") != 0) return NULL;
 return DeeFileIO_GetMTime((DeeObject *)self);
}
static DeeObject *_deefileio_settimes(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *tm_access = NULL,*tm_creation = NULL,*tm_modification = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|ooo:settimes",&tm_access,&tm_creation,&tm_modification) != 0) return NULL;
 if DEE_UNLIKELY(DeeFileIO_SetTimes2((DeeObject *)self,tm_access,tm_creation,tm_modification) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deefileio_setatime(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *tm_access;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:setatime",&tm_access) != 0) return NULL;
 if DEE_UNLIKELY(DeeFileIO_SetATime((DeeObject *)self,tm_access) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deefileio_setctime(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *tm_creation;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:setctime",&tm_creation) != 0) return NULL;
 if DEE_UNLIKELY(DeeFileIO_SetCTime((DeeObject *)self,tm_creation) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deefileio_setmtime(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *tm_modification;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:setmtime",&tm_modification) != 0) return NULL;
 if DEE_UNLIKELY(DeeFileIO_SetMTime((DeeObject *)self,tm_modification) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deefileio_isfile(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":isfile") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsFile((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *_deefileio_isdir(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":isdir") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsDir((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *_deefileio_islink(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":islink") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsLink((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *_deefileio_isdrive(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":isdrive") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsDrive((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *_deefileio_ismount(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":ismount") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsMount((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *_deefileio_ishidden(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":ishidden") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsHidden((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *_deefileio_isxfile(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":isxfile") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsExecutable((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *_deefileio_ischrdev(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":ischrdev") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsCharDev((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *_deefileio_isblkdev(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":isblkdev") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsBlockDev((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *_deefileio_isfifo(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":isfifo") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsFiFo((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *_deefileio_issocket(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":issocket") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsSocket((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}

static DeeObject *_deefileio_dir(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":dir") != 0) return NULL;
 return DeeFileIO_ListDir((DeeObject *)self);
}
static DeeObject *_deefileio_chmod(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *mode;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:chmod",&mode) != 0) return NULL;
 if (DeeString_Check(mode)) {
  if DEE_UNLIKELY(DeeFileIO_ChmodString((DeeObject *)self,DeeString_STR(mode)) != 0) return NULL;
 } else {
  Dee_mode_t modeid;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_mode_t,mode,&modeid) != 0) return NULL;
  if DEE_UNLIKELY(DeeFileIO_Chmod((DeeObject *)self,modeid) != 0) return NULL;
 }
 DeeReturn_None;
}
static DeeObject *_deefileio_getmod(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_mode_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":getmod") != 0) return NULL;
 if DEE_UNLIKELY(DeeFileIO_GetMod((DeeObject *)self,&result) != 0) return NULL;
 return DeeObject_New(Dee_mode_t,result);
}
static DeeObject *_deefileio_chown(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *arg1,*arg2 = NULL; Dee_uid_t uid; Dee_gid_t gid;
#ifdef DEE_PLATFORM_WINDOWS
 int error; DeeObject *buf1,*buf2;
#endif
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:chown",&arg1,&arg2) != 0) return NULL;
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(_DeeFS_AutoGetUserAndGroup(arg1,arg2,&uid,&gid,&buf1,&buf2) != 0) return NULL;
 error = DeeFileIO_Chown((DeeObject *)self,uid,gid);
 Dee_XDECREF(buf1);
 Dee_XDECREF(buf2);
 if DEE_UNLIKELY(error != 0) return NULL;
#else
 if DEE_UNLIKELY(_DeeFS_AutoGetUserAndGroup(arg1,arg2,&uid,&gid) != 0) return NULL;
 if DEE_UNLIKELY(DeeFileIO_Chown((DeeObject *)self,uid,gid) != 0) return NULL;
#endif
 DeeReturn_None;
}
static DeeObject *_deefileio_getown(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result; Dee_uid_t uid; Dee_gid_t gid;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":getown") != 0) return NULL;
 if DEE_UNLIKELY(DeeFileIO_GetOwn((DeeObject *)self,&uid,&gid) != 0) return NULL;
 if DEE_UNLIKELY((result = _DeeTuple_NewUnsafe(2)) == NULL) return NULL;
 if DEE_UNLIKELY((DeeTuple_ELEM(result)[0] = DeeObject_New(Dee_uid_t,uid)) == NULL) {
err_0: _DeeTuple_FreeUnsafe(result); return NULL;
 }
 if DEE_UNLIKELY((DeeTuple_ELEM(result)[1] = DeeObject_New(Dee_gid_t,gid)) == NULL) {
  Dee_DECREF(DeeTuple_ELEM(result)[0]); goto err_0;
 }
 return result;
}
static DeeObject *_deefileioclass_tmp(
 DeeFileTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 int delete_when_closed = 1;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|b:tmp",&delete_when_closed) != 0) return NULL;
 return DeeFile_OpenTemporary(DEE_LIKELY(delete_when_closed)
  ? DEE_FILE_OPENTEMPORARY_FLAG_DELETE_WHEN_CLOSED
  : DEE_FILE_OPENTEMPORARY_FLAG_NONE);
}

static DeeObject *_deefileio_mode(DeeObject *self, void *DEE_UNUSED(closure)) { DeeReturn_NEWREF(DeeFile_ModeToString(DeeFileIO_MODE(self))); }
#define DEE_FILE_IO_USER_OVERRIDE_STD_FILES 1
static DeeObject *_deefileioclass_stdin_get(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { return DeeFile_Std(DEE_STDIN); }
static DeeObject *_deefileioclass_stdout_get(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { return DeeFile_Std(DEE_STDOUT); }
static DeeObject *_deefileioclass_stderr_get(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { return DeeFile_Std(DEE_STDERR); }
static DeeObject *_deefileioclass_stdnull_get(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { return DeeFile_Std(DEE_STDNULL); }
static DeeObject *_deefileioclass_stddbg_get(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { return DeeFile_Std(DEE_STDDBG); }
#if DEE_FILE_IO_USER_OVERRIDE_STD_FILES
static int _deefileioclass_stdin_del(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { DeeFile_DelStd(DEE_STDIN); return 0; }
static int _deefileioclass_stdout_del(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { DeeFile_DelStd(DEE_STDOUT); return 0; }
static int _deefileioclass_stderr_del(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { DeeFile_DelStd(DEE_STDERR); return 0; }
static int _deefileioclass_stdnull_del(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { DeeFile_DelStd(DEE_STDNULL); return 0; }
static int _deefileioclass_stddbg_del(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { DeeFile_DelStd(DEE_STDDBG); return 0; }
static int _deefileioclass_stdin_set(DeeObject *DEE_UNUSED(self), DeeObject *v, void *DEE_UNUSED(closure)) { if DEE_UNLIKELY((v = DeeObject_GetInstance(v,(DeeTypeObject *)&DeeFile_Type)) == NULL) return -1; DeeFile_SetStd(DEE_STDIN,v); return 0; }
static int _deefileioclass_stdout_set(DeeObject *DEE_UNUSED(self), DeeObject *v, void *DEE_UNUSED(closure)) { if DEE_UNLIKELY((v = DeeObject_GetInstance(v,(DeeTypeObject *)&DeeFile_Type)) == NULL) return -1; DeeFile_SetStd(DEE_STDOUT,v); return 0; }
static int _deefileioclass_stderr_set(DeeObject *DEE_UNUSED(self), DeeObject *v, void *DEE_UNUSED(closure)) { if DEE_UNLIKELY((v = DeeObject_GetInstance(v,(DeeTypeObject *)&DeeFile_Type)) == NULL) return -1; DeeFile_SetStd(DEE_STDERR,v); return 0; }
static int _deefileioclass_stdnull_set(DeeObject *DEE_UNUSED(self), DeeObject *v, void *DEE_UNUSED(closure)) { if DEE_UNLIKELY((v = DeeObject_GetInstance(v,(DeeTypeObject *)&DeeFile_Type)) == NULL) return -1; DeeFile_SetStd(DEE_STDNULL,v); return 0; }
static int _deefileioclass_stddbg_set(DeeObject *DEE_UNUSED(self), DeeObject *v, void *DEE_UNUSED(closure)) { if DEE_UNLIKELY((v = DeeObject_GetInstance(v,(DeeTypeObject *)&DeeFile_Type)) == NULL) return -1; DeeFile_SetStd(DEE_STDDBG,v); return 0; }
#endif
static DeeObject *_deefileio_times_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeFileIO_GetTimes(self); }
static DeeObject *_deefileio_atime_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeFileIO_GetATime(self); }
static DeeObject *_deefileio_ctime_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeFileIO_GetCTime(self); }
static DeeObject *_deefileio_mtime_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeFileIO_GetMTime(self); }
static int _deefileio_times_set(DeeObject *self, DeeObject *ob, void *DEE_UNUSED(closure)) {
 DeeObject *tm_access,*tm_creation,*tm_modification;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(ob,&DeeTuple_Type) != 0) return -1;
 if DEE_UNLIKELY(DeeTuple_SIZE(ob) != 3) { DeeError_TypeError_TupleSizeExpected(3,DeeTuple_SIZE(ob)); return -1; }
 if DEE_UNLIKELY((tm_access = DeeObject_GetInstance(DeeTuple_GET(ob,0),&DeeTime_Type)) == NULL) return -1;
 if DEE_UNLIKELY((tm_creation = DeeObject_GetInstance(DeeTuple_GET(ob,1),&DeeTime_Type)) == NULL) return -1;
 if DEE_UNLIKELY((tm_modification = DeeObject_GetInstance(DeeTuple_GET(ob,2),&DeeTime_Type)) == NULL) return -1;
 return DeeFileIO_SetTimes2(self,tm_access,tm_creation,tm_modification);
}
static int _deefileio_atime_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY((v = DeeObject_GetInstance(v,&DeeTime_Type)) == NULL) return -1;
 return DeeFileIO_SetATime(self,v);
}
static int _deefileio_ctime_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY((v = DeeObject_GetInstance(v,&DeeTime_Type)) == NULL) return -1;
 return DeeFileIO_SetCTime(self,v);
}
static int _deefileio_mtime_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY((v = DeeObject_GetInstance(v,&DeeTime_Type)) == NULL) return -1;
 return DeeFileIO_SetMTime(self,v);
}



//////////////////////////////////////////////////////////////////////////
// Virtual functions for fileio file objects
static int _deefileio_tp_ctor(DeeTypeObject *DEE_UNUSED(tp_self), DeeFileIOObject *self) {
 _DeeFile_Init(self);
 self->fio_handle = DEE_FILEIO_INVALID_HANDLE;
 Dee_INCREF(self->fio_utf8file = (DeeUtf8StringObject *)Dee_EmptyUtf8String);
 self->fio_mode = DEE_OPENMODE_READ;
 return 0;
}
static DeeObject *_deefileio_tp_str(DeeFileIOObject *self) {
 Dee_uint64_t pos,size;
 if DEE_UNLIKELY(DeeFile_Tell((DeeObject *)self,&pos) != 0 ||
                 DeeFile_Seek((DeeObject *)self,0,DEE_SEEK_END,&size) != 0 ||
                 DeeFile_Seek((DeeObject *)self,*(Dee_int64_t*)&pos,DEE_SEEK_SET,NULL) != 0) {
  if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return NULL;
  if (DeeFileIO_Size((DeeObject *)self,&size) != 0) {
   if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return NULL;
   return DeeString_Newf("<file.io: file %r, mode %q, pos ?/?>",
                         DeeFileIO_FILE(self),DeeString_STR(DeeFile_ModeToString(self->fio_mode)));
  } else {
   // Couldn't deduce position, but did deduce size!
   return DeeString_Newf("<file.io: file %r, mode %q, pos ?/%I64u>",
                         DeeFileIO_FILE(self),
                         DeeString_STR(DeeFile_ModeToString(self->fio_mode)),size);
  }
 } else {
  return DeeString_Newf("<file.io: file %r, mode %q, pos %I64u/%I64u>",
                        DeeFileIO_FILE(self),DeeString_STR(DeeFile_ModeToString(self->fio_mode)),
                        pos,size);
 }
}
DEE_VISIT_PROC(_deefileio_tp_visit,DeeFileIOObject *self) {
 Dee_VISIT(DeeFileIO_FILE(self));
}
static DeeObject *_deefileio_tp_not(DeeFileIOObject *self) {
 int result;
 DeeFile_ACQUIRE(self);
 result = (self->fio_handle == DEE_FILEIO_INVALID_HANDLE);
 DeeFile_RELEASE(self);
 DeeReturn_Bool(result);
}
static int _deefileio_tp_bool(DeeFileIOObject *self) {
 int result;
 DeeFile_ACQUIRE(self);
 result = (self->fio_handle != DEE_FILEIO_INVALID_HANDLE);
 DeeFile_RELEASE(self);
 return result;
}
static int _deefileio_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileIOObject *self, DeeObject *args) {
 DeeAnyStringObject *filename; Dee_mode_t perms = DEE_FILEIO_DEFAULT_PERMISSIONS; int result;
 DeeStringObject *mode = (DeeStringObject *)&_deefileio_default_mode;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|oI" DEE_PP_STR(DEE_PP_MUL8(
  DEE_TYPES_SIZEOF_MODE_T)) "u:file.io",&filename,&mode,&perms) != 0) return -1;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)mode,&DeeString_Type) != 0) return -1;
 if DEE_UNLIKELY(DeeWideString_Check(filename)) Dee_INCREF(filename);
 else if DEE_UNLIKELY((filename = (DeeAnyStringObject *)DeeUtf8String_Cast(
  DeeFileIO_Check(filename) ? DeeFileIO_FILE(filename) : (DeeObject *)filename)
  ) == NULL) return -1;
 result = DEE_UNLIKELY(DeeWideString_Check(filename))
  ? DeeFileIO_WideInitObjectEx(self,(DeeObject *)filename,DeeString_STR(mode),perms)
  : DeeFileIO_Utf8InitObjectEx(self,(DeeObject *)filename,DeeString_STR(mode),perms);
 Dee_DECREF(filename);
 return result;
}
static void _deefileio_tp_dtor(DeeFileIOObject *self) {
 _DeeFileIO_CloseAlreadyLocked(self);
 Dee_DECREF(self->fio_utf8file);
}
static int _deefileio_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileIOObject *self, DeeFileIOObject *right) {
 struct DeeFileIOObject new_file;
 DeeFile_ACQUIRE(right);
 new_file = *right;
 right->fio_mode = DEE_OPENMODE_READ;
 right->fio_handle = DEE_FILEIO_INVALID_HANDLE;
 Dee_INCREF(right->fio_utf8file = (DeeUtf8StringObject *)Dee_EmptyUtf8String);
 right->fo_flags = DEE_PRIVATE_FILEFLAG_NONE;
 DeeFile_RELEASE(right);
 DeeAtomicMutex_Init(DeeFile_LOCK(self));
 memcpy(&self->fo_flags,&new_file.fo_flags,
        sizeof(DeeFileIOObject)-Dee_OFFSETOF(DeeFileIOObject,fo_flags));
 return 0;
}
static int _deefileio_tp_move_assign(
 DeeFileIOObject *self, DeeFileIOObject *right) {
 if DEE_LIKELY(self != right) {
  struct DeeFileIOObject new_file;
  DeeUtf8StringObject *old_filename;
  DeeFile_ACQUIRE(right);
  new_file = *right;
  right->fio_mode = DEE_OPENMODE_READ;
  right->fio_handle = DEE_FILEIO_INVALID_HANDLE;
  Dee_INCREF(right->fio_utf8file = (DeeUtf8StringObject *)Dee_EmptyUtf8String);
  right->fo_flags = DEE_PRIVATE_FILEFLAG_NONE;
  DeeFile_RELEASE(right);
  DeeFile_ACQUIRE(self);
  _DeeFileIO_CloseAlreadyLocked(self);
  old_filename = self->fio_utf8file;
  memcpy(&self->fo_flags,&new_file.fo_flags,
         sizeof(DeeFileIOObject)-Dee_OFFSETOF(DeeFileIOObject,fo_flags));
  DeeFile_RELEASE(self);
  Dee_DECREF(old_filename);
 }
 return 0;
}



static struct DeeMemberDef const _deefileio_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("file",DeeFileIOObject,fio_utf8file,object),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__fio_utf8file",DeeFileIOObject,fio_utf8file,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__fio_widefile",DeeFileIOObject,fio_widefile,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__fio_mode",DeeFileIOObject,fio_mode,Dee_uint16_t),
#if defined(DEE_PLATFORM_WINDOWS)
 DEE_MEMBERDEF_NAMED_RO_v100("__fio_handle",DeeFileIOObject,fio_handle,p(void)),
#elif defined(DEE_PLATFORM_UNIX)
 DEE_MEMBERDEF_NAMED_RO_v100("__fio_handle",DeeFileIOObject,fio_handle,int),
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
 DEE_MEMBERDEF_NAMED_RO_v100("__fio_handle",DeeFileIOObject,fio_handle,p(void)),
#endif
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 DEE_MEMBERDEF_NAMED_RO_v100("__fio_vfs_fio_type",DeeFileIOObject,fio_vfs.fio_type,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__fio_vfs_fio_data",DeeFileIOObject,fio_vfs.fio_data,p(void)),
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};

static struct DeeGetSetDef const _deefileio_tp_getsets[] = {
 DEE_GETSETDEF_CONST_v100("mode",member(&_deefileio_mode),null,null,"-> string\n\tReturns the mode used to create the file"),
 DEE_GETSETDEF_CONST_v100("times",member(&_deefileio_times_get),null,member(&_deefileio_times_set),"-> (time,time,time)\n"),
 DEE_GETSETDEF_CONST_v100("atime",member(&_deefileio_atime_get),null,member(&_deefileio_atime_set),"-> time\n"),
 DEE_GETSETDEF_CONST_v100("ctime",member(&_deefileio_ctime_get),null,member(&_deefileio_ctime_set),"-> time\n"),
 DEE_GETSETDEF_CONST_v100("mtime",member(&_deefileio_mtime_get),null,member(&_deefileio_mtime_set),"-> time\n"),
 DEE_GETSETDEF_END_v100
};

static struct DeeMethodDef const _deefileio_tp_methods[] = {
 DEE_METHODDEF_v100("reopen",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_reopen),
  "(string filename, string mode = \"r\", mode_t permissions = " DEE_PP_STR(DEE_FILEIO_DEFAULT_PERMISSIONS) ") -> none"),
 DEE_METHODDEF_v100("size",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_size),"() -> uint64_t"),
#if DEE_PLATFORM_HAVE_IO_HANDLE
#ifdef DEE_PLATFORM_WINDOWS
 DEE_METHODDEF_v100("fileno",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_fileno),"() -> uintptr_t"),
#elif defined(DEE_PLATFORM_UNIX)
 DEE_METHODDEF_v100("fileno",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_fileno),"() -> int"),
#endif
#endif /* DEE_PLATFORM_HAVE_IO_HANDLE */
 DEE_METHODDEF_v100("gettimes",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_gettimes),"() -> (time,time,time)"),
 DEE_METHODDEF_v100("getatime",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_getatime),"() -> time"),
 DEE_METHODDEF_v100("getctime",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_getctime),"() -> time"),
 DEE_METHODDEF_v100("getmtime",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_getmtime),"() -> time"),
 DEE_METHODDEF_v100("settimes",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_settimes),
  "(time atime) -> none\n"
  "(time atime, time ctime) -> none\n"
  "(time atime, time ctime, time mtime) -> none\n"),
 DEE_METHODDEF_v100("setatime",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_setatime),"(time atime) -> none"),
 DEE_METHODDEF_v100("setctime",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_setctime),"(time ctime) -> none"),
 DEE_METHODDEF_v100("setmtime",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_setmtime),"(time mtime) -> none"),
 DEE_METHODDEF_v100("isfile",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_isfile),"() -> bool"),
 DEE_METHODDEF_v100("isdir",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_isdir),"() -> bool"),
 DEE_METHODDEF_v100("islink",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_islink),"() -> bool"),
 DEE_METHODDEF_v100("isdrive",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_isdrive),"() -> bool"),
 DEE_METHODDEF_v100("ismount",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_ismount),"() -> bool"),
 DEE_METHODDEF_v100("ishidden",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_ishidden),"() -> bool"),
 DEE_METHODDEF_v100("isxfile",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_isxfile),"() -> bool"),
 DEE_METHODDEF_v100("ischrdev",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_ischrdev),"() -> bool"),
 DEE_METHODDEF_v100("isblkdev",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_isblkdev),"() -> bool"),
 DEE_METHODDEF_v100("isfifo",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_isfifo),"() -> bool"),
 DEE_METHODDEF_v100("issocket",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_issocket),"() -> bool"),
 DEE_METHODDEF_v100("dir",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_dir),"() -> dir"),
 DEE_METHODDEF_v100("chmod",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_chmod),
  "(string mod) -> none\n"
  "(mode_t mod) -> none"),
 DEE_METHODDEF_v100("chown",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_chown),
  "(string owner_and_group) -> none\n"
  "(string owner, string group) -> none\n"
  "(string owner, gid_t group) -> none\n"
  "(uid_t owner, string group) -> none\n"
  "(uid_t owner, gid_t group) -> none\n"),
 DEE_METHODDEF_v100("getmod",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_getmod),
  "() -> mode_t"),
 DEE_METHODDEF_v100("getown",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileio_getown),
  "() -> (uid_t,gid_t)"),
 DEE_METHODDEF_END_v100
};


static struct DeeMethodDef const _deefileio_tp_class_methods[] = {
 DEE_METHODDEF_v100("tmp",member((DeeObject*(*)(DeeObject*,DeeObject*,void*))&_deefileioclass_tmp),"(bool delete_when_closed = true) -> file.io"),
 DEE_METHODDEF_END_v100
};
static struct DeeGetSetDef const _deefileio_tp_class_getsets[] = {
#if DEE_FILE_IO_USER_OVERRIDE_STD_FILES
 DEE_GETSETDEF_v100("stdin",member(&_deefileioclass_stdin_get),member(&_deefileioclass_stdin_del),member(&_deefileioclass_stdin_set),"-> file.io(\"/dev/stdin\")\n\tThe standard input stream"),
 DEE_GETSETDEF_v100("stdout",member(&_deefileioclass_stdout_get),member(&_deefileioclass_stdout_del),member(&_deefileioclass_stdout_set),"-> file.io(\"/dev/stdout\")\n\tThe standard output stream"),
 DEE_GETSETDEF_v100("stderr",member(&_deefileioclass_stderr_get),member(&_deefileioclass_stderr_del),member(&_deefileioclass_stderr_set),"-> file.io(\"/dev/stderr\")\n\tThe standard error stream"),
 DEE_GETSETDEF_v100("stdnull",member(&_deefileioclass_stdnull_get),member(&_deefileioclass_stdnull_del),member(&_deefileioclass_stdnull_set),"-> file.io(\"/dev/null\")\n\tA stream that swallows everything you throw at it"),
 DEE_GETSETDEF_v100("stddbg",member(&_deefileioclass_stddbg_get),member(&_deefileioclass_stddbg_del),member(&_deefileioclass_stddbg_set),"-> file.io.stddbg\n\tA stream that can be used for debug output"),
#else /* DEE_FILE_IO_USER_OVERRIDE_STD_FILES */
 DEE_GETSETDEF_v100("stdin",member(&_deefileioclass_stdin_get),null,null,"-> file.io(\"/dev/stdin\")\n\tThe standard input stream"),
 DEE_GETSETDEF_v100("stdout",member(&_deefileioclass_stdout_get),null,null,"-> file.io(\"/dev/stdout\")\n\tThe standard output stream"),
 DEE_GETSETDEF_v100("stderr",member(&_deefileioclass_stderr_get),null,null,"-> file.io(\"/dev/stderr\")\n\tThe standard error stream"),
 DEE_GETSETDEF_v100("stdnull",member(&_deefileioclass_stdnull_get),null,null,"-> file.io(\"/dev/null\")\n\tA stream that swallows everything you throw at it"),
 DEE_GETSETDEF_v100("stddbg",member(&_deefileioclass_stddbg_get),null,null,"-> file.io.stddbg\n\tA stream that can be used for debug output"),
#endif /* !DEE_FILE_IO_USER_OVERRIDE_STD_FILES */
 DEE_GETSETDEF_END_v100
};


DeeFileTypeObject DeeFileIO_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("file.io"),member(
   "(string filename, string mode = \"r\", mode_t permissions = " DEE_PP_STR(DEE_FILEIO_DEFAULT_PERMISSIONS) ") -> file.io\n"
   "\tThe file type used to access files in the filesystem of the host system"),
   null,member((DeeTypeObject *)&DeeFile_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFileIOObject),null,
   member(&_deefileio_tp_ctor),null,
   member(&_deefileio_tp_move_ctor),
   member(&_deefileio_tp_any_ctor)),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
   member(&_deefileio_tp_dtor)),
  DEE_TYPE_OBJECT_ASSIGN_v100(null,
   member(&_deefileio_tp_move_assign),null),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&_deefileio_tp_str),
   member(&_deefileio_tp_str),null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,
   member(&_deefileio_tp_visit)),
  DEE_TYPE_OBJECT_MATH_v101(
   member(&_deefileio_tp_not),
   member(&_deefileio_tp_bool),
   null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
   member(_deefileio_tp_members),
   member(_deefileio_tp_getsets),
   member(_deefileio_tp_methods),null,
   member(_deefileio_tp_class_getsets),
   member(_deefileio_tp_class_methods)),
  DEE_TYPE_OBJECT_FOOTER_v100
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(
  member(&_deefileio_tp_io_read),
  member(&_deefileio_tp_io_write),
  member(&_deefileio_tp_io_seek),
  member(&_deefileio_tp_io_flush),
  member(&_deefileio_tp_io_trunc),
  member(&_deefileio_tp_io_close))
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};

#if 0
#ifndef DEE_FS_FILEIO_INITOBJECT_DEFINED
#define DEE_FS_FILEIO_INITOBJECT_DEFINED 1
DEE_A_RET_EXCEPT(-1) int _DeeFileIO_Utf8InitObjectEx(
 DEE_A_IN DeeFileIOObject *self, DEE_A_IN_OBJECT(DeeWideStringObject) const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions);
DEE_A_RET_EXCEPT(-1) int _DeeFileIO_WideInitObjectEx(
 DEE_A_IN DeeFileIOObject *self, DEE_A_IN_OBJECT(DeeWideStringObject) const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions);
#endif

#ifdef WIDE
DEE_A_RET_EXCEPT(-1) int _DeeFileIO_WideInitObjectEx
#else
DEE_A_RET_EXCEPT(-1) int _DeeFileIO_Utf8InitObjectEx
#endif
(DEE_A_IN DeeFileIOObject *self, DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *file,
 DEE_A_IN_Z char const *mode, DEE_A_IN Dee_mode_t permissions) {
 // Parse the open mode
 DEE_ASSERT(DeeObject_Check(file) && DEE_STRING_Check(file));
 if DEE_UNLIKELY(DeeFile_TryParseMode(mode,&self->fio_mode) != 0) return -1;
 _DeeFile_Init(self);
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (_DeeVFS_F(IsVirtualPath)(DEE_STRING_STR(file))) {
  struct DeeVFSFile vfs_file; int temp; DeeObject *rfs_path;
  if DEE_UNLIKELY((temp = DeeVFS_F(LocateOrReadReFsLink)(
   DEE_STRING_STR(file),&vfs_file,&rfs_path)) < 0) return temp;
  if (temp == 1) {
   DeeError_SetStringf(&DeeErrorType_FileNotFound,
                       "Could not find virtual file: %r:%q",
                       file,mode);
   return -1;
  }
  if (rfs_path) {
#ifdef WIDE
   temp = _DeeFileIO_WideInitObjectEx(self,rfs_path,mode,permissions);
#else
   temp = _DeeFileIO_Utf8InitObjectEx(self,rfs_path,mode,permissions);
#endif
   Dee_DECREF(rfs_path);
   return temp;
  }
  if DEE_UNLIKELY(!vfs_file.vf_open) {
   DeeError_SetStringf(&DeeErrorType_IOError,
                       "Cannot open virtual file: %r:%q",
                       file,mode);
   return -1;
  }
#ifdef WIDE
  Dee_INCREF(self->fio_widefile = (DEE_STRINGOBJECT *)file);
#else
  Dee_INCREF(self->fio_utf8file = (DEE_STRINGOBJECT *)file);
#endif
  if DEE_UNLIKELY((*vfs_file.vf_open)(&vfs_file,self,self->fio_mode) != 0) {
   Dee_DECREF(self->fio_utf8file);
   return -1;
  }
  return 0;
 }
#endif
 {
#ifdef DEE_PLATFORM_WINDOWS
  DWORD disposition,access,attr;
  DEE_STATIC_ASSERT(DEE_FILEIO_INVALID_HANDLE == (void *)INVALID_HANDLE_VALUE);
  switch (self->fio_mode&DEE_OPENMODE_MASKBASIC) {
   case DEE_OPENMODE_READ: disposition = OPEN_EXISTING; access = GENERIC_READ; break;
   case DEE_OPENMODE_WRITE: disposition = CREATE_ALWAYS; access = GENERIC_WRITE; break;
   case DEE_OPENMODE_APPEND: disposition = OPEN_ALWAYS; access = GENERIC_WRITE; break;
   case DEE_OPENMODE_READ|DEE_OPENMODE_FLAG_UPDATE: disposition = OPEN_EXISTING; access = GENERIC_READ|GENERIC_WRITE; break;
   case DEE_OPENMODE_WRITE|DEE_OPENMODE_FLAG_UPDATE: disposition = CREATE_ALWAYS; access = GENERIC_READ|GENERIC_WRITE; break;
   case DEE_OPENMODE_APPEND|DEE_OPENMODE_FLAG_UPDATE: disposition = OPEN_ALWAYS; access = GENERIC_READ|GENERIC_WRITE; break;
   default: DEE_BUILTIN_UNREACHABLE();
  }
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
#if 0 /*< Not a good idea... (data leakage and such) */
  DEE_ATOMIC_ONCE({
   // Required for taking advantage of the performance boost gained by 'SetFileValidData'
   _Dee_Win32AcquireProcessPrivilege(L"SeManageVolumePrivilege");
  });
#endif
  attr = FILE_ATTRIBUTE_NORMAL;
  // Make it readonly if there are no write permissions
  if ((permissions&0444)==0) attr |= FILE_ATTRIBUTE_READONLY;
  self->fio_handle = WIN32_F(CreateFile)(DEE_STRING_STR(file),access,
                                         FILE_SHARE_READ|FILE_SHARE_WRITE,
                                         NULL,disposition,attr,NULL);
  if (self->fio_handle == DEE_FILEIO_INVALID_HANDLE) {
   int temp,error = (int)DeeSystemError_Win32Consume();
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
   if (DEE_FS_WIN32_IS_UNC_ERROR(error) &&
      !DeeFS_F(Win32IsPathUNC)(DEE_STRING_STR(file))) {
    file = DeeFS_F(Win32PathUNC)(DEE_STRING_STR(file));
    error = _DeeFileIO_WideInitObjectEx(self,file,mode,permissions);
    Dee_DECREF(file);
    return error;
   }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
   switch (error) {
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
    case ERROR_BAD_NETPATH:not_found:
     DeeError_SetStringf(&DeeErrorType_FileNotFound,
                         DEE_PP_STR(WIN32_F(CreateFile))
                         "(%r:%q) : %K",file,mode,
                         DeeSystemError_Win32ToString((unsigned long)error));
     break;

    case ERROR_ACCESS_DENIED:
     // This might be caused by 'file' being a folder
     // Which is something that deemon actually allows
     if DEE_UNLIKELY((temp = _DeeFS_F(IsDir)(DEE_STRING_STR(file))) < 0) return temp;
     else if (temp) goto no_err;
     if (error == ERROR_PATH_NOT_FOUND) goto not_found;
     DEE_ATTRIBUTE_FALLTHROUGH

    default:
     DeeError_SetStringf(&DeeErrorType_IOError,
                         DEE_PP_STR(WIN32_F(CreateFile))
                         "(%r:%q) : %K",file,mode,
                         DeeSystemError_Win32ToString((unsigned long)error));
     break;
   }
   return -1;
  }
no_err:
#ifdef WIDE
  Dee_INCREF(self->fio_widefile = (DEE_STRINGOBJECT *)file);
#else
  Dee_INCREF(self->fio_utf8file = (DEE_STRINGOBJECT *)file);
#endif
#elif defined(WIDE)
 DeeObject *utf8file; int result;
 if DEE_UNLIKELY((utf8file = DeeUtf8String_Cast(file)) == NULL) return -1;
 result = _DeeFileIO_Utf8InitObjectEx(self,utf8file,mode,permissions);
 Dee_DECREF(utf8file);
 if DEE_LIKELY(result == 0) {
  Dee_DECREF(self->fio_utf8file);
  Dee_INCREF(self->fio_widefile = (DeeWideStringObject *)file);
 }
 return result;
#elif defined(DEE_PLATFORM_UNIX)
  int open_mode;
  switch (self->fio_mode&DEE_OPENMODE_MASKBASIC) {
   case DEE_OPENMODE_READ: open_mode = O_RDONLY; break;
   case DEE_OPENMODE_WRITE: open_mode = O_WRONLY|O_CREAT|O_TRUNC; break;
   case DEE_OPENMODE_APPEND: open_mode = O_WRONLY|O_CREAT|O_APPEND; break;
   case DEE_OPENMODE_READ|DEE_OPENMODE_FLAG_UPDATE: open_mode = O_RDWR; break;
   case DEE_OPENMODE_WRITE|DEE_OPENMODE_FLAG_UPDATE: open_mode = O_RDWR|O_CREAT|O_TRUNC; break;
   case DEE_OPENMODE_APPEND|DEE_OPENMODE_FLAG_UPDATE: open_mode = O_RDWR|O_CREAT|O_APPEND; break;
   default: DEE_BUILTIN_UNREACHABLE();
  }
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
  self->fio_handle = open(DeeUtf8String_STR(file),open_mode,permissions);
  if DEE_UNLIKELY(self->fio_handle == DEE_FILEIO_INVALID_HANDLE) {
   int error;
   switch ((error = DeeSystemError_Consume())) {
    case EISDIR:
    case ENOTDIR:
    case ENOENT:
     DeeError_SetStringf(&DeeErrorType_FileNotFound,
                         "open(%r:%q) : %K",file,mode,
                         DeeSystemError_ToString(error));
     break;
    default:
     DeeError_SetStringf(&DeeErrorType_IOError,
                         "open(%r:%q) : %K",file,mode,
                         DeeSystemError_ToString(error));
     break;
   }
   return -1;
  }
  Dee_INCREF(self->fio_utf8file = (DeeUtf8StringObject *)file);
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
  char const *open_mode;
  (void)permissions;
  switch (self->fio_mode&DEE_OPENMODE_MASKBASIC) {
   case DEE_OPENMODE_READ: open_mode = "r"; break;
   case DEE_OPENMODE_WRITE: open_mode = "w"; break;
   case DEE_OPENMODE_APPEND: open_mode = "a"; break;
   case DEE_OPENMODE_READ|DEE_OPENMODE_FLAG_UPDATE: open_mode = "r+"; break;
   case DEE_OPENMODE_WRITE|DEE_OPENMODE_FLAG_UPDATE: open_mode = "w+"; break;
   case DEE_OPENMODE_APPEND|DEE_OPENMODE_FLAG_UPDATE: open_mode = "a+"; break;
   default: DEE_BUILTIN_UNREACHABLE();
  }
  if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
  self->fio_handle = (void *)fopen(DeeUtf8String_STR(file),open_mode);
  if DEE_UNLIKELY(self->fio_handle == DEE_FILEIO_INVALID_HANDLE) {
   DeeError_SetStringf(&DeeErrorType_IOError,
                       "fopen(%r,%q) : %K",file,mode,
                       DeeSystemError_ToString(DeeSystemError_Consume()));
   return -1;
  }
  Dee_INCREF(self->ob_file = (DeeUtf8StringObject *)file);
#endif
 }
 return 0;
}

#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FILE_IO_INL */
