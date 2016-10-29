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

DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeXAst_Equals(
 DEE_A_IN DeeXAstObject const *self,
 DEE_A_IN DeeXAstObject const *right) {
 int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeXAst_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeXAst_Check(right));
 if DEE_UNLIKELY(self == right) return 1;
 if DEE_LIKELY(self->ast_kind != right->ast_kind) return 0;
 switch (self->ast_kind) {
  case DEE_XASTKIND_CONST:
   if (Dee_TYPE(self->ast_const.c_const) != Dee_TYPE(right->ast_const.c_const)) return 0;
   return DeeObject_DeepEquals(self->ast_const.c_const,right->ast_const.c_const);
  case DEE_XASTKIND_VAR:
   return self->ast_var.vs_var == right->ast_var.vs_var;
  case DEE_XASTKIND_VARDECL:
   if (self->ast_vardecl.vd_var != right->ast_vardecl.vd_var) return 0;
   return DeeXAst_Equals(self->ast_vardecl.vd_init,right->ast_vardecl.vd_init);
  case DEE_XASTKIND_MULTIVARDECL: {
   struct DeeVarDeclStorage *iter,*end,*rhs_iter;
   if (self->ast_multivardecl.mvd_varc != right->ast_multivardecl.mvd_varc) return 0;
   end = (iter = self->ast_multivardecl.mvd_varv)+self->ast_multivardecl.mvd_varc;
   rhs_iter = right->ast_multivardecl.mvd_varv;
   while (iter != end) {
    if ((temp = DeeVarDeclStorage_Equals(iter,rhs_iter)) <= 0) return temp;
    ++iter,++rhs_iter;
   }
   return DeeXAst_Equals(self->ast_multivardecl.mvd_init,right->ast_multivardecl.mvd_init);
  } break;
  case DEE_XASTKIND_STATEMENT:
   return DeeSAst_Equals(self->ast_statement.s_stmt,right->ast_statement.s_stmt);
  case DEE_XASTKIND_IF:
   if ((self->ast_if.if_succ != NULL) != (right->ast_if.if_succ != NULL)) return 0;
   if ((self->ast_if.if_fail != NULL) != (right->ast_if.if_fail != NULL)) return 0;
   if ((temp = DeeXAst_Equals(self->ast_if.if_cond,right->ast_if.if_cond)) <= 0) return temp;
   if (self->ast_if.if_succ && (temp = DeeXAst_Equals(self->ast_if.if_succ,right->ast_if.if_succ)) <= 0) return temp;
   if (self->ast_if.if_fail && (temp = DeeXAst_Equals(self->ast_if.if_fail,right->ast_if.if_fail)) <= 0) return temp;
   break;
  case DEE_XASTKIND_FUNCTION: {
   // NOTE: Since when comparing the function code different variables
   //       will cause equality to be denied, we can already predict
   //       that any arguments at all will prevent the asts from being equal.
   //       -> Though whether or not that's the correct way of dealing with this is another question...
   if (self->ast_function.f_argc || right->ast_function.f_argc) return 0;
   if (self->ast_function.f_argc != right->ast_function.f_argc) return 0;
   if (self->ast_function.f_flags != right->ast_function.f_flags) return 0;
   // NOTE: The function name does actually count!
   if (self->ast_function.f_name) {
    if (!right->ast_function.f_name) return 0;
    DEE_ASSERT(DeeObject_Check(self->ast_function.f_name) && DeeString_Check(self->ast_function.f_name));
    DEE_ASSERT(DeeObject_Check(right->ast_function.f_name) && DeeString_Check(right->ast_function.f_name));
    if (DeeString_SIZE(self->ast_function.f_name) != DeeString_SIZE(right->ast_function.f_name)) return 0;
    if (memcmp(DeeString_STR(self->ast_function.f_name),DeeString_STR(right->ast_function.f_name),
     DeeString_SIZE(self->ast_function.f_name)*sizeof(char)) != 0) return 0;
   } else {
    if (right->ast_function.f_name) return 0;
   }
   return DeeSAst_Equals(self->ast_function.f_code,right->ast_function.f_code);
  } break;
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case DEE_XASTKIND_CLASS: {
   struct DeeXAstClassAstEntry *iter,*end,*rhs_iter;
   if (self->ast_class.c_entryc != right->ast_class.c_entryc) return 0;
   if (self->ast_class.c_tpflags != right->ast_class.c_tpflags) return 0;
   if ((self->ast_class.c_tpflags&DEE_XAST_CLASSAST_FLAG_HAS_UUID)!=0 &&
       !DeeUUID_EQUAL(&self->ast_class.c_uuid,&right->ast_class.c_uuid)) return 0;
   if (self->ast_class.c_name) {
    if (!right->ast_class.c_name) return 0;
    DEE_ASSERT(DeeObject_Check(self->ast_class.c_name) && DeeString_Check(self->ast_class.c_name));
    DEE_ASSERT(DeeObject_Check(right->ast_class.c_name) && DeeString_Check(right->ast_class.c_name));
    if (DeeString_SIZE(self->ast_class.c_name) != DeeString_SIZE(right->ast_class.c_name)) return 0;
    if (memcmp(DeeString_STR(self->ast_class.c_name),DeeString_STR(right->ast_class.c_name),
     DeeString_SIZE(self->ast_class.c_name)*sizeof(char)) != 0) return 0;
   } else {
    if (right->ast_class.c_name) return 0;
   }
   if (self->ast_class.c_base) {
    if (!right->ast_class.c_base) return 0;
    if ((temp = DeeXAst_Equals(self->ast_class.c_base,right->ast_class.c_base)) <= 0) return temp;
   } else {
    if (right->ast_class.c_base) return 0;
   }
   end = (iter = self->ast_class.c_entryv)+self->ast_class.c_entryc;
   rhs_iter = right->ast_class.c_entryv;
   while (iter != end) {
    if (iter->ce_kind != rhs_iter->ce_kind) return 0;
    switch (iter->ce_kind) {
     case DEE_XAST_CLASSENTRY_KIND_SLOT:
      if (iter->ce_slot.cs_slotid != rhs_iter->ce_slot.cs_slotid) return 0;
      if ((temp = DeeXAst_Equals(iter->ce_slot.cs_callback,rhs_iter->ce_slot.cs_callback)) <= 0) return temp;
      break;
     case DEE_XAST_CLASSENTRY_KIND_METHOD:
     case DEE_XAST_CLASSENTRY_KIND_CLASS_METHOD:
      DEE_ASSERT(DeeObject_Check(iter->ce_method.cm_name) && DeeString_Check(iter->ce_method.cm_name));
      DEE_ASSERT(DeeObject_Check(rhs_iter->ce_method.cm_name) && DeeString_Check(rhs_iter->ce_method.cm_name));
      if (DeeString_SIZE(iter->ce_method.cm_name) != DeeString_SIZE(rhs_iter->ce_method.cm_name)) return 0;
      if (memcmp(DeeString_STR(iter->ce_method.cm_name),DeeString_STR(rhs_iter->ce_method.cm_name),
       DeeString_SIZE(iter->ce_method.cm_name)*sizeof(char)) != 0) return 0;
      if ((temp = DeeXAst_Equals(iter->ce_method.cm_callback,rhs_iter->ce_method.cm_callback)) <= 0) return 0;
      break;
     case DEE_XAST_CLASSENTRY_KIND_GETSET:
     case DEE_XAST_CLASSENTRY_KIND_CLASS_GETSET:
      DEE_ASSERT(DeeObject_Check(iter->ce_getset.cg_name) && DeeString_Check(iter->ce_getset.cg_name));
      DEE_ASSERT(DeeObject_Check(rhs_iter->ce_getset.cg_name) && DeeString_Check(rhs_iter->ce_getset.cg_name));
      if (DeeString_SIZE(iter->ce_getset.cg_name) != DeeString_SIZE(rhs_iter->ce_getset.cg_name)) return 0;
      if (memcmp(DeeString_STR(iter->ce_getset.cg_name),DeeString_STR(rhs_iter->ce_getset.cg_name),
       DeeString_SIZE(iter->ce_getset.cg_name)*sizeof(char)) != 0) return 0;
      if ((iter->ce_getset.cg_get != NULL) != (rhs_iter->ce_getset.cg_get != NULL)) return 0;
      if ((iter->ce_getset.cg_del != NULL) != (rhs_iter->ce_getset.cg_del != NULL)) return 0;
      if ((iter->ce_getset.cg_set != NULL) != (rhs_iter->ce_getset.cg_set != NULL)) return 0;
      if (iter->ce_getset.cg_get && (temp = DeeXAst_Equals(iter->ce_getset.cg_get,rhs_iter->ce_getset.cg_get)) <= 0) return temp;
      if (iter->ce_getset.cg_del && (temp = DeeXAst_Equals(iter->ce_getset.cg_del,rhs_iter->ce_getset.cg_del)) <= 0) return temp;
      if (iter->ce_getset.cg_set && (temp = DeeXAst_Equals(iter->ce_getset.cg_set,rhs_iter->ce_getset.cg_set)) <= 0) return temp;
      break;
     default: break;
    }
    ++iter,++rhs_iter;
   }
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
  case DEE_XASTKIND_TUPLE:
  case DEE_XASTKIND_LIST:
  case DEE_XASTKIND_SET: {
   DeeXAstObject **iter,**end,**rhs_iter;
   if (self->ast_sequence.s_elemc != right->ast_sequence.s_elemc) return 0;
   end = (iter = self->ast_sequence.s_elemv)+self->ast_sequence.s_elemc;
   rhs_iter = right->ast_sequence.s_elemv;
   while (iter != end) if ((temp = DeeXAst_Equals(*iter++,*rhs_iter++)) <= 0) return temp;
  } break;
  case DEE_XASTKIND_DICT: {
   struct DeeXAstDictEntry *iter,*end,*rhs_iter;
   if (self->ast_dict.d_elemc != right->ast_dict.d_elemc) return 0;
   end = (iter = self->ast_dict.d_elemv)+self->ast_dict.d_elemc;
   rhs_iter = right->ast_dict.d_elemv;
   while (iter != end) {
    if ((temp = DeeXAst_Equals(iter->de_key,rhs_iter->de_key)) <= 0) return temp;
    if ((temp = DeeXAst_Equals(iter->de_item,rhs_iter->de_item)) <= 0) return temp;
    ++iter,++rhs_iter;
   }
  } break;
  case DEE_XASTKIND_SEQ_RANGE_SET:
   if ((temp = DeeXAst_Equals(self->ast_seq_range_set.sr_value,right->ast_seq_range_set.sr_value)) <= 0) return temp;
  case DEE_XASTKIND_SEQ_RANGE_GET:
  case DEE_XASTKIND_SEQ_RANGE_DEL:
   if ((self->ast_seq_range.sr_begin != NULL) != (right->ast_seq_range.sr_begin != NULL)) return 0;
   if ((self->ast_seq_range.sr_end != NULL) != (right->ast_seq_range.sr_end != NULL)) return 0;
   if ((temp = DeeXAst_Equals(self->ast_seq_range.sr_seq,right->ast_seq_range.sr_seq)) <= 0) return temp;
   if (self->ast_seq_range.sr_begin && (temp = DeeXAst_Equals(self->ast_seq_range.sr_begin,right->ast_seq_range.sr_begin)) <= 0) return temp;
   if (self->ast_seq_range.sr_end && (temp = DeeXAst_Equals(self->ast_seq_range.sr_end,right->ast_seq_range.sr_end)) <= 0) return temp;
   break;
  case DEE_XASTKIND_ATTR_SET_C:
   if ((temp = DeeXAst_Equals(self->ast_attr_set_c.ac_value,right->ast_attr_set_c.ac_value)) <= 0) return temp;
  case DEE_XASTKIND_ATTR_GET_C:
  case DEE_XASTKIND_ATTR_HAS_C:
  case DEE_XASTKIND_ATTR_DEL_C:
   DEE_ASSERT(DeeObject_Check(self->ast_attr_c.ac_name) && DeeString_Check(self->ast_attr_c.ac_name));
   DEE_ASSERT(DeeObject_Check(right->ast_attr_c.ac_name) && DeeString_Check(right->ast_attr_c.ac_name));
   if (DeeString_SIZE(self->ast_attr_c.ac_name) != DeeString_SIZE(right->ast_attr_c.ac_name)) return 0;
   if (memcmp(DeeString_STR(self->ast_attr_c.ac_name),DeeString_STR(right->ast_attr_c.ac_name),
    DeeString_SIZE(self->ast_attr_c.ac_name)*sizeof(char)) != 0) return 0;
   return DeeXAst_Equals(self->ast_attr_c.ac_object,right->ast_attr_c.ac_object);
  case DEE_XASTKIND_MODULE:
   return self->ast_module.m_module == right->ast_module.m_module;
  case DEE_XASTKIND_DEL_VAR:
   return self->ast_delvar.d_var == right->ast_delvar.d_var;
#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
  case DEE_XASTKIND_FOREIGN_FUNCTION: {
   DeeXAstObject **iter,**end,**rhs_iter;
   if (self->ast_foreignfunction.ff_argc != right->ast_foreignfunction.ff_argc) return 0;
   if (self->ast_foreignfunction.ff_flags != right->ast_foreignfunction.ff_flags) return 0;
   if ((temp = DeeXAst_Equals(self->ast_foreignfunction.ff_base,right->ast_foreignfunction.ff_base)) <= 0) return temp;
   end = (iter = self->ast_foreignfunction.ff_argv)+self->ast_foreignfunction.ff_argc;
   rhs_iter = right->ast_foreignfunction.ff_argv;
   while (iter != end) if ((temp = DeeXAst_Equals(*iter++,*rhs_iter++)) <= 0) return 0;
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */
  case DEE_XASTKIND_SWITCH: {
   struct DeeXAstSwitchEntry *iter,*end,*rhs_iter;
   if (self->ast_switch.s_entryc != right->ast_switch.s_entryc) return 0;
   if ((temp = DeeXAst_Equals(self->ast_switch.s_expr,right->ast_switch.s_expr)) <= 0) return 0;
   if ((temp = DeeXAst_Equals(self->ast_switch.s_default,right->ast_switch.s_default)) <= 0) return 0;
   end = (iter = self->ast_switch.s_entryv)+self->ast_switch.s_entryc;
   rhs_iter = right->ast_switch.s_entryv;
   while (iter != end) {
    if ((temp = DeeObject_DeepEquals(iter->se_key,rhs_iter->se_key)
     ) != DEE_OBJECT_DEEPEQUALS_TRUE) return DEE_UNLIKELY(temp < 0) ? temp : 0;
    if ((temp = DeeXAst_Equals(iter->se_value,rhs_iter->se_value)) <= 0) return temp;
    ++iter,++rhs_iter;
   }
  } break;
  case DEE_XASTKIND_IFTRUE:
  case DEE_XASTKIND_IFFALSE:
   if ((self->ast_ifconst.ic_succ != NULL) != (right->ast_ifconst.ic_succ != NULL)) return 0;
   if ((self->ast_ifconst.ic_fail != NULL) != (right->ast_ifconst.ic_fail != NULL)) return 0;
   if (self->ast_ifconst.ic_succ && (temp = DeeXAst_Equals(self->ast_ifconst.ic_succ,right->ast_ifconst.ic_succ)) <= 0) return temp;
   if (self->ast_ifconst.ic_fail && (temp = DeeXAst_Equals(self->ast_ifconst.ic_fail,right->ast_ifconst.ic_fail)) <= 0) return temp;
   break;
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case DEE_XASTKIND_BUILTIN_BOUND:
   return self->ast_builtin_bound.b_var == right->ast_builtin_bound.b_var;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */
#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
  case DEE_XASTKIND_BUILTIN_EXTERN:
   if ((self->ast_builtin_extern.ex_file != NULL) != (right->ast_builtin_extern.ex_file != NULL)) return 0;
   if (self->ast_builtin_extern.ex_file && (temp = DeeXAst_Equals(self->ast_builtin_extern.ex_file,right->ast_builtin_extern.ex_file)) <= 0) return temp;
   if ((temp = DeeXAst_Equals(self->ast_builtin_extern.ex_name,right->ast_builtin_extern.ex_name)) <= 0) return temp;
   if ((temp = DeeXAst_Equals(self->ast_builtin_extern.ex_type,right->ast_builtin_extern.ex_type)) <= 0) return temp;
   break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
  case DEE_XASTKIND_BUILTIN_EXPECT:
   if ((temp = DeeXAst_Equals(self->ast_builtin_expect.e_value,right->ast_builtin_expect.e_value)) <= 0) return temp;
   if ((temp = DeeObject_DeepEquals(self->ast_builtin_expect.e_expect,right->ast_builtin_expect.e_expect)
    ) != DEE_OBJECT_DEEPEQUALS_TRUE) return DEE_UNLIKELY(temp < 0) ? temp : 0;
   break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */
  default:
   if (DEE_XASTKIND_ISOPERATOR(self->ast_kind)) {
    switch (DEE_XASTKIND_OPCOUNT(self->ast_kind)) {
     case 3: if ((temp = DeeXAst_Equals(self->ast_operator.op_c,right->ast_operator.op_c)) <= 0) return temp; DEE_ATTRIBUTE_FALLTHROUGH
     case 2: if ((temp = DeeXAst_Equals(self->ast_operator.op_b,right->ast_operator.op_b)) <= 0) return temp; DEE_ATTRIBUTE_FALLTHROUGH
     case 1: if ((temp = DeeXAst_Equals(self->ast_operator.op_a,right->ast_operator.op_a)) <= 0) return temp; DEE_ATTRIBUTE_FALLTHROUGH
     default: break;
    }
   }
   break;
 }
 return 1;
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeVarDeclStorage_Equals(
 DEE_A_IN struct DeeVarDeclStorage const *self,
 DEE_A_IN struct DeeVarDeclStorage const *right) {
 int temp;
 DEE_ASSERT(self); DEE_ASSERT(right);
 if DEE_UNLIKELY(self == right) return 1;
 if DEE_LIKELY(self->vds_kind != right->vds_kind) return 0;
 switch (self->vds_kind) {
  case DEE_VARDECLSTORAGEKIND_VAR:
   return self->vds_var.vs_var == right->vds_var.vs_var;
  case DEE_VARDECLSTORAGEKIND_ATTR:
   if ((temp = DeeXAst_Equals(self->vds_attr.as_object,right->vds_attr.as_object)) <= 0) return temp;
   if ((temp = DeeXAst_Equals(self->vds_attr.as_attr,right->vds_attr.as_attr)) <= 0) return temp;
   break;
  case DEE_VARDECLSTORAGEKIND_ATTR_C:
   if ((temp = DeeXAst_Equals(self->vds_attr_c.acs_object,right->vds_attr_c.acs_object)) <= 0) return temp;
   DEE_ASSERT(DeeObject_Check(self->vds_attr_c.acs_attr) && DeeString_Check(self->vds_attr_c.acs_attr));
   DEE_ASSERT(DeeObject_Check(right->vds_attr_c.acs_attr) && DeeString_Check(right->vds_attr_c.acs_attr));
   if (self->vds_attr_c.acs_attr == right->vds_attr_c.acs_attr) return 1;
   if (DeeString_SIZE(self->vds_attr_c.acs_attr) != DeeString_SIZE(right->vds_attr_c.acs_attr)) return 0;
   return memcmp(DeeString_STR(self->vds_attr_c.acs_attr),
                 DeeString_STR(right->vds_attr_c.acs_attr),
                 DeeString_SIZE(self->vds_attr_c.acs_attr)
                 *sizeof(char)) == 0;
  case DEE_VARDECLSTORAGEKIND_ITEM:
   if ((temp = DeeXAst_Equals(self->vds_item.is_object,right->vds_item.is_object)) <= 0) return temp;
   return DeeXAst_Equals(self->vds_item.is_key,right->vds_item.is_key);
  case DEE_VARDECLSTORAGEKIND_RANGE:
   if ((self->vds_range.rs_begin != NULL) != (right->vds_range.rs_begin != NULL)) return 0;
   if ((self->vds_range.rs_end != NULL) != (right->vds_range.rs_end != NULL)) return 0;
   if (self->vds_range.rs_begin && (temp = DeeXAst_Equals(self->vds_range.rs_begin,right->vds_range.rs_begin)) <= 0) return temp;
   if (self->vds_range.rs_end && (temp = DeeXAst_Equals(self->vds_range.rs_end,right->vds_range.rs_end)) <= 0) return temp;
   return DeeXAst_Equals(self->vds_range.rs_object,right->vds_range.rs_object);
  case DEE_VARDECLSTORAGEKIND_EXPAND: {
   struct DeeVarDeclStorage *iter,*end,*rhs_iter;
   if (self->vds_expand.es_declc != right->vds_expand.es_declc) return 0;
   end = (iter = self->vds_expand.es_declv)+self->vds_expand.es_declc;
   rhs_iter = right->vds_expand.es_declv;
   while (iter != end) if ((temp = DeeVarDeclStorage_Equals(iter++,rhs_iter++)) <= 0) return temp;
  } break;
  case DEE_VARDECLSTORAGEKIND_ASSIGN:
   return DeeXAst_Equals(self->vds_assign.as_ast,right->vds_assign.as_ast);
  default: break;
 }
 return 1;
}


DEE_DECL_END
