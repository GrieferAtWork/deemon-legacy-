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
#ifndef GUARD_DEEMON_FILE_FD_INL
#define GUARD_DEEMON_FILE_FD_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/bool.h>
#include <deemon/file.h>

// /src/*
#include <deemon/sys/sysfd.h>

// */ (nano...)

DEE_DECL_BEGIN

DEE_STATIC_ASSERT(DEE_PRIVATE_FILEFD_DESCRITOR_SIZE == sizeof(struct DeeSysFD));

#define DeeFileFD_FD(ob) ((struct DeeSysFD *)((DeeFileFDObject *)Dee_REQUIRES_POINTER(ob))->fd_data)

static int _deefilefd_tp_ctor(
 DeeFileTypeObject *DEE_UNUSED(tp_self), DeeFileFDObject *self) {
 // Don't set the 'DEE_PRIVATE_FILEFLAG_FD_VALID' flag here.
 DeeFileFD_InitBasic(self,DEE_FILE_FLAG_NONE);
 return 0;
}

#ifdef DeeSysFD_InitCopy /*< The system supports fd duplication. */
#define _pdeefilefd_tp_copy_ctor &_deefilefd_tp_copy_ctor
static int _deefilefd_tp_copy_ctor(
 DeeFileTypeObject *DEE_UNUSED(tp_self),
 DeeFileFDObject *self, DeeFileFDObject *right) {
 Dee_uint32_t newflags;
 // In here, we always copy the file-descriptor
 DeeFileFD_ACQUIRE_SHARED(right,{
  DeeFileFD_InitBasic(self,DEE_FILE_FLAG_NONE);
  return 0;
 });
 DeeSysFD_InitCopy(DeeFileFD_FD(self),DeeFileFD_FD(right),{
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

static int _deefilefd_tp_move_ctor(
 DeeFileTypeObject *DEE_UNUSED(tp_self),
 DeeFileFDObject *self, DeeFileFDObject *right) {
 Dee_uint32_t newflags;
 DeeFileFD_ACQUIRE_EXCLUSIVE(right,{});
 newflags = right->fo_flags;
 *DeeFileFD_FD(self) = *DeeFileFD_FD(right);
 right->fo_flags = DEE_FILE_FLAG_NONE;
 DeeFileFD_RELEASE_EXCLUSIVE(right);
 DeeFileFD_InitBasic(self,newflags);
 return 0;
}

#if defined(DEE_PLATFORM_WINDOWS)\
 || defined(DEE_PLATFORM_UNIX)
#define _pdeefilefd_tp_any_ctor &_deefilefd_tp_any_ctor
static int _deefilefd_tp_any_ctor(
 DeeFileTypeObject *DEE_UNUSED(tp_self),
 DeeFileFDObject *self, DeeObject *args) {
 Dee_filedescr_t new_fd;
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"p:file.fd",&new_fd) != 0) return -1;
 DeeFileFD_FD(self)->w32_handle = (HANDLE)new_fd;
#else
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"d:file.fd",&new_fd) != 0) return -1;
 DeeFileFD_FD(self)->unx_fd = new_fd;
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
static void _deefilefd_tp_dtor(DeeFileFDObject *self) {
 if ((self->fo_flags&(DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) == 
                     (DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) {
  DeeSysFD_Quit(DeeFileFD_FD(self));
 }
}
#else
#define _pdeefilefd_tp_dtor DeeType_DEFAULT_SLOT(tp_dtor)
#endif


static int _deefilefd_tp_move_assign(
 DeeFileFDObject *self, DeeFileFDObject *right) {
 struct DeeSysFD new_fd,old_fd;
 Dee_uint32_t newflags,oldflags;
 if (self != right) {
  DeeFileFD_ACQUIRE_EXCLUSIVE(right);
  new_fd = *DeeFileFD_FD(right);
  newflags = right->fo_flags;
  right->fo_flags = DEE_FILE_FLAG_NONE;
  DeeFileFD_RELEASE_EXCLUSIVE(right);
  DeeFileFD_ACQUIRE_EXCLUSIVE(self);
  oldflags = self->fo_flags;
  old_fd = *DeeFileFD_FD(self);
  self->fo_flags = newflags;
  *DeeFileFD_FD(self) = new_fd;
  DeeFileFD_RELEASE_EXCLUSIVE(self);
  if ((oldflags&(DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) ==
                (DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) {
   DeeSysFD_Quit(&old_fd);
  }
 }
 return 0;
}

#ifdef DeeSysFD_InitCopy
#define _pdeefilefd_tp_copy_assign &_deefilefd_tp_copy_assign
static int _deefilefd_tp_copy_assign(
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
   DeeSysFD_InitCopy(&new_fd,DeeFileFD_FD(right),{
    DeeFileFD_RELEASE_SHARED(right);
    return -1;
   });
  } else {
   new_fd = *DeeFileFD_FD(right);
  }
  DeeFileFD_RELEASE_SHARED(right);
after_copy:
  DeeFileFD_ACQUIRE_EXCLUSIVE(self);
  oldflags = self->fo_flags;
  old_fd = *DeeFileFD_FD(self);
  self->fo_flags = newflags;
  *DeeFileFD_FD(self) = new_fd;
  DeeFileFD_RELEASE_EXCLUSIVE(self);
  if ((oldflags&(DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) ==
                (DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) {
   DeeSysFD_Quit(&old_fd);
  }
 }
 return 0;
}
#else
#define _pdeefilefd_tp_copy_assign DeeType_DEFAULT_SLOT(tp_copy_assign)
#endif


#ifdef DEE_PLATFORM_UNIX
#define _pdeefilefd_tp_str &_deefilefd_tp_str
static DeeObject *_deefilefd_tp_str(DeeFileFDObject *self) {
 return DeeUnixSysFD_DoGetUtf8Name(DeeFileFD_FD(self)->unx_fd);
}
#else
#define _pdeefilefd_tp_str DeeType_DEFAULT_SLOT(tp_str)
#endif

static int _deefilefd_tp_bool(DeeFileFDObject *self) {
 int result;
 DeeFile_ACQUIRE(self);
 result = ((self->fo_flags&DEE_PRIVATE_FILEFLAG_FD_VALID) != 0);
 DeeFile_RELEASE(self);
 return result;
}
static DeeObject *_deefilefd_tp_not(DeeFileFDObject *self) {
 int result;
 DeeFile_ACQUIRE(self);
 result = ((self->fo_flags&DEE_PRIVATE_FILEFLAG_FD_VALID) == 0);
 DeeFile_RELEASE(self);
 DeeReturn_Bool(result);
}


DeeError_NEW_STATIC(_error_file_closed,&DeeErrorType_IOError,
                    "Invalid file descriptor");
#define throw_error_file_closed() DeeError_Throw((DeeObject *)&_error_file_closed)

static int DEE_CALL _deefilefd_tp_io_read(
 DeeFileFDObject *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileFD_ACQUIRE_SHARED(self,{throw_error_file_closed(); return -1;});
 DeeSysFD_Read(DeeFileFD_FD(self),p,s,rs,return -1);
 DeeFileFD_RELEASE_SHARED(self);
 return 0;
}
static int DEE_CALL _deefilefd_tp_io_write(
 DeeFileFDObject *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileFD_ACQUIRE_SHARED(self,{throw_error_file_closed(); return -1;});
 DeeSysFD_Write(DeeFileFD_FD(self),p,s,ws,return -1);
 DeeFileFD_RELEASE_SHARED(self);
 return 0;
}
static int DEE_CALL _deefilefd_tp_io_seek(
 DeeFileFDObject *self, Dee_int64_t off, int whence, Dee_uint64_t *pos) {
 // If the sysfd seek modes differ from deemon's static codes, fix the differences
 // NOTE: This is mearly meant as a precausion.
 //       Both windows and linux use 0,1,2 for these modes, just like deemon does.
#if DEE_SYSFD_SEEK_SET != DEE_SEEK_SET
 if (whence == DEE_SEEK_SET) whence = DEE_SYSFD_SEEK_SET; else
#endif
#if DEE_SYSFD_SEEK_CUR != DEE_SEEK_CUR
 if (whence == DEE_SEEK_CUR) whence = DEE_SYSFD_SEEK_CUR; else
#endif
#if DEE_SYSFD_SEEK_END != DEE_SEEK_END
 if (whence == DEE_SEEK_END) whence = DEE_SYSFD_SEEK_END; else
#endif
 ;
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFileFD_ACQUIRE_SHARED(self,{throw_error_file_closed(); return -1;});
 DeeSysFD_Seek(DeeFileFD_FD(self),off,whence,pos,return -1);
 DeeFileFD_RELEASE_SHARED(self);
 return 0;
}
static int DEE_CALL _deefilefd_tp_io_flush(DeeFileFDObject *self) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
#ifdef DeeSysFD_Flush
 DeeFileFD_ACQUIRE_SHARED(self,{throw_error_file_closed(); return -1;});
 DeeSysFD_Flush(DeeFileFD_FD(self),return -1);
 DeeFileFD_RELEASE_SHARED(self);
 return 0;
#else
 DeeError_NotImplemented_str("file.fp.flush");
 return -1;
#endif
}
static int DEE_CALL _deefilefd_tp_io_trunc(DeeFileFDObject *self) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
#ifdef DeeSysFD_Trunc
 DeeFileFD_ACQUIRE_SHARED(self,{throw_error_file_closed(); return -1;});
 DeeSysFD_Trunc(DeeFileFD_FD(self),return -1);
 DeeFileFD_RELEASE_SHARED(self);
 return 0;
#else
 DeeError_NotImplemented_str("file.fp.trunc");
 return -1;
#endif
}
#ifdef DeeSysFD_Quit
#define _pdeefilefd_tp_io_close &_deefilefd_tp_io_close
static void DEE_CALL _deefilefd_tp_io_close(DeeFileFDObject *self) {
 DeeFileFD_ACQUIRE_EXCLUSIVE(self,return);
 DeeSysFD_Quit(DeeFileFD_FD(self));
 self->fo_flags &= ~DEE_PRIVATE_FILEFLAG_FD_VALID;
 DeeFileFD_RELEASE_EXCLUSIVE(self);
}
#else
#define _pdeefilefd_tp_io_close DeeType_DEFAULT_SLOT(tp_io_close)
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
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
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

#endif /* !GUARD_DEEMON_FILE_FD_INL */
