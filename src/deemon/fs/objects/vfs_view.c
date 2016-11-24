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
#if DEE_CONFIG_RUNTIME_HAVE_VFS2
#include <deemon/bool.h>
#include <deemon/error.h>
#include <deemon/fs/objects/vfs_view.h>
#include <deemon/generic_vtable.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/string.h>
#include <deemon/tuple.h>

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT_REF DeeVFSViewObject *
DeeVFSView_OpenNode(DEE_A_INOUT struct DeeVFSNode *node) {
 DeeVFSViewObject *result; Dee_size_t view_size;
 if (!DeeVFSNode_HasView(node)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Can't view virtual node %R",
                      DeeVFSNode_Filename(node));
  return NULL;
 }
 // Make sure we allocate enough memory for the view buffer
 view_size = Dee_OFFSETOF(DeeVFSViewObject,vv_view)+
             _DeeVFSNode_GetViewBufferSize(node);
 if DEE_UNLIKELY((result = (DeeVFSViewObject *)DeeObject_Malloc(view_size)) == NULL) return NULL;
 // Open the buffer
 if DEE_UNLIKELY(_DeeVFSNode_OpenViewBuffer(node,&result->vv_view) != 0) {
  DeeObject_Free(result);
  return NULL;
 }
 // Now just initialize the regular object data.
 DeeObject_INIT(result,&DeeVFSView_Type);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeVFSViewObject *
DeeVFSView_TryOpenNode(DEE_A_INOUT struct DeeVFSNode *node) {
 DeeVFSViewObject *result; Dee_size_t view_size;
 if (!DeeVFSNode_HasView(node)) return NULL;
 // Make sure we allocate enough memory for the view buffer
 view_size = Dee_OFFSETOF(DeeVFSViewObject,vv_view)+
             _DeeVFSNode_GetViewBufferSize(node);
 if DEE_UNLIKELY((result = (DeeVFSViewObject *)DeeObject_TryMalloc(view_size)) == NULL) return NULL;
 // Open the buffer
 if DEE_UNLIKELY(_DeeVFSNode_OpenViewBuffer(node,&result->vv_view) != 0) {
  DeeError_HandledOne();
  DeeObject_Free(result);
  return NULL;
 }
 // Now just initialize the regular object data.
 DeeObject_INIT(result,&DeeVFSView_Type);
 return result;
}

static DeeVFSViewObject *DEE_CALL _deevfsview_tp_any_new(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *args) {
 DeeVFSViewObject *result; DeeObject *path; struct DeeVFSNode *node;
 if (DeeTuple_Unpack(args,"o:vfs_view",&path) != 0) return NULL;
 if ((path = DeeString_Cast(path)) == NULL) return NULL;
 node = DeeVFS_Locate(DeeString_STR(path));
 Dee_DECREF(path);
 if (!node) return NULL;
 result = DeeVFSView_OpenNode(node);
 DeeVFSNode_DECREF(node);
 return result;
}
static void DEE_CALL _deevfsview_tp_dtor(DeeVFSViewObject *self) {
 DeeVFSView_Quit(&self->vv_view);
}

static DeeObject *DEE_CALL _deevfsview_tp_str(DeeVFSViewObject *self) {
 DeeObject *path,*file,*result;
 if DEE_UNLIKELY((path = DeeVFSView_GetPath(&self->vv_view)) == NULL) return NULL;
 if DEE_UNLIKELY((file = DeeVFSView_GetFilenameObject(&self->vv_view)) == NULL) { Dee_DECREF(path); return NULL; }
 result = DeeString_Newf("<vfs_view %r -> %r>",path,file);
 Dee_DECREF(file);
 Dee_DECREF(path);
 return result;
}
static DeeObject *DEE_CALL _deevfsview_tp_repr(DeeVFSViewObject *self) {
 DeeObject *path,*result;
 if DEE_UNLIKELY((path = DeeVFSView_GetPath(&self->vv_view)) == NULL) return NULL;
 result = DeeString_Newf("vfs_view(%r)",path);
 Dee_DECREF(path);
 return result;
}
static int DEE_CALL _deevfsview_tp_bool(DeeVFSViewObject *self) {
 struct DeeVFSNode *node; int error;
 DEE_ASSERTF(self->vv_view.vv_type->vnt_view,"How did you create file view?");
 if (!self->vv_view.vv_type->vnt_view->vvt_curr) return 0;
 error = (*self->vv_view.vv_type->vnt_view->vvt_curr)((struct DeeVFSView *)self,&node);
 if (error == 0) { DeeVFSNode_DECREF(node); return 1; }
 return DEE_UNLIKELY(error < 0) ? error : 0;
}
static DeeObject *DEE_CALL _deevfsview_tp_not(DeeVFSViewObject *self) {
 struct DeeVFSNode *node; int error;
 DEE_ASSERTF(self->vv_view.vv_type->vnt_view,"How did you create file view?");
 if (!self->vv_view.vv_type->vnt_view->vvt_curr) DeeReturn_True;
 error = (*self->vv_view.vv_type->vnt_view->vvt_curr)((struct DeeVFSView *)self,&node);
 if (error == 0) { DeeVFSNode_DECREF(node); DeeReturn_False; }
 if DEE_UNLIKELY(error < 0) return NULL;
 DeeReturn_True;
}
static int DEE_CALL _deevfsview_tp_seq_iter_next(
 DeeVFSViewObject *self, DeeObject **result) {
 int error; struct DeeVFSNode *node;
 DEE_ASSERTF(self->vv_view.vv_type->vnt_view,"How did you create file view?");
 if DEE_UNLIKELY(!self->vv_view.vv_type->vnt_view->vvt_yield) return 1;
 error = (*self->vv_view.vv_type->vnt_view->vvt_yield)(&self->vv_view,&node);
 if DEE_UNLIKELY(error != 0) return error;
 *result = DeeVFSNode_Name(node);
 DeeVFSNode_DECREF(node);
 if DEE_UNLIKELY(!*result) return -1;
 return 0;
}


DeeTypeObject DeeVFSView_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("vfs_view"),null,member(
  DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|
  DEE_TYPE_FLAG_NO_SUBCLASS|DEE_TYPE_FLAG_VAR_OBJECT),null),
 DEE_TYPE_OBJECT_VAR_CONSTRUCTOR_v101(
  null,null,null,member(&_deevfsview_tp_any_new)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deevfsview_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deevfsview_tp_str),member(&_deevfsview_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deevfsview_tp_not),member(&_deevfsview_tp_bool),
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,
  member(&_DeeGeneric_ReturnSelf),
  member(&_deevfsview_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(null),
 DEE_TYPE_OBJECT_FOOTER_v101
};

DEE_DECL_END
#endif

#endif /* !GUARD_DEEMON_FS_VFS_VIEW_C */
