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
DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseGeneratorDoWhileStatement(
 DEE_A_INOUT DeeTokenObject *do_token, DEE_PARSER_PARAMS)
#else
DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseDoWhileStatement(
 DEE_A_INOUT DeeTokenObject *do_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS)
#endif
{
 // Do-While loop
 DeeScopeObject *cond_scope,*loop_scope;
 DeeSAstObject *ast_result,*loop_block;
 DeeXAstObject *cond_ast;
 if ((cond_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,1)) == NULL) return NULL;
 if ((loop_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)cond_scope,1)) == NULL) {
err_cond_scope: Dee_DECREF(cond_scope); return NULL;
 }
#ifdef GENERATOR
 if ((loop_block = DeeSAst_ParseGeneratorBlock(DEE_PARSER_ARGS_SCOPE_EX(
  loop_scope,parser_flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE))) == NULL)
#else
 if ((loop_block = DeeSAst_Parse(DEE_PARSER_ARGS_SCOPE_EX(
  loop_scope,parser_flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE))) == NULL)
#endif
 {
err_do_1: Dee_DECREF(loop_scope); goto err_cond_scope;
 }
 if (token.tk_id != KWD_while) {
#ifdef GENERATOR
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_WHILE_AFTER_GENERATOR_DO,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected 'while' after generator 'do %r'",
   loop_block) != 0) goto err_do_2;
#else
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_WHILE_AFTER_DO,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected 'while' after 'do %r'",
   loop_block) != 0) goto err_do_2;
#endif
 } else if DEE_UNLIKELY(!yield()) {
err_do_2: Dee_DECREF(loop_block); goto err_do_1;
 }
 if (token.tk_id != '(') {
#ifdef GENERATOR
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_LPAREN_AFTER_GENERATOR_DO_WHILE,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after generator 'do %r' while",
   loop_block) != 0) goto err_do_2;
#else
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_LPAREN_AFTER_DO_WHILE,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after 'do %r' while",
   loop_block) != 0) goto err_do_2;
#endif
 } else if DEE_UNLIKELY(!yield()) goto err_do_2;
 if ((cond_ast = DeeXAst_ParseDecl(DEE_PARSER_ARGS_SCOPE_EX(
  cond_scope,parser_flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE))) == NULL) goto err_do_2;
 if (token.tk_id != ')') {
#ifdef GENERATOR
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_GENERATOR_DO_WHILE,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after generator 'do %r' while (%r",
   loop_block,cond_ast) != 0) goto err_do_3;
#else
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_DO_WHILE,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after 'do %r' while (%r",
   loop_block,cond_ast) != 0) goto err_do_3;
#endif
 } else if DEE_UNLIKELY(!yield()) {
err_do_3: Dee_DECREF(cond_ast); goto err_do_2;
 }
#ifdef GENERATOR
 ast_result = DeeSAst_NewDoWhile(do_token,DeeParserLabelRefList_Empty,lexer,parser_flags,
                                 cond_scope,cond_ast,loop_scope,loop_block);
#else
 ast_result = DeeSAst_NewDoWhile(do_token,labels_,lexer,parser_flags,
                                 cond_scope,cond_ast,loop_scope,loop_block);
#endif
 Dee_DECREF(cond_ast);
 Dee_DECREF(loop_block);
 Dee_DECREF(loop_scope);
 Dee_DECREF(cond_scope);
#ifndef GENERATOR
 if DEE_UNLIKELY(!ast_result) return NULL;
 if (token.tk_id != ';') {
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_DO_WHILE,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' after 'do %r' while (%r)",
   ast_result->ast_dowhile.w_block,ast_result->ast_dowhile.w_cond) != 0) goto err_r;
 } else if DEE_UNLIKELY(!yield()) {
err_r: Dee_DECREF(ast_result); return NULL;
 }
#endif
 return ast_result;
}


DEE_DECL_END
