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
#define DO_COUNT
#endif

#ifdef DO_COUNT
#ifdef DO_STORE
#define CHECK_VAR(v)    do{}while(0)
#define CHECK_VAR_L(v)  do{}while(0)
#define CHECK_VAR_S(v)  do if((v)==variable)++result;while(0)
#define CHECK_VAR_LS(v) do if((v)==variable)++result;while(0)
#define CHECK_XAST(x)   result+=DeeXAst_CountVariableStores(x,variable)
#define CHECK_SAST(x)   result+=DeeSAst_CountVariableStores(x,variable)
#define CHECK_VSTO(x)   result+=DeeVarDeclStorage_CountVariableStores(x,variable)
#elif defined(DO_LOAD)
#define CHECK_VAR(v)    do{}while(0)
#define CHECK_VAR_L(v)  do if((v)==variable)++result;while(0)
#define CHECK_VAR_S(v)  do{}while(0)
#define CHECK_VAR_LS(v) do if((v)==variable)++result;while(0)
#define CHECK_XAST(x)   result+=DeeXAst_CountVariableLoads(x,variable)
#define CHECK_SAST(x)   result+=DeeSAst_CountVariableLoads(x,variable)
#define CHECK_VSTO(x)   result+=DeeVarDeclStorage_CountVariableLoads(x,variable)
#else
#define CHECK_VAR(v)    do if((v)==variable)++result;while(0)
#define CHECK_VAR_L(v)  do if((v)==variable)++result;while(0)
#define CHECK_VAR_S(v)  do if((v)==variable)++result;while(0)
#define CHECK_VAR_LS(v) do if((v)==variable)++result;while(0)
#define CHECK_XAST(x)   result+=DeeXAst_CountVariableUses(x,variable)
#define CHECK_SAST(x)   result+=DeeSAst_CountVariableUses(x,variable)
#define CHECK_VSTO(x)   result+=DeeVarDeclStorage_CountVariableUses(x,variable)
#endif
#else
#ifdef DO_STORE
#define CHECK_VAR(v)    do{}while(0)
#define CHECK_VAR_L(v)  do{}while(0)
#define CHECK_VAR_S(v)  do if((v)==variable)return 1;while(0)
#define CHECK_VAR_LS(v) do if((v)==variable)return 1;while(0)
#define CHECK_XAST(x)   do if(DeeXAst_StoresVariable(x,variable))return 1;while(0)
#define CHECK_SAST(x)   do if(DeeSAst_StoresVariable(x,variable))return 1;while(0)
#define CHECK_VSTO(x)   do if(DeeVarDeclStorage_StoresVariable(x,variable))return 1;while(0)
#elif defined(DO_LOAD)
#define CHECK_VAR(v)    do{}while(0)
#define CHECK_VAR_L(v)  do if((v)==variable)return 1;while(0)
#define CHECK_VAR_S(v)  do{}while(0)
#define CHECK_VAR_LS(v) do if((v)==variable)return 1;while(0)
#define CHECK_XAST(x)   do if(DeeXAst_LoadsVariable(x,variable))return 1;while(0)
#define CHECK_SAST(x)   do if(DeeSAst_LoadsVariable(x,variable))return 1;while(0)
#define CHECK_VSTO(x)   do if(DeeVarDeclStorage_LoadsVariable(x,variable))return 1;while(0)
#else
#define CHECK_VAR(v)    do if((v)==variable)return 1;while(0)
#define CHECK_VAR_L(v)  do if((v)==variable)return 1;while(0)
#define CHECK_VAR_S(v)  do if((v)==variable)return 1;while(0)
#define CHECK_VAR_LS(v) do if((v)==variable)return 1;while(0)
#define CHECK_XAST(x)   do if(DeeXAst_UsesVariable(x,variable))return 1;while(0)
#define CHECK_SAST(x)   do if(DeeSAst_UsesVariable(x,variable))return 1;while(0)
#define CHECK_VSTO(x)   do if(DeeVarDeclStorage_UsesVariable(x,variable))return 1;while(0)
#endif
#endif

DEE_DECL_BEGIN

#ifdef DO_COUNT
#ifdef DO_STORE
DEE_A_RET_WUNUSED Dee_size_t DeeXAst_CountVariableStores
#elif defined(DO_LOAD)
DEE_A_RET_WUNUSED Dee_size_t DeeXAst_CountVariableLoads
#else
DEE_A_RET_WUNUSED Dee_size_t DeeXAst_CountVariableUses
#endif
#else
#ifdef DO_STORE
DEE_A_RET_WUNUSED int DeeXAst_StoresVariable
#elif defined(DO_LOAD)
DEE_A_RET_WUNUSED int DeeXAst_LoadsVariable
#else
DEE_A_RET_WUNUSED int DeeXAst_UsesVariable
#endif
#endif
(DEE_A_IN DeeXAstObject const *self,
 DEE_A_IN DeeLocalVarObject const *variable) {
#ifdef DO_COUNT
 Dee_size_t result = 0;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeXAst_Check(self));
 DEE_ASSERT(DeeObject_Check(variable) && DeeLocalVar_Check(variable));
 switch (self->ast_kind) {
  case DEE_XASTKIND_VAR:
   CHECK_VAR_L(self->ast_var.vs_var);
   break;
  case DEE_XASTKIND_VARDECL:
   CHECK_VAR_S(self->ast_vardecl.vd_var);
   CHECK_XAST(self->ast_vardecl.vd_init);
   break;
  case DEE_XASTKIND_MULTIVARDECL: {
   struct DeeVarDeclStorage *iter,*end;
   CHECK_XAST(self->ast_multivardecl.mvd_init);
   end = (iter = self->ast_multivardecl.mvd_varv)+self->ast_multivardecl.mvd_varc;
   DEE_ASSERT(self->ast_multivardecl.mvd_varc);
   do {
    CHECK_VSTO(iter);
   } while (++iter != end);
  } break;

  case DEE_XASTKIND_STATEMENT:
   CHECK_SAST(self->ast_statement.s_stmt);
   break;

  case DEE_XASTKIND_IF:
   CHECK_XAST(self->ast_if.if_cond);
   if (self->ast_if.if_succ) CHECK_XAST(self->ast_if.if_succ);
   if (self->ast_if.if_fail) CHECK_XAST(self->ast_if.if_fail);
   break;

  case DEE_XASTKIND_FUNCTION: {
   struct DeeXAstFunctionAstArgument *iter,*end;
   end = (iter = self->ast_function.f_argv)+self->ast_function.f_argc;
   while (iter != end) { if (iter->a_default) CHECK_XAST(iter->a_default); ++iter; }
   if (self->ast_function.f_code) CHECK_SAST(self->ast_function.f_code);
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case DEE_XASTKIND_CLASS: {
   struct DeeXAstClassAstEntry *iter,*end;
   if (self->ast_class.c_base) CHECK_XAST(self->ast_class.c_base);
   end = (iter = self->ast_class.c_entryv)+self->ast_class.c_entryc;
   while (iter != end) {
    switch (iter->ce_kind) {
     case DEE_XAST_CLASSENTRY_KIND_SLOT:
      CHECK_XAST(iter->ce_slot.cs_callback);
      break;
     case DEE_XAST_CLASSENTRY_KIND_METHOD:
     case DEE_XAST_CLASSENTRY_KIND_CLASS_METHOD:
      CHECK_XAST(iter->ce_method.cm_callback);
      break;
     case DEE_XAST_CLASSENTRY_KIND_GETSET:
     case DEE_XAST_CLASSENTRY_KIND_CLASS_GETSET:
      if (iter->ce_getset.cg_get) CHECK_XAST(iter->ce_getset.cg_get);
      if (iter->ce_getset.cg_del) CHECK_XAST(iter->ce_getset.cg_del);
      if (iter->ce_getset.cg_set) CHECK_XAST(iter->ce_getset.cg_set);
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
   do CHECK_XAST(*iter); while (++iter != end);
  } break;

  case DEE_XASTKIND_DICT: {
   struct DeeXAstDictEntry *iter,*end;
   end = (iter = self->ast_dict.d_elemv)+self->ast_dict.d_elemc;
   DEE_ASSERT(self->ast_dict.d_elemv != NULL);
   DEE_ASSERT(self->ast_dict.d_elemc != 0);
   do {
    CHECK_XAST(iter->de_key);
    CHECK_XAST(iter->de_item);
   } while (++iter != end);
  } break;

  case DEE_XASTKIND_SEQ_RANGE_SET:
   CHECK_XAST(self->ast_seq_range_set.sr_value);
   DEE_ATTRIBUTE_FALLTHROUGH
  case DEE_XASTKIND_SEQ_RANGE_GET:
  case DEE_XASTKIND_SEQ_RANGE_DEL:
   CHECK_XAST(self->ast_seq_range.sr_seq);
   if (self->ast_seq_range.sr_begin) CHECK_XAST(self->ast_seq_range.sr_begin);
   if (self->ast_seq_range.sr_end) CHECK_XAST(self->ast_seq_range.sr_end);
   break;

  case DEE_XASTKIND_ATTR_SET_C:
   CHECK_XAST(self->ast_attr_set_c.ac_value);
   DEE_ATTRIBUTE_FALLTHROUGH
  case DEE_XASTKIND_ATTR_GET_C:
  case DEE_XASTKIND_ATTR_HAS_C:
  case DEE_XASTKIND_ATTR_DEL_C:
   CHECK_XAST(self->ast_attr_c.ac_object);
   break;

  case DEE_XASTKIND_DEL_VAR:
   CHECK_VAR(self->ast_delvar.d_var);
   break;

#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
  case DEE_XASTKIND_FOREIGN_FUNCTION: {
   DeeXAstObject **iter,**end;
   CHECK_XAST(self->ast_foreignfunction.ff_base);
   end = (iter = self->ast_foreignfunction.ff_argv)+self->ast_foreignfunction.ff_argc;
   while (iter != end) CHECK_XAST(*iter++);
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */

  case DEE_XASTKIND_SWITCH: {
   struct DeeXAstSwitchEntry *iter,*end;
   CHECK_XAST(self->ast_switch.s_expr);
   end = (iter = self->ast_switch.s_entryv)+self->ast_switch.s_entryc;
   DEE_ASSERT(iter != end);
   do CHECK_XAST(iter->se_value); while (++iter != end);
   if (self->ast_switch.s_default) CHECK_XAST(self->ast_switch.s_default);
  } break;

  case DEE_XASTKIND_IFTRUE:
  case DEE_XASTKIND_IFFALSE: {
   if (self->ast_ifconst.ic_succ) CHECK_XAST(self->ast_ifconst.ic_succ);
   if (self->ast_ifconst.ic_fail) CHECK_XAST(self->ast_ifconst.ic_fail);
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case DEE_XASTKIND_BUILTIN_BOUND:
   CHECK_VAR(self->ast_builtin_bound.b_var);
   break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */

#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
  case DEE_XASTKIND_BUILTIN_EXTERN:
   if (self->ast_builtin_extern.ex_file) CHECK_XAST(self->ast_builtin_extern.ex_file);
   CHECK_XAST(self->ast_builtin_extern.ex_name);
   CHECK_XAST(self->ast_builtin_extern.ex_type);
   break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
  case DEE_XASTKIND_BUILTIN_EXPECT:
   CHECK_XAST(self->ast_builtin_expect.e_value);
   break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */

  DEE_XASTKIND_CASE_INPLACE_VAR_UNARY
  DEE_XASTKIND_CASE_INPLACE_VAR_BINARY
   CHECK_VAR_LS(self->ast_unary_var.uiv_var);
   break;

  default: if (DEE_XASTKIND_ISOPERATOR(self->ast_kind)) {
   switch (DEE_XASTKIND_OPCOUNT(self->ast_kind)) {
    case 3: CHECK_XAST(self->ast_operator.op_c); DEE_ATTRIBUTE_FALLTHROUGH
    case 2: CHECK_XAST(self->ast_operator.op_b); DEE_ATTRIBUTE_FALLTHROUGH
    case 1: CHECK_XAST(self->ast_operator.op_a); DEE_ATTRIBUTE_FALLTHROUGH
    default: break;
   }
  } break;
 }
#ifdef DO_COUNT
 return result;
#else
 return 0;
#endif
}

#ifdef DO_COUNT
#ifdef DO_STORE
DEE_A_RET_WUNUSED Dee_size_t DeeVarDeclStorage_CountVariableStores
#elif defined(DO_LOAD)
DEE_A_RET_WUNUSED Dee_size_t DeeVarDeclStorage_CountVariableLoads
#else
DEE_A_RET_WUNUSED Dee_size_t DeeVarDeclStorage_CountVariableUses
#endif
#else
#ifdef DO_STORE
DEE_A_RET_WUNUSED int DeeVarDeclStorage_StoresVariable
#elif defined(DO_LOAD)
DEE_A_RET_WUNUSED int DeeVarDeclStorage_LoadsVariable
#else
DEE_A_RET_WUNUSED int DeeVarDeclStorage_UsesVariable
#endif
#endif
(DEE_A_IN struct DeeVarDeclStorage const *self,
 DEE_A_IN DeeLocalVarObject const *variable) {
#ifdef DO_COUNT
 Dee_size_t result = 0;
#endif
 switch (self->vds_kind) {
  case DEE_VARDECLSTORAGEKIND_VAR:
   CHECK_VAR_S(self->vds_var.vs_var);
   break;
  case DEE_VARDECLSTORAGEKIND_ATTR:
   CHECK_XAST(self->vds_attr.as_object);
   CHECK_XAST(self->vds_attr.as_attr);
   break;
  case DEE_VARDECLSTORAGEKIND_ATTR_C:
   CHECK_XAST(self->vds_attr_c.acs_object);
   break;
  case DEE_VARDECLSTORAGEKIND_ITEM:
   CHECK_XAST(self->vds_item.is_object);
   CHECK_XAST(self->vds_item.is_key);
  case DEE_VARDECLSTORAGEKIND_RANGE:
   CHECK_XAST(self->vds_range.rs_object);
   if (self->vds_range.rs_begin) CHECK_XAST(self->vds_range.rs_begin);
   if (self->vds_range.rs_end) CHECK_XAST(self->vds_range.rs_end);
   break;
  case DEE_VARDECLSTORAGEKIND_EXPAND: {
   struct DeeVarDeclStorage *iter,*end;
   end = (iter = self->vds_expand.es_declv)+self->vds_expand.es_declc;
   DEE_ASSERT(self->vds_expand.es_declc);
   do {
    CHECK_VSTO(iter);
   } while (++iter != end);
  } break;
  case DEE_VARDECLSTORAGEKIND_ASSIGN:
   CHECK_XAST(self->vds_assign.as_ast);
   break;
  default: break;
 }
#ifdef DO_COUNT
 return result;
#else
 return 0;
#endif
}

#undef CHECK_SAST
#undef CHECK_XAST
#undef CHECK_VAR
#undef CHECK_VSTO
#undef CHECK_VAR_L
#undef CHECK_VAR_S
#undef CHECK_VAR_LS

#ifdef DO_STORE
#undef DO_STORE
#endif
#ifdef DO_LOAD
#undef DO_LOAD
#endif
#ifdef DO_COUNT
#undef DO_COUNT
#endif

DEE_DECL_END
