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
#ifndef GUARD_DEEMON_CFUNCTION_H
#define GUARD_DEEMON_CFUNCTION_H 1

#include <deemon/__conf.inl>

DEE_DECL_BEGIN

DEE_OBJECT_DEF(DeeCFunctionObject);

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_CFUNCTION
DEE_PRIVATE_DECL_DEE_CFUNCTION
#undef DEE_PRIVATE_DECL_DEE_CFUNCTION
#endif

//////////////////////////////////////////////////////////////////////////
// Simple CFunction wrapper object
DEE_DATA_DECL(DeeTypeObject) DeeCFunction_Type;

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeCFunctionObject) *) DeeCFunction_New(
 DEE_A_IN_Z_OPT char const *name, DEE_A_IN DeeCFunction func) DEE_ATTRIBUTE_NONNULL((2));
DEE_FUNC_DECL(DEE_A_RET_NEVER_NULL DEE_A_RET_WUNUSED DeeCFunction) DeeCFunction_Func(
 DEE_A_IN_OBJECT(DeeCFunctionObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC_IF(DeeCFunction_CALL(self,args)) DEE_A_RET_EXCEPT_REF DeeObject *) DeeCFunction_Call(
 DEE_A_IN_OBJECT(DeeCFunctionObject) const *self, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2));

#ifdef DEE_DEBUG
#define DEE_PRIVATE_OBJECT_NEW_DeeCFunction(f) DeeCFunction_New(#f,f)
#define DEE_PRIVATE_OBJECT_NEW_cfunction(f)    DeeCFunction_New(#f,f)
#else
#define DEE_PRIVATE_OBJECT_NEW_DeeCFunction(f) DeeCFunction_New(NULL,f)
#define DEE_PRIVATE_OBJECT_NEW_cfunction(f)    DeeCFunction_New(NULL,f)
#endif
#define DEE_PRIVATE_OBJECT_TYPE_DeeCFunction   &DeeCFunction_Type
#define DEE_PRIVATE_OBJECT_TYPE_cfunction      &DeeCFunction_Type

DEE_DECL_END

#ifdef GUARD_DEEMON_OBJECT_H
#ifndef GUARD_DEEMON_OPTIONAL_CFUNCTION_DECL_H
#include <deemon/optional/cfunction_decl.h>
#endif
#endif

#endif /* !GUARD_DEEMON_CFUNCTION_H */
