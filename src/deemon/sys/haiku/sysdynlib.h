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
#ifndef GUARD_DEEMON_SYS_HAIKU_SYSDYNLIB_H
#define GUARD_DEEMON_SYS_HAIKU_SYSDYNLIB_H 1

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/__alloca.inl>
#include <deemon/optional/std/string.h>

//////////////////////////////////////////////////////////////////////////
// === Posix ===
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#ifdef __INTELLISENSE__
typedef int image_id;
typedef int status_t;
#define	B_SYMBOL_TYPE_DATA	1
#define	B_SYMBOL_TYPE_TEXT	2
#define B_SYMBOL_TYPE_ANY	 4
#define B_NO_ERROR 0
static image_id load_add_on(const char *path);
static status_t unload_add_on(image_id image);
static status_t get_image_symbol(
 image_id image, const char *name, __int32 symbolType, void **_symbolLocation);
#else
#include <os/kernel/image.h>
#endif
#include <errno.h>
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#include DEE_INCLUDE_MEMORY_API()

#if 1
#define DEE_HAIKU_DYNLIB_USED_SYMBOLTYPE B_SYMBOL_TYPE_ANY
#else
#define DEE_HAIKU_DYNLIB_USED_SYMBOLTYPE B_SYMBOL_TYPE_TEXT
#endif

DEE_DECL_BEGIN

#if DEE_HAVE_ALLOCA
// NOTE: Make sure the alloca-version isn't inlined
// >> Due to using alloca, it should get inlined in the first place,
//    but we really don't want it to be, as if it still would be,
//    we'd crash if this function was used in a loop or smth.
static DEE_ATTRIBUTE_NOINLINE status_t _haiku_with_underscore(
 image_id lib, char const *name, void **result) {
 char *newname; Dee_size_t namelen;
 namelen = strlen(name);
 newname = (char *)alloca((namelen+2)*sizeof(char));
 memcpy(newname+1,name,namelen*sizeof(char));
 newname[namelen+1] = 0;
 newname[0] = '_';
 return get_image_symbol(lib,newname,DEE_HAIKU_DYNLIB_USED_SYMBOLTYPE,result);
}
#elif DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS
// The other versions should most definitely get inlined
#define _haiku_with_underscore(lib,name,result)\
DEE_GCC_EXTENSION({\
 char *_newname; Dee_size_t _namelen; status_t _error;\
 _namelen = strlen(name);\
 if ((_newname = (char *)malloc_nz((_namelen+2)*sizeof(char))) == NULL) _error = ENOMEM;\
 else {\
  memcpy(_newname+1,name,_namelen*sizeof(char));\
  _newname[_namelen+1] = 0,_newname[0] = '_';\
  _error = get_image_symbol(lib,_newname,DEE_HAIKU_DYNLIB_USED_SYMBOLTYPE,result);\
  free_nn(_newname);\
 }\
 _error;\
})
#else
// *ditto*
DEE_STATIC_INLINE(status_t) _haiku_with_underscore(
 image_id lib, char const *name, void **result) {
 char *newname; Dee_size_t namelen; status_t error;
 namelen = strlen(name);
 if ((newname = (char *)malloc_nz((namelen+2)*sizeof(char))) == NULL) return ENOMEM;
 memcpy(newname+1,name,namelen*sizeof(char));
 newname[namelen+1] = 0;
 newname[0] = '_';
 error = get_image_symbol(lib,newname,DEE_HAIKU_DYNLIB_USED_SYMBOLTYPE,result);
 free_nn(newname);
 return error;
}
#endif

#define DEE_SYSDYNLIB_USES_DLFCN
struct DeeHaikuSysDynlib  { image_id sd_lib; };
#define DeeHaikuSysDynlib_CompareLo(a,b) ((a)->sd_lib < (b)->sd_lib)
#define DeeHaikuSysDynlib_CompareLe(a,b) ((a)->sd_lib <= (b)->sd_lib)
#define DeeHaikuSysDynlib_CompareEq(a,b) ((a)->sd_lib == (b)->sd_lib)
#define DeeHaikuSysDynlib_CompareNe(a,b) ((a)->sd_lib != (b)->sd_lib)
#define DeeHaikuSysDynlib_CompareGr(a,b) ((a)->sd_lib > (b)->sd_lib)
#define DeeHaikuSysDynlib_CompareGe(a,b) ((a)->sd_lib >= (b)->sd_lib)
#define DeeHaikuSysDynlib_Quit(ob)  do{ unload_add_on((ob)->sd_lib); }while(0)
#define DeeHaikuSysDynlib_TryInitFromSelf(ob) \
 (((ob)->sd_lib = load_add_on(NULL)) < 0)
#define DeeHaikuSysDynlib_Utf8TryInitFromFilename(ob,filename) \
 (((ob)->sd_lib = load_add_on(filename)) < 0)
#if DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS
#define DeeHaikuSysDynlib_WideTryInitFromFilename(ob,filename) \
({ int _result; DeeObject *_utf8_filename = DeeUtf8String_FromWideString(filename);\
   if DEE_UNLIKELY(!_utf8_filename) { DeeError_HandledOne(); _result = 0; } else {\
    _result = DeeHaikuSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR(_utf8_filename));\
    Dee_DECREF(_utf8_filename);\
   }\
   _result;\
})
#else /* DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */
#define DeeHaikuSysDynlib_WideTryInitFromFilename DeeHaikuSysDynlib_WideTryInitFromFilename
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED int) DeeHaikuSysDynlib_WideTryInitFromFilename(
 struct DeeHaikuSysDynlib *lib, Dee_WideChar const *filename) {
 int result; DeeObject *utf8_filename = DeeString_FromWideString(filename);
 if DEE_UNLIKELY(!utf8_filename) { DeeError_HandledOne(); result = 0; } else {
  result = DeeHaikuSysDynlib_Utf8TryInitFromFilename(lib,DeeString_STR(utf8_filename));
  Dee_DECREF(utf8_filename);
 }
 return result;
}
#endif /* !DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */
#define DeeHaikuSysDynlib_InitFromSelf(ob,...)\
do{\
 if DEE_UNLIKELY(!DeeHaikuSysDynlib_TryInitFromSelf(ob)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"load_add_on(NULL) : %K",\
                      DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeHaikuSysDynlib_Utf8InitFromFilename(ob,filename,...)\
do{\
 if DEE_UNLIKELY(!DeeHaikuSysDynlib_Utf8TryInitFromFilename(ob,filename)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"load_add_on(%q) : %K",\
                      filename,DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
}while(0)
#define DeeHaikuSysDynlib_WideInitFromFilename(ob,filename,...)\
do{\
 DeeObject *u8filename;\
 if DEE_UNLIKELY((u8filename = DeeString_FromWideString(filename)) == NULL) {__VA_ARGS__;}\
 if DEE_UNLIKELY(!DeeHaikuSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR(u8filename))) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"load_add_on(%q) : %K",\
                      DeeString_STR(u8filename),DeeSystemError_ToString(DeeSystemError_Consume()));\
  Dee_DECREF(u8filename);\
  {__VA_ARGS__;}\
 }\
 Dee_DECREF(u8filename);\
}while(0)

//static status_t get_image_symbol(
// image_id image, const char *name, __int32 symbolType, void **_symbolLocation);
#define DeeHaikuSysDynlib_TryImport(ob,import_name,result) \
      (DEE_UNLIKELY(get_image_symbol((ob)->sd_lib,import_name,DEE_HAIKU_DYNLIB_USED_SYMBOLTYPE,(void **)&(result)) != B_NO_ERROR) \
? DEE_LIKELY((_haiku_with_underscore((ob)->sd_lib,import_name,(void **)&(result))) == B_NO_ERROR) : 1)

#define DeeHaikuSysDynlib_Import(ob,import_name,result,...) \
do{\
 status_t _error = get_image_symbol((ob)->sd_lib,import_name,\
        DEE_HAIKU_DYNLIB_USED_SYMBOLTYPE,(void **)&(result));\
 if DEE_UNLIKELY(_error != B_NO_ERROR) {\
  _error = _haiku_with_underscore((ob)->sd_lib,import_name,(void **)&(result));\
  if DEE_UNLIKELY(_error != B_NO_ERROR) {\
   DeeError_SetStringf(&DeeErrorType_SystemError,"get_image_symbol(%p,%q,B_SYMBOL_TYPE_ANY) : %K",\
                       (void *)(ob)->sd_lib,import_name,DeeSystemError_ToString(_error));\
   {__VA_ARGS__;}\
  }\
 }\
}while(0)



//////////////////////////////////////////////////////////////////////////
// Dynlib w/ filename
struct DeeHaikuSysDynlibN {
 image_id                   sd_lib;
 DEE_A_REF DeeStringObject *sd_name; /*< [1..1] The dynlib filename. */
};
#define DeeHaikuSysDynlibN_Quit(ob) do{ DeeHaikuSysDynlib_Quit(ob); Dee_DECREF((ob)->sd_name); }while(0)
#define DeeHaikuSysDynlibN_TryInitFromSelf(ob) \
 (DeeHaikuSysDynlib_TryInitFromSelf(ob) ? ((ob)->sd_name = (DeeStringObject *)DeeString_NewEmpty(),1) : 0)
#define DeeHaikuSysDynlibN_InitFromSelf(ob,...) \
 do{ DeeHaikuSysDynlib_InitFromSelf(ob,__VA_ARGS__); (ob)->sd_name = (DeeStringObject *)DeeString_NewEmpty(); }while(0)
#define DeeHaikuSysDynlibN_Utf8TryInitFromFilename(ob,filename)\
  (DEE_LIKELY(DeeHaikuSysDynlib_Utf8TryInitFromFilename(ob,filename)) ? (\
 DEE_UNLIKELY(((ob)->sd_name = (DeeStringObject *)DeeString_FromUtf8String(filename)) == NULL)\
 ? (DeeError_HandledOne(),unload_add_on((ob)->sd_lib),0) : 1)) : 0)
#define DeeHaikuSysDynlibN_WideTryInitFromFilename(ob,filename)\
(DEE_UNLIKELY(((ob)->sd_name = (DeeStringObject *)DeeString_FromWideString(filename)) == NULL)\
 ? (DeeError_HandledOne(),0) : (DeeHaikuSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR((ob)->sd_name))\
 ? 1 : (Dee_DecRef((ob)->sd_name),0)))
#define DeeHaikuSysDynlibN_Utf8TryInitFromFilenameObject(ob,filename)\
 (DeeHaikuSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR(filename))\
 ? ((ob)->sd_name = (DeeStringObject *)(filename),_DeeObject_INCREF((ob)->sd_name),1) : 0)
#define DeeHaikuSysDynlibN_WideTryInitFromFilenameObject(ob,filename)\
(DEE_UNLIKELY(((ob)->sd_name = (DeeStringObject *)DeeString_FromWideStringWithLength(DeeWideString_SIZE(filename),DeeWideString_STR(filename))) == NULL)\
 ? (DeeError_HandledOne(),0) : (DeeHaikuSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR((ob)->sd_name))\
 ? 1 : (Dee_DecRef((ob)->sd_name),0)))
#define DeeHaikuSysDynlibN_Utf8InitFromFilename(ob,filename,...)\
do{\
 if DEE_UNLIKELY(((ob)->sd_name = (DeeStringObject *)DeeString_FromUtf8String(filename)) == NULL) {__VA_ARGS__;}\
 if DEE_UNLIKELY(!DeeHaikuSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR((ob)->sd_name))) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"load_add_on(%r) : %K",\
                     (ob)->sd_name,DeeSystemError_ToString(DeeSystemError_Consume()));\
  Dee_DECREF((ob)->sd_name);\
  {__VA_ARGS__;}\
 }\
 Dee_DECREF((ob)->sd_name);\
}while(0)
#define DeeHaikuSysDynlibN_WideInitFromFilename(ob,filename,...)\
do{\
 if DEE_UNLIKELY(((ob)->sd_name = (DeeStringObject *)DeeString_FromWideString(filename)) == NULL) {__VA_ARGS__;}\
 if DEE_UNLIKELY(!DeeHaikuSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR((ob)->sd_name))) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"load_add_on(%r) : %K",\
                     (ob)->sd_name,DeeSystemError_ToString(DeeSystemError_Consume()));\
  Dee_DECREF((ob)->sd_name);\
  {__VA_ARGS__;}\
 }\
 Dee_DECREF((ob)->sd_name);\
}while(0)
#define DeeHaikuSysDynlibN_Utf8InitFromFilenameObject(ob,filename,...)\
do{\
 if DEE_UNLIKELY(!DeeHaikuSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR(filename))) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"load_add_on(%r) : %K",\
                     filename,DeeSystemError_ToString(DeeSystemError_Consume()));\
  {__VA_ARGS__;}\
 }\
 Dee_INCREF((ob)->sd_name = (DeeStringObject *)(filename));\
}while(0)
#define DeeHaikuSysDynlibN_WideInitFromFilenameObject(ob,filename,...)\
do{\
 if DEE_UNLIKELY(((ob)->sd_name = (DeeStringObject *)DeeString_FromWideStringWithLength(\
  DeeWideString_SIZE(filename),DeeWideString_STR(filename))) == NULL) {__VA_ARGS__;}\
 if DEE_UNLIKELY(!DeeHaikuSysDynlib_Utf8TryInitFromFilename(ob,DeeString_STR((ob)->sd_name))) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,"load_add_on(%r) : %K",\
                     (ob)->sd_name,DeeSystemError_ToString(DeeSystemError_Consume()));\
  Dee_DECREF((ob)->sd_name);\
  {__VA_ARGS__;}\
 }\
 Dee_DECREF((ob)->sd_name);\
}while(0)
#define DeeHaikuSysDynlibN_Name(ob)     (Dee_IncRef((DeeStringObject *)(ob)->sd_name),(DeeObject *)(ob)->sd_name)
#define DeeHaikuSysDynlibN_Utf8Name(ob) (Dee_IncRef((DeeStringObject *)(ob)->sd_name),(DeeObject *)(ob)->sd_name)
#define DeeHaikuSysDynlibN_WideName(ob) DeeWideString_Cast((DeeObject *)(ob)->sd_name)



#define DeeSysDynlib                                DeeHaikuSysDynlib
#define DeeSysDynlibN                               DeeHaikuSysDynlibN 
#define DeeSysDynlib_CompareLo                      DeeHaikuSysDynlib_CompareLo
#define DeeSysDynlib_CompareLe                      DeeHaikuSysDynlib_CompareLe
#define DeeSysDynlib_CompareEq                      DeeHaikuSysDynlib_CompareEq
#define DeeSysDynlib_CompareNe                      DeeHaikuSysDynlib_CompareNe
#define DeeSysDynlib_CompareGr                      DeeHaikuSysDynlib_CompareGr
#define DeeSysDynlib_CompareGe                      DeeHaikuSysDynlib_CompareGe
#define DeeSysDynlib_TryInitFromSelf                DeeHaikuSysDynlib_TryInitFromSelf
#define DeeSysDynlib_InitFromSelf                   DeeHaikuSysDynlib_InitFromSelf
#define DeeSysDynlib_Utf8TryInitFromFilename        DeeHaikuSysDynlib_Utf8TryInitFromFilename
#define DeeSysDynlib_WideTryInitFromFilename        DeeHaikuSysDynlib_WideTryInitFromFilename
#define DeeSysDynlib_Utf8InitFromFilename           DeeHaikuSysDynlib_Utf8InitFromFilename
#define DeeSysDynlib_WideInitFromFilename           DeeHaikuSysDynlib_WideInitFromFilename
#define DeeSysDynlib_TryImport                      DeeHaikuSysDynlib_TryImport
#define DeeSysDynlib_Import                         DeeHaikuSysDynlib_Import
#define DeeSysDynlib_Quit                           DeeHaikuSysDynlib_Quit
#define DeeSysDynlibN_TryInitFromSelf               DeeHaikuSysDynlibN_TryInitFromSelf
#define DeeSysDynlibN_InitFromSelf                  DeeHaikuSysDynlibN_InitFromSelf
#define DeeSysDynlibN_Utf8TryInitFromFilename       DeeHaikuSysDynlibN_Utf8TryInitFromFilename
#define DeeSysDynlibN_WideTryInitFromFilename       DeeHaikuSysDynlibN_WideTryInitFromFilename
#define DeeSysDynlibN_Utf8TryInitFromFilenameObject DeeHaikuSysDynlibN_Utf8TryInitFromFilenameObject
#define DeeSysDynlibN_WideTryInitFromFilenameObject DeeHaikuSysDynlibN_WideTryInitFromFilenameObject
#define DeeSysDynlibN_Utf8InitFromFilename          DeeHaikuSysDynlibN_Utf8InitFromFilename
#define DeeSysDynlibN_WideInitFromFilename          DeeHaikuSysDynlibN_WideInitFromFilename
#define DeeSysDynlibN_Utf8InitFromFilenameObject    DeeHaikuSysDynlibN_Utf8InitFromFilenameObject
#define DeeSysDynlibN_WideInitFromFilenameObject    DeeHaikuSysDynlibN_WideInitFromFilenameObject
#define DeeSysDynlibN_Name                          DeeHaikuSysDynlibN_Name
#define DeeSysDynlibN_Utf8Name                      DeeHaikuSysDynlibN_Utf8Name
#define DeeSysDynlibN_WideName                      DeeHaikuSysDynlibN_WideName
#define DeeSysDynlibN_Quit                          DeeHaikuSysDynlibN_Quit

DEE_DECL_END


#endif /* !GUARD_DEEMON_SYS_HAIKU_SYSDYNLIB_H */
