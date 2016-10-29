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
#ifndef GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEMETHODDOCREF_TYPE_INL
#define GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEMETHODDOCREF_TYPE_INL 1
#ifdef __INTELLISENSE__
#include "../_docgen.c"
#endif
#include "_deedocref.inl"

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// DeeMethodDocRefObject
static DeeObject *_deemethoddocref_tp_repr(DeeMethodDocRefObject *self) {
 return DeeMethodDoc_Repr(self->dr_doc,"function");
}
static DeeObject *_deemethoddocref_name_get(
 DeeMethodDocRefObject *self, void *DEE_UNUSED(closure)) {
 DeeReturn_NEWREF((DeeObject *)self->dr_doc->md_name);
}
static DeeObject *_deemethoddocref_flags_get(
 DeeMethodDocRefObject *self, void *DEE_UNUSED(closure)) {
 return DeeObject_New(Dee_uint32_t,self->dr_doc->md_flags);
}
static DeeObject *_deemethoddocref_thistype_get(
 DeeMethodDocRefObject *self, void *DEE_UNUSED(closure)) {
 if (self->dr_doc->md_this) DeeReturn_NEWREF(self->dr_doc->md_this);
 DeeReturn_EmptyString;
}
static DeeOverloadDocsRefObject *_deemethoddocref_overloads_get(
 DeeMethodDocRefObject *self, void *DEE_UNUSED(closure)) {
 DeeOverloadDocsRefObject *result;
 if ((result = DeeObject_MALLOC(DeeOverloadDocsRefObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeOverloadDocsRef_Type);
 Dee_INCREF(result->dr_keepalive = self->dr_keepalive);
 result->dr_doc = &self->dr_doc->md_over;
 return result;
}
static DeeObject *_deemethoddocref_is_class(
 DeeMethodDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_class") != 0) return NULL;
 DeeReturn_Bool((self->dr_doc->md_flags&DEE_METHODDOC_FLAG_CLASS)!=0);
}
static DeeObject *_deemethoddocref_is_constexpr(
 DeeMethodDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_constexpr") != 0) return NULL;
 DeeReturn_Bool((self->dr_doc->md_flags&DEE_METHODDOC_FLAG_CONSTEXPR)!=0);
}
static DeeObject *_deemethoddocref_is_operator(
 DeeMethodDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_operator") != 0) return NULL;
 DeeReturn_Bool((self->dr_doc->md_flags&DEE_METHODDOC_FLAG_OPERATOR)!=0);
}
static DeeObject *_deemethoddocref_has_thistype(
 DeeMethodDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_thistype") != 0) return NULL;
 DeeReturn_Bool(self->dr_doc->md_this != NULL);
}


static struct DeeMemberDef const _deemethoddocref_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("FLAG_NONE",uint32_t,DEE_METHODDOC_FLAG_NONE),
 DEE_MEMBERDEF_CONST_v100("FLAG_CLASS",uint32_t,DEE_METHODDOC_FLAG_CLASS),
 DEE_MEMBERDEF_CONST_v100("FLAG_CONSTEXPR",uint32_t,DEE_METHODDOC_FLAG_CONSTEXPR),
 DEE_MEMBERDEF_CONST_v100("FLAG_OPERATOR",uint32_t,DEE_METHODDOC_FLAG_OPERATOR),
 DEE_MEMBERDEF_CONST_v100("overloads",object,&DeeOverloadDocsRef_Type),
 DEE_MEMBERDEF_CONST_v100("overload",object,&DeeOverloadDocRef_Type),
 DEE_MEMBERDEF_END_v100
};
static struct DeeGetSetDef const _deemethoddocref_tp_getsets[] = {
 DEE_GETSETDEF_v100("name",member(&_deemethoddocref_name_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("flags",member(&_deemethoddocref_flags_get),null,null,"-> uint32_t"),
 DEE_GETSETDEF_v100("thistype",member(&_deemethoddocref_thistype_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("overloads",member(&_deemethoddocref_overloads_get),null,null,"-> doc.method.overloads"),
 DEE_GETSETDEF_END_v100
};
static struct DeeMethodDef const _deemethoddocref_tp_methods[] = {
 DEE_METHODDEF_v100("is_class",member(&_deemethoddocref_is_class),"-> bool"),
 DEE_METHODDEF_v100("is_constexpr",member(&_deemethoddocref_is_constexpr),"-> bool"),
 DEE_METHODDEF_v100("is_operator",member(&_deemethoddocref_is_operator),"-> bool"),
 DEE_METHODDEF_v100("has_thistype",member(&_deemethoddocref_has_thistype),"-> bool"),
 DEE_METHODDEF_END_v100
};

DeeTypeObject DeeMethodDocRef_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("doc.method"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeMethodDocRefObject),null,
  null,member(&_deedocref_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deedocref_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,member(&_deemethoddocref_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,member(&_deedocref_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,member(_deemethoddocref_tp_getsets),member(_deemethoddocref_tp_methods),
  member(_deemethoddocref_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(null),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEMETHODDOCREF_TYPE_INL */
