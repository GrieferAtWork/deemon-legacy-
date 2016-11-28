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
#ifndef GUARD_DEEMON_FS_VFS_VIEW_H
#define GUARD_DEEMON_FS_VFS_VIEW_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/object_decl.h>
#include <deemon/type.h>
#include "../virtual_view.h"

DEE_DECL_BEGIN

DEE_OBJECT_DEF(DeeVFSViewObject);
struct DeeVFSViewObject {
 DEE_OBJECT_HEAD
 struct DeeVFSView vv_view; /*< Underlying & variadic view object. */
};

extern DEE_A_RET_EXCEPT_REF DeeVFSViewObject *
 DeeVFSView_OpenNode(DEE_A_INOUT struct DeeVFSNode *node);
extern DEE_A_RET_NOEXCEPT_REF DeeVFSViewObject *
 DeeVFSView_TryOpenNode(DEE_A_INOUT struct DeeVFSNode *node);

extern DeeTypeObject DeeVFSView_Type;

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_VFS_VIEW_H */
