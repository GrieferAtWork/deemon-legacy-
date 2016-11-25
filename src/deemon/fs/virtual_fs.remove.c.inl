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
#ifndef GUARD_DEEMON_FS_VIRTUAL_FS_REMOVE_C_INL
#define GUARD_DEEMON_FS_VIRTUAL_FS_REMOVE_C_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#if DEE_CONFIG_RUNTIME_HAVE_VFS
#include <deemon/string.h>
#include <deemon/type.h>
#include <deemon/fs/native_fs.h>
#include <deemon/fs/virtual_fs.h>
#include <deemon/vfs/vfs_core.h>

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8Remove(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DeeObject *native_path; struct DeeVFSNode *cwd; int error;
 DEE_ASSERT(path);
 if (DeeVFS_Utf8IsAbsoluteNativePath(path)) {
call_native: return DeeNFS_Utf8Remove(path);
 }
 if (DeeVFS_Utf8IsVirtualPath(path)) {
  native_path = DeeVFS_Utf8ForceNativeRootPath(path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native; }
  native_path = DeeVFS_Utf8ForceNativePathWithCwd(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!native_path) return -1;
 error = DeeNFS_Utf8RemoveObject(native_path);
 Dee_DECREF(native_path);
 return error;
}
DEE_A_RET_EXCEPT(-1) int DeeVFS_WideRemove(DEE_A_IN_Z Dee_WideChar const *path) {
 DeeObject *native_path; struct DeeVFSNode *cwd; int error;
 DEE_ASSERT(path);
 if (DeeVFS_WideIsAbsoluteNativePath(path)) {
call_native: return DeeNFS_WideRemove(path);
 }
 if (DeeVFS_WideIsVirtualPath(path)) {
  native_path = DeeVFS_WideForceNativeRootPath(path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native; }
  native_path = DeeVFS_WideForceNativePathWithCwd(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!native_path) return -1;
 error = DeeNFS_WideRemoveObject(native_path);
 Dee_DECREF(native_path);
 return error;
}
DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8RemoveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
 DeeObject *native_path; int error;
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
 if DEE_UNLIKELY((native_path = DeeVFS_Utf8ForceNativePathObject(path)) == NULL) return -1;
 error = DeeNFS_Utf8RemoveObject(native_path);
 Dee_DECREF(native_path);
 return error;
}
DEE_A_RET_EXCEPT(-1) int DeeVFS_WideRemoveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 DeeObject *native_path; int error;
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 if DEE_UNLIKELY((native_path = DeeVFS_WideForceNativePathObject(path)) == NULL) return -1;
 error = DeeNFS_WideRemoveObject(native_path);
 Dee_DECREF(native_path);
 return error;
}

DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS */

#endif /* !GUARD_DEEMON_FS_VIRTUAL_FS_REMOVE_C_INL */
