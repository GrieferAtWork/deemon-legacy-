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
#ifndef GUARD_DEEMON_COMPILER_SAST_OPTIMIZE_INL
#define GUARD_DEEMON_COMPILER_SAST_OPTIMIZE_INL 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include <deemon/none.h>
#include <deemon/deemonrun.h>
#include <deemon/compiler/compiler.h>
#include <deemon/compiler/sast.h>
#include <deemon/compiler/xast.h>
#include <deemon/compiler/scope.h>
#include <deemon/compiler/lexer.h>

#define VLOG_OPTIMIZE(tk,...) (\
 DEE_LVERBOSE1R("%s(%d) : %k : OPTIMIZE : ",\
 DeeToken_FILE(tk),DeeToken_LINE(tk)+1,tk),\
 DEE_LVERBOSE1R(__VA_ARGS__))

DEE_DECL_BEGIN

Dee_size_t DeeScope_OptimizeRemoveUnusedNames(
 DEE_A_INOUT DeeScopeObject *self) {
 struct _DeeScopeEntry *iter,*end;
 Dee_size_t result = 0;
 DEE_ASSERT(DeeObject_Check(self) && DeeScope_Check(self));
 end = (iter = self->sc_namesv)+self->sc_namesc;
 while (iter != end) {
  DEE_ASSERT(DeeObject_Check(iter->e_const));
  DEE_ASSERT(!iter->e_depr || DeeObject_Check(iter->e_depr));
  if (!DeeLocalVar_Check(iter->e_local) ||
      !iter->e_local->lv_init &&
      !iter->e_local->lv_uses) {
   if (DeeLocalVar_Check(iter->e_local)) {
    --iter->e_local->lv_module_refs;
    DEE_ASSERTF(iter->e_name == iter->e_local->lv_name,
                "Inconsistent names: %u != %u (%r)",
                iter->e_name,iter->e_local->lv_name,iter->e_local);
    DEE_LVERBOSE2("Removing unused local variable %u from scope %p\n",
                  iter->e_name,self);
   } else {
    DEE_LVERBOSE2("Removing unused scope entry %u from scope %p: %q:%k\n",
                  iter->e_name,self,DeeType_NAME(Dee_TYPE(iter->e_const)),iter->e_const);
   }
   ++result;
   _DeeScopeEntry_Quit(iter);
   memmove(iter,iter+1,((Dee_size_t)((end-iter)-1))*sizeof(struct _DeeScopeEntry));
   if (!--self->sc_namesc) {
    // Conserve memory
    free_nn(self->sc_namesv);
    self->sc_namesv = NULL;
    self->sc_namesa = 0;
    break;
   }
   --end;
  } else ++iter;
 }
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeSAst_Optimize(
 DEE_A_INOUT DeeSAstObject *self, DEE_OPTIMIZE_PARAMS) {
 int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeSAst_Check(self));
 _DeeOptimizerAssumptions_EncounterSAst(assumptions,self,{ return -1 });
 DEE_ASSERT(assumptions->oas_scope == scope);
 if ((optimize_flags&(DEE_OPTIMIZE_FLAG_DEL_NOEFFECT|
  DEE_OPTIMIZE_FLAG_USED))==DEE_OPTIMIZE_FLAG_DEL_NOEFFECT) {
  struct DeeParserLabel **iter,**end,*elem;
  // Remove unreferenced labels
  end = (iter = self->ast_common.ast_labels.lrl_v
         )+self->ast_common.ast_labels.lrl_c;
  while (iter != end) {
   elem = *iter;
   if (!elem->pl_refs) {
    VLOG_OPTIMIZE(self->ast_common.ast_token,"Removing unreferenced label\n");
    memmove(iter,iter+1,((Dee_size_t)((end-iter)-1))*sizeof(struct DeeParserLabel *));
    if (!--self->ast_common.ast_labels.lrl_c) {
     free_nn(self->ast_common.ast_labels.lrl_v);
     self->ast_common.ast_labels.lrl_v = NULL;
     break;
    }
    --end;
    ++*performed_optimizations;
   } else ++iter;
  }
 }
 switch (self->ast_kind) {
  case DEE_SASTKIND_EXPRESSION: {
   if (DeeXAst_Optimize(self->ast_expression.x_ast,DEE_OPTIMIZE_ARGS) != 0) return -1;
   if ((optimize_flags&(DEE_OPTIMIZE_FLAG_DEL_NOEFFECT|
       DEE_OPTIMIZE_FLAG_USED))==DEE_OPTIMIZE_FLAG_DEL_NOEFFECT && (
       self->ast_expression.x_ast->ast_kind == DEE_XASTKIND_CONST ||
       DeeXAst_IsNoEffect(self->ast_expression.x_ast))) {
    // Optimize away single expressions without side-effects
    VLOG_OPTIMIZE(self->ast_expression.x_ast->ast_common.ast_token,
                  "Transforming expression without side-effects into empty statement\n");
    _DeeSAstExpressionAst_Quit(&self->ast_expression);
    self->ast_kind = DEE_SASTKIND_EMPTY;
    ++*performed_optimizations;
   }
  } break;
  case DEE_SASTKIND_BLOCK: {
   DeeSAstObject **iter,**end,**begin,**real_end,*elem;
   DeeScope_OPTIMIZE(self->ast_block.b_scope);
   DEE_ASSERT(self->ast_block.b_astc);
   end = (iter = self->ast_block.b_astv)+self->ast_block.b_astc;
   --end; // Only the last argument is used
   DEE_OPTIMIZE_SCOPE_ENTER(block_assumptions,self->ast_block.b_scope,assumptions) {
    while (iter != end) {
     if (DeeSAst_Optimize(*iter++,DEE_OPTIMIZE_ARGS_SCOPE_EX(
      self->ast_block.b_scope,&block_assumptions,optimize_flags&~(DEE_OPTIMIZE_FLAG_USED))) != 0) {
/*err_block_assumptions:*/ DEE_OPTIMIZE_SCOPE_BREAK(block_assumptions); return -1;
     }
    }
    if (DeeSAst_Optimize(*end,DEE_OPTIMIZE_ARGS_SCOPE(
     self->ast_block.b_scope,&block_assumptions)) != 0) return -1;
   } DEE_OPTIMIZE_SCOPE_LEAVE(block_assumptions);

   if ((optimize_flags&DEE_OPTIMIZE_FLAG_DEL_UNREACHABLE)!=0) {
    // TODO: The unreachable optimization can be added in a lot more
    //       places (everywhere, where two asts exist sequentially)
    int is_unreachable = 0;
    end = (iter = self->ast_block.b_astv)+self->ast_block.b_astc;
    while (iter != end) {
     elem = *iter;
     if (is_unreachable && !DeeParserLabelRefList_EMPTY(
      &elem->ast_common.ast_labels)) is_unreachable = 0; // Reachable by labels
     if (is_unreachable) {
      // Remove unreachable AST
      // NOTE: Since the ast has no labels, we can skip trying to move them forward
      VLOG_OPTIMIZE(elem->ast_common.ast_token,
                    "Removing unreachable statement from block\n");
      Dee_DECREF(elem);
      memmove(iter,iter+1,(Dee_size_t)((end-iter)-1)*sizeof(DeeSAstObject *));
      --end,--self->ast_block.b_astc;
      continue;
     } else {
      // NOTE: We need to include 'DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS',
      //       so we don't accidentally erase code that could under circumstances still be reachable...
      is_unreachable = DeeSAst_IsNoReturn(
       elem,DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK|
       DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS);
     }
     ++iter;
    }
   }

   if ((optimize_flags&DEE_OPTIMIZE_FLAG_DEL_NOEFFECT)!=0) {
    // Remove asts that don't have any effect
    real_end = iter = (begin = self->ast_block.b_astv)+self->ast_block.b_astc;
    // Note, that the last AST cannot be removed if the statement's value is used
    if ((optimize_flags&DEE_OPTIMIZE_FLAG_USED)!=0) --iter;
    while (iter != begin) {
     elem = *--iter;
     if (DeeSAst_IsNoEffect(elem)) {
      // NOTE: This happens so often, I decided to change the verbose output level to 2
      DEE_LVERBOSE2R("%s(%d) : %k : OPTIMIZE : Removing statement without side-effects from block\n",
                     DeeToken_FILE(elem->ast_common.ast_token),
                     DeeToken_LINE(elem->ast_common.ast_token)+1,
                     elem->ast_common.ast_token);
      // Remove this AST
      // Note though, that we have to transfer all of its labels to the next statement
      if (DeeParserLabelRefList_EMPTY(&elem->ast_common.ast_labels)) {
remove_ast_from_block:
       Dee_DECREF(elem);
       memmove(iter,iter+1,(Dee_size_t)((real_end-iter)-1)*sizeof(DeeSAstObject *));
       --real_end,--self->ast_block.b_astc;
      } else if (iter != real_end-1) {
       if (DeeSAst_TransferAllLabels(elem,iter[1]) != 0) return -1;
       goto remove_ast_from_block;
      } else DeeSAst_AssignEmpty(elem);
      ++*performed_optimizations;
     } else if (iter != begin) {
      DeeSAstObject *prev_elem = iter[-1];
#if 1
      if (elem->ast_kind == DEE_SASTKIND_YIELD &&
          prev_elem->ast_kind == DEE_SASTKIND_YIELD) {
       // Merge consecutive yield statements
       VLOG_OPTIMIZE(prev_elem->ast_common.ast_token,"Merging two consecutive yield statements\n");
       if (DeeSAst_MergeYield(prev_elem,lexer,config->parser_flags,elem) != 0) return -1;
       goto remove_ast_from_block;
      } else
#endif
#if 0 /* TODO */
      if (elem->ast_kind == DEE_SASTKIND_PRINT &&
          prev_elem->ast_kind == DEE_SASTKIND_PRINT) {
       // Merge consecutive yield statements
       VLOG_OPTIMIZE(prev_elem->ast_common.ast_token,"Merging two consecutive print statements\n");
       if (DeeSAst_MergePrint(prev_elem,lexer,config->parser_flags,elem) != 0) return -1;
       goto remove_ast_from_block;
      } else
#endif
      ;
      // TODO : Optimize : (Only allowed if the condition has no side-effects)
      // >> local x = 42;         // >> local x = 42;
      // >> if (x) { print "a"; } // >> if (x) { print "a";
      // >> if (x) { print "b"; } // >>          print "b"; }

      // TODO : Optimize : (Remove assignments that get overwritten directly afterwards)
      // >> local x = 10;         // >>
      // >> local x = 20;         // >> local x = 22;
     }
    }
    if (!self->ast_block.b_astc) {
     // None of the asts had any effect
     // >> Convert the block into an empty AST
     free_nn(self->ast_block.b_astv);
     Dee_DECREF(self->ast_block.b_scope);
     self->ast_kind = DEE_SASTKIND_EMPTY;
     ++*performed_optimizations;
    } else if (self->ast_block.b_astc == 1 && !self->ast_block.b_scope->sc_namesc) {
     // Remove unnecessary blocks
     DeeSAstObject *new_statement = self->ast_block.b_astv[0];
     struct DeeScopeList child_scopes = DeeScopeList_INIT();
     // Replace 'ob_weak_prev' in all sub-scopes of 'scope' with 'scope->ob_weak_prev'
     if (DeeSAst_CollectTopScopeChildren(new_statement,&child_scopes) != 0) { _DeeScopeList_Quit(&child_scopes); return -1; }
     if (DeeScopeList_SIZE(&child_scopes)) {
      DeeScopeObject **iter,**end,*elem;
      if (scope == self->ast_block.b_scope) { _DeeScopeList_Quit(&child_scopes); break; }
      // Replace 'ob_weak_prev' in all children with 'scope->ob_weak_prev'
      end = (iter = DeeScopeList_ELEM(&child_scopes))+DeeScopeList_SIZE(&child_scopes);
      while (iter != end) { elem = *iter++;
       if (elem->sc_weak_prev) {
        DEE_ASSERTF(elem->sc_weak_prev == self->ast_block.b_scope,
                    "Invalid parent or missing scope in %r",new_statement);
        elem->sc_weak_prev = scope;
       } else {
        // Can happen if a function is being declared in 'new_statement'
        DEE_ASSERTF(elem->sc_prev == self->ast_block.b_scope,
                    "Invalid parent or missing scope in %r",new_statement);
        elem->sc_prev = scope;
       }
      }
     }
     _DeeScopeList_Quit(&child_scopes);
     // Move labels from 'self' into 'new_statement'
     if (DeeParserLabelRefList_AppendMoveAll(
      &new_statement->ast_common.ast_labels,
      &self->ast_common.ast_labels) != 0) return -1;
     DeeSAst_AssignMove(self,new_statement);
     ++*performed_optimizations;
    } else if (!self->ast_block.b_scope->sc_namesc) {
     end = (iter = self->ast_block.b_astv)+self->ast_block.b_astc;
inline_block_next:
     while (iter != end) {
      elem = *iter;
      DEE_ASSERTF(elem->ast_kind != DEE_SASTKIND_BLOCK || (
       elem->ast_block.b_scope->sc_weak_prev == self->ast_block.b_scope),
       "Block statement scope has an unexpected parent");
      if (elem->ast_kind == DEE_SASTKIND_BLOCK &&
         !elem->ast_block.b_scope->sc_namesc) {
       ++*performed_optimizations;
       VLOG_OPTIMIZE(elem->ast_common.ast_token,"Inlining block-statement into parent block\n");
       // Optimize: '{ print "a"; { print "b"; print "c"; } print "d"; }' --> '{ print "a"; print "b"; print "c"; print "d"; }'
       if (!elem->ast_block.b_astc) {
        // Simple case: An empty block
        // NOTE: No, this wasn't necessarily detected by the 'DeeSAst_IsNoEffect'-optimization
        //       above. Simply because that one isn't allowed to remove the block's last
        //       element if the statement's value is used.
        memmove(iter,iter+1,((Dee_size_t)((end-iter)-1))*sizeof(DeeSAstObject *));
        --end,--self->ast_block.b_astc;
        goto inline_block_next;
       } else {
        DeeScopeObject **scopes_iter,**scopes_end,*scopes_elem;
        if (elem->ast_block.b_astc == 1) {
         struct DeeScopeList child_scopes = DeeScopeList_INIT();
         DeeSAstObject *block_elem;
         // Even easier case: The block only has one element
         Dee_INCREF(block_elem = elem->ast_block.b_astv[0]);
         // Replace 'ob_weak_prev' in all sub-scopes of 'scope' with 'scope->ob_weak_prev'
         if (DeeSAst_CollectTopScopeChildren(block_elem,&child_scopes) != 0) {
          _DeeScopeList_Quit(&child_scopes);
          Dee_DECREF(block_elem);
          return -1;
         }
         // Replace 'ob_weak_prev' in all children with 'scope->ob_weak_prev'
         scopes_end = (scopes_iter = DeeScopeList_ELEM(&child_scopes))+DeeScopeList_SIZE(&child_scopes);
         while (scopes_iter != scopes_end) {
          scopes_elem = *scopes_iter++;
          if (scopes_elem->sc_weak_prev) {
           DEE_ASSERTF(scopes_elem->sc_weak_prev == elem->ast_block.b_scope,
                       "Invalid parent or missing scope in %r",block_elem);
           scopes_elem->sc_weak_prev = self->ast_block.b_scope;
          } else {
           // Can happen if a function is being declared in 'block_elem'
           DEE_ASSERTF(scopes_elem->sc_prev == elem->ast_block.b_scope,
                       "Invalid parent or missing scope in %r",block_elem);
           scopes_elem->sc_prev = self->ast_block.b_scope;
          }
         }
         _DeeScopeList_Quit(&child_scopes);
         // Move labels from 'elem' to 'block_elem'
         if (DeeParserLabelRefList_AppendMoveAll(
          &block_elem->ast_common.ast_labels,
          &elem->ast_common.ast_labels) != 0) {
          Dee_DECREF(block_elem);
          return -1;
         }
         Dee_DECREF(elem);
         Dee_INHERIT_REF(*iter,block_elem);
         // fallthrough to the '++iter'
        } else {
         DeeSAstObject **new_elemv; Dee_size_t new_elemc; Dee_size_t old_iterpos;
         DeeSAstObject **fill_iter,**fill_end,**fill_src;
         new_elemc = (self->ast_block.b_astc-1)+elem->ast_block.b_astc;
         while ((new_elemv = (DeeSAstObject **)realloc_nz(
          self->ast_block.b_astv,new_elemc*sizeof(DeeSAstObject *))) == NULL) {
          if DEE_LIKELY(Dee_CollectMemory()) continue;
          DeeError_NoMemory();
          return -1;
         }
         old_iterpos = (Dee_size_t)(iter-self->ast_block.b_astv);
         self->ast_block.b_astv = new_elemv;
         iter = new_elemv+old_iterpos;
         end = new_elemv+new_elemc;
         {
          struct DeeScopeList child_scopes = DeeScopeList_INIT();
          fill_end = (fill_iter = elem->ast_block.b_astv)+elem->ast_block.b_astc;
          do if (DeeSAst_CollectTopScopeChildren(*fill_iter,&child_scopes) != 0) {
           _DeeScopeList_Quit(&child_scopes);
           return -1;
          } while (++fill_iter != fill_end);
          // Replace 'ob_weak_prev' in all children with 'scope->ob_weak_prev'
          scopes_end = (scopes_iter = DeeScopeList_ELEM(&child_scopes))+DeeScopeList_SIZE(&child_scopes);
          while (scopes_iter != scopes_end) {
           scopes_elem = *scopes_iter++;
           if (scopes_elem->sc_weak_prev) {
            DEE_ASSERTF(scopes_elem->sc_weak_prev == elem->ast_block.b_scope,
                        "Invalid parent or missing scope in %r",elem);
            scopes_elem->sc_weak_prev = self->ast_block.b_scope;
           } else {
            // Can happen if a function is being declared in 'block_elem'
            DEE_ASSERTF(scopes_elem->sc_prev == elem->ast_block.b_scope,
                        "Invalid parent or missing scope in %r",elem);
            scopes_elem->sc_prev = self->ast_block.b_scope;
           }
          }
          _DeeScopeList_Quit(&child_scopes);
         }
         self->ast_block.b_astc = new_elemc;
         DEE_ASSERT(*iter == elem);
         fill_end = (fill_iter = iter)+elem->ast_block.b_astc;
         fill_src = elem->ast_block.b_astv;
         // Move labels from 'elem' to '*fill_src'
         if (DeeParserLabelRefList_AppendMoveAll(
          &(*fill_src)->ast_common.ast_labels,
          &elem->ast_common.ast_labels) != 0) {
          return -1;
         }
         memmove(iter+elem->ast_block.b_astc,iter+1,
                ((Dee_size_t)(end-iter)-elem->ast_block.b_astc)*sizeof(DeeSAstObject *));
         while (fill_iter != fill_end) Dee_INCREF(*fill_iter++ = *fill_src++);
         Dee_DECREF(elem);
         goto inline_block_next;
        }
       }
      }
      ++iter;
     }
    }
   }
  } break;

  case DEE_SASTKIND_RETURN:
   if (!self->ast_return.r_value) return 0;
   if (DeeXAst_Optimize(self->ast_return.r_value,DEE_OPTIMIZE_ARGS_EX(
    optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_DEL_NOEFFECT)!=0 &&
       self->ast_return.r_value->ast_kind == DEE_XASTKIND_CONST &&
       DeeNone_Check(self->ast_return.r_value->ast_const.c_const)
       ) Dee_CLEAR(self->ast_return.r_value);
   break;

  case DEE_SASTKIND_YIELD:
   return DeeXAst_Optimize(self->ast_yield.y_value,DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED));

  case DEE_SASTKIND_ASSERT:
   if (DeeXAst_Optimize(self->ast_assert.a_check,DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
   if (self->ast_assert.a_message && DeeXAst_Optimize(self->ast_assert.a_message,DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
   break;

  case DEE_SASTKIND_PRINT:
   if (self->ast_print.p_file && DeeXAst_Optimize(self->ast_print.p_file,DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
   if (self->ast_print.p_data && DeeXAst_Optimize(self->ast_print.p_data,DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
   break;

  case DEE_SASTKIND_IF:
   DEE_ASSERT((self->ast_if.if_succ_block != NULL) == (self->ast_if.if_succ_scope != NULL));
   DEE_ASSERT((self->ast_if.if_fail_block != NULL) == (self->ast_if.if_fail_scope != NULL));
   DeeScope_OPTIMIZE(self->ast_if.if_cond_scope);
   DEE_OPTIMIZE_SCOPE_ENTER(cond_assumptions,self->ast_if.if_cond_scope,assumptions) {
    if (DeeXAst_Optimize(self->ast_if.if_cond,DEE_OPTIMIZE_ARGS_SCOPE_EX(
     self->ast_if.if_cond_scope,&cond_assumptions,optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) {
/*err_if_cond_assumptions:*/ DEE_OPTIMIZE_SCOPE_BREAK(cond_assumptions); return -1;
    }
    if (self->ast_if.if_succ_block) {
     DeeScope_OPTIMIZE(self->ast_if.if_succ_scope);
     DEE_OPTIMIZE_SCOPE_ENTER(succ_assumptions,self->ast_if.if_succ_scope,&cond_assumptions) {
      error = DeeSAst_Optimize(self->ast_if.if_succ_block,
                               DEE_OPTIMIZE_ARGS_SCOPE(self->ast_if.if_succ_scope,&succ_assumptions));
     } DEE_OPTIMIZE_SCOPE_LEAVE(succ_assumptions);
     if (error != 0) return -1;
    }
    if (self->ast_if.if_fail_block) {
     DeeScope_OPTIMIZE(self->ast_if.if_fail_scope);
     DEE_OPTIMIZE_SCOPE_ENTER(fail_assumptions,self->ast_if.if_fail_scope,&cond_assumptions) {
      error = DeeSAst_Optimize(self->ast_if.if_fail_block,
                               DEE_OPTIMIZE_ARGS_SCOPE(self->ast_if.if_fail_scope,&fail_assumptions));
     } DEE_OPTIMIZE_SCOPE_LEAVE(fail_assumptions);
     if (error != 0) return -1;
    }
   } DEE_OPTIMIZE_SCOPE_LEAVE(cond_assumptions);
   // TODO: Optimize: 'if (v) { x; } else { y; }' --> 'v ? ({ x; }) : ({ y; });'
   break;

  case DEE_SASTKIND_FOR: {
   DeeScope_OPTIMIZE(self->ast_for.f_head_init_scope);
   DEE_OPTIMIZE_SCOPE_ENTER(init_assumptions,self->ast_for.f_head_init_scope,assumptions) {
    if (self->ast_for.f_head_init && DeeXAst_Optimize(self->ast_for.f_head_init,DEE_OPTIMIZE_ARGS_SCOPE_EX(
     self->ast_for.f_head_init_scope,&init_assumptions,optimize_flags&~(DEE_OPTIMIZE_FLAG_USED))) != 0) {
err_for_init_assumptions: DEE_OPTIMIZE_SCOPE_BREAK(init_assumptions); return -1;
    }
    DeeScope_OPTIMIZE(self->ast_for.f_head_cond_scope);
    DEE_OPTIMIZE_SCOPE_ENTER(cond_assumptions,self->ast_for.f_head_cond_scope,&init_assumptions) {
     DEE_ASSERT(DeeObject_Check(self->ast_for.f_head_cond) &&
                DeeXAst_Check(self->ast_for.f_head_cond));
     if (DeeXAst_Optimize(self->ast_for.f_head_cond,DEE_OPTIMIZE_ARGS_SCOPE_EX(
      self->ast_for.f_head_cond_scope,&cond_assumptions,optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) {
err_for_cond_assumptions: DEE_OPTIMIZE_SCOPE_BREAK(cond_assumptions); goto err_for_init_assumptions;
     }
     DeeScope_OPTIMIZE(self->ast_for.f_scope);
     DEE_OPTIMIZE_SCOPE_ENTER(block_assumptions,self->ast_for.f_scope,&cond_assumptions) {
      if (self->ast_for.f_head_next && DeeXAst_Optimize(self->ast_for.f_head_next,DEE_OPTIMIZE_ARGS_SCOPE_EX(
       self->ast_for.f_scope,&block_assumptions,optimize_flags&~(DEE_OPTIMIZE_FLAG_USED))) != 0) {
/*err_for_block_assumptions:*/ DEE_OPTIMIZE_SCOPE_BREAK(block_assumptions); goto err_for_cond_assumptions;
      }
      error = DeeSAst_Optimize(self->ast_for.f_block,DEE_OPTIMIZE_ARGS_SCOPE_EX(
       self->ast_for.f_scope,&block_assumptions,optimize_flags&~(DEE_OPTIMIZE_FLAG_USED)));
     } DEE_OPTIMIZE_SCOPE_LEAVE(block_assumptions);
    } DEE_OPTIMIZE_SCOPE_LEAVE(cond_assumptions);
   } DEE_OPTIMIZE_SCOPE_LEAVE(init_assumptions);
   return error;
  } break;

  case DEE_SASTKIND_FORIN:
  case DEE_SASTKIND_FOREACH: {
   DeeScope_OPTIMIZE(self->ast_foreach.fi_seq_scope);
   DEE_OPTIMIZE_SCOPE_ENTER(seq_assumptions,self->ast_foreach.fi_seq_scope,assumptions) {
    if (DeeXAst_Optimize(self->ast_foreach.fi_seq,DEE_OPTIMIZE_ARGS_SCOPE_EX(
     self->ast_foreach.fi_seq_scope,&seq_assumptions,optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) {
err_foreach_seq_assumptions: DEE_OPTIMIZE_SCOPE_BREAK(seq_assumptions); return -1;
    }
    DeeScope_OPTIMIZE(self->ast_foreach.fi_scope);
    DEE_OPTIMIZE_SCOPE_ENTER(block_assumptions,self->ast_foreach.fi_scope,&seq_assumptions) {
     if (DeeVarDeclStorage_Optimize(&self->ast_foreach.fi_var,
      DEE_VARDECLSTORAGE_OPTIMIZE_FLAG_OPTIONAL, // Should never make a difference... (though technically not correct)
      DEE_OPTIMIZE_ARGS_SCOPE_EX(self->ast_foreach.fi_scope,&block_assumptions,optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) {
err_foreach_block_assumptions: DEE_OPTIMIZE_SCOPE_BREAK(block_assumptions); goto err_foreach_seq_assumptions;
     }
     if (DeeSAst_Optimize(self->ast_foreach.fi_block,DEE_OPTIMIZE_ARGS_SCOPE_EX(
      self->ast_foreach.fi_scope,&block_assumptions,optimize_flags
      &~(DEE_OPTIMIZE_FLAG_USED))) != 0) goto err_foreach_block_assumptions;
    } DEE_OPTIMIZE_SCOPE_LEAVE(block_assumptions);
   } DEE_OPTIMIZE_SCOPE_LEAVE(seq_assumptions);
   if (self->ast_kind == DEE_SASTKIND_FORIN &&
       self->ast_forin.fi_var.vds_kind == DEE_VARDECLSTORAGEKIND_NONE &&
       self->ast_forin.fi_block->ast_kind == DEE_SASTKIND_EMPTY) {
    DeeXAstObject *expanded_seq;
    DeeSAstObject *expanded_seq_expr,*expanded_seq_block;
    // Optimize: 'for (none: seq());' --> 'seq()...;'
    VLOG_OPTIMIZE(self->ast_common.ast_token,
                  "Optimizing unused iteration over sequence to expand\n");
    expanded_seq = DeeXAst_NewExpand(self->ast_common.ast_token,lexer,
                                     config->parser_flags,self->ast_forin.fi_seq);
    if (!expanded_seq) return -1;
    expanded_seq_expr = DeeSAst_NewExpression(self->ast_common.ast_token,DeeParserLabelRefList_Empty,
                                              lexer,config->parser_flags,expanded_seq);
    Dee_DECREF(expanded_seq);
    if (!expanded_seq_expr) return -1;
    expanded_seq_block = DeeSAst_NewBlock(self->ast_common.ast_token,&self->ast_common.ast_labels,
                                          lexer,config->parser_flags,self->ast_forin.fi_seq_scope,
                                          1,&expanded_seq_expr);
    Dee_DECREF(expanded_seq_expr);
    if (!expanded_seq_block) return -1;
    DeeSAst_AssignMove(self,expanded_seq_block);
    Dee_DECREF(expanded_seq_block);
    ++*performed_optimizations;
   }
  } break;

  case DEE_SASTKIND_WHILE:
  case DEE_SASTKIND_DOWHILE:
   DeeScope_OPTIMIZE(self->ast_while.w_cond_scope);
   DEE_OPTIMIZE_SCOPE_ENTER(cond_assumptions,self->ast_while.w_cond_scope,assumptions) {
    if (DeeXAst_Optimize(self->ast_while.w_cond,DEE_OPTIMIZE_ARGS_SCOPE_EX(
     self->ast_while.w_cond_scope,&cond_assumptions,optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) {
/*err_while_cond_assumptions:*/ DEE_OPTIMIZE_SCOPE_BREAK(cond_assumptions); return -1;
    }
    DeeScope_OPTIMIZE(self->ast_while.w_scope);
    DEE_OPTIMIZE_SCOPE_ENTER(block_assumptions,self->ast_while.w_scope,&cond_assumptions) {
     error = DeeSAst_Optimize(self->ast_while.w_block,DEE_OPTIMIZE_ARGS_SCOPE_EX(
      self->ast_while.w_scope,&block_assumptions,optimize_flags&~(DEE_OPTIMIZE_FLAG_USED)));
    } DEE_OPTIMIZE_SCOPE_LEAVE(block_assumptions);
   } DEE_OPTIMIZE_SCOPE_LEAVE(cond_assumptions);
   return error;

  // TODO: v special optimizations for these
  case DEE_SASTKIND_LOOPEVER:
  case DEE_SASTKIND_LOOPONCE:
   DeeScope_OPTIMIZE(self->ast_specialloop.sl_scope);
   DEE_OPTIMIZE_SCOPE_ENTER(block_assumptions,self->ast_specialloop.sl_scope,assumptions) {
    error = DeeSAst_Optimize(self->ast_specialloop.sl_block,DEE_OPTIMIZE_ARGS_SCOPE(
     self->ast_specialloop.sl_scope,&block_assumptions));
   } DEE_OPTIMIZE_SCOPE_LEAVE(block_assumptions);
   if (error != 0) return error;
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_DEL_NOEFFECT)!=0 &&
       self->ast_kind == DEE_SASTKIND_LOOPONCE &&
       !DeeSAst_HasBreakOrContinue(self->ast_loopnone.sl_block)) {
    // Optimize: '__looponce { foo(); }' --> '{ foo(); }'
    // NOTE: Only if there are no break/continue statements
    DeeSAstObject *loop_block;
    VLOG_OPTIMIZE(self->ast_common.ast_token,
                  "Removing __looponce without any break/continue statements\n");
    loop_block = DeeSAst_NewBlock(self->ast_common.ast_token,&self->ast_common.ast_labels,
                                  lexer,config->parser_flags,self->ast_looponce.sl_scope,
                                  1,&self->ast_looponce.sl_block);
    if (!loop_block) return -1;
    DeeSAst_AssignMove(self,loop_block);
    Dee_DECREF(loop_block);
    ++*performed_optimizations;
   }
   break;

  case DEE_SASTKIND_LOOPNONE:
   DeeScope_OPTIMIZE(self->ast_loopnone.sl_scope);
   DEE_OPTIMIZE_SCOPE_ENTER(block_assumptions,self->ast_loopnone.sl_scope,assumptions) {
    error = DeeSAst_Optimize(self->ast_loopnone.sl_block,
     DEE_OPTIMIZE_ARGS_SCOPE(self->ast_loopnone.sl_scope,&block_assumptions));
   } DEE_OPTIMIZE_SCOPE_LEAVE(block_assumptions);
   if (error != 0) return error;
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_DEL_UNREACHABLE)!=0 &&
       !DeeSAst_HasLabels(self->ast_loopnone.sl_block)) {
    VLOG_OPTIMIZE(self->ast_loopnone.sl_block->ast_common.ast_token,
                  "Removing unreachable dead branch in __loopnone\n");
    DeeSAst_AssignEmpty(self);
    ++*performed_optimizations;
   }
   break;


  case DEE_SASTKIND_THROW:
   if (!self->ast_throw.t_object) return 0;
   return DeeXAst_Optimize(self->ast_throw.t_object,
                           DEE_OPTIMIZE_ARGS_EX(optimize_flags|DEE_OPTIMIZE_FLAG_USED));

  case DEE_SASTKIND_TRY: {
   struct DeeSAstTryHandler *iter,*end;
   DeeScope_OPTIMIZE(self->ast_try.t_scope);
   DEE_OPTIMIZE_SCOPE_ENTER(root_assumptions,self->ast_try.t_rootscope,assumptions) {
    DEE_OPTIMIZE_SCOPE_ENTER(block_assumptions,self->ast_try.t_scope,&root_assumptions) {
     if (DeeSAst_Optimize(self->ast_try.t_block,DEE_OPTIMIZE_ARGS_SCOPE_EX(
      self->ast_try.t_scope,&block_assumptions,optimize_flags&~(DEE_OPTIMIZE_FLAG_USED))) != 0) {
      DEE_OPTIMIZE_SCOPE_BREAK(block_assumptions);
err_try_root_assumptions: DEE_OPTIMIZE_SCOPE_BREAK(root_assumptions); return -1;
     }
    } DEE_OPTIMIZE_SCOPE_LEAVE(block_assumptions);
    DEE_ASSERT(self->ast_try.t_handlerc != 0);
    end = (iter = self->ast_try.t_handlerv)+self->ast_try.t_handlerc;
    do {
     DeeScope_OPTIMIZE(iter->th_scope);
     DEE_OPTIMIZE_SCOPE_ENTER(handler_assumptions,iter->th_scope,&root_assumptions) {
      if (DeeSAst_Optimize(iter->th_block,DEE_OPTIMIZE_ARGS_SCOPE_EX(
       iter->th_scope,&handler_assumptions,optimize_flags&~(DEE_OPTIMIZE_FLAG_USED))) != 0) {
/*err_try_handler_assumptions:*/ DEE_OPTIMIZE_SCOPE_BREAK(handler_assumptions); goto err_try_root_assumptions;
      }
      if ((iter->th_kind&DeeExceptionHandleKind_FLAG_VAR)!=0) {
       if (DeeVarDeclStorage_Optimize(&iter->th_local,
        // The value is also stored in the list of active exceptions.
        // So no need to also keep is alive through unused variables
        DEE_VARDECLSTORAGE_OPTIMIZE_FLAG_OPTIONAL,DEE_OPTIMIZE_ARGS_SCOPE_EX(
        iter->th_scope,&handler_assumptions,optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) return -1;
       // Apply the variable storage flags
       if (iter->th_local.vds_kind == DEE_VARDECLSTORAGEKIND_NONE)
        iter->th_kind &= ~DeeExceptionHandleKind_FLAG_VAR;
      }
     } DEE_OPTIMIZE_SCOPE_LEAVE(handler_assumptions);
    } while (++iter != end);
   } DEE_OPTIMIZE_SCOPE_LEAVE(root_assumptions);
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_DEL_NOEFFECT)!=0 &&
       DeeSAst_IsReallyNoExcept(self->ast_try.t_block)) {
    DeeAstAttribute attr = DeeSAst_Attr(self->ast_try.t_block,DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS);
    // Optimize: 'try { noexcept_expr; } catch (foo) { bar(); }' --> 'noexcept_expr;'
    // Optimize: 'try { noexcept_expr; } finally { bar(); }' --> '{ noexcept_expr; bar(); }'
    DeeSAstObject *root_block,**blockv,**block_iter;
    Dee_size_t finally_handler_c;
    if ((attr&DEE_AST_ATTRIBUTE_NOEXCEPT)==0) break;
    // - Remove the try statement if the try-block is noexcept
    // - Only keep finally handlers (as those would have still been executed)
    finally_handler_c = 0;
    DEE_ASSERT(self->ast_try.t_handlerc != 0);
    end = (iter = self->ast_try.t_handlerv)+self->ast_try.t_handlerc;
    do if (iter->th_kind == DeeExceptionHandleKind_FINALLY) ++finally_handler_c;
    while (++iter != end);
    if ((attr&DEE_AST_ATTRIBUTE_NOJUMP)==0 && finally_handler_c) {
     // The block is noexcept, but may not always return
     // >> Something like: 'try { yield "foo"; } ...' or 'try { if (x) return "foo"; } ...'
     // In these cases, we can only remove catch handlers, but 'finally' blocks must stay
     // NOTE: If no finally blocks exist, this special handling isn't required and we can inline the try-block
     iter = self->ast_try.t_handlerv;
     do {
      if (iter->th_kind != DeeExceptionHandleKind_FINALLY) {
       VLOG_OPTIMIZE(iter->th_block->ast_common.ast_token,
                     "Removing catch handler from noexcept, sometimes-return try block");
       _DeeSAstTryHandler_Quit(iter);
       memmove(iter,iter+1,((Dee_size_t)((end-iter)-1))*sizeof(struct DeeSAstTryHandler));
       --end,--self->ast_try.t_handlerc;
       ++*performed_optimizations;
      } else {
       ++iter;
      }
     } while (iter != end);
     DEE_ASSERT(finally_handler_c);
     DEE_ASSERTF(self->ast_try.t_handlerc == finally_handler_c,
                 "Incorrect amount of handlers after removed all non-finally ones");
#if 0
     if (!self->ast_try.t_handlerc) {
      DeeSAstObject *try_block,*root_block;
      // Without any handlers, we need to turn this into a normal block
      // NOTE: This is not something optional:
      //       The specification for 't_handlerv' requires at least one handler.
      DEE_ASSERT(self->ast_try.t_handlerv != NULL);
      free_nn(self->ast_try.t_handlerv);
      try_block = DeeSAst_NewBlock(self->ast_try.t_block->ast_common.ast_token,
                                   DeeParserLabelRefList_Empty,lexer,config->parser_flags,
                                   self->ast_try.t_scope,1,&self->ast_try.t_block);
      Dee_DECREF(self->ast_try.t_block);
      Dee_DECREF(self->ast_try.t_scope);
      if (!try_block) {
       Dee_DECREF(self->ast_try.t_rootscope);
       self->ast_kind = DEE_SASTKIND_EMPTY;
       return -1;
      }
      root_block = DeeSAst_NewBlock(self->ast_common.ast_token,&self->ast_common.ast_labels,
                                    lexer,config->parser_flags,self->ast_try.t_rootscope,
                                    1,&try_block);
      Dee_DECREF(try_block);
      Dee_DECREF(self->ast_try.t_rootscope);
      self->ast_kind = DEE_SASTKIND_EMPTY;
      if (!root_block) return -1;
      DeeSAst_AssignMove(self,root_block);
      Dee_DECREF(root_block);
     }
#endif
    } else {
     while ((blockv = (DeeSAstObject **)malloc_nz(
      (1+self->ast_try.t_handlerc)*sizeof(DeeSAstObject *))) == NULL) {
      if DEE_LIKELY(Dee_CollectMemory()) continue;
      DeeError_NoMemory();
      return -1;
     }
     // Pack the try block and all finally-handlers into a linear block:
     // >> try {       // >> {
     // >>   foo;      // >>   { foo; }
     // >> } finally { // >>   { bar; }
     // >>   bar;      // >> }
     // >> }           //
     block_iter = blockv;
     *block_iter = DeeSAst_NewBlock(self->ast_try.t_block->ast_common.ast_token,
                                    DeeParserLabelRefList_Empty,lexer,config->parser_flags,
                                    self->ast_try.t_scope,1,&self->ast_try.t_block);
     if (!*block_iter) {
err_blockiter:
      while (block_iter != blockv) Dee_DECREF(*--block_iter);
      free_nn(blockv);
      return -1;
     }
     ++block_iter;
     DEE_ASSERT(self->ast_try.t_handlerc != 0);
     end = (iter = self->ast_try.t_handlerv)+self->ast_try.t_handlerc;
     do {
      if (iter->th_kind == DeeExceptionHandleKind_FINALLY) {
       *block_iter = DeeSAst_NewBlock(iter->th_block->ast_common.ast_token,
                                      DeeParserLabelRefList_Empty,lexer,config->parser_flags,
                                      iter->th_scope,1,&iter->th_block);
       if (!*block_iter) goto err_blockiter;
       ++block_iter;
      }
     } while (++iter != end);
     DEE_ASSERT(block_iter == blockv+(finally_handler_c+1));
     root_block = DeeSAst_NewBlockFromInheritedStatements(self->ast_common.ast_token,&self->ast_common.ast_labels,
                                                          lexer,config->parser_flags,self->ast_try.t_rootscope,
                                                          finally_handler_c+1,blockv);
     if (!root_block) goto err_blockiter;
     DeeSAst_AssignMove(self,root_block);
     Dee_DECREF(root_block);
     ++*performed_optimizations;
    }
   }
  } break;

  case DEE_SASTKIND_MODULE: {
   DeeScope_OPTIMIZE(self->ast_module.m_module);
   DEE_OPTIMIZE_SCOPE_ENTER(block_assumptions,self->ast_module.m_module,assumptions) {
    error = DeeSAst_Optimize(self->ast_module.m_block,DEE_OPTIMIZE_ARGS_SCOPE(
     self->ast_module.m_module,&block_assumptions));
   } DEE_OPTIMIZE_SCOPE_LEAVE(block_assumptions);
   return error;
  } break;

  case DEE_SASTKIND_SWITCH: {
   DeeScope_OPTIMIZE(self->ast_switch.sw_expr_scope);
   DEE_OPTIMIZE_SCOPE_ENTER(expr_assumptions,self->ast_switch.sw_expr_scope,assumptions) {
    if (DeeXAst_Optimize(self->ast_switch.sw_expr,DEE_OPTIMIZE_ARGS_SCOPE_EX(
     self->ast_switch.sw_expr_scope,&expr_assumptions,optimize_flags|DEE_OPTIMIZE_FLAG_USED)) != 0) {
     DEE_OPTIMIZE_SCOPE_BREAK(expr_assumptions); return -1;
    }
    DeeScope_OPTIMIZE(self->ast_switch.sw_scope);
    DEE_OPTIMIZE_SCOPE_ENTER(block_assumptions,self->ast_switch.sw_scope,&expr_assumptions) {
     error = DeeSAst_Optimize(self->ast_switch.sw_block,DEE_OPTIMIZE_ARGS_SCOPE_EX(
      self->ast_switch.sw_scope,&block_assumptions,optimize_flags&~(DEE_OPTIMIZE_FLAG_USED)));
    } DEE_OPTIMIZE_SCOPE_LEAVE(block_assumptions);
   } DEE_OPTIMIZE_SCOPE_LEAVE(expr_assumptions);
   return error;
  } break;

  case DEE_SASTKIND_IFTRUE:
   DEE_ASSERT((self->ast_iftrue.ic_succ_block != NULL) == (self->ast_iftrue.ic_succ_scope != NULL));
   DEE_ASSERT((self->ast_iftrue.ic_fail_block != NULL) == (self->ast_iftrue.ic_fail_scope != NULL));
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_DEL_NOEFFECT)==0) {
ifc_optimize_basic:
    if (self->ast_ifconst.ic_succ_block) {
     DeeScope_OPTIMIZE(self->ast_ifconst.ic_succ_scope);
     DEE_OPTIMIZE_SCOPE_ENTER(succ_assumptions,self->ast_ifconst.ic_succ_scope,assumptions) {
      error = DeeSAst_Optimize(self->ast_ifconst.ic_succ_block,
       DEE_OPTIMIZE_ARGS_SCOPE(self->ast_ifconst.ic_succ_scope,&succ_assumptions));
     } DEE_OPTIMIZE_SCOPE_LEAVE(succ_assumptions);
     if (error != 0) return error;
    }
    if (self->ast_ifconst.ic_fail_block) {
     DeeScope_OPTIMIZE(self->ast_ifconst.ic_fail_scope);
     DEE_OPTIMIZE_SCOPE_ENTER(fail_assumptions,self->ast_ifconst.ic_succ_scope,assumptions) {
      error = DeeSAst_Optimize(self->ast_ifconst.ic_fail_block,
       DEE_OPTIMIZE_ARGS_SCOPE(self->ast_ifconst.ic_fail_scope,&fail_assumptions));
     } DEE_OPTIMIZE_SCOPE_LEAVE(fail_assumptions);
     if (error != 0) return error;
    }
    break;
   }
   if (!self->ast_iftrue.ic_fail_block) {
    DeeSAstObject *block_ast;
    if (!self->ast_iftrue.ic_succ_block) {
     goto assign_empty;
    } else {
assign_ifc_succ_branch:
     if ((block_ast = DeeSAst_NewBlock(self->ast_common.ast_token,
      &self->ast_common.ast_labels,lexer,config->parser_flags,
      self->ast_iftrue.ic_succ_scope,1,&self->ast_iftrue.ic_succ_block)) == NULL) return -1;
     DeeSAst_AssignMove(self,block_ast);
     Dee_DECREF(block_ast);
    }
    ++*performed_optimizations;
    return 0;
   }
   if (self->ast_iftrue.ic_succ_block) {
    DeeScope_OPTIMIZE(self->ast_iftrue.ic_succ_scope);
    DEE_OPTIMIZE_SCOPE_ENTER(succ_assumptions,self->ast_iftrue.ic_succ_scope,assumptions) {
     error = DeeSAst_Optimize(self->ast_iftrue.ic_succ_block,
      DEE_OPTIMIZE_ARGS_SCOPE(self->ast_iftrue.ic_succ_scope,&succ_assumptions));
    } DEE_OPTIMIZE_SCOPE_LEAVE(succ_assumptions);
    if (error != 0) return error;
   }
   DeeScope_OPTIMIZE(self->ast_iftrue.ic_fail_scope);
   DEE_OPTIMIZE_SCOPE_ENTER(fail_assumptions,self->ast_iftrue.ic_fail_scope,assumptions) {
    error = DeeSAst_Optimize(self->ast_iftrue.ic_fail_block,
     DEE_OPTIMIZE_ARGS_SCOPE(self->ast_iftrue.ic_fail_scope,&fail_assumptions));
   } DEE_OPTIMIZE_SCOPE_LEAVE(fail_assumptions);
   if (error != 0) return error;
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_DEL_UNREACHABLE)!=0 &&
       !DeeSAst_HasLabels(self->ast_iftrue.ic_fail_block)) {
    VLOG_OPTIMIZE(self->ast_iftrue.ic_fail_block->ast_common.ast_token,
                  "Removing unreachable dead fail-branch in __if_true\n");
    if (self->ast_iftrue.ic_succ_block) goto assign_ifc_succ_branch;
    goto assign_empty;
   }
   break;

  case DEE_SASTKIND_IFFALSE:
   DEE_ASSERT((self->ast_iffalse.ic_succ_block != NULL) == (self->ast_iffalse.ic_succ_scope != NULL));
   DEE_ASSERT((self->ast_iffalse.ic_fail_block != NULL) == (self->ast_iffalse.ic_fail_scope != NULL));
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_DEL_NOEFFECT)==0) goto ifc_optimize_basic;
   if (!self->ast_iffalse.ic_succ_block) {
    DeeSAstObject *block_ast;
    if (!self->ast_iffalse.ic_fail_block) {
assign_empty:
     DeeSAst_AssignEmpty(self);
    } else {
assign_ifc_fail_branch:
     if ((block_ast = DeeSAst_NewBlock(self->ast_common.ast_token,
      &self->ast_common.ast_labels,lexer,config->parser_flags,
      self->ast_iffalse.ic_fail_scope,1,&self->ast_iffalse.ic_fail_block)) == NULL) return -1;
     DeeSAst_AssignMove(self,block_ast);
     Dee_DECREF(block_ast);
    }
    ++*performed_optimizations;
    return 0;
   }
   if (self->ast_iffalse.ic_fail_block) {
    DeeScope_OPTIMIZE(self->ast_iffalse.ic_fail_scope);
    DEE_OPTIMIZE_SCOPE_ENTER(fail_assumptions,self->ast_iffalse.ic_fail_scope,assumptions) {
     error = DeeSAst_Optimize(self->ast_iffalse.ic_fail_block,
      DEE_OPTIMIZE_ARGS_SCOPE(self->ast_iffalse.ic_fail_scope,&fail_assumptions));
    } DEE_OPTIMIZE_SCOPE_LEAVE(fail_assumptions);
    if (error != 0) return error;
   }
   DeeScope_OPTIMIZE(self->ast_iffalse.ic_succ_scope);
   DEE_OPTIMIZE_SCOPE_ENTER(succ_assumptions,self->ast_iffalse.ic_succ_scope,assumptions) {
    error = DeeSAst_Optimize(self->ast_iffalse.ic_succ_block,
     DEE_OPTIMIZE_ARGS_SCOPE(self->ast_iffalse.ic_succ_scope,&succ_assumptions));
   } DEE_OPTIMIZE_SCOPE_LEAVE(succ_assumptions);
   if (error != 0) return error;
   if ((optimize_flags&DEE_OPTIMIZE_FLAG_DEL_UNREACHABLE)!=0 &&
       !DeeSAst_HasLabels(self->ast_iffalse.ic_succ_block)) {
    VLOG_OPTIMIZE(self->ast_iffalse.ic_succ_block->ast_common.ast_token,
                  "Removing unreachable dead succ-branch in __if_false\n");
    if (self->ast_iffalse.ic_fail_block) goto assign_ifc_fail_branch;
    goto assign_empty;
   }
   break;

  default: break;
 }
 return 0;
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_COMPILER_SAST_OPTIMIZE_INL */
