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

#ifdef DeeSysFSUtf8View
struct DeeNFSUtf8ViewObject {
 DEE_OBJECT_HEAD
 struct DeeNFSUtf8View nwv_view;
};

extern DeeTypeObject DeeNFSUtf8View_Type;

extern DEE_A_RET_EXCEPT_REF DeeNFSUtf8ViewObject *
 DeeNFSUtf8View_OpenPath(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_REF DeeNFSUtf8ViewObject *
 DeeNFSUtf8View_OpenPathObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
#endif

#ifdef DeeNFSWideView

DEE_A_RET_EXCEPT_REF DeeNFSWideViewObject *
DeeNFSWideView_OpenPath(DEE_A_IN_Z Dee_WideChar const *path) {
 DeeNFSWideViewObject *result;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeNFSWideViewObject)) == NULL) return NULL;
 DeeNFSWideView_Init(&result->nwv_view,path,goto end,{ DeeObject_Free(result); return NULL; });
end: DeeObject_INIT(result,&DeeNFSWideView_Type);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeNFSWideViewObject *
DeeNFSWideView_OpenPathObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 DeeNFSWideViewObject *result;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeNFSWideViewObject)) == NULL) return NULL;
 DeeNFSWideView_InitObject(&result->nwv_view,path,goto end,{ DeeObject_Free(result); return NULL; });
end: DeeObject_INIT(result,&DeeNFSWideView_Type);
 return result;
}
static int DEE_CALL _deenfswideview_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeNFSWideViewObject *self, DeeObject *args) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:native_wideview",&path) != 0) return -1;
 if DEE_UNLIKELY((path = DeeWideString_Cast(path)) == NULL) return -1;
 DeeNFSWideView_InitObject(&self->nwv_view,path,goto end,{ Dee_DECREF(path); return -1; });
end: Dee_DECREF(path);
 return 0;
}
static int DEE_CALL _deenfswideview_tp_seq_iter_next(
 DeeNFSWideViewObject *self, DeeObject **result) {
 // TODO: No way to safely implement this
 // >> OK! I designed "sys/sysfs.view" incorretly. ('ll have to redo it)
 (void)self,result;
 DeeError_NotImplemented();
 return -1;
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





DeeTypeObject DeeNFSWideView_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("native_wideview"),null,member(
  DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeNFSWideViewObject),
  null,null,null,null,member(&_deenfswideview_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deenfswideview_tp_str),
  member(&_deenfswideview_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,null,null),
 DEE_TYPE_OBJECT_MATH_v101(null,null,
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

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_VFS_VIEW_C */
