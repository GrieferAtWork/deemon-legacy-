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
#ifndef GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEOVERLOADDOCREF_TYPE_INL
#define GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEOVERLOADDOCREF_TYPE_INL 1
#ifdef __INTELLISENSE__
#include "../_docgen.c"
#endif
#include "_deedocref.inl"

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// DeeOverloadDocRefObject
static DeeObject *_deeoverloaddocref_tp_repr(DeeOverloadDocRefObject *self) {
 return DeeOverloadDoc_Repr(self->dr_doc,DEE_METHODDOC_FLAG_NONE,NULL,NULL);
}
static DeeObject *_deeoverloaddocref_doc_get(
 DeeOverloadDocRefObject *self, void *DEE_UNUSED(closure)) {
 if (self->dr_doc->od_doc) DeeReturn_NEWREF((DeeObject *)self->dr_doc->od_doc);
 DeeReturn_EmptyString;
}
static DeeParamDocRefObject *_deeoverloaddocref_return_get(
 DeeOverloadDocRefObject *self, void *DEE_UNUSED(closure)) {
 DeeParamDocRefObject *result;
 if ((result = DeeObject_MALLOC(DeeParamDocRefObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeParamDocRef_Type);
 Dee_INCREF(result->dr_keepalive = self->dr_keepalive);
 result->dr_doc = &self->dr_doc->od_return;
 return result;
}
static DeeParamDocsRefObject *_deeoverloaddocref_params_get(
 DeeOverloadDocRefObject *self, void *DEE_UNUSED(closure)) {
 DeeParamDocsRefObject *result;
 if ((result = DeeObject_MALLOC(DeeParamDocsRefObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeParamDocsRef_Type);
 Dee_INCREF(result->dr_keepalive = self->dr_keepalive);
 result->dr_doc = &self->dr_doc->od_params;
 return result;
}
static DeeThrowDocsRefObject *_deeoverloaddocref_throws_get(
 DeeOverloadDocRefObject *self, void *DEE_UNUSED(closure)) {
 DeeThrowDocsRefObject *result;
 if ((result = DeeObject_MALLOC(DeeThrowDocsRefObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeThrowDocsRef_Type);
 Dee_INCREF(result->dr_keepalive = self->dr_keepalive);
 result->dr_doc = &self->dr_doc->od_throws;
 return result;
}

static DeeObject *_deeoverloaddocref_has_doc(
 DeeOverloadDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_doc") != 0) return NULL;
 DeeReturn_Bool(self->dr_doc->od_doc != NULL);
}



static struct DeeGetSetDef const _deeoverloaddocref_tp_getsets[] = {
 DEE_GETSETDEF_v100("doc",member(&_deeoverloaddocref_doc_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("return",member(&_deeoverloaddocref_return_get),null,null,"-> doc.method.overload.param"),
 DEE_GETSETDEF_v100("params",member(&_deeoverloaddocref_params_get),null,null,"-> doc.method.overload.params"),
 DEE_GETSETDEF_v100("throws",member(&_deeoverloaddocref_throws_get),null,null,"-> doc.method.overload.throws"),
 DEE_GETSETDEF_END_v100
};

static struct DeeMethodDef const _deeoverloaddocref_tp_methods[] = {
 DEE_METHODDEF_v100("has_doc",member(&_deeoverloaddocref_has_doc),"() -> bool"),
 DEE_METHODDEF_END_v100
};


DeeTypeObject DeeOverloadDocRef_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("doc.method.overload"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeOverloadDocRefObject),null,
  null,member(&_deedocref_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deedocref_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,member(&_deeoverloaddocref_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,member(&_deedocref_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,member(_deeoverloaddocref_tp_getsets),
  member(_deeoverloaddocref_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(null),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEOVERLOADDOCREF_TYPE_INL */
