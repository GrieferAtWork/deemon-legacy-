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
#ifndef GUARD_DEEMON_OPTIONAL_FUNCTIONFLAGS_H
#define GUARD_DEEMON_OPTIONAL_FUNCTIONFLAGS_H 1

#include <deemon/__conf.inl>

DEE_DECL_BEGIN

typedef DEE_TYPES_UINT16_T Dee_funflags_t;
#define DEE_FUNCTIONFLAGS_NONE                      DEE_UINT16_C(0x0000)
#define DEE_FUNCTIONFLAGS_DEFAULT                   DEE_UINT16_C(0x0000)
#define DEE_FUNCTIONFLAGS_FLAG_THISCALL             DEE_UINT16_C(0x0003)
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
#define DEE_FUNCTIONFLAGS_FLAG_SYSV                 DEE_UINT16_C(0x0001)
#define DEE_FUNCTIONFLAGS_FLAG_STDCALL              DEE_UINT16_C(0x0002)
#define DEE_FUNCTIONFLAGS_FLAG_FASTCALL             DEE_UINT16_C(0x0004)
#define DEE_FUNCTIONFLAGS_FLAG_MS_CDECL             DEE_UINT16_C(0x0005)
#define DEE_FUNCTIONFLAGS_FLAG_PASCAL               DEE_UINT16_C(0x0006)
#define DEE_FUNCTIONFLAGS_FLAG_REGISTER             DEE_UINT16_C(0x0007)
#define DEE_FUNCTIONFLAGS_FLAG_WIN64                DEE_UINT16_C(0x0008)
#define DEE_FUNCTIONFLAGS_FLAG_UNIX64               DEE_UINT16_C(0x0009)
#define DEE_FUNCTIONFLAGS_FLAG_ADD_RESULT_REFERENCE DEE_UINT16_C(0x0010)
#define DEE_FUNCTIONFLAGS_FLAG_VARARGS              DEE_UINT16_C(0x0020)
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#define DEE_FUNCTIONFLAGS_FLAG_CC_MASK              DEE_UINT16_C(0x000F)
#define DEE_FUNCTIONFLAGS_FLAG_NOEXCEPT             DEE_UINT16_C(0x0040)
#define DEE_FUNCTIONFLAGS_FLAG_NORETURN             DEE_UINT16_C(0x0080)

#define DEE_FUNCTIONFLAGS_GETCC(flags)      ((flags)&DEE_FUNCTIONFLAGS_FLAG_CC_MASK)
#define DEE_FUNCTIONFLAGS_ISNOEXCEPT(flags) (((flags)&DEE_FUNCTIONFLAGS_FLAG_NOEXCEPT)!=0)
#define DEE_FUNCTIONFLAGS_ISNORETURN(flags) (((flags)&DEE_FUNCTIONFLAGS_FLAG_NORETURN)!=0)


#if DEE_DEPRECATED_API_VERSION(100,102,104)
#define DeeFunctionFlags_NONE                 DEE_DEPRECATED_MACRO(DEE_FUNCTIONFLAGS_NONE)
#define DeeFunctionFlags_DEFAULT              DEE_DEPRECATED_MACRO(DEE_FUNCTIONFLAGS_DEFAULT)
#define DeeFunctionFlags_THISCALL             DEE_DEPRECATED_MACRO(DEE_FUNCTIONFLAGS_FLAG_THISCALL)
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
#define DeeFunctionFlags_SYSV                 DEE_DEPRECATED_MACRO(DEE_FUNCTIONFLAGS_FLAG_SYSV)
#define DeeFunctionFlags_STDCALL              DEE_DEPRECATED_MACRO(DEE_FUNCTIONFLAGS_FLAG_STDCALL)
#define DeeFunctionFlags_FASTCALL             DEE_DEPRECATED_MACRO(DEE_FUNCTIONFLAGS_FLAG_FASTCALL)
#define DeeFunctionFlags_MS_CDECL             DEE_DEPRECATED_MACRO(DEE_FUNCTIONFLAGS_FLAG_MS_CDECL)
#define DeeFunctionFlags_PASCAL               DEE_DEPRECATED_MACRO(DEE_FUNCTIONFLAGS_FLAG_PASCAL)
#define DeeFunctionFlags_REGISTER             DEE_DEPRECATED_MACRO(DEE_FUNCTIONFLAGS_FLAG_REGISTER)
#define DeeFunctionFlags_WIN64                DEE_DEPRECATED_MACRO(DEE_FUNCTIONFLAGS_FLAG_WIN64)
#define DeeFunctionFlags_UNIX64               DEE_DEPRECATED_MACRO(DEE_FUNCTIONFLAGS_FLAG_UNIX64)
#define DeeFunctionFlags_CC_MASK              DEE_DEPRECATED_MACRO(DEE_FUNCTIONFLAGS_FLAG_CC_MASK)
#define DeeFunctionFlags_ADD_RESULT_REFERENCE DEE_DEPRECATED_MACRO(DEE_FUNCTIONFLAGS_FLAG_ADD_RESULT_REFERENCE)
#define DeeFunctionFlags_VARARGS              DEE_DEPRECATED_MACRO(DEE_FUNCTIONFLAGS_FLAG_VARARGS)
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#define DeeFunctionFlags_NOEXCEPT             DEE_DEPRECATED_MACRO(DEE_FUNCTIONFLAGS_FLAG_NOEXCEPT)
#define DeeFunctionFlags_NORETURN             DEE_DEPRECATED_MACRO(DEE_FUNCTIONFLAGS_FLAG_NORETURN)
#endif /* DEE_DEPRECATED_API_VERSION(100,102,104) */

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_FUNCTIONFLAGS_H */
