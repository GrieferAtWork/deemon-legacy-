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
#include "sast.c"
#endif


DEE_DECL_BEGIN

DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_CopyWithScope(
 DEE_A_INOUT DeeSAstObject *self, DEE_A_INOUT DeeScopeObject *new_scope,
 DEE_A_INOUT DeeLexerObject *lexer) {
 DeeSAstObject *ast_result;
 DeeXAstObject *expr_ast;
 DEE_ASSERT(DeeObject_Check(self) && DeeSAst_Check(self));
 DEE_ASSERT(DeeObject_Check(new_scope) && DeeScope_Check(new_scope));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 if (!DeeParserLabelRefList_EMPTY(&self->ast_common.ast_labels)) {
  if (DeeError_CompilerError(DEE_WARNING_CANT_COPY_LABEL_IN_ALIAS_DECLARATION,
   (DeeObject *)lexer,(DeeObject *)self->ast_common.ast_token,
   "Can't copy [case-]labels in alias reference") != 0) return NULL;
 }
 switch (self->ast_kind) {
  case DEE_SASTKIND_CONTINUE:
  case DEE_SASTKIND_BREAK:
  case DEE_SASTKIND_EMPTY:
ret_self: Dee_INCREF(self); return self;

  case DEE_SASTKIND_EXPRESSION: {
   if ((expr_ast = DeeXAst_CopyWithScope(self->ast_expression.x_ast,new_scope,lexer)) == NULL) return NULL;
   if (expr_ast == self->ast_expression.x_ast) {decref_expr_and_retself: Dee_DECREF(expr_ast); goto ret_self; }
   if ((ast_result = _DeeSAst_NewUnsafe(DEE_SASTKIND_EXPRESSION,
    self->ast_common.ast_token,DeeParserLabelRefList_Empty)) == NULL)
   {err_expr: Dee_DECREF(expr_ast); return NULL; }
   Dee_INHERIT_REF(ast_result->ast_expression.x_ast,expr_ast);
   return ast_result;
  } break;

  case DEE_SASTKIND_BLOCK: {
   DeeSAstObject **new_astv,**iter,**end,**src;
   DeeScopeObject *new_block_scope;
   DEE_ASSERT(self->ast_block.b_astc);
   if ((new_block_scope = (DeeScopeObject *)DeeScope_New(
    (DeeObject *)new_scope,1)) == NULL) return NULL;
   while ((new_astv = (DeeSAstObject **)malloc_nz(
    self->ast_block.b_astc*sizeof(DeeSAstObject *))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    Dee_DECREF(new_block_scope);
    DeeError_NoMemory();
    return NULL;
   }
   end = (iter = new_astv)+self->ast_block.b_astc;
   src = self->ast_block.b_astv;
   while (iter != end) {
    if ((*iter = DeeSAst_CopyWithScope(*src,new_block_scope,lexer)) == NULL) {
err_block_iter: while (iter != new_astv) Dee_DECREF(*--iter);
     free_nn(new_astv);
     Dee_DECREF(new_block_scope);
     return NULL;
    }
    ++iter,++src;
   }
   if ((ast_result = _DeeSAst_NewUnsafe(DEE_SASTKIND_BLOCK,
    self->ast_common.ast_token,DeeParserLabelRefList_Empty)) == NULL) {
    iter = new_astv+self->ast_block.b_astc; goto err_block_iter;
   }
   Dee_INHERIT_REF(ast_result->ast_block.b_scope,new_block_scope);
   ast_result->ast_block.b_astc = self->ast_block.b_astc;
   Dee_INHERIT_REF(ast_result->ast_block.b_astv,new_astv);
   return ast_result;
  } break;

  case DEE_SASTKIND_RETURN: {
   if (!self->ast_return.r_value) goto ret_self;
   if ((expr_ast = DeeXAst_CopyWithScope(self->ast_return.r_value,new_scope,lexer)) == NULL) return NULL;
   if (expr_ast == self->ast_return.r_value) goto decref_expr_and_retself;
   if ((ast_result = _DeeSAst_NewUnsafe(DEE_SASTKIND_RETURN,
    self->ast_common.ast_token,DeeParserLabelRefList_Empty)) == NULL) goto err_expr;
   Dee_INHERIT_REF(ast_result->ast_return.r_value,expr_ast);
   return ast_result;
  } break;

  case DEE_SASTKIND_YIELD: {
   if ((expr_ast = DeeXAst_CopyWithScope(self->ast_yield.y_value,new_scope,lexer)) == NULL) return NULL;
   if (expr_ast == self->ast_yield.y_value) goto decref_expr_and_retself;
   if ((ast_result = _DeeSAst_NewUnsafe(DEE_SASTKIND_YIELD,
    self->ast_common.ast_token,DeeParserLabelRefList_Empty)) == NULL) goto err_expr;
   Dee_INHERIT_REF(ast_result->ast_yield.y_value,expr_ast);
   return ast_result;
  } break;

  case DEE_SASTKIND_ASSERT: {
   DeeXAstObject *message_copy;
   if ((expr_ast = DeeXAst_CopyWithScope(self->ast_assert.a_check,new_scope,lexer)) == NULL) return NULL;
   if (!self->ast_assert.a_message) message_copy = NULL;
   else if ((message_copy = DeeXAst_CopyWithScope(self->ast_assert.a_message,new_scope,lexer)) == NULL) goto err_expr;
   if (expr_ast == self->ast_assert.a_check && message_copy == self->ast_assert.a_message) { Dee_XDECREF(message_copy); goto decref_expr_and_retself; }
   if ((ast_result = _DeeSAst_NewUnsafe(DEE_SASTKIND_ASSERT,self->ast_common.ast_token,DeeParserLabelRefList_Empty)) == NULL) { Dee_XDECREF(message_copy); goto err_expr; }
   Dee_INHERIT_REF(ast_result->ast_assert.a_check,expr_ast);
   Dee_INHERIT_REF(ast_result->ast_assert.a_message,message_copy);
   return ast_result;
  } break;

  case DEE_SASTKIND_PRINT: {
   DeeXAstObject *file_copy;
   if ((expr_ast = DeeXAst_CopyWithScope(self->ast_print.p_data,new_scope,lexer)) == NULL) return NULL;
   if (!self->ast_print.p_file) file_copy = NULL;
   else if ((file_copy = DeeXAst_CopyWithScope(self->ast_print.p_file,new_scope,lexer)) == NULL) goto err_expr;
   if (expr_ast == self->ast_print.p_data && file_copy == self->ast_print.p_file) { Dee_XDECREF(file_copy); goto decref_expr_and_retself; }
   if ((ast_result = _DeeSAst_NewUnsafe(DEE_SASTKIND_PRINT,self->ast_common.ast_token,DeeParserLabelRefList_Empty)) == NULL) { Dee_XDECREF(file_copy); goto err_expr; }
   Dee_INHERIT_REF(ast_result->ast_print.p_file,file_copy);
   Dee_INHERIT_REF(ast_result->ast_print.p_data,expr_ast);
   ast_result->ast_print.p_flags = self->ast_print.p_flags;
   return ast_result;
  } break;

  case DEE_SASTKIND_IF: {
   DEE_ASSERT((self->ast_if.if_succ_block != NULL) == (self->ast_if.if_succ_scope != NULL));
   DEE_ASSERT((self->ast_if.if_fail_block != NULL) == (self->ast_if.if_fail_scope != NULL));
   if ((ast_result = _DeeSAst_NewUnsafe(DEE_SASTKIND_IF,
    self->ast_common.ast_token,DeeParserLabelRefList_Empty)) == NULL) return NULL;
   if ((ast_result->ast_if.if_cond_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)new_scope,1)) == NULL) {
err_r_unsafe: _DeeSAst_FreeUnsafe(ast_result); return NULL;
   }
   if ((ast_result->ast_if.if_cond = DeeXAst_CopyWithScope(
    self->ast_if.if_cond,ast_result->ast_if.if_cond_scope,lexer)) == NULL) {
err_r_if_cond_scope: Dee_DECREF(ast_result->ast_if.if_cond_scope); goto err_r_unsafe;
   }
   if (self->ast_if.if_succ_block) {
    if ((ast_result->ast_if.if_succ_scope = (struct DeeScopeObject *)DeeScope_New((
     DeeObject *)ast_result->ast_if.if_cond_scope,1)) == NULL) {
err_r_if_cond: Dee_DECREF(ast_result->ast_if.if_cond); goto err_r_if_cond_scope;
    }
    if ((ast_result->ast_if.if_succ_block = DeeSAst_CopyWithScope(
     self->ast_if.if_succ_block,ast_result->ast_if.if_succ_scope,lexer)) == NULL) {
     Dee_DECREF(ast_result->ast_if.if_succ_scope); goto err_r_if_cond;
    }
   } else {
    ast_result->ast_if.if_succ_block = NULL;
    ast_result->ast_if.if_succ_scope = NULL;
   }
   if (self->ast_if.if_fail_block) {
    if ((ast_result->ast_if.if_fail_scope = (struct DeeScopeObject *)DeeScope_New((
     DeeObject *)ast_result->ast_if.if_cond_scope,1)) == NULL) {
err_r_if_succ:
     Dee_XDECREF(ast_result->ast_if.if_succ_block);
     Dee_XDECREF(ast_result->ast_if.if_succ_scope);
     goto err_r_if_cond;
    }
    if ((ast_result->ast_if.if_fail_block = DeeSAst_CopyWithScope(
     self->ast_if.if_fail_block,ast_result->ast_if.if_fail_scope,lexer)) == NULL) {
     Dee_DECREF(ast_result->ast_if.if_fail_scope); goto err_r_if_succ;
    }
   } else {
    ast_result->ast_if.if_fail_block = NULL;
    ast_result->ast_if.if_fail_scope = NULL;
   }
   return ast_result;
  } break;

  case DEE_SASTKIND_FOR: {
   DEE_ASSERT(DeeObject_Check(self->ast_for.f_head_init_scope) && DeeScope_Check(self->ast_for.f_head_init_scope));
   DEE_ASSERT(!self->ast_for.f_head_init || (DeeObject_Check(self->ast_for.f_head_init) && DeeXAst_Check(self->ast_for.f_head_init)));
   DEE_ASSERT(DeeObject_Check(self->ast_for.f_head_cond_scope) && DeeScope_Check(self->ast_for.f_head_cond_scope));
   DEE_ASSERT(DeeObject_Check(self->ast_for.f_head_cond) && DeeXAst_Check(self->ast_for.f_head_cond));
   DEE_ASSERT(DeeObject_Check(self->ast_for.f_scope) && DeeScope_Check(self->ast_for.f_scope));
   DEE_ASSERT(!self->ast_for.f_head_next || (DeeObject_Check(self->ast_for.f_head_next) && DeeXAst_Check(self->ast_for.f_head_next)));
   DEE_ASSERT(DeeObject_Check(self->ast_for.f_block) && DeeSAst_Check(self->ast_for.f_block));
   if ((ast_result = _DeeSAst_NewUnsafe(DEE_SASTKIND_FOR,
    self->ast_common.ast_token,DeeParserLabelRefList_Empty)) == NULL) return NULL;
   if ((ast_result->ast_for.f_head_init_scope = (DeeScopeObject *)
    DeeScope_New((DeeObject *)new_scope,1)) == NULL) goto err_r_unsafe;
   if (!self->ast_for.f_head_init) ast_result->ast_for.f_head_init = NULL;
   else if ((ast_result->ast_for.f_head_init = DeeXAst_CopyWithScope(
    self->ast_for.f_head_init,ast_result->ast_for.f_head_init_scope,lexer)) == NULL) {
err_r_for_head_init_scope: Dee_DECREF(ast_result->ast_for.f_head_init_scope); goto err_r_unsafe;
   }
   if ((ast_result->ast_for.f_head_cond_scope = (DeeScopeObject *)
    DeeScope_New((DeeObject *)ast_result->ast_for.f_head_init_scope,1)) == NULL) {
err_r_for_head_init: Dee_XDECREF(ast_result->ast_for.f_head_init); goto err_r_for_head_init_scope;
   }
   if ((ast_result->ast_for.f_head_cond = DeeXAst_CopyWithScope(
    self->ast_for.f_head_cond,ast_result->ast_for.f_head_cond_scope,lexer)) == NULL) {
err_r_for_head_cond_scope: Dee_XDECREF(ast_result->ast_for.f_head_cond_scope); goto err_r_for_head_init;
   }
   if ((ast_result->ast_for.f_scope = (DeeScopeObject *)DeeScope_New((
    DeeObject *)ast_result->ast_for.f_head_cond_scope,1)) == NULL) {
err_r_for_head_cond: Dee_DECREF(ast_result->ast_for.f_head_cond); goto err_r_for_head_cond_scope;
   }
   if (!self->ast_for.f_head_next) ast_result->ast_for.f_head_next = NULL;
   else if ((ast_result->ast_for.f_head_next = DeeXAst_CopyWithScope(
    self->ast_for.f_head_next,ast_result->ast_for.f_scope,lexer)) == NULL) {
err_r_for_scope: Dee_DECREF(ast_result->ast_for.f_scope); goto err_r_for_head_cond;
   }
   if ((ast_result->ast_for.f_block = DeeSAst_CopyWithScope(
    self->ast_for.f_block,ast_result->ast_for.f_scope,lexer)) == NULL) {
    Dee_XDECREF(ast_result->ast_for.f_head_next); goto err_r_for_scope;
   }
   return ast_result;
  } break;

  case DEE_SASTKIND_FORIN:
  case DEE_SASTKIND_FOREACH: {
   if ((ast_result = _DeeSAst_NewUnsafe(self->ast_kind,
    self->ast_common.ast_token,DeeParserLabelRefList_Empty)) == NULL) return NULL;
   if ((ast_result->ast_foreach.fi_seq_scope = (DeeScopeObject *)
    DeeScope_New((DeeObject *)new_scope,1)) == NULL) goto err_r_unsafe;
   if ((ast_result->ast_foreach.fi_seq = DeeXAst_CopyWithScope(
    self->ast_foreach.fi_seq,ast_result->ast_foreach.fi_seq_scope,lexer)) == NULL) {
err_r_foreach_seq_scope: Dee_DECREF(ast_result->ast_foreach.fi_seq_scope); goto err_r_unsafe;
   }
   if ((ast_result->ast_foreach.fi_scope = (DeeScopeObject *)
    DeeScope_New((DeeObject *)ast_result->ast_foreach.fi_seq_scope,1)) == NULL) {
err_r_foreach_seq: Dee_DECREF(ast_result->ast_foreach.fi_seq); goto err_r_foreach_seq_scope;
   }
   if (DeeVarDeclStorage_InitCopyWithScope(&ast_result->ast_foreach.fi_var,
    &self->ast_foreach.fi_var,ast_result->ast_foreach.fi_scope,lexer) < 0) {
err_r_foreach_scope: Dee_DECREF(ast_result->ast_foreach.fi_scope); goto err_r_foreach_seq;
   }
   if ((ast_result->ast_foreach.fi_block = DeeSAst_CopyWithScope(
    self->ast_foreach.fi_block,ast_result->ast_foreach.fi_scope,lexer)) == NULL) {
/*err_r_foreach_var:*/ _DeeVarDeclStorage_Quit(&ast_result->ast_foreach.fi_var); goto err_r_foreach_scope;
   }
   return ast_result;
  } break;

  case DEE_SASTKIND_WHILE:
  case DEE_SASTKIND_DOWHILE: {
   if ((ast_result = _DeeSAst_NewUnsafe(self->ast_kind,
    self->ast_common.ast_token,DeeParserLabelRefList_Empty)) == NULL) return NULL;
   if ((ast_result->ast_while.w_cond_scope = (DeeScopeObject *)
    DeeScope_New((DeeObject *)new_scope,1)) == NULL) goto err_r_unsafe;
   if ((ast_result->ast_while.w_cond = DeeXAst_CopyWithScope(
    self->ast_while.w_cond,ast_result->ast_while.w_cond_scope,lexer)) == NULL) {
err_r_while_cond_scope: Dee_DECREF(ast_result->ast_while.w_cond_scope); goto err_r_unsafe;
   }
   if ((ast_result->ast_while.w_scope = (DeeScopeObject *)
    DeeScope_New((DeeObject *)ast_result->ast_while.w_cond_scope,1)) == NULL) {
err_r_while_cond: Dee_DECREF(ast_result->ast_while.w_cond); goto err_r_while_cond_scope;
   }
   if ((ast_result->ast_while.w_block = DeeSAst_CopyWithScope(
    self->ast_while.w_block,ast_result->ast_while.w_scope,lexer)) == NULL) {
/*err_r_while_scope:*/ Dee_DECREF(ast_result->ast_while.w_scope); goto err_r_while_cond;
   }
   return ast_result;
  } break;

  case DEE_SASTKIND_LOOPEVER:
  case DEE_SASTKIND_LOOPONCE:
  case DEE_SASTKIND_LOOPNONE: {
   if ((ast_result = _DeeSAst_NewUnsafe(self->ast_kind,
    self->ast_common.ast_token,DeeParserLabelRefList_Empty)) == NULL) return NULL;
   if ((ast_result->ast_specialloop.sl_scope = (DeeScopeObject *)
    DeeScope_New((DeeObject *)new_scope,1)) == NULL) goto err_r_unsafe;
   if ((ast_result->ast_specialloop.sl_block = DeeSAst_CopyWithScope(
    self->ast_specialloop.sl_block,ast_result->ast_specialloop.sl_scope,lexer)) == NULL) {
/*err_r_specialloop_scope:*/ Dee_DECREF(ast_result->ast_specialloop.sl_scope); goto err_r_unsafe;
   }
   return ast_result;
  } break;

  case DEE_SASTKIND_GOTO: {
   if (DeeError_CompilerError(DEE_WARNING_CANT_COPY_GOTO_IN_ALIAS_DECLARATION,
    (DeeObject *)lexer,(DeeObject *)self->ast_common.ast_token,
    "Can't copy goto statement in alias reference") != 0) return NULL;
ret_none: return DeeSAst_NewEmpty(self->ast_common.ast_token,DeeParserLabelRefList_Empty);
  } break;

  case DEE_SASTKIND_MODULE: {
   if (DeeError_CompilerError(DEE_WARNING_CANT_COPY_MODULE_IN_ALIAS_DECLARATION,
    (DeeObject *)lexer,(DeeObject *)self->ast_common.ast_token,
    "Can't copy module statement in alias reference") != 0) return NULL;
   goto ret_none;
  } break;

  case DEE_SASTKIND_THROW: {
   if (!self->ast_throw.t_object) goto ret_self;
   if ((expr_ast = DeeXAst_CopyWithScope(self->ast_throw.t_object,new_scope,lexer)) == NULL) return NULL;
   if (expr_ast == self->ast_throw.t_object) goto decref_expr_and_retself;
   if ((ast_result = _DeeSAst_NewUnsafe(DEE_SASTKIND_THROW,
    self->ast_common.ast_token,DeeParserLabelRefList_Empty)) == NULL) goto err_expr;
   Dee_INHERIT_REF(ast_result->ast_throw.t_object,expr_ast);
   return ast_result;
  } break;

  case DEE_SASTKIND_TRY: {
   struct DeeSAstTryHandler *iter,*end,*src;
   if ((ast_result = _DeeSAst_NewUnsafe(DEE_SASTKIND_TRY,
    self->ast_common.ast_token,DeeParserLabelRefList_Empty)) == NULL) return NULL;
   if ((ast_result->ast_try.t_rootscope = (DeeScopeObject *)
    DeeScope_New((DeeObject *)new_scope,1)) == NULL) goto err_r_unsafe;
   if ((ast_result->ast_try.t_scope = (DeeScopeObject *)
    DeeScope_New((DeeObject *)ast_result->ast_try.t_rootscope,1)) == NULL) {
err_r_try_rootscope: Dee_DECREF(ast_result->ast_try.t_rootscope); goto err_r_unsafe;
   }
   if ((ast_result->ast_try.t_block = DeeSAst_CopyWithScope(
    self->ast_try.t_block,new_scope,lexer)) == NULL) {
err_r_try_scope: Dee_DECREF(ast_result->ast_try.t_scope); goto err_r_try_rootscope;
   }
   DEE_ASSERT(self->ast_try.t_handlerc);
   while ((ast_result->ast_try.t_handlerv = (struct DeeSAstTryHandler *)malloc_nz(
    self->ast_try.t_handlerc*sizeof(struct DeeSAstTryHandler))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
err_r_try_block: Dee_DECREF(ast_result->ast_try.t_block);
    goto err_r_try_scope;
   }
   ast_result->ast_try.t_handlerc = self->ast_try.t_handlerc;
   end = (iter = ast_result->ast_try.t_handlerv)+ast_result->ast_try.t_handlerc;
   src = self->ast_try.t_handlerv;
   do {
    if (DeeSAstTryHandler_InitCopyWithScope(
     iter,src,ast_result->ast_try.t_rootscope,lexer) < 0) {
/*err_r_try_handler_iter:*/
     while (iter != ast_result->ast_try.t_handlerv) { --iter; _DeeSAstTryHandler_Quit(iter); }
     free(ast_result->ast_try.t_handlerv);
     goto err_r_try_block;
    }
    ++iter,++src;
   } while (iter != end);
   return ast_result;
  } break;

  case DEE_SASTKIND_SWITCH: {
   if ((ast_result = _DeeSAst_NewUnsafe(DEE_SASTKIND_SWITCH,
    self->ast_common.ast_token,DeeParserLabelRefList_Empty)) == NULL) return NULL;
   if ((ast_result->ast_switch.sw_expr_scope = (DeeScopeObject *)
    DeeScope_New((DeeObject *)new_scope,1)) == NULL) goto err_r_unsafe;
   if ((ast_result->ast_switch.sw_expr = DeeXAst_CopyWithScope(
    self->ast_switch.sw_expr,ast_result->ast_switch.sw_expr_scope,lexer)) == NULL) {
err_r_switch_expr_scope: Dee_DECREF(ast_result->ast_switch.sw_expr_scope); goto err_r_unsafe;
   }
   if ((ast_result->ast_switch.sw_scope = (DeeScopeObject *)
    DeeScope_New((DeeObject *)ast_result->ast_switch.sw_expr_scope,1)) == NULL) {
err_r_switch_expr: Dee_DECREF(ast_result->ast_switch.sw_expr); goto err_r_switch_expr_scope;
   }
   if ((ast_result->ast_switch.sw_block = DeeSAst_CopyWithScope(
    self->ast_switch.sw_block,ast_result->ast_switch.sw_scope,lexer)) == NULL) {
/*err_r_switch_scope:*/ Dee_DECREF(ast_result->ast_switch.sw_scope); goto err_r_switch_expr;
   }
   _DeeParserLabelList_Init(&ast_result->ast_switch.sw_cases);
   return ast_result;
  } break;

  case DEE_SASTKIND_IFTRUE:
  case DEE_SASTKIND_IFFALSE: {
   if (!self->ast_ifconst.ic_succ_block &&
       !self->ast_ifconst.ic_fail_block) goto ret_self;
   if ((ast_result = _DeeSAst_NewUnsafe(self->ast_kind,
    self->ast_common.ast_token,DeeParserLabelRefList_Empty)) == NULL) return NULL;
   if (self->ast_ifconst.ic_succ_block) {
    if ((ast_result->ast_ifconst.ic_succ_scope = (DeeScopeObject *)
     DeeScope_New((DeeObject *)new_scope,1)) == NULL) goto err_r_unsafe;
    if ((ast_result->ast_ifconst.ic_succ_block = DeeSAst_CopyWithScope(
     self->ast_ifconst.ic_succ_block,ast_result->ast_ifconst.ic_succ_scope,lexer)) == NULL) {
err_r_ifc_succ_scope: Dee_XDECREF(ast_result->ast_ifconst.ic_succ_scope); goto err_r_unsafe;
    }
   }
   if (self->ast_ifconst.ic_fail_block) {
    if ((ast_result->ast_ifconst.ic_fail_scope = (DeeScopeObject *)
     DeeScope_New((DeeObject *)new_scope,1)) == NULL) {
err_r_ifc_succ_block: Dee_XDECREF(ast_result->ast_ifconst.ic_succ_block); goto err_r_ifc_succ_scope;
     goto err_r_unsafe;
    }
    if ((ast_result->ast_ifconst.ic_fail_block = DeeSAst_CopyWithScope(
     self->ast_ifconst.ic_fail_block,ast_result->ast_ifconst.ic_fail_scope,lexer)) == NULL) {
/*err_r_ifc_fail_scope:*/ Dee_XDECREF(ast_result->ast_ifconst.ic_fail_scope); goto err_r_ifc_succ_block;
    }
   }
   return ast_result;
  } break;

  default: break;
 }
 // Unsupported type (shouldn't happen, but might once I forget smth)
 DeeError_NotImplementedf("Can't copy SAst: %r",self);
 return NULL;
}


DEE_A_RET_EXCEPT(-1) int DeeSAstTryHandler_InitCopyWithScope(
 DEE_A_OUT struct DeeSAstTryHandler *self, DEE_A_IN struct DeeSAstTryHandler const *right,
 DEE_A_INOUT DeeScopeObject *new_scope, DEE_A_INOUT DeeLexerObject *lexer) {
 int temp;
 DEE_ASSERT(self);
 DEE_ASSERT(right);
 DEE_ASSERT(DeeObject_Check(new_scope) && DeeScope_Check(new_scope));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 if ((self->th_scope = (DeeScopeObject *)DeeScope_New((
  DeeObject *)new_scope,1)) == NULL) return -1;
 if ((temp = DeeVarDeclStorage_InitCopyWithScope(
  &self->th_local,&right->th_local,self->th_scope,lexer)) < 0) {
err_scope: Dee_DECREF(self->th_scope); return -1;
 }
 if ((self->th_block = DeeSAst_CopyWithScope(right->th_block,self->th_scope,lexer)) == NULL) {
/*err_local:*/ _DeeVarDeclStorage_Quit(&self->th_local); goto err_scope;
 }
 self->th_kind = right->th_kind;
 Dee_XINCREF(self->th_type = right->th_type);
 // v make sure it really is a reference copy
 return temp ? self->th_block == right->th_block : 0;
}


DEE_DECL_END

