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


#if DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int)
_DeeXAstTypeOperations_IncBuf(DEE_A_INOUT struct DeeXAstTypeOperations *self) {
 struct DeeXAstTypeOperationsEntry *new_entryv;
 DEE_ASSERT(self);
 if (!self->o_opsa) self->o_opsa = 2; else self->o_opsa *= 2;
 while ((new_entryv = (struct DeeXAstTypeOperationsEntry *)realloc_nz(
  self->o_opsv,self->o_opsa*sizeof(struct DeeXAstTypeOperationsEntry))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 self->o_opsv = new_entryv;
 return 0;
}

// Appends an uninitialized entry and returns it
DEE_A_RET_EXCEPT(NULL) struct DeeXAstTypeOperationsEntry *
DeeXAstTypeOperations_AppendNew(DEE_A_INOUT struct DeeXAstTypeOperations *self) {
 DEE_ASSERT(self);
 if (self->o_opsc == self->o_opsa && _DeeXAstTypeOperations_IncBuf(self) != 0) return NULL;
 return &self->o_opsv[self->o_opsc++];
}

DEE_A_RET_EXCEPT(NULL) struct DeeXAstTypeOperationsEntry *DeeXAstTypeOperations_InsertNew(
 DEE_A_INOUT struct DeeXAstTypeOperations *self, DEE_A_IN Dee_size_t pos) {
 DEE_ASSERT(self);
 DEE_ASSERT(pos <= self->o_opsc);
 if (self->o_opsc == self->o_opsa && _DeeXAstTypeOperations_IncBuf(self) != 0) return NULL;
 memmove(self->o_opsv+(pos+1),self->o_opsv+pos,(
  self->o_opsc-pos)*sizeof(struct DeeXAstTypeOperationsEntry));
 ++self->o_opsc;
 return &self->o_opsv[pos];
}
void DeeXAstTypeOperations_InsertFree(
 DEE_A_INOUT struct DeeXAstTypeOperations *self, DEE_A_IN Dee_size_t pos) {
 DEE_ASSERT(self);
 DEE_ASSERT(pos < self->o_opsc);
 memmove(self->o_opsv+pos,self->o_opsv+(pos+1),(
  self->o_opsc-(pos+1))*sizeof(struct DeeXAstTypeOperationsEntry));
 --self->o_opsc;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAstTypeOperations_Exec(
 DEE_A_IN struct DeeXAstTypeOperations const *self, DEE_A_INOUT DeeXAstObject *base,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags) {
 DeeXAstObject *result,*new_result;
 struct DeeXAstTypeOperationsEntry *iter,*begin;
 Dee_INCREF(result = base);
 iter = (begin = self->o_opsv)+self->o_opsc;
 while (iter != begin) {
  --iter;
  switch (iter->oe_kind) {
   default: Dee_INCREF(new_result = result); break;
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
   case DEE_XASTTYPEOPERATION_KIND_LVALUE:
    new_result = DeeXAst_NewUnary(DEE_XASTKIND_LVALOF,iter->oe_token,lexer,parser_flags,result);
    break;
   case DEE_XASTTYPEOPERATION_KIND_POINTER:
    new_result = DeeXAst_NewUnary(DEE_XASTKIND_PTROF,iter->oe_token,lexer,parser_flags,result);
    break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
#if DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
   case DEE_XASTTYPEOPERATION_KIND_ARRAY:
    new_result = DeeXAst_NewBinary(DEE_XASTKIND_SEQ_GET,iter->oe_token,lexer,
                                   parser_flags,result,iter->oe_array.a_size);
    break;
   case DEE_XASTTYPEOPERATION_KIND_VARARRAY:
    new_result = DeeXAst_NewUnary(DEE_XASTKIND_VARRAYOF,iter->oe_token,lexer,parser_flags,result);
    break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ARRAYS */
#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
    // Foreign function
   case DEE_XASTTYPEOPERATION_KIND_FOREIGN_FUNCTION: {
    DeeXAstObject **argv,**argv_iter,**argv_end;
    struct DeeXAstTypeOperationsForeignFunctionArgument *arg;
    if (iter->oe_foreign_function.ff_argc) {
     while ((argv = (DeeXAstObject **)malloc_nz(
      iter->oe_foreign_function.ff_argc*sizeof(DeeXAstObject *))) == NULL) {
      if DEE_LIKELY(Dee_CollectMemory()) continue;
      Dee_CLEAR(result);
      DeeError_NoMemory();
      goto end;
     }
     argv_end = (argv_iter = argv)+iter->oe_foreign_function.ff_argc;
     arg = iter->oe_foreign_function.ff_argv;
     while (argv_iter != argv_end) Dee_INCREF(*argv_iter++ = (arg++)->ffa_type);
    } else argv = NULL;
    new_result = DeeXAst_NewForeignFunctionFromInheritedArgv(iter->oe_token,lexer,parser_flags,
                                                             result,iter->oe_foreign_function.ff_flags,
                                                             iter->oe_foreign_function.ff_argc,argv);
    if (!new_result) {
     argv_end = (argv_iter = argv)+iter->oe_foreign_function.ff_argc;
     while (argv_iter != argv_end) Dee_DECREF(*argv_iter++);
     free(argv);
    }
   } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */
   
  }
  Dee_DECREF(result);
  Dee_INHERIT_REF(result,new_result);
  if (!result) goto end;
 }
end: return result;
}
#endif /* ... */




DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_DoParseNamedType(
 DEE_A_REF DEE_A_OUT DeeTokenObject **name_token,
 DEE_A_OUT_OPT Dee_uint32_t *vardecl_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 if (DeeAttributeDecl_Parse(&attr,DEE_PARSER_ARGS) == 0)
  result = DeeXAst_DoParseNamedTypeWithAttribute(name_token,vardecl_mode,&attr,DEE_PARSER_ARGS);
 else result = NULL;
 _DeeAttributeDecl_Quit(&attr);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_DoParseNamedTypeWithAttribute(
 DEE_A_REF DEE_A_OUT DeeTokenObject **name_token, DEE_A_OUT_OPT Dee_uint32_t *vardecl_mode,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeXAstObject *base,*result;
 if ((base = _DeeXAst_ParseInitOrDecl(
  DEE_XAST_VARDECL_MODE_DEFAULT,
  DEE_XAST_INITORDECL_FLAGS_SINGLEVAL|
  DEE_XAST_INITORDECL_FLAGS_FORCECTYPES|
  DEE_XAST_INITORDECL_FLAGS_NOCTYPESSUFFIX,
  DEE_XASTKIND_TUPLE,attr,NULL,DEE_PARSER_ARGS)
  ) == NULL) return NULL;
 result = DeeXAst_DoParseNamedTypeSuffixWithAttribute(base,name_token,vardecl_mode,attr,DEE_PARSER_ARGS);
 Dee_DECREF(base);
 return result;
}


DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_DoParseNamedTypeSuffix(
 DEE_A_IN DeeXAstObject *base, DEE_A_REF DEE_A_OUT DeeTokenObject **name_token,
 DEE_A_OUT_OPT Dee_uint32_t *vardecl_mode, DEE_PARSER_PARAMS) {
 DeeXAstObject *result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 if (DeeAttributeDecl_Parse(&attr,DEE_PARSER_ARGS) == 0)
  result = DeeXAst_DoParseNamedTypeSuffixWithAttribute(base,name_token,vardecl_mode,&attr,DEE_PARSER_ARGS);
 else result = NULL;
 _DeeAttributeDecl_Quit(&attr);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_DoParseNamedTypeSuffixWithAttributeCopy(
 DEE_A_IN DeeXAstObject *base, DEE_A_REF DEE_A_OUT DeeTokenObject **name_token,
 DEE_A_OUT_OPT Dee_uint32_t *vardecl_mode, DEE_A_INOUT struct DeeAttributeDecl const *attr, DEE_PARSER_PARAMS) {
 DeeXAstObject *result;
 struct DeeAttributeDecl used_attr;
 _DeeAttributeDecl_InitCopy(&used_attr,attr);
 if (DeeAttributeDecl_Parse(&used_attr,DEE_PARSER_ARGS) == 0)
  result = DeeXAst_DoParseNamedTypeSuffixWithAttribute(base,name_token,vardecl_mode,&used_attr,DEE_PARSER_ARGS);
 else result = NULL;
 _DeeAttributeDecl_Quit(&used_attr);
 return result;
}


DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_DoParseNamedTypeSuffixWithAttribute(
 DEE_A_IN DeeXAstObject *base, DEE_A_REF DEE_A_OUT DeeTokenObject **name_token,
 DEE_A_OUT_OPT Dee_uint32_t *vardecl_mode, DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
#if DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS
 DeeXAstObject *result;
 struct DeeXAstTypeOperations post_ops = DeeXAstTypeOperations_INIT();
 if (_DeeXAst_InternalDoParseNamedTypeSuffix(name_token,vardecl_mode,attr,
  &post_ops,DEE_PARSER_ARGS) != 0) result = NULL; else {
  result = DeeXAstTypeOperations_Exec(&post_ops,base,lexer,parser_flags);
  if (!result) Dee_XDECREF(*name_token);
 }
 _DeeXAstTypeOperations_Quit(&post_ops);
 return result;
#else /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
 if (_DeeXAst_InternalDoParseNamedTypeSuffix(
  name_token,vardecl_mode,attr,DEE_PARSER_ARGS) != 0) return NULL;
 Dee_INCREF(base);
 return base;
#endif /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
}


DEE_A_RET_EXCEPT(-1) int _DeeXAst_InternalDoParseNamedTypeSuffix(
 DEE_A_REF DEE_A_OUT DeeTokenObject **name_token, DEE_A_OUT_OPT Dee_uint32_t *vardecl_mode,
 DEE_A_INOUT struct DeeAttributeDecl *attr,
#if DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS
 DEE_A_INOUT struct DeeXAstTypeOperations *post_operations,
#endif /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
 DEE_PARSER_PARAMS) {
#if DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS
 struct DeeXAstTypeOperationsEntry *entry;
 Dee_size_t my_pos;
#endif /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
 DEE_ASSERT(name_token);
 DEE_ASSERT(attr);
#if DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS
 DEE_ASSERT(post_operations);
 my_pos = DeeXAstTypeOperations_SIZE(post_operations);
#endif /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
 while (1) switch (token.tk_id) {

#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case TPP_TOK_POW: // Pointer type
   if ((entry = DeeXAstTypeOperations_InsertNew(post_operations,my_pos)) == NULL) return -1;
   Dee_INCREF(entry->oe_token = token_ob);
   entry->oe_kind = DEE_XASTTYPEOPERATION_KIND_POINTER;
   DEE_ATTRIBUTE_FALLTHROUGH
  case '*':
   if ((entry = DeeXAstTypeOperations_InsertNew(post_operations,my_pos)) == NULL) return -1;
   Dee_INCREF(entry->oe_token = token_ob);
   entry->oe_kind = DEE_XASTTYPEOPERATION_KIND_POINTER;
y1_prefix:
  DEE_PARSE_TOKENID_CASE_CONST
  DEE_PARSE_TOKENID_CASE_VOLATILE
   if DEE_UNLIKELY(!yield()) return -1;
   if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) return -1;
   break;

  case '&': // LValue type
   if ((entry = DeeXAstTypeOperations_InsertNew(post_operations,my_pos)) == NULL) return -1;
   Dee_INCREF(entry->oe_token = token_ob);
   entry->oe_kind = DEE_XASTTYPEOPERATION_KIND_LVALUE;
   goto y1_prefix;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */

  default: goto end_prefix;
 }
end_prefix: // End of prefix
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
 if (token.tk_id == '(') {
#if DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS
  Dee_size_t old_size;
#endif /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
  // Recursion
  if DEE_UNLIKELY(!yield()) return -1;
  if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) return -1;
#if DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS
  old_size = DeeXAstTypeOperations_SIZE(post_operations);
  if (_DeeXAst_InternalDoParseNamedTypeSuffix(
   name_token,vardecl_mode,attr,post_operations,DEE_PARSER_ARGS) != 0) return -1;
#else /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
  if (_DeeXAst_InternalDoParseNamedTypeSuffix(
   name_token,vardecl_mode,attr,DEE_PARSER_ARGS) != 0) return -1;
#endif /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
#if DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS
  my_pos += (DeeXAstTypeOperations_SIZE(post_operations)-old_size);
#endif /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
  if (token.tk_id != ')') {
   if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_RECURSIVE_TYPE,
    (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ')' after recursive type") != 0) goto err_name_token;
  } else if DEE_UNLIKELY(!yield()) {
err_name_token: Dee_XDECREF(*name_token); return -1;
  }
 } else if (TPPTokenID_IS_KEYWORD(token.tk_id)) {
  if (vardecl_mode) {
   // Parse some variable lookup modifiers if requested by the caller
   *vardecl_mode = DEE_XAST_VARDECL_MODE_DEFAULT;
   if (DeeXAst_ParseVariableLookupModifiers(vardecl_mode,attr,DEE_PARSER_ARGS) != 0) return -1;
  }
  switch (token.tk_id) {
    // Warn about reserved identifiers tokens in expression
#if defined(DEE_XAST_UNARY_TOKENS_NOSTMT) || \
    defined(DEE_XAST_RESERVED_KEYWORD_TOKENS) || \
    defined(DEE_SAST_STATEMENT_ONLY_TOKENS)
   if (0) {
#ifdef DEE_XAST_UNARY_TOKENS_NOSTMT
    DEE_XAST_UNARY_TOKENS_NOSTMT
#endif
#ifdef DEE_XAST_RESERVED_KEYWORD_TOKENS
    DEE_XAST_RESERVED_KEYWORD_TOKENS
#endif
#ifdef DEE_SAST_STATEMENT_ONLY_TOKENS
    DEE_SAST_STATEMENT_ONLY_TOKENS
#endif
    if (DeeError_CompilerError(DEE_WARNING_RESERVED_KEYWORD_IN_TYPENAME,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Reserved identifier used for typename") != 0) return -1;
   }
#endif /* ... */
   default:
    if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_KEYWORD_FOR_TYPED_VARIABLE_NAME,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Expected keyword for variable name in typed variable declaration") != 0) return -1;
     name_token = NULL;
     goto named_suffix;
    }
    break;
  }
  // This is the type name
  Dee_INCREF(*name_token = token_ob);
  if DEE_UNLIKELY(!yield()) goto err_name_token;
 } else {
  *name_token = NULL;
  if (vardecl_mode) *vardecl_mode = DEE_XAST_VARDECL_MODE_DEFAULT;
 }
named_suffix:
#if DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS
 while (1) {
  if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) return -1;
  switch (token.tk_id) {
#if DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
   case '[': {
    DeeXAstObject *array_size;
    DeeTokenObject *array_token;
    Dee_INCREF(array_token = token_ob);
    if DEE_UNLIKELY(!yield()) {
err_array_token_name_token: Dee_DECREF(array_token); goto err_name_token;
    }
    if (token.tk_id == ']') array_size = NULL;
    else if ((array_size = DeeXAst_ParseSingle(DEE_PARSER_ARGS)) == NULL) goto err_array_token_name_token;
    if (token.tk_id != ']') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RBRACKET_AFTER_LBRACKET_IN_RECURSIVE_TYPE,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "expected ']' to close array descriptor in recursive type") != 0) goto err_array_size;
    } else if DEE_UNLIKELY(!yield()) {
err_array_size: Dee_XDECREF(array_size); goto err_array_token_name_token;
    }
    if ((entry = DeeXAstTypeOperations_InsertNew(post_operations,my_pos)) == NULL) goto err_array_size;
    Dee_INHERIT_REF(entry->oe_token,array_token);
    if (array_size) {
     entry->oe_kind = DEE_XASTTYPEOPERATION_KIND_ARRAY;
     Dee_INHERIT_REF(entry->oe_array.a_size,array_size);
    } else entry->oe_kind = DEE_XASTTYPEOPERATION_KIND_VARARRAY;
   } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ARRAYS */

#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
   case '(': {
    // Foreign function type suffix
    DeeTokenObject *ff_token;
    Dee_INCREF(ff_token = token_ob);
    if DEE_UNLIKELY(!yield()) {
err_ff_token_name_token: Dee_DECREF(ff_token); goto err_name_token;
    }
    if ((entry = DeeXAstTypeOperations_InsertNew(
     post_operations,my_pos)) == NULL) goto err_ff_token_name_token;
    Dee_INHERIT_REF(entry->oe_token,ff_token);
    entry->oe_kind = DEE_XASTTYPEOPERATION_KIND_FOREIGN_FUNCTION;
    if (token.tk_id == ')') {
ff_noargs:
     entry->oe_foreign_function.ff_argc = 0;
     entry->oe_foreign_function.ff_argv = NULL;
    } else if (token.tk_id == KWD_none) {
     DeeTokenObject *none_token; int temp;
     Dee_INCREF(none_token = token_ob);
     if DEE_UNLIKELY(!yield()) { Dee_DECREF(none_token); goto err_ff_entry_token; }
     // Convert 'int(none)' --> 'int()' (Note, that 'none' has the meaning of 'void')
     if (token.tk_id == ')') {
      Dee_DECREF(none_token);
      goto ff_noargs;
     }
     temp = DeeLexer_Return((DeeObject *)lexer,(DeeObject *)none_token);
     Dee_DECREF(none_token);
     if (temp != 0) goto err_ff_entry_token;
     goto ff_default_args;
    } else {
ff_default_args:
     if (_DeeXAst_InternalParseForeignFunctionArguments(
      &entry->oe_foreign_function,DEE_PARSER_ARGS) != 0) {
err_ff_entry_token: Dee_DECREF(entry->oe_token);
err_ff_entry: DeeXAstTypeOperations_InsertFree(post_operations,my_pos);
      return -1;
     }
     // Warn about incorrect use of 'none' vs. an alias for it
     if (entry->oe_foreign_function.ff_argc == 1 &&
         entry->oe_foreign_function.ff_argv[0].ffa_type->ast_kind == DEE_XASTKIND_CONST) {
      DeeObject *single_arg_const = entry->oe_foreign_function.ff_argv[0].ffa_type->ast_const.c_const;
      if (DeeNone_Check(single_arg_const) || single_arg_const == (DeeObject *)&DeeNone_Type) {
       if (DeeError_CompilerError(DEE_WARNING_VOID_PARAMETER_LIST_MUST_BE_EMPTY_OR_NONE,
        (DeeObject *)lexer,(DeeObject *)entry->oe_foreign_function.ff_argv[0].ffa_type->ast_common.ast_token,"\n"
        "\t'__DEEMON__ >= 101' requires that an empty parameter list contains nothing or the keyword 'none'\n"
        "\tUsing a compile-time alias for 'none', such as here, is no longer supported"
        ) != 0) goto err_ff_entry_args;
      }
     }
    }
    if (token.tk_id != ')') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_RPAREN_AFTER_FOREIGN_ARGUMENT_LIST,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "Expected ')' to end argument list of foreign function type"
      ) != 0) goto err_ff_entry_args;
    } else if DEE_UNLIKELY(!yield()) goto err_ff_entry_args;
    // Parse attributes one more time after the parameter list
    if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) {
err_ff_entry_args:
     _DeeXAstTypeOperationsForeignFunctionEntry_Quit(&entry->oe_foreign_function);
     goto err_ff_entry;
    }
    // Finally apply the function flags from the attribute
    entry->oe_foreign_function.ff_flags = attr->a_fun_flags;
   } break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */

   default: goto end_suffix;
  }
  ++my_pos;
 }
end_suffix:
#else /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
 if (DeeAttributeDecl_Parse(attr,DEE_PARSER_ARGS) != 0) return -1;
#endif /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
 return 0;
}

#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
DEE_A_RET_EXCEPT(-1) int _DeeXAst_InternalParseForeignFunctionArgument(
 DEE_A_INOUT struct DeeXAstTypeOperationsForeignFunctionArgument *arg, DEE_PARSER_PARAMS) {
 int result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 if (DeeAttributeDecl_Parse(&attr,DEE_PARSER_ARGS) == 0) {
  if ((arg->ffa_type = DeeXAst_DoParseNamedTypeWithAttribute(
   &arg->ffa_name,NULL,&attr,DEE_PARSER_ARGS)) == NULL) result = -1; else {
   arg->ffa_varflags = DEE_LOCALVAR_KIND_PARAM;
   if ((attr.a_flags&DEE_ATTRIBUTE_DECL_FLAG_UNUSED)!=0) arg->ffa_varflags |= DEE_LOCALVAR_FLAG_UNUSED;
   if ((attr.a_flags&DEE_ATTRIBUTE_DECL_FLAG_USED)!=0) arg->ffa_varflags |= DEE_LOCALVAR_FLAG_USED;
   result = 0;
  }
 } else result = -1;
 _DeeAttributeDecl_Quit(&attr);
 return result;
}
DEE_A_RET_EXCEPT(-1) int _DeeXAst_InternalParseForeignFunctionArguments(
 DEE_A_INOUT struct DeeXAstTypeOperationsForeignFunctionEntry *ff, DEE_PARSER_PARAMS) {
 struct DeeXAstTypeOperationsForeignFunctionArgument *new_argv,*argv = NULL;
 Dee_size_t argc = 0,arga = 2;
 while ((argv = (struct DeeXAstTypeOperationsForeignFunctionArgument *)malloc_nz(
  2*sizeof(struct DeeXAstTypeOperationsForeignFunctionArgument))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 while (1) {
  if (argc == arga) {
   DEE_ASSERT(arga);
   arga *= 2;
   while ((new_argv = (struct DeeXAstTypeOperationsForeignFunctionArgument *)realloc_nnz(
    argv,arga*sizeof(struct DeeXAstTypeOperationsForeignFunctionArgument))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
err_argv:
    new_argv = argv+argc;
    while (new_argv != argv) { --new_argv; _DeeXAstTypeOperationsForeignFunctionArgument_Quit(new_argv); }
    free(argv);
    return -1;
   }
   argv = new_argv;
  }
  if (token.tk_id == TPP_TOK_DOTS) {
   // Varargs (terminates the argument list)
   ff->ff_flags |= DEE_FUNCTIONFLAGS_FLAG_VARARGS;
   if DEE_UNLIKELY(!yield()) goto err_argv;
   break;
  }
  if (_DeeXAst_InternalParseForeignFunctionArgument(
   &argv[argc],DEE_PARSER_ARGS) != 0) goto err_argv;
  ++argc;
  if (token.tk_id != ',') break;
  if DEE_UNLIKELY(!yield()) goto err_argv;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
  if (token.tk_id == ')') break;
#endif
 }
 DEE_ASSERT(argc != 0);
 if (argc != arga && (new_argv = (struct DeeXAstTypeOperationsForeignFunctionArgument *)realloc_nnz(
  argv,argc*sizeof(struct DeeXAstTypeOperationsForeignFunctionArgument))) != NULL) argv = new_argv;
 ff->ff_argc = argc;
 ff->ff_argv = argv; // inherit data
 return 0;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */


DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseNamedTypeSuffix(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN DeeXAstObject *base, DEE_PARSER_PARAMS) {
 DeeXAstObject *result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 if (DeeAttributeDecl_Parse(&attr,DEE_PARSER_ARGS) == 0)
  result = (vardecl_mode&DEE_XAST_VARDECL_FLAG_CONST)!=0
   ? DeeXAst_ParseConstantNamedTypeSuffixWithAttribute(vardecl_mode,flags,base,&attr,
      DEE_PARSER_ARGS_EX(parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))
   : DeeXAst_ParseNamedTypeSuffixWithAttribute(vardecl_mode,flags,base,&attr,DEE_PARSER_ARGS);
 else result = NULL;
 _DeeAttributeDecl_Quit(&attr);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseNamedTypeSuffixWithCopyAttribute(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN DeeXAstObject *base, DEE_A_INOUT struct DeeAttributeDecl const *attr, DEE_PARSER_PARAMS) {
 DeeXAstObject *result;
 struct DeeAttributeDecl used_attr;
 _DeeAttributeDecl_InitCopy(&used_attr,attr);
 if (DeeAttributeDecl_Parse(&used_attr,DEE_PARSER_ARGS) == 0)
  result = (vardecl_mode&DEE_XAST_VARDECL_FLAG_CONST)!=0
   ? DeeXAst_ParseConstantNamedTypeSuffixWithAttribute(vardecl_mode,flags,base,&used_attr,
      DEE_PARSER_ARGS_EX(parser_flags|DEE_PARSER_FLAG_GENERATE_CONSTANT))
   : DeeXAst_ParseNamedTypeSuffixWithAttribute(vardecl_mode,flags,base,&used_attr,DEE_PARSER_ARGS);
 else result = NULL;
 _DeeAttributeDecl_Quit(&used_attr);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseAllNamedTypeSuffix(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN DeeXAstObject *base, DEE_A_IN DeeXAstKind seq_kind, DEE_PARSER_PARAMS) {
 DeeXAstObject *result;
 struct DeeAttributeDecl attr = DeeAttributeDecl_INIT();
 if (DeeAttributeDecl_Parse(&attr,DEE_PARSER_ARGS) == 0)
  result = DeeXAst_ParseAllNamedTypeSuffixWithAttribute(vardecl_mode,flags,base,seq_kind,&attr,DEE_PARSER_ARGS);
 else result = NULL;
 _DeeAttributeDecl_Quit(&attr);
 return result;
}

DEE_COMPILER_MSVC_WARNING_PUSH(4701 4703)
extern DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseAllNamedTypeSuffixWithAttribute(
 DEE_A_IN Dee_uint32_t vardecl_mode, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN DeeXAstObject *base, DEE_A_IN DeeXAstKind seq_kind,
 DEE_A_INOUT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeXAstObject *single_ast,*ast_result;
 DeeTokenObject *ast_token;
 // NOTE: Remember to force create a sequence if 'DEE_XAST_INITORDECL_FLAGS_RETURNSEQ' is set in 'flags'
 // NOTE: The 'DEE_XAST_INITORDECL_FLAGS_RETURNSEQ' will never be set when calling from '_DeeXAst_ParseInitOrDecl'
 if ((single_ast = DeeXAst_ParseNamedTypeSuffixWithCopyAttribute(
  vardecl_mode,(flags/*&~(DEE_XAST_INITORDECL_FLAGS_REQSEMICOLLON)*/)|DEE_XAST_INITORDECL_FLAGS_NOSEMIBUTCOMMA,
  base,attr,DEE_PARSER_ARGS)) == NULL) return NULL;
 if (token.tk_id != ',') {
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
single_element:
#endif
  if ((flags&DEE_XAST_INITORDECL_FLAGS_RETURNSEQ) == 0) return single_ast;
  ast_result = DeeXAst_NewSequence(seq_kind,single_ast->ast_common.ast_token,
                                   lexer,parser_flags,1,&single_ast);
  Dee_DECREF(single_ast);
  return ast_result;
 }
 { // Parse a sequence
  Dee_size_t      asta;
  Dee_size_t      astc;
  DeeXAstObject **astv,**new_astv;
  asta = 0,astc = 0,astv = NULL;
  goto parse_seq;
parse_seq_next:
  if ((single_ast = DeeXAst_ParseNamedTypeSuffixWithCopyAttribute(
   vardecl_mode,flags|DEE_XAST_INITORDECL_FLAGS_NOSEMIBUTCOMMA,
   base,attr,DEE_PARSER_ARGS)) == NULL) goto err_ast_sequence;
parse_seq:
  if (astc == asta) {
   if (!asta) asta = 2; else asta *= 2;
   while ((new_astv = (DeeXAstObject **)realloc_nz(
    astv,asta*sizeof(DeeXAstObject *))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
    goto err_ast_sequence;
   }
   astv = new_astv;
  }
  Dee_INHERIT_REF(astv[astc++],single_ast);

  if (token.tk_id != ',') goto parse_seq_end;
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
  // Ignore the padding ',' if we don't find what we were looking for
  if ((flags&DEE_XAST_INITORDECL_FLAGS_NOPOSTCOMMA)!=0)
   Dee_INCREF(ast_token = token_ob);
#endif
  if DEE_UNLIKELY(!yield()) {
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
err_ast_sequence_ast_token:
   if ((flags&DEE_XAST_INITORDECL_FLAGS_NOPOSTCOMMA)!=0)
    Dee_DECREF(ast_token);
#endif
err_ast_sequence:
   new_astv = astv+astc;
   while (new_astv != astv) Dee_DECREF(*--new_astv);
   free(astv);
   return NULL;
  }
#if DEE_CONFIG_LANGUAGE_HAVE_RELAXED_COMMA_LIST
  switch (token.tk_id) {
   case '(':
#if DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS
   case '*': case TPP_TOK_POW: case '&':
#endif /* DEE_PRIVATE_XAST_HAVE_TYPE_OPERATIONS */
   DEE_ATTRIBUTEDECL_TOKENS
    break;
   default:
    if (!TPPTokenID_IS_KEYWORD(token.tk_id)) {
     if ((flags&(DEE_XAST_INITORDECL_FLAGS_NOPOSTCOMMA|
      DEE_XAST_INITORDECL_FLAGS_REQSEMICOLLON)
      )==DEE_XAST_INITORDECL_FLAGS_REQSEMICOLLON) {
      if (token.tk_id != ';') {
       DEE_ASSERT(astc);
       if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_SEMICOLLON_AFTER_EXPRESSION,
        (DeeObject *)lexer,(DeeObject *)token_ob,"Expected ';' after expression '...,%r'",
        astv[astc-1]) != 0) goto err_ast_sequence_ast_token;
      } else {
       if DEE_UNLIKELY(!yield()) goto err_ast_sequence_ast_token;
       goto after_semicollon_seq_end;
      }
     }
   case ';':
     // Ignore the previous ',' (still stored in 'ast_token')
     if ((flags&DEE_XAST_INITORDECL_FLAGS_NOPOSTCOMMA)!=0) {
      if (DeeLexer_Return((DeeObject *)lexer,(DeeObject *)ast_token) != 0
          ) goto err_ast_sequence_ast_token;
      Dee_DECREF(ast_token);
     }
after_semicollon_seq_end:
     if (astc == 1 && (flags&DEE_XAST_INITORDECL_FLAGS_RETURNSEQ) == 0) {
      Dee_INHERIT_REF(single_ast,astv[0]);
      free_nn(astv);
      goto single_element;
     }
     goto parse_seq_end;
    }
    break;
  }
  if ((flags&DEE_XAST_INITORDECL_FLAGS_NOPOSTCOMMA)!=0)
   Dee_DECREF(ast_token);
#endif
  goto parse_seq_next;
parse_seq_end:
  // Try to safe some memory
  if (astc != astc && (new_astv = (DeeXAstObject **)realloc_nz(
   astv,astc*sizeof(DeeXAstObject *))) != NULL) astv = new_astv;
  ast_result = DeeXAst_NewSequenceFromInheritedAstv(seq_kind,base->ast_common.ast_token,
                                                    lexer,parser_flags,astc,astv);
  if (!ast_result) goto err_ast_sequence;
  return ast_result;
 }
}
DEE_COMPILER_MSVC_WARNING_POP

DEE_DECL_END

#ifndef __INTELLISENSE__
#define DO_CONST
#include "xast_parse.named_type_suffix_impl.inl"
#include "xast_parse.named_type_suffix_impl.inl"
#endif

