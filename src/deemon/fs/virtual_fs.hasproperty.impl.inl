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
#define DO_TRY
#define OBJECT
#endif

#ifdef WIDE
#define VFS_PREFIX1(name)   DeeVFS_Wide##name
#define NFS_PREFIX1(name)   DeeNFS_Wide##name
#else
#define VFS_PREFIX1(name)   DeeVFS_Utf8##name
#define NFS_PREFIX1(name)   DeeNFS_Utf8##name
#endif
#ifdef DO_TRY
#define VFS_PREFIX2(name)   VFS_PREFIX1(Try##name)
#define NFS_PREFIX2(name)   NFS_PREFIX1(Try##name)
#else
#define VFS_PREFIX2         VFS_PREFIX1
#define NFS_PREFIX2         NFS_PREFIX1
#endif
#ifdef OBJECT
#define VFS_FUNC(name)      VFS_PREFIX2(name##Object)
#define NFS_FUNC(name)      NFS_PREFIX2(name##Object)
#define VFS_FUNCNOTRY(name) VFS_PREFIX1(name##Object)
#else
#define VFS_FUNC            VFS_PREFIX2
#define NFS_FUNC            NFS_PREFIX2
#define VFS_FUNCNOTRY       VFS_PREFIX1
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
 if (VFS_FUNCNOTRY(IsAbsoluteNativePath)(path)) {
call_native: return NFS_FUNC(HasProperty)(path,prop);
 }
 if (VFS_FUNCNOTRY(IsVirtualPath)(path)) {
  filenode = VFS_FUNCNOTRY(Locate)(path);
 } else {
  if ((cwd = DeeVFS_GetActiveCwdNode()) == NULL) goto call_native;
  filenode = VFS_FUNCNOTRY(LocateWithCWD)(cwd,path);
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
  default: error = 0; break;
 }
#ifdef DO_TRY
 if (error < 0) { DeeError_HandledOne(); error = 0; }
#endif
 DeeVFSNode_DECREF(filenode);
 return error;
}

DEE_DECL_END

#undef VFS_FUNCNOTRY
#undef VFS_FUNC
#undef VFS_PREFIX2
#undef VFS_PREFIX1
#undef NFS_FUNC
#undef NFS_PREFIX2
#undef NFS_PREFIX1

#ifdef WIDE
#undef WIDE
#endif
#ifdef DO_TRY
#undef DO_TRY
#endif
#ifdef OBJECT
#undef OBJECT
#endif

