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
//#define DO_CONST
#endif

DEE_DECL_BEGIN

#ifdef DO_CONST
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseConstantNamedTypeSuffixWithAttribute
#else
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseNamedTypeSuffixWithAttribute
#endif
(DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN DeeXAstObject *base, DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeXAstObject *ast_result,*result2,*args_ast,*call_ast,*ast_init;
 DeeTokenObject *name_token,*assign_token; Dee_uint32_t vardecl_mode2;
#ifndef DO_CONST
 DeeXAstObject *var_ast;
 DeeLocalVarObject *local_var;
#endif
#if DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS
 struct DeeXAstTypeOperations post_ops = DeeXAstTypeOperations_INIT();
#endif /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
 // We need to preserve the post_ops as to still know argument names in foreign function closures
 if (_DeeXAst_InternalDoParseNamedTypeSuffix(
  &name_token,&vardecl_mode2,attr,
#if DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS
  &post_ops,
#endif /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
  DEE_PARSER_ARGS) != 0) {
err_post_ops: ast_result = NULL; goto end;
 } 
 if (!name_token) {
#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE
  // NOTE: We alloc unnamed closures
  if ((token.tk_id == '{' || token.tk_id == ':' || token.tk_id == TPP_TOK_ARROW) &&
      post_ops.o_opsc && post_ops.o_opsv[0].oe_kind == DEE_XASTTYPEOPERATION_KIND_FOREIGN_FUNCTION) {
#ifndef DO_CONST
   var_ast = NULL;
#endif
   goto parse_ff_initializer;
  }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE */
return_raw_ast_after_post_ops:
#if DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS
  ast_result = DeeXAstTypeOperations_Exec(&post_ops,base,lexer,parser_flags);
#else /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
  Dee_INCREF(ast_result = base);
#endif /* !DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
  goto end_with_semicollon;
 } 
 if ((flags&DEE_XAST_INITORDECL_FLAGS_ALLOWINIT)==0) {
  if (DeeError_CompilerError(DEE_WARNING_CTYPES_NAMED_TYPE_WITHOUT_INIT_RIGHTS,
   (DeeObject *)lexer,(DeeObject *)name_token,
   "Named c-type variable without initialization rights (variable cannot be initialized here)"
   ) != 0) {
err_name_post_ops: Dee_DECREF(name_token); goto err_post_ops;
  }
  goto return_raw_ast_after_post_ops;
 }
 // Must create a var-decl branch
#ifndef DO_CONST
#if DEE_CONFIG_LANGUAGE_NAMED_CTYPE_DECL_ALWAYS_LOCAL
 local_var = (DeeLocalVarObject *)_DeeScope_GetName(scope,name_token->tk_token.tk_id);
#else
 switch (DEE_XAST_VARDECL_MODE(vardecl_mode2)) {
  case DEE_XAST_VARDECL_MODE_GLOBAL:
   local_var = (DeeLocalVarObject *)_DeeScope_GetName((DeeScopeObject *)(
    DeeScope_Global((DeeObject *)scope)),name_token->tk_token.tk_id);
   break;
  case DEE_XAST_VARDECL_MODE_LOCAL:
   local_var = (DeeLocalVarObject *)_DeeScope_GetName(scope,name_token->tk_token.tk_id);
   break;
  { DeeScopeObject *scope_iter;
  default: scope_iter = scope; while (1) {
   if ((local_var = (DeeLocalVarObject *)_DeeScope_GetName(scope_iter,name_token->tk_token.tk_id)) != NULL) break;
   if (scope_iter->sc_weak_prev) scope_iter = scope_iter->sc_weak_prev;
   else if (scope_iter->sc_prev) scope_iter = scope_iter->sc_prev;
   else break;
  } break;
  }
 }
#endif
 if (local_var != NULL) {
  if ((vardecl_mode&DEE_XAST_VARDECL_FLAG_ENABLED)==0) {
   if (DeeError_CompilerError(DEE_WARNING_CTYPES_INITIALIZATION_WITHOUT_DECL_RIGHTS,
    (DeeObject *)lexer,(DeeObject *)name_token,
    "Initialization of a c-types variable without declaration rights is discouraged. "
    "(Please Move this initialization elsewhere.)") != 0) goto err_name_post_ops;
  }
  if (!DeeLocalVar_Check(local_var)) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_VARIABLE_FOR_NAMED_TYPE,
    (DeeObject *)lexer,(DeeObject *)name_token,
    "Expected variable for type name, but got instance of %q",
    DeeType_NAME(Dee_TYPE(local_var))) != 0) goto err_name_post_ops;
   // Ignore the type name as fallback
   goto return_raw_ast_after_post_ops;
  }
#if DEE_CONFIG_LANGUAGE_NAMED_CTYPE_DECL_ALWAYS_BOUND
  if ((DeeLocalVar_FLAGS(local_var)&(DEE_LOCALVAR_FLAG_INSTBOUNDC|DEE_LOCALVAR_FLAG_INSTBOUNDM))==0) {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_VARIABLE_FOR_NAMED_TYPE,
    (DeeObject *)lexer,(DeeObject *)name_token,
    "Expected instance-bound variable for named type declaration") != 0) goto err_name_post_ops;
  }
#endif /* DEE_CONFIG_LANGUAGE_NAMED_CTYPE_DECL_ALWAYS_BOUND */
  if ((var_ast = DeeXAst_NewVar(name_token,local_var,scope)) == NULL) goto err_name_post_ops;
 } else if ((vardecl_mode&DEE_XAST_VARDECL_FLAG_ENABLED)!=0) {
  if ((local_var = (DeeLocalVarObject *)DeeLocalVar_New(
   name_token->tk_token.tk_id,(DeeObject *)scope)) == NULL) goto err_name_post_ops;
  DeeLocalVar_ApplyAttributes(local_var,attr);
#if DEE_CONFIG_LANGUAGE_NAMED_CTYPE_DECL_ALWAYS_BOUND
  if ((local_var->lv_flags&(DEE_LOCALVAR_FLAG_INSTBOUNDC|DEE_LOCALVAR_FLAG_INSTBOUNDM))==0) {
   // These types of variables are instance-bound by default
   local_var->lv_flags |= DEE_LOCALVAR_FLAG_INSTBOUNDC;
  }
#endif /* DEE_CONFIG_LANGUAGE_NAMED_CTYPE_DECL_ALWAYS_BOUND */
  // TODO: Standardized interface to accessing variables with a given vardecl_mode
  if ((vardecl_mode2&DEE_XAST_VARDECL_FLAG_STACK)!=0)
   local_var->lv_flags = DEE_LOCALVARFLAGS_FLAGS(local_var->lv_flags)|DEE_LOCALVAR_KIND_STACK;
  else if ((vardecl_mode2&DEE_XAST_VARDECL_FLAG_STATIC)!=0)
   local_var->lv_flags = DEE_LOCALVARFLAGS_FLAGS(local_var->lv_flags)|DEE_LOCALVAR_KIND_STATIC;
  if ((vardecl_mode2&DEE_XAST_VARDECL_FLAG_VOLATILE)!=0)
   local_var->lv_flags |= DEE_LOCALVAR_FLAG_VOLATILE;
  if (
#if DEE_CONFIG_XAST_CHECK_RESERVED_IDENTIFERS
   DeeXAst_CheckReservedToken(lexer,name_token) != 0 ||
#endif
   _DeeScope_AddDeprecatedName(scope,name_token->tk_token.tk_id,
   (DeeObject *)local_var,(DeeObject *)attr->a_depr) != 0) {
   Dee_DECREF(local_var); goto err_name_post_ops;
  }
  var_ast = DeeXAst_NewLocalVar(name_token,local_var,scope);
  Dee_DECREF(local_var);
  if (!var_ast) goto err_name_post_ops;
 } else {
  if (DeeError_CompilerError(DEE_WARNING_UNKNOWN_VARIABLE,
   (DeeObject *)lexer,(DeeObject *)name_token,"Unknown variable") != 0) goto err_name_post_ops;
  var_ast = NULL;
 }
#else
 // TODO: Some types like 'list' should get an empty constant to work with:
 //    >> const list my_list = { 10,20,my_list,30 }; // Constant,self-referencing list
#endif
#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE
 if (token.tk_id == '{' || token.tk_id == ':' || token.tk_id == TPP_TOK_ARROW) {
  TPPTokenID old_function_name;
  DeeObject *function_name_str;
parse_ff_initializer:
  Dee_INCREF(assign_token = token_ob);
  old_function_name = lexer->l_func_name;
  if (name_token) {
   lexer->l_func_name = name_token->tk_token.tk_id;
   if ((function_name_str = DeeToken_Str((
    DeeObject *)name_token)) == NULL) goto err_var_ast_name_result;
  } else {
   lexer->l_func_name = KWD___anonymous__;
   function_name_str = NULL;
  }
  call_ast = DeeXAst_ParseForeignFunctionClosureCode(
   base,&post_ops,attr,function_name_str,DEE_PARSER_ARGS);
  Dee_XDECREF(function_name_str);
  lexer->l_func_name = old_function_name;
  if (!call_ast) { Dee_DECREF(assign_token); goto err_var_ast_name_result; }
#ifndef DO_CONST
  if (var_ast)
   result2 = DeeXAst_NewStore(assign_token,lexer,parser_flags,var_ast,call_ast,scope);
  else
#endif
  {
   Dee_INCREF(result2 = call_ast);
  }
  Dee_DECREF(assign_token);
  Dee_DECREF(call_ast);
#ifndef DO_CONST
  Dee_XDECREF(var_ast);
#endif
  if (!result2) goto err_name_post_ops;
  Dee_INHERIT_REF(ast_result,result2);
  // Still consume an optional ';'
  if ((flags&DEE_XAST_INITORDECL_FLAGS_REQSEMICOLLON)!=0 &&
      token.tk_id == ';' && !yield()) goto err_r_name_token;
  goto end_with_name;
 }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE */
#if DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS
 if ((ast_result = DeeXAstTypeOperations_Exec(
  &post_ops,base,lexer,parser_flags)) == NULL) goto err_var_ast_name_result;
#else /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
 Dee_INCREF(ast_result = base);
#endif /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
 if (token.tk_id == '=') {
  Dee_INCREF(assign_token = token_ob);
  if DEE_UNLIKELY(!yield()) {
err_assign_token_name_result:
   Dee_DECREF(assign_token);
   Dee_DECREF(ast_result);
err_var_ast_name_result:
#ifndef DO_CONST
   Dee_XDECREF(var_ast);
#endif
   goto err_name_post_ops;
  }
  // NOTE: Brace initializers are allowed in here
  if (token.tk_id == '{') { // Brace initializer
   ast_init = DeeXAst_ParseRangeInitializerWithType(ast_result,DEE_PARSER_ARGS);
   if (!ast_init) goto err_assign_token_name_result;
   goto create_store_from_ast_init;
  }
  if ((ast_init = DeeXAst_ParseSingle(DEE_PARSER_ARGS
   )) == NULL) goto err_assign_token_name_result;
  if (ast_result->ast_kind == DEE_XASTKIND_CONST &&
      DeeType_Check(ast_result->ast_const.c_const) &&
     (DeeTypeObject *)ast_result->ast_const.c_const ==
      DeeXAst_PredictType(ast_init)) {
   // The initializer already has the typing of the base
   // >> Skip the call to the constructor
create_store_from_ast_init:
   Dee_INHERIT_REF(call_ast,ast_init);
   Dee_DECREF(ast_result);
   goto create_store_ast;
  }
  // Create the argument tuple from the initializer
  args_ast = DeeXAst_NewSequence(DEE_XASTKIND_TUPLE,assign_token,lexer,
                                 parser_flags,1,&ast_init);
  Dee_DECREF(ast_init);
  if (!args_ast) goto err_assign_token_name_result;
  call_ast = DeeXAst_NewBinary(DEE_XASTKIND_CALL,assign_token,lexer,parser_flags,
                               ast_result,args_ast);
  Dee_DECREF(args_ast);
  if (!call_ast) goto err_assign_token_name_result;
  Dee_DECREF(ast_result);
create_store_ast:
#ifndef DO_CONST
  if (var_ast)
   result2 = DeeXAst_NewStore(assign_token,lexer,parser_flags,var_ast,call_ast,scope);
  else
#endif
  {
   Dee_INCREF(result2 = call_ast);
  }
  Dee_DECREF(assign_token);
 } else {
  // No initializer given (compile a call to the default constructor)
  if ((args_ast = DeeXAst_NewConst(name_token,Dee_EmptyTuple)) == NULL) goto err_var_ast_name_result;
  call_ast = DeeXAst_NewBinary(DEE_XASTKIND_CALL,name_token,lexer,parser_flags,
                               ast_result,args_ast);
  Dee_DECREF(args_ast);
  if (!call_ast) goto err_var_ast_name_result;
  Dee_DECREF(ast_result);
#ifndef DO_CONST
  if (var_ast)
   result2 = DeeXAst_NewStore(name_token,lexer,parser_flags,var_ast,call_ast,scope);
  else
#endif
  {
   Dee_INCREF(result2 = call_ast);
  }
 }
 Dee_DECREF(call_ast);
#ifndef DO_CONST
 Dee_XDECREF(var_ast);
#endif
 Dee_INHERIT_REF(ast_result,result2);
end_with_semicollon:
 if ((flags&DEE_XAST_INITORDECL_FLAGS_REQSEMICOLLON)!=0) {
  if (token.tk_id != ';') {
   if ((flags&DEE_XAST_INITORDECL_FLAGS_NOSEMIBUTCOMMA)==0 || token.tk_id != ',') {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_EXPRESSION,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' after expression '%r'",
     ast_result) != 0) goto err_r_name_token;
   }
  } else if DEE_UNLIKELY(!yield()) {
err_r_name_token:
   Dee_XDECREF(name_token);
   Dee_CLEAR(ast_result);
   goto end;
  }
 }
end_with_name:
#ifdef DO_CONST
 if (!ast_result) { Dee_XDECREF(name_token); goto end; }
 if (ast_result->ast_kind != DEE_XASTKIND_CONST) {
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_IN_CONSTANT_NAMED_TYPE_DECLARATION,
   (DeeObject *)lexer,(DeeObject *)token_ob,"Expected constant for name type declaration, but got %r",
   ast_result) != 0) goto err_r_name_token;
  Dee_XCLEAR(name_token);
 } else
#endif
 if (name_token) {
#ifdef DO_CONST
  DeeObject *found_constant;
  if (ast_result) {
   DEE_ASSERT(ast_result->ast_kind == DEE_XASTKIND_CONST);
#if DEE_CONFIG_LANGUAGE_NAMED_CTYPE_DECL_ALWAYS_LOCAL
   found_constant = _DeeScope_GetName(scope,name_token->tk_token.tk_id);
#else
   switch (DEE_XAST_VARDECL_MODE(vardecl_mode)) {
    case DEE_XAST_VARDECL_MODE_LOCAL: found_constant = _DeeScope_GetName(scope,name_token->tk_token.tk_id); break;
    case DEE_XAST_VARDECL_MODE_GLOBAL: found_constant = _DeeScope_GetName(DeeScope_GLOBAL(scope),name_token->tk_token.tk_id); break;
    {DeeScopeObject *scope_iter;
    default: scope_iter = scope; while (1) {
      if ((found_constant = _DeeScope_GetName(scope_iter,name_token->tk_token.tk_id)) != NULL) break;
      if (scope_iter->sc_weak_prev) scope_iter = scope_iter->sc_weak_prev;
      else if (scope_iter->sc_prev) scope_iter = scope_iter->sc_prev;
      else break;
     }
    } break;
   }
#endif
   if (found_constant) {
    int temp;
    if ((temp = DeeObject_DeepEquals(found_constant,ast_result->ast_const.c_const)) < 0) {
     DeeError_Handled();
     goto constant_already_defined;
    }
    if (!temp) {
constant_already_defined:
     if (DeeError_CompilerErrorf(DEE_WARNING_CONST_NAME_ALREADY_USED,
      (DeeObject *)lexer,(DeeObject *)name_token,
      "Constant was already defined with a different value:\n"
      "\tOld value: %>r\n\tNew value: %>r",found_constant,
      ast_result->ast_const.c_const) != 0) goto err_r_name_token;
    }
   } else if ((vardecl_mode&DEE_XAST_VARDECL_FLAG_ENABLED)==0) {
    if (DeeError_CompilerError(DEE_WARNING_CANT_DECLARE_NAMED_TYPE_CONSTANT_WITHOUT_DECL_RIGHTS,
     (DeeObject *)lexer,(DeeObject *)name_token,
     "Can't declare constant without declaration rights") != 0) goto err_r_name_token;
   } else {
    if (_DeeScope_AddDeprecatedName(scope,name_token->tk_token.tk_id,
     ast_result->ast_const.c_const,(DeeObject *)attr->a_depr) != 0) goto err_r_name_token;
   }
  }
#endif
  Dee_DECREF(name_token);
 }
end:
#if DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS
 _DeeXAstTypeOperations_Quit(&post_ops);
#endif /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
 return ast_result;
}

DEE_DECL_END

#ifdef DO_CONST
#undef DO_CONST
#endif

