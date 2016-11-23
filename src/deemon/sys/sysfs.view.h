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
#ifndef GUARD_DEEMON_SYS_SYSFS_VIEW_H
#define GUARD_DEEMON_SYS_SYSFS_VIEW_H 1

#include <deemon/__conf.inl>

//////////////////////////////////////////////////////////////////////////
// >> [[optional]] struct DeeSysFSUtf8View { ... };
// >> [[optional]] bool DeeSysFSUtf8View_Quit(DEE_A_IN struct DeeSysFSUtf8View *self);
// >> [[optional]] bool DeeSysFSUtf8View_IsDone(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] void DeeSysFSUtf8View_TryInit(DEE_A_OUT struct DeeSysFSUtf8View *self, DEE_A_IN_Z Dee_Utf8Char const *path, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_TryInitObject(DEE_A_OUT struct DeeSysFSUtf8View *self, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_Init(DEE_A_OUT struct DeeSysFSUtf8View *self, DEE_A_IN_Z Dee_Utf8Char const *path, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_InitObject(DEE_A_OUT struct DeeSysFSUtf8View *self, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, CODE on_error);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] void DeeSysFSUtf8View_TryAdvanceAndReleaseOnError(DEE_A_INOUT struct DeeSysFSUtf8View *self, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_AdvanceAndReleaseOnError(DEE_A_INOUT struct DeeSysFSUtf8View *self, CODE on_error);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *DeeSysFSUtf8View_TryGetPath(DEE_A_INOUT struct DeeSysFSUtf8View *self);
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeSysFSUtf8View_GetPath(DEE_A_INOUT struct DeeSysFSUtf8View *self);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *DeeSysFSUtf8View_TryGetFilenameObject(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeSysFSUtf8View_GetFilenameObject(DEE_A_IN struct DeeSysFSUtf8View const *self);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] void DeeSysFSUtf8View_Acquire(DEE_A_INOUT struct DeeSysFSUtf8View *self);
//                 Manually acquires a lock to the view.
//                 NOTE: If the view wasn't yielded since its creation or the last lock
//                       was held, the return values of query functions will not have changed.
//                 NOTE: Accessing attributes after the view is done causes weak undefined behavior
// >> [[optional]] void DeeSysFSUtf8View_Release(DEE_A_INOUT struct DeeSysFSUtf8View *self);
//                 Releases a lock help by the view after no error occurred in any of the above methods.
//////////////////////////////////////////////////////////////////////////
// The following functions may only be called while a lock to the View is held
// (as returned by 'DeeSysFSUtf8View_TryAdvanceAndReleaseOnError', 'DeeSysFSUtf8View_AdvanceAndReleaseOnError' and 'DeeSysFSUtf8View_Acquire')
// NOTE: If an error occurs in any of these functions, the lock is automatically released,
//       meaning that the caller should __NOT__ attempt to release it again.
// NOTE: Though the try-versions of the methods below will never release the lock.
// >> [[optional]] Dee_Utf8Char const *DeeSysFSUtf8View_GetFilenameStrLocked(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] #define DEE_SYSFS_UTF8VIEW_GETFILENAMESTRLOCKED_NEVER_NULL
// >> [[optional]] #define DEE_SYSFS_UTF8VIEW_GETFILENAMESTRLOCKED_ZERO_TERMINATED
// >> [[optional]] Dee_size_t DeeSysFSUtf8View_GetFilenameSizeLocked(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *DeeSysFSUtf8View_TryGetFilenameObjectLocked(DEE_A_IN struct DeeSysFSUtf8View const *self);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] bool DeeSysFSUtf8View_TryGetTimes(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime);
// >> [[optional]] void DeeSysFSUtf8View_GetTimes(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime, CODE on_error);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] bool DeeSysFSUtf8View_TryIsFile(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] bool DeeSysFSUtf8View_TryIsDir(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] bool DeeSysFSUtf8View_TryIsLink(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] bool DeeSysFSUtf8View_TryIsDrive(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] bool DeeSysFSUtf8View_TryIsMount(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] bool DeeSysFSUtf8View_TryIsHidden(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] bool DeeSysFSUtf8View_TryIsExecutable(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] bool DeeSysFSUtf8View_TryIsCharDev(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] bool DeeSysFSUtf8View_TryIsBlockDev(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] bool DeeSysFSUtf8View_TryIsFiFo(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] bool DeeSysFSUtf8View_TryIsSocket(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] void DeeSysFSUtf8View_IsFile(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsDir(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsLink(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsDrive(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsMount(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsHidden(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsExecutable(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsCharDev(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsBlockDev(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsFiFo(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsSocket(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
//////////////////////////////////////////////////////////////////////////
// ... Everything again, but with every 'Utf8'/'UTF8' replaced with 'Wide'/'WIDE'
//////////////////////////////////////////////////////////////////////////


#if defined(DEE_PLATFORM_WINDOWS)
# include <deemon/sys/_win32.sysfs.view.h>
#elif defined(DEE_PLATFORM_UNIX)
# include <deemon/sys/_unix.sysfs.h>
#elif !defined(__DEEMON__)
# error "No Filesystem implementation for this platform"
#endif

#if !defined(DeeSysFSUtf8View_InitObject)\
 && !defined(DeeSysFSUtf8View_TryInitObject)
#undef DeeSysFSUtf8View
#endif

#if !defined(DeeSysFSUtf8View_InitObject)\
  && defined(DeeSysFSUtf8View_TryInitObject)
#define DeeSysFSUtf8View_InitObject DeeSysFSUtf8View_TryInitObject
#endif

#if !defined(DeeSysFSWideView_InitObject)\
 && !defined(DeeSysFSWideView_TryInitObject)
#undef DeeSysFSWideView
#endif

#if !defined(DeeSysFSWideView_InitObject)\
  && defined(DeeSysFSWideView_TryInitObject)
#define DeeSysFSWideView_InitObject DeeSysFSWideView_TryInitObject
#endif

#if !defined(DeeSysFSUtf8View_IsDone)\
 || !defined(DeeSysFSUtf8View_TryAdvanceAndReleaseOnError)
#undef DeeSysFSUtf8View
#endif

#if !defined(DeeSysFSWideView_IsDone)\
 || !defined(DeeSysFSWideView_TryAdvanceAndReleaseOnError)
#undef DeeSysFSWideView
#endif

#if !defined(DeeSysFSUtf8View_TryGetFilenameObject)\
 && !defined(DeeSysFSUtf8View_GetFilenameObject)\
 && ((!defined(DeeSysFSUtf8View_GetFilenameStrLocked)\
   && !defined(DeeSysFSUtf8View_TryGetFilenameObjectLocked))\
  || !defined(DeeSysFSUtf8View_Acquire)\
  || !defined(DeeSysFSUtf8View_Release))
#undef DeeSysFSUtf8View
#endif

#if !defined(DeeSysFSWideView_TryGetFilenameObject)\
 && !defined(DeeSysFSWideView_GetFilenameObject)\
 && ((!defined(DeeSysFSWideView_GetFilenameStrLocked)\
   && !defined(DeeSysFSWideView_TryGetFilenameObjectLocked))\
  || !defined(DeeSysFSWideView_Acquire)\
  || !defined(DeeSysFSWideView_Release))
#undef DeeSysFSWideView
#endif

#if !defined(DeeSysFSUtf8View_Acquire) || !defined(DeeSysFSUtf8View_Release)
#undef DeeSysFSUtf8View_GetFilenameStrLocked
#undef DEE_SYSFS_UTF8VIEW_GETFILENAMESTRLOCKED_NEVER_NULL
#undef DEE_SYSFS_UTF8VIEW_GETFILENAMESTRLOCKED_ZERO_TERMINATED
#undef DeeSysFSUtf8View_GetFilenameSizeLocked
#undef DeeSysFSUtf8View_TryGetFilenameObjectLocked
#undef DeeSysFSUtf8View_TryAdvanceAndReleaseOnError
#undef DeeSysFSUtf8View_AdvanceAndReleaseOnError
#undef DeeSysFSUtf8View_Acquire
#undef DeeSysFSUtf8View_Release
#endif

#if !defined(DeeSysFSWideView_Acquire) || !defined(DeeSysFSWideView_Release)
#undef DeeSysFSWideView_GetFilenameStrLocked
#undef DEE_SYSFS_WIDEVIEW_GETFILENAMESTRLOCKED_NEVER_NULL
#undef DEE_SYSFS_WIDEVIEW_GETFILENAMESTRLOCKED_ZERO_TERMINATED
#undef DeeSysFSWideView_GetFilenameSizeLocked
#undef DeeSysFSWideView_TryGetFilenameObjectLocked
#undef DeeSysFSWideView_TryAdvanceAndReleaseOnError
#undef DeeSysFSWideView_AdvanceAndReleaseOnError
#undef DeeSysFSWideView_Acquire
#undef DeeSysFSWideView_Release
#endif

#if !defined(DeeSysFSUtf8View_TryInitObject) && defined(DeeSysFSUtf8View_TryInit)
#define DeeSysFSUtf8View_TryInitObject(self,path) DeeSysFSUtf8View_TryInit(self,DeeUtf8String_STR(path))
#endif
#if !defined(DeeSysFSUtf8View_InitObject) && defined(DeeSysFSUtf8View_Init)
#define DeeSysFSUtf8View_InitObject(self,path,...) DeeSysFSUtf8View_Init(self,DeeUtf8String_STR(path),__VA_ARGS__)
#endif
#if !defined(DeeSysFSWideView_TryInitObject) && defined(DeeSysFSWideView_TryInit)
#define DeeSysFSWideView_TryInitObject(self,path) DeeSysFSWideView_TryInit(self,DeeWideString_STR(path))
#endif
#if !defined(DeeSysFSWideView_InitObject) && defined(DeeSysFSWideView_Init)
#define DeeSysFSWideView_InitObject(self,path,...) DeeSysFSWideView_Init(self,DeeWideString_STR(path),__VA_ARGS__)
#endif

#if !defined(DeeSysFSUtf8View_AdvanceAndReleaseOnError) && defined(DeeSysFSUtf8View_TryAdvanceAndReleaseOnError)
#define DeeSysFSUtf8View_AdvanceAndReleaseOnError(self,on_done,...) DeeSysFSUtf8View_TryAdvanceAndReleaseOnError(self,on_done,on_done)
#endif
#if !defined(DeeSysFSWideView_AdvanceAndReleaseOnError) && defined(DeeSysFSWideView_TryAdvanceAndReleaseOnError)
#define DeeSysFSWideView_AdvanceAndReleaseOnError(self,on_done,...) DeeSysFSWideView_TryAdvanceAndReleaseOnError(self,on_done,on_done)
#endif

#if defined(DeeSysFSUtf8View_TryIsFile) && !defined(DeeSysFSUtf8View_IsFile)
#define DeeSysFSUtf8View_IsFile(self,result,...) do{*(result)=DeeSysFSUtf8View_TryIsFile(self);}while(0)
#endif
#if defined(DeeSysFSUtf8View_TryIsDir) && !defined(DeeSysFSUtf8View_IsDir)
#define DeeSysFSUtf8View_IsDir(self,result,...) do{*(result)=DeeSysFSUtf8View_TryIsDir(self);}while(0)
#endif
#if defined(DeeSysFSUtf8View_TryIsLink) && !defined(DeeSysFSUtf8View_IsLink)
#define DeeSysFSUtf8View_IsLink(self,result,...) do{*(result)=DeeSysFSUtf8View_TryIsLink(self);}while(0)
#endif
#if defined(DeeSysFSUtf8View_TryIsDrive) && !defined(DeeSysFSUtf8View_IsDrive)
#define DeeSysFSUtf8View_IsDrive(self,result,...) do{*(result)=DeeSysFSUtf8View_TryIsDrive(self);}while(0)
#endif
#if defined(DeeSysFSUtf8View_TryIsMount) && !defined(DeeSysFSUtf8View_IsMount)
#define DeeSysFSUtf8View_IsMount(self,result,...) do{*(result)=DeeSysFSUtf8View_TryIsMount(self);}while(0)
#endif
#if defined(DeeSysFSUtf8View_TryIsHidden) && !defined(DeeSysFSUtf8View_IsHidden)
#define DeeSysFSUtf8View_IsHidden(self,result,...) do{*(result)=DeeSysFSUtf8View_TryIsHidden(self);}while(0)
#endif
#if defined(DeeSysFSUtf8View_TryIsExecutable) && !defined(DeeSysFSUtf8View_IsExecutable)
#define DeeSysFSUtf8View_IsExecutable(self,result,...) do{*(result)=DeeSysFSUtf8View_TryIsExecutable(self);}while(0)
#endif
#if defined(DeeSysFSUtf8View_TryIsCharDev) && !defined(DeeSysFSUtf8View_IsCharDev)
#define DeeSysFSUtf8View_IsCharDev(self,result,...) do{*(result)=DeeSysFSUtf8View_TryIsCharDev(self);}while(0)
#endif
#if defined(DeeSysFSUtf8View_TryIsBlockDev) && !defined(DeeSysFSUtf8View_IsBlockDev)
#define DeeSysFSUtf8View_IsBlockDev(self,result,...) do{*(result)=DeeSysFSUtf8View_TryIsBlockDev(self);}while(0)
#endif
#if defined(DeeSysFSUtf8View_TryIsFiFo) && !defined(DeeSysFSUtf8View_IsFiFo)
#define DeeSysFSUtf8View_IsFiFo(self,result,...) do{*(result)=DeeSysFSUtf8View_TryIsFiFo(self);}while(0)
#endif
#if defined(DeeSysFSUtf8View_TryIsSocket) && !defined(DeeSysFSUtf8View_IsSocket)
#define DeeSysFSUtf8View_IsSocket(self,result,...) do{*(result)=DeeSysFSUtf8View_TryIsSocket(self);}while(0)
#endif

#if defined(DeeSysFSWideView_TryIsFile) && !defined(DeeSysFSWideView_IsFile)
#define DeeSysFSWideView_IsFile(self,result,...) do{*(result)=DeeSysFSWideView_TryIsFile(self);}while(0)
#endif
#if defined(DeeSysFSWideView_TryIsDir) && !defined(DeeSysFSWideView_IsDir)
#define DeeSysFSWideView_IsDir(self,result,...) do{*(result)=DeeSysFSWideView_TryIsDir(self);}while(0)
#endif
#if defined(DeeSysFSWideView_TryIsLink) && !defined(DeeSysFSWideView_IsLink)
#define DeeSysFSWideView_IsLink(self,result,...) do{*(result)=DeeSysFSWideView_TryIsLink(self);}while(0)
#endif
#if defined(DeeSysFSWideView_TryIsDrive) && !defined(DeeSysFSWideView_IsDrive)
#define DeeSysFSWideView_IsDrive(self,result,...) do{*(result)=DeeSysFSWideView_TryIsDrive(self);}while(0)
#endif
#if defined(DeeSysFSWideView_TryIsMount) && !defined(DeeSysFSWideView_IsMount)
#define DeeSysFSWideView_IsMount(self,result,...) do{*(result)=DeeSysFSWideView_TryIsMount(self);}while(0)
#endif
#if defined(DeeSysFSWideView_TryIsHidden) && !defined(DeeSysFSWideView_IsHidden)
#define DeeSysFSWideView_IsHidden(self,result,...) do{*(result)=DeeSysFSWideView_TryIsHidden(self);}while(0)
#endif
#if defined(DeeSysFSWideView_TryIsExecutable) && !defined(DeeSysFSWideView_IsExecutable)
#define DeeSysFSWideView_IsExecutable(self,result,...) do{*(result)=DeeSysFSWideView_TryIsExecutable(self);}while(0)
#endif
#if defined(DeeSysFSWideView_TryIsCharDev) && !defined(DeeSysFSWideView_IsCharDev)
#define DeeSysFSWideView_IsCharDev(self,result,...) do{*(result)=DeeSysFSWideView_TryIsCharDev(self);}while(0)
#endif
#if defined(DeeSysFSWideView_TryIsBlockDev) && !defined(DeeSysFSWideView_IsBlockDev)
#define DeeSysFSWideView_IsBlockDev(self,result,...) do{*(result)=DeeSysFSWideView_TryIsBlockDev(self);}while(0)
#endif
#if defined(DeeSysFSWideView_TryIsFiFo) && !defined(DeeSysFSWideView_IsFiFo)
#define DeeSysFSWideView_IsFiFo(self,result,...) do{*(result)=DeeSysFSWideView_TryIsFiFo(self);}while(0)
#endif
#if defined(DeeSysFSWideView_TryIsSocket) && !defined(DeeSysFSWideView_IsSocket)
#define DeeSysFSWideView_IsSocket(self,result,...) do{*(result)=DeeSysFSWideView_TryIsSocket(self);}while(0)
#endif


#endif /* !GUARD_DEEMON_SYS_SYSFS_VIEW_H */
