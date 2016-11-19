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
#ifndef GUARD_DEEMON_VFS_VFS_VIRTUAL_DIR_C_INL
#define GUARD_DEEMON_VFS_VFS_VIRTUAL_DIR_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/string.h>
#include <deemon/vfs/vfs_core.h>
#include <deemon/vfs/vfs_virtual_dir.h>

#if DEE_CONFIG_RUNTIME_HAVE_VFS2
DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// Node VTable
static struct DeeVFSNode *DEE_CALL _deevfs_virtualdirnode_vnt_walk(
 struct DeeVFSVirtualDirNode *self, char const *name) {
 struct DeeVFSVirtualDirEntry *iter;
 iter = self->vdn_children;
 while (1) {
  if (!iter->name) break;
  DEE_ASSERT(iter->node);
  if (strcmp(iter->name,name) == 0) {
   DeeVFSNode_INCREF(iter->node);
   return iter->node;
  }
 }
 return NULL;
}
static DeeObject *DEE_CALL _deevfs_virtualdirnode_vnt_nameof(
 struct DeeVFSVirtualDirNode *self, struct DeeVFSNode *child) {
 struct DeeVFSVirtualDirEntry *iter;
 iter = self->vdn_children;
 while (1) {
  if (!iter->name) break;
  if (iter->node == child) return DeeString_New(iter->name);
 }
 return NULL;
}


//////////////////////////////////////////////////////////////////////////
// File VTable
static int DEE_CALL _deevfs_virtualdirfile_vft_open(
 struct DeeVFSVirtualDirFile *DEE_UNUSED(self),
 Dee_openmode_t DEE_UNUSED(openmode), Dee_mode_t DEE_UNUSED(permissions)) {
 return 0;
}


//////////////////////////////////////////////////////////////////////////
// View VTable
static int DEE_CALL _deevfs_virtualdirview_vvt_open(struct DeeVFSVirtualDirView *self) {
 DeeAtomicMutex_Init(&self->vdv_lock);
 self->vdv_pos = ((struct DeeVFSVirtualDirNode *)self->vdv_view.vv_node)->vdn_children;
 return 0;
}
static int DEE_CALL _deevfs_virtualdirview_vvt_yield(
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



struct DeeVFSNodeType const DeeVFSVirtualDirNode_Type = {
 { sizeof(struct DeeVFSVirtualDirNode), // vnt_node
  (void (DEE_CALL *)(struct DeeVFSNode *))                                                                                              NULL,
  (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,char const *))                                                                   &_deevfs_virtualdirnode_vnt_walk,
  (DeeObject *(DEE_CALL *)(struct DeeVFSNode *,struct DeeVFSNode *))                                                                    &_deevfs_virtualdirnode_vnt_nameof,
  (DeeObject *(DEE_CALL *)(struct DeeVFSNode *))                                                                                        NULL,
  (int (DEE_CALL *)(struct DeeVFSNode *))                                                                                               NULL,
  (int (DEE_CALL *)(struct DeeVFSNode *))                                                                                               NULL,
  (int (DEE_CALL *)(struct DeeVFSNode *))                                                                                               NULL,
  (int (DEE_CALL *)(struct DeeVFSNode *))                                                                                               NULL,
  (int (DEE_CALL *)(struct DeeVFSNode *))                                                                                               NULL,
  (int (DEE_CALL *)(struct DeeVFSNode *))                                                                                               NULL,
  (int (DEE_CALL *)(struct DeeVFSNode *))                                                                                               NULL,
  (int (DEE_CALL *)(struct DeeVFSNode *))                                                                                               NULL,
  (int (DEE_CALL *)(struct DeeVFSNode *,Dee_mode_t *))                                                                                  NULL,
  (int (DEE_CALL *)(struct DeeVFSNode *,Dee_mode_t ))                                                                                   NULL,
  (int (DEE_CALL *)(struct DeeVFSNode *,Dee_uid_t *,DEE_A_OUT Dee_gid_t *))                                                             NULL,
  (int (DEE_CALL *)(struct DeeVFSNode *,Dee_uid_t,DEE_A_IN Dee_gid_t))                                                                  NULL,
  (int (DEE_CALL *)(struct DeeVFSNode *,Dee_timetick_t *,DEE_A_OUT_OPT Dee_timetick_t *,DEE_A_OUT_OPT Dee_timetick_t *))                NULL,
  (int (DEE_CALL *)(struct DeeVFSNode *,Dee_timetick_t const *,DEE_A_IN_OPT Dee_timetick_t const *,DEE_A_IN_OPT Dee_timetick_t const *))NULL,
 },{ sizeof(struct DeeVFSVirtualDirFile),// vnt_file
  (int (DEE_CALL *)(struct DeeVFSFile *,Dee_openmode_t,Dee_mode_t))           &_deevfs_virtualdirfile_vft_open,
  (void(DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
  (int (DEE_CALL *)(struct DeeVFSFile *,void *,Dee_size_t,Dee_size_t *))      NULL,
  (int (DEE_CALL *)(struct DeeVFSFile *,void const *,Dee_size_t,Dee_size_t *))NULL,
  (int (DEE_CALL *)(struct DeeVFSFile *,Dee_int64_t,int,Dee_uint64_t *))      NULL,
  (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
  (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 },{ sizeof(struct DeeVFSVirtualDirView), // vnt_view
  (int (DEE_CALL *)(struct DeeVFSView *))                     &_deevfs_virtualdirview_vvt_open,
  (void (DEE_CALL *)(struct DeeVFSView *))                    NULL,
  (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))&_deevfs_virtualdirview_vvt_yield,
 }
};


DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */

#endif /* !GUARD_DEEMON_VFS_VFS_VIRTUAL_DIR_C_INL */
