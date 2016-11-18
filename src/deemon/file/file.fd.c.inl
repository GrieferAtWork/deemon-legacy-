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
#ifndef GUARD_DEEMON_FILE_FD_C_INL
#define GUARD_DEEMON_FILE_FD_C_INL 1
#define DEE_LIMITED_API 1

#include "file.fd.h"
#include <deemon/__conf.inl>
#include <deemon/__xconf.inl>
#include <deemon/bool.h>
#include <deemon/file.h>
#include <deemon/none.h>
#include <deemon/memberdef.h>
#include <deemon/integer.h>
#include <deemon/structured.h>
#include <deemon/sys/sysfd.h>

DEE_DECL_BEGIN

#ifdef DEE_PLATFORM_WINDOWS
DEE_A_RET_NOEXCEPT(1) int DeeFileFD_Win32AcquireHandle(
 DEE_A_INOUT_OBJECT(DeeFileFDObject) *self, DEE_A_OUT /*PHANDLE*/ void **result) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileFD_Check(self));
 DEE_ASSERT(result);
 DeeFileFD_ACQUIRE_SHARED((DeeFileFDObject *)self,return 1);
 *(PHANDLE)result = ((DeeFileFDObject *)self)->fd_descr.w32_handle;
 return 0;
}
#endif

#ifdef DEE_PLATFORM_UNIX
DEE_A_RET_NOEXCEPT(1) int DeeFileFD_PosixAcquireFileno(
 DEE_A_INOUT_OBJECT(DeeFileFDObject) *self, DEE_A_OUT int **result) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileFD_Check(self));
 DEE_ASSERT(result);
 DeeFileFD_ACQUIRE_SHARED((DeeFileFDObject *)self,return 1);
 *result = ((DeeFileFDObject *)self)->fd_descr.unx_fd;
 return 0;
}
#endif

#if defined(DEE_PLATFORM_WINDOWS) || defined(DEE_PLATFORM_UNIX)
void _DeeFileFD_PrivateReleaseTicket(
 DEE_A_INOUT_OBJECT(DeeFileFDObject) *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeFileFD_Check(self));
 DeeFileFD_RELEASE_SHARED((DeeFileFDObject *)self);
}
#endif



int DEE_CALL _deefilefd_tp_ctor(
 DeeFileTypeObject *DEE_UNUSED(tp_self), DeeFileFDObject *self) {
 // Don't set the 'DEE_PRIVATE_FILEFLAG_FD_VALID' flag here.
 DeeFileFD_InitBasic(self,DEE_PRIVATE_FILEFLAG_NONE);
 return 0;
}

#ifdef DeeSysFD_InitCopy /*< The system supports fd duplication. */
#define _pdeefilefd_tp_copy_ctor &_deefilefd_tp_copy_ctor
int DEE_CALL _deefilefd_tp_copy_ctor(
 DeeFileTypeObject *DEE_UNUSED(tp_self),
 DeeFileFDObject *self, DeeFileFDObject *right) {
 Dee_uint32_t newflags;
 // In here, we always copy the file-descriptor
 DeeFileFD_ACQUIRE_SHARED(right,{
  DeeFileFD_InitBasic(self,DEE_PRIVATE_FILEFLAG_NONE);
  return 0;
 });
 DeeSysFD_InitCopy(&self->fd_descr,&right->fd_descr,{
  DeeFileFD_RELEASE_SHARED(right);
  return -1;
 });
 newflags = right->fo_flags;
 DeeFileFD_RELEASE_SHARED(right);
 DeeFileFD_InitBasic(self,newflags|DEE_PRIVATE_FILEFLAG_FD_OWNED);
 return 0;
}
#else
#define _pdeefilefd_tp_copy_ctor DeeType_DEFAULT_SLOT(tp_copy_ctor)
#endif

int DEE_CALL _deefilefd_tp_move_ctor(
 DeeFileTypeObject *DEE_UNUSED(tp_self),
 DeeFileFDObject *self, DeeFileFDObject *right) {
 Dee_uint32_t newflags;
 DeeFileFD_ACQUIRE_EXCLUSIVE(right,{});
 newflags = right->fo_flags;
 *&self->fd_descr = *&right->fd_descr;
 right->fo_flags = DEE_PRIVATE_FILEFLAG_NONE;
 DeeFileFD_RELEASE_EXCLUSIVE(right);
 DeeFileFD_InitBasic(self,newflags);
 return 0;
}

#if defined(DEE_PLATFORM_WINDOWS)\
 || defined(DEE_PLATFORM_UNIX)
#define _pdeefilefd_tp_any_ctor &_deefilefd_tp_any_ctor
int DEE_CALL _deefilefd_tp_any_ctor(
 DeeFileTypeObject *DEE_UNUSED(tp_self),
 DeeFileFDObject *self, DeeObject *args) {
 Dee_filedescr_t new_fd;
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"p:file.fd",&new_fd) != 0) return -1;
 self->fd_descr.w32_handle = (HANDLE)new_fd;
#else
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"d:file.fd",&new_fd) != 0) return -1;
 self->fd_descr.unx_fd = new_fd;
#endif
 // XXX: Validate handle?
 DeeFileFD_InitBasic(self,DEE_PRIVATE_FILEFLAG_FD_VALID);
 return 0;
}
#else
#define _pdeefilefd_tp_any_ctor DeeType_DEFAULT_SLOT(tp_any_ctor)
#endif

#ifdef DeeSysFD_Quit
#define _pdeefilefd_tp_dtor &_deefilefd_tp_dtor
void DEE_CALL _deefilefd_tp_dtor(DeeFileFDObject *self) {
 if ((self->fo_flags&(DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) == 
                     (DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) {
  DeeSysFD_Quit(&self->fd_descr);
 }
}
#else
#define _pdeefilefd_tp_dtor DeeType_DEFAULT_SLOT(tp_dtor)
#endif


int DEE_CALL _deefilefd_tp_move_assign(
 DeeFileFDObject *self, DeeFileFDObject *right) {
 struct DeeSysFD new_fd,old_fd;
 Dee_uint32_t newflags,oldflags;
 if (self != right) {
  DeeFileFD_ACQUIRE_EXCLUSIVE(right);
  new_fd = *&right->fd_descr;
  newflags = right->fo_flags;
  right->fo_flags = DEE_PRIVATE_FILEFLAG_NONE;
  DeeFileFD_RELEASE_EXCLUSIVE(right);
  DeeFileFD_ACQUIRE_EXCLUSIVE(self);
  oldflags = self->fo_flags;
  old_fd = *&self->fd_descr;
  self->fo_flags = newflags;
  *&self->fd_descr = new_fd;
  DeeFileFD_RELEASE_EXCLUSIVE(self);
#ifdef DeeSysFD_Quit
  if ((oldflags&(DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) ==
                (DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) {
   DeeSysFD_Quit(&old_fd);
  }
#endif
 }
 return 0;
}

#ifdef DeeSysFD_InitCopy
#define _pdeefilefd_tp_copy_assign &_deefilefd_tp_copy_assign
DEE_COMPILER_MSVC_WARNING_PUSH(4701)
int DEE_CALL _deefilefd_tp_copy_assign(
 DeeFileFDObject *self, DeeFileFDObject *right) {
 struct DeeSysFD new_fd,old_fd;
 Dee_uint32_t newflags,oldflags;
 if (self != right) {
  DeeFileFD_ACQUIRE_SHARED(right,{
   /* NOTE: new_fd is intentionally left uninitialized. */
   newflags = right->fo_flags;
   goto after_copy;
  });
  newflags = right->fo_flags;
  // NOTE: Unlike within the copy-constructor, here we
  //       inherit the ownership attribute from 'right', too.
  if ((newflags&DEE_PRIVATE_FILEFLAG_FD_OWNED)!=0) {
   DeeSysFD_InitCopy(&new_fd,&right->fd_descr,{
    DeeFileFD_RELEASE_SHARED(right);
    return -1;
   });
  } else {
   new_fd = *&right->fd_descr;
  }
  DeeFileFD_RELEASE_SHARED(right);
after_copy:
  DeeFileFD_ACQUIRE_EXCLUSIVE(self);
  oldflags = self->fo_flags;
  old_fd = *&self->fd_descr;
  self->fo_flags = newflags;
  *&self->fd_descr = new_fd;
  DeeFileFD_RELEASE_EXCLUSIVE(self);
#ifdef DeeSysFD_Quit
  if ((oldflags&(DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) ==
                (DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) {
   DeeSysFD_Quit(&old_fd);
  }
#endif
 }
 return 0;
}
DEE_COMPILER_MSVC_WARNING_POP
#else
#define _pdeefilefd_tp_copy_assign DeeType_DEFAULT_SLOT(tp_copy_assign)
#endif


#ifdef DEE_PLATFORM_UNIX
#define _pdeefilefd_tp_str &_deefilefd_tp_str
DeeObject *DEE_CALL _deefilefd_tp_str(DeeFileFDObject *self) {
 return DeeUnixSysFD_DoGetUtf8Name(&self->fd_descr->unx_fd);
}
#else
#define _pdeefilefd_tp_str DeeType_DEFAULT_SLOT(tp_str)
#endif

int DEE_CALL _deefilefd_tp_bool(DeeFileFDObject *self) {
 int result;
 DeeFile_ACQUIRE(self);
 result = ((self->fo_flags&DEE_PRIVATE_FILEFLAG_FD_VALID) != 0);
 DeeFile_RELEASE(self);
 return result;
}
DeeObject *DEE_CALL _deefilefd_tp_not(DeeFileFDObject *self) {
 int result;
 DeeFile_ACQUIRE(self);
 result = ((self->fo_flags&DEE_PRIVATE_FILEFLAG_FD_VALID) == 0);
 DeeFile_RELEASE(self);
 DeeReturn_Bool(result);
}


static int DEE_CALL _deefilefd_tp_io_read(
 DeeFileFDObject *self, void *p, Dee_size_t s, Dee_size_t *rs) {
#ifdef DeeSysFD_Read
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileFD_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed);
  return -1;
 });
 DeeSysFD_Read(&self->fd_descr,p,s,rs,{
  DeeFileFD_RELEASE_SHARED(self);
  return -1;
 });
 DeeFileFD_RELEASE_SHARED(self);
 return 0;
#else
 (void)self,p,s,rs;
 DeeError_NotImplemented_str("file.fd.read");
 return -1;
#endif
}
static int DEE_CALL _deefilefd_tp_io_write(
 DeeFileFDObject *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
#ifdef DeeSysFD_Write
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileFD_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed);
  return -1;
 });
 DeeSysFD_Write(&self->fd_descr,p,s,ws,{
  DeeFileFD_RELEASE_SHARED(self);
  return -1;
 });
 DeeFileFD_RELEASE_SHARED(self);
 return 0;
#else
 (void)self,p,s,ws;
 DeeError_NotImplemented_str("file.fd.write");
 return -1;
#endif
}
static int DEE_CALL _deefilefd_tp_io_seek(
 DeeFileFDObject *self, Dee_int64_t off, int whence, Dee_uint64_t *pos) {
#ifdef DeeSysFD_Seek
 // If the sysfd seek modes differ from deemon's static codes, fix the differences
 // NOTE: This is mearly meant as a precausion.
 //       Both windows and linux use 0,1,2 for these modes, just like deemon does.
 DEE_FILEFD_FIX_SEEKWHENCE(whence);
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileFD_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed);
  return -1;
 });
 DeeSysFD_Seek(&self->fd_descr,off,whence,pos,{
  DeeFileFD_RELEASE_SHARED(self);
  return -1;
 });
 DeeFileFD_RELEASE_SHARED(self);
 return 0;
#else
 (void)self,off,whence,pos;
 DeeError_NotImplemented_str("file.fd.seek");
 return -1;
#endif
}
static int DEE_CALL _deefilefd_tp_io_flush(DeeFileFDObject *self) {
#ifdef DeeSysFD_Flush
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileFD_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed);
  return -1;
 });
 DeeSysFD_Flush(&self->fd_descr,{
  DeeFileFD_RELEASE_SHARED(self);
  return -1;
 });
 DeeFileFD_RELEASE_SHARED(self);
 return 0;
#else
 (void)self;
 DeeError_NotImplemented_str("file.fd.flush");
 return -1;
#endif
}
static int DEE_CALL _deefilefd_tp_io_trunc(DeeFileFDObject *self) {
#ifdef DeeSysFD_Trunc
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileFD_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed);
  return -1;
 });
 DeeSysFD_Trunc(&self->fd_descr,{
  DeeFileFD_RELEASE_SHARED(self);
  return -1;
 });
 DeeFileFD_RELEASE_SHARED(self);
 return 0;
#else
 (void)self;
 DeeError_NotImplemented_str("file.fd.trunc");
 return -1;
#endif
}
#ifdef DeeSysFD_Quit
#define _pdeefilefd_tp_io_close &_deefilefd_tp_io_close
static void DEE_CALL _deefilefd_tp_io_close(DeeFileFDObject *self) {
 DeeFileFD_ACQUIRE_EXCLUSIVE(self,return);
 DeeSysFD_Quit(&self->fd_descr);
 self->fo_flags &= ~DEE_PRIVATE_FILEFLAG_FD_VALID;
 DeeFileFD_RELEASE_EXCLUSIVE(self);
}
#else
#define _pdeefilefd_tp_io_close DeeType_DEFAULT_SLOT(tp_io_close)
#endif


#ifdef DeeSysFD_GetSize
#define _deefilefd_size _deefilefd_size
static DeeObject *DEE_CALL _deefilefd_size(
 DeeFileFDObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint64_t fp_size;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":size") != 0) return NULL;
 DeeFileFD_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return NULL; });
 DeeSysFD_GetSize(&self->fd_descr,&fp_size,{ DeeFileFD_RELEASE_SHARED(self); return NULL; });
 DeeFileFD_RELEASE_SHARED(self);
 return DeeObject_New(Dee_uint64_t,fp_size);
}
#endif /* DeeSysFileFD_GetSize */

#ifdef DEE_PLATFORM_WINDOWS
#define _deefilefd_win32_handle _deefilefd_win32_handle
static DeeObject *DEE_CALL _deefilefd_win32_handle(
 DeeFileFDObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 HANDLE result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":win32_handle") != 0) return NULL;
 DeeFileFD_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return NULL; });
 result = self->fd_descr.w32_handle;
 DeeFileFD_RELEASE_SHARED(self);
 return DeeVoidPointer_New(result);
}
#endif

#ifdef DEE_PLATFORM_UNIX
#define _deefilefd_posix_fileno _deefilefd_posix_fileno
static DeeObject *DEE_CALL _deefilefd_posix_fileno(
 DeeFileFDObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":posix_fileno") != 0) return NULL;
 DeeFileFD_ACQUIRE_SHARED(self,{ DeeError_Throw(DeeErrorInstance_FileFDAlreadyClosed); return NULL; });
 result = self->fd_descr.unx_fd;
 DeeFileFD_RELEASE_SHARED(self);
 return DeeObject_New(int,result);
}
#endif

#if defined(_deefilefd_size)\
 || defined(_deefilefd_win32_handle)\
 || defined(_deefilefd_posix_fileno)
static struct DeeMethodDef const _deefilefd_tp_methods[] = {
#ifdef _deefilefd_win32_handle
 DEE_METHODDEF_v100("win32_handle",member(&_deefilefd_win32_handle),"() -> HANDLE"),
#endif
#ifdef _deefilefd_posix_fileno
 DEE_METHODDEF_v100("posix_fileno",member(&_deefileio_fileno),"() -> int"),
#endif
#ifdef _deefilefd_size
 DEE_METHODDEF_v100("size",member(&_deefilefd_size),"() -> uint64_t"),
#endif
 DEE_METHODDEF_END_v100
};
#else
#define _deefilefd_tp_methods DeeType_DEFAULT_SLOT(tp_methods)
#endif




DeeFileTypeObject DeeFileFD_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("file.fd"),member(
    "The generalized root-type of all filedescriptor-based file types"),
   null,member((DeeTypeObject *)&DeeFile_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFileFDObject),null,
   member(&_deefilefd_tp_ctor),member(_pdeefilefd_tp_copy_ctor),
   member(&_deefilefd_tp_move_ctor),member(_pdeefilefd_tp_any_ctor)),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(_pdeefilefd_tp_dtor)),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(_pdeefilefd_tp_copy_assign),
   member(&_deefilefd_tp_move_assign),null),
  DEE_TYPE_OBJECT_CAST_v101(
   member(_pdeefilefd_tp_str),null,null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
  DEE_TYPE_OBJECT_MATH_v101(
   member(&_deefilefd_tp_not),
   member(&_deefilefd_tp_bool),
   null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,
   member(_deefilefd_tp_methods),null,null,null),
  DEE_TYPE_OBJECT_FOOTER_v100
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(
  member(&_deefilefd_tp_io_read),
  member(&_deefilefd_tp_io_write),
  member(&_deefilefd_tp_io_seek),
  member(&_deefilefd_tp_io_flush),
  member(&_deefilefd_tp_io_trunc),
  member(_pdeefilefd_tp_io_close))
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};

DEE_DECL_END

#endif /* !GUARD_DEEMON_FILE_FD_C_INL */
