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
#ifndef GUARD_DEEMON_DEX_VFS_C
#define GUARD_DEEMON_DEX_VFS_C 1
#define DEE_EXTENSION 1

#include "_vfs.h"
#include "vfs/vfs_core.h"
#include "vfs/virtual_fs.h"
#include "vfs/native_hooks.h"
#include <deemon/optional/fs_api.fsapimode.h>
#include <deemon/dex.h>
#include <deemon/fs/dynamic_fs.h>

DEE_DECL_BEGIN

static struct DeeFSApi_v102 const DeeVFSApi_Exports = {
#define REG_ENTRY(T,name) NULL,
#define DIR_ENTRY(T,name) &DeeVFS_##name,
#define IND_ENTRY(T,name) NULL,
#include <deemon/fs/dynamic_fs.v102.inl>
};

DEE_ATTRIBUTE_DLLEXPORT DEE_A_RET_NEVER_NULL DeeFunPointer
DeeVFS_GetFunction_v102(DEE_A_IN DEE_TYPES_UINT(DEE_TYPES_SIZEOF_SIZE_T) id) {
 DEE_ASSERTF(id < sizeof(struct DeeFSApi_v102),"Invalid id: out of bounds");
 DEE_ASSERTF((id%sizeof(DeeFunPointer))==0,"Invalid id: Not pointer aligned");
 return *(DeeFunPointer *)(((Dee_uintptr_t)&DeeVFSApi_Exports)+id);
}

DEE_DECL_END

#include <deemon/none.h>
#include <deemon/sys/systhread.h>

DEE_DECL_BEGIN
static DeeObject *DEE_CALL test(DeeObject *) {
 struct DeeSysThread _tid_self; Dee_tid_t a,b;
 DeeSysThread_GetSelf(&_tid_self);
 DeeSysThread_GetTID(&_tid_self,&a);
 DeeSysThread_GetSelfTID(&b);
 DEE_LDEBUG("a: %lu\n",a);
 DEE_LDEBUG("b: %lu\n",b);

 DeeReturn_None;
}

//////////////////////////////////////////////////////////////////////////
// Module exports
struct DeeDexExportDef DeeDex_Exports[] = {
 DeeDex_EXPORT_FUNCTION("test",&test,"() -> none"),
 DeeDex_EXPORT_END
};



#ifndef DEE_FSAPIMODE_ENABLEVFS
#define DEE_FSAPIMODE_ENABLEVFS  DEE_UINT16_C(0x0002)
#endif

// v from "native_hooks.h"
struct DEE_PP_CAT_2(DeeFSApi_v,DEE_VFS_HFSVERSION) DeeVFS_NativeHooks;

int DeeDex_Main(DEE_A_INOUT DeeDexContext *context) {
 Dee_fsapimode_t mode; int error;
 switch (context->dc_kind) {

  case DEE_DEXCONTEXTKIND_INITIALIZE:
   // Import the native filesystem api
   error = DeeFS_GetApi(DEE_VFS_HFSVERSION,DEE_FSAPIMODE_NONE,
                        &DeeVFS_NativeHooks,sizeof(DeeVFS_NativeHooks),NULL);
   if DEE_UNLIKELY(error != 0) {
    if (error < 0) return error;
    DeeError_SET_STRING(&DeeErrorType_RuntimeError,
                        "v" DEE_PP_STR(DEE_VFS_HFSVERSION) " FS "
                        "API has a different size that it should!");
    return -1;
   }
   break;

  case DEE_DEXCONTEXTKIND_FINALIZE:
   // Disable the VFS hooks within the core
   if (((mode = DeeFS_GetAPIMode())&DEE_FSAPIMODE_ENABLEVFS)!=0) {
    if (DeeFS_SetAPIMode((Dee_fsapimode_t)(mode&~(DEE_FSAPIMODE_ENABLEVFS))) != 0) {
     // Shouldn't happen
     while (!DeeError_Print("Failed to unlink VFS",1));
    }
   }
   DeeVFS_Shutdown();
   break;
  default: break;
 }
 return 0;
}



DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_VFS_C */
