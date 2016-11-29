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
#ifndef GUARD_DEEMON_OPTIONAL_TYPE_NEWINSTANCE_H
#define GUARD_DEEMON_OPTIONAL_TYPE_NEWINSTANCE_H 1

#include <deemon/__conf.inl>
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
// v allocates and returns a new instance of "self" with "args"
DEE_FUNC_DECL(DEE_A_DEEMON("self(args...)") DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeType_NewInstance(
 DEE_A_IN DeeTypeObject const *self, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_DEEMON("self()") DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeType_NewInstanceDefault(
 DEE_A_IN DeeTypeObject const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeType_NewInstancef(DEE_A_IN DeeTypeObject const *self, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeType_NewInstanceObjArgs(DEE_A_IN DeeTypeObject const *self, ...) DEE_ATTRIBUTE_SENTINAL DEE_ATTRIBUTE_NONNULL((1));
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeType_VNewInstancef(DEE_A_IN DeeTypeObject const *self, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeType_VNewInstanceObjArgs(DEE_A_IN DeeTypeObject const *self, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */


DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_TYPE_NEWINSTANCE_H */
