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
#ifndef GUARD_DEEMON_VFS_VFS_ROOT_H
#define GUARD_DEEMON_VFS_VFS_ROOT_H 1

#include <deemon/__conf.inl>
#include "__vfsconf.inl"
#include "vfs_native_mount.h"
#include "vfs_virtual_dir.h"

DEE_DECL_BEGIN

// The root node of the entire virtual file system
extern struct DeeVFSVirtualDirNode _DeeVFS_Root;
#define DeeVFS_Root ((struct DeeVFSNode *)&_DeeVFS_Root)

// '/native/': Mounting point for the native filesystem
// NOTE: On windows, this is a folder filled with directories matching all drive names.
extern struct DeeVFSNode _DeeVFSNative_Root;
#define DeeVFSNative_Root ((struct DeeVFSNode *)&_DeeVFSNative_Root)
#if DEE_VFSCONFIG_HAVEFILE_MNT
#define DeeVFS_Mnt        DeeVFSNative_Root
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_VFS_VFS_ROOT_H */
