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
extern DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseGeneratorIfStatement(
 DEE_A_INOUT DeeTokenObject *if_token, DEE_PARSER_PARAMS)
#else
DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseIfStatement(
 DEE_A_INOUT DeeTokenObject *if_token, DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS)
#endif
{
 DeeScopeObject *cond_scope,*succ_scope,*fail_scope;
 DeeSAstObject *succ_block,*fail_block,*ast_result;
 DeeXAstObject *cond_ast;
 // If statement
 if (token.tk_id != '(') {
#ifdef GENERATOR
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_GENERATOR_IF,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after generator 'if'") != 0) return NULL;
#else
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_IF,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after 'if'") != 0) return NULL;
#endif
 } else if DEE_UNLIKELY(!yield()) return NULL;
 if ((cond_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,1)) == NULL) return NULL;
 if ((cond_ast = DeeXAst_ParseDecl(DEE_PARSER_ARGS_SCOPE(cond_scope))) == NULL) {
err_cond_scope_ast_token: Dee_DECREF(cond_scope); return NULL;
 }
 if (token.tk_id != ')') {
#ifdef GENERATOR
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_GENERATOR_IF,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after generator 'if (%r'",cond_ast) != 0) goto err_cond_ast_ast_token;
#else
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_IF,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after 'if (%r'",cond_ast) != 0) goto err_cond_ast_ast_token;
#endif
 } else if DEE_UNLIKELY(!yield()) {
err_cond_ast_ast_token: Dee_DECREF(cond_ast); goto err_cond_scope_ast_token;
 }
 if ((succ_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)cond_scope,1)) == NULL) goto err_cond_ast_ast_token;
#ifdef GENERATOR
 if ((succ_block = DeeSAst_ParseGeneratorBlock(DEE_PARSER_ARGS_SCOPE(succ_scope))) == NULL)
#else
 if ((succ_block = DeeSAst_Parse(DEE_PARSER_ARGS_SCOPE(succ_scope))) == NULL)
#endif
 {
err_succ_scope_ast_token: Dee_DECREF(succ_scope); goto err_cond_ast_ast_token;
 }
 if (token.tk_id == KWD_else) {
  if DEE_UNLIKELY(!yield()) {
err_succ_block_ast_token: Dee_DECREF(succ_block); goto err_succ_scope_ast_token;
  }
#if DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS
parse_if_else_noyield:
#endif
  if ((fail_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)cond_scope,1)) == NULL) goto err_succ_block_ast_token;
#ifdef GENERATOR
  if ((fail_block = DeeSAst_ParseGeneratorBlock(DEE_PARSER_ARGS_SCOPE(fail_scope))) == NULL)
#else
  if ((fail_block = DeeSAst_Parse(DEE_PARSER_ARGS_SCOPE(fail_scope))) == NULL)
#endif
  {
/*err_fail_scope_ast_token:*/ Dee_DECREF(fail_scope); goto err_succ_block_ast_token;
  }
#if DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS
 } else if (token.tk_id == KWD_elif) {
  token.tk_id = KWD_if;
  goto parse_if_else_noyield;
#endif
 } else fail_scope = NULL,fail_block = NULL;
 ast_result = DeeSAst_NewIf(if_token,
#ifdef GENERATOR
                            DeeParserLabelRefList_Empty,
#else
                            labels_,
#endif
                            lexer,parser_flags,cond_scope,
                            cond_ast,succ_scope,succ_block,
                            fail_scope,fail_block);
 Dee_XDECREF(fail_block);
 Dee_XDECREF(fail_scope);
 Dee_DECREF(succ_block);
 Dee_DECREF(succ_scope);
 Dee_DECREF(cond_ast);
 Dee_DECREF(cond_scope);
 return ast_result;
}


DEE_DECL_END
