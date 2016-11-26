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
#ifndef GUARD_DEEMON_INTEGER_H
#define GUARD_DEEMON_INTEGER_H 1

#include <deemon/__conf.inl>
#ifdef DEE_LIMITED_API
#include <deemon/error.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
#endif

DEE_OBJECT_DEF(DeeInt8Object);
DEE_OBJECT_DEF(DeeInt16Object);
DEE_OBJECT_DEF(DeeInt32Object);
DEE_OBJECT_DEF(DeeInt64Object);
DEE_OBJECT_DEF(DeeRangeInt8IteratorObject);
DEE_OBJECT_DEF(DeeRangeUInt8IteratorObject);
DEE_OBJECT_DEF(DeeRangeInt16IteratorObject);
DEE_OBJECT_DEF(DeeRangeUInt16IteratorObject);
DEE_OBJECT_DEF(DeeRangeInt32IteratorObject);
DEE_OBJECT_DEF(DeeRangeUInt32IteratorObject);
DEE_OBJECT_DEF(DeeRangeInt64IteratorObject);
DEE_OBJECT_DEF(DeeRangeUInt64IteratorObject);

typedef enum DeeMemoryOrder {
 // memory order values used in the member functions of atomics
 Dee_memory_order_relaxed = 0,
 Dee_memory_order_consume = 1,
 Dee_memory_order_acquire = 2,
 Dee_memory_order_release = 3,
 Dee_memory_order_acq_rel = 4,
 Dee_memory_order_seq_cst = 5
} DeeMemoryOrder;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Integer
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DEE_DATA_DECL(DeeStructuredTypeObject) DeeInt8_Type;
DEE_DATA_DECL(DeeStructuredTypeObject) DeeInt16_Type;
DEE_DATA_DECL(DeeStructuredTypeObject) DeeInt32_Type;
DEE_DATA_DECL(DeeStructuredTypeObject) DeeInt64_Type;
DEE_DATA_DECL(DeeStructuredTypeObject) DeeUInt8_Type;
DEE_DATA_DECL(DeeStructuredTypeObject) DeeUInt16_Type;
DEE_DATA_DECL(DeeStructuredTypeObject) DeeUInt32_Type;
DEE_DATA_DECL(DeeStructuredTypeObject) DeeUInt64_Type;

// NOTE: Atomic integer types are derived from the non-atomic integers
//       >> Meaning that 'atomic(42) is int' evaluates to true
DEE_DATA_DECL(DeeStructuredTypeObject) DeeAtomicInt8_Type;
DEE_DATA_DECL(DeeStructuredTypeObject) DeeAtomicInt16_Type;
DEE_DATA_DECL(DeeStructuredTypeObject) DeeAtomicInt32_Type;
DEE_DATA_DECL(DeeStructuredTypeObject) DeeAtomicInt64_Type;
DEE_DATA_DECL(DeeStructuredTypeObject) DeeAtomicUInt8_Type;
DEE_DATA_DECL(DeeStructuredTypeObject) DeeAtomicUInt16_Type;
DEE_DATA_DECL(DeeStructuredTypeObject) DeeAtomicUInt32_Type;
DEE_DATA_DECL(DeeStructuredTypeObject) DeeAtomicUInt64_Type;

DEE_DATA_DECL(DeeTypeObject) DeeRangeInt8Iterator_Type;
DEE_DATA_DECL(DeeTypeObject) DeeRangeInt16Iterator_Type;
DEE_DATA_DECL(DeeTypeObject) DeeRangeInt32Iterator_Type;
DEE_DATA_DECL(DeeTypeObject) DeeRangeInt64Iterator_Type;
DEE_DATA_DECL(DeeTypeObject) DeeRangeUInt8Iterator_Type;
DEE_DATA_DECL(DeeTypeObject) DeeRangeUInt16Iterator_Type;
DEE_DATA_DECL(DeeTypeObject) DeeRangeUInt32Iterator_Type;
DEE_DATA_DECL(DeeTypeObject) DeeRangeUInt64Iterator_Type;

#define DeeRangeInt8Iterator_Check(ob)        DeeObject_InstanceOf(ob,&DeeRangeInt8Iterator_Type)
#define DeeRangeInt16Iterator_Check(ob)       DeeObject_InstanceOf(ob,&DeeRangeInt16Iterator_Type)
#define DeeRangeInt32Iterator_Check(ob)       DeeObject_InstanceOf(ob,&DeeRangeInt32Iterator_Type)
#define DeeRangeInt64Iterator_Check(ob)       DeeObject_InstanceOf(ob,&DeeRangeInt64Iterator_Type)
#define DeeRangeUInt8Iterator_Check(ob)       DeeObject_InstanceOf(ob,&DeeRangeUInt8Iterator_Type)
#define DeeRangeUInt16Iterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeRangeUInt16Iterator_Type)
#define DeeRangeUInt32Iterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeRangeUInt32Iterator_Type)
#define DeeRangeUInt64Iterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeRangeUInt64Iterator_Type)
#define DeeRangeInt8Iterator_CheckExact(ob)   DeeObject_InstanceOfExact(ob,&DeeRangeInt8Iterator_Type)
#define DeeRangeInt16Iterator_CheckExact(ob)  DeeObject_InstanceOfExact(ob,&DeeRangeInt16Iterator_Type)
#define DeeRangeInt32Iterator_CheckExact(ob)  DeeObject_InstanceOfExact(ob,&DeeRangeInt32Iterator_Type)
#define DeeRangeInt64Iterator_CheckExact(ob)  DeeObject_InstanceOfExact(ob,&DeeRangeInt64Iterator_Type)
#define DeeRangeUInt8Iterator_CheckExact(ob)  DeeObject_InstanceOfExact(ob,&DeeRangeUInt8Iterator_Type)
#define DeeRangeUInt16Iterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeRangeUInt16Iterator_Type)
#define DeeRangeUInt32Iterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeRangeUInt32Iterator_Type)
#define DeeRangeUInt64Iterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeRangeUInt64Iterator_Type)


//////////////////////////////////////////////////////////////////////////
// NOTE: Be careful with these checks, as they fail if 'ob' is an lvalue to the given int type
#define DeeInt8_Check(ob)              DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeInt8_Type)
#define DeeInt16_Check(ob)             DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeInt16_Type)
#define DeeInt32_Check(ob)             DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeInt32_Type)
#define DeeInt64_Check(ob)             DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeInt64_Type)
#define DeeUInt8_Check(ob)             DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeUInt8_Type)
#define DeeUInt16_Check(ob)            DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeUInt16_Type)
#define DeeUInt32_Check(ob)            DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeUInt32_Type)
#define DeeUInt64_Check(ob)            DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeUInt64_Type)
#define DeeAtomicInt8_Check(ob)        DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeAtomicInt8_Type)
#define DeeAtomicInt16_Check(ob)       DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeAtomicInt16_Type)
#define DeeAtomicInt32_Check(ob)       DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeAtomicInt32_Type)
#define DeeAtomicInt64_Check(ob)       DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeAtomicInt64_Type)
#define DeeAtomicUInt8_Check(ob)       DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeAtomicUInt8_Type)
#define DeeAtomicUInt16_Check(ob)      DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeAtomicUInt16_Type)
#define DeeAtomicUInt32_Check(ob)      DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeAtomicUInt32_Type)
#define DeeAtomicUInt64_Check(ob)      DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeAtomicUInt64_Type)
#define DeeInt8_CheckExact(ob)         DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeInt8_Type)
#define DeeInt16_CheckExact(ob)        DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeInt16_Type)
#define DeeInt32_CheckExact(ob)        DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeInt32_Type)
#define DeeInt64_CheckExact(ob)        DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeInt64_Type)
#define DeeUInt8_CheckExact(ob)        DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeUInt8_Type)
#define DeeUInt16_CheckExact(ob)       DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeUInt16_Type)
#define DeeUInt32_CheckExact(ob)       DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeUInt32_Type)
#define DeeUInt64_CheckExact(ob)       DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeUInt64_Type)
#define DeeAtomicInt8_CheckExact(ob)   DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeAtomicInt8_Type)
#define DeeAtomicInt16_CheckExact(ob)  DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeAtomicInt16_Type)
#define DeeAtomicInt32_CheckExact(ob)  DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeAtomicInt32_Type)
#define DeeAtomicInt64_CheckExact(ob)  DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeAtomicInt64_Type)
#define DeeAtomicUInt8_CheckExact(ob)  DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeAtomicUInt8_Type)
#define DeeAtomicUInt16_CheckExact(ob) DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeAtomicUInt16_Type)
#define DeeAtomicUInt32_CheckExact(ob) DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeAtomicUInt32_Type)
#define DeeAtomicUInt64_CheckExact(ob) DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeAtomicUInt64_Type)
#define DeeSignedIntegral_Check(ob) (DeeInt8_Check(ob)||DeeInt16_Check(ob)||DeeInt32_Check(ob)||DeeInt64_Check(ob))
#define DeeSignedIntegral_CheckExact(ob) (DeeInt8_CheckExact(ob)||DeeInt16_CheckExact(ob)||DeeInt32_CheckExact(ob)||DeeInt64_CheckExact(ob))
#define DeeUnsignedIntegral_Check(ob) (DeeUInt8_Check(ob)||DeeUInt16_Check(ob)||DeeUInt32_Check(ob)||DeeUInt64_Check(ob))
#define DeeUnsignedIntegral_CheckExact(ob) (DeeUInt8_CheckExact(ob)||DeeUInt16_CheckExact(ob)||DeeUInt32_CheckExact(ob)||DeeUInt64_CheckExact(ob))
#define DeeAtomicSignedIntegral_Check(ob) (DeeAtomicInt8_Check(ob)||DeeAtomicInt16_Check(ob)||DeeAtomicInt32_Check(ob)||DeeAtomicInt64_Check(ob))
#define DeeAtomicSignedIntegral_CheckExact(ob) (DeeAtomicInt8_CheckExact(ob)||DeeAtomicInt16_CheckExact(ob)||DeeAtomicInt32_CheckExact(ob)||DeeAtomicInt64_CheckExact(ob))
#define DeeAtomicUnsignedIntegral_Check(ob) (DeeAtomicUInt8_Check(ob)||DeeAtomicUInt16_Check(ob)||DeeAtomicUInt32_Check(ob)||DeeAtomicUInt64_Check(ob))
#define DeeAtomicUnsignedIntegral_CheckExact(ob) (DeeAtomicUInt8_CheckExact(ob)||DeeAtomicUInt16_CheckExact(ob)||DeeAtomicUInt32_CheckExact(ob)||DeeAtomicUInt64_CheckExact(ob))
#define DeeIntegral_Check(ob) (DeeSignedIntegral_Check(ob)||DeeUnsignedIntegral_Check(ob))
#define DeeIntegral_CheckExact(ob) (DeeSignedIntegral_CheckExact(ob)||DeeUnsignedIntegral_CheckExact(ob))
#define DeeAtomicIntegral_Check(ob) (DeeAtomicSignedIntegral_Check(ob)||DeeAtomicUnsignedIntegral_Check(ob))
#define DeeAtomicIntegral_CheckExact(ob) (DeeAtomicSignedIntegral_CheckExact(ob)||DeeAtomicUnsignedIntegral_CheckExact(ob))

DEE_FUNC_DECL(Dee_size_t) DeeInt_ClearPool(void);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeInt8Object) *) DeeInt8_New(DEE_A_IN Dee_int8_t v);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeInt16Object) *) DeeInt16_New(DEE_A_IN Dee_int16_t v);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeInt32Object) *) DeeInt32_New(DEE_A_IN Dee_int32_t v);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeInt64Object) *) DeeInt64_New(DEE_A_IN Dee_int64_t v);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeInt8Object) *) DeeUInt8_New(DEE_A_IN Dee_uint8_t v);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeInt16Object) *) DeeUInt16_New(DEE_A_IN Dee_uint16_t v);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeInt32Object) *) DeeUInt32_New(DEE_A_IN Dee_uint32_t v);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeInt64Object) *) DeeUInt64_New(DEE_A_IN Dee_uint64_t v);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeInt8Object) *) DeeAtomicInt8_New(DEE_A_IN Dee_int8_t v);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeInt16Object) *) DeeAtomicInt16_New(DEE_A_IN Dee_int16_t v);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeInt32Object) *) DeeAtomicInt32_New(DEE_A_IN Dee_int32_t v);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeInt64Object) *) DeeAtomicInt64_New(DEE_A_IN Dee_int64_t v);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeInt8Object) *) DeeAtomicUInt8_New(DEE_A_IN Dee_uint8_t v);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeInt16Object) *) DeeAtomicUInt16_New(DEE_A_IN Dee_uint16_t v);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeInt32Object) *) DeeAtomicUInt32_New(DEE_A_IN Dee_uint32_t v);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeInt64Object) *) DeeAtomicUInt64_New(DEE_A_IN Dee_uint64_t v);

//////////////////////////////////////////////////////////////////////////
// Makes sure '*ob' is an instance of the given type.
// If it isn't, call the appropriate cast operator and store its value in '*ob'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeInt8_Force(DEE_A_REF DEE_A_INOUT_OBJECT(DeeInt8Object) **ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeInt16_Force(DEE_A_REF DEE_A_INOUT_OBJECT(DeeInt16Object) **ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeInt32_Force(DEE_A_REF DEE_A_INOUT_OBJECT(DeeInt32Object) **ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeInt64_Force(DEE_A_REF DEE_A_INOUT_OBJECT(DeeInt64Object) **ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeUInt8_Force(DEE_A_REF DEE_A_INOUT_OBJECT(DeeInt8Object) **ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeUInt16_Force(DEE_A_REF DEE_A_INOUT_OBJECT(DeeInt16Object) **ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeUInt32_Force(DEE_A_REF DEE_A_INOUT_OBJECT(DeeInt32Object) **ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeUInt64_Force(DEE_A_REF DEE_A_INOUT_OBJECT(DeeInt64Object) **ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeAtomicInt8_Force(DEE_A_REF DEE_A_INOUT_OBJECT(DeeInt8Object) **ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeAtomicInt16_Force(DEE_A_REF DEE_A_INOUT_OBJECT(DeeInt16Object) **ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeAtomicInt32_Force(DEE_A_REF DEE_A_INOUT_OBJECT(DeeInt32Object) **ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeAtomicInt64_Force(DEE_A_REF DEE_A_INOUT_OBJECT(DeeInt64Object) **ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeAtomicUInt8_Force(DEE_A_REF DEE_A_INOUT_OBJECT(DeeInt8Object) **ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeAtomicUInt16_Force(DEE_A_REF DEE_A_INOUT_OBJECT(DeeInt16Object) **ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeAtomicUInt32_Force(DEE_A_REF DEE_A_INOUT_OBJECT(DeeInt32Object) **ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeAtomicUInt64_Force(DEE_A_REF DEE_A_INOUT_OBJECT(DeeInt64Object) **ob) DEE_ATTRIBUTE_NONNULL((1));

#define DEE_PRIVATE_XINT_TYPE_0        DeeUINT_Type
#define DEE_PRIVATE_XINT_TYPE_1        DeeINT_Type
#define DEE_PRIVATE_ATOMIC_XINT_TYPE_0 DeeAtomicUINT_Type
#define DEE_PRIVATE_ATOMIC_XINT_TYPE_1 DeeAtomicINT_Type
#define DEE_PRIVATE_XINT_NEW_0         DeeUINT_New_F
#define DEE_PRIVATE_XINT_NEW_1         DeeINT_New_F
#define DEE_PRIVATE_ATOMIC_XINT_NEW_0  DeeAtomicUINT_New_F
#define DEE_PRIVATE_ATOMIC_XINT_NEW_1  DeeAtomicINT_New_F
#define DeeXINT_Type(signed,sizeof)        DEE_PRIVATE_PP_CAT_2(DEE_PRIVATE_XINT_TYPE_,signed)(sizeof)
#define DeeAtomicXINT_Type(signed,sizeof)  DEE_PRIVATE_PP_CAT_2(DEE_PRIVATE_ATOMIC_XINT_TYPE_,signed)(sizeof)
#define DeeXINT_New_F(signed,sizeof)       DEE_PRIVATE_PP_CAT_2(DEE_PRIVATE_XINT_NEW_,signed)(sizeof)
#define DeeAtomicXINT_New_F(signed,sizeof) DEE_PRIVATE_PP_CAT_2(DEE_PRIVATE_ATOMIC_XINT_NEW_,signed)(sizeof)
#define DeeXINT_New(signed,sizeof,v)       DeeXINT_New_F(signed,sizeof)(v)
#define DeeAtomicXINT_New(signed,sizeof,v) DeeAtomicXINT_New_F(signed,sizeof)(v)

#define DeeINTObject(sizeof)                 DEE_PP_CAT_3(DeeInt,DEE_PP_MUL8(sizeof),Object)
#define DeeINT_VALUE_F(sizeof)               DEE_PP_CAT_3(DeeInt,DEE_PP_MUL8(sizeof),_VALUE)
#define DeeUINT_VALUE_F(sizeof)              DEE_PP_CAT_3(DeeUInt,DEE_PP_MUL8(sizeof),_VALUE)
#define DeeINT_VALUE(sizeof,ob)              DeeINT_VALUE_F(sizeof)(ob)
#define DeeUINT_VALUE(sizeof,ob)             DeeUINT_VALUE_F(sizeof)(ob)
#define DeeINT_Type(sizeof)                  DEE_PP_CAT_3(DeeInt,DEE_PP_MUL8(sizeof),_Type)
#define DeeUINT_Type(sizeof)                 DEE_PP_CAT_3(DeeUInt,DEE_PP_MUL8(sizeof),_Type)
#define DeeAtomicINT_Type(sizeof)            DEE_PP_CAT_3(DeeAtomicInt,DEE_PP_MUL8(sizeof),_Type)
#define DeeAtomicUINT_Type(sizeof)           DEE_PP_CAT_3(DeeAtomicUInt,DEE_PP_MUL8(sizeof),_Type)
#define DeeINT_Check_F(sizeof)               DEE_PP_CAT_3(DeeInt,DEE_PP_MUL8(sizeof),_Check)
#define DeeUINT_Check_F(sizeof)              DEE_PP_CAT_3(DeeUInt,DEE_PP_MUL8(sizeof),_Check)
#define DeeAtomicINT_Check_F(sizeof)         DEE_PP_CAT_3(DeeAtomicInt,DEE_PP_MUL8(sizeof),_Check)
#define DeeAtomicUINT_Check_F(sizeof)        DEE_PP_CAT_3(DeeAtomicUInt,DEE_PP_MUL8(sizeof),_Check)
#define DeeINT_Check(sizeof,ob)              DeeINT_Check_F(sizeof)(ob)
#define DeeUINT_Check(sizeof,ob)             DeeUINT_Check_F(sizeof)(ob)
#define DeeAtomicINT_Check(sizeof,ob)        DeeAtomicINT_Check_F(sizeof)(ob)
#define DeeAtomicUINT_Check(sizeof,ob)       DeeAtomicUINT_Check_F(sizeof)(ob)
#define DeeINT_New_F(sizeof)                 DEE_PP_CAT_3(DeeInt,DEE_PP_MUL8(sizeof),_New)
#define DeeUINT_New_F(sizeof)                DEE_PP_CAT_3(DeeUInt,DEE_PP_MUL8(sizeof),_New)
#define DeeAtomicINT_New_F(sizeof)           DEE_PP_CAT_3(DeeAtomicInt,DEE_PP_MUL8(sizeof),_New)
#define DeeAtomicUINT_New_F(sizeof)          DEE_PP_CAT_3(DeeAtomicUInt,DEE_PP_MUL8(sizeof),_New)
#define DeeINT_New(sizeof,v)                 DeeINT_New_F(sizeof)(v)
#define DeeUINT_New(sizeof,v)                DeeUINT_New_F(sizeof)(v)
#define DeeAtomicINT_New(sizeof,v)           DeeAtomicINT_New_F(sizeof)(v)
#define DeeAtomicUINT_New(sizeof,v)          DeeAtomicUINT_New_F(sizeof)(v)

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeRangeInt8IteratorObject) *) DeeRangeInt8Iterator_New(DEE_A_IN Dee_int8_t lo, DEE_A_IN Dee_int8_t hi, DEE_A_IN Dee_int8_t step);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeRangeInt16IteratorObject) *) DeeRangeInt16Iterator_New(DEE_A_IN Dee_int16_t lo, DEE_A_IN Dee_int16_t hi, DEE_A_IN Dee_int16_t step);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeRangeInt32IteratorObject) *) DeeRangeInt32Iterator_New(DEE_A_IN Dee_int32_t lo, DEE_A_IN Dee_int32_t hi, DEE_A_IN Dee_int32_t step);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeRangeInt64IteratorObject) *) DeeRangeInt64Iterator_New(DEE_A_IN Dee_int64_t lo, DEE_A_IN Dee_int64_t hi, DEE_A_IN Dee_int64_t step);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeRangeUInt8IteratorObject) *) DeeRangeUInt8Iterator_New(DEE_A_IN Dee_uint8_t lo, DEE_A_IN Dee_uint8_t hi, DEE_A_IN Dee_uint8_t step);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeRangeUInt16IteratorObject) *) DeeRangeUInt16Iterator_New(DEE_A_IN Dee_uint16_t lo, DEE_A_IN Dee_uint16_t hi, DEE_A_IN Dee_uint16_t step);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeRangeUInt32IteratorObject) *) DeeRangeUInt32Iterator_New(DEE_A_IN Dee_uint32_t lo, DEE_A_IN Dee_uint32_t hi, DEE_A_IN Dee_uint32_t step);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeRangeUInt64IteratorObject) *) DeeRangeUInt64Iterator_New(DEE_A_IN Dee_uint64_t lo, DEE_A_IN Dee_uint64_t hi, DEE_A_IN Dee_uint64_t step);

#ifdef DEE_LIMITED_API
extern DeeErrorObject _DeeError_ValueError_RangeZeroStep;
#endif

//////////////////////////////////////////////////////////////////////////
// Defines for 'DeeObject_New' / 'DeeObject_TYPE'
#define DEE_PRIVATE_OBJECT_NEW_atomic(x)         DEE_PRIVATE_OBJECT_NEW_atomic_##x
#define DEE_PRIVATE_OBJECT_TYPE_atomic(x)        DEE_PRIVATE_OBJECT_TYPE_atomic_##x
#define DEE_PRIVATE_OBJECT_NEW_int8_t            DeeInt8_New
#define DEE_PRIVATE_OBJECT_NEW_int16_t           DeeInt16_New
#define DEE_PRIVATE_OBJECT_NEW_int32_t           DeeInt32_New
#define DEE_PRIVATE_OBJECT_NEW_int64_t           DeeInt64_New
#define DEE_PRIVATE_OBJECT_NEW_uint8_t           DeeUInt8_New
#define DEE_PRIVATE_OBJECT_NEW_uint16_t          DeeUInt16_New
#define DEE_PRIVATE_OBJECT_NEW_uint32_t          DeeUInt32_New
#define DEE_PRIVATE_OBJECT_NEW_uint64_t          DeeUInt64_New
#define DEE_PRIVATE_OBJECT_NEW_atomic_int8_t     DeeAtomicInt8_New
#define DEE_PRIVATE_OBJECT_NEW_atomic_int16_t    DeeAtomicInt16_New
#define DEE_PRIVATE_OBJECT_NEW_atomic_int32_t    DeeAtomicInt32_New
#define DEE_PRIVATE_OBJECT_NEW_atomic_int64_t    DeeAtomicInt64_New
#define DEE_PRIVATE_OBJECT_NEW_atomic_uint8_t    DeeAtomicUInt8_New
#define DEE_PRIVATE_OBJECT_NEW_atomic_uint16_t   DeeAtomicUInt16_New
#define DEE_PRIVATE_OBJECT_NEW_atomic_uint32_t   DeeAtomicUInt32_New
#define DEE_PRIVATE_OBJECT_NEW_atomic_uint64_t   DeeAtomicUInt64_New
#define DEE_PRIVATE_OBJECT_TYPE_int8_t           ((DeeTypeObject *)&DeeInt8_Type)
#define DEE_PRIVATE_OBJECT_TYPE_int16_t          ((DeeTypeObject *)&DeeInt16_Type)
#define DEE_PRIVATE_OBJECT_TYPE_int32_t          ((DeeTypeObject *)&DeeInt32_Type)
#define DEE_PRIVATE_OBJECT_TYPE_int64_t          ((DeeTypeObject *)&DeeInt64_Type)
#define DEE_PRIVATE_OBJECT_TYPE_uint8_t          ((DeeTypeObject *)&DeeUInt8_Type)
#define DEE_PRIVATE_OBJECT_TYPE_uint16_t         ((DeeTypeObject *)&DeeUInt16_Type)
#define DEE_PRIVATE_OBJECT_TYPE_uint32_t         ((DeeTypeObject *)&DeeUInt32_Type)
#define DEE_PRIVATE_OBJECT_TYPE_uint64_t         ((DeeTypeObject *)&DeeUInt64_Type)
#define DEE_PRIVATE_OBJECT_TYPE_atomic_int8_t    ((DeeTypeObject *)&DeeAtomicInt8_Type)
#define DEE_PRIVATE_OBJECT_TYPE_atomic_int16_t   ((DeeTypeObject *)&DeeAtomicInt16_Type)
#define DEE_PRIVATE_OBJECT_TYPE_atomic_int32_t   ((DeeTypeObject *)&DeeAtomicInt32_Type)
#define DEE_PRIVATE_OBJECT_TYPE_atomic_int64_t   ((DeeTypeObject *)&DeeAtomicInt64_Type)
#define DEE_PRIVATE_OBJECT_TYPE_atomic_uint8_t   ((DeeTypeObject *)&DeeAtomicUInt8_Type)
#define DEE_PRIVATE_OBJECT_TYPE_atomic_uint16_t  ((DeeTypeObject *)&DeeAtomicUInt16_Type)
#define DEE_PRIVATE_OBJECT_TYPE_atomic_uint32_t  ((DeeTypeObject *)&DeeAtomicUInt32_Type)
#define DEE_PRIVATE_OBJECT_TYPE_atomic_uint64_t  ((DeeTypeObject *)&DeeAtomicUInt64_Type)

#include <deemon/__tg_integer.inl>

DEE_DECL_END

#if defined(GUARD_DEEMON_OBJECT_H) && \
    defined(GUARD_DEEMON_STRUCTURED_H) &&\
   !defined(GUARD_DEEMON_OPTIONAL_INTEGER_DECL_H)
#include <deemon/optional/integer_decl.h>
#endif

#endif /* !GUARD_DEEMON_OBJECT_H */
