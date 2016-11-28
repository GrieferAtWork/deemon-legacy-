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
#ifndef GUARD_DEEMON_VFS_VFS_VIRTUAL_LINK_H
#define GUARD_DEEMON_VFS_VFS_VIRTUAL_LINK_H 1

#include <deemon/__conf.inl>
#include "vfs_core.h"

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// Virtual link
struct DeeVFSVirtualLinkNode {
 struct DeeVFSNode             vln_node; /*< Underlying node. */
 DEE_A_REF DeeAnyStringObject *vln_link; /*< [1..1] Link path. */
};
extern struct DeeVFSNodeType const DeeVFSVirtualLinkNode_Type;
extern DEE_A_RET_EXCEPT_REF struct DeeVFSVirtualLinkNode *DeeVFSVirtualLinkNode_New(
 DEE_A_INOUT struct DeeVFSNode *parent, DEE_A_IN_OBJECT(DeeAnyStringObject) *lnk);

DEE_DECL_END

#endif /* !GUARD_DEEMON_VFS_VFS_VIRTUAL_LINK_H */
