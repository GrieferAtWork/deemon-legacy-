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
#ifndef GUARD_DEEMON_VFS_VFS_NATIVE_NODE_H
#define GUARD_DEEMON_VFS_VFS_NATIVE_NODE_H 1

#include <deemon/__conf.inl>

#if DEE_CONFIG_RUNTIME_HAVE_VFS
#include <deemon/vfs/vfs_core.h>
#include <deemon/sys/sysfd.h>
#include <deemon/optional/atomic_mutex.h>
#include <deemon/fs/native_view.wide.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()


DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// Native VFS node: A filesystem node that points back into the 
struct DeeVFSNativeNode {
 struct DeeVFSNode             vnn_node; /*< Underlying node. */
 DEE_A_REF DeeAnyStringObject *vnn_path; /*< [1..1] Absolute, native path to this node (NOT trail terminated). */
};
#define DeeVFSNativeNode_PATH(self)\
  (DeeObject *)(((struct DeeVFSNativeNode *)self)->vnn_path)

#define DeeVFSNativeNode_NFS_Chdir(self)                      (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8ChdirObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideChdirObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_GetTimes(self,atime,ctime,mtime) (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8GetTimesObject(DeeVFSNativeNode_PATH(self),atime,ctime,mtime) : DeeNFS_WideGetTimesObject(DeeVFSNativeNode_PATH(self),atime,ctime,mtime))
#define DeeVFSNativeNode_NFS_SetTimes(self,atime,ctime,mtime) (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8SetTimesObject(DeeVFSNativeNode_PATH(self),atime,ctime,mtime) : DeeNFS_WideSetTimesObject(DeeVFSNativeNode_PATH(self),atime,ctime,mtime))
#define DeeVFSNativeNode_NFS_GetMod(self,mode)                (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8GetModObject(DeeVFSNativeNode_PATH(self),mode) : DeeNFS_WideGetModObject(DeeVFSNativeNode_PATH(self),mode))
#define DeeVFSNativeNode_NFS_Chmod(self,mode)                 (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8ChmodObject(DeeVFSNativeNode_PATH(self),mode) : DeeNFS_WideChmodObject(DeeVFSNativeNode_PATH(self),mode))
#define DeeVFSNativeNode_NFS_GetOwn(self,owner,group)         (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8GetOwnObject(DeeVFSNativeNode_PATH(self),owner,group) : DeeNFS_WideGetOwnObject(DeeVFSNativeNode_PATH(self),owner,group))
#define DeeVFSNativeNode_NFS_Chown(self,owner,group)          (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8ChownObject(DeeVFSNativeNode_PATH(self),owner,group) : DeeNFS_WideChownObject(DeeVFSNativeNode_PATH(self),owner,group))
#define DeeVFSNativeNode_NFS_HasProperty(self,prop)           (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8HasPropertyObject(DeeVFSNativeNode_PATH(self),prop) : DeeNFS_WideHasPropertyObject(DeeVFSNativeNode_PATH(self),prop))
#define DeeVFSNativeNode_NFS_IsFile(self)                     (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8IsFileObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideIsFileObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_IsDir(self)                      (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8IsDirObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideIsDirObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_IsLink(self)                     (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8IsLinkObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideIsLinkObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_IsHidden(self)                   (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8IsHiddenObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideIsHiddenObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_IsExecutable(self)               (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8IsExecutableObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideIsExecutableObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_IsCharDev(self)                  (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8IsCharDevObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideIsCharDevObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_IsBlockDev(self)                 (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8IsBlockDevObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideIsBlockDevObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_IsFiFo(self)                     (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8IsFiFoObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideIsFiFoObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_IsSocket(self)                   (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8IsSocketObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideIsSocketObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_Exists(self)                     (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8ExistsObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideExistsObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_Remove(self)                     (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8RemoveObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideRemoveObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_Unlink(self)                     (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8UnlinkObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideUnlinkObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_RmDir(self)                      (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8RmDirObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideRmDirObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_MkDir(self)                      (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8MkDirObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideMkDirObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_Opendir(self)                    (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8OpendirObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideOpendirObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_Open(self,openmode,permissions)  (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8OpenObject(DeeVFSNativeNode_PATH(self),openmode,permissions) : DeeNFS_WideOpenObject(DeeVFSNativeNode_PATH(self),openmode,permissions))
#define DeeVFSNativeNode_NFS_Readlink(self)                   (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8ReadlinkObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideReadlinkObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_IsDrive(self)                    (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8IsDriveObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideIsDriveObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_NFS_IsMount(self)                    (DeeUtf8String_Check(DeeVFSNativeNode_PATH(self)) ? DeeNFS_Utf8IsMountObject(DeeVFSNativeNode_PATH(self)) : DeeNFS_WideIsMountObject(DeeVFSNativeNode_PATH(self)))
#define DeeVFSNativeNode_IsDrive(self) \
(DeeUtf8String_Check(DeeVFSNativeNode_PATH(self))\
 ? (DeeUtf8String_SIZE(DeeVFSNativeNode_PATH(self)) == 2 && DeeUtf8String_STR(DeeVFSNativeNode_PATH(self))[1] == ':')\
 : (DeeWideString_SIZE(DeeVFSNativeNode_PATH(self)) == 2 && DeeWideString_STR(DeeVFSNativeNode_PATH(self))[1] == ':'))


struct DeeVFSNativeFile {
 struct DeeVFSFile   vnf_file; /*< Underlying file. */
#ifdef DeeSysFileFD
 struct DeeSysFileFD vnf_fd;   /*< Native file descriptor. */
#endif
};
struct DeeVFSNativeView {
 struct DeeVFSView     vnv_view; /*< Underlying view. */
 // v TODO: This can be a 'DeeSysFSWideView'
 struct DeeNFSWideView vnv_dir;
};


extern void DEE_CALL _deevfs_nativefile_quit(struct DeeVFSNativeFile *self);
extern int DEE_CALL _deevfs_nativefile_vft_read(DEE_A_INOUT struct DeeVFSNativeFile *self, void *p, Dee_size_t s, Dee_size_t *rs);
extern int DEE_CALL _deevfs_nativefile_vft_write(DEE_A_INOUT struct DeeVFSNativeFile *self, void const *p, Dee_size_t s, Dee_size_t *ws);
extern int DEE_CALL _deevfs_nativefile_vft_seek(DEE_A_INOUT struct DeeVFSNativeFile *self, Dee_int64_t off, int whence, Dee_uint64_t *pos);
extern int DEE_CALL _deevfs_nativefile_vft_flush(DEE_A_INOUT struct DeeVFSNativeFile *self);
extern int DEE_CALL _deevfs_nativefile_vft_trunc(DEE_A_INOUT struct DeeVFSNativeFile *self);
extern DeeObject *DEE_CALL _deevfs_nativenode_vnt_nameof(struct DeeVFSNode *self, struct DeeVFSNativeNode *child);

DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#endif /* !GUARD_DEEMON_VFS_VFS_NATIVE_NODE_H */
