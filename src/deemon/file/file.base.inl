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
#ifndef GUARD_DEEMON_FILE_BASE_INL
#define GUARD_DEEMON_FILE_BASE_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/file.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/structured.h>
#include <deemon/tuple.h>
#include <deemon/optional/object_malloc.h>

// /src/*
#include <deemon/marshal_data.h>
#include <deemon/file/file.io.h>
#include <deemon/sys/sysfd.h>

// */ (nano...)

DEE_DECL_BEGIN

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
 DEE_MEMBERDEF_CONST_v100("fd",object,&DeeFileFD_Type),
 DEE_MEMBERDEF_CONST_v100("io",object,&DeeFileIO_Type),
 DEE_MEMBERDEF_CONST_v100("reader",object,&DeeFileReader_Type),
 DEE_MEMBERDEF_CONST_v100("writer",object,&DeeFileWriter_Type),
 DEE_MEMBERDEF_CONST_v100("joined",object,&DeeFileJoined_Type),
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeFileIterator_Type),
 DEE_MEMBERDEF_CONST_v100("MARSHAL_VERSION",Dee_uint16_t,DEE_MARSHAL_VERSION),
#ifdef DEE_PLATFORM_WINDOWS
 DEE_MEMBERDEF_CONST_v100("win32_debug",object,&DeeFileWin32Dbg_Type),
#endif /* DEE_PLATFORM_WINDOWS */
 DEE_MEMBERDEF_END_v100
};

DeeString_NEW_STATIC_EX(_deefile_default_open_mode,1,{'r'});
static DeeObject *DEE_CALL _deefileclass_open(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeAnyStringObject *filename; DeeObject *result; Dee_openmode_t openmode;
 Dee_mode_t perms = DEE_FILEIO_DEFAULT_PERMISSIONS;
 DeeStringObject *mode = (DeeStringObject *)&_deefile_default_open_mode;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|oI" DEE_PP_STR(DEE_PP_MUL8(
  DEE_TYPES_SIZEOF_MODE_T)) "u:file.fd.fp",&filename,&mode,&perms) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)mode,&DeeString_Type) != 0) return NULL;
 openmode = DeeFile_TryParseMode(DeeString_STR(mode));
 if (DeeFileIO_Check(filename)) {
  if DEE_UNLIKELY((result = (DeeObject *)DeeObject_Malloc(sizeof(DeeFileIOObject))) == NULL) return NULL;
#ifdef DEE_PLATFORM_WINDOWS
  if DEE_UNLIKELY((filename = (DeeAnyStringObject *)(
   DeeFileIO_WideFilename((DeeObject *)filename))) == NULL) {
err_freer: DeeObject_Free(result); return NULL;
  }
  DeeSysFileFD_WideInitObject(&((DeeFileIOObject *)result)->io_descr,(DeeObject *)filename,openmode,
                              perms,{ Dee_DECREF(filename); goto err_freer; });
#else
  if DEE_UNLIKELY((filename = (DeeAnyStringObject *)(
   DeeFileIO_Utf8Filename((DeeObject *)filename))) == NULL) {
err_freer: DeeObject_Free(result); return NULL;
  }
  DeeSysFileFD_Utf8InitObject(&((DeeFileIOObject *)result)->io_descr,(DeeObject *)filename,openmode,
                              perms,{ Dee_DECREF(filename); goto err_freer; });
#endif
  Dee_DECREF(filename);
  return result;
 }
 return DeeFile_OpenObjectEx((DeeObject *)filename,openmode,perms);
}

static struct DeeMethodDef const _deefile_tp_class_methods[] = {
 DEE_METHODDEF_v100("open",member(&_deefileclass_open),
  "(string filename, string mode = \"r\", mode_t permissions = " DEE_PP_STR(DEE_FILEIO_DEFAULT_PERMISSIONS) ") -> file\n"
  "(string.wide filename, string mode = \"r\", mode_t permissions = " DEE_PP_STR(DEE_FILEIO_DEFAULT_PERMISSIONS) ") -> file\n"
  "(file.io filename, string mode = \"r\", mode_t permissions = " DEE_PP_STR(DEE_FILEIO_DEFAULT_PERMISSIONS) ") -> file\n"
  "@return: A file object useable for reading/writing from either a virtual, device or regular file on disk\n"
  "\tNOTE: Starting in v102, only this function should be used for opening files."),
 DEE_METHODDEF_END_v100
};

extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIteratorObject) *
DeeFile_IterSelf(DEE_A_INOUT_OBJECT(DeeFileObject) *self);

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
   member(_deefile_tp_members),null,
   member(_deefile_tp_methods),
   member(_deefile_tp_class_members),null,
   member(_deefile_tp_class_methods)),
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

DEE_DECL_END

#endif /* !GUARD_DEEMON_FILE_BASE_INL */
