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

DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeSAst_Equals(
 DEE_A_IN DeeSAstObject const *self,
 DEE_A_IN DeeSAstObject const *right) {
 int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeSAst_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeSAst_Check(right));
 if DEE_UNLIKELY(self == right) return 1;
 if DEE_LIKELY(self->ast_kind != right->ast_kind) return 0;
 switch (self->ast_kind) {
  case DEE_SASTKIND_EXPRESSION:
   return DeeXAst_Equals(self->ast_expression.x_ast,right->ast_expression.x_ast);
  case DEE_SASTKIND_BLOCK: {
   DeeSAstObject **iter,**end,**src;
   if (self->ast_block.b_astc != right->ast_block.b_astc) return 0;
   end = (iter = self->ast_block.b_astv)+self->ast_block.b_astc;
   src = right->ast_block.b_astv;
   while (iter != end) if ((temp = DeeSAst_Equals(*iter++,*src++)) <= 0) return temp;
  } break;
  case DEE_SASTKIND_RETURN:
   if (!self->ast_return.r_value) return right->ast_return.r_value == NULL;
   if (!right->ast_return.r_value) return 0;
   return DeeXAst_Equals(self->ast_return.r_value,right->ast_return.r_value);
  case DEE_SASTKIND_YIELD:
   return DeeXAst_Equals(self->ast_yield.y_value,right->ast_yield.y_value);
  case DEE_SASTKIND_ASSERT:
   if ((temp = DeeXAst_Equals(self->ast_assert.a_check,right->ast_assert.a_check)) <= 0) return temp;
   if (!self->ast_assert.a_message) return right->ast_assert.a_message == NULL;
   if (!right->ast_assert.a_message) return 0;
   return DeeXAst_Equals(self->ast_assert.a_message,right->ast_assert.a_message);
  case DEE_SASTKIND_PRINT:
   if (!self->ast_print.p_file) {
    if (right->ast_print.p_file) return 0;
   } else {
    if (!right->ast_print.p_file) return 0;
    if ((temp = DeeXAst_Equals(self->ast_print.p_file,right->ast_print.p_file)) <= 0) return temp;
   }
   if (!self->ast_print.p_data) return right->ast_print.p_data == NULL;
   if (!right->ast_print.p_data) return 0;
   return DeeXAst_Equals(self->ast_print.p_data,right->ast_print.p_data);
  case DEE_SASTKIND_IF:
   if ((self->ast_if.if_succ_block != NULL) != (right->ast_if.if_succ_block != NULL) ||
       (self->ast_if.if_fail_block != NULL) != (right->ast_if.if_fail_block != NULL)) return 0;
   if ((temp = DeeXAst_Equals(self->ast_if.if_cond,right->ast_if.if_cond)) <= 0) return temp;
   if (self->ast_if.if_succ_block && (temp = DeeSAst_Equals(self->ast_if.if_succ_block,right->ast_if.if_succ_block)) <= 0) return temp;
   if (self->ast_if.if_fail_block && (temp = DeeSAst_Equals(self->ast_if.if_fail_block,right->ast_if.if_fail_block)) <= 0) return temp;
   break;
  case DEE_SASTKIND_FOR:
   DEE_ASSERT(DeeObject_Check(self->ast_for.f_head_cond) && DeeXAst_Check(self->ast_for.f_head_cond));
   if ((self->ast_for.f_head_init != NULL) != (right->ast_for.f_head_init != NULL) ||
       (self->ast_for.f_head_next != NULL) != (right->ast_for.f_head_next != NULL)) return 0;
   if (self->ast_for.f_head_init && (temp = DeeXAst_Equals(self->ast_for.f_head_init,right->ast_for.f_head_init)) <= 0) return temp;
   if ((temp = DeeXAst_Equals(self->ast_for.f_head_cond,right->ast_for.f_head_cond)) <= 0) return temp;
   if (self->ast_for.f_head_next && (temp = DeeXAst_Equals(self->ast_for.f_head_next,right->ast_for.f_head_next)) <= 0) return temp;
   return DeeSAst_Equals(self->ast_for.f_block,right->ast_for.f_block);
  case DEE_SASTKIND_FORIN:
  case DEE_SASTKIND_FOREACH:
   if ((temp = DeeVarDeclStorage_Equals(&self->ast_forin.fi_var,&right->ast_forin.fi_var)) <= 0) return temp;
   if ((temp = DeeXAst_Equals(self->ast_forin.fi_seq,right->ast_forin.fi_seq)) <= 0) return temp;
   return DeeSAst_Equals(self->ast_forin.fi_block,right->ast_forin.fi_block);
  case DEE_SASTKIND_WHILE:
  case DEE_SASTKIND_DOWHILE:
   if ((temp = DeeXAst_Equals(self->ast_while.w_cond,right->ast_while.w_cond)) <= 0) return temp;
   return DeeSAst_Equals(self->ast_while.w_block,right->ast_while.w_block);
  case DEE_SASTKIND_LOOPONCE:
  case DEE_SASTKIND_LOOPEVER:
  case DEE_SASTKIND_LOOPNONE:
   return DeeSAst_Equals(self->ast_specialloop.sl_block,right->ast_specialloop.sl_block);
  case DEE_SASTKIND_GOTO:
   return self->ast_goto.g_label == right->ast_goto.g_label;
  case DEE_SASTKIND_THROW:
   if (!self->ast_throw.t_object) return right->ast_throw.t_object == NULL;
   return DeeXAst_Equals(self->ast_throw.t_object,right->ast_throw.t_object);
  case DEE_SASTKIND_TRY: {
   struct DeeSAstTryHandler *iter,*end,*rhs_iter;
   DEE_ASSERT(self->ast_try.t_handlerc != 0);
   DEE_ASSERT(right->ast_try.t_handlerc != 0);
   if (self->ast_try.t_handlerc != right->ast_try.t_handlerc) return 0;
   if ((temp = DeeSAst_Equals(self->ast_try.t_block,right->ast_try.t_block)) <= 0) return temp;
   end = (iter = self->ast_try.t_handlerv)+self->ast_try.t_handlerc;
   rhs_iter = right->ast_try.t_handlerv;
   do {
    if (iter->th_kind != rhs_iter->th_kind) return 0;
    if ((temp = DeeVarDeclStorage_Equals(&iter->th_local,&rhs_iter->th_local)) <= 0) return temp;
    if (iter->th_type != rhs_iter->th_type) return 0;
    if ((temp = DeeSAst_Equals(iter->th_block,rhs_iter->th_block)) <= 0) return temp;
    ++iter,++rhs_iter;
   } while (iter != end);
  } break;
  case DEE_SASTKIND_MODULE:
   if (self->ast_module.m_module != right->ast_module.m_module) return 0;
   return DeeSAst_Equals(self->ast_module.m_block,right->ast_module.m_block);
  case DEE_SASTKIND_SWITCH:
   if (self->ast_switch.sw_expr != right->ast_switch.sw_expr) return 0;
   if ((temp = DeeSAst_Equals(self->ast_switch.sw_block,right->ast_switch.sw_block)) <= 0) return temp;
   // NOTE: No need to compare the labels, as those should already be included in 'sw_block'
   break;
  case DEE_SASTKIND_IFTRUE:
  case DEE_SASTKIND_IFFALSE:
   if ((self->ast_ifconst.ic_succ_block != NULL) != (right->ast_ifconst.ic_succ_block != NULL)) return 0;
   if ((self->ast_ifconst.ic_fail_block != NULL) != (right->ast_ifconst.ic_fail_block != NULL)) return 0;
   if (self->ast_ifconst.ic_succ_block && (temp = DeeSAst_Equals(self->ast_ifconst.ic_succ_block,right->ast_ifconst.ic_succ_block)) <= 0) return temp;
   if (self->ast_ifconst.ic_fail_block && (temp = DeeSAst_Equals(self->ast_ifconst.ic_fail_block,right->ast_ifconst.ic_fail_block)) <= 0) return temp;
   break;
  default: break;
 }
 return 1;
}

DEE_DECL_END

