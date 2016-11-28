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
#ifndef GUARD_DEEMON_SYS_WIN32_W32_PROCESS_MAPS_H
#define GUARD_DEEMON_SYS_WIN32_W32_PROCESS_MAPS_H 1

#include <deemon/__conf.inl>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/optional/atomic_once.h>

//////////////////////////////////////////////////////////////////////////
// === Win32 ===
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN


static int DeeWin32_CaptureProcessMaps(
 DEE_A_INOUT struct DeeStringWriter *writer, HANDLE hProcess) {
 Dee_uintptr_t addr;
 MEMORY_BASIC_INFORMATION info;
 if (!VirtualQueryEx(hProcess,NULL,&info,sizeof(info))) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "VirtualQueryEx(%p,NULL,...) : %K",hProcess,
                      DeeSystemError_Win32ToString(GetLastError()));
  return -1;
 }
 addr = 0;
 for (; ; i = cur.rend)
}


DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS_WIN32_W32_PROCESS_MAPS_H */
