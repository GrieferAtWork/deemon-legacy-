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
#ifndef GUARD_DEEMON_SYS__WIN32_SYSDYNLIB_H
#define GUARD_DEEMON_SYS__WIN32_SYSDYNLIB_H 1

#include <deemon/__conf.inl>
#include <deemon/error.h>

//////////////////////////////////////////////////////////////////////////
// === Win32 ===
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#include <deemon/fs_api.h>

// TODO: VFS Support

DEE_DECL_BEGIN
#define DEE_SYSDYNLIB_USES_WIN32
struct DeeSysDynlib  { HMODULE sd_lib; };
#define DeeSysDynlib_CompareLo(a,b) ((a)->sd_lib < (b)->sd_lib)
#define DeeSysDynlib_CompareLe(a,b) ((a)->sd_lib <= (b)->sd_lib)
#define DeeSysDynlib_CompareEq(a,b) ((a)->sd_lib == (b)->sd_lib)
#define DeeSysDynlib_CompareNe(a,b) ((a)->sd_lib != (b)->sd_lib)
#define DeeSysDynlib_CompareGr(a,b) ((a)->sd_lib > (b)->sd_lib)
#define DeeSysDynlib_CompareGe(a,b) ((a)->sd_lib >= (b)->sd_lib)
#define DeeSysDynlib_Quit(ob)                             do{ if DEE_UNLIKELY(!FreeLibrary((ob)->sd_lib)) SetLastError(0); }while(0)
// TODO: msdn says you shouldn't use '/' with LoadLibrary
// s.a.: https://msdn.microsoft.com/en-us/library/windows/desktop/ms684175(v=vs.85).aspx
//    >> If the syscall fails, search the given path for '/', replace them with '\\' and retry
// TODO: Auto UNC formatting
#if 1
#define DeeSysDynlib_TryInitFromSelf(ob)                  ((ob)->sd_lib = GetModuleHandleW(NULL),1)
#define DeeSysDynlib_InitFromSelf(ob,...)                 do{ DeeSysDynlib_TryInitFromSelf(ob); }while(0)
#else
#define DeeSysDynlib_TryInitFromSelf(ob)                  (((ob)->sd_lib = GetModuleHandleW(NULL)) != NULL)
#define DeeSysDynlib_InitFromSelf(ob,...)\
do{\
 if DEE_UNLIKELY(!DeeSysDynlib_TryInitFromSelf(ob)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"LoadLibraryA(NULL) : %K",\
                     DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif

#define DeeSysDynlib_Utf8TryInitFromFilename(ob,filename) (((ob)->sd_lib = LoadLibraryA(filename)) != NULL)
#define DeeSysDynlib_WideTryInitFromFilename(ob,filename) (((ob)->sd_lib = LoadLibraryW(filename)) != NULL)

#define DeeSysDynlib_Utf8TryInitFromFilenameObject(ob,filename) DeeSysDynlib_Utf8TryInitFromFilename(ob,DeeUtf8String_STR(filename))
#define DeeSysDynlib_WideTryInitFromFilenameObject(ob,filename) DeeSysDynlib_WideTryInitFromFilename(ob,DeeWideString_STR(filename))
#define DeeSysDynlib_Utf8InitFromFilename(ob,filename,...)\
do{\
 if DEE_UNLIKELY(!DeeSysDynlib_Utf8TryInitFromFilename(ob,filename)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"LoadLibraryA(%q) : %K",\
                     (filename),DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeSysDynlib_WideInitFromFilename(ob,filename,...)\
do{\
 if DEE_UNLIKELY(!DeeSysDynlib_WideTryInitFromFilename(ob,filename)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"LoadLibraryW(%lq) : %K",\
                     (filename),DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeSysDynlib_Utf8InitFromFilenameObject(ob,filename,...) DeeSysDynlib_Utf8InitFromFilename(ob,DeeUtf8String_STR(filename),__VA_ARGS__)
#define DeeSysDynlib_WideInitFromFilenameObject(ob,filename,...) DeeSysDynlib_WideInitFromFilename(ob,DeeWideString_STR(filename),__VA_ARGS__)
#define DeeSysDynlib_Utf8Win32Name(ob) DeeFS_Utf8Win32GetModuleName((void *)(ob)->sd_lib)
#define DeeSysDynlib_WideWin32Name(ob) DeeFS_WideWin32GetModuleName((void *)(ob)->sd_lib)
#define DeeSysDynlib_TryImport(ob,import_name,result) \
 ((*(void **)&(result) = (void *)GetProcAddress((ob)->sd_lib,import_name)) != NULL)
#define DeeSysDynlib_Import(ob,import_name,result,...) \
do{\
 if DEE_UNLIKELY(!DeeSysDynlib_TryImport(ob,import_name,result)) {\
  DWORD err = GetLastError();\
  DeeError_SetStringf(&DeeErrorType_SystemError,"GetProcAddress(%R,%q) : %K",\
                     DeeSysDynlib_Utf8Win32Name(ob),import_name,\
                     DeeSystemError_Win32ToString(err));\
  {__VA_ARGS__;}\
 }\
}while(0)

#define DeeSysDynlibN                               DeeSysDynlib
#define DeeSysDynlibN_TryInitFromSelf               DeeSysDynlib_TryInitFromSelf
#define DeeSysDynlibN_InitFromSelf                  DeeSysDynlib_InitFromSelf
#define DeeSysDynlibN_Utf8TryInitFromFilename       DeeSysDynlib_Utf8TryInitFromFilename
#define DeeSysDynlibN_WideTryInitFromFilename       DeeSysDynlib_WideTryInitFromFilename
#define DeeSysDynlibN_Utf8TryInitFromFilenameObject DeeSysDynlib_Utf8TryInitFromFilenameObject
#define DeeSysDynlibN_WideTryInitFromFilenameObject DeeSysDynlib_WideTryInitFromFilenameObject
#define DeeSysDynlibN_Utf8InitFromFilename          DeeSysDynlib_Utf8InitFromFilename
#define DeeSysDynlibN_WideInitFromFilename          DeeSysDynlib_WideInitFromFilename
#define DeeSysDynlibN_Utf8InitFromFilenameObject    DeeSysDynlib_Utf8InitFromFilenameObject
#define DeeSysDynlibN_WideInitFromFilenameObject    DeeSysDynlib_WideInitFromFilenameObject
#define DeeSysDynlibN_Utf8Name                      DeeSysDynlib_Utf8Win32Name
#define DeeSysDynlibN_WideName                      DeeSysDynlib_WideWin32Name
#define DeeSysDynlibN_Quit                          DeeSysDynlib_Quit


DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS__WIN32_SYSDYNLIB_H */
