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
#ifndef GUARD_DEEMON_VFS_VFS_PROC_NODE_C_INL
#define GUARD_DEEMON_VFS_VFS_PROC_NODE_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>

#if DEE_CONFIG_RUNTIME_HAVE_VFS
#include <deemon/vfs/vfs_core.h>
#include <deemon/vfs/vfs_proc_node.h>
#include <deemon/vfs/vfs_root.h>
#include <deemon/vfs/vfs_virtual_file.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN



//////////////////////////////////////////////////////////////////////////
// +++ DeeVFSProcNode_Type +++
struct DeeVFSVirtualDirEntry const DeeVFSProc_VirtualEntries[] = {
 {"self",DeeVFS_ProcSelf},
 {NULL,NULL},
};
struct DeeVFSNode _DeeVFS_Proc = DeeVFSNode_INIT(&DeeVFSProcNode_Type,DeeVFS_Root);

static struct DeeVFSNode *DEE_CALL _deevfs_procnode_vnt_walk(
 struct DeeVFSNode *DEE_UNUSED(self), Dee_Utf8Char const *name, Dee_size_t name_size) {
 DWORD pid = 0; Dee_Utf8Char const *iter,*end;
 struct DeeVFSVirtualDirEntry const *eiter;
 end = (iter = name)+name_size;
 while (iter != end) {
  if (*iter < '0' || *iter > '9') goto nopid;
  pid = (pid*10)+(*iter-'0');
  ++iter;
 }
 return (struct DeeVFSNode *)DeeVFSProcPIDNode_NewFromPID(pid);
nopid:
 for (eiter = DeeVFSProc_VirtualEntries; eiter->name; ++eiter) {
  if (Dee_Utf8StrNCmp(eiter->name,name,name_size) == 0) { DeeVFSNode_INCREF(eiter->node); return eiter->node; }
 }
 DeeError_SetStringf(&DeeErrorType_SystemError,
                     "Failed to find %$q in \"/proc\"",
                     name_size,name);
 return NULL;
}
static struct DeeVFSNode *DEE_CALL _deevfs_procnode_vnt_wwalk(
 struct DeeVFSNode *DEE_UNUSED(self), Dee_WideChar const *name, Dee_size_t name_size) {
 DWORD pid = 0; Dee_WideChar const *iter,*end;
 struct DeeVFSVirtualDirEntry const *eiter;
 end = (iter = name)+name_size;
 while (iter != end) {
  if (*iter < '0' || *iter > '9') goto nopid;
  pid = (pid*10)+(*iter-'0');
  ++iter;
 }
 return (struct DeeVFSNode *)DeeVFSProcPIDNode_NewFromPID(pid);
nopid:
 for (eiter = DeeVFSProc_VirtualEntries; eiter->name; ++eiter) {
  if (Dee_WideStrNCmp(eiter->name,name,name_size) == 0) { DeeVFSNode_INCREF(eiter->node); return eiter->node; }
 }
 DeeError_SetStringf(&DeeErrorType_SystemError,
                     "Failed to find %l$q in \"/proc\"",
                     name_size,name);
 return NULL;
}
static DeeObject *DEE_CALL _deevfs_procnode_vnt_nameof(
 struct DeeVFSNode *DEE_UNUSED(self), struct DeeVFSNode *child) {
 struct DeeVFSVirtualDirEntry const *eiter;
 if (child->vn_type == &DeeVFSProcPIDNode_Type) {
  DWORD pid = DeeProcess_ID((DeeObject *)((struct DeeVFSProcPIDNode *)child)->vpn_proc);
  return DeeString_FromUInt32(pid);
 }
 for (eiter = DeeVFSProc_VirtualEntries;;++eiter) {
  DEE_ASSERTF(eiter->name,"Given %R node isn't a child of \"/proc\"",
              DeeVFSNode_Filename(child));
  if (eiter->node == child) return DeeString_New(eiter->name);
 }
}

static int DEE_CALL _deevfs_procview_vvt_open(struct DeeVFSProcView *self) {
 if DEE_UNLIKELY((self->vpr_procs = (DeeListObject *)
  DeeProcess_Enumerate()) == NULL) return -1;
 DEE_ASSERT(DeeObject_Check(self->vpr_procs) &&
            DeeList_Check(self->vpr_procs));
 self->vpr_procend = (self->vpr_procpos = (DeeProcessObject **)
                      DeeList_ELEM(self->vpr_procs))+DeeList_SIZE(self->vpr_procs);
 self->vpr_pos = DeeVFSProc_VirtualEntries;
 DeeAtomicMutex_Init(&self->vpr_lock);
 return 0;
}
static void DEE_CALL _deevfs_procview_vvt_quit(struct DeeVFSProcView *self) {
 Dee_DECREF(self->vpr_procs);
}
static int DEE_CALL _deevfs_procview_vvt_curr(
 struct DeeVFSProcView *self, struct DeeVFSNode **result) {
 DeeAtomicMutex_Acquire(&self->vpr_lock);
 if (self->vpr_procpos != self->vpr_procend) {
  DeeProcessObject *process;
  Dee_INCREF(process = *self->vpr_procpos);
  DeeAtomicMutex_Release(&self->vpr_lock);
  *result = (struct DeeVFSNode *)DeeVFSProcPIDNode_NewFromProcess(process);
  Dee_DECREF(process);
  return *result ? 0 : -1;
 }
 if (!self->vpr_pos->name) {
  DeeAtomicMutex_Release(&self->vpr_lock);
  return 1;
 }
 *result = self->vpr_pos->node;
 DeeVFSNode_INCREF(*result);
 DeeAtomicMutex_Release(&self->vpr_lock);
 return 0;
}
static int DEE_CALL _deevfs_procview_vvt_yield(
 struct DeeVFSProcView *self, struct DeeVFSNode **result) {
 DeeAtomicMutex_Acquire(&self->vpr_lock);
 if (self->vpr_procpos != self->vpr_procend) {
  DeeProcessObject *process;
  Dee_INCREF(process = *self->vpr_procpos);
  ++self->vpr_procpos;
  DeeAtomicMutex_Release(&self->vpr_lock);
  *result = (struct DeeVFSNode *)DeeVFSProcPIDNode_NewFromProcess(process);
  Dee_DECREF(process);
  return *result ? 0 : -1;
 }
 if (!self->vpr_pos->name) {
  DeeAtomicMutex_Release(&self->vpr_lock);
  return 1;
 }
 *result = self->vpr_pos->node;
 ++self->vpr_pos;
 DeeVFSNode_INCREF(*result);
 DeeAtomicMutex_Release(&self->vpr_lock);
 return 0;
}


static struct _DeeVFSViewTypeData _deevfs_procnode_vnt_view = {
 sizeof(struct DeeVFSProcView),
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,Dee_Utf8Char const *,Dee_size_t))&_deevfs_procnode_vnt_walk,
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,Dee_WideChar const *,Dee_size_t))&_deevfs_procnode_vnt_wwalk,
 (DeeObject *(DEE_CALL *)(struct DeeVFSNode *,struct DeeVFSNode *))                    &_deevfs_procnode_vnt_nameof,
 (int (DEE_CALL *)(struct DeeVFSView *))                                               &_deevfs_procview_vvt_open,
 (void (DEE_CALL *)(struct DeeVFSView *))                                              &_deevfs_procview_vvt_quit,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_procview_vvt_curr,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_procview_vvt_yield,
};
struct DeeVFSNodeType const DeeVFSProcNode_Type = {
 {NULL,NULL},NULL,DeeVFSNoopNodeType_FileData,
 &_deevfs_procnode_vnt_view};
// --- DeeVFSProcNode_Type ---
//////////////////////////////////////////////////////////////////////////








//////////////////////////////////////////////////////////////////////////
// +++ DeeVFSProcPIDNode_Type +++
DEE_A_RET_EXCEPT_REF struct DeeVFSProcPIDNode *
DeeVFSProcPIDNode_NewFromProcess(DEE_A_IN DeeProcessObject *proc) {
 struct DeeVFSProcPIDNode *result;
 DEE_ASSERT(DeeObject_Check(proc) && DeeProcess_Check(proc));
 if DEE_UNLIKELY((result = DeeVFSNode_ALLOC(struct DeeVFSProcPIDNode)) == NULL) return NULL;
 DeeVFSNode_InitWithParent(&result->vpn_node,&DeeVFSProcPIDNode_Type,DeeVFS_Proc);
 Dee_INCREF(result->vpn_proc = proc);
 return result;
}
DEE_A_RET_EXCEPT_REF struct DeeVFSProcPIDNode *
DeeVFSProcPIDNode_NewFromPID(DEE_A_IN DWORD pid) {
 DeeProcessObject *proc; struct DeeVFSProcPIDNode *result;
 if DEE_UNLIKELY((proc = (DeeProcessObject *)DeeProcess_Win32NewFromID(pid)) == NULL) return NULL;
 result = DeeVFSProcPIDNode_NewFromProcess(proc);
 Dee_DECREF(proc);
 return result;
}
static void DEE_CALL _deevfs_procpidnode_quit(struct DeeVFSProcPIDNode *self) {
 Dee_DECREF(self->vpn_proc);
}
#define DEE_VFSPROCPIDMEMBER_NONE    0
#define DEE_VFSPROCPIDMEMBER_CMDLINE 1
#define DEE_VFSPROCPIDMEMBER_CWD     2
#define DEE_VFSPROCPIDMEMBER_EXE     3
#define DEE_VFSPROCPIDMEMBER_FD      4
#define DEE_VFSPROCPIDMEMBER_ROOT    5
#define DEE_VFSPROCPIDMEMBER_BEGIN   1
#define DEE_VFSPROCPIDMEMBER_END     6
static int DeeVFSProcPIDNode_GetMemberID(char const *name, Dee_size_t name_size) {
#define CHECK(s) (name_size == (sizeof(s)/sizeof(char))-1 && memcmp(name,s,sizeof(s)-sizeof(char)) == 0)
 if (CHECK("cmdline")) return DEE_VFSPROCPIDMEMBER_CMDLINE;
 if (CHECK("cwd")) return DEE_VFSPROCPIDMEMBER_CWD;
 if (CHECK("exe")) return DEE_VFSPROCPIDMEMBER_EXE;
 if (CHECK("fd")) return DEE_VFSPROCPIDMEMBER_FD;
 if (CHECK("root")) return DEE_VFSPROCPIDMEMBER_ROOT;
#undef CHECK
 return DEE_VFSPROCPIDMEMBER_NONE;
}
static struct DeeVFSNode *DeeVFSProcPIDNode_GetMember(
 struct DeeVFSProcPIDNode *self, int member_id) {
 struct DeeVFSNode *result; struct DeeVFSNodeType const *node_type;
 switch (member_id) {
  case DEE_VFSPROCPIDMEMBER_CMDLINE: node_type = &DeeVFSProcPIDNode_Type_cmdline; break;
  case DEE_VFSPROCPIDMEMBER_CWD: node_type = &DeeVFSProcPIDNode_Type_cwd; break;
  case DEE_VFSPROCPIDMEMBER_EXE: node_type = &DeeVFSProcPIDNode_Type_exe; break;
  case DEE_VFSPROCPIDMEMBER_FD: node_type = &DeeVFSProcPIDNode_Type_fd; break;
  case DEE_VFSPROCPIDMEMBER_ROOT: node_type = &DeeVFSProcPIDNode_Type_root; break;
  default: DEE_BUILTIN_UNREACHABLE();
 }
 if ((result = DeeVFSNode_ALLOC(struct DeeVFSNode)) == NULL) return NULL;
 DeeVFSNode_InitWithParent(result,node_type,&self->vpn_node);
 return result;
}
static DeeObject *DEE_CALL _deevfs_procpidnode_nameof(
 struct DeeVFSProcPIDNode *DEE_UNUSED(self), struct DeeVFSNode *child) {
 struct DeeVFSNodeType const *node_type = child->vn_type;
 if (node_type == &DeeVFSProcPIDNode_Type_cmdline) DeeReturn_STATIC_STRING("cmdline");
 if (node_type == &DeeVFSProcPIDNode_Type_cwd) DeeReturn_STATIC_STRING("cwd");
 if (node_type == &DeeVFSProcPIDNode_Type_exe) DeeReturn_STATIC_STRING("exe");
 if (node_type == &DeeVFSProcPIDNode_Type_root) DeeReturn_STATIC_STRING("root");
 if (node_type == &DeeVFSProcPIDNode_Type_fd) DeeReturn_STATIC_STRING("fd");
 DEE_ASSERTF(0,"%R is not a parent of /proc/[PID]",
             DeeVFSNode_Filename(child));
 DEE_BUILTIN_UNREACHABLE();
}

static struct DeeVFSNode *DEE_CALL _deevfs_procpidnode_walk(
 struct DeeVFSProcPIDNode *self, char const *name, Dee_size_t name_size) {
 int member = DeeVFSProcPIDNode_GetMemberID(name,name_size);
 if (member == DEE_VFSPROCPIDMEMBER_NONE) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Failed to find %$q in %R",name_size,name,
                      DeeVFSNode_Filename((struct DeeVFSNode *)self));
  return NULL;
 }
 return DeeVFSProcPIDNode_GetMember(self,member);
}
static int DEE_CALL _deevfs_procpidview_vvt_open(struct DeeVFSProcPIDView *self) {
 DEE_ASSERT(self->vpv_view.vv_node->vn_type == &DeeVFSProcPIDNode_Type);
 self->vpv_proc = ((struct DeeVFSProcPIDNode *)self->vpv_view.vv_node)->vpn_proc;
 Dee_INCREF(self->vpv_proc);
 self->vpv_member = DEE_VFSPROCPIDMEMBER_BEGIN;
 return 0;
}
static void DEE_CALL _deevfs_procpidview_vvt_quit(
 struct DeeVFSProcPIDView *self) { Dee_DECREF(self->vpv_proc); }
static int DEE_CALL _deevfs_procpidview_vvt_curr(
 struct DeeVFSProcPIDView *self, struct DeeVFSNode **result) {
 int member_id;
 member_id = DeeAtomicInt_Load(self->vpv_member,memory_order_seq_cst);
 if (member_id >= DEE_VFSPROCPIDMEMBER_END) return 1;
 DEE_ASSERT(member_id >= DEE_VFSPROCPIDMEMBER_BEGIN);
 *result = DeeVFSProcPIDNode_GetMember((struct DeeVFSProcPIDNode *)self->vpv_view.vv_node,member_id);
 return *result ? 0 : -1;
}
static int DEE_CALL _deevfs_procpidview_vvt_yield(
 struct DeeVFSProcPIDView *self, struct DeeVFSNode **result) {
 int member_id;
 member_id = DeeAtomicInt_FetchInc(self->vpv_member,memory_order_seq_cst);
 if (member_id >= DEE_VFSPROCPIDMEMBER_END) { DeeAtomicInt_FetchDec(self->vpv_member,memory_order_seq_cst); return 1; }
 DEE_ASSERT(member_id >= DEE_VFSPROCPIDMEMBER_BEGIN);
 *result = DeeVFSProcPIDNode_GetMember((struct DeeVFSProcPIDNode *)self->vpv_view.vv_node,member_id);
 return *result ? 0 : -1;
}
static struct _DeeVFSViewTypeData _deevfs_procpidnode_vnt_view = {
 sizeof(struct DeeVFSVirtualDirView),
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,Dee_Utf8Char const *,Dee_size_t))&_deevfs_procpidnode_walk,
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,Dee_WideChar const *,Dee_size_t))NULL,
 (DeeObject *(DEE_CALL *)(struct DeeVFSNode *,struct DeeVFSNode *))                    &_deevfs_procpidnode_nameof,
 (int (DEE_CALL *)(struct DeeVFSView *))                                               &_deevfs_procpidview_vvt_open,
 (void (DEE_CALL *)(struct DeeVFSView *))                                              &_deevfs_procpidview_vvt_quit,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_procpidview_vvt_curr,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_procpidview_vvt_yield,
};
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type = {
 {(void (DEE_CALL *)(struct DeeVFSNode *))&_deevfs_procpidnode_quit,NULL},
 NULL,DeeVFSNoopNodeType_FileData,&_deevfs_procpidnode_vnt_view};

//////////////////////////////////////////////////////////////////////////
static DeeObject *DEE_CALL _deevfs_procpidnode_exe_readlink(struct DeeVFSNode *self) {
 DeeObject *process;
 DEE_ASSERT(self->vn_parent && self->vn_parent->vn_type == &DeeVFSProcPIDNode_Type);
 process = (DeeObject *)((struct DeeVFSProcPIDNode *)self->vn_parent)->vpn_proc;
 return DeeProcess_Exe(process);
}
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_exe = { /*< '/proc/[PID]/exe' */
 {NULL,(DeeObject *(DEE_CALL *)(struct DeeVFSNode *))&_deevfs_procpidnode_exe_readlink},
 NULL,DeeVFSNoopNodeType_FileData,NULL};
//////////////////////////////////////////////////////////////////////////
static DeeObject *DEE_CALL _deevfs_procpidnode_cwd_readlink(struct DeeVFSNode *self) {
 DeeObject *process;
 DEE_ASSERT(self->vn_parent && self->vn_parent->vn_type == &DeeVFSProcPIDNode_Type);
 process = (DeeObject *)((struct DeeVFSProcPIDNode *)self->vn_parent)->vpn_proc;
 return DeeProcess_Cwd(process);
}
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_cwd = { /*< '/proc/[PID]/cwd' */
 {NULL,(DeeObject *(DEE_CALL *)(struct DeeVFSNode *))&_deevfs_procpidnode_cwd_readlink},
 NULL,DeeVFSNoopNodeType_FileData,NULL};
//////////////////////////////////////////////////////////////////////////
static DeeObject *DEE_CALL _deevfs_procpidnode_root_readlink(struct DeeVFSNode *self) {
 DeeObject *process,*cwd,*root;
 DEE_ASSERT(self->vn_parent && self->vn_parent->vn_type == &DeeVFSProcPIDNode_Type);
 process = (DeeObject *)((struct DeeVFSProcPIDNode *)self->vn_parent)->vpn_proc;
 if DEE_UNLIKELY((cwd = DeeProcess_Cwd(process)) == NULL) return NULL;
 root = DeeFS_PathDriveObject(cwd);
 Dee_DECREF(cwd);
 return root;
}
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_root = { /*< '/proc/[PID]/root' */
 {NULL,(DeeObject *(DEE_CALL *)(struct DeeVFSNode *))&_deevfs_procpidnode_root_readlink},
 NULL,DeeVFSNoopNodeType_FileData,NULL};
//////////////////////////////////////////////////////////////////////////
static int DEE_CALL _deevfs_procpidnode_cmdline_open(struct DeeVFSVirtualFile *self) {
 DeeObject *process,*cmdline;
 DEE_ASSERT(self->vff_file.vf_node->vn_parent &&
            self->vff_file.vf_node->vn_parent->vn_type == &DeeVFSProcPIDNode_Type);
 process = (DeeObject *)((struct DeeVFSProcPIDNode *)self->vff_file.vf_node->vn_parent)->vpn_proc;
 if DEE_UNLIKELY((cmdline = DeeProcess_Cmd(process)) == NULL) return -1;
 DeeVFSVirtualFile_InitWithInheritedText(self,cmdline);
 return 0;
}
static struct _DeeVFSFileTypeData _deevfs_procpidnode_cmdline_vnt_file = {
 sizeof(struct DeeVFSVirtualFile),
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_openmode_t,Dee_mode_t))           &_deevfs_procpidnode_cmdline_open,
 (void(DEE_CALL *)(struct DeeVFSFile *))                                     &DeeVFSVirtualFile_Quit,
 (int (DEE_CALL *)(struct DeeVFSFile *,void *,Dee_size_t,Dee_size_t *))      &DeeVFSVirtualFile_Read,
 (int (DEE_CALL *)(struct DeeVFSFile *,void const *,Dee_size_t,Dee_size_t *))NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_int64_t,int,Dee_uint64_t *))      &DeeVFSVirtualFile_Seek,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
};
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_cmdline = { /*< '/proc/[PID]/cmdline' */
 {NULL,NULL},NULL,&_deevfs_procpidnode_cmdline_vnt_file,NULL};
// --- DeeVFSProcPIDNode_Type ---
//////////////////////////////////////////////////////////////////////////










//////////////////////////////////////////////////////////////////////////
static DEE_A_RET_EXCEPT(-1) int
DeeWin32_GetSystemHandleFromHandleAndProcessID(SYSTEM_HANDLE *result, DWORD pid, WORD hid) {
 SYSTEM_HANDLE_INFORMATION *hinfo;
 SYSTEM_HANDLE *iter,*end;
 // TODO: Is there some way of getting the handle information
 //       without having to re-query !!EVERYTHING!! ?
 hinfo = DeeWin32_CaptureSystemHandleInformation();
 if DEE_UNLIKELY(!hinfo) return -1;
 end = (iter = hinfo->Handles)+hinfo->Count;
 while (iter != end) {
  if (iter->ProcessID == pid && iter->HandleNumber == hid) {
   *result = *iter;
   DeeWin32_FreeSystemHandleInformation(hinfo);
   return 0;
  }
  ++iter;
 }
 DeeWin32_FreeSystemHandleInformation(hinfo);
 DeeError_SetStringf(&DeeErrorType_SystemError,
                     "Failed to find handle: %#I32x:%#I16x",pid,hid);
 return -1;
}

static struct DeeVFSProcPIDFDHIDNode *DEE_CALL _deevfs_procpidnode_fd_vnt_walk(
 struct DeeVFSNode *self, Dee_Utf8Char const *path, Dee_size_t path_size) {
 DWORD pid; WORD hid; struct DeeVFSProcPIDFDHIDNode *result;
 DEE_ASSERT(self->vn_parent->vn_type == &DeeVFSProcPIDNode_Type);
 if DEE_UNLIKELY(DeeUtf8String_ToNumberWithLength(WORD,path_size,path,&hid) != 0) return NULL;
 pid = DeeProcess_ID((DeeObject *)((struct DeeVFSProcPIDNode *)self->vn_parent)->vpn_proc);
 if DEE_UNLIKELY((result = DeeVFSNode_ALLOC(struct DeeVFSProcPIDFDHIDNode)) == NULL) return NULL;
 if (DeeWin32_GetSystemHandleFromHandleAndProcessID(&result->vpfh_info,pid,hid) != 0) { DeeVFSNode_FREE(result); return NULL; }
 DeeVFSNode_InitWithParent(&result->vpfh_node,&DeeVFSProcPIDNode_Type_fd_link,self);
 return result;
}
static struct DeeVFSProcPIDFDHIDNode *DEE_CALL _deevfs_procpidnode_fd_vnt_wwalk(
 struct DeeVFSNode *self, Dee_WideChar const *path, Dee_size_t path_size) {
 DWORD pid; WORD hid; struct DeeVFSProcPIDFDHIDNode *result;
 DEE_ASSERT(self->vn_parent->vn_type == &DeeVFSProcPIDNode_Type);
 if DEE_UNLIKELY(DeeWideString_ToNumberWithLength(WORD,path_size,path,&hid) != 0) return NULL;
 pid = DeeProcess_ID((DeeObject *)((struct DeeVFSProcPIDNode *)self->vn_parent)->vpn_proc);
 if DEE_UNLIKELY((result = DeeVFSNode_ALLOC(struct DeeVFSProcPIDFDHIDNode)) == NULL) return NULL;
 if (DeeWin32_GetSystemHandleFromHandleAndProcessID(&result->vpfh_info,pid,hid) != 0) { DeeVFSNode_FREE(result); return NULL; }
 DeeVFSNode_InitWithParent(&result->vpfh_node,&DeeVFSProcPIDNode_Type_fd_link,self);
 return result;
}
static DeeObject *DEE_CALL _deevfs_procpidnode_fd_vnt_nameof(
 struct DeeVFSNode *DEE_UNUSED(self), struct DeeVFSProcPIDFDHIDNode *child) {
 DEE_ASSERT(child->vpfh_node.vn_type == &DeeVFSProcPIDNode_Type_fd_link);
 return DeeString_FromUInt16(child->vpfh_info.HandleNumber);
}
static int DEE_CALL _deevfs_procpidnode_fd_vnt_open(struct DeeVFSProcPIDFDView *self) {
 DEE_ASSERTF(self->vpf_view.vv_node &&
             self->vpf_view.vv_node->vn_parent &&
             self->vpf_view.vv_node->vn_parent->vn_type == &DeeVFSProcPIDNode_Type,
             "%R is not a valid \"/proc/[PID]/fd\" node",
             DeeVFSNode_Filename(self->vpf_view.vv_node));
 if ((self->vpf_info = DeeWin32_CaptureSystemHandleInformation()) == NULL) return -1;
 self->vpf_proc = ((struct DeeVFSProcPIDNode *)self->vpf_view.vv_node->vn_parent)->vpn_proc;
 Dee_INCREF(self->vpf_proc);
 self->vpf_curr = self->vpf_info->Handles;
 self->vpf_end = self->vpf_curr+self->vpf_info->Count;
 self->vpf_pid = DeeProcess_ID((DeeObject *)self->vpf_proc);
 return 0;
}
static void DEE_CALL _deevfs_procpidnode_fd_vnt_quit(struct DeeVFSProcPIDFDView *self) {
 DeeWin32_FreeSystemHandleInformation(self->vpf_info);
 Dee_DECREF(self->vpf_proc);
}
static SYSTEM_HANDLE *DEE_CALL DeeVFSProcPIDFD_LocateNextHandle(
 struct DeeVFSProcPIDFDView *self, int advance) {
 SYSTEM_HANDLE *result,*newpos; int should_return;
 do {
  result = (SYSTEM_HANDLE *)DeeAtomicPtr_Load(self->vpf_curr,memory_order_seq_cst);
  if (result < self->vpf_info->Handles || result >= self->vpf_end) return NULL;
  should_return = result->ProcessID == self->vpf_pid && DeeWin32_CapturedHandleIsSupported(result);
  newpos = (advance || !should_return) ? result+1 : result;
 } while (!DeeAtomicPtr_CompareExchangeStrong(
  self->vpf_curr,result,newpos,
  memory_order_seq_cst,memory_order_seq_cst) || !should_return);
 return result;
}

static int DEE_CALL _deevfs_procpidnode_fd_vnt_curr(
 struct DeeVFSProcPIDFDView *self, struct DeeVFSProcPIDFDHIDNode **result) {
 struct DeeVFSProcPIDFDHIDNode *resultnode; SYSTEM_HANDLE *my_handle;
 if (( my_handle = DeeVFSProcPIDFD_LocateNextHandle(self,0)) == NULL) return 1; // End of directory
 if DEE_UNLIKELY((resultnode = DeeVFSNode_ALLOC(struct DeeVFSProcPIDFDHIDNode)) == NULL) return -1;
 resultnode->vpfh_info = *my_handle;
 DeeVFSNode_InitWithParent(&resultnode->vpfh_node,
                           &DeeVFSProcPIDNode_Type_fd_link,
                           self->vpf_view.vv_node);
 *result = resultnode;
 return 0;
}
static int DEE_CALL _deevfs_procpidnode_fd_vnt_yield(
 struct DeeVFSProcPIDFDView *self, struct DeeVFSProcPIDFDHIDNode **result) {
 struct DeeVFSProcPIDFDHIDNode *resultnode; SYSTEM_HANDLE *my_handle;
 if (( my_handle = DeeVFSProcPIDFD_LocateNextHandle(self,1)) == NULL) return 1; // End of directory
 if DEE_UNLIKELY((resultnode = DeeVFSNode_ALLOC(struct DeeVFSProcPIDFDHIDNode)) == NULL) return -1;
 resultnode->vpfh_info = *my_handle;
 DeeVFSNode_InitWithParent(&resultnode->vpfh_node,
                           &DeeVFSProcPIDNode_Type_fd_link,
                           self->vpf_view.vv_node);
 *result = resultnode;
 return 0;
}

static struct _DeeVFSViewTypeData _deevfs_procpidnode_fd_vnt_view = {
 sizeof(struct DeeVFSProcPIDFDView),
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,Dee_Utf8Char const *,Dee_size_t))&_deevfs_procpidnode_fd_vnt_walk,
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,Dee_WideChar const *,Dee_size_t))&_deevfs_procpidnode_fd_vnt_wwalk,
 (DeeObject *(DEE_CALL *)(struct DeeVFSNode *,struct DeeVFSNode *))                    &_deevfs_procpidnode_fd_vnt_nameof,
 (int (DEE_CALL *)(struct DeeVFSView *))                                               &_deevfs_procpidnode_fd_vnt_open,
 (void (DEE_CALL *)(struct DeeVFSView *))                                              &_deevfs_procpidnode_fd_vnt_quit,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_procpidnode_fd_vnt_curr,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_procpidnode_fd_vnt_yield,
};
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_fd = {
 {NULL,NULL},NULL,DeeVFSNoopNodeType_FileData,&_deevfs_procpidnode_fd_vnt_view};
//////////////////////////////////////////////////////////////////////////
static DEE_A_RET_EXCEPT_REF DeeObject *
DeeWin32_GetSystemHandleLink(SYSTEM_HANDLE const *h) {
 DEE_ASSERT(h);
 // TODO: Proper, type-specific link names
 // e.g.: An actual file link if it's a FILE-handle
 return DeeString_Newf("%#I32x:%s:%#I16x",
                       h->ProcessID,
                       DeeWin32_GetSystemHandleTypeName(h->HandleType),
                       h->HandleNumber);
}
static DeeObject *_deevfs_procpidnodefd_hid_vnt_readlink(struct DeeVFSProcPIDFDHIDNode *self) {
 return DeeWin32_GetSystemHandleLink(&self->vpfh_info);
}
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_fd_link = {
 {NULL,(DeeObject *(*)(struct DeeVFSNode *))&_deevfs_procpidnodefd_hid_vnt_readlink},
 NULL,DeeVFSNoopNodeType_FileData,NULL};
//////////////////////////////////////////////////////////////////////////












//////////////////////////////////////////////////////////////////////////
// +++ DeeVFSProcSelfNode_Type +++
struct DeeVFSNode _DeeVFS_ProcSelf = DeeVFSNode_INIT(&DeeVFSProcSelfNode_Type,DeeVFS_Proc);
static DeeObject *DEE_CALL _deevfs_procself_readlink(struct DeeVFSNode *DEE_UNUSED(self)) {
 DWORD pid = GetCurrentProcessId();
 return DeeString_FromUInt32(pid);
}
struct DeeVFSNodeType const DeeVFSProcSelfNode_Type = {
 {NULL,(DeeObject *(DEE_CALL *)(struct DeeVFSNode *))&_deevfs_procself_readlink},
 NULL,DeeVFSNoopNodeType_FileData,NULL};
// --- DeeVFSProcSelfNode_Type ---
//////////////////////////////////////////////////////////////////////////


DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#endif /* !GUARD_DEEMON_VFS_VFS_PROC_NODE_C_INL */
