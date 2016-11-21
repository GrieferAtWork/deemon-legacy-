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

// /include/*
#include <deemon/file.h>
#include <deemon/list.h>
#include <deemon/none.h>
#include <deemon/structured.h>
#include <deemon/super.h>
#include <deemon/tuple.h>

// */ (nano...)

DEE_DECL_BEGIN

int DEE_CALL _DeeGenericContext_tp_io_readat_from_tp_io_readseek(
 struct DeeFileObject *self, Dee_uint64_t pos, void *p, Dee_size_t s, Dee_size_t *rs) {
 if (DeeFile_Seek((DeeObject *)self,*(Dee_int64_t*)&pos,DEE_SEEK_SET,NULL) != 0) return -1;
 return DeeFile_Read((DeeObject *)self,p,s,rs);
}
int DEE_CALL _DeeGenericContext_tp_io_writeat_from_tp_io_writeseek(
 struct DeeFileObject *self, Dee_uint64_t pos, void const *p, Dee_size_t s, Dee_size_t *ws) {
 if (DeeFile_Seek((DeeObject *)self,*(Dee_int64_t*)&pos,DEE_SEEK_SET,NULL) != 0) return -1;
 return DeeFile_Write((DeeObject *)self,p,s,ws);
}





DEE_A_RET_WUNUSED Dee_openmode_t DeeFile_TryParseMode(
 DEE_A_IN_Z char const *mode) {
 Dee_openmode_t result; DEE_ASSERT(mode);
 switch (*mode++) {
  case 'r': result = DEE_OPENMODE_READ; break;
  case 'w': result = DEE_OPENMODE_WRITE; break;
  case 'x': result = DEE_OPENMODE_CREATE; break;
  case 'a': result = DEE_OPENMODE_APPEND; break;
  default: return 0; // Invalid mode
 }
 if (*mode == '+') {
  if (mode[1]) return 0;
  result |= DEE_OPENMODE_FLAG_UPDATE;
 }
 return result;
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
 if DEE_UNLIKELY(ch == '\n' && (DeeFile_FLAGS(self)&DEE_PRIVATE_FILEFLAG_SKIPREADLINELF)!=0) {
  DeeFile_FLAGS(self) &= ~DEE_PRIVATE_FILEFLAG_SKIPREADLINELF;
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
   DeeFile_FLAGS(self) |= DEE_PRIVATE_FILEFLAG_SKIPREADLINELF;
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


DEE_DECL_END

#ifndef __INTELLISENSE__
#include "file.fd.c.inl"
#include "file.io.c.inl"
#include "file.std.c.inl"
#include "file.reader.inl"
#include "file.joined.inl"
#include "file.writer.inl"
#include "file.iterator.inl"
#include "file.base.inl"
#include "file_type.inl"
#ifdef DEE_PLATFORM_WINDOWS
#include "file.win32_debug.c.inl"
#include "../sys/_win32.sysfd.filename.c.inl"
#endif /* DEE_PLATFORM_WINDOWS */
#if DEE_CONFIG_RUNTIME_HAVE_VFS
#include "file.vfs.c.inl"
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */
#endif

#endif /* !GUARD_DEEMON_FILE_C */
