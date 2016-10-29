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
#ifndef GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEPARAMDOCREF_TYPE_INL
#define GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEPARAMDOCREF_TYPE_INL 1
#ifdef __INTELLISENSE__
#include "../_docgen.c"
#endif
#include "_deedocref.inl"

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// DeeParamDocRefObject
static DeeObject *_deeparamdocref_tp_repr(DeeParamDocRefObject *self) {
 return DeeParamDoc_Repr(self->dr_doc);
}
static DeeObject *_deeparamdocref_type_get(
 DeeParamDocRefObject *self, void *DEE_UNUSED(closure)) {
 DeeReturn_NEWREF((DeeObject *)self->dr_doc->pd_type);
}
static DeeObject *_deeparamdocref_name_get(
 DeeParamDocRefObject *self, void *DEE_UNUSED(closure)) {
 if (self->dr_doc->pd_name) DeeReturn_NEWREF((DeeObject *)self->dr_doc->pd_name);
 DeeReturn_EmptyString;
}
static DeeObject *_deeparamdocref_default_get(
 DeeParamDocRefObject *self, void *DEE_UNUSED(closure)) {
 if (self->dr_doc->pd_def) DeeReturn_NEWREF((DeeObject *)self->dr_doc->pd_def);
 DeeReturn_EmptyString;
}
static DeeObject *_deeparamdocref_doc_get(
 DeeParamDocRefObject *self, void *DEE_UNUSED(closure)) {
 if (self->dr_doc->pd_doc) DeeReturn_NEWREF((DeeObject *)self->dr_doc->pd_doc);
 DeeReturn_EmptyString;
}
static DeeObject *_deeparamdocref_flags_get(
 DeeParamDocRefObject *self, void *DEE_UNUSED(closure)) {
 return DeeObject_New(Dee_uint32_t,self->dr_doc->pd_flags);
}

static DeeObject *_deeparamdocref_flags_has_varg(
 DeeParamDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_varg") != 0) return NULL;
 DeeReturn_Bool((self->dr_doc->pd_flags&DEE_PARAMDOC_FLAG_VARG)!=0);
}
static DeeObject *_deeparamdocref_flags_is_return(
 DeeParamDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_return") != 0) return NULL;
 DeeReturn_Bool((self->dr_doc->pd_flags&DEE_PARAMDOC_FLAG_RETURN)!=0);
}
static DeeObject *_deeparamdocref_flags_has_doc(
 DeeParamDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_doc") != 0) return NULL;
 DeeReturn_Bool(self->dr_doc->pd_doc != NULL);
}
static DeeObject *_deeparamdocref_flags_has_name(
 DeeParamDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_name") != 0) return NULL;
 DeeReturn_Bool(self->dr_doc->pd_name != NULL);
}
static DeeObject *_deeparamdocref_flags_has_default(
 DeeParamDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_default") != 0) return NULL;
 DeeReturn_Bool(self->dr_doc->pd_def != NULL);
}


static struct DeeMemberDef const _deeparamdocref_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("FLAG_NONE",Dee_uint32_t,DEE_PARAMDOC_FLAG_NONE),
 DEE_MEMBERDEF_CONST_v100("FLAG_VARG",Dee_uint32_t,DEE_PARAMDOC_FLAG_VARG),
 DEE_MEMBERDEF_CONST_v100("FLAG_RETURN",Dee_uint32_t,DEE_PARAMDOC_FLAG_RETURN),
 DEE_MEMBERDEF_END_v100
};

static struct DeeGetSetDef const _deeparamdocref_tp_getsets[] = {
 DEE_GETSETDEF_v100("type",member(&_deeparamdocref_type_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("name",member(&_deeparamdocref_name_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("doc",member(&_deeparamdocref_doc_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("default",member(&_deeparamdocref_default_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("flags",member(&_deeparamdocref_flags_get),null,null,"-> uint32_t"),
 DEE_GETSETDEF_END_v100
};
static struct DeeMethodDef const _deeparamdocref_tp_methods[] = {
 DEE_METHODDEF_v100("is_return",member(&_deeparamdocref_flags_is_return),"() -> bool"),
 DEE_METHODDEF_v100("has_varg",member(&_deeparamdocref_flags_has_varg),"() -> bool"),
 DEE_METHODDEF_v100("has_doc",member(&_deeparamdocref_flags_has_doc),"() -> bool"),
 DEE_METHODDEF_v100("has_name",member(&_deeparamdocref_flags_has_name),"() -> bool"),
 DEE_METHODDEF_v100("has_default",member(&_deeparamdocref_flags_has_default),"() -> bool"),
 DEE_METHODDEF_END_v100
};

DeeTypeObject DeeParamDocRef_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("doc.method.overload.param"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeParamDocRefObject),null,
  null,member(&_deedocref_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deedocref_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,member(&_deeparamdocref_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,member(&_deedocref_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,member(_deeparamdocref_tp_getsets),member(_deeparamdocref_tp_methods),
  member(_deeparamdocref_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(null),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEPARAMDOCREF_TYPE_INL */
