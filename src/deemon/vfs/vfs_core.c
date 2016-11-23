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

#if DEE_CONFIG_RUNTIME_HAVE_VFS2
DEE_DECL_BEGIN


void _DeeVFSNode_Delete(DEE_A_IN struct DeeVFSNode *self) {
 void (DEE_CALL *quit_func)(struct DeeVFSNode *);
 if ((quit_func = self->vn_type->vnt_node.vnt_quit) != NULL) (*quit_func)(self);
 if (self->vn_parent) DeeVFSNode_DECREF(self->vn_parent);
 DeeVFSNode_FREE(self);
}



DEE_A_RET_EXCEPT(-1) int DeeVFSNode_WriteFilename(
 DEE_A_INOUT struct DeeStringWriter *writer, DEE_A_IN struct DeeVFSNode const *node) {
 DeeObject *name; int error;
 DeeObject *(DEE_CALL *nameofproc)(struct DeeVFSNode *,struct DeeVFSNode *);
 DEE_ASSERT(writer); DEE_ASSERT(node);
 if (!node->vn_parent) return 0;
 if (DeeVFSNode_WriteFilename(writer,node->vn_parent) != 0) return -1;
 if (DeeStringWriter_WriteChar(writer,DEE_VFS_SEP) != 0) return -1;
 nameofproc = node->vn_parent->vn_type->vnt_node.vnt_nameof;
 if (!nameofproc) return DeeStringWriter_WriteChar(writer,'?');
 if ((name = (*nameofproc)(node->vn_parent,(struct DeeVFSNode *)node)) == NULL) return -1;
 DEE_ASSERT(DeeObject_Check(name) && DeeString_Check(name));
 error = DeeStringWriter_WriteStringWithLength(writer,DeeString_SIZE(name),DeeString_STR(name));
 Dee_DECREF(name);
 return error;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeVFSNode_Name(DEE_A_IN struct DeeVFSNode const *node) {
 DeeObject *(DEE_CALL *nameof_func)(struct DeeVFSNode *,struct DeeVFSNode *);
 if DEE_UNLIKELY(!node->vn_parent) DeeReturn_STATIC_STRING("/");
 nameof_func = node->vn_parent->vn_type->vnt_node.vnt_nameof;
 if DEE_UNLIKELY(!nameof_func) DeeReturn_STATIC_STRING("?");
 return (*nameof_func)(node->vn_parent,(struct DeeVFSNode *)node);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeVFSNode_Filename(DEE_A_IN struct DeeVFSNode const *node) {
 DeeObject *result; struct DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(node);
 if (!node->vn_parent) DeeReturn_STATIC_STRING_EX(1,{DEE_VFS_SEP});
 if (DeeVFSNode_WriteFilename(&writer,node) != 0) result = NULL;
 else result = DeeStringWriter_Pack(&writer);
 DeeStringWriter_Quit(&writer);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeVFSNode_Pathname(DEE_A_IN struct DeeVFSNode const *node) {
 DeeObject *result;
 DEE_ASSERT(node);
 if (!node->vn_parent) DeeReturn_STATIC_STRING_EX(1,{DEE_VFS_SEP});
 {
  struct DeeStringWriter writer = DeeStringWriter_INIT();
  if (DeeVFSNode_WriteFilename(&writer,node->vn_parent) != 0
   || DeeStringWriter_WriteChar(&writer,DEE_VFS_SEP) != 0) result = NULL;
  else result = DeeStringWriter_Pack(&writer);
  DeeStringWriter_Quit(&writer);
  return result;
 }
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeVFSNode_HasHiddenFilename(
 DEE_A_IN struct DeeVFSNode const *self) {
 DeeObject *node_name; int result;
 DeeObject *(DEE_CALL *nameofproc)(struct DeeVFSNode *,struct DeeVFSNode *);
 DEE_ASSERT(self);
 if DEE_UNLIKELY(!self->vn_parent) return 0;
 nameofproc = self->vn_parent->vn_type->vnt_node.vnt_nameof;
 if DEE_UNLIKELY(!nameofproc) return 0;
 node_name = (*nameofproc)(self->vn_parent,(struct DeeVFSNode *)self);
 if DEE_UNLIKELY(!node_name) return -1;
 DEE_ASSERT(DeeObject_Check(node_name) &&
            DeeString_Check(node_name));
 result = DeeString_STR(node_name)[0] == '.';
 Dee_DECREF(node_name);
 return result;
}

DEE_A_RET_WUNUSED int DeeVFSNode_IsMount(DEE_A_IN struct DeeVFSNode const *self) {
 DeeStringObject *native_path;
 DEE_ASSERT(self);
 if (!DeeVFSNode_IsNative(self)) return 0;
 native_path = ((struct DeeVFSNativeNode *)self)->vnn_path;
 if (!DeeString_SIZE(native_path)) return 0;
 return DeeString_STR(native_path)[DeeString_SIZE(native_path)-1] == ':';
}

DEE_A_RET_EXCEPT(-1) int DeeVFSNode_GetTimes(
 DEE_A_INOUT struct DeeVFSNode *self, DEE_A_OUT_OPT Dee_timetick_t *atime,
 DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 DEE_ASSERT(self);
 if (!self->vn_type->vnt_node.vnt_gettimes) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Can't read time for %R",
                      DeeVFSNode_Filename(self));
  return -1;
 }
 return (*self->vn_type->vnt_node.vnt_gettimes)(self,atime,ctime,mtime);
}
DEE_A_RET_EXCEPT(-1) int DeeVFSNode_SetTimes(
      DEE_A_INOUT struct DeeVFSNode *self, DEE_A_IN_OPT Dee_timetick_t const *atime,
 DEE_A_IN_OPT Dee_timetick_t const *ctime, DEE_A_IN_OPT Dee_timetick_t const *mtime) {
 DEE_ASSERT(self);
 if (!self->vn_type->vnt_node.vnt_settimes) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Can't write time for %R",
                      DeeVFSNode_Filename(self));
  return -1;
 }
 return (*self->vn_type->vnt_node.vnt_settimes)(self,atime,ctime,mtime);
}
DEE_A_RET_EXCEPT(-1) int DeeVFSNode_GetMod(
 DEE_A_INOUT struct DeeVFSNode *self, DEE_A_OUT Dee_mode_t *mode) {
 int error;
 DEE_ASSERT(self); DEE_ASSERT(mode);
 if (self->vn_type->vnt_node.vnt_getmod)
  return (*self->vn_type->vnt_node.vnt_getmod)(self,mode);
 *mode = (Dee_mode_t)(self->vn_type->vnt_file.vft_write ? 0222 : 0000);
 if (self->vn_type->vnt_node.vnt_isexecutable) {
  error = (*self->vn_type->vnt_node.vnt_isexecutable)(self);
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
 DEE_ASSERT(self);
 if (!self->vn_type->vnt_node.vnt_chmod) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Can't chmod %R",
                      DeeVFSNode_Filename(self));
  return -1;
 }
 return (*self->vn_type->vnt_node.vnt_chmod)(self,mode);
}
DEE_A_RET_EXCEPT(-1) int DeeVFSNode_GetOwn(
 DEE_A_INOUT struct DeeVFSNode *self,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 DEE_ASSERT(self);
 if (!self->vn_type->vnt_node.vnt_getown) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Can't getown %R",
                      DeeVFSNode_Filename(self));
  return -1;
 }
 return (*self->vn_type->vnt_node.vnt_getown)(self,owner,group);
}
DEE_A_RET_EXCEPT(-1) int DeeVFSNode_Chown(
 DEE_A_INOUT struct DeeVFSNode *self,
 DEE_A_IN Dee_uid_t owner, DEE_A_IN Dee_gid_t group) {
 DEE_ASSERT(self);
 if (!self->vn_type->vnt_node.vnt_chown) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Can't chown %R",
                      DeeVFSNode_Filename(self));
  return -1;
 }
 return (*self->vn_type->vnt_node.vnt_chown)(self,owner,group);
}




DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFSNode_WalkLink_impl(
 DEE_A_INOUT struct DeeVFSLocateState *state, DEE_A_INOUT struct DeeVFSNode *self) {
 struct DeeVFSNode *result;
 DeeObject *link_path; char const *link_str;
 if DEE_UNLIKELY((link_path = DeeVFSNode_ReadLink(self)) == NULL) return NULL;
 link_str = DeeString_STR(link_path);
 if (DEE_VFS_ISSEP(link_str[0])) { // Absolute link
  do ++link_str; while (DEE_VFS_ISSEP(link_str[0]));
  result = DeeVFS_LLocateAt_impl(state,DeeVFS_Root,link_str);
 } else { // Relative link
  result = DeeVFS_LLocateAt_impl(state,self,link_str);
 }
 Dee_DECREF(link_path);
 return result;
}


static void _DeeVFSError_MaxLinkIndirectionReached(char const *path) {
 DeeError_SetStringf(&DeeErrorType_ValueError,
                     "Max level of link indirection reached: %q",path);
}


DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_LLocateAt_impl(
 DEE_A_INOUT struct DeeVFSLocateState *state,
 DEE_A_IN struct DeeVFSNode *root, DEE_A_IN_Z char const *path) {
 struct DeeVFSNode *newroot,*result,*newresult;
 char *part; Dee_size_t partsize;
 char const *path_iter,*part_begin;
 DEE_ASSERTF(!DEE_VFS_ISSEP(path[0]),
             "Given path %q isn't a relative path to %R",
             path,DeeVFSNode_Filename(root));
 if (DeeVFSNode_IsLink(root)) {
  if DEE_UNLIKELY(++state->vld_link_ind == DEE_VFS_MAX_LINK_INDIRECTION) {
   _DeeVFSError_MaxLinkIndirectionReached(state->vld_startpath);
   return NULL;
  }
  newroot = DeeVFSNode_WalkLink_impl(state,root);
  result = DeeVFS_LLocateAt_impl(state,newroot,path);
  DeeVFSNode_DECREF(newroot);
  return result;
 }
 DeeVFSNode_INCREF(root); result = root;
 path_iter = path;
 while (*path_iter) {
  if (!DeeVFSNode_HasWalk(result)) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "Can't walk virtual path %R with %q",
                       DeeVFSNode_Filename(result),path_iter);
err_r:
   DeeVFSNode_DECREF(result);
   return NULL;
  }
  part_begin = path_iter;
  while (*path_iter && !DEE_VFS_ISSEP(*path_iter)) ++path_iter;
  if ((partsize = (Dee_size_t)(path_iter-part_begin)) != 0) {
   // Check for special part names ('.' and '..')
   if (partsize == 1 && part_begin[0] == '.') {
    // same-directory reference (ignore)
   } else if (partsize == 2 && part_begin[0] == '.' && part_begin[1] == '.') {
    // parent-directory reference
    if DEE_LIKELY(result->vn_parent) {
     newresult = result->vn_parent;
     DeeVFSNode_INCREF(newresult);
     DeeVFSNode_DECREF(result);
     result = newresult;
    }
   } else {
    while DEE_UNLIKELY((part = (char *)malloc_nz((partsize+1)*sizeof(char))) == NULL) {
     if DEE_LIKELY(Dee_CollectMemory()) continue;
     DeeError_NoMemory();
     goto err_r;
    }
    memcpy(part,part_begin,partsize*sizeof(char));
    part[partsize] = 0;
    newresult = (*result->vn_type->vnt_node.vnt_walk)(result,part);
    free_nn(part);
    if DEE_UNLIKELY(!newresult) goto err_r;
    DeeVFSNode_DECREF(result);
    result = newresult;
   }
  }
  while (DEE_VFS_ISSEP(*path_iter)) ++path_iter;
 }
 return result;
}

DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_LocateAt_impl(
 DEE_A_INOUT struct DeeVFSLocateState *state,
 DEE_A_IN struct DeeVFSNode *root, DEE_A_IN_Z char const *path) {
 struct DeeVFSNode *result,*newresult;
 if DEE_UNLIKELY((result = DeeVFS_LLocateAt_impl(state,root,path)) == NULL) return NULL;
 while (DeeVFSNode_IsLink(result)) {
  if DEE_UNLIKELY(++state->vld_link_ind == DEE_VFS_MAX_LINK_INDIRECTION) {
   _DeeVFSError_MaxLinkIndirectionReached(state->vld_startpath);
   DeeVFSNode_DECREF(result);
   return NULL;
  }
  newresult = DeeVFSNode_WalkLink_impl(state,result);
  DeeVFSNode_DECREF(result);
  if (!newresult) return NULL;
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




DEE_A_RET_EXCEPT_REF struct DeeVFSNode *
DeeVFS_LocateNative(DEE_A_IN_Z char const *path) {
#ifdef DEE_PLATFORM_WINDOWS
 if (DeeVFS_Utf8IsAbsoluteNativePath(path)) {
  struct DeeVFSNode *result,*newresult;
  char drive_name[2];
  // Locate a native win32-style path
  drive_name[0] = *path;
  drive_name[1] = 0;
  result = DeeVFSNode_Walk(DeeVFSNative_Root,drive_name);
  if DEE_UNLIKELY(!result) return NULL;
  // Now just traverse the regular virtual path
  path += 2; while (DEE_VFS_ISSEP(*path)) ++path;
  newresult = DeeVFS_LLocateAt(result,path);
  DeeVFSNode_DECREF(result);
  return newresult;
 }
#endif
 return DeeVFS_LLocate(path);
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
  if (DeeNFS_ChdirObject((DeeObject *)((DeeVFSNativeNode *)cwd)->vnn_path) != 0) return -1;
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



DEE_DECL_END

#ifndef __INTELLISENSE__
#define WIDE
#include "vfs_core.unicode_impl.inl"
#include "vfs_core.unicode_impl.inl"
#include "vfs_native_mount.c.inl"
#include "vfs_native_node.c.inl"
#include "vfs_virtual_stdfile.c.inl"
#include "vfs_virtual_dir.c.inl"
#include "vfs_root.c.inl"
#endif

#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */

#endif /* !GUARD_DEEMON_VFS_VFS_CORE_C */
