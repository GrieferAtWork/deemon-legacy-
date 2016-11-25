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
#ifndef GUARD_DEEMON_VFS_VFS_CORE_C
#define GUARD_DEEMON_VFS_VFS_CORE_C 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/vfs/vfs_core.h>
#include <deemon/vfs/vfs_native_node.h>
#include <deemon/vfs/vfs_root.h>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/deemonrun.h>
#include <deemon/fs/native_fs.h>

#if DEE_CONFIG_RUNTIME_HAVE_VFS
DEE_DECL_BEGIN


void _DeeVFSNode_Delete(DEE_A_IN struct DeeVFSNode *self) {
 void (DEE_CALL *quit_func)(struct DeeVFSNode *);
 if ((quit_func = self->vn_type->vnt_node.vnt_quit) != NULL) (*quit_func)(self);
 if (self->vn_parent) DeeVFSNode_DECREF(self->vn_parent);
 DeeVFSNode_FREE(self);
}



DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeVFSNode_Name(DEE_A_IN struct DeeVFSNode const *node) {
 struct _DeeVFSViewTypeData *viewtype;
 DeeObject *(DEE_CALL *nameofproc)(struct DeeVFSNode *,struct DeeVFSNode *);
 if DEE_UNLIKELY(!node->vn_parent) DeeReturn_STATIC_STRING("/");
 viewtype = node->vn_parent->vn_type->vnt_view;
 if DEE_UNLIKELY(!viewtype || (nameofproc = viewtype->vnt_nameof) == NULL) DeeReturn_STATIC_STRING("?");
 return (*nameofproc)(node->vn_parent,(struct DeeVFSNode *)node);
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeVFSNode_HasHiddenFilename(
 DEE_A_IN struct DeeVFSNode const *self) {
 DeeObject *node_name; int result; struct _DeeVFSViewTypeData *viewtype;
 DeeObject *(DEE_CALL *nameofproc)(struct DeeVFSNode *,struct DeeVFSNode *);
 DEE_ASSERT(self);
 if DEE_UNLIKELY(!self->vn_parent) return 0;
 viewtype = self->vn_parent->vn_type->vnt_view;
 if DEE_UNLIKELY(!viewtype || (nameofproc = viewtype->vnt_nameof) == NULL) return 0;
 node_name = (*nameofproc)(self->vn_parent,(struct DeeVFSNode *)self);
 if DEE_UNLIKELY(!node_name) return -1;
 if (DeeUtf8String_Check(node_name)) {
 DEE_ASSERT(DeeObject_Check(node_name) && (
  DeeUtf8String_Check(node_name) ||
  DeeWideString_Check(node_name)));
 }
 result = DeeUtf8String_Check(node_name)
  ? DeeUtf8String_STR(node_name)[0] == '.'
  : DeeWideString_STR(node_name)[0] == '.';
 Dee_DECREF(node_name);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeVFSNode_GetTimes(
 DEE_A_INOUT struct DeeVFSNode *self, DEE_A_OUT_OPT Dee_timetick_t *atime,
 DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 struct _DeeVFSPropTypeData *proptype; DEE_ASSERT(self);
 proptype = self->vn_type->vnt_prop;
 if (!proptype || !proptype->vnt_gettimes) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Can't read time for %R",
                      DeeVFSNode_Filename(self));
  return -1;
 }
 return (*proptype->vnt_gettimes)(self,atime,ctime,mtime);
}
DEE_A_RET_EXCEPT(-1) int DeeVFSNode_SetTimes(
      DEE_A_INOUT struct DeeVFSNode *self, DEE_A_IN_OPT Dee_timetick_t const *atime,
 DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime) {
 struct _DeeVFSPropTypeData *proptype; DEE_ASSERT(self);
 proptype = self->vn_type->vnt_prop;
 if (!proptype || !proptype->vnt_settimes) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Can't write time for %R",
                      DeeVFSNode_Filename(self));
  return -1;
 }
 return (*proptype->vnt_settimes)(self,atime,ctime,mtime);
}
DEE_A_RET_EXCEPT(-1) int DeeVFSNode_GetMod(
 DEE_A_INOUT struct DeeVFSNode *self, DEE_A_OUT Dee_mode_t *mode) {
 int error; struct _DeeVFSPropTypeData *proptype;
 struct _DeeVFSFileTypeData *filetype;
 DEE_ASSERT(self); DEE_ASSERT(mode);
 proptype = self->vn_type->vnt_prop;
 if (proptype && proptype->vnt_getmod)
  return (*proptype->vnt_getmod)(self,mode);
 filetype = self->vn_type->vnt_file;
 *mode = (Dee_mode_t)((filetype && filetype->vft_open && filetype->vft_write) ? 0222 : 0000);
 if (proptype && proptype->vnt_isexecutable) {
  error = (*proptype->vnt_isexecutable)(self);
  if (error < 0) return error;
  if (error) *mode |= 0111;
 } else if (DeeVFSNode_HasView(self)) {
  *mode |= 0111;
 }
 if (DeeVFSNode_HasOpen(self)) *mode |= 0444;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeVFSNode_Chmod(
 DEE_A_INOUT struct DeeVFSNode *self, DEE_A_IN Dee_mode_t mode) {
 struct _DeeVFSPropTypeData *proptype; DEE_ASSERT(self);
 proptype = self->vn_type->vnt_prop;
 if (!proptype || !proptype->vnt_chmod) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Can't chmod %R",
                      DeeVFSNode_Filename(self));
  return -1;
 }
 return (*proptype->vnt_chmod)(self,mode);
}
DEE_A_RET_EXCEPT(-1) int DeeVFSNode_GetOwn(
 DEE_A_INOUT struct DeeVFSNode *self,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 struct _DeeVFSPropTypeData *proptype; DEE_ASSERT(self);
 proptype = self->vn_type->vnt_prop;
 if (!proptype || !proptype->vnt_getown) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Can't getown %R",
                      DeeVFSNode_Filename(self));
  return -1;
 }
 return (*proptype->vnt_getown)(self,owner,group);
}
DEE_A_RET_EXCEPT(-1) int DeeVFSNode_Chown(
 DEE_A_INOUT struct DeeVFSNode *self,
 DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
 struct _DeeVFSPropTypeData *proptype; DEE_ASSERT(self);
 proptype = self->vn_type->vnt_prop;
 if (!proptype || !proptype->vnt_chown) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Can't chown %R",
                      DeeVFSNode_Filename(self));
  return -1;
 }
 return (*proptype->vnt_chown)(self,owner,group);
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeVFSNode_Utf8Readlink(DEE_A_INOUT struct DeeVFSNode *self) {
 if (!self->vn_type->vnt_node.vnt_readlink) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Can't readlink %R",
                      DeeVFSNode_Filename(self));
  return NULL;
 }
 return (*self->vn_type->vnt_node.vnt_readlink)(self);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeVFSNode_WideReadlink(DEE_A_INOUT struct DeeVFSNode *self) {
 DeeObject *result,*newresult;
 if DEE_UNLIKELY((result = DeeVFSNode_Utf8Readlink(self)) == NULL) return NULL;
 newresult = DeeWideString_FromUtf8StringWithLength(DeeUtf8String_SIZE(result),
                                                    DeeUtf8String_STR(result));
 Dee_DECREF(result);
 return newresult;
}




DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFSNode_WalkLink_impl(
 DEE_A_INOUT struct DeeVFSLocateState *state, DEE_A_INOUT struct DeeVFSNode *self) {
 struct DeeVFSNode *result; DeeObject *link_path;
 if DEE_UNLIKELY((link_path = DeeVFSNode_DoReadlink(self)) == NULL) return NULL;
 DEE_ASSERT(DeeObject_Check(link_path) && (DeeUtf8String_Check(link_path) || DeeWideString_Check(link_path)));
 if (DeeUtf8String_Check(link_path)) {
  Dee_Utf8Char const *link_str = DeeUtf8String_STR(link_path);
  if (DEE_VFS_ISSEP(link_str[0])) { // Absolute link
   do ++link_str; while (DEE_VFS_ISSEP(link_str[0]));
   result = DeeVFS_Utf8LLocateAt_impl(state,DeeVFS_Root,link_str);
  } else if (DeeVFS_Utf8IsAbsoluteNativePath(link_str)) {
   // Absolute path within the native filesystem
   if DEE_LIKELY((result = (struct DeeVFSNode *)DeeVFSNode_ALLOC(struct DeeVFSNativeNode)) != NULL) {
    // NOTE: We initialize the node's parent as ourselves, which though technically
    //       not being correct, is much faster than trying to find the real native node.
    DeeVFSNode_InitWithParent(result,&DeeVFSNativeNode_Type,self);
    ((struct DeeVFSNativeNode *)result)->vnn_path = (DeeAnyStringObject *)link_path; // Inherit reference
    return result; // Skip the decref on link_path (we inherited that reference)
   }
  } else { // Relative link
   DEE_ASSERTF(self->vn_parent,"Node %R without parent is a link",
               DeeVFSNode_Filename(self));
   result = DeeVFS_Utf8LLocateAt_impl(state,self->vn_parent,link_str);
  }
 } else {
  Dee_WideChar const *link_str = DeeWideString_STR(link_path);
  if (DEE_VFS_ISSEP(link_str[0])) { // Absolute link
   do ++link_str; while (DEE_VFS_ISSEP(link_str[0]));
   result = DeeVFS_WideLLocateAt_impl(state,DeeVFS_Root,link_str);
  } else if (DeeVFS_WideIsAbsoluteNativePath(link_str)) {
   // Absolute path within the native filesystem
   if DEE_LIKELY((result = (struct DeeVFSNode *)DeeVFSNode_ALLOC(struct DeeVFSNativeNode)) != NULL) {
    // NOTE: We initialize the node's parent as ourselves, which though technically
    //       not being correct, is much faster than trying to find the real native node.
    DeeVFSNode_InitWithParent(result,&DeeVFSNativeNode_Type,self);
    ((struct DeeVFSNativeNode *)result)->vnn_path = (DeeAnyStringObject *)link_path; // Inherit reference
    return result; // Skip the decref on link_path (we inherited that reference)
   }
  } else { // Relative link
   DEE_ASSERTF(self->vn_parent,"Node %R without parent is a link",
               DeeVFSNode_Filename(self));
   result = DeeVFS_WideLLocateAt_impl(state,self->vn_parent,link_str);
  }
 }
 Dee_DECREF(link_path);
 return result;
}


static void _DeeVFSError_MaxLinkIndirectionReached(char const *path) {
 DeeError_SetStringf(&DeeErrorType_ValueError,
                     "Max level of link indirection reached: %q",path);
}


DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_Utf8LocateAt_impl(
 DEE_A_INOUT struct DeeVFSLocateState *state,
 DEE_A_IN struct DeeVFSNode *root, DEE_A_IN_Z char const *path) {
 struct DeeVFSNode *result,*newresult; int error;
 if DEE_UNLIKELY((result = DeeVFS_LLocateAt_impl(state,root,path)) == NULL) return NULL;
 while (1) {
  if DEE_UNLIKELY((error = DeeVFSNode_IsLink(result)) < 0) goto err_r;
  if (!error) break;
  if DEE_UNLIKELY(++state->vld_link_ind == DEE_VFS_MAX_LINK_INDIRECTION) {
   _DeeVFSError_MaxLinkIndirectionReached(state->vld_startpath);
err_r:
   DeeVFSNode_DECREF(result);
   return NULL;
  }
  newresult = DeeVFSNode_WalkLink_impl(state,result);
  DeeVFSNode_DECREF(result);
  if DEE_UNLIKELY(!newresult) return NULL;
  result = newresult;
 }
 return result;
}



DEE_A_RET_EXCEPT_REF struct DeeVFSNode *
DeeVFS_LLocate(DEE_A_IN_Z char const *path) {
 struct DeeVFSLocateState state;
 struct DeeVFSNode *result,*cwd;
 state.vld_startpath = path;
 state.vld_link_ind = 0;
 if (DEE_VFS_ISSEP(*path)) { // Absolute path
  do ++path; while (DEE_VFS_ISSEP(*path));
  return DeeVFS_LLocateAt_impl(&state,DeeVFS_Root,path);
 }
 if DEE_UNLIKELY((cwd = DeeVFS_GetCwdNode()) == NULL) return NULL;
 result = DeeVFS_LLocateAt_impl(&state,cwd,path);
 DeeVFSNode_DECREF(cwd);
 return result;
}
DEE_A_RET_EXCEPT_REF struct DeeVFSNode *
DeeVFS_Locate(DEE_A_IN_Z char const *path) {
 struct DeeVFSLocateState state;
 struct DeeVFSNode *result,*cwd;
 state.vld_startpath = path;
 state.vld_link_ind = 0;
 if (DEE_VFS_ISSEP(*path)) { // Absolute path
  do ++path; while (DEE_VFS_ISSEP(*path));
  return DeeVFS_LocateAt_impl(&state,DeeVFS_Root,path);
 }
 if DEE_UNLIKELY((cwd = DeeVFS_GetCwdNode()) == NULL) return NULL;
 result = DeeVFS_LocateAt_impl(&state,cwd,path);
 DeeVFSNode_DECREF(cwd);
 return result;
}
DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_LLocateWithCWD(
 DEE_A_IN struct DeeVFSNode *cwd, DEE_A_IN_Z char const *path) {
 struct DeeVFSLocateState state;
 DEE_ASSERT(cwd);
 state.vld_startpath = path;
 state.vld_link_ind = 0;
 if (DEE_VFS_ISSEP(*path)) { // Absolute path
  do ++path; while (DEE_VFS_ISSEP(*path));
  return DeeVFS_LLocateAt_impl(&state,DeeVFS_Root,path);
 }
 return DeeVFS_LLocateAt_impl(&state,cwd,path);
}
DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_LocateWithCWD(
 DEE_A_IN struct DeeVFSNode *cwd, DEE_A_IN_Z char const *path) {
 struct DeeVFSLocateState state;
 DEE_ASSERT(cwd);
 state.vld_startpath = path;
 state.vld_link_ind = 0;
 if (DEE_VFS_ISSEP(*path)) { // Absolute path
  do ++path; while (DEE_VFS_ISSEP(*path));
  return DeeVFS_LocateAt_impl(&state,DeeVFS_Root,path);
 }
 return DeeVFS_LocateAt_impl(&state,cwd,path);
}


DEE_A_RET_EXCEPT_REF struct DeeVFSNode *
DeeVFS_WideLLocate(DEE_A_IN_Z Dee_WideChar const *path) {
 DeeObject *u8path; struct DeeVFSNode *result;
 if DEE_UNLIKELY((u8path = DeeUtf8String_FromWideString(path)) == NULL) return NULL;
 result = DeeVFS_Utf8LLocate(DeeUtf8String_STR(u8path));
 Dee_DECREF(u8path);
 return result;
}
DEE_A_RET_EXCEPT_REF struct DeeVFSNode *
DeeVFS_WideLocate(DEE_A_IN_Z Dee_WideChar const *path) {
 DeeObject *u8path; struct DeeVFSNode *result;
 if DEE_UNLIKELY((u8path = DeeWideString_FromWideString(path)) == NULL) return NULL;
 result = DeeVFS_Utf8Locate(DeeUtf8String_STR(u8path));
 Dee_DECREF(u8path);
 return result;
}
DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_WideLLocateWithCWD(
 DEE_A_IN struct DeeVFSNode *cwd, DEE_A_IN_Z Dee_WideChar const *path) {
 DeeObject *u8path; struct DeeVFSNode *result;
 if DEE_UNLIKELY((u8path = DeeUtf8String_FromWideString(path)) == NULL) return NULL;
 result = DeeVFS_Utf8LLocateWithCWD(cwd,DeeUtf8String_STR(u8path));
 Dee_DECREF(u8path);
 return result;
}
DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_WideLocateWithCWD(
 DEE_A_IN struct DeeVFSNode *cwd, DEE_A_IN_Z Dee_WideChar const *path) {
 DeeObject *u8path; struct DeeVFSNode *result;
 if DEE_UNLIKELY((u8path = DeeUtf8String_FromWideString(path)) == NULL) return NULL;
 result = DeeVFS_Utf8LocateWithCWD(cwd,DeeUtf8String_STR(u8path));
 Dee_DECREF(u8path);
 return result;
}

DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_WideLLocateObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 struct DeeVFSNode *result; DeeObject *u8path;
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 if ((u8path = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return NULL;
 result = DeeVFS_Utf8LLocateObject(u8path);
 Dee_DECREF(u8path);
 return result;
}
DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_WideLocateObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 struct DeeVFSNode *result; DeeObject *u8path;
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 if ((u8path = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return NULL;
 result = DeeVFS_Utf8LocateObject(u8path);
 Dee_DECREF(u8path);
 return result;
}
DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_WideLLocateWithCWDObject(
 DEE_A_IN struct DeeVFSNode *cwd, DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 struct DeeVFSNode *result; DeeObject *u8path;
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 if ((u8path = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return NULL;
 result = DeeVFS_Utf8LLocateWithCWDObject(cwd,u8path);
 Dee_DECREF(u8path);
 return result;
}
DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_WideLocateWithCWDObject(
 DEE_A_IN struct DeeVFSNode *cwd, DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 struct DeeVFSNode *result; DeeObject *u8path;
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 if ((u8path = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return NULL;
 result = DeeVFS_Utf8LocateWithCWDObject(cwd,u8path);
 Dee_DECREF(u8path);
 return result;
}
DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_WideLLocateAt(
 DEE_A_IN struct DeeVFSNode *root, DEE_A_IN_Z Dee_WideChar const *path) {
 struct DeeVFSNode *result; DeeObject *u8path;
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 if ((u8path = DeeUtf8String_FromWideString(path)) == NULL) return NULL;
 result = DeeVFS_Utf8LLocateAtObject(root,u8path);
 Dee_DECREF(u8path);
 return result;
}
DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_WideLocateAt(
 DEE_A_IN struct DeeVFSNode *root, DEE_A_IN_Z Dee_WideChar const *path) {
 struct DeeVFSNode *result; DeeObject *u8path;
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 if ((u8path = DeeUtf8String_FromWideString(path)) == NULL) return NULL;
 result = DeeVFS_Utf8LocateAtObject(root,u8path);
 Dee_DECREF(u8path);
 return result;
}
DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_WideLLocateAtObject(
 DEE_A_IN struct DeeVFSNode *root, DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 struct DeeVFSNode *result; DeeObject *u8path;
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 if ((u8path = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return NULL;
 result = DeeVFS_Utf8LLocateAtObject(root,u8path);
 Dee_DECREF(u8path);
 return result;
}
DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_WideLocateAtObject(
 DEE_A_IN struct DeeVFSNode *root, DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 struct DeeVFSNode *result; DeeObject *u8path;
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 if ((u8path = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return NULL;
 result = DeeVFS_Utf8LocateAtObject(root,u8path);
 Dee_DECREF(u8path);
 return result;
}







// The current working directory
struct DeeAtomicMutex        DeeVFS_CWD_lock = DeeAtomicMutex_INIT();
DEE_A_REF struct DeeVFSNode *DeeVFS_CWD = NULL;

DEE_A_RET_NOEXCEPT_REF struct DeeVFSNode *DeeVFS_GetActiveCwdNode(void) {
 struct DeeVFSNode *result;
 DeeAtomicMutex_Acquire(&DeeVFS_CWD_lock);
 if ((result = DeeVFS_CWD) != NULL) DeeVFSNode_INCREF(result);
 DeeAtomicMutex_Release(&DeeVFS_CWD_lock);
 return result;
}
DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_GetCwdNode(void) {
 struct DeeVFSNode *result; DeeObject *native_cwd;
 // Check if the cwd lies within the virtual filesystem
 DeeAtomicMutex_Acquire(&DeeVFS_CWD_lock);
 if ((result = DeeVFS_CWD) != NULL) {
  DeeVFSNode_INCREF(result);
  DeeAtomicMutex_Release(&DeeVFS_CWD_lock);
  return result;
 }
 DeeAtomicMutex_Release(&DeeVFS_CWD_lock);
 // Must use the native cwd, returning a path to it.
 if DEE_UNLIKELY((native_cwd = DeeNFS_GetCwd()) == NULL) return NULL;
 result = DeeVFS_LocateNative(DeeString_STR(native_cwd));
 Dee_DECREF(native_cwd);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeVFS_SetCwdNode(DEE_A_IN struct DeeVFSNode *cwd) {
 struct DeeVFSNode *old_cwd;
 DEE_ASSERT(cwd);
 if (DeeVFSNode_IsNative(cwd)) { // Update the native CWD directory
  if (DeeVFSNativeNode_NFS_Chdir(cwd) != 0) return -1;
 }
 DeeVFSNode_INCREF(cwd); // New reference for 'DeeVFS_CWD'
 DeeAtomicMutex_Acquire(&DeeVFS_CWD_lock);
 old_cwd = DeeVFS_CWD; DeeVFS_CWD = cwd;
 DeeAtomicMutex_Release(&DeeVFS_CWD_lock);
 if (old_cwd) DeeVFSNode_DECREF(old_cwd);
 return 0;
}
void DeeVFS_DelCwdNode(void) {
 struct DeeVFSNode *old_cwd;
 // Reset, to use the native CWD next time around
 DeeAtomicMutex_Acquire(&DeeVFS_CWD_lock);
 old_cwd = DeeVFS_CWD; DeeVFS_CWD = NULL;
 DeeAtomicMutex_Release(&DeeVFS_CWD_lock);
 if (old_cwd) DeeVFSNode_DECREF(old_cwd);
}

void DeeVFS_Shutdown(void) {
 DeeVFS_DelCwdNode();
}



static int DEE_CALL _deevfs_noopnode_vft_open(
 struct DeeVFSFile *DEE_UNUSED(self),
 Dee_openmode_t DEE_UNUSED(openmode),
 Dee_mode_t DEE_UNUSED(permissions)) {
 return 0;
}
struct _DeeVFSFileTypeData _DeeVFSNoopNodeType_FileData = {
 sizeof(struct DeeVFSFile),&_deevfs_noopnode_vft_open,
 NULL,NULL,NULL,NULL,NULL,NULL};



DEE_DECL_END

#ifndef __INTELLISENSE__
#define WIDE
#include "vfs_core.unicode_impl.inl"
#include "vfs_core.unicode_impl.inl"

#include "vfs_native_mount.c.inl"
#include "vfs_native_netmount.c.inl"
#include "vfs_native_node.c.inl"
#include "vfs_proc_node.c.inl"
#include "vfs_root.c.inl"
#include "vfs_virtual_dir.c.inl"
#include "vfs_virtual_file.c.inl"
#include "vfs_virtual_nullfile.c.inl"
#include "vfs_virtual_randomfile.c.inl"
#include "vfs_virtual_stdfile.c.inl"
#endif

#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#endif /* !GUARD_DEEMON_VFS_VFS_CORE_C */
