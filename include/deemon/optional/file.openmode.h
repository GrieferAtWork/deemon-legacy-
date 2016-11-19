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
#ifndef GUARD_DEEMON_OPTIONAL_FILE_OPENMODE_H
#define GUARD_DEEMON_OPTIONAL_FILE_OPENMODE_H 1

#include <deemon/__conf.inl>

DEE_DECL_BEGIN

typedef DEE_TYPES_UINT16_T Dee_openmode_t;
#define DEE_OPENMODE_READ            DEE_UINT16_C(0x0000)
#define DEE_OPENMODE_WRITE           DEE_UINT16_C(0x0001)
#define DEE_OPENMODE_APPEND          DEE_UINT16_C(0x0002)
#define DEE_OPENMODE_FLAG_UPDATE     DEE_UINT16_C(0x0004) // flag
#define DEE_OPENMODE_MASKMODE        DEE_UINT16_C(0x0003) // last 2 bits
#define DEE_OPENMODE_MASKBASIC       DEE_UINT16_C(0x0007) // last 3 bits
#define DEE_OPENMODE_ISREAD(mode)    (((mode)&DEE_OPENMODE_MASKMODE)==DEE_OPENMODE_READ)
#define DEE_OPENMODE_ISWRITE(mode)   (((mode)&DEE_OPENMODE_MASKMODE)==DEE_OPENMODE_WRITE)
#define DEE_OPENMODE_ISAPPEND(mode)  (((mode)&DEE_OPENMODE_MASKMODE)==DEE_OPENMODE_APPEND)
#define DEE_OPENMODE_HASUPDATE(mode) (((mode)&DEE_OPENMODE_FLAG_UPDATE)!=0)

//////////////////////////////////////////////////////////////////////////
// >> Dee_openmode_t DEE_OPENMODE(char rwa, bool update);
// Returns a basic open mode:
// @param rwa:    A character, that is either 'r', 'w' or 'a'
// @param update: A boolean, indicating if a file should be opened in update mode
#define DEE_OPENMODE(rwa,update)\
(((rwa) == 'r' ?  DEE_OPENMODE_READ :\
  (rwa) == 'w' ?  DEE_OPENMODE_WRITE :\
/*(rwa) == 'a' ?*/DEE_OPENMODE_APPEND\
)|((update) ? DEE_OPENMODE_FLAG_UPDATE : 0))



#if DEE_DEPRECATED_API_VERSION(100,102,104)
#define DEE_FILE_MODE_READ        DEE_MACRO_DEPRECATED(DEE_OPENMODE_READ)
#define DEE_FILE_MODE_WRITE       DEE_MACRO_DEPRECATED(DEE_OPENMODE_WRITE)
#define DEE_FILE_MODE_APPEND      DEE_MACRO_DEPRECATED(DEE_OPENMODE_APPEND)
#define DEE_FILE_FLAG_UPDATE      DEE_MACRO_DEPRECATED(DEE_OPENMODE_FLAG_UPDATE)
#define DEE_FILE_MASK_MODE        DEE_MACRO_DEPRECATED(DEE_OPENMODE_MASKMODE)
#define DEE_FILE_MASK_BASIC_MODE  DEE_MACRO_DEPRECATED(DEE_OPENMODE_MASKBASIC)
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_FILE_OPENMODE_H */
