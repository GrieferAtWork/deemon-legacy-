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
#ifndef GUARD_DEEMON_SUPER_H
#define GUARD_DEEMON_SUPER_H 1

#include <deemon/__conf.inl>

#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#include <deemon/type.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif

DEE_OBJECT_DEF(DeeSuperObject);
#ifdef DEE_LIMITED_API
struct DeeSuperObject {
 DEE_OBJECT_HEAD
           DeeTypeObject const *s_type; /*< [1..1][const] Type of the self pointer (not a reference as kept alive through 's_self'). */
 DEE_A_REF DeeObject           *s_self; /*< [1..1][const] Self pointer. */
};
#define /* DeeObject     * */DeeSuper_TYPE(ob) ((DeeSuperObject *)Dee_REQUIRES_POINTER(ob))->s_type
#define /* DeeTypeObject * */DeeSuper_SELF(ob) ((DeeSuperObject *)Dee_REQUIRES_POINTER(ob))->s_self
#else
#define /* DeeObject     * */DeeSuper_TYPE(ob) DeeSuper_GetType((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define /* DeeTypeObject * */DeeSuper_SELF(ob) DeeSuper_GetSelf((DeeObject *)Dee_REQUIRES_POINTER(ob))
#endif

DEE_DATA_DECL(DeeTypeObject) DeeSuper_Type;
#define /* int */DeeSuper_Check(ob)      DeeObject_InstanceOf(ob,&DeeSuper_Type)
#define /* int */DeeSuper_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeSuper_Type)

//////////////////////////////////////////////////////////////////////////
// Returns a new super object with a given type and object
// Requires: 'DeeObject_InstanceOf(self,tp_self)'
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeSuperObject) *) DeeSuper_New(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Returns a new super object that describes the super class of 'ob'
// Throws an 'Error.TypeError' if 'type ob' doesn't have a super class
// Returns the super of 'DeeSuper_TYPE(ob)' if 'ob' is already a super object
#define DeeSuper_Of(ob) DeeSuper_TOf(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeSuperObject) *) DeeSuper_TOf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Non-limited Member access
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeObject *)
 DeeSuper_GetSelf(DEE_A_IN_OBJECT(DeeSuperObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeTypeObject const *)
 DeeSuper_GetType(DEE_A_IN_OBJECT(DeeSuperObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Clears the pool of free super objects
DEE_FUNC_DECL(Dee_size_t) DeeSuper_ClearPool(void);

#ifdef DEE_LIMITED_API
extern DeeObject *_deesuper_tp_attr_get(DeeSuperObject *self, DeeObject *attr);
extern int _deesuper_tp_attr_del(DeeSuperObject *self, DeeObject *attr);
extern int _deesuper_tp_attr_set(DeeSuperObject *self, DeeObject *attr, DeeObject *v);
#elif defined(DEE_LIMITED_DEX)
#define _deesuper_tp_attr_get  (*DeeType_GET_SLOT(&DeeSuper_Type,tp_attr_get))
#define _deesuper_tp_attr_del  (*DeeType_GET_SLOT(&DeeSuper_Type,tp_attr_del))
#define _deesuper_tp_attr_set  (*DeeType_GET_SLOT(&DeeSuper_Type,tp_attr_set))
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_SUPER_H */
