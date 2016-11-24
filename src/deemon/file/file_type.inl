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
#ifndef GUARD_DEEMON_FILE_TYPE_INL
#define GUARD_DEEMON_FILE_TYPE_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/file.h>
#include <deemon/memberdef.h>
#include <deemon/structured.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>

// */ (nano...)

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// Virtual functions of the stub base class 'file'
static void _DeeFileType_InitCommon(DeeFileTypeObject *self) {
 self->tp_io.tp_io_read = NULL;
 self->tp_io.tp_io_write = NULL;
 self->tp_io.tp_io_seek = NULL;
 self->tp_io.tp_io_flush = NULL;
 self->tp_io.tp_io_trunc = NULL;
 self->tp_io.tp_io_close = NULL;
}
static int _deefiletype_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileTypeObject *self) {
 _DeeFileType_InitCommon(self);
 return (*DeeType_GET_SLOT(&DeeType_Type,tp_ctor))(&DeeType_Type,(DeeObject *)self);
}
static int _deefiletype_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileTypeObject *self, DeeFileTypeObject *right) {
 memcpy(&self->tp_io,&right->tp_io,sizeof(struct _DeeFileTypeIOOperators));
 return (*DeeType_GET_SLOT(&DeeType_Type,tp_copy_ctor))(
  &DeeType_Type,(DeeObject *)self,(DeeObject *)right);
}
static int _deefiletype_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileTypeObject *self, DeeFileTypeObject *right) {
 memcpy(&self->tp_io,&right->tp_io,sizeof(struct _DeeFileTypeIOOperators));
 return (*DeeType_GET_SLOT(&DeeType_Type,tp_move_ctor))(
  &DeeType_Type,(DeeObject *)self,(DeeObject *)right);
}
static int _deefiletype_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeFileTypeObject *self, DeeObject *args) {
 _DeeFileType_InitCommon(self);
 return (*DeeType_GET_SLOT(&DeeType_Type,tp_any_ctor))(&DeeType_Type,(DeeObject *)self,args);
}

extern int DEE_CALL _deefiletype_tp_marshal_ctor(
 DeeTypeObject *tp_self, DeeFileTypeObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 if DEE_UNLIKELY(_deetype_tp_marshal_ctor(tp_self,(DeeTypeObject *)self,infile,map) != 0) return -1;
 _DeeFileType_InitCommon(self);
 return 0;
}

static struct DeeTypeMarshal _deefiletype_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALID_NONE),
 member(&_deefiletype_tp_marshal_ctor),member(&_deetype_tp_marshal_put));


#if DEE_XCONFIG_HAVE_FILE_TYPE_SLOT_MEMBERS
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DEE_FILETYPESLOT_GETSET(name,tystring) \
static DeeObject *_deefiletype_##name##_get(DeeFileTypeObject *self, void *DEE_UNUSED(closure))\
{\
 return DeePointer_Newf((void *)DeeType_GET_SLOT(self,name),tystring);\
}\
static int _deefiletype_##name##_del(DeeFileTypeObject *self, void *DEE_UNUSED(closure))\
{\
 DeeType_DEL_SLOT(self,name);\
 return 0;\
}\
static int _deefiletype_##name##_set(DeeFileTypeObject *self, DeeObject *v, void *DEE_UNUSED(closure))\
{\
 DeeType_SLOT_TYPE(name) new_v;\
 if DEE_UNLIKELY(DeeObject_GetPointerExf(v,(void **)&new_v,tystring) != 0) return -1;\
 DeeType_GET_SLOT(self,name) = new_v;\
 return 0;\
}
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
DEE_COMPILER_MSVC_WARNING_PUSH(4052 4054)
DEE_FILETYPESLOT_GETSET(tp_io_read,"d(opII*)")
DEE_FILETYPESLOT_GETSET(tp_io_write,"d(opII*)")
DEE_FILETYPESLOT_GETSET(tp_io_seek,"d(oI64ddI64u*)")
DEE_FILETYPESLOT_GETSET(tp_io_flush,"d(o)")
DEE_FILETYPESLOT_GETSET(tp_io_trunc,"d(o)")
DEE_FILETYPESLOT_GETSET(tp_io_close,"v(o)")
DEE_COMPILER_MSVC_WARNING_POP
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#undef DEE_FILETYPESLOT_GETSET
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#endif /* DEE_XCONFIG_HAVE_FILE_TYPE_SLOT_MEMBERS */

#if DEE_XCONFIG_HAVE_FILE_TYPE_SLOT_MEMBERS
static struct DeeGetSetDef _deefiletype_tp_getsets[] = {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DEE_FILETYPESLOT_GETSET_REF(name,tydoc)\
 DEE_GETSETDEF_v100("__" #name,\
  member(&_deefiletype_##name##_get),\
  member(&_deefiletype_##name##_del),\
  member(&_deefiletype_##name##_set),\
  "-> " tydoc)
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
 DEE_FILETYPESLOT_GETSET_REF(tp_io_read,"int(*)(object,none *,size_t,size_t *)"),
 DEE_FILETYPESLOT_GETSET_REF(tp_io_write,"int(*)(object,none *,size_t,size_t *)"),
 DEE_FILETYPESLOT_GETSET_REF(tp_io_seek,"int(*)(object,int64_t,int,uint64_t *)"),
 DEE_FILETYPESLOT_GETSET_REF(tp_io_flush,"int(*)(object)"),
 DEE_FILETYPESLOT_GETSET_REF(tp_io_trunc,"int(*)(object)"),
 DEE_FILETYPESLOT_GETSET_REF(tp_io_close,"none(*)(object)"),
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#undef DEE_FILETYPESLOT_GETSET_REF
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DEE_GETSETDEF_END_v100
};
#else /* DEE_XCONFIG_HAVE_FILE_TYPE_SLOT_MEMBERS */
#define _deefiletype_tp_getsets DeeType_DEFAULT_SLOT(tp_getsets)
#endif /* !DEE_XCONFIG_HAVE_FILE_TYPE_SLOT_MEMBERS */


DeeTypeObject DeeFileType_Type = {
 // Type for the file type and all its derived types
 // NOTE: This is a type_type that is simply required
 //       because it adds more virtual operators!
 DEE_TYPE_OBJECT_HEAD_v100(member("file_type"),member(
  "(...) -> file_type\n"
  "\nType for all file types.\n"
  "\tInherits from #type and adds the r/w functions used by files."),null,member(&DeeType_Type)),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeFileTypeObject),null,
  member(&_deefiletype_tp_ctor),
  member(&_deefiletype_tp_copy_ctor),
  member(&_deefiletype_tp_move_ctor),
  member(&_deefiletype_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,member(_deefiletype_tp_getsets),null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deefiletype_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};

DEE_DECL_END

#endif /* !GUARD_DEEMON_FILE_TYPE_INL */
