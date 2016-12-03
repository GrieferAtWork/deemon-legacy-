/* Copyright (c) 2016 - deemon by Griefer@Work                                    *
 *                                                                                *
 * Permission is hereby granted, free of charge, to any person obtaining a copy   *
 * of this software and associated documentation files (the"Software"), to deal  *
 * in the Software without restriction, including without limitation the rights   *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
 * copies of the Software, and to permit persons to whom the Software is          *
 * furnished to do so, subject to the following conditions:                       *
 *                                                                                *
 * The above copyright notice and this permission notice shall be included in all *
 * copies or substantial portions of the Software.                                *
 *                                                                                *
 * THE SOFTWARE IS PROVIDED"AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
 * SOFTWARE.                                                                      *
 */
#ifndef GUARD_DEEMON_SYS_WIN32_W32_NTDLL_H
#define GUARD_DEEMON_SYS_WIN32_W32_NTDLL_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/atomic_once.h>

//////////////////////////////////////////////////////////////////////////
// === Win32 ===
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

#define DEE_WIN32_STATUS_INFO_LENGTH_MISMATCH 0xC0000004
#define DeeWin32_SystemHandleInformation (/*(SYSTEM_INFORMATION_CLASS)*/16)
#define DeeWin32_ObjectBasicInformation  (/*(OBJECT_INFORMATION_CLASS)*/0)
#define DeeWin32_ObjectNameInformation   (/*(OBJECT_INFORMATION_CLASS)*/1)
#define DeeWin32_ObjectTypeInformation   (/*(OBJECT_INFORMATION_CLASS)*/2)
#define DeeWin32_ObjectAllInformation    (/*(OBJECT_INFORMATION_CLASS)*/3)
#define DeeWin32_ObjectDataInformation   (/*(OBJECT_INFORMATION_CLASS)*/4)


static WCHAR const DeeWin32_sNTDLL[] = {'n','t','d','l','l','.','d','l','l',0};


typedef NTSTATUS (NTAPI *DEE_WIN32_LPNTQUERYSYSTEMINFORMATION)(/*SYSTEM_INFORMATION_CLASS*/ int SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
typedef NTSTATUS (NTAPI *DEE_WIN32_LPNTQUERYOBJECT)(HANDLE Handle, /*OBJECT_INFORMATION_CLASS*/ int ObjectInformationClass, PVOID ObjectInformation, ULONG ObjectInformationLength, PULONG ReturnLength);

#define DEE_ENTRY static DEE_ATTRIBUTE_UNUSED


DEE_ENTRY DEE_WIN32_LPNTQUERYSYSTEMINFORMATION DeeWin32_NtQuerySystemInformation = NULL;
DEE_ENTRY DEE_WIN32_LPNTQUERYOBJECT            DeeWin32_NtQueryObject            = NULL;
DEE_ENTRY char const DeeWin32Name_NtQuerySystemInformation[] = "NtQuerySystemInformation";
DEE_ENTRY char const DeeWin32Name_NtQueryObject[]            = "NtQueryObject";
DEE_ENTRY struct DeeAtomicOnceFlag DeeWin32Loaded_NtQuerySystemInformation = DeeAtomicOnceFlag_INIT();
DEE_ENTRY struct DeeAtomicOnceFlag DeeWin32Loaded_NtQueryObject            = DeeAtomicOnceFlag_INIT();

#define DeeWin32Load_NtQuerySystemInformation() DeeAtomicOnceFlag_RUN(&DeeWin32Loaded_NtQuerySystemInformation,{ *(FARPROC *)&DeeWin32_NtQuerySystemInformation = GetProcAddress(GetModuleHandleW(DeeWin32_sNTDLL),DeeWin32Name_NtQuerySystemInformation); })
#define DeeWin32Load_NtQueryObject()            DeeAtomicOnceFlag_RUN(&DeeWin32Loaded_NtQueryObject,           { *(FARPROC *)&DeeWin32_NtQueryObject            = GetProcAddress(GetModuleHandleW(DeeWin32_sNTDLL),DeeWin32Name_NtQueryObject); })

#undef DEE_ENTRY

DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS_WIN32_W32_NTDLL_H */
