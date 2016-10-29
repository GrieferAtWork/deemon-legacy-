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
#ifndef GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEGENERICDOCREF_TYPE_INL
#define GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEGENERICDOCREF_TYPE_INL 1
#ifdef __INTELLISENSE__
#include "../_docgen.c"
#endif
#include "_deedocref.inl"

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// DeeGenericDocRefObject
static DeeObject *_deegenericdocref_tp_repr(DeeGenericDocRefObject *self) {
 return DeeGenericDoc_Repr(self->dr_doc);
}
static DeeObject *_deegenericdocref_name_get(
 DeeGenericDocRefObject *self, void *DEE_UNUSED(closure)) {
 DeeObject *result = (DeeObject *)self->dr_doc->gd_name;
 if (result) DeeReturn_NEWREF(result);
 DeeReturn_EmptyString;
}
static DeeObject *_deegenericdocref_doc_get(
 DeeGenericDocRefObject *self, void *DEE_UNUSED(closure)) {
 DeeObject *result = (DeeObject *)self->dr_doc->gd_doc;
 if (result) DeeReturn_NEWREF(result);
 DeeReturn_EmptyString;
}

static DeeObject *_deegenericdocref_has_name(
 DeeGenericDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_name") != 0) return NULL;
 DeeReturn_Bool(self->dr_doc->gd_name != NULL);
}
static DeeObject *_deegenericdocref_has_doc(
 DeeGenericDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_doc") != 0) return NULL;
 DeeReturn_Bool(self->dr_doc->gd_doc != NULL);
}


static struct DeeGetSetDef const _deegenericdocref_tp_getsets[] = {
 DEE_GETSETDEF_v100("name",member(&_deegenericdocref_name_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("doc",member(&_deegenericdocref_doc_get),null,null,"-> string"),
 DEE_GETSETDEF_END_v100
};

static struct DeeMethodDef const _deegenericdocref_tp_methods[] = {
 DEE_METHODDEF_v100("has_name",member(&_deegenericdocref_has_name),"() -> bool"),
 DEE_METHODDEF_v100("has_doc",member(&_deegenericdocref_has_doc),"() -> bool"),
 DEE_METHODDEF_END_v100
};

DeeTypeObject DeeGenericDocRef_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("doc.generic"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeGenericDocRefObject),null,
  null,member(&_deedocref_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deedocref_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,member(&_deegenericdocref_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,member(&_deedocref_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,member(_deegenericdocref_tp_getsets),
  member(_deegenericdocref_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(null),
 DEE_TYPE_OBJECT_FOOTER_v101
};



DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEGENERICDOCREF_TYPE_INL */
