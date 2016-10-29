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
#ifndef GUARD_DEEMON_OPTIONAL_DETAIL_OBJECT_CALLATTR_H
#define GUARD_DEEMON_OPTIONAL_DETAIL_OBJECT_CALLATTR_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/std/stdarg.h>

// Don't include directly.
// Include these two instead:
// >> #include <deemon/optional/object_call.h>
// >> #include <deemon/optional/object_attr.h>

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
// Optimized calls to member functions
// - Using this interface will often cut the intermediate function object
//   that has to be created temporarily out of the loop.
// >> a = "foo";
// >> f = a.lower;
// >> b = f(); // f is a bound method object
// >> c = a.lower(); // bound method object is never created (this is a member call)
#define /* DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject * */DeeObject_CallAttr(ob,attr,args)                                DeeObject_TCallAttr(Dee_TYPE(ob),ob,attr,args)
#define /* DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject * */DeeObject_CallAttrString(ob,attr,args)                          DeeObject_TCallAttrString(Dee_TYPE(ob),ob,attr,args)
#define /* DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject * */DeeObject_CallAttrObjArgs(ob,attr,...)                          DeeObject_TCallAttrObjArgs(Dee_TYPE(ob),ob,attr,__VA_ARGS__)
#define /* DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject * */DeeObject_CallAttrStringObjArgs(ob,attr,...)                    DeeObject_TCallAttrStringObjArgs(Dee_TYPE(ob),ob,attr,__VA_ARGS__)
#define /* DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject * */DeeObject_CallAttrf(ob,attr,...)                                DeeObject_TCallAttrf(Dee_TYPE(ob),ob,attr,__VA_ARGS__)
#define /* DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject * */DeeObject_CallAttrStringf(ob,attr,...)                          DeeObject_TCallAttrStringf(Dee_TYPE(ob),ob,attr,__VA_ARGS__)
#define /* DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int         */DeeObject_CallAttrAndCastf(ob,result,result_fmt,attr,...)       DeeObject_TCallAttrAndCastf(Dee_TYPE(ob),ob,result,result_fmt,attr,__VA_ARGS__)
#define /* DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int         */DeeObject_CallAttrStringAndCastf(ob,result,result_fmt,attr,...) DeeObject_TCallAttrStringAndCastf(Dee_TYPE(ob),ob,result,result_fmt,attr,__VA_ARGS__)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TCallAttr(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TCallAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TCallAttrObjArgs(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, ...) DEE_ATTRIBUTE_SENTINAL DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TCallAttrStringObjArgs(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr, ...) DEE_ATTRIBUTE_SENTINAL DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TCallAttrf(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TCallAttrStringf(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TCallAttrAndCastf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_OUT_OPT void *result, DEE_A_IN_Z char const *result_fmt, DEE_A_IN_OBJECT(DeeStringObject) const *attr,
 DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2,4,5,6));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TCallAttrStringAndCastf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_OUT_OPT void *result, DEE_A_IN_Z char const *result_fmt, DEE_A_IN_Z char const *attr,
 DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2,4,5,6));
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
#define /* DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject * */DeeObject_VCallAttrObjArgs(ob,attr,args)                              DeeObject_VTCallAttrObjArgs(Dee_TYPE(ob),ob,attr,args)
#define /* DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject * */DeeObject_VCallAttrStringObjArgs(ob,attr,args)                        DeeObject_VTCallAttrStringObjArgs(Dee_TYPE(ob),ob,attr,args)
#define /* DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject * */DeeObject_VCallAttrf(ob,attr,fmt,args)                                DeeObject_VTCallAttrf(Dee_TYPE(ob),ob,attr,fmt,args)
#define /* DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject * */DeeObject_VCallAttrStringf(ob,attr,fmt,args)                          DeeObject_VTCallAttrStringf(Dee_TYPE(ob),ob,attr,fmt,args)
#define /* DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int         */DeeObject_VCallAttrAndCastf(ob,result,result_fmt,attr,fmt,args)       DeeObject_TCallAttrAndCastf(Dee_TYPE(ob),ob,result,result_fmt,attr,fmt,args)
#define /* DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int         */DeeObject_VCallAttrStringAndCastf(ob,result,result_fmt,attr,fmt,args) DeeObject_TCallAttrStringAndCastf(Dee_TYPE(ob),ob,result,result_fmt,attr,fmt,args)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_VTCallAttrObjArgs(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_VTCallAttrStringObjArgs(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_VTCallAttrf(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_VTCallAttrStringf(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_VTCallAttrAndCastf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_OUT_OPT void *result, DEE_A_IN_Z char const *result_fmt,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_IN_BUILDTUPLEF char const *fmt,
 DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2,4,5,6));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_VTCallAttrStringAndCastf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_OUT_OPT void *result, DEE_A_IN_Z char const *result_fmt,
 DEE_A_IN_Z char const *attr, DEE_A_IN_BUILDTUPLEF char const *fmt,
 DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2,4,5,6));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PCallAttrString(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PCallAttr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PCallAttrObjArgs(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr, ...) DEE_ATTRIBUTE_SENTINAL DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PCallAttrStringObjArgs(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr, ...) DEE_ATTRIBUTE_SENTINAL DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PCallAttrf(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PCallAttrStringf(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_VPCallAttrObjArgs(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_VPCallAttrStringObjArgs(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_VPCallAttrf(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_VPCallAttrStringf(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */


DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_DETAIL_OBJECT_CALLATTR_H */
