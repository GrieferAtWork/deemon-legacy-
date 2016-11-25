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
#ifndef GUARD_DEEMON_VFS_VFS_NATIVE_MOUNT_C_INL
#define GUARD_DEEMON_VFS_VFS_NATIVE_MOUNT_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/__xconf.inl>
#include <deemon/fs/native_fs.h>
#include <deemon/vfs/vfs_core.h>
#include <deemon/vfs/vfs_native_node.h>
#include <deemon/vfs/vfs_native_mount.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

#if DEE_CONFIG_RUNTIME_HAVE_VFS
DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(NULL) char *DeeWin32Sys_GetDriveStrings(void) {
 char *buffer,*new_buffer;
 DWORD req_bufsize,used_bufsize;
 while DEE_UNLIKELY((buffer = (char *)malloc_nz((
  DEE_XCONFIG_FSBUFSIZE_WIN32DRIVES+1)*sizeof(char))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
no_mem:
  DeeError_NoMemory();
  return NULL;
 }
 used_bufsize = DEE_XCONFIG_FSBUFSIZE_WIN32DRIVES;
again:
 DEE_NFS_CHECKINTERRUPT({ free_nn(buffer); return NULL; })
 if DEE_UNLIKELY((req_bufsize = GetLogicalDriveStringsA(used_bufsize,buffer)) == 0) {
  free_nn(buffer);
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      DEE_PP_STR(GetLogicalDriveStringsA) "() : %K",
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return NULL;
 }
 if DEE_UNLIKELY(req_bufsize > DEE_XCONFIG_FSBUFSIZE_WIN32DRIVES) {
  while DEE_UNLIKELY((new_buffer = (char *)realloc_nnz(
   buffer,(req_bufsize+1)*sizeof(char))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   free_nn(buffer);
   goto no_mem;
  }
  buffer = new_buffer;
  used_bufsize = req_bufsize;
  goto again;
 }
 buffer[req_bufsize] = '\0';
 return buffer;
}


//////////////////////////////////////////////////////////////////////////
// View VTable
static struct DeeVFSNativeNode *DEE_CALL _deevfs_nativemountnode_vnt_walk(
 struct DeeVFSNode *self, Dee_Utf8Char const *name, Dee_size_t path_size) {
 struct DeeVFSNativeNode *result; DeeObject *newpath;
 static Dee_Utf8Char const _fmt[] = {'%','$','s',':',0};
 if DEE_UNLIKELY((newpath = DeeUtf8String_Newf(_fmt,path_size,name)) == NULL) return NULL;
 if DEE_UNLIKELY((result = DeeVFSNode_ALLOC(struct DeeVFSNativeNode)) == NULL) { Dee_DECREF(newpath); return NULL; }
 DeeVFSNode_InitWithParent(&result->vnn_node,&DeeVFSNativeNode_Type,self);
 Dee_INHERIT_REF(result->vnn_path,*(DeeAnyStringObject **)&newpath);
 return result;
}
static struct DeeVFSNativeNode *DEE_CALL _deevfs_nativemountnode_vnt_wwalk(
 struct DeeVFSNode *self, Dee_WideChar const *name, Dee_size_t path_size) {
 struct DeeVFSNativeNode *result; DeeObject *newpath;
 static Dee_WideChar const _fmt[] = {'%','l','$','s',':',0};
 if DEE_UNLIKELY((newpath = DeeWideString_Newf(_fmt,path_size,name)) == NULL) return NULL;
 if DEE_UNLIKELY((result = DeeVFSNode_ALLOC(struct DeeVFSNativeNode)) == NULL) { Dee_DECREF(newpath); return NULL; }
 DeeVFSNode_InitWithParent(&result->vnn_node,&DeeVFSNativeNode_Type,self);
 Dee_INHERIT_REF(result->vnn_path,*(DeeAnyStringObject **)&newpath);
 return result;
}
static DeeObject *DEE_CALL _deevfs_nativemountnode_vnt_nameof(
 struct DeeVFSNode *self, struct DeeVFSNativeNode *child) {
 DEE_ASSERTF(DeeVFSNode_IsNative(&child->vnn_node),"Node %r is not a child of %r",
             DeeVFSNode_Filename(&child->vnn_node),DeeVFSNode_Filename(self));
 (void)self;
 DEE_ASSERT(DeeObject_Check(child->vnn_path));
 if (DeeUtf8String_Check(child->vnn_path)) {
  Dee_Utf8Char const *begin,*end,*iter;
  end = (begin = iter = DeeUtf8String_STR(child->vnn_path))+DeeUtf8String_SIZE(child->vnn_path);
  while (iter != end && *iter != ':') ++iter;
  if (iter == end) DeeReturn_NEWREF(child->vnn_path);
  return DeeUtf8String_NewWithLength((Dee_size_t)(iter-begin),begin);
 } else {
  Dee_WideChar const *begin,*end,*iter;
  DEE_ASSERT(DeeWideString_Check(child->vnn_path));
  end = (begin = iter = DeeWideString_STR(child->vnn_path))+DeeWideString_SIZE(child->vnn_path);
  while (iter != end && *iter != ':') ++iter;
  if (iter == end) DeeReturn_NEWREF(child->vnn_path);
  return DeeWideString_NewWithLength((Dee_size_t)(iter-begin),begin);
 }
}
static int DEE_CALL _deevfs_nativemountview_vvt_open(struct DeeVFSNativeMountView *self) {
 if DEE_UNLIKELY((self->vmv_drives = DeeWin32Sys_GetDriveStrings()) == NULL) return -1;
 self->vmv_drivespos = self->vmv_drives;
 DeeAtomicMutex_Init(&self->vmv_lock);
 return 0;
}
static void DEE_CALL _deevfs_nativemountview_vvt_quit(struct DeeVFSNativeMountView *self) {
 DeeWin32Sys_FreeDriveStrings(self->vmv_drives);
}
static int DEE_CALL _deevfs_nativemountview_vvt_curr(
 struct DeeVFSNativeMountView *self, struct DeeVFSNativeNode **result) {
 struct DeeVFSNativeNode *result_node;
 if DEE_UNLIKELY((result_node = DeeVFSNode_ALLOC(struct DeeVFSNativeNode)) == NULL) return -1;
 if DEE_UNLIKELY((result_node->vnn_path = (DeeAnyStringObject *)DeeWideString_NewSized(2)) == NULL) {
/*err_r:*/ DeeVFSNode_FREE(result); return -1;
 }
 DeeWideString_STR(result_node->vnn_path)[1] = ':';
 DeeAtomicMutex_Acquire(&self->vmv_lock);
 DEE_ASSERT(self->vmv_drives);
 DEE_ASSERT(self->vmv_drivespos);
 if (!*self->vmv_drivespos) {
  DeeAtomicMutex_Release(&self->vmv_lock);
  Dee_DECREF(result_node->vnn_path);
  DeeVFSNode_FREE(result);
  return 1;
 }
 DeeWideString_STR(result_node->vnn_path)[0] = (Dee_WideChar)*self->vmv_drivespos;
 DeeAtomicMutex_Release(&self->vmv_lock);
 DeeVFSNode_InitWithParent(&result_node->vnn_node,&DeeVFSNativeNode_Type,
                           self->vmv_view.vv_node);
 *result = result_node;
 return 0;
}
static int DEE_CALL _deevfs_nativemountview_vvt_yield(
 struct DeeVFSNativeMountView *self, struct DeeVFSNativeNode **result) {
 struct DeeVFSNativeNode *result_node;
 if DEE_UNLIKELY((result_node = DeeVFSNode_ALLOC(struct DeeVFSNativeNode)) == NULL) return -1;
 if DEE_UNLIKELY((result_node->vnn_path = (DeeAnyStringObject *)DeeWideString_NewSized(2)) == NULL) {
/*err_r:*/ DeeVFSNode_FREE(result_node); return -1;
 }
 DeeWideString_STR(result_node->vnn_path)[1] = ':';
 DeeAtomicMutex_Acquire(&self->vmv_lock);
 DEE_ASSERT(self->vmv_drives);
 DEE_ASSERT(self->vmv_drivespos);
 if (!*self->vmv_drivespos) {
  DeeAtomicMutex_Release(&self->vmv_lock);
  Dee_DECREF(result_node->vnn_path);
  DeeVFSNode_FREE(result_node);
  return 1;
 }
 DeeWideString_STR(result_node->vnn_path)[0] = (Dee_WideChar)*self->vmv_drivespos;
 self->vmv_drivespos += (strlen(self->vmv_drivespos)+1);
 DeeAtomicMutex_Release(&self->vmv_lock);
 DeeVFSNode_InitWithParent(&result_node->vnn_node,&DeeVFSNativeNode_Type,
                           self->vmv_view.vv_node);
 *result = result_node;
 return 0;
}

static struct _DeeVFSViewTypeData _deevfs_nativemountnode_vnt_view = {
 sizeof(struct DeeVFSNativeMountView),
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,Dee_Utf8Char const *,Dee_size_t))&_deevfs_nativemountnode_vnt_walk,
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,Dee_WideChar const *,Dee_size_t))&_deevfs_nativemountnode_vnt_wwalk,
 (DeeObject *(DEE_CALL *)(struct DeeVFSNode *,struct DeeVFSNode *))                    &_deevfs_nativemountnode_vnt_nameof,
 (int (DEE_CALL *)(struct DeeVFSView *))                                               &_deevfs_nativemountview_vvt_open,
 (void (DEE_CALL *)(struct DeeVFSView *))                                              &_deevfs_nativemountview_vvt_quit,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_nativemountview_vvt_curr,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_nativemountview_vvt_yield,
};

struct DeeVFSNodeType const DeeVFSNativeMountNode_Type = {
 {NULL,NULL},NULL,DeeVFSNoopNodeType_FileData,
 &_deevfs_nativemountnode_vnt_view};


DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#endif /* !GUARD_DEEMON_VFS_VFS_NATIVE_MOUNT_C_INL */
