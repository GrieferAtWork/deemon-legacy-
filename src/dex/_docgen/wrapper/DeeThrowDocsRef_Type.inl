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
#ifndef GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEETHROWDOCSREF_TYPE_INL
#define GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEETHROWDOCSREF_TYPE_INL 1
#ifdef __INTELLISENSE__
#include "../_docgen.c"
#endif
#include "_deedocref.inl"

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// DeeThrowDocsRefObject
static DeeObject *_deethrowdocsref_tp_repr(DeeThrowDocsRefObject *self) {
 return DeeThrowDocs_Repr(self->dr_doc);
}
static DeeThrowDocsRefIteratorObject *
_deethrowdocsref_tp_seq_iter_self(DeeThrowDocsRefObject *self) {
 DeeThrowDocsRefIteratorObject *result;
 if ((result = DeeObject_MALLOC(DeeThrowDocsRefIteratorObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeThrowDocsRefIterator_Type);
 Dee_INCREF(result->dri_keepalive = self->dr_keepalive);
 result->dri_end = (result->dri_iter = self->dr_doc->tds_throwv)+self->dr_doc->tds_throwc;
 return result;
}


static struct DeeMethodDef const _deethrowdocsref_tp_methods[] = {
 DEE_METHODDEF_CONST_v100("at",member(&_deegenericiterable_at),NULL),
 DEE_METHODDEF_CONST_v100("back",member(&_deegenericiterable_back),NULL),
 DEE_METHODDEF_CONST_v100("contains",member(&_deegenericiterable_contains),NULL),
 DEE_METHODDEF_CONST_v100("count",member(&_deegenericiterable_count),NULL),
 DEE_METHODDEF_CONST_v100("empty",member(&_deegenericiterable_empty),NULL),
 DEE_METHODDEF_CONST_v100("find",member(&_deegenericiterable_find),NULL),
 DEE_METHODDEF_CONST_v100("front",member(&_deegenericiterable_front),NULL),
 DEE_METHODDEF_CONST_v100("index",member(&_deegenericiterable_index),NULL),
 DEE_METHODDEF_CONST_v100("locate",member(&_deegenericiterable_locate),NULL),
 DEE_METHODDEF_CONST_v100("locate_all",member(&_deegenericiterable_locate_all),NULL),
 DEE_METHODDEF_CONST_v100("non_empty",member(&_deegenericiterable_non_empty),NULL),
 DEE_METHODDEF_CONST_v100("reversed",member(&_deegenericiterable_reversed),NULL),
 DEE_METHODDEF_CONST_v100("rfind",member(&_deegenericiterable_rfind),NULL),
 DEE_METHODDEF_CONST_v100("rindex",member(&_deegenericiterable_rindex),NULL),
 DEE_METHODDEF_CONST_v100("rlocate",member(&_deegenericiterable_rlocate),NULL),
 DEE_METHODDEF_CONST_v100("segments",member(&_deegenericiterable_segments),NULL),
 DEE_METHODDEF_CONST_v100("sorted",member(&_deegenericiterable_sorted),NULL),
 DEE_METHODDEF_CONST_v100("transform",member(&_deegenericiterable_transform),NULL),
 DEE_METHODDEF_END_v100
};
static struct DeeMemberDef const _deethrowdocsref_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("elem",object,&DeeThrowDocRef_Type),
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeThrowDocsRefIterator_Type),
 DEE_MEMBERDEF_END_v100
};

DeeTypeObject DeeThrowDocsRef_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("doc.method.overload.throws"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeThrowDocsRefObject),null,
  null,member(&_deedocref_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deedocref_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,member(&_deethrowdocsref_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,member(&_deedocref_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,
  member(&_deethrowdocsref_tp_seq_iter_self),null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,null,member(_deethrowdocsref_tp_methods),
  member(_deethrowdocsref_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(null),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEETHROWDOCSREF_TYPE_INL */
