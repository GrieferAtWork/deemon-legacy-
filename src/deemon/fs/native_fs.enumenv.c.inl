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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_ENUMENV_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_ENUMENV_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/list.h>
#include <deemon/fs/native_fs.h>
#include <deemon/sys/sysfs.h>

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_ENUMENV
DeeError_NEW_STATIC(_dee_notimplemented_enumenv,&DeeErrorType_NotImplemented,"enumenv");
#endif
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeNFS_WideEnumEnv(void) {
#ifdef DeeSysFS_Utf8EnumEnv
 DeeObject *result;
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) return NULL;
#define DO_ENUM(key_size,key,val_size,val,...)\
do{\
 DeeObject *used_key,*used_val,*pair; int error;\
 if DEE_UNLIKELY((used_key = DeeUtf8String_NewWithLength(key_size,key)) == NULL) {__VA_ARGS__;}\
 if DEE_UNLIKELY((used_val = DeeUtf8String_NewWithLength(val_size,val)) == NULL) {Dee_DECREF(used_key);{__VA_ARGS__;}}\
 pair = DeeTuple_Pack(2,used_key,used_val);\
 Dee_DECREF(used_val);\
 Dee_DECREF(used_key);\
 if DEE_UNLIKELY(!pair) {__VA_ARGS__;}\
 error = DeeList_Append(result,pair);\
 Dee_DECREF(pair);\
 if DEE_UNLIKELY(error != 0) {__VA_ARGS__;}\
}while(0)
 DeeSysFS_Utf8EnumEnv(DO_ENUM,{ Dee_DECREF(result); return NULL; });
#undef DO_ENUM
 return result;
#elif defined(DeeSysFS_WideEnumEnv)
 DeeObject *result;
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) return NULL;
#define DO_ENUM(key_size,key,val_size,val,...)\
do{\
 DeeObject *used_key,*used_val,*pair; int error;\
 if DEE_UNLIKELY((used_key = DeeUtf8String_FromWideStringWithLength(key_size,key)) == NULL) {__VA_ARGS__;}\
 if DEE_UNLIKELY((used_val = DeeUtf8String_FromWideStringWithLength(val_size,val)) == NULL) {Dee_DECREF(used_key);{__VA_ARGS__;}}\
 pair = DeeTuple_Pack(2,used_key,used_val);\
 Dee_DECREF(used_val);\
 Dee_DECREF(used_key);\
 if DEE_UNLIKELY(!pair) {__VA_ARGS__;}\
 error = DeeList_Append(result,pair);\
 Dee_DECREF(pair);\
 if DEE_UNLIKELY(error != 0) {__VA_ARGS__;}\
}while(0)
 DeeSysFS_WideEnumEnv(DO_ENUM,{ Dee_DECREF(result); return NULL; });
#undef DO_ENUM
 return result;
#else
 DeeError_Throw((DeeObject *)&_dee_notimplemented_enumenv);
 return NULL;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeNFS_Utf8EnumEnv(void) {
#ifdef DeeSysFS_WideEnumEnv
 DeeObject *result;
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) return NULL;
#define DO_ENUM(key_size,key,val_size,val,...)\
do{\
 DeeObject *used_key,*used_val,*pair; int error;\
 if DEE_UNLIKELY((used_key = DeeWideString_NewWithLength(key_size,key)) == NULL) {__VA_ARGS__;}\
 if DEE_UNLIKELY((used_val = DeeWideString_NewWithLength(val_size,val)) == NULL) {Dee_DECREF(used_key);{__VA_ARGS__;}}\
 pair = DeeTuple_Pack(2,used_key,used_val);\
 Dee_DECREF(used_val);\
 Dee_DECREF(used_key);\
 if DEE_UNLIKELY(!pair) {__VA_ARGS__;}\
 error = DeeList_Append(result,pair);\
 Dee_DECREF(pair);\
 if DEE_UNLIKELY(error != 0) {__VA_ARGS__;}\
}while(0)
 DeeSysFS_WideEnumEnv(DO_ENUM,{ Dee_DECREF(result); return NULL; });
#undef DO_ENUM
 return result;
#elif defined(DeeSysFS_Utf8EnumEnv)
 DeeObject *result;
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) return NULL;
#define DO_ENUM(key_size,key,val_size,val,...)\
do{\
 DeeObject *used_key,*used_val,*pair; int error;\
 if DEE_UNLIKELY((used_key = DeeWideString_FromUtf8StringWithLength(key_size,key)) == NULL) {__VA_ARGS__;}\
 if DEE_UNLIKELY((used_val = DeeWideString_FromUtf8StringWithLength(val_size,val)) == NULL) {Dee_DECREF(used_key);{__VA_ARGS__;}}\
 pair = DeeTuple_Pack(2,used_key,used_val);\
 Dee_DECREF(used_val);\
 Dee_DECREF(used_key);\
 if DEE_UNLIKELY(!pair) {__VA_ARGS__;}\
 error = DeeList_Append(result,pair);\
 Dee_DECREF(pair);\
 if DEE_UNLIKELY(error != 0) {__VA_ARGS__;}\
}while(0)
 DeeSysFS_Utf8EnumEnv(DO_ENUM,{ Dee_DECREF(result); return NULL; });
#undef DO_ENUM
 return result;
#else
 DeeError_Throw((DeeObject *)&_dee_notimplemented_enumenv);
 return NULL;
#endif
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_ENUMENV_C_INL */
