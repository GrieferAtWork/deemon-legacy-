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
#ifndef GUARD_DEEMON_FS_EXPAND_FS_MOVE_C_INL
#define GUARD_DEEMON_FS_EXPAND_FS_MOVE_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/fs_api.h>
#include <deemon/fs/expand_fs.h>

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8Move(
DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) {
 DeeObject *xsrc,*xdst; int result;
 if DEE_UNLIKELY((xsrc = DeeFS_Utf8PathExpand(src)) == NULL) return -1;
 if DEE_UNLIKELY((xdst = DeeFS_Utf8PathExpand(dst)) == NULL) { result = -1; goto end_xsrc; }
 result = _DeeFS_Utf8MoveObject(xsrc,xdst);
 Dee_DECREF(xdst);
end_xsrc: Dee_DECREF(xsrc);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeXFS_WideMove(
 DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) {
 DeeObject *xsrc,*xdst; int result;
 if DEE_UNLIKELY((xsrc = DeeFS_WidePathExpand(src)) == NULL) return -1;
 if DEE_UNLIKELY((xdst = DeeFS_WidePathExpand(dst)) == NULL) { result = -1; goto end_xsrc; }
 result = _DeeFS_WideMoveObject(xsrc,xdst);
 Dee_DECREF(xdst);
end_xsrc: Dee_DECREF(xsrc);
 return result;
}
DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryMove(
 DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) {
 DeeObject *xsrc,*xdst; int result;
 if DEE_UNLIKELY((xsrc = DeeFS_Utf8PathExpand(src)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((xdst = DeeFS_Utf8PathExpand(dst)) == NULL) { Dee_DECREF(xsrc); goto err_h1; }
 result = _DeeFS_Utf8TryMoveObject(xsrc,xdst);
 Dee_DECREF(xdst);
 Dee_DECREF(xsrc);
 return result;
}
DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryMove(
 DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) {
 DeeObject *xsrc,*xdst; int result;
 if DEE_UNLIKELY((xsrc = DeeFS_WidePathExpand(src)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((xdst = DeeFS_WidePathExpand(dst)) == NULL) { Dee_DECREF(xsrc); goto err_h1; }
 result = _DeeFS_WideTryMoveObject(xsrc,xdst);
 Dee_DECREF(xdst);
 Dee_DECREF(xsrc);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeXFS_Utf8MoveObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src,
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst) {
 DeeObject *xsrc,*xdst; int result;
 if DEE_UNLIKELY((xsrc = DeeFS_Utf8PathExpandObject(src)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((xdst = DeeFS_Utf8PathExpandObject(dst)) == NULL) { Dee_DECREF(xsrc); goto err_h1; }
 result = _DeeFS_Utf8TryMoveObject(xsrc,xdst);
 Dee_DECREF(xdst);
 Dee_DECREF(xsrc);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeXFS_WideMoveObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *src,
 DEE_A_IN_OBJECT(DeeWideStringObject) const *dst) {
 DeeObject *xsrc,*xdst; int result;
 if DEE_UNLIKELY((xsrc = DeeFS_WidePathExpandObject(src)) == NULL) return -1;
 if DEE_UNLIKELY((xdst = DeeFS_WidePathExpandObject(dst)) == NULL) { result = -1; goto end_xsrc; }
 result = _DeeFS_WideMoveObject(xsrc,xdst);
 Dee_DECREF(xdst);
end_xsrc: Dee_DECREF(xsrc);
 return result;
}
DEE_A_RET_NOEXCEPT(0) int DeeXFS_Utf8TryMoveObject(
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src,
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst) {
 DeeObject *xsrc,*xdst; int result;
 if DEE_UNLIKELY((xsrc = DeeFS_Utf8PathExpandObject(src)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((xdst = DeeFS_Utf8PathExpandObject(dst)) == NULL) { Dee_DECREF(xsrc); goto err_h1; }
 result = _DeeFS_Utf8TryMoveObject(xsrc,xdst);
 Dee_DECREF(xdst);
 Dee_DECREF(xsrc);
 return result;
}
DEE_A_RET_NOEXCEPT(0) int DeeXFS_WideTryMoveObject(
 DEE_A_IN_OBJECT(DeeWideStringObject) const *src,
 DEE_A_IN_OBJECT(DeeWideStringObject) const *dst) {
 DeeObject *xsrc,*xdst; int result;
 if DEE_UNLIKELY((xsrc = DeeFS_WidePathExpandObject(src)) == NULL) {err_h1: DeeError_HandledOne(); return 0; }
 if DEE_UNLIKELY((xdst = DeeFS_WidePathExpandObject(dst)) == NULL) { Dee_DECREF(xsrc); goto err_h1; }
 result = _DeeFS_WideTryMoveObject(xsrc,xdst);
 Dee_DECREF(xdst);
 Dee_DECREF(xsrc);
 return result;
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_EXPAND_FS_MOVE_C_INL */
