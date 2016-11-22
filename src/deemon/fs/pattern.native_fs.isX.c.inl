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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_{name.upper()}_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_{name.upper()}_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/sys/sysfs.h>
#include <deemon/sys/sysfd.h>
#include <deemon/fs/native_filefd.h>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_{name.upper()}
DeeError_NEW_STATIC(_dee_notimplemented_{name.lower()},&DeeErrorType_NotImplemented,"{name.lower()}");
#endif

#ifdef DEE_NFS_HAVE_{name.upper()}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8{name}(DEE_A_IN_Z Dee_Utf8Char const *path) {{
 DEE_ASSERT(path);
#ifdef DeeSysFS_Utf8{name}
 int result;
 DeeSysFS_Utf8{name}(path,&result,return -1);
 return result;
#elif defined(DeeSysFS_Utf8{name}Object)
 DeeObject *path_ob;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) return -1;
 DeeSysFS_Utf8{name}Object(path_ob,&result,{{ Dee_DECREF(path_ob); return -1; }});
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_Wide{name}Object)\
   || defined(DeeSysFS_Wide{name})
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8String(path)) == NULL) return -1;
 result = DeeNFS_Wide{name}Object(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_{name})
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_Utf8TryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeSysFileFD_{name}(&fd,&result,{{ DeeNativeFileFD_Quit(&fd); return -1; }});
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 return DeeNFS_Utf8Try{name}(path);
#endif
}}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Wide{name}(DEE_A_IN_Z Dee_WideChar const *path) {{
 DEE_ASSERT(path);
#ifdef DeeSysFS_Wide{name}
 int result;
 DeeSysFS_Wide{name}(path,&result,return -1);
 return result;
#elif defined(DeeSysFS_Wide{name}Object)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_New(path)) == NULL) return -1;
 DeeSysFS_Wide{name}Object(path_ob,&result,{{ Dee_DECREF(path_ob); return -1; }});
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_Utf8{name}Object)\
   || defined(DeeSysFS_Utf8{name})
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 result = DeeNFS_Utf8{name}Object(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_{name})
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_WideTryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeSysFileFD_{name}(&fd,&result,{{ DeeNativeFileFD_Quit(&fd); return -1; }});
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 return DeeNFS_WideTry{name}(path);
#endif
}}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8{name}Object(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {{
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
#ifdef DeeSysFS_Utf8{name}Object
 int result;
 DeeSysFS_Utf8{name}Object(path,&result,return -1);
 return result;
#elif defined(DeeSysFS_Wide{name}Object)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) return -1;
 DeeSysFS_Wide{name}Object(path_ob,&result,{{ Dee_DECREF(path_ob); return -1; }});
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_{name})
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_Utf8TryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeSysFileFD_{name}(&fd,&result,{{ DeeNativeFileFD_Quit(&fd); return -1; }});
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 return DeeNFS_Utf8Try{name}Object(path);
#endif
}}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Wide{name}Object(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {{
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
#ifdef DeeSysFS_Wide{name}Object
 int result;
 DeeSysFS_Wide{name}Object(path,&result,return -1);
 return result;
#elif defined(DeeSysFS_Utf8{name}Object)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) return -1;
 DeeSysFS_Utf8{name}Object(path_ob,&result,{{ Dee_DECREF(path_ob); return -1; }});
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_{name})
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_WideTryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 DeeSysFileFD_{name}(&fd,&result,{{ DeeNativeFileFD_Quit(&fd); return -1; }});
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 return DeeNFS_WideTry{name}Object(path);
#endif
}}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8Try{name}(DEE_A_IN_Z Dee_Utf8Char const *path) {{
 DEE_ASSERT(path);
#ifdef DeeSysFS_Utf8Try{name}
 return DeeSysFS_Utf8Try{name}(path);
#elif defined(DeeSysFS_Utf8Try{name}Object)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_New(path)) == NULL) {{ DeeError_HandledOne(); return 0; }}
 result = DeeSysFS_Utf8Try{name}Object(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_WideTry{name}Object)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8String(path)) == NULL) {{ DeeError_HandledOne(); return 0; }}
 result = DeeSysFS_WideTry{name}Object(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_Try{name})
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_Utf8TryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 result = DeeSysFileFD_Try{name}(&fd);
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8{name}(path)) < 0) {{ DeeError_HandledOne(); result = 0; }}
 return result;
#endif
}}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTry{name}(DEE_A_IN_Z Dee_WideChar const *path) {{
 DEE_ASSERT(path);
#ifdef DeeSysFS_WideTry{name}
 return DeeSysFS_WideTry{name}(path);
#elif defined(DeeSysFS_WideTry{name}Object)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_New(path)) == NULL) {{ DeeError_HandledOne(); return 0; }}
 result = DeeSysFS_WideTry{name}Object(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFS_Utf8Try{name}Object)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideString(path)) == NULL) {{ DeeError_HandledOne(); return 0; }}
 result = DeeSysFS_Utf8Try{name}Object(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_Try{name})
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_WideTryInit(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 result = DeeSysFileFD_Try{name}(&fd);
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_Wide{name}(path)) < 0) {{ DeeError_HandledOne(); result = 0; }}
 return result;
#endif
}}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8Try{name}Object(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {{
 DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path));
#ifdef DeeSysFS_Utf8Try{name}Object
 return DeeSysFS_Utf8Try{name}Object(path);
#elif defined(DeeSysFS_WideTry{name}Object)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(path),DeeUtf8String_STR(path))) == NULL) {{ DeeError_HandledOne(); return 0; }}
 result = DeeSysFS_WideTry{name}Object(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_Try{name})
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_Utf8TryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 result = DeeSysFileFD_Try{name}(&fd);
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8{name}Object(path)) < 0) {{ DeeError_HandledOne(); result = 0; }}
 return result;
#endif
}}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTry{name}Object(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {{
 DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path));
#ifdef DeeSysFS_WideTry{name}Object
 return DeeSysFS_WideTry{name}Object(path);
#elif defined(DeeSysFS_Utf8Try{name}Object)
 DeeObject *path_ob; int result;
 if DEE_UNLIKELY((path_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(path),DeeWideString_STR(path))) == NULL) {{ DeeError_HandledOne(); return -1; }}
 result = DeeSysFS_Utf8Try{name}Object(path_ob);
 Dee_DECREF(path_ob);
 return result;
#elif defined(DeeSysFileFD_Try{name})
 struct DeeNativeFileFD fd; int result;
 if (!DeeNativeFileFD_WideTryInitObject(&fd,path,DEE_OPENMODE('r',0),0)) return 0;
 result = DeeSysFileFD_Try{name}(&fd);
 DeeNativeFileFD_Quit(&fd);
 return result;
#else
 int result;
 if ((result = DeeNFS_Wide{name}Object(path)) < 0) {{ DeeError_HandledOne(); result = 0; }}
 return result;
#endif
}}
#else
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8{name}(DEE_A_IN_Z Dee_Utf8Char const *path) {{ DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_{name.lower()}); return -1; }}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Wide{name}(DEE_A_IN_Z Dee_WideChar const *path) {{ DEE_ASSERT(path); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_{name.lower()}); return -1; }}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Utf8{name}Object(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {{ DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_{name.lower()}); return -1; }}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeNFS_Wide{name}Object(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {{ DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path)); (void)path; DeeError_Throw((DeeObject *)&_dee_notimplemented_{name.lower()}); return -1; }}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8Try{name}(DEE_A_IN_Z Dee_Utf8Char const *path) {{ DEE_ASSERT(path); (void)path; return 0; }}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTry{name}(DEE_A_IN_Z Dee_WideChar const *path) {{ DEE_ASSERT(path); (void)path; return 0; }}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8Try{name}Object(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {{ DEE_ASSERT(DeeObject_Check(path) && DeeUtf8String_Check(path)); (void)path; return 0; }}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTry{name}Object(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {{ DEE_ASSERT(DeeObject_Check(path) && DeeWideString_Check(path)); (void)path; return 0; }}
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_{name.upper()}_C_INL */