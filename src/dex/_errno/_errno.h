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
#ifndef GUARD_DEEMON_DEX_ERRNO_H
#define GUARD_DEEMON_DEX_ERRNO_H 1
 
#include <deemon/__conf.inl>
#include <deemon/optional/string_forward.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
#ifdef DEE_PLATFORM_WINDOWS
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#endif
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#include <errno.h>
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H */
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif

extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeErrno_ErrnoName(DEE_A_IN int err);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeErrno_ErrnoText(DEE_A_IN int err);

#ifdef DEE_PLATFORM_WINDOWS
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeErrno_Win32ErrnoName(DEE_A_IN DWORD err);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeErrno_Win32ErrnoText(DEE_A_IN DWORD err);
#endif /* DEE_PLATFORM_WINDOWS */

DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_ERRNO_H */
