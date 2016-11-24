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
#ifndef GUARD_DEEMON_VIRTUAL_ZERO_INL
#define GUARD_DEEMON_VIRTUAL_ZERO_INL 1
#ifdef __INTELLISENSE__
#define DEE_LIMITED_API 1
#endif

#include "virtual_fs.h.inl"
#include <deemon/optional/std/string.h>
#ifdef __INTELLISENSE__
#include "fs_api.c"
#endif

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
//
static int _vfs_fileio_zero_read(
 DeeFileIOObject *DEE_UNUSED(self), void *p, Dee_size_t s, Dee_size_t *rs) {
 memset(p,0,s);
 *rs = s;
 return 0;
}
static int _vfs_fileio_zero_write(
 DeeFileIOObject *DEE_UNUSED(self),
 void const *DEE_UNUSED(p), Dee_size_t s, Dee_size_t *ws) {
 *ws = s;
 return 0;
}
struct DeeFileIOVFSType _vfs_fileio_zero_type = {
 &_vfs_fileio_zero_read,
 &_vfs_fileio_zero_write,
 NULL,
 NULL,
 NULL,
 NULL,
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_VIRTUAL_ZERO_INL */
