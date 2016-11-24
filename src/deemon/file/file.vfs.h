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
#ifndef GUARD_DEEMON_FILE_VFS_H
#define GUARD_DEEMON_FILE_VFS_H 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include "file.fd.h"
#if DEE_CONFIG_RUNTIME_HAVE_VFS2
#include <deemon/file.h>
#include <deemon/vfs/vfs_core.h>

DEE_DECL_BEGIN

struct DeeFileVFSObject {
 DEE_FILE_OBJECT_HEAD
 Dee_uint32_t      vf_users; /*< [atomic,inc:lock(fo_lock)] Amount of threads currently using this file-descriptor (incrementing requires a lock to 'fo_lock'). */
 struct DeeVFSFile vf_file;  /*< Virtual file data. */
};

extern DeeFileTypeObject DeeFileVFS_Type;

extern DEE_A_RET_EXCEPT_REF DeeFileVFSObject *DeeFileVFS_OpenNode(
 DEE_A_INOUT struct DeeVFSNode *node, DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeFileVFS_Filename(DEE_A_IN DeeFileVFSObject const *self);


#define DeeFileVFS_ACQUIRE_SHARED(ob,...)\
do{\
 DeeFile_ACQUIRE(ob);\
 if ((((DeeFileVFSObject *)(ob))->fo_flags&DEE_PRIVATE_FILEFLAG_VFS_VALID)==0)\
 { DeeFile_RELEASE(ob); {__VA_ARGS__;} } else {\
  DeeAtomic32_FetchInc(((DeeFileVFSObject *)(ob))->vf_users,memory_order_seq_cst);\
  DeeFile_RELEASE(ob);\
 }\
}while(0)
#define DeeFileVFS_RELEASE_SHARED(ob)\
 DeeAtomic32_FetchDec(((DeeFileVFSObject *)(ob))->vf_users,memory_order_seq_cst)
#define DeeFileVFS_ACQUIRE_EXCLUSIVE(ob,...)\
do{\
 DeeFile_ACQUIRE(ob);\
 while (1) {\
  if (((ob)->fo_flags&DEE_PRIVATE_FILEFLAG_VFS_VALID)==0)\
  { DeeFile_RELEASE(ob); {__VA_ARGS__;} break; }\
  /* NOTE: No need for atomically loading 'vf_users' \
           here (compare against 0 - quirk) */\
  if ((ob)->vf_users == 0) break; /*< We did it! */\
  DeeFile_RELEASE(ob);\
  /* Sleep a bit while waiting for other threads \
     to finish (and hope this isn't a deadlock...). */\
  DeeThread_SleepNoInterrupt(1);\
  DeeFile_ACQUIRE(ob);\
 }\
}while(0)
#define DeeFileVFS_RELEASE_EXCLUSIVE(ob) DeeFile_RELEASE(ob)


DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */

#endif /* !GUARD_DEEMON_FILE_VFS_H */
