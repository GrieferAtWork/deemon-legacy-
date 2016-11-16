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
#ifndef GUARD_DEEMON_FILE_C
#define GUARD_DEEMON_FILE_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// include/*
#include <deemon/bool.h>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/fs_api.h>
#include <deemon/gc.h>
#include <deemon/generic_vtable.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_visit.h>
#include <deemon/integer.h>
#include <deemon/list.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/mp/thread.h>
#include <deemon/none.h>
#include <deemon/structured.h>
#include <deemon/super.h>
#include <deemon/time.h>
#include <deemon/tuple.h>
#include <deemon/tuple.h>

// src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>
#include <deemon/posix_features.inl>
#include DEE_INCLUDE_MEMORY_API()

#ifndef __INTELLISENSE__
#ifdef DEE_PLATFORM_WINDOWS
#include <deemon/sys/_win32.sysfd.filename.c.inl>
#endif /* DEE_PLATFORM_WINDOWS */
#endif /* ... */

#if DEE_PLATFORM_HAVE_IO
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#if defined(DEE_PLATFORM_WINDOWS)
#include <Windows.h>
#else
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#include <errno.h>
#endif
#if defined(DEE_PLATFORM_UNIX)
#if DEE_ENVIRONMENT_HAVE_INCLUDE_FCNTL_H
#include <fcntl.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_FEATURES_H
#include <features.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H
#include <unistd.h>
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_IO_H
#include <io.h>
#endif
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
#include <stdio.h>
#endif
#endif
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif /* DEE_PLATFORM_HAVE_IO */

DEE_DECL_BEGIN

int _DeeGenericContext_tp_io_readat_from_tp_io_readseek(
 struct DeeFileObject *self, Dee_uint64_t pos, void *p, Dee_size_t s, Dee_size_t *rs) {
 if (DeeFile_Seek((DeeObject *)self,*(Dee_int64_t*)&pos,DEE_SEEK_SET,NULL) != 0) return -1;
 return DeeFile_Read((DeeObject *)self,p,s,rs);
}
int _DeeGenericContext_tp_io_writeat_from_tp_io_writeseek(
 struct DeeFileObject *self, Dee_uint64_t pos, void const *p, Dee_size_t s, Dee_size_t *ws) {
 if (DeeFile_Seek((DeeObject *)self,*(Dee_int64_t*)&pos,DEE_SEEK_SET,NULL) != 0) return -1;
 return DeeFile_Write((DeeObject *)self,p,s,ws);
}




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
DEE_A_RET_EXCEPT(-1) int DeeFileIO_Size(
 DEE_A_IN_OBJECT(DeeFileObject) const *self, DEE_A_OUT Dee_uint64_t *size) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(!GetFileSizeEx(DeeFileIO_HANDLE(self),(PLARGE_INTEGER)size)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "GetFileSizeEx(%p:%r) : %K",
                      DeeFileIO_HANDLE(self),DeeFileIO_FILE(self),
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
#else // Fallback: Use default implementation
 // NOTE: Technically this cast removing the const qualifier is unsafe,
 //       as we can't guaranty that the file will remain in its original
 //       state after we have invoked seek.
 return DeeFile_Size((DeeObject *)self,size);
#endif
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





DEE_A_RET_EXCEPT(-1) int DeeFile_ParseMode(
 DEE_A_IN_Z char const *mode, DEE_A_OUT Dee_uint16_t *result) {
 DEE_ASSERT(mode && result);
 switch (*mode++) {
  case 'r': *result = DEE_OPENMODE_READ; break;
  case 'w': *result = DEE_OPENMODE_WRITE; break;
  case 'a': *result = DEE_OPENMODE_APPEND; break;
  default: return -1; // Invalid mode
 }
 if (*mode == '+') ++mode,*result |= DEE_OPENMODE_FLAG_UPDATE;
 if (*mode) return -1; // expected string end
 return 0;
}
DEE_A_RET_OBJECT(DeeStringObject) *DeeFile_ModeToString(DEE_A_IN Dee_uint16_t mode) {
 switch (mode&DEE_OPENMODE_MASKBASIC) {
  case DEE_OPENMODE_READ: DeeReturn_STATIC_STRING_NOREF("r");
  case DEE_OPENMODE_WRITE: DeeReturn_STATIC_STRING_NOREF("w");
  case DEE_OPENMODE_APPEND: DeeReturn_STATIC_STRING_NOREF("a");
  case DEE_OPENMODE_READ|DEE_OPENMODE_FLAG_UPDATE: DeeReturn_STATIC_STRING_NOREF("r+");
  case DEE_OPENMODE_WRITE|DEE_OPENMODE_FLAG_UPDATE: DeeReturn_STATIC_STRING_NOREF("w+");
  case DEE_OPENMODE_APPEND|DEE_OPENMODE_FLAG_UPDATE: DeeReturn_STATIC_STRING_NOREF("a+");
  default: break;
 }
 DeeReturn_EmptyString; // fallback
}





DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileWriterObject) *
DeeFileWriter_NewWithSizeHint(DEE_A_IN Dee_size_t size_hint) {
 DeeFileWriterObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOCF(
  DeeFileWriterObject,"String writer for")) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DeeFileWriter_Type);
  _DeeFile_Init(result);
  DeeStringWriter_InitWithSizeHint(&result->fw_writer,size_hint);
  result->fw_pos = DeeStringWriter_STR(&result->fw_writer);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileReaderObject) *
DeeFileReader_New(DEE_A_IN_OBJECT(DeeAnyStringObject) const *str_) {
 DeeFileReaderObject *result;
 DEE_ASSERT(DeeObject_Check(str_) && DeeAnyString_Check(str_));
 if DEE_LIKELY((result = DeeObject_MALLOCF(
  DeeFileReaderObject,"String reader for: %s",DeeString_STR(str_))) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DeeFileReader_Type);
  _DeeFile_Init(result);
  Dee_INCREF(result->fr_string = (DeeAnyStringObject *)str_);
  result->fr_end = (result->fr_pos = (Dee_uint8_t *)
                    DeeAnyString_DATAV(str_))+DeeAnyString_DATAC(str_);
 }
 return (DeeObject *)result;
}


#if DEE_PLATFORM_HAVE_IO
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
DeeFileIO_NewTemporary(DEE_A_IN Dee_uint32_t flags) {
 DeeString_NEW_STATIC(deemon_prefix,"deemon_fileio_temp");
 DeeObject *filename,*tmp,*result;
 if DEE_UNLIKELY((tmp = DeeFS_GetTmp()) == NULL) return NULL;
 filename = DeeFS_GetTmpNameObject(tmp,(DeeObject *)&deemon_prefix);
 Dee_DECREF(tmp);
 if DEE_UNLIKELY(!filename) return NULL;
 result = _DeeFileIO_NewObject(filename,"w");
 Dee_DECREF(filename);
 if ((flags&DEE_FILEIO_NEWTEMPORARY_FLAG_DELETE_WHEN_CLOSED)!=0 && result)
  ((DeeFileObject *)result)->fo_flags |= DEE_FILE_FLAG_IO_DELETE_ON_CLOSE;
 return result;
}
DEE_A_INTERRUPT DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *DeeFileIO_Utf8NewEx(
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
DEE_A_INTERRUPT DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *_DeeFileIO_Utf8NewEx(
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
DEE_A_INTERRUPT DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *DeeFileIO_WideNewEx(
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
DEE_A_INTERRUPT DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *_DeeFileIO_WideNewEx(
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
DEE_A_INTERRUPT DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *DeeFileIO_NewObjectEx(
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
DEE_A_INTERRUPT DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIOObject) *_DeeFileIO_NewObjectEx(
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

#endif

DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIteratorObject) *
DeeFile_IterSelf(DEE_A_INOUT_OBJECT(DeeFileObject) *self) {
 DeeFileIteratorObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 if DEE_LIKELY((result = DeeObject_MALLOCF(
  DeeFileIteratorObject,"Iterator for file: %p",self)) != NULL) {
  DeeObject_INIT(result,&DeeFileIterator_Type);
  Dee_INCREF(result->fi_file = (DeeFileObject *)self);
 }
 return (DeeObject *)result;
}


#if DEE_PLATFORM_HAVE_IO
#ifdef DEE_PLATFORM_WINDOWS
struct _DeeFileWin32DbgObject { DEE_FILE_OBJECT_HEAD };
static DeeObject *_deefilewin32dbg_tp_str(struct _DeeFileWin32DbgObject *DEE_UNUSED(self)) {
 DeeReturn_STATIC_STRING("<file.io.stddbg: mode \"w\", pos 0/0>");
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
 DeeFileIOObject ob_stdout;
 DeeFileIOObject ob_stderr;
 DeeFileIOObject ob_stdin;
 DeeFileObject   ob_stdnull;
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
#endif


DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileJoinedObject) *DeeFileJoined_New(
 DEE_A_INOUT_OBJECT(DeeFileObject) *fa, DEE_A_INOUT_OBJECT(DeeFileObject) *fb) {
 DeeFileJoinedObject *result;
 DEE_ASSERT(DeeObject_Check(fa) && DeeFile_Check(fa));
 DEE_ASSERT(DeeObject_Check(fb) && DeeFile_Check(fb));
 if DEE_LIKELY((result = DeeObject_MALLOCF(
  DeeFileJoinedObject,"file.joined(%p,%p)",fa,fb)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DeeFileJoined_Type);
  _DeeFile_Init(result);
  Dee_INCREF(result->fj_fa = (DeeFileObject *)fa);
  Dee_INCREF(result->fj_fb = (DeeFileObject *)fb);
 }
 return (DeeObject *)result;
}



DeeString_NEW_STATIC(_deefile_cant_read_error_msg,"Can't read more data");
DeeErrorObject _DeeErrorInstance_FileCantRead = {
 DEE_OBJECT_HEAD_INIT(&DeeErrorType_IOError),
 (DeeStringObject *)&_deefile_cant_read_error_msg};
DeeString_NEW_STATIC(_deefile_cant_write_error_msg,"Can't write more data");
DeeErrorObject _DeeErrorInstance_FileCantWrite = {
 DEE_OBJECT_HEAD_INIT(&DeeErrorType_IOError),
 (DeeStringObject *)&_deefile_cant_write_error_msg};
DEE_A_RET_EXCEPT(-1) int DeeFile_TRead(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_OUT_WB(*rs) void *p,
 DEE_A_IN Dee_size_t s, DEE_A_OUT Dee_size_t *rs) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeFileType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_io_read);
 return (*DeeType_GET_SLOT(tp_iter,tp_io_read))((DeeFileObject *)self,p,s,rs);
}
DEE_A_RET_EXCEPT(-1) int DeeFile_TWrite(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN_RB(s) void const *p,
 DEE_A_IN Dee_size_t s, DEE_A_OUT Dee_size_t *ws) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeFileType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_io_write);
 return (*DeeType_GET_SLOT(tp_iter,tp_io_write))((DeeFileObject *)self,p,s,ws);
}
DEE_A_RET_EXCEPT(-1) int DeeFile_TReadAt(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_uint64_t pos,
 DEE_A_OUT_WB(*rs) void *p, DEE_A_IN Dee_size_t s, DEE_A_OUT Dee_size_t *rs) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeFileType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_io_readat);
 return (*DeeType_GET_SLOT(tp_iter,tp_io_readat))((DeeFileObject *)self,pos,p,s,rs);
}
DEE_A_RET_EXCEPT(-1) int DeeFile_TWriteAt(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_uint64_t pos,
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s, DEE_A_OUT Dee_size_t *ws) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeFileType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_io_writeat);
 return (*DeeType_GET_SLOT(tp_iter,tp_io_writeat))((DeeFileObject *)self,pos,p,s,ws);
}
DEE_A_RET_EXCEPT(-1) int DeeFile_TReadAll(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s) {
 DeeTypeObject const *tp_iter;
 Dee_size_t temp; DeeType_SLOT_TYPE(tp_io_read) read_func;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeFileType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self;
 while DEE_UNLIKELY((read_func = DeeType_GET_SLOT(tp_iter,tp_io_read)) == NULL)
  tp_iter = DeeType_BASE(tp_iter);
 if DEE_LIKELY(s) while (1) {
  if DEE_UNLIKELY((*read_func)((DeeFileObject *)self,p,s,&temp) != 0) return -1;
  if DEE_UNLIKELY((s -= temp) == 0) break; // Most likely case on the first run: everything was read
  if DEE_UNLIKELY(!temp) goto err; // Second likely case: Can't read at all
  (*(char **)&p) += temp; // rare case: only read a bit (remainder must be read next)
 }
 return 0;
err:
 DeeError_Throw(DeeErrorInstance_FileCantRead);
 return -1;
}
DEE_A_RET_EXCEPT(-1) int DeeFile_TWriteAll(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self,
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s) {
 DeeTypeObject const *tp_iter;
 Dee_size_t temp; DeeType_SLOT_TYPE(tp_io_write) write_func;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeFileType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self)); tp_iter = tp_self;
 while DEE_UNLIKELY((write_func = DeeType_GET_SLOT(tp_iter,tp_io_write)) == NULL)
  tp_iter = DeeType_BASE(tp_iter);
 if DEE_LIKELY(s) while (1) {
  if DEE_UNLIKELY((*write_func)((DeeFileObject *)self,p,s,&temp) != 0) return -1;
  if DEE_UNLIKELY((s -= temp) == 0) break; // Most likely case on the first run: everything was written
  if DEE_UNLIKELY(!temp) goto err; // Second likely case: Can't write at all
  (*(char **)&p) += temp; // rare case: only written a bit (remainder must be written next)
 }
 return 0;
err:
 DeeError_Throw(DeeErrorInstance_FileCantWrite);
 return -1;
}
DEE_A_RET_EXCEPT(-1) int DeeFile_TReadAllAt(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_uint64_t pos,
 DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s) {
 DeeTypeObject const *tp_iter;
 Dee_size_t temp; DeeType_SLOT_TYPE(tp_io_readat) read_func;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeFileType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self;
 while DEE_UNLIKELY((read_func = DeeType_GET_SLOT(tp_iter,tp_io_readat)) == NULL)
  tp_iter = DeeType_BASE(tp_iter);
 if DEE_LIKELY(s) while (1) {
  if DEE_UNLIKELY((*read_func)((DeeFileObject *)self,pos,p,s,&temp) != 0) return -1;
  if DEE_UNLIKELY((s -= temp) == 0) break; // Most likely case on the first run: everything was read
  if DEE_UNLIKELY(!temp) goto err; // Second likely case: Can't read at all
  (*(char **)&p) += temp; // rare case: only read a bit (remainder must be read next)
  pos += temp;
 }
 return 0;
err:
 DeeError_Throw(DeeErrorInstance_FileCantRead);
 return -1;
}
DEE_A_RET_EXCEPT(-1) int DeeFile_TWriteAllAt(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_uint64_t pos,
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s) {
 DeeTypeObject const *tp_iter;
 Dee_size_t temp; DeeType_SLOT_TYPE(tp_io_writeat) write_func;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeFileType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self)); tp_iter = tp_self;
 while DEE_UNLIKELY((write_func = DeeType_GET_SLOT(tp_iter,tp_io_writeat)) == NULL)
  tp_iter = DeeType_BASE(tp_iter);
 if DEE_LIKELY(s) while (1) {
  if DEE_UNLIKELY((*write_func)((DeeFileObject *)self,pos,p,s,&temp) != 0) return -1;
  if DEE_UNLIKELY((s -= temp) == 0) break; // Most likely case on the first run: everything was written
  if DEE_UNLIKELY(!temp) goto err; // Second likely case: Can't write at all
  (*(char **)&p) += temp; // rare case: only written a bit (remainder must be written next)
  pos += temp;
 }
 return 0;
err:
 DeeError_Throw(DeeErrorInstance_FileCantWrite);
 return -1;
}

DEE_A_RET_EXCEPT(-1) int DeeFile_TSeek(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_int64_t off,
 DEE_A_IN int whence, DEE_A_OUT_OPT Dee_uint64_t *pos) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeFileType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_io_seek);
 return (*DeeType_GET_SLOT(tp_iter,tp_io_seek))((DeeFileObject *)self,off,whence,pos);
}
DEE_A_RET_EXCEPT(-1) int DeeFile_TFlush(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeFileType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_io_flush);
 return (*DeeType_GET_SLOT(tp_iter,tp_io_flush))((DeeFileObject *)self);
}
DEE_A_RET_EXCEPT(-1) int DeeFile_TTrunc(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeFileType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_io_trunc);
 return (*DeeType_GET_SLOT(tp_iter,tp_io_trunc))((DeeFileObject *)self);
}
void DeeFile_TClose(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeFileType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_io_close);
 (*DeeType_GET_SLOT(tp_iter,tp_io_close))((DeeFileObject *)self);
}


DEE_A_RET_EXCEPT(-1) int DeeFile_TWriteStructuredObject(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN DeeObject const *ob) {
 if DEE_UNLIKELY((ob = DeeObject_GetInstance((DeeObject *)ob,(DeeTypeObject *)&DeeStructured_Type)) == NULL) return -1;
 return DeeFile_TWriteAll(tp_self,self,DeeStructured_DATA(ob),DeeStructured_SIZE(ob));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeFile_TReadStructuredObject(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN DeeTypeObject const *tp) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&tp,(DeeTypeObject *)&DeeStructuredType_Type) != 0) return NULL;
 if DEE_UNLIKELY((result = (*DeeType_GET_SLOT(tp,tp_alloc))((DeeTypeObject *)tp)) == NULL) return NULL;
 if DEE_UNLIKELY(DeeFile_TReadAll(tp_self,self,DeeStructured_DATA(result),DeeStructured_SIZE(result)) != 0) Dee_CLEAR(result);
 return result;
}


#ifndef __INTELLISENSE__
#define AT
#define ALL
#include "file.impl.read.inl"
#define AT
#include "file.impl.read.inl"
#define ALL
#include "file.impl.read.inl"
#include "file.impl.read.inl"
#endif

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeFile_TReadLine(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_size_t max_bytes) {
 DeeObject *result; char ch; Dee_size_t rb;
 DeeType_SLOT_TYPE(tp_io_read) read_func;
 DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeFileType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 while DEE_UNLIKELY((read_func = DeeType_GET_SLOT(tp_self,tp_io_read)) == NULL) tp_self = DeeType_BASE(tp_self);
begin:
 if DEE_UNLIKELY((*read_func)((DeeFileObject *)self,&ch,sizeof(ch),&rb) != 0) goto err;
 if DEE_UNLIKELY(!rb) DeeReturn_EmptyString;
 if DEE_UNLIKELY(ch == '\n' && (DeeFile_FLAGS(self)&DEE_FILE_FLAG_SKIP_READLINE_LF)!=0) {
  DeeFile_FLAGS(self) &= ~DEE_FILE_FLAG_SKIP_READLINE_LF;
  goto begin;
 }
 while (max_bytes--) {
  if DEE_UNLIKELY(DeeStringWriter_WriteChar(&writer,ch) != 0) goto err;
  if DEE_UNLIKELY(ch == '\n') break;
  else if DEE_UNLIKELY(ch == '\r') {
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6011)
#endif
   // Always use '\n' for linefeeds
   DeeStringWriter_ELEM(&writer)[DeeStringWriter_SIZE(&writer)-1] = '\n';
#ifdef _PREFAST_
#pragma warning(pop)
#endif
   // Skip an optional '\n' char after '\r'
   DeeFile_FLAGS(self) |= DEE_FILE_FLAG_SKIP_READLINE_LF;
   break;
  }
  if DEE_UNLIKELY((*read_func)((DeeFileObject *)self,&ch,sizeof(ch),&rb) != 0) goto err;
  if DEE_UNLIKELY(!rb) break;
 }
 result = DeeStringWriter_Pack(&writer);
end:
 DeeStringWriter_Quit(&writer);
 return result;
err:
 result = NULL;
 goto end;
}

DEE_A_RET_EXCEPT(-1) int DeeFile_TGetc(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_OUT Dee_uint8_t *c) {
 Dee_size_t rb;
 if DEE_UNLIKELY(DeeFile_TRead(tp_self,self,c,1,&rb) != 0) return -1;
 if DEE_UNLIKELY(!rb) {
  DeeError_Throw(DeeErrorInstance_FileCantRead);
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeFile_TPutc(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN Dee_uint8_t c) {
 Dee_size_t wb;
 if DEE_UNLIKELY(DeeFile_TWrite(tp_self,self,&c,1,&wb) != 0) return -1;
 if DEE_UNLIKELY(!wb) {
  DeeError_Throw(DeeErrorInstance_FileCantWrite);
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeFile_TPrintf(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self, DEE_A_IN_PRINTF char const *fmt, ...) {
 va_list args; int result;
 DEE_VA_START(args,fmt);
 result = DeeFile_VTPrintf(tp_self,self,fmt,args);
 DEE_VA_END(args);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFile_VTPrintf(
 DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self,
 DEE_A_IN_PRINTF char const *fmt, DEE_A_INOUT va_list args) {
 DeeObject *temp; int result;
 if DEE_UNLIKELY((temp = DeeString_VNewf(fmt,args)) == NULL) return -1;
 result = DeeFile_TWriteAll(tp_self,self,DeeString_STR(temp),DeeString_SIZE(temp));
 Dee_DECREF(temp);
 return result;
}

DEE_A_RET_EXCEPT(-1) int _DeeFile_PrintString(
 DEE_A_INOUT_OBJECT(DeeFileObject) *self,
 DeeType_SLOT_TYPE(tp_io_write) write_func,
 DEE_A_IN_RB(s) char const *p, DEE_A_IN Dee_size_t s) {
 Dee_size_t temp;
 DEE_ASSERT(write_func);
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 if DEE_LIKELY(s) while (1) {
  if DEE_UNLIKELY((*write_func)((DeeFileObject *)self,p,s,&temp) != 0) return -1;
  if DEE_UNLIKELY((s -= temp) == 0) break; // Most likely case on the first run: everything was written
  if DEE_UNLIKELY(!temp) goto err; // Second likely case: Can't write at all
  (*(char **)&p) += temp; // rare case: only written a bit (remainder must be written next)
 }
 return 0;
err:
 DeeError_Throw(DeeErrorInstance_FileCantWrite);
 return -1;
}

static DEE_A_RET_EXCEPT(-1) int _DeeFile_PrintAllTuple(
 DEE_A_INOUT_OBJECT(DeeFileObject) *self,
 DeeType_SLOT_TYPE(tp_io_write) write_func,
 DEE_A_IN_OBJECT(DeeTupleObject) const *ob) {
 DeeObject **iter,**end,*elem_str; int temp;
 DEE_ASSERT(write_func);
 end = (iter = DeeTuple_ELEM(ob))+DeeTuple_SIZE(ob);
 if DEE_LIKELY(iter != end) while (1) {
  if DEE_UNLIKELY((elem_str = DeeObject_Str(*iter)) == NULL) return -1;
  temp = _DeeFile_PrintString(self,write_func,DeeString_STR(elem_str),DeeString_SIZE(elem_str));
  Dee_DECREF(elem_str);
  if DEE_UNLIKELY(temp != 0) return temp;
  if DEE_UNLIKELY(++iter == end) break;
  if DEE_UNLIKELY(_DeeFile_PrintString(self,write_func," ",1) != 0) return -1;
 }
 return 0;
}
static DEE_A_RET_EXCEPT(-1) int _DeeFile_PrintAllList(
 DEE_A_INOUT_OBJECT(DeeFileObject) *self,
 DeeType_SLOT_TYPE(tp_io_write) write_func,
 DEE_A_IN_OBJECT(DeeListObject) const *ob) {
 DeeObject *elem,*elem_str; Dee_size_t i; int temp;
 i = 0;
 DeeList_ACQUIRE(ob);
 for (; DEE_LIKELY(i < DeeList_SIZE(ob)); ++i) {
  Dee_INCREF(elem = DeeList_GET(ob,i));
  DeeList_RELEASE(ob);
  if DEE_UNLIKELY(i != 0 && _DeeFile_PrintString(
   self,write_func," ",1) != 0) { Dee_DECREF(elem); return -1; }
  elem_str = DeeObject_Str(elem);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(!elem_str) return -1;
  temp = _DeeFile_PrintString(self,write_func,
                              DeeString_STR(elem_str),
                              DeeString_SIZE(elem_str));
  Dee_DECREF(elem_str);
  if DEE_UNLIKELY(temp != 0) return temp;
  DeeList_ACQUIRE(ob);
 }
 DeeList_RELEASE(ob);
 return 0;
}
static DEE_A_RET_EXCEPT(-1) int _DeeFile_PrintAllUtf8String(
 DEE_A_INOUT_OBJECT(DeeFileObject) *self,
 DeeType_SLOT_TYPE(tp_io_write) write_func,
 DEE_A_INOUT_OBJECT(DeeUtf8StringObject) const *ob) {
 Dee_Utf8Char const *iter,*end; int temp;
 end = (iter = DeeUtf8String_STR(ob))+DeeUtf8String_SIZE(ob);
 if (iter != end) while (1) {
  temp = _DeeFile_PrintString(self,write_func,iter,1);
  if (temp != 0) return temp;
  if (iter == end) break;
  if (_DeeFile_PrintString(self,write_func," ",1) != 0) return -1;
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeFile_TPrintOne(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeTypeObject const *tp_v, DEE_A_INOUT DeeObject *v) {
 int result;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(v));
 DEE_ASSERT(DeeObject_Check(tp_v) && DeeType_Check(tp_v));
 DEE_ASSERT(DeeObject_InstanceOf(v,tp_v));
 if (tp_self == (DeeTypeObject *)&DeeNone_Type) return 0; // no-op
 if (tp_self == &DeeSuper_Type) return DeeFile_TPrintOne(DeeSuper_TYPE(self),DeeSuper_SELF(self),tp_v,v);
 if (DeeError_TypeError_CheckType((DeeObject *)tp_self,&DeeFileType_Type) != 0) return -1;
 if ((v = DeeObject_TStr(tp_v,v)) == NULL) return -1;
 result = DeeFile_TWriteAll(tp_self,self,DeeString_STR(v),DeeString_SIZE(v));
 Dee_DECREF(v);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeFile_TPrintAll(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeTypeObject const *tp_v, DEE_A_INOUT DeeObject *v) {
 DeeObject *iterator,*elem,*elem_str; int temp;
 DeeType_SLOT_TYPE(tp_io_write) write_func;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(v));
 DEE_ASSERT(DeeObject_Check(tp_v) && DeeType_Check(tp_v));
 DEE_ASSERT(DeeObject_InstanceOf(v,tp_v));
 if (tp_self == (DeeTypeObject *)&DeeNone_Type) return 0; // no-op
 if (tp_self == &DeeSuper_Type) return DeeFile_TPrintAll(DeeSuper_TYPE(self),DeeSuper_SELF(self),tp_v,v);
 if (DeeError_TypeError_CheckType((DeeObject *)tp_self,&DeeFileType_Type) != 0) return -1;
 while DEE_UNLIKELY((write_func = DeeType_GET_SLOT(tp_self,tp_io_write)) == NULL) tp_self = DeeType_BASE(tp_self);
 while DEE_UNLIKELY(tp_v == &DeeSuper_Type) tp_v = DeeSuper_TYPE(v),v = DeeSuper_SELF(v);
 // Optimizations for known types
 if (tp_v == &DeeTuple_Type) return _DeeFile_PrintAllTuple(self,write_func,v);
 if (tp_v == &DeeList_Type) return _DeeFile_PrintAllList(self,write_func,v);
 if (tp_v == &DeeUtf8String_Type) return _DeeFile_PrintAllUtf8String(self,write_func,v);
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_v,v)) == NULL) return -1;
 if DEE_LIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
next:
  if DEE_LIKELY((elem_str = DeeObject_Str(elem)) != NULL) {
   temp = _DeeFile_PrintString(
    self,write_func,DeeString_STR(elem_str),DeeString_SIZE(elem_str));
   Dee_DECREF(elem_str);
  } else temp = -1;
  Dee_DECREF(elem);
  if DEE_UNLIKELY(temp != 0) {err_iter: Dee_DECREF(iterator); return -1; }
  if DEE_LIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
   if DEE_UNLIKELY(_DeeFile_PrintString(self,write_func," ",1) != 0) { Dee_DECREF(elem); goto err_iter; }
   goto next;
  }
 }
 Dee_DECREF(iterator);
 return DEE_UNLIKELY(temp < 0) ? temp : 0;
}



#if DEE_PLATFORM_HAVE_IO
DEE_A_RET_OBJECT(DeeAnyStringObject) *
DeeFileIO_File(DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
 DeeFile_ACQUIRE(self);
 Dee_INCREF(result = DeeFileIO_FILE(self));
 DeeFile_RELEASE(self);
 return result;
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
#endif /* DEE_PLATFORM_HAVE_IO */

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeFileWriter_PackEx(
 DEE_A_INOUT_OBJECT(DeeFileWriterObject) *self, DEE_A_IN DeeTypeObject const *string_type) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileWriter_Check(self));
 DEE_ASSERTF(string_type == &DeeWideString_Type ||
             string_type == &DeeUtf8String_Type ||
             string_type == &DeeUtf16String_Type ||
             string_type == &DeeUtf32String_Type,
             "Invalid string type");
 result = DeeFileWriter_PACK(self);
 if (DEE_UNLIKELY(string_type != &DeeUtf8String_Type) && result) {
  Dee_DECREF(Dee_TYPE(result));
  Dee_INCREF(Dee_TYPE(result) = (DeeTypeObject *)string_type);
 }
 return result;
}

DEE_A_RET_OBJECT_REF(DeeStringObject) *DeeFileReader_String(
 DEE_A_IN_OBJECT(DeeFileReaderObject) const *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileReader_Check(self));
 DeeFile_ACQUIRE(self);
 Dee_INCREF(result = DeeFileReader_STRING(self));
 DeeFile_RELEASE(self);
 return result;
}




//////////////////////////////////////////////////////////////////////////
// File Iterator
static int _deefileiterator_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileIteratorObject *self) {
 if DEE_UNLIKELY((self->fi_file = (DeeFileObject *)DeeType_NewInstanceDefault(
  (DeeTypeObject *)&DeeFile_Type)) == NULL) return -1;
 return 0;
}
static void _deefileiterator_tp_dtor(DeeFileIteratorObject *self) {
 Dee_DECREF(self->fi_file);
}
DEE_VISIT_PROC(_deefileiterator_tp_visit,DeeFileIteratorObject *self) {
 Dee_VISIT(self->fi_file);
}
static int _deefileiterator_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileIteratorObject *self, DeeFileIteratorObject *right) {
 Dee_INCREF(self->fi_file = right->fi_file);
 return 0;
}
static int _deefileiterator_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileIteratorObject *self, DeeObject *args) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:file.iterator",&self->fi_file) != 0) return -1;
 if DEE_UNLIKELY((self->fi_file = (DeeFileObject *)DeeObject_GetInstance((
  DeeObject *)self->fi_file,(DeeTypeObject *)&DeeFile_Type)) == NULL) return -1;
 Dee_INCREF(self->fi_file);
 return 0;
}
static DeeObject *_deefileiterator_tp_str(DeeFileIteratorObject *self) {
 return DeeString_Newf("file.iterator(%k)",self->fi_file);
}
static DeeObject *_deefileiterator_tp_repr(DeeFileIteratorObject *self) {
 return DeeString_Newf("file.iterator(%r)",self->fi_file);
}
static int _deefileiterator_tp_seq_iter_next(
 DeeFileIteratorObject *self, DeeObject **result) {
 DeeObject *result_ob;
 if DEE_UNLIKELY((result_ob = DeeFile_ReadLine((
  DeeObject *)self->fi_file,(Dee_size_t)-1)) == NULL) return -1;
 if DEE_UNLIKELY(!DeeString_SIZE(result_ob)) { Dee_DECREF(result_ob); return 1; }
 Dee_INHERIT_REF(*result,result_ob);
 return 0;
}
static int DEE_CALL _deefileiterator_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileIteratorObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 if DEE_UNLIKELY((self->fi_file = (DeeFileObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if DEE_UNLIKELY(DeeError_TypeError_CheckType((DeeObject *)self->fi_file,(DeeTypeObject *)&DeeFile_Type) != 0) { Dee_DECREF(self->fi_file); return -1; }
 return 0;
}
static int DEE_CALL _deefileiterator_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileIteratorObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 return DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)self->fi_file);
}
static struct DeeTypeMarshal _deefileiterator_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(file_iterator)),
 member(&_deefileiterator_tp_marshal_ctor),member(&_deefileiterator_tp_marshal_put));

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deefileiterator_tp_member DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deefileiterator_tp_member[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__fi_file",DeeFileIteratorObject,fi_file,object),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */



#if DEE_PLATFORM_HAVE_IO
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
#endif


//////////////////////////////////////////////////////////////////////////
// Platform-specific stuff for FileIO
#if DEE_PLATFORM_HAVE_IO
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
 if DEE_UNLIKELY((self->fo_flags&DEE_FILE_FLAG_IO_VFSFILE)!=0) {
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
 if DEE_UNLIKELY((self->fo_flags&DEE_FILE_FLAG_IO_VFSFILE)!=0) {
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
 if DEE_UNLIKELY((self->fo_flags&DEE_FILE_FLAG_IO_VFSFILE)!=0) {
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
 if DEE_UNLIKELY((self->fo_flags&DEE_FILE_FLAG_IO_VFSFILE)!=0) {
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
 if DEE_UNLIKELY((self->fo_flags&DEE_FILE_FLAG_IO_VFSFILE)!=0) {
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
 if DEE_UNLIKELY((self->fo_flags&DEE_FILE_FLAG_IO_VFSFILE)!=0) {
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
   if ((self->fo_flags&DEE_FILE_FLAG_IO_DELETE_ON_CLOSE)!=0) {
    // Try to delete the file
    if DEE_UNLIKELY(_DeeFS_RmFileObject(DeeFileIO_FILE(self)) != 0) DeeError_Handled();
    self->fo_flags &= ~DEE_FILE_FLAG_IO_DELETE_ON_CLOSE;
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
#endif /* DEE_PLATFORM_HAVE_IO */


//////////////////////////////////////////////////////////////////////////
// Virtual functions of the stub base class 'file'
static int _deefile_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileObject *self) {
 _DeeFile_Init(self);
 return 0;
}
static int _deefile_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeFileObject *self, DeeFileObject *right) {
 DeeFile_ACQUIRE(right);
 _DeeFile_InitCopy(self,right);
 DeeFile_RELEASE(right);
 return 0;
}
static int _deefile_tp_copy_assign(
 DeeFileObject *self, DeeFileObject *right) {
 if DEE_LIKELY(self != right) {
  Dee_uint32_t new_flags;
  DeeFile_ACQUIRE(right);
  new_flags = right->fo_flags;
  DeeFile_RELEASE(right);
  DeeFile_ACQUIRE(self);
  self->fo_flags = new_flags;
  DeeFile_RELEASE(self);
 }
 return 0;
}
static DeeObject *_deefile_tp_str(DeeFileObject *self) {
 Dee_uint64_t pos,size;
 if DEE_UNLIKELY(DeeFile_Tell((DeeObject *)self,&pos) != 0 ||
                 DeeFile_Seek((DeeObject *)self,0,DEE_SEEK_END,&size) != 0 ||
                 DeeFile_Seek((DeeObject *)self,*(Dee_int64_t*)&pos,DEE_SEEK_SET,NULL) != 0) {
  if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return NULL;
  DeeReturn_STATIC_STRING("<file: pos ?/?>");
 } else {
  return DeeString_Newf("<file: pos %I64u/%I64u>",pos,size);
 }
}
static DeeObject *_deefile_tp_shl(DeeFileObject *self, DeeObject *right) {
 int temp;
 if DEE_UNLIKELY((right = DeeObject_Str(right)) == NULL) return NULL;
 temp = DeeFile_WriteAll((DeeObject *)self,DeeString_STR(right),DeeString_SIZE(right)*sizeof(char));
 Dee_DECREF(right);
 if DEE_UNLIKELY(temp != 0) return NULL;
 DeeReturn_NEWREF(self);
}
static DeeObject *_deefile_tp_or(DeeFileObject *self, DeeObject *right) {
 if DEE_UNLIKELY((right = DeeObject_GetInstance(right,(
  DeeTypeObject *)&DeeFile_Type)) == NULL) return NULL;
 return DeeFileJoined_New((DeeObject *)self,right);
}
static int _deefile_tp_io_read(
#ifdef DEE_DEBUG
 DeeFileObject *self,
#else
 DeeFileObject *DEE_UNUSED(self),
#endif
 void *DEE_UNUSED(p), Dee_size_t DEE_UNUSED(s), Dee_size_t *rs) {
#ifdef DEE_DEBUG
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
#endif
 *rs = 0; // Don't read anything (prevents very rightful out-of-memory in 'file().read()')
 return 0;
}
static int _deefile_tp_io_write(
 DeeFileObject *self, void const *DEE_UNUSED(p),
 Dee_size_t DEE_UNUSED(s), Dee_size_t *ws) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 *ws = 0; // Can't write, but not an error
 return 0;
}
static int _deefile_tp_io_seek(
 DeeFileObject *self, Dee_int64_t DEE_UNUSED(off),
 int DEE_UNUSED(whence), Dee_uint64_t *pos) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 if (pos) *pos = 0;
 return 0;
}
static int _deefile_tp_io_flush(DeeFileObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 return 0;
}
#define _deefile_tp_io_trunc _deefile_tp_io_flush
static void _deefile_tp_io_close(DeeFileObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
}


void _deefile_invalid_seek_mode(DEE_A_IN int mode) {
 DeeError_SetStringf(&DeeErrorType_IOError,"Invalid seek mode: %d",mode);
}

static int DEE_CALL _deefile_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileObject *self,
 DeeObject *DEE_UNUSED(infile), struct DeeMarshalReadMap *DEE_UNUSED(map)) {
 _DeeFile_Init(self);
 return 0;
}
static int DEE_CALL _deefile_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileObject *DEE_UNUSED(self),
 DeeObject *DEE_UNUSED(outfile), struct DeeMarshalWriteMap *DEE_UNUSED(map)) {
 return 0;
}
static struct DeeTypeMarshal _deefile_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(file)),
 member(&_deefile_tp_marshal_ctor),member(&_deefile_tp_marshal_put));





//////////////////////////////////////////////////////////////////////////
// Virtual functions for fileio file objects
#if DEE_PLATFORM_HAVE_IO
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
DeeString_NEW_STATIC_EX(_deefileio_default_mode,1,{'r'});
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
 right->fo_flags = DEE_FILE_FLAG_NONE;
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
  right->fo_flags = DEE_FILE_FLAG_NONE;
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
#endif /* DEE_PLATFORM_HAVE_IO */


static void _deefilewriter_tp_dtor(DeeFileWriterObject *self) {
 DeeStringWriter_Quit(&self->fw_writer);
}
static int _deefilewriter_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileWriterObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileWriter_Check(self));
 _DeeFile_Init(self);
 DeeStringWriter_Init(&self->fw_writer);
 self->fw_pos = NULL;
 return 0;
}
static int _deefilewriter_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileWriterObject *self, DeeObject *args) {
 Dee_size_t size_hint = 0;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileWriter_Check(self));
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|Iu",&size_hint) != 0) return -1;
 _DeeFile_Init(self);
 DeeStringWriter_InitWithSizeHint(&self->fw_writer,size_hint);
 self->fw_pos = DeeStringWriter_STR(&self->fw_writer);
 return 0;
}
static int _deefilewriter_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileWriterObject *self, DeeFileWriterObject *right) {
 Dee_size_t right_size;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileWriter_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeFileWriter_Check(right));
 _DeeFile_Init(self);
again:
 DeeFile_ACQUIRE(right);
 if DEE_LIKELY((right_size = right->fw_writer.sw_len) != 0) {
  if DEE_UNLIKELY((self->fw_writer.sw_str = (DeeStringObject *)DeeObject_TryWeakMalloc(
   Dee_OFFSETOF(DeeAnyStringObject,as_str)+((right_size+1)*sizeof(Dee_Char)))) == NULL) {
   DeeFile_RELEASE(right);
   if DEE_LIKELY(Dee_CollectMemory()) goto again;
   DeeError_NoMemory();
   return -1;
  }
  DeeObject_INIT(self->fw_writer.sw_str,&DeeString_Type);
  self->fw_writer.sw_str->s_len = right_size;
  memcpy(DeeString_STR(self->fw_writer.sw_str),
         DeeString_STR(right->fw_writer.sw_str),
        (right_size)*sizeof(Dee_Char));
  DeeString_STR(self->fw_writer.sw_str)[right_size] = 0;
 } else {
  self->fw_writer.sw_len = 0;
  self->fw_writer.sw_str = NULL;
 }
 self->fo_flags = right->fo_flags;
 if DEE_LIKELY(self->fw_writer.sw_str) {
  self->fw_pos = DeeString_STR(self->fw_writer.sw_str)+(
   Dee_size_t)(right->fw_pos-DeeString_STR(right->fw_writer.sw_str));
 } else self->fw_pos = NULL;
 DeeFile_RELEASE(right);
 return 0;
}
static int _deefilewriter_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileWriterObject *self, DeeFileWriterObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileWriter_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeFileWriter_Check(right));
 DeeFile_ACQUIRE(right);
 DeeStringWriter_InitMove(&self->fw_writer,&right->fw_writer);
 self->fw_pos = right->fw_pos;
 right->fw_pos = NULL;
 DeeFile_RELEASE(right);
 return 0;
}
static int _deefilewriter_tp_copy_assign(
 DeeFileWriterObject *self, DeeFileWriterObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileWriter_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeFileWriter_Check(right));
 if DEE_LIKELY(self != right) {
  DeeStringWriter new_writer;
  DeeStringObject *old_string;
  Dee_uint32_t new_flags;
  Dee_size_t new_size,new_pos;
again:
  DeeFile_ACQUIRE(right);
  if DEE_LIKELY((new_size = right->fw_writer.sw_len) != 0) {
   if DEE_UNLIKELY((new_writer.sw_str = (DeeStringObject *)DeeObject_TryWeakMalloc(
    Dee_OFFSETOF(DeeAnyStringObject,as_str)+((new_size+1)*sizeof(Dee_Char)))) == NULL) {
    DeeFile_RELEASE(right);
    if DEE_LIKELY(Dee_CollectMemory()) goto again;
    DeeError_NoMemory();
    return -1;
   }
   DeeObject_INIT(new_writer.sw_str,&DeeString_Type);
   new_writer.sw_str->s_len = new_size;
   memcpy(DeeString_STR(new_writer.sw_str),
          DeeString_STR(right->fw_writer.sw_str),
         (new_size)*sizeof(Dee_Char));
   DeeString_STR(new_writer.sw_str)[new_size] = 0;
  } else {
   new_writer.sw_len = 0;
   new_writer.sw_str = NULL;
  }
  new_flags = right->fo_flags;
  new_pos = (Dee_size_t)(right->fw_pos-DeeString_STR(right->fw_writer.sw_str));
  DeeFile_RELEASE(right);
  DeeFile_ACQUIRE(self);
  old_string = self->fw_writer.sw_str;
  self->fw_writer = new_writer;
  self->fo_flags = new_flags;
  self->fw_pos = DeeStringWriter_STR(&new_writer)+new_pos;
  DeeFile_RELEASE(self);
  if (old_string) {
   Dee_DECREF(Dee_TYPE(old_string));
   DeeObject_Free(old_string);
  }
 }
 return 0;
}
static int _deefilewriter_tp_move_assign(DeeFileWriterObject *self, DeeFileWriterObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileWriter_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeFileWriter_Check(right));
 if DEE_LIKELY(self != right) {
  DeeStringWriter new_writer;
  DeeStringObject *old_string;
  Dee_uint32_t new_flags; char *new_pos;
  DeeFile_ACQUIRE(right);
  new_writer = right->fw_writer;
  new_flags = right->fo_flags;
  new_pos = right->fw_pos;
  DeeStringWriter_Init(&right->fw_writer);
  right->fo_flags = DEE_FILE_FLAG_NONE;
  right->fw_pos = NULL;
  DeeFile_RELEASE(right);
  DeeFile_ACQUIRE(self);
  old_string = self->fw_writer.sw_str;
  self->fw_writer = new_writer;
  self->fo_flags = new_flags;
  self->fw_pos = new_pos;
  DeeFile_RELEASE(self);
  if (old_string) {
   Dee_DECREF(Dee_TYPE(old_string));
   DeeObject_Free(old_string);
  }
 }
 return 0;
}

static DeeObject *_deefilewriter_tp_str(DeeFileWriterObject *self) {
 Dee_size_t pos,size;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileWriter_Check(self));
 DeeFile_ACQUIRE(self);
 pos = (Dee_size_t)(self->fw_pos-DeeStringWriter_STR(&self->fw_writer));
 size = DeeStringWriter_SIZE(&self->fw_writer);
 DeeFile_RELEASE(self);
 return DeeString_Newf("<file.writer: pos %Iu/%Iu>",pos,size);
}
static DeeObject *_deefilewriter_tp_repr(DeeFileWriterObject *self) {
 Dee_size_t pos,size,alloc;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileWriter_Check(self));
 DeeFile_ACQUIRE(self);
 pos = (Dee_size_t)(self->fw_pos-DeeStringWriter_STR(&self->fw_writer));
 size = DeeStringWriter_SIZE(&self->fw_writer);
 alloc = DeeStringWriter_ALLOCATED(&self->fw_writer);
 DeeFile_RELEASE(self);
 return DeeString_Newf("<file.writer: pos %Iu/%Iu, allocated %Iu>",pos,size,alloc);
}
static int _deefilewriter_tp_io_read(DeeFileWriterObject *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 Dee_size_t max_read;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileWriter_Check(self));
 DeeFile_ACQUIRE(self);
 max_read = (Dee_size_t)(self->fw_pos-DeeStringWriter_STR(&self->fw_writer));
 max_read = (max_read >= DeeStringWriter_SIZE(&self->fw_writer))
  ?0:(DeeStringWriter_SIZE(&self->fw_writer)-max_read);
 if DEE_UNLIKELY(s > max_read) s = max_read;
 memcpy(p,self->fw_pos,s);
 self->fw_pos += s;
 DeeFile_RELEASE(self);
 *rs = s;
 return 0;
}
static int _deefilewriter_tp_io_write(DeeFileWriterObject *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 Dee_size_t req_size,old_pos,new_size;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileWriter_Check(self));
again:
 DeeFile_ACQUIRE(self);
 old_pos = (Dee_size_t)(self->fw_pos-DeeStringWriter_STR(&self->fw_writer));
 req_size = old_pos+s;
 if DEE_UNLIKELY(req_size > DeeStringWriter_ALLOCATED(&self->fw_writer)) {
  // Make sure the buffer capacity can sustain our memory
  do {
   enum{min_size=8};
   DEE_ASSERT(self);
   if DEE_LIKELY(self->fw_writer.sw_str) {
    DeeStringObject *new_str;
    DEE_ASSERT(DeeObject_Check(self->fw_writer.sw_str) && DeeString_Check(self->fw_writer.sw_str));
    new_size = DeeString_SIZE(self->fw_writer.sw_str)*2;
    if DEE_UNLIKELY((new_str = (DeeStringObject *)DeeObject_TryWeakRealloc(self->fw_writer.sw_str,
     Dee_OFFSETOF(DeeAnyStringObject,as_str)+((new_size+1)*sizeof(Dee_Char)))) == NULL) {
no_mem:
     DeeFile_RELEASE(self);
     if DEE_LIKELY(Dee_CollectMemory()) goto again;
     DeeError_NoMemory();
     return -1;
    }
    new_str->s_len = new_size;
    self->fw_writer.sw_str = new_str;
   } else {
    DEE_ASSERT(!self->fw_writer.sw_len && "No string allocated, but size is set");
    if DEE_UNLIKELY((self->fw_writer.sw_str = (DeeStringObject *)DeeObject_TryWeakMalloc(
     Dee_OFFSETOF(DeeAnyStringObject,as_str)+(min_size+1)*sizeof(Dee_Char))) == NULL) goto no_mem;
    DeeObject_INIT(self->fw_writer.sw_str,&DeeString_Type);
    self->fw_writer.sw_str->s_len = min_size;
   }
  } while DEE_UNLIKELY(req_size > DeeStringWriter_ALLOCATED(&self->fw_writer));
  self->fw_pos = DeeStringWriter_ELEM(&self->fw_writer)+old_pos;
  DeeStringWriter_SIZE(&self->fw_writer) = old_pos+s;
 } else if DEE_LIKELY(DeeStringWriter_SIZE(&self->fw_writer) < old_pos+s) {
  DeeStringWriter_SIZE(&self->fw_writer) = old_pos+s;
 }
 DEE_ASSERT(self->fw_pos);
 memcpy(self->fw_pos,p,s);
 self->fw_pos += s;
 DeeFile_RELEASE(self);
 *ws = s;
 return 0;
}
static int _deefilewriter_tp_io_seek(
 DeeFileWriterObject *self, Dee_int64_t off,
 int whence, Dee_uint64_t *pos) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileWriter_Check(self));
 DeeFile_ACQUIRE(self);
 switch (whence) {
  case DEE_SEEK_SET:
   self->fw_pos = DeeStringWriter_STR(&self->fw_writer)+(Dee_size_t)(*(Dee_uint64_t*)&off);
   break;
  case DEE_SEEK_CUR:
   self->fw_pos += (Dee_ptrdiff_t)off;
   goto check_pre;
  case DEE_SEEK_END:
   self->fw_pos = (DeeStringWriter_STR(&self->fw_writer)+DeeStringWriter_SIZE(&self->fw_writer))-(Dee_size_t)(*(Dee_uint64_t*)&off);
check_pre:
   if DEE_UNLIKELY(DEE_UNLIKELY(self->fw_pos < DeeStringWriter_STR(&self->fw_writer)))
    self->fw_pos = DeeStringWriter_STR(&self->fw_writer); // Fix the rare chance of an underflow
   break;
  default:
   DeeFile_RELEASE(self);
   _deefile_invalid_seek_mode(whence);
   return -1;
 }
 if (pos) *pos = (Dee_uint64_t)((Dee_size_t)(self->fw_pos-DeeStringWriter_STR(&self->fw_writer)));
 DeeFile_RELEASE(self);
 return 0;
}
static int _deefilewriter_tp_io_flush(DeeFileWriterObject *self) {
 Dee_size_t old_pos,new_size; DeeStringObject *new_str;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileWriter_Check(self));
again:
 DeeFile_ACQUIRE(self);
 if DEE_LIKELY(self->fw_writer.sw_str) {
  old_pos = (Dee_size_t)(self->fw_pos-DeeStringWriter_STR(&self->fw_writer));
  if DEE_LIKELY(DeeString_SIZE(self->fw_writer.sw_str) != self->fw_writer.sw_len) {
   if DEE_LIKELY((new_size = self->fw_writer.sw_len) != 0) {
    if DEE_UNLIKELY((new_str = (DeeStringObject *)DeeObject_TryWeakRealloc(self->fw_writer.sw_str,
     Dee_OFFSETOF(DeeAnyStringObject,as_str)+((new_size+1)*sizeof(Dee_Char)))) == NULL) {
     DeeFile_RELEASE(self);
     if DEE_LIKELY(Dee_CollectMemory()) goto again;
     DeeError_NoMemory();
     return -1;
    }
    new_str->s_len = new_size;
    Dee_INHERIT_REF(self->fw_writer.sw_str,new_str);
    self->fw_pos = DeeStringWriter_STR(&self->fw_writer)+old_pos;
   } else {
    Dee_DECREF(Dee_TYPE(self->fw_writer.sw_str));
    DeeObject_Free(self->fw_writer.sw_str);
    self->fw_writer.sw_str = NULL;
    self->fw_pos = ((Dee_Char*)0)+old_pos;
   }
  }
 }
 DeeFile_RELEASE(self);
 return 0;
}
static int _deefilewriter_tp_io_trunc(DeeFileWriterObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileWriter_Check(self));
 DeeFile_ACQUIRE(self); // User must call flush if he wants the unused data to be freed
 self->fw_writer.sw_len = (Dee_size_t)(
  self->fw_pos-DeeStringWriter_STR(&self->fw_writer));
 DeeFile_RELEASE(self);
 return 0;
}
static void _deefilewriter_tp_io_close(DeeFileWriterObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileWriter_Check(self));
 DeeFile_ACQUIRE(self);
 DeeStringWriter_ClearAndFlush(&self->fw_writer);
 self->fw_pos = NULL;
 DeeFile_RELEASE(self);
}
static int DEE_CALL _deefilewriter_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileWriterObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *DEE_UNUSED(map)) {
 Dee_uint64_t read_pos,read_size; Dee_size_t used_size;
 if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&read_pos) != 0) return -1;
 if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&read_size) != 0) return -1;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_pos > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "file.writer position is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_pos,(Dee_size_t)-1);
  return -1;
 }
 if DEE_UNLIKELY(read_size > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "file.writer size is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_size,(Dee_size_t)-1);
  return -1;
 }
#endif
 used_size = (Dee_size_t)read_size;
 self->fw_writer.sw_len = used_size;
 if DEE_LIKELY(used_size) {
  if DEE_UNLIKELY((self->fw_writer.sw_str = (DeeStringObject *)DeeString_NewSized(used_size)) == NULL) return -1;
  if DEE_UNLIKELY(DeeFile_ReadAll(infile,DeeString_STR(self->fw_writer.sw_str),used_size) != 0) { Dee_DECREF(self->fw_writer.sw_str); return -1; }
  self->fw_pos = DeeString_STR(self->fw_writer.sw_str)+(Dee_size_t)read_pos;
 } else {
  self->fw_pos = ((Dee_Char *)NULL)+(Dee_size_t)read_pos;
  self->fw_writer.sw_str = NULL;
 }
 _DeeFile_Init(self);
 return 0;
}
static int DEE_CALL _deefilewriter_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileWriterObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *DEE_UNUSED(map)) {
 Dee_size_t str_size; Dee_uint64_t written_pos;
 Dee_Char *str_,*stored_str; int result;
again:
 DeeFile_ACQUIRE(self);
 stored_str = DeeStringWriter_STR(&self->fw_writer);
 written_pos = (Dee_uint64_t)((Dee_size_t)(self->fw_pos-stored_str));
 str_size = DeeStringWriter_SIZE(&self->fw_writer);
 while DEE_UNLIKELY((str_ = (Dee_Char *)malloc_nz(str_size*sizeof(Dee_Char))) == NULL) {
  DeeFile_RELEASE(self);
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return -1;
 }
 memcpy(str_,stored_str,str_size*sizeof(Dee_Char));
 DeeFile_RELEASE(self);
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,written_pos) != 0) goto err;
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)str_size) != 0) goto err;
 result = DeeFile_WriteAll(outfile,str_,str_size);
end: free(str_); return result;
err: result = -1; goto end;
}
static struct DeeTypeMarshal _deefilewriter_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(file_writer)),
 member(&_deefilewriter_tp_marshal_ctor),member(&_deefilewriter_tp_marshal_put));





DEE_STATIC_INLINE(void) _deefilereader_error_readonly(void) {
 DeeError_SET_STRING(&DeeErrorType_IOError,"file.reader is a read-only stream");
}
static int _deefilereader_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileReaderObject *self) {
 _DeeFile_Init(self);
 Dee_INCREF(self->fr_string = (DeeAnyStringObject *)Dee_EmptyString);
 self->fr_pos = self->fr_end = (Dee_uint8_t *)DeeString_STR(Dee_EmptyString);
 return 0;
}
static void _deefilereader_tp_dtor(DeeFileReaderObject *self) {
 Dee_DECREF(self->fr_string);
}
DEE_VISIT_PROC(_deefilereader_tp_visit,DeeFileReaderObject *self) {
 Dee_VISIT(self->fr_string);
}
static int _deefilereader_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileReaderObject *self, DeeFileReaderObject *right) {
 _DeeFile_Init(self);
 DeeFile_ACQUIRE(right);
 Dee_INCREF(self->fr_string = right->fr_string);
 self->fr_pos = right->fr_pos;
 self->fr_end = right->fr_end;
 DeeFile_RELEASE(right);
 return 0;
}
static int _deefilereader_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileReaderObject *self, DeeObject *args) {
 DeeObject *arg;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:file.reader",&arg) != 0) return -1;
 _DeeFileReader_SET_INITIAL_STRING(self,arg,{ return -1; });
 _DeeFile_Init(self);
 return 0;
}
static int _deefilereader_tp_copy_assign(
 DeeFileReaderObject *self, DeeFileReaderObject *right) {
 if DEE_LIKELY(self != right) {
  DeeAnyStringObject *old_string,*new_string;
  Dee_uint32_t new_flags; Dee_uint8_t const *new_pos;
  DeeFile_ACQUIRE(right);
  new_flags = right->fo_flags;
  new_pos = right->fr_pos;
  Dee_INCREF(new_string = right->fr_string);
  DeeFile_RELEASE(right);
  DeeFile_ACQUIRE(self);
  self->fo_flags = new_flags;
  old_string = self->fr_string;
  Dee_INCREF(self->fr_string = right->fr_string);
  self->fr_pos = new_pos;
  self->fr_end = (Dee_uint8_t *)DeeAnyString_DATAV(new_string)+DeeString_SIZE(new_string);
  DeeFile_RELEASE(self);
  Dee_DECREF(old_string);
 }
 return 0;
}
static int _deefilereader_tp_any_assign(DeeFileReaderObject *self, DeeObject *right) {
 Dee_uint8_t const *new_pos,*new_end; DeeObject *old_string;
 if DEE_LIKELY(DeeAnyString_Check(right)) {
  new_end = (new_pos = (Dee_uint8_t const *)DeeAnyString_DATAV(right))+DeeAnyString_DATAC(right);
 } else if DEE_LIKELY(DeeNone_Check(right)) {
  new_end = new_pos = (Dee_uint8_t const *)DeeString_STR(Dee_EmptyString);
  right = Dee_EmptyString;
 } else {
  DeeError_SetStringf(&DeeErrorType_TypeError,
                      "Unexpected type (expected: any string; got: %s)",
                      DeeType_NAME(Dee_TYPE(right)));
  return -1;
 }
 Dee_INCREF(right); // Later owned by 'self->fr_string'
 DeeFile_ACQUIRE(self);
 old_string = (DeeObject *)self->fr_string;
 self->fr_string = (DeeAnyStringObject *)right;
 self->fr_pos = new_pos;
 self->fr_end = new_end;
 DeeFile_RELEASE(self);
 Dee_DECREF(old_string);
 return 0;
}
static DeeObject *_deefilereader_tp_str(DeeFileReaderObject *self) {
 Dee_size_t pos,size;
 DeeFile_ACQUIRE(self);
 pos = (Dee_size_t)(self->fr_pos-(Dee_uint8_t *)DeeAnyString_DATAV(self->fr_string));
 size = DeeAnyString_DATAC(self->fr_string);
 DeeFile_RELEASE(self);
 return DeeString_Newf("<file.reader: pos %Iu/%Iu>",pos,size);
}
static int _deefilereader_tp_io_read(
 DeeFileReaderObject *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 Dee_size_t max_read;
 DeeFile_ACQUIRE(self);
 if DEE_UNLIKELY(self->fr_pos >= self->fr_end) {
  DeeFile_RELEASE(self);
  *rs = 0;
  return 0;
 }
 if DEE_UNLIKELY(s > (max_read = (Dee_size_t)(self->fr_end-self->fr_pos))) s = max_read;
 memcpy(p,self->fr_pos,s);
 self->fr_pos += s;
 DeeFile_RELEASE(self);
 *rs = s;
 return 0;
}
static int _deefilereader_tp_io_write(DeeFileReaderObject *DEE_UNUSED(self),
 void const *DEE_UNUSED(p), Dee_size_t DEE_UNUSED(s), Dee_size_t *DEE_UNUSED(ws)) {
 _deefilereader_error_readonly();
 return -1;
}
static int _deefilereader_tp_io_seek(
 DeeFileReaderObject *self, Dee_int64_t off,
 int whence, Dee_uint64_t *pos) {
 Dee_uint8_t const *new_pos;
 DeeFile_ACQUIRE(self);
 switch (whence) {
  case DEE_SEEK_SET:
   new_pos = (Dee_uint8_t *)DeeAnyString_DATAV(
    self->fr_string)+(Dee_size_t)*(Dee_uint64_t*)&off;
   break;
  case DEE_SEEK_CUR:
   new_pos = self->fr_pos+off;
   break;
  case DEE_SEEK_END:
   new_pos = self->fr_end-(Dee_size_t)*(Dee_uint64_t *)&off;
   break;
  default:
   DeeFile_RELEASE(self);
   _deefile_invalid_seek_mode(whence);
   return -1;
 }
 if DEE_UNLIKELY(new_pos < (Dee_uint8_t *)DeeAnyString_DATAV(self->fr_string))
  new_pos = (Dee_uint8_t *)DeeAnyString_DATAV(self->fr_string);
 self->fr_pos = new_pos;
 if (pos) *pos = (Dee_uint64_t)((Dee_size_t)(
  self->fr_pos-(Dee_uint8_t *)DeeAnyString_DATAV(self->fr_string)));
 DeeFile_RELEASE(self);
 return 0;
}
static int _deefilereader_tp_io_flush(DeeFileReaderObject *DEE_UNUSED(self)) { return 0;}
static int _deefilereader_tp_io_trunc(DeeFileReaderObject *DEE_UNUSED(self)) {
 _deefilereader_error_readonly();
 return -1;
}
static void _deefilereader_tp_io_close(DeeFileReaderObject *self) {
 DeeAnyStringObject *old_string;
 DeeFile_ACQUIRE(self);
 old_string = self->fr_string;
 Dee_INCREF(self->fr_string = (DeeAnyStringObject *)Dee_EmptyString);
 self->fr_pos = self->fr_end = (Dee_uint8_t *)DeeAnyString_DATAV(Dee_EmptyString);
 DeeFile_RELEASE(self);
 Dee_DECREF(old_string);
}

static int DEE_CALL _deefilereader_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileReaderObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_uint64_t read_pos; DeeObject *read_string;
 if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&read_pos) != 0) return -1;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_pos > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "file.reader position is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_pos,(Dee_size_t)-1);
  return -1;
 }
#endif
 if DEE_UNLIKELY((read_string = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 _DeeFileReader_SET_INITIAL_STRING(self,read_string,{ Dee_DECREF(read_string); return -1; });
 self->fr_pos += (Dee_size_t)read_pos;
 _DeeFile_Init(self);
 Dee_DECREF(read_string);
 return 0;
}
static int DEE_CALL _deefilereader_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileReaderObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 Dee_uint64_t written_pos; DeeAnyStringObject *read_from_str; int result;
 DeeFile_ACQUIRE(self);
 Dee_INCREF(read_from_str = self->fr_string);
 written_pos = (Dee_uint64_t)((Dee_size_t)(self->fr_pos-(
  Dee_uint8_t *)DeeAnyString_DATAV(read_from_str)));
 DeeFile_RELEASE(self);
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,written_pos) != 0) { Dee_DECREF(read_from_str); return -1; }
 result = DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)read_from_str);
 Dee_DECREF(read_from_str);
 return result;
}
static struct DeeTypeMarshal _deefilereader_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(file_reader)),
 member(&_deefilereader_tp_marshal_ctor),member(&_deefilereader_tp_marshal_put));




//////////////////////////////////////////////////////////////////////////
// file.joined VTable
#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deefilejoined_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deefilejoined_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__fj_fa",DeeFileJoinedObject,fj_fa,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__fj_fb",DeeFileJoinedObject,fj_fb,object),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static int _deefilejoined_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileJoinedObject *self) {
 _DeeFile_Init(self);
 if DEE_UNLIKELY((self->fj_fa = (DeeFileObject *)DeeType_NewInstanceDefault(
  (DeeTypeObject *)&DeeFile_Type)) == NULL) return -1;
 Dee_INCREF(self->fj_fb = self->fj_fa);
 return 0;
}
static void _deefilejoined_tp_dtor(DeeFileJoinedObject *self) {
 Dee_DECREF(self->fj_fa);
 Dee_DECREF(self->fj_fb);
}
DEE_VISIT_PROC(_deefilejoined_tp_visit,DeeFileJoinedObject *self) {
 DeeFile_ACQUIRE(self);
 Dee_VISIT(self->fj_fa);
 Dee_VISIT(self->fj_fb);
 DeeFile_RELEASE(self);
}
static int _deefilejoined_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileJoinedObject *self, DeeFileJoinedObject *right) {
 _DeeFile_Init(self);
 DeeFile_ACQUIRE(right);
 Dee_INCREF(self->fj_fa = right->fj_fa);
 Dee_INCREF(self->fj_fb = right->fj_fb);
 DeeFile_RELEASE(right);
 return 0;
}
static int _deefilejoined_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileJoinedObject *self, DeeObject *args) {
 self->fj_fa = self->fj_fb = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|oo:file.joined",&self->fj_fa,&self->fj_fb) != 0) return -1;
 if (DEE_LIKELY(self->fj_fa) && DEE_UNLIKELY((self->fj_fa = (DeeFileObject *)DeeObject_GetInstance((DeeObject *)self->fj_fa,(DeeTypeObject *)&DeeFile_Type)) == NULL)) return -1;
 if (DEE_LIKELY(self->fj_fb) && DEE_UNLIKELY((self->fj_fb = (DeeFileObject *)DeeObject_GetInstance((DeeObject *)self->fj_fb,(DeeTypeObject *)&DeeFile_Type)) == NULL)) return -1;
 _DeeFile_Init(self);
 if DEE_LIKELY(self->fj_fa) Dee_INCREF(self->fj_fa);
 else self->fj_fa = (DeeFileObject *)DeeFile_Std(DEE_STDNULL);
 if DEE_LIKELY(self->fj_fb) Dee_INCREF(self->fj_fb);
 else self->fj_fb = (DeeFileObject *)DeeFile_Std(DEE_STDNULL);
 return 0;
}
static int _deefilejoined_tp_copy_assign(
 DeeFileJoinedObject *self, DeeFileJoinedObject *right) {
 if DEE_LIKELY(self != right) {
  DeeFileObject *old_fa,*old_fb,*new_fa,*new_fb;
  Dee_uint32_t new_flags;
  DeeFile_ACQUIRE(right);
  new_flags = right->fo_flags;
  Dee_INCREF(new_fa = right->fj_fa);
  Dee_INCREF(new_fb = right->fj_fb);
  DeeFile_RELEASE(right);
  DeeFile_ACQUIRE(self);
  self->fo_flags = new_flags;
  old_fa = self->fj_fa;
  old_fb = self->fj_fb;
  Dee_INHERIT_REF(self->fj_fa,new_fa);
  Dee_INHERIT_REF(self->fj_fb,new_fb);
  DeeFile_RELEASE(self);
  Dee_DECREF(old_fa);
  Dee_DECREF(old_fb);
 }
 return 0;
}
static DeeObject *_deefilejoined_tp_str(DeeFileJoinedObject *self) {
 DeeFileObject *fa,*fb;
 DeeFile_ACQUIRE(self);
 Dee_INCREF(fa = self->fj_fa);
 Dee_INCREF(fb = self->fj_fb);
 DeeFile_RELEASE(self);
 return DeeString_Newf("<file.joined {%K, %K}>",self->fj_fa,self->fj_fb);
}
static DeeObject *_deefilejoined_tp_repr(DeeFileJoinedObject *self) {
 DeeFileObject *fa,*fb;
 DeeFile_ACQUIRE(self);
 Dee_INCREF(fa = self->fj_fa);
 Dee_INCREF(fb = self->fj_fb);
 DeeFile_RELEASE(self);
 return DeeString_Newf("<file.joined {%R, %R}>",self->fj_fa,self->fj_fb);
}
static int _deefilejoined_tp_io_read(
 DeeFileJoinedObject *DEE_UNUSED(self), void *DEE_UNUSED(p),
 Dee_size_t DEE_UNUSED(s), Dee_size_t *DEE_UNUSED(rs)) {
 DeeError_SET_STRING(&DeeErrorType_IOError,"file.joined is a write-only stream");
 return -1;
}
DEE_COMPILER_MSVC_WARNING_PUSH(4701)
DEE_COMPILER_PREFAST_WARNING_PUSH(6001)
static int _deefilejoined_tp_io_write(
 DeeFileJoinedObject *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 Dee_size_t wsa,wsb; DeeFileObject *fa,*fb; int result;
 DeeFile_ACQUIRE(self);
 Dee_INCREF(fa = self->fj_fa);
 Dee_INCREF(fb = self->fj_fb);
 DeeFile_RELEASE(self);
 result = DeeFile_Write((DeeObject *)fa,p,s,&wsa);
 if DEE_LIKELY(result == 0) result = DeeFile_Write((DeeObject *)fb,p,s,&wsb);
 Dee_DECREF(fa);
 Dee_DECREF(fb);
 if DEE_LIKELY(result == 0) *ws = wsa > wsb ? wsa : wsb; // max
 return result;
}
DEE_COMPILER_PREFAST_WARNING_POP
DEE_COMPILER_MSVC_WARNING_POP

DEE_COMPILER_MSVC_WARNING_PUSH(4701)
DEE_COMPILER_PREFAST_WARNING_PUSH(6001)
static int _deefilejoined_tp_io_seek(
 DeeFileJoinedObject *self, Dee_int64_t off, int whence, Dee_uint64_t *pos) {
 Dee_uint64_t posa,posb; DeeFileObject *fa,*fb; int result;
 DeeFile_ACQUIRE(self);
 Dee_INCREF(fa = self->fj_fa);
 Dee_INCREF(fb = self->fj_fb);
 DeeFile_RELEASE(self);
 if (pos) {
  result = DeeFile_Seek((DeeObject *)fa,off,whence,&posa);
  Dee_DECREF(fa);
  if DEE_LIKELY(result == 0) result = DeeFile_Seek((DeeObject *)fb,off,whence,&posb);
  Dee_DECREF(fb);
  if DEE_LIKELY(result == 0) {
   if DEE_UNLIKELY(posa != posb) {
    DeeError_SetStringf(&DeeErrorType_IOError,
                        "Can't determinate file.joined position (%I64u, %I64u)",
                        posa,posb);
    result = -1;
   } else {
    *pos = posa;
    result = 0;
   }
  }
 } else {
  result = DeeFile_Seek((DeeObject *)fa,off,whence,&posa);
  Dee_DECREF(fa);
  if DEE_LIKELY(result == 0) result = DeeFile_Seek((DeeObject *)fb,off,whence,&posb);
  Dee_DECREF(fb);
 }
 Dee_DECREF(fb);
 Dee_DECREF(fa);
 return result;
}
DEE_COMPILER_PREFAST_WARNING_POP
DEE_COMPILER_MSVC_WARNING_POP

static int _deefilejoined_tp_io_flush(DeeFileJoinedObject *self) {
 DeeFileObject *fa,*fb; int result;
 DeeFile_ACQUIRE(self);
 Dee_INCREF(fa = self->fj_fa);
 Dee_INCREF(fb = self->fj_fb);
 DeeFile_RELEASE(self);
 result = DeeFile_Flush((DeeObject *)self->fj_fa);
 Dee_DECREF(fa);
 if DEE_LIKELY(result == 0) result = DeeFile_Flush((DeeObject *)self->fj_fb);
 Dee_DECREF(fb);
 return result;
}
static int _deefilejoined_tp_io_trunc(DeeFileJoinedObject *self) {
 DeeFileObject *fa,*fb; int result;
 DeeFile_ACQUIRE(self);
 Dee_INCREF(fa = self->fj_fa);
 Dee_INCREF(fb = self->fj_fb);
 DeeFile_RELEASE(self);
 result = DeeFile_Trunc((DeeObject *)self->fj_fa);
 Dee_DECREF(fa);
 if DEE_LIKELY(result == 0) result = DeeFile_Trunc((DeeObject *)self->fj_fb);
 Dee_DECREF(fb);
 return result;
}
static void _deefilejoined_tp_io_close(DeeFileJoinedObject *self) {
 DeeFileObject *old_fa,*old_fb;
 DeeFile_ACQUIRE(self);
 Dee_INHERIT_REF(old_fa,self->fj_fa);
 Dee_INHERIT_REF(old_fb,self->fj_fb);
 self->fj_fa = (DeeFileObject *)DeeFile_Std(DEE_STDNULL);
 Dee_INCREF(self->fj_fb = self->fj_fa);
 DeeFile_RELEASE(self);
 Dee_DECREF(old_fa);
 Dee_DECREF(old_fb);
}
static int DEE_CALL _deefilejoined_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileJoinedObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 if DEE_UNLIKELY((self->fj_fa = (DeeFileObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if DEE_UNLIKELY(DeeError_TypeError_CheckType((DeeObject *)self->fj_fa,(DeeTypeObject *)&DeeFile_Type) != 0) {err_fa: Dee_DECREF(self->fj_fa); return -1; }
 if DEE_UNLIKELY((self->fj_fb = (DeeFileObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) goto err_fa;
 if DEE_UNLIKELY(DeeError_TypeError_CheckType((DeeObject *)self->fj_fb,(DeeTypeObject *)&DeeFile_Type) != 0) {/*err_fb:*/ Dee_DECREF(self->fj_fb); goto err_fa; }
 _DeeFile_Init(self);
 return 0;
}
static int DEE_CALL _deefilejoined_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileJoinedObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 if (DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)self->fj_fa) != 0) return -1;
 return DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)self->fj_fb);
}
static struct DeeTypeMarshal _deefilejoined_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(file_joined)),
 member(&_deefilejoined_tp_marshal_ctor),member(&_deefilejoined_tp_marshal_put));







static DeeObject *_deefile_read(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeObject *data = NULL,*size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|oo:read",&data,&size) != 0) return NULL;
 if (data == NULL) {
  return DeeFile_ReadData(self,(Dee_size_t)-1);
 } else if (size == NULL) {
  Dee_size_t max_read;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,data,&max_read) != 0) return NULL;
  return DeeFile_ReadData(self,max_read);
 } else {
  void *p; Dee_size_t s;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0 || 
                  DeeObject_Cast(Dee_size_t,size,&s) != 0) return NULL;
  if DEE_UNLIKELY(DeeFile_ReadAll(self,p,s) != 0) return NULL;
  DeeReturn_None;
 }
#else
 DeeObject *size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:read",&size) != 0) return NULL;
 if (size == NULL) {
  return DeeFile_ReadData(self,(Dee_size_t)-1);
 } else {
  Dee_size_t max_read;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&max_read) != 0) return NULL;
  return DeeFile_ReadData(self,max_read);
 }
#endif
}
static DeeObject *_deefile_readall(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeObject *data = NULL,*size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|oo:readall",&data,&size) != 0) return NULL;
 if (data == NULL) {
  return DeeFile_ReadAllData(self,(Dee_size_t)-1);
 } else if (size == NULL) {
  Dee_size_t max_read;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,data,&max_read) != 0) return NULL;
  return DeeFile_ReadAllData(self,max_read);
 } else {
  void *p; Dee_size_t s;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0 ||
                  DeeObject_Cast(Dee_size_t,size,&s) != 0) return NULL;
  if DEE_UNLIKELY(DeeFile_ReadAll(self,p,s) != 0) return NULL;
  DeeReturn_None;
 }
#else
 DeeObject *size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:readall",&size) != 0) return NULL;
 if (size == NULL) {
  return DeeFile_ReadAllData(self,(Dee_size_t)-1);
 } else {
  Dee_size_t max_read;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&max_read) != 0) return NULL;
  return DeeFile_ReadAllData(self,max_read);
 }
#endif
}
static DeeObject *_deefile_maybe_read(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeObject *data = NULL,*size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|oo:maybe_read",&data,&size) != 0) return NULL;
 if (data == NULL) {
  return DeeFile_ReadData(self,(Dee_size_t)-1);
 } else if (size == NULL) {
  Dee_size_t max_read;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,data,&max_read) != 0) return NULL;
  return DeeFile_ReadData(self,max_read);
 } else {
  void *p; Dee_size_t s,result;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0 ||
                  DeeObject_Cast(Dee_size_t,size,&s) != 0) return NULL;
  if DEE_UNLIKELY(DeeFile_Read(self,p,s,&result) != 0) return NULL;
  return DeeObject_New(Dee_size_t,result);
 }
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DeeObject *size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:maybe_read",&size) != 0) return NULL;
 if (size == NULL) {
  return DeeFile_ReadData(self,(Dee_size_t)-1);
 } else {
  Dee_size_t max_read;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&max_read) != 0) return NULL;
  return DeeFile_ReadData(self,max_read);
 }
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
}
static DeeObject *_deefile_readat(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 Dee_uint64_t pos; DeeObject *data = NULL,*size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64u|oo:readat",&pos,&data,&size) != 0) return NULL;
 if (data == NULL) {
  return DeeFile_ReadData(self,(Dee_size_t)-1);
 } else if (size == NULL) {
  Dee_size_t max_read;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,data,&max_read) != 0) return NULL;
  return DeeFile_ReadDataAt(self,pos,max_read);
 } else {
  void *p; Dee_size_t s;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0 || 
                  DeeObject_Cast(Dee_size_t,size,&s) != 0) return NULL;
  if DEE_UNLIKELY(DeeFile_ReadAllAt(self,pos,p,s) != 0) return NULL;
  DeeReturn_None;
 }
#else
 Dee_uint64_t pos; DeeObject *size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64u|o:read",&pos,&size) != 0) return NULL;
 if (size == NULL) {
  return DeeFile_ReadDataAt(self,pos,(Dee_size_t)-1);
 } else {
  Dee_size_t max_read;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&max_read) != 0) return NULL;
  return DeeFile_ReadDataAt(self,pos,max_read);
 }
#endif
}
static DeeObject *_deefile_readallat(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 Dee_uint64_t pos; DeeObject *data = NULL,*size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64u|oo:readallat",&pos,&data,&size) != 0) return NULL;
 if (data == NULL) {
  return DeeFile_ReadAllDataAt(self,pos,(Dee_size_t)-1);
 } else if (size == NULL) {
  Dee_size_t max_read;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,data,&max_read) != 0) return NULL;
  return DeeFile_ReadAllDataAt(self,pos,max_read);
 } else {
  void *p; Dee_size_t s;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0 ||
                  DeeObject_Cast(Dee_size_t,size,&s) != 0) return NULL;
  if DEE_UNLIKELY(DeeFile_ReadAllAt(self,pos,p,s) != 0) return NULL;
  DeeReturn_None;
 }
#else
 Dee_uint64_t pos; DeeObject *size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64u|o:readall",&pos,&size) != 0) return NULL;
 if (size == NULL) {
  return DeeFile_ReadAllDataAt(self,pos,(Dee_size_t)-1);
 } else {
  Dee_size_t max_read;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&max_read) != 0) return NULL;
  return DeeFile_ReadAllDataAt(self,pos,max_read);
 }
#endif
}
static DeeObject *_deefile_maybe_readat(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 Dee_uint64_t pos; DeeObject *data = NULL,*size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64u|oo:maybe_readat",&pos,&data,&size) != 0) return NULL;
 if (data == NULL) {
  return DeeFile_ReadDataAt(self,pos,(Dee_size_t)-1);
 } else if (size == NULL) {
  Dee_size_t max_read;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,data,&max_read) != 0) return NULL;
  return DeeFile_ReadDataAt(self,pos,max_read);
 } else {
  void *p; Dee_size_t s,result;
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0 ||
                  DeeObject_Cast(Dee_size_t,size,&s) != 0) return NULL;
  if DEE_UNLIKELY(DeeFile_ReadAt(self,pos,p,s,&result) != 0) return NULL;
  return DeeObject_New(Dee_size_t,result);
 }
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 Dee_uint64_t pos; DeeObject *size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64u|o:maybe_read",&pos,&size) != 0) return NULL;
 if (size == NULL) {
  return DeeFile_ReadDataAt(self,pos,(Dee_size_t)-1);
 } else {
  Dee_size_t max_read;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&max_read) != 0) return NULL;
  return DeeFile_ReadDataAt(self,pos,max_read);
 }
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
}
static DeeObject *_deefile_write(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *data,*size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:write",&data,&size) != 0) return NULL;
 if (size) {
  void *p; Dee_size_t s,max_size;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&s) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  if (!DeeString_Check(data)) {
   if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0) return NULL;
  } else
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  {
   p = DeeString_STR(data);
   if ((max_size = DeeString_SIZE(data)) > s) s = max_size;
  }
  if DEE_UNLIKELY(DeeFile_WriteAll(self,p,s) != 0) return NULL;
 } else {
  if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) return NULL;
  if DEE_UNLIKELY(DeeFile_WriteAll(self,DeeString_STR(data),DeeString_SIZE(data)) != 0) return NULL;
 }
 DeeReturn_None;
}
static DeeObject *_deefile_maybe_write(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *data,*size = NULL; Dee_size_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:maybe_write",&data,&size) != 0) return NULL;
 if (size) {
  void *p; Dee_size_t s,max_size;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&s) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  if (!DeeString_Check(data)) {
   if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0) return NULL;
  } else
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  {
   p = DeeString_STR(data);
   if ((max_size = DeeString_SIZE(data)) > s) s = max_size;
  }
  if DEE_UNLIKELY(DeeFile_Write(self,p,s,&result) != 0) return NULL;
 } else {
  if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) return NULL;
  if DEE_UNLIKELY(DeeFile_Write(self,DeeString_STR(data),DeeString_SIZE(data),&result) != 0) return NULL;
 }
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *_deefile_writeat(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint64_t pos; DeeObject *data,*size = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64uo|o:write",&pos,&data,&size) != 0) return NULL;
 if (size) {
  void *p; Dee_size_t s,max_size;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&s) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  if (!DeeString_Check(data)) {
   if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0) return NULL;
  } else
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  {
   p = DeeString_STR(data);
   if ((max_size = DeeString_SIZE(data)) > s) s = max_size;
  }
  if DEE_UNLIKELY(DeeFile_WriteAllAt(self,pos,p,s) != 0) return NULL;
 } else {
  if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) return NULL;
  if DEE_UNLIKELY(DeeFile_WriteAllAt(self,pos,DeeString_STR(data),DeeString_SIZE(data)) != 0) return NULL;
 }
 DeeReturn_None;
}
static DeeObject *_deefile_maybe_writeat(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint64_t pos; DeeObject *data,*size = NULL; Dee_size_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64uo|o:maybe_writeat",&pos,&data,&size) != 0) return NULL;
 if (size) {
  void *p; Dee_size_t s,max_size;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&s) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  if (!DeeString_Check(data)) {
   if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0) return NULL;
  } else
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  {
   p = DeeString_STR(data);
   if ((max_size = DeeString_SIZE(data)) > s) s = max_size;
  }
  if DEE_UNLIKELY(DeeFile_WriteAt(self,pos,p,s,&result) != 0) return NULL;
 } else {
  if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) return NULL;
  if DEE_UNLIKELY(DeeFile_WriteAt(self,pos,DeeString_STR(data),DeeString_SIZE(data),&result) != 0) return NULL;
 }
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *_deefile_reado(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeTypeObject *tp = DeeObject_TYPE(int);
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:reado",&tp) != 0) return NULL;
 return DeeFile_ReadStructuredObject(self,tp);
}
static DeeObject *_deefile_writeo(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject **iter,**end;
 end = (iter = DeeTuple_ELEM(args))+DeeTuple_SIZE(args);
 while DEE_LIKELY(iter != end) {
  if DEE_UNLIKELY(DeeFile_WriteStructuredObject(self,*iter) != 0) return NULL;
  ++iter;
 }
 DeeReturn_None;
}
static DeeObject *_deefile_readm(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeMarshalVersion ver = DEE_MARSHAL_VERSION;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|I16u:readm",&ver) != 0) return NULL;
 return DeeMarshal_ReadObject(self,ver);
}
static DeeObject *_deefile_writem(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *ob; Dee_uint32_t flags = DEE_MARSHAL_WRITEFLAG_DEFAULT;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|I32u:writem",&ob,&flags) != 0) return NULL;
 if DEE_UNLIKELY(DeeMarshal_WriteObjectEx(self,ob,flags) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deefile_readline(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t max_read = (Dee_size_t)-1;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|Iu:readline",&max_read) != 0) return NULL;
 return DeeFile_ReadLine(self,max_read);
}
static DeeObject *_deefile_close(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":close") != 0) return NULL;
 DeeFile_Close(self);
 DeeReturn_None;
}
static DeeObject *_deefile_seek(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint64_t result; Dee_int64_t off; int whence = DEE_SEEK_SET;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64d|d:seek",&off,&whence) != 0) return NULL;
 if DEE_UNLIKELY(DeeFile_Seek(self,off,whence,&result) != 0) return NULL;
 return DeeUInt64_New(result);
}
static DeeObject *_deefile_setpos(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint64_t pos;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64u:setpos",&pos) != 0) return NULL;
 if DEE_UNLIKELY(DeeFile_SetPos(self,pos) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deefile_rewind(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":rewind") != 0) return NULL;
 if DEE_UNLIKELY(DeeFile_Rewind(self) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deefile_trunc(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":trunc") != 0) return NULL;
 if DEE_UNLIKELY(DeeFile_Trunc(self) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deefile_flush(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":flush") != 0) return NULL;
 if DEE_UNLIKELY(DeeFile_Flush(self) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deefile_tell(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint64_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":tell") != 0) return NULL;
 if DEE_UNLIKELY(DeeFile_Tell(self,&result) != 0) return NULL;
 return DeeUInt64_New(result);
}
static DeeObject *_deefile_size(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint64_t fp_size;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":size") != 0) return NULL;
 if DEE_UNLIKELY(DeeFile_Size(self,&fp_size) != 0) return NULL;
 return DeeUInt64_New(fp_size);
}
static DeeObject *_deefile_getc(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint8_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":getc") != 0) return NULL;
 if DEE_UNLIKELY(DeeFile_Getc(self,&result) != 0) return NULL;
 return DeeObject_New(Dee_uint8_t,result);
}
static DeeObject *_deefile_putc(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint8_t c;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I8u:putc",&c) != 0) return NULL;
 if DEE_UNLIKELY(DeeFile_Putc(self,c) != 0) return NULL;
 DeeReturn_None;
}
DEE_COMPILER_PREFAST_WARNING_PUSH(6011 6054)
static DeeObject *_deefile_puts(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 char const *s;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"s:puts",&s) != 0) return NULL;
 if DEE_UNLIKELY(DeeFile_Puts(self,s ? s : "") != 0) return NULL;
 DeeReturn_None;
}
DEE_COMPILER_PREFAST_WARNING_POP



#if DEE_PLATFORM_HAVE_IO_HANDLE
static DeeObject *_deefileio_fileno(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":fileno") != 0) return NULL;
#if defined(DEE_PLATFORM_WINDOWS)
 return DeeObject_New(Dee_uintptr_t,(Dee_uintptr_t)DeeFileIO_HANDLE(self));
#elif defined(DEE_PLATFORM_UNIX)
 return DeeObject_New(int,DeeFileIO_HANDLE(self));
#endif
}
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
static DeeObject *_deefileio_size(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint64_t fp_size;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":size") != 0) return NULL;
 if DEE_UNLIKELY(DeeFileIO_Size((DeeObject *)self,&fp_size) != 0) return NULL;
 return DeeUInt64_New(fp_size);
}
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
 return DeeFileIO_NewTemporary(DEE_LIKELY(delete_when_closed)
  ? DEE_FILEIO_NEWTEMPORARY_FLAG_DELETE_WHEN_CLOSED
  : DEE_FILEIO_NEWTEMPORARY_FLAG_NONE);
}
#endif /* DEE_PLATFORM_HAVE_IO_HANDLE */

static DeeObject *_deefilewriter_pack(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeTypeObject *string_type = &DeeUtf8String_Type;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:pack",&string_type) != 0) return NULL;
 if DEE_UNLIKELY((string_type = (DeeTypeObject *)DeeObject_GetInstance((
  DeeObject *)string_type,&DeeType_Type)) == NULL) return NULL;
 if DEE_UNLIKELY(1
#if DEE_CONFIG_HAVE_ENCODING_WIDE
  && string_type != &DeeWideString_Type
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
  && string_type != &DeeUtf8String_Type
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
  && string_type != &DeeUtf16String_Type
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
  && string_type != &DeeUtf32String_Type
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
  ) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Expected string type (got: %k)",
                      string_type);
  return NULL;
 }
 return DeeFileWriter_PackEx(self,string_type);
}

#if DEE_PLATFORM_HAVE_IO
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
#endif

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deefile_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deefile_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__fo_lock",DeeFileObject,fo_lock,DeeAtomicMutex),
 DEE_MEMBERDEF_NAMED_RO_v100("__fo_flags",DeeFileObject,fo_flags,Dee_uint32_t),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */

// Methods available in all classes derived from 'DeeFile_Type'
static struct DeeMethodDef const _deefile_tp_methods[] = {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DEE_METHODDEF_v100("read",member(&_deefile_read),
  "(size_t max = size_t(-1)) -> string\n"
  "(none *p, size_t size) -> none\n"
  "@throws object: Error occurred in implementation of #(this.operator __read__)\n"
  "@return: The read data, or none\n"
  "\tReads @size bytes into @p or at most @max bytes from a stream and returns the read data as a string.\n"),
 DEE_METHODDEF_v100("readall",member(&_deefile_readall),
  "(size_t max = size_t(-1)) -> string\n"
  "(none *p, size_t size) -> none\n"
  "@throws object: Error occurred in implementation of #(this.operator __read__)\n"
  "@return: The read data, or none\n"
  "\tReads @size bytes into @p, or at most @max bytes from a stream and returns the read data as a string.\n"
  "\tSimilar to #(file.read), but keeps trying if data can only be read a piece at a time"),
 DEE_METHODDEF_v100("write",member(&_deefile_write),
  "(string s) -> none\n"
  "(none *p, size_t size) -> none\n"
  "@throws object: Error occurred in implementation of #(this.operator __write__)\n"
  "@throws Error.IOError: Not all data could be written.\n"
  "\tWrites @s to the stream.\n"
  "\tSame as #(file.maybe_write), but retires until no more data is written and raises an #(Error.IOError)"),
 DEE_METHODDEF_v100("maybe_read",member(&_deefile_maybe_read),
  "(size_t max = size_t(-1)) -> string\n"
  "(none *p, size_t size) -> size_t\n"
  "@throws object: Error occurred in implementation of #(this.operator __read__)\n"
  "\tReads at most @max bytes from a stream, returning actual amount of bytes read."),
 DEE_METHODDEF_v100("maybe_write",member(&_deefile_maybe_write),
  "(string s) -> size_t\n"
  "(none *p, size_t size) -> size_t\n"
  "@throws object: Error occurred in implementation of #(this.operator __write__)\n"
  "\tWrites @s or @size bytes starting at @p to the stream, returning the actual amount of bytes written."),
 DEE_METHODDEF_v100("readat",member(&_deefile_readat),
  "(uint64_t pos, size_t max = size_t(-1)) -> string\n"
  "(uint64_t pos, none *p, size_t size) -> none\n"
  "@throws object: Error occurred in implementation of #(this.operator __readat__)\n"
  "@return: The read data, or none\n"
  "\tReads @size bytes into @p or at most @max bytes from a stream at the given offset @pos and returns the read data as a string.\n"),
 DEE_METHODDEF_v100("readallat",member(&_deefile_readallat),
  "(uint64_t pos, size_t max = size_t(-1)) -> string\n"
  "(uint64_t pos, none *p, size_t size) -> none\n"
  "@throws object: Error occurred in implementation of #(this.operator __readat__)\n"
  "@return: The read data, or none\n"
  "\tReads @size bytes into @p, or at most @max bytes from a stream at the given offset @pos and returns the read data as a string.\n"
  "\tSimilar to #(file.readat), but keeps trying if data can only be read a piece at a time"),
 DEE_METHODDEF_v100("writeat",member(&_deefile_writeat),
  "(uint64_t pos, string s) -> none\n"
  "(uint64_t pos, none *p, size_t size) -> none\n"
  "@throws object: Error occurred in implementation of #(this.operator __writeat__)\n"
  "@throws Error.IOError: Not all data could be written.\n"
  "\tWrites @s to the stream at the given offset @(pos).\n"
  "\tSame as #(file.maybe_writeat), but retires until no more data is written and raises an #(Error.IOError)"),
 DEE_METHODDEF_v100("maybe_readat",member(&_deefile_maybe_readat),
  "(uint64_t pos, size_t max = size_t(-1)) -> string\n"
  "(uint64_t pos, none *p, size_t size) -> size_t\n"
  "@throws object: Error occurred in implementation of #(this.operator __readat__)\n"
  "\tReads at most @max bytes from a stream at the given offset @(pos), returning actual amount of bytes read."),
 DEE_METHODDEF_v100("maybe_writeat",member(&_deefile_maybe_writeat),
  "(uint64_t pos, string s) -> size_t\n"
  "(uint64_t pos, none *p, size_t size) -> size_t\n"
  "@throws object: Error occurred in implementation of #(this.operator __writeat__)\n"
  "\tWrites @s or @size bytes starting at @p to the stream at the given offset @(pos), returning the actual amount of bytes written."),
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DEE_METHODDEF_v100("read",member(&_deefile_read),"(size_t max = size_t(-1)) -> string\n"
  "@throws object: Error occurred in implementation of #(this.operator __read__)\n"
  "\tReads at most @max bytes from a stream."),
 DEE_METHODDEF_v100("readall",member(&_deefile_readall),"(size_t max = size_t(-1)) -> string\n"
  "@throws object: Error occurred in implementation of #(this.operator __read__)\n"
  "\tSimilar to #(file.read), but keeps trying if data can only be read a piece at a time"),
 DEE_METHODDEF_v100("write",member(&_deefile_write),"(string s) -> none\n"
  "@throws object: Error occurred in implementation of #(this.operator __write__)\n"
  "@throws Error.IOError: Not all data could be written.\n"
  "\tWrites @s to the stream.\n"
  "\tSame as maybe_write, but retires until no more data is written and raises an #(Error.IOError)."),
 DEE_METHODDEF_v100("maybe_read",member(&_deefile_maybe_read),"(size_t max = size_t(-1)) -> string\n"
  "@throws object: Error occurred in implementation of #(this.operator __read__)\n"
  "\tReads at most @max bytes from a stream."),
 DEE_METHODDEF_v100("maybe_write",member(&_deefile_maybe_write),"(string s) -> size_t\n"
  "@throws object: Error occurred in implementation of #(this.operator __write__)\n"
  "\tWrites @s to the stream, but returns the actual amount of bytes written"),
 DEE_METHODDEF_v100("readat",member(&_deefile_readat),"(uint64_t pos, size_t max = size_t(-1)) -> string\n"
  "@throws object: Error occurred in implementation of #(this.operator __readat__)\n"
  "\tReads at most @max bytes from a stream at the given offset @(pos)."),
 DEE_METHODDEF_v100("readallat",member(&_deefile_readallat),"(uint64_t pos, size_t max = size_t(-1)) -> string\n"
  "@throws object: Error occurred in implementation of #(this.operator __readat__)\n"
  "\tSimilar to #(file.readat), but keeps trying if data can only be read a piece at a time"),
 DEE_METHODDEF_v100("writeat",member(&_deefile_writeat),"(uint64_t pos, string s) -> none\n"
  "@throws object: Error occurred in implementation of #(this.operator __writeat__)\n"
  "@throws Error.IOError: Not all data could be written.\n"
  "\tWrites @s to the stream at the given offset @(pos).\n"
  "\tSame as maybe_write, but retires until no more data is written and raises an #(Error.IOError)."),
 DEE_METHODDEF_v100("maybe_readat",member(&_deefile_maybe_readat),"(uint64_t pos, size_t max = size_t(-1)) -> string\n"
  "@throws object: Error occurred in implementation of #(this.operator __readat__)\n"
  "\tReads at most @max bytes from a stream at the given offset @(pos)."),
 DEE_METHODDEF_v100("maybe_writeat",member(&_deefile_maybe_writeat),"(uint64_t pos, string s) -> size_t\n"
  "@throws object: Error occurred in implementation of #(this.operator __writeat__)\n"
  "\tWrites @s to the stream at the given offset @(pos), but returns the actual amount of bytes written"),
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DEE_METHODDEF_v100("reado",member(&_deefile_reado),"(type object ty = int) -> ty\n"
  "\tReads a structured object from binary data and returns it.\n"
  "\tThrows an Error.TypeError if @ty is not a structured type"),
 DEE_METHODDEF_v100("writeo",member(&_deefile_writeo),"(object args...) -> none\n"
  "\tWrites all arguments in sequencial order, which must be structured objects, as binary data.\n"
  "\tThrows an Error.TypeError if any argument isn't a structured object"),
 DEE_METHODDEF_v100("readm",member(&_deefile_readm),"(uint16_t version = file.MARSHAL_VERSION) -> object\n"
  "\tReads a marshalled object from the file and returns it"),
 DEE_METHODDEF_v100("writem",member(&_deefile_writem),"(object ob) -> none\n"
  "\tWrites a mashallable object to the file"),
 DEE_METHODDEF_v100("readline",member(&_deefile_readline),"(size_t max = size_t(-1)) -> string\n"
  "\tReads at most @max bytes from a stream or stops if a line end was encountered"),
 DEE_METHODDEF_v100("seek",member(&_deefile_seek),"(int64_t off, int whence = file.SEEK_SET) -> uint64_t\n"
  "\tSets the r/w pointer position within a file and returns the new position from the SEEK_SET"),
 DEE_METHODDEF_v100("setpos",member(&_deefile_setpos),"(uint64_t pos) -> none\n"
  "\tSets the r/w pointer position within a file"),
 DEE_METHODDEF_v100("rewind",member(&_deefile_rewind),"() -> none\n"
  "\tPlaces the r/w pointer at the beginning of the file"),
 DEE_METHODDEF_v100("trunc",member(&_deefile_trunc),"() -> none\n"
  "\tTruncates the file to end at the current r/w pointer position"),
 DEE_METHODDEF_v100("tell",member(&_deefile_tell),"() -> uint64_t\n"
  "\tReturns the current position of the r/w pointer position\n"
  "\tSame as calling #(file.seek(0,file.SEEK_CUR))"),
 DEE_METHODDEF_v100("size",member(&_deefile_size),"() -> uint64_t\n"
  "\tReturns the size of the file"),
 DEE_METHODDEF_v100("flush",member(&_deefile_flush),"() -> none\n"
  "\tFlushes any unwritten data onto the disk and releases unused buffers.\n"
  "\tMay be a no-op"),
 DEE_METHODDEF_v100("close",member(&_deefile_close),"() -> none\n"
  "\tCloses the file object\n"
  "\tMay be a no-op, but if it isn't, any subsequence operations throw an Error"),
 DEE_METHODDEF_v100("getc",member(&_deefile_getc),"() -> uint8_t\n"
  "\tReads one byte and returns it"),
 DEE_METHODDEF_v100("putc",member(&_deefile_putc),"(uint8_t ch) -> none\n"
  "\tWrites one byte"),
 DEE_METHODDEF_v100("puts",member(&_deefile_puts),"(char const *s) -> none\n"
  "\tWrites a string and a linefeed"),
 DEE_METHODDEF_END_v100
};

static struct DeeMemberDef const _deefile_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("SEEK_SET",int,DEE_SEEK_SET),
 DEE_MEMBERDEF_CONST_v100("SEEK_CUR",int,DEE_SEEK_CUR),
 DEE_MEMBERDEF_CONST_v100("SEEK_END",int,DEE_SEEK_END),
#if DEE_PLATFORM_HAVE_IO
 DEE_MEMBERDEF_CONST_v100("io",object,&DeeFileIO_Type),
#else
 DEE_MEMBERDEF_CONST_v100("io",object,none),
#endif
 DEE_MEMBERDEF_CONST_v100("reader",object,&DeeFileReader_Type),
 DEE_MEMBERDEF_CONST_v100("writer",object,&DeeFileWriter_Type),
 DEE_MEMBERDEF_CONST_v100("joined",object,&DeeFileJoined_Type),
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeFileIterator_Type),
 DEE_MEMBERDEF_CONST_v100("MARSHAL_VERSION",Dee_uint16_t,DEE_MARSHAL_VERSION),
 DEE_MEMBERDEF_END_v100
};

#if DEE_PLATFORM_HAVE_IO
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
#endif

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


#if DEE_PLATFORM_HAVE_IO
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
#endif

static struct DeeMethodDef const _deefilewriter_tp_methods[] = {
 DEE_METHODDEF_CONST_v100("pack",member(&_deefilewriter_pack),
  "(type object string_type = string) -> string_type\n"
  "@return: The generated string as an instance of @string_type\n"
  "\tThis function may clear all previously written data after the string is generated."),
 DEE_METHODDEF_END_v100
};
#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deefilewriter_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deefilewriter_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__fw_pos",DeeFileWriterObject,fw_pos,p(char)),
 DEE_MEMBERDEF_NAMED_RO_v100("__fw_writer_sw_str",DeeFileWriterObject,fw_writer.sw_str,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__fw_writer_sw_len",DeeFileWriterObject,fw_writer.sw_len,Dee_size_t),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */

static struct DeeMemberDef const _deefilereader_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("data",DeeFileReaderObject,fr_string,object),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__fr_string",DeeFileReaderObject,fr_string,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__fr_pos",DeeFileReaderObject,fr_pos,p1(Dee_uint8_t)),
 DEE_MEMBERDEF_NAMED_RO_v100("__fr_end",DeeFileReaderObject,fr_end,p1(Dee_uint8_t)),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};



static void _DeeFileType_InitCommon(DeeFileTypeObject *self) {
 self->tp_io.tp_io_read = NULL;
 self->tp_io.tp_io_write = NULL;
 self->tp_io.tp_io_seek = NULL;
 self->tp_io.tp_io_flush = NULL;
 self->tp_io.tp_io_trunc = NULL;
 self->tp_io.tp_io_close = NULL;
}
static int _deefiletype_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileTypeObject *self) {
 _DeeFileType_InitCommon(self);
 return (*DeeType_GET_SLOT(&DeeType_Type,tp_ctor))(&DeeType_Type,(DeeObject *)self);
}
static int _deefiletype_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileTypeObject *self, DeeFileTypeObject *right) {
 memcpy(&self->tp_io,&right->tp_io,sizeof(struct _DeeFileTypeIOOperators));
 return (*DeeType_GET_SLOT(&DeeType_Type,tp_copy_ctor))(
  &DeeType_Type,(DeeObject *)self,(DeeObject *)right);
}
static int _deefiletype_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileTypeObject *self, DeeFileTypeObject *right) {
 memcpy(&self->tp_io,&right->tp_io,sizeof(struct _DeeFileTypeIOOperators));
 return (*DeeType_GET_SLOT(&DeeType_Type,tp_move_ctor))(
  &DeeType_Type,(DeeObject *)self,(DeeObject *)right);
}
static int _deefiletype_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileTypeObject *self, DeeObject *args) {
 _DeeFileType_InitCommon(self);
 return (*DeeType_GET_SLOT(&DeeType_Type,tp_any_ctor))(&DeeType_Type,(DeeObject *)self,args);
}

extern int DEE_CALL _deefiletype_tp_marshal_ctor(
 DeeTypeObject *tp_self, DeeFileTypeObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 if DEE_UNLIKELY(_deetype_tp_marshal_ctor(tp_self,(DeeTypeObject *)self,infile,map) != 0) return -1;
 _DeeFileType_InitCommon(self);
 return 0;
}

static struct DeeTypeMarshal _deefiletype_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALID_NONE),
 member(&_deefiletype_tp_marshal_ctor),member(&_deetype_tp_marshal_put));


#if DEE_XCONFIG_HAVE_FILE_TYPE_SLOT_MEMBERS
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DEE_FILETYPESLOT_GETSET(name,tystring) \
static DeeObject *_deefiletype_##name##_get(DeeFileTypeObject *self, void *DEE_UNUSED(closure))\
{\
 return DeePointer_Newf((void *)DeeType_GET_SLOT(self,name),tystring);\
}\
static int _deefiletype_##name##_del(DeeFileTypeObject *self, void *DEE_UNUSED(closure))\
{\
 DeeType_DEL_SLOT(self,name);\
 return 0;\
}\
static int _deefiletype_##name##_set(DeeFileTypeObject *self, DeeObject *v, void *DEE_UNUSED(closure))\
{\
 DeeType_SLOT_TYPE(name) new_v;\
 if DEE_UNLIKELY(DeeObject_GetPointerExf(v,(void **)&new_v,tystring) != 0) return -1;\
 DeeType_GET_SLOT(self,name) = new_v;\
 return 0;\
}
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
DEE_COMPILER_MSVC_WARNING_PUSH(4052 4054)
DEE_FILETYPESLOT_GETSET(tp_io_read,"d(opII*)")
DEE_FILETYPESLOT_GETSET(tp_io_write,"d(opII*)")
DEE_FILETYPESLOT_GETSET(tp_io_seek,"d(oI64ddI64u*)")
DEE_FILETYPESLOT_GETSET(tp_io_flush,"d(o)")
DEE_FILETYPESLOT_GETSET(tp_io_trunc,"d(o)")
DEE_FILETYPESLOT_GETSET(tp_io_close,"v(o)")
DEE_COMPILER_MSVC_WARNING_POP
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#undef DEE_FILETYPESLOT_GETSET
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#endif /* DEE_XCONFIG_HAVE_FILE_TYPE_SLOT_MEMBERS */

#if DEE_XCONFIG_HAVE_FILE_TYPE_SLOT_MEMBERS
static struct DeeGetSetDef _deefiletype_tp_getsets[] = {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DEE_FILETYPESLOT_GETSET_REF(name,tydoc)\
 DEE_GETSETDEF_v100("__" #name,\
  member(&_deefiletype_##name##_get),\
  member(&_deefiletype_##name##_del),\
  member(&_deefiletype_##name##_set),\
  "-> " tydoc)
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
 DEE_FILETYPESLOT_GETSET_REF(tp_io_read,"int(*)(object,none *,size_t,size_t *)"),
 DEE_FILETYPESLOT_GETSET_REF(tp_io_write,"int(*)(object,none *,size_t,size_t *)"),
 DEE_FILETYPESLOT_GETSET_REF(tp_io_seek,"int(*)(object,int64_t,int,uint64_t *)"),
 DEE_FILETYPESLOT_GETSET_REF(tp_io_flush,"int(*)(object)"),
 DEE_FILETYPESLOT_GETSET_REF(tp_io_trunc,"int(*)(object)"),
 DEE_FILETYPESLOT_GETSET_REF(tp_io_close,"none(*)(object)"),
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#undef DEE_FILETYPESLOT_GETSET_REF
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DEE_GETSETDEF_END_v100
};
#else /* DEE_XCONFIG_HAVE_FILE_TYPE_SLOT_MEMBERS */
#define _deefiletype_tp_getsets DeeType_DEFAULT_SLOT(tp_getsets)
#endif /* !DEE_XCONFIG_HAVE_FILE_TYPE_SLOT_MEMBERS */


DeeTypeObject DeeFileType_Type = {
 // Type for the file type and all its derived types
 // NOTE: This is a type_type that is simply required
 //       because it adds more virtual operators!
 DEE_TYPE_OBJECT_HEAD_v100(member("file_type"),member(
  "(...) -> file_type\n"
  "\nType for all file types.\n"
  "\tInherits from #type and adds the r/w functions used by files."),null,member(&DeeType_Type)),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFileTypeObject),null,
  member(&_deefiletype_tp_ctor),
  member(&_deefiletype_tp_copy_ctor),
  member(&_deefiletype_tp_move_ctor),
  member(&_deefiletype_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,member(_deefiletype_tp_getsets),null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deefiletype_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DeeFileTypeObject DeeFile_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(member(&DeeFileType_Type),
   member("file"),member("() -> file\n"
    "\tBase class for all file steam types"),null,null),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFileObject),null,
   member(&_deefile_tp_ctor),
   member(&_deefile_tp_copy_ctor),null,null),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(&_deefile_tp_copy_assign),null,null),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&_deefile_tp_str),null,null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
  DEE_TYPE_OBJECT_MATH_v101(
   null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,
   member(&_deefile_tp_shl),null,null,null,null,null,
   member(&_deefile_tp_or),null,null,null,null,null,
   member(&_deegenericsameob_tp_hash)),
  DEE_TYPE_OBJECT_COMPARE_v100(
   member(&_deegenericsameob_tp_cmp_lo),
   member(&_deegenericsameob_tp_cmp_le),
   member(&_deegenericsameob_tp_cmp_eq),
   member(&_deegenericsameob_tp_cmp_ne),
   member(&_deegenericsameob_tp_cmp_gr),
   member(&_deegenericsameob_tp_cmp_ge)),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,
   member(&DeeFile_IterSelf),null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
   member(_deefile_tp_members),null,member(_deefile_tp_methods),
   member(_deefile_tp_class_members),null,null),
  DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deefile_tp_marshal)),
  DEE_TYPE_OBJECT_FOOTER_v101
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(
  member(&_deefile_tp_io_read),
  member(&_deefile_tp_io_write),
  member(&_deefile_tp_io_seek),
  member(&_deefile_tp_io_flush),
  member(&_deefile_tp_io_trunc),
  member(&_deefile_tp_io_close))
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};

#if DEE_PLATFORM_HAVE_IO
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

#ifdef DEE_PLATFORM_WINDOWS
DeeFileTypeObject DeeFileWin32Dbg_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("file.io.stddbg"),null,
   null,member((DeeTypeObject *)&DeeFile_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(struct _DeeFileWin32DbgObject),null,
   member(&_deefile_tp_ctor),
   member(&_deefile_tp_copy_ctor),null,null),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(&_deefile_tp_copy_assign),null,null),
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
#endif /* DEE_PLATFORM_HAVE_IO */

DeeFileTypeObject DeeFileWriter_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("file.writer"),member(
   "(size_t size_hint = 0) -> file.writer\n"
   "\tA file type that can be used to write into a memory buffer.\n"
   "\tThat buffer can then be packed into a string, using the member function #file.writer.pack."),
   null,member((DeeTypeObject *)&DeeFile_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFileWriterObject),null,
   member(&_deefilewriter_tp_ctor),
   member(&_deefilewriter_tp_copy_ctor),
   member(&_deefilewriter_tp_move_ctor),
   member(&_deefilewriter_tp_any_ctor)),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
   member(&_deefilewriter_tp_dtor)),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(&_deefilewriter_tp_copy_assign),
   member(&_deefilewriter_tp_move_assign),null),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&_deefilewriter_tp_str),
   member(&_deefilewriter_tp_repr),null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
  DEE_TYPE_OBJECT_MATH_v101(
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
   member(_deefilewriter_tp_members),null,
   member(_deefilewriter_tp_methods),null,null,null),
  DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deefilewriter_tp_marshal)),
  DEE_TYPE_OBJECT_FOOTER_v101
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(
  member(&_deefilewriter_tp_io_read),
  member(&_deefilewriter_tp_io_write),
  member(&_deefilewriter_tp_io_seek),
  member(&_deefilewriter_tp_io_flush),
  member(&_deefilewriter_tp_io_trunc),
  member(&_deefilewriter_tp_io_close))
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};
DeeFileTypeObject DeeFileReader_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("file.reader"),member(
   "(string data) -> file.reader\n"
   "\tA file type that can be used to read from a constant string buffer.\n"
   "\tThis is a read-only stream, meaning you can't write or truncate it."),
   null,member((DeeTypeObject *)&DeeFile_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFileWriterObject),null,
   member(&_deefilereader_tp_ctor),
   member(&_deefilereader_tp_copy_ctor),null,
   member(&_deefilereader_tp_any_ctor)),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
   member(&_deefilereader_tp_dtor)),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(&_deefilereader_tp_copy_assign),null,
   member(&_deefilereader_tp_any_assign)),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&_deefilereader_tp_str),
   member(&_deefilereader_tp_str),null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,
   member(&_deefilereader_tp_visit)),
  DEE_TYPE_OBJECT_MATH_v101(
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
   member(_deefilereader_tp_members),null,null,null,null,null),
  DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deefilereader_tp_marshal)),
  DEE_TYPE_OBJECT_FOOTER_v101
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(
  member(&_deefilereader_tp_io_read),
  member(&_deefilereader_tp_io_write),
  member(&_deefilereader_tp_io_seek),
  member(&_deefilereader_tp_io_flush),
  member(&_deefilereader_tp_io_trunc),
  member(&_deefilereader_tp_io_close))
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};
DeeFileTypeObject DeeFileJoined_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("file.joined"),member(
   "(file a, file b) -> file.joined\n"
   "\tA file type that can be used to write into two other files at once."),
   null,member((DeeTypeObject *)&DeeFile_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFileJoinedObject),null,
   member(&_deefilejoined_tp_ctor),
   member(&_deefilejoined_tp_copy_ctor),null,
   member(&_deefilejoined_tp_any_ctor)),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
   member(&_deefilejoined_tp_dtor)),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(&_deefilejoined_tp_copy_assign),null,null),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&_deefilejoined_tp_str),
   member(&_deefilejoined_tp_repr),null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,
   member(&_deefilejoined_tp_visit)),
  DEE_TYPE_OBJECT_MATH_v101(
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
   member(_deefilejoined_tp_members),null,null,null,null,null),
  DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deefilejoined_tp_marshal)),
  DEE_TYPE_OBJECT_FOOTER_v101
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(
  member(&_deefilejoined_tp_io_read),
  member(&_deefilejoined_tp_io_write),
  member(&_deefilejoined_tp_io_seek),
  member(&_deefilejoined_tp_io_flush),
  member(&_deefilejoined_tp_io_trunc),
  member(&_deefilejoined_tp_io_close))
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};
DeeTypeObject DeeFileIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("file.iterator"),member(
  "(file fp) -> file.iterator\n"
  "\tIterator type for any file object, that returns the contents of the file split into lines.\n"
  "\tIt does so incrementally, meaning that is is safe to use this with <stdin>, but you can think of it as #(fp.read().splitlines(true))\n"
  "\tLinefeeds are normalized to \"\\n\" characters."),
  null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFileIteratorObject),null,
  member(&_deefileiterator_tp_ctor),
  member(&_deefileiterator_tp_copy_ctor),null,
  member(&_deefileiterator_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deefileiterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deefileiterator_tp_str),
  member(&_deefileiterator_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deefileiterator_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,
  member(&_deefileiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deefileiterator_tp_member),null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deefileiterator_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};

DEE_DECL_END

#endif /* !GUARD_DEEMON_FILE_C */
