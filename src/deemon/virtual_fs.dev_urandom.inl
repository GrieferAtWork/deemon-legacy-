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
#ifndef GUARD_DEEMON_VIRTUAL_URANDOM_INL
#define GUARD_DEEMON_VIRTUAL_URANDOM_INL 1
#ifdef __INTELLISENSE__
#define DEE_LIMITED_API 1
#endif

#include "virtual_fs.h.inl"
#ifdef __INTELLISENSE__
#include "fs_api.c"
#endif

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
//
#if DEE_TYPES_SIZEOF_POINTER < 8
struct _vfs_fileio_urandom_data {
 struct DeeRandomNumberGenerator urd_rnd;
};
#define DEE_VFS_URANDOM_RNG(fileio) \
 (&((struct _vfs_fileio_urandom_data *)(fileio)->fio_vfs.fio_data)->urd_rnd)
#else
#define DEE_VFS_URANDOM_RNG(fileio) \
 ((struct DeeRandomNumberGenerator *)&(fileio)->fio_vfs.fio_data)
#endif
static int _vfs_fileio_urandom_read(
 DeeFileIOObject *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 DeeRandomNumberGenerator_NextData(DEE_VFS_URANDOM_RNG(self),p,s);
 *rs = s;
 return 0;
}
#if DEE_TYPES_SIZEOF_POINTER < 8
static void _vfs_fileio_urandom_close(DeeFileIOObject *self) {
 free_nn(self->fio_vfs.fio_data);
}
#endif
struct DeeFileIOVFSType _vfs_fileio_urandom_type = {
 &_vfs_fileio_urandom_read,
 NULL,
 NULL,
 NULL,
 NULL,
#if DEE_TYPES_SIZEOF_POINTER < 8
 &_vfs_fileio_urandom_close,
#else
 NULL,
#endif
};
#undef DEE_VFS_URANDOM_RNG


DEE_DECL_END

#endif /* !GUARD_DEEMON_VIRTUAL_URANDOM_INL */
