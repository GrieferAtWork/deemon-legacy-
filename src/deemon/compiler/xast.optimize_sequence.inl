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

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_OptimizeSequence(
 DEE_A_INOUT DeeXAstObject *self, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags) {
 DeeXAstObject *result; DeeXAstObject **iter,**end,*elem;
 DeeXAstObject **astv,**new_astv; Dee_size_t astc;
 DeeObject *const_result; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeXAst_Check(self));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERTF(self->ast_kind == DEE_XASTKIND_TUPLE ||
             self->ast_kind == DEE_XASTKIND_LIST ||
             self->ast_kind == DEE_XASTKIND_SET,"Given xast kind isn't a sequence");
 DEE_ASSERT(self->ast_sequence.s_elemc != 0);
 DEE_ASSERT(self->ast_sequence.s_elemv != NULL);
 astc = self->ast_sequence.s_elemc;
 astv = self->ast_sequence.s_elemv;
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_MERGE_OPERATORS)!=0) {
  // TODO: Optimization: [10,(20,30)...,40] --> [10,20,30,40]
  //       NOTE: Should also work with non-constant elements
  end = (iter = astv)+astc;
merge_sequences:
  do {
   elem = *iter;
   if (elem->ast_kind == DEE_XASTKIND_EXPAND) {
    elem = elem->ast_operator.op_a;
    switch (elem->ast_kind) {
     // NOTE: If it's constant, it will be handled later

     case DEE_XASTKIND_DICT: {
      DEE_ASSERT(self->ast_dict.d_elemc);
      // TODO: Repack as tuples
     } break;

     case DEE_XASTKIND_CELL: {
      Dee_INCREF(elem = self->ast_operator.op_a);
      goto seq_replace_elem;
     } break;

     case DEE_XASTKIND_CONST: {
      DeeObject *const_seq_iter,*const_seq_elem;
      if ((const_seq_iter = DeeObject_IterSelf(
       elem->ast_const.c_const)) == NULL) goto err_rt_iter_error;
      if ((error = DeeObject_IterNextEx(const_seq_iter,&const_seq_elem)) < 0) {
err_rt_const_seq_iter: Dee_DECREF(const_seq_iter); goto err_rt_iter_error;
      }
      if (error != 0) {
/*seq_remove_elem:*/
       // Simple enough: Simply remove this entry
       VLOG_PARSER((*iter)->ast_common.ast_token,"Removing empty expanded sequence\n");
       Dee_DECREF(*iter);
       memmove(iter,iter+1,((end-iter)-1)*sizeof(DeeXAstObject *));
       --astc,--end,--self->ast_sequence.s_elemc;
       if (iter == end) { Dee_DECREF(const_seq_iter); goto end_merge_operators; }
      } else {
       DeeXAstObject *seq_elem;
       DeeXAst_AssignConst(*iter,const_seq_elem);
       Dee_DECREF(const_seq_elem);
       while ((error = DeeObject_IterNextEx(const_seq_iter,&const_seq_elem)) == 0) {
        // Insert a new element
        while ((new_astv = (DeeXAstObject **)realloc_nnz(
         astv,(astc+1)*sizeof(DeeXAstObject *))) == NULL) {
         if DEE_LIKELY(Dee_CollectMemory()) continue;
         DeeError_NoMemory();
         Dee_DECREF(elem);
err_const_seq_iter:
         Dee_DECREF(const_seq_iter);
         if (!self->ast_sequence.s_elemc) goto err_fix_self;
         return NULL;
        }
        iter = new_astv+(iter-astv);
        end = new_astv+(++astc);
        astv = new_astv;
        memmove(iter+1,iter,((Dee_size_t)((end-iter)-1))*sizeof(DeeXAstObject *));
        self->ast_sequence.s_elemc = astc;
        self->ast_sequence.s_elemv = new_astv;
        seq_elem = DeeXAst_NewConst((*iter)->ast_common.ast_token,const_seq_elem);
        Dee_DECREF(const_seq_elem);
        if (!seq_elem) goto err_const_seq_iter;
        Dee_INHERIT_REF(*++iter,seq_elem);
       }
       if (error < 0) goto err_rt_const_seq_iter;
      }
      Dee_DECREF(const_seq_iter);
     } break;

     case DEE_XASTKIND_TUPLE:
     case DEE_XASTKIND_LIST:
     case DEE_XASTKIND_SET: {
      // Inline elements (NOTE: Ignore the fact that a set would probably screw with the order)
      DEE_ASSERT(elem->ast_sequence.s_elemc != 0);
      if (elem->ast_sequence.s_elemc == 1) {
       // Even easier: Just replace the entry
       Dee_INCREF(elem = elem->ast_sequence.s_elemv[0]);
seq_replace_elem:
       VLOG_PARSER(elem->ast_common.ast_token,"Inlining single-element expanded sequence\n");
       Dee_DECREF(*iter);
       Dee_INHERIT_REF(*iter,elem);
      } else {
       Dee_size_t old_size,new_size,inl_size;
       inl_size = elem->ast_sequence.s_elemc;
       VLOG_PARSER(elem->ast_common.ast_token,"Inlining expanded sequence (%Iu elements)\n",inl_size);
       new_size = ((old_size = astc)-1)+inl_size;
       // This is where it gets complicated, because we need to realloc the sequence
       while ((new_astv = (DeeXAstObject **)realloc_nnz(
        astv,new_size*sizeof(DeeXAstObject *))) == NULL) {
        if DEE_LIKELY(Dee_CollectMemory()) continue;
        DeeError_NoMemory();
        if (!self->ast_sequence.s_elemc) goto err_fix_self;
        return NULL;
       }
       // Update pointers and sizes
       self->ast_sequence.s_elemc = new_size;
       self->ast_sequence.s_elemv = new_astv;
       iter = new_astv+(iter-astv);
       end = new_astv+new_size;
       astv = new_astv;
       // Delete the expand ast
       Dee_INCREF(elem);
       Dee_DECREF(*iter);
       // Now make space for the inlined sequence
       memmove((iter+1)+(inl_size-1),iter+1,
               ((old_size-(Dee_size_t)(iter-astv))-1)*sizeof(DeeXAstObject *));
       // Now copy the inlined sequence into our's, starting at 'iter'
       {
        DeeXAstObject **copy_iter,**copy_end,**copy_src;
        copy_end = (copy_iter = iter)+inl_size;
        copy_src = elem->ast_sequence.s_elemv;
        while (copy_iter != copy_end) Dee_INCREF(*copy_iter++ = *copy_src++);
       }
       Dee_DECREF(elem);
      }
      goto merge_sequences;
     } break;
     default: break;
    }
   }
  } while (++iter != end);
 }
end_merge_operators:
 if (!astc) {
  DEE_ASSERT(!self->ast_sequence.s_elemc);
  DEE_ASSERT(self->ast_sequence.s_elemv);
  switch (self->ast_kind) {
   case DEE_XASTKIND_TUPLE: Dee_INCREF(const_result = Dee_EmptyTuple); break;
   case DEE_XASTKIND_LIST:
    if ((const_result = DeeList_New()) == NULL) {
err_fix_self:
     DEE_ASSERT(!self->ast_sequence.s_elemc);
     DEE_ASSERT(self->ast_sequence.s_elemv);
     free_nn(self->ast_sequence.s_elemv);
     self->ast_kind = DEE_XASTKIND_CONST;
     self->ast_const.c_const = DeeNone_New();
     return NULL;
    }
    break;
   case DEE_XASTKIND_SET:
    if ((const_result = DeeSet_New()) == NULL) goto err_fix_self;
    break;
   default: DEE_BUILTIN_UNREACHABLE();
  }
  free_nn(self->ast_sequence.s_elemv);
  self->ast_kind = DEE_XASTKIND_CONST;
  Dee_INHERIT_REF(self->ast_const.c_const,const_result);
  goto normal_ast;
 }
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE)!=0) {
  // Check to make sure that all given asts are either constant, or are expanding a constant
  end = (iter = (DeeXAstObject **)astv)+astc;
  while (iter != end) {
   elem = *iter++;
   if (elem->ast_kind != DEE_XASTKIND_CONST ||
      (elem->ast_kind == DEE_XASTKIND_EXPAND &&
       elem->ast_operator.op_a->ast_kind != DEE_XASTKIND_CONST)
       ) goto normal_ast; // Not a constant
  }
  // This does qualify for a constant, so we can proceed
  switch (self->ast_kind) {
   case DEE_XASTKIND_TUPLE: Dee_INCREF(const_result = Dee_EmptyTuple); break;
   case DEE_XASTKIND_LIST:  if ((const_result = DeeList_New()) == NULL) return NULL; break;
   case DEE_XASTKIND_SET:   if ((const_result = DeeSet_New()) == NULL) return NULL; break;
   default: DEE_BUILTIN_UNREACHABLE();
  }
  iter = (DeeXAstObject **)astv;
  while (iter != end) {
   elem = *iter++;
   if (elem->ast_kind == DEE_XASTKIND_CONST) {
    switch (self->ast_kind) {
     case DEE_XASTKIND_TUPLE: error = DeeTuple_Append(&const_result,elem->ast_const.c_const); break;
     case DEE_XASTKIND_LIST:  error = DeeList_Append(const_result,elem->ast_const.c_const); break;
     case DEE_XASTKIND_SET:   error = DeeSet_Insert(const_result,elem->ast_const.c_const); break;
     default: DEE_BUILTIN_UNREACHABLE();
    }
    if (error != 0) { Dee_DECREF(const_result); return NULL; }
   } else {
    DEE_ASSERTF(elem->ast_kind == DEE_XASTKIND_EXPAND &&
                elem->ast_operator.op_a->ast_kind == DEE_XASTKIND_CONST,
                "We just checking this...");
    switch (self->ast_kind) {
     case DEE_XASTKIND_TUPLE: error = DeeTuple_AppendSequence(&const_result,elem->ast_operator.op_a->ast_const.c_const); break;
     case DEE_XASTKIND_LIST:  error = DeeList_AppendSequence(const_result,elem->ast_operator.op_a->ast_const.c_const); break;
     case DEE_XASTKIND_SET:   error = DeeSet_InsertSequence(const_result,elem->ast_operator.op_a->ast_const.c_const); break;
     default: DEE_BUILTIN_UNREACHABLE();
    }
    if (error != 0) {
     Dee_DECREF(const_result);
err_rt_iter_error:
     if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP,
      (DeeObject *)lexer,(DeeObject *)elem->ast_common.ast_token,
      "Iterating %q will cause an %K at runtime",
      DeeType_NAME(Dee_TYPE(elem->ast_operator.op_a->ast_const.c_const)),
      _DeeError_HandleAndReturnType()) != 0) return NULL;
     goto normal_ast;
    }
   }
  }
  result = DeeXAst_NewConst(self->ast_common.ast_token,const_result);
  Dee_DECREF(const_result);
  return result;
 }
normal_ast:
 Dee_INCREF(self);
 return self;
}

DEE_DECL_END
