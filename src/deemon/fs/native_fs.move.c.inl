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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_MOVE_C_INL
#define GUARD_DEEMON_FS_NATIVE_FS_MOVE_C_INL 1
#ifndef DEE_LIMITED_API
#define DEE_LIMITED_API 1
#endif

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/sys/sysfs.h>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN

#ifndef DEE_NFS_HAVE_MOVE
DeeError_NEW_STATIC(_dee_notimplemented_move,&DeeErrorType_NotImplemented,"move");
#endif

#ifdef DEE_NFS_HAVE_MOVE
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Move(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) {
 DEE_ASSERT(src); DEE_ASSERT(dst);
#ifdef DeeSysFS_Utf8Move
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_Utf8Move(src,dst,return -1);
 return 0;
#elif defined(DeeSysFS_Utf8MoveObject)
 DeeObject *src_ob,*dst_ob;
 if DEE_UNLIKELY((src_ob = DeeUtf8String_New(src)) == NULL) return -1;
 if DEE_UNLIKELY((dst_ob = DeeUtf8String_New(dst)) == NULL) {err_srcob: Dee_DECREF(src_ob); return -1; }
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(dst_ob); goto err_srcob; })
 DeeSysFS_Utf8MoveObject(src_ob,dst_ob,{ Dee_DECREF(dst_ob); goto err_srcob; });
 Dee_DECREF(dst_ob);
 Dee_DECREF(src_ob);
 return 0;
#else
 DeeObject *src_ob,*dst_ob; int result;
 if DEE_UNLIKELY((src_ob = DeeWideString_FromUtf8String(src)) == NULL) return -1;
 if DEE_UNLIKELY((dst_ob = DeeWideString_FromUtf8String(dst)) == NULL) { result = -1; goto end_srcob; }
 result = DeeNFS_WideMoveObject(src_ob,dst_ob);
 Dee_DECREF(dst_ob);
end_srcob: Dee_DECREF(src_ob);
 return result;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideMove(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) {
 DEE_ASSERT(src); DEE_ASSERT(dst);
#ifdef DeeSysFS_WideMove
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_WideMove(src,dst,return -1);
 return 0;
#elif defined(DeeSysFS_WideMoveObject)
 DeeObject *src_ob,*dst_ob;
 if DEE_UNLIKELY((src_ob = DeeWideString_New(src)) == NULL) return -1;
 if DEE_UNLIKELY((dst_ob = DeeWideString_New(dst)) == NULL) {err_srcob: Dee_DECREF(src_ob); return -1; }
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(dst_ob); goto err_srcob; })
 DeeSysFS_WideMoveObject(src_ob,dst_ob,{ Dee_DECREF(dst_ob); goto err_srcob; });
 Dee_DECREF(dst_ob);
 Dee_DECREF(src_ob);
 return 0;
#else
 DeeObject *src_ob,*dst_ob; int result;
 if DEE_UNLIKELY((src_ob = DeeUtf8String_FromWideString(src)) == NULL) return -1;
 if DEE_UNLIKELY((dst_ob = DeeUtf8String_FromWideString(dst)) == NULL) { result = -1; goto end_srcob; }
 result = DeeNFS_Utf8MoveObject(src_ob,dst_ob);
 Dee_DECREF(dst_ob);
end_srcob: Dee_DECREF(src_ob);
 return result;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8MoveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst) {
 DEE_ASSERT(DeeObject_Check(src) && DeeUtf8String_Check(src));
 DEE_ASSERT(DeeObject_Check(dst) && DeeUtf8String_Check(dst));
#ifdef DeeSysFS_Utf8MoveObject
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_Utf8MoveObject(src,dst,return -1);
 return 0;
#else
 DeeObject *src_ob,*dst_ob;
 if DEE_UNLIKELY((src_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(src),DeeUtf8String_STR(src))) == NULL) return -1;
 if DEE_UNLIKELY((dst_ob = DeeWideString_FromUtf8StringWithLength(
  DeeUtf8String_SIZE(dst),DeeUtf8String_STR(dst))) == NULL) {err_srcob: Dee_DECREF(src_ob); return -1; }
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(dst_ob); goto err_srcob; })
 DeeSysFS_WideMoveObject(src_ob,dst_ob,{ Dee_DECREF(dst_ob); goto err_srcob; });
 Dee_DECREF(dst_ob);
 Dee_DECREF(src_ob);
 return 0;
#endif
}
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideMoveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *src, DEE_A_IN_OBJECT(DeeWideStringObject) const *dst) {
 DEE_ASSERT(DeeObject_Check(src) && DeeWideString_Check(src));
 DEE_ASSERT(DeeObject_Check(dst) && DeeWideString_Check(dst));
#ifdef DeeSysFS_WideMoveObject
 DEE_NFS_CHECKINTERRUPT(return -1)
 DeeSysFS_WideMoveObject(src,dst,return -1);
 return 0;
#else
 DeeObject *src_ob,*dst_ob;
 if DEE_UNLIKELY((src_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(src),DeeWideString_STR(src))) == NULL) return -1;
 if DEE_UNLIKELY((dst_ob = DeeUtf8String_FromWideStringWithLength(
  DeeWideString_SIZE(dst),DeeWideString_STR(dst))) == NULL) {err_srcob: Dee_DECREF(src_ob); return -1; }
 DEE_NFS_CHECKINTERRUPT({ Dee_DECREF(dst_ob); goto err_srcob; })
 DeeSysFS_Utf8MoveObject(src_ob,dst_ob,{ Dee_DECREF(dst_ob); goto err_srcob; });
 Dee_DECREF(dst_ob);
 Dee_DECREF(src_ob);
 return 0;
#endif
}
#else
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8Move(DEE_A_IN_Z Dee_Utf8Char const *src, DEE_A_IN_Z Dee_Utf8Char const *dst) { DEE_ASSERT(src),DEE_ASSERT(dst); (void)src,dst; DeeError_Throw((DeeObject *)&_dee_notimplemented_move); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideMove(DEE_A_IN_Z Dee_WideChar const *src, DEE_A_IN_Z Dee_WideChar const *dst) { DEE_ASSERT(src),DEE_ASSERT(dst); (void)src,dst; DeeError_Throw((DeeObject *)&_dee_notimplemented_move); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_Utf8MoveObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *src, DEE_A_IN_OBJECT(DeeUtf8StringObject) const *dst) { DEE_ASSERT(DeeObject_Check(src) && DeeUtf8String_Check(src)); DEE_ASSERT(DeeObject_Check(dst) && DeeUtf8String_Check(dst)); (void)src,dst; DeeError_Throw((DeeObject *)&_dee_notimplemented_move); return -1; }
DEE_A_RET_EXCEPT(-1) int DeeNFS_WideMoveObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *src, DEE_A_IN_OBJECT(DeeWideStringObject) const *dst) { DEE_ASSERT(DeeObject_Check(src) && DeeWideString_Check(src)); DEE_ASSERT(DeeObject_Check(dst) && DeeWideString_Check(dst)); (void)src,dst; DeeError_Throw((DeeObject *)&_dee_notimplemented_move); return -1; }
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_MOVE_C_INL */
