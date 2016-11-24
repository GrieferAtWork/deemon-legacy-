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

#ifdef __INTELLISENSE__
#include <deemon/__conf.inl>
#include <deemon/optional/fs_api.fileproperty.h>
#include <deemon/fs/native_fs.h>
#define WIDE
#define OBJECT
#endif

#ifdef WIDE
#define PREFIX(name) DeeNFS_Wide##name
#else
#define PREFIX(name) DeeNFS_Utf8##name
#endif
#ifdef OBJECT
#define FUNC(name) PREFIX(name##Object)
#else
#define FUNC       PREFIX
#endif

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT_FAIL(-1,0) int FUNC(HasProperty)(
#ifdef OBJECT
#ifdef WIDE
 DEE_A_IN_OBJECT(DeeWideStringObject) const *path,
#else
 DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path,
#endif
#else
#ifdef WIDE
 DEE_A_IN_Z Dee_WideChar const *path,
#else
 DEE_A_IN_Z Dee_Utf8Char const *path,
#endif
#endif
 DEE_A_IN Dee_fileproperty_t prop) {
#ifdef OBJECT
 DEE_ASSERT(DeeObject_Check(path));
#ifdef WIDE
 DEE_ASSERT(DeeWideString_Check(path));
#else
 DEE_ASSERT(DeeUtf8String_Check(path));
#endif
#else
 DEE_ASSERT(path);
#endif
 switch (prop) {
  case DEE_FILEPROPERTY_ISFILE      : return FUNC(IsFile)(path);
  case DEE_FILEPROPERTY_ISDIR       : return FUNC(IsDir)(path);
  case DEE_FILEPROPERTY_ISLINK      : return FUNC(IsLink)(path);
  case DEE_FILEPROPERTY_ISDRIVE     : return FUNC(IsDrive)(path);
  case DEE_FILEPROPERTY_ISMOUNT     : return FUNC(IsMount)(path);
  case DEE_FILEPROPERTY_ISHIDDEN    : return FUNC(IsHidden)(path);
  case DEE_FILEPROPERTY_ISEXECUTABLE: return FUNC(IsExecutable)(path);
  case DEE_FILEPROPERTY_ISCHARDEV   : return FUNC(IsCharDev)(path);
  case DEE_FILEPROPERTY_ISBLOCKDEV  : return FUNC(IsBlockDev)(path);
  case DEE_FILEPROPERTY_ISFIFO      : return FUNC(IsFiFo)(path);
  case DEE_FILEPROPERTY_ISSOCKET    : return FUNC(IsSocket)(path);
  case DEE_FILEPROPERTY_ISABS       : return FUNC(IsAbs)(path);
  case DEE_FILEPROPERTY_EXISTS      : return FUNC(Exists)(path);
  default: break;
 }
 return 0;
}

DEE_DECL_END

#undef FUNC
#undef PREFIX

#ifdef WIDE
#undef WIDE
#endif
#ifdef OBJECT
#undef OBJECT
#endif

