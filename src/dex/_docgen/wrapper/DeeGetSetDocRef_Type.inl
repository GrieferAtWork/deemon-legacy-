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
#ifndef GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEGETSETDOCREF_TYPE_INL
#define GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEGETSETDOCREF_TYPE_INL 1
#ifdef __INTELLISENSE__
#include "../_docgen.c"
#endif
#include "_deedocref.inl"

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// DeeGetSetDocRefObject
static DeeObject *_deegetsetdocref_tp_repr(DeeGetSetDocRefObject *self) {
 return DeeGetSetDoc_Repr(self->dr_doc);
}
static DeeObject *_deegetsetdocref_name_get(
 DeeGetSetDocRefObject *self, void *DEE_UNUSED(closure)) {
 DeeReturn_NEWREF((DeeObject *)self->dr_doc->gd_name);
}
static DeeObject *_deegetsetdocref_doc_get(
 DeeGetSetDocRefObject *self, void *DEE_UNUSED(closure)) {
 if (self->dr_doc->gd_doc) DeeReturn_NEWREF((DeeObject *)self->dr_doc->gd_doc);
 DeeReturn_EmptyString;
}
static DeeObject *_deegetsetdocref_thistype_get(
 DeeGetSetDocRefObject *self, void *DEE_UNUSED(closure)) {
 if (self->dr_doc->gd_this) DeeReturn_NEWREF((DeeObject *)self->dr_doc->gd_this);
 DeeReturn_EmptyString;
}
static DeeObject *_deegetsetdocref_flags_get(
 DeeGetSetDocRefObject *self, void *DEE_UNUSED(closure)) {
 return DeeObject_New(Dee_uint32_t,self->dr_doc->gd_flags);
}
static DeeParamDocRefObject *_deegetsetdocref_return_get(
 DeeGetSetDocRefObject *self, void *DEE_UNUSED(closure)) {
 DeeParamDocRefObject *result;
 if ((result = DeeObject_MALLOC(DeeParamDocRefObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeParamDocRef_Type);
 Dee_INCREF(result->dr_keepalive = self->dr_keepalive);
 result->dr_doc = &self->dr_doc->gd_return;
 return result;
}
static DeeThrowDocsRefObject *_deegetsetdocref_throws_get(
 DeeGetSetDocRefObject *self, void *DEE_UNUSED(closure)) {
 DeeThrowDocsRefObject *result;
 if ((result = DeeObject_MALLOC(DeeThrowDocsRefObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeThrowDocsRef_Type);
 Dee_INCREF(result->dr_keepalive = self->dr_keepalive);
 result->dr_doc = &self->dr_doc->gd_throw;
 return result;
}
static DeeObject *_deegetsetdocref_is_class(
 DeeGetSetDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_class") != 0) return NULL;
 DeeReturn_Bool((self->dr_doc->gd_flags&DEE_GETSETDOC_FLAG_CLASS)!=0);
}
static DeeObject *_deegetsetdocref_is_constexpr(
 DeeGetSetDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_constexpr") != 0) return NULL;
 DeeReturn_Bool((self->dr_doc->gd_flags&DEE_GETSETDOC_FLAG_CONSTEXPR)!=0);
}
static DeeObject *_deegetsetdocref_has_get(
 DeeGetSetDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_get") != 0) return NULL;
 DeeReturn_Bool((self->dr_doc->gd_flags&DEE_GETSETDOC_FLAG_GET)!=0);
}
static DeeObject *_deegetsetdocref_has_del(
 DeeGetSetDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_del") != 0) return NULL;
 DeeReturn_Bool((self->dr_doc->gd_flags&DEE_GETSETDOC_FLAG_DEL)!=0);
}
static DeeObject *_deegetsetdocref_has_set(
 DeeGetSetDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_set") != 0) return NULL;
 DeeReturn_Bool((self->dr_doc->gd_flags&DEE_GETSETDOC_FLAG_SET)!=0);
}
static DeeObject *_deegetsetdocref_has_doc(
 DeeGetSetDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_doc") != 0) return NULL;
 DeeReturn_Bool(self->dr_doc->gd_doc != NULL);
}
static DeeObject *_deegetsetdocref_has_thistype(
 DeeGetSetDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_thistype") != 0) return NULL;
 DeeReturn_Bool(self->dr_doc->gd_this != NULL);
}


static struct DeeMemberDef const _deegetsetdocref_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("FLAG_NONE",uint32_t,DEE_GETSETDOC_FLAG_NONE),
 DEE_MEMBERDEF_CONST_v100("FLAG_CLASS",uint32_t,DEE_GETSETDOC_FLAG_CLASS),
 DEE_MEMBERDEF_CONST_v100("FLAG_CONSTEXPR",uint32_t,DEE_GETSETDOC_FLAG_CONSTEXPR),
 DEE_MEMBERDEF_CONST_v100("FLAG_GET",uint32_t,DEE_GETSETDOC_FLAG_GET),
 DEE_MEMBERDEF_CONST_v100("FLAG_DEL",uint32_t,DEE_GETSETDOC_FLAG_DEL),
 DEE_MEMBERDEF_CONST_v100("FLAG_SET",uint32_t,DEE_GETSETDOC_FLAG_SET),
 DEE_MEMBERDEF_CONST_v100("throws",object,&DeeThrowDocsRef_Type),
 DEE_MEMBERDEF_CONST_v100("return",object,&DeeParamDocRef_Type),
 DEE_MEMBERDEF_END_v100
};
static struct DeeGetSetDef const _deegetsetdocref_tp_getsets[] = {
 DEE_GETSETDEF_v100("name",member(&_deegetsetdocref_name_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("doc",member(&_deegetsetdocref_doc_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("thistype",member(&_deegetsetdocref_thistype_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("flags",member(&_deegetsetdocref_flags_get),null,null,"-> uint32_t"),
 DEE_GETSETDEF_v100("return",member(&_deegetsetdocref_return_get),null,null,"-> doc.method.overload.param"),
 DEE_GETSETDEF_v100("throws",member(&_deegetsetdocref_throws_get),null,null,"-> doc.method.overload.throws"),
 DEE_GETSETDEF_END_v100
};
static struct DeeMethodDef const _deegetsetdocref_tp_methods[] = {
 DEE_METHODDEF_v100("is_class",member(&_deegetsetdocref_is_class),"-> bool"),
 DEE_METHODDEF_v100("is_constexpr",member(&_deegetsetdocref_is_constexpr),"-> bool"),
 DEE_METHODDEF_v100("has_thistype",member(&_deegetsetdocref_has_thistype),"-> bool"),
 DEE_METHODDEF_v100("has_get",member(&_deegetsetdocref_has_get),"-> bool"),
 DEE_METHODDEF_v100("has_del",member(&_deegetsetdocref_has_del),"-> bool"),
 DEE_METHODDEF_v100("has_set",member(&_deegetsetdocref_has_set),"-> bool"),
 DEE_METHODDEF_v100("has_doc",member(&_deegetsetdocref_has_doc),"-> bool"),
 DEE_METHODDEF_END_v100
};

DeeTypeObject DeeGetSetDocRef_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("doc.getset"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeGetSetDocRefObject),null,
  null,member(&_deedocref_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deedocref_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,member(&_deegetsetdocref_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,member(&_deedocref_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,member(_deegetsetdocref_tp_getsets),member(_deegetsetdocref_tp_methods),
  member(_deegetsetdocref_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(null),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEGETSETDOCREF_TYPE_INL */
