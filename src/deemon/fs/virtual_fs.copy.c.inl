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
#ifndef GUARD_DEEMON_FS_VIRTUAL_FS_COPY_C_INL
#define GUARD_DEEMON_FS_VIRTUAL_FS_COPY_C_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#if DEE_CONFIG_RUNTIME_HAVE_VFS2
#include <deemon/string.h>
#include <deemon/type.h>
#include <deemon/fs/native_fs.h>
#include <deemon/fs/virtual_fs.h>
#include <deemon/vfs/vfs_core.h>

DEE_DECL_BEGIN

// TODO: Copying a virtual file into the real FS should be allowed!

DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8Copy(
 DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) {
 DeeObject *native_src,*native_dst; struct DeeVFSNode *cwd; int error;
 DEE_ASSERT(src); DEE_ASSERT(dst);
 if (DeeVFS_Utf8IsAbsoluteNativePath(src)
  && DeeVFS_Utf8IsAbsoluteNativePath(dst)) {
call_native: return DeeNFS_Utf8Copy(src,dst);
 }
 if (DeeVFS_Utf8IsVirtualPath(src)) {
  if ((native_src = DeeVFS_Utf8ForceNativeRootPath(src)) == NULL) return -1;
 } else native_src = NULL;
 if (DeeVFS_Utf8IsVirtualPath(dst)) {
  if ((native_dst = DeeVFS_Utf8ForceNativeRootPath(dst)) == NULL) return -1;
 } else native_dst = NULL;
 if (!native_src) {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_dst:
   if (!native_dst) goto call_native;
   error = DeeNFS_Utf8Copy(src,DeeUtf8String_STR(native_dst));
   Dee_DECREF(native_dst);
   return error;
  }
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_dst; }
  native_src = DeeVFS_Utf8ForceNativePathWithCwd(cwd,src);
  if DEE_UNLIKELY(!native_src) { DeeVFSNode_DECREF(cwd); return -1; }
 } else cwd = NULL;
 if (!native_dst) {
  if (!cwd) {
   if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_src:
    error = DeeNFS_Utf8Copy(DeeUtf8String_STR(native_src),dst);
    Dee_DECREF(native_src);
    return error;
   }
   if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_src; }
  }
  native_dst = DeeVFS_Utf8ForceNativePathWithCwd(cwd,dst);
  DeeVFSNode_DECREF(cwd);
  if DEE_UNLIKELY(!native_dst) { error = -1; goto end_nativesrc; }
 } else if (cwd) DeeVFSNode_DECREF(cwd);
 error = DeeNFS_Utf8CopyObject(native_src,native_dst);
 Dee_DECREF(native_dst);
end_nativesrc: Dee_DECREF(native_src);
 return error;
}
DEE_A_RET_EXCEPT(-1) int DeeVFS_WideCopy(
 DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) {
 DeeObject *native_src,*native_dst; struct DeeVFSNode *cwd; int error;
 DEE_ASSERT(src); DEE_ASSERT(dst);
 if (DeeVFS_WideIsAbsoluteNativePath(src)
  && DeeVFS_WideIsAbsoluteNativePath(dst)) {
call_native: return DeeNFS_WideCopy(src,dst);
 }
 if (DeeVFS_WideIsVirtualPath(src)) {
  if ((native_src = DeeVFS_WideForceNativeRootPath(src)) == NULL) return -1;
 } else native_src = NULL;
 if (DeeVFS_WideIsVirtualPath(dst)) {
  if ((native_dst = DeeVFS_WideForceNativeRootPath(dst)) == NULL) return -1;
 } else native_dst = NULL;
 if (!native_src) {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_dst:
   if (!native_dst) goto call_native;
   error = DeeNFS_WideCopy(src,DeeWideString_STR(native_dst));
   Dee_DECREF(native_dst);
   return error;
  }
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_dst; }
  native_src = DeeVFS_WideForceNativePathWithCwd(cwd,src);
  if DEE_UNLIKELY(!native_src) { DeeVFSNode_DECREF(cwd); return -1; }
 } else cwd = NULL;
 if (!native_dst) {
  if (!cwd) {
   if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_src:
    error = DeeNFS_WideCopy(DeeWideString_STR(native_src),dst);
    Dee_DECREF(native_src);
    return error;
   }
   if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_src; }
  }
  native_dst = DeeVFS_WideForceNativePathWithCwd(cwd,dst);
  DeeVFSNode_DECREF(cwd);
  if DEE_UNLIKELY(!native_dst) { error = -1; goto end_nativesrc; }
 } else if (cwd) DeeVFSNode_DECREF(cwd);
 error = DeeNFS_WideCopyObject(native_src,native_dst);
 Dee_DECREF(native_dst);
end_nativesrc: Dee_DECREF(native_src);
 return error;
}
DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8CopyObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src,
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst) {
 DeeObject *native_src,*native_dst; struct DeeVFSNode *cwd; int error;
 DEE_ASSERT(DeeObject_Check(src) && DeeUtf8String_Check(src));
 DEE_ASSERT(DeeObject_Check(dst) && DeeUtf8String_Check(dst));
 if (DeeVFS_Utf8IsAbsoluteNativePathObject(src)
  && DeeVFS_Utf8IsAbsoluteNativePathObject(dst)) {
call_native: return DeeNFS_Utf8CopyObject(src,dst);
 }
 if (DeeVFS_Utf8IsVirtualPathObject(src)) {
  if ((native_src = DeeVFS_Utf8ForceNativeRootPathObject(src)) == NULL) return -1;
 } else native_src = NULL;
 if (DeeVFS_Utf8IsVirtualPathObject(dst)) {
  if ((native_dst = DeeVFS_Utf8ForceNativeRootPathObject(dst)) == NULL) return -1;
 } else native_dst = NULL;
 if (!native_src) {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_dst:
   if (!native_dst) goto call_native;
   error = DeeNFS_Utf8CopyObject(src,native_dst);
   Dee_DECREF(native_dst);
   return error;
  }
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_dst; }
  native_src = DeeVFS_Utf8ForceNativePathWithCwdObject(cwd,src);
  if DEE_UNLIKELY(!native_src) { DeeVFSNode_DECREF(cwd); return -1; }
 } else cwd = NULL;
 if (!native_dst) {
  if (!cwd) {
   if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_src:
    error = DeeNFS_Utf8CopyObject(native_src,dst);
    Dee_DECREF(native_src);
    return error;
   }
   if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_src; }
  }
  native_dst = DeeVFS_Utf8ForceNativePathWithCwdObject(cwd,dst);
  DeeVFSNode_DECREF(cwd);
  if DEE_UNLIKELY(!native_dst) { error = -1; goto end_nativesrc; }
 } else if (cwd) DeeVFSNode_DECREF(cwd);
 error = DeeNFS_Utf8CopyObject(native_src,native_dst);
 Dee_DECREF(native_dst);
end_nativesrc: Dee_DECREF(native_src);
 return error;
}
DEE_A_RET_EXCEPT(-1) int DeeVFS_WideCopyObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *src,
 DEE_A_IN_OBJECT(DeeWideStringObject) const *dst) {
 DeeObject *native_src,*native_dst; struct DeeVFSNode *cwd; int error;
 DEE_ASSERT(DeeObject_Check(src) && DeeWideString_Check(src));
 DEE_ASSERT(DeeObject_Check(dst) && DeeWideString_Check(dst));
 if (DeeVFS_WideIsAbsoluteNativePathObject(src)
  && DeeVFS_WideIsAbsoluteNativePathObject(dst)) {
call_native: return DeeNFS_WideCopyObject(src,dst);
 }
 if (DeeVFS_WideIsVirtualPathObject(src)) {
  if ((native_src = DeeVFS_WideForceNativeRootPathObject(src)) == NULL) return -1;
 } else native_src = NULL;
 if (DeeVFS_WideIsVirtualPathObject(dst)) {
  if ((native_dst = DeeVFS_WideForceNativeRootPathObject(dst)) == NULL) return -1;
 } else native_dst = NULL;
 if (!native_src) {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_dst:
   if (!native_dst) goto call_native;
   error = DeeNFS_WideCopyObject(src,native_dst);
   Dee_DECREF(native_dst);
   return error;
  }
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_dst; }
  native_src = DeeVFS_WideForceNativePathWithCwdObject(cwd,src);
  if DEE_UNLIKELY(!native_src) { DeeVFSNode_DECREF(cwd); return -1; }
 } else cwd = NULL;
 if (!native_dst) {
  if (!cwd) {
   if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_src:
    error = DeeNFS_WideCopyObject(native_src,dst);
    Dee_DECREF(native_src);
    return error;
   }
   if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_src; }
  }
  native_dst = DeeVFS_WideForceNativePathWithCwdObject(cwd,dst);
  DeeVFSNode_DECREF(cwd);
  if DEE_UNLIKELY(!native_dst) { error = -1; goto end_nativesrc; }
 } else if (cwd) DeeVFSNode_DECREF(cwd);
 error = DeeNFS_WideCopyObject(native_src,native_dst);
 Dee_DECREF(native_dst);
end_nativesrc: Dee_DECREF(native_src);
 return error;
}

DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */

#endif /* !GUARD_DEEMON_FS_VIRTUAL_FS_COPY_C_INL */
