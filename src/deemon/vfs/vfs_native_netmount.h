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
#ifndef GUARD_DEEMON_VFS_VFS_NATIVE_NETMOUNT_H
#define GUARD_DEEMON_VFS_VFS_NATIVE_NETMOUNT_H 1

#include <deemon/__conf.inl>
#if DEE_CONFIG_RUNTIME_HAVE_VFS

#include <deemon/optional/atomic_mutex.h>
#include <deemon/string.h>
#include <deemon/vfs/vfs_core.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
#include <lm.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
struct DeeVFSNetServerNode {
 struct DeeVFSNode          vnm_node; /*< Underlying node. */
 DEE_A_REF DeeStringObject *vnm_name; /*< Name of this server. */
};
struct DeeVFSNetServerView {
 struct DeeVFSView nsv_view;   /*< Underlying view. */
 struct DeeAtomicMutex nsv_lock; /*< Lock for everything below */
 DEE_A_REF DeeWideStringObject *nsv_server; /*< [1..1] Name of server. */
 PSHARE_INFO_502   nsv_pBegin; /*< [0..1] Begin of server list. */
 PSHARE_INFO_502   nsv_pEnd;   /*< [0..1] End of server list. */
 PSHARE_INFO_502   nsv_pCurr;  /*< [0..1] Record for the current server. */
 DWORD             nsv_dwResumeHandle;
 int               nsv_hasresume;
};
struct DeeVFSNetMountView {
 struct DeeVFSView nmv_view;   /*< Underlying view. */
 struct DeeAtomicMutex nmv_lock; /*< Lock for everything below */
 LPSERVER_INFO_101 nmv_pBegin; /*< [0..1] Begin of server list. */
 LPSERVER_INFO_101 nmv_pEnd;   /*< [0..1] End of server list. */
 LPSERVER_INFO_101 nmv_pCurr;  /*< [0..1] Record for the current server. */
 DWORD             nmv_dwResumeHandle;
 int               nmv_hasresume;
};

extern struct DeeVFSNodeType const DeeVFSNetServerNode_Type; /*< "/net/[SERVER]" */
extern struct DeeVFSNodeType const DeeVFSNetMountNode_Type;  /*< "/net" */

DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#endif /* !GUARD_DEEMON_VFS_VFS_NATIVE_NETMOUNT_H */
