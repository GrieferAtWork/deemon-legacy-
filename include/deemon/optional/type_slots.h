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
#ifndef GUARD_DEEMON_OPTIONAL_TYPE_SLOTS_H
#define GUARD_DEEMON_OPTIONAL_TYPE_SLOTS_H 1

#include <deemon/__conf.inl>

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
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_HASH_T
DEE_PRIVATE_DECL_DEE_HASH_T
#undef DEE_PRIVATE_DECL_DEE_HASH_T
#endif


struct DeeFileObject;
struct DeeFileTypeObject;
struct DeeGetSetDef;
struct DeeMemberDef;
struct DeeMethodDef;
struct DeeStructuredTypeObject;
struct DeeTypeMarshal;

#ifdef DEE_PRIVATE_DECL_DEE_VISIT_PROC
// v returns false (0) if the current branch should no longer be walked
//   NOTE: Implementers of this function are responsible
//         for preventing infinite loops in gc objects.
DEE_PRIVATE_DECL_DEE_VISIT_PROC
#undef DEE_PRIVATE_DECL_DEE_VISIT_PROC
#endif

#include <deemon/__typeslots_ids.inl>
#include <deemon/__typeslots_typ.inl>
#ifdef DEE_LIMITED_DEX
#include <deemon/__typeslots_def.inl>
#include <deemon/__typeslots_get.inl>
#ifdef __INTELLISENSE__
// Get some nice syntax highlighting for slot names
#include <deemon/__typeslots_stx.inl>
#endif
DEE_DECL_END
#include <deemon/__typecompatible.inl>
DEE_DECL_BEGIN
#endif


//////////////////////////////////////////////////////////////////////////
// Slot names are either the tp_* names as found in "DeeTypeObject" and its group structures
// or are the names as seen in error messages and builtin operator functions.
// e.g.:
//  >> 'tp_seq_contains' - '__contains__'
//  >> 'tp_add'          - '__add__'
//  >> ...
#define DeeType_SLOT_ID(name)       DEE_PRIVATE_PP_CAT_2(DeeType_SlotID_,name)
#define DeeType_SLOT_TYPE(name)     DEE_PRIVATE_PP_CAT_2(_DeeType_SlotType_,name)
#ifdef DEE_LIMITED_DEX
#define DeeType_DEFAULT_SLOT(name)  DEE_PRIVATE_PP_CAT_2(_DeeType_SlotDefault_,name)
#endif


//////////////////////////////////////////////////////////////////////////
// NOTE: If you're wondering how binary forward compatibility works,
//       look no further, because the following block is the perfect example!
#ifdef DEE_LIMITED_DEX
#ifdef __INTELLISENSE__
#define DeeType_GET_SLOT(ob,name)        (*(DEE_PRIVATE_PP_CAT_2(_DeeType_SlotType_,name)*)(Dee_REQUIRES_POINTER(ob),NULL))
#else
#define DeeType_GET_SLOT(ob,name)         DEE_PRIVATE_PP_CAT_2(_DeeType_SlotGet_,name)(Dee_REQUIRES_POINTER(ob))
#endif
#define DeeType_HAS_SLOT(ob,name)        (DeeType_GET_SLOT(ob,name)!=DeeType_DEFAULT_SLOT(name))
#define DeeType_DEL_SLOT(ob,name)  (void)(DeeType_GET_SLOT(ob,name)=DeeType_DEFAULT_SLOT(name))
#else /* DEE_LIMITED_DEX */
#define DeeType_GET_SLOT(ob,name)  (*(DeeType_SLOT_TYPE(name)*)DeeType_GetSlot((DeeTypeObject *)Dee_REQUIRES_POINTER(ob),DeeType_SLOT_ID(name)))
#define DeeType_HAS_SLOT(ob,name)  DeeType_HasSlot((DeeTypeObject *)Dee_REQUIRES_POINTER(ob),DeeType_SLOT_ID(name))
#define DeeType_DEL_SLOT(ob,name)  DeeType_DelSlot((DeeTypeObject *)Dee_REQUIRES_POINTER(ob),DeeType_SLOT_ID(name))
#endif /* !DEE_LIMITED_DEX */

//////////////////////////////////////////////////////////////////////////
// Generic type slot functions
// NOTE: These functions work with all type slot ids (for type, file_type, structured_type, ...)
// NOTE: When possible use macros above, as they work whenever you know
//       the slot name and are faster if limited headers are loaded
// NOTE: The 'DeeType_HasSlot' function does NOT correlate with the ability of calling
//       the function without causing a SEGFAULT. It just means that the function
//       has been overwritten in some meaningful manner, differing from its default.
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL void *) DeeType_GetSlot(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN int slot) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeType_HasSlot(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN int slot) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeType_DelSlot( // Restore default slot value
 DEE_A_IN DeeTypeObject *self, DEE_A_IN int slot) DEE_ATTRIBUTE_NONNULL((1));

DEE_FUNC_DECL(DEE_A_RET_Z_OPT char const *) DeeType_Name(
 DEE_A_IN DeeTypeObject const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_MAYBE_NULL DEE_A_RET_WUNUSED DeeTypeObject *)
 DeeType_Base(DEE_A_IN DeeTypeObject const *self) DEE_ATTRIBUTE_NONNULL((1));

#ifdef DEE_LIMITED_DEX
#define DeeType_BASE(ob) DeeType_GET_SLOT(ob,tp_base)
#define DeeType_NAME(ob) DeeType_GET_SLOT(ob,tp_name)
#else
#define DeeType_BASE(ob) DeeType_Base((DeeTypeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeType_NAME(ob) DeeType_Name((DeeTypeObject *)Dee_REQUIRES_POINTER(ob))
#endif

//////////////////////////////////////////////////////////////////////////
// Iterate with 'tp', to find the version of 'slot' that should be used
// NOTE: This is why the base class 'object' needs to support every slot!
#define DeeType_FIND_SLOT(tp,slot)\
do{\
 while(!DeeType_HAS_SLOT(tp,slot))\
  *(DeeTypeObject **)&(tp)=DeeType_BASE(tp);\
}while(0)


DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_TYPE_SLOTS_H */
