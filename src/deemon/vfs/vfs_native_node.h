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
#include <deemon/vfs/vfs_core.h>
#include <deemon/sys/sysfd.h>

#if DEE_CONFIG_RUNTIME_HAVE_VFS2
DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// Native VFS node: A filesystem node that points back into the 
struct DeeVFSNativeNode {
 struct DeeVFSNode          vnn_node; /*< Underlying node. */
 DEE_A_REF DeeStringObject *vnn_path; /*< [1..1] Absolute, native path to this node. */
};
struct DeeVFSNativeFile {
 struct DeeVFSFile   vnf_file; /*< Underlying file. */
#ifdef DeeSysFileFD
 struct DeeSysFileFD vnf_fd;   /*< Native file descriptor. */
#endif
};
struct DeeVFSNativeView {
 struct DeeVFSView   vnv_view; /*< Underlying view. */
 // TODO: Implement 'sysdir'
};


extern struct DeeVFSNodeType const DeeVFSNativeNode_Type;


DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */

#endif /* !GUARD_DEEMON_VFS_VFS_NATIVE_NODE_H */
