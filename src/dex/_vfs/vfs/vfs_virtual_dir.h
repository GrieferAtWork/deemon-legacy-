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
#ifndef GUARD_DEEMON_VFS_VFS_VIRTUAL_DIR_H
#define GUARD_DEEMON_VFS_VFS_VIRTUAL_DIR_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/atomic_mutex.h>
#include <deemon/sys/sysfd.h>
#include "vfs_core.h"

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// Native VFS node: A filesystem node that points back into the 
struct DeeVFSVirtualDirEntry {
 char const        *name; /* [1..1;term(0)] Name of this entry (NULL to terminate list). */
 struct DeeVFSNode *node; /* [1..1] Node associated with this entry. */
};
struct DeeVFSVirtualDirNode {
 struct DeeVFSNode                   vdn_node;     /*< Underlying node. */
 struct DeeVFSVirtualDirEntry const *vdn_children; /*< [1..1] Vector of children nodes. */
};
extern DEE_A_RET_EXCEPT_REF struct DeeVFSVirtualDirNode *DeeVFSVirtualDirNode_New(
 DEE_A_INOUT struct DeeVFSNode *parent, struct DeeVFSVirtualDirEntry const *children);
#define DeeVFSVirtualDirNode_INIT(parent,children) \
 {DeeVFSNode_INIT(&DeeVFSVirtualDirNode_Type,parent),children}

struct DeeVFSVirtualDirFile {
 struct DeeVFSFile vdf_file; /*< Underlying file. */
};
struct DeeVFSVirtualDirView {
 struct DeeVFSView                   vdv_view; /*< Underlying view. */
 struct DeeVFSVirtualDirEntry const *vdv_pos;  /*< [1..1] Next directory entry. */
 struct DeeAtomicMutex               vdv_lock; /*< Yield-lock. */
};

extern struct DeeVFSNodeType const DeeVFSVirtualDirNode_Type;
extern int DEE_CALL _deevfs_virtualdirview_vvt_curr(struct DeeVFSVirtualDirView *self, struct DeeVFSNode **result);
extern int DEE_CALL _deevfs_virtualdirview_vvt_yield(struct DeeVFSVirtualDirView *self, struct DeeVFSNode **result);

DEE_DECL_END

#endif /* !GUARD_DEEMON_VFS_VFS_VIRTUAL_DIR_H */
