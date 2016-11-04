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
#include "xast_parse.c"
#endif

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseSwitchBlock(
 DEE_A_INOUT DeeTokenObject *switch_token,
 DEE_A_IN DeeXAstObject *switch_expr, DEE_PARSER_PARAMS) {
 Dee_size_t entry_a,entry_c;
 DeeXAstObject *default_switch,*result;
 struct DeeXAstSwitchEntry *entry_v,*new_entryv;
 entry_c = 0; entry_a = 0; entry_v = 0;
 default_switch = NULL;
 while (1) {
  DeeXAstObject *case_key;
  DeeXAstObject *case_expr;
  while (token.tk_id == ';') if DEE_UNLIKELY(!yield()) goto err;
  switch (token.tk_id) {
   case KWD_case: {
    if DEE_UNLIKELY(!yield()) goto err;
parse_case:
    if DEE_UNLIKELY((case_key = DeeXAst_ParseSingle(DEE_PARSER_ARGS_EX(
     parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) goto err;
    if DEE_UNLIKELY(token.tk_id != ':') {
     if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_COLLON_AFTER_CASE_IN_EXPR_SWITCH,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Expected ':' in case expression after 'case %r'",case_key) != 0) goto err_case_key;
    } else if DEE_UNLIKELY(!yield()) goto err_case_key;
    goto parse_case_expr;
   } break;

   case KWD_default:
    if DEE_UNLIKELY(!yield()) goto err;
    if DEE_UNLIKELY(token.tk_id != ':') {
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_COLLON_AFTER_DEFAULT_IN_EXPR_SWITCH,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Expected ':' in case expression after 'default'") != 0) goto err;
    } else if DEE_UNLIKELY(!yield()) goto err;
    case_key = NULL;
parse_case_expr:
    if DEE_UNLIKELY((case_expr = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) {
err_case_key: Dee_XDECREF(case_key); goto err;
    }
    break;

   default:
    if (!DeeXAst_TokenIDIsExpressionBegin(token.tk_id,scope)) goto end;
    goto parse_case;
  }
  if (case_key) {
   if (entry_c == entry_a) {
    if (!entry_a) entry_a = 2; else entry_a *= 2;
    while DEE_UNLIKELY((new_entryv = (struct DeeXAstSwitchEntry *)realloc_nz(
     entry_v,entry_a*sizeof(struct DeeXAstSwitchEntry))) == NULL) {
     if DEE_LIKELY(Dee_CollectMemory()) continue;
     Dee_DECREF(case_expr);
     Dee_XDECREF(case_key);
     DeeError_NoMemory();
     goto err;
    }
    entry_v = new_entryv;
   }
   Dee_INHERIT_REF(entry_v[entry_c].se_key,case_key);
   Dee_INHERIT_REF(entry_v[entry_c].se_value,case_expr);
   ++entry_c;
  } else {
   if DEE_UNLIKELY(default_switch) {
    if DEE_UNLIKELY(DeeError_CompilerErrorf(
     DEE_WARNING_DEFAULT_CASE_ALREADY_DECLARED_IN_EXPR_SWITCH,
     (DeeObject *)lexer,(DeeObject *)case_expr->ast_common.ast_token,
     "The default switch was already declared\n"
     "%s(%d) : %k : See reference to previous declaration",
     DeeToken_FILE(default_switch->ast_common.ast_token),
     DeeToken_LINE(default_switch->ast_common.ast_token),
     default_switch->ast_common.ast_token) != 0)
    { Dee_DECREF(case_expr); goto err; }
    Dee_DECREF(case_expr);
   } else {
    Dee_INHERIT_REF(default_switch,case_expr);
   }
  }
  if (token.tk_id != ';' && token.tk_id != ',') break;
  if DEE_UNLIKELY(!yield()) goto err;
 }
end:
 result = DeeXAst_NewSwitchFromInheritedSwitches(switch_token,lexer,parser_flags,
                                                 switch_expr,entry_c,entry_v,default_switch);
 if (!result) goto err;
 Dee_XDECREF(default_switch);
 return result;
err:
 Dee_XDECREF(default_switch);
 while (entry_c) { --entry_c; _DeeXAstSwitchEntry_Quit(entry_v+entry_c); }
 free(entry_v);
 return NULL;
}

DEE_DECL_END
