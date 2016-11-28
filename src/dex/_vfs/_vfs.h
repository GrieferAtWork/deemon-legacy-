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
#ifndef GUARD_DEEMON_DEX_VFS_H
#define GUARD_DEEMON_DEX_VFS_H 1
 
#include <deemon/__conf.inl>
#include <deemon/optional/fs_api.fsapimode.h>

DEE_DECL_BEGIN

// v Uses the same ids as the regular fs_api.
//   NOTE: Functions not implemented return NULL
extern DEE_ATTRIBUTE_DLLEXPORT DEE_A_RET_NEVER_NULL DeeFunPointer
 DeeVFS_GetFunction_v102(DEE_A_IN DEE_TYPES_UINT(DEE_TYPES_SIZEOF_SIZE_T) id);

DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_VFS_H */
