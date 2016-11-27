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
#ifndef GUARD_DEEMON_SYS_STUB_SYSINFO_H
#define GUARD_DEEMON_SYS_STUB_SYSINFO_H 1

#include <deemon/__conf.inl>
#include <deemon/string.h>

//////////////////////////////////////////////////////////////////////////
// === NOT-IMPLEMENTED ===
DEE_DECL_BEGIN

#define DeeStubSysInfo_Utf8GetHostname(result,...) \
do{\
 DeeUtf8String_NEW_STATIC_EX(_hn_stub,9,{'l','o','c','a','l','h','o','s','t'});\
 Dee_INCREF(*(result) = (DeeObject *)&_hn_stub);\
}while(0)
#define DeeStubSysInfo_WideGetHostname(result,...) \
do{\
 DeeWideString_NEW_STATIC_EX(_hn_stub,9,{'l','o','c','a','l','h','o','s','t'});\
 Dee_INCREF(*(result) = (DeeObject *)&_hn_stub);\
}while(0)

#define DeeStubSysInfo_Utf8GetUsername(result,...) \
do{\
 DeeUtf8String_NEW_STATIC_EX(_un_stub,4,{'U','S','E','R'});\
 Dee_INCREF(*(result) = (DeeObject *)&_un_stub);\
}while(0)
#define DeeStubSysInfo_WideGetUsername(result,...) \
do{\
 DeeWideString_NEW_STATIC_EX(_un_stub,4,{'U','S','E','R'});\
 Dee_INCREF(*(result) = (DeeObject *)&_un_stub);\
}while(0)

#define DeeSysInfo_Utf8GetHostname DeeStubSysInfo_Utf8GetHostname
#define DeeSysInfo_WideGetHostname DeeStubSysInfo_WideGetHostname
#define DeeSysInfo_Utf8GetUsername DeeStubSysInfo_Utf8GetUsername
#define DeeSysInfo_WideGetUsername DeeStubSysInfo_WideGetUsername

DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS_STUB_SYSINFO_H */
