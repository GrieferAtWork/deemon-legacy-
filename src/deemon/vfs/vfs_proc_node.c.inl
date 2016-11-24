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

#if DEE_CONFIG_RUNTIME_HAVE_VFS2
#include <deemon/vfs/vfs_core.h>
#include <deemon/vfs/vfs_proc_node.h>
#include <deemon/vfs/vfs_root.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN



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



//////////////////////////////////////////////////////////////////////////
// +++ DeeVFSProcNode_Type +++
struct DeeVFSVirtualDirEntry const DeeVFSProc_VirtualEntries[] = {
 {"self",DeeVFS_ProcSelf},
 {NULL,NULL},
};
struct DeeVFSNode _DeeVFS_Proc = DeeVFSNode_INIT(&DeeVFSProcNode_Type,DeeVFS_Root);

static struct DeeVFSNode *DEE_CALL _deevfs_procnode_vnt_walk(
 struct DeeVFSNode *DEE_UNUSED(self), char const *name) {
 DWORD pid = 0; char const *iter = name;
 struct DeeVFSVirtualDirEntry const *eiter;
 while (*iter) {
  if (*iter < '0' || *iter > '9') goto nopid;
  pid = (pid*10)+(*iter-'0');
  ++iter;
 }
 return (struct DeeVFSNode *)DeeVFSProcPIDNode_NewFromPID(pid);
nopid:
 for (eiter = DeeVFSProc_VirtualEntries; eiter->name; ++eiter) {
  if (strcmp(eiter->name,name) == 0) { DeeVFSNode_INCREF(eiter->node); return eiter->node; }
 }
 DeeError_SetStringf(&DeeErrorType_SystemError,
                     "Failed to find %q in \"/proc\"",
                     name);
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
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,char const *))&_deevfs_procnode_vnt_walk,
 (DeeObject *(DEE_CALL *)(struct DeeVFSNode *,struct DeeVFSNode *)) &_deevfs_procnode_vnt_nameof,
 (int (DEE_CALL *)(struct DeeVFSView *))                            &_deevfs_procview_vvt_open,
 (void (DEE_CALL *)(struct DeeVFSView *))                           &_deevfs_procview_vvt_quit,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))       &_deevfs_procview_vvt_curr,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))       &_deevfs_procview_vvt_yield,
};
struct DeeVFSNodeType const DeeVFSProcNode_Type = {
 {NULL,NULL},NULL,DeeVFSNoopNodeType_FileData,
 &_deevfs_procnode_vnt_view};
// --- DeeVFSProcNode_Type ---
//////////////////////////////////////////////////////////////////////////








//////////////////////////////////////////////////////////////////////////
// +++ DeeVFSProcPIDNode_Type +++
static void DEE_CALL _deevfs_procpidnode_quit(struct DeeVFSProcPIDNode *self) {
 Dee_DECREF(self->vpn_proc);
}
static struct _DeeVFSViewTypeData _deevfs_procpidnode_vnt_view = {
 sizeof(struct DeeVFSVirtualDirView),
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,char const *))NULL, // TODO
 (DeeObject *(DEE_CALL *)(struct DeeVFSNode *,struct DeeVFSNode *)) NULL, // TODO
 (int (DEE_CALL *)(struct DeeVFSView *))                            NULL, // TODO
 (void (DEE_CALL *)(struct DeeVFSView *))                           NULL, // TODO
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))       NULL, // TODO
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))       NULL, // TODO
};
struct DeeVFSNodeType const DeeVFSProcPIDNode_Type = {
 {(void (DEE_CALL *)(struct DeeVFSNode *))&_deevfs_procpidnode_quit,NULL},
 NULL,DeeVFSNoopNodeType_FileData,&_deevfs_procpidnode_vnt_view};
// --- DeeVFSProcPIDNode_Type ---
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
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */

#endif /* !GUARD_DEEMON_VFS_VFS_PROC_NODE_C_INL */
