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
#ifndef GUARD_DEEMON_VFS_VFS_VIRTUAL_STDFILE_C_INL
#define GUARD_DEEMON_VFS_VFS_VIRTUAL_STDFILE_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/string.h>
#include <deemon/vfs/vfs_core.h>
#include <deemon/vfs/vfs_native_node.h>
#include <deemon/vfs/vfs_virtual_stdfile.h>

#if DEE_CONFIG_RUNTIME_HAVE_VFS2
DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// File VTable
static int DEE_CALL _deevfs_virtualstdfilefile_vft_open(
 struct DeeVFSNativeFile *self,
 Dee_openmode_t DEE_UNUSED(openmode), Dee_mode_t DEE_UNUSED(permissions)) {
 self->vnf_fd.w32_handle = GetStdHandle(((
  struct DeeVFSVirtualStdFileNode *)self->vnf_file.vf_node)->std_handleid);
 self->vnf_fd.w32_openmode = DEE_OPENMODE('w',1);
 return 0;
}



struct DeeVFSNodeType const DeeVFSVirtualStdFileNode_Type = {
 { sizeof(struct DeeVFSVirtualStdFileNode), // vnt_node
  (void (DEE_CALL *)(struct DeeVFSNode *))                                                                                              NULL,
  (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,char const *))                                                                   NULL,
  (DeeObject *(DEE_CALL *)(struct DeeVFSNode *,struct DeeVFSNode *))                                                                    NULL,
  (DeeObject *(DEE_CALL *)(struct DeeVFSNode *))                                                                                        NULL,
  (int (DEE_CALL *)(struct DeeVFSNode *))                                                                                               NULL,
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
 },{ sizeof(struct DeeVFSNativeFile),// vnt_file
  (int (DEE_CALL *)(struct DeeVFSFile *,Dee_openmode_t,Dee_mode_t))           &_deevfs_virtualstdfilefile_vft_open,
  (void(DEE_CALL *)(struct DeeVFSFile *))                                     &_deevfs_nativefile_quit,
  (int (DEE_CALL *)(struct DeeVFSFile *,void *,Dee_size_t,Dee_size_t *))      &_deevfs_nativefile_vft_read,
  (int (DEE_CALL *)(struct DeeVFSFile *,void const *,Dee_size_t,Dee_size_t *))&_deevfs_nativefile_vft_write,
  (int (DEE_CALL *)(struct DeeVFSFile *,Dee_int64_t,int,Dee_uint64_t *))      &_deevfs_nativefile_vft_seek,
  (int (DEE_CALL *)(struct DeeVFSFile *))                                     &_deevfs_nativefile_vft_flush,
  (int (DEE_CALL *)(struct DeeVFSFile *))                                     &_deevfs_nativefile_vft_trunc,
 },{ 0, // vnt_view
  (int (DEE_CALL *)(struct DeeVFSView *))                     NULL,
  (void (DEE_CALL *)(struct DeeVFSView *))                    NULL,
  (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))NULL,
 }
};


DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */

#endif /* !GUARD_DEEMON_VFS_VFS_VIRTUAL_STDFILE_C_INL */
