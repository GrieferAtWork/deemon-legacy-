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
#ifndef GUARD_DEEMON_FS_VFS_VIEW_C
#define GUARD_DEEMON_FS_VFS_VIEW_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include <deemon/bool.h>
#include <deemon/error.h>
#include <deemon/fs/objects/native_view.h>
#include <deemon/generic_vtable.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/string.h>
#include <deemon/tuple.h>

DEE_DECL_BEGIN

/*[[[deemon
#include <file>
const begin_tag = "@DeeSysFSWideView:begin";
const end_tag   = "@DeeSysFSWideView:end";
text = file.open(__FILE__,"r").read();
begin = text.rindex(begin_tag)+#begin_tag;
end = text.rindex(end_tag,begin);
text = text[begin:end];

text = text.replace("wide","utf8");
text = text.replace("Wide","Utf8");
text = text.replace("WIDE","UTF8");
text = text.strip().rsstrip("//").strip();
print text;
]]]*/
#ifdef DeeNFSUtf8View
DEE_A_RET_EXCEPT_REF DeeNFSUtf8ViewObject *
DeeNFSUtf8View_OpenPath(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DeeNFSUtf8ViewObject *result;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeNFSUtf8ViewObject)) == NULL) return NULL;
 DeeNFSUtf8View_Init(&result->nwv_view,path,{ DeeObject_Free(result); return NULL; });
 DeeObject_INIT(result,&DeeNFSUtf8View_Type);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeNFSUtf8ViewObject *
DeeNFSUtf8View_OpenPathObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
 DeeNFSUtf8ViewObject *result;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeNFSUtf8ViewObject)) == NULL) return NULL;
 DeeNFSUtf8View_InitObject(&result->nwv_view,path,{ DeeObject_Free(result); return NULL; });
 DeeObject_INIT(result,&DeeNFSUtf8View_Type);
 return result;
}
static int DEE_CALL _deenfsutf8view_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeNFSUtf8ViewObject *self, DeeObject *args) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:native_utf8view",&path) != 0) return -1;
 if DEE_UNLIKELY((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 DeeNFSUtf8View_InitObject(&self->nwv_view,path,{ Dee_DECREF(path); return -1; });
 Dee_DECREF(path);
 return 0;
}
static void DEE_CALL _deenfsutf8view_tp_dtor(DeeNFSUtf8ViewObject *self) {
 DeeNFSUtf8View_Quit(&self->nwv_view);
}
static int DEE_CALL _deenfsutf8view_tp_seq_iter_next(
 DeeNFSUtf8ViewObject *self, DeeObject **result) {
again:
 DeeNFSUtf8View_Acquire(&self->nwv_view);
 if (DeeNFSUtf8View_IsDone(&self->nwv_view)) { DeeNFSUtf8View_Release(&self->nwv_view); return 1; }
 if ((*result = DeeNFSUtf8View_TryGetFilenameObjectLocked(&self->nwv_view)) == NULL) {
  DeeNFSUtf8View_Release(&self->nwv_view);
  if (Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return -1;
 }
 DeeNFSUtf8View_AdvanceAndReleaseOnError(&self->nwv_view,return -1);
 DeeNFSUtf8View_Release(&self->nwv_view);
 return 0;
}

static DeeObject *DEE_CALL _deenfsutf8view_tp_str(DeeNFSUtf8ViewObject *self) {
 DeeObject *path,*file,*result;
 if DEE_UNLIKELY((path = DeeNFSUtf8View_GetPath(&self->nwv_view)) == NULL) return NULL;
 if DEE_UNLIKELY((file = DeeNFSUtf8View_GetFilenameObject(&self->nwv_view)) == NULL) { Dee_DECREF(path); return NULL; }
 result = DeeString_Newf("<native_utf8view %r -> %r>",path,file);
 Dee_DECREF(file);
 Dee_DECREF(path);
 return result;
}
static DeeObject *DEE_CALL _deenfsutf8view_tp_repr(DeeNFSUtf8ViewObject *self) {
 DeeObject *path,*result;
 if DEE_UNLIKELY((path = DeeNFSUtf8View_GetPath(&self->nwv_view)) == NULL) return NULL;
 result = DeeString_Newf("native_utf8view(%r)",path);
 Dee_DECREF(path);
 return result;
}
static int DEE_CALL _deenfsutf8view_tp_bool(DeeNFSUtf8ViewObject *self) {
 int result;
 DeeNFSUtf8View_Acquire(&self->nwv_view);
 result = !DeeNFSUtf8View_IsDone(&self->nwv_view);
 DeeNFSUtf8View_Release(&self->nwv_view);
 return result;
}
static DeeObject *DEE_CALL _deenfsutf8view_tp_not(DeeNFSUtf8ViewObject *self) {
 int result;
 DeeNFSUtf8View_Acquire(&self->nwv_view);
 result = DeeNFSUtf8View_IsDone(&self->nwv_view);
 DeeNFSUtf8View_Release(&self->nwv_view);
 DeeReturn_Bool(result);
}


DeeTypeObject DeeNFSUtf8View_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("native_utf8view"),null,member(
  DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeNFSUtf8ViewObject),
  null,null,null,null,member(&_deenfsutf8view_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deenfsutf8view_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deenfsutf8view_tp_str),
  member(&_deenfsutf8view_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deenfsutf8view_tp_not),member(&_deenfsutf8view_tp_bool),
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,
  member(&_DeeGeneric_ReturnSelf),
  member(&_deenfsutf8view_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(null),
 DEE_TYPE_OBJECT_FOOTER_v101
};

#endif
//[[[end]]]


//@DeeSysFSWideView:begin
#ifdef DeeNFSWideView
DEE_A_RET_EXCEPT_REF DeeNFSWideViewObject *
DeeNFSWideView_OpenPath(DEE_A_IN_Z Dee_WideChar const *path) {
 DeeNFSWideViewObject *result;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeNFSWideViewObject)) == NULL) return NULL;
 DeeNFSWideView_Init(&result->nwv_view,path,{ DeeObject_Free(result); return NULL; });
 DeeObject_INIT(result,&DeeNFSWideView_Type);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeNFSWideViewObject *
DeeNFSWideView_OpenPathObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 DeeNFSWideViewObject *result;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeNFSWideViewObject)) == NULL) return NULL;
 DeeNFSWideView_InitObject(&result->nwv_view,path,{ DeeObject_Free(result); return NULL; });
 DeeObject_INIT(result,&DeeNFSWideView_Type);
 return result;
}
static int DEE_CALL _deenfswideview_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeNFSWideViewObject *self, DeeObject *args) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:native_wideview",&path) != 0) return -1;
 if DEE_UNLIKELY((path = DeeWideString_Cast(path)) == NULL) return -1;
 DeeNFSWideView_InitObject(&self->nwv_view,path,{ Dee_DECREF(path); return -1; });
 Dee_DECREF(path);
 return 0;
}
static void DEE_CALL _deenfswideview_tp_dtor(DeeNFSWideViewObject *self) {
 DeeNFSWideView_Quit(&self->nwv_view);
}
static int DEE_CALL _deenfswideview_tp_seq_iter_next(
 DeeNFSWideViewObject *self, DeeObject **result) {
again:
 DeeNFSWideView_Acquire(&self->nwv_view);
 if (DeeNFSWideView_IsDone(&self->nwv_view)) { DeeNFSWideView_Release(&self->nwv_view); return 1; }
 if ((*result = DeeNFSWideView_TryGetFilenameObjectLocked(&self->nwv_view)) == NULL) {
  DeeNFSWideView_Release(&self->nwv_view);
  if (Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return -1;
 }
 DeeNFSWideView_AdvanceAndReleaseOnError(&self->nwv_view,return -1);
 DeeNFSWideView_Release(&self->nwv_view);
 return 0;
}

static DeeObject *DEE_CALL _deenfswideview_tp_str(DeeNFSWideViewObject *self) {
 DeeObject *path,*file,*result;
 if DEE_UNLIKELY((path = DeeNFSWideView_GetPath(&self->nwv_view)) == NULL) return NULL;
 if DEE_UNLIKELY((file = DeeNFSWideView_GetFilenameObject(&self->nwv_view)) == NULL) { Dee_DECREF(path); return NULL; }
 result = DeeString_Newf("<native_wideview %r -> %r>",path,file);
 Dee_DECREF(file);
 Dee_DECREF(path);
 return result;
}
static DeeObject *DEE_CALL _deenfswideview_tp_repr(DeeNFSWideViewObject *self) {
 DeeObject *path,*result;
 if DEE_UNLIKELY((path = DeeNFSWideView_GetPath(&self->nwv_view)) == NULL) return NULL;
 result = DeeString_Newf("native_wideview(%r)",path);
 Dee_DECREF(path);
 return result;
}
static int DEE_CALL _deenfswideview_tp_bool(DeeNFSWideViewObject *self) {
 int result;
 DeeNFSWideView_Acquire(&self->nwv_view);
 result = !DeeNFSWideView_IsDone(&self->nwv_view);
 DeeNFSWideView_Release(&self->nwv_view);
 return result;
}
static DeeObject *DEE_CALL _deenfswideview_tp_not(DeeNFSWideViewObject *self) {
 int result;
 DeeNFSWideView_Acquire(&self->nwv_view);
 result = DeeNFSWideView_IsDone(&self->nwv_view);
 DeeNFSWideView_Release(&self->nwv_view);
 DeeReturn_Bool(result);
}


DeeTypeObject DeeNFSWideView_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("native_wideview"),null,member(
  DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeNFSWideViewObject),
  null,null,null,null,member(&_deenfswideview_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deenfswideview_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deenfswideview_tp_str),
  member(&_deenfswideview_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deenfswideview_tp_not),member(&_deenfswideview_tp_bool),
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,
  member(&_DeeGeneric_ReturnSelf),
  member(&_deenfswideview_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(null),
 DEE_TYPE_OBJECT_FOOTER_v101
};

#endif
//@DeeSysFSWideView:end

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_VFS_VIEW_C */
