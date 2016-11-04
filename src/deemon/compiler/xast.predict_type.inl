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

DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL DeeTypeObject const *
DeeXAst_PredictType(DEE_A_IN DeeXAstObject const *self) {
 DeeTypeObject const *result,*ta,*tb;
 DEE_ASSERT(DeeObject_Check(self) && DeeXAst_Check(self));
 switch (self->ast_kind) {
  case DEE_XASTKIND_ATTR_DEL:
  case DEE_XASTKIND_ATTR_DEL_C:
  case DEE_XASTKIND_SEQ_RANGE_DEL:
  case DEE_XASTKIND_DEL_VAR:
  case DEE_XASTKIND_MODULE:
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT
  case DEE_XASTKIND_BUILTIN_BREAKPOINT:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BREAKPOINT */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE
  case DEE_XASTKIND_BUILTIN_UNREACHABLE:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_UNREACHABLE */
#if DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE
  case DEE_XASTKIND_DELETE:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE */
  case DEE_XASTKIND_IO_FLUSH:
  case DEE_XASTKIND_IO_TRUNC:
  case DEE_XASTKIND_IO_CLOSE:
   return (DeeTypeObject *)&DeeNone_Type;

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  case DEE_XASTKIND_IO_READP:
  case DEE_XASTKIND_IO_WRITEP:
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  case DEE_XASTKIND_IO_WRITENP:
   return DeeObject_TYPE(Dee_size_t);

  case DEE_XASTKIND_IO_SEEK:
   return (DeeTypeObject *)&DeeUInt64_Type;

  case DEE_XASTKIND_CONST:
   return Dee_TYPE(self->ast_const.c_const);
  case DEE_XASTKIND_VAR:
   return NULL; // TODO: Type-bound variables
  case DEE_XASTKIND_VARDECL:
   return DeeXAst_PredictType(self->ast_vardecl.vd_init);
  case DEE_XASTKIND_MULTIVARDECL:
   switch (self->ast_multivardecl.mvd_type) {
    case DEE_XASTKIND_TUPLE: return &DeeTuple_Type;
    case DEE_XASTKIND_LIST: return &DeeList_Type;
    case DEE_XASTKIND_SET: return &DeeSet_Type;
    default: break;
   }
   break;
  case DEE_XASTKIND_STATEMENT:
   return DeeSAst_PredictType(self->ast_statement.s_stmt);
  case DEE_XASTKIND_IF: {
   if ((ta = self->ast_if.if_succ
    ? DeeXAst_PredictType(self->ast_if.if_succ)
    : DeeXAst_PredictType(self->ast_if.if_cond)) == NULL) return NULL;
   if (!self->ast_if.if_fail) tb = (DeeTypeObject const *)&DeeNone_Type;
   else if ((tb = DeeXAst_PredictType(self->ast_if.if_fail)) == NULL) return NULL;
   return ta == tb ? ta : NULL;
  } break;
  case DEE_XASTKIND_FUNCTION: return &DeeFunction_Type;
  case DEE_XASTKIND_TUPLE: return &DeeTuple_Type;
  case DEE_XASTKIND_LIST: return &DeeList_Type;
  case DEE_XASTKIND_SET: return &DeeSet_Type;
  case DEE_XASTKIND_DICT: return &DeeDict_Type;

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case DEE_XASTKIND_CLASS:
   //DeeClassType_TypeOf(); // We can't use this because it returns a reference
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
  case DEE_XASTKIND_TYPE:
  case DEE_XASTKIND_CLASSOF:
   return &DeeType_Type; // Returns some kind of type
  case DEE_XASTKIND_WEAK:   return &DeeWeakref_Type;
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case DEE_XASTKIND_REF:    return (DeeTypeObject *)&DeePointer_Type;
  case DEE_XASTKIND_DEREF:  return (DeeTypeObject *)&DeeLValue_Type;
  case DEE_XASTKIND_PTROF:  return &DeePointerType_Type;
  case DEE_XASTKIND_LVALOF: return &DeeLValueType_Type;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
#if DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
  case DEE_XASTKIND_VARRAYOF: return &DeeVarArrayType_Type;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ARRAYS */

  case DEE_XASTKIND_EXPAND:
   if ((result = DeeXAst_PredictType(self->ast_operator.op_a)) == NULL) return NULL;
   DeeType_UnaryAstAttr(result,DeeType_SLOT_ID(tp_seq_iter_self),DEE_AST_ATTRIBUTE_FLAG_NONE,&result);
   if (!result) return NULL;
   DeeType_UnaryAstAttr(result,DeeType_SLOT_ID(tp_seq_iter_next),DEE_AST_ATTRIBUTE_FLAG_NONE,&result);
   return result;

  case DEE_XASTKIND_STR:
  case DEE_XASTKIND_REPR:
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP
  case DEE_XASTKIND_BUILTIN_HELP:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP */
  case DEE_XASTKIND_IO_READNP:
   return &DeeString_Type;

  case DEE_XASTKIND_CELL:
   return &DeeCell_Type;

  case DEE_XASTKIND_COPY:
  case DEE_XASTKIND_MOVE:
  case DEE_XASTKIND_MOVE_ASSIGN:
  case DEE_XASTKIND_COPY_ASSIGN:
   return DeeXAst_PredictType(self->ast_operator.op_a);

  case DEE_XASTKIND_SEQ_SET:
  case DEE_XASTKIND_ATTR_SET:
   return DeeXAst_PredictType(self->ast_operator.op_c);

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA
  case DEE_XASTKIND_BUILTIN_ALLOCA:
   if ((result = DeeType_Pointer((DeeTypeObject *)
    &DeeNone_Type)) == NULL) DeeError_Handled();
   return result;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_ALLOCA */

  case DEE_XASTKIND_BOOL:
  case DEE_XASTKIND_ATTR_HAS_C:
  case DEE_XASTKIND_IS:
  case DEE_XASTKIND_NOT_IN:
  case DEE_XASTKIND_NOT_IS:
  case DEE_XASTKIND_LAND:
  case DEE_XASTKIND_LOR:
  case DEE_XASTKIND_LXOR:
  case DEE_XASTKIND_ATTR_HAS:
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case DEE_XASTKIND_BUILTIN_BOUND:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */
  case DEE_XASTKIND_SEQ_ANY:
  case DEE_XASTKIND_SEQ_ALL:
   return &DeeBool_Type;

#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
  case DEE_XASTKIND_FOREIGN_FUNCTION:
   return &DeeForeignFunctionType_Type;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */

  case DEE_XASTKIND_SWITCH: {
   struct DeeXAstSwitchEntry *iter,*end;
   if (self->ast_switch.s_default) {
    if ((result = DeeXAst_PredictType(self->ast_switch.s_default)) == NULL) return NULL;
   } else result = (DeeTypeObject *)&DeeNone_Type;
   end = (iter = self->ast_switch.s_entryv)+self->ast_switch.s_entryc;
   do if ((ta = DeeXAst_PredictType(iter->se_value)) == NULL || ta != result) return NULL;
   while (++iter != end);
   return result;
  } break;

  case DEE_XASTKIND_IFTRUE:
   return self->ast_iftrue.ic_succ
    ? DeeXAst_PredictType(self->ast_iftrue.ic_succ)
    : (DeeTypeObject *)&DeeNone_Type;
  case DEE_XASTKIND_IFFALSE:
   return self->ast_iffalse.ic_fail
    ? DeeXAst_PredictType(self->ast_iffalse.ic_fail)
    : (DeeTypeObject *)&DeeNone_Type;

#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
  case DEE_XASTKIND_BUILTIN_EXTERN:
   // __builtin_extern returns an structured lvalue based on the third argument
   // >> '__builtin_extern("foo.dll","bar",int) --> int &'
   if (self->ast_builtin_extern.ex_type->ast_kind == DEE_XASTKIND_CONST &&
       DeeStructuredType_Check(self->ast_builtin_extern.ex_type->ast_const.c_const)) {
    if ((result = DeeType_LValue((DeeTypeObject *)
     self->ast_builtin_extern.ex_type->ast_const.c_const)) == NULL) DeeError_Handled();
    return result;
   }
   return NULL;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */

#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE
  case DEE_XASTKIND_BUILTIN_FF_CLOSURE:
   if (self->ast_operator.op_a->ast_kind == DEE_XASTKIND_CONST &&
       DeeType_Check(self->ast_operator.op_a->ast_const.c_const)
       ) return (DeeTypeObject *)self->ast_operator.op_a->ast_const.c_const;
   return NULL;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTIONCLOSURE */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEX
  case DEE_XASTKIND_BUILTIN_DEX:
   return &DeeDexModule_Type;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_DEX */

  case DEE_XASTKIND_SUPER_AT:
   return self->ast_operator.op_a->ast_kind == DEE_XASTKIND_CONST
    ? (DeeTypeObject *)self->ast_operator.op_a->ast_const.c_const : NULL;

#if DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE
  case DEE_XASTKIND_NEW:
   if (self->ast_operator.op_a->ast_kind == DEE_XASTKIND_CONST) {
    if ((result = DeeType_Pointer((DeeTypeObject *)
     self->ast_operator.op_a->ast_const.c_const)) == NULL) DeeError_Handled();
    return result;
   }
   return NULL;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_NEWDELETE */

  case DEE_XASTKIND_BUILTIN_UNUSED:
   return DeeXAst_PredictType(self->ast_operator.op_b);

  case DEE_XASTKIND_ATTR_GET_C:
   if ((ta = DeeXAst_PredictType(self->ast_attr_get_c.ac_object)) == NULL) return NULL;
   DeeType_GetAttrAstAttr(ta,DEE_AST_ATTRIBUTE_FLAG_NONE,
                          (DeeObject *)self->ast_attr_get_c.ac_name,&result);
   return result;

  case DEE_XASTKIND_ATTR_SET_C:
   return DeeXAst_PredictType(self->ast_attr_set_c.ac_value);

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
  case DEE_XASTKIND_BUILTIN_EXPECT:
   return DeeXAst_PredictType(self->ast_builtin_expect.e_value);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */

  case DEE_XASTKIND_SEQ_RANGE_GET: {
   if ((ta = DeeXAst_PredictType(self->ast_seq_range_get.sr_seq)) == NULL) return NULL;
   DeeType_GetRangeAstAttr(ta,DEE_AST_ATTRIBUTE_FLAG_NONE,
                           self->ast_seq_range_get.sr_begin ? DeeXAst_PredictType(self->ast_seq_range_get.sr_begin) : (DeeTypeObject *)&DeeNone_Type,
                           self->ast_seq_range_get.sr_end ? DeeXAst_PredictType(self->ast_seq_range_get.sr_end) : (DeeTypeObject *)&DeeNone_Type,
                           &result);
   return result;
  } break;

  case DEE_XASTKIND_SEQ_RANGE_SET:
   return DeeXAst_PredictType(self->ast_seq_range_set.sr_value);

  case DEE_XASTKIND_IN:
   if ((ta = DeeXAst_PredictType(self->ast_operator.op_b)) != NULL) {
    DeeType_BinaryAstAttr(ta,DeeType_SLOT_ID(tp_seq_contains),DEE_AST_ATTRIBUTE_FLAG_NONE,
                          DeeXAst_PredictType(self->ast_operator.op_a),&result);
    return result;
   }
   break;

  case DEE_XASTKIND_CALL: {
   DeeTypeObject const **argv,**iter,**end; Dee_size_t argc;
   // Check for a constructor call
   if (self->ast_operator.op_a->ast_kind == DEE_XASTKIND_CONST &&
       DeeType_Check(self->ast_operator.op_a->ast_const.c_const)
       ) return (DeeTypeObject *)self->ast_operator.op_a->ast_const.c_const;
   if (self->ast_operator.op_a->ast_kind == DEE_XASTKIND_ATTR_GET_C) {
    if ((ta = DeeXAst_PredictType(self->ast_operator.op_a->ast_attr_get_c.ac_object)) == NULL) return NULL;
    switch (self->ast_operator.op_b->ast_kind) {
     case DEE_XASTKIND_CONST: {
      DeeObject **src,*args = self->ast_operator.op_b->ast_const.c_const;
      if (!DeeTuple_Check(args)) goto default_predict;
      if ((argc = DeeTuple_SIZE(args)) != 0) {
       while ((argv = (DeeTypeObject const **)malloc_nz(
        argc*sizeof(DeeTypeObject *))) == NULL) {
        if DEE_LIKELY(Dee_CollectMemory()) continue;
        return NULL; // Don't report this error...
       }
       end = (iter = argv)+argc;
       src = DeeTuple_ELEM(args);
       do *iter++ = Dee_TYPE(*src++); while (iter != end);
      } else argv = NULL;
      if (self->ast_operator.op_a->ast_attr_get_c.ac_object->ast_kind == DEE_XASTKIND_CONST) {
       DeeObject *constant = self->ast_operator.op_a->ast_attr_get_c.ac_object->ast_const.c_const;
       DEE_ASSERT(ta == Dee_TYPE(constant));
       DeeObject_CallAttrAstAttr(ta,constant,DEE_AST_ATTRIBUTE_FLAG_NONE,
                                 (DeeObject *)self->ast_operator.op_a->ast_attr_get_c.ac_name,
                                 argc,argv,&result);
      } else {
       DeeType_CallAttrAstAttr(ta,DEE_AST_ATTRIBUTE_FLAG_NONE,
                               (DeeObject *)self->ast_operator.op_a->ast_attr_get_c.ac_name,
                               argc,argv,&result);
      }
      free((void *)argv);
      return result;
     } break;
     case DEE_XASTKIND_TUPLE: {
      DeeXAstObject **src = self->ast_operator.op_b->ast_sequence.s_elemv;
      DEE_ASSERT(self->ast_operator.op_b->ast_sequence.s_elemc != 0);
      DEE_ASSERT(self->ast_operator.op_b->ast_sequence.s_elemv != NULL);
      argc = self->ast_operator.op_b->ast_sequence.s_elemc;
      while ((argv = (DeeTypeObject const **)malloc_nz(
       argc*sizeof(DeeTypeObject *))) == NULL) {
       if DEE_LIKELY(Dee_CollectMemory()) continue;
       return NULL; // Don't report this error...
      }
      end = (iter = argv)+argc;
      do *iter++ = DeeXAst_PredictType(*src++);
      while (iter != end);
      if (self->ast_operator.op_a->ast_attr_get_c.ac_object->ast_kind == DEE_XASTKIND_CONST) {
       DeeObject *constant = self->ast_operator.op_a->ast_attr_get_c.ac_object->ast_const.c_const;
       DEE_ASSERT(ta == Dee_TYPE(constant));
       DeeObject_CallAttrAstAttr(ta,constant,DEE_AST_ATTRIBUTE_FLAG_NONE,
                                 (DeeObject *)self->ast_operator.op_a->ast_attr_get_c.ac_name,
                                 argc,argv,&result);
      } else {
       DeeType_CallAttrAstAttr(ta,DEE_AST_ATTRIBUTE_FLAG_NONE,
                               (DeeObject *)self->ast_operator.op_a->ast_attr_get_c.ac_name,
                               argc,argv,&result);
      }
      free_nn((void *)argv);
      return result;
     } break;
     default: break;
    }
   } else {
    if ((ta = DeeXAst_PredictType(self->ast_operator.op_a)) == NULL) return NULL;
    switch (self->ast_operator.op_b->ast_kind) {
     case DEE_XASTKIND_CONST: {
      DeeObject **src,*args = self->ast_operator.op_b->ast_const.c_const;
      if (!DeeTuple_Check(args)) goto default_predict;
      if ((argc = DeeTuple_SIZE(args)) != 0) {
       while ((argv = (DeeTypeObject const **)malloc_nz(
        argc*sizeof(DeeTypeObject *))) == NULL) {
        if DEE_LIKELY(Dee_CollectMemory()) continue;
        return NULL; // Don't report this error...
       }
       end = (iter = argv)+argc;
       src = DeeTuple_ELEM(args);
       do *iter++ = Dee_TYPE(*src++); while (iter != end);
      } else argv = NULL;
      if (self->ast_operator.op_a->ast_kind == DEE_XASTKIND_CONST) {
       DeeObject *constant = self->ast_operator.op_a->ast_const.c_const;
       DEE_ASSERT(ta == Dee_TYPE(constant));
       DeeObject_CallAstAttr(ta,constant,DEE_AST_ATTRIBUTE_FLAG_NONE,argc,argv,&result);
      } else {
       DeeType_CallAstAttr(ta,DEE_AST_ATTRIBUTE_FLAG_NONE,argc,argv,&result);
      }
      free((void *)argv);
      return result;
     } break;
     case DEE_XASTKIND_TUPLE: {
      DeeXAstObject **src = self->ast_operator.op_b->ast_sequence.s_elemv;
      DEE_ASSERT(self->ast_operator.op_b->ast_sequence.s_elemc != 0);
      DEE_ASSERT(self->ast_operator.op_b->ast_sequence.s_elemv != NULL);
      argc = self->ast_operator.op_b->ast_sequence.s_elemc;
      while ((argv = (DeeTypeObject const **)malloc_nz(
       argc*sizeof(DeeTypeObject *))) == NULL) {
       if DEE_LIKELY(Dee_CollectMemory()) continue;
       return NULL; // Don't report this error...
      }
      end = (iter = argv)+argc;
      do *iter++ = DeeXAst_PredictType(*src++);
      while (iter != end);
      if (self->ast_operator.op_a->ast_kind == DEE_XASTKIND_CONST) {
       DeeObject *constant = self->ast_operator.op_a->ast_const.c_const;
       DEE_ASSERT(ta == Dee_TYPE(constant));
       DeeObject_CallAstAttr(ta,constant,DEE_AST_ATTRIBUTE_FLAG_NONE,argc,argv,&result);
      } else {
       DeeType_CallAstAttr(ta,DEE_AST_ATTRIBUTE_FLAG_NONE,argc,argv,&result);
      }
      free_nn((void *)argv);
      return result;
     } break;
     default: break;
    }
   }
   goto default_predict;
  } break;

  DEE_XASTKIND_CASE_INPLACE_VAR_UNARY {
#if 0
   // TODO: Predict type of variable using assumptions
   if ((ta = DeeXAst_PredictType(self->ast_unary_var.uiv_var)) != NULL) {
    DeeType_UnaryAstAttr(ta,DeeType_SlotIdFromXAstKind(self->ast_kind),
                         DEE_AST_ATTRIBUTE_FLAG_NONE,&result);
    return result;
   } else
#endif
   {
    return NULL;
   }
  } break;

  DEE_XASTKIND_CASE_INPLACE_VAR_BINARY {
#if 0
   // TODO: Predict type of variable using assumptions
   if ((ta = DeeXAst_PredictType(self->ast_binary_var.biv_var)) != NULL) {
    DeeType_BinaryAstAttr(ta,DeeType_SlotIdFromXAstKind(self->ast_kind),DEE_AST_ATTRIBUTE_FLAG_NONE,
                          DeeXAst_PredictType(self->ast_binary_var.biv_arg),&result);
    return result;
   } else
#endif
   {
    return NULL;
   }
  } break;


  default:default_predict:
   if (DEE_XASTKIND_ISOPERATOR(self->ast_kind)) {
   switch (DEE_XASTKIND_OPCOUNT(self->ast_kind)) {
    case 3:
     if ((ta = DeeXAst_PredictType(self->ast_operator.op_a)) != NULL) {
      DeeType_TrinaryAstAttr(ta,DeeType_SlotIdFromXAstKind(self->ast_kind),DEE_AST_ATTRIBUTE_FLAG_NONE,
                             DeeXAst_PredictType(self->ast_operator.op_b),
                             DeeXAst_PredictType(self->ast_operator.op_c),&result);
      return result;
     }
     break;
    case 2:
     if ((ta = DeeXAst_PredictType(self->ast_operator.op_a)) != NULL) {
      DeeType_BinaryAstAttr(ta,DeeType_SlotIdFromXAstKind(self->ast_kind),DEE_AST_ATTRIBUTE_FLAG_NONE,
                            DeeXAst_PredictType(self->ast_operator.op_b),&result);
      return result;
     }
     break;
    case 1:
     if ((ta = DeeXAst_PredictType(self->ast_operator.op_a)) != NULL) {
      DeeType_UnaryAstAttr(ta,DeeType_SlotIdFromXAstKind(self->ast_kind),
                           DEE_AST_ATTRIBUTE_FLAG_NONE,&result);
      return result;
     }
     break;
    default: break;
   }
  } break;
 }
 return NULL;
}



DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL DeeTypeObject const *
_DeeType_IteratorType(DEE_A_IN DeeTypeObject const *seq_type) {
 DEE_ASSERT(DeeObject_Check(seq_type) && DeeType_Check(seq_type));
 // Map known iterator types
 if (seq_type == &DeeTuple_Type) return &DeeTupleIterator_Type;
 if (seq_type == &DeeList_Type) return &DeeListIterator_Type;
 if (seq_type == &DeeSet_Type) return &DeeSetIterator_Type;
 if (seq_type == &DeeDict_Type) return &DeeDictIterator_Type;
 if (seq_type == &DeeDictKeys_Type) return &DeeDictKeysIterator_Type;
 if (seq_type == &DeeDictItems_Type) return &DeeDictItemsIterator_Type;
 if (seq_type == &DeeDictValues_Type) return &DeeDictIterator_Type;
 if (seq_type == &DeeCell_Type) return &DeeCell_Type;
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (seq_type == &DeeWideString_Type) return &DeeWideStringIterator_Type;
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (seq_type == &DeeUtf8String_Type) return &DeeUtf8StringIterator_Type;
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (seq_type == &DeeUtf16String_Type) return &DeeUtf16StringIterator_Type;
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (seq_type == &DeeUtf32String_Type) return &DeeUtf32StringIterator_Type;
#endif
 if (seq_type == &DeeYieldFunction_Type) return &DeeYieldFunctionIterator_Type;
 if (seq_type == &DeeGenericRangeIterator_Type) return &DeeGenericRangeIterator_Type;
 if (seq_type == &DeeGenericSequenceIterator_Type) return &DeeGenericSequenceIterator_Type;
 if (seq_type == &DeeSubRangeIterator_Type) return &DeeSubRangeIterator_Type;
 if (seq_type == &DeeZipIterator_Type) return &DeeZipIterator_Type;
 if (DeeType_IsSameOrDerived(seq_type,(DeeTypeObject *)&DeeFile_Type)) return &DeeFileIterator_Type;
 if (seq_type == &DeeFSDir_Type) return &DeeFSDirIterator_Type;
 if (seq_type == &DeeFSQuery_Type) return &DeeFSQueryIterator_Type;
 if (DeeType_GET_SLOT(seq_type,tp_seq_iter_self) == &DeeObject_Copy ||
     DeeType_GET_SLOT(seq_type,tp_seq_iter_self) == &_DeeGeneric_ReturnSelf
     ) return seq_type; // This applies to a bunch of types
 return NULL;
}




DEE_DECL_END

