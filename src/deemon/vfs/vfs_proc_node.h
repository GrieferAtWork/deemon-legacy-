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

#if DEE_CONFIG_RUNTIME_HAVE_VFS
#include <deemon/vfs/vfs_core.h>
#include <deemon/vfs/vfs_virtual_dir.h>
#include <deemon/sys/sysfd.h>
#include <deemon/list.h>
#include <deemon/mp/process.h>
#include <deemon/optional/atomic_mutex.h>
#include <deemon/fs/native_view.wide.h>
#include <deemon/sys/win32/w32_process_handles.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()


DEE_DECL_BEGIN

extern struct DeeVFSNode _DeeVFS_Proc;
extern struct DeeVFSNode _DeeVFS_ProcSelf;
#define DeeVFS_Proc     ((struct DeeVFSNode *)&_DeeVFS_Proc)
#define DeeVFS_ProcSelf ((struct DeeVFSNode *)&_DeeVFS_ProcSelf)




//////////////////////////////////////////////////////////////////////////
// "/proc" (Virtual directory; views of this yield ids of all running processes, as well as select virtual elements) 
struct DeeVFSProcView {
 struct DeeVFSView                   vpr_view;    /*< Underlying view. */
 DEE_A_REF DeeListObject            *vpr_procs;   /*< [1..1][const] List of processes. */
           DeeProcessObject        **vpr_procpos; /*< [1..1][0..1] Current position in the process list. */
           DeeProcessObject        **vpr_procend; /*< [?..?][0..1] End of the process list. */
 // NOTE: The following data is used when the list of process ids ends
 struct DeeVFSVirtualDirEntry const *vpr_pos;     /*< [1..1] Next virtual directory entry. */
 struct DeeAtomicMutex               vpr_lock;    /*< Yield-lock. */
};
extern struct DeeVFSVirtualDirEntry const DeeVFSProc_VirtualEntries[];    /*< List of non-process id entries in '/proc' */
extern struct DeeVFSNodeType const DeeVFSProcNode_Type;     /*< '/proc' */
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// "/proc/[PID]" (Virtual directory; provides utilities for querying information about a process)
struct DeeVFSProcPIDNode {
 struct DeeVFSNode           vpn_node; /*< Underlying node. */
 DEE_A_REF DeeProcessObject *vpn_proc; /*< Process associated with this node. */
};
struct DeeVFSProcPIDView {
 struct DeeVFSView           vpv_view;   /*< Underlying view. */
 DEE_A_REF DeeProcessObject *vpv_proc;   /*< Process associated with this view. */
 /*atomic*/int               vpv_member; /*< Member ID. */
};
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type;  /*< '/proc/[PID]' */
extern DEE_A_RET_EXCEPT_REF struct DeeVFSProcPIDNode *DeeVFSProcPIDNode_NewFromProcess(DEE_A_IN DeeProcessObject *proc);
extern DEE_A_RET_EXCEPT_REF struct DeeVFSProcPIDNode *DeeVFSProcPIDNode_NewFromPID(DEE_A_IN DWORD pid);
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_exe;     /*< '/proc/[PID]/exe' */
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_cwd;     /*< '/proc/[PID]/cwd' */
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_cmdline; /*< '/proc/[PID]/cmdline' */
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_root;    /*< '/proc/[PID]/root' */
//////////////////////////////////////////////////////////////////////////
struct DeeVFSProcPIDFDView {
 struct DeeVFSView           vpf_view; /*< Underlying view. */
 DEE_A_REF DeeProcessObject *vpf_proc; /*< Process associated with this node. */
 SYSTEM_HANDLE_INFORMATION  *vpf_info; /*< [1..1] Handle information. */
 /*atomic*/SYSTEM_HANDLE    *vpf_curr; /*< [1..1] Current entry. */
 SYSTEM_HANDLE              *vpf_end;  /*< [1..1] End entry. */
 DWORD                       vpf_pid;  /*< Process id to search for. */
};
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_fd;      /*< '/proc/[PID]/fd' */
struct DeeVFSProcPIDFDHIDNode {
 struct DeeVFSNode           vpfh_node; /*< Underlying node. */
 SYSTEM_HANDLE               vpfh_info; /*< Handle information (only has 'ProcessId', 'ObjectTypeNumber' and 'Handle'). */
};
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_fd_link; /*< '/proc/[PID]/fd/[HID]' */
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
// "/proc/self" (Symblic link; when read, returns the current process' id)
extern struct DeeVFSNodeType const DeeVFSProcSelfNode_Type; /*< '/proc/self' */
//////////////////////////////////////////////////////////////////////////


DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#endif /* !GUARD_DEEMON_VFS_VFS_PROC_NODE_H */
