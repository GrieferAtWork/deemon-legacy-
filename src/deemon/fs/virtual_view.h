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
#ifndef GUARD_DEEMON_FS_VIRTUAL_VIEW_H
#define GUARD_DEEMON_FS_VIRTUAL_VIEW_H 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/type.h>
#include <deemon/string.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_decl.h>
#include <deemon/fs/native_fs.h>
#include <deemon/vfs/vfs_core.h>

DEE_DECL_BEGIN

#define DeeVFSView DeeVFSView
// Declared in "/src/deemon/vfs/vfs_core.h"
//struct DeeVFSView { ... };

#define DeeVFSView_GetPath(self) DeeVFSNode_Filename((self)->vv_node)
#define DeeVFSView_TryGetPath    DeeVFSView_TryGetPath
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeStringObject) *)
DeeVFSView_TryGetPath(struct DeeVFSView const *self) {
 DeeObject *result;
 if ((result = DeeVFSNode_Filename(self->vv_node)) == NULL) DeeError_HandledOne();
 return result;
}

DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT_REF struct DeeVFSNode *)
DeeVFSView_TryGetCurrentNode(struct DeeVFSView const *self) {
 struct DeeVFSNode *result; int error;
 if (!self->vv_type->vnt_view.vvt_curr) return NULL;
 error = (*self->vv_type->vnt_view.vvt_curr)((struct DeeVFSView *)self,&result);
 if (error < 0) { DeeError_HandledOne(); return NULL; }
 return error == 0 ? result : NULL;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF struct DeeVFSNode *)
DeeVFSView_GetCurrentNode(struct DeeVFSView const *self) {
 struct DeeVFSNode *result; int error;
 if (!self->vv_type->vnt_view.vvt_curr) return NULL;
 error = (*self->vv_type->vnt_view.vvt_curr)((struct DeeVFSView *)self,&result);
 if (error == 0) return result;
 if (error > 0) {
  DeeError_SET_STRING(&DeeErrorType_SystemError,
                      "No file selected");
 }
 return NULL;
}

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeStringObject) *)
DeeVFSView_TryGetFilenameObject(struct DeeVFSView const *self) {
 struct DeeVFSNode *node; DeeObject *result;
 if DEE_UNLIKELY((node = DeeVFSView_TryGetCurrentNode(self)) == NULL) return NULL;
 result = DeeVFSNode_Name(node);
 DeeVFSNode_DECREF(node);
 if DEE_UNLIKELY(!result) DeeError_HandledOne();
 return result;
}
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeStringObject) *)
DeeVFSView_GetFilenameObject(struct DeeVFSView const *self) {
 struct DeeVFSNode *node; DeeObject *result;
 if DEE_UNLIKELY((node = DeeVFSView_GetCurrentNode(self)) == NULL) return NULL;
 result = DeeVFSNode_Name(node);
 DeeVFSNode_DECREF(node);
 return result;
}

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeStringObject) *)
DeeVFSView_TryGetAbsoluteFilename(struct DeeVFSView const *self) {
 struct DeeVFSNode *node; DeeObject *result;
 if DEE_UNLIKELY((node = DeeVFSView_TryGetCurrentNode(self)) == NULL) return NULL;
 result = DeeVFSNode_Filename(node);
 DeeVFSNode_DECREF(node);
 if DEE_UNLIKELY(!result) DeeError_HandledOne();
 return result;
}
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeStringObject) *)
DeeVFSView_GetAbsoluteFilename(struct DeeVFSView const *self) {
 struct DeeVFSNode *node; DeeObject *result;
 if DEE_UNLIKELY((node = DeeVFSView_GetCurrentNode(self)) == NULL) return NULL;
 result = DeeVFSNode_Filename(node);
 DeeVFSNode_DECREF(node);
 return result;
}


#define DeeVFSView_PRIVATE_GET_ATTRIBUTE(self,node_query,result,...) \
do{\
 struct DeeVFSNode *_vv_node;\
 if DEE_UNLIKELY((_vv_node = DeeVFSView_GetCurrentNode(self)) == NULL) {__VA_ARGS__;}\
 *(result) = node_query(_vv_node);\
 DeeVFSNode_DECREF(_vv_node);\
 if DEE_UNLIKELY(*(result) < 0) {__VA_ARGS__;}\
}while(0)

#define DeeVFSView_TryIsFile DeeVFSView_TryIsFile
DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(0) int)
DeeVFSView_TryIsFile(struct DeeVFSView const *self) {
 struct DeeVFSNode *node; int result;
 if DEE_UNLIKELY((node = DeeVFSView_TryGetCurrentNode(self)) == NULL) return 0;
 result = DeeVFSNode_IsFile(node);
 DeeVFSNode_DECREF(node);
 if DEE_UNLIKELY(result < 0) DeeError_HandledOne();
 return result;
}
#define DeeVFSView_IsFile(self,result,...) \
 DeeVFSView_PRIVATE_GET_ATTRIBUTE(self,DeeVFSNode_IsFile,result,__VA_ARGS__)

#define DeeVFSView_TryIsDir DeeVFSView_TryIsDir
DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(0) int)
DeeVFSView_TryIsDir(struct DeeVFSView const *self) {
 struct DeeVFSNode *node; int result;
 if DEE_UNLIKELY((node = DeeVFSView_TryGetCurrentNode(self)) == NULL) return 0;
 result = DeeVFSNode_IsDir(node);
 DeeVFSNode_DECREF(node);
 if DEE_UNLIKELY(result < 0) DeeError_HandledOne();
 return result;
}
#define DeeVFSView_IsDir(self,result,...) \
 DeeVFSView_PRIVATE_GET_ATTRIBUTE(self,DeeVFSNode_IsDir,result,__VA_ARGS__)

#define DeeVFSView_TryIsLink DeeVFSView_TryIsLink
DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(0) int)
DeeVFSView_TryIsLink(struct DeeVFSView const *self) {
 struct DeeVFSNode *node; int result;
 if DEE_UNLIKELY((node = DeeVFSView_TryGetCurrentNode(self)) == NULL) return 0;
 result = DeeVFSNode_IsLink(node);
 DeeVFSNode_DECREF(node);
 if DEE_UNLIKELY(result < 0) DeeError_HandledOne();
 return result;
}
#define DeeVFSView_IsLink(self,result,...) \
 DeeVFSView_PRIVATE_GET_ATTRIBUTE(self,DeeVFSNode_IsLink,result,__VA_ARGS__)

#define DeeVFSView_TryIsDrive DeeVFSView_TryIsMount
#define DeeVFSView_IsDrive    DeeVFSView_IsMount
#define DeeVFSView_TryIsMount DeeVFSView_TryIsMount
DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(0) int)
DeeVFSView_TryIsMount(struct DeeVFSView const *self) {
 struct DeeVFSNode *node; int result;
 if DEE_UNLIKELY((node = DeeVFSView_TryGetCurrentNode(self)) == NULL) return 0;
 result = DeeVFSNode_IsMount(node);
 DeeVFSNode_DECREF(node);
 if DEE_UNLIKELY(result < 0) DeeError_HandledOne();
 return result;
}
#define DeeVFSView_IsMount(self,result,...) \
 DeeVFSView_PRIVATE_GET_ATTRIBUTE(self,DeeVFSNode_IsMount,result,__VA_ARGS__)

#define DeeVFSView_TryIsHidden DeeVFSView_TryIsHidden
DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(0) int)
DeeVFSView_TryIsHidden(struct DeeVFSView const *self) {
 struct DeeVFSNode *node; int result;
 if DEE_UNLIKELY((node = DeeVFSView_TryGetCurrentNode(self)) == NULL) return 0;
 result = DeeVFSNode_IsHidden(node);
 DeeVFSNode_DECREF(node);
 if DEE_UNLIKELY(result < 0) DeeError_HandledOne();
 return result;
}
#define DeeVFSView_IsHidden(self,result,...) \
 DeeVFSView_PRIVATE_GET_ATTRIBUTE(self,DeeVFSNode_IsHidden,result,__VA_ARGS__)

#define DeeVFSView_TryIsExecutable DeeVFSView_TryIsExecutable
DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(0) int)
DeeVFSView_TryIsExecutable(struct DeeVFSView const *self) {
 struct DeeVFSNode *node; int result;
 if DEE_UNLIKELY((node = DeeVFSView_TryGetCurrentNode(self)) == NULL) return 0;
 result = DeeVFSNode_IsExecutable(node);
 DeeVFSNode_DECREF(node);
 if DEE_UNLIKELY(result < 0) DeeError_HandledOne();
 return result;
}
#define DeeVFSView_IsExecutable(self,result,...) \
 DeeVFSView_PRIVATE_GET_ATTRIBUTE(self,DeeVFSNode_IsExecutable,result,__VA_ARGS__)

#define DeeVFSView_TryIsCharDev DeeVFSView_TryIsCharDev
DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(0) int)
DeeVFSView_TryIsCharDev(struct DeeVFSView const *self) {
 struct DeeVFSNode *node; int result;
 if DEE_UNLIKELY((node = DeeVFSView_TryGetCurrentNode(self)) == NULL) return 0;
 result = DeeVFSNode_IsCharDev(node);
 DeeVFSNode_DECREF(node);
 if DEE_UNLIKELY(result < 0) DeeError_HandledOne();
 return result;
}
#define DeeVFSView_IsCharDev(self,result,...) \
 DeeVFSView_PRIVATE_GET_ATTRIBUTE(self,DeeVFSNode_IsCharDev,result,__VA_ARGS__)

#define DeeVFSView_TryIsBlockDev DeeVFSView_TryIsBlockDev
DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(0) int)
DeeVFSView_TryIsBlockDev(struct DeeVFSView const *self) {
 struct DeeVFSNode *node; int result;
 if DEE_UNLIKELY((node = DeeVFSView_TryGetCurrentNode(self)) == NULL) return 0;
 result = DeeVFSNode_IsBlockDev(node);
 DeeVFSNode_DECREF(node);
 if DEE_UNLIKELY(result < 0) DeeError_HandledOne();
 return result;
}
#define DeeVFSView_IsBlockDev(self,result,...) \
 DeeVFSView_PRIVATE_GET_ATTRIBUTE(self,DeeVFSNode_IsBlockDev,result,__VA_ARGS__)

#define DeeVFSView_TryIsFiFo DeeVFSView_TryIsFiFo
DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(0) int)
DeeVFSView_TryIsFiFo(struct DeeVFSView const *self) {
 struct DeeVFSNode *node; int result;
 if DEE_UNLIKELY((node = DeeVFSView_TryGetCurrentNode(self)) == NULL) return 0;
 result = DeeVFSNode_IsFiFo(node);
 DeeVFSNode_DECREF(node);
 if DEE_UNLIKELY(result < 0) DeeError_HandledOne();
 return result;
}
#define DeeVFSView_IsFiFo(self,result,...) \
 DeeVFSView_PRIVATE_GET_ATTRIBUTE(self,DeeVFSNode_IsFiFo,result,__VA_ARGS__)

#define DeeVFSView_TryIsSocket DeeVFSView_TryIsSocket
DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(0) int)
DeeVFSView_TryIsSocket(struct DeeVFSView const *self) {
 struct DeeVFSNode *node; int result;
 if DEE_UNLIKELY((node = DeeVFSView_TryGetCurrentNode(self)) == NULL) return 0;
 result = DeeVFSNode_IsSocket(node);
 DeeVFSNode_DECREF(node);
 if DEE_UNLIKELY(result < 0) DeeError_HandledOne();
 return result;
}
#define DeeVFSView_IsSocket(self,result,...) \
 DeeVFSView_PRIVATE_GET_ATTRIBUTE(self,DeeVFSNode_IsSocket,result,__VA_ARGS__)

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_VIEW_UTF8_H */
