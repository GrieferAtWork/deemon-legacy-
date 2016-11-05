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
#endif


DEE_DECL_BEGIN

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewTrinary(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeXAstObject *ast_a, DEE_A_INOUT DeeXAstObject *ast_b,
 DEE_A_INOUT DeeXAstObject *ast_c) {
 DeeXAstObject *result; DeeObject *const_copy_a,*const_copy_b,*const_copy_c,*trinary_result;
 DEE_ASSERT(DEE_XASTKIND_ISTRINARY(kind));
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(ast_a) && DeeXAst_Check(ast_a));
 DEE_ASSERT(DeeObject_Check(ast_b) && DeeXAst_Check(ast_b));
 DEE_ASSERT(DeeObject_Check(ast_c) && DeeXAst_Check(ast_c));
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_INTRINSIC_CALLS) != 0 &&
     ast_b->ast_kind == DEE_XASTKIND_CONST &&
     kind == DEE_XASTKIND_ATTR_SET) {
  if (DeeError_TypeError_CheckTypeExact(ast_b->ast_const.c_const,&DeeString_Type) != 0) goto handle_trinary_error;
  return DeeXAst_NewAttrSetC(tk,lexer,parser_flags,ast_a,ast_b->ast_const.c_const,ast_c);
 }
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0 &&
     ast_a->ast_kind == DEE_XASTKIND_CONST && ast_b->ast_kind == DEE_XASTKIND_CONST &&
     ast_c->ast_kind == DEE_XASTKIND_CONST) {
  if ((const_copy_a = DeeObject_DeepCopy(ast_a->ast_const.c_const)) == NULL) {
err_copy: // Ignore errors in copy, as the copy-operation wouldn't be performed at runtime
   DeeError_Handled();
   goto normal_ast;
  }
  if ((const_copy_b = DeeObject_DeepCopy(ast_b->ast_const.c_const)) == NULL) {
err_copy_2: Dee_DECREF(const_copy_a); goto err_copy;
  }
  if ((const_copy_c = DeeObject_DeepCopy(ast_c->ast_const.c_const)) == NULL) {
/*err_copy_3:*/ Dee_DECREF(const_copy_b); goto err_copy_2;
  }
  switch (kind) {
   case DEE_XASTKIND_SEQ_SET:
    if (DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_SEQ_SET,lexer,tk,Dee_TYPE(const_copy_a)) != 0) trinary_result = NULL;
    else if (DeeObject_SetItem(const_copy_a,const_copy_b,const_copy_c) == 0) Dee_INCREF(trinary_result = const_copy_c);
    else trinary_result = NULL;
    break;
   default:
    Dee_DECREF(const_copy_b);
    Dee_DECREF(const_copy_c);
    Dee_DECREF(const_copy_a);
    goto normal_ast;
  }
  Dee_DECREF(const_copy_b);
  Dee_DECREF(const_copy_c);
  Dee_DECREF(const_copy_a);
//handle_trinary_result:
  if (!trinary_result) {
   DeeTypeObject *rt_error;
handle_trinary_error:
   if ((rt_error = _DeeError_HandleAndReturnType()) != NULL) {
    int temp = DeeXAst_ThrowInvalidTrinaryOperatorError(kind,tk,lexer,ast_a,ast_b,ast_c,rt_error);
    Dee_DECREF(rt_error);
    if (temp != 0) return NULL;
   }
  } else {
//return_trinary_result:
   result = DeeXAst_NewConst(tk,trinary_result);
   Dee_DECREF(trinary_result);
   return result;
  }
 }
normal_ast:
 if ((result = _DeeXAst_NewUnsafe(kind,tk)) != NULL) {
  Dee_INCREF(result->ast_operator.op_a = ast_a);
  Dee_INCREF(result->ast_operator.op_b = ast_b);
  Dee_INCREF(result->ast_operator.op_c = ast_c);
 }
 return result;
}

DEE_DECL_END
