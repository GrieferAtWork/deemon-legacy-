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

typedef DEE_TYPES_UINT16_T Dee_openmode_t;                // =file_state= | =permissions= | =notes=
#define DEE_OPENMODE_READ            DEE_UINT16_C(0x0000) // open         | read          | -
#define DEE_OPENMODE_WRITE           DEE_UINT16_C(0x0001) // create/clear | write         | -
#define DEE_OPENMODE_APPEND          DEE_UINT16_C(0x0002) // open/create  | write         | Write will seek to end
#define DEE_OPENMODE_CREATE          DEE_UINT16_C(0x0003) // create       | write         | Fails if file already exists
#define DEE_OPENMODE_FLAG_UPDATE     DEE_UINT16_C(0x0100) // flag (Extends permissions to read+write)
#define DEE_OPENMODE_MASKMODE        DEE_UINT16_C(0x00FF)
#define DEE_OPENMODE_ISREAD(mode)    (((mode)&DEE_OPENMODE_MASKMODE)==DEE_OPENMODE_READ)
#define DEE_OPENMODE_ISWRITE(mode)   (((mode)&DEE_OPENMODE_MASKMODE)==DEE_OPENMODE_WRITE)
#define DEE_OPENMODE_ISAPPEND(mode)  (((mode)&DEE_OPENMODE_MASKMODE)==DEE_OPENMODE_APPEND)
#define DEE_OPENMODE_ISCREATE(mode)  (((mode)&DEE_OPENMODE_MASKMODE)==DEE_OPENMODE_CREATE)
#define DEE_OPENMODE_HASUPDATE(mode) (((mode)&DEE_OPENMODE_FLAG_UPDATE)!=0)
#define DEE_OPENMODE_HASREAD(mode)   (DEE_OPENMODE_ISREAD(mode)||DEE_OPENMODE_HASUPDATE(mode))
#define DEE_OPENMODE_HASWRITE(mode)  (DEE_OPENMODE_ISWRITE(mode)||DEE_OPENMODE_HASUPDATE(mode))

//////////////////////////////////////////////////////////////////////////
// >> Dee_openmode_t DEE_OPENMODE(char rwa, bool update);
// Returns a basic open mode:
// @param rwa:    A character, that is either 'r', 'w', 'x' or 'a'
// @param update: A boolean, indicating if a file should be opened in update mode
#define DEE_OPENMODE(rwa,update) \
(((rwa) == 'r' ?  DEE_OPENMODE_READ :\
  (rwa) == 'w' ?  DEE_OPENMODE_WRITE :\
  (rwa) == 'x' ?  DEE_OPENMODE_CREATE :\
/*(rwa) == 'a' ?*/DEE_OPENMODE_APPEND\
)|((update) ? DEE_OPENMODE_FLAG_UPDATE : 0))

//////////////////////////////////////////////////////////////////////////
// Parse the open mode for a file
// >> Invalid modes are silently ignored, and if unrecognized, the default mode is used
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_openmode_t)
 DeeFile_TryParseMode(DEE_A_IN_Z char const *mode) DEE_ATTRIBUTE_NONNULL((1));


#if DEE_DEPRECATED_API_VERSION(100,102,104)
#define DEE_FILE_MODE_READ        DEE_DEPRECATED_MACRO(DEE_OPENMODE_READ)
#define DEE_FILE_MODE_WRITE       DEE_DEPRECATED_MACRO(DEE_OPENMODE_WRITE)
#define DEE_FILE_MODE_APPEND      DEE_DEPRECATED_MACRO(DEE_OPENMODE_APPEND)
#define DEE_FILE_FLAG_UPDATE      DEE_DEPRECATED_MACRO(DEE_OPENMODE_FLAG_UPDATE)
#define DEE_FILE_MASK_MODE        DEE_DEPRECATED_MACRO(DEE_OPENMODE_MASKMODE)
#define DEE_FILE_MASK_BASIC_MODE  DEE_UINT16_C(0xFFFF)
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_FILE_OPENMODE_H */
