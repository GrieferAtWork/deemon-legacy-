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
#ifndef GUARD_DEEMON_DEX_MARSHAL_C
#define GUARD_DEEMON_DEX_MARSHAL_C 1
#define DEE_EXTENSION 1

#include "_marshal.h"
#include <deemon/bool.h>
#include <deemon/dex.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/integer.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/optional/memberdef_decl.h>
#include <deemon/optional/object_visit.h>
#include <deemon/tuple.h>
#include <deemon/type.h>
#include <deemon/value.h>
#include <deemon/__string_encoding.inl>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN


//////////////////////////////////////////////////////////////////////////
// Confirm a given version to be supported
// -> Taken from 'src/deemon/marshal.c':_DeeMarshalReadMap_CheckVersion
//   and modified for forward-compatibility.
DEE_A_RET_EXCEPT(-1) int DeeMarshal_ConfirmVersion(DEE_A_IN DeeMarshalVersion version) {
 if (version > DEE_MARSHAL_VERSION) {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Future marshal version: %d (current: "
                      DEE_PP_STR(DEE_MARSHAL_VERSION)
                      ")",version);
  return -1;
 }
 if (version <= DEE_MARSHAL_V100) {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "No backwards compatibility for marshal version %d (please recompile)",
                      (int)version);
  return -1;
 }
 if (!DeeMarshal_IsSupported(version)) {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Unsupported marshal version %d",
                      (int)version);
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeMarshal_ConfirmWriteFlags(DEE_A_IN Dee_uint32_t flags) {
 if ((flags&~(DEE_MARSHAL_WRITEFLAG_MASK))!=0) {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Unknown flags provided to constructor of 'writemap': %#.8I32x",
                      flags);
  return -1;
 }
 return 0;
}




//////////////////////////////////////////////////////////////////////////
// DeeMarshalReadMapObject
static int DEE_CALL _deemarshalreadmap_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeMarshalReadMapObject *self) {
 DEE_ASSERT(DeeMarshal_IsSupported(DEE_MARSHAL_VERSION));
 if (DeeNativeMutex_Init(&self->mrm_lock) != 0) return -1;
 _DeeMarshalReadMap_Init(&self->mrm_map,DEE_MARSHAL_VERSION);
 return 0;
}
static int DEE_CALL _deemarshalreadmap_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeMarshalReadMapObject *self, DeeObject *args) {
 DeeMarshalVersion version;
 if (DeeTuple_Unpack(args,"I16u:readmap",&version) != 0) return -1;
 // Make sure the requested version is supported
 if (DeeMarshal_ConfirmVersion(self->mrm_map.mrm_version) != 0) return -1;
 if (DeeNativeMutex_Init(&self->mrm_lock) != 0) return -1;
 _DeeMarshalReadMap_Init(&self->mrm_map,version);
 return 0;
}
static void DEE_CALL _deemarshalreadmap_tp_dtor(DeeMarshalReadMapObject *self) {
 _DeeMarshalReadMap_Quit(&self->mrm_map);
 DeeNativeMutex_Quit(&self->mrm_lock);
}
DEE_VISIT_PROC(_deemarshalreadmap_tp_visit,DeeMarshalReadMapObject *self) {
 DeeNativeMutex_AcquireNoexcept(&self->mrm_lock);
 _DeeMarshalReadMap_Visit(&self->mrm_map);
 DeeNativeMutex_ReleaseNoexcept(&self->mrm_lock);
}
static DeeObject *DEE_CALL _deemarshalreadmap_uuid_get(
 DeeMarshalReadMapObject *self, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if (DeeNativeMutex_Acquire(&self->mrm_lock) != 0) return NULL;
 if (!self->mrm_map.mrm_uuid) {
  if (DeeNativeMutex_Release(&self->mrm_lock) != 0) return NULL;
  DeeError_SET_STRING(&DeeErrorType_ValueError,
                      "readmap does not have an associated uuid");
  result = NULL;
 } else {
  struct DeeUUID used_uuid;
  DeeUUID_SET(&used_uuid,DeeMarshalReadMap_UUID(&self->mrm_map));
  if (DeeNativeMutex_Release(&self->mrm_lock) != 0) return NULL;
  result = DeeUUID_ToString(&used_uuid);
 }
 return result;
}
static DeeObject *DEE_CALL _deemarshalreadmap_version_get(
 DeeMarshalReadMapObject *self, void *DEE_UNUSED(closure)) {
 DeeMarshalVersion result;
 if (DeeNativeMutex_Acquire(&self->mrm_lock) != 0) return NULL;
 result = self->mrm_map.mrm_version;
 if (DeeNativeMutex_Release(&self->mrm_lock) != 0) return NULL;
 return DeeObject_New(DeeMarshalVersion,result);
}
static DeeObject *DEE_CALL _deemarshalreadmapclass_is_supported(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeMarshalVersion version;
 if (DeeTuple_Unpack(args,"I16u:is_supported",&version) != 0) return NULL;
 DeeReturn_Bool(DeeMarshal_IsSupported(version));
}

static DeeObject *DEE_CALL _deemarshalreadmap_read(
 DeeMarshalReadMapObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *fp,*result;
 if (DeeTuple_Unpack(args,"o:read",&fp) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&fp,(DeeTypeObject *)&DeeFile_Type) != 0) return NULL;
 if (DeeNativeMutex_Acquire(&self->mrm_lock) != 0) return NULL;
 result = DeeMarshal_ReadObjectWithMap(fp,&self->mrm_map);
 DeeNativeMutex_ReleaseNoexcept(&self->mrm_lock);
 return result;
}

static struct DeeMemberDef const _deemarshalreadmap_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_DOC_v100("V100",DeeMarshalVersion,DEE_MARSHAL_V100,"\tv100 (UNSUPPORTED; original version)."),
 DEE_MEMBERDEF_CONST_DOC_v100("V101",DeeMarshalVersion,DEE_MARSHAL_V101,"\tv101 (new in deemon 101:1)."),
 DEE_MEMBERDEF_CONST_DOC_v100("V102",DeeMarshalVersion,DEE_MARSHAL_V102,"\tv102 (new in deemon 102:0)."),
 DEE_MEMBERDEF_CONST_DOC_v100("VERSION",DeeMarshalVersion,DEE_MARSHAL_VERSION,"\tThe current marshal version (equal to #file.MARSHAL_VERSION)."),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMethodDef const _deemarshalreadmap_tp_class_methods[] = {
 DEE_METHODDEF_CONST_v100("is_supported",member(&_deemarshalreadmapclass_is_supported),"(uint16_t version) -> bool\n"
  "@param version: The version to check for support\n"
  "@return: #true if the given @version is supported"),
 DEE_METHODDEF_END_v100
};
static struct DeeMethodDef const _deemarshalreadmap_tp_methods[] = {
 DEE_METHODDEF_v100("read",member(&_deemarshalreadmap_read),"(file fp) -> object\n"
  "@param fp: The file to read from\n"
  "@throws Signal.Interrupt: The calling thread was interrupted\n"
  "@throws Error.SystemError: Failed to acquire a lock\n"
  "@throws object: Same as the constructing any object through the marshal\n"
  "@return: The read object"),
 DEE_METHODDEF_END_v100
};
static struct DeeGetSetDef const _deemarshalreadmap_tp_getsets[] = {
 DEE_GETSETDEF_v100("uuid",member(&_deemarshalreadmap_uuid_get),null,null,"-> string\n"
  "@throws Signal.Interrupt: The calling thread was interrupted\n"
  "@throws Error.ValueError: No valid uuid currently associated with the readmap\n"
  "@throws Error.SystemError: Failed to acquire a lock\n"
  "@return: The UUID of the type currently being read"),
 DEE_GETSETDEF_v100("version",member(&_deemarshalreadmap_version_get),null,null,"-> uint16_t\n"
  "@throws Signal.Interrupt: The calling thread was interrupted\n"
  "@throws Error.SystemError: Failed to acquire a lock\n"
  "@return: The marshal version associated with the readmap"),
 DEE_GETSETDEF_END_v100
};




//////////////////////////////////////////////////////////////////////////
// DeeMarshalWriteMapObject
static int DEE_CALL _deemarshalwritemap_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeMarshalWriteMapObject *self) {
 DEE_ASSERT(DeeMarshal_IsSupported(DEE_MARSHAL_VERSION));
 if (DeeNativeMutex_Init(&self->mwm_lock) != 0) return -1;
 _DeeMarshalWriteMap_Init(&self->mwm_map,DEE_MARSHAL_WRITEFLAG_DEFAULT);
 return 0;
}
static int DEE_CALL _deemarshalwritemap_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeMarshalWriteMapObject *self, DeeObject *args) {
 Dee_uint32_t flags;
 if (DeeTuple_Unpack(args,"I32u:writemap",&flags) != 0) return -1;
 // Make sure only valid flags were used
 if (DeeMarshal_ConfirmWriteFlags(flags) != 0) return -1;
 if (DeeNativeMutex_Init(&self->mwm_lock) != 0) return -1;
 _DeeMarshalWriteMap_Init(&self->mwm_map,flags);
 return 0;
}
static void DEE_CALL _deemarshalwritemap_tp_dtor(DeeMarshalWriteMapObject *self) {
 _DeeMarshalWriteMap_Quit(&self->mwm_map);
 DeeNativeMutex_Quit(&self->mwm_lock);
}
DEE_VISIT_PROC(_deemarshalwritemap_tp_visit,DeeMarshalWriteMapObject *self) {
 DeeNativeMutex_AcquireNoexcept(&self->mwm_lock);
 _DeeMarshalWriteMap_Visit(&self->mwm_map);
 DeeNativeMutex_ReleaseNoexcept(&self->mwm_lock);
}
static DeeObject *DEE_CALL _deemarshalwritemap_flags_get(
 DeeMarshalWriteMapObject *self, void *DEE_UNUSED(closure)) {
 Dee_uint32_t result;
 if DEE_UNLIKELY(DeeNativeMutex_Acquire(&self->mwm_lock) != 0) return NULL;
 result = DeeMarshalWriteMap_GET_FLAGS(&self->mwm_map);
 if DEE_UNLIKELY(DeeNativeMutex_Release(&self->mwm_lock) != 0) return NULL;
 return DeeObject_New(Dee_uint32_t,result);
}
static int DEE_CALL _deemarshalwritemap_flags_del(
 DeeMarshalWriteMapObject *self, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeNativeMutex_Acquire(&self->mwm_lock) != 0) return -1;
 DeeMarshalWriteMap_SET_FLAGS(&self->mwm_map,DEE_MARSHAL_WRITEFLAG_DEFAULT);
 DeeNativeMutex_ReleaseNoexcept(&self->mwm_lock);
 return 0;
}
static int DEE_CALL _deemarshalwritemap_flags_set(
 DeeMarshalWriteMapObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 Dee_uint32_t new_flags;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_uint32_t,v,&new_flags) != 0) return -1;
 if (DeeMarshal_ConfirmWriteFlags(new_flags) != 0) return -1;
 if DEE_UNLIKELY(DeeNativeMutex_Acquire(&self->mwm_lock) != 0) return -1;
 DeeMarshalWriteMap_SET_FLAGS(&self->mwm_map,new_flags);
 DeeNativeMutex_ReleaseNoexcept(&self->mwm_lock);
 return 0;
}

static DeeObject *DEE_CALL _deemarshalwritemap_write(
 DeeMarshalWriteMapObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *fp,*ob; int error;
 if (DeeTuple_Unpack(args,"oo:write",&fp,&ob) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&fp,(DeeTypeObject *)&DeeFile_Type) != 0) return NULL;
 if (DeeNativeMutex_Acquire(&self->mwm_lock) != 0) return NULL;
 error = DeeMarshal_WriteObjectWithMap(fp,&self->mwm_map,ob);
 DeeNativeMutex_ReleaseNoexcept(&self->mwm_lock);
 if (error != 0) return NULL;
 DeeReturn_None;
}

#if DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING) == DEE_STRING_ENCODING(Wide) && DEE_CONFIG_HAVE_ENCODING_WIDE
#define _deemarshalwritemap_write_string _deemarshalwritemap_write_widestring
#elif DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING) == DEE_STRING_ENCODING(Utf8) && DEE_CONFIG_HAVE_ENCODING_UTF8
#define _deemarshalwritemap_write_string _deemarshalwritemap_write_utf8string
#elif DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING) == DEE_STRING_ENCODING(Utf16) && DEE_CONFIG_HAVE_ENCODING_UTF8
#define _deemarshalwritemap_write_string _deemarshalwritemap_write_utf16string
#elif DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING) == DEE_STRING_ENCODING(Utf32) && DEE_CONFIG_HAVE_ENCODING_UTF8
#define _deemarshalwritemap_write_string _deemarshalwritemap_write_utf32string
#endif

#if DEE_CONFIG_HAVE_ENCODING_WIDE
static DeeObject *DEE_CALL _deemarshalwritemap_write_widestring(
 DeeMarshalWriteMapObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *fp,*arg; Dee_WideChar const *p;
 int error; Dee_size_t newlen,len = (Dee_size_t)-1;
 if (DeeTuple_Unpack(args,"oo|Iu:write_widestring",&fp,&arg,&len) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&fp,(DeeTypeObject *)&DeeFile_Type) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (!DeeWideString_Check(arg)) {
  if (DeeObject_GetPointerEx(arg,DeeObject_TYPE(Dee_WideChar),(void **)&p) != 0) return NULL;
  len = Dee_WideStrNLen(p,len);
 } else
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (DeeError_TypeError_CheckTypeExact(arg,&DeeWideString_Type) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 {
  p = DeeWideString_STR(arg);
  newlen = DeeWideString_SIZE(arg);
  if (newlen > len) len = newlen;
 }
 if (DeeNativeMutex_Acquire(&self->mwm_lock) != 0) return NULL;
 error = DeeMarshal_WriteWideStringWithMapAndLength(fp,&self->mwm_map,len,p);
 DeeNativeMutex_ReleaseNoexcept(&self->mwm_lock);
 if (error != 0) return NULL;
 DeeReturn_None;
}
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
static DeeObject *DEE_CALL _deemarshalwritemap_write_utf8string(
 DeeMarshalWriteMapObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *fp,*arg; Dee_Utf8Char const *p;
 int error; Dee_size_t newlen,len = (Dee_size_t)-1;
 if (DeeTuple_Unpack(args,"oo|Iu:write_utf8string",&fp,&arg,&len) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&fp,(DeeTypeObject *)&DeeFile_Type) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (!DeeUtf8String_Check(arg)) {
  if (DeeObject_GetPointerEx(arg,DeeObject_TYPE(Dee_Utf8Char),(void **)&p) != 0) return NULL;
  len = Dee_Utf8StrNLen(p,len);
 } else
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (DeeError_TypeError_CheckTypeExact(arg,&DeeUtf8String_Type) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 {
  p = DeeUtf8String_STR(arg);
  newlen = DeeUtf8String_SIZE(arg);
  if (newlen > len) len = newlen;
 }
 if (DeeNativeMutex_Acquire(&self->mwm_lock) != 0) return NULL;
 error = DeeMarshal_WriteUtf8StringWithMapAndLength(fp,&self->mwm_map,len,p);
 DeeNativeMutex_ReleaseNoexcept(&self->mwm_lock);
 if (error != 0) return NULL;
 DeeReturn_None;
}
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
static DeeObject *DEE_CALL _deemarshalwritemap_write_utf16string(
 DeeMarshalWriteMapObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *fp,*arg; Dee_Utf16Char const *p;
 int error; Dee_size_t newlen,len = (Dee_size_t)-1;
 if (DeeTuple_Unpack(args,"oo|Iu:write_utf16string",&fp,&arg,&len) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&fp,(DeeTypeObject *)&DeeFile_Type) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (!DeeUtf16String_Check(arg)) {
  if (DeeObject_GetPointerEx(arg,DeeObject_TYPE(Dee_Utf16Char),(void **)&p) != 0) return NULL;
  len = Dee_Utf16StrNLen(p,len);
 } else
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (DeeError_TypeError_CheckTypeExact(arg,&DeeUtf16String_Type) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 {
  p = DeeUtf16String_STR(arg);
  newlen = DeeUtf16String_SIZE(arg);
  if (newlen > len) len = newlen;
 }
 if (DeeNativeMutex_Acquire(&self->mwm_lock) != 0) return NULL;
 error = DeeMarshal_WriteUtf16StringWithMapAndLength(fp,&self->mwm_map,len,p);
 DeeNativeMutex_ReleaseNoexcept(&self->mwm_lock);
 if (error != 0) return NULL;
 DeeReturn_None;
}
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
static DeeObject *DEE_CALL _deemarshalwritemap_write_utf32string(
 DeeMarshalWriteMapObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *fp,*arg; Dee_Utf32Char const *p;
 int error; Dee_size_t newlen,len = (Dee_size_t)-1;
 if (DeeTuple_Unpack(args,"oo|Iu:write_utf32string",&fp,&arg,&len) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&fp,(DeeTypeObject *)&DeeFile_Type) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (!DeeUtf32String_Check(arg)) {
  if (DeeObject_GetPointerEx(arg,DeeObject_TYPE(Dee_Utf32Char),(void **)&p) != 0) return NULL;
  len = Dee_Utf32StrNLen(p,len);
 } else
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (DeeError_TypeError_CheckTypeExact(arg,&DeeUtf32String_Type) != 0) return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 {
  p = DeeUtf32String_STR(arg);
  newlen = DeeUtf32String_SIZE(arg);
  if (newlen > len) len = newlen;
 }
 if (DeeNativeMutex_Acquire(&self->mwm_lock) != 0) return NULL;
 error = DeeMarshal_WriteUtf32StringWithMapAndLength(fp,&self->mwm_map,len,p);
 DeeNativeMutex_ReleaseNoexcept(&self->mwm_lock);
 if (error != 0) return NULL;
 DeeReturn_None;
}
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */


static struct DeeGetSetDef const _deemarshalwritemap_tp_getsets[] = {
 DEE_GETSETDEF_v100("flags",
  member(&_deemarshalwritemap_flags_get),
  member(&_deemarshalwritemap_flags_del),
  member(&_deemarshalwritemap_flags_set),"-> uint32_t\n"
  "@return: The marshal version associated with the readmap\n"
  "@throws Signal.Interrupt: The calling thread was interrupted\n"
  "@throws Error.RuntimeError.NotImplemented: The given flags are invalid\n"
  "@throws Error.SystemError: Failed to acquire a lock"),
 DEE_GETSETDEF_END_v100
};
static struct DeeMethodDef const _deemarshalwritemap_tp_methods[] = {
 DEE_METHODDEF_v100("write",member(&_deemarshalwritemap_write),"(file fp, object ob) -> none\n"
  "@param fp: The file to write to\n"
  "@param ob: The object that should be written\n"
  "@throws Signal.Interrupt: The calling thread was interrupted\n"
  "@throws Error.SystemError: Failed to acquire a lock\n"
  "@throws object: Same as the writing @ob through the marshal"),
#ifdef _deemarshalwritemap_write_string
 DEE_METHODDEF_v100("write_string",member(&_deemarshalwritemap_write_string),
  "(file fp, string s, size_t max_len = size_t(-1)) -> none\n"
  "(file fp, char const *p, size_t max_len = size_t(-1)) -> none\n"
  "@param fp: The file to write to\n"
  "@param max_len: The maximal length of the string that should be written\n"
  "@throws Signal.Interrupt: The calling thread was interrupted\n"
  "@throws Error.SystemError: Failed to acquire a lock\n"
  "@throws object: Same as the writing the given string through the marshal"),
#endif
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 DEE_METHODDEF_v100("write_widestring",member(&_deemarshalwritemap_write_widestring),
  "(file fp, string.wide s, size_t max_len = size_t(-1)) -> none\n"
  "(file fp, wchar_t const *p, size_t max_len = size_t(-1)) -> none\n"
  "@param fp: The file to write to\n"
  "@param max_len: The maximal length of the string that should be written\n"
  "@throws Signal.Interrupt: The calling thread was interrupted\n"
  "@throws Error.SystemError: Failed to acquire a lock\n"
  "@throws object: Same as the writing the given string through the marshal"),
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 DEE_METHODDEF_v100("write_utf8string",member(&_deemarshalwritemap_write_utf8string),
  "(file fp, string.utf8 s, size_t max_len = size_t(-1)) -> none\n"
  "(file fp, char const *p, size_t max_len = size_t(-1)) -> none\n"
  "@param fp: The file to write to\n"
  "@param max_len: The maximal length of the string that should be written\n"
  "@throws Signal.Interrupt: The calling thread was interrupted\n"
  "@throws Error.SystemError: Failed to acquire a lock\n"
  "@throws object: Same as the writing the given string through the marshal"),
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 DEE_METHODDEF_v100("write_utf16string",member(&_deemarshalwritemap_write_utf16string),
  "(file fp, string.utf16 s, size_t max_len = size_t(-1)) -> none\n"
  "(file fp, char16_t const *p, size_t max_len = size_t(-1)) -> none\n"
  "@param fp: The file to write to\n"
  "@param max_len: The maximal length of the string that should be written\n"
  "@throws Signal.Interrupt: The calling thread was interrupted\n"
  "@throws Error.SystemError: Failed to acquire a lock\n"
  "@throws object: Same as the writing the given string through the marshal"),
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 DEE_METHODDEF_v100("write_utf32string",member(&_deemarshalwritemap_write_utf32string),
  "(file fp, string.utf32 s, size_t max_len = size_t(-1)) -> none\n"
  "(file fp, char32_t const *p, size_t max_len = size_t(-1)) -> none\n"
  "@param fp: The file to write to\n"
  "@param max_len: The maximal length of the string that should be written\n"
  "@throws Signal.Interrupt: The calling thread was interrupted\n"
  "@throws Error.SystemError: Failed to acquire a lock\n"
  "@throws object: Same as the writing the given string through the marshal"),
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 DEE_METHODDEF_END_v100
};


static struct DeeMemberDef const _deemarshalwritemap_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("FLAG_NONE",Dee_uint32_t,DEE_MARSHAL_WRITEFLAG_NONE),
 DEE_MEMBERDEF_CONST_DOC_v100("FLAG_REFERENCE_ALL",Dee_uint32_t,DEE_MARSHAL_WRITEFLAG_REFERENCE_ALL,
                              "\tIf set, all gc object written can be referenced.\n"
                              "\tMight reduce binary size for very big objects.\n"
                              "\tIncreases binary size for smaller objects."),
 DEE_MEMBERDEF_CONST_DOC_v100("FLAG_TYPE_NAMES",Dee_uint32_t,DEE_MARSHAL_WRITEFLAG_TYPE_NAMES,
                              "\tWrite type names."),
 DEE_MEMBERDEF_CONST_DOC_v100("FLAG_UUID_REFS",Dee_uint32_t,DEE_MARSHAL_WRITEFLAG_UUID_REFS,
                              "\tAllow uuid references.\n"
                              "\tWARNING: Will cause unknown uuid errors if the target doesn't recognize a given uuid."),
 DEE_MEMBERDEF_CONST_DOC_v100("FLAG_DEFAULT",Dee_uint32_t,DEE_MARSHAL_WRITEFLAG_DEFAULT,
                              "\tDefault set of flags."),
 DEE_MEMBERDEF_END_v100
};








DeeTypeObject DeeMarshalReadMap_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("readmap"),member(
  "(uint16_t version = readmap.VERSION) -> readmap\n"
  "@throws Signal.Interrupt: The calling thread was interrupted.\n"
  "@throws Error.SystemError: Failed to initalize the lock\n"
  "@throws Error.RuntimeError.NotImplemented: The given @version isn't supported\n"),null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(
  sizeof(DeeMarshalReadMapObject),null,
  member(&_deemarshalreadmap_tp_ctor),null,null,
  member(&_deemarshalreadmap_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deemarshalreadmap_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deemarshalreadmap_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,member(_deemarshalreadmap_tp_getsets),
  member(_deemarshalreadmap_tp_methods),
  member(_deemarshalreadmap_tp_class_members),null,
  member(_deemarshalreadmap_tp_class_methods)),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DeeTypeObject DeeMarshalWriteMap_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("writemap"),member(
  "(uint32_t flags = writemap.FLAG_DEFAULT) -> writemap\n"
  "@throws Signal.Interrupt: The calling thread was interrupted.\n"
  "@throws Error.SystemError: Failed to initalize the lock\n"
  "@throws Error.RuntimeError.NotImplemented: The given @flags are invalid\n"),null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(
  sizeof(DeeMarshalWriteMapObject),null,
  member(&_deemarshalwritemap_tp_ctor),null,null,
  member(&_deemarshalwritemap_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deemarshalwritemap_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deemarshalwritemap_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,member(_deemarshalwritemap_tp_getsets),
  member(_deemarshalwritemap_tp_methods),
  member(_deemarshalwritemap_tp_class_members),null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};


static DeeObject *DEE_CALL _deemarshal_write_compiled_file_header(DeeObject *args) {
 DeeObject *fp;
 if (DeeTuple_Unpack(args,"o:write_compiled_file_header",&fp) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&fp,(DeeTypeObject *)&DeeFile_Type) != 0) return NULL;
 if (DeeMarshal_WriteCompiledFileHeader(fp) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deemarshal_is_compiled_file(DeeObject *args) {
 DeeObject *fp; Dee_uint32_t deemon_version; DeeMarshalVersion marshal_version; int error;
 if (DeeTuple_Unpack(args,"o:write_compiled_file_header",&fp) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&fp,(DeeTypeObject *)&DeeFile_Type) != 0) return NULL;
 if ((error = DeeMarshal_IsCompiledFile(fp,&deemon_version,&marshal_version)) < 0) return NULL;
 if (!error) deemon_version = 0,marshal_version = 0;
 return Dee_BuildValue("(bI32uI16u)",error,deemon_version,marshal_version);
}





//////////////////////////////////////////////////////////////////////////
// Module exports
struct DeeDexExportDef DeeDex_Exports[] = {
 DeeDex_EXPORT_OBJECT("readmap",&DeeMarshalReadMap_Type),
 DeeDex_EXPORT_OBJECT("writemap",&DeeMarshalWriteMap_Type),
 DeeDex_EXPORT_FUNCTION("write_compiled_file_header",&_deemarshal_write_compiled_file_header,
  "(file fp) -> none\n"
  "@param fp: The file to write the header to\n"
  "@throws object: Same as writing to @fp\n"
  "\tWrites the header of a compiled deemon file to @(fp)."),
 DeeDex_EXPORT_FUNCTION("is_compiled_file",&_deemarshal_is_compiled_file,
  "(file fp) -> (bool,uint32_t,uint16_t)\n"
  "@param fp: The file to read the header from\n"
  "@throws object: Same as reading from @fp\n"
  "\tChecks if @fp describes a compiled deemon file by reading a compiled file header, as written with #_marshal.write_compiled_file_header.\n"
  "\tIf the header wasn't found, the tuple '(false,0,0)' is returned and @fp is rewinded to the position it was at when the function was called.\n"
  "\tIf the header was found, it will be skipped and a tuple '(true,deemon_version,marshal_version)' is returned, where 'deemon_version' referrs to "
    "the full deemon version that the header was generated by, and 'marshal_version' to the marshal version that the remainder of the file is written in."),
 DeeDex_EXPORT_END
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_MARSHAL_C */
