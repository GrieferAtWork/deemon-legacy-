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
#ifndef GUARD_DEEMON_FS_VIRTUAL_FS_GETTIMES_C_INL
#define GUARD_DEEMON_FS_VIRTUAL_FS_GETTIMES_C_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#if DEE_CONFIG_RUNTIME_HAVE_VFS2
#include <deemon/vfs/vfs_core.h>
#include <deemon/fs/virtual_fs.h>
#include <deemon/fs/native_fs.h>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/type.h>

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8GetTimes(
 DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
 DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 struct DeeVFSNode *cwd,*filenode; int error;
 if (DeeVFS_Utf8IsAbsoluteNativePath(path)) {
call_native: return DeeNFS_Utf8GetTimes(path,atime,ctime,mtime);
 }
 if (DeeVFS_Utf8IsVirtualPath(path)) {
  filenode = DeeVFS_Utf8Locate(path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  filenode = DeeVFS_Utf8LocateWithCWD(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) return -1;
 error = DeeVFSNode_GetTimes(filenode,atime,ctime,mtime);
 DeeVFSNode_DECREF(filenode);
 return error;
}
DEE_A_RET_EXCEPT(-1) int DeeVFS_WideGetTimes(
 DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
 DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 struct DeeVFSNode *cwd,*filenode; int error;
 if (DeeVFS_WideIsAbsoluteNativePath(path)) {
call_native: return DeeNFS_WideGetTimes(path,atime,ctime,mtime);
 }
 if (DeeVFS_WideIsVirtualPath(path)) {
  filenode = DeeVFS_WideLocate(path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  filenode = DeeVFS_WideLocateWithCWD(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) return -1;
 error = DeeVFSNode_GetTimes(filenode,atime,ctime,mtime);
 DeeVFSNode_DECREF(filenode);
 return error;
}
DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8GetTimesObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
              DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 struct DeeVFSNode *cwd,*filenode; int error;
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
 if (DeeVFS_Utf8IsAbsoluteNativePathObject(path)) {
call_native: return DeeNFS_Utf8GetTimesObject(path,atime,ctime,mtime);
 }
 if (DeeVFS_Utf8IsVirtualPathObject(path)) {
  filenode = DeeVFS_Utf8LocateObject(path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  filenode = DeeVFS_Utf8LocateWithCWDObject(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) return -1;
 error = DeeVFSNode_GetTimes(filenode,atime,ctime,mtime);
 DeeVFSNode_DECREF(filenode);
 return error;
}
DEE_A_RET_EXCEPT(-1) int DeeVFS_WideGetTimesObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
              DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 struct DeeVFSNode *cwd,*filenode; int error;
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 if (DeeVFS_WideIsAbsoluteNativePathObject(path)) {
call_native: return DeeNFS_WideGetTimesObject(path,atime,ctime,mtime);
 }
 if (DeeVFS_WideIsVirtualPathObject(path)) {
  filenode = DeeVFS_WideLocateObject(path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  filenode = DeeVFS_WideLocateWithCWDObject(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) return -1;
 error = DeeVFSNode_GetTimes(filenode,atime,ctime,mtime);
 DeeVFSNode_DECREF(filenode);
 return error;
}


DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryGetTimes(
 DEE_A_IN_Z Dee_Utf8Char const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
 DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 struct DeeVFSNode *cwd,*filenode; int error;
 if (DeeVFS_Utf8IsAbsoluteNativePath(path)) {
call_native: return DeeNFS_Utf8TryGetTimes(path,atime,ctime,mtime);
 }
 if (DeeVFS_Utf8IsVirtualPath(path)) {
  filenode = DeeVFS_Utf8Locate(path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  filenode = DeeVFS_Utf8LocateWithCWD(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) { DeeError_HandledOne(); return -1; }
 error = DeeVFSNode_TryGetTimes(filenode,atime,ctime,mtime);
 DeeVFSNode_DECREF(filenode);
 return error;
}
DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryGetTimes(
 DEE_A_IN_Z Dee_WideChar const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
 DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 struct DeeVFSNode *cwd,*filenode; int error;
 if (DeeVFS_WideIsAbsoluteNativePath(path)) {
call_native: return DeeNFS_WideTryGetTimes(path,atime,ctime,mtime);
 }
 if (DeeVFS_WideIsVirtualPath(path)) {
  filenode = DeeVFS_WideLocate(path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  filenode = DeeVFS_WideLocateWithCWD(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) { DeeError_HandledOne(); return -1; }
 error = DeeVFSNode_TryGetTimes(filenode,atime,ctime,mtime);
 DeeVFSNode_DECREF(filenode);
 return error;
}
DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryGetTimesObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
              DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 struct DeeVFSNode *cwd,*filenode; int error;
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
 if (DeeVFS_Utf8IsAbsoluteNativePathObject(path)) {
call_native: return DeeNFS_Utf8TryGetTimesObject(path,atime,ctime,mtime);
 }
 if (DeeVFS_Utf8IsVirtualPathObject(path)) {
  filenode = DeeVFS_Utf8LocateObject(path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  filenode = DeeVFS_Utf8LocateWithCWDObject(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) { DeeError_HandledOne(); return -1; }
 error = DeeVFSNode_TryGetTimes(filenode,atime,ctime,mtime);
 DeeVFSNode_DECREF(filenode);
 return error;
}
DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryGetTimesObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *path, DEE_A_OUT_OPT Dee_timetick_t *atime,
              DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime) {
 struct DeeVFSNode *cwd,*filenode; int error;
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
 if (DeeVFS_WideIsAbsoluteNativePathObject(path)) {
call_native: return DeeNFS_WideTryGetTimesObject(path,atime,ctime,mtime);
 }
 if (DeeVFS_WideIsVirtualPathObject(path)) {
  filenode = DeeVFS_WideLocateObject(path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  filenode = DeeVFS_WideLocateWithCWDObject(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) { DeeError_HandledOne(); return -1; }
 error = DeeVFSNode_TryGetTimes(filenode,atime,ctime,mtime);
 DeeVFSNode_DECREF(filenode);
 return error;
}


DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */

#endif /* !GUARD_DEEMON_FS_VIRTUAL_FS_GETTIMES_C_INL */
