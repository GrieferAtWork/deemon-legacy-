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
#ifndef GUARD_DEEMON_FILE_WRITER_INL
#define GUARD_DEEMON_FILE_WRITER_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/bool.h>
#include <deemon/file.h>
#include <deemon/string.h>
#include <deemon/memberdef.h>
#include <deemon/deemonrun.h>
#include <deemon/marshal.h>
#include <deemon/optional/object_malloc.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>
#include DEE_INCLUDE_MEMORY_API()

// */ (nano...)

DEE_DECL_BEGIN


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

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeFileWriter_PackEx(
 DEE_A_INOUT_OBJECT(DeeFileWriterObject) *self, DEE_A_IN DeeTypeObject const *string_type) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileWriter_Check(self));
 DEE_ASSERTF(string_type == &DeeWideString_Type
          || string_type == &DeeUtf8String_Type
          || string_type == &DeeUtf16String_Type
          || string_type == &DeeUtf32String_Type,
             "Invalid string type");
 result = DeeFileWriter_PACK(self);
 if (DEE_UNLIKELY(string_type != &DeeUtf8String_Type) && result) {
  Dee_DECREF(Dee_TYPE(result));
  Dee_INCREF(Dee_TYPE(result) = (DeeTypeObject *)string_type);
 }
 return result;
}



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
  right->fo_flags = DEE_PRIVATE_FILEFLAG_NONE;
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


static struct DeeMethodDef const _deefilewriter_tp_methods[] = {
 DEE_METHODDEF_CONST_v100("pack",member(&_deefilewriter_pack),
  "(type object string_type = string) -> string_type\n"
  "@return: The generated string as an instance of @string_type\n"
  "\tThis function may clear all previously written data after the string is generated."),
 DEE_METHODDEF_END_v100
};


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

DEE_DECL_END

#endif /* !GUARD_DEEMON_FILE_WRITER_INL */
