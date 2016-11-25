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
#ifndef GUARD_DEEMON_VFS_VFS_VIRTUAL_RANDOMFILE_C_INL
#define GUARD_DEEMON_VFS_VFS_VIRTUAL_RANDOMFILE_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/vfs/vfs_core.h>
#include <deemon/vfs/vfs_virtual_randomfile.h>

#if DEE_CONFIG_RUNTIME_HAVE_VFS
DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// File VTable
static int DEE_CALL _deevfs_randomfile_vft_open(
 struct DeeVFSRandomFile *self, Dee_openmode_t DEE_UNUSED(openmode),
 Dee_mode_t DEE_UNUSED(permissions)) {
 if DEE_UNLIKELY(!CryptAcquireContext(&self->vr_hCryptProv,NULL,NULL,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,"CryptAcquireContext(...) : %K",
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}
static void DEE_CALL _deevfs_randomfile_vft_quit(
 struct DeeVFSRandomFile *self) {
 CryptReleaseContext(self->vr_hCryptProv,0);
}
static int DEE_CALL _deevfs_randomfile_vft_read(
 struct DeeVFSRandomFile *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 *rs = s;
#ifdef DEE_PLATFORM_64_BIT
 while (s) {
  DWORD chunk = (DWORD)(s > 0xFFFFFFFF ? 0xFFFFFFFF : s);
  if DEE_UNLIKELY(!CryptGenRandom(self->vr_hCryptProv,(DWORD)chunk,(BYTE *)p))
#else
  if DEE_UNLIKELY(!CryptGenRandom(self->vr_hCryptProv,(DWORD)s,(BYTE *)p))
#endif
  {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "CryptGenRandom(%#p,%I32u,%#p) : %K",self->vr_hCryptProv,(DWORD)s,p,
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
   return -1;
  }
#ifdef DEE_PLATFORM_64_BIT
  *(char **)&p += chunk;
  s -= chunk;
 }
#endif
 return 0;
}
static int DEE_CALL _deevfs_urandomfile_vft_open(
 struct DeeVFSURandomFile *self,
 Dee_openmode_t DEE_UNUSED(openmode),
 Dee_mode_t DEE_UNUSED(permissions)) {
 DeeRandomNumberGenerator_Randomize(&self->vur_rng);
 return 0;
}
static int DEE_CALL _deevfs_urandomfile_vft_read(
 struct DeeVFSURandomFile *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 DeeRandomNumberGenerator_NextData(&self->vur_rng,p,s);
 *rs = s;
 return 0;
}

static struct _DeeVFSFileTypeData _deevfs_randomfile_vft_file = {
 sizeof(struct DeeVFSRandomFile),
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_openmode_t,Dee_mode_t))           &_deevfs_randomfile_vft_open,
 (void(DEE_CALL *)(struct DeeVFSFile *))                                     &_deevfs_randomfile_vft_quit,
 (int (DEE_CALL *)(struct DeeVFSFile *,void *,Dee_size_t,Dee_size_t *))      &_deevfs_randomfile_vft_read,
 (int (DEE_CALL *)(struct DeeVFSFile *,void const *,Dee_size_t,Dee_size_t *))NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_int64_t,int,Dee_uint64_t *))      NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
};
static struct _DeeVFSFileTypeData _deevfs_urandomfile_vft_file = {
 sizeof(struct DeeVFSURandomFile),
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_openmode_t,Dee_mode_t))           &_deevfs_urandomfile_vft_open,
 (void(DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *,void *,Dee_size_t,Dee_size_t *))      &_deevfs_urandomfile_vft_read,
 (int (DEE_CALL *)(struct DeeVFSFile *,void const *,Dee_size_t,Dee_size_t *))NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_int64_t,int,Dee_uint64_t *))      NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
};

struct DeeVFSNodeType const DeeVFSVirtualRandomFileNode_Type = {
 {NULL,NULL},NULL,&_deevfs_randomfile_vft_file,NULL};
struct DeeVFSNodeType const DeeVFSVirtualURandomFileNode_Type = {
 {NULL,NULL},NULL,&_deevfs_urandomfile_vft_file,NULL};

DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#endif /* !GUARD_DEEMON_VFS_VFS_VIRTUAL_RANDOMFILE_C_INL */
