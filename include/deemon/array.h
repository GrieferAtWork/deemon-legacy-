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
#ifndef GUARD_DEEMON_ARRAY_H
#define GUARD_DEEMON_ARRAY_H 1

#include <deemon/__conf.inl>

#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
#ifdef DEE_LIMITED_DEX
#include <deemon/structured.h>
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

DEE_OBJECT_DEF(DeeVarArrayTypeObject);
DEE_OBJECT_DEF(DeeVarArrayObject);
DEE_OBJECT_DEF(DeeArrayTypeObject);
DEE_OBJECT_DEF(DeeArrayObject);
DEE_OBJECT_DEF(DeeArrayIteratorObject);

#ifdef DEE_LIMITED_DEX
struct DeeArrayTypeObject {
           DeeStructuredTypeObject  at_base;
 DEE_A_REF DeeStructuredTypeObject *at_array_base;       /*< [1..1] Array element type. */
           Dee_size_t               at_array_elem_size;  /*< Array element size (== at_array_base->tp_p_instance_size). */
           Dee_size_t               at_array_elem_count; /*< Amount of array elements. */
};
#define DeeArrayType_NELEM(ob) ((DeeArrayTypeObject *)Dee_REQUIRES_POINTER(ob))->at_array_elem_count
#define DeeArrayType_SELEM(ob) ((DeeArrayTypeObject *)Dee_REQUIRES_POINTER(ob))->at_array_elem_size
#define DeeArrayType_EMPTY(tp)  (DeeArrayType_NELEM(tp)==0)
#define DeeArrayType_BASE(ob)  ((DeeTypeObject *)((DeeArrayTypeObject *)Dee_REQUIRES_POINTER(ob))->at_array_base)

struct DeeArrayObject {
 DEE_OBJECT_HEAD_EX(DeeArrayTypeObject)
 Dee_uint8_t a_data[1024];
};
#define DeeArray_DATA                   DeeArray_ELEM
#define DeeArray_ELEM(ob)    ((void *)((DeeArrayObject *)Dee_REQUIRES_POINTER(ob))->a_data)
#define DeeArray_SIZE(ob)               DeeArrayType_NELEM(Dee_TYPE(ob))
#define DeeArray_EMPTY(ob)              DeeArrayType_EMPTY(Dee_TYPE(ob))
#define DeeArray_BYTEV                  DeeArray_DATA
#define DeeArray_BYTEC(ob)              DeeType_GET_SLOT(Dee_TYPE(ob),tp_p_instance_size)
#define DeeArray_GET(ob,i)   ((void *)((Dee_uintptr_t)DeeArray_ELEM(ob)+(i)*DeeArrayType_SELEM(Dee_TYPE(ob))))
#define DeeArray_DEL(ob,i)   ((void)memset(DeeArray_GET(ob,i),0,DeeArrayType_SELEM(Dee_TYPE(ob))))
#define DeeArray_SET(ob,i,p) ((void)memcpy(DeeArray_GET(ob,i),p,DeeArrayType_SELEM(Dee_TYPE(ob))))
#endif

#ifdef DEE_LIMITED_DEX
struct DeeArrayIteratorObject {
 DEE_OBJECT_HEAD
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DEE_A_REF DeeLValueTypeObject     *ai_elem_lv_type; /*< [1..1] Element lvalue type. */
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DEE_A_REF DeeStructuredTypeObject *ai_elem_type;    /*< [1..1] Element type. */
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 Dee_size_t                         ai_elem_size;    /*< Element size/offset to next elem ( == tp_elem_lv_type->tp_lval_lvalue_base->tp_p_instance_size). */
 /*atomic*/Dee_uint8_t             *ai_pos;          /*< [1..1] Current iterator position. */
 Dee_uint8_t                       *ai_end;          /*< [1..1] Iterator end ( == ai_pos+?*ai_elem_size). */
 Dee_uint8_t                       *ai_begin;        /*< [1..1] Iterator begin. */
};
#endif

DEE_DATA_DECL(DeeTypeObject) DeeArrayType_Type;
DEE_DATA_DECL(DeeTypeObject) DeeArrayIterator_Type;
#define DeeArrayType_CheckExact(ob)     DeeObject_InstanceOfExact(ob,&DeeArrayType_Type)
#define DeeArrayType_Check              DeeArrayType_CheckExact
#define DeeArray_Check(ob)              DeeArrayType_Check(Dee_TYPE(ob))
#define DeeArrayIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeArrayIterator_Type)
#define DeeArrayIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeArrayIterator_Type)

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeArrayIteratorObject) *) DeeArrayIterator_New(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) *elem_type,
 DEE_A_IN void *begin, DEE_A_IN Dee_size_t n_elem) DEE_ATTRIBUTE_NONNULL((1,2));

#ifdef DEE_LIMITED_API
extern DEE_A_EXEC DEE_A_RET_TYPEOBJECT_EXCEPT_REF(DeeArrayTypeObject) *_DeeArrayType_New(
 DEE_A_INOUT DeeStructuredTypeObject *base, DEE_A_IN Dee_size_t n) DEE_ATTRIBUTE_NONNULL((1));
#endif



#ifdef DEE_LIMITED_DEX
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeVarArrayTypeObject {
           DeeTypeObject            vat_base;
 union{ // [0..1] Used to keep the global linked list of structured types
  struct DeeStructuredTypeObject   *vat_prev_structured_object_type;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  struct DeePointerTypeObject      *vat_prev_pointer_type;
  struct DeeLValueTypeObject       *vat_prev_lvalule_type;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  struct DeeVarArrayTypeObject     *vat_prev_varray_type;
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define vat_prev_structured_object_type _vat_prev_data.vat_prev_structured_object_type
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define vat_prev_pointer_type           _vat_prev_data.vat_prev_pointer_type
#define vat_prev_lvalule_type           _vat_prev_data.vat_prev_lvalule_type
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#define vat_prev_varray_type            _vat_prev_data.vat_prev_varray_type
   _vat_prev_data
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
 ;
 DEE_A_REF DeeStructuredTypeObject *vat_array_base;       /*< [1..1] Array element type. */
           Dee_size_t               vat_array_elem_size;  /*< Array element size (== at_array_base->tp_p_instance_size). */
};
DEE_COMPILER_MSVC_WARNING_POP
#define DeeVarArrayType_SELEM(ob) ((DeeVarArrayTypeObject *)Dee_REQUIRES_POINTER(ob))->vat_array_elem_size
#define DeeVarArrayType_BASE(ob)  ((DeeTypeObject *)((DeeVarArrayTypeObject *)Dee_REQUIRES_POINTER(ob))->vat_array_base)

struct DeeVarArrayObject {
 DEE_OBJECT_HEAD_EX(DeeVarArrayTypeObject)
 Dee_size_t  va_size;       /*< Amount of elements. */
 Dee_uint8_t va_data[1024]; /*< Variable element data. */
};
#define DeeVarArray_BYTEC(ob) ((Dee_size_t)((DeeVarArrayObject *)Dee_REQUIRES_POINTER(ob))->va_size)
#define DeeVarArray_BYTEV(ob)     ((void *)((DeeVarArrayObject *)Dee_REQUIRES_POINTER(ob))->va_data)
#define DeeVarArray_DATA                     DeeVarArray_BYTEV
#define DeeVarArray_ELEM                     DeeVarArray_BYTEV
#define DeeVarArray_SIZE(ob)                (DeeVarArray_BYTEC(ob)/DeeVarArrayType_SELEM(Dee_TYPE(ob)))
#define DeeVarArray_EMPTY(ob)              (!DeeVarArray_BYTEC(ob))
#define DeeVarArray_GET(ob,i)     ((void *)((Dee_uintptr_t)DeeVarArray_ELEM(ob)+(i)*DeeVarArrayType_SELEM(Dee_TYPE(ob))))
#define DeeVarArray_DEL(ob,i)     ((void)memset(DeeVarArray_GET(ob,i),0,DeeVarArrayType_SELEM(Dee_TYPE(ob))))
#define DeeVarArray_SET(ob,i,p)   ((void)memcpy(DeeVarArray_GET(ob,i),p,DeeVarArrayType_SELEM(Dee_TYPE(ob))))

DEE_DATA_DECL(DeeTypeObject) DeeVarArrayType_Type;
#define DeeVarArrayType_CheckExact(ob)  DeeObject_InstanceOfExact(ob,&DeeVarArrayType_Type)
#define DeeVarArrayType_Check           DeeVarArrayType_CheckExact
#define DeeVarArray_Check(ob)           DeeVarArrayType_Check(Dee_TYPE(ob))
#ifdef DEE_LIMITED_API
extern DEE_A_RET_EXCEPT_REF DeeVarArrayTypeObject *_DeeVarArrayType_New(
 DEE_A_INOUT DeeStructuredTypeObject *base) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT_REF DeeVarArrayObject *DEE_CALL DeeVarArray_NewSized(
 DEE_A_INOUT DeeVarArrayTypeObject *tp_self, DEE_A_IN Dee_size_t n_elem) DEE_ATTRIBUTE_NONNULL((1));
#define DeeVarArray_NewEmpty(tp_self) DeeVarArray_NewSized(tp_self,0)
extern DEE_A_RET_EXCEPT(-1) int DeeVarArray_Resize(
 DEE_A_INOUT DeeVarArrayObject **self, DEE_A_IN Dee_size_t n_elem) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT_REF DeeVarArrayObject *DeeVarArray_NewFromIterator(
 DEE_A_INOUT DeeVarArrayTypeObject *tp_self, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT_REF DeeVarArrayObject *DeeVarArray_NewFromSequence(
 DEE_A_INOUT DeeVarArrayTypeObject *tp_self, DEE_A_INOUT DeeObject *sequence) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT_REF DeeVarArrayObject *DeeVarArray_NewFrom8BitString(
 DEE_A_INOUT DeeVarArrayTypeObject *tp_self, DEE_A_IN Dee_size_t len,
 DEE_A_IN_R(len) Dee_uint8_t const *data) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT_REF DeeVarArrayObject *DeeVarArray_NewFrom16BitString(
 DEE_A_INOUT DeeVarArrayTypeObject *tp_self, DEE_A_IN Dee_size_t len,
 DEE_A_IN_R(len) Dee_uint16_t const *data) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT_REF DeeVarArrayObject *DeeVarArray_NewFrom32BitString(
 DEE_A_INOUT DeeVarArrayTypeObject *tp_self, DEE_A_IN Dee_size_t len,
 DEE_A_IN_R(len) Dee_uint32_t const *data) DEE_ATTRIBUTE_NONNULL((1));
#endif /* DEE_LIMITED_API */
#endif


DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */

#endif /* !GUARD_DEEMON_ARRAY_H */
