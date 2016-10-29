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


DEE_A_RET_EXCEPT(-1) int DeeSAst_ParseDefaultLabel(
 DEE_A_INOUT DeeTokenObject *default_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS) {
 DeeScopeObject *weak_root;
 struct DeeParserLabel *default_label;
 (void)DEE_PARSER_ARGS;
 weak_root = DeeScope_WEAK_ROOT(scope);
 if (!weak_root->sc_cases) {
  if (DeeError_CompilerError(DEE_WARNING_DEFAULT_ONLY_ALLOWED_IN_SWITCH_STATEMENTS,
   (DeeObject *)lexer,(DeeObject *)token_ob,
   "'default' labels are only allowed in switch statements") != 0) return -1;
 } else {
  if ((default_label = DeeParserLabelList_GetDefaultCase(weak_root->sc_cases)) != NULL) {
   DEE_ASSERT(DeeObject_Check(default_label->pl_dest_token) && DeeToken_Check(default_label->pl_dest_token));
   if (DeeError_CompilerErrorf(DEE_WARNING_DEFAULT_LABEL_ALREADY_USED,
    (DeeObject *)lexer,(DeeObject *)default_token,
    "'default' label was already declaration\n"
    "%s(%d) : See reference to previous declaration",
    DeeToken_FILE(default_label->pl_dest_token),
    DeeToken_LINE(default_label->pl_dest_token+1)) != 0) return -1;
  } else { // Add the case label to the expression
   if ((default_label = DeeParserLabelList_AddDefaultCase(
    weak_root->sc_cases,default_token)) == NULL) return -1;
   if (DeeParserLabelRefList_Append(labels_,default_label) != 0) return -1;
  }
 }
 if (token.tk_id != ':') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_COLLON_AFTER_DEFAULT,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ':' after 'default'") != 0) return -1;
 } else if DEE_UNLIKELY(!yield()) return -1;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeSAst_ParseCaseLabel(
 DEE_A_INOUT DeeTokenObject *case_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_PARSER_PARAMS) {
 DeeObject *case_lo,*case_hi;
 DeeXAstObject *case_ast;
 DeeScopeObject *weak_root;
 if ((case_ast = DeeXAst_ParseUnaryEx(
  DEE_XAST_UNARAYSUFFIX_FLAG_NOEXPAND,
  DEE_XAST_VARDECL_MODE_DEFAULT,NULL,DEE_PARSER_ARGS)) == NULL) return -1;
 if (case_ast->ast_kind != DEE_XASTKIND_CONST) {
/*err_case_not_const:*/
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_CASE,
   (DeeObject *)lexer,(DeeObject *)case_ast->ast_common.ast_token,
   "Expected constant expression after 'case'") != 0) {
err_case_ast: Dee_DECREF(case_ast); return -1;
  }
  Dee_INCREF(case_lo = Dee_None);
 } else Dee_INCREF(case_lo = case_ast->ast_const.c_const);
 Dee_DECREF(case_ast);

 if (token.tk_id == TPP_TOK_DOTS) {
  // Case range
  if DEE_UNLIKELY(!yield()) {err_case_lo: Dee_DECREF(case_lo); return -1; }
  if ((case_ast = DeeXAst_ParseUnaryEx(
   DEE_XAST_UNARAYSUFFIX_FLAG_NOEXPAND,
   DEE_XAST_VARDECL_MODE_DEFAULT,NULL,DEE_PARSER_ARGS)) == NULL) goto err_case_lo;
  if (case_ast->ast_kind != DEE_XASTKIND_CONST) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_CASE,
    (DeeObject *)lexer,(DeeObject *)case_ast->ast_common.ast_token,
    "Expected constant expression after 'case %r ...'",case_lo) != 0) {
/*err_case_lo_case_ast:*/ Dee_DECREF(case_lo); goto err_case_ast;
   }
   Dee_INCREF(case_hi = Dee_None);
  } else {
   Dee_INCREF(case_hi = case_ast->ast_const.c_const);
  }
  Dee_DECREF(case_ast);
 } else case_hi = NULL;

 // Done parsing case / case range
 if (token.tk_id != ':') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_COLLON_AFTER_CASE_EXPR,
   (DeeObject *)lexer,(DeeObject *)token_ob,
   "Expected ':' after 'case ...'") != 0) {
err_case_lohi: Dee_XDECREF(case_hi); goto err_case_lo;
  }
 } else if DEE_UNLIKELY(!yield()) goto err_case_lohi;
 weak_root = DeeScope_WEAK_ROOT(scope);
 if (weak_root->sc_cases) {
  struct DeeParserLabel *case_label;
  if (case_hi) {
   if ((case_label = DeeParserLabelList_GetCaseRange(weak_root->sc_cases,case_lo,case_hi)) != NULL) {
    DEE_ASSERT(DeeObject_Check(case_label->pl_dest_token) && DeeToken_Check(case_label->pl_dest_token));
    if (DeeError_CompilerErrorf(DEE_WARNING_CASE_LABEL_ALREADY_USED,
     (DeeObject *)lexer,(DeeObject *)case_token,
     "'case (%r) ... (%r):' label already used\n"
     "%s(%d) : %k : See reference to previous declaration",
     case_lo,case_hi,DeeToken_FILE(case_label->pl_dest_token),
     DeeToken_LINE(case_label->pl_dest_token)+1,
     case_label->pl_dest_token) != 0) goto err_case_lohi;
    goto case_end;
   }
   case_label = DeeParserLabelList_AddCaseRange(weak_root->sc_cases,case_token,case_lo,case_hi);
  } else {
   if ((case_label = DeeParserLabelList_GetCase(weak_root->sc_cases,case_lo)) != NULL) {
    if (DeeError_CompilerErrorf(DEE_WARNING_CASE_LABEL_ALREADY_USED,
     (DeeObject *)lexer,(DeeObject *)case_token,
     "'case %r:' label already used\n"
     "%s(%d) : %k : See reference to previous declaration",
     case_lo,DeeToken_FILE(case_label->pl_dest_token),
     DeeToken_LINE(case_label->pl_dest_token)+1,
     case_label->pl_dest_token) != 0) goto err_case_lohi;
    goto case_end;
   }
   case_label = DeeParserLabelList_AddCase(weak_root->sc_cases,case_token,case_lo);
  }
  // Add the case label to the expression
  if (!case_label) goto err_case_lohi;
  if (DeeParserLabelRefList_Append(labels_,case_label) != 0) goto err_case_lohi;
 } else {
  if (DeeError_CompilerError(DEE_WARNING_CASE_ONLY_ALLOWED_IN_SWITCH_STATEMENTS,
   (DeeObject *)lexer,(DeeObject *)case_token,
   "'case' is only allowed in switch statements") != 0) goto err_case_lohi;
 }
case_end:
 Dee_XDECREF(case_hi);
 Dee_DECREF(case_lo);
 return 0;
}

DEE_DECL_END
