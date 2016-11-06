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
#ifndef GUARD_DEEMON_SYS__NOTIMPLEMENTED_SYSDYNLIB_H
#define GUARD_DEEMON_SYS__NOTIMPLEMENTED_SYSDYNLIB_H 1

#include <deemon/__conf.inl>
#include <deemon/error.h>

//////////////////////////////////////////////////////////////////////////
// === NOT-IMPLEMENTED ===
DEE_DECL_BEGIN

struct DeeSysDynlib  { int sd_unused; };
struct DeeSysDynlibN { int sd_unused; };
#define DeeSysDynlib_CompareLo(a,b) 0
#define DeeSysDynlib_CompareLe(a,b) 1
#define DeeSysDynlib_CompareEq(a,b) 1
#define DeeSysDynlib_CompareNe(a,b) 0
#define DeeSysDynlib_CompareGr(a,b) 0
#define DeeSysDynlib_CompareGe(a,b) 1
#define DeeSysDynlib_TryInitFromSelf(ob) 0
#define DeeSysDynlib_InitFromSelf(ob,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeSysDynlib_Utf8TryInitFromFilename(ob,filename) 0
#define DeeSysDynlib_WideTryInitFromFilename(ob,filename) 0
#define DeeSysDynlib_Utf8InitFromFilename(ob,filename,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeSysDynlib_WideInitFromFilename(ob,filename,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeSysDynlib_TryImport(ob,name,result) 0
#define DeeSysDynlib_Import(ob,name,result,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeSysDynlib_Quit(ob) do{}while(0)
#define DeeSysDynlibN_Utf8TryInitFromFilename(ob,filename) 0
#define DeeSysDynlibN_WideTryInitFromFilename(ob,filename) 0
#define DeeSysDynlibN_Utf8InitFromFilename(ob,filename,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeSysDynlibN_WideInitFromFilename(ob,filename,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeSysDynlibN_Utf8Name(ob) (DeeError_SetString(&DeeErrorType_NotImplemented,"dynlib"),NULL)
#define DeeSysDynlibN_WideName(ob) (DeeError_SetString(&DeeErrorType_NotImplemented,"dynlib"),NULL)
#define DeeSysDynlibN_Quit(ob) do{}while(0)
#define DeeSysDynlib_Utf8TryInitFromFilenameObject(ob,filename) 0
#define DeeSysDynlib_WideTryInitFromFilenameObject(ob,filename) 0
#define DeeSysDynlib_Utf8InitFromFilenameObject(ob,filename,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeSysDynlib_WideInitFromFilenameObject(ob,filename,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeSysDynlibN_TryInitFromSelf(ob) 0
#define DeeSysDynlibN_InitFromSelf(ob,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeSysDynlibN_Utf8TryInitFromFilenameObject(ob,filename) 0
#define DeeSysDynlibN_WideTryInitFromFilenameObject(ob,filename) 0
#define DeeSysDynlibN_Utf8InitFromFilenameObject(ob,filename,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeSysDynlibN_WideInitFromFilenameObject(ob,filename,...) do{DeeError_NotImplemented_str("dynlib");{__VA_ARGS__;}}while(0)
#define DeeSysDynlibN_Name(ob) (DeeError_SetString(&DeeErrorType_NotImplemented,"dynlib"),NULL)
#define DeeSysDynlibN_Quit(ob) do{}while(0)

DEE_DECL_END

#endif /* !GUARD_DEEMON_SYS__NOTIMPLEMENTED_SYSDYNLIB_H */
