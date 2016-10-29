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
#ifndef GUARD_DEEMON_WEAKREF_H
#define GUARD_DEEMON_WEAKREF_H 1

#include <deemon/__conf.inl>
#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#include <deemon/error.h>
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

DEE_OBJECT_DEF(DeeWeakrefObject);

//////////////////////////////////////////////////////////////////////////
// Weak reference object
//////////////////////////////////////////////////////////////////////////

#ifdef DEE_LIMITED_API
struct DeeWeakrefObject {
 DEE_OBJECT_HEAD
 DEE_A_WEAK_REF DeeObject *wr_value; /*< [1..1][weak_ref][const] Contained object. */
};
#define DeeWeakref_VALUE(ob)  ((DeeWeakrefObject *)Dee_REQUIRES_POINTER(ob))->wr_value
#define DeeWeakref_ALIVE(ob)  DeeObject_IS_ALIVE(DeeWeakref_VALUE(ob))
#define DeeWeakref_LOCK(self) DeeObject_LockWeakref(DeeWeakref_VALUE(self))
#else /* DEE_LIMITED_API */
#define DeeWeakref_ALIVE(ob)  DeeWeakref_Alive((DeeObject *)Dee_REQUIRES_POINTER(ob))
#endif /* !DEE_LIMITED_API */

#define DeeWeakref_Check(ob)      DeeObject_InstanceOf(ob,&DeeWeakref_Type)
#define DeeWeakref_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeWeakref_Type)
DEE_DATA_DECL(DeeTypeObject) DeeWeakref_Type;

DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWeakrefObject) *) DeeWeakref_New(
 DEE_A_INOUT DeeObject *value) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeWeakref_Alive(
 DEE_A_INOUT_OBJECT(DeeWeakrefObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Lock a weak reference and return a new reference to the contained object
// >> Returns NULL if the lock failed (only sets an error, if *WithError was called)
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT_REF DeeObject *) DeeWeakref_Lock(
 DEE_A_INOUT_OBJECT(DeeWeakrefObject) *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_REF DeeObject *) DeeWeakref_LockWithError(
 DEE_A_INOUT_OBJECT(DeeWeakrefObject) *self) DEE_ATTRIBUTE_NONNULL((1));

DEE_FUNC_DECL(Dee_size_t) DeeWeakref_ClearPool(void);

#ifdef DEE_LIMITED_API
extern DeeErrorObject _DeeError_DeadWeakReference;
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_WEAKREF_H */
