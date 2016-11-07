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
#include "xast_parse.c"
#endif

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeXAst_FixOperatorWithArgcount(
 DEE_A_IN DeeLexerObject *lexer, DEE_A_IN DeeTokenObject *error_token,
 DEE_A_INOUT int *operator_name, DEE_A_IN Dee_size_t argc) {
 Dee_size_t req_argc;
 DEE_ASSERT(operator_name);
 switch (*operator_name) {
  case DEE_XAST_TYPESLOT_NONE:
  case DeeType_SLOT_ID(tp_call):
   // var-arg operators
   return 0;
  case DeeType_SLOT_ID(tp_any_new):
  case DeeType_SLOT_ID(tp_any_ctor):
   // Without any arguments, this is a special operator
   *operator_name = argc
    ? DeeType_SLOT_ID(tp_any_ctor)
    : DeeType_SLOT_ID(tp_ctor);
   return 0;
  case DEE_XAST_TYPESLOT_SUPERARGS:
   *operator_name = argc
    ? DEE_CLASS_SLOTID_SUPERARGS_ANY_CTOR
    : DEE_CLASS_SLOTID_SUPERARGS_CTOR;
   return 0;
  case DeeType_SLOT_ID(tp_p_add):
   if (argc == 0) *operator_name = DeeType_SLOT_ID(tp_pos);
   else if (argc == 1) *operator_name = DeeType_SLOT_ID(tp_add);
   else {
    if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_ARGUMENT_COUNT_FOR_OPERATOR_CALL,
     (DeeObject *)lexer,(DeeObject *)error_token,
     "'operator + (...)' requires 0 arguments for 'operator __pos__' "
     "or 1 argument for 'operator __add__', but %Iu given were given",argc) != 0) return -1;
    return 1;
   }
   return 0;
  case DeeType_SLOT_ID(tp_p_sub):
   if (argc == 0) *operator_name = DeeType_SLOT_ID(tp_neg);
   else if (argc == 1) *operator_name = DeeType_SLOT_ID(tp_sub);
   else {
    if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_ARGUMENT_COUNT_FOR_OPERATOR_CALL,
     (DeeObject *)lexer,(DeeObject *)error_token,
     "'operator - (...)' requires 0 arguments for 'operator __neg__' "
     "or 1 argument for 'operator __sub__', but %Iu given were given",argc) != 0) return -1;
    return 1;
   }
   return 0;
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case DeeType_SLOT_ID(tp_p_mul):
   if (argc == 0) *operator_name = DEE_XAST_TYPESLOT_DEREF;
   else if (argc == 1) *operator_name = DeeType_SLOT_ID(tp_mul);
   else {
    if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_ARGUMENT_COUNT_FOR_OPERATOR_CALL,
     (DeeObject *)lexer,(DeeObject *)error_token,
     "'operator * (...)' requires 0 arguments for 'operator __deref__' "
     "or 1 argument for 'operator __mul__', but %Iu given were given",argc) != 0) return -1;
    return 1;
   }
   return 0;
  case DeeType_SLOT_ID(tp_p_and):
   if (argc == 0) *operator_name = DEE_XAST_TYPESLOT_REF;
   else if (argc == 1) *operator_name = DeeType_SLOT_ID(tp_and);
   else {
    if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_ARGUMENT_COUNT_FOR_OPERATOR_CALL,
     (DeeObject *)lexer,(DeeObject *)error_token,
     "'operator & (...)' requires 0 arguments for 'operator __ref__' "
     "or 1 argument for 'operator __and__', but %Iu given were given",argc) != 0) return -1;
    return 1;
   }
   return 0;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
  case DeeType_SLOT_ID(tp_p_seq_get):
   if (argc == 1) *operator_name = DeeType_SLOT_ID(tp_seq_get);
   else if (argc == 2) *operator_name = DeeType_SLOT_ID(tp_seq_range_get);
   else {
    if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_ARGUMENT_COUNT_FOR_OPERATOR_CALL,
     (DeeObject *)lexer,(DeeObject *)error_token,
     "'operator [] (...)' requires 1 arguments for 'operator __getitem__' "
     "or 2 argument for 'operator __getrange__', but %Iu given were given",argc) != 0) return -1;
    return 1;
   }
   return 0;
  case DeeType_SLOT_ID(tp_p_seq_del):
   if (argc == 1) *operator_name = DeeType_SLOT_ID(tp_seq_del);
   else if (argc == 2) *operator_name = DeeType_SLOT_ID(tp_seq_range_del);
   else {
    if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_ARGUMENT_COUNT_FOR_OPERATOR_CALL,
     (DeeObject *)lexer,(DeeObject *)error_token,
     "'operator del [] (...)' requires 1 arguments for 'operator __delitem__' "
     "or 2 argument for 'operator __delrange__', but %Iu given were given",argc) != 0) return -1;
    return 1;
   }
   return 0;
  case DeeType_SLOT_ID(tp_p_seq_set):
   if (argc == 2) *operator_name = DeeType_SLOT_ID(tp_seq_set);
   else if (argc == 3) *operator_name = DeeType_SLOT_ID(tp_seq_range_set);
   else {
    if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_ARGUMENT_COUNT_FOR_OPERATOR_CALL,
     (DeeObject *)lexer,(DeeObject *)error_token,
     "'operator [] = (...)' requires 2 arguments for 'operator __setitem__' "
     "or 3 argument for 'operator __setrange__', but %Iu given were given",argc) != 0) return -1;
    return 1;
   }
   return 0;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  case DEE_XAST_TYPESLOT_IO_READ:
   if (argc == 1) *operator_name = DEE_CLASS_SLOTID_OPERATOR_READNP;
   else if (argc == 2) *operator_name = DeeType_SLOT_ID(tp_io_read);
   else {
    if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_ARGUMENT_COUNT_FOR_OPERATOR_CALL,
     (DeeObject *)lexer,(DeeObject *)error_token,
     "'operator __read__(...)' requires 1 arguments for 'operator __readnp__' "
     "or 2 argument for 'operator __readp__', but %Iu given were given",argc) != 0) return -1;
    return 1;
   }
   return 0;
  case DEE_XAST_TYPESLOT_IO_WRITE:
   if (argc == 1) *operator_name = DEE_CLASS_SLOTID_OPERATOR_WRITENP;
   else if (argc == 2) *operator_name = DeeType_SLOT_ID(tp_io_write);
   else {
    if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_ARGUMENT_COUNT_FOR_OPERATOR_CALL,
     (DeeObject *)lexer,(DeeObject *)error_token,
     "'operator __write__(...)' requires 1 arguments for 'operator __writenp__' "
     "or 2 argument for 'operator __writep__', but %Iu given were given",argc) != 0) return -1;
    return 1;
   }
   return 0;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */

#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case DEE_XAST_TYPESLOT_REF: case DEE_XAST_TYPESLOT_DEREF:
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
  case DeeType_SLOT_ID(tp_int32): case DeeType_SLOT_ID(tp_int64):
  case DeeType_SLOT_ID(tp_double): case DeeType_SLOT_ID(tp_str):
  case DeeType_SLOT_ID(tp_repr): case DeeType_SLOT_ID(tp_not):
  case DeeType_SLOT_ID(tp_bool): case DeeType_SLOT_ID(tp_inv):
  case DeeType_SLOT_ID(tp_pos): case DeeType_SLOT_ID(tp_neg):
  case DeeType_SLOT_ID(tp_inc): case DeeType_SLOT_ID(tp_incpost):
  case DeeType_SLOT_ID(tp_dec): case DeeType_SLOT_ID(tp_decpost):
  case DeeType_SLOT_ID(tp_seq_size): case DeeType_SLOT_ID(tp_seq_iter_self):
  case DeeType_SLOT_ID(tp_seq_iter_next): case DeeType_SLOT_ID(tp_hash):
  case DeeType_SLOT_ID(tp_new): case DeeType_SLOT_ID(tp_ctor):
  case DeeType_SLOT_ID(tp_dtor): case DeeType_SLOT_ID(tp_clear):
  case DEE_CLASS_SLOTID_OPERATOR_FOR: req_argc = 0; break;

  case DEE_XAST_TYPESLOT_ATTR_HAS:
  case DeeType_SLOT_ID(tp_copy_new): case DeeType_SLOT_ID(tp_copy_ctor):
  case DeeType_SLOT_ID(tp_move_new): case DeeType_SLOT_ID(tp_move_ctor):
  case DeeType_SLOT_ID(tp_any_assign): case DeeType_SLOT_ID(tp_copy_assign):
  case DeeType_SLOT_ID(tp_move_assign): case DeeType_SLOT_ID(tp_add):
  case DeeType_SLOT_ID(tp_iadd): case DeeType_SLOT_ID(tp_sub):
  case DeeType_SLOT_ID(tp_isub): case DeeType_SLOT_ID(tp_mul):
  case DeeType_SLOT_ID(tp_imul): case DeeType_SLOT_ID(tp_div):
  case DeeType_SLOT_ID(tp_idiv): case DeeType_SLOT_ID(tp_mod):
  case DeeType_SLOT_ID(tp_imod): case DeeType_SLOT_ID(tp_shl):
  case DeeType_SLOT_ID(tp_ishl): case DeeType_SLOT_ID(tp_shr):
  case DeeType_SLOT_ID(tp_ishr): case DeeType_SLOT_ID(tp_and):
  case DeeType_SLOT_ID(tp_iand): case DeeType_SLOT_ID(tp_or):
  case DeeType_SLOT_ID(tp_ior): case DeeType_SLOT_ID(tp_xor):
  case DeeType_SLOT_ID(tp_ixor): case DeeType_SLOT_ID(tp_pow):
  case DeeType_SLOT_ID(tp_ipow): case DeeType_SLOT_ID(tp_cmp_lo):
  case DeeType_SLOT_ID(tp_cmp_le): case DeeType_SLOT_ID(tp_cmp_eq):
  case DeeType_SLOT_ID(tp_cmp_ne): case DeeType_SLOT_ID(tp_cmp_gr):
  case DeeType_SLOT_ID(tp_cmp_ge): case DeeType_SLOT_ID(tp_seq_get):
  case DeeType_SLOT_ID(tp_seq_del): case DeeType_SLOT_ID(tp_seq_contains):
  case DeeType_SLOT_ID(tp_attr_get): case DeeType_SLOT_ID(tp_attr_del):
#if !DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case DeeType_SLOT_ID(tp_p_mul): case DeeType_SLOT_ID(tp_p_and):
#endif /* !DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
  case DEE_CLASS_SLOTID_OPERATOR_READNP:
  case DEE_CLASS_SLOTID_OPERATOR_WRITENP:
   req_argc = 1; break;

  case DeeType_SLOT_ID(tp_seq_set): case DeeType_SLOT_ID(tp_seq_range_get):
  case DeeType_SLOT_ID(tp_seq_range_del): case DeeType_SLOT_ID(tp_attr_set):
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  case DeeType_SLOT_ID(tp_io_read):
  case DeeType_SLOT_ID(tp_io_write):
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   req_argc = 2; break;

  case DeeType_SLOT_ID(tp_seq_range_set): req_argc = 3; break;

  default: return 0; break; // shouldn't happen...
 }
 if (argc != req_argc) {
  if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_ARGUMENT_COUNT_FOR_OPERATOR_CALL,
   (DeeObject *)lexer,(DeeObject *)error_token,
   "'operator %s(...)' requires %Iu arguments, but %Iu were provided",
   _DeeType_ClassOperatorName(*operator_name),req_argc,argc) != 0) return -1;
  return 1; // Resolve at runtime...
 }
 return 0;
}


DEE_COMPILER_MSVC_WARNING_PUSH(4701 4702 4703)
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewOperatorCall(
 DEE_A_INOUT DeeTokenObject *operator_token, DEE_A_INOUT DeeXAstObject *ast_this,
 DEE_A_INOUT DeeXAstObject *ast_args, DEE_A_IN int typeslot,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags) {
 DeeXAstObject *ast_result,*ast_operator_function; int error;
 DeeXAstObject *ast_arg0,*ast_arg1,*ast_arg2;
 Dee_size_t argc; DeeXAstKind kind;
 if (typeslot == DeeType_SLOT_ID(tp_call)) {
  // Special handling for tp_call: this operator accepts a variadic amount of arguments
  return DeeXAst_NewBinary(DEE_XASTKIND_CALL,operator_token,lexer,
                           parser_flags,ast_this,ast_args);
 }
 if DEE_UNLIKELY((argc = DeeXAst_TupleSize(ast_args)) == (Dee_size_t)-1) {
  DeeObject *operator_function;
  DeeXAstObject *expanded_args[2],*merged_args;
deduce_runtime:
  // Cannot deduce argument count --> Must use intrinsic functions to determine at runtime
  if DEE_UNLIKELY((operator_function = DeeBuiltin_GetIntrinsicFunctionOfTypeSlot(typeslot)) == NULL) {
   // Can this even happen? (Yes, some technical operators get here)
   if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_NO_RUNTIME_SUPPORT_FOR_OPERATOR,
    (DeeObject *)lexer,(DeeObject *)operator_token,
    "No runtime support for 'operator %s'",
    _DeeType_ClassOperatorName(typeslot)) != 0) return NULL;
call_none:
   operator_function = Dee_None; // *shrugs*
  }
  if DEE_UNLIKELY((ast_operator_function = DeeXAst_NewConst(operator_token,operator_function)) == NULL) return NULL;
  expanded_args[1] = DeeXAst_NewExpand(ast_args->ast_common.ast_token,
                                       lexer,parser_flags,ast_args);
  if DEE_UNLIKELY(!expanded_args[1]) {err_ast_operator_function: ast_result = NULL; goto end_ast_operator_function; }
  expanded_args[0] = ast_this;
  merged_args = DeeXAst_NewTuple(ast_this->ast_common.ast_token,
                                 lexer,parser_flags,2,expanded_args);
  Dee_DECREF(expanded_args[1]);
  if DEE_UNLIKELY(!merged_args) goto err_ast_operator_function;
  ast_result = DeeXAst_NewBinary(DEE_XASTKIND_CALL,operator_token,lexer,
                                 parser_flags,ast_operator_function,merged_args);
  Dee_DECREF(merged_args);
end_ast_operator_function:
  Dee_DECREF(ast_operator_function);
  return ast_result;
 }
 // Optimize operator calls into real operator ASTs
 if DEE_UNLIKELY((error = DeeXAst_FixOperatorWithArgcount(lexer,operator_token,&typeslot,argc)) < 0) return NULL;
 if (error != 0) goto deduce_runtime;
 switch (typeslot) {
  case DEE_XAST_TYPESLOT_NONE: goto deduce_runtime;
  case DeeType_SLOT_ID(tp_int32):
  case DeeType_SLOT_ID(tp_int64):
  case DeeType_SLOT_ID(tp_double): kind = DEE_XASTKIND_NONE; break; // Special handling
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case DEE_XAST_TYPESLOT_REF: kind = DEE_XASTKIND_REF; break;
  case DEE_XAST_TYPESLOT_DEREF: kind = DEE_XASTKIND_DEREF; break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
  case DEE_XAST_TYPESLOT_ATTR_HAS: kind = DEE_XASTKIND_ATTR_HAS; break;
  case DeeType_SLOT_ID(tp_copy_new):
  case DeeType_SLOT_ID(tp_copy_ctor): kind = DEE_XASTKIND_COPY; break;
  case DeeType_SLOT_ID(tp_move_new):
  case DeeType_SLOT_ID(tp_move_ctor): kind = DEE_XASTKIND_MOVE; break;
  case DeeType_SLOT_ID(tp_any_assign):
  case DeeType_SLOT_ID(tp_copy_assign): kind = DEE_XASTKIND_COPY_ASSIGN; break;
  case DeeType_SLOT_ID(tp_move_assign): kind = DEE_XASTKIND_MOVE_ASSIGN; break;
  case DeeType_SLOT_ID(tp_str): kind = DEE_XASTKIND_STR; break;
  case DeeType_SLOT_ID(tp_repr): kind = DEE_XASTKIND_REPR; break;
  case DeeType_SLOT_ID(tp_not): kind = DEE_XASTKIND_NOT; break;
  case DeeType_SLOT_ID(tp_bool): kind = DEE_XASTKIND_BOOL; break;
  case DeeType_SLOT_ID(tp_inv): kind = DEE_XASTKIND_INV; break;
  case DeeType_SLOT_ID(tp_pos): kind = DEE_XASTKIND_POS; break;
  case DeeType_SLOT_ID(tp_neg): kind = DEE_XASTKIND_NEG; break;
  case DeeType_SLOT_ID(tp_inc): kind = DEE_XASTKIND_INC; break;
  case DeeType_SLOT_ID(tp_incpost): kind = DEE_XASTKIND_INCPOST; break;
  case DeeType_SLOT_ID(tp_dec): kind = DEE_XASTKIND_DEC; break;
  case DeeType_SLOT_ID(tp_decpost): kind = DEE_XASTKIND_DECPOST; break;
  case DeeType_SLOT_ID(tp_add): kind = DEE_XASTKIND_ADD; break;
  case DeeType_SLOT_ID(tp_iadd): kind = DEE_XASTKIND_ADD; break;
  case DeeType_SLOT_ID(tp_sub): kind = DEE_XASTKIND_SUB; break;
  case DeeType_SLOT_ID(tp_isub): kind = DEE_XASTKIND_SUB; break;
  case DeeType_SLOT_ID(tp_mul): kind = DEE_XASTKIND_MUL; break;
  case DeeType_SLOT_ID(tp_imul): kind = DEE_XASTKIND_MUL; break;
  case DeeType_SLOT_ID(tp_div): kind = DEE_XASTKIND_DIV; break;
  case DeeType_SLOT_ID(tp_idiv): kind = DEE_XASTKIND_DIV; break;
  case DeeType_SLOT_ID(tp_mod): kind = DEE_XASTKIND_MOD; break;
  case DeeType_SLOT_ID(tp_imod): kind = DEE_XASTKIND_MOD; break;
  case DeeType_SLOT_ID(tp_shl): kind = DEE_XASTKIND_SHL; break;
  case DeeType_SLOT_ID(tp_ishl): kind = DEE_XASTKIND_SHL; break;
  case DeeType_SLOT_ID(tp_shr): kind = DEE_XASTKIND_SHR; break;
  case DeeType_SLOT_ID(tp_ishr): kind = DEE_XASTKIND_SHR; break;
  case DeeType_SLOT_ID(tp_and): kind = DEE_XASTKIND_AND; break;
  case DeeType_SLOT_ID(tp_iand): kind = DEE_XASTKIND_AND; break;
  case DeeType_SLOT_ID(tp_or): kind = DEE_XASTKIND_OR; break;
  case DeeType_SLOT_ID(tp_ior): kind = DEE_XASTKIND_OR; break;
  case DeeType_SLOT_ID(tp_xor): kind = DEE_XASTKIND_XOR; break;
  case DeeType_SLOT_ID(tp_ixor): kind = DEE_XASTKIND_XOR; break;
  case DeeType_SLOT_ID(tp_pow): kind = DEE_XASTKIND_POW; break;
  case DeeType_SLOT_ID(tp_ipow): kind = DEE_XASTKIND_POW; break;
  case DeeType_SLOT_ID(tp_cmp_lo): kind = DEE_XASTKIND_COMPARE_LO; break;
  case DeeType_SLOT_ID(tp_cmp_le): kind = DEE_XASTKIND_COMPARE_LE; break;
  case DeeType_SLOT_ID(tp_cmp_eq): kind = DEE_XASTKIND_COMPARE_EQ; break;
  case DeeType_SLOT_ID(tp_cmp_ne): kind = DEE_XASTKIND_COMPARE_NE; break;
  case DeeType_SLOT_ID(tp_cmp_gr): kind = DEE_XASTKIND_COMPARE_GR; break;
  case DeeType_SLOT_ID(tp_cmp_ge): kind = DEE_XASTKIND_COMPARE_GE; break;
  case DeeType_SLOT_ID(tp_seq_get): kind = DEE_XASTKIND_SEQ_GET; break;
  case DeeType_SLOT_ID(tp_seq_del): kind = DEE_XASTKIND_SEQ_DEL; break;
  case DeeType_SLOT_ID(tp_seq_set): kind = DEE_XASTKIND_SEQ_SET; break;
  case DeeType_SLOT_ID(tp_seq_size): kind = DEE_XASTKIND_SEQ_SIZE; break;
  case DeeType_SLOT_ID(tp_seq_contains): kind = DEE_XASTKIND_IN; break; // Parameters will be swapped later
  case DeeType_SLOT_ID(tp_seq_range_get): kind = DEE_XASTKIND_SEQ_RANGE_GET; break;
  case DeeType_SLOT_ID(tp_seq_range_del): kind = DEE_XASTKIND_SEQ_RANGE_DEL; break;
  case DeeType_SLOT_ID(tp_seq_range_set): kind = DEE_XASTKIND_SEQ_RANGE_SET; break;
  case DeeType_SLOT_ID(tp_seq_iter_self): kind = DEE_XASTKIND_ITERSELF; break;
  case DeeType_SLOT_ID(tp_seq_iter_next): kind = DEE_XASTKIND_ITERNEXT; break;
  case DeeType_SLOT_ID(tp_attr_set): kind = DEE_XASTKIND_ATTR_SET; break;
  case DeeType_SLOT_ID(tp_attr_get): kind = DEE_XASTKIND_ATTR_GET; break;
  case DeeType_SLOT_ID(tp_attr_del): kind = DEE_XASTKIND_ATTR_DEL; break;
//case DeeType_SLOT_ID(tp_hash): goto deduce_runtime; // TODO: Compile-optimization for this
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  case DeeType_SLOT_ID(tp_io_read): kind = DEE_XASTKIND_IO_READP; break;
  case DeeType_SLOT_ID(tp_io_write): kind = DEE_XASTKIND_IO_WRITEP; break;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  case DEE_CLASS_SLOTID_OPERATOR_READNP: kind = DEE_XASTKIND_IO_READNP; break;
  case DEE_CLASS_SLOTID_OPERATOR_WRITENP: kind = DEE_XASTKIND_IO_WRITENP; break;
  case DeeType_SLOT_ID(tp_io_seek): kind = DEE_XASTKIND_IO_SEEK; break;
  case DeeType_SLOT_ID(tp_io_flush): kind = DEE_XASTKIND_IO_FLUSH; break;
  case DeeType_SLOT_ID(tp_io_trunc): kind = DEE_XASTKIND_IO_TRUNC; break;
  case DeeType_SLOT_ID(tp_io_close): kind = DEE_XASTKIND_IO_CLOSE; break;
  case DeeType_SLOT_ID(tp_new):
  case DeeType_SLOT_ID(tp_ctor):
  case DeeType_SLOT_ID(tp_any_new):
  case DeeType_SLOT_ID(tp_any_ctor):
  case DeeType_SLOT_ID(tp_dtor):
  case DeeType_SLOT_ID(tp_clear):
  case DEE_CLASS_SLOTID_OPERATOR_FOR: // TODO: Add support for 'operator for'
   if (DeeError_CompilerErrorf(DEE_WARNING_CANNOT_CALL_OPERATOR_EXPLICITLY,
    (DeeObject *)lexer,(DeeObject *)operator_token,
    "'operator %s(...)' cannot be called explicitly",
    _DeeType_ClassOperatorName(typeslot)) != 0) return NULL;
   goto call_none;
  default: goto deduce_runtime; // shouldn't happen...
 }
 if (ast_args->ast_kind == DEE_XASTKIND_CONST) {
  DEE_ASSERT(DeeObject_Check(ast_args->ast_const.c_const) &&
             DeeTuple_Check(ast_args->ast_const.c_const));
  switch (argc) {
   case 3: if ((ast_arg2 = DeeXAst_NewConst(ast_args->ast_common.ast_token,DeeTuple_GET(ast_args->ast_const.c_const,2))) == NULL) return NULL; DEE_ATTRIBUTE_FALLTHROUGH
   case 2: if ((ast_arg1 = DeeXAst_NewConst(ast_args->ast_common.ast_token,DeeTuple_GET(ast_args->ast_const.c_const,1))) == NULL) {err_arg2: if (argc >= 3) Dee_DECREF(ast_arg2); return NULL; } DEE_ATTRIBUTE_FALLTHROUGH
   case 1: if ((ast_arg0 = DeeXAst_NewConst(ast_args->ast_common.ast_token,DeeTuple_GET(ast_args->ast_const.c_const,0))) == NULL) {/*err_arg1:*/ if (argc >= 2) Dee_DECREF(ast_arg1); goto err_arg2; } DEE_ATTRIBUTE_FALLTHROUGH
   default: break;
  }
 } else {
  DEE_ASSERT(ast_args->ast_kind == DEE_XASTKIND_TUPLE);
  switch (argc) {
   case 3: ast_arg2 = ast_args->ast_tuple.s_elemv[2]; DEE_ATTRIBUTE_FALLTHROUGH
   case 2: ast_arg1 = ast_args->ast_tuple.s_elemv[1]; DEE_ATTRIBUTE_FALLTHROUGH
   case 1: ast_arg0 = ast_args->ast_tuple.s_elemv[0]; DEE_ATTRIBUTE_FALLTHROUGH
   default: break;
  }
 }
 switch (kind) {
  case DEE_XASTKIND_IN: {
   // Swapped parameter order (was written as 'operator __contains__')
   DEE_ASSERT(argc == 1);
   ast_result = DeeXAst_NewBinary(kind,operator_token,lexer,parser_flags,ast_arg0,ast_this);
  } break;
  case DEE_XASTKIND_NONE: {
   DeeObject *special_function;
   DeeXAstObject *ast_special_function,*ast_function_args;
   switch (typeslot) {
    // Special handling for integral conversion operators
    case DeeType_SLOT_ID(tp_int32):  DEE_ASSERT(argc == 0); special_function = (DeeObject *)DeeObject_TYPE(Dee_rt_int32_t); break;
    case DeeType_SLOT_ID(tp_int64):  DEE_ASSERT(argc == 0); special_function = (DeeObject *)DeeObject_TYPE(Dee_rt_int64_t); break;
    case DeeType_SLOT_ID(tp_double): DEE_ASSERT(argc == 0); special_function = (DeeObject *)DeeObject_TYPE(double); break;
    default: DEE_BUILTIN_UNREACHABLE();
   }
   // create: 'special_function(ast_this)'
   if ((ast_special_function = DeeXAst_NewConst(operator_token,special_function)) != NULL) {
    ast_function_args = DeeXAst_NewTuple(operator_token,lexer,parser_flags,1,&ast_this);
    if (ast_function_args) {
     ast_result = DeeXAst_NewBinary(DEE_XASTKIND_CALL,operator_token,lexer,parser_flags,
                                    ast_special_function,ast_function_args);
     Dee_DECREF(ast_function_args);
    } else ast_result = NULL;
    Dee_DECREF(ast_special_function);
   } else ast_result = NULL;
  } break;
  case DEE_XASTKIND_SEQ_RANGE_GET: DEE_ASSERT(argc == 2); ast_result = DeeXAst_NewSeqRangeGet(operator_token,lexer,parser_flags,ast_this,ast_arg0,ast_arg1); break;
  case DEE_XASTKIND_SEQ_RANGE_DEL: DEE_ASSERT(argc == 2); ast_result = DeeXAst_NewSeqRangeDel(operator_token,lexer,parser_flags,ast_this,ast_arg0,ast_arg1); break;
  case DEE_XASTKIND_SEQ_RANGE_SET: DEE_ASSERT(argc == 3); ast_result = DeeXAst_NewSeqRangeSet(operator_token,lexer,parser_flags,ast_this,ast_arg0,ast_arg1,ast_arg2); break;
  default:
   DEE_ASSERT(/*argc >= 0 &&*/ argc <= 2);
   DEE_ASSERT(DEE_XASTKIND_ISOPERATOR(kind));
DEE_COMPILER_PREFAST_WARNING_PUSH(6001)
   switch (argc) {
    case 0: ast_result = DeeXAst_NewUnary(kind,operator_token,lexer,parser_flags,ast_this); break;
    case 1: ast_result = DeeXAst_NewBinary(kind,operator_token,lexer,parser_flags,ast_this,ast_arg0); break;
    case 2: ast_result = DeeXAst_NewTrinary(kind,operator_token,lexer,parser_flags,ast_this,ast_arg0,ast_arg1); break;
    default: DEE_BUILTIN_UNREACHABLE();
   }
DEE_COMPILER_PREFAST_WARNING_POP
   break;
 }
 if (ast_args->ast_kind == DEE_XASTKIND_CONST) switch (argc) {
  case 3: Dee_DECREF(ast_arg2); DEE_ATTRIBUTE_FALLTHROUGH
  case 2: Dee_DECREF(ast_arg1); DEE_ATTRIBUTE_FALLTHROUGH
  case 1: Dee_DECREF(ast_arg0); DEE_ATTRIBUTE_FALLTHROUGH
  default: break;
 }
 return ast_result;
}
DEE_COMPILER_MSVC_WARNING_POP


DEE_A_RET_EXCEPT(-1) int DeeXAstOperatorName_Parse(DEE_PARSER_PARAMS) {
 int result;
 switch (token.tk_id) {
  DEE_PARSE_TOKENID_CASE_CONST
  DEE_PARSE_TOKENID_CASE_VOLATILE
  case KWD_long: case KWD_char: case KWD_short: case KWD_int:
  DEE_PARSE_TOKENID_CASE_WCHAR_T DEE_PARSE_TOKENID_CASE_CHAR16_T
  DEE_PARSE_TOKENID_CASE_CHAR32_T DEE_PARSE_TOKENID_CASE_ATOMIC
  DEE_PARSE_TOKENID_CASE_SIGNED DEE_PARSE_TOKENID_CASE_UNSIGNED
  DEE_PARSE_TOKENID_CASE_UINT8 DEE_PARSE_TOKENID_CASE_UINT16
  DEE_PARSE_TOKENID_CASE_UINT32 DEE_PARSE_TOKENID_CASE_UINT64
  DEE_PARSE_TOKENID_CASE_INT8 DEE_PARSE_TOKENID_CASE_INT16
  DEE_PARSE_TOKENID_CASE_INT32 DEE_PARSE_TOKENID_CASE_INT64
  case KWD_float: case KWD_double: case KWD_string: {
   int result;
   DeeXAstObject *ast_conversion_type;
   DeeTypeObject *conversion_type;
   if ((ast_conversion_type = DeeXAst_ParseType(
    DEE_XAST_VARDECL_MODE_DEFAULT,NULL,DEE_PARSER_ARGS)) == NULL) return -1;
   if (ast_conversion_type->ast_kind != DEE_XASTKIND_CONST) {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT,
     (DeeObject *)lexer,(DeeObject *)ast_conversion_type->ast_common.ast_token,
     "Expected constant after 'operator' (got: %r)",ast_conversion_type) != 0) return -1;
   }
   conversion_type = (DeeTypeObject *)ast_conversion_type->ast_const.c_const;
   if (!DeeType_Check(conversion_type)) {
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_TYPE_AFTER_OPERATOR,
     (DeeObject *)lexer,(DeeObject *)ast_conversion_type->ast_common.ast_token,
     "Expected type after 'operator', but got instance of %q",
     DeeType_NAME(Dee_TYPE(conversion_type))) != 0) {
err_ast_conversion_type:
     Dee_DECREF(ast_conversion_type);
     return -1;
    }
    conversion_type = Dee_TYPE(conversion_type);
   }
   if (conversion_type == &DeeString_Type) result = DeeType_SLOT_ID(tp_str);
   else if (conversion_type == DeeObject_TYPE(Dee_int32_t)) result = DeeType_SLOT_ID(tp_int32);
   else if (conversion_type == DeeObject_TYPE(Dee_int64_t)) result = DeeType_SLOT_ID(tp_int64);
   else if (conversion_type == DeeObject_TYPE(double)) result = DeeType_SLOT_ID(tp_double);
   else if (conversion_type == DeeObject_TYPE(Dee_uint32_t)) {
    result = DeeType_SLOT_ID(tp_int32);
cant_use_operator:
    if (DeeError_CompilerErrorf(DEE_WARNING_CANT_USE_CONVERSION_OPERATOR,
     (DeeObject *)lexer,(DeeObject *)ast_conversion_type->ast_common.ast_token,
     "'operator %q' will be parsed as 'operator %q'",
     DeeType_NAME(conversion_type),_DeeType_ClassOperatorName(result)
     ) != 0) goto err_ast_conversion_type;
   } else if (conversion_type == DeeObject_TYPE(Dee_uint64_t)) {
    result = DeeType_SLOT_ID(tp_int64);
    goto cant_use_operator;
   } else if (conversion_type == DeeObject_TYPE(atomic(Dee_int32_t)) ||
              conversion_type == DeeObject_TYPE(atomic(Dee_uint32_t))) {
    result = DeeType_SLOT_ID(tp_int32);
    goto cant_use_operator;
   } else if (conversion_type == DeeObject_TYPE(atomic(Dee_int64_t)) ||
              conversion_type == DeeObject_TYPE(atomic(Dee_uint64_t))) {
    result = DeeType_SLOT_ID(tp_int64);
    goto cant_use_operator;
   } else if (conversion_type == DeeObject_TYPE(Dee_rt_float) ||
              conversion_type == DeeObject_TYPE(Dee_rt_ldouble)) {
    result = DeeType_SLOT_ID(tp_double);
    goto cant_use_operator;
   } else if (conversion_type == DeeObject_TYPE(Dee_int8_t) ||
              conversion_type == DeeObject_TYPE(Dee_uint8_t) ||
              conversion_type == DeeObject_TYPE(atomic(Dee_int8_t)) ||
              conversion_type == DeeObject_TYPE(atomic(Dee_uint8_t)) ||
              conversion_type == DeeObject_TYPE(Dee_int16_t) ||
              conversion_type == DeeObject_TYPE(Dee_uint16_t) ||
              conversion_type == DeeObject_TYPE(atomic(Dee_int16_t)) ||
              conversion_type == DeeObject_TYPE(atomic(Dee_uint16_t))) {
    result = DeeType_SLOT_ID(tp_int32);
    goto cant_use_operator;
   } else {
    // Shouldn't happen (technically...)
    result = DeeType_SLOT_ID(tp_int32);
    goto cant_use_operator;
   }
   Dee_DECREF(ast_conversion_type);
   return result;
  } break;

  case TPP_TOK_STR: {
   DeeObject *parsed_string;
   DeeTokenObject *str_token;
   Dee_INCREF(str_token = token_ob);
   if ((parsed_string = DeeLexer_ParseString((DeeObject *)lexer)) == NULL) {
err_str_token: Dee_DECREF(str_token); return -1;
   }
   if (!DeeString_Check(parsed_string) || DeeString_SIZE(parsed_string) != 0) {
    // >> operator "illegal" (); // Warning
    // >> operator "" ();        // OK
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_EMPTY_NORMAL_STRING_FOR_OPERATOR_STR,
     (DeeObject *)lexer,(DeeObject *)str_token,
     "Expected empty, regular string for 'operator __str__', but got %R",
     parsed_string) != 0) goto err_str_token;
   } else Dee_DECREF(parsed_string);
   Dee_DECREF(str_token);
   return DeeType_SLOT_ID(tp_str);
  } break;

  case KWD_str: case KWD___str__:
   result = DeeType_SLOT_ID(tp_str);
y1: if DEE_UNLIKELY(!yield()) return -1;
   return result;
#define YIELD_AND_RETURN(x) do{result=(x);goto y1;}while(0)
  case KWD_none: YIELD_AND_RETURN(DEE_XAST_TYPESLOT_NONE);
  case KWD_repr: case KWD___repr__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_repr));
  case KWD___copy__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_copy_ctor));
  case KWD_copy: if DEE_UNLIKELY(!yield()) return -1;
   if (token.tk_id == '=' || token.tk_id == TPP_TOK_COLLON_ASSIGN) YIELD_AND_RETURN(DeeType_SLOT_ID(tp_copy_assign));
   return DeeType_SLOT_ID(tp_copy_ctor);
  case KWD___move__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_move_ctor));
  case KWD_move: if DEE_UNLIKELY(!yield()) return -1;
   if (token.tk_id == '=' || token.tk_id == TPP_TOK_COLLON_ASSIGN) YIELD_AND_RETURN(DeeType_SLOT_ID(tp_move_assign));
   return DeeType_SLOT_ID(tp_move_ctor);
  case KWD___assign__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_any_assign));
  case '=': case TPP_TOK_COLLON_ASSIGN:
   if DEE_UNLIKELY(!yield()) return -1;
   if (token.tk_id == KWD_copy) YIELD_AND_RETURN(DeeType_SLOT_ID(tp_copy_assign));
   if (token.tk_id == KWD_move) YIELD_AND_RETURN(DeeType_SLOT_ID(tp_move_assign));
   return DeeType_SLOT_ID(tp_any_assign);
  case '?': {
   if DEE_UNLIKELY(!yield()) return -1;
   if (token.tk_id != '.') {
    if (DeeError_CompilerError(DEE_WARNING_DOT_AFTER_QUESTION_IN_OPERATOR_NAME,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected '.' after '?' in operator name to form 'operator __hasattr__'") != 0) return -1;
   } else if DEE_UNLIKELY(!yield()) return -1;
   return DEE_XAST_TYPESLOT_ATTR_HAS;
  } break;

  case KWD___copyassign__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_copy_assign));
  case KWD___moveassign__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_move_assign));
  case KWD___constructor__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_p_any_ctor));
  case KWD___destructor__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_dtor));
  case KWD___superargs__: YIELD_AND_RETURN(DEE_XAST_TYPESLOT_SUPERARGS);
  DEE_PARSE_TOKENID_CASE_BOOL case KWD___bool__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_bool));
  case '+': YIELD_AND_RETURN(DeeType_SLOT_ID(tp_p_add));
  case '-': YIELD_AND_RETURN(DeeType_SLOT_ID(tp_p_sub));
  case KWD___add__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_add));
  case KWD___sub__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_sub));
  case '!': case KWD___not__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_not));
  case '~': case KWD___inv__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_inv));
  case KWD___pos__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_pos));
  case KWD___neg__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_neg));
  case TPP_TOK_INC: case KWD___inc__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_inc));
  case TPP_TOK_DEC: case KWD___dec__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_dec));
  case KWD___incpost__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_incpost));
  case KWD___decpost__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_decpost));
  case TPP_TOK_CMP_LO: case KWD___lo__: case KWD___lt__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_cmp_lo));
  case TPP_TOK_CMP_LE: case KWD___le__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_cmp_le));
  case TPP_TOK_CMP_EQ: case KWD___eq__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_cmp_eq));
  case TPP_TOK_CMP_NE: case KWD___ne__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_cmp_ne));
  case TPP_TOK_CMP_GR: case KWD___gr__: case KWD___gt__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_cmp_gr));
  case TPP_TOK_CMP_GE: case KWD___ge__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_cmp_ge));
  case '*': YIELD_AND_RETURN(DeeType_SLOT_ID(tp_p_mul));
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case KWD___deref__: YIELD_AND_RETURN(DEE_XAST_TYPESLOT_DEREF);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
  case KWD___mul__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_mul));
  case '/': case KWD___div__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_div));
  case '%': case KWD___mod__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_mod));
  case TPP_TOK_SHL: case KWD___shl__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_shl));
  case TPP_TOK_SHR: case KWD___shr__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_shr));
  case '&': YIELD_AND_RETURN(DeeType_SLOT_ID(tp_p_and));
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case KWD___ref__: YIELD_AND_RETURN(DEE_XAST_TYPESLOT_REF);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
  case KWD___and__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_and));
  case '|': case KWD___or__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_or));
  case '^': case KWD___xor__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_xor));
  case TPP_TOK_POW: case KWD___pow__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_pow));
  case TPP_TOK_ADD_ASSIGN:     case KWD___iadd__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_iadd));
  case TPP_TOK_SUB_ASSIGN:     case KWD___isub__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_isub));
  case TPP_TOK_MUL_ASSIGN:     case KWD___imul__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_imul));
  case TPP_TOK_DIV_ASSIGN:     case KWD___idiv__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_idiv));
  case TPP_TOK_MOD_ASSIGN:     case KWD___imod__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_imod));
  case TPP_TOK_SHL_ASSIGN:     case KWD___ishl__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_ishl));
  case TPP_TOK_SHR_ASSIGN:     case KWD___ishr__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_ishr));
  case TPP_TOK_BIN_AND_ASSIGN: case KWD___iand__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_iand));
  case TPP_TOK_BIN_OR_ASSIGN:  case KWD___ior__:  YIELD_AND_RETURN(DeeType_SLOT_ID(tp_ior));
  case TPP_TOK_BIN_XOR_ASSIGN: case KWD___ixor__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_ixor));
  case TPP_TOK_POW_ASSIGN:     case KWD___ipow__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_ipow));
  case KWD___call__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_call));
  case '#': case KWD___size__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_seq_size));
  case KWD___getitem__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_seq_get));
  case KWD___delitem__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_seq_del));
  case KWD___setitem__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_seq_set));
  case KWD___getrange__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_seq_range_get));
  case KWD___delrange__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_seq_range_del));
  case KWD___setrange__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_seq_range_set));
  /*case KWD_in: NO!!! - nonsensical parameter order*/
  case KWD___contains__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_seq_contains));
  case KWD___getattr__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_attr_get));
  case KWD___hasattr__: YIELD_AND_RETURN(DEE_XAST_TYPESLOT_ATTR_HAS);
  case KWD___delattr__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_attr_del));
  case KWD___setattr__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_attr_set));
  case KWD_for: YIELD_AND_RETURN(DEE_CLASS_SLOTID_OPERATOR_FOR);
  case KWD___iterself__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_seq_iter_self));
  case KWD___iternext__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_seq_iter_next));
  case KWD___hash__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_hash));
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  case KWD___read__: YIELD_AND_RETURN(DEE_XAST_TYPESLOT_IO_READ);
  case KWD___write__: YIELD_AND_RETURN(DEE_XAST_TYPESLOT_IO_WRITE);
  case KWD___readp__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_io_read));
  case KWD___writep__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_io_write));
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  case KWD___read__: YIELD_AND_RETURN(DEE_CLASS_SLOTID_OPERATOR_READNP);
  case KWD___write__: YIELD_AND_RETURN(DEE_CLASS_SLOTID_OPERATOR_WRITENP);
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  case KWD___readnp__: YIELD_AND_RETURN(DEE_CLASS_SLOTID_OPERATOR_READNP);
  case KWD___writenp__: YIELD_AND_RETURN(DEE_CLASS_SLOTID_OPERATOR_WRITENP);
  case KWD___seek__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_io_seek));
  case KWD___flush__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_io_flush));
  case KWD___trunc__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_io_trunc));
  case KWD___close__: YIELD_AND_RETURN(DeeType_SLOT_ID(tp_io_close));

  case '(': {
   int result;
   if DEE_UNLIKELY(!yield()) return -1;
   if (token.tk_id == ')') {
    if DEE_UNLIKELY(!yield()) return -1;
    // call operator
    return DeeType_SLOT_ID(tp_call);
   }
   // Operator name parenthesis
   if ((result = DeeXAstOperatorName_Parse(DEE_PARSER_ARGS)) < 0) return -1;
   if (token.tk_id != ')') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_LPAREN_IN_OPERATOR_NAME,(DeeObject *)lexer,(DeeObject *)token_ob,
     "Expected ')' to close parenthesis after '(' <operator_name> in operator name") != 0) return -1;
   } else if DEE_UNLIKELY(!yield()) return -1;
   return result;
  }
  case KWD_del:
   if DEE_UNLIKELY(!yield()) return -1;
   if (token.tk_id == '.') YIELD_AND_RETURN(DeeType_SLOT_ID(tp_attr_del));
   if (token.tk_id == '[') {
    if DEE_UNLIKELY(!yield()) return -1;
    if (token.tk_id == ':') {
     if DEE_UNLIKELY(!yield()) return -1;
     if (token.tk_id != ']') {
      if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_DEL_LBRACKET_COLLON_IN_OPERATOR_NAME,
       (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ']' after 'operator del[:'") != 0
       ) return -1;
     } else if DEE_UNLIKELY(!yield()) return -1;
     return DeeType_SLOT_ID(tp_seq_range_del);
    } else if (token.tk_id != ']') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_DEL_LBRACKET_IN_OPERATOR_NAME,
      (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ']' or ':' after 'operator del['") != 0
      ) return -1;
    } else if DEE_UNLIKELY(!yield()) return -1;
    return DeeType_SLOT_ID(tp_p_seq_del);
   }
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LBRACKET_OR_DOT_AFTER_DEL_IN_OPERATOR_NAME,
    (DeeObject *)lexer,(DeeObject *)token_ob,"Expected '[' or '.' after 'operator del'") != 0
    ) return -1;
   return DeeType_SLOT_ID(tp_attr_del); // ~shrug's~
  case '.':
   if DEE_UNLIKELY(!yield()) return -1;
   if (token.tk_id == '=') YIELD_AND_RETURN(DeeType_SLOT_ID(tp_attr_set));
   return DeeType_SLOT_ID(tp_attr_get);
  case '[':
   if DEE_UNLIKELY(!yield()) return -1;
   if (token.tk_id == ':') {
    if DEE_UNLIKELY(!yield()) return -1;
    if (token.tk_id != ']') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_LBRACKET_COLLON_IN_OPERATOR_NAME,
      (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ']' after 'operator[:'") != 0
      ) return -1;
    } else if DEE_UNLIKELY(!yield()) return -1;
    if (token.tk_id == '=') YIELD_AND_RETURN(DeeType_SLOT_ID(tp_seq_range_set));
    return DeeType_SLOT_ID(tp_seq_range_get);
   } else if (token.tk_id != ']') {
    if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_LBRACKET_IN_OPERATOR_NAME,
     (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ']' or ':' after 'operator['") != 0) return -1;
   } else if DEE_UNLIKELY(!yield()) return -1;
   if (token.tk_id == '=') YIELD_AND_RETURN(DeeType_SLOT_ID(tp_p_seq_set));
   return DeeType_SLOT_ID(tp_p_seq_get);
#undef YIELD_AND_RETURN
#ifdef DEE_PRIVATE_AST_RESERVED_OPERATOR_TOKENS
  DEE_PRIVATE_AST_RESERVED_OPERATOR_TOKENS
   if (DeeError_CompilerError(DEE_WARNING_USING_RESERVED_KEYWORD,
    (DeeObject *)lexer,(DeeObject *)token_ob,"Using reserved operator name"
    ) != 0) return -1;
   DEE_ATTRIBUTE_FALLTHROUGH
#endif
  default:
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_OPERATOR_NAME_IN_CLASSDECL,
    (DeeObject *)lexer,(DeeObject *)token_ob,"Expected operator name after 'operator'") != 0
    ) return -1;
   break;
 }
 return 0;
}

DEE_DECL_END
