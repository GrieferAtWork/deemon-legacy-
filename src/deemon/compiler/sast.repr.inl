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

static DeeObject *_deesast_tp_basicrepr(DeeSAstObject *self) {
 switch (self->ast_kind) {
  case DEE_SASTKIND_EMPTY:
   DeeReturn_STATIC_STRING(";");
  case DEE_SASTKIND_EXPRESSION:
   return DeeString_Newf("%r;",self->ast_expression.x_ast);
  case DEE_SASTKIND_BLOCK: {
   DeeObject *result; DeeSAstObject **iter,**end;
   DeeStringWriter writer = DeeStringWriter_INIT();
   end = (iter = self->ast_block.b_astv)+self->ast_block.b_astc;
   if (DeeStringWriter_WRITE_STRING(&writer,"{\n") != 0) goto block_err;
   while (iter != end) if (DeeStringWriter_Writef(&writer,"\t%>r\n",*iter++) != 0) goto block_err;
   if (DeeStringWriter_WRITE_STRING(&writer,"}") != 0) goto block_err;
   result = DeeStringWriter_Pack(&writer);
block_end: DeeStringWriter_Quit(&writer); return result;
block_err: result = NULL; goto block_end;
  } break;
  case DEE_SASTKIND_RETURN:
   if (!self->ast_return.r_value) DeeReturn_STATIC_STRING("return;");
   return DeeString_Newf("return %r;",self->ast_return.r_value);
  case DEE_SASTKIND_YIELD:
   return DeeString_Newf("yield %r...;",self->ast_yield.y_value);
  case DEE_SASTKIND_ASSERT:
   return self->ast_assert.a_message
    ? DeeString_Newf("assert %r: %r;",self->ast_assert.a_check,self->ast_assert.a_message)
    : DeeString_Newf("assert %r;",self->ast_assert.a_check);
  case DEE_SASTKIND_PRINT:
   if (self->ast_print.p_data) {
    return self->ast_print.p_file
     ? DeeString_Newf("print %r: (%r)...%s;",self->ast_print.p_file,self->ast_print.p_data,
       (self->ast_print.p_flags&DEE_SASTPRINT_FLAG_NOLF)!=0 ? "," : "")
     : DeeString_Newf("print (%r)...%s;",self->ast_print.p_data,
       (self->ast_print.p_flags&DEE_SASTPRINT_FLAG_NOLF)!=0 ? "," : "");
   } else {
    return self->ast_print.p_file
     ? DeeString_Newf("print %r:%s;",self->ast_print.p_file,
       (self->ast_print.p_flags&DEE_SASTPRINT_FLAG_NOLF)!=0 ? "," : "")
     : DeeString_Newf("print%s;",
       (self->ast_print.p_flags&DEE_SASTPRINT_FLAG_NOLF)!=0 ? "," : "");
   }
  case DEE_SASTKIND_IF:
   if (self->ast_if.if_succ_block && self->ast_if.if_fail_block)
    return DeeString_Newf("if (%r) %r%selse %r",self->ast_if.if_cond,self->ast_if.if_succ_block,
     self->ast_if.if_succ_block->ast_kind != DEE_SASTKIND_BLOCK ? "\n" : " ",self->ast_if.if_fail_block);
   else if (self->ast_if.if_succ_block)
    return DeeString_Newf("if (%r) %r",self->ast_if.if_cond,self->ast_if.if_succ_block);
   else if (self->ast_if.if_fail_block)
    return DeeString_Newf("if (%r);\nelse %r",self->ast_if.if_cond,self->ast_if.if_fail_block);
   else return DeeString_Newf("if (%r);",self->ast_if.if_cond);
  case DEE_SASTKIND_FOR:
   DEE_ASSERT(DeeObject_Check(self->ast_for.f_head_cond) && DeeXAst_Check(self->ast_for.f_head_cond));
   return DeeString_Newf("for (%K;%K;%K) %r",
                         self->ast_for.f_head_init ? DeeObject_Repr((DeeObject *)self->ast_for.f_head_init) : DeeString_NewSized(0),
                         DeeObject_Repr((DeeObject *)self->ast_for.f_head_cond),
                         self->ast_for.f_head_next ? DeeObject_Repr((DeeObject *)self->ast_for.f_head_next) : DeeString_NewSized(0),
                         self->ast_for.f_block);
  case DEE_SASTKIND_FORIN:
  case DEE_SASTKIND_FOREACH:
   return DeeString_Newf("%s (%K: %r) %r",
                         self->ast_kind == DEE_SASTKIND_FORIN ? "for" : "__foreach",
                         DeeVarDeclStorage_Repr(&self->ast_forin.fi_var),
                         self->ast_forin.fi_seq,self->ast_forin.fi_block);
  case DEE_SASTKIND_WHILE:
   return DeeString_Newf("while (%r) %r",
                         self->ast_while.w_cond,
                         self->ast_while.w_block);
  case DEE_SASTKIND_DOWHILE:
   return DeeString_Newf("do %r while (%r);",
                         self->ast_dowhile.w_block,
                         self->ast_dowhile.w_cond);
  case DEE_SASTKIND_LOOPEVER:
   return DeeString_Newf("__loopever %r",
                         self->ast_loopever.sl_block);
  case DEE_SASTKIND_LOOPONCE:
   return DeeString_Newf("__looponce %r",
                         self->ast_looponce.sl_block);
  case DEE_SASTKIND_LOOPNONE:
   return DeeString_Newf("__loopnone %r",
                         self->ast_loopnone.sl_block);
  case DEE_SASTKIND_CONTINUE:
   DeeReturn_STATIC_STRING("continue;");
  case DEE_SASTKIND_BREAK:
   DeeReturn_STATIC_STRING("break;");
  case DEE_SASTKIND_GOTO:
   // WARNING: This isn't safe if calling 'repr' on an xast during runtime
   DEE_ASSERT(self->ast_goto.g_label->pl_kind == DEE_PARSERLABEL_KIND_LABEL);
   return DeeString_Newf("goto @%Iu;",self->ast_goto.g_label->pl_name);
  case DEE_SASTKIND_THROW:
   if (!self->ast_throw.t_object) DeeReturn_STATIC_STRING("throw;");
   return DeeString_Newf("throw %r;",self->ast_throw.t_object);
  case DEE_SASTKIND_TRY: {
   DeeObject *result; struct DeeSAstTryHandler *iter,*end;
   DeeStringWriter writer = DeeStringWriter_INIT();
   DEE_ASSERT(self->ast_try.t_handlerc != 0);
   if (DeeStringWriter_Writef(&writer,"try %r%s",self->ast_try.t_block,
    self->ast_try.t_block->ast_kind == DEE_SASTKIND_BLOCK ? " " : "\n") != 0) goto try_err;
   end = (iter = self->ast_try.t_handlerv)+self->ast_try.t_handlerc;
   do {
    switch (iter->th_kind) {
     case DeeExceptionHandleKind_ALL:
      if (DeeStringWriter_WRITE_STRING(&writer,"catch (...)") != 0) goto try_err; break;
     case DeeExceptionHandleKind_ALL_VAR:
      if (DeeStringWriter_Writef(&writer,"catch (%K...)",
       DeeVarDeclStorage_Repr(&iter->th_local)) != 0) goto try_err; break;
     case DeeExceptionHandleKind_TYPED:
      if (DeeStringWriter_Writef(&writer,"catch (%r)",
       iter->th_type ? (DeeObject *)iter->th_type : (DeeObject *)&DeeObject_Type) != 0) goto try_err; break;
     case DeeExceptionHandleKind_TYPED_VAR:
      if (DeeStringWriter_Writef(&writer,"catch (%r %K)",
       iter->th_type ? (DeeObject *)iter->th_type : (DeeObject *)&DeeObject_Type,
       DeeVarDeclStorage_Repr(&iter->th_local)) != 0) goto try_err; break;
     default: if (DeeStringWriter_WRITE_STRING(&writer,"finally") != 0) goto try_err; break;
    }
    if (DeeStringWriter_Writef(&writer," %r%s",iter->th_block,
     iter == end-1 ? "" : iter->th_block->ast_kind == DEE_SASTKIND_BLOCK ? " " : "\n") != 0) goto try_err;
   } while (++iter != end);
   result = DeeStringWriter_Pack(&writer);
try_end: DeeStringWriter_Quit(&writer); return result;
try_err: result = NULL; goto try_end;
  } break;

  case DEE_SASTKIND_MODULE:
   return DeeString_Newf("module %r",self->ast_module.m_block);
  case DEE_SASTKIND_SWITCH:
   return DeeString_Newf("switch (%r) %r;",
                         self->ast_switch.sw_expr,
                         self->ast_switch.sw_block);

  {
   char const *sname;
   if (0) { case DEE_SASTKIND_IFTRUE: sname = "__if_true"; }
   if (0) { case DEE_SASTKIND_IFFALSE: sname = "__if_false"; }
   if (self->ast_ifconst.ic_succ_block && self->ast_ifconst.ic_fail_block)
    return DeeString_Newf("%s %r%selse %r",sname,self->ast_ifconst.ic_succ_block,
     self->ast_ifconst.ic_succ_block->ast_kind != DEE_SASTKIND_BLOCK ? "\n" : " ",self->ast_ifconst.ic_fail_block);
   else if (self->ast_ifconst.ic_succ_block)
    return DeeString_Newf("%s %r",sname,self->ast_ifconst.ic_succ_block);
   else if (self->ast_ifconst.ic_fail_block)
    return DeeString_Newf("%s;\nelse %r",sname,self->ast_ifconst.ic_fail_block);
   else return DeeString_Newf("%s;",sname);
  }

  default: break;
 }
 return DeeString_Newf("/* Unknown ast: %#x(%d) */none;",
                       self->ast_kind,self->ast_kind);
}
static DeeObject *_deesast_tp_repr(DeeSAstObject *self) {
 if (DeeParserLabelRefList_EMPTY(&self->ast_common.ast_labels))
  return _deesast_tp_basicrepr(self);
 {
  DeeObject *result;
  struct DeeParserLabel **iter,**end,*elem;
  end = (iter = DeeParserLabelRefList_ELEM(&self->ast_common.ast_labels)
         )+DeeParserLabelRefList_SIZE(&self->ast_common.ast_labels);
  DeeStringWriter writer = DeeStringWriter_INIT();
  while (iter != end) {
   elem = *iter++;
   switch (elem->pl_kind) {
    case DEE_PARSERLABEL_KIND_LABEL:
     if (DeeStringWriter_Writef(&writer,"@%Iu: ",elem->pl_name) != 0) goto err;
     break;
    case DEE_PARSERLABEL_KIND_C_DEFAULT:
     if (DeeStringWriter_WRITE_STRING(&writer,"default: ") != 0) goto err;
     break;
    case DEE_PARSERLABEL_KIND_C_CASE:
     if (DeeStringWriter_Writef(&writer,"case (%r): ",elem->pl_c_case.clc_case) != 0) goto err;
     break;
    case DEE_PARSERLABEL_KIND_C_RANGE:
     if (DeeStringWriter_Writef(&writer,"case (%r) ... (%r): ",
      elem->pl_c_range.clr_begin,elem->pl_c_range.clr_end) != 0) goto err;
     break;
    default: break;
   }
  }
  if ((result = _deesast_tp_basicrepr(self)) == NULL) goto end;
  if (DeeStringWriter_WriteStringWithLength(&writer,DeeString_SIZE(result),DeeString_STR(result)) != 0) { Dee_DECREF(result); goto err; }
  Dee_DECREF(result);
  result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer); return result;
err: result = NULL; goto end;
 }
}

DEE_DECL_END

