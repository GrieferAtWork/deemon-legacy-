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
#ifndef GUARD_DEEMON_FILE_JOINED_INL
#define GUARD_DEEMON_FILE_JOINED_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/file.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_visit.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>
#include DEE_INCLUDE_MEMORY_API()

// */ (nano...)

DEE_DECL_BEGIN


struct DeeFileJoinedObject {
 DEE_FILE_OBJECT_HEAD
 DEE_A_REF DeeFileObject *fj_fa; /*< [1..1] First output stream. */
 DEE_A_REF DeeFileObject *fj_fb; /*< [1..1] Second output stream. */
};
#define DeeFileJoined_FA(ob) ((DeeObject *)((DeeFileJoinedObject *)Dee_REQUIRES_POINTER(ob))->fj_fa)
#define DeeFileJoined_FB(ob) ((DeeObject *)((DeeFileJoinedObject *)Dee_REQUIRES_POINTER(ob))->fj_fb)


DEE_A_RET_OBJECT_EXCEPT_REF(DeeFileJoinedObject) *DeeFileJoined_New(
 DEE_A_INOUT_OBJECT(DeeFileObject) *fa, DEE_A_INOUT_OBJECT(DeeFileObject) *fb) {
 DeeFileJoinedObject *result;
 DEE_ASSERT(DeeObject_Check(fa) && DeeFile_Check(fa));
 DEE_ASSERT(DeeObject_Check(fb) && DeeFile_Check(fb));
 if DEE_LIKELY((result = DeeObject_MALLOCF(
  DeeFileJoinedObject,"file.joined(%p,%p)",fa,fb)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)&DeeFileJoined_Type);
  _DeeFile_Init(result);
  Dee_INCREF(result->fj_fa = (DeeFileObject *)fa);
  Dee_INCREF(result->fj_fb = (DeeFileObject *)fb);
 }
 return (DeeObject *)result;
}






//////////////////////////////////////////////////////////////////////////
// file.joined VTable
#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deefilejoined_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deefilejoined_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__fj_fa",DeeFileJoinedObject,fj_fa,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__fj_fb",DeeFileJoinedObject,fj_fb,object),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static int _deefilejoined_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileJoinedObject *self) {
 _DeeFile_Init(self);
 if DEE_UNLIKELY((self->fj_fa = (DeeFileObject *)DeeType_NewInstanceDefault(
  (DeeTypeObject *)&DeeFile_Type)) == NULL) return -1;
 Dee_INCREF(self->fj_fb = self->fj_fa);
 return 0;
}
static void _deefilejoined_tp_dtor(DeeFileJoinedObject *self) {
 Dee_DECREF(self->fj_fa);
 Dee_DECREF(self->fj_fb);
}
DEE_VISIT_PROC(_deefilejoined_tp_visit,DeeFileJoinedObject *self) {
 DeeFile_ACQUIRE(self);
 Dee_VISIT(self->fj_fa);
 Dee_VISIT(self->fj_fb);
 DeeFile_RELEASE(self);
}
static int _deefilejoined_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileJoinedObject *self, DeeFileJoinedObject *right) {
 _DeeFile_Init(self);
 DeeFile_ACQUIRE(right);
 Dee_INCREF(self->fj_fa = right->fj_fa);
 Dee_INCREF(self->fj_fb = right->fj_fb);
 DeeFile_RELEASE(right);
 return 0;
}
static int _deefilejoined_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileJoinedObject *self, DeeObject *args) {
 self->fj_fa = self->fj_fb = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|oo:file.joined",&self->fj_fa,&self->fj_fb) != 0) return -1;
 if (DEE_LIKELY(self->fj_fa) && DEE_UNLIKELY((self->fj_fa = (DeeFileObject *)DeeObject_GetInstance((DeeObject *)self->fj_fa,(DeeTypeObject *)&DeeFile_Type)) == NULL)) return -1;
 if (DEE_LIKELY(self->fj_fb) && DEE_UNLIKELY((self->fj_fb = (DeeFileObject *)DeeObject_GetInstance((DeeObject *)self->fj_fb,(DeeTypeObject *)&DeeFile_Type)) == NULL)) return -1;
 _DeeFile_Init(self);
 if DEE_LIKELY(self->fj_fa) Dee_INCREF(self->fj_fa);
 else self->fj_fa = (DeeFileObject *)DeeFile_Std(DEE_STDNULL);
 if DEE_LIKELY(self->fj_fb) Dee_INCREF(self->fj_fb);
 else self->fj_fb = (DeeFileObject *)DeeFile_Std(DEE_STDNULL);
 return 0;
}
static int _deefilejoined_tp_copy_assign(
 DeeFileJoinedObject *self, DeeFileJoinedObject *right) {
 if DEE_LIKELY(self != right) {
  DeeFileObject *old_fa,*old_fb,*new_fa,*new_fb;
  Dee_uint32_t new_flags;
  DeeFile_ACQUIRE(right);
  new_flags = right->fo_flags;
  Dee_INCREF(new_fa = right->fj_fa);
  Dee_INCREF(new_fb = right->fj_fb);
  DeeFile_RELEASE(right);
  DeeFile_ACQUIRE(self);
  self->fo_flags = new_flags;
  old_fa = self->fj_fa;
  old_fb = self->fj_fb;
  Dee_INHERIT_REF(self->fj_fa,new_fa);
  Dee_INHERIT_REF(self->fj_fb,new_fb);
  DeeFile_RELEASE(self);
  Dee_DECREF(old_fa);
  Dee_DECREF(old_fb);
 }
 return 0;
}
static DeeObject *_deefilejoined_tp_str(DeeFileJoinedObject *self) {
 DeeFileObject *fa,*fb;
 DeeFile_ACQUIRE(self);
 Dee_INCREF(fa = self->fj_fa);
 Dee_INCREF(fb = self->fj_fb);
 DeeFile_RELEASE(self);
 return DeeString_Newf("<file.joined {%K, %K}>",self->fj_fa,self->fj_fb);
}
static DeeObject *_deefilejoined_tp_repr(DeeFileJoinedObject *self) {
 DeeFileObject *fa,*fb;
 DeeFile_ACQUIRE(self);
 Dee_INCREF(fa = self->fj_fa);
 Dee_INCREF(fb = self->fj_fb);
 DeeFile_RELEASE(self);
 return DeeString_Newf("<file.joined {%R, %R}>",self->fj_fa,self->fj_fb);
}
static int _deefilejoined_tp_io_read(
 DeeFileJoinedObject *DEE_UNUSED(self), void *DEE_UNUSED(p),
 Dee_size_t DEE_UNUSED(s), Dee_size_t *DEE_UNUSED(rs)) {
 DeeError_SET_STRING(&DeeErrorType_IOError,"file.joined is a write-only stream");
 return -1;
}
DEE_COMPILER_MSVC_WARNING_PUSH(4701)
DEE_COMPILER_PREFAST_WARNING_PUSH(6001)
static int _deefilejoined_tp_io_write(
 DeeFileJoinedObject *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 Dee_size_t wsa,wsb; DeeFileObject *fa,*fb; int result;
 DeeFile_ACQUIRE(self);
 Dee_INCREF(fa = self->fj_fa);
 Dee_INCREF(fb = self->fj_fb);
 DeeFile_RELEASE(self);
 result = DeeFile_Write((DeeObject *)fa,p,s,&wsa);
 if DEE_LIKELY(result == 0) result = DeeFile_Write((DeeObject *)fb,p,s,&wsb);
 Dee_DECREF(fa);
 Dee_DECREF(fb);
 if DEE_LIKELY(result == 0) *ws = wsa > wsb ? wsa : wsb; // max
 return result;
}
DEE_COMPILER_PREFAST_WARNING_POP
DEE_COMPILER_MSVC_WARNING_POP

DEE_COMPILER_MSVC_WARNING_PUSH(4701)
DEE_COMPILER_PREFAST_WARNING_PUSH(6001)
static int _deefilejoined_tp_io_seek(
 DeeFileJoinedObject *self, Dee_int64_t off, int whence, Dee_uint64_t *pos) {
 Dee_uint64_t posa,posb; DeeFileObject *fa,*fb; int result;
 DeeFile_ACQUIRE(self);
 Dee_INCREF(fa = self->fj_fa);
 Dee_INCREF(fb = self->fj_fb);
 DeeFile_RELEASE(self);
 if (pos) {
  result = DeeFile_Seek((DeeObject *)fa,off,whence,&posa);
  Dee_DECREF(fa);
  if DEE_LIKELY(result == 0) result = DeeFile_Seek((DeeObject *)fb,off,whence,&posb);
  Dee_DECREF(fb);
  if DEE_LIKELY(result == 0) {
   if DEE_UNLIKELY(posa != posb) {
    DeeError_SetStringf(&DeeErrorType_IOError,
                        "Can't determinate file.joined position (%I64u, %I64u)",
                        posa,posb);
    result = -1;
   } else {
    *pos = posa;
    result = 0;
   }
  }
 } else {
  result = DeeFile_Seek((DeeObject *)fa,off,whence,&posa);
  Dee_DECREF(fa);
  if DEE_LIKELY(result == 0) result = DeeFile_Seek((DeeObject *)fb,off,whence,&posb);
  Dee_DECREF(fb);
 }
 Dee_DECREF(fb);
 Dee_DECREF(fa);
 return result;
}
DEE_COMPILER_PREFAST_WARNING_POP
DEE_COMPILER_MSVC_WARNING_POP

static int _deefilejoined_tp_io_flush(DeeFileJoinedObject *self) {
 DeeFileObject *fa,*fb; int result;
 DeeFile_ACQUIRE(self);
 Dee_INCREF(fa = self->fj_fa);
 Dee_INCREF(fb = self->fj_fb);
 DeeFile_RELEASE(self);
 result = DeeFile_Flush((DeeObject *)self->fj_fa);
 Dee_DECREF(fa);
 if DEE_LIKELY(result == 0) result = DeeFile_Flush((DeeObject *)self->fj_fb);
 Dee_DECREF(fb);
 return result;
}
static int _deefilejoined_tp_io_trunc(DeeFileJoinedObject *self) {
 DeeFileObject *fa,*fb; int result;
 DeeFile_ACQUIRE(self);
 Dee_INCREF(fa = self->fj_fa);
 Dee_INCREF(fb = self->fj_fb);
 DeeFile_RELEASE(self);
 result = DeeFile_Trunc((DeeObject *)self->fj_fa);
 Dee_DECREF(fa);
 if DEE_LIKELY(result == 0) result = DeeFile_Trunc((DeeObject *)self->fj_fb);
 Dee_DECREF(fb);
 return result;
}
static void _deefilejoined_tp_io_close(DeeFileJoinedObject *self) {
 DeeFileObject *old_fa,*old_fb;
 DeeFile_ACQUIRE(self);
 Dee_INHERIT_REF(old_fa,self->fj_fa);
 Dee_INHERIT_REF(old_fb,self->fj_fb);
 self->fj_fa = (DeeFileObject *)DeeFile_Std(DEE_STDNULL);
 Dee_INCREF(self->fj_fb = self->fj_fa);
 DeeFile_RELEASE(self);
 Dee_DECREF(old_fa);
 Dee_DECREF(old_fb);
}
static int DEE_CALL _deefilejoined_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileJoinedObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 if DEE_UNLIKELY((self->fj_fa = (DeeFileObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if DEE_UNLIKELY(DeeError_TypeError_CheckType((DeeObject *)self->fj_fa,(DeeTypeObject *)&DeeFile_Type) != 0) {err_fa: Dee_DECREF(self->fj_fa); return -1; }
 if DEE_UNLIKELY((self->fj_fb = (DeeFileObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) goto err_fa;
 if DEE_UNLIKELY(DeeError_TypeError_CheckType((DeeObject *)self->fj_fb,(DeeTypeObject *)&DeeFile_Type) != 0) {/*err_fb:*/ Dee_DECREF(self->fj_fb); goto err_fa; }
 _DeeFile_Init(self);
 return 0;
}
static int DEE_CALL _deefilejoined_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileJoinedObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 if (DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)self->fj_fa) != 0) return -1;
 return DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)self->fj_fb);
}
static struct DeeTypeMarshal _deefilejoined_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(file_joined)),
 member(&_deefilejoined_tp_marshal_ctor),member(&_deefilejoined_tp_marshal_put));




DeeFileTypeObject DeeFileJoined_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeFileType_Type),member("file.joined"),member(
   "(file a, file b) -> file.joined\n"
   "\tA file type that can be used to write into two other files at once."),
   null,member((DeeTypeObject *)&DeeFile_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFileJoinedObject),null,
   member(&_deefilejoined_tp_ctor),
   member(&_deefilejoined_tp_copy_ctor),null,
   member(&_deefilejoined_tp_any_ctor)),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
   member(&_deefilejoined_tp_dtor)),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(&_deefilejoined_tp_copy_assign),null,null),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&_deefilejoined_tp_str),
   member(&_deefilejoined_tp_repr),null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,
   member(&_deefilejoined_tp_visit)),
  DEE_TYPE_OBJECT_MATH_v101(
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
   member(_deefilejoined_tp_members),null,null,null,null,null),
  DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deefilejoined_tp_marshal)),
  DEE_TYPE_OBJECT_FOOTER_v101
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(
  member(&_deefilejoined_tp_io_read),
  member(&_deefilejoined_tp_io_write),
  member(&_deefilejoined_tp_io_seek),
  member(&_deefilejoined_tp_io_flush),
  member(&_deefilejoined_tp_io_trunc),
  member(&_deefilejoined_tp_io_close))
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};

DEE_DECL_END

#endif /* !GUARD_DEEMON_FILE_JOINED_INL */
