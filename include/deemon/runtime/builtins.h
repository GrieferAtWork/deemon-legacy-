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
#ifndef GUARD_DEEMON_BUILTINS_H
#define GUARD_DEEMON_BUILTINS_H 1

#include <deemon/__conf.inl>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif

//////////////////////////////////////////////////////////////////////////
// Builtin objects: All the global DeeObject vars
DEE_DATA_DECL(DeeObject *const) _DeeBuiltin_ID2Type[];

//////////////////////////////////////////////////////////////////////////
// Returns the id of a given builtin object
// >> Returns (Dee_uint16_t)-1 for unknown builtins
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint16_t) DeeBuiltin_Id(
 DEE_A_IN DeeObject const *ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint16_t) _DeeBuiltin_Count(void);
#define DeeBuiltin_Count (_DeeBuiltin_Count())

#ifdef DEE_LIMITED_DEX
// Returns true if a builtin function 'ob' can be called at compile-time
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) _DeeBuiltin_AllowConstantCall(
 DEE_A_IN DeeObject const *ob) DEE_ATTRIBUTE_NONNULL((1));
#endif

//////////////////////////////////////////////////////////////////////////
// Returns the object associated with 'id'
// >> Returns NULL if no object is associated with the id
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL DeeObject *)
 DeeBuiltin_Get(DEE_A_IN Dee_uint16_t id);
#define DeeBuiltin_GET(id) _DeeBuiltin_ID2Type[id]

DEE_DECL_END

#endif /* !GUARD_DEEMON_BUILTINS_H */
