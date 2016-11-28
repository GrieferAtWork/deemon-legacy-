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
#ifndef GUARD_DEEMON_VFS_VFS_VIRTUAL_STDFILE_H
#define GUARD_DEEMON_VFS_VFS_VIRTUAL_STDFILE_H 1

#include <deemon/__conf.inl>
#include "__vfsconf.inl"
#if DEE_VFSCONFIG_HAVEFILE_DEV_STDIN\
 || DEE_VFSCONFIG_HAVEFILE_DEV_STDOUT\
 || DEE_VFSCONFIG_HAVEFILE_DEV_STDERR
#include <deemon/optional/atomic_mutex.h>
#include "vfs_core.h"
#include <deemon/fs/native_filefd.h>

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// Native VFS node: A filesystem node that points back into the 
struct DeeVFSVirtualStdFileNode {
 struct DeeVFSNode std_node;     /*< Underlying node. */
 DWORD             std_handleid; /*< [1..1] Vector of children nodes. */
};
#define DeeVFSVirtualStdFileNode_INIT(parent,std_handle_id) \
 {DeeVFSNode_INIT(&DeeVFSVirtualStdFileNode_Type,parent),std_handle_id}

extern struct DeeVFSNodeType const DeeVFSVirtualStdFileNode_Type;

DEE_DECL_END
#endif /* ... */

#endif /* !GUARD_DEEMON_VFS_VFS_VIRTUAL_STDFILE_H */
