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
DEE_A_RET_NOEXCEPT(0) int _DeeObject_PGenericHasAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN_Z char const *name)
#else
DEE_A_RET_NOEXCEPT(0) int _DeeObject_TGenericHasAttrString(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *name)
#endif
{
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
 struct DeeGenericAttributeCache *cache;
#endif
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
#ifndef STRUCTURED
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
#endif
 while (1) {
  struct DeeMemberDef const *members;
  struct DeeGetSetDef const *getsets;
  struct DeeMethodDef const *methods;
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
  // Fast pass through attribute cache
  cache = DEE_ATTRIBUTE_CACHE_STRING(tp_self,name);
  DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_READ(cache) {
   if (cache->gac_self == tp_self && cache->gac_name == name && (
       cache->gac_type == DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_MEMBER
    || cache->gac_type == DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_GETSET
    || cache->gac_type == DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_METHOD)) {
    DEE_ATTRIBUTE_CACHE_RELEASE(cache);
    return 1;
   }
   DEE_ATTRIBUTE_CACHE_RELEASE(cache);
  }
#endif
  members = DeeType_GET_SLOT(tp_self,tp_members);
  DEE_ASSERT(members);
  while (members->d_name) {
   if (strcmp(members->d_name,name) == 0) {
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
    DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_WRITE(cache) {
     DeeGenericAttributeCache_SET_MEMBER(cache,tp_self,name,members);
     DEE_ATTRIBUTE_CACHE_RELEASE(cache);
    }
#endif
    return 1;
   }
   ++members;
  }
  getsets = DeeType_GET_SLOT(tp_self,tp_getsets);
  DEE_ASSERT(getsets);
  while (getsets->d_name) {
#ifdef STRUCTURED
   if ((getsets->d_flags&DEE_GETSETDEF_FLAG_STRUCT) != 0)
#endif
   if (strcmp(getsets->d_name,name) == 0) {
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
    DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_WRITE(cache) {
     DeeGenericAttributeCache_SET_GETSET(cache,tp_self,name,getsets);
     DEE_ATTRIBUTE_CACHE_RELEASE(cache);
    }
#endif
    return 1;
   }
   ++getsets;
  }
  methods = DeeType_GET_SLOT(tp_self,tp_methods);
  DEE_ASSERT(methods);
  while (methods->d_name) {
#ifdef STRUCTURED
   if ((methods->d_flags&DEE_METHODDEF_FLAG_STRUCT) != 0)
#endif
   if (strcmp(methods->d_name,name) == 0) {
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
    DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_WRITE(cache) {
     DeeGenericAttributeCache_SET_METHOD(cache,tp_self,name,methods);
     DEE_ATTRIBUTE_CACHE_RELEASE(cache);
    }
#endif
    return 1;
   }
   ++methods;
  }
  if ((tp_self = DeeType_BASE(tp_self)) == NULL) break;
 }
 return 0;
}


#ifdef STRUCTURED
#undef STRUCTURED
#endif

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
