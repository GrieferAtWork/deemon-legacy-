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
#ifndef GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEETYPEDOCREF_TYPE_INL
#define GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEETYPEDOCREF_TYPE_INL 1
#ifdef __INTELLISENSE__
#include "../_docgen.c"
#endif
#include "_deedocref.inl"

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// DeeTypeDocRefObject
static DeeObject *_deetypedocref_tp_repr(DeeTypeDocRefObject *self) {
 return DeeTypeDoc_Repr(self->dr_doc);
}
static DeeObject *_deetypedocref_base_get(
 DeeTypeDocRefObject *self, void *DEE_UNUSED(closure)) {
 if (self->dr_doc->td_base) DeeReturn_NEWREF((DeeObject *)self->dr_doc->td_base);
 DeeReturn_NEWREF((DeeObject *)&_object_name);
}
static DeeObject *_deetypedocref_name_get(
 DeeTypeDocRefObject *self, void *DEE_UNUSED(closure)) {
 DeeReturn_NEWREF((DeeObject *)self->dr_doc->td_ctor.md_name);
}
static DeeObject *_deetypedocref_descr_get(
 DeeTypeDocRefObject *self, void *DEE_UNUSED(closure)) {
 if (self->dr_doc->td_descr) DeeReturn_NEWREF((DeeObject *)self->dr_doc->td_descr);
 DeeReturn_EmptyString;
}
static DeeMethodDocRefObject *_deetypedocref_constructor_get(
 DeeTypeDocRefObject *self, void *DEE_UNUSED(closure)) {
 DeeMethodDocRefObject *result;
 if ((result = DeeObject_MALLOC(DeeMethodDocRefObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeMethodDocRef_Type);
 Dee_INCREF(result->dr_keepalive = self->dr_keepalive);
 result->dr_doc = &self->dr_doc->td_ctor;
 return result;
}
static DeeDocsRefObject *_deetypedocref_attrib_get(
 DeeTypeDocRefObject *self, void *DEE_UNUSED(closure)) {
 DeeDocsRefObject *result;
 if ((result = DeeObject_MALLOC(DeeDocsRefObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeDocsRef_Type);
 Dee_INCREF(result->dr_keepalive = self->dr_keepalive);
 result->dr_doc = &self->dr_doc->td_attrib;
 return result;
}

static DeeObject *_deetypedocref_has_base(
 DeeTypeDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_base") != 0) return NULL;
 DeeReturn_Bool(self->dr_doc->td_base != NULL);
}
static DeeObject *_deetypedocref_has_descr(
 DeeTypeDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_descr") != 0) return NULL;
 DeeReturn_Bool(self->dr_doc->td_descr != NULL);
}
static DeeObject *_deetypedocref_has_constructor(
 DeeTypeDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_constructor") != 0) return NULL;
 DeeReturn_Bool(self->dr_doc->td_ctor.md_over.ods_docc != 0);
}

static struct DeeMemberDef const _deetypedocref_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("constructor",object,&DeeMethodDocRef_Type),
 DEE_MEMBERDEF_CONST_v100("attrib",object,&DeeDocsRef_Type),
 DEE_MEMBERDEF_END_v100
};
static struct DeeGetSetDef const _deetypedocref_tp_getsets[] = {
 DEE_GETSETDEF_v100("base",member(&_deetypedocref_base_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("name",member(&_deetypedocref_name_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("descr",member(&_deetypedocref_descr_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("constructor",member(&_deetypedocref_constructor_get),null,null,"-> doc.method"),
 DEE_GETSETDEF_v100("attrib",member(&_deetypedocref_attrib_get),null,null,"-> docs"),
 DEE_GETSETDEF_END_v100
};
static struct DeeMethodDef const _deetypedocref_tp_methods[] = {
 DEE_METHODDEF_v100("has_base",member(&_deetypedocref_has_base),"() -> bool"),
 DEE_METHODDEF_v100("has_descr",member(&_deetypedocref_has_descr),"() -> bool"),
 DEE_METHODDEF_v100("has_constructor",member(&_deetypedocref_has_constructor),"() -> bool"),
 DEE_METHODDEF_END_v100
};

DeeTypeObject DeeTypeDocRef_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("doc.type"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeTypeDocRefObject),null,
  null,member(&_deedocref_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deedocref_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,member(&_deetypedocref_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,member(&_deedocref_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,member(_deetypedocref_tp_getsets),member(_deetypedocref_tp_methods),
  member(_deetypedocref_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(null),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEETYPEDOCREF_TYPE_INL */
