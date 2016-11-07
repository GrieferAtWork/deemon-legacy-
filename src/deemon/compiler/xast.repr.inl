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

DEE_A_RET_WUNUSED DEE_A_RET_Z char const *
_DeeType_ClassOperatorName(DEE_A_IN int slot) {
 switch (slot) {
  case DeeType_SLOT_ID(tp_new):
  case DeeType_SLOT_ID(tp_ctor):
  case DeeType_SLOT_ID(tp_any_new):
  case DeeType_SLOT_ID(tp_any_ctor):
   return "__constructor__";
  case DEE_XAST_TYPESLOT_SUPERARGS:
  case DEE_CLASS_SLOTID_SUPERARGS_CTOR:
  case DEE_CLASS_SLOTID_SUPERARGS_ANY_CTOR:
   return "__superargs__";
  case DeeType_SLOT_ID(tp_copy_new):
  case DeeType_SLOT_ID(tp_copy_ctor):
   return "__copy__";
  case DeeType_SLOT_ID(tp_move_new):
  case DeeType_SLOT_ID(tp_move_ctor):
   return "__move__";
  case DeeType_SLOT_ID(tp_dtor):
  case DeeType_SLOT_ID(tp_clear):
   return "__destructor__";
  case DeeType_SLOT_ID(tp_copy_assign): return "__copyassign__";
  case DeeType_SLOT_ID(tp_move_assign): return "__moveassign__";
  case DeeType_SLOT_ID(tp_any_assign): return "__assign__";
  case DeeType_SLOT_ID(tp_str): return "__str__";
  case DeeType_SLOT_ID(tp_repr): return "__repr__";
  case DeeType_SLOT_ID(tp_int32): return "__int32__";
  case DeeType_SLOT_ID(tp_int64): return "__int64__";
  case DeeType_SLOT_ID(tp_double): return "__double__";
  case DeeType_SLOT_ID(tp_call): return "__call__";
  case DeeType_SLOT_ID(tp_not): return "__not__";
  case DeeType_SLOT_ID(tp_bool): return "__bool__";
  case DeeType_SLOT_ID(tp_inv): return "__inv__";
  case DeeType_SLOT_ID(tp_pos): return "__pos__";
  case DeeType_SLOT_ID(tp_neg): return "__neg__";
  case DeeType_SLOT_ID(tp_inc): return "__inc__";
  case DeeType_SLOT_ID(tp_incpost): return "__incpost__";
  case DeeType_SLOT_ID(tp_dec): return "__dec__";
  case DeeType_SLOT_ID(tp_decpost): return "__decpost__";
  case DeeType_SLOT_ID(tp_add): return "__add__";
  case DeeType_SLOT_ID(tp_iadd): return "__iadd__";
  case DeeType_SLOT_ID(tp_sub): return "__sub__";
  case DeeType_SLOT_ID(tp_isub): return "__isub__";
  case DeeType_SLOT_ID(tp_mul): return "__mul__";
  case DeeType_SLOT_ID(tp_imul): return "__imul__";
  case DeeType_SLOT_ID(tp_div): return "__div__";
  case DeeType_SLOT_ID(tp_idiv): return "__idiv__";
  case DeeType_SLOT_ID(tp_mod): return "__mod__";
  case DeeType_SLOT_ID(tp_imod): return "__imod__";
  case DeeType_SLOT_ID(tp_shl): return "__shl__";
  case DeeType_SLOT_ID(tp_ishl): return "__ishl__";
  case DeeType_SLOT_ID(tp_shr): return "__shr__";
  case DeeType_SLOT_ID(tp_ishr): return "__ishr__";
  case DeeType_SLOT_ID(tp_and): return "__and__";
  case DeeType_SLOT_ID(tp_iand): return "__iand__";
  case DeeType_SLOT_ID(tp_or): return "__or__";
  case DeeType_SLOT_ID(tp_ior): return "__ior__";
  case DeeType_SLOT_ID(tp_xor): return "__xor__";
  case DeeType_SLOT_ID(tp_ixor): return "__ixor__";
  case DeeType_SLOT_ID(tp_pow): return "__pow__";
  case DeeType_SLOT_ID(tp_ipow): return "__ipow__";
  case DeeType_SLOT_ID(tp_hash): return "__hash__";
  case DeeType_SLOT_ID(tp_cmp_lo): return "__lo__";
  case DeeType_SLOT_ID(tp_cmp_le): return "__le__";
  case DeeType_SLOT_ID(tp_cmp_eq): return "__eq__";
  case DeeType_SLOT_ID(tp_cmp_ne): return "__ne__";
  case DeeType_SLOT_ID(tp_cmp_gr): return "__gr__";
  case DeeType_SLOT_ID(tp_cmp_ge): return "__ge__";
  case DeeType_SLOT_ID(tp_seq_get): return "__getitem__";
  case DeeType_SLOT_ID(tp_seq_del): return "__delitem__";
  case DeeType_SLOT_ID(tp_seq_set): return "__setitem__";
  case DeeType_SLOT_ID(tp_seq_size): return "__size__";
  case DeeType_SLOT_ID(tp_seq_contains): return "__contains__";
  case DeeType_SLOT_ID(tp_seq_range_get): return "__getrange__";
  case DeeType_SLOT_ID(tp_seq_range_del): return "__delrange__";
  case DeeType_SLOT_ID(tp_seq_range_set): return "__setrange__";
  case DeeType_SLOT_ID(tp_seq_iter_self): return "__iterself__";
  case DEE_CLASS_SLOTID_OPERATOR_FOR: return "__for__"; // Special: 'operator for { ... }'
  case DeeType_SLOT_ID(tp_seq_iter_next): return "__iternext__";
  case DeeType_SLOT_ID(tp_attr_get): return "__getattr__";
  case DeeType_SLOT_ID(tp_attr_del): return "__delattr__";
  case DeeType_SLOT_ID(tp_attr_set): return "__setattr__";
  case DeeType_SLOT_ID(tp_p_add): return "+";
  case DeeType_SLOT_ID(tp_p_sub): return "-";
  case DeeType_SLOT_ID(tp_p_mul): return "*";
  case DeeType_SLOT_ID(tp_p_and): return "&";
  case DeeType_SLOT_ID(tp_p_seq_get): return "[]";
  case DeeType_SLOT_ID(tp_p_seq_del): return "del []";
  case DeeType_SLOT_ID(tp_p_seq_set): return "[] =";
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  case DEE_XAST_TYPESLOT_IO_READ: return "__read__";
  case DeeType_SLOT_ID(tp_io_read): return "__readp__";
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  case DEE_CLASS_SLOTID_OPERATOR_READNP: return "__readnp__";
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  case DEE_XAST_TYPESLOT_IO_WRITE: return "__write__";
  case DeeType_SLOT_ID(tp_io_write): return "__writep__";
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  case DEE_CLASS_SLOTID_OPERATOR_WRITENP: return "__writenp__";
  case DeeType_SLOT_ID(tp_io_seek): return "__seek__";
  case DeeType_SLOT_ID(tp_io_flush): return "__flush__";
  case DeeType_SLOT_ID(tp_io_trunc): return "__trunc__";
  case DeeType_SLOT_ID(tp_io_close): return "__close__";
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case DEE_XAST_TYPESLOT_REF: return "__ref__";
  case DEE_XAST_TYPESLOT_DEREF: return "__deref__";
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
  case DEE_XAST_TYPESLOT_ATTR_HAS: return "__hasattr__";
  case DEE_XAST_TYPESLOT_NONE: return DeeType_NAME(&DeeNone_Type);
  default: break;
 }
 return "?";
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeVarDeclStorage_Repr(DEE_A_IN struct DeeVarDeclStorage const *self) {
 switch (self->vds_kind) {
  case DEE_VARDECLSTORAGEKIND_NONE:
   DeeReturn_STATIC_STRING("none");
  case DEE_VARDECLSTORAGEKIND_VAR:
#if DEE_XCONFIG_PARSER_XAST_REPR_ALWAYS_ADD_IDS
   return DeeString_Newf("%k@%u",self->vds_var.vs_token,self->vds_var.vs_var->lv_name);
#else
   return DeeToken_Str((DeeObject *)self->vds_var.vs_token);
#endif
  case DEE_VARDECLSTORAGEKIND_ATTR:
   return DeeString_Newf("__getattr__(%r,%r)",
                         self->vds_attr.as_object,
                         self->vds_attr.as_attr);
  case DEE_VARDECLSTORAGEKIND_ATTR_C:
   return DeeString_Newf("(%r).%r",
                         self->vds_attr_c.acs_object,
                         self->vds_attr_c.acs_attr);
  case DEE_VARDECLSTORAGEKIND_ITEM:
   return DeeString_Newf("(%r)[%r]",
                         self->vds_item.is_object,
                         self->vds_item.is_key);
  case DEE_VARDECLSTORAGEKIND_RANGE:
   return DeeString_Newf("(%r)[%r:%r]",
                         self->vds_range.rs_object,
                         self->vds_range.rs_begin ? (DeeObject *)self->vds_range.rs_begin : Dee_None,
                         self->vds_range.rs_end ? (DeeObject *)self->vds_range.rs_end : Dee_None);
  case DEE_VARDECLSTORAGEKIND_EXPAND: {
   DeeObject *result,*elem_repr; int temp;
   struct DeeVarDeclStorage *begin,*iter,*end;
   DeeStringWriter writer = DeeStringWriter_INIT();
   end = (iter = begin = self->vds_expand.es_declv)+self->vds_expand.es_declc;
   DEE_ASSERT(self->vds_expand.es_declc);
   do {
    if (iter != begin && DeeStringWriter_WriteChar(&writer,',') != 0) goto err_expand_writer;
    if ((elem_repr = DeeVarDeclStorage_Repr(iter)) == NULL) goto err_expand_writer;
    temp = DeeStringWriter_WriteObjectStr(&writer,elem_repr);
    Dee_DECREF(elem_repr);
    if (temp != 0) goto err_expand_writer;
    ++iter;
   } while (iter != end);
   result = DeeStringWriter_Pack(&writer);
end_expand_writer: DeeStringWriter_Quit(&writer);
   return result;
err_expand_writer: result = NULL; goto end_expand_writer;
  } break;
  case DEE_VARDECLSTORAGEKIND_ASSIGN:
   return DeeObject_Repr((DeeObject *)self->vds_assign.as_ast);
  default: break;
 }
 DeeError_NotImplemented();
 return NULL;
}

DEE_A_RET_EXCEPT_REF DeeObject *DeeXAst_UnaryDescrStr(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeXAstObject *ast_a) {
 DEE_ASSERT(DEE_XASTKIND_ISUNARY(kind));
 DEE_ASSERT(DeeObject_Check(ast_a) && DeeXAst_Check(ast_a));
 switch (kind) {
  case DEE_XASTKIND_STR: return DeeString_Newf("str (%r)",ast_a);
  case DEE_XASTKIND_REPR: return DeeString_Newf("repr (%r)",ast_a);
  case DEE_XASTKIND_COPY: return DeeString_Newf("copy (%r)",ast_a);
  case DEE_XASTKIND_MOVE: return DeeString_Newf("move (%r)",ast_a);
  case DEE_XASTKIND_TYPE: return DeeString_Newf("type (%r)",ast_a);
  case DEE_XASTKIND_WEAK: return DeeString_Newf("weak (%r)",ast_a);
  case DEE_XASTKIND_BOOL: return DeeString_Newf("bool (%r)",ast_a);
  case DEE_XASTKIND_SUPEROF: return DeeString_Newf("(%r).super",ast_a);
  case DEE_XASTKIND_CLASSOF: return DeeString_Newf("(%r).class",ast_a);
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case DEE_XASTKIND_REF: return DeeString_Newf("&(%r)",ast_a);
  case DEE_XASTKIND_DEREF: return DeeString_Newf("*(%r)",ast_a);
  case DEE_XASTKIND_PTROF: return DeeString_Newf("(%r) *",ast_a);
  case DEE_XASTKIND_LVALOF: return DeeString_Newf("(%r) &",ast_a);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
#if DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
  case DEE_XASTKIND_VARRAYOF: return DeeString_Newf("(%r)[]",ast_a);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ARRAYS */
  case DEE_XASTKIND_NOT: return DeeString_Newf("!(%r)",ast_a);
  case DEE_XASTKIND_NEG: return DeeString_Newf("-(%r)",ast_a);
  case DEE_XASTKIND_POS: return DeeString_Newf("+(%r)",ast_a);
  case DEE_XASTKIND_INV: return DeeString_Newf("~(%r)",ast_a);
  case DEE_XASTKIND_INC: return DeeString_Newf("++(%r)",ast_a);
  case DEE_XASTKIND_DEC: return DeeString_Newf("--(%r)",ast_a);
  case DEE_XASTKIND_INCPOST: return DeeString_Newf("(%r)++",ast_a);
  case DEE_XASTKIND_DECPOST: return DeeString_Newf("(%r)--",ast_a);
  case DEE_XASTKIND_EXPAND: return DeeString_Newf("(%r)...",ast_a);
  case DEE_XASTKIND_SEQ_SIZE: return DeeString_Newf("#(%r)",ast_a);
#if DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE
  case DEE_XASTKIND_NEW: return DeeString_Newf("new (%r)",ast_a);
  case DEE_XASTKIND_DELETE: return DeeString_Newf("delete (%r)",ast_a);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE */
#if DEE_XCONFIG_PARSER_XAST_REPR_USE_SUBSCRIPT_OPERATORS
  case DEE_XASTKIND_ITERSELF: return DeeString_Newf("(%r).operator __iterself__()",ast_a);
  case DEE_XASTKIND_ITERNEXT: return DeeString_Newf("(%r).operator __iternext__()",ast_a);
#else /* DEE_XCONFIG_PARSER_XAST_REPR_USE_SUBSCRIPT_OPERATORS */
  case DEE_XASTKIND_ITERSELF: return DeeString_Newf("__iterself__(%r)",ast_a);
  case DEE_XASTKIND_ITERNEXT: return DeeString_Newf("__iternext__(%r)",ast_a);
#endif /* !DEE_XCONFIG_PARSER_XAST_REPR_USE_SUBSCRIPT_OPERATORS */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
  case DEE_XASTKIND_BUILTIN_ALLOCA: return DeeString_Newf("__builtin_alloca(%r)",ast_a);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP
  case DEE_XASTKIND_BUILTIN_HELP: return DeeString_Newf("__builtin_help(%r)",ast_a);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP */
  case DEE_XASTKIND_SEQ_ANY: return DeeString_Newf("(%r)||...",ast_a);
  case DEE_XASTKIND_SEQ_ALL: return DeeString_Newf("(%r)&&...",ast_a);
  case DEE_XASTKIND_SEQ_SUM: return DeeString_Newf("(%r)+...",ast_a);
  case DEE_XASTKIND_SEQ_MIN: return DeeString_Newf("(%r)<...",ast_a);
  case DEE_XASTKIND_SEQ_MAX: return DeeString_Newf("(%r)>...",ast_a);
  default: break;
 }
 return DeeString_Newf("(%r).operator ? ()",ast_a);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeXAst_BinaryDescrStr(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeXAstObject *ast_a, DEE_A_INOUT DeeXAstObject *ast_b) {
 DEE_ASSERT(DEE_XASTKIND_ISBINARY(kind));
 DEE_ASSERT(DeeObject_Check(ast_a) && DeeXAst_Check(ast_a));
 DEE_ASSERT(DeeObject_Check(ast_b) && DeeXAst_Check(ast_b));
 switch (kind) {
  case DEE_XASTKIND_CALL: {
   DeeObject *result;
   struct DeeObject **begin,**iter,**end;
   if (ast_b->ast_kind == DEE_XASTKIND_CONST && DeeTuple_Check(ast_b->ast_const.c_const)) {
    end = (begin = DeeTuple_ELEM(ast_b->ast_const.c_const))+DeeTuple_SIZE(ast_b->ast_const.c_const);
    goto do_write_tuple;
   } else if (ast_b->ast_kind == DEE_XASTKIND_TUPLE) {
    end = (begin = (DeeObject **)ast_b->ast_sequence.s_elemv)+ast_b->ast_sequence.s_elemc;
do_write_tuple:
    {
     DeeStringWriter writer = DeeStringWriter_INIT();
     iter = begin;
     if (DeeStringWriter_Writef(&writer,"(%r)(",ast_a) != 0) goto err_call_writer;
     while (iter != end) {
      if (iter != begin && DeeStringWriter_WriteChar(&writer,',') != 0) goto err_call_writer;
      if (DeeStringWriter_WriteObjectRepr(&writer,*iter) != 0) goto err_call_writer;
      ++iter;
     }
     if (DeeStringWriter_WRITE_STRING(&writer,")") != 0) goto err_call_writer;
     result = DeeStringWriter_Pack(&writer);
end_call_writer: DeeStringWriter_Quit(&writer);
     return result;
err_call_writer: result = NULL; goto end_call_writer;
    }
   } else {
    return DeeString_Newf("(%r)%r",ast_a,ast_b);
   }
  } break;
  case DEE_XASTKIND_COMPARE_LO: return DeeString_Newf("(%r) < (%r)",ast_a,ast_b);
  case DEE_XASTKIND_COMPARE_LE: return DeeString_Newf("(%r) <= (%r)",ast_a,ast_b);
  case DEE_XASTKIND_COMPARE_EQ: return DeeString_Newf("(%r) == (%r)",ast_a,ast_b);
  case DEE_XASTKIND_COMPARE_NE: return DeeString_Newf("(%r) != (%r)",ast_a,ast_b);
  case DEE_XASTKIND_COMPARE_GR: return DeeString_Newf("(%r) > (%r)",ast_a,ast_b);
  case DEE_XASTKIND_COMPARE_GE: return DeeString_Newf("(%r) >= (%r)",ast_a,ast_b);
  case DEE_XASTKIND_ADD: return DeeString_Newf("(%r) + (%r)",ast_a,ast_b);
  case DEE_XASTKIND_SUB: return DeeString_Newf("(%r) - (%r)",ast_a,ast_b);
  case DEE_XASTKIND_MUL: return DeeString_Newf("(%r) * (%r)",ast_a,ast_b);
  case DEE_XASTKIND_DIV: return DeeString_Newf("(%r) / (%r)",ast_a,ast_b);
  case DEE_XASTKIND_MOD: return DeeString_Newf("(%r) % (%r)",ast_a,ast_b);
  case DEE_XASTKIND_SHL: return DeeString_Newf("(%r) << (%r)",ast_a,ast_b);
  case DEE_XASTKIND_SHR: return DeeString_Newf("(%r) >> (%r)",ast_a,ast_b);
  case DEE_XASTKIND_AND: return DeeString_Newf("(%r) & (%r)",ast_a,ast_b);
  case DEE_XASTKIND_OR: return DeeString_Newf("(%r) | (%r)",ast_a,ast_b);
  case DEE_XASTKIND_XOR: return DeeString_Newf("(%r) ^ (%r)",ast_a,ast_b);
  case DEE_XASTKIND_POW: return DeeString_Newf("(%r) ** (%r)",ast_a,ast_b);
  case DEE_XASTKIND_IADD: return DeeString_Newf("(%r) += (%r)",ast_a,ast_b);
  case DEE_XASTKIND_ISUB: return DeeString_Newf("(%r) -= (%r)",ast_a,ast_b);
  case DEE_XASTKIND_IMUL: return DeeString_Newf("(%r) *= (%r)",ast_a,ast_b);
  case DEE_XASTKIND_IDIV: return DeeString_Newf("(%r) /= (%r)",ast_a,ast_b);
  case DEE_XASTKIND_IMOD: return DeeString_Newf("(%r) %= (%r)",ast_a,ast_b);
  case DEE_XASTKIND_ISHL: return DeeString_Newf("(%r) <<= (%r)",ast_a,ast_b);
  case DEE_XASTKIND_ISHR: return DeeString_Newf("(%r) >>= (%r)",ast_a,ast_b);
  case DEE_XASTKIND_IAND: return DeeString_Newf("(%r) &= (%r)",ast_a,ast_b);
  case DEE_XASTKIND_IOR: return DeeString_Newf("(%r) |= (%r)",ast_a,ast_b);
  case DEE_XASTKIND_IXOR: return DeeString_Newf("(%r) ^= (%r)",ast_a,ast_b);
  case DEE_XASTKIND_IPOW: return DeeString_Newf("(%r) **= (%r)",ast_a,ast_b);
  case DEE_XASTKIND_SEQ_GET: return DeeString_Newf("(%r)[%r]",ast_a,ast_b);
#if DEE_XCONFIG_PARSER_XAST_REPR_USE_SUBSCRIPT_OPERATORS
  case DEE_XASTKIND_SEQ_DEL: return DeeString_Newf("(%r).operator del[] (%r)",ast_a,ast_b);
  case DEE_XASTKIND_ATTR_GET: return DeeString_Newf("(%r).operator . (%r)",ast_a,ast_b);
  case DEE_XASTKIND_ATTR_DEL: return DeeString_Newf("(%r).operator del. (%r)",ast_a,ast_b);
#else
  case DEE_XASTKIND_SEQ_DEL: return DeeString_Newf("__delitem__(%r,%r)",ast_a,ast_b);
  case DEE_XASTKIND_ATTR_GET: return DeeString_Newf("__getattr__(%r,%r)",ast_a,ast_b);
  case DEE_XASTKIND_ATTR_DEL: return DeeString_Newf("__delattr__(%r,%r)",ast_a,ast_b);
#endif
  case DEE_XASTKIND_ATTR_HAS: return DeeString_Newf("__hasattr__(%r,%r)",ast_a,ast_b);
  case DEE_XASTKIND_IS: return DeeString_Newf("(%r) is (%r)",ast_a,ast_b);
  case DEE_XASTKIND_IN: return DeeString_Newf("(%r) in (%r)",ast_a,ast_b);
  case DEE_XASTKIND_NOT_IS: return DeeString_Newf("(%r) !is (%r)",ast_a,ast_b);
  case DEE_XASTKIND_NOT_IN: return DeeString_Newf("(%r) !in (%r)",ast_a,ast_b);
  case DEE_XASTKIND_LAND: return DeeString_Newf("(%r) && (%r)",ast_a,ast_b);
  case DEE_XASTKIND_LOR: return DeeString_Newf("(%r) || (%r)",ast_a,ast_b);
  case DEE_XASTKIND_LXOR: return DeeString_Newf("(%r) ^^ (%r)",ast_a,ast_b);
#if DEE_XCONFIG_PARSER_XAST_REPR_USE_SUBSCRIPT_OPERATORS
  case DEE_XASTKIND_MOVE_ASSIGN: return DeeString_Newf("(%r).operator move := (%r)",ast_a,ast_b);
#else
  case DEE_XASTKIND_MOVE_ASSIGN: return DeeString_Newf("__moveassign__(%r,%r)",ast_a,ast_b);
#endif
  case DEE_XASTKIND_COPY_ASSIGN: return DeeString_Newf("(%r) := (%r)",ast_a,ast_b);
  case DEE_XASTKIND_BUILTIN_UNUSED: return DeeString_Newf("__builtin_unused(%r,%r)",ast_a,ast_b);
  default: break;
 }
 return DeeString_Newf("(%r).operator ? (%r)",ast_a,ast_b);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeXAst_TrinaryDescrStr(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeXAstObject *ast_a,
 DEE_A_INOUT DeeXAstObject *ast_b, DEE_A_INOUT DeeXAstObject *ast_c) {
 DEE_ASSERT(DEE_XASTKIND_ISTRINARY(kind));
 DEE_ASSERT(DeeObject_Check(ast_a) && DeeXAst_Check(ast_a));
 DEE_ASSERT(DeeObject_Check(ast_b) && DeeXAst_Check(ast_b));
 DEE_ASSERT(DeeObject_Check(ast_c) && DeeXAst_Check(ast_c));
 switch (kind) {
  case DEE_XASTKIND_SEQ_SET:  return DeeString_Newf("(%r)[%r] = (%r)",ast_a,ast_b,ast_c);
  case DEE_XASTKIND_ATTR_SET: return DeeString_Newf("__setattr__(%r,%r,%r)",ast_a,ast_b,ast_c);
  default: break;
 }
 return DeeString_Newf("(%r).operator ? (%r,%r)",ast_a,ast_b,ast_c);
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeXAst_Name(
 DEE_A_IN DeeXAstObject const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeXAst_Check(self));
 switch (self->ast_kind) {
  case DEE_XASTKIND_VAR:
   return DeeToken_Str((DeeObject *)self->ast_common.ast_token);
  default: break;
 }
 return DeeObject_Repr((DeeObject *)self);
}



DEE_STATIC_INLINE(DeeObject *) _DeeLocalVar_ReprWithToken(
 DeeLocalVarObject *local_var, DeeTokenObject *tk) {
 return DeeString_Newf("%s%K"
#if DEE_XCONFIG_PARSER_XAST_REPR_ALWAYS_ADD_IDS
                       "@%u"
#endif
                       ,DeeLocalVar_KIND(local_var) == DEE_LOCALVAR_KIND_STATIC ? "static " : ""
                       ,DeeToken_Str((DeeObject *)tk)
#if DEE_XCONFIG_PARSER_XAST_REPR_ALWAYS_ADD_IDS
                       ,local_var->lv_loc_id
#endif
                       );
}

DEE_COMPILER_MSVC_WARNING_PUSH(4061 4062)
static DeeObject *_deexast_tp_repr(DeeXAstObject *self) {
 DeeObject *result;
 switch (self->ast_kind) {

  case DEE_XASTKIND_CONST: {
   DeeObject *error;
   result = DeeObject_Repr(self->ast_const.c_const);
   if (!result && DeeError_GetOccurred(&error,NULL) && error) {
    Dee_INCREF(error); DeeError_Handled();
    result = DeeObject_Repr(error);
    Dee_DECREF(error);
   }
   return result;
  }
  case DEE_XASTKIND_VAR:
   return _DeeLocalVar_ReprWithToken(self->ast_var.vs_var,self->ast_common.ast_token);
  case DEE_XASTKIND_VARDECL:
   return DeeString_Newf("%K = %r",_DeeLocalVar_ReprWithToken(
    self->ast_vardecl.vd_var,self->ast_vardecl.vd_token),
    self->ast_vardecl.vd_init);
  case DEE_XASTKIND_MULTIVARDECL: {
   struct DeeVarDeclStorage *begin,*iter,*end;
   DeeStringWriter writer = DeeStringWriter_INIT();
   end = (iter = begin = self->ast_multivardecl.mvd_varv)+self->ast_multivardecl.mvd_varc;
   DEE_ASSERT(self->ast_multivardecl.mvd_varc);
   do {
    DeeObject *vds_repr; int temp;
    if (iter != begin && DeeStringWriter_WriteChar(&writer,',') != 0) goto err_mvd_writer;
    if ((vds_repr = DeeVarDeclStorage_Repr(iter)) == NULL) goto err_mvd_writer;
    temp = DeeStringWriter_WriteObjectStr(&writer,vds_repr);
    Dee_DECREF(vds_repr);
    if (temp != 0) goto err_mvd_writer;
   } while (++iter != end);
   if (DeeStringWriter_Writef(&writer," = (%r)...",
    self->ast_multivardecl.mvd_init) != 0) goto err_mvd_writer;
   result = DeeStringWriter_Pack(&writer);
end_mvd_writer: DeeStringWriter_Quit(&writer); return result;
err_mvd_writer: result = NULL; goto end_mvd_writer;
  } break;
  case DEE_XASTKIND_STATEMENT:
   return self->ast_statement.s_stmt->ast_kind == DEE_SASTKIND_BLOCK
    ? DeeString_Newf("(%r)",self->ast_statement.s_stmt)
    : DeeString_Newf("(/* symbolic */{\n\t%>r\n})",self->ast_statement.s_stmt);
  case DEE_XASTKIND_IF:
   return self->ast_if.if_succ
    ? DeeString_Newf("(%r) ? (%r) : (%r)",self->ast_if.if_cond,self->ast_if.if_succ,self->ast_if.if_fail ? (DeeObject *)self->ast_if.if_fail : Dee_None)
    : DeeString_Newf("(%r) ? : (%r)",self->ast_if.if_cond,self->ast_if.if_fail ? (DeeObject *)self->ast_if.if_fail : Dee_None);

  case DEE_XASTKIND_FUNCTION: {
   struct DeeXAstFunctionAstArgument *begin,*iter,*end;
   DeeStringWriter writer = DeeStringWriter_INIT();
   if (DeeStringWriter_WRITE_STRING(&writer,"function") != 0) goto err_f_writer;
   if ((self->ast_function.f_flags&DEE_FUNCTION_FLAG_THIS)!=0) {
    if (DeeStringWriter_WRITE_STRING(&writer," [[thiscall]]") != 0) goto err_f_writer;
   }
   if (self->ast_function.f_scope && self->ast_function.f_scope->sc_supertp) {
    if (DeeStringWriter_Writef(&writer," [[super(%r)]]",self->ast_function.f_scope->sc_supertp) != 0) goto err_f_writer;
   }
   if (self->ast_function.f_name) {
    if (DeeStringWriter_Writef(&writer," [[name(%q)]]",
     DeeString_STR(self->ast_function.f_name)) != 0) goto err_f_writer;
   }
   end = (iter = begin = self->ast_function.f_argv)+self->ast_function.f_argc;
   if (DeeStringWriter_WriteChar(&writer,'(') != 0) goto err_f_writer;
   while (iter != end) {
    if (iter != begin && DeeStringWriter_WriteChar(&writer,',') != 0) goto err_f_writer;
    if (DeeStringWriter_Writef(&writer,"%K",_DeeLocalVar_ReprWithToken(iter->a_var,iter->a_token)) != 0) goto err_f_writer;
    ++iter;
   }
   if ((self->ast_function.f_flags&DEE_FUNCTION_FLAG_VARG)!=0) {
    if (DeeStringWriter_WRITE_STRING(&writer,"...") != 0) goto err_f_writer;
   }
   if (self->ast_function.f_code->ast_kind != DEE_SASTKIND_BLOCK) {
    if (DeeStringWriter_Writef(&writer,") /* symbolic */{\n\t%>r\n}",self->ast_function.f_code) != 0) goto err_f_writer;
   } else {
    if (DeeStringWriter_Writef(&writer,") %r",
     self->ast_function.f_code) != 0) goto err_f_writer;
   }
   result = DeeStringWriter_Pack(&writer);
end_f_writer: DeeStringWriter_Quit(&writer); return result;
err_f_writer: result = NULL; goto end_f_writer;
  } break;

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case DEE_XASTKIND_CLASS: {
   struct DeeXAstClassAstEntry *begin,*iter,*end;
   DeeStringWriter writer = DeeStringWriter_INIT();
   if (DeeStringWriter_WRITE_STRING(&writer,"class") != 0) goto err_c_writer;
   if (self->ast_class.c_name) {
    if (DeeStringWriter_Writef(&writer," [[name(%q)]]",
     DeeString_STR(self->ast_class.c_name)) != 0) goto err_c_writer;
   }
   if ((self->ast_class.c_tpflags&DEE_XAST_CLASSAST_FLAG_HAS_UUID)!=0) {
    if (DeeStringWriter_Writef(&writer," [[uuid(%R)]]",
     DeeUUID_ToString(&self->ast_class.c_uuid)) != 0) goto err_c_writer;
   }
   end = (iter = begin = self->ast_class.c_entryv)+self->ast_class.c_entryc;
   if (DeeStringWriter_WRITE_STRING(&writer," {\n") != 0) goto err_c_writer;
   while (iter != end) {
    switch (iter->ce_kind) {
     case DEE_XAST_CLASSENTRY_KIND_SLOT:
      if (DeeStringWriter_Writef(&writer,"\toperator %s = %>r;\n",
       _DeeType_ClassOperatorName(iter->ce_slot.cs_slotid),
       iter->ce_slot.cs_callback) != 0) goto err_c_writer;
      break;
     case DEE_XAST_CLASSENTRY_KIND_METHOD:
      if (DeeStringWriter_Writef(&writer,"\tfunction %s = %>r;\n",
       DeeString_STR(iter->ce_method.cm_name),
       iter->ce_method.cm_callback) != 0) goto err_c_writer;
      break;
     case DEE_XAST_CLASSENTRY_KIND_CLASS_METHOD:
      if (DeeStringWriter_Writef(&writer,"\tclass function %s = %>r;\n",
       DeeString_STR(iter->ce_method.cm_name),
       iter->ce_method.cm_callback) != 0) goto err_c_writer;
      break;
     case DEE_XAST_CLASSENTRY_KIND_CLASS_GETSET:
      if (DeeStringWriter_Writef(&writer,"\tclass property %s = {\n",
       DeeString_STR(iter->ce_getset.cg_name)) != 0) goto err_c_writer;
      goto write_c_property;
     case DEE_XAST_CLASSENTRY_KIND_GETSET:
      if (DeeStringWriter_Writef(&writer,"\tproperty %s = {\n",
       DeeString_STR(iter->ce_getset.cg_name)) != 0) goto err_c_writer;
write_c_property:
      if (iter->ce_getset.cg_get && DeeStringWriter_Writef(&writer,
       "\t\toperator get = %>>r;\n",iter->ce_getset.cg_get) != 0) goto err_c_writer;
      if (iter->ce_getset.cg_del && DeeStringWriter_Writef(&writer,
       "\t\toperator del = %>>r;\n",iter->ce_getset.cg_del) != 0) goto err_c_writer;
      if (iter->ce_getset.cg_set && DeeStringWriter_Writef(&writer,
       "\t\toperator set = %>>r;\n",iter->ce_getset.cg_set) != 0) goto err_c_writer;
      if (DeeStringWriter_WRITE_STRING(&writer,"\t};\n") != 0) goto err_c_writer;
      break;
     default: break;
    }
    iter->ce_getset;
    ++iter;
   }
   if (DeeStringWriter_WRITE_STRING(&writer,"}") != 0) goto err_c_writer;
   result = DeeStringWriter_Pack(&writer);
end_c_writer: DeeStringWriter_Quit(&writer); return result;
err_c_writer: result = NULL; goto end_c_writer;
  } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */

  case DEE_XASTKIND_TUPLE:
  case DEE_XASTKIND_LIST:
  case DEE_XASTKIND_SET: {
   struct DeeXAstObject **begin,**iter,**end;
   DeeStringWriter writer = DeeStringWriter_INIT();
   switch (self->ast_kind) {
    case DEE_XASTKIND_TUPLE: if (DeeStringWriter_WRITE_STRING(&writer,"pack(") != 0) goto err_s_writer; break;
    case DEE_XASTKIND_LIST:  if (DeeStringWriter_WRITE_STRING(&writer,"[") != 0) goto err_s_writer; break;
    case DEE_XASTKIND_SET:   if (DeeStringWriter_WRITE_STRING(&writer,"set { ") != 0) goto err_s_writer; break;
    default: DEE_BUILTIN_UNREACHABLE();
   }
   end = (iter = begin = self->ast_sequence.s_elemv)+self->ast_sequence.s_elemc;
   while (iter != end) {
    if (iter != begin && DeeStringWriter_WriteChar(&writer,',') != 0) goto err_s_writer;
    if (DeeStringWriter_WriteObjectRepr(&writer,(DeeObject *)*iter) != 0) goto err_s_writer;
    ++iter;
   }
   switch (self->ast_kind) {
    case DEE_XASTKIND_TUPLE: if (DeeStringWriter_WRITE_STRING(&writer,")") != 0) goto err_s_writer; break;
    case DEE_XASTKIND_LIST:  if (DeeStringWriter_WRITE_STRING(&writer,"]") != 0) goto err_s_writer; break;
    case DEE_XASTKIND_SET:   if (DeeStringWriter_WRITE_STRING(&writer," }") != 0) goto err_s_writer; break;
    default: DEE_BUILTIN_UNREACHABLE();
   }
   result = DeeStringWriter_Pack(&writer);
end_s_writer: DeeStringWriter_Quit(&writer); return result;
err_s_writer: result = NULL; goto end_s_writer;
  } break;

  case DEE_XASTKIND_DICT: {
   struct DeeXAstDictEntry *begin,*iter,*end;
   DeeStringWriter writer = DeeStringWriter_INIT();
   if (DeeStringWriter_WRITE_STRING(&writer,"dict { ") != 0) goto err_d_writer;
   end = (iter = begin = self->ast_dict.d_elemv)+self->ast_sequence.s_elemc;
   while (iter != end) {
    if (iter != begin && DeeStringWriter_WriteChar(&writer,',') != 0) goto err_d_writer;
    if (DeeStringWriter_WriteObjectRepr(&writer,(DeeObject *)iter->de_key) != 0) goto err_d_writer;
    if (DeeStringWriter_WRITE_STRING(&writer,": ") != 0) goto err_d_writer;
    if (DeeStringWriter_WriteObjectRepr(&writer,(DeeObject *)iter->de_item) != 0) goto err_d_writer;
    ++iter;
   }
   if (DeeStringWriter_WRITE_STRING(&writer," }") != 0) goto err_d_writer;
   result = DeeStringWriter_Pack(&writer);
end_d_writer: DeeStringWriter_Quit(&writer); return result;
err_d_writer: result = NULL; goto end_d_writer;
  } break;

  case DEE_XASTKIND_SEQ_RANGE_GET:
   return DeeString_Newf("(%r)[%r:%r]",self->ast_seq_range_get.sr_seq,
                         self->ast_seq_range_get.sr_begin ? (DeeObject *)self->ast_seq_range_get.sr_begin : Dee_None,
                         self->ast_seq_range_get.sr_end ? (DeeObject *)self->ast_seq_range_get.sr_end : Dee_None);
  case DEE_XASTKIND_SEQ_RANGE_DEL:
#if DEE_XCONFIG_PARSER_XAST_REPR_USE_SUBSCRIPT_OPERATORS
   return DeeString_Newf("(%r).operator del[] (%r,%r)",self->ast_seq_range_del.sr_seq,
                         self->ast_seq_range_del.sr_begin ? (DeeObject *)self->ast_seq_range_del.sr_begin : Dee_None,
                         self->ast_seq_range_del.sr_end ? (DeeObject *)self->ast_seq_range_del.sr_end : Dee_None);
#else
   return DeeString_Newf("__delrange__(%r,%r,%r)",self->ast_seq_range_del.sr_seq,
                         self->ast_seq_range_del.sr_begin ? (DeeObject *)self->ast_seq_range_del.sr_begin : Dee_None,
                         self->ast_seq_range_del.sr_end ? (DeeObject *)self->ast_seq_range_del.sr_end : Dee_None);
#endif
  case DEE_XASTKIND_SEQ_RANGE_SET:
   return DeeString_Newf("(%r)[%r:%r] = (%r)",self->ast_seq_range_set.sr_seq,
                         self->ast_seq_range_set.sr_begin ? (DeeObject *)self->ast_seq_range_set.sr_begin : Dee_None,
                         self->ast_seq_range_set.sr_end ? (DeeObject *)self->ast_seq_range_set.sr_end : Dee_None,
                         self->ast_seq_range_set.sr_value);
  case DEE_XASTKIND_ATTR_GET_C:
   return DeeString_Newf("(%r).%s",self->ast_attr_get_c.ac_object,
                         DeeString_STR(self->ast_attr_get_c.ac_name));
  case DEE_XASTKIND_ATTR_HAS_C:
   return DeeString_Newf("__hasattr__(%r,%q)",self->ast_attr_has_c.ac_object,
                         DeeString_STR(self->ast_attr_has_c.ac_name));
  case DEE_XASTKIND_ATTR_DEL_C:
#if DEE_XCONFIG_PARSER_XAST_REPR_USE_SUBSCRIPT_OPERATORS
   return DeeString_Newf("(%r).operator del. (%q)",self->ast_attr_del_c.ac_object,
                         DeeString_STR(self->ast_attr_del_c.ac_name));
#else
   return DeeString_Newf("__delattr__(%r,%q)",self->ast_attr_del_c.ac_object,
                         DeeString_STR(self->ast_attr_del_c.ac_name));
#endif
  case DEE_XASTKIND_ATTR_SET_C:
   return DeeString_Newf("(%r).%s = (%r)",self->ast_attr_set_c.ac_object,
                         DeeString_STR(self->ast_attr_set_c.ac_name),
                         self->ast_attr_set_c.ac_value);
  case DEE_XASTKIND_MODULE: DeeReturn_STATIC_STRING("module");
  case DEE_XASTKIND_DEL_VAR:
   return DeeString_Newf("__builtin_delvar(%K)",
                         _DeeLocalVar_ReprWithToken(self->ast_delvar.d_var,self->ast_common.ast_token));
#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
  case DEE_XASTKIND_FOREIGN_FUNCTION:
   return DeeString_Newf("%r(TODO)",
                         self->ast_foreignfunction.ff_base);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */
  case DEE_XASTKIND_SWITCH: {
   DeeObject *result; int first = 1;
   struct DeeXAstSwitchEntry *iter,*end;
   DeeStringWriter writer = DeeStringWriter_INIT();
   if (DeeStringWriter_Writef(&writer,"switch (%r) { ",
    self->ast_switch.s_expr) != 0) goto err_switch_writer;
   DEE_ASSERT(self->ast_switch.s_entryc);
   end = (iter = self->ast_switch.s_entryv)+self->ast_switch.s_entryc;
   do {
    if (first) first = 0; else if (DeeStringWriter_WRITE_STRING(
     &writer,", ") != 0) goto err_switch_writer;
    if (DeeStringWriter_Writef(&writer,"%r: %r",
     iter->se_key,iter->se_value) != 0) goto err_switch_writer;
   } while (++iter != end);
   if (self->ast_switch.s_default && DeeStringWriter_Writef(
    &writer,", default: %r",self->ast_switch.s_default) != 0) goto err_switch_writer;
   if (DeeStringWriter_WRITE_STRING(&writer," }") != 0) goto err_switch_writer;
   result = DeeStringWriter_Pack(&writer);
end_switch_writer: DeeStringWriter_Quit(&writer);
   return result;
err_switch_writer: result = NULL; goto end_switch_writer;
  } break;

  case DEE_XASTKIND_IFTRUE:
   return DeeString_Newf("/* constant */true ? (%r) : (%r)",
                         self->ast_iftrue.ic_succ ? (DeeObject *)self->ast_iftrue.ic_succ : Dee_None,
                         self->ast_iftrue.ic_fail ? (DeeObject *)self->ast_iftrue.ic_fail : Dee_None);
  case DEE_XASTKIND_IFFALSE:
   return DeeString_Newf("/* constant */false ? (%r) : (%r)",
                         self->ast_iftrue.ic_succ ? (DeeObject *)self->ast_iftrue.ic_succ : Dee_None,
                         self->ast_iftrue.ic_fail ? (DeeObject *)self->ast_iftrue.ic_fail : Dee_None);

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
  case DEE_XASTKIND_BUILTIN_BREAKPOINT: DeeReturn_STATIC_STRING("__builtin_breakpoint()");
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
  case DEE_XASTKIND_BUILTIN_UNREACHABLE: DeeReturn_STATIC_STRING("__builtin_unreachable()");
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case DEE_XASTKIND_BUILTIN_BOUND:
   return DeeString_Newf("__builtin_bound(%K)",_DeeLocalVar_ReprWithToken(
    self->ast_builtin_bound.b_var,self->ast_builtin_bound.b_token));
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
  case DEE_XASTKIND_BUILTIN_EXPECT:
   return DeeString_Newf("__builtin_expect(%r,%r)",
                         self->ast_builtin_expect.e_value,
                         self->ast_builtin_expect.e_expect);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */
  case DEE_XASTKIND_SUPER_AT:
   return DeeString_Newf("__super(%r,%r)",self->ast_operator.op_a,self->ast_operator.op_b);

  case DEE_XASTKIND_VAR_INC:     return DeeString_Newf("/* inplace-var */++%K",_DeeLocalVar_ReprWithToken(self->ast_unary_var.uiv_var,self->ast_unary_var.uiv_tok));
  case DEE_XASTKIND_VAR_DEC:     return DeeString_Newf("/* inplace-var */--%K",_DeeLocalVar_ReprWithToken(self->ast_unary_var.uiv_var,self->ast_unary_var.uiv_tok));
  case DEE_XASTKIND_VAR_INCPOST: return DeeString_Newf("/* inplace-var */%K++",_DeeLocalVar_ReprWithToken(self->ast_unary_var.uiv_var,self->ast_unary_var.uiv_tok));
  case DEE_XASTKIND_VAR_DECPOST: return DeeString_Newf("/* inplace-var */%K--",_DeeLocalVar_ReprWithToken(self->ast_unary_var.uiv_var,self->ast_unary_var.uiv_tok));
  case DEE_XASTKIND_VAR_IADD:    return DeeString_Newf("/* inplace-var */%K += (%r)",_DeeLocalVar_ReprWithToken(self->ast_binary_var.biv_var,self->ast_binary_var.biv_tok),self->ast_binary_var.biv_arg);
  case DEE_XASTKIND_VAR_ISUB:    return DeeString_Newf("/* inplace-var */%K -= (%r)",_DeeLocalVar_ReprWithToken(self->ast_binary_var.biv_var,self->ast_binary_var.biv_tok),self->ast_binary_var.biv_arg);
  case DEE_XASTKIND_VAR_IMUL:    return DeeString_Newf("/* inplace-var */%K *= (%r)",_DeeLocalVar_ReprWithToken(self->ast_binary_var.biv_var,self->ast_binary_var.biv_tok),self->ast_binary_var.biv_arg);
  case DEE_XASTKIND_VAR_IDIV:    return DeeString_Newf("/* inplace-var */%K /= (%r)",_DeeLocalVar_ReprWithToken(self->ast_binary_var.biv_var,self->ast_binary_var.biv_tok),self->ast_binary_var.biv_arg);
  case DEE_XASTKIND_VAR_IMOD:    return DeeString_Newf("/* inplace-var */%K %= (%r)",_DeeLocalVar_ReprWithToken(self->ast_binary_var.biv_var,self->ast_binary_var.biv_tok),self->ast_binary_var.biv_arg);
  case DEE_XASTKIND_VAR_ISHL:    return DeeString_Newf("/* inplace-var */%K <<= (%r)",_DeeLocalVar_ReprWithToken(self->ast_binary_var.biv_var,self->ast_binary_var.biv_tok),self->ast_binary_var.biv_arg);
  case DEE_XASTKIND_VAR_ISHR:    return DeeString_Newf("/* inplace-var */%K >>= (%r)",_DeeLocalVar_ReprWithToken(self->ast_binary_var.biv_var,self->ast_binary_var.biv_tok),self->ast_binary_var.biv_arg);
  case DEE_XASTKIND_VAR_IAND:    return DeeString_Newf("/* inplace-var */%K &= (%r)",_DeeLocalVar_ReprWithToken(self->ast_binary_var.biv_var,self->ast_binary_var.biv_tok),self->ast_binary_var.biv_arg);
  case DEE_XASTKIND_VAR_IOR:     return DeeString_Newf("/* inplace-var */%K |= (%r)",_DeeLocalVar_ReprWithToken(self->ast_binary_var.biv_var,self->ast_binary_var.biv_tok),self->ast_binary_var.biv_arg);
  case DEE_XASTKIND_VAR_IXOR:    return DeeString_Newf("/* inplace-var */%K ^= (%r)",_DeeLocalVar_ReprWithToken(self->ast_binary_var.biv_var,self->ast_binary_var.biv_tok),self->ast_binary_var.biv_arg);
  case DEE_XASTKIND_VAR_IPOW:    return DeeString_Newf("/* inplace-var */%K **= (%r)",_DeeLocalVar_ReprWithToken(self->ast_binary_var.biv_var,self->ast_binary_var.biv_tok),self->ast_binary_var.biv_arg);

  default:
   switch (DEE_XASTKIND_OPCOUNT(self->ast_kind)) {
    case 1: return DeeXAst_UnaryDescrStr(self->ast_kind,self->ast_operator.op_a);
    case 2: return DeeXAst_BinaryDescrStr(self->ast_kind,self->ast_operator.op_a,self->ast_operator.op_b);
    case 3: return DeeXAst_TrinaryDescrStr(self->ast_kind,self->ast_operator.op_a,self->ast_operator.op_b,self->ast_operator.op_c);
    default: break;
   }
   break;
 }
 DeeReturn_STATIC_STRING("operator ? ()");
}
DEE_COMPILER_MSVC_WARNING_POP

DEE_DECL_END
