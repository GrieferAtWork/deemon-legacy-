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

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeScope_InitializeAttributes(
 DEE_A_INOUT DeeScopeObject *self, DEE_A_IN struct DeeAttributeDecl const *attr) {
 DEE_ASSERT(DeeObject_Check(self) && DeeScope_Check(self));
 DEE_ASSERT(attr);
 // Set the copyable bit
 if ((attr->a_flags&DEE_ATTRIBUTE_DECL_FLAG_COPYABLE)!=0)
  self->sc_flags |= DEE_SCOPE_FLAG_CODE_COPYABLE;

 // Create the this-variable in [[thiscall]] functions
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
 if (DeeAttributeDecl_GET_CALLING_CONVENTION(attr) == DEE_FUNCTIONFLAGS_FLAG_THISCALL &&
     (self->sc_flags&DEE_SCOPE_FLAG_FOUND_THIS) == 0) {
  DeeLocalVarObject *this_var;
  self->sc_flags |= DEE_SCOPE_FLAG_FOUND_THIS;
  if DEE_UNLIKELY((this_var = (DeeLocalVarObject *)DeeLocalVar_New(KWD_this,(DeeObject *)self)) == NULL) return -1;
  this_var->lv_flags = (DEE_LOCALVAR_KIND_THIS|DEE_LOCALVAR_FLAG_INITIALIZED);
  ++this_var->lv_init; // The this-variable is always initialized
  if DEE_UNLIKELY((_DeeScope_AddName(self,KWD_this,(DeeObject *)this_var)) != 0) { Dee_DECREF(this_var); return -1; }
  Dee_DECREF(this_var);
 }

 // Setup the super-type attribute
 if (attr->a_super && self->sc_supertp != attr->a_super) {
  Dee_XINCREF(self->sc_supertp);
  Dee_INCREF(self->sc_supertp = attr->a_super);
 }

 return 0;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseUnnamedFunction(
 DEE_A_INOUT DeeTokenObject *function_token, DEE_A_INOUT struct DeeAttributeDecl *attr,
 DEE_A_OUT_OPT Dee_uint32_t *syntax_mode, DEE_PARSER_PARAMS) {
 Dee_size_t argc; struct DeeXAstFunctionAstArgument *argv,*iter;
 DeeScopeObject *function_scope; DeeSAstObject *function_block;
 DeeTokenObject *block_token; DeeXAstObject *ast_result;
 TPPTokenID old_function_name; Dee_uint32_t function_flags;
 DEE_ASSERT(!attr->a_name || (DeeObject_Check(attr->a_name) && DeeString_Check(attr->a_name)));
 DEE_ASSERT(attr);
 if DEE_UNLIKELY(DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) return NULL;
 if DEE_UNLIKELY((function_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,0)) == NULL) return NULL;
 function_flags = 0;
 // Initialize scope attributes
 if DEE_UNLIKELY(_DeeScope_InitializeAttributes(function_scope,attr) != 0) goto err_scope;
 
 // Check for a parameter list
 if (token.tk_id == '(') {
  if DEE_UNLIKELY(!yield()) {err_scope: Dee_DECREF(function_scope); return NULL; }
  if (token.tk_id == ')') { if DEE_UNLIKELY(!yield()) goto err_scope; goto noargs; }
  if DEE_UNLIKELY(DeeXAst_ParseFunctionArgs(&argc,&argv,&function_flags,DEE_PARSER_ARGS_SCOPE(function_scope)) != 0) goto err_scope;
  if DEE_UNLIKELY(token.tk_id != ')') {
   if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_RPAREN_TO_END_OF_FUNCTION_PARAMETER_LIST,
    (DeeObject *)lexer,(DeeObject *)token_ob,
    "Expected ')' to end parameter list after 'function %s (...'",
    attr->a_name ? DeeString_STR(attr->a_name) : NULL) != 0) goto err_scope_argv;
  } else if DEE_UNLIKELY(!yield()) {
err_scope_argv:
   iter = argv+argc;
   while DEE_LIKELY(iter != argv) { --iter; _DeeXAstFunctionAstArgument_Quit(iter); }
   free(argv);
   goto err_scope;
  }
  if DEE_UNLIKELY(DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_scope_argv;
 } else noargs: argc = 0,argv = NULL;
 // Check for a return variable
 if (token.tk_id == ':') {
  if DEE_UNLIKELY(!yield()) goto err_scope_argv;
  if DEE_UNLIKELY(DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_scope_argv;
  if DEE_UNLIKELY(!TPPTokenID_IS_KEYWORD(token.tk_id)) {
   if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_KEYWORD_AFTER_FUNCTION_COLLON,
    (DeeObject *)lexer,(DeeObject *)token_ob,"Expected keyword after 'function %s(...):'",
    attr->a_name ? DeeString_STR(attr->a_name) : NULL) != 0) goto err_scope_argv;
  } else {
   DeeLocalVarObject *return_var; int temp;
   if DEE_UNLIKELY((return_var = (DeeLocalVarObject *)DeeLocalVar_New(
    token.tk_id,(DeeObject *)function_scope)) == NULL) goto err_scope_argv;
   return_var->lv_flags = (DEE_LOCALVAR_KIND_RETVAL|DEE_LOCALVAR_FLAG_INITIALIZED);
   ++return_var->lv_uses; // Return variables are always used
   function_scope->sc_flags |= DEE_SCOPE_FLAG_FOUND_RETVAR;
   temp = _DeeScope_AddName(function_scope,token.tk_id,(DeeObject *)return_var);
   Dee_DECREF(return_var);
   if DEE_UNLIKELY(temp != 0) goto err_scope_argv;
   if DEE_UNLIKELY(!yield()) goto err_scope_argv;
  }
  if DEE_UNLIKELY(DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_scope_argv;
 }
 // Initialize scope attributes (again)
 if DEE_UNLIKELY(_DeeScope_InitializeAttributes(function_scope,attr) != 0) goto err_scope_argv;

 // Push the new function name
 old_function_name = lexer->l_func_name;
 if (attr->a_name) {
  lexer->l_func_name = TPPKeywordList_GetS(&lexer->l_lexer.l_keywords,
                                           DeeString_STR(attr->a_name),
                                           DeeString_SIZE(attr->a_name),1);
  if DEE_UNLIKELY(!lexer->l_func_name) {
err_func_name_scope_argv:
   lexer->l_func_name = old_function_name;
   goto err_scope_argv;
  }
 } else {
  lexer->l_func_name = KWD___anonymous__;
 }

 // Check for an expression-style function
 if (token.tk_id == TPP_TOK_ARROW) {
  DeeXAstObject *function_expression;
  Dee_INCREF(block_token = token_ob);
  if DEE_UNLIKELY(!yield()) {err_scope_argv_block_token: Dee_DECREF(block_token); goto err_func_name_scope_argv; }
  // allow 'function -> { ... }' as alias for 'function { ... }'
  if (token.tk_id == '{') { Dee_DECREF(block_token); goto block_function; }
  if DEE_UNLIKELY((function_scope->sc_flags&DEE_SCOPE_FLAG_FOUND_RETVAR)!=0) {
   if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPRESSION_FUNCTION_AFTER_RETURN_VARIABLE,
    (DeeObject *)lexer,(DeeObject *)block_token,
    "Expression-style function has a return variable") != 0) goto err_scope_argv_block_token;
  } else function_scope->sc_flags |= DEE_SCOPE_FLAG_FOUND_RETURN;
  if DEE_UNLIKELY((function_expression = DeeXAst_ParseSingle(
   DEE_PARSER_ARGS_SCOPE(function_scope))) == NULL) goto err_scope_argv_block_token;
  function_block = DeeSAst_NewReturn(block_token,DeeParserLabelRefList_Empty,function_expression);
  Dee_DECREF(function_expression);
  Dee_DECREF(block_token);
  if DEE_UNLIKELY(!function_block) goto err_func_name_scope_argv;
  if (syntax_mode) *syntax_mode = DEE_XAST_FUNCTION_SYNTAX_MODE_EXPR;
 } else if DEE_LIKELY(token.tk_id == '{') {
block_function:
  Dee_INCREF(block_token = token_ob);
  if DEE_UNLIKELY(!yield()) goto err_scope_argv_block_token;
  function_block = DeeSAst_ParseAllInOldScopeUntil(block_token,'}',DeeParserLabelRefList_Empty,
                                                   DEE_PARSER_ARGS_SCOPE(function_scope));
  Dee_DECREF(block_token);
  if DEE_UNLIKELY(!function_block) goto err_func_name_scope_argv;
  if (syntax_mode) *syntax_mode = DEE_XAST_FUNCTION_SYNTAX_MODE_STMT;
 } else {
  if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_FORWARD_FUNCTION_WITH_ARGUMENT_LIST,
   (DeeObject *)lexer,(DeeObject *)function_token,
   "Forward-declared function %q should not feature an argument list. "
   "With an argument list present, an implicit, empty block is added to the function.",
   attr->a_name ? DeeString_STR(attr->a_name) : "<unnamed>") != 0) goto err_func_name_scope_argv;
  if DEE_UNLIKELY((function_block = DeeSAst_NewEmpty(
   function_token,DeeParserLabelRefList_Empty)
   ) == NULL) goto err_func_name_scope_argv;
  if (syntax_mode) *syntax_mode = DEE_XAST_FUNCTION_SYNTAX_MODE_NONE;
 }
 // Restore the old function name
 lexer->l_func_name = old_function_name;

 // Finally, put everything together
 if ((function_scope->sc_flags&DEE_SCOPE_FLAG_FOUND_THIS)!=0)
  function_flags |= DEE_FUNCTION_FLAG_THIS;
 if ((function_scope->sc_flags&DEE_SCOPE_FLAG_FOUND_YIELD)!=0)
  function_flags |= DEE_FUNCTION_FLAG_YILD; // Correct the flag for yield functions
 ast_result = DeeXAst_NewFunctionFromInheritedArgv(function_token,lexer,parser_flags,config,
                                                   (DeeObject *)attr->a_name,function_flags,
                                                   argc,argv,function_block,function_scope);
 Dee_DECREF(function_block);
 if DEE_UNLIKELY(!ast_result) goto err_scope_argv;
 // NOTE: The arguments were inherited by 'DeeXAst_NewFunctionFromInheritedArgv'
 Dee_DECREF(function_scope);
 return ast_result;
}

DEE_DECL_END
