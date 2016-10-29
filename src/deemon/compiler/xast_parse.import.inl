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


DEE_A_RET_EXCEPT_REF DeeXAstObject *
DeeXAst_ParseImport(DEE_PARSER_PARAMS) {
 DeeXAstObject *result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 if (DeeAttributeDecl_Parse(&attr,DEE_PARSER_ARGS) == 0)
  result = DeeXAst_ParseImportWithAttribute(&attr,DEE_PARSER_ARGS);
 else result = NULL;
 _DeeAttributeDecl_Quit(&attr);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseImportWithAttribute(
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 int import_all;
 DeeObject *deprecated_reason;
 DeeTokenObject *import_name_token;
 DeeObject *import_value,*new_import_value;
 DeeXAstObject *import_source,*ast_result;
 if (token.tk_id == KWD_module) {
  if DEE_UNLIKELY(!yield()) return NULL;
  if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) return NULL;
  import_all = 1;
 } else import_all = 0;
 if ((import_source = DeeXAst_ParseVariableLookup(
  DEE_XAST_VARDECL_MODE_DEFAULT,attr,DEE_PARSER_ARGS)
  ) == NULL) return NULL;
 if (import_source->ast_kind == DEE_XASTKIND_MODULE) {
  Dee_INCREF(import_name_token = import_source->ast_common.ast_token);
  Dee_INCREF(import_value = (DeeObject *)import_source->ast_module.m_module);
 } else {
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_MODULE_AFTER_IMPORT,
   (DeeObject *)lexer,(DeeObject *)token_ob,
   "The a module after 'import%s', but got %r",
   import_all ? " module" : "",import_source) != 0) {
/*err_import_source:*/ Dee_DECREF(import_source); return NULL;
  }
  import_name_token = NULL;
  Dee_INCREF(import_value = Dee_None);
 }
 Dee_DECREF(import_source);
 if (token.tk_id != '.' && token.tk_id != TPP_TOK_COLLON_COLLON) import_all = 1;
 else do {
  if DEE_UNLIKELY(!yield()) {
err_import_value:
   Dee_DECREF(import_value);
   Dee_XDECREF(import_name_token);
   return NULL;
  }
  if (token.tk_id == '*') {
   if (import_all) {
    if (DeeError_CompilerError(DEE_WARNING_IMPORT_START_AFTER_IMPORT_MODULE,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Import * after 'import module' does not make any sense"
     ) != 0) goto err_import_value;
   }
   if DEE_UNLIKELY(!yield()) goto err_import_value;
   import_all = 1;
   break;
  }
  if (!DeeScope_Check(import_value)) {
   if DEE_UNLIKELY(!yield()) goto err_import_value;
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_MODULE_AFTER_IMPORT,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "Expected a module to import from, but got an instance of %q",
    DeeType_NAME(Dee_TYPE(import_value))) != 0) goto err_import_value;
  } else {
   Dee_XDECREF(import_name_token);
   Dee_INCREF(import_name_token = token_ob);
   if ((new_import_value = _DeeScope_GetNameEx(
    (DeeScopeObject *)import_value,token.tk_id,
    &deprecated_reason)) == NULL) {
    if (DeeError_CompilerErrorf(DEE_WARNING_IMPORT_OBJECT_NOT_FOUND_IN_MODULE,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Import failed to find a variable %q in the module path",
     TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),token.tk_id)
     ) != 0) goto err_import_value;
   } else {
    if (deprecated_reason && DeeError_CompilerErrorf(DEE_WARNING_DEPRECATED_USAGE,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Deprecated : %r",deprecated_reason
     ) != 0) goto err_import_value;
    Dee_INCREF(new_import_value);
    Dee_DECREF(import_value);
    import_value = new_import_value;
   }
  }
  if DEE_UNLIKELY(!yield()) goto err_import_value;
 } while (token.tk_id == '.' || token.tk_id == TPP_TOK_COLLON_COLLON);
 if (import_all) {
  if (!DeeScope_Check(import_value)) {
   if (DeeError_CompilerErrorf(DEE_WARNING_IMPORT_ALL_EXPECTED_MODULE,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "Expected a module after 'import module', but got an instance of %q",
    DeeType_NAME(Dee_TYPE(import_value))) != 0) goto err_import_value;
  } else {
   if (DeeScope_ImportScope((DeeObject *)scope,import_value,
    (DeeObject *)lexer,(DeeObject *)(import_name_token
    ? import_name_token : token_ob),(DeeObject *)attr->a_depr) != 0) goto err_import_value;
  }
  ast_result = DeeXAst_NewConst(import_name_token ? import_name_token : token_ob,Dee_None);
 } else if (import_name_token) {
  if (DeeScope_ImportSingle((DeeObject *)scope,import_name_token->tk_token.tk_id,
   import_value,(DeeObject *)lexer,(DeeObject *)import_name_token,(DeeObject *)attr->a_depr) != 0) goto err_import_value;
  ast_result = DeeXAst_NewConst(import_name_token,import_value);
 } else ast_result = DeeXAst_NewConst(token_ob,import_value);
 Dee_DECREF(import_value);
 Dee_XDECREF(import_name_token);
 return ast_result;
}


DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseAllImports(
 DEE_A_IN DeeTokenObject *import_token, DEE_PARSER_PARAMS) {
 DeeXAstObject  *import_ast,*ast_result;
 Dee_size_t      import_asta;
 Dee_size_t      import_astc;
 DeeXAstObject **import_astv,**new_import_astv;
 int should_end;
 if ((import_ast = DeeXAst_ParseImport(DEE_PARSER_ARGS)) == NULL) return NULL;
 if (token.tk_id != ',') return import_ast;
 if DEE_UNLIKELY(!yield()) {/*err_import_ast:*/ Dee_DECREF(import_ast); return NULL; }
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
 if (!DeeXAst_TokenIDIsExpressionBegin(token.tk_id,scope)) return import_ast;
#endif
 import_asta = 0;
 import_astc = 0;
 import_astv = NULL;
 should_end = 0;
 goto seq_begin;
 while (1) {
  if (token.tk_id != ',') should_end = 1; else {
   if DEE_UNLIKELY(!yield()) goto err_import_ast_v;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (!DeeXAst_TokenIDIsExpressionBegin(token.tk_id,scope)) should_end = 1;
#endif
  }
seq_begin:
  if (import_astc == import_asta) {
   if (!import_asta) import_asta = 2; else import_asta *= 2;
   while ((new_import_astv = (DeeXAstObject **)realloc_nz(
    import_astv,import_asta*sizeof(DeeXAstObject *))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    Dee_DECREF(import_ast);
    DeeError_NoMemory();
err_import_ast_v:
    new_import_astv = import_astv+import_astc;
    while (new_import_astv != import_astv) Dee_DECREF(*--new_import_astv);
    free(import_astv);
    return NULL;
   }
   import_astv = new_import_astv;
  }
  Dee_INHERIT_REF(import_astv[import_astc++],import_ast);
  if (should_end) break;
  if ((import_ast = DeeXAst_ParseImport(DEE_PARSER_ARGS)) == NULL) goto err_import_ast_v;
 }
 if (import_astc != import_asta) {
  // Try to conserve memory
  if ((new_import_astv = (DeeXAstObject **)realloc_nnz(
   import_astv,import_astc*sizeof(DeeXAstObject *))
   ) != NULL) import_astv = new_import_astv;
 }
 ast_result = DeeXAst_NewSequenceFromInheritedAstv(DEE_XASTKIND_TUPLE,import_token,
                                                   lexer,parser_flags,import_astc,import_astv);
 if (!ast_result) {
  new_import_astv = import_astv+import_astc;
  while (new_import_astv != import_astv) Dee_DECREF(*--new_import_astv);
  free(import_astv);
 }
 return ast_result;
}


DEE_DECL_END
