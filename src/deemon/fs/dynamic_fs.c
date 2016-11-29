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
#include <deemon/dex.h>
#include <deemon/fs/dynamic_fs.h>
#include <deemon/fs/expand_fs.h>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN

#define DeeFSApi DeeFSApi_v102

#ifdef DEE_FSAPIMODE_ENABLEVFS
#define DEE_FSAPIMODE_MASK (DEE_FSAPIMODE_EXPANDVARS|DEE_FSAPIMODE_ENABLEVFS)
#else
#define DEE_FSAPIMODE_MASK  DEE_FSAPIMODE_EXPANDVARS
#endif


//////////////////////////////////////////////////////////////////////////
// Dynamic FS API
static struct DeeAtomicMutex DeeFSApi_ActiveLock = DeeAtomicMutex_INIT();
static struct DeeFSApi DeeFSApi_Active = {
#define REG_ENTRY(T,name) &DeeNFS_##name,
#define DIR_ENTRY(T,name) &DeeNFS_##name,
#if (DEE_FSAPIMODE_DEFAULT&DEE_FSAPIMODE_EXPANDVARS)!=0
#define IND_ENTRY(T,name) &DeeXFS_##name,
#else
#define IND_ENTRY(T,name) &DeeNFS_##name,
#endif
#include "dynamic_fs.v102.inl"
};

DEE_A_RET_NEVER_NULL DeeFunPointer DeeFS_GetFunction_v102(DEE_A_IN Dee_size_t id) {
 DeeFunPointer result;
 DEE_ASSERTF(id < sizeof(struct DeeFSApi),"Invalid id: out of bounds");
 DEE_ASSERTF((id%sizeof(DeeFunPointer))==0,"Invalid id: Not pointer aligned");
 DeeAtomicMutex_Acquire(&DeeFSApi_ActiveLock);
 result = *(DeeFunPointer *)(((Dee_uintptr_t)&DeeFSApi_Active)+id);
 DeeAtomicMutex_Release(&DeeFSApi_ActiveLock);
 return result;
}





DEE_A_RET_EXCEPT(-1) int DeeFS_DoGetApi_v102(
 DEE_A_IN Dee_fsapimode_t mode, DEE_A_OUT struct DeeFSApi_v102 *api,
 DEE_A_IN_OPT struct DeeAtomicMutex *lock) {
 if ((mode&~(DEE_FSAPIMODE_MASK))!=0) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Invalid FS-ApiMode: %#.8I32x",mode);
  return -1;
 }
 // Load regular entries
#define DIR_ENTRY(T,name) /* nothing */
#define REG_ENTRY(T,name) api->fs102_##name = &DeeNFS_##name;
#define IND_ENTRY(T,name) /* nothing */
#include "dynamic_fs.v102.inl"
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if ((mode&DEE_FSAPIMODE_ENABLEVFS)!=0) {
  typedef DeeFunPointer (DEE_CALL *LPDEEVFS_GETFUNCTION_V102)(Dee_size_t id);
  LPDEEVFS_GETFUNCTION_V102 import; DeeObject *vfs_dex;
  if DEE_UNLIKELY((vfs_dex = DeeDex_Open("_vfs")) == NULL) {
   if (!DeeError_Catch(&DeeErrorType_DexError)) return -1;
   goto map_without_vfs;
  }
  *(void **)&import = DeeDexModule_TryGetNativeAddress(vfs_dex,"DeeVFS_GetFunction_v102");
  if (!import) { Dee_DECREF(vfs_dex); goto map_without_vfs; }
  // Link against the virtual file system
  // NOTE: Functions not exported by the VFS are linked against the NFS as fallback
  // Try to import functions from the VFS dex
  if (lock) DeeAtomicMutex_Acquire(lock);
#define DIR_ENTRY(T,name) \
 if ((*(DeeFunPointer *)&api->_fs102_##name =\
     (*import)(DeeVFSApiID_v102__##name*DEE_TYPES_SIZEOF_POINTER)) == NULL)\
  api->_fs102_##name = &DeeNFS_##name;
#define REG_ENTRY(T,name) /* nothing */
#define IND_ENTRY(T,name) /* nothing */
#include "dynamic_fs.v102.inl"
  Dee_DECREF(vfs_dex);
 } else map_without_vfs:
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */
 {
  if (lock) DeeAtomicMutex_Acquire(lock);
  // Directly link against the native filesystem
#define DIR_ENTRY(T,name) api->_fs102_##name = &DeeNFS_##name;
#define REG_ENTRY(T,name) /* nothing */
#define IND_ENTRY(T,name) /* nothing */
#include "dynamic_fs.v102.inl"
 }
 if ((mode&DEE_FSAPIMODE_EXPANDVARS)!=0) {
  // Link against the expand-fs functions.
#define DIR_ENTRY(T,name) /* nothing */
#define REG_ENTRY(T,name) /* nothing */
#define IND_ENTRY(T,name) api->fs102_##name = &DeeXFS_##name;
#include "dynamic_fs.v102.inl"
 } else {
#define DIR_ENTRY(T,name) /* nothing */
#define REG_ENTRY(T,name) /* nothing */
#define IND_ENTRY(T,name) api->fs102_##name = api->_fs102_##name;
#include "dynamic_fs.v102.inl"
 }
 if (lock) DeeAtomicMutex_Release(lock);
 return 0;
}

DEE_A_RET_WUNUSED Dee_fsapimode_t DeeFS_GetAPIMode(void) {
 Dee_fsapimode_t result = DEE_FSAPIMODE_NONE;
 DeeAtomicMutex_Acquire(&DeeFSApi_ActiveLock);
 if (DeeFSApi_Active._fs102_Utf8Chdir == &DeeXFS_Utf8Chdir) result |= DEE_FSAPIMODE_EXPANDVARS;
#if DEE_CONFIG_RUNTIME_HAVE_VFS
 if (DeeFSApi_Active._fs102_Utf8GetCwd != &DeeNFS_Utf8GetCwd) result |= DEE_FSAPIMODE_ENABLEVFS;
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */
 DeeAtomicMutex_Release(&DeeFSApi_ActiveLock);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeFS_SetAPIMode(DEE_A_IN Dee_fsapimode_t mode) {
 return DeeFS_DoGetApi_v102(mode,&DeeFSApi_Active,&DeeFSApi_ActiveLock);
}

DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeFS_GetApi(
 DEE_A_IN int version, DEE_A_IN Dee_fsapimode_t mode,
 DEE_A_OUT_W(s) void *api, DEE_A_IN Dee_size_t s,
 DEE_A_OUT_OPT Dee_size_t *required_size) {
 DEE_ASSERT(api || !s);
 if (version != 102) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Unsupported version: %d",
                      version);
  return -1;
 }
 if (required_size) *required_size = sizeof(struct DeeFSApi_v102);
 if (s < sizeof(struct DeeFSApi_v102)) return 1; // Buffer too small
 return DeeFS_DoGetApi_v102(mode,(struct DeeFSApi_v102 *)api,NULL);
}


#undef MAP_PUBLIC_FSAPI_EXPORTS
#undef MAP_PRIVATE_FSAPI_EXPORTS

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_DYNAMIC_FS_C */
