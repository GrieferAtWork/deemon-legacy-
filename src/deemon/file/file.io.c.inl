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
#ifndef GUARD_DEEMON_FILE_IO_C_INL
#define GUARD_DEEMON_FILE_IO_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include "file.io.h"
#include <deemon/__conf.inl>
#include <deemon/__xconf.inl>
#include <deemon/bool.h>
#include <deemon/none.h>
#include <deemon/fs_api.h>
#include <deemon/time.h>
#include <deemon/integer.h>
#include <deemon/memberdef.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/sys/sysfd.h>

DEE_DECL_BEGIN




DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileObject) *
DeeFile_OpenTemporary(DEE_A_IN Dee_uint32_t flags) {
 DeeString_NEW_STATIC(deemon_prefix,"deemon_fileio_temp");
 DeeObject *filename,*tmp,*result;
 if DEE_UNLIKELY((tmp = DeeFS_GetTmp()) == NULL) return NULL;
 filename = DeeFS_GetTmpNameObject(tmp,(DeeObject *)&deemon_prefix);
 Dee_DECREF(tmp);
 if DEE_UNLIKELY(!filename) return NULL;
 result = _DeeFile_OpenObject(filename,DEE_OPENMODE('w',0));
 Dee_DECREF(filename);
 if ((flags&DEE_FILE_OPENTEMPORARY_FLAG_DELETE_WHEN_CLOSED)!=0 && result)
  ((DeeFileObject *)result)->fo_flags |= DEE_PRIVATE_FILEFLAG_IO_DELONCLOSE;
 return result;
}

DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileObject) *_DeeFile_Utf8OpenEx(
 DEE_A_IN_Z Dee_Utf8Char const *file, DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) {
 DeeFileIOObject *result; // TODO: VFS
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeFileIOObject)) == NULL) return NULL;
 DeeSysFileFD_Utf8Init(&result->io_descr,file,openmode,permissions,
                       { DeeObject_Free(result); return NULL; });
 DeeObject_INIT(result,(DeeTypeObject *)&DeeFileIO_Type);
 DeeFileFD_InitBasic(result,DEE_PRIVATE_FILEFLAG_FD_VALID|
                            DEE_PRIVATE_FILEFLAG_FD_OWNED);
 return (DeeObject *)result;
}
DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileObject) *_DeeFile_WideOpenEx(
 DEE_A_IN_Z Dee_WideChar const *file, DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) {
 DeeFileIOObject *result; // TODO: VFS
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeFileIOObject)) == NULL) return NULL;
 DeeSysFileFD_WideInit(&result->io_descr,file,openmode,permissions,
                       { DeeObject_Free(result); return NULL; });
 DeeObject_INIT(result,(DeeTypeObject *)&DeeFileIO_Type);
 DeeFileFD_InitBasic(result,DEE_PRIVATE_FILEFLAG_FD_VALID|
                            DEE_PRIVATE_FILEFLAG_FD_OWNED);
 return (DeeObject *)result;
}
DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileObject) *DeeFile_Utf8OpenEx(
 DEE_A_IN_Z Dee_Utf8Char const *file, DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) {
 DeeFileIOObject *result; DeeObject *newfile; // TODO: VFS
 if DEE_UNLIKELY((newfile = DeeFS_Utf8PathExpand(file)) == NULL) return NULL;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeFileIOObject)) == NULL) {
err_newfilename: Dee_DECREF(newfile); return NULL;
 }
 DeeSysFileFD_Utf8InitObject(&result->io_descr,newfile,openmode,permissions,
                             { DeeObject_Free(result); goto err_newfilename; });
 DeeObject_INIT(result,(DeeTypeObject *)&DeeFileIO_Type);
 DeeFileFD_InitBasic(result,DEE_PRIVATE_FILEFLAG_FD_VALID|
                            DEE_PRIVATE_FILEFLAG_FD_OWNED);
 Dee_DECREF(newfile);
 return (DeeObject *)result;
}
DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileObject) *DeeFile_WideOpenEx(
 DEE_A_IN_Z Dee_WideChar const *file, DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) {
 DeeFileIOObject *result; DeeObject *newfile; // TODO: VFS
 if DEE_UNLIKELY((newfile = DeeFS_WidePathExpand(file)) == NULL) return NULL;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeFileIOObject)) == NULL) {
err_newfilename: Dee_DECREF(newfile); return NULL;
 }
 DeeSysFileFD_WideInitObject(&result->io_descr,newfile,openmode,permissions,
                             { DeeObject_Free(result); goto err_newfilename; });
 DeeObject_INIT(result,(DeeTypeObject *)&DeeFileIO_Type);
 DeeFileFD_InitBasic(result,DEE_PRIVATE_FILEFLAG_FD_VALID|
                            DEE_PRIVATE_FILEFLAG_FD_OWNED);
 Dee_DECREF(newfile);
 return (DeeObject *)result;
}
DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileObject) *DeeFile_OpenObjectEx(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *file,
 DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) {
 DeeObject *result,*newfile; // TODO: VFS
 if DEE_UNLIKELY((newfile = DeeFS_PathExpandObject(file)) == NULL) return NULL;
 result = _DeeFile_OpenObjectEx(newfile,openmode,permissions);
 Dee_DECREF(newfile);
 return result;
}
DEE_A_INTERRUPT DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileObject) *_DeeFile_OpenObjectEx(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *file,
 DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) {
 DeeFileIOObject *result; DeeObject *newfilename; // TODO: VFS
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY((newfilename = DeeWideString_Cast(file)) == NULL) return NULL;
#else
 if DEE_UNLIKELY((newfilename = DeeUtf8String_Cast(file)) == NULL) return NULL;
#endif
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeFileIOObject)) == NULL) {
err_newfilename: Dee_DECREF(newfilename); return NULL;
 }
#ifdef DEE_PLATFORM_WINDOWS
 DeeSysFileFD_WideInitObject(&result->io_descr,newfilename,openmode,permissions,{
  DeeObject_Free(result);
  goto err_newfilename;
 });
#else
 DeeSysFileFD_Utf8InitObject(&result->io_descr,newfilename,openmode,permissions,{
  DeeObject_Free(result);
  goto err_newfilename;
 });
#endif
 DeeObject_INIT(result,(DeeTypeObject *)&DeeFileIO_Type);
 DeeFileFD_InitBasic(result,DEE_PRIVATE_FILEFLAG_FD_VALID|
                            DEE_PRIVATE_FILEFLAG_FD_OWNED);
 Dee_DECREF(newfilename);
 return (DeeObject *)result;
}


DEE_A_RET_OBJECT_REF(DeeUtf8StringObject) *DeeFileIO_Utf8Filename(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
#ifdef DeeSysFileFD_Utf8Filename
 DeeObject *result;
 DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return NULL; });
 result = DeeSysFileFD_Utf8Filename(&((DeeFileIOObject *)self)->io_descr);
 DeeFileIO_RELEASE_SHARED(self);
 return result;
#elif defined(DeeSysFileFD_WideFilename)\
   || defined(DeeSysFileFD_Filename)
 DeeObject *result,*newresult;
 DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return NULL; });
#ifdef DeeSysFileFD_WideFilename
 result = DeeSysFileFD_WideFilename(&((DeeFileIOObject *)self)->io_descr);
#else
 result = DeeSysFileFD_Filename(&((DeeFileIOObject *)self)->io_descr);
#endif
 DeeFileIO_RELEASE_SHARED(self);
 if DEE_UNLIKELY(!result) return NULL;
 newresult = DeeUtf8String_Cast(result);
 Dee_DECREF(result);
 return newresult;
#else
 DeeError_NotImplemented_str("file.io.filename");
 return NULL;
#endif
}
DEE_A_RET_OBJECT_REF(DeeWideStringObject) *DeeFileIO_WideFilename(
 DEE_A_IN_OBJECT(DeeFileIOObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileIO_Check(self));
#ifdef DeeSysFileFD_WideFilename
 DeeObject *result;
 DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return NULL; });
 result = DeeSysFileFD_WideFilename(&((DeeFileIOObject *)self)->io_descr);
 DeeFileIO_RELEASE_SHARED(self);
 return result;
#elif defined(DeeSysFileFD_Utf8Filename)\
   || defined(DeeSysFileFD_Filename)
 DeeObject *result,*newresult;
 DeeFileIO_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return NULL; });
#ifdef DeeSysFileFD_Utf8Filename
 result = DeeSysFileFD_Utf8Filename(&((DeeFileIOObject *)self)->io_descr);
#else
 result = DeeSysFileFD_Filename(&((DeeFileIOObject *)self)->io_descr);
#endif
 DeeFileIO_RELEASE_SHARED(self);
 if DEE_UNLIKELY(!result) return NULL;
 newresult = DeeWideString_Cast(result);
 Dee_DECREF(result);
 return newresult;
#else
 DeeError_NotImplemented_str("file.io.wfilename");
 return NULL;
#endif
}



DeeString_NEW_STATIC_EX(_deefileio_default_mode,1,{'r'});
static int _deefileio_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileIOObject *self, DeeObject *args) {
 DeeAnyStringObject *filename; Dee_openmode_t openmode;
 Dee_mode_t perms = DEE_FILEIO_DEFAULT_PERMISSIONS;
 DeeStringObject *mode = (DeeStringObject *)&_deefileio_default_mode;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|oI" DEE_PP_STR(DEE_PP_MUL8(
  DEE_TYPES_SIZEOF_MODE_T)) "u:file.fd.fp",&filename,&mode,&perms) != 0) return -1;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)mode,&DeeString_Type) != 0) return -1;
 openmode = DeeFile_TryParseMode(DeeString_STR(mode));
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY((filename = (DeeAnyStringObject *)(DeeFileIO_Check(filename)
  ? DeeFileIO_WideFilename((DeeObject *)filename)
  : DeeWideString_Cast((DeeObject *)filename))) == NULL) return -1;
 DeeSysFileFD_WideInitObject(&self->io_descr,(DeeObject *)filename,openmode,
                             perms,{ Dee_DECREF(filename); return -1; });
 Dee_DECREF(filename);
#else
 if DEE_UNLIKELY((filename = (DeeAnyStringObject *)(DeeFileIO_Check(filename)
  ? DeeFileIO_Utf8Filename((DeeObject *)filename)
  : DeeUtf8String_Cast((DeeObject *)filename))) == NULL) return -1;
 DeeSysFileFD_Utf8InitObject(&self->io_descr,(DeeObject *)filename,openmode,
                             perms,{ Dee_DECREF(filename); return -1; });
 Dee_DECREF(filename);
#endif
 DeeFileFD_InitBasic(self,DEE_PRIVATE_FILEFLAG_FD_VALID|
                          DEE_PRIVATE_FILEFLAG_FD_OWNED);
 return 0;
}

//////////////////////////////////////////////////////////////////////////
// Scan for additional callbacks required for filefd descriptors, and link them
#ifdef DeeSysFileFD_Read
#define _pdeefileio_tp_io_read &_deefileio_tp_io_read
static int DEE_CALL _deefileio_tp_io_read(
 DeeFileIOObject *self, void const const *p, Dee_size_t s, Dee_size_t *rs) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileFD_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed);
  return -1;
 });
 DeeSysFileFD_Read(&self->io_descr,p,s,rs,{
  DeeFileFD_RELEASE_SHARED(self);
  return -1;
 });
 DeeFileFD_RELEASE_SHARED(self);
 return 0;
}
#else
#define _pdeefileio_tp_io_read  DeeType_DEFAULT_SLOT(tp_io_read)
#endif
#ifdef DeeSysFileFD_Write
#define _pdeefileio_tp_io_write &_deefileio_tp_io_write
static int DEE_CALL _deefileio_tp_io_write(
 DeeFileIOObject *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileFD_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed);
  return -1;
 });
 DeeSysFileFD_Write(&self->io_descr,p,s,ws,{
  DeeFileFD_RELEASE_SHARED(self);
  return -1;
 });
 DeeFileFD_RELEASE_SHARED(self);
 return 0;
}
#else
#define _pdeefileio_tp_io_write DeeType_DEFAULT_SLOT(tp_io_write)
#endif
#ifdef DeeSysFileFD_Seek
#define _pdeefileio_tp_io_seek &_deefileio_tp_io_seek
static int DEE_CALL _deefileio_tp_io_seek(
 DeeFileIOObject *self, Dee_int64_t off, int whence, Dee_uint64_t *pos) {
 DEE_FILEFD_FIX_SEEKWHENCE(whence);
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileFD_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed);
  return -1;
 });
 DeeSysFileFD_Seek(&self->io_descr,off,whence,pos,{
  DeeFileFD_RELEASE_SHARED(self);
  return -1;
 });
 DeeFileFD_RELEASE_SHARED(self);
 return 0;
}
#else
#define _pdeefileio_tp_io_seek  DeeType_DEFAULT_SLOT(tp_io_seek)
#endif


#ifdef DeeSysFileFD_Flush
#define _pdeefileio_tp_io_flush &_deefileio_tp_io_flush
static int DEE_CALL _deefileio_tp_io_flush(DeeFileIOObject *self) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileFD_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed);
  return -1;
 });
 DeeSysFD_Flush(&self->io_descr,{
  DeeFileFD_RELEASE_SHARED(self);
  return -1;
 });
 DeeFileFD_RELEASE_SHARED(self);
 return 0;
}
#else
#define _pdeefileio_tp_io_flush DeeType_DEFAULT_SLOT(tp_io_flush)
#endif

#ifdef DeeSysFileFD_Trunc
#define _pdeefileio_tp_io_trunc &_deefileio_tp_io_trunc
static int DEE_CALL _deefileio_tp_io_trunc(DeeFileIOObject *self) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileFD_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed);
  return -1;
 });
 DeeSysFileFD_Trunc(&self->io_descr,{
  DeeFileFD_RELEASE_SHARED(self);
  return -1;
 });
 DeeFileFD_RELEASE_SHARED(self);
 return 0;
}
#else
#define _pdeefileio_tp_io_trunc DeeType_DEFAULT_SLOT(tp_io_trunc)
#endif

#ifdef DeeSysFileFD_Quit
#define _pdeefileio_tp_dtor     &_deefileio_tp_dtor
#define _pdeefileio_tp_io_close &_deefileio_tp_io_close
static void DEE_CALL _deefileio_tp_dtor(DeeFileIOObject *self) {
 if ((self->fo_flags&(DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) == 
                     (DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) {
  self->fo_flags &= ~(DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED);
  DeeSysFileFD_Quit(&self->fd_descr);
#ifdef DeeSysFD_Quit
  DeeSysFD_Quit(&self->fd_descr);
#endif
 }
}
static void DEE_CALL _deefileio_tp_io_close(DeeFileIOObject *self) {
 DeeFileFD_ACQUIRE_EXCLUSIVE(self,return);
 DeeSysFileFD_Quit(&self->io_descr);
 DeeSysFD_Quit(&self->io_descr);
 self->fo_flags &= ~DEE_PRIVATE_FILEFLAG_FD_VALID;
 DeeFileFD_RELEASE_EXCLUSIVE(self);
}
#else
#define _pdeefileio_tp_dtor     DeeType_DEFAULT_SLOT(tp_dtor)
#define _pdeefileio_tp_io_close DeeType_DEFAULT_SLOT(tp_io_close)
#endif





//////////////////////////////////////////////////////////////////////////
// File.io member functions

#ifdef DeeSysFileFD_GetSize
#define _deefileio_size _deefileio_size
static DeeObject *_deefileio_size(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint64_t fp_size;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":size") != 0) return NULL;
 DeeSysFileFD_GetSize(DeeFileIO_FD(self),&fp_size,return NULL);
 return DeeObject_New(Dee_uint64_t,fp_size);
}
#endif /* DeeSysFileFD_GetSize */


static DeeObject *_deefileio_filename_get(
 DeeFileIOObject *self, void *DEE_UNUSED(closure)) {
#ifdef DeeSysFileFD_Utf8Filename
 DeeObject *result;
 DeeFileIO_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed);
  return NULL;
 });
 result = DeeSysFileFD_Utf8Filename(DeeFileIO_FD(self));
 DeeFileIO_RELEASE_SHARED(self);
 return result;
#else
 (void)self,
 DeeError_NotImplemented_str("file.fd.file.filename");
 return NULL;
#endif
}
static DeeObject *_deefileio_wfilename_get(
 DeeFileIOObject *self, void *DEE_UNUSED(closure)) {
#ifdef DeeSysFileFD_WideFilename
 DeeObject *result;
 DeeFileIO_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed);
  return NULL;
 });
 result = DeeSysFileFD_WideFilename(DeeFileIO_FD(self));
 DeeFileIO_RELEASE_SHARED(self);
 return result;
#else
 (void)self,
 DeeError_NotImplemented_str("file.fd.file.wfilename");
 return NULL;
#endif
}

static DeeObject *DEE_CALL _deefileio_gettimes(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":gettimes") != 0) return NULL;
 return DeeFileIO_GetTimes((DeeObject *)self);
}
static DeeObject *DEE_CALL _deefileio_getatime(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":getatime") != 0) return NULL;
 return DeeFileIO_GetATime((DeeObject *)self);
}
static DeeObject *DEE_CALL _deefileio_getctime(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":getctime") != 0) return NULL;
 return DeeFileIO_GetCTime((DeeObject *)self);
}
static DeeObject *DEE_CALL _deefileio_getmtime(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":getmtime") != 0) return NULL;
 return DeeFileIO_GetMTime((DeeObject *)self);
}
static DeeObject *DEE_CALL _deefileio_settimes(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *tm_access = NULL,*tm_creation = NULL,*tm_modification = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|ooo:settimes",&tm_access,&tm_creation,&tm_modification) != 0) return NULL;
 if DEE_UNLIKELY(DeeFileIO_SetTimes2((DeeObject *)self,tm_access,tm_creation,tm_modification) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deefileio_setatime(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *tm_access;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:setatime",&tm_access) != 0) return NULL;
 if DEE_UNLIKELY(DeeFileIO_SetATime((DeeObject *)self,tm_access) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deefileio_setctime(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *tm_creation;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:setctime",&tm_creation) != 0) return NULL;
 if DEE_UNLIKELY(DeeFileIO_SetCTime((DeeObject *)self,tm_creation) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deefileio_setmtime(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *tm_modification;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:setmtime",&tm_modification) != 0) return NULL;
 if DEE_UNLIKELY(DeeFileIO_SetMTime((DeeObject *)self,tm_modification) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deefileio_isfile(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":isfile") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsFile((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *DEE_CALL _deefileio_isdir(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":isdir") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsDir((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *DEE_CALL _deefileio_islink(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":islink") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsLink((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *DEE_CALL _deefileio_isdrive(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":isdrive") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsDrive((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *DEE_CALL _deefileio_ismount(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":ismount") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsMount((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *DEE_CALL _deefileio_ishidden(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":ishidden") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsHidden((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *DEE_CALL _deefileio_isxfile(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":isxfile") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsExecutable((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *DEE_CALL _deefileio_ischrdev(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":ischrdev") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsCharDev((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *DEE_CALL _deefileio_isblkdev(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":isblkdev") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsBlockDev((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *DEE_CALL _deefileio_isfifo(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":isfifo") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsFiFo((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
static DeeObject *DEE_CALL _deefileio_issocket(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":issocket") != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_IsSocket((DeeObject *)self)) < 0) return NULL;
 DeeReturn_Bool(temp);
}

static DeeObject *DEE_CALL _deefileio_dir(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":dir") != 0) return NULL;
 return DeeFileIO_ListDir((DeeObject *)self);
}
static DeeObject *DEE_CALL _deefileio_chmod(
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
static DeeObject *DEE_CALL _deefileio_getmod(
 DeeFileIOObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_mode_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":getmod") != 0) return NULL;
 if DEE_UNLIKELY(DeeFileIO_GetMod((DeeObject *)self,&result) != 0) return NULL;
 return DeeObject_New(Dee_mode_t,result);
}
static DeeObject *DEE_CALL _deefileio_chown(
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
static DeeObject *DEE_CALL _deefileio_getown(
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
static DeeObject *DEE_CALL _deefileioclass_tmp(
 DeeFileTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 int delete_when_closed = 1;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|b:tmp",&delete_when_closed) != 0) return NULL;
 return DeeFile_OpenTemporary(DEE_LIKELY(delete_when_closed)
  ? DEE_FILE_OPENTEMPORARY_FLAG_DELETE_WHEN_CLOSED
  : DEE_FILE_OPENTEMPORARY_FLAG_NONE);
}

static DeeObject *DEE_CALL _deefileioclass_stdin_get(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { return DeeFile_Std(DEE_STDIN); }
static DeeObject *DEE_CALL _deefileioclass_stdout_get(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { return DeeFile_Std(DEE_STDOUT); }
static DeeObject *DEE_CALL _deefileioclass_stderr_get(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { return DeeFile_Std(DEE_STDERR); }
static DeeObject *DEE_CALL _deefileioclass_stdnull_get(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { return DeeFile_Std(DEE_STDNULL); }
static DeeObject *DEE_CALL _deefileioclass_stddbg_get(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { return DeeFile_Std(DEE_STDDBG); }
static int DEE_CALL _deefileioclass_stdin_del(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { DeeFile_DelStd(DEE_STDIN); return 0; }
static int DEE_CALL _deefileioclass_stdout_del(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { DeeFile_DelStd(DEE_STDOUT); return 0; }
static int DEE_CALL _deefileioclass_stderr_del(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { DeeFile_DelStd(DEE_STDERR); return 0; }
static int DEE_CALL _deefileioclass_stdnull_del(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { DeeFile_DelStd(DEE_STDNULL); return 0; }
static int DEE_CALL _deefileioclass_stddbg_del(DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) { DeeFile_DelStd(DEE_STDDBG); return 0; }
static int DEE_CALL _deefileioclass_stdin_set(DeeObject *DEE_UNUSED(self), DeeObject *v, void *DEE_UNUSED(closure)) { if DEE_UNLIKELY((v = DeeObject_GetInstance(v,(DeeTypeObject *)&DeeFile_Type)) == NULL) return -1; DeeFile_SetStd(DEE_STDIN,v); return 0; }
static int DEE_CALL _deefileioclass_stdout_set(DeeObject *DEE_UNUSED(self), DeeObject *v, void *DEE_UNUSED(closure)) { if DEE_UNLIKELY((v = DeeObject_GetInstance(v,(DeeTypeObject *)&DeeFile_Type)) == NULL) return -1; DeeFile_SetStd(DEE_STDOUT,v); return 0; }
static int DEE_CALL _deefileioclass_stderr_set(DeeObject *DEE_UNUSED(self), DeeObject *v, void *DEE_UNUSED(closure)) { if DEE_UNLIKELY((v = DeeObject_GetInstance(v,(DeeTypeObject *)&DeeFile_Type)) == NULL) return -1; DeeFile_SetStd(DEE_STDERR,v); return 0; }
static int DEE_CALL _deefileioclass_stdnull_set(DeeObject *DEE_UNUSED(self), DeeObject *v, void *DEE_UNUSED(closure)) { if DEE_UNLIKELY((v = DeeObject_GetInstance(v,(DeeTypeObject *)&DeeFile_Type)) == NULL) return -1; DeeFile_SetStd(DEE_STDNULL,v); return 0; }
static int DEE_CALL _deefileioclass_stddbg_set(DeeObject *DEE_UNUSED(self), DeeObject *v, void *DEE_UNUSED(closure)) { if DEE_UNLIKELY((v = DeeObject_GetInstance(v,(DeeTypeObject *)&DeeFile_Type)) == NULL) return -1; DeeFile_SetStd(DEE_STDDBG,v); return 0; }

static DeeObject *DEE_CALL _deefileio_times_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeFileIO_GetTimes(self); }
static DeeObject *DEE_CALL _deefileio_atime_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeFileIO_GetATime(self); }
static DeeObject *DEE_CALL _deefileio_ctime_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeFileIO_GetCTime(self); }
static DeeObject *DEE_CALL _deefileio_mtime_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeFileIO_GetMTime(self); }
static int DEE_CALL _deefileio_times_set(DeeObject *self, DeeObject *ob, void *DEE_UNUSED(closure)) {
 DeeObject *tm_access,*tm_creation,*tm_modification;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(ob,&DeeTuple_Type) != 0) return -1;
 if DEE_UNLIKELY(DeeTuple_SIZE(ob) != 3) { DeeError_TypeError_TupleSizeExpected(3,DeeTuple_SIZE(ob)); return -1; }
 if DEE_UNLIKELY((tm_access = DeeObject_GetInstance(DeeTuple_GET(ob,0),&DeeTime_Type)) == NULL) return -1;
 if DEE_UNLIKELY((tm_creation = DeeObject_GetInstance(DeeTuple_GET(ob,1),&DeeTime_Type)) == NULL) return -1;
 if DEE_UNLIKELY((tm_modification = DeeObject_GetInstance(DeeTuple_GET(ob,2),&DeeTime_Type)) == NULL) return -1;
 return DeeFileIO_SetTimes2(self,tm_access,tm_creation,tm_modification);
}
static int DEE_CALL _deefileio_atime_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY((v = DeeObject_GetInstance(v,&DeeTime_Type)) == NULL) return -1;
 return DeeFileIO_SetATime(self,v);
}
static int DEE_CALL _deefileio_ctime_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY((v = DeeObject_GetInstance(v,&DeeTime_Type)) == NULL) return -1;
 return DeeFileIO_SetCTime(self,v);
}
static int DEE_CALL _deefileio_mtime_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY((v = DeeObject_GetInstance(v,&DeeTime_Type)) == NULL) return -1;
 return DeeFileIO_SetMTime(self,v);
}


static struct DeeGetSetDef const _deefileio_tp_getsets[] = {
 DEE_GETSETDEF_v100("filename",member(&_deefileio_filename_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("wfilename",member(&_deefileio_wfilename_get),null,null,"-> string.wide"),
 DEE_GETSETDEF_v100("times",member(&_deefileio_times_get),null,member(&_deefileio_times_set),"-> (time,time,time)\n"),
 DEE_GETSETDEF_v100("atime",member(&_deefileio_atime_get),null,member(&_deefileio_atime_set),"-> time\n"),
 DEE_GETSETDEF_v100("ctime",member(&_deefileio_ctime_get),null,member(&_deefileio_ctime_set),"-> time\n"),
 DEE_GETSETDEF_v100("mtime",member(&_deefileio_mtime_get),null,member(&_deefileio_mtime_set),"-> time\n"),
 DEE_GETSETDEF_END_v100
};


static struct DeeMethodDef const _deefileio_tp_methods[] = {
#ifdef _deefileio_size
 DEE_METHODDEF_v100("size",member(&_deefileio_size),"() -> uint64_t"),
#endif
 DEE_METHODDEF_v100("gettimes",member(&_deefileio_gettimes),"() -> (time,time,time)"),
 DEE_METHODDEF_v100("getatime",member(&_deefileio_getatime),"() -> time"),
 DEE_METHODDEF_v100("getctime",member(&_deefileio_getctime),"() -> time"),
 DEE_METHODDEF_v100("getmtime",member(&_deefileio_getmtime),"() -> time"),
 DEE_METHODDEF_v100("settimes",member(&_deefileio_settimes),
  "(time atime) -> none\n"
  "(time atime, time ctime) -> none\n"
  "(time atime, time ctime, time mtime) -> none\n"),
 DEE_METHODDEF_v100("setatime",member(&_deefileio_setatime),"(time atime) -> none"),
 DEE_METHODDEF_v100("setctime",member(&_deefileio_setctime),"(time ctime) -> none"),
 DEE_METHODDEF_v100("setmtime",member(&_deefileio_setmtime),"(time mtime) -> none"),
 DEE_METHODDEF_v100("isfile",member(&_deefileio_isfile),"() -> bool"),
 DEE_METHODDEF_v100("isdir",member(&_deefileio_isdir),"() -> bool"),
 DEE_METHODDEF_v100("islink",member(&_deefileio_islink),"() -> bool"),
 DEE_METHODDEF_v100("isdrive",member(&_deefileio_isdrive),"() -> bool"),
 DEE_METHODDEF_v100("ismount",member(&_deefileio_ismount),"() -> bool"),
 DEE_METHODDEF_v100("ishidden",member(&_deefileio_ishidden),"() -> bool"),
 DEE_METHODDEF_v100("isxfile",member(&_deefileio_isxfile),"() -> bool"),
 DEE_METHODDEF_v100("ischrdev",member(&_deefileio_ischrdev),"() -> bool"),
 DEE_METHODDEF_v100("isblkdev",member(&_deefileio_isblkdev),"() -> bool"),
 DEE_METHODDEF_v100("isfifo",member(&_deefileio_isfifo),"() -> bool"),
 DEE_METHODDEF_v100("issocket",member(&_deefileio_issocket),"() -> bool"),
 DEE_METHODDEF_v100("dir",member(&_deefileio_dir),"() -> dir"),
 DEE_METHODDEF_v100("chmod",member(&_deefileio_chmod),
  "(string mod) -> none\n"
  "(mode_t mod) -> none"),
 DEE_METHODDEF_v100("chown",member(&_deefileio_chown),
  "(string owner_and_group) -> none\n"
  "(string owner, string group) -> none\n"
  "(string owner, gid_t group) -> none\n"
  "(uid_t owner, string group) -> none\n"
  "(uid_t owner, gid_t group) -> none\n"),
 DEE_METHODDEF_v100("getmod",member(&_deefileio_getmod),
  "() -> mode_t"),
 DEE_METHODDEF_v100("getown",member(&_deefileio_getown),
  "() -> (uid_t,gid_t)"),
 DEE_METHODDEF_END_v100
};

static struct DeeMethodDef const _deefileio_tp_class_methods[] = {
 DEE_METHODDEF_v100("tmp",member(&_deefileioclass_tmp),"(bool delete_when_closed = true) -> file.io"),
 DEE_METHODDEF_END_v100
};
static struct DeeGetSetDef const _deefileio_tp_class_getsets[] = {
 DEE_GETSETDEF_v100("stdin",member(&_deefileioclass_stdin_get),member(&_deefileioclass_stdin_del),member(&_deefileioclass_stdin_set),"-> file\n\tThe standard input stream"),
 DEE_GETSETDEF_v100("stdout",member(&_deefileioclass_stdout_get),member(&_deefileioclass_stdout_del),member(&_deefileioclass_stdout_set),"-> file\n\tThe standard output stream"),
 DEE_GETSETDEF_v100("stderr",member(&_deefileioclass_stderr_get),member(&_deefileioclass_stderr_del),member(&_deefileioclass_stderr_set),"-> file\n\tThe standard error stream"),
 DEE_GETSETDEF_v100("stdnull",member(&_deefileioclass_stdnull_get),member(&_deefileioclass_stdnull_del),member(&_deefileioclass_stdnull_set),"-> file\n\tA stream that swallows everything you throw at it"),
 DEE_GETSETDEF_v100("stddbg",member(&_deefileioclass_stddbg_get),member(&_deefileioclass_stddbg_del),member(&_deefileioclass_stddbg_set),"-> file\n\tA stream that can be used for debug output"),
 DEE_GETSETDEF_END_v100
};




DeeFileTypeObject DeeFileIO_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("file.io"),member(
    "(string filename, string mode = \"r\", mode_t permissions = " DEE_PP_STR(DEE_FILEIO_DEFAULT_PERMISSIONS) ")\n"
    "(string.wide filename, string mode = \"r\", mode_t permissions = " DEE_PP_STR(DEE_FILEIO_DEFAULT_PERMISSIONS) ")\n"
    "(file.io filename, string mode = \"r\", mode_t permissions = " DEE_PP_STR(DEE_FILEIO_DEFAULT_PERMISSIONS) ")\n"
    "\tThe file type used to access files in the filesystem of the host system\n"
    "\tNOTE: Starting in v102, direct use of this type's constructor is discouraged "
            "due to various restraints. Use #file.open instead."),
   null,member((DeeTypeObject *)&DeeFileFD_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFileIOObject),null,
   member(_pdeefilefd_tp_ctor),
   member(_pdeefilefd_tp_copy_ctor),
   member(_pdeefilefd_tp_move_ctor),
   member(&_deefileio_tp_any_ctor)),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(_pdeefileio_tp_dtor)),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(_pdeefilefd_tp_copy_assign),
   member(_pdeefilefd_tp_move_assign),null),
  DEE_TYPE_OBJECT_CAST_v101(
   member(_pdeefilefd_tp_str),null,null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
  DEE_TYPE_OBJECT_MATH_v101(
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
   null,member(_deefileio_tp_getsets),
   member(_deefileio_tp_methods),null,
   member(_deefileio_tp_class_getsets),
   member(_deefileio_tp_class_methods)),
  DEE_TYPE_OBJECT_FOOTER_v100
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(
  member(_pdeefileio_tp_io_read),
  member(_pdeefileio_tp_io_write),
  member(_pdeefileio_tp_io_seek),
  member(_pdeefileio_tp_io_flush),
  member(_pdeefileio_tp_io_trunc),
  member(_pdeefileio_tp_io_close))
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};

DEE_DECL_END

#ifndef __INTELLISENSE__
#include "file.io.fsops.c.inl"
#endif

#endif /* !GUARD_DEEMON_FILE_IO_C_INL */
