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
#include "object.generic_attribute.inl"
DEE_DECL_BEGIN
#define STRUCTURED
#endif

#ifdef STRUCTURED
DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeObject_PGenericGetAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN void *self,
 DEE_A_IN_Z char const *name, DEE_A_REF DEE_A_OUT DeeObject **result)
#else
#ifdef CONST_ONLY
DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeObject_TGenericGetConstAttrString
#else
DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeObject_TGenericGetAttrString
#endif
(DEE_A_IN DeeTypeObject const *tp_self, 
 DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *name,
 DEE_A_REF DEE_A_OUT DeeObject **result)
#endif
{
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
 struct DeeGenericAttributeCache *cache;
#endif
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
#ifdef STRUCTURED
 DEE_ASSERT(self);
#else
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
#endif
 tp_iter = tp_self;
 while (1) {
  struct DeeMemberDef const *members;
  struct DeeGetSetDef const *getsets;
  struct DeeMethodDef const *methods;
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
  cache = DEE_ATTRIBUTE_CACHE_STRING(tp_iter,name);
  DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_READ(cache) {
   if (cache->gac_self == tp_iter && cache->gac_name == name) {
    switch (cache->gac_type) {
     case DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_MEMBER:
      members = cache->gac_member;
      DEE_ATTRIBUTE_CACHE_RELEASE(cache);
      DEE_ASSERT(members);
      goto use_member;
     case DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_GETSET:
      getsets = cache->gac_getset;
      DEE_ATTRIBUTE_CACHE_RELEASE(cache);
      DEE_ASSERT(getsets);
#ifdef STRUCTURED
      if ((getsets->d_flags&DEE_GETSETDEF_FLAG_STRUCT)==0) goto end_cache;
#endif
      goto use_getset;
     case DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_METHOD:
      methods = cache->gac_method;
      DEE_ATTRIBUTE_CACHE_RELEASE(cache);
      DEE_ASSERT(methods);
#ifdef STRUCTURED
      if ((methods->d_flags&DEE_METHODDEF_FLAG_STRUCT)==0) goto end_cache;
#endif
      goto use_method;
     default: break;
    }
   }
   DEE_ATTRIBUTE_CACHE_RELEASE(cache);
  }
#ifdef STRUCTURED
end_cache:
#endif
#endif
  members = DeeType_GET_SLOT(tp_iter,tp_members);
  DEE_ASSERT(members);
  while (members->d_name) {
   if (strcmp(members->d_name,name) == 0) {
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
    DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_WRITE(cache) {
     DeeGenericAttributeCache_SET_MEMBER(cache,tp_iter,name,members);
     DEE_ATTRIBUTE_CACHE_RELEASE(cache);
    }
use_member:
#endif
    if ((members->d_access&DeeMemberAccess_READ) != 0) {
#ifdef STRUCTURED
     if ((*result = DeeMember_Get(((
      DeeObject *)self)-1,members)) == NULL) return -1;
#else
     if ((*result = DeeMember_Get(self,members)) == NULL) return -1;
#endif
     return 0;
    }
    goto no_r;
   }
   ++members;
  }
  getsets = DeeType_GET_SLOT(tp_iter,tp_getsets);
  DEE_ASSERT(getsets);
  while (getsets->d_name) {
#ifdef STRUCTURED
   if ((getsets->d_flags&DEE_GETSETDEF_FLAG_STRUCT)!=0)
#endif
   if (strcmp(getsets->d_name,name) == 0) {
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
    DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_WRITE(cache) {
     DeeGenericAttributeCache_SET_GETSET(cache,tp_iter,name,getsets);
     DEE_ATTRIBUTE_CACHE_RELEASE(cache);
    }
use_getset:
#endif
    if (getsets->d_get) {
#ifdef CONST_ONLY
     if ((getsets->d_flags&DEE_GETSETDEF_FLAG_CONST)==0) return 1;
#endif
#ifdef STRUCTURED
     if ((*result = (*DeeGetSetDef_SGET(getsets))((DeeStructuredTypeObject *)
      tp_iter,self,getsets->d_closure)) == NULL) return -1;
#else
     if ((*result = (getsets->d_flags&DEE_GETSETDEF_FLAG_STRUCT)!=0
      ? (*DeeGetSetDef_SGET(getsets))((DeeStructuredTypeObject *)Dee_TYPE(self),DeeStructured_DATA(self),getsets->d_closure)
      : (*DeeGetSetDef_OGET(getsets))(self,getsets->d_closure)) == NULL) return -1;
#endif
     return 0;
    }
no_r: // no-read
    DeeError_AttributeError_NoRead(tp_self,name);
    return -1;
   }
   ++getsets;
  }
  methods = DeeType_GET_SLOT(tp_iter,tp_methods);
  DEE_ASSERT(methods);
  while (methods->d_name) {
#ifdef STRUCTURED
   if ((methods->d_flags&DEE_METHODDEF_FLAG_STRUCT)!=0)
#endif
   if (strcmp(methods->d_name,name) == 0) {
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
    DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_WRITE(cache) {
     DeeGenericAttributeCache_SET_METHOD(cache,tp_iter,name,methods);
     DEE_ATTRIBUTE_CACHE_RELEASE(cache);
    }
use_method:
#endif
#ifdef CONST_ONLY
    if ((methods->d_flags&DEE_METHODDEF_FLAG_CONST)==0) return 1;
#endif
#ifdef STRUCTURED
    if ((*result = DeeMemberFunction_NewStructString(
     (DeeTypeObject *)tp_iter,self,DeeMethodDef_SFUNC(methods),
     methods->d_closure,methods->d_name)) == NULL) return -1;
#else
    if ((*result = (methods->d_flags&DEE_METHODDEF_FLAG_STRUCT)!=0
     ? DeeMemberFunction_NewStructString(Dee_TYPE(self),DeeStructured_DATA(self),DeeMethodDef_SFUNC(methods),methods->d_closure,methods->d_name)
     : DeeMemberFunction_NewObjectString(self,DeeMethodDef_OFUNC(methods),methods->d_closure,methods->d_name)
     ) == NULL) return -1;
#endif
    return 0;
   }
   ++methods;
  }
  if ((tp_iter = DeeType_BASE(tp_iter)) == NULL) break;
 }
 return 1;
}

#ifndef STRUCTURED
#ifdef CONST_ONLY
DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeType_GetConstAttrString
#else
DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeType_GetAttrString
#endif
(DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z char const *name,
 DEE_A_REF DEE_A_OUT DeeObject **result) {
 struct DeeMemberDef const *members;
 struct DeeGetSetDef const *getsets;
 struct DeeMethodDef const *methods;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 while (1) {
  int temp;
  // v default type attributes ("__name__", ...)
  //   >> s.a.: "_deetype_tp_members"
#ifdef CONST_ONLY
  if ((temp = _DeeObject_GenericGetConstAttrString((DeeObject *)self,name,result)) != 1) return temp;
#else
  if ((temp = _DeeObject_GenericGetAttrString((DeeObject *)self,name,result)) != 1) return temp;
#endif
  members = DeeType_GET_SLOT(self,tp_class_members);
  DEE_ASSERT(members);
  while (members->d_name) {
   if (strcmp(members->d_name,name) == 0) {
    if ((members->d_access&DeeMemberAccess_READ) != 0) {
     if ((*result = DeeMember_Get((DeeObject *)self,members)) == NULL) return -1;
     return 0;
    }
    goto no_r;
   }
   ++members;
  }
  getsets = DeeType_GET_SLOT(self,tp_class_getsets);
  DEE_ASSERT(getsets);
  while (getsets->d_name) {
   if (strcmp(getsets->d_name,name) == 0) {
    if (getsets->d_get) {
#ifdef CONST_ONLY
     if ((getsets->d_flags&DEE_GETSETDEF_FLAG_CONST)==0) return 1;
#endif
     if ((*result = (getsets->d_flags&DEE_GETSETDEF_FLAG_STRUCT)!=0
      ? (*DeeGetSetDef_SGET(getsets))((DeeStructuredTypeObject *)Dee_TYPE(self),DeeStructured_DATA(self),getsets->d_closure)
      : (*DeeGetSetDef_OGET(getsets))((DeeObject *)self,getsets->d_closure)) == NULL) return -1;
     return 0;
    }
no_r: // no-read
    DeeError_AttributeError_NoRead(Dee_TYPE(self),name);
    return -1;
   }
   ++getsets;
  }
  methods = DeeType_GET_SLOT(self,tp_class_methods);
  DEE_ASSERT(methods);
  while (methods->d_name) {
   if (strcmp(methods->d_name,name) == 0) {
#ifdef CONST_ONLY
    if ((methods->d_flags&DEE_METHODDEF_FLAG_CONST)==0) return 1;
#endif
    if ((*result = (methods->d_flags&DEE_METHODDEF_FLAG_STRUCT)!=0
     ? DeeMemberFunction_NewStructString(Dee_TYPE(self),DeeStructured_DATA(self),DeeMethodDef_SFUNC(methods),methods->d_closure,methods->d_name)
     : DeeMemberFunction_NewObjectString((DeeObject *)self,DeeMethodDef_OFUNC(methods),methods->d_closure,methods->d_name)
     ) == NULL) return -1;
    return 0;
   }
   ++methods;
  }
  if ((self = DeeType_BASE(self)) == NULL) break;
 }
 return 1;
}
#endif

#ifdef STRUCTURED
#undef STRUCTURED
#endif
#ifdef CONST_ONLY
#undef CONST_ONLY
#endif

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
