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
#ifndef GUARD_DEEMON_VFS_VFS_VIRTUAL_FILE_C
#define GUARD_DEEMON_VFS_VFS_VIRTUAL_FILE_C 1
#ifndef DEE_EXTENSION
#define DEE_EXTENSION 1
#endif

#include <deemon/__conf.inl>
#include <deemon/file.h>
#include <deemon/string.h>
#include "vfs_core.h"
#include "vfs_virtual_file.h"

DEE_DECL_BEGIN

#define _self ((struct DeeVFSVirtualFile *)self)
void DEE_CALL DeeVFSVirtualFile_Quit(struct DeeVFSFile *self) { Dee_DECREF(_self->vff_text); }
int DEE_CALL DeeVFSVirtualFile_Read(struct DeeVFSFile *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 Dee_size_t max_read;
 DeeAtomicMutex_Acquire(&_self->vff_lock);
 if (_self->vff_pos >= _self->vff_end) {
  DeeAtomicMutex_Release(&_self->vff_lock);
  *rs = 0;
  return 0;
 }
 max_read = (Dee_size_t)(_self->vff_end-_self->vff_pos);
 if (s < max_read) max_read = s;
 memcpy(p,_self->vff_pos,max_read);
 *(Dee_uintptr_t *)&_self->vff_pos += max_read;
 DeeAtomicMutex_Release(&_self->vff_lock);
 *rs = max_read;
 return 0;
}
int DEE_CALL DeeVFSVirtualFile_Seek(
 struct DeeVFSFile *self, Dee_int64_t off, int whence, Dee_uint64_t *pos) {
 char const *new_pos;
 DeeAtomicMutex_Acquire(&_self->vff_lock);
 switch (whence) {
  case DEE_SEEK_SET: new_pos = DeeString_STR(_self->vff_text)+(Dee_size_t)*(Dee_uint64_t *)&off; break;
  case DEE_SEEK_CUR: new_pos = _self->vff_pos+(Dee_ssize_t)off; break;
  case DEE_SEEK_END: new_pos = _self->vff_end-(Dee_size_t)*(Dee_uint64_t *)&off; break;
  default: new_pos = _self->vff_pos; break;
 }
 if (new_pos < DeeString_STR(_self->vff_text)) new_pos = DeeString_STR(_self->vff_text);
 if (pos) *pos = (Dee_uint64_t)(new_pos-DeeString_STR(_self->vff_text));
 _self->vff_pos = new_pos;
 DeeAtomicMutex_Release(&_self->vff_lock);
 return 0;
}
#undef _self

DEE_DECL_END

#endif /* !GUARD_DEEMON_VFS_VFS_VIRTUAL_FILE_C */
