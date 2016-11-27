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

#include "_crypt.h"
#include <deemon/__conf.inl>
#include <deemon/dex.h>
#include <deemon/none.h>
#include <deemon/string.h>
#include <deemon/error.h>
#include <deemon/type.h>
#include <deemon/optional/std/string.h>

DEE_DECL_BEGIN

// Based of the code here: https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64
static char const _deebase64_encodetable[] =
 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

DEE_A_RET_EXCEPT(-1) int DeeCrypt_Base64EncodeEx(
 DEE_A_INOUT struct DeeStringWriter *writer,
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s,
 DEE_A_IN_RB_OPT(64) char const *salt) {
 Dee_uint8_t const *data,*end;
 int padCount,havemore; Dee_uint32_t n;
 end = (data = (const Dee_uint8_t *)p)+s;
 padCount = (int)(s % 3),n = 0;
 if (!salt) salt = _deebase64_encodetable;
 while (data != end) {
  n = ((Dee_uint32_t)*data++) << 16;
  if (data != end) {
   n += ((Dee_uint32_t)*data++) << 8;
   if (data != end) {
    n += *data++;
    havemore = 2;
   } else havemore = 1;
  } else havemore = 0;
  if (DeeStringWriter_WriteChar(writer,salt[(Dee_uint8_t)(n >> 18) & 63]) != 0 ||
      DeeStringWriter_WriteChar(writer,salt[(Dee_uint8_t)(n >> 12) & 63]) != 0) return -1;
  if (havemore >= 1) {
   if (DeeStringWriter_WriteChar(writer,salt[(Dee_uint8_t)(n >> 6) & 63]) != 0) return -1;
   if (havemore >= 2 && DeeStringWriter_WriteChar(writer,salt[(Dee_uint8_t)n & 63]) != 0) return -1;
  }
 }
 switch (padCount) {
  case 1: if (DeeStringWriter_WriteChar(writer,'=') != 0) return -1;
  case 2: if (DeeStringWriter_WriteChar(writer,'=') != 0) return -1;
  default: break;
 }
 return 0;
}


#define DEE_BASE64_WHITESPACE 64
#define DEE_BASE64_EQUALS     65
#define DEE_BASE64_INVALID    66
static Dee_uint8_t const _deebase64_decodetable[] = {
 66,66,66,66,66,66,66,66,66,66,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
 66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
 54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
 29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
 66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
 66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
 66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
 66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
 66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
 66,66,66,66,66,66
};

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeCrypt_Base64DecodeImpl(
 DEE_A_INOUT struct DeeStringWriter *writer,
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s,
 DEE_A_IN_RB(256) Dee_uint8_t const *decode_salt) { 
 Dee_uint8_t const *iter,*end; char *wbuf;
 Dee_uint8_t ch,quadpos = 0; Dee_size_t quad = 0;
 end = (iter = (Dee_uint8_t const *)p)+s;
 while (iter < end) {
  ch = decode_salt[*iter++];
  switch (ch) {
   case DEE_BASE64_WHITESPACE: continue;/* skip whitespace */
   case DEE_BASE64_INVALID:
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        "Invalid character in base64: %.1q",
                        iter-1);
    return -1;
   case DEE_BASE64_EQUALS: goto done;
   default:
    quad = quad << 6 | ch;
    quadpos++; // increment the number of iteration
    /* If the buffer is full, split it into bytes */
    if (quadpos == 4) {
     if ((wbuf = DeeStringWriter_Require(writer,3)) == NULL) return -1;
     wbuf[0] = (char)((quad >> 16) & 255);
     wbuf[1] = (char)((quad >> 8) & 255);
     wbuf[2] = (char)(quad & 255);
     quad = 0,quadpos = 0;
    }
    break;
  }
 }
done:
 switch (quadpos) {
  case 3:
   if ((wbuf = DeeStringWriter_Require(writer,2)) == NULL) return -1;
   wbuf[0] = (char)((quad >> 10) & 255);
   wbuf[1] = (char)((quad >> 2) & 255);
   break;
  case 2:
   return DeeStringWriter_WriteChar(writer,(char)((quad >> 4) & 255));
  default: break;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeCrypt_Base64DecodeUserSalt(
 DEE_A_INOUT struct DeeStringWriter *writer, DEE_A_IN_RB(s) void const *p,
 DEE_A_IN Dee_size_t s, DEE_A_IN_RB(64) char const *salt) {
 Dee_uint8_t decode_salt[256]; Dee_size_t i;
 memcpy(decode_salt,_deebase64_decodetable,256);
 for (i = 0; i < 64; ++i) decode_salt[salt[i]] = (Dee_uint8_t)i;
 return _DeeCrypt_Base64DecodeImpl(writer,p,s,decode_salt);
}
DEE_A_RET_EXCEPT(-1) int DeeCrypt_Base64DecodeEx(
 DEE_A_INOUT struct DeeStringWriter *writer, DEE_A_IN_RB(s) void const *p,
 DEE_A_IN Dee_size_t s, DEE_A_IN_RB_OPT(64) char const *salt) {
 return salt
  ?  DeeCrypt_Base64DecodeUserSalt(writer,p,s,salt)
  : _DeeCrypt_Base64DecodeImpl(writer,p,s,_deebase64_decodetable);
} 



static DeeObject *_deecrypt_base64encode(DeeObject *args) {
 void *p; Dee_size_t s; DeeObject *result,*data,*size_or_salt = NULL,*salt = NULL;
 if (DeeTuple_Unpack(args,"o|oo:base64encode",&data,&size_or_salt,&salt) != 0) return NULL;
 {DeeStringWriter writer = DeeStringWriter_INIT();
  if (!size_or_salt) {
   // base64encode(string data)
   if (DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) goto err;
   if (DeeCrypt_Base64Encode(&writer,DeeString_STR(data),DeeString_SIZE(data)) != 0) goto err;
  } else if (salt) {
   // base64encode(string data, Dee_size_t s, string salt)
   // base64encode(none *p, Dee_size_t s, string salt)
   if (DeeError_TypeError_CheckTypeExact(salt,&DeeString_Type) != 0) goto err;
   if (DeeString_SIZE(salt) < 64) {
small_salt:
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        "Expected size of salt to be at least 64 (got: %Iu)",
                        DeeString_SIZE(salt));
   }
   if (DeeObject_Cast(Dee_size_t,size_or_salt,&s) != 0) goto err;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   if (!DeeString_Check(data)) {
    if (DeeObject_GetVoidPointerEx(data,&p) != 0) goto err;
   } else
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   if (DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) goto err;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   {
    p = DeeString_STR(data);
    if (DeeString_SIZE(data)*sizeof(char) > s) s = DeeString_SIZE(data)*sizeof(char);
   }
   if (DeeCrypt_Base64EncodeEx(&writer,p,s,DeeString_STR(salt)) != 0) goto err;
  } else if (DeeString_Check(size_or_salt)) {
   // base64encode(string data, string salt)
   if (DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) goto err;
   if (DeeString_SIZE(size_or_salt) < 64) { salt = size_or_salt; goto small_salt; }
   if (DeeCrypt_Base64EncodeEx(&writer,DeeString_STR(data),DeeString_SIZE(data),DeeString_STR(size_or_salt)) != 0) goto err;
  } else {
   // base64encode(none *p, Dee_size_t s)
   // base64encode(string data, Dee_size_t s)
   if (DeeObject_Cast(Dee_size_t,size_or_salt,&s) != 0) goto err;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   if (!DeeString_Check(data)) {
    if (DeeObject_GetVoidPointerEx(data,&p) != 0) goto err;
   } else
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   if (DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) goto err;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   {
    p = DeeString_STR(data);
    if (DeeString_SIZE(data)*sizeof(char) > s) s = DeeString_SIZE(data)*sizeof(char);
   }
   if (DeeCrypt_Base64Encode(&writer,p,s) != 0) goto err;
  }
  result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer); 
  return result;
err: result = NULL; goto end;
 }
}
static DeeObject *_deecrypt_base64decode(DeeObject *args) {
 void *p; Dee_size_t s; DeeObject *result,*data,*size_or_salt = NULL,*salt = NULL;
 if (DeeTuple_Unpack(args,"o|oo:base64decode",&data,&size_or_salt,&salt) != 0) return NULL;
 {DeeStringWriter writer = DeeStringWriter_INIT();
  if (!size_or_salt) {
   // base64decode(string data)
   if (DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) goto err;
   if (DeeCrypt_Base64Decode(&writer,DeeString_STR(data),DeeString_SIZE(data)) != 0) goto err;
  } else if (salt) {
   // base64decode(string data, Dee_size_t s, string salt)
   // base64decode(none *p, Dee_size_t s, string salt)
   if (DeeError_TypeError_CheckTypeExact(salt,&DeeString_Type) != 0) goto err;
   if (DeeString_SIZE(salt) < 64) {
small_salt:
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        "Expected size of salt to be at least 64 (got: %Iu)",
                        DeeString_SIZE(salt));
   }
   if (DeeObject_Cast(Dee_size_t,size_or_salt,&s) != 0) goto err;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   if (!DeeString_Check(data)) {
    if (DeeObject_GetVoidPointerEx(data,&p) != 0) goto err;
   } else
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   if (DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) goto err;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   {
    p = DeeString_STR(data);
    if (DeeString_SIZE(data)*sizeof(char) > s) s = DeeString_SIZE(data)*sizeof(char);
   }
   if (DeeCrypt_Base64DecodeEx(&writer,p,s,DeeString_STR(salt)) != 0) goto err;
  } else if (DeeString_Check(size_or_salt)) {
   // base64decode(string data, string salt)
   if (DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) goto err;
   if (DeeString_SIZE(size_or_salt) < 64) { salt = size_or_salt; goto small_salt; }
   if (DeeCrypt_Base64DecodeEx(&writer,DeeString_STR(data),DeeString_SIZE(data),DeeString_STR(size_or_salt)) != 0) goto err;
  } else {
   // base64decode(none *p, Dee_size_t s)
   // base64decode(string data, Dee_size_t s)
   if (DeeObject_Cast(Dee_size_t,size_or_salt,&s) != 0) goto err;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   if (!DeeString_Check(data)) {
    if (DeeObject_GetVoidPointerEx(data,&p) != 0) goto err;
   } else
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   if (DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) goto err;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   {
    p = DeeString_STR(data);
    if (DeeString_SIZE(data)*sizeof(char) > s) s = DeeString_SIZE(data)*sizeof(char);
   }
   if (DeeCrypt_Base64Decode(&writer,p,s) != 0) goto err;
  }
  result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer); 
  return result;
err: result = NULL; goto end;
 }
}



//////////////////////////////////////////////////////////////////////////
// Module exports
struct DeeDexExportDef DeeDex_Exports[] = {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeDex_EXPORT_FUNCTION("base64_encode",&_deecrypt_base64encode,
  "base64_encode(string data) -> string\n"
  "base64_encode(string data, size_t max_s) -> string\n"
  "base64_encode(none *p, size_t s) -> string\n"
  "base64_encode(string data, string salt) -> string\n"
  "base64_encode(string data, size_t max_s, string salt) -> string\n"
  "base64_encode(none *p, size_t s, string salt) -> string\n"
  "@throws Error.ValueError: Invalid salt\n"
  "\tEncodes given data to base64, either using the common\n"
  "\tdefault salt, or a given salt string with a size of >= 64\n"),
 DeeDex_EXPORT_FUNCTION("base64_decode",&_deecrypt_base64decode,
  "base64_decode(string data) -> string\n"
  "base64_decode(string data, size_t max_s) -> string\n"
  "base64_decode(none *p, size_t s) -> string\n"
  "base64_decode(string data, string salt) -> string\n"
  "base64_decode(string data, size_t max_s, string salt) -> string\n"
  "base64_decode(none *p, size_t s, string salt) -> string\n"
  "@throws Error.ValueError: Invalid data / Invalid salt\n"
  "\tDecodes given data from base64, either using the common\n"
  "\tdefault salt, or a given salt string with a size of >= 64\n"),
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DeeDex_EXPORT_FUNCTION("base64_encode",&_deecrypt_base64encode,
  "base64_encode(string data) -> string\n"
  "base64_encode(string data, size_t max_s) -> string\n"
  "base64_encode(string data, string salt) -> string\n"
  "base64_encode(string data, size_t max_s, string salt) -> string\n"
  "@throws Error.ValueError: Invalid salt\n"
  "\tEncodes given data to base64, either using the common\n"
  "\tdefault salt, or a given salt string with a size of >= 64\n"),
 DeeDex_EXPORT_FUNCTION("base64_decode",&_deecrypt_base64decode,
  "base64_decode(string data) -> string\n"
  "base64_decode(string data, size_t max_s) -> string\n"
  "base64_decode(string data, string salt) -> string\n"
  "base64_decode(string data, size_t max_s, string salt) -> string\n"
  "@throws Error.ValueError: Invalid data / Invalid salt\n"
  "\tDecodes given data from base64, either using the common\n"
  "\tdefault salt, or a given salt string with a size of >= 64\n"),
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DeeDex_EXPORT_END
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_ZLIB_C */
