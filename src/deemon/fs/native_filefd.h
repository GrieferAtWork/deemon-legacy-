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
#ifndef GUARD_DEEMON_FS_NATIVE_FILEFD_H
#define GUARD_DEEMON_FS_NATIVE_FILEFD_H 1

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/sys/sysfd.h>

DEE_DECL_BEGIN

#ifdef DeeSysFileFD
#define DeeNativeFileFD                   DeeSysFileFD
#if defined(DeeSysFileFD_Quit) && defined(DeeSysFD_Quit)
# define DeeNativeFileFD_Quit(self) \
  do{ DeeSysFileFD_Quit(self); DeeSysFD_Quit(self); }while(0)
#elif defined(DeeSysFileFD_Quit)
# define DeeNativeFileFD_Quit DeeSysFileFD_Quit
#elif defined(DeeSysFD_Quit)
# define DeeNativeFileFD_Quit DeeSysFD_Quit
#else
# define DeeNativeFileFD_Quit(self) do{}while(0)
#endif


#ifdef DeeSysFileFD_Utf8TryInit
#define DeeNativeFileFD_Utf8TryInit       DeeSysFileFD_Utf8TryInit
#else
DEE_STATIC_INLINE(int) DeeNativeFileFD_Utf8TryInit(
 DEE_A_OUT struct DeeNativeFileFD *self, DEE_A_IN_Z Dee_Utf8Char const *filename,
 DEE_A_IN Dee_openmode_t mode, DEE_A_IN Dee_mode_t perms);
#endif
#ifdef DeeSysFileFD_WideTryInit
#define DeeNativeFileFD_WideTryInit       DeeSysFileFD_WideTryInit
#else
DEE_STATIC_INLINE(int) DeeNativeFileFD_WideTryInit(
 DEE_A_OUT struct DeeNativeFileFD *self, DEE_A_IN_Z Dee_WideChar const *filename,
 DEE_A_IN Dee_openmode_t mode, DEE_A_IN Dee_mode_t perms);
#endif
#ifdef DeeSysFileFD_Utf8TryInitObject
#define DeeNativeFileFD_Utf8TryInitObject DeeSysFileFD_Utf8TryInitObject
#else
#define DeeNativeFileFD_Utf8TryInitObject(self,filename,mode,perms) \
 DeeNativeFileFD_Utf8TryInit(self,DeeUtf8String_STR(filename),mode,perms)
#endif
#ifdef DeeSysFileFD_WideTryInitObject
#define DeeNativeFileFD_WideTryInitObject DeeSysFileFD_WideTryInitObject
#else
#define DeeNativeFileFD_WideTryInitObject(self,filename,mode,perms) \
 DeeNativeFileFD_WideTryInit(self,DeeWideString_STR(filename),mode,perms)
#endif
#ifdef DeeSysFileFD_Utf8Init
#define DeeNativeFileFD_Utf8Init          DeeSysFileFD_Utf8Init
#elif defined(DeeSysFileFD_Utf8InitObject)
#define DeeNativeFileFD_Utf8Init(self,filename,mode,perms,...) \
do{\
 DeeObject *_fp_filenameob;\
 if DEE_UNLIKELY((_fp_filenameob = DeeUtf8String_New(filename)) == NULL) {__VA_ARGS__;}\
 DeeSysFileFD_Utf8InitObject(self,_fp_filenameob,mode,perms,{Dee_DECREF(_fp_filenameob);{__VA_ARGS__;}});\
 Dee_DECREF(_fp_filenameob);\
}while(0)
#elif defined(DeeSysFileFD_WideInitObject)
#define DeeNativeFileFD_Utf8Init(self,filename,mode,perms,...) \
do{\
 DeeObject *_fp_filenameob;\
 if DEE_UNLIKELY((_fp_filenameob = DeeWideString_FromUtf8String(filename)) == NULL) {__VA_ARGS__;}\
 DeeSysFileFD_WideInitObject(self,_fp_filenameob,mode,perms,{Dee_DECREF(_fp_filenameob);{__VA_ARGS__;}});\
 Dee_DECREF(_fp_filenameob);\
}while(0)
#else
#define DeeNativeFileFD_Utf8Init(self,filename,mode,perms,...) \
do{\
 if (!DeeNativeFileFD_Utf8TryInit(self,filename,mode,perms)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "Failed to open %q",filename);\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif
#ifdef DeeSysFileFD_WideInit
#define DeeNativeFileFD_WideInit          DeeSysFileFD_WideInit
#elif defined(DeeSysFileFD_WideInitObject)
#define DeeNativeFileFD_WideInit(self,filename,mode,perms,...) \
do{\
 DeeObject *_fp_filenameob;\
 if DEE_UNLIKELY((_fp_filenameob = DeeWideString_New(filename)) == NULL) {__VA_ARGS__;}\
 DeeSysFileFD_WideInitObject(self,_fp_filenameob,mode,perms,{Dee_DECREF(_fp_filenameob);{__VA_ARGS__;}});\
 Dee_DECREF(_fp_filenameob);\
}while(0)
#elif defined(DeeSysFileFD_Utf8InitObject)
#define DeeNativeFileFD_WideInit(self,filename,mode,perms,...) \
do{\
 DeeObject *_fp_filenameob;\
 if DEE_UNLIKELY((_fp_filenameob = DeeUtf8String_FromWideString(filename)) == NULL) {__VA_ARGS__;}\
 DeeSysFileFD_Utf8InitObject(self,_fp_filenameob,mode,perms,{Dee_DECREF(_fp_filenameob);{__VA_ARGS__;}});\
 Dee_DECREF(_fp_filenameob);\
}while(0)
#else
#define DeeNativeFileFD_WideInit(self,filename,mode,perms,...) \
do{\
 if (!DeeNativeFileFD_WideTryInit(self,filename,mode,perms)) {\
  DeeError_SetStringf(&DeeErrorType_SystemError,\
                      "Failed to open %lq",filename);\
  {__VA_ARGS__;}\
 }\
}while(0)
#endif

#ifdef DeeSysFileFD_Utf8InitObject
#define DeeNativeFileFD_Utf8InitObject    DeeSysFileFD_Utf8InitObject
#else
#define DeeNativeFileFD_Utf8InitObject(self,filename,mode,perms,...)\
 DeeNativeFileFD_Utf8Init(self,DeeUtf8String_STR(filename),mode,perms,__VA_ARGS__)
#endif
#ifdef DeeSysFileFD_WideInitObject
#define DeeNativeFileFD_WideInitObject    DeeSysFileFD_WideInitObject
#else
#define DeeNativeFileFD_WideInitObject(self,filename,mode,perms,...)\
 DeeNativeFileFD_WideInit(self,DeeWideString_STR(filename),mode,perms,__VA_ARGS__)
#endif


#ifndef DeeNativeFileFD_Utf8TryInit
DEE_STATIC_INLINE(int) DeeNativeFileFD_Utf8TryInit(
 DEE_A_OUT struct DeeNativeFileFD *self, DEE_A_IN_Z Dee_Utf8Char const *filename,
 DEE_A_IN Dee_openmode_t mode, DEE_A_IN Dee_mode_t perms) {
#ifdef DeeSysFileFD_Utf8TryInitObject
 DeeObject *filename_ob; int result;
 if ((filename_ob = DeeUtf8String_New(filename)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFileFD_Utf8TryInitObject(self,filename_ob,mode,perms);
 Dee_DECREF(filename_ob);
 return result;
#elif defined(DeeSysFileFD_WideTryInitObject)
 DeeObject *filename_ob; int result;
 if ((filename_ob = DeeWideString_FromUtf8String(filename)) == NULL) { DeeError_HandledOne(); return 0; }
 result = DeeSysFileFD_WideTryInitObject(self,filename_ob,mode,perms);
 Dee_DECREF(filename_ob);
 return result;
#else
 DeeNativeFileFD_Utf8Init(self,filename,mode,perms,{ DeeError_HandledOne(); return 0; });
 return 1;
#endif
}
#endif

#ifdef DeeSysFileFD_TryRead
# define DeeNativeFileFD_TryRead  DeeSysFileFD_TryRead
#elif defined(DeeSysFD_TryRead)
# define DeeNativeFileFD_TryRead  DeeSysFD_TryRead
#endif
#ifdef DeeSysFileFD_TryWrite
# define DeeNativeFileFD_TryWrite DeeSysFileFD_TryWrite
#elif defined(DeeSysFD_TryWrite)
# define DeeNativeFileFD_TryWrite DeeSysFD_TryWrite
#endif
#ifdef DeeSysFileFD_TrySeek
# define DeeNativeFileFD_TrySeek  DeeSysFileFD_TrySeek
#elif defined(DeeSysFD_TrySeek)
# define DeeNativeFileFD_TrySeek  DeeSysFD_TrySeek
#endif
#ifdef DeeSysFileFD_TryFlush
# define DeeNativeFileFD_TryFlush DeeSysFileFD_TryFlush
#elif defined(DeeSysFD_TryFlush)
# define DeeNativeFileFD_TryFlush DeeSysFD_TryFlush
#endif
#ifdef DeeSysFileFD_TryTrunc
# define DeeNativeFileFD_TryTrunc DeeSysFileFD_TryTrunc
#elif defined(DeeSysFD_TryTrunc)
# define DeeNativeFileFD_TryTrunc DeeSysFD_TryTrunc
#endif

#ifdef DeeSysFileFD_Read
# define DeeNativeFileFD_Read  DeeSysFileFD_Read
#elif defined(DeeSysFD_Read)
# define DeeNativeFileFD_Read  DeeSysFD_Read
#endif
#ifdef DeeSysFileFD_Write
# define DeeNativeFileFD_Write DeeSysFileFD_Write
#elif defined(DeeSysFD_Write)
# define DeeNativeFileFD_Write DeeSysFD_Write
#endif
#ifdef DeeSysFileFD_Seek
# define DeeNativeFileFD_Seek  DeeSysFileFD_Seek
#elif defined(DeeSysFD_Seek)
# define DeeNativeFileFD_Seek  DeeSysFD_Seek
#endif
#ifdef DeeSysFileFD_Flush
# define DeeNativeFileFD_Flush DeeSysFileFD_Flush
#elif defined(DeeSysFD_Flush)
# define DeeNativeFileFD_Flush DeeSysFD_Flush
#endif
#ifdef DeeSysFileFD_Trunc
# define DeeNativeFileFD_Trunc DeeSysFileFD_Trunc
#elif defined(DeeSysFD_Trunc)
# define DeeNativeFileFD_Trunc DeeSysFD_Trunc
#endif

#endif /* DeeSysFileFD */

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FILEFD_H */
