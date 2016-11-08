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
#ifndef GUARD_DEEMON_OPTIONAL_TYPE_DECL_H
#define GUARD_DEEMON_OPTIONAL_TYPE_DECL_H 1

#include <deemon/__conf.inl>
#ifdef DEE_LIMITED_DEX
#include <deemon/optional/memberdef_decl.h>
#include <deemon/optional/type_slots.h>
#include <deemon/optional/type_flags.h>
#include <deemon/optional/uuid.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_LIMITED_DEX
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#endif /* DEE_LIMITED_DEX */


#ifdef DEE_LIMITED_DEX
//////////////////////////////////////////////////////////////////////////
// NOTE: None of the '_DeeGeneric*' functions are meant to be called
//       directly. They are only meant as substitutes when important
//       operators are missing in type declarations.
//       They are also used to simplify the addition of new operators,
//       by combining already existing operators in an understandable way.

//////////////////////////////////////////////////////////////////////////
// Generic function for returning "self" in a unary operator (e.g.: tp_pos)
// >> Can be used for optimizations during compilation
// NOTE: Obviously a new reference is added to 'self'
// - '_DeeGeneric_Noop' does the same, but always returns 0.
//   Thereby it can be used for no-op constructors
DEE_FUNC_DECL(DeeObject *) _DeeGeneric_ReturnSelf(DeeObject *self);
DEE_FUNC_DECL(int) _DeeGeneric_Noop(DeeTypeObject *tp_self, DeeObject *self);
DEE_DATA_DECL(struct DeeMemberDef const) _DeeGenericFallback_MemberDef[1];
DEE_DATA_DECL(struct DeeGetSetDef const) _DeeGenericFallback_GetSetDef[1];
DEE_DATA_DECL(struct DeeMethodDef const) _DeeGenericFallback_MethodDef[1];
DEE_FUNC_DECL(DeeObject *) _DeeGenericFallback_tp_new(DeeTypeObject *tp);
DEE_FUNC_DECL(DeeObject *) _DeeGenericFallback_tp_copy_new(DeeTypeObject *tp, DeeObject *self);
DEE_FUNC_DECL(DeeObject *) _DeeGenericFallback_tp_move_new(DeeTypeObject *tp, DeeObject *self);
DEE_FUNC_DECL(DeeObject *) _DeeGenericFallback_tp_any_new(DeeTypeObject *tp, DeeObject *args);
DEE_FUNC_DECL(DeeObject *) _DeeGenericFallback_tp_var_alloc(DeeTypeObject *tp);
DEE_FUNC_DECL(DeeObject *) _DeeGenericFallback_tp_alloc(DeeTypeObject *tp);
DEE_FUNC_DECL(void) _DeeGenericFallback_tp_free(DeeTypeObject *tp, DeeObject *self);
DEE_FUNC_DECL(int) _DeeGenericFallback_tp_ctor_callany(DeeTypeObject *tp, DeeObject *self);
DEE_FUNC_DECL(int) _DeeGenericFallback_tp_copy_ctor_callany(DeeTypeObject *tp, DeeObject *self, DeeObject *arg0);
#define _DeeGenericFallback_tp_move_ctor_callany _DeeGenericFallback_tp_copy_ctor_callany

DEE_FUNC_DECL(DeeObject *) _DeeGenericFallback_tp_new_callany(DeeTypeObject *tp);
DEE_FUNC_DECL(DeeObject *) _DeeGenericFallback_tp_copy_new_callany(DeeTypeObject *tp, DeeObject *arg0);
#define _DeeGenericFallback_tp_move_new_callany _DeeGenericFallback_tp_copy_new_callany
DEE_FUNC_DECL(int) _DeeGenericContext_tp_int32_from_tp_int64(DeeObject *self, Dee_int32_t *result);
DEE_FUNC_DECL(int) _DeeGenericContext_tp_int32_from_tp_double(DeeObject *self, Dee_int32_t *result);
DEE_FUNC_DECL(int) _DeeGenericContext_tp_int64_from_tp_int32(DeeObject *self, Dee_int64_t *result);
DEE_FUNC_DECL(int) _DeeGenericContext_tp_int64_from_tp_double(DeeObject *self, Dee_int64_t *result);
DEE_FUNC_DECL(int) _DeeGenericContext_tp_double_from_tp_int32(DeeObject *self, double *result);
DEE_FUNC_DECL(int) _DeeGenericContext_tp_double_from_tp_int64(DeeObject *self, double *result);
DEE_FUNC_DECL(DeeObject *) _DeeGenericContext_tp_incpost_from_tp_inc(DeeObject *self);
DEE_FUNC_DECL(DeeObject *) _DeeGenericContext_tp_decpost_from_tp_dec(DeeObject *self);
DEE_FUNC_DECL(DeeObject *) _DeeGenericContext_tp_not_from_bool(DeeObject *self);
DEE_FUNC_DECL(int) _DeeGenericMissing_tp_ctor(DeeTypeObject *tp, DeeObject *self);
DEE_FUNC_DECL(int) _DeeGenericMissing_tp_copy_ctor(DeeTypeObject *tp, DeeObject *self, DeeObject *right);
DEE_FUNC_DECL(int) _DeeGenericMissing_tp_move_ctor(DeeTypeObject *tp, DeeObject *self, DeeObject *right);
DEE_FUNC_DECL(int) _DeeGenericMissing_tp_any_ctor(DeeTypeObject *tp, DeeObject *self, DeeObject *args);
#endif /* DEE_LIMITED_DEX */


#ifdef DEE_LIMITED_DEX
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct _DeeTypeConstructorOperators {
 // Constant sizeof instances (unused if variadic size)
 DeeType_SLOT_TYPE(tp_instance_size) tp_instance_size;
 DeeType_SLOT_TYPE(tp_alloc)         tp_alloc;
 // NOTE: When overriding a constructor, you must call the
 //       constructor of the underlying class yourself.
 //       >> int _myobject_tp_ctor(DeeObject *self) {
 //       >>  if (DeeType_GET_SLOT(DeeType_BASE(Dee_TYPE(self)),tp_ctor)(self) == -1) return -1;
 //       >>  // Your own initialization here
 //       >>  return 0;
 //       >> }
 //       - Do so before executing your own initialization code
 //       - Same goes for all the other constructors, but
 //         you can decide how to call them yourself.
 //       Hint: Only applies if you're adding a type in c/c++.
 //             In user code this is enforced by the compiler!
 // NOTE: The same rule applies to assign operators.
 // NOTE: Destructors are different though!
 //       - Deemon will automatically break down objects by starting
 //         with the destructor of the top-most type.
 //       - After that one is called, it is checked if the object
 //         hasn't been given another reference by some means.
 //         (That's how you abort a destructor)
 //       - If it has gained a reference, destruction is aborted and the object
 //         will live on as the type who's destructor added the last reference.
 //         Once the last destructor has been called, the object will be freed.
 //       - While being freed the object can no longer be revived!
 // -- The *new* versions --
 //  - allocate and initialize header
 //  - used if a "DEE_TYPE_FLAG_VAR_OBJECT" type
 //  - Does the job of both the tp_alloc and tp_any_ctor
 //    -> Variadic types don't use the different ctors
 union{
  DeeType_SLOT_TYPE(tp_ctor)      tp_ctor_;
  DeeType_SLOT_TYPE(tp_new)       tp_new_;
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define tp_ctor_ _tp_ctor.tp_ctor_
#define tp_new_  _tp_ctor.tp_new_
 _tp_ctor
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
 ;
 union{
  DeeType_SLOT_TYPE(tp_copy_ctor) tp_copy_ctor_;
  DeeType_SLOT_TYPE(tp_copy_new)  tp_copy_new_;
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define tp_copy_ctor_ _tp_copy_ctor.tp_copy_ctor_
#define tp_copy_new_  _tp_copy_ctor.tp_copy_new_
 _tp_copy_ctor
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
 ;
 union{
  DeeType_SLOT_TYPE(tp_move_ctor) tp_move_ctor_;
  DeeType_SLOT_TYPE(tp_move_new)  tp_move_new_;
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define tp_move_ctor_ _tp_move_ctor.tp_move_ctor_
#define tp_move_new_  _tp_move_ctor.tp_move_new_
 _tp_move_ctor
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
 ;
 union{
  DeeType_SLOT_TYPE(tp_any_ctor)  tp_any_ctor_;
  DeeType_SLOT_TYPE(tp_any_new)   tp_any_new_;
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define tp_any_ctor_ _tp_any_ctor.tp_any_ctor_
#define tp_any_new_  _tp_any_ctor.tp_any_new_
 _tp_any_ctor
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
 ;
};
DEE_COMPILER_MSVC_WARNING_POP
struct _DeeTypeDestructorOperators {
 // v Must finalize object header (decref type) and free(self)
 //   >> "self" is no longer a valid DeeObject ('DeeObject_Check(self)' will return false)
 //   >> Dee_TYPE(self) cannot be trusted; use "tp" as typing for 'self'
 DeeType_SLOT_TYPE(tp_free) tp_free;
 // v Must finalize object content (finalize / free / decref members)
 //   Object must not remain in a consistent state afterwards;
 //   >> 
 DeeType_SLOT_TYPE(tp_dtor) tp_dtor;
};
struct _DeeTypeAssignOperators {
 DeeType_SLOT_TYPE(tp_copy_assign) tp_copy_assign;
 DeeType_SLOT_TYPE(tp_move_assign) tp_move_assign;
 DeeType_SLOT_TYPE(tp_any_assign)  tp_any_assign;
};
struct _DeeTypeCastOperators {
 DeeType_SLOT_TYPE(tp_str)    tp_str;
 DeeType_SLOT_TYPE(tp_repr)   tp_repr;
 DeeType_SLOT_TYPE(tp_int32)  tp_int32;   // Integer value (if -1: must check for error)
 DeeType_SLOT_TYPE(tp_int64)  tp_int64;   // Integer value (if -1: must check for error)
 DeeType_SLOT_TYPE(tp_double) tp_double; // Double value (if -1.0: must check for error)
};
struct _DeeTypeObjectOperators {
 DeeType_SLOT_TYPE(tp_call)  tp_call;  // 'self(args...)'
 // Internal; Used for implementing garbage collection
 // - Visit all referenced objects
 DeeType_SLOT_TYPE(tp_visit) tp_visit;
 // Internal; Used for implementing garbage collection
 // - Kill any potential references to self
 // - Abort destruction if some references remain
 // - Object must remain in a valid state (tp_clear can be called multiple times)
 // - Only called if object is being destructed by the garbage collector
 DeeType_SLOT_TYPE(tp_clear) tp_clear;
};
struct _DeeTypeMathOperators {
 DeeType_SLOT_TYPE(tp_not)     tp_not;     // '!self'
 DeeType_SLOT_TYPE(tp_bool)    tp_bool;    // 0: false; 1: true; -1: error
 DeeType_SLOT_TYPE(tp_inv)     tp_inv;     // '~self'
 DeeType_SLOT_TYPE(tp_pos)     tp_pos;     // '+self'
 DeeType_SLOT_TYPE(tp_neg)     tp_neg;     // '-self'
 DeeType_SLOT_TYPE(tp_inc)     tp_inc;     // '++self'
 DeeType_SLOT_TYPE(tp_incpost) tp_incpost; // 'self++'
 DeeType_SLOT_TYPE(tp_dec)     tp_dec;     // '--self'
 DeeType_SLOT_TYPE(tp_decpost) tp_decpost; // 'self--'
 DeeType_SLOT_TYPE(tp_add)     tp_add;     // 'lhs+rhs'
 DeeType_SLOT_TYPE(tp_iadd)    tp_iadd;    // 'lhs+=rhs'
 DeeType_SLOT_TYPE(tp_sub)     tp_sub;     // 'lhs-rhs'
 DeeType_SLOT_TYPE(tp_isub)    tp_isub;    // 'lhs-=rhs'
 DeeType_SLOT_TYPE(tp_mul)     tp_mul;     // 'lhs*rhs'
 DeeType_SLOT_TYPE(tp_imul)    tp_imul;    // 'lhs*=rhs'
 DeeType_SLOT_TYPE(tp_div)     tp_div;     // 'lhs/rhs'
 DeeType_SLOT_TYPE(tp_idiv)    tp_idiv;    // 'lhs/=rhs'
 DeeType_SLOT_TYPE(tp_mod)     tp_mod;     // 'lhs%rhs'
 DeeType_SLOT_TYPE(tp_imod)    tp_imod;    // 'lhs%=rhs'
 DeeType_SLOT_TYPE(tp_shl)     tp_shl;     // 'lhs<<rhs'
 DeeType_SLOT_TYPE(tp_ishl)    tp_ishl;    // 'lhs<<=rhs'
 DeeType_SLOT_TYPE(tp_shr)     tp_shr;     // 'lhs>>rhs'
 DeeType_SLOT_TYPE(tp_ishr)    tp_ishr;    // 'lhs>>=rhs'
 DeeType_SLOT_TYPE(tp_and)     tp_and;     // 'lhs&rhs'
 DeeType_SLOT_TYPE(tp_iand)    tp_iand;    // 'lhs&=rhs'
 DeeType_SLOT_TYPE(tp_or)      tp_or;      // 'lhs|rhs'
 DeeType_SLOT_TYPE(tp_ior)     tp_ior;     // 'lhs|=rhs'
 DeeType_SLOT_TYPE(tp_xor)     tp_xor;     // 'lhs^rhs'
 DeeType_SLOT_TYPE(tp_ixor)    tp_ixor;    // 'lhs^=rhs'
 DeeType_SLOT_TYPE(tp_pow)     tp_pow;     // 'lhs**rhs'
 DeeType_SLOT_TYPE(tp_ipow)    tp_ipow;    // 'lhs**=rhs'
 DeeType_SLOT_TYPE(tp_hash)    tp_hash;
};
struct _DeeTypeCompareOperators {
 DeeType_SLOT_TYPE(tp_cmp_lo) tp_cmp_lo; // 'lhs<rhs'
 DeeType_SLOT_TYPE(tp_cmp_le) tp_cmp_le; // 'lhs<=rhs'
 DeeType_SLOT_TYPE(tp_cmp_eq) tp_cmp_eq; // 'lhs==rhs'
 DeeType_SLOT_TYPE(tp_cmp_ne) tp_cmp_ne; // 'lhs!=rhs'
 DeeType_SLOT_TYPE(tp_cmp_gr) tp_cmp_gr; // 'lhs>rhs'
 DeeType_SLOT_TYPE(tp_cmp_ge) tp_cmp_ge; // 'lhs>=rhs'
};
struct _DeeTypeSeqOperators {
 DeeType_SLOT_TYPE(tp_seq_get)       tp_seq_get;       // 'self[i]'
 DeeType_SLOT_TYPE(tp_seq_del)       tp_seq_del;       // 'del self[i]'
 DeeType_SLOT_TYPE(tp_seq_set)       tp_seq_set;       // 'self[i] = v'
 DeeType_SLOT_TYPE(tp_seq_size)      tp_seq_size;      // '#self'
 DeeType_SLOT_TYPE(tp_seq_contains)  tp_seq_contains;  // 'ob in self'
 DeeType_SLOT_TYPE(tp_seq_range_get) tp_seq_range_get; // 'self[lo:hi]'     // NOTE: lo / hi are Dee_None if omitted
 DeeType_SLOT_TYPE(tp_seq_range_del) tp_seq_range_del; // 'del self[lo:hi]' // NOTE: lo / hi are Dee_None if omitted
 DeeType_SLOT_TYPE(tp_seq_range_set) tp_seq_range_set; // 'self[lo:hi] = v' // NOTE: lo / hi are Dee_None if omitted
 DeeType_SLOT_TYPE(tp_seq_iter_self) tp_seq_iter_self; // Returns a new iterator for "self"
 DeeType_SLOT_TYPE(tp_seq_iter_next) tp_seq_iter_next; // Yields one from an iterator (return NULL for end / error)
};
struct _DeeTypeAttributeOperators {
 // NOTE: If you override these, you won't be able to use
 //       tp_members / tp_getsets / tp_methods
 DeeType_SLOT_TYPE(tp_attr_get)      tp_attr_get;      /*< 'self.attr'. */
 DeeType_SLOT_TYPE(tp_attr_del)      tp_attr_del;      /*< 'del self.attr;'. */
 DeeType_SLOT_TYPE(tp_attr_set)      tp_attr_set;      /*< 'self.attr = v'. */
 DeeType_SLOT_TYPE(tp_members)       tp_members;       /*< [1..1] Member attributes. */
 DeeType_SLOT_TYPE(tp_getsets)       tp_getsets;       /*< [1..1] Get/Set attributes. */
 DeeType_SLOT_TYPE(tp_methods)       tp_methods;       /*< [1..1] Method attributes. */
 DeeType_SLOT_TYPE(tp_class_members) tp_class_members; /*< [1..1] Class Member attributes. */
 DeeType_SLOT_TYPE(tp_class_getsets) tp_class_getsets; /*< [1..1] Class Get/Set attributes. */
 DeeType_SLOT_TYPE(tp_class_methods) tp_class_methods; /*< [1..1] Class Method attributes. */
};

struct DeeMarshalReadMap;
struct DeeMarshalWriteMap;

//////////////////////////////////////////////////////////////////////////
// Constructor callback for mashaled object initialization
typedef DEE_A_RET_EXCEPT(-1) int (DEE_CALL *DeeType_SLOT_TYPE(tp_marshal_ctor))(
 DEE_A_INOUT DeeTypeObject *tp_self, DEE_A_OUT DeeObject *self,
 DEE_A_INOUT DeeObject *infile, DEE_A_INOUT struct DeeMarshalReadMap *map);

typedef void (DEE_CALL *PDeeMashalNewAfterAlloc)(
 DEE_A_INOUT DeeObject *allocated_object, void *closure);

//////////////////////////////////////////////////////////////////////////
// Callback for variable-style objects creation
// NOTE: As soon as the object can, and is allocated, 'after_alloc'
//       MUST be called BEFORE the object that will eventually be
//       returned has been fully initialized.
//       >> This is required for supporting recursive objects.
//       >> Basically call it ONCE as soon as Dee_INCREF would work.
typedef DeeObject *(DEE_CALL *DeeType_SLOT_TYPE(tp_marshal_new))(
 DEE_A_INOUT DeeTypeObject *tp_self, DEE_A_INOUT DeeObject *infile,
 DEE_A_INOUT struct DeeMarshalReadMap *map,
 DEE_A_IN PDeeMashalNewAfterAlloc after_alloc, void *after_alloc_data);

typedef DEE_A_RET_EXCEPT(-1) int (DEE_CALL *DeeType_SLOT_TYPE(tp_marshal_put))(
 DEE_A_INOUT DeeTypeObject *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *outfile, DEE_A_INOUT struct DeeMarshalWriteMap *map);

DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeTypeMarshal {
 struct DeeUUID                      tp_uuid;          /*< Unique identifier for this type (static). */
 union{
  DeeType_SLOT_TYPE(tp_marshal_ctor) tp_marshal_ctor_; /*< [1..1] Constructor from marshal. */
  DeeType_SLOT_TYPE(tp_marshal_new)  tp_marshal_new_;  /*< [1..1] Variable-size constructor from marshal. */
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define tp_marshal_ctor_ _tp_marshal_ctor.tp_marshal_ctor_
#define tp_marshal_new_  _tp_marshal_ctor.tp_marshal_new_
 _tp_marshal_ctor
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
 ;
 DeeType_SLOT_TYPE(tp_marshal_put)   tp_marshal_put;   /*< [1..1] Marshal-write operator. */
 struct DeeUUID                     *tp_alias_uuids;   /*< [0..1] Empty-uuid terminated list of aliases. */
};
DEE_COMPILER_MSVC_WARNING_POP
#define DEE_TYPE_MARSHAL_v101(_tp_uuid,_tp_marshal_ctor,_tp_marshal_put,_tp_alias_uuids) \
{_tp_uuid\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_marshal_ctor),DEE_TYPEMEMBER(_tp_marshal_ctor,____MUST_IMPLEMENT____))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_marshal_put),DEE_TYPEMEMBER(_tp_marshal_put,____MUST_IMPLEMENT____))\
,DEE_TYPEMEMBER(_tp_alias_uuids,NULL)\
}
#define DEE_TYPE_MARSHAL_VAR_v101(_tp_uuid,_tp_marshal_new,_tp_marshal_put,_tp_alias_uuids) \
{_tp_uuid,(DeeType_SLOT_TYPE(tp_marshal_ctor))\
 DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_marshal_new),DEE_TYPEMEMBER(_tp_marshal_new,____MUST_IMPLEMENT____))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_marshal_put),DEE_TYPEMEMBER(_tp_marshal_put,____MUST_IMPLEMENT____))\
,DEE_TYPEMEMBER(_tp_alias_uuids,NULL)\
}
#define DEE_TYPE_MARSHAL_v100(_tp_uuid,_tp_marshal_ctor,_tp_marshal_put) \
{_tp_uuid\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_marshal_ctor),DEE_TYPEMEMBER(_tp_marshal_ctor,____MUST_IMPLEMENT____))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_marshal_put),DEE_TYPEMEMBER(_tp_marshal_put,____MUST_IMPLEMENT____))\
,NULL}
#define DEE_TYPE_MARSHAL_VAR_v100(_tp_uuid,_tp_marshal_new,_tp_marshal_put) \
{_tp_uuid,(DeeType_SLOT_TYPE(tp_marshal_ctor))\
 DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_marshal_new),DEE_TYPEMEMBER(_tp_marshal_new,____MUST_IMPLEMENT____))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_marshal_put),DEE_TYPEMEMBER(_tp_marshal_put,____MUST_IMPLEMENT____))\
,NULL}

struct DeeMemberDef;
struct DeeGetSetDef;
struct DeeMethodDef;
struct DeeTupleObject;
struct DeeTypeObject {
 DEE_OBJECT_HEAD
 DeeType_SLOT_TYPE(tp_name)           tp_name;  /*< [0..1] Name of this type. */
 DeeType_SLOT_TYPE(tp_doc)            tp_doc;   /*< [0..1] Doc of this type. */
 DeeType_SLOT_TYPE(tp_flags)          tp_flags; /*< Type flags. */
 DEE_A_REF DeeType_SLOT_TYPE(tp_base) tp_base;  /*< [0..1] Base class (may only be NULL if all operators are implemented). */
 struct _DeeTypeConstructorOperators  tp_constructor;
 struct _DeeTypeDestructorOperators   tp_destructor;
 struct _DeeTypeAssignOperators       tp_assign;
 struct _DeeTypeCastOperators         tp_cast;
 struct _DeeTypeObjectOperators       tp_object;
 struct _DeeTypeMathOperators         tp_math;
 struct _DeeTypeCompareOperators      tp_compare;
 struct _DeeTypeSeqOperators          tp_seq;
 struct _DeeTypeAttributeOperators    tp_attr;
 DeeType_SLOT_TYPE(tp_marshal)        tp_marshal;
};

#if 0
#define _DEE_IS_TYPEMEMBER_(x)       1
#define _DEE_TYPEMEMBER_GET(x)       x
#endif
#define _DEE_IS_TYPEMEMBER_member(x) 1
#define _DEE_IS_TYPEMEMBER_null      0
#define _DEE_TYPEMEMBER_0(x,f)       f
#define _DEE_TYPEMEMBER_1(x,f)       x
#define _DEE_TYPEMEMBER_GETmember(x) x
#define DEE_TYPEMEMBER(x,f)      DEE_PP_CAT_2(_DEE_TYPEMEMBER_,_DEE_IS_TYPEMEMBER_##x)(_DEE_TYPEMEMBER_GET##x,f)
#define DEE_TYPEMEMBER_EX(x,t,f) DEE_PP_CAT_2(_DEE_TYPEMEMBER_,_DEE_IS_TYPEMEMBER_##x)(t,f)

//////////////////////////////////////////////////////////////////////////
// Type initializers
// - Functions can be
//   - 'null'  : Use default function / member
//   - '(fun)' : 'fun' is the function to use
// NOTE: Due to the way macros work, you can't use
//       'NULL' as argument. - Use 'null' instead

//////////////////////////////////////////////////////////////////////////
// Static type initializers
#define DEE_TYPE_OBJECT_HEAD_v100(_tp_name,_tp_doc,_tp_flags,_tp_base)\
 DEE_OBJECT_HEAD_INIT(&DeeType_Type),\
 DEE_TYPEMEMBER(_tp_name,DeeType_DEFAULT_SLOT(tp_name)),\
 Dee_DOCSTRING(DEE_TYPEMEMBER(_tp_doc,DeeType_DEFAULT_SLOT(tp_doc))),\
 DEE_TYPEMEMBER(_tp_flags,DeeType_DEFAULT_SLOT(tp_flags))|\
 DEE_TYPE_FLAG_STATIC_NAME|DEE_TYPE_FLAG_STATIC_DOC,\
 DEE_TYPEMEMBER(_tp_base,DeeType_DEFAULT_SLOT(tp_base))
#define DEE_TYPE_OBJECT_HEAD_EX_v100(_ob_type,_tp_name,_tp_doc,_tp_flags,_tp_base)\
 DEE_OBJECT_HEAD_INIT(DEE_TYPEMEMBER(_ob_type,&DeeType_Type)),\
 DEE_TYPEMEMBER(_tp_name,DeeType_DEFAULT_SLOT(tp_name)),\
 Dee_DOCSTRING(DEE_TYPEMEMBER(_tp_doc,DeeType_DEFAULT_SLOT(tp_doc))),\
 DEE_TYPEMEMBER(_tp_flags,DeeType_DEFAULT_SLOT(tp_flags))|\
 DEE_TYPE_FLAG_STATIC_NAME|DEE_TYPE_FLAG_STATIC_DOC,\
 DEE_TYPEMEMBER(_tp_base,DeeType_DEFAULT_SLOT(tp_base))

#define DEE_TYPE_OBJECT_CONSTRUCTOR_v100(_tp_instance_size,_tp_alloc,_tp_ctor,_tp_copy_ctor,_tp_move_ctor,_tp_any_ctor) \
{(_tp_instance_size)\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_alloc),DEE_TYPEMEMBER(_tp_alloc,DeeType_DEFAULT_SLOT(tp_alloc)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_ctor),DEE_TYPEMEMBER(_tp_ctor,DEE_TYPEMEMBER_EX(_tp_any_ctor,&_DeeGenericFallback_tp_ctor_callany,DeeType_DEFAULT_SLOT(tp_ctor))))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_copy_ctor),DEE_TYPEMEMBER(_tp_copy_ctor,DEE_TYPEMEMBER_EX(_tp_any_ctor,&_DeeGenericFallback_tp_copy_ctor_callany,DeeType_DEFAULT_SLOT(tp_copy_ctor))))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_move_ctor),DEE_TYPEMEMBER(_tp_move_ctor,DEE_TYPEMEMBER(_tp_copy_ctor,DEE_TYPEMEMBER_EX(_tp_any_ctor,&_DeeGenericFallback_tp_move_ctor_callany,DeeType_DEFAULT_SLOT(tp_move_ctor)))))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_any_ctor),DEE_TYPEMEMBER(_tp_any_ctor,DeeType_DEFAULT_SLOT(tp_any_ctor)))\
}
#define DEE_TYPE_OBJECT_VAR_CONSTRUCTOR_EX_v101(tp_instance_size,_tp_new,_tp_copy_new,_tp_move_new,_tp_any_new) \
{tp_instance_size,&_DeeGenericFallback_tp_var_alloc\
,(DeeType_SLOT_TYPE(tp_ctor))DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_new),DEE_TYPEMEMBER(_tp_new,DEE_TYPEMEMBER_EX(_tp_any_new,&_DeeGenericFallback_tp_new_callany,DeeType_DEFAULT_SLOT(tp_new))))\
,(DeeType_SLOT_TYPE(tp_copy_ctor))DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_copy_new),DEE_TYPEMEMBER(_tp_copy_new,DEE_TYPEMEMBER_EX(_tp_any_new,&_DeeGenericFallback_tp_copy_new_callany,DeeType_DEFAULT_SLOT(tp_copy_new))))\
,(DeeType_SLOT_TYPE(tp_move_ctor))DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_move_new),DEE_TYPEMEMBER(_tp_move_new,DEE_TYPEMEMBER(_tp_copy_new,DEE_TYPEMEMBER_EX(_tp_any_new,&_DeeGenericFallback_tp_move_new_callany,DeeType_DEFAULT_SLOT(tp_move_new)))))\
,(DeeType_SLOT_TYPE(tp_any_ctor))DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_any_new),DEE_TYPEMEMBER(_tp_any_new,DeeType_DEFAULT_SLOT(tp_any_new)))\
}
#define DEE_TYPE_OBJECT_VAR_CONSTRUCTOR_v101(_tp_new,_tp_copy_new,_tp_move_new,_tp_any_new) \
 DEE_TYPE_OBJECT_VAR_CONSTRUCTOR_EX_v101(0,_tp_new,_tp_copy_new,_tp_move_new,_tp_any_new)
#define DEE_TYPE_OBJECT_DESTRUCTOR_v100(_tp_free,_tp_dtor) \
{DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_free),DEE_TYPEMEMBER(_tp_free,DeeType_DEFAULT_SLOT(tp_free)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_dtor),DEE_TYPEMEMBER(_tp_dtor,DeeType_DEFAULT_SLOT(tp_dtor)))\
}
#define DEE_TYPE_OBJECT_ASSIGN_v100(_tp_copy_assign,_tp_move_assign,_tp_any_assign) \
{DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_copy_assign),DEE_TYPEMEMBER(_tp_copy_assign,DEE_TYPEMEMBER(_tp_any_assign,DeeType_DEFAULT_SLOT(tp_copy_assign))))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_move_assign),DEE_TYPEMEMBER(_tp_move_assign,DEE_TYPEMEMBER(_tp_copy_assign,DEE_TYPEMEMBER(_tp_any_assign,DeeType_DEFAULT_SLOT(tp_move_assign)))))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_any_assign),DEE_TYPEMEMBER(_tp_any_assign,DeeType_DEFAULT_SLOT(tp_any_assign)))\
}
// NOTE: Cannot support 'DEE_TYPE_OBJECT_CAST_v100' (incompatible binary structure)
#define DEE_TYPE_OBJECT_CAST_v101(_tp_str,_tp_repr,_tp_int32,_tp_int64,_tp_double) \
{DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_str),DEE_TYPEMEMBER(_tp_str,DeeType_DEFAULT_SLOT(tp_str)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_repr),DEE_TYPEMEMBER(_tp_repr,DeeType_DEFAULT_SLOT(tp_repr)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_int32),DEE_TYPEMEMBER(_tp_int32,DEE_TYPEMEMBER_EX(_tp_int64,&_DeeGenericContext_tp_int32_from_tp_int64,DEE_TYPEMEMBER_EX(_tp_double,&_DeeGenericContext_tp_int32_from_tp_double,DeeType_DEFAULT_SLOT(tp_int32)))))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_int64),DEE_TYPEMEMBER(_tp_int64,DEE_TYPEMEMBER_EX(_tp_int32,&_DeeGenericContext_tp_int64_from_tp_int32,DEE_TYPEMEMBER_EX(_tp_double,&_DeeGenericContext_tp_int64_from_tp_double,DeeType_DEFAULT_SLOT(tp_int64)))))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_double),DEE_TYPEMEMBER(_tp_double,DEE_TYPEMEMBER_EX(_tp_int32,&_DeeGenericContext_tp_double_from_tp_int32,DEE_TYPEMEMBER_EX(_tp_int64,&_DeeGenericContext_tp_double_from_tp_int64,DeeType_DEFAULT_SLOT(tp_double)))))\
}
#define DEE_TYPE_OBJECT_OBJECT_v101(_tp_call,_tp_visit,_tp_clear)\
{DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_call),DEE_TYPEMEMBER(_tp_call,DeeType_DEFAULT_SLOT(tp_call)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_visit),DEE_TYPEMEMBER(_tp_visit,DeeType_DEFAULT_SLOT(tp_visit)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_clear),DEE_TYPEMEMBER(_tp_clear,DeeType_DEFAULT_SLOT(tp_clear)))\
}
#define DEE_TYPE_OBJECT_OBJECT_v100(_tp_call,_tp_visit) \
 DEE_TYPE_OBJECT_OBJECT_v101(_tp_call,_tp_visit,null)

// NOTE: Cannot support 'DEE_TYPE_OBJECT_MATH_v100' (incompatible binary structure)
#define DEE_TYPE_OBJECT_MATH_v101(\
 _tp_not,_tp_bool,_tp_inv,_tp_pos,_tp_neg,_tp_inc,_tp_incpost,_tp_dec,_tp_decpost,_tp_add,_tp_iadd,\
 _tp_sub,_tp_isub,_tp_mul,_tp_imul,_tp_div,_tp_idiv,_tp_mod,_tp_imod,_tp_shl,_tp_ishl,\
 _tp_shr,_tp_ishr,_tp_and,_tp_iand,_tp_or,_tp_ior,_tp_xor,_tp_ixor,_tp_pow,_tp_ipow,_tp_hash)\
{DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_not),DEE_TYPEMEMBER(_tp_not,DEE_TYPEMEMBER_EX(_tp_bool,&_DeeGenericContext_tp_not_from_bool,DeeType_DEFAULT_SLOT(tp_not))))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_bool),DEE_TYPEMEMBER(_tp_bool,DeeType_DEFAULT_SLOT(tp_bool)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_inv),DEE_TYPEMEMBER(_tp_inv,DeeType_DEFAULT_SLOT(tp_inv)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_pos),DEE_TYPEMEMBER(_tp_pos,DeeType_DEFAULT_SLOT(tp_pos)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_neg),DEE_TYPEMEMBER(_tp_neg,DeeType_DEFAULT_SLOT(tp_neg)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_inc),DEE_TYPEMEMBER(_tp_inc,DeeType_DEFAULT_SLOT(tp_inc)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_incpost),DEE_TYPEMEMBER(_tp_incpost,DEE_TYPEMEMBER_EX(_tp_inc,&_DeeGenericContext_tp_incpost_from_tp_inc,DeeType_DEFAULT_SLOT(tp_incpost))))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_dec),DEE_TYPEMEMBER(_tp_dec,DeeType_DEFAULT_SLOT(tp_dec)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_decpost),DEE_TYPEMEMBER(_tp_decpost,DEE_TYPEMEMBER_EX(_tp_dec,&_DeeGenericContext_tp_decpost_from_tp_dec,DeeType_DEFAULT_SLOT(tp_decpost))))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_add),DEE_TYPEMEMBER(_tp_add,DeeType_DEFAULT_SLOT(tp_add)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_iadd),DEE_TYPEMEMBER(_tp_iadd,DeeType_DEFAULT_SLOT(tp_iadd)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_sub),DEE_TYPEMEMBER(_tp_sub,DeeType_DEFAULT_SLOT(tp_sub)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_isub),DEE_TYPEMEMBER(_tp_isub,DeeType_DEFAULT_SLOT(tp_isub)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_mul),DEE_TYPEMEMBER(_tp_mul,DeeType_DEFAULT_SLOT(tp_mul)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_imul),DEE_TYPEMEMBER(_tp_imul,DeeType_DEFAULT_SLOT(tp_imul)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_div),DEE_TYPEMEMBER(_tp_div,DeeType_DEFAULT_SLOT(tp_div)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_idiv),DEE_TYPEMEMBER(_tp_idiv,DeeType_DEFAULT_SLOT(tp_idiv)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_mod),DEE_TYPEMEMBER(_tp_mod,DeeType_DEFAULT_SLOT(tp_mod)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_imod),DEE_TYPEMEMBER(_tp_imod,DeeType_DEFAULT_SLOT(tp_imod)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_shl),DEE_TYPEMEMBER(_tp_shl,DeeType_DEFAULT_SLOT(tp_shl)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_ishl),DEE_TYPEMEMBER(_tp_ishl,DeeType_DEFAULT_SLOT(tp_ishl)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_shr),DEE_TYPEMEMBER(_tp_shr,DeeType_DEFAULT_SLOT(tp_shr)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_ishr),DEE_TYPEMEMBER(_tp_ishr,DeeType_DEFAULT_SLOT(tp_ishr)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_and),DEE_TYPEMEMBER(_tp_and,DeeType_DEFAULT_SLOT(tp_and)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_iand),DEE_TYPEMEMBER(_tp_iand,DeeType_DEFAULT_SLOT(tp_iand)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_or),DEE_TYPEMEMBER(_tp_or,DeeType_DEFAULT_SLOT(tp_or)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_ior),DEE_TYPEMEMBER(_tp_ior,DeeType_DEFAULT_SLOT(tp_ior)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_xor),DEE_TYPEMEMBER(_tp_xor,DeeType_DEFAULT_SLOT(tp_xor)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_ixor),DEE_TYPEMEMBER(_tp_ixor,DeeType_DEFAULT_SLOT(tp_ixor)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_pow),DEE_TYPEMEMBER(_tp_pow,DeeType_DEFAULT_SLOT(tp_pow)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_ipow),DEE_TYPEMEMBER(_tp_ipow,DeeType_DEFAULT_SLOT(tp_ipow)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_hash),DEE_TYPEMEMBER(_tp_hash,DeeType_DEFAULT_SLOT(tp_hash)))\
}
#define DEE_TYPE_OBJECT_COMPARE_v100(\
 _tp_cmp_lo,_tp_cmp_le,_tp_cmp_eq,_tp_cmp_ne,_tp_cmp_gr,_tp_cmp_ge)\
{DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_cmp_lo),DEE_TYPEMEMBER(_tp_cmp_lo,DeeType_DEFAULT_SLOT(tp_cmp_lo)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_cmp_le),DEE_TYPEMEMBER(_tp_cmp_le,DeeType_DEFAULT_SLOT(tp_cmp_le)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_cmp_eq),DEE_TYPEMEMBER(_tp_cmp_eq,DeeType_DEFAULT_SLOT(tp_cmp_eq)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_cmp_ne),DEE_TYPEMEMBER(_tp_cmp_ne,DeeType_DEFAULT_SLOT(tp_cmp_ne)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_cmp_gr),DEE_TYPEMEMBER(_tp_cmp_gr,DeeType_DEFAULT_SLOT(tp_cmp_gr)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_cmp_ge),DEE_TYPEMEMBER(_tp_cmp_ge,DeeType_DEFAULT_SLOT(tp_cmp_ge)))\
}
// NOTE: Cannot support 'DEE_TYPE_OBJECT_SEQ_v101' (incompatible binary structure)
#define DEE_TYPE_OBJECT_SEQ_v101(_tp_seq_get,_tp_seq_del,_tp_seq_set,_tp_seq_size,_tp_seq_contains,\
 _tp_seq_range_get,_tp_seq_range_del,_tp_seq_range_set,_tp_seq_iter_self,_tp_seq_iter_next)\
{DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_seq_get),DEE_TYPEMEMBER(_tp_seq_get,DEE_TYPEMEMBER_EX(_tp_seq_iter_self,&_deegenericiterable_tp_seq_get,DeeType_DEFAULT_SLOT(tp_seq_get))))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_seq_del),DEE_TYPEMEMBER(_tp_seq_del,DeeType_DEFAULT_SLOT(tp_seq_del)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_seq_set),DEE_TYPEMEMBER(_tp_seq_set,DeeType_DEFAULT_SLOT(tp_seq_set)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_seq_size),DEE_TYPEMEMBER(_tp_seq_size,DEE_TYPEMEMBER_EX(_tp_seq_iter_self,&_deegenericiterable_tp_seq_size,DeeType_DEFAULT_SLOT(tp_seq_size))))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_seq_contains),DEE_TYPEMEMBER(_tp_seq_contains,DEE_TYPEMEMBER_EX(_tp_seq_iter_self,&_deegenericiterable_tp_seq_contains,DEE_TYPEMEMBER_EX(_tp_seq_size,DEE_TYPEMEMBER_EX(_tp_seq_get,&_deegenericiterable_tp_seq_contains,DeeType_DEFAULT_SLOT(tp_seq_contains)),DeeType_DEFAULT_SLOT(tp_seq_contains)))))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_seq_range_get),DEE_TYPEMEMBER(_tp_seq_range_get,DEE_TYPEMEMBER_EX(_tp_seq_range_get,&_deegenericiterable_tp_seq_range_get,DEE_TYPEMEMBER_EX(_tp_seq_size,DEE_TYPEMEMBER_EX(_tp_seq_get,&_deegenericiterable_tp_seq_range_get,DeeType_DEFAULT_SLOT(tp_seq_range_get)),DeeType_DEFAULT_SLOT(tp_seq_range_get)))))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_seq_range_del),DEE_TYPEMEMBER(_tp_seq_range_del,DeeType_DEFAULT_SLOT(tp_seq_range_del)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_seq_range_set),DEE_TYPEMEMBER(_tp_seq_range_set,DeeType_DEFAULT_SLOT(tp_seq_range_set)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_seq_iter_self),DEE_TYPEMEMBER(_tp_seq_iter_self,DEE_TYPEMEMBER_EX(_tp_seq_size,DEE_TYPEMEMBER_EX(_tp_seq_get,&DeeGenericSequenceIterator_New,DeeType_DEFAULT_SLOT(tp_seq_iter_self)),DeeType_DEFAULT_SLOT(tp_seq_iter_self))))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_seq_iter_next),DEE_TYPEMEMBER(_tp_seq_iter_next,DeeType_DEFAULT_SLOT(tp_seq_iter_next)))\
}
#define DEE_TYPE_OBJECT_ATTRIBUTE_v100(_tp_attr_get,_tp_attr_del,_tp_attr_set,\
 _tp_members,_tp_getsets,_tp_methods,_tp_class_members,_tp_class_getsets,_tp_class_methods)\
{DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_attr_get),DEE_TYPEMEMBER(_tp_attr_get,DeeType_DEFAULT_SLOT(tp_attr_get)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_attr_del),DEE_TYPEMEMBER(_tp_attr_del,DeeType_DEFAULT_SLOT(tp_attr_del)))\
,DEE_REQUIRE_TYPECOMPATIBLE(DeeType_SLOT_TYPE(tp_attr_set),DEE_TYPEMEMBER(_tp_attr_set,DeeType_DEFAULT_SLOT(tp_attr_set)))\
,DEE_TYPEMEMBER(_tp_members,DeeType_DEFAULT_SLOT(tp_members))\
,DEE_TYPEMEMBER(_tp_getsets,DeeType_DEFAULT_SLOT(tp_getsets))\
,DEE_TYPEMEMBER(_tp_methods,DeeType_DEFAULT_SLOT(tp_methods))\
,DEE_TYPEMEMBER(_tp_class_members,DeeType_DEFAULT_SLOT(tp_class_members))\
,DEE_TYPEMEMBER(_tp_class_getsets,DeeType_DEFAULT_SLOT(tp_class_getsets))\
,DEE_TYPEMEMBER(_tp_class_methods,DeeType_DEFAULT_SLOT(tp_class_methods))}
#define DEE_TYPE_OBJECT_FOOTER_v100 DeeType_DEFAULT_SLOT(tp_marshal),DEE_TYPE_OBJECT_FOOTER_v101

#define DEE_TYPE_OBJECT_MARSHAL_v101(_tp_marshal) \
 DEE_TYPEMEMBER(_tp_marshal,DeeType_DEFAULT_SLOT(tp_marshal))
#define DEE_TYPE_OBJECT_FOOTER_v101 /* nothing (yet...) */

#endif /* DEE_LIMITED_DEX */


DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_TYPE_DECL_H */
