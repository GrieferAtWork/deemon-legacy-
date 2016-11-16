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

#ifdef __INTELLISENSE__
#include "file.c"
DEE_DECL_BEGIN
#endif

#ifndef BUFFERED_BUFSIZE_DEF
#define BUFFERED_BUFSIZE_DEF 256
#define BUFFERED_BUFSIZE_BIG 4096
#endif

#define BUFFERED_BUFSIZE BUFFERED_BUFSIZE_DEF
#include "file.impl.buffered_read.inl"
#define BUFFERED_BUFSIZE BUFFERED_BUFSIZE_BIG
#include "file.impl.buffered_read.inl"

#ifdef AT
#ifdef ALL
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeFile_TReadAllDataAt
#else
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeFile_TReadDataAt
#endif
#else
#ifdef ALL
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeFile_TReadAllData
#else
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeFile_TReadData
#endif
#endif
(DEE_A_IN_TYPEOBJECT(DeeFileTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *self,
#ifdef AT
 DEE_A_IN Dee_uint64_t pos,
#endif
 DEE_A_IN Dee_size_t max_bytes) {
 DeeObject *result;
#ifndef AT
 Dee_uint64_t file_pos;
#endif
 Dee_uint64_t file_size;
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeFileType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
#ifdef AT
 if (DeeFile_TSeek(tp_self,self,0,DEE_SEEK_END,&file_size) != 0)
#else
 if (DeeFile_TTell(tp_self,self,&file_pos) != 0 ||
     DeeFile_TSeek(tp_self,self,0,DEE_SEEK_END,&file_size) != 0 ||
     DeeFile_TSeek(tp_self,self,*(Dee_int64_t*)&file_pos,DEE_SEEK_SET,NULL) != 0)
#endif
 {
  if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) return NULL;
  // Data read without prior knowledge of file size (buffered)
#ifdef AT
#ifdef ALL
  return DEE_PP_CAT_2(_deefile_buffered_read_all_data_at_,BUFFERED_BUFSIZE_DEF)((DeeFileObject *)self,pos,max_bytes);
#else
  return DEE_PP_CAT_2(_deefile_buffered_read_data_at_,BUFFERED_BUFSIZE_DEF)((DeeFileObject *)self,pos,max_bytes);
#endif
#else
#ifdef ALL
  return DEE_PP_CAT_2(_deefile_buffered_read_all_data_,BUFFERED_BUFSIZE_DEF)((DeeFileObject *)self,max_bytes);
#else
  return DEE_PP_CAT_2(_deefile_buffered_read_data_,BUFFERED_BUFSIZE_DEF)((DeeFileObject *)self,max_bytes);
#endif
#endif
 }
 // Some special device files don't implement seek properly (aka don't set an errno)
 // - Instead they act as if they were empty file.
 //   So if we read that a file is empty, we try to read it as buffered.
 // - If it really is empty, this will still simply return an empty string.
 //   But if it isn't, this will manage to read the data
 // NOTE: We also use an extremely big buffer to be able to read pretty much all data in one go.
 if DEE_UNLIKELY(file_size == 0) {
#ifdef AT
#ifdef ALL
  return DEE_PP_CAT_2(_deefile_buffered_read_all_data_at_,BUFFERED_BUFSIZE_BIG)((DeeFileObject *)self,pos,max_bytes);
#else
  return DEE_PP_CAT_2(_deefile_buffered_read_data_at_,BUFFERED_BUFSIZE_BIG)((DeeFileObject *)self,pos,max_bytes);
#endif
#else
#ifdef ALL
  return DEE_PP_CAT_2(_deefile_buffered_read_all_data_,BUFFERED_BUFSIZE_BIG)((DeeFileObject *)self,max_bytes);
#else
  return DEE_PP_CAT_2(_deefile_buffered_read_data_,BUFFERED_BUFSIZE_BIG)((DeeFileObject *)self,max_bytes);
#endif
#endif
 }
#ifdef AT
 if DEE_LIKELY(max_bytes > file_size-pos) max_bytes = (Dee_size_t)(file_size-pos);
#else
 if DEE_LIKELY(max_bytes > file_size-file_pos) max_bytes = (Dee_size_t)(file_size-file_pos);
#endif
 if DEE_LIKELY((result = DeeString_NewSized(max_bytes/sizeof(Dee_Char))) != NULL) {
#ifdef ALL
#ifdef AT
  if (DeeFile_TReadAllAt(tp_self,self,pos,DeeString_STR(result),max_bytes) != 0)
#else
  if (DeeFile_TReadAll(tp_self,self,DeeString_STR(result),max_bytes) != 0)
#endif
  {
   Dee_CLEAR(result);
  }
#else
  Dee_size_t str_length;
#ifdef AT
  if (DeeFile_TReadAt(tp_self,self,pos,DeeString_STR(result),max_bytes,&str_length) != 0)
#else
  if (DeeFile_TRead(tp_self,self,DeeString_STR(result),max_bytes,&str_length) != 0)
#endif
  {
err_r: Dee_CLEAR(result);
  } else if (DEE_UNLIKELY(str_length != max_bytes)) {
   if (DeeString_Resize(&result,str_length/sizeof(Dee_Char)) != 0) goto err_r;
  }
#endif
 }
 return result;
}

#ifdef AT
#undef AT
#endif
#ifdef ALL
#undef ALL
#endif

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
