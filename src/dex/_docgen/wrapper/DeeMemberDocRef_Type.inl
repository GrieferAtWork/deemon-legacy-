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
#ifndef GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEMEMBERDOCREF_TYPE_INL
#define GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEMEMBERDOCREF_TYPE_INL 1
#ifdef __INTELLISENSE__
#include "../_docgen.c"
#endif
#include "_deedocref.inl"

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// DeeMemberDocRefObject
static DeeObject *_deememberdocref_tp_repr(DeeMemberDocRefObject *self) {
 return DeeMemberDoc_Repr(self->dr_doc);
}
static DeeObject *_deememberdocref_name_get(
 DeeMemberDocRefObject *self, void *DEE_UNUSED(closure)) {
 DeeReturn_NEWREF((DeeObject *)self->dr_doc->mbd_name);
}
static DeeObject *_deememberdocref_doc_get(
 DeeMemberDocRefObject *self, void *DEE_UNUSED(closure)) {
 if (self->dr_doc->mbd_doc) DeeReturn_NEWREF((DeeObject *)self->dr_doc->mbd_doc);
 DeeReturn_EmptyString;
}
static DeeParamDocRefObject *_deememberdocref_return_get(
 DeeMemberDocRefObject *self, void *DEE_UNUSED(closure)) {
 DeeParamDocRefObject *result;
 if ((result = DeeObject_MALLOC(DeeParamDocRefObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeParamDocRef_Type);
 result->dr_doc = &self->dr_doc->mbd_return;
 Dee_INCREF(result->dr_keepalive = self->dr_keepalive);
 return result;
}
static DeeObject *_deememberdocref_thistype_get(
 DeeMemberDocRefObject *self, void *DEE_UNUSED(closure)) {
 DeeReturn_NEWREF((DeeObject *)self->dr_doc->mbd_this);
}
static DeeObject *_deememberdocref_value_get(
 DeeMemberDocRefObject *self, void *DEE_UNUSED(closure)) {
 if (!self->dr_doc->mbd_value) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "doc.member %k does not have a value set",
                      self->dr_doc->mbd_name);
  return NULL;
 }
 DeeReturn_NEWREF((DeeObject *)self->dr_doc->mbd_value);
}
static DeeObject *_deememberdocref_flags_get(
 DeeMemberDocRefObject *self, void *DEE_UNUSED(closure)) {
 return DeeObject_New(Dee_uint32_t,self->dr_doc->mbd_flags);
}
static DeeObject *_deememberdocref_is_class(
 DeeMemberDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_class") != 0) return NULL;
 DeeReturn_Bool((self->dr_doc->mbd_flags&DEE_MEMBERDOC_FLAG_CLASS)!=0);
}
static DeeObject *_deememberdocref_has_read(
 DeeMemberDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_read") != 0) return NULL;
 DeeReturn_Bool((self->dr_doc->mbd_flags&DEE_MEMBERDOC_FLAG_READ)!=0);
}
static DeeObject *_deememberdocref_has_write(
 DeeMemberDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_write") != 0) return NULL;
 DeeReturn_Bool((self->dr_doc->mbd_flags&DEE_MEMBERDOC_FLAG_WRITE)!=0);
}
static DeeObject *_deememberdocref_is_atomic(
 DeeMemberDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_atomic") != 0) return NULL;
 DeeReturn_Bool((self->dr_doc->mbd_flags&DEE_MEMBERDOC_FLAG_ATOMIC)!=0);
}
static DeeObject *_deememberdocref_has_doc(
 DeeMemberDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_doc") != 0) return NULL;
 DeeReturn_Bool(self->dr_doc->mbd_doc != NULL);
}
static DeeObject *_deememberdocref_has_value(
 DeeMemberDocRefObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":has_value") != 0) return NULL;
 DeeReturn_Bool(self->dr_doc->mbd_value != NULL);
}


static struct DeeMemberDef const _deememberdocref_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("FLAG_NONE",uint32_t,DEE_MEMBERDOC_FLAG_NONE),
 DEE_MEMBERDEF_CONST_v100("FLAG_CLASS",uint32_t,DEE_MEMBERDOC_FLAG_CLASS),
 DEE_MEMBERDEF_CONST_v100("FLAG_READ",uint32_t,DEE_MEMBERDOC_FLAG_READ),
 DEE_MEMBERDEF_CONST_v100("FLAG_WRITE",uint32_t,DEE_MEMBERDOC_FLAG_WRITE),
 DEE_MEMBERDEF_CONST_v100("FLAG_ATOMIC",uint32_t,DEE_MEMBERDOC_FLAG_ATOMIC),
 DEE_MEMBERDEF_CONST_v100("overloads",object,&DeeOverloadDocsRef_Type),
 DEE_MEMBERDEF_CONST_v100("overload",object,&DeeOverloadDocRef_Type),
 DEE_MEMBERDEF_END_v100
};
static struct DeeGetSetDef const _deememberdocref_tp_getsets[] = {
 DEE_GETSETDEF_v100("name",member(&_deememberdocref_name_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("doc",member(&_deememberdocref_doc_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("thistype",member(&_deememberdocref_thistype_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("return",member(&_deememberdocref_return_get),null,null,"-> string"),
 DEE_GETSETDEF_v100("value",member(&_deememberdocref_value_get),null,null,"-> object"),
 DEE_GETSETDEF_v100("flags",member(&_deememberdocref_flags_get),null,null,"-> uint32_t"),
 DEE_GETSETDEF_END_v100
};
static struct DeeMethodDef const _deememberdocref_tp_methods[] = {
 DEE_METHODDEF_v100("is_class",member(&_deememberdocref_is_class),"-> bool"),
 DEE_METHODDEF_v100("is_atomic",member(&_deememberdocref_is_atomic),"-> bool"),
 DEE_METHODDEF_v100("has_read",member(&_deememberdocref_has_read),"-> bool"),
 DEE_METHODDEF_v100("has_write",member(&_deememberdocref_has_write),"-> bool"),
 DEE_METHODDEF_v100("has_doc",member(&_deememberdocref_has_doc),"-> bool"),
 DEE_METHODDEF_v100("has_value",member(&_deememberdocref_has_value),"-> bool"),
 DEE_METHODDEF_END_v100
};

DeeTypeObject DeeMemberDocRef_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("doc.member"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeMemberDocRefObject),null,
  null,member(&_deedocref_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deedocref_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,member(&_deememberdocref_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,member(&_deedocref_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,member(_deememberdocref_tp_getsets),member(_deememberdocref_tp_methods),
  member(_deememberdocref_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(null),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEMEMBERDOCREF_TYPE_INL */
