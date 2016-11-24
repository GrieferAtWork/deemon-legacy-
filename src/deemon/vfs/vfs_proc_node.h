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
#ifndef GUARD_DEEMON_VFS_VFS_PROC_NODE_H
#define GUARD_DEEMON_VFS_VFS_PROC_NODE_H 1

#include <deemon/__conf.inl>

#if DEE_CONFIG_RUNTIME_HAVE_VFS2
#include <deemon/vfs/vfs_core.h>
#include <deemon/vfs/vfs_virtual_dir.h>
#include <deemon/sys/sysfd.h>
#include <deemon/list.h>
#include <deemon/mp/process.h>
#include <deemon/optional/atomic_mutex.h>
#include <deemon/fs/native_view.wide.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()


DEE_DECL_BEGIN

struct DeeVFSProcPIDNode {
 struct DeeVFSNode           vpn_node; /*< Underlying node. */
 DEE_A_REF DeeProcessObject *vpn_proc; /*< Process associated with this node. */
};

//////////////////////////////////////////////////////////////////////////
// Native VFS node: A filesystem node that points back into the 
struct DeeVFSProcView {
 struct DeeVFSView                   vpr_view;    /*< Underlying view. */
 DEE_A_REF DeeListObject            *vpr_procs;   /*< [1..1][const] List of processes. */
           DeeProcessObject        **vpr_procpos; /*< [1..1][0..1] Current position in the process list. */
           DeeProcessObject        **vpr_procend; /*< [?..?][0..1] End of the process list. */
 // NOTE: The following data is used when the list of process ids ends
 struct DeeVFSVirtualDirEntry const *vpr_pos;  /*< [1..1] Next virtual directory entry. */
 struct DeeAtomicMutex               vpr_lock; /*< Yield-lock. */
};

extern struct DeeVFSVirtualDirEntry const DeeVFSProc_VirtualEntries[];    /*< List of non-process id entries in '/proc' */
extern struct DeeVFSVirtualDirEntry const DeeVFSProcPID_VirtualEntries[]; /*< List of non-process id entries in '/proc/[PID]' */

extern struct DeeVFSNode _DeeVFS_Proc;
extern struct DeeVFSNode _DeeVFS_ProcSelf;
#define DeeVFS_Proc     ((struct DeeVFSNode *)&_DeeVFS_Proc)
#define DeeVFS_ProcSelf ((struct DeeVFSNode *)&_DeeVFS_ProcSelf)

extern struct DeeVFSNodeType const DeeVFSProcNode_Type;     /*< '/proc'       (Virtual directory; views of this yield ids of all running processes, as well as select virtual elements) */
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type;  /*< '/proc/[PID]' (Virtual directory; provides utilities for querying information about a process) */
extern struct DeeVFSNodeType const DeeVFSProcSelfNode_Type; /*< '/proc/self'  (Symblic link; when read returns the current process' id) */

extern DEE_A_RET_EXCEPT_REF struct DeeVFSProcPIDNode *DeeVFSProcPIDNode_NewFromProcess(DEE_A_IN DeeProcessObject *proc);
extern DEE_A_RET_EXCEPT_REF struct DeeVFSProcPIDNode *DeeVFSProcPIDNode_NewFromPID(DEE_A_IN DWORD pid);

DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */

#endif /* !GUARD_DEEMON_VFS_VFS_PROC_NODE_H */
