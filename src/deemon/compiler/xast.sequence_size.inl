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
#include "xast.c"
#define SEQ_TYPE DEE_XASTKIND_TUPLE
#endif

#ifndef SEQ_TYPE
#error "Must '#define SEQ_TYPE' before #including this file"
#endif

DEE_DECL_BEGIN

#if SEQ_TYPE == DEE_XASTKIND_TUPLE
DEE_A_RET_WUNUSED Dee_size_t DeeXAst_TupleSize
#elif SEQ_TYPE == DEE_XASTKIND_LIST
DEE_A_RET_WUNUSED Dee_size_t DeeXAst_ListSize
#else
DEE_A_RET_WUNUSED Dee_size_t DeeXAst_SetSize
#endif
(DEE_A_IN DeeXAstObject const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeXAst_Check(self));
 switch (self->ast_kind) {
  case DEE_XASTKIND_MULTIVARDECL:
   if (self->ast_multivardecl.mvd_type == SEQ_TYPE)
    return self->ast_multivardecl.mvd_varc;
   break;
  case DEE_XASTKIND_CONST:
#if SEQ_TYPE == DEE_XASTKIND_TUPLE
   if (DeeTuple_Check(self->ast_const.c_const))
    return DeeTuple_SIZE(self->ast_const.c_const);
#elif SEQ_TYPE == DEE_XASTKIND_LIST
   if (DeeList_Check(self->ast_const.c_const))
    return DeeList_SIZE(self->ast_const.c_const);
#else
   if (DeeSet_Check(self->ast_const.c_const))
    return DeeSet_SIZE(self->ast_const.c_const);
#endif
   break;
  case SEQ_TYPE: {
   DeeXAstObject **iter,**end;
   end = (iter = self->ast_sequence.s_elemv)+self->ast_sequence.s_elemc;
   DEE_ASSERT(self->ast_sequence.s_elemc);
   do if ((*iter)->ast_kind == DEE_XASTKIND_EXPAND)
    return (Dee_size_t)-1; // Size is determined at runtime
   while (++iter != end);
   // If the tuple doesn't contain any expand expressions, we can known its size.
   return self->ast_sequence.s_elemc;
  } break;
  default: break;
 }
 return (Dee_size_t)-1;
}

DEE_DECL_END
#undef SEQ_TYPE
