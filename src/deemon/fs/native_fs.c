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
#ifndef GUARD_DEEMON_FS_NATIVE_FS_C
#define GUARD_DEEMON_FS_NATIVE_FS_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include <deemon/fs/native_fs.h>

DEE_DECL_BEGIN


DEE_DECL_END

#ifndef __INTELLISENSE__
#include "native_fs.chdir.c.inl"
#include "native_fs.chmod.c.inl"
#include "native_fs.chown.c.inl"
#include "native_fs.copy.c.inl"
#include "native_fs.delenv.c.inl"
#include "native_fs.enumenv.c.inl"
#include "native_fs.exists.c.inl"
#include "native_fs.getcwd.c.inl"
#include "native_fs.getenv.c.inl"
#include "native_fs.gethome.c.inl"
#include "native_fs.getmod.c.inl"
#include "native_fs.getown.c.inl"
#include "native_fs.gettimes.c.inl"
#include "native_fs.gettmp.c.inl"
#include "native_fs.getuserhome.c.inl"
#include "native_fs.hasenv.c.inl"
#include "native_fs.hasproperty.c.inl"
#include "native_fs.isabs.c.inl"
#include "native_fs.isblockdev.c.inl"
#include "native_fs.ischardev.c.inl"
#include "native_fs.isdir.c.inl"
#include "native_fs.isdrive.c.inl"
#include "native_fs.isexecutable.c.inl"
#include "native_fs.isfifo.c.inl"
#include "native_fs.isfile.c.inl"
#include "native_fs.ishidden.c.inl"
#include "native_fs.islink.c.inl"
#include "native_fs.ismount.c.inl"
#include "native_fs.issocket.c.inl"
#include "native_fs.link.c.inl"
#include "native_fs.mkdir.c.inl"
#include "native_fs.move.c.inl"
#include "native_fs.remove.c.inl"
#include "native_fs.rmdir.c.inl"
#include "native_fs.setenv.c.inl"
#include "native_fs.settimes.c.inl"
#include "native_fs.unlink.c.inl"
#endif

#endif /* !GUARD_DEEMON_FS_NATIVE_FS_H */
