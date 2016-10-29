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
DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseGeneratorForOrForeachStatement(
 DEE_A_INOUT DeeTokenObject *for_token, DEE_A_IN DeeSAstKind forin_kind, DEE_PARSER_PARAMS)
#else
DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseForOrForeachStatement(
 DEE_A_INOUT DeeTokenObject *for_token, DEE_A_IN DeeSAstKind forin_kind,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS)
#endif
{
 // For statement
 DeeScopeObject *seq_scope,*var_scope,*block_scope;
 DeeXAstObject *var_ast,*seq,*loop_cond,*loop_next;
 DeeSAstObject *block,*ast_result;
 if (token.tk_id != '(') {
#ifdef GENERATOR
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_GENERATOR_FOR,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after generator 'for'") != 0) return NULL;
#else
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_FOR,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after 'for'") != 0) return NULL;
#endif
 } else if DEE_UNLIKELY(!yield()) return NULL;
 // Common part for both 'for (x: y)' and 'for (x;y;z)'-style loops
 if ((seq_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,1)) == NULL) return NULL;
 if ((var_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)seq_scope,1)) == NULL) {
err_seq_scope_ast_token: Dee_DECREF(seq_scope); return NULL;
 }
 if (token.tk_id == ';') { var_ast = NULL; goto parse_for; }
 // Here's a small different:
 //  - foreach-style parses what is the init-expression of an iterative loop one scope deeper.
 //    So once we can safely deduce that this isn't a foreach-style loop, we have to fiddle
 //    around with the scope to fix the context of the following line.
 if ((var_ast = DeeXAst_ParseDecl(DEE_PARSER_ARGS_SCOPE(var_scope))) == NULL) {
err_var_scope_ast_token: Dee_DECREF(var_scope); goto err_seq_scope_ast_token;
 }
 // 'loop_init_or_var' is either the variable that we will store the
 // element of a foreach, or the initializer of an iterative for loop.
 if (token.tk_id == ':' || token.tk_id == KWD_in) { // I know that 'in' can never happen...
parse_foreach:
  if DEE_UNLIKELY(!yield()) {
err_var_ast_token: Dee_XDECREF(var_ast); goto err_var_scope_ast_token;
  }
  // Parse the sequence expression one scope above the loop variable
  if ((seq = DeeXAst_ParseInit(DEE_PARSER_ARGS_SCOPE(seq_scope)
   )) == NULL) goto err_var_ast_token;
  if (token.tk_id != ')') {
#ifdef GENERATOR
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_GENERATOR_FOR,
    (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after generator 'for (%r: %r'",
    var_ast,seq) != 0) goto err_var_ast_token;
#else
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_FOR,
    (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after 'for (%r: %r'",
    var_ast,seq) != 0) goto err_var_ast_token;
#endif
  } else if DEE_UNLIKELY(!yield()) {
err_seq_ast_token: Dee_DECREF(seq); goto err_var_ast_token;
  }
#ifdef GENERATOR
  if ((block = DeeSAst_ParseGeneratorBlock(DEE_PARSER_ARGS_SCOPE_EX(
   var_scope,parser_flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)
   )) == NULL) goto err_seq_ast_token;
#else
  if ((block = DeeSAst_Parse(DEE_PARSER_ARGS_SCOPE_EX(
   var_scope,parser_flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)
   )) == NULL) goto err_seq_ast_token;
#endif
  // Finally create the ast (based on the original keyword)
  ast_result = _DeeSAst_NewForInOrForeach(forin_kind,for_token,
#ifdef GENERATOR
                                          DeeParserLabelRefList_Empty,
#else
                                          labels_,
#endif
                                          lexer,parser_flags,
                                          var_ast,seq_scope,seq,
                                          var_scope,block);
  Dee_DECREF(block);
  Dee_DECREF(seq);
 } else if (token.tk_id == ';') {
parse_for:
#define loop_init                    var_ast
#define head_init_scope              var_scope
#define head_cond_scope              seq_scope
#define err_loop_init_ast_token      err_var_ast_token
  // Parse iterative for loop
  if DEE_UNLIKELY(!yield()) goto err_loop_init_ast_token;
  // As said before: Now we must fiddle with the scopes
  // This is actually quite simple:
  //   Since we didn't do anything with the 'seq_scope', we
  //   can simply remove it and act like it's never existed.
  // Though we do re-use the seq-scope as the blockscope
  head_init_scope->sc_weak_prev = head_cond_scope->sc_weak_prev;
  head_cond_scope->sc_weak_prev = head_init_scope;
  if ((block_scope = (DeeScopeObject *)DeeScope_New((
   DeeObject *)head_cond_scope,1)) == NULL) goto err_loop_init_ast_token;
  if (token.tk_id != ';') {
   if ((loop_cond = DeeXAst_Parse(DEE_PARSER_ARGS_SCOPE_EX(
    head_cond_scope,parser_flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE))) == NULL) {
err_block_scope_ast_token: Dee_DECREF(block_scope); goto err_loop_init_ast_token;
   }
  } else loop_cond = NULL;
  if (token.tk_id != ';') {
#ifdef GENERATOR
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_SEMICOLLON_OR_COLLON_AFTER_GENERATOR_FOR,
    (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' after generator 'for (%r;%r'",
    loop_init ? (DeeObject *)loop_init : Dee_None,
    loop_cond ? (DeeObject *)loop_cond : Dee_None) != 0) goto err_loop_cond_ast_token;
#else
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_SEMICOLLON_OR_COLLON_AFTER_FOR,
    (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' after 'for (%r;%r'",
    loop_init ? (DeeObject *)loop_init : Dee_None,
    loop_cond ? (DeeObject *)loop_cond : Dee_None) != 0) goto err_loop_cond_ast_token;
#endif
  } else if DEE_UNLIKELY(!yield()) {
err_loop_cond_ast_token: Dee_XDECREF(loop_cond); goto err_block_scope_ast_token;
  }
  if (token.tk_id != ')') {
   if ((loop_next = DeeXAst_Parse(DEE_PARSER_ARGS_SCOPE_EX(
    block_scope,parser_flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)
    )) == NULL) goto err_loop_cond_ast_token;
  } else loop_next = NULL;
  if (token.tk_id != ')') {
#ifdef GENERATOR
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_GENERATOR_FOR,
    (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after generator 'for (%r;%r;%r'",
    loop_init ? (DeeObject *)loop_init : Dee_None,loop_cond ? (DeeObject *)loop_cond : Dee_None,
    loop_next ? (DeeObject *)loop_next : Dee_None) != 0) goto err_loop_next_ast_token;
#else
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_FOR,
    (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after 'for (%r;%r;%r'",
    loop_init ? (DeeObject *)loop_init : Dee_None,loop_cond ? (DeeObject *)loop_cond : Dee_None,
    loop_next ? (DeeObject *)loop_next : Dee_None) != 0) goto err_loop_next_ast_token;
#endif
  } else if DEE_UNLIKELY(!yield()) {
err_loop_next_ast_token: Dee_XDECREF(loop_cond); goto err_loop_cond_ast_token;
  }
#ifdef GENERATOR
  if ((block = DeeSAst_ParseGeneratorBlock(DEE_PARSER_ARGS_SCOPE_EX(
   block_scope,parser_flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)
   )) == NULL) goto err_loop_next_ast_token;
#else
  if ((block = DeeSAst_Parse(DEE_PARSER_ARGS_SCOPE_EX(
   block_scope,parser_flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)
   )) == NULL) goto err_loop_next_ast_token;
#endif
  ast_result = DeeSAst_NewFor(for_token,
#ifdef GENERATOR
                              DeeParserLabelRefList_Empty,
#else
                              labels_,
#endif
                              lexer,parser_flags,head_init_scope,
                              loop_init,head_cond_scope,loop_cond,loop_next,
                              block_scope,block);
  Dee_XDECREF(block);
  Dee_XDECREF(loop_next);
  Dee_XDECREF(loop_cond);
  Dee_DECREF(block_scope);
  //Dee_XDECREF(loop_init); // Done below ('loop_init_or_var')
#undef err_loop_init_ast_token
#undef head_cond_scope
#undef head_init_scope
#undef loop_init
 } else {
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_SEMICOLLON_OR_COLLON_AFTER_FOR,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' or ':' after 'for (%r'",
   var_ast ? (DeeObject *)var_ast : Dee_None) != 0
   ) goto err_var_ast_token;
  goto parse_foreach;
 }
 Dee_XDECREF(var_ast);
 Dee_DECREF(var_scope);
 Dee_DECREF(seq_scope);
 return ast_result;
}




DEE_DECL_END
