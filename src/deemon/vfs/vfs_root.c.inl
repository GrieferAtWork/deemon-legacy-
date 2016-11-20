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
#ifndef GUARD_DEEMON_VFS_VFS_ROOT_C_INL
#define GUARD_DEEMON_VFS_VFS_ROOT_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/vfs/vfs_root.h>
#include <deemon/vfs/vfs_virtual_stdfile.h>

#if DEE_CONFIG_RUNTIME_HAVE_VFS2
DEE_DECL_BEGIN

extern struct DeeVFSVirtualDirNode _deevfs_dev;
static struct DeeVFSVirtualStdFileNode _deevfs_dev_stdin  = DeeVFSVirtualStdFileNode_INIT((struct DeeVFSNode *)&_deevfs_dev,STD_INPUT_HANDLE);
static struct DeeVFSVirtualStdFileNode _deevfs_dev_stdout = DeeVFSVirtualStdFileNode_INIT((struct DeeVFSNode *)&_deevfs_dev,STD_OUTPUT_HANDLE);
static struct DeeVFSVirtualStdFileNode _deevfs_dev_stderr = DeeVFSVirtualStdFileNode_INIT((struct DeeVFSNode *)&_deevfs_dev,STD_ERROR_HANDLE);


static struct DeeVFSVirtualDirEntry _deevfs_dev_nodes[] = {
 {"stdin", (struct DeeVFSNode *)&_deevfs_dev_stdin},
 {"stdout",(struct DeeVFSNode *)&_deevfs_dev_stdout},
 {"stderr",(struct DeeVFSNode *)&_deevfs_dev_stderr},
 {NULL,NULL},
};
struct DeeVFSVirtualDirNode _deevfs_dev =
 DeeVFSVirtualDirNode_INIT(DeeVFS_Root,_deevfs_dev_nodes);



struct DeeVFSNativeMountNode _DeeVFSNative_Root =
 {DeeVFSNode_INIT(&DeeVFSNativeMountNode_Type,DeeVFS_Root)};

static struct DeeVFSVirtualDirEntry _deevfs_root_nodes[] = {
 {"dev",(struct DeeVFSNode *)&_deevfs_dev},
 {"mount",DeeVFSNative_Root},
 {NULL,NULL},
};

struct DeeVFSVirtualDirNode _DeeVFS_Root =
 DeeVFSVirtualDirNode_INIT(NULL,_deevfs_root_nodes);

DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */

#endif /* !GUARD_DEEMON_VFS_VFS_ROOT_C_INL */
