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
#ifndef GUARD_DEEMON_VFS_VFS_VIRTUAL_DIR_C
#define GUARD_DEEMON_VFS_VFS_VIRTUAL_DIR_C 1
#ifndef DEE_EXTENSION
#define DEE_EXTENSION 1
#endif

#include <deemon/__conf.inl>
#include <deemon/string.h>
#include "vfs_core.h"
#include "vfs_virtual_dir.h"

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT_REF struct DeeVFSVirtualDirNode *DeeVFSVirtualDirNode_New(
 DEE_A_INOUT struct DeeVFSNode *parent, struct DeeVFSVirtualDirEntry const *children) {
 struct DeeVFSVirtualDirNode *result;
 if DEE_UNLIKELY((result = DeeVFSNode_ALLOC(struct DeeVFSVirtualDirNode)) == NULL) return NULL;
 result->vdn_children = children;
 DeeVFSNode_InitWithParent(&result->vdn_node,&DeeVFSVirtualDirNode_Type,parent);
 return result;
}


//////////////////////////////////////////////////////////////////////////
// Node VTable
static struct DeeVFSNode *DEE_CALL _deevfs_virtualdirnode_vnt_walk(
 struct DeeVFSVirtualDirNode *self, char const *name, Dee_size_t name_size) {
 struct DeeVFSVirtualDirEntry const *iter;
 iter = self->vdn_children;
 while (1) {
  if (!iter->name) break;
  DEE_ASSERT(iter->node);
  if (strncmp(iter->name,name,name_size) == 0) {
   DeeVFSNode_INCREF(iter->node);
   return iter->node;
  }
  ++iter;
 }
 DeeError_SetStringf(&DeeErrorType_SystemError,
                     "Virtual directory %R does not contain a node %q",
                     DeeVFSNode_Filename((struct DeeVFSNode *)self),
                     name);
 return NULL;
}
static DeeObject *DEE_CALL _deevfs_virtualdirnode_vnt_nameof(
 struct DeeVFSVirtualDirNode *self, struct DeeVFSNode *child) {
 struct DeeVFSVirtualDirEntry const *iter;
 iter = self->vdn_children;
 while (1) {
  if (!iter->name) break;
  if (iter->node == child) return DeeString_New(iter->name);
  ++iter;
 }
 return NULL;
}

//////////////////////////////////////////////////////////////////////////
// View VTable
static int DEE_CALL _deevfs_virtualdirview_vvt_open(struct DeeVFSVirtualDirView *self) {
 DeeAtomicMutex_Init(&self->vdv_lock);
 self->vdv_pos = ((struct DeeVFSVirtualDirNode *)self->vdv_view.vv_node)->vdn_children;
 return 0;
}
int DEE_CALL _deevfs_virtualdirview_vvt_curr(
 struct DeeVFSVirtualDirView *self, struct DeeVFSNode **result) {
 DeeAtomicMutex_Acquire(&self->vdv_lock);
 DEE_ASSERT(self->vdv_pos);
 if (!self->vdv_pos->name) {
  DeeAtomicMutex_Release(&self->vdv_lock);
  return 1;
 }
 *result = self->vdv_pos->node;
 DeeVFSNode_INCREF(*result);
 DeeAtomicMutex_Release(&self->vdv_lock);
 return 0;
}
int DEE_CALL _deevfs_virtualdirview_vvt_yield(
 struct DeeVFSVirtualDirView *self, struct DeeVFSNode **result) {
 DeeAtomicMutex_Acquire(&self->vdv_lock);
 DEE_ASSERT(self->vdv_pos);
 if (!self->vdv_pos->name) {
  DeeAtomicMutex_Release(&self->vdv_lock);
  return 1;
 }
 *result = (self->vdv_pos++)->node;
 DeeVFSNode_INCREF(*result);
 DeeAtomicMutex_Release(&self->vdv_lock);
 return 0;
}



static struct _DeeVFSViewTypeData _deevfs_virtualdirnode_vnt_view = {
 sizeof(struct DeeVFSVirtualDirView),
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,Dee_Utf8Char const *,Dee_size_t))&_deevfs_virtualdirnode_vnt_walk,
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,Dee_WideChar const *,Dee_size_t))NULL,
 (DeeObject *(DEE_CALL *)(struct DeeVFSNode *,struct DeeVFSNode *))                    &_deevfs_virtualdirnode_vnt_nameof,
 (int (DEE_CALL *)(struct DeeVFSView *))                                               &_deevfs_virtualdirview_vvt_open,
 (void (DEE_CALL *)(struct DeeVFSView *))                                              NULL,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_virtualdirview_vvt_curr,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_virtualdirview_vvt_yield,
};

struct DeeVFSNodeType const DeeVFSVirtualDirNode_Type = {
 {NULL,NULL},NULL,DeeVFSNoopNodeType_FileData,
 &_deevfs_virtualdirnode_vnt_view
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_VFS_VFS_VIRTUAL_DIR_C */
