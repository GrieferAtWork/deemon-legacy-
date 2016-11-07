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

#include DEE_INCLUDE_MEMORY_API_DISABLE()
#ifdef DEE_PLATFORM_WINDOWS
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#elif defined(DEE_PLATFORM_UNIX)
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#include <errno.h>
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H
#include <unistd.h>
#endif
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()

// /scr/*
#include <deemon/__xconf.inl>
#include DEE_INCLUDE_MEMORY_API()

// */ (nano...)


DEE_DECL_BEGIN

DEE_A_RET_OBJECT_EXCEPT_REF(DeePipeObject) *
_DeePipeReader_NewFromInheritedHandle(DEE_A_IN DeeIOHandle handle) {
 DeePipeObject *result;
 if ((result = DeeObject_MALLOCF(DeePipeObject,
  "pipe.reader (handle = %lu)",(unsigned long)handle)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DeePipeReader_Type);
  _DeeFile_Init(result);
  result->p_handle = handle;
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeePipeObject) *
_DeePipeWriter_NewFromInheritedHandle(DEE_A_IN DeeIOHandle handle) {
 DeePipeObject *result;
 if ((result = DeeObject_MALLOCF(DeePipeObject,
  "pipe.writer (handle = %lu)",(unsigned long)handle)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DeePipeWriter_Type);
  _DeeFile_Init(result);
  result->p_handle = handle;
 }
 return (DeeObject *)result;
}

DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int DeePipe_New(
 DEE_A_REF DEE_A_OUT_OBJECT(DeePipeObject) **reader,
 DEE_A_REF DEE_A_OUT_OBJECT(DeePipeObject) **writer,
 DEE_A_IN Dee_size_t size_hint) {
 DeePipeObject *r,*w;
 DEE_ASSERT(reader && writer);
 if DEE_UNLIKELY((r = DeeObject_MALLOCF(DeePipeObject,"pipe.reader (size_hint = %lu)",(unsigned long)size_hint)) == NULL) return -1;
 if DEE_UNLIKELY((w = DeeObject_MALLOCF(DeePipeObject,"pipe.writer (size_hint = %lu)",(unsigned long)size_hint)) == NULL) {err_free_r: DeeObject_Free(r); return -1; }
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) goto err_free_rw;
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(!CreatePipe((PHANDLE)&r->p_handle,(PHANDLE)&w->p_handle,NULL,(DWORD)size_hint)) {
  DeeError_SystemError("CreatePipe");
err_free_rw:
  DeeObject_Free(w);
  goto err_free_r;
 }
#elif defined(DEE_PLATFORM_UNIX)
 (void)size_hint; // unused
 {int handles[2];
  if DEE_UNLIKELY(pipe(handles) != 0) {
   DeeError_SystemError("pipe");
err_free_rw:
   DeeObject_Free(w);
   goto err_free_r;
  }
  r->p_handle = handles[0];
  w->p_handle = handles[1];
 }
#endif
 DeeObject_INIT(r,(DeeTypeObject *)&DeePipeReader_Type);
 DeeObject_INIT(w,(DeeTypeObject *)&DeePipeWriter_Type);
 _DeeFile_Init(r);
 _DeeFile_Init(w);
 Dee_INHERIT_REF(*(DeePipeObject **)reader,r);
 Dee_INHERIT_REF(*(DeePipeObject **)writer,w);
 return 0;
}




//////////////////////////////////////////////////////////////////////////
// Pipe VTable
static void DEE_CALL _deepipe_tp_dtor(DeePipeObject *self) {
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(self->p_handle && !CloseHandle(self->p_handle)) SetLastError(0);
#elif defined(DEE_PLATFORM_UNIX)
 if DEE_UNLIKELY(self->p_handle > 0 && close(self->p_handle) != 0) errno = 0;
#endif
}
static DeeObject *DEE_CALL _deepipe_tp_str(DeePipeObject *self) {
#ifdef DEE_PLATFORM_WINDOWS
 return DeeString_Newf("<pipe: %#Ix>",self->p_handle);
#elif defined(DEE_PLATFORM_UNIX)
 return DeeString_Newf("<pipe: %d>",self->p_handle);
#endif
}
static int DEE_CALL _deepipe_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeePipeObject *self, DeePipeObject *right) {
 _DeeFile_InitMove(self,right);
 self->p_handle = right->p_handle;
#ifdef DEE_PLATFORM_WINDOWS
 right->p_handle = NULL;
#elif defined(DEE_PLATFORM_UNIX)
 right->p_handle = -1;
#endif
 return 0;
}
static int DEE_CALL _deepipe_tp_move_assign(
 DeePipeObject *self, DeePipeObject *right) {
 if DEE_LIKELY(self != right) {
  Dee_uint32_t new_flags;
  DeeIOHandle new_handle;
  DeeFile_ACQUIRE(right);
  new_flags = right->fo_flags;
  new_handle = right->p_handle;
#ifdef DEE_PLATFORM_WINDOWS
  right->p_handle = NULL;
#elif defined(DEE_PLATFORM_UNIX)
  right->p_handle = -1;
#endif
  DeeFile_RELEASE(right);
  DeeFile_ACQUIRE(self);
  _deepipe_tp_dtor(self);
  self->p_handle = new_handle;
  self->fo_flags = new_flags;
  DeeFile_RELEASE(self);
 }
 return 0;
}
static int DEE_CALL _deepipe_tp_io_flush(DeePipeObject *DEE_UNUSED(self)) {
 return 0;
}
static void DEE_CALL _deepipe_tp_io_close(DeePipeObject *self) {
 DeeFile_ACQUIRE(self);
 if DEE_LIKELY(self->p_handle) {
#ifdef DEE_PLATFORM_WINDOWS
  if DEE_UNLIKELY(!CloseHandle(self->p_handle)) SetLastError(0);
  self->p_handle = NULL;
#elif defined(DEE_PLATFORM_UNIX)
  if DEE_UNLIKELY(close(self->p_handle) != 0) errno = 0;
  self->p_handle = -1;
#endif
 }
 DeeFile_RELEASE(self);
}



#ifdef DEE_PLATFORM_WINDOWS
#ifndef SIZEOF_DWORD
#define SIZEOF_DWORD 4
#endif
#ifndef DWORD_MAX
#define DWORD_MAX 0xFFFFFFFF
#endif
#endif


//////////////////////////////////////////////////////////////////////////
// Pipe reader VTable
static DeeObject *DEE_CALL _deepipereader_tp_str(DeePipeObject *self) {
 DeeIOHandle handle;
 DeeFile_ACQUIRE(self);
 handle = self->p_handle;
 DeeFile_RELEASE(self);
#ifdef DEE_PLATFORM_WINDOWS
 return DeeString_Newf("<pipe.reader: %#Ix>",handle);
#elif defined(DEE_PLATFORM_UNIX)
 return DeeString_Newf("<pipe.reader: %d>",handle);
#endif
}
#ifdef DEE_PLATFORM_WINDOWS
#ifndef SIZEOF_DWORD
#define SIZEOF_DWORD 4
#endif
#ifndef DWORD_MAX
#define DWORD_MAX 0xFFFFFFFF
#endif
#endif

static int DEE_CALL _deepipereader_tp_io_read(DeePipeObject *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFile_ACQUIRE(self);
 {
#if defined(DEE_PLATFORM_WINDOWS)
#if DEE_TYPES_SIZEOF_SIZE_T > SIZEOF_DWORD
  DWORD temp; *rs = 0;
  while (s) {
   if DEE_UNLIKELY(!ReadFile((HANDLE)self->p_handle,p,s >= DWORD_MAX
    ?(DWORD)DWORD_MAX:(DWORD)s,(LPDWORD)&temp,NULL))
#else
   if DEE_UNLIKELY(!ReadFile((HANDLE)self->p_handle,p,s,(LPDWORD)rs,NULL))
#endif
   {
    DWORD error;
    DeeFile_RELEASE(self);
    error = DeeSystemError_Win32Consume();
    if (error == ERROR_BROKEN_PIPE) { *rs = 0; return 0; } // End of pipe
    DeeError_SystemErrorExplicit("ReadFile",error);
    return -1;
   }
#if DEE_TYPES_SIZEOF_SIZE_T > SIZEOF_DWORD
   if (!temp) break; // can't read more
   s -= (Dee_size_t)temp;
   *(char const **)&p += (Dee_size_t)temp;
   *rs += (Dee_size_t)temp;
  }
#endif
#elif defined(DEE_PLATFORM_UNIX)
  Dee_ssize_t result;
  result = (Dee_ssize_t)read(self->p_handle,p,s);
  if (result == -1) {
   int error = errno;
   if (error) {
    errno = 0;
    DeeFile_RELEASE(self);
    DeeError_SystemErrorExplicit("read",error);
    return -1;
   }
  }
  *rs = *(Dee_size_t *)&result;
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
  if ((*rs = fread(p,sizeof(char),s,(FILE *)self->p_handle)) != s) {
   int error;
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
   if (ferror((FILE *)self->p_handle) != 0)
#else /* DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H */
   if ((error = ferror((FILE *)self->p_handle)) != 0)
#endif /* !DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H */
   {
    DeeFile_RELEASE(self);
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
    error = errno;
    errno = 0;
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H */
    DeeError_SystemErrorExplicit("fread",error);
   }
  }
#endif
  DeeFile_RELEASE(self);
  return 0;
 }
}
static int DEE_CALL _deepipereader_tp_io_write(
 DeePipeObject *DEE_UNUSED(self), void const *DEE_UNUSED(p),
 Dee_size_t DEE_UNUSED(s), Dee_size_t *DEE_UNUSED(ws)) {
 DeeError_NotImplemented_str("pipe.reader.write");
 return -1;
}
static int DEE_CALL _deepipereader_tp_io_seek(
 DeePipeObject *DEE_UNUSED(self), Dee_int64_t DEE_UNUSED(off),
 int DEE_UNUSED(whence), Dee_uint64_t *DEE_UNUSED(pos)) {
 DeeError_NotImplemented_str("pipe.reader.seek");
 return -1;
}
static int DEE_CALL _deepipereader_tp_io_trunc(DeePipeObject *DEE_UNUSED(self)) {
 DeeError_NotImplemented_str("pipe.reader.trunc");
 return -1;
}


//////////////////////////////////////////////////////////////////////////
// Pipe writer VTable
static DeeObject *DEE_CALL _deepipewriter_tp_str(DeePipeObject *self) {
 DeeIOHandle handle;
 DeeFile_ACQUIRE(self);
 handle = self->p_handle;
 DeeFile_RELEASE(self);
#ifdef DEE_PLATFORM_WINDOWS
 return DeeString_Newf("<pipe.writer: %#Ix>",handle);
#elif defined(DEE_PLATFORM_UNIX)
 return DeeString_Newf("<pipe.writer: %d>",handle);
#endif
}
static int DEE_CALL _deepipewriter_tp_io_read(
 DeePipeObject *DEE_UNUSED(self), void *DEE_UNUSED(p),
 Dee_size_t DEE_UNUSED(s), Dee_size_t *DEE_UNUSED(rs)) {
 DeeError_NotImplemented_str("pipe.writer.read");
 return -1;
}
static int DEE_CALL _deepipewriter_tp_io_write(
 DeePipeObject *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 if DEE_UNLIKELY(DeeThread_CheckInterrupt() != 0) return -1;
 DeeFile_ACQUIRE(self);
 {
#if defined(DEE_PLATFORM_WINDOWS)
#if DEE_TYPES_SIZEOF_SIZE_T > SIZEOF_DWORD
  DWORD temp; *ws = 0;
  while (s) {
   if DEE_UNLIKELY(!WriteFile((HANDLE)self->p_handle,p,s >= DWORD_MAX
    ?(DWORD)DWORD_MAX:(DWORD)s,(LPDWORD)&temp,NULL))
#else
   if DEE_UNLIKELY(!WriteFile(self->p_handle,p,s,(LPDWORD)ws,NULL))
#endif
   {
    DeeFile_RELEASE(self);
    DeeError_SystemError("WriteFile");
    return -1;
   }
#if DEE_TYPES_SIZEOF_SIZE_T > SIZEOF_DWORD
   if (!temp) break; // can't write more
   s -= (Dee_size_t)temp;
   *(char const **)&p += (Dee_size_t)temp;
   *ws += (Dee_size_t)temp;
  }
#endif
#elif defined(DEE_PLATFORM_UNIX)
  Dee_ssize_t result;
  result = (Dee_ssize_t)write(self->p_handle,p,s);
  if DEE_UNLIKELY(result == -1) {
   int error = errno;
   if (error) {
    DeeFile_RELEASE(self);
    errno = 0;
    DeeError_SystemErrorExplicit("write",error);
    return -1;
   }
  }
  *ws = *(Dee_size_t *)&result;
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_STDIO_H
  if ((*ws = fwrite(p,sizeof(char),s,(FILE *)self->p_handle)) != s) {
   int error;
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
   if (ferror((FILE *)self->p_handle) != 0)
#else /* DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H */
   if ((error = ferror((FILE *)self->p_handle)) != 0)
#endif /* !DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H */
   {
    DeeFile_RELEASE(self);
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
    error = errno;
    errno = 0;
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H */
    DeeError_SystemErrorExplicit("fwrite",error);
   }
  }
#endif
  DeeFile_RELEASE(self);
  return 0;
 }
}
static int DEE_CALL _deepipewriter_tp_io_seek(
 DeePipeObject *DEE_UNUSED(self), Dee_int64_t DEE_UNUSED(off),
 int DEE_UNUSED(whence), Dee_uint64_t *DEE_UNUSED(pos)) {
 DeeError_NotImplemented_str("pipe.writer.seek");
 return -1;
}
static int DEE_CALL _deepipewriter_tp_io_trunc(DeePipeObject *DEE_UNUSED(self)) {
 DeeError_NotImplemented_str("pipe.writer.trunc");
 return -1;
}


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
static DeeObject *DEE_CALL _deepipe_fileno(
 DeePipeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeIOHandle handle;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":fileno") != 0) return NULL;
 DeeFile_ACQUIRE(self);
 handle = DeePipe_HANDLE(self);
 DeeFile_RELEASE(self);
 return DeeObject_New(DeeIOHandle,handle);
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

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deepipe_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deepipe_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__p_handle",DeePipeObject,p_handle,DeeIOHandle),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */

static struct DeeMethodDef const _deepipe_tp_methods[] = {
#ifdef DEE_PLATFORM_WINDOWS
 DEE_METHODDEF_v100("fileno",member(&_deepipe_fileno),"() -> none *"),
#elif defined(DEE_PLATFORM_UNIX)
 DEE_METHODDEF_v100("fileno",member(&_deepipe_fileno),"() -> int"),
#endif /* ... */
 DEE_METHODDEF_END_v100
};

DeeFileTypeObject DeePipe_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("pipe"),null,null,
   member((DeeTypeObject *)&DeeFile_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeePipeObject),null,null,null,
   member(&_deepipe_tp_move_ctor),null),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
   member(&_deepipe_tp_dtor)),
  DEE_TYPE_OBJECT_ASSIGN_v100(null,
   member(&_deepipe_tp_move_assign),null),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&_deepipe_tp_str),
   member(&_deepipe_tp_str),null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
  DEE_TYPE_OBJECT_MATH_v101(
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
   member(_deepipe_tp_members),null,member(_deepipe_tp_methods),
   member(_deepipe_tp_class_members),null,
   member(_deepipe_tp_class_methods)),
  DEE_TYPE_OBJECT_FOOTER_v100
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(null,null,null,null,null,
  member(&_deepipe_tp_io_close))
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};
DeeFileTypeObject DeePipeReader_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("pipe.reader"),null,null,
   member((DeeTypeObject *)&DeePipe_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeePipeObject),null,null,null,
   member(&_deepipe_tp_move_ctor),null),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
  DEE_TYPE_OBJECT_ASSIGN_v100(null,
   member(&_deepipe_tp_move_assign),null),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&_deepipereader_tp_str),
   member(&_deepipereader_tp_str),null,null,null),
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
  member(&_deepipereader_tp_io_read),
  member(&_deepipereader_tp_io_write),
  member(&_deepipereader_tp_io_seek),
  member(&_deepipe_tp_io_flush),
  member(&_deepipereader_tp_io_trunc),null)
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};
DeeFileTypeObject DeePipeWriter_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("pipe.writer"),null,null,
   member((DeeTypeObject *)&DeePipe_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeePipeObject),null,null,null,
   member(&_deepipe_tp_move_ctor),null),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
  DEE_TYPE_OBJECT_ASSIGN_v100(null,
   member(&_deepipe_tp_move_assign),null),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&_deepipewriter_tp_str),
   member(&_deepipewriter_tp_str),null,null,null),
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
  member(&_deepipewriter_tp_io_read),
  member(&_deepipewriter_tp_io_write),
  member(&_deepipewriter_tp_io_seek),
  member(&_deepipe_tp_io_flush),
  member(&_deepipewriter_tp_io_trunc),null)
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};


DEE_DECL_END
#endif /* DEE_PLATFORM_HAVE_PIPES */

#endif /* !GUARD_DEEMON_PIPE_C */
