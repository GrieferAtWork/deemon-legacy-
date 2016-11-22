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
#ifndef GUARD_DEEMON_FS_VIRTUAL_FS_LINK_C_INL
#define GUARD_DEEMON_FS_VIRTUAL_FS_LINK_C_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#if DEE_CONFIG_RUNTIME_HAVE_VFS2
#include <deemon/string.h>
#include <deemon/type.h>
#include <deemon/fs/native_fs.h>
#include <deemon/fs/virtual_fs.h>
#include <deemon/vfs/vfs_core.h>

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8Link(
 DEE_A_IN_Z Dee_Utf8Char const *link_name, DEE_A_IN_Z Dee_Utf8Char const *target_name) {
 DeeObject *native_link_name,*native_target_name; struct DeeVFSNode *cwd; int error;
 DEE_ASSERT(link_name); DEE_ASSERT(target_name);
 if (DeeVFS_Utf8IsAbsoluteNativePath(link_name)
  && DeeVFS_Utf8IsAbsoluteNativePath(target_name)) {
call_native: return DeeNFS_Utf8Link(link_name,target_name);
 }
 if (DeeVFS_Utf8IsVirtualPath(link_name)) {
  if ((native_link_name = DeeVFS_Utf8ForceNativeRootPath(link_name)) == NULL) return -1;
 } else native_link_name = NULL;
 if (DeeVFS_Utf8IsVirtualPath(target_name)) {
  if ((native_target_name = DeeVFS_Utf8ForceNativeRootPath(target_name)) == NULL) return -1;
 } else native_target_name = NULL;
 if (!native_link_name) {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_target_name:
   if (!native_target_name) goto call_native;
   error = DeeNFS_Utf8Link(link_name,DeeUtf8String_STR(native_target_name));
   Dee_DECREF(native_target_name);
   return error;
  }
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_target_name; }
  native_link_name = DeeVFS_Utf8ForceNativePathWithCwd(cwd,link_name);
  if DEE_UNLIKELY(!native_link_name) { DeeVFSNode_DECREF(cwd); return -1; }
 } else cwd = NULL;
 if (!native_target_name) {
  if (!cwd) {
   if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_link_name:
    error = DeeNFS_Utf8Link(DeeUtf8String_STR(native_link_name),target_name);
    Dee_DECREF(native_link_name);
    return error;
   }
   if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_link_name; }
  }
  native_target_name = DeeVFS_Utf8ForceNativePathWithCwd(cwd,target_name);
  DeeVFSNode_DECREF(cwd);
  if DEE_UNLIKELY(!native_target_name) { error = -1; goto end_nativelink_name; }
 } else if (cwd) DeeVFSNode_DECREF(cwd);
 error = DeeNFS_Utf8LinkObject(native_link_name,native_target_name);
 Dee_DECREF(native_target_name);
end_nativelink_name: Dee_DECREF(native_link_name);
 return error;
}
DEE_A_RET_EXCEPT(-1) int DeeVFS_WideLink(
 DEE_A_IN_Z Dee_WideChar const *link_name, DEE_A_IN_Z Dee_WideChar const *target_name) {
 DeeObject *native_link_name,*native_target_name; struct DeeVFSNode *cwd; int error;
 DEE_ASSERT(link_name); DEE_ASSERT(target_name);
 if (DeeVFS_WideIsAbsoluteNativePath(link_name)
  && DeeVFS_WideIsAbsoluteNativePath(target_name)) {
call_native: return DeeNFS_WideLink(link_name,target_name);
 }
 if (DeeVFS_WideIsVirtualPath(link_name)) {
  if ((native_link_name = DeeVFS_WideForceNativeRootPath(link_name)) == NULL) return -1;
 } else native_link_name = NULL;
 if (DeeVFS_WideIsVirtualPath(target_name)) {
  if ((native_target_name = DeeVFS_WideForceNativeRootPath(target_name)) == NULL) return -1;
 } else native_target_name = NULL;
 if (!native_link_name) {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_target_name:
   if (!native_target_name) goto call_native;
   error = DeeNFS_WideLink(link_name,DeeWideString_STR(native_target_name));
   Dee_DECREF(native_target_name);
   return error;
  }
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_target_name; }
  native_link_name = DeeVFS_WideForceNativePathWithCwd(cwd,link_name);
  if DEE_UNLIKELY(!native_link_name) { DeeVFSNode_DECREF(cwd); return -1; }
 } else cwd = NULL;
 if (!native_target_name) {
  if (!cwd) {
   if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_link_name:
    error = DeeNFS_WideLink(DeeWideString_STR(native_link_name),target_name);
    Dee_DECREF(native_link_name);
    return error;
   }
   if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_link_name; }
  }
  native_target_name = DeeVFS_WideForceNativePathWithCwd(cwd,target_name);
  DeeVFSNode_DECREF(cwd);
  if DEE_UNLIKELY(!native_target_name) { error = -1; goto end_nativelink_name; }
 } else if (cwd) DeeVFSNode_DECREF(cwd);
 error = DeeNFS_WideLinkObject(native_link_name,native_target_name);
 Dee_DECREF(native_target_name);
end_nativelink_name: Dee_DECREF(native_link_name);
 return error;
}
DEE_A_RET_EXCEPT(-1) int DeeVFS_Utf8LinkObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *link_name,
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *target_name) {
 DeeObject *native_link_name,*native_target_name; struct DeeVFSNode *cwd; int error;
 DEE_ASSERT(DeeObject_Check(link_name) && DeeUtf8String_Check(link_name));
 DEE_ASSERT(DeeObject_Check(target_name) && DeeUtf8String_Check(target_name));
 if (DeeVFS_Utf8IsAbsoluteNativePathObject(link_name)
  && DeeVFS_Utf8IsAbsoluteNativePathObject(target_name)) {
call_native: return DeeNFS_Utf8LinkObject(link_name,target_name);
 }
 if (DeeVFS_Utf8IsVirtualPathObject(link_name)) {
  if ((native_link_name = DeeVFS_Utf8ForceNativeRootPathObject(link_name)) == NULL) return -1;
 } else native_link_name = NULL;
 if (DeeVFS_Utf8IsVirtualPathObject(target_name)) {
  if ((native_target_name = DeeVFS_Utf8ForceNativeRootPathObject(target_name)) == NULL) return -1;
 } else native_target_name = NULL;
 if (!native_link_name) {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_target_name:
   if (!native_target_name) goto call_native;
   error = DeeNFS_Utf8LinkObject(link_name,native_target_name);
   Dee_DECREF(native_target_name);
   return error;
  }
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_target_name; }
  native_link_name = DeeVFS_Utf8ForceNativePathWithCwdObject(cwd,link_name);
  if DEE_UNLIKELY(!native_link_name) { DeeVFSNode_DECREF(cwd); return -1; }
 } else cwd = NULL;
 if (!native_target_name) {
  if (!cwd) {
   if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_link_name:
    error = DeeNFS_Utf8LinkObject(native_link_name,target_name);
    Dee_DECREF(native_link_name);
    return error;
   }
   if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_link_name; }
  }
  native_target_name = DeeVFS_Utf8ForceNativePathWithCwdObject(cwd,target_name);
  DeeVFSNode_DECREF(cwd);
  if DEE_UNLIKELY(!native_target_name) { error = -1; goto end_nativelink_name; }
 } else if (cwd) DeeVFSNode_DECREF(cwd);
 error = DeeNFS_Utf8LinkObject(native_link_name,native_target_name);
 Dee_DECREF(native_target_name);
end_nativelink_name: Dee_DECREF(native_link_name);
 return error;
}
DEE_A_RET_EXCEPT(-1) int DeeVFS_WideLinkObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *link_name,
 DEE_A_IN_OBJECT(DeeWideStringObject) const *target_name) {
 DeeObject *native_link_name,*native_target_name; struct DeeVFSNode *cwd; int error;
 DEE_ASSERT(DeeObject_Check(link_name) && DeeWideString_Check(link_name));
 DEE_ASSERT(DeeObject_Check(target_name) && DeeWideString_Check(target_name));
 if (DeeVFS_WideIsAbsoluteNativePathObject(link_name)
  && DeeVFS_WideIsAbsoluteNativePathObject(target_name)) {
call_native: return DeeNFS_WideLinkObject(link_name,target_name);
 }
 if (DeeVFS_WideIsVirtualPathObject(link_name)) {
  if ((native_link_name = DeeVFS_WideForceNativeRootPathObject(link_name)) == NULL) return -1;
 } else native_link_name = NULL;
 if (DeeVFS_WideIsVirtualPathObject(target_name)) {
  if ((native_target_name = DeeVFS_WideForceNativeRootPathObject(target_name)) == NULL) return -1;
 } else native_target_name = NULL;
 if (!native_link_name) {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_target_name:
   if (!native_target_name) goto call_native;
   error = DeeNFS_WideLinkObject(link_name,native_target_name);
   Dee_DECREF(native_target_name);
   return error;
  }
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_target_name; }
  native_link_name = DeeVFS_WideForceNativePathWithCwdObject(cwd,link_name);
  if DEE_UNLIKELY(!native_link_name) { DeeVFSNode_DECREF(cwd); return -1; }
 } else cwd = NULL;
 if (!native_target_name) {
  if (!cwd) {
   if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_link_name:
    error = DeeNFS_WideLinkObject(native_link_name,target_name);
    Dee_DECREF(native_link_name);
    return error;
   }
   if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_link_name; }
  }
  native_target_name = DeeVFS_WideForceNativePathWithCwdObject(cwd,target_name);
  DeeVFSNode_DECREF(cwd);
  if DEE_UNLIKELY(!native_target_name) { error = -1; goto end_nativelink_name; }
 } else if (cwd) DeeVFSNode_DECREF(cwd);
 error = DeeNFS_WideLinkObject(native_link_name,native_target_name);
 Dee_DECREF(native_target_name);
end_nativelink_name: Dee_DECREF(native_link_name);
 return error;
}


DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryLink(
 DEE_A_IN_Z Dee_Utf8Char const *link_name, DEE_A_IN_Z Dee_Utf8Char const *target_name) {
 DeeObject *native_link_name,*native_target_name; struct DeeVFSNode *cwd; int error;
 DEE_ASSERT(link_name); DEE_ASSERT(target_name);
 if (DeeVFS_Utf8IsAbsoluteNativePath(link_name)
  && DeeVFS_Utf8IsAbsoluteNativePath(target_name)) {
call_native: return DeeNFS_Utf8TryLink(link_name,target_name);
 }
 if (DeeVFS_Utf8IsVirtualPath(link_name)) {
  if ((native_link_name = DeeVFS_Utf8ForceNativeRootPath(link_name)) == NULL) {err_h1: DeeError_HandledOne(); return -1; }
 } else native_link_name = NULL;
 if (DeeVFS_Utf8IsVirtualPath(target_name)) {
  if ((native_target_name = DeeVFS_Utf8ForceNativeRootPath(target_name)) == NULL) goto err_h1;
 } else native_target_name = NULL;
 if (!native_link_name) {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_target_name:
   if (!native_target_name) goto call_native;
   error = DeeNFS_Utf8TryLink(link_name,DeeUtf8String_STR(native_target_name));
   Dee_DECREF(native_target_name);
   return error;
  }
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_target_name; }
  native_link_name = DeeVFS_Utf8ForceNativePathWithCwd(cwd,link_name);
  if DEE_UNLIKELY(!native_link_name) { DeeVFSNode_DECREF(cwd); goto err_h1; }
 } else cwd = NULL;
 if (!native_target_name) {
  if (!cwd) {
   if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_link_name:
    error = DeeNFS_Utf8TryLink(DeeUtf8String_STR(native_link_name),target_name);
    Dee_DECREF(native_link_name);
    return error;
   }
   if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_link_name; }
  }
  native_target_name = DeeVFS_Utf8ForceNativePathWithCwd(cwd,target_name);
  DeeVFSNode_DECREF(cwd);
  if DEE_UNLIKELY(!native_target_name) { DeeError_HandledOne(); error = 0; goto end_nativelink_name; }
 } else if (cwd) DeeVFSNode_DECREF(cwd);
 error = DeeNFS_Utf8TryLinkObject(native_link_name,native_target_name);
 Dee_DECREF(native_target_name);
end_nativelink_name: Dee_DECREF(native_link_name);
 return error;
}


DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryLink(
 DEE_A_IN_Z Dee_WideChar const *link_name, DEE_A_IN_Z Dee_WideChar const *target_name) {
 DeeObject *native_link_name,*native_target_name; struct DeeVFSNode *cwd; int error;
 DEE_ASSERT(link_name); DEE_ASSERT(target_name);
 if (DeeVFS_WideIsAbsoluteNativePath(link_name)
  && DeeVFS_WideIsAbsoluteNativePath(target_name)) {
call_native: return DeeNFS_WideTryLink(link_name,target_name);
 }
 if (DeeVFS_WideIsVirtualPath(link_name)) {
  if ((native_link_name = DeeVFS_WideForceNativeRootPath(link_name)) == NULL) {err_h1: DeeError_HandledOne(); return -1; }
 } else native_link_name = NULL;
 if (DeeVFS_WideIsVirtualPath(target_name)) {
  if ((native_target_name = DeeVFS_WideForceNativeRootPath(target_name)) == NULL) goto err_h1;
 } else native_target_name = NULL;
 if (!native_link_name) {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_target_name:
   if (!native_target_name) goto call_native;
   error = DeeNFS_WideTryLink(link_name,DeeWideString_STR(native_target_name));
   Dee_DECREF(native_target_name);
   return error;
  }
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_target_name; }
  native_link_name = DeeVFS_WideForceNativePathWithCwd(cwd,link_name);
  if DEE_UNLIKELY(!native_link_name) { DeeVFSNode_DECREF(cwd); goto err_h1; }
 } else cwd = NULL;
 if (!native_target_name) {
  if (!cwd) {
   if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_link_name:
    error = DeeNFS_WideTryLink(DeeWideString_STR(native_link_name),target_name);
    Dee_DECREF(native_link_name);
    return error;
   }
   if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_link_name; }
  }
  native_target_name = DeeVFS_WideForceNativePathWithCwd(cwd,target_name);
  DeeVFSNode_DECREF(cwd);
  if DEE_UNLIKELY(!native_target_name) { DeeError_HandledOne(); error = 0; goto end_nativelink_name; }
 } else if (cwd) DeeVFSNode_DECREF(cwd);
 error = DeeNFS_WideTryLinkObject(native_link_name,native_target_name);
 Dee_DECREF(native_target_name);
end_nativelink_name: Dee_DECREF(native_link_name);
 return error;
}
DEE_A_RET_NOEXCEPT(0) int DeeVFS_Utf8TryLinkObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *link_name,
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *target_name) {
 DeeObject *native_link_name,*native_target_name; struct DeeVFSNode *cwd; int error;
 DEE_ASSERT(link_name); DEE_ASSERT(target_name);
 if (DeeVFS_Utf8IsAbsoluteNativePathObject(link_name)
  && DeeVFS_Utf8IsAbsoluteNativePathObject(target_name)) {
call_native: return DeeNFS_Utf8TryLinkObject(link_name,target_name);
 }
 if (DeeVFS_Utf8IsVirtualPathObject(link_name)) {
  if ((native_link_name = DeeVFS_Utf8ForceNativeRootPathObject(link_name)) == NULL) {err_h1: DeeError_HandledOne(); return -1; }
 } else native_link_name = NULL;
 if (DeeVFS_Utf8IsVirtualPathObject(target_name)) {
  if ((native_target_name = DeeVFS_Utf8ForceNativeRootPathObject(target_name)) == NULL) goto err_h1;
 } else native_target_name = NULL;
 if (!native_link_name) {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_target_name:
   if (!native_target_name) goto call_native;
   error = DeeNFS_Utf8TryLinkObject(link_name,native_target_name);
   Dee_DECREF(native_target_name);
   return error;
  }
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_target_name; }
  native_link_name = DeeVFS_Utf8ForceNativePathWithCwdObject(cwd,link_name);
  if DEE_UNLIKELY(!native_link_name) { DeeVFSNode_DECREF(cwd); goto err_h1; }
 } else cwd = NULL;
 if (!native_target_name) {
  if (!cwd) {
   if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_link_name:
    error = DeeNFS_Utf8TryLinkObject(native_link_name,target_name);
    Dee_DECREF(native_link_name);
    return error;
   }
   if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_link_name; }
  }
  native_target_name = DeeVFS_Utf8ForceNativePathWithCwdObject(cwd,target_name);
  DeeVFSNode_DECREF(cwd);
  if DEE_UNLIKELY(!native_target_name) { DeeError_HandledOne(); error = 0; goto end_nativelink_name; }
 } else if (cwd) DeeVFSNode_DECREF(cwd);
 error = DeeNFS_Utf8TryLinkObject(native_link_name,native_target_name);
 Dee_DECREF(native_target_name);
end_nativelink_name: Dee_DECREF(native_link_name);
 return error;
}
DEE_A_RET_NOEXCEPT(0) int DeeVFS_WideTryLinkObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *link_name,
 DEE_A_IN_OBJECT(DeeWideStringObject) const *target_name) {
 DeeObject *native_link_name,*native_target_name; struct DeeVFSNode *cwd; int error;
 DEE_ASSERT(link_name); DEE_ASSERT(target_name);
 if (DeeVFS_WideIsAbsoluteNativePathObject(link_name)
  && DeeVFS_WideIsAbsoluteNativePathObject(target_name)) {
call_native: return DeeNFS_WideTryLinkObject(link_name,target_name);
 }
 if (DeeVFS_WideIsVirtualPathObject(link_name)) {
  if ((native_link_name = DeeVFS_WideForceNativeRootPathObject(link_name)) == NULL) {err_h1: DeeError_HandledOne(); return -1; }
 } else native_link_name = NULL;
 if (DeeVFS_WideIsVirtualPathObject(target_name)) {
  if ((native_target_name = DeeVFS_WideForceNativeRootPathObject(target_name)) == NULL) goto err_h1;
 } else native_target_name = NULL;
 if (!native_link_name) {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_target_name:
   if (!native_target_name) goto call_native;
   error = DeeNFS_WideTryLinkObject(link_name,native_target_name);
   Dee_DECREF(native_target_name);
   return error;
  }
  if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_target_name; }
  native_link_name = DeeVFS_WideForceNativePathWithCwdObject(cwd,link_name);
  if DEE_UNLIKELY(!native_link_name) { DeeVFSNode_DECREF(cwd); goto err_h1; }
 } else cwd = NULL;
 if (!native_target_name) {
  if (!cwd) {
   if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) {
call_native_link_name:
    error = DeeNFS_WideTryLinkObject(native_link_name,target_name);
    Dee_DECREF(native_link_name);
    return error;
   }
   if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native_link_name; }
  }
  native_target_name = DeeVFS_WideForceNativePathWithCwdObject(cwd,target_name);
  DeeVFSNode_DECREF(cwd);
  if DEE_UNLIKELY(!native_target_name) { DeeError_HandledOne(); error = 0; goto end_nativelink_name; }
 } else if (cwd) DeeVFSNode_DECREF(cwd);
 error = DeeNFS_WideTryLinkObject(native_link_name,native_target_name);
 Dee_DECREF(native_target_name);
end_nativelink_name: Dee_DECREF(native_link_name);
 return error;
}


DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */

#endif /* !GUARD_DEEMON_FS_VIRTUAL_FS_LINK_C_INL */
