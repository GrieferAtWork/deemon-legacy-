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
#ifndef GUARD_DEEMON_OPTIONAL_OBJECT_IO_H
#define GUARD_DEEMON_OPTIONAL_OBJECT_IO_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/object_decl.h>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif

#define /* DEE_A_EXEC int */DeeObject_IORead(ob,p,s,rs) DeeObject_TIORead(Dee_TYPE(ob),ob,p,s,rs)
#define /* DEE_A_EXEC int */DeeObject_IOWrite(ob,p,s,ws) DeeObject_TIOWrite(Dee_TYPE(ob),ob,p,s,ws)
#define /* DEE_A_EXEC int */DeeObject_IOSeek(ob,off,whence,new_pos) DeeObject_TIOSeek(Dee_TYPE(ob),ob,off,whence,new_pos)
#define /* DEE_A_EXEC int */DeeObject_IOFlush(ob) DeeObject_TIOFlush(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC int */DeeObject_IOTrunc(ob) DeeObject_TIOTrunc(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC int */DeeObject_IOClose(ob) DeeObject_TIOClose(Dee_TYPE(ob),ob)

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TIORead(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s,
 DEE_A_OUT Dee_size_t *rs) DEE_ATTRIBUTE_NONNULL((1,2,5));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TIOWrite(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s,
 DEE_A_OUT Dee_size_t *ws) DEE_ATTRIBUTE_NONNULL((1,2,5));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TIOSeek(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN Dee_int64_t off, DEE_A_IN int whence,
 DEE_A_OUT_OPT Dee_uint64_t *new_pos) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TIOFlush(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TIOTrunc(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
// NOTE: Only reason why this can throw an error, is
//       'Error.RuntimeError.NotImplemented' if it's not a file
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TIOClose(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));


DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_OBJECT_IO_H */
