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
#ifndef GUARD_DEEMON_FS_VIRTUAL_FS_GETOWN_C_INL
#define GUARD_DEEMON_FS_VIRTUAL_FS_GETOWN_C_INL 1
#define DEE_EXTENSION 1

#include <deemon/__conf.inl>
#include "vfs_core.h"
#include "virtual_fs.h"
#include "native_hooks.h"
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/type.h>

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8GetOwn(
 DEE_A_IN_Z Dee_Utf8Char const *path,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 struct DeeVFSNode *cwd,*filenode; int error;
 if (DeeVFS_Utf8IsAbsoluteNativePath(path)) {
call_native: return DeeHFS_Utf8GetOwn(path,owner,group);
 }
 if (DeeVFS_Utf8IsVirtualPath(path)) {
  filenode = DeeVFS_Utf8Locate(path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  filenode = DeeVFS_Utf8LocateWithCWD(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) return -1;
 error = DeeVFSNode_GetOwn(filenode,owner,group);
 DeeVFSNode_DECREF(filenode);
 return error;
}
DEE_A_RET_EXCEPT(-1) int DeeVFS_WideGetOwn(
 DEE_A_IN_Z Dee_WideChar const *path,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 struct DeeVFSNode *cwd,*filenode; int error;
 if (DeeVFS_WideIsAbsoluteNativePath(path)) {
call_native: return DeeHFS_WideGetOwn(path,owner,group);
 }
 if (DeeVFS_WideIsVirtualPath(path)) {
  filenode = DeeVFS_WideLocate(path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  filenode = DeeVFS_WideLocateWithCWD(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) return -1;
 error = DeeVFSNode_GetOwn(filenode,owner,group);
 DeeVFSNode_DECREF(filenode);
 return error;
}
DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8GetOwnObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 struct DeeVFSNode *cwd,*filenode; int error;
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
 if (DeeVFS_Utf8IsAbsoluteNativePathObject(path)) {
call_native: return DeeHFS_Utf8GetOwnObject(path,owner,group);
 }
 if (DeeVFS_Utf8IsVirtualPathObject(path)) {
  filenode = DeeVFS_Utf8LocateObject(path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  filenode = DeeVFS_Utf8LocateWithCWDObject(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) return -1;
 error = DeeVFSNode_GetOwn(filenode,owner,group);
 DeeVFSNode_DECREF(filenode);
 return error;
}
DEE_A_RET_EXCEPT(-1) int DeeVFS_WideGetOwnObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *path,
 DEE_A_OUT Dee_uid_t *owner, DEE_A_OUT Dee_gid_t *group) {
 struct DeeVFSNode *cwd,*filenode; int error;
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 if (DeeVFS_WideIsAbsoluteNativePathObject(path)) {
call_native: return DeeHFS_WideGetOwnObject(path,owner,group);
 }
 if (DeeVFS_WideIsVirtualPathObject(path)) {
  filenode = DeeVFS_WideLocateObject(path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  filenode = DeeVFS_WideLocateWithCWDObject(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) return -1;
 error = DeeVFSNode_GetOwn(filenode,owner,group);
 DeeVFSNode_DECREF(filenode);
 return error;
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_VIRTUAL_FS_GETOWN_C_INL */
