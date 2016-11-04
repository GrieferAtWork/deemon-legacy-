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

DEE_A_RET_WUNUSED DeeAstAttribute DeeXAst_AttrEx(
 DEE_A_IN DeeXAstObject const *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN DeeObject const *root_ast) {
 DeeAstAttribute result;
 DeeTypeObject const *ta,*tb;
 DEE_ASSERT(DeeObject_Check(self) && DeeXAst_Check(self));
 DEE_ASSERT(DeeObject_Check(root_ast) && (DeeXAst_Check(root_ast) || DeeSAst_Check(root_ast)));
 switch (self->ast_kind) {

  case DEE_XASTKIND_CONST:
  case DEE_XASTKIND_MODULE:
   return DEE_AST_ATTRIBUTE_NOOP;
  case DEE_XASTKIND_VAR:
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case DEE_XASTKIND_CLASS:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
  case DEE_XASTKIND_FUNCTION:
   // TODO: Consider attributes of class slot / member initializers
   return (DeeAstAttribute)((flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS)!=0
    ? (DEE_AST_ATTRIBUTE_NOEFFECT|DEE_AST_ATTRIBUTE_NOJUMP) // Could possibly throw an out-of-memory/unbound local error
    : (DEE_AST_ATTRIBUTE_NOEFFECT|DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOJUMP));

  case DEE_XASTKIND_VARDECL:
   return DeeXAst_AttrEx(self->ast_vardecl.vd_init,flags,root_ast
                         )&~(DEE_AST_ATTRIBUTE_NOEFFECT);
  case DEE_XASTKIND_MULTIVARDECL: {
   struct DeeVarDeclStorage *iter,*end;
   end = (iter = self->ast_multivardecl.mvd_varv)+self->ast_multivardecl.mvd_varc;
   DEE_ASSERT(self->ast_multivardecl.mvd_varc);
   result = DEE_AST_ATTRIBUTE_NOOP;
   while (iter != end) result = DeeAstAttribute_MergeSequential(
    result,DeeVarDeclStorage_AttrEx(iter++,flags,root_ast),flags);
   return DeeAstAttribute_MergeSequential(
    result,DeeXAst_AttrEx(self->ast_multivardecl.mvd_init,flags,root_ast),
    flags)&~(DEE_AST_ATTRIBUTE_NOEFFECT);
  } break;
  case DEE_XASTKIND_STATEMENT:
   return DeeSAst_AttrEx(self->ast_statement.s_stmt,flags,root_ast);
  case DEE_XASTKIND_IF:
   return DeeAstAttribute_MergeSequential(
    DeeXAst_AttrEx(self->ast_if.if_cond,flags,root_ast),
    DeeAstAttribute_MergeParallel(
     self->ast_if.if_succ ? DeeXAst_AttrEx(self->ast_if.if_succ,flags,root_ast) : DEE_AST_ATTRIBUTE_NOOP,
     self->ast_if.if_fail ? DeeXAst_AttrEx(self->ast_if.if_fail,flags,root_ast) : DEE_AST_ATTRIBUTE_NOOP,
     flags),flags);
  case DEE_XASTKIND_TUPLE:
  case DEE_XASTKIND_LIST:
  case DEE_XASTKIND_SET: {
   DeeXAstObject **iter,**end;
   result = DEE_AST_ATTRIBUTE_NOOP;
   end = (iter = self->ast_sequence.s_elemv)+self->ast_sequence.s_elemc;
   DEE_ASSERT(self->ast_sequence.s_elemc);
   do result = DeeAstAttribute_MergeSequential(
    result,DeeXAst_AttrEx(*iter,flags,root_ast),flags);
   while (++iter != end);
   return result;
  } break;
  case DEE_XASTKIND_DICT: {
   struct DeeXAstDictEntry *iter,*end;
   result = DEE_AST_ATTRIBUTE_NOOP;
   end = (iter = self->ast_dict.d_elemv)+self->ast_dict.d_elemc;
   DEE_ASSERT(self->ast_dict.d_elemv != NULL);
   DEE_ASSERT(self->ast_dict.d_elemc != 0);
   do result = DeeAstAttribute_MergeSequential(DeeAstAttribute_MergeSequential(result,
    DeeXAst_AttrEx(iter->de_key,flags,root_ast),flags),
    DeeXAst_AttrEx(iter->de_item,flags,root_ast),flags);
   while (++iter != end);
   return result;
  } break;

  case DEE_XASTKIND_SEQ_RANGE_GET:
  case DEE_XASTKIND_SEQ_RANGE_DEL:
  case DEE_XASTKIND_SEQ_RANGE_SET: {
   result = DeeXAst_AttrEx(self->ast_seq_range.sr_seq,flags,root_ast);
   if (self->ast_seq_range.sr_begin) result = DeeAstAttribute_MergeSequential(result,DeeXAst_AttrEx(self->ast_seq_range.sr_begin,flags,root_ast),flags);
   if (self->ast_seq_range.sr_end) result = DeeAstAttribute_MergeSequential(result,DeeXAst_AttrEx(self->ast_seq_range.sr_end,flags,root_ast),flags);
   if (self->ast_kind == DEE_XASTKIND_SEQ_RANGE_SET) result = DeeAstAttribute_MergeSequential(result,DeeXAst_AttrEx(self->ast_seq_range.sr_value,flags,root_ast),flags);
   if ((ta = DeeXAst_PredictType(self->ast_seq_range.sr_seq)) != NULL) {
    DeeTypeObject const *tbegin,*tend,*tvalue;
    tbegin = self->ast_seq_range.sr_begin ? DeeXAst_PredictType(self->ast_seq_range.sr_begin) : (DeeTypeObject *)&DeeNone_Type;
    tend = self->ast_seq_range.sr_end ? DeeXAst_PredictType(self->ast_seq_range.sr_end) : (DeeTypeObject *)&DeeNone_Type;
    switch (self->ast_kind) {
     case DEE_XASTKIND_SEQ_RANGE_GET: result = DeeAstAttribute_MergeSequential(result,DeeType_GetRangeAstAttr(ta,flags,tbegin,tend,NULL),flags); break;
     case DEE_XASTKIND_SEQ_RANGE_DEL: result = DeeAstAttribute_MergeSequential(result,DeeType_DelRangeAstAttr(ta,flags,tbegin,tend),flags); break;
     case DEE_XASTKIND_SEQ_RANGE_SET:
      tvalue = DeeXAst_PredictType(self->ast_seq_range.sr_value);
      result = DeeAstAttribute_MergeSequential(result,DeeType_SetRangeAstAttr(ta,flags,tbegin,tend,tvalue),flags);
      break;
     default: DEE_BUILTIN_UNREACHABLE();
    }
   } else result &= ~(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT);
   return result;
  } break;

  case DEE_XASTKIND_ATTR_GET_C: {
   result = DeeXAst_AttrEx(self->ast_attr_get_c.ac_object,flags,root_ast);
   if ((ta = DeeXAst_PredictType(self->ast_attr_get_c.ac_object)) != NULL) {
    result = DeeAstAttribute_MergeSequential(result,DeeType_GetAttrAstAttr(
     ta,flags,(DeeObject *)self->ast_attr_get_c.ac_name,NULL),flags);
   } else result &= ~(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT);
  } break;

  case DEE_XASTKIND_ATTR_HAS_C: {
   result = DeeXAst_AttrEx(self->ast_attr_has_c.ac_object,flags,root_ast);
   if ((ta = DeeXAst_PredictType(self->ast_attr_has_c.ac_object)) != NULL) {
    result = DeeAstAttribute_MergeSequential(result,DeeType_HasAttrAstAttr(
     ta,flags,(DeeObject *)self->ast_attr_has_c.ac_name),flags);
   } else result &= ~(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT);
  } break;

  case DEE_XASTKIND_ATTR_DEL_C: {
   result = DeeXAst_AttrEx(self->ast_attr_del_c.ac_object,flags,root_ast);
   if ((ta = DeeXAst_PredictType(self->ast_attr_del_c.ac_object)) != NULL) {
    result = DeeAstAttribute_MergeSequential(result,DeeType_DelAttrAstAttr(
     ta,flags,(DeeObject *)self->ast_attr_del_c.ac_name),flags);
   } else result &= ~(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT);
  } break;

  case DEE_XASTKIND_ATTR_SET_C: {
   result = DeeAstAttribute_MergeSequential(
    DeeXAst_AttrEx(self->ast_attr_set_c.ac_object,flags,root_ast),
    DeeXAst_AttrEx(self->ast_attr_set_c.ac_value,flags,root_ast),flags);
   if ((ta = DeeXAst_PredictType(self->ast_attr_set_c.ac_object)) != NULL) {
    DeeTypeObject const *tvalue;
    tvalue = DeeXAst_PredictType(self->ast_attr_set_c.ac_value);
    result = DeeAstAttribute_MergeSequential(result,DeeType_SetAttrAstAttr(
     ta,flags,(DeeObject *)self->ast_attr_set_c.ac_name,tvalue),flags);
   } else result &= ~(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT);
  } break;

  case DEE_XASTKIND_DEL_VAR:
   return DEE_AST_ATTRIBUTE_NOEXCEPT;

#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
  case DEE_XASTKIND_FOREIGN_FUNCTION: {
   DeeXAstObject **iter,**end,*arg;
   result = DeeXAst_AttrEx(self->ast_foreignfunction.ff_base,flags,root_ast);
   if ((result&DEE_AST_ATTRIBUTE_NOEXCEPT)!=0) {
    ta = DeeXAst_PredictType(self->ast_foreignfunction.ff_base);
    if (!ta || !DeeType_Check(ta)) result &= ~(DEE_AST_ATTRIBUTE_NOEXCEPT);
   }
   end = (iter = self->ast_foreignfunction.ff_argv)+self->ast_foreignfunction.ff_argc;
   while (iter != end) {
    arg = *iter++;
    result = DeeAstAttribute_MergeSequential(result,DeeXAst_AttrEx(arg,flags,root_ast),flags);
    if ((result&DEE_AST_ATTRIBUTE_NOEXCEPT)!=0) {
     ta = DeeXAst_PredictType(arg);
     if (!ta || !DeeStructuredType_Check(ta)) result &= ~(DEE_AST_ATTRIBUTE_NOEXCEPT);
    }
   }
   return result;
  } break;
#endif

  case DEE_XASTKIND_SWITCH: {
   DeeAstAttribute case_attributes;
   struct DeeXAstSwitchEntry *iter,*end;
   result = DeeXAst_AttrEx(self->ast_switch.s_expr,flags,root_ast);
   if (result == DEE_AST_ATTRIBUTE_SUCCESSIVE_MAX) return result;
   end = (iter = self->ast_switch.s_entryv)+self->ast_switch.s_entryc;
   DEE_ASSERT(iter != end);
   if (self->ast_switch.s_default)
    case_attributes = DeeXAst_AttrEx(self->ast_switch.s_default,flags,root_ast);
   else case_attributes = DEE_AST_ATTRIBUTE_NOOP;
   do case_attributes = DeeAstAttribute_MergeParallel(
    case_attributes,DeeXAst_AttrEx(iter->se_value,flags,root_ast),flags);
   while (++iter != end);
   return DeeAstAttribute_MergeSequential(result,case_attributes,flags);
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
  case DEE_XASTKIND_BUILTIN_ALLOCA: {
   result = DeeXAst_AttrEx(self->ast_operator.op_a,flags,root_ast);
   // On platforms without 'alloca', the operator might throw an out-of-memory
   if ((result&(DEE_AST_ATTRIBUTE_NORETURN|DEE_AST_ATTRIBUTE_NOEXCEPT))==DEE_AST_ATTRIBUTE_NOEXCEPT &&
       (flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS)!=0
       ) result &= ~(DEE_AST_ATTRIBUTE_NOEXCEPT);
   return result;
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
  case DEE_XASTKIND_BUILTIN_UNREACHABLE:
   return DEE_AST_ATTRIBUTE_NORETURN|DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOJUMP;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case DEE_XASTKIND_BUILTIN_BOUND:
   return DEE_AST_ATTRIBUTE_NOOP;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
  case DEE_XASTKIND_BUILTIN_EXPECT:
   return DeeXAst_AttrEx(self->ast_builtin_expect.e_value,flags,root_ast);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */

  case DEE_XASTKIND_BUILTIN_UNUSED:
   return DeeAstAttribute_MergeSequential(
    DeeXAst_AttrEx(self->ast_operator.op_a,flags,root_ast),
    DeeXAst_AttrEx(self->ast_operator.op_b,flags,root_ast),flags);

#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
  case DEE_XASTKIND_BUILTIN_EXTERN:
   // Extern expression are similar to dex, in that they are
   // considered to not have any effect (thus allowing them to be optimized away)
   return DeeAstAttribute_MergeSequential(
    self->ast_builtin_extern.ex_file ? DeeXAst_AttrEx(
    self->ast_builtin_extern.ex_file,flags,root_ast) : DEE_AST_ATTRIBUTE_NOOP,
    DeeAstAttribute_MergeSequential(
     DeeXAst_AttrEx(self->ast_builtin_extern.ex_name,flags,root_ast),
     DeeXAst_AttrEx(self->ast_builtin_extern.ex_type,flags,root_ast),flags),flags)&~(
    DEE_AST_ATTRIBUTE_NOEXCEPT/*|DEE_AST_ATTRIBUTE_NOEFFECT*/);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEX
  case DEE_XASTKIND_BUILTIN_DEX:
   // dex expressions are considered to have no effect, meaning
   // they are allowed to simply be removed without warning.
   return DeeXAst_AttrEx(self->ast_operator.op_a,flags,root_ast)&~(
    DEE_AST_ATTRIBUTE_NOEXCEPT/*|DEE_AST_ATTRIBUTE_NOEFFECT*/);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEX */

  case DEE_XASTKIND_IFTRUE: {
   result = self->ast_iffalse.ic_succ
    ? DeeXAst_AttrEx(self->ast_iffalse.ic_succ,flags,root_ast)
    : DEE_AST_ATTRIBUTE_NOOP;
   if ((result&DEE_AST_ATTRIBUTE_NOEFFECT)!=0 &&
       self->ast_iffalse.ic_fail &&
       !DeeXAst_IsNoEffect(self->ast_iffalse.ic_fail)
       ) result &= ~(DEE_AST_ATTRIBUTE_NOEFFECT);
  } break;

  case DEE_XASTKIND_IFFALSE: {
   result = self->ast_iffalse.ic_fail
    ? DeeXAst_AttrEx(self->ast_iffalse.ic_fail,flags,root_ast)
    : DEE_AST_ATTRIBUTE_NOOP;
   if ((result&DEE_AST_ATTRIBUTE_NOEFFECT)!=0 &&
       self->ast_iffalse.ic_succ &&
       !DeeXAst_IsNoEffect(self->ast_iffalse.ic_succ)
       ) result &= ~(DEE_AST_ATTRIBUTE_NOEFFECT);
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE
  case DEE_XASTKIND_BUILTIN_FF_CLOSURE:
   // foreign function closures are considered to have no effect,
   // meaning that they can be optimized away.
   return DeeAstAttribute_MergeSequential(
    DeeXAst_AttrEx(self->ast_operator.op_a,flags,root_ast),
    DeeXAst_AttrEx(self->ast_operator.op_b,flags,root_ast),flags)&~( // Can throw all kinds of errors
    DEE_AST_ATTRIBUTE_NOEXCEPT/*|DEE_AST_ATTRIBUTE_NOEFFECT*/);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE */

  case DEE_XASTKIND_IS:
  case DEE_XASTKIND_NOT_IS:
   return DeeAstAttribute_MergeSequential(
    DeeXAst_AttrEx(self->ast_operator.op_a,flags,root_ast),
    DeeXAst_AttrEx(self->ast_operator.op_b,flags,root_ast),flags);

  case DEE_XASTKIND_IN:
  case DEE_XASTKIND_NOT_IN:
   // For 'x in y' we only case about the type of 'y',
   // as that's the one we call the operator on.
   result = DeeAstAttribute_MergeSequential(
    DeeXAst_AttrEx(self->ast_operator.op_a,flags,root_ast),
    DeeXAst_AttrEx(self->ast_operator.op_b,flags,root_ast),flags);
   if ((tb = DeeXAst_PredictType(self->ast_operator.op_b)) != NULL) {
    ta = DeeXAst_PredictType(self->ast_operator.op_a);
    result = DeeAstAttribute_MergeSequential(
     result,DeeType_BinaryAstAttr(tb,DeeType_SLOT_ID(tp_seq_contains),flags,ta,NULL),flags);
   } else result &= ~(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT);
   return result;

  case DEE_XASTKIND_CALL: {
   DeeTypeObject const **argv,**iter,**end; Dee_size_t argc;
   DeeAstAttribute more_attr;
   if (self->ast_operator.op_a->ast_kind == DEE_XASTKIND_ATTR_GET_C) {
    if ((ta = DeeXAst_PredictType(self->ast_operator.op_a->ast_attr_get_c.ac_object)) == NULL) goto default_attr;
    switch (self->ast_operator.op_b->ast_kind) {
     case DEE_XASTKIND_CONST: {
      DeeObject **src,*args = self->ast_operator.op_b->ast_const.c_const;
      if (!DeeTuple_Check(args)) goto default_attr;
      result = DeeXAst_AttrEx(self->ast_operator.op_a,flags,root_ast);
      if (result == DEE_AST_ATTRIBUTE_SUCCESSIVE_MAX) return result;
      if ((argc = DeeTuple_SIZE(args)) != 0) {
       while ((argv = (DeeTypeObject const **)malloc_nz(
        argc*sizeof(DeeTypeObject *))) == NULL) {
        if DEE_LIKELY(Dee_CollectMemory()) continue;
        goto default_attr; // Don't report this error...
       }
       end = (iter = argv)+argc;
       src = DeeTuple_ELEM(args);
       do *iter++ = Dee_TYPE(*src++); while (iter != end);
      } else argv = NULL;
      if (self->ast_operator.op_a->ast_attr_get_c.ac_object->ast_kind == DEE_XASTKIND_CONST) {
       DeeObject *constant = self->ast_operator.op_a->ast_attr_get_c.ac_object->ast_const.c_const;
       DEE_ASSERT(ta == Dee_TYPE(constant));
       more_attr = DeeObject_CallAttrAstAttr(ta,constant,DEE_AST_ATTRIBUTE_FLAG_NONE,
                                             (DeeObject *)self->ast_operator.op_a->ast_attr_get_c.ac_name,
                                             argc,argv,NULL);
      } else {
       more_attr = DeeType_CallAttrAstAttr(ta,DEE_AST_ATTRIBUTE_FLAG_NONE,
                                           (DeeObject *)self->ast_operator.op_a->ast_attr_get_c.ac_name,
                                           argc,argv,NULL);
      }
      result = DeeAstAttribute_MergeSequential(result,more_attr,flags);
      free((void *)argv);
      return result;
     } break;
     case DEE_XASTKIND_TUPLE: {
      DeeXAstObject **src = self->ast_operator.op_b->ast_sequence.s_elemv;
      result = DeeXAst_AttrEx(self->ast_operator.op_a,flags,root_ast);
      if (result == DEE_AST_ATTRIBUTE_SUCCESSIVE_MAX) return result;
      argc = self->ast_operator.op_b->ast_sequence.s_elemc;
      DEE_ASSERT(self->ast_operator.op_b->ast_sequence.s_elemc);
      while ((argv = (DeeTypeObject const **)malloc_nz(
       argc*sizeof(DeeTypeObject *))) == NULL) {
       if DEE_LIKELY(Dee_CollectMemory()) continue;
       goto default_attr; // Don't report this error...
      }
      end = (iter = argv)+argc;
      do {
       result = DeeAstAttribute_MergeSequential(result,DeeXAst_AttrEx(*src,flags,root_ast),flags);
       if (result == DEE_AST_ATTRIBUTE_SUCCESSIVE_MAX) { free_nn((void *)argv); return result; }
       if ((*src)->ast_kind == DEE_XASTKIND_EXPAND) {
        free_nn((void *)argv); goto default_attr;
       }
       *iter++ = DeeXAst_PredictType(*src++);
      } while (iter != end);
      if (self->ast_operator.op_a->ast_attr_get_c.ac_object->ast_kind == DEE_XASTKIND_CONST) {
       DeeObject *constant = self->ast_operator.op_a->ast_attr_get_c.ac_object->ast_const.c_const;
       DEE_ASSERT(ta == Dee_TYPE(constant));
       more_attr = DeeObject_CallAttrAstAttr(ta,constant,DEE_AST_ATTRIBUTE_FLAG_NONE,
                                             (DeeObject *)self->ast_operator.op_a->ast_attr_get_c.ac_name,
                                             argc,argv,NULL);
      } else {
       more_attr = DeeType_CallAttrAstAttr(ta,DEE_AST_ATTRIBUTE_FLAG_NONE,
                                           (DeeObject *)self->ast_operator.op_a->ast_attr_get_c.ac_name,
                                           argc,argv,NULL);
      }
      result = DeeAstAttribute_MergeSequential(result,more_attr,flags);
      free((void *)argv);
      return result;
     } break;
     default: break;
    }
   } else {
    if ((ta = DeeXAst_PredictType(self->ast_operator.op_a)) == NULL) goto default_attr;
    switch (self->ast_operator.op_b->ast_kind) {
     case DEE_XASTKIND_CONST: {
      DeeObject **src,*args = self->ast_operator.op_b->ast_const.c_const;
      if (!DeeTuple_Check(args)) goto default_attr;
      result = DeeXAst_AttrEx(self->ast_operator.op_a,flags,root_ast);
      if (result == DEE_AST_ATTRIBUTE_SUCCESSIVE_MAX) return result;
      if ((argc = DeeTuple_SIZE(args)) != 0) {
       while ((argv = (DeeTypeObject const **)malloc_nz(
        argc*sizeof(DeeTypeObject *))) == NULL) {
        if DEE_LIKELY(Dee_CollectMemory()) continue;
        goto default_attr; // Don't report this error...
       }
       end = (iter = argv)+argc;
       src = DeeTuple_ELEM(args);
       do *iter++ = Dee_TYPE(*src++); while (iter != end);
      } else argv = NULL;
      if (self->ast_operator.op_a->ast_kind == DEE_XASTKIND_CONST) {
       DeeObject *constant = self->ast_operator.op_a->ast_const.c_const;
       DEE_ASSERT(ta == Dee_TYPE(constant));
       more_attr = DeeObject_CallAstAttr(ta,constant,DEE_AST_ATTRIBUTE_FLAG_NONE,argc,argv,NULL);
      } else {
       more_attr = DeeType_CallAstAttr(ta,DEE_AST_ATTRIBUTE_FLAG_NONE,argc,argv,NULL);
      }
      result = DeeAstAttribute_MergeSequential(result,more_attr,flags);
      free((void *)argv);
      return result;
     } break;
     case DEE_XASTKIND_TUPLE: {
      DeeXAstObject **src = self->ast_operator.op_b->ast_sequence.s_elemv;
      result = DeeXAst_AttrEx(self->ast_operator.op_a,flags,root_ast);
      if (result == DEE_AST_ATTRIBUTE_SUCCESSIVE_MAX) return result;
      argc = self->ast_operator.op_b->ast_sequence.s_elemc;
      DEE_ASSERT(self->ast_operator.op_b->ast_sequence.s_elemc);
      while ((argv = (DeeTypeObject const **)malloc_nz(
       argc*sizeof(DeeTypeObject *))) == NULL) {
       if DEE_LIKELY(Dee_CollectMemory()) continue;
       goto default_attr; // Don't report this error...
      }
      end = (iter = argv)+argc;
      do {
       result = DeeAstAttribute_MergeSequential(result,DeeXAst_AttrEx(*src,flags,root_ast),flags);
       if (result == DEE_AST_ATTRIBUTE_SUCCESSIVE_MAX) { free_nn((void *)argv); return result; }
       if ((*src)->ast_kind == DEE_XASTKIND_EXPAND) {
        free_nn((void *)argv); goto default_attr;
       }
       *iter++ = DeeXAst_PredictType(*src++);
      } while (iter != end);
      if (self->ast_operator.op_a->ast_kind == DEE_XASTKIND_CONST) {
       DeeObject *constant = self->ast_operator.op_a->ast_const.c_const;
       DEE_ASSERT(ta == Dee_TYPE(constant));
       more_attr = DeeObject_CallAstAttr(ta,constant,DEE_AST_ATTRIBUTE_FLAG_NONE,argc,argv,NULL);
      } else {
       more_attr = DeeType_CallAstAttr(ta,DEE_AST_ATTRIBUTE_FLAG_NONE,argc,argv,NULL);
      }
      result = DeeAstAttribute_MergeSequential(result,more_attr,flags);
      free((void *)argv);
      return result;
     } break;
     default: break;
    }
   }
   goto default_attr;
  } break;

  case DEE_XASTKIND_EXPAND: {
   result = DeeXAst_AttrEx(self->ast_operator.op_a,flags,root_ast);
   if (result == DEE_AST_ATTRIBUTE_SUCCESSIVE_MAX) return result;
   if ((ta = DeeXAst_PredictType(self->ast_operator.op_a)) == NULL)
    return DeeAstAttribute_MergeSequential(result,DEE_AST_ATTRIBUTE_NONE,flags);
   result = DeeType_UnaryAstAttr(ta,DeeType_SLOT_ID(tp_seq_iter_self),flags,&ta);
   if (!ta) return DeeAstAttribute_MergeSequential(result,DEE_AST_ATTRIBUTE_NONE,flags);
   if (result == DEE_AST_ATTRIBUTE_SUCCESSIVE_MAX) return result;
   result = DeeType_UnaryAstAttr(ta,DeeType_SLOT_ID(tp_seq_iter_next),flags,&ta);
   if (!ta) return DeeAstAttribute_MergeSequential(result,DEE_AST_ATTRIBUTE_NONE,flags);
   return result;
  } break;

  case DEE_XASTKIND_LOR:
  case DEE_XASTKIND_LAND: {
   DeeAstAttribute optast_attr;
   result = DeeXAst_AttrEx(self->ast_operator.op_a,flags,root_ast);
   if (result == DEE_AST_ATTRIBUTE_SUCCESSIVE_MAX) return result;
   if ((ta = DeeXAst_PredictType(self->ast_operator.op_a)) == NULL)
    return DeeAstAttribute_MergeSequential(result,DEE_AST_ATTRIBUTE_NONE,flags);
   result = DeeType_UnaryAstAttr(ta,DeeType_SLOT_ID(tp_bool),flags,NULL);
   if (result == DEE_AST_ATTRIBUTE_SUCCESSIVE_MAX) return result;

   if ((tb = DeeXAst_PredictType(self->ast_operator.op_b)) == NULL)
    return DeeAstAttribute_MergeSequential(result,DeeAstAttribute_MergeParallel(
     DEE_AST_ATTRIBUTE_NONE,DEE_AST_ATTRIBUTE_NOOP,flags),flags); // < no-op branch if 'op_b' isn't executed
   optast_attr = DeeType_UnaryAstAttr(tb,DeeType_SLOT_ID(tp_bool),flags,NULL);
   return DeeAstAttribute_MergeSequential(result,DeeAstAttribute_MergeParallel(
    optast_attr,DEE_AST_ATTRIBUTE_NOOP,flags),flags); // < no-op branch if 'op_b' isn't executed
  } break;

  case DEE_XASTKIND_LXOR: {
   result = DeeXAst_AttrEx(self->ast_operator.op_a,flags,root_ast);
   if (result == DEE_AST_ATTRIBUTE_SUCCESSIVE_MAX) return result;
   if ((ta = DeeXAst_PredictType(self->ast_operator.op_a)) == NULL)
    return DeeAstAttribute_MergeSequential(result,DEE_AST_ATTRIBUTE_NONE,flags);
   result = DeeType_UnaryAstAttr(ta,DeeType_SLOT_ID(tp_bool),flags,NULL);
   if (result == DEE_AST_ATTRIBUTE_SUCCESSIVE_MAX) return result;
   return DeeAstAttribute_MergeSequential(
    result,(ta = DeeXAst_PredictType(self->ast_operator.op_b)) != NULL
    ? DeeType_UnaryAstAttr(ta,DeeType_SLOT_ID(tp_bool),flags,NULL)
    : DEE_AST_ATTRIBUTE_NONE,flags);
  } break;

  DEE_XASTKIND_CASE_INPLACE_VAR_UNARY {
#if 0
   // TODO: Predict type of variable using assumptions
   if ((ta = DeeXAst_PredictType(self->ast_unary_var.uiv_var)) != NULL) {
    // TODO: Using assumptions, we can predict if the variable will be assigned
    result = (DeeAstAttribute)((flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS)!=0
     ? (DEE_AST_ATTRIBUTE_NOEFFECT|DEE_AST_ATTRIBUTE_NOJUMP) // Could possibly throw an unbound local error
     : (DEE_AST_ATTRIBUTE_NOEFFECT|DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOJUMP));
    result = DeeAstAttribute_MergeSequential(result,DeeType_UnaryAstAttr(
     ta,DeeType_SlotIdFromXAstKind(self->ast_kind),flags,NULL),flags);
    return result;
   } else
#endif
   {
    return DEE_AST_ATTRIBUTE_NONE;
   }
  } break;

  DEE_XASTKIND_CASE_INPLACE_VAR_BINARY {
#if 0
   // TODO: Predict type of variable using assumptions
   if ((ta = DeeXAst_PredictType(self->ast_binary_var.biv_var)) != NULL) {
    // TODO: Using assumptions, we can predict if the variable will be assigned
    result = (DeeAstAttribute)((flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS)!=0
     ? (DEE_AST_ATTRIBUTE_NOEFFECT|DEE_AST_ATTRIBUTE_NOJUMP) // Could possibly throw an unbound local error
     : (DEE_AST_ATTRIBUTE_NOEFFECT|DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOJUMP));
    result = DeeAstAttribute_MergeSequential(result,DeeType_BinaryAstAttr(
     ta,DeeType_SlotIdFromXAstKind(self->ast_kind),flags,
     DeeXAst_PredictType(self->ast_binary_var.biv_arg),NULL),flags);
    return result;
   } else
#endif
   {
    return DEE_AST_ATTRIBUTE_NONE;
   }
  } break;

  default:default_attr: if (DEE_XASTKIND_ISOPERATOR(self->ast_kind)) {
   switch (DEE_XASTKIND_OPCOUNT(self->ast_kind)) {
    case 3: {
     result = DeeAstAttribute_MergeSequential(
      DeeXAst_AttrEx(self->ast_operator.op_a,flags,root_ast),DeeAstAttribute_MergeSequential(
      DeeXAst_AttrEx(self->ast_operator.op_b,flags,root_ast),
      DeeXAst_AttrEx(self->ast_operator.op_c,flags,root_ast),flags),flags);
     if ((ta = DeeXAst_PredictType(self->ast_operator.op_a)) != NULL) {
      result = DeeAstAttribute_MergeSequential(result,DeeType_TrinaryAstAttr(
       ta,DeeType_SlotIdFromXAstKind(self->ast_kind),flags,
       DeeXAst_PredictType(self->ast_operator.op_b),
       DeeXAst_PredictType(self->ast_operator.op_c),NULL),flags);
     } else result &= ~(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT);
    } return result;
    case 2: {
     result = DeeAstAttribute_MergeSequential(
      DeeXAst_AttrEx(self->ast_operator.op_a,flags,root_ast),
      DeeXAst_AttrEx(self->ast_operator.op_b,flags,root_ast),flags);
     if ((ta = DeeXAst_PredictType(self->ast_operator.op_a)) != NULL) {
      result = DeeAstAttribute_MergeSequential(result,DeeType_BinaryAstAttr(
       ta,DeeType_SlotIdFromXAstKind(self->ast_kind),flags,
       DeeXAst_PredictType(self->ast_operator.op_b),NULL),flags);
     } else result &= ~(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT);
    } return result;
    case 1: {
     result = DeeXAst_AttrEx(self->ast_operator.op_a,flags,root_ast);
     ta = DeeXAst_PredictType(self->ast_operator.op_a);
     if ((ta = DeeXAst_PredictType(self->ast_operator.op_a)) != NULL) {
      result = DeeAstAttribute_MergeSequential(result,DeeType_UnaryAstAttr(
       ta,DeeType_SlotIdFromXAstKind(self->ast_kind),flags,NULL),flags);
     } else result &= ~(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT);
    } return result;
    default: break;
   }
  } break;
 }
 return DEE_AST_ATTRIBUTE_NONE;
}

DEE_A_RET_WUNUSED DeeAstAttribute DeeVarDeclStorage_AttrEx(
 DEE_A_IN struct DeeVarDeclStorage const *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN DeeObject const *root_ast) {
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(root_ast) && (DeeXAst_Check(root_ast) || DeeSAst_Check(root_ast)));
 switch (self->vds_kind) {
  case DEE_VARDECLSTORAGEKIND_VAR:
   // v can't throw an unbound local, since this is meant as a storage,
   //   meaning that the compiler would generate a some kind of store operation.
   return /*(flags&DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS)!=0
    ? (DEE_AST_ATTRIBUTE_NOEFFECT)
    : */DEE_AST_ATTRIBUTE_NOOP;
  case DEE_VARDECLSTORAGEKIND_ATTR:
   return DeeAstAttribute_MergeSequential(
    DeeXAst_AttrEx(self->vds_attr.as_object,flags,root_ast),
    DeeXAst_AttrEx(self->vds_attr.as_attr,flags,root_ast),flags
    )&~(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT);
  case DEE_VARDECLSTORAGEKIND_ATTR_C:
   return DeeXAst_AttrEx(self->vds_attr_c.acs_object,flags,root_ast)
    &~(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT);
  case DEE_VARDECLSTORAGEKIND_ITEM:
   return DeeAstAttribute_MergeSequential(
    DeeXAst_AttrEx(self->vds_item.is_object,flags,root_ast),
    DeeXAst_AttrEx(self->vds_item.is_key,flags,root_ast),flags)
    &~(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT);
  case DEE_VARDECLSTORAGEKIND_RANGE:
   return DeeAstAttribute_MergeSequential(
    DeeXAst_AttrEx(self->vds_range.rs_object,flags,root_ast),
     DeeAstAttribute_MergeSequential(
      self->vds_range.rs_begin ? DeeXAst_AttrEx(self->vds_range.rs_begin,flags,root_ast) : DEE_AST_ATTRIBUTE_NOOP,
      self->vds_range.rs_end ? DeeXAst_AttrEx(self->vds_range.rs_end,flags,root_ast) : DEE_AST_ATTRIBUTE_NOOP,
      flags),flags)&~(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT);
  case DEE_VARDECLSTORAGEKIND_EXPAND: {
   struct DeeVarDeclStorage *iter,*end;
   // NOTE: We remove noexcept and noeffect, because of the necessary expand expression
   // TODO: Should it really be us who does this? (if anyone does it at all)
   DeeAstAttribute result = DEE_AST_ATTRIBUTE_NOOP&~(
    DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT);
   end = (iter = self->vds_expand.es_declv)+self->vds_expand.es_declc;
   do result = DeeAstAttribute_MergeSequential(
    result,DeeVarDeclStorage_AttrEx(iter,flags,root_ast),flags);
   while (++iter != end);
   return result;
  } break;
  case DEE_VARDECLSTORAGEKIND_ASSIGN:
   return DeeXAst_AttrEx(self->vds_assign.as_ast,flags,root_ast)&
    ~(DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT);
  default: break;
 }
 return DEE_AST_ATTRIBUTE_NONE;
}

DEE_DECL_END
