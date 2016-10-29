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
#ifndef GUARD_DEEMON_OPTIONAL_TYPE_FLAGS_H
#define GUARD_DEEMON_OPTIONAL_TYPE_FLAGS_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/type_slots.h>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif

#define DEE_TYPE_FLAG_NONE               DEE_UINT32_C(0x00000000)
#define DEE_TYPE_FLAG_VAR_OBJECT         DEE_UINT32_C(0x00000001) /*< Type has a variadic instance size (requires user_defined 'tp_new' virtual function). */
#define DEE_TYPE_FLAG_STATIC_NAME        DEE_UINT32_C(0x00000002) /*< tp_name is statically allocated (don't free upon destruction). */
#define DEE_TYPE_FLAG_STATIC_DOC         DEE_UINT32_C(0x00000004) /*< tp_doc is statically allocated (don't free upon destruction). */
#define DEE_TYPE_FLAG_HEAP_TYPE          DEE_UINT32_C(0x00000008) /*< Type is allocated on the heap. */
#define DEE_TYPE_FLAG_MUST_COPY          DEE_UINT32_C(0x00000010) /*< Type must be copied if a constant of this is pushed onto the stack at runtime. */
#define DEE_TYPE_FLAG_HAS_GC             DEE_UINT32_C(0x00000020) /*< Type uses garbage collection (requires: tp_alloc == &_DeeGC_TpAlloc; tp_free == &_DeeGC_TpFree). */
#define DEE_TYPE_FLAG_CONST_CTOR         DEE_UINT32_C(0x00000040) /*< Allow constructor calls to this type in constant expressions (set for types that can be written to the constant memory of compiled code). */
#define DEE_TYPE_FLAG_NO_SUBCLASS        DEE_UINT32_C(0x00000080) /*< This type cannot be sub-classed. */
#define DEE_TYPE_FLAG_ALWAYS_CLEAR       DEE_UINT32_C(0x00000100) /*< tp_clear is always called (even when not destructing from the gc, but still not when called from aggressive gc). */
#define DEE_TYPE_FLAG_DONT_COPY          DEE_UINT32_C(0x00000200) /*< Don't copy this type (copy/move will just reference the original type). NOTE: Manually calling the copy-constructor still works, though. */
#define DEE_TYPE_FLAG_NO_LIFETIME_EFFECT DEE_UINT32_C(0x00000400) /*< Destroying this object prematurely has no side-effects. (allows the compiler to optimize away unused assignments with this type, or destroy it prematurely) */
#define DEE_TYPE_FLAG_CLASS_TYPE         DEE_UINT32_C(0x00000800) /*< This type is a class type (requires a 'struct DeeClassSuffix' to be located at the end of the type structure). */
#define DEE_TYPE_FLAG_NO_ITERATE_EFFECT  DEE_UINT32_C(0x00001000) /*< Iterating, or not iterating this type doesn't have any side-effects (used at runtime to optimize 'x...' with an unused result value). */
#define DEE_TYPE_FLAG_INCOMPLETE         DEE_UINT32_C(0x80000000) /*< This type is incomplete. */
#ifdef DEE_LIMITED_API
#define DEE_TYPE_FLAG_FUNDAMENTAL        DEE_UINT32_C(0x20000000) /*< The type is fundamental and can be used in sandbox environments. */
#define DEE_TYPE_FLAG_REGISTERED_MARSHAL DEE_UINT32_C(0x40000000) /*< Atomic flag indicating that the type was used in a call to 'DeeMarshal_RegisterType' (even if this is set, marshal support might not be present). */
#define DEE_TYPE_FLAG_PUBLIC_MASK        DEE_UINT32_C(0x00FFFFFF) /*< Public (aka. non-internal) type flags. */
#endif


DEE_FUNC_DECL(DEE_A_RET_WUNUSED DeeType_SLOT_TYPE(tp_flags))
 DeeType_Flags(DEE_A_IN DeeTypeObject const *self) DEE_ATTRIBUTE_NONNULL((1));

#ifdef DEE_LIMITED_DEX
#define DeeType_FLAGS(ob) DeeType_GET_SLOT(ob,tp_flags)
#else
#define DeeType_FLAGS(ob) DeeType_Flags((DeeTypeObject *)Dee_REQUIRES_POINTER(ob))
#endif


#define /* int */DeeType_IsVarType(ob)   ((DeeType_FLAGS(ob)&DEE_TYPE_FLAG_VAR_OBJECT)!=0)
#define /* int */DeeType_IsHeapType(ob)  ((DeeType_FLAGS(ob)&DEE_TYPE_FLAG_HEAP_TYPE)!=0)
#define /* int */DeeType_IsGCType(ob)    ((DeeType_FLAGS(ob)&DEE_TYPE_FLAG_HAS_GC)!=0)
#define /* int */DeeType_IsFinal(ob)     ((DeeType_FLAGS(ob)&DEE_TYPE_FLAG_NO_SUBCLASS)!=0)
#define /* int */DeeType_IsClass(ob)     ((DeeType_FLAGS(ob)&DEE_TYPE_FLAG_CLASS_TYPE)!=0)
#define /* int */DeeType_IsComplete(ob)  ((DeeType_FLAGS(ob)&DEE_TYPE_FLAG_INCOMPLETE)==0)

//////////////////////////////////////////////////////////////////////////
// A super-base type is a type that doesn't have a base itself.
// Such a type must implement all operators described by 'DeeType_Type'
// and must allow them to be called with any instance of any of its
// sub-classes, yielding meaningful results (such as throwing a Not-Implemented error).
//  - Unless implemented as a no-op, the constructor of such a type must still be
//    executed during instantiation of the instance of one of its sub-classes,
//    but should not add any special members other than the common members
//    described by DEE_OBJECT_HEAD (which must be shared by all objects).
//  - Deemon's standard technically leaves the possibility of many such
//    types open, only stating that at least one such type 'DeeObject_Type'
//    must be defined by the host compiler/runtime.
//    Yet in the end, there isn't really any point to re-implementing
//    that type, mainly because 'DeeObject_Type' is already publicly accessible.
// -> Enabling 'DEE_CONFIG_ONLY_ONE_SUPER_BASE' breaks this definition
//    by ensuring that there can only be one super base (DeeObject_Type),
//    thus allowing for compiler-optimization like 'catch(object)' -> 'catch(...)',
//    as well as simplifying the implementation of 'DeeType_IsSuperBase'.
// -> If a literal interpretation of the standard is required, such as if
//    you implement your own super-base, this option must be disabled
//    before builtin deemon (aka. '-DDEE_CONFIG_ONLY_ONE_SUPER_BASE=0')
//////////////////////////////////////////////////////////////////////////
#ifndef DEE_CONFIG_ONLY_ONE_SUPER_BASE
#define DEE_CONFIG_ONLY_ONE_SUPER_BASE 1
#endif

#if DEE_CONFIG_ONLY_ONE_SUPER_BASE
# define /* int */DeeType_IsSuperBase(ob) ((ob)==&DeeObject_Type)
#else
# define /* int */DeeType_IsSuperBase(ob) (DeeType_BASE(ob)==NULL)
#endif

//////////////////////////////////////////////////////////////////////////
// Returns true if the type describes immutable objects.
//  -> That is object that cannot be modified after being constructed.
// NOTE: Despite sounding illogical, tuples are not immutable,
//       as it is possible for a tuple to contain an immutable object,
//       thus implicitly allowing for a tuple to be modified
//       by modifying one of its elements:
//       >> a = pack([]);
//       >> a[0].append(10);
//       >> print a; // prints 'pack([10])'
#define /* int */DeeType_IsImmutable(ob) \
 (((DeeType_FLAGS(ob)&(DEE_TYPE_FLAG_CONST_CTOR|DEE_TYPE_FLAG_MUST_COPY))==\
                      (DEE_TYPE_FLAG_CONST_CTOR)) || \
  DeeType_IsSameOrDerived(ob,&DeeType_Type))


DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_TYPE_FLAGS_H */
