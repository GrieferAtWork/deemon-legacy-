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
#ifndef GUARD_DEEMON_FS_VIRTUAL_FS_READLINK_C_INL
#define GUARD_DEEMON_FS_VIRTUAL_FS_READLINK_C_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#if DEE_CONFIG_RUNTIME_HAVE_VFS
#include <deemon/string.h>
#include <deemon/type.h>
#include <deemon/fs/native_fs.h>
#include <deemon/fs/virtual_fs.h>
#include <deemon/vfs/vfs_root.h>
#include <deemon/vfs/vfs_core.h>
#include <deemon/vfs/vfs_native_node.h>

DEE_DECL_BEGIN

DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeVFS_Utf8Readlink(DEE_A_IN_Z Dee_Utf8Char const *path) {
 struct DeeVFSNode *cwd,*node; DeeObject *result;
 DEE_ASSERT(path);
 if (DeeVFS_Utf8IsAbsoluteNativePath(path)) {
call_native: return DeeNFS_Utf8Readlink(path);
 }
 if (DeeVFS_Utf8IsVirtualPath(path)) {
  do ++path; while (DEE_VFS_ISSEP(*path));
  node = DeeVFS_Utf8LLocateAt(DeeVFS_Root,path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native; }
  node = DeeVFS_Utf8LLocateAt(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!node) return NULL;
 result = DeeVFSNode_Utf8Readlink(node);
 DeeVFSNode_DECREF(node);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeVFS_WideReadlink(DEE_A_IN_Z Dee_WideChar const *path) {
 struct DeeVFSNode *cwd,*node; DeeObject *result;
 DEE_ASSERT(path);
 if (DeeVFS_WideIsAbsoluteNativePath(path)) {
call_native: return DeeNFS_WideReadlink(path);
 }
 if (DeeVFS_WideIsVirtualPath(path)) {
  do ++path; while (DEE_VFS_ISSEP(*path));
  node = DeeVFS_WideLLocateAt(DeeVFS_Root,path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native; }
  node = DeeVFS_WideLLocateAt(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!node) return NULL;
 result = DeeVFSNode_WideReadlink(node);
 DeeVFSNode_DECREF(node);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeVFS_Utf8ReadlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
 struct DeeVFSNode *cwd,*node; DeeObject *result;
 DEE_ASSERT(path);
 if (DeeVFS_Utf8IsAbsoluteNativePathObject(path)) {
call_native: return DeeNFS_Utf8ReadlinkObject(path);
 }
 if (DeeVFS_Utf8IsVirtualPathObject(path)) {
  Dee_Utf8Char const *path_begin = DeeUtf8String_STR(path);
  do ++path_begin; while (DEE_VFS_ISSEP(*path_begin));
  node = DeeVFS_Utf8LLocateAt(DeeVFS_Root,path_begin);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native; }
  node = DeeVFS_Utf8LLocateAtObject(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!node) return NULL;
 result = DeeVFSNode_Utf8Readlink(node);
 DeeVFSNode_DECREF(node);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeVFS_WideReadlinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 struct DeeVFSNode *cwd,*node; DeeObject *result;
 DEE_ASSERT(path);
 if (DeeVFS_WideIsAbsoluteNativePathObject(path)) {
call_native: return DeeNFS_WideReadlinkObject(path);
 }
 if (DeeVFS_WideIsVirtualPathObject(path)) {
  Dee_WideChar const *path_begin = DeeWideString_STR(path);
  do ++path_begin; while (DEE_VFS_ISSEP(*path_begin));
  node = DeeVFS_WideLLocateAt(DeeVFS_Root,path_begin);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native; }
  node = DeeVFS_WideLLocateAtObject(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!node) return NULL;
 result = DeeVFSNode_WideReadlink(node);
 DeeVFSNode_DECREF(node);
 return result;
}

DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#endif /* !GUARD_DEEMON_FS_VIRTUAL_FS_READLINK_C_INL */
