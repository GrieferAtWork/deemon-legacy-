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
#ifndef GUARD_DEEMON_FLOAT_H
#define GUARD_DEEMON_FLOAT_H 1

#include <deemon/__conf.inl>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
#undef DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
#endif

DEE_OBJECT_DEF(DeeFloatObject);
DEE_OBJECT_DEF(DeeDoubleObject);
DEE_OBJECT_DEF(DeeLDoubleObject);

//////////////////////////////////////////////////////////////////////////
// NOTE: Be careful with these checks, as they fail if 'ob' is an lvalue to the given float type
#define DeeFloat_Check(ob) DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeFloat_Type)
#define DeeDouble_Check(ob) DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeDouble_Type)
#define DeeLDouble_Check(ob) DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeLDouble_Type)
#define DeeFloat_CheckExact(ob) DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeFloat_Type)
#define DeeDouble_CheckExact(ob) DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeDouble_Type)
#define DeeLDouble_CheckExact(ob) DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeLDouble_Type)
DEE_DATA_DECL(DeeStructuredTypeObject) DeeFloat_Type;
DEE_DATA_DECL(DeeStructuredTypeObject) DeeDouble_Type;
DEE_DATA_DECL(DeeStructuredTypeObject) DeeLDouble_Type;

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeFloatObject) *) DeeFloat_New(DEE_A_IN Dee_rt_float x);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDoubleObject) *) DeeDouble_New(DEE_A_IN Dee_rt_double x);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeLDoubleObject) *) DeeLDouble_New(DEE_A_IN Dee_rt_ldouble x);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_rt_float) DeeFloat_Value(DEE_A_IN_OBJECT(DeeFloatObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_rt_double) DeeDouble_Value(DEE_A_IN_OBJECT(DeeDoubleObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_rt_ldouble) DeeLDouble_Value(DEE_A_IN_OBJECT(DeeLDoubleObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

DEE_FUNC_DECL(Dee_size_t) DeeFloat_ClearPool(void);

#define DEE_PRIVATE_OBJECT_TYPE_Dee_rt_float   ((DeeTypeObject *)&DeeFloat_Type)
#define DEE_PRIVATE_OBJECT_TYPE_Dee_rt_double  ((DeeTypeObject *)&DeeDouble_Type)
#define DEE_PRIVATE_OBJECT_TYPE_Dee_rt_ldouble ((DeeTypeObject *)&DeeLDouble_Type)
#define DEE_PRIVATE_OBJECT_NEW_Dee_rt_float    DeeFloat_New
#define DEE_PRIVATE_OBJECT_NEW_Dee_rt_double   DeeDouble_New
#define DEE_PRIVATE_OBJECT_NEW_Dee_rt_ldouble  DeeLDouble_New
#ifdef DEE_TYPES_FLOATID_FLOAT
#if DEE_TYPES_FLOATID_FLOAT == DEE_CONFIG_RT_FLOATID_FLOAT
#define DEE_PRIVATE_OBJECT_TYPE_float          ((DeeTypeObject *)&DeeFloat_Type)
#define DEE_PRIVATE_OBJECT_NEW_float           DeeFloat_New
#elif DEE_TYPES_FLOATID_FLOAT == DEE_CONFIG_RT_FLOATID_DOUBLE
#define DEE_PRIVATE_OBJECT_TYPE_float          ((DeeTypeObject *)&DeeDouble_Type)
#define DEE_PRIVATE_OBJECT_NEW_float           DeeDouble_New
#elif DEE_TYPES_FLOATID_FLOAT == DEE_CONFIG_RT_FLOATID_LDOUBLE
#define DEE_PRIVATE_OBJECT_TYPE_float          ((DeeTypeObject *)&DeeLDouble_Type)
#define DEE_PRIVATE_OBJECT_NEW_float           DeeLDouble_New
#endif
#endif
#ifdef DEE_TYPES_FLOATID_DOUBLE
#if DEE_TYPES_FLOATID_DOUBLE == DEE_CONFIG_RT_FLOATID_FLOAT
#define DEE_PRIVATE_OBJECT_TYPE_double         ((DeeTypeObject *)&DeeFloat_Type)
#define DEE_PRIVATE_OBJECT_NEW_double          DeeFloat_New
#elif DEE_TYPES_FLOATID_DOUBLE == DEE_CONFIG_RT_FLOATID_DOUBLE
#define DEE_PRIVATE_OBJECT_TYPE_double         ((DeeTypeObject *)&DeeDouble_Type)
#define DEE_PRIVATE_OBJECT_NEW_double          DeeDouble_New
#elif DEE_TYPES_FLOATID_DOUBLE == DEE_CONFIG_RT_FLOATID_LDOUBLE
#define DEE_PRIVATE_OBJECT_TYPE_double         ((DeeTypeObject *)&DeeLDouble_Type)
#define DEE_PRIVATE_OBJECT_NEW_double          DeeLDouble_New
#endif
#endif
#ifdef DEE_TYPES_FLOATID_LDOUBLE
#if DEE_TYPES_FLOATID_LDOUBLE == DEE_CONFIG_RT_FLOATID_FLOAT
#define DEE_PRIVATE_OBJECT_TYPE_ldouble        ((DeeTypeObject *)&DeeFloat_Type)
#define DEE_PRIVATE_OBJECT_NEW_ldouble         DeeFloat_New
#elif DEE_TYPES_FLOATID_LDOUBLE == DEE_CONFIG_RT_FLOATID_DOUBLE
#define DEE_PRIVATE_OBJECT_TYPE_ldouble        ((DeeTypeObject *)&DeeDouble_Type)
#define DEE_PRIVATE_OBJECT_NEW_ldouble         DeeDouble_New
#elif DEE_TYPES_FLOATID_LDOUBLE == DEE_CONFIG_RT_FLOATID_LDOUBLE
#define DEE_PRIVATE_OBJECT_TYPE_ldouble        ((DeeTypeObject *)&DeeLDouble_Type)
#define DEE_PRIVATE_OBJECT_NEW_ldouble         DeeLDouble_New
#endif
#endif

DEE_DECL_END

#if defined(GUARD_DEEMON_OBJECT_H) && defined(GUARD_DEEMON_STRUCTURED_H)
#include <deemon/optional/float_decl.h>
#endif

#endif /* !GUARD_DEEMON_FLOAT_H */
