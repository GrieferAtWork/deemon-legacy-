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
#ifndef GUARD_DEEMON_OPTIONAL_ENUM_ATTRIBUTES_H
#define GUARD_DEEMON_OPTIONAL_ENUM_ATTRIBUTES_H 1

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
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif

//////////////////////////////////////////////////////////////////////////
// Callback prototype for enumerating object attributes
// @param tp_self:   The type implementing the attribute
// @param self:      The object featuring the given attribute
// @param name:      The name of the attribute
// @param flags:     Set of 'DEE_ENUMATTRIBUTE_FLAG_*' describing permissions granted by the attribute
// @param attr_type: The type of object that would be returned by 'DeeObject_GetAttr', or NULL if unknown
// @param closure:   User-defined callback closure
typedef DEE_A_RET_EXCEPT(-1) int (DEE_CALL *DeeEnumAttrFunc)(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_Z char const *name, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN_OPT DeeTypeObject const *attr_type, void *closure) DEE_ATTRIBUTE_NONNULL((1,2,3));
#define DEE_ENUMATTRIBUTE_FLAG_NONE           DEE_UINT32_C(0x00000000)
#define DEE_ENUMATTRIBUTE_FLAG_PERMISSION_GET DEE_UINT32_C(0x00000001) /*< Attribute supports get/has queries. */
#define DEE_ENUMATTRIBUTE_FLAG_PERMISSION_DEL DEE_UINT32_C(0x00000002) /*< Attribute supports del queries. */
#define DEE_ENUMATTRIBUTE_FLAG_PERMISSION_SET DEE_UINT32_C(0x00000004) /*< Attribute supports set queries. */

//////////////////////////////////////////////////////////////////////////
// Enumerate all 
// WARNING: This function is incapable of enumerating every possible attribute,
//          as some custom protocols might not be supported, as well as
//          overwritten 'tp_attr_get' and friends operators.
// WARNING: This will only enumerate the attributes of a given instance,
//          meaning that calling something like 'DeeObject_EnumAttr((DeeObject *)&DeeList_Type)' won't enumerate 'pop'.
//          To get those attributes, you must enumerate an instance of list.
// SUPPORTED TYPE PROTOCOLS (__getattr__ operator names):
//          - DeeObject_GenericGetAttr
//          - DeeType_GetAttr
//          - _deesuper_tp_attr_get
//          - DeeInstance_GetAttr
//          - DeeClass_GetAttr
//          - DeeStructured_GetAttr
//          - DeeStruct_GetAttr
//          - _deelvalue_tp_attr_get
//          - _deedexmodule_tp_attr_get
#define DeeObject_EnumAttr(ob,enum_attr,closure) \
 DeeObject_TEnumAttr(Dee_TYPE(ob),ob,enum_attr,closure)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeObject_TEnumAttr(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeEnumAttrFunc enum_attr, void *closure) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeObject_PEnumAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN DeeEnumAttrFunc enum_attr,
 void *closure) DEE_ATTRIBUTE_NONNULL((1,2,3));

#ifdef DEE_LIMITED_API
extern DEE_A_RET_EXCEPT(-1) int DeeDexModule_EnumAttr( // in "dex.c"
 DEE_A_INOUT DeeDexModuleObject *self, DEE_A_IN DeeEnumAttrFunc enum_attr,
 void *closure) DEE_ATTRIBUTE_NONNULL((1,2));
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_ENUM_ATTRIBUTES_H */
