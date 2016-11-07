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

DEE_A_RET_EXCEPT(-1) int DeeXAst_ParseFunctionArgs(
 DEE_A_OUT Dee_size_t *out_argc, DEE_A_OUT struct DeeXAstFunctionAstArgument **out_argv,
 DEE_A_INOUT Dee_uint32_t *flags, DEE_PARSER_PARAMS) {
 Dee_size_t                         arga = 0;
 Dee_size_t                         argc = 0;
 struct DeeXAstFunctionAstArgument *argv = NULL;
 struct DeeXAstFunctionAstArgument *new_argv;
 DeeTokenObject *entry_token;
 DeeLocalVarObject *entry_var;
 DeeXAstObject *entry_default;
 DEE_ASSERT(out_argc);
 DEE_ASSERT(out_argv);
 DEE_ASSERT(flags);
 while (1) {
  {
   struct DeeAttributeDecl arg_attr = DeeAttributeDecl_INIT();
   Dee_uint32_t vardecl_mode;
   vardecl_mode = DEE_XAST_VARDECL_MODE_DEFAULT;
   if DEE_UNLIKELY(DeeXAst_ParseVariableLookupModifiers(&vardecl_mode,&arg_attr,DEE_PARSER_ARGS) != 0) {
err_arg_attr: _DeeAttributeDecl_Quit(&arg_attr); goto err;
   }
   if DEE_UNLIKELY(vardecl_mode != DEE_XAST_VARDECL_MODE_DEFAULT &&
                   vardecl_mode != DEE_XAST_VARDECL_MODE_LOCAL) {
    if DEE_UNLIKELY(DeeError_CompilerError(
     DEE_WARNING_EXPECTED_LOCAL_OR_DEFAULT_VARIABLE_FOR_FUNCTION_PARAMETER,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected 'local' / default variable for function parameter") != 0) goto err_arg_attr;
   }
   if (!TPPTokenID_IS_KEYWORD(token.tk_id) && token.tk_id != TPP_TOK_DOTS)
   { _DeeAttributeDecl_Quit(&arg_attr); break; }
   // Add the new argument
   if DEE_UNLIKELY(_DeeScope_HasName(scope,token.tk_id)) {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_EXPECTED_FUNCTION_PARAMETER_ALREADY_USED,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "This function parameter has already been used") != 0) goto err_arg_attr;
    entry_var = NULL;
    entry_token = NULL;
   } else {
    if DEE_UNLIKELY((entry_var = (DeeLocalVarObject *)DeeLocalVar_New(
     token.tk_id,(DeeObject *)scope)) == NULL) goto err_arg_attr;
    DeeLocalVar_ApplyAttributes(entry_var,&arg_attr);
    entry_var->lv_flags = (DEE_LOCALVAR_KIND_PARAM|DEE_LOCALVAR_FLAG_INITIALIZED);
    ++entry_var->lv_init; // Argument variables are always initialized
    entry_var->lv_loc_id = (Dee_uint16_t)argc; // The argument index is the variable ID
    Dee_INCREF(entry_token = token_ob);
    if (
#if DEE_CONFIG_XAST_CHECK_RESERVED_IDENTIFERS
     DeeXAst_CheckReservedToken(lexer,token_ob) != 0 ||
#endif
     _DeeScope_AddDeprecatedName(scope,token.tk_id,
     (DeeObject *)entry_var,(DeeObject *)arg_attr.a_depr) != 0) {
     _DeeAttributeDecl_Quit(&arg_attr); goto err_entry_var;
    }
   }
   _DeeAttributeDecl_Quit(&arg_attr);
  }
  if DEE_UNLIKELY(!yield()) {err_entry_var: Dee_XDECREF(entry_token); Dee_XDECREF(entry_var); goto err; }
  if (token.tk_id == '=') { // Parse a default argument
   if DEE_UNLIKELY(!yield()) goto err_entry_var;
   if DEE_UNLIKELY((entry_default = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_entry_var;
   if (!entry_var) Dee_CLEAR(entry_default);
  } else entry_default = NULL;

  if (entry_var) {
   // Add a new entry
   if (argc == arga) {
    // Must reallocate the argument list
    if (!arga) arga = 2; else arga *= 2;
    while DEE_UNLIKELY((new_argv = (struct DeeXAstFunctionAstArgument *)realloc_nz(
     argv,arga*sizeof(struct DeeXAstFunctionAstArgument))) == NULL) {
     if DEE_LIKELY(Dee_CollectMemory()) continue;
     Dee_XDECREF(entry_default);
     DeeError_NoMemory();
     goto err_entry_var;
    }
    argv = new_argv;
   }
   argv[argc].a_token = entry_token; // inherit reference
   argv[argc].a_var = entry_var; // inherit reference
   argv[argc].a_default = entry_default; // inherit reference
   ++argc;
   if (entry_var->lv_name == TPP_TOK_DOTS) {
    // Unnamed dots is the '...'-variable vararg argument:
    // >> function foo(...) { print ...; }
    // >> foo(10,20,30); // prints '(10,20,30)'
    goto varargs_and_break_after_relaxed_comma;
   }
  } else DEE_ASSERT(!entry_default);
  if (token.tk_id == TPP_TOK_DOTS) {
   // Var-arg argument (this ends the argument list)
   if DEE_UNLIKELY(!yield()) goto err;
varargs_and_break_after_relaxed_comma:
    *flags |= DEE_FUNCTION_FLAG_VARG;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   if (token.tk_id == ',' && DEE_UNLIKELY(!yield())) goto err;
#endif
   break;
  }
  if (token.tk_id != ',') break;
  if DEE_UNLIKELY(!yield()) goto err;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
  if (token.tk_id == ')') break;
#endif
 }
 if (argc != arga) {
  // Try to safe some memory, but ignore errors silently
  if DEE_LIKELY((new_argv = (struct DeeXAstFunctionAstArgument *)realloc_nnz(
   argv,argc*sizeof(struct DeeXAstFunctionAstArgument))
   ) != NULL) argv = new_argv;
 }
 *out_argv = argv;
 *out_argc = argc;
 return 0;
err:
 new_argv = argv+argc;
 while DEE_LIKELY(new_argv != argv) { --new_argv; _DeeXAstFunctionAstArgument_Quit(new_argv); }
 free(argv);
 return -1;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseConstantFunction(
 DEE_A_INOUT DeeTokenObject *function_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_OUT_OPT Dee_uint32_t *syntax_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 if DEE_LIKELY(DeeAttributeDecl_Parse(&attr,DEE_PARSER_ARGS) == 0) {
  result = DeeXAst_ParseConstantFunctionWithAttribute(function_token,vardecl_mode,
                                                      syntax_mode,&attr,DEE_PARSER_ARGS);
 } else result = NULL;
 _DeeAttributeDecl_Quit(&attr);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseConstantFunctionWithAttribute(
 DEE_A_INOUT DeeTokenObject *function_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_OUT_OPT Dee_uint32_t *syntax_mode, DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeXAstObject *function_ast,*ast_result; TPPTokenID function_name;
 DeeObject *const_function_ob;
 DEE_ASSERT((vardecl_mode&DEE_XAST_VARDECL_FLAG_CONST)!=0);
 if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
  DeeTokenObject *function_name_tok;
  Dee_INCREF(function_name_tok = token_ob);
  function_name = token.tk_id;
  if DEE_UNLIKELY(!yield()) {err_function_name_tok: Dee_DECREF(function_name_tok); return NULL; }
  if DEE_UNLIKELY(DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_function_name_tok;
  if ((const_function_ob = _DeeScope_GetName(scope,function_name)) != NULL)
   Dee_INCREF(const_function_ob);
  else {
#if DEE_CONFIG_XAST_CHECK_RESERVED_IDENTIFERS
   if (DeeXAst_CheckReservedToken(lexer,function_name_tok) != 0) goto err_function_name_tok;
#endif /* DEE_CONFIG_XAST_CHECK_RESERVED_IDENTIFERS */
   if DEE_UNLIKELY((const_function_ob = DeeFunction_New(Dee_EmptyCode,Dee_EmptyTuple,0,
    DeeAttributeDecl_GET_FUNCTION_FLAGS(attr))) == NULL) goto err_function_name_tok;
   if DEE_UNLIKELY(_DeeScope_AddDeprecatedName(scope,function_name,
    const_function_ob,(DeeObject *)attr->a_depr) != 0) {
    Dee_DECREF(function_name_tok);
    goto err_const_function_ob;
   }
  }
  Dee_DECREF(function_name_tok);
 } else function_name = 0,const_function_ob = NULL;
 if (token.tk_id == '(' || token.tk_id == TPP_TOK_ARROW ||
     token.tk_id == '{' || token.tk_id == ':') {
  if (function_name && !attr->a_name) {
   if DEE_UNLIKELY((attr->a_name = (DeeStringObject *)DeeString_New(TPPLexer_TokenIDStr(
    DeeLexer_LEXER(lexer),function_name))) == NULL) {
err_const_function_ob: Dee_XDECREF(const_function_ob); return NULL;
   }
  }
  // Generate a constant function
  function_ast = DeeXAst_ParseUnnamedFunction(function_token,attr,syntax_mode,DEE_PARSER_ARGS_EX(
   // NOTE: Also perform known predicate optimization
   parser_flags|DEE_PARSER_FLAG_CONST_FUNCTION|DEE_PARSER_FLAG_OPTIMIZE_PREDICATES));
  if DEE_UNLIKELY(!function_ast) goto err_const_function_ob;
 } else {
  if (syntax_mode) *syntax_mode = DEE_XAST_FUNCTION_SYNTAX_MODE_NONE;
no_function_ast:
  if DEE_UNLIKELY(!const_function_ob && (const_function_ob = DeeFunction_New(
   Dee_EmptyCode,Dee_EmptyTuple,0,DEE_FUNCTION_FLAG_NONE)) == NULL) return NULL;
  goto return_const_function_ob;
 }

 if DEE_UNLIKELY(function_ast->ast_kind != DEE_XASTKIND_CONST) {
  if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_CANNOT_GENERATE_CONSTANT_FUNCTION,
   (DeeObject *)lexer,(DeeObject *)function_ast->ast_common.ast_token,
   "Could not generate constant function from:\n\t%>r\n",function_ast) != 0) {
err_function_ast_const_function_ob:
   Dee_DECREF(function_ast);
   goto err_const_function_ob;
  }
  Dee_DECREF(function_ast);
  goto no_function_ast;
 }

 if (const_function_ob) {
  if DEE_UNLIKELY(!DeeFunction_Check(function_ast->ast_const.c_const) &&
                  DeeFunction_CODE(const_function_ob) == Dee_EmptyCode &&
                  DeeFunction_REFS(const_function_ob) == Dee_EmptyTuple) {
   // This can happen due to predicate detection
   // >> Now we must actually exchange the function constant
   _DeeScope_DelName(scope,function_name);
   if DEE_UNLIKELY(_DeeScope_AddDeprecatedName(scope,function_name,
    function_ast->ast_const.c_const,
    (DeeObject *)attr->a_depr) != 0) goto err_function_ast_const_function_ob;
  } else if DEE_UNLIKELY(DeeObject_MoveAssign(const_function_ob,function_ast->ast_const.c_const) != 0) {
   DeeError_Handled();
   if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_CONST_NAME_ALREADY_USED,
    (DeeObject *)lexer,(DeeObject *)function_ast->ast_common.ast_token,
    "Constant function %s was already defined with a different value",
    function_name ? TPPLexer_TokenIDStr(DeeLexer_LEXER(lexer),function_name) : "??" "?"
    ) != 0) goto err_function_ast_const_function_ob;
  }
 } else Dee_INCREF(const_function_ob = function_ast->ast_const.c_const);
 Dee_DECREF(function_ast);
 if ((attr->a_flags&DEE_ATTRIBUTE_DECL_FLAG_COPYABLE)!=0 &&
     DeeFunction_Check(const_function_ob) // v set the copyable flag
     ) ((DeeCodeObject *)DeeFunction_CODE(const_function_ob))->co_flags |= DEE_CODE_FLAG_COPY;
return_const_function_ob:
 ast_result = DeeXAst_NewConst(function_token,const_function_ob);
 Dee_DECREF(const_function_ob);
 return ast_result;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseFunction(
 DEE_A_INOUT DeeTokenObject *function_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_OUT_OPT Dee_uint32_t *syntax_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 if DEE_LIKELY(DeeAttributeDecl_Parse(&attr,DEE_PARSER_ARGS) == 0) {
  result = DeeXAst_ParseFunctionWithAttribute(function_token,vardecl_mode,
                                              syntax_mode,&attr,DEE_PARSER_ARGS);
 } else result = NULL;
 _DeeAttributeDecl_Quit(&attr);
 return result;
}
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseFunctionWithAttribute(
 DEE_A_INOUT DeeTokenObject *function_token, DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_OUT_OPT Dee_uint32_t *syntax_mode, DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeXAstObject *function_storage,*function_ast,*ast_result;
 if ((vardecl_mode&DEE_XAST_VARDECL_FLAG_CONST)!=0) {
  return DeeXAst_ParseConstantFunctionWithAttribute(function_token,vardecl_mode,
                                                    syntax_mode,attr,DEE_PARSER_ARGS);
 }
 switch (token.tk_id) {
  DEE_PARSE_TOKENID_CASE_CONST
   if DEE_UNLIKELY(!yield()) return NULL; // Function has a constant name
   return DeeXAst_ParseConstantFunctionWithAttribute(function_token,
                                                     vardecl_mode|DEE_XAST_VARDECL_FLAG_CONST,
                                                     syntax_mode,attr,DEE_PARSER_ARGS);
  default: break;
 }
 if (!TPPTokenID_IS_KEYWORD(token.tk_id) && token.tk_id != TPP_TOK_COLLON_COLLON) {
  function_storage = NULL;
  goto unnamed;
 }
 // Named function
 // NOTE: The variable associated with this is always instance-bound
 if DEE_UNLIKELY((attr->a_flags&(
  DEE_ATTRIBUTE_DECL_FLAG_INSTBOUNDC|
  DEE_ATTRIBUTE_DECL_FLAG_INSTBOUNDM))!=0) {
  if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_NAMED_FUNCTION_ALWAYS_INSTANCE_BOUND,
   (DeeObject *)lexer,(DeeObject *)function_token,
   "Named function %q was explicitly declared as [[instance_bound(\"move\")]]",
   attr->a_name ? DeeString_STR(attr->a_name) : "<unnamed>") != 0) return NULL;
  attr->a_flags &= ~(DEE_ATTRIBUTE_DECL_FLAG_INSTBOUNDC);
  attr->a_flags |=  (DEE_ATTRIBUTE_DECL_FLAG_INSTBOUNDM);
 } else attr->a_flags |= DEE_ATTRIBUTE_DECL_FLAG_INSTBOUNDM;
 if DEE_UNLIKELY((function_storage = DeeXAst_ParseVariableLookup(
  vardecl_mode,attr,DEE_PARSER_ARGS)) == NULL) return NULL;
 if DEE_UNLIKELY(DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) goto err_storage;
unnamed:
 if (token.tk_id == '(' || token.tk_id == TPP_TOK_ARROW ||
     token.tk_id == '{' || token.tk_id == ':') {
  if (function_storage && !attr->a_name) {
   if DEE_UNLIKELY((attr->a_name = (DeeStringObject *)DeeXAst_Name(function_storage)) == NULL) {
err_storage: Dee_XDECREF(function_storage); return NULL;
   }
  }
  function_ast = DeeXAst_ParseUnnamedFunction(
   function_token,attr,syntax_mode,DEE_PARSER_ARGS);
 } else {
  if (syntax_mode) *syntax_mode = DEE_XAST_FUNCTION_SYNTAX_MODE_NONE;
  function_ast = DeeXAst_NewFunction(function_token,lexer,parser_flags,config,
                                     (DeeObject *)attr->a_name,
                                     DeeAttributeDecl_GET_FUNCTION_FLAGS(attr),
                                     0,NULL,NULL,NULL);
 }
 if DEE_UNLIKELY(!function_ast) goto err_storage;
 if DEE_UNLIKELY(!function_storage) return function_ast;
 ast_result = DeeXAst_NewStore(function_token,lexer,parser_flags,
                               function_storage,function_ast,scope);
 Dee_DECREF(function_ast);
 Dee_DECREF(function_storage);
 return ast_result;
}

DEE_DECL_END
