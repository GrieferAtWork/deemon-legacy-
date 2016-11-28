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
#ifndef GUARD_DEEMON_FS_DYNAMIC_FS_H
#define GUARD_DEEMON_FS_DYNAMIC_FS_H 1

#include <deemon/__conf.inl>
#include <deemon/file.h>
#include <deemon/fs_api.h>

DEE_DECL_BEGIN

#ifdef DEE_FSAPIMODE_ENABLEVFS
#define DEE_FSAPIMODE_MASK (DEE_FSAPIMODE_EXPANDVARS|DEE_FSAPIMODE_ENABLEVFS)
#else
#define DEE_FSAPIMODE_MASK  DEE_FSAPIMODE_EXPANDVARS
#endif


//////////////////////////////////////////////////////////////////////////
// Dynamic FS API

DEE_COMPILER_STRUCTURE_PACKED_BEGIN
struct DeeFSApi_v102 {
#define REG_ENTRY(T,name) T  fs102_##name;
#define DIR_ENTRY(T,name) T _fs102_##name;
#define IND_ENTRY(T,name) T  fs102_##name;
#include "dynamic_fs.v102.inl"
};

enum{ // NOTE: These ids have to be multiplied with DEE_TYPES_SIZEOF_POINTER
#define REG_ENTRY(T,name) DeeVFSApiID_v102_ ##name,
#define DIR_ENTRY(T,name) DeeVFSApiID_v102__##name,
#define IND_ENTRY(T,name) DeeVFSApiID_v102_ ##name,
#include "dynamic_fs.v102.inl"
};


#ifdef __INTELLISENSE__
enum{s=sizeof(struct DeeFSApi_v102)/sizeof(void *)};
#endif
DEE_COMPILER_STRUCTURE_PACKED_END

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_DYNAMIC_FS_H */
