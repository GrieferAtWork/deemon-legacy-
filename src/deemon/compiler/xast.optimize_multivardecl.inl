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

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_OptimizeMultiVarDecl(
 DEE_A_INOUT DeeXAstObject *self, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeScopeObject *curr_scope) {
 DEE_ASSERT(DeeObject_Check(self) && DeeXAst_Check(self));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(self->ast_kind == DEE_XASTKIND_MULTIVARDECL);
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_MULTIVARDECL) != 0) {
  DeeXAstObject *ast_init = self->ast_multivardecl.mvd_init;
  switch (ast_init->ast_kind) {

   case DEE_XASTKIND_CONST: {
    DeeXAstObject **elem_v,**iter,**end,*init_const;
    Dee_size_t storage_c; struct DeeVarDeclStorage *storage_src;
    DeeXAstKind vardecl_kind;
    vardecl_kind = self->ast_multivardecl.mvd_type;
    storage_c = self->ast_multivardecl.mvd_varc;
    storage_src = self->ast_multivardecl.mvd_varv;
    while ((elem_v = (DeeXAstObject **)malloc_nz(storage_c*sizeof(DeeXAstObject *))) == NULL) {
     if DEE_LIKELY(Dee_CollectMemory()) continue;
     DeeError_NoMemory();
     return NULL;
    }
    end = (iter = elem_v)+storage_c;
    if (DeeNone_Check(ast_init->ast_const.c_const)) {
     // 'none' can be re-expanded to any amount of 'none'
     if ((init_const = DeeXAst_NewConst(ast_init->ast_common.ast_token,
      Dee_None)) == NULL) goto err_const_elemv;
     // Optimize: 'x,y,z = none...;' -> 'x = none,y = none,z = none;' 
     do {
      *iter = DeeXAst_NewStoreVardeclStorage(self->ast_common.ast_token,
                                             lexer,parser_flags,storage_src,
                                             init_const,curr_scope);
      if (!*iter) {/*err_const_init:*/ Dee_DECREF(init_const); goto err_const_iter; }
      ++iter,++storage_src;
     } while (iter != end);
     Dee_DECREF(init_const);
    } else {
     DeeObject *init_iterator,*init_elem; int error;
     if ((init_iterator = DeeObject_IterSelf(
      ast_init->ast_const.c_const)) == NULL) goto err_const_elemv;
     while ((error = DeeObject_IterNextEx(init_iterator,&init_elem)) == 0) {
      init_const = DeeXAst_NewConst(ast_init->ast_common.ast_token,init_elem);
      Dee_DECREF(init_elem);
      if (!init_const) {
err_const_iterator:
       Dee_DECREF(init_iterator);
err_const_iter:
       while (iter != elem_v) Dee_DECREF(*--iter);
err_const_elemv:
       free_nn(elem_v);
       return NULL;
      }
      *iter = DeeXAst_NewStoreVardeclStorage(self->ast_common.ast_token,
                                             lexer,parser_flags,storage_src,
                                             init_const,curr_scope);
      Dee_DECREF(init_const);
      if (!*iter) goto err_const_iterator;
      if (++iter == end) {
       // Make sure the iterator is empty
       if ((error = DeeObject_IterNextEx(init_iterator,&init_elem)) < 0) break;
       if (error == 0) {
        Dee_DECREF(init_elem);
        if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP,
         (DeeObject *)lexer,(DeeObject *)self->ast_common.ast_token,
         "Invalid sequence expand size (expected: %Iu, but got >= %Iu)",
         storage_c,storage_c+1) != 0) goto err_const_iterator;
        goto err_rt_invalid_iterator_size;
       }
       goto end_const_iter;
      }
      ++storage_src;
     }
     if (error == 1) {
      if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP,
       (DeeObject *)lexer,(DeeObject *)self->ast_common.ast_token,
       "Invalid sequence expand size (expected: %Iu, but got %Iu)",
       storage_c,(Dee_size_t)(iter-elem_v)) != 0) goto err_const_iterator;
err_rt_invalid_iterator_size:
      Dee_DECREF(init_iterator);
      while (iter != elem_v) Dee_DECREF(*--iter);
      free_nn(elem_v);
      goto normal_ast;
     }
end_const_iter:
     Dee_DECREF(init_iterator);
     if (error < 0) {
      DeeTypeObject *rt_error;
/*err_rt_init_iterator:*/
      while (iter != elem_v) Dee_DECREF(*--iter);
      free_nn(elem_v);
      if ((rt_error = _DeeError_HandleAndReturnType()) != NULL) {
       error = DeeXAst_ThrowInvalidUnaryOperatorError(DEE_XASTKIND_EXPAND,
                                                      self->ast_common.ast_token,
                                                      lexer,ast_init,rt_error);
       Dee_DECREF(rt_error);
       if (error < 0) return NULL;
      }
      goto normal_ast;
     }
    }
    _DeeXAstMultiVarDeclAst_Quit(&self->ast_multivardecl);
    self->ast_kind = vardecl_kind;
    self->ast_sequence.s_elemc = storage_c;
    self->ast_sequence.s_elemv = elem_v;
    goto optimize_self_seq;
   } break;

   case DEE_XASTKIND_TUPLE:
   case DEE_XASTKIND_LIST:
   case DEE_XASTKIND_SET: {
    Dee_size_t init_size = DeeXAst_SequenceSize(ast_init);
    if (init_size == (Dee_size_t)-1) break; // Unpredictable
    // Optimize: 'x,y,z = [foo(),42,"bar"]...;' -> 'x = foo(),y = 42,z = "bar";' 
    if (init_size != self->ast_multivardecl.mvd_varc) {
     if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP,
      (DeeObject *)lexer,(DeeObject *)self->ast_common.ast_token,
      "Invalid sequence expand size (expected: %Iu, but got %Iu)",
      self->ast_multivardecl.mvd_varc,init_size) != 0) return NULL;
    } else {
     DeeXAstObject **elem_v,**iter,**end,**init_src;
     struct DeeVarDeclStorage *storage_src;
     DeeXAstKind vardecl_kind = self->ast_multivardecl.mvd_type;
     DEE_ASSERT(init_size == self->ast_multivardecl.mvd_varc);
     while ((elem_v = (DeeXAstObject **)malloc_nz(
      init_size*sizeof(DeeXAstObject *))) == NULL) {
      if DEE_LIKELY(Dee_CollectMemory()) continue;
      DeeError_NoMemory();
      return NULL;
     }
     init_src = ast_init->ast_sequence.s_elemv;
     storage_src = self->ast_multivardecl.mvd_varv;
     end = (iter = elem_v)+init_size;
     while (iter != end) {
      *iter = DeeXAst_NewStoreVardeclStorage(self->ast_common.ast_token,
                                             lexer,parser_flags,storage_src,
                                             *init_src,curr_scope);
      if (!*iter) {
       while (iter != elem_v) Dee_DECREF(*--iter);
       free_nn(elem_v);
       return NULL;
      }
      ++iter,++storage_src,++init_src;
     }
     _DeeXAstMultiVarDeclAst_Quit(&self->ast_multivardecl);
     self->ast_kind = vardecl_kind;
     self->ast_sequence.s_elemc = init_size;
     self->ast_sequence.s_elemv = elem_v;
optimize_self_seq:
     return DeeXAst_OptimizeSequence(self,lexer,parser_flags);
    }
   } break;

   case DEE_XASTKIND_DICT: {
    // Optimize: 'a,b,c = dict { x:10,20:20,30:30 }...;' -> 'a = pack(x,10),b = pack(20,20),c = pack(30,30);'
    if (self->ast_multivardecl.mvd_varc != ast_init->ast_dict.d_elemc) {
     if (DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP,
      (DeeObject *)lexer,(DeeObject *)self->ast_common.ast_token,
      "Invalid sequence expand size (expected: %Iu, but got %Iu)",
      self->ast_multivardecl.mvd_varc,ast_init->ast_dict.d_elemc) != 0) return NULL;
    } else {
     struct DeeXAstDictEntry *src_iter; Dee_size_t storage_c;
     struct DeeVarDeclStorage *storage_src;
     DeeXAstObject **elem_v,**iter,**end;
     DeeXAstObject *init_tuple;
     DeeXAstKind vardecl_kind;
     vardecl_kind = self->ast_multivardecl.mvd_type;
     storage_c = self->ast_multivardecl.mvd_varc;
     storage_src = self->ast_multivardecl.mvd_varv;
     src_iter = ast_init->ast_dict.d_elemv;
     DEE_ASSERT(storage_c == ast_init->ast_dict.d_elemc);
     while ((elem_v = (DeeXAstObject **)malloc_nz(
      storage_c*sizeof(DeeXAstObject *))) == NULL) {
      if DEE_LIKELY(Dee_CollectMemory()) continue;
      DeeError_NoMemory();
      return NULL;
     }
     end = (iter = elem_v)+storage_c;
     // Expand as tuples
     do {
      DeeXAstObject *key_item[2];
      key_item[0] = src_iter->de_key;
      key_item[1] = src_iter->de_item;
      init_tuple = DeeXAst_NewTuple(key_item[0]->ast_common.ast_token,
                                    lexer,parser_flags,2,key_item);
      if (!init_tuple) {
err_dict_iter:
       while (iter != elem_v) Dee_DECREF(*--iter);
       free_nn(elem_v);
       return NULL;
      }
      *iter = DeeXAst_NewStoreVardeclStorage(self->ast_common.ast_token,
                                             lexer,parser_flags,storage_src,
                                             init_tuple,curr_scope);
      Dee_DECREF(init_tuple);
      if (!*iter) goto err_dict_iter;
      ++iter,++storage_src,++src_iter;
     } while (iter != end);
     _DeeXAstMultiVarDeclAst_Quit(&self->ast_multivardecl);
     self->ast_kind = vardecl_kind;
     self->ast_sequence.s_elemc = storage_c;
     self->ast_sequence.s_elemv = elem_v;
     goto optimize_self_seq;
    }
   } break;

   default: break;
  }
 }
normal_ast:
 Dee_INCREF(self);
 return self;
}


DEE_DECL_END
