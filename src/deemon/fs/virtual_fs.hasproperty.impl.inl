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
#include "virtual_fs.hasproperty.c.inl"
#define WIDE
#define OBJECT
#endif

#ifdef WIDE
#define VFS_PREFIX(name) DeeVFS_Wide##name
#define NFS_PREFIX(name) DeeNFS_Wide##name
#else
#define VFS_PREFIX(name) DeeVFS_Utf8##name
#define NFS_PREFIX(name) DeeNFS_Utf8##name
#endif
#ifdef OBJECT
#define VFS_FUNC(name)   VFS_PREFIX(name##Object)
#define NFS_FUNC(name)   NFS_PREFIX(name##Object)
#else
#define VFS_FUNC         VFS_PREFIX
#define NFS_FUNC         NFS_PREFIX
#endif

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT_FAIL(-1,0) int VFS_FUNC(HasProperty)(
#ifdef OBJECT
#ifdef WIDE
 DEE_A_IN_OBJECT(DeeWideStringObject) const *path,
#else
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path,
#endif
#else
#ifdef WIDE
 DEE_A_IN_Z Dee_WideChar const *path,
#else
 DEE_A_IN_Z Dee_Utf8Char const *path,
#endif
#endif
 DEE_A_IN Dee_fileproperty_t prop) {
 struct DeeVFSNode *cwd,*filenode; int error;
#ifdef OBJECT
 DEE_ASSERT(DeeObject_Check(path));
#ifdef WIDE
 DEE_ASSERT(DeeWideString_Check(path));
#else
 DEE_ASSERT(DeeUtf8String_Check(path));
#endif
#else
 DEE_ASSERT(path);
#endif
 if (prop == DEE_FILEPROPERTY_ISABS) {
  return VFS_FUNC(IsAbsoluteNativePath)(path)
      || VFS_FUNC(IsVirtualPath)(path);
 }
 if (VFS_FUNC(IsAbsoluteNativePath)(path)) {
call_native: return NFS_FUNC(HasProperty)(path,prop);
 }
 if (VFS_FUNC(IsVirtualPath)(path)) {
  filenode = VFS_FUNC(LLocate)(path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  // if (DeeVFSNode_IsNative(cwd)) { DeeVFSNode_DECREF(cwd); goto call_native; }
  filenode = VFS_FUNC(LLocateWithCWD)(cwd,path);
  DeeVFSNode_DECREF(cwd);
 }
 if DEE_UNLIKELY(!filenode) { DeeError_HandledOne(); return 0; }
 switch (prop) {
  case DEE_FILEPROPERTY_ISFILE      : error = DeeVFSNode_IsFile(filenode); break;
  case DEE_FILEPROPERTY_ISDIR       : error = DeeVFSNode_IsDir(filenode); break;
  case DEE_FILEPROPERTY_ISLINK      : error = DeeVFSNode_IsLink(filenode); break;
  case DEE_FILEPROPERTY_ISDRIVE     : error = DeeVFSNode_IsDrive(filenode); break;
  case DEE_FILEPROPERTY_ISMOUNT     : error = DeeVFSNode_IsMount(filenode); break;
  case DEE_FILEPROPERTY_ISHIDDEN    : error = DeeVFSNode_IsHidden(filenode); break;
  case DEE_FILEPROPERTY_ISEXECUTABLE: error = DeeVFSNode_IsExecutable(filenode); break;
  case DEE_FILEPROPERTY_ISCHARDEV   : error = DeeVFSNode_IsCharDev(filenode); break;
  case DEE_FILEPROPERTY_ISBLOCKDEV  : error = DeeVFSNode_IsBlockDev(filenode); break;
  case DEE_FILEPROPERTY_ISFIFO      : error = DeeVFSNode_IsFiFo(filenode); break;
  case DEE_FILEPROPERTY_ISSOCKET    : error = DeeVFSNode_IsSocket(filenode); break;
  case DEE_FILEPROPERTY_EXISTS      : error = 1; break;
  default: error = 0; break;
 }
 DeeVFSNode_DECREF(filenode);
 return error;
}

DEE_DECL_END

#undef NFS_FUNC
#undef VFS_FUNC
#undef VFS_PREFIX
#undef NFS_PREFIX

#ifdef WIDE
#undef WIDE
#endif
#ifdef OBJECT
#undef OBJECT
#endif

