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

DEE_A_RET_EXCEPT(-1) int DeeXAst_CollectTopScopeChildren(
 DEE_A_IN DeeXAstObject const *self, DEE_A_INOUT struct DeeScopeList *scopes) {
 DEE_ASSERT(DeeObject_Check(self) && DeeXAst_Check(self));
 DEE_ASSERT(scopes);
 switch (self->ast_kind) {

  case DEE_XASTKIND_VARDECL:
   return DeeXAst_CollectTopScopeChildren(self->ast_vardecl.vd_init,scopes);

  case DEE_XASTKIND_MULTIVARDECL: {
   struct DeeVarDeclStorage *iter,*end;
   end = (iter = self->ast_multivardecl.mvd_varv)+self->ast_multivardecl.mvd_varc;
   DEE_ASSERT(self->ast_multivardecl.mvd_varc);
   do if DEE_UNLIKELY(DeeVarDeclStorage_CollectTopScopeChildren(iter,scopes) != 0) return -1;
   while (++iter != end);
   return DeeXAst_CollectTopScopeChildren(self->ast_multivardecl.mvd_init,scopes);
  } break;

  case DEE_XASTKIND_STATEMENT:
   return DeeSAst_CollectTopScopeChildren(self->ast_statement.s_stmt,scopes);

  case DEE_XASTKIND_IF:
   if DEE_UNLIKELY(DeeXAst_CollectTopScopeChildren(self->ast_if.if_cond,scopes) != 0) return -1;
   if DEE_UNLIKELY(self->ast_if.if_succ && DeeXAst_CollectTopScopeChildren(self->ast_if.if_succ,scopes) != 0) return -1;
   if DEE_UNLIKELY(self->ast_if.if_fail && DeeXAst_CollectTopScopeChildren(self->ast_if.if_fail,scopes) != 0) return -1;
   break;

  case DEE_XASTKIND_FUNCTION:
   if (!self->ast_function.f_scope) return 0;
   return DeeScopeList_Append(scopes,self->ast_function.f_scope);

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case DEE_XASTKIND_CLASS: {
   struct DeeXAstClassAstEntry *iter,*end;
   if DEE_UNLIKELY(self->ast_class.c_base && DeeXAst_CollectTopScopeChildren(self->ast_class.c_base,scopes) != 0) return -1;
   end = (iter = self->ast_class.c_entryv)+self->ast_class.c_entryc;
   while (iter != end) {
    if DEE_UNLIKELY(DeeXAstClassAstEntry_CollectTopScopeChildren(iter,scopes) != 0) return -1;
    ++iter;
   }
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */

  case DEE_XASTKIND_TUPLE:
  case DEE_XASTKIND_LIST:
  case DEE_XASTKIND_SET: {
   DeeXAstObject **iter,**end;
   end = (iter = self->ast_sequence.s_elemv)+self->ast_sequence.s_elemc;
   DEE_ASSERT(self->ast_sequence.s_elemc);
   do if DEE_UNLIKELY(DeeXAst_CollectTopScopeChildren(*iter,scopes) != 0) return -1;
   while (++iter != end);
  } break;

  case DEE_XASTKIND_DICT: {
   struct DeeXAstDictEntry *iter,*end;
   end = (iter = self->ast_dict.d_elemv)+self->ast_dict.d_elemc;
   DEE_ASSERT(self->ast_dict.d_elemc);
   do if DEE_UNLIKELY(DeeXAst_CollectTopScopeChildren(iter->de_key,scopes) != 0 ||
                      DeeXAst_CollectTopScopeChildren(iter->de_item,scopes) != 0) return -1;
   while (++iter != end);
  } break;

  case DEE_XASTKIND_SEQ_RANGE_SET:
   if DEE_UNLIKELY(DeeXAst_CollectTopScopeChildren(self->ast_seq_range_set.sr_value,scopes) != 0) return -1;
   DEE_ATTRIBUTE_FALLTHROUGH
  case DEE_XASTKIND_SEQ_RANGE_GET:
  case DEE_XASTKIND_SEQ_RANGE_DEL:
   if DEE_UNLIKELY(self->ast_seq_range.sr_begin && DeeXAst_CollectTopScopeChildren(self->ast_seq_range.sr_begin,scopes) != 0) return -1;
   if DEE_UNLIKELY(self->ast_seq_range.sr_end && DeeXAst_CollectTopScopeChildren(self->ast_seq_range.sr_end,scopes) != 0) return -1;
   return DeeXAst_CollectTopScopeChildren(self->ast_seq_range.sr_seq,scopes);

  case DEE_XASTKIND_ATTR_SET_C:
   if DEE_UNLIKELY(DeeXAst_CollectTopScopeChildren(self->ast_attr_set_c.ac_value,scopes) != 0) return -1;
   DEE_ATTRIBUTE_FALLTHROUGH
  case DEE_XASTKIND_ATTR_GET_C:
  case DEE_XASTKIND_ATTR_HAS_C:
  case DEE_XASTKIND_ATTR_DEL_C:
   return DeeXAst_CollectTopScopeChildren(self->ast_attr_c.ac_object,scopes);

#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
  case DEE_XASTKIND_FOREIGN_FUNCTION: {
   DeeXAstObject **iter,**end;
   if DEE_UNLIKELY(DeeXAst_CollectTopScopeChildren(self->ast_foreignfunction.ff_base,scopes) != 0) return -1;
   end = (iter = self->ast_foreignfunction.ff_argv)+self->ast_foreignfunction.ff_argc;
   while (iter != end) {
    if DEE_UNLIKELY(DeeXAst_CollectTopScopeChildren(*iter,scopes) != 0) return -1;
    ++iter;
   }
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */

  case DEE_XASTKIND_SWITCH: {
   struct DeeXAstSwitchEntry *iter,*end;
   if DEE_UNLIKELY(DeeXAst_CollectTopScopeChildren(self->ast_switch.s_expr,scopes) != 0) return -1;
   end = (iter = self->ast_switch.s_entryv)+self->ast_switch.s_entryc;
   DEE_ASSERT(iter != end);
   do if DEE_UNLIKELY(DeeXAst_CollectTopScopeChildren(iter->se_value,scopes) != 0) return -1;
   while (++iter != end);
   if DEE_UNLIKELY(self->ast_switch.s_default && DeeXAst_CollectTopScopeChildren(
    self->ast_switch.s_default,scopes) != 0) return -1;
  } break;

  case DEE_XASTKIND_IFTRUE:
  case DEE_XASTKIND_IFFALSE: {
   if DEE_UNLIKELY(self->ast_ifconst.ic_succ && DeeXAst_CollectTopScopeChildren(self->ast_ifconst.ic_succ,scopes) != 0) return -1;
   if DEE_UNLIKELY(self->ast_ifconst.ic_fail && DeeXAst_CollectTopScopeChildren(self->ast_ifconst.ic_fail,scopes) != 0) return -1;
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
  case DEE_XASTKIND_BUILTIN_EXTERN:
   if DEE_UNLIKELY(self->ast_builtin_extern.ex_file && DeeXAst_CollectTopScopeChildren(self->ast_builtin_extern.ex_file,scopes) != 0) return -1;
   if DEE_UNLIKELY(DeeXAst_CollectTopScopeChildren(self->ast_builtin_extern.ex_name,scopes) != 0) return -1;
   return DeeXAst_CollectTopScopeChildren(self->ast_builtin_extern.ex_type,scopes);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
  case DEE_XASTKIND_BUILTIN_EXPECT:
   return DeeXAst_CollectTopScopeChildren(self->ast_builtin_expect.e_value,scopes);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */

  default: if (DEE_XASTKIND_ISOPERATOR(self->ast_kind)) {
   switch (DEE_XASTKIND_OPCOUNT(self->ast_kind)) {
    case 3: if DEE_UNLIKELY(DeeXAst_CollectTopScopeChildren(self->ast_operator.op_c,scopes) != 0) return -1; DEE_ATTRIBUTE_FALLTHROUGH
    case 2: if DEE_UNLIKELY(DeeXAst_CollectTopScopeChildren(self->ast_operator.op_b,scopes) != 0) return -1; DEE_ATTRIBUTE_FALLTHROUGH
    case 1: if DEE_UNLIKELY(DeeXAst_CollectTopScopeChildren(self->ast_operator.op_a,scopes) != 0) return -1; DEE_ATTRIBUTE_FALLTHROUGH
    default: break;
   }
  } break;
 }
 return 0;
}

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
DEE_A_RET_EXCEPT(-1) int DeeXAstClassAstEntry_CollectTopScopeChildren(
 DEE_A_IN struct DeeXAstClassAstEntry const *self, DEE_A_INOUT struct DeeScopeList *scopes) {
 DEE_ASSERT(self);
 DEE_ASSERT(scopes);
 switch (self->ce_kind) {

  case DEE_XAST_CLASSENTRY_KIND_SLOT:
   return DeeXAst_CollectTopScopeChildren(self->ce_slot.cs_callback,scopes);

  case DEE_XAST_CLASSENTRY_KIND_METHOD:
  case DEE_XAST_CLASSENTRY_KIND_CLASS_METHOD:
   return DeeXAst_CollectTopScopeChildren(self->ce_method.cm_callback,scopes);

  case DEE_XAST_CLASSENTRY_KIND_GETSET:
  case DEE_XAST_CLASSENTRY_KIND_CLASS_GETSET:
   if DEE_UNLIKELY(self->ce_getset.cg_get && DeeXAst_CollectTopScopeChildren(self->ce_getset.cg_get,scopes) != 0) return -1;
   if DEE_UNLIKELY(self->ce_getset.cg_del && DeeXAst_CollectTopScopeChildren(self->ce_getset.cg_del,scopes) != 0) return -1;
   if DEE_UNLIKELY(self->ce_getset.cg_set && DeeXAst_CollectTopScopeChildren(self->ce_getset.cg_set,scopes) != 0) return -1;
   break;

  default: break;
 }
 return 0;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */

DEE_A_RET_EXCEPT(-1) int DeeVarDeclStorage_CollectTopScopeChildren(
 DEE_A_IN struct DeeVarDeclStorage const *self,
 DEE_A_INOUT struct DeeScopeList *scopes) {
 DEE_ASSERT(self);
 DEE_ASSERT(scopes);
 switch (self->vds_kind) {

  case DEE_VARDECLSTORAGEKIND_ATTR:
   if DEE_UNLIKELY(DeeXAst_CollectTopScopeChildren(self->vds_attr.as_object,scopes) != 0) return -1;
   return DeeXAst_CollectTopScopeChildren(self->vds_attr.as_attr,scopes);

  case DEE_VARDECLSTORAGEKIND_ATTR_C:
   return DeeXAst_CollectTopScopeChildren(self->vds_attr_c.acs_object,scopes);

  case DEE_VARDECLSTORAGEKIND_ITEM:
   if DEE_UNLIKELY(DeeXAst_CollectTopScopeChildren(self->vds_item.is_object,scopes) != 0) return -1;
   return DeeXAst_CollectTopScopeChildren(self->vds_item.is_key,scopes);

  case DEE_VARDECLSTORAGEKIND_RANGE:
   if DEE_UNLIKELY(self->vds_range.rs_begin && DeeXAst_CollectTopScopeChildren(self->vds_range.rs_begin,scopes) != 0) return -1;
   if DEE_UNLIKELY(self->vds_range.rs_end && DeeXAst_CollectTopScopeChildren(self->vds_range.rs_end,scopes) != 0) return -1;
   return DeeXAst_CollectTopScopeChildren(self->vds_range.rs_object,scopes);

  case DEE_VARDECLSTORAGEKIND_EXPAND: {
   struct DeeVarDeclStorage *iter,*end;
   end = (iter = self->vds_expand.es_declv)+self->vds_expand.es_declc;
   do if DEE_UNLIKELY(DeeVarDeclStorage_CollectTopScopeChildren(iter,scopes) != 0) return -1;
   while (++iter != end);
  } break;

  case DEE_VARDECLSTORAGEKIND_ASSIGN:
   return DeeXAst_CollectTopScopeChildren(self->vds_assign.as_ast,scopes);

  default: break;
 }
 return 0;
}


DEE_DECL_END
