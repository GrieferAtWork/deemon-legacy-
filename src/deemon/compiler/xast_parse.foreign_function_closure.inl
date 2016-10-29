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

#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseForeignFunctionClosureCode(
 DEE_A_INOUT DeeXAstObject *base, DEE_A_INOUT struct DeeXAstTypeOperations *post_operations,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_A_INOUT_OPT DeeObject *function_name, DEE_PARSER_PARAMS) {
 DeeTokenObject *block_token;
 DeeScopeObject *ff_scope; DeeSAstObject *ff_code;
 DeeXAstObject *ast_result,*ast_ff_type,*ast_closure; DeeLocalVarObject *arg_var;
 struct DeeXAstTypeOperationsEntry *ff_entry; Dee_size_t argc;
 struct DeeXAstTypeOperationsForeignFunctionArgument *arg_iter,*arg_end;
 struct DeeXAstFunctionAstArgument *dee_argv,*dee_argv_iter;
 (void)attr;

 // Make sure that the last (front) entry in post_operations describes an ff type
 if (!post_operations->o_opsc ||
     post_operations->o_opsv[0].oe_kind != DEE_XASTTYPEOPERATION_KIND_FOREIGN_FUNCTION) {
  if ((ast_result = DeeXAstTypeOperations_Exec(post_operations,base,lexer,parser_flags)) == NULL) return NULL;
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_FOREIGN_FUNCTION_TYPE_FOR_FF_CLOSURE,
   (DeeObject *)lexer,(DeeObject *)base->ast_common.ast_token,
   "Expected foreign function type for foreign function closure, but got %r",
   ast_result) != 0) { Dee_DECREF(ast_result); return NULL; }
  return ast_result;
 }
 ff_entry = &post_operations->o_opsv[0];
 if ((ff_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,0)) == NULL) return NULL;
 ff_scope->sc_flags |= DEE_SCOPE_FLAG_FOUND_RETURN; // Suppress yield in ff-closures

 // Setup all the deemon-style arguments in the ff scope
 DEE_ASSERT(ff_entry->oe_kind == DEE_XASTTYPEOPERATION_KIND_FOREIGN_FUNCTION);
 argc = ff_entry->oe_foreign_function.ff_argc;
 arg_end = (arg_iter = ff_entry->oe_foreign_function.ff_argv)+argc;
 if (argc) {
  while ((dee_argv = (struct DeeXAstFunctionAstArgument *)malloc_nz(
   argc*sizeof(struct DeeXAstFunctionAstArgument))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
err_ff_scope: Dee_DECREF(ff_scope); return NULL;
  }
 } else dee_argv = NULL;
 dee_argv_iter = dee_argv;

process_args:
 while (arg_iter != arg_end) {
  DEE_ASSERT(dee_argv_iter);
  DEE_ASSERT(DEE_LOCALVARFLAGS_KIND(arg_iter->ffa_varflags) == DEE_LOCALVAR_KIND_PARAM);
  if (arg_iter->ffa_name) {
   Dee_size_t args_after;
   if (_DeeScope_HasName(ff_scope,arg_iter->ffa_name->tk_token.tk_id)) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_FUNCTION_PARAMETER_ALREADY_USED,
     (DeeObject *)lexer,(DeeObject *)arg_iter->ffa_name,
     "This function parameter name has already been used (parameter will be ignored)") != 0) {
err_deeargiter_ff_scope:
     while (dee_argv_iter != dee_argv) { --dee_argv_iter; _DeeXAstFunctionAstArgument_Quit(dee_argv_iter); }
     free(dee_argv);
     goto err_ff_scope;
    }
/*remove_argument:*/
    args_after = (Dee_size_t)((arg_end-arg_iter)-1);
    _DeeXAstTypeOperationsForeignFunctionArgument_Quit(arg_iter);
    memmove(arg_iter,arg_iter+1,args_after*sizeof(struct DeeXAstTypeOperationsForeignFunctionArgument));
    memmove(dee_argv_iter,dee_argv_iter+1,args_after*sizeof(struct DeeXAstFunctionAstArgument));
    --argc,--arg_end,--ff_entry->oe_foreign_function.ff_argc;
    goto process_args;
   } else {
    if ((arg_var = (DeeLocalVarObject *)DeeLocalVar_New(
     arg_iter->ffa_name->tk_token.tk_id,(DeeObject *)ff_scope
     )) == NULL) goto err_deeargiter_ff_scope;
    arg_var->lv_flags = arg_iter->ffa_varflags|DEE_LOCALVAR_FLAG_INITIALIZED;
    ++arg_var->lv_init; // Argument variables are always initialized
    arg_var->lv_loc_id = (Dee_uint16_t)(arg_iter-ff_entry->oe_foreign_function.ff_argv);
    if (
#if DEE_CONFIG_XAST_CHECK_RESERVED_IDENTIFERS
     DeeXAst_CheckReservedToken(lexer,arg_iter->ffa_name) != 0 ||
#endif
     _DeeScope_AddName(ff_scope,arg_iter->ffa_name->tk_token.tk_id,(DeeObject *)arg_var) != 0) {
     Dee_DECREF(arg_var); goto err_deeargv_ff_scope;
    }
    Dee_INHERIT_REF(dee_argv_iter->a_var,arg_var);
    dee_argv_iter->a_default = NULL;
    Dee_INCREF(dee_argv_iter->a_token = arg_iter->ffa_name);
   }
  } else {
   DEE_ASSERT(!arg_iter->ffa_name);
   // Set a stub variable and name for this argument
   // >> This is similar to immediatly deleting the argument as soon as you enter the scope
   if ((arg_var = (DeeLocalVarObject *)DeeLocalVar_New(0,(DeeObject *)ff_scope)) == NULL) goto err_deeargiter_ff_scope;
   arg_var->lv_flags = arg_iter->ffa_varflags|DEE_LOCALVAR_FLAG_INITIALIZED;
   ++arg_var->lv_init; // Argument variables are always initialized
   arg_var->lv_loc_id = (Dee_uint16_t)(arg_iter-ff_entry->oe_foreign_function.ff_argv);
   dee_argv_iter->a_default = NULL;
   Dee_INCREF(dee_argv_iter->a_token = base->ast_common.ast_token); // We need *something*...
   Dee_INHERIT_REF(dee_argv_iter->a_var,arg_var);
  }
  ++arg_iter;
  ++dee_argv_iter;
 }

 // Parse the function block
 Dee_INCREF(block_token = token_ob);
 // TODO: Support for expression-style functions (aka. '->')
 if (token.tk_id != '{') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LBRACE_IN_FF_CLOSURE_BLOCK,
   (DeeObject *)lexer,(DeeObject *)token_ob,
   "Expected '{' to begin foreign function closure block"
   ) != 0) goto err_blocktoken_deeargv_ff_scope;
 } else if DEE_UNLIKELY(!yield()) goto err_blocktoken_deeargv_ff_scope;
 if ((ff_code = DeeSAst_ParseAllInOldScopeUntil(block_token,'}',
  DeeParserLabelRefList_Empty,DEE_PARSER_ARGS_SCOPE(ff_scope))) == NULL) {
err_blocktoken_deeargv_ff_scope:
  Dee_DECREF(block_token);
err_deeargv_ff_scope:
  dee_argv_iter = dee_argv+argc;
#if 1
  goto err_deeargiter_ff_scope;
#else
  while (dee_argv_iter != dee_argv) { --dee_argv_iter; _DeeXAstFunctionAstArgument_Quit(dee_argv_iter); }
  free(dee_argv);
  goto err_ff_scope;
#endif
 }
 Dee_DECREF(block_token);

 // Everything is parsed. Now for the actual code
 // Here we generate the deemon-style function that will be called
 ast_result = DeeXAst_NewFunctionFromInheritedArgv(
  base->ast_common.ast_token,lexer,parser_flags,
  config,function_name,DEE_FUNCTION_FLAG_NONE,
  argc,dee_argv,ff_code,ff_scope);
 if (!ast_result) goto err_deeargv_ff_scope;
 Dee_DECREF(ff_code);
 Dee_DECREF(ff_scope);
 // At this point, the only remaining object is the deemon-style function in 'ast_result'

 // Calculate the actual ff-typing
 if ((ast_ff_type = DeeXAstTypeOperations_Exec(
  post_operations,base,lexer,parser_flags)) == NULL) {
  Dee_DECREF(ast_result);
  return NULL;
 }
 // We wrap 'ast_result' in a proxy-style AST
 // that generates the foreign function closure
 // >> typedef int FUN_TYPE(int,int);
 // >> fun_a = __builtin_ff_closure(FUN_TYPE,function(a,b) { return a+b; })
 // >> int fun_b(int a, int b) { return a+b; }
 ast_closure = DeeXAst_NewFFClosure(ast_result->ast_common.ast_token,lexer,
                                    parser_flags,ast_ff_type,ast_result);
 Dee_DECREF(ast_ff_type);
 Dee_DECREF(ast_result);

 // Done!
 return ast_closure;
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */

DEE_DECL_END
