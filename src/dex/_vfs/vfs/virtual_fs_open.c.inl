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
#ifndef GUARD_DEEMON_FS_VIRTUAL_FS_OPEN_C_INL
#define GUARD_DEEMON_FS_VIRTUAL_FS_OPEN_C_INL 1
#ifndef DEE_EXTENSION
#define DEE_EXTENSION 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/fs/native_filefd.h>
#include <deemon/sys/sysfd.h>
#include "objects/vfs_file.h"
#include "native_hooks.h"
#include "vfs_native_node.h"

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT_REF DeeObject *DeeVFS_Utf8Open(
 DEE_A_IN_Z Dee_Utf8Char const *file,
 DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) {
 struct DeeVFSNode *cwd,*filenode; DeeObject *result;
 DEE_ASSERT(file);
 if (DeeVFS_Utf8IsAbsoluteNativePath(file)) {
call_native: return DeeHFS_Utf8Open(file,openmode,permissions);
 }
 if (DeeVFS_Utf8IsVirtualPath(file)) {
  filenode = DeeVFS_Utf8Locate(file);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  filenode = DeeVFS_Utf8LocateWithCWD(cwd,file);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) return NULL;
 result = DeeVFSNode_IsNative(filenode)
  ? DeeVFSNativeNode_HFS_Open(filenode,openmode,permissions)
  : (DeeObject *)DeeFileVFS_OpenNode(filenode,openmode,permissions);
 DeeVFSNode_DECREF(filenode);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeVFS_WideOpen(
 DEE_A_IN_Z Dee_WideChar const *file,
 DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) {
 struct DeeVFSNode *cwd,*filenode; DeeObject *result;
 DEE_ASSERT(file);
 if (DeeVFS_WideIsAbsoluteNativePath(file)) {
call_native: return DeeHFS_WideOpen(file,openmode,permissions);
 }
 if (DeeVFS_WideIsVirtualPath(file)) {
  filenode = DeeVFS_WideLocate(file);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  filenode = DeeVFS_WideLocateWithCWD(cwd,file);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) return NULL;
 result = DeeVFSNode_IsNative(filenode)
  ? DeeVFSNativeNode_HFS_Open(filenode,openmode,permissions)
  : (DeeObject *)DeeFileVFS_OpenNode(filenode,openmode,permissions);
 DeeVFSNode_DECREF(filenode);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeVFS_Utf8OpenObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *file,
 DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) {
 struct DeeVFSNode *cwd,*filenode; DeeObject *result;
 DEE_ASSERT(file);
 if (DeeVFS_Utf8IsAbsoluteNativePathObject(file)) {
call_native: return DeeHFS_Utf8OpenObject(file,openmode,permissions);
 }
 if (DeeVFS_Utf8IsVirtualPathObject(file)) {
  filenode = DeeVFS_Utf8LocateObject(file);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  filenode = DeeVFS_Utf8LocateWithCWDObject(cwd,file);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) return NULL;
 result = DeeVFSNode_IsNative(filenode)
  ? DeeVFSNativeNode_HFS_Open(filenode,openmode,permissions)
  : (DeeObject *)DeeFileVFS_OpenNode(filenode,openmode,permissions);
 DeeVFSNode_DECREF(filenode);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeVFS_WideOpenObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *file,
 DEE_A_IN Dee_openmode_t openmode, DEE_A_IN Dee_mode_t permissions) {
 struct DeeVFSNode *cwd,*filenode; DeeObject *result;
 DEE_ASSERT(file);
 if (DeeVFS_WideIsAbsoluteNativePathObject(file)) {
call_native: return DeeHFS_WideOpenObject(file,openmode,permissions);
 }
 if (DeeVFS_WideIsVirtualPathObject(file)) {
  filenode = DeeVFS_WideLocateObject(file);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  filenode = DeeVFS_WideLocateWithCWDObject(cwd,file);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) return NULL;
 result = DeeVFSNode_IsNative(filenode)
  ? DeeVFSNativeNode_HFS_Open(filenode,openmode,permissions)
  : (DeeObject *)DeeFileVFS_OpenNode(filenode,openmode,permissions);
 DeeVFSNode_DECREF(filenode);
 return result;
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_VIRTUAL_FS_OPEN_C_INL */
