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
#define DEESTRINGOBJECT DeeWideStringObject
#define DeeString_F(x)  DeeWideString_##x
#define DeeVFS_F(x)     DeeVFS_Wide##x
#define DeeSysFS_F(x)   DeeSysFS_Wide##x
#define DEE_CHAR        Dee_WideChar
#else
#define DEESTRINGOBJECT DeeUtf8StringObject
#define DeeString_F(x)  DeeUtf8String_##x
#define DeeVFS_F(x)     DeeVFS_Utf8##x
#define DeeSysFS_F(x)   DeeSysFS_Utf8##x
#define DEE_CHAR        Dee_Utf8Char
#endif


DEE_DECL_BEGIN

DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *DeeVFS_F(GetCwd)(void)
{
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
 return DeeSysFS_F(GetCwd)();
}
DEE_A_RET_EXCEPT(-1) int DeeVFS_F(Chdir)(DEE_A_IN_Z DEE_CHAR const *path) {
#ifdef WIDE
 DeeObject *charpath;
#endif
 struct DeeVFSNode *oldcwd,*newcwd; int error;
 if (DeeVFS_F(IsVirtualPath)(path)) { // Absolute virtual path
#ifdef WIDE
  if DEE_UNLIKELY((charpath = DeeString_FromWideString(path)) == NULL) return -1;
  newcwd = DeeVFS_Locate(DeeString_STR(charpath));
  Dee_DECREF(charpath);
#else
  newcwd = DeeVFS_Locate(path);
#endif
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
  DeeSysFS_F(Chdir)(path,return -1);
  return 0;
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
#ifdef WIDE
 if DEE_UNLIKELY((charpath = DeeString_FromWideString(path)) == NULL) return -1;
 newcwd = DeeVFS_LocateAt(oldcwd,DeeString_STR(charpath));
 Dee_DECREF(charpath);
#else
 newcwd = DeeVFS_LocateAt(oldcwd,path);
#endif
 if DEE_UNLIKELY(!newcwd) { DeeVFSNode_DECREF(oldcwd); return -1; }
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
#ifdef WIDE
 DeeObject *charpath;
#endif
 struct DeeVFSNode *cwd,*resultpath; DeeObject *result;
 if (DeeVFS_F(IsAbsoluteNativePath)(DeeString_F(STR)(path))) {
  // Simple case: 'path' is an absolute, native filesystem path --> Just re-return it
  DeeReturn_NEWREF(path);
 }
 if ((cwd = DeeVFS_GetCwdNode()) == NULL) {
  // No virtual CWD node set --> 'path' must be relative within the native filesystem
  // In this situation, we're allowed to return a relative native filesystem path.
  DeeReturn_NEWREF(path);
 }
 // Expand the given path based on what we just learned from 'cwd'
#ifdef WIDE
 if DEE_UNLIKELY((charpath = DeeString_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL)
 { DeeVFSNode_DECREF(cwd); return NULL; }
 resultpath = DeeVFS_LocateAt(cwd,DeeString_STR(charpath));
 Dee_DECREF(charpath);
#else
 resultpath = DeeVFS_LocateAt(cwd,DeeString_STR(path));
#endif
 DeeVFSNode_DECREF(cwd);
 if DEE_UNLIKELY(!resultpath) return NULL;
 if DEE_UNLIKELY(!DeeVFSNode_IsNative(resultpath)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "Expected native path, but %r describes virtual path %R",
                      path,DeeVFSNode_Filename(resultpath));
  DeeVFSNode_DECREF(resultpath);
  return NULL;
 }
#ifdef WIDE
 result = DeeWideString_FromStringWithLength(
  DeeString_SIZE(((DeeVFSNativeNode *)resultpath)->vnn_path),
  DeeString_STR(((DeeVFSNativeNode *)resultpath)->vnn_path));
#else
 Dee_INCREF(result = (DeeObject *)((DeeVFSNativeNode *)resultpath)->vnn_path);
#endif
 DeeVFSNode_DECREF(resultpath);
 return result;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DEESTRINGOBJECT) *
DeeVFS_F(ForceNativePath)(DEE_A_IN_Z DEE_CHAR const *path) {
#ifdef WIDE
 DeeObject *charpath;
#endif
 struct DeeVFSNode *cwd,*resultpath; DeeObject *result;
 if (DeeVFS_F(IsAbsoluteNativePath)(path)) {
  // Simple case: 'path' is an absolute, native filesystem path --> Just re-return it
  return DeeString_F(New)(path);
 }
 if ((cwd = DeeVFS_GetCwdNode()) == NULL) {
  // No virtual CWD node set --> 'path' must be relative within the native filesystem
  // In this situation, we're allowed to return a relative native filesystem path.
  return DeeString_F(New)(path);
 }
 // Expand the given path based on what we just learned from 'cwd'
#ifdef WIDE
 if DEE_UNLIKELY((charpath = DeeString_FromWideString(path)) == NULL)
 { DeeVFSNode_DECREF(cwd); return NULL; }
 resultpath = DeeVFS_LocateAt(cwd,DeeString_STR(charpath));
 Dee_DECREF(charpath);
#else
 resultpath = DeeVFS_LocateAt(cwd,path);
#endif
 DeeVFSNode_DECREF(cwd);
 if DEE_UNLIKELY(!resultpath) return NULL;
 if DEE_UNLIKELY(!DeeVFSNode_IsNative(resultpath)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
#ifdef WIDE
                      "Expected native path, but %lq describes virtual path %R",
#else
                      "Expected native path, but %q describes virtual path %R",
#endif
                      path,DeeVFSNode_Filename(resultpath));
  DeeVFSNode_DECREF(resultpath);
  return NULL;
 }
#ifdef WIDE
 result = DeeWideString_FromStringWithLength(
  DeeString_SIZE(((DeeVFSNativeNode *)resultpath)->vnn_path),
  DeeString_STR(((DeeVFSNativeNode *)resultpath)->vnn_path));
#else
 Dee_INCREF(result = (DeeObject *)((DeeVFSNativeNode *)resultpath)->vnn_path);
#endif
 DeeVFSNode_DECREF(resultpath);
 return result;
}


#undef DEESTRINGOBJECT
#undef DeeString_F
#undef DeeVFS_F
#undef DeeSysFS_F
#undef DEE_CHAR
#ifdef WIDE
#undef WIDE
#endif

DEE_DECL_END
