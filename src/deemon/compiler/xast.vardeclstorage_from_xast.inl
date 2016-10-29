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

DEE_A_RET_EXCEPT(-1) int DeeVarDeclStorage_InitFromVar(
 DEE_A_OUT struct DeeVarDeclStorage *self, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeLocalVarObject *dest_var,
 DEE_A_INOUT DeeTokenObject *dest_var_token, DEE_A_INOUT DeeScopeObject *curr_scope) {
 DeeXAstObject *dest_var_ast;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(dest_var) && DeeLocalVar_Check(dest_var));
 DEE_ASSERT(DeeObject_Check(dest_var_token) && DeeToken_Check(dest_var_token));
 (void)parser_flags;
 if (DeeScope_SameWeakScope((DeeObject *)curr_scope,(DeeObject *)dest_var->lv_scope)) {
  switch (DeeLocalVar_KIND(dest_var)) {
   case DEE_LOCALVAR_KIND_THIS:
    if (DeeError_CompilerError(DEE_WARNING_CANT_STORE_THIS,
     (DeeObject *)lexer,(DeeObject *)dest_var_token,
     "Can't store a value in a this-variable") != 0) return -1;
store_asssign:
    if ((dest_var_ast = DeeXAst_NewLocalVar(dest_var_token,dest_var,curr_scope)) == NULL) return -1;
    goto store_default_with_dest_var_ast;
   case DEE_LOCALVAR_KIND_STATIC:
    if (DeeLocalVar_IS_INIT(dest_var)) {
     // Warn about secondary static initializers
     if (DeeError_CompilerError(DEE_WARNING_STATIC_VARIABLE_ALREADY_INITIALIZED,
      (DeeObject *)lexer,(DeeObject *)dest_var_token,
      "The static variable was already initialized. "
      "Note that secondary initializer will only be executed once.") != 0) return -1;
    }
    break;
   case DEE_LOCALVAR_KIND_STACK:
    if (!dest_var->lv_init) {
     // First initialization
     if (dest_var->lv_scope != curr_scope) {
      if (DeeError_CompilerError(DEE_WARNING_STACK_VARIABLE_FIRST_INIT_NOT_DECL_SCOPE,
       (DeeObject *)lexer,(DeeObject *)dest_var_token,
       "The first initialization of a stack variable must happen inside its declaration scope") != 0) return -1;
      goto store_asssign;
     } else if (DeeScope_IS_STRONG(curr_scope)) {
      if (DeeError_CompilerError(DEE_WARNING_STACK_VARIABLE_IN_ROOT_SCOPE,
       (DeeObject *)lexer,(DeeObject *)dest_var_token,
       "Declaration of stack variables inside the root/strong scope is not recommended") != 0) return -1;
     }
    }
    break;
   default: break;
  }
  self->vds_kind = DEE_VARDECLSTORAGEKIND_VAR;
  Dee_INCREF(self->vds_var.vs_token = dest_var_token);
  Dee_INCREF(self->vds_var.vs_var = dest_var);
  ++dest_var->lv_init;
 } else {
  if (DeeError_CompilerErrorf(DEE_WARNING_CANT_STORE_VAR_FROM_DIFFERENT_SCOPE,
   (DeeObject *)lexer,(DeeObject *)dest_var_token,
   "Store to variable from different strong scope will be compiled as assign:\n"
   "\t%>K := ...",_DeeLocalVar_ReprWithToken(dest_var,dest_var_token)) != 0) return -1;
  if ((dest_var_ast = DeeXAst_NewReferenceVar(dest_var_token,dest_var,curr_scope)) == NULL) return -1;
store_default_with_dest_var_ast:
  self->vds_kind = DEE_VARDECLSTORAGEKIND_ASSIGN;
  Dee_INHERIT_REF(self->vds_assign.as_ast,dest_var_ast);
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeVarDeclStorage_InitFromXAst(
 DEE_A_OUT struct DeeVarDeclStorage *self, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *ast_dest,
 DEE_A_INOUT DeeScopeObject *curr_scope) {
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(ast_dest) && DeeXAst_Check(ast_dest));
 DEE_ASSERT(DeeObject_Check(curr_scope) && DeeScope_Check(curr_scope));
 switch (ast_dest->ast_kind) {
  case DEE_XASTKIND_VAR:
   return DeeVarDeclStorage_InitFromVar(self,lexer,parser_flags,ast_dest->ast_var.vs_var,
                                        ast_dest->ast_common.ast_token,curr_scope);
  case DEE_XASTKIND_SEQ_RANGE_GET:
   self->vds_kind = DEE_VARDECLSTORAGEKIND_RANGE;
   Dee_INCREF(self->vds_range.rs_object = ast_dest->ast_seq_range_get.sr_seq);
   Dee_XINCREF(self->vds_range.rs_begin = ast_dest->ast_seq_range_get.sr_begin);
   Dee_XINCREF(self->vds_range.rs_end = ast_dest->ast_seq_range_get.sr_end);
   break;
  case DEE_XASTKIND_SEQ_GET:
   self->vds_kind = DEE_VARDECLSTORAGEKIND_ITEM;
   Dee_INCREF(self->vds_item.is_object = ast_dest->ast_operator.op_a);
   Dee_INCREF(self->vds_item.is_key = ast_dest->ast_operator.op_b);
   break;
  case DEE_XASTKIND_ATTR_GET:
   self->vds_kind = DEE_VARDECLSTORAGEKIND_ATTR;
   Dee_INCREF(self->vds_attr.as_object = ast_dest->ast_operator.op_a);
   Dee_INCREF(self->vds_attr.as_attr = ast_dest->ast_operator.op_b);
   break;
  case DEE_XASTKIND_ATTR_GET_C:
   self->vds_kind = DEE_VARDECLSTORAGEKIND_ATTR_C;
   Dee_INCREF(self->vds_attr_c.acs_object = ast_dest->ast_attr_get_c.ac_object);
   Dee_INCREF(self->vds_attr_c.acs_attr = ast_dest->ast_attr_get_c.ac_name);
   break;

  case DEE_XASTKIND_TUPLE: {
   // Create expanded storage
   struct DeeVarDeclStorage *declv,*iter,*end;
   DeeXAstObject **src;
   DEE_ASSERT(ast_dest->ast_tuple.s_elemc);
   while ((declv = (struct DeeVarDeclStorage *)malloc_nz(
    ast_dest->ast_tuple.s_elemc*sizeof(struct DeeVarDeclStorage))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
    return -1;
   }
   self->vds_kind = DEE_VARDECLSTORAGEKIND_EXPAND;
   self->vds_expand.es_declc = ast_dest->ast_tuple.s_elemc;
   self->vds_expand.es_declv = declv;
   src = ast_dest->ast_tuple.s_elemv;
   end = (iter = declv)+self->vds_expand.es_declc;
   do if (DeeVarDeclStorage_InitFromXAst(iter,lexer,parser_flags,*src++,curr_scope) != 0) {
    while (iter != declv) { --iter; DeeVarDeclStorage_Quit(iter); }
    free_nn(declv);
    return -1;
   } while (++iter != end);
  } break;

  case DEE_XASTKIND_CONST:
   if (DeeNone_Check(ast_dest->ast_const.c_const)) {
    // Storing to none simply drops the value
    self->vds_kind = DEE_VARDECLSTORAGEKIND_NONE;
    return 0;
   }
  default: // Fallback: Create an assign operation
   if (DeeError_CompilerErrorf(DEE_WARNING_STORE_IS_ASSIGN,
    (DeeObject *)lexer,(DeeObject *)ast_dest->ast_common.ast_token,
    "Using '%r' in a var declaration will be compiled as assign ':='",ast_dest) != 0) return -1;
   self->vds_kind = DEE_VARDECLSTORAGEKIND_ASSIGN;
   Dee_INCREF(self->vds_assign.as_ast = ast_dest);
   break;
 }
 return 0;
}

DEE_DECL_END

