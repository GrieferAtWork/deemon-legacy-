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
#ifndef GUARD_DEEMON_STRUCT_C
#define GUARD_DEEMON_STRUCT_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/bool.h>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/gc.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/string.h>
#include <deemon/struct.h>
#include <deemon/tuple.h>
#include <deemon/optional/object_visit.h>

// /src/*
#include <deemon/marshal_data.h>
#include <deemon/signalhandler.h>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

struct DeeStructMember _DeeStructType_EmptyMembers[1] = {{NULL,NULL,0}};

DEE_A_RET_EXCEPT(-1) int DeeStructType_SetBase(
 DEE_A_INOUT_TYPEOBJECT(DeeStructTypeObject) *self, DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) *base) {
 DeeTypeObject *old_base; Dee_size_t offset; struct DeeStructMember *iter;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructType_Check(self));
 DEE_ASSERT(DeeObject_Check(base) && DeeStructuredType_Check(base));
 if (DeeType_IsFinal(base)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Can't use final type %q as base for struct",
                      DeeType_NAME(base));
  return -1;
 }
 if (!DeeType_IsComplete(base)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Can't use incomplete type %q as base for struct",
                      DeeType_NAME(base));
  return -1;
 }
 old_base = DeeType_BASE(self);
 if ((offset = DeeType_GET_SLOT(old_base,tp_p_instance_size)) != 0) {
  // Adjust offsets according to old base
  iter = ((DeeStructTypeObject *)self)->tp_smembers;
  DeeType_GET_SLOT(self,tp_p_instance_size) -= offset;
  while (iter->sm_name) iter->sm_offset -= offset;
 }
 DEE_ASSERT(DeeType_GET_SLOT(self,tp_base) == old_base);
 Dee_DECREF(old_base);
 Dee_INCREF(DeeType_GET_SLOT(self,tp_base) = (DeeTypeObject *)base);
 if ((offset = DeeType_GET_SLOT(base,tp_p_instance_size)) != 0) {
  // Adjust offsets according to new base
  iter = ((DeeStructTypeObject *)self)->tp_smembers;
  DeeType_GET_SLOT(self,tp_p_instance_size) += offset;
  while (iter->sm_name) iter->sm_offset += offset;
 }
 return 0;
}

DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeStructType_SetUUID(
 DEE_A_INOUT_TYPEOBJECT(DeeStructTypeObject) *self,
 DEE_A_IN struct DeeUUID const *uuid) {
 int temp; struct DeeTypeMarshal *marshal;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructType_Check(self));
 DEE_ASSERT(uuid);
 if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_REGISTERED_MARSHAL)!=0) return 1; // already registered
 while ((marshal = (struct DeeTypeMarshal *)malloc_nz(
  sizeof(struct DeeTypeMarshal))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
 }
 marshal->tp_uuid = *uuid;
 marshal->tp_marshal_ctor_ = (DeeType_SLOT_TYPE(tp_marshal_ctor))&_deestructured_tp_marshal_ctor;
 marshal->tp_marshal_put = (DeeType_SLOT_TYPE(tp_marshal_put))&_deestructured_tp_marshal_put;
 DEE_ASSERT(!DeeType_GET_SLOT(self,tp_marshal));
 DeeType_GET_SLOT(self,tp_marshal) = marshal;
 if ((temp = DeeMarshal_RegisterType(self)) != 0) {
  DeeType_GET_SLOT(self,tp_marshal) = NULL;
  free_nn(marshal);
  return temp;
 }
 return 0;
}


 
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
static ffi_type *_deeffi_empty_type[] = {NULL};
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
void DeeStructType_Changed(DEE_A_INOUT_TYPEOBJECT(DeeStructTypeObject) *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStructType_Check(self));
 (void)self;
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
 DEE_ASSERT(((DeeStructTypeObject *)self)->tp_ffitype.type == FFI_TYPE_STRUCT);
 ((DeeStructTypeObject *)self)->tp_ffitype.alignment = 1;
 ((DeeStructTypeObject *)self)->tp_ffitype.size = DeeType_GET_SLOT(self,tp_p_instance_size);
 ((DeeStructTypeObject *)self)->tp_ffitype.elements = _deeffi_empty_type;
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
}
DEE_A_RET_WUNUSED int DeeStructType_Initialized(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStructType_Check(self));
 return ((DeeStructTypeObject *)self)->tp_smembers != _DeeStructType_EmptyMembers;
}
DEE_A_RET_EXCEPT(-1) int DeeStructType_AddField(
 DEE_A_INOUT_TYPEOBJECT(DeeStructTypeObject) *self, DEE_A_INOUT_OBJECT(DeeStringObject) const *name,
 DEE_A_IN Dee_size_t offset, DEE_A_INOUT DeeTypeObject *type_) {
 Dee_size_t max_size; struct DeeStructMember *new_members; Dee_size_t member_count;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructType_Check(self));
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 DEE_ASSERT(DeeObject_Check(type_) && DeeStructuredType_Check(type_));
 DEE_ASSERT(!DeeStructType_HasField(self,name) && "Member name already exists");
 new_members = ((DeeStructTypeObject *)self)->tp_smembers;
 if (new_members != _DeeStructType_EmptyMembers) {
  member_count = 0;
  DEE_ASSERT(DeeObject_Check(new_members->sm_name));
  do ++member_count,++new_members; while (new_members->sm_type);
  while ((new_members = (struct DeeStructMember *)realloc_nz(
   ((DeeStructTypeObject *)self)->tp_smembers,(member_count+2)*
   sizeof(struct DeeStructMember))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
  ((DeeStructTypeObject *)self)->tp_smembers = new_members;
  new_members += member_count;
 } else {
  while ((new_members = (struct DeeStructMember *)malloc_nz(
   2*sizeof(struct DeeStructMember))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
  ((DeeStructTypeObject *)self)->tp_smembers = new_members;
 }
 Dee_INCREF(new_members->sm_name = (DeeStringObject *)name);
 Dee_INCREF(new_members->sm_type = (DeeStructuredTypeObject *)type_);
 offset += DeeType_GET_SLOT(DeeType_BASE(self),tp_p_instance_size);
 max_size = offset+DeeType_GET_SLOT(type_,tp_p_instance_size);
 if (max_size > DeeType_GET_SLOT(self,tp_p_instance_size)) {
  DeeType_GET_SLOT(self,tp_p_instance_size) = max_size;
  DeeType_GET_SLOT(self,tp_instance_size) = Dee_OFFSETOF(DeeStructObject,st_data)+max_size;
 }
 new_members->sm_offset = offset;
 new_members[1].sm_type = NULL;
#ifdef DEE_DEBUG
 new_members[1].sm_name = NULL;
 new_members[1].sm_offset = 0;
#endif
 return 0;
}
DEE_A_RET_NOEXCEPT(0) int DeeStructType_HasField(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *self, DEE_A_IN_OBJECT(DeeStringObject) const *name) {
 struct DeeStructMember *members; char const *name_str; Dee_size_t name_len;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructType_Check(self));
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 members = ((DeeStructTypeObject *)self)->tp_smembers;
 DEE_ASSERT(members);
 name_str = DeeString_STR(name);
 name_len = DeeString_SIZE(name);
 while (members->sm_type) {
  if (DeeString_SIZE(members->sm_name) == name_len &&
      memcmp(DeeString_STR(members->sm_name),name_str,name_len) == 0
      ) return 1;
  ++members;
 }
 return 0;
}
DEE_A_RET_NOEXCEPT(0) int DeeStructType_HasFieldString(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *self, DEE_A_IN_Z char const *name) {
 struct DeeStructMember *members; Dee_size_t name_len;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructType_Check(self));
 DEE_ASSERT(name);
 members = ((DeeStructTypeObject *)self)->tp_smembers;
 DEE_ASSERT(members);
 name_len = strlen(name);
 while (members->sm_type) {
  if (DeeString_SIZE(members->sm_name) == name_len &&
      memcmp(DeeString_STR(members->sm_name),name,name_len) == 0
      ) return 1;
  ++members;
 }
 return 0;
}
DEE_A_RET_NOEXCEPT(NULL) DeeTypeObject *DeeStructType_GetField(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *self, DEE_A_IN_OBJECT(DeeStringObject) const *name,
 DEE_A_OUT_OPT Dee_size_t *offset) {
 struct DeeStructMember *members; char const *name_str; Dee_size_t name_len;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructType_Check(self));
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 members = ((DeeStructTypeObject *)self)->tp_smembers;
 DEE_ASSERT(members);
 name_str = DeeString_STR(name);
 name_len = DeeString_SIZE(name);
 while (members->sm_type) {
  if (DeeString_SIZE(members->sm_name) == name_len &&
      memcmp(DeeString_STR(members->sm_name),name_str,name_len) == 0) {
   if (offset) *offset = members->sm_offset;
   return (DeeTypeObject *)members->sm_type;
  }
  ++members;
 }
 return NULL;
}
DEE_A_RET_NOEXCEPT(NULL) DeeTypeObject *DeeStructType_GetFieldString(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *self, DEE_A_IN_Z char const *name,
 DEE_A_OUT_OPT Dee_size_t *offset) {
 struct DeeStructMember *members; char const *name_str; Dee_size_t name_len;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructType_Check(self));
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 members = ((DeeStructTypeObject *)self)->tp_smembers;
 DEE_ASSERT(members);
 name_len = strlen((name_str = name));
 while (members->sm_type) {
  if (DeeString_SIZE(members->sm_name) == name_len &&
      memcmp(DeeString_STR(members->sm_name),name_str,name_len) == 0) {
   if (offset) *offset = members->sm_offset;
   return (DeeTypeObject *)members->sm_type;
  }
  ++members;
 }
 return NULL;
}


DEE_STATIC_INLINE(DeeObject *) _DeeStruct_GetAttrString(
 DeeStructTypeObject *tp_self, void *self, char const *attr, Dee_size_t attr_len) {
 DeeObject *result; int temp;
 DeeStructTypeObject *tp_iter;
 struct DeeStructMember *members;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(attr);
 tp_iter = tp_self;
 while (1) {
  members = tp_iter->tp_smembers;
  DEE_ASSERT(members);
  while (members->sm_type) {
   if (DeeString_SIZE(members->sm_name) == attr_len &&
       memcmp(DeeString_STR(members->sm_name),attr,attr_len) == 0) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
    DeeTypeObject *lv_type = DeeType_LValue((DeeTypeObject *)members->sm_type);
    return lv_type ? DeeLValue_New(lv_type,(void *)((
     Dee_uintptr_t)self+members->sm_offset)) : NULL;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
    return DeeStructured_NewFromData((DeeTypeObject *)members->sm_type,
                                     (void *)((Dee_uintptr_t)self+members->sm_offset));
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   }
   ++members;
  }
  tp_iter = (DeeStructTypeObject *)DeeType_BASE(tp_iter);
  DEE_ASSERT(tp_iter);
  if (!DeeStructType_Check(tp_iter)) break;
 }
 
 if ((temp = _DeeObject_PGenericGetAttrString(
  (DeeTypeObject *)tp_self,self,attr,&result)) != 0) return NULL;
 if (temp == 0) return result;
 DeeError_AttributeError_NotFound((DeeTypeObject *)tp_self,attr);
 return NULL;
}
DEE_STATIC_INLINE(int) _DeeStruct_HasAttrString(
 DeeStructTypeObject *tp_self, char const *attr, Dee_size_t attr_len) {
 DeeStructTypeObject *tp_iter;
 struct DeeStructMember *members;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(attr);
 tp_iter = tp_self;
 while (1) {
  members = tp_iter->tp_smembers;
  DEE_ASSERT(members);
  while (members->sm_type) {
   if (DeeString_SIZE(members->sm_name) == attr_len &&
       memcmp(DeeString_STR(members->sm_name),attr,attr_len) == 0
       ) return 1;
   ++members;
  }
  tp_iter = (DeeStructTypeObject *)DeeType_BASE(tp_iter);
  DEE_ASSERT(tp_iter);
  if (!DeeStructType_Check(tp_iter)) break;
 }
 return _DeeObject_PGenericHasAttrString((DeeTypeObject *)tp_self,attr);
}
DEE_STATIC_INLINE(int) _DeeStruct_DelAttrString(
 DeeStructTypeObject *tp_self, void *self, char const *attr, Dee_size_t attr_len) {
 int temp;
 DeeStructTypeObject *tp_iter;
 struct DeeStructMember *members;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(attr);
 tp_iter = tp_self;
 while (1) {
  members = tp_iter->tp_smembers;
  DEE_ASSERT(members);
  while (members->sm_type) {
   if (DeeString_SIZE(members->sm_name) == attr_len &&
       memcmp(DeeString_STR(members->sm_name),attr,attr_len) == 0) {
    memset((void *)((Dee_uintptr_t)self+members->sm_offset),0,
           DeeType_GET_SLOT(members->sm_type,tp_p_instance_size));
    return 0;
   }
   ++members;
  }
  tp_iter = (DeeStructTypeObject *)DeeType_BASE(tp_iter);
  DEE_ASSERT(tp_iter);
  if (!DeeStructType_Check(tp_iter)) break;
 }
 if ((temp = _DeeObject_PGenericDelAttrString(
  (DeeTypeObject *)tp_self,self,attr)) != 0) return -1;
 if (temp == 0) return 0;
 DeeError_AttributeError_NotFound((DeeTypeObject *)tp_self,attr);
 return -1;
}
DEE_STATIC_INLINE(int) _DeeStruct_SetAttrString(
 DeeStructTypeObject *tp_self, void *self, char const *attr, Dee_size_t attr_len, DeeObject *v) {
 int temp;
 DeeStructTypeObject *tp_iter;
 struct DeeStructMember *members;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(attr);
 tp_iter = tp_self;
 while (1) {
  members = tp_iter->tp_smembers;
  DEE_ASSERT(members);
  while (members->sm_type) {
   if (DeeString_SIZE(members->sm_name) == attr_len &&
       memcmp(DeeString_STR(members->sm_name),attr,attr_len) == 0
       ) return DeeStructured_SetData((DeeTypeObject *)members->sm_type,
                                      (void *)((Dee_uintptr_t)self+members->sm_offset),v);
   ++members;
  }
  tp_iter = (DeeStructTypeObject *)DeeType_BASE(tp_iter);
  DEE_ASSERT(tp_iter);
  if (!DeeStructType_Check(tp_iter)) break;
 }
 if ((temp = _DeeObject_PGenericSetAttrString(
  (DeeTypeObject *)tp_self,self,attr,v)) != 0) return -1;
 if (temp == 0) return 0;
 DeeError_AttributeError_NotFound((DeeTypeObject *)tp_self,attr);
 return -1;
}
DEE_STATIC_INLINE(DeeObject *) _DeeStruct_CallAttrStringString(
 DeeStructTypeObject *tp_self, void *self,
 char const *attr, Dee_size_t member_len, DeeObject *args) {
 DeeObject *result; int temp;
 DeeStructTypeObject *tp_iter;
 struct DeeStructMember *members;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(attr);
 tp_iter = tp_self;
 while (1) {
  members = tp_iter->tp_smembers;
  DEE_ASSERT(members);
  while (members->sm_type) {
   if (DeeString_SIZE(members->sm_name) == member_len &&
       memcmp(DeeString_STR(members->sm_name),attr,member_len) == 0)
       return DeeObject_PCall((DeeTypeObject *)members->sm_type,
        (void *)((Dee_uintptr_t)self+members->sm_offset),args);
   ++members;
  }
  tp_iter = (DeeStructTypeObject *)DeeType_BASE(tp_iter);
  DEE_ASSERT(tp_iter);
  if (!DeeStructType_Check(tp_iter)) break;
 }
 if ((temp = _DeeObject_PGenericCallAttrString(
  (DeeTypeObject *)tp_self,self,attr,args,&result)) != 0) return NULL;
 if (temp == 0) return result;
 DeeError_AttributeError_NotFound((DeeTypeObject *)tp_self,attr);
 return NULL;
}



DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_TGetAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStruct_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 return _DeeStruct_GetAttrString((
  DeeStructTypeObject *)tp_self,DeeStruct_DATA(self),
  DeeString_STR(attr),DeeString_SIZE(attr));
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_GetAttr(
 DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStruct_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 return _DeeStruct_GetAttrString(
  (DeeStructTypeObject *)Dee_TYPE(self),DeeStruct_DATA(self),
  DeeString_STR(attr),DeeString_SIZE(attr));
}
DEE_A_RET_EXCEPT(-1) int DeeStruct_TDelAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStruct_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 return _DeeStruct_DelAttrString((
  DeeStructTypeObject *)tp_self,DeeStruct_DATA(self),
  DeeString_STR(attr),DeeString_SIZE(attr));
}
DEE_A_RET_EXCEPT(-1) int DeeStruct_DelAttr(
 DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStruct_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 return _DeeStruct_DelAttrString(
  (DeeStructTypeObject *)Dee_TYPE(self),DeeStruct_DATA(self),
  DeeString_STR(attr),DeeString_SIZE(attr));
}
DEE_A_RET_EXCEPT(-1) int DeeStruct_TSetAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStruct_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(v));
 return _DeeStruct_SetAttrString(
  (DeeStructTypeObject *)tp_self,DeeStruct_DATA(self),
  DeeString_STR(attr),DeeString_SIZE(attr),v);
}
DEE_A_RET_EXCEPT(-1) int DeeStruct_SetAttr(
 DEE_A_INOUT_OBJECT(DeeStructObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStruct_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(v));
 return _DeeStruct_SetAttrString(
  (DeeStructTypeObject *)Dee_TYPE(self),DeeStruct_DATA(self),
  DeeString_STR(attr),DeeString_SIZE(attr),v);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_TGetAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_Z char const *attr) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStruct_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(attr);
 return _DeeStruct_GetAttrString((
  DeeStructTypeObject *)tp_self,DeeStruct_DATA(self),attr,strlen(attr));
}
DEE_A_RET_EXCEPT(-1) int DeeStruct_TDelAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_Z char const *attr) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStruct_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(attr);
 return _DeeStruct_DelAttrString((
  DeeStructTypeObject *)tp_self,DeeStruct_DATA(self),attr,strlen(attr));
}
DEE_A_RET_EXCEPT(-1) int DeeStruct_TSetAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructObject) *self,
 DEE_A_IN_Z char const *attr, DEE_A_INOUT DeeObject *v) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStruct_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(attr);
 return _DeeStruct_SetAttrString((
  DeeStructTypeObject *)tp_self,DeeStruct_DATA(self),attr,strlen(attr),v);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_TCallAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_Z char const *attr,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStruct_Check(self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 return _DeeStruct_CallAttrStringString((
  DeeStructTypeObject *)tp_self,
  DeeStruct_DATA(self),attr,strlen(attr),args);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_TCallAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStruct_Check(self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 return _DeeStruct_CallAttrStringString(
  (DeeStructTypeObject *)tp_self,DeeStruct_DATA(self),
  DeeString_STR(attr),DeeString_SIZE(attr),args);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_PGetAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 return _DeeStruct_GetAttrString(
  (DeeStructTypeObject *)tp_self,base_addr,
  DeeString_STR(attr),DeeString_SIZE(attr));
}
DEE_A_RET_NOEXCEPT(0) int DeeStruct_PHasAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 return _DeeStruct_HasAttrString(
  (DeeStructTypeObject *)tp_self,
  DeeString_STR(attr),DeeString_SIZE(attr));
}
DEE_A_RET_EXCEPT(-1) int DeeStruct_PDelAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 return _DeeStruct_DelAttrString(
  (DeeStructTypeObject *)tp_self,base_addr,
  DeeString_STR(attr),DeeString_SIZE(attr));
}
DEE_A_RET_EXCEPT(-1) int DeeStruct_PSetAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 return _DeeStruct_SetAttrString(
  (DeeStructTypeObject *)tp_self,base_addr,
  DeeString_STR(attr),DeeString_SIZE(attr),v);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_PGetAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(attr);
 return _DeeStruct_GetAttrString((DeeStructTypeObject *)tp_self,base_addr,attr,strlen(attr));
}
DEE_A_RET_NOEXCEPT(0) int DeeStruct_PHasAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_IN_Z char const *attr) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(attr);
 return _DeeStruct_HasAttrString(
  (DeeStructTypeObject *)tp_self,attr,strlen(attr));
}
DEE_A_RET_EXCEPT(-1) int DeeStruct_PDelAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(attr);
 return _DeeStruct_DelAttrString(
  (DeeStructTypeObject *)tp_self,base_addr,attr,strlen(attr));
}
DEE_A_RET_EXCEPT(-1) int DeeStruct_PSetAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr, DEE_A_INOUT DeeObject *v) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(attr);
 return _DeeStruct_SetAttrString(
  (DeeStructTypeObject *)tp_self,base_addr,attr,strlen(attr),v);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_PCallAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 DEE_ASSERT(attr);
 return _DeeStruct_CallAttrStringString(
  (DeeStructTypeObject *)tp_self,base_addr,attr,strlen(attr),args);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_PCallAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 return _DeeStruct_CallAttrStringString(
  (DeeStructTypeObject *)tp_self,base_addr,
  DeeString_STR(attr),DeeString_SIZE(attr),args);
}


DEE_A_RET_EXCEPT(-1) int DeeStruct_PInitializeFromIterator(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *iterator) {
 struct DeeStructMember *member_iter; int error; DeeObject *init_elem;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(iterator));
 // Walk the members if the initializer doesn't yield tuples
 member_iter = ((DeeStructTypeObject *)tp_self)->tp_smembers;
 while ((error = DeeObject_IterNextEx(iterator,&init_elem)) == 0) {
  if (DeeTuple_Check(init_elem) && DeeTuple_SIZE(init_elem) == 2 &&
      DeeString_Check(DeeTuple_GET(init_elem,0))) { // named member intializer
   if (DeeStruct_PSetAttr(tp_self,base_addr,DeeTuple_GET(
    init_elem,0),DeeTuple_GET(init_elem,1)) != 0) goto err_init_elem;
  } else {
   if (DeeStructured_SetData((DeeTypeObject *)member_iter->sm_type,
    (void *)((Dee_uintptr_t)base_addr+member_iter->sm_offset),init_elem) != 0) {
err_init_elem: Dee_DECREF(init_elem); return -1;
   }
   ++member_iter;
  }
  Dee_DECREF(init_elem);
 }
 return error < 0 ? error : 0;
}


static int _deestruct_tp_p_any_ctor(
 DeeStructTypeObject *tp_self, Dee_uint8_t *self_data, DeeObject *args) {
 int result; DeeObject *initializer,*init_iterator;
 if (DeeTuple_Unpack(args,"o:struct",&initializer) != 0) return -1;
 if ((init_iterator = DeeObject_IterSelf(initializer)) == NULL) return -1;
 result = DeeStruct_PInitializeFromIterator(
  (DeeTypeObject *)tp_self,(void *)self_data,init_iterator);
 Dee_DECREF(init_iterator);
 return result;
}
static int _deestruct_tp_p_any_assign(
 DeeStructTypeObject *tp_self, Dee_uint8_t *self_data, DeeObject *right) {
 int result; DeeObject *init_iterator;
 if ((init_iterator = DeeObject_IterSelf(right)) == NULL) return -1;
 result = DeeStruct_PInitializeFromIterator(
  (DeeTypeObject *)tp_self,(void *)self_data,init_iterator);
 Dee_DECREF(init_iterator);
 return result;
}
static DeeObject *_deestruct_tp_p_str(
 DeeStructTypeObject *tp_self, Dee_uint8_t *self_data) {
 struct DeeStructMember *members;
 DeeObject *result,*elem_str;
 DeeStringWriter writer = DeeStringWriter_INIT();
 members = tp_self->tp_smembers;
 DEE_ASSERT(members);
 if (DeeStringWriter_WRITE_STRING(&writer,"{ ") != 0) goto err;
 while (members->sm_type) {
  DEE_SIGNALHANDLER_BEGIN {
   elem_str = DeeObject_PStr((DeeTypeObject *)members->sm_type,self_data+members->sm_offset);
  } DEE_SIGNALHANDLER_END({ elem_str = NULL; });
  if (!elem_str) goto err;
  if (DeeStringWriter_Writef(&writer,".%s = %K, ",
   DeeString_STR(members->sm_name),elem_str) != 0) goto err;
  ++members;
 }
 if (DeeStringWriter_WRITE_STRING(&writer,"}") != 0) goto err;
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}
static DeeObject *_deestruct_tp_p_repr(
 DeeStructTypeObject *tp_self, Dee_uint8_t *self_data) {
 return DeeString_Newf(
  "struct %s %K",DeeType_NAME(tp_self),
  _deestruct_tp_p_str(tp_self,self_data));
}

static void _DeeStructType_InitCommon(DeeStructTypeObject *self) {
 self->tp_smembers = _DeeStructType_EmptyMembers;
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
 self->tp_ffitype.size = 0;
 self->tp_ffitype.alignment = 0;
 self->tp_ffitype.elements = _deeffi_empty_type;
 self->tp_ffitype.type = FFI_TYPE_STRUCT;
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
 DeeType_GET_SLOT(self,tp_str) = DeeType_GET_SLOT(&DeeStructured_Type,tp_str);
 DeeType_GET_SLOT(self,tp_repr) = DeeType_GET_SLOT(&DeeStructured_Type,tp_repr);
 DeeType_GET_SLOT(self,tp_p_any_ctor) = (int(*)(struct DeeStructuredTypeObject*,void*,DeeObject*))&_deestruct_tp_p_any_ctor;
 DeeType_GET_SLOT(self,tp_p_any_assign) = (int(*)(struct DeeStructuredTypeObject*,void*,DeeObject*))&_deestruct_tp_p_any_assign;
 DeeType_GET_SLOT(self,tp_p_str) = (DeeObject*(*)(struct DeeStructuredTypeObject*,void*))&_deestruct_tp_p_str;
 DeeType_GET_SLOT(self,tp_p_repr) = (DeeObject*(*)(struct DeeStructuredTypeObject*,void*))&_deestruct_tp_p_repr;
 DeeType_GET_SLOT(self,tp_attr_get) = (DeeObject*(*)(DeeObject*,DeeObject*))&DeeStruct_GetAttr;
 DeeType_GET_SLOT(self,tp_attr_del) = (int(*)(DeeObject*,DeeObject*))&DeeStruct_DelAttr;
 DeeType_GET_SLOT(self,tp_attr_set) = (int(*)(DeeObject*,DeeObject*,DeeObject*))&DeeStruct_SetAttr;
 DeeType_GET_SLOT(self,tp_p_attr_get) = (DeeObject*(*)(struct DeeStructuredTypeObject*,void*,DeeObject*))&DeeStruct_PGetAttr;
 DeeType_GET_SLOT(self,tp_p_attr_del) = (int(*)(struct DeeStructuredTypeObject*,void*,DeeObject*))&DeeStruct_PDelAttr;
 DeeType_GET_SLOT(self,tp_p_attr_set) = (int(*)(struct DeeStructuredTypeObject*,void*,DeeObject*,DeeObject*))&DeeStruct_PSetAttr;
}

static int _deestructtype_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeStructTypeObject *self) {
 if ((*DeeType_GET_SLOT(&DeeStructuredType_Type,tp_ctor))(
  &DeeStructuredType_Type,(DeeObject *)self) != 0) return -1;
 DeeType_GET_SLOT(self,tp_name) = "anonymous_struct";
 _DeeStructType_InitCommon(self);
 return 0;
}
DEE_VISIT_PROC(_deestructtype_tp_visit,DeeStructTypeObject *self) {
 struct DeeStructMember *members = self->tp_smembers;
 while (members->sm_type) {
  Dee_VISIT(members->sm_type);
  Dee_VISIT(members->sm_name);
  ++members;
 }
}
static DeeObject *_deestructtype_tp_repr(DeeStructTypeObject *self) {
 DeeObject *result; int is_union;
 struct DeeStringWriter writer = DeeStringWriter_INIT();
 struct DeeStructMember *members;
 if (DeeType_IsFinal(self) && DeeStringWriter_WRITE_STRING(&writer,"final ") != 0) goto err;
 members = self->tp_smembers;
 is_union = (members[0].sm_type != NULL && members[1].sm_type != NULL);
 if (is_union) {
  Dee_size_t first_offset = members->sm_offset;
  do {
   if ((++members)->sm_offset != first_offset) {
    is_union = 0;
    break;
   }
  } while (members->sm_type);
 }
 members = self->tp_smembers;
 if (DeeStringWriter_Writef(&writer,"%s %s { ",
  is_union ? "union" : "struct",DeeType_NAME(self)) != 0) goto err;
 while (members->sm_type) {
  if (DeeStringWriter_Writef(&writer,
   "%r %k; ",members->sm_type,members->sm_name) != 0) goto err;
  ++members;
 }
 if (DeeStringWriter_WRITE_STRING(&writer,"}") != 0) goto err;
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}
static void _deestructtype_tp_dtor(DeeStructTypeObject *self) {
 struct DeeStructMember *members;
 DEE_ASSERT(!DeeType_HAS_SLOT(self,tp_marshal) || (DeeType_GET_SLOT(self,tp_marshal)->
  tp_marshal_ctor_ == (DeeType_SLOT_TYPE(tp_marshal_ctor))&_deestructured_tp_marshal_ctor));
 DEE_ASSERT(!DeeType_HAS_SLOT(self,tp_marshal) || (DeeType_GET_SLOT(self,tp_marshal)->
  tp_marshal_put == (DeeType_SLOT_TYPE(tp_marshal_put))&_deestructured_tp_marshal_put));
 free(DeeType_GET_SLOT(self,tp_marshal)); // Free a custom marshal struct uuid
 if ((members = self->tp_smembers) != _DeeStructType_EmptyMembers) {
  while (members->sm_type) {
   Dee_DECREF(members->sm_type);
   Dee_DECREF(members->sm_name);
   ++members;
  }
  free_nn(self->tp_smembers);
 }
}
static void _deestructtype_tp_clear(DeeStructTypeObject *self) {
 struct DeeStructMember *members;
 if ((members = self->tp_smembers) != _DeeStructType_EmptyMembers) {
  while (members->sm_type) {
   Dee_DECREF(members->sm_type);
   Dee_DECREF(members->sm_name);
   ++members;
  }
  free_nn(self->tp_smembers);
  self->tp_smembers = _DeeStructType_EmptyMembers;
 }
}
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) _DeeStructType_Equals(
 DeeStructTypeObject *self, DeeStructTypeObject *right) {
 struct DeeStructMember *lm,*rm; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructType_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeStructType_Check(right));
 if (DeeType_NAME(self)) {
  if (!DeeType_NAME(right)) return 0;
  if (strcmp(DeeType_NAME(self),DeeType_NAME(right)) != 0) return 0;
 } else if (DeeType_NAME(right)) return 0;
 lm = self->tp_smembers,rm = right->tp_smembers;
 if (lm == rm) return 1; // Unlikely, but can happen for '_DeeStructType_EmptyMembers'
 while (lm->sm_type) {
  if (!rm->sm_type) return 0;
  if (lm->sm_offset != rm->sm_offset) return 0;
  if (DeeString_SIZE(lm->sm_name) != DeeString_SIZE(rm->sm_name) ||
      memcmp(DeeString_STR(lm->sm_name),DeeString_STR(rm->sm_name),
      DeeString_SIZE(lm->sm_name)) != 0) return 0;
  if (lm->sm_type != rm->sm_type) {
   temp = DeeObject_CompareEq((DeeObject *)lm->sm_type,(DeeObject *)rm->sm_type);
   if (temp < 0) { DeeError_Handled(); return 0; } // Ignore errors here...
   if (!temp) return 0;
  }
  ++lm,++rm;
 }
 return rm->sm_type == NULL;
}
static DeeObject *_deestructtype_tp_cmp_eq(
 DeeStructTypeObject *self, DeeStructTypeObject *right) {
 if (DeeObject_InplaceGetInstance(&right,&DeeStructType_Type) != 0) return NULL;
 DeeReturn_Bool(_DeeStructType_Equals(self,right));
}
static DeeObject *_deestructtype_tp_cmp_ne(
 DeeStructTypeObject *self, DeeStructTypeObject *right) {
 if (DeeObject_InplaceGetInstance(&right,&DeeStructType_Type) != 0) return NULL;
 DeeReturn_Bool(!_DeeStructType_Equals(self,right));
}

#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
static DeeObject *_deestructtype___add_field(
 DeeStructTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *name,*ty; Dee_size_t offset;
 if (DeeTuple_Unpack(args,"oIuo:__add_field",&name,&offset,&ty) != 0) return NULL;
 if (DeeError_TypeError_CheckTypeExact(name,&DeeString_Type) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&ty,&DeeStructuredType_Type) != 0) return NULL;
 if (DeeStructType_HasField((DeeTypeObject *)self,name)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,"struct %k already contains member %k",self,name);
  return NULL;
 }
 if (DeeStructType_AddField((DeeTypeObject *)self,name,offset,(DeeTypeObject *)ty) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deestructtype___set_base(
 DeeStructTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeTypeObject *new_base;
 if (DeeTuple_Unpack(args,"o:__set_base",&new_base) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&new_base,&DeeStructuredType_Type) != 0) return NULL;
 if (DeeStructType_SetBase((DeeTypeObject *)self,new_base) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deestructtype___changed(
 DeeStructTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":__changed") != 0) return NULL;
 DeeStructType_Changed((DeeTypeObject *)self);
 DeeReturn_None;
}

static struct DeeMemberDef const _deestructtype_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__tp_smembers",DeeStructTypeObject,tp_smembers,p(void)),
 DEE_MEMBERDEF_END_v100
};

static struct DeeMethodDef const _deestructtype_tp_methods[] = {
 DEE_METHODDEF_v100("__add_field",member(&_deestructtype___add_field),"(size_t name, type object ty, size_t offset) -> none"),
 DEE_METHODDEF_v100("__set_base",member(&_deestructtype___set_base),"(type object ty) -> none"),
 DEE_METHODDEF_v100("__changed",member(&_deestructtype___changed),"() -> none"),
 DEE_METHODDEF_END_v100
};
#else /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
#define _deestructtype_tp_members DeeType_DEFAULT_SLOT(tp_members)
#define _deestructtype_tp_methods DeeType_DEFAULT_SLOT(tp_methods)
#endif /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */


static int DEE_CALL _deestructtype_tp_marshal_ctor(
 DeeTypeObject *tp_self, DeeStructTypeObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_uint64_t read_size; Dee_size_t size,new_size,max_size;
 struct DeeStructMember *members;
 if DEE_UNLIKELY((*DeeType_GET_SLOT(&DeeStructuredType_Type,tp_marshal)->
  tp_marshal_ctor_)(tp_self,(DeeObject *)self,infile,map) != 0) return -1;
 _DeeStructType_InitCommon(self);
 if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&read_size) != 0) goto err;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_size > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Struct member count is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_size,(Dee_size_t)-1);
  return -1;
 }
#endif
 DEE_ASSERT(self->tp_smembers == _DeeStructType_EmptyMembers);
 if ((size = (Dee_size_t)read_size) != 0) {
  while DEE_UNLIKELY((members = (struct DeeStructMember *)malloc_nz(
   (size+1)*sizeof(struct DeeStructMember))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   goto err;
  }
  self->tp_smembers = members;
  max_size = 0;
  do {
   if DEE_UNLIKELY((members->sm_type = (DeeStructuredTypeObject *)
    DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {
err_members: while (members != self->tp_smembers) {
     --members;
     Dee_DECREF(members->sm_name);
     Dee_DECREF(members->sm_type);
    }
    free_nn(members);
    self->tp_smembers = _DeeStructType_EmptyMembers;
    goto err;
   }
   if DEE_UNLIKELY(DeeError_TypeError_CheckType((DeeObject *)members->sm_type,&DeeStructuredType_Type) != 0) goto err_members;
   if DEE_UNLIKELY((members->sm_name = (DeeStringObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) {err_members_type: Dee_DECREF(members->sm_type); goto err_members; }
   if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)members->sm_name,&DeeString_Type) != 0) {err_members_name: Dee_DECREF(members->sm_name); goto err_members_type; }
   if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&read_size) != 0) goto err_members_name;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
   if DEE_UNLIKELY(read_size > (Dee_uint64_t)((Dee_size_t)-1)) {
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        "Struct member offset is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                        read_size,(Dee_size_t)-1);
    return -1;
   }
#endif
   new_size = (members->sm_offset = (Dee_size_t)read_size)+
    DeeType_GET_SLOT(members->sm_type,tp_p_instance_size);
   if (new_size > max_size) max_size = new_size;
   ++members;
  } while (--size);
  members->sm_type = NULL;
#ifdef DEE_DEBUG
  members->sm_name = NULL;
  members->sm_offset = 0;
#endif
  DeeType_GET_SLOT(self,tp_p_instance_size) = max_size;
  DeeType_GET_SLOT(self,tp_instance_size) = Dee_OFFSETOF(DeeStructObject,st_data)+max_size;
  DeeStructType_Changed((DeeTypeObject *)self);
 }
 return 0;
err:
 _deestructtype_tp_dtor(self);
 (*DeeType_GET_SLOT(&DeeStructuredType_Type,tp_dtor))((DeeObject *)self);
 (*DeeType_GET_SLOT(&DeeType_Type,tp_dtor))((DeeObject *)self);
 return -1;
}

static int DEE_CALL _deestructtype_tp_marshal_put(
 DeeTypeObject *tp_self, DeeStructTypeObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 Dee_size_t size; struct DeeStructMember *members;
 if DEE_UNLIKELY((*DeeType_GET_SLOT(&DeeStructuredType_Type,tp_marshal)->tp_marshal_put)(
  tp_self,(DeeObject *)self,outfile,map) != 0) return -1;
 members = self->tp_smembers; size = 0;
 while (members->sm_type) ++size,++members;
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)size) != 0) return -1;
 members = self->tp_smembers;
 while (members->sm_type) {
  if DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)members->sm_type) != 0) return -1;
  if DEE_UNLIKELY(DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)members->sm_name) != 0) return -1;
  if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)members->sm_offset) != 0) return -1;
  ++members;
 }
 return 0;
}

static struct DeeTypeMarshal _deestructtype_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(struct_type)),
 member(&_deestructtype_tp_marshal_ctor),member(&_deestructtype_tp_marshal_put));

DeeTypeObject DeeStructType_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("struct_type"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_HAS_GC),member(&DeeStructuredType_Type)),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeStructTypeObject),member(&_DeeGC_TpAlloc),
  member(&_deestructtype_tp_ctor),null,null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(member(&_DeeGC_TpFree),
  member(&_deestructtype_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,
  member(&_deestructtype_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deestructtype_tp_visit),
  member(&_deestructtype_tp_clear)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,
  member(&_deestructtype_tp_cmp_eq),
  member(&_deestructtype_tp_cmp_ne),null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deestructtype_tp_members),null,
  member(_deestructtype_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deestructtype_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_STRUCT_C */
