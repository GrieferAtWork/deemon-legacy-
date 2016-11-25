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
#ifndef GUARD_DEEMON_VFS_VFS_NATIVE_NETMOUNT_C_INL
#define GUARD_DEEMON_VFS_VFS_NATIVE_NETMOUNT_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#if DEE_CONFIG_RUNTIME_HAVE_VFS
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/vfs/vfs_core.h>
#include <deemon/vfs/vfs_native_node.h>
#include <deemon/vfs/vfs_native_netmount.h>

DEE_DECL_BEGIN

typedef NET_API_STATUS (NET_API_FUNCTION *LPNETSERVERENUM)(
 LMCSTR servername, DWORD level, LPBYTE *bufptr, DWORD prefmaxlen, LPDWORD entriesread,
 LPDWORD totalentries, DWORD servertype, LMCSTR domain, LPDWORD resume_handle);
typedef NET_API_STATUS (NET_API_FUNCTION *LPNETSHAREENUM)(
 LMSTR servername, DWORD level, LPBYTE *bufptr, DWORD prefmaxlen, LPDWORD entriesread,
 LPDWORD totalentries, LPDWORD resume_handle);

typedef NET_API_STATUS (NET_API_FUNCTION *LPNETAPIBUFFERFREE)(LPVOID Buffer);


static LPNETSERVERENUM PNetServerEnum = NULL;
static LPNETSHAREENUM PNetShareEnum = NULL;
static LPNETAPIBUFFERFREE PNetApiBufferFree = NULL;
static struct DeeAtomicOnceFlag PNetApiBufferFree_loaded = DeeAtomicOnceFlag_INIT();
#define PNetApiBufferFree_LOAD()\
 DeeAtomicOnceFlag_RUN(&PNetApiBufferFree_loaded,{\
  *(void **)&PNetApiBufferFree = DeeSharedLib_Utf8PoolTryImport("Netapi32.dll","NetApiBufferFree");\
 })






static void DEE_CALL _deevfs_netservernode_vnt_quit(struct DeeVFSNetServerNode *self) {
 Dee_DECREF(self->vnm_name);
}
static struct DeeVFSNativeNode *DEE_CALL _deevfs_netservernode_vnt_walk(
 struct DeeVFSNetServerNode *self, char const *name) {
 struct DeeVFSNativeNode *result;
 if DEE_UNLIKELY((result = DeeVFSNode_ALLOC(struct DeeVFSNativeNode)) == NULL) return NULL;
 // Server-share path
 result->vnn_path = (DeeStringObject *)DeeString_Newf("\\\\%k\\%s",self->vnm_name,name);
 if (!result->vnn_path) { DeeVFSNode_FREE(result); return NULL; }
 DeeVFSNode_InitWithParent(&result->vnn_node,&DeeVFSNativeNode_Type,&self->vnm_node);
 return result;
}
static int DEE_CALL _deevfs_netserverview_vvt_open(
 struct DeeVFSNetServerView *self) {
 DeeObject *servername; NET_API_STATUS nStatus;
 DWORD dwEntriesRead = 0,dwTotalEntries = 0;
 DEE_ATOMIC_ONCE({
  *(void **)&PNetShareEnum = DeeSharedLib_Utf8PoolTryImport("Netapi32.dll","NetShareEnum");
 });
 if (!PNetShareEnum) { DeeError_NotImplemented_str("NetShareEnum"); return -1; }
 // Retrive the server name
 servername = (DeeObject *)((struct DeeVFSNetServerNode *)self->nsv_view.vv_node)->vnm_name;
 DEE_ASSERT(DeeObject_Check(servername) && DeeString_Check(servername));
 servername = DeeWideString_FromUtf8StringWithLength(DeeUtf8String_SIZE(servername),
                                                     DeeUtf8String_STR(servername));
 if (!servername) return -1;
 self->nsv_pBegin = NULL,self->nsv_dwResumeHandle = 0;
 // Begin enumerating
 nStatus = (*PNetShareEnum)(DeeWideString_STR(servername),502,(LPBYTE *)&self->nsv_pBegin,
                            MAX_PREFERRED_LENGTH,&dwEntriesRead,&dwTotalEntries,&self->nsv_dwResumeHandle);
// nStatus = NetShareEnum(lpszServer,502,(LPBYTE *)&BufPtr,MAX_PREFERRED_LENGTH,&dwEntriesRead,&dwTotalEntries,&resume);
 self->nsv_server = (DeeWideStringObject *)servername;
 if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)) {
  if (!self->nsv_pBegin) { DeeError_SetNone(&DeeErrorType_SegFault); goto err_servername; }
  self->nsv_pBegin = self->nsv_pBegin;
  self->nsv_pEnd = self->nsv_pBegin+dwEntriesRead;
  self->nsv_pCurr = self->nsv_pBegin;
  self->nsv_hasresume = nStatus == ERROR_MORE_DATA;
 } else {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "NetShareEnum(%r,502,...,MAX_PREFERRED_LENGTH,...,...,...) : %K",
                      servername,DeeSystemError_Win32ToString(nStatus));
err_servername:
  Dee_DECREF(servername);
  return -1;
 }
 DeeAtomicMutex_Init(&self->nsv_lock);
 return 0;
}
static void DEE_CALL _deevfs_netserverview_vvt_quit(
 struct DeeVFSNetServerView *self) {
 PNetApiBufferFree_LOAD();
 if (self->nsv_pBegin && PNetApiBufferFree) (*PNetApiBufferFree)(self->nsv_pBegin);
 Dee_DECREF(self->nsv_server);
}


static int _DeeVFSNetServerView_GetInfoLocked(
 struct DeeVFSNetServerView *self, PSHARE_INFO_502 *result) {
 PSHARE_INFO_502 result_info; NET_API_STATUS nStatus;
 DWORD dwEntriesRead = 0,dwTotalEntries = 0;
 result_info = self->nsv_pCurr;
 if (result_info < self->nsv_pBegin || result_info >= self->nsv_pEnd) {
  if (!self->nsv_hasresume) return 1; // No more data
  PNetApiBufferFree_LOAD();
  if (self->nsv_pBegin && PNetApiBufferFree) (*PNetApiBufferFree)(self->nsv_pBegin);
  self->nsv_pBegin = NULL;
  nStatus = (*PNetShareEnum)(DeeWideString_STR(self->nsv_server),502,(LPBYTE *)&self->nsv_pBegin,
                             MAX_PREFERRED_LENGTH,&dwEntriesRead,&dwTotalEntries,
                             &self->nsv_dwResumeHandle);
  if (!dwEntriesRead) {
   if (self->nsv_pBegin && PNetApiBufferFree) (*PNetApiBufferFree)(self->nsv_pBegin);
   self->nsv_dwResumeHandle = 0,self->nsv_hasresume = 0;
   self->nsv_pBegin = NULL,self->nsv_pCurr = NULL,self->nsv_pEnd = NULL;
   return 1; // End of data
  }
  if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)) {
   if (!self->nsv_pBegin) {
    DeeAtomicMutex_Release(&self->nsv_lock);
    DeeError_SetNone(&DeeErrorType_SegFault);
    goto on_err;
   }
   self->nsv_pBegin = self->nsv_pBegin;
   self->nsv_pEnd = self->nsv_pBegin+dwEntriesRead;
   self->nsv_pCurr = self->nsv_pBegin;
   self->nsv_hasresume = nStatus == ERROR_MORE_DATA;
  } else {
   DeeAtomicMutex_Release(&self->nsv_lock);
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "NetServerEnum(NULL,101,...,MAX_PREFERRED_LENGTH,...,...,SV_TYPE_SERVER,NULL,...) : %K",
                       DeeSystemError_Win32ToString(nStatus));
on_err:
   self->nsv_dwResumeHandle = 0;
   self->nsv_hasresume = 0;
   self->nsv_pBegin = NULL;
   self->nsv_pCurr = NULL;
   self->nsv_pEnd = NULL;
   return -1;
  }
  result_info = self->nsv_pCurr;
 }
 *result = result_info;
 return 0;
}
static int DEE_CALL DeeVFSNetServerView_GetCurrentNode(
 struct DeeVFSNetServerView *self, struct DeeVFSNativeNode **result, int advance) {
 PSHARE_INFO_502 my_info; int error;
 Dee_WideChar *wname; Dee_size_t wname_size;
again:
 DeeAtomicMutex_AcquireRelaxed(&self->nsv_lock);
 if ((error = _DeeVFSNetServerView_GetInfoLocked(self,&my_info)) != 0) {
  if (error > 0) DeeAtomicMutex_Release(&self->nsv_lock);
  return error;
 }
 wname_size = Dee_WideStrLen(my_info->shi502_netname);
 if DEE_UNLIKELY(!wname_size) { // Shouldn't happen
  if (advance) ++self->nsv_pCurr;
  DeeAtomicMutex_Release(&self->nsv_lock);
  if ((*result = DeeVFSNode_ALLOC(struct DeeVFSNativeNode)) == NULL) return -1;
  (*result)->vnn_path = (DeeStringObject *)DeeString_Newf(
   "\\\\%k\\" /* shrugs... */ "\\",self->nsv_server);
  if DEE_UNLIKELY(!(*result)->vnn_path) goto err_rnode;
  return 0;
 }
 if ((wname = (Dee_WideChar *)malloc_nz(wname_size*sizeof(Dee_WideChar))) == NULL) {
  DeeAtomicMutex_Release(&self->nsv_lock);
  if (Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return -1;
 }
 memcpy(wname,my_info->shi502_netname,
        wname_size*sizeof(Dee_WideChar));
 if (advance) ++self->nsv_pCurr;
 DeeAtomicMutex_Release(&self->nsv_lock);
 if ((*result = DeeVFSNode_ALLOC(struct DeeVFSNativeNode)) == NULL) { free_nn(wname); return -1; }
 (*result)->vnn_path = (DeeStringObject *)DeeString_Newf("\\\\%k\\%.*ls",
                                                         self->nsv_server,
                                                         (unsigned)wname_size,wname);
 free_nn(wname);
 if (!(*result)->vnn_path) {err_rnode: DeeVFSNode_FREE(*result); return -1; }
 DeeVFSNode_InitWithParent(&(*result)->vnn_node,
                           &DeeVFSNativeNode_Type,
                           self->nsv_view.vv_node);
 return 0;
}

static int DEE_CALL _deevfs_netserverview_vvt_curr(
 struct DeeVFSNetServerView *self, struct DeeVFSNativeNode **result) {
 return DeeVFSNetServerView_GetCurrentNode(self,result,0);
}
static int DEE_CALL _deevfs_netserverview_vvt_yield(
 struct DeeVFSNetServerView *self, struct DeeVFSNativeNode **result) {
 return DeeVFSNetServerView_GetCurrentNode(self,result,1);
}
static struct _DeeVFSViewTypeData _deevfs_netserver_vnt_view = {
 sizeof(struct DeeVFSNetServerView),
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,char const *))&_deevfs_netservernode_vnt_walk,
 (DeeObject *(DEE_CALL *)(struct DeeVFSNode *,struct DeeVFSNode *)) &_deevfs_nativenode_vnt_nameof,
 (int (DEE_CALL *)(struct DeeVFSView *))                            &_deevfs_netserverview_vvt_open,
 (void (DEE_CALL *)(struct DeeVFSView *))                           &_deevfs_netserverview_vvt_quit,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))       &_deevfs_netserverview_vvt_curr,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))       &_deevfs_netserverview_vvt_yield,
};

struct DeeVFSNodeType const DeeVFSNetServerNode_Type = {
 {(void(DEE_CALL*)(struct DeeVFSNode *))&_deevfs_netservernode_vnt_quit,NULL},
 NULL,DeeVFSNoopNodeType_FileData,&_deevfs_netserver_vnt_view};















static struct DeeVFSNetServerNode *DEE_CALL _deevfs_netmountnode_vnt_walk(
 struct DeeVFSNode *self, char const *name) {
 struct DeeVFSNetServerNode *result;
 if DEE_UNLIKELY((result = DeeVFSNode_ALLOC(struct DeeVFSNetServerNode)) == NULL) return NULL;
 if DEE_UNLIKELY((result->vnm_name = (DeeStringObject *)DeeString_New(name)) == NULL) { DeeVFSNode_FREE(result); return NULL; }
 DeeVFSNode_InitWithParent(&result->vnm_node,&DeeVFSNetServerNode_Type,self);
 return result;
}
static DeeObject *DEE_CALL _deevfs_netmountnode_vnt_nameof(
 struct DeeVFSNode *DEE_UNUSED(self), struct DeeVFSNetServerNode *child) {
 DEE_ASSERT(child->vnm_node.vn_type == &DeeVFSNetServerNode_Type);
 DeeReturn_NEWREF((DeeObject *)child->vnm_name);
}


static int DEE_CALL _deevfs_netmountview_vvt_open(
 struct DeeVFSNetMountView *self) {
 NET_API_STATUS nStatus;
 DWORD dwEntriesRead = 0,dwTotalEntries = 0;
 DEE_ATOMIC_ONCE({
  *(void **)&PNetServerEnum = DeeSharedLib_Utf8PoolTryImport("Netapi32.dll","NetServerEnum");
 });
 if (!PNetServerEnum) { DeeError_NotImplemented_str("NetServerEnum"); return -1; }
 self->nmv_pBegin = NULL,self->nmv_dwResumeHandle = 0;
 nStatus = (*PNetServerEnum)(NULL,101,(LPBYTE *)&self->nmv_pBegin,MAX_PREFERRED_LENGTH,
                             &dwEntriesRead,&dwTotalEntries,SV_TYPE_SERVER,NULL,
                             &self->nmv_dwResumeHandle);
 if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)) {
  if (!self->nmv_pBegin) { DeeError_SetNone(&DeeErrorType_SegFault); return -1; }
  self->nmv_pBegin = self->nmv_pBegin;
  self->nmv_pEnd = self->nmv_pBegin+dwEntriesRead;
  self->nmv_pCurr = self->nmv_pBegin;
  self->nmv_hasresume = nStatus == ERROR_MORE_DATA;
 } else {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "NetServerEnum(NULL,101,...,MAX_PREFERRED_LENGTH,...,...,SV_TYPE_SERVER,NULL,...) : %K",
                      DeeSystemError_Win32ToString(nStatus));
  return -1;
 }
 DeeAtomicMutex_Init(&self->nmv_lock);
 return 0;
}


static void DEE_CALL _deevfs_netmountview_vvt_quit(struct DeeVFSNetMountView *self) {
 PNetApiBufferFree_LOAD();
 if (self->nmv_pBegin && PNetApiBufferFree) (*PNetApiBufferFree)(self->nmv_pBegin);
}

static int _DeeVFSNetMountView_GetInfoLocked(
 struct DeeVFSNetMountView *self, LPSERVER_INFO_101 *result) {
 LPSERVER_INFO_101 result_info; NET_API_STATUS nStatus;
 DWORD dwEntriesRead = 0,dwTotalEntries = 0;
 result_info = self->nmv_pCurr;
 if (result_info < self->nmv_pBegin || result_info >= self->nmv_pEnd) {
  if (!self->nmv_hasresume) return 1; // No more data
  PNetApiBufferFree_LOAD();
  if (self->nmv_pBegin && PNetApiBufferFree) (*PNetApiBufferFree)(self->nmv_pBegin);
  self->nmv_pBegin = NULL;
  nStatus = (*PNetServerEnum)(NULL,101,(LPBYTE *)&self->nmv_pBegin,MAX_PREFERRED_LENGTH,
                              &dwEntriesRead,&dwTotalEntries,SV_TYPE_SERVER,NULL,
                              &self->nmv_dwResumeHandle);
  if (!dwEntriesRead) {
   if (self->nmv_pBegin && PNetApiBufferFree) (*PNetApiBufferFree)(self->nmv_pBegin);
   self->nmv_dwResumeHandle = 0,self->nmv_hasresume = 0;
   self->nmv_pBegin = NULL,self->nmv_pCurr = NULL,self->nmv_pEnd = NULL;
   return 1; // End of data
  }
  if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA)) {
   if (!self->nmv_pBegin) {
    DeeAtomicMutex_Release(&self->nmv_lock);
    DeeError_SetNone(&DeeErrorType_SegFault);
    goto on_err;
   }
   self->nmv_pBegin = self->nmv_pBegin;
   self->nmv_pEnd = self->nmv_pBegin+dwEntriesRead;
   self->nmv_pCurr = self->nmv_pBegin;
   self->nmv_hasresume = nStatus == ERROR_MORE_DATA;
  } else {
   DeeAtomicMutex_Release(&self->nmv_lock);
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "NetServerEnum(NULL,101,...,MAX_PREFERRED_LENGTH,...,...,SV_TYPE_SERVER,NULL,...) : %K",
                       DeeSystemError_Win32ToString(nStatus));
on_err:
   self->nmv_dwResumeHandle = 0;
   self->nmv_hasresume = 0;
   self->nmv_pBegin = NULL;
   self->nmv_pCurr = NULL;
   self->nmv_pEnd = NULL;
   return -1;
  }
  result_info = self->nmv_pCurr;
 }
 *result = result_info;
 return 0;
}
static int DEE_CALL DeeVFSNetMountView_GetCurrentNode(
 struct DeeVFSNetMountView *self, struct DeeVFSNetServerNode **result, int advance) {
 LPSERVER_INFO_101 my_info; int error;
 Dee_WideChar *wname; Dee_size_t wname_size;
again:
 DeeAtomicMutex_AcquireRelaxed(&self->nmv_lock);
 if ((error = _DeeVFSNetMountView_GetInfoLocked(self,&my_info)) != 0) {
  if (error > 0) DeeAtomicMutex_Release(&self->nmv_lock);
  return error;
 }
 wname_size = Dee_WideStrLen(my_info->sv101_name);
 if DEE_UNLIKELY(!wname_size) { // Shouldn't happen
  if (advance) ++self->nmv_pCurr;
  DeeAtomicMutex_Release(&self->nmv_lock);
  if ((*result = DeeVFSNode_ALLOC(struct DeeVFSNetServerNode)) == NULL) return -1;
  Dee_INCREF((*result)->vnm_name = (DeeStringObject *)Dee_EmptyString);
  return 0;
 }
 if ((wname = (Dee_WideChar *)malloc_nz(wname_size*sizeof(Dee_WideChar))) == NULL) {
  DeeAtomicMutex_Release(&self->nmv_lock);
  if (Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return -1;
 }
 memcpy(wname,my_info->sv101_name,
        wname_size*sizeof(Dee_WideChar));
 if (advance) ++self->nmv_pCurr;
 DeeAtomicMutex_Release(&self->nmv_lock);
 if ((*result = DeeVFSNode_ALLOC(struct DeeVFSNetServerNode)) == NULL) { free_nn(wname); return -1; }
 (*result)->vnm_name = (DeeStringObject *)DeeString_FromWideStringWithLength(wname_size,wname);
 free_nn(wname);
 if (!(*result)->vnm_name) { DeeVFSNode_FREE(*result); return -1; }
 DeeVFSNode_InitWithParent(&(*result)->vnm_node,
                           &DeeVFSNetServerNode_Type,
                           self->nmv_view.vv_node);
 return 0;
}

static int DEE_CALL _deevfs_netmountview_vvt_curr(
 struct DeeVFSNetMountView *self, struct DeeVFSNetServerNode **result) {
 return DeeVFSNetMountView_GetCurrentNode(self,result,0);
}
static int DEE_CALL _deevfs_netmountview_vvt_yield(
 struct DeeVFSNetMountView *self, struct DeeVFSNetServerNode **result) {
 return DeeVFSNetMountView_GetCurrentNode(self,result,1);
}


static struct _DeeVFSViewTypeData _deevfs_netmount_vnt_view = {
 sizeof(struct DeeVFSNetMountView),
 (struct DeeVFSNode *(DEE_CALL *)(struct DeeVFSNode *,char const *))&_deevfs_netmountnode_vnt_walk,
 (DeeObject *(DEE_CALL *)(struct DeeVFSNode *,struct DeeVFSNode *)) &_deevfs_netmountnode_vnt_nameof,
 (int (DEE_CALL *)(struct DeeVFSView *))                            &_deevfs_netmountview_vvt_open,
 (void (DEE_CALL *)(struct DeeVFSView *))                           &_deevfs_netmountview_vvt_quit,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))       &_deevfs_netmountview_vvt_curr,
 (int (DEE_CALL *)(struct DeeVFSView *,struct DeeVFSNode **))       &_deevfs_netmountview_vvt_yield,
};

struct DeeVFSNodeType const DeeVFSNetMountNode_Type = {
 {NULL,NULL},NULL,DeeVFSNoopNodeType_FileData,
 &_deevfs_netmount_vnt_view};


DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#endif /* !GUARD_DEEMON_VFS_VFS_NATIVE_NETMOUNT_C_INL */
