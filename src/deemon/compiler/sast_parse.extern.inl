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

#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeSAst_ParseSingleExternWithBaseStatement(
 DEE_A_INOUT_OPT DeeXAstObject *extern_file, DEE_A_INOUT DeeXAstObject *extern_base,
 DEE_A_INOUT struct DeeAttributeDecl const *attr, DEE_PARSER_PARAMS) {
 DeeXAstObject *extern_type,*ast_result,*ast_extern_loader,*ast_extern_name;
 DeeTokenObject *extern_name;
 struct DeeAttributeDecl used_attr;
 _DeeAttributeDecl_InitCopy(&used_attr,attr);
 if (DeeAttributeDecl_Parse(&used_attr,DEE_PARSER_ARGS) != 0) {
err_used_attr: _DeeAttributeDecl_Quit(&used_attr); return NULL;
 }
 if ((extern_type = DeeXAst_DoParseNamedTypeSuffixWithAttribute(
  extern_base,&extern_name,NULL,&used_attr,DEE_PARSER_ARGS)
  ) == NULL) goto err_used_attr;
 if (!used_attr.a_name) {
  if (extern_name) {
   if ((used_attr.a_name = (DeeStringObject *)
    DeeToken_Str((DeeObject *)extern_name)) == NULL) {
err_extern_type_name: Dee_XDECREF(extern_name); Dee_DECREF(extern_type); goto err_used_attr;
   }
  } else {
   if (DeeError_CompilerError(DEE_WARNING_UNNAMED_EXTERN_VARIABLE_WITHOUT_ATTRIBUTE_NAME,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "Unnamed extern variable without __attribute__((name(...))) will be ignored"
    ) != 0) goto err_extern_type_name;
   ast_result = DeeXAst_NewConst(extern_type->ast_common.ast_token,Dee_None);
   Dee_DECREF(extern_type);
   _DeeAttributeDecl_Quit(&used_attr);
   return ast_result;
  }
 }
 // All we need is now parsed
 // >> Time to put it all together
 DEE_ASSERT(used_attr.a_name);
 // Create the ast containing the name of the extern object (based on attributes / the variable name)
 if ((ast_extern_name = DeeXAst_NewConst(
  extern_name ? extern_name : extern_type->ast_common.ast_token,
  (DeeObject *)used_attr.a_name)) == NULL) goto err_extern_type_name;
 // Create the extern loader AST
 ast_extern_loader = DeeXAst_NewBuiltinExtern(
  extern_name ? extern_name : extern_type->ast_common.ast_token,
  lexer,parser_flags,extern_file,ast_extern_name,extern_type);
 Dee_DECREF(ast_extern_name);
 if (!ast_extern_loader) goto err_extern_type_name;
 Dee_DECREF(extern_type);
 if (extern_name) {
  DeeLocalVarObject *extern_storage_var;
  // If the extern variable should be stored in variable, generate a vardecl AST
  if ((extern_storage_var = (DeeLocalVarObject *)_DeeScope_GetName(
   scope,extern_name->tk_token.tk_id)) != NULL) {
   if (!DeeLocalVar_Check(extern_storage_var)) {
    // Variable name is already used
    if (DeeError_CompilerErrorf(DEE_WARNING_EXTERN_VARIABLE_ALREADY_USED,
     (DeeObject *)lexer,(DeeObject *)extern_name,
     "Extern declaration %q name was already used",
     DeeString_STR(used_attr.a_name)) != 0) goto err_ast_extern_loader_name;
    goto extern_name_end;
   }
  } else {
   // Create a new local variable
#if DEE_CONFIG_XAST_CHECK_RESERVED_IDENTIFERS
   if (DeeXAst_CheckReservedToken(lexer,extern_name) != 0) goto err_ast_extern_loader_name;
#endif
   if ((extern_storage_var = (DeeLocalVarObject *)DeeLocalVar_New(
    extern_name->tk_token.tk_id,(DeeObject *)scope)) == NULL) {
err_ast_extern_loader_name: Dee_DECREF(ast_extern_loader); Dee_DECREF(extern_name); goto err_used_attr;
   }
   DeeLocalVar_ApplyAttributes(extern_storage_var,&used_attr);
   if (_DeeScope_AddDeprecatedName(scope,extern_name->tk_token.tk_id,
    (DeeObject *)extern_storage_var,(DeeObject *)used_attr.a_depr) != 0) {
    Dee_DECREF(extern_storage_var); goto err_ast_extern_loader_name;
   }
   Dee_DECREF(extern_storage_var);
  }
  // Store the declaration in a local variable
  ast_result = DeeXAst_NewStoreVar(extern_name,lexer,parser_flags,extern_storage_var,
                                   extern_name,ast_extern_loader,scope);
  Dee_DECREF(ast_extern_loader);
  Dee_INHERIT_REF(ast_extern_loader,ast_result);
extern_name_end:
  Dee_DECREF(extern_name);
 }
 _DeeAttributeDecl_Quit(&used_attr);
 return ast_extern_loader;
}



DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseExternStatement(
 DEE_A_INOUT DeeTokenObject *extern_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS) {
 DeeSAstObject *result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 if (DeeAttributeDecl_Parse(&attr,DEE_PARSER_ARGS) == 0)
  result = DeeSAst_ParseExternStatementWithAttribute(extern_token,labels_,&attr,DEE_PARSER_ARGS);
 else result = NULL;
 _DeeAttributeDecl_Quit(&attr);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseExternStatementWithAttribute(
 DEE_A_INOUT DeeTokenObject *extern_token, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeXAstObject *extern_base,*extern_file,*extern_decl;
 DeeSAstObject *ast_result;
 // Parse the extern base (for auto imports) or the extern file (for explicit imports)
 if ((extern_base = _DeeXAst_ParseInitOrDecl(
  DEE_XAST_VARDECL_FLAG_ENABLED,
  DEE_XAST_INITORDECL_FLAGS_SINGLEVAL|
  DEE_XAST_INITORDECL_FLAGS_FORCECTYPES|
  DEE_XAST_INITORDECL_FLAGS_NOCTYPESSUFFIX,
  DEE_XASTKIND_TUPLE,attr,NULL,DEE_PARSER_ARGS)
  ) == NULL) return NULL;
 if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) {
err_extern_base: Dee_DECREF(extern_base); return NULL;
 }
 if (token.tk_id == ':') {
  // What we just parsed was actually the extern-file
  if DEE_UNLIKELY(!yield()) goto err_extern_base;
  Dee_INHERIT_REF(extern_file,extern_base);
  if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) {
err_extern_file: Dee_DECREF(extern_file); return NULL;
  }
  // Now parse the extern base
  if ((extern_base = _DeeXAst_ParseInitOrDecl(
   DEE_XAST_VARDECL_FLAG_ENABLED,
   DEE_XAST_INITORDECL_FLAGS_SINGLEVAL|
   DEE_XAST_INITORDECL_FLAGS_FORCECTYPES|
   DEE_XAST_INITORDECL_FLAGS_NOCTYPESSUFFIX,
   DEE_XASTKIND_TUPLE,attr,NULL,DEE_PARSER_ARGS)
   ) == NULL) goto err_extern_file;
  if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_extern_file;
 } else extern_file = NULL;
 if (token.tk_id == ';') {
  if (DeeError_CompilerError(DEE_WARNING_EMPTY_EXTERN_DECLARATION_ONLY_BASE,
   (DeeObject *)lexer,(DeeObject *)token_ob,
   "Extern statement only contains a base (base & file will not be evaluated)") != 0) {
err_extern_base_file: Dee_DECREF(extern_base); Dee_XDECREF(extern_file); return NULL;
  }
  Dee_DECREF(extern_base);
  Dee_XDECREF(extern_file);
  return DeeSAst_NewEmpty(extern_token,labels_);
 }
 // We've parsed the base & file, now for the actual declarations
 if ((extern_decl = DeeSAst_ParseSingleExternWithBaseStatement(
  extern_file,extern_base,attr,DEE_PARSER_ARGS)) == NULL) goto err_extern_base_file;
 if (token.tk_id == ',') {
  // TODO: Parse a tuple of extern declarations
 }
 Dee_DECREF(extern_base);
 Dee_XDECREF(extern_file);
 ast_result = DeeSAst_NewExpression(extern_token,labels_,lexer,parser_flags,extern_decl);
 Dee_DECREF(extern_decl);
 return ast_result;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */


DEE_DECL_END
