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
#ifndef GUARD_DEEMON_SEQUENCE_H
#define GUARD_DEEMON_SEQUENCE_H 1

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
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif

//////////////////////////////////////////////////////////////////////////
// Generic sequence interface (advanced)
#define /* DEE_A_EXEC Dee_ssize_t */DeeSequence_Find(ob,findob) DeeSequence_TFind(Dee_TYPE(ob),ob,findob)
#define /* DEE_A_EXEC Dee_ssize_t */DeeSequence_RFind(ob,findob) DeeSequence_TRFind(Dee_TYPE(ob),ob,findob)
#define /* DEE_A_EXEC Dee_size_t */DeeSequence_Index(ob,findob) DeeSequence_TIndex(Dee_TYPE(ob),ob,findob)
#define /* DEE_A_EXEC Dee_size_t */DeeSequence_RIndex(ob,findob) DeeSequence_TRIndex(Dee_TYPE(ob),ob,findob)
#define /* DEE_A_EXEC Dee_size_t */DeeSequence_Count(ob,findob) DeeSequence_TCount(Dee_TYPE(ob),ob,findob)
#define /* DEE_A_EXEC Dee_ssize_t */DeeSequence_FindPred(ob,findob,pred) DeeSequence_TFindPred(Dee_TYPE(ob),ob,findob,pred)
#define /* DEE_A_EXEC Dee_ssize_t */DeeSequence_RFindPred(ob,findob,pred) DeeSequence_TRFindPred(Dee_TYPE(ob),ob,findob,pred)
#define /* DEE_A_EXEC Dee_size_t */DeeSequence_IndexPred(ob,findob,pred) DeeSequence_TIndexPred(Dee_TYPE(ob),ob,findob,pred)
#define /* DEE_A_EXEC Dee_size_t */DeeSequence_RIndexPred(ob,findob,pred) DeeSequence_TRIndexPred(Dee_TYPE(ob),ob,findob,pred)
#define /* DEE_A_EXEC Dee_size_t */DeeSequence_CountPred(ob,findob,pred) DeeSequence_TCountPred(Dee_TYPE(ob),ob,findob,pred)
#define /* DEE_A_EXEC DeeObject * */DeeSequence_Locate(ob,pred) DeeSequence_TLocate(Dee_TYPE(ob),ob,pred)
#define /* DEE_A_EXEC DeeObject * */DeeSequence_RLocate(ob,pred) DeeSequence_TRLocate(Dee_TYPE(ob),ob,pred)
#define /* DEE_A_EXEC DeeObject * */DeeSequence_LocateAll(ob,pred) DeeSequence_TLocateAll(Dee_TYPE(ob),ob,pred)
#define /* DEE_A_EXEC DeeObject * */DeeSequence_Transform(ob,pred) DeeSequence_TTransform(Dee_TYPE(ob),ob,pred)
#define /* DEE_A_EXEC int */DeeSequence_Empty(ob) DeeSequence_TEmpty(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC int */DeeSequence_NonEmpty(ob) DeeSequence_TNonEmpty(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeSequence_Front(ob) DeeSequence_TFront(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeSequence_Back(ob) DeeSequence_TBack(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeSequence_Get(ob,idx) DeeSequence_TGet(Dee_TYPE(ob),ob,idx)
#define /* DEE_A_EXEC DeeObject * */DeeSequence_At(ob,idx) DeeSequence_TAt(Dee_TYPE(ob),ob,idx)
#define /* DEE_A_EXEC DeeObject * */DeeSequence_Reversed(ob) DeeSequence_TReversed(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeSequence_Sorted(ob) DeeSequence_TSorted(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeSequence_SortedPred(ob,pred) DeeSequence_TSortedPred(Dee_TYPE(ob),ob,pred)
#define /* DEE_A_EXEC DeeObject * */DeeSequence_Segments(ob,n) DeeSequence_TSegments(Dee_TYPE(ob),ob,n)
#define /* DEE_A_EXEC int */DeeSequence_Contains(ob,elem) DeeSequence_TContains(Dee_TYPE(ob),ob,elem)
#define /* DEE_A_EXEC int */DeeSequence_ContainsPred(ob,elem,pred) DeeSequence_TContainsPred(Dee_TYPE(ob),ob,elem,pred)
#define /* DEE_A_EXEC int */DeeSequence_Any(ob) DeeSequence_TAny(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC int */DeeSequence_All(ob) DeeSequence_TAll(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeSequence_Sum(ob) DeeSequence_TSum(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeSequence_Min(ob) DeeSequence_TMin(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeSequence_Max(ob) DeeSequence_TMax(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeSequence_TFind(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeSequence_TRFind(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeSequence_TIndex(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeSequence_TRIndex(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeSequence_TCount(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeSequence_TFindPred(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t) DeeSequence_TRFindPred(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeSequence_TIndexPred(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeSequence_TRIndexPred(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) Dee_size_t) DeeSequence_TCountPred(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSequence_TLocate(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSequence_TRLocate(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSequence_TLocateAll(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSequence_TTransform(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeSequence_TEmpty(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeSequence_TNonEmpty(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSequence_TFront(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSequence_TBack(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSequence_TGet(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_ssize_t idx) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSequence_TAt(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t idx) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSequence_TReversed(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSequence_TSorted(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSequence_TSortedPred(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSequence_TSegments(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t n) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeSequence_TContains(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeSequence_TContainsPred(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeSequence_TAny(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeSequence_TAll(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSequence_TSum(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSequence_TMin(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeSequence_TMax(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeSequence_PEmpty(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeSequence_PNonEmpty(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));


DEE_DECL_END

#endif /* !GUARD_DEEMON_SEQUENCE_H */
