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

//////////////////////////////////////////////////////////////////////////
#ifdef STRUCTURED
DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeObject_PGenericCallAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN void *self,
 DEE_A_IN_Z char const *name, DEE_A_INOUT_OBJECT(DeeTupleObject) *args,
 DEE_A_REF DEE_A_OUT DeeObject **result)
#else
DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeObject_TGenericCallAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *name,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args, DEE_A_REF DEE_A_OUT DeeObject **result)
#endif
{
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
 struct DeeGenericAttributeCache *cache;
#endif
 DeeTypeObject const *tp_iter;
 DeeObject *temp;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
#ifndef STRUCTURED
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
#endif
 DEE_ASSERT(name && result);
 tp_iter = tp_self; while (1) {
  struct DeeMethodDef const *methods;
  struct DeeGetSetDef const *getsets;
  struct DeeMemberDef const *members;
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
  methods = DeeType_GET_SLOT(tp_iter,tp_methods);
  DEE_ASSERT(methods);
  while (methods->d_name) {
#ifdef STRUCTURED
   if ((methods->d_flags&DEE_METHODDEF_FLAG_STRUCT)!=0)
#endif
   if DEE_UNLIKELY(strcmp(methods->d_name,name) == 0) {
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
    DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_WRITE(cache) {
     DeeGenericAttributeCache_SET_METHOD(cache,tp_iter,name,methods);
     DEE_ATTRIBUTE_CACHE_RELEASE(cache);
    }
use_method:
#endif
    DEE_ASSERT(methods->d_func);
#ifdef STRUCTURED
    if DEE_UNLIKELY((*result = (*DeeMethodDef_SFUNC(methods))((
     DeeStructuredTypeObject *)tp_self,self,args,methods->d_closure)) == NULL) return -1;
#else
    if ((*result = DEE_UNLIKELY((methods->d_flags&DEE_METHODDEF_FLAG_STRUCT)!=0)
     ? (*DeeMethodDef_SFUNC(methods))((DeeStructuredTypeObject *)
          tp_self,DeeStructured_DATA(self),args,methods->d_closure)
     : (*DeeMethodDef_OFUNC(methods))(self,args,methods->d_closure)) == NULL) return -1;
#endif
    return 0;
   }
   ++methods;
  }
  getsets = DeeType_GET_SLOT(tp_iter,tp_getsets);
  DEE_ASSERT(getsets);
  while (getsets->d_name) {
#ifdef STRUCTURED
   if ((getsets->d_flags&DEE_GETSETDEF_FLAG_STRUCT)!=0)
#endif
   if DEE_UNLIKELY(strcmp(getsets->d_name,name) == 0) {
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
    DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_WRITE(cache) {
     DeeGenericAttributeCache_SET_GETSET(cache,tp_iter,name,getsets);
     DEE_ATTRIBUTE_CACHE_RELEASE(cache);
    }
use_getset:
#endif
    if DEE_UNLIKELY(!getsets->d_get) goto no_r;
#ifdef STRUCTURED
    if DEE_UNLIKELY((temp = (*DeeGetSetDef_SGET(getsets))((
     DeeStructuredTypeObject *)tp_self,self,getsets->d_closure)) == NULL) return -1;
#else
    if DEE_UNLIKELY((temp = DEE_UNLIKELY((getsets->d_flags&DEE_GETSETDEF_FLAG_STRUCT)!=0)
     ? (*DeeGetSetDef_SGET(getsets))((DeeStructuredTypeObject *)
         tp_self,DeeStructured_DATA(self),getsets->d_closure)
     : (*DeeGetSetDef_OGET(getsets))(self,getsets->d_closure)) == NULL) return -1;
#endif
    *result = DeeObject_Call(temp,args);
    Dee_DECREF(temp);
    return *result ? 0 : -1;
   }
   ++getsets;
  }
  members = DeeType_GET_SLOT(tp_iter,tp_members);
  DEE_ASSERT(members);
  while (members->d_name) {
   if DEE_UNLIKELY(strcmp(members->d_name,name) == 0) {
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
    DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_WRITE(cache) {
     DeeGenericAttributeCache_SET_MEMBER(cache,tp_iter,name,members);
     DEE_ATTRIBUTE_CACHE_RELEASE(cache);
    }
use_member:
#endif
    if ((members->d_access&DeeMemberAccess_READ) != 0) {
#ifdef STRUCTURED
     if DEE_UNLIKELY((temp = DeeMember_Get(((
      DeeObject *)self)-1,members)) == NULL) return -1;
#else
     if DEE_UNLIKELY((temp = DeeMember_Get(self,members)) == NULL) return -1;
#endif
     *result = DeeObject_Call(temp,args);
     Dee_DECREF(temp);
     return DEE_LIKELY(*result) ? 0 : -1;
    }
no_r: // no-read
    DeeError_AttributeError_NoRead(tp_self,name);
    return -1;
   }
   ++members;
  }
  if DEE_UNLIKELY((tp_iter = DeeType_BASE(tp_iter)) == NULL) break;
 }
 // Method not found
 return 1;
}


#ifdef STRUCTURED
#undef STRUCTURED
#endif

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
