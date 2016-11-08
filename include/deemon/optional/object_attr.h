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
#ifndef GUARD_DEEMON_OPTIONAL_OBJECT_ATTR_H
#define GUARD_DEEMON_OPTIONAL_OBJECT_ATTR_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/object_decl.h>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif

//////////////////////////////////////////////////////////////////////////
// Generic attribute interface
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TGetAttr(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TDelAttr(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TSetAttr(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TGetAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeObject_THasAttr(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeObject_THasAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TDelAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TSetAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3,4));

#define DeeObject_GetAttrAndCast(self,result,result_fmt,attr) \
 DeeObject_TGetAttrAndCast(Dee_TYPE(self),self,result,result_fmt,attr)
#define DeeObject_GetAttrStringAndCast(self,result,result_fmt,attr) \
 DeeObject_TGetAttrStringAndCast(Dee_TYPE(self),self,result,result_fmt,attr)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TGetAttrAndCast(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_OUT_OPT void *result,
 DEE_A_IN_Z char const *result_fmt, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2,4,5));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TGetAttrStringAndCast(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_OUT_OPT void *result,
 DEE_A_IN_Z char const *result_fmt, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,4,5));


DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PGetAttr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PDelAttr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PSetAttr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PGetAttrString(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeObject_PHasAttr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeObject_PHasAttrString(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PDelAttrString(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PSetAttrString(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3,4));

#define /*DEE_A_EXEC*/ DeeObject_GetAttr(ob,attr)         DeeObject_TGetAttr(Dee_TYPE(ob),ob,attr)
#define /*DEE_A_EXEC*/ DeeObject_DelAttr(ob,attr)         DeeObject_TDelAttr(Dee_TYPE(ob),ob,attr)
#define /*DEE_A_EXEC*/ DeeObject_SetAttr(ob,attr,v)       DeeObject_TSetAttr(Dee_TYPE(ob),ob,attr,v)
#define /*DEE_A_EXEC*/ DeeObject_GetAttrString(ob,attr)   DeeObject_TGetAttrString(Dee_TYPE(ob),ob,attr)
#define /*DEE_A_EXEC*/ DeeObject_HasAttr(ob,attr)         DeeObject_THasAttr(Dee_TYPE(ob),ob,attr)
#define /*DEE_A_EXEC*/ DeeObject_HasAttrString(ob,attr)   DeeObject_THasAttrString(Dee_TYPE(ob),ob,attr)
#define /*DEE_A_EXEC*/ DeeObject_DelAttrString(ob,attr)   DeeObject_TDelAttrString(Dee_TYPE(ob),ob,attr)
#define /*DEE_A_EXEC*/ DeeObject_SetAttrString(ob,attr,v) DeeObject_TSetAttrString(Dee_TYPE(ob),ob,attr,v)

//////////////////////////////////////////////////////////////////////////
// Constant attribute access
//  - Only works for types using no custom attribute operator
//  - Makes sure that getsets+methods are flagged with the constant attribute flags
//  - Used during compilation to optimize attribute lookup
//    So if this fails, the compiler will generate a getattr operator
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TGetAttrConst(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TGetAttrConstString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
#define /*DEE_A_EXEC*/ DeeObject_GetAttrConst(ob,attr)       DeeObject_TGetAttrConst(Dee_TYPE(ob),ob,attr)
#define /*DEE_A_EXEC*/ DeeObject_GetAttrConstString(ob,attr) DeeObject_TGetAttrConstString(Dee_TYPE(ob),ob,attr)

//////////////////////////////////////////////////////////////////////////
// These generic functions for attributes only use the
// tp_members, tp_getsets and tp_methods fields of the object
// >> They don't respect any overwritten tp_attr_get / tp_attr_del / tp_attr_set operators!
// >> So don't use these accidentally, OK; You have been warned!
// NOTE: - They don't raise any errors concerning missing attributes.
DEE_FUNC_DECL(           DEE_A_RET_NOEXCEPT(0) int) _DeeObject_TGenericHasAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) _DeeObject_TGenericGetAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *name, DEE_A_REF DEE_A_OUT DeeObject **result) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) _DeeObject_TGenericGetConstAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *name, DEE_A_REF DEE_A_OUT DeeObject **result) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) _DeeObject_TGenericDelAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) _DeeObject_TGenericSetAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *name, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) _DeeObject_TGenericCallAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *name, DEE_A_INOUT_OBJECT(DeeTupleObject) *args, DEE_A_REF DEE_A_OUT DeeObject **result) DEE_ATTRIBUTE_NONNULL((1,2,3,4,5));
DEE_FUNC_DECL(           DEE_A_RET_NOEXCEPT(0) int) _DeeObject_PGenericHasAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) _DeeObject_PGenericGetAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN void *self, DEE_A_IN_Z char const *name, DEE_A_REF DEE_A_OUT DeeObject **result) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) _DeeObject_PGenericDelAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN void *self, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) _DeeObject_PGenericSetAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN void *self, DEE_A_IN_Z char const *name, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) _DeeObject_PGenericCallAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN void *self, DEE_A_IN_Z char const *name, DEE_A_INOUT_OBJECT(DeeTupleObject) *args, DEE_A_REF DEE_A_OUT DeeObject **result) DEE_ATTRIBUTE_NONNULL((1,2,3,4,5));
#define                _DeeObject_GenericHasAttrString(ob,name)              _DeeObject_TGenericHasAttrString(Dee_TYPE(ob),ob,name)
#define /*DEE_A_EXEC*/ _DeeObject_GenericGetAttrString(ob,name,result)       _DeeObject_TGenericGetAttrString(Dee_TYPE(ob),ob,name,result)
#define /*DEE_A_EXEC*/ _DeeObject_GenericGetConstAttrString(ob,name,result)  _DeeObject_TGenericGetConstAttrString(Dee_TYPE(ob),ob,name,result)
#define /*DEE_A_EXEC*/ _DeeObject_GenericDelAttrString(ob,name)              _DeeObject_TGenericDelAttrString(Dee_TYPE(ob),ob,name)
#define /*DEE_A_EXEC*/ _DeeObject_GenericSetAttrString(ob,name,v)            _DeeObject_TGenericSetAttrString(Dee_TYPE(ob),ob,name,v)
#define /*DEE_A_EXEC*/ _DeeObject_GenericCallAttrString(ob,name,args,result) _DeeObject_TGenericCallAttrString(Dee_TYPE(ob),ob,name,args,result)

#ifdef DEE_LIMITED_API
extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TGenericGetAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TGenericGetConstAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_TGenericDelAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_TGenericSetAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TGenericCallAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
#define /*DEE_A_EXEC*/ DeeObject_TGenericGetAttr(tp_self,self,attr) DeeObject_TGenericGetAttrString(tp_self,self,DeeString_STR(attr))
#define /*DEE_A_EXEC*/ DeeObject_TGenericGetConstAttr(tp_self,self,attr) DeeObject_TGenericGetConstAttrString(tp_self,self,DeeString_STR(attr))
#define /*DEE_A_EXEC*/ DeeObject_TGenericDelAttr(tp_self,self,attr) DeeObject_TGenericDelAttrString(tp_self,self,DeeString_STR(attr))
#define /*DEE_A_EXEC*/ DeeObject_TGenericSetAttr(tp_self,self,attr,v) DeeObject_TGenericSetAttrString(tp_self,self,DeeString_STR(attr),v)
#define /*DEE_A_EXEC*/ DeeObject_TGenericCallAttr(tp_self,self,attr,args) DeeObject_TGenericCallAttrString(tp_self,self,DeeString_STR(attr),args)

extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PGenericGetAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_PGenericDelAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_PGenericSetAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PGenericCallAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
#define /*DEE_A_EXEC*/ DeeObject_PGenericGetAttr(tp_self,base_addr,attr) DeeObject_PGenericGetAttrString(tp_self,base_addr,DeeString_STR(attr))
#define /*DEE_A_EXEC*/ DeeObject_PGenericDelAttr(tp_self,base_addr,attr) DeeObject_PGenericDelAttrString(tp_self,base_addr,DeeString_STR(attr))
#define /*DEE_A_EXEC*/ DeeObject_PGenericSetAttr(tp_self,base_addr,attr,v) DeeObject_PGenericSetAttrString(tp_self,base_addr,DeeString_STR(attr),v)
#define /*DEE_A_EXEC*/ DeeObject_PGenericCallAttr(tp_self,base_addr,attr,args) DeeObject_PGenericCallAttrString(tp_self,base_addr,DeeString_STR(attr),args)

extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_GenericGetAttr(DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_GenericDelAttr(DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeObject_GenericSetAttr(DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3));
#define /*DEE_A_EXEC*/ DeeObject_GenericGetAttrString(ob,attr)       DeeObject_TGenericGetAttrString(Dee_TYPE(ob),ob,attr)
#define /*DEE_A_EXEC*/ DeeObject_GenericGetConstAttrString(ob,attr)  DeeObject_TGenericGetConstAttrString(Dee_TYPE(ob),ob,attr)
#define /*DEE_A_EXEC*/ DeeObject_GenericGetConstAttr(ob,attr)        DeeObject_GenericGetConstAttrString(ob,DeeString_STR(attr))
#define /*DEE_A_EXEC*/ DeeObject_GenericDelAttrString(ob,attr)       DeeObject_TGenericDelAttrString(Dee_TYPE(ob),ob,attr)
#define /*DEE_A_EXEC*/ DeeObject_GenericSetAttrString(ob,attr,v)     DeeObject_TGenericSetAttrString(Dee_TYPE(ob),ob,attr,v)
#define /*DEE_A_EXEC*/ DeeObject_GenericCallAttrString(ob,attr,args) DeeObject_TGenericCallAttrString(Dee_TYPE(ob),ob,attr,args);
#define /*DEE_A_EXEC*/ DeeObject_GenericCallAttr(ob,attr,args)       DeeObject_GenericCallAttrString(ob,DeeString_STR(attr),args)
#elif defined(DEE_LIMITED_DEX)
#define DeeObject_GenericGetAttr  (*DeeType_GET_SLOT(&DeeObject_Type,tp_attr_get))
#define DeeObject_GenericDelAttr  (*DeeType_GET_SLOT(&DeeObject_Type,tp_attr_del))
#define DeeObject_GenericSetAttr  (*DeeType_GET_SLOT(&DeeObject_Type,tp_attr_set))
#endif

DEE_DECL_END

#ifdef GUARD_DEEMON_OPTIONAL_OBJECT_CALL_H
#ifndef GUARD_DEEMON_OPTIONAL_DETAIL_OBJECT_CALLATTR_H
#include <deemon/optional/detail/object_callattr.h>
#endif
#endif /* GUARD_DEEMON_OPTIONAL_OBJECT_CALL_H */

#endif /* !GUARD_DEEMON_OPTIONAL_OBJECT_ATTR_H */
