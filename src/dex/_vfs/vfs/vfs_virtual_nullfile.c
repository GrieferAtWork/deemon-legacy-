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
#ifndef GUARD_DEEMON_VFS_VFS_VIRTUAL_NULLFILE_C
#define GUARD_DEEMON_VFS_VFS_VIRTUAL_NULLFILE_C 1
#ifndef DEE_EXTENSION
#define DEE_EXTENSION 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include "__vfsconf.inl"
#include "vfs_core.h"
#include "vfs_virtual_nullfile.h"

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// File VTable
#if DEE_VFSCONFIG_HAVEFILE_DEV_NULL \
 || DEE_VFSCONFIG_HAVEFILE_DEV_ZERO \
 || DEE_VFSCONFIG_HAVEFILE_DEV_FULL
static int DEE_CALL _deevfs_nullfile_vft_flush(struct DeeVFSFile *DEE_UNUSED(self)) { return 0; }
static int DEE_CALL _deevfs_nullfile_vft_trunc(struct DeeVFSFile *DEE_UNUSED(self)) { return 0; }
#endif /* ... */

#if DEE_VFSCONFIG_HAVEFILE_DEV_NULL \
 || DEE_VFSCONFIG_HAVEFILE_DEV_ZERO
static int DEE_CALL _deevfs_nullfile_vft_write(struct DeeVFSFile *DEE_UNUSED(self), void const *DEE_UNUSED(p), Dee_size_t s, Dee_size_t *ws) { *ws = s; return 0; }
#endif /* ... */

#if DEE_VFSCONFIG_HAVEFILE_DEV_ZERO \
 || DEE_VFSCONFIG_HAVEFILE_DEV_FULL
static int DEE_CALL _deevfs_zerofile_vft_read(struct DeeVFSFile *DEE_UNUSED(self), void *p, Dee_size_t s, Dee_size_t *rs) { memset(p,0,*rs = s); return 0; }
static int DEE_CALL _deevfs_zerofile_vft_seek(struct DeeVFSFile *DEE_UNUSED(self), Dee_int64_t DEE_UNUSED(off), int DEE_UNUSED(whence), Dee_uint64_t *pos) { if (pos) *pos = (Dee_uint64_t)-1; return 0; }
#endif /* ... */

#if DEE_VFSCONFIG_HAVEFILE_DEV_NULL
static int DEE_CALL _deevfs_nullfile_vft_read(struct DeeVFSFile *DEE_UNUSED(self), void *DEE_UNUSED(p), Dee_size_t DEE_UNUSED(s), Dee_size_t *rs) { *rs = 0; return 0; }
static int DEE_CALL _deevfs_nullfile_vft_seek(struct DeeVFSFile *DEE_UNUSED(self), Dee_int64_t DEE_UNUSED(off), int DEE_UNUSED(whence), Dee_uint64_t *pos) { if (pos) *pos = 0; return 0; }
#endif /* DEE_VFSCONFIG_HAVEFILE_DEV_NULL */

#if DEE_VFSCONFIG_HAVEFILE_DEV_FULL
static int DEE_CALL _deevfs_fullfile_vft_write(
 struct DeeVFSFile *DEE_UNUSED(self), void const *DEE_UNUSED(p),
 Dee_size_t DEE_UNUSED(s), Dee_size_t *DEE_UNUSED(ws)) {
 DeeError_SetStringf(&DeeErrorType_IOError,
                     "write(\"/dev/full\") : %K",
                     DeeSystemError_Win32ToString(ERROR_DISK_FULL));
 return -1;
}
#endif /* DEE_VFSCONFIG_HAVEFILE_DEV_FULL */

#if DEE_VFSCONFIG_HAVEFILE_DEV_NULL
static struct _DeeVFSFileTypeData _deevfs_nullfile_vft_file = {
 sizeof(struct DeeVFSFile),
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_openmode_t,Dee_mode_t))           &_deevfs_genericfile_openany,
 (void(DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *,void *,Dee_size_t,Dee_size_t *))      &_deevfs_nullfile_vft_read,
 (int (DEE_CALL *)(struct DeeVFSFile *,void const *,Dee_size_t,Dee_size_t *))&_deevfs_nullfile_vft_write,
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_int64_t,int,Dee_uint64_t *))      &_deevfs_nullfile_vft_seek,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     &_deevfs_nullfile_vft_flush,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     &_deevfs_nullfile_vft_trunc,
};
struct DeeVFSNodeType const DeeVFSVirtualNULLFileNode_Type = {
 {NULL,NULL},NULL,&_deevfs_nullfile_vft_file,NULL};
#endif /* DEE_VFSCONFIG_HAVEFILE_DEV_NULL */

#if DEE_VFSCONFIG_HAVEFILE_DEV_ZERO
static struct _DeeVFSFileTypeData _deevfs_zerofile_vft_file = {
 sizeof(struct DeeVFSFile),
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_openmode_t,Dee_mode_t))           &_deevfs_genericfile_openany,
 (void(DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *,void *,Dee_size_t,Dee_size_t *))      &_deevfs_zerofile_vft_read,
 (int (DEE_CALL *)(struct DeeVFSFile *,void const *,Dee_size_t,Dee_size_t *))&_deevfs_nullfile_vft_write,
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_int64_t,int,Dee_uint64_t *))      &_deevfs_zerofile_vft_seek,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     &_deevfs_nullfile_vft_flush,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     &_deevfs_nullfile_vft_trunc,
};
struct DeeVFSNodeType const DeeVFSVirtualZEROFileNode_Type = {
 {NULL,NULL},NULL,&_deevfs_zerofile_vft_file,NULL};
#endif /* DEE_VFSCONFIG_HAVEFILE_DEV_ZERO */

#if DEE_VFSCONFIG_HAVEFILE_DEV_FULL
static struct _DeeVFSFileTypeData _deevfs_fullfile_vft_file = {
 sizeof(struct DeeVFSFile),
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_openmode_t,Dee_mode_t))           &_deevfs_genericfile_openany,
 (void(DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *,void *,Dee_size_t,Dee_size_t *))      &_deevfs_zerofile_vft_read,
 (int (DEE_CALL *)(struct DeeVFSFile *,void const *,Dee_size_t,Dee_size_t *))&_deevfs_fullfile_vft_write,
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_int64_t,int,Dee_uint64_t *))      &_deevfs_zerofile_vft_seek,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     &_deevfs_nullfile_vft_flush,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     &_deevfs_nullfile_vft_trunc,
};
struct DeeVFSNodeType const DeeVFSVirtualFULLFileNode_Type = {
 {NULL,NULL},NULL,&_deevfs_fullfile_vft_file,NULL};
#endif /* DEE_VFSCONFIG_HAVEFILE_DEV_FULL */

DEE_DECL_END

#endif /* !GUARD_DEEMON_VFS_VFS_VIRTUAL_NULLFILE_C */
