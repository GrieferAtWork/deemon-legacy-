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
#ifndef GUARD_DEEMON_FS_VIRTUAL_FS_C
#define GUARD_DEEMON_FS_VIRTUAL_FS_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

#if DEE_CONFIG_RUNTIME_HAVE_VFS2
DEE_DECL_BEGIN

DEE_DECL_END

#ifndef __INTELLISENSE__
#include "virtual_fs.chdir.c.inl"
#include "virtual_fs.gettimes.c.inl"
#include "virtual_fs.hasproperty.c.inl"
#include "virtual_fs.mkdir.c.inl"
#include "virtual_fs.remove.c.inl"
#include "virtual_fs.rmdir.c.inl"
#include "virtual_fs.settimes.c.inl"
#include "virtual_fs.unlink.c.inl"
#endif

#endif /* DEE_CONFIG_RUNTIME_HAVE_VFS2 */

#endif /* !GUARD_DEEMON_FS_VIRTUAL_FS_C */
