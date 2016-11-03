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
#ifndef GUARD_DEEMON_SAST_COMPILE_C
#define GUARD_DEEMON_SAST_COMPILE_C 1
#define DEE_LIMITED_API 1

// include/*
#include <deemon/__conf.inl>
#include <deemon/compiler/code.h>
#include <deemon/compiler/compiler.h>
#include <deemon/error.h>
#include <deemon/list.h>
#include <deemon/none.h>
#include <deemon/runtime/execute.h>
#include <deemon/tuple.h>

// src/*
#include <deemon/compiler/__opcode.inl>
#include <deemon/compiler/codewriter.h>
#include <deemon/compiler/sast.h>
#include <deemon/compiler/xast.h>
#include <deemon/runtime/builtin_functions.h>

// */ (nano...)

DEE_DECL_BEGIN

#ifdef DEE_SCOPE_FLAG_USING_LOCALS
#define have_local_names \
 ((compiler_flags&DEE_COMPILER_FLAG_DEBUG_LOCALS)!=0||\
 (((DeeScopeObject *)scope)->ob_flags&DEE_SCOPE_FLAG_USING_LOCALS)!=0)
#else
#define have_local_names \
 ((compiler_flags&DEE_COMPILER_FLAG_DEBUG_LOCALS)!=0)
#endif
#define ret_used  ((compiler_flags&DEE_COMPILER_FLAG_USED)!=0)

DEE_A_RET_EXCEPT(-1) int DeeSAst_CompileStrongScopeRoot(
 DEE_A_INOUT DeeSAstObject *self, DEE_COMPILER_PARAMS) {
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 DEE_ASSERTF(DeeScope_IS_STRONG(scope),"DeeSAst_CompileStrongScopeRoot() expects a strong scope");
 if (self->ast_kind == DEE_SASTKIND_BLOCK) {
  if DEE_UNLIKELY(DeeSAst_CompileLabelEndpoints(&self->ast_common.ast_labels,
   self->ast_common.ast_token,DEE_COMPILER_ARGS) != 0) return -1;
  if DEE_UNLIKELY(DeeSAst_CompileBlockWithoutLocalCleanup(
   self->ast_block.b_astc,self->ast_block.b_astv,DEE_COMPILER_ARGS) != 0) return -1;
 } else {
  if DEE_UNLIKELY(DeeSAst_Compile(self,DEE_COMPILER_ARGS) != 0) return -1;
 }
 // Finalize labels in the scope
 return DeeCodeWriter_FinalizeStrongScope(writer,scope,lexer,compiler_flags);
}

DEE_A_RET_EXCEPT(-1) int DeeSAst_CompileBlockWithoutLocalCleanup(
 DEE_A_IN Dee_size_t blockc, DEE_A_IN_R(blockc) DeeSAstObject *const *blockv, DEE_COMPILER_PARAMS) {
 DeeSAstObject **iter,**end;
 end = (iter = (DeeSAstObject **)blockv)+blockc;
 if (ret_used) {
  if (iter != end) {
   --end; // Compile the last entry differently
   while (iter != end) {
    if DEE_UNLIKELY(DeeSAst_Compile(*iter,DEE_COMPILER_ARGS_EX(
     compiler_flags&~(DEE_COMPILER_FLAG_USED))) != 0) return -1;
    ++iter;
   }
   if DEE_UNLIKELY(DeeSAst_Compile(*iter,DEE_COMPILER_ARGS) != 0) return -1;
  } else {
   if DEE_UNLIKELY(DeeCodeWriter_PushNone(writer) != 0) return -1;
  }
 } else {
  while (iter != end) {
   if DEE_UNLIKELY(DeeSAst_Compile(*iter,DEE_COMPILER_ARGS) != 0) return -1;
   ++iter;
  }
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeSAst_CompileLabelEndpoints(
 DEE_A_IN struct DeeParserLabelRefList const *endpts,
 DEE_A_IN DeeTokenObject *error_token, DEE_COMPILER_PARAMS) {
 struct DeeParserLabel *elem,**label_iter,**label_end; // Put label endpoints
 (void)DEE_COMPILER_ARGS;
 DEE_ASSERT((DeeParserLabelRefList_SIZE(endpts) != 0) == (DeeParserLabelRefList_ELEM(endpts) != NULL));
 label_end = (label_iter = DeeParserLabelRefList_ELEM(endpts)
              )+DeeParserLabelRefList_SIZE(endpts);
 while (label_iter != label_end) {
  elem = *label_iter++;
  if DEE_UNLIKELY(DeeCodeWriter_PutLabel(
   writer,&elem->pl_code_label,lexer,error_token) != 0) return -1;
 }
 return 0;
}

//static DEE_A_RET_EXCEPT(-1) int DeeXAst_CompileWithoutExpand() {
//}


DEE_COMPILER_MSVC_WARNING_PUSH(4701)
DEE_A_RET_EXCEPT(-1) int DeeSAst_Compile(DEE_A_INOUT DeeSAstObject *self, DEE_COMPILER_PARAMS) {
 DEE_ASSERT(DeeObject_Check(self) && DeeSAst_Check(self));
 if DEE_UNLIKELY(DeeSAst_CompileLabelEndpoints(&self->ast_common.ast_labels,
  self->ast_common.ast_token,DEE_COMPILER_ARGS) != 0) return -1;
 if ((compiler_flags&DEE_COMPILER_FLAG_DEBUG_FILE_AND_LINE)!=0) {
  // Always put debug info if this statement is a label endpoint
  if DEE_LIKELY(DeeParserLabelRefList_EMPTY(&self->ast_common.ast_labels)) {
   // Only put debug information if the statement isn't noexcept or has side-effects
   if ((DeeSAst_Attr(self,(Dee_uint32_t)(
    (compiler_flags&DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE)!=0
    ?DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK:DEE_AST_ATTRIBUTE_FLAG_NONE))&
    (DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT)) ==
    (DEE_AST_ATTRIBUTE_NOEXCEPT|DEE_AST_ATTRIBUTE_NOEFFECT)) goto no_debug;
  }
/*put_debug:*/
  // Put some debug information
  if DEE_UNLIKELY(DeeCodeWriter_Debug(writer,
   (compiler_flags&DEE_COMPILER_FLAG_DEBUG_FILE) != 0 ? DeeToken_FILE(self->ast_common.ast_token) : NULL,
   (compiler_flags&DEE_COMPILER_FLAG_DEBUG_LINE) != 0 ? DeeToken_LINE(self->ast_common.ast_token) : 0
   ) != 0) return -1;
 }
no_debug:
 switch (self->ast_kind) {
  case DEE_SASTKIND_EXPRESSION:
   return DeeXAst_Compile(self->ast_expression.x_ast,DEE_COMPILER_ARGS);

  case DEE_SASTKIND_BLOCK: {
   if DEE_UNLIKELY(DeeSAst_CompileBlockWithoutLocalCleanup(
    self->ast_block.b_astc,self->ast_block.b_astv,
    DEE_COMPILER_ARGS_SCOPE(self->ast_block.b_scope)) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(writer,self->ast_block.b_scope,lexer,compiler_flags) != 0) return -1;
  } break;

  case DEE_SASTKIND_RETURN: {
   if (self->ast_return.r_value) {
    DEE_ASSERT((DeeScope_WEAK_ROOT(scope)->sc_flags&DEE_SCOPE_FLAG_FOUND_YIELD)==0);
    if DEE_UNLIKELY(DeeXAst_Compile(self->ast_return.r_value,DEE_COMPILER_ARGS_EX(
     compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
    if DEE_UNLIKELY(DeeCodeWriter_Ret(writer) != 0) return -1;
   } else {
    if ((DeeScope_WEAK_ROOT(scope)->sc_flags&DEE_SCOPE_FLAG_FOUND_YIELD)!=0) {
     if DEE_UNLIKELY(DeeCodeWriter_YieldExit(writer) != 0) return -1;
    } else if ((DeeScope_WEAK_ROOT(scope)->sc_flags&DEE_SCOPE_FLAG_FOUND_RETVAR)!=0) {
     if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_RETVAREXIT) != 0) return -1;
    } else {
     if DEE_UNLIKELY(DeeCodeWriter_RetNone(writer) != 0) return -1;
    }
   }
on_unreachable:
   if (ret_used) DeeCodeWriter_INCSTACK(writer);
   if DEE_UNLIKELY((compiler_flags&DEE_COMPILER_FLAG_GEN_UNREACHABLE)!=0 &&
                   DeeCodeWriter_WriteOp(writer,OP_UNREACHABLE) != 0) return -1;
  } break;

  case DEE_SASTKIND_YIELD: {
   DeeXAstObject *yvalue;
   yvalue = self->ast_yield.y_value;
   switch (yvalue->ast_kind) {
    case DEE_XASTKIND_CONST: {
     DeeObject *iterator,*elem; int error;
     Dee_size_t old_code_pos;
     writer->cw_code_pos;
     if DEE_UNLIKELY((iterator = DeeObject_IterSelf(yvalue->ast_const.c_const)) == NULL) {
handle_error_and_yield_default: DeeError_Handled(); goto yield_default;
     }
     old_code_pos = DeeCodeWriter_ADDR(writer);
     while ((error = DeeObject_IterNextEx(iterator,&elem)) == 0) {
      error = DeeCodeWriter_PushConst(writer,elem,compiler_flags);
      Dee_DECREF(elem);
      if DEE_UNLIKELY(error != 0) {err_yield_iterator: Dee_DECREF(iterator); return -1; }
      if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_YIELD) != 0) goto err_yield_iterator;
      DeeCodeWriter_DECSTACK(writer);
     }
     Dee_DECREF(iterator);
     if DEE_UNLIKELY(error < 0) {
      // Compile using the default way
      DeeCodeWriter_SETADDR(writer,old_code_pos); // Ignore what we just did...
      goto handle_error_and_yield_default;
     }
    } break;

    case DEE_XASTKIND_TUPLE:
    case DEE_XASTKIND_LIST:
    case DEE_XASTKIND_SET: {
     DeeXAstObject **iter,**end,*elem;
     end = (iter = yvalue->ast_sequence.s_elemv)+yvalue->ast_sequence.s_elemc;
     DEE_ASSERT(yvalue->ast_sequence.s_elemc);
     do {
      elem = *iter;
      if (elem->ast_kind == DEE_XASTKIND_EXPAND) {
       // Expand an elemntal sequence
       if DEE_UNLIKELY(DeeXAst_Compile(elem->ast_operator.op_a,DEE_COMPILER_ARGS_EX(
        compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
       if DEE_UNLIKELY(DeeCodeWriter_UnaryOp(writer,OP_SEQ_ITER_SELF) != 0) return -1;
       if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_YIELDALL) != 0) return -1;
      } else {
       if DEE_UNLIKELY(DeeXAst_Compile(elem,DEE_COMPILER_ARGS_EX(
        compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
       if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_YIELD) != 0) return -1;
      }
      DeeCodeWriter_DECSTACK(writer);
     } while (++iter != end);
    } break;

    case DEE_XASTKIND_DICT: {
     struct DeeXAstDictEntry *iter,*end;
     end = (iter = yvalue->ast_dict.d_elemv)+yvalue->ast_dict.d_elemc;
     DEE_ASSERT(yvalue->ast_dict.d_elemv != NULL);
     DEE_ASSERT(yvalue->ast_dict.d_elemc != 0);
     do { // Yield as tuples
      if (iter->de_key->ast_kind == DEE_XASTKIND_CONST &&
          iter->de_item->ast_kind == DEE_XASTKIND_CONST) {
       DeeObject *yield_tuple; int error; // Yield as constant tuple
       if DEE_UNLIKELY((yield_tuple = DeeTuple_Pack(2,iter->de_key,iter->de_item)) == NULL) return -1;
       error = DeeCodeWriter_PushConst(writer,yield_tuple,compiler_flags);
       Dee_DECREF(yield_tuple);
       if DEE_UNLIKELY(error != 0) return error;
      } else {
       if DEE_UNLIKELY(DeeXAst_Compile(iter->de_key,DEE_COMPILER_ARGS_EX(
        compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
       if DEE_UNLIKELY(DeeXAst_Compile(iter->de_item,DEE_COMPILER_ARGS_EX(
        compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
       if DEE_UNLIKELY(DeeCodeWriter_MakeTuple(writer,2) != 0) return -1;
      }
      if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_YIELD) != 0) return -1;
      DeeCodeWriter_DECSTACK(writer);
     } while (++iter != end);
    } break;

    default: // default: Yield all from sequence
yield_default:
     if DEE_UNLIKELY(DeeXAst_Compile(yvalue,DEE_COMPILER_ARGS_EX(
      compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_UnaryOp(writer,OP_SEQ_ITER_SELF) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_YIELDALL) != 0) return -1;
     DeeCodeWriter_DECSTACK(writer);
     break;
   }
   goto push_none; // yield statements return 'none'
  } break;

  case DEE_SASTKIND_ASSERT: {
   Dee_size_t check_pos,check_jmparg;
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_assert.a_check,DEE_COMPILER_ARGS_EX(
    compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   check_pos = DeeCodeWriter_ADDR(writer); // setup the jump if the check fails
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(writer,OP_JUMP_IF_TT,&check_jmparg) != 0) return -1;
   // Assertion failed --> Compile the message (if there is one)
   if (self->ast_assert.a_message) {
    if DEE_UNLIKELY(DeeXAst_Compile(self->ast_assert.a_message,DEE_COMPILER_ARGS_EX(
    compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
    // Pack the assert expression and message into a tuple
    if DEE_UNLIKELY(_DeeCodeWriter_MakeSequence(writer,OP_TUPLE,2) != 0) return -1;
   } else { // Pack ~only~ the assert expression into a tuple
    if DEE_UNLIKELY(_DeeCodeWriter_MakeSequence(writer,OP_TUPLE,1) != 0) return -1;
   }
   if DEE_UNLIKELY(DeeCodeWriter_PushConst(writer,(DeeObject *)&
    DeeBuiltinFunction___builtin_assertion_failed,compiler_flags) != 0) return -1;
   // Rotate the assertion_failed function with it's argument tuple
   if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_ROT_2) != 0) return -1;
   // Call the function
   if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,OP_CALL) != 0) return -1;
   // NOTE: At this point, the function return value is still on the stack, but
   //       the pop that would otherwise remove the assert expression can handle that as well!
   if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,check_jmparg,(
    Dee_size_t)(DeeCodeWriter_ADDR(writer)-check_pos)) != 0) return -1;
   // Pop the assert expression (or '__builtin_assertion_failed' return value, if it didn't throw an error for some reason...)
   if DEE_UNLIKELY(DeeCodeWriter_Pop(writer) != 0) return -1;
   goto push_none; // assert statements return 'none'
  } break;

  case DEE_SASTKIND_PRINT: {
#define no_seperators    ((self->ast_print.p_flags&DEE_SASTPRINT_FLAG_NOSEP)!=0)
#define append_linefeed  ((self->ast_print.p_flags&DEE_SASTPRINT_FLAG_NOLF)==0)
   DeeXAstObject *print_file = self->ast_print.p_file;
   DeeXAstObject *print_data = self->ast_print.p_data;
   if (print_file) {
    if (!print_data && (self->ast_print.p_flags&DEE_SASTPRINT_FLAG_NOLF)!=0) {
     if DEE_UNLIKELY(DeeXAst_Compile(print_file,DEE_COMPILER_ARGS_EX(
      compiler_flags&~(DEE_COMPILER_FLAG_USED))) != 0) return -1;
     goto push_none;
    }
    if DEE_UNLIKELY(DeeXAst_Compile(print_file,DEE_COMPILER_ARGS_EX(
     compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   }
   if (print_data) {
    switch (print_data->ast_kind) {

     case DEE_XASTKIND_TUPLE:
     case DEE_XASTKIND_LIST:
     case DEE_XASTKIND_SET: {
      DeeXAstObject **iter,**end,*elem;
      end = (iter = print_data->ast_sequence.s_elemv)+print_data->ast_sequence.s_elemc;
      DEE_ASSERT(print_data->ast_sequence.s_elemc);
      do {
       elem = *iter++;
       if (elem->ast_kind == DEE_XASTKIND_EXPAND) {
        // TODO: Expanded expressions currently ignore the 'no_seperators' flag
        if DEE_UNLIKELY(DeeXAst_Compile(elem->ast_operator.op_a,DEE_COMPILER_ARGS_EX(
         compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
        if (print_file) {
         if (iter == end) {
          if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)(
           append_linefeed ? OP_PRINT_ALL_F_END : OP_PRINT_ALL_F)) != 0) return -1;
          DeeCodeWriter_DECSTACK_N(writer,2);
         } else {
          if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)(
           OP_PRINT_ALL_F_SEP)) != 0) return -1;
          DeeCodeWriter_DECSTACK(writer);
         }
        } else {
         if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)(iter == end
          ? (append_linefeed ? OP_PRINT_ALL_END : OP_PRINT_ALL)
          : OP_PRINT_ALL_SEP)) != 0) return -1;
         DeeCodeWriter_DECSTACK(writer);
        }
       } else {
        if DEE_UNLIKELY(DeeXAst_Compile(elem,DEE_COMPILER_ARGS_EX(
         compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
        if (print_file) {
         if (iter == end) {
          if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)(
           append_linefeed ? OP_PRINT_ONE_F_END : OP_PRINT_ONE_F)) != 0) return -1;
          DeeCodeWriter_DECSTACK_N(writer,2);
         } else {
          if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)(
           no_seperators ? OP_PRINT_ONE_F : OP_PRINT_ONE_F_SEP)) != 0) return -1;
          DeeCodeWriter_DECSTACK(writer);
         }
        } else {
         if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)(iter == end
          ? (append_linefeed ? OP_PRINT_ONE_END : OP_PRINT_ONE)
          : (no_seperators ? OP_PRINT_ONE : OP_PRINT_ONE_SEP))) != 0) return -1;
         DeeCodeWriter_DECSTACK(writer);
        }
       }
      } while (iter != end);
     } break;


     case DEE_XASTKIND_CONST: {
      DeeObject *iterator,*elem,*next_elem; int error;
      Dee_size_t old_writer_pos;
      if DEE_UNLIKELY((iterator = DeeObject_IterSelf(print_data->ast_const.c_const)) == NULL) {
handle_error_and_print_compile_default: DeeError_Handled(); goto print_compile_default;
      }
      if DEE_UNLIKELY((error = DeeObject_IterNextEx(iterator,&elem)) < 0)
       goto handle_error_and_print_compile_default;
      if (error != 0) { Dee_DECREF(iterator); goto empty_print_data; } // Empty printer
      old_writer_pos = DeeCodeWriter_ADDR(writer);
      // Print all elements (except for the last)
      while ((error = DeeObject_IterNextEx(iterator,&next_elem)) == 0) {
       if DEE_UNLIKELY(DeeCodeWriter_PushConst(writer,elem,compiler_flags) != 0 ||
                       DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)(print_file
                       ? OP_PRINT_ONE_F_SEP : OP_PRINT_ONE_SEP)) != 0) {
        Dee_DECREF(elem); Dee_DECREF(next_elem); Dee_DECREF(iterator);
        return -1;
       }
       DeeCodeWriter_DECSTACK(writer);
       Dee_DECREF(elem);
       Dee_INHERIT_REF(elem,next_elem);
      }
      Dee_DECREF(iterator);
      if DEE_UNLIKELY(error < 0) {
       Dee_DECREF(elem);
       // Restore the old writer position on error
       DeeCodeWriter_SETADDR(writer,old_writer_pos);
       goto handle_error_and_print_compile_default;
      }
      // Now print the last element
      if DEE_UNLIKELY(DeeCodeWriter_PushConst(writer,elem,compiler_flags) != 0) { Dee_DECREF(elem); return -1; }
      Dee_DECREF(elem);
      if (print_file) {
       if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)(
        append_linefeed ? OP_PRINT_ONE_F_END : OP_PRINT_ONE_F)) != 0) return -1;
       DeeCodeWriter_DECSTACK_N(writer,2);
      } else {
       if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)(
        append_linefeed ? OP_PRINT_ONE_END : OP_PRINT_ONE)) != 0) return -1;
       DeeCodeWriter_DECSTACK(writer);
      }
     } break;

      //case DEE_XASTKIND_DICT: // todo?

     default:print_compile_default:
      if DEE_UNLIKELY(DeeXAst_Compile(print_data,DEE_COMPILER_ARGS_EX(
       compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
      if (print_file) {
       if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)(
        append_linefeed ? OP_PRINT_ALL_F_END : OP_PRINT_ALL_F)) != 0) return -1;
       DeeCodeWriter_DECSTACK_N(writer,2);
      } else {
       if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)(
        append_linefeed ? OP_PRINT_ALL_END : OP_PRINT_ALL)) != 0) return -1;
       DeeCodeWriter_DECSTACK(writer);
      }
      break;
    }
   } else {
empty_print_data:
    if (append_linefeed) {
     // print;
     // print my_file:;
     if (print_file) {
      if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_PRINT_END_F) != 0) return -1;
      DeeCodeWriter_DECSTACK(writer);
     } else {
      if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_PRINT_END) != 0) return -1;
     }
    }
   }
   goto push_none; // Print returns 'none' now
#undef append_linefeed
  } break;

  case DEE_SASTKIND_IF: {
   Dee_size_t succ_pos,succ_jmparg;
   Dee_size_t fail_pos,fail_jmparg;
   DEE_ASSERT((self->ast_if.if_succ_block != NULL) == (self->ast_if.if_succ_scope != NULL));
   DEE_ASSERT((self->ast_if.if_fail_block != NULL) == (self->ast_if.if_fail_scope != NULL));
   // TODO: We can optimize the jumps, if the 'then' or 'else'
   //       block only contains a 'goto' statement
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_if.if_cond,DEE_COMPILER_ARGS_SCOPE_EX(
    self->ast_if.if_cond_scope,compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   succ_pos = DeeCodeWriter_ADDR(writer); // setup the jump if the check fails
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(
    writer,OP_JUMP_IF_FF_POP,&succ_jmparg) != 0) return -1;
   DeeCodeWriter_DECSTACK(writer);
   // Write the then (succ) branch
   if (self->ast_if.if_succ_block) {
    if DEE_UNLIKELY(DeeSAst_Compile(self->ast_if.if_succ_block,DEE_COMPILER_ARGS_SCOPE_EX(
     self->ast_if.if_succ_scope,compiler_flags)) != 0) return -1;
    if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(writer,self->ast_if.if_succ_scope,lexer,compiler_flags) != 0) return -1;
   } else if (ret_used && DEE_UNLIKELY(DeeCodeWriter_PushNone(writer) != 0)) return -1;

   if (self->ast_if.if_fail_block || ret_used) {
    fail_pos = DeeCodeWriter_ADDR(writer); // setup the jump if the check succeeds
    if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(writer,OP_JUMP,&fail_jmparg) != 0) return -1;
    // if false, we jump after the jump instruction that skips the fail branch
    if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,succ_jmparg,
     (Dee_size_t)(DeeCodeWriter_ADDR(writer)-succ_pos)) != 0) return -1;
    // Write the else (fail) branch
    if (self->ast_if.if_fail_block) { // else branch exists
     if DEE_UNLIKELY(DeeSAst_Compile(self->ast_if.if_fail_block,DEE_COMPILER_ARGS_SCOPE_EX(
     self->ast_if.if_fail_scope,compiler_flags)) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(writer,self->ast_if.if_fail_scope,lexer,compiler_flags) != 0) return -1;
    } else {
     DEE_ASSERT(ret_used);
     // In case of a missing else branch and a requested return value,
     // we use 'none' at that return value.
     if DEE_UNLIKELY(DeeCodeWriter_PushNone(writer) != 0) return -1;
    }
    if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,fail_jmparg,
     (Dee_size_t)(DeeCodeWriter_ADDR(writer)-fail_pos)) != 0) return -1;
    if (ret_used) DeeCodeWriter_DECSTACK(writer); // Only one branch is on the stack
   } else {
    // Then block is skipped (we end up after the expression)
    if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,succ_jmparg,
     (Dee_size_t)(DeeCodeWriter_ADDR(writer)-succ_pos)) != 0) return -1;
   }
   // Cleanup the condition scope
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(
    writer,self->ast_if.if_cond_scope,lexer,compiler_flags) != 0) return -1;
  } break;

  // generated code:
  //{
  //                  [init]
  //                  cont:
  // if_exists(done): if ([done]) goto end;
  //                  [block]
  //                  [next]
  //                  goto cont;
  // if_exists(done): end:
  //}
  case DEE_SASTKIND_FOR: {
   Dee_size_t for_cond_pos,for_end_jmparg;
   Dee_size_t for_end_origin;
   // initializer
   if DEE_UNLIKELY(self->ast_for.f_head_init && DeeXAst_Compile(
    self->ast_for.f_head_init,DEE_COMPILER_ARGS_SCOPE_EX(
    self->ast_for.f_head_init_scope,compiler_flags&~(DEE_COMPILER_FLAG_USED))
    ) != 0) return -1;
   // setup the 'cont' label with 'pos1'
   for_cond_pos = DeeCodeWriter_ADDR(writer);
   DeeCodeWriter_PUSH_LOOP(writer,for_cond_pos);
   DEE_ASSERT(DeeObject_Check(self->ast_for.f_head_cond) &&
              DeeXAst_Check(self->ast_for.f_head_cond));
   /*if (self->ast_for.f_head_cond)*/ {
    // setup the jump to 'end' if 'done' is true
    if DEE_UNLIKELY(DeeXAst_Compile(self->ast_for.f_head_cond,
     DEE_COMPILER_ARGS_SCOPE_EX(self->ast_for.f_head_cond_scope,
     compiler_flags|(DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE|
     DEE_COMPILER_FLAG_USED))) != 0) goto err_for;
    for_end_origin = DeeCodeWriter_ADDR(writer); // required for origin of 'goto end'
    if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(writer,OP_JUMP_IF_FF_POP,&for_end_jmparg) != 0) goto err_for;
    DeeCodeWriter_DECSTACK(writer); // 'OP_JUMP_IF_FF_POP' popped this one
   }
   if DEE_UNLIKELY(DeeSAst_Compile( // Compile the for [block]
    self->ast_for.f_block,DEE_COMPILER_ARGS_SCOPE_EX(
    self->ast_for.f_scope,(compiler_flags|
    DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE)&~(DEE_COMPILER_FLAG_USED))) != 0) goto err_for;
   if (self->ast_for.f_head_next) {
    if DEE_UNLIKELY(DeeXAst_Compile(// Compile the for [next]
     self->ast_for.f_head_next,DEE_COMPILER_ARGS_SCOPE_EX(
     self->ast_for.f_scope,(compiler_flags|
     DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE)&~(
     DEE_COMPILER_FLAG_USED))) != 0) goto err_for;
   }
   // Cleanup the block scope
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(writer,self->ast_for.f_scope,lexer,compiler_flags) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(writer,self->ast_for.f_head_cond_scope,lexer,compiler_flags) != 0) return -1;
   // Jump back to 'cont'
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSSizeArg(writer,OP_JUMP,
    -(Dee_ssize_t)(DeeCodeWriter_ADDR(writer)-for_cond_pos)) != 0) {
err_for: DeeCodeWriter_FAIL_LOOP(writer); return -1;
   }
   DeeCodeWriter_POP_LOOP(writer,DeeCodeWriter_ADDR(writer),{return -1;});
   DEE_ASSERT(DeeObject_Check(self->ast_for.f_head_cond) &&
              DeeXAst_Check(self->ast_for.f_head_cond));
   /*if (self->ast_for.f_head_cond)*/ {
    // This is where we jump if 'done' is true
    if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,for_end_jmparg,(
     Dee_size_t)(DeeCodeWriter_ADDR(writer)-for_end_origin)) != 0) return -1;
   }
   // Cleanup the head scope
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(
    writer,self->ast_for.f_head_init_scope,lexer,compiler_flags) != 0) return -1;
   goto push_none;
  } break;

  case DEE_SASTKIND_FORIN:
  case DEE_SASTKIND_FOREACH: {
   Dee_size_t forin_pos,forin_jmparg;
   // Compile+Push the list expression to loop
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_forin.fi_seq,DEE_COMPILER_ARGS_SCOPE_EX(
    self->ast_forin.fi_seq_scope,compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   if (self->ast_kind != DEE_SASTKIND_FOREACH) {
    // Create an iterator for the list
    if DEE_UNLIKELY(DeeCodeWriter_UnaryOp(writer,OP_SEQ_ITER_SELF) != 0) return -1;
   }
   // Setup the jump and iter_next operator
   forin_pos = DeeCodeWriter_ADDR(writer);
   DeeCodeWriter_PUSH_LOOP(writer,forin_pos);
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(
    writer,OP_SEQ_ITER_WALK,&forin_jmparg) != 0) goto err_for_in;
   // If we don't jump, we iterate the block
   DeeCodeWriter_INCSTACK(writer); // notify of the existence of the pushed value
   // Store the iterator value in the given local variable
   if DEE_UNLIKELY(DeeXAst_CompileStore(&self->ast_forin.fi_var,DEE_COMPILER_ARGS_SCOPE_EX(
    self->ast_forin.fi_scope,(compiler_flags|DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE
    )&~(DEE_COMPILER_FLAG_USED))) != 0) goto err_for_in;
   // Now lets execute the block
   if DEE_UNLIKELY(DeeSAst_Compile(self->ast_forin.fi_block,DEE_COMPILER_ARGS_SCOPE_EX(
    self->ast_forin.fi_scope,(compiler_flags|DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE
    )&~(DEE_COMPILER_FLAG_USED))) != 0) goto err_for_in;
   // Cleanup the for scope
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(
    writer,self->ast_forin.fi_scope,lexer,compiler_flags) != 0) return -1;
   // Jump back to collect the next iterator value
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSSizeArg(writer,OP_JUMP,
    -(Dee_ssize_t)(DeeCodeWriter_ADDR(writer)-forin_pos)) != 0) {
err_for_in:
    DeeCodeWriter_FAIL_LOOP(writer);
    return -1;
   }
   // This is where the jump leads if the for-in iterator is done (after the block jump)
   DeeCodeWriter_POP_LOOP(writer,DeeCodeWriter_ADDR(writer),{return -1;});
   if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,forin_jmparg,
    (Dee_size_t)(DeeCodeWriter_ADDR(writer)-forin_pos)) != 0) return -1;
   // Cleanup the sequence scope
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(writer,self->ast_forin.fi_seq_scope,lexer,compiler_flags) != 0) return -1;
   // Finally pop the finished iterator from the stack (if we aren't supposed to return a value)
   if (!ret_used && DEE_UNLIKELY(DeeCodeWriter_Pop(writer) != 0)) return -1;
  } break;

  case DEE_SASTKIND_WHILE: {
   Dee_size_t cond_pos,end_jmparg;
   Dee_size_t block_pos;
   // Setup the jump back to the condition check
   // NOTE: This is where 'continue' jump to!
   cond_pos = DeeCodeWriter_ADDR(writer);
   DeeCodeWriter_PUSH_LOOP(writer,cond_pos);
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_while.w_cond,DEE_COMPILER_ARGS_SCOPE_EX(
    self->ast_while.w_cond_scope,compiler_flags|(DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE|DEE_COMPILER_FLAG_USED))) != 0)  goto err_while;
   // Skip the block if the condition fails
   block_pos = DeeCodeWriter_ADDR(writer);
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(writer,
    OP_JUMP_IF_FF_POP,&end_jmparg) != 0) goto err_while;
   DeeCodeWriter_DECSTACK(writer); // 'OP_JUMP_IF_FF_POP' popped this one
   // Compile the block
   if DEE_UNLIKELY(DeeSAst_Compile(self->ast_while.w_block,DEE_COMPILER_ARGS_SCOPE_EX(
    self->ast_while.w_scope,(compiler_flags|DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE)&~(DEE_COMPILER_FLAG_USED))) != 0) goto err_while;
   // Cleanup the loop scope
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(
    writer,self->ast_while.w_scope,lexer,compiler_flags) != 0) return -1;
   // Jump back to the condition
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSSizeArg(writer,OP_JUMP,
    -(Dee_ssize_t)(DeeCodeWriter_ADDR(writer)-cond_pos)) != 0) {
err_while:
    DeeCodeWriter_FAIL_LOOP(writer);
    return -1;
   }
   // This is where we jump if the condition fails or break is encountered
   DeeCodeWriter_POP_LOOP(writer,DeeCodeWriter_ADDR(writer),{return -1;});
   if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,end_jmparg,
    (Dee_size_t)(DeeCodeWriter_ADDR(writer)-block_pos)) != 0) return -1;
   // Cleanup the condition scope
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(
    writer,self->ast_while.w_cond_scope,lexer,compiler_flags) != 0) return -1;
   goto push_none;
  } break;

  case DEE_SASTKIND_DOWHILE: {
   Dee_size_t block_pos;
   // Setup the jump back to the condition checks out
   block_pos = DeeCodeWriter_ADDR(writer);
   DeeCodeWriter_PUSH_LOOP(writer,block_pos);
   // Compile the block
   if DEE_UNLIKELY(DeeSAst_Compile(self->ast_dowhile.w_block,DEE_COMPILER_ARGS_SCOPE_EX(
    self->ast_dowhile.w_scope,(compiler_flags|DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE)&~(DEE_COMPILER_FLAG_USED))) != 0) goto err_do_while;
   // Cleanup the loop scope
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(
    writer,self->ast_dowhile.w_scope,lexer,compiler_flags) != 0) return -1;
   // Cleanup the condition
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_dowhile.w_cond,DEE_COMPILER_ARGS_SCOPE_EX(
    self->ast_dowhile.w_cond_scope,compiler_flags|(
    DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE|DEE_COMPILER_FLAG_USED))) != 0) goto err_do_while;
   // Only jump back if the condition prevails
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSSizeArg(writer,OP_JUMP_IF_TT_POP,
    -(Dee_ssize_t)(DeeCodeWriter_ADDR(writer)-block_pos)) != 0) {
err_do_while: DeeCodeWriter_FAIL_LOOP(writer); return -1;
   }
   DeeCodeWriter_DECSTACK(writer); // 'OP_JUMP_IF_TT_POP' popped this one
   // This is where we end up if the condition fails
   DeeCodeWriter_POP_LOOP(writer,DeeCodeWriter_ADDR(writer),{return -1;});
   // Cleanup the condition scope
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(
    writer,self->ast_dowhile.w_cond_scope,lexer,compiler_flags) != 0) return -1;
   goto push_none;
  } break;

  case DEE_SASTKIND_LOOPEVER: {
   Dee_size_t block_pos;
   // Setup the jump back to the condition checks out
   block_pos = DeeCodeWriter_ADDR(writer);
   DeeCodeWriter_PUSH_LOOP(writer,block_pos);
   // Compile the block
   if DEE_UNLIKELY(DeeSAst_Compile(self->ast_loopever.sl_block,DEE_COMPILER_ARGS_SCOPE_EX(
    self->ast_loopever.sl_scope,(compiler_flags&~(DEE_COMPILER_FLAG_USED))|
    DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE)) != 0) { DeeCodeWriter_FAIL_LOOP(writer); return -1; }
   // Cleanup the loop scope
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(writer,self->ast_loopever.sl_scope,lexer,compiler_flags) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_JmpAbs(writer,block_pos) != 0) return -1;
   DeeCodeWriter_POP_LOOP(writer,DeeCodeWriter_ADDR(writer),{return -1;});
   goto push_none;
  } break;

  case DEE_SASTKIND_LOOPONCE: {
   Dee_size_t block_pos;
   // Setup the jump back to the condition checks out
   block_pos = DeeCodeWriter_ADDR(writer);
   DeeCodeWriter_PUSH_LOOP(writer,block_pos);
   // Compile the block
   if DEE_UNLIKELY(DeeSAst_Compile(self->ast_looponce.sl_block,DEE_COMPILER_ARGS_SCOPE_EX(
    self->ast_looponce.sl_scope,(compiler_flags&~(DEE_COMPILER_FLAG_USED))|
    DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE)) != 0) { DeeCodeWriter_FAIL_LOOP(writer); return -1; }
   // Cleanup the loop scope
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(writer,self->ast_looponce.sl_scope,lexer,compiler_flags) != 0) return -1;
   DeeCodeWriter_POP_LOOP(writer,DeeCodeWriter_ADDR(writer),{return -1;});
   goto push_none;
  } break;

  case DEE_SASTKIND_LOOPNONE: {
   Dee_size_t block_pos;
   Dee_size_t jmp_start,dead_jump;
   jmp_start = DeeCodeWriter_ADDR(writer);
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(writer,OP_JUMP,&dead_jump) != 0) return -1;
   block_pos = DeeCodeWriter_ADDR(writer);
   DeeCodeWriter_PUSH_LOOP(writer,block_pos);
   if DEE_UNLIKELY(DeeSAst_Compile(self->ast_loopnone.sl_block,DEE_COMPILER_ARGS_SCOPE_EX(
    self->ast_loopnone.sl_scope,(compiler_flags&~(DEE_COMPILER_FLAG_USED))|
    DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE)) != 0) {
err_loopnone_failloop: DeeCodeWriter_FAIL_LOOP(writer); return -1;
   }
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(writer,self->ast_loopnone.sl_scope,lexer,compiler_flags) != 0) goto err_loopnone_failloop;
   if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,dead_jump,
    (Dee_size_t)(DeeCodeWriter_ADDR(writer)-jmp_start)) != 0) return -1;
   DeeCodeWriter_POP_LOOP(writer,DeeCodeWriter_ADDR(writer),{return -1;});
   goto push_none;
  } break;

  { // Break/Continue
   if (0) {case DEE_SASTKIND_CONTINUE: if DEE_UNLIKELY(DeeCodeWriter_Continue(writer) != 0) return -1; }
   if (0) {case DEE_SASTKIND_BREAK: if DEE_UNLIKELY(DeeCodeWriter_Break(writer) != 0) return -1; }
   DEE_ASSERTF((compiler_flags&DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE)!=0,
               "break/continue isn't actually allowed");
   goto on_unreachable;
  } break;

  case DEE_SASTKIND_GOTO: {
   DEE_ASSERT(self->ast_goto.g_label);
   if DEE_UNLIKELY(DeeCodeWriter_GotoLabel(writer,
    &self->ast_goto.g_label->pl_code_label,
    lexer,self->ast_common.ast_token) != 0) return -1;
   goto on_unreachable;
  } break;

  case DEE_SASTKIND_THROW: {
   if (self->ast_throw.t_object) {
    if DEE_UNLIKELY(DeeXAst_Compile(self->ast_throw.t_object,DEE_COMPILER_ARGS_EX(
     compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
    if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_THROW) != 0) return -1;
    DeeCodeWriter_DECSTACK(writer);
   } else {
    if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_RETHROW) != 0) return -1;
   }
   goto on_unreachable;
  } break;

  case DEE_SASTKIND_TRY: {
   Dee_size_t pos1,noexcept_jmparg;
   Dee_size_t noexcept_pos;
   struct DeeSAstTryHandler *begin,*end;
   DEE_ASSERT(self->ast_try.t_handlerc != 0);
   end = (begin = self->ast_try.t_handlerv)+self->ast_try.t_handlerc;
   // Setup all the handler blocks (in reverse order)
   do { --end;
    if DEE_UNLIKELY(DeeCodeWriter_TryBegin(writer,&end->th_runtime_id) != 0) return -1;
   } while (end != begin);
   end = begin+self->ast_try.t_handlerc;
   // Compile the protected code
   if DEE_UNLIKELY(DeeSAst_Compile(self->ast_try.t_block,DEE_COMPILER_ARGS_SCOPE_EX(
    self->ast_try.t_scope,compiler_flags&~(DEE_COMPILER_FLAG_USED))) != 0) return -1;
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(
    writer,self->ast_try.t_scope,lexer,compiler_flags) != 0) return -1;
   // End all the exception handler blocks
   begin = self->ast_try.t_handlerv;
   DEE_ASSERT(begin != end);
   while (begin++ != end) if DEE_UNLIKELY(DeeCodeWriter_TryEnd(writer) != 0) return -1;
   // Setup the jump across the handler code blocks
   pos1 = DeeCodeWriter_ADDR(writer);
   if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(writer,OP_JUMP,&noexcept_jmparg) != 0) return -1;
   begin = self->ast_try.t_handlerv;
   DEE_ASSERT(begin != end);
   while (1) {
    // Initialize the exception headers + compile the handler code
    int is_finally,runtime_exctype; Dee_size_t n_other_handlers;
    struct DeeExceptionHandlerEntry *rt_entry;
    rt_entry = writer->cw_exceptv+begin->th_runtime_id;
    if (begin->th_type) {
     if (begin->th_type->ast_kind == DEE_XASTKIND_CONST) {
      Dee_XINCREF(rt_entry->e_type = (DeeTypeObject *)begin->th_type->ast_const.c_const);
      rt_entry->e_kind = begin->th_kind;
      runtime_exctype = 0;
     } else {
      rt_entry->e_kind = (begin->th_kind&~(
       DeeExceptionHandleKind_FLAG_TYPED))|DeeExceptionHandleKind_FLAG_ALL;
      rt_entry->e_type = NULL;
      runtime_exctype = 1;
     }
    } else {
     rt_entry->e_kind = begin->th_kind;
     rt_entry->e_type = NULL;
     runtime_exctype = 0;
    }
    switch (begin->th_local.vds_kind) {
     case DEE_VARDECLSTORAGEKIND_NONE:
      DEE_ASSERTF((rt_entry->e_kind&DeeExceptionHandleKind_FLAG_VAR)==0,
                  "Exception handler with variable has an empty variable storage");
      rt_entry->e_store = 0;
      break;
     case DEE_VARDECLSTORAGEKIND_VAR: {
      DeeLocalVarObject *exception_storage;
      // Local variable to store the exception within
      exception_storage = begin->th_local.vds_var.vs_var;
      if (DeeLocalVar_KIND(exception_storage) == DEE_LOCALVAR_KIND_LOCAL) {
       if ((exception_storage->lv_flags&DEE_LOCALVAR_FLAG_INITIALIZED) == 0) {
        if DEE_UNLIKELY((exception_storage->lv_loc_id = DeeCodeWriter_AllocLocal(
         writer,have_local_names ? TPPLexer_TokenIDStr(DeeLexer_LEXER(
         lexer),exception_storage->lv_name) : NULL)) == (Dee_size_t)-1) return -1;
        exception_storage->lv_flags |= DEE_LOCALVAR_FLAG_INITIALIZED;
       }
       rt_entry->e_store = exception_storage->lv_loc_id;
      } else {
       if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_LOCAL_FOR_EXCEPTION_STORAGE,
        (DeeObject *)lexer,(DeeObject *)begin->th_local.vds_var.vs_token,
        "Expected local variable for exception storage, but got a %s variable instead",
        DeeLocalVarKind_Name(DeeLocalVar_KIND(exception_storage))) != 0) return -1;
       rt_entry->e_store = 0;
       rt_entry->e_kind &= ~DeeExceptionHandleKind_FLAG_VAR;
      }
     } break;
     // TODO: DEE_VARDECLSTORAGEKIND_ATTR
     // TODO: DEE_VARDECLSTORAGEKIND_ATTR_C
     // TODO: DEE_VARDECLSTORAGEKIND_ITEM
     // TODO: DEE_VARDECLSTORAGEKIND_RANGE
     // TODO: DEE_VARDECLSTORAGEKIND_EXPAND
     // TODO: DEE_VARDECLSTORAGEKIND_ASSIGN
     default: rt_entry->e_kind &= ~DeeExceptionHandleKind_FLAG_VAR; break; 
    }
    if (runtime_exctype &&
       (rt_entry->e_kind&DeeExceptionHandleKind_FLAG_VAR)==0) {
     // Must create a hidden local variable for the exception storage
     // >> Required to compare the actual exception against
     if ((rt_entry->e_store = DeeCodeWriter_AllocLocal(writer,NULL)) == (Dee_size_t)-1) return -1;
     rt_entry->e_kind |= DeeExceptionHandleKind_FLAG_VAR;
    }

    // Align the stack to the current stack size
    rt_entry->e_stack = writer->cw_stack_size;
    rt_entry->e_handler = DeeCodeWriter_ADDR(writer);
    // Header is now initialized
    // Update finally recursion of the entire code
    is_finally = begin->th_kind == DeeExceptionHandleKind_FINALLY;
    // >> Compile the actual handler code
    if (is_finally) DeeCodeWriter_INC_FINALLYSIZE(writer);

    if (runtime_exctype) {
     // Check that the thrown exception matches what was specified
     // >> try {
     // >>   ...
     // >> } catch (get_error()) {
     // >>   print "Error!";
     // >> }
     //Compile as:
     // >> try {
     // >>   ...
     // >> } catch (__hidden...) {
     // >>   if (__hidden !is get_error()) throw;
     // >>   print "Error!";
     // >> }
//#define MASK_NOEXCEPT
     Dee_size_t rethrow_jmpaddr,rethrow_jmparg;
#ifndef MASK_NOEXCEPT
     Dee_size_t rethrow_catch_id,rethrow_finally_id;
     if DEE_UNLIKELY(DeeCodeWriter_TryBegin(writer,&rethrow_catch_id) != 0) return -1;;
#endif

     rt_entry = writer->cw_exceptv+begin->th_runtime_id;
     DEE_ASSERT(!is_finally);
     DEE_ASSERT((rt_entry->e_kind&DeeExceptionHandleKind_FLAG_VAR)!=0);
     if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(writer,OP_LOAD_LOC,rt_entry->e_store) != 0) return -1;
     DeeCodeWriter_INCSTACK(writer);
     // TODO: If an error occurs in the mask expression,
     //       we should rethrow the original error as well:
     //       >> try {
     //       >>   ...
     //       >> } catch (e...) {
     //       >>   __stack local mask = none;
     //       >>   try {
     //       >>     mask = get_mask();
     //       >>   } catch (e2...) {
     //       >>     // The original error should be below
     //       >>     // the new one in the exception stack.
     //       >>     try {
     //       >>       throw e;
     //       >>     } finally {
     //       >>       throw e2;
     //       >>     }
     //       >>   }
     //       >>   if (e !is mask) throw e;
     //       >>   ...
     //       >> }
     if DEE_UNLIKELY(DeeXAst_Compile(begin->th_type,DEE_COMPILER_ARGS_EX(
      compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_BinaryOp(writer,OP_IS) != 0) return -1;
     rethrow_jmpaddr = DeeCodeWriter_ADDR(writer);
     if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(
      writer,OP_JUMP_IF_TT_POP,&rethrow_jmparg) != 0) return -1;
     DeeCodeWriter_DECSTACK(writer);
#ifdef MASK_NOEXCEPT
     if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_EXCEPT_END) != 0) return -1;
#endif
     if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithSizeArg(
      writer,OP_LOAD_LOC,rt_entry->e_store) != 0) return -1;
     //DeeCodeWriter_INCSTACK(writer); // Not really required...
     if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_THROW) != 0) return -1;
     //DeeCodeWriter_DECSTACK(writer);
#ifndef MASK_NOEXCEPT
     rt_entry = writer->cw_exceptv+rethrow_catch_id;
     DeeExceptionHandlerEntry_InitCatchAll(rt_entry,writer);
     DeeCodeWriter_INC_FINALLYSIZE(writer);
     // TODO: Optimize: This can be a non-checking opcode
     if DEE_UNLIKELY(DeeCodeWriter_TryBegin(writer,&rethrow_finally_id) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_RETHROW) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_TryEnd(writer) != 0) return -1;
     rt_entry = writer->cw_exceptv+rethrow_finally_id;
     DeeExceptionHandlerEntry_InitFinally(rt_entry,writer);
     if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_EXCEPT_END) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_EXCEPT_END) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_FINALLY_END) != 0) return -1;
     DeeCodeWriter_DEC_FINALLYSIZE(writer);
#endif
     if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,rethrow_jmparg,
      DeeCodeWriter_ADDR(writer)-rethrow_jmpaddr) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_TryEnd(writer) != 0) return -1;
    }

    // Drop all other exception handlers, so they're not active while out's is being executed
    n_other_handlers = (Dee_size_t)((end-begin)-1);
    while (n_other_handlers--) if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_TRY_END) != 0) return -1;

    // Optimization for skipping the additional try...finally block in the exception handler
    // NOTE: We can only ever do this, if the code __ALWAYS__ returns normally!
    if (DEE_AST_ATTRIBUTE_IS_ALWAYSRETURN(DeeSAst_Attr(begin->th_block,(Dee_uint32_t)(
        DEE_AST_ATTRIBUTE_FLAG_NORETURN_REQUIRE_NOEXCEPT|
        DEE_AST_ATTRIBUTE_FLAG_ALL_EXCEPTIONS|( // We need to known for sure!
        (compiler_flags&DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE)!=0
        ? DEE_AST_ATTRIBUTE_FLAG_NORETURN_ALLOW_BREAK : DEE_AST_ATTRIBUTE_FLAG_NONE))))) {
     // If the handler is noexcept and always returns, we can skip the additional finally-block protecting it
     if DEE_UNLIKELY(DeeSAst_Compile(begin->th_block,DEE_COMPILER_ARGS_SCOPE_EX(
      begin->th_scope,compiler_flags&~(DEE_COMPILER_FLAG_USED))) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(writer,begin->th_scope,lexer,compiler_flags) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,(Dee_uint8_t)(is_finally ? OP_FINALLY_END : OP_EXCEPT_END)) != 0) return -1;
    } else {
     Dee_size_t handler_finally_id;
     Dee_size_t handler_finally_jmp;
     // This is a bit complex, but we need a finally block to
     // cleanup our handler (even if he's a finally block himself...)
     DeeCodeWriter_INC_FINALLYSIZE(writer);
     if DEE_UNLIKELY(DeeCodeWriter_TryBegin(writer,&handler_finally_id) != 0) return -1;;
     // We need a small finally block across the catch handler, so we
     // can make sure to always remove the exception from the list of handled exceptions
     if DEE_UNLIKELY(DeeSAst_Compile(begin->th_block,DEE_COMPILER_ARGS_SCOPE_EX(
      begin->th_scope,compiler_flags&~(DEE_COMPILER_FLAG_USED))) != 0) return -1;
     if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(writer,begin->th_scope,lexer,compiler_flags) != 0) return -1;
     // End the try block and initialize the finally block at the end of the catch
     if DEE_UNLIKELY(DeeCodeWriter_TryEnd(writer) != 0) return -1;
     // Skip the finally block once try_end returned
     if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(writer,OP_JUMP,&handler_finally_jmp) != 0) return -1;
     rt_entry = writer->cw_exceptv+handler_finally_id;
     DeeExceptionHandlerEntry_InitFinally(rt_entry,writer);
     if (is_finally) {
      if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_FINALLY_END) != 0) return -1;
      if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,handler_finally_jmp,(Dee_size_t)(
       DeeCodeWriter_ADDR(writer)-(handler_finally_jmp-1))) != 0) return -1;
      if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_FINALLY_END) != 0) return -1;
     } else {
      if DEE_UNLIKELY(DeeCodeWriter_WriteOp(writer,OP_EXCEPT_END) != 0 ||
                      DeeCodeWriter_WriteOp(writer,OP_FINALLY_END) != 0) return -1;
      if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,handler_finally_jmp,(Dee_size_t)(
       DeeCodeWriter_ADDR(writer)-(handler_finally_jmp-1))) != 0) return -1;
     }
     DeeCodeWriter_DEC_FINALLYSIZE(writer);
    }
    if (is_finally) DeeCodeWriter_DEC_FINALLYSIZE(writer);
    if (++begin == end) break; else {
     // Jump to the end of all handlers
     if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(
      writer,OP_JUMP,&begin[-1].th_runtime_jmp) != 0) return -1;
    }
   }
   // This is where we jump to if no exception occurred
   noexcept_pos = DeeCodeWriter_ADDR(writer);
   if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,noexcept_jmparg,(
    Dee_size_t)(noexcept_pos-pos1)) != 0) return -1;
   begin = self->ast_try.t_handlerv;
   // Initialize all the jumps from the exception handlers to the end
   if (begin != end) {
    --end;
    while (begin != end) {
     // Calculate the jumps from the end of the handlers
     if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,begin->th_runtime_jmp,(
      Dee_size_t)(noexcept_pos-(begin->th_runtime_jmp-1))) != 0) return -1;
     ++begin;
    }
   }
   // Finalize the root scope of the try-block
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(writer,
    self->ast_try.t_rootscope,lexer,compiler_flags) != 0) return -1;
   goto push_none;
  } break;

  case DEE_SASTKIND_MODULE: {
   if (self->ast_module.m_block->ast_kind == DEE_SASTKIND_BLOCK) {
    // Compile with local cleanup
    DEE_ASSERT(self->ast_module.m_block->ast_block.b_scope == self->ast_module.m_module);
    if DEE_UNLIKELY(DeeSAst_CompileBlockWithoutLocalCleanup(
     self->ast_module.m_block->ast_block.b_astc,
     self->ast_module.m_block->ast_block.b_astv,
     DEE_COMPILER_ARGS_SCOPE(self->ast_module.m_module)) != 0) return -1;
   } else if DEE_UNLIKELY(DeeSAst_Compile(self->ast_module.m_block,
    DEE_COMPILER_ARGS_SCOPE(self->ast_module.m_module)) != 0) return -1;
   goto push_none;
  } break;

  case DEE_SASTKIND_SWITCH: {
   struct DeeCodeLabel *default_label;
   struct DeeParserLabel *case_iter;
   struct DeeParserLabelList *old_cases;
   DeeScopeObject *weak_root;
   Dee_size_t pos1;
   DEE_ASSERT(self->ast_switch.sw_scope->sc_weak_prev == self->ast_switch.sw_expr_scope);
   DEE_ASSERT(DeeScope_SameWeakScope((DeeObject *)scope,(DeeObject *)self->ast_switch.sw_scope));
   pos1 = DeeCodeWriter_ADDR(writer);
   // Compile the switch expression
   if DEE_UNLIKELY(DeeXAst_Compile(self->ast_switch.sw_expr,DEE_COMPILER_ARGS_SCOPE_EX(
    self->ast_switch.sw_expr_scope,compiler_flags|DEE_COMPILER_FLAG_USED)) != 0) return -1;
   default_label = NULL;
   case_iter = self->ast_switch.sw_cases.ll_front;
   // Go through all the labels and compile the forward jumps into the switch block
   while (case_iter) {
    DEE_ASSERTF(case_iter->pl_kind != DEE_PARSERLABEL_KIND_LABEL,
                "Regular label in switch label list (how did this happen?)");
    switch (case_iter->pl_kind) {
     case DEE_PARSERLABEL_KIND_C_DEFAULT:
      DEE_ASSERTF(!default_label,"Default label already set");
      default_label = &case_iter->pl_code_label;
      break;
     case DEE_PARSERLABEL_KIND_C_CASE:
      if DEE_UNLIKELY(DeeCodeWriter_PushConst(writer,case_iter->pl_c_case.clc_case,compiler_flags) != 0) return -1;
      DeeCodeWriter_DECSTACK(writer); // Consumed by OP_JUMP_IF_CASE_COMPARE
      if DEE_UNLIKELY(_DeeCodeWriter_GotoForwardLabelEx(writer,&case_iter->pl_code_label,
       OP_JUMP_IF_CASE_COMPARE,self->ast_common.ast_token) != 0) return -1;
      break;
     case DEE_PARSERLABEL_KIND_C_RANGE:
      if DEE_UNLIKELY(DeeCodeWriter_PushConst(writer,case_iter->pl_c_range.clr_begin,compiler_flags) != 0) return -1;
      if DEE_UNLIKELY(DeeCodeWriter_PushConst(writer,case_iter->pl_c_range.clr_end,compiler_flags) != 0) return -1;
      DeeCodeWriter_DECSTACK_N(writer,2); // Consumed by OP_JUMP_IF_RCASE_COMPARE
      if DEE_UNLIKELY(_DeeCodeWriter_GotoForwardLabelEx(writer,&case_iter->pl_code_label,
       OP_JUMP_IF_RCASE_COMPARE,self->ast_common.ast_token) != 0) return -1;
      break;
     default: DEE_BUILTIN_UNREACHABLE();
    }
    case_iter = case_iter->pl_next;
   }
   weak_root = DeeScope_WEAK_ROOT(scope);
   DeeCodeWriter_PUSH_LOOP(writer,pos1);
   old_cases = weak_root->sc_cases;
   weak_root->sc_cases = &self->ast_switch.sw_cases;
   // Compile the fallback label
   if (default_label) { // User-defined default label
    if DEE_UNLIKELY(DeeCodeWriter_GotoLabel(
     writer,default_label,lexer,self->ast_common.ast_token) != 0) {
err_switch_loop:
     DeeCodeWriter_FAIL_LOOP(writer);
     weak_root->sc_cases = old_cases;
     return -1;
    }
   } else { // Compiler-generated default label (jump over the block)
    if DEE_UNLIKELY(DeeCodeWriter_Break(writer) != 0) goto err_switch_loop;
   }
   if DEE_UNLIKELY(DeeSAst_Compile(self->ast_switch.sw_block,
    DEE_COMPILER_ARGS_SCOPE_EX(self->ast_switch.sw_scope,(compiler_flags|
    DEE_COMPILER_FLAG_ALLOW_BREAK_CONTINUE)&~(DEE_COMPILER_FLAG_USED))) != 0) goto err_switch_loop;
   // Cleanup the switch scope
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(
    writer,self->ast_switch.sw_scope,lexer,compiler_flags) != 0) return -1;
   DeeCodeWriter_POP_LOOP(writer,DeeCodeWriter_ADDR(writer),{return -1;});
   weak_root->sc_cases = old_cases;
   // Cleanup the all labels
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeLabels(
    writer,&self->ast_switch.sw_cases,lexer) != 0) return -1;
   // Cleanup the switch expression scope
   if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(
    writer,self->ast_switch.sw_expr_scope,lexer,compiler_flags) != 0) return -1;
   // Pop the switch condition expression
   if (!ret_used && DEE_UNLIKELY(DeeCodeWriter_Pop(writer) != 0)) return -1;
  } break;

  {
   DeeSAstObject *live_block,*dead_block;
   DeeScopeObject *live_scope,*dead_scope;
   Dee_size_t dead_start,dead_jump,dead_end,live_jump;
   if (0) {case DEE_SASTKIND_IFFALSE:
    live_block = self->ast_iffalse.ic_fail_block;
    live_scope = self->ast_iffalse.ic_fail_scope;
    dead_block = self->ast_iffalse.ic_succ_block;
    dead_scope = self->ast_iffalse.ic_succ_scope;
   }
   if (0) {case DEE_SASTKIND_IFTRUE:
    live_block = self->ast_iftrue.ic_succ_block;
    live_scope = self->ast_iftrue.ic_succ_scope;
    dead_block = self->ast_iftrue.ic_fail_block;
    dead_scope = self->ast_iftrue.ic_fail_scope;
   }
   if (!dead_block && !live_block) goto push_none;
   if (dead_block) {
    dead_start = DeeCodeWriter_ADDR(writer);
    if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(writer,OP_JUMP,&dead_jump) != 0) return -1;
    if DEE_UNLIKELY(DeeSAst_Compile(dead_block,DEE_COMPILER_ARGS_SCOPE(dead_scope)) != 0) return -1;
    if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(writer,dead_scope,lexer,compiler_flags) != 0) return -1;
    if (live_block || ret_used) {
     // Setup the jump to skip the live block
     dead_end = DeeCodeWriter_ADDR(writer);
     if DEE_UNLIKELY(DeeCodeWriter_WriteOpWithFutureSizeArg(writer,OP_JUMP,&live_jump) != 0) return -1;
    }
    if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,dead_jump,(
     Dee_size_t)(DeeCodeWriter_ADDR(writer)-dead_start)) != 0) return -1;
   }
   if (live_block) {
    if DEE_UNLIKELY(DeeSAst_Compile(live_block,DEE_COMPILER_ARGS_SCOPE(live_scope)) != 0) return -1;
    if DEE_UNLIKELY(DeeCodeWriter_FinalizeWeakScope(writer,live_scope,lexer,compiler_flags) != 0) return -1;
    if (dead_block) {
     // This is where the dead block jumps when skipping the live block
     if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,live_jump,(
      Dee_size_t)(DeeCodeWriter_ADDR(writer)-dead_end)) != 0) return -1;
     if (ret_used) DeeCodeWriter_DECSTACK(writer); // Only one branch is on the stack
    }
   } else if (ret_used) {
    if (dead_block) {
     if DEE_UNLIKELY(DeeCodeWriter_SetFutureSizeArg(writer,live_jump,(
      Dee_size_t)(DeeCodeWriter_ADDR(writer)-dead_end)) != 0) return -1;
     if (ret_used) DeeCodeWriter_DECSTACK(writer); // Only one branch is on the stack
    }
    goto push_none;
   }
  } break;

  default: // Shouldn't happen
   DEE_LVERBOSE1("Compiling unknown SAst: %r\n",self);
  case DEE_SASTKIND_EMPTY:push_none:
   if (ret_used && DEE_UNLIKELY(DeeCodeWriter_PushNone(writer) != 0)) return -1;
   break;
 }
 return 0;
}
DEE_COMPILER_MSVC_WARNING_POP


DEE_DECL_END

#endif /* !GUARD_DEEMON_SAST_COMPILE_C */
