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
#include "class.c"
DEE_DECL_BEGIN
#define STRING
#endif

#ifdef STRING
#define attr_str       attr
#define ATTR_ARG       DEE_A_IN_Z char const *attr
#define ATTR_ARG_CHECK attr
#else
#define ATTR_ARG       DEE_A_IN_OBJECT(DeeStringObject) const *attr
#define ATTR_ARG_CHECK DeeObject_Check(attr)&&DeeString_Check(attr)
#endif

#ifdef STRING
DEE_A_RET_EXCEPT_REF DeeObject *DeeInstance_TGetAttrString
#else
DEE_A_RET_EXCEPT_REF DeeObject *DeeInstance_TGetAttr
#endif
(DEE_A_IN_TYPEOBJECT(DeeClassObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, ATTR_ARG) {
 struct _DeeClassTypeMethodDef *managed_methods;
 struct _DeeClassTypeGetSetDef *managed_getsets;
 struct DeeMemberDef const *members;
 struct DeeGetSetDef const *getsets;
 struct DeeMethodDef const *methods;
 struct DeeClassSuffix *suffix; DeeObject *result,*args;
 DeeTypeObject const *tp_iter; struct DeeInstanceLevel *level;
 int temp; Dee_size_t attr_len; Dee_hash_t bucket;
#ifndef STRING
 char const *attr_str;
#else
 DeeObject *attr_ob;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeClass_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(ATTR_ARG_CHECK);
#ifndef STRING
 attr_str = DeeString_STR(attr),attr_len = DeeString_SIZE(attr);
#else
 attr_len = strlen(attr);
#endif
 bucket = DeeHash_StringWithLength(attr_len,attr_str,DEE_HASH_START);
 tp_iter = tp_self; do {
  suffix = DeeClass_SUFFIX(tp_iter);
  // Step #1: Check for managed methods/getsets
  managed_methods = suffix->cs_methods;
  DEE_ASSERT(managed_methods);
  while (managed_methods->ctmd_name) {
   DEE_ASSERT(DeeString_Check(managed_methods->ctmd_name));
   if (DeeString_SIZE(managed_methods->ctmd_name) == attr_len &&
       memcmp(DeeString_STR(managed_methods->ctmd_name),attr_str,attr_len*sizeof(char)) == 0
       ) return DeeInstanceMethod_New(self,managed_methods->ctmd_func); // Found it!
   ++managed_methods;
  }
  managed_getsets = suffix->cs_getsets;
  DEE_ASSERT(managed_getsets);
  while (managed_getsets->ctgd_name) {
   DEE_ASSERT(DeeString_Check(managed_getsets->ctgd_name));
   if (DeeString_SIZE(managed_getsets->ctgd_name) == attr_len &&
       memcmp(DeeString_STR(managed_getsets->ctgd_name),attr_str,attr_len*sizeof(char)) == 0) {
    if (!managed_getsets->ctgd_get) goto no_r;
    return DeeObject_ThisCall(managed_getsets->ctgd_get,self,Dee_EmptyTuple);
   }
   ++managed_getsets;
  }
  // Step #2: Check for member variables
  level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
#ifdef STRING
  if ((temp = DeeHashMap_GetItemExStringWithKnownBucketAndLock(
   &level->il_members,attr,bucket,&result,&level->il_lock)) == -1) return NULL;
#else
  if ((temp = DeeHashMap_GetItemExWithKnownBucketAndLock(
   &level->il_members,(DeeObject *)attr,bucket,&result,&level->il_lock)) == -1) return NULL;
#endif
  if (temp == 0) return result;
  // Step #3: Check for unmanaged members/getsets/methods
  members = DeeType_GET_SLOT(tp_iter,tp_members);
  DEE_ASSERT(members);
  while (members->d_name) {
   if (strcmp(members->d_name,attr_str) == 0) {
    if ((members->d_access&DeeMemberAccess_READ) != 0) return DeeMember_Get(self,members);
    goto no_r;
   }
   ++members;
  }
  getsets = DeeType_GET_SLOT(tp_iter,tp_getsets);
  DEE_ASSERT(getsets);
  while (getsets->d_name) {
   if (strcmp(getsets->d_name,attr_str) == 0) {
    if (getsets->d_get) return (getsets->d_flags&DEE_GETSETDEF_FLAG_STRUCT)!=0
     ? (*DeeGetSetDef_SGET(getsets))((DeeStructuredTypeObject *)Dee_TYPE(self),DeeStructured_DATA(self),getsets->d_closure)
     : (*DeeGetSetDef_OGET(getsets))(self,getsets->d_closure);
no_r: // no_read
    DeeError_AttributeError_NoRead(tp_self,attr_str);
    return NULL;
   }
   ++getsets;
  }
  methods = DeeType_GET_SLOT(tp_iter,tp_methods);
  DEE_ASSERT(methods);
  while (methods->d_name) {
   if (strcmp(methods->d_name,attr_str) == 0) {
    return (methods->d_flags&DEE_METHODDEF_FLAG_STRUCT)!=0
     ? DeeMemberFunction_NewStructString(Dee_TYPE(self),DeeStructured_DATA(self),DeeMethodDef_SFUNC(methods),methods->d_closure,methods->d_name)
     : DeeMemberFunction_NewObjectString(self,DeeMethodDef_OFUNC(methods),methods->d_closure,methods->d_name);
   }
   ++methods;
  }
  tp_iter = DeeType_BASE(tp_iter);
 } while (DeeClass_Check(tp_iter));
 // Step #4: Check unmanaged/generic attributes
 if ((temp = _DeeObject_TGenericGetAttrString(tp_iter,self,attr_str,&result)) != 1)
  return temp == -1 ? NULL : result;
 // Step #5: Check for managed __getattr__ operator
 tp_iter = tp_self; do {
  DeeObject *getattr_callback;
  suffix = DeeClass_SUFFIX(tp_iter);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
  if ((getattr_callback = suffix->cs_attr.ctp_attr_get) != NULL)
#else
  if ((getattr_callback = DeeClassSuffix_GetVirtOperator(
   suffix,DeeType_SLOT_ID(tp_attr_get))) != NULL)
#endif
  {
#ifdef STRING
   if ((attr_ob = DeeString_New(attr)) == NULL) return NULL;
   args = DeeTuple_Pack(1,attr_ob);
   Dee_DECREF(attr_ob);
   if (!args) return NULL;
#else
   if ((args = DeeTuple_Pack(1,attr)) == NULL) return NULL;
#endif
   result = DeeObject_ThisCall(getattr_callback,self,args);
   Dee_DECREF(args);
   return result;
  }
  tp_iter = DeeType_BASE(tp_iter);
 } while (DeeClass_Check(tp_iter));
 // Step #6: Attribute not found
 DeeError_AttributeError_NotFound(tp_self,attr_str);
 return NULL;
}



#ifdef STRING
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeInstance_THasAttrString
#else
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeInstance_THasAttr
#endif
(DEE_A_IN_TYPEOBJECT(DeeClassObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, ATTR_ARG) {
 struct _DeeClassTypeMethodDef *managed_methods;
 struct _DeeClassTypeGetSetDef *managed_getsets;
 struct DeeMemberDef const *members;
 struct DeeGetSetDef const *getsets;
 struct DeeMethodDef const *methods;
 struct DeeClassSuffix *suffix; DeeObject *result,*args;
 DeeTypeObject const *tp_iter; struct DeeInstanceLevel *level;
 int temp; Dee_size_t attr_len; Dee_hash_t bucket;
#ifndef STRING
 char const *attr_str;
#else
 DeeObject *attr_ob;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeClass_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(ATTR_ARG_CHECK);
#ifndef STRING
 attr_str = DeeString_STR(attr),attr_len = DeeString_SIZE(attr);
#else
 attr_len = strlen(attr);
#endif
 bucket = DeeHash_StringWithLength(attr_len,attr_str,DEE_HASH_START);
 tp_iter = tp_self; do {
  suffix = DeeClass_SUFFIX(tp_iter);
  // Step #1: Check for managed methods/getsets
  managed_methods = suffix->cs_methods;
  DEE_ASSERT(managed_methods);
  while (managed_methods->ctmd_name) {
   DEE_ASSERT(DeeString_Check(managed_methods->ctmd_name));
   if (DeeString_SIZE(managed_methods->ctmd_name) == attr_len &&
       memcmp(DeeString_STR(managed_methods->ctmd_name),attr_str,attr_len*sizeof(char)) == 0
       ) return 1; // Found it!
   ++managed_methods;
  }
  managed_getsets = suffix->cs_getsets;
  DEE_ASSERT(managed_getsets);
  while (managed_getsets->ctgd_name) {
   DEE_ASSERT(DeeString_Check(managed_getsets->ctgd_name));
   if (DeeString_SIZE(managed_getsets->ctgd_name) == attr_len &&
       memcmp(DeeString_STR(managed_getsets->ctgd_name),attr_str,attr_len*sizeof(char)) == 0
       ) return managed_getsets->ctgd_get != NULL;
   ++managed_getsets;
  }
  // Step #2: Check for member variables
  level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
#ifdef STRING
  if ((temp = DeeHashMap_HasKeyStringWithKnownBucketAndLock(
   &level->il_members,attr,bucket,&level->il_lock)) != 0) return temp;
#else
  if ((temp = DeeHashMap_HasKeyWithKnownBucketAndLock(
   &level->il_members,(DeeObject *)attr,bucket,&level->il_lock)) != 0) return temp;
#endif
  // Step #3: Check for unmanaged members/getsets/methods
  members = DeeType_GET_SLOT(tp_iter,tp_members),DEE_ASSERT(members);
  while (members->d_name) { if (strcmp(members->d_name,attr_str) == 0) return 1; ++members; }
  getsets = DeeType_GET_SLOT(tp_iter,tp_getsets),DEE_ASSERT(getsets);
  while (getsets->d_name) { if (strcmp(getsets->d_name,attr_str) == 0) return 1; ++getsets; }
  methods = DeeType_GET_SLOT(tp_iter,tp_methods),DEE_ASSERT(methods);
  while (methods->d_name) { if (strcmp(methods->d_name,attr_str) == 0) return 1; ++methods; }
  tp_iter = DeeType_BASE(tp_iter); // Try the next type
 } while (DeeClass_Check(tp_iter));
 // Step #4: Check unmanaged/generic attributes
 if (_DeeObject_TGenericHasAttrString(tp_iter,self,attr_str)) return 1;
 // Step #5: Check for managed __getattr__ operator
 tp_iter = tp_self; do {
  DeeObject *getattr_callback;
  suffix = DeeClass_SUFFIX(tp_iter);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
  if ((getattr_callback = suffix->cs_attr.ctp_attr_get) != NULL)
#else
  if ((getattr_callback = DeeClassSuffix_GetVirtOperator(
   suffix,DeeType_SLOT_ID(tp_attr_get))) != NULL)
#endif
  {
#ifdef STRING
   if ((attr_ob = DeeString_New(attr)) == NULL) return -1;
   args = DeeTuple_Pack(1,attr_ob);
   Dee_DECREF(attr_ob);
   if (!args) return -1;
#else
   if ((args = DeeTuple_Pack(1,attr)) == NULL) return -1;
#endif
   result = DeeObject_ThisCall(getattr_callback,self,args);
   Dee_DECREF(args);
   if (result) { Dee_DECREF(result); return 1; }
   if DEE_UNLIKELY(!result &&
                   !DeeError_Catch(&DeeErrorType_AttributeError) &&
                   !DeeError_Catch(&DeeErrorType_NotImplemented)
                   ) return -1;
  }
  tp_iter = DeeType_BASE(tp_iter); // Try the next type
 } while (DeeClass_Check(tp_iter));
 // Step #6: Attribute not found
 return 0;
}



#ifdef STRING
DEE_A_RET_EXCEPT(-1) int DeeInstance_TDelAttrString
#else
DEE_A_RET_EXCEPT(-1) int DeeInstance_TDelAttr
#endif
(DEE_A_IN_TYPEOBJECT(DeeClassObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, ATTR_ARG) {
 struct _DeeClassTypeGetSetDef *managed_getsets;
 struct DeeGetSetDef const *getsets;
 struct DeeClassSuffix *suffix; DeeObject *result,*args;
 DeeTypeObject const *tp_iter; struct DeeInstanceLevel *level;
 int temp; Dee_size_t attr_len; Dee_hash_t bucket;
#ifndef STRING
 char const *attr_str;
#else
 DeeObject *attr_ob;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeClass_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(ATTR_ARG_CHECK);
#ifndef STRING
 attr_str = DeeString_STR(attr),attr_len = DeeString_SIZE(attr);
#else
 attr_len = strlen(attr);
#endif
 bucket = DeeHash_StringWithLength(attr_len,attr_str,DEE_HASH_START);
 tp_iter = tp_self; do {
  suffix = DeeClass_SUFFIX(tp_iter);
  // Step #1: Check for managed getsets
  managed_getsets = suffix->cs_getsets;
  DEE_ASSERT(managed_getsets);
  while (managed_getsets->ctgd_name) {
   DEE_ASSERT(DeeString_Check(managed_getsets->ctgd_name));
   if (DeeString_SIZE(managed_getsets->ctgd_name) == attr_len &&
       memcmp(DeeString_STR(managed_getsets->ctgd_name),attr_str,attr_len*sizeof(char)) == 0) {
    if (!managed_getsets->ctgd_del) goto no_del;
    result = DeeObject_ThisCall(managed_getsets->ctgd_del,self,Dee_EmptyTuple);
    if (!result) return -1;
    Dee_DECREF(result);
    return 0;
   }
   ++managed_getsets;
  }
  // Step #2: Check for member variables
  level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
#ifdef STRING
  if ((temp = DeeHashMap_DelItemStringWithKnownBucketAndLock(
   &level->il_members,attr,bucket,&level->il_lock)) != 1) return temp;
#else
  if ((temp = DeeHashMap_DelItemWithKnownBucketAndLock(
   &level->il_members,(DeeObject *)attr,bucket,&level->il_lock)) != 1) return temp;
#endif
  // Step #3: Check for unmanaged getsets
  getsets = DeeType_GET_SLOT(tp_iter,tp_getsets);
  DEE_ASSERT(getsets);
  while (getsets->d_name) {
   if (strcmp(getsets->d_name,attr_str) == 0) {
    if (getsets->d_del) return (getsets->d_flags&DEE_GETSETDEF_FLAG_STRUCT)!=0
     ? (*DeeGetSetDef_SDEL(getsets))((DeeStructuredTypeObject *)Dee_TYPE(self),DeeStructured_DATA(self),getsets->d_closure)
     : (*DeeGetSetDef_ODEL(getsets))(self,getsets->d_closure);
no_del: // no_read
    DeeError_AttributeError_NoDel(tp_self,attr_str);
    return -1;
   }
   ++getsets;
  }
  tp_iter = DeeType_BASE(tp_iter); // Try the next type
 } while (DeeClass_Check(tp_iter));
 // Step #4: Check unmanaged/generic attributes
 if ((temp = _DeeObject_TGenericDelAttrString(tp_iter,self,attr_str)) != 1) return temp;
 // Step #5: Check for managed __delattr__ operator
 tp_iter = tp_self; do {
  DeeObject *delattr_callback;
  suffix = DeeClass_SUFFIX(tp_iter);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
  if ((delattr_callback = suffix->cs_attr.ctp_attr_del) != NULL)
#else
  if ((delattr_callback = DeeClassSuffix_GetVirtOperator(
   suffix,DeeType_SLOT_ID(tp_attr_del))) != NULL)
#endif
  {
#ifdef STRING
   if ((attr_ob = DeeString_New(attr)) == NULL) return -1;
   args = DeeTuple_Pack(1,attr_ob);
   Dee_DECREF(attr_ob);
   if (!args) return -1;
#else
   if ((args = DeeTuple_Pack(1,attr)) == NULL) return -1;
#endif
   result = DeeObject_ThisCall(delattr_callback,self,args);
   Dee_DECREF(args);
   if (!result) return -1;
   Dee_DECREF(result);
   return 0;
  }
  tp_iter = DeeType_BASE(tp_iter); // Try the next type
 } while (DeeClass_Check(tp_iter));
 // Step #6: Attribute not found
 DeeError_AttributeError_NotFound(tp_self,attr_str);
 return -1;
}



#ifdef STRING
DEE_A_RET_EXCEPT(-1) int DeeInstance_TSetAttrString
#else
DEE_A_RET_EXCEPT(-1) int DeeInstance_TSetAttr
#endif
(DEE_A_IN_TYPEOBJECT(DeeClassObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeInstanceObject) *self,
 ATTR_ARG, DEE_A_INOUT DeeObject *v) {
 struct _DeeClassTypeGetSetDef *managed_getsets;
 struct DeeMemberDef const *members;
 struct DeeGetSetDef const *getsets;
 struct DeeClassSuffix *suffix; DeeObject *result,*args;
 DeeTypeObject const *tp_iter; struct DeeInstanceLevel *level;
 int temp; Dee_size_t attr_len; Dee_hash_t bucket;
#ifndef STRING
 char const *attr_str;
#else
 DeeObject *attr_ob;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeClass_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(ATTR_ARG_CHECK);
 DEE_ASSERT(DeeObject_Check(v));
#ifndef STRING
 attr_str = DeeString_STR(attr),attr_len = DeeString_SIZE(attr);
#else
 attr_len = strlen(attr);
#endif
 bucket = DeeHash_StringWithLength(attr_len,attr_str,DEE_HASH_START);
 tp_iter = tp_self; do {
  suffix = DeeClass_SUFFIX(tp_iter);
  // Step #1: Check for managed getsets
  managed_getsets = suffix->cs_getsets;
  DEE_ASSERT(managed_getsets);
  while (managed_getsets->ctgd_name) {
   DEE_ASSERT(DeeString_Check(managed_getsets->ctgd_name));
   if (DeeString_SIZE(managed_getsets->ctgd_name) == attr_len &&
       memcmp(DeeString_STR(managed_getsets->ctgd_name),attr_str,attr_len*sizeof(char)) == 0) {
    if (!managed_getsets->ctgd_set) goto no_w;
    if ((args = DeeTuple_Pack(1,v)) == NULL) return -1;
    result = DeeObject_ThisCall(managed_getsets->ctgd_set,self,args);
    Dee_DECREF(args);
    if (!result) return -1;
    Dee_DECREF(result);
    return 0;
   }
   ++managed_getsets;
  }
  // Step #2: Check for member variables
  level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
#ifdef STRING
  temp = DeeHashMap_SetOldItemStringWithKnownBucketAndLock(
   &level->il_members,attr,bucket,v,NULL,&level->il_lock);
#else
  temp = DeeHashMap_SetOldItemWithKnownBucketAndLock(
   &level->il_members,(DeeObject *)attr,bucket,v,NULL,&level->il_lock);
#endif
  if (temp != 1) return temp;
  // Step #3: Check for unmanaged members/getsets/methods
  members = DeeType_GET_SLOT(tp_iter,tp_members);
  DEE_ASSERT(members);
  while (members->d_name) {
   if (strcmp(members->d_name,attr_str) == 0) {
#if !DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS
    if ((members->d_access&DeeMemberAccess_WRITE) != 0)
#endif /* !DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS */
    {
     return DeeMember_Set(self,members,v);
    }
#if !DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS
    goto no_w;
#endif /* !DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS */
   }
   ++members;
  }
  getsets = DeeType_GET_SLOT(tp_iter,tp_getsets);
  DEE_ASSERT(getsets);
  while (getsets->d_name) {
   if (strcmp(getsets->d_name,attr_str) == 0) {
    if (getsets->d_set) return (getsets->d_flags&DEE_GETSETDEF_FLAG_STRUCT)!=0
     ? (*DeeGetSetDef_SSET(getsets))((DeeStructuredTypeObject *)Dee_TYPE(self),DeeStructured_DATA(self),v,getsets->d_closure)
     : (*DeeGetSetDef_OSET(getsets))(self,v,getsets->d_closure);
no_w: // no_write
    DeeError_AttributeError_NoWrite(tp_self,attr_str);
    return -1;
   }
   ++getsets;
  }
  tp_iter = DeeType_BASE(tp_iter); // Try the next type
 } while (DeeClass_Check(tp_iter));
 // Step #4: Check unmanaged/generic attributes
 if ((temp = _DeeObject_TGenericSetAttrString(tp_iter,self,attr_str,v)) != 1) return temp;
 // Step #5: Check for managed __setattr__ operator
 tp_iter = tp_self; do {
  DeeObject *setattr_callback;
  suffix = DeeClass_SUFFIX(tp_iter);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
  if ((setattr_callback = suffix->cs_attr.ctp_attr_set) != NULL)
#else
  if ((setattr_callback = DeeClassSuffix_GetVirtOperator(
   suffix,DeeType_SLOT_ID(tp_attr_set))) != NULL)
#endif
  {
#ifdef STRING
   if ((attr_ob = DeeString_New(attr)) == NULL) return -1;
   args = DeeTuple_Pack(2,attr_ob,v);
   Dee_DECREF(attr_ob);
   if (!args) return -1;
#else
   if ((args = DeeTuple_Pack(2,attr,v)) == NULL) return -1;
#endif
   result = DeeObject_ThisCall(setattr_callback,self,args);
   Dee_DECREF(args);
   if (!result) return -1;
   Dee_DECREF(result);
   return 0;
  }
  tp_iter = DeeType_BASE(tp_iter); // Try the next type
 } while (DeeClass_Check(tp_iter));
 // Step #6: Create new member variable in 'tp_self@self'
 level = DeeInstance_MEMBERS_EX(tp_self,self);
#ifdef STRING
 return DeeHashMap_SetItemStringWithKnownBucketAndLock(
  &level->il_members,attr,bucket,v,&level->il_lock);
#else
 return DeeHashMap_SetItemWithKnownBucketAndLock(
  &level->il_members,(DeeObject *)attr,bucket,v,&level->il_lock);
#endif
}



#ifdef STRING
DEE_A_RET_EXCEPT_REF DeeObject *DeeInstance_TCallAttrString
#else
DEE_A_RET_EXCEPT_REF DeeObject *DeeInstance_TCallAttr
#endif
(DEE_A_IN_TYPEOBJECT(DeeClassObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, ATTR_ARG,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 struct _DeeClassTypeMethodDef *managed_methods;
 struct _DeeClassTypeGetSetDef *managed_getsets;
 struct DeeMemberDef const *members;
 struct DeeGetSetDef const *getsets;
 struct DeeMethodDef const *methods;
 struct DeeClassSuffix *suffix; DeeObject *callback,*result,*temp_args;
 DeeTypeObject const *tp_iter; struct DeeInstanceLevel *level;
 int temp; Dee_size_t attr_len; Dee_hash_t bucket;
#ifndef STRING
 char const *attr_str;
#else
 DeeObject *attr_ob;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeInstance_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeClass_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(ATTR_ARG_CHECK);
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
#ifndef STRING
 attr_str = DeeString_STR(attr),attr_len = DeeString_SIZE(attr);
#else
 attr_len = strlen(attr);
#endif
 bucket = DeeHash_StringWithLength(attr_len,attr_str,DEE_HASH_START);
 tp_iter = tp_self; do {
  suffix = DeeClass_SUFFIX(tp_iter);
  // Step #1: Check for managed methods/getsets
  managed_methods = suffix->cs_methods;
  DEE_ASSERT(managed_methods);
  while (managed_methods->ctmd_name) {
   DEE_ASSERT(DeeString_Check(managed_methods->ctmd_name));
   if (DeeString_SIZE(managed_methods->ctmd_name) == attr_len &&
       memcmp(DeeString_STR(managed_methods->ctmd_name),attr_str,attr_len*sizeof(char)) == 0
       ) return DeeObject_ThisCall(managed_methods->ctmd_func,self,args); // Found it!
   ++managed_methods;
  }
  managed_getsets = suffix->cs_getsets;
  DEE_ASSERT(managed_getsets);
  while (managed_getsets->ctgd_name) {
   DEE_ASSERT(DeeString_Check(managed_getsets->ctgd_name));
   if (DeeString_SIZE(managed_getsets->ctgd_name) == attr_len &&
       memcmp(DeeString_STR(managed_getsets->ctgd_name),attr_str,attr_len*sizeof(char)) == 0) {
    if (!managed_getsets->ctgd_get) goto no_r;
    callback = DeeObject_ThisCall(managed_getsets->ctgd_get,self,Dee_EmptyTuple);
check_and_run_cb:
    if (!callback) return NULL;
run_cb:
    result = DeeObject_Call(callback,args);
    Dee_DECREF(callback);
    return result;
   }
   ++managed_getsets;
  }
  // Step #2: Check for member variables
  level = DeeInstance_MEMBERS_AT(self,suffix->cs_member_offset);
#ifdef STRING
  if ((temp = DeeHashMap_GetItemExStringWithKnownBucketAndLock(
   &level->il_members,attr,bucket,&callback,&level->il_lock)) == -1) return NULL;
#else
  if ((temp = DeeHashMap_GetItemExWithKnownBucketAndLock(
   &level->il_members,(DeeObject *)attr,bucket,&callback,&level->il_lock)) == -1) return NULL;
#endif
  if (temp == 0) goto run_cb;
  // Step #3: Check for unmanaged members/getsets/methods
  members = DeeType_GET_SLOT(tp_iter,tp_members);
  DEE_ASSERT(members);
  while (members->d_name) {
   if (strcmp(members->d_name,attr_str) == 0) {
    if ((members->d_access&DeeMemberAccess_READ) != 0) {
     callback = DeeMember_Get(self,members);
     goto check_and_run_cb;
    }
    goto no_r;
   }
   ++members;
  }
  getsets = DeeType_GET_SLOT(tp_iter,tp_getsets);
  DEE_ASSERT(getsets);
  while (getsets->d_name) {
   if (strcmp(getsets->d_name,attr_str) == 0) {
    if (getsets->d_get) {
     callback = (getsets->d_flags&DEE_GETSETDEF_FLAG_STRUCT)!=0
      ? (*DeeGetSetDef_SGET(getsets))((DeeStructuredTypeObject *)Dee_TYPE(self),DeeStructured_DATA(self),getsets->d_closure)
      : (*DeeGetSetDef_OGET(getsets))(self,getsets->d_closure);
     goto check_and_run_cb;
    }
no_r: // no_read
    DeeError_AttributeError_NoRead(tp_self,attr_str);
    return NULL;
   }
   ++getsets;
  }
  methods = DeeType_GET_SLOT(tp_iter,tp_methods);
  DEE_ASSERT(methods);
  while (methods->d_name) {
   if (strcmp(methods->d_name,attr_str) == 0) {
    return (methods->d_flags&DEE_METHODDEF_FLAG_STRUCT)!=0
     ? (*DeeMethodDef_SFUNC(methods))((DeeStructuredTypeObject *)Dee_TYPE(self),DeeStructured_DATA(self),args,methods->d_closure)
     : (*DeeMethodDef_OFUNC(methods))(self,args,methods->d_closure);
   }
   ++methods;
  }
  tp_iter = DeeType_BASE(tp_iter); // Try the next type
 } while (DeeClass_Check(tp_iter));
 // Step #4: Check unmanaged/generic attributes
 if ((temp = _DeeObject_TGenericCallAttrString(tp_iter,self,attr_str,args,&result)) != 1)
  return temp == -1 ? NULL : result;
 // Step #5: Check for managed __getattr__ operator
 tp_iter = tp_self; do {
  DeeObject *getattr_callback;
  suffix = DeeClass_SUFFIX(tp_iter);
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
  if ((getattr_callback = suffix->cs_attr.ctp_attr_get) != NULL)
#else
  if ((getattr_callback = DeeClassSuffix_GetVirtOperator(
   suffix,DeeType_SLOT_ID(tp_attr_get))) != NULL)
#endif
  {
#ifdef STRING
   if ((attr_ob = DeeString_New(attr)) == NULL) return NULL;
   temp_args = DeeTuple_Pack(1,attr_ob);
   Dee_DECREF(attr_ob);
   if (!temp_args) return NULL;
#else
   if ((temp_args = DeeTuple_Pack(1,attr)) == NULL) return NULL;
#endif
   callback = DeeObject_ThisCall(getattr_callback,self,temp_args);
   Dee_DECREF(temp_args);
   goto check_and_run_cb;
  }
  tp_iter = DeeType_BASE(tp_iter); // Try the next type
 } while (DeeClass_Check(tp_iter));
 // Step #6: Attribute not found
 DeeError_AttributeError_NotFound(tp_self,attr_str);
 return NULL;
}

#ifdef STRING
#undef STRING
#endif
#undef attr_str
#undef ATTR_ARG
#undef ATTR_ARG_CHECK
#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
