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
#ifndef GUARD_DEEMON_DEX_ZLIB_C
#define GUARD_DEEMON_DEX_ZLIB_C 1
#define DEE_EXTENSION 1

//@ dex.name = _zlib

#include "_zlib.h"
#include <deemon/error.h>
#include <deemon/dex.h>
#include <deemon/bool.h>
#include <deemon/type.h>
#include <deemon/tuple.h>
#include <deemon/string.h>
#include <deemon/none.h>
#include <deemon/structured.h>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

DEE_A_RET_Z char const *DeeZLib_ErrorName(DEE_A_IN int error) {
 switch (error) {
  case MZ_OK:            return "MZ_OK";
  case MZ_STREAM_END:    return "MZ_STREAM_END";
  case MZ_NEED_DICT:;    return "MZ_NEED_DICT";
  case MZ_ERRNO:         return "MZ_ERRNO";
  case MZ_STREAM_ERROR:  return "MZ_STREAM_ERROR";
  case MZ_DATA_ERROR:    return "MZ_DATA_ERROR";
  case MZ_MEM_ERROR:     return "MZ_MEM_ERROR";
  case MZ_BUF_ERROR:     return "MZ_BUF_ERROR";
  case MZ_VERSION_ERROR: return "MZ_VERSION_ERROR";
  case MZ_PARAM_ERROR:   return "MZ_PARAM_ERROR";
  default: break;
 }
 return NULL;
}
void DeeZLib_ThrowError(DEE_A_IN int error) {
 DEE_ASSERT(error != MZ_OK);
 DeeError_SetStringf(&DeeZLibError_Type,
                     "%d : %s : %s",
                     error,
                     DeeZLib_ErrorName(error),
                     mz_error(error));
}


DEE_A_RET_EXCEPT(-1) int DeeZLib_CompressEx(
 DEE_A_INOUT DeeStringWriter *writer,
 DEE_A_IN_R(s) void const *p, DEE_A_IN size_t s,
 DEE_A_OUT_OPT size_t *compressed_size, DEE_A_IN int level) {
 mz_ulong max_size,used_size; char *dest;
 size_t old_size;  int error;
 DEE_ASSERT(writer),DEE_ASSERT(!s || p);
 used_size = max_size = mz_compressBound((mz_ulong)s);
 old_size = DeeStringWriter_SIZE(writer);
 dest = DeeStringWriter_Require(writer,(size_t)max_size);
 if (!dest) return -1;
 error = mz_compress2((unsigned char *)dest,&used_size,(unsigned char const *)p,s,level);
 if (error != 0) {
  if (DeeStringWriter_Resize(writer,old_size) != 0) DeeError_Handled();
  DeeZLib_ThrowError(error);
 }
 if (used_size < max_size && DeeStringWriter_Resize(
  writer,old_size+(size_t)used_size) != 0) return -1;
 if (compressed_size) *compressed_size = (size_t)used_size;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeZLib_Uncompress(
 DEE_A_INOUT DeeStringWriter *writer,
 DEE_A_IN_R(s) void const *p, DEE_A_IN size_t s,
 DEE_A_OUT_OPT size_t *uncompressed_size) {
 mz_ulong used_size; void *dest;
 size_t old_size,bufsize; int error;
 DEE_ASSERT(writer),DEE_ASSERT(!s || p);
 old_size = DeeStringWriter_SIZE(writer);
 bufsize = s+s/2; // Start with a 1.5 times buffer (double each time the call fails)
again:
 if (DeeStringWriter_Resize(writer,old_size+bufsize) != 0) return -1;
 dest = (void *)(DeeStringWriter_ELEM(writer)+old_size);
 used_size = (mz_ulong)bufsize;
 error = mz_uncompress((unsigned char *)dest,&used_size,(unsigned char const *)p,s);
 switch (error) {
  case MZ_OK: break; // this is what we want
  case MZ_BUF_ERROR:
   // TODO: cap this value with some kind of roof
   bufsize *= 2;
   goto again;
  default: // whoopsie
   if (DeeStringWriter_Resize(writer,old_size) != 0) DeeError_Handled();
   DeeZLib_ThrowError(error);
   return -1;
 }
 if (used_size < DeeStringWriter_SIZE(writer) &&
     DeeStringWriter_Resize(writer,old_size+used_size) != 0) return -1;
 if (uncompressed_size) *uncompressed_size = (size_t)used_size;
 return 0;
}





DeeTypeObject DeeZLibError_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("ZlibError"),member(
  "\tAn error type used for wrapping zlib errors."),null,member(&DeeErrorType_ValueError)),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeErrorObject),null,null,null,null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};





static DeeObject *_zlib_compress_data(DeeObject *args) {
 DeeObject *result,*data,*size_or_level = NULL,*level_ob = NULL;
 int level; void *p; size_t s;
 if (DeeTuple_Unpack(args,"o|oo:compress_data",&data,&size_or_level,&level_ob) != 0) return NULL;
 if (level_ob) {
  if (DeeObject_Cast(int,level_ob,&level) != 0) return NULL;
  if (DeeObject_Cast(size_t,size_or_level,&s) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  if (!DeeString_Check(data)) {
   if (DeeObject_GetVoidPointerEx(data,&p) != 0) return NULL;
  } else
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  if (DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  {
   p = DeeString_STR(data);
   if (s > DeeString_SIZE(data)) s = DeeString_SIZE(data);
  }
 } else {
  if (size_or_level) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   if (!DeeString_Check(data)) {
    if (DeeObject_GetVoidPointerEx(data,&p) != 0) return NULL;
    if (DeeObject_Cast(Dee_size_t,size_or_level,&s) != 0) return NULL;
    level = Z_DEFAULT_COMPRESSION;
   } else
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   if (DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   {
    p = DeeString_STR(data);
    s = DeeString_SIZE(data);
    if (DeeObject_Cast(int,size_or_level,&level) != 0) return NULL;
   }
  } else {
   if (DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) return NULL;
   p = DeeString_STR(data),s = DeeString_SIZE(data);
   level = Z_DEFAULT_COMPRESSION;
  }
 }
 {DeeStringWriter writer = DeeStringWriter_INIT();
  if (DeeZLib_CompressEx(&writer,p,s,NULL,level) == 0)
   result = DeeStringWriter_Pack(&writer);
  else result = NULL;
  DeeStringWriter_Quit(&writer);
 }
 return result;
}
static DeeObject *_zlib_uncompress_data(DeeObject *args) {
 DeeObject *result,*data,*size = NULL; void *p; size_t s;
 if (DeeTuple_Unpack(args,"o|o:uncompress_data",&data,&size) != 0) return NULL;
 if (size) {
  if (DeeObject_Cast(size_t,size,&s) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  if (DeeString_Check(data)) {
   if (DeeObject_GetVoidPointerEx(data,&p) != 0) return NULL;
  } else
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  if (DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  {
   p = DeeString_STR(data);
   if (DeeString_SIZE(data) > s) s = DeeString_SIZE(data);
  }
 } else {
  if (DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) return NULL;
  p = DeeString_STR(data),s = DeeString_SIZE(data);
 }
 {DeeStringWriter writer = DeeStringWriter_INIT();
  if (DeeZLib_Uncompress(&writer,p,s,NULL) == 0)
   result = DeeStringWriter_Pack(&writer);
  else result = NULL;
  DeeStringWriter_Quit(&writer);
 }
 return result;
}



//////////////////////////////////////////////////////////////////////////
// Module exports
struct DeeDexExportDef DeeDex_Exports[] = {
 DeeDex_EXPORT_OBJECT("ZLibError",&DeeZLibError_Type),
 // High-level compression/decompression functions
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeDex_EXPORT_FUNCTION("compress_data",&_zlib_compress_data,
  "(string data, int level = Z_DEFAULT_COMPRESSION) -> string\n"
  "(string data, size_t max_size, int level) -> string\n"
  "(none *data, size_t size, int level = Z_DEFAULT_COMPRESSION) -> string\n"
  "@throws ZLibError: Failed to compress @data\n"
  "\tCompresses the given @data and returns it as a #(string)."),
 DeeDex_EXPORT_FUNCTION("uncompress_data",&_zlib_uncompress_data,
  "(string data, size_t max_size = size_t(-1)) -> string\n"
  "(none *data, size_t size) -> string\n"
  "@throws ZLibError: Failed to uncompress @data\n"
  "\tUncompresses the given @data and returns it as a #(string)."),
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DeeDex_EXPORT_FUNCTION("compress_data",&_zlib_compress_data,
  "(string data, int level = Z_DEFAULT_COMPRESSION) -> string\n"
  "(string data, size_t max_size, int level) -> string\n"
  "@throws ZLibError: Failed to compress @data\n"
  "\tCompresses the given @data and returns it as a #(string)."),
 DeeDex_EXPORT_FUNCTION("uncompress_data",&_zlib_uncompress_data,
  "(string data, size_t max_size = size_t(-1)) -> string\n"
  "@throws ZLibError: Failed to uncompress @data\n"
  "\tUncompresses the given @data and returns it as a #(string)."),
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DeeDex_EXPORT_CONST_DOC("Z_NO_COMPRESSION",int,MZ_NO_COMPRESSION,"Compression strategies: No compression."),
 DeeDex_EXPORT_CONST_DOC("Z_BEST_SPEED",int,MZ_BEST_SPEED,"Compression strategies: Best speed."),
 DeeDex_EXPORT_CONST_DOC("Z_BEST_COMPRESSION",int,MZ_BEST_COMPRESSION,"Compression strategies: Best compression."),
 DeeDex_EXPORT_CONST_DOC("Z_DEFAULT_COMPRESSION",int,MZ_DEFAULT_COMPRESSION,"Compression strategies: Default compression."),
 DeeDex_EXPORT_CONST_DOC("Z_UBER_COMPRESSION",int,MZ_UBER_COMPRESSION,"Compression strategies: Uber compression."), // extension

 DeeDex_EXPORT_END
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_ZLIB_C */
