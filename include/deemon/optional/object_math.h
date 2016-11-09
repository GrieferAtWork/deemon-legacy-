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
#ifndef GUARD_DEEMON_OPTIONAL_OBJECT_MATH_H
#define GUARD_DEEMON_OPTIONAL_OBJECT_MATH_H 1

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
// Generic object math interface
#define /* DEE_A_EXEC int */DeeObject_Bool(ob) DeeObject_TBool(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Not(ob) DeeObject_TNot(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Inv(ob) DeeObject_TInv(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Pos(ob) DeeObject_TPos(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Neg(ob) DeeObject_TNeg(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Inc(ob) DeeObject_TInc(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Dec(ob) DeeObject_TDec(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeObject_IncPost(ob) DeeObject_TIncPost(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeObject_DecPost(ob) DeeObject_TDecPost(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Add(lhs,rhs) DeeObject_TAdd(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Sub(lhs,rhs) DeeObject_TSub(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Mul(lhs,rhs) DeeObject_TMul(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Div(lhs,rhs) DeeObject_TDiv(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Mod(lhs,rhs) DeeObject_TMod(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Shl(lhs,rhs) DeeObject_TShl(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Shr(lhs,rhs) DeeObject_TShr(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_And(lhs,rhs) DeeObject_TAnd(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Or(lhs,rhs) DeeObject_TOr(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Xor(lhs,rhs) DeeObject_TXor(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Pow(lhs,rhs) DeeObject_TPow(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_InplaceAdd(lhs,rhs) DeeObject_TInplaceAdd(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_InplaceSub(lhs,rhs) DeeObject_TInplaceSub(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_InplaceMul(lhs,rhs) DeeObject_TInplaceMul(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_InplaceDiv(lhs,rhs) DeeObject_TInplaceDiv(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_InplaceMod(lhs,rhs) DeeObject_TInplaceMod(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_InplaceShl(lhs,rhs) DeeObject_TInplaceShl(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_InplaceShr(lhs,rhs) DeeObject_TInplaceShr(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_InplaceAnd(lhs,rhs) DeeObject_TInplaceAnd(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_InplaceOr(lhs,rhs) DeeObject_TInplaceOr(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_InplaceXor(lhs,rhs) DeeObject_TInplaceXor(Dee_TYPE(lhs),lhs,rhs)
#define /* DEE_A_EXEC DeeObject * */DeeObject_InplacePow(lhs,rhs) DeeObject_TInplacePow(Dee_TYPE(lhs),lhs,rhs)

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TBool(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TNot(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TInv(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TPos(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TNeg(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TInc(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TDec(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TIncPost(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TDecPost(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TAdd(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TSub(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TMul(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TDiv(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TMod(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TShl(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TShr(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TAnd(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TOr(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TXor(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TPow(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TInplaceAdd(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TInplaceSub(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TInplaceMul(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TInplaceDiv(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TInplaceMod(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TInplaceShl(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TInplaceShr(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TInplaceAnd(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TInplaceOr(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TInplaceXor(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TInplacePow(DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PBool(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PNot(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PInv(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PPos(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PNeg(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PInc(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PDec(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PIncPost(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PDecPost(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PAdd(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PSub(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PMul(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PDiv(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PMod(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PShl(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PShr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PAnd(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_POr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PXor(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PPow(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PInplaceAdd(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PInplaceSub(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PInplaceMul(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PInplaceDiv(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PInplaceMod(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PInplaceShl(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PInplaceShr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PInplaceAnd(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PInplaceOr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PInplaceXor(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PInplacePow(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *lhs_tp_self, DEE_A_INOUT void *lhs_base_addr, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2,3));

//////////////////////////////////////////////////////////////////////////
// These versions of the inplace functions are
// used when executing an inplace operator on: 
//  - A static variable
//  - A local variable
//  - A result value variable
//  - An argument value variable
// Unlike the regular inplace operators, these functions
// first try the regular inplace operator, before executing
// the non-inplace operator and storing its result in *lhs.
// NOTE: Even when the real inplace version is available, its result is stored in *lhs as well.
//  - In the end, this is the hack we use to get stuff like this working:
//    >> a = "";
//    >> a += "foo";
//    >> a += "bar";
//    >> print a; // "foobar"
//    Even through 'string' doesn't override, nor could ever implement '__iadd__'
// NOTE: The inc/dec versions are somewhat different, in that they will fall back to
//       calling 'DeeObject_Inplace[Add|Sub]Var(lhs,int(1))', with the post-versions
//       creating a copy that will be returned beforehand.
DEE_FUNC_DECL(DEE_A_DEEMON("++*self") DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_InplaceIncVar(DEE_A_INOUT DeeObject **self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_DEEMON("--*self") DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_InplaceDecVar(DEE_A_INOUT DeeObject **self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_DEEMON("*self++") DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_InplaceIncPostVar(DEE_A_INOUT DeeObject **self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_DEEMON("*self--") DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_InplaceDecPostVar(DEE_A_INOUT DeeObject **self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_DEEMON("*lhs += rhs") DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_InplaceAddVar(DEE_A_INOUT DeeObject **lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_DEEMON("*lhs -= rhs") DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_InplaceSubVar(DEE_A_INOUT DeeObject **lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_DEEMON("*lhs *= rhs") DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_InplaceMulVar(DEE_A_INOUT DeeObject **lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_DEEMON("*lhs /= rhs") DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_InplaceDivVar(DEE_A_INOUT DeeObject **lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_DEEMON("*lhs %= rhs") DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_InplaceModVar(DEE_A_INOUT DeeObject **lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_DEEMON("*lhs <<= rhs") DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_InplaceShlVar(DEE_A_INOUT DeeObject **lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_DEEMON("*lhs >>= rhs") DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_InplaceShrVar(DEE_A_INOUT DeeObject **lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_DEEMON("*lhs &= rhs") DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_InplaceAndVar(DEE_A_INOUT DeeObject **lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_DEEMON("*lhs |= rhs") DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_InplaceOrVar(DEE_A_INOUT DeeObject **lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_DEEMON("*lhs ^= rhs") DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_InplaceXorVar(DEE_A_INOUT DeeObject **lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_DEEMON("*lhs **= rhs") DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_InplacePowVar(DEE_A_INOUT DeeObject **lhs, DEE_A_INOUT DeeObject *rhs) DEE_ATTRIBUTE_NONNULL((1,2));

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_OBJECT_MATH_H */
