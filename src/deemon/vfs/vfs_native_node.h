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
 struct DeeVFSNode          vnn_node; /*< Underlying node. */
 DEE_A_REF DeeStringObject *vnn_path; /*< [1..1] Absolute, native path to this node (NOT trail terminated). */
};
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
