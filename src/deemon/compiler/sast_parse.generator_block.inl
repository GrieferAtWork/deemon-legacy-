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
#endif

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT_REF DeeSAstObject *
DeeSAst_ParseGeneratorBlock(DEE_PARSER_PARAMS) {
 DeeSAstKind kind;
 DeeXAstObject *yield_expression;
 DeeSAstObject *ast_result;
 DeeTokenObject *ast_token;

 switch (token.tk_id) {

  {
   if (0) { case KWD_for: kind = DEE_SASTKIND_FORIN; }
   if (0) { case KWD___foreach: kind = DEE_SASTKIND_FOREACH; }
   // Generator for-statement
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_result = DeeSAst_ParseGeneratorForOrForeachStatement(ast_token,kind,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;

  case KWD_while: {
   // Generator while-statement
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_result = DeeSAst_ParseGeneratorWhileStatement(ast_token,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;

  case KWD_do: {
   // Generator do...while-statement
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_result = DeeSAst_ParseGeneratorDoWhileStatement(ast_token,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;

  case KWD_if: {
   // Generator if-statement
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   ast_result = DeeSAst_ParseGeneratorIfStatement(ast_token,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;

  {
   if (0) { case KWD___looponce: kind = DEE_SASTKIND_LOOPONCE; }
   if (0) { case KWD___loopever: kind = DEE_SASTKIND_LOOPEVER; }
   if (0) { case KWD___loopnone: kind = DEE_SASTKIND_LOOPNONE; }
   // Generator __looponce-statement
   Dee_INCREF(ast_token = token_ob);
   if DEE_UNLIKELY(!yield()) {err_ast_token: Dee_DECREF(ast_token); return NULL; }
   ast_result = DeeSAst_ParseGeneratorSpecialLoopStatement(kind,ast_token,DEE_PARSER_ARGS);
   Dee_DECREF(ast_token);
   return ast_result;
  } break;

  default: break;
 }
 if ((yield_expression = _DeeXAst_ParseInitOrDecl(
  DEE_XAST_VARDECL_MODE_DEFAULT,DEE_XAST_INITORDECL_FLAGS_ALLOWINIT|
  DEE_XAST_INITORDECL_FLAGS_RETURNSEQ|DEE_XAST_INITORDECL_FLAGS_NOCTYPESSUFFIX,
  DEE_XASTKIND_TUPLE,NULL,NULL,DEE_PARSER_ARGS)) == NULL) return NULL;
 ast_result = DeeSAst_NewYield(yield_expression->ast_common.ast_token,
                               DeeParserLabelRefList_Empty,yield_expression);
 Dee_DECREF(yield_expression);
 return ast_result;
}


DEE_DECL_END
