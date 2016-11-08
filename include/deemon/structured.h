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
#ifndef GUARD_DEEMON_STRUCTURED_H
#define GUARD_DEEMON_STRUCTURED_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/structured_decl.h>
#include <deemon/optional/std/stdarg.h>
#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#include <deemon/optional/type_decl.h>
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION || \
    DEE_CONFIG_RUNTIME_HAVE_ARRAYS
#include <deemon/optional/atomic_mutex.h>
#endif /* ... */
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
#ifdef DEE_PRIVATE_DECL_DEE_STRUCTURED_OBJECT
DEE_PRIVATE_DECL_DEE_STRUCTURED_OBJECT
#undef DEE_PRIVATE_DECL_DEE_STRUCTURED_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_FUNCTION_FLAGS
DEE_PRIVATE_DECL_DEE_FUNCTION_FLAGS
#undef DEE_PRIVATE_DECL_DEE_FUNCTION_FLAGS
#endif

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
DEE_OBJECT_DEF(DeePointerTypeObject);
DEE_OBJECT_DEF(DeeLValueTypeObject);
DEE_OBJECT_DEF(DeePointerObject);
DEE_OBJECT_DEF(DeeLValueObject);
#endif

//////////////////////////////////////////////////////////////////////////
// -- Explanation --
//   - Structured objects
//     - Can be used in structures
//     - Don't have a destructor
//     - Implement 'tp_p_*' operators instead of the 'tp_*' equivalents
//     - Variables can be referenced to create pointer objects
//     - Default constructor returns objects in undetermined state (uninitialized)
//     - copy constructor/assign are implemented with memcpy
//     - inc/dec always return the original object
//     - incpost/decpost call inc/dec, but return a copy from before the call
//     - hash is implemented by hashing the object memory (if not overwritten)
//     - tp_seq_iter_next cannot be implemented, as iterators are never structured objects
//     - tp_members, tp_getsets and tp_methods can still be used, but
//       you can't call DeeObject_Check() on the self object in function calls
//       - The self object is just a pointer offset by sizeof(DeeObject)
//         from the real data. Modifying data in the object header causes
//         undefined behavior, meaning you can never call incref on 'self'
//         in a member function / property.
//     - tp_attr_get / tp_attr_del / tp_attr_set can't be overwritten
//     - Structured types cannot be inherited from each other.
//       >> tp_p_* operators will only be called on the top layer.
//     - Used as base types for user defined c-struct types
//
//   - Pointer objects - inherits from: Structured objects
//     - Always refer to a structured object: 'base' ('tp_ptr_pointer_base')
//     - Are always Structured objects
//     - Can be dereferenced to create an lvalue to 'base'
//
//   - LValue objects
//     - NOTE: These are not structured objects; they can't be used in structures
//     - Always refer to a structured object: 'base' ('tp_lval_lvalue_base')
//     - Can be dereferenced if 'base' is a pointer
//     - Can be referenced to return a pointer to 'base'
//

#ifdef DEE_LIMITED_API
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
struct DeeForeignFunctionTypeObject;
struct DeeForeignFunctionTypeList {
 struct DeeAtomicMutex                 fftl_lock;  /*< Lock (required for synchronization) */
 Dee_size_t                            fftl_typec; /*< Amount of known types. */
 struct DeeForeignFunctionTypeObject **fftl_typev; /*< [1..1][0..atl_typec][owned] List of types. */
};
#define DeeForeignFunctionTypeList_INIT() {DeeAtomicMutex_INIT(),0,NULL}
extern void DeeForeignFunctionTypeList_Init(DEE_A_OUT struct DeeForeignFunctionTypeList *self);
extern void DeeForeignFunctionTypeList_Quit(DEE_A_IN struct DeeForeignFunctionTypeList *self);
extern void DeeForeignFunctionTypeList_Clear(DEE_A_IN struct DeeForeignFunctionTypeList *self);
// NOTE: 'return_type' must be the structured/lvalue type with this cache list
extern DEE_A_RET_EXCEPT(NULL) struct DeeForeignFunctionTypeObject *
DeeForeignFunctionTypeList_FindOrAddToGlobalCache(
 DEE_A_INOUT struct DeeForeignFunctionTypeList *self,
 DEE_A_IN DeeFunctionFlags flags, DEE_A_IN DeeTypeObject const *return_type,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeTypeObject const *const *argv);
extern DEE_A_RET_EXCEPT(NULL) struct DeeForeignFunctionTypeObject *
DeeForeignFunctionTypeList_FindOrAddToGlobalCacheDefault(
 DEE_A_IN DeeFunctionFlags flags, DEE_A_IN DeeTypeObject const *return_type,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeTypeObject const *const *argv);
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#endif

#ifdef DEE_LIMITED_API
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
struct DeeArrayTypeObject;
struct DeeArrayTypeList {
 struct DeeAtomicMutex       atl_lock;  /*< Lock (required for synchronization) */
 Dee_size_t                  atl_typec; /*< Amount of known types. */
 struct DeeArrayTypeObject **atl_typev; /*< [1..1][0..atl_typec][owned] List of types. */
};
#define DeeArrayTypeList_INIT() {DeeAtomicMutex_INIT(),0,NULL}
extern void DeeArrayTypeList_Init(DEE_A_OUT struct DeeArrayTypeList *self);
extern void DeeArrayTypeList_Quit(DEE_A_IN struct DeeArrayTypeList *self);
extern void DeeArrayTypeList_Clear(DEE_A_IN struct DeeArrayTypeList *self);
// NOTE: 'return_type' must be the structured type with this cache list
extern DEE_A_RET_EXCEPT(NULL) struct DeeArrayTypeObject *
DeeArrayTypeList_FindOrAddToGlobalCache(
 DEE_A_INOUT struct DeeArrayTypeList *self,
 DEE_A_IN DeeStructuredTypeObject const *elem_type, DEE_A_IN Dee_size_t n_elem);
#endif
#endif

#ifdef DEE_LIMITED_API
struct _DeeStructuredTypeConstructorOperators {
 DeeType_SLOT_TYPE(tp_p_instance_size) tp_p_instance_size; /*< Basic instance size '== tp_instance_size-sizeof(DeeObject)'. */
 DeeType_SLOT_TYPE(tp_p_any_ctor)      tp_p_any_ctor;
};
struct _DeeStructuredTypeAssignOperators {
 DeeType_SLOT_TYPE(tp_p_any_assign) tp_p_any_assign;
};
struct _DeeStructuredTypeCastOperators {
 DeeType_SLOT_TYPE(tp_p_str)    tp_p_str;
 DeeType_SLOT_TYPE(tp_p_repr)   tp_p_repr;
 DeeType_SLOT_TYPE(tp_p_int32)  tp_p_int32;
 DeeType_SLOT_TYPE(tp_p_int64)  tp_p_int64;
 DeeType_SLOT_TYPE(tp_p_double) tp_p_double;
};
struct _DeeStructuredTypeObjectOperators {
 DeeType_SLOT_TYPE(tp_p_call) tp_p_call;
};
struct _DeeStructuredTypeMathOperators {
 DeeType_SLOT_TYPE(tp_p_not)     tp_p_not;
 DeeType_SLOT_TYPE(tp_p_bool)    tp_p_bool;
 DeeType_SLOT_TYPE(tp_p_inv)     tp_p_inv;
 DeeType_SLOT_TYPE(tp_p_pos)     tp_p_pos;
 DeeType_SLOT_TYPE(tp_p_neg)     tp_p_neg;
 DeeType_SLOT_TYPE(tp_p_inc)     tp_p_inc;
 DeeType_SLOT_TYPE(tp_p_incpost) tp_p_incpost;
 DeeType_SLOT_TYPE(tp_p_dec)     tp_p_dec;
 DeeType_SLOT_TYPE(tp_p_decpost) tp_p_decpost;
 DeeType_SLOT_TYPE(tp_p_add)     tp_p_add;
 DeeType_SLOT_TYPE(tp_p_iadd)    tp_p_iadd;
 DeeType_SLOT_TYPE(tp_p_sub)     tp_p_sub;
 DeeType_SLOT_TYPE(tp_p_isub)    tp_p_isub;
 DeeType_SLOT_TYPE(tp_p_mul)     tp_p_mul;
 DeeType_SLOT_TYPE(tp_p_imul)    tp_p_imul;
 DeeType_SLOT_TYPE(tp_p_div)     tp_p_div;
 DeeType_SLOT_TYPE(tp_p_idiv)    tp_p_idiv;
 DeeType_SLOT_TYPE(tp_p_mod)     tp_p_mod;
 DeeType_SLOT_TYPE(tp_p_imod)    tp_p_imod;
 DeeType_SLOT_TYPE(tp_p_shl)     tp_p_shl;
 DeeType_SLOT_TYPE(tp_p_ishl)    tp_p_ishl;
 DeeType_SLOT_TYPE(tp_p_shr)     tp_p_shr;
 DeeType_SLOT_TYPE(tp_p_ishr)    tp_p_ishr;
 DeeType_SLOT_TYPE(tp_p_and)     tp_p_and;
 DeeType_SLOT_TYPE(tp_p_iand)    tp_p_iand;
 DeeType_SLOT_TYPE(tp_p_or)      tp_p_or;
 DeeType_SLOT_TYPE(tp_p_ior)     tp_p_ior;
 DeeType_SLOT_TYPE(tp_p_xor)     tp_p_xor;
 DeeType_SLOT_TYPE(tp_p_ixor)    tp_p_ixor;
 DeeType_SLOT_TYPE(tp_p_pow)     tp_p_pow;
 DeeType_SLOT_TYPE(tp_p_ipow)    tp_p_ipow;
 DeeType_SLOT_TYPE(tp_p_hash)    tp_p_hash;
};
struct _DeeStructuredTypeCompareOperators {
 DeeType_SLOT_TYPE(tp_p_cmp_lo) tp_p_cmp_lo;
 DeeType_SLOT_TYPE(tp_p_cmp_le) tp_p_cmp_le;
 DeeType_SLOT_TYPE(tp_p_cmp_eq) tp_p_cmp_eq;
 DeeType_SLOT_TYPE(tp_p_cmp_ne) tp_p_cmp_ne;
 DeeType_SLOT_TYPE(tp_p_cmp_gr) tp_p_cmp_gr;
 DeeType_SLOT_TYPE(tp_p_cmp_ge) tp_p_cmp_ge;
};
struct _DeeStructuredTypeSeqOperators {
 DeeType_SLOT_TYPE(tp_p_seq_get)       tp_p_seq_get;
 DeeType_SLOT_TYPE(tp_p_seq_del)       tp_p_seq_del;
 DeeType_SLOT_TYPE(tp_p_seq_set)       tp_p_seq_set;
 DeeType_SLOT_TYPE(tp_p_seq_size)      tp_p_seq_size;
 DeeType_SLOT_TYPE(tp_p_seq_contains)  tp_p_seq_contains;
 DeeType_SLOT_TYPE(tp_p_seq_range_get) tp_p_seq_range_get;
 DeeType_SLOT_TYPE(tp_p_seq_range_del) tp_p_seq_range_del;
 DeeType_SLOT_TYPE(tp_p_seq_range_set) tp_p_seq_range_set;
 DeeType_SLOT_TYPE(tp_p_seq_iter_self) tp_p_seq_iter_self;
};
struct _DeeStructuredTypeAttributeOperators {
 DeeType_SLOT_TYPE(tp_p_attr_get) tp_p_attr_get;
 DeeType_SLOT_TYPE(tp_p_attr_del) tp_p_attr_del;
 DeeType_SLOT_TYPE(tp_p_attr_set) tp_p_attr_set;
};
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeStructuredTypeObject {
 DeeTypeObject                    stp_type;     /*< Underlying type structure (default for all types). */
 union{ // [0..1] Used to keep the global linked list of structured types
  struct DeeStructuredTypeObject *stp_prev_structured_object_type;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  struct DeePointerTypeObject    *stp_prev_pointer_type;
  struct DeeLValueTypeObject     *stp_prev_lvalule_type;
#endif
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
  struct DeeVarArrayTypeObject   *stp_prev_varray_type;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define stp_prev_structured_object_type _stp_prev_data.stp_prev_structured_object_type
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define stp_prev_pointer_type _stp_prev_data.stp_prev_pointer_type
#define stp_prev_lvalule_type _stp_prev_data.stp_prev_lvalule_type
#endif
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
#define stp_prev_varray_type  _stp_prev_data.stp_prev_varray_type
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 _stp_prev_data
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
 ;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 /*atomic_read|locked_write*/DeePointerTypeObject         *stp_pointer;    /*< [0..1] 'type(this) *'. */
 /*atomic_read|locked_write*/DeeLValueTypeObject          *stp_lvalue;     /*< [0..1] 'type(this) &'. */
#endif
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
 struct DeeForeignFunctionTypeList                         stp_ffunctions; /*< List of foreign function types returning this type. */
#endif
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 struct DeeArrayTypeList                                   stp_arrays;     /*< List of array types using this type as element. */
 /*atomic_read|locked_write*/struct DeeVarArrayTypeObject *stp_varray;     /*< [0..1] Variable size array type. */
#endif
 // New operator that don't require 'self' to necessarily be a 'DeeObject'
 // 'self' instead points directly after the object header.
 // Or in other 'self == DeeStructured_DATA(ob)'
 // Note that inplace operators can't be set to return
 // a custom value and will always return the original lhs object,
 // may that be an actual instance or an lvalue or something different.
 // NOTE: All of these operators can be set to NULL if they are not implemented
 struct _DeeStructuredTypeConstructorOperators tp_p_constructor;
 struct _DeeStructuredTypeAssignOperators      tp_p_assign;
 struct _DeeStructuredTypeCastOperators        tp_p_cast;
 struct _DeeStructuredTypeObjectOperators      tp_p_object;
 struct _DeeStructuredTypeMathOperators        tp_p_math;
 struct _DeeStructuredTypeCompareOperators     tp_p_compare;
 struct _DeeStructuredTypeSeqOperators         tp_p_seq;
 struct _DeeStructuredTypeAttributeOperators   tp_p_attr;
};
DEE_COMPILER_MSVC_WARNING_POP

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DEE_PRIVATE_STRUCTURED_TYPE_OBJECT_HEADER_POINTER_v100 NULL,NULL,
#else
#define DEE_PRIVATE_STRUCTURED_TYPE_OBJECT_HEADER_POINTER_v100 
#endif
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
#define DEE_PRIVATE_STRUCTURED_TYPE_OBJECT_HEADER_FOREIGNFUNCTION_v100 DeeForeignFunctionTypeList_INIT(),
#else
#define DEE_PRIVATE_STRUCTURED_TYPE_OBJECT_HEADER_FOREIGNFUNCTION_v100 
#endif
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
#define DEE_PRIVATE_STRUCTURED_TYPE_OBJECT_HEADER_ARRAY_v100 DeeArrayTypeList_INIT(),NULL,
#else
#define DEE_PRIVATE_STRUCTURED_TYPE_OBJECT_HEADER_ARRAY_v100
#endif

#define DEE_STRUCTURED_TYPE_OBJECT_HEADER_v100 \
 {NULL},DEE_PRIVATE_STRUCTURED_TYPE_OBJECT_HEADER_POINTER_v100 \
 DEE_PRIVATE_STRUCTURED_TYPE_OBJECT_HEADER_FOREIGNFUNCTION_v100 \
 DEE_PRIVATE_STRUCTURED_TYPE_OBJECT_HEADER_ARRAY_v100
#define DEE_STRUCTURED_TYPE_OBJECT_CONSTRUCTOR_v100(_tp_p_instance_size,_tp_p_any_ctor) \
{DEE_TYPEMEMBER(_tp_p_instance_size,DeeType_DEFAULT_SLOT(tp_p_instance_size))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_any_ctor),DEE_TYPEMEMBER(_tp_p_any_ctor,DeeType_DEFAULT_SLOT(tp_p_any_ctor)))\
}
#define DEE_STRUCTURED_TYPE_OBJECT_ASSIGN_v100(_tp_p_any_assign) \
{DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_any_assign),DEE_TYPEMEMBER(_tp_p_any_assign,DeeType_DEFAULT_SLOT(tp_p_any_assign)))\
}
#define DEE_STRUCTURED_TYPE_OBJECT_CAST_v100(\
 _tp_p_str,_tp_p_repr,_tp_p_int32,_tp_p_int64,_tp_p_double) \
{DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_str),DEE_TYPEMEMBER(_tp_p_str,DeeType_DEFAULT_SLOT(tp_p_str)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_repr),DEE_TYPEMEMBER(_tp_p_repr,DeeType_DEFAULT_SLOT(tp_p_repr)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_int32),DEE_TYPEMEMBER(_tp_p_int32,DeeType_DEFAULT_SLOT(tp_p_int32)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_int64),DEE_TYPEMEMBER(_tp_p_int64,DeeType_DEFAULT_SLOT(tp_p_int64)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_double),DEE_TYPEMEMBER(_tp_p_double,DeeType_DEFAULT_SLOT(tp_p_double)))\
}
#define DEE_STRUCTURED_TYPE_OBJECT_OBJECT_v100(_tp_p_call) \
{DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_call),DEE_TYPEMEMBER(_tp_p_call,DeeType_DEFAULT_SLOT(tp_p_call)))\
}
#define DEE_STRUCTURED_TYPE_OBJECT_MATH_v100(\
 _tp_p_not,_tp_p_bool,_tp_p_inv,_tp_p_pos,_tp_p_neg,_tp_p_inc,_tp_p_incpost,\
 _tp_p_dec,_tp_p_decpost,_tp_p_add,_tp_p_iadd,_tp_p_sub,_tp_p_isub,\
 _tp_p_mul,_tp_p_imul,_tp_p_div,_tp_p_idiv,_tp_p_mod,_tp_p_imod,_tp_p_shl,\
 _tp_p_ishl,_tp_p_shr,_tp_p_ishr,_tp_p_and,_tp_p_iand,_tp_p_or,_tp_p_ior,\
 _tp_p_xor,_tp_p_ixor,_tp_p_pow,_tp_p_ipow,_tp_p_hash)\
{DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_not),DEE_TYPEMEMBER(_tp_p_not,DeeType_DEFAULT_SLOT(tp_p_not)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_bool),DEE_TYPEMEMBER(_tp_p_bool,DeeType_DEFAULT_SLOT(tp_p_bool)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_inv),DEE_TYPEMEMBER(_tp_p_inv,DeeType_DEFAULT_SLOT(tp_p_inv)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_pos),DEE_TYPEMEMBER(_tp_p_pos,DeeType_DEFAULT_SLOT(tp_p_pos)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_neg),DEE_TYPEMEMBER(_tp_p_neg,DeeType_DEFAULT_SLOT(tp_p_neg)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_inc),DEE_TYPEMEMBER(_tp_p_inc,DeeType_DEFAULT_SLOT(tp_p_inc)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_incpost),DEE_TYPEMEMBER(_tp_p_incpost,DeeType_DEFAULT_SLOT(tp_p_incpost)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_dec),DEE_TYPEMEMBER(_tp_p_dec,DeeType_DEFAULT_SLOT(tp_p_dec)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_decpost),DEE_TYPEMEMBER(_tp_p_decpost,DeeType_DEFAULT_SLOT(tp_p_decpost)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_add),DEE_TYPEMEMBER(_tp_p_add,DeeType_DEFAULT_SLOT(tp_p_add)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_iadd),DEE_TYPEMEMBER(_tp_p_iadd,DeeType_DEFAULT_SLOT(tp_p_iadd)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_sub),DEE_TYPEMEMBER(_tp_p_sub,DeeType_DEFAULT_SLOT(tp_p_sub)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_isub),DEE_TYPEMEMBER(_tp_p_isub,DeeType_DEFAULT_SLOT(tp_p_isub)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_mul),DEE_TYPEMEMBER(_tp_p_mul,DeeType_DEFAULT_SLOT(tp_p_mul)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_imul),DEE_TYPEMEMBER(_tp_p_imul,DeeType_DEFAULT_SLOT(tp_p_imul)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_div),DEE_TYPEMEMBER(_tp_p_div,DeeType_DEFAULT_SLOT(tp_p_div)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_idiv),DEE_TYPEMEMBER(_tp_p_idiv,DeeType_DEFAULT_SLOT(tp_p_idiv)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_mod),DEE_TYPEMEMBER(_tp_p_mod,DeeType_DEFAULT_SLOT(tp_p_mod)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_imod),DEE_TYPEMEMBER(_tp_p_imod,DeeType_DEFAULT_SLOT(tp_p_imod)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_shl),DEE_TYPEMEMBER(_tp_p_shl,DeeType_DEFAULT_SLOT(tp_p_shl)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_ishl),DEE_TYPEMEMBER(_tp_p_ishl,DeeType_DEFAULT_SLOT(tp_p_ishl)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_shr),DEE_TYPEMEMBER(_tp_p_shr,DeeType_DEFAULT_SLOT(tp_p_shr)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_ishr),DEE_TYPEMEMBER(_tp_p_ishr,DeeType_DEFAULT_SLOT(tp_p_ishr)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_and),DEE_TYPEMEMBER(_tp_p_and,DeeType_DEFAULT_SLOT(tp_p_and)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_iand),DEE_TYPEMEMBER(_tp_p_iand,DeeType_DEFAULT_SLOT(tp_p_iand)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_or),DEE_TYPEMEMBER(_tp_p_or,DeeType_DEFAULT_SLOT(tp_p_or)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_ior),DEE_TYPEMEMBER(_tp_p_ior,DeeType_DEFAULT_SLOT(tp_p_ior)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_xor),DEE_TYPEMEMBER(_tp_p_xor,DeeType_DEFAULT_SLOT(tp_p_xor)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_ixor),DEE_TYPEMEMBER(_tp_p_ixor,DeeType_DEFAULT_SLOT(tp_p_ixor)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_pow),DEE_TYPEMEMBER(_tp_p_pow,DeeType_DEFAULT_SLOT(tp_p_pow)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_ipow),DEE_TYPEMEMBER(_tp_p_ipow,DeeType_DEFAULT_SLOT(tp_p_ipow)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_hash),DEE_TYPEMEMBER(_tp_p_hash,DeeType_DEFAULT_SLOT(tp_p_hash)))\
}
#define DEE_STRUCTURED_TYPE_OBJECT_COMPARE_v100(\
 _tp_p_cmp_lo,_tp_p_cmp_le,_tp_p_cmp_eq,\
 _tp_p_cmp_ne,_tp_p_cmp_gr,_tp_p_cmp_ge) \
{DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_cmp_lo),DEE_TYPEMEMBER(_tp_p_cmp_lo,DeeType_DEFAULT_SLOT(tp_p_cmp_lo)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_cmp_le),DEE_TYPEMEMBER(_tp_p_cmp_le,DeeType_DEFAULT_SLOT(tp_p_cmp_le)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_cmp_eq),DEE_TYPEMEMBER(_tp_p_cmp_eq,DeeType_DEFAULT_SLOT(tp_p_cmp_eq)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_cmp_ne),DEE_TYPEMEMBER(_tp_p_cmp_ne,DeeType_DEFAULT_SLOT(tp_p_cmp_ne)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_cmp_gr),DEE_TYPEMEMBER(_tp_p_cmp_gr,DeeType_DEFAULT_SLOT(tp_p_cmp_gr)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_cmp_ge),DEE_TYPEMEMBER(_tp_p_cmp_ge,DeeType_DEFAULT_SLOT(tp_p_cmp_ge)))\
}
#define DEE_STRUCTURED_TYPE_OBJECT_SEQ_v100(\
 _tp_p_seq_get,_tp_p_seq_del,_tp_p_seq_set,_tp_p_seq_size,_tp_p_seq_contains,\
 _tp_p_seq_range_get,_tp_p_seq_range_del,_tp_p_seq_range_set,_tp_p_seq_iter_self)\
{DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_seq_get),DEE_TYPEMEMBER(_tp_p_seq_get,DeeType_DEFAULT_SLOT(tp_p_seq_get)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_seq_del),DEE_TYPEMEMBER(_tp_p_seq_del,DeeType_DEFAULT_SLOT(tp_p_seq_del)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_seq_set),DEE_TYPEMEMBER(_tp_p_seq_set,DeeType_DEFAULT_SLOT(tp_p_seq_set)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_seq_size),DEE_TYPEMEMBER(_tp_p_seq_size,DeeType_DEFAULT_SLOT(tp_p_seq_size)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_seq_contains),DEE_TYPEMEMBER(_tp_p_seq_contains,DeeType_DEFAULT_SLOT(tp_p_seq_contains)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_seq_range_get),DEE_TYPEMEMBER(_tp_p_seq_range_get,DeeType_DEFAULT_SLOT(tp_p_seq_range_get)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_seq_range_del),DEE_TYPEMEMBER(_tp_p_seq_range_del,DeeType_DEFAULT_SLOT(tp_p_seq_range_del)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_seq_range_set),DEE_TYPEMEMBER(_tp_p_seq_range_set,DeeType_DEFAULT_SLOT(tp_p_seq_range_set)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_seq_iter_self),DEE_TYPEMEMBER(_tp_p_seq_iter_self,DeeType_DEFAULT_SLOT(tp_p_seq_iter_self)))\
}
#define DEE_STRUCTURED_TYPE_OBJECT_ATTRIBUTE_v100(\
 _tp_p_attr_get,_tp_p_attr_del,_tp_p_attr_set)\
{DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_attr_get),DEE_TYPEMEMBER(_tp_p_attr_get,DeeType_DEFAULT_SLOT(tp_p_attr_get)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_attr_del),DEE_TYPEMEMBER(_tp_p_attr_del,DeeType_DEFAULT_SLOT(tp_p_attr_del)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_p_attr_set),DEE_TYPEMEMBER(_tp_p_attr_set,DeeType_DEFAULT_SLOT(tp_p_attr_set)))\
}
#define DEE_STRUCTURED_TYPE_OBJECT_FOOTER_v100 /* nothing (yet...) */


#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
struct DeePointerTypeObject {
           DeeStructuredTypeObject  ptp_type;            /*< Underlying type structure (default for all types). */
           Dee_size_t               tp_ptr_elem_size;    /*< instance size helper '== tp_pointer_rem->tp_basesize'. */
 DEE_A_REF DeeStructuredTypeObject *tp_ptr_pointer_base; /*< [1..1] non-lvalue pointer base type: 'type(*this).tp_lval_lvalue_base'. */
};
#define DeePointerType_ELEMSIZ(ob) ((DeePointerTypeObject *)Dee_REQUIRES_POINTER(ob))->tp_ptr_elem_size
#define DeePointerType_BASE(ob)    ((DeeTypeObject *)((DeePointerTypeObject *)Dee_REQUIRES_POINTER(ob))->tp_ptr_pointer_base)

DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeLValueTypeObject {
 DeeTypeObject                      ltp_type; /*< Underlying type structure (default for all types). */
 // Lvalue_type doesn't inherit from structured_object_type,
 // meaning we have to add the linked list again.
 union{ // [0..1] Used to keep the global linked list of structured types
  struct DeeStructuredTypeObject   *ltp_prev_structured_object_type;
  struct DeePointerTypeObject      *ltp_prev_pointer_type;
  struct DeeLValueTypeObject       *ltp_prev_lvalule_type;
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
  struct DeeVarArrayTypeObject     *ltp_prev_varray_type;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define ltp_prev_structured_object_type _ltp_prev_data.ltp_prev_structured_object_type
#define ltp_prev_pointer_type           _ltp_prev_data.ltp_prev_pointer_type
#define ltp_prev_lvalule_type           _ltp_prev_data.ltp_prev_lvalule_type
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
#define ltp_prev_varray_type            _ltp_prev_data.ltp_prev_varray_type
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 _ltp_prev_data
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
 ;
 DEE_A_REF DeeStructuredTypeObject *tp_lval_lvalue_base;  /*< [1..1] Lvalue-less type (without the '&'). This type is used to implement operators. */
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
 struct DeeForeignFunctionTypeList  ltp_ffunctions; /*< List of foreign function types returning this type. */
#endif
};
DEE_COMPILER_MSVC_WARNING_POP
#define DeeLValueType_BASE(ob) ((DeeTypeObject *)((DeeLValueTypeObject *)(ob))->tp_lval_lvalue_base)
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#else /* DEE_LIMITED_API */
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DeePointerType_ELEMSIZ(ob) DeeType_GET_SLOT(ob,tp_ptr_elem_size)
#define DeePointerType_BASE(ob)    DeeType_GET_SLOT(ob,tp_ptr_pointer_base)
#define DeeLValueType_BASE(ob)     DeeType_GET_SLOT(ob,tp_lval_lvalue_base)
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#endif /* !DEE_LIMITED_API */

#if DEE_DEPRECATED_API_VERSION(100,101,103)
// >> DEE_A_RET_OBJECT_EXCEPT_REF(DeeStructuredObject) *DeeStructured_Copy(
//     DeeStructured_Copy(DEE_A_IN_OBJECT(DeeStructuredObject) const *self);
#define DeeStructured_Copy(ob) DeeObject_Copy((DeeObject *)(ob))
#endif

//////////////////////////////////////////////////////////////////////////
// Creates a new structured object from a type and data
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeStructuredObject) *) DeeStructured_NewFromData(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *ty,
 DEE_A_IN void const *data) DEE_ATTRIBUTE_NONNULL((1,2));
#define DeeStructured_GetData  DeeStructured_NewFromData

//////////////////////////////////////////////////////////////////////////
// Assign data from 'right' to structured data contained in 'self' and described by 'tp_self'
#define DeeStructured_SetData(tp_self,self,right) \
 DeeStructured_TSetData(tp_self,self,Dee_TYPE(right),right)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeStructured_TSetData(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *self, DEE_A_IN DeeTypeObject const *tp_right,
 DEE_A_INOUT DeeObject *right) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeStructured_DelData(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *self) DEE_ATTRIBUTE_NONNULL((1,2));


#if DEE_CONFIG_RUNTIME_HAVE_NEWDELETE
//////////////////////////////////////////////////////////////////////////
// C-api versions of 'new' and 'delete' in user code
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeePointerObject) *) DeePointer_NewInstance(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *ty);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeePointer_DeleteInstance(DEE_A_INOUT DeeObject *self);
#endif


DEE_DATA_DECL(DeeTypeObject) DeeStructuredType_Type; // for: DeeStructuredTypeObject; __ob_type: &DeeType_Type; tp_base: &DeeType_Type;           @deemon: 'type(structured_object)'
#define DeeStructuredType_Check(ob)      DeeObject_InstanceOf(ob,&DeeStructuredType_Type)
#define DeeStructuredType_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeStructuredType_Type)

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
DEE_DATA_DECL(DeeTypeObject) DeePointerType_Type;    // for: DeePointerTypeObject;    __ob_type: &DeeType_Type; tp_base: &DeeStructuredType_Type; @deemon: 'type(pointer)'
DEE_DATA_DECL(DeeTypeObject) DeeLValueType_Type;     // for: DeeLValueTypeObject;     __ob_type: &DeeType_Type; tp_base: &DeeType_Type;           @deemon: 'type(lvalue)'
#define DeePointerType_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeePointerType_Type)
#define DeeLValueType_CheckExact(ob)  DeeObject_InstanceOfExact(ob,&DeeLValueType_Type)
#define DeePointerType_Check          DeePointerType_CheckExact
#define DeeLValueType_Check           DeeLValueType_CheckExact
#endif

//////////////////////////////////////////////////////////////////////////
// v no-op base types for structured objects, pointers and lvalues
//   >> These exist to give the three groups names and allow for 'p is pointer' - style checks
//   >> These groups are each sub-classed by all the different kinds of structured object, pointer and lvalue types.
//   NOTE: We also need to check if an object is a pointer or lvalue to implement 'DeeObject_Ref' / 'DeeObject_Deref'
//   NOTE: These also implement common operators, such as pointer arithmetic for 'DeePointer_Type'
//   NOTE: 'DeePointer_Type' is used as an untyped pointer, that can implicitly be
//         casted to any other pointer, similar to how none is always a NULL pointer
DEE_DATA_DECL(DeeStructuredTypeObject) DeeStructured_Type; // __ob_type: &DeeStructuredType_Type; tp_base: &DeeObject_Type;     @deemon: 'structured_object'
#define DeeStructured_Check(ob)      DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeStructured_Type)

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
DEE_DATA_DECL(DeePointerTypeObject)    DeePointer_Type;    // __ob_type: &DeePointerType_Type;    tp_base: &DeeStructured_Type; @deemon: 'pointer'
DEE_DATA_DECL(DeeLValueTypeObject)     DeeLValue_Type;     // __ob_type: &DeeLValueType_Type;     tp_base: &DeeObject_Type;     @deemon: 'lvalue'
#define DeePointer_Check(ob)         DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeePointer_Type)
#define DeeLValue_Check(ob)          DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeLValue_Type)
#endif

// v Don't even think about using these!
//   - They only check if an object is an instance of the no-op
//     base classes for structured_object, pointer and lvalue.
//   - What you want to do is probably check if an object is a pointer (DeePointer_Check) or lvalue (DeeLValue_Check).
#define DeeStructured_CheckExact(ob) DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeStructured_Type)
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DeePointer_CheckExact(ob)    DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeePointer_Type)
#define DeeLValue_CheckExact(ob)     DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeLValue_Type)


DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeePointerObject) *) DeePointer_New(
 DEE_A_IN_TYPEOBJECT(DeePointerTypeObject) const *tp, DEE_A_IN_OPT void *p);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeLValueObject) *) DeeLValue_New(
 DEE_A_IN_TYPEOBJECT(DeeLValueTypeObject) const *tp, DEE_A_IN void *p);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeePointerObject) *) DeeBasePointer_New(
 DEE_A_IN DeeTypeObject const *ptr_base, DEE_A_IN_OPT void *p);
#define DeeVoidPointer_New(p) DeeBasePointer_New((DeeTypeObject *)&DeeNone_Type,p)
#define DeeCharPointer_New(p) DeeBasePointer_New(DeeObject_TYPE(char),p)


//////////////////////////////////////////////////////////////////////////
// Same as 'DeeLValue_New', but throws a ValueError if 'p' is NULL
// NOTE: Now just a perfect alias for 'DeeLValue_New', but kept for self-documenting code
// -> Had to be removed for correctly working offsetof-style runtime code.
// Also: Since lvalues are (supposed to be) able to correctly handle invalid
//       pointers, null pointers should be possible to handle as well.
#define DeeLValue_NewWithError DeeLValue_New


//////////////////////////////////////////////////////////////////////////
// Returns a copy of the object 'self' is pointing towards
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeStructuredObject) *)
 DeeLValue_Value(DEE_A_IN_OBJECT(DeeLValueObject) const *self);



#define DeePointerType_Pointer DeeStructuredType_Pointer /*< Because pointers are structured types. */
DEE_FUNC_DECL(DEE_A_RET_TYPEOBJECT_EXCEPT(DeePointerTypeObject) *) DeeStructuredType_Pointer(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_TYPEOBJECT_EXCEPT(DeePointerTypeObject) *) DeeLValueType_Pointer(
 DEE_A_IN_TYPEOBJECT(DeeLValueTypeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

DEE_FUNC_DECL(DEE_A_RET_TYPEOBJECT_EXCEPT(DeeLValueTypeObject) *) DeeStructuredType_LValue(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */


#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
DEE_FUNC_DECL(DEE_A_RET_TYPEOBJECT_EXCEPT(DeeForeignFunctionTypeObject) *) DeeStructuredType_ForeignFunction(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *self, DEE_A_IN DeeFunctionFlags flags,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeTypeObject const *const *argv) DEE_ATTRIBUTE_NONNULL((1,4));
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DeePointerType_ForeignFunction DeeStructuredType_ForeignFunction /*< Because pointers are structured types. */
DEE_FUNC_DECL(DEE_A_RET_TYPEOBJECT_EXCEPT(DeeForeignFunctionTypeObject) *) DeeLValueType_ForeignFunction(
 DEE_A_IN_TYPEOBJECT(DeeLValueTypeObject) const *self, DEE_A_IN DeeFunctionFlags flags,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeTypeObject const *const *argv) DEE_ATTRIBUTE_NONNULL((1,4));
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
DEE_FUNC_DECL(DEE_A_RET_TYPEOBJECT_EXCEPT(DeeForeignFunctionTypeObject) *) _DeeType_ForeignFunctionDefault(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN DeeFunctionFlags flags,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeTypeObject const *const *argv) DEE_ATTRIBUTE_NONNULL((1,4));
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */


//////////////////////////////////////////////////////////////////////////
// Returns a structured type described by a given format string.
// - The given format string is strictly read left-to-right and is designed like this:
// >> tyname ::= ['a'] ['l','ll','h','hh','I','I8','I16','I32','I64','L'] (
// >>     ('%o')         // Take the type from varargs
// >>   | ('d'|'i')      // [Atomic] signed integer type (prefix used like in printf)
// >>   | ('u'|'x'|'X')  // [Atomic] unsigned integer type (prefix used like in printf)
// >>   | ('f'|'D','D')  // Floating point types (use 'LD' for 'long double')
// >>   | ('o')          // DeeObject_Type
// >>   | ('n','v')      // DeeNone_Type
// >>   | ('p')          // void-pointer (alias for 'v*' or 'n*')
// >>   | ('s')          // char|wchar_t-pointer (prefix with 'l' for wide)
// >>   | ('c')          // char|wchar_t (prefix with 'l' for wide)
// >> );
// >> intname ::= (
// >>     ('%' ['l','ll','h','hh','I8','I16','I32','I64'] 'd'|'u'|'x'|'X') // Take an integer
// >>   | (('0'...'9')...)
// >> );
// >> funflags ::= ('') // TODO
// >> @structf ::= tyname [
// >>      '*'                                        // Create a pointer type.
// >>   | ('[' intname ']')                           // Create an array type.
// >>   | ('(' ['!' funflags] [structf [',']]... ')') // Create a foreign-function type.
// >> ]...;
// - Example:
// >> DeeType_Structf("d[10]*");    // returns the type from: 'typedef int (*T)[10];'
// >> DeeType_Structf("d[%d]*",10); // returns the type from: 'typedef int (*T)[10];'
// in "structured.typef.inl"
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(NULL) DeeTypeObject *) DeeType_Structf(
 DEE_A_IN_Z char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1));
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(NULL) DeeTypeObject *) DeeType_VStructf(
 DEE_A_IN_Z char const *fmt, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */

//////////////////////////////////////////////////////////////////////////
// Returns a new structured object from the data in 'self'
// Uses the same formating as 'DeeType_Structf'
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeStructuredObject) *) DeeStructured_NewFromDataf(
 DEE_A_IN void const *self, DEE_A_IN_Z char const *ty_fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2));
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeStructuredObject) *) DeeStructured_VNewFromDataf(
 DEE_A_IN void const *self, DEE_A_IN_Z char const *ty_fmt, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */


#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
//////////////////////////////////////////////////////////////////////////
// Returns a new pointer from a formatted type name
// Uses the same formating as 'DeeType_Structf', but adds
// another pointer dereference ontop of the parsed type
// - Example:
// >> // Return a pointer: 'int (*p)(int,float);'
// >> DeePointer_Newf(p,"i(if)");
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeePointerObject) *) DeePointer_Newf(
 void *p, DEE_A_IN_Z char const *ty_fmt, ...) DEE_ATTRIBUTE_NONNULL((2));
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeePointerObject) *) DeePointer_VNewf(
 void *p, DEE_A_IN_Z char const *ty_fmt, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((2));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */

//////////////////////////////////////////////////////////////////////////
// Returns a new lvalue from a formatted type name
// Uses the same formating as 'DeeType_Structf', but adds
// another lvalue dereference ontop of the parsed type
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeLValueObject) *) DeeLValue_Newf(
 DEE_A_IN void *p, DEE_A_IN_Z char const *ty_fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2));
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeLValueObject) *) DeeLValue_VNewf(
 DEE_A_IN void *p, DEE_A_IN_Z char const *ty_fmt, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */

//////////////////////////////////////////////////////////////////////////
// Error-capable version of getting a pointer type
//  - Checks if 'self' is a structured or lvalue type, calling
//    'DeeStructuredType_Pointer' and 'DeeLValueType_Pointer' respectively,
//    but raises an 'Error.TypeError' and return NULL if it's neither of those
// >> NOTE: All of these function throw a TypeError if an invalid type is given
#define DeeType_Pointer(ob)        DeeType_TPointer(Dee_TYPE(ob),ob)
#define DeeType_LValue(ob)         DeeType_TLValue(Dee_TYPE(ob),ob)
#define DeeType_WithoutPointer(ob) DeeType_TWithoutPointer(Dee_TYPE(ob),ob)
#define DeeType_WithoutLValue(ob)  DeeType_TWithoutLValue(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_RET_TYPEOBJECT_EXCEPT(DeePointerTypeObject) *) DeeType_TPointer(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeTypeObject const *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_TYPEOBJECT_EXCEPT(DeeLValueTypeObject) *) DeeType_TLValue(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeTypeObject const *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_TYPEOBJECT_EXCEPT(DeeStructuredTypeObject) *) DeeType_TWithoutPointer(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeTypeObject const *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_TYPEOBJECT_EXCEPT(DeeStructuredTypeObject) *) DeeType_TWithoutLValue(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeTypeObject const *self) DEE_ATTRIBUTE_NONNULL((1,2));
#endif

#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
//////////////////////////////////////////////////////////////////////////
// Returns a foreign_function type with the given properties, returning 'self'
#define DeeType_ForeignFunction(ob,flags,argc,argv) DeeType_TForeignFunction(Dee_TYPE(ob),ob,flags,argc,argv)
DEE_FUNC_DECL(DEE_A_RET_TYPEOBJECT_EXCEPT(DeeForeignFunctionTypeObject) *) DeeType_TForeignFunction(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeTypeObject const *self, DEE_A_IN DeeFunctionFlags flags,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeTypeObject const *const *argv) DEE_ATTRIBUTE_NONNULL((1,2));
#endif

#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
#define DeeType_Array(ob,array_size) DeeType_TArray(Dee_TYPE(ob),ob,array_size)
#define DeeType_VArray(ob) DeeType_TVArray(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_RET_TYPEOBJECT_EXCEPT(DeeArrayTypeObject) *) DeeType_TArray(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeTypeObject const *self,
 DEE_A_IN Dee_size_t array_size) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_TYPEOBJECT_EXCEPT(DeeVarArrayTypeObject) *) DeeType_TVArray(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeTypeObject const *self) DEE_ATTRIBUTE_NONNULL((1,2));

#ifdef DEE_LIMITED_API
extern DEE_A_RET_TYPEOBJECT_EXCEPT(DeeVarArrayTypeObject) *DeeStructuredType_VArray(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
extern DEE_A_RET_TYPEOBJECT_EXCEPT(DeeVarArrayTypeObject) *DeeLValueType_VArray(
 DEE_A_IN_TYPEOBJECT(DeeLValueTypeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#endif /* DEE_LIMITED_API */
#endif /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
//////////////////////////////////////////////////////////////////////////
// Returns a pointer to the referenced location of 'ob'
//  - 'ob' must be a structured type ('DeeStructured_Check') or an lvalue object ('DeeLValue_Check')
//  - Raises an 'Error.TypeError' and return NULL if it's neither of those
#define DeeObject_Ref(ob) DeeObject_TRef(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeePointerObject) *) DeeObject_TRef(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Returns an lvalue object pointing to the dereferenced location of 'ob'
//  - 'ob' must be a pointer ('DeePointer_Check') or lvalue-to-pointer object
//  - Raises an 'Error.TypeError' and return NULL if it's neither of those
#define DeeObject_Deref(ob) DeeObject_TDeref(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeLValueObject) *) DeeObject_TDeref(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Returns an lvalue object pointing to the location of 'ob'
//  - 'ob' must be a structured object ('DeeStructured_Check')
//  - Raises an 'Error.TypeError' and return NULL if it's neither of those
#define DeeObject_LValue(ob) DeeObject_TLValue(Dee_TYPE(ob),ob)
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeLValueObject) *) DeeObject_TLValue(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
#endif

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
//////////////////////////////////////////////////////////////////////////
// Clear the pool of generated structure types
DEE_FUNC_DECL(Dee_size_t) DeePointer_ClearPool(void);
#endif

#ifdef DEE_LIMITED_API
extern void DeeStructuredType_Shutdown(void);
#endif

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
//////////////////////////////////////////////////////////////////////////
// Returns the pointer of 'ob' if it's deref type is compatible with 'tp_result'
// Note, that 'tp_result' is the dereferenced type, meaning you put
// 'DeeInt_Type' if the function should return an 'int *'
// >> Returns -1 and throws an error on failure
// NOTE: Returns NULL for any type if DeeNone_Check(ob) is true
//       or if 'ob' is convertible to '(Dee_size_t)0' (DeeObject_Cast_F(Dee_size_t))
// >> set 'tp_result' to '&DeeNone_Type' to accept any kind of pointer
// >> 'tp_result' must be a structured type
#define DeeObject_GetPointerEx(ob,tp_result,result)\
 DeeObject_TGetPointerEx(Dee_TYPE(ob),ob,tp_result,result)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeObject_TGetPointerEx(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_result,
 DEE_A_OUT void **result) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
#endif

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
//////////////////////////////////////////////////////////////////////////
// Using structured typenames, parse 'ty_fmt' for the pointer base type.
// NOTE: The formatted type is the BASE of the actual pointer type that will be searched for.
// s.a.: 'DeeType_Structf'
#define DeeObject_GetPointerExf(ob,result,...)\
 DeeObject_TGetPointerExf(Dee_TYPE(ob),ob,result,__VA_ARGS__)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeObject_TGetPointerExf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_OUT void **result, DEE_A_IN_Z char const *ty_fmt, ...) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
#define DeeObject_VGetPointerExf(ob,result,ty_fmt,args) \
 DeeObject_VTGetPointerExf(Dee_TYPE(ob),ob,result,ty_fmt,args)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeObject_VTGetPointerExf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_OUT void **result,
 DEE_A_IN_Z char const *ty_fmt, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */

#define DeeObject_GetVoidPointerEx(ob,result) \
 DeeObject_GetPointerEx(ob,(DeeTypeObject *)&DeeNone_Type,result)

#if DEE_DEPRECATED_API_VERSION(100,101,102)
#define DeeObject_AnyPointer(ob)  DeeObject_GetPointer(ob,(DeeTypeObject *)&DeeNone_Type)
#define DeeObject_Pointer_ERROR   (void *)((Dee_size_t)-1)
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2))
 DEE_ATTRIBUTE_DEPRECATED("'DeeObject_GetPointer()' isn't exception safe. Use 'DeeObject_GetPointerEx' instead")
 DEE_A_RET_EXCEPT(-1) void *) DeeObject_GetPointer(
 DEE_A_INOUT DeeObject *ob, DEE_A_IN DeeTypeObject *tp) {
 void *result;
 if (DeeObject_GetPointerEx(ob,tp,&result) != 0)
  return DeeObject_Pointer_ERROR; // Pointer might naturally be this; Error may have occurred before
 return result;
}
#endif
#endif


#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DeeObject_TryGetPointer(ob,tp_result,result) \
 DeeObject_TTryGetPointer(Dee_TYPE(ob),ob,tp_result,result)
#define DeeObject_TryGetPointer2(ob,tp_result,result) \
 DeeObject_TTryGetPointer2(Dee_TYPE(ob),ob,tp_result,result)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeObject_TTryGetPointer(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_result,
 DEE_A_OUT void **result) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeObject_TTryGetPointer2(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_result,
 DEE_A_OUT void **result) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
#endif

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
//////////////////////////////////////////////////////////////////////////
// Similar to 'DeeObject_GetPointerEx'.
// Returns DeeLValue_ADDR(ob) if ob is an lvalue to an instance of tp
// Returns DeeObject_ABSOLUTE_DATA(ob) if ob is an instance of tp
// >> Since lvalues can't be NULL, we return NULL if an error occurred
#define DeeObject_GetLValue(ob,tp_result,result)\
 DeeObject_TGetLValue(Dee_TYPE(ob),ob,tp_result,result)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeObject_TGetLValue(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) *tp_result,
 DEE_A_OUT void **result) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
#endif


#ifdef DEE_LIMITED_API
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeStructured_TGetAttr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructuredObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeStructured_THasAttr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructuredObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT(-1) int DeeStructured_TDelAttr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructuredObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT(-1) int DeeStructured_TSetAttr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructuredObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeStructured_TGetAttrString(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructuredObject) *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeStructured_THasAttrString(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructuredObject) *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT(-1) int DeeStructured_TDelAttrString(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructuredObject) *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT(-1) int DeeStructured_TSetAttrString(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructuredObject) *self, DEE_A_IN_Z char const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeStructured_TCallAttrString(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructuredObject) *self, DEE_A_IN_Z char const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeStructured_TCallAttr(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructuredObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeStructured_GetAttr(DEE_A_INOUT_OBJECT(DeeStructuredObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeStructured_DelAttr(DEE_A_INOUT_OBJECT(DeeStructuredObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeStructured_SetAttr(DEE_A_INOUT_OBJECT(DeeStructuredObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3));
#define DeeStructured_HasAttr(ob,attr)             DeeStructured_THasAttr(Dee_TYPE(ob),ob,attr)
#define DeeStructured_GetAttrString(ob,attr)       DeeStructured_TGetAttrString(Dee_TYPE(ob),ob,attr)
#define DeeStructured_HasAttrString(ob,attr)       DeeStructured_THasAttrString(Dee_TYPE(ob),ob,attr)
#define DeeStructured_DelAttrString(ob,attr)       DeeStructured_TDelAttrString(Dee_TYPE(ob),ob,attr)
#define DeeStructured_SetAttrString(ob,attr,v)     DeeStructured_TSetAttrString(Dee_TYPE(ob),ob,attr,v)
#define DeeStructured_CallAttrString(ob,attr,args) DeeStructured_TCallAttrString(Dee_TYPE(ob),ob,attr,args)
#define DeeStructured_CallAttr(ob,attr,args)       DeeStructured_TCallAttr(Dee_TYPE(ob),ob,attr,args)
#elif defined(DEE_LIMITED_DEX)
#define DeeStructured_GetAttr  (*DeeType_GET_SLOT(&DeeStructured_Type,tp_attr_get))
#define DeeStructured_DelAttr  (*DeeType_GET_SLOT(&DeeStructured_Type,tp_attr_del))
#define DeeStructured_SetAttr  (*DeeType_GET_SLOT(&DeeStructured_Type,tp_attr_set))
#endif

#ifdef DEE_LIMITED_API
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
extern DeeObject *_deelvalue_tp_attr_get(DeeLValueObject *self, DeeObject *attr);
extern int _deelvalue_tp_attr_del(DeeLValueObject *self, DeeObject *attr);
extern int _deelvalue_tp_attr_set(DeeLValueObject *self, DeeObject *attr, DeeObject *v);
#endif
struct DeeMarshalReadMap; struct DeeMarshalWriteMap;
extern int DEE_CALL _deestructured_tp_marshal_ctor(DeeTypeObject *tp_self, DeeStructuredObject *self, DeeObject *infile, struct DeeMarshalReadMap *map);
extern int DEE_CALL _deestructured_tp_marshal_put(DeeTypeObject *tp_self, DeeStructuredObject *self, DeeObject *outfile, struct DeeMarshalWriteMap *map);
#elif defined(DEE_LIMITED_DEX)
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define _deelvalue_tp_attr_get  (*DeeType_GET_SLOT(&DeeLValue_Type,tp_attr_get))
#define _deelvalue_tp_attr_del  (*DeeType_GET_SLOT(&DeeLValue_Type,tp_attr_del))
#define _deelvalue_tp_attr_set  (*DeeType_GET_SLOT(&DeeLValue_Type,tp_attr_set))
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#endif

#if DEE_PLATFORM_HAVE_IO_HANDLE
#ifdef DEE_PLATFORM_WINDOWS
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DEE_PRIVATE_OBJECT_NEW_DeeIOHandle  DeeVoidPointer_New
#define DEE_PRIVATE_OBJECT_TYPE_DeeIOHandle DeeStructuredType_Pointer((DeeTypeObject *)&DeeNone_Type)
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#endif /* DEE_PLATFORM_WINDOWS */
#endif /* DEE_PLATFORM_HAVE_IO_HANDLE */

DEE_DECL_END

#ifdef GUARD_DEEMON_OBJECT_H
#ifndef GUARD_DEEMON_OPTIONAL_STRUCTURED_LVALUE_DECL_H
#include <deemon/optional/structured_lvalue_decl.h>
#endif
#ifndef GUARD_DEEMON_OPTIONAL_STRUCTURED_POINTER_DECL_H
#include <deemon/optional/structured_pointer_decl.h>
#endif
#ifdef GUARD_DEEMON_FLOAT_H
#ifndef GUARD_DEEMON_OPTIONAL_FLOAT_DECL_H
#include <deemon/optional/float_decl.h>
#endif
#endif /* GUARD_DEEMON_FLOAT_H */
#ifdef GUARD_DEEMON_INTEGER_H
#ifndef GUARD_DEEMON_OPTIONAL_INTEGER_DECL_H
#include <deemon/optional/integer_decl.h>
#endif
#endif /* GUARD_DEEMON_INTEGER_H */
#endif /* GUARD_DEEMON_OBJECT_H */

#endif /* !GUARD_DEEMON_STRUCTURED_H */
