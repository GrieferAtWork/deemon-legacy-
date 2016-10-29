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
#include "object.c"
DEE_DECL_BEGIN
#endif

#ifndef DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
#define DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE 0
#endif

#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE
#define DeeGenericAttributeCache_SET(cache,self,name,type,slot,data)\
((cache)->gac_self = (self),(cache)->gac_name = (name)\
,(cache)->gac_type = (type),(cache)->slot = (data))


// NOTE: The 3 unused bytes are:
//   - gac_lock (if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_LOCKEACH is disabled)
//   - gac_unused
//   - gac_type (Most significant byte)
struct DeeGenericAttributeCache {
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_LOCKEACH
 struct DeeAtomicMutex gac_lock;   /*< The lock for this cache entry. */
 Dee_uint8_t           gac_unused;
#else
 Dee_uint16_t          gac_unused;
#endif
#define DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_NONE         DEE_UINT16_C(0)
#define DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_MEMBER       DEE_UINT16_C(1)
#define DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_GETSET       DEE_UINT16_C(2)
#define DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_METHOD       DEE_UINT16_C(3)
#define DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_CLASS_MEMBER DEE_UINT16_C(4)
#define DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_CLASS_GETSET DEE_UINT16_C(5)
#define DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_CLASS_METHOD DEE_UINT16_C(6)
 Dee_uint16_t          gac_type;   /*< The type of cache */
 DeeTypeObject const  *gac_self;   /*< [0..1] Type implementing the attribute. */
 char const           *gac_name;   /*< [0..1] Name of the attribute. */
 union{
  struct DeeMemberDef const *gac_member;
  struct DeeGetSetDef const *gac_getset;
  struct DeeMethodDef const *gac_method;
 };
};
#define DeeGenericAttributeCache_SET_MEMBER(cache,self,name,member) DeeGenericAttributeCache_SET(cache,self,name,DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_MEMBER,gac_member,member)
#define DeeGenericAttributeCache_SET_GETSET(cache,self,name,getset) DeeGenericAttributeCache_SET(cache,self,name,DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_GETSET,gac_getset,getset)
#define DeeGenericAttributeCache_SET_METHOD(cache,self,name,method) DeeGenericAttributeCache_SET(cache,self,name,DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_METHOD,gac_method,method)
#define DeeGenericAttributeCache_SET_CLASS_MEMBER(cache,self,name,member) DeeGenericAttributeCache_SET(cache,self,name,DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_CLASS_MEMBER,gac_member,member)
#define DeeGenericAttributeCache_SET_CLASS_GETSET(cache,self,name,getset) DeeGenericAttributeCache_SET(cache,self,name,DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_CLASS_GETSET,gac_getset,getset)
#define DeeGenericAttributeCache_SET_CLASS_METHOD(cache,self,name,method) DeeGenericAttributeCache_SET(cache,self,name,DEE_GENERIC_ATTRIBUTE_CACHE_TYPE_CLASS_METHOD,gac_method,method)


#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_USEHASH
#define DEE_ATTRIBUTE_CACHE_INDEX_STRING(tp_self,name)\
(((Dee_uintptr_t)(tp_self)^(Dee_uintptr_t)DeeHash_String(name,DEE_HASH_START)\
  )%DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE)
#else
#define DEE_ATTRIBUTE_CACHE_INDEX_STRING(tp_self,name)\
(((Dee_uintptr_t)(tp_self)^(Dee_uintptr_t)(name)\
  )%DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE)
#endif
#define DEE_ATTRIBUTE_CACHE_INDEX(tp_self,name) \
 DEE_ATTRIBUTE_CACHE_INDEX_STRING(tp_self,DeeString_STR(name))
#define DEE_ATTRIBUTE_CACHE_STRING(tp_self,name) \
 (_generic_attr_cache+DEE_ATTRIBUTE_CACHE_INDEX_STRING(tp_self,name))
#define DEE_ATTRIBUTE_CACHE(tp_self,name) \
 (_generic_attr_cache+DEE_ATTRIBUTE_CACHE_INDEX(tp_self,name))

#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_LOCKEACH
static struct DeeGenericAttributeCache _generic_attr_cache[
 DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE] = {{DeeAtomicMutex_INIT(),0,0,NULL,NULL,{NULL}},};
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_TRYREAD
#define DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_READ(cache) if DEE_LIKELY(DeeAtomicMutex_TryAcquire(&(cache)->gac_lock))
#else
#define DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_READ(cache) DeeAtomicMutex_Acquire(&(cache)->gac_lock);
#endif
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_TRYWRITE
#define DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_WRITE(cache) if DEE_LIKELY(DeeAtomicMutex_TryAcquire(&(cache)->gac_lock))
#else
#define DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_WRITE(cache) DeeAtomicMutex_Acquire(&(cache)->gac_lock);
#endif
#define DEE_ATTRIBUTE_CACHE_RELEASE(cache)          DeeAtomicMutex_Release(&(cache)->gac_lock)
#else /* DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_LOCKEACH */
static struct DeeGenericAttributeCache _generic_attr_cache[
 DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_SIZE] = {{0,0,NULL,NULL,{NULL}},};
static struct DeeAtomicMutex _generic_attr_cache_lock = DeeAtomicMutex_INIT();
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_TRYREAD
#define DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_READ(cache) if DEE_LIKELY(DeeAtomicMutex_TryAcquire(&_generic_attr_cache_lock))
#else
#define DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_READ(cache) DeeAtomicMutex_Acquire(&_generic_attr_cache_lock);
#endif
#if DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_CACHE_TRYWRITE
#define DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_WRITE(cache) if DEE_LIKELY(DeeAtomicMutex_TryAcquire(&_generic_attr_cache_lock))
#else
#define DEE_ATTRIBUTE_CACHE_IF_ACQUIRE_WRITE(cache) DeeAtomicMutex_Acquire(&_generic_attr_cache_lock);
#endif
#define DEE_ATTRIBUTE_CACHE_RELEASE(cache)          DeeAtomicMutex_Release(&_generic_attr_cache_lock)
#endif /* !DEE_XCONFIG_GENERIC_ATTRIBUTE_LOOKUP_LOCKEACH */

#endif



#ifndef __INTELLISENSE__
#define STRUCTURED
#include "object.generic_attribute.get_impl.inl"
#define CONST_ONLY
#include "object.generic_attribute.get_impl.inl"
#include "object.generic_attribute.get_impl.inl"

#define STRUCTURED
#include "object.generic_attribute.has_impl.inl"
#include "object.generic_attribute.has_impl.inl"

#define STRUCTURED
#include "object.generic_attribute.del_impl.inl"
#include "object.generic_attribute.del_impl.inl"

#define STRUCTURED
#include "object.generic_attribute.set_impl.inl"
#include "object.generic_attribute.set_impl.inl"

#define STRUCTURED
#include "object.generic_attribute.call_impl.inl"
#include "object.generic_attribute.call_impl.inl"
#endif


DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TGenericGetAttrString(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *name) {
 DeeObject *result; int temp;
 DEE_ASSERT(DeeObject_Check(self)),DEE_ASSERT(name);
 if ((temp = _DeeObject_TGenericGetAttrString(tp_self,self,name,&result)) != 0) {
  if (temp == 1) DeeError_AttributeError_NotFound(tp_self,name);
  result = NULL;
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PGenericGetAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT void *base_addr,
 DEE_A_IN_Z char const *name) {
 DeeObject *result; int temp;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(name);
 if ((temp = _DeeObject_PGenericGetAttrString(tp_self,base_addr,name,&result)) != 0) {
  if (temp == 1) DeeError_AttributeError_NotFound(tp_self,name);
  result = NULL;
 }
 return result;
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TGenericGetConstAttrString(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *name) {
 DeeObject *result; int temp;
 DEE_ASSERT(DeeObject_Check(self)),DEE_ASSERT(name);
 if ((temp = _DeeObject_TGenericGetConstAttrString(tp_self,self,name,&result)) != 0) {
  if (temp == 1) DeeError_AttributeError_NotFound(tp_self,name);
  result = NULL;
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TGenericDelAttrString(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *name) {
 int result;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self)),DEE_ASSERT(name);
 if ((result = _DeeObject_TGenericDelAttrString(tp_self,self,name)) == 1) {
  DeeError_AttributeError_NotFound(tp_self,name);
  result = -1;
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PGenericDelAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT void *base_addr,
 DEE_A_IN_Z char const *name) {
 int result;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(name);
 if ((result = _DeeObject_PGenericDelAttrString(tp_self,base_addr,name)) == 1) {
  DeeError_AttributeError_NotFound(tp_self,name);
  result = -1;
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TGenericSetAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_Z char const *name, DEE_A_INOUT DeeObject *v) {
 int result;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(name),DEE_ASSERT(DeeObject_Check(v));
 if ((result = _DeeObject_TGenericSetAttrString(tp_self,self,name,v)) == 1)
  DeeError_AttributeError_NotFound(tp_self,name),result = -1;
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PGenericSetAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT void *base_addr,
 DEE_A_IN_Z char const *name, DEE_A_INOUT DeeObject *v) {
 int result;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(v));
 DEE_ASSERT(name);
 if ((result = _DeeObject_PGenericSetAttrString(tp_self,base_addr,name,v)) == 1)
  DeeError_AttributeError_NotFound(tp_self,name),result = -1;
 return result;
}


//////////////////////////////////////////////////////////////////////////
// Type-member linkage
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_GenericGetAttr(
 DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DEE_ASSERT(DeeObject_Check(self));
 return DeeObject_TGenericGetAttr(Dee_TYPE(self),self,attr);
}
DEE_A_RET_EXCEPT(-1) int DeeObject_GenericDelAttr(
 DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DEE_ASSERT(DeeObject_Check(self));
 return DeeObject_TGenericDelAttr(Dee_TYPE(self),self,attr);
}
DEE_A_RET_EXCEPT(-1) int DeeObject_GenericSetAttr(DEE_A_INOUT DeeObject *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v) {
 DEE_ASSERT(DeeObject_Check(self));
 return DeeObject_TGenericSetAttr(Dee_TYPE(self),self,attr,v);
}



#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
