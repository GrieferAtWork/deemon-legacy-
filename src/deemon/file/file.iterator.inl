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
#ifndef GUARD_DEEMON_FILE_ITERATOR_INL
#define GUARD_DEEMON_FILE_ITERATOR_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/file.h>
#include <deemon/memberdef.h>
#include <deemon/marshal.h>
#include <deemon/optional/object_visit.h>
#include <deemon/optional/object_malloc.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>

// */ (nano...)

DEE_DECL_BEGIN

struct DeeFileIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeFileObject *fi_file; /*< [1..1] The file we are reading from. */
};


DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileIteratorObject) *
DeeFile_IterSelf(DEE_A_INOUT_OBJECT(DeeFileObject) *self) {
 DeeFileIteratorObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeFile_Check(self));
 if DEE_LIKELY((result = DeeObject_MALLOCF(
  DeeFileIteratorObject,"Iterator for file: %p",self)) != NULL) {
  DeeObject_INIT(result,&DeeFileIterator_Type);
  Dee_INCREF(result->fi_file = (DeeFileObject *)self);
 }
 return (DeeObject *)result;
}



static int _deefileiterator_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileIteratorObject *self) {
 if DEE_UNLIKELY((self->fi_file = (DeeFileObject *)DeeType_NewInstanceDefault(
  (DeeTypeObject *)&DeeFile_Type)) == NULL) return -1;
 return 0;
}
static void _deefileiterator_tp_dtor(DeeFileIteratorObject *self) {
 Dee_DECREF(self->fi_file);
}
DEE_VISIT_PROC(_deefileiterator_tp_visit,DeeFileIteratorObject *self) {
 Dee_VISIT(self->fi_file);
}
static int _deefileiterator_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileIteratorObject *self, DeeFileIteratorObject *right) {
 Dee_INCREF(self->fi_file = right->fi_file);
 return 0;
}
static int _deefileiterator_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileIteratorObject *self, DeeObject *args) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:file.iterator",&self->fi_file) != 0) return -1;
 if DEE_UNLIKELY((self->fi_file = (DeeFileObject *)DeeObject_GetInstance((
  DeeObject *)self->fi_file,(DeeTypeObject *)&DeeFile_Type)) == NULL) return -1;
 Dee_INCREF(self->fi_file);
 return 0;
}
static DeeObject *_deefileiterator_tp_str(DeeFileIteratorObject *self) {
 return DeeString_Newf("file.iterator(%k)",self->fi_file);
}
static DeeObject *_deefileiterator_tp_repr(DeeFileIteratorObject *self) {
 return DeeString_Newf("file.iterator(%r)",self->fi_file);
}
static int _deefileiterator_tp_seq_iter_next(
 DeeFileIteratorObject *self, DeeObject **result) {
 DeeObject *result_ob;
 if DEE_UNLIKELY((result_ob = DeeFile_ReadLine((
  DeeObject *)self->fi_file,(Dee_size_t)-1)) == NULL) return -1;
 if DEE_UNLIKELY(!DeeString_SIZE(result_ob)) { Dee_DECREF(result_ob); return 1; }
 Dee_INHERIT_REF(*result,result_ob);
 return 0;
}
static int DEE_CALL _deefileiterator_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileIteratorObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 if DEE_UNLIKELY((self->fi_file = (DeeFileObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if DEE_UNLIKELY(DeeError_TypeError_CheckType((DeeObject *)self->fi_file,(DeeTypeObject *)&DeeFile_Type) != 0) { Dee_DECREF(self->fi_file); return -1; }
 return 0;
}
static int DEE_CALL _deefileiterator_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileIteratorObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 return DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)self->fi_file);
}
static struct DeeTypeMarshal _deefileiterator_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(file_iterator)),
 member(&_deefileiterator_tp_marshal_ctor),member(&_deefileiterator_tp_marshal_put));

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deefileiterator_tp_member DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deefileiterator_tp_member[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__fi_file",DeeFileIteratorObject,fi_file,object),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */



DeeTypeObject DeeFileIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("file.iterator"),member(
  "(file fp) -> file.iterator\n"
  "\tIterator type for any file object, that returns the contents of the file split into lines.\n"
  "\tIt does so incrementally, meaning that is is safe to use this with <stdin>, but you can think of it as #(fp.read().splitlines(true))\n"
  "\tLinefeeds are normalized to \"\\n\" characters."),
  null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFileIteratorObject),null,
  member(&_deefileiterator_tp_ctor),
  member(&_deefileiterator_tp_copy_ctor),null,
  member(&_deefileiterator_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deefileiterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deefileiterator_tp_str),
  member(&_deefileiterator_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deefileiterator_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,
  member(&_deefileiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deefileiterator_tp_member),null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deefileiterator_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_FILE_ITERATOR_INL */
