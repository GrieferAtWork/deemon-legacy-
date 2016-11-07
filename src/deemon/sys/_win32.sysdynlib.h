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
#include <deemon/virtual_fs.h.inl>

//////////////////////////////////////////////////////////////////////////
// === Win32 ===
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#include DEE_INCLUDE_MEMORY_API()

// TODO: VFS Support

DEE_DECL_BEGIN
#define DEE_SYSDYNLIB_USES_WIN32
struct DeeWin32SysDynlib  { HMODULE sd_lib; };
#define DeeWin32SysDynlib_CompareLo(a,b) ((a)->sd_lib < (b)->sd_lib)
#define DeeWin32SysDynlib_CompareLe(a,b) ((a)->sd_lib <= (b)->sd_lib)
#define DeeWin32SysDynlib_CompareEq(a,b) ((a)->sd_lib == (b)->sd_lib)
#define DeeWin32SysDynlib_CompareNe(a,b) ((a)->sd_lib != (b)->sd_lib)
#define DeeWin32SysDynlib_CompareGr(a,b) ((a)->sd_lib > (b)->sd_lib)
#define DeeWin32SysDynlib_CompareGe(a,b) ((a)->sd_lib >= (b)->sd_lib)
#define DeeWin32SysDynlib_Quit(ob)                             do{ if DEE_UNLIKELY(!FreeLibrary((ob)->sd_lib)) SetLastError(0); }while(0)
#if 1
#define DeeWin32SysDynlib_TryInitFromSelf(ob)                  ((ob)->sd_lib = GetModuleHandleW(NULL),1)
#define DeeWin32SysDynlib_InitFromSelf(ob,...)                 do{ DeeWin32SysDynlib_TryInitFromSelf(ob); }while(0)
#else
#define DeeWin32SysDynlib_TryInitFromSelf(ob)                  (((ob)->sd_lib = GetModuleHandleW(NULL)) != NULL)
#define DeeWin32SysDynlib_InitFromSelf(ob,...)\
do{\
 if DEE_UNLIKELY(!DeeWin32SysDynlib_TryInitFromSelf(ob)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"LoadLibraryA(NULL) : %K",\
                     DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif

// msdn says you shouldn't use '/' with LoadLibrary
// s.a.: https://msdn.microsoft.com/en-us/library/windows/desktop/ms684175(v=vs.85).aspx
//    >> If the syscall fails, search the given path for '/', replace them with '\\' and retry
// NOTE: Also perform auto UNC formatting to allow for longer path names
DEE_STATIC_INLINE(HMODULE) _fixed_LoadLibraryW(Dee_WideChar const *filename) {
 HMODULE result; Dee_WideChar *fixed_filename,*iter;
 Dee_size_t filename_size; DWORD saved_error;
 filename_size = Dee_WideStrLen(filename);
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
 if (filename[0] == '\\' && filename[1] == '\\' &&
     filename[2] == '?' && filename[2] == '\\')
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
 {
  // Already a UNC filename
  fixed_filename = (Dee_WideChar *)malloc_nz((filename_size+1)*sizeof(Dee_WideChar));
  if DEE_UNLIKELY(!fixed_filename) {no_mem: SetLastError(ERROR_NOT_ENOUGH_MEMORY); return NULL; }
  memcpy(fixed_filename,filename,filename_size*sizeof(Dee_WideChar));
  iter = (Dee_WideChar *)filename;
  fixed_filename[filename_size] = 0;
 }
#if DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC
 else {
  fixed_filename = (Dee_WideChar *)malloc_nz((filename_size+5)*sizeof(Dee_WideChar));
  if DEE_UNLIKELY(!fixed_filename) goto no_mem;
  fixed_filename[0] = '\\';
  fixed_filename[1] = '\\';
  fixed_filename[2] = '?';
  fixed_filename[3] = '\\';
  memcpy(fixed_filename+4,filename,filename_size*sizeof(Dee_WideChar));
  fixed_filename[filename_size+4] = 0;
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_AUTO_UNC */
 iter = fixed_filename;
 while (*iter) { if (*iter == '/') *iter = '\\'; ++iter; }
 result = LoadLibraryW(fixed_filename);
 saved_error = GetLastError();
 free_nn(fixed_filename);
 SetLastError(saved_error);
 return result;
}
DEE_STATIC_INLINE(HMODULE) _fixed_LoadLibraryA(Dee_Utf8Char const *filename) {
 HMODULE result; DeeObject *widefile; DWORD saved_error;
 if DEE_UNLIKELY((widefile = DeeWideString_FromUtf8String(filename)) == NULL)
 { DeeError_HandledOne(); return LoadLibraryA(filename); }
 result = _fixed_LoadLibraryW(DeeWideString_STR(widefile));
 saved_error = GetLastError();
 Dee_DECREF(widefile);
 SetLastError(saved_error);
 return result;
}

#define DeeWin32SysDynlib_Utf8TryInitFromFilename(ob,filename) \
 (((ob)->sd_lib = LoadLibraryA(filename)) == NULL ?\
 (((ob)->sd_lib = _fixed_LoadLibraryA(filename)) != NULL) : 1)
#define DeeWin32SysDynlib_WideTryInitFromFilename(ob,filename) \
 (((ob)->sd_lib = LoadLibraryW(filename)) == NULL ?\
 (((ob)->sd_lib = _fixed_LoadLibraryW(filename)) != NULL) : 1)
#define DeeWin32SysDynlib_Utf8InitFromFilename(ob,filename,...)\
do{\
 if DEE_UNLIKELY(!DeeWin32SysDynlib_Utf8TryInitFromFilename(ob,filename)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"LoadLibraryA(%q) : %K",\
                     (filename),DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeWin32SysDynlib_WideInitFromFilename(ob,filename,...)\
do{\
 if DEE_UNLIKELY(!DeeWin32SysDynlib_WideTryInitFromFilename(ob,filename)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"LoadLibraryW(%lq) : %K",\
                     (filename),DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeWin32SysDynlib_Utf8Win32Name(ob) DeeFS_Utf8Win32GetModuleName((void *)(ob)->sd_lib)
#define DeeWin32SysDynlib_WideWin32Name(ob) DeeFS_WideWin32GetModuleName((void *)(ob)->sd_lib)
#define DeeWin32SysDynlib_TryImport(ob,import_name,result) \
 ((*(void **)&(result) = (void *)GetProcAddress((ob)->sd_lib,import_name)) != NULL)
#define DeeWin32SysDynlib_Import(ob,import_name,result,...) \
do{\
 if DEE_UNLIKELY(!DeeWin32SysDynlib_TryImport(ob,import_name,result)) {\
  DWORD err = GetLastError();\
  DeeError_SetStringf(&DeeErrorType_SystemError,"GetProcAddress(%R,%q) : %K",\
                     DeeWin32SysDynlib_Utf8Win32Name(ob),import_name,\
                     DeeSystemError_Win32ToString(err));\
  {__VA_ARGS__;}\
 }\
}while(0)

#define DeeWin32SysDynlibN                         DeeWin32SysDynlib
#define DeeWin32SysDynlibN_TryInitFromSelf         DeeWin32SysDynlib_TryInitFromSelf
#define DeeWin32SysDynlibN_InitFromSelf            DeeWin32SysDynlib_InitFromSelf
#define DeeWin32SysDynlibN_Utf8TryInitFromFilename DeeWin32SysDynlib_Utf8TryInitFromFilename
#define DeeWin32SysDynlibN_WideTryInitFromFilename DeeWin32SysDynlib_WideTryInitFromFilename
#define DeeWin32SysDynlibN_Utf8InitFromFilename    DeeWin32SysDynlib_Utf8InitFromFilename
#define DeeWin32SysDynlibN_WideInitFromFilename    DeeWin32SysDynlib_WideInitFromFilename
#define DeeWin32SysDynlibN_Utf8Name                DeeWin32SysDynlib_Utf8Win32Name
#define DeeWin32SysDynlibN_WideName                DeeWin32SysDynlib_WideWin32Name
#define DeeWin32SysDynlibN_Quit                    DeeWin32SysDynlib_Quit



#define DeeSysDynlib                                DeeWin32SysDynlib
#define DeeSysDynlibN                               DeeWin32SysDynlibN 
#define DeeSysDynlib_CompareLo                      DeeWin32SysDynlib_CompareLo
#define DeeSysDynlib_CompareLe                      DeeWin32SysDynlib_CompareLe
#define DeeSysDynlib_CompareEq                      DeeWin32SysDynlib_CompareEq
#define DeeSysDynlib_CompareNe                      DeeWin32SysDynlib_CompareNe
#define DeeSysDynlib_CompareGr                      DeeWin32SysDynlib_CompareGr
#define DeeSysDynlib_CompareGe                      DeeWin32SysDynlib_CompareGe
#define DeeSysDynlib_TryInitFromSelf                DeeWin32SysDynlib_TryInitFromSelf
#define DeeSysDynlib_InitFromSelf                   DeeWin32SysDynlib_InitFromSelf
#define DeeSysDynlib_Utf8TryInitFromFilename        DeeWin32SysDynlib_Utf8TryInitFromFilename
#define DeeSysDynlib_WideTryInitFromFilename        DeeWin32SysDynlib_WideTryInitFromFilename
#define DeeSysDynlib_Utf8InitFromFilename           DeeWin32SysDynlib_Utf8InitFromFilename
#define DeeSysDynlib_WideInitFromFilename           DeeWin32SysDynlib_WideInitFromFilename
#define DeeSysDynlib_TryImport                      DeeWin32SysDynlib_TryImport
#define DeeSysDynlib_Import                         DeeWin32SysDynlib_Import
#define DeeSysDynlib_Quit                           DeeWin32SysDynlib_Quit
#define DeeSysDynlibN_TryInitFromSelf               DeeWin32SysDynlibN_TryInitFromSelf
#define DeeSysDynlibN_InitFromSelf                  DeeWin32SysDynlibN_InitFromSelf
#define DeeSysDynlibN_Utf8TryInitFromFilename       DeeWin32SysDynlibN_Utf8TryInitFromFilename
#define DeeSysDynlibN_WideTryInitFromFilename       DeeWin32SysDynlibN_WideTryInitFromFilename
#define DeeSysDynlibN_Utf8InitFromFilename          DeeWin32SysDynlibN_Utf8InitFromFilename
#define DeeSysDynlibN_WideInitFromFilename          DeeWin32SysDynlibN_WideInitFromFilename
#define DeeSysDynlibN_Utf8Name                      DeeWin32SysDynlibN_Utf8Name
#define DeeSysDynlibN_WideName                      DeeWin32SysDynlibN_WideName
#define DeeSysDynlibN_Quit                          DeeWin32SysDynlibN_Quit


DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS__WIN32_SYSDYNLIB_H */
