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
#ifndef GUARD_DEEMON_VFS_VFS_VIRTUAL_NULLFILE_H
#define GUARD_DEEMON_VFS_VFS_VIRTUAL_NULLFILE_H 1

#include <deemon/__conf.inl>
#include "vfs_core.h"
#include "__vfsconf.inl"

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// NULLFile VFS node: A file node that drops all io-requests
// Difference between these is described here:
// >> http://unix.stackexchange.com/questions/254384/difference-between-dev-null-and-dev-zero
// >> https://en.wikipedia.org/wiki//dev/full
#if DEE_VFSCONFIG_HAVEFILE_DEV_NULL
extern struct DeeVFSNodeType const DeeVFSVirtualNULLFileNode_Type;
#endif /* DEE_VFSCONFIG_HAVEFILE_DEV_NULL */
#if DEE_VFSCONFIG_HAVEFILE_DEV_ZERO
extern struct DeeVFSNodeType const DeeVFSVirtualZEROFileNode_Type;
#endif /* DEE_VFSCONFIG_HAVEFILE_DEV_ZERO */
#if DEE_VFSCONFIG_HAVEFILE_DEV_FULL
extern struct DeeVFSNodeType const DeeVFSVirtualFULLFileNode_Type;
#endif /* DEE_VFSCONFIG_HAVEFILE_DEV_FULL */

DEE_DECL_END

#endif /* !GUARD_DEEMON_VFS_VFS_VIRTUAL_NULLFILE_H */
