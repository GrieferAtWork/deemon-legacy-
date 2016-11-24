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
#ifndef GUARD_DEEMON_PIPE_C
#define GUARD_DEEMON_PIPE_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

#if DEE_PLATFORM_HAVE_PIPES

// /include/*
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/integer.h>
#include <deemon/memberdef.h>
#include <deemon/mp/pipe.h>
#include <deemon/mp/thread.h>
#include <deemon/none.h>
#include <deemon/string.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/structured.h>
#include <deemon/tuple.h>

// /scr/*
#include <deemon/__xconf.inl>
#include <deemon/file/file.fd.h>
#include <deemon/sys/sysfd.h>
#include DEE_INCLUDE_MEMORY_API()

// */ (nano...)


DEE_DECL_BEGIN

#ifdef DEE_LIMITED_API
struct DeePipeObject {
#ifdef DeeSysPipeFD
 DEE_FILE_FD_OBJECT_HEAD
 struct DeeSysPipeFD p_descr; /*< Pipe file descriptor. */
#else
 DEE_FILE_OBJECT_HEAD
#endif
};
#endif


DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int DeePipe_New(
 DEE_A_REF DEE_A_OUT_OBJECT(DeePipeObject) **reader,
 DEE_A_REF DEE_A_OUT_OBJECT(DeePipeObject) **writer,
 DEE_A_IN Dee_size_t size_hint) {
#ifdef DeeSysPipeFD_InitEx
 DeePipeObject *r,*w; (void)size_hint;
 DEE_ASSERT(reader && writer);
 if DEE_UNLIKELY((r = DeeObject_MALLOCF(DeePipeObject,"pipe.reader (size_hint = %lu)",(unsigned long)size_hint)) == NULL) return -1;
 if DEE_UNLIKELY((w = DeeObject_MALLOCF(DeePipeObject,"pipe.writer (size_hint = %lu)",(unsigned long)size_hint)) == NULL) {err_free_r: DeeObject_Free(r); return -1; }
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) {err_free_rw: DeeObject_Free(w); goto err_free_r; }
 DeeSysPipeFD_InitEx(&r->p_descr,&w->p_descr,size_hint,goto err_free_rw);
 DeeObject_INIT(r,(DeeTypeObject *)&DeePipeReader_Type);
 DeeObject_INIT(w,(DeeTypeObject *)&DeePipeWriter_Type);
 DeeFileFD_InitBasic(r,DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED);
 DeeFileFD_InitBasic(w,DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED);
 Dee_INHERIT_REF(*(DeePipeObject **)reader,r);
 Dee_INHERIT_REF(*(DeePipeObject **)writer,w);
 return 0;
#else
 (void)reader,writer,size_hint;
 DeeError_NotImplemented_str("pipe");
 return -1;
#endif
}




//////////////////////////////////////////////////////////////////////////
// Pipe VTable
#if !defined(DEE_PLATFORM_UNIX) && defined(DeeSysPipeFD)
static DeeObject *DEE_CALL _DeePipe_ToString(DeePipeObject *self, char const *name) {
 DeeObject *result;
 DeeFileFD_ACQUIRE_SHARED(self,{ return DeeString_Newf("<%s(CLOSED)>",name); });
#ifdef DEE_PLATFORM_WINDOWS
 result = DeeString_Newf("<%s(%p)>",name,self->p_descr.w32_handle);
#else
 result = DeeString_Newf("<%s(%.*q)>",name,(unsigned)sizeof(self->p_descr),&self->p_descr);
#endif
 DeeFileFD_RELEASE_SHARED(self);
 return result;
}

#define _pdeepipe_tp_str       &_deepipe_tp_str
#define _pdeepipereader_tp_str &_deepipereader_tp_str
#define _pdeepipewriter_tp_str &_deepipewriter_tp_str
static DeeObject *DEE_CALL _deepipe_tp_str(DeePipeObject *self) { return _DeePipe_ToString(self,"pipe"); }
static DeeObject *DEE_CALL _deepipereader_tp_str(DeePipeObject *self) { return _DeePipe_ToString(self,"pipe.reader"); }
static DeeObject *DEE_CALL _deepipewriter_tp_str(DeePipeObject *self) { return _DeePipe_ToString(self,"pipe.writer"); }
#else
#define _pdeepipe_tp_str       DeeType_DEFAULT_SLOT(tp_str)
#define _pdeepipereader_tp_str DeeType_DEFAULT_SLOT(tp_str)
#define _pdeepipewriter_tp_str DeeType_DEFAULT_SLOT(tp_str)
#endif



#ifdef DeeSysPipeFD_InitCopy
#define _pdeepipe_tp_copy_ctor &_deepipe_tp_copy_ctor
static int DEE_CALL _deepipe_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeePipeObject *self, DeePipeObject *right) {
 Dee_uint32_t newflags;
 // In here, we always copy the file-descriptor
 DeePipeFD_ACQUIRE_SHARED(right,{ DeePipeFD_InitBasic(self,DEE_PRIVATE_FILEFLAG_NONE); return 0; });
 DeeSysPipeFD_InitCopy(&self->p_descr,&right->p_descr,{ DeePipeFD_RELEASE_SHARED(right); return -1; });
 newflags = right->fo_flags;
 DeePipeFD_RELEASE_SHARED(right);
 DeePipeFD_InitBasic(self,newflags|DEE_PRIVATE_FILEFLAG_FD_OWNED);
 return 0;
}
DEE_COMPILER_MSVC_WARNING_PUSH(4701)
#define _pdeepipe_tp_copy_assign &_deepipe_tp_copy_assign
static int DEE_CALL _deepipe_tp_copy_assign(
 DeePipeObject *self, DeePipeObject *right) {
 struct DeeSysPipeFD new_fd,old_fd;
 Dee_uint32_t newflags,oldflags;
 if (self != right) {
  DeePipeFD_ACQUIRE_SHARED(right,{
   /* NOTE: new_fd is intentionally left uninitialized. */
   newflags = right->fo_flags;
   goto after_copy;
  });
  newflags = right->fo_flags;
  // NOTE: Unlike within the copy-constructor, here we
  //       inherit the ownership attribute from 'right', too.
  if ((newflags&DEE_PRIVATE_FILEFLAG_FD_OWNED)!=0) {
   DeeSysPipeFD_InitCopy(&new_fd,&right->p_descr,{ DeePipeFD_RELEASE_SHARED(right); return -1; });
  } else {
   new_fd = right->p_descr;
  }
  DeePipeFD_RELEASE_SHARED(right);
after_copy:
  DeePipeFD_ACQUIRE_EXCLUSIVE(self);
  oldflags = self->fo_flags;
  old_fd = self->p_descr;
  self->fo_flags = newflags;
  self->p_descr = new_fd;
  DeePipeFD_RELEASE_EXCLUSIVE(self);
#if defined(DeeSysPipeFD_Quit) || defined(DeeSysFD_Quit)
  if ((oldflags&(DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) ==
                (DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) {
#ifdef DeeSysPipeFD_Quit
   DeeSysPipeFD_Quit(&old_fd);
#endif
#ifdef DeeSysFD_Quit
   DeeSysFD_Quit(&old_fd);
#endif
  }
#endif
 }
 return 0;
}
DEE_COMPILER_MSVC_WARNING_POP
#else
#define _pdeepipe_tp_copy_ctor   _pdeefilefd_tp_copy_ctor
#define _pdeepipe_tp_copy_assign _pdeefilefd_tp_copy_assign
#endif

#if defined(DeeSysPipeFD_InitCopy) || defined(DeeSysPipeFD_Quit)
#define _pdeepipe_tp_move_ctor &_deepipe_tp_move_ctor
static int _deepipe_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeePipeObject *self, DeePipeObject *right) {
 Dee_uint32_t newflags;
 DeePipeFD_ACQUIRE_EXCLUSIVE(right,{});
 newflags = right->fo_flags;
 self->p_descr = right->p_descr;
 right->fo_flags = DEE_PRIVATE_FILEFLAG_NONE;
 DeePipeFD_RELEASE_EXCLUSIVE(right);
 DeePipeFD_InitBasic(self,newflags);
 return 0;
}
#define _pdeepipe_tp_move_assign &_deepipe_tp_move_assign
static int DEE_CALL _deepipe_tp_move_assign(
 DeePipeObject *self, DeePipeObject *right) {
 struct DeeSysPipeFD new_fd,old_fd;
 Dee_uint32_t newflags,oldflags;
 if (self != right) {
  DeePipeFD_ACQUIRE_EXCLUSIVE(right);
  new_fd = right->p_descr;
  newflags = right->fo_flags;
  right->fo_flags = DEE_PRIVATE_FILEFLAG_NONE;
  DeePipeFD_RELEASE_EXCLUSIVE(right);
  DeePipeFD_ACQUIRE_EXCLUSIVE(self);
  oldflags = self->fo_flags;
  old_fd = self->p_descr;
  self->fo_flags = newflags;
  self->p_descr = new_fd;
  DeePipeFD_RELEASE_EXCLUSIVE(self);
#if defined(DeeSysPipeFD_Quit) || defined(DeeSysFD_Quit)
  if ((oldflags&(DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) ==
                (DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) {
#ifdef DeeSysPipeFD_Quit
   DeeSysPipeFD_Quit(&old_fd);
#endif
#ifdef DeeSysFD_Quit
   DeeSysFD_Quit(&old_fd);
#endif
  }
#endif
 }
 return 0;
}
#else
#define _pdeepipe_tp_move_ctor   _pdeefilefd_tp_move_ctor
#define _pdeepipe_tp_move_assign _pdeefilefd_tp_move_assign
#endif


//////////////////////////////////////////////////////////////////////////
// Scan for additional callbacks required for pipefd descriptors, and link them.
#ifdef DeeSysPipeFD_Read
#define _pdeepipe_tp_io_read &_deepipe_tp_io_read
static int DEE_CALL _deepipe_tp_io_read(
 DeePipeObject *self, void const const *p, Dee_size_t s, Dee_size_t *rs) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeePipeFD_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_PipeFDAlreadyClosed);
  return -1;
 });
 DeeSysPipeFD_Read(&self->p_descr,p,s,rs,{
  DeePipeFD_RELEASE_SHARED(self);
  return -1;
 });
 DeePipeFD_RELEASE_SHARED(self);
 return 0;
}
#else
#define _pdeepipe_tp_io_read  DeeType_DEFAULT_SLOT(tp_io_read)
#endif
#ifdef DeeSysPipeFD_Write
#define _pdeepipe_tp_io_write &_deepipe_tp_io_write
static int DEE_CALL _deepipe_tp_io_write(
 DeePipeObject *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeePipeFD_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_PipeFDAlreadyClosed);
  return -1;
 });
 DeeSysPipeFD_Write(&self->p_descr,p,s,ws,{
  DeePipeFD_RELEASE_SHARED(self);
  return -1;
 });
 DeePipeFD_RELEASE_SHARED(self);
 return 0;
}
#else
#define _pdeepipe_tp_io_write DeeType_DEFAULT_SLOT(tp_io_write)
#endif
#ifdef DeeSysPipeFD_Seek
#define _pdeepipe_tp_io_seek &_deepipe_tp_io_seek
static int DEE_CALL _deepipe_tp_io_seek(
 DeePipeObject *self, Dee_int64_t off, int whence, Dee_uint64_t *pos) {
 DEE_FILEFD_FIX_SEEKWHENCE(whence,return-1);
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeePipeFD_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_PipeFDAlreadyClosed);
  return -1;
 });
 DeeSysPipeFD_Seek(&self->p_descr,off,whence,pos,{
  DeePipeFD_RELEASE_SHARED(self);
  return -1;
 });
 DeePipeFD_RELEASE_SHARED(self);
 return 0;
}
#else
#define _pdeepipe_tp_io_seek  DeeType_DEFAULT_SLOT(tp_io_seek)
#endif


#ifdef DeeSysPipeFD_Flush
#define _pdeepipe_tp_io_flush &_deepipe_tp_io_flush
static int DEE_CALL _deepipe_tp_io_flush(DeePipeObject *self) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeePipeFD_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_PipeFDAlreadyClosed);
  return -1;
 });
 DeeSysFD_Flush(&self->p_descr,{
  DeePipeFD_RELEASE_SHARED(self);
  return -1;
 });
 DeePipeFD_RELEASE_SHARED(self);
 return 0;
}
#else
#define _pdeepipe_tp_io_flush DeeType_DEFAULT_SLOT(tp_io_flush)
#endif

#ifdef DeeSysPipeFD_Trunc
#define _pdeepipe_tp_io_trunc &_deepipe_tp_io_trunc
static int DEE_CALL _deepipe_tp_io_trunc(DeePipeObject *self) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeePipeFD_ACQUIRE_SHARED(self,{
  DeeError_Throw(DeeErrorInstance_PipeFDAlreadyClosed);
  return -1;
 });
 DeeSysPipeFD_Trunc(&self->p_descr,{
  DeePipeFD_RELEASE_SHARED(self);
  return -1;
 });
 DeePipeFD_RELEASE_SHARED(self);
 return 0;
}
#else
#define _pdeepipe_tp_io_trunc DeeType_DEFAULT_SLOT(tp_io_trunc)
#endif

#ifdef DeeSysPipeFD_Quit
#define _pdeepipe_tp_dtor     &_deepipe_tp_dtor
#define _pdeepipe_tp_io_close &_deepipe_tp_io_close
static void DEE_CALL _deepipe_tp_dtor(DeePipeObject *self) {
 if ((self->fo_flags&(DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) == 
                     (DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED)) {
  self->fo_flags &= ~(DEE_PRIVATE_FILEFLAG_FD_VALID|DEE_PRIVATE_FILEFLAG_FD_OWNED);
  DeeSysPipeFD_Quit(&self->fd_descr);
#ifdef DeeSysFD_Quit
  DeeSysFD_Quit(&self->fd_descr);
#endif
 }
}
static void DEE_CALL _deepipe_tp_io_close(DeePipeObject *self) {
 DeePipeFD_ACQUIRE_EXCLUSIVE(self,return);
 DeeSysPipeFD_Quit(&self->p_descr);
 DeeSysFD_Quit(&self->p_descr);
 self->fo_flags &= ~DEE_PRIVATE_FILEFLAG_FD_VALID;
 DeePipeFD_RELEASE_EXCLUSIVE(self);
}
#else
#define _pdeepipe_tp_dtor     DeeType_DEFAULT_SLOT(tp_dtor)
#define _pdeepipe_tp_io_close DeeType_DEFAULT_SLOT(tp_io_close)
#endif







static DeeObject *DEE_CALL _deepipeclass_new(
 DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *res,*r,*w; Dee_size_t size_hint = 0;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|Iu:pipe.new",&size_hint) != 0) return NULL;
 if DEE_UNLIKELY(DeePipe_New(&r,&w,size_hint) != 0) return NULL;
 res = DeeTuple_Pack(2,r,w);
 Dee_DECREF(w);
 Dee_DECREF(r);
 return res;
}

static struct DeeMemberDef const _deepipe_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("reader",object,&DeePipeReader_Type),
 DEE_MEMBERDEF_CONST_v100("writer",object,&DeePipeWriter_Type),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMethodDef const _deepipe_tp_class_methods[] = {
 DEE_METHODDEF_v100("new",member(&_deepipeclass_new),
  "(size_t size_hint = 0) -> (pipe.reader,pipe.writer)\n"
  "@return: A tuple containing a new pipe reader and writer"),
 DEE_METHODDEF_END_v100
};

DeeFileTypeObject DeePipe_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("pipe"),null,null,
   member((DeeTypeObject *)&DeeFileFD_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeePipeObject),null,
   member(_pdeefilefd_tp_ctor),
   member(_pdeepipe_tp_copy_ctor),
   member(_pdeepipe_tp_move_ctor),null),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(_pdeepipe_tp_dtor)),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(_pdeepipe_tp_copy_assign),
   member(_pdeepipe_tp_move_assign),null),
  DEE_TYPE_OBJECT_CAST_v101(
   member(_pdeepipe_tp_str),
   member(_pdeepipe_tp_str),null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
  DEE_TYPE_OBJECT_MATH_v101(
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,
   member(_deepipe_tp_class_members),null,
   member(_deepipe_tp_class_methods)),
  DEE_TYPE_OBJECT_FOOTER_v100
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(
  member(_pdeepipe_tp_io_read),
  member(_pdeepipe_tp_io_write),
  member(_pdeepipe_tp_io_seek),
  member(_pdeepipe_tp_io_flush),
  member(_pdeepipe_tp_io_trunc),
  member(_pdeepipe_tp_io_close))
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};
DeeFileTypeObject DeePipeReader_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("pipe.reader"),null,null,
   member((DeeTypeObject *)&DeePipe_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeePipeObject),null,
   member(_pdeefilefd_tp_ctor),
   member(_pdeepipe_tp_copy_ctor),
   member(_pdeepipe_tp_move_ctor),null),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(_pdeepipe_tp_copy_assign),
   member(_pdeepipe_tp_move_assign),null),
  DEE_TYPE_OBJECT_CAST_v101(
   member(_pdeepipereader_tp_str),
   member(_pdeepipereader_tp_str),null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
  DEE_TYPE_OBJECT_MATH_v101(
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_FOOTER_v100
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(
  member(_pdeepipe_tp_io_read),
  member(_pdeepipe_tp_io_write),
  member(_pdeepipe_tp_io_seek),
  member(_pdeepipe_tp_io_flush),
  member(_pdeepipe_tp_io_trunc),
  member(_pdeepipe_tp_io_close))
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};
DeeFileTypeObject DeePipeWriter_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("pipe.writer"),null,null,
   member((DeeTypeObject *)&DeePipe_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeePipeObject),null,
   member(_pdeefilefd_tp_ctor),
   member(_pdeepipe_tp_copy_ctor),
   member(_pdeepipe_tp_move_ctor),null),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(_pdeepipe_tp_copy_assign),
   member(_pdeepipe_tp_move_assign),null),
  DEE_TYPE_OBJECT_CAST_v101(
   member(_pdeepipewriter_tp_str),
   member(_pdeepipewriter_tp_str),null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
  DEE_TYPE_OBJECT_MATH_v101(
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_FOOTER_v100
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(
  member(_pdeepipe_tp_io_read),
  member(_pdeepipe_tp_io_write),
  member(_pdeepipe_tp_io_seek),
  member(_pdeepipe_tp_io_flush),
  member(_pdeepipe_tp_io_trunc),
  member(_pdeepipe_tp_io_close))
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};


DEE_DECL_END
#endif /* DEE_PLATFORM_HAVE_PIPES */

#endif /* !GUARD_DEEMON_PIPE_C */
