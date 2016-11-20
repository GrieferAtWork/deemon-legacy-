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
#ifndef GUARD_DEEMON_FS_DYNAMIC_FS_C
#define GUARD_DEEMON_FS_DYNAMIC_FS_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include <deemon/fs_api.h>
#include <deemon/fs/native_fs.h>
#include <deemon/fs/virtual_fs.h>

DEE_DECL_BEGIN

#ifdef DEE_FSAPIMODE_ENABLEVFS
#define DEE_FSAPIMODE_MASK (DEE_FSAPIMODE_EXPANDVARS|DEE_FSAPIMODE_ENABLEVFS)
#else
#define DEE_FSAPIMODE_MASK  DEE_FSAPIMODE_EXPANDVARS
#endif


//////////////////////////////////////////////////////////////////////////
// Dynamic FS API

struct DeeFSApi {
 PDEEFS_UTF8GETCWD       fa_utf8getcwd;
 PDEEFS_WIDEGETCWD       fa_widegetcwd;
 PDEEFS_UTF8CHDIR        fa_utf8chdir;
 PDEEFS_WIDECHDIR        fa_widechdir;
 PDEEFS_UTF8CHDIROBJECT  fa_utf8chdirobject;
 PDEEFS_WIDECHDIROBJECT  fa_widechdirobject;
 PDEEFS_UTF8CHDIR        _fa_utf8chdir;
 PDEEFS_WIDECHDIR        _fa_widechdir;
 PDEEFS_UTF8CHDIROBJECT  _fa_utf8chdirobject;
 PDEEFS_WIDECHDIROBJECT  _fa_widechdirobject;
};

#if DEE_CONFIG_RUNTIME_HAVE_VFS2 && ((\
    DEE_FSAPIMODE_DEFAULT&DEE_FSAPIMODE_ENABLEVFS)!=0)
#define _DeeDFS(x) DeeVFS_##x
#else
#define _DeeDFS(x) DeeNFS_##x
#endif
#define DeeDFS _DeeDFS

static struct DeeAtomicMutex DeeFSApi_ActiveLock = DeeAtomicMutex_INIT();
static struct DeeFSApi DeeFSApi_Active = {
 DeeDFS(Utf8GetCwd),       /*< fa_utf8getcwd. */
 DeeDFS(WideGetCwd),       /*< fa_widegetcwd. */
 DeeDFS(Utf8Chdir),        /*< fa_utf8chdir. */
 DeeDFS(WideChdir),        /*< fa_widechdir. */
 DeeDFS(Utf8ChdirObject),  /*< fa_utf8chdirobject. */
 DeeDFS(WideChdirObject),  /*< fa_widechdirobject. */
 _DeeDFS(Utf8Chdir),       /*< _fa_utf8chdir. */
 _DeeDFS(WideChdir),       /*< _fa_widechdir. */
 _DeeDFS(Utf8ChdirObject), /*< _fa_utf8chdirobject. */
 _DeeDFS(WideChdirObject), /*< _fa_widechdirobject. */
};

DEE_A_RET_NEVER_NULL DeeFunPointer DeeFS_GetFunction(DEE_A_IN Dee_size_t id) {
 DeeFunPointer result;
 DEE_ASSERTF(id < sizeof(struct DeeFSApi),"Invalid id: out of bounds");
 DEE_ASSERTF((id%sizeof(DeeFunPointer))==0,"Invalid id: Not pointer aligned");
 DeeAtomicMutex_Acquire(&DeeFSApi_ActiveLock);
 result = *(DeeFunPointer *)(((Dee_uintptr_t)&DeeFSApi_Active)+id);
 DeeAtomicMutex_Release(&DeeFSApi_ActiveLock);
 return result;
}

DEE_A_RET_WUNUSED Dee_fsapimode_t DeeFS_GetAPIMode(void) {
 Dee_fsapimode_t result = DEE_FSAPIMODE_NONE;
 DeeAtomicMutex_Acquire(&DeeFSApi_ActiveLock);
 // TODO: DEE_FSAPIMODE_EXPANDVARS
#if DEE_CONFIG_RUNTIME_HAVE_VFS2
 if (DeeFSApi_Active.fa_utf8getcwd == &DeeVFS_Utf8GetCwd)
  result |= DEE_FSAPIMODE_ENABLEVFS;
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */
 DeeAtomicMutex_Release(&DeeFSApi_ActiveLock);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeFS_SetAPIMode(DEE_A_IN Dee_fsapimode_t mode) {
 if ((mode&~(DEE_FSAPIMODE_MASK))!=0) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Invalid FS-ApiMode: %#.8I32x",mode);
  return -1;
 }
 DeeAtomicMutex_Acquire(&DeeFSApi_ActiveLock);
#if DEE_CONFIG_RUNTIME_HAVE_VFS2
 if ((mode&DEE_FSAPIMODE_ENABLEVFS)!=0) {
  DeeFSApi_Active.fa_utf8getcwd       = &DeeVFS_Utf8GetCwd;
  DeeFSApi_Active.fa_widegetcwd       = &DeeVFS_WideGetCwd;
  DeeFSApi_Active._fa_utf8chdir       = &DeeVFS_Utf8Chdir;
  DeeFSApi_Active._fa_widechdir       = &DeeVFS_WideChdir;
  DeeFSApi_Active._fa_utf8chdirobject = &DeeVFS_Utf8ChdirObject;
  DeeFSApi_Active._fa_widechdirobject = &DeeVFS_WideChdirObject;
 } else
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */
 {
  DeeFSApi_Active.fa_utf8getcwd       = &DeeNFS_Utf8GetCwd;
  DeeFSApi_Active.fa_widegetcwd       = &DeeNFS_WideGetCwd;
  DeeFSApi_Active._fa_utf8chdir       = &DeeNFS_Utf8Chdir;
  DeeFSApi_Active._fa_widechdir       = &DeeNFS_WideChdir;
  DeeFSApi_Active._fa_utf8chdirobject = &DeeNFS_Utf8ChdirObject;
  DeeFSApi_Active._fa_widechdirobject = &DeeNFS_WideChdirObject;
 }
 // TODO: DEE_FSAPIMODE_EXPANDVARS
 DeeFSApi_Active.fa_utf8chdir       = DeeFSApi_Active._fa_utf8chdir;
 DeeFSApi_Active.fa_widechdir       = DeeFSApi_Active._fa_widechdir;
 DeeFSApi_Active.fa_utf8chdirobject = DeeFSApi_Active._fa_utf8chdirobject;
 DeeFSApi_Active.fa_widechdirobject = DeeFSApi_Active._fa_widechdirobject;
 DeeAtomicMutex_Release(&DeeFSApi_ActiveLock);
 return 0;
}


DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_DYNAMIC_FS_C */
