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

#define _DeeType_ADD_ATOMIC(ty)\
do{   if ((ty) == NULL) (ty) = DeeObject_TYPE(atomic(Dee_rt_int));\
 else if ((ty) == DeeObject_TYPE(Dee_rt_int8_t)) (ty) = DeeObject_TYPE(atomic(Dee_rt_int8_t));\
 else if ((ty) == DeeObject_TYPE(Dee_rt_int16_t)) (ty) = DeeObject_TYPE(atomic(Dee_rt_int16_t));\
 else if ((ty) == DeeObject_TYPE(Dee_rt_int32_t)) (ty) = DeeObject_TYPE(atomic(Dee_rt_int32_t));\
 else if ((ty) == DeeObject_TYPE(Dee_rt_int64_t)) (ty) = DeeObject_TYPE(atomic(Dee_rt_int64_t));\
 else if ((ty) == DeeObject_TYPE(Dee_rt_uint8_t)) (ty) = DeeObject_TYPE(atomic(Dee_rt_uint8_t));\
 else if ((ty) == DeeObject_TYPE(Dee_rt_uint16_t)) (ty) = DeeObject_TYPE(atomic(Dee_rt_uint16_t));\
 else if ((ty) == DeeObject_TYPE(Dee_rt_uint32_t)) (ty) = DeeObject_TYPE(atomic(Dee_rt_uint32_t));\
 else if ((ty) == DeeObject_TYPE(Dee_rt_uint64_t)) (ty) = DeeObject_TYPE(atomic(Dee_rt_uint64_t));\
}while(0)
#define _DeeType_ADD_SIGNED(ty)\
do{   if ((ty) == NULL) (ty) = DeeObject_TYPE(Dee_rt_int);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_uint8_t)) (ty) = DeeObject_TYPE(Dee_rt_int8_t);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_uint16_t)) (ty) = DeeObject_TYPE(Dee_rt_int16_t);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_uint32_t)) (ty) = DeeObject_TYPE(Dee_rt_int32_t);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_uint64_t)) (ty) = DeeObject_TYPE(Dee_rt_int64_t);\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_uint8_t))) (ty) = DeeObject_TYPE(atomic(Dee_rt_int8_t));\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_uint16_t))) (ty) = DeeObject_TYPE(atomic(Dee_rt_int16_t));\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_uint32_t))) (ty) = DeeObject_TYPE(atomic(Dee_rt_int32_t));\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_uint64_t))) (ty) = DeeObject_TYPE(atomic(Dee_rt_int64_t));\
}while(0)
#define _DeeType_ADD_UNSIGNED(ty)\
do{   if ((ty) == NULL) (ty) = DeeObject_TYPE(Dee_rt_uint);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_int8_t)) (ty) = DeeObject_TYPE(Dee_rt_uint8_t);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_int16_t)) (ty) = DeeObject_TYPE(Dee_rt_uint16_t);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_int32_t)) (ty) = DeeObject_TYPE(Dee_rt_uint32_t);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_int64_t)) (ty) = DeeObject_TYPE(Dee_rt_uint64_t);\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_int8_t))) (ty) = DeeObject_TYPE(atomic(Dee_rt_uint8_t));\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_int16_t))) (ty) = DeeObject_TYPE(atomic(Dee_rt_uint16_t));\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_int32_t))) (ty) = DeeObject_TYPE(atomic(Dee_rt_uint32_t));\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_int64_t))) (ty) = DeeObject_TYPE(atomic(Dee_rt_uint64_t));\
}while(0)
#define _DeeType_ADD_CHAR(ty)\
do{   if ((ty) == NULL) (ty) = DeeObject_TYPE(Dee_rt_char);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_int8_t) || (ty) == DeeObject_TYPE(Dee_rt_int16_t) || \
          (ty) == DeeObject_TYPE(Dee_rt_int32_t) || (ty) == DeeObject_TYPE(Dee_rt_int64_t)) (ty) = (DeeTypeObject *)&DeeINT_Type(DEE_CONFIG_RT_SIZEOF_CHAR);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_uint8_t) || (ty) == DeeObject_TYPE(Dee_rt_uint16_t) || \
          (ty) == DeeObject_TYPE(Dee_rt_uint32_t) || (ty) == DeeObject_TYPE(Dee_rt_uint64_t)) (ty) = (DeeTypeObject *)&DeeUINT_Type(DEE_CONFIG_RT_SIZEOF_CHAR);\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_int8_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_int16_t)) || \
          (ty) == DeeObject_TYPE(atomic(Dee_rt_int32_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_int64_t))) (ty) = (DeeTypeObject *)&DeeAtomicINT_Type(DEE_CONFIG_RT_SIZEOF_CHAR);\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_uint8_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_uint16_t)) || \
          (ty) == DeeObject_TYPE(atomic(Dee_rt_uint32_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_uint64_t))) (ty) = (DeeTypeObject *)&DeeAtomicUINT_Type(DEE_CONFIG_RT_SIZEOF_CHAR);\
}while(0)
#define _DeeType_ADD_SHORT(ty)\
do{   if ((ty) == NULL) (ty) = DeeObject_TYPE(Dee_rt_short);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_int8_t) || (ty) == DeeObject_TYPE(Dee_rt_int16_t) || \
          (ty) == DeeObject_TYPE(Dee_rt_int32_t) || (ty) == DeeObject_TYPE(Dee_rt_int64_t)) (ty) = (DeeTypeObject *)&DeeINT_Type(DEE_CONFIG_RT_SIZEOF_SHORT);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_uint8_t) || (ty) == DeeObject_TYPE(Dee_rt_uint16_t) || \
          (ty) == DeeObject_TYPE(Dee_rt_uint32_t) || (ty) == DeeObject_TYPE(Dee_rt_uint64_t)) (ty) = (DeeTypeObject *)&DeeUINT_Type(DEE_CONFIG_RT_SIZEOF_SHORT);\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_int8_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_int16_t)) || \
          (ty) == DeeObject_TYPE(atomic(Dee_rt_int32_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_int64_t))) (ty) = (DeeTypeObject *)&DeeAtomicINT_Type(DEE_CONFIG_RT_SIZEOF_SHORT);\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_uint8_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_uint16_t)) || \
          (ty) == DeeObject_TYPE(atomic(Dee_rt_uint32_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_uint64_t))) (ty) = (DeeTypeObject *)&DeeAtomicUINT_Type(DEE_CONFIG_RT_SIZEOF_SHORT);\
}while(0)
#define _DeeType_ADD_LONG(ty)\
do{   if ((ty) == NULL) (ty) = DeeObject_TYPE(Dee_rt_long);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_int8_t) || (ty) == DeeObject_TYPE(Dee_rt_int16_t) || \
          (ty) == DeeObject_TYPE(Dee_rt_int32_t) || (ty) == DeeObject_TYPE(Dee_rt_int64_t)) (ty) = (DeeTypeObject *)&DeeINT_Type(DEE_CONFIG_RT_SIZEOF_LONG);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_uint8_t) || (ty) == DeeObject_TYPE(Dee_rt_uint16_t) || \
          (ty) == DeeObject_TYPE(Dee_rt_uint32_t) || (ty) == DeeObject_TYPE(Dee_rt_uint64_t)) (ty) = (DeeTypeObject *)&DeeUINT_Type(DEE_CONFIG_RT_SIZEOF_LONG);\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_int8_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_int16_t)) || \
          (ty) == DeeObject_TYPE(atomic(Dee_rt_int32_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_int64_t))) (ty) = (DeeTypeObject *)&DeeAtomicINT_Type(DEE_CONFIG_RT_SIZEOF_LONG);\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_uint8_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_uint16_t)) || \
          (ty) == DeeObject_TYPE(atomic(Dee_rt_uint32_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_uint64_t))) (ty) = (DeeTypeObject *)&DeeAtomicUINT_Type(DEE_CONFIG_RT_SIZEOF_LONG);\
}while(0)
#define _DeeType_ADD_LLONG(ty)\
do{   if ((ty) == NULL) (ty) = DeeObject_TYPE(Dee_rt_llong);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_int8_t) || (ty) == DeeObject_TYPE(Dee_rt_int16_t) || \
          (ty) == DeeObject_TYPE(Dee_rt_int32_t) || (ty) == DeeObject_TYPE(Dee_rt_int64_t)) (ty) = (DeeTypeObject *)&DeeINT_Type(DEE_CONFIG_RT_SIZEOF_LLONG);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_uint8_t) || (ty) == DeeObject_TYPE(Dee_rt_uint16_t) || \
          (ty) == DeeObject_TYPE(Dee_rt_uint32_t) || (ty) == DeeObject_TYPE(Dee_rt_uint64_t)) (ty) = (DeeTypeObject *)&DeeUINT_Type(DEE_CONFIG_RT_SIZEOF_LLONG);\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_int8_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_int16_t)) || \
          (ty) == DeeObject_TYPE(atomic(Dee_rt_int32_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_int64_t))) (ty) = (DeeTypeObject *)&DeeAtomicINT_Type(DEE_CONFIG_RT_SIZEOF_LLONG);\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_uint8_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_uint16_t)) || \
          (ty) == DeeObject_TYPE(atomic(Dee_rt_uint32_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_uint64_t))) (ty) = (DeeTypeObject *)&DeeAtomicUINT_Type(DEE_CONFIG_RT_SIZEOF_LLONG);\
}while(0)
#define _DeeType_ADD_INT8(ty)\
do{   if ((ty) == NULL) (ty) = DeeObject_TYPE(Dee_rt_int8_t);\
 else if (/*(ty) == DeeObject_TYPE(Dee_rt_int8_t) || */(ty) == DeeObject_TYPE(Dee_rt_int16_t) || \
          (ty) == DeeObject_TYPE(Dee_rt_int32_t) || (ty) == DeeObject_TYPE(Dee_rt_int64_t)) (ty) = DeeObject_TYPE(Dee_rt_int8_t);\
 else if (/*(ty) == DeeObject_TYPE(Dee_rt_uint8_t) || */(ty) == DeeObject_TYPE(Dee_rt_uint16_t) || \
          (ty) == DeeObject_TYPE(Dee_rt_uint32_t) || (ty) == DeeObject_TYPE(Dee_rt_uint64_t)) (ty) = DeeObject_TYPE(Dee_rt_uint8_t);\
 else if (/*(ty) == DeeObject_TYPE(atomic(Dee_rt_int8_t)) || */(ty) == DeeObject_TYPE(atomic(Dee_rt_int16_t)) || \
          (ty) == DeeObject_TYPE(atomic(Dee_rt_int32_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_int64_t))) (ty) = DeeObject_TYPE(atomic(Dee_rt_int8_t));\
 else if (/*(ty) == DeeObject_TYPE(atomic(Dee_rt_uint8_t)) || */(ty) == DeeObject_TYPE(atomic(Dee_rt_uint16_t)) || \
          (ty) == DeeObject_TYPE(atomic(Dee_rt_uint32_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_uint64_t))) (ty) = DeeObject_TYPE(atomic(Dee_rt_uint8_t));\
}while(0)
#define _DeeType_ADD_INT16(ty)\
do{   if ((ty) == NULL) (ty) = DeeObject_TYPE(Dee_rt_int16_t);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_int8_t) || /*(ty) == DeeObject_TYPE(Dee_rt_int16_t) || */\
          (ty) == DeeObject_TYPE(Dee_rt_int32_t) || (ty) == DeeObject_TYPE(Dee_rt_int64_t)) (ty) = DeeObject_TYPE(Dee_rt_int16_t);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_uint8_t) || /*(ty) == DeeObject_TYPE(Dee_rt_uint16_t) || */\
          (ty) == DeeObject_TYPE(Dee_rt_uint32_t) || (ty) == DeeObject_TYPE(Dee_rt_uint64_t)) (ty) = DeeObject_TYPE(Dee_rt_uint16_t);\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_int8_t)) || /*(ty) == DeeObject_TYPE(atomic(Dee_rt_int16_t)) || */\
          (ty) == DeeObject_TYPE(atomic(Dee_rt_int32_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_int64_t))) (ty) = DeeObject_TYPE(atomic(Dee_rt_int16_t));\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_uint8_t)) || /*(ty) == DeeObject_TYPE(atomic(Dee_rt_uint16_t)) || */\
          (ty) == DeeObject_TYPE(atomic(Dee_rt_uint32_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_uint64_t))) (ty) = DeeObject_TYPE(atomic(Dee_rt_uint16_t));\
}while(0)
#define _DeeType_ADD_INT32(ty)\
do{   if ((ty) == NULL) (ty) = DeeObject_TYPE(Dee_rt_int32_t);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_int8_t) || (ty) == DeeObject_TYPE(Dee_rt_int16_t) || \
          /*(ty) == DeeObject_TYPE(Dee_rt_int32_t) || */(ty) == DeeObject_TYPE(Dee_rt_int64_t)) (ty) = DeeObject_TYPE(Dee_rt_int32_t);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_uint8_t) || (ty) == DeeObject_TYPE(Dee_rt_uint16_t) || \
          /*(ty) == DeeObject_TYPE(Dee_rt_uint32_t) || */(ty) == DeeObject_TYPE(Dee_rt_uint64_t)) (ty) = DeeObject_TYPE(Dee_rt_uint32_t);\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_int8_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_int16_t)) || \
          /*(ty) == DeeObject_TYPE(atomic(Dee_rt_int32_t)) || */(ty) == DeeObject_TYPE(atomic(Dee_rt_int64_t))) (ty) = DeeObject_TYPE(atomic(Dee_rt_int32_t));\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_uint8_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_uint16_t)) || \
          /*(ty) == DeeObject_TYPE(atomic(Dee_rt_uint32_t)) || */(ty) == DeeObject_TYPE(atomic(Dee_rt_uint64_t))) (ty) = DeeObject_TYPE(atomic(Dee_rt_uint32_t));\
}while(0)
#define _DeeType_ADD_INT64(ty)\
do{   if ((ty) == NULL) (ty) = DeeObject_TYPE(Dee_rt_int64_t);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_int8_t) || (ty) == DeeObject_TYPE(Dee_rt_int16_t) || \
          (ty) == DeeObject_TYPE(Dee_rt_int32_t)/* || (ty) == DeeObject_TYPE(Dee_rt_int64_t)*/) (ty) = DeeObject_TYPE(Dee_rt_int64_t);\
 else if ((ty) == DeeObject_TYPE(Dee_rt_uint8_t) || (ty) == DeeObject_TYPE(Dee_rt_uint16_t) || \
          (ty) == DeeObject_TYPE(Dee_rt_uint32_t)/* || (ty) == DeeObject_TYPE(Dee_rt_uint64_t)*/) (ty) = DeeObject_TYPE(Dee_rt_uint64_t);\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_int8_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_int16_t)) || \
          (ty) == DeeObject_TYPE(atomic(Dee_rt_int32_t))/* || (ty) == DeeObject_TYPE(atomic(Dee_rt_int64_t))*/) (ty) = DeeObject_TYPE(atomic(Dee_rt_int64_t));\
 else if ((ty) == DeeObject_TYPE(atomic(Dee_rt_uint8_t)) || (ty) == DeeObject_TYPE(atomic(Dee_rt_uint16_t)) || \
          (ty) == DeeObject_TYPE(atomic(Dee_rt_uint32_t))/* || (ty) == DeeObject_TYPE(atomic(Dee_rt_uint64_t))*/) (ty) = DeeObject_TYPE(atomic(Dee_rt_uint64_t));\
}while(0)


DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_ParseType(
 DEE_A_IN Dee_uint32_t vardecl_mode,
 DEE_A_INOUT_OPT struct DeeAttributeDecl *attr, DEE_PARSER_PARAMS) {
 DeeTokenObject *ast_token;
 DeeXAstObject *result;
#define FLAG_EXPLICIT_SIGN  0x01 // "signed" or "unsigned" was used
#define FLAG_EXPLICIT_ATOM  0x02 // "atomic" was used
#define FLAG_EXPLICIT_INT   0x04 // "int" was used
#define FLAG_EXPLICIT_SIZE  0x08 // "char", "short", "long" was used
#define FLAG_EXPLICIT_LSIZE (FLAG_EXPLICIT_SIZE|0x10) // "long" was used twice
 int type_flags = 0;
 DeeTypeObject *type_result = NULL;
 (void)DEE_PARSER_ARGS;
 Dee_INCREF(ast_token = (DeeTokenObject *)token_ob);
again_with_mods:
 switch (token.tk_id) {
  {
   if (0) {
  DEE_PARSE_TOKENID_CASE_CONST
    if ((vardecl_mode&DEE_XAST_VARDECL_FLAG_CONST)!=0) {
     if (DeeError_CompilerError(DEE_WARNING_VAR_MODIFER_CONST_ALREADY_UNSED,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "The 'const' variable modifier was already used") != 0) return NULL;
    } else vardecl_mode |= DEE_XAST_VARDECL_FLAG_CONST;
   }
   if (0) {
  DEE_PARSE_TOKENID_CASE_VOLATILE
    if ((vardecl_mode&DEE_XAST_VARDECL_FLAG_VOLATILE)!=0) {
     if (DeeError_CompilerError(DEE_WARNING_VAR_MODIFER_VOLATILE_ALREADY_UNSED,
      (DeeObject *)lexer,(DeeObject *)token_ob,
      "The 'volatile' variable modifier was already used") != 0) return NULL;
    } else vardecl_mode |= DEE_XAST_VARDECL_FLAG_VOLATILE;
   }
  }
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   goto again_with_mods;
  DEE_PARSE_TOKENID_CASE_BOOL type_result = (DeeTypeObject *)&DeeBool_Type; goto y1_end;
  case KWD_float:    type_result = (DeeTypeObject *)&DeeFloat_Type; goto y1_end;
  case KWD_object:   type_result = &DeeObject_Type; goto y1_end;
  case KWD_dict:     type_result = &DeeDict_Type; goto y1_end;
  case KWD_list:     type_result = &DeeList_Type; goto y1_end;
  case KWD_set:      type_result = &DeeSet_Type; goto y1_end;
  case KWD_tuple:    type_result = &DeeTuple_Type; goto y1_end;
  case KWD_string:   type_result = &DeeString_Type; 
y1_end: if DEE_UNLIKELY(!yield()) goto err_ast_token;
   goto end;
  case KWD_double:
   type_result = (DeeTypeObject *)&DeeDouble_Type;
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   if (token.tk_id == KWD_long) {
    type_result = (DeeTypeObject *)&DeeLDouble_Type;
    if DEE_UNLIKELY(!yield()) goto err_ast_token;
   }
   goto end;
  default: break;
 }
again:
 switch (token.tk_id) {
  DEE_PARSE_TOKENID_CASE_CONST
  DEE_PARSE_TOKENID_CASE_VOLATILE
   if DEE_UNLIKELY(!yield()) goto err_ast_token;
   goto again;
  DEE_PARSE_TOKENID_CASE_ATOMIC
   if ((type_flags & FLAG_EXPLICIT_ATOM)!=0) {
    if (DeeError_CompilerError(DEE_WARNING_TYPENAME_ALREADY_USED_ATOMIC,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "'atomic' was already used as modifier") == -1) goto err_ast_token;
   } else _DeeType_ADD_ATOMIC(type_result);
   type_flags |= FLAG_EXPLICIT_ATOM;
next:
   if DEE_UNLIKELY(!yield()) {err_ast_token: Dee_DECREF(ast_token); return NULL; }
   goto again;

  case KWD_int:
   if ((type_flags&FLAG_EXPLICIT_INT)!=0) {
int_used:
    if (DeeError_CompilerError(DEE_WARNING_TYPENAME_ALREADY_USED_INT,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "'int' was already used as modifier") == -1) goto err_ast_token;
   } else if (!type_result) type_result = DeeObject_TYPE(Dee_rt_int);
   type_flags |= FLAG_EXPLICIT_INT;
   goto next;

  DEE_PARSE_TOKENID_CASE_SIGNED
   if ((type_flags&FLAG_EXPLICIT_SIGN)!=0) {
sign_used:
    if (DeeError_CompilerError(DEE_WARNING_TYPENAME_ALREADY_USED_SIGN,(DeeObject *)lexer,(DeeObject *)token_ob,
     "'signed' or 'unsigned' was already used as modifier") == -1) goto err_ast_token;
   } else _DeeType_ADD_SIGNED(type_result);
   type_flags |= FLAG_EXPLICIT_SIGN;
   goto next;

  DEE_PARSE_TOKENID_CASE_UNSIGNED
   if ((type_flags&FLAG_EXPLICIT_SIGN)!=0) goto sign_used;
   else _DeeType_ADD_UNSIGNED(type_result);
   type_flags |= FLAG_EXPLICIT_SIGN;
   goto next;

  case KWD_char:
   if ((type_flags&FLAG_EXPLICIT_SIZE)!=0) {
size_used:
    if (DeeError_CompilerError(DEE_WARNING_TYPENAME_ALREADY_USED_SIZE,
     (DeeObject *)lexer,(DeeObject *)token_ob,
     "Type size was already used as modifier") == -1) goto err_ast_token;
   } else _DeeType_ADD_CHAR(type_result);
   type_flags |= FLAG_EXPLICIT_SIZE;
   goto next;
  
  case KWD_short:
   if ((type_flags&FLAG_EXPLICIT_SIZE)!=0) goto size_used;
   else _DeeType_ADD_SHORT(type_result);
   type_flags |= FLAG_EXPLICIT_SIZE;
   goto next;
  
  case KWD_long:
   if ((type_flags&FLAG_EXPLICIT_LSIZE)==FLAG_EXPLICIT_LSIZE) goto size_used;
   else {
    if ((type_flags&FLAG_EXPLICIT_SIZE)==0) { // first long
     if DEE_UNLIKELY(!yield()) goto err_ast_token;
     if (token.tk_id == KWD_double) {
      type_result = (DeeTypeObject *)&DeeLDouble_Type;
      if DEE_UNLIKELY(!yield()) goto err_ast_token;
      goto end;
     }
     type_flags |= FLAG_EXPLICIT_SIZE;
     _DeeType_ADD_LONG(type_result);
     goto again;
    } else { // second long
     if DEE_UNLIKELY(!yield()) goto err_ast_token;
     if (token.tk_id == KWD_double) {
      type_result = (DeeTypeObject *)&DeeLDouble_Type;
      if DEE_UNLIKELY(!yield()) goto err_ast_token;
      goto end;
     }
     type_flags |= FLAG_EXPLICIT_LSIZE;
     _DeeType_ADD_LLONG(type_result);
     goto again;
    }
   }
   goto next;

  DEE_PARSE_TOKENID_CASE_WCHAR_T
  DEE_PARSE_TOKENID_CASE_CHAR16_T
  DEE_PARSE_TOKENID_CASE_CHAR32_T
   if ((type_flags&FLAG_EXPLICIT_INT)!=0) goto int_used;
   if ((type_flags&FLAG_EXPLICIT_SIGN)!=0) goto sign_used;
   if ((type_flags&FLAG_EXPLICIT_LSIZE)!=0) goto size_used;
   switch (token.tk_id) {
    DEE_PARSE_TOKENID_CASE_WCHAR_T
     type_result = ((type_flags&FLAG_EXPLICIT_ATOM)!=0)
      ?(DeeTypeObject *)&DeeAtomicXINT_Type(DEE_CONFIG_RT_WCHAR_T_SIGNED,DEE_CONFIG_RT_SIZEOF_WCHAR_T)
      :(DeeTypeObject *)&      DeeXINT_Type(DEE_CONFIG_RT_WCHAR_T_SIGNED,DEE_CONFIG_RT_SIZEOF_WCHAR_T);
     goto next;
    DEE_PARSE_TOKENID_CASE_CHAR16_T
     type_result = ((type_flags&FLAG_EXPLICIT_ATOM)!=0)
      ?(DeeTypeObject *)&DeeAtomicXINT_Type(DEE_CONFIG_RT_CHAR16_T_SIGNED,DEE_CONFIG_RT_SIZEOF_CHAR16_T)
      :(DeeTypeObject *)&      DeeXINT_Type(DEE_CONFIG_RT_CHAR16_T_SIGNED,DEE_CONFIG_RT_SIZEOF_CHAR16_T);
     goto next;
    DEE_PARSE_TOKENID_CASE_CHAR32_T
     type_result = ((type_flags&FLAG_EXPLICIT_ATOM)!=0)
      ?(DeeTypeObject *)&DeeAtomicXINT_Type(DEE_CONFIG_RT_CHAR32_T_SIGNED,DEE_CONFIG_RT_SIZEOF_CHAR32_T)
      :(DeeTypeObject *)&      DeeXINT_Type(DEE_CONFIG_RT_CHAR32_T_SIGNED,DEE_CONFIG_RT_SIZEOF_CHAR32_T);
     goto next;
    default: break;
   }
   DEE_BUILTIN_UNREACHABLE();

  DEE_PARSE_TOKENID_CASE_UINT8
  DEE_PARSE_TOKENID_CASE_UINT16
  DEE_PARSE_TOKENID_CASE_UINT32
  DEE_PARSE_TOKENID_CASE_UINT64
#if 1
   if ((type_flags&FLAG_EXPLICIT_INT)!=0 ||
       (type_flags&FLAG_EXPLICIT_SIGN)!=0 ||
       (type_flags&FLAG_EXPLICIT_LSIZE)!=0
       ) break;
#else
   if ((type_flags&FLAG_EXPLICIT_INT)!=0) goto int_used;
   if ((type_flags&FLAG_EXPLICIT_SIGN)!=0) goto sign_used;
   if ((type_flags&FLAG_EXPLICIT_LSIZE)!=0) goto size_used;
#endif
   type_flags |= (FLAG_EXPLICIT_INT|FLAG_EXPLICIT_SIGN|FLAG_EXPLICIT_LSIZE);
   switch (token.tk_id) {
    DEE_PARSE_TOKENID_CASE_UINT8  type_result = ((type_flags&FLAG_EXPLICIT_ATOM)!=0)?(DeeTypeObject *)&DeeAtomicUInt8_Type:(DeeTypeObject *)&DeeUInt8_Type; break;
    DEE_PARSE_TOKENID_CASE_UINT16 type_result = ((type_flags&FLAG_EXPLICIT_ATOM)!=0)?(DeeTypeObject *)&DeeAtomicUInt16_Type:(DeeTypeObject *)&DeeUInt16_Type; break;
    DEE_PARSE_TOKENID_CASE_UINT32 type_result = ((type_flags&FLAG_EXPLICIT_ATOM)!=0)?(DeeTypeObject *)&DeeAtomicUInt32_Type:(DeeTypeObject *)&DeeUInt32_Type; break;
    DEE_PARSE_TOKENID_CASE_UINT64 type_result = ((type_flags&FLAG_EXPLICIT_ATOM)!=0)?(DeeTypeObject *)&DeeAtomicUInt64_Type:(DeeTypeObject *)&DeeUInt64_Type; break;
    default: DEE_BUILTIN_UNREACHABLE();
   }
   goto next;

  DEE_PARSE_TOKENID_CASE_INT8
  DEE_PARSE_TOKENID_CASE_INT16
  DEE_PARSE_TOKENID_CASE_INT32
  DEE_PARSE_TOKENID_CASE_INT64
#if 1
   if ((type_flags&FLAG_EXPLICIT_INT)!=0 ||
       (type_flags&FLAG_EXPLICIT_LSIZE)!=0
       ) break;
#else
   if ((type_flags&FLAG_EXPLICIT_INT)!=0) goto int_used;
   if ((type_flags&FLAG_EXPLICIT_LSIZE)!=0) goto size_used;
#endif
   type_flags |= (FLAG_EXPLICIT_INT|FLAG_EXPLICIT_LSIZE);
   switch (token.tk_id) {
    DEE_PARSE_TOKENID_CASE_INT8  _DeeType_ADD_INT8(type_result); break;
    DEE_PARSE_TOKENID_CASE_INT16 _DeeType_ADD_INT16(type_result); break;
    DEE_PARSE_TOKENID_CASE_INT32 _DeeType_ADD_INT32(type_result); break;
    DEE_PARSE_TOKENID_CASE_INT64 _DeeType_ADD_INT64(type_result); break;
    default: DEE_BUILTIN_UNREACHABLE();
   }
   goto next;

  default:
   // Parse a variable if we didn't get a type
   if (!type_result) {
    Dee_DECREF(ast_token);
    return DeeXAst_ParseVariableLookup(vardecl_mode,attr,DEE_PARSER_ARGS);
   }
   break;
 }
#undef FLAG_EXPLICIT_LSIZE
#undef FLAG_EXPLICIT_SIZE
#undef FLAG_EXPLICIT_INT
#undef FLAG_EXPLICIT_ATOM
#undef FLAG_EXPLICIT_SIGN
 DEE_ASSERT(type_result); // Should always be non-NULL
end: result = DeeXAst_NewConst(ast_token,(DeeObject *)type_result);
 Dee_DECREF(ast_token);
 return result;
}

DEE_DECL_END
