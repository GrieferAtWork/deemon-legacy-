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
#ifndef GUARD_DEEMON_OPTIONAL_OBJECT_COPY_H
#define GUARD_DEEMON_OPTIONAL_OBJECT_COPY_H 1

#include <deemon/__conf.inl>

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
// v makes sure that "*self" is a unique object (refcnt == 1)
//   If it isn't, "*self" will be assigned a copy of itself
//   NOTE: May not work with custom user types that add references
//         to themselves from within their constructor
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_ForceUnique(
 DEE_A_INOUT DeeObject **self) DEE_ATTRIBUTE_NONNULL((1));
#if DEE_DEPRECATED_API_VERSION(100,101,105)
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1))
 DEE_ATTRIBUTE_DEPRECATED("'DeeObject_Unique()' is deprecated. Use 'DeeObject_ForceUnique' instead")
 DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_Unique(DEE_A_INOUT DeeObject **self) {
 return DeeObject_ForceUnique(self);
}
#endif

//////////////////////////////////////////////////////////////////////////
// Copies a constant based on its copy attributes
// >> if (DeeType_MustDeepCopy(Dee_TYPE(ob))) return DeeObject_DeepCopy(ob);
// >> else if (DeeType_MustCopy(Dee_TYPE(ob))) return DeeObject_Copy(ob);
// >> else DeeReturn_NEWREF(ob);
#define DeeObject_DeepCopy(ob) DeeObject_TDeepCopy(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TDeepCopy(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Generic object assign interface
#define /* DEE_A_EXEC int */DeeObject_CopyAssign(ob,right) DeeObject_TCopyAssign(Dee_TYPE(ob),ob,right)
#define /* DEE_A_EXEC int */DeeObject_MoveAssign(ob,right) DeeObject_TMoveAssign(Dee_TYPE(ob),ob,right)
#define /* DEE_A_EXEC int */DeeObject_Assign  DeeObject_CopyAssign
#define /* DEE_A_EXEC int */DeeObject_TAssign DeeObject_TCopyAssign
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TCopyAssign(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *right) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TMoveAssign(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *right) DEE_ATTRIBUTE_NONNULL((1,2,3));

//////////////////////////////////////////////////////////////////////////
// Copy / Move an object
// NOTE: 'DeeObject_Copy' and 'DeeObject_Move' are also implemented as
//       functions to allow use of their pointer in type operators,
//       possibly allowing the compiler to predict the return type of
//       the operator, or optimize usercode in certain situations
DEE_FUNC_DECL(DEE_A_DEEMON("copy self") DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_Copy(DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_DEEMON("move self") DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_Move(DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TCopy(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TMove(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));

#ifndef __INTELLISENSE__
#define /* DEE_A_EXEC DeeObject * */DeeObject_Copy(ob) DeeObject_TCopy(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Move(ob) DeeObject_TMove(Dee_TYPE(ob),ob)
#endif

#define DEE_PRIVATE_OBJECT_TYPE_object    &DeeObject_Type
#define DEE_PRIVATE_OBJECT_TYPE_DeeObject &DeeObject_Type
#define DEE_PRIVATE_OBJECT_NEW_object      DeeObject_Copy
#define DEE_PRIVATE_OBJECT_NEW_DeeObject   DeeObject_Copy

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_OBJECT_COPY_H */
