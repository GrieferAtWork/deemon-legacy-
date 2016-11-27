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
// >> [[optional]] void DeeSysFSUtf8View_Quit(DEE_A_IN struct DeeSysFSUtf8View *self);
// >> [[optional]] bool DeeSysFSUtf8View_IsDone(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] void DeeSysFSUtf8View_Init(DEE_A_OUT struct DeeSysFSUtf8View *self, DEE_A_IN_Z Dee_Utf8Char const *path, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_InitObject(DEE_A_OUT struct DeeSysFSUtf8View *self, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path, CODE on_error);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] void DeeSysFSUtf8View_AdvanceAndReleaseOnError(DEE_A_INOUT struct DeeSysFSUtf8View *self, CODE on_error);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeSysFSUtf8View_GetPath(DEE_A_INOUT struct DeeSysFSUtf8View *self);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeSysFSUtf8View_GetFilenameObject(DEE_A_IN struct DeeSysFSUtf8View const *self);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] void DeeSysFSUtf8View_Acquire(DEE_A_INOUT struct DeeSysFSUtf8View *self);
//                 Manually acquires a lock to the view.
//                 NOTE: If the view wasn't yielded since its creation or the last lock
//                       was held, the return values of query functions will not have changed.
//                 NOTE: Accessing attributes after the view is done causes undefined behavior
// >> [[optional]] void DeeSysFSUtf8View_Release(DEE_A_INOUT struct DeeSysFSUtf8View *self);
//                 Releases a lock help by the view after no error occurred in any of the above methods.
//////////////////////////////////////////////////////////////////////////
// The following functions may only be called while a lock to the View is held
// (as returned by 'DeeSysFSUtf8View_AdvanceAndReleaseOnError' and 'DeeSysFSUtf8View_Acquire')
// NOTE: If an error occurs in any of these functions, the lock is automatically released,
//       meaning that the caller should __NOT__ attempt to release it again.
// NOTE: Though the try-versions of the methods below will never release the lock.
// >> [[optional]] Dee_Utf8Char const *DeeSysFSUtf8View_GetFilenameStrLocked(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] #define DEE_SYSFS_UTF8VIEW_GETFILENAMESTRLOCKED_NEVER_NULL
// >> [[optional]] #define DEE_SYSFS_UTF8VIEW_GETFILENAMESTRLOCKED_ZERO_TERMINATED
// >> [[optional]] Dee_size_t DeeSysFSUtf8View_GetFilenameSizeLocked(DEE_A_IN struct DeeSysFSUtf8View const *self);
// >> [[optional]] DEE_A_RET_OBJECT_NOEXCEPT_REF(DeeUtf8StringObject) *DeeSysFSUtf8View_TryGetFilenameObjectLocked(DEE_A_IN struct DeeSysFSUtf8View const *self);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] void DeeSysFSUtf8View_GetTimesAndReleaseOnError(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT_OPT Dee_timetick_t *atime, DEE_A_OUT_OPT Dee_timetick_t *ctime, DEE_A_OUT_OPT Dee_timetick_t *mtime, CODE on_error);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] void DeeSysFSUtf8View_IsFileAndReleaseOnError(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsDirAndReleaseOnError(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsLinkAndReleaseOnError(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsDriveAndReleaseOnError(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsMountAndReleaseOnError(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsHiddenAndReleaseOnError(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsExecutableAndReleaseOnError(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsCharDevAndReleaseOnError(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsBlockDevAndReleaseOnError(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsFiFoAndReleaseOnError(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
// >> [[optional]] void DeeSysFSUtf8View_IsSocketAndReleaseOnError(DEE_A_IN struct DeeSysFSUtf8View const *self, DEE_A_OUT int *result, CODE on_error);
//////////////////////////////////////////////////////////////////////////
// ... Everything again, but with every 'Utf8'/'UTF8' replaced with 'Wide'/'WIDE'
//////////////////////////////////////////////////////////////////////////


#if defined(DEE_PLATFORM_WINDOWS)
# include <deemon/sys/win32/sysfs.view.h>
#elif defined(DEE_PLATFORM_UNIX)
# include <deemon/sys/unix/sysfs.view.h>
#elif 1
// Stub: Everything is optional...
#else
# error "No Filesystem implementation for this platform"
#endif

#if !defined(DeeSysFSUtf8View_InitObject) && defined(DeeSysFSUtf8View_Init)
#define DeeSysFSUtf8View_InitObject(self,path,...) DeeSysFSUtf8View_Init(self,DeeUtf8String_STR(path),__VA_ARGS__)
#endif
#if !defined(DeeSysFSWideView_InitObject) && defined(DeeSysFSWideView_Init)
#define DeeSysFSWideView_InitObject(self,path,...) DeeSysFSWideView_Init(self,DeeWideString_STR(path),__VA_ARGS__)
#endif

#if !defined(DeeSysFSUtf8View_InitObject)
#undef DeeSysFSUtf8View
#endif
#if !defined(DeeSysFSWideView_InitObject)
#undef DeeSysFSWideView
#endif

#if !defined(DeeSysFSUtf8View_IsDone)\
 || !defined(DeeSysFSUtf8View_AdvanceAndReleaseOnError)
#undef DeeSysFSUtf8View
#endif

#if !defined(DeeSysFSWideView_IsDone)\
 || !defined(DeeSysFSWideView_AdvanceAndReleaseOnError)
#undef DeeSysFSWideView
#endif

#if !defined(DeeSysFSUtf8View_GetFilenameObject)\
 && ((!defined(DeeSysFSUtf8View_GetFilenameStrLocked)\
   && !defined(DeeSysFSUtf8View_TryGetFilenameObjectLocked))\
  || !defined(DeeSysFSUtf8View_Acquire)\
  || !defined(DeeSysFSUtf8View_Release))
#undef DeeSysFSUtf8View
#endif

#if !defined(DeeSysFSWideView_GetFilenameObject)\
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
#undef DeeSysFSWideView_AdvanceAndReleaseOnError
#undef DeeSysFSWideView_Acquire
#undef DeeSysFSWideView_Release
#endif


#endif /* !GUARD_DEEMON_SYS_SYSFS_VIEW_H */
