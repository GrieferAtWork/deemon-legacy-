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
#ifndef GUARD_DEEMON_CLASS_H
#define GUARD_DEEMON_CLASS_H 1

#include <deemon/__conf.inl>
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
#ifdef DEE_LIMITED_API
#include <deemon/type.h>
#include <deemon/object.h>
#include <deemon/optional/atomic_mutex.h>
#include <deemon/optional/hashmap.h>
#include <deemon/optional/string_forward.h>
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
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif

// NOTE: What you see in here is only part of the c-api for 
//       user defined classes. They are not done yet, but
//       will be included in a future release.

DEE_OBJECT_DEF(DeeClassObject);
DEE_OBJECT_DEF(DeeClassTypeObject);
DEE_OBJECT_DEF(DeeInstanceObject);
DEE_OBJECT_DEF(DeeInstanceMethodObject);
DEE_OBJECT_DEF(DeePropertyObject);
DEE_OBJECT_DEF(DeeClassOperatorForObject);

#ifdef DEE_LIMITED_API

struct _DeeClassTypeMethodDef {
 DEE_A_REF struct DeeStringObject *ctmd_name; /*< [0..1] Member name (NULL if end of list). */
 DEE_A_REF DeeObject              *ctmd_func; /*< [1..1] Function object to call. */
};
struct _DeeClassTypeGetSetDef {
 DEE_A_REF struct DeeStringObject *ctgd_name; /*< [0..1] GetSet name (NULL if end of list). */
 DEE_A_REF DeeObject              *ctgd_get;  /*< [0..1] Getter function. */
 DEE_A_REF DeeObject              *ctgd_del;  /*< [0..1] Delete function. */
 DEE_A_REF DeeObject              *ctgd_set;  /*< [0..1] Setter function. */
};

#if !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
struct _DeeClassDynamicVTableEntry {
           int        dvte_slot;     /*< Type slot of this entry. */
 DEE_A_REF DeeObject *dvte_callback; /*< [1..1] Callback of this entry. */
};
struct _DeeClassDynamicVTableList {
 Dee_size_t                          dvt_tablec; /*< Size of the vtable. */
 struct _DeeClassDynamicVTableEntry *dvt_tablev; /*< [0..dvt_tablec][owned] Class vtable. */
};
#define _DeeClassDynamicVTableList_Init(ob)\
do{ (ob)->dvt_tablec = 0; (ob)->dvt_tablev = NULL; }while(0)
#define _DeeClassDynamicVTableList_Visit(ob)\
do{\
 struct _DeeClassDynamicVTableEntry *_vt_iter,*_vt_end;\
 _vt_end = (_vt_iter = (ob)->dvt_tablev)+(ob)->dvt_tablec;\
 while (_vt_iter != _vt_end) Dee_VISIT((_vt_iter++)->dvte_callback);\
}while(0)
#define _DeeClassDynamicVTableList_Quit(ob)\
do{\
 struct _DeeClassDynamicVTableEntry *_vt_iter,*_vt_end;\
 _vt_end = (_vt_iter = (ob)->dvt_tablev)+(ob)->dvt_tablec;\
 while (_vt_iter != _vt_end) Dee_DECREF((_vt_iter++)->dvte_callback);\
 free((ob)->dvt_tablev);\
}while(0)
#define _DeeClassDynamicVTableList_Clear(ob)\
do{\
 _DeeClassDynamicVTableList_Quit(ob);\
 _DeeClassDynamicVTableList_Init(ob);\
}while(0)

extern DEE_A_RET_NEVER_NULL DeeObject *_DeeClassDynamicVTableList_GetKnownOperator(
 DEE_A_IN struct _DeeClassDynamicVTableList const *self, DEE_A_IN int slot_id);
extern DEE_A_RET_MAYBE_NULL DeeObject *_DeeClassDynamicVTableList_GetOperator(
 DEE_A_IN struct _DeeClassDynamicVTableList const *self, DEE_A_IN int slot_id);
extern DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeClassDynamicVTableList_DelOperator(
 DEE_A_INOUT struct _DeeClassDynamicVTableList *self, DEE_A_IN int slot_id);
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int _DeeClassDynamicVTableList_SetOperator(
 DEE_A_INOUT struct _DeeClassDynamicVTableList *self,
 DEE_A_IN int slot_id, DEE_A_IN DeeObject *callback);
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */


#if DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE
struct _DeeClassTypeConstructorOperators {
 DEE_A_REF DeeObject *ctp_ctor;      /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_copy_ctor; /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_move_ctor; /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_any_ctor;  /*< [0..1]. */
};
struct _DeeClassTypeDestructorOperators {
 DEE_A_REF DeeObject *ctp_dtor;      /*< [0..1]. */
};
struct _DeeClassTypeAssignOperators {
 DEE_A_REF DeeObject *ctp_copy_assign; /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_move_assign; /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_any_assign;  /*< [0..1]. */
};
struct _DeeClassTypeCastOperators {
 DEE_A_REF DeeObject *ctp_str;    /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_repr;   /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_int32;  /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_int64;  /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_double; /*< [0..1]. */
};
struct _DeeClassTypeObjectOperators {
 DEE_A_REF DeeObject *ctp_call; /*< [0..1]. */
};
struct _DeeClassTypeMathOperators {
 DEE_A_REF DeeObject *ctp_not;     /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_bool;    /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_inv;     /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_pos;     /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_neg;     /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_inc;     /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_incpost; /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_dec;     /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_decpost; /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_add;     /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_iadd;    /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_sub;     /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_isub;    /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_mul;     /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_imul;    /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_div;     /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_idiv;    /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_mod;     /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_imod;    /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_shl;     /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_ishl;    /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_shr;     /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_ishr;    /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_and;     /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_iand;    /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_or;      /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_ior;     /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_xor;     /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_ixor;    /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_pow;     /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_ipow;    /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_hash;    /*< [0..1]. */
};
struct _DeeClassTypeCompareOperators {
 DEE_A_REF DeeObject *ctp_cmp_lo; /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_cmp_le; /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_cmp_eq; /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_cmp_ne; /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_cmp_gr; /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_cmp_ge; /*< [0..1]. */
};
struct _DeeClassTypeSeqOperators {
 DEE_A_REF DeeObject *ctp_seq_get;       /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_seq_del;       /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_seq_set;       /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_seq_size;      /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_seq_contains;  /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_seq_range_get; /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_seq_range_del; /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_seq_range_set; /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_seq_iter_self; /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_seq_iter_next; /*< [0..1]. */
};
struct _DeeClassTypeAttributeOperators {
 // NOTE: Unlike internal overrides for these operators,
 //       these are only used if the attribute can't be found elsewhere!
 DEE_A_REF DeeObject *ctp_attr_get; /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_attr_del; /*< [0..1]. */
 DEE_A_REF DeeObject *ctp_attr_set; /*< [0..1]. */
};
struct _DeeClassTypeFileOperators {
#ifdef __INTELLISENSE__
 DeeObject *ctp_io_read;  /*< [0..1] Read operator. */
 DeeObject *ctp_io_write; /*< [0..1] Write operator. */
 DeeObject *ctp_io_seek;  /*< [0..1] Seek operator. */
 DeeObject *ctp_io_flush; /*< [0..1] Flush operator. */
 DeeObject *ctp_io_trunc; /*< [0..1] Trunc operator. */
 DeeObject *ctp_io_close; /*< [0..1] Close operator. */
#else
 DEE_A_REF DeeObject *ctp_io_read;  /*< [0..1] Read operator. */
 DEE_A_REF DeeObject *ctp_io_write; /*< [0..1] Write operator. */
 DEE_A_REF DeeObject *ctp_io_seek;  /*< [0..1] Seek operator. */
 DEE_A_REF DeeObject *ctp_io_flush; /*< [0..1] Flush operator. */
 DEE_A_REF DeeObject *ctp_io_trunc; /*< [0..1] Trunc operator. */
 DEE_A_REF DeeObject *ctp_io_close; /*< [0..1] Close operator. */
#endif
};
#define _DeeClassTypeFileOperators_ALL(ob,xcall)\
do{\
 xcall((ob)->ctp_io_read);\
 xcall((ob)->ctp_io_write);\
 xcall((ob)->ctp_io_seek);\
 xcall((ob)->ctp_io_flush);\
 xcall((ob)->ctp_io_trunc);\
 xcall((ob)->ctp_io_close);\
}while(0)

struct DeeClassSuffix {
 // v Offset from the base address of an instance to the DeeInstanceLevel.
 Dee_size_t                               cs_member_offset;
 struct _DeeClassTypeMethodDef           *cs_methods;       /*< [1..1][maybe(owned)] List of methods. */
 struct _DeeClassTypeGetSetDef           *cs_getsets;       /*< [1..1][maybe(owned)] List of getsets. */
 struct _DeeClassTypeMethodDef           *cs_class_methods; /*< [1..1][maybe(owned)] List of class methods. */
 struct _DeeClassTypeGetSetDef           *cs_class_getsets; /*< [1..1][maybe(owned)] List of class getsets. */
 struct _DeeClassTypeConstructorOperators cs_constructor;
 struct _DeeClassTypeDestructorOperators  cs_destructor;
 struct _DeeClassTypeAssignOperators      cs_assign;
 struct _DeeClassTypeCastOperators        cs_cast;
 struct _DeeClassTypeObjectOperators      cs_object;
 struct _DeeClassTypeMathOperators        cs_math;
 struct _DeeClassTypeCompareOperators     cs_compare;
 struct _DeeClassTypeSeqOperators         cs_seq;
 struct _DeeClassTypeAttributeOperators   cs_attr;
};
struct DeeFileClassSuffix {
 struct DeeClassSuffix             tp_base;
 struct _DeeClassTypeFileOperators tp_file;
};
#define DeeAnyClassSuffix_Init(classbase_type,sfx)\
do{\
 if ((classbase_type) == &DeeFileType_Type) {\
  memset(sfx,0,sizeof(struct DeeFileClassSuffix));\
 } else {\
  memset(sfx,0,sizeof(struct DeeClassSuffix));\
 }\
}while(0)

#define _DeeGenericClassSuffix_ALL(sfx,xcall)\
do{\
 xcall((sfx)->cs_constructor.ctp_ctor);\
 xcall((sfx)->cs_constructor.ctp_copy_ctor);\
 xcall((sfx)->cs_constructor.ctp_move_ctor);\
 xcall((sfx)->cs_constructor.ctp_any_ctor);\
 xcall((sfx)->cs_destructor.ctp_dtor);\
 xcall((sfx)->cs_assign.ctp_copy_assign);\
 xcall((sfx)->cs_assign.ctp_move_assign);\
 xcall((sfx)->cs_assign.ctp_any_assign);\
 xcall((sfx)->cs_cast.ctp_str);\
 xcall((sfx)->cs_cast.ctp_repr);\
 xcall((sfx)->cs_cast.ctp_int32);\
 xcall((sfx)->cs_cast.ctp_int64);\
 xcall((sfx)->cs_cast.ctp_double);\
 xcall((sfx)->cs_object.ctp_call);\
 xcall((sfx)->cs_math.ctp_not);\
 xcall((sfx)->cs_math.ctp_bool);\
 xcall((sfx)->cs_math.ctp_inv);\
 xcall((sfx)->cs_math.ctp_pos);\
 xcall((sfx)->cs_math.ctp_neg);\
 xcall((sfx)->cs_math.ctp_inc);\
 xcall((sfx)->cs_math.ctp_incpost);\
 xcall((sfx)->cs_math.ctp_dec);\
 xcall((sfx)->cs_math.ctp_decpost);\
 xcall((sfx)->cs_math.ctp_add);\
 xcall((sfx)->cs_math.ctp_iadd);\
 xcall((sfx)->cs_math.ctp_sub);\
 xcall((sfx)->cs_math.ctp_isub);\
 xcall((sfx)->cs_math.ctp_mul);\
 xcall((sfx)->cs_math.ctp_imul);\
 xcall((sfx)->cs_math.ctp_div);\
 xcall((sfx)->cs_math.ctp_idiv);\
 xcall((sfx)->cs_math.ctp_mod);\
 xcall((sfx)->cs_math.ctp_imod);\
 xcall((sfx)->cs_math.ctp_shl);\
 xcall((sfx)->cs_math.ctp_ishl);\
 xcall((sfx)->cs_math.ctp_shr);\
 xcall((sfx)->cs_math.ctp_ishr);\
 xcall((sfx)->cs_math.ctp_and);\
 xcall((sfx)->cs_math.ctp_iand);\
 xcall((sfx)->cs_math.ctp_or);\
 xcall((sfx)->cs_math.ctp_ior);\
 xcall((sfx)->cs_math.ctp_xor);\
 xcall((sfx)->cs_math.ctp_ixor);\
 xcall((sfx)->cs_math.ctp_pow);\
 xcall((sfx)->cs_math.ctp_ipow);\
 xcall((sfx)->cs_math.ctp_hash);\
 xcall((sfx)->cs_compare.ctp_cmp_lo);\
 xcall((sfx)->cs_compare.ctp_cmp_le);\
 xcall((sfx)->cs_compare.ctp_cmp_eq);\
 xcall((sfx)->cs_compare.ctp_cmp_ne);\
 xcall((sfx)->cs_compare.ctp_cmp_gr);\
 xcall((sfx)->cs_compare.ctp_cmp_ge);\
 xcall((sfx)->cs_seq.ctp_seq_get);\
 xcall((sfx)->cs_seq.ctp_seq_del);\
 xcall((sfx)->cs_seq.ctp_seq_set);\
 xcall((sfx)->cs_seq.ctp_seq_size);\
 xcall((sfx)->cs_seq.ctp_seq_contains);\
 xcall((sfx)->cs_seq.ctp_seq_range_get);\
 xcall((sfx)->cs_seq.ctp_seq_range_del);\
 xcall((sfx)->cs_seq.ctp_seq_range_set);\
 xcall((sfx)->cs_seq.ctp_seq_iter_self);\
 xcall((sfx)->cs_seq.ctp_seq_iter_next);\
 xcall((sfx)->cs_attr.ctp_attr_get);\
 xcall((sfx)->cs_attr.ctp_attr_del);\
 xcall((sfx)->cs_attr.ctp_attr_set);\
}while(0)
#define _DeeGenericClassSuffix_Quit(sfx)  _DeeGenericClassSuffix_ALL(sfx,Dee_XDECREF)
#define _DeeGenericClassSuffix_Visit(sfx) _DeeGenericClassSuffix_ALL(sfx,Dee_XVISIT)
#define _DeeGenericClassSuffix_Clear(sfx) _DeeGenericClassSuffix_ALL(sfx,Dee_XCLEAR)

#define DeeAnyClassSuffix_Quit(classbase_type,sfx)\
do{\
 _DeeGenericClassSuffix_Quit(sfx);\
 if (DeeType_IsDerived((DeeTypeObject *)(classbase_type),&DeeFileType_Type)) {\
  _DeeClassTypeFileOperators_ALL(&((struct DeeFileClassSuffix *)(sfx))->tp_file,Dee_XDECREF);\
 }\
}while(0)
#define DeeAnyClassSuffix_Visit(classbase_type,sfx)\
do{\
 _DeeGenericClassSuffix_Visit(sfx);\
 if (DeeType_IsDerived((DeeTypeObject *)(classbase_type),&DeeFileType_Type)) {\
  _DeeClassTypeFileOperators_ALL(&((struct DeeFileClassSuffix *)(sfx))->tp_file,Dee_XVISIT);\
 }\
}while(0)

#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */

#define DeeFileClassSuffix  DeeClassSuffix
struct DeeClassSuffix {
 // v Offset from the base address of an instance to the DeeInstanceLevel.
 Dee_size_t                          cs_member_offset;
 struct _DeeClassDynamicVTableList   cs_vtable;
 struct _DeeClassTypeMethodDef      *cs_methods;       /*< [1..1][maybe(owned)] List of methods. */
 struct _DeeClassTypeGetSetDef      *cs_getsets;       /*< [1..1][maybe(owned)] List of getsets. */
 struct _DeeClassTypeMethodDef      *cs_class_methods; /*< [1..1][maybe(owned)] List of class methods. */
 struct _DeeClassTypeGetSetDef      *cs_class_getsets; /*< [1..1][maybe(owned)] List of class getsets. */
};

#define DeeAnyClassSuffix_Init(classbase_type,sfx)\
do{ memset(sfx,0,sizeof(struct DeeClassSuffix)); }while(0)
#define DeeAnyClassSuffix_Quit(classbase_type,sfx)\
do{ _DeeClassDynamicVTableList_Quit(&(sfx)->cs_vtable); }while(0)
#define DeeAnyClassSuffix_Visit(classbase_type,sfx)\
do{ _DeeClassDynamicVTableList_Visit(&(sfx)->cs_vtable); }while(0)


#define /* DEE_A_RET_NEVER_NULL */DeeClassSuffix_VTABLE(sfx) (&(sfx)->cs_vtable)
#define /* DEE_A_RET_NEVER_NULL */DeeClassSuffix_GetKnownVirtOperator(ob,slot_id)\
 _DeeClassDynamicVTableList_GetKnownOperator(DeeClassSuffix_VTABLE(ob),slot_id)
#define /* DEE_A_RET_MAYBE_NULL */DeeClassSuffix_GetVirtOperator(ob,slot_id)\
 _DeeClassDynamicVTableList_GetOperator(DeeClassSuffix_VTABLE(ob),slot_id)
#define /* DEE_A_RET_EXCEPT_FAIL(-1,1) */DeeClassSuffix_DelVirtOperator(ob,slot_id)\
 _DeeClassDynamicVTableList_DelOperator(DeeClassSuffix_VTABLE(ob),slot_id)
#define /* DEE_A_RET_EXCEPT(-1) */DeeClassSuffix_SetVirtOperator(ob,slot_id,callback)\
 _DeeClassDynamicVTableList_SetOperator(DeeClassSuffix_VTABLE(ob),slot_id,callback)
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_STATIC_VTABLE */

struct DeeClassTypeObject {
 // This is just the control class that sets tp_instance_size to fit  __cs_suffix
 DeeTypeObject ct_type;
};

// NOTE: Following the regular naming conventions,
//       this type should have been called DeeInstanceTypeObject
struct DeeClassObject {
 // NOTE: The tp_marshal protocol (if non-null) is owned
 DeeTypeObject cs_type; /*< Shared by all classes. */
#ifdef DEE_DEBUG
 // Location of the class suffix in regular types (e.g.: directly derived from 'type object')
 // Note, that this location is invalid for special types, like file-types and is only meant
 // to ease debugging by showing the suffix for the most common type of type.
 struct DeeClassSuffix __cs_suffix;
#endif
};
#define DeeClass_SUFFIX(ob)\
 ((struct DeeClassSuffix *)((Dee_uintptr_t)(ob)+(\
 DeeType_GET_SLOT(DeeType_BASE(Dee_TYPE(ob)),tp_instance_size))))
#define DeeClass_TSUFFIX(tp_self,ob)\
 ((struct DeeClassSuffix *)((Dee_uintptr_t)(ob)+(\
 DeeType_GET_SLOT(DeeType_BASE(tp_self),tp_instance_size))))
#endif

#ifdef DEE_LIMITED_API
struct DeeInstanceLevel {
 struct DeeAtomicMutex il_lock;    /*< Synchronization for this instance level. */
#define DEE_INSTANCELEVEL_FLAG_NONE   DEE_UINT32_C(0x00000000)
#define DEE_INSTANCELEVEL_FLAG_NODTOR DEE_UINT32_C(0x00000001) /*< Don't call the user destructor (required for a really complicated cornercase...) */
 Dee_uint32_t          il_flags;   /*< Instance level flags. */
 struct DeeHashMap     il_members; /*< Member variables of this instance level. */
};
#define _DeeInstanceLevel_Init(ob)\
(DeeAtomicMutex_Init(&(ob)->il_lock),\
 (ob)->il_flags = DEE_INSTANCELEVEL_FLAG_NONE)

struct DeeInstanceObject {
 DEE_OBJECT_HEAD_EX(DeeClassObject)
#ifdef __INTELLISENSE__
  ;
#endif
#ifdef DEE_DEBUG
 // This is where the members are located for classes
 // derived  from 'object' (something most of them are)
 struct DeeInstanceLevel __i_members_default[64];
#endif
};
// Returns the active member table for a given class type and instance object
#define DeeInstance_MEMBERS_AT(ob,offset) \
 ((struct DeeInstanceLevel *)((Dee_uintptr_t)Dee_REQUIRES_POINTER(ob)+(offset)))
#define DeeInstance_MEMBERS_EX(tp,ob) \
 DeeInstance_MEMBERS_AT(ob,DeeClass_SUFFIX(tp)->cs_member_offset)
// Returns the member table for an instance object
#define DeeInstance_MEMBERS(ob) DeeInstance_MEMBERS_EX(Dee_TYPE(ob),ob)
#endif

#ifdef DEE_LIMITED_API
struct DeePropertyObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeObject *pr_get; /*< [0..1] Getter callback. */
 DEE_A_REF DeeObject *pr_del; /*< [0..1] Delete callback. */
 DEE_A_REF DeeObject *pr_set; /*< [0..1] Setter callback. */
};
#define DeeProperty_GETTER(ob) ((DeePropertyObject *)Dee_REQUIRES_POINTER(ob))->pr_get
#define DeeProperty_DELETE(ob) ((DeePropertyObject *)Dee_REQUIRES_POINTER(ob))->pr_del
#define DeeProperty_SETTER(ob) ((DeePropertyObject *)Dee_REQUIRES_POINTER(ob))->pr_set
#else
#define DeeProperty_GETTER(ob) DeeProperty_Getter((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeProperty_DELETE(ob) DeeProperty_Delete((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeProperty_SETTER(ob) DeeProperty_Setter((DeeObject *)Dee_REQUIRES_POINTER(ob))
#endif

#ifdef DEE_LIMITED_API
struct DeeInstanceMethodObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeObject *im_this; /*< [1..1] The this argument used during the function call (likely a DeeInstanceObject). */
 DEE_A_REF DeeObject *im_func; /*< [1..1] The function that will be called (likely a DeeFunctionObject). */
};
#define DeeInstanceMethod_THIS(ob) ((DeeInstanceMethodObject *)Dee_REQUIRES_POINTER(ob))->im_this
#define DeeInstanceMethod_FUNC(ob) ((DeeInstanceMethodObject *)Dee_REQUIRES_POINTER(ob))->im_func
#define /*DEE_A_EXEC*/ DeeInstanceMethod_CALL(ob,args)\
 DeeObject_ThisCall(DeeInstanceMethod_FUNC(ob),DeeInstanceMethod_THIS(ob),args)
#else
#define /*DEE_A_EXEC*/ DeeInstanceMethod_CALL(ob,args) \
 DeeInstanceMethod_Call((DeeObject *)Dee_REQUIRES_POINTER(ob),args)
#endif



//////////////////////////////////////////////////////////////////////////
// - How all the class stuff works:
// >> class cls {
// >>   function foo() {}
// >>   property my_prop = { . -> 42; }
// >> };
// >> inst = cls();
// >> assert DeeClassType_Check(type cls);
// >> assert DeeClass_Check(cls);
// >> assert DeeInstance_Check(inst);
// >> assert DeeInstanceMethod_Check(inst.foo);
// >> assert DeeFunction_Check(cls.foo);
// >> assert DeeProperty_Check(cls.my_prop);
// >> assert DeeFunction_Check(cls.my_prop.get);
// >> assert DeeInt32_Check(inst.my_prop);
// NOTE: Logically speaking, 'DeeInstanceType_Check'
//       would have been a better name for 'DeeClass_Check'.
DEE_DATA_DECL(DeeTypeObject) DeeInstanceMethod_Type;
DEE_DATA_DECL(DeeTypeObject) DeeProperty_Type;
#define DeeClassType_Check(ob)         ((DeeType_FLAGS(ob)&DEE_TYPE_FLAG_CLASS_TYPE)!=0)
#define DeeFileClassType_Check(ob)      (DeeType_IsDerived((DeeTypeObject *)(ob),&DeeFileType_Type) && DeeClassType_Check(ob))
#define DeeClass_Check(ob)               DeeClassType_Check(Dee_TYPE(ob))
#define DeeFileClass_Check(ob)           DeeFileClassType_Check(Dee_TYPE(ob))
#define DeeInstance_Check(ob)            DeeClass_Check(Dee_TYPE(ob))
#define DeeFileInstance_Check(ob)        DeeFileClass_Check(Dee_TYPE(ob))
#define DeeInstanceMethod_Check(ob)      DeeObject_InstanceOf(ob,&DeeInstanceMethod_Type)
#define DeeInstanceMethod_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeInstanceMethod_Type)
#define DeeProperty_Check(ob)            DeeObject_InstanceOf(ob,&DeeProperty_Type)
#define DeeProperty_CheckExact(ob)       DeeObject_InstanceOfExact(ob,&DeeProperty_Type)


//////////////////////////////////////////////////////////////////////////
// Instance-Method proxy object
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeInstanceMethodObject) *) DeeInstanceMethod_New(
 DEE_A_INOUT DeeObject *this_arg, DEE_A_INOUT DeeObject *func) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeInstanceMethod_Call(
 DEE_A_INOUT_OBJECT(DeeInstanceMethodObject) *self, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(Dee_size_t) DeeInstanceMethod_ClearPool(void);

//////////////////////////////////////////////////////////////////////////
// Property proxy object
//  - Access to the callbacks stored in a instance-property of a class object
// >> class foo { property my_prop = { get -> this.bar; } };
// >> a = foo();
// >> a.bar = 42;
// >> print foo.my_prop.get(a);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeePropertyObject) *) DeeProperty_New(
 DEE_A_IN_OPT DeeObject *getter_, DEE_A_IN_OPT DeeObject *delete_, DEE_A_IN_OPT DeeObject *setter_);
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_MAYBE_NULL DEE_A_RET_WUNUSED DeeObject *)
 DeeProperty_Getter(DEE_A_IN_OBJECT(DeePropertyObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_MAYBE_NULL DEE_A_RET_WUNUSED DeeObject *)
 DeeProperty_Delete(DEE_A_IN_OBJECT(DeePropertyObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_MAYBE_NULL DEE_A_RET_WUNUSED DeeObject *)
 DeeProperty_Setter(DEE_A_IN_OBJECT(DeePropertyObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

#ifdef DEE_LIMITED_API
extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeInstance_TGetAttr(DEE_A_IN_TYPEOBJECT(DeeClassObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern            DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeInstance_THasAttr(DEE_A_IN_TYPEOBJECT(DeeClassObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeInstance_TDelAttr(DEE_A_IN_TYPEOBJECT(DeeClassObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeInstance_TSetAttr(DEE_A_IN_TYPEOBJECT(DeeClassObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeInstance_TCallAttr(DEE_A_IN_TYPEOBJECT(DeeClassObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeInstance_TGetAttrString(DEE_A_IN_TYPEOBJECT(DeeClassObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern            DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeInstance_THasAttrString(DEE_A_IN_TYPEOBJECT(DeeClassObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeInstance_TDelAttrString(DEE_A_IN_TYPEOBJECT(DeeClassObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeInstance_TSetAttrString(DEE_A_IN_TYPEOBJECT(DeeClassObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_Z char const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeInstance_TCallAttrString(DEE_A_IN_TYPEOBJECT(DeeClassObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_Z char const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeInstance_GetAttr(DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr);
DEE_STATIC_INLINE(       DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeInstance_HasAttr(DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) { DEE_ASSERT(DeeObject_Check(self)); return DeeInstance_THasAttr(Dee_TYPE(self),self,attr); }
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeInstance_DelAttr(DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr);
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeInstance_SetAttr(DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v);
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeInstance_CallAttr(DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) { DEE_ASSERT(DeeObject_Check(self)); return DeeInstance_TCallAttr(Dee_TYPE(self),self,attr,args); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeInstance_GetAttrString(DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_Z char const *attr) { DEE_ASSERT(DeeObject_Check(self)); return DeeInstance_TGetAttrString(Dee_TYPE(self),self,attr); }
DEE_STATIC_INLINE(           DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeInstance_HasAttrString(DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_Z char const *attr) { DEE_ASSERT(DeeObject_Check(self)); return DeeInstance_THasAttrString(Dee_TYPE(self),self,attr); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeInstance_DelAttrString(DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_Z char const *attr) { DEE_ASSERT(DeeObject_Check(self)); return DeeInstance_TDelAttrString(Dee_TYPE(self),self,attr); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeInstance_SetAttrString(DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_Z char const *attr, DEE_A_INOUT DeeObject *v) { DEE_ASSERT(DeeObject_Check(self)); return DeeInstance_TSetAttrString(Dee_TYPE(self),self,attr,v); }
DEE_STATIC_INLINE(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeInstance_CallAttrString(DEE_A_INOUT_OBJECT(DeeInstanceObject) *self, DEE_A_IN_Z char const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) { DEE_ASSERT(DeeObject_Check(self)); return DeeInstance_TCallAttrString(Dee_TYPE(self),self,attr,args); }
#endif

struct DeeUUID;

//////////////////////////////////////////////////////////////////////////
// Creates a new class type with a given base type and name
// >> Callbacks for operators can then be set using 'DeeClass_SetSlot'
#define DeeClass_New(base,name)       DeeClass_NewEx(base,name,NULL)
#define DeeClass_NewObject(base,name) DeeClass_NewObjectEx(base,name,NULL)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_TYPEOBJECT_EXCEPT_REF(DeeClassObject) *) DeeClass_NewEx(
 DEE_A_IN DeeTypeObject const *base, DEE_A_IN_Z_OPT char const *name,
 DEE_A_IN_OPT struct DeeUUID const *uuid) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_TYPEOBJECT_EXCEPT_REF(DeeClassObject) *) DeeClass_NewObjectEx(
 DEE_A_IN DeeTypeObject const *base, DEE_A_IN_OBJECT_OPT(DeeAnyStringObject) const *name,
 DEE_A_IN_OPT struct DeeUUID const *uuid) DEE_ATTRIBUTE_NONNULL((1));


#ifdef DEE_LIMITED_API
//////////////////////////////////////////////////////////////////////////
// Returns the classtype ('DeeClassType_Check(return)') for a given type
// This returned type is used as the control type to correctly create/destroy classes
// The following describes what exactly the returned type is (while also causing a reference leak...)
// >> DeeTypeObject *my_class = DeeClass_New(base,"my_class");
// >> DEE_ASSERT(Dee_TYPE(my_class) == DeeClassType_TypeOf(base));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeClassTypeObject *DeeClassType_TypeOf(
 DEE_A_IN DeeTypeObject const *base) DEE_ATTRIBUTE_NONNULL((1));
// May only be called from the 'Dee_Finalize()'
extern void _DeeClassType_Finalize(void);
#endif

#ifdef DEE_LIMITED_API
extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeClass_GetAttr(DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeClass_HasAttr(DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeClass_DelAttr(DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeClass_SetAttr(DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeClass_CallAttr(DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeClass_GetAttrString(DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeClass_HasAttrString(DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeClass_DelAttrString(DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeClass_SetAttrString(DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_Z char const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *DeeClass_CallAttrString(DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_Z char const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3));
#endif

//////////////////////////////////////////////////////////////////////////
// WARNING: Modification of class slots, functions and getsets is not thread-safe
// - The runtime solves this problem by not revealing these functions or anything
//   similar to them to the user, while also enforcing classes to only be modified
//   when they're still being constructed while remaining hidden in the stack.
// NOTE: 'slot' refers to a type slot (aka. 'DeeType_SLOT_ID(...)')
// HINT: The simulated 'operator for' can be addressed with 'DEE_CLASS_SLOTID_OPERATOR_FOR'
// HINT: The non-pointer 'operator __writenp__(string s) -> size_t' can be addressed with 'DEE_CLASS_SLOTID_OPERATOR_WRITENP'
// HINT: The non-pointer 'operator __readnp__(size_s s) -> string'  can be addressed with 'DEE_CLASS_SLOTID_OPERATOR_READNP'
// HINT: The pointer 'operator __writep__(none *p, size_t s) -> size_t' can be addressed with 'DeeType_SLOT_ID(tp_io_read)'
// HINT: The pointer 'operator __readp__(none *p, size_t s) -> size_t'  can be addressed with 'DeeType_SLOT_ID(tp_io_write)'
#define DEE_CLASS_SLOTID_OPERATOR_FOR      DeeType_SLOT_ID(tp_p_seq_iter_self)
#define DEE_CLASS_SLOTID_OPERATOR_READNP   DeeType_SLOT_ID(tp_p_shr) /*< No special reason for why I chose these. But since classes can't */
#define DEE_CLASS_SLOTID_OPERATOR_WRITENP  DeeType_SLOT_ID(tp_p_shl) /*< override structured slots, I might as well use them for this. */

#define DeeClass_HasSlot(ob,slot) (DeeClass_GetSlot(ob,slot)!=NULL)
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_NOEXCEPT(NULL) DeeObject *) DeeClass_GetSlot(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self, DEE_A_IN int slot) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeClass_DelSlot(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN int slot) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeClass_SetSlot(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN int slot,
 DEE_A_INOUT DeeObject *callback) DEE_ATTRIBUTE_NONNULL((1,3));

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeClass_AddMethod(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *name,
 DEE_A_INOUT DeeObject *method) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeClass_AddProperty(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *name,
 DEE_A_INOUT_OPT DeeObject *getter_, DEE_A_INOUT_OPT DeeObject *delete_, DEE_A_INOUT_OPT DeeObject *setter_) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeClass_AddClassMethod(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *name,
 DEE_A_INOUT DeeObject *method) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeClass_AddClassProperty(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *name,
 DEE_A_INOUT_OPT DeeObject *getter_, DEE_A_INOUT_OPT DeeObject *delete_, DEE_A_INOUT_OPT DeeObject *setter_) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) DeeClass_HasMethod(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) DeeClass_HasProperty(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) DeeClass_HasClassMethod(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) DeeClass_HasClassProperty(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(NULL) DeeObject *) DeeClass_GetMethod(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) DeeClass_GetProperty(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self, DEE_A_IN_OBJECT(DeeStringObject) const *name,
 DEE_A_OUT_OPT DeeObject **getter_, DEE_A_OUT_OPT DeeObject **delete_,
 DEE_A_OUT_OPT DeeObject **setter_) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(NULL) DeeObject *) DeeClass_GetClassMethod(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) DeeClass_GetClassProperty(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self, DEE_A_IN_OBJECT(DeeStringObject) const *name,
 DEE_A_OUT_OPT DeeObject **getter_, DEE_A_OUT_OPT DeeObject **delete_,
 DEE_A_OUT_OPT DeeObject **setter_) DEE_ATTRIBUTE_NONNULL((1,2));

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeClass_AddMethodString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_Z char const *name,
 DEE_A_INOUT DeeObject *method) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeClass_AddPropertyString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_Z char const *name,
 DEE_A_INOUT_OPT DeeObject *getter_, DEE_A_INOUT_OPT DeeObject *delete_,
 DEE_A_INOUT_OPT DeeObject *setter_) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeClass_AddClassMethodString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_Z char const *name,
 DEE_A_INOUT DeeObject *method) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeClass_AddClassPropertyString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, DEE_A_IN_Z char const *name,
 DEE_A_INOUT_OPT DeeObject *getter_, DEE_A_INOUT_OPT DeeObject *delete_,
 DEE_A_INOUT_OPT DeeObject *setter_) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) DeeClass_HasMethodString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) DeeClass_HasPropertyString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) DeeClass_HasClassMethodString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) DeeClass_HasClassPropertyString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(NULL) DeeObject *) DeeClass_GetMethodString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) DeeClass_GetPropertyString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self, DEE_A_IN_Z char const *name,
 DEE_A_OUT_OPT DeeObject **getter_, DEE_A_OUT_OPT DeeObject **delete_,
 DEE_A_OUT_OPT DeeObject **setter_) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(NULL) DeeObject *) DeeClass_GetClassMethodString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self,
 DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) DeeClass_GetClassPropertyString(
 DEE_A_INOUT_TYPEOBJECT(DeeClassObject) const *self, DEE_A_IN_Z char const *name,
 DEE_A_OUT_OPT DeeObject **getter_, DEE_A_OUT_OPT DeeObject **delete_,
 DEE_A_OUT_OPT DeeObject **setter_) DEE_ATTRIBUTE_NONNULL((1,2));

// Deprecated names
#if DEE_DEPRECATED_API_VERSION(101,102,104)
#define DeeClass_GetGetSet            DeeClass_GetProperty
#define DeeClass_GetClassGetSet       DeeClass_GetClassProperty
#define DeeClass_GetGetSetString      DeeClass_GetPropertyString
#define DeeClass_GetClassGetSetString DeeClass_GetClassPropertyString
#define DeeClass_AddGetSet            DeeClass_AddProperty
#define DeeClass_AddClassGetSet       DeeClass_AddClassProperty
#define DeeClass_AddGetSetString      DeeClass_AddPropertyString
#define DeeClass_AddClassGetSetString DeeClass_AddClassPropertyString
#define DeeClass_HasGetSet            DeeClass_HasProperty
#define DeeClass_HasClassGetSet       DeeClass_HasClassProperty
#define DeeClass_HasGetSetString      DeeClass_HasPropertyString
#define DeeClass_HasClassGetSetString DeeClass_HasClassPropertyString
#endif


DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES */

#endif /* !GUARD_DEEMON_CLASS_H */
