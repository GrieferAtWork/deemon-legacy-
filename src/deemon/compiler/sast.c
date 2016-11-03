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
#ifndef GUARD_DEEMON_SAST_C
#define GUARD_DEEMON_SAST_C 1
#define DEE_LIMITED_API 1

// include/*
#include <deemon/__conf.inl>
#include <deemon/bool.h>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/generic_vtable.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/optional/object_pool.h>
#include <deemon/optional/object_visit.h>
#include <deemon/file.h>
#include <deemon/object.h>
#include <deemon/string.h>
#include <deemon/type.h>

// src/*
#include <deemon/__xconf.inl>
#include <deemon/compiler/sast.h>
#include <deemon/compiler/xast.h>

// */ (nano...)

DEE_DECL_BEGIN

#define _DeeSAstCommonAst_Quit(ob)\
do{\
 struct DeeParserLabel **label_iter,**label_end;\
 Dee_DECREF((ob)->ast_token);\
 label_end = (label_iter = DeeParserLabelRefList_ELEM(&(ob)->ast_labels)\
              )+DeeParserLabelRefList_SIZE(&(ob)->ast_labels);\
 while (label_iter != label_end) { Dee_CLEAR((*label_iter)->pl_dest_token); ++label_iter; }\
 _DeeParserLabelRefList_Quit(&(ob)->ast_labels);\
}while(0)
#ifdef _DeeLabelRefList_Visit
#define _DeeSAstCommonAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->ast_token);\
 _DeeLabelRefList_Visit(&(ob)->ast_labels);\
}while(0)
#else
#define _DeeSAstCommonAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->ast_token);\
}while(0)
#endif

#define _DeeSAstBlockAst_Quit(ob)\
do{\
 struct DeeSAstObject **iter,**end;\
 Dee_DECREF((ob)->b_scope);\
 end = (iter = (ob)->b_astv)+(ob)->b_astc;\
 while (iter != end) Dee_DECREF(*iter++);\
 free((ob)->b_astv);\
}while(0)
#define _DeeSAstReturnAst_Quit(ob)\
do{\
 Dee_XDECREF((ob)->r_value);\
}while(0)
#define _DeeSAstYieldAst_Quit(ob)\
do{\
 Dee_DECREF((ob)->y_value);\
}while(0)
#define _DeeSAstAssertAst_Quit(ob)\
do{\
 Dee_DECREF((ob)->a_check);\
 Dee_XDECREF((ob)->a_message);\
}while(0)
#define _DeeSAstPrintAst_Quit(ob)\
do{\
 Dee_XDECREF((ob)->p_file);\
 Dee_XDECREF((ob)->p_data);\
}while(0)
#define _DeeSAstIfAst_Quit(ob)\
do{\
 Dee_DECREF((ob)->if_cond_scope);\
 Dee_DECREF((ob)->if_cond);\
 Dee_XDECREF((ob)->if_succ_scope);\
 Dee_XDECREF((ob)->if_succ_block);\
 Dee_XDECREF((ob)->if_fail_scope);\
 Dee_XDECREF((ob)->if_fail_block);\
}while(0)
#define _DeeSAstForAst_Quit(ob)\
do{\
 Dee_DECREF((ob)->f_head_init_scope);\
 Dee_XDECREF((ob)->f_head_init);\
 Dee_DECREF((ob)->f_head_cond_scope);\
 Dee_DECREF((ob)->f_head_cond);\
 Dee_DECREF((ob)->f_scope);\
 Dee_XDECREF((ob)->f_head_next);\
 Dee_DECREF((ob)->f_block);\
}while(0)
#define _DeeSAstForInAst_Quit(ob)\
do{\
 _DeeVarDeclStorage_Quit(&(ob)->fi_var);\
 Dee_XDECREF((ob)->fi_seq_scope);\
 Dee_DECREF((ob)->fi_seq);\
 Dee_XDECREF((ob)->fi_scope);\
 Dee_XDECREF((ob)->fi_block);\
}while(0)
#define _DeeSAstWhileAst_Quit(ob)\
do{\
 Dee_DECREF((ob)->w_cond_scope);\
 Dee_DECREF((ob)->w_cond);\
 Dee_DECREF((ob)->w_scope);\
 Dee_DECREF((ob)->w_block);\
}while(0)
#define _DeeSAstSpecialLoopAst_Quit(ob)\
do{\
 Dee_DECREF((ob)->sl_scope);\
 Dee_DECREF((ob)->sl_block);\
}while(0)
#define _DeeSAstGotoAst_Quit(ob)\
do{\
 DeeParserLabel_REM_GOTO((ob)->g_label);\
}while(0)
#define _DeeSAstThrowAst_Quit(ob)\
do{\
 Dee_XDECREF((ob)->t_object);\
}while(0)
#define _DeeSAstTryAst_Quit(ob)\
do{\
 struct DeeSAstTryHandler *iter,*end;\
 Dee_DECREF((ob)->t_rootscope);\
 Dee_DECREF((ob)->t_scope);\
 Dee_DECREF((ob)->t_block);\
 end = (iter = (ob)->t_handlerv)+(ob)->t_handlerc;\
 while (iter != end) { _DeeSAstTryHandler_Quit(iter); ++iter; }\
 free((ob)->t_handlerv);\
}while(0)
#define _DeeSAstModuleAst_Quit(ob)\
do{\
 Dee_DECREF((ob)->m_module);\
 Dee_DECREF((ob)->m_block);\
}while(0)
#define _DeeSAstSwitchAst_Quit(ob)\
do{\
 Dee_DECREF((ob)->sw_expr_scope);\
 Dee_DECREF((ob)->sw_expr);\
 Dee_DECREF((ob)->sw_scope);\
 Dee_DECREF((ob)->sw_block);\
 _DeeParserLabelList_Quit(&(ob)->sw_cases);\
}while(0)
#define _DeeSAstIfConstAst_Quit(ob)\
do{\
 Dee_XDECREF((ob)->ic_succ_scope);\
 Dee_XDECREF((ob)->ic_succ_block);\
 Dee_XDECREF((ob)->ic_fail_scope);\
 Dee_XDECREF((ob)->ic_fail_block);\
}while(0)


#define _DeeSAstTryHandler_Visit(ob)\
do{\
 _DeeVarDeclStorage_Visit(&(ob)->th_local);\
 Dee_XVISIT((ob)->th_type);\
 Dee_VISIT((ob)->th_scope);\
 Dee_VISIT((ob)->th_block);\
}while(0)

#define _DeeSAstExpressionAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->x_ast);\
}while(0)
#define _DeeSAstBlockAst_Visit(ob)\
do{\
 struct DeeSAstObject **iter,**end;\
 Dee_VISIT((ob)->b_scope);\
 end = (iter = (ob)->b_astv)+(ob)->b_astc;\
 while (iter != end) Dee_VISIT(*iter++);\
}while(0)
#define _DeeSAstReturnAst_Visit(ob)\
do{\
 Dee_XVISIT((ob)->r_value);\
}while(0)
#define _DeeSAstYieldAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->y_value);\
}while(0)
#define _DeeSAstAssertAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->a_check);\
 Dee_XVISIT((ob)->a_message);\
}while(0)
#define _DeeSAstPrintAst_Visit(ob)\
do{\
 Dee_XVISIT((ob)->p_file);\
 Dee_XVISIT((ob)->p_data);\
}while(0)
#define _DeeSAstIfAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->if_cond_scope);\
 Dee_VISIT((ob)->if_cond);\
 Dee_XVISIT((ob)->if_succ_scope);\
 Dee_XVISIT((ob)->if_succ_block);\
 Dee_XVISIT((ob)->if_fail_scope);\
 Dee_XVISIT((ob)->if_fail_block);\
}while(0)
#define _DeeSAstForAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->f_head_init_scope);\
 Dee_XVISIT((ob)->f_head_init);\
 Dee_VISIT((ob)->f_head_cond_scope);\
 Dee_VISIT((ob)->f_head_cond);\
 Dee_VISIT((ob)->f_scope);\
 Dee_XVISIT((ob)->f_head_next);\
 Dee_VISIT((ob)->f_block);\
}while(0)
#define _DeeSAstForInAst_Visit(ob)\
do{\
 _DeeVarDeclStorage_Visit(&(ob)->fi_var);\
 Dee_XVISIT((ob)->fi_seq_scope);\
 Dee_VISIT((ob)->fi_seq);\
 Dee_XVISIT((ob)->fi_scope);\
 Dee_XVISIT((ob)->fi_block);\
}while(0)
#define _DeeSAstWhileAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->w_cond_scope);\
 Dee_VISIT((ob)->w_cond);\
 Dee_VISIT((ob)->w_scope);\
 Dee_VISIT((ob)->w_block);\
}while(0)
#define _DeeSAstSpecialLoopAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->sl_scope);\
 Dee_VISIT((ob)->sl_block);\
}while(0)
#define _DeeSAstThrowAst_Visit(ob)\
do{\
 Dee_XVISIT((ob)->t_object);\
}while(0)
#define _DeeSAstTryAst_Visit(ob)\
do{\
 struct DeeSAstTryHandler *iter,*end;\
 Dee_VISIT((ob)->t_rootscope);\
 Dee_VISIT((ob)->t_scope);\
 Dee_VISIT((ob)->t_block);\
 end = (iter = (ob)->t_handlerv)+(ob)->t_handlerc;\
 while (iter != end) { _DeeSAstTryHandler_Visit(iter); ++iter; }\
}while(0)
#define _DeeSAstModuleAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->m_module);\
 Dee_VISIT((ob)->m_block);\
}while(0)
#define _DeeSAstSwitchAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->sw_expr_scope);\
 Dee_VISIT((ob)->sw_expr);\
 Dee_VISIT((ob)->sw_scope);\
 Dee_VISIT((ob)->sw_block);\
 _DeeParserLabelList_Visit(&(ob)->sw_cases);\
}while(0)
#define _DeeSAstIfConstAst_Visit(ob)\
do{\
 Dee_XVISIT((ob)->ic_succ_scope);\
 Dee_XVISIT((ob)->ic_succ_block);\
 Dee_XVISIT((ob)->ic_fail_scope);\
 Dee_XVISIT((ob)->ic_fail_block);\
}while(0)



int DEE_CALL _DeeSAst_HasLabelVisit(
 DEE_A_IN DeeSAstObject *ast, struct DeeParserLabel *lbl) {
 struct DeeParserLabel **iter,**end;
 if (DeeParserLabelRefList_EMPTY(&ast->ast_common.ast_labels)) return 0;
 end = (iter = ast->ast_common.ast_labels.lrl_v)+ast->ast_common.ast_labels.lrl_c;
 do if (*iter == lbl) return 1; while (++iter != end);
 return 0;
}
int DEE_CALL _DeeSAst_HasLabelsVisit(
 DEE_A_IN DeeSAstObject *ast, void *DEE_UNUSED(closure)) {
 return !DeeParserLabelRefList_EMPTY(&ast->ast_common.ast_labels);
}
int DEE_CALL _DeeSAst_HasBreakContinueVisit(
 DEE_A_IN DeeSAstObject *ast, void *DEE_UNUSED(closure)) {
 return ast->ast_kind == DEE_SASTKIND_BREAK || ast->ast_kind == DEE_SASTKIND_CONTINUE;
}


DEE_OBJECTPOOL_DEFINE(sast,DeeSAstObject)

DEE_A_RET_WUNUSED Dee_size_t DeeSAst_ClearPool(void) {
 return DEE_OBJECTPOOL_CLEAR(sast);
}

DEE_A_RET_EXCEPT_REF DeeSAstObject *_DeeSAst_NewUnsafe(
 DEE_A_IN DeeSAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_) {
 DeeSAstObject *result;
 DEE_ASSERT(labels_);
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 if ((result = DEE_OBJECTPOOL_ALLOC(sast)) != NULL) {
  DeeObject_INIT(result,&DeeSAst_Type);
  result->ast_common.ast_kind = kind;
  _DeeParserLabelRefList_InitMove(&result->ast_common.ast_labels,labels_);
  Dee_INCREF(result->ast_common.ast_token = tk);
 }
 return result;
}

DEE_A_RET_EXCEPT_REF DeeSAstObject *_DeeSAst_NewEmptyEx(
 DEE_A_IN DeeSAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_) {
 DeeSAstObject *result;
 DEE_ASSERT(kind == DEE_SASTKIND_EMPTY ||
            kind == DEE_SASTKIND_CONTINUE ||
            kind == DEE_SASTKIND_BREAK);
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 if ((result = DEE_OBJECTPOOL_ALLOC(sast)) != NULL) {
  DeeObject_INIT(result,&DeeSAst_Type);
  result->ast_common.ast_kind = kind;
  _DeeParserLabelRefList_InitMove(&result->ast_common.ast_labels,labels_);
  Dee_INCREF(result->ast_common.ast_token = tk);
 }
 return result;
}


DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewExpression(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeXAstObject *ast_expr) {
 DeeSAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(ast_expr) && DeeXAst_Check(ast_expr));
 DEE_ASSERT(labels_);
 (void)lexer;
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_MERGE_OPERATORS)!=0) {
  switch (ast_expr->ast_kind) {
   case DEE_XASTKIND_CONST:
    if (DeeNone_Check(ast_expr->ast_const.c_const))
     return DeeSAst_NewEmpty(tk,labels_); // Empty statements evalute to 'none'
    break;
   case DEE_XASTKIND_STATEMENT:
    Dee_INCREF(ast_expr->ast_statement.s_stmt);
    return ast_expr->ast_statement.s_stmt;
   default: break;
  }
 }
 if ((result = _DeeSAst_NewUnsafe(DEE_SASTKIND_EXPRESSION,tk,labels_)) != NULL) {
  Dee_INCREF(result->ast_expression.x_ast = ast_expr);
 }
 return result;
}


DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewBlockFromInheritedStatements(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeScopeObject *scope, DEE_A_IN Dee_size_t statementc,
 DEE_A_IN_R(statementc) DeeSAstObject **statementv) {
 DeeSAstObject *result,**iter,**end;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 DEE_ASSERT(labels_);
#ifdef DEE_DEBUG
 end = (iter = statementv)+statementc;
 while (iter != end) { DEE_ASSERT(DeeObject_Check(*iter) && DeeSAst_Check(*iter)); ++iter; }
#endif
 DEE_ASSERT(statementv);
 (void)lexer;
 if (!statementc) {
  if ((result = DeeSAst_NewEmpty(tk,labels_)) == NULL) return NULL;
  goto return_block_unused;
 }
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_MERGE_OPERATORS)!=0) {
  if (statementc == 1 && !scope->sc_namesc) {
   DeeScopeObject *new_parent;
   struct DeeScopeList child_scopes = DeeScopeList_INIT();
   // Replace 'ob_weak_prev' in all sub-scopes of 'scope' with 'scope->ob_weak_prev'
   if (DeeSAst_CollectTopScopeChildren(statementv[0],&child_scopes) != 0) {
    _DeeScopeList_Quit(&child_scopes);
    return NULL;
   }
   new_parent = scope->sc_weak_prev;
   if (DeeScopeList_SIZE(&child_scopes)) {
    DeeScopeObject **iter,**end,*elem;
    if (!new_parent) {
     // This scope must prevail, as we
     // have no one to replace the parent with
     _DeeScopeList_Quit(&child_scopes);
     goto normal_ast;
    }
    // Replace 'ob_weak_prev' in all children with 'new_parent'
    end = (iter = DeeScopeList_ELEM(&child_scopes))+DeeScopeList_SIZE(&child_scopes);
    while (iter != end) {
     elem = *iter++;
     //DEE_LDEBUG("Updating scope parent in:\n\t%>r\n",statementv[0]);
     if (elem->sc_weak_prev) {
      DEE_ASSERTF(elem->sc_weak_prev == scope,
                  "Invalid parent or missing scope in %r",statementv[0]);
      elem->sc_weak_prev = new_parent;
     } else {
      // Can happen if a function is being declared in 'statementv[0]'
      DEE_ASSERTF(elem->sc_prev == scope,
                  "Invalid parent or missing scope in %r",statementv[0]);
      elem->sc_prev = new_parent;
     }
    }
   }
   _DeeScopeList_Quit(&child_scopes);
   // Remove unnecessary blocks
   Dee_INHERIT_REF(result,statementv[0]);
   free_nn(statementv);
   if (DeeParserLabelRefList_AppendMoveAll(&result->ast_common.ast_labels,labels_) != 0) Dee_CLEAR(result);
   return result;
  }
 }
normal_ast:
 if ((result = _DeeSAst_NewUnsafe(DEE_SASTKIND_BLOCK,tk,labels_)) != NULL) {
  Dee_INCREF(result->ast_block.b_scope = scope);
  result->ast_block.b_astv = statementv;
  result->ast_block.b_astc = statementc;
 }
 return result;
return_block_unused:
 end = (iter = statementv)+statementc;
 while (iter != end) Dee_DECREF(*iter++);
 free(statementv);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewBlock(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeScopeObject *scope, DEE_A_IN Dee_size_t statementc,
 DEE_A_IN_R(statementc) DeeSAstObject *const *statementv) {
 DeeSAstObject *result,**iter,**end,**my_statementv;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 DEE_ASSERT(labels_);
#ifdef DEE_DEBUG
 end = (iter = (DeeSAstObject **)statementv)+statementc;
 while (iter != end) { DEE_ASSERT(DeeObject_Check(*iter) && DeeSAst_Check(*iter)); ++iter; }
#endif
 if (!statementc) return DeeSAst_NewEmpty(tk,labels_);
 while ((my_statementv = (DeeSAstObject **)malloc_nz(
  statementc*sizeof(DeeSAstObject *))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 end = (iter = my_statementv)+statementc;
 do Dee_INCREF(*iter++ = *statementv++); while (iter != end);
 result = DeeSAst_NewBlockFromInheritedStatements(tk,labels_,lexer,parser_flags,
                                                  scope,statementc,my_statementv);
 if (!result) { // Failed to create block (must undo what we did)
  end = (iter = my_statementv)+statementc;
  do Dee_DECREF(*iter++); while (iter != end);
  free_nn(my_statementv);
 }
 return result;
}


DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewReturn(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT_OPT DeeXAstObject *return_expr) {
 DeeSAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(!return_expr || (DeeObject_Check(return_expr) && DeeXAst_Check(return_expr)));
 DEE_ASSERT(labels_);
 if ((result = _DeeSAst_NewUnsafe(DEE_SASTKIND_RETURN,tk,labels_)) != NULL) {
  Dee_XINCREF(result->ast_return.r_value = return_expr);
 }
 return result;
}



DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewYield(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeXAstObject *yield_expr) {
 DeeSAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(yield_expr) && DeeXAst_Check(yield_expr));
 DEE_ASSERT(DeeXAst_IsTuple(yield_expr));
 DEE_ASSERT(labels_);
 if ((result = _DeeSAst_NewUnsafe(DEE_SASTKIND_YIELD,tk,labels_)) != NULL) {
  Dee_INCREF(result->ast_yield.y_value = yield_expr);
 }
 return result;
}




DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewAssert(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeXAstObject *check_expr, DEE_A_INOUT_OPT DeeXAstObject *fail_msg) {
 DeeSAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(check_expr) && DeeXAst_Check(check_expr));
 DEE_ASSERT(!fail_msg || (DeeObject_Check(fail_msg) && DeeXAst_Check(fail_msg)));
 // Compile as empty when assertions are disabled
 if ((parser_flags&DEE_PARSER_FLAG_NO_ASSERTIONS)!=0)
  return DeeSAst_NewEmpty(tk,labels_);
 // Optimize away none-assertion messages
 if (fail_msg && fail_msg->ast_kind == DEE_XASTKIND_CONST &&
    (parser_flags&DEE_PARSER_FLAG_OPTIMIZE_MERGE_OPERATORS)!=0 &&
     DeeNone_Check(fail_msg->ast_const.c_const)) fail_msg = NULL;
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0 &&
     check_expr->ast_kind == DEE_XASTKIND_CONST) {
  DeeObject *expr_copy; int expr_bool;
  if ((expr_copy = DeeObject_DeepCopy(check_expr->ast_const.c_const)) == NULL)
   DeeError_Handled(); // Ignore errors in copy
  else {
   expr_bool = DeeObject_Bool(expr_copy);
   Dee_DECREF(expr_copy);
   if (expr_bool < 0) {
    DeeTypeObject *rt_error; int temp;
    if ((rt_error = _DeeError_HandleAndReturnType()) != NULL) {
     temp = DeeXAst_ThrowInvalidUnaryOperatorError(
      DEE_XASTKIND_BOOL,tk,lexer,check_expr,rt_error);
     Dee_DECREF(rt_error);
     if (temp != 0) return NULL;
    }
    goto normal_ast;
   }
   if (expr_bool) return DeeSAst_NewEmpty(tk,labels_); // Assertion is true
   if ((fail_msg ? (fail_msg->ast_kind == DEE_XASTKIND_CONST
    ? DeeError_CompilerErrorf(DEE_WARNING_CONSTANT_ASSERTION_FAILED,
        (DeeObject *)lexer,(DeeObject *)check_expr->ast_common.ast_token,
        "Assertion failed : %r : %k",check_expr,fail_msg->ast_const.c_const)
    : DeeError_CompilerErrorf(DEE_WARNING_CONSTANT_ASSERTION_FAILED,
        (DeeObject *)lexer,(DeeObject *)check_expr->ast_common.ast_token,
        "Assertion failed : %r : %r",check_expr,fail_msg)
    ) : DeeError_CompilerErrorf(DEE_WARNING_CONSTANT_ASSERTION_FAILED,
    (DeeObject *)lexer,(DeeObject *)check_expr->ast_common.ast_token,
    "Assertion failed : %r",check_expr)
    ) != 0) return NULL;
  }
 }

normal_ast:
 if ((result = _DeeSAst_NewUnsafe(DEE_SASTKIND_ASSERT,tk,labels_)) != NULL) {
  Dee_INCREF(result->ast_assert.a_check = check_expr);
  Dee_XINCREF(result->ast_assert.a_message = fail_msg);
 }
 return result;
}



DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewPrint(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags, 
 DEE_A_IN Dee_uint32_t print_flags, DEE_A_INOUT_OPT DeeXAstObject *print_file,
 DEE_A_INOUT_OPT DeeXAstObject *print_data) {
 DeeSAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(!print_file || (DeeObject_Check(print_file) && DeeXAst_Check(print_file)));
 DEE_ASSERT(!print_data || (DeeObject_Check(print_data) && DeeXAst_Check(print_data)));
 (void)lexer,parser_flags; // TODO: optimize
 // TODO: Optimize: 'print 42;' -> 'print "42";'
 // TODO: Optimize: 'print "foo","bar";' -> 'print "foo bar";'
 // TODO: Optimize: 'print "foobar";' -> 'print "foobar\n",;'
 // TODO: Optimize: 'print "\n",;' -> 'print;'
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_PRINT_STDOUT) != 0 &&
     print_file && print_file->ast_kind == DEE_XASTKIND_ATTR_GET_C &&
     print_file->ast_attr_get_c.ac_object->ast_kind == DEE_XASTKIND_CONST &&
     print_file->ast_attr_get_c.ac_object->ast_const.c_const == (DeeObject *)&DeeFileIO_Type &&
     DeeString_EQUALS(print_file->ast_attr_get_c.ac_name,"stdout")) {
  DEE_LVERBOSE1R("%s(%d) : %k : Optimizing print to stdout\n",
                 DeeToken_FILE(tk),DeeToken_LINE(tk)+1,tk);
  print_file = NULL;
 }
 if ((result = _DeeSAst_NewUnsafe(DEE_SASTKIND_PRINT,tk,labels_)) != NULL) {
  Dee_XINCREF(result->ast_print.p_file = print_file);
  Dee_XINCREF(result->ast_print.p_data = print_data);
  result->ast_print.p_flags = print_flags;
 }
 return result;
}





DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewIf(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeScopeObject *cond_scope, DEE_A_INOUT DeeXAstObject *ast_cond,
 DEE_A_INOUT_OPT DeeScopeObject *succ_scope, DEE_A_INOUT_OPT DeeSAstObject *succ_block,
 DEE_A_INOUT_OPT DeeScopeObject *fail_scope, DEE_A_INOUT_OPT DeeSAstObject *fail_block) {
 DeeSAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(cond_scope) && DeeScope_Check(cond_scope));
 DEE_ASSERT(DeeObject_Check(ast_cond) && DeeXAst_Check(ast_cond));
 DEE_ASSERT(!succ_scope || (DeeObject_Check(succ_scope) && DeeScope_Check(succ_scope)));
 DEE_ASSERT(!succ_block || (DeeObject_Check(succ_block) && DeeSAst_Check(succ_block)));
 DEE_ASSERT(!fail_scope || (DeeObject_Check(fail_scope) && DeeScope_Check(fail_scope)));
 DEE_ASSERT(!fail_block || (DeeObject_Check(fail_block) && DeeSAst_Check(fail_block)));
 DEE_ASSERT((succ_scope != NULL) == (succ_block != NULL));
 DEE_ASSERT((fail_scope != NULL) == (fail_block != NULL));
 // Do some assertions on relations between the different scopes
 DEE_ASSERT(!succ_scope || ( DeeScope_IsWeakParent((DeeObject *)cond_scope,(DeeObject *)succ_scope)));
 DEE_ASSERT(!fail_scope || ( DeeScope_IsWeakParent((DeeObject *)cond_scope,(DeeObject *)fail_scope)));
 DEE_ASSERT(!succ_scope || !fail_scope || (!DeeScope_IsWeakParent((DeeObject *)succ_scope,(DeeObject *)fail_scope)));
 DEE_ASSERT(!fail_scope || !succ_scope || (!DeeScope_IsWeakParent((DeeObject *)fail_scope,(DeeObject *)succ_scope)));
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH)!=0 &&
     ast_cond->ast_kind == DEE_XASTKIND_CONST) {
  DeeObject *cond_copy; int choose_succ_branch;
  if ((cond_copy = DeeObject_DeepCopy(ast_cond->ast_const.c_const)) != NULL) {
   choose_succ_branch = DeeObject_Bool(cond_copy);
   Dee_DECREF(cond_copy);
   if (choose_succ_branch < 0) {
    DeeTypeObject *rt_error;
    if ((rt_error = _DeeError_HandleAndReturnType()) != NULL) {
     choose_succ_branch = DeeXAst_ThrowInvalidUnaryOperatorError(
      DEE_XASTKIND_BOOL,tk,lexer,ast_cond,rt_error);
     Dee_DECREF(rt_error);
     if (choose_succ_branch != 0) return NULL;
    }
   } else {
    DeeSAstObject *if_const_object;
#ifdef DEE_DEBUG
    if (choose_succ_branch) {
     DEE_LVERBOSE1R("%s(%d) : Removing dead fail branch in 'if (true) ...; else %r'\n",
                    DeeToken_FILE(tk),DeeToken_LINE(tk)+1,
                    fail_block ? (DeeObject *)fail_block : Dee_None);
    } else {
     DEE_LVERBOSE1R("%s(%d) : Removing dead succ branch in 'if (false) %r'\n",
                    DeeToken_FILE(tk),DeeToken_LINE(tk)+1,
                    succ_block ? (DeeObject *)succ_block : Dee_None);
    }
#endif
    if_const_object = DeeSAst_NewIfConst((DeeSAstKind)(
     choose_succ_branch ? DEE_SASTKIND_IFTRUE : DEE_SASTKIND_IFFALSE),
     tk,DeeParserLabelRefList_Empty,succ_scope,succ_block,fail_scope,fail_block);
    if (!if_const_object) return NULL;
    result = DeeSAst_NewBlock(tk,labels_,lexer,parser_flags,
                              cond_scope,1,&if_const_object);
    Dee_DECREF(if_const_object);
    return result;
   }
  } else DeeError_Handled(); // Ignore errors in copy
 }

 if ((result = _DeeSAst_NewUnsafe(DEE_SASTKIND_IF,tk,labels_)) != NULL) {
  Dee_INCREF(result->ast_if.if_cond_scope = cond_scope);
  Dee_INCREF(result->ast_if.if_cond = ast_cond);
  Dee_XINCREF(result->ast_if.if_succ_scope = succ_scope);
  Dee_XINCREF(result->ast_if.if_succ_block = succ_block);
  Dee_XINCREF(result->ast_if.if_fail_scope = fail_scope);
  Dee_XINCREF(result->ast_if.if_fail_block = fail_block);
 }
 return result;
}



DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewFor(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeScopeObject *head_init_scope, DEE_A_INOUT_OPT DeeXAstObject *head_init,
 DEE_A_INOUT DeeScopeObject *head_cond_scope, DEE_A_INOUT_OPT DeeXAstObject *head_cond,
 DEE_A_INOUT_OPT DeeXAstObject *head_next, DEE_A_INOUT DeeScopeObject *block_scope,
 DEE_A_INOUT DeeSAstObject *block) {
 DeeSAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(head_init_scope) && DeeScope_Check(head_init_scope));
 DEE_ASSERT(DeeObject_Check(head_cond_scope) && DeeScope_Check(head_cond_scope));
 DEE_ASSERT(!head_init || (DeeObject_Check(head_init) && DeeXAst_Check(head_init)));
 DEE_ASSERT(!head_cond || (DeeObject_Check(head_cond) && DeeXAst_Check(head_cond)));
 DEE_ASSERT(!head_next || (DeeObject_Check(head_next) && DeeXAst_Check(head_next)));
 DEE_ASSERT(DeeObject_Check(block_scope) && DeeScope_Check(block_scope));
 DEE_ASSERT(DeeObject_Check(block) && DeeSAst_Check(block));
 DEE_ASSERT(DeeScope_IsWeakParent((DeeObject *)head_init_scope,(DeeObject *)head_cond_scope));
 DEE_ASSERT(DeeScope_IsWeakParent((DeeObject *)head_cond_scope,(DeeObject *)block_scope));
 // These branches can't do anything when they're constant
 if (head_init && head_init->ast_kind == DEE_XASTKIND_CONST) head_init = NULL;
 if (head_next && head_next->ast_kind == DEE_XASTKIND_CONST) head_next = NULL;
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH)!=0 &&
      head_cond && head_cond->ast_kind == DEE_XASTKIND_CONST) {
  DeeObject *cond_copy; int temp;
  if ((cond_copy = DeeObject_DeepCopy(head_cond->ast_const.c_const)) == NULL)
   DeeError_Handled(); // Ignore errors during copy
  else {
   temp = DeeObject_Bool(cond_copy);
   Dee_DECREF(cond_copy);
   if (temp < 0) {
    DeeTypeObject *rt_error;
    if ((rt_error = _DeeError_HandleAndReturnType()) != NULL) {
     temp = DeeXAst_ThrowInvalidUnaryOperatorError(
      DEE_XASTKIND_BOOL,tk,lexer,head_cond,rt_error);
     Dee_DECREF(rt_error);
     if (temp != 0) return NULL;
    }
   } else if (temp) {
    head_cond = NULL; // forever loop
   } else {
    goto special_loopnone; // never loop
   }
  }
 }
 if (!head_cond) {
  // >> { // head_init_scope
  // >>   head_init;
  // >>   __loopever { // head_cond_scope
  // >>     { // block_scope
  // >>       block;
  // >>       head_next;
  // >>     }
  // >>   }
  // >> }
  DeeSAstObject *block_and_next,*blocks[2];
  DeeSAstKind special_loop_kind;
  special_loop_kind = DEE_SASTKIND_LOOPEVER;
  if (0) { special_loopnone: special_loop_kind = DEE_SASTKIND_LOOPNONE; }
  if (head_next) {
   blocks[0] = block;
   blocks[1] = DeeSAst_NewExpression(head_next->ast_common.ast_token,DeeParserLabelRefList_Empty,
                                     lexer,parser_flags,head_next);
   if (!blocks[1]) return NULL;
   block_and_next = DeeSAst_NewBlock(tk,DeeParserLabelRefList_Empty,lexer,parser_flags,
                                     block_scope,2,blocks);
   Dee_DECREF(blocks[1]);
  } else {
   block_and_next = DeeSAst_NewBlock(tk,DeeParserLabelRefList_Empty,lexer,parser_flags,
                                     block_scope,1,&block);
  }
  if (!block_and_next) return NULL;
  blocks[1] = DeeSAst_NewSpecialLoop(special_loop_kind,tk,DeeParserLabelRefList_Empty,
                                     head_cond_scope,block_and_next);
  Dee_DECREF(block_and_next);
  if (!blocks[1]) return NULL;
  if (head_init) {
   blocks[0] = DeeSAst_NewExpression(head_init->ast_common.ast_token,DeeParserLabelRefList_Empty,
                                     lexer,parser_flags,head_init);
   if (!blocks[0]) { Dee_DECREF(blocks[1]); return NULL; }
   result = DeeSAst_NewBlock(tk,labels_,lexer,parser_flags,
                             head_init_scope,2,blocks);
   Dee_DECREF(blocks[0]);
  } else {
   result = DeeSAst_NewBlock(tk,labels_,lexer,parser_flags,
                             head_init_scope,1,blocks+1);
  }
  Dee_DECREF(blocks[1]);
  return result;
 }

 // todo: Loop unwinding?
 if ((result = _DeeSAst_NewUnsafe(DEE_SASTKIND_FOR,tk,labels_)) != NULL) {
  Dee_INCREF(result->ast_for.f_head_init_scope = head_init_scope);
  Dee_XINCREF(result->ast_for.f_head_init = head_init);
  Dee_INCREF(result->ast_for.f_head_cond_scope = head_cond_scope);
  Dee_INCREF(result->ast_for.f_head_cond = head_cond);
  Dee_XINCREF(result->ast_for.f_head_next = head_next);
  Dee_INCREF(result->ast_for.f_scope = block_scope);
  Dee_INCREF(result->ast_for.f_block = block);
 }
 return result;
}


DEE_A_RET_EXCEPT_REF DeeSAstObject *_DeeSAst_NewForInOrForeach(
 DEE_A_IN DeeSAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *var,
 DEE_A_INOUT DeeScopeObject *seq_scope, DEE_A_INOUT DeeXAstObject *seq, 
 DEE_A_INOUT DeeScopeObject *block_scope, DEE_A_INOUT DeeSAstObject *block) {
 DeeSAstObject *result;
 DEE_ASSERT(kind == DEE_SASTKIND_FORIN || kind == DEE_SASTKIND_FOREACH);
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(var) && DeeXAst_Check(var));
 DEE_ASSERT(DeeObject_Check(seq_scope) && DeeScope_Check(seq_scope));
 DEE_ASSERT(DeeObject_Check(seq) && DeeXAst_Check(seq));
 DEE_ASSERT(DeeObject_Check(block_scope) && DeeScope_Check(block_scope));
 DEE_ASSERT(DeeObject_Check(block) && DeeSAst_Check(block));
 DEE_ASSERT(DeeScope_IsWeakParent((DeeObject *)seq_scope,(DeeObject *)block_scope));
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_LOOPS)!=0 && var->ast_kind == DEE_XASTKIND_VAR &&
     block->ast_kind == DEE_SASTKIND_YIELD &&
     DeeParserLabelRefList_EMPTY(&block->ast_common.ast_labels) &&
     block->ast_yield.y_value->ast_kind == DEE_XASTKIND_TUPLE &&
     block->ast_yield.y_value->ast_tuple.s_elemc == 1 &&
     block->ast_yield.y_value->ast_tuple.s_elemv[0]->ast_kind == DEE_XASTKIND_VAR &&
     block->ast_yield.y_value->ast_tuple.s_elemv[0]->ast_var.vs_var == var->ast_var.vs_var) {
  DeeXAstObject *expanded_seq,*expanded_tuple;
  // Optimize 'for (x: y) yield x;' --> 'yield y...;'
  DEE_LVERBOSE1R("%s(%d) : %k : OPTIMIZE : 'for (x: y) yield x;' --> 'yield y...;'\n",
                 DeeToken_FILE(tk),DeeToken_LINE(tk)+1,tk);
  if ((expanded_seq = DeeXAst_NewExpand(seq->ast_common.ast_token,lexer,parser_flags,seq)) == NULL) return NULL;
  expanded_tuple = DeeXAst_NewTuple(seq->ast_common.ast_token,lexer,parser_flags,1,&expanded_seq);
  Dee_DECREF(expanded_seq);
  if (!expanded_tuple) return NULL;
  result = DeeSAst_NewYield(block->ast_common.ast_token,labels_,expanded_tuple);
  Dee_DECREF(expanded_tuple);
  return result;
 }
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH)!=0 &&
     seq->ast_kind == DEE_XASTKIND_CONST) {
  DeeObject *seq_copy,*elem; int temp;
  if ((seq_copy = DeeObject_DeepCopy(seq->ast_const.c_const)) != NULL) {
   if (kind == DEE_SASTKIND_FORIN) {
    temp = DeeGeneric_IterableEmpty(seq_copy); // This one uses iterators, just like for-loops
   } else {
    temp = DeeObject_IterNextEx(seq_copy,&elem);
    if (temp == 0) Dee_DECREF(elem); // non-empty (temp == 0)
   }
   Dee_DECREF(seq_copy);
   if (temp < 0) {
    DeeTypeObject *rt_error;
    if ((rt_error = _DeeError_HandleAndReturnType()) != NULL) {
     temp = DeeXAst_ThrowInvalidUnaryOperatorError((DeeXAstKind)(
      seq->ast_kind == DEE_SASTKIND_FORIN ? DEE_XASTKIND_ITERSELF
      : DEE_XASTKIND_ITERNEXT),tk,lexer,seq,rt_error);
     Dee_DECREF(rt_error);
     if (temp != 0) return NULL;
    }
   } else if (temp) {
    DeeXAstObject *store_none,*store;
    DeeSAstObject *blocks[2],*result2;
    // Empty sequence --> the forin block is dead
    DEE_LVERBOSE1R("%s(%d) : Removing dead block branch in 'for (x:[]) %r'\n",
                   DeeToken_FILE(tk),DeeToken_LINE(tk)+1,block);
    // ugh! We must still store ~something~ in the variable to prevent
    //      misleading/wrong compiler warnings later down the line...
    if ((store_none = DeeXAst_NewConst(tk,Dee_None)) == NULL) return NULL;
    store = DeeXAst_NewStore(tk,lexer,parser_flags,var,store_none,block_scope);
    Dee_DECREF(store_none);
    if (!store) return NULL;
    blocks[0] = DeeSAst_NewExpression(seq->ast_common.ast_token,
                                      DeeParserLabelRefList_Empty,
                                      lexer,parser_flags,store);
    Dee_DECREF(store);
    blocks[1] = block;
    result = DeeSAst_NewBlock(tk,DeeParserLabelRefList_Empty,lexer,parser_flags,
                              block_scope,2,blocks);
    Dee_DECREF(blocks[0]);
    if (!result) return NULL;
    result2 = DeeSAst_NewLoopNone(tk,labels_,seq_scope,result);
    Dee_DECREF(result);
    return result2;
   }
  } else DeeError_Handled(); // Ignore errors in copy
 }
 // todo: Loop unwinding?
 if ((result = _DeeSAst_NewUnsafe(kind,tk,labels_)) != NULL) {
  // Remember how the loop variable lives in the block scope...
  if (DeeVarDeclStorage_InitFromXAst(&result->ast_forin.fi_var,lexer,parser_flags,var,block_scope) != 0) {
   _DeeSAst_FreeUnsafe(result);
   return NULL;
  }
  Dee_INCREF(result->ast_forin.fi_seq_scope = seq_scope);
  Dee_INCREF(result->ast_forin.fi_seq = seq);
  Dee_INCREF(result->ast_forin.fi_scope = block_scope);
  Dee_INCREF(result->ast_forin.fi_block = block);
 }
 return result;
}


DEE_A_RET_EXCEPT_REF DeeSAstObject *_DeeSAst_NewWhileOrDoWhile(
 DEE_A_IN DeeSAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeScopeObject *cond_scope,
 DEE_A_INOUT DeeXAstObject *cond, DEE_A_INOUT DeeScopeObject *block_scope,
 DEE_A_INOUT DeeSAstObject *block) {
 DeeSAstObject *result; DeeObject *cond_copy; int temp;
 DEE_ASSERT(kind == DEE_SASTKIND_WHILE || kind == DEE_SASTKIND_DOWHILE);
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(cond_scope) && DeeScope_Check(cond_scope));
 DEE_ASSERT(DeeObject_Check(cond) && DeeXAst_Check(cond));
 DEE_ASSERT(DeeObject_Check(block_scope) && DeeScope_Check(block_scope));
 DEE_ASSERT(DeeObject_Check(block) && DeeSAst_Check(block));
 DEE_ASSERT(DeeScope_IsWeakParent((DeeObject *)cond_scope,(DeeObject *)block_scope));
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH)!=0 &&
     cond->ast_kind == DEE_XASTKIND_CONST) {
  if ((cond_copy = DeeObject_DeepCopy(cond->ast_const.c_const)) != NULL) {
   temp = DeeObject_Bool(cond_copy);
   Dee_DECREF(cond_copy);
   if (temp < 0) {
    DeeTypeObject *rt_error;
    if ((rt_error = _DeeError_HandleAndReturnType()) != NULL) {
     temp = DeeXAst_ThrowInvalidUnaryOperatorError(
      DEE_XASTKIND_BOOL,tk,lexer,cond,rt_error);
     Dee_DECREF(rt_error);
     if (temp != 0) return NULL;
    }
   } else if (temp) {
    DeeSAstObject *loopever;
    // Infinite loop (represent as '__loopever { ... }')
    DEE_LVERBOSE1R("%s(%d) : Detected infinite loop in 'while (true) %r'\n",
                   DeeToken_FILE(tk),DeeToken_LINE(tk)+1,block);
    if ((loopever = DeeSAst_NewLoopEver(tk,labels_,block_scope,block)) == NULL) return NULL;
    result = DeeSAst_NewBlock(tk,labels_,lexer,parser_flags,
                              cond_scope,1,&loopever);
    Dee_DECREF(loopever);
    return result;
   } else if (kind == DEE_SASTKIND_WHILE) {
    DeeSAstObject *dead_block;
    // Never loop (represent as '__loopnone { ... }')
    DEE_LVERBOSE1R("%s(%d) : Removing dead block branch in 'while (false) %r'\n",
                   DeeToken_FILE(tk),DeeToken_LINE(tk)+1,block);
    if ((dead_block = DeeSAst_NewLoopNone(block->ast_common.ast_token,
     DeeParserLabelRefList_Empty,block_scope,block)) == NULL) return NULL;
    result = DeeSAst_NewBlock(tk,labels_,lexer,parser_flags,cond_scope,1,&dead_block);
    Dee_DECREF(dead_block);
    return result;
   } else {
    // Loop once (represent as '__looponce { ... }')
    DeeSAstObject *loop_once_ast; // Loop-once
    if ((loop_once_ast = DeeSAst_NewLoopOnce(tk,labels_,block_scope,block)) == NULL) return NULL;
    result = DeeSAst_NewBlock(tk,labels_,lexer,parser_flags,
                              cond_scope,1,&loop_once_ast);
    Dee_DECREF(loop_once_ast);
    return result;
   }
  } else DeeError_Handled(); // Ignore errors in copy
 }
 // todo: Loop unwinding?
 if ((result = _DeeSAst_NewUnsafe(kind,tk,labels_)) != NULL) {
  Dee_INCREF(result->ast_while.w_cond_scope = cond_scope);
  Dee_INCREF(result->ast_while.w_cond = cond);
  Dee_INCREF(result->ast_while.w_scope = block_scope);
  Dee_INCREF(result->ast_while.w_block = block);
 }
 return result;
}


DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewSpecialLoop(
 DEE_A_IN DeeSAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeScopeObject *block_scope, DEE_A_INOUT DeeSAstObject *block) {
 DeeSAstObject *result;
 DEE_ASSERT(kind == DEE_SASTKIND_LOOPEVER || kind == DEE_SASTKIND_LOOPONCE ||
            kind == DEE_SASTKIND_LOOPNONE);
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(block_scope) && DeeScope_Check(block_scope));
 DEE_ASSERT(DeeObject_Check(block) && DeeSAst_Check(block));
 // TODO: Warning is the block doesn't have any side-effects
 if ((result = _DeeSAst_NewUnsafe(kind,tk,labels_)) != NULL) {
  Dee_INCREF(result->ast_specialloop.sl_scope = block_scope);
  Dee_INCREF(result->ast_specialloop.sl_block = block);
 }
 return result;
}

DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewGoto(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_IN struct DeeParserLabel *dest_label) {
 DeeSAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(dest_label);
 if ((result = _DeeSAst_NewUnsafe(DEE_SASTKIND_GOTO,tk,labels_)) != NULL) {
  result->ast_goto.g_label = dest_label;
  DeeParserLabel_ADD_GOTO(dest_label); // Add a goto reference
 }
 return result;
}

DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewThrow(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT_OPT DeeXAstObject *ast_throw) {
 DeeSAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(!ast_throw || (DeeObject_Check(ast_throw) && DeeXAst_Check(ast_throw)));
 if ((result = _DeeSAst_NewUnsafe(DEE_SASTKIND_THROW,tk,labels_)) != NULL) {
  Dee_XINCREF(result->ast_throw.t_object = ast_throw);
 }
 return result;
}


DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewTry(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeScopeObject *rootscope, DEE_A_INOUT DeeScopeObject *scope,
 DEE_A_INOUT DeeSAstObject  *block) {
 DeeSAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(rootscope) && DeeScope_Check(rootscope));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 DEE_ASSERT(DeeObject_Check(block) && DeeSAst_Check(block));
 DEE_ASSERT(DeeScope_IsWeakParent((DeeObject *)rootscope,(DeeObject *)scope));
 if ((result = _DeeSAst_NewUnsafe(DEE_SASTKIND_TRY,tk,labels_)) != NULL) {
  Dee_INCREF(result->ast_try.t_rootscope = rootscope);
  Dee_INCREF(result->ast_try.t_scope = scope);
  Dee_INCREF(result->ast_try.t_block = block);
  result->ast_try.t_handlerc = 0;
  result->ast_try.t_handlerv = NULL;
 }
 return result;
}


static int DEE_CALL _DeeSAst_VisitReplaceThrowVarWithRethrow(
 DeeSAstObject *self, DeeLocalVarObject *var) {
 if (self->ast_kind == DEE_SASTKIND_THROW &&
     self->ast_throw.t_object &&
     self->ast_throw.t_object->ast_kind == DEE_XASTKIND_VAR &&
     self->ast_throw.t_object->ast_var.vs_var == var) {
  DEE_LVERBOSE1R("%s(%d) : %k : OPTIMIZE : throw --> rethrow\n",
                 DeeToken_FILE(self->ast_common.ast_token),
                 DeeToken_LINE(self->ast_common.ast_token)+1,
                 self->ast_common.ast_token);
  Dee_CLEAR(self->ast_throw.t_object);
  return 1;
 }
 return 0;
}

//////////////////////////////////////////////////////////////////////////
// Replaces all reachable instances of 'throw $throw_var;' with 'throw;'
// >> Used during optimization of catch blocks
#define _DeeSAst_ReplaceThrowVarWithRethrow(slef,throw_var)\
 DeeSAst_VisitAll((self),NULL,(DeeSAstVisitProc)&_DeeSAst_VisitReplaceThrowVarWithRethrow,(throw_var))

DEE_A_RET_EXCEPT(-1) int DeeSAst_AddTryHandler(
 DEE_A_INOUT DeeSAstObject *self, DEE_A_IN DeeExceptionHandlerKind kind,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT_OPT DeeXAstObject *exception_storage, DEE_A_INOUT_OPT DeeXAstObject *type_mask,
 DEE_A_INOUT DeeScopeObject *handler_scope, DEE_A_INOUT DeeSAstObject *handler_block) {
 Dee_size_t new_entryc; struct DeeSAstTryHandler *new_entryv;
 DEE_ASSERT(DeeObject_Check(self) && DeeSAst_Check(self));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(!exception_storage || (DeeObject_Check(exception_storage) && DeeXAst_Check(exception_storage)));
 DEE_ASSERT((kind&DeeExceptionHandleKind_FLAG_TYPED)==0 || type_mask);
 DEE_ASSERT(!type_mask || (DeeObject_Check(type_mask) && DeeXAst_Check(type_mask)));
 DEE_ASSERT(DeeObject_Check(handler_scope) && DeeScope_Check(handler_scope));
 DEE_ASSERT(DeeObject_Check(handler_block) && DeeSAst_Check(handler_block));
 DEE_ASSERT(DeeScope_IsWeakParent((DeeObject *)self->ast_try.t_rootscope,(DeeObject *)handler_scope));
 DEE_ASSERT(!DeeScope_IsWeakParent((DeeObject *)self->ast_try.t_scope,(DeeObject *)handler_scope));
 DEE_ASSERT(self->ast_kind == DEE_SASTKIND_TRY);
 DEE_ASSERT((exception_storage != NULL) == ((kind&DeeExceptionHandleKind_FLAG_VAR)!=0));
 if (self->ast_try.t_handlerc && self->ast_try.t_handlerv[
  self->ast_try.t_handlerc-1].th_kind == DeeExceptionHandleKind_FINALLY) {
  if (DeeError_CompilerError(DEE_WARNING_CATCH_AFTER_FINALLY,
   (DeeObject *)lexer,(DeeObject *)handler_block->ast_common.ast_token,
   "Did not expect 'catch' block after 'finally'") != 0) return -1;
 }
 if ((kind&DeeExceptionHandleKind_FLAG_TYPED) != 0 &&
     type_mask->ast_kind == DEE_XASTKIND_CONST &&
     (DeeTypeObject *)type_mask->ast_const.c_const == &DeeObject_Type) {
  // This is actually a catch-all
  kind &= ~DeeExceptionHandleKind_FLAG_TYPED;
  kind |= DeeExceptionHandleKind_FLAG_ALL;
  type_mask = NULL;
 }

 new_entryc = self->ast_try.t_handlerc+1;
 while ((new_entryv = (struct DeeSAstTryHandler *)realloc_nz(
  self->ast_try.t_handlerv,new_entryc*sizeof(struct DeeSAstTryHandler))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 self->ast_try.t_handlerc = new_entryc;
 self->ast_try.t_handlerv = new_entryv;
 new_entryv += (new_entryc-1);
 new_entryv->th_kind = kind;
 Dee_XINCREF(new_entryv->th_type = type_mask);
 Dee_INCREF(new_entryv->th_scope = handler_scope);
 Dee_INCREF(new_entryv->th_block = handler_block);
 if (!exception_storage) new_entryv->th_local.vds_kind = DEE_VARDECLSTORAGEKIND_NONE;
 else if (DeeVarDeclStorage_InitFromXAst(&new_entryv->th_local,lexer,parser_flags,exception_storage,handler_scope) != 0) {
  // psht. We don't attempt to realloc the handler list, but simply put it into a consistent state
  // -> This way we don't have to deal with the possibility of an out-of-memory error
  new_entryv->th_local.vds_kind = DEE_VARDECLSTORAGEKIND_NONE;
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeSAst_FinalizeTry(
 DEE_A_INOUT DeeSAstObject *self, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags) {
 struct DeeSAstTryHandler *iter,*end;
 DEE_ASSERT(DeeObject_Check(self) && DeeSAst_Check(self));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(self->ast_kind == DEE_SASTKIND_TRY);
 DEE_ASSERT((self->ast_try.t_handlerv == NULL) == (self->ast_try.t_handlerc == 0));
 if (!self->ast_try.t_handlerc) {
  if (DeeError_CompilerError(DEE_WARNING_EXPECTED_NO_TRY_HANDLERS,
   (DeeObject *)lexer,(DeeObject *)self->ast_common.ast_token,
   "try statement has no handlers") != 0) return -1;
  goto empty_try;
 }
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_EXCEPTIONS)!=0) {
  end = (iter = self->ast_try.t_handlerv)+self->ast_try.t_handlerc;
  while (iter != end) {
   // Optimize handlers:
   //    'try { ... } catch (e...) { ...; throw e; ...; }'
   // -> 'try { ... } catch (...) { ...; throw; ...; }'
   if ((iter->th_kind&DeeExceptionHandleKind_FLAG_VAR)!=0) {
    if (_DeeSAst_ReplaceThrowVarWithRethrow(iter->th_block,iter->th_local.vds_var.vs_var) < 0) return -1;
   }

   // Optimize handlers: 'try { ... } catch (...) { throw; }' -> 'try { ... }'
   // Don't optimize rethrow in finally blocks (they work differently)
   if (iter->th_kind != DeeExceptionHandleKind_FINALLY &&
       iter->th_block->ast_kind == DEE_SASTKIND_THROW &&
      !iter->th_block->ast_throw.t_object &&
       DeeParserLabelRefList_EMPTY(&iter->th_block->ast_common.ast_labels)) {
    // Single rethrow statement (remove this handler)
    _DeeSAstTryHandler_Quit(iter);
    memmove(iter,iter+1,((end-iter)-1)*sizeof(struct DeeSAstTryHandler));
    --end,--self->ast_try.t_handlerc;
   } else ++iter;
  }
 }

 if (!self->ast_try.t_handlerc) {
  DeeSAstObject *block_block,*root_block;
  // Without try handlers, create a block statement
empty_try:
  block_block = DeeSAst_NewBlock(self->ast_common.ast_token,DeeParserLabelRefList_Empty,
                                 lexer,parser_flags,self->ast_try.t_scope,
                                 1,&self->ast_try.t_block);
  if (!block_block) return -1;
  root_block = DeeSAst_NewBlock(self->ast_common.ast_token,DeeParserLabelRefList_Empty,
                                lexer,parser_flags,self->ast_try.t_rootscope,
                                1,&block_block);
  Dee_DECREF(block_block);
  if (!root_block) return -1;
  DeeSAst_AssignMove(self,root_block);
  Dee_DECREF(root_block);
  return 0;
 }

 return 0;
}



DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewModule(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeScopeObject *module_, DEE_A_INOUT DeeSAstObject *block) {
 DeeSAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(module_) && DeeScope_Check(module_));
 DEE_ASSERT(DeeObject_Check(block) && DeeSAst_Check(block));
 if ((result = _DeeSAst_NewUnsafe(DEE_SASTKIND_MODULE,tk,labels_)) != NULL) {
  Dee_INCREF(result->ast_module.m_module = module_);
  Dee_INCREF(result->ast_module.m_block = block);
 }
 return result;
}

DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewSwitch(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeScopeObject *expr_scope, DEE_A_INOUT DeeXAstObject *switch_expr,
 DEE_A_INOUT DeeScopeObject *scope, DEE_A_INOUT DeeSAstObject *block,
 DEE_A_INOUT struct DeeParserLabelList *case_labels) {
 DeeSAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(expr_scope) && DeeScope_Check(expr_scope));
 DEE_ASSERT(DeeObject_Check(switch_expr) && DeeXAst_Check(switch_expr));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 DEE_ASSERT(DeeObject_Check(block) && DeeSAst_Check(block));
 DEE_ASSERT(case_labels);
 (void)lexer,parser_flags; // TODO: Optimizations
 if (!case_labels->ll_front) {
  if (DeeError_CompilerError(DEE_WARNING_SWITCH_HAS_NO_CASES,
   (DeeObject *)lexer,(DeeObject *)tk,"switch statement has no cases") != 0) return NULL;
 } else if (case_labels->ll_front->pl_next == NULL &&
            case_labels->ll_front->pl_kind == DEE_PARSERLABEL_KIND_C_DEFAULT) {
  if (DeeError_CompilerError(DEE_WARNING_SWITCH_HAS_ONLY_DEFAULT_CASE,
   (DeeObject *)lexer,(DeeObject *)tk,"switch statement only has a default case") != 0) return NULL;
 }
 if ((result = _DeeSAst_NewUnsafe(DEE_SASTKIND_SWITCH,tk,labels_)) != NULL) {
  Dee_INCREF(result->ast_switch.sw_expr_scope = expr_scope);
  Dee_INCREF(result->ast_switch.sw_expr = switch_expr);
  Dee_INCREF(result->ast_switch.sw_scope = scope);
  Dee_INCREF(result->ast_switch.sw_block = block);
  _DeeParserLabelList_InitMove(&result->ast_switch.sw_cases,case_labels);
 }
 return result;
}

DEE_A_RET_EXCEPT_REF DeeSAstObject *DeeSAst_NewIfConst(
 DEE_A_IN DeeSAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT struct DeeParserLabelRefList *labels_,
 DEE_A_INOUT_OPT DeeScopeObject *succ_scope, DEE_A_INOUT_OPT DeeSAstObject *succ_block,
 DEE_A_INOUT_OPT DeeScopeObject *fail_scope, DEE_A_INOUT_OPT DeeSAstObject *fail_block) {
 DeeSAstObject *result;
 DEE_ASSERT(kind == DEE_SASTKIND_IFTRUE || kind == DEE_SASTKIND_IFFALSE);
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT((succ_block != NULL) == (succ_scope != NULL));
 DEE_ASSERT((fail_block != NULL) == (fail_scope != NULL));
 DEE_ASSERT(!succ_scope || (DeeObject_Check(succ_scope) && DeeScope_Check(succ_scope)));
 DEE_ASSERT(!succ_block || (DeeObject_Check(succ_block) && DeeSAst_Check(succ_block)));
 DEE_ASSERT(!fail_scope || (DeeObject_Check(fail_scope) && DeeScope_Check(fail_scope)));
 DEE_ASSERT(!fail_block || (DeeObject_Check(fail_block) && DeeSAst_Check(fail_block)));
 DEE_ASSERT(succ_block != fail_block || (!succ_block && !fail_block));
 if (!succ_block && !fail_block) return DeeSAst_NewEmpty(tk,labels_);
 if ((result = _DeeSAst_NewUnsafe(kind,tk,labels_)) != NULL) {
  Dee_XINCREF(result->ast_ifconst.ic_succ_block = succ_block);
  Dee_XINCREF(result->ast_ifconst.ic_succ_scope = succ_scope);
  Dee_XINCREF(result->ast_ifconst.ic_fail_block = fail_block);
  Dee_XINCREF(result->ast_ifconst.ic_fail_scope = fail_scope);
 }
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeSAst_MergeYield(
 DEE_A_INOUT DeeSAstObject *self, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeSAstObject *other_yield) {
 DeeXAstObject *common_yield,*expand_elem[2];
 DEE_ASSERT(DeeObject_Check(self) && DeeSAst_Check(self));
 DEE_ASSERT(DeeObject_Check(other_yield) && DeeSAst_Check(other_yield));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(self->ast_kind == DEE_SASTKIND_YIELD);
 DEE_ASSERT(other_yield->ast_kind == DEE_SASTKIND_YIELD);
 // Merge consecutive yield statements
 if ((expand_elem[0] = DeeXAst_NewExpand(self->ast_common.ast_token,
  lexer,parser_flags,self->ast_yield.y_value)) == NULL) return -1;
 if ((expand_elem[1] = DeeXAst_NewExpand(other_yield->ast_common.ast_token,
  lexer,parser_flags,other_yield->ast_yield.y_value)) == NULL) { Dee_DECREF(expand_elem[0]); return -1; }
 common_yield = DeeXAst_NewTuple(self->ast_common.ast_token,lexer,
                                 parser_flags,2,expand_elem);
 Dee_DECREF(expand_elem[1]);
 Dee_DECREF(expand_elem[0]);
 if (!common_yield) return -1;
 Dee_DECREF(self->ast_yield.y_value);
 Dee_INHERIT_REF(self->ast_yield.y_value,common_yield);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeSAst_MergePrint(
 DEE_A_INOUT DeeSAstObject *self, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeSAstObject *other_print) {
 //DeeXAstObject *common_yield,*expand_elem[2];
 DEE_ASSERT(DeeObject_Check(self) && DeeSAst_Check(self));
 DEE_ASSERT(DeeObject_Check(other_print) && DeeSAst_Check(other_print));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(self->ast_kind == DEE_SASTKIND_PRINT);
 DEE_ASSERT(other_print->ast_kind == DEE_SASTKIND_PRINT);
 // Merge consecutive print statements
 (void)self,lexer,parser_flags,other_print;
 // TODO
 DeeError_TODONotImplemented();
 return -1;
}



DEE_STATIC_INLINE(void) _DeeSAst_DestroyUncommon(DeeSAstObject *self) {
 switch (self->ast_kind) {
  case DEE_SASTKIND_EXPRESSION: _DeeSAstExpressionAst_Quit(&self->ast_expression); break;
  case DEE_SASTKIND_BLOCK:      _DeeSAstBlockAst_Quit(&self->ast_block); break;
  case DEE_SASTKIND_RETURN:     _DeeSAstReturnAst_Quit(&self->ast_return); break;
  case DEE_SASTKIND_YIELD:      _DeeSAstYieldAst_Quit(&self->ast_yield); break;
  case DEE_SASTKIND_ASSERT:     _DeeSAstAssertAst_Quit(&self->ast_assert); break;
  case DEE_SASTKIND_PRINT:      _DeeSAstPrintAst_Quit(&self->ast_print); break;
  case DEE_SASTKIND_IF:         _DeeSAstIfAst_Quit(&self->ast_if); break;
  case DEE_SASTKIND_FOR:        _DeeSAstForAst_Quit(&self->ast_for); break;
  case DEE_SASTKIND_FORIN:
  case DEE_SASTKIND_FOREACH:    _DeeSAstForInAst_Quit(&self->ast_forin); break;
  case DEE_SASTKIND_WHILE:
  case DEE_SASTKIND_DOWHILE:    _DeeSAstWhileAst_Quit(&self->ast_while); break;
  case DEE_SASTKIND_LOOPEVER:
  case DEE_SASTKIND_LOOPONCE:
  case DEE_SASTKIND_LOOPNONE:   _DeeSAstSpecialLoopAst_Quit(&self->ast_specialloop); break;
  case DEE_SASTKIND_GOTO:       _DeeSAstGotoAst_Quit(&self->ast_goto); break;
  case DEE_SASTKIND_THROW:      _DeeSAstThrowAst_Quit(&self->ast_throw); break;
  case DEE_SASTKIND_TRY:        _DeeSAstTryAst_Quit(&self->ast_try); break;
  case DEE_SASTKIND_MODULE:     _DeeSAstModuleAst_Quit(&self->ast_module); break;
  case DEE_SASTKIND_SWITCH:     _DeeSAstSwitchAst_Quit(&self->ast_switch); break;
  case DEE_SASTKIND_IFTRUE:
  case DEE_SASTKIND_IFFALSE:    _DeeSAstIfConstAst_Quit(&self->ast_ifconst); break;
  default: break;
 }
}

static void _deesast_tp_dtor(DeeSAstObject *self) {
 _DeeSAstCommonAst_Quit(&self->ast_common);
 _DeeSAst_DestroyUncommon(self);
}
DEE_VISIT_PROC(_deesast_tp_visit,DeeSAstObject *self) {
 _DeeSAstCommonAst_Visit(&self->ast_common);
 switch (self->ast_kind) {
  case DEE_SASTKIND_EXPRESSION: _DeeSAstExpressionAst_Visit(&self->ast_expression); break;
  case DEE_SASTKIND_BLOCK:      _DeeSAstBlockAst_Visit(&self->ast_block); break;
  case DEE_SASTKIND_RETURN:     _DeeSAstReturnAst_Visit(&self->ast_return); break;
  case DEE_SASTKIND_YIELD:      _DeeSAstYieldAst_Visit(&self->ast_yield); break;
  case DEE_SASTKIND_ASSERT:     _DeeSAstAssertAst_Visit(&self->ast_assert); break;
  case DEE_SASTKIND_PRINT:      _DeeSAstPrintAst_Visit(&self->ast_print); break;
  case DEE_SASTKIND_IF:         _DeeSAstIfAst_Visit(&self->ast_if); break;
  case DEE_SASTKIND_FOR:        _DeeSAstForAst_Visit(&self->ast_for); break;
  case DEE_SASTKIND_FORIN:
  case DEE_SASTKIND_FOREACH:    _DeeSAstForInAst_Visit(&self->ast_forin); break;
  case DEE_SASTKIND_WHILE:
  case DEE_SASTKIND_DOWHILE:    _DeeSAstWhileAst_Visit(&self->ast_while); break;
  case DEE_SASTKIND_LOOPEVER:
  case DEE_SASTKIND_LOOPONCE:
  case DEE_SASTKIND_LOOPNONE:   _DeeSAstSpecialLoopAst_Visit(&self->ast_specialloop); break;
  case DEE_SASTKIND_THROW:      _DeeSAstThrowAst_Visit(&self->ast_throw); break;
  case DEE_SASTKIND_TRY:        _DeeSAstTryAst_Visit(&self->ast_try); break;
  case DEE_SASTKIND_MODULE:     _DeeSAstModuleAst_Visit(&self->ast_module); break;
  case DEE_SASTKIND_SWITCH:     _DeeSAstSwitchAst_Visit(&self->ast_switch); break;
  case DEE_SASTKIND_IFTRUE:
  case DEE_SASTKIND_IFFALSE:    _DeeSAstIfConstAst_Visit(&self->ast_ifconst); break;
  default: break;
 }
}

// Implemented later
static DeeObject *_deesast_tp_repr(DeeSAstObject *self);

static DeeSAstObject *_deesast_tp_alloc(DeeTypeObject *tp_self) {
 DeeSAstObject *result = DEE_OBJECTPOOL_ALLOC(sast);
 if (result) DeeObject_INIT(result,tp_self);
 return result;
}
static void _deesast_tp_free(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSAstObject *ob) {
 DEE_OBJECTPOOL_FREE(sast,ob);
}

static DeeObject *_deesast_tp_cmp_eq(
 DeeSAstObject *self, DeeSAstObject *right) {
 int result;
 if DEE_UNLIKELY((right = (DeeSAstObject *)DeeObject_GetInstance((
  DeeObject *)right,&DeeSAst_Type)) == NULL) return NULL;
 if ((result = DeeSAst_Equals(self,right)) < 0) return NULL;
 DeeReturn_Bool(result);
}
static DeeObject *_deesast_tp_cmp_ne(
 DeeSAstObject *self, DeeSAstObject *right) {
 int result;
 if DEE_UNLIKELY((right = (DeeSAstObject *)DeeObject_GetInstance((
  DeeObject *)right,&DeeSAst_Type)) == NULL) return NULL;
 if ((result = DeeSAst_Equals(self,right)) < 0) return NULL;
 DeeReturn_Bool(!result);
}


static int _deesast_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeSAstObject *self, DeeSAstObject *right) {
 DeeSAst_InitMove(self,right);
 return 0;
}
static int _deesast_tp_move_assign(
 DeeSAstObject *self, DeeSAstObject *right) {
 DeeSAst_AssignMove(self,right);
 return 0;
}


static struct DeeMemberDef const _deesast_tp_class_members[] = {
/*[[[deemon
#include <file>
const prefix = "#define DEE_SASTKIND_";
const max_line_length = 100;
local line_length = 0;
for (local name: file.io("sast.h")) {
  name = name.strip();
  if (name.startswith(prefix)) {
    name = name[#prefix:].partition(" ")[0];
    if ("(" in name) continue;
    local line = "DEE_MEMBERDEF_CONST_v100("+repr("KIND_"+name)+",DeeSAstKind,DEE_SASTKIND_"+name+"),";
    if (!line_length) print " ",;
    print line,;
    line_length += #line;
    if (line_length >= max_line_length) {
      print "\n",;
      line_length = 0;
    }
  }
}
if (line_length) print;
]]]*/
 DEE_MEMBERDEF_CONST_v100("KIND_NONE",DeeSAstKind,DEE_SASTKIND_NONE),DEE_MEMBERDEF_CONST_v100("KIND_EMPTY",DeeSAstKind,DEE_SASTKIND_EMPTY),
 DEE_MEMBERDEF_CONST_v100("KIND_EXPRESSION",DeeSAstKind,DEE_SASTKIND_EXPRESSION),DEE_MEMBERDEF_CONST_v100("KIND_BLOCK",DeeSAstKind,DEE_SASTKIND_BLOCK),
 DEE_MEMBERDEF_CONST_v100("KIND_RETURN",DeeSAstKind,DEE_SASTKIND_RETURN),DEE_MEMBERDEF_CONST_v100("KIND_YIELD",DeeSAstKind,DEE_SASTKIND_YIELD),
 DEE_MEMBERDEF_CONST_v100("KIND_ASSERT",DeeSAstKind,DEE_SASTKIND_ASSERT),DEE_MEMBERDEF_CONST_v100("KIND_PRINT",DeeSAstKind,DEE_SASTKIND_PRINT),
 DEE_MEMBERDEF_CONST_v100("KIND_IF",DeeSAstKind,DEE_SASTKIND_IF),DEE_MEMBERDEF_CONST_v100("KIND_FOR",DeeSAstKind,DEE_SASTKIND_FOR),
 DEE_MEMBERDEF_CONST_v100("KIND_FORIN",DeeSAstKind,DEE_SASTKIND_FORIN),DEE_MEMBERDEF_CONST_v100("KIND_FOREACH",DeeSAstKind,DEE_SASTKIND_FOREACH),
 DEE_MEMBERDEF_CONST_v100("KIND_WHILE",DeeSAstKind,DEE_SASTKIND_WHILE),DEE_MEMBERDEF_CONST_v100("KIND_DOWHILE",DeeSAstKind,DEE_SASTKIND_DOWHILE),
 DEE_MEMBERDEF_CONST_v100("KIND_LOOPONCE",DeeSAstKind,DEE_SASTKIND_LOOPONCE),DEE_MEMBERDEF_CONST_v100("KIND_CONTINUE",DeeSAstKind,DEE_SASTKIND_CONTINUE),
 DEE_MEMBERDEF_CONST_v100("KIND_BREAK",DeeSAstKind,DEE_SASTKIND_BREAK),DEE_MEMBERDEF_CONST_v100("KIND_GOTO",DeeSAstKind,DEE_SASTKIND_GOTO),
 DEE_MEMBERDEF_CONST_v100("KIND_THROW",DeeSAstKind,DEE_SASTKIND_THROW),DEE_MEMBERDEF_CONST_v100("KIND_TRY",DeeSAstKind,DEE_SASTKIND_TRY),
 DEE_MEMBERDEF_CONST_v100("KIND_MODULE",DeeSAstKind,DEE_SASTKIND_MODULE),DEE_MEMBERDEF_CONST_v100("KIND_SWITCH",DeeSAstKind,DEE_SASTKIND_SWITCH),
 DEE_MEMBERDEF_CONST_v100("KIND_DEAD",DeeSAstKind,DEE_SASTKIND_IFFALSE),
//[[[end]]]
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_CONST_v100("__ASTPRINT_FLAG_NONE",Dee_uint32_t,DEE_SASTPRINT_FLAG_NONE),
 DEE_MEMBERDEF_CONST_v100("__ASTPRINT_FLAG_NOLF",Dee_uint32_t,DEE_SASTPRINT_FLAG_NOLF),
 DEE_MEMBERDEF_CONST_v100("__ASTPRINT_FLAG_NOSEP",Dee_uint32_t,DEE_SASTPRINT_FLAG_NOSEP),
 DEE_MEMBERDEF_CONST_v100("__ASTPRINT_FLAG_BUFFER",Dee_uint32_t,DEE_SASTPRINT_FLAG_BUFFER),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};

static struct DeeMemberDef const _deesast_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("kind",DeeSAstObject,ast_kind,DeeSAstKind),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_kind",DeeSAstObject,ast_kind,DeeSAstKind),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_common_ast_kind",DeeSAstObject,ast_common.ast_kind,DeeSAstKind),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_common_ast_token",DeeSAstObject,ast_common.ast_token,object,"-> token"),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_common_ast_labels_lrl_c",DeeSAstObject,ast_common.ast_labels.lrl_c,Dee_size_t),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_common_ast_labels_lrl_v",DeeSAstObject,ast_common.ast_labels.lrl_v,p2(void),"-> DeeParserLabel **"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_expression_x_ast",DeeSAstObject,ast_expression.x_ast,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_block_b_scope",DeeSAstObject,ast_block.b_scope,object,"-> scope"),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_block_b_astc",DeeSAstObject,ast_block.b_astc,Dee_size_t),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_block_b_astv",DeeSAstObject,ast_block.b_astv,p2(void),"-> DeeSAstObject *"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_return_r_value",DeeSAstObject,ast_return.r_value,object_null,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_yield_y_value",DeeSAstObject,ast_yield.y_value,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_assert_a_check",DeeSAstObject,ast_assert.a_check,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_assert_a_message",DeeSAstObject,ast_assert.a_message,object_null,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_print_p_file",DeeSAstObject,ast_print.p_file,object_null,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_print_p_data",DeeSAstObject,ast_print.p_data,object_null,"-> xast"),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_print_p_flags",DeeSAstObject,ast_print.p_flags,Dee_uint32_t),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_if_if_cond_scope",DeeSAstObject,ast_if.if_cond_scope,object,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_if_if_cond",DeeSAstObject,ast_if.if_cond,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_if_if_succ_scope",DeeSAstObject,ast_if.if_succ_scope,object_null,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_if_if_succ_block",DeeSAstObject,ast_if.if_succ_block,object_null,"-> sast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_if_if_fail_scope",DeeSAstObject,ast_if.if_fail_scope,object_null,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_if_if_fail_block",DeeSAstObject,ast_if.if_fail_block,object_null,"-> sast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_for_f_head_init_scope",DeeSAstObject,ast_for.f_head_init_scope,object,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_for_f_head_init",DeeSAstObject,ast_for.f_head_init,object_null,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_for_f_head_cond_scope",DeeSAstObject,ast_for.f_head_cond_scope,object,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_for_f_head_cond",DeeSAstObject,ast_for.f_head_cond,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_for_f_scope",DeeSAstObject,ast_for.f_scope,object,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_for_f_head_next",DeeSAstObject,ast_for.f_head_next,object_null,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_for_f_block",DeeSAstObject,ast_for.f_block,object,"-> sast"),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_forin_fi_var_vds_kind",DeeSAstObject,ast_forin.fi_var.vds_kind,Dee_uint32_t),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_forin_fi_var_vds_var_vs_token",DeeSAstObject,ast_forin.fi_var.vds_var.vs_token,object,"-> token"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_forin_fi_var_vds_var_vs_var",DeeSAstObject,ast_forin.fi_var.vds_var.vs_var,object,"-> local_var"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_forin_fi_var_vds_attr_as_object",DeeSAstObject,ast_forin.fi_var.vds_attr.as_object,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_forin_fi_var_vds_attr_as_attr",DeeSAstObject,ast_forin.fi_var.vds_attr.as_attr,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_forin_fi_var_vds_attr_c_acs_object",DeeSAstObject,ast_forin.fi_var.vds_attr_c.acs_object,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_forin_fi_var_vds_attr_c_acs_attr",DeeSAstObject,ast_forin.fi_var.vds_attr_c.acs_attr,object,"-> string"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_forin_fi_var_vds_item_is_object",DeeSAstObject,ast_forin.fi_var.vds_item.is_object,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_forin_fi_var_vds_item_is_key",DeeSAstObject,ast_forin.fi_var.vds_item.is_key,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_forin_fi_var_vds_range_rs_object",DeeSAstObject,ast_forin.fi_var.vds_range.rs_object,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_forin_fi_var_vds_range_rs_begin",DeeSAstObject,ast_forin.fi_var.vds_range.rs_begin,object_null,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_forin_fi_var_vds_range_rs_end",DeeSAstObject,ast_forin.fi_var.vds_range.rs_end,object_null,"-> xast"),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_forin_fi_var_vds_expand_es_declc",DeeSAstObject,ast_forin.fi_var.vds_expand.es_declc,Dee_size_t),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_forin_fi_var_vds_expand_es_declv",DeeSAstObject,ast_forin.fi_var.vds_expand.es_declv,p(void),"-> DeeVarDeclStorage *"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_forin_fi_var_vds_assign_as_ast",DeeSAstObject,ast_forin.fi_var.vds_assign.as_ast,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_forin_fi_seq_scope",DeeSAstObject,ast_forin.fi_seq_scope,object,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_forin_fi_seq",DeeSAstObject,ast_forin.fi_seq,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_forin_fi_scope",DeeSAstObject,ast_forin.fi_scope,object,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_forin_fi_block",DeeSAstObject,ast_forin.fi_scope,object,"-> sast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_while_w_cond_scope",DeeSAstObject,ast_while.w_cond_scope,object,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_while_w_cond",DeeSAstObject,ast_while.w_cond,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_while_w_scope",DeeSAstObject,ast_while.w_scope,object,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_while_w_block",DeeSAstObject,ast_while.w_scope,object,"-> sast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_specialloop_sl_scope",DeeSAstObject,ast_specialloop.sl_scope,object,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_specialloop_sl_block",DeeSAstObject,ast_specialloop.sl_block,object,"-> sast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_goto",DeeSAstObject,ast_goto.g_label,p(void),"-> DeeParserLabel *"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_throw_t_object",DeeSAstObject,ast_throw.t_object,object_null,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_try_t_rootscope",DeeSAstObject,ast_try.t_rootscope,object,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_try_t_scope",DeeSAstObject,ast_try.t_scope,object,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_try_t_block",DeeSAstObject,ast_try.t_block,object,"-> sast"),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_try_t_handlerc",DeeSAstObject,ast_try.t_handlerc,Dee_size_t),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_try_t_handlerv",DeeSAstObject,ast_try.t_handlerv,p(void),"-> DeeSAstTryHandler *"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_module_m_module",DeeSAstObject,ast_module.m_module,object,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_module_m_block",DeeSAstObject,ast_module.m_block,object,"-> sast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_switch_sw_expr_scope",DeeSAstObject,ast_switch.sw_expr_scope,object,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_switch_sw_expr",DeeSAstObject,ast_switch.sw_expr,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_switch_sw_scope",DeeSAstObject,ast_switch.sw_scope,object,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_switch_sw_block",DeeSAstObject,ast_switch.sw_block,object,"-> sast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_switch_sw_cases_ll_front",DeeSAstObject,ast_switch.sw_cases.ll_front,object,"-> DeeParserLabel *"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_ifconst_ic_succ_scope",DeeSAstObject,ast_ifconst.ic_succ_scope,object_null,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_ifconst_ic_succ_block",DeeSAstObject,ast_ifconst.ic_succ_block,object_null,"-> sast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_ifconst_ic_fail_scope",DeeSAstObject,ast_ifconst.ic_fail_scope,object_null,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_ifconst_ic_fail_block",DeeSAstObject,ast_ifconst.ic_fail_block,object_null,"-> sast"),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};



DeeTypeObject DeeSAst_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("sast"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeSAstObject),
  member(&_deesast_tp_alloc),null,null,
  member(&_deesast_tp_move_ctor),null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(
  member(&_deesast_tp_free),
  member(&_deesast_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,
  member(&_deesast_tp_move_assign),null),
 DEE_TYPE_OBJECT_CAST_v101(null,member(&_deesast_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,member(&_deesast_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,
  member(&_deesast_tp_cmp_eq),
  member(&_deesast_tp_cmp_ne),null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deesast_tp_members),null,null,
  member(_deesast_tp_class_members),null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DEE_DECL_END

#ifndef __INTELLISENSE__
#include "sast.assign.inl"
#include "sast.attributes.inl"
#include "sast.collect_top_scope_children.inl"
#include "sast.copy_with_scope.inl"
#include "sast.equals.inl"
#include "sast.predict_type.inl"
#include "sast.repr.inl"
#include "sast.visit_all.inl"
#include "sast_optimize.inl"
#define DO_COUNT
#include "sast.uses_variable.inl"
#include "sast.uses_variable.inl"
#endif

#endif /* !GUARD_DEEMON_SAST_C */
