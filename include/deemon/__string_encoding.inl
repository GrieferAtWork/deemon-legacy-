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

#ifndef DEE_STRING_ENCODING
#include <deemon/__conf.inl>
#define DEE_PRIVATE_STRING_ENCODING_Wide  0
#define DEE_PRIVATE_STRING_ENCODING_Utf8  1
#define DEE_PRIVATE_STRING_ENCODING_Utf16 2
#define DEE_PRIVATE_STRING_ENCODING_Utf32 3
#define DEE_PRIVATE_STRING_ENCODING(enc) DEE_PRIVATE_STRING_ENCODING_##enc
#define DEE_PRIVATE_STRING_ENCODING_SIZE_Wide  DEE_TYPES_SIZEOF_WCHAR_T
#define DEE_PRIVATE_STRING_ENCODING_SIZE_Utf8  1
#define DEE_PRIVATE_STRING_ENCODING_SIZE_Utf16 2
#define DEE_PRIVATE_STRING_ENCODING_SIZE_Utf32 4
#define DEE_PRIVATE_STRING_ENCODING_SIZE(enc) DEE_PRIVATE_STRING_ENCODING_SIZE_##enc
#define DEE_STRING_ENCODING(enc)      DEE_PRIVATE_STRING_ENCODING(enc)
#define DEE_STRING_ENCODING_SIZE(enc) DEE_PRIVATE_STRING_ENCODING_SIZE(enc)
#endif
