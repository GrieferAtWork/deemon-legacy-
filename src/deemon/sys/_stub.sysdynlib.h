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
#ifndef GUARD_DEEMON_SYS__STUB_SYSDYNLIB_H
#define GUARD_DEEMON_SYS__STUB_SYSDYNLIB_H 1

#include <deemon/__conf.inl>
#include <deemon/error.h>

//////////////////////////////////////////////////////////////////////////
// === NOT-IMPLEMENTED ===
DEE_DECL_BEGIN

struct DeeStubSysDynlib  { int sd_unused; };
struct DeeStubSysDynlibN { int sd_unused; };
#define DeeStubSysDynlib_CompareLo(a,b) 0
#define DeeStubSysDynlib_CompareLe(a,b) 1
#define DeeStubSysDynlib_CompareEq(a,b) 1
#define DeeStubSysDynlib_CompareNe(a,b) 0
#define DeeStubSysDynlib_CompareGr(a,b) 0
#define DeeStubSysDynlib_CompareGe(a,b) 1
#define DeeStubSysDynlib_TryInitFromSelf(ob) 0
#define DeeStubSysDynlib_InitFromSelf(ob,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeStubSysDynlib_Utf8TryInitFromFilename(ob,filename) 0
#define DeeStubSysDynlib_WideTryInitFromFilename(ob,filename) 0
#define DeeStubSysDynlib_Utf8InitFromFilename(ob,filename,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeStubSysDynlib_WideInitFromFilename(ob,filename,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeStubSysDynlib_TryImport(ob,name,result) 0
#define DeeStubSysDynlib_Import(ob,name,result,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeStubSysDynlib_Quit(ob) do{}while(0)
#define DeeStubSysDynlibN_Utf8TryInitFromFilename(ob,filename) 0
#define DeeStubSysDynlibN_WideTryInitFromFilename(ob,filename) 0
#define DeeStubSysDynlibN_Utf8InitFromFilename(ob,filename,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeStubSysDynlibN_WideInitFromFilename(ob,filename,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeStubSysDynlibN_Utf8Name(ob) (DeeError_SetString(&DeeErrorType_NotImplemented,"dynlib"),NULL)
#define DeeStubSysDynlibN_WideName(ob) (DeeError_SetString(&DeeErrorType_NotImplemented,"dynlib"),NULL)
#define DeeStubSysDynlibN_Quit(ob) do{}while(0)
#define DeeStubSysDynlib_Utf8TryInitFromFilenameObject(ob,filename) 0
#define DeeStubSysDynlib_WideTryInitFromFilenameObject(ob,filename) 0
#define DeeStubSysDynlib_Utf8InitFromFilenameObject(ob,filename,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeStubSysDynlib_WideInitFromFilenameObject(ob,filename,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeStubSysDynlibN_TryInitFromSelf(ob) 0
#define DeeStubSysDynlibN_InitFromSelf(ob,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeStubSysDynlibN_Utf8TryInitFromFilenameObject(ob,filename) 0
#define DeeStubSysDynlibN_WideTryInitFromFilenameObject(ob,filename) 0
#define DeeStubSysDynlibN_Utf8InitFromFilenameObject(ob,filename,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeStubSysDynlibN_WideInitFromFilenameObject(ob,filename,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeStubSysDynlibN_Name(ob) (DeeError_SetString(&DeeErrorType_NotImplemented,"dynlib"),NULL)
#define DeeStubSysDynlibN_Quit(ob) do{}while(0)

#define DeeSysDynlib                                DeeStubSysDynlib
#define DeeSysDynlibN                               DeeStubSysDynlibN 
#define DeeSysDynlib_CompareLo                      DeeStubSysDynlib_CompareLo
#define DeeSysDynlib_CompareLe                      DeeStubSysDynlib_CompareLe
#define DeeSysDynlib_CompareEq                      DeeStubSysDynlib_CompareEq
#define DeeSysDynlib_CompareNe                      DeeStubSysDynlib_CompareNe
#define DeeSysDynlib_CompareGr                      DeeStubSysDynlib_CompareGr
#define DeeSysDynlib_CompareGe                      DeeStubSysDynlib_CompareGe
#define DeeSysDynlib_TryInitFromSelf                DeeStubSysDynlib_TryInitFromSelf
#define DeeSysDynlib_InitFromSelf                   DeeStubSysDynlib_InitFromSelf
#define DeeSysDynlib_Utf8TryInitFromFilename        DeeStubSysDynlib_Utf8TryInitFromFilename
#define DeeSysDynlib_WideTryInitFromFilename        DeeStubSysDynlib_WideTryInitFromFilename
#define DeeSysDynlib_Utf8TryInitFromFilenameObject  DeeStubSysDynlib_Utf8TryInitFromFilenameObject
#define DeeSysDynlib_WideTryInitFromFilenameObject  DeeStubSysDynlib_WideTryInitFromFilenameObject
#define DeeSysDynlib_Utf8InitFromFilename           DeeStubSysDynlib_Utf8InitFromFilename
#define DeeSysDynlib_WideInitFromFilename           DeeStubSysDynlib_WideInitFromFilename
#define DeeSysDynlib_Utf8InitFromFilenameObject     DeeStubSysDynlib_Utf8InitFromFilenameObject
#define DeeSysDynlib_WideInitFromFilenameObject     DeeStubSysDynlib_WideInitFromFilenameObject
#define DeeSysDynlib_TryImport                      DeeStubSysDynlib_TryImport
#define DeeSysDynlib_Import                         DeeStubSysDynlib_Import
#define DeeSysDynlib_Quit                           DeeStubSysDynlib_Quit
#define DeeSysDynlibN_TryInitFromSelf               DeeStubSysDynlibN_TryInitFromSelf
#define DeeSysDynlibN_InitFromSelf                  DeeStubSysDynlibN_InitFromSelf
#define DeeSysDynlibN_Utf8TryInitFromFilename       DeeStubSysDynlibN_Utf8TryInitFromFilename
#define DeeSysDynlibN_WideTryInitFromFilename       DeeStubSysDynlibN_WideTryInitFromFilename
#define DeeSysDynlibN_Utf8TryInitFromFilenameObject DeeStubSysDynlibN_Utf8TryInitFromFilenameObject
#define DeeSysDynlibN_WideTryInitFromFilenameObject DeeStubSysDynlibN_WideTryInitFromFilenameObject
#define DeeSysDynlibN_Utf8InitFromFilename          DeeStubSysDynlibN_Utf8InitFromFilename
#define DeeSysDynlibN_WideInitFromFilename          DeeStubSysDynlibN_WideInitFromFilename
#define DeeSysDynlibN_Utf8InitFromFilenameObject    DeeStubSysDynlibN_Utf8InitFromFilenameObject
#define DeeSysDynlibN_WideInitFromFilenameObject    DeeStubSysDynlibN_WideInitFromFilenameObject
#define DeeSysDynlibN_Name                          DeeStubSysDynlibN_Name
#define DeeSysDynlibN_Utf8Name                      DeeStubSysDynlibN_Utf8Name
#define DeeSysDynlibN_WideName                      DeeStubSysDynlibN_WideName
#define DeeSysDynlibN_Quit                          DeeStubSysDynlibN_Quit


DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS__STUB_SYSDYNLIB_H */
