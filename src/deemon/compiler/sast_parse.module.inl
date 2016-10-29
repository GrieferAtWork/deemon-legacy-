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

DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseModuleStatement(
 DEE_A_INOUT DeeTokenObject *module_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS) {
 DeeSAstObject *result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 if (DeeAttributeDecl_Parse(&attr,DEE_PARSER_ARGS) == 0)
  result = DeeSAst_ParseModuleStatementWithAttribute(module_token,labels_,&attr,DEE_PARSER_ARGS);
 else result = NULL;
 _DeeAttributeDecl_Quit(&attr);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseModuleStatementWithAttribute(
 DEE_A_INOUT DeeTokenObject *module_token, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeScopeObject *module_scope,*scope_iter;
 DeeTokenObject *module_name;
 DeeSAstObject *ast_result,*module_block;
 if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_MODULE,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected a keyword after 'module'"
   ) != 0) return NULL;
  return DeeSAst_Parse(DEE_PARSER_ARGS);
 }
 scope_iter = scope;
 while (1) {
  if ((module_scope = (DeeScopeObject *)_DeeScope_GetName(scope_iter,token.tk_id)) != NULL) {
   if (!DeeScope_Check(module_scope)) {
    if (DeeError_CompilerError(DEE_WARNING_MODULE_NAME_ALREADY_USED,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Module name is already used"
     ) != 0) return NULL;
    return DeeSAst_Parse(DEE_PARSER_ARGS);
   }
   if DEE_UNLIKELY(!yield()) return NULL;
   Dee_INCREF(module_scope);
   goto module_found_scope;
  }
  if (scope_iter->sc_weak_prev) scope_iter = scope_iter->sc_weak_prev;
//else if (scope_iter->ob_prev) scope_iter = scope_iter->ob_prev;
  else break;
 }
 Dee_INCREF(module_name = token_ob);
 if DEE_UNLIKELY(!yield()) {err_module_name: Dee_DECREF(module_name); return NULL; }
 if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_module_name;
#if DEE_CONFIG_XAST_CHECK_RESERVED_IDENTIFERS
 if (DeeXAst_CheckReservedToken(lexer,module_name) != 0) goto err_module_name;
#endif
 if ((module_scope = (DeeScopeObject *)DeeScope_New((
  DeeObject *)scope,1)) == NULL) goto err_module_name;
 if (_DeeScope_AddDeprecatedName(scope,module_name->tk_token.tk_id,
  (DeeObject *)module_scope,(DeeObject *)attr->a_depr) != 0) {
  Dee_DECREF(module_name); goto err_module_scope;
 }
 Dee_DECREF(module_name);
module_found_scope:
 if (token.tk_id == '.' || token.tk_id == TPP_TOK_COLLON_COLLON)
  token.tk_id = KWD_module; // Little hack to allow 'module x::y { ... }'
 if (token.tk_id == '{') {
  if DEE_UNLIKELY(!yield()) {
err_module_scope: Dee_DECREF(module_scope); return NULL;
 }
  module_block = DeeSAst_ParseAllInOldScopeUntil(
   module_token,'}',DeeParserLabelRefList_Empty,DEE_PARSER_ARGS_SCOPE(module_scope));
 } else module_block = DeeSAst_Parse(DEE_PARSER_ARGS_SCOPE(module_scope));
 if (!module_block) goto err_module_scope;
 ast_result = DeeSAst_NewModule(module_token,labels_,module_scope,module_block);
 Dee_DECREF(module_block);
 Dee_DECREF(module_scope);
 return ast_result;
}

DEE_DECL_END
