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
#ifndef GUARD_DEEMON_OPTIONAL_OBJECT_COMPARE_H
#define GUARD_DEEMON_OPTIONAL_OBJECT_COMPARE_H 1

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
// Generic compare interface
#define /* DEE_A_EXEC int */DeeObject_CompareLo(lhs,rhs)               DeeObject_TCompareLo(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC int */DeeObject_CompareLe(lhs,rhs)               DeeObject_TCompareLe(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC int */DeeObject_CompareEq(lhs,rhs)               DeeObject_TCompareEq(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC int */DeeObject_CompareNe(lhs,rhs)               DeeObject_TCompareNe(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC int */DeeObject_CompareGr(lhs,rhs)               DeeObject_TCompareGr(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC int */DeeObject_CompareGe(lhs,rhs)               DeeObject_TCompareGe(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_CompareLoObject(lhs,rhs) DeeObject_TCompareLoObject(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_CompareLeObject(lhs,rhs) DeeObject_TCompareLeObject(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_CompareEqObject(lhs,rhs) DeeObject_TCompareEqObject(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_CompareNeObject(lhs,rhs) DeeObject_TCompareNeObject(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_CompareGrObject(lhs,rhs) DeeObject_TCompareGrObject(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_CompareGeObject(lhs,rhs) DeeObject_TCompareGeObject(Dee_TYPE(lhs),lhs,rhs)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TCompareLo(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TCompareLe(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TCompareEq(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TCompareNe(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TCompareGr(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TCompareGe(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TCompareLoObject(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TCompareLeObject(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TCompareEqObject(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TCompareNeObject(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TCompareGrObject(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TCompareGeObject(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PCompareLo(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PCompareLe(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PCompareEq(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PCompareNe(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PCompareGr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PCompareGe(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PCompareLoObject(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PCompareLeObject(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PCompareEqObject(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PCompareNeObject(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PCompareGrObject(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PCompareGeObject(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));

//////////////////////////////////////////////////////////////////////////
// Deep object comparison
//  - For two objects to be equal, they must share the same type, as well as compare equal.
//  - For contains, all contained objects must be deep-equal as well.
//  - Unlike 'DeeObject_CompareEq', deep_equals guaranties equality if '1' is returned
// NOTE: This function is used for same-constant optimization in usercode
// NOTE: Returns 2 if the objects are most likely equal, but proving so isn't possible.
#define DEE_OBJECT_DEEPEQUALS_FALSE  0
#define DEE_OBJECT_DEEPEQUALS_TRUE   1
#define DEE_OBJECT_DEEPEQUALS_LIKELY 2
#define /*DEE_A_EXEC*/ DeeObject_DeepEquals(lhs,rhs) DeeObject_TDeepEquals(Dee_TYPE(lhs),lhs,rhs)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeObject_TDeepEquals(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));

DEE_DECL_END


#endif /* !GUARD_DEEMON_OPTIONAL_OBJECT_COMPARE_H */
