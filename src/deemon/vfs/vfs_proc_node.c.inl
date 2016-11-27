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
#include <deemon/vfs/__vfsconf.inl>
#if DEE_VFSCONFIG_HAVEFILE_PROC
#include <deemon/vfs/vfs_core.h>
#include <deemon/vfs/vfs_proc_node.h>
#include <deemon/vfs/vfs_root.h>
#include <deemon/vfs/vfs_virtual_file.h>
#include <deemon/sys/win32/sysfd.h>
#include <deemon/unicode/char_traits.inl>

DEE_DECL_BEGIN



//////////////////////////////////////////////////////////////////////////
// +++ DeeVFSProcNode_Type +++
struct DeeVFSVirtualDirEntry const DeeVFSProc_VirtualEntries[] = {
#if DEE_VFSCONFIG_HAVEFILE_PROC_SELF
 {"self",DeeVFS_ProcSelf},
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_SELF */
 {NULL,NULL},
};
struct DeeVFSNode _DeeVFS_Proc = DeeVFSNode_INIT(&DeeVFSProcNode_Type,DeeVFS_Root);

static struct DeeVFSNode *DEE_CALL _deevfs_procnode_vnt_walk(
 struct DeeVFSNode *DEE_UNUSED(self), Dee_Utf8Char const *name, Dee_size_t name_size) {
 struct DeeVFSVirtualDirEntry const *eiter;
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID
 DWORD pid = 0; Dee_Utf8Char const *iter,*end;
 end = (iter = name)+name_size;
 while (iter != end) {
  if (*iter < '0' || *iter > '9') goto nopid;
  pid = (pid*10)+(*iter-'0');
  ++iter;
 }
 return (struct DeeVFSNode *)DeeVFSProcPIDNode_NewFromPID(pid);
nopid:
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID */
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
 struct DeeVFSVirtualDirEntry const *eiter;
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID
 DWORD pid = 0; Dee_WideChar const *iter,*end;
 end = (iter = name)+name_size;
 while (iter != end) {
  if (*iter < '0' || *iter > '9') goto nopid;
  pid = (pid*10)+(*iter-'0');
  ++iter;
 }
 return (struct DeeVFSNode *)DeeVFSProcPIDNode_NewFromPID(pid);
nopid:
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID */
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
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID
 if (child->vn_type == &DeeVFSProcPIDNode_Type) {
  DWORD pid = DeeProcess_ID((DeeObject *)((struct DeeVFSProcPIDNode *)child)->vpn_proc);
  return DeeString_FromUInt32(pid);
 }
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID */
 for (eiter = DeeVFSProc_VirtualEntries;;++eiter) {
  DEE_ASSERTF(eiter->name,"Given %R node isn't a child of \"/proc\"",
              DeeVFSNode_Filename(child));
  if (eiter->node == child) return DeeString_New(eiter->name);
 }
}

static int DEE_CALL _deevfs_procview_vvt_open(struct DeeVFSProcView *self) {
#if DEE_VFSCONFIG_HAVE_PROC_ENUMPIDS
 if DEE_UNLIKELY((self->vpr_procs = (DeeListObject *)
  DeeProcess_Enumerate()) == NULL) return -1;
 DEE_ASSERT(DeeObject_Check(self->vpr_procs) &&
            DeeList_Check(self->vpr_procs));
 self->vpr_procend = (self->vpr_procpos = (DeeProcessObject **)
                      DeeList_ELEM(self->vpr_procs))+DeeList_SIZE(self->vpr_procs);
#endif /* DEE_VFSCONFIG_HAVE_PROC_ENUMPIDS */
 self->vpr_pos = DeeVFSProc_VirtualEntries;
 DeeAtomicMutex_Init(&self->vpr_lock);
 return 0;
}
#if DEE_VFSCONFIG_HAVE_PROC_ENUMPIDS
static void DEE_CALL _deevfs_procview_vvt_quit(struct DeeVFSProcView *self) {
 Dee_DECREF(self->vpr_procs);
}
#endif /* DEE_VFSCONFIG_HAVE_PROC_ENUMPIDS */
static int DEE_CALL _deevfs_procview_vvt_curr(
 struct DeeVFSProcView *self, struct DeeVFSNode **result) {
 DeeAtomicMutex_Acquire(&self->vpr_lock);
#if DEE_VFSCONFIG_HAVE_PROC_ENUMPIDS
 if (self->vpr_procpos != self->vpr_procend) {
  DeeProcessObject *process;
  Dee_INCREF(process = *self->vpr_procpos);
  DeeAtomicMutex_Release(&self->vpr_lock);
  *result = (struct DeeVFSNode *)DeeVFSProcPIDNode_NewFromProcess(process);
  Dee_DECREF(process);
  return *result ? 0 : -1;
 }
#endif /* DEE_VFSCONFIG_HAVE_PROC_ENUMPIDS */
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
#if DEE_VFSCONFIG_HAVE_PROC_ENUMPIDS
 if (self->vpr_procpos != self->vpr_procend) {
  DeeProcessObject *process;
  Dee_INCREF(process = *self->vpr_procpos);
  ++self->vpr_procpos;
  DeeAtomicMutex_Release(&self->vpr_lock);
  *result = (struct DeeVFSNode *)DeeVFSProcPIDNode_NewFromProcess(process);
  Dee_DECREF(process);
  return *result ? 0 : -1;
 }
#endif /* DEE_VFSCONFIG_HAVE_PROC_ENUMPIDS */
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
#if DEE_VFSCONFIG_HAVE_PROC_ENUMPIDS
 (void (DEE_CALL *)(struct DeeVFSView *))                                              &_deevfs_procview_vvt_quit,
#else /* DEE_VFSCONFIG_HAVE_PROC_ENUMPIDS */
 (void (DEE_CALL *)(struct DeeVFSView *))                                              NULL,
#endif /* !DEE_VFSCONFIG_HAVE_PROC_ENUMPIDS */
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_procview_vvt_curr,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_procview_vvt_yield,
};
struct DeeVFSNodeType const DeeVFSProcNode_Type = {
 {NULL,NULL},NULL,DeeVFSNoopNodeType_FileData,
 &_deevfs_procnode_vnt_view};
// --- DeeVFSProcNode_Type ---
//////////////////////////////////////////////////////////////////////////







#if DEE_VFSCONFIG_HAVEFILE_PROC_PID
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
enum{
 DEE_VFSPROCPID_MEMBER_NONE = 0,
#define DEE_VFSPROCPID_MEMBER_BEGIN 1
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_CMDLINE
 DEE_VFSPROCPID_MEMBER_CMDLINE,
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_CMDLINE */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_CWD
 DEE_VFSPROCPID_MEMBER_CWD,
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_CWD */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_EXE
 DEE_VFSPROCPID_MEMBER_EXE,
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_EXE */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD
 DEE_VFSPROCPID_MEMBER_FD,
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_ROOT
 DEE_VFSPROCPID_MEMBER_ROOT,
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_ROOT */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32
 DEE_VFSPROCPID_MEMBER_W32,
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32 */
 DEE_VFSPROCPID_MEMBER_END,
};
static unsigned int DeeVFSProcPIDNode_GetMemberID(char const *name, Dee_size_t name_size) {
#define CHECK(s) (name_size == (sizeof(s)/sizeof(char))-1 && memcmp(name,s,sizeof(s)-sizeof(char)) == 0)
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_CMDLINE
 if (CHECK("cmdline")) return DEE_VFSPROCPID_MEMBER_CMDLINE;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_CMDLINE */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_CWD
 if (CHECK("cwd")) return DEE_VFSPROCPID_MEMBER_CWD;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_CWD */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_EXE
 if (CHECK("exe")) return DEE_VFSPROCPID_MEMBER_EXE;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_EXE */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD
 if (CHECK("fd")) return DEE_VFSPROCPID_MEMBER_FD;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_ROOT
 if (CHECK("root")) return DEE_VFSPROCPID_MEMBER_ROOT;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_ROOT */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32
 if (CHECK("w32")) return DEE_VFSPROCPID_MEMBER_W32;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32 */
#undef CHECK
 return DEE_VFSPROCPID_MEMBER_NONE;
}
static struct DeeVFSNode *DeeVFSProcPIDNode_GetMember(
 struct DeeVFSProcPIDNode *self, unsigned int member_id) {
 struct DeeVFSNode *result; struct DeeVFSNodeType const *node_type;
 switch (member_id) {
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_CMDLINE
  case DEE_VFSPROCPID_MEMBER_CMDLINE: node_type = &DeeVFSProcPIDNode_Type_cmdline; break;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_CMDLINE */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_CWD
  case DEE_VFSPROCPID_MEMBER_CWD: node_type = &DeeVFSProcPIDNode_Type_cwd; break;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_CWD */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_EXE
  case DEE_VFSPROCPID_MEMBER_EXE: node_type = &DeeVFSProcPIDNode_Type_exe; break;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_EXE */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD
  case DEE_VFSPROCPID_MEMBER_FD:
#if !DEE_VFSCONFIG_HAVE_PROC_PID_FD_NOSELF
   if (DeeProcess_ID((DeeObject *)self->vpn_proc) != GetCurrentProcessId()) {
    DeeError_SET_STRING(&DeeErrorType_SystemError,
                        "/proc/[PID]/fd can only be accessed by /proc/self");
    return NULL;
   }
#endif
   node_type = &DeeVFSProcPIDNode_Type_fd;
   break;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_ROOT
  case DEE_VFSPROCPID_MEMBER_ROOT: node_type = &DeeVFSProcPIDNode_Type_root; break;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_ROOT */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32
  case DEE_VFSPROCPID_MEMBER_W32: node_type = &DeeVFSProcPIDNode_Type_w32; break;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32 */
  default: DEE_BUILTIN_UNREACHABLE();
 }
 if ((result = DeeVFSNode_ALLOC(struct DeeVFSNode)) == NULL) return NULL;
 DeeVFSNode_InitWithParent(result,node_type,&self->vpn_node);
 return result;
}
static DeeObject *DEE_CALL _deevfs_procpidnode_nameof(
 struct DeeVFSProcPIDNode *DEE_UNUSED(self), struct DeeVFSNode *child) {
 struct DeeVFSNodeType const *node_type = child->vn_type;
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_CMDLINE
 if (node_type == &DeeVFSProcPIDNode_Type_cmdline) DeeReturn_STATIC_STRING("cmdline");
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_CMDLINE */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_CWD
 if (node_type == &DeeVFSProcPIDNode_Type_cwd) DeeReturn_STATIC_STRING("cwd");
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_CWD */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_EXE
 if (node_type == &DeeVFSProcPIDNode_Type_exe) DeeReturn_STATIC_STRING("exe");
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_EXE */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_ROOT
 if (node_type == &DeeVFSProcPIDNode_Type_root) DeeReturn_STATIC_STRING("root");
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_ROOT */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD
 if (node_type == &DeeVFSProcPIDNode_Type_fd) DeeReturn_STATIC_STRING("fd");
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32
 if (node_type == &DeeVFSProcPIDNode_Type_w32) DeeReturn_STATIC_STRING("w32");
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32 */
 DEE_ASSERTF(0,"%R is not a parent of /proc/[PID]",
             DeeVFSNode_Filename(child));
 DEE_BUILTIN_UNREACHABLE();
}

static struct DeeVFSNode *DEE_CALL _deevfs_procpidnode_walk(
 struct DeeVFSProcPIDNode *self, char const *name, Dee_size_t name_size) {
 unsigned int member = DeeVFSProcPIDNode_GetMemberID(name,name_size);
 if (member == DEE_VFSPROCPID_MEMBER_NONE) {
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
 self->vpv_member = DEE_VFSPROCPID_MEMBER_BEGIN;
 return 0;
}
static void DEE_CALL _deevfs_procpidview_vvt_quit(
 struct DeeVFSProcPIDView *self) { Dee_DECREF(self->vpv_proc); }
static int DEE_CALL _deevfs_procpidview_vvt_curr(
 struct DeeVFSProcPIDView *self, struct DeeVFSNode **result) {
 unsigned int member_id;
 member_id = (unsigned int)DeeAtomicInt_Load(self->vpv_member,memory_order_seq_cst);
#if !DEE_VFSCONFIG_HAVE_PROC_PID_FD_NOSELF && DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD
 if (member_id == DEE_VFSPROCPID_MEMBER_FD &&
     DeeProcess_ID((DeeObject *)self->vpv_proc) != GetCurrentProcessId()
     ) ++member_id;
#endif
 if (member_id >= DEE_VFSPROCPID_MEMBER_END) return 1;
 DEE_ASSERT(member_id >= DEE_VFSPROCPID_MEMBER_BEGIN);
 *result = DeeVFSProcPIDNode_GetMember((struct DeeVFSProcPIDNode *)self->vpv_view.vv_node,member_id);
 return *result ? 0 : -1;
}
static int DEE_CALL _deevfs_procpidview_vvt_yield(
 struct DeeVFSProcPIDView *self, struct DeeVFSNode **result) {
 unsigned int member_id;
#if !DEE_VFSCONFIG_HAVE_PROC_PID_FD_NOSELF && DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD
again:
 member_id = (unsigned int)DeeAtomicInt_FetchInc(self->vpv_member,memory_order_seq_cst);
 if (member_id == DEE_VFSPROCPID_MEMBER_FD &&
     DeeProcess_ID((DeeObject *)self->vpv_proc) != GetCurrentProcessId()) {
  if (!DeeAtomicInt_CompareExchangeStrong(self->vpv_member,member_id,member_id+1,
   memory_order_seq_cst,memory_order_seq_cst)) goto again;
  ++member_id;
 }
#else
 member_id = (unsigned int)DeeAtomicInt_FetchInc(self->vpv_member,memory_order_seq_cst);
#endif
 if (member_id >= DEE_VFSPROCPID_MEMBER_END) { DeeAtomicInt_FetchDec(self->vpv_member,memory_order_seq_cst); return 1; }
 DEE_ASSERT(member_id >= DEE_VFSPROCPID_MEMBER_BEGIN);
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
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_CWD
static DeeObject *DEE_CALL _deevfs_procpidnode_cwd_readlink(struct DeeVFSNode *self) {
 DeeObject *process;
 DEE_ASSERT(self->vn_parent && self->vn_parent->vn_type == &DeeVFSProcPIDNode_Type);
 process = (DeeObject *)((struct DeeVFSProcPIDNode *)self->vn_parent)->vpn_proc;
 return DeeProcess_Cwd(process);
}
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_cwd = { /*< '/proc/[PID]/cwd' */
 {NULL,(DeeObject *(DEE_CALL *)(struct DeeVFSNode *))&_deevfs_procpidnode_cwd_readlink},
 NULL,DeeVFSNoopNodeType_FileData,NULL};
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_CWD */
//////////////////////////////////////////////////////////////////////////
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_EXE
static DeeObject *DEE_CALL _deevfs_procpidnode_exe_readlink(struct DeeVFSNode *self) {
 DeeObject *process;
 DEE_ASSERT(self->vn_parent && self->vn_parent->vn_type == &DeeVFSProcPIDNode_Type);
 process = (DeeObject *)((struct DeeVFSProcPIDNode *)self->vn_parent)->vpn_proc;
 return DeeProcess_Exe(process);
}
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_exe = { /*< '/proc/[PID]/exe' */
 {NULL,(DeeObject *(DEE_CALL *)(struct DeeVFSNode *))&_deevfs_procpidnode_exe_readlink},
 NULL,DeeVFSNoopNodeType_FileData,NULL};
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_EXE */
//////////////////////////////////////////////////////////////////////////
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_ROOT
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
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_ROOT */
//////////////////////////////////////////////////////////////////////////
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_CMDLINE
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
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_CMDLINE */
// --- DeeVFSProcPIDNode_Type ---
//////////////////////////////////////////////////////////////////////////










//////////////////////////////////////////////////////////////////////////
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD
static DEE_A_RET_EXCEPT(-1) int
DeeWin32_GetSystemHandleFromHandleAndProcessID(SYSTEM_HANDLE *result, DWORD pid, WORD hid) {
 SYSTEM_HANDLE_INFORMATION *hinfo; SYSTEM_HANDLE *iter,*end;
 if (pid == GetCurrentProcessId()) {
  result->ObjectTypeNumber = DeeWin32_GetEffectiveHandleTypeID((HANDLE)hid);
  result->Handle = hid;
  result->ProcessId = pid;
  return 0;
 } else if ((result->ObjectTypeNumber = DeeWin32_GetEffectivePHIDTypeID(pid,hid)) != 0xFF) {
  result->Handle = hid;
  result->ProcessId = pid;
  return 0;
 }
 // TODO: Is there some way of getting the handle information
 //       without having to re-query !!EVERYTHING!! ?
 hinfo = DeeWin32_CaptureSystemHandleInformation();
 if DEE_UNLIKELY(!hinfo) return -1;
 end = (iter = hinfo->Handles)+hinfo->Count;
 while (iter != end) {
  if (iter->ProcessId == pid && iter->Handle == hid) {
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
 return DeeString_FromUInt16(child->vpfh_info.Handle);
}

#if DEE_VFSCONFIG_HAVE_PROC_PID_FD_ENUMFDS
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
  should_return = result->ProcessId == self->vpf_pid && DeeWin32_CapturedHandleIsSupported(result);
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
#endif /* DEE_VFSCONFIG_HAVE_PROC_PID_FD_ENUMFDS */

static struct _DeeVFSViewTypeData _deevfs_procpidnode_fd_vnt_view = {
#if DEE_VFSCONFIG_HAVE_PROC_PID_FD_ENUMFDS
 sizeof(struct DeeVFSProcPIDFDView),
#else /* DEE_VFSCONFIG_HAVE_PROC_PID_FD_ENUMFDS */
 sizeof(struct DeeVFSView),
#endif /* DEE_VFSCONFIG_HAVE_PROC_PID_FD_ENUMFDS */
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,Dee_Utf8Char const *,Dee_size_t))&_deevfs_procpidnode_fd_vnt_walk,
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,Dee_WideChar const *,Dee_size_t))&_deevfs_procpidnode_fd_vnt_wwalk,
 (DeeObject *(DEE_CALL *)(struct DeeVFSNode *,struct DeeVFSNode *))                    &_deevfs_procpidnode_fd_vnt_nameof,
#if DEE_VFSCONFIG_HAVE_PROC_PID_FD_ENUMFDS
 (int (DEE_CALL *)(struct DeeVFSView *))                                               &_deevfs_procpidnode_fd_vnt_open,
 (void (DEE_CALL *)(struct DeeVFSView *))                                              &_deevfs_procpidnode_fd_vnt_quit,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_procpidnode_fd_vnt_curr,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_procpidnode_fd_vnt_yield,
#else /* DEE_VFSCONFIG_HAVE_PROC_PID_FD_ENUMFDS */
 NULL,NULL,NULL,NULL,
#endif /* DEE_VFSCONFIG_HAVE_PROC_PID_FD_ENUMFDS */
};
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_fd = {
 {NULL,NULL},NULL,DeeVFSNoopNodeType_FileData,&_deevfs_procpidnode_fd_vnt_view};
//////////////////////////////////////////////////////////////////////////
static DEE_A_RET_EXCEPT_REF DeeObject *
DeeWin32_GetSystemHandleLink(SYSTEM_HANDLE const *h) {
 BYTE type_id; HANDLE handle; int owns_handle;
 DeeObject *result;
 DEE_ASSERT(h);
 type_id = DeeWin32_GetHandleType(h);
 owns_handle = (h->ProcessId != GetCurrentProcessId());
 if (owns_handle) handle = DeeWin32_SystemHandle_Open(h);
 else handle = (HANDLE)h->Handle;
 switch (type_id) {
  // TODO: Proper, type-specific link names
  // e.g.: An actual file link if it's a FILE-handle
  case DEE_WIN32_HANDLETYPE_TYPE_THREAD:
   result = DeeString_Newf("[THREAD %#.4I16x]",h->Handle);
   break;
  case DEE_WIN32_HANDLETYPE_TYPE_DIRECTORY:
  case DEE_WIN32_HANDLETYPE_TYPE_FILE:
   result = DeeWin32Sys_WideGetHandleFilename(handle);
   if (!result && DeeError_Catch(&DeeErrorType_SystemError))
    result = DeeString_Newf("[FILE %#.4I16x]",h->Handle);
   break;
  default:
   result = DeeString_Newf("[??" "?(%#.4I16x)]",h->Handle);
   break;
 }
 if (owns_handle) CloseHandle(handle);
 return result;
}
static DeeObject *_deevfs_procpidnodefd_hid_vnt_readlink(struct DeeVFSProcPIDFDHIDNode *self) {
 return DeeWin32_GetSystemHandleLink(&self->vpfh_info);
}
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_fd_link = {
 {NULL,(DeeObject *(*)(struct DeeVFSNode *))&_deevfs_procpidnodefd_hid_vnt_readlink},
 NULL,DeeVFSNoopNodeType_FileData,NULL};
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_FD */
//////////////////////////////////////////////////////////////////////////










//////////////////////////////////////////////////////////////////////////
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32
enum{
 DEE_VFSPROCPIDWIN32_MEMBER_NONE = 0,
#define DEE_VFSPROCPIDWIN32_MEMBER_BEGIN   1
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_CLOSEFD
 DEE_VFSPROCPIDWIN32_MEMBER_CLOSEFD,
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_CLOSEFD */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_EXITCODE
 DEE_VFSPROCPIDWIN32_MEMBER_EXITCODE,
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_EXITCODE */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYBOOST
 DEE_VFSPROCPIDWIN32_MEMBER_PRIORITYBOOST,
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYBOOST */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYCLASS
 DEE_VFSPROCPIDWIN32_MEMBER_PRIORITYCLASS,
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYCLASS */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_TERMINATE
 DEE_VFSPROCPIDWIN32_MEMBER_TERMINATE,
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_TERMINATE */
 DEE_VFSPROCPIDWIN32_MEMBER_END,
};
static unsigned int DeeVFSProcPIDWin32Node_GetMemberID(char const *name, Dee_size_t name_size) {
#define CHECK(s) (name_size == (sizeof(s)/sizeof(char))-1 && memcmp(name,s,sizeof(s)-sizeof(char)) == 0)
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_CLOSEFD
 if (CHECK("closefd")) return DEE_VFSPROCPIDWIN32_MEMBER_CLOSEFD;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_CLOSEFD */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_EXITCODE
 if (CHECK("exitcode")) return DEE_VFSPROCPIDWIN32_MEMBER_EXITCODE;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_EXITCODE */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYBOOST
 if (CHECK("priority_boost")) return DEE_VFSPROCPIDWIN32_MEMBER_PRIORITYBOOST;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYBOOST */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYCLASS
 if (CHECK("priority_class")) return DEE_VFSPROCPIDWIN32_MEMBER_PRIORITYCLASS;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYCLASS */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_TERMINATE
 if (CHECK("terminate")) return DEE_VFSPROCPIDWIN32_MEMBER_TERMINATE;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_TERMINATE */
#undef CHECK
 return DEE_VFSPROCPIDWIN32_MEMBER_NONE;
}
static struct DeeVFSNode *DeeVFSProcPIDWin32Node_GetMember(
 struct DeeVFSNode *self, unsigned int member_id) {
 struct DeeVFSNode *result; struct DeeVFSNodeType const *node_type;
 switch (member_id) {
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_CLOSEFD
  case DEE_VFSPROCPIDWIN32_MEMBER_CLOSEFD: node_type = &DeeVFSProcPIDNode_Type_w32_closefd; break;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_CLOSEFD */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_EXITCODE
  case DEE_VFSPROCPIDWIN32_MEMBER_EXITCODE: node_type = &DeeVFSProcPIDNode_Type_w32_exitcode; break;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_EXITCODE */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYBOOST
  case DEE_VFSPROCPIDWIN32_MEMBER_PRIORITYBOOST: node_type = &DeeVFSProcPIDNode_Type_w32_priorityboost; break;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYBOOST */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYCLASS
  case DEE_VFSPROCPIDWIN32_MEMBER_PRIORITYCLASS: node_type = &DeeVFSProcPIDNode_Type_w32_priorityclass; break;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYCLASS */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_TERMINATE
  case DEE_VFSPROCPIDWIN32_MEMBER_TERMINATE: node_type = &DeeVFSProcPIDNode_Type_w32_terminate; break;
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_TERMINATE */
  default: DEE_BUILTIN_UNREACHABLE();
 }
 if ((result = DeeVFSNode_ALLOC(struct DeeVFSNode)) == NULL) return NULL;
 DeeVFSNode_InitWithParent(result,node_type,self);
 return result;
}
static struct DeeVFSNode *DEE_CALL _deevfs_procpidnode_w32_vnt_walk(
 struct DeeVFSNode *self, Dee_Utf8Char const *name, Dee_size_t name_size) {
 unsigned int member = DeeVFSProcPIDWin32Node_GetMemberID(name,name_size);
 if (member == DEE_VFSPROCPIDWIN32_MEMBER_NONE) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Failed to find %$q in %R",name_size,name,
                      DeeVFSNode_Filename((struct DeeVFSNode *)self));
  return NULL;
 }
 return DeeVFSProcPIDWin32Node_GetMember(self,member);
}

static DeeObject *DEE_CALL _deevfs_procpidnode_w32_vnt_nameof(
 struct DeeVFSNode *DEE_UNUSED(self), struct DeeVFSNode *child) {
 struct DeeVFSNodeType const *node_type = child->vn_type;
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_CLOSEFD
 if (node_type == &DeeVFSProcPIDNode_Type_w32_closefd) DeeReturn_STATIC_STRING("closefd");
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_CLOSEFD */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_EXITCODE
 if (node_type == &DeeVFSProcPIDNode_Type_w32_exitcode) DeeReturn_STATIC_STRING("exitcode");
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_EXITCODE */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYBOOST
 if (node_type == &DeeVFSProcPIDNode_Type_w32_priorityboost) DeeReturn_STATIC_STRING("priority_boost");
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYBOOST */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYCLASS
 if (node_type == &DeeVFSProcPIDNode_Type_w32_priorityclass) DeeReturn_STATIC_STRING("priority_class");
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYCLASS */
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_TERMINATE
 if (node_type == &DeeVFSProcPIDNode_Type_w32_terminate) DeeReturn_STATIC_STRING("terminate");
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_TERMINATE */
 DEE_ASSERTF(0,"%R is not a parent of /proc/[PID]/win32",DeeVFSNode_Filename(child));
 DEE_BUILTIN_UNREACHABLE();
}
static int DEE_CALL _deevfs_procpidnode_w32_vnt_open(
 struct DeeVFSProcPIDW32View *self) {
 DEE_ASSERT(self->vpwv_view.vv_node->vn_type == &DeeVFSProcPIDNode_Type_w32);
 self->vpwv_curr = DEE_VFSPROCPIDWIN32_MEMBER_BEGIN;
 return 0;
}
static int DEE_CALL _deevfs_procpidnode_w32_vnt_curr(
 struct DeeVFSProcPIDW32View *self, struct DeeVFSNode **result) {
 unsigned int member_id = (unsigned int)DeeAtomicInt_Load(self->vpwv_curr,memory_order_seq_cst);
 if (member_id >= DEE_VFSPROCPIDWIN32_MEMBER_END) return 1;
 DEE_ASSERT(member_id >= DEE_VFSPROCPIDWIN32_MEMBER_BEGIN);
 *result = DeeVFSProcPIDWin32Node_GetMember(self->vpwv_view.vv_node,member_id);
 return *result ? 0 : -1;
}
static int DEE_CALL _deevfs_procpidnode_w32_vnt_yield(
 struct DeeVFSProcPIDW32View *self, struct DeeVFSNode **result) {
 unsigned int member_id = (unsigned int)DeeAtomicInt_FetchInc(self->vpwv_curr,memory_order_seq_cst);
 if (member_id >= DEE_VFSPROCPIDWIN32_MEMBER_END) { DeeAtomicInt_FetchDec(self->vpwv_curr,memory_order_seq_cst); return 1; }
 DEE_ASSERT(member_id >= DEE_VFSPROCPIDWIN32_MEMBER_BEGIN);
 *result = DeeVFSProcPIDWin32Node_GetMember(self->vpwv_view.vv_node,member_id);
 return *result ? 0 : -1;
}
static struct _DeeVFSViewTypeData _deevfs_procpidnode_w32_vnt_view = {
 sizeof(struct DeeVFSProcPIDW32View),
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,Dee_Utf8Char const *,Dee_size_t))&_deevfs_procpidnode_w32_vnt_walk,
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,Dee_WideChar const *,Dee_size_t))NULL,
 (DeeObject *(DEE_CALL *)(struct DeeVFSNode *,struct DeeVFSNode *))                    &_deevfs_procpidnode_w32_vnt_nameof,
 (int (DEE_CALL *)(struct DeeVFSView *))                                               &_deevfs_procpidnode_w32_vnt_open,
 (void (DEE_CALL *)(struct DeeVFSView *))                                              NULL,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_procpidnode_w32_vnt_curr,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))                          &_deevfs_procpidnode_w32_vnt_yield,
};
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_w32 = {
 {NULL,NULL},NULL,DeeVFSNoopNodeType_FileData,&_deevfs_procpidnode_w32_vnt_view};
//////////////////////////////////////////////////////////////////////////
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_CLOSEFD
static int DEE_CALL _deevfs_procpidnodew32_closefd_vft_write(
 struct DeeVFSFile *self, void const *p, Dee_size_t s, Dee_size_t *rs) {
 Dee_uintptr_t hid; DWORD pid,error; DeeObject *proc; HANDLE hMyProcess,hProcess,hClosed;
 char const *cmd_begin,*cmd_end,*end; *rs = s;
 DEE_ASSERT(self->vf_node->vn_type == &DeeVFSProcPIDNode_Type_w32_closefd);
 DEE_ASSERT(self->vf_node->vn_parent && self->vf_node->vn_parent->vn_type == &DeeVFSProcPIDNode_Type_w32);
 DEE_ASSERT(self->vf_node->vn_parent->vn_parent && self->vf_node->vn_parent->vn_parent->vn_type == &DeeVFSProcPIDNode_Type);
 proc = (DeeObject *)((struct DeeVFSProcPIDNode *)self->vf_node->vn_parent->vn_parent)->vpn_proc;
 DEE_ASSERT(DeeObject_Check(proc) && DeeProcess_Check(proc));
 pid = DeeProcess_ID(proc);
 end = (cmd_end = (char const *)p)+s;
 // Acquire access to the other process
 hMyProcess = GetCurrentProcess();
 if (pid == GetCurrentProcessId()) {
#if 1
  // Since the user could potentially just start two instances of deemon
  // capable of closing each other's handles, and the fact that the
  // surprise about something like this even being possible was the
  // reason why I even added '/proc/[PID]/w32/closefd', we might
  // as well not just break the assumptions of millions of other programs
  // concerning open handles, but also break our own.
  // (file.fd does its own tracking of valid handles, and win32 API class
  //  fail with various levels of severity, ranging from error codes
  //  to full on access violations)
  hProcess = hMyProcess;
#else
  DeeError_NotImplemented_str("Can't close handles of /proc/self");
  return -1;
#endif
 } else if ((hProcess = OpenProcess(PROCESS_DUP_HANDLE,FALSE,pid)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "OpenProcess(PROCESS_DUP_HANDLE,FALSE,%lu) : %K",
                      pid,DeeSystemError_Win32ToString(GetLastError()));
  return -1;
 }
 do {
  while (cmd_end != end && (*cmd_end == '\r' || *cmd_end == '\n')) ++cmd_end;
  cmd_begin = cmd_end;
  while (cmd_end != end && (*cmd_end != '\r' && *cmd_end != '\n')) ++cmd_end;
  if (cmd_begin == cmd_end) break;
  while (cmd_begin != cmd_end && DEE_CH_IS_SPACE(cmd_begin[0])) ++cmd_begin;
  while (cmd_begin != cmd_end && DEE_CH_IS_SPACE(cmd_end[-1])) --cmd_end;
  if (cmd_begin != cmd_end) {
   if (DeeString_ToNumberWithLength(Dee_uintptr_t,(Dee_size_t)(cmd_end-cmd_begin),cmd_begin,&hid) != 0) {
err: CloseHandle(hProcess); return -1;
   }
   if (DuplicateHandle(hProcess,(HANDLE)hid,hMyProcess,&hClosed,0,FALSE,DUPLICATE_CLOSE_SOURCE)) {
    // Simply close the transferred handle
    CloseHandle(hClosed);
   } else {
    error = GetLastError();
    if (error == ERROR_NOT_SUPPORTED) {
     // Some handles cannot be duplicated.
     // Although windows doesn't say if this works, even stating that doing this may produce leaks,
     // lets try to ~duplicate~ the handles without providing destination informations (and possibly skip the copy)
     if (DuplicateHandle(hProcess,(HANDLE)hid,NULL,NULL,0,FALSE,DUPLICATE_CLOSE_SOURCE)) goto succ;
     // NOPE! Didn't work...
     error = GetLastError();
    }
    DeeError_SetStringf(&DeeErrorType_SystemError,
                        "DuplicateHandle(%p,%p,%p,...,0,FALSE,DUPLICATE_CLOSE_SOURCE) : %K",
                        hProcess,(HANDLE)hid,hMyProcess,
                        DeeSystemError_Win32ToString(error));
    goto err;
   }
  }
succ:;
 } while (cmd_end != end);
 // Cleanup the process handle
 CloseHandle(hProcess);
 return 0;
}
static struct _DeeVFSFileTypeData _deevfs_procpidnodew32_closefd_vnt_file = {
 sizeof(struct DeeVFSFile),
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_openmode_t,Dee_mode_t))           &_deevfs_genericfile_openwriteonly,
 (void(DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *,void *,Dee_size_t,Dee_size_t *))      NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *,void const *,Dee_size_t,Dee_size_t *))&_deevfs_procpidnodew32_closefd_vft_write,
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_int64_t,int,Dee_uint64_t *))      NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
};
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_w32_closefd = {
 {NULL,NULL},NULL,&_deevfs_procpidnodew32_closefd_vnt_file,NULL};
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_CLOSEFD */
//////////////////////////////////////////////////////////////////////////
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_EXITCODE
static int DEE_CALL _deevfs_procpidnodew32_exitcode_vft_read(
 struct DeeVFSFile *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 DeeObject *proc,*result_str; DWORD pid; HANDLE hProcess; DWORD result;
 DEE_ASSERT(self->vf_node->vn_type == &DeeVFSProcPIDNode_Type_w32_exitcode);
 DEE_ASSERT(self->vf_node->vn_parent && self->vf_node->vn_parent->vn_type == &DeeVFSProcPIDNode_Type_w32);
 DEE_ASSERT(self->vf_node->vn_parent->vn_parent && self->vf_node->vn_parent->vn_parent->vn_type == &DeeVFSProcPIDNode_Type);
 proc = (DeeObject *)((struct DeeVFSProcPIDNode *)self->vf_node->vn_parent->vn_parent)->vpn_proc;
 pid = DeeProcess_ID(proc);
 if DEE_UNLIKELY((hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,FALSE,pid)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,FALSE,%lu) : %K",
                      pid,DeeSystemError_Win32ToString(GetLastError()));
  return -1;
 }
 if DEE_UNLIKELY(!GetExitCodeProcess(hProcess,&result)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "GetExitCodeProcess(%p,...) : %K",
                      hProcess,DeeSystemError_Win32ToString(GetLastError()));
  CloseHandle(hProcess);
  return -1;
 }
 CloseHandle(hProcess);
 result_str = DeeString_FromUInt32(result);
 if DEE_UNLIKELY(!result_str) return -1;
 *rs = DeeString_SIZE(result_str) > s ? s : DeeString_SIZE(result_str);
 memcpy(p,DeeString_STR(result_str),*rs);
 Dee_DECREF(result_str);
 return 0;
}
static struct _DeeVFSFileTypeData _deevfs_procpidnodew32_exitcode_vnt_file = {
 sizeof(struct DeeVFSFile),
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_openmode_t,Dee_mode_t))           &_deevfs_genericfile_openreadonly,
 (void(DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *,void *,Dee_size_t,Dee_size_t *))      &_deevfs_procpidnodew32_exitcode_vft_read,
 (int (DEE_CALL *)(struct DeeVFSFile *,void const *,Dee_size_t,Dee_size_t *))NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_int64_t,int,Dee_uint64_t *))      NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
};
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_w32_exitcode = {
 {NULL,NULL},NULL,&_deevfs_procpidnodew32_exitcode_vnt_file,NULL};
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_EXITCODE */
//////////////////////////////////////////////////////////////////////////
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYBOOST
static int DEE_CALL _deevfs_procpidnodew32_priorityboost_vft_read(
 struct DeeVFSFile *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 DeeObject *proc; DWORD pid; HANDLE hProcess; BOOL result;
 DEE_ASSERT(self->vf_node->vn_type == &DeeVFSProcPIDNode_Type_w32_priorityboost);
 DEE_ASSERT(self->vf_node->vn_parent && self->vf_node->vn_parent->vn_type == &DeeVFSProcPIDNode_Type_w32);
 DEE_ASSERT(self->vf_node->vn_parent->vn_parent && self->vf_node->vn_parent->vn_parent->vn_type == &DeeVFSProcPIDNode_Type);
 proc = (DeeObject *)((struct DeeVFSProcPIDNode *)self->vf_node->vn_parent->vn_parent)->vpn_proc;
 pid = DeeProcess_ID(proc);
 if DEE_UNLIKELY((hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,FALSE,pid)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,FALSE,%lu) : %K",
                      pid,DeeSystemError_Win32ToString(GetLastError()));
  return -1;
 }
 if DEE_UNLIKELY(!GetProcessPriorityBoost(hProcess,&result)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "GetProcessPriorityBoost(%p,...) : %K",
                      hProcess,DeeSystemError_Win32ToString(GetLastError()));
  CloseHandle(hProcess);
  return -1;
 }
 CloseHandle(hProcess);
 if ((*rs = (Dee_size_t)(s ? 1 : 0)) != 0) *(char *)p = result ? '0' : '1';
 return 0;
}
static int DEE_CALL _deevfs_procpidnodew32_priorityboost_vft_write(
 struct DeeVFSFile *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 int value; char const *begin,*end; DeeObject *proc; DWORD pid; HANDLE hProcess;
 DEE_ASSERT(self->vf_node->vn_type == &DeeVFSProcPIDNode_Type_w32_priorityboost);
 DEE_ASSERT(self->vf_node->vn_parent && self->vf_node->vn_parent->vn_type == &DeeVFSProcPIDNode_Type_w32);
 DEE_ASSERT(self->vf_node->vn_parent->vn_parent && self->vf_node->vn_parent->vn_parent->vn_type == &DeeVFSProcPIDNode_Type);
 proc = (DeeObject *)((struct DeeVFSProcPIDNode *)self->vf_node->vn_parent->vn_parent)->vpn_proc;
 pid = DeeProcess_ID(proc);
 end = (begin = (char const *)p)+s;
 while (begin != end && DEE_CH_IS_SPACE(begin[0])) ++begin;
 while (begin != end && DEE_CH_IS_SPACE(end[-1])) --end;
 if (DeeString_ToNumberWithLength(int,(Dee_size_t)(end-begin),begin,&value) != 0) return -1;
 if DEE_UNLIKELY((hProcess = OpenProcess(PROCESS_SET_INFORMATION,FALSE,pid)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "OpenProcess(PROCESS_SET_INFORMATION,FALSE,%lu) : %K",
                      pid,DeeSystemError_Win32ToString(GetLastError()));
  return -1;
 }
 if DEE_UNLIKELY(!SetProcessPriorityBoost(hProcess,value ? FALSE : TRUE)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "SetProcessPriorityBoost(%p,%s) : %K",
                      hProcess,value ? "FALSE" : "TRUE",
                      DeeSystemError_Win32ToString(GetLastError()));
  CloseHandle(hProcess);
  return -1;
 }
 CloseHandle(hProcess);
 *ws = s;
 return 0;
}
static struct _DeeVFSFileTypeData _deevfs_procpidnodew32_priorityboost_vnt_file = {
 sizeof(struct DeeVFSFile),
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_openmode_t,Dee_mode_t))           &_deevfs_genericfile_openreadwrite,
 (void(DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *,void *,Dee_size_t,Dee_size_t *))      &_deevfs_procpidnodew32_priorityboost_vft_read,
 (int (DEE_CALL *)(struct DeeVFSFile *,void const *,Dee_size_t,Dee_size_t *))&_deevfs_procpidnodew32_priorityboost_vft_write,
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_int64_t,int,Dee_uint64_t *))      NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
};
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_w32_priorityboost = {
 {NULL,NULL},NULL,&_deevfs_procpidnodew32_priorityboost_vnt_file,NULL};
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYBOOST */
//////////////////////////////////////////////////////////////////////////
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYCLASS
static int DEE_CALL _deevfs_procpidnodew32_priorityclass_vft_read(
 struct DeeVFSFile *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 DeeObject *proc,*result_str; DWORD pid; HANDLE hProcess; DWORD result;
 DEE_ASSERT(self->vf_node->vn_type == &DeeVFSProcPIDNode_Type_w32_priorityclass);
 DEE_ASSERT(self->vf_node->vn_parent && self->vf_node->vn_parent->vn_type == &DeeVFSProcPIDNode_Type_w32);
 DEE_ASSERT(self->vf_node->vn_parent->vn_parent && self->vf_node->vn_parent->vn_parent->vn_type == &DeeVFSProcPIDNode_Type);
 proc = (DeeObject *)((struct DeeVFSProcPIDNode *)self->vf_node->vn_parent->vn_parent)->vpn_proc;
 pid = DeeProcess_ID(proc);
 if DEE_UNLIKELY((hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,FALSE,pid)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,FALSE,%lu) : %K",
                      pid,DeeSystemError_Win32ToString(GetLastError()));
  return -1;
 }
 if DEE_UNLIKELY((result = GetPriorityClass(hProcess)) == 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "GetPriorityClass(%p) : %K",hProcess,
                      DeeSystemError_Win32ToString(GetLastError()));
  CloseHandle(hProcess);
  return -1;
 }
 CloseHandle(hProcess);
 result_str = DeeString_FromUInt32(result);
 if DEE_UNLIKELY(!result_str) return -1;
 *rs = DeeString_SIZE(result_str) > s ? s : DeeString_SIZE(result_str);
 memcpy(p,DeeString_STR(result_str),*rs);
 Dee_DECREF(result_str);
 return 0;
}
static int DEE_CALL _deevfs_procpidnodew32_priorityclass_vft_write(
 struct DeeVFSFile *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 DWORD value; char const *begin,*end; DeeObject *proc; DWORD pid; HANDLE hProcess;
 DEE_ASSERT(self->vf_node->vn_type == &DeeVFSProcPIDNode_Type_w32_priorityclass);
 DEE_ASSERT(self->vf_node->vn_parent && self->vf_node->vn_parent->vn_type == &DeeVFSProcPIDNode_Type_w32);
 DEE_ASSERT(self->vf_node->vn_parent->vn_parent && self->vf_node->vn_parent->vn_parent->vn_type == &DeeVFSProcPIDNode_Type);
 proc = (DeeObject *)((struct DeeVFSProcPIDNode *)self->vf_node->vn_parent->vn_parent)->vpn_proc;
 pid = DeeProcess_ID(proc);
 end = (begin = (char const *)p)+s;
 while (begin != end && DEE_CH_IS_SPACE(begin[0])) ++begin;
 while (begin != end && DEE_CH_IS_SPACE(end[-1])) --end;
 if (DeeString_ToNumberWithLength(DWORD,(Dee_size_t)(
  end-begin),begin,(Dee_uint32_t *)&value) != 0) return -1;
 if DEE_UNLIKELY((hProcess = OpenProcess(PROCESS_SET_INFORMATION,FALSE,pid)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "OpenProcess(PROCESS_SET_INFORMATION,FALSE,%lu) : %K",
                      pid,DeeSystemError_Win32ToString(GetLastError()));
  return -1;
 }
 if DEE_UNLIKELY(!SetPriorityClass(hProcess,value)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "SetProcessPriority(%p,%lu) : %K",hProcess,value,
                      DeeSystemError_Win32ToString(GetLastError()));
  CloseHandle(hProcess);
  return -1;
 }
 CloseHandle(hProcess);
 *ws = s;
 return 0;
}
static struct _DeeVFSFileTypeData _deevfs_procpidnodew32_priorityclass_vnt_file = {
 sizeof(struct DeeVFSFile),
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_openmode_t,Dee_mode_t))           &_deevfs_genericfile_openreadwrite,
 (void(DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *,void *,Dee_size_t,Dee_size_t *))      &_deevfs_procpidnodew32_priorityclass_vft_read,
 (int (DEE_CALL *)(struct DeeVFSFile *,void const *,Dee_size_t,Dee_size_t *))&_deevfs_procpidnodew32_priorityclass_vft_write,
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_int64_t,int,Dee_uint64_t *))      NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
};
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_w32_priorityclass = {
 {NULL,NULL},NULL,&_deevfs_procpidnodew32_priorityclass_vnt_file,NULL};
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_PRIORITYCLASS */
//////////////////////////////////////////////////////////////////////////
#if DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_TERMINATE
static int DEE_CALL _deevfs_procpidnodew32_terminate_vft_write(
 struct DeeVFSFile *self, void const *p, Dee_size_t s, Dee_size_t *ws) {
 UINT exitcode; char const *begin,*end; DeeObject *proc; DWORD pid; HANDLE hProcess;
 end = (begin = (char const *)p)+s;
 while (begin != end && DEE_CH_IS_SPACE(begin[0])) ++begin;
 while (begin != end && DEE_CH_IS_SPACE(end[-1])) --end;
 if (begin == end) exitcode = 0; else if (DeeString_ToNumberWithLength(
  UINT,(Dee_size_t)(end-begin),begin,(Dee_uint32_t *)&exitcode) != 0) return -1;
 DEE_ASSERT(self->vf_node->vn_type == &DeeVFSProcPIDNode_Type_w32_terminate);
 DEE_ASSERT(self->vf_node->vn_parent && self->vf_node->vn_parent->vn_type == &DeeVFSProcPIDNode_Type_w32);
 DEE_ASSERT(self->vf_node->vn_parent->vn_parent && self->vf_node->vn_parent->vn_parent->vn_type == &DeeVFSProcPIDNode_Type);
 proc = (DeeObject *)((struct DeeVFSProcPIDNode *)self->vf_node->vn_parent->vn_parent)->vpn_proc;
 pid = DeeProcess_ID(proc);
#if !DEE_CONFIG_RUNTIME_HAVE_EXIT
 if (pid == GetCurrentProcessId()) {
  DeeError_SET_STRING(&DeeErrorType_SystemError,
                      "Can't terminate current process");
  return -1;
 }
#endif /* !DEE_CONFIG_RUNTIME_HAVE_EXIT */
 if DEE_UNLIKELY((hProcess = OpenProcess(PROCESS_TERMINATE,FALSE,pid)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "OpenProcess(PROCESS_TERMINATE,FALSE,%lu) : %K",
                      pid,DeeSystemError_Win32ToString(GetLastError()));
  return -1;
 }
 if DEE_UNLIKELY(!TerminateProcess(hProcess,exitcode)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "TerminateProcess(%p,%u) : %K",hProcess,exitcode,
                      DeeSystemError_Win32ToString(GetLastError()));
  CloseHandle(hProcess);
  return -1;
 }
 *ws = s;
 CloseHandle(hProcess);
 return 0;
}
static struct _DeeVFSFileTypeData _deevfs_procpidnodew32_terminate_vnt_file = {
 sizeof(struct DeeVFSFile),
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_openmode_t,Dee_mode_t))           &_deevfs_genericfile_openwriteonly,
 (void(DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *,void *,Dee_size_t,Dee_size_t *))      NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *,void const *,Dee_size_t,Dee_size_t *))&_deevfs_procpidnodew32_terminate_vft_write,
 (int (DEE_CALL *)(struct DeeVFSFile *,Dee_int64_t,int,Dee_uint64_t *))      NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
 (int (DEE_CALL *)(struct DeeVFSFile *))                                     NULL,
};
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type_w32_terminate = {
 {NULL,NULL},NULL,&_deevfs_procpidnodew32_terminate_vnt_file,NULL};
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32_TERMINATE */
//////////////////////////////////////////////////////////////////////////
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID_W32 */
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_PID */











#if DEE_VFSCONFIG_HAVEFILE_PROC_SELF
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
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC_SELF */


DEE_DECL_END
#endif /* DEE_VFSCONFIG_HAVEFILE_PROC */
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#endif /* !GUARD_DEEMON_VFS_VFS_PROC_NODE_C_INL */
