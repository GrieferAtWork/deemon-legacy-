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
int DeeXAst_VisitAll(
 DEE_A_IN DeeXAstObject *self, DEE_A_IN_OPT DeeXAstVisitProc visit_xast,
 DEE_A_IN_OPT DeeSAstVisitProc visit_sast, void *closure) {
 int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeXAst_Check(self));
 if (visit_xast && (temp = (*visit_xast)(self,closure)) != 0) return temp;
 switch (self->ast_kind) {

  case DEE_XASTKIND_VARDECL:
   VISIT_XAST(self->ast_vardecl.vd_init);
   break;

  case DEE_XASTKIND_MULTIVARDECL: {
   struct DeeVarDeclStorage *iter,*end;
   VISIT_XAST(self->ast_multivardecl.mvd_init);
   end = (iter = self->ast_multivardecl.mvd_varv)+self->ast_multivardecl.mvd_varc;
   DEE_ASSERT(self->ast_multivardecl.mvd_varc);
   do if (DeeVarDeclStorage_VisitAll(iter,visit_xast,visit_sast,closure) != 0) return -1;
   while (++iter != end);
  } break;

  case DEE_XASTKIND_STATEMENT:
   VISIT_SAST(self->ast_statement.s_stmt);
   break;

  case DEE_XASTKIND_IF:
   VISIT_XAST(self->ast_if.if_cond);
   XVISIT_XAST(self->ast_if.if_succ);
   XVISIT_XAST(self->ast_if.if_fail);
   break;

  case DEE_XASTKIND_FUNCTION: {
   struct DeeXAstFunctionAstArgument *iter,*end;
   end = (iter = self->ast_function.f_argv)+self->ast_function.f_argc;
   while (iter != end) { XVISIT_XAST(iter->a_default); ++iter; }
   VISIT_SAST(self->ast_function.f_code);
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case DEE_XASTKIND_CLASS: {
   struct DeeXAstClassAstEntry *iter,*end;
   XVISIT_XAST(self->ast_class.c_base);
   end = (iter = self->ast_class.c_entryv)+self->ast_class.c_entryc;
   while (iter != end) {
    switch (iter->ce_kind) {
     case DEE_XAST_CLASSENTRY_KIND_SLOT:
      VISIT_XAST(iter->ce_slot.cs_callback);
      break;
     case DEE_XAST_CLASSENTRY_KIND_METHOD:
     case DEE_XAST_CLASSENTRY_KIND_CLASS_METHOD:
      VISIT_XAST(iter->ce_method.cm_callback);
      break;
     case DEE_XAST_CLASSENTRY_KIND_GETSET:
     case DEE_XAST_CLASSENTRY_KIND_CLASS_GETSET:
      XVISIT_XAST(iter->ce_getset.cg_get);
      XVISIT_XAST(iter->ce_getset.cg_del);
      XVISIT_XAST(iter->ce_getset.cg_set);
      break;
     default: break;
    }
    ++iter;
   }
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */

  case DEE_XASTKIND_TUPLE:
  case DEE_XASTKIND_LIST:
  case DEE_XASTKIND_SET: {
   DeeXAstObject **iter,**end;
   end = (iter = self->ast_sequence.s_elemv)+self->ast_sequence.s_elemc;
   DEE_ASSERT(self->ast_sequence.s_elemv != NULL);
   DEE_ASSERT(self->ast_sequence.s_elemc != 0);
   do VISIT_XAST(*iter); while (++iter != end);
  } break;

  case DEE_XASTKIND_DICT: {
   struct DeeXAstDictEntry *iter,*end;
   end = (iter = self->ast_dict.d_elemv)+self->ast_dict.d_elemc;
   DEE_ASSERT(self->ast_dict.d_elemv != NULL);
   DEE_ASSERT(self->ast_dict.d_elemc != 0);
   do {
    VISIT_XAST(iter->de_key);
    VISIT_XAST(iter->de_item);
   } while (++iter != end);
  } break;

  case DEE_XASTKIND_SEQ_RANGE_SET:
   VISIT_XAST(self->ast_seq_range_set.sr_value);
   DEE_ATTRIBUTE_FALLTHROUGH
  case DEE_XASTKIND_SEQ_RANGE_GET:
  case DEE_XASTKIND_SEQ_RANGE_DEL:
   XVISIT_XAST(self->ast_seq_range.sr_begin);
   XVISIT_XAST(self->ast_seq_range.sr_end);
   VISIT_XAST(self->ast_seq_range.sr_seq);
   break;

  case DEE_XASTKIND_ATTR_SET_C:
   VISIT_XAST(self->ast_attr_set_c.ac_value);
   DEE_ATTRIBUTE_FALLTHROUGH
  case DEE_XASTKIND_ATTR_GET_C:
  case DEE_XASTKIND_ATTR_HAS_C:
  case DEE_XASTKIND_ATTR_DEL_C:
   VISIT_XAST(self->ast_attr_c.ac_object);
   break;

#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
  case DEE_XASTKIND_FOREIGN_FUNCTION: {
   DeeXAstObject **iter,**end;
   VISIT_XAST(self->ast_foreignfunction.ff_base);
   end = (iter = self->ast_foreignfunction.ff_argv)+self->ast_foreignfunction.ff_argc;
   while (iter != end) { VISIT_XAST(*iter);  ++iter; }
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */

  case DEE_XASTKIND_SWITCH: {
   struct DeeXAstSwitchEntry *iter,*end;
   end = (iter = self->ast_switch.s_entryv)+self->ast_switch.s_entryc;
   VISIT_XAST(self->ast_switch.s_expr);
   DEE_ASSERT(iter != end);
   do VISIT_XAST(iter->se_value); while (++iter != end);
   XVISIT_XAST(self->ast_switch.s_default);
  } break;

  case DEE_XASTKIND_IFTRUE:
  case DEE_XASTKIND_IFFALSE:
   XVISIT_XAST(self->ast_ifconst.ic_succ);
   XVISIT_XAST(self->ast_ifconst.ic_fail);
   break;

#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
  case DEE_XASTKIND_BUILTIN_EXTERN:
   XVISIT_XAST(self->ast_builtin_extern.ex_file);
   VISIT_XAST(self->ast_builtin_extern.ex_name);
   VISIT_XAST(self->ast_builtin_extern.ex_type);
   break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
  case DEE_XASTKIND_BUILTIN_EXPECT:
   VISIT_XAST(self->ast_builtin_expect.e_value);
   break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */
  
  default: if (DEE_XASTKIND_ISOPERATOR(self->ast_kind)) {
   switch (DEE_XASTKIND_OPCOUNT(self->ast_kind)) {
    case 3: VISIT_XAST(self->ast_operator.op_c); DEE_ATTRIBUTE_FALLTHROUGH
    case 2: VISIT_XAST(self->ast_operator.op_b); DEE_ATTRIBUTE_FALLTHROUGH
    case 1: VISIT_XAST(self->ast_operator.op_a); DEE_ATTRIBUTE_FALLTHROUGH
    default: break;
   }
  } break;
 }
 return 0;
}

int DeeVarDeclStorage_VisitAll(
 DEE_A_IN struct DeeVarDeclStorage const *self, DEE_A_IN_OPT DeeXAstVisitProc visit_xast,
 DEE_A_IN_OPT DeeSAstVisitProc visit_sast, void *closure) {
 int temp;
 DEE_ASSERT(self);
 switch (self->vds_kind) {
  case DEE_VARDECLSTORAGEKIND_ATTR:
   VISIT_XAST(self->vds_attr.as_object);
   VISIT_XAST(self->vds_attr.as_attr);
   break;
  case DEE_VARDECLSTORAGEKIND_ATTR_C:
   VISIT_XAST(self->vds_attr_c.acs_object);
   break;
  case DEE_VARDECLSTORAGEKIND_ITEM:
   VISIT_XAST(self->vds_item.is_object);
   VISIT_XAST(self->vds_item.is_key);
   break;
  case DEE_VARDECLSTORAGEKIND_RANGE:
   VISIT_XAST(self->vds_range.rs_object);
   XVISIT_XAST(self->vds_range.rs_begin);
   XVISIT_XAST(self->vds_range.rs_end);
   break;
  case DEE_VARDECLSTORAGEKIND_EXPAND: {
   struct DeeVarDeclStorage *iter,*end;
   end = (iter = self->vds_expand.es_declv)+self->vds_expand.es_declc;
   DEE_ASSERT(self->vds_expand.es_declc);
   do if (DeeVarDeclStorage_VisitAll(iter,visit_xast,visit_sast,closure) != 0) return -1;
   while (++iter != end);
  } break;
  case DEE_VARDECLSTORAGEKIND_ASSIGN:
   VISIT_XAST(self->vds_assign.as_ast);
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
