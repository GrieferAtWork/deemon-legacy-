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
#ifndef GUARD_DEEMON_OPTIONAL_OBJECT_CALL_H
#define GUARD_DEEMON_OPTIONAL_OBJECT_CALL_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/std/stdarg.h>

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
// Generic call interface
#define /*DEE_A_EXEC*/ DeeObject_Call(ob,args) DeeObject_TCall(Dee_TYPE(ob),ob,args)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TCall(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PCall(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3));

//////////////////////////////////////////////////////////////////////////
// Performs a this-call on 'self' but re-packs args with
// 'this_arg' if no special this-call handling is available.
#define /*DEE_A_EXEC*/ DeeObject_ThisCall(ob,this_arg,args) DeeObject_TThisCall(Dee_TYPE(ob),ob,this_arg,args)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TThisCall(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *this_arg, DEE_A_INOUT_OBJECT(DeeTupleObject) *args)
 DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PThisCall(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *this_arg,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));

//////////////////////////////////////////////////////////////////////////
// Call an object with a NULL-terminated list of arguments
#define DeeObject_CallObjArgs(ob,...) DeeObject_TCallObjArgs(Dee_TYPE(ob),ob,__VA_ARGS__)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TCallObjArgs(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, ...) DEE_ATTRIBUTE_SENTINAL DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PCallObjArgs(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, ...) DEE_ATTRIBUTE_SENTINAL DEE_ATTRIBUTE_NONNULL((1,2));
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
#define /*DEE_A_EXEC*/ DeeObject_VCallObjArgs(ob,args) DeeObject_VTCallObjArgs(Dee_TYPE(ob),ob,args)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_VTCallObjArgs(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_VPCallObjArgs(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */

//////////////////////////////////////////////////////////////////////////
// Call an object with a BuildTuple list of arguments
#define DeeObject_Callf(ob,...) DeeObject_TCallf(Dee_TYPE(ob),ob,__VA_ARGS__)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TCallf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PCallf(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_BUILDTUPLEF char const *fmt, ...)
 DEE_ATTRIBUTE_NONNULL((1,2,3));
#define DeeObject_ThisCallf(ob,this_arg,...) DeeObject_TThisCallf(Dee_TYPE(ob),ob,this_arg,__VA_ARGS__)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TThisCallf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *this_arg, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2,3));

#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
#define DeeObject_VCallf(ob,fmt,args) DeeObject_VTCallf(Dee_TYPE(ob),ob,fmt,args)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_VTCallf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_VPCallf(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args)
 DEE_ATTRIBUTE_NONNULL((1,2,3));
#define DeeObject_VThisCallf(ob,this_arg,fmt,args) DeeObject_VTThisCallf(Dee_TYPE(ob),ob,this_arg,fmt,args)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_VTThisCallf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *this_arg, DEE_A_IN_BUILDTUPLEF char const *fmt,
 DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2,3));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */

DEE_DECL_END

#ifdef GUARD_DEEMON_OPTIONAL_OBJECT_ATTR_H
#ifndef GUARD_DEEMON_OPTIONAL_DETAIL_OBJECT_CALLATTR_H
#include <deemon/optional/detail/object_callattr.h>
#endif
#endif /* GUARD_DEEMON_OPTIONAL_OBJECT_ATTR_H */

#endif /* !GUARD_DEEMON_OPTIONAL_OBJECT_CALL_H */
