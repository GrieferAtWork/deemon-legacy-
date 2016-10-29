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

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewUnary(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeXAstObject *ast_a) {
 DeeXAstObject *result; DeeObject *const_copy,*unary_result;
 DEE_ASSERT(DEE_XASTKIND_ISUNARY(kind));
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(ast_a) && DeeXAst_Check(ast_a));
 // TODO: '[10,20,30]+...' --> 'pack(10,20,30)+...'
 //       --> Convert all sequences into tuples (they're the fastest for sum)
 // TODO: '(a)+...' --> 'a' // Sum one does nothing
 // TODO: '(a,b,c)+...' --> 'a+b+c'
 // TODO: '(a,b,c)&&...' --> 'a && b && c'
 // TODO: '(a,b,c)||...' --> 'a || b || c'
 // TODO: '"" + a + b + c + d' --> ("",a,b,c,d) + ... // Allows use for a much faster StringWriter at runtime

 switch (kind) {
  case DEE_XASTKIND_CELL: {
   if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE)!=0 &&
       ast_a->ast_kind == DEE_XASTKIND_EXPAND) {
    // Optimize '<[]...>' -> '<>' (as well as with all other constant sequences)
    // Optimize '<[42]...>' -> '<42>' (as well as with all other constant sequences)
    DeeXAstObject *sequence_ast = ast_a->ast_operator.op_a;
    switch (sequence_ast->ast_kind) {
     case DEE_XASTKIND_CONST: {
      DeeObject *temp,*seq_iterator; int error;
      if ((temp = DeeObject_DeepCopy(
       sequence_ast->ast_const.c_const)) == NULL) {
handle_error_and_return_normal: DeeError_Handled(); goto normal_ast;
      }
      seq_iterator = DeeObject_IterSelf(temp);
      Dee_DECREF(temp);
      if (!seq_iterator) goto handle_error_and_return_normal;
      error = DeeObject_IterNextEx(seq_iterator,&temp);
      if (error < 0) {handle_error_iterator_and_return_normal: Dee_DECREF(seq_iterator); goto handle_error_and_return_normal; }
      if (error == 0) {
       DeeObject *null_element;
       // Might be a single-element cell
       error = DeeObject_IterNextEx(seq_iterator,&null_element);
       if (error < 0) { Dee_DECREF(temp); goto handle_error_iterator_and_return_normal; }
       Dee_DECREF(seq_iterator);
       if (error == 0) {
        // non-single element cell
        Dee_DECREF(null_element);
        Dee_DECREF(temp);
        if (DeeError_CompilerError(DEE_WARNING_INVALID_CONSTOP,
         (DeeObject *)lexer,(DeeObject *)ast_a->ast_common.ast_token,
         "Creating a constant cell with more than one element will cause "
         "an Error.ValueError at runtime") != 0) return NULL;
        goto normal_ast;
       }
       // Single element cell
       null_element = DeeCell_New(temp);
       Dee_DECREF(temp);
       if (!null_element) goto handle_error_and_return_normal;
       result = DeeXAst_NewConst(tk,null_element);
       Dee_DECREF(null_element);
       return result;
      } else {
       Dee_DECREF(seq_iterator);
       if ((temp = DeeCell_NewEmpty()) == NULL) goto handle_error_and_return_normal;
       result = DeeXAst_NewConst(tk,temp);
       Dee_DECREF(temp);
       return result;
      }
     } break;
    }
   }
  } break;

  case DEE_XASTKIND_EXPAND:
   if (ast_a->ast_kind == DEE_XASTKIND_EXPAND) {
    if (DeeError_CompilerErrorf(DEE_WARNING_DOUBLE_EXPAND_REQUIRES_INNER_EXPAND_SINGLE,
     (DeeObject *)lexer,(DeeObject *)tk,
     "This expression requires the inner expression to be iterable with a size of 1:\n"
     "\t(%>r)...",ast_a) != 0) return NULL;
   } else if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE)!=0) {
    // Unpack non-constant sequence expression, where we
    // know that the sequence will have a length of '1'
    switch (ast_a->ast_kind) {
     case DEE_XASTKIND_TUPLE:
     case DEE_XASTKIND_LIST:
     case DEE_XASTKIND_SET:
      // Generic sequence
      if (ast_a->ast_sequence.s_elemc == 1) {
       Dee_INCREF(result = ast_a->ast_sequence.s_elemv[0]);
       return result;
      }
      break;
     case DEE_XASTKIND_DICT:
      if (ast_a->ast_dict.d_elemc == 1) {
       DeeXAstObject *seq_asts[2];
       seq_asts[0] = ast_a->ast_dict.d_elemv[0].de_key;
       seq_asts[1] = ast_a->ast_dict.d_elemv[0].de_item; // Dicts yield tuples
       return DeeXAst_NewSequence(DEE_XASTKIND_TUPLE,tk,lexer,parser_flags,2,seq_asts);
      }
      break;
     case DEE_XASTKIND_CELL:
      // Cell element
      Dee_INCREF(result = ast_a->ast_operator.op_a);
      return result;
     default: break;
    }
   }
   break;
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case DEE_XASTKIND_DEREF:
   if (ast_a->ast_kind == DEE_XASTKIND_REF) {
    // ref+deref cancel each other out (return the inner AST of 'ast_a')
    Dee_INCREF(ast_a->ast_operator.op_a);
    return ast_a->ast_operator.op_a;
   }
   break;
  case DEE_XASTKIND_REF:
   if (ast_a->ast_kind == DEE_XASTKIND_ATTR_GET_C ||
       ast_a->ast_kind == DEE_XASTKIND_SEQ_GET) {
    Dee_size_t final_offset; DeeTypeObject *final_type;
    if (DeeXAst_GetOffsetof(ast_a,&final_offset,&final_type)) {
     DeeObject *offset_constant;
     DEE_LVERBOSE1R("%s(%d) : %k : Detected offsetof (final_offset: %Iu; final_type: %q)\n",
                    DeeToken_FILE(tk),DeeToken_LINE(tk)+1,tk,final_offset,DeeType_NAME(final_type));
     if ((final_type = DeeType_Pointer(final_type)) == NULL) return NULL;
     if ((offset_constant = DeePointer_New(final_type,(void *)final_offset)) == NULL) return NULL;
     result = DeeXAst_NewConst(tk,offset_constant);
     Dee_DECREF(offset_constant);
     return result;
    }
   }
   break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP
  case DEE_XASTKIND_BUILTIN_HELP:
   if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0) {
    switch (ast_a->ast_kind) {
     case DEE_XASTKIND_CONST:
      // Doc of a constant object
      unary_result = DeeObject_Doc(ast_a->ast_const.c_const);
      break;
     case DEE_XASTKIND_ATTR_GET_C:
      // Doc of the attribute of a constant object
      if (ast_a->ast_attr_get_c.ac_object->ast_kind == DEE_XASTKIND_CONST) {
       unary_result = DeeObject_DocAttr(
        ast_a->ast_attr_get_c.ac_object->ast_const.c_const,
        (DeeObject *)ast_a->ast_attr_get_c.ac_name);
       break;
      }
      goto normal_ast;
     default: goto normal_ast;
    }
    goto handle_unary_result;
   }
   goto normal_ast;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_HELP */

  default: break;
 }

 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_MERGE_OPERATORS) != 0) {
  if (kind == DEE_XASTKIND_NOT) switch (ast_a->ast_kind) {
   // Optimization: !(x in y) --> x !in y
   // Optimization: !(x is y) --> x !is y
   case DEE_XASTKIND_IN: return DeeXAst_NewBinary(DEE_XASTKIND_NOT_IN,tk,lexer,parser_flags,ast_a->ast_operator.op_a,ast_a->ast_operator.op_b);
   case DEE_XASTKIND_IS: return DeeXAst_NewBinary(DEE_XASTKIND_NOT_IS,tk,lexer,parser_flags,ast_a->ast_operator.op_a,ast_a->ast_operator.op_b);
   case DEE_XASTKIND_NOT_IN: return DeeXAst_NewBinary(DEE_XASTKIND_IN,tk,lexer,parser_flags,ast_a->ast_operator.op_a,ast_a->ast_operator.op_b);
   case DEE_XASTKIND_NOT_IS: return DeeXAst_NewBinary(DEE_XASTKIND_IS,tk,lexer,parser_flags,ast_a->ast_operator.op_a,ast_a->ast_operator.op_b);
   default: break;
  }
  // Don't add casts when the requested type is already given
  switch (kind) {
   case DEE_XASTKIND_STR:  if (DeeXAst_PredictType(ast_a) == &DeeString_Type) {ret_ast_a: Dee_INCREF(ast_a); return ast_a; } break;
   case DEE_XASTKIND_BOOL: if (DeeXAst_PredictType(ast_a) == &DeeBool_Type) goto ret_ast_a; break;
   case DEE_XASTKIND_MOVE:
   case DEE_XASTKIND_COPY: {
    DeeTypeObject const *predicted_type;
    predicted_type = DeeXAst_PredictType(ast_a);
    // Don't copy/move an object that has copy/move implemented as no-op
    if (predicted_type && (DeeType_FLAGS(
     predicted_type)&DEE_TYPE_FLAG_DONT_COPY)!=0) goto ret_ast_a;
   } break;
   default: break;
  }
 }
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0 &&
     ast_a->ast_kind == DEE_XASTKIND_CONST) {
  // Fast-track for asts that we don't need to copy the constant for
  switch (kind) {
   case DEE_XASTKIND_SUPEROF:
    unary_result = DeeSuper_Of(ast_a->ast_const.c_const);
    goto handle_unary_result;
   case DEE_XASTKIND_CLASSOF:
    Dee_INCREF(unary_result = (DeeObject *)DeeObject_ClassOf(ast_a->ast_const.c_const));
    goto return_unary_result;
   case DEE_XASTKIND_TYPE:
    Dee_INCREF(unary_result = (DeeObject *)Dee_TYPE(ast_a->ast_const.c_const));
    goto return_unary_result;
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
   case DEE_XASTKIND_REF:
    unary_result = DeeObject_Ref(ast_a->ast_const.c_const);
    goto handle_unary_result;
   case DEE_XASTKIND_DEREF:
    // todo: Should we really allow this at compile-time?
    unary_result = DeeObject_Deref(ast_a->ast_const.c_const);
    goto handle_unary_result;
   case DEE_XASTKIND_PTROF:
    unary_result = (DeeObject *)DeeType_Pointer((DeeTypeObject *)ast_a->ast_const.c_const);
    if (!unary_result) goto handle_unary_error;
    Dee_INCREF(unary_result);
    goto return_unary_result;
   case DEE_XASTKIND_LVALOF:
    unary_result = (DeeObject *)DeeType_LValue((DeeTypeObject *)ast_a->ast_const.c_const);
    if (!unary_result) goto handle_unary_error;
    Dee_INCREF(unary_result);
    goto return_unary_result;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
#if DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
   case DEE_XASTKIND_VARRAYOF:
    unary_result = (DeeObject *)DeeType_VArray((DeeTypeObject *)ast_a->ast_const.c_const);
    if (!unary_result) goto handle_unary_error;
    Dee_INCREF(unary_result);
    goto return_unary_result;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ARRAYS */
   default: break;
  }
  if ((const_copy = DeeObject_DeepCopy(ast_a->ast_const.c_const)) == NULL) {
   // Ignore errors in copy, as the copy-operation wouldn't be performed at runtime
   DeeError_Handled();
   goto normal_ast;
  }
  switch (kind) {
   case DEE_XASTKIND_STR: unary_result = DeeObject_Str(const_copy); break;
   case DEE_XASTKIND_REPR: unary_result = DeeObject_Repr(const_copy); break;
   case DEE_XASTKIND_COPY: unary_result = DeeObject_Copy(const_copy); break;
   case DEE_XASTKIND_MOVE: unary_result = DeeObject_Move(const_copy); break;
   case DEE_XASTKIND_WEAK: unary_result = DeeWeakref_New(const_copy); break;
   case DEE_XASTKIND_BOOL: {
    int temp = DeeObject_Bool(const_copy);
    if (temp < 0) {
handle_unary_error_const_copy:
     Dee_DECREF(const_copy);
     goto handle_unary_error;
    }
    unary_result = DeeBool_New(temp);
   } break;
   case DEE_XASTKIND_NOT: unary_result = DeeObject_Not(const_copy); break;
   case DEE_XASTKIND_NEG: unary_result = DeeObject_Neg(const_copy); break;
   case DEE_XASTKIND_POS: unary_result = DeeObject_Pos(const_copy); break;
   case DEE_XASTKIND_INV: unary_result = DeeObject_Inv(const_copy); break;
   case DEE_XASTKIND_INC: unary_result = DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_INC,lexer,tk,Dee_TYPE(const_copy)) != 0 ? NULL : DeeObject_Inc(const_copy); break;
   case DEE_XASTKIND_DEC: unary_result = DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_DEC,lexer,tk,Dee_TYPE(const_copy)) != 0 ? NULL : DeeObject_Dec(const_copy); break;
   case DEE_XASTKIND_INCPOST: unary_result = DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_INCPOST,lexer,tk,Dee_TYPE(const_copy)) != 0 ? NULL : DeeObject_IncPost(const_copy); break;
   case DEE_XASTKIND_DECPOST: unary_result = DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_DECPOST,lexer,tk,Dee_TYPE(const_copy)) != 0 ? NULL : DeeObject_DecPost(const_copy); break;
   case DEE_XASTKIND_SEQ_SIZE: unary_result = DeeObject_SizeObject(const_copy); break;
   case DEE_XASTKIND_ITERSELF: unary_result = DeeObject_IterSelf(const_copy); break;
   case DEE_XASTKIND_SEQ_SUM: unary_result = DeeSequence_Sum(const_copy); break;
   case DEE_XASTKIND_ITERNEXT: {
    int error;
    if ((error = DeeObject_IterNextEx(const_copy,&unary_result)) < 0)
     goto handle_unary_error_const_copy;
    else if (error != 0) {
     DeeError_Throw(DeeErrorInstance_IterDone);
     unary_result = NULL;
    }
   } break;
   case DEE_XASTKIND_EXPAND: {
    int error; DeeObject *seq_iter,*seq_null; // Expand a single-element constant sequence
    if ((seq_iter = DeeObject_IterSelf(const_copy)) == NULL) goto handle_unary_error_const_copy;
    if ((error = DeeObject_IterNextEx(seq_iter,&unary_result)) != 0) {
     Dee_DECREF(seq_iter);
     if (error < 0) goto handle_unary_error_const_copy; // Error while iterating
     else goto normal_ast_const_copy; // Empty sequence
    }
    error = DeeObject_IterNextEx(seq_iter,&seq_null);
    Dee_DECREF(seq_iter);
    if (error == 0) {
     // sequence size isn't 1
     Dee_DECREF(seq_null);
     Dee_DECREF(unary_result);
     goto normal_ast_const_copy;
    }
    if (error < 0) {
     // Error after first element was found
     Dee_DECREF(unary_result);
     goto handle_unary_error_const_copy;
    }
    // Yes! Sequence has one element (now stored in 'unary_result')
   } break;
   {
    int temp;
   case DEE_XASTKIND_SEQ_ANY:
    if ((temp = DeeSequence_Any(const_copy)) < 0)
     unary_result = NULL;
    else unary_result = DeeBool_New(temp);
    break;
   case DEE_XASTKIND_SEQ_ALL:
    if ((temp = DeeSequence_All(const_copy)) < 0)
     unary_result = NULL;
    else unary_result = DeeBool_New(temp);
    break;
   }

   default:normal_ast_const_copy:
    Dee_DECREF(const_copy);
    goto normal_ast;
  }
  Dee_DECREF(const_copy);
handle_unary_result:
  if (!unary_result) {
   DeeTypeObject *rt_error;
handle_unary_error:
   if ((rt_error = _DeeError_HandleAndReturnType()) != NULL) {
    int temp = DeeXAst_ThrowInvalidUnaryOperatorError(kind,tk,lexer,ast_a,rt_error);
    Dee_DECREF(rt_error);
    if (temp != 0) return NULL;
   }
  } else {
return_unary_result:
   result = DeeXAst_NewConst(tk,unary_result);
   Dee_DECREF(unary_result);
   return result;
  }
 }
normal_ast:
 if ((result = _DeeXAst_NewUnsafe(kind,tk)) != NULL) {
  Dee_INCREF(result->ast_operator.op_a = ast_a);
 }
 return result;
}

DEE_DECL_END
