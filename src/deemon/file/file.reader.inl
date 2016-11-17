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
#ifndef GUARD_DEEMON_FILE_READER_INL
#define GUARD_DEEMON_FILE_READER_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/bool.h>
#include <deemon/file.h>
#include <deemon/string.h>
#include <deemon/none.h>
#include <deemon/memberdef.h>
#include <deemon/deemonrun.h>
#include <deemon/marshal.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_visit.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>
#include DEE_INCLUDE_MEMORY_API()

// */ (nano...)

DEE_DECL_BEGIN

#define _DeeFileReader_SET_STRING(ob,str_,on_err)\
do{\
 Dee_DECREF(((DeeFileReaderObject *)Dee_REQUIRES_POINTER(ob))->fr_string);\
 _DeeFileReader_SET_INITIAL_STRING(ob,str_,on_err);\
}while(0)
#define _DeeFileReader_SET_INITIAL_STRING(ob,str_,on_err)\
do{\
 if (DeeAnyString_Check(str_)) {\
  Dee_INCREF(((DeeFileReaderObject *)(ob))->fr_string = (DeeAnyStringObject *)Dee_REQUIRES_POINTER(str_));\
  ((DeeFileReaderObject *)(ob))->fr_end = (((DeeFileReaderObject *)(ob))->fr_pos = \
   (Dee_uint8_t *)DeeAnyString_DATAV(str_))+DeeAnyString_DATAC(str_);\
 } else if (DeeNone_Check(str_)) {\
  Dee_INCREF(((DeeFileReaderObject *)(ob))->fr_string = (DeeAnyStringObject *)Dee_EmptyString);\
  ((DeeFileReaderObject *)(ob))->fr_end = \
  ((DeeFileReaderObject *)(ob))->fr_pos = \
   (Dee_uint8_t *)DeeString_STR(Dee_EmptyString);\
 } else {\
  DeeError_SetStringf(&DeeErrorType_TypeError,"Unexpected type (expected: any string; got: %s)",\
                      DeeType_NAME(Dee_TYPE(str_)));\
  on_err;\
 }\
}while(0)


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

DEE_A_RET_OBJECT_REF(DeeStringObject) *DeeFileReader_String(
 DEE_A_IN_OBJECT(DeeFileReaderObject) const *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeFileReader_Check(self));
 DeeFile_ACQUIRE(self);
 Dee_INCREF(result = DeeFileReader_STRING(self));
 DeeFile_RELEASE(self);
 return result;
}





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

static struct DeeMemberDef const _deefilereader_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("data",DeeFileReaderObject,fr_string,object),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__fr_string",DeeFileReaderObject,fr_string,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__fr_pos",DeeFileReaderObject,fr_pos,p1(Dee_uint8_t)),
 DEE_MEMBERDEF_NAMED_RO_v100("__fr_end",DeeFileReaderObject,fr_end,p1(Dee_uint8_t)),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};




DeeFileTypeObject DeeFileReader_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("file.reader"),member(
   "(string data) -> file.reader\n"
   "\tA file type that can be used to read from a constant string buffer.\n"
   "\tThis is a read-only stream, meaning you can't write or truncate it."),
   null,member((DeeTypeObject *)&DeeFile_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFileReaderObject),null,
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

DEE_DECL_END

#endif /* !GUARD_DEEMON_FILE_READER_INL */
