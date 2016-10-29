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
#ifndef GUARD_DEEMON_OPTIONAL_OBJECT_STR_H
#define GUARD_DEEMON_OPTIONAL_OBJECT_STR_H 1

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

#define /* DEE_A_EXEC DeeObject * */DeeObject_Str(ob)  DeeObject_TStr(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeObject_Repr(ob) DeeObject_TRepr(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeObject_TStr(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeObject_TRepr(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeObject_PStr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeObject_PRepr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));

#ifdef DEE_LIMITED_API
extern DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *_DeeObject_TStr(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *_DeeObject_TRepr(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
#define /* DEE_A_EXEC DeeObject * */_DeeObject_Str(ob)  _DeeObject_TStr(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */_DeeObject_Repr(ob) _DeeObject_TRepr(Dee_TYPE(ob),ob)
extern DeeObject *_DeeObject_RecursiveString;
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_OBJECT_STR_H */
