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
#ifndef GUARD_DEEMON_COMPILER_LEXER_C
#define GUARD_DEEMON_COMPILER_LEXER_C 1
#define DEE_LIMITED_API 1

// include/*
#include <deemon/__conf.inl>
#include <deemon/compiler/code.h>
#include <deemon/compiler/compiler.h>
#include <deemon/compiler/lexer.h>
#include <deemon/deemonrun.h>
#include <deemon/optional/object_pool.h>
#include <deemon/optional/object_visit.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/float.h>
#include <deemon/fs_api.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/runtime/function.h>
#include <deemon/string.h>

// src/*
#include <deemon/__xconf.inl>
#include <deemon/compiler/__opcode.inl>
#include <deemon/compiler/sast.h>
#include <deemon/compiler/scope.h>
#include <deemon/compiler/xast.h>
#include <deemon/unicode/char_traits.inl>
#include DEE_INCLUDE_MEMORY_API()

#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <float.h>
#ifdef DEE_PLATFORM_WINDOWS
#include <Windows.h>
#endif
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()

// */ (nano...)

DEE_DECL_BEGIN
// Setup our callbacks to tpp
#define TPP_CALLBACK_NO_MEMORY      DeeError_NoMemory
#define TPP_CALLBACK_PARSE_PRAGMA   TPP_CALLBACK_PARSE_PRAGMA
#define TPP_CALLBACK_FATAL_ERROR    TPP_CALLBACK_FATAL_ERROR
#define TPP_CALLBACK_PRAGMA_MESSAGE TPP_CALLBACK_PRAGMA_MESSAGE
#define TPP_CALLBACK_ERROR_MESSAGE  TPP_CALLBACK_ERROR_MESSAGE
DEE_STATIC_INLINE(int) TPP_CALLBACK_PARSE_PRAGMA(TPPLexerObject *self, TPPTokenObject *token, int is_directive);
DEE_STATIC_INLINE(int) TPP_CALLBACK_FATAL_ERROR(TPPLexerObject const *lexer, int code, TPPTokenObject const *token, char const *message);
DEE_STATIC_INLINE(int) TPP_CALLBACK_PRAGMA_MESSAGE(TPPLexerObject const *lexer, char const *p, Dee_size_t s);
DEE_STATIC_INLINE(int) TPP_CALLBACK_ERROR_MESSAGE(TPPLexerObject const *lexer, char const *p);
DEE_DECL_END

// Link tpp source file
DEE_COMPILER_MSVC_WARNING_PUSH(4365 4820)
#define TPP_USER_WARNINGS_FILE_0() <deemon/compiler/__warnings.inl>
#define TPP_CONFIG_EXTENSIONS_ARE_NOT_FEATURES
#include <tpp.c>
DEE_COMPILER_MSVC_WARNING_POP

DEE_DECL_BEGIN
DEE_OBJECTPOOL_DEFINE(token_pool,DeeTokenObject)

Dee_size_t DeeToken_ClearPool(void) {
 return DEE_OBJECTPOOL_CLEAR(token_pool);
}



DEE_A_RET_EXCEPT(-1) int DeeStaticIfStack_Push(
 struct DeeStaticIfStack *self, DeeTokenObject *tok, int val, int brace_rec) {
 struct DeeStaticIfStackEntry *new_list;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(tok) && DeeToken_Check(tok));
 while ((new_list = (struct DeeStaticIfStackEntry *)realloc_nz(
  self->si_v,(self->si_c+1)*sizeof(struct DeeStaticIfStackEntry))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 self->si_v = new_list;
 new_list += self->si_c++;
 new_list->sie_brace_recursion = brace_rec;
 new_list->sie_value = val;
 Dee_INCREF(new_list->sie_tok = tok);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeStaticIfStack_InitCopy(
 struct DeeStaticIfStack *self, struct DeeStaticIfStack const *right) {
 struct DeeStaticIfStackEntry *vcopy,*end,*src;
 DEE_ASSERT(self && right);
 if ((self->si_c = right->si_c) != 0) {
  while ((vcopy = (struct DeeStaticIfStackEntry *)malloc_nz(
   self->si_c*sizeof(struct DeeStaticIfStackEntry))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
  end = (self->si_v = vcopy)+self->si_c;
  src = right->si_v;
  while (vcopy != end) {
   vcopy->sie_brace_recursion = src->sie_brace_recursion;
   vcopy->sie_value = src->sie_value;
   Dee_INCREF(vcopy->sie_tok = src->sie_tok);
   ++vcopy,++src;
  }
 } else self->si_v = NULL;
 return 0;
}
void DeeStaticIfStack_Clear(DEE_A_INOUT struct DeeStaticIfStack *self) {
 struct DeeStaticIfStackEntry *begin,*end;
 DEE_ASSERT(self);
 if ((begin = self->si_v) != NULL) {
  end = begin+self->si_c;
  while (end != begin) DeeStaticIfStackEntry_Quit(--end);
  free_nn(begin);
  self->si_v = NULL;
  self->si_c = 0;
 } else DEE_ASSERT(self->si_c == 0);
}
void DeeStaticIfStack_Quit(DEE_A_IN struct DeeStaticIfStack *self) {
 struct DeeStaticIfStackEntry *begin,*end;
 DEE_ASSERT(self);
 if ((begin = self->si_v) != NULL) {
  end = begin+self->si_c;
  while (end != begin) DeeStaticIfStackEntry_Quit(--end);
  free_nn(begin);
 } else DEE_ASSERT(self->si_c == 0);
}




DEE_A_RET_EXCEPT(-1) int DeeStructPackingStack_InitCopy(
 DEE_A_OUT struct DeeStructPackingStack *self,
 DEE_A_IN struct DeeStructPackingStack const *right) {
 Dee_size_t *new_packv;
 DEE_ASSERT(self && right);
 self->sps_packing = right->sps_packing;
 if ((self->sps_packing_c = right->sps_packing_c) != 0) {
  while ((new_packv = (Dee_size_t *)malloc_nz(
   self->sps_packing_c*sizeof(Dee_size_t))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
  self->sps_packing_v = new_packv;
  memcpy(new_packv,right->sps_packing_v,self->sps_packing_c*sizeof(Dee_size_t));
 } else self->sps_packing_v = NULL;
 return 0;
}
void DeeStructPackingStack_InitMove(
 DEE_A_OUT struct DeeStructPackingStack *self,
 DEE_A_IN struct DeeStructPackingStack *right) {
 DEE_ASSERT(self && right);
 self->sps_packing = right->sps_packing;
 self->sps_packing_c = right->sps_packing_c;
 self->sps_packing_v = right->sps_packing_v;
 right->sps_packing_c = 0;
 right->sps_packing_v = NULL;
}

DEE_A_RET_EXCEPT(-1) int DeeStructPackingStack_Push(
 DEE_A_INOUT struct DeeStructPackingStack *self) {
 Dee_size_t *new_stack;
 DEE_ASSERT(self);
 while ((new_stack = (Dee_size_t *)realloc_nz(
  self->sps_packing_v,(
  self->sps_packing_c+1)*sizeof(Dee_size_t))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 self->sps_packing_v = new_stack;
 new_stack[self->sps_packing_c++] = self->sps_packing;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeStructPackingStack_Pop(
 DEE_A_INOUT struct DeeStructPackingStack *self) {
 Dee_size_t *new_stack,restore_level;
 DEE_ASSERT(self && "Invalid stack");
 DEE_ASSERT(DeeStructPackingStack_SIZE(self) && "Empty stack");
 restore_level = self->sps_packing_v[self->sps_packing_c-1];
 if (self->sps_packing_c == 1) {
  self->sps_packing_c = 0;
  free_nn(self->sps_packing_v);
  self->sps_packing_v = NULL;
 } else {
  while ((new_stack = (Dee_size_t *)realloc_nnz(
    self->sps_packing_v,(
    self->sps_packing_c-1)*sizeof(Dee_size_t))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
  self->sps_packing_v = new_stack;
  --self->sps_packing_c;
 }
 self->sps_packing = restore_level;
 return 0;
}








void DeeFutureTokensList_Quit(DEE_A_IN struct DeeFutureTokensList *self) {
 DeeTokenObject **iter,**end;
 end = (iter = self->ftl_tokenv)+self->ftl_tokenc;
 while (iter != end) Dee_DECREF(*iter++);
 free(self->ftl_tokenv);
}
DEE_A_RET_EXCEPT(-1) int DeeFutureTokensList_InitCopy(
 DEE_A_OUT struct DeeFutureTokensList *self,
 DEE_A_IN struct DeeFutureTokensList const *right) {
 DeeTokenObject **vcopy,**end,**src;
 if ((self->ftl_tokenc = right->ftl_tokenc) != 0) {
  while ((vcopy = (DeeTokenObject **)malloc_nz(
   self->ftl_tokenc*sizeof(DeeTokenObject *))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
  end = (self->ftl_tokenv = vcopy)+self->ftl_tokenc;
  src = right->ftl_tokenv;
  while (vcopy != end) Dee_INCREF(*vcopy++ = *src++);
 } else self->ftl_tokenv = NULL;
 self->ftl_tokena = self->ftl_tokenc;
 return 0;
}
void DeeFutureTokensList_InitMove(
 DEE_A_OUT struct DeeFutureTokensList *self,
 DEE_A_IN struct DeeFutureTokensList *right) {
 self->ftl_tokenc = right->ftl_tokenc;
 self->ftl_tokena = right->ftl_tokena;
 self->ftl_tokenv = right->ftl_tokenv;
 right->ftl_tokenc = 0;
 right->ftl_tokena = 0;
 right->ftl_tokenv = NULL;
}











DEE_STATIC_INLINE(int) TPP_CALLBACK_PARSE_PRAGMA(
 TPPLexerObject *self, TPPTokenObject *token, int is_directive) {
#define _lexer ((DeeLexerObject *)((Dee_uint8_t *)self-Dee_OFFSETOF(DeeLexerObject,l_lexer)))
 TPPInteger temp_int; int err;
 (void)is_directive;
 switch (token->tk_id) {
  case KWD_deemon:
   if (DEE_UNLIKELY(TPPLexer_Next(self,token) != 0) ||
      (err = TPPLexer_TrySkip(self,token,TPP_TOK_COLLON_COLLON)) < 0 ||
      (err != 0 && TPPLexer_Skip(self,token,'.') < 0)) return -1;
   switch (token->tk_id) {

    case KWD_yield_token:
     if (DEE_UNLIKELY(TPPLexer_Next(self,token) != 0) ||
         TPPLexer_Skip(self,token,'(') < 0 ||
         DeeLexer_ReturnTPPToken((DeeObject *)_lexer,token) != 0 ||
         DEE_UNLIKELY(TPPLexer_Next(self,token) != 0) ||
         TPPLexer_Skip(self,token,')') < 0) return -1;
     break;

    case KWD_set_function_name:
     if (DEE_UNLIKELY(TPPLexer_Next(self,token) != 0) ||
         TPPLexer_Skip(self,token,'(') < 0) return -1;
     if (!TPPTokenID_IS_KEYWORD(token->tk_id)) {
      if (TPPWarn_ExpectedKeyword(self,token) != 0) return -1;
     } else {
      _lexer->l_func_name = token->tk_id;
      if (DEE_UNLIKELY(TPPLexer_Next(self,token) != 0)) return -1;
     }
     if (TPPLexer_Skip(self,token,')') < 0) return -1;
     break;

    case KWD_set_max_syntax_recursion:
     if (DEE_UNLIKELY(TPPLexer_Next(self,token) != 0) ||
         TPPLexer_Skip(self,token,'(') < 0 ||
         TPPLexer_EvalConst1(self,token,&temp_int) != 0 ||
         TPPLexer_Skip(self,token,')') < 0) return -1;
     _lexer->l_max_syntax_recursion = (unsigned int)temp_int;
     break;

    case KWD_add_include_path: {
     char *include_path; Dee_size_t include_path_len;
     if (DEE_UNLIKELY(TPPLexer_Next(self,token) != 0) ||
         TPPLexer_Skip(self,token,'(') < 0 ||
         _TPPLexer_ParseMallocedString(self,token,
         &include_path,&include_path_len,')') != 0) return -1;
     if (include_path) {
      int temp; char const *file;
      DeeObject *include_path_ob,*real_include_path_ob,*cwd;
      include_path_ob = DeeString_NewWithLength(include_path_len,include_path);
      free_nn(include_path);
      if (!include_path_ob) return -1;
      if ((file = TPPToken_File(token)) != NULL) {
       cwd = DeeFS_PathHead(file);
      } else cwd = DeeFS_GetCwd();
      if (!cwd) { Dee_DECREF(include_path_ob); return -1; }
      real_include_path_ob = DeeFS_PathAbsObject(include_path_ob,cwd);
      Dee_DECREF(cwd);
      Dee_DECREF(include_path_ob);
      if (!real_include_path_ob) return -1;
      temp = DeeLexer_AddSysIncludePathObject((DeeObject *)_lexer,real_include_path_ob);
      Dee_DECREF(real_include_path_ob);
      if (temp != 0) return -1;
     }
    } break;

    default: break;
   }
   break;

  case KWD_pack:
   if (DEE_UNLIKELY(TPPLexer_Next(self,token) != 0) ||
       TPPLexer_Skip(self,token,'(') < 0) return -1;
   while (1) {
    if (token->tk_id == KWD_push) {
     if (DeeStructPackingStack_Push(&_lexer->l_pack) != 0) return -1;
pack_next: if (DEE_UNLIKELY(TPPLexer_Next(self,token) != 0)) return -1;
    } else if (token->tk_id == KWD_pop) {
     if (!DeeStructPackingStack_SIZE(&_lexer->l_pack)) {
      if (DeeError_TPPCompilerError(DEE_WARNING_PRAGMA_PACK_CANT_POP_PACK_LEVEL,
       self,token,"Can't pop pack level (missing push)") != 0) return -1;
     } else if (DeeStructPackingStack_Pop(&_lexer->l_pack) != 0) return -1;
     goto pack_next;
    } else {
     if (TPPLexer_EvalConst1(self,token,&temp_int) != 0) return -1;
     if ((temp_int&(temp_int-1)) != 0) { // Not a 2**n number
      if (DeeError_TPPCompilerErrorf(DEE_WARNING_PRAGMA_PACK_EXPECTED_TWO_POW_N_INTEGRAL_CONSTANT,
       self,token,"Expected power-of-two integral expression (got: " TPPInteger_PRINTF_FMT ")",
       temp_int) != 0) return -1;
     }
     DeeStructPackingStack_SET(&_lexer->l_pack,(Dee_size_t)temp_int);
    }
    if (token->tk_id != ',') break;
    if (DEE_UNLIKELY(TPPLexer_Next(self,token) != 0)) return -1;
   }
   if (TPPLexer_Skip(self,token,')') != 0) return -1;
   break;

  default: return 1;
 }
 return 0;
}

DEE_STATIC_INLINE(int) TPP_CALLBACK_FATAL_ERROR(
 TPPLexerObject const *self, int code, TPPTokenObject const *token, char const *message) {
 DeeObject *error;
 error = DeeType_NewInstancef(&DeeErrorType_CompilerError,"%K",
                              DeeString_Newf("%s(%d) : %c%.4d : %K : %s",
                              TPPToken_File(token),TPPToken_Line(token)+1,
                              code >= TPPWarnings_COUNT_WITH_RESERVED ? 'C' : 'P',code,
                              DeeToken_StrTPPToken(token),message));
 if DEE_UNLIKELY(!error) return -1;
 Dee_XDECREF(_lexer->l_error_last);
 _lexer->l_error_last = error;
 if DEE_UNLIKELY(++_lexer->l_error_cur >= _lexer->l_error_max) {
  DeeError_Throw(error);
  return -1;
 }
 return 0;
}
DEE_STATIC_INLINE(int)  TPP_CALLBACK_PRAGMA_MESSAGE(
 TPPLexerObject const *self, char const *p, Dee_size_t s) {
 return DeeFile_WriteAll((DeeObject *)_lexer->l_message_out,p,s);
}
DEE_STATIC_INLINE(int)  TPP_CALLBACK_ERROR_MESSAGE(
 TPPLexerObject const *self, char const *p) {
 return DeeFile_Print((DeeObject *)_lexer->l_error_out,p);
}




DEE_A_RET_OBJECT_EXCEPT_REF(DeeTokenObject) *DeeToken_New(void) {
 DeeTokenObject *result;
 if ((result = DEE_OBJECTPOOL_ALLOC(token_pool)) != NULL) {
  DeeObject_INIT(result,&DeeToken_Type);
  TPPToken_Init(&result->tk_token);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTokenObject) *
DeeToken_NewFromTPPToken(DEE_A_IN TPPTokenObject const *tk) {
 DeeTokenObject *result;
 if ((result = DEE_OBJECTPOOL_ALLOC(token_pool)) != NULL) {
  DeeObject_INIT(result,&DeeToken_Type);
  TPPToken_InitCopy(&result->tk_token,tk);
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTokenObject) *
DeeToken_Copy(DEE_A_IN_OBJECT(DeeTokenObject) const *self) {
 DeeTokenObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeToken_Check(self));
 if ((result = DEE_OBJECTPOOL_ALLOC(token_pool)) != NULL) {
  DeeObject_INIT(result,&DeeToken_Type);
  TPPToken_InitCopy(&result->tk_token,DeeToken_TOKEN(self));
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeToken_StrTPPToken(TPPTokenObject const *self) {
 Dee_size_t len; DeeObject *result;
 len = TPPToken_Str(self,NULL,0);
 result = DeeString_NewSized(len);
 if (result) TPPToken_Str(self,DeeString_STR(result),len);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeToken_Str(
 DEE_A_IN_OBJECT(DeeTokenObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeToken_Check(self));
 return DeeToken_StrTPPToken(DeeToken_TOKEN(self));
}
DEE_A_RET_WUNUSED TPPTokenID DeeToken_Id(DEE_A_IN_OBJECT(DeeTokenObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeToken_Check(self));
 return DeeToken_ID(self);
}
DEE_A_RET_WUNUSED DEE_A_RET_Z_OPT char const *
DeeToken_File(DEE_A_IN_OBJECT(DeeTokenObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeToken_Check(self));
 return DeeToken_FILE(self);
}
DEE_A_RET_WUNUSED int DeeToken_Line(DEE_A_IN_OBJECT(DeeTokenObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeToken_Check(self));
 return DeeToken_LINE(self);
}
DEE_A_RET_WUNUSED int DeeToken_LineAfter(DEE_A_IN_OBJECT(DeeTokenObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeToken_Check(self));
 return DeeToken_LINE_AFTER(self);
}
DEE_A_RET_EXCEPT_REF /*Any integer type*/DeeObject *DeeToken_EvalInt(
 DEE_A_IN_OBJECT(DeeTokenObject) const *self, DEE_A_IN_OBJECT(DeeLexerObject) *lexer) {
 DeeObject *result;
 TPPUInteger int_val;
 TPPIntegerType int_val_t;
 DEE_ASSERT(DeeObject_Check(self) && DeeToken_Check(self));
 if (TPPToken_EvalInt(DeeToken_TOKEN(self),&int_val,&int_val_t) != 0) {
  if (DeeError_CompilerError(DEE_WARNING_INVALID_INTEGER_FORMAT,
      lexer,(DeeObject *)self,"Invalid int format") != 0) return NULL;
  return DeeObject_New(Dee_rt_int,0);
 }
 switch (int_val_t) {
  case TPPIntegerType_CHAR:               result = DeeObject_New(Dee_rt_char,(Dee_rt_char)int_val); break;
  case TPPIntegerType_WCHAR:              result = DeeObject_New(Dee_rt_wchar_t,(Dee_rt_wchar_t)int_val); break;
  case TPPIntegerType_CHAR16:             result = DeeObject_New(Dee_rt_char16_t,(Dee_rt_char16_t)int_val); break;
  case TPPIntegerType_CHAR32:             result = DeeObject_New(Dee_rt_char32_t,(Dee_rt_char32_t)int_val); break;
  case TPPIntegerType_INT:                result = DeeObject_New(Dee_rt_int,(Dee_rt_int)int_val); break;
  case TPPIntegerType_UNSIGNED:           result = DeeObject_New(Dee_rt_uint,(Dee_rt_uint)int_val); break;
  case TPPIntegerType_LONG:               result = DeeObject_New(Dee_rt_long,(Dee_rt_long)int_val); break;
  case TPPIntegerType_LONG_LONG:          result = DeeObject_New(Dee_rt_llong,(Dee_rt_llong)int_val); break;
  case TPPIntegerType_UNSIGNED_LONG:      result = DeeObject_New(Dee_rt_ulong,(Dee_rt_ulong)int_val); break;
  case TPPIntegerType_UNSIGNED_LONG_LONG: result = DeeObject_New(Dee_rt_ullong,(Dee_rt_ullong)int_val); break;
  case TPPIntegerType_INT8:               result = DeeObject_New(Dee_int8_t,(Dee_int8_t)int_val); break;
  case TPPIntegerType_INT16:              result = DeeObject_New(Dee_int16_t,(Dee_int16_t)int_val); break;
  case TPPIntegerType_INT32:              result = DeeObject_New(Dee_int32_t,(Dee_int32_t)int_val); break;
  case TPPIntegerType_INT64:              result = DeeObject_New(Dee_int64_t,(Dee_int64_t)int_val); break;
  case TPPIntegerType_UINT8:              result = DeeObject_New(Dee_uint8_t,(Dee_uint8_t)int_val); break;
  case TPPIntegerType_UINT16:             result = DeeObject_New(Dee_uint16_t,(Dee_uint16_t)int_val); break;
  case TPPIntegerType_UINT32:             result = DeeObject_New(Dee_uint32_t,(Dee_uint32_t)int_val); break;
  case TPPIntegerType_UINT64:             result = DeeObject_New(Dee_uint64_t,(Dee_uint64_t)int_val); break;
  default: DEE_BUILTIN_UNREACHABLE();
 }
 return result;
}
DEE_A_RET_EXCEPT_REF /*Any float type*/DeeObject *DeeToken_EvalFloat(
 DEE_A_IN_OBJECT(DeeTokenObject) const *self, DEE_A_IN_OBJECT(DeeLexerObject) *lexer) {
 DeeObject *result;
 TPPFloat float_val;
 TPPFloatType float_val_t;
 DEE_ASSERT(DeeObject_Check(self) && DeeToken_Check(self));
 if (TPPToken_EvalFloat(DeeToken_TOKEN(self),&float_val,&float_val_t) != 0) {
  if (DeeError_CompilerError(DEE_WARNING_INVALID_FLOAT_FORMAT,lexer,
      (DeeObject *)self,"Invalid float format") != 0) return NULL;
  result = DeeObject_New(Dee_rt_double,(Dee_rt_double)0.0);
 } else switch (float_val_t) {
  case TPPFloatType_DOUBLE:  result = DeeObject_New(Dee_rt_double,(Dee_rt_double)float_val); break;
  case TPPFloatType_FLOAT:   result = DeeObject_New(Dee_rt_float,(Dee_rt_float)float_val); break;
  case TPPFloatType_LDOUBLE: result = DeeObject_New(Dee_rt_ldouble,(Dee_rt_ldouble)float_val); break;
  default: DEE_BUILTIN_UNREACHABLE();
 }
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeToken_EvalString(
 DEE_A_IN_OBJECT(DeeTokenObject) const *self) {
 Dee_size_t len; DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeToken_Check(self));
 len = TPPToken_EvalStr(DeeToken_TOKEN(self),NULL,0);
#if DEE_CONFIG_ENCODING_COUNT > 1
 switch (TPPToken_GetStringType(DeeToken_TOKEN(self))) {
#if DEE_CONFIG_HAVE_ENCODING_WIDE
  case TPPStringType_WCHAR:  result = DeeWideString_NewSized(len/sizeof(Dee_rt_wchar_t)); break;
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
  case TPPStringType_CHAR16: result = DeeUtf16String_NewSized(len/sizeof(Dee_rt_char16_t)); break;
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
  case TPPStringType_CHAR32: result = DeeUtf32String_NewSized(len/sizeof(Dee_rt_char32_t)); break;
#endif
  default:
#if DEE_CONFIG_HAVE_ENCODING_UTF8
   result = DeeUtf8String_NewSized(len/sizeof(Dee_rt_char));
#else
   result = DeeString_NewSized(len/sizeof(char));
#endif
   break;
 }
#else
 result = DeeString_NewSized(len/sizeof(char));
#endif
 if (result) TPPToken_EvalStr(DeeToken_TOKEN(self),DeeString_STR(result),len);
 return result;
}



#if !defined(DEE_PLATFORM_ENDIAN)
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST char const *) _dee_get_byteorder(void) {
 // I know I could detect this through preprocessor definitions,
 // but this is as portable as it gets. And deemon compiler doesn't
 // need to know about its endian when it's compiled.
 // But using this, we can put defines in deemon's preprocessor,
 // that can tell scripts what the current endian looks like.
 // PLUS: I also know 99% these endians don't really exist. - But hey, why not?
 static union {
  Dee_uint8_t ui8[4];
  Dee_uint32_t ui32;
 } const endian_c = {{0x44,0x33,0x22,0x11}};
#ifndef UINT32_C
#define UINT32_C(x) x
#endif
 switch (endian_c.ui32) {
#define CASE(a,b,c,d) \
 case UINT32_C(0x##a##a##b##b##c##c##d##d): return #a #b #c #d
  // Every decimal combination of '1', '2',
  // '3' and '4' with non-recurring digits.
  CASE(1,2,3,4);
  CASE(1,2,4,3);
  CASE(1,3,2,4);
  CASE(1,3,4,2);
  CASE(1,4,2,3);
  CASE(1,4,3,2);
  CASE(2,1,3,4);
  CASE(2,1,4,3);
  CASE(2,3,1,4);
  CASE(2,3,4,1);
  CASE(2,4,1,3);
  CASE(2,4,3,1);
  CASE(3,1,2,4);
  CASE(3,1,4,2);
  CASE(3,2,1,4);
  CASE(3,2,4,1);
  CASE(3,4,1,2);
  CASE(3,4,2,1);
  CASE(4,1,2,3);
  CASE(4,1,3,2);
  CASE(4,2,1,3);
  CASE(4,2,3,1);
  CASE(4,3,1,2);
  CASE(4,3,2,1);
#undef CASE
  // v It's impossible that we didn't cover them all!
  default: DEE_BUILTIN_UNREACHABLE();
 }
}
#endif

DEE_A_RET_OBJECT_EXCEPT_REF(DeeLexerObject) *DeeLexer_New(void) {
 DeeLexerObject *result;
 if ((result = DeeObject_MALLOCF(DeeLexerObject,"lexer object")) != NULL) {
  DeeObject_INIT(result,&DeeLexer_Type);
  if (DeeLexer_Init(result) != 0) {
   _DeeObject_DELETE(&DeeLexer_Type,result);
   result = NULL;
  }
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeLexerObject) *DeeLexer_NewWithStdInc(void) {
 DeeObject *result = DeeLexer_New();
 if (result && DeeLexer_AddStdInc(result) != 0) Dee_CLEAR(result);
 return result;
}

static int _DeeLexer_TryAddPathObject(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self,
 DEE_A_IN_OBJECT(DeeAnyStringObject) *path_ob) {
 int result;
 // Only add the path if it really exists
 if ((result = _DeeFS_IsDirObject(path_ob)) > 0)
  result = _DeeLexer_AddSysIncludePathObject(self,path_ob);
 return result;
}
static int _DeeLexer_TryAddPathf(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z char const *fmt, ...) {
 va_list args; int result; DeeObject *path_ob;
 DEE_VA_START(args,fmt);
 path_ob = DeeString_VNewf(fmt,args);
 DEE_VA_END(args);
 if (!path_ob) return -1;
 result = _DeeLexer_TryAddPathObject(self,path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_AddStdInc(DEE_A_INOUT_OBJECT(DeeLexerObject) *self) {
 DeeObject *home; int error;
 if DEE_UNLIKELY((home = Dee_GetHome()) == NULL) return -1;
 error = (_DeeLexer_TryAddPathf(self,"%kinclude",home) != 0
       || _DeeLexer_TryAddPathf(self,"%kinclude" DEE_FS_SEP_S "cxx",home) != 0
       || _DeeLexer_TryAddPathf(self,"%kinclude" DEE_FS_SEP_S "dex",home) != 0
       || _DeeLexer_TryAddPathf(self,"%kinclude" DEE_FS_SEP_S "tpp",home) != 0
       ) ? -1 : 0;
 Dee_DECREF(home);
 return error;
}
DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeObject *
DeeLexer_Token(DEE_A_INOUT_OBJECT(DeeLexerObject) *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 return DeeLexer_TOKEN(self);
}

DEE_A_RET_EXCEPT(-1) int DeeLexer_Init(DEE_A_IN DeeLexerObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 if ((self->l_token = (DeeTokenObject *)DeeToken_New()) == NULL) {
  return -1;
 }
 if (TPPLexer_Init(&self->l_lexer) != 0) {
err_tok: Dee_DECREF(self->l_token); return -1;
 }
 self->l_error_cur = 0;
 self->l_error_max = 100;
 self->l_error_last = NULL;
 _DeeStructPackingStack_Init(&self->l_pack);
 self->l_suppress_warnings = 0;
 self->l_suppress_Wcop = 0;
 self->l_token_pos = 0;
 self->l_syntax_recursion = 0;
 self->l_max_syntax_recursion = 256;
 DEE_LVERBOSE3("Defining builtin macros for %k\n",self);
#define DEFINE(name,value) \
 if(TPPLexer_FastDefine(&self->l_lexer,name,value)==-1)goto err_def
 DEFINE("__DEEMON__",DEE_PP_STR(DEE_VERSION_COMPILER));
 DEFINE("__DEEMON_API__",DEE_PP_STR(DEE_VERSION_API));
 DEFINE("__DEEMON_REVISION__",DEE_PP_STR(DEE_VERSION_REVISION));
#if defined(DEE_PLATFORM_ENDIAN)
 DEFINE("__BYTEORDER__",DEE_PP_STR(DEE_PLATFORM_ENDIAN));
#else
 DEFINE("__BYTEORDER__",_dee_get_byteorder());
#endif
#define DEE_INT_T_1     "__signed__ __int8"
#define DEE_INT_T_2     "__signed__ __int16"
#define DEE_INT_T_4     "__signed__ __int32"
#define DEE_INT_T_8     "__signed__ __int64"
#define DEE_UINT_T_1    "__unsigned__ __int8"
#define DEE_UINT_T_2    "__unsigned__ __int16"
#define DEE_UINT_T_4    "__unsigned__ __int32"
#define DEE_UINT_T_8    "__unsigned__ __int64"
#define DEE_INT_MIN_1   "(-128i8)"
#define DEE_INT_MIN_2   "(-32768i16)"
#define DEE_INT_MIN_4   "(-2147483648i32)"
#define DEE_INT_MIN_8   "(-9223372036854775808i64)"
#define DEE_INT_MAX_1   "127i8"
#define DEE_INT_MAX_2   "32767i16"
#define DEE_INT_MAX_4   "2147483647i32"
#define DEE_INT_MAX_8   "9223372036854775807i64"
#define DEE_UINT_MIN_1  "0x00ui8"
#define DEE_UINT_MIN_2  "0x0000ui16"
#define DEE_UINT_MIN_4  "0x00000000ui32"
#define DEE_UINT_MIN_8  "0x0000000000000000ui64"
#define DEE_UINT_MAX_1  "0xffui8"
#define DEE_UINT_MAX_2  "0xffffui16"
#define DEE_UINT_MAX_4  "0xffffffffui32"
#define DEE_UINT_MAX_8  "0xffffffffffffffffui64"
#define DEE_INT_T(sizeof)  DEE_PP_CAT_2(DEE_INT_T_,sizeof)
#define DEE_UINT_T(sizeof) DEE_PP_CAT_2(DEE_UINT_T_,sizeof)
#define DEE_INT_MIN(sizeof) DEE_PP_CAT_2(DEE_INT_MIN_,sizeof)
#define DEE_INT_MAX(sizeof) DEE_PP_CAT_2(DEE_INT_MAX_,sizeof)
#define DEE_UINT_MIN(sizeof) DEE_PP_CAT_2(DEE_UINT_MIN_,sizeof)
#define DEE_UINT_MAX(sizeof) DEE_PP_CAT_2(DEE_UINT_MAX_,sizeof)

#define DEE_PRIVATE_XINT_T_0 DEE_UINT_T
#define DEE_PRIVATE_XINT_T_1 DEE_INT_T
#define DEE_XINT_T(signed,sizeof) DEE_PP_CAT_2(DEE_PRIVATE_XINT_T_,signed)(sizeof)
#define DEE_PRIVATE_XINT_MIN_0 DEE_UINT_MIN
#define DEE_PRIVATE_XINT_MIN_1 DEE_INT_MIN
#define DEE_PRIVATE_XINT_MAX_0 DEE_UINT_MAX
#define DEE_PRIVATE_XINT_MAX_1 DEE_INT_MAX
#define DEE_XINT_MIN(signed,sizeof) DEE_PP_CAT_2(DEE_PRIVATE_XINT_MIN_,signed)(sizeof)
#define DEE_XINT_MAX(signed,sizeof) DEE_PP_CAT_2(DEE_PRIVATE_XINT_MAX_,signed)(sizeof)

 DEFINE("_INTEGRAL_MAX_BITS",DEE_PP_STR(DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_INTMAX_T)));
 DEFINE("__CHAR_MIN__",DEE_XINT_MIN(DEE_CONFIG_RT_CHAR_SIGNED,DEE_CONFIG_RT_SIZEOF_CHAR));
 DEFINE("__CHAR_MAX__",DEE_XINT_MIN(DEE_CONFIG_RT_CHAR_SIGNED,DEE_CONFIG_RT_SIZEOF_CHAR));
 DEFINE("__SCHAR_MIN__",DEE_INT_MIN(DEE_CONFIG_RT_SIZEOF_CHAR));
 DEFINE("__SCHAR_MAX__",DEE_INT_MAX(DEE_CONFIG_RT_SIZEOF_CHAR));
 DEFINE("__UCHAR_MAX__",DEE_UINT_MAX(DEE_CONFIG_RT_SIZEOF_CHAR));
 DEFINE("__SHORT_MIN__",DEE_INT_MIN(DEE_CONFIG_RT_SIZEOF_SHORT));
 DEFINE("__SHORT_MAX__",DEE_INT_MAX(DEE_CONFIG_RT_SIZEOF_SHORT));
 DEFINE("__USHORT_MAX__",DEE_UINT_MAX(DEE_CONFIG_RT_SIZEOF_SHORT));
 DEFINE("__INT_MIN__",DEE_INT_MIN(DEE_CONFIG_RT_SIZEOF_INT));
 DEFINE("__INT_MAX__",DEE_INT_MAX(DEE_CONFIG_RT_SIZEOF_INT));
 DEFINE("__UINT_MAX__",DEE_UINT_MAX(DEE_CONFIG_RT_SIZEOF_INT));
 DEFINE("__LONG_MIN__",DEE_INT_MIN(DEE_CONFIG_RT_SIZEOF_LONG));
 DEFINE("__LONG_MAX__",DEE_INT_MAX(DEE_CONFIG_RT_SIZEOF_LONG));
 DEFINE("__ULONG_MAX__",DEE_UINT_MAX(DEE_CONFIG_RT_SIZEOF_LONG));
 DEFINE("__LLONG_MIN__",DEE_INT_MIN(DEE_CONFIG_RT_SIZEOF_LLONG));
 DEFINE("__LLONG_MAX__",DEE_INT_MAX(DEE_CONFIG_RT_SIZEOF_LLONG));
 DEFINE("__ULLONG_MAX__",DEE_UINT_MAX(DEE_CONFIG_RT_SIZEOF_LLONG));
 DEFINE("__SIZEOF_CHAR__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_CHAR));
 DEFINE("__SIZEOF_WCHAR__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_WCHAR_T));
 DEFINE("__SIZEOF_CHAR16__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_CHAR16_T));
 DEFINE("__SIZEOF_CHAR32__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_CHAR32_T));
 DEFINE("__SIZEOF_BOOL__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_BOOL));
 DEFINE("__SIZEOF_SHORT__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_SHORT));
 DEFINE("__SIZEOF_INT__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_INT));
 DEFINE("__SIZEOF_LONG__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_LONG));
 DEFINE("__SIZEOF_LONG_LONG__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_LLONG));
 DEFINE("__SIZEOF_LLONG__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_LLONG)); // deprecated
 DEFINE("__SIZEOF_POINTER__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_POINTER));
 DEFINE("__SIZEOF_INTMAX__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_INTMAX_T));
 DEFINE("__SIZEOF_INT_LEAST8__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_INT_LEAST8_T));
 DEFINE("__SIZEOF_INT_LEAST16__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_INT_LEAST16_T));
 DEFINE("__SIZEOF_INT_LEAST32__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_INT_LEAST32_T));
 DEFINE("__SIZEOF_INT_LEAST64__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_INT_LEAST64_T));
 DEFINE("__SIZEOF_INT_FAST8__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_INT_FAST8_T));
 DEFINE("__SIZEOF_INT_FAST16__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_INT_FAST16_T));
 DEFINE("__SIZEOF_INT_FAST32__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_INT_FAST32_T));
 DEFINE("__SIZEOF_INT_FAST64__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_INT_FAST64_T));
 DEFINE("__SIZEOF_TIME_T__",DEE_PP_STR(DEE_TYPES_SIZEOF_TIME_T));

 // A whole bunch of more predefined macros
 DEFINE("__INT8_TYPE__",DEE_INT_T_1);
 DEFINE("__INT8_MIN__",DEE_INT_MIN_1);
 DEFINE("__INT8_MAX__",DEE_INT_MAX_1);

 DEFINE("__INT16_TYPE__",DEE_INT_T_2);
 DEFINE("__INT16_MIN__",DEE_INT_MIN_2);
 DEFINE("__INT16_MAX__",DEE_INT_MAX_2);

 DEFINE("__INT32_TYPE__",DEE_INT_T_4);
 DEFINE("__INT32_MIN__",DEE_INT_MIN_4);
 DEFINE("__INT32_MAX__",DEE_INT_MAX_4);

 DEFINE("__INT64_TYPE__",DEE_INT_T_8);
 DEFINE("__INT64_MIN__",DEE_INT_MIN_8);
 DEFINE("__INT64_MAX__",DEE_INT_MAX_8);

 DEFINE("__UINT8_TYPE__",DEE_UINT_T_1);
 DEFINE("__UINT8_MIN__",DEE_UINT_MIN_1);
 DEFINE("__UINT8_MAX__",DEE_UINT_MAX_1);

 DEFINE("__UINT16_TYPE__",DEE_UINT_T_2);
 DEFINE("__UINT16_MIN__",DEE_UINT_MIN_2);
 DEFINE("__UINT16_MAX__",DEE_UINT_MAX_2);

 DEFINE("__UINT32_TYPE__",DEE_UINT_T_4);
 DEFINE("__UINT32_MIN__",DEE_UINT_MIN_4);
 DEFINE("__UINT32_MAX__",DEE_UINT_MAX_4);

 DEFINE("__UINT64_TYPE__",DEE_UINT_T_8);
 DEFINE("__UINT64_MIN__",DEE_UINT_MIN_8);
 DEFINE("__UINT64_MAX__",DEE_UINT_MAX_8);

 DEFINE("__INT_LEAST8_TYPE__",DEE_INT_T(DEE_CONFIG_RT_SIZEOF_INT_LEAST8_T));
 DEFINE("__INT_LEAST8_MIN__",DEE_INT_MIN(DEE_CONFIG_RT_SIZEOF_INT_LEAST8_T));
 DEFINE("__INT_LEAST8_MAX__",DEE_INT_MAX(DEE_CONFIG_RT_SIZEOF_INT_LEAST8_T));

 DEFINE("__INT_LEAST16_TYPE__",DEE_INT_T(DEE_CONFIG_RT_SIZEOF_INT_LEAST16_T));
 DEFINE("__INT_LEAST16_MIN__",DEE_INT_MIN(DEE_CONFIG_RT_SIZEOF_INT_LEAST16_T));
 DEFINE("__INT_LEAST16_MAX__",DEE_INT_MAX(DEE_CONFIG_RT_SIZEOF_INT_LEAST16_T));

 DEFINE("__INT_LEAST32_TYPE__",DEE_INT_T(DEE_CONFIG_RT_SIZEOF_INT_LEAST32_T));
 DEFINE("__INT_LEAST32_MIN__",DEE_INT_MIN(DEE_CONFIG_RT_SIZEOF_INT_LEAST32_T));
 DEFINE("__INT_LEAST32_MAX__",DEE_INT_MAX(DEE_CONFIG_RT_SIZEOF_INT_LEAST32_T));

 DEFINE("__INT_LEAST64_TYPE__",DEE_INT_T(DEE_CONFIG_RT_SIZEOF_INT_LEAST64_T));
 DEFINE("__INT_LEAST64_MIN__",DEE_INT_MIN(DEE_CONFIG_RT_SIZEOF_INT_LEAST64_T));
 DEFINE("__INT_LEAST64_MAX__",DEE_INT_MAX(DEE_CONFIG_RT_SIZEOF_INT_LEAST64_T));

 DEFINE("__UINT_LEAST8_TYPE__",DEE_UINT_T(DEE_CONFIG_RT_SIZEOF_INT_LEAST8_T));
 DEFINE("__UINT_LEAST8_MIN__",DEE_UINT_MIN(DEE_CONFIG_RT_SIZEOF_INT_LEAST8_T));
 DEFINE("__UINT_LEAST8_MAX__",DEE_UINT_MAX(DEE_CONFIG_RT_SIZEOF_INT_LEAST8_T));

 DEFINE("__UINT_LEAST16_TYPE__",DEE_UINT_T(DEE_CONFIG_RT_SIZEOF_INT_LEAST16_T));
 DEFINE("__UINT_LEAST16_MIN__",DEE_UINT_MIN(DEE_CONFIG_RT_SIZEOF_INT_LEAST16_T));
 DEFINE("__UINT_LEAST16_MAX__",DEE_UINT_MAX(DEE_CONFIG_RT_SIZEOF_INT_LEAST16_T));

 DEFINE("__UINT_LEAST32_TYPE__",DEE_UINT_T(DEE_CONFIG_RT_SIZEOF_INT_LEAST32_T));
 DEFINE("__UINT_LEAST32_MIN__",DEE_UINT_MIN(DEE_CONFIG_RT_SIZEOF_INT_LEAST32_T));
 DEFINE("__UINT_LEAST32_MAX__",DEE_UINT_MAX(DEE_CONFIG_RT_SIZEOF_INT_LEAST32_T));

 DEFINE("__UINT_LEAST64_TYPE__",DEE_UINT_T(DEE_CONFIG_RT_SIZEOF_INT_LEAST64_T));
 DEFINE("__UINT_LEAST64_MIN__",DEE_UINT_MIN(DEE_CONFIG_RT_SIZEOF_INT_LEAST64_T));
 DEFINE("__UINT_LEAST64_MAX__",DEE_UINT_MAX(DEE_CONFIG_RT_SIZEOF_INT_LEAST64_T));

 DEFINE("__INT_FAST8_TYPE__",DEE_INT_T(DEE_CONFIG_RT_SIZEOF_INT_FAST8_T));
 DEFINE("__INT_FAST8_MIN__",DEE_INT_MIN(DEE_CONFIG_RT_SIZEOF_INT_FAST8_T));
 DEFINE("__INT_FAST8_MAX__",DEE_INT_MAX(DEE_CONFIG_RT_SIZEOF_INT_FAST8_T));

 DEFINE("__INT_FAST16_TYPE__",DEE_INT_T(DEE_CONFIG_RT_SIZEOF_INT_FAST16_T));
 DEFINE("__INT_FAST16_MIN__",DEE_INT_MIN(DEE_CONFIG_RT_SIZEOF_INT_FAST16_T));
 DEFINE("__INT_FAST16_MAX__",DEE_INT_MAX(DEE_CONFIG_RT_SIZEOF_INT_FAST16_T));

 DEFINE("__INT_FAST32_TYPE__",DEE_INT_T(DEE_CONFIG_RT_SIZEOF_INT_FAST32_T));
 DEFINE("__INT_FAST32_MIN__",DEE_INT_MIN(DEE_CONFIG_RT_SIZEOF_INT_FAST32_T));
 DEFINE("__INT_FAST32_MAX__",DEE_INT_MAX(DEE_CONFIG_RT_SIZEOF_INT_FAST32_T));

 DEFINE("__INT_FAST64_TYPE__",DEE_INT_T(DEE_CONFIG_RT_SIZEOF_INT_FAST64_T));
 DEFINE("__INT_FAST64_MIN__",DEE_INT_MIN(DEE_CONFIG_RT_SIZEOF_INT_FAST64_T));
 DEFINE("__INT_FAST64_MAX__",DEE_INT_MAX(DEE_CONFIG_RT_SIZEOF_INT_FAST64_T));

 DEFINE("__UINT_FAST8_TYPE__",DEE_UINT_T(DEE_CONFIG_RT_SIZEOF_INT_FAST8_T));
 DEFINE("__UINT_FAST8_MIN__",DEE_UINT_MIN(DEE_CONFIG_RT_SIZEOF_INT_FAST8_T));
 DEFINE("__UINT_FAST8_MAX__",DEE_UINT_MAX(DEE_CONFIG_RT_SIZEOF_INT_FAST8_T));

 DEFINE("__UINT_FAST16_TYPE__",DEE_UINT_T(DEE_CONFIG_RT_SIZEOF_INT_FAST16_T));
 DEFINE("__UINT_FAST16_MIN__",DEE_UINT_MIN(DEE_CONFIG_RT_SIZEOF_INT_FAST16_T));
 DEFINE("__UINT_FAST16_MAX__",DEE_UINT_MAX(DEE_CONFIG_RT_SIZEOF_INT_FAST16_T));

 DEFINE("__UINT_FAST32_TYPE__",DEE_UINT_T(DEE_CONFIG_RT_SIZEOF_INT_FAST32_T));
 DEFINE("__UINT_FAST32_MIN__",DEE_UINT_MIN(DEE_CONFIG_RT_SIZEOF_INT_FAST32_T));
 DEFINE("__UINT_FAST32_MAX__",DEE_UINT_MAX(DEE_CONFIG_RT_SIZEOF_INT_FAST32_T));

 DEFINE("__UINT_FAST64_TYPE__",DEE_UINT_T(DEE_CONFIG_RT_SIZEOF_INT_FAST64_T));
 DEFINE("__UINT_FAST64_MIN__",DEE_UINT_MIN(DEE_CONFIG_RT_SIZEOF_INT_FAST64_T));
 DEFINE("__UINT_FAST64_MAX__",DEE_UINT_MAX(DEE_CONFIG_RT_SIZEOF_INT_FAST64_T));

 DEFINE("__INTMAX_TYPE__",DEE_INT_T(DEE_CONFIG_RT_SIZEOF_INTMAX_T));
 DEFINE("__INTMAX_MIN__",DEE_INT_MIN(DEE_CONFIG_RT_SIZEOF_INTMAX_T));
 DEFINE("__INTMAX_MAX__",DEE_INT_MAX(DEE_CONFIG_RT_SIZEOF_INTMAX_T));

 DEFINE("__UINTMAX_TYPE__",DEE_UINT_T(DEE_CONFIG_RT_SIZEOF_INTMAX_T));
 DEFINE("__UINTMAX_MIN__",DEE_UINT_MIN(DEE_CONFIG_RT_SIZEOF_INTMAX_T));
 DEFINE("__UINTMAX_MAX__",DEE_UINT_MAX(DEE_CONFIG_RT_SIZEOF_INTMAX_T));

 DEFINE("__INTPTR_TYPE__",DEE_INT_T(DEE_CONFIG_RT_SIZEOF_POINTER));
 DEFINE("__INTPTR_MIN__",DEE_INT_MIN(DEE_CONFIG_RT_SIZEOF_POINTER));
 DEFINE("__INTPTR_MAX__",DEE_INT_MAX(DEE_CONFIG_RT_SIZEOF_POINTER));

 DEFINE("__UINTPTR_TYPE__",DEE_UINT_T(DEE_CONFIG_RT_SIZEOF_POINTER));
 DEFINE("__UINTPTR_MIN__",DEE_UINT_MIN(DEE_CONFIG_RT_SIZEOF_POINTER));
 DEFINE("__UINTPTR_MAX__",DEE_UINT_MAX(DEE_CONFIG_RT_SIZEOF_POINTER));

 DEFINE("__PTRDIFF_TYPE__",DEE_INT_T(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T));
 DEFINE("__PTRDIFF_MIN__",DEE_INT_MIN(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T));
 DEFINE("__PTRDIFF_MAX__",DEE_INT_MAX(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T));

 DEFINE("__SIZE_TYPE__",DEE_UINT_T(DEE_CONFIG_RT_SIZEOF_SIZE_T));
 DEFINE("__SIZE_MIN__",DEE_UINT_MIN(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T));
 DEFINE("__SIZE_MAX__",DEE_UINT_MAX(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T));

 DEFINE("__WCHAR_MIN__",DEE_XINT_MIN(DEE_CONFIG_RT_WCHAR_T_SIGNED,DEE_CONFIG_RT_SIZEOF_WCHAR_T));
 DEFINE("__WCHAR_MAX__",DEE_XINT_MAX(DEE_CONFIG_RT_WCHAR_T_SIGNED,DEE_CONFIG_RT_SIZEOF_WCHAR_T));

 DEFINE("__CHAR16_MIN__",DEE_XINT_MIN(DEE_CONFIG_RT_CHAR16_T_SIGNED,DEE_CONFIG_RT_SIZEOF_CHAR16_T));
 DEFINE("__CHAR16_MAX__",DEE_XINT_MAX(DEE_CONFIG_RT_CHAR16_T_SIGNED,DEE_CONFIG_RT_SIZEOF_CHAR16_T));

 DEFINE("__CHAR32_MIN__",DEE_XINT_MIN(DEE_CONFIG_RT_CHAR32_T_SIGNED,DEE_CONFIG_RT_SIZEOF_CHAR32_T));
 DEFINE("__CHAR32_MAX__",DEE_XINT_MAX(DEE_CONFIG_RT_CHAR32_T_SIGNED,DEE_CONFIG_RT_SIZEOF_CHAR32_T));

 DEFINE("__SIZEOF_FLOAT__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_FLOAT));
 DEFINE("__SIZEOF_DOUBLE__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_DOUBLE));
 DEFINE("__SIZEOF_LONG_DOUBLE__",DEE_PP_STR(DEE_CONFIG_RT_SIZEOF_LDOUBLE));

 DEFINE("__FLT_RADIX__",DEE_PP_STR(FLT_RADIX));
 DEFINE("__FLT_ROUNDS__",DEE_PP_STR(FLT_ROUNDS));
#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_FLOAT
 DEFINE("__FLT_DIG__",DEE_PP_STR(FLT_DIG));
 DEFINE("__FLT_MANT_DIG__",DEE_PP_STR(FLT_MANT_DIG));
 DEFINE("__FLT_MAX__",DEE_PP_STR(FLT_MAX));
 DEFINE("__FLT_MIN__",DEE_PP_STR(FLT_MIN));
 DEFINE("__FLT_MIN_EXP__",DEE_PP_STR(FLT_MIN_EXP));
 DEFINE("__FLT_MIN_10_EXP__",DEE_PP_STR(FLT_MIN_10_EXP));
 DEFINE("__FLT_MAX_EXP__",DEE_PP_STR(FLT_MAX_EXP));
 DEFINE("__FLT_MAX_10_EXP__",DEE_PP_STR(FLT_MAX_10_EXP));
 DEFINE("__FLT_EPSILON__",DEE_PP_STR(FLT_EPSILON));
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_DOUBLE
 DEFINE("__FLT_DIG__",DEE_PP_STR(DBL_DIG));
 DEFINE("__FLT_MANT_DIG__",DEE_PP_STR(DBL_MANT_DIG));
 DEFINE("__FLT_MAX__",DEE_PP_STR(DBL_MAX));
 DEFINE("__FLT_MIN__",DEE_PP_STR(DBL_MIN));
 DEFINE("__FLT_MIN_EXP__",DEE_PP_STR(DBL_MIN_EXP));
 DEFINE("__FLT_MIN_10_EXP__",DEE_PP_STR(DBL_MIN_10_EXP));
 DEFINE("__FLT_MAX_EXP__",DEE_PP_STR(DBL_MAX_EXP));
 DEFINE("__FLT_MAX_10_EXP__",DEE_PP_STR(DBL_MAX_10_EXP));
 DEFINE("__FLT_EPSILON__",DEE_PP_STR(DBL_EPSILON));
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_LDOUBLE
 DEFINE("__FLT_DIG__",DEE_PP_STR(LDBL_DIG));
 DEFINE("__FLT_MANT_DIG__",DEE_PP_STR(LDBL_MANT_DIG));
 DEFINE("__FLT_MAX__",DEE_PP_STR(LDBL_MAX));
 DEFINE("__FLT_MIN__",DEE_PP_STR(LDBL_MIN));
 DEFINE("__FLT_MIN_EXP__",DEE_PP_STR(LDBL_MIN_EXP));
 DEFINE("__FLT_MIN_10_EXP__",DEE_PP_STR(LDBL_MIN_10_EXP));
 DEFINE("__FLT_MAX_EXP__",DEE_PP_STR(LDBL_MAX_EXP));
 DEFINE("__FLT_MAX_10_EXP__",DEE_PP_STR(LDBL_MAX_10_EXP));
 DEFINE("__FLT_EPSILON__",DEE_PP_STR(LDBL_EPSILON));
#endif

#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_FLOAT
 DEFINE("__DBL_DIG__",DEE_PP_STR(FLT_DIG));
 DEFINE("__DBL_MANT_DIG__",DEE_PP_STR(FLT_MANT_DIG));
 DEFINE("__DBL_MAX__",DEE_PP_STR(FLT_MAX));
 DEFINE("__DBL_MIN__",DEE_PP_STR(FLT_MIN));
 DEFINE("__DBL_MIN_EXP__",DEE_PP_STR(FLT_MIN_EXP));
 DEFINE("__DBL_MIN_10_EXP__",DEE_PP_STR(FLT_MIN_10_EXP));
 DEFINE("__DBL_MAX_EXP__",DEE_PP_STR(FLT_MAX_EXP));
 DEFINE("__DBL_MAX_10_EXP__",DEE_PP_STR(FLT_MAX_10_EXP));
 DEFINE("__DBL_EPSILON__",DEE_PP_STR(FLT_EPSILON));
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_DOUBLE
 DEFINE("__DBL_DIG__",DEE_PP_STR(DBL_DIG));
 DEFINE("__DBL_MANT_DIG__",DEE_PP_STR(DBL_MANT_DIG));
 DEFINE("__DBL_MAX__",DEE_PP_STR(DBL_MAX));
 DEFINE("__DBL_MIN__",DEE_PP_STR(DBL_MIN));
 DEFINE("__DBL_MIN_EXP__",DEE_PP_STR(DBL_MIN_EXP));
 DEFINE("__DBL_MIN_10_EXP__",DEE_PP_STR(DBL_MIN_10_EXP));
 DEFINE("__DBL_MAX_EXP__",DEE_PP_STR(DBL_MAX_EXP));
 DEFINE("__DBL_MAX_10_EXP__",DEE_PP_STR(DBL_MAX_10_EXP));
 DEFINE("__DBL_EPSILON__",DEE_PP_STR(DBL_EPSILON));
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_LDOUBLE
 DEFINE("__DBL_DIG__",DEE_PP_STR(LDBL_DIG));
 DEFINE("__DBL_MANT_DIG__",DEE_PP_STR(LDBL_MANT_DIG));
 DEFINE("__DBL_MAX__",DEE_PP_STR(LDBL_MAX));
 DEFINE("__DBL_MIN__",DEE_PP_STR(LDBL_MIN));
 DEFINE("__DBL_MIN_EXP__",DEE_PP_STR(LDBL_MIN_EXP));
 DEFINE("__DBL_MIN_10_EXP__",DEE_PP_STR(LDBL_MIN_10_EXP));
 DEFINE("__DBL_MAX_EXP__",DEE_PP_STR(LDBL_MAX_EXP));
 DEFINE("__DBL_MAX_10_EXP__",DEE_PP_STR(LDBL_MAX_10_EXP));
 DEFINE("__DBL_EPSILON__",DEE_PP_STR(LDBL_EPSILON));
#endif

#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_FLOAT
 DEFINE("__LDBL_DIG__",DEE_PP_STR(FLT_DIG));
 DEFINE("__LDBL_MANT_DIG__",DEE_PP_STR(FLT_MANT_DIG));
 DEFINE("__LDBL_MAX__",DEE_PP_STR(FLT_MAX));
 DEFINE("__LDBL_MIN__",DEE_PP_STR(FLT_MIN));
 DEFINE("__LDBL_MIN_EXP__",DEE_PP_STR(FLT_MIN_EXP));
 DEFINE("__LDBL_MIN_10_EXP__",DEE_PP_STR(FLT_MIN_10_EXP));
 DEFINE("__LDBL_MAX_EXP__",DEE_PP_STR(FLT_MAX_EXP));
 DEFINE("__LDBL_MAX_10_EXP__",DEE_PP_STR(FLT_MAX_10_EXP));
 DEFINE("__LDBL_EPSILON__",DEE_PP_STR(FLT_EPSILON));
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_DOUBLE
 DEFINE("__LDBL_DIG__",DEE_PP_STR(DBL_DIG));
 DEFINE("__LDBL_MANT_DIG__",DEE_PP_STR(DBL_MANT_DIG));
 DEFINE("__LDBL_MAX__",DEE_PP_STR(DBL_MAX));
 DEFINE("__LDBL_MIN__",DEE_PP_STR(DBL_MIN));
 DEFINE("__LDBL_MIN_EXP__",DEE_PP_STR(DBL_MIN_EXP));
 DEFINE("__LDBL_MIN_10_EXP__",DEE_PP_STR(DBL_MIN_10_EXP));
 DEFINE("__LDBL_MAX_EXP__",DEE_PP_STR(DBL_MAX_EXP));
 DEFINE("__LDBL_MAX_10_EXP__",DEE_PP_STR(DBL_MAX_10_EXP));
 DEFINE("__LDBL_EPSILON__",DEE_PP_STR(DBL_EPSILON));
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_LDOUBLE
 DEFINE("__LDBL_DIG__",DEE_PP_STR(LDBL_DIG));
 DEFINE("__LDBL_MANT_DIG__",DEE_PP_STR(LDBL_MANT_DIG));
 DEFINE("__LDBL_MAX__",DEE_PP_STR(LDBL_MAX));
 DEFINE("__LDBL_MIN__",DEE_PP_STR(LDBL_MIN));
 DEFINE("__LDBL_MIN_EXP__",DEE_PP_STR(LDBL_MIN_EXP));
 DEFINE("__LDBL_MIN_10_EXP__",DEE_PP_STR(LDBL_MIN_10_EXP));
 DEFINE("__LDBL_MAX_EXP__",DEE_PP_STR(LDBL_MAX_EXP));
 DEFINE("__LDBL_MAX_10_EXP__",DEE_PP_STR(LDBL_MAX_10_EXP));
 DEFINE("__LDBL_EPSILON__",DEE_PP_STR(LDBL_EPSILON));
#endif

#ifdef DEE_TYPES_SIZEOF_UID_T
 DEFINE("__SIZEOF_UID_T__",DEE_PP_STR(DEE_TYPES_SIZEOF_UID_T));
#endif
#ifdef DEE_TYPES_SIZEOF_GID_T
 DEFINE("__SIZEOF_GID_T__",DEE_PP_STR(DEE_TYPES_SIZEOF_GID_T));
#endif
#ifdef DEE_TYPES_SIZEOF_PID_T
 DEFINE("__SIZEOF_PID_T__",DEE_PP_STR(DEE_TYPES_SIZEOF_PID_T));
#endif

#undef DEE_INT_MIN_1
#undef DEE_INT_MIN_2
#undef DEE_INT_MIN_4
#undef DEE_INT_MIN_8
#undef DEE_INT_MAX_1
#undef DEE_INT_MAX_2
#undef DEE_INT_MAX_4
#undef DEE_INT_MAX_8
#undef DEE_UINT_MIN_1
#undef DEE_UINT_MIN_2
#undef DEE_UINT_MIN_4
#undef DEE_UINT_MIN_8
#undef DEE_UINT_MAX_1
#undef DEE_UINT_MAX_2
#undef DEE_UINT_MAX_4
#undef DEE_UINT_MAX_8
#undef DEE_INT_MIN
#undef DEE_INT_MAX
#undef DEE_UINT_MIN
#undef DEE_UINT_MAX
#undef DEFINE
 _DeeFutureTokensList_Init(&self->l_future);
 self->l_func_name = 0;
 self->l_brace_recursion = 0;
 _DeeStaticIfStack_Init(&self->l_static_if_stack);
#if defined(DEE_PLATFORM_WINDOWS)
 if (IsDebuggerPresent()) {
  DeeObject *fp1,*fp2;
  // Output errors to stderr and stddbg when a debugger is present
  fp1 = DeeFile_Std(DEE_STDERR);
  fp2 = DeeFile_Std(DEE_STDDBG);
  self->l_error_out = (DeeFileObject *)DeeFileJoined_New(fp1,fp2);
  Dee_DECREF(fp2);
  Dee_DECREF(fp1);
  if (!self->l_error_out) goto err_def;
 } else
#endif
 {
  self->l_error_out = (DeeFileObject *)DeeFile_Std(DEE_STDERR);
 }
 self->l_message_out = (DeeFileObject *)DeeFile_Std(DEE_STDERR);
 return 0;
err_def:
 TPPLexer_Quit(&self->l_lexer);
 goto err_tok;
}

DEE_A_RET_EXCEPT(-1) int DeeLexer_Utf8AddSysIncludePath(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_Utf8Char const *path) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_Utf8PathExpand(path)) == NULL) return -1;
 result = _DeeLexer_AddSysIncludePathObject(self,path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_WideAddSysIncludePath(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_WideChar const *path) {
 DeeObject *path_ob; int result;
 if ((path_ob = DeeFS_WidePathExpand(path)) == NULL) return -1;
 result = _DeeLexer_AddSysIncludePathObject(self,path_ob);
 Dee_DECREF(path_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_AddSysIncludePathObject(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 if DEE_UNLIKELY((path = DeeFS_PathExpandObject(path)) == NULL) return -1;
 temp = _DeeLexer_AddSysIncludePathObject(self,path);
 Dee_DECREF(path);
 return temp;
}
DEE_A_RET_EXCEPT(-1) int _DeeLexer_Utf8AddSysIncludePath(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_Utf8Char const *path) {
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 DEE_LVERBOSE3("Adding #include path: %q\n",path);
 if (!TPPIncludeList_Has(&DeeLexer_LEXER(self)->l_sys_include_list,path)) {
  return TPPIncludeList_Add(&DeeLexer_LEXER(self)->l_sys_include_list,path);
 } else return 0;
}
DEE_A_RET_EXCEPT(-1) int _DeeLexer_WideAddSysIncludePath(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_WideChar const *path) {
 DeeObject *utf8_path; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 DEE_LVERBOSE3("Adding #include path: %lq\n",path);
 if ((utf8_path = DeeUtf8String_FromWideString(path)) == NULL) return -1;
 if (!TPPIncludeList_HasS(&DeeLexer_LEXER(self)->l_sys_include_list,
     DeeUtf8String_STR(path),DeeUtf8String_SIZE(path))) {
  result = TPPIncludeList_AddS(
   &DeeLexer_LEXER(self)->l_sys_include_list,
   DeeUtf8String_STR(utf8_path),DeeUtf8String_SIZE(utf8_path));
 } else result = 0;
 Dee_DECREF(utf8_path);
 return result;
}
DEE_A_RET_EXCEPT(-1) int _DeeLexer_AddSysIncludePathObject(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeAnyStringObject) const *path) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 if DEE_UNLIKELY((path = DeeUtf8String_Cast(path)) == NULL) return -1;
 if (!TPPIncludeList_HasS(&DeeLexer_LEXER(self)->l_sys_include_list,
     DeeUtf8String_STR(path),DeeUtf8String_SIZE(path))) {
  DEE_LVERBOSE3("Adding #include path: %r\n",path);
  result = TPPIncludeList_AddS(
   &DeeLexer_LEXER(self)->l_sys_include_list,
   DeeUtf8String_STR(path),DeeUtf8String_SIZE(path));
 } else result = 0;
 Dee_DECREF(path);
 return result;
}

#ifdef DEE_DEBUG
DEE_STATIC_INLINE(int) _DeeLexer_BuiltinNext(
 TPPLexerObject *lexer, TPPTokenObject *token) {
 TPPLexerFlags old_flags;
 old_flags = lexer->l_flags&~(TPPLexer_FLAG_RAND_INIT);
 if (TPPLexer_Next(lexer,token) != 0) {
  DEE_ASSERT(DeeError_OCCURRED() && "TPP didn't set an error");
  return -1;
 }
 DEE_ASSERT((lexer->l_flags&~(TPPLexer_FLAG_RAND_INIT)) == old_flags);
 return 0;
}
#else
#define _DeeLexer_BuiltinNext  TPPLexer_Next
#endif

DEE_STATIC_INLINE(int) _DeeLexer_KickStart(DeeLexerObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 DEE_ASSERT(DeeObject_Check(self->l_token) && DeeToken_Check(self->l_token));
 if (!self->l_token->tk_token.tk_id &&
     !DeeFutureTokensList_SIZE(&self->l_future)) { // Must kick-start the lexer
#if 1
  DeeObject *temp_scope;
  // NOTE: This is ok, scope constants created in a __static_if decl aren't visible
  //       from the outside, meaning that we can safely drop all information gathered in temp_scope
  if ((temp_scope = DeeScope_NewRoot()) == NULL) return -1;
  if (!DeeLexer_YieldEx((DeeObject *)self,temp_scope,&DeeCompilerConfig_Default)) { Dee_DECREF(temp_scope); return -1; }
  Dee_DECREF(temp_scope);
#else
  if (_DeeLexer_BuiltinNext(&self->l_lexer,&self->l_token->tk_token) != 0)
   return -1;
#endif
 }
 return 0;
}

static DEE_A_RET_EXCEPT(-1) int _DeeLexer_IncludeFileStreamWithRealName(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_INOUT_OBJECT(DeeFileObject) *filestream,
 DEE_A_IN_Z char const *real_filename, DEE_A_IN_Z char const *disp_filename) {
 DeeObject *filedata; TPPFileObject *tpp_file;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 if ((filedata = DeeFile_ReadData(filestream,(Dee_size_t)-1)) == NULL) return -1;
 tpp_file = TPPFile_NewFromStringEx(
  (((DeeLexerObject *)self)->l_lexer.l_flags&TPPLexer_FLAG_TOK_COMMENTS)!=0
  ?TPPFileFormatFlag_KEEP_COMMENTS:TPPFileFormatFlag_ENABLED,
  DeeString_STR(filedata),DeeString_SIZE(filedata)
  );
 Dee_DECREF(filedata);
 if (tpp_file == NULL) return -1;
 if (_TPPFile_SetName(tpp_file,real_filename) != 0 ||
     TPPFile_SetNewName(tpp_file,disp_filename) != 0) {
  TPPFile_DECREF(tpp_file);
  return -1;
 }
 TPPLexer_PushFileInheritedNoErr(DeeLexer_LEXER(self),tpp_file);
 return _DeeLexer_KickStart((DeeLexerObject *)self);
}
static char const _dee_default_string_filename[] = "<string>";
DEE_A_RET_EXCEPT(-1) int DeeLexer_IncludeFileStream(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *filestream) {
 int result; DeeObject *filedata;
 if (DeeFileIO_Check(filestream)) {
  if ((filedata = DeeUtf8String_Cast(DeeFileIO_FILE(filestream))) == NULL) return -1;
  result = _DeeLexer_IncludeFileStreamWithRealName(
   self,filestream,DeeUtf8String_STR(filedata),DeeUtf8String_STR(filedata));
  Dee_DECREF(filedata);
  return result;
 }
 if ((filedata = DeeFile_ReadData(filestream,(Dee_size_t)-1)) == NULL) return -1;
 result = DeeLexer_IncludeStringObjectEx(self,filedata,_dee_default_string_filename);
 Dee_DECREF(filedata);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_IncludeFileStreamEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self,
 DEE_A_INOUT_OBJECT(DeeFileObject) *filestream,
 DEE_A_IN_Z char const *disp_filename) {
 int result; DeeObject *filedata;
 if (DeeFileIO_Check(filestream)) {
  if ((filedata = DeeUtf8String_Cast(DeeFileIO_FILE(filestream))) == NULL) return -1;
  result = _DeeLexer_IncludeFileStreamWithRealName(
   self,filestream,DeeUtf8String_STR(filedata),disp_filename);
  Dee_DECREF(filedata);
  return result;
 }
 if ((filedata = DeeFile_ReadData(filestream,(Dee_size_t)-1)) == NULL) return -1;
 result = DeeLexer_IncludeStringObjectEx(self,filedata,disp_filename);
 Dee_DECREF(filedata);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_Utf8IncludeFilename(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self,
 DEE_A_IN_Z Dee_Utf8Char const *filename) {
 return DeeLexer_Utf8IncludeFilenameEx(self,filename,filename);
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_Utf8IncludeFilenameEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self,
 DEE_A_IN_Z Dee_Utf8Char const *filename,
 DEE_A_IN_Z char const *disp_filename) {
 DeeObject *file_ob; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 DEE_ASSERT(filename);
 if ((file_ob = DeeFileIO_Utf8New(filename,"r")) == NULL) return -1;
 temp = _DeeLexer_IncludeFileStreamWithRealName(self,file_ob,filename,disp_filename);
 Dee_DECREF(file_ob);
 return temp;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_WideIncludeFilename(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self,
 DEE_A_IN_Z Dee_WideChar const *filename) {
 DeeObject *file_ob,*utf8_filename; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 DEE_ASSERT(filename);
 if ((utf8_filename = DeeUtf8String_FromWideString(filename)) == NULL) return -1;
 if ((file_ob = DeeFileIO_WideNew(filename,"r")) == NULL) { Dee_DECREF(utf8_filename); return -1; }
 temp = _DeeLexer_IncludeFileStreamWithRealName(
  self,file_ob,DeeUtf8String_STR(utf8_filename),DeeUtf8String_STR(utf8_filename));
 Dee_DECREF(utf8_filename);
 Dee_DECREF(file_ob);
 return temp;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_WideIncludeFilenameEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self,
 DEE_A_IN_Z Dee_WideChar const *filename,
 DEE_A_IN_Z char const *disp_filename) {
 DeeObject *file_ob,*utf8_filename; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 DEE_ASSERT(filename);
 if ((utf8_filename = DeeUtf8String_FromWideString(filename)) == NULL) return -1;
 if ((file_ob = DeeFileIO_WideNew(filename,"r")) == NULL) { Dee_DECREF(utf8_filename); return -1; }
 temp = _DeeLexer_IncludeFileStreamWithRealName(
  self,file_ob,DeeUtf8String_STR(utf8_filename),disp_filename);
 Dee_DECREF(utf8_filename);
 Dee_DECREF(file_ob);
 return temp;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_IncludeFilenameObject(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename) {
 DEE_ASSERT(DeeObject_Check(filename) && DeeString_Check(filename));
 return DeeLexer_IncludeFilenameObjectEx(self,filename,DeeString_STR(filename));
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_IncludeFilenameObjectEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename,
 DEE_A_IN_Z char const *disp_filename) {
 DeeObject *file_ob; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 DEE_ASSERT(DeeObject_Check(filename) && DeeString_Check(filename));
 if ((file_ob = DeeFileIO_NewObject((DeeObject *)filename,"r")) == NULL) return -1;
 temp = _DeeLexer_IncludeFileStreamWithRealName(
  self,file_ob,DeeString_STR(filename),disp_filename);
 Dee_DECREF(file_ob);
 return temp;
}

DEE_A_RET_EXCEPT(-1) int DeeLexer_IncludeString(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z char const *code) {
 return DeeLexer_IncludeStringEx(self,code,_dee_default_string_filename);
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_IncludeStringObject(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeStringObject) const *code) {
 return DeeLexer_IncludeStringObjectEx(self,code,_dee_default_string_filename);
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_IncludeStringWithLength(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z char const *code, DEE_A_IN Dee_size_t code_length) {
 return DeeLexer_IncludeStringWithLengthEx(self,code,code_length,_dee_default_string_filename);
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_IncludeStringEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self,
 DEE_A_IN_Z char const *code,
 DEE_A_IN_Z char const *disp_filename) {
 DEE_ASSERT(code);
 return DeeLexer_IncludeStringWithLengthEx(self,code,strlen(code),disp_filename);
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_IncludeStringObjectEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *code,
 DEE_A_IN_Z char const *disp_filename) {
 DEE_ASSERT(DeeObject_Check(code) && DeeString_Check(code));
 return DeeLexer_IncludeStringWithLengthEx(
  self,DeeString_STR(code),DeeString_SIZE(code),disp_filename);
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_IncludeStringWithLengthEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z char const *code,
 DEE_A_IN Dee_size_t code_length, DEE_A_IN_Z char const *disp_filename) {
 TPPFileObject *tpp_file;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 if ((tpp_file = TPPFile_NewFromStringEx(
  (((DeeLexerObject *)self)->l_lexer.l_flags&TPPLexer_FLAG_TOK_COMMENTS)!=0
  ?TPPFileFormatFlag_KEEP_COMMENTS:TPPFileFormatFlag_ENABLED,code,code_length)
  ) == NULL) return -1;
 if (TPPFile_SetNewName(tpp_file,disp_filename) != 0) { TPPFile_DECREF(tpp_file); return -1; }
 TPPLexer_PushFileInheritedNoErr(DeeLexer_LEXER(self),tpp_file);
 return _DeeLexer_KickStart((DeeLexerObject *)self);
}


DEE_A_RET_OBJECT_EXCEPT(DeeTokenObject) *DeeLexer_Yield(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self) {
 DeeTokenObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 if (((DeeLexerObject *)self)->l_future.ftl_tokenc) {
  Dee_DECREF(((DeeLexerObject *)self)->l_token);
  result = ((DeeLexerObject *)self)->l_token =
   ((DeeLexerObject *)self)->l_future.ftl_tokenv[
    --((DeeLexerObject *)self)->l_future.ftl_tokenc]; // Transfer reference
#ifdef DEE_DEBUG
  ((DeeLexerObject *)self)->l_future.ftl_tokenv[
   ((DeeLexerObject *)self)->l_future.ftl_tokenc] = NULL;
#endif
 } else {
  result = (DeeTokenObject *)DeeLexer_TOKEN(self);
  if (!DeeObject_IS_UNIQUE(result)) {
   // Create a new token (last one is used by someone else...)
   if ((result = (DeeTokenObject *)DeeToken_New()) == NULL) return NULL;
   Dee_DECREF(((DeeLexerObject *)self)->l_token);
   ((DeeLexerObject *)self)->l_token = result;
  }
  if (_DeeLexer_BuiltinNext(DeeLexer_LEXER(self),DeeToken_TOKEN(result)) != 0) return NULL;
 }
 ++((DeeLexerObject *)self)->l_token_pos;
 return (DeeObject *)result;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_Return(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_INOUT_OBJECT(DeeTokenObject) *token) {
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 if (((DeeLexerObject *)self)->l_future.ftl_tokena >
     ((DeeLexerObject *)self)->l_future.ftl_tokenc) {
  Dee_INHERIT_REF(((DeeLexerObject *)self)->l_future.ftl_tokenv[
   ((DeeLexerObject *)self)->l_future.ftl_tokenc++],((DeeLexerObject *)self)->l_token);
 } else {
  Dee_size_t new_future_size = ((DeeLexerObject *)self)->l_future.ftl_tokena+1;
  DeeTokenObject **new_future;
  while ((new_future = (DeeTokenObject **)realloc_nz(((
   DeeLexerObject *)self)->l_future.ftl_tokenv,
   new_future_size*sizeof(DeeTokenObject *))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return -1;
  }
  Dee_INHERIT_REF(new_future[new_future_size-1],((DeeLexerObject *)self)->l_token);
  ((DeeLexerObject *)self)->l_future.ftl_tokenv = new_future;
  ((DeeLexerObject *)self)->l_future.ftl_tokena = new_future_size;
  ((DeeLexerObject *)self)->l_future.ftl_tokenc = new_future_size;
 }
 Dee_INCREF(((DeeLexerObject *)self)->l_token = (DeeTokenObject *)token);
 --((DeeLexerObject *)self)->l_token_pos;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_ReturnTPPToken(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN TPPTokenObject const *token) {
 DeeObject *tk; int temp;
 if ((tk = DeeToken_NewFromTPPToken(token)) == NULL) return -1;
 temp = DeeLexer_Return(self,tk);
 Dee_DECREF(tk);
 return temp;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_SkipTokensUntil(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_INOUT_OBJECT(DeeScopeObject) *scope,
 DEE_A_IN struct DeeCompilerConfig const *config, DEE_A_IN TPPTokenID end_token_1,
 DEE_A_IN TPPTokenID end_token_2) {
 TPPTokenID cur_id;
 int paren_recursion;
 int bracket_recursion;
 int brace_recursion;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 DEE_ASSERT(DeeObject_Check(scope) && DeeScope_Check(scope));
 paren_recursion = bracket_recursion = brace_recursion = 0;
 while (1) {
  cur_id = ((DeeLexerObject *)self)->l_token->tk_token.tk_id;
  if ((cur_id == end_token_1 || cur_id == end_token_2) &&
      !paren_recursion && !bracket_recursion && !brace_recursion) break;
  switch (cur_id) {
   case 0: return 0; // Always stop for this
   case '(': ++paren_recursion; break;
   case ')': --paren_recursion; break;
   case '[': ++bracket_recursion; break;
   case ']': --bracket_recursion; break;
   case '{': ++brace_recursion; break;
   case '}': --brace_recursion; break;
   default: break;
  }
  if (!DeeLexer_YieldEx(self,scope,config)) return -1;
 }
 return 0;
}
extern DEE_A_RET_EXCEPT(-1) int DeeLexer_SetFunctionNameStringWithLength(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN Dee_size_t s, DEE_A_IN_R(s) char const *name) {
 TPPTokenID new_function_name;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 DEE_ASSERT(name);
 if ((new_function_name = TPPKeywordList_GetS(
  &DeeLexer_LEXER(self)->l_keywords,name,s,1)) == 0) return -1;
 ((DeeLexerObject *)self)->l_func_name = new_function_name;
 return 0;
}

static int _DeeLexer_SkipBraceBlock(DeeLexerObject *self, DeeTokenObject *start_token) {
 Dee_int32_t needed_recursion,current_recursion;
 needed_recursion = (current_recursion = self->l_brace_recursion)-1;
 // Skip block
 while (1) {
  switch (self->l_token->tk_token.tk_id) {
   case 0:
    if (DeeError_CompilerError(DEE_WARNING_STATIC_IF_BLOCK_NEVER_CLOSED,
     (DeeObject *)self,(DeeObject *)start_token,
     "__static_if-block never closed") != 0) return -1;
    return 0; // EOF
   case '{': ++current_recursion; break;
   case '}':
    if (--current_recursion == needed_recursion) {
     // Done (only consume the '}' token)
     if (!DeeLexer_Yield((DeeObject *)self)) return -1;
     self->l_brace_recursion = needed_recursion;
     return 0;
    }
   default: break;
  }
  if (!DeeLexer_Yield((DeeObject *)self)) return -1;
 }
}

DEE_A_RET_OBJECT_EXCEPT(DeeTokenObject) *DeeLexer_YieldEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self,
 DEE_A_INOUT_OBJECT(DeeScopeObject) *scope,
 DEE_A_IN struct DeeCompilerConfig const *config) {
 DeeXAstObject *const_ast;
 DeeTokenObject *result; int static_if_value;
#if DEE_CONFIG_LANGUAGE_HAVE_IF_EXISTS
 int static_else_mode; // 0: static_else; 1: __if_exists; 2: __if_not_exists
#endif
#ifdef DEE_DEBUG
 DeeLexerObject *_self = (DeeLexerObject *)self;
#else
#define _self ((DeeLexerObject *)self)
#endif

#ifdef DEE_DEBUG
#define RETURN(x)\
do{\
 if (x) DEE_LVERBOSE4R(\
  "%s(%d) : TOKEN : %R\n",\
  DeeToken_FILE(x),\
  DeeToken_LINE(x),\
  DeeToken_Str(x));\
 return x;\
}while(0)
#else
#define RETURN(x) return x
#endif
 

 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
//again:
 if ((result = (DeeTokenObject *)DeeLexer_Yield(self)) != NULL) {
parse_result:
  switch (result->tk_token.tk_id) {

   case 0: { // EOF
    if (DEE_UNLIKELY(_self->l_static_if_stack.si_c)) {
     struct DeeStaticIfStackEntry *begin,*iter;
     iter = (begin = _self->l_static_if_stack.si_v)+_self->l_static_if_stack.si_c;
     do { --iter;
      if (DeeError_CompilerError(DEE_WARNING_UNCLOSED_STATIC_IF_BLOCK,
       (DeeObject *)self,(DeeObject *)iter->sie_tok,
       "Unclosed static-if block") != 0) goto err_r;
     } while (iter != begin);
     DeeStaticIfStack_Clear(&_self->l_static_if_stack);
    }
   } break;

   case '{':
    ++_self->l_brace_recursion;
    break;
   case '}':
    --_self->l_brace_recursion;
    if (!DeeStaticIfStack_EMPTY(&_self->l_static_if_stack) &&
        _self->l_brace_recursion == DeeStaticIfStack_TOP(
        &_self->l_static_if_stack)->sie_brace_recursion) {
     if (!DeeLexer_Yield(self)) return NULL;
after_rparen:
     if (_self->l_token->tk_token.tk_id == KWD_static) {
      if ((DeeStaticIfStack_TOP(&_self->l_static_if_stack)->sie_value & 2) == 0) {
       Dee_INCREF(result = _self->l_token);
       if (!DeeLexer_Yield(self)) goto err_r;
       switch (_self->l_token->tk_token.tk_id) {
        case KWD_else: goto do_else_y1;
#if DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS
        case KWD_elif: goto do_elif_y1;
#endif
        default: break;
       }
pop_and_return_current:
       DeeStaticIfStack_POP(&_self->l_static_if_stack);
       if (DeeLexer_Return(self,(DeeObject *)_self->l_token) != 0) goto err_r;
       Dee_DECREF(_self->l_token);
       _self->l_token = result; // Inherit reference
       goto parse_result; // Parse again
      }
     } else if (_self->l_token->tk_token.tk_id == KWD_else ||
                _self->l_token->tk_token.tk_id == KWD___static_else) {
      if ((DeeStaticIfStack_TOP(&_self->l_static_if_stack)->sie_value & 2) == 0) {
       Dee_INCREF(result = _self->l_token);
do_else_y1:
       if (!DeeLexer_Yield(self)) goto err_r;
/*do_else:*/
       if (_self->l_token->tk_token.tk_id == KWD_static) {
        if (!DeeLexer_Yield(self)) goto err_r;
        if (_self->l_token->tk_token.tk_id != KWD_if) {
         if (DeeError_CompilerError(DEE_WARNING_EXPECTED_IF_AFTER_STATIC_IF_ELSE_STATIC,(DeeObject *)self,(DeeObject *)_self->l_token,
          "Expected 'if' after '__static_if' '(' expr ')' '{' ... '}' 'else' 'static'") != 0) goto err_r;
        } else if (!DeeLexer_Yield(self)) goto err_r;
        goto do_elif;
       }
#if DEE_CONFIG_LANGUAGE_HAVE_IF_EXISTS
       if (_self->l_token->tk_token.tk_id == KWD___if_exists) {
        if (!DeeLexer_Yield(self)) goto err_r;
        static_else_mode = 1;
        goto do_elif_now;
       }
       if (_self->l_token->tk_token.tk_id == KWD___if_not_exists) {
        if (!DeeLexer_Yield(self)) goto err_r;
        static_else_mode = 2;
        goto do_elif_now;
       }
#endif
       if (_self->l_token->tk_token.tk_id == KWD_if ||
           _self->l_token->tk_token.tk_id == KWD___static_if) {
        int always_skip;
#if DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS
do_elif_y1:
#endif
        if (!DeeLexer_Yield(self)) goto err_r;
do_elif:
#if DEE_CONFIG_LANGUAGE_HAVE_IF_EXISTS
        static_else_mode = 0;
do_elif_now:
#endif
        static_if_value = DeeStaticIfStack_TOP(&_self->l_static_if_stack)->sie_value;
        if (_self->l_token->tk_token.tk_id != '(') {
         if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_STATIC_ELSE_IF,(DeeObject *)self,(DeeObject *)_self->l_token,
          "Expected '(' after '__static_if' '(' expr ')' '{' ... '}' 'else' 'if'") != 0) goto err_r;
        } else if (!DeeLexer_Yield(self)) goto err_r;
        always_skip = static_if_value == 1;
#if DEE_CONFIG_LANGUAGE_HAVE_IF_EXISTS
        switch (static_else_mode) {
         case 1: case 2:
          if (always_skip) DeeLexer_WSUPPRESS_BEGIN(_self);
          static_if_value = DeeXAst_ParseExistsExpression(
           _self,(DeeScopeObject *)scope,config,DEE_PARSER_FLAG_GENERATE_CONSTANT);
          if (always_skip) DeeLexer_WSUPPRESS_END(_self);
          if (static_if_value < 0) goto err_r;
          if (static_else_mode == 2) static_if_value = !static_if_value;
          break;
         default:
#endif
          if (always_skip) DeeLexer_WSUPPRESS_BEGIN(_self);
          if ((const_ast = DeeXAst_ParseSingle((
           DeeLexerObject *)self,(DeeScopeObject *)scope,config,
           DEE_PARSER_FLAG_GENERATE_CONSTANT)) == NULL) {
           if (always_skip) DeeLexer_WSUPPRESS_END(_self);
           goto err_r;
          }
          if (always_skip) {
           DeeLexer_WSUPPRESS_END(_self);
          } else if (const_ast->ast_kind == DEE_XASTKIND_CONST) {
           if ((static_if_value = DeeObject_Bool(const_ast->ast_const.c_const)) < 0) {
            DeeError_Handled();
            if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_BOOL_EXPRESSION_AFTER_STATIC_ELSE_IF,
             (DeeObject *)self,(DeeObject *)const_ast->ast_common.ast_token,
             "Expected constant bool expression after '__static_if' '(' expr ')' '{' ... '}' 'else' 'if' '(' (got: %s)",
             DeeType_NAME(Dee_TYPE(const_ast->ast_const.c_const))) != 0) goto err_castr;
            static_if_value = 0;
           }
          } else {
           if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_STATIC_ELSE_IF,
            (DeeObject *)self,(DeeObject *)const_ast->ast_common.ast_token,
            "Expected constant expression after '__static_if' '(' expr ')' '{' ... '}' 'else' 'if' '('"
            ) != 0) {err_castr: Dee_DECREF(const_ast); goto err_r;}
           static_if_value = 0;
          }
          Dee_DECREF(const_ast);
#if DEE_CONFIG_LANGUAGE_HAVE_IF_EXISTS
         break;
        }
#endif
        DeeStaticIfStack_TOP(&_self->l_static_if_stack)->sie_value = static_if_value;
        if (_self->l_token->tk_token.tk_id != ')') {
         if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_STATIC_ELSE_IF,
          (DeeObject *)self,(DeeObject *)_self->l_token,
          "Expected ')' after '__static_if' '(' expr ')' '{' ... '}' 'else' 'if' '(' <expr>"
          ) != 0) goto err_r;
        } else if (!DeeLexer_Yield(self)) goto err_r;
        if (_self->l_token->tk_token.tk_id != '{') {
         if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LBRACE_AFTER_STATIC_ELSE_IF,
          (DeeObject *)self,(DeeObject *)_self->l_token,
          "Expected '{' after '__static_if' '(' <expr> ')' '{' ... '}' 'else' 'if' '(' <expr> ')'"
          ) != 0) goto err_r;
         goto drop_res_and_ret_stored;
        } else if (!DeeLexer_Yield(self)) goto err_r;
        if (always_skip) { ++_self->l_brace_recursion; goto do_skip; }
        goto check_skip;
       } else {
        if (_self->l_token->tk_token.tk_id != '{') {
         if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LBRACE_AFTER_STATIC_IF_ELSE,
          (DeeObject *)self,(DeeObject *)_self->l_token,
          "Expected '{' after '__static_if' '(' <expr> ')' '{' ... '}' 'else'") != 0) goto err_r;
         goto pop_and_return_current;
        } else if (!DeeLexer_Yield(self)) goto err_r;
        static_if_value = (DeeStaticIfStack_TOP(&_self->l_static_if_stack)->sie_value ^= 3);
        goto check_skip;
       }
      } else {
       // another else block was already found --> emit this else as a regular token
      }
     } else if ((
      _self->l_token->tk_token.tk_id == KWD___static_elif
#if DEE_CONFIG_LANGUAGE_HAVE_ELIF_ELSE_IF_ALIAS
      || _self->l_token->tk_token.tk_id == KWD_elif
#endif
      ) && (DeeStaticIfStack_TOP(&_self->l_static_if_stack)->sie_value & 2) == 0) {
      Dee_INCREF(result = _self->l_token);
      goto do_elif_y1;
     }
     DeeStaticIfStack_POP(&_self->l_static_if_stack);
     result = _self->l_token;
     goto parse_result;
    }
    break;

#if DEE_CONFIG_LANGUAGE_HAVE_IF_EXISTS
   case KWD___if_exists:
   case KWD___if_not_exists: {
    int inverse,temp;
    inverse = result->tk_token.tk_id == KWD___if_not_exists;
    Dee_INCREF(result);
    if (!DeeLexer_Yield(self)) goto err_r;
    if (_self->l_token->tk_token.tk_id != '(') {
     if (DeeError_CompilerErrorf(inverse
      ? DEE_WARNING_EXPECTED_LPAREN_AFTER_IF_NOT_EXISTS
      : DEE_WARNING_EXPECTED_LPAREN_AFTER_IF_EXISTS
      ,(DeeObject *)self,(DeeObject *)_self->l_token,
      "Expected '(' after '%s'",inverse ? "__if_not_exists" : "__if_exists"
      ) != 0) goto err_r;
    } else if (!DeeLexer_Yield(self)) goto err_r;
    if ((temp = DeeXAst_ParseExistsExpression(
     _self,(DeeScopeObject *)scope,config,
     DEE_PARSER_FLAG_GENERATE_CONSTANT)) < 0) goto err_r;
    static_if_value = inverse^temp;
    if (_self->l_token->tk_token.tk_id != ')') {
     if (DeeError_CompilerErrorf(inverse
      ? DEE_WARNING_EXPECTED_RPAREN_AFTER_IF_NOT_EXISTS
      : DEE_WARNING_EXPECTED_RPAREN_AFTER_IF_EXISTS
      ,(DeeObject *)self,(DeeObject *)_self->l_token,
      "Expected ')' after '%s' '(' <exists_expr>",inverse ? "__if_not_exists" : "__if_exists"
      ) != 0) goto err_r;
    } else if (!DeeLexer_Yield(self)) goto err_r;
    if (_self->l_token->tk_token.tk_id != '{') {
     if (DeeError_CompilerErrorf(inverse
      ? DEE_WARNING_EXPECTED_LBRACE_AFTER_IF_NOT_EXISTS
      : DEE_WARNING_EXPECTED_LBRACE_AFTER_IF_EXISTS
      ,(DeeObject *)self,(DeeObject *)_self->l_token,
      "Expected '{' after '%s' '(' <exists_expr> ')'",inverse ? "__if_not_exists" : "__if_exists"
      ) != 0) goto err_r;
     goto drop_res_and_ret_stored;
    } else if (!DeeLexer_Yield(self)) goto err_r;
    goto push_si;
   } break;
#endif

   case KWD___static_if: {
     // '__static_if' '(' expr ')'
//do_si:
    Dee_INCREF(result);
do_si_inherit_ref:
    if (!DeeLexer_Yield(self)) goto err_r;
    if (_self->l_token->tk_token.tk_id != '(') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_STATIC_IF,(DeeObject *)self,(DeeObject *)_self->l_token,
      "Expected '(' after '__static_if'") != 0) goto err_r;
    } else if (!DeeLexer_Yield(self)) goto err_r;
    if ((const_ast = DeeXAst_ParseSingle((
     DeeLexerObject *)self,(DeeScopeObject *)scope,config,
     DEE_PARSER_FLAG_GENERATE_CONSTANT)) == NULL) goto err_r;
    if (const_ast->ast_kind == DEE_XASTKIND_CONST) {
     if ((static_if_value = DeeObject_Bool(const_ast->ast_const.c_const)) < 0) {
      DeeError_Handled();
      if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_CONSTANT_BOOL_EXPRESSION_AFTER_STATIC_IF,
       (DeeObject *)self,(DeeObject *)const_ast->ast_common.ast_token,
       "Expected constant bool expression after '__static_if' '(' (got: %s)",
       DeeType_NAME(Dee_TYPE(const_ast->ast_const.c_const))) != 0) goto err_castr;
      static_if_value = 0;
     }
    } else {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_CONSTANT_EXPRESSION_AFTER_STATIC_IF,
      (DeeObject *)self,(DeeObject *)const_ast->ast_common.ast_token,
      "Expected constant expression after '__static_if' '('") != 0) goto err_castr;
     static_if_value = 0;
    }
    Dee_DECREF(const_ast);
    if (_self->l_token->tk_token.tk_id != ')') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LPAREN_AFTER_STATIC_IF,(DeeObject *)self,(DeeObject *)_self->l_token,
      "Expected ')' after '__static_if' '(' <expr>") != 0) goto err_r;
    } else if (!DeeLexer_Yield(self)) goto err_r;
    if (_self->l_token->tk_token.tk_id != '{') {
     if (DeeError_CompilerError(DEE_WARNING_EXPECTED_LBRACE_AFTER_STATIC_IF,(DeeObject *)self,(DeeObject *)_self->l_token,
      "Expected '{' after '__static_if' '(' <expr> ')'") != 0) goto err_r;
drop_res_and_ret_stored:
     Dee_DECREF(result);
     RETURN((DeeObject *)_self->l_token);
    } else if (!DeeLexer_Yield(self)) goto err_r;
push_si:
    if (DeeStaticIfStack_Push(&_self->l_static_if_stack,
     result,static_if_value,_self->l_brace_recursion) != 0) goto err_r;
check_skip:
    ++_self->l_brace_recursion;
    if (!(static_if_value & 1)) {
do_skip:
     if (_DeeLexer_SkipBraceBlock(_self,result) != 0) goto err_r;
     Dee_DECREF(result);
     goto after_rparen;
    } else {
     Dee_DECREF(result);
     result = _self->l_token;
     goto parse_result;
    }
   } break;

   case KWD_static: {
    DeeTokenObject *tok2;
    Dee_INCREF(result);
    if ((tok2 = (DeeTokenObject *)DeeLexer_Yield(self)) == NULL) {
err_r: Dee_DECREF(result); return NULL;
    }
    if (tok2->tk_token.tk_id == KWD_if) {
     // 'static' 'if' '(' expr ')'
     goto do_si_inherit_ref;
    } else {
     if (DeeLexer_Return(self,(DeeObject *)tok2) != 0) goto err_r;
/*restore_result:*/
     Dee_DECREF(_self->l_token);
     _self->l_token = result; // Inherit reference
    }
   } break;

   case KWD___static_else:
   case KWD___static_elif:
    // Warn about special keywords
    if (DeeError_CompilerError(DEE_WARNING_STATIC_IF_SPECIFIC_KEYWORD_USED_OUT_OF_CONTEXT,
     (DeeObject *)self,(DeeObject *)_self->l_token,
     "static-if specific keyword parsed as regular keyword (is this intended?)"
     ) != 0) return NULL;
    break;

   default: break;
  }
 }
 RETURN((DeeObject *)result);
#undef RETURN
#ifdef _self
#undef _self
#endif
}

#define DEE_PRIVATE_MAKE_LEXER_STRING_PARSE(enc,tpp_kind)\
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(Dee##enc##StringObject) *)\
_DeeLexer_Parse##enc##String(DEE_A_INOUT_OBJECT(DeeLexerObject) *self) {\
 DeeObject *result;\
 Dee##enc##StringWriter writer = Dee##enc##StringWriter_INIT();\
 while (1) {\
  if (DeeToken_ID(DeeLexer_TOKEN(self)) == KWD___FUNCTION__) {\
   char const *func_name;\
   func_name = TPPLexer_TokenIDStr(DeeLexer_LEXER(self),((DeeLexerObject *)self)->l_func_name);\
   if (!func_name) func_name = "__unnamed__";\
   if (Dee##enc##StringWriter_WriteUtf8String(&writer,func_name) != 0) goto err;\
  } else {\
   Dee_##enc##Char *buffer;\
   Dee_size_t len = TPPToken_EvalStr(DeeToken_TOKEN(DeeLexer_TOKEN(self)),NULL,0);\
   if ((buffer = Dee##enc##StringWriter_Require(&writer,len/sizeof(Dee_##enc##Char))) == NULL) goto err;\
   TPPToken_EvalStr(DeeToken_TOKEN(DeeLexer_TOKEN(self)),(char *)buffer,len);\
  }\
  if (!DeeLexer_Yield(self)) goto err;\
  if (DeeToken_ID(DeeLexer_TOKEN(self)) == TPP_TOK_STR) {\
   if (TPPToken_GetStringType(DeeToken_TOKEN(DeeLexer_TOKEN(self))) != tpp_kind) break;\
  } else if (DeeToken_ID(DeeLexer_TOKEN(self)) != KWD___FUNCTION__) break;\
  \
 }\
 result = Dee##enc##StringWriter_Pack(&writer);\
end: Dee##enc##StringWriter_Quit(&writer);\
 return result;\
err: result = NULL; goto end;\
}
#if DEE_CONFIG_HAVE_ENCODING_WIDE
DEE_PRIVATE_MAKE_LEXER_STRING_PARSE(Wide,TPPStringType_WCHAR)
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF8
DEE_PRIVATE_MAKE_LEXER_STRING_PARSE(Utf8,TPPStringType_CHAR)
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
DEE_PRIVATE_MAKE_LEXER_STRING_PARSE(Utf16,TPPStringType_CHAR16)
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
DEE_PRIVATE_MAKE_LEXER_STRING_PARSE(Utf32,TPPStringType_CHAR32)
#endif
#undef DEE_PRIVATE_MAKE_LEXER_STRING_PARSE


DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *
DeeLexer_ParseString(DEE_A_INOUT_OBJECT(DeeLexerObject) *self) {
 TPPStringType str_kind = 0;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 if (DeeToken_ID(DeeLexer_TOKEN(self)) == KWD___FUNCTION__) {
  str_kind = TPPStringType_CHAR;
 } else str_kind = TPPToken_GetStringType(DeeToken_TOKEN(DeeLexer_TOKEN(self)));
 switch (str_kind) {
#if DEE_CONFIG_HAVE_ENCODING_WIDE
  case TPPStringType_WCHAR:
   return _DeeLexer_ParseWideString(self);
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
  case TPPStringType_CHAR16:
   return _DeeLexer_ParseUtf16String(self);
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
  case TPPStringType_CHAR32:
   return _DeeLexer_ParseUtf32String(self);
#endif
  default:
   return DEE_PP_CAT_3(_DeeLexer_Parse,DEE_CONFIG_DEFAULT_ENCODING,String)(self);
 }
}


DeeString_NEW_STATIC(_dee_main_name,"__main__");

DEE_COMPILER_MSVC_WARNING_PUSH(4701)
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeFunctionObject) *DeeLexer_ParseAndCompile(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self,
 DEE_A_IN_OPT struct DeeCompilerConfig const *config) {
 TPPTokenID old_func_name;
 DeeSAstObject *root_ast;
 DeeScopeObject *root_scope;
 DeeCodeObject *code;
 DeeFunctionObject *result;
 unsigned int used_compiler_flags,function_flags;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 if (!config) config = &DeeCompilerConfig_Default;
 DEE_ASSERT((!config->main_name || (DeeObject_Check(config->main_name) &&
  DeeString_Check(config->main_name))) && "main_name is neither NULL, nor a string");
 old_func_name = ((DeeLexerObject *)self)->l_func_name;
 if (config->main_name) {
  if DEE_UNLIKELY(DeeLexer_SetFunctionNameStringWithLength(
   self,DeeString_SIZE(config->main_name),DeeString_STR(config->main_name)) != 0) goto err_0;
 } else {
  ((DeeLexerObject *)self)->l_func_name = KWD___main__;
 }
 if DEE_UNLIKELY((root_scope = (DeeScopeObject *)DeeScope_NewRoot()) == NULL) goto err_0;
 {
  DeeLocalVarObject *vararg_var; int temp;
  if DEE_UNLIKELY((vararg_var = (DeeLocalVarObject *)DeeLocalVar_New(
   TPP_TOK_DOTS,(DeeObject *)root_scope)) == NULL) goto err_1;
  vararg_var->lv_loc_id = 0;
  vararg_var->lv_flags = (DEE_LOCALVAR_FLAG_INITIALIZED|DEE_LOCALVAR_KIND_PARAM);
  temp = _DeeScope_AddName(root_scope,TPP_TOK_DOTS,(DeeObject *)vararg_var);
  Dee_DECREF(vararg_var);
  if DEE_UNLIKELY(temp != 0) goto err_1;
 }
 {
  DeeTokenObject *first_token;
  Dee_INCREF(first_token = (DeeTokenObject *)DeeLexer_TOKEN(self));
  root_ast = DeeSAst_ParseAllInOldScopeUntil(first_token,TPP_TOK_EOF,DeeParserLabelRefList_Empty,
                                            (DeeLexerObject *)self,root_scope,config,config->parser_flags);
  Dee_DECREF(first_token);
  if DEE_UNLIKELY(!root_ast) goto err_1;
 }
 if DEE_UNLIKELY(DeeLexer_FAILED(self)) { DeeError_Throw(((DeeLexerObject *)self)->l_error_last); goto err_2; }
 if ((root_scope->sc_flags&(DEE_SCOPE_FLAG_FOUND_RETURN|
  DEE_SCOPE_FLAG_FOUND_YIELD|DEE_SCOPE_FLAG_FOUND_RETVAR))!=0) {
  // Don't return the last statement if there are any explicit yield/return statements
  used_compiler_flags = config->compiler_flags&~(DEE_COMPILER_FLAG_USED);
 } else {
  used_compiler_flags = config->compiler_flags;
 }
 // Optimize the generated code (second pass)
 if (config->optimize_flags != DEE_OPTIMIZE_FLAG_NONE) {
  struct DeeOptimizerAssumptions assumptions; int error;
  Dee_size_t n_performed; Dee_uint32_t used_optimize_flags;
  used_optimize_flags = (Dee_uint32_t)(
   (used_compiler_flags&DEE_COMPILER_FLAG_USED) != 0
   ? (config->optimize_flags|DEE_OPTIMIZE_FLAG_USED)
   : (config->optimize_flags&~(DEE_OPTIMIZE_FLAG_USED)));
  do { // Optimize until there's nothing left to optimize
   n_performed = 0;
   _DeeOptimizerAssumptions_InitRoot(&assumptions,(DeeScopeObject *)root_scope);
   error = DeeSAst_Optimize(root_ast,&n_performed,(DeeLexerObject *)self,
                            (DeeScopeObject *)root_scope,config,
                            used_optimize_flags,&assumptions);
   _DeeOptimizerAssumptions_Quit(&assumptions);
   if DEE_UNLIKELY(error != 0) goto err_2;
  } while (n_performed);
 }
 {
  struct DeeCodeWriter writer = DeeCodeWriter_INIT();
  writer.cw_root_scope = root_scope;
  if DEE_UNLIKELY(DeeSAst_CompileStrongScopeRoot(root_ast,&writer,(
   DeeLexerObject *)self,root_scope,config,used_compiler_flags) != 0) goto err_3;
  DEE_ASSERTF(writer.cw_refc == 0,"How can the absolute root/global scope have references?");
  if (DeeLexer_FAILED(self)) { DeeError_Throw(((DeeLexerObject *)self)->l_error_last); goto err_3; }
  if (!DeeSAst_IsNoReturn(root_ast,DEE_AST_ATTRIBUTE_FLAG_NONE)) {
   // Generate the return value code
   if ((root_scope->sc_flags&DEE_SCOPE_FLAG_FOUND_YIELD)!=0) {
    if DEE_UNLIKELY(DeeCodeWriter_YieldExit(&writer) != 0) goto err_3; // YIELD_EXIT
   } else if ((used_compiler_flags&DEE_COMPILER_FLAG_USED)!=0) {
    if DEE_UNLIKELY(DeeCodeWriter_Ret(&writer) != 0) goto err_3; // RET_POP
   } else {
    if DEE_UNLIKELY(DeeCodeWriter_RetNone(&writer) != 0) goto err_3; // RET_NONE
   }
  } else {
   // Satisfy the writer, if the ast left behind an unreachable value
   if ((used_compiler_flags&DEE_COMPILER_FLAG_USED)!=0)
    DeeCodeWriter_DECSTACK(&writer);
   // End the code with an unreachable opcode (for safety)
   if ((used_compiler_flags&DEE_COMPILER_FLAG_GEN_UNREACHABLE)!=0 &&
       DEE_UNLIKELY(DeeCodeWriter_Unreachable(&writer) != 0)) goto err_3;
  }
  // Do some final optimizations (peephole & such...)
  if DEE_UNLIKELY(DeeCodeWriter_Optimize(&writer,used_compiler_flags) != 0) goto err_3;
  // All code is now generated --> Time to back everything together!
  if DEE_UNLIKELY((code = (DeeCodeObject *)DeeCodeWriter_Pack(&writer,
   config->main_name ? config->main_name : (DeeObject *)&_dee_main_name)) == NULL) goto err_3;
  function_flags = DEE_FUNCTION_FLAG_VARG;
  if ((root_scope->sc_flags&DEE_SCOPE_FLAG_FOUND_YIELD)!=0) function_flags |= DEE_FUNCTION_FLAG_YILD;
  if ((root_scope->sc_flags&DEE_SCOPE_FLAG_FOUND_THIS)!=0)  function_flags |= DEE_FUNCTION_FLAG_THIS;
  if DEE_UNLIKELY((result = (DeeFunctionObject *)DeeFunction_New(
   (DeeObject *)code,Dee_EmptyTuple,1,function_flags)) == NULL) goto err_4;

  // Cleanup time!
  Dee_DECREF(code);
  DeeCodeWriter_Quit(&writer);
  Dee_DECREF(root_ast);
  Dee_DECREF(root_scope);
  ((DeeLexerObject *)self)->l_func_name = old_func_name;
  return (DeeObject *)result;
  // Cleanup on error
err_4: Dee_DECREF(code);
err_3: DeeCodeWriter_Quit(&writer);
 }
err_2: Dee_DECREF(root_ast);
err_1: Dee_DECREF(root_scope);
err_0: ((DeeLexerObject *)self)->l_func_name = old_func_name;
 return NULL;
}
DEE_COMPILER_MSVC_WARNING_POP


//////////////////////////////////////////////////////////////////////////
// NOTE: Having this enabled will reduce the amount of line feeds, but
//       will also lead to incorrect token output, when preprocessing with "-tok"
#define REDUCE_LINE_FEEDS 0
DEE_A_RET_EXCEPT(-1) int DeeLexer_Preprocess(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self) {
 struct DeePreprocessConfig config; int result;
 config.output_file = DeeFile_Std(DEE_STDOUT);
 config.output_line_directives = 0;
 config.output_tok_brackets = 0;
 config.padding_0 = 0;
 config.padding_1 = 0;
 config.padding_2 = 0;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 result = DeeLexer_PreprocessEx(self,&config);
 Dee_DECREF(config.output_file);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_PreprocessEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN struct DeePreprocessConfig const *config) {
 char tok_buf[1024];
 Dee_size_t tok_size;
 TPPFileObject *last_token_end_file;
 char const *current_file_name,*last_token_end;
 int current_line_offset = 0;
 TPPLexerFlags old_flags;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 DEE_ASSERT(config);
 // Initialize the lexer
 // We need line-feeds + invisible line-feeds + whitespace
 old_flags = DeeLexer_LEXER(self)->l_flags;
 DeeLexer_LEXER(self)->l_flags |= (TPPLexer_FLAG_WANT_LF|TPPLexer_FLAG_WANT_SPC);
#define _token  (*DeeToken_TOKEN(DeeLexer_TOKEN(self)))
#if 1
 // Force a #line directive at the start of the output file
 current_file_name = NULL;
 last_token_end = NULL;
 last_token_end_file = NULL;
#else
 current_file_name = TPPFile_Name(lexer.l_files);
 last_token_end = lexer.l_files->f_data;
 last_token_end_file = lexer.l_files;
 TPPFile_INCREF(last_token_end_file);
#endif
 // Main loop
 while (1) {
#if REDUCE_LINE_FEEDS
next:
#endif /* REDUCE_LINE_FEEDS */
  // Must check first, as only EOF token are allowed, to have NULL files
  if (_token.tk_id == TPP_TOK_EOF) break;
  if (config->output_line_directives) {
   // Check for #line directives
   if (current_file_name != TPPFile_Name(_token.tk_file)) {
    current_file_name = TPPFile_Name(_token.tk_file);
    current_line_offset = _token.tk_file->f_line_offset;
    last_token_end = _token.tk_str_end;
    TPPFile_XDECREF(last_token_end_file);
    last_token_end_file = _token.tk_str_file;
    TPPFile_INCREF(last_token_end_file);
#if REDUCE_LINE_FEEDS
    if DEE_UNLIKELY(DeeFile_Printf(config->output_file,"\n#line %d %q\n",
     TPPToken_LineAfter(&_token)+1,current_file_name) != 0) goto err;
    if (token.tk_id == '\n') goto next;
#else /* REDUCE_LINE_FEEDS */
    if DEE_UNLIKELY(DeeFile_Printf(config->output_file,"\n#line %d %q\n",
     TPPToken_Line(&_token)+1,current_file_name) != 0) goto err;
#endif /* !REDUCE_LINE_FEEDS */
   } else if (current_line_offset != _token.tk_file->f_line_offset) {
    // Re-emit manual #line directives
    current_line_offset = _token.tk_file->f_line_offset;
    last_token_end = _token.tk_str_end;
    TPPFile_XDECREF(last_token_end_file);
    last_token_end_file = _token.tk_str_file;
    TPPFile_INCREF(last_token_end_file);
    goto emitl;
   } else if (last_token_end >= _token.tk_str_file->f_data &&
              last_token_end < _token.tk_str_file->f_end) {
    // Check for changes in the lines
    char const *iter = last_token_end;
    last_token_end = _token.tk_str_end;
    TPPFile_XDECREF(last_token_end_file);
    last_token_end_file = _token.tk_str_file;
    TPPFile_INCREF(last_token_end_file);
    while (iter != _token.tk_str_begin) {
     if (*iter++ == '\n') {
emitl:
#if REDUCE_LINE_FEEDS
      if DEE_UNLIKELY(DeeFile_Printf(config->output_file,
       "\n#line %d\n",TPPToken_LineAfter(&_token)+1) != 0) goto err;
      if (token.tk_id == '\n') goto next;
#else /* REDUCE_LINE_FEEDS */
      if DEE_UNLIKELY(DeeFile_Printf(config->output_file,
       "\n#line %d\n",TPPToken_Line(&_token)+1) != 0) goto err;
#endif /* !REDUCE_LINE_FEEDS */
      goto put;
     }
    }
   }
  }
put:
  // Convert the token into a string
  // >> prefer, to use a static buffer, but fall back to a dynamic
  //    one, if the static one isn't big enough
  if (config->output_tok_brackets && DEE_UNLIKELY(DeeFile_PRINT(config->output_file,"[") != 0)) goto err;
  tok_size = TPPToken_Str(&_token,tok_buf,sizeof(tok_buf));
  if (tok_size > sizeof(tok_buf)) {
   char *tok_buf2;
   while DEE_UNLIKELY((tok_buf2 = (char *)malloc_nz(tok_size*sizeof(char))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
    goto err;
   }
   TPPToken_Str(&_token,tok_buf2,tok_size);
   if DEE_UNLIKELY(DeeFile_WriteAll(config->output_file,tok_buf2,tok_size*sizeof(char)) != 0) goto err;
   free_nn(tok_buf2);
  } else {
   if DEE_UNLIKELY(DeeFile_WriteAll(config->output_file,tok_buf,tok_size*sizeof(char)) != 0) goto err;
  }
  if (config->output_tok_brackets && DEE_UNLIKELY(DeeFile_PRINT(config->output_file,"]") != 0)) goto err;
  if (config->output_line_directives) {
   if (_token.tk_id == '\n') {
    if (_token.tk_str_file != _token.tk_file) {
     // Emit a #line directive, if a line-feed token got emitted from a macro
     // >> @L+0 #define PUT(...) __VA_ARGS__
     // >> @L+1 PUT({
     // >> @L+2  int x = 42;
     // >> @L+3 })
     // >> @L+4 ...
     // >> // Output:
     // >> // >> {
     // >> // >> #line @L+1 // This is us!
     // >> // >>  int x = 42;
     // >> // >> #line @L+1 // This is us!
     // >> // >> } // NOTE: This is technically not correct, because the line-feed here should
     // >> //      //       come after #line @L+3. But there is no may, to express that, since
     // >> //      //       directives must start at the beginning of a line!
     // >> // >> #line @L+3
     if DEE_UNLIKELY(DeeFile_Printf(config->output_file,
      "#line %d\n",TPPToken_Line(&_token)+1) != 0) goto err;
    }
   } else {
    // This part is required, to emit line directives for escaped line feeds.
    char const *tok_iter,*tok_end;
    tok_iter = _token.tk_str_begin;
    tok_end = _token.tk_str_end;
    while (tok_iter != tok_end) {
     if (*tok_iter++ == '\n') {
      int n_line_feeds = 1;
      while (tok_iter != tok_end) {
       if (*tok_iter == '\n') ++n_line_feeds;
       ++tok_iter;
      }
      if DEE_UNLIKELY(DeeFile_Printf(config->output_file,
       "\n#line %d\n",1+TPPToken_Line(&_token)+n_line_feeds) != 0) goto err;
      break;
     }
    }
   }
  }
  // Read 1 token
  if DEE_UNLIKELY(!DeeLexer_Yield(self)) goto err;
 }
 // Cleanup
 TPPFile_XDECREF(last_token_end_file);
 DeeLexer_LEXER(self)->l_flags = old_flags;
 return 0;
err:
 TPPFile_XDECREF(last_token_end_file);
 DeeLexer_LEXER(self)->l_flags = old_flags;
 return -1;
}


DEE_A_RET_EXCEPT(-1) int DeeLexer_Utf8FormatFilename(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_Utf8Char const *filename) {
 DeeObject *filename_ob; int result;
 if DEE_UNLIKELY((filename_ob = DeeUtf8String_New(filename)) == NULL) return -1;
 result = DeeLexer_FormatFilenameObject(self,filename_ob);
 Dee_DECREF(filename_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_Utf8FormatFilenameEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_Utf8Char const *filename,
 DEE_A_IN struct DeeDeemonFormatConfig const *config) {
 DeeObject *filename_ob; int result;
 if DEE_UNLIKELY((filename_ob = DeeUtf8String_New(filename)) == NULL) return -1;
 result = DeeLexer_FormatFilenameObjectEx(self,filename_ob,config);
 Dee_DECREF(filename_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_WideFormatFilename(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_WideChar const *filename) {
 DeeObject *filename_ob; int result;
 if DEE_UNLIKELY((filename_ob = DeeWideString_New(filename)) == NULL) return -1;
 result = DeeLexer_FormatFilenameObject(self,filename_ob);
 Dee_DECREF(filename_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_WideFormatFilenameEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_Z Dee_WideChar const *filename,
 DEE_A_IN struct DeeDeemonFormatConfig const *config) {
 DeeObject *filename_ob; int result;
 if DEE_UNLIKELY((filename_ob = DeeWideString_New(filename)) == NULL) return -1;
 result = DeeLexer_FormatFilenameObjectEx(self,filename_ob,config);
 Dee_DECREF(filename_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_FormatFilenameObject(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename) {
 DeeObject *filestream; int result; // Must open in read/update mode
 if DEE_UNLIKELY((filestream = DeeFileIO_NewObject(filename,"r+")) == NULL) return -1;
 result = DeeLexer_FormatFileStream(self,filestream,filestream);
 Dee_DECREF(filestream);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_FormatFilenameObjectEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename,
 DEE_A_IN struct DeeDeemonFormatConfig const *config) {
 DeeObject *filestream; int result; // Must open in read/update mode
 if DEE_UNLIKELY((filestream = DeeFileIO_NewObject(filename,"r+")) == NULL) return -1;
 result = DeeLexer_FormatFileStreamEx(self,filestream,filestream,config);
 Dee_DECREF(filestream);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeLexer_FormatFileStream(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_INOUT_OBJECT(DeeFileObject) *in,
 DEE_A_INOUT_OBJECT(DeeFileObject) *out) {
 struct DeeDeemonFormatConfig config = {0,0,0,0};
 return DeeLexer_FormatFileStreamEx(self,in,out,&config);
}


DEE_STATIC_INLINE(void) _DeeToken_GetCommentText(
 DEE_A_IN TPPTokenObject *tok, DEE_A_OUT Dee_size_t *text_offset,
 DEE_A_OUT char const **text_begin, DEE_A_OUT char const **text_end) {
 char const *iter,*end;
 iter = tok->tk_str_begin,end = tok->tk_str_end;
#define NEXT() \
do{if(*iter++=='\\'&&(*iter=='\r'||*iter=='\n')){\
 if (*iter++ == '\r' && *iter == '\r') ++iter;\
}else break;}while(1)

 if (*iter == '/') {
  NEXT();
  if (*iter == '*') {
   end -= 2;
   while (1) {
    if (*end == '\n' && end[-1] == '\\') end -= 2;
    else if (*end == '\r' && end[-1] == '\\') end -= 2;
    else if (*end == '\n' && end[-1] == '\r' && end[-2] == '\\') end -= 3;
    else break;
   }
  }
  NEXT();
 } else if (*iter == '#') NEXT();
 if (iter[0] == '/' && iter[1] == '/') {
  iter += 2;
 } else if (iter[0] == '/' && iter[1] == '*') {
  iter += 2,end -= 2;
 } else if (*iter == '#') {
  ++iter;
 }
 DEE_ASSERT(iter <= end);
 *text_offset = tok->tk_file_off+(Dee_size_t)(iter-tok->tk_str_begin);
 *text_begin = iter;
 *text_end = end;
}


DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeObject *) _DeeLexer_CompileString(
 DEE_A_IN DeeLexerObject *self, char const *code_begin, char const *code_end,
 TPPFileObject *ref_file, Dee_size_t ref_file_offset) {
 TPPFileObject *code_file,*old_files; TPPLexerFlags old_flags;
 DeeObject *result; DeeTokenObject *old_token,*kick_token;
 unsigned int old_one_file_rec;
 if DEE_UNLIKELY((kick_token = (DeeTokenObject *)DeeToken_New()) == NULL) return NULL;
 if DEE_UNLIKELY((code_file = TPPFile_NewFromStringEx(
  TPPFileFormatFlag_ENABLED,code_begin,(Dee_size_t)(code_end-code_begin)
  )) == NULL) {err_ktok: Dee_DECREF(kick_token); return NULL; }
#if 1
 if (DEE_UNLIKELY(_TPPFile_SetName(code_file,ref_file->f_name) != 0) ||
    (ref_file->f_new_name && DEE_UNLIKELY(TPPFile_SetNewName(code_file,ref_file->f_new_name) != 0))
    ) { TPPFile_DECREF(code_file); goto err_ktok; }
 code_file->f_line_offset = TPPFile_LineAt(ref_file,ref_file_offset);
#else
 TPPFile_INCREF(ref_file);
 code_file->f_ref_file = ref_file;
 code_file->f_ref_file_off = ref_file_offset;
#endif
 // Configure the lexer
 old_one_file_rec = self->l_lexer.l_one_file_rec;
 self->l_lexer.l_one_file_rec = 0;
 old_flags = self->l_lexer.l_flags;
 self->l_lexer.l_flags &= ~TPPLexer_FLAG_TOK_COMMENTS;
 old_token = self->l_token;
 Dee_INHERIT_REF(self->l_token,kick_token);
 old_files = self->l_lexer.l_files;
 self->l_lexer.l_files = code_file;
 DEE_ASSERT(code_file->f_prev == NULL);
 // Parse the code
 if DEE_UNLIKELY(_DeeLexer_KickStart(self) != 0) result = NULL;
 else result = DeeLexer_ParseAndCompile((DeeObject *)self,NULL);
 // Restore old configuration
 TPPFile_XDECREF(self->l_lexer.l_files);
 self->l_lexer.l_files = old_files;
 Dee_DECREF(self->l_token);
 self->l_token = old_token;
 self->l_lexer.l_flags = old_flags;
 self->l_lexer.l_one_file_rec = old_one_file_rec;
 return result;
}

DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *)
_DeeLexer_ExecStringAndCaptureStdout(
 DEE_A_IN DeeLexerObject *self, char const *code_begin, char const *code_end,
 TPPFileObject *ref_file, Dee_size_t ref_file_offset) {
 DeeObject *temp,*callable_code,*old_stdout,*new_stdout,*result;
 if DEE_UNLIKELY((callable_code = _DeeLexer_CompileString(
  self,code_begin,code_end,ref_file,ref_file_offset)) == NULL) return NULL;
 if DEE_UNLIKELY((new_stdout = DeeFileWriter_New()) == NULL) { Dee_DECREF(callable_code); return NULL; }
 // Backup the old stdout and override it with our stringwriter
 old_stdout = DeeFile_Std(DEE_STDOUT);
 DeeFile_SetStd(DEE_STDOUT,new_stdout);
 // Execute the deemon code
 temp = DeeObject_Call(callable_code,Dee_EmptyTuple);
 // Restore the old stdout and decref the code
 DeeFile_SetStd(DEE_STDOUT,old_stdout);
 Dee_DECREF(old_stdout);
 Dee_DECREF(callable_code);
 if DEE_UNLIKELY(!temp) { Dee_DECREF(new_stdout); return NULL; }
 Dee_DECREF(temp);
 // Pack the generated output into a string object
 result = DeeFileWriter_Pack(new_stdout);
 Dee_DECREF(new_stdout);
 return result;
}

DEE_COMPILER_MSVC_WARNING_PUSH(4701)
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeLexer_DoFormatFileStreamEx(
 DEE_A_IN DeeLexerObject *self, DEE_A_INOUT_OBJECT(DeeFileObject) *in,
 DEE_A_INOUT_OBJECT(DeeFileObject) *out, DEE_A_IN struct DeeDeemonFormatConfig const *config) {
 static char const code_begin_text[] = "[[[deemon";
 static char const code_end_text[]   = "]]]";
 static char const paste_end_text[]  = "[[[end]]]";
 int result; TPPTokenObject token = TPPToken_INIT();
 TPPFileObject *root_file;
 TPPLexerObject *lexer = &self->l_lexer;
 char const *copy_start;
 DeeObject *in_data;
 Dee_uint64_t old_out_pos;
 (void)config;
 if (in == out) {
  if DEE_UNLIKELY(DeeFile_Tell(out,&old_out_pos) != 0) return -1;
  if DEE_UNLIKELY((in_data = DeeFile_ReadData(in,(Dee_size_t)-1)) == NULL) return -1;
  if DEE_UNLIKELY(DeeFile_SetPos(out,old_out_pos) != 0) { Dee_DECREF(in_data); return -1; }
 } else {
  if DEE_UNLIKELY((in_data = DeeFile_ReadData(in,(Dee_size_t)-1)) == NULL) return -1;
 }
 root_file = TPPFile_NewFromStringEx(TPPFileFormatFlag_NONE,
                                     DeeString_STR(in_data),
                                     DeeString_SIZE(in_data));
 Dee_DECREF(in_data);
 if DEE_UNLIKELY(!root_file) goto err;
 if (DeeFileIO_Check(in)) {
  DeeObject *temp_ob;
  if DEE_UNLIKELY((temp_ob = DeeUtf8String_Cast(DeeFileIO_FILE(in))) == NULL) goto err2_;
  result = _TPPFile_SetName(root_file,DeeUtf8String_STR(temp_ob));
  Dee_DECREF(temp_ob);
  if DEE_UNLIKELY(result != 0) goto err2_;
 } else {
  if DEE_UNLIKELY(_TPPFile_SetName(root_file,"format_input_file") != 0) goto err2_;
 }
 TPPLexer_PushFile(lexer,root_file);
 copy_start = root_file->f_data;
 while (1) {
  if DEE_UNLIKELY(TPPLexer_Next(lexer,&token) != 0) goto err2;
  switch (token.tk_id) {

   case 0: goto done; // EOF
   case TPP_TOK_COMMENT:
    // Comment token
    if (token.tk_file == root_file) { // Root file
     DeeLexerObject *lexer_copy;
     DeeObject *comment_output; Dee_size_t code_offset;
     char const *code_begin,*code_end,*file_pos;
     if DEE_UNLIKELY(DeeFile_WriteAll(out,copy_start,(
      Dee_size_t)(token.tk_str_end-copy_start)) != 0) goto err2;
     copy_start = token.tk_str_end;
     _DeeToken_GetCommentText(&token,&code_offset,&code_begin,&code_end);
     if ((Dee_size_t)(code_end-code_begin) >= (((
      sizeof(code_begin_text)+sizeof(code_end_text))/sizeof(char))-2) &&
      memcmp(code_begin,code_begin_text,((sizeof(code_begin_text)/sizeof(char))-1)) == 0 &&
      memcmp(code_end-((sizeof(code_end_text)/sizeof(char))-1),
      code_end_text,((sizeof(code_end_text)/sizeof(char))-1)) == 0) {
      TPPTokenObject token2 = TPPToken_INIT();
      DEE_LVERBOSE3("%s(%d) : FormatFile : %.*q\n",
                    TPPToken_File(&token),TPPToken_Line(&token)+1,
                    (code_end-code_begin)-12,code_begin+9);
      file_pos = root_file->f_iter;
      root_file->f_iter = token.tk_str_begin;
      // Execute the code in a copy of the current lexer (that way macros don't leak back into the real lexer)
      // NOTE: Don't be fooled. - This line has a heck of a lot to do...
      if DEE_UNLIKELY((lexer_copy = (DeeLexerObject *)DeeObject_Copy((DeeObject *)self)) == NULL) goto err2;
      comment_output = _DeeLexer_ExecStringAndCaptureStdout(
       lexer_copy,code_begin+9,code_end-3,root_file,code_offset);
      Dee_DECREF(lexer_copy);
      if DEE_UNLIKELY(!comment_output) goto err2;
      root_file->f_iter = file_pos;
      //DEE_LDEBUG("Output: %k\n",comment_output);
      // skip old generated code
      // NOTE: We parse this code without macros or directives, since it is considered dead
      // TODO: This one could be performed more efficiently (similar to how pp blocks are skipped in tpp)
      while (1) {
       if DEE_UNLIKELY(TPPLexer_NextNoMacro(lexer,&token2) != 0) {
err_ins: Dee_DECREF(comment_output); TPPToken_Quit(&token2); goto err2;
       }
       if (token2.tk_id == 0) break;
       if (token2.tk_file == root_file && token2.tk_id == TPP_TOK_COMMENT) {
        _DeeToken_GetCommentText(&token2,&code_offset,&code_begin,&code_end);
        if ((Dee_size_t)(code_end-code_begin) == ((sizeof(paste_end_text)/sizeof(char))-1) &&
            memcmp(code_begin,paste_end_text,(sizeof(paste_end_text)/sizeof(char))-1) == 0) break;
       }
      }
      DEE_ASSERT(DeeObject_Check(comment_output) &&
                 DeeString_Check(comment_output));
      if (token2.tk_id) {
       int has_prefixed_linefeed;
       // Only prepend it if begin and end are on different lines
       // and the output doesn't start with a linefeed.
       has_prefixed_linefeed = (DeeString_SIZE(comment_output) && (
        DeeString_STR(comment_output)[0] == '\r' ||
        DeeString_STR(comment_output)[0] == '\n'));
       if (TPPToken_Line(&token) != TPPToken_Line(&token2) && !has_prefixed_linefeed) {
        if DEE_UNLIKELY(DeeFile_PRINT(out,"\n") != 0) goto err_ins;
        has_prefixed_linefeed = 1;
       }
       if DEE_UNLIKELY(DeeFile_WriteAll(out,DeeString_STR(comment_output),
        DeeString_SIZE(comment_output)) != 0) goto err_ins;
       if (has_prefixed_linefeed && (!DeeString_SIZE(comment_output) || (
        DeeString_STR(comment_output)[DeeString_SIZE(comment_output)-1] != '\r' &&
        DeeString_STR(comment_output)[DeeString_SIZE(comment_output)-1] != '\n'))) {
        // If the output doesn't end with a linefeed, append one
        // NOTE: Don't do this if we didn't prepend one either
        if DEE_UNLIKELY(DeeFile_PRINT(out,"\n") != 0) goto err_ins;
       }
       copy_start = token2.tk_str_begin; // Skip all old data
      } else {
       if DEE_UNLIKELY(DeeFile_StdPrintf(DEE_STDERR,
        "%s(%d) : Format block is missing its [[[end]]] marker\n",
        TPPToken_File(&token),TPPToken_Line(&token)+1) != 0) goto err_ins;
      }
      Dee_DECREF(comment_output);
      TPPToken_Quit(&token2);
     }
    }
    break;

   default: break; // Ignore all tokens by default
  }
 }
done:
 result = 0;
 if DEE_UNLIKELY(DeeFile_WriteAll(out,copy_start,(
  Dee_size_t)(root_file->f_end-copy_start)) != 0) goto err2;
 // Flush All remaining data to the output
 copy_start = token.tk_str_end;
end2: TPPFile_DECREF(root_file);
end: TPPToken_Quit(&token);
 return result;
err2:
 // Try to restore the original file
 if (in == out) {
  if DEE_UNLIKELY(DeeFile_SetPos(out,old_out_pos) != 0) {
restore_fail: DeeError_Print("Failed to restore original input\n",1); goto err2_;
  }
  if DEE_UNLIKELY(DeeFile_WriteAll(out,TPPFile_DATA(root_file),TPPFile_SIZE(root_file)) != 0) goto restore_fail;
  if DEE_UNLIKELY(DeeFile_Trunc(out) != 0) DeeError_Handled(); // Ignore errors in here
 }
err2_:
 result = -1;
 goto end2;
err: result = -1; goto end;
}
DEE_COMPILER_MSVC_WARNING_POP

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeLexer_DoSafeFormatFileStreamEx(
 DEE_A_IN DeeLexerObject *self, DEE_A_INOUT_OBJECT(DeeFileObject) *in,
 DEE_A_INOUT_OBJECT(DeeFileObject) *out, DEE_A_IN struct DeeDeemonFormatConfig const *config) {
 if (DeeFileIO_Check(out)) {
  DeeObject *out_mode,*temp_fp,*out_filename,*temp_filename; int temp;
  if DEE_UNLIKELY((temp_fp = DeeFileIO_NewTemporary(DEE_FILEIO_NEWTEMPORARY_FLAG_NONE)) == NULL) return -1;
  if DEE_UNLIKELY((_DeeLexer_DoFormatFileStreamEx(self,in,temp_fp,config)) != 0) {
   DeeFile_Close(temp_fp);
   DeeError_PUSH_STATE() {
    if DEE_UNLIKELY(_DeeFS_RmFileObject(DeeFileIO_FILE(temp_fp)) != 0)
     DeeError_Handled();
   } DeeError_POP_STATE();
   Dee_DECREF(temp_fp);
   return -1;
  }
  Dee_INCREF(temp_filename = DeeFileIO_FILE(temp_fp));
  Dee_INCREF(out_filename = DeeFileIO_FILE(out));
  out_mode = DeeFileIO_Mode(out);
  DeeFile_Close(temp_fp);
  DeeFile_Close(out);
  // Even if we crash after the rmfile, the user
  // could still recover his data from '/tmp/'
  if (DEE_UNLIKELY(_DeeFS_RmFileObject(out_filename) != 0)
   || DEE_UNLIKELY(_DeeFS_MoveObject(temp_filename,out_filename) != 0)
  // v better not re-open the file. What if it was opened with "w"
// || DEE_UNLIKELY(DeeFileIO_ReOpenObject(out,out_filename,DeeString_STR(out_mode)) != 0)
   ) temp = -1; else temp = 0;
  Dee_DECREF(temp_filename);
  Dee_DECREF(temp_fp);
  Dee_DECREF(out_filename);
  return temp;
 } else {
  return _DeeLexer_DoFormatFileStreamEx(self,in,out,config);
 }
}

DEE_A_RET_EXCEPT(-1) int DeeLexer_FormatFileStreamEx(
 DEE_A_INOUT_OBJECT(DeeLexerObject) *self, DEE_A_INOUT_OBJECT(DeeFileObject) *in,
 DEE_A_INOUT_OBJECT(DeeFileObject) *out, DEE_A_IN struct DeeDeemonFormatConfig const *config) {
 int result; TPPFileObject *old_files;
 TPPLexerFlags old_flags; unsigned int old_one_file_rec;
 DEE_ASSERT(DeeObject_Check(self) && DeeLexer_Check(self));
 DEE_ASSERT(DeeObject_Check(in) && DeeFile_Check(in));
 DEE_ASSERT(DeeObject_Check(out) && DeeFile_Check(out));
 DEE_ASSERT(config);
 old_flags = ((DeeLexerObject *)self)->l_lexer.l_flags;
 ((DeeLexerObject *)self)->l_lexer.l_flags |= TPPLexer_FLAG_TOK_COMMENTS;
 ((DeeLexerObject *)self)->l_lexer.l_flags &= ~(
  TPPLexer_FLAG_WANT_LF|TPPLexer_FLAG_WANT_SPC|
  TPPLexer_FLAG_NO_MACROS|TPPLexer_FLAG_NO_DIRECTIVES|
  TPPLexer_FLAG_ONE_FILE|TPPLexer_FLAG_ONE_REAL_FILE);
 old_files = ((DeeLexerObject *)self)->l_lexer.l_files;
 ((DeeLexerObject *)self)->l_lexer.l_files = NULL;
 old_one_file_rec = ((DeeLexerObject *)self)->l_lexer.l_one_file_rec;
 ((DeeLexerObject *)self)->l_lexer.l_one_file_rec = 0;
 result = config->unsafe_format
  ? _DeeLexer_DoFormatFileStreamEx((DeeLexerObject *)self,in,out,config)
  : _DeeLexer_DoSafeFormatFileStreamEx((DeeLexerObject *)self,in,out,config);
 ((DeeLexerObject *)self)->l_lexer.l_one_file_rec = old_one_file_rec;
 TPPFile_XDECREF(((DeeLexerObject *)self)->l_lexer.l_files);
 ((DeeLexerObject *)self)->l_lexer.l_files = old_files;
 ((DeeLexerObject *)self)->l_lexer.l_flags = old_flags;
 return result;
}






static void DEE_CALL _deetoken_tp_dtor(DeeTokenObject *self) {
 TPPToken_Quit(&self->tk_token);
}
static int DEE_CALL _deetoken_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTokenObject *self) {
 TPPToken_Init(&self->tk_token);
 return 0;
}
static int DEE_CALL _deetoken_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTokenObject *self, DeeTokenObject *right) {
 TPPToken_InitCopy(&self->tk_token,&right->tk_token);
 return 0;
}
static int DEE_CALL _deetoken_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTokenObject *self, DeeTokenObject *right) {
 TPPToken_InitMove(&self->tk_token,&right->tk_token);
 return 0;
}
static int DEE_CALL _deetoken_tp_copy_assign(DeeTokenObject *self, DeeTokenObject *right) {
 if (self != right) {
  _deetoken_tp_dtor(self);
  _deetoken_tp_copy_ctor(Dee_TYPE(self),self,right);
 }
 return 0;
}
static int DEE_CALL _deetoken_tp_move_assign(
 DeeTokenObject *self, DeeTokenObject *right) {
 if (self != right) {
  _deetoken_tp_dtor(self);
  _deetoken_tp_move_ctor(Dee_TYPE(self),self,right);
 }
 return 0;
}
static int DEE_CALL _deelexer_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeLexerObject *self) {
 return DeeLexer_Init(self);
}
static int DEE_CALL _deelexer_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeLexerObject *self, DeeLexerObject *right) {
 if DEE_UNLIKELY(TPPLexer_InitCopy(&self->l_lexer,&right->l_lexer,
  TPP_LEXER_COPY_FLAG_MACROS|TPP_LEXER_COPY_FLAG_INCLUDES|
  TPP_LEXER_COPY_FLAG_SYSINCLUDES|TPP_LEXER_COPY_FLAG_WARNINGS) != 0) return -1;
 if DEE_UNLIKELY((self->l_token = (DeeTokenObject *)DeeObject_Copy((DeeObject *)right->l_token)) == NULL) {
err_0: TPPLexer_Quit(&self->l_lexer); return -1;
 }
 if DEE_UNLIKELY(DeeFutureTokensList_InitCopy(&self->l_future,&right->l_future) != 0) {
err_1: Dee_DECREF(self->l_token); goto err_0;
 }
 if DEE_UNLIKELY(DeeStaticIfStack_InitCopy(&self->l_static_if_stack,&right->l_static_if_stack) != 0) {
err_2: DeeFutureTokensList_Quit(&self->l_future); goto err_1;
 }
 if DEE_UNLIKELY(DeeStructPackingStack_InitCopy(&self->l_pack,&right->l_pack) != 0) {
/*err_3:*/ DeeStaticIfStack_Quit(&self->l_static_if_stack); goto err_2;
 }
 Dee_INCREF(self->l_error_out = right->l_error_out);
 Dee_INCREF(self->l_message_out = right->l_message_out);
 self->l_error_cur = right->l_error_cur;
 self->l_error_max = right->l_error_max;
 Dee_XINCREF(self->l_error_last = right->l_error_last);
 self->l_func_name = right->l_func_name;
 self->l_suppress_warnings = right->l_suppress_warnings;
 self->l_suppress_Wcop = right->l_suppress_Wcop;
 self->l_token_pos = right->l_token_pos;
 self->l_brace_recursion = right->l_brace_recursion;
 self->l_syntax_recursion = right->l_syntax_recursion;
 self->l_max_syntax_recursion = right->l_max_syntax_recursion;
 return 0;
}
static int DEE_CALL _deelexer_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeLexerObject *self, DeeLexerObject *right) {
 if DEE_UNLIKELY(TPPLexer_InitMove(&self->l_lexer,&right->l_lexer) != 0) return -1;
 Dee_INCREF(self->l_token = right->l_token);
 DeeFutureTokensList_InitMove(&self->l_future,&right->l_future);
 _DeeStaticIfStack_InitMove(&self->l_static_if_stack,&right->l_static_if_stack);
 DeeStructPackingStack_InitMove(&self->l_pack,&right->l_pack);
 Dee_INCREF(self->l_error_out = right->l_error_out);
 Dee_INCREF(self->l_message_out = right->l_message_out);
 self->l_error_cur = right->l_error_cur;
 self->l_error_max = right->l_error_max;
 self->l_error_last = right->l_error_last;
 right->l_error_last = NULL;
 self->l_func_name = right->l_func_name;
 self->l_suppress_warnings = right->l_suppress_warnings;
 self->l_suppress_Wcop = right->l_suppress_Wcop;
 self->l_token_pos = right->l_token_pos;
 self->l_brace_recursion = right->l_brace_recursion;
 self->l_syntax_recursion = right->l_syntax_recursion;
 self->l_max_syntax_recursion = right->l_max_syntax_recursion;
 return 0;
}
static void DEE_CALL _deelexer_tp_dtor(DeeLexerObject *self) {
 Dee_XDECREF(self->l_error_last);
 Dee_DECREF(self->l_token);
 Dee_DECREF(self->l_message_out);
 Dee_DECREF(self->l_error_out);
 DEE_ASSERT(self->l_suppress_warnings == 0);
 DEE_ASSERT(self->l_suppress_Wcop == 0);
 DeeFutureTokensList_Quit(&self->l_future);
 TPPLexer_Quit(&self->l_lexer);
 DeeStructPackingStack_Quit(&self->l_pack);
 DeeStaticIfStack_Quit(&self->l_static_if_stack);
}
DEE_VISIT_PROC(_deelexer_tp_visit,DeeLexerObject *self) {
 DeeTokenObject **future_begin,**future_end;
 Dee_VISIT(self->l_token);
 future_end = (future_begin = self->l_future.ftl_tokenv)+self->l_future.ftl_tokenc;
 while (future_begin != future_end) Dee_VISIT(*future_begin++);
}





static DeeObject *DEE_CALL _deetoken_file_get(DeeObject *self, void *DEE_UNUSED(closure)) {
 return DeeString_New(DeeToken_FILE(self));
}
static DeeObject *DEE_CALL _deetoken_line_get(DeeObject *self, void *DEE_UNUSED(closure)) {
 return DeeObject_New(int,DeeToken_LINE(self));
}
static DeeObject *DEE_CALL _deetoken_eval_int(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *lexer;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:eval_int",&lexer) != 0) return NULL;
 if DEE_UNLIKELY((lexer = DeeObject_GetInstance(lexer,&DeeLexer_Type)) == NULL) return NULL;
 return DeeToken_EvalInt(self,lexer);
}
static DeeObject *DEE_CALL _deetoken_eval_float(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *lexer;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:eval_float",&lexer) != 0) return NULL;
 if DEE_UNLIKELY((lexer = DeeObject_GetInstance(lexer,&DeeLexer_Type)) == NULL) return NULL;
 return DeeToken_EvalFloat(self,lexer);
}
static DeeObject *DEE_CALL _deetoken_eval_str(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":eval_str") != 0) return NULL;
 return DeeToken_EvalString(self);
}



static DeeObject *_deelexer_yield(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":yield") != 0) return NULL;
 return DeeLexer_Yield(self);
}
static DeeObject *_deelexer_return(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *tok;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:return",&tok) != 0) return NULL;
 if DEE_UNLIKELY((tok = DeeObject_GetInstance(tok,&DeeToken_Type)) == NULL) return NULL;
 if DEE_UNLIKELY(DeeLexer_Return(self,tok) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelexer_include(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *include_ob; char const *disp_filename = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|s:include",&include_ob,&disp_filename) != 0) return NULL;
 if (DeeFile_Check(include_ob)) {
  if DEE_UNLIKELY(DeeLexer_IncludeFileStreamEx(
   self,include_ob,disp_filename ? disp_filename
   : DeeFileIO_Check(include_ob)
   ? DeeString_STR(DeeFileIO_FILE(include_ob))
   : "<file>") != 0) return NULL;
 } else {
  if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(include_ob,&DeeString_Type) != 0) return NULL;
  if DEE_UNLIKELY(DeeLexer_IncludeFilenameObjectEx(self,include_ob,disp_filename
   ? disp_filename : DeeString_STR(include_ob)) != 0) return NULL;
 }
 DeeReturn_None;
}
static DeeObject *_deelexer_include_string(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *include_str; char const *disp_filename = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|s:include_string",&include_str,&disp_filename) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(include_str,&DeeString_Type) != 0) return NULL;
 if DEE_UNLIKELY(DeeLexer_IncludeStringObjectEx(self,include_str,disp_filename ? disp_filename : "<string>") != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelexer_include_path(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *include_path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:include_path",&include_path) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(include_path,&DeeString_Type) != 0) return NULL;
 if DEE_UNLIKELY(DeeLexer_AddSysIncludePathObject(self,include_path) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelexer_pack_push(
 DeeLexerObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":pack_push") != 0) return NULL;
 if DEE_UNLIKELY(DeeStructPackingStack_Push(&self->l_pack) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelexer_pack_pop(
 DeeLexerObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":pack_pop") != 0) return NULL;
 if DEE_UNLIKELY(DeeStructPackingStack_Pop(&self->l_pack) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deelexer_parse_and_compile(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 struct DeeCompilerConfig *config = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|p:parse_and_compile",&config) != 0) return NULL;
 return DeeLexer_ParseAndCompile(self,config);
}
static DeeObject *_deelexer_parse_str(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":parse_str") != 0) return NULL;
 return DeeLexer_ParseString(self);
}

static struct DeeMemberDef _deetoken_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("id",DeeTokenObject,tk_token.tk_id,TPPTokenID),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__tk_token_tk_id",DeeTokenObject,tk_token.tk_id,TPPTokenID),
 DEE_MEMBERDEF_NAMED_RO_v100("__tk_token_tk_file",DeeTokenObject,tk_token.tk_file,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__tk_token_tk_file_off",DeeTokenObject,tk_token.tk_file_off,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__tk_token_tk_str_file",DeeTokenObject,tk_token.tk_str_file,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__tk_token_tk_str_begin",DeeTokenObject,tk_token.tk_str_begin,p(char)),
 DEE_MEMBERDEF_NAMED_RO_v100("__tk_token_tk_str_end",DeeTokenObject,tk_token.tk_str_end,p(char)),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};
static struct DeeGetSetDef _deetoken_tp_getsets[] = {
 DEE_GETSETDEF_CONST_v100("file",member(&_deetoken_file_get),null,null,"-> string\n\tThe __FILE__ value of this token"),
 DEE_GETSETDEF_CONST_v100("line",member(&_deetoken_line_get),null,null,"-> int\n\tThe __LINE__ value of this token"),
 DEE_GETSETDEF_END_v100
};
static struct DeeMethodDef _deetoken_tp_methods[] = {
 DEE_METHODDEF_CONST_v100("eval_int",member(&_deetoken_eval_int),"() -> object\n\tReturns the token processed as an integral"),
 DEE_METHODDEF_CONST_v100("eval_float",member(&_deetoken_eval_float),"() -> object\n\tReturns the token processed as a float"),
 DEE_METHODDEF_CONST_v100("eval_str",member(&_deetoken_eval_str),"() -> string\n\tReturns the token processed as a string"),
 DEE_METHODDEF_END_v100
};


static struct DeeMemberDef _deelexer_tp_members[] = {
 DEE_MEMBERDEF_NAMED_v100("pack",DeeLexerObject,l_pack.sps_packing,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("token",DeeLexerObject,l_token,object),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__l_token",DeeLexerObject,l_token,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_future_ftl_tokenc",DeeLexerObject,l_future.ftl_tokenc,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_future_ftl_tokena",DeeLexerObject,l_future.ftl_tokena,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_future_ftl_tokenv",DeeLexerObject,l_future.ftl_tokena,p2(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_error_out",DeeLexerObject,l_error_out,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_message_out",DeeLexerObject,l_message_out,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_error_cur",DeeLexerObject,l_error_cur,Dee_uint32_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_error_max",DeeLexerObject,l_error_max,Dee_uint32_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_error_last",DeeLexerObject,l_error_last,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_static_if_stack_si_c",DeeLexerObject,l_static_if_stack.si_c,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_static_if_stack_si_v",DeeLexerObject,l_static_if_stack.si_v,p(void)),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_func_name",DeeLexerObject,l_func_name,TPPTokenID),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_suppress_warnings",DeeLexerObject,l_suppress_warnings,uint),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_suppress_Wcop",DeeLexerObject,l_suppress_Wcop,uint),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_pack_sps_packing",DeeLexerObject,l_pack.sps_packing,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_pack_sps_packing_c",DeeLexerObject,l_pack.sps_packing_c,Dee_size_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_pack_sps_packing_v",DeeLexerObject,l_pack.sps_packing_v,p(Dee_size_t)),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_token_pos",DeeLexerObject,l_token_pos,Dee_uint64_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_brace_recursion",DeeLexerObject,l_brace_recursion,Dee_uint32_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_syntax_recursion",DeeLexerObject,l_syntax_recursion,uint),
 DEE_MEMBERDEF_NAMED_RO_v100("__l_max_syntax_recursion",DeeLexerObject,l_max_syntax_recursion,uint),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};
static struct DeeMethodDef _deelexer_tp_methods[] = {
 DEE_METHODDEF_CONST_v100("yield",member(&_deelexer_yield),"() -> token\n"
  "\tReads one token and returns it"),
 DEE_METHODDEF_CONST_v100("return",member(&_deelexer_return),"(token tk) -> none\n"
  "\tReturns a previously parsed token to be yieled the next time yield() is called"),
 DEE_METHODDEF_CONST_v100("include",member(&_deelexer_include),"(file fp, string disp_name = none) -> none\n"
  "(string path, string disp_name = none) -> none\n"
  "\tIncludes a file/path"),
 DEE_METHODDEF_CONST_v100("include_string",member(&_deelexer_include_string),"(string path, string disp_name = none) -> none\n"
  "\tIncludes a string as code"),
 DEE_METHODDEF_CONST_v100("include_path",member(&_deelexer_include_path),"(string path) -> none\n"
  "\tAdds 'path' to the list of used include paths"),
 DEE_METHODDEF_CONST_v100("pack_push",member(&_deelexer_pack_push),"() -> none\n"
  "\tSame #pragma pack(push)"),
 DEE_METHODDEF_CONST_v100("pack_pop",member(&_deelexer_pack_pop),"() -> none\n"
  "\tSame #pragma pack(pop)"),
 DEE_METHODDEF_CONST_v100("parse_and_compile",member(&_deelexer_parse_and_compile),"(none *config = none) -> function\n"
  "\tParses and compiles deemon code, returning a callable function object containing said code"),
 DEE_METHODDEF_CONST_v100("parse_str",member(&_deelexer_parse_str),"() -> str\n"
  "\tParses a string and returns it"),
 DEE_METHODDEF_END_v100
};
static struct DeeMemberDef _deelexeriterator_tp_members[] = {
 DEE_MEMBERDEF_v100(DeeLexerIteratorObject,li_lexer,object),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef _deelexer_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("token",object,&DeeToken_Type),
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeLexerIterator_Type),
 DEE_MEMBERDEF_END_v100
};


static int DEE_CALL _deelexeriterator_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeLexerIteratorObject *self, DeeObject *args) {
 DeeLexerObject *lexer;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:lexer.iterator",&lexer) != 0) return -1;
 if DEE_UNLIKELY((lexer = (DeeLexerObject *)DeeObject_GetInstance((
  DeeObject *)lexer,&DeeLexer_Type)) == NULL) return -1;
 Dee_INCREF(self->li_lexer = lexer);
 return 0;
}
static int DEE_CALL _deelexeriterator_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeLexerIteratorObject *self, DeeLexerIteratorObject *right) {
 Dee_INCREF(self->li_lexer = right->li_lexer);
 return 0;
}
static void DEE_CALL _deelexeriterator_tp_dtor(DeeLexerIteratorObject *self) {
 Dee_DECREF(self->li_lexer);
}
static DeeObject *DEE_CALL _deelexer_tp_seq_iter_self(DeeLexerObject *self) {
 DeeLexerIteratorObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOC(DeeLexerIteratorObject)) != NULL) {
  DeeObject_INIT(result,&DeeLexerIterator_Type);
  Dee_INCREF(result->li_lexer = self);
 }
 return (DeeObject *)result;
}
static int DEE_CALL _deelexeriterator_tp_seq_iter_next(
 DeeLexerIteratorObject *self, DeeTokenObject **result) {
 DeeTokenObject *result_ob;
 if DEE_UNLIKELY(!self->li_lexer->l_token->tk_token.tk_id) return 1;
 Dee_INCREF(result_ob = self->li_lexer->l_token);
 if DEE_UNLIKELY(!DeeLexer_Yield((DeeObject *)self->li_lexer)) { Dee_DECREF(result_ob); return -1; }
 Dee_INHERIT_REF(*result,result_ob);
 return 0;
}
DEE_VISIT_PROC(_deelexeriterator_tp_visit,DeeLexerIteratorObject *self) {
 Dee_VISIT(self->li_lexer);
}


static DeeTokenObject *DEE_CALL _deetoken_tp_alloc(DeeTypeObject *tp_self) {
 DeeTokenObject *result = DEE_OBJECTPOOL_ALLOC(token_pool);
 if DEE_LIKELY(result) DeeObject_INIT(result,tp_self);
 return result;
}
static void DEE_CALL _deetoken_tp_free(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTokenObject *ob) {
 DEE_OBJECTPOOL_FREE(token_pool,ob);
}


DeeTypeObject DeeToken_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("token"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeLexerObject),
  member(&_deetoken_tp_alloc),
  member(&_deetoken_tp_ctor),
  member(&_deetoken_tp_copy_ctor),
  member(&_deetoken_tp_move_ctor),null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(
  member(&_deetoken_tp_free),
  member(&_deetoken_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deetoken_tp_copy_assign),
  member(&_deetoken_tp_move_assign),null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&DeeToken_Str),
  member(&DeeToken_Str),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deetoken_tp_members),member(_deetoken_tp_getsets),
  member(_deetoken_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};
DeeTypeObject DeeLexer_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("lexer"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeLexerObject),null,
  member(&_deelexer_tp_ctor),
  member(&_deelexer_tp_copy_ctor),
  member(&_deelexer_tp_move_ctor),null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deelexer_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deelexer_tp_visit),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,
  member(&_deelexer_tp_seq_iter_self),null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deelexer_tp_members),null,member(_deelexer_tp_methods),
  member(_deelexer_tp_class_members),null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};
DeeTypeObject DeeLexerIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("lexer.iterator"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeLexerIteratorObject),null,null,
  member(&_deelexeriterator_tp_copy_ctor),null,
  member(&_deelexeriterator_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deelexeriterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deelexeriterator_tp_visit),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,
  member(&_deelexeriterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deelexeriterator_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};

DEE_DECL_END

#endif /* !GUARD_DEEMON_COMPILER_LEXER_C */
