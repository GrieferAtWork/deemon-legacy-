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
#ifndef GUARD_DEEMON_SYS_SYSINFO_H
#define GUARD_DEEMON_SYS_SYSINFO_H 1

#include <deemon/__conf.inl>


//////////////////////////////////////////////////////////////////////////
// >> [[optional]] void DeeSysInfo_Utf8GetHostname(DEE_A_OUT_OBJECT(DeeUtf8StringObject) **result, CODE on_error);
// >> [[optional]] void DeeSysInfo_WideGetHostname(DEE_A_OUT_OBJECT(DeeUtf8StringObject) **result, CODE on_error);
//////////////////////////////////////////////////////////////////////////
// >> [[optional]] void DeeSysInfo_Utf8GetUsername(DEE_A_OUT_OBJECT(DeeUtf8StringObject) **result, CODE on_error);
// >> [[optional]] void DeeSysInfo_WideGetUsername(DEE_A_OUT_OBJECT(DeeUtf8StringObject) **result, CODE on_error);
//////////////////////////////////////////////////////////////////////////


#if defined(DEE_PLATFORM_WINDOWS)
# include <deemon/sys/win32/sysinfo.h>
#elif defined(DEE_PLATFORM_UNIX)
# include <deemon/sys/unix/sysinfo.h>
#elif 1
# include <deemon/sys/stub/sysinfo.h>
#else
# error "No Sysinfo implementation for this platform"
#endif



#endif /* !GUARD_DEEMON_SYS_SYSINFO_H */
