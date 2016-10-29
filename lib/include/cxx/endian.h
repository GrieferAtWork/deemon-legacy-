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
#pragma once
#ifndef __BYTEORDER__
#	error Missing predefined macro: '__BYTEORDER__'
#endif

#ifdef __STDC__
#	warning <endian.h> is not STDC
#endif

#include <features.h>

#undef __BYTE_ORDER
#undef _BYTE_ORDER
#define __BYTE_ORDER  __BYTEORDER__
#define _BYTE_ORDER   __BYTEORDER__

#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN    4321
#define __PDB_ENDIAN    3412

#ifndef __FLOAT_WORD_ORDER
#	define __FLOAT_WORD_ORDER __BYTEORDER__
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
#	define __LONG_LONG_PAIR(hi,lo) lo,hi
#elif __BYTE_ORDER == __BIG_ENDIAN
#	define __LONG_LONG_PAIR(hi,lo) hi,lo
#endif

#ifdef __USE_MISC
#	define LITTLE_ENDIAN __LITTLE_ENDIAN
#	define BIG_ENDIAN    __BIG_ENDIAN
#	define PDP_ENDIAN    __PDP_ENDIAN
#	define BYTE_ORDER    __BYTE_ORDER
#	if __has_include(<endian>)
#		include <endian>
#		define htobe16 __swap_be16
#		define htole16 __swap_le16
#		define be16toh __swap_be16
#		define le16toh __swap_le16
#		define htobe32 __swap_be32
#		define htole32 __swap_le32
#		define be32toh __swap_be32
#		define le32toh __swap_le32
#		define htobe64 __swap_be64
#		define htole64 __swap_le64
#		define be64toh __swap_be64
#		define le64toh __swap_le64
#	endif
#endif
