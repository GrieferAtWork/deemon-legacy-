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

DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXAst_ParseExistsExpressionUnary(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_PARSER_PARAMS) {
 int result; struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 result = DeeXAst_ParseExistsExpressionUnaryWithAttribute(vardecl_mode,&attr,DEE_PARSER_ARGS);
 _DeeAttributeDecl_Quit(&attr);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXAst_ParseExistsExpressionUnaryWithAttribute(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_IN struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DEE_A_REF DeeObject *found_object;
 int maybe_exists = 0;
 // TODO: Special handling for:
 //     - __builtin_exists(if (x) y else z) >> __builtin_exists(x) ? __builtin_exists(y) : __builtin_exists(z)
again: switch (token.tk_id) {
  
  DEE_ATTRIBUTEDECL_TOKENS_NOUNARY {
   Dee_uint64_t old_tokenpos = lexer->l_token_pos;
   if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) return -1;
   if (lexer->l_token_pos != old_tokenpos) goto again;
   goto unexpected_token;
  } break;

  DEE_XAST_VARIBLE_LOOKUP_MODIFER_TOKENS
   if (DeeXAst_ParseVariableLookupModifiers(&vardecl_mode,attr,DEE_PARSER_ARGS) != 0) return -1;
   goto again;

  // Stuff we simply use the normal expression parser for
  DEE_XAST_VARIABLE_DECL_TOKENS
  DEE_XAST_UNARY_TOKENS {
   DeeXAstObject *temp_ast;
   if ((temp_ast = DeeXAst_ParseUnaryEx(
    DEE_XAST_UNARAYSUFFIX_FLAG_NONE,vardecl_mode,attr,DEE_PARSER_ARGS_EX(
    parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) return -1;
   if (temp_ast->ast_kind == DEE_XASTKIND_CONST) {
    Dee_INCREF(found_object = temp_ast->ast_const.c_const);
   } else found_object = NULL,maybe_exists = 1;
   Dee_DECREF(temp_ast);
  } break;

  case KWD___nth: {
   Dee_size_t nth_id;
   if (DeeXAst_ParseVariableLookupNTh(&nth_id,DEE_PARSER_ARGS) != 0) return -1;
   if (DeeXAst_ParseVariableLookupModifiers(&vardecl_mode,attr,DEE_PARSER_ARGS) != 0) return -1;
   while (1) {
    if ((found_object = _DeeScope_GetName(scope,token.tk_id)) != NULL)
    { if (!nth_id) break; found_object = NULL; --nth_id; }
    if (scope->sc_weak_prev) scope = scope->sc_weak_prev;
    else if (scope->sc_prev) scope = scope->sc_prev;
    else break;
   }
   goto yield_and_xincref_found_object;
  } break;

  case KWD___scope: {
   Dee_size_t scope_uprefs;
   if (DeeXAst_ParseVariableLookupScope(&scope_uprefs,DEE_PARSER_ARGS) != 0) return -1;
   if (DeeXAst_ParseVariableLookupModifiers(&vardecl_mode,attr,DEE_PARSER_ARGS) != 0) return -1;
   while (scope_uprefs--) {
    if (scope->sc_weak_prev) scope = scope->sc_weak_prev;
    else if (scope->sc_prev) scope = scope->sc_prev;
    else break;
   }
   found_object = _DeeScope_GetName(scope,token.tk_id);
   goto yield_and_xincref_found_object;
  } break;


  default:
   if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
  case TPP_TOK_DOTS:
    switch (DEE_XAST_VARDECL_MODE(vardecl_mode)) {
     case DEE_XAST_VARDECL_MODE_LOCAL:
      found_object = _DeeScope_GetName(scope,token.tk_id);
      break;
     case DEE_XAST_VARDECL_MODE_GLOBAL:
      found_object = _DeeScope_GetName((DeeScopeObject *)DeeScope_Global((DeeObject *)(scope)),token.tk_id);
      break;
     default: while (1) {
      if ((found_object = _DeeScope_GetName(scope,token.tk_id)) != NULL) break;
      if (scope->sc_weak_prev) scope = scope->sc_weak_prev;
      else if (scope->sc_prev) scope = scope->sc_prev;
      else break;
     } break;
    }
yield_and_xincref_found_object:
    if DEE_UNLIKELY(!yield()) return -1;
    Dee_XINCREF(found_object);
   } else {
unexpected_token:
    if (DeeError_CompilerError(DEE_WARNING_UNEXPECTED_TOKEN_IN_EXISTS_EXPRESSION,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Unexpected token in __builtin_exists expression") != 0) return -1;
    return 0;
   }
   break;
 }
 while (1) switch (token.tk_id) {
  case '.':
  case TPP_TOK_COLLON_COLLON:
   if DEE_UNLIKELY(!yield()) goto err_found;
   if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_DOT_IN_EXISTS_EXPRESSION,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Unexpected keyword after '.' in exists-expression") == -1) return -1;
   } else {
    if (found_object) {
     DeeObject *found_object_attr;
     if (DeeScope_Check(found_object)) {
      found_object_attr = _DeeScope_GetName((DeeScopeObject *)found_object,token.tk_id);
      Dee_DECREF(found_object);
      Dee_XINCREF(found_object_attr);
      Dee_INHERIT_REF(found_object,found_object_attr);
     } else {
      found_object_attr = DeeObject_GetAttrConstString(
       found_object,TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),token.tk_id));
      if (!found_object_attr) DeeError_Handled();
      Dee_DECREF(found_object);
      Dee_INHERIT_REF(found_object,found_object_attr);
     }
    }
    if DEE_UNLIKELY(!yield()) goto err_found;
   }
   break;

  default: goto end;
 }
end: if (found_object) { Dee_DECREF(found_object); return 1; }
 return maybe_exists;
err_found: Dee_XDECREF(found_object); return -1;
}


DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXAst_ParseExistsExpressionLand(DEE_PARSER_PARAMS) {
 int result,result2;
 if ((result = DeeXAst_ParseExistsExpressionUnary(
  DEE_XAST_VARDECL_MODE_DEFAULT,DEE_PARSER_ARGS)) < 0) return result;
 while (token.tk_id == TPP_TOK_LAND) {
  if DEE_UNLIKELY(!yield()) return -1;
  if ((result2 = DeeXAst_ParseExistsExpressionUnary(
   DEE_XAST_VARDECL_MODE_DEFAULT,DEE_PARSER_ARGS)) < 0) return result2;
  if (!result2) result = 0;
 }
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXAst_ParseExistsExpressionLxor(DEE_PARSER_PARAMS) {
 int result,result2;
 if ((result = DeeXAst_ParseExistsExpressionLand(DEE_PARSER_ARGS)) < 0) return result;
 while (token.tk_id == TPP_TOK_LXOR) {
  if DEE_UNLIKELY(!yield()) return -1;
  if ((result2 = DeeXAst_ParseExistsExpressionLand(DEE_PARSER_ARGS)) < 0) return result2;
  result = !!result ^ !!result2;
 }
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXAst_ParseExistsExpressionLor(DEE_PARSER_PARAMS) {
 int result,result2;
 if ((result = DeeXAst_ParseExistsExpressionLxor(DEE_PARSER_ARGS)) < 0) return result;
 while (token.tk_id == TPP_TOK_LOR) {
  if DEE_UNLIKELY(!yield()) return -1;
  if ((result2 = DeeXAst_ParseExistsExpressionLxor(DEE_PARSER_ARGS)) < 0) return result2;
  if (result2) result = 1;
 }
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXAst_ParseExistsExpressionIf(DEE_PARSER_PARAMS) {
 int result,val_succ,val_fail;
 if ((result = DeeXAst_ParseExistsExpressionLor(DEE_PARSER_ARGS)) < 0) return result;
 while (token.tk_id == '?') {
  if DEE_UNLIKELY(!yield()) return -1;
  if (token.tk_id == ':') val_succ = result;
  else if ((val_succ = DeeXAst_ParseExistsExpression(DEE_PARSER_ARGS)) < 0) return val_succ;
  if (token.tk_id != ':') val_fail = 0; else {
   if DEE_UNLIKELY(!yield()) return -1;
   if ((val_fail = DeeXAst_ParseExistsExpressionIf(DEE_PARSER_ARGS)) < 0) return val_fail;
  }
  result = result ? val_succ : val_fail;
 }
 return result;
}



DEE_DECL_END
