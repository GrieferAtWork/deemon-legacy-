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
#ifndef GUARD_DEEMON_FS_EXPAND_FS_UNLINK_C_INL
#define GUARD_DEEMON_FS_EXPAND_FS_UNLINK_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/fs_api.h>
#include <deemon/fs/expand_fs.h>

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8Unlink(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DeeObject *xpath; int result;
 if DEE_UNLIKELY((xpath = DeeFS_Utf8PathExpand(path)) == NULL) return -1;
 result = _DeeFS_Utf8UnlinkObject(xpath);
 Dee_DECREF(xpath);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeXFS_WideUnlink(DEE_A_IN_Z Dee_WideChar const *path) {
 DeeObject *xpath; int result;
 if DEE_UNLIKELY((xpath = DeeFS_WidePathExpand(path)) == NULL) return -1;
 result = _DeeFS_WideUnlinkObject(xpath);
 Dee_DECREF(xpath);
 return result;
}
DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryUnlink(DEE_A_IN_Z Dee_Utf8Char const *path) {
 DeeObject *xpath; int result;
 if DEE_UNLIKELY((xpath = DeeFS_Utf8PathExpand(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = _DeeFS_Utf8TryUnlinkObject(xpath);
 Dee_DECREF(xpath);
 return result;
}
DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryUnlink(DEE_A_IN_Z Dee_WideChar const *path) {
 DeeObject *xpath; int result;
 if DEE_UNLIKELY((xpath = DeeFS_WidePathExpand(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = _DeeFS_WideTryUnlinkObject(xpath);
 Dee_DECREF(xpath);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8UnlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
 DeeObject *xpath; int result;
 if DEE_UNLIKELY((xpath = DeeFS_Utf8PathExpandObject(path)) == NULL) return -1;
 result = _DeeFS_Utf8UnlinkObject(xpath);
 Dee_DECREF(xpath);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeXFS_WideUnlinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 DeeObject *xpath; int result;
 if DEE_UNLIKELY((xpath = DeeFS_WidePathExpandObject(path)) == NULL) return -1;
 result = _DeeFS_WideUnlinkObject(xpath);
 Dee_DECREF(xpath);
 return result;
}
DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryUnlinkObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path) {
 DeeObject *xpath; int result;
 if DEE_UNLIKELY((xpath = DeeFS_Utf8PathExpandObject(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = _DeeFS_Utf8TryUnlinkObject(xpath);
 Dee_DECREF(xpath);
 return result;
}
DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryUnlinkObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path) {
 DeeObject *xpath; int result;
 if DEE_UNLIKELY((xpath = DeeFS_WidePathExpandObject(path)) == NULL) { DeeError_HandledOne(); return 0; }
 result = _DeeFS_WideTryUnlinkObject(xpath);
 Dee_DECREF(xpath);
 return result;
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_EXPAND_FS_UNLINK_C_INL */
