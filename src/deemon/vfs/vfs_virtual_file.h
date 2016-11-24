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
#ifndef GUARD_DEEMON_VFS_VFS_VIRTUAL_FILE_H
#define GUARD_DEEMON_VFS_VFS_VIRTUAL_FILE_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/atomic_mutex.h>
#include <deemon/vfs/vfs_core.h>

#if DEE_CONFIG_RUNTIME_HAVE_VFS2
DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// Virtual file: A file that can be read to return a given string
struct DeeVFSVirtualFile {
 struct DeeVFSFile          vff_file; /*< Underlying node. */
 DEE_A_REF DeeStringObject *vff_text; /*< [1..1] Text returned by this file. */
 char const                *vff_pos;  /*< [1..1] Current text position. */
 char const                *vff_end;  /*< [?..?] End of the file. */
 struct DeeAtomicMutex      vff_lock; /*< File Lock. */
};
#define DeeVFSVirtualFile_InitWithInheritedText(self,text)\
do{\
 (self)->vff_text = (DeeStringObject *)(text);\
 (self)->vff_end = ((self)->vff_pos = DeeString_STR(text))+DeeString_SIZE(text);\
 DeeAtomicMutex_Init(&(self)->vff_lock);\
}while(0)
#define DeeVFSVirtualFile_InitWithText(self,text)\
do{\
 Dee_INCREF((self)->vff_text = (DeeStringObject *)(text));\
 (self)->vff_end = ((self)->vff_pos = DeeString_STR(text))+DeeString_SIZE(text);\
 DeeAtomicMutex_Init(&(self)->vff_lock);\
}while(0)
extern void DEE_CALL DeeVFSVirtualFile_Quit(struct DeeVFSFile *self);
extern int DEE_CALL DeeVFSVirtualFile_Read(struct DeeVFSFile *self, void *p, Dee_size_t s, Dee_size_t *rs);
extern int DEE_CALL DeeVFSVirtualFile_Seek(struct DeeVFSFile *self, Dee_int64_t off, int whence, Dee_uint64_t *pos);
/*
 sizeof(struct DeeVFSVirtualFile),
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_openmode_t,Dee_mode_t))           &...,
 (void(DEE_CALL *)(struct DeeVFSFile *))                                     &DeeVFSVirtualFile_Quit,
 (int (DEE_CALL *)(struct DeeVFSFile *,void *,Dee_size_t,Dee_size_t *))      &DeeVFSVirtualFile_Read,
 (int (DEE_CALL *)(struct DeeVFSFile *,void const *,Dee_size_t,Dee_size_t *))NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_int64_t,int,Dee_uint64_t *))      &DeeVFSVirtualFile_Seek,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
*/


DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */

#endif /* !GUARD_DEEMON_VFS_VFS_VIRTUAL_FILE_H */
