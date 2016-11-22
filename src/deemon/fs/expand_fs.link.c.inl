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
#ifndef GUARD_DEEMON_FS_EXPAND_FS_LINK_C_INL
#define GUARD_DEEMON_FS_EXPAND_FS_LINK_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/fs_api.h>
#include <deemon/fs/expand_fs.h>

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8Link(
DEE_A_IN_Z Dee_Utf8Char const *link_name, DEE_A_IN_Z Dee_Utf8Char const *target_name) {
 DeeObject *xlink_name,*xtarget_name; int result;
 if DEE_UNLIKELY((xlink_name = DeeFS_Utf8PathExpand(link_name)) == NULL) return -1;
 if DEE_UNLIKELY((xtarget_name = DeeFS_Utf8PathExpand(target_name)) == NULL) { result = -1; goto end_xlink_name; }
 result = _DeeFS_Utf8LinkObject(xlink_name,xtarget_name);
 Dee_DECREF(xtarget_name);
end_xlink_name: Dee_DECREF(xlink_name);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeXFS_WideLink(
 DEE_A_IN_Z Dee_WideChar const *link_name, DEE_A_IN_Z Dee_WideChar const *target_name) {
 DeeObject *xlink_name,*xtarget_name; int result;
 if DEE_UNLIKELY((xlink_name = DeeFS_WidePathExpand(link_name)) == NULL) return -1;
 if DEE_UNLIKELY((xtarget_name = DeeFS_WidePathExpand(target_name)) == NULL) { result = -1; goto end_xlink_name; }
 result = _DeeFS_WideLinkObject(xlink_name,xtarget_name);
 Dee_DECREF(xtarget_name);
end_xlink_name: Dee_DECREF(xlink_name);
 return result;
}
DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryLink(
 DEE_A_IN_Z Dee_Utf8Char const *link_name, DEE_A_IN_Z Dee_Utf8Char const *target_name) {
 DeeObject *xlink_name,*xtarget_name; int result;
 if DEE_UNLIKELY((xlink_name = DeeFS_Utf8PathExpand(link_name)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((xtarget_name = DeeFS_Utf8PathExpand(target_name)) == NULL) { Dee_DECREF(xlink_name); goto err_h1; }
 result = _DeeFS_Utf8TryLinkObject(xlink_name,xtarget_name);
 Dee_DECREF(xtarget_name);
 Dee_DECREF(xlink_name);
 return result;
}
DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryLink(
 DEE_A_IN_Z Dee_WideChar const *link_name, DEE_A_IN_Z Dee_WideChar const *target_name) {
 DeeObject *xlink_name,*xtarget_name; int result;
 if DEE_UNLIKELY((xlink_name = DeeFS_WidePathExpand(link_name)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((xtarget_name = DeeFS_WidePathExpand(target_name)) == NULL) { Dee_DECREF(xlink_name); goto err_h1; }
 result = _DeeFS_WideTryLinkObject(xlink_name,xtarget_name);
 Dee_DECREF(xtarget_name);
 Dee_DECREF(xlink_name);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8LinkObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *link_name,
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *target_name) {
 DeeObject *xlink_name,*xtarget_name; int result;
 if DEE_UNLIKELY((xlink_name = DeeFS_Utf8PathExpandObject(link_name)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((xtarget_name = DeeFS_Utf8PathExpandObject(target_name)) == NULL) { Dee_DECREF(xlink_name); goto err_h1; }
 result = _DeeFS_Utf8TryLinkObject(xlink_name,xtarget_name);
 Dee_DECREF(xtarget_name);
 Dee_DECREF(xlink_name);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeXFS_WideLinkObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *link_name,
 DEE_A_IN_OBJECT(DeeWideStringObject) const *target_name) {
 DeeObject *xlink_name,*xtarget_name; int result;
 if DEE_UNLIKELY((xlink_name = DeeFS_WidePathExpandObject(link_name)) == NULL) return -1;
 if DEE_UNLIKELY((xtarget_name = DeeFS_WidePathExpandObject(target_name)) == NULL) { result = -1; goto end_xlink_name; }
 result = _DeeFS_WideLinkObject(xlink_name,xtarget_name);
 Dee_DECREF(xtarget_name);
end_xlink_name: Dee_DECREF(xlink_name);
 return result;
}
DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryLinkObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *link_name,
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *target_name) {
 DeeObject *xlink_name,*xtarget_name; int result;
 if DEE_UNLIKELY((xlink_name = DeeFS_Utf8PathExpandObject(link_name)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((xtarget_name = DeeFS_Utf8PathExpandObject(target_name)) == NULL) { Dee_DECREF(xlink_name); goto err_h1; }
 result = _DeeFS_Utf8TryLinkObject(xlink_name,xtarget_name);
 Dee_DECREF(xtarget_name);
 Dee_DECREF(xlink_name);
 return result;
}
DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryLinkObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *link_name,
 DEE_A_IN_OBJECT(DeeWideStringObject) const *target_name) {
 DeeObject *xlink_name,*xtarget_name; int result;
 if DEE_UNLIKELY((xlink_name = DeeFS_WidePathExpandObject(link_name)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((xtarget_name = DeeFS_WidePathExpandObject(target_name)) == NULL) { Dee_DECREF(xlink_name); goto err_h1; }
 result = _DeeFS_WideTryLinkObject(xlink_name,xtarget_name);
 Dee_DECREF(xtarget_name);
 Dee_DECREF(xlink_name);
 return result;
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_EXPAND_FS_LINK_C_INL */
