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
#ifndef GUARD_DEEMON_MARSHAL_C
#define GUARD_DEEMON_MARSHAL_C 1
#define DEE_LIMITED_API 1

// /include/*
#include <deemon/__bswap_intrin.inl>
#include <deemon/__conf.inl>
#include <deemon/array.h>
#include <deemon/bool.h>
#include <deemon/class.h>
#include <deemon/deemonrun.h>
#include <deemon/dex.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/foreign_function.h>
#include <deemon/fs_api.h>
#include <deemon/gc.h>
#include <deemon/marshal.h>
#include <deemon/none.h>
#include <deemon/object.h>
#include <deemon/optional/uuid.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/runtime/builtins.h>
#include <deemon/runtime/extern.h>
#include <deemon/structured.h>
#include <deemon/super.h>
#include <deemon/type.h>

// /src/*
#include <deemon/marshal_data.h>
#include <deemon/uuid_list.h>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

DEE_A_RET_WUNUSED Dee_uint32_t DeeMarshalWriteMap_GetFlags(
 DEE_A_IN struct DeeMarshalWriteMap const *self) {
 DEE_ASSERT(self);
 return self->mwm_flags;
}
void DeeMarshalWriteMap_SetFlags(
 DEE_A_INOUT struct DeeMarshalWriteMap *self, DEE_A_IN Dee_uint32_t flags) {
 DEE_ASSERT(self);
 self->mwm_flags = flags;
}
DEE_A_RET_EXCEPT(NULL) struct DeeMarshalWriteMap *DeeMarshalWriteMap_New(void) {
 struct DeeMarshalWriteMap *result;
 while ((result = (struct DeeMarshalWriteMap *)
  malloc_nz(sizeof(struct DeeMarshalWriteMap))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 result->mwm_ref_next = 0;
 result->mwm_refc = 0;
 result->mwm_refv = NULL;
 return result;
}
void DeeMarshalWriteMap_Delete(DEE_A_IN DEE_A_OUT_INVALID struct DeeMarshalWriteMap *self) {
 DEE_ASSERT(self);
 _DeeMarshalWriteMap_Quit(self);
 free_nn(self);
}
DEE_A_RET_EXCEPT(-1) Dee_uint32_t _DeeMarshalWriteMap_RegisterID(
 DEE_A_IN struct DeeMarshalWriteMap *self,
 DEE_A_IN DeeTypeObject const *tp_ob, DEE_A_INOUT DeeObject *ob) {
 struct _DeeMarshalWriteRef *new_refs;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(ob));
 DEE_ASSERT(DeeObject_Check(tp_ob) && DeeType_Check(tp_ob));
 DEE_ASSERT(DeeObject_InstanceOf(ob,tp_ob));
 DEE_ASSERT(_DeeMarshalWriteMap_FindObject(self,tp_ob,ob) == (Dee_uint32_t)-1);
 if ((new_refs = (struct _DeeMarshalWriteRef *)realloc_nz(
  self->mwm_refv,(self->mwm_refc+1)*sizeof(struct _DeeMarshalWriteRef))) == NULL) {
  DeeError_NoMemory();
  return (Dee_uint32_t)-1;
 }
 self->mwm_refv = new_refs;
 new_refs += self->mwm_refc++;
 new_refs->mwr_tp = tp_ob;
 Dee_INCREF(new_refs->mwr_ob = ob);
 return new_refs->mwr_id = self->mwm_ref_next++;
}
DEE_A_RET_NOEXCEPT(-1) Dee_uint32_t _DeeMarshalWriteMap_FindObject(
 DEE_A_IN struct DeeMarshalWriteMap const *self,
 DEE_A_IN DeeTypeObject const *tp_ob, DEE_A_INOUT DeeObject *ob) {
 struct _DeeMarshalWriteRef *iter,*end; int temp;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(ob));
 DEE_ASSERT(DeeObject_Check(tp_ob) && DeeType_Check(tp_ob));
 DEE_ASSERT(DeeObject_InstanceOf(ob,tp_ob));
 end = (iter = self->mwm_refv)+self->mwm_refc;
 while (iter != end) {
  if (iter->mwr_ob == ob && DeeType_IsSameOrDerived(
   tp_ob,iter->mwr_tp)) return iter->mwr_id;
  temp = DeeObject_TDeepEquals(tp_ob,ob,iter->mwr_ob);
  if (temp < 0) DeeError_Handled();
  else if (temp) return iter->mwr_id;
  ++iter;
 }
 return (Dee_uint32_t)-1;
}
DEE_A_RET_NOEXCEPT(-1) Dee_uint32_t _DeeMarshalWriteMap_FindWideStringWithLength(
 DEE_A_IN struct DeeMarshalWriteMap const *self,
 DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s) {
 struct _DeeMarshalWriteRef *iter,*end;
 DEE_ASSERT(self); DEE_ASSERT(!len || s);
 end = (iter = self->mwm_refv)+self->mwm_refc;
 while (iter != end) {
  if (iter->mwr_tp == &DeeWideString_Type &&
      DeeWideString_SIZE(iter->mwr_ob) == len &&
      Dee_WideStrNCmp(DeeWideString_STR(iter->mwr_ob),s,len) == 0
      ) return iter->mwr_id;
  ++iter;
 }
 return (Dee_uint32_t)-1;
}
DEE_A_RET_NOEXCEPT(-1) Dee_uint32_t _DeeMarshalWriteMap_FindUtf8StringWithLength(
 DEE_A_IN struct DeeMarshalWriteMap const *self,
 DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s) {
 struct _DeeMarshalWriteRef *iter,*end;
 DEE_ASSERT(self); DEE_ASSERT(!len || s);
 end = (iter = self->mwm_refv)+self->mwm_refc;
 while (iter != end) {
  if (iter->mwr_tp == &DeeUtf8String_Type &&
      DeeUtf8String_SIZE(iter->mwr_ob) == len &&
      Dee_Utf8StrNCmp(DeeUtf8String_STR(iter->mwr_ob),s,len) == 0
      ) return iter->mwr_id;
  ++iter;
 }
 return (Dee_uint32_t)-1;
}
DEE_A_RET_NOEXCEPT(-1) Dee_uint32_t _DeeMarshalWriteMap_FindUtf16StringWithLength(
 DEE_A_IN struct DeeMarshalWriteMap const *self,
 DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s) {
 struct _DeeMarshalWriteRef *iter,*end;
 DEE_ASSERT(self); DEE_ASSERT(!len || s);
 end = (iter = self->mwm_refv)+self->mwm_refc;
 while (iter != end) {
  if (iter->mwr_tp == &DeeUtf16String_Type &&
      DeeUtf16String_SIZE(iter->mwr_ob) == len &&
      Dee_Utf16StrNCmp(DeeUtf16String_STR(iter->mwr_ob),s,len) == 0
      ) return iter->mwr_id;
  ++iter;
 }
 return (Dee_uint32_t)-1;
}
DEE_A_RET_NOEXCEPT(-1) Dee_uint32_t _DeeMarshalWriteMap_FindUtf32StringWithLength(
 DEE_A_IN struct DeeMarshalWriteMap const *self,
 DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s) {
 struct _DeeMarshalWriteRef *iter,*end;
 DEE_ASSERT(self); DEE_ASSERT(!len || s);
 end = (iter = self->mwm_refv)+self->mwm_refc;
 while (iter != end) {
  if (iter->mwr_tp == &DeeUtf32String_Type &&
      DeeUtf32String_SIZE(iter->mwr_ob) == len &&
      Dee_Utf32StrNCmp(DeeUtf32String_STR(iter->mwr_ob),s,len) == 0
      ) return iter->mwr_id;
  ++iter;
 }
 return (Dee_uint32_t)-1;
}



DEE_A_RET_EXCEPT(NULL) struct DeeMarshalReadMap *
DeeMarshalReadMap_New(DEE_A_IN DeeMarshalVersion version) {
 struct DeeMarshalReadMap *result;
 if (_DeeMarshalReadMap_CheckVersion(version) != 0) return NULL;
 while ((result = (struct DeeMarshalReadMap *)
  malloc_nz(sizeof(struct DeeMarshalReadMap))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 _DeeMarshalReadMap_Init(result,version);
 return result;
}
void DeeMarshalReadMap_Delete(DEE_A_IN DEE_A_OUT_INVALID struct DeeMarshalReadMap *self) {
 DEE_ASSERT(self);
 _DeeMarshalReadMap_Quit(self);
 free_nn(self);
}
DEE_A_RET_MAYBE_NULL struct DeeUUID const *DeeMarshalReadMap_GetUUID(
 DEE_A_IN struct DeeMarshalReadMap const *self) {
 DEE_ASSERT(self);
 return DeeMarshalReadMap_UUID(self);
}
DEE_A_RET_WUNUSED DeeMarshalVersion DeeMarshalReadMap_Version(
 DEE_A_IN struct DeeMarshalReadMap const *self) {
 DEE_ASSERT(self);
 return DeeMarshalReadMap_VERSION(self);
}


DEE_A_RET_WUNUSED int DeeMarshal_IsSupported(DEE_A_IN DeeMarshalVersion version) {
 if (version > DEE_MARSHAL_VERSION) return 0; // Future version
 if (version <= DEE_MARSHAL_V100) return 0; // Too old
 return 1;
}
DEE_A_RET_EXCEPT(-1) int _DeeMarshalReadMap_CheckVersion(
 DEE_A_IN DeeMarshalVersion version) {
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
 return 0;
}

DEE_A_RET_EXCEPT(-1) int _DeeMarshalReadMap_TryPushRefWithoutIncref(
 DEE_A_INOUT struct DeeMarshalReadMap *self, DEE_A_IN Dee_uint32_t id, DEE_A_IN DeeObject *ob) {
 struct _DeeMarshalReadRef *new_refs;
 DEE_ASSERT(self);
 while ((new_refs = (struct _DeeMarshalReadRef *)realloc_nz(self->mrm_refv,
  (self->mrm_refc+1)*sizeof(struct _DeeMarshalReadRef))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
//DeeError_NoMemory();
  return -1;
 }
 self->mrm_refv = new_refs;
 new_refs += self->mrm_refc++;
 new_refs->ref_id = id;
 new_refs->ref_ob = ob; // Incref must be done by the caller
 return 0;
}
void _DeeMarshalReadMap_PopRefWithoutDecref(
 DEE_A_INOUT struct DeeMarshalReadMap *self, DEE_A_IN DeeObject *ob) {
 struct _DeeMarshalReadRef *iter,*end;
 DEE_ASSERT(self);
 //DEE_ASSERT(DeeObject_Check(ob));
 iter = end = self->mrm_refv+self->mrm_refc;
 while (1) {
  DEE_ASSERTF(iter != self->mrm_refv,
              "Object not found: %k:%k",
              Dee_TYPE(ob),ob);
  if ((--iter)->ref_ob == ob) {
   //Dee_DECREF(ob);
   memmove(iter,iter+1,(((Dee_size_t)(end-iter))-1)*
           sizeof(struct _DeeMarshalReadRef));
   --self->mrm_refc;
   return;
  }
 }
}
DEE_A_RET_NOEXCEPT(NULL) DeeObject *_DeeMarshalReadMap_GetRef(
 DEE_A_INOUT struct DeeMarshalReadMap *self, DEE_A_IN Dee_uint32_t id) {
 struct _DeeMarshalReadRef *iter,*end;
 DEE_ASSERT(self);
 end = (iter = self->mrm_refv)+self->mrm_refc;
 while (iter != end) {
  if (iter->ref_id == id) return iter->ref_ob;
  ++iter;
 }
 return NULL;
}


//////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------- //
//////////////////////////////////////////////////////////////////////////


DEE_A_RET_EXCEPT_REF DeeObject *DeeMarshal_ReadObject(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_IN DeeMarshalVersion version) {
 DeeObject *result;
 if (_DeeMarshalReadMap_CheckVersion(version) != 0) return NULL;
 {struct DeeMarshalReadMap map; _DeeMarshalReadMap_Init(&map,version);
  result = DeeMarshal_ReadObjectWithMap(file,&map);
  _DeeMarshalReadMap_Quit(&map);
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeMarshal_ReadObjects(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file,
 DEE_A_IN Dee_size_t resultc, DEE_A_OUT_W(resultc) DeeObject **resultv,
 DEE_A_IN DeeMarshalVersion version) {
 int result;
 if (_DeeMarshalReadMap_CheckVersion(version) != 0) return -1;
 {struct DeeMarshalReadMap map; _DeeMarshalReadMap_Init(&map,version);
  result = DeeMarshal_ReadObjectsWithMap(file,&map,resultc,resultv);
  _DeeMarshalReadMap_Quit(&map);
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeMarshal_WriteObject(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_IN DeeObject *ob) {
 int result; struct DeeMarshalWriteMap map = DeeMarshalWriteMap_INIT();
 result = DeeMarshal_WriteObjectWithMap(file,&map,ob);
 _DeeMarshalWriteMap_Quit(&map);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeMarshal_WriteObjects(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file,
 DEE_A_IN Dee_size_t objectc, DEE_A_IN_R(objectc) DeeObject *const *objectv) {
 int result; struct DeeMarshalWriteMap map = DeeMarshalWriteMap_INIT();
 result = DeeMarshal_WriteObjectsWithMap(file,&map,objectc,objectv);
 _DeeMarshalWriteMap_Quit(&map);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeMarshal_WriteObjectEx(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_IN DeeObject *ob, DEE_A_IN Dee_uint32_t flags) {
 int result; struct DeeMarshalWriteMap map; _DeeMarshalWriteMap_Init(&map,flags);
 result = DeeMarshal_WriteObjectWithMap(file,&map,ob);
 _DeeMarshalWriteMap_Quit(&map);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeMarshal_WriteObjectsEx(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_IN Dee_size_t objectc,
 DEE_A_IN_R(objectc) DeeObject *const *objectv, DEE_A_IN Dee_uint32_t flags) {
 int result; struct DeeMarshalWriteMap map; _DeeMarshalWriteMap_Init(&map,flags);
 result = DeeMarshal_WriteObjectsWithMap(file,&map,objectc,objectv);
 _DeeMarshalWriteMap_Quit(&map);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeMarshal_ReadObjectsWithMap(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalReadMap *file_map, 
 DEE_A_IN Dee_size_t resultc, DEE_A_OUT_W(resultc) DeeObject **resultv) {
 DeeObject **iter = resultv;
 while (resultc--) {
  if ((*iter = DeeMarshal_ReadObjectWithMap(file,file_map)) == NULL) {
   while (iter != resultv) Dee_DECREF(*--iter);
   return -1;
  }
  ++iter;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeMarshal_WriteObjectsWithMap(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalWriteMap *file_map,
 DEE_A_IN Dee_size_t objectc, DEE_A_IN_R(objectc) DeeObject *const *objectv) {
 while (objectc--) if (DeeMarshal_WriteObjectWithMap(file,file_map,*objectv++) != 0) return -1;
 return 0;
}



DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) _DeeMashal_WriteGetRef(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_IN Dee_uint32_t ref_id) {
 struct DeeMarshalHeaderGetRef block;
 DEE_ASSERT(DeeObject_Check(file) && DeeFile_Check(file));
 if (ref_id <= 0xFF) {
  block.mhgr_marker = DEE_MARSHALID_GETREF8;
  block.mhgr_block.mhgr_ref8 = (Dee_uint8_t)ref_id;
 } else if (ref_id <= 0xFFFF) {
  block.mhgr_marker = DEE_MARSHALID_GETREF16;
  block.mhgr_block.mhgr_ref16 = DEE_BUILTIN_LESWAP16((Dee_uint16_t)ref_id);
 } else {
  block.mhgr_marker = DEE_MARSHALID_GETREF32;
  block.mhgr_block.mhgr_ref32 = DEE_BUILTIN_LESWAP32((Dee_uint32_t)ref_id);
 }
 return DeeFile_WriteAll(file,&block,DeeMarshalHeaderGetRef_SIZE(&block));
}


DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeMarshal_WriteWideStringWithMapAndLength(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalWriteMap *file_map,
 DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s) {
 Dee_uint32_t ref_id; DeeObject *file_str; int result;
 ref_id = _DeeMarshalWriteMap_FindWideStringWithLength(file_map,len,s);
 if DEE_UNLIKELY(ref_id != (Dee_uint32_t)-1)
  return _DeeMashal_WriteGetRef(file,ref_id);
 if ((file_str = DeeWideString_NewWithLength(len,s)) == NULL) return -1;
 result = DeeMarshal_TWriteNewObjectWithMap(file,file_map,&DeeWideString_Type,file_str);
 Dee_DECREF(file_str);
 return result;
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeMarshal_WriteUtf8StringWithMapAndLength(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalWriteMap *file_map,
 DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s) {
 Dee_uint32_t ref_id; DeeObject *file_str; int result;
 ref_id = _DeeMarshalWriteMap_FindUtf8StringWithLength(file_map,len,s);
 if DEE_UNLIKELY(ref_id != (Dee_uint32_t)-1)
  return _DeeMashal_WriteGetRef(file,ref_id);
 if ((file_str = DeeUtf8String_NewWithLength(len,s)) == NULL) return -1;
 result = DeeMarshal_TWriteNewObjectWithMap(file,file_map,&DeeUtf8String_Type,file_str);
 Dee_DECREF(file_str);
 return result;
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeMarshal_WriteUtf16StringWithMapAndLength(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalWriteMap *file_map,
 DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s) {
 Dee_uint32_t ref_id; DeeObject *file_str; int result;
 ref_id = _DeeMarshalWriteMap_FindUtf16StringWithLength(file_map,len,s);
 if DEE_UNLIKELY(ref_id != (Dee_uint32_t)-1)
  return _DeeMashal_WriteGetRef(file,ref_id);
 if ((file_str = DeeUtf16String_NewWithLength(len,s)) == NULL) return -1;
 result = DeeMarshal_TWriteNewObjectWithMap(file,file_map,&DeeUtf16String_Type,file_str);
 Dee_DECREF(file_str);
 return result;
}
DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeMarshal_WriteUtf32StringWithMapAndLength(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalWriteMap *file_map,
 DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s) {
 Dee_uint32_t ref_id; DeeObject *file_str; int result;
 ref_id = _DeeMarshalWriteMap_FindUtf32StringWithLength(file_map,len,s);
 if DEE_UNLIKELY(ref_id != (Dee_uint32_t)-1)
  return _DeeMashal_WriteGetRef(file,ref_id);
 if ((file_str = DeeUtf32String_NewWithLength(len,s)) == NULL) return -1;
 result = DeeMarshal_TWriteNewObjectWithMap(file,file_map,&DeeUtf32String_Type,file_str);
 Dee_DECREF(file_str);
 return result;
}




DEE_COMPILER_STRUCTURE_PACKED_BEGIN
struct DeeCompiledFileHeader {
 // it's a gonna magic...
 Dee_uint8_t magic[8];      // == "deemon\x03\x05"
 Dee_uint32_t reserved_0;   // == 0 (Reserved)
 Dee_uint16_t reserved_1;   // == 0 (Reserved)
 Dee_uint16_t marshal_ver;  // == DEE_MARSHAL_VERSION
 Dee_uint32_t full_version; // == DEE_VERSION_FULL
};
DEE_COMPILER_STRUCTURE_PACKED_END
static struct DeeCompiledFileHeader const default_header = {
 // ...it's a gonna magic...
 {'d','e','e','m','o','n','\x03','\x05'},
 0,0,DEE_MARSHAL_VERSION,
 DEE_BUILTIN_LESWAP32_M(DEE_VERSION_FULL)
};


DEE_A_RET_EXCEPT(-1) int DeeMarshal_WriteCompiledFileHeader(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file) {
#ifdef DEE_PLATFORM_UNIX
 // Add the script tag to the front of the compiled file
 // NOTE: Don't be fooled: This is still very much a binary file!
 if (DeeFile_Printf(file,"#!%K\n",DeeFS_GetDeemonExecutable()) != 0) return -1;
#else
 // We use the default linux install path on windows
 if (DeeFile_PRINT(file,"#!/usr/bin/deemon\n") != 0) return -1;
#endif
 if (DeeFile_WriteAll(file,&default_header,sizeof(default_header)) != 0) return -1;
 return 0;
}

DEE_STATIC_INLINE(int) _DeeMarshal_DoIsCompiledFile(
 DeeObject *file, struct DeeCompiledFileHeader *header,
 Dee_uint32_t *deemon_version, DeeMarshalVersion *marshal_version) {
 DEE_ASSERT(deemon_version && marshal_version);
 // ...One dream, one soul, one prize...
 if (memcmp(header->magic,default_header.magic,sizeof(default_header.magic)) == 0) {
  *deemon_version = DEE_BUILTIN_LESWAP32(header->full_version);
  // NOTE: Technically it would be good enough to simple compare: '*deemon_version > DEE_VERSION_FULL'
  //       But we check manually in case deemon ever gets a second development branch
  if (DEE_VERSION_GET_API(*deemon_version) > DEE_VERSION_API ||
      DEE_VERSION_GET_COMPILER(*deemon_version) > DEE_VERSION_COMPILER ||
     (DEE_VERSION_GET_API_AND_COMPILER(*deemon_version) ==
      DEE_VERSION_GET_API_AND_COMPILER(DEE_VERSION_FULL) &&
      DEE_VERSION_GET_REVISION(*deemon_version) > DEE_VERSION_REVISION)) {
   //                   ___________________
   // You've got to go /Back To The Future/
   if (DeeFile_StdPrintf(DEE_STDERR,
    "Warning: File %k compiled with future deemon %d/%d:%d\n",
    file,(int)DEE_VERSION_GET_API(*deemon_version),
    (int)DEE_VERSION_GET_COMPILER(*deemon_version),
    (int)DEE_VERSION_GET_REVISION(*deemon_version)
    ) != 0) return -1;
  }
  *marshal_version = (DeeMarshalVersion)DEE_BUILTIN_LESWAP16(header->marshal_ver);
  return 1;
 }
 return 0;
}
DEE_STATIC_INLINE(int) _DeeMarshal_IsCompiledFile(
 DeeObject *file, struct DeeCompiledFileHeader *header,
 Dee_uint32_t *deemon_version, DeeMarshalVersion *marshal_version) {
 Dee_size_t did_read;
 DEE_ASSERT(deemon_version && marshal_version);
 if (DeeFile_Read(file,header,sizeof(*header),&did_read) != 0) return -1;
 if (did_read != sizeof(*header)) {
  if (DeeFile_Seek(file,-(Dee_int64_t)did_read,DEE_SEEK_CUR,NULL) != 0) return -1;
  return 2; // File too small
 }
 return _DeeMarshal_DoIsCompiledFile(file,header,deemon_version,marshal_version);
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeMarshal_IsCompiledFile(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_OUT Dee_uint32_t *deemon_version,
 DEE_A_OUT DeeMarshalVersion *marshal_version) {
 Dee_size_t did_read; int temp;
 struct DeeCompiledFileHeader header;
 DEE_ASSERT(deemon_version && marshal_version);
 if ((temp = _DeeMarshal_IsCompiledFile(file,&header,deemon_version,marshal_version)) < 0) return -1;
 if (temp == 1) goto found; // Yes!
 if (temp == 2) return 0; // File too small
 // ...One goal, one golden glance of what should be...
 if (header.magic[0] == '#') {
  Dee_uint64_t total_read;
  Dee_uint8_t ch;
  char *iter,*end;
  total_read = sizeof(header);
  end = (iter = (char *)&header)+sizeof(header);
  // Make sure we havn't actually already read the end of the bash-execution header
  while (iter != end) {
   if (*iter++ == '\n') {
    char *unread_start;
    Dee_size_t bytes_after = ((Dee_size_t)(end-iter))*sizeof(char);
    memmove(&header,iter,bytes_after);
    unread_start = ((char *)&header)+bytes_after;
    bytes_after = ((Dee_size_t)(end-unread_start))*sizeof(char);
    if (DeeFile_ReadAll(file,unread_start,bytes_after) != 0) return -1;
    temp = _DeeMarshal_DoIsCompiledFile(file,&header,deemon_version,marshal_version);
    total_read += bytes_after;
    goto after_check2;
   }
  }
  // NOPE! --> Skip forward until we read it
  do { // Skip unix-style exec script linkage
   if (DeeFile_Read(file,&ch,1,&did_read) != 0) return -1;
   if (!did_read) goto not_compiled;
   ++total_read;
  } while (ch != '\n');
  // v error or actually found
  temp = _DeeMarshal_IsCompiledFile(file,&header,deemon_version,marshal_version);
  if (temp == 2) return 0; // File too small
  // v read by '_DeeMarshal_IsCompiledFile'
  total_read += sizeof(header);
after_check2:
  if (temp < 0) return -1;
  if (temp) {
found:
   // v101 doesn't implement backwards compatibility
   if (DEE_VERSION_GET_API(*deemon_version) < 101) {
    DeeError_SetStringf(&DeeErrorType_NotImplemented,
     "No backwards compatibility for pre-v101 version %d/%d:%d",
     (int)DEE_VERSION_GET_API(*deemon_version),
     (int)DEE_VERSION_GET_COMPILER(*deemon_version),
     (int)DEE_VERSION_GET_REVISION(*deemon_version));
    return -1;
   }
   return 1;
  }
not_compiled:
  return DeeFile_Seek(file,-(Dee_int64_t)total_read,DEE_SEEK_CUR,NULL);
 }
 return DeeFile_Seek(file,-(Dee_int64_t)sizeof(header),DEE_SEEK_CUR,NULL);
}

// ...It's a kind of magic 

















//////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------- //
//////////////////////////////////////////////////////////////////////////

struct selfref_data {
 unsigned int visit_id; /*< gc visit id that we are using to  */
 DeeObject   *ob;       /*< [0..1] the object we are checking. (set to NULL if it does reference itself) */
};
static int _deegc_visit_is_visible(
 DeeObject *self, struct selfref_data *data) {
 if (!data->ob) return 0; // Already found
 if (data->ob == self) {
  data->ob = NULL; // Found it
  return 0;
 } else if (DeeObject_IS_GC(self)) {
  struct _DeeGCHead *head = DeeGC_OB2HEAD(self);
  if (head->gc_last_seen != data->visit_id) {
   head->gc_last_seen = data->visit_id;
   return 1;
  }
  return 0;
 }
 return 1;
}
DEE_STATIC_INLINE(int) DeeMarshal_IsRecursiveObject(
 DeeTypeObject const *tp_self, DeeObject *self) {
 DeeObject *gc_iter,*ob_next; DeeTypeObject const *tp_iter;
 struct selfref_data data;
 if (!DeeObject_IS_GC(self)) return 0; // Only gc object can reference themselves
 data.ob = gc_iter = self;
 DeeAtomicMutex_AcquireRelaxed(&_dee_gc_collect_lock);
 data.visit_id = _dee_gc_next_visit_id++;
 ob_next = (DeeObject *)(tp_iter = tp_self);
 while (1) {
  while (1) {
   if (DeeType_HAS_SLOT(tp_iter,tp_visit))
    (*DeeType_GET_SLOT(tp_iter,tp_visit))((DeeTypeObject *)tp_iter,
    gc_iter,(DeeVisitProc)&_deegc_visit_is_visible,&data);
   if ((tp_iter = DeeType_BASE(tp_iter)) == NULL) break;
  }
  if (!data.ob) break; // Found it
  if (gc_iter == ob_next) break; // nothing left to check...
  gc_iter = ob_next;
  ob_next = (DeeObject *)(tp_iter = Dee_TYPE(gc_iter));
 }
 DeeAtomicMutex_Release(&_dee_gc_collect_lock);
 // data.ob is set to NULL if it is referencing itself
 return data.ob == NULL;
}

//////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------------------- //
//////////////////////////////////////////////////////////////////////////

static DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeMarshal_TDoWriteObjectWithMap(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalWriteMap *file_map,
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeTypeObject const *marshal_typelevel; struct DeeTypeMarshal *type_marshal;
 DEE_ASSERT(DeeObject_Check(file) && DeeFile_Check(file));
 DEE_ASSERT(file_map);
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));

 marshal_typelevel = tp_self;
 // Remember that 'object' always implements everything...
 while ((type_marshal = DeeType_GET_SLOT(marshal_typelevel,tp_marshal)) == NULL)
  marshal_typelevel = DeeType_BASE(marshal_typelevel);

 if (DeeUUID_CHECK_INTERNAL(&type_marshal->tp_uuid)) {
  Dee_uint8_t internal_id; Dee_uint16_t builtin_id;
  internal_id = DeeUUID_GET_INTERNAL(&type_marshal->tp_uuid);
  DEE_ASSERTF(internal_id != 0xFF,"Not a valid internal id");
  DEE_ASSERTF(internal_id < DEE_MARSHALTYPEIDMIN || type_marshal->tp_marshal_put,
              "Invalid marshal put operator (intrinsic types must implement this operator)");
  if (tp_self == Dee_TYPE(self) &&
     (builtin_id = DeeBuiltin_Id(self)) != (Dee_uint16_t)-1) {
   // Reference a builtin object
   struct DeeMarshalHeaderBuiltin block;
   block.mhb_marker = DEE_MARSHALID_BUILTIN;
   block.mhb_block.mhb_id = DEE_BUILTIN_LESWAP16(builtin_id);
   return DeeFile_WriteAll(file,&block,sizeof(block));
  }
  switch (internal_id) {
   // This can happen for unregistered types
   case 0: break;

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   case DEE_MARSHALID_POINTERTYPE: {
    if (DeeFile_Putc(file,DEE_MARSHALID_POINTERTYPE) != 0) return -1;
    return DeeMarshal_WriteObjectWithMap(file,file_map,(DeeObject *)DeePointerType_BASE(self));
   } break;

   case DEE_MARSHALID_LVALUETYPE: {
    if (DeeFile_Putc(file,DEE_MARSHALID_LVALUETYPE) != 0) return -1;
    return DeeMarshal_WriteObjectWithMap(file,file_map,(DeeObject *)DeeLValueType_BASE(self));
   } break;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */

#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
   case DEE_MARSHALID_FOREIGNFUNCTIONTYPE: {
    struct DeeMarshalHeaderForeignFunctionType block;
    block.mhfft_marker = DEE_MARSHALID_FOREIGNFUNCTIONTYPE;
    block.mhfft_block.mhfft_flags = DEE_BUILTIN_LESWAP16(DeeForeignFunctionType_FLAGS(self));
    if (DeeFile_WriteAll(file,&block,sizeof(block)) != 0) return -1;
    if (DeeFile_PutLeSmall64(file,(Dee_uint64_t)DeeForeignFunctionType_ARGTYC(self)) != 0) return -1;
    if (DeeMarshal_WriteObjectWithMap(file,file_map,(DeeObject *)DeeForeignFunctionType_RETURNTYPE(self)) != 0) return -1;
    return DeeMarshal_WriteObjectsWithMap(file,file_map,DeeForeignFunctionType_ARGTYC(self),
                                         (DeeObject *const *)DeeForeignFunctionType_ARGTYV(self));
   } break;
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */

#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
   case DEE_MARSHALID_ARRAYTYPE: {
    if (DeeFile_Putc(file,DEE_MARSHALID_ARRAYTYPE) != 0) return -1;
    if (DeeFile_PutLeSmall64(file,(Dee_uint64_t)DeeArrayType_NELEM(self)) != 0) return -1;
    return DeeMarshal_WriteObjectWithMap(file,file_map,(DeeObject *)DeeArrayType_BASE(self));
   } break;
   case DEE_MARSHALID_VARARRAYTYPE: {
    if (DeeFile_Putc(file,DEE_MARSHALID_VARARRAYTYPE) != 0) return -1;
    return DeeMarshal_WriteObjectWithMap(file,file_map,(DeeObject *)DeeVarArrayType_BASE(self));
   } break;
#endif /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */

#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
   case DEE_MARSHALID_CLASS: {
    struct DeeMarshalHeaderClass block;
    struct DeeTypeMarshal *self_class_marshal;
    char const *class_name; Dee_size_t class_name_len;
    DeeTypeObject *class_base;
    DEE_ASSERT(DeeClassType_Check(tp_self));
    DEE_ASSERT(DeeClass_Check(self));
    DEE_ASSERTF(DeeType_HAS_SLOT(self,tp_marshal),"All classes must implement tp_marshal");
    self_class_marshal = DeeType_GET_SLOT(self,tp_marshal);
    block.mhc_marker = DEE_MARSHALID_CLASS;
    DEE_ASSERTF(!DeeUUID_CHECK_INTERNAL(&self_class_marshal->tp_uuid) ||
                DeeUUID_GET_INTERNAL(&self_class_marshal->tp_uuid) == DEE_MARSHALID_INSTANCE,
                "Class object %q must either have a custom uuid, or "
                "use the internal 'DEE_MARSHALID_INSTANCE' protocol",DeeType_NAME(self));
    DEE_ASSERTF(self_class_marshal->tp_marshal_ctor,
                "Class object %q must implement 'tp_marshal_ctor'",DeeType_NAME(self));
    DEE_ASSERTF(self_class_marshal->tp_marshal_put,
                "Class object %q must implement 'tp_marshal_put'",DeeType_NAME(self));
    // Copy the class uuid into the block
    block.mhc_block.mhc_uuid = self_class_marshal->tp_uuid;
    class_name = DeeType_GET_SLOT(self,tp_name);
    class_name_len = class_name ? strlen(class_name) : 0;
    if (DeeFile_WriteAll(file,&block,sizeof(block)) != 0) return -1;
    if (DeeFile_PutLeSmall64(file,(Dee_uint64_t)class_name_len) != 0) return -1;
    if (class_name_len && DeeFile_WriteAll(file,class_name,class_name_len) != 0) return -1;
    class_base = DeeType_BASE(self);
    DEE_ASSERTF(DeeObject_Check(class_base) && DeeType_Check(class_base),
                "Class %q has an invalid base",class_name ? class_name : "?");
    // Write the class base
    if (DeeMarshal_WriteObjectWithMap(file,file_map,(DeeObject *)class_base) != 0) return -1;
    // Write the class declaration itself (slots and such...)
    DEE_ASSERTF(type_marshal->tp_marshal_put,
                "Class type %q must implement 'tp_marshal_put'",
                DeeType_NAME(tp_self));
    return (*type_marshal->tp_marshal_put)((DeeTypeObject *)tp_self,self,file,file_map);
   } break;
#endif /* DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES */

   case DEE_MARSHALID_INSTANCE: {
    DEE_ASSERTF(type_marshal->tp_marshal_ctor,
                "Marshal objects using the 'DEE_MARSHALID_INSTANCE' protocol "
                "must implement 'tp_marshal_ctor' / 'tp_marshal_new'");
    DEE_ASSERTF(type_marshal->tp_marshal_put,
                "Marshal objects using the 'DEE_MARSHALID_INSTANCE' protocol "
                "must implement 'tp_marshal_put'");
    if (DeeFile_Putc(file,DEE_MARSHALID_INSTANCE) != 0) return -1;
    if (DeeMarshal_WriteObjectWithMap(file,file_map,(DeeObject *)tp_self) != 0) return -1;
    return (*type_marshal->tp_marshal_put)((DeeTypeObject *)tp_self,self,file,file_map);
   } break;

   case DEE_MARSHALID_SHAREDLIB: {
    DeeObject *sl_name; int temp;
    DEE_ASSERT(DeeSharedLib_Check(self));
    if (DeeFile_Putc(file,DEE_MARSHALID_SHAREDLIB) != 0) return -1;
    if ((sl_name = DeeSharedLib_Name(self)) == NULL) return -1;
    temp = DeeMarshal_WriteObjectWithMap(file,file_map,sl_name);
    Dee_DECREF(sl_name);
    return temp;
   } break;

   case DEE_MARSHALID_DEXMODULE: {
    DeeObject *dex_name; int temp;
    DEE_ASSERT(DeeSharedLib_Check(self));
    if (DeeFile_Putc(file,DEE_MARSHALID_DEXMODULE) != 0) return -1;
    if ((dex_name = DeeDexModule_Name(self)) == NULL) return -1;
    temp = DeeMarshal_WriteObjectWithMap(file,file_map,dex_name);
    Dee_DECREF(dex_name);
    return temp;
   } break;

   case DEE_MARSHALTYPEID_object: {
    if (tp_self == &DeeSuper_Type) { // Special handling for 'super': Write only the object referenced
     DEE_ASSERT(DeeSuper_Check(self));
     return DeeMarshal_TDoWriteObjectWithMap(file,file_map,DeeSuper_TYPE(self),DeeSuper_SELF(self));
    }
    if (tp_self == &DeeBool_Type) { // Special handling for copies of true/false: Write the original bool object
     DEE_ASSERT(DeeBool_Check(self));
     DEE_ASSERT(self != Dee_True && self != Dee_False);
     return DeeMarshal_TDoWriteObjectWithMap(file,file_map,&DeeBool_Type,
                                             DeeBool_Const(DeeBool_VALUE(self)));
    }
    DEE_ATTRIBUTE_FALLTHROUGH
   }

   default: {
    DEE_ASSERTF(internal_id >= DEE_MARSHALTYPEIDMIN,
                "The internal type id doesn't describe a type");
    if (DeeFile_Putc(file,internal_id) != 0) return -1;
    return (*type_marshal->tp_marshal_put)((DeeTypeObject *)tp_self,self,file,file_map);
   } break;
  }
 }

 if (DeeType_IsSameOrDerived(tp_self,&DeeType_Type)) {
  struct DeeTypeMarshal *self_marshal;
  DEE_ASSERT(DeeType_Check(self));
  // Types may be referencing known marshal types
  self_marshal = DeeType_GET_SLOT(self,tp_marshal);
  if DEE_UNLIKELY(!self_marshal) {
   DeeError_SetStringf(&DeeErrorType_NotImplemented,
                       "Can't write type %q: Type doesn't support marshaling",
                       DeeType_NAME(self));
   return -1;
  }
  DEE_ASSERT(self_marshal);
  if ((file_map->mwm_flags&DEE_MARSHAL_WRITEFLAG_UUID_REFS)!=0) {
   struct DeeMarshalHeaderMarshalType block;
   // Register the type now, if necessary (unlikely that this has to be done...)
   if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_REGISTERED_MARSHAL)==0 &&
       DeeMarshal_RegisterType((DeeTypeObject *)self) < 0) return -1;
   block.mhmt_marker = DEE_MARSHALID_MARSHALTYPE;
   block.mhmt_block.mhmt_uuid = self_marshal->tp_uuid;
   return DeeFile_WriteAll(file,&block,sizeof(block));
  }
 }

 // Fallback case: Invoke user-specific marshal code from 'type_marshal'
 {
  DeeObject *buffered_writer; Dee_size_t block_size;
  struct DeeMarshalHeaderUser block;
  DEE_ASSERT(type_marshal->tp_marshal_put);
  if DEE_UNLIKELY((buffered_writer = DeeFileWriter_New()) == NULL) return -1;
  if DEE_UNLIKELY((*type_marshal->tp_marshal_put)((
   DeeTypeObject *)tp_self,self,buffered_writer,file_map) < 0) {
err_buffered_writer: Dee_DECREF(buffered_writer); return -1;
  }
  block.mhu_marker = DEE_MARSHALID_NONINTRINSIC;
  block.mhu_block.mhu_unused = 0;
  block.mhu_block.mhu_uuid = type_marshal->tp_uuid;
  block.mhu_block.mhu_flags = DEE_MASHALHEADER_USER_FLAGS_NONE;
#if defined(DEE_PLATFORM_BIG_ENDIAN) && 0
  block.mhu_unused = DEE_BUILTIN_LESWAP32(block.mhu_unused);
#endif
  if DEE_UNLIKELY(DeeFile_WriteAll(file,&block,sizeof(block)) != 0) goto err_buffered_writer;
  block_size = DeeStringWriter_SIZE(DeeFileWriter_WRITER(buffered_writer));
  if DEE_UNLIKELY(DeeFile_PutLeSmall64(file,(Dee_uint64_t)block_size) != 0) goto err_buffered_writer;
  if DEE_UNLIKELY(DeeFile_WriteAll(file,DeeStringWriter_ELEM(DeeFileWriter_WRITER(
   buffered_writer)),block_size) != 0) goto err_buffered_writer;
  Dee_DECREF(buffered_writer);
 }
 return 0;
}

#ifdef DEE_DEBUG
#define DEE_MARSHALERROR_FILEINFO_PRINTF     "[%k] "
#define DEE_MARSHALERROR_FILEINFO_ARGS(file) file,
#else
#define DEE_MARSHALERROR_FILEINFO_PRINTF     /* nothing */
#define DEE_MARSHALERROR_FILEINFO_ARGS(file) /* nothing */
#endif


DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeObject *) DeeMarshal_DoReadObjectWithMap(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalReadMap *file_map,
 DEE_A_IN Dee_uint8_t marker, DEE_A_IN PDeeMashalNewAfterAlloc after_alloc, void *after_alloc_data) {
 DeeObject *result; DeeTypeObject *loadtype; struct DeeTypeMarshal *type_marshal;
 switch (marker) {

  // Reference declaration is illegal at this point
  case DEE_MARSHALID_PUTREF8:
  case DEE_MARSHALID_PUTREF16:
  case DEE_MARSHALID_PUTREF32: {
   DeeError_SET_STRING(&DeeErrorType_ValueError,
                       "Using the marshal to create a reference of a reference is illegal "
                       "(DEE_MARSHALID_PUTREF[8|16|32] may not be followed by another DEE_MARSHALID_PUTREF[8|16|32])");
   return NULL;
  } break;

  { // Reference lookup
   Dee_uint32_t refid;
   if (0) { Dee_uint8_t temp;  case DEE_MARSHALID_GETREF8:  if (DeeFile_GetLe(file,temp) != 0) return NULL;  refid = (Dee_uint32_t)temp; }
   if (0) { Dee_uint16_t temp; case DEE_MARSHALID_GETREF16: if (DeeFile_GetLe(file,temp) != 0) return NULL;  refid = (Dee_uint32_t)DEE_BUILTIN_LESWAP16(temp); }
   if (0) {                    case DEE_MARSHALID_GETREF32: if (DeeFile_GetLe(file,refid) != 0) return NULL; refid = DEE_BUILTIN_LESWAP32(refid); }
   result = _DeeMarshalReadMap_GetRef(file_map,refid);
   if DEE_UNLIKELY(!result) {
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        DEE_MARSHALERROR_FILEINFO_PRINTF "Reference id 0x%I32X(%I32u) was never defined in the marshal map",
                        DEE_MARSHALERROR_FILEINFO_ARGS(file) refid,refid,file);
    return NULL;
   }
   (*after_alloc)(result,after_alloc_data);
   DeeReturn_NEWREF(result);
  } break;

  case DEE_MARSHALID_NONE: {
   result = DeeNone_New();
   (*after_alloc)(result,after_alloc_data);
   return result;
  } break;

  case DEE_MARSHALID_BUILTIN: {
   Dee_uint16_t builtin_id;
   if (DeeFile_GetLe(file,builtin_id) != 0) return NULL;
   if DEE_UNLIKELY((result = DeeBuiltin_Get(builtin_id)) == NULL) {
    DeeError_SetStringf(&DeeErrorType_NotImplemented,
                        DEE_MARSHALERROR_FILEINFO_PRINTF "Unknown/Invalid builtin object object: %I16u",
                        DEE_MARSHALERROR_FILEINFO_ARGS(file) builtin_id);
    return NULL;
   }
   Dee_INCREF(result);
   (*after_alloc)(result,after_alloc_data);
   return result;
  } break;

  case DEE_MARSHALID_POINTERTYPE: {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   DeeTypeObject *ptr_base;
   if ((ptr_base = (DeeTypeObject *)DeeMarshal_ReadObjectWithMap(file,file_map)) == NULL) return NULL;
   result = (DeeObject *)DeeType_Pointer((DeeTypeObject *)ptr_base);
   Dee_DECREF(ptr_base);
   if (result) { Dee_INCREF(result); (*after_alloc)(result,after_alloc_data); }
   return result;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   DeeError_NotImplemented_str("DEE_CONFIG_RUNTIME_HAVE_POINTERS");
   return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  } break;

  case DEE_MARSHALID_LVALUETYPE: {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   DeeTypeObject *ptr_base;
   if ((ptr_base = (DeeTypeObject *)DeeMarshal_ReadObjectWithMap(file,file_map)) == NULL) return NULL;
   result = (DeeObject *)DeeType_LValue((DeeTypeObject *)ptr_base);
   if (result) { Dee_INCREF(result); (*after_alloc)(result,after_alloc_data); }
   Dee_DECREF(ptr_base);
   return result;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   DeeError_NotImplemented_str("DEE_CONFIG_RUNTIME_HAVE_POINTERS");
   return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  } break;

  case DEE_MARSHALID_FOREIGNFUNCTIONTYPE: {
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
   DeeTypeObject *ff_base,**ff_typev,**ff_iter,**ff_end; Dee_size_t ff_typec;
   struct DeeMarshalHeaderForeignFunctionType_ block; Dee_uint64_t argc;
   if (DeeFile_ReadAll(file,&block,sizeof(block)) != 0) return NULL;
#ifndef DEE_PLATFORM_LIL_ENDIAN
   block.mhfft_flags = DEE_BUILTIN_BESWAP16(block.mhfft_flags);
#endif
   if (DeeFile_GetLeSmall64(file,&argc) != 0) return NULL;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
   if DEE_UNLIKELY(argc > (Dee_uint64_t)((Dee_size_t)-1)) {
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        DEE_MARSHALERROR_FILEINFO_PRINTF "Foreign function argument count is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                        DEE_MARSHALERROR_FILEINFO_ARGS(file) argc,(Dee_size_t)-1);
    return NULL;
   }
#endif
   ff_typec = (Dee_size_t)argc;
   if ((ff_base = (DeeTypeObject *)DeeMarshal_ReadObjectWithMap(file,file_map)) == NULL) return NULL;
   if (ff_typec) {
    while ((ff_typev = (DeeTypeObject **)malloc_nz(
     ff_typec*sizeof(DeeTypeObject *))) == NULL) {
     if DEE_LIKELY(Dee_CollectMemory()) continue;
     Dee_DECREF(ff_base);
     DeeError_NoMemory();
     return NULL;
    }
    if (DeeMarshal_ReadObjectsWithMap(file,file_map,
     ff_typec,(DeeObject **)ff_typev) != 0) {
     free_nn(ff_typev);
     Dee_DECREF(ff_base);
     return NULL;
    }
   } else ff_typev = NULL;
   result = (DeeObject *)DeeType_ForeignFunction(ff_base,block.mhfft_flags,
                                                 ff_typec,(DeeTypeObject const *const *)ff_typev);
   ff_end = (ff_iter = ff_typev)+ff_typec;
   while (ff_iter != ff_end) Dee_DECREF(*ff_iter++);
   free(ff_typev);
   Dee_DECREF(ff_base);
   if (result) { Dee_INCREF(result); (*after_alloc)(result,after_alloc_data); }
   return result;
#else /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
   DeeError_NotImplemented_str("DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION");
   return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
  } break;

  case DEE_MARSHALID_ARRAYTYPE: {
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
   DeeTypeObject *array_base; Dee_uint64_t array_size;
   if (DeeFile_GetLeSmall64(file,&array_size) != 0) return NULL;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
   if DEE_UNLIKELY(array_size > (Dee_uint64_t)((Dee_size_t)-1)) {
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        DEE_MARSHALERROR_FILEINFO_PRINTF "Array size is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                        DEE_MARSHALERROR_FILEINFO_ARGS(file) array_size,(Dee_size_t)-1);
    return NULL;
   }
#endif
   if ((array_base = (DeeTypeObject *)DeeMarshal_ReadObjectWithMap(file,file_map)) == NULL) return NULL;
   result = (DeeObject *)DeeType_Array(array_base,(Dee_size_t)array_size);
   Dee_DECREF(array_base);
   if (result) { Dee_INCREF(result); (*after_alloc)(result,after_alloc_data); }
   return result;
#else /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
   DeeError_NotImplemented_str("DEE_CONFIG_RUNTIME_HAVE_ARRAYS");
   return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
  } break;

  case DEE_MARSHALID_VARARRAYTYPE: {
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
   DeeTypeObject *array_base;
   if ((array_base = (DeeTypeObject *)DeeMarshal_ReadObjectWithMap(file,file_map)) == NULL) return NULL;
   result = (DeeObject *)DeeType_VArray(array_base);
   Dee_DECREF(array_base);
   if (result) { Dee_INCREF(result); (*after_alloc)(result,after_alloc_data); }
   return result;
#else /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
   DeeError_NotImplemented_str("DEE_CONFIG_RUNTIME_HAVE_ARRAYS");
   return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
  } break;

  case DEE_MARSHALID_CLASS: {
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
   struct DeeMarshalHeaderClass_ block; Dee_uint64_t name_size;
   char *class_name; DeeTypeObject *class_base;
   if (DeeFile_ReadAll(file,&block,sizeof(block)) != 0) return NULL;
   if (DeeFile_GetLeSmall64(file,&name_size) != 0) return NULL;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
   if DEE_UNLIKELY(name_size > (Dee_uint64_t)((Dee_size_t)-1)) {
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        DEE_MARSHALERROR_FILEINFO_PRINTF "Class name is too long for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                        DEE_MARSHALERROR_FILEINFO_ARGS(file) name_size,(Dee_size_t)-1);
    return NULL;
   }
#endif
   if (name_size) {
    Dee_size_t used_name_len;
    used_name_len = (Dee_size_t)name_size;
    while ((class_name = (char *)malloc_nz(
     (used_name_len+1)*sizeof(char))) == NULL) {
     if DEE_LIKELY(Dee_CollectMemory()) continue;
     DeeError_NoMemory();
     return NULL;
    }
    if (DeeFile_ReadAll(file,class_name,used_name_len) != 0) {
err_class_name: free(class_name); return NULL;
    }
    class_name[used_name_len] = 0;
   } else class_name = NULL;
   if ((class_base = (DeeTypeObject *)DeeMarshal_ReadObjectWithMap(file,file_map)) == NULL) goto err_class_name;
   if (DeeError_TypeError_CheckType((DeeObject *)class_base,&DeeType_Type) != 0) {
/*err_class_base:*/ Dee_DECREF(class_base); goto err_class_name;
   }
   // Create the class object
   // NOTE: If a uuid was set, this function will also register
   //       it in the marshal's global uuid type-lookup cache.
   result = (DeeObject *)DeeClass_NewEx(class_base,class_name,
                                        DeeMarshalHeaderClass_HAS_UUID(&block)
                                        ? &block.mhc_uuid : NULL);
   Dee_DECREF(class_base);
   free(class_name);
   if (!result) return NULL;
   // Do some assertions to make sure that we got what we wanted,
   // that being an actual class, who's type is a class-type.
   DEE_ASSERT(DeeObject_Check(result));
   DEE_ASSERT(DeeClass_Check(result));
   DEE_ASSERT(DeeType_GET_SLOT(result,tp_marshal));
   DEE_ASSERT(DeeUUID_CHECK_INTERNAL(&DeeType_GET_SLOT(Dee_TYPE(result),tp_marshal)->tp_uuid));
   DEE_ASSERT(DeeUUID_GET_INTERNAL(&DeeType_GET_SLOT(Dee_TYPE(result),tp_marshal)->tp_uuid) == DEE_MARSHALID_CLASS);
   DEE_ASSERT(DeeType_GET_SLOT(Dee_TYPE(result),tp_marshal)->tp_marshal_ctor);
   DEE_ASSERT(DeeType_GET_SLOT(Dee_TYPE(result),tp_marshal)->tp_marshal_put);
   (*after_alloc)(result,after_alloc_data);
   // Read the rest of the class declaration (slots and such...)
   if ((*DeeType_GET_SLOT(Dee_TYPE(result),tp_marshal)->tp_marshal_ctor)(
    Dee_TYPE(result),(DeeObject *)result,file,file_map) != 0) {
    Dee_CLEAR(result);
   }
   return result;
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES */
   DeeError_NotImplemented_str("DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES");
   return NULL;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
  } break;

  case DEE_MARSHALID_INSTANCE: {
   DeeTypeObject *load_marshal_type;
   if ((loadtype = (DeeTypeObject *)DeeMarshal_ReadObjectWithMap(file,file_map)) == NULL) return NULL;
   if (DeeError_TypeError_CheckType((DeeObject *)loadtype,&DeeType_Type) != 0) {
err_load_type: Dee_DECREF(loadtype); return NULL;
   }
   load_marshal_type = loadtype;
   while ((type_marshal = DeeType_GET_SLOT(load_marshal_type,tp_marshal)) == NULL)
    load_marshal_type = DeeType_BASE(load_marshal_type);
#if 0
   if (load_marshal_type == &DeeObject_Type) {
    DeeError_SetStringf(&DeeErrorType_NotImplemented,
                        "Can't load instance of %q: Type doesn't support marshaling",
                        DeeType_NAME(loadtype));
    goto err_load_type;
   }
#endif
   if ((DeeType_FLAGS(loadtype)&DEE_TYPE_FLAG_VAR_OBJECT)!=0) {
    DEE_ASSERT(type_marshal->tp_marshal_new);
    result = (*type_marshal->tp_marshal_new)(loadtype,file,file_map,after_alloc,after_alloc_data);
   } else {
    DEE_ASSERT(type_marshal->tp_marshal_ctor);
    // Allocate a new instance
    result = (*DeeType_GET_SLOT(loadtype,tp_alloc))(loadtype);
    if (!result) goto err_load_type;
    DEE_ASSERTF(DeeObject_Check(result) && DeeObject_InstanceOf(result,loadtype),
                "'%#q.tp_alloc' returned an invalid object",DeeType_NAME(loadtype));
    // Register it with the supplied callback
    (*after_alloc)(result,after_alloc_data);
    if ((*type_marshal->tp_marshal_ctor)(loadtype,result,file,file_map) != 0) {
     // Error during loading
     _DeeObject_DELETE(loadtype,result);
     goto err_load_type;
    }
   }
   Dee_DECREF(loadtype);
   return result;
  } break;

  case DEE_MARSHALID_MARSHALTYPE: {
   struct DeeMarshalHeaderMarshalType_ block;
   if (DeeFile_ReadAll(file,&block,sizeof(block)) != 0) return NULL;
   result = (DeeObject *)DeeMarshal_LookupType(&block.mhmt_uuid);
   if (!result) {
    DeeError_SetStringf(&DeeErrorType_NotImplemented,
                        DEE_MARSHALERROR_FILEINFO_PRINTF "Unknown uuid when trying to load a marshal type: %K",
                        DEE_MARSHALERROR_FILEINFO_ARGS(file) DeeUUID_ToString(&block.mhmt_uuid));
    return NULL;
   }
   return result;
  } break;

  case DEE_MARSHALID_SHAREDLIB: {
   DeeObject *filename;
   if ((filename = DeeMarshal_ReadObjectWithMap(file,file_map)) == NULL) return NULL;
   result = DeeSharedLib_PoolLoadObject(filename);
   Dee_DECREF(filename);
   if (!result) return NULL;
   (*after_alloc)(result,after_alloc_data);
   return result;
  } break;

  case DEE_MARSHALID_DEXMODULE: {
   DeeObject *dexname;
   if ((dexname = DeeMarshal_ReadObjectWithMap(file,file_map)) == NULL) return NULL;
   if (DeeString_InplaceCast((DeeObject const **)&dexname) != 0) { Dee_DECREF(dexname); return NULL; }
   result = DeeDex_Open(DeeString_STR(dexname));
   Dee_DECREF(dexname);
   if (!result) return NULL;
   (*after_alloc)(result,after_alloc_data);
   return result;
  } break;

  case DEE_MARSHALID_IGNORE: {
   DeeObject *ignored;
   if ((ignored = DeeMarshal_ReadObjectWithMap(file,file_map)) == NULL) return NULL;
   result = DeeMarshal_ReadObjectWithMap(file,file_map);
   Dee_DECREF(ignored);
   if (!result) return NULL;
   (*after_alloc)(result,after_alloc_data);
   return result;
  } break;

  case DEE_MARSHALID_NONINTRINSIC: {
   struct DeeMarshalHeaderUser_ block; Dee_uint64_t block_size;
   if (DeeFile_ReadAll(file,&block,sizeof(block)) != 0) return NULL;
   if (DeeFile_GetLeSmall64(file,&block_size) != 0) return NULL;
   loadtype = DeeMarshal_LookupType(&block.mhu_uuid);
#if defined(DEE_PLATFORM_BIG_ENDIAN) && 0
   block.mhu_unused = DEE_BUILTIN_LESWAP32(block.mhu_unused);
#endif
   if (!loadtype) {
    if ((block.mhu_flags&DEE_MASHALHEADER_USER_FLAGS_IGNORE)!=0) {
ignore_return_none:
     // Skip the block if it should be ignored
     while DEE_UNLIKELY((block_size&DEE_UINT64_C(0x8000000000000000))!=0) {
      // Prevent an integer overflow
      if DEE_UNLIKELY(DeeFile_Seek(file,DEE_INT64_C(0x7FFFFFFFFFFFFFFF),DEE_SEEK_CUR,NULL) != 0) return NULL;
      block_size -= DEE_UINT64_C(0x7FFFFFFFFFFFFFFF);
     }
     if (DeeFile_Seek(file,*(Dee_int64_t*)&block_size,DEE_SEEK_CUR,NULL) != 0) return NULL;
     (*after_alloc)(Dee_None,after_alloc_data);
     DeeReturn_None;
    }
    DeeError_SetStringf(&DeeErrorType_NotImplemented,
                        DEE_MARSHALERROR_FILEINFO_PRINTF "Can't load block (size: %I64u) with unknown uuid %K",
                        DEE_MARSHALERROR_FILEINFO_ARGS(file) block_size,DeeUUID_ToString(&block.mhu_uuid));
    return NULL;
   } else {
    type_marshal = DeeType_GET_SLOT(loadtype,tp_marshal);
    if (!type_marshal) {
     if ((block.mhu_flags&DEE_MASHALHEADER_USER_FLAGS_IGNORE)!=0) goto ignore_return_none;
     DeeError_SetStringf(&DeeErrorType_NotImplemented,
                         DEE_MARSHALERROR_FILEINFO_PRINTF "Can't load instance of %q (size: %I64u): Type doesn't support marshaling",
                         DEE_MARSHALERROR_FILEINFO_ARGS(file) DeeType_NAME(loadtype),block_size);
     Dee_DECREF(loadtype);
     return NULL;
    }
load_from_type_marshal:
    if ((DeeType_FLAGS(loadtype)&DEE_TYPE_FLAG_VAR_OBJECT)!=0) {
     DEE_ASSERT(type_marshal->tp_marshal_new);
     file_map->mrm_uuid = &block.mhu_uuid;
     result = (*type_marshal->tp_marshal_new)(loadtype,file,file_map,after_alloc,after_alloc_data);
     file_map->mrm_uuid = NULL;
    } else {
     DEE_ASSERT(type_marshal->tp_marshal_ctor);
     // Allocate a new instance
     result = (*DeeType_GET_SLOT(loadtype,tp_alloc))(loadtype);
     if (!result) { Dee_DECREF(loadtype); return NULL; }
     DEE_ASSERTF(DeeObject_Check(result) && DeeObject_InstanceOf(result,loadtype),
                 "'%#q.tp_alloc' returned an invalid object",DeeType_NAME(loadtype));
     // Register it with the supplied callback
     (*after_alloc)(result,after_alloc_data);
     file_map->mrm_uuid = &block.mhu_uuid;
     if ((*type_marshal->tp_marshal_ctor)(loadtype,result,file,file_map) != 0) {
      // Error during loading
      _DeeObject_DELETE(loadtype,result);
      result = NULL;
     }
     file_map->mrm_uuid = NULL;
    }
    Dee_DECREF(loadtype);
    return result;
   }
  } break;

  default:
   loadtype = DeeMarshal_LookupIntrinsicType(marker);
   if DEE_LIKELY(loadtype) {
    type_marshal = DeeType_GET_SLOT(loadtype,tp_marshal);
    DEE_ASSERTF(type_marshal,"Intrinsic marshal type %q doesn't support marshalling",
                DeeType_NAME(loadtype));
    Dee_INCREF(loadtype);
    goto load_from_type_marshal;
   }
   break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     DEE_MARSHALERROR_FILEINFO_PRINTF "Unsupported/Unknown type marker: %#.2I8x(%I8u)",
                     DEE_MARSHALERROR_FILEINFO_ARGS(file) marker,marker);
 return NULL;
}




DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeMarshlAfterAllocPutRefData {
 struct DeeMarshalReadMap *aaprd_map; /*< [1..1] File map to insert the reference into. */
 DeeObject                *aaprd_object; /*< [0..1] The object that was pushed (NULL if failed). */
#ifdef DEE_DEBUG
 Dee_uint8_t               aaprd_failed; /*< Used in debug mode to ensure integrity if the callback was never executed. */
#endif
union{
 Dee_uint8_t               aaprd_id8;  /*< Reference id that should be used. */
 Dee_uint16_t              aaprd_id16; /*< Reference id that should be used. */
 Dee_uint32_t              aaprd_id32; /*< Reference id that should be used. */
};
};
DEE_COMPILER_MSVC_WARNING_POP

static void DEE_CALL DeeMarshalAfterAllocPutRef(
 DeeObject *allocated_object,
 struct DeeMarshlAfterAllocPutRefData *data) {
 DEE_ASSERT(allocated_object);
 DEE_ASSERT(data);
 DEE_ASSERTF(!data->aaprd_object,"Object was already registered");
#ifdef DEE_DEBUG
 DEE_ASSERTF(!data->aaprd_failed,"Object was already registered (called failed last time)");
#endif
 if (_DeeMarshalReadMap_TryPushRefWithoutIncref(data->aaprd_map,data->aaprd_id32,allocated_object) < 0) {
#ifdef DEE_DEBUG
  data->aaprd_failed = 1;
#endif
 } else {
  data->aaprd_object = allocated_object;
  DEE_LVERBOSE2("Defining marshal ref-object: %I32u as instance of %q\n",
                data->aaprd_id32,DeeType_NAME(Dee_TYPE(allocated_object)));
 }
}
static void DEE_CALL DeeMarshalAfterAllocNoop(
 DeeObject *DEE_UNUSED(allocated_object), void *DEE_UNUSED(data)) {}

DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeMarshal_ReadObjectWithMap(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file,
 DEE_A_INOUT struct DeeMarshalReadMap *file_map) {
 PDeeMashalNewAfterAlloc after_alloc; DeeObject *result;
 struct DeeMarshlAfterAllocPutRefData putref_data;
 Dee_uint8_t marker;
 if (DeeFile_Getc(file,&marker) != 0) return NULL;
 switch (marker) {
  {
   if (0) { case DEE_MARSHALID_PUTREF8:  if (DeeFile_GetLe(file,putref_data.aaprd_id8)  != 0) return NULL; putref_data.aaprd_id32 = putref_data.aaprd_id8; }
   if (0) { case DEE_MARSHALID_PUTREF16: if (DeeFile_GetLe(file,putref_data.aaprd_id16) != 0) return NULL; putref_data.aaprd_id32 = putref_data.aaprd_id16; }
   if (0) { case DEE_MARSHALID_PUTREF32: if (DeeFile_GetLe(file,putref_data.aaprd_id32) != 0) return NULL; }
   after_alloc = (void(DEE_CALL*)(DeeObject*,void*))&DeeMarshalAfterAllocPutRef;
   putref_data.aaprd_map = file_map;
   putref_data.aaprd_object = NULL;
#ifdef DEE_DEBUG
   putref_data.aaprd_failed = 0;
#endif
   // Read a new marker
   if (DeeFile_Getc(file,&marker) != 0) return NULL;
  } break;
  default: after_alloc = &DeeMarshalAfterAllocNoop; break;
 }
 // NOTE: The fact that 'putref_data' can still be
 //       uninitialized is intentional
 result = DeeMarshal_DoReadObjectWithMap(file,file_map,marker,after_alloc,&putref_data);
 if (after_alloc == (void(DEE_CALL*)(DeeObject*,void*))&DeeMarshalAfterAllocPutRef) {
  DEE_ASSERT(putref_data.aaprd_map == file_map);
  if DEE_UNLIKELY(!result) {
   if (putref_data.aaprd_object) {
    // Must undo the previous registration (the read failed, but still registered an object)
    _DeeMarshalReadMap_PopRefWithoutDecref(file_map,putref_data.aaprd_object);
   }
  } else if (!putref_data.aaprd_object) {
#ifdef DEE_DEBUG
   DEE_ASSERTF(putref_data.aaprd_failed,
               "Callback code executing marker %I8u forgot to execute the after_alloc callback",
               marker);
#endif
   Dee_CLEAR(result);
  } else {
   // Here we finally add the reference still missing from "_DeeMarshalReadMap_TryPushRefWithoutIncref"
   DEE_ASSERTF(result == putref_data.aaprd_object,
               "Marshal returned different object (%p) than what was registered in after_alloc (%p)",
               result,putref_data.aaprd_object);
   Dee_INCREF(result);
  }
 }
 return result;
}



DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeMarshal_RegisterType(
 DEE_A_INOUT DeeTypeObject *marshal_type) {
 DeeType_SLOT_TYPE(tp_flags) flags;
 struct DeeTypeMarshal *marshal;
 DEE_ASSERT(DeeObject_Check(marshal_type) && DeeType_Check(marshal_type));
 if ((marshal = DeeType_GET_SLOT(marshal_type,tp_marshal)) == NULL) return 1;
 if (DeeUUID_CHECK_INTERNAL(&marshal->tp_uuid)) return 3; // Intrinsic support
 do {
  flags = DeeType_FLAGS(marshal_type);
  if ((flags&DEE_TYPE_FLAG_REGISTERED_MARSHAL)!=0) return 2; // already registered
 } while (!DeeAtomic32_CompareExchangeWeak(
  DeeType_GET_SLOT(marshal_type,tp_flags),
  flags,flags|DEE_TYPE_FLAG_REGISTERED_MARSHAL,
  memory_order_seq_cst,memory_order_seq_cst));
 if (_DeeMarshal_DoRegisterType(marshal_type) != 0) {
  DeeAtomic32_FetchAnd(DeeType_GET_SLOT(marshal_type,tp_flags),
                       ~(DEE_TYPE_FLAG_REGISTERED_MARSHAL),
                       memory_order_seq_cst);
  return -1;
 }
 return 0;
}







static struct DeeUUIDList _knowntypes_list = DeeUUIDList_INIT();
static struct DeeAtomicMutex _knowntypes_lock = DeeAtomicMutex_INIT();


void _DeeMarshal_ClearRegisteredTypes(void) {
 DeeTypeObject *dangling_type; struct DeeUUID uuid;
 while DEE_UNLIKELY((dangling_type = (DeeTypeObject *)
  DeeUUIDList_PopOne(&_knowntypes_list,&uuid)) != NULL) {
  if (DeeFile_StdPrintf(DEE_STDERR,
   "Cleaning up dangling marshal type %q (uuid: %K)\n",
   DeeType_NAME(dangling_type),DeeUUID_ToString(&uuid)
   ) != 0) DeeError_Print(NULL,1);
  Dee_DECREF(dangling_type);
 }
}

void DeeMarshal_UnregisterType(
 DEE_A_INOUT DeeTypeObject *marshal_type) {
 DeeObject *popped_type;
 struct DeeUUID const *alias_uuids;
 struct DeeTypeMarshal const *type_marshal;
 DeeType_SLOT_TYPE(tp_flags) flags;
 // 'DeeObject_Check' might fail when this is called from a destructor...
 DEE_ASSERT(/*DeeObject_Check(marshal_type) &&*/ DeeType_Check(marshal_type));
 do {
  flags = DeeType_FLAGS(marshal_type);
  if ((flags&DEE_TYPE_FLAG_REGISTERED_MARSHAL)==0) return; // Never registered
 } while DEE_UNLIKELY(!DeeAtomic32_CompareExchangeWeak(
  DeeType_GET_SLOT(marshal_type,tp_flags),
  flags,flags&~(DEE_TYPE_FLAG_REGISTERED_MARSHAL),
  memory_order_seq_cst,memory_order_seq_cst));
 DEE_ASSERT(DeeType_HAS_SLOT(marshal_type,tp_marshal));
 type_marshal = DeeType_GET_SLOT(marshal_type,tp_marshal);
 DeeAtomicMutex_AcquireRelaxed(&_knowntypes_lock);
 popped_type = DeeUUIDList_Del(&_knowntypes_list,&type_marshal->tp_uuid);
 //if (popped_type != (DeeObject *)marshal_type) {
 // // TODO: Warning?
 //}
 if DEE_UNLIKELY((alias_uuids = type_marshal->tp_alias_uuids) != NULL) {
  while (!DeeUUID_IS_EMPTY(alias_uuids)) {
   popped_type = DeeUUIDList_Del(&_knowntypes_list,alias_uuids);
   //if (popped_type != (DeeObject *)marshal_type) {
   // // TODO: Warning?
   //}
   ++alias_uuids;
  }
 }
 DeeAtomicMutex_Release(&_knowntypes_lock);
}

DEE_A_RET_NOEXCEPT_REF DeeTypeObject *DeeMarshal_LookupType(
 DEE_A_IN struct DeeUUID const *uuid) {
 DeeTypeObject *result;
 DEE_ASSERT(uuid);
 DeeAtomicMutex_AcquireRelaxed(&_knowntypes_lock);
 result = (DeeTypeObject *)DeeUUIDList_Lookup(&_knowntypes_list,uuid);
 Dee_XINCREF(result);
 DeeAtomicMutex_Release(&_knowntypes_lock);
 return result;
}
DEE_A_RET_EXCEPT(-1) int _DeeMarshal_DoRegisterType(
 DEE_A_INOUT DeeTypeObject *marshal_type) {
 int error; DeeTypeObject *other_type;
 struct DeeUUID const *alias_uuids;
 struct DeeTypeMarshal const *type_marshal;
 DEE_ASSERT(DeeObject_Check(marshal_type) && DeeType_Check(marshal_type));
 DEE_ASSERTF(DeeType_HAS_SLOT(marshal_type,tp_marshal),
             "Type %q doesn't support marshaling",
             DeeType_NAME(marshal_type));
 type_marshal = DeeType_GET_SLOT(marshal_type,tp_marshal);
again:
 DeeAtomicMutex_AcquireRelaxed(&_knowntypes_lock);
 error = DeeUUIDList_Add(&_knowntypes_list,
                         &type_marshal->tp_uuid,
                         (DeeObject *)marshal_type);
 if (error == DEE_UUIDLIST_ADD_ERROR_NOMEM) {
  DeeAtomicMutex_Release(&_knowntypes_lock);
  // Collect memory and try again
  if DEE_LIKELY(Dee_CollectMemory()) goto again;
  // Nothing collected --> out-of-memory
  DeeError_NoMemory();
  return -1;
 }
 if (error == DEE_UUIDLIST_ADD_ERROR_DUPLICATE) {
err_dupuuid:
  // Duplicate uuid
  other_type = (DeeTypeObject *)DeeUUIDList_Lookup(
   &_knowntypes_list,&DeeType_GET_SLOT(marshal_type,tp_marshal)->tp_uuid);
  DEE_ASSERT(DeeObject_Check(other_type) && DeeType_Check(other_type));
  Dee_INCREF(other_type);
  DeeAtomicMutex_Release(&_knowntypes_lock);
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Can't register %q, because uuid %K is already used by %q",
                      DeeType_NAME(marshal_type),
                      DeeUUID_ToString(&DeeType_GET_SLOT(marshal_type,tp_marshal)->tp_uuid),
                      DeeType_NAME(other_type));
  Dee_DECREF(other_type);
  return -1;
 }
 DEE_ASSERT(error == DEE_UUIDLIST_ADD_ERROR_SUCCESS);
 // Now to register all known aliases
 if DEE_UNLIKELY((alias_uuids = type_marshal->tp_alias_uuids) != NULL) {
  while (!DeeUUID_IS_EMPTY(alias_uuids)) {
again_alias:
   error = DeeUUIDList_Add(&_knowntypes_list,alias_uuids,
                           (DeeObject *)marshal_type);
   switch (error) {
    case DEE_UUIDLIST_ADD_ERROR_NOMEM: {
     DeeAtomicMutex_Release(&_knowntypes_lock);
     // Collect memory and try again
     if DEE_LIKELY(Dee_CollectMemory()) goto again_alias;
     // Nothing collected --> out-of-memory
     DeeError_NoMemory();
     DeeAtomicMutex_AcquireRelaxed(&_knowntypes_lock);
     return -1;
    } break;
    case DEE_UUIDLIST_ADD_ERROR_DUPLICATE: goto err_dupuuid;
    default: DEE_ASSERT(error == DEE_UUIDLIST_ADD_ERROR_SUCCESS); break;
   }
   ++alias_uuids;
  }
 }
 // everything OK!
 DeeAtomicMutex_Release(&_knowntypes_lock);
 return 0;
}

DEE_DECL_END

#ifndef __INTELLISENSE__
#define NEW_OBJECT
#include "marshal.write_object.base.inl"
#include "marshal.write_object.base.inl"
#endif

#endif /* !GUARD_DEEMON_MARSHAL_H */
