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
DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeObject_PGenericSetAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN void *self,
 DEE_A_IN_Z char const *name, DEE_A_INOUT DeeObject *v)
#else
DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeObject_TGenericSetAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_Z char const *name, DEE_A_INOUT DeeObject *v)
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
 tp_iter = tp_self; while(1) {
  struct DeeMemberDef const *members;
  struct DeeGetSetDef const *getsets;
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
  // Fast pass through attribute cache
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
#if !DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS
    if ((members->d_access&DeeMemberAccess_WRITE) != 0)
#endif /* !DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS */
    {
#ifdef STRUCTURED
     return DeeMember_Set(((DeeObject *)self)-1,members,v);
#else
     return DeeMember_Set(self,members,v);
#endif
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
    if (getsets->d_set) {
#ifdef STRUCTURED
     return (*DeeGetSetDef_SSET(getsets))((
      DeeStructuredTypeObject *)tp_self,self,v,getsets->d_closure);
#else
     return (getsets->d_flags&DEE_GETSETDEF_FLAG_STRUCT)!=0
      ? (*DeeGetSetDef_SSET(getsets))((DeeStructuredTypeObject *)
          tp_self,DeeStructured_DATA(self),v,getsets->d_closure)
      : (*DeeGetSetDef_OSET(getsets))(self,v,getsets->d_closure);
#endif
    }
#if !DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS
no_w: // no_write
#endif /* !DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS */
    DeeError_AttributeError_NoWrite(tp_self,name);
    return -1;
   }
   ++getsets;
  }
  if ((tp_iter = DeeType_BASE(tp_iter)) == NULL) break;
 }
 return 1;
}


#ifdef STRUCTURED
#undef STRUCTURED
#endif

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
