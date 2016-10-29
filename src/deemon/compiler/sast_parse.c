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
#ifndef GUARD_DEEMON_SAST_PARSE_C
#define GUARD_DEEMON_SAST_PARSE_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/object.h>
#include <deemon/deemonrun.h>
#include <deemon/none.h>
#include <deemon/type.h>
#include <deemon/string.h>
#include <deemon/compiler/sast.h>
#include <deemon/compiler/xast.h>

DEE_DECL_BEGIN

#define yield()  DeeLexer_YieldEx((DeeObject *)lexer,(DeeObject *)scope,config)
#define token_ob lexer->l_token
#define token    lexer->l_token->tk_token

DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_Parse(DEE_PARSER_PARAMS) {
 DeeSAstObject *ast_result;
 DeeXAstObject *ast_expression;
 DeeTokenObject *ast_token; DeeXAstKind kind;
 DeeScopeObject *cond_scope;
 struct DeeParserLabelRefList label_endpts = DeeParserLabelRefList_INIT();
again: switch (token.tk_id) {

  case KWD_case: {
   // Case label
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (DeeSAst_ParseCaseLabel(ast_token,&label_endpts,DEE_PARSER_ARGS) != 0) goto err_ast_token;
   Dee_DECREF(ast_token);
   goto again;
  } break;

  case KWD_default: {
   // Default label
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (DeeSAst_ParseDefaultLabel(ast_token,&label_endpts,DEE_PARSER_ARGS) != 0) goto err_ast_token;
   Dee_DECREF(ast_token);
   goto again;
  } break;

  case KWD_import: {
   DeeXAstObject *import_ast;
   // Import statement
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((import_ast = DeeXAst_ParseAllImports(ast_token,DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
   ast_result = DeeSAst_NewExpression(ast_token,&label_endpts,lexer,parser_flags,import_ast);
   Dee_DECREF(import_ast);
   Dee_DECREF(ast_token);
   if (!ast_result) goto err;
   if (token.tk_id != ';') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_IMPORT,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ';' after 'import ...'") != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
  } break;

  case ';': {
   // Empty statement
   if ((ast_result = DeeSAst_NewEmpty(token_ob,&label_endpts)) == NULL) goto err;
   if DEE_UNLIKELY(!yield()) {err_r: Dee_DECREF(ast_result); goto err; }
  } break;

  case '}': {
   // Empty statement (With warning)
   // NOTE: This is done to prevent a flood of errors further down the line
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_FOR_EMPTY_STATEMENT,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "Expected ';' for empty expression") != 0) goto err;
   if ((ast_result = DeeSAst_NewEmpty(token_ob,&label_endpts)) == NULL) goto err;
  } break;

  case '{': {
   // Block statement
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_result = DeeSAst_ParseAllInNewScopeUntil(ast_token,'}',&label_endpts,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
  } break;

  case KWD_return: {
   // Return statement
parse_return:
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != ';') {
    DeeScopeObject *weak_root;
    if ((ast_expression = DeeXAst_Parse(DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
    weak_root = DeeScope_WEAK_ROOT(scope);
    if ((weak_root->sc_flags&DEE_SCOPE_FLAG_FOUND_YIELD)!=0) {
     if (DeeError_CompilerError(DEE_WARNING_RETURN_AFTER_YIELD,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "non-empty 'return' statement found after a 'yield' statement had already been encountered"
      ) != 0) goto err_expression_ast_token;
     Dee_CLEAR(ast_expression);
    } else weak_root->sc_flags |= DEE_SCOPE_FLAG_FOUND_RETURN;
   } else ast_expression = NULL;
   ast_result = DeeSAst_NewReturn(ast_token,&label_endpts,ast_expression);
   Dee_XDECREF(ast_expression);
   Dee_DECREF(ast_token);
   if (!ast_result) goto err;
   if (token.tk_id != ';') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_RETURN,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' after 'return %r'",
     ast_result->ast_return.r_value) != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
  } break;

  case KWD_yield: {
   // Yield statement
   DeeScopeObject *weak_root;
   weak_root = DeeScope_WEAK_ROOT(scope);
   if ((weak_root->sc_flags&DEE_SCOPE_FLAG_FOUND_RETURN)!=0) {
    if (DeeError_CompilerError(DEE_WARNING_YIELD_AFTER_RETURN,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "'yield' statement found after a non-empty "
     "'return' statement had already been encountered") != 0) goto err;
    goto parse_return; // Just parse it as an return
   } else if ((weak_root->sc_flags&DEE_SCOPE_FLAG_FOUND_RETVAR)!=0) {
    if (DeeError_CompilerError(DEE_WARNING_YIELD_AFTER_RETVAR,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "'yield' statement found in a return-variable function") != 0) goto err;
    goto parse_return; // Just parse it as an return (again...)
   } else weak_root->sc_flags |= DEE_SCOPE_FLAG_FOUND_YIELD;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((ast_expression = DeeXAst_ParseTuple(DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
   ast_result = DeeSAst_NewYield(ast_token,&label_endpts,ast_expression);
   Dee_DECREF(ast_expression);
   Dee_DECREF(ast_token);
   if (!ast_result) goto err;
   if (token.tk_id != ';') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_YIELD,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' after 'yield %r...'",
     ast_result->ast_yield.y_value) != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
  } break;

  case KWD___assert:
  case KWD_assert: {
   DeeXAstObject *assert_message;
   // Assert statement
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((ast_expression = DeeXAst_Parse(DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
   if (token.tk_id == ':') {
    if DEE_UNLIKELY(!yield()) {err_expression_ast_token: Dee_DECREF(ast_expression); goto err_ast_token; }
    if ((assert_message = DeeXAst_Parse(DEE_PARSER_ARGS)) == NULL) goto err_expression_ast_token;
   } else assert_message = NULL;
   ast_result = DeeSAst_NewAssert(ast_token,&label_endpts,lexer,parser_flags,
                                  ast_expression,assert_message);
   Dee_XDECREF(assert_message);
   Dee_DECREF(ast_expression);
   Dee_DECREF(ast_token);
   if (!ast_result) goto err;
   if (token.tk_id != ';') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_ASSERT,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' after 'assert ...'") != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
  } break;

  case KWD_print: {
   DeeXAstObject *print_file,*print_data;
   Dee_uint32_t print_flags = DEE_SASTPRINT_FLAG_NONE;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id == ',') {
    if DEE_UNLIKELY(!yield()) goto err_ast_token;
    print_flags |= DEE_SASTPRINT_FLAG_NOLF;
    print_file = NULL;
    print_data = NULL;
   } else if (token.tk_id != ';') {
    // Parse the print file/data
    if ((print_file = _DeeXAst_ParseInitOrDecl(
     DEE_XAST_VARDECL_MODE_DEFAULT,
     DEE_XAST_INITORDECL_FLAGS_ALLOWINIT|
     DEE_XAST_INITORDECL_FLAGS_RETURNSEQ|
     DEE_XAST_INITORDECL_FLAGS_NOPOSTCOMMA|
     DEE_XAST_INITORDECL_FLAGS_NOSEQONCOLLON, // Don't return a sequence on ':'
     DEE_XASTKIND_TUPLE,NULL,NULL,DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
    if (token.tk_id == ':') {
     // Print into a custom file
     if DEE_UNLIKELY(!yield()) {err_print_file_ast_token: Dee_DECREF(print_file); goto err_ast_token; }
     if (token.tk_id == ',' || token.tk_id == ';') print_data = NULL;
     else {
      if ((print_data = _DeeXAst_ParseInitOrDecl(
       DEE_XAST_VARDECL_MODE_DEFAULT,
       DEE_XAST_INITORDECL_FLAGS_ALLOWINIT|
       DEE_XAST_INITORDECL_FLAGS_RETURNSEQ|
       DEE_XAST_INITORDECL_FLAGS_NOPOSTCOMMA,
       DEE_XASTKIND_TUPLE,NULL,NULL,DEE_PARSER_ARGS)) == NULL) goto err_print_file_ast_token;
     }
    } else print_data = print_file,print_file = NULL;
    if (token.tk_id == ',') {
     if DEE_UNLIKELY(!yield()) { Dee_XDECREF(print_data); goto err_print_file_ast_token; }
     print_flags |= DEE_SASTPRINT_FLAG_NOLF;
    }
   } else print_file = NULL,print_data = NULL;
   ast_result = DeeSAst_NewPrint(ast_token,&label_endpts,lexer,parser_flags,
                                 print_flags,print_file,print_data);
   Dee_XDECREF(print_data);
   Dee_XDECREF(print_file);
   Dee_XDECREF(ast_token);
   if (token.tk_id != ';') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_PRINT,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' after 'print ...'") != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
  } break;

  case KWD_if: {
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_result = DeeSAst_ParseIfStatement(ast_token,&label_endpts,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
  } break;

  case KWD___foreach: kind = DEE_SASTKIND_FOREACH; goto parse_any_for;
  case KWD_for: kind = DEE_SASTKIND_FORIN; parse_any_for: {
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_result = DeeSAst_ParseForOrForeachStatement(ast_token,kind,&label_endpts,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
  } break;

  case KWD_while: {
   // While loop
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_result = DeeSAst_ParseWhileStatement(ast_token,&label_endpts,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
  } break;

  case KWD_do: {
   // Do-While loop
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_result = DeeSAst_ParseDoWhileStatement(ast_token,&label_endpts,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
  } break;

  { // Special loop statements
   if (0) { case KWD___looponce: kind = DEE_SASTKIND_LOOPONCE; }
   if (0) { case KWD___loopever: kind = DEE_SASTKIND_LOOPEVER; }
   if (0) { case KWD___loopnone: kind = DEE_SASTKIND_LOOPNONE; }
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_result = DeeSAst_ParseSpecialLoopStatement(kind,ast_token,&label_endpts,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
  } break;

  {
   DeeSAstObject *succ_block,*fail_block;
   DeeScopeObject *succ_scope,*fail_scope;
   if (0) { case KWD___if_true: kind = DEE_SASTKIND_IFTRUE; }
   if (0) { case KWD___if_false: kind = DEE_SASTKIND_IFFALSE; }
   // __deadbranch Statement
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((succ_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,1)) == NULL) goto err_ast_token;
   if ((succ_block = DeeSAst_Parse(DEE_PARSER_ARGS_SCOPE(succ_scope))) == NULL) {
err_ifc_succ_scope: Dee_DECREF(succ_scope); goto err_ast_token;
   }
   if (token.tk_id == KWD_else) {
    if DEE_UNLIKELY(!yield()) goto err_ifc_succ_block;
#if DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS
parse_ifc_else_noyield:
#endif
    if ((fail_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,1)) == NULL) {
err_ifc_succ_block: Dee_DECREF(succ_block); goto err_ifc_succ_scope;
    }
    if ((fail_block = DeeSAst_Parse(DEE_PARSER_ARGS_SCOPE(fail_scope))) == NULL) {
/*err_ifc_fail_scope:*/ Dee_DECREF(fail_scope); goto err_ifc_succ_block;
    }
   }
#if DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS
   else if (token.tk_id == KWD_elif) {
    token.tk_id = KWD_if;
    goto parse_ifc_else_noyield;
   }
#endif
   else fail_scope = NULL,fail_block = NULL;
   ast_result = DeeSAst_NewIfConst(kind,ast_token,&label_endpts,
                                          succ_scope,succ_block,
                                          fail_scope,fail_block);
   Dee_XDECREF(fail_block);
   Dee_XDECREF(fail_scope);
   Dee_DECREF(succ_block);
   Dee_DECREF(succ_scope);
   Dee_DECREF(ast_token);
  } break;

  case KWD_break: kind = DEE_SASTKIND_BREAK; goto parse_break_or_continue;
  case KWD_continue: kind = DEE_SASTKIND_CONTINUE; parse_break_or_continue: {
   // Break/Continue statement
   if ((ast_result = _DeeSAst_NewEmptyEx(kind,token_ob,&label_endpts)) == NULL) goto err;
   if DEE_UNLIKELY(!yield()) goto err_r;
   if (token.tk_id != ';') {
    if (DeeError_CompilerErrorf(kind == DEE_SASTKIND_BREAK
     ? DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_BREAK
     : DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_CONTINUE,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' after '%s",
     TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),
     ast_result->ast_common.ast_token->tk_token.tk_id)) != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
  } break;

  case KWD_goto: {
   DeeScopeObject *weak_root;
   // Goto statement
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_GOTO,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected keyword after 'goto'") != 0) goto err_ast_token;
    ast_result = DeeSAst_NewEmpty(ast_token,&label_endpts);
   } else {
    struct DeeParserLabel *dest_label;
    weak_root = DeeScope_WEAK_ROOT(scope);
    if ((dest_label = DeeParserLabelList_GetOrAddForwardLabel(
     &weak_root->sc_labels,token.tk_id)) == NULL) goto err_ast_token;
    if DEE_UNLIKELY(!yield()) goto err_ast_token;
    ast_result = DeeSAst_NewGoto(ast_token,&label_endpts,dest_label);
   }
   Dee_DECREF(ast_token);
   if (!ast_result) goto err;
   if (token.tk_id != ';') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_GOTO,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' after 'goto ...'"
     ) != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
  } break;

  case KWD_throw: {
   // Throw statement
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != ';') {
    if ((ast_expression = DeeXAst_Parse(DEE_PARSER_ARGS)) == NULL) goto err_ast_token;
   } else ast_expression = NULL;
   ast_result = DeeSAst_NewThrow(ast_token,&label_endpts,ast_expression);
   Dee_XDECREF(ast_expression);
   Dee_DECREF(ast_token);
   if (!ast_result) goto err;
   if (token.tk_id != ';') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_THROW,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' after 'throw ...'"
     ) != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
  } break;

  case KWD_try: {
   DeeScopeObject *try_root_scope,*try_block_scope;
   DeeSAstObject *try_block;
   // Try statement
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((try_root_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,1)) == NULL) goto err_ast_token;
   if ((try_block_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)try_root_scope,1)) == NULL) {
err_try_1: Dee_DECREF(try_root_scope); goto err_ast_token;
   }
   if ((try_block = DeeSAst_Parse(DEE_PARSER_ARGS_SCOPE(try_block_scope))) == NULL) {
/*err_try_2:*/ Dee_DECREF(try_block_scope); goto err_try_1;
   }
   ast_result = DeeSAst_NewTry(ast_token,&label_endpts,
                               try_root_scope,try_block_scope,try_block);
   Dee_DECREF(try_block);
   Dee_DECREF(try_block_scope);
   if (!ast_result) goto err_try_1;
   Dee_DECREF(ast_token);
   // Parse try handler blocks
try_handler_more:
   switch (token.tk_id) {
    case KWD_catch:
    case KWD_finally:
     if (_DeeSAst_ParseTryHandler(ast_result,DEE_PARSER_ARGS_SCOPE(try_root_scope)) != 0) {
      Dee_DECREF(try_root_scope);
      goto err_r;
     }
     goto try_handler_more;
    default: break;
   }
   Dee_DECREF(try_root_scope);
   // Finalize the try block
   if (DeeSAst_FinalizeTry(ast_result,lexer,parser_flags) != 0) goto err_r;
  } break;

  case KWD_module: {
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_result = DeeSAst_ParseModuleStatement(ast_token,&label_endpts,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
  } break;

  case KWD_switch: {
   DeeScopeObject *weak_root,*block_scope;
   struct DeeParserLabelList *old_cases;
   DeeXAstObject *switch_expr;
   DeeSAstObject *switch_block;
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_SWITCH,
     (DeeObject *)lexer,(DeeObject *)token_ob,"expected '(' after 'switch'") != 0) goto err_ast_token;
   } else if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if ((cond_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,1)) == NULL) goto err_ast_token;
   if ((switch_expr = DeeXAst_ParseDecl(DEE_PARSER_ARGS_SCOPE(cond_scope))) == NULL) {
err_cond_scope_ast_token: Dee_DECREF(cond_scope); goto err_ast_token;
   }
   if (token.tk_id != ')') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_AFTER_SWITCH,
     (DeeObject *)lexer,(DeeObject *)token_ob,"expected ')' after 'switch (%r'",switch_expr) != 0) {
errs_switch_expr_cond_scope: Dee_DECREF(switch_expr); goto err_cond_scope_ast_token;
    }
   } else if DEE_UNLIKELY(!yield()) goto errs_switch_expr_cond_scope;
   {struct DeeParserLabelList switch_cases = DeeParserLabelList_INIT();
    weak_root = DeeScope_WEAK_ROOT(scope);
    old_cases = weak_root->sc_cases;
    weak_root->sc_cases = &switch_cases;
    if ((block_scope = (DeeScopeObject *)DeeScope_New((
     DeeObject *)cond_scope,1)) == NULL) {
     weak_root->sc_cases = old_cases;
err_switch_switch_cases:
     _DeeParserLabelList_Quit(&switch_cases);
     goto errs_switch_expr_cond_scope;
    }
    switch_block = DeeSAst_Parse(DEE_PARSER_ARGS_SCOPE_EX(
     block_scope,parser_flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE));
    weak_root->sc_cases = old_cases;
    if (!switch_block) { Dee_DECREF(block_scope); goto err_switch_switch_cases; }
    ast_result = DeeSAst_NewSwitch(ast_token,&label_endpts,lexer,parser_flags,
                                   cond_scope,switch_expr,block_scope,switch_block,&switch_cases);
    _DeeParserLabelList_Quit(&switch_cases);
    Dee_DECREF(switch_block);
    Dee_DECREF(block_scope);
    Dee_DECREF(switch_expr);
    Dee_DECREF(cond_scope);
    Dee_DECREF(ast_token);
   }
  } break;

  case KWD_del: {
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_expression = DeeXAst_ParseAllDel(ast_token,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
   if (!ast_expression) goto err;
   ast_result = DeeSAst_NewExpression(ast_expression->ast_common.ast_token,
                                      &label_endpts,lexer,parser_flags,ast_expression);
   Dee_DECREF(ast_expression);
   if (!ast_result) goto err;
   if (token.tk_id != ';') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_DEL,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' after 'del ...'"
     ) != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
  case KWD_extern: {
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id == ';') {
    if (DeeError_CompilerError(DEE_WARNING_EMPTY_EXTERN_DECLARATION,
     (DeeObject *)lexer,(DeeObject *)ast_token,"Empty extern declaration"
     ) != 0) goto err_ast_token;
    ast_result = DeeSAst_NewEmpty(ast_token,&label_endpts);
   } else ast_result = DeeSAst_ParseExternStatement(ast_token,&label_endpts,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
   if (!ast_result) goto err;
   if (token.tk_id != ';') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_EXTERN,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' after 'extern ...'"
     ) != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */

  case KWD_typedef: {
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id == ';') {
    if (DeeError_CompilerError(DEE_WARNING_EMPTY_TYPEDEF_DECLARATION,
     (DeeObject *)lexer,(DeeObject *)ast_token,"Empty typedef declaration"
     ) != 0) goto err_ast_token;
    ast_result = DeeSAst_NewEmpty(ast_token,&label_endpts);
   } else ast_result = DeeSAst_ParseTypedefStatement(ast_token,&label_endpts,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
   if (!ast_result) goto err;
   if (token.tk_id != ';') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_TYPEDEF,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' after 'typedef ...'"
     ) != 0) goto err_r;
   } else if DEE_UNLIKELY(!yield()) goto err_r;
  } break;

#ifdef DEE_SAST_RESERVED_KEYWORD_TOKENS
  DEE_SAST_RESERVED_KEYWORD_TOKENS
   if (DeeError_CompilerError(DEE_WARNING_USING_RESERVED_KEYWORD,
    (DeeObject *)lexer,(DeeObject *)token_ob,"Using reserved keyword"
    ) != 0) goto err;
   DEE_ATTRIBUTE_FALLTHROUGH
#endif
  if (0) {
  case KWD_catch:
  case KWD_finally:
   if (DeeError_CompilerError(DEE_WARNING_CATCH_FINALLY_MUST_FOLLOW_TRY,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "'catch' and 'finally' must follow 'try'") != 0) goto err;
  }
  if (0) {
  case KWD_else:
#if DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS
  case KWD_elif:
   if (DeeError_CompilerError(DEE_WARNING_ELSE_ELIF_MUST_FOLLOW_IF,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "'else' and 'elif' must follow 'if'") != 0) goto err;
#else /* DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS */
   if (DeeError_CompilerError(DEE_WARNING_ELSE_ELIF_MUST_FOLLOW_IF,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "'else' must follow 'if'") != 0) goto err;
#endif /* !DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS */
  }

  default:
   if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
    // Check for a label endpoint
    Dee_INCREF(ast_token = token_ob);
    if DEE_UNLIKELY(!yield()) {err_ast_token: Dee_DECREF(ast_token); goto err; }
    if (token.tk_id == ':') { // Yup! It's a label
     DeeScopeObject *weak_root;
     struct DeeParserLabel *label_endp;
     if DEE_UNLIKELY(!yield()) goto err_ast_token;
     weak_root = DeeScope_WEAK_ROOT(scope);
     if ((label_endp = DeeParserLabelList_GetLabel(
      &weak_root->sc_labels,ast_token->tk_token.tk_id)) != NULL) {
      if (DeeParserLabel_IS_FORWARD(label_endp)) {
       // Initialize forward labels
       DeeParserLabel_DECLARE(label_endp,ast_token);
       goto add_label_endp;
      } else {
       if (DeeError_CompilerErrorf(DEE_WARNING_LABEL_ENDPOINT_ALREADY_DEFINED,
        (DeeObject *)lexer,(DeeObject *)ast_token,
        "Label endpoint was already defined\n"
        "%s(%d) : %k : See reference to previous definition",
        DeeToken_FILE(label_endp->pl_dest_token),
        DeeToken_LINE(label_endp->pl_dest_token)+1,
        label_endp->pl_dest_token) != 0) goto err_ast_token;
      }
     } else {
      if ((label_endp = DeeParserLabelList_AddLabel(
       &weak_root->sc_labels,ast_token,ast_token->tk_token.tk_id)) == NULL) goto err_ast_token;
add_label_endp:
      if (DeeParserLabelRefList_Append(&label_endpts,label_endp) != 0) goto err_ast_token;
     }
     Dee_DECREF(ast_token);
     goto again; // Scan more
    }
    // Not a label
    if (DeeLexer_Return((DeeObject *)lexer,(DeeObject *)ast_token) != 0) goto err_ast_token;
    Dee_DECREF(ast_token);
   }
   DEE_ATTRIBUTE_FALLTHROUGH
   // A select set of tokens for which we skip the label check
   // -> These are keywords that can never be used as label names,
   //    some of which actually implement a different meaning to a
   //    ':' after the keyword (mostly for declaring a type base)
   // NOTE: Some additional tokens were added to improve performance by skipping the peek for ':'
  DEE_ATTRIBUTEDECL_TOKENS
  DEE_XAST_UNARY_TOKENS_NOSTMT
  DEE_XAST_VARIBLE_LOOKUP_TOKENS_NOSTMT
   if ((ast_expression = DeeSAst_ParseExpression(DEE_PARSER_ARGS)) == NULL) goto err;
   ast_result = DeeSAst_NewExpression(ast_expression->ast_common.ast_token,
                                      &label_endpts,lexer,parser_flags,ast_expression);
   Dee_DECREF(ast_expression);
   break;
 }
end: _DeeParserLabelRefList_Quit(&label_endpts);
 return ast_result;
err: ast_result = NULL; goto end;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeSAst_ParseExpression(DEE_PARSER_PARAMS) {
 DeeXAstObject *result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 if (DeeAttributeDecl_Parse(&attr,DEE_PARSER_ARGS) == 0)
  result = DeeSAst_ParseExpressionWithAttribute(&attr,DEE_PARSER_ARGS);
 else result = NULL;
 _DeeAttributeDecl_Quit(&attr);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeSAst_ParseExpressionWithAttribute(
DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_result; DeeTokenObject *ast_token;
 Dee_uint32_t vardecl_mode = DEE_XAST_VARDECL_MODE_DEFAULT|DEE_XAST_VARDECL_FLAG_ENABLED;
again: switch (token.tk_id) {

  DEE_XAST_VARIBLE_LOOKUP_MODIFER_TOKENS
   if (DeeXAst_ParseVariableLookupModifiers(&vardecl_mode,attr,DEE_PARSER_ARGS) != 0) return NULL;
   goto again;

  case KWD_function: {
   Dee_uint32_t syntax_mode;
   // Special handling for 'function', allowing it to be written without a post-';'
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) {err_ast_token: Dee_DECREF(ast_token); return NULL; }
   if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_ast_token;
   ast_result = DeeXAst_ParseFunctionWithAttribute(
    ast_token,vardecl_mode,&syntax_mode,attr,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
   if DEE_UNLIKELY(!ast_result) return NULL;
   // Expression-style functions still require a ';' afterwards
   if (DEE_XAST_FUNCTION_SYNTAX_MODE(syntax_mode) ==
       DEE_XAST_FUNCTION_SYNTAX_MODE_EXPR) {
    if (token.tk_id != ';') {
     if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_EXPRESSION,
      (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' after expression '%r'",
      ast_result) != 0) goto err_r;
    } else if DEE_UNLIKELY(!yield()) {
err_r: Dee_DECREF(ast_result); return NULL;
    }
   } else {
    // Still consume an optional ';' if it follows a statement-style function
    if (token.tk_id == ';' && !yield()) goto err_r;
   }
  } break;

  default:
   if ((ast_result = _DeeXAst_ParseInitOrDecl(vardecl_mode,
    DEE_XAST_INITORDECL_FLAGS_ALLOWINIT|
    DEE_XAST_INITORDECL_FLAGS_REQSEMICOLLON,
    DEE_XASTKIND_TUPLE,attr,NULL,DEE_PARSER_ARGS)) == NULL) return NULL;
   break;
 }
 return ast_result;
}


DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseAllInOldScopeUntil(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_IN TPPTokenID end_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS) {
 DeeSAstObject *result,*single_stmt,**iter,**end,**new_statementv,**statementv = NULL;
 Dee_size_t statementa = 0,statementc = 0; Dee_uint64_t old_token_pos;
 // Create the child scope in which the statements will be parsed
 while (1) { // Make sure to not hang on an unexpected EOF
  if (token.tk_id == end_token) {
   // End token has been reached. - Consume it and stop iterating
   if DEE_UNLIKELY(!yield()) {
err: end = (iter = statementv)+statementc;
    while (iter != end) Dee_DECREF(*iter++);
    free(statementv);
    return NULL;
   }
   break; // Done!
  } else if (!token.tk_id) {
   if (DeeError_CompilerError(DEE_WARNING_UNEXPECTED_EOF_IN_STATEMENT,
    (DeeObject *)lexer,(DeeObject *)token_ob,"Unexpected EOF in statement"
    ) != 0) goto err;
   break;
  }
  // Parse one statement
  old_token_pos = lexer->l_token_pos;
  if ((single_stmt = DeeSAst_Parse(DEE_PARSER_ARGS)) == NULL) goto err;
  // Append it to our list of statements
  if (statementc == statementa) {
   // Must realloc out list of statements
   if (!statementa) statementa = 2; else statementa *= 2;
   while ((new_statementv = (DeeSAstObject **)realloc_nz(
    statementv,statementa*sizeof(DeeSAstObject *))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    Dee_DECREF(single_stmt);
    DeeError_NoMemory();
    goto err;
   }
   statementv = new_statementv;
  }
  // Place the statement in our list
  Dee_INHERIT_REF(statementv[statementc++],single_stmt);
  if (lexer->l_token_pos == old_token_pos && !yield()) goto err;
 }
 DEE_ASSERT((statementc == 0) == (statementv == NULL));
 if (!statementc) return DeeSAst_NewEmpty(tk,labels_);
 result = DeeSAst_NewBlockFromInheritedStatements(tk,labels_,lexer,parser_flags,
                                                  scope,statementc,statementv);
 if (!result) goto err; // Must cleanup the scope and statements ourselves
 return result;
}
DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseAllInNewScopeUntil(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_IN TPPTokenID end_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS) {
 DeeSAstObject *result;
 DeeScopeObject *child_scope;
 // Create the child scope in which the statements will be parsed
 if ((child_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,1)) == NULL) return NULL;
 result = DeeSAst_ParseAllInOldScopeUntil(tk,end_token,labels_,DEE_PARSER_ARGS_SCOPE(child_scope));
 Dee_DECREF(child_scope);
 return result;
}


DEE_A_RET_EXCEPT(-1) int _DeeSAst_ParseTryHandler(
 DEE_A_INOUT DeeSAstObject *self, DEE_PARSER_PARAMS) {
 int result;
 DeeExceptionHandlerKind handler_kind;
 DeeXAstObject *exception_storage; /*< [0..1]. */
 DeeTypeObject *type_mask;         /*< [0..1]. */
 DeeScopeObject *handler_scope;    /*< [1..1]. */
 DeeSAstObject *handler_block;     /*< [1..1]. */
 DEE_ASSERT(token.tk_id == KWD_finally || token.tk_id == KWD_catch);
 if ((handler_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,1)) == NULL) return -1;
 switch (token.tk_id) {

  case KWD_finally: {
   if DEE_UNLIKELY(!yield()) {err_handler_scope: Dee_DECREF(handler_scope); return -1; }
   if ((handler_block = DeeSAst_Parse(DEE_PARSER_ARGS_SCOPE(handler_scope))) == NULL) goto err_handler_scope;
   exception_storage = NULL;
   type_mask = NULL;
   handler_kind = DeeExceptionHandleKind_FINALLY;
  } break;

  case KWD_catch: {
   if DEE_UNLIKELY(!yield()) goto err_handler_scope;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_CATCH,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '(' after 'catch'"
     ) != 0) goto err_handler_scope;
   } else if DEE_UNLIKELY(!yield()) goto err_handler_scope;
   if (token.tk_id == TPP_TOK_DOTS) {
    if DEE_UNLIKELY(!yield()) goto err_handler_scope;
    if (token.tk_id == ')') { // 'catch (...)'
     handler_kind = DeeExceptionHandleKind_ALL;
     type_mask = NULL;
     exception_storage = NULL;
     goto catch_end;
    }
    handler_kind = DeeExceptionHandleKind_ALL_VAR;
   } else handler_kind = DeeExceptionHandleKind_TYPED;
   // Parse the exception storage of its type_mask
   // NOTE: We don't parse post-script '...' here, as that indicates 
   if ((exception_storage = DeeXAst_ParseUnaryEx(
    DEE_XAST_UNARAYSUFFIX_FLAG_NOEXPAND,DEE_XAST_VARDECL_FLAG_ENABLED,
    NULL,DEE_PARSER_ARGS_SCOPE_EX(handler_scope,(Dee_uint32_t)(
    (handler_kind&DeeExceptionHandleKind_FLAG_ALL)!=0 ? parser_flags
    : (parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))))) == NULL) goto err_handler_scope;
   if (handler_kind == DeeExceptionHandleKind_ALL_VAR) { type_mask = NULL; goto catch_end; } // "catch (...x)"
   if (token.tk_id == TPP_TOK_DOTS) { // "catch (x...)"
    if DEE_UNLIKELY(!yield()) {err_catch_1: Dee_DECREF(exception_storage); goto err_handler_scope; }
    handler_kind = DeeExceptionHandleKind_ALL_VAR;
    type_mask = NULL;
    goto catch_end;
   }
   // Exception storage is actually the type!: "catch (int x)"
   if (exception_storage->ast_kind != DEE_XASTKIND_CONST) {
    // TODO: We should allow and compile runtime evaluation of exception types like this:
    // >> Terror = string;     // >> Terror = string;
    // >> try {                // >> try {
    // >>   throw "NOPE!";     // >>   throw "NOPE!";
    // >> } catch (Terror e) { // >> } catch (e...) {
    // >>   print "Caught:",e; // >>   if (e !is Terror) throw;
    // >> }                    // >>   print "Caught:",e;
    // >>                      // >> }
    // NOTE: It's not that easy, as the rethrow must be allowed to be
    //       forwarded to other handlers within the same catch block:
    // >> try {
    // >>   throw "Nope!";
    // >> } catch (e...) {
    // >>   if (e is string) throw; // This would be forwarded to the catch below...
    // >> } catch (e...) {
    // >>   print e;
    // >> }
    // >> try {
    // >>   try {
    // >>     throw "Nope!";
    // >>   } catch (e...) {
    // >>     if (e is string) throw; // But it would be here
    // >>   }
    // >> } catch (e...) {
    // >>   print e;
    // >> }
    if ((result = DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_CATCH,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected constant after 'catch(', but got %r",
     exception_storage)) != 0) goto err_catch_1;
    Dee_INCREF(type_mask = &DeeObject_Type);
   } else {
    Dee_INCREF(type_mask = (DeeTypeObject *)exception_storage->ast_const.c_const);
    if (!DeeType_Check(type_mask)) {
     Dee_DECREF(type_mask);
     if ((result = DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_TYPE_EXPRESSION_AFTER_CATCH,
      (DeeObject *)lexer,(DeeObject *)token_ob,"Expected type after 'catch(', but got instance of %s",
      DeeType_NAME(Dee_TYPE(exception_storage->ast_const.c_const)))) != 0) goto err_catch_1;
     Dee_INCREF(type_mask = Dee_TYPE(exception_storage->ast_const.c_const));
    }
   }
   Dee_CLEAR(exception_storage);
   handler_kind = DeeExceptionHandleKind_TYPED;
   // Check for storage for the caught expression
   if (token.tk_id != ')') {
    // Must parse an additional storage expression
    // TODO: Parse this as a named type suffix
    if ((exception_storage = DeeXAst_ParseUnaryEx(
     DEE_XAST_UNARAYSUFFIX_FLAG_NOEXPAND,DEE_XAST_VARDECL_FLAG_ENABLED,
     NULL,DEE_PARSER_ARGS_SCOPE(handler_scope))) == NULL) {
/*err_catch_2:*/ Dee_DECREF(type_mask); goto err_handler_scope;
    }
    handler_kind |= DeeExceptionHandleKind_FLAG_VAR;
   }
catch_end:
   if (token.tk_id != ')') {
    if ((result = DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_CATCH,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after 'catch(...'"
     )) != 0) goto err_catch_3;
   } else if DEE_UNLIKELY(!yield()) {
err_catch_3: Dee_XDECREF(exception_storage); Dee_XDECREF(type_mask); goto err_handler_scope;
   }
   // Finally parse the handler block
   if ((handler_block = DeeSAst_Parse(DEE_PARSER_ARGS_SCOPE(handler_scope))) == NULL) goto err_catch_3;
  } break;

  default: DEE_BUILTIN_UNREACHABLE();
 }
 result = DeeSAst_AddTryHandler(self,handler_kind,lexer,parser_flags,
                                exception_storage,type_mask,
                                handler_scope,handler_block);
 Dee_XDECREF(type_mask);
 Dee_XDECREF(exception_storage);
 Dee_DECREF(handler_block);
 Dee_DECREF(handler_scope);
 return result;
}

DEE_DECL_END

#ifndef __INTELLISENSE__
#include "sast_parse.case_label.inl"
#include "sast_parse.do_while.inl"
#include "sast_parse.extern.inl"
#include "sast_parse.for_or_foreach.inl"
#include "sast_parse.if_statement.inl"
#include "sast_parse.module.inl"
#include "sast_parse.specialloop.inl"
#include "sast_parse.typedef.inl"
#include "sast_parse.while.inl"
#if DEE_CONFIG_LANGUAGE_HAVE_GENERATORS
#include "sast_parse.generator_block.inl"
#define GENERATOR
#include "sast_parse.do_while.inl"
#include "sast_parse.for_or_foreach.inl"
#include "sast_parse.if_statement.inl"
#include "sast_parse.specialloop.inl"
#include "sast_parse.while.inl"
#undef GENERATOR
#endif
#endif

#endif /* !GUARD_DEEMON_SAST_PARSE_C */
