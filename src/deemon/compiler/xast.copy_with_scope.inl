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
#include "xast.c"
#endif

DEE_DECL_BEGIN

DEE_COMPILER_MSVC_WARNING_PUSH(4701 4702 4703)
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_CopyWithScope(
 DEE_A_INOUT DeeXAstObject *self, DEE_A_INOUT DeeScopeObject *new_scope,
 DEE_A_INOUT DeeLexerObject *lexer) {
 DeeXAstObject *ast_result,*ast_a,*ast_b,*ast_c,*ast_d;
 DEE_ASSERT(DeeObject_Check(self) && DeeXAst_Check(self));
 DEE_ASSERT(DeeObject_Check(new_scope) && DeeScope_Check(new_scope));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 switch (self->ast_kind) {
  case DEE_XASTKIND_NONE:
  case DEE_XASTKIND_CONST: goto ret_self;
  case DEE_XASTKIND_MODULE:
   DEE_ASSERT(self->ast_module.m_module->sc_weak_prev);
   if (DeeScope_IsSameOrParent((DeeObject *)
    self->ast_module.m_module->sc_weak_prev,(DeeObject *)new_scope)) goto ret_self;
   goto unreachable_var;
  case DEE_XASTKIND_VAR: {
   DeeScopeObject *actual_var_scope = DeeScope_MODULE_ROOT(self->ast_var.vs_var->lv_scope);
   if (DeeScope_IsSameOrWeakParent((DeeObject *)actual_var_scope,(DeeObject *)new_scope)) goto ret_self;
   if (DeeScope_IsSameOrParent((DeeObject *)actual_var_scope,(DeeObject *)new_scope)) {
    (DeeScope_WEAK_ROOT(new_scope))->sc_flags |= DEE_SCOPE_FLAG_FOUND_REFS;
    goto ret_self;
   }
unreachable_var:
   if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_CANT_ACCESS_VAR_FROM_UNREACHABLE_SCOPE,
    (DeeObject *)lexer,(DeeObject *)self->ast_common.ast_token,
    "Can't access variable form unreachable scope") != 0) return NULL;
ret_none:
   return DeeXAst_NewConst(self->ast_common.ast_token,Dee_None);
  }

  case DEE_XASTKIND_VARDECL: {
   DeeScopeObject *actual_var_scope;
   if DEE_UNLIKELY((ast_a = DeeXAst_CopyWithScope(self->ast_vardecl.vd_init,new_scope,lexer)) == NULL) return NULL;
   actual_var_scope = DeeScope_MODULE_ROOT(self->ast_vardecl.vd_var->lv_scope);
   if DEE_LIKELY(DeeScope_IsSameOrWeakParent((DeeObject *)actual_var_scope,(DeeObject *)new_scope)) {
    if (ast_a == self->ast_vardecl.vd_init) { Dee_DECREF(ast_a); goto ret_self; }
    if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(DEE_XASTKIND_VARDECL,self->ast_common.ast_token)) == NULL) {
err_ast_a: Dee_DECREF(ast_a); return NULL;
   }
    Dee_INHERIT_REF(ast_result->ast_vardecl.vd_init,ast_a);
    Dee_INCREF(ast_result->ast_vardecl.vd_token = self->ast_vardecl.vd_token);
    Dee_INCREF(ast_result->ast_vardecl.vd_var = self->ast_vardecl.vd_var);
    return ast_result;
   } 
   if (DeeScope_IsSameOrParent((DeeObject *)actual_var_scope,(DeeObject *)new_scope)) {
    if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_CANT_STORE_VAR_FROM_DIFFERENT_SCOPE,
     (DeeObject *)lexer,(DeeObject *)self->ast_common.ast_token,
     "Store to variable from different strong scope will be compiled as assign:\n"
     "\t%>K := %>r",_DeeLocalVar_ReprWithToken(
     self->ast_vardecl.vd_var,self->ast_vardecl.vd_token),
     self->ast_vardecl.vd_init) != 0) goto err_ast_a;
    if DEE_UNLIKELY((ast_b = _DeeXAst_NewUnsafe(DEE_XASTKIND_VAR,self->ast_common.ast_token)) == NULL) goto err_ast_a;
    DeeScope_WEAK_ROOT(new_scope)->sc_flags |= DEE_SCOPE_FLAG_FOUND_REFS;
    Dee_INCREF(ast_b->ast_var.vs_var = self->ast_vardecl.vd_var);
    DeeLocalVar_ADD_USE(ast_b->ast_var.vs_var);
    if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(
     DEE_XASTKIND_COPY_ASSIGN,self->ast_common.ast_token)) == NULL) {
 /*err_ast_ab:*/ Dee_DECREF(ast_b); goto err_ast_a;
    }
    Dee_INHERIT_REF(ast_result->ast_operator.op_a,ast_b);
    Dee_INHERIT_REF(ast_result->ast_operator.op_b,ast_a);
    return ast_result;
   }
   if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_CANT_ACCESS_VAR_FROM_UNREACHABLE_SCOPE,
    (DeeObject *)lexer,(DeeObject *)self->ast_vardecl.vd_token,
    "Can't access variable form unreachable scope") != 0) goto err_ast_a;
   return ast_a;
  } break;

  case DEE_XASTKIND_MULTIVARDECL: {
   int temp,can_reference = 1;
   DeeXAstObject *init_copy;
   struct DeeVarDeclStorage *new_storage,*iter,*end,*src;
   DEE_ASSERT(self->ast_multivardecl.mvd_varc);
   if DEE_UNLIKELY((init_copy = DeeXAst_CopyWithScope(
    self->ast_multivardecl.mvd_init,new_scope,lexer)
    ) == NULL) return NULL;
   while DEE_UNLIKELY((new_storage = (struct DeeVarDeclStorage *)malloc_nz(
    self->ast_multivardecl.mvd_varc*sizeof(struct DeeVarDeclStorage))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    Dee_DECREF(init_copy);
    DeeError_NoMemory();
    return NULL;
   }
   end = (iter = new_storage)+self->ast_multivardecl.mvd_varc;
   src = self->ast_multivardecl.mvd_varv;
   do {
    if DEE_UNLIKELY((temp = DeeVarDeclStorage_InitCopyWithScope(iter,src,new_scope,lexer)) < 0) {
err_mvd_iter:
     while (iter != new_storage) { --iter; DeeVarDeclStorage_Quit(iter); }
     free_nn(new_storage);
     Dee_DECREF(init_copy);
     return NULL;
    }
    if (!temp) can_reference = 0; // Can't use a reference copy
    ++iter,++src;
   } while (iter != end);
   if (init_copy == self->ast_multivardecl.mvd_init && can_reference) {
    while (iter != new_storage) { --iter; _DeeVarDeclStorage_Quit(iter); }
    free_nn(new_storage);
    Dee_DECREF(init_copy);
    goto ret_self;
   }
   if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(DEE_XASTKIND_MULTIVARDECL,self->ast_common.ast_token)) == NULL) {
    iter = new_storage+self->ast_multivardecl.mvd_varc;
    goto err_mvd_iter;
   }
   ast_result->ast_multivardecl.mvd_type = self->ast_multivardecl.mvd_type;
   ast_result->ast_multivardecl.mvd_varc = self->ast_multivardecl.mvd_varc;
   ast_result->ast_multivardecl.mvd_varv = new_storage;
   Dee_INHERIT_REF(ast_result->ast_multivardecl.mvd_init,init_copy);
   return ast_result;
  } break;

  case DEE_XASTKIND_STATEMENT: {
   DeeSAstObject *new_statements;
   if DEE_UNLIKELY((new_statements = DeeSAst_CopyWithScope(
    self->ast_statement.s_stmt,new_scope,lexer)) == NULL) return NULL;
   if (new_statements == self->ast_statement.s_stmt) { Dee_DECREF(new_statements); goto ret_self; }
   if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(DEE_XASTKIND_STATEMENT,self->ast_common.ast_token)) == NULL) {
    Dee_DECREF(new_statements); return NULL;
   }
   Dee_INHERIT_REF(ast_result->ast_statement.s_stmt,new_statements);
   return ast_result;
  } break;

  case DEE_XASTKIND_IF: {
   if DEE_UNLIKELY((ast_a = DeeXAst_CopyWithScope(self->ast_if.if_cond,new_scope,lexer)) == NULL) return NULL;
   if (!self->ast_if.if_succ) ast_b = NULL;
   else if DEE_UNLIKELY((ast_b = DeeXAst_CopyWithScope(self->ast_if.if_succ,new_scope,lexer)) == NULL) goto err_ast_a;
   if (!self->ast_if.if_fail) ast_c = NULL;
   else if DEE_UNLIKELY((ast_c = DeeXAst_CopyWithScope(self->ast_if.if_fail,new_scope,lexer)) == NULL) {/*err_ast_xba:*/ Dee_XDECREF(ast_b); goto err_ast_a; }
   if (ast_a == self->ast_if.if_cond &&
       ast_b == self->ast_if.if_succ &&
       ast_c == self->ast_if.if_fail) {
    Dee_INCREF(ast_result = self); // Conserve memory
end_xcxba:
    Dee_XDECREF(ast_c);
    Dee_XDECREF(ast_b);
    Dee_DECREF(ast_a);
   } else {
    if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(DEE_XASTKIND_IF,
     self->ast_common.ast_token)) == NULL) goto end_xcxba;
    Dee_INHERIT_REF(ast_result->ast_if.if_cond,ast_a);
    Dee_INHERIT_REF(ast_result->ast_if.if_succ,ast_b);
    Dee_INHERIT_REF(ast_result->ast_if.if_fail,ast_c);
   }
   return ast_result;
  } break;

  case DEE_XASTKIND_FUNCTION: {
   DeeScopeObject *new_function_scope;
   struct DeeXAstFunctionAstArgument *new_argv,*iter,*end,*src;
   DeeSAstObject *new_function_code;
   if (self->ast_function.f_scope) {
    DEE_ASSERT(DeeScope_IS_STRONG(self->ast_function.f_scope));
    // Function scope parent is equivalent to us, so we can simply re-use the AST
    if (self->ast_function.f_scope->sc_prev == new_scope) goto ret_self;
   }
   if DEE_UNLIKELY((new_function_scope = (DeeScopeObject *)
    DeeScope_New((DeeObject *)new_scope,0)) == NULL) return NULL;
   if (self->ast_function.f_argc) {
    while DEE_UNLIKELY((new_argv = (struct DeeXAstFunctionAstArgument *)malloc_nz(
     self->ast_function.f_argc*sizeof(struct DeeXAstFunctionAstArgument))) == NULL) {
     if DEE_LIKELY(Dee_CollectMemory()) continue;
     Dee_DECREF(new_function_scope);
     DeeError_NoMemory();
     return NULL;
    }
    src = self->ast_function.f_argv;
    end = (iter = new_argv)+self->ast_function.f_argc;
    while (iter != end) {
     if (src->a_default) {
      if DEE_UNLIKELY((iter->a_default = DeeXAst_CopyWithScope(
       src->a_default,new_function_scope,lexer)) == NULL) {
err_fun_argiter:
       while (iter != new_argv) { --iter; _DeeXAstFunctionAstArgument_Quit(iter); }
       free(new_argv);
err_new_function_scope:
       Dee_DECREF(new_function_scope);
       return NULL;
      }
     } else iter->a_default = NULL;
     if DEE_UNLIKELY((iter->a_var = (DeeLocalVarObject *)DeeLocalVar_New(
      src->a_var->lv_name,(DeeObject *)new_function_scope)) == NULL) {
      Dee_XDECREF(iter->a_default);
      goto err_fun_argiter;
     }
     Dee_INCREF(iter->a_token = src->a_token);
     ++iter,++src;
    }
   } else new_argv = NULL;
   if (self->ast_function.f_code) {
    if DEE_UNLIKELY((new_function_code = DeeSAst_CopyWithScope(
     self->ast_function.f_code,new_function_scope,lexer)) == NULL) {
/*err_fun_argv:*/
     if (!new_argv) goto err_new_function_scope;
     iter = new_argv+self->ast_function.f_argc;
     goto err_fun_argiter;
    }
   } else new_function_code = NULL;
   if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(DEE_XASTKIND_FUNCTION,self->ast_common.ast_token)) == NULL)
   { Dee_XDECREF(new_function_code); Dee_DECREF(new_function_scope); return NULL; }
   Dee_XINCREF(ast_result->ast_function.f_name = self->ast_function.f_name);
   ast_result->ast_function.f_flags = self->ast_function.f_flags;
   ast_result->ast_function.f_argc = self->ast_function.f_argc;
   if (!self->ast_function.f_argc && !new_function_code)
    Dee_CLEAR(new_function_scope); // Never used
   Dee_INHERIT_REF(ast_result->ast_function.f_argv,new_argv);
   Dee_INHERIT_REF(ast_result->ast_function.f_code,new_function_code);
   Dee_INHERIT_REF(ast_result->ast_function.f_scope,new_function_scope);
   return ast_result;
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case DEE_XASTKIND_CLASS: {
   struct DeeXAstClassAstEntry *new_entryv,*src,*iter,*end;
   DeeXAstObject *base_copy;
   int temp,can_reference = 1;
   if (self->ast_class.c_entryc) {
    while DEE_UNLIKELY((new_entryv = (struct DeeXAstClassAstEntry *)malloc_nz(
     self->ast_class.c_entryc*sizeof(struct DeeXAstClassAstEntry))) == NULL) {
     if DEE_LIKELY(Dee_CollectMemory()) continue;
     DeeError_NoMemory();
     return NULL;
    }
    end = (iter = new_entryv)+self->ast_class.c_entryc;
    src = self->ast_class.c_entryv;
    while (iter != end) {
     if DEE_UNLIKELY((temp = DeeXAstClassAstEntry_InitCopyWithScope(
      iter,src,new_scope,lexer)) < 0) {
err_classentry_iter:
      while (iter != new_entryv) { --iter; _DeeXAstClassAstEntry_Quit(iter); }
      free_nn(new_entryv);
      return NULL;
     }
     if (temp) can_reference = 0;
     ++iter,++src;
    }
   } else new_entryv = NULL;
   if (self->ast_class.c_base) {
    if DEE_UNLIKELY((base_copy = DeeXAst_CopyWithScope(
     self->ast_class.c_base,new_scope,lexer)) == NULL) {
     if (new_entryv) {
err_classentryv:
      iter = new_entryv+self->ast_class.c_entryc;
      goto err_classentry_iter;
     }
     return NULL;
    }
   } else base_copy = NULL;
   if (can_reference && base_copy == self->ast_class.c_base) {
    // Return as reference copy
    Dee_XDECREF(base_copy);
    if (new_entryv) {
     end = (iter = new_entryv)+self->ast_class.c_entryc;
     do _DeeXAstClassAstEntry_Quit(iter);
     while (++iter != end);
     free_nn(new_entryv);
    }
    goto ret_self;
   }
   if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(
    DEE_XASTKIND_CLASS,self->ast_common.ast_token)) == NULL) {
    Dee_XDECREF(base_copy);
    if (new_entryv) goto err_classentryv;
    return NULL;
   }
   Dee_INHERIT_REF(ast_result->ast_class.c_base,base_copy);
   ast_result->ast_class.c_entryc = self->ast_class.c_entryc;
   ast_result->ast_class.c_entryv = new_entryv;
   Dee_XINCREF(ast_result->ast_class.c_name = self->ast_class.c_name);
   return ast_result;
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */

  case DEE_XASTKIND_TUPLE:
  case DEE_XASTKIND_LIST:
  case DEE_XASTKIND_SET: {
   DeeXAstObject **new_elem,**iter,**end,**src;
   int can_reference = 1;
   DEE_ASSERT(self->ast_sequence.s_elemc);
   while DEE_UNLIKELY((new_elem = (DeeXAstObject **)malloc_nz(
    self->ast_sequence.s_elemc*sizeof(DeeXAstObject *))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
    return NULL;
   }
   end = (iter = new_elem)+self->ast_sequence.s_elemc;
   src = self->ast_sequence.s_elemv;
   do {
    if DEE_UNLIKELY((*iter = DeeXAst_CopyWithScope(
     *src,new_scope,lexer)) == NULL) {
err_seq_iter:
     while (iter != new_elem) Dee_DECREF(*--iter);
     free_nn(new_elem);
     return NULL;
    }
    if (*iter != *src) can_reference = 0;
    ++iter,++src;
   } while (iter != end);
   if (can_reference) {
    end = (iter = new_elem)+self->ast_sequence.s_elemc;
    DEE_ASSERT(self->ast_sequence.s_elemc);
    do Dee_DECREF(*iter);
    while (++iter != end);
    free_nn(new_elem);
    goto ret_self;
   }
   if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(
    self->ast_kind,self->ast_common.ast_token)) == NULL) {
    iter = new_elem+self->ast_sequence.s_elemc;
    goto err_seq_iter;
   }
   ast_result->ast_sequence.s_elemc = self->ast_sequence.s_elemc;
   ast_result->ast_sequence.s_elemv = new_elem;
   return ast_result;
  } break;

  case DEE_XASTKIND_DICT: {
   struct DeeXAstDictEntry *new_elem,*iter,*end,*src;
   int can_reference = 1;
   DEE_ASSERT(self->ast_dict.d_elemv != NULL);
   DEE_ASSERT(self->ast_dict.d_elemc != 0);
   while DEE_UNLIKELY((new_elem = (struct DeeXAstDictEntry *)malloc_nz(
    self->ast_dict.d_elemc*sizeof(struct DeeXAstDictEntry))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
    return NULL;
   }
   end = (iter = new_elem)+self->ast_dict.d_elemc;
   src = self->ast_dict.d_elemv;
   do {
    if DEE_UNLIKELY((iter->de_key = DeeXAst_CopyWithScope(
     src->de_key,new_scope,lexer)) == NULL) {
err_dict_iter:
     while (iter != new_elem) { --iter; _DeeXAstDictEntry_Quit(iter); }
     free_nn(new_elem);
     return NULL;
    }
    if DEE_UNLIKELY((iter->de_item = DeeXAst_CopyWithScope(
     src->de_item,new_scope,lexer)) == NULL) {
     Dee_DECREF(iter->de_key);
     goto err_dict_iter;
    }
    if (iter->de_key != src->de_key ||
        iter->de_item != src->de_item) can_reference = 0;
    ++iter,++src;
   } while (iter != end);
   if (can_reference) {
    end = (iter = new_elem)+self->ast_dict.d_elemc;
    DEE_ASSERT(self->ast_dict.d_elemc);
    do _DeeXAstDictEntry_Quit(iter);
    while (++iter != end);
    free(new_elem);
    goto ret_self;
   }
   if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(
    DEE_XASTKIND_DICT,self->ast_common.ast_token)) == NULL) {
    iter = new_elem+self->ast_dict.d_elemc;
    goto err_dict_iter;
   }
   ast_result->ast_dict.d_elemc = self->ast_dict.d_elemc;
   ast_result->ast_dict.d_elemv = new_elem;
   return ast_result;
  } break;

  case DEE_XASTKIND_SEQ_RANGE_GET:
  case DEE_XASTKIND_SEQ_RANGE_DEL: { ast_d = NULL;
   if (0) { case DEE_XASTKIND_SEQ_RANGE_SET: if DEE_UNLIKELY((ast_d = DeeXAst_CopyWithScope(self->ast_seq_range_set.sr_value,new_scope,lexer)) == NULL) return NULL; }
   if DEE_UNLIKELY((ast_a = DeeXAst_CopyWithScope(self->ast_seq_range.sr_seq,new_scope,lexer)) == NULL) {err_xd: Dee_XDECREF(ast_d); return NULL; }
   if (!self->ast_seq_range.sr_begin) ast_b = NULL;
   else if DEE_UNLIKELY((ast_b = DeeXAst_CopyWithScope(self->ast_seq_range.sr_begin,new_scope,lexer)) == NULL) {err_axd: Dee_DECREF(ast_a); goto err_xd; }
   if (!self->ast_seq_range.sr_end) ast_c = NULL;
   else if DEE_UNLIKELY((ast_c = DeeXAst_CopyWithScope(self->ast_seq_range.sr_end,new_scope,lexer)) == NULL) {err_xbaxd: Dee_XDECREF(ast_b); goto err_axd; }
   if (ast_a == self->ast_seq_range.sr_seq &&
       ast_b == self->ast_seq_range.sr_begin &&
       ast_c == self->ast_seq_range.sr_end &&
      (self->ast_kind != DEE_XASTKIND_SEQ_RANGE_SET ||
       ast_d == self->ast_seq_range.sr_value)) {
    Dee_XDECREF(ast_c);
    Dee_XDECREF(ast_b);
    Dee_DECREF(ast_a);
    Dee_XDECREF(ast_d);
    goto ret_self;
   }
   if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(self->ast_kind,self->ast_common.ast_token)) == NULL) {/*err_xcxbaxd:*/ Dee_XDECREF(ast_c); goto err_xbaxd; }
   Dee_INHERIT_REF(ast_result->ast_seq_range.sr_seq,ast_a);
   Dee_INHERIT_REF(ast_result->ast_seq_range.sr_begin,ast_b);
   Dee_INHERIT_REF(ast_result->ast_seq_range.sr_end,ast_c);
   Dee_INHERIT_REF(ast_result->ast_seq_range.sr_value,ast_d);
   return ast_result;
  } break;

  case DEE_XASTKIND_ATTR_GET_C:
  case DEE_XASTKIND_ATTR_HAS_C:
  case DEE_XASTKIND_ATTR_DEL_C: {
   if DEE_UNLIKELY((ast_a = DeeXAst_CopyWithScope(self->ast_attr_c.ac_object,new_scope,lexer)) == NULL) return NULL;
   if (ast_a == self->ast_attr_c.ac_object) { Dee_DECREF(ast_a); goto ret_self; }
   if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(self->ast_kind,self->ast_common.ast_token)) == NULL) goto err_a;
   Dee_INHERIT_REF(ast_result->ast_attr_c.ac_object,ast_a);
   Dee_INCREF(ast_result->ast_attr_c.ac_name = self->ast_attr_c.ac_name);
   return ast_result;
  } break;

  case DEE_XASTKIND_ATTR_SET_C: {
   if DEE_UNLIKELY((ast_a = DeeXAst_CopyWithScope(self->ast_attr_set_c.ac_object,new_scope,lexer)) == NULL) return NULL;
   if DEE_UNLIKELY((ast_b = DeeXAst_CopyWithScope(self->ast_attr_set_c.ac_value,new_scope,lexer)) == NULL) { err_a: Dee_DECREF(ast_a); return NULL; }
   if (ast_a == self->ast_attr_set_c.ac_object &&
       ast_b == self->ast_attr_set_c.ac_value) {
    Dee_DECREF(ast_a);
    Dee_DECREF(ast_b);
    goto ret_self;
   }
   if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(
    DEE_XASTKIND_ATTR_SET_C,self->ast_common.ast_token)) == NULL) {
/*err_ba:*/ Dee_DECREF(ast_b); goto err_a;
   }
   Dee_INHERIT_REF(ast_result->ast_attr_set_c.ac_object,ast_a);
   Dee_INHERIT_REF(ast_result->ast_attr_set_c.ac_value,ast_b);
   Dee_INCREF(ast_result->ast_attr_set_c.ac_name = self->ast_attr_set_c.ac_name);
   return ast_result;
  } break;

  case DEE_XASTKIND_DEL_VAR: {
   DeeScopeObject *actual_var_scope = DeeScope_MODULE_ROOT(self->ast_delvar.d_var->lv_scope);
   if (DeeLocalVar_KIND(self->ast_delvar.d_var) == DEE_LOCALVAR_KIND_STACK) {
    if DEE_UNLIKELY(actual_var_scope != new_scope) {
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_CANT_DEL_STACK_VARIABLE_FROM_DIFFERENT_STRONG_SCOPE,
      (DeeObject *)lexer,(DeeObject *)self->ast_common.ast_token,
      "Can't del __stack variable from different scope") != 0) return NULL;
     goto ret_none;
    }
   } else {
    if (DeeScope_IsSameOrWeakParent((DeeObject *)actual_var_scope,(DeeObject *)new_scope)) goto ret_self;
    if DEE_UNLIKELY(DeeScope_IsSameOrParent((DeeObject *)actual_var_scope,(DeeObject *)new_scope)) {
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_CANT_DEL_VARIABLE_FROM_DIFFERENT_STRONG_SCOPE,
      (DeeObject *)lexer,(DeeObject *)self->ast_common.ast_token,
      "Can't del variable form different scope") != 0) return NULL;
     goto ret_none;
    }
   }
   goto unreachable_var;
  }

#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
  case DEE_XASTKIND_FOREIGN_FUNCTION: {
   DeeXAstObject *new_base,**new_argv,**iter,**end,**src;
   int can_reference = 1;
   if (self->ast_foreignfunction.ff_argc) {
    while DEE_UNLIKELY((new_argv = (DeeXAstObject **)malloc_nz(
     self->ast_foreignfunction.ff_argc*sizeof(DeeXAstObject *))) == NULL) {
     DeeError_NoMemory();
     return NULL;
    }
    src = self->ast_foreignfunction.ff_argv;
    end = (iter = new_argv)+self->ast_foreignfunction.ff_argc;
    while (iter != end) {
     if DEE_UNLIKELY((*iter = DeeXAst_CopyWithScope(*src,new_scope,lexer)) == NULL) {
err_ff_iter:
      while (iter != new_argv) Dee_DECREF(*--iter);
      free_nn(new_argv);
      return NULL;
     }
     if (*iter != *src) can_reference = 0;
     ++iter,++src;
    }
   } else new_argv = NULL;
   if DEE_UNLIKELY((new_base = DeeXAst_CopyWithScope(
    self->ast_foreignfunction.ff_base,new_scope,lexer)) == NULL) {
err_ff_argv:
    iter = new_argv+self->ast_foreignfunction.ff_argc;
    goto err_ff_iter;
   }
   if (can_reference && new_base == self->ast_foreignfunction.ff_base) {
    end = (iter = new_argv)+self->ast_foreignfunction.ff_argc;
    do Dee_DECREF(*iter);
    while (++iter != end);
    free_nn(new_argv);
    Dee_DECREF(new_base);
    goto ret_self;
   }
   if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(
    DEE_XASTKIND_FOREIGN_FUNCTION,self->ast_common.ast_token)) == NULL) {
    Dee_DECREF(new_argv); goto err_ff_argv;
   }
   Dee_INHERIT_REF(ast_result->ast_foreignfunction.ff_base,new_base);
   ast_result->ast_foreignfunction.ff_flags = self->ast_foreignfunction.ff_flags;
   ast_result->ast_foreignfunction.ff_argc = self->ast_foreignfunction.ff_argc;
   Dee_INHERIT_REF(ast_result->ast_foreignfunction.ff_argv,new_argv);
   return ast_result;
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */

  case DEE_XASTKIND_SWITCH: {
   struct DeeXAstSwitchEntry *iter,*end,*my_entryv,*src;
   int can_reference;
   if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(DEE_XASTKIND_SWITCH,self->ast_common.ast_token)) == NULL) return NULL;
   if DEE_UNLIKELY((ast_result->ast_switch.s_expr = DeeXAst_CopyWithScope(self->ast_switch.s_expr,new_scope,lexer)) == NULL) {
err_r_unsafe: _DeeXAst_FreeUnsafe(ast_result); return NULL;
   }
   if (self->ast_switch.s_default) {
    if DEE_UNLIKELY((ast_result->ast_switch.s_default = DeeXAst_CopyWithScope(
     self->ast_switch.s_default,new_scope,lexer)) == NULL) {
/*err_r_switch_default:*/ Dee_XDECREF(ast_result->ast_switch.s_default); goto err_r_unsafe;
    }
    if (ast_result->ast_switch.s_default != self->ast_switch.s_default) can_reference = 0;
   } else ast_result->ast_switch.s_default = NULL;
   can_reference = ast_result->ast_switch.s_expr == self->ast_switch.s_expr;
   ast_result->ast_switch.s_entryc = self->ast_switch.s_entryc;
   DEE_ASSERT(self->ast_switch.s_entryc);
   while DEE_UNLIKELY((my_entryv = (struct DeeXAstSwitchEntry *)malloc_nz(
    self->ast_switch.s_entryc*sizeof(struct DeeXAstSwitchEntry))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
    goto err_r_unsafe;
   }
   ast_result->ast_switch.s_entryv = my_entryv;
   src = self->ast_switch.s_entryv;
   end = (iter = my_entryv)+self->ast_switch.s_entryc;
   do {
    if DEE_UNLIKELY((iter->se_value = DeeXAst_CopyWithScope(src->se_value,new_scope,lexer)) == NULL) {
/*err_r_switch_iter:*/ while (iter != my_entryv) { --iter; _DeeXAstSwitchEntry_Quit(iter); }
    }
    if (can_reference && iter->se_value != src->se_value) can_reference = 0;
    Dee_INCREF(iter->se_key = src->se_key);
    ++iter,++src;
   } while (iter != end);
   if (can_reference) {
decref_result_and_ret_self: Dee_DECREF(ast_result); goto ret_self;
   }
   return ast_result;
  } break;

  case DEE_XASTKIND_IFTRUE:
  case DEE_XASTKIND_IFFALSE: {
   if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(self->ast_kind,self->ast_common.ast_token)) == NULL) return NULL;
   if (!self->ast_ifconst.ic_succ) ast_result->ast_ifconst.ic_succ = NULL;
   else if DEE_UNLIKELY((ast_result->ast_ifconst.ic_succ = DeeXAst_CopyWithScope(
    self->ast_ifconst.ic_succ,new_scope,lexer)) == NULL) goto err_r_unsafe;
   if (!self->ast_ifconst.ic_fail) ast_result->ast_ifconst.ic_fail = NULL;
   else if DEE_UNLIKELY((ast_result->ast_ifconst.ic_fail = DeeXAst_CopyWithScope(
    self->ast_ifconst.ic_fail,new_scope,lexer)) == NULL) {
/*err_r_ifc_succ:*/ Dee_XDECREF(ast_result->ast_ifconst.ic_succ); goto err_r_unsafe;
   }
   if (ast_result->ast_ifconst.ic_succ == self->ast_ifconst.ic_succ &&
       ast_result->ast_ifconst.ic_fail == self->ast_ifconst.ic_fail
       ) goto decref_result_and_ret_self;
   return ast_result;
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case DEE_XASTKIND_BUILTIN_BOUND: {
   DeeScopeObject *actual_var_scope = DeeScope_MODULE_ROOT(self->ast_builtin_bound.b_var->lv_scope);
   if (DeeScope_IsSameOrWeakParent((DeeObject *)actual_var_scope,(DeeObject *)new_scope)) goto ret_self;
   if DEE_UNLIKELY(DeeScope_IsSameOrParent((DeeObject *)actual_var_scope,(DeeObject *)new_scope)) {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_BUILTIN_BOUND_FROM_DIFFERENT_STRONG_SCOPE,
     (DeeObject *)lexer,(DeeObject *)self->ast_builtin_bound.b_token,
     "The given variable in '__builtin_bound' is from another strong scope, "
     "thus preventing detecting at runtime") != 0) return NULL;
    return DeeXAst_NewConst(self->ast_common.ast_token,DeeBool_Const(
     self->ast_builtin_bound.b_var->lv_init != 0));
   }
   goto unreachable_var;
  }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */

#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
  case DEE_XASTKIND_BUILTIN_EXTERN: {
   if (!self->ast_builtin_extern.ex_file) ast_a = NULL;
   else if DEE_UNLIKELY((ast_a = DeeXAst_CopyWithScope(self->ast_builtin_extern.ex_file,new_scope,lexer)) == NULL) return NULL;
   if DEE_UNLIKELY((ast_b = DeeXAst_CopyWithScope(self->ast_builtin_extern.ex_name,new_scope,lexer)) == NULL) {err_xa: Dee_XDECREF(ast_a); return NULL; }
   if DEE_UNLIKELY((ast_c = DeeXAst_CopyWithScope(self->ast_builtin_extern.ex_type,new_scope,lexer)) == NULL) {err_bxa: Dee_DECREF(ast_b); goto err_xa; }
   if (ast_a == self->ast_builtin_extern.ex_file &&
       ast_b == self->ast_builtin_extern.ex_name &&
       ast_c == self->ast_builtin_extern.ex_type) {
    Dee_DECREF(ast_c);
    Dee_DECREF(ast_b);
    Dee_XDECREF(ast_a);
    goto ret_self;
   }
   if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(DEE_XASTKIND_BUILTIN_EXTERN,self->ast_common.ast_token)) == NULL) goto err_bxa;
   Dee_INHERIT_REF(ast_result->ast_builtin_extern.ex_file,ast_a);
   Dee_INHERIT_REF(ast_result->ast_builtin_extern.ex_name,ast_b);
   Dee_INHERIT_REF(ast_result->ast_builtin_extern.ex_type,ast_c);
   return ast_result;
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
  case DEE_XASTKIND_BUILTIN_EXPECT: {
   if DEE_UNLIKELY((ast_a = DeeXAst_CopyWithScope(self->ast_builtin_expect.e_value,new_scope,lexer)) == NULL) return NULL;
   if (ast_a == self->ast_builtin_expect.e_value) { Dee_DECREF(ast_a); goto ret_self; }
   if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(DEE_XASTKIND_BUILTIN_EXPECT,self->ast_common.ast_token)) == NULL) goto err_a;
   Dee_INHERIT_REF(ast_result->ast_builtin_expect.e_value,ast_a);
   Dee_INCREF(ast_result->ast_builtin_expect.e_expect = self->ast_builtin_expect.e_expect);
   return ast_result;
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */

  default:
   if (DEE_XASTKIND_ISOPERATOR(self->ast_kind)) {
    int is_equal = 1;
    switch (DEE_XASTKIND_OPCOUNT(self->ast_kind)) {
     default:ret_self: Dee_INCREF(self); return self;
     case 3: if DEE_UNLIKELY((ast_c = DeeXAst_CopyWithScope(self->ast_operator.op_c,new_scope,lexer)) == NULL) return NULL; if (ast_c != self->ast_operator.op_c) is_equal = 0; DEE_ATTRIBUTE_FALLTHROUGH
     case 2: if DEE_UNLIKELY((ast_b = DeeXAst_CopyWithScope(self->ast_operator.op_b,new_scope,lexer)) == NULL) {err_op_ast_c: if (DEE_XASTKIND_OPCOUNT(self->ast_kind) >= 3) Dee_DECREF(ast_c); return NULL; } if (ast_b != self->ast_operator.op_b) is_equal = 0; DEE_ATTRIBUTE_FALLTHROUGH
     case 1: if DEE_UNLIKELY((ast_a = DeeXAst_CopyWithScope(self->ast_operator.op_a,new_scope,lexer)) == NULL) {err_op_ast_b: if (DEE_XASTKIND_OPCOUNT(self->ast_kind) >= 2) Dee_DECREF(ast_b); goto err_op_ast_c; } if (ast_a != self->ast_operator.op_a) is_equal = 0; DEE_ATTRIBUTE_FALLTHROUGH
             break;
    }
    if (is_equal) { // Conserve memory
     switch (DEE_XASTKIND_OPCOUNT(self->ast_kind)) {
      case 3: Dee_DECREF(ast_c); DEE_ATTRIBUTE_FALLTHROUGH
      case 2: Dee_DECREF(ast_b); DEE_ATTRIBUTE_FALLTHROUGH
      case 1: Dee_DECREF(ast_a); break;
      default: DEE_BUILTIN_UNREACHABLE();
     }
     goto ret_self;
    }
    if DEE_UNLIKELY((ast_result = _DeeXAst_NewUnsafe(self->ast_kind,self->ast_common.ast_token)) == NULL) {
     if (DEE_XASTKIND_OPCOUNT(self->ast_kind) >= 1) Dee_DECREF(ast_a);
     goto err_op_ast_b;
    }
    switch (DEE_XASTKIND_OPCOUNT(self->ast_kind)) {
     case 3: Dee_INHERIT_REF(ast_result->ast_operator.op_c,ast_c); DEE_ATTRIBUTE_FALLTHROUGH
     case 2: Dee_INHERIT_REF(ast_result->ast_operator.op_b,ast_b); DEE_ATTRIBUTE_FALLTHROUGH
     case 1: Dee_INHERIT_REF(ast_result->ast_operator.op_a,ast_a); break;
     default: DEE_BUILTIN_UNREACHABLE();
    }
    return ast_result;
   }
   break;
 }
 // Unsupported type (shouldn't happen, but might once I forget smth)
 DeeError_NotImplementedf("Can't copy XAst: %r",self);
 return NULL;
}
DEE_COMPILER_MSVC_WARNING_POP

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
DEE_A_RET_EXCEPT(-1) int DeeXAstClassAstEntry_InitCopyWithScope(
 DEE_A_OUT struct DeeXAstClassAstEntry *self, DEE_A_IN struct DeeXAstClassAstEntry const *right,
 DEE_A_INOUT DeeScopeObject *new_scope, DEE_A_INOUT DeeLexerObject *lexer) {
 DeeXAstObject *ast_a,*ast_b,*ast_c;
 DEE_ASSERT(self && right);
 DEE_ASSERT(DeeObject_Check(new_scope) && DeeScope_Check(new_scope));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 switch (right->ce_kind) {
  case DEE_XAST_CLASSENTRY_KIND_SLOT:
   if DEE_UNLIKELY((ast_a = DeeXAst_CopyWithScope(right->ce_slot.cs_callback,new_scope,lexer)) == NULL) return -1;
   self->ce_kind = DEE_XAST_CLASSENTRY_KIND_SLOT;
   self->ce_slot.cs_callback = ast_a; // inherit reference
   self->ce_slot.cs_slotid = right->ce_slot.cs_slotid;
   return (ast_a == right->ce_slot.cs_callback) ? 1 : 0;
  case DEE_XAST_CLASSENTRY_KIND_METHOD:
  case DEE_XAST_CLASSENTRY_KIND_CLASS_METHOD:
   if DEE_UNLIKELY((ast_a = DeeXAst_CopyWithScope(right->ce_method.cm_callback,new_scope,lexer)) == NULL) return -1;
   self->ce_kind = right->ce_kind;
   self->ce_method.cm_callback = ast_a; // inherit reference
   Dee_INCREF(self->ce_method.cm_name = self->ce_method.cm_name);
   return (ast_a == right->ce_method.cm_callback) ? 1 : 0;
  case DEE_XAST_CLASSENTRY_KIND_GETSET:
  case DEE_XAST_CLASSENTRY_KIND_CLASS_GETSET:
   if (!right->ce_getset.cg_get) ast_a = NULL;
   else if DEE_UNLIKELY((ast_a = DeeXAst_CopyWithScope(right->ce_getset.cg_get,new_scope,lexer)) == NULL) return -1;
   if (!right->ce_getset.cg_del) ast_b = NULL;
   else if DEE_UNLIKELY((ast_b = DeeXAst_CopyWithScope(right->ce_getset.cg_del,new_scope,lexer)) == NULL) {err_xa: Dee_XDECREF(ast_a); return -1; }
   if (!right->ce_getset.cg_set) ast_c = NULL;
   else if DEE_UNLIKELY((ast_c = DeeXAst_CopyWithScope(right->ce_getset.cg_set,new_scope,lexer)) == NULL) {/*err_xbxa:*/ Dee_XDECREF(ast_b); goto err_xa; }
   self->ce_kind = right->ce_kind;
   self->ce_getset.cg_get = ast_a; // inherit reference
   self->ce_getset.cg_del = ast_b; // inherit reference
   self->ce_getset.cg_set = ast_c; // inherit reference
   return (ast_a == right->ce_getset.cg_get &&
           ast_b == right->ce_getset.cg_del &&
           ast_c == right->ce_getset.cg_set) ? 1 : 0;
  default: self->ce_kind = DEE_XAST_CLASSENTRY_KIND_NONE; break;
 }
 return 0;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */

DEE_A_RET_EXCEPT(-1) int DeeVarDeclStorage_InitCopyWithScope(
 DEE_A_OUT struct DeeVarDeclStorage *self, DEE_A_IN struct DeeVarDeclStorage const *right,
 DEE_A_INOUT DeeScopeObject *new_scope, DEE_A_INOUT struct DeeLexerObject *lexer) {
 DeeXAstObject *ast_a,*ast_b,*ast_c;
 DEE_ASSERT(self && right);
 DEE_ASSERT(DeeObject_Check(new_scope) && DeeScope_Check(new_scope));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 switch (right->vds_kind) {
  case DEE_VARDECLSTORAGEKIND_VAR: {
   DeeScopeObject *actual_right_var_scope = DeeScope_MODULE_ROOT(right->vds_var.vs_var->lv_scope);
   if (DeeScope_IsSameOrWeakParent((DeeObject *)actual_right_var_scope,(DeeObject *)new_scope)) {
    self->vds_kind = DEE_VARDECLSTORAGEKIND_VAR;
    Dee_INCREF(self->vds_var.vs_token = right->vds_var.vs_token);
    Dee_INCREF(self->vds_var.vs_var = right->vds_var.vs_var);
    return 1;
   }
   if DEE_UNLIKELY(DeeScope_IsSameOrParent((DeeObject *)actual_right_var_scope,(DeeObject *)new_scope)) {
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_CANT_STORE_VAR_FROM_DIFFERENT_SCOPE,
     (DeeObject *)lexer,(DeeObject *)right->vds_var.vs_token,
     "Store to variable from different strong scope will be compiled as assign") != 0) return -1;
    if DEE_UNLIKELY((self->vds_assign.as_ast = DeeXAst_NewReferenceVar(
     right->vds_var.vs_token,right->vds_var.vs_var,new_scope)) == NULL) return -1;
    self->vds_kind = DEE_VARDECLSTORAGEKIND_ASSIGN;
    return 0;
   }
   if DEE_UNLIKELY(DeeError_CompilerError(
    DEE_WARNING_CANT_ACCESS_VAR_FROM_UNREACHABLE_SCOPE,
    (DeeObject *)lexer,(DeeObject *)right->vds_var.vs_token,
    "Can't access variable form unreachable scope") != 0) return -1;
   self->vds_kind = DEE_VARDECLSTORAGEKIND_NONE;
   return 0;
  }
  case DEE_VARDECLSTORAGEKIND_ATTR:
   if DEE_UNLIKELY((ast_a = DeeXAst_CopyWithScope(right->vds_attr.as_object,new_scope,lexer)) == NULL) return -1;
   if DEE_UNLIKELY((ast_b = DeeXAst_CopyWithScope(right->vds_attr.as_attr,new_scope,lexer)) == NULL) {err_a: Dee_DECREF(ast_a); return -1; }
   self->vds_kind = DEE_VARDECLSTORAGEKIND_ATTR;
   self->vds_attr.as_object = ast_a; // inherit reference
   self->vds_attr.as_attr = ast_b; // inherit reference
   return (ast_a == right->vds_attr.as_object && ast_b == right->vds_attr.as_attr) ? 1 : 0;
  case DEE_VARDECLSTORAGEKIND_ATTR_C:
   if DEE_UNLIKELY((ast_a = DeeXAst_CopyWithScope(right->vds_attr_c.acs_object,new_scope,lexer)) == NULL) return -1;
   self->vds_kind = DEE_VARDECLSTORAGEKIND_ATTR_C;
   self->vds_attr_c.acs_object = ast_a; // inherit reference
   Dee_INCREF(self->vds_attr_c.acs_attr = right->vds_attr_c.acs_attr);
   return (ast_a == right->vds_attr_c.acs_object) ? 1 : 0;
  case DEE_VARDECLSTORAGEKIND_ITEM:
   if DEE_UNLIKELY((ast_a = DeeXAst_CopyWithScope(right->vds_item.is_object,new_scope,lexer)) == NULL) return -1;
   if DEE_UNLIKELY((ast_b = DeeXAst_CopyWithScope(right->vds_item.is_key,new_scope,lexer)) == NULL) goto err_a;
   self->vds_kind = DEE_VARDECLSTORAGEKIND_ITEM;
   self->vds_item.is_object = ast_a; // inherit reference
   self->vds_item.is_key = ast_b; // inherit reference
   return (ast_a == right->vds_item.is_object && ast_b == right->vds_item.is_key) ? 1 : 0;
  case DEE_VARDECLSTORAGEKIND_RANGE:
   if DEE_UNLIKELY((ast_a = DeeXAst_CopyWithScope(right->vds_range.rs_object,new_scope,lexer)) == NULL) return -1;
   if (!right->vds_range.rs_begin) ast_b = NULL;
   else if DEE_UNLIKELY((ast_b = DeeXAst_CopyWithScope(right->vds_range.rs_begin,new_scope,lexer)) == NULL) goto err_a;
   if (!right->vds_range.rs_end) ast_c = NULL;
   else if DEE_UNLIKELY((ast_c = DeeXAst_CopyWithScope(right->vds_range.rs_end,new_scope,lexer)) == NULL) {/*err_xba:*/ Dee_XDECREF(ast_b); goto err_a; }
   self->vds_kind = DEE_VARDECLSTORAGEKIND_RANGE;
   self->vds_range.rs_object = ast_a; // inherit reference
   self->vds_range.rs_begin = ast_b; // inherit reference
   self->vds_range.rs_end = ast_c; // inherit reference
   return (ast_a == right->vds_range.rs_object &&
           ast_b == right->vds_range.rs_begin &&
           ast_c == right->vds_range.rs_end) ? 1 : 0;
  case DEE_VARDECLSTORAGEKIND_EXPAND: {
   struct DeeVarDeclStorage *new_declv,*iter,*end,*src;
   int can_reference = 1,temp;
   DEE_ASSERT(right->vds_expand.es_declc);
   while DEE_UNLIKELY((new_declv = (struct DeeVarDeclStorage *)malloc_nz(
    right->vds_expand.es_declc*sizeof(struct DeeVarDeclStorage))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
    return -1;
   }
   self->vds_expand.es_declv = new_declv;
   self->vds_expand.es_declc = right->vds_expand.es_declc;
   end = (iter = new_declv)+right->vds_expand.es_declc;
   src = right->vds_expand.es_declv;
   do {
    if DEE_UNLIKELY((temp = DeeVarDeclStorage_InitCopyWithScope(iter,src,new_scope,lexer)) <= 0) {
     if DEE_UNLIKELY(temp < 0) {
      while (iter != new_declv) { --iter; DeeVarDeclStorage_Quit(iter); }
      free_nn(new_declv);
      return -1;
     }
     DEE_ASSERT(temp == 0);
     can_reference = 0;
    }
    ++iter,++src;
   } while (iter != end);
   return can_reference;
  } break;
  case DEE_VARDECLSTORAGEKIND_ASSIGN:
   if DEE_UNLIKELY((ast_a = DeeXAst_CopyWithScope(right->vds_assign.as_ast,new_scope,lexer)) == NULL) return -1;
   self->vds_kind = DEE_VARDECLSTORAGEKIND_ASSIGN;
   self->vds_assign.as_ast = ast_a; // inherit reference
   return (ast_a == right->vds_assign.as_ast) ? 1 : 0;
  default: self->vds_kind = DEE_VARDECLSTORAGEKIND_NONE; break;
 }
 return 0;
}


DEE_DECL_END
