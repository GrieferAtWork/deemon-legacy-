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
#ifndef GUARD_DEEMON_OPTIONAL_STRUCTURED_POINTER_DECL_H
#define GUARD_DEEMON_OPTIONAL_STRUCTURED_POINTER_DECL_H 1

#include <deemon/__conf.inl>

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#include <deemon/optional/object_decl.h>
#include <deemon/optional/structured_decl.h>

DEE_DECL_BEGIN

struct DeePointerObject;

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif

DEE_COMPILER_MSVC_WARNING_PUSH(4201 4255)
struct DeePointerObject {
 DEE_STRUCTURED_OBJECT_HEAD // '__ob_type' is a 'DeePointerTypeObject'
 union{ // NOTE: 'p' may be NULL
  // Just a bunch of different pointer types (to make debugging easier)
  void *p; char *p_str; int (*p_fun)();
  Dee_int8_t  *p_i8;  Dee_uint8_t  *p_ui8;
  Dee_int16_t *p_i16; Dee_uint16_t *p_ui16;
  Dee_int32_t *p_i32; Dee_uint32_t *p_ui32;
  Dee_int64_t *p_i64; Dee_uint64_t *p_ui64;
 };
};
DEE_COMPILER_MSVC_WARNING_POP
#define DeePointer_ADDR(ob) (((DeePointerObject *)(ob))->p)

DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */

#endif /* !GUARD_DEEMON_OPTIONAL_STRUCTURED_POINTER_DECL_H */
