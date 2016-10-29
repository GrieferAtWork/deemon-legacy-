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
#ifndef GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEDOC_TYPE_INL
#define GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEDOC_TYPE_INL 1
#ifdef __INTELLISENSE__
#include "../_docgen.c"
#endif

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// DeeDocObject
static int _deedoc_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeDocObject *self) {
 self->d_kind = DEE_DOCOBJECT_KIND_OBJ;
 self->d_doc.doc_kind = DEE_DOCKIND_NONE;
 return 0;
}
static int _deedoc_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeDocObject *self, DeeDocObject *right) {
 self->d_kind = DEE_DOCOBJECT_KIND_REF;
 self->d_ref.dr_doc = DeeDoc_DOC(right);
 while (right->d_kind == DEE_DOCOBJECT_KIND_REF)
  right = right->d_ref.dr_keepalive;
 Dee_INCREF(self->d_ref.dr_keepalive = right);
 return 0;
}
static int _deedoc_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeDocObject *self, DeeObject *args) {
 DeeObject *doc_ob,*attr = NULL; int error;
 if (DeeTuple_Unpack(args,"o|o:doc",&doc_ob,&attr) != 0) return -1;
 self->d_kind = DEE_DOCOBJECT_KIND_OBJ;
 if (!attr) return DeeDoc_InitFromObject(&self->d_doc,doc_ob);
 if ((attr = DeeString_Cast(attr)) == NULL) return -1;
 error = DeeDoc_InitFromObjectAttrString(&self->d_doc,doc_ob,DeeString_STR(attr));
 Dee_DECREF(attr);
 return error;
}
static void _deedoc_tp_dtor(DeeDocObject *self) {
 if (self->d_kind == DEE_DOCOBJECT_KIND_OBJ) {
  DeeDoc_Quit(&self->d_doc);
 } else {
  DEE_ASSERT(self->d_kind == DEE_DOCOBJECT_KIND_REF);
  DEE_ASSERT(self->d_ref.dr_doc);
  DEE_ASSERT(DeeObject_Check(self->d_ref.dr_keepalive));
  Dee_DECREF(self->d_ref.dr_keepalive);
 }
}
DEE_VISIT_PROC(_deedoc_tp_visit,DeeDocObject *self) {
 if (self->d_kind == DEE_DOCOBJECT_KIND_OBJ) {
  _DeeDoc_Visit(&self->d_doc);
 } else {
  DEE_ASSERT(self->d_kind == DEE_DOCOBJECT_KIND_REF);
  DEE_ASSERT(self->d_ref.dr_doc);
  DEE_ASSERT(DeeObject_Check(self->d_ref.dr_keepalive));
  Dee_VISIT(self->d_ref.dr_keepalive);
 }
}
static DeeObject *_deedoc_tp_repr(DeeDocObject *self) {
 return DeeDoc_Repr(DeeDoc_DOC(self));
}


static struct DeeMemberDef const _deedoc_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("KIND_NONE",Dee_uint32_t,DEE_DOCKIND_NONE),
 DEE_MEMBERDEF_CONST_v100("KIND_GENERIC",Dee_uint32_t,DEE_DOCKIND_GENERIC),
 DEE_MEMBERDEF_CONST_v100("KIND_METHOD",Dee_uint32_t,DEE_DOCKIND_METHOD),
 DEE_MEMBERDEF_CONST_v100("KIND_MEMBER",Dee_uint32_t,DEE_DOCKIND_MEMBER),
 DEE_MEMBERDEF_CONST_v100("KIND_GETSET",Dee_uint32_t,DEE_DOCKIND_GETSET),
 DEE_MEMBERDEF_CONST_v100("KIND_TYPE",Dee_uint32_t,DEE_DOCKIND_TYPE),
 DEE_MEMBERDEF_CONST_v100("generic",object,&DeeGenericDocRef_Type),
 DEE_MEMBERDEF_CONST_v100("method",object,&DeeMethodDocRef_Type),
 DEE_MEMBERDEF_CONST_v100("member",object,&DeeMemberDocRef_Type),
 DEE_MEMBERDEF_CONST_v100("getset",object,&DeeGetSetDocRef_Type),
 DEE_MEMBERDEF_CONST_v100("type",object,&DeeTypeDocRef_Type),
 DEE_MEMBERDEF_END_v100
};
static char const *DeeDocKind_ToString(Dee_uint32_t kind) {
 struct DeeMemberDef const *iter;
 iter = _deedoc_tp_class_members;
 while (iter->d_name) {
  if (iter->d_type == (DeeMemberType_CONST|DeeMemberType_uint32_t) &&
      ((Dee_uint32_t)iter->d_offset) == kind) return iter->d_name;
  ++iter;
 }
 return "?";
}



static void _error_invalid_doc_kind(Dee_uint32_t expected, Dee_uint32_t found) {
 DeeError_SetStringf(&DeeErrorType_ValueError,
                     "Invalid doc kind. Expected %s, but got %s",
                     DeeDocKind_ToString(expected),DeeDocKind_ToString(found));
}
static DeeObject *_deedoc_kind_get(
 DeeDocObject *self, void *DEE_UNUSED(closure)) {
 return DeeObject_New(Dee_uint32_t,DeeDoc_DOC(self)->doc_kind);
}
static DeeObject *_deedoc_name_get(
 DeeDocObject *self, void *DEE_UNUSED(closure)) {
 return DeeDoc_GetName(DeeDoc_DOC(self));
}
static DeeGenericDocRefObject *_deedoc_generic_get(
 DeeDocObject *self, void *DEE_UNUSED(closure)) {
 DeeGenericDocRefObject *result; struct DeeDoc *doc;
 doc = DeeDoc_DOC(self);
 if (doc->doc_kind != DEE_DOCKIND_GENERIC) {
  _error_invalid_doc_kind(DEE_DOCKIND_GENERIC,doc->doc_kind);
  return NULL;
 }
 if ((result = DeeObject_MALLOC(DeeGenericDocRefObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeGenericDocRef_Type);
 Dee_INCREF(result->dr_keepalive = self);
 result->dr_doc = &doc->doc_generic;
 return result;
}
static DeeMethodDocRefObject *_deedoc_method_get(
 DeeDocObject *self, void *DEE_UNUSED(closure)) {
 DeeMethodDocRefObject *result; struct DeeDoc *doc;
 doc = DeeDoc_DOC(self);
 if (doc->doc_kind != DEE_DOCKIND_METHOD) {
  _error_invalid_doc_kind(DEE_DOCKIND_METHOD,doc->doc_kind);
  return NULL;
 }
 if ((result = DeeObject_MALLOC(DeeMethodDocRefObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeMethodDocRef_Type);
 Dee_INCREF(result->dr_keepalive = self);
 result->dr_doc = &doc->doc_method;
 return result;
}

static DeeMemberDocRefObject *_deedoc_member_get(
 DeeDocObject *self, void *DEE_UNUSED(closure)) {
 DeeMemberDocRefObject *result; struct DeeDoc *doc;
 doc = DeeDoc_DOC(self);
 if (doc->doc_kind != DEE_DOCKIND_MEMBER) {
  _error_invalid_doc_kind(DEE_DOCKIND_MEMBER,doc->doc_kind);
  return NULL;
 }
 if ((result = DeeObject_MALLOC(DeeMemberDocRefObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeMemberDocRef_Type);
 Dee_INCREF(result->dr_keepalive = self);
 result->dr_doc = &doc->doc_member;
 return result;
}

static DeeGetSetDocRefObject *_deedoc_getset_get(
 DeeDocObject *self, void *DEE_UNUSED(closure)) {
 DeeGetSetDocRefObject *result; struct DeeDoc *doc;
 doc = DeeDoc_DOC(self);
 if (doc->doc_kind != DEE_DOCKIND_GETSET) {
  _error_invalid_doc_kind(DEE_DOCKIND_GETSET,doc->doc_kind);
  return NULL;
 }
 if ((result = DeeObject_MALLOC(DeeGetSetDocRefObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeGetSetDocRef_Type);
 Dee_INCREF(result->dr_keepalive = self);
 result->dr_doc = &doc->doc_getset;
 return result;
}

static DeeTypeDocRefObject *_deedoc_type_get(
 DeeDocObject *self, void *DEE_UNUSED(closure)) {
 DeeTypeDocRefObject *result; struct DeeDoc *doc;
 doc = DeeDoc_DOC(self);
 if (doc->doc_kind != DEE_DOCKIND_TYPE) {
  _error_invalid_doc_kind(DEE_DOCKIND_TYPE,doc->doc_kind);
  return NULL;
 }
 if ((result = DeeObject_MALLOC(DeeTypeDocRefObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeTypeDocRef_Type);
 Dee_INCREF(result->dr_keepalive = self);
 result->dr_doc = &doc->doc_type;
 return result;
}


static DeeObject *_deedoc_is_generic(
 DeeDocObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_generic") != 0) return NULL;
 DeeReturn_Bool(DeeDoc_DOC(self)->doc_kind == DEE_DOCKIND_GENERIC);
}
static DeeObject *_deedoc_is_method(
 DeeDocObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_method") != 0) return NULL;
 DeeReturn_Bool(DeeDoc_DOC(self)->doc_kind == DEE_DOCKIND_METHOD);
}
static DeeObject *_deedoc_is_member(
 DeeDocObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_member") != 0) return NULL;
 DeeReturn_Bool(DeeDoc_DOC(self)->doc_kind == DEE_DOCKIND_MEMBER);
}
static DeeObject *_deedoc_is_getset(
 DeeDocObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_getset") != 0) return NULL;
 DeeReturn_Bool(DeeDoc_DOC(self)->doc_kind == DEE_DOCKIND_GETSET);
}
static DeeObject *_deedoc_is_type(
 DeeDocObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_type") != 0) return NULL;
 DeeReturn_Bool(DeeDoc_DOC(self)->doc_kind == DEE_DOCKIND_TYPE);
}




static struct DeeGetSetDef const _deedoc_tp_getsets[] = {
 DEE_GETSETDEF_v100("kind",member(&_deedoc_kind_get),null,null,"-> uint32_t"),
 DEE_GETSETDEF_v100("name",member(&_deedoc_name_get),null,null,"-> doc.type"),
 DEE_GETSETDEF_v100("generic",member(&_deedoc_generic_get),null,null,"-> doc.generic"),
 DEE_GETSETDEF_v100("method",member(&_deedoc_method_get),null,null,"-> doc.method"),
 DEE_GETSETDEF_v100("member",member(&_deedoc_member_get),null,null,"-> doc.member"),
 DEE_GETSETDEF_v100("getset",member(&_deedoc_getset_get),null,null,"-> doc.getset"),
 DEE_GETSETDEF_v100("type",member(&_deedoc_type_get),null,null,"-> doc.type"),
 DEE_GETSETDEF_END_v100
};

static struct DeeMethodDef const _deedoc_tp_methods[] = {
 DEE_METHODDEF_v100("is_generic",member(&_deedoc_is_generic),"() -> bool"),
 DEE_METHODDEF_v100("is_method",member(&_deedoc_is_method),"() -> bool"),
 DEE_METHODDEF_v100("is_member",member(&_deedoc_is_member),"() -> bool"),
 DEE_METHODDEF_v100("is_getset",member(&_deedoc_is_getset),"() -> bool"),
 DEE_METHODDEF_v100("is_type",member(&_deedoc_is_type),"() -> bool"),
 DEE_METHODDEF_END_v100
};


DeeTypeObject DeeDoc_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("doc"),member(
  "() -> doc\n"
  "(object ob) -> doc\n"
  "(object ob, string attr) -> doc\n"
  "@param ob: Object to generate a doc for\n"
  "@param attr: An attribute of 'ob' to generate a doc for\n"
  "\tInitializes a new doc object, collecting documentation for a given object, or attribute"
  ),
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeDocObject),null,
  member(&_deedoc_tp_ctor),member(&_deedoc_tp_copy_ctor),null,
  member(&_deedoc_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deedoc_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,member(&_deedoc_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,member(&_deedoc_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,member(_deedoc_tp_getsets),member(_deedoc_tp_methods),
  member(_deedoc_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(null),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEDOC_TYPE_INL */
