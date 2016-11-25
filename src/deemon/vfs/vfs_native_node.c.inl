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

#if DEE_CONFIG_RUNTIME_HAVE_VFS
#include <deemon/vfs/vfs_core.h>
#include <deemon/vfs/vfs_native_node.h>
#include <deemon/fs/native_filefd.h>
#include <deemon/fs/native_fs.h>
#include <deemon/optional/std/string.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeNFS_WideChdirDriveRoot(DEE_A_IN Dee_WideChar drive_name) {
 // Without the \ character at the end, calling 'SetCurrentDirectory("C:")' will
 // only switch the cwd of the c-drive, but not actually set that cwd to its root.
 // >> SetCurrentDirectory("C:"); // Not what we want
 // >> SetCurrentDirectory("C:\\"); // Different, and what we want!
 Dee_WideChar path[4] = {0,':','\\',0};
 path[0] = drive_name;
 return DeeNFS_WideChdir(path);
}


//////////////////////////////////////////////////////////////////////////
// Node VTable
static void DEE_CALL _deevfs_nativenode_vnt_quit(DEE_A_INOUT struct DeeVFSNativeNode *self) { Dee_DECREF(self->vnn_path); }
static struct DeeVFSNativeNode *DEE_CALL _deevfs_nativenode_vnt_walk(
 DEE_A_INOUT struct DeeVFSNativeNode *self, DEE_A_IN_Z Dee_Utf8Char const *name, Dee_size_t name_size) {
 struct DeeVFSNativeNode *result; DeeObject *newpath;
 static Dee_Utf8Char const _fmt[] = {'%','k','\\','%','$','s',0};
 if DEE_UNLIKELY((result = DeeVFSNode_ALLOC(struct DeeVFSNativeNode)) == NULL) return NULL;
 newpath = DeeUtf8String_Newf(_fmt,self->vnn_path,name_size,name);
 if (!newpath) { DeeVFSNode_FREE(result); return NULL; }
 DeeVFSNode_InitWithParent(&result->vnn_node,&DeeVFSNativeNode_Type,
                           &self->vnn_node);
 Dee_INHERIT_REF(result->vnn_path,*(DeeAnyStringObject **)&newpath);
 return result;
}
static struct DeeVFSNativeNode *DEE_CALL _deevfs_nativenode_vnt_wwalk(
 DEE_A_INOUT struct DeeVFSNativeNode *self, DEE_A_IN_Z Dee_WideChar const *name, Dee_size_t name_size) {
 struct DeeVFSNativeNode *result; DeeObject *newpath;
 static Dee_WideChar const _fmt[] = {'%','k','\\','%','$','s',0};
 if DEE_UNLIKELY((result = DeeVFSNode_ALLOC(struct DeeVFSNativeNode)) == NULL) return NULL;
 newpath = DeeWideString_Newf(_fmt,self->vnn_path,name_size,name);
 if (!newpath) { DeeVFSNode_FREE(result); return NULL; }
 DeeVFSNode_InitWithParent(&result->vnn_node,&DeeVFSNativeNode_Type,
                           &self->vnn_node);
 Dee_INHERIT_REF(result->vnn_path,*(DeeAnyStringObject **)&newpath);
 return result;
}
extern DeeObject *DEE_CALL _deevfs_nativenode_vnt_nameof(
 struct DeeVFSNode *self, struct DeeVFSNativeNode *child) {
 DEE_ASSERTF(DeeVFSNode_IsNative(&child->vnn_node),"Node %r is not a child of %r",
             DeeVFSNode_Filename(&child->vnn_node),DeeVFSNode_Filename(self));
 (void)self;
 if (DeeUtf8String_Check(child->vnn_path)) {
  Dee_Utf8Char const *end,*result,*resultmin;
  end = result = (resultmin = DeeUtf8String_STR(child->vnn_path))+DeeUtf8String_SIZE(child->vnn_path);
  while(result != resultmin && result[-1] != '\\') --result;
  return DeeUtf8String_NewWithLength((Dee_size_t)(end-result),result);
 } else {
  Dee_WideChar const *end,*result,*resultmin;
  end = result = (resultmin = DeeWideString_STR(child->vnn_path))+DeeWideString_SIZE(child->vnn_path);
  while(result != resultmin && result[-1] != '\\') --result;
  return DeeWideString_NewWithLength((Dee_size_t)(end-result),result);
 }
}
static DeeObject *DEE_CALL _deevfs_nativenode_vnt_readlink(DEE_A_INOUT struct DeeVFSNativeNode *self) { return DeeVFSNativeNode_NFS_Readlink(self); }
static int DEE_CALL _deevfs_nativenode_vnt_isfile(DEE_A_INOUT struct DeeVFSNativeNode *self) { return DeeVFSNativeNode_NFS_IsFile(self); }
static int DEE_CALL _deevfs_nativenode_vnt_isdir(DEE_A_INOUT struct DeeVFSNativeNode *self) { return DeeVFSNativeNode_NFS_IsDir(self); }
static int DEE_CALL _deevfs_nativenode_vnt_islink(DEE_A_INOUT struct DeeVFSNativeNode *self) { return DeeVFSNativeNode_NFS_IsLink(self); }
static int DEE_CALL _deevfs_nativenode_vnt_ishidden(DEE_A_INOUT struct DeeVFSNativeNode *self) { return DeeVFSNativeNode_NFS_IsHidden(self); }
static int DEE_CALL _deevfs_nativenode_vnt_isexecutable(DEE_A_INOUT struct DeeVFSNativeNode *self) { return DeeVFSNativeNode_NFS_IsExecutable(self); }
static int DEE_CALL _deevfs_nativenode_vnt_ischardev(DEE_A_INOUT struct DeeVFSNativeNode *self) { return DeeVFSNativeNode_NFS_IsCharDev(self); }
static int DEE_CALL _deevfs_nativenode_vnt_isblockdev(DEE_A_INOUT struct DeeVFSNativeNode *self) { return DeeVFSNativeNode_NFS_IsBlockDev(self); }
static int DEE_CALL _deevfs_nativenode_vnt_isfifo(DEE_A_INOUT struct DeeVFSNativeNode *self) { return DeeVFSNativeNode_NFS_IsFiFo(self); }
static int DEE_CALL _deevfs_nativenode_vnt_issocket(DEE_A_INOUT struct DeeVFSNativeNode *self) { return DeeVFSNativeNode_NFS_IsSocket(self); }
static int DEE_CALL _deevfs_nativenode_vnt_getmod(DEE_A_INOUT struct DeeVFSNativeNode *self, DEE_A_OUT Dee_mode_t *mode) { return DeeVFSNativeNode_NFS_GetMod(self,mode); }
static int DEE_CALL _deevfs_nativenode_vnt_chmod(DEE_A_INOUT struct DeeVFSNativeNode *self, DEE_A_IN Dee_mode_t mode) { return DeeVFSNativeNode_NFS_Chmod(self,mode); }
static int DEE_CALL _deevfs_nativenode_vnt_getown(DEE_A_INOUT struct DeeVFSNativeNode *self, DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) { return DeeVFSNativeNode_NFS_GetOwn(self,owner,group); }
static int DEE_CALL _deevfs_nativenode_vnt_chown(DEE_A_INOUT struct DeeVFSNativeNode *self, DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) { return DeeVFSNativeNode_NFS_Chown(self,owner,group); }
static int DEE_CALL _deevfs_nativenode_vnt_gettimes(DEE_A_INOUT struct DeeVFSNativeNode *self, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) { return DeeVFSNativeNode_NFS_GetTimes(self,atime,ctime,mtime); }
static int DEE_CALL _deevfs_nativenode_vnt_settimes(DEE_A_INOUT struct DeeVFSNativeNode *self, DEE_A_IN_OPT Dee_timetick_t const *atime, DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime) { return DeeVFSNativeNode_NFS_SetTimes(self,atime,ctime,mtime); }


#ifdef DeeSysFileFD
//////////////////////////////////////////////////////////////////////////
// File VTable
static int DEE_CALL _deevfs_nativefile_vft_open(
 struct DeeVFSNativeFile *self, Dee_openmode_t openmode, Dee_mode_t permissions) {
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeNativeFileFD_Utf8InitObject(&self->vnf_fd,(DeeObject *)((
  struct DeeVFSNativeNode *)self->vnf_file.vf_node)->vnn_path,
  openmode,permissions,return -1);
 return 0;
}
void DEE_CALL _deevfs_nativefile_quit(struct DeeVFSNativeFile *self) {
 DeeNativeFileFD_Quit(&self->vnf_fd);
}
int DEE_CALL _deevfs_nativefile_vft_read(
 DEE_A_INOUT struct DeeVFSNativeFile *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeNativeFileFD_Read(&self->vnf_fd,p,s,rs,return -1);
 return 0;
}
int DEE_CALL _deevfs_nativefile_vft_write(
 DEE_A_INOUT struct DeeVFSNativeFile *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeNativeFileFD_Write(&self->vnf_fd,p,s,ws,return -1);
 return 0;
}
int DEE_CALL _deevfs_nativefile_vft_seek(
 DEE_A_INOUT struct DeeVFSNativeFile *self, Dee_int64_t off, int whence, Dee_uint64_t *pos) {
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeNativeFileFD_Seek(&self->vnf_fd,off,whence,pos,return -1);
 return 0;
}
int DEE_CALL _deevfs_nativefile_vft_flush(DEE_A_INOUT struct DeeVFSNativeFile *self) {
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeNativeFileFD_Flush(&self->vnf_fd,return -1);
 return 0;
}
int DEE_CALL _deevfs_nativefile_vft_trunc(DEE_A_INOUT struct DeeVFSNativeFile *self) {
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeNativeFileFD_Trunc(&self->vnf_fd,return -1);
 return 0;
}
#endif


//////////////////////////////////////////////////////////////////////////
// View VTable
static int DEE_CALL _deevfs_nativeview_vvt_open(struct DeeVFSNativeView *self) {
 DeeObject *native_path;
 DEE_ASSERT(DeeVFSNode_IsNative(self->vnv_view.vv_node));
 if DEE_UNLIKELY((native_path = DeeWideString_Cast((DeeObject *)((
  struct DeeVFSNativeNode *)self->vnv_view.vv_node)->vnn_path)) == NULL) return -1;
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(native_path); return -1; })
 DeeNFSWideView_InitObject(&self->vnv_dir,native_path,{ Dee_DECREF(native_path); return -1; });
 Dee_DECREF(native_path);
 return 0;
}
static void DEE_CALL _deevfs_nativeview_vvt_quit(struct DeeVFSNativeView *self) {
 DeeNFSWideView_Quit(&self->vnv_dir);
}

static int DEE_CALL _deevfs_nativeview_vvt_curr(
 struct DeeVFSNativeView *self, struct DeeVFSNativeNode **result) {
 DeeObject *filename,*u8filename;
again:
 DEE_NFS_CHECKINTERRUPT(return -1);
 DeeNFSWideView_Acquire(&self->vnv_dir);
 if (DeeNFSWideView_IsDone(&self->vnv_dir)) {
  DeeNFSWideView_Release(&self->vnv_dir);
  return 1;
 }
 if DEE_UNLIKELY((filename = DeeNFSWideView_TryGetFilenameObjectLocked(&self->vnv_dir)) == NULL) {
  DeeNFSWideView_Release(&self->vnv_dir);
  if (Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return -1;
 }
 DeeNFSWideView_Release(&self->vnv_dir);
 u8filename = DeeString_FromWideStringWithLength(DeeWideString_SIZE(filename),
                                                 DeeWideString_STR(filename));
 Dee_DECREF(filename);
 if DEE_UNLIKELY(!u8filename) return -1;
 *result = _deevfs_nativenode_vnt_walk((struct DeeVFSNativeNode *)self->vnv_view.vv_node,
                                       DeeString_STR(u8filename),DeeString_SIZE(u8filename));
 Dee_DECREF(u8filename);
 if DEE_UNLIKELY(!*result) return -1;
 return 0;
}
static int DEE_CALL _deevfs_nativeview_vvt_yield(
 struct DeeVFSNativeView *self, struct DeeVFSNativeNode **result) {
 DeeObject *filename,*u8filename;
again:
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeNFSWideView_Acquire(&self->vnv_dir);
 if (DeeNFSWideView_IsDone(&self->vnv_dir)) {
  DeeNFSWideView_Release(&self->vnv_dir);
  return 1;
 }
 if DEE_UNLIKELY((filename = DeeNFSWideView_TryGetFilenameObjectLocked(&self->vnv_dir)) == NULL) {
  DeeNFSWideView_Release(&self->vnv_dir);
  if (Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return -1;
 }
 DeeNFSWideView_AdvanceAndReleaseOnError(&self->vnv_dir,{ Dee_DECREF(filename); return -1; });
 DeeNFSWideView_Release(&self->vnv_dir);
 u8filename = DeeString_FromWideStringWithLength(DeeWideString_SIZE(filename),
                                                 DeeWideString_STR(filename));
 Dee_DECREF(filename);
 if DEE_UNLIKELY(!u8filename) return -1;
 *result = _deevfs_nativenode_vnt_walk((struct DeeVFSNativeNode *)self->vnv_view.vv_node,
                                       DeeString_STR(u8filename),DeeString_SIZE(u8filename));
 Dee_DECREF(u8filename);
 if DEE_UNLIKELY(!*result) return -1;
 return 0;
}
static struct _DeeVFSPropTypeData _deevfs_nativenode_vnt_prop = {
 (int (DEE_CALL *)(struct DeeVFSNode *))                                                                     &_deevfs_nativenode_vnt_isfile,
 (int (DEE_CALL *)(struct DeeVFSNode *))                                                                     &_deevfs_nativenode_vnt_isdir,
 (int (DEE_CALL *)(struct DeeVFSNode *))                                                                     &_deevfs_nativenode_vnt_islink,
 (int (DEE_CALL *)(struct DeeVFSNode *))                                                                     &_deevfs_nativenode_vnt_ishidden,
 (int (DEE_CALL *)(struct DeeVFSNode *))                                                                     &_deevfs_nativenode_vnt_isexecutable,
 (int (DEE_CALL *)(struct DeeVFSNode *))                                                                     &_deevfs_nativenode_vnt_ischardev,
 (int (DEE_CALL *)(struct DeeVFSNode *))                                                                     &_deevfs_nativenode_vnt_isblockdev,
 (int (DEE_CALL *)(struct DeeVFSNode *))                                                                     &_deevfs_nativenode_vnt_isfifo,
 (int (DEE_CALL *)(struct DeeVFSNode *))                                                                     &_deevfs_nativenode_vnt_issocket,
 (int (DEE_CALL *)(struct DeeVFSNode *,Dee_mode_t *))                                                        &_deevfs_nativenode_vnt_getmod,
 (int (DEE_CALL *)(struct DeeVFSNode *,Dee_mode_t ))                                                         &_deevfs_nativenode_vnt_chmod,
 (int (DEE_CALL *)(struct DeeVFSNode *,Dee_uid_t *,Dee_gid_t *))                                             &_deevfs_nativenode_vnt_getown,
 (int (DEE_CALL *)(struct DeeVFSNode *,Dee_uid_t,Dee_gid_t))                                                 &_deevfs_nativenode_vnt_chown,
 (int (DEE_CALL *)(struct DeeVFSNode *,Dee_timetick_t *,Dee_timetick_t *,Dee_timetick_t *))                  &_deevfs_nativenode_vnt_gettimes,
 (int (DEE_CALL *)(struct DeeVFSNode *,Dee_timetick_t const *,Dee_timetick_t const *,Dee_timetick_t const *))&_deevfs_nativenode_vnt_settimes,
};
#ifdef DeeNativeFileFD
static struct _DeeVFSFileTypeData _deevfs_nativenode_vnt_file = {
 sizeof(struct DeeVFSNativeFile),
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_openmode_t,Dee_mode_t))           &_deevfs_nativefile_vft_open,
 (void(DEE_CALL *)(struct DeeVFSFile *))                                     &_deevfs_nativefile_quit,
 (int (DEE_CALL *)(struct DeeVFSFile *,void *,Dee_size_t,Dee_size_t *))      &_deevfs_nativefile_vft_read,
 (int (DEE_CALL *)(struct DeeVFSFile *,void const *,Dee_size_t,Dee_size_t *))&_deevfs_nativefile_vft_write,
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_int64_t,int,Dee_uint64_t *))      &_deevfs_nativefile_vft_seek,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     &_deevfs_nativefile_vft_flush,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     &_deevfs_nativefile_vft_trunc,
};
#endif
static struct _DeeVFSViewTypeData _deevfs_nativenode_vnt_view = {
 sizeof(struct DeeVFSNativeView),
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,Dee_Utf8Char const *,Dee_size_t))&_deevfs_nativenode_vnt_walk,
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,Dee_WideChar const *,Dee_size_t))&_deevfs_nativenode_vnt_wwalk,
 (DeeObject *(DEE_CALL *)(struct DeeVFSNode *,struct DeeVFSNode *))                    &_deevfs_nativenode_vnt_nameof,
 (int (DEE_CALL *)(struct DeeVFSView *))                                               &_deevfs_nativeview_vvt_open,
 (void (DEE_CALL *)(struct DeeVFSView *))                                              &_deevfs_nativeview_vvt_quit,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_nativeview_vvt_curr,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_nativeview_vvt_yield,
};

struct DeeVFSNodeType const DeeVFSNativeNode_Type = {
 {(void (DEE_CALL *)(struct DeeVFSNode *))      &_deevfs_nativenode_vnt_quit,
  (DeeObject *(DEE_CALL *)(struct DeeVFSNode *))&_deevfs_nativenode_vnt_readlink},
 &_deevfs_nativenode_vnt_prop,
#ifdef DeeNativeFileFD
 &_deevfs_nativenode_vnt_file,
#else
 DeeVFSNoopNodeType_FileData,
#endif
 &_deevfs_nativenode_vnt_view
};



DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#endif /* !GUARD_DEEMON_VFS_VFS_NATIVE_NODE_C_INL */
