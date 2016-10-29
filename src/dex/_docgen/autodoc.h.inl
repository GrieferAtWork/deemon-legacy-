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
#ifndef GUARD_DEEMON_DEX_DOCGEN_AUTODOC_H_INL
#define GUARD_DEEMON_DEX_DOCGEN_AUTODOC_H_INL 1
 
#include <deemon/__conf.inl>
#include <deemon/generic_vtable.h>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
#undef DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
#endif

static DEE_A_RET_Z_OPT char const *_DeeMemberName_GetDoc(DEE_A_IN_Z char const *member_name);
static DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeMemberName_GetDocWithArgs(
 DEE_A_IN_Z char const *member_name, DEE_A_IN DeeObject *args,
 DEE_A_REF DEE_A_OUT DeeObject **result);
static DEE_A_RET_Z_OPT char const *_DeeMemberFunction_GetDoc(DEE_A_IN DeeMemberFunction func);
static DEE_A_RET_EXCEPT_REF DeeObject *_DeeMemberName_GetDocArgs(DEE_A_IN_Z char const *args);

//////////////////////////////////////////////////////////////////////////
// Returns the doc of a builtin object by id
static DEE_A_RET_Z_OPT char const *_DeeBuiltin_GetDoc(
 DEE_A_IN Dee_uint16_t builtin_id, DEE_A_OUT_OPT char const **name);

DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DOCGEN_AUTODOC_H_INL */
