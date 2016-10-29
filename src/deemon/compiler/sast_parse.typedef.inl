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

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseSingleTypedefWithBaseStatement(
 DEE_A_INOUT DeeXAstObject *typedef_base,
 DEE_A_IN struct DeeAttributeDecl const *attr, DEE_PARSER_PARAMS) {
 DeeTokenObject *typedef_name; DeeXAstObject *typedef_ast;
 struct DeeAttributeDecl single_typedef_attr;
 _DeeAttributeDecl_InitCopy(&single_typedef_attr,attr);
 // Now parse as my suffixes as there are
 if (DeeAttributeDecl_Parse(&single_typedef_attr,DEE_PARSER_ARGS) != 0) {
err_typedef_attr: _DeeAttributeDecl_Quit(&single_typedef_attr);
  return NULL;
 }
 if ((typedef_ast = DeeXAst_DoParseNamedTypeSuffixWithAttribute(
  typedef_base,&typedef_name,NULL,&single_typedef_attr,DEE_PARSER_ARGS_EX(
  parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) goto err_typedef_attr;
 if (typedef_ast->ast_kind != DEE_XASTKIND_CONST) {
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_TYPEDEF,
   (DeeObject *)lexer,(DeeObject *)(typedef_name ? typedef_name : typedef_ast->ast_common.ast_token),
   "Expected constant expression after 'typedef', but got %r",typedef_ast) != 0) {
err_typedef_ast: Dee_DECREF(typedef_ast); Dee_XDECREF(typedef_name); goto err_typedef_attr;
  }
 }
 if (!typedef_name) {
  if (DeeError_CompilerErrorf(DEE_WARNING_UNNAMED_TYPEDEF_CONSTANT,
   (DeeObject *)lexer,(DeeObject *)typedef_ast->ast_common.ast_token,
   "Unnamed type declaration in 'typedef': %r",typedef_ast) != 0) goto err_typedef_ast;
 } else if (typedef_ast->ast_kind == DEE_XASTKIND_CONST) {
  TPPTokenID typedef_name_id = typedef_name->tk_token.tk_id;
  DeeObject *old_typedef_value,*new_typedef_value;
  new_typedef_value = typedef_ast->ast_const.c_const;
  // Make sure any re-definition is identical to the previous
  if ((old_typedef_value = _DeeScope_GetName(scope,typedef_name_id)) != NULL) {
   int temp; // v very likely case for basic type redefinitions
   if (old_typedef_value != new_typedef_value) {
    if ((old_typedef_value = DeeObject_DeepCopy(old_typedef_value)) == NULL) { DeeError_Handled(); temp = 0; }
    else if ((new_typedef_value = DeeObject_DeepCopy(new_typedef_value)) == NULL) { Dee_DECREF(old_typedef_value); DeeError_Handled(); temp = 0; }
    else {
     temp = DeeObject_DeepEquals(old_typedef_value,new_typedef_value);
     Dee_DECREF(new_typedef_value);
     Dee_DECREF(old_typedef_value);
     if (temp < 0) { DeeError_Handled(); temp = 0; }
    }
    if (!temp && DeeError_CompilerErrorf(DEE_WARNING_TYPEDEF_TYPENAME_ALREADY_USED,
     (DeeObject *)lexer,(DeeObject *)typedef_name,
     "Typename used in 'typedef' is already used by '%r'",
     old_typedef_value) != 0) goto err_typedef_ast;
    // For safety, we overwrite the constant value of our ast
    // >> This was we reduce the number for different, but technically identical types
    DEE_ASSERT(typedef_ast->ast_const.c_const == new_typedef_value);
    Dee_INCREF(typedef_ast->ast_const.c_const = old_typedef_value);
    Dee_DECREF(new_typedef_value);
   }
  } else {
#if DEE_CONFIG_XAST_CHECK_RESERVED_IDENTIFERS
   if (DeeXAst_CheckReservedToken(lexer,typedef_name) != 0) goto err_typedef_ast;
#endif
   if (_DeeScope_AddDeprecatedName(scope,typedef_name_id,
    new_typedef_value,(DeeObject *)single_typedef_attr.a_depr
    ) != 0) goto err_typedef_ast;
  }
 }
 Dee_XDECREF(typedef_name);
 return typedef_ast;
}

DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_ParseTypedefStatement(
 DEE_A_INOUT DeeTokenObject *typedef_token,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_PARSER_PARAMS) {
 DeeSAstObject *result;
 DeeXAstObject *typedef_base,*typedef_ast,*ast_result;
 struct DeeAttributeDecl typedef_attr = DeeAttributeDecl_INIT();
 if (DeeAttributeDecl_Parse(&typedef_attr,DEE_PARSER_ARGS) != 0) {
err_typedef_attr: _DeeAttributeDecl_Quit(&typedef_attr); return NULL;
 }
 // Parse the typedef base
 if ((typedef_base = _DeeXAst_ParseInitOrDecl(
  DEE_XAST_VARDECL_FLAG_ENABLED,
  DEE_XAST_INITORDECL_FLAGS_SINGLEVAL|
  DEE_XAST_INITORDECL_FLAGS_FORCECTYPES|
  DEE_XAST_INITORDECL_FLAGS_NOCTYPESSUFFIX,
  DEE_XASTKIND_TUPLE,&typedef_attr,NULL,DEE_PARSER_ARGS_EX(
  parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))
  ) == NULL) goto err_typedef_attr;
 if (token.tk_id == ';') {
  if (DeeError_CompilerErrorf(DEE_WARNING_EMPTY_TYPEDEF_ONLYBASE_DECLARATION,
   (DeeObject *)lexer,(DeeObject *)typedef_base->ast_common.ast_token,
   "Typedef declaration only contains a base: %r",typedef_base) != 0) goto err_typedef_base;
  result = DeeSAst_NewExpression(typedef_token,labels_,lexer,parser_flags,typedef_base);
  Dee_DECREF(typedef_base);
  return result;
 }
 if ((typedef_ast = DeeXAst_ParseSingleTypedefWithBaseStatement(
  typedef_base,&typedef_attr,DEE_PARSER_ARGS_EX(
  parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))) == NULL) {
err_typedef_base: Dee_DECREF(typedef_base); goto err_typedef_attr;
 }
 if (token.tk_id == ',') {
  if DEE_UNLIKELY(!yield()) {err_typedef_ast: Dee_DECREF(typedef_ast); goto err_typedef_base; }
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
  if (DeeXAst_TokenIDIsCTypeSuffixBegin(token.tk_id))
#endif
  {// Parse ',' - separated list of typedefs
   Dee_size_t      typedef_a = 2;
   Dee_size_t      typedef_c = 1;
   DeeXAstObject **typedef_v,**new_typedef_v;
   while ((typedef_v = (DeeXAstObject **)malloc_nz(
    2*sizeof(DeeXAstObject *))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
    goto err_typedef_ast;
   }
   Dee_INHERIT_REF(typedef_v[0],typedef_ast);
   while (1) {
    if ((typedef_ast = DeeXAst_ParseSingleTypedefWithBaseStatement(
     typedef_base,&typedef_attr,DEE_PARSER_ARGS)) == NULL) {
err_typedefv: new_typedef_v = typedef_v+typedef_c;
     while (new_typedef_v != typedef_v) Dee_DECREF(*--new_typedef_v);
     free(typedef_v);
     goto err_typedef_base;
    }
    if (typedef_c == typedef_a) {
     DEE_ASSERT(typedef_a); typedef_a *= 2;
     while ((new_typedef_v = (DeeXAstObject **)realloc_nnz(
      typedef_v,typedef_a*sizeof(DeeXAstObject *))) == NULL) {
      if DEE_LIKELY(Dee_CollectMemory()) continue;
      Dee_DECREF(typedef_ast);
      DeeError_NoMemory();
      goto err_typedefv;
     }
     typedef_v = new_typedef_v;
    }
    Dee_INHERIT_REF(typedef_v[typedef_c++],typedef_ast);
    if (token.tk_id != ',') break;
    if DEE_UNLIKELY(!yield()) goto err_typedefv;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
    if (!DeeXAst_TokenIDIsCTypeSuffixBegin(token.tk_id)) break;
#endif
   }
   // try to conserve some memory
   if (typedef_c != typedef_c && (new_typedef_v = (DeeXAstObject **)realloc_nnz(
    typedef_v,typedef_c*sizeof(DeeXAstObject *))) != NULL) typedef_v = new_typedef_v;
   ast_result = DeeXAst_NewSequenceFromInheritedAstv(DEE_XASTKIND_TUPLE,typedef_token,lexer,
                                                     parser_flags,typedef_c,typedef_v);
   if (!ast_result) goto err_typedefv;
   Dee_INHERIT_REF(typedef_ast,ast_result);
  }
  // fallthrough to the code below
  DEE_ATTRIBUTE_FALLTHROUGH
 }
 Dee_DECREF(typedef_base);
 _DeeAttributeDecl_Quit(&typedef_attr);
 result = DeeSAst_NewExpression(typedef_token,labels_,lexer,parser_flags,typedef_ast);
 Dee_DECREF(typedef_ast);
 return result;
}

DEE_DECL_END
