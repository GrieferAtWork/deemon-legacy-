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

DEE_A_RET_EXCEPT(-1) int DeeSAst_CollectTopScopeChildren(
 DEE_A_IN DeeSAstObject const *self, DEE_A_INOUT struct DeeScopeList *scopes) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSAst_Check(self));
 DEE_ASSERT(scopes);
 switch (self->ast_kind) {
  case DEE_SASTKIND_EXPRESSION:
   return DeeXAst_CollectTopScopeChildren(self->ast_expression.x_ast,scopes);
  case DEE_SASTKIND_BLOCK:
   return DeeScopeList_Append(scopes,self->ast_block.b_scope);
  case DEE_SASTKIND_RETURN:
   if (!self->ast_return.r_value) return 0;
   return DeeXAst_CollectTopScopeChildren(self->ast_return.r_value,scopes);
  case DEE_SASTKIND_YIELD:
   return DeeXAst_CollectTopScopeChildren(self->ast_yield.y_value,scopes);
  case DEE_SASTKIND_ASSERT:
   if (DeeXAst_CollectTopScopeChildren(self->ast_assert.a_check,scopes) != 0) return -1;
   if (!self->ast_assert.a_message) return 0;
   return DeeXAst_CollectTopScopeChildren(self->ast_assert.a_message,scopes);
  case DEE_SASTKIND_PRINT: 
   if (self->ast_print.p_file && DeeXAst_CollectTopScopeChildren(self->ast_print.p_file,scopes) != 0) return -1;
   if (self->ast_print.p_data && DeeXAst_CollectTopScopeChildren(self->ast_print.p_data,scopes) != 0) return -1;
   break;
  case DEE_SASTKIND_IF:
   return DeeScopeList_Append(scopes,self->ast_if.if_cond_scope);
  case DEE_SASTKIND_FOR:
   return DeeScopeList_Append(scopes,self->ast_for.f_head_init_scope);
  case DEE_SASTKIND_FORIN:
  case DEE_SASTKIND_FOREACH:
   return DeeScopeList_Append(scopes,self->ast_foreach.fi_seq_scope);
  case DEE_SASTKIND_WHILE:
  case DEE_SASTKIND_DOWHILE:
   return DeeScopeList_Append(scopes,self->ast_while.w_cond_scope);
  case DEE_SASTKIND_LOOPEVER:
  case DEE_SASTKIND_LOOPONCE:
  case DEE_SASTKIND_LOOPNONE:
   return DeeScopeList_Append(scopes,self->ast_specialloop.sl_scope);
  case DEE_SASTKIND_THROW:
   if (!self->ast_throw.t_object) return 0;
   return DeeXAst_CollectTopScopeChildren(self->ast_throw.t_object,scopes);
  case DEE_SASTKIND_TRY:
   return DeeScopeList_Append(scopes,self->ast_try.t_rootscope);
  case DEE_SASTKIND_MODULE:
   return DeeScopeList_Append(scopes,self->ast_module.m_module);
  case DEE_SASTKIND_SWITCH:
   return DeeScopeList_Append(scopes,self->ast_switch.sw_expr_scope);
  case DEE_SASTKIND_IFTRUE:
  case DEE_SASTKIND_IFFALSE:
   if (self->ast_ifconst.ic_succ_scope && DeeScopeList_Append(scopes,self->ast_ifconst.ic_succ_scope) != 0) return -1;
   if (self->ast_ifconst.ic_fail_scope && DeeScopeList_Append(scopes,self->ast_ifconst.ic_fail_scope) != 0) return -1;
   break;
  default: break;
 }
 return 0;
}

DEE_DECL_END
