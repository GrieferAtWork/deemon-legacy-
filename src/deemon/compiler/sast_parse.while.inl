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
#include "sast_parse.c"
#define GENERATOR
#endif

DEE_DECL_BEGIN


#ifdef GENERATOR
DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseGeneratorWhileStatement(
 DEE_A_INOUT DeeTokenObject *while_token, DEE_PARSER_PARAMS)
#else
DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseWhileStatement(
 DEE_A_INOUT DeeTokenObject *while_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS)
#endif
{
 DeeScopeObject *cond_scope,*loop_scope;
 DeeSAstObject *ast_result,*loop_block;
 DeeXAstObject *cond_ast;
 // While loop
 if (token.tk_id != '(') {
#ifdef GENERATOR
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_GENERATOR_WHILE,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after generator 'while'"
   ) != 0) return NULL;
#else
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_WHILE,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after 'while'"
   ) != 0) return NULL;
#endif
 } else if DEE_UNLIKELY(!yield()) return NULL;
 if ((cond_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,1)) == NULL) return NULL;
 if ((cond_ast = DeeXAst_ParseDecl(DEE_PARSER_ARGS_SCOPE_EX(
  cond_scope,parser_flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)
  )) == NULL) {err_cond_scope: Dee_DECREF(cond_scope); return NULL; }
 if (token.tk_id != ')') {
#ifdef GENERATOR
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_GENERATOR_WHILE,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after generator 'while (%r'",
   cond_ast) != 0) goto err_cond_ast;
#else
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_WHILE,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after 'while (%r'",
   cond_ast) != 0) goto err_cond_ast;
#endif
 } else if DEE_UNLIKELY(!yield()) {
err_cond_ast: Dee_DECREF(cond_ast); goto err_cond_scope;
  }
 if ((loop_scope = (DeeScopeObject *)DeeScope_New((
  DeeObject *)cond_scope,1)) == NULL) goto err_cond_ast;
#ifdef GENERATOR
 if ((loop_block = DeeSAst_ParseGeneratorBlock(DEE_PARSER_ARGS_SCOPE_EX(
  loop_scope,parser_flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE))) == NULL)
#else
 if ((loop_block = DeeSAst_Parse(DEE_PARSER_ARGS_SCOPE_EX(
  loop_scope,parser_flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE))) == NULL)
#endif
 {
/*err_loop_scope:*/ Dee_DECREF(loop_scope); goto err_cond_scope;
 }
#ifdef GENERATOR
 ast_result = DeeSAst_NewWhile(while_token,DeeParserLabelRefList_Empty,lexer,parser_flags,
                               cond_scope,cond_ast,loop_scope,loop_block);
#else
 ast_result = DeeSAst_NewWhile(while_token,labels_,lexer,parser_flags,
                               cond_scope,cond_ast,loop_scope,loop_block);
#endif
 Dee_DECREF(loop_block);
 Dee_DECREF(loop_scope);
 Dee_DECREF(cond_ast);
 Dee_DECREF(cond_scope);
 return ast_result;
}




DEE_DECL_END
