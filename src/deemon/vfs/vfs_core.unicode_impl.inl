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

#ifdef __INTELLISENSE__
#include "vfs_core.c"
#endif

#ifdef WIDE
#define DEESTRINGOBJECT      DeeWideStringObject
#define DEESTRINGWRITER      DeeWideStringWriter
#define DeeVFSNode_F(x)      DeeVFSNode_Wide##x
#define DeeString_F(x)       DeeWideString_##x
#define DeeStringWriter_F(x) DeeWideStringWriter_##x
#define DeeVFS_F(x)          DeeVFS_Wide##x
#define DeeNFS_F(x)          DeeNFS_Wide##x
#define DEE_CHAR             Dee_WideChar
#define DEE_STRPRINTF        "l"
#else
#define DEESTRINGOBJECT      DeeUtf8StringObject
#define DEESTRINGWRITER      DeeUtf8StringWriter
#define DeeVFSNode_F(x)      DeeVFSNode_Utf8##x
#define DeeString_F(x)       DeeUtf8String_##x
#define DeeStringWriter_F(x) DeeUtf8StringWriter_##x
#define DeeVFS_F(x)          DeeVFS_Utf8##x
#define DeeNFS_F(x)          DeeNFS_Utf8##x
#define DEE_CHAR             Dee_Utf8Char
#define DEE_STRPRINTF        ""
#endif

#ifdef WIDE
#define vld_startpath vld_wstartpath
#else
#define vld_startpath vld_8startpath
#endif


DEE_DECL_BEGIN

static void DeeVFS_F(ErrorMaxLinkIndirectionReached)(DEE_CHAR const *path) {
 DeeError_SetStringf(&DeeErrorType_ValueError,
                     "Max level of link indirection reached: %" DEE_STRPRINTF "q",path);
}




DEE_A_RET_EXCEPT(-1) int DeeVFSNode_F(WriteFilename)(
 DEE_A_INOUT struct DEESTRINGWRITER *writer, DEE_A_IN struct DeeVFSNode const *node) {
 DeeObject *name; int error; struct _DeeVFSViewTypeData *viewtype;
 DeeObject *(DEE_CALL *nameofproc)(struct DeeVFSNode *,struct DeeVFSNode *);
 DEE_ASSERT(writer); DEE_ASSERT(node);
 if (!node->vn_parent) return 0;
 if (DeeVFSNode_F(WriteFilename)(writer,node->vn_parent) != 0) return -1;
 if (DeeStringWriter_F(WriteChar)(writer,DEE_VFS_SEP) != 0) return -1;
 viewtype = node->vn_parent->vn_type->vnt_view;
 if DEE_UNLIKELY(!viewtype || (nameofproc = viewtype->vnt_nameof) == NULL)
  return DeeStringWriter_F(WriteChar)(writer,'?');
 if ((name = (*nameofproc)(node->vn_parent,(struct DeeVFSNode *)node)) == NULL) return -1;
 error = DeeStringWriter_F(WriteObjectStr)(writer,name);
 Dee_DECREF(name);
 return error;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeVFSNode_F(Filename)(DEE_A_IN struct DeeVFSNode const *node) {
 struct DEESTRINGWRITER writer = DeeStringWriter_F(INIT)();
 DeeObject *result; DEE_ASSERT(node);
#ifdef WIDE
 if (!node->vn_parent) DeeReturn_STATIC_WIDE_STRING_EX(1,{DEE_VFS_SEP});
#else
 if (!node->vn_parent) DeeReturn_STATIC_UTF8_STRING_EX(1,{DEE_VFS_SEP});
#endif
 if (DeeVFSNode_F(WriteFilename)(&writer,node) != 0) result = NULL;
 else result = DeeStringWriter_F(Pack)(&writer);
 DeeStringWriter_F(Quit)(&writer);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeVFSNode_F(Pathname)(DEE_A_IN struct DeeVFSNode const *node) {
 DeeObject *result; DEE_ASSERT(node);
#ifdef WIDE
 if (!node->vn_parent) DeeReturn_STATIC_WIDE_STRING_EX(1,{DEE_VFS_SEP});
#else
 if (!node->vn_parent) DeeReturn_STATIC_UTF8_STRING_EX(1,{DEE_VFS_SEP});
#endif
 {
  struct DEESTRINGWRITER writer = DeeStringWriter_F(INIT)();
  if (DeeVFSNode_F(WriteFilename)(&writer,node->vn_parent) != 0
   || DeeStringWriter_F(WriteChar)(&writer,DEE_VFS_SEP) != 0) result = NULL;
  else result = DeeStringWriter_F(Pack)(&writer);
  DeeStringWriter_F(Quit)(&writer);
  return result;
 }
}


DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_F(LLocateAt_impl)(
 DEE_A_INOUT struct DeeVFSLocateState *state,
 DEE_A_IN struct DeeVFSNode *root, DEE_A_IN_Z DEE_CHAR const *path) {
 struct DeeVFSNode *result,*newresult;
 Dee_size_t partsize; int error;
 DEE_CHAR const *path_iter,*part_begin;
 DEE_ASSERTF(!DEE_VFS_ISSEP(path[0]),
             "Given path %" DEE_STRPRINTF "q isn't a relative path to %R",
             path,DeeVFSNode_Filename(root));
 DeeVFSNode_INCREF(root); result = root;
 path_iter = path;
 while (*path_iter) {
  while (1) {
   if DEE_UNLIKELY((error = DeeVFSNode_IsLink(result)) < 0) return NULL;
   if (!error) break;
   if DEE_UNLIKELY(++state->vld_link_ind == DEE_VFS_MAX_LINK_INDIRECTION) {
    DeeVFS_F(ErrorMaxLinkIndirectionReached)(state->vld_startpath);
    return NULL;
   }
   newresult = DeeVFSNode_WalkLink_impl(state,result);
   DeeVFSNode_DECREF(result);
   if DEE_UNLIKELY(!newresult) return NULL;
   result = newresult;
  }
  if (!DeeVFSNode_HasWalk(result)) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "Can't walk virtual path %R with %" DEE_STRPRINTF "q",
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
    DEE_ASSERTF(result->vn_type->vnt_view && (
     result->vn_type->vnt_view->vnt_walk ||
     result->vn_type->vnt_view->vnt_wwalk),
     "Already checked above");
#ifdef WIDE
    if (result->vn_type->vnt_view->vnt_wwalk)
#else
    if (result->vn_type->vnt_view->vnt_walk)
#endif
    {
#ifdef WIDE
     newresult = (*result->vn_type->vnt_view->vnt_wwalk)(result,part_begin,partsize);
#else
     newresult = (*result->vn_type->vnt_view->vnt_walk)(result,part_begin,partsize);
#endif
    } else {
     DeeObject *other_path;
#ifdef WIDE
     if DEE_UNLIKELY((other_path = DeeUtf8String_FromWideStringWithLength(
      partsize,part_begin)) == NULL) goto err_r;
     newresult = (*result->vn_type->vnt_view->vnt_walk)(
      result,DeeUtf8String_STR(other_path),DeeUtf8String_SIZE(other_path));
#else
     if DEE_UNLIKELY((other_path = DeeWideString_FromUtf8StringWithLength(
      partsize,part_begin)) == NULL) goto err_r;
     newresult = (*result->vn_type->vnt_view->vnt_wwalk)(
      result,DeeWideString_STR(other_path),DeeWideString_SIZE(other_path));
#endif
     Dee_DECREF(other_path);
    }
    if DEE_UNLIKELY(!newresult) goto err_r;
    DeeVFSNode_DECREF(result);
    result = newresult;
   }
  }
  while (DEE_VFS_ISSEP(*path_iter)) ++path_iter;
 }
 return result;
}


#ifdef DEE_PLATFORM_WINDOWS
DEE_A_RET_EXCEPT_REF struct DeeVFSNode *
DeeVFS_F(LocateNative)(DEE_A_IN_Z DEE_CHAR const *path) {
 if (DeeVFS_F(IsAbsoluteNativePath)(path)) {
  struct DeeVFSNode *result,*newresult;
  // Locate a native win32-style path
#ifdef WIDE
  result = DeeVFSNode_WideWalk(DeeVFSNative_Root,path,1);
#else
  result = DeeVFSNode_Utf8Walk(DeeVFSNative_Root,path,1);
#endif
  if DEE_UNLIKELY(!result) return NULL;
  // Now just traverse the regular virtual path
  path += 2; while (DEE_VFS_ISSEP(*path)) ++path;
  newresult = DeeVFS_F(LLocateAt)(result,path);
  DeeVFSNode_DECREF(result);
  return newresult;
 }
 return DeeVFS_F(LLocate)(path);
}
#endif


DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *DeeVFS_F(GetCwd)(void) {
 struct DeeVFSNode *virtual_cwd; DeeObject *result;
 DeeAtomicMutex_Acquire(&DeeVFS_CWD_lock);
 if ((virtual_cwd = DeeVFS_CWD) != NULL) {
  // A virtual CWD has been set (return its path)
  DeeVFSNode_INCREF(virtual_cwd);
  DeeAtomicMutex_Release(&DeeVFS_CWD_lock);
  result = DeeVFSNode_Filename(virtual_cwd);
  DeeVFSNode_DECREF(virtual_cwd);
#ifdef WIDE
  if DEE_LIKELY(result) {
   DeeObject *newresult;
   newresult = DeeWideString_Cast(result);
   Dee_DECREF(result);
   result = newresult;
  }
#endif
  return result;
 }
 DeeAtomicMutex_Release(&DeeVFS_CWD_lock);
 // Not virtual CWD --> Return the native one
 return DeeNFS_F(GetCwd)();
}

DEE_A_RET_EXCEPT(-1) int DeeVFS_F(Chdir)(DEE_A_IN_Z DEE_CHAR const *path) {
 struct DeeVFSNode *oldcwd,*newcwd; int error;
 if (DeeVFS_F(IsVirtualPath)(path)) { // Absolute virtual path
  newcwd = DeeVFS_F(Locate)(path);
  if DEE_UNLIKELY(!newcwd) return -1;
  error = DeeVFS_SetCwdNode(newcwd);
  Dee_DECREF(newcwd);
  return error;
 }
 if (DeeVFS_F(IsAbsoluteNativePath)(path)) { // Absolute native path
  // Delete the (possibly) active virtual CWD path
  DeeVFS_DelCwdNode();
  // Update the native CWD
native_chdir:
  return DeeNFS_F(Chdir)(path);
 }
walk_relative:
 // Relative virtual/native path
 DeeAtomicMutex_Acquire(&DeeVFS_CWD_lock);
 if ((oldcwd = DeeVFS_CWD) == NULL) {
  DeeAtomicMutex_Release(&DeeVFS_CWD_lock);
  // No virtual CWD node set --> 'path' must be relative within the native filesystem
  goto native_chdir;
 }
 DeeVFSNode_INCREF(oldcwd);
 DeeAtomicMutex_Release(&DeeVFS_CWD_lock);
 // Walk a relative path within the virtual filesystem
 newcwd = DeeVFS_F(LocateAt)(oldcwd,path);
 if DEE_UNLIKELY(!newcwd) {err_oldcwd: DeeVFSNode_DECREF(oldcwd); return -1; }
 if (DeeVFSNode_IsNative(newcwd) && DeeVFSNativeNode_NFS_Chdir(newcwd) != 0) {
  DeeVFSNode_DECREF(newcwd);
  goto err_oldcwd;
 }
 DeeAtomicMutex_Acquire(&DeeVFS_CWD_lock);
 // Make sure that the cwd wasn't changed by another thread in the meantime
 if (oldcwd != DeeVFS_CWD) {
  DeeAtomicMutex_Release(&DeeVFS_CWD_lock);
  DeeVFSNode_DECREF(newcwd);
  DeeVFSNode_DECREF(oldcwd);
  goto walk_relative;
 }
 // NOTE: Also inherit a second reference from 'DeeVFS_CWD' into 'oldcwd'
 Dee_INHERIT_REF(DeeVFS_CWD,newcwd);
 DeeAtomicMutex_Release(&DeeVFS_CWD_lock);
 // Remove 2 references: One was stored in 'oldcwd' while we
 //                      were traversing the relative path.
 //                      And the other was stored on DeeVFS_CWD
 DeeVFSNode_DECREF(oldcwd);
 DeeVFSNode_DECREF(oldcwd);

 return 0;
}




DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeVFS_F(ForceNativePathObject)(DEE_A_IN_OBJECT(DEESTRINGOBJECT) const *path) {
 struct DeeVFSNode *cwd,*resultpath; DeeObject *result;
 if (DeeVFS_F(IsAbsoluteNativePathObject)(path)) {
  // Simple case: 'path' is an absolute, native filesystem path --> Just re-return it
retpath:
  DeeReturn_NEWREF(path);
 }
 if (DeeVFS_F(IsVirtualPathObject)(path)) {
  DEE_CHAR const *path_begin = DeeString_F(STR)(path);
  do ++path_begin; while (DEE_VFS_ISSEP(*path_begin));
  resultpath = DeeVFS_F(LocateAt)(DeeVFS_Root,path_begin);
 } else {
  if ((cwd = DeeVFS_GetCwdNode()) == NULL) {
   // No virtual CWD node set --> 'path' must be relative within the native filesystem
   // In this situation, we're allowed to return a relative native filesystem path.
   goto retpath;
  }
  if (DeeVFSNode_IsNative(cwd)) {
   // The CWD lies with the native part of the FS.
   // >> No need to adjust the path
   DeeVFSNode_DECREF(cwd);
   goto retpath;
  }
  // Expand the given path based on what we just learned from 'cwd'
  resultpath = DeeVFS_F(LocateAt)(cwd,DeeString_F(STR)(path));
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!resultpath) return NULL;
 if DEE_UNLIKELY(!DeeVFSNode_IsNative(resultpath)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Expected native path, but %r describes virtual path %R",
                      path,DeeVFSNode_Filename(resultpath));
  DeeVFSNode_DECREF(resultpath);
  return NULL;
 }
#ifdef WIDE
 result = DeeWideString_Cast(DeeVFSNativeNode_PATH(resultpath));
#else
 result = DeeUtf8String_Cast(DeeVFSNativeNode_PATH(resultpath));
#endif
 DeeVFSNode_DECREF(resultpath);
 return result;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeVFS_F(ForceNativePath)(DEE_A_IN_Z DEE_CHAR const *path) {
 struct DeeVFSNode *cwd,*resultpath; DeeObject *result;
 if (DeeVFS_F(IsAbsoluteNativePath)(path)) {
  // Simple case: 'path' is an absolute, native filesystem path --> Just re-return it
retpath:
  return DeeString_F(New)(path);
 }
 if (DeeVFS_F(IsVirtualPath)(path)) {
  do ++path; while (DEE_VFS_ISSEP(*path));
  resultpath = DeeVFS_F(LocateAt)(DeeVFS_Root,path);
 } else {
  if ((cwd = DeeVFS_GetCwdNode()) == NULL) {
   // No virtual CWD node set --> 'path' must be relative within the native filesystem
   // In this situation, we're allowed to return a relative native filesystem path.
   goto retpath;
  }
  if (DeeVFSNode_IsNative(cwd)) {
   // The CWD lies with the native part of the FS.
   // >> No need to adjust the path
   DeeVFSNode_DECREF(cwd);
   goto retpath;
  }
  // Expand the given path based on what we just learned from 'cwd'
  resultpath = DeeVFS_F(LocateAt)(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!resultpath) return NULL;
 if DEE_UNLIKELY(!DeeVFSNode_IsNative(resultpath)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Expected native path, but %" DEE_STRPRINTF "q describes virtual path %R",
                      path,DeeVFSNode_Filename(resultpath));
  DeeVFSNode_DECREF(resultpath);
  return NULL;
 }
#ifdef WIDE
 result = DeeWideString_Cast(DeeVFSNativeNode_PATH(resultpath));
#else
 result = DeeUtf8String_Cast(DeeVFSNativeNode_PATH(resultpath));
#endif
 DeeVFSNode_DECREF(resultpath);
 return result;
}

extern DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *DeeVFS_F(ForceNativePathWithCwd)(
 DEE_A_IN struct DeeVFSNode *cwd, DEE_A_IN_Z DEE_CHAR const *path) {
 struct DeeVFSNode *resultpath; DeeObject *result;
 DEE_ASSERT(cwd);
 DEE_ASSERT(path);
 DEE_ASSERT(!DeeVFS_F(IsVirtualPath)(path));
 DEE_ASSERT(!DeeVFS_F(IsAbsoluteNativePath)(path));
 DEE_ASSERT(!DeeVFSNode_IsNative(cwd));
 // Expand the given path based on what we just learned from 'cwd'
 resultpath = DeeVFS_F(LocateAt)(cwd,path);
 if DEE_UNLIKELY(!resultpath) return NULL;
 if DEE_UNLIKELY(!DeeVFSNode_IsNative(resultpath)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Expected native path, but %" DEE_STRPRINTF "q describes virtual path %R",
                      path,DeeVFSNode_Filename(resultpath));
  DeeVFSNode_DECREF(resultpath);
  return NULL;
 }
#ifdef WIDE
 result = DeeWideString_Cast(DeeVFSNativeNode_PATH(resultpath));
#else
 result = DeeUtf8String_Cast(DeeVFSNativeNode_PATH(resultpath));
#endif
 DeeVFSNode_DECREF(resultpath);
 return result;
}
extern DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeVFS_F(ForceNativeRootPath)(DEE_A_IN_Z DEE_CHAR const *path) {
 struct DeeVFSNode *resultpath; DeeObject *result;
 DEE_ASSERT(path);
 DEE_ASSERT(DeeVFS_F(IsVirtualPath)(path));
 do ++path; while (DEE_VFS_ISSEP(*path));
 resultpath = DeeVFS_F(LocateAt)(DeeVFS_Root,path);
 if DEE_UNLIKELY(!resultpath) return NULL;
 if DEE_UNLIKELY(!DeeVFSNode_IsNative(resultpath)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Expected native path, but %" DEE_STRPRINTF "q describes virtual path %R",
                      path,DeeVFSNode_Filename(resultpath));
  DeeVFSNode_DECREF(resultpath);
  return NULL;
 }
#ifdef WIDE
 result = DeeWideString_Cast(DeeVFSNativeNode_PATH(resultpath));
#else
 result = DeeUtf8String_Cast(DeeVFSNativeNode_PATH(resultpath));
#endif
 DeeVFSNode_DECREF(resultpath);
 return result;
}


DEE_A_RET_EXCEPT_REF struct DeeVFSNode *
DeeVFS_F(LLocate)(DEE_A_IN_Z DEE_CHAR const *path) {
 struct DeeVFSLocateState state;
 struct DeeVFSNode *result,*cwd;
 state.vld_startpath = path;
 state.vld_link_ind = 0;
 if (DEE_VFS_ISSEP(*path)) { // Absolute path
  do ++path; while (DEE_VFS_ISSEP(*path));
  return DeeVFS_F(LLocateAt_impl)(&state,DeeVFS_Root,path);
 }
 if DEE_UNLIKELY((cwd = DeeVFS_GetCwdNode()) == NULL) return NULL;
 result = DeeVFS_F(LLocateAt_impl)(&state,cwd,path);
 DeeVFSNode_DECREF(cwd);
 return result;
}
DEE_A_RET_EXCEPT_REF struct DeeVFSNode *
DeeVFS_F(Locate)(DEE_A_IN_Z DEE_CHAR const *path) {
 struct DeeVFSLocateState state;
 struct DeeVFSNode *result,*cwd;
 state.vld_startpath = path;
 state.vld_link_ind = 0;
 if (DEE_VFS_ISSEP(*path)) { // Absolute path
  do ++path; while (DEE_VFS_ISSEP(*path));
  return DeeVFS_F(LocateAt_impl)(&state,DeeVFS_Root,path);
 }
 if DEE_UNLIKELY((cwd = DeeVFS_GetCwdNode()) == NULL) return NULL;
 result = DeeVFS_F(LocateAt_impl)(&state,cwd,path);
 DeeVFSNode_DECREF(cwd);
 return result;
}

DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_F(LocateAt_impl)(
 DEE_A_INOUT struct DeeVFSLocateState *state,
 DEE_A_IN struct DeeVFSNode *root, DEE_A_IN_Z DEE_CHAR const *path) {
 struct DeeVFSNode *result,*newresult; int error;
 if DEE_UNLIKELY((result = DeeVFS_F(LLocateAt_impl)(state,root,path)) == NULL) return NULL;
 while (1) {
  if DEE_UNLIKELY((error = DeeVFSNode_IsLink(result)) < 0) goto err_r;
  if (!error) break;
  if DEE_UNLIKELY(++state->vld_link_ind == DEE_VFS_MAX_LINK_INDIRECTION) {
    DeeVFS_F(ErrorMaxLinkIndirectionReached)(state->vld_startpath);
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

DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_F(LLocateWithCWD)(
 DEE_A_IN struct DeeVFSNode *cwd, DEE_A_IN_Z DEE_CHAR const *path) {
 struct DeeVFSLocateState state;
 DEE_ASSERT(cwd);
 state.vld_startpath = path;
 state.vld_link_ind = 0;
 if (DEE_VFS_ISSEP(*path)) { // Absolute path
  do ++path; while (DEE_VFS_ISSEP(*path));
  return DeeVFS_F(LLocateAt_impl)(&state,DeeVFS_Root,path);
 }
 return DeeVFS_F(LLocateAt_impl)(&state,cwd,path);
}
DEE_A_RET_EXCEPT_REF struct DeeVFSNode *DeeVFS_F(LocateWithCWD)(
 DEE_A_IN struct DeeVFSNode *cwd, DEE_A_IN_Z DEE_CHAR const *path) {
 struct DeeVFSLocateState state;
 DEE_ASSERT(cwd);
 state.vld_startpath = path;
 state.vld_link_ind = 0;
 if (DEE_VFS_ISSEP(*path)) { // Absolute path
  do ++path; while (DEE_VFS_ISSEP(*path));
  return DeeVFS_F(LocateAt_impl)(&state,DeeVFS_Root,path);
 }
 return DeeVFS_F(LocateAt_impl)(&state,cwd,path);
}



#undef DEESTRINGOBJECT
#undef DEESTRINGWRITER
#undef DeeVFSNode_F
#undef DeeString_F
#undef DeeStringWriter_F
#undef DeeVFS_F
#undef DeeNFS_F
#undef DEE_CHAR
#undef DEE_STRPRINTF
#undef vld_startpath
#ifdef WIDE
#undef WIDE
#endif

DEE_DECL_END
