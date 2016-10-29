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
#ifndef GUARD_DEEMON_STMT_AST_C
#define GUARD_DEEMON_STMT_AST_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// What a mess...
#if !DEE_CONFIG_COMPILER_USE_NEW_AST_TYPES
#include <deemon/compiler/compiler.h>
#include <deemon/compiler/expr_ast.h>
#include <deemon/compiler/scope.h>
#include <deemon/compiler/stmt_ast.h>
#include <deemon/memberdef.h>
#include <deemon/guid.h>
#include <deemon/list.h>
#include <deemon/integer.h>
#include <deemon/dict.h>
#include <deemon/file.h>
#include <deemon/error.h>
#include <deemon/none.h>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN


_DeeObjectPool_Define(_DeeStmtAstPool,DeeStmtAstObject)

#ifdef DEE_DEBUG
_DeeObjectPool_DefineStastics(_DeeStmtAstPool)
#endif


Dee_size_t DeeStmtAst_ClearPool(void) {
#ifdef DEE_DEBUG
 DEE_LVERBOSE3("Clearing stmt_ast pool (%Iu)\n",_DeeStmtAstPool_Size());
#endif
 return _DeeStmtAstPool_Clear();
}

DEE_COMPILER_MSVC_WARNING_PUSH(4061)
DEE_A_RET_EXCEPT(-1) int DeeStmtAst_Optimize(
 DEE_A_INOUT_OBJECT(DeeStmtAstObject) *self,
 DEE_A_INOUT DeeObject *scope, DEE_A_IN Dee_uint32_t flags) {
 DeeObject **iter,**end;
#ifdef DEE_DEBUG
 DeeStmtAstObject *ast = (DeeStmtAstObject *)self;
#else
#define ast ((DeeStmtAstObject *)self)
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeStmtAst_Check(self));
 switch (ast->ob_kind) {
  // TODO: All the rest
  case DeeStmtAstKind_RETURN:
   return ast->ob_return.ob_ret_expr
    ? DeeExprAst_Optimize((DeeObject *)ast->ob_return.ob_ret_expr,scope,flags|DEE_OPTIMIZE_FLAG_USED)
    : 0;
  case DeeStmtAstKind_YIELD:
   end = (iter = DeeTuple_ELEM(ast->ob_yield.ob_yld_exprs))+DeeTuple_SIZE(ast->ob_yield.ob_yld_exprs);
optimize_expr_iter:
   while (iter != end) if (DeeExprAst_Optimize(*iter++,scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   break;
  case DeeStmtAstKind_DEL:
   end = (iter = DeeTuple_ELEM(ast->ob_del.ob_del_exprs))+DeeTuple_SIZE(ast->ob_del.ob_del_exprs);
   goto optimize_expr_iter;
  case DeeStmtAstKind_ASSERT:
   if (ast->ob_assert.ob_asrt_msg && DeeExprAst_Optimize(
    (DeeObject *)ast->ob_assert.ob_asrt_msg,scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   return DeeExprAst_Optimize((DeeObject *)ast->ob_assert.ob_asrt_expr,scope,flags|DEE_OPTIMIZE_FLAG_USED);
  case DeeStmtAstKind_PRINT:
  case DeeStmtAstKind_PRINT_NOLF:
   if (ast->ob_print.ob_prt_file && DeeExprAst_Optimize(
    (DeeObject *)ast->ob_print.ob_prt_file,scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   if (ast->ob_print.ob_prt_tpl && DeeExprAst_Optimize(
    (DeeObject *)ast->ob_print.ob_prt_tpl,scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   break;
  case DeeStmtAstKind_IF:
   if (DeeExprAst_Optimize((DeeObject *)ast->ob_if.ob_if_expr,(DeeObject *)ast->ob_if.ob_if_thenscope,flags|DEE_OPTIMIZE_FLAG_USED) == -1 ||
       DeeStmtAst_Optimize((DeeObject *)ast->ob_if.ob_if_thenstmt,(DeeObject *)ast->ob_if.ob_if_thenscope,flags&~(DEE_OPTIMIZE_FLAG_USED)) == -1) return -1;
   if (ast->ob_if.ob_if_elsestmt && DeeStmtAst_Optimize((DeeObject *)
    ast->ob_if.ob_if_elsestmt,(DeeObject *)ast->ob_if.ob_if_elsescope,flags&~(DEE_OPTIMIZE_FLAG_USED)) == -1) return -1;
   break;
  case DeeStmtAstKind_FOR:
   if (ast->ob_for.ob_f_forinit && DeeExprAst_Optimize(
    (DeeObject *)ast->ob_for.ob_f_forinit,(DeeObject *)ast->ob_for.ob_f_scope,flags&~(DEE_OPTIMIZE_FLAG_USED)) == -1) return -1;
   if (ast->ob_for.ob_f_forcont && DeeExprAst_Optimize(
    (DeeObject *)ast->ob_for.ob_f_forcont,(DeeObject *)ast->ob_for.ob_f_scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   if (ast->ob_for.ob_f_fornext && DeeExprAst_Optimize(
    (DeeObject *)ast->ob_for.ob_f_fornext,(DeeObject *)ast->ob_for.ob_f_scope,flags&~(DEE_OPTIMIZE_FLAG_USED)) == -1) return -1;
   return DeeStmtAst_Optimize((DeeObject *)ast->ob_for.ob_f_block,(DeeObject *)ast->ob_for.ob_f_scope,flags&~(DEE_OPTIMIZE_FLAG_USED));
  case DeeStmtAstKind_FOR_IN:
  case DeeStmtAstKind_FOREACH:
   if (DeeExprAst_Optimize((DeeObject *)ast->ob_for_in.ob_fi_iterlist,(
    DeeObject *)ast->ob_for_in.ob_fi_scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   return DeeStmtAst_Optimize((DeeObject *)ast->ob_for_in.ob_fi_block,(DeeObject *)ast->ob_for_in.ob_fi_scope,flags&~(DEE_OPTIMIZE_FLAG_USED));
  case DeeStmtAstKind_WHILE:
   if (DeeExprAst_Optimize((DeeObject *)ast->ob_while.ob_w_loopcond,(
    DeeObject *)ast->ob_while.ob_w_scope,flags|DEE_OPTIMIZE_FLAG_USED) == -1) return -1;
   return DeeStmtAst_Optimize((DeeObject *)ast->ob_while.ob_w_block,(
    DeeObject *)ast->ob_while.ob_w_scope,flags&~(DEE_OPTIMIZE_FLAG_USED));
  case DeeStmtAstKind_DO_WHILE:
   if (DeeStmtAst_Optimize((DeeObject *)ast->ob_do_while.ob_dw_block,(
    DeeObject *)ast->ob_do_while.ob_dw_scope,flags&~(DEE_OPTIMIZE_FLAG_USED)) == -1) return -1;
   return DeeExprAst_Optimize((DeeObject *)ast->ob_do_while.ob_dw_loopcond,(
    DeeObject *)ast->ob_do_while.ob_dw_scope,flags|DEE_OPTIMIZE_FLAG_USED);
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  case DeeStmtAstKind_THROW:
   return DeeExprAst_Optimize((DeeObject *)ast->ob_throw.ob_trw_expr,scope,flags|DEE_OPTIMIZE_FLAG_USED);
  case DeeStmtAstKind_TRY: {
   struct DeeStmtAstTryHandler *hiter,*hend;
   if (DeeStmtAst_Optimize((DeeObject *)ast->ob_try.ob_try_block,(
    DeeObject *)ast->ob_try.ob_try_scope,flags&~(DEE_OPTIMIZE_FLAG_USED)) == -1) return -1;
   hend = (hiter = ast->ob_try.ob_try_handlerv)+ast->ob_try.ob_try_handlerc;
   while (hiter != hend) {
    if (DeeStmtAst_Optimize((DeeObject *)hiter->ob_block,(
     DeeObject *)hiter->ob_scope,flags&~(DEE_OPTIMIZE_FLAG_USED)) == -1) return -1;
    ++hiter;
   }
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
  case DeeStmtAstKind_MODULE:
   return DeeStmtAst_Optimize(
    (DeeObject *)ast->ob_module.ob_mod_block,
    (DeeObject *)ast->ob_module.ob_mod_module->ob_scope,flags);
  case DeeStmtAstKind_INL_STMTS:
   end = (iter = (DeeObject **)ast->ob_inl_stmts.ob_inl_stmts_v)+ast->ob_inl_stmts.ob_inl_stmts_c;
   goto optimize_stmt_iter;
  case DeeStmtAstKind_LOOP_ONCE:
   return DeeStmtAst_Optimize(
    (DeeObject *)ast->ob_loop_once.ob_lonce_block,
    (DeeObject *)ast->ob_loop_once.ob_lonce_scope,flags);
  case DeeStmtAstKind_SWITCH:
   break;
  case DeeStmtAstKind_STMTS:
   end = (iter = (DeeObject **)ast->ob_stmts.ob_stmts_v)+ast->ob_stmts.ob_stmts_c;
optimize_stmt_iter:
   while (iter != end) {
    if (DeeStmtAst_Optimize(*iter++,(DeeObject *)ast->ob_stmts.ob_stmts_scope,(Dee_uint32_t)(iter == end
     ? flags|DEE_OPTIMIZE_FLAG_USED : flags&~(DEE_OPTIMIZE_FLAG_USED))) == -1) return -1;
   }
   break;
  case DeeStmtAstKind_SEXPR:
   return DeeExprAst_Optimize(
    (DeeObject *)ast->ob_sexpr.ob_sexpr_ast,
    (DeeObject *)ast->ob_sexpr.ob_sexpr_scope,flags);
  case DeeStmtAstKind_SSTMT:
   return DeeStmtAst_Optimize(
    (DeeObject *)ast->ob_sstmt.ob_sstmt_ast,
    (DeeObject *)ast->ob_sstmt.ob_sstmt_scope,flags);
  case DeeStmtAstKind_EXPR:
   return DeeExprAst_Optimize(
    (DeeObject *)ast->ob_expr.ob_expr_ast,scope,flags);
  default: break;
 }
 return 0;
#ifdef ast
#undef ast
#endif
}
DEE_COMPILER_MSVC_WARNING_POP


DEE_A_RET_OBJECT_EXCEPT_REF(DeeStmtAstObject) *
_DeeStmtAst_NewUnsafe(DEE_A_IN_OBJECT(DeeTokenObject) *token) {
 DeeStmtAstObject *result;
 DEE_ASSERT(DeeObject_Check(token) && DeeToken_Check(token));
 if ((result = (DeeStmtAstObject *)_DeeStmtAstPool_Malloc()) != NULL) {
  DeeObject_INIT(result,&DeeStmtAst_Type);
  Dee_INCREF(result->ob_token = (DeeTokenObject *)token);
  DeeLabelRefList_Init(&result->ob_label_endpoints);
 }
 return (DeeObject *)result;
}




int _DeeStmtAst_OptimizeFor(DeeStmtAstObject *self) {
 DEE_ASSERT(self->ob_kind == DeeStmtAstKind_FOR);
 if (self->ob_for.ob_f_forcont &&
     self->ob_for.ob_f_forcont->ob_kind == DeeExprAstKind_CONSTANT) {
  int is_true = DeeObject_Bool(self->ob_for.ob_f_forcont->ob_const);
  if (is_true == -1) DeeError_Handled(); else {
   if (is_true) {
    // Constant true in for condition can be ignored
    Dee_CLEAR(self->ob_for.ob_f_forcont);
   } else {
    // Constant false removes cont, loop and next, creating a regular expression from init
    // - So we convert the whole thing into a scoped expression
    // >> for(expr; false; 42) { print "what about me?"; } --> {expr;}
    DeeScopeObject *f_scope;
    DeeExprAstObject *f_init;
    Dee_DECREF(self->ob_for.ob_f_forcont);
    Dee_DECREF(self->ob_for.ob_f_fornext);
    Dee_DECREF(self->ob_for.ob_f_block);
    f_init = self->ob_for.ob_f_forinit;
    f_scope = self->ob_for.ob_f_scope;
    if (f_init) {
     Dee_INHERIT_REF(self->ob_sexpr.ob_sexpr_ast,f_init);
     Dee_INHERIT_REF(self->ob_sexpr.ob_sexpr_scope,f_scope);
     self->ob_kind = DeeStmtAstKind_SEXPR;
    } else {
     Dee_DECREF(f_scope);
     self->ob_kind = DeeStmtAstKind_EMPTY;
    }
    return 0;
   }
  }
 }
 if (self->ob_for.ob_f_fornext && DeeExprAst_IsNoeffect(
  (DeeObject *)self->ob_for.ob_f_fornext,
  (DeeObject *)self->ob_for.ob_f_scope)) {
  // If the continue expression doesn't have any side effects, remove it
  Dee_CLEAR(self->ob_for.ob_f_fornext);
 }
 return 0;
}
int _DeeStmtAst_OptimizeForIn(DeeStmtAstObject *self, Dee_uint32_t flags) {
 DEE_ASSERT(self->ob_kind == DeeStmtAstKind_FOR_IN ||
            self->ob_kind == DeeStmtAstKind_FOREACH);
 if (self->ob_for_in.ob_fi_iterlist->ob_kind == DeeExprAstKind_CONSTANT &&
     (flags&DEE_PARSER_FLAG_OPTIMIZE_UNWIND_LOOPS) != 0) {
  // TODO: Loop unwinding
 }
 return 0;
}

#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
int _DeeStmtAst_OptimizeTry(DeeStmtAstObject *self) {
 DEE_ASSERT(self->ob_kind == DeeStmtAstKind_TRY);
restart:
 if (!self->ob_try.ob_try_handlerc) {
  DeeStmtAstObject *block;
  DeeScopeObject *scope;
make_sstmt:
  block = self->ob_try.ob_try_block;
  scope = self->ob_try.ob_try_scope;
  self->ob_kind = DeeStmtAstKind_SSTMT;
  Dee_INHERIT_REF(self->ob_sstmt.ob_sstmt_ast,block);
  Dee_INHERIT_REF(self->ob_sstmt.ob_sstmt_scope,scope);
 } else {
  struct DeeStmtAstTryHandler *iter,*end;
  // Remove empty finally blocks
  end = (iter = self->ob_try.ob_try_handlerv)+self->ob_try.ob_try_handlerc;
  while (iter != end) {
   if (iter->ob_kind == DeeExceptionHandleKind_FINALLY) {
    if (DeeStmtAst_IsNoeffect(
     (DeeObject *)iter->ob_block,
     (DeeObject *)iter->ob_scope)) {
     DeeStmtAstTryHandler_QUIT(iter);
     // v Shouldn't happen, but the spec doesn't specify
     memcpy(iter,iter+1,((Dee_size_t)(end-iter))*sizeof(struct DeeStmtAstTryHandler));
     --self->ob_try.ob_try_handlerc;
     if ((iter = (struct DeeStmtAstTryHandler *)realloc_nn(
      self->ob_try.ob_try_handlerv,self->ob_try.ob_try_handlerc*
      sizeof(struct DeeStmtAstTryHandler))) == NULL && self->ob_try.ob_try_handlerc) {
      DeeError_NoMemory();
      return -1;
     }
     self->ob_try.ob_try_handlerv = iter;
     goto restart;
    }
    break;
   }
   ++iter;
  }
#ifdef DEE_DEBUG
  while (iter++ != end) {
   DEE_ASSERT(iter->ob_kind != DeeExceptionHandleKind_FINALLY &&
              "Second finally block found");
  }
#endif

  if (DeeStmtAst_IsNoexcept(
   (DeeObject *)self->ob_try.ob_try_block,
   (DeeObject *)self->ob_try.ob_try_scope)) {
   iter = self->ob_try.ob_try_handlerv;
   do if (iter->ob_kind == DeeExceptionHandleKind_FINALLY) goto with_finally;
   while (++iter != end); // Make sure there are no finally handlers
   // Noexcept code & only exception handlers
   // >> Jump delete all the handlers & convert into sstmt
   iter = self->ob_try.ob_try_handlerv;
   do DeeStmtAstTryHandler_QUIT(iter); while (++iter != end);
   free_nn(self->ob_try.ob_try_handlerv);
   goto make_sstmt;
with_finally: {
    DeeStmtAstObject **merge_ast_v;
    DeeStmtAstObject *temp;
    if ((merge_ast_v = (DeeStmtAstObject **)malloc_nz(2*sizeof(DeeStmtAstObject *))) == NULL) {
     DeeError_NoMemory();
     return -1;
    }
    if ((temp = (DeeStmtAstObject *)_DeeStmtAst_NewUnsafe((DeeObject *)self->ob_token)) == NULL)
     return free_nn(merge_ast_v),-1;
    temp->ob_kind = DeeStmtAstKind_SSTMT;
    Dee_INHERIT_REF(temp->ob_sstmt.ob_sstmt_ast,self->ob_try.ob_try_block);
    Dee_INHERIT_REF(temp->ob_sstmt.ob_sstmt_scope,self->ob_try.ob_try_scope);
    Dee_INHERIT_REF(merge_ast_v[0],temp);
    if ((temp = (DeeStmtAstObject *)_DeeStmtAst_NewUnsafe((DeeObject *)self->ob_token)) == NULL) {
     Dee_DECREF(merge_ast_v[0]);
     free_nn(merge_ast_v);
     return -1;
    }
    temp->ob_kind = DeeStmtAstKind_SSTMT;
    DEE_ASSERT(!iter->ob_local);
    DEE_ASSERT(!iter->ob_type);
    Dee_INHERIT_REF(temp->ob_sstmt.ob_sstmt_ast,iter->ob_block);
    Dee_INHERIT_REF(temp->ob_sstmt.ob_sstmt_scope,iter->ob_scope);
    Dee_INHERIT_REF(merge_ast_v[1],temp);
    // Destroy all other handlers
    while (--end != iter) DeeStmtAstTryHandler_QUIT(end);
    while (iter-- != self->ob_try.ob_try_handlerv) DeeStmtAstTryHandler_QUIT(iter);
    // Override our statement
    free_nn(self->ob_try.ob_try_handlerv);
    self->ob_inl_stmts.ob_inl_stmts_v = merge_ast_v;
    self->ob_inl_stmts.ob_inl_stmts_c = 2;
    self->ob_kind = DeeStmtAstKind_INL_STMTS;
   }
  }
 }
 return 0;
}
#endif


DEE_STATIC_INLINE(int) _dee_is_noexcept_iterable(DeeTypeObject *a) {
 return 
  a == (DeeTypeObject *)&DeeNone_Type ||
  a == &DeeTuple_Type ||
  a == &DeeList_Type ||
  a == &DeeDict_Type ||
  a == &DeeDictKeys_Type ||
  a == &DeeDictItems_Type ||
  a == &DeeDictValues_Type ||
  a == &DeeString_Type ||
  a == &DeeRangeInt8Iterator_Type ||
  a == &DeeRangeInt16Iterator_Type ||
  a == &DeeRangeInt32Iterator_Type ||
  a == &DeeRangeInt64Iterator_Type ||
  a == &DeeRangeUInt8Iterator_Type ||
  a == &DeeRangeUInt16Iterator_Type ||
  a == &DeeRangeUInt32Iterator_Type ||
  a == &DeeRangeUInt64Iterator_Type
 ;
}

#ifndef DEE_DEBUG
#define _stmt ((DeeStmtAstObject *)self)
#endif
DEE_COMPILER_MSVC_WARNING_PUSH(4061)
DEE_A_RET_WUNUSED int DeeStmtAst_IsNoexcept(
 DEE_A_IN_OBJECT(DeeStmtAstObject) const *self,
 DEE_A_IN_OBJECT(DeeScopeObject) const *scope) {
#ifdef DEE_DEBUG
 DeeStmtAstObject *_stmt = (DeeStmtAstObject *)self;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeStmtAst_Check(self));
 switch (_stmt->ob_kind) {
  case DeeStmtAstKind_EMPTY:
  case DeeStmtAstKind_DEL:
  case DeeStmtAstKind_GOTO:
   return 1;

  case DeeStmtAstKind_MODULE:
   return DeeStmtAst_IsNoexcept(
    (DeeObject *)_stmt->ob_module.ob_mod_block,
    (DeeObject *)_stmt->ob_module.ob_mod_module->ob_scope);

  case DeeStmtAstKind_RETURN:
  case DeeStmtAstKind_YIELD:
  case DeeStmtAstKind_BREAK:
  case DeeStmtAstKind_CONTINUE:
   return 0; // Technically 1, but you could think of these as exceptions

  case DeeStmtAstKind_SEXPR:
   return DeeExprAst_IsNoexcept(
    (DeeObject *)_stmt->ob_sexpr.ob_sexpr_ast,
    (DeeObject *)_stmt->ob_sexpr.ob_sexpr_scope);
  case DeeStmtAstKind_SSTMT:
   return DeeStmtAst_IsNoexcept(
    (DeeObject *)_stmt->ob_sstmt.ob_sstmt_ast,
    (DeeObject *)_stmt->ob_sstmt.ob_sstmt_scope);

  case DeeStmtAstKind_INL_STMTS: {
   DeeStmtAstObject **iter,**end;
   end = (iter = _stmt->ob_inl_stmts.ob_inl_stmts_v)+_stmt->ob_inl_stmts.ob_inl_stmts_c;
   while (iter != end) if (!DeeStmtAst_IsNoexcept((DeeObject *)*iter++,scope)) return 0;
   return 1;
  } break;

  case DeeStmtAstKind_STMTS: {
   DeeStmtAstObject **iter,**end;
   end = (iter = _stmt->ob_stmts.ob_stmts_v)+_stmt->ob_stmts.ob_stmts_c;
   while (iter != end) if (!DeeStmtAst_IsNoexcept(
    (DeeObject *)*iter++,(DeeObject *)_stmt->ob_stmts.ob_stmts_scope)) return 0;
   return 1;
  } break;

  case DeeStmtAstKind_EXPR:
   return DeeExprAst_IsNoexcept((DeeObject *)_stmt->ob_expr.ob_expr_ast,scope);

  case DeeStmtAstKind_IF:
   if (_stmt->ob_if.ob_if_elsestmt && !DeeStmtAst_IsNoexcept(
    (DeeObject *)_stmt->ob_if.ob_if_elsestmt,(DeeObject *)_stmt->ob_if.ob_if_elsescope)) return 0;
   return DeeExprAst_IsNoexcept((DeeObject *)_stmt->ob_if.ob_if_expr,(DeeObject *)_stmt->ob_if.ob_if_thenscope) &&
          DeeStmtAst_IsNoexcept((DeeObject *)_stmt->ob_if.ob_if_thenstmt,(DeeObject *)_stmt->ob_if.ob_if_thenscope);

  case DeeStmtAstKind_FOR:
   if (_stmt->ob_for.ob_f_forinit && !DeeExprAst_IsNoexcept(
    (DeeObject *)_stmt->ob_for.ob_f_forinit,(DeeObject *)_stmt->ob_for.ob_f_scope)) return 0;
   if (_stmt->ob_for.ob_f_forcont && !DeeExprAst_IsNoexcept(
    (DeeObject *)_stmt->ob_for.ob_f_forcont,(DeeObject *)_stmt->ob_for.ob_f_scope)) return 0;
   if (_stmt->ob_for.ob_f_fornext && !DeeExprAst_IsNoexcept(
    (DeeObject *)_stmt->ob_for.ob_f_fornext,(DeeObject *)_stmt->ob_for.ob_f_scope)) return 0;
   return DeeStmtAst_IsNoexcept((DeeObject *)_stmt->ob_for.ob_f_block,(DeeObject *)_stmt->ob_for.ob_f_scope);

  case DeeStmtAstKind_FOR_IN:
  case DeeStmtAstKind_FOREACH: {
   DeeTypeObject *loop_expr_type;
   loop_expr_type = DeeExprAst_PredictType(
    (DeeObject *)_stmt->ob_for_in.ob_fi_iterlist,
    (DeeObject *)_stmt->ob_for_in.ob_fi_scope);
   if (loop_expr_type && _dee_is_noexcept_iterable(loop_expr_type)) return 1;
  } break;
   
  case DeeStmtAstKind_WHILE:
   return DeeExprAst_IsNoexcept((DeeObject *)_stmt->ob_while.ob_w_loopcond,(DeeObject *)_stmt->ob_while.ob_w_scope) &&
          DeeStmtAst_IsNoexcept((DeeObject *)_stmt->ob_while.ob_w_block,(DeeObject *)_stmt->ob_while.ob_w_scope);
   
  case DeeStmtAstKind_LOOP_ONCE:
   return DeeStmtAst_IsNoexcept(
    (DeeObject *)_stmt->ob_loop_once.ob_lonce_block,
    (DeeObject *)_stmt->ob_loop_once.ob_lonce_scope);
   
  case DeeStmtAstKind_DO_WHILE:
   return DeeStmtAst_IsNoexcept((DeeObject *)_stmt->ob_do_while.ob_dw_block,(DeeObject *)_stmt->ob_do_while.ob_dw_scope) &&
          DeeExprAst_IsNoexcept((DeeObject *)_stmt->ob_do_while.ob_dw_loopcond,(DeeObject *)_stmt->ob_do_while.ob_dw_scope);
   
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  //case DeeStmtAstKind_THROW:
  //case DeeStmtAstKind_RETHROW:
  // return 0; // Never
  case DeeStmtAstKind_TRY: {
   struct DeeStmtAstTryHandler *iter,*end;
   end = (iter = _stmt->ob_try.ob_try_handlerv)+_stmt->ob_try.ob_try_handlerc;
   if (DeeStmtAst_IsNoexcept(
    (DeeObject *)_stmt->ob_try.ob_try_block,
    (DeeObject *)_stmt->ob_try.ob_try_scope)) {
    // Try block is noexcept (make sure that all finally blocks are noexcept as well!)
    while (iter != end) {
     if (iter->ob_kind == DeeExceptionHandleKind_FINALLY) {
      // Finally block can throw an exception
      if (!DeeStmtAst_IsNoexcept(
       (DeeObject *)iter->ob_block,
       (DeeObject *)iter->ob_scope)) return 0;
     }
     ++iter;
    }
    return 1;
   } else {
    // Try block can throw (is there a noexcept catch-all handler?)
    while (iter != end) {
     if ((iter->ob_kind&DeeExceptionHandleKind_FLAG_ALL)!=0) {
      // If the first catch-all handler is noexcept, the try stmt is, too.
      return DeeStmtAst_IsNoexcept(
       (DeeObject *)iter->ob_block,
       (DeeObject *)iter->ob_scope);
     }
     ++iter;
    }
   }
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */

  default: break;
 }
 return 0;
}
DEE_COMPILER_MSVC_WARNING_POP

DEE_COMPILER_MSVC_WARNING_PUSH(4061)
DEE_A_RET_WUNUSED int DeeStmtAst_IsNoreturn(
 DEE_A_IN_OBJECT(DeeStmtAstObject) const *self,
 DEE_A_IN_OBJECT(DeeScopeObject) const *scope,
 DEE_A_IN Dee_uint32_t flags) {
#ifdef DEE_DEBUG
 DeeStmtAstObject *_stmt = (DeeStmtAstObject *)self;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeStmtAst_Check(self));
 switch (_stmt->ob_kind) {

  case DeeStmtAstKind_MODULE:
   return DeeStmtAst_IsNoreturn(
    (DeeObject *)_stmt->ob_module.ob_mod_block,
    (DeeObject *)_stmt->ob_module.ob_mod_module->ob_scope,flags);
  case DeeStmtAstKind_SEXPR:
   return DeeExprAst_IsNoreturn(
    (DeeObject *)_stmt->ob_sexpr.ob_sexpr_ast,
    (DeeObject *)_stmt->ob_sexpr.ob_sexpr_scope,flags);
  case DeeStmtAstKind_SSTMT:
   return DeeStmtAst_IsNoreturn(
    (DeeObject *)_stmt->ob_sstmt.ob_sstmt_ast,
    (DeeObject *)_stmt->ob_sstmt.ob_sstmt_scope,flags);

  case DeeStmtAstKind_INL_STMTS: {
   DeeStmtAstObject **iter,**end;
   end = (iter = _stmt->ob_inl_stmts.ob_inl_stmts_v)+_stmt->ob_inl_stmts.ob_inl_stmts_c;
   while (iter != end) if (DeeStmtAst_IsNoreturn((DeeObject *)*iter++,scope,flags)) return 1;
  } break;

  case DeeStmtAstKind_STMTS: {
   DeeStmtAstObject **iter,**end;
   end = (iter = _stmt->ob_stmts.ob_stmts_v)+_stmt->ob_stmts.ob_stmts_c;
   while (iter != end) if (DeeStmtAst_IsNoreturn(
    (DeeObject *)*iter++,(DeeObject *)_stmt->ob_stmts.ob_stmts_scope,flags)) return 1;
  } break;

  case DeeStmtAstKind_EXPR:
   return DeeExprAst_IsNoreturn((DeeObject *)_stmt->ob_expr.ob_expr_ast,scope,flags);

  case DeeStmtAstKind_BREAK:
  case DeeStmtAstKind_CONTINUE:
   return (flags & DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_BREAK_CONTINUE) != 0;

  //case DeeStmtAstKind_YIELD: // Sometimes...
  case DeeStmtAstKind_RETURN:
  case DeeStmtAstKind_GOTO:
   return 1;

#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  case DeeStmtAstKind_THROW:
  case DeeStmtAstKind_RETHROW:
   return (flags & DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_EXCEPTIONS) != 0;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */

  case DeeStmtAstKind_IF:
   if (DeeExprAst_IsNoreturn(
    (DeeObject *)_stmt->ob_if.ob_if_expr,
    (DeeObject *)_stmt->ob_if.ob_if_thenscope,flags)) return 1;
   if (_stmt->ob_if.ob_if_thenstmt && _stmt->ob_if.ob_if_elsestmt && 
       DeeStmtAst_IsNoreturn((DeeObject *)_stmt->ob_if.ob_if_thenstmt,
                             (DeeObject *)_stmt->ob_if.ob_if_thenscope,flags) &&
       DeeStmtAst_IsNoreturn((DeeObject *)_stmt->ob_if.ob_if_elsestmt,
                             (DeeObject *)_stmt->ob_if.ob_if_elsescope,flags)) return 1;
   break;

  case DeeStmtAstKind_FOR:
   if (_stmt->ob_for.ob_f_forinit && DeeExprAst_IsNoreturn(
    (DeeObject *)_stmt->ob_for.ob_f_forinit,
    (DeeObject *)_stmt->ob_for.ob_f_scope,flags)) return 1;
   if (!_stmt->ob_for.ob_f_forcont) {
    // Without a condition, we can be sure the block will always be executed
    if (_stmt->ob_for.ob_f_fornext && DeeExprAst_IsNoreturn(
     (DeeObject *)_stmt->ob_for.ob_f_fornext,
     (DeeObject *)_stmt->ob_for.ob_f_scope,
     flags&~DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_BREAK_CONTINUE)) return 1;
    if (DeeStmtAst_IsNoreturn(
     (DeeObject *)_stmt->ob_for.ob_f_block,
     (DeeObject *)_stmt->ob_for.ob_f_scope,
     flags&~DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_BREAK_CONTINUE)) return 1;
   } else {
    if (DeeExprAst_IsNoreturn(
     (DeeObject *)_stmt->ob_for.ob_f_forcont,
     (DeeObject *)_stmt->ob_for.ob_f_scope,
     flags&~DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_BREAK_CONTINUE)) return 1;
   }
   break;

  case DeeStmtAstKind_WHILE:
   if (DeeExprAst_IsNoreturn(
    (DeeObject *)_stmt->ob_while.ob_w_loopcond,
    (DeeObject *)_stmt->ob_while.ob_w_scope,
    flags&~DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_BREAK_CONTINUE)) return 1;
   break;

  case DeeStmtAstKind_LOOP_ONCE:
   return DeeStmtAst_IsNoreturn(
    (DeeObject *)_stmt->ob_loop_once.ob_lonce_block,
    (DeeObject *)_stmt->ob_loop_once.ob_lonce_scope,
    flags&~DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_BREAK_CONTINUE);

  case DeeStmtAstKind_DO_WHILE:
   return DeeStmtAst_IsNoreturn(
    (DeeObject *)_stmt->ob_do_while.ob_dw_block,
    (DeeObject *)_stmt->ob_do_while.ob_dw_scope,
    flags&~DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_BREAK_CONTINUE) ||
    DeeExprAst_IsNoreturn(
    (DeeObject *)_stmt->ob_do_while.ob_dw_loopcond,
    (DeeObject *)_stmt->ob_do_while.ob_dw_scope,
    flags&~DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_BREAK_CONTINUE);

#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  case DeeStmtAstKind_TRY: {
   struct DeeStmtAstTryHandler *iter,*end;
   // Special handling required because of this:
   // >> print __builtin_noreturn(({ // false (returns if 'cond') is true
   // >>   try {
   // >>     if (cond) throw "foobar";
   // >>     return; // would only cause noreturn, if all prior statements are noexcept
   // >>   } catch (...);
   // >> }));
   // If the try block is noexcept (meaning no catch handlers will ever be executed),
   // we can check if it will ever return.
   // >> If the try block isn't noexcept, there is a chance
   //    that any of the handlers are going to be executed.
   // NOTE: Finally blocks don't make a difference here.
   if (DeeStmtAst_IsNoreturn((DeeObject *)_stmt->ob_try.ob_try_block,
    (DeeObject *)_stmt->ob_try.ob_try_scope,(_stmt->ob_try.ob_try_handlerc
    ? (flags&~DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_EXCEPTIONS) : flags))) return 0;
   if (DeeStmtAst_IsNoexcept(
    (DeeObject *)_stmt->ob_try.ob_try_block,
    (DeeObject *)_stmt->ob_try.ob_try_scope)) return 1;
   // If all catch handlers are noreturn as well, we can safely say that
   // the entire statement will be too, even if the try-block isn't noexcept.
   end = (iter = _stmt->ob_try.ob_try_handlerv)+_stmt->ob_try.ob_try_handlerc;
   // -> Serach for returning exception handlers
   while (iter != end) {
    if (!DeeStmtAst_IsNoreturn((DeeObject *)iter->ob_block,
     (DeeObject *)iter->ob_scope,flags)) return 0;
    ++iter;
   }
   return 1;
  } break;

#endif

  default: break;
 }
 return 0;
}
DEE_COMPILER_MSVC_WARNING_POP
#undef _stmt


#define token     ((DeeLexerObject *)self)->ob_token->ob_token
#define token_ob  (DeeObject *)((DeeLexerObject *)self)->ob_token
#if DEE_CONFIG_LANGUAGE_HAVE_STATIC_IF
#define yield()   DeeLexer_YieldEx((DeeObject *)self,(DeeObject *)scope)
#else
#define yield()   DeeLexer_Yield((DeeObject *)self)
#endif

static int DeeLexer_ParseStatements2(
 DeeStmtAstObject *result, DeeObject *self, DeeObject *scope,
 Dee_uint32_t flags, TPPTokenID end_token);
DEE_STATIC_INLINE(int) DeeLexer_ParseStatements2_NewScope(
 DeeStmtAstObject *result, DeeObject *self, DeeObject *scope,
 Dee_uint32_t flags, TPPTokenID end_token) {
 DeeObject *new_scope; int iresult;
 if ((new_scope = DeeScope_New(scope,1)) == NULL) return -1;
 iresult = DeeLexer_ParseStatements2(result,self,new_scope,flags,end_token);
 Dee_DECREF(new_scope);
 return iresult;
}

static int DeeLexer_ParseStatements2(
 DeeStmtAstObject *result, DeeObject *self, DeeObject *scope,
 Dee_uint32_t flags, TPPTokenID end_token) {
 Dee_uint64_t last_token_id;
 Dee_size_t stmts_a,stmts_c;
 DeeStmtAstObject **stmts;
 DeeStmtAstObject *stmt;
 DEE_ASSERT(end_token != ';');
 if (token.tk_id == end_token) {
  if (!yield()) return -1;
  result->ob_kind = DeeStmtAstKind_EMPTY;
  return 0;
 }
 if ((stmt = (DeeStmtAstObject *)DeeLexer_ParseStatement(self,scope,flags)) == NULL) return -1;
 while (token.tk_id == ';') if (!yield()) goto err_stmt;
 if ((stmts = (DeeStmtAstObject **)mallocf_nz(
  (stmts_a = 8)*sizeof(DeeStmtAstObject *),"ast: statements")) == NULL) {err_stmt: Dee_DECREF(stmt); return -1; }
 stmts_c = 1,stmts[0] = stmt; // inherit reference
 result->ob_kind = DeeStmtAstKind_STMTS;
 while (token.tk_id != end_token) {
  if (DeeStmtAst_IsNoreturn((DeeObject *)stmt,scope,
   DEE_COMPILER_IS_NO_RETURN_FLAG_ALLOW_EXCEPTIONS)) flags |= DEE_PARSER_FLAG_UNREACHABLE;
next_stmt:
  last_token_id = ((DeeLexerObject *)self)->ob_token_pos;
  if ((stmt = (DeeStmtAstObject *)DeeLexer_ParseStatement(self,scope,flags)) == NULL) {
err_stmts:
   for (stmts_a = 0; stmts_a < stmts_c; ++stmts_a)
    Dee_DECREF(stmts[stmts_a]);
   free_nn(stmts);
   return -1;
  }
  if (((DeeLexerObject *)self)->ob_token_pos == last_token_id) {
   if (!token.tk_id) { Dee_DECREF(stmt); break; }
   // Manually consume a token, if we are stuck
   if (!yield()) { Dee_DECREF(stmt); goto err_stmts; }
  }
  // v Skip all additional ';' after the statement
  while (token.tk_id == ';') if (!yield()) { Dee_DECREF(stmt); goto err_stmts; }
  if (stmt->ob_kind == DeeStmtAstKind_EMPTY &&
      stmt->ob_label_endpoints.ob_c == 0) { // Empty statement
   Dee_DECREF(stmt);
   if (token.tk_id == end_token) break;
   goto next_stmt;
  } else {
   if (stmts_c == stmts_a && (stmts = (DeeStmtAstObject **)realloc_nnz(
     stmts,(stmts_a *= 2)*sizeof(DeeStmtAstObject *))) == NULL) {
    Dee_DECREF(stmt);
    goto err_stmts;
   }
   stmts[stmts_c++] = stmt; // Inherit reference (don't set to NULL, as we need stmt again)
  }
 }
 if (!yield()) goto err_stmts;
 result->ob_stmts.ob_stmts_c = stmts_c;
 result->ob_stmts.ob_stmts_v = stmts;
 Dee_INCREF(result->ob_stmts.ob_stmts_scope = (DeeScopeObject *)scope);
 return 0; // no ';' at the end of this kind of statement
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStmtAstObject) *DeeLexer_ParseStatements(
 DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeScopeObject) *scope, Dee_uint32_t flags, TPPTokenID end_token) {
 DeeStmtAstObject *result;
 if ((result = (DeeStmtAstObject *)_DeeStmtAst_NewUnsafe(
  DeeLexer_TOKEN(self))) != NULL) {
  if (DeeLexer_ParseStatements2(result,self,scope,flags,end_token) == -1) {
   _DeeStmtAst_Free(result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}



DEE_STATIC_INLINE(DEE_A_REF /*DeeExprAstObject*/ DeeObject *) _DeeLexer_ParsePrintExpressionTuple(
 // First if we accidentally parsed one statement, thinking it was the print output
 DEE_A_IN_OPT DeeExprAstObject *first,
 /*DeeLexerObject*/DeeObject *self, DEE_A_IN_OBJECT(DeeScopeObject) *scope, Dee_uint32_t flags, int *has_comma_suffix) {
 DeeExprAstObject *ast; DeeObject *temp; Dee_size_t tuple_size;
 DEE_ASSERT(has_comma_suffix);
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 if ((ast = (DeeExprAstObject *)DeeExprAst_New(DeeExprAstKind_TUPLE,
  first ? (DeeObject *)first->ob_token : DeeLexer_TOKEN(self))) != NULL) {
  if ((temp = DeeLexer_ParseExpression(self,scope,flags)) == NULL ||
      (ast->ob_args_a = (DeeTupleObject *)_DeeTuple_NewUnsafe(
      tuple_size = (Dee_size_t)(first ? 2 : 1))) == NULL) goto err_ast;
  if (first) {
   DeeTuple_SET(ast->ob_args_a,0,(DeeObject *)first);
   DeeTuple_SET(ast->ob_args_a,1,(DeeObject *)temp); // inherit reference
   Dee_INCREF(first);
  } else {
   DeeTuple_SET(ast->ob_args_a,0,(DeeObject *)temp); // inherit reference
  }
#ifdef DEE_DEBUG
  temp = NULL;
#endif
  while (((DeeTokenObject *)DeeLexer_TOKEN(self))->ob_token.tk_id == ',') {
   if (!yield()) goto err_ast;
   if (((DeeTokenObject *)DeeLexer_TOKEN(self))->ob_token.tk_id == ')' ||
       ((DeeTokenObject *)DeeLexer_TOKEN(self))->ob_token.tk_id == ';') {
    // This print list has a ',' suffix
    *has_comma_suffix = 1;
    break;
   }
   if ((temp = DeeLexer_ParseExpression(self,scope,flags)) == NULL) goto err_ast;
   if (_DeeTuple_ResizeUnsafe((DeeObject **)&ast->ob_args_a,++tuple_size) == -1) { Dee_DECREF(temp); goto err_ast; }
   DeeTuple_SET(ast->ob_args_a,tuple_size-1,temp); // inherit reference
#ifdef DEE_DEBUG
   temp = NULL;
#endif
  }
 }
 return (DeeObject *)ast;
err_ast:
 Dee_DECREF(ast);
 return NULL;
}




#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
// Returns -1 on error
// Returns 0 on success
DEE_STATIC_INLINE(int) _DeeLexer_ParseCatchOrFinally(
 struct DeeStmtAstTryHandler *handler,
 struct DeeLexerObject *self,
 struct DeeScopeObject *scope,
 Dee_uint32_t flags) {
 TPPTokenID tok_id = token.tk_id;
 if (!yield()) return -1;
 if (tok_id == KWD_catch) {
  if (token.tk_id != '(') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_CATCH,
    (DeeObject *)self,token_ob,"Expected '(' after 'catch'") == -1) return -1;
  } else if (!yield()) return -1;
  if (token.tk_id == TPP_TOK_DOTS) {
   if (!yield()) return -1;
catch_all:
   handler->ob_kind = DeeExceptionHandleKind_FLAG_ALL;
   handler->ob_type = NULL;
  } else if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
   DeeTokenObject *name_token;
   Dee_INCREF(name_token = (DeeTokenObject *)token_ob);
   // Variable to store the exception in
   if ((handler->ob_local = (DeeLocalVarObject *)DeeScope_Lookup(
    (DeeObject *)scope,token.tk_id,DEE_SCOPE_LOOKUP_MODE_LOCAL,NULL)) == NULL ||
    !yield()) return -1;
   if (token.tk_id == TPP_TOK_DOTS) {
    handler->ob_kind = DeeExceptionHandleKind_ALL_VAR;
    handler->ob_type = NULL;
    Dee_DECREF(name_token);
    if (!yield()) return -1;
    Dee_INCREF(handler->ob_local);
    goto parse_rparen;
   } else {
    int temp;
    handler->ob_local = NULL;
    temp = DeeLexer_Return((DeeObject *)self,(DeeObject *)name_token);
    _DeeScope_DelName(scope,name_token->ob_token.tk_id);
    Dee_DECREF(name_token);
    if (temp == -1) return -1;
    goto parse_catch_type;
   }
  } else {
   DeeExprAstObject *type_ast;
parse_catch_type:
   handler->ob_kind = DeeExceptionHandleKind_FLAG_TYPED;
   if ((type_ast = (DeeExprAstObject *)DeeLexer_ParseExpression(
    (DeeObject *)self,(DeeObject *)scope,flags|DEE_PARSER_FLAG_GENERATE_CONSTANT)
    ) == NULL) return -1;
   if (type_ast->ob_kind != DeeExprAstKind_CONSTANT) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_CATCH,
     (DeeObject *)self,(DeeObject *)type_ast->ob_token,"Expected constant expression after 'catch' '('"
     ) == -1) {err_tast: Dee_DECREF(type_ast); return -1; }
    Dee_DECREF(type_ast);
    goto catch_all;
   } else if (DeeNone_Check(type_ast->ob_const)) {
    Dee_DECREF(type_ast->ob_const);
    Dee_INCREF(type_ast->ob_const = (DeeObject *)&DeeNone_Type);
   } else if (!DeeType_Check(type_ast->ob_const)) {
    DeeTypeObject *tp;
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_TYPE_EXPRESSION_AFTER_CATCH,
    (DeeObject *)self,(DeeObject *)type_ast->ob_token,"Expected type expression after 'catch' '('"
    ) == -1) goto err_tast;
    Dee_INCREF(tp = Dee_TYPE(type_ast->ob_const));
    Dee_DECREF(type_ast->ob_const);
    Dee_INHERIT_REF(type_ast->ob_const,*(DeeObject **)&tp);
   }
   if (type_ast->ob_const == (DeeObject *)&DeeObject_Type) {
    // Optimization: catch(object) is the same as catch(...)
    handler->ob_kind = DeeExceptionHandleKind_ALL;
    handler->ob_type = NULL;
   } else {
    Dee_INCREF(handler->ob_type = (DeeTypeObject *)type_ast->ob_const);
   }
   Dee_DECREF(type_ast);
  }
  if (token.tk_id == TPP_TOK_COLLON || token.tk_id == TPP_TOK_ARROW) {
   if (!yield()) goto err_ty;
   if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_CATCH_COLLON,
     (DeeObject *)self,token_ob,"Expected <keyword> after 'catch' '(' <const_expr> ':'") == -1) goto err_ty;
   } else goto parse_var;
  }
  if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
parse_var:
   // Variable to store the exception in
   if ((handler->ob_local = (DeeLocalVarObject *)DeeScope_Lookup(
    (DeeObject *)scope,token.tk_id,DEE_SCOPE_LOOKUP_MODE_LOCAL,NULL)) == NULL ||
    !yield()) goto err_ty;
   Dee_INCREF(handler->ob_local);
   handler->ob_kind |= DeeExceptionHandleKind_FLAG_VAR;
  } else handler->ob_local = NULL;
parse_rparen:
  if (token.tk_id != ')') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_CATCH,
    (DeeObject *)self,token_ob,"Expected ')' after 'catch' '(' ...") == -1) return -1;
  } else if (!yield()) return -1;
 } else {
  DEE_ASSERT(tok_id == KWD_finally);
  handler->ob_kind = DeeExceptionHandleKind_FINALLY;
  handler->ob_local = NULL;
  handler->ob_type = NULL;
 }
 if ((handler->ob_block = (DeeStmtAstObject *)DeeLexer_ParseStatement(
  (DeeObject *)self,(DeeObject *)scope,flags)) == NULL) {
  Dee_XDECREF(handler->ob_local);
err_ty:
  Dee_XDECREF(handler->ob_type);
  return -1;
 }
 Dee_INCREF(handler->ob_scope = scope);
 return 0;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */

DEE_STATIC_INLINE(int) _DeeLexer_ParseAlignedAttribute(
 struct DeeAttributeDecl *attr, struct DeeLexerObject *self,
 struct DeeScopeObject *scope, Dee_uint32_t flags) {
 DeeExprAstObject *expr_ast; Dee_size_t new_align;
 if (token.tk_id != '(') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_ALIGNAS,(DeeObject *)self,token_ob,
   "Expected '(' after 'alignas'") == -1) return -1;
 } else if (!yield()) return -1;
 if ((expr_ast = (DeeExprAstObject *)DeeLexer_ParseExpression(
  (DeeObject *)self,(DeeObject *)scope,flags|DEE_PARSER_FLAG_GENERATE_CONSTANT)) == NULL) return -1;
 if (token.tk_id != ')') {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_ALIGNAS,(DeeObject *)self,token_ob,
   "Expected ')' after 'alignas' '(' <const_expr>") == -1) {err_ast: Dee_DECREF(expr_ast); return -1;}
 } else if (!yield()) goto err_ast;
 if (expr_ast->ob_kind != DeeExprAstKind_CONSTANT) {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_ALIGNAS,(DeeObject *)self,(DeeObject *)expr_ast->ob_token,
   "Expected constant expression after 'alignas' '('") == -1) goto err_ast;
  goto end;
 }
 if (DeeObject_Cast(Dee_size_t,expr_ast->ob_const,&new_align) != 0) {
  DeeError_Handled();
  if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_INTEGRAL_EXPRESSION_AFTER_ALIGNAS,(DeeObject *)self,(DeeObject *)expr_ast->ob_token,
   "Expected integral constant expression after 'alignas' '(' (got: %s:%k)",
   DeeType_NAME(Dee_TYPE(expr_ast->ob_const)),expr_ast->ob_const) == -1) goto err_ast;
  goto end;
 }
 if (new_align == 0) {
  if (DeeError_CompilerError(DEE_WARNING_INVALID_ALIGNMENT_IN_ALIGNAS,(DeeObject *)self,
   (DeeObject *)expr_ast->ob_token,"Invalid alignment in alignas: 0") == -1) goto err_ast;
  goto end;
 }
 attr->ob_alignment = new_align;
end:
  Dee_DECREF(expr_ast);
  return 0;
}

DEE_STATIC_INLINE(TPPTokenID) _TPPLexer_GetKeywordWithoutUnderscores(
 TPPLexerObject *self, TPPTokenID tokid) {
 char const *str_begin,*str_end;
 str_begin = TPPLexer_TokenIDStr(self,tokid);
 while (*str_begin == '_') ++str_begin;
 str_end = str_begin+strlen(str_begin);
 while (str_end[-1] == '_') if (str_end-- == str_begin) return 0;
 return TPPKeywordList_GetL(&self->l_keywords,str_begin,str_end,0);
}

int _DeeLexer_ParseAttributeNothrowDecl(
 struct DeeAttributeDecl *attr, struct DeeLexerObject *self,
 struct DeeScopeObject *scope, Dee_uint32_t flags) {
 if (!yield()) return -1;
 if (token.tk_id == '(') {
  DeeExprAstObject *noexcept_cond; int temp;
  if (!yield()) return -1;
  if ((noexcept_cond = (DeeExprAstObject *)DeeLexer_ParseExpression((DeeObject *)self,
   (DeeObject *)scope,flags|DEE_PARSER_FLAG_GENERATE_CONSTANT)) == NULL) return -1;
  if (noexcept_cond->ob_kind != DeeExprAstKind_CONSTANT) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_NOTHROW,(DeeObject *)self,(DeeObject *)noexcept_cond->ob_token,
    "Expected a constant expression in '__attribute__' '(' '(' 'nothrow' '(' <const_expr> (got: %r)",noexcept_cond) == -1) {
    Dee_DECREF(noexcept_cond);
    return -1;
   }
   Dee_DECREF(noexcept_cond);
  } else {
   temp = DeeObject_Bool(noexcept_cond->ob_const);
   Dee_DECREF(noexcept_cond);
   if (temp == -1) return -1;
   if (temp) attr->ob_fun_flags |= DeeFunctionFlags_NOEXCEPT;
   else attr->ob_fun_flags &= ~DeeFunctionFlags_NOEXCEPT;
  }
  if (token.tk_id != ')') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_ATTRIBUTE_NOTHROW,(DeeObject *)self,token_ob,
    "Expected ')' after '__attribute__' '(' '(' 'noexcept' '(' <const_expr>") == -1) return -1;
  } else if (!yield()) return -1;
 } else attr->ob_fun_flags |= DeeFunctionFlags_NOEXCEPT;
 return 0;
}

int _DeeLexer_ParseAttributeContent(
 struct DeeAttributeDecl *attr, struct DeeLexerObject *self,
 struct DeeScopeObject *scope, Dee_uint32_t flags,
 TPPTokenID end_token_1, TPPTokenID end_token_2) {
 DEE_LVERBOSE2("Parsing attribute: %q\n",TPPLexer_TokenIDStr(DeeLexer_LEXER(self),token.tk_id));
 switch (_TPPLexer_GetKeywordWithoutUnderscores(DeeLexer_LEXER(self),token.tk_id)) {

  case KWD_super: {
   DeeExprAstObject *new_super;
   DeeTypeObject *predicted_super_type;
   if (!yield()) return -1;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_ATTRIBUTE_SUPER,(DeeObject *)self,token_ob,
     "Expected '(' after '__attribute__' '(' '(' 'super'") == -1) return -1;
   } else if (!yield()) return -1;
   Dee_XDECREF(attr->ob_super);
   if ((new_super = (DeeExprAstObject *)DeeLexer_ParseExpression((DeeObject *)self,
    (DeeObject *)scope,flags|DEE_PARSER_FLAG_GENERATE_CONSTANT)) == NULL) return -1;
   Dee_XDECREF(attr->ob_super);
   attr->ob_super = new_super; // Install the new super ast
   if (!DeeExprAst_IsNoeffect((DeeObject *)new_super,(DeeObject *)scope)) {
    if (DeeError_CompilerErrorf(DEE_WARNING_ATTRIBUTE_SUPER_EXPRESSION_HAS_SIDE_EFFECTS,
     (DeeObject *)self,(DeeObject *)new_super->ob_token,
     "Argument of attribute super has side-effects but may be evaluated more than once: %r",
     new_super) == -1) return -1;
   }
   predicted_super_type = DeeExprAst_PredictType((DeeObject *)new_super,(DeeObject *)scope);
   if (predicted_super_type && !DeeType_Check(predicted_super_type)) {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_TYPE_FOR_ATTRIBUTE_SUPER,
     (DeeObject *)self,(DeeObject *)new_super->ob_token,
     "Expected 'type' as first argument to attribute super (got: '%#q')",
     DeeType_NAME(predicted_super_type)) == -1) return -1;
   }
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_ATTRIBUTE_SUPER,(DeeObject *)self,token_ob,
     "Expected ')' after '__attribute__' '(' '(' 'super' '(' <expr>") == -1) return -1;
   } else if (!yield()) return -1;
  } break;

  case KWD_guid: {
   DeeExprAstObject *temp;
   DeeStringObject *guid_string;
   if (!yield()) return -1;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_ATTRIBUTE_GUID,(DeeObject *)self,token_ob,
     "Expected '(' after '__attribute__' '(' '(' 'guid'") == -1) return -1;
   } else if (!yield()) return -1;
   if ((temp = (DeeExprAstObject *)DeeLexer_ParseExpression((DeeObject *)self,
    (DeeObject *)scope,flags|DEE_PARSER_FLAG_GENERATE_CONSTANT)) == NULL) return -1;
   if (temp->ob_kind == DeeExprAstKind_CONSTANT) {
    if (DeeString_Check(temp->ob_const)) {
     Dee_INCREF(guid_string = (DeeStringObject *)temp->ob_const);
    } else {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_STRING_CONSTANT_EXPRESSION_AFTER_ATTRIBUTE_GUID,(DeeObject *)self,token_ob,
      "Expected string after '__attribute__' '(' '(' 'guid' '('") == -1) {err_guid_temp: Dee_DECREF(temp); return -1; }
     guid_string = (DeeStringObject *)DeeObject_Str(temp->ob_const);
     if (!guid_string) DeeError_Handled();
    }
   } else {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_ATTRIBUTE_GUID,(DeeObject *)self,token_ob,
     "Expected const_expr after '__attribute__' '(' '(' 'guid' '('") == -1) goto err_guid_temp;
    guid_string = NULL;
   }
   Dee_DECREF(temp);
   if (guid_string) {
    char const *invalid_char;
    if (!DeeGUID_FromString(&attr->ob_guid,DeeString_STR(guid_string),&invalid_char)) {
     if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_GUID_STRING,(DeeObject *)self,token_ob,
      "Invalid guid string: %r (invalid character '%c' | pos %Iu)",
      guid_string,*invalid_char,(Dee_size_t)(invalid_char-DeeString_STR(guid_string))
      ) == -1) { Dee_DECREF(guid_string); goto err_guid_temp; }
    } else {
     attr->ob_flags |= DEE_ATTRIBUTE_DECL_FLAGS_GUID;
    }
    Dee_DECREF(guid_string);
   }
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_ATTRIBUTE_GUID,(DeeObject *)self,token_ob,
     "Expected ')' after '__attribute__' '(' '(' 'guid' '(' <const_expr>") == -1) return -1;
   } else if (!yield()) return -1;
  } break;

  case KWD_packed:
   attr->ob_alignment = 1;
   goto y1;

  case KWD_copyable:
   attr->ob_flags |= DEE_ATTRIBUTE_DECL_FLAGS_COPYABLE;
   goto y1;

  case KWD_aligned:
   if (!yield()) return -1;
   return _DeeLexer_ParseAlignedAttribute(attr,self,scope,flags);

  case KWD_syssv:    attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_SYSV; goto y1;
  case KWD_stdcall:  attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_STDCALL; goto y1;
  case KWD_thiscall: attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_THISCALL; goto y1;
  case KWD_fastcall: attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_FASTCALL; goto y1;
  case KWD_cdecl:    attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_MS_CDECL; goto y1;
  case KWD_pascal:   attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_PASCAL; goto y1;
  case KWD_register: attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_REGISTER; goto y1;
  case KWD_win64:    attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_WIN64; goto y1;
  case KWD_unix64:   attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_UNIX64;
y1: if (!yield()) return -1; break;

  case KWD_add_result_reference:
   if (!yield()) return -1;
   attr->ob_fun_flags |= DeeFunctionFlags_ADD_RESULT_REFERENCE;
   break;

  case KWD_noreturn:
   if (!yield()) return -1;
   // Someday, but not now...
   break;

  case KWD_nothrow:
   if (_DeeLexer_ParseAttributeNothrowDecl(attr,self,scope,flags) != 0) return -1;
   break;


  case KWD_deprecated: {
   DeeExprAstObject *temp;
   if (!yield()) return -1;
   if (token.tk_id != '(') {
    DeeString_NEW_STATIC(_depr_default,"deprecated identifier");
    Dee_XDECREF(attr->ob_depr);
    Dee_INCREF(attr->ob_depr = (DeeStringObject *)&_depr_default);
   } else {
     if (!yield()) return -1;
    if ((temp = (DeeExprAstObject *)DeeLexer_ParseExpression((DeeObject *)self,
     (DeeObject *)scope,flags|DEE_PARSER_FLAG_GENERATE_CONSTANT)) == NULL) return -1;
    if (temp->ob_kind == DeeExprAstKind_CONSTANT) {
     if (DeeString_Check(temp->ob_const)) {
      Dee_XDECREF(attr->ob_depr);
      Dee_INCREF(attr->ob_depr = (DeeStringObject *)temp->ob_const);
     } else {
      DeeStringObject *temp2;
      if (DeeError_CompilerError(DEE_WARNING_EXPECTED_STRING_CONSTANT_EXPRESSION_AFTER_ATTRIBUTE_DEPRECATED,(DeeObject *)self,token_ob,
       "Expected string after '__attribute__' '(' '(' 'deprecated' '('") == -1) {err_depr_temp: Dee_DECREF(temp); return -1; }
      temp2 = (DeeStringObject *)DeeObject_Str(temp->ob_const);
      if (!temp2) DeeError_Handled(); else {
       Dee_XDECREF(attr->ob_depr);
       Dee_INHERIT_REF(attr->ob_depr,temp2);
      }
     }
    } else {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_ATTRIBUTE_DEPRECATED,(DeeObject *)self,token_ob,
      "Expected const_expr after '__attribute__' '(' '(' 'deprecated' '('") == -1) goto err_depr_temp;
    }
    Dee_DECREF(temp);
    if (token.tk_id != ')') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_ATTRIBUTE_DEPRECATED,(DeeObject *)self,token_ob,
      "Expected ')' after '__attribute__' '(' '(' 'deprecated' '(' <const_expr>") == -1) return -1;
    } else if (!yield()) return -1;
   }
  } break;


  case KWD_name: {
   DeeExprAstObject *temp;
   if (!yield()) return -1;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_ATTRIBUTE_NAME,(DeeObject *)self,token_ob,
     "Expected '(' after '__attribute__' '(' '(' 'name'") == -1) return -1;
   } else if (!yield()) return -1;
   if ((temp = (DeeExprAstObject *)DeeLexer_ParseExpression((DeeObject *)self,
    (DeeObject *)scope,flags|DEE_PARSER_FLAG_GENERATE_CONSTANT)) == NULL) return -1;
   if (temp->ob_kind == DeeExprAstKind_CONSTANT) {
    if (DeeString_Check(temp->ob_const)) {
     Dee_XDECREF(attr->ob_name);
     Dee_INCREF(attr->ob_name = (DeeStringObject *)temp->ob_const);
    } else {
     DeeStringObject *temp2;
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_STRING_CONSTANT_EXPRESSION_AFTER_ATTRIBUTE_NAME,(DeeObject *)self,token_ob,
      "Expected string after '__attribute__' '(' '(' 'name' '('") == -1) {err_name_temp: Dee_DECREF(temp); return -1; }
     temp2 = (DeeStringObject *)DeeObject_Str(temp->ob_const);
     if (!temp2) DeeError_Handled(); else {
      Dee_XDECREF(attr->ob_name);
      Dee_INHERIT_REF(attr->ob_name,temp2);
     }
    }
   } else {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_ATTRIBUTE_NAME,(DeeObject *)self,token_ob,
     "Expected const_expr after '__attribute__' '(' '(' 'name' '('") == -1) goto err_name_temp;
   }
   Dee_DECREF(temp);
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_ATTRIBUTE_NAME,(DeeObject *)self,token_ob,
     "Expected ')' after '__attribute__' '(' '(' 'name' '(' <const_expr>") == -1) return -1;
   } else if (!yield()) return -1;
  } break;

  default: {
   int paren_rec = 0;
   if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
    if (DeeError_CompilerErrorf(DEE_WARNING_ATTRIBUTE_UNKNOWN,(DeeObject *)self,token_ob,
     "Unknown attribute: %s",TPPLexer_TokenIDStr(DeeLexer_LEXER(self),token.tk_id)) == -1) return -1;
    if (!yield()) return -1;
   } else {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_ATTRIBUTE,(DeeObject *)self,token_ob,
     "Expected <keyword> after '__attribute__' '(' '('") == -1) return -1;
   }
   // Try to consume the attribute with all its arguments
   while (token.tk_id) {
    if ((token.tk_id == end_token_1 || token.tk_id == end_token_2) && !paren_rec) break;
    if (token.tk_id == '(') ++paren_rec; else
    if (token.tk_id == ')') --paren_rec;
    if (!yield()) return -1;
   }
  } break;
 }
 return 0;
}

int _DeeLexer_ParseAttributeDecl(
 struct DeeAttributeDecl *attr, struct DeeLexerObject *self,
 struct DeeScopeObject *scope, Dee_uint32_t flags) {
again:
 switch (token.tk_id) {

  case KWD__Noexcept:
   if (_DeeLexer_ParseAttributeNothrowDecl(attr,self,scope,flags) != 0) return -1;
   break;

  case KWD__Noreturn:
   goto y1;

  case KWD__Alignas:
   if (!yield()) return -1;
   if (_DeeLexer_ParseAlignedAttribute(attr,self,scope,flags) == -1) return -1;
   goto again;

  DEE_PARSE_TOKENID_CASE_CC_SOME(syssv)
   attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_SYSV; goto y1;
  DEE_PARSE_TOKENID_CASE_CC_SOME(stdcall)
   attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_STDCALL; goto y1;
  DEE_PARSE_TOKENID_CASE_CC_SOME(thiscall)
   attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_THISCALL; goto y1;
  DEE_PARSE_TOKENID_CASE_CC_SOME(fastcall)
   attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_FASTCALL; goto y1;
  DEE_PARSE_TOKENID_CASE_CC_SOME(cdecl)
   attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_MS_CDECL; goto y1;
  DEE_PARSE_TOKENID_CASE_CC_SOME(pascal)
   attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_PASCAL; goto y1;
  DEE_PARSE_TOKENID_CASE_CC_SOME(register)
   attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_REGISTER; goto y1;
  DEE_PARSE_TOKENID_CASE_CC_SOME(win64)
   attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_WIN64; goto y1;
  DEE_PARSE_TOKENID_CASE_CC_SOME(unix64)
   attr->ob_fun_flags = (attr->ob_fun_flags&~DeeFunctionFlags_CC_MASK)|DeeFunctionFlags_UNIX64;
y1: if (!yield()) return -1;
   goto again;

#define CAREFUL_ARRAY 1
  case '[': {
#if CAREFUL_ARRAY
   DeeTokenObject *stored_token;
   Dee_INCREF(stored_token = (DeeTokenObject *)token_ob);
#endif
   if (!yield()) return -1;
   if (token.tk_id != '[') {
#if CAREFUL_ARRAY
    int temp = DeeLexer_Return((DeeObject *)self,(DeeObject *)stored_token);
    Dee_DECREF(stored_token);
    return temp;
#else
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LBRACKET_AFTER_BRACKET_ATTRIBUTE,(DeeObject *)self,token_ob,
     "Expected '[' after '['") == -1) return -1;
#endif
   } else {
#if CAREFUL_ARRAY
    Dee_DECREF(stored_token);
#endif
    if (!yield()) return -1;
   }
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   while (token.tk_id != ']')
#else
   while (1)
#endif
   {
    if (_DeeLexer_ParseAttributeContent(attr,self,scope,flags,']',',') == -1) return -1;
    if (token.tk_id != ',') break;
    if (!yield()) return -1;
   }
   if (token.tk_id != ']') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_BRACKET_ATTRIBUTE_1,(DeeObject *)self,token_ob,
     "Expected ']' after '[' '[' ...") == -1) return -1;
   } else if (!yield()) return -1;
   if (token.tk_id != ']') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_BRACKET_ATTRIBUTE_2,(DeeObject *)self,token_ob,
     "Expected ']' after '[' '[' ... ']'") == -1) return -1;
   } else if (!yield()) return -1;
   goto again;
  }

  case KWD___attribute:
  case KWD___attribute__:
   if (!yield()) return -1;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_ATTRIBUTE_1,(DeeObject *)self,token_ob,
     "Expected '(' after '__attribute__'") == -1) return -1;
   } else if (!yield()) return -1;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_ATTRIBUTE_2,(DeeObject *)self,token_ob,
     "Expected '(' after '__attribute__' '('") == -1) return -1;
   } else if (!yield()) return -1;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   while (token.tk_id != ')')
#else
   while (1)
#endif
   {
    if (_DeeLexer_ParseAttributeContent(attr,self,scope,flags,')',',') == -1) return -1;
    if (token.tk_id != ',') break;
    if (!yield()) return -1;
   }
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_ATTRIBUTE_1,(DeeObject *)self,token_ob,
     "Expected ')' after '__attribute__' '(' '(' ...") == -1) return -1;
   } else if (!yield()) return -1;
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_ATTRIBUTE_2,(DeeObject *)self,token_ob,
     "Expected ')' after '__attribute__' '(' '(' ... ')'") == -1) return -1;
   } else if (!yield()) return -1;
   goto again;

  case KWD__declspec:
  case KWD___declspec:
   if (!yield()) return -1;
   if (token.tk_id != '(') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_DECLSPEC,(DeeObject *)self,token_ob,
     "Expected '(' after '__declspec'") == -1) return -1;
   } else if (!yield()) return -1;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
   while (token.tk_id != ')')
#else
   while (1)
#endif
   {
    if (_DeeLexer_ParseAttributeContent(attr,self,scope,flags,')',',') == -1) return -1;
    if (token.tk_id != ',') break;
    if (!yield()) return -1;
   }
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_DECLSPEC,(DeeObject *)self,token_ob,
     "Expected ')' after '__declspec' '(' ...") == -1) return -1;
   } else if (!yield()) return -1;
   goto again;

  // Not an attribute
  default: break;
 }
 return 0;
}

struct DeeExprAstObject *_DeeLexer_ParseForeignClosureCodeBlock(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags,
 struct DeeAttributeDecl *attr, struct DeeForeignFunctionTypeObject *type,
 struct _DeeTokenIDListObject *argnames, DEE_A_IN_OPT struct DeeTokenObject *name_token) {
 DeeExprAstObject *result;
 // NOTE: For now, we are just parsing these kinds of functions as regular functions
 DEE_LVERBOSE1("TODO: foreign_closure: %k\n",type);
 if ((result = (DeeExprAstObject *)DeeExprAst_New(DeeExprAstKind_FUNCTION,
  name_token ? (DeeObject *)name_token : token_ob)) == NULL) return NULL;
 if (!argnames) argnames = (struct _DeeTokenIDListObject *)_DeeTokenIDList_EmptyList;
 Dee_INCREF(result->ob_fun_args = argnames);
 if (name_token && ((result->ob_fun_name = (DeeStringObject *)DeeString_New(
  TPPLexer_TokenIDStr(DeeLexer_LEXER(self),name_token->ob_token.tk_id))) == NULL)) goto err_r;
 if (token.tk_id == '{' || token.tk_id == TPP_TOK_ARROW) {
  int expr_func = token.tk_id == TPP_TOK_ARROW;
  if (!yield()) {err_r: Dee_DECREF(result); return NULL; }
  if ((result->ob_scope = (DeeScopeObject *)DeeScope_New((DeeObject *)scope,0)) == NULL) goto err_r;
  if (DeeScope_SetupParameters((DeeObject *)result->ob_scope,
   _DeeTokenIDList_SIZE(argnames),
   _DeeTokenIDList_ELEM(argnames),attr) == -1) goto err_r;
  if (expr_func) {
   result->ob_kind = DeeExprAstKind_FUNCTION_EXPR;
   if ((result->ob_fun_expr = (DeeExprAstObject *)DeeLexer_ParseExpressionLv2(
    (DeeObject *)self,(DeeObject *)result->ob_scope,flags&~(
    DEE_PARSER_FLAG_UNREACHABLE|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE))) == NULL) goto err_r;
  } else {
   if ((result->ob_fun_stmt = (DeeStmtAstObject *)DeeLexer_ParseStatements(
    (DeeObject *)self,(DeeObject *)result->ob_scope,flags&~(
    DEE_PARSER_FLAG_UNREACHABLE|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE),'}')) == NULL) goto err_r;
  }
 } else {
  result->ob_kind = DeeExprAstKind_FUNCTION_NONE;
 }
 return result;
}

int _DeeLexer_ParseConstDecl(
 struct DeeLexerObject *self, struct DeeScopeObject *scope, Dee_uint32_t flags) {
 TPPTokenID const_name;
 DeeObject *new_const;
 DeeExprAstObject *const_ast;
 int need_semicollon;
 while (1) {
  struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
  if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_attr;
  switch ((const_name = token.tk_id)) {
   case KWD_function:
    new_const = (DeeObject *)_DeeLexer_ParseConstFunction(self,scope,flags);
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
drop_const:
#endif
    Dee_DECREF(new_const);
    need_semicollon = 0;
    goto next_const2;
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
   case KWD_class:
    if ((new_const = (DeeObject *)_DeeLexer_ParseConstClass(
     self,scope,flags|DEE_PARSER_FLAG_GENERATE_CONSTANT)) == NULL) goto err_attr;
    goto drop_const;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES
   case KWD_struct:
   case KWD_union:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES */
#if DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES
   case KWD_enum:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES */
    goto unnamed;
   default: break;
  }
  if (!TPPTokenID_IS_KEYWORD(const_name)) {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_CONST,(DeeObject *)self,token_ob,
    "Expected <keyword> after 'const'") == -1) {err_attr: DeeAttributeDecl_Quit(&attr); return -1; }
#if DEE_CONFIG_LANGUAGE_HAVE_STRUCT_TYPES || \
    DEE_CONFIG_LANGUAGE_HAVE_ENUM_TYPES || \
    DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
unnamed:
#endif
   const_name = 0;
  } else {
   if (!yield()) goto err_attr;
   if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_attr;
  }
  if (token.tk_id != '=') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_ASSIGN_AFTER_UNKNOWN_CONST,(DeeObject *)self,token_ob,
    "Expected '=' after 'const' <keyword>") == -1) goto err_attr;
  } else {
   if (!yield()) goto err_attr;
   if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_attr;
  }
  if ((const_ast = (DeeExprAstObject *)DeeLexer_ParseExpression(
   (DeeObject *)self,(DeeObject *)scope,flags|DEE_PARSER_FLAG_GENERATE_CONSTANT)
   ) == NULL) goto err_attr;
  need_semicollon = DeeExprAst_NeedSemicollon((DeeObject *)const_ast);
  if (_DeeLexer_ParseAttributeDecl(&attr,self,scope,flags) == -1) goto err_attr;
  if (const_ast->ob_kind != DeeExprAstKind_CONSTANT &&
      // We rely on the fact that modules share the same address as constants
      const_ast->ob_kind != DeeExprAstKind_MODULE) {
   if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_CONST,
    (DeeObject *)self,(DeeObject *)const_ast->ob_token,
    "Expected a constant expression after 'const' <keyword> '=' (got: %r)",const_ast) == -1) {
err_const: Dee_DECREF(const_ast); goto err_attr;
   }
   new_const = Dee_None;
  } else new_const = const_ast->ob_const;
  DEE_LVERBOSE2R("%s(%d) : Defining constant : %s = %r\n",
                 DeeToken_FILE(token_ob),DeeToken_LINE(token_ob)+1,
                 TPPLexer_TokenIDStr(DeeLexer_LEXER(self),const_name),new_const);
  if (const_name) {
   DeeObject *old_const;
   if ((old_const = _DeeScope_GetName(scope,const_name)) != NULL) {
    int temp;
    if (Dee_TYPE(old_const) == Dee_TYPE(new_const)) {
     temp = DeeObject_CompareEq(old_const,new_const);
     if (temp == -1) { DeeError_Handled(); temp = 0; }
    } else temp = 0;
    if (!temp && DeeError_CompilerErrorf(DEE_WARNING_CONST_NAME_ALREADY_USED,
     (DeeObject *)self,token_ob,"Constant name %s already used",
     TPPLexer_TokenIDStr(DeeLexer_LEXER(self),const_name)) == -1) goto err_const;
    goto next_const;
   }
   if ((_DeeScope_AddDeprecatedName(
    (DeeScopeObject *)scope,const_name,
    new_const,(DeeObject *)attr.ob_depr)) == -1) goto err_const;
  }
next_const:
  Dee_DECREF(const_ast);
next_const2:
  DeeAttributeDecl_Quit(&attr);
  if (token.tk_id != ',') break;
  if (!yield()) return -1;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
  if (token.tk_id == ';') break; // Stop parsing constants
#endif
 }
 if (token.tk_id != ';') {
  if (need_semicollon) {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_CONST,(DeeObject *)self,token_ob,
    "Expected ';' after 'const' <keyword> '=' <const_expr>") == -1) return -1;
  }
 } else if (!yield()) return -1;
 return 0;
}



DEE_A_RET_OBJECT_EXCEPT_REF(DeeStmtAstObject) *DeeLexer_ParseStatementOldScope(
 DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeScopeObject) *scope, Dee_uint32_t flags) {
 if (token.tk_id == '{') {
  if (!yield()) return NULL;
  return DeeLexer_ParseStatements(self,scope,flags,'}');
 } else {
  return DeeLexer_ParseStatement(self,scope,flags);
 }
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStmtAstObject) *DeeLexer_ParseStatementNewScope(
 DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeScopeObject) *scope, Dee_uint32_t flags) {
 if (token.tk_id != '{') {
  DeeStmtAstObject *result,*ast1;
  DEE_A_REF struct DeeStmtAstObject **stmtv;
  DeeScopeObject *new_scope;
  if ((new_scope = (DeeScopeObject *)DeeScope_New(scope,1)) == NULL) return NULL;
  ast1 = (DeeStmtAstObject *)DeeLexer_ParseStatement(self,(DeeObject *)new_scope,flags);
  if (!ast1) { err_s: Dee_DECREF(scope); return NULL; }
  stmtv = (struct DeeStmtAstObject **)mallocf_nz(sizeof(struct DeeStmtAstObject *),"Single statement");
  if (!stmtv) { DeeError_NoMemory(); err_a1: Dee_DECREF(ast1); goto err_s; }
  result = (DeeStmtAstObject *)_DeeStmtAst_NewUnsafe(DeeLexer_TOKEN(self));
  if (!result) { free_nn(stmtv); goto err_a1; }
  Dee_INHERIT_REF(*stmtv,ast1);
  result->ob_kind = DeeStmtAstKind_STMTS;
  result->ob_stmts.ob_stmts_c = 1;
  result->ob_stmts.ob_stmts_v = stmtv;
  Dee_INHERIT_REF(result->ob_stmts.ob_stmts_scope,new_scope);
  return (DeeObject *)result;
 }
 return DeeLexer_ParseStatement(self,scope,flags);
}


DEE_COMPILER_MSVC_WARNING_PUSH(4701 4703)
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStmtAstObject) *DeeLexer_ParseStatement(
DEE_A_INOUT DeeObject *self, DEE_A_IN_OBJECT(DeeScopeObject) *scope, Dee_uint32_t flags) {
 static char const expected_semicolon_stmt_end[] = "Expected ';' to end statement";
 DeeStmtAstObject *result;
 DeeTokenObject *tok;
 DeeObject *new_scope;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 result = (DeeStmtAstObject *)_DeeStmtAst_NewUnsafe(DeeLexer_TOKEN(self));
 if (result) {
  goto first;
again:
  Dee_DECREF(result->ob_token);
  Dee_INCREF(result->ob_token = (DeeTokenObject *)token_ob);
first:
  if ((flags&DEE_PARSER_FLAG_UNREACHABLE)!=0) {
   if (DeeError_CompilerError(DEE_WARNING_UNREACHABLE_CODE,
    self,token_ob,"Unreachable code") == -1) goto err;
  }
  switch (token.tk_id) {

   // ';'
   case ';': // empty statement
    if (!yield()) goto err;
    result->ob_kind = DeeStmtAstKind_EMPTY;
    break;

   case KWD_switch: {
    DeeScopeObject *weak_root;
    struct DeeCaseLabelDeclList *old_cases;
    if (!yield()) goto err;
    if (token.tk_id != '(') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_SWITCH,self,
      (DeeObject *)token_ob,"expected '(' after 'switch'") == -1) goto err;
    } else if (!yield()) goto err;
    if ((new_scope = DeeScope_New(scope,1)) == NULL) goto err;
    if ((result->ob_switch.ob_sw_expr = (DeeExprAstObject *)
     DeeLexer_ParseExpressionLv2(self,new_scope,flags)) == NULL) goto errs;
    if (token.tk_id != ')') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_SWITCH,self,
      (DeeObject *)token_ob,"expected ')' after 'switch'") == -1) {errs_sw_exast: Dee_DECREF(result->ob_switch.ob_sw_expr); goto errs;}
    } else if (!yield()) goto errs_sw_exast;
    DeeCaseLabelDeclList_Init(&result->ob_switch.ob_sw_cases);
    weak_root = (DeeScopeObject *)DeeScope_WeakRoot(scope);
    old_cases = weak_root->ob_cases;
    weak_root->ob_cases = &result->ob_switch.ob_sw_cases;
    result->ob_switch.ob_sw_stmt = (DeeStmtAstObject *)DeeLexer_ParseStatement(
     self,new_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE);
    weak_root->ob_cases = old_cases;
    if (!result->ob_switch.ob_sw_stmt) {
     DeeCaseLabelDeclList_Quit(&result->ob_switch.ob_sw_cases);
     goto errs_sw_exast;
    }
    Dee_INHERIT_REF(result->ob_switch.ob_sw_scope,*(DeeScopeObject **)&new_scope);
    result->ob_kind = DeeStmtAstKind_SWITCH;
    if (!result->ob_switch.ob_sw_cases.ob_c) {
     if (DeeError_CompilerError(DEE_WARNING_SWITCH_HAS_NO_CASES,self,(DeeObject *)result->ob_token,
      "switch statement has no cases") == -1) goto err_r;
    } else if (result->ob_switch.ob_sw_cases.ob_c == 1 &&
               !result->ob_switch.ob_sw_cases.ob_v[0].ob_case) {
     if (DeeError_CompilerError(DEE_WARNING_SWITCH_HAS_ONLY_DEFAULT_CASE,self,(DeeObject *)result->ob_token,
      "switch statement only has a default case") == -1) goto err_r;
    }
   } break;

   case KWD_default: {
    DeeScopeObject *weak_root;
    if ((weak_root = (DeeScopeObject *)DeeScope_WeakRoot(scope))->ob_cases == NULL) {
     if (DeeError_CompilerError(DEE_WARNING_DEFAULT_ONLY_ALLOWED_IN_SWITCH_STATEMENTS,self,
      token_ob,"'default' is only allowed in switch statements") == -1) goto err;
    } else {
     if (DeeCaseLabelDeclList_HasDefault(weak_root->ob_cases,NULL)) {
      if (DeeError_CompilerError(DEE_WARNING_DEFAULT_LABEL_ALREADY_USED,self,
       (DeeObject *)token_ob,"'default' label already used") == -1) goto err;
     } else {
      Dee_size_t case_id; // Add the case label to the expression
      if ((case_id = DeeCaseLabelDeclList_AddDefault(
       weak_root->ob_cases)) == (Dee_size_t)-1) goto err;
      if (DeeLabelRefList_Add(&result->ob_label_endpoints,
       DEE_LABEL_REF_FLAGS_CASELABEL,case_id) == -1) goto err;
     }
    }
    if (!yield()) goto err;
    if (token.tk_id != ':') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_COLLON_AFTER_DEFAULT,self,token_ob,
      "Expected ':' after 'default'") == -1) goto err;
    } else if (!yield()) goto err;
   } goto again;

   case KWD_case: {
    DeeObject *case_lo,*case_hi;
    DeeExprAstObject *case_ast;
    DeeScopeObject *weak_root;
    DeeTokenObject *case_token;
    Dee_INCREF(case_token = (DeeTokenObject *)token_ob);
    if (!yield()) {err_case_tok: Dee_DECREF(case_token); goto err; }
    if ((case_ast = (DeeExprAstObject *)DeeLexer_ParseExpression(self,scope,flags)) == NULL) goto err_case_tok;
    if (case_ast->ob_kind == DeeExprAstKind_EXPAND) {
     if (case_ast->ob_op_a->ob_kind != DeeExprAstKind_CONSTANT) {
      if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_CASE,self,
       (DeeObject *)case_ast->ob_op_a->ob_token,"Expected constant expression after 'case'") == -1) {err_case_expr: Dee_DECREF(case_ast); goto err_case_tok; }
      Dee_INCREF(case_lo = Dee_None);
     } else {
      Dee_INCREF(case_lo = case_ast->ob_op_a->ob_const);
     }
     Dee_DECREF(case_ast);
     if ((case_ast = (DeeExprAstObject *)DeeLexer_ParseExpression(self,scope,flags)) == NULL) {err_case_lo: Dee_DECREF(case_lo); goto err_case_tok; }
     if (case_ast->ob_kind != DeeExprAstKind_CONSTANT) {
      if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_CASE,self,
       (DeeObject *)case_ast->ob_token,"Expected constant expression after 'case'") == -1) { Dee_DECREF(case_ast); goto err_case_lo; }
      Dee_INCREF(case_hi = Dee_None);
     } else {
      Dee_INCREF(case_hi = case_ast->ob_const);
     }
    } else if (case_ast->ob_kind != DeeExprAstKind_CONSTANT) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_CASE,self,
      (DeeObject *)case_ast->ob_token,"Expected constant expression after 'case'") == -1) goto err_case_expr;
     Dee_INCREF(case_lo = Dee_None);
     case_hi = NULL;
    } else {
     Dee_INCREF(case_lo = case_ast->ob_const);
     case_hi = NULL;
    }
    Dee_DECREF(case_ast);
    // Done parsing case / case range
    if (token.tk_id != ':') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_COLLON_AFTER_CASE_EXPR,self,token_ob,
      "Expected ':' after 'case' <expr>") == -1) {err_case_hi: Dee_XDECREF(case_hi); goto err_case_lo;}
    } else if (!yield()) goto err_case_hi;
    if ((weak_root = (DeeScopeObject *)DeeScope_WeakRoot(scope))->ob_cases) {
     if (case_hi
         ? DeeCaseLabelDeclList_HasRange(weak_root->ob_cases,case_lo,case_hi)
         : DeeCaseLabelDeclList_Has(weak_root->ob_cases,case_lo)) {
      if (DeeError_CompilerError(DEE_WARNING_CASE_LABEL_ALREADY_USED,self,
       (DeeObject *)case_token,"'case' label already used") == -1) goto err_case_hi;
     } else {
      Dee_size_t case_id; // Add the case label to the expression
      if ((case_id = (case_hi
       ? DeeCaseLabelDeclList_AddRange(weak_root->ob_cases,case_lo,case_hi)
       : DeeCaseLabelDeclList_Add(weak_root->ob_cases,case_lo)
       )) == (Dee_size_t)-1) goto err_case_hi;
      if (DeeLabelRefList_Add(&result->ob_label_endpoints,
       DEE_LABEL_REF_FLAGS_CASELABEL,case_id) == -1) goto err_case_hi;
     }
    } else {
     if (DeeError_CompilerError(DEE_WARNING_CASE_ONLY_ALLOWED_IN_SWITCH_STATEMENTS,self,
      (DeeObject *)case_token,"'case' is only allowed in switch statements") == -1) goto err_case_hi;
    }
    Dee_DECREF(case_token);
    Dee_DECREF(case_lo);
    Dee_XDECREF(case_hi);
   } goto again;

   // 'module' keyword stmt
   case KWD_module: {
    TPPTokenID module_name;
    DeeModuleObject *module;
    struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
    int temp;
    if (!yield()) {err_mod_attr: DeeAttributeDecl_Quit(&attr); goto err; }
    if (_DeeLexer_ParseAttributeDecl(&attr,(DeeLexerObject *)self,(DeeScopeObject *)scope,flags) == -1) goto err_mod_attr;
    if (!TPPTokenID_IS_KEYWORD((module_name = token.tk_id))) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_MODULE,self,token_ob,
      "Expected <keyword> after 'module'") == -1) goto err_mod_attr;
     goto again; // Parse in the parent module
    }
    if ((module = (DeeModuleObject *)_DeeScope_GetName((DeeScopeObject *)scope,module_name)) != NULL) {
     // If it's already a module, just add additional elements
     if (!DeeModule_Check(module)) {
      if (DeeError_CompilerError(DEE_WARNING_MODULE_NAME_ALREADY_USED,
       self,token_ob,"Module name already used by local/const") == -1) goto err_mod_attr;
      if (!yield()) goto err_mod_attr;
      goto again; // Parse in the parent module
     }
     if (!yield()) goto err_mod_attr;
    } else {
     if (!yield()) goto err_mod_attr;
     if (_DeeLexer_ParseAttributeDecl(&attr,(DeeLexerObject *)self,(DeeScopeObject *)scope,flags) == -1) goto err_mod_attr;
     if ((module = (DeeModuleObject *)DeeModule_New(module_name,scope,NULL)) == NULL) goto err;
     temp = _DeeScope_AddDeprecatedName(
      (DeeScopeObject *)scope,module_name,
      (DeeObject *)module,(DeeObject *)attr.ob_depr);
     Dee_DECREF(module); // Module kept alive through reference in scope
     if (temp == -1) goto err_mod_attr;
    }
    DeeAttributeDecl_Quit(&attr);
    // Really hacky hack to allow 'module x::y' as alias for 'module x module y'
    if (token.tk_id == TPP_TOK_COLLON_COLLON || token.tk_id == '.') token.tk_id = KWD_module;
    // Parse the module scope block
    if ((result->ob_module.ob_mod_block = (DeeStmtAstObject *)DeeLexer_ParseStatementOldScope(
     self,(DeeObject *)module->ob_scope,flags)) == NULL) goto err;
    Dee_INCREF(result->ob_module.ob_mod_module = module);
    result->ob_kind = DeeStmtAstKind_MODULE;
   } break;

   // 'const' (',' ~~ (keyword '=' const_expr)...) RELAXED_COMMA ';'
   case KWD_const: {
    if (!yield()) goto err;
    if (_DeeLexer_ParseConstDecl((DeeLexerObject *)self,(DeeScopeObject *)scope,flags) == -1) goto err;
    result->ob_kind = DeeStmtAstKind_EMPTY;
   } break;

   case KWD_typedef: {
    struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
    DEE_A_REF DeeTypeObject *base_type;
    if (!yield()) goto err;
    if (_DeeLexer_ParseAttributeDecl(&attr,
     (DeeLexerObject *)self,(DeeScopeObject *)scope,flags) == -1) goto err_td_attr;
    if ((base_type = _DeeLexer_ParseConstantType(
     (DeeLexerObject *)self,(DeeScopeObject *)scope,flags|
     DEE_PARSER_FLAG_NO_TYPE_SUFFIX|DEE_PARSER_FLAG_GENERATE_CONSTANT)) == NULL) {
err_td_attr: DeeAttributeDecl_Quit(&attr); goto err;
    }
    while (1) { // Parse names for this type
     DeeTokenObject *type_name;
     DeeTypeObject *used_type;
     if ((used_type = _DeeLexer_ParseNamedTypeWithBaseAttr(
      (DeeLexerObject *)self,(DeeScopeObject *)scope,
      flags,NULL,&type_name,NULL,base_type,&attr)) == NULL) {
err_td_base: Dee_DECREF(base_type); goto err_td_attr;
     }
     if (type_name) { // Register the type constant
      DeeTypeObject *old_type;
      if ((old_type = (DeeTypeObject *)_DeeScope_GetName(
       (DeeScopeObject *)scope,type_name->ob_token.tk_id)) != NULL) {
       if (old_type != used_type) { // Only emit the warning if it's a different type
        if (DeeError_CompilerErrorf(DEE_WARNING_TYPEDEF_TYPENAME_ALREADY_USED,
         self,token_ob,"typedef typename %s is already used by %s:%k",
         TPPLexer_TokenIDStr(DeeLexer_LEXER(self),type_name->ob_token.tk_id),
         DeeType_NAME(Dee_TYPE(old_type)),old_type) == -1) {
err_td_used_base_tyname: Dee_DECREF(type_name);
err_td_used_base: Dee_DECREF(used_type); goto err_td_base;
        }
       }
      } else {
       if ((_DeeScope_AddDeprecatedName((DeeScopeObject *)scope,
        type_name->ob_token.tk_id,(DeeObject *)used_type,
        (DeeObject *)attr.ob_depr)) == -1) goto err_td_used_base_tyname;
      }
      Dee_DECREF(type_name);
     }
     Dee_DECREF(used_type);
     if (token.tk_id != ',') break;
     if (!yield()) goto err_td_used_base;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
     if (token.tk_id == ';') break; // Stop parsing typedefs
#endif
    }
    Dee_DECREF(base_type);
    DeeAttributeDecl_Quit(&attr);
    if (token.tk_id != ';') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_TYPEDEF,self,token_ob,
      "Expected ';' after 'typedef' ...") == -1) goto err;
    } else if (!yield()) goto err;
    result->ob_kind = DeeStmtAstKind_EMPTY;
   } break;

   // '{' [stmt...] '}'
   case '{': {
    if (!yield()) goto err;
    if (DeeLexer_ParseStatements2_NewScope(result,self,scope,flags,'}') == -1) goto err;
   } break; // no ';' at the end of this kind of statement

   // 'print' [expr ':'] (',' ~~ expr...) RELAXED_COMMA ';'
   case KWD_print: {
    int has_comma;
    result->ob_kind = DeeStmtAstKind_PRINT;
    if (!yield()) goto err;
    if (token.tk_id == ';') {
empty_print:
     result->ob_print.ob_prt_file = NULL; // print simple linefeed
     result->ob_print.ob_prt_tpl = NULL; // print simple linefeed
    } else if (token.tk_id == ',') {
     if (!yield()) goto err;
     result->ob_kind = DeeStmtAstKind_PRINT_NOLF;
     goto empty_print;
    } else {
     DeeExprAstObject *first = (DeeExprAstObject *)
      DeeLexer_ParseExpression(self,scope,flags);
     if (!first) goto err;
     if (token.tk_id == ':') {
      DeeTypeObject *predicted_file_type;
      if (!yield()) {err_first: Dee_DECREF(first); goto err; }
      if ((predicted_file_type = DeeExprAst_PredictType((DeeObject *)first,scope)) != NULL &&
          predicted_file_type != (DeeTypeObject *)&DeeNone_Type &&
          !DeeFileType_Check(predicted_file_type)) {
       if (DeeError_CompilerErrorf(DEE_WARNING_PRINT_FILE_NOT_A_FILE,self,token_ob,
        "Using non-file type '%#q' as print destination will throw an 'Error.TypeError' at runtime",
        DeeType_NAME(predicted_file_type)) == -1) goto err;
      }
      Dee_INHERIT_REF(result->ob_print.ob_prt_file,first);
      first = NULL; // Must be set to NULL
      if (token.tk_id == ';') { // 'print' <expr> ':' ';'
       result->ob_print.ob_prt_tpl = NULL;
       goto print_end;
      } else if (token.tk_id == ',') { // 'print' <expr> ':' ','
       if (!yield()) goto err;
       result->ob_kind = DeeStmtAstKind_PRINT_NOLF;
       result->ob_print.ob_prt_tpl = NULL; // print simple linefeed
       goto print_end;
      }
     } else {
      result->ob_print.ob_prt_file = NULL;
      if (token.tk_id != ',') {
print_single:
       if ((result->ob_print.ob_prt_tpl = (DeeExprAstObject *)
        DeeExprAst_New(DeeExprAstKind_TUPLE,(DeeObject *)first->ob_token)
        ) == NULL) goto err_first;
       if ((result->ob_print.ob_prt_tpl->ob_args_a = 
        (DeeTupleObject *)_DeeTuple_NewUnsafe(1)) == NULL) {
        Dee_DECREF(result->ob_print.ob_prt_tpl);
        goto err_first;
       }
       // Set the expression we parsed as the print file as the single
       // print statement
       Dee_INHERIT_REF(DeeTuple_ELEM(
        result->ob_print.ob_prt_tpl->ob_args_a)[0],
        *(DeeObject **)&first);
       goto print_end;
      }
      if (!yield()) goto err_first;
      if (token.tk_id == ';') { // 'print' <expr> ',' ';'
       result->ob_kind = DeeStmtAstKind_PRINT_NOLF;
       goto print_single;
      }
     }
     has_comma = 0;
     result->ob_print.ob_prt_tpl = (DeeExprAstObject *)
      _DeeLexer_ParsePrintExpressionTuple(first,self,scope,flags,&has_comma);
     Dee_XDECREF(first);
     if (!result->ob_print.ob_prt_tpl) goto err;
     if (has_comma) result->ob_kind = DeeStmtAstKind_PRINT_NOLF;
    }
print_end:
    if ((flags&DEE_PARSER_FLAG_OPTIMIZE_PRINT_STDOUT)!=0 &&
        result->ob_print.ob_prt_file &&
        result->ob_print.ob_prt_file->ob_kind == DeeExprAstKind_CONSTANT &&
        result->ob_print.ob_prt_file->ob_const == DeeFile_Std(DEE_STDOUT)) {
     // No need to specify a constant file ast, that points to stdout
     Dee_CLEAR(result->ob_print.ob_prt_file);
    }
    if (result->ob_print.ob_prt_tpl) {
     DeeTupleObject *expr_list;
     DEE_ASSERT(result->ob_print.ob_prt_tpl->ob_kind == DeeExprAstKind_TUPLE);
     expr_list = result->ob_print.ob_prt_tpl->ob_args_a;
     if (!DeeExprAst_NeedSemicollon(DeeExprAst_End(
      DeeTuple_GET(expr_list,DeeTuple_SIZE(expr_list)-1)))
      // If the last expression of a paren-less 'print' statement
      // is a function, we don't require a ';' to be following
      ) break;
    }
    if (token.tk_id != ';') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_PRINT,self,token_ob,
      "expected ';' after 'print' [<expr> [',' <expr>]... [',']]") == -1) goto err_ptpl;
    } else if (!yield()) { err_ptpl: Dee_XDECREF(result->ob_print.ob_prt_tpl); goto err; }
   } break;

   // 'return' ';'
   // 'return' exprLv2 ';'
   case KWD_return: {
    result->ob_kind = DeeStmtAstKind_RETURN;
    if (!yield()) goto err;
    if (token.tk_id == ';') {
     result->ob_return.ob_ret_expr = NULL; // No return value
    } else {
     DeeScopeObject *weak_root = (DeeScopeObject *)DeeScope_WeakRoot(scope);
     if ((weak_root->ob_flags&DEE_SCOPE_FLAG_FOUND_YIELD)!=0) {
      if (DeeError_CompilerError(DEE_WARNING_RETURN_AFTER_YIELD,self,token_ob,
       "'return' statement found after a 'yield' statement was already encountered"
       ) == -1) goto err_rval;
      result->ob_kind = DeeStmtAstKind_YIELD; // Act like it's a yield
      goto parse_yield_expr;
     } else weak_root->ob_flags |= DEE_SCOPE_FLAG_FOUND_RETURN;
parse_return_expr:
     if ((result->ob_return.ob_ret_expr = (
      DeeExprAstObject *)DeeLexer_ParseExpressionLv2(
      self,scope,flags)) == NULL) goto err;
    }
    if (token.tk_id != ';') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_RETURN,self,token_ob,
      "expected ';' after 'return' <expr>") == -1) goto err_rval;
    } else if (!yield()) { err_rval: Dee_XDECREF(result->ob_return.ob_ret_expr); goto err; }
   } break;

   // 'yield' (',' ~~ expr...) RELAXED_COMMA ';'
   case KWD_yield: {
    DeeScopeObject *weak_root = (DeeScopeObject *)DeeScope_WeakRoot(scope);
    DeeExprAstObject *yield_expr;
    DeeObject *yield_exprs;
    if (!yield()) goto err;
    result->ob_kind = DeeStmtAstKind_YIELD;
    if ((weak_root->ob_flags&DEE_SCOPE_FLAG_FOUND_RETURN)!=0) {
     if (DeeError_CompilerError(DEE_WARNING_YIELD_AFTER_RETURN,self,token_ob,
      "'yield' statement found after a 'return' statement was already encountered"
      ) == -1) goto err_rval;
     result->ob_kind = DeeStmtAstKind_RETURN; // Act like it's a return
     goto parse_return_expr;
    } else weak_root->ob_flags |= DEE_SCOPE_FLAG_FOUND_YIELD;
parse_yield_expr:
    yield_expr = (DeeExprAstObject *)DeeLexer_ParseExpression(self,scope,flags);
    if (yield_expr == NULL) goto err;
    yield_exprs = DeeTuple_Pack(1,yield_expr);
    Dee_DECREF(yield_expr);
    if (!yield_exprs) goto err;
    while (token.tk_id == ',') {
     int temp;
     if (!yield()) {err_yexpr: Dee_DECREF(yield_exprs); goto err; }
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
     if (token.tk_id == ';') break; // Stop parsing yield expressiosn
#endif
     yield_expr = (DeeExprAstObject *)DeeLexer_ParseExpression(self,scope,flags);
     if (yield_expr == NULL) goto err_yexpr;
     temp = DeeTuple_Append(&yield_exprs,(DeeObject *)yield_expr);
     Dee_DECREF(yield_expr);
     if (temp == -1) goto err_yexpr;
    }
    Dee_INHERIT_REF(result->ob_yield.ob_yld_exprs,*(DeeTupleObject **)&yield_exprs);
    if (token.tk_id != ';') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_YIELD,self,token_ob,
      "expected ';' after 'yield' <expr>") == -1) goto err_yval;
    } else if (!yield()) { err_yval: Dee_DECREF(result->ob_yield.ob_yld_exprs); goto err; }
   } break;

   // 'del' (',' ~~ expr...) RELAXED_COMMA ';'
   case KWD_del: {
    if (!yield()) goto err;
    if (token.tk_id == ';') {
     result->ob_kind = DeeStmtAstKind_EMPTY;
    } else {
     DeeExprAstObject *del_expr;
     DeeObject *del_exprs = NULL;
     int temp;
     while (1) {
      if ((del_expr = (DeeExprAstObject *)DeeLexer_ParseExpression(self,scope,flags)) == NULL) goto err;
      switch (del_expr->ob_kind) {
       case DeeExprAstKind_DECLVARIABLE:
       case DeeExprAstKind_VARIABLE: {
        DeeLocalVarObject *loc = del_expr->ob_var;
        if (!DeeScope_SameWeakScope((DeeObject *)loc->ob_scope,scope)) {
         Dee_DECREF(del_expr);
         if (DeeError_CompilerError(DEE_WARNING_CANT_DEL_VARIABLE_FROM_DIFFERENT_STRONG_SCOPE,self,
          (DeeObject *)result->ob_token,"Variable in 'del' is located in a different scope") == -1) goto err;
         goto next_expr;
        } else if ((loc->ob_flags&(DEE_LOCAL_VAR_FLAG_PARAMENTER|DEE_LOCAL_VAR_FLAG_RETVAL|DEE_LOCAL_VAR_FLAG_STATIC)) != 0) {
         if ((loc->ob_flags&DEE_LOCAL_VAR_FLAG_RETVAL)!=0){
          temp = DeeError_CompilerErrorf(DEE_WARNING_CANT_DEL_ARGUMENT_VARIABLE,self,(DeeObject *)result->ob_token,
           "Can't delete argument variable %q",TPPLexer_TokenIDStr(DeeLexer_LEXER(self),loc->ob_name));
         } else if ((loc->ob_flags&DEE_LOCAL_VAR_FLAG_STATIC)!=0){
          temp = DeeError_CompilerErrorf(DEE_WARNING_CANT_DEL_STATIC_VARIABLE,self,(DeeObject *)result->ob_token,
           "Can't delete static variable %q",TPPLexer_TokenIDStr(DeeLexer_LEXER(self),loc->ob_name));
         } else {
          temp = DeeError_CompilerErrorf(DEE_WARNING_CANT_DEL_RESULT_VARIABLE,self,(DeeObject *)result->ob_token,
           "Can't delete result variable %q",TPPLexer_TokenIDStr(DeeLexer_LEXER(self),loc->ob_name));
         }
         if (temp == -1) { Dee_DECREF(del_expr); goto err; }
         Dee_DECREF(del_expr);
         goto next_expr;
        } else {
         DEE_LVERBOSE2("Deleting variable: %q\n",TPPLexer_TokenIDStr(DeeLexer_LEXER(self),loc->ob_name));
         _DeeScope_DelName(loc->ob_scope,loc->ob_name);
        }
       } break;
       case DeeExprAstKind_SEQ_GET:
       case DeeExprAstKind_SEQ_RANGE_GET:
       case DeeExprAstKind_ATTR_GET:
        break;
       default:
        Dee_DECREF(del_expr);
        if (DeeError_CompilerError(DEE_WARNING_INVALID_EXPRSSION_FOR_DEL,self,(DeeObject *)result->ob_token,
         "'del' can't be used with this kind of expression") == -1) goto err;
        goto next_expr;
      }
      temp = DeeTuple_Append(&del_exprs,(DeeObject *)del_expr);
      Dee_DECREF(del_expr);
      if (temp == -1) {err_del_exprs: Dee_XDECREF(del_exprs); goto err; }
next_expr:
      if (token.tk_id != ',') break;
      if (!yield()) goto err_del_exprs;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
      if (token.tk_id == ';') break; // Stop parsing del expressions
#endif
     }
     if (del_exprs) {
      result->ob_kind = DeeStmtAstKind_DEL;
      Dee_INHERIT_REF(result->ob_del.ob_del_exprs,*(DeeTupleObject **)&del_exprs);
     } else {
      result->ob_kind = DeeStmtAstKind_EMPTY;
     }
    }
    if (token.tk_id != ';') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_DEL,self,token_ob,
      "expected ';' after 'del' <expr>") == -1) goto err_dval;
    } else if (!yield()) {
err_dval:
     if (result->ob_kind == DeeStmtAstKind_DEL)
      Dee_DECREF(result->ob_del.ob_del_exprs);
     goto err;
    }
   } break;

   // 'break' ';'
   case KWD_break:
    if ((flags&DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)!=0) {
     result->ob_kind = DeeStmtAstKind_BREAK;
    } else {
     if (DeeError_CompilerError(DEE_WARNING_ILLEGAL_STATEMENT_BREAK,self,token_ob,
      "'break' can only be used inside of a loop") == -1) goto err;
     result->ob_kind = DeeStmtAstKind_EMPTY;
    }
    if (!yield()) goto err;
    if (token.tk_id != ';') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_BREAK,
      self,token_ob,expected_semicolon_stmt_end) == -1) goto err;
    } else if (!yield()) goto err;
    break;

   // 'continue' ';'
   case KWD_continue:
    if ((flags&DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)!=0) {
     result->ob_kind = DeeStmtAstKind_CONTINUE;
    } else {
     if (DeeError_CompilerError(DEE_WARNING_ILLEGAL_STATEMENT_CONTINUE,self,token_ob,
      "'continue' can only be used inside of a loop") == -1) goto err;
     result->ob_kind = DeeStmtAstKind_EMPTY;
    }
    if (!yield()) goto err;
    if (token.tk_id != ';') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_CONTINUE,
      self,token_ob,"expected ';' after 'continue'") == -1) goto err;
    } else if (!yield()) goto err;
    break;

   // 'assert' exprLv2 ["," expr] ';'
   case KWD_assert:
   case KWD___assert:
    if (!yield()) goto err;
    result->ob_kind = DeeStmtAstKind_ASSERT;
    if ((result->ob_assert.ob_asrt_expr = _DeeLexer_ParseConditionalExpressionLv2(
     (DeeLexerObject *)self,(DeeScopeObject *)scope,flags)) == NULL) goto err;
    if (token.tk_id == ':') {
     if (!yield() || (result->ob_assert.ob_asrt_msg = (
      DeeExprAstObject *)DeeLexer_ParseExpression(
      self,scope,flags)) == NULL) {
err_ass_0:
      Dee_DECREF(result->ob_assert.ob_asrt_expr);
      goto err;
     }
    } else {
     result->ob_assert.ob_asrt_msg = NULL;
    }
    if (token.tk_id != ';') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_ASSERT,
      self,token_ob,expected_semicolon_stmt_end) == -1) goto err_ass_1;
    } else if (!yield()) { err_ass_1: Dee_XDECREF(result->ob_assert.ob_asrt_msg); goto err_ass_0; }
    break;

   // 'if' '(' exprLv2 ')' stmt ['else' stmt]
   case KWD_if:
    if (!yield() || (new_scope = DeeScope_New(scope,1)) == NULL) goto err;
    result->ob_kind = DeeStmtAstKind_IF;
    if (token.tk_id != '(') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_IF,self,token_ob,"Expected '(' after 'if'") == -1) goto errs;
    } else if (!yield()) goto errs;
    if ((result->ob_if.ob_if_expr = _DeeLexer_ParseConditionalExpressionLv2(
     (DeeLexerObject *)self,(DeeScopeObject *)new_scope,flags)) == NULL) goto errs;
    if (token.tk_id != ')') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_IF,self,token_ob,"Expected ')' after 'if' '(' <expr>") == -1) goto err_ies;
    } else if (!yield()) goto err_ies;
    if (result->ob_if.ob_if_expr->ob_kind == DeeExprAstKind_CONSTANT &&
       (flags & DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH) != 0 &&
        DEE_HAVE_CONSTOP_OPTIMIZATION(result->ob_if.ob_if_expr->ob_const)) {
     int expr_is_true;
     if ((expr_is_true = DeeObject_Bool(result->ob_if.ob_if_expr->ob_const)) == -1) {
      if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_COND,(DeeObject *)self,(DeeObject *)result->ob_if.ob_if_expr->ob_token,
       "Invalid constant operator: Using '%#q' as condition in if-statement will throw an '%K' at runtime",
       DeeType_NAME(Dee_TYPE(result->ob_if.ob_if_expr->ob_const)),_DeeError_HandleReturnType()) == -1) {
err_ies: Dee_DECREF(result->ob_if.ob_if_expr); goto errs;
      }
     } else {
      DeeStmtAstObject *false_branch;
      Dee_DECREF(result->ob_if.ob_if_expr);
      result->ob_kind = DeeStmtAstKind_SSTMT;
      if ((result->ob_sstmt.ob_sstmt_ast = (DeeStmtAstObject *)DeeLexer_ParseStatement(
       self,new_scope,expr_is_true ? flags : flags|DEE_PARSER_FLAG_UNREACHABLE)
       ) == NULL) goto err_ies;
      result->ob_sstmt.ob_sstmt_scope = (DeeScopeObject *)new_scope;
#if DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS
      if (token.tk_id == KWD_elif) {
       token.tk_id = KWD_if;
       goto parse_opelse;
      } else
#endif
      if (token.tk_id == KWD_else) {
       if (!yield()) goto err;
#if DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS
parse_opelse:
#endif
       if ((new_scope = DeeScope_New(scope,1)) == NULL) goto err;
       if ((false_branch = (DeeStmtAstObject *)DeeLexer_ParseStatement(
        self,new_scope,expr_is_true ? flags|DEE_PARSER_FLAG_UNREACHABLE : flags)
        ) == NULL) {
        Dee_DECREF(result->ob_sstmt.ob_sstmt_ast);
        Dee_DECREF(result->ob_sstmt.ob_sstmt_scope);
        goto errs;
       }
      } else false_branch = NULL;
      if (!expr_is_true) {
       Dee_DECREF(result->ob_sstmt.ob_sstmt_ast);
       Dee_DECREF(result->ob_sstmt.ob_sstmt_scope);
       if (false_branch) {
        Dee_INHERIT_REF(result->ob_sstmt.ob_sstmt_ast,false_branch);
        Dee_INHERIT_REF(result->ob_sstmt.ob_sstmt_scope,*(DeeScopeObject **)&new_scope);
       } else {
        result->ob_kind = DeeStmtAstKind_EMPTY;
       }
      } else if (false_branch) {
       Dee_DECREF(false_branch);
       Dee_DECREF(new_scope);
      }
      break;
     }
    }
    if ((result->ob_if.ob_if_thenstmt = (
     DeeStmtAstObject *)DeeLexer_ParseStatement(self,new_scope,flags)
     ) == NULL) goto err_ies;
    Dee_INCREF(result->ob_if.ob_if_thenscope = (DeeScopeObject *)new_scope);
#if DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS
    if (token.tk_id == KWD_elif) {
     token.tk_id = KWD_if;
     goto parse_else;
    } else
#endif
    if (token.tk_id == KWD_else) {
     if (!yield()) goto errs;
#if DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS
parse_else:
#endif
     // The false block has a different scope than the cond and true block
     Dee_DECREF(new_scope);
     if ((new_scope = DeeScope_New(scope,1)) == NULL) goto err;
     if ((result->ob_if.ob_if_elsestmt = (
      DeeStmtAstObject *)DeeLexer_ParseStatement(self,new_scope,flags)) == NULL) {
      Dee_DECREF(result->ob_if.ob_if_expr);
      Dee_DECREF(result->ob_if.ob_if_thenstmt);
      goto errs;
     }
     Dee_INCREF(result->ob_if.ob_if_elsescope = (DeeScopeObject *)new_scope);
    } else {
     result->ob_if.ob_if_elsestmt = NULL;
     result->ob_if.ob_if_elsescope = NULL;
    }
    Dee_DECREF(new_scope);
    break;

   // foreach-loop (without __iterself__())
   // '__foreach' '(' <keyword> ':' <expr> ')' <stmt>
   case KWD___foreach:
    if (!yield() || (new_scope = DeeScope_New(scope,1)) == NULL) goto err;
    if (token.tk_id != '(') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_FOREACH,self,token_ob,
      "Expected '(' after '__foreach'") == -1) goto errs;
    } else if (!yield()) goto errs;
    if (token.tk_id == KWD_local && !yield()) goto errs;
    if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_FOREACH,self,token_ob,
      "Expected <keyword> after '__foreach' '('") == -1) goto errs;
     result->ob_for_in.ob_fi_itername = (DeeLocalVarObject *)
      DeeScope_Lookup(new_scope,KWD___anonymous__,DEE_SCOPE_LOOKUP_MODE_LOCAL,NULL);
     if (!result->ob_for_in.ob_fi_itername) goto errs;
    } else {
     result->ob_for_in.ob_fi_itername = (DeeLocalVarObject *)
      DeeScope_Lookup(new_scope,token.tk_id,
      DEE_SCOPE_LOOKUP_MODE_LOCAL,NULL); // foreach iterators are always local variables
     if (!result->ob_for_in.ob_fi_itername) goto errs;
     if (!yield()) goto errs;
    }
    if (token.tk_id != ':' && token.tk_id != KWD_in) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_COLLON_AFTER_FOREACH,self,token_ob,
      "Expected ':' after '__foreach' '(' <keyword>") == -1) goto errs;
    } else if (!yield()) goto errs;
    DEE_ASSERT(DeeLocalVar_Check(result->ob_for_in.ob_fi_itername));
    DEE_ASSERT(((DeeLocalVarObject *)result->ob_for_in.ob_fi_itername)
               ->ob_scope == (DeeScopeObject *)new_scope);
    // v 'DeeScope_Lookup' doesn't return a reference
    Dee_INCREF(result->ob_for_in.ob_fi_itername);
    result->ob_kind = DeeStmtAstKind_FOREACH;
    if ((result->ob_for_in.ob_fi_iterlist = (DeeExprAstObject *)
     DeeLexer_ParseExpression(self,new_scope,flags)) == NULL) goto fe_err_1;
    if (token.tk_id != ')') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_FOREACH,self,token_ob,
      "Expected ')' after '__foreach' '(' <keyword> ':' <expr>") == -1) goto fe_err_2;
    } else if (!yield()) goto fe_err_2;
    if ((result->ob_for_in.ob_fi_block = (DeeStmtAstObject *)DeeLexer_ParseStatementNewScope(
     self,new_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto fe_err_2;
    Dee_INHERIT_REF(result->ob_for_in.ob_fi_scope,*(DeeScopeObject **)&new_scope);
    if ((flags&DEE_PARSER_FLAG_OPTIMIZE_LOOPS)!=0 &&
        _DeeStmtAst_OptimizeForIn(result,flags) == -1) goto err_r;
    break;

   // 'for' '(' <keyword> ':' <expr> ')' <stmt>
   // 'for' '(' [<expr>] ';' [<expr>] ';' [<expr>] ')' <stmt>
   case KWD_for:
    if (!yield() || (new_scope = DeeScope_New(scope,1)) == NULL) goto err;
    if (token.tk_id != '(') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_FOR,self,token_ob,
      "Expected '(' after 'for'") == -1) goto errs;
    } else if (!yield()) goto errs;
    if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
     DeeTokenObject *tok2;
     Dee_INCREF(tok = (DeeTokenObject *)token_ob);
     if (token.tk_id == KWD_local) {
      if (!yield()) goto terr2;
      Dee_INCREF(tok2 = (DeeTokenObject *)token_ob);
     } else tok2 = NULL;
     if (!yield()) {
terr: Dee_XDECREF(tok2);
terr2: Dee_DECREF(tok); goto errs;
     }
     if (token.tk_id == ':' || token.tk_id == KWD_in) {
      // foreach-loop
      result->ob_for_in.ob_fi_itername = (DeeLocalVarObject *)
       DeeScope_Lookup(new_scope,tok2 ? tok2->ob_token.tk_id : tok->ob_token.tk_id,
       DEE_SCOPE_LOOKUP_MODE_LOCAL,NULL); // foreach iterators are always local variables
      Dee_DECREF(tok);
      Dee_XDECREF(tok2);
      if (!result->ob_for_in.ob_fi_itername) goto errs;
      DEE_ASSERT(DeeLocalVar_Check(result->ob_for_in.ob_fi_itername));
      DEE_ASSERT(((DeeLocalVarObject *)result->ob_for_in.ob_fi_itername)
                 ->ob_scope == (DeeScopeObject *)new_scope);
      if (!yield()) goto errs;
      // v 'DeeScope_Lookup' doesn't return a reference
      Dee_INCREF(result->ob_for_in.ob_fi_itername);
      result->ob_kind = DeeStmtAstKind_FOR_IN;
      if ((result->ob_for_in.ob_fi_iterlist = (DeeExprAstObject *)
       DeeLexer_ParseExpression(self,new_scope,flags)) == NULL) {
fe_err_1: Dee_DECREF(result->ob_for_in.ob_fi_itername); goto errs;
      }
      if (token.tk_id != ')') {
       if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_FOR,self,token_ob,"Expected ')' after 'for' '(' <keyword> ':' <expr> ") == -1) goto fe_err_2;
      } else if (!yield()) {fe_err_2: Dee_DECREF(result->ob_for_in.ob_fi_iterlist); goto fe_err_1; }
      if ((result->ob_for_in.ob_fi_block = (DeeStmtAstObject *)
       DeeLexer_ParseStatementNewScope(self,new_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto fe_err_2;
      Dee_INHERIT_REF(result->ob_for_in.ob_fi_scope,*(DeeScopeObject **)&new_scope);
      if ((flags&DEE_PARSER_FLAG_OPTIMIZE_LOOPS)!=0 &&
          _DeeStmtAst_OptimizeForIn(result,flags) == -1) goto err_r;
      goto end;
     } else {
      // Return the tokens we consumed
      if (tok2 && DeeLexer_Return(self,(DeeObject *)tok2) == -1) goto terr;
      if (DeeLexer_Return(self,(DeeObject *)tok) == -1) goto terr;
      Dee_XDECREF(tok2);
      Dee_DECREF(tok);
     }
    }
    // regular for-loop
    result->ob_kind = DeeStmtAstKind_FOR;
    if (token.tk_id == ';') {
     result->ob_for.ob_f_forinit = NULL;
     goto f_1_semi;
    } else {
     if ((result->ob_for.ob_f_forinit = (DeeExprAstObject *)
      DeeLexer_ParseExpressionDecl(self,new_scope,flags)) == NULL) goto errs;
     if (token.tk_id != ';') {
      if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_OR_COLLON_AFTER_FOR,
       self,token_ob,"Expected ';' after 'for' '(' [<expr>]") == -1) goto f_err_1;
     } else {
f_1_semi:
      if (!yield()) {f_err_1: Dee_XDECREF(result->ob_for.ob_f_forinit); goto errs; }
     }
    }
    if (token.tk_id == ';') {
     result->ob_for.ob_f_forcont = NULL;
     goto f_2_semi;
    } else {
     if ((result->ob_for.ob_f_forcont = _DeeLexer_ParseConditionalExpressionLv2(
      (DeeLexerObject *)self,(DeeScopeObject *)new_scope,flags|
      DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto f_err_1;
     // TODO: optimization:
     //  - if ob_forcont evaluates to constant true, we can delete the branch
     //  - if ob_forcont evaluates to constant false, we can delete the for loop
     if (token.tk_id != ';') {
      if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_FOR,self,token_ob,
       "Expected ';' after 'for' '(' [<expr>] ';' [<expr>]") == -1) goto f_err_2;
     } else {
f_2_semi:
      if (!yield()) {f_err_2: Dee_XDECREF(result->ob_for.ob_f_forcont); goto f_err_1; }
     }
    }
    if (token.tk_id == ')') {
     result->ob_for.ob_f_fornext = NULL;
     goto f_3_paren;
    } else {
     if ((result->ob_for.ob_f_fornext = (DeeExprAstObject *)
      DeeLexer_ParseExpressionLv2(self,new_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto f_err_2;
     if (token.tk_id != ')') {
      if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_FOR,self,token_ob,
       "Expected ')' after 'for' '(' [<expr>] ';' [<expr>] ';' [<expr>]") == -1) goto f_err_3;
     } else {
f_3_paren:
      if (!yield()) {f_err_3: Dee_XDECREF(result->ob_for.ob_f_fornext); goto f_err_2; }
     }
    }
    if ((result->ob_for.ob_f_block = (DeeStmtAstObject *)
     DeeLexer_ParseStatementNewScope(self,new_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto f_err_3;
    Dee_INHERIT_REF(result->ob_for.ob_f_scope,*(DeeScopeObject **)&new_scope);
    if ((flags&DEE_PARSER_FLAG_OPTIMIZE_LOOPS) != 0 &&
        _DeeStmtAst_OptimizeFor(result) == -1) {
err_r:
     Dee_DECREF(result);
     goto err_fast;
    }
    break;

   // 'while' '(' cond_exprLv2 ')' stmt
   case KWD_while:
    if (!yield() || (new_scope = DeeScope_New(scope,1)) == NULL) goto err;
    if (token.tk_id != '(') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_WHILE,self,token_ob,"Expected '(' after 'while'") == -1) goto errs;
    } else if (!yield()) goto errs;
    if ((result->ob_while.ob_w_loopcond = _DeeLexer_ParseConditionalExpressionLv2(
     (DeeLexerObject *)self,(DeeScopeObject *)new_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto errs;
    if (token.tk_id != ')') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_WHILE,self,token_ob,"Expected ')' after 'while' '(' <expr>") == -1) goto err_wcs;
    } else if (!yield()) { err_wcs: Dee_DECREF(result->ob_while.ob_w_loopcond); goto errs; }
    if (result->ob_while.ob_w_loopcond->ob_kind == DeeExprAstKind_CONSTANT &&
       (flags&DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH) != 0 &&
       DEE_HAVE_CONSTOP_OPTIMIZATION_BOOL(result->ob_while.ob_w_loopcond->ob_const)) {
     int cond_is_true;
     if ((cond_is_true = DeeObject_Bool(result->ob_while.ob_w_loopcond->ob_const)) == -1) {
      if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_COND,(DeeObject *)self,(DeeObject *)result->ob_while.ob_w_loopcond->ob_token,
       "Invalid constant operator: Using '%#q' as condition in a while loop will throw an '%K' at runtime",
       DeeType_NAME(Dee_TYPE(result->ob_while.ob_w_loopcond->ob_const)),_DeeError_HandleReturnType()) == -1) goto err_wcs;
     } else {
      DeeStmtAstObject *while_block;
      Dee_DECREF(result->ob_while.ob_w_loopcond);
      if ((while_block = (DeeStmtAstObject *)
       DeeLexer_ParseStatementNewScope(self,new_scope,(Dee_uint32_t)(cond_is_true
       ? flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE
       : flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE|DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH))
       ) == NULL) goto errs;
      if (cond_is_true) {
       result->ob_kind = DeeStmtAstKind_FOR;
       result->ob_for.ob_f_scope = (DeeScopeObject *)new_scope;
       result->ob_for.ob_f_block = while_block;
       result->ob_for.ob_f_forinit = NULL;
       result->ob_for.ob_f_forcont = NULL;
       result->ob_for.ob_f_fornext = NULL;
      } else {
       result->ob_kind = DeeStmtAstKind_EMPTY;
       Dee_DECREF(new_scope);
       Dee_DECREF(while_block);
      }
     }
     break;
    }
    if ((result->ob_while.ob_w_block = (DeeStmtAstObject *)
     DeeLexer_ParseStatementNewScope(self,new_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto err_wcs;
    result->ob_kind = DeeStmtAstKind_WHILE;
    Dee_INHERIT_REF(result->ob_while.ob_w_scope,*(DeeScopeObject **)&new_scope);
    break;

   case KWD___looponce:
    if (!yield() || (new_scope = DeeScope_New(scope,1)) == NULL) goto err;
    result->ob_kind = DeeStmtAstKind_LOOP_ONCE;
    if ((result->ob_loop_once.ob_lonce_block = (DeeStmtAstObject *)
     DeeLexer_ParseStatementOldScope(self,new_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto errs;
    Dee_INHERIT_REF(result->ob_loop_once.ob_lonce_scope,*(DeeScopeObject **)&new_scope);
    break;

   // 'do' stmt 'while' '(' cond_exprLv2 ')' ';'
   case KWD_do:
    if (!yield() || (new_scope = DeeScope_New(scope,1)) == NULL) goto err;
    if ((result->ob_do_while.ob_dw_block = (DeeStmtAstObject *)
     DeeLexer_ParseStatementNewScope(self,new_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto err_wcs;
    if (token.tk_id != KWD_while) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_WHILE_AFTER_DO,self,token_ob,"Expected 'while' after 'do' <stmt>") == -1) goto err_wbs;
    } else if (!yield()) {err_wbs: Dee_DECREF(result->ob_do_while.ob_dw_block); goto errs; }
    if (token.tk_id != '(') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_DO_WHILE,self,token_ob,"Expected '(' after 'do' <stmt> 'while'") == -1) goto err_wbs;
    } else if (!yield()) goto err_wbs;
    if ((result->ob_do_while.ob_dw_loopcond = _DeeLexer_ParseConditionalExpressionLv2(
     (DeeLexerObject *)self,(DeeScopeObject *)new_scope,flags|DEE_PARSER_FLAG_ALLOW_BREAK_CONTINUE)) == NULL) goto err_wbs;
    if (token.tk_id != ')') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_DO_WHILE,self,token_ob,"Expected ')' after 'do' <stmt> 'while' '(' <expr>") == -1) goto err_wbcs;
    } else if (!yield()) { err_wbcs: Dee_DECREF(result->ob_do_while.ob_dw_loopcond); goto err_wbs; }
    if (token.tk_id != ';') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_DO_WHILE,self,token_ob,
      "expected ';' after 'do' <stmt> 'while' '(' <expr> ')'") == -1) goto err_wbcs;
    } else if (!yield()) goto err_wbcs;
    result->ob_kind = DeeStmtAstKind_DO_WHILE;
    result->ob_do_while.ob_dw_scope = (DeeScopeObject *)new_scope; // Inherit reference
    if (result->ob_do_while.ob_dw_loopcond->ob_kind == DeeExprAstKind_CONSTANT &&
       (flags&DEE_PARSER_FLAG_OPTIMIZE_LOOPS)!=0 &&
       DEE_HAVE_CONSTOP_OPTIMIZATION_BOOL(result->ob_do_while.ob_dw_loopcond->ob_const)) {
     int expr_is_true;
     if ((expr_is_true = DeeObject_Bool(result->ob_do_while.ob_dw_loopcond->ob_const)) == -1) {
      if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP_COND,(DeeObject *)self,(DeeObject *)result->ob_do_while.ob_dw_loopcond->ob_token,
       "Invalid constant operator: Using '%#q' as condition in a do..while loop will throw an '%K' at runtime",
       DeeType_NAME(Dee_TYPE(result->ob_do_while.ob_dw_loopcond->ob_const)),_DeeError_HandleReturnType()) == -1) goto err_r;
     } else {
      DeeStmtAstObject *loop_block;
      loop_block = result->ob_do_while.ob_dw_block;
      Dee_DECREF(result->ob_do_while.ob_dw_loopcond);
      if (expr_is_true) {
       result->ob_kind = DeeStmtAstKind_FOR;
       result->ob_for.ob_f_block = loop_block;
       result->ob_for.ob_f_forcont = NULL;
       result->ob_for.ob_f_forinit = NULL;
       result->ob_for.ob_f_fornext = NULL;
       result->ob_for.ob_f_scope = (DeeScopeObject *)new_scope;
      } else {
       result->ob_kind = DeeStmtAstKind_LOOP_ONCE;
       result->ob_loop_once.ob_lonce_block = loop_block;
       result->ob_loop_once.ob_lonce_scope = (DeeScopeObject *)new_scope;
      }
     }
    }
    break;

   // 'goto' keyword ';'
   case KWD_goto:
    if (!yield()) goto err;
    if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_GOTO,self,token_ob,
      "Expected <keyword> after 'goto'") == -1) goto err;
     result->ob_kind = DeeStmtAstKind_EMPTY;
    } else {
     DeeScopeObject *weak_root = (DeeScopeObject *)DeeScope_WeakRoot(scope);
     result->ob_kind = DeeStmtAstKind_GOTO;
     if ((result->ob_goto.ob_gt_label_id = DeeNamedLabelDeclList_GetOrAdd(
      &weak_root->ob_labels,token.tk_id)) == (Dee_size_t)-1) goto err;
     if (!yield()) goto err;
    }
    if (token.tk_id != ';') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_GOTO,self,token_ob,
      "expected ';' after 'goto' <keyword>") == -1) goto err;
    } else if (!yield()) goto err;
    break;

#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS

   // 'throw' expr ';'
   case KWD_throw:
    if (!yield()) goto err;
    if (token.tk_id == ';') {
     result->ob_kind = DeeStmtAstKind_RETHROW;
    } else {
     if ((result->ob_throw.ob_trw_expr = (DeeExprAstObject *)
      DeeLexer_ParseExpression(self,scope,flags)) == NULL) goto err;
     result->ob_kind = DeeStmtAstKind_THROW;
     if (token.tk_id != ';') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_THROW,
      self,token_ob,"expected ';' after 'throw' <expr>") == -1) goto err_throw_0;
     } else if (!yield()) { err_throw_0: Dee_DECREF(result->ob_throw.ob_trw_expr); goto err; }
    }
    break;

   // 'try' stmt (('catch' '(' (
   //  ((keyword '...')|('...' [keyword]))| // Catch all
   //  (const_expr [['->'] keyword])        // Typed catch
   // ) ')' stmt)... | ('finally' stmt))
   case KWD_try: {
    DeeStmtAstObject *protected_code;
    struct DeeStmtAstTryHandler *handler,*handler_v,*new_handler_v;
    Dee_size_t handler_c;
    if (!yield()) goto err;
    if ((new_scope = DeeScope_New(scope,1)) == NULL) goto err;
    if ((protected_code = (DeeStmtAstObject *)DeeLexer_ParseStatement(self,new_scope,flags)) == NULL) goto errs;
    handler_v = NULL;
    handler_c = 0;
    if (token.tk_id != KWD_finally && token.tk_id != KWD_catch) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CATCH_OR_FINALLY_AFTER_TRY,self,token_ob,
      "Expected 'catch' or 'finally' after 'try' <stmt>") == -1) goto errs_try_prot;
    } else {
     DeeScopeObject *handler_scope;
     do {
      new_handler_v = (struct DeeStmtAstTryHandler *)reallocf_nz(
       handler_v,(handler_c+1)*sizeof(struct DeeStmtAstTryHandler),
       "list of exception handler for try block (len = %u)",(unsigned)handler_c);
      if (!new_handler_v) { DeeError_NoMemory(); goto errs_try_prot; }
      if ((handler_scope = (DeeScopeObject *)DeeScope_New(scope,1)) == NULL) goto errs_try_prot;
      if (_DeeLexer_ParseCatchOrFinally(handler = ((handler_v = new_handler_v)+handler_c),
       (DeeLexerObject *)self,handler_scope,flags) == -1) {
       Dee_DECREF(handler_scope);
errs_try_prot:
       // Error occurred
       while (handler_c--) {
        Dee_XDECREF(handler_v[handler_c].ob_local);
        Dee_XDECREF(handler_v[handler_c].ob_type);
        Dee_DECREF(handler_v[handler_c].ob_scope);
        Dee_DECREF(handler_v[handler_c].ob_block);
       }
       free_nn(handler_v);
       Dee_DECREF(protected_code);
       goto errs;
      }
      Dee_DECREF(handler_scope);
      ++handler_c;
      // If an finally handler is encountered, no more 
      if (handler->ob_kind == DeeExceptionHandleKind_FINALLY) break;
     } while (token.tk_id == KWD_finally || token.tk_id == KWD_catch);
    }
    result->ob_kind = DeeStmtAstKind_TRY;
#ifndef __INTELLISENSE__
    // For some reason this line causes intellisense to freak out... huh...
    result->ob_try.ob_try_handlerc = handler_c;
#endif
    result->ob_try.ob_try_handlerv = handler_v;
    Dee_INHERIT_REF(result->ob_try.ob_try_block,protected_code);
    Dee_INHERIT_REF(result->ob_try.ob_try_scope,*(DeeScopeObject **)&new_scope);
    if ((flags&DEE_PARSER_FLAG_OPTIMIZE_EXCEPTIONS) != 0 &&
        _DeeStmtAst_OptimizeTry(result) == -1) goto err_r;
   } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */

   // 'extern'
   //   [function_attr_decl] const_expr
   //   [function_attr_decl] ':'
   case KWD_extern: {
    DeeExprAstObject *ast;
    struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
    result->ob_kind = DeeStmtAstKind_EXTERN;
    if (!yield()) {err_func_attr: DeeAttributeDecl_Quit(&attr); goto err; }
    if (_DeeLexer_ParseAttributeDecl(&attr,(DeeLexerObject *)self,(DeeScopeObject *)scope,flags) == -1) goto err_func_attr;
    if ((ast = (DeeExprAstObject *)DeeLexer_ParseExpression(
     self,scope,flags|DEE_PARSER_FLAG_GENERATE_CONSTANT)) == NULL) goto err_func_attr;
    if (ast->ob_kind == DeeExprAstKind_CONSTANT) {
     if (!DeeString_Check(ast->ob_const)) {
      if (DeeError_CompilerError(DEE_WARNING_EXPECTED_STRING_CONSTANT_EXPRESSION_DLL_AFTER_EXTERN,
       self,(DeeObject *)ast->ob_token,"Expected string after 'extern'") == -1) { err_extern_ast_1: Dee_DECREF(ast); goto err_func_attr; }
      result->ob_extern.ob_extern_decl.ob_libfile = (DeeStringObject *)DeeObject_Str(ast->ob_const);
      if (!result->ob_extern.ob_extern_decl.ob_libfile) goto err_extern_ast_1;
     } else {
      Dee_INCREF(result->ob_extern.ob_extern_decl.ob_libfile = (DeeStringObject *)ast->ob_const);
     }
    } else {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_DLL_AFTER_EXTERN,
      self,(DeeObject *)ast->ob_token,"Expected constant expression after 'extern'") == -1) goto err_extern_ast_1;
     Dee_INCREF(result->ob_extern.ob_extern_decl.ob_libfile = (DeeStringObject *)Dee_EmptyString);
    }
    Dee_DECREF(ast);
    if (_DeeLexer_ParseAttributeDecl(&attr,(DeeLexerObject *)self,(DeeScopeObject *)scope,flags) == -1) goto err_extern_1;
    if (token.tk_id != ':') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_COLLON_AFTER_EXTERN_DLL,self,token_ob,
      "Expected ':' after 'extern' <const_expr>") == -1) goto err_extern_1;
    } else {
     if (!yield()) { err_extern_1: Dee_DECREF(result->ob_extern.ob_extern_decl.ob_libfile); goto err_func_attr; }
     if (_DeeLexer_ParseAttributeDecl(&attr,(DeeLexerObject *)self,(DeeScopeObject *)scope,flags) == -1) goto err_extern_1;
    }
    // Dll name + ':' is now parsed (continue with return type)
    {
     DeeTokenObject *name_token;
     DeeTypeObject *extern_type;
     if ((extern_type = _DeeLexer_ParseNamedTypeAttr(
      (DeeLexerObject *)self,(DeeScopeObject *)scope,flags,NULL,&name_token,NULL,&attr)
      ) == NULL) goto err_extern_1;
     if (!DeeStructuredType_Check(extern_type)) {
      if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_STRUCTURED_TYPE_FOR_EXTERN_DECL,self,token_ob,
       "Expected structured type for extern decl (got: %s)",DeeType_NAME(extern_type)) == -1) {
err_extern_2:
       Dee_XDECREF(name_token);
       Dee_DECREF(extern_type);
       goto err_extern_1;
      }
      Dee_DECREF(extern_type);
      Dee_INCREF(extern_type = (DeeTypeObject *)&DeeNone_Type);
     } else if (!name_token) {
      if (DeeError_CompilerErrorf(DEE_WARNING_UNNAMED_EXTERN_VARIABLE,self,token_ob,
       "Unnamed extern variable of type %s",DeeType_NAME(extern_type)) == -1) goto err_extern_2;
      Dee_DECREF(extern_type);
extern_undo_0:
      Dee_DECREF(result->ob_extern.ob_extern_decl.ob_libfile);
      result->ob_kind = DeeStmtAstKind_EMPTY;
      goto extern_end;
     }
     Dee_INHERIT_REF(result->ob_extern.ob_extern_decl.ob_type,*(DeeStructuredTypeObject **)&extern_type);
     if ((result->ob_extern.ob_extern_decl.ob_name = (
      DeeStringObject *)DeeString_New(TPPLexer_TokenIDStr(
      DeeLexer_LEXER(self),name_token->ob_token.tk_id))) == NULL) {
err_extern_3:
      Dee_DECREF(name_token);
      Dee_DECREF(result->ob_extern.ob_extern_decl.ob_type);
      goto err_extern_1;
     }
     if ((result->ob_extern.ob_extern_store = (DeeLocalVarObject *)DeeScope_Lookup(
      scope,name_token->ob_token.tk_id,DEE_SCOPE_LOOKUP_MODE_LOCAL,NULL)
      ) == NULL) goto err_extern_3;
     if (!DeeLocalVar_Check(result->ob_extern.ob_extern_store)) {
      if (DeeError_CompilerErrorf(DEE_WARNING_EXTERN_VARIABLE_ALREADY_USED,self,token_ob,
       "Extern variable name %q is already used by a constant %s:%k",
       DeeString_STR(result->ob_extern.ob_extern_decl.ob_name),
       DeeType_NAME(Dee_TYPE(result->ob_extern.ob_extern_store)),
       result->ob_extern.ob_extern_store) == -1) goto err_extern_3;
extern_undo_1:
      Dee_DECREF(name_token);
      Dee_DECREF(result->ob_extern.ob_extern_decl.ob_name);
      Dee_DECREF(result->ob_extern.ob_extern_decl.ob_type);
      goto extern_undo_0;
     } else {
      DeeTypeObject *temp,*old_type;
      if ((temp = DeeType_LValue((DeeTypeObject *)result->ob_extern.ob_extern_decl.ob_type
       )) == NULL) goto extern_undo_1;
      if ((result->ob_extern.ob_extern_store->ob_flags&DEE_LOCALVAR_FLAG_TYPEBOUND)!=0) {
       old_type = result->ob_extern.ob_extern_store->ob_type;
       DEE_ASSERT(old_type);
       if (old_type != temp) {
        if (DeeError_CompilerErrorf(DEE_WARNING_VARIABLE_HAS_ALREADY_BEEN_BOUND_TO_DIFFERENT_TYPE,
         (DeeObject *)self,(DeeObject *)name_token,
         "Variable was already bound to a different type (old: %s; new: %s)",
         DeeType_NAME(old_type),DeeType_NAME(temp)) == -1
         ) goto extern_undo_1;
        Dee_INCREF(result->ob_extern.ob_extern_store->ob_type =
                   DeeLValueType_Check(old_type) ? DeeLValueType_BASE(old_type) : old_type);
       }
      } else {
       result->ob_extern.ob_extern_store->ob_flags |= DEE_LOCALVAR_FLAG_TYPEBOUND;
       Dee_INCREF(result->ob_extern.ob_extern_store->ob_type = temp);
      }
     }
     Dee_DECREF(name_token);
     Dee_INCREF(result->ob_extern.ob_extern_store);
    }
    if (attr.ob_name) {
     Dee_DECREF(result->ob_extern.ob_extern_decl.ob_name);
     Dee_INCREF(result->ob_extern.ob_extern_decl.ob_name = attr.ob_name);
    }
    DeeAttributeDecl_Quit(&attr);
    // Done!
extern_end:
    if (token.tk_id != ';') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_EXTERN,self,token_ob,
      "expected ';' after 'extern' <const_expr> ':' <keyword> '(' ... ')'") == -1) goto err_r;
    } else if (!yield()) goto err_r;
   } break;


   // 'import' keyword ';'
   // 'import' (('.'|'::') ~~ keyword...) [('.'|'::') '*'] ';'
   case KWD_import: {
    if (!yield()) goto err;
    while (1) {
     struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
     if (_DeeLexer_ParseAttributeDecl(&attr,(
      DeeLexerObject *)self,(DeeScopeObject *)scope,flags) == -1) {
err_imp_attr: DeeAttributeDecl_Quit(&attr); goto err;
     }
     if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
      if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_IMPORT,self,token_ob,
       "expected <keyword> after 'import'") == -1) goto err_imp_attr;
     } else {
      TPPTokenID import_name = 0; // If 0, import all
      DeeObject *import_object,*is_deprecated;
      int explicit_import_all = 0;
      if (token.tk_id == KWD_module) { if (!yield()) goto err_imp_attr; explicit_import_all = 1; }
      is_deprecated = NULL;
      import_object = DeeScope_Lookup(scope,token.tk_id,DEE_SCOPE_LOOKUP_MODE_CONST,&is_deprecated);
      if (is_deprecated) {
       if (DeeError_CompilerErrorf(DEE_WARNING_DEPRECATED_USAGE,self,token_ob,
        "Deprecated : %r",is_deprecated) == -1) goto err_imp_attr;
      }
      if (!import_object) {
       if (DeeError_CompilerError(DEE_WARNING_EXPECTED_MODULE_AFTER_IMPORT,self,token_ob,
        "expected module name after 'import'") == -1) goto err_imp_attr;
       if (!yield()) goto err_imp_attr;
      } else {
       if (!yield()) goto err_imp_attr;
       while (1) {
        if (token.tk_id != '.' && token.tk_id != TPP_TOK_COLLON_COLLON) break;
        if (!DeeModule_Check(import_object)) {
         if (DeeError_CompilerErrorf(DEE_WARNING_IMPORT_START_AFTER_IMPORT_MODULE,self,token_ob,
          "expected module name after 'import' (got: %k:%r)",Dee_TYPE(import_object),import_object) == -1) goto err_imp_attr;
         break;
        }
        if (!yield()) goto err_imp_attr;
        if (token.tk_id == '*') {
         import_name = 0;
         if (!yield()) goto err_imp_attr;
         break;
        } else if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
         if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_AFTER_IMPORT_SUBSCRIPT,self,token_ob,
          "expected <keyword> after 'import' [<keyword> '.']...") == -1) goto err_imp_attr;
         break;
        } else {
         DeeObject *next_import_object = _DeeScope_GetName(
          ((DeeModuleObject *)import_object)->ob_scope,token.tk_id);
         if (!next_import_object) {
          if (DeeError_CompilerError(DEE_WARNING_IMPORT_OBJECT_NOT_FOUND_IN_MODULE,self,token_ob,
           "expected object after 'import' subscript [<keyword> '.']...") == -1) goto err_imp_attr;
          break;
         }
         import_name = token.tk_id;
         if (!yield()) goto err_imp_attr;
         import_object = next_import_object;
        }
       }
       if (_DeeLexer_ParseAttributeDecl(&attr,(
        DeeLexerObject *)self,(DeeScopeObject *)scope,flags) == -1) goto err_imp_attr;
       if (explicit_import_all) {
        if (DeeModule_Check(import_object)) goto import_all;
        if (DeeError_CompilerError(DEE_WARNING_IMPORT_ALL_EXPECTED_MODULE,self,token_ob,
         "expected module after 'import' 'module' (<keyword> '.')...") == -1) goto err_imp_attr;
       } else if (DeeModule_Check(import_object) && !import_name) {
import_all:
        // Our equivalent of: from @import_object import *
        if (DeeScope_ImportScope(scope,(DeeObject *)(
         (DeeModuleObject *)import_object)->ob_scope,self,token_ob,(DeeObject *)attr.ob_depr) == -1) goto err_imp_attr;
       } else if (import_name) {
        // Our equivalent of: import @import_object as @import_name
        if (DeeScope_ImportSingle(scope,import_name,import_object,self,token_ob,(DeeObject *)attr.ob_depr) == -1) goto err_imp_attr;
       }
      }
     }
     if (token.tk_id != ',') {
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
import_break:
#endif
      DeeAttributeDecl_Quit(&attr);
      break;
     }
     if (!yield()) goto err_imp_attr;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
     if (token.tk_id == ';') goto import_break;
#endif
     DeeAttributeDecl_Quit(&attr);
    }
    if (token.tk_id != ';') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_IMPORT,self,token_ob,
      "expected ';' after 'import' [<keyword> '.']...") == -1) goto err_r;
    } else if (!yield()) goto err_r;
    result->ob_kind = DeeStmtAstKind_EMPTY;
   } break;

   case KWD_static:
   case KWD_local:
   case KWD_forward: {
    TPPTokenID inital_kwd;
default_decl:
    inital_kwd = token.tk_id;
    result->ob_kind = DeeStmtAstKind_EXPR;
    if ((result->ob_expr.ob_expr_ast = (DeeExprAstObject *)
     DeeLexer_ParseExpressionDecl(self,scope,flags)) == NULL) goto err;
    if (token.tk_id != ';') {
     //DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_LOCAL;
     //DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_FORWARD;
     if (DeeError_CompilerError(inital_kwd == KWD_local
      ? DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_LOCAL : inital_kwd == KWD_forward
      ? DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_FORWARD
      : DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_EXPRESSION,self,token_ob,
      "expected ';' after <expr>") == -1) goto err_r;
    } else if (!yield()) goto err_r;
   } break;

   case KWD_function: {
    // Special handling because 'function' doesn't always require a ';' to follow
    result->ob_kind = DeeStmtAstKind_EXPR;
    if ((result->ob_expr.ob_expr_ast = (DeeExprAstObject *)
     DeeLexer_ParseExpressionDecl(self,scope,flags)) == NULL) goto err;
    switch (result->ob_expr.ob_expr_ast->ob_kind) {
     case DeeExprAstKind_FUNCTION_EXPR:
     case DeeExprAstKind_FUNCTION_NONE:
     case DeeExprAstKind_VFUNCTION_EXPR:
     case DeeExprAstKind_VFUNCTION_NONE:
      if (token.tk_id != ';') {
       if (DeeError_CompilerError(
        DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_EXPRESSION,self,token_ob,
        "expected ';' after <expr>") == -1) goto err_r;
      } else if (!yield()) goto err_r;
      break;
     default: break;
    }
   } break;

   case KWD__Static_assert:
   case KWD_alias:
   case KWD_asm:
   case KWD_concept:
   case KWD_constexpr:
   case KWD_constructor:
   case KWD_destructor:
   case KWD_exec:
   case KWD_explicit:
   case KWD_export:
   case KWD_finalization:
   case KWD_friend:
   case KWD_from:
   case KWD_generic:
   case KWD_implementation:
   case KWD_initialization:
   case KWD_inline:
   case KWD_namespace:
   //case KWD_operator:
   //case KWD_private:
   //case KWD_protected:
   //case KWD_public:
   case KWD_raise:
   case KWD_repeat:
   case KWD_static_assert:
   case KWD_template:
   case KWD_unit:
   case KWD_until:
   case KWD_uses:
   case KWD_using:
   case KWD_var:
   case KWD_with:
    if (DeeError_CompilerError(DEE_WARNING_USING_RESERVED_KEYWORD,self,token_ob,
     "Reserved keyword used in a context that might change its meaning.") == -1) goto err_r;
    // fallthrough

   // exprLv2
   // const_expr (',' ~~ (keyword ['=' expr])...)
   default:
    // Check label
    if (TPPTokenID_IS_KEYWORD(token.tk_id)/* &&
        // v these are expression keywords, that allow
        //   a ':' to immediately follow the keyword,
        //   implementing a different meaning for it.
        token.tk_id != KWD_class && token.tk_id != KWD_struct &&
        token.tk_id != KWD_union && token.tk_id != KWD_enum*/) {
     DEE_ASSERT(result->ob_token == (DeeTokenObject *)token_ob);
     if (!yield()) goto err;
     if (token.tk_id == ':') {
      // Label declaration
      Dee_size_t label_decl;
      DeeScopeObject *weak_root;
      if (!yield()) goto err;
      weak_root = (DeeScopeObject *)DeeScope_WeakRoot(scope);
      if ((label_decl = DeeNamedLabelDeclList_GetOrAdd(
       &weak_root->ob_labels,result->ob_token->ob_token.tk_id)) == (Dee_size_t)-1) goto err;
      if (DeeLabelRefList_Add(&result->ob_label_endpoints,DEE_LABEL_REF_FLAGS_NONE,label_decl) == -1) goto err;
      // Continue parsing statements
      goto again;
     } else { // Return the original token
      if (DeeLexer_Return(self,(DeeObject *)result->ob_token) == -1) goto err;
     }
    }
    goto default_decl;
errs:     Dee_DECREF(new_scope);
err:      _DeeStmtAst_Free(result);
err_fast: result = NULL;
    break;
  }
 }
end:
 return (DeeObject *)result;
}
DEE_COMPILER_MSVC_WARNING_POP


#undef yield
#undef token


DEE_COMPILER_MSVC_WARNING_PUSH(4061)
static int _deestmtast_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeStmtAstObject *self, DeeStmtAstObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStmtAst_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeStmtAst_Check(right));
 switch (self->ob_kind = right->ob_kind) {
  case DeeStmtAstKind_INL_STMTS: {
   DeeStmtAstObject **dst,**begin,**end;
   if ((dst = (DeeStmtAstObject **)mallocf_nz(
    sizeof(DeeStmtAstObject *)*(self->ob_inl_stmts.ob_inl_stmts_c = right->ob_inl_stmts.ob_inl_stmts_c),
    "stmt_ast inl_statements of (n = %d; right = %p)",(int)right->ob_inl_stmts.ob_inl_stmts_c,right
    )) == NULL) return -1;
   self->ob_inl_stmts.ob_inl_stmts_v = dst;
   end = (begin = right->ob_inl_stmts.ob_inl_stmts_v)+right->ob_inl_stmts.ob_inl_stmts_c;
   while (begin != end) Dee_INCREF(*dst++ = *begin++);
  } break;
  case DeeStmtAstKind_STMTS: {
   DeeStmtAstObject **dst,**begin,**end;
   if ((dst = (DeeStmtAstObject **)mallocf_nz(
    sizeof(DeeStmtAstObject *)*(self->ob_stmts.ob_stmts_c = right->ob_stmts.ob_stmts_c),
    "stmt_ast statements of (n = %d; right = %p)",(int)right->ob_stmts.ob_stmts_c,right
    )) == NULL) return -1;
   self->ob_stmts.ob_stmts_v = dst;
   end = (begin = right->ob_stmts.ob_stmts_v)+right->ob_stmts.ob_stmts_c;
   while (begin != end) Dee_INCREF(*dst++ = *begin++);
   Dee_INCREF(self->ob_stmts.ob_stmts_scope = right->ob_stmts.ob_stmts_scope);
  } break;
  case DeeStmtAstKind_EXPR:
   Dee_INCREF(self->ob_expr.ob_expr_ast = right->ob_expr.ob_expr_ast);
   break;
  case DeeStmtAstKind_SSTMT:
   Dee_INCREF(self->ob_sstmt.ob_sstmt_scope = right->ob_sstmt.ob_sstmt_scope);
   Dee_INCREF(self->ob_sstmt.ob_sstmt_ast = right->ob_sstmt.ob_sstmt_ast);
   break;
  case DeeStmtAstKind_SEXPR:
   Dee_INCREF(self->ob_sexpr.ob_sexpr_scope = right->ob_sexpr.ob_sexpr_scope);
   Dee_INCREF(self->ob_sexpr.ob_sexpr_ast = right->ob_sexpr.ob_sexpr_ast);
   break;
  case DeeStmtAstKind_RETURN:
   Dee_XINCREF(self->ob_return.ob_ret_expr = right->ob_return.ob_ret_expr);
   break;
  case DeeStmtAstKind_YIELD:
   Dee_INCREF(self->ob_yield.ob_yld_exprs = right->ob_yield.ob_yld_exprs);
   break;
  case DeeStmtAstKind_DEL:
   Dee_INCREF(self->ob_del.ob_del_exprs = right->ob_del.ob_del_exprs);
   break;
  case DeeStmtAstKind_ASSERT:
   Dee_INCREF(self->ob_assert.ob_asrt_expr = right->ob_assert.ob_asrt_expr);
   Dee_XINCREF(self->ob_assert.ob_asrt_msg = right->ob_assert.ob_asrt_msg);
   break;
  case DeeStmtAstKind_PRINT:
  case DeeStmtAstKind_PRINT_NOLF:
   Dee_XINCREF(self->ob_print.ob_prt_file = right->ob_print.ob_prt_file);
   Dee_XINCREF(self->ob_print.ob_prt_tpl = right->ob_print.ob_prt_tpl);
   break;
  case DeeStmtAstKind_IF:
   Dee_INCREF(self->ob_if.ob_if_expr = right->ob_if.ob_if_expr);
   Dee_INCREF(self->ob_if.ob_if_thenstmt = right->ob_if.ob_if_thenstmt);
   Dee_INCREF(self->ob_if.ob_if_thenscope = right->ob_if.ob_if_thenscope);
   Dee_XINCREF(self->ob_if.ob_if_elsestmt = right->ob_if.ob_if_elsestmt);
   Dee_XINCREF(self->ob_if.ob_if_elsescope = right->ob_if.ob_if_elsescope);
   break;
  case DeeStmtAstKind_FOR:
   Dee_XINCREF(self->ob_for.ob_f_scope = right->ob_for.ob_f_scope);
   Dee_XINCREF(self->ob_for.ob_f_forinit = right->ob_for.ob_f_forinit);
   Dee_XINCREF(self->ob_for.ob_f_fornext = right->ob_for.ob_f_fornext);
   Dee_XINCREF(self->ob_for.ob_f_forcont = right->ob_for.ob_f_forcont);
   Dee_INCREF(self->ob_for.ob_f_block = right->ob_for.ob_f_block);
   break;
  case DeeStmtAstKind_FOR_IN:
  case DeeStmtAstKind_FOREACH:
   Dee_INCREF(self->ob_for_in.ob_fi_scope = right->ob_for_in.ob_fi_scope);
   Dee_INCREF(self->ob_for_in.ob_fi_itername = right->ob_for_in.ob_fi_itername);
   Dee_INCREF(self->ob_for_in.ob_fi_iterlist = right->ob_for_in.ob_fi_iterlist);
   Dee_INCREF(self->ob_for_in.ob_fi_block = right->ob_for_in.ob_fi_block);
   break;
  case DeeStmtAstKind_WHILE:
   Dee_INCREF(self->ob_while.ob_w_scope = right->ob_while.ob_w_scope);
   Dee_INCREF(self->ob_while.ob_w_loopcond = right->ob_while.ob_w_loopcond);
   Dee_INCREF(self->ob_while.ob_w_block = right->ob_while.ob_w_block);
   break;
  case DeeStmtAstKind_DO_WHILE:
   Dee_INCREF(self->ob_do_while.ob_dw_scope = right->ob_do_while.ob_dw_scope);
   Dee_INCREF(self->ob_do_while.ob_dw_loopcond = right->ob_do_while.ob_dw_loopcond);
   Dee_INCREF(self->ob_do_while.ob_dw_block = right->ob_do_while.ob_dw_block);
   break;
  case DeeStmtAstKind_GOTO:
   self->ob_goto.ob_gt_label_id = right->ob_goto.ob_gt_label_id;
   break;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  case DeeStmtAstKind_THROW:
   Dee_INCREF(self->ob_throw.ob_trw_expr = right->ob_throw.ob_trw_expr);
   break;
  case DeeStmtAstKind_TRY: {
   struct DeeStmtAstTryHandler *begin,*end,*src;
   self->ob_try.ob_try_handlerc = right->ob_try.ob_try_handlerc;
   begin = (struct DeeStmtAstTryHandler *)mallocf_nz(
    self->ob_try.ob_try_handlerc*sizeof(struct DeeStmtAstTryHandler),
    "list of try handlers (%u entries)",(unsigned)self->ob_try.ob_try_handlerc);
   if (!begin) return DeeError_NoMemory(),-1;
   end = (self->ob_try.ob_try_handlerv = begin)+self->ob_try.ob_try_handlerc;
   src = right->ob_try.ob_try_handlerv;
   DEE_ASSERT(begin != end);
   do {
    Dee_XINCREF(begin->ob_local = src->ob_local);
    Dee_XINCREF(begin->ob_type = src->ob_type);
    Dee_INCREF(begin->ob_scope = src->ob_scope);
    Dee_INCREF(begin->ob_block = src->ob_block);
    ++src;
   } while (++begin != end);
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
  case DeeStmtAstKind_EXTERN:
   _DeeExternFuncDecl_InitCopy(&self->ob_extern.ob_extern_decl,&right->ob_extern.ob_extern_decl);
   Dee_INCREF(self->ob_extern.ob_extern_store = right->ob_extern.ob_extern_store);
   break;
  case DeeStmtAstKind_MODULE:
   Dee_INCREF(self->ob_module.ob_mod_block = right->ob_module.ob_mod_block);
   Dee_INCREF(self->ob_module.ob_mod_module = right->ob_module.ob_mod_module);
   break;
  case DeeStmtAstKind_LOOP_ONCE:
   Dee_INCREF(self->ob_loop_once.ob_lonce_block = right->ob_loop_once.ob_lonce_block);
   Dee_INCREF(self->ob_loop_once.ob_lonce_scope = right->ob_loop_once.ob_lonce_scope);
   break;
  case DeeStmtAstKind_SWITCH:
   if (DeeCaseLabelDeclList_InitCopy(
    &self->ob_switch.ob_sw_cases,&right->ob_switch.ob_sw_cases) == -1) return -1;
   Dee_INCREF(self->ob_switch.ob_sw_expr = right->ob_switch.ob_sw_expr);
   Dee_INCREF(self->ob_switch.ob_sw_stmt = right->ob_switch.ob_sw_stmt);
   Dee_INCREF(self->ob_switch.ob_sw_scope = right->ob_switch.ob_sw_scope);
   break;
  default: break;
 }
 Dee_INCREF(self->ob_token = right->ob_token);
 return 0;
}
DEE_COMPILER_MSVC_WARNING_POP

DEE_COMPILER_MSVC_WARNING_PUSH(4061)
static void _deestmtast_tp_dtor(DeeStmtAstObject *self) {
 Dee_DECREF(self->ob_token);
 DeeLabelRefList_Quit(&self->ob_label_endpoints);
 switch (self->ob_kind) {
  case DeeStmtAstKind_INL_STMTS: {
   DeeStmtAstObject **begin,**end;
   end = (begin = self->ob_inl_stmts.ob_inl_stmts_v)+self->ob_inl_stmts.ob_inl_stmts_c;
   while (begin != end) Dee_DECREF(*--end);
   free_nn(begin);
  } break;
  case DeeStmtAstKind_STMTS: {
   DeeStmtAstObject **begin,**end;
   end = (begin = self->ob_stmts.ob_stmts_v)+self->ob_stmts.ob_stmts_c;
   while (begin != end) Dee_DECREF(*--end);
   Dee_DECREF(self->ob_stmts.ob_stmts_scope);
   free_nn(begin);
  } break;
  case DeeStmtAstKind_EXPR:
   Dee_DECREF(self->ob_expr.ob_expr_ast);
   break;
  case DeeStmtAstKind_SSTMT:
   Dee_DECREF(self->ob_sstmt.ob_sstmt_scope);
   Dee_DECREF(self->ob_sstmt.ob_sstmt_ast);
   break;
  case DeeStmtAstKind_SEXPR:
   Dee_DECREF(self->ob_sexpr.ob_sexpr_scope);
   Dee_DECREF(self->ob_sexpr.ob_sexpr_ast);
   break;
  case DeeStmtAstKind_RETURN:
   Dee_XDECREF(self->ob_return.ob_ret_expr);
   break;
  case DeeStmtAstKind_YIELD:
   Dee_DECREF(self->ob_yield.ob_yld_exprs);
   break;
  case DeeStmtAstKind_DEL:
   Dee_DECREF(self->ob_del.ob_del_exprs);
   break;
  case DeeStmtAstKind_ASSERT:
   Dee_DECREF(self->ob_assert.ob_asrt_expr);
   Dee_XDECREF(self->ob_assert.ob_asrt_msg);
   break;
  case DeeStmtAstKind_PRINT:
  case DeeStmtAstKind_PRINT_NOLF:
   Dee_XDECREF(self->ob_print.ob_prt_file);
   Dee_XDECREF(self->ob_print.ob_prt_tpl);
   break;
  case DeeStmtAstKind_IF:
   Dee_DECREF(self->ob_if.ob_if_expr);
   Dee_DECREF(self->ob_if.ob_if_thenstmt);
   Dee_DECREF(self->ob_if.ob_if_thenscope);
   Dee_XDECREF(self->ob_if.ob_if_elsestmt);
   Dee_XDECREF(self->ob_if.ob_if_elsescope);
   break;
  case DeeStmtAstKind_FOR:
   Dee_XDECREF(self->ob_for.ob_f_scope);
   Dee_XDECREF(self->ob_for.ob_f_forinit);
   Dee_XDECREF(self->ob_for.ob_f_forcont);
   Dee_XDECREF(self->ob_for.ob_f_fornext);
   Dee_DECREF(self->ob_for.ob_f_block);
   break;
  case DeeStmtAstKind_FOR_IN:
  case DeeStmtAstKind_FOREACH:
   Dee_DECREF(self->ob_for_in.ob_fi_scope);
   Dee_DECREF(self->ob_for_in.ob_fi_itername);
   Dee_DECREF(self->ob_for_in.ob_fi_iterlist);
   Dee_DECREF(self->ob_for_in.ob_fi_block);
   break;
  case DeeStmtAstKind_WHILE:
   Dee_DECREF(self->ob_while.ob_w_scope);
   Dee_DECREF(self->ob_while.ob_w_loopcond);
   Dee_DECREF(self->ob_while.ob_w_block);
   break;
  case DeeStmtAstKind_DO_WHILE:
   Dee_DECREF(self->ob_do_while.ob_dw_scope);
   Dee_DECREF(self->ob_do_while.ob_dw_loopcond);
   Dee_DECREF(self->ob_do_while.ob_dw_block);
   break;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  case DeeStmtAstKind_THROW:
   Dee_DECREF(self->ob_throw.ob_trw_expr);
   break;
  case DeeStmtAstKind_TRY: {
   struct DeeStmtAstTryHandler *begin,*end;
   Dee_DECREF(self->ob_try.ob_try_scope);
   Dee_DECREF(self->ob_try.ob_try_block);
   end = (begin = self->ob_try.ob_try_handlerv)+self->ob_try.ob_try_handlerc;
   while (begin != end) {
    DeeStmtAstTryHandler_QUIT(begin);
    ++begin;
   }
   free(self->ob_try.ob_try_handlerv);
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
  case DeeStmtAstKind_EXTERN:
   DeeExternFuncDecl_QUIT(&self->ob_extern.ob_extern_decl);
   Dee_DECREF(self->ob_extern.ob_extern_store);
   break;
  case DeeStmtAstKind_MODULE:
   Dee_DECREF(self->ob_module.ob_mod_block);
   Dee_DECREF(self->ob_module.ob_mod_module);
   break;
  case DeeStmtAstKind_LOOP_ONCE:
   Dee_DECREF(self->ob_loop_once.ob_lonce_block);
   Dee_DECREF(self->ob_loop_once.ob_lonce_scope);
   break;
  case DeeStmtAstKind_SWITCH:
   DeeCaseLabelDeclList_Quit(&self->ob_switch.ob_sw_cases);
   Dee_DECREF(self->ob_switch.ob_sw_expr);
   Dee_DECREF(self->ob_switch.ob_sw_stmt);
   Dee_DECREF(self->ob_switch.ob_sw_scope);
   break;
  default: break;
 }
}
DEE_COMPILER_MSVC_WARNING_POP

DEE_COMPILER_MSVC_WARNING_PUSH(4061)
DEE_VISIT_PROC(_deestmtast_tp_visit,DeeStmtAstObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStmtAst_Check(self));
 Dee_VISIT(self->ob_token);
 switch (self->ob_kind) {
  case DeeStmtAstKind_INL_STMTS: {
   DeeStmtAstObject **begin,**end;
   end = (begin = self->ob_inl_stmts.ob_inl_stmts_v)+self->ob_inl_stmts.ob_inl_stmts_c;
   while (begin != end) Dee_VISIT(*--end);
  } break;
  case DeeStmtAstKind_STMTS: {
   DeeStmtAstObject **begin,**end;
   end = (begin = self->ob_stmts.ob_stmts_v)+self->ob_stmts.ob_stmts_c;
   while (begin != end) Dee_VISIT(*--end);
   Dee_VISIT(self->ob_stmts.ob_stmts_scope);
  } break;
  case DeeStmtAstKind_EXPR:
   Dee_VISIT(self->ob_expr.ob_expr_ast);
   break;
  case DeeStmtAstKind_SSTMT:
   Dee_VISIT(self->ob_sstmt.ob_sstmt_scope);
   Dee_VISIT(self->ob_sstmt.ob_sstmt_ast);
   break;
  case DeeStmtAstKind_SEXPR:
   Dee_VISIT(self->ob_sexpr.ob_sexpr_scope);
   Dee_VISIT(self->ob_sexpr.ob_sexpr_ast);
   break;
  case DeeStmtAstKind_RETURN:
   Dee_XVISIT(self->ob_return.ob_ret_expr);
   break;
  case DeeStmtAstKind_YIELD:
   Dee_VISIT(self->ob_yield.ob_yld_exprs);
   break;
  case DeeStmtAstKind_DEL:
   Dee_VISIT(self->ob_del.ob_del_exprs);
   break;
  case DeeStmtAstKind_ASSERT:
   Dee_VISIT(self->ob_assert.ob_asrt_expr);
   Dee_XVISIT(self->ob_assert.ob_asrt_msg);
   break;
  case DeeStmtAstKind_PRINT:
  case DeeStmtAstKind_PRINT_NOLF:
   Dee_XVISIT(self->ob_print.ob_prt_file);
   Dee_XVISIT(self->ob_print.ob_prt_tpl);
   break;
  case DeeStmtAstKind_IF:
   Dee_VISIT(self->ob_if.ob_if_expr);
   Dee_VISIT(self->ob_if.ob_if_thenstmt);
   Dee_VISIT(self->ob_if.ob_if_thenscope);
   Dee_XVISIT(self->ob_if.ob_if_elsestmt);
   Dee_XVISIT(self->ob_if.ob_if_elsescope);
   break;
  case DeeStmtAstKind_FOR:
   Dee_XVISIT(self->ob_for.ob_f_scope);
   Dee_XVISIT(self->ob_for.ob_f_forinit);
   Dee_XVISIT(self->ob_for.ob_f_forcont);
   Dee_XVISIT(self->ob_for.ob_f_fornext);
   Dee_VISIT(self->ob_for.ob_f_block);
   break;
  case DeeStmtAstKind_FOR_IN:
  case DeeStmtAstKind_FOREACH:
   Dee_VISIT(self->ob_for_in.ob_fi_scope);
   Dee_VISIT(self->ob_for_in.ob_fi_itername);
   Dee_VISIT(self->ob_for_in.ob_fi_iterlist);
   Dee_VISIT(self->ob_for_in.ob_fi_block);
   break;
  case DeeStmtAstKind_WHILE:
   Dee_VISIT(self->ob_while.ob_w_scope);
   Dee_VISIT(self->ob_while.ob_w_loopcond);
   Dee_VISIT(self->ob_while.ob_w_block);
   break;
  case DeeStmtAstKind_DO_WHILE:
   Dee_VISIT(self->ob_do_while.ob_dw_scope);
   Dee_VISIT(self->ob_do_while.ob_dw_loopcond);
   Dee_VISIT(self->ob_do_while.ob_dw_block);
   break;
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  case DeeStmtAstKind_THROW:
   Dee_VISIT(self->ob_throw.ob_trw_expr);
   break;
  case DeeStmtAstKind_TRY: {
   struct DeeStmtAstTryHandler *begin,*end;
   Dee_VISIT(self->ob_try.ob_try_scope);
   Dee_VISIT(self->ob_try.ob_try_block);
   end = (begin = self->ob_try.ob_try_handlerv)+self->ob_try.ob_try_handlerc;
   while (begin != end) {
    Dee_XVISIT(begin->ob_local);
    Dee_XVISIT(begin->ob_type);
    Dee_VISIT(begin->ob_scope);
    Dee_VISIT(begin->ob_block);
    ++begin;
   }
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
  case DeeStmtAstKind_EXTERN:
   Dee_VISIT(self->ob_extern.ob_extern_decl.ob_libfile);
   Dee_VISIT(self->ob_extern.ob_extern_decl.ob_name);
   Dee_VISIT(self->ob_extern.ob_extern_decl.ob_type);
   Dee_VISIT(self->ob_extern.ob_extern_store);
   break;
  case DeeStmtAstKind_MODULE:
   Dee_VISIT(self->ob_module.ob_mod_block);
   Dee_VISIT(self->ob_module.ob_mod_module);
   break;
  case DeeStmtAstKind_LOOP_ONCE:
   Dee_VISIT(self->ob_loop_once.ob_lonce_block);
   Dee_VISIT(self->ob_loop_once.ob_lonce_scope);
   break;
  case DeeStmtAstKind_SWITCH:
   // TODO: Visit switch cases
   //DeeCaseLabelDeclList_Visit(&self->ob_switch.ob_sw_cases);
   Dee_VISIT(self->ob_switch.ob_sw_expr);
   Dee_VISIT(self->ob_switch.ob_sw_stmt);
   Dee_VISIT(self->ob_switch.ob_sw_scope);
   break;
  default: break;
 }
}
DEE_COMPILER_MSVC_WARNING_POP

DEE_STATIC_INLINE(DeeObject *) _deestmts_inlrepr(Dee_size_t c, DeeStmtAstObject **v) {
 {DeeObject *result;
  DeeStringWriter writer = DeeStringWriter_INIT();
  while (c--) {
   if (DeeStringWriter_WRITE(&writer,"\t") == -1 ||
       DeeStringWriter_WriteObjectRepr(&writer,(DeeObject *)*v++) == -1 ||
       DeeStringWriter_WRITE(&writer,"\n") == -1) goto err;
  }
  result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
  return result;
err: result = NULL; goto end;
 }
}
DEE_STATIC_INLINE(DeeObject *) _deestmts_repr(Dee_size_t c, DeeStmtAstObject **v) {
 {DeeObject *result;
  DeeStringWriter writer = DeeStringWriter_INIT();
  if (DeeStringWriter_WRITE(&writer,"{\n") == -1) goto err;
  while (c--) {
   if (DeeStringWriter_WRITE(&writer,"\t") == -1 ||
       DeeStringWriter_WriteObjectRepr(&writer,(DeeObject *)*v++) == -1 ||
       DeeStringWriter_WRITE(&writer,"\n") == -1) goto err;
  }
  if (DeeStringWriter_WRITE(&writer,"}") == -1) goto err;
  result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
  return result;
err: result = NULL; goto end;
 }
}

static DeeObject *_DeeStmtAst_BasicRepr(DeeStmtAstObject *self) {
 switch (self->ob_kind) {
  case DeeStmtAstKind_EMPTY:
   DeeReturn_STATIC_STRING(";");
  case DeeStmtAstKind_STMTS:
   return _deestmts_repr(self->ob_stmts.ob_stmts_c,self->ob_stmts.ob_stmts_v);
  case DeeStmtAstKind_EXPR:
   return DeeString_Newf("%r;",self->ob_expr.ob_expr_ast);
  case DeeStmtAstKind_RETURN:
   return DeeString_Newf("return %r;",self->ob_return.ob_ret_expr);
  case DeeStmtAstKind_YIELD:
   return DeeString_Newf("yield %r;",self->ob_yield.ob_yld_exprs);
  case DeeStmtAstKind_DEL:
   return DeeString_Newf("del %r;",self->ob_del.ob_del_exprs);
  case DeeStmtAstKind_BREAK:
   DeeReturn_STATIC_STRING("break;");
  case DeeStmtAstKind_CONTINUE:
   DeeReturn_STATIC_STRING("continue;");
  case DeeStmtAstKind_ASSERT:
   return self->ob_assert.ob_asrt_msg
    ? DeeString_Newf("assert %r: %r;",self->ob_assert.ob_asrt_expr,self->ob_assert.ob_asrt_msg)
    : DeeString_Newf("assert %r;",self->ob_assert.ob_asrt_expr);
  case DeeStmtAstKind_PRINT:
   if (self->ob_print.ob_prt_tpl) {
    return self->ob_print.ob_prt_file
     ? DeeString_Newf("print %r: pack%r...;",self->ob_print.ob_prt_file,self->ob_print.ob_prt_tpl)
     : DeeString_Newf("print pack%r...;",self->ob_print.ob_prt_tpl);
   } else if (self->ob_print.ob_prt_file) {
    return DeeString_Newf("print pack%r...:;",self->ob_print.ob_prt_file);
   } else {
    DeeReturn_STATIC_STRING("print;");
   }
  case DeeStmtAstKind_PRINT_NOLF:
   if (self->ob_print.ob_prt_tpl) {
    return self->ob_print.ob_prt_file
     ? DeeString_Newf("print %r: pack%r...,;",self->ob_print.ob_prt_file,self->ob_print.ob_prt_tpl)
     : DeeString_Newf("print pack%r...,;",self->ob_print.ob_prt_tpl);
   } else if (self->ob_print.ob_prt_file) {
    return DeeString_Newf("print pack%r...:,;",self->ob_print.ob_prt_file);
   } else {
    DeeReturn_STATIC_STRING("print,;");
   }
  case DeeStmtAstKind_IF:
   return self->ob_if.ob_if_elsestmt
    ? DeeString_Newf("if (%r) %r else %r",self->ob_if.ob_if_expr,self->ob_if.ob_if_thenstmt,self->ob_if.ob_if_elsestmt)
    : DeeString_Newf("if (%r) %r",        self->ob_if.ob_if_expr,self->ob_if.ob_if_thenstmt);
  case DeeStmtAstKind_FOR:
   return DeeString_Newf("for (%K;%K;%K) %r",
    self->ob_for.ob_f_forinit ? DeeObject_Repr((DeeObject *)self->ob_for.ob_f_forinit) : DeeString_NewEmpty(),
    self->ob_for.ob_f_forcont ? DeeObject_Repr((DeeObject *)self->ob_for.ob_f_forcont) : DeeString_NewEmpty(),
    self->ob_for.ob_f_fornext ? DeeObject_Repr((DeeObject *)self->ob_for.ob_f_fornext) : DeeString_NewEmpty(),
    self->ob_for.ob_f_block);
  case DeeStmtAstKind_FOR_IN:
   return DeeString_Newf("for (@%u: %r) %r",
    self->ob_for_in.ob_fi_itername->ob_name,
    self->ob_for_in.ob_fi_iterlist,
    self->ob_for_in.ob_fi_block);
  case DeeStmtAstKind_FOREACH:
   return DeeString_Newf("__foreach(@%u: %r) %r",
    self->ob_for_in.ob_fi_itername->ob_name,
    self->ob_for_in.ob_fi_iterlist,
    self->ob_for_in.ob_fi_block);
  case DeeStmtAstKind_WHILE:
   return DeeString_Newf("while (%r) %r",
    self->ob_while.ob_w_loopcond,
    self->ob_while.ob_w_block);
  case DeeStmtAstKind_DO_WHILE:
   return DeeString_Newf("do %r while (%r);",
    self->ob_do_while.ob_dw_block,
    self->ob_do_while.ob_dw_loopcond);
  case DeeStmtAstKind_GOTO:
   return DeeString_Newf("goto @%Iu;",self->ob_goto.ob_gt_label_id);
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
  case DeeStmtAstKind_THROW:
   return DeeString_Newf("throw %r;",self->ob_throw.ob_trw_expr);
  case DeeStmtAstKind_RETHROW:
   DeeReturn_STATIC_STRING("throw;");
  case DeeStmtAstKind_TRY: {
   DeeObject *result;
   struct DeeStmtAstTryHandler *iter,*end;
   DeeStringWriter writer = DeeStringWriter_INIT();
   if (DeeStringWriter_WRITE(&writer,"try ") == -1) goto err_try;
   if (DeeStringWriter_WriteObjectRepr(&writer,(DeeObject *)self->ob_try.ob_try_block) == -1) goto err_try;
   end = (iter = self->ob_try.ob_try_handlerv)+self->ob_try.ob_try_handlerc;
   while (iter != end) {
    switch (iter->ob_kind) {
     case DeeExceptionHandleKind_FINALLY:
      if (DeeStringWriter_WRITE(&writer," finally ") == -1) goto err_try;
      break;
     case DeeExceptionHandleKind_FLAG_ALL:
      if (DeeStringWriter_WRITE(&writer," catch(...) ") == -1) goto err_try;
      break;
     case DeeExceptionHandleKind_FLAG_ALL|DeeExceptionHandleKind_FLAG_VAR:
      if (DeeStringWriter_Writef(&writer," catch(@%u...) ",iter->ob_local->ob_name) == -1) goto err_try;
      break;
     case DeeExceptionHandleKind_FLAG_TYPED:
      if (DeeStringWriter_Writef(&writer," catch(%k) ",iter->ob_type) == -1) goto err_try;
      break;
     case DeeExceptionHandleKind_FLAG_TYPED|DeeExceptionHandleKind_FLAG_VAR:
      if (DeeStringWriter_Writef(&writer," catch(%k @%u) ",iter->ob_type,iter->ob_local->ob_name) == -1) goto err_try;
      break;
     default: DEE_BUILTIN_UNREACHABLE();
    }
    if (DeeStringWriter_WriteObjectRepr(&writer,(DeeObject *)iter->ob_block) == -1) goto err_try;
    ++iter;
   }
   result = DeeStringWriter_Pack(&writer);
end_try: DeeStringWriter_Quit(&writer);
   return result;
err_try: result = NULL; goto end_try;
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */

  case DeeStmtAstKind_EXTERN:
   return DeeString_Newf(
    "extern %r: [[name(%r)]] @%u %k;",
    self->ob_extern.ob_extern_decl.ob_libfile,
    self->ob_extern.ob_extern_decl.ob_name,
    self->ob_extern.ob_extern_store->ob_name,
    self->ob_extern.ob_extern_decl.ob_type);

  case DeeStmtAstKind_MODULE:
   return DeeString_Newf(
    "module @%u %r",
    self->ob_module.ob_mod_module->ob_name,
    self->ob_module.ob_mod_block);

  case DeeStmtAstKind_SEXPR:
   return DeeString_Newf("%r;",self->ob_sexpr.ob_sexpr_ast);

  case DeeStmtAstKind_SSTMT:
   return DeeObject_Repr((DeeObject *)self->ob_sstmt.ob_sstmt_ast);
  case DeeStmtAstKind_INL_STMTS:
   return _deestmts_inlrepr(
    self->ob_inl_stmts.ob_inl_stmts_c,
    self->ob_inl_stmts.ob_inl_stmts_v);

  case DeeStmtAstKind_LOOP_ONCE:
   return DeeString_Newf("__looponce %r",self->ob_loop_once.ob_lonce_block);

  case DeeStmtAstKind_SWITCH:
   return DeeString_Newf(
    "switch (%r) %r",
    self->ob_switch.ob_sw_expr,
    self->ob_switch.ob_sw_stmt);
  default:
   DeeReturn_STATIC_STRING("<unknown_stmt>");
 }
}

static DeeObject *_deestmtast_tp_repr(DeeStmtAstObject *self) {
 if (self->ob_label_endpoints.ob_c) {
  DeeObject *result,*temp; struct DeeLabelRef *iter,*end; int itemp;
  DeeStringWriter writer = DeeStringWriter_INIT();
  end = (iter = self->ob_label_endpoints.ob_v)+self->ob_label_endpoints.ob_c;
  while (iter != end) {
   if ((iter->ob_flags&DEE_LABEL_REF_FLAGS_CASELABEL)!=0) {
    if (DeeStringWriter_Writef(&writer,"case @%u: ",iter->ob_id) == -1) goto err;
   } else {
    if (DeeStringWriter_Writef(&writer,"@%u: ",iter->ob_id) == -1) goto err;
   }
   ++iter;
  }
  if ((temp = _DeeStmtAst_BasicRepr(self)) == NULL) goto err;
  itemp = DeeStringWriter_WriteObjectStr(&writer,temp);
  Dee_DECREF(temp);
  if (itemp == -1) goto err;
  result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
  return result;
err: result = NULL; goto end;
 } else {
  return _DeeStmtAst_BasicRepr(self);
 }
}

static int _deestmtast_tp_copy_assign(DeeStmtAstObject *self, DeeStmtAstObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeLocalVar_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeLocalVar_Check(right));
 if (self != right) {
  DeeStmtAstObject temp = {DEE_OBJECT_HEAD_INIT(&DeeStmtAst_Type)};
  if (_deestmtast_tp_copy_ctor(Dee_TYPE(self),&temp,right) == -1) return -1;
  _deestmtast_tp_dtor(self);
  memcpy(DeeObject_ABSOLUTE_DATA(self),DeeObject_ABSOLUTE_DATA(&temp),
         sizeof(DeeStmtAstObject)-sizeof(DeeObject));
 }
 return 0;
}

static DeeObject *_deestmtast_tp_alloc(DeeTypeObject *tp_self) {
 DeeObject *result = _DeeStmtAstPool_Malloc();
 if (result) DeeObject_INIT(result,tp_self);
 return result;
}
static void _deestmtast_tp_free(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *ob) {
 _DeeStmtAstPool_Free(ob);
}


static struct DeeMemberDef const _deemodule_tp_members[] = {
 DEE_MEMBERDEF_RO_v100(DeeStmtAstObject,ob_token,object),
 DEE_MEMBERDEF_RO_v100(DeeStmtAstObject,ob_kind,enum),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_stmts_scope",DeeStmtAstObject,ob_stmts.ob_stmts_scope,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_stmts_c",DeeStmtAstObject,ob_stmts.ob_stmts_c,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_stmts_v",DeeStmtAstObject,ob_stmts.ob_stmts_c,p2(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_expr_ast",DeeStmtAstObject,ob_expr.ob_expr_ast,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_sstmt_scope",DeeStmtAstObject,ob_sstmt.ob_sstmt_scope,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_sstmt_ast",DeeStmtAstObject,ob_sstmt.ob_sstmt_ast,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_sexpr_scope",DeeStmtAstObject,ob_sexpr.ob_sexpr_scope,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_sexpr_ast",DeeStmtAstObject,ob_sexpr.ob_sexpr_ast,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_ret_expr",DeeStmtAstObject,ob_return.ob_ret_expr,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_yld_exprs",DeeStmtAstObject,ob_yield.ob_yld_exprs,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_del_exprs",DeeStmtAstObject,ob_del.ob_del_exprs,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_asrt_expr",DeeStmtAstObject,ob_assert.ob_asrt_expr,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_asrt_msg",DeeStmtAstObject,ob_assert.ob_asrt_msg,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_prt_file",DeeStmtAstObject,ob_print.ob_prt_file,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_prt_tpl",DeeStmtAstObject,ob_print.ob_prt_tpl,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_if_expr",DeeStmtAstObject,ob_if.ob_if_expr,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_if_thenstmt",DeeStmtAstObject,ob_if.ob_if_thenstmt,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_if_thenscope",DeeStmtAstObject,ob_if.ob_if_thenscope,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_if_elsestmt",DeeStmtAstObject,ob_if.ob_if_elsestmt,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_if_elsescope",DeeStmtAstObject,ob_if.ob_if_elsestmt,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_f_scope",DeeStmtAstObject,ob_for.ob_f_scope,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_f_forinit",DeeStmtAstObject,ob_for.ob_f_forinit,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_f_forcont",DeeStmtAstObject,ob_for.ob_f_forcont,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_f_fornext",DeeStmtAstObject,ob_for.ob_f_fornext,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_f_block",DeeStmtAstObject,ob_for.ob_f_block,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_fi_scope",DeeStmtAstObject,ob_for_in.ob_fi_scope,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_fi_itername",DeeStmtAstObject,ob_for_in.ob_fi_itername,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_fi_iterlist",DeeStmtAstObject,ob_for_in.ob_fi_iterlist,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_fi_block",DeeStmtAstObject,ob_for_in.ob_fi_block,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_w_scope",DeeStmtAstObject,ob_while.ob_w_scope,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_w_loopcond",DeeStmtAstObject,ob_while.ob_w_loopcond,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_w_block",DeeStmtAstObject,ob_while.ob_w_block,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_dw_scope",DeeStmtAstObject,ob_do_while.ob_dw_scope,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_dw_loopcond",DeeStmtAstObject,ob_do_while.ob_dw_loopcond,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_dw_block",DeeStmtAstObject,ob_do_while.ob_dw_block,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_gt_label_id",DeeStmtAstObject,ob_goto.ob_gt_label_id,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_trw_expr",DeeStmtAstObject,ob_throw.ob_trw_expr,object),
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 DEE_MEMBERDEF_NAMED_RO_v100("ob_try_scope",DeeStmtAstObject,ob_try.ob_try_scope,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_try_block",DeeStmtAstObject,ob_try.ob_try_block,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_try_handlerc",DeeStmtAstObject,ob_try.ob_try_handlerc,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_try_handlerv",DeeStmtAstObject,ob_try.ob_try_handlerc,p(void)),
#endif
 DEE_MEMBERDEF_NAMED_RO_v100("ob_extern_decl_dll",DeeStmtAstObject,ob_extern.ob_extern_decl.ob_libfile,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_extern_decl_name",DeeStmtAstObject,ob_extern.ob_extern_decl.ob_name,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_extern_decl_type",DeeStmtAstObject,ob_extern.ob_extern_decl.ob_type,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_extern_store",DeeStmtAstObject,ob_extern.ob_extern_store,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_mod_module",DeeStmtAstObject,ob_module.ob_mod_module,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_mod_block",DeeStmtAstObject,ob_module.ob_mod_block,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_sw_expr",DeeStmtAstObject,ob_switch.ob_sw_expr,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_sw_stmt",DeeStmtAstObject,ob_switch.ob_sw_stmt,object),
 DEE_MEMBERDEF_NAMED_RO_v100("ob_sw_scope",DeeStmtAstObject,ob_switch.ob_sw_scope,object),
 DEE_MEMBERDEF_END_v100
};



DeeTypeObject DeeStmtAst_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("stmt_ast"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeStmtAstObject),
  member(&_deestmtast_tp_alloc),null,
  member(&_deestmtast_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(
  member(&_deestmtast_tp_free),
  member(&_deestmtast_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deestmtast_tp_copy_assign),null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,
  member(&_deestmtast_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deestmtast_tp_visit),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deemodule_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DEE_DECL_END
#endif /* !DEE_CONFIG_COMPILER_USE_NEW_AST_TYPES */

#endif /* !GUARD_DEEMON_STMT_AST_C */
