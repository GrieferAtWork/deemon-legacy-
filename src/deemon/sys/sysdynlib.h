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
#ifndef GUARD_DEEMON_SYS_SYSDYNLIB_H
#define GUARD_DEEMON_SYS_SYSDYNLIB_H 1

#include <deemon/__conf.inl>

//////////////////////////////////////////////////////////////////////////
// >> struct DeeSysDynlib  { ... };
// >> struct DeeSysDynlibN { ... };
// >> bool DeeSysDynlib_CompareLo(struct DeeSysDynlib const *a, struct DeeSysDynlib const *b);
// >> bool DeeSysDynlib_CompareLe(struct DeeSysDynlib const *a, struct DeeSysDynlib const *b);
// >> bool DeeSysDynlib_CompareEq(struct DeeSysDynlib const *a, struct DeeSysDynlib const *b);
// >> bool DeeSysDynlib_CompareNe(struct DeeSysDynlib const *a, struct DeeSysDynlib const *b);
// >> bool DeeSysDynlib_CompareGr(struct DeeSysDynlib const *a, struct DeeSysDynlib const *b);
// >> bool DeeSysDynlib_CompareGe(struct DeeSysDynlib const *a, struct DeeSysDynlib const *b);
// >> bool DeeSysDynlib_TryInitFromSelf(struct DeeSysDynlib *ob);
// >> void DeeSysDynlib_InitFromSelf(struct DeeSysDynlib *ob, CODE on_error);
// >> bool DeeSysDynlib_Utf8TryInitFromFilename(struct DeeSysDynlib *ob, Dee_Utf8Char const *filename);
// >> bool DeeSysDynlib_WideTryInitFromFilename(struct DeeSysDynlib *ob, Dee_WideChar const *filename);
// >> bool DeeSysDynlib_Utf8TryInitFromFilenameObject(struct DeeSysDynlib *ob, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *filename);
// >> bool DeeSysDynlib_WideTryInitFromFilenameObject(struct DeeSysDynlib *ob, DEE_A_IN_OBJECT(DeeWideStringObject) const *filename);
// >> void DeeSysDynlib_Utf8InitFromFilename(struct DeeSysDynlib *ob, Dee_Utf8Char const *filename, CODE on_error);
// >> void DeeSysDynlib_WideInitFromFilename(struct DeeSysDynlib *ob, Dee_WideChar const *filename, CODE on_error);
// >> void DeeSysDynlib_Utf8InitFromFilenameObject(struct DeeSysDynlib *ob, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *filename, CODE on_error);
// >> void DeeSysDynlib_WideInitFromFilenameObject(struct DeeSysDynlib *ob, DEE_A_IN_OBJECT(DeeWideStringObject) const *filename, CODE on_error);
// >> bool DeeSysDynlib_TryImport(struct DeeSysDynlib const *lib, char const *name, T *&result);
// >> void DeeSysDynlib_Import(struct DeeSysDynlib const *lib, char const *name, T *&result, CODE on_error);
// >> void DeeSysDynlib_Quit(struct DeeSysDynlib *ob);
// >> bool DeeSysDynlibN_TryInitFromSelf(struct DeeSysDynlibN *ob);
// >> void DeeSysDynlibN_InitFromSelf(struct DeeSysDynlibN *ob, CODE on_error);
// >> bool DeeSysDynlibN_Utf8TryInitFromFilename(struct DeeSysDynlibN *ob, Dee_Utf8Char const *filename);
// >> bool DeeSysDynlibN_WideTryInitFromFilename(struct DeeSysDynlibN *ob, Dee_WideChar const *filename);
// >> bool DeeSysDynlibN_Utf8TryInitFromFilenameObject(struct DeeSysDynlibN *ob, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *filename);
// >> bool DeeSysDynlibN_WideTryInitFromFilenameObject(struct DeeSysDynlibN *ob, DEE_A_IN_OBJECT(DeeWideStringObject) const *filename);
// >> void DeeSysDynlibN_Utf8InitFromFilename(struct DeeSysDynlibN *ob, Dee_Utf8Char const *filename, CODE on_error);
// >> void DeeSysDynlibN_WideInitFromFilename(struct DeeSysDynlibN *ob, Dee_WideChar const *filename, CODE on_error);
// >> void DeeSysDynlibN_Utf8InitFromFilenameObject(struct DeeSysDynlibN *ob, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *filename, CODE on_error);
// >> void DeeSysDynlibN_WideInitFromFilenameObject(struct DeeSysDynlibN *ob, DEE_A_IN_OBJECT(DeeWideStringObject) const *filename, CODE on_error);
// >> DEE_A_REF DeeAnyStringObject *DeeSysDynlibN_Name(struct DeeSysDynlibN const *ob);
// >> DEE_A_REF DeeUtf8StringObject *DeeSysDynlibN_Utf8Name(struct DeeSysDynlibN const *ob);
// >> DEE_A_REF DeeWideStringObject *DeeSysDynlibN_WideName(struct DeeSysDynlibN const *ob);
// >> void DeeSysDynlibN_Quit(struct DeeSysDynlibN *ob);
#define DeeSysDynlib_TryInitFromFilename        DeeSysDynlib_Utf8TryInitFromFilename
#define DeeSysDynlib_TryInitFromFilenameObject  DeeSysDynlib_Utf8TryInitFromFilenameObject
#define DeeSysDynlib_InitFromFilename           DeeSysDynlib_Utf8InitFromFilename
#define DeeSysDynlib_InitFromFilenameObject     DeeSysDynlib_Utf8InitFromFilenameObject
#define DeeSysDynlibN_TryInitFromFilename       DeeSysDynlibN_Utf8TryInitFromFilename
#define DeeSysDynlibN_TryInitFromFilenameObject DeeSysDynlibN_Utf8TryInitFromFilenameObject
#define DeeSysDynlibN_InitFromFilename          DeeSysDynlibN_Utf8InitFromFilename
#define DeeSysDynlibN_InitFromFilenameObject    DeeSysDynlibN_Utf8InitFromFilenameObject

#if defined(DEE_PLATFORM_WINDOWS)
# include <deemon/sys/_win32.sysdynlib.h>
#elif defined(DEE_PLATFORM_UNIX)
# include <deemon/sys/_posix.sysdynlib.h>
#elif defined(__HAIKU__)
# include <deemon/sys/_haiku.sysdynlib.h>
#elif 1
# include <deemon/sys/_stub.sysdynlib.h>
#else
# error "No dynlib implementation for this platform"
#endif


#ifndef DeeSysDynlibN_Name
#define DeeSysDynlibN_Name DeeSysDynlibN_Utf8Name
#endif

#ifndef DeeSysDynlib_InitFromSelf
#define DeeSysDynlib_InitFromSelf(ob,...) do{if (!DeeSysDynlib_TryInitFromSelf(ob)) { DeeError_SET_STRING(&DeeErrorType_SystemError,"Failed to initialize dynlib from self");{__VA_ARGS__;}}}while(0)
#endif
#ifndef DeeSysDynlib_Utf8TryInitFromFilenameObject
#define DeeSysDynlib_Utf8TryInitFromFilenameObject(ob,filename) DeeSysDynlib_Utf8TryInitFromFilename(ob,DeeUtf8String_STR(filename))
#endif
#ifndef DeeSysDynlib_WideTryInitFromFilenameObject
#define DeeSysDynlib_WideTryInitFromFilenameObject(ob,filename) DeeSysDynlib_WideTryInitFromFilename(ob,DeeWideString_STR(filename))
#endif
#ifndef DeeSysDynlib_Utf8InitFromFilenameObject
#define DeeSysDynlib_Utf8InitFromFilenameObject(ob,filename,...) DeeSysDynlib_Utf8InitFromFilename(ob,DeeUtf8String_STR(filename),__VA_ARGS__)
#endif
#ifndef DeeSysDynlib_WideInitFromFilenameObject
#define DeeSysDynlib_WideInitFromFilenameObject(ob,filename,...) DeeSysDynlib_WideInitFromFilename(ob,DeeWideString_STR(filename),__VA_ARGS__)
#endif
#ifndef DeeSysDynlibN_InitFromSelf
#define DeeSysDynlibN_InitFromSelf(ob,...) do{if (!DeeSysDynlibN_TryInitFromSelf(ob)) { DeeError_SET_STRING(&DeeErrorType_SystemError,"Failed to initialize dynlib from self");{__VA_ARGS__;}}}while(0)
#endif
#ifndef DeeSysDynlibN_Quit
#define DeeSysDynlibN_Quit DeeSysDynlib_Quit
#endif
#ifndef DeeSysDynlibN_Utf8TryInitFromFilenameObject
#define DeeSysDynlibN_Utf8TryInitFromFilenameObject(ob,filename) DeeSysDynlibN_Utf8TryInitFromFilename(ob,DeeUtf8String_STR(filename))
#endif
#ifndef DeeSysDynlibN_WideTryInitFromFilenameObject
#define DeeSysDynlibN_WideTryInitFromFilenameObject(ob,filename) DeeSysDynlibN_WideTryInitFromFilename(ob,DeeWideString_STR(filename))
#endif
#ifndef DeeSysDynlibN_Utf8InitFromFilenameObject
#define DeeSysDynlibN_Utf8InitFromFilenameObject(ob,filename,...) DeeSysDynlibN_Utf8InitFromFilename(ob,DeeUtf8String_STR(filename),__VA_ARGS__)
#endif
#ifndef DeeSysDynlibN_WideInitFromFilenameObject
#define DeeSysDynlibN_WideInitFromFilenameObject(ob,filename,...) DeeSysDynlibN_WideInitFromFilename(ob,DeeWideString_STR(filename),__VA_ARGS__)
#endif

#endif /* !GUARD_DEEMON_SYS_SYSDYNLIB_H */
