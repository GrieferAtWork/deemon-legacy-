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
#ifndef GUARD_DEEMON_FS_VIRTUAL_FS_OPENDIR_C_INL
#define GUARD_DEEMON_FS_VIRTUAL_FS_OPENDIR_C_INL 1
#ifndef DEE_EXTENSION
#define DEE_EXTENSION 1
#endif

#include <deemon/__conf.inl>
#include <deemon/string.h>
#include <deemon/type.h>
#include "native_hooks.h"
#include "objects/vfs_view.h"
#include "vfs_core.h"
#include "vfs_native_node.h"
#include "vfs_root.h"
#include "virtual_fs.h"

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT_REF DeeObject *DeeVFS_Utf8Opendir(DEE_A_IN_Z Dee_Utf8Char const *path) {
 struct DeeVFSNode *cwd,*filenode; DeeObject *result;
 DEE_ASSERT(path);
 if (DeeVFS_Utf8IsAbsoluteNativePath(path)) {
call_native: return DeeHFS_Utf8Opendir(path);
 }
 if (DeeVFS_Utf8IsVirtualPath(path)) {
  do ++path; while (DEE_VFS_ISSEP(*path));
  filenode = DeeVFS_Utf8LocateAt(DeeVFS_Root,path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  // if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native; }
  filenode = DeeVFS_Utf8LocateAt(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) return NULL;
 result = DeeVFSNode_IsNative(filenode)
  ? DeeVFSNativeNode_HFS_Opendir(filenode)
  : (DeeObject *)DeeVFSView_OpenNode(filenode);
 DeeVFSNode_DECREF(filenode);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeVFS_WideOpendir(DEE_A_IN_Z Dee_WideChar const *path) {
 struct DeeVFSNode *cwd,*filenode; DeeObject *result;
 DEE_ASSERT(path);
 if (DeeVFS_WideIsAbsoluteNativePath(path)) {
call_native: return DeeHFS_WideOpendir(path);
 }
 if (DeeVFS_WideIsVirtualPath(path)) {
  do ++path; while (DEE_VFS_ISSEP(*path));
  filenode = DeeVFS_WideLocateAt(DeeVFS_Root,path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  // if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native; }
  filenode = DeeVFS_WideLocateAt(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) return NULL;
 result = DeeVFSNode_IsNative(filenode)
  ? DeeVFSNativeNode_HFS_Opendir(filenode)
  : (DeeObject *)DeeVFSView_OpenNode(filenode);
 DeeVFSNode_DECREF(filenode);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeVFS_Utf8OpendirObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
 struct DeeVFSNode *cwd,*filenode; DeeObject *result;
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
 if (DeeVFS_Utf8IsAbsoluteNativePathObject(path)) {
call_native: return DeeHFS_Utf8OpendirObject(path);
 }
 if (DeeVFS_Utf8IsVirtualPathObject(path)) {
  Dee_Utf8Char const *path_begin = DeeUtf8String_STR(path);
  do ++path_begin; while (DEE_VFS_ISSEP(*path_begin));
  filenode = DeeVFS_Utf8LocateAt(DeeVFS_Root,path_begin);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  // if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native; }
  filenode = DeeVFS_Utf8LocateAtObject(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) return NULL;
 result = DeeVFSNode_IsNative(filenode)
  ? DeeVFSNativeNode_HFS_Opendir(filenode)
  : (DeeObject *)DeeVFSView_OpenNode(filenode);
 DeeVFSNode_DECREF(filenode);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeVFS_WideOpendirObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 struct DeeVFSNode *cwd,*filenode; DeeObject *result;
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 if (DeeVFS_WideIsAbsoluteNativePathObject(path)) {
call_native: return DeeHFS_WideOpendirObject(path);
 }
 if (DeeVFS_WideIsVirtualPathObject(path)) {
  Dee_WideChar const *path_begin = DeeWideString_STR(path);
  do ++path_begin; while (DEE_VFS_ISSEP(*path_begin));
  filenode = DeeVFS_WideLocateAt(DeeVFS_Root,path_begin);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  // if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native; }
  filenode = DeeVFS_WideLocateAtObject(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) return NULL;
 result = DeeVFSNode_IsNative(filenode)
  ? DeeVFSNativeNode_HFS_Opendir(filenode)
  : (DeeObject *)DeeVFSView_OpenNode(filenode);
 DeeVFSNode_DECREF(filenode);
 return result;
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_VIRTUAL_FS_OPENDIR_C_INL */
