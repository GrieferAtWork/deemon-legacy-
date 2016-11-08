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
#ifndef GUARD_DEEMON_ENUM_ATTRIBUTES_C
#define GUARD_DEEMON_ENUM_ATTRIBUTES_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include <deemon/object.h>
#include <deemon/dex.h>
#include <deemon/type.h>
#include <deemon/class.h>
#include <deemon/struct.h>
#include <deemon/string.h>
#include <deemon/super.h>
#include <deemon/memberfunction.h>
#include <deemon/structured.h>
#include <deemon/memberdef.h>
#include <deemon/optional/hashmap.h>
#include <deemon/optional/enum_attributes.h>
#if !DEE_CONFIG_RUNTIME_HAVE_POINTERS
#include <deemon/none.h>
#endif

DEE_DECL_BEGIN

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeMemberDef_EnumAttr(
 DEE_A_IN struct DeeMemberDef const *members,
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeEnumAttrFunc enum_attr, void *closure) {
 int temp; DeeTypeObject *member_type;
 while (members->d_name) {
  if ((member_type = DeeMember_Type(members)) == NULL) return -1;
  temp = (*enum_attr)(tp_self,self,members->d_name,(Dee_uint32_t)(
   ((members->d_access&DeeMemberAccess_READ)!=0 ? (DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET) : 0)|
   ((members->d_access&DeeMemberAccess_WRITE)!=0 ? (DEE_ENUMATTRIBUTE_FLAG_PERMISSION_DEL|DEE_ENUMATTRIBUTE_FLAG_PERMISSION_SET) : 0)
   ),member_type,closure);
  if (temp != 0) return temp;
  ++members;
 }
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeGetSetDef_EnumAttr(
 DEE_A_IN struct DeeGetSetDef const *getsets,
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeEnumAttrFunc enum_attr, void *closure) {
 int temp; Dee_uint32_t flags;
 while (getsets->d_name) {
  flags = DEE_ENUMATTRIBUTE_FLAG_NONE;
  if (getsets->d_get) flags |= DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET;
  if (getsets->d_del) flags |= DEE_ENUMATTRIBUTE_FLAG_PERMISSION_DEL;
  if (getsets->d_set) flags |= DEE_ENUMATTRIBUTE_FLAG_PERMISSION_SET;
  temp = (*enum_attr)(tp_self,self,getsets->d_name,flags,NULL,closure);
  if (temp != 0) return temp;
  ++getsets;
 }
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeMethodDef_EnumAttr(
 DEE_A_IN struct DeeMethodDef const *methods,
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeEnumAttrFunc enum_attr, void *closure) {
 int temp;
 while (methods->d_name) {
  temp = (*enum_attr)(tp_self,self,methods->d_name,
   DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET,&DeeMemberFunction_Type,closure);
  if (temp != 0) return temp;
  ++methods;
 }
 return 0;
}
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeClassTypeMethodDef_EnumAttr(
 DEE_A_IN struct _DeeClassTypeMethodDef const *methods,
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeEnumAttrFunc enum_attr, void *closure) {
 int temp;
 while (methods->ctmd_name) {
  temp = (*enum_attr)(tp_self,self,DeeString_STR(methods->ctmd_name),
   DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET,&DeeInstanceMethod_Type,closure);
  if (temp != 0) return temp;
  ++methods;
 }
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeClassTypeGetSetDef_EnumAttr(
 DEE_A_IN struct _DeeClassTypeGetSetDef const *getsets,
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeEnumAttrFunc enum_attr, void *closure) {
 int temp; Dee_uint32_t flags;
 while (getsets->ctgd_name) {
  getsets->ctgd_get;
  flags = DEE_ENUMATTRIBUTE_FLAG_NONE;
  if (getsets->ctgd_get) flags |= DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET;
  if (getsets->ctgd_del) flags |= DEE_ENUMATTRIBUTE_FLAG_PERMISSION_DEL;
  if (getsets->ctgd_set) flags |= DEE_ENUMATTRIBUTE_FLAG_PERMISSION_SET;
  temp = (*enum_attr)(tp_self,self,DeeString_STR(getsets->ctgd_name),
                      flags,&DeeInstanceMethod_Type,closure);
  if (temp != 0) return temp;
  ++getsets;
 }
 return 0;
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES */


DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeObject_GenericEnumAttr(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeEnumAttrFunc enum_attr, void *closure) {
 int temp;
 if ((temp = DeeMemberDef_EnumAttr(DeeType_GET_SLOT(
  tp_self,tp_members),tp_self,self,enum_attr,closure)) != 0) return temp;
 if ((temp = DeeGetSetDef_EnumAttr(DeeType_GET_SLOT(
  tp_self,tp_getsets),tp_self,self,enum_attr,closure)) != 0) return temp;
 if ((temp = DeeMethodDef_EnumAttr(DeeType_GET_SLOT(
  tp_self,tp_methods),tp_self,self,enum_attr,closure)) != 0) return temp;
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeType_EnumAttr(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeTypeObject *self,
 DEE_A_IN DeeEnumAttrFunc enum_attr, void *closure) {
 int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 if ((temp = DeeObject_GenericEnumAttr(tp_self,(DeeObject *)self,enum_attr,closure)) != 0) return temp;
 if ((temp = DeeMemberDef_EnumAttr(DeeType_GET_SLOT(self,tp_class_members),tp_self,(DeeObject *)self,enum_attr,closure)) != 0) return temp;
 if ((temp = DeeGetSetDef_EnumAttr(DeeType_GET_SLOT(self,tp_class_getsets),tp_self,(DeeObject *)self,enum_attr,closure)) != 0) return temp;
 if ((temp = DeeMethodDef_EnumAttr(DeeType_GET_SLOT(self,tp_class_methods),tp_self,(DeeObject *)self,enum_attr,closure)) != 0) return temp;
 return 0;
}
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeInstance_EnumAttr(
 DEE_A_INOUT DeeClassObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeEnumAttrFunc enum_attr, void *closure) {
 struct DeeClassSuffix *suffix; int temp;
 struct DeeInstanceLevel *level;
 struct _DeeHashMapNode *node;
 DeeHashMap_TRAVERSE_SAFE_VARS;
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeClass_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,(DeeTypeObject *)tp_self));
 suffix = DeeClass_SUFFIX(tp_self);
 if ((temp = _DeeClassTypeMethodDef_EnumAttr(suffix->cs_methods,(DeeTypeObject *)tp_self,self,enum_attr,closure)) != 0) return temp;
 if ((temp = _DeeClassTypeGetSetDef_EnumAttr(suffix->cs_getsets,(DeeTypeObject *)tp_self,self,enum_attr,closure)) != 0) return temp;
 // Check for member variables
 level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
 DeeAtomicMutex_Acquire(&level->il_lock);
 DeeHashMap_TRAVERSE_SAFE(node,&level->il_members) {
  DeeObject *key; DeeTypeObject *item_type;
  Dee_INCREF(key = node->hmn_key);
  item_type = Dee_TYPE(node->hmn_item);
  DeeAtomicMutex_Release(&level->il_lock);
  if (DeeString_Check(key)) {
   temp = (*enum_attr)((DeeTypeObject *)tp_self,self,DeeString_STR(key),
                       DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET|
                       DEE_ENUMATTRIBUTE_FLAG_PERMISSION_DEL|
                       DEE_ENUMATTRIBUTE_FLAG_PERMISSION_SET,
                       item_type,closure);
  } else temp = 0;
  Dee_DECREF(key);
  if (temp != 0) return temp;
  DeeAtomicMutex_Acquire(&level->il_lock);
 }
 DeeAtomicMutex_Release(&level->il_lock);
 return DeeObject_GenericEnumAttr((DeeTypeObject *)tp_self,self,enum_attr,closure);
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeClass_EnumAttr(
 DEE_A_IN DeeClassTypeObject const *tp_self, DEE_A_INOUT DeeClassObject *self,
 DEE_A_IN DeeEnumAttrFunc enum_attr, void *closure) {
 struct DeeClassSuffix *suffix; int temp;
 struct _DeeClassTypeMethodDef const *methods;
 struct _DeeClassTypeGetSetDef const *getsets;
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeClassType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,(DeeTypeObject *)tp_self));
 suffix = DeeClass_SUFFIX(self);
 if ((temp = _DeeClassTypeMethodDef_EnumAttr(suffix->cs_class_methods,(DeeTypeObject *)self,(DeeObject *)self,enum_attr,closure)) != 0) return temp;
 if ((temp = _DeeClassTypeGetSetDef_EnumAttr(suffix->cs_class_getsets,(DeeTypeObject *)self,(DeeObject *)self,enum_attr,closure)) != 0) return temp;
 // Check for instance methods/getsets (accessible through the type)
 methods = suffix->cs_methods;
 while (methods->ctmd_name) {
  temp = (*enum_attr)((DeeTypeObject *)self,(DeeObject *)self,DeeString_STR(methods->ctmd_name),
   DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET,Dee_TYPE(methods->ctmd_func),closure);
  if (temp != 0) return temp;
  ++methods;
 }
 getsets = suffix->cs_getsets;
 while (getsets->ctgd_name) {
  temp = (*enum_attr)((DeeTypeObject *)self,(DeeObject *)self,DeeString_STR(getsets->ctgd_name),
   DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET,&DeeProperty_Type,closure);
  if (temp != 0) return temp;
  ++getsets;
 }
 return DeeType_EnumAttr((DeeTypeObject *)tp_self,
                         (DeeTypeObject *)self,
                         enum_attr,closure);
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES */


DEE_A_RET_EXCEPT(-1) int DeeObject_PGenericEnumAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeEnumAttrFunc enum_attr, void *closure) {
 struct DeeGetSetDef const *getsets;
 struct DeeMethodDef const *methods;
 int temp; Dee_uint32_t flags;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 (void)base_addr;
 if ((temp = DeeMemberDef_EnumAttr(DeeType_GET_SLOT(
  tp_self,tp_members),tp_self,self,enum_attr,closure)) != 0) return temp;
 getsets = DeeType_GET_SLOT(tp_self,tp_getsets);
 // Enumerate structured getsets/methods
 while (getsets->d_name) {
  if ((getsets->d_flags&DEE_GETSETDEF_FLAG_STRUCT)!=0) {
   flags = DEE_ENUMATTRIBUTE_FLAG_NONE;
   if (getsets->d_get) flags |= DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET;
   if (getsets->d_del) flags |= DEE_ENUMATTRIBUTE_FLAG_PERMISSION_DEL;
   if (getsets->d_set) flags |= DEE_ENUMATTRIBUTE_FLAG_PERMISSION_SET;
   temp = (*enum_attr)(tp_self,self,getsets->d_name,flags,NULL,closure);
   if (temp != 0) return temp;
  }
  ++getsets;
 }
 methods = DeeType_GET_SLOT(tp_self,tp_methods);
 while (methods->d_name) {
  if ((methods->d_flags&DEE_METHODDEF_FLAG_STRUCT)!=0) {
   temp = (*enum_attr)(tp_self,self,getsets->d_name,
                       DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET,
                       &DeeMemberFunction_Type,closure);
   if (temp != 0) return temp;
  }
  ++getsets;
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeStructMember_EnumAttr(
 DEE_A_IN struct DeeStructMember const *members,
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeEnumAttrFunc enum_attr, void *closure) {
 int temp;
 while (members->sm_type) {
  temp = (*enum_attr)(tp_self,self,DeeString_STR(members->sm_name),
                      DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET|
                      DEE_ENUMATTRIBUTE_FLAG_PERMISSION_DEL|
                      DEE_ENUMATTRIBUTE_FLAG_PERMISSION_SET,
                      (DeeTypeObject *)members->sm_type,closure);
  ++members;
 }
 return 0;
}
#define DeeStruct_EnumAttr(tp_self,self,enum_attr,closure)\
 (DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructType_Check(tp_self)),\
  DeeStructMember_EnumAttr((tp_self)->tp_smembers,(DeeTypeObject *)(tp_self),self,enum_attr,closure))


DEE_A_RET_EXCEPT(-1) int DeeObject_PEnumAttrImpl(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeEnumAttrFunc enum_attr, void *closure) {
 DeeTypeObject const *tp_iter;
 int temp; DeeType_SLOT_TYPE(tp_p_attr_get) protocol;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(base_addr);
 DEE_ASSERT(enum_attr);
 (void)base_addr;
 tp_iter = tp_self;
 while (1) {
  protocol = DeeType_GET_SLOT(tp_iter,tp_p_attr_get);
  if (protocol == (DeeType_SLOT_TYPE(tp_p_attr_get))&DeeStruct_PGetAttr)
   temp = DeeStruct_EnumAttr((DeeStructTypeObject *)tp_iter,self,enum_attr,closure);
  else temp = 0;
  if (temp != 0) return temp;
  if ((tp_iter = DeeType_BASE(tp_iter)) == NULL) break;
 }
 return DeeObject_PGenericEnumAttr(tp_self,DeeStructured_ADDR(self),self,enum_attr,closure);
}

DEE_A_RET_EXCEPT(-1) int DeeObject_TEnumAttr(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeEnumAttrFunc enum_attr, void *closure) {
 DeeTypeObject const *tp_iter;
 int temp; DeeType_SLOT_TYPE(tp_attr_get) protocol;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(enum_attr);
 tp_iter = tp_self;
 while (1) {
  protocol = DeeType_GET_SLOT(tp_iter,tp_attr_get);
  if (protocol == (DeeObject*(*)(DeeObject*,DeeObject*))&DeeObject_GenericGetAttr)
   temp = DeeObject_GenericEnumAttr(tp_iter,self,enum_attr,closure);
  else if (protocol == (DeeObject*(*)(DeeObject*,DeeObject*))&DeeType_GetAttr)
   temp = DeeType_EnumAttr((DeeTypeObject *)tp_iter,(DeeTypeObject *)self,enum_attr,closure);
  else if (protocol == (DeeObject*(*)(DeeObject*,DeeObject*))&_deesuper_tp_attr_get)
   return DeeObject_TEnumAttr(DeeSuper_TYPE(self),DeeSuper_SELF(self),enum_attr,closure);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
  else if (protocol == (DeeObject*(*)(DeeObject*,DeeObject*))&DeeInstance_GetAttr)
   temp = DeeInstance_EnumAttr((DeeClassObject *)tp_iter,self,enum_attr,closure);
  else if (protocol == (DeeObject*(*)(DeeObject*,DeeObject*))&DeeClass_GetAttr)
   temp = DeeClass_EnumAttr((DeeClassTypeObject *)tp_self,(DeeClassObject *)self,enum_attr,closure);
#endif /* DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES */
  else if (protocol == (DeeObject*(*)(DeeObject*,DeeObject*))&DeeStructured_GetAttr)
   return DeeObject_PEnumAttrImpl(tp_iter,DeeStructured_ADDR(self),self,enum_attr,closure);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  else if (protocol == (DeeObject*(*)(DeeObject*,DeeObject*))&_deelvalue_tp_attr_get)
   return DeeObject_PEnumAttrImpl(DeeLValueType_BASE(tp_iter),DeeLValue_ADDR(self),self,enum_attr,closure);
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  else if (protocol == (DeeObject*(*)(DeeObject*,DeeObject*))&DeeStruct_GetAttr)
   temp = DeeStruct_EnumAttr((DeeStructTypeObject *)tp_iter,self,enum_attr,closure);
  else if (protocol == (DeeObject*(*)(DeeObject*,DeeObject*))&_deedexmodule_tp_attr_get)
   return DeeDexModule_EnumAttr((DeeDexModuleObject *)self,enum_attr,closure);
  else temp = 0;
  if (temp != 0) return temp;
  if ((tp_iter = DeeType_BASE(tp_iter)) == NULL) break;
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeObject_PEnumAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN DeeEnumAttrFunc enum_attr, void *closure) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeObject *lval_self; DeeTypeObject *lval_type; int result;
 if ((lval_type = DeeType_LValue(tp_self)) == NULL) return -1;
 if ((lval_self = DeeLValue_New(lval_type,base_addr)) == NULL) return -1;
 result = DeeObject_PEnumAttrImpl(tp_self,base_addr,lval_self,enum_attr,closure);
 Dee_DECREF(lval_self);
 return result;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 // We use 'none' for self in this, as we don't have lvalue types to use as proxy...
 return DeeObject_PEnumAttrImpl(tp_self,base_addr,Dee_None,enum_attr,closure);
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_ENUM_ATTRIBUTES_H */
