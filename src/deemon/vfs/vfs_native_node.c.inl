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
#ifndef GUARD_DEEMON_VFS_VFS_NATIVE_NODE_C_INL
#define GUARD_DEEMON_VFS_VFS_NATIVE_NODE_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/vfs/vfs_core.h>
#include <deemon/vfs/vfs_native_node.h>
#include <deemon/sys/sysfd.h>
#include <deemon/optional/std/string.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

#if DEE_CONFIG_RUNTIME_HAVE_VFS2
DEE_DECL_BEGIN

// TODO: System-callbacks in here are kind-of dependent on a windows-style layout....


//////////////////////////////////////////////////////////////////////////
// Node VTable
static void DEE_CALL _deevfs_nativenode_vnt_quit(DEE_A_INOUT struct DeeVFSNativeNode *self) { Dee_DECREF(self->vnn_path); }
static struct DeeVFSNativeNode *DEE_CALL _deevfs_nativenode_vnt_walk(
 DEE_A_INOUT struct DeeVFSNativeNode *self, DEE_A_IN_Z char const *name) {
 struct DeeVFSNativeNode *result;
 DeeObject *newpath; Dee_size_t pathsize,namesize;
 if DEE_UNLIKELY((result = DeeVFSNode_ALLOC(struct DeeVFSNativeNode)) == NULL) return NULL;
 pathsize = DeeString_SIZE(self->vnn_path);
 namesize = strlen(name);
 if DEE_UNLIKELY((newpath = DeeString_NewSized(pathsize+1+namesize)) == NULL) {
err_r: DeeVFSNode_FREE(result); return NULL;
 }
 memcpy(DeeString_STR(newpath),DeeString_STR(self->vnn_path),pathsize*sizeof(char));
 DeeString_STR(newpath)[pathsize] = '\\';
 memcpy(DeeString_STR(newpath)+(pathsize+1),name,namesize*sizeof(char));
 // Make sure that the path really exists
 if (GetFileAttributesA(DeeString_STR(newpath)) == INVALID_FILE_ATTRIBUTES) {
  // Invalid path
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Invalid path: %r: %K",newpath,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  Dee_DECREF(newpath);
  goto err_r;
 }
 DeeVFSNode_InitWithParent(&result->vnn_node,&DeeVFSNativeNode_Type,
                           &self->vnn_node);
 Dee_INHERIT_REF(result->vnn_path,*(DeeStringObject **)&newpath);
 return result;
}
static DeeObject *DEE_CALL _deevfs_nativenode_vnt_nameof(
 struct DeeVFSNativeNode *self, struct DeeVFSNativeNode *child) {
 char const *end,*result,*resultmin;
 DEE_ASSERTF(DeeVFSNode_IsNative(&child->vnn_node),"Node %r is not a child of %r",
             DeeVFSNode_Filename(&child->vnn_node),DeeVFSNode_Filename(&self->vnn_node));
 (void)self;
 end = result = (resultmin = DeeString_STR(child->vnn_path))+DeeString_SIZE(child->vnn_path);
 do --result; while(*result != '\\' && result != resultmin);
 return DeeString_NewWithLength((Dee_size_t)(end-result),result);
}
static DeeObject *DEE_CALL _deevfs_nativenode_vnt_link(DEE_A_INOUT struct DeeVFSNativeNode *self) {
 return DeeNativeFS_ReadlinkObject((DeeObject *)self->vnn_path);
}
static int DEE_CALL _deevfs_nativenode_vnt_isfile(DEE_A_INOUT struct DeeVFSNativeNode *self) {
 return DeeNativeFS_IsFileObject((DeeObject *)self->vnn_path);
}
static int DEE_CALL _deevfs_nativenode_vnt_isdir(DEE_A_INOUT struct DeeVFSNativeNode *self) {
 return DeeNativeFS_IsDirObject((DeeObject *)self->vnn_path);
}
static int DEE_CALL _deevfs_nativenode_vnt_islink(DEE_A_INOUT struct DeeVFSNativeNode *self) {
 return DeeNativeFS_IsLinkObject((DeeObject *)self->vnn_path);
}
static int DEE_CALL _deevfs_nativenode_vnt_ishidden(DEE_A_INOUT struct DeeVFSNativeNode *self) {
 return DeeNativeFS_IsHiddenObject((DeeObject *)self->vnn_path);
}
static int DEE_CALL _deevfs_nativenode_vnt_ischardev(DEE_A_INOUT struct DeeVFSNativeNode *self) {
 return DeeNativeFS_IsCharDevObject((DeeObject *)self->vnn_path);
}
static int DEE_CALL _deevfs_nativenode_vnt_isblockdev(DEE_A_INOUT struct DeeVFSNativeNode *self) {
 return DeeNativeFS_IsBlockDevObject((DeeObject *)self->vnn_path);
}
static int DEE_CALL _deevfs_nativenode_vnt_isfifo(DEE_A_INOUT struct DeeVFSNativeNode *self) {
 return DeeNativeFS_IsFiFoObject((DeeObject *)self->vnn_path);
}
static int DEE_CALL _deevfs_nativenode_vnt_issocket(DEE_A_INOUT struct DeeVFSNativeNode *self) {
 return DeeNativeFS_IsSocketObject((DeeObject *)self->vnn_path);
}
static int DEE_CALL _deevfs_nativenode_vnt_getmod(
 DEE_A_INOUT struct DeeVFSNativeNode *self, DEE_A_OUT Dee_mode_t *mode) {
 return DeeNativeFS_GetModObject((DeeObject *)self->vnn_path,mode);
}
static int DEE_CALL _deevfs_nativenode_vnt_chmod(
 DEE_A_INOUT struct DeeVFSNativeNode *self, DEE_A_IN Dee_mode_t mode) {
 return DeeNativeFS_ChmodObject((DeeObject *)self->vnn_path,mode);
}
static int DEE_CALL _deevfs_nativenode_vnt_getown(
 DEE_A_INOUT struct DeeVFSNativeNode *self, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 return DeeNativeFS_GetOwnObject((DeeObject *)self->vnn_path,owner,group);
}
static int DEE_CALL _deevfs_nativenode_vnt_chown(
 DEE_A_INOUT struct DeeVFSNativeNode *self, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
 return DeeNativeFS_ChownObject((DeeObject *)self->vnn_path,owner,group);
}
static int DEE_CALL _deevfs_nativenode_vnt_gettimes(
 DEE_A_INOUT struct DeeVFSNativeNode *self, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 (void)self,atime,ctime,mtime; // TODO
 return 0;
}
static int DEE_CALL _deevfs_nativenode_vnt_settimes(
 DEE_A_INOUT struct DeeVFSNativeNode *self, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime) {
 (void)self,atime,ctime,mtime; // TODO
 return 0;
}


//////////////////////////////////////////////////////////////////////////
// File VTable
static int DEE_CALL _deevfs_nativefile_vft_open(
 struct DeeVFSNativeFile *self, Dee_openmode_t openmode, Dee_mode_t permissions) {
 DeeSysFileFD_Utf8InitObject(&self->vnf_fd,(DeeObject *)((
  DeeVFSNativeNode *)self->vnf_file.vf_node)->vnn_path,
  openmode,permissions,return -1);
 return 0;
}
static void DEE_CALL _deevfs_nativefile_quit(struct DeeVFSNativeFile *self) {
 DeeSysFD_Quit(&self->vnf_fd);
}
static int DEE_CALL _deevfs_nativefile_vft_read(
 DEE_A_INOUT struct DeeVFSNativeFile *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 DeeSysFD_Read(&self->vnf_fd,p,s,rs,return -1);
 return 0;
}
static int DEE_CALL _deevfs_nativefile_vft_write(
 DEE_A_INOUT struct DeeVFSNativeFile *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 DeeSysFD_Write(&self->vnf_fd,p,s,ws,return -1);
 return 0;
}
static int DEE_CALL _deevfs_nativefile_vft_seek(
 DEE_A_INOUT struct DeeVFSNativeFile *self, Dee_int64_t off, int whence, Dee_uint64_t *pos) {
 DeeSysFD_Seek(&self->vnf_fd,off,whence,pos,return -1);
 return 0;
}
static int DEE_CALL _deevfs_nativefile_vft_flush(DEE_A_INOUT struct DeeVFSNativeFile *self) {
 DeeSysFD_Flush(&self->vnf_fd,return -1);
 return 0;
}
static int DEE_CALL _deevfs_nativefile_vft_trunc(DEE_A_INOUT struct DeeVFSNativeFile *self) {
 DeeSysFD_Trunc(&self->vnf_fd,return -1);
 return 0;
}


//////////////////////////////////////////////////////////////////////////
// View VTable
static int  DEE_CALL _deevfs_nativeview_vvt_open(struct DeeVFSNativeView *self) {
 (void)self; // TODO
 return 0;
}
static void DEE_CALL _deevfs_nativeview_vvt_quit(struct DeeVFSNativeView *self) {
 (void)self; // TODO
}
static int DEE_CALL _deevfs_nativeview_vvt_yield(
 struct DeeVFSNativeView *self, struct DeeVFSNode **result) {
 (void)self,result; // TODO
 return 1;
}



struct DeeVFSNodeType const DeeVFSNativeNode_Type = {
 { sizeof(struct DeeVFSNativeNode), // vnt_node
  (void (DEE_CALL *)(struct DeeVFSNode *))                                                                                              &_deevfs_nativenode_vnt_quit,
  (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,char const *))                                                                   &_deevfs_nativenode_vnt_walk,
  (DeeObject *(DEE_CALL *)(struct DeeVFSNode *,struct DeeVFSNode *))                                                                    &_deevfs_nativenode_vnt_nameof,
  (DeeObject *(DEE_CALL *)(struct DeeVFSNode *))                                                                                        &_deevfs_nativenode_vnt_link,
  (int (DEE_CALL *)(struct DeeVFSNode *))                                                                                               &_deevfs_nativenode_vnt_isfile,
  (int (DEE_CALL *)(struct DeeVFSNode *))                                                                                               &_deevfs_nativenode_vnt_isdir,
  (int (DEE_CALL *)(struct DeeVFSNode *))                                                                                               &_deevfs_nativenode_vnt_islink,
  (int (DEE_CALL *)(struct DeeVFSNode *))                                                                                               &_deevfs_nativenode_vnt_ishidden,
  (int (DEE_CALL *)(struct DeeVFSNode *))                                                                                               &_deevfs_nativenode_vnt_ischardev,
  (int (DEE_CALL *)(struct DeeVFSNode *))                                                                                               &_deevfs_nativenode_vnt_isblockdev,
  (int (DEE_CALL *)(struct DeeVFSNode *))                                                                                               &_deevfs_nativenode_vnt_isfifo,
  (int (DEE_CALL *)(struct DeeVFSNode *))                                                                                               &_deevfs_nativenode_vnt_issocket,
  (int (DEE_CALL *)(struct DeeVFSNode *,Dee_mode_t *))                                                                                  &_deevfs_nativenode_vnt_getmod,
  (int (DEE_CALL *)(struct DeeVFSNode *,Dee_mode_t ))                                                                                   &_deevfs_nativenode_vnt_chmod,
  (int (DEE_CALL *)(struct DeeVFSNode *,Dee_uid_t *,DEE_A_OUT Dee_gid_t *))                                                             &_deevfs_nativenode_vnt_getown,
  (int (DEE_CALL *)(struct DeeVFSNode *,Dee_uid_t,DEE_A_IN Dee_gid_t))                                                                  &_deevfs_nativenode_vnt_chown,
  (int (DEE_CALL *)(struct DeeVFSNode *,Dee_timetick_t *,DEE_A_OUT_OPT Dee_timetick_t *,DEE_A_OUT_OPT Dee_timetick_t *))                &_deevfs_nativenode_vnt_gettimes,
  (int (DEE_CALL *)(struct DeeVFSNode *,Dee_timetick_t const *,DEE_A_IN_OPT Dee_timetick_t const *,DEE_A_IN_OPT Dee_timetick_t const *))&_deevfs_nativenode_vnt_settimes,
 },{ sizeof(struct DeeVFSNativeFile),// vnt_file
  (int (DEE_CALL *)(struct DeeVFSFile *,Dee_openmode_t,Dee_mode_t))           &_deevfs_nativefile_vft_open,
  (void(DEE_CALL *)(struct DeeVFSFile *))                                     &_deevfs_nativefile_quit,
  (int (DEE_CALL *)(struct DeeVFSFile *,void *,Dee_size_t,Dee_size_t *))      &_deevfs_nativefile_vft_read,
  (int (DEE_CALL *)(struct DeeVFSFile *,void const *,Dee_size_t,Dee_size_t *))&_deevfs_nativefile_vft_write,
  (int (DEE_CALL *)(struct DeeVFSFile *,Dee_int64_t,int,Dee_uint64_t *))      &_deevfs_nativefile_vft_seek,
  (int (DEE_CALL *)(struct DeeVFSFile *))                                     &_deevfs_nativefile_vft_flush,
  (int (DEE_CALL *)(struct DeeVFSFile *))                                     &_deevfs_nativefile_vft_trunc,
 },{ sizeof(struct DeeVFSNativeView), // vnt_view
 // TODO
  (int (DEE_CALL *)(struct DeeVFSView *))                     &_deevfs_nativeview_vvt_open,
  (void (DEE_CALL *)(struct DeeVFSView *))                    &_deevfs_nativeview_vvt_quit,
  (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))&_deevfs_nativeview_vvt_yield,
 }
};



DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */

#endif /* !GUARD_DEEMON_VFS_VFS_NATIVE_NODE_C_INL */
