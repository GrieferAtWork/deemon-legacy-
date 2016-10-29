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
#ifndef GUARD_DEEMON_STRUCT_H
#define GUARD_DEEMON_STRUCT_H 1

#include <deemon/__conf.inl>
#ifdef DEE_LIMITED_API
#include <deemon/foreign_function.h>
#include <deemon/object.h>
#include <deemon/optional/string_forward.h>
#include <deemon/optional/uuid.h>
#include <deemon/structured.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif


DEE_OBJECT_DEF(DeeStructTypeObject);
DEE_OBJECT_DEF(DeeStructObject);

#ifdef DEE_LIMITED_API
struct DeeStructMember {
 DEE_A_REF DeeStructuredTypeObject *sm_type;   /*< [0..1] Member type (NULL for list end). */
 DEE_A_REF struct DeeStringObject  *sm_name;   /*< [1..1] Member name (undefined in list end). */
           Dee_size_t               sm_offset; /*< Member offset (from data base). */
};
struct DeeStructTypeObject {
 DeeStructuredTypeObject tp_type;
 struct DeeStructMember *tp_smembers; /*< [1..1][maybe(owned)] List of struct members. */
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
 ffi_type                tp_ffitype;  /*< ffi structure type. */
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
};
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
#define DeeStructType_FFITYPE(ob) (&((DeeStructTypeObject *)Dee_REQUIRES_POINTER(ob))->tp_ffitype)
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
extern struct DeeStructMember _DeeStructType_EmptyMembers[1];

struct DeeStructObject {
 DEE_OBJECT_HEAD_EX(DeeStructTypeObject)
 Dee_uint8_t st_data[1024]; /*< User structure data (offsets and actual size depends on type). */
};
#define DeeStruct_DATA(ob) ((DeeStructObject *)Dee_REQUIRES_POINTER(ob))->st_data
#endif

DEE_DATA_DECL(DeeTypeObject) DeeStructType_Type;
#define DeeStructType_Check(ob)      DeeObject_InstanceOf(ob,&DeeStructType_Type)
#define DeeStructType_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeStructType_Type)
#define DeeStruct_Check(ob)          DeeStructType_Check(Dee_TYPE(ob))

#define DeeStructType_New() ((DeeTypeObject *)DeeType_NewInstanceDefault(&DeeStructType_Type))
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeStructType_Initialized(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeStructType_AddField(
 DEE_A_INOUT_TYPEOBJECT(DeeStructTypeObject) *self, DEE_A_INOUT_OBJECT(DeeStringObject) const *name,
 DEE_A_IN Dee_size_t offset, DEE_A_INOUT DeeTypeObject *type_) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) DeeStructType_HasField(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) DeeStructType_HasFieldString(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *self,
 DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(NULL) DeeTypeObject *) DeeStructType_GetField(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *self, DEE_A_IN_OBJECT(DeeStringObject) const *name,
 DEE_A_OUT_OPT Dee_size_t *offset) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(NULL) DeeTypeObject *) DeeStructType_GetFieldString(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *self, DEE_A_IN_Z char const *name,
 DEE_A_OUT_OPT Dee_size_t *offset) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Initialize a struct with data form a given iterator
// >> DeeStruct_PInitializeFromIterator(...,dict { .foo = 42, .bar = 21 }.operator __iterself__())
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeStruct_PInitializeFromIterator(
 DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,2,3));

//////////////////////////////////////////////////////////////////////////
// Must be called after changes have been made to the struct type
// >> Updates the ffi type, thus allowing usage in foreign functions
// NOTE: Using the struct type in a foreign function before calling
//       this, or after modifying it again, causes undefined behavior.
DEE_FUNC_DECL(void) DeeStructType_Changed(
 DEE_A_INOUT_TYPEOBJECT(DeeStructTypeObject) *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Set the base/uuid of a given struct type
// WARNING: Not thread-safe
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeStructType_SetBase(
 DEE_A_INOUT_TYPEOBJECT(DeeStructTypeObject) *self,
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) *base) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeStructType_SetUUID(
 DEE_A_INOUT_TYPEOBJECT(DeeStructTypeObject) *self, DEE_A_IN struct DeeUUID const *uuid) DEE_ATTRIBUTE_NONNULL((1,2));


#ifdef DEE_LIMITED_API
#define /* int */DeeStruct_HasAttr(ob,attr) DeeStruct_THasAttr(Dee_TYPE(ob),ob,attr)
#define /* DeeObject * */DeeStruct_GetAttrString(ob,attr) DeeStruct_TGetAttrString(Dee_TYPE(ob),ob,attr)
#define /* int */DeeStruct_HasAttrString(ob,attr) DeeStruct_THasAttrString(Dee_TYPE(ob),ob,attr)
#define /* int */DeeStruct_DelAttrString(ob,attr) DeeStruct_TDelAttrString(Dee_TYPE(ob),ob,attr)
#define /* int */DeeStruct_SetAttrString(ob,attr,v) DeeStruct_TSetAttrString(Dee_TYPE(ob),ob,attr,v)
#define /* DeeObject * */DeeStruct_CallAttrString(ob,attr,args) DeeStruct_TCallAttrString(Dee_TYPE(ob),ob,attr,args)
#define /* DeeObject * */DeeStruct_CallAttr(ob,attr,args) DeeStruct_TCallAttr(Dee_TYPE(ob),ob,attr,args)
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_TGetAttr(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
#define /* int */DeeStruct_THasAttr(tp_self,self,attr) DeeStruct_PHasAttr(tp_self,attr)
extern DEE_A_RET_EXCEPT(-1) int DeeStruct_TDelAttr(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT(-1) int DeeStruct_TSetAttr(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_TGetAttrString(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
#define /* int */DeeStruct_THasAttrString(tp_self,self,attr) DeeStruct_PHasAttrString(tp_self,attr)
extern DEE_A_RET_EXCEPT(-1) int DeeStruct_TDelAttrString(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT(-1) int DeeStruct_TSetAttrString(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_Z char const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_TCallAttrString(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_Z char const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_TCallAttr(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_PGetAttr(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_NOEXCEPT(0) int DeeStruct_PHasAttr(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeStruct_PDelAttr(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT(-1) int DeeStruct_PSetAttr(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_PGetAttrString(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_NOEXCEPT(0) int DeeStruct_PHasAttrString(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeStruct_PDelAttrString(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT(-1) int DeeStruct_PSetAttrString(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_PCallAttrString(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_PCallAttr(DEE_A_IN_TYPEOBJECT(DeeStructTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3,4));

extern DEE_A_RET_EXCEPT_REF DeeObject *DeeStruct_GetAttr(DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeStruct_DelAttr(DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeStruct_SetAttr(DEE_A_INOUT_OBJECT(DeeStructObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3));
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_STRUCT_H */
