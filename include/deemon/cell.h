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
#ifndef GUARD_DEEMON_CELL_H
#define GUARD_DEEMON_CELL_H 1

#include <deemon/__conf.inl>
#ifdef DEE_LIMITED_DEX
#include <deemon/optional/atomic_mutex.h>
#include <deemon/optional/object_decl.h>
#endif /* DEE_LIMITED_DEX */

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif

#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif

DEE_OBJECT_DEF(DeeCellObject);

//////////////////////////////////////////////////////////////////////////
// Cell reference object
//////////////////////////////////////////////////////////////////////////

#ifdef DEE_LIMITED_DEX
struct DeeCellObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeObject  *c_value; /*< [0..1] Contained object (NULL if empty). */
 struct DeeAtomicMutex c_lock;  /*< Lock for this cell. */
};
#define DeeCell_LOCK(ob)  (&((DeeCellObject *)Dee_REQUIRES_POINTER(ob))->c_lock)
#define DeeCell_VALUE(ob)   ((DeeCellObject *)Dee_REQUIRES_POINTER(ob))->c_value
#define DeeCell_ACQUIRE(ob)   DeeAtomicMutex_Acquire(DeeCell_LOCK(ob))
#define DeeCell_RELEASE(ob)   DeeAtomicMutex_Release(DeeCell_LOCK(ob))
// v It doesn't matter if we only read a portion of the pointer if another thread
//   is writing to it at the same time. Because we only care if the pointer is NULL,
//   meaning that this is thread-safe and OK to do.
//   NOTE: Though using this doesn't make too much sense in a multi-threaded context...
#define DeeCell_EMPTY(ob)   (((DeeCellObject *)Dee_REQUIRES_POINTER(ob))->c_value==NULL)
#endif /* !DEE_LIMITED_DEX */

DEE_DATA_DECL(DeeTypeObject) DeeCell_Type;
#define DeeCell_Check(ob)      DeeObject_InstanceOf(ob,&DeeCell_Type)
#define DeeCell_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeCell_Type)

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeCellObject) *) DeeCell_NewEmpty(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeCellObject) *) DeeCell_New(DEE_A_INOUT DeeObject *value) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeCellObject) *) DeeCell_DeepCopy(DEE_A_IN_OBJECT(DeeCellObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeCell_Get(DEE_A_IN_OBJECT(DeeCellObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(           DEE_A_RET_NOEXCEPT_REF DeeObject *) DeeCell_TryGet(DEE_A_IN_OBJECT(DeeCellObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeCell_Pop(DEE_A_INOUT_OBJECT(DeeCellObject) *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(           DEE_A_RET_NOEXCEPT_REF DeeObject *) DeeCell_TryPop(DEE_A_INOUT_OBJECT(DeeCellObject) *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC void) DeeCell_Del(DEE_A_INOUT_OBJECT(DeeCellObject) *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC void) DeeCell_Set(DEE_A_INOUT_OBJECT(DeeCellObject) *self, DEE_A_INOUT DeeObject *value) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(           DEE_A_RET_NOEXCEPT_REF DeeObject *) DeeCell_Exchange(DEE_A_INOUT_OBJECT(DeeCellObject) *self, DEE_A_INOUT DeeObject *value) DEE_ATTRIBUTE_NONNULL((1,2));

DEE_DECL_END

#endif /* !GUARD_DEEMON_CELL_H */
