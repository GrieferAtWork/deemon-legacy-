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
#ifndef GUARD_DEEMON_VFS_VFS_VIRTUAL_LINK_C_INL
#define GUARD_DEEMON_VFS_VFS_VIRTUAL_LINK_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/string.h>
#include <deemon/vfs/vfs_core.h>
#include <deemon/vfs/vfs_virtual_link.h>
#include <deemon/type.h>

#if DEE_CONFIG_RUNTIME_HAVE_VFS
DEE_DECL_BEGIN

DEE_A_RET_EXCEPT_REF struct DeeVFSVirtualLinkNode *DeeVFSVirtualLinkNode_New(
 DEE_A_INOUT struct DeeVFSNode *parent, DEE_A_IN_OBJECT(DeeAnyStringObject) *lnk) {
 struct DeeVFSVirtualLinkNode *result;
 DEE_ASSERT(DeeObject_Check(lnk) && DeeAnyString_Check(lnk));
 if DEE_UNLIKELY((result = DeeVFSNode_ALLOC(struct DeeVFSVirtualLinkNode)) == NULL) return NULL;
 DeeVFSNode_InitWithParent(&result->vln_node,&DeeVFSVirtualLinkNode_Type,parent);
 Dee_INCREF(result->vln_link = (DeeAnyStringObject *)lnk);
 return result;
}
static DeeObject *DEE_CALL _deevfsvirtuallink_vnt_readlink(
 struct DeeVFSVirtualLinkNode *self) {
 DeeReturn_NEWREF((DeeObject *)self->vln_link);
}

struct DeeVFSNodeType const DeeVFSVirtualLinkNode_Type = {
 {NULL,(DeeObject*(*)(struct DeeVFSNode *))&_deevfsvirtuallink_vnt_readlink},
 NULL,DeeVFSNoopNodeType_FileData,NULL};


DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#endif /* !GUARD_DEEMON_VFS_VFS_VIRTUAL_FILE_C_INL */
