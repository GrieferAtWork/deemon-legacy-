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
#ifndef GUARD_DEEMON_SYS__STUB_SYSFD_H
#define GUARD_DEEMON_SYS__STUB_SYSFD_H 1

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/file.h>

DEE_DECL_BEGIN

#define DEE_STUB_SYSFD_HEAD  int placeholder;
struct DeeStubSysFD { DEE_STUB_SYSFD_HEAD };

#define DEE_STUB_SYSFD_SEEK_SET                       DEE_SEEK_SET
#define DEE_STUB_SYSFD_SEEK_CUR                       DEE_SEEK_CUR
#define DEE_STUB_SYSFD_SEEK_END                       DEE_SEEK_END
#define DeeStubSysFD_TryRead(self,p,s,rs)             0
#define DeeStubSysFD_TryWrite(self,p,s,ws)            0
#define DeeStubSysFD_TrySeek(self,off,whence,newpos)  0
#define DeeStubSysFD_Seek(self,off,whence,newpos,...) do{DeeError_NotImplemented_str("sysfd");{__VA_ARGS__;}}while(0)
#define DeeStubSysFD_Read(self,p,s,rs,...)            do{DeeError_NotImplemented_str("sysfd");{__VA_ARGS__;}}while(0)
#define DeeStubSysFD_Write(self,p,s,ws,...)           do{DeeError_NotImplemented_str("sysfd");{__VA_ARGS__;}}while(0)
#define DeeStubSysFD_INIT_STDIN()     {0}
#define DeeStubSysFD_INIT_STDOUT()    {0}
#define DeeStubSysFD_INIT_STDERR()    {0}
#define DeeStubSysFD_GET_STDIN(self)  (void)0
#define DeeStubSysFD_GET_STDOUT(self) (void)0
#define DeeStubSysFD_GET_STDERR(self) (void)0

//////////////////////////////////////////////////////////////////////////
// === DeeStubSysFileFD ===
struct DeeStubSysFileFD { DEE_STUB_SYSFD_HEAD };
#define DeeStubSysFileFD_Utf8TryInit(self,filename,mode,perms)        0
#define DeeStubSysFileFD_Utf8TryInitObject(self,filename,mode,perms)  0
#define DeeStubSysFileFD_WideTryInit(self,filename,mode,perms)        0
#define DeeStubSysFileFD_WideTryInitObject(self,filename,mode,perms)  0
#define DeeStubSysFileFD_Utf8Init(self,filename,mode,perms,...)       do{DeeError_NotImplemented_str("sysfd");{__VA_ARGS__;}}while(0)
#define DeeStubSysFileFD_Utf8InitObject(self,filename,mode,perms,...) do{DeeError_NotImplemented_str("sysfd");{__VA_ARGS__;}}while(0)
#define DeeStubSysFileFD_WideInit(self,filename,mode,perms,...)       do{DeeError_NotImplemented_str("sysfd");{__VA_ARGS__;}}while(0)
#define DeeStubSysFileFD_WideInitObject(self,filename,mode,perms,...) do{DeeError_NotImplemented_str("sysfd");{__VA_ARGS__;}}while(0)
#define DeeStubSysFileFD_Utf8Filename(ob) (DeeError_SetString(&DeeErrorType_NotImplemented,"sysfd"),(DeeObject *)NULL)
#define DeeStubSysFileFD_WideFilename(ob) (DeeError_SetString(&DeeErrorType_NotImplemented,"sysfd"),(DeeObject *)NULL)

#define DEE_SYSFD_SEEK_SET  DEE_STUB_SYSFD_SEEK_SET
#define DEE_SYSFD_SEEK_CUR  DEE_STUB_SYSFD_SEEK_CUR
#define DEE_SYSFD_SEEK_END  DEE_STUB_SYSFD_SEEK_END

#define DeeSysFD             DeeStubSysFD
#define DeeSysFD_TryRead     DeeStubSysFD_TryRead
#define DeeSysFD_TryWrite    DeeStubSysFD_TryWrite
#define DeeSysFD_TrySeek     DeeStubSysFD_TrySeek
#define DeeSysFD_Read        DeeStubSysFD_Read
#define DeeSysFD_Write       DeeStubSysFD_Write
#define DeeSysFD_Seek        DeeStubSysFD_Seek
#define DeeSysFD_INIT_STDIN  DeeStubSysFD_INIT_STDIN
#define DeeSysFD_INIT_STDOUT DeeStubSysFD_INIT_STDOUT
#define DeeSysFD_INIT_STDERR DeeStubSysFD_INIT_STDERR
#define DeeSysFD_GET_STDIN   DeeStubSysFD_GET_STDIN
#define DeeSysFD_GET_STDOUT  DeeStubSysFD_GET_STDOUT
#define DeeSysFD_GET_STDERR  DeeStubSysFD_GET_STDERR

#define DeeSysFileFD                   DeeStubSysFileFD
#define DeeSysFileFD_Utf8TryInit       DeeStubSysFileFD_Utf8TryInit
#define DeeSysFileFD_WideTryInit       DeeStubSysFileFD_WideTryInit
#define DeeSysFileFD_Utf8TryInitObject DeeStubSysFileFD_Utf8TryInitObject
#define DeeSysFileFD_WideTryInitObject DeeStubSysFileFD_WideTryInitObject
#define DeeSysFileFD_Utf8Init          DeeStubSysFileFD_Utf8Init
#define DeeSysFileFD_WideInit          DeeStubSysFileFD_WideInit
#define DeeSysFileFD_Utf8InitObject    DeeStubSysFileFD_Utf8InitObject
#define DeeSysFileFD_WideInitObject    DeeStubSysFileFD_WideInitObject
#define DeeSysFileFD_Filename          DeeStubSysFileFD_WideFilename
#define DeeSysFileFD_Utf8Filename      DeeStubSysFileFD_Utf8Filename
#define DeeSysFileFD_WideFilename      DeeStubSysFileFD_WideFilename

#undef DEE_STUB_SYSFD_HEAD

DEE_DECL_END


#endif /* !GUARD_DEEMON_SYS__STUB_SYSFD_H */
