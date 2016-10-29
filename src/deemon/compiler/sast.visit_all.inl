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

#define VISIT_XAST(ast) \
do{if((temp = DeeXAst_VisitAll((ast),visit_xast,visit_sast,closure))!=0)return temp;}while(0)
#define VISIT_SAST(ast) \
do{if((temp = DeeSAst_VisitAll((ast),visit_xast,visit_sast,closure))!=0)return temp;}while(0)
#define XVISIT_XAST(ast) do{if(ast)VISIT_XAST(ast);}while(0)
#define XVISIT_SAST(ast) do{if(ast)VISIT_SAST(ast);}while(0)

int DeeSAst_VisitAll(
 DEE_A_IN DeeSAstObject *self, DEE_A_IN_OPT DeeXAstVisitProc visit_xast,
 DEE_A_IN_OPT DeeSAstVisitProc visit_sast, void *closure) {
 int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeSAst_Check(self));
 if (visit_sast && (temp = (*visit_sast)(self,closure)) != 0) return temp;
 switch (self->ast_kind) {
  case DEE_SASTKIND_EXPRESSION:
   VISIT_XAST(self->ast_expression.x_ast);
   break;
  case DEE_SASTKIND_BLOCK: {
   DeeSAstObject **iter,**end;
   end = (iter = self->ast_block.b_astv)+self->ast_block.b_astc;
   while (iter != end) { VISIT_SAST(*iter); ++iter; }
  } break;
  case DEE_SASTKIND_RETURN:
   XVISIT_XAST(self->ast_return.r_value);
   break;
  case DEE_SASTKIND_YIELD:
   VISIT_XAST(self->ast_yield.y_value);
   break;
  case DEE_SASTKIND_ASSERT:
   VISIT_XAST(self->ast_assert.a_check);
   XVISIT_XAST(self->ast_assert.a_message);
   break;
  case DEE_SASTKIND_PRINT:
   XVISIT_XAST(self->ast_print.p_file);
   XVISIT_XAST(self->ast_print.p_data);
   break;
  case DEE_SASTKIND_IF:
   VISIT_XAST(self->ast_if.if_cond);
   XVISIT_SAST(self->ast_if.if_succ_block);
   XVISIT_SAST(self->ast_if.if_fail_block);
   break;
  case DEE_SASTKIND_FOR:
   XVISIT_XAST(self->ast_for.f_head_init);
   VISIT_XAST(self->ast_for.f_head_cond);
   XVISIT_XAST(self->ast_for.f_head_next);
   VISIT_SAST(self->ast_for.f_block);
   break;
  case DEE_SASTKIND_FORIN:
  case DEE_SASTKIND_FOREACH:
   VISIT_XAST(self->ast_foreach.fi_seq);
   VISIT_SAST(self->ast_foreach.fi_block);
   return DeeVarDeclStorage_VisitAll(&self->ast_foreach.fi_var,visit_xast,visit_sast,closure);
  case DEE_SASTKIND_WHILE:
  case DEE_SASTKIND_DOWHILE:
   VISIT_XAST(self->ast_while.w_cond);
   VISIT_SAST(self->ast_while.w_block);
   break;
  case DEE_SASTKIND_LOOPEVER:
  case DEE_SASTKIND_LOOPONCE:
  case DEE_SASTKIND_LOOPNONE:
   VISIT_SAST(self->ast_specialloop.sl_block);
   break;
  case DEE_SASTKIND_THROW:
   XVISIT_XAST(self->ast_throw.t_object);
   break;
  case DEE_SASTKIND_TRY: {
   struct DeeSAstTryHandler *iter,*end;
   VISIT_SAST(self->ast_try.t_block);
   DEE_ASSERT(self->ast_try.t_handlerc != 0);
   end = (iter = self->ast_try.t_handlerv)+self->ast_try.t_handlerc;
   do {
    VISIT_SAST(iter->th_block);
    if (DeeVarDeclStorage_VisitAll(&iter->th_local,visit_xast,visit_sast,closure) != 0) return -1;
   } while (++iter != end);
  } break;
  case DEE_SASTKIND_MODULE:
   VISIT_SAST(self->ast_module.m_block);
   break;
  case DEE_SASTKIND_SWITCH:
   VISIT_XAST(self->ast_switch.sw_expr);
   VISIT_SAST(self->ast_switch.sw_block);
   break;
  case DEE_SASTKIND_IFTRUE:
  case DEE_SASTKIND_IFFALSE:
   XVISIT_SAST(self->ast_ifconst.ic_succ_block);
   XVISIT_SAST(self->ast_ifconst.ic_fail_block);
   break;
  default: break;
 }
 return 0;
}
#undef XVISIT_SAST
#undef XVISIT_XAST
#undef VISIT_SAST
#undef VISIT_XAST


DEE_DECL_END
