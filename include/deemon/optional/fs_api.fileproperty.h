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
#ifndef GUARD_DEEMON_OPTIONAL_FS_API_FILEPROPERTY_H
#define GUARD_DEEMON_OPTIONAL_FS_API_FILEPROPERTY_H 1

#include <deemon/__conf.inl>

DEE_DECL_BEGIN

typedef DEE_TYPES_UINT16_T Dee_fileproperty_t;

//////////////////////////////////////////////////////////////////////////
// File property enumerators
// >> Used to determinate various host-dependent file properties.
#define DEE_FILEPROPERTY_NONE          0
#define DEE_FILEPROPERTY_ISFILE        1
#define DEE_FILEPROPERTY_ISDIR         2
#define DEE_FILEPROPERTY_ISLINK        3
#define DEE_FILEPROPERTY_ISDRIVE       4
#define DEE_FILEPROPERTY_ISMOUNT       5
#define DEE_FILEPROPERTY_ISHIDDEN      6
#define DEE_FILEPROPERTY_ISEXECUTABLE  7
#define DEE_FILEPROPERTY_ISCHARDEV     8
#define DEE_FILEPROPERTY_ISBLOCKDEV    9
#define DEE_FILEPROPERTY_ISFIFO       10
#define DEE_FILEPROPERTY_ISSOCKET     11

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_FS_API_FILEPROPERTY_H */
