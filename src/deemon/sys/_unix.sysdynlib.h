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
#ifndef GUARD_DEEMON_SYS__UNIX_SYSDYNLIB_H
#define GUARD_DEEMON_SYS__UNIX_SYSDYNLIB_H 1

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/__alloca.inl>
#include <deemon/optional/std/string.h>

//////////////////////////////////////////////////////////////////////////
// === Posix ===
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#ifdef __INTELLISENSE__
static void *dlopen(const char *filename, int flag);
static char *dlerror(void);
static void *dlsym(void *handle, const char *symbol);
static int dlclose(void *handle);
#define RTLD_LAZY     0
#define RTLD_NOW      1
#define RTLD_GLOBAL   4
#define RTLD_LOCAL    8
#define RTLD_NODELETE 16
#define RTLD_NOLOAD   32
#define RTLD_DEEPBIND 64
#else
#include <dlfcn.h>
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

#if DEE_HAVE_ALLOCA
// NOTE: Make sure the alloca-version isn't inlined
// >> Due to using alloca, it should get inlined in the first place,
//    but we really don't want it to be, as if it still would be,
//    we'd crash if this function was used in a loop or smth.
static DEE_ATTRIBUTE_NOINLINE void *_dlsym_with_underscore(void *lib, char const *name) {
 char *newname; Dee_size_t namelen;
 namelen = strlen(name);
 newname = (char *)alloca((namelen+2)*sizeof(char));
 memcpy(newname+1,name,namelen*sizeof(char));
 newname[namelen+1] = 0;
 newname[0] = '_';
 return dlsym(lib,newname);
}
#elif DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS
// The other versions should most definitely get inlined
#define _dlsym_with_underscore(lib,name)\
DEE_GCC_EXTENSION({\
 char *_newname; Dee_size_t _namelen; void *_result;\
 _namelen = strlen(name);\
 if ((_newname = (char *)malloc_nz((_namelen+2)*sizeof(char))) == NULL) _result = NULL;\
 else {\
  memcpy(_newname+1,name,_namelen*sizeof(char));\
  _newname[_namelen+1] = 0,_newname[0] = '_';\
  _result = dlsym(lib,_newname);\
  free_nn(_newname);\
 }\
 _result;\
})
#else
// *ditto*
DEE_STATIC_INLINE(void *) _dlsym_with_underscore(void *lib, char const *name) {
 char *newname; Dee_size_t namelen; void *result;
 namelen = strlen(name);
 if ((newname = (char *)malloc_nz((namelen+2)*sizeof(char))) == NULL) return NULL;
 memcpy(newname+1,name,namelen*sizeof(char));
 newname[namelen+1] = 0;
 newname[0] = '_';
 result = dlsym(lib,newname);
 free_nn(newname);
 return result;
}
#endif

#define DEE_SYSDYNLIB_USES_DLFCN
struct DeePosixSysDynlib  { void *sd_lib; };
#define DeePosixSysDynlib_CompareLo(a,b) ((a)->sd_lib < (b)->sd_lib)
#define DeePosixSysDynlib_CompareLe(a,b) ((a)->sd_lib <= (b)->sd_lib)
#define DeePosixSysDynlib_CompareEq(a,b) ((a)->sd_lib == (b)->sd_lib)
#define DeePosixSysDynlib_CompareNe(a,b) ((a)->sd_lib != (b)->sd_lib)
#define DeePosixSysDynlib_CompareGr(a,b) ((a)->sd_lib > (b)->sd_lib)
#define DeePosixSysDynlib_CompareGe(a,b) ((a)->sd_lib >= (b)->sd_lib)
#define DeePosixSysDynlib_Quit(ob)  do{ dlclose((ob)->sd_lib); }while(0)
#define DeePosixSysDynlib_TryInitFromSelf(ob) \
 (((ob)->sd_lib = dlopen(NULL,RTLD_LAZY)) != NULL)
#define DeePosixSysDynlib_Utf8TryInitFromFilename(ob,filename) \
 (((ob)->sd_lib = dlopen(filename,RTLD_NOW|RTLD_GLOBAL)) != NULL)
#if DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS
#define DeePosixSysDynlib_WideTryInitFromFilename(ob,filename) \
({ int _result; DeeObject *_utf8_filename = DeeUtf8String_FromWideString(filename);\
   if DEE_UNLIKELY(!_utf8_filename) { DeeError_HandledOne(); _result = 0; } else {\
    _result = DeePosixSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR(_utf8_filename));\
    Dee_DECREF(_utf8_filename);\
   }\
   _result;\
})
#else /* DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */
#define DeePosixSysDynlib_WideTryInitFromFilename DeePosixSysDynlib_WideTryInitFromFilename
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) DeePosixSysDynlib_WideTryInitFromFilename(
 struct DeePosixSysDynlib *lib, Dee_WideChar const *filename) {
 int result; DeeObject *utf8_filename = DeeString_FromWideString(filename);
 if DEE_UNLIKELY(!utf8_filename) { DeeError_HandledOne(); result = 0; } else {
  result = DeePosixSysDynlib_Utf8TryInitFromFilename(lib,DeeString_STR(utf8_filename));
  Dee_DECREF(utf8_filename);
 }
 return result;
}
#endif /* !DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */
#define DeePosixSysDynlib_InitFromSelf(ob,...)\
do{\
 if DEE_UNLIKELY(!DeePosixSysDynlib_TryInitFromSelf(ob)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"dlopen(NULL,RTLD_LAZY) : %s",dlerror());\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeePosixSysDynlib_Utf8InitFromFilename(ob,filename,...)\
do{\
 if DEE_UNLIKELY(!DeePosixSysDynlib_Utf8TryInitFromFilename(ob,filename)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"dlopen(%q,RTLD_NOW|RTLD_GLOBAL) : %s",\
                     (filename),dlerror());\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeePosixSysDynlib_WideInitFromFilename(ob,filename,...)\
do{\
 DeeObject *u8filename;\
 if DEE_UNLIKELY((u8filename = DeeString_FromWideString(filename)) == NULL) {__VA_ARGS__;}\
 if DEE_UNLIKELY(!DeePosixSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR(u8filename))) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"dlopen(%q,RTLD_NOW|RTLD_GLOBAL) : %s",\
                      DeeString_STR(u8filename),dlerror());\
  Dee_DECREF(u8filename);\
  {__VA_ARGS__;}\
 }\
 Dee_DECREF(u8filename);\
}while(0)

#define DeePosixSysDynlib_TryImport(ob,import_name,result) \
 ((*(void **)&(result) = (void *)dlsym((ob)->sd_lib,import_name)) == NULL\
? (*(void **)&(result) = _dlsym_with_underscore((ob)->sd_lib,import_name)) != NULL : 1)

#define DeePosixSysDynlib_Import(ob,import_name,result,...) \
do{\
 if DEE_UNLIKELY(!DeePosixSysDynlib_TryImport(ob,import_name,result)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"dlsym(%p,%q) : %s",\
                      (ob)->sd_lib,import_name,dlerror());\
  {__VA_ARGS__;}\
 }\
}while(0)



//////////////////////////////////////////////////////////////////////////
// Dynlib w/ filename
struct DeePosixSysDynlibN {
 void                      *sd_lib;
 DEE_A_REF DeeStringObject *sd_name; /*< [1..1] The dynlib filename. */
};
#define DeePosixSysDynlibN_Quit(ob) do{ DeePosixSysDynlib_Quit(ob); Dee_DECREF((ob)->sd_name); }while(0)
#define DeePosixSysDynlibN_TryInitFromSelf(ob) \
 (DeePosixSysDynlib_TryInitFromSelf(ob) ? ((ob)->sd_name = (DeeStringObject *)DeeString_NewEmpty(),1) : 0)
#define DeePosixSysDynlibN_InitFromSelf(ob,...) \
 do{ DeePosixSysDynlib_InitFromSelf(ob,__VA_ARGS__); (ob)->sd_name = (DeeStringObject *)DeeString_NewEmpty(); }while(0)
#define DeePosixSysDynlibN_Utf8TryInitFromFilename(ob,filename)\
  (DEE_LIKELY(DeePosixSysDynlib_Utf8TryInitFromFilename(ob,filename)) ? (\
 DEE_UNLIKELY(((ob)->sd_name = (DeeStringObject *)DeeString_FromUtf8String(filename)) == NULL)\
 ? (DeeError_HandledOne(),dlclose((ob)->sd_lib),0) : 1)) : 0)
#define DeePosixSysDynlibN_WideTryInitFromFilename(ob,filename)\
(DEE_UNLIKELY(((ob)->sd_name = (DeeStringObject *)DeeString_FromWideString(filename)) == NULL)\
 ? (DeeError_HandledOne(),0) : (DeePosixSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR((ob)->sd_name))\
 ? 1 : (Dee_DecRef((ob)->sd_name),0)))
#define DeePosixSysDynlibN_Utf8TryInitFromFilenameObject(ob,filename)\
 (DeePosixSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR(filename))\
 ? ((ob)->sd_name = (DeeStringObject *)(filename),_DeeObject_INCREF((ob)->sd_name),1) : 0)
#define DeePosixSysDynlibN_WideTryInitFromFilenameObject(ob,filename)\
(DEE_UNLIKELY(((ob)->sd_name = (DeeStringObject *)DeeString_FromWideStringWithLength(DeeWideString_SIZE(filename),DeeWideString_STR(filename))) == NULL)\
 ? (DeeError_HandledOne(),0) : (DeePosixSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR((ob)->sd_name))\
 ? 1 : (Dee_DecRef((ob)->sd_name),0)))
#define DeePosixSysDynlibN_Utf8InitFromFilename(ob,filename,...)\
do{\
 if DEE_UNLIKELY(((ob)->sd_name = (DeeStringObject *)DeeString_FromUtf8String(filename)) == NULL) {__VA_ARGS__;}\
 if DEE_UNLIKELY(!DeePosixSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR((ob)->sd_name))) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"dlopen(%r,RTLD_NOW|RTLD_GLOBAL) : %s",\
                     (ob)->sd_name,dlerror());\
  Dee_DECREF((ob)->sd_name);\
  {__VA_ARGS__;}\
 }\
 Dee_DECREF((ob)->sd_name);\
}while(0)
#define DeePosixSysDynlibN_WideInitFromFilename(ob,filename,...)\
do{\
 if DEE_UNLIKELY(((ob)->sd_name = (DeeStringObject *)DeeString_FromWideString(filename)) == NULL) {__VA_ARGS__;}\
 if DEE_UNLIKELY(!DeePosixSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR((ob)->sd_name))) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"dlopen(%r,RTLD_NOW|RTLD_GLOBAL) : %s",\
                     (ob)->sd_name,dlerror());\
  Dee_DECREF((ob)->sd_name);\
  {__VA_ARGS__;}\
 }\
 Dee_DECREF((ob)->sd_name);\
}while(0)
#define DeePosixSysDynlibN_Utf8InitFromFilenameObject(ob,filename,...)\
do{\
 if DEE_UNLIKELY(!DeePosixSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR(filename))) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"dlopen(%r,RTLD_NOW|RTLD_GLOBAL) : %s",\
                     filename,dlerror());\
  {__VA_ARGS__;}\
 }\
 Dee_INCREF((ob)->sd_name = (DeeStringObject *)(filename));\
}while(0)
#define DeePosixSysDynlibN_WideInitFromFilenameObject(ob,filename,...)\
do{\
 if DEE_UNLIKELY(((ob)->sd_name = (DeeStringObject *)DeeString_FromWideStringWithLength(\
  DeeWideString_SIZE(filename),DeeWideString_STR(filename))) == NULL) {__VA_ARGS__;}\
 if DEE_UNLIKELY(!DeePosixSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR((ob)->sd_name))) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"dlopen(%r,RTLD_NOW|RTLD_GLOBAL) : %s",\
                     (ob)->sd_name,dlerror());\
  Dee_DECREF((ob)->sd_name);\
  {__VA_ARGS__;}\
 }\
 Dee_DECREF((ob)->sd_name);\
}while(0)
#define DeePosixSysDynlibN_Name(ob)     (Dee_IncRef((DeeStringObject *)(ob)->sd_name),(DeeObject *)(ob)->sd_name)
#define DeePosixSysDynlibN_Utf8Name(ob) (Dee_IncRef((DeeStringObject *)(ob)->sd_name),(DeeObject *)(ob)->sd_name)
#define DeePosixSysDynlibN_WideName(ob) DeeWideString_Cast((DeeObject *)(ob)->sd_name)



#define DeeSysDynlib                                DeePosixSysDynlib
#define DeeSysDynlibN                               DeePosixSysDynlibN 
#define DeeSysDynlib_CompareLo                      DeePosixSysDynlib_CompareLo
#define DeeSysDynlib_CompareLe                      DeePosixSysDynlib_CompareLe
#define DeeSysDynlib_CompareEq                      DeePosixSysDynlib_CompareEq
#define DeeSysDynlib_CompareNe                      DeePosixSysDynlib_CompareNe
#define DeeSysDynlib_CompareGr                      DeePosixSysDynlib_CompareGr
#define DeeSysDynlib_CompareGe                      DeePosixSysDynlib_CompareGe
#define DeeSysDynlib_TryInitFromSelf                DeePosixSysDynlib_TryInitFromSelf
#define DeeSysDynlib_InitFromSelf                   DeePosixSysDynlib_InitFromSelf
#define DeeSysDynlib_Utf8TryInitFromFilename        DeePosixSysDynlib_Utf8TryInitFromFilename
#define DeeSysDynlib_WideTryInitFromFilename        DeePosixSysDynlib_WideTryInitFromFilename
#define DeeSysDynlib_Utf8InitFromFilename           DeePosixSysDynlib_Utf8InitFromFilename
#define DeeSysDynlib_WideInitFromFilename           DeePosixSysDynlib_WideInitFromFilename
#define DeeSysDynlib_TryImport                      DeePosixSysDynlib_TryImport
#define DeeSysDynlib_Import                         DeePosixSysDynlib_Import
#define DeeSysDynlib_Quit                           DeePosixSysDynlib_Quit
#define DeeSysDynlibN_TryInitFromSelf               DeePosixSysDynlibN_TryInitFromSelf
#define DeeSysDynlibN_InitFromSelf                  DeePosixSysDynlibN_InitFromSelf
#define DeeSysDynlibN_Utf8TryInitFromFilename       DeePosixSysDynlibN_Utf8TryInitFromFilename
#define DeeSysDynlibN_WideTryInitFromFilename       DeePosixSysDynlibN_WideTryInitFromFilename
#define DeeSysDynlibN_Utf8TryInitFromFilenameObject DeePosixSysDynlibN_Utf8TryInitFromFilenameObject
#define DeeSysDynlibN_WideTryInitFromFilenameObject DeePosixSysDynlibN_WideTryInitFromFilenameObject
#define DeeSysDynlibN_Utf8InitFromFilename          DeePosixSysDynlibN_Utf8InitFromFilename
#define DeeSysDynlibN_WideInitFromFilename          DeePosixSysDynlibN_WideInitFromFilename
#define DeeSysDynlibN_Utf8InitFromFilenameObject    DeePosixSysDynlibN_Utf8InitFromFilenameObject
#define DeeSysDynlibN_WideInitFromFilenameObject    DeePosixSysDynlibN_WideInitFromFilenameObject
#define DeeSysDynlibN_Name                          DeePosixSysDynlibN_Name
#define DeeSysDynlibN_Utf8Name                      DeePosixSysDynlibN_Utf8Name
#define DeeSysDynlibN_WideName                      DeePosixSysDynlibN_WideName
#define DeeSysDynlibN_Quit                          DeePosixSysDynlibN_Quit

DEE_DECL_END


#endif /* !GUARD_DEEMON_SYS__UNIX_SYSDYNLIB_H */
