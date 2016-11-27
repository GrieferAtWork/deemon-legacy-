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
#include <deemon/vfs/__vfsconf.inl>
#if DEE_VFSCONFIG_HAVEFILE_PROC
#include <deemon/vfs/vfs_core.h>
#include <deemon/vfs/vfs_virtual_dir.h>
#include <deemon/sys/sysfd.h>
#include <deemon/list.h>
#include <deemon/mp/process.h>
#include <deemon/optional/atomic_mutex.h>

#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD
#include <deemon/sys/win32/w32_process_handles.h>
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD */

#if DEE_VFSCONFIG_HAVEFILE_PROC_PID
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID */


DEE_DECL_BEGIN

extern struct DeeVFSNode _DeeVFS_Proc;
extern struct DeeVFSNode _DeeVFS_ProcSelf;
#define DeeVFS_Proc     ((struct DeeVFSNode *)&_DeeVFS_Proc)
#define DeeVFS_ProcSelf ((struct DeeVFSNode *)&_DeeVFS_ProcSelf)




//////////////////////////////////////////////////////////////////////////
// "/proc" (Virtual directory; views of this yield ids of all running processes, as well as select virtual elements) 
struct DeeVFSProcView {
 struct DeeVFSView                   vpr_view;    /*< Underlying view. */
#if DEE_VFSCONFIG_HAVE_PROC_ENUMPIDS
 DEE_A_REF DeeListObject            *vpr_procs;   /*< [1..1][const] List of processes. */
           DeeProcessObject        **vpr_procpos; /*< [1..1][0..1] Current position in the process list. */
           DeeProcessObject        **vpr_procend; /*< [?..?][0..1] End of the process list. */
#endif /* DEE_VFSCONFIG_HAVE_PROC_ENUMPIDS */
 // NOTE: The following data is used when the list of process ids ends
 struct DeeVFSVirtualDirEntry const *vpr_pos;     /*< [1..1] Next virtual directory entry. */
 struct DeeAtomicMutex               vpr_lock;    /*< Yield-lock. */
};
extern struct DeeVFSVirtualDirEntry const DeeVFSProc_VirtualEntries[];    /*< List of non-process id entries in '/proc' */
extern struct DeeVFSNodeType const DeeVFSProcNode_Type;     /*< '/proc' */
//////////////////////////////////////////////////////////////////////////



#if DEE_VFSCONFIG_HAVEFILE_PROC_PID
//////////////////////////////////////////////////////////////////////////
// "/proc/[PID]" (Virtual directory; provides utilities for querying information about a process)
struct DeeVFSProcPIDNode {
 struct DeeVFSNode           vpn_node; /*< Underlying node. */
 DEE_A_REF DeeProcessObject *vpn_proc; /*< Process associated with this node. */
};
struct DeeVFSProcPIDView {
 struct DeeVFSView           vpv_view;   /*< Underlying view. */
 DEE_A_REF DeeProcessObject *vpv_proc;   /*< Process associated with this view. */
 /*atomic*/unsigned int      vpv_member; /*< Member ID. */
};
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type;  /*< '/proc/[PID]' */
extern DEE_A_RET_EXCEPT_REF struct DeeVFSProcPIDNode *DeeVFSProcPIDNode_NewFromProcess(DEE_A_IN DeeProcessObject *proc);
extern DEE_A_RET_EXCEPT_REF struct DeeVFSProcPIDNode *DeeVFSProcPIDNode_NewFromPID(DEE_A_IN DWORD pid);
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_CMDLINE
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_cmdline; /*< '/proc/[PID]/cmdline' */
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_CMDLINE */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_CWD
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_cwd;     /*< '/proc/[PID]/cwd' */
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_CWD */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_EXE
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_exe;     /*< '/proc/[PID]/exe' */
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_EXE */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_MEM
struct DeeVFSProcPIDMemFile {
 struct DeeVFSFile           vpm_file; /*< Underlying file. */
 HANDLE                      vpm_proc; /*< [1..1] Associated process handle (with 'PROCESS_VM_WRITE'/'PROCESS_VM_READ' rights). */
 Dee_uint64_t                vpm_addr; /*< Address offset (64-bit to accommodate for x64 from x86; aka. WOW64). */
};
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_mem;     /*< '/proc/[PID]/mem' */
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_MEM */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_ROOT
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_root;    /*< '/proc/[PID]/root' */
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_ROOT */
//////////////////////////////////////////////////////////////////////////
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD
#if DEE_VFSCONFIG_HAVE_PROC_PID_FD_ENUMFDS
struct DeeVFSProcPIDFDView {
 struct DeeVFSView           vpf_view; /*< Underlying view. */
 DEE_A_REF DeeProcessObject *vpf_proc; /*< Process associated with this node. */
 SYSTEM_HANDLE_INFORMATION  *vpf_info; /*< [1..1] Handle information. */
 /*atomic*/SYSTEM_HANDLE    *vpf_curr; /*< [1..1] Current entry. */
 SYSTEM_HANDLE              *vpf_end;  /*< [1..1] End entry. */
 DWORD                       vpf_pid;  /*< Process id to search for. */
};
#endif /* DEE_VFSCONFIG_HAVE_PROC_PID_FD_ENUMFDS */
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_fd;      /*< '/proc/[PID]/fd' */
struct DeeVFSProcPIDFDHIDNode {
 struct DeeVFSNode           vpfh_node; /*< Underlying node. */
 SYSTEM_HANDLE               vpfh_info; /*< Handle information (only has 'ProcessId', 'ObjectTypeNumber' and 'Handle'). */
};
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_fd_link; /*< '/proc/[PID]/fd/[HID]' */
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD */
//////////////////////////////////////////////////////////////////////////
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32
struct DeeVFSProcPIDW32View {
 struct DeeVFSView      vpwv_view; /*< Underlying view. */
 /*atomic*/unsigned int vpwv_curr; /*< Current element index. */
};
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_w32;           /*< '/proc/[PID]/w32' */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_CLOSEFD
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_w32_closefd;   /*< '/proc/[PID]/w32/closefd' */
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_CLOSEFD */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_EXITCODE
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_w32_exitcode;  /*< '/proc/[PID]/w32/exitcode' */
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_EXITCODE */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYBOOST
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_w32_priorityboost; /*< '/proc/[PID]/w32/priority_boost' */
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYBOOST */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYCLASS
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_w32_priorityclass; /*< '/proc/[PID]/w32/priority' */
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYCLASS */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_TERMINATE
extern struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_w32_terminate; /*< '/proc/[PID]/w32/terminate' */
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_TERMINATE */
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32 */
//////////////////////////////////////////////////////////////////////////
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID */


#if DEE_VFSCONFIG_HAVEFILE_PROC_SELF
//////////////////////////////////////////////////////////////////////////
// "/proc/self" (Symblic link; when read, returns the current process' id)
extern struct DeeVFSNodeType const DeeVFSProcSelfNode_Type; /*< '/proc/self' */
//////////////////////////////////////////////////////////////////////////
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_SELF */


DEE_DECL_END
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC */
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#endif /* !GUARD_DEEMON_VFS_VFS_PROC_NODE_H */
