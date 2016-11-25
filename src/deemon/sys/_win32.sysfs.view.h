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
#ifndef GUARD_DEEMON_SYS__WIN32_SYSFS_VIEW_H
#define GUARD_DEEMON_SYS__WIN32_SYSFS_VIEW_H 1

#include <deemon/__conf.inl>
#include <deemon/__xconf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/deemonrun.h>
#include <deemon/string.h>
#include <deemon/optional/atomic_mutex.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

#define DeeWin32SysFSWideViewNL DeeWin32SysFSWideViewNL
struct DeeWin32SysFSWideViewNL {
 HANDLE           w32_hview;
 WIN32_FIND_DATAW w32_viewdata;
};

#define DeeWin32SysFS_ISIGNOREDVIEWENTRY(name)\
 ((name)[0] == '.' && ((name)[1] == 0 || ((name)[1] == '.' && (name)[2] == 0)))
#define DeeWin32SysFSWideViewNL_ISIGNOREDENTRY(self)\
 DeeWin32SysFS_ISIGNOREDVIEWENTRY((self)->w32_viewdata.cFileName)

#define DeeWin32SysFSWideViewNL_InitWithLength(self,path,path_size,...)\
do{\
 Dee_WideChar *_wv_pattern,*_wv_patternend;\
 Dee_size_t _wv_path_size; DWORD _wv_error;\
 _wv_path_size = (path_size);\
 /* +7: Max pattern size: '\\?\' + '\*' + 0 */\
 while DEE_UNLIKELY((_wv_pattern = (Dee_WideChar *)malloc_nz((\
  _wv_path_size+7)*sizeof(Dee_WideChar))) == NULL) {\
  if DEE_LIKELY(Dee_CollectMemory()) continue;\
  DeeError_NoMemory();\
  {__VA_ARGS__;}\
 }\
 _wv_patternend = _wv_pattern;\
 if (((path)[0] != '\\' || (path)[1] != '\\'\
   || (path)[2] != '?'  || (path)[3] != '\\')\
   && (path)[0] && (path)[1] == ':') {\
  _wv_patternend[0] = '\\',_wv_patternend[1] = '\\';\
  _wv_patternend[2] = '?', _wv_patternend[3] = '\\';\
  _wv_patternend += 4;\
 }\
 memcpy(_wv_patternend,path,_wv_path_size*sizeof(Dee_WideChar));\
 _wv_patternend += _wv_path_size;\
 while (_wv_patternend != _wv_pattern && (_wv_patternend[-1] == '/'\
     || _wv_patternend[-1] == '\\')) --_wv_patternend;\
 _wv_patternend[0] = '\\';\
 _wv_patternend[1] = '*';\
 _wv_patternend[2] = 0;\
 DEE_LVERBOSE_SYS("FindFirstFileW(%lq)...",_wv_pattern);\
 (self)->w32_hview = FindFirstFileW(_wv_pattern,&(self)->w32_viewdata);\
 if ((self)->w32_hview == INVALID_HANDLE_VALUE) {\
  _wv_error = GetLastError();\
  DEE_LVERBOSE_SYSR(" FAILED\n");\
  if (_wv_error == ERROR_FILE_NOT_FOUND) {\
   free_nn(_wv_pattern);\
  } else {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "FindFirstFileW(%lq) : %K",_wv_pattern,\
                       DeeSystemError_Win32ToString(_wv_error));\
   free_nn(_wv_pattern);\
   {__VA_ARGS__;}\
  }\
 } else {\
  free_nn(_wv_pattern);\
  DEE_LVERBOSE_SYSR(" %lq\n",(self)->w32_viewdata.cFileName);\
  /* Skip '.' and '..' (always located at the start of views) */\
  while (DeeWin32SysFSWideViewNL_ISIGNOREDENTRY(self)) {\
   DEE_LVERBOSE_SYS("FindNextFileW(%p)...",(self)->w32_hview);\
   if DEE_UNLIKELY(!FindNextFileW((self)->w32_hview,&(self)->w32_viewdata)) {\
    _wv_error = GetLastError();\
    DEE_LVERBOSE_SYSR(" FAILED\n");\
    FindClose((self)->w32_hview);\
    if (_wv_error == ERROR_NO_MORE_FILES) {\
     (self)->w32_hview = INVALID_HANDLE_VALUE;\
     break;\
    } else {\
     DeeError_SetStringf(&DeeErrorType_SystemError,\
                         "FindNextFileW(%p,...) : %K",(self)->w32_hview,\
                         DeeSystemError_Win32ToString(_wv_error));\
     {__VA_ARGS__;}\
    }\
   }\
   DEE_LVERBOSE_SYSR(" %lq\n",(self)->w32_viewdata.cFileName);\
  }\
 }\
}while(0)
#define DeeWin32SysFSWideViewNL_Quit(self) \
 ((self)->w32_hview != INVALID_HANDLE_VALUE ? (void)FindClose((self)->w32_hview) : (void)0)
#define DeeWin32SysFSWideViewNL_IsDone(self) ((self)->w32_hview == INVALID_HANDLE_VALUE)
#define DeeWin32SysFSWideViewNL_Init(self,path,...)\
 DeeWin32SysFSWideViewNL_InitWithLength(self,path,Dee_WideStrLen(path),__VA_ARGS__)
#define DeeWin32SysFSWideViewNL_InitObject(self,path,...)\
 DeeWin32SysFSWideViewNL_InitWithLength(self,DeeWideString_STR(path),DeeWideString_SIZE(path),__VA_ARGS__)
#define DeeWin32SysFSWideViewNL_GetFilenameStrLocked(self) (self)->w32_viewdata.cFileName

#define DeeWin32SysFSWideViewNL_Advance(self,...) \
do{\
 DEE_LVERBOSE_SYS("FindNextFileW(%p)...",(self)->w32_hview);\
 if (!FindNextFileW((self)->w32_hview,&(self)->w32_viewdata)) {\
  DWORD _wv_error = GetLastError();\
  DEE_LVERBOSE_SYSR(" FAILED\n");\
  FindClose((self)->w32_hview);\
  if (_wv_error == ERROR_NO_MORE_FILES) {\
   (self)->w32_hview = INVALID_HANDLE_VALUE;\
  } else {\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "FindNextFileW(%p,...) : %K",(self)->w32_hview,\
                       DeeSystemError_Win32ToString(_wv_error));\
   {__VA_ARGS__;}\
  }\
 }\
 DEE_LVERBOSE_SYSR(" %lq\n",(self)->w32_viewdata.cFileName);\
}while(0)

#define DeeWin32SysFSWideViewNL_TryIsFile(self)   (((self)->w32_viewdata.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))==0)
#define DeeWin32SysFSWideViewNL_TryIsDir(self)    (((self)->w32_viewdata.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))==(FILE_ATTRIBUTE_DIRECTORY))
#define DeeWin32SysFSWideViewNL_TryIsLink(self)   (((self)->w32_viewdata.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))==(FILE_ATTRIBUTE_REPARSE_POINT))
#define DeeWin32SysFSWideViewNL_TryIsDrive(self)  0
#define DeeWin32SysFSWideViewNL_TryIsMount(self)  0
#define DeeWin32SysFSWideViewNL_TryIsHidden(self) (((self)->w32_viewdata.dwFileAttributes&(FILE_ATTRIBUTE_HIDDEN))!=0)

#define DeeWin32SysFSWideView DeeWin32SysFSWideView
struct DeeWin32SysFSWideView {
 HANDLE                w32_hview;
 WIN32_FIND_DATAW      w32_viewdata;
 struct DeeAtomicMutex w32_lock;
};

#define DeeWin32SysFSWideView_Quit DeeWin32SysFSWideViewNL_Quit
#define DeeWin32SysFSWideView_IsDone DeeWin32SysFSWideViewNL_IsDone
#define DeeWin32SysFSWideView_Init(self,path,...) \
do{DeeWin32SysFSWideViewNL_Init(self,path,__VA_ARGS__);DeeAtomicMutex_Init(&(self)->w32_lock);}while(0)
#define DeeWin32SysFSWideView_InitObject(self,path,...) \
do{DeeWin32SysFSWideViewNL_InitObject(self,path,__VA_ARGS__);DeeAtomicMutex_Init(&(self)->w32_lock);}while(0)


#define DeeWin32SysFSWideView_AdvanceAndReleaseOnError(self,...) \
do{\
 DEE_LVERBOSE_SYS("FindNextFileW(%p)...",(self)->w32_hview);\
 if (!FindNextFileW((self)->w32_hview,&(self)->w32_viewdata)) {\
  DWORD _wv_error = GetLastError();\
  DEE_LVERBOSE_SYSR(" FAILED\n");\
  FindClose((self)->w32_hview);\
  if (_wv_error == ERROR_NO_MORE_FILES) {\
   (self)->w32_hview = INVALID_HANDLE_VALUE;\
  } else {\
   HANDLE _wv_hview = (self)->w32_hview;\
   DeeAtomicMutex_Release(&(self)->w32_lock);\
   DeeError_SetStringf(&DeeErrorType_SystemError,\
                       "FindNextFileW(%p,...) : %K",_wv_hview,\
                       DeeSystemError_Win32ToString(_wv_error));\
   {__VA_ARGS__;}\
  }\
 }\
 DEE_LVERBOSE_SYSR(" %lq\n",(self)->w32_viewdata.cFileName);\
}while(0)


#define DeeWin32SysFSWideView_Acquire(self) DeeAtomicMutex_AcquireRelaxed(&(self)->w32_lock)
#define DeeWin32SysFSWideView_Release(self) DeeAtomicMutex_Release(&(self)->w32_lock)

DEE_STATIC_INLINE(DWORD) DeeWin32SysFSWideView_GetAttributes(struct DeeWin32SysFSWideView const *self) {
 DWORD result;
 DeeWin32SysFSWideView_Acquire(self);
 result = self->w32_viewdata.dwFileAttributes;
 DeeWin32SysFSWideView_Release(self);
 return result;
}
#define DeeWin32SysFSWideView_IsFileAndReleaseOnError(self,result,...)   do{*(result)=(((self)->w32_viewdata.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))==0);}while(0)
#define DeeWin32SysFSWideView_IsDirAndReleaseOnError(self,result,...)    do{*(result)=(((self)->w32_viewdata.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))==(FILE_ATTRIBUTE_DIRECTORY));}while(0)
#define DeeWin32SysFSWideView_IsLinkAndReleaseOnError(self,result,...)   do{*(result)=(((self)->w32_viewdata.dwFileAttributes&(FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT))==(FILE_ATTRIBUTE_REPARSE_POINT));}while(0)
#define DeeWin32SysFSWideView_IsDriveAndReleaseOnError(self,result,...)  do{*(result)=0;}while(0)
#define DeeWin32SysFSWideView_IsMountAndReleaseOnError(self,result,...)  do{*(result)=0;}while(0)
#define DeeWin32SysFSWideView_IsHiddenAndReleaseOnError(self,result,...) do{*(result)=((DeeWin32SysFSWideView_GetAttributes(self)&(FILE_ATTRIBUTE_HIDDEN))!=0);}while(0)

#define DeeWin32SysFSWideView_GetFilenameStrLocked DeeWin32SysFSWideViewNL_GetFilenameStrLocked
#define DEE_WIN32SYSFS_WIDEVIEW_GETFILENAMESTRLOCKED_NEVER_NULL

#define DeeSysFSWideView                           DeeWin32SysFSWideView
#define DeeSysFSWideView_Quit                      DeeWin32SysFSWideView_Quit
#define DeeSysFSWideView_IsDone                    DeeWin32SysFSWideView_IsDone
#define DeeSysFSWideView_Init                      DeeWin32SysFSWideView_Init
#define DeeSysFSWideView_InitObject                DeeWin32SysFSWideView_InitObject
#define DeeSysFSWideView_AdvanceAndReleaseOnError  DeeWin32SysFSWideView_AdvanceAndReleaseOnError
#define DeeSysFSWideView_Acquire                   DeeWin32SysFSWideView_Acquire
#define DeeSysFSWideView_Release                   DeeWin32SysFSWideView_Release
#define DeeSysFSWideView_GetFilenameStrLocked      DeeWin32SysFSWideView_GetFilenameStrLocked
#define DeeSysFSWideView_IsFileAndReleaseOnError   DeeWin32SysFSWideView_IsFileAndReleaseOnError
#define DeeSysFSWideView_IsDirAndReleaseOnError    DeeWin32SysFSWideView_IsDirAndReleaseOnError
#define DeeSysFSWideView_IsLinkAndReleaseOnError   DeeWin32SysFSWideView_IsLinkAndReleaseOnError
#define DeeSysFSWideView_IsDriveAndReleaseOnError  DeeWin32SysFSWideView_IsDriveAndReleaseOnError
#define DeeSysFSWideView_IsMountAndReleaseOnError  DeeWin32SysFSWideView_IsMountAndReleaseOnError
#define DeeSysFSWideView_IsHiddenAndReleaseOnError DeeWin32SysFSWideView_IsHiddenAndReleaseOnError
#define DEE_SYSFS_WIDEVIEW_GETFILENAMESTRLOCKED_NEVER_NULL

DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS__WIN32_SYSFS_VIEW_H */
