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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_LINK_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_LINK_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/sys/sysfs.h>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_LINK
DeeError_NEW_STATIC(_dee_notimplemented_link,&DeeErrorType_NotImplemented,"link");
#endif

#ifdef DEE_NFS_HAVE_LINK
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Link(DEE_A_IN_Z Dee_Utf8Char const *link_name, DEE_A_IN_Z Dee_Utf8Char const *target_name) {
 DEE_ASSERT(link_name); DEE_ASSERT(target_name);
#ifdef DeeSysFS_Utf8Link
 DeeSysFS_Utf8Link(link_name,target_name,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8LinkObject)
 DeeObject *link_name_ob,*target_name_ob;
 if DEE_UNLIKELY((link_name_ob = DeeUtf8String_New(link_name)) == NULL) return -1;
 if DEE_UNLIKELY((target_name_ob = DeeUtf8String_New(target_name)) == NULL) {err_link_nameob: Dee_DECREF(link_name_ob); return -1; }
 DeeSysFS_Utf8LinkObject(link_name_ob,target_name_ob,{ Dee_DECREF(target_name_ob); goto err_link_nameob; });
 Dee_DECREF(target_name_ob);
 Dee_DECREF(link_name_ob);
 return 0;
#elif defined(DeeSysFS_WideLinkObject)\
   || defined(DeeSysFS_WideLink)
 DeeObject *link_name_ob,*target_name_ob; int result;
 if DEE_UNLIKELY((link_name_ob = DeeWideString_FromUtf8String(link_name)) == NULL) return -1;
 if DEE_UNLIKELY((target_name_ob = DeeWideString_FromUtf8String(target_name)) == NULL) { result = -1; goto end_link_nameob; }
 result = DeeNFS_WideLinkObject(link_name_ob,target_name_ob);
 Dee_DECREF(target_name_ob);
end_link_nameob: Dee_DECREF(link_name_ob);
 return result;
#else
 DeeNFS_Utf8TryLink(link_name,target_name);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideLink(DEE_A_IN_Z Dee_WideChar const *link_name, DEE_A_IN_Z Dee_WideChar const *target_name) {
 DEE_ASSERT(link_name); DEE_ASSERT(target_name);
#ifdef DeeSysFS_WideLink
 DeeSysFS_WideLink(link_name,target_name,return -1);
 return 0;
#elif defined(DeeSysFS_WideLinkObject)
 DeeObject *link_name_ob,*target_name_ob;
 if DEE_UNLIKELY((link_name_ob = DeeWideString_New(link_name)) == NULL) return -1;
 if DEE_UNLIKELY((target_name_ob = DeeWideString_New(target_name)) == NULL) {err_link_nameob: Dee_DECREF(link_name_ob); return -1; }
 DeeSysFS_WideLinkObject(link_name_ob,target_name_ob,{ Dee_DECREF(target_name_ob); goto err_link_nameob; });
 Dee_DECREF(target_name_ob);
 Dee_DECREF(link_name_ob);
 return 0;
#elif defined(DeeSysFS_Utf8LinkObject)\
   || defined(DeeSysFS_Utf8Link)
 DeeObject *link_name_ob,*target_name_ob; int result;
 if DEE_UNLIKELY((link_name_ob = DeeUtf8String_FromWideString(link_name)) == NULL) return -1;
 if DEE_UNLIKELY((target_name_ob = DeeUtf8String_FromWideString(target_name)) == NULL) { result = -1; goto end_link_nameob; }
 result = DeeNFS_Utf8LinkObject(link_name_ob,target_name_ob);
 Dee_DECREF(target_name_ob);
end_link_nameob: Dee_DECREF(link_name_ob);
 return result;
#else
 DeeNFS_WideTryLink(link_name,target_name);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8LinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *link_name, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *target_name) {
 DEE_ASSERT(DeeObject_Check(link_name) && DeeUtf8String_Check(link_name));
 DEE_ASSERT(DeeObject_Check(target_name) && DeeUtf8String_Check(target_name));
#ifdef DeeSysFS_Utf8LinkObject
 DeeSysFS_Utf8LinkObject(link_name,target_name,return -1);
 return 0;
#elif defined(DeeSysFS_WideLinkObject)
 DeeObject *link_name_ob,*target_name_ob;
 if DEE_UNLIKELY((link_name_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(link_name),DeeUtf8String_STR(link_name))) == NULL) return -1;
 if DEE_UNLIKELY((target_name_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(target_name),DeeUtf8String_STR(target_name))) == NULL) {err_link_nameob: Dee_DECREF(link_name_ob); return -1; }
 DeeSysFS_WideLinkObject(link_name_ob,target_name_ob,{ Dee_DECREF(target_name_ob); goto err_link_nameob; });
 Dee_DECREF(target_name_ob);
 Dee_DECREF(link_name_ob);
 return 0;
#else
 DeeNFS_Utf8TryLinkObject(link_name,target_name);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideLinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *link_name, DEE_A_IN_OBJECT(DeeWideStringObject) const *target_name) {
 DEE_ASSERT(DeeObject_Check(link_name) && DeeWideString_Check(link_name));
 DEE_ASSERT(DeeObject_Check(target_name) && DeeWideString_Check(target_name));
#ifdef DeeSysFS_WideLinkObject
 DeeSysFS_WideLinkObject(link_name,target_name,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8LinkObject)
 DeeObject *link_name_ob,*target_name_ob;
 if DEE_UNLIKELY((link_name_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(link_name),DeeWideString_STR(link_name))) == NULL) return -1;
 if DEE_UNLIKELY((target_name_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(target_name),DeeWideString_STR(target_name))) == NULL) {err_link_nameob: Dee_DECREF(link_name_ob); return -1; }
 DeeSysFS_Utf8LinkObject(link_name_ob,target_name_ob,{ Dee_DECREF(target_name_ob); goto err_link_nameob; });
 Dee_DECREF(target_name_ob);
 Dee_DECREF(link_name_ob);
 return 0;
#else
 DeeNFS_WideTryLinkObject(link_name,target_name);
 return 0;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryLink(DEE_A_IN_Z Dee_Utf8Char const *link_name, DEE_A_IN_Z Dee_Utf8Char const *target_name) {
 DEE_ASSERT(link_name); DEE_ASSERT(target_name);
#ifdef DeeSysFS_Utf8TryLink
 return DeeSysFS_Utf8TryLink(link_name,target_name);
#elif defined(DeeSysFS_Utf8TryLinkObject)
 DeeObject *link_name_ob,*target_name_ob; int result;
 if DEE_UNLIKELY((link_name_ob = DeeUtf8String_New(link_name)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((target_name_ob = DeeUtf8String_New(target_name)) == NULL) { Dee_DECREF(link_name_ob); goto err_h1; }
 result = DeeSysFS_Utf8TryLinkObject(link_name_ob,target_name_ob);
 Dee_DECREF(target_name_ob);
 Dee_DECREF(link_name_ob);
 return result;
#elif defined(DeeSysFS_WideTryLinkObject)
 DeeObject *link_name_ob,*target_name_ob; int result;
 if DEE_UNLIKELY((link_name_ob = DeeWideString_FromUtf8String(link_name)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((target_name_ob = DeeWideString_FromUtf8String(target_name)) == NULL) { Dee_DECREF(link_name_ob); goto err_h1; }
 result = DeeSysFS_WideTryLinkObject(link_name_ob,target_name_ob);
 Dee_DECREF(target_name_ob);
 Dee_DECREF(link_name_ob);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8Link(link_name,target_name)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryLink(DEE_A_IN_Z Dee_WideChar const *link_name, DEE_A_IN_Z Dee_WideChar const *target_name) {
 DEE_ASSERT(link_name); DEE_ASSERT(target_name);
#ifdef DeeSysFS_WideTryLink
 return DeeSysFS_WideTryLink(link_name,target_name);
#elif defined(DeeSysFS_WideTryLinkObject)
 DeeObject *link_name_ob,*target_name_ob; int result;
 if DEE_UNLIKELY((link_name_ob = DeeWideString_New(link_name)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((target_name_ob = DeeWideString_New(target_name)) == NULL) { Dee_DECREF(link_name_ob); goto err_h1; }
 result = DeeSysFS_WideTryLinkObject(link_name_ob,target_name_ob);
 Dee_DECREF(target_name_ob);
 Dee_DECREF(link_name_ob);
 return result;
#elif defined(DeeSysFS_Utf8TryLinkObject)
 DeeObject *link_name_ob,*target_name_ob; int result;
 if DEE_UNLIKELY((link_name_ob = DeeUtf8String_FromWideString(link_name)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((target_name_ob = DeeUtf8String_FromWideString(target_name)) == NULL) { Dee_DECREF(link_name_ob); goto err_h1; }
 result = DeeSysFS_Utf8TryLinkObject(link_name_ob,target_name_ob);
 Dee_DECREF(target_name_ob);
 Dee_DECREF(link_name_ob);
 return result;
#else
 int result;
 if ((result = DeeNFS_WideLink(link_name,target_name)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryLinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *link_name, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *target_name) {
 DEE_ASSERT(DeeObject_Check(link_name) && DeeUtf8String_Check(link_name));
 DEE_ASSERT(DeeObject_Check(target_name) && DeeUtf8String_Check(target_name));
#ifdef DeeSysFS_Utf8TryLinkObject
 return DeeSysFS_Utf8TryLinkObject(link_name,target_name);
#elif defined(DeeSysFS_WideTryLinkObject)
 DeeObject *link_name_ob,*target_name_ob; int result;
 if DEE_UNLIKELY((link_name_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(link_name),DeeUtf8String_STR(link_name))) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((target_name_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(target_name),DeeUtf8String_STR(target_name))) == NULL) { Dee_DECREF(link_name_ob); goto err_h1; }
 result = DeeSysFS_WideTryLinkObject(link_name_ob,target_name_ob);
 Dee_DECREF(target_name_ob);
 Dee_DECREF(link_name_ob);
 return result;
#else
 int result;
 if ((result = DeeNFS_Utf8LinkObject(link_name,target_name)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryLinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *link_name, DEE_A_IN_OBJECT(DeeWideStringObject) const *target_name) {
 DEE_ASSERT(DeeObject_Check(link_name) && DeeWideString_Check(link_name));
 DEE_ASSERT(DeeObject_Check(target_name) && DeeWideString_Check(target_name));
#ifdef DeeSysFS_WideTryLinkObject
 return DeeSysFS_WideTryLinkObject(link_name,target_name);
#elif defined(DeeSysFS_Utf8TryLinkObject)
 DeeObject *link_name_ob,*target_name_ob; int result;
 if DEE_UNLIKELY((link_name_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(link_name),DeeWideString_STR(link_name))) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((target_name_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(target_name),DeeWideString_STR(target_name))) == NULL) { Dee_DECREF(link_name_ob); goto err_h1; }
 result = DeeSysFS_Utf8TryLinkObject(link_name_ob,target_name_ob);
 Dee_DECREF(target_name_ob);
 Dee_DECREF(link_name_ob);
 return result;
#else
 int result;
 if ((result = DeeNFS_WideLinkObject(link_name,target_name)) < 0) { DeeError_HandledOne(); result = 1; }
 return !result;
#endif
}
#else
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Link(DEE_A_IN_Z Dee_Utf8Char const *link_name, DEE_A_IN_Z Dee_Utf8Char const *target_name) { DEE_ASSERT(link_name),DEE_ASSERT(target_name); (void)link_name,target_name; DeeError_Throw((DeeObject *)&_dee_notimplemented_link); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideLink(DEE_A_IN_Z Dee_WideChar const *link_name, DEE_A_IN_Z Dee_WideChar const *target_name) { DEE_ASSERT(link_name),DEE_ASSERT(target_name); (void)link_name,target_name; DeeError_Throw((DeeObject *)&_dee_notimplemented_link); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8LinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *link_name, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *target_name) { DEE_ASSERT(DeeObject_Check(link_name) && DeeUtf8String_Check(link_name)); DEE_ASSERT(DeeObject_Check(target_name) && DeeUtf8String_Check(target_name)); (void)link_name,target_name; DeeError_Throw((DeeObject *)&_dee_notimplemented_link); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideLinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *link_name, DEE_A_IN_OBJECT(DeeWideStringObject) const *target_name) { DEE_ASSERT(DeeObject_Check(link_name) && DeeWideString_Check(link_name)); DEE_ASSERT(DeeObject_Check(target_name) && DeeWideString_Check(target_name)); (void)link_name,target_name; DeeError_Throw((DeeObject *)&_dee_notimplemented_link); return -1; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryLink(DEE_A_IN_Z Dee_Utf8Char const *link_name, DEE_A_IN_Z Dee_Utf8Char const *target_name) { DEE_ASSERT(link_name),DEE_ASSERT(target_name); (void)link_name,target_name; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryLink(DEE_A_IN_Z Dee_WideChar const *link_name, DEE_A_IN_Z Dee_WideChar const *target_name) { DEE_ASSERT(link_name),DEE_ASSERT(target_name); (void)link_name,target_name; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_Utf8TryLinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *link_name, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *target_name) { DEE_ASSERT(DeeObject_Check(link_name) && DeeUtf8String_Check(link_name)); DEE_ASSERT(DeeObject_Check(target_name) && DeeUtf8String_Check(target_name)); (void)link_name,target_name; return 0; }
DEE_A_RET_NOEXCEPT(0) int DeeNFS_WideTryLinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *link_name, DEE_A_IN_OBJECT(DeeWideStringObject) const *target_name) { DEE_ASSERT(DeeObject_Check(link_name) && DeeWideString_Check(link_name)); DEE_ASSERT(DeeObject_Check(target_name) && DeeWideString_Check(target_name)); (void)link_name,target_name; return 0; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_LINK_C_INL */
