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
#ifndef GUARD_DEEMON_VALUE_C
#define GUARD_DEEMON_VALUE_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// include/*
#include <deemon/bool.h>
#include <deemon/dict.h>
#include <deemon/error.h>
#include <deemon/float.h>
#include <deemon/integer.h>
#include <deemon/list.h>
#include <deemon/none.h>
#include <deemon/set.h>
#include <deemon/string.h>
#include <deemon/structured.h>
#include <deemon/tuple.h>
#include <deemon/value.h>
#include <deemon/optional/std/string.h>

// src/*
#include <deemon/__min_int_t.inl>

// */ (nano...)


DEE_DECL_BEGIN

#ifdef DEE_DEBUG
#define DEE_BUILDVAL_INVALID_FORMAT() \
 do{DEE_LDEBUG("%s(%d) : Invalid build_value format : %s (near %.5q)\n",__FILE__,__LINE__,*fmt_begin,fmt-2);DEE_ABNORMAL_TERMINATION();}while(0)
#else /* DEE_DEBUG */
#define DEE_BUILDVAL_INVALID_FORMAT   DEE_BUILTIN_UNREACHABLE
#endif /* !DEE_DEBUG */

static void _Dee_VBuildValue_RunRemainder(
 char const **fmt_begin, va_list *args) {
 char const *fmt = *fmt_begin;
again: switch (*fmt++) {
  case '(':
   while (*fmt != ')') {
    DEE_ASSERT(*fmt);
    _Dee_VBuildValue_RunRemainder(&fmt,args);
   }
   ++fmt;
   break;
  case '[':
   while (*fmt != ']') {
    DEE_ASSERT(*fmt);
    _Dee_VBuildValue_RunRemainder(&fmt,args);
   }
   ++fmt;
   break;
  case '<':
   while (*fmt != '>') {
    DEE_ASSERT(*fmt);
    _Dee_VBuildValue_RunRemainder(&fmt,args);
   }
   ++fmt;
   break;
  case '{':
   while (*fmt != '}') {
    DEE_ASSERT(*fmt);
    _Dee_VBuildValue_RunRemainder(&fmt,args);
   }
   ++fmt;
   break;
#define GOTO_X(sizeof)  goto DEE_PP_CAT_2(skip_,DEE_PP_MUL8(sizeof))
  case '%': goto again;
  case 'c': GOTO_X(DEE_TYPES_SIZEOF_CHAR); goto end;
  case 'p': GOTO_X(DEE_TYPES_SIZEOF_POINTER); goto end;
  case 'f':
#if 0
   (void)DEE_VA_ARG(*args,float); goto end;
#else
   DEE_ATTRIBUTE_FALLTHROUGH
#endif
  case 'D': (void)DEE_VA_ARG(*args,double); goto end;
  case 'L': switch (*fmt++) {
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
   case 'D': (void)DEE_VA_ARG(*args,long double); goto end;
#else
   case 'D': (void)DEE_VA_ARG(*args,double); goto end;
#endif
   default: DEE_BUILDVAL_INVALID_FORMAT();
  } break;
  case 'b': case 'd': case 'i':
  case 'u': case 'x': case 'X': GOTO_X(DEE_TYPES_SIZEOF_INT);
  case 'j': switch (*fmt++) {
   case 'b': case 'd': case 'i':
   case 'u': case 'x': case 'X': GOTO_X(DEE_TYPES_SIZEOF_INTMAX_T);
   default: DEE_BUILDVAL_INVALID_FORMAT();
  } break;
  case 'z': switch (*fmt++) {
   case 'b': case 'd': case 'i':
   case 'u': case 'x': case 'X': GOTO_X(DEE_TYPES_SIZEOF_SIZE_T);
   default: DEE_BUILDVAL_INVALID_FORMAT();
  } break;
  case 'I': switch (*fmt++) {
   case 'b': case 'd': case 'i':
   case 'u': case 'x': case 'X': GOTO_X(DEE_TYPES_SIZEOF_SIZE_T);
   case '8': switch (*fmt++) {
    case 'b': case 'd': case 'i': 
    case 'u': case 'x': case 'X':skip_8:
     (void)DEE_VA_ARG(*args,DEE_MININT_T(1)); goto end;
    default: DEE_BUILDVAL_INVALID_FORMAT();
   } break;
   case '1': switch (*fmt++) {
    case '6': switch (*fmt++) {
     case 'b': case 'd': case 'i': 
     case 'u': case 'x': case 'X':skip_16:
      (void)DEE_VA_ARG(*args,DEE_MININT_T(2)); goto end;
     default: DEE_BUILDVAL_INVALID_FORMAT();
    } break;
    default: DEE_BUILDVAL_INVALID_FORMAT();
   } break;
   case '3': switch (*fmt++) {
    case '2': switch (*fmt++) {
     case 'b': case 'd': case 'i': 
     case 'u': case 'x': case 'X':skip_32:
      (void)DEE_VA_ARG(*args,DEE_MININT_T(4)); goto end;
     default: DEE_BUILDVAL_INVALID_FORMAT();
    } break;
    default: DEE_BUILDVAL_INVALID_FORMAT();
   } break;
   case '6': switch (*fmt++) {
    case '4': switch (*fmt++) {
     case 'b': case 'd': case 'i': 
     case 'u': case 'x': case 'X':skip_64:
      (void)DEE_VA_ARG(*args,DEE_MININT_T(8)); goto end;
     default: DEE_BUILDVAL_INVALID_FORMAT();
    } break;
    default: DEE_BUILDVAL_INVALID_FORMAT();
   } break;
   default: DEE_BUILDVAL_INVALID_FORMAT();
  } break;
  case 't': switch (*fmt++) {
   case 'b': case 'd': case 'i':
   case 'u': case 'x': case 'X': GOTO_X(DEE_TYPES_SIZEOF_SIZE_T);
   default: DEE_BUILDVAL_INVALID_FORMAT();
  } break;
  case 'l': switch (*fmt++) {
   case 'c': GOTO_X(DEE_TYPES_SIZEOF_WCHAR_T);
   case 'b': case 'd': case 'i':
   case 'u': case 'x': case 'X': GOTO_X(DEE_TYPES_SIZEOF_LONG);
   case 'l': switch (*fmt++) {
    case 'd': case 'i': case 'u':
    case 'b': case 'x': case 'X': GOTO_X(DEE_TYPES_SIZEOF_LLONG);
    default: DEE_BUILDVAL_INVALID_FORMAT();
   } break;
   default: DEE_BUILDVAL_INVALID_FORMAT();
  } break;
  case 'h': switch (*fmt++) {
   case 'b': case 'd': case 'i':
   case 'u': case 'x': case 'X': GOTO_X(DEE_TYPES_SIZEOF_SHORT);
   case 'h': switch (*fmt++) {
    case 'b': case 'd': case 'i':
    case 'u': case 'x': case 'X': GOTO_X(DEE_TYPES_SIZEOF_CHAR);
    default: DEE_BUILDVAL_INVALID_FORMAT();
   } break;
   default: DEE_BUILDVAL_INVALID_FORMAT();
  } break;
  case '.': switch (*fmt++) {
   case '*': switch (*fmt++) {
    case 'l':
#ifdef DEE_DEBUG
     if (*fmt != 's') DEE_BUILDVAL_INVALID_FORMAT();
#endif
     ++fmt;
     DEE_ATTRIBUTE_FALLTHROUGH
    case 's':
     (void)DEE_VA_ARG(*args,int);
     GOTO_X(DEE_TYPES_SIZEOF_POINTER);
    default: DEE_BUILDVAL_INVALID_FORMAT();
   } break;
   default: DEE_BUILDVAL_INVALID_FORMAT();
  } break;
  case 's': GOTO_X(DEE_TYPES_SIZEOF_POINTER);
  case 'O': case 'K': case 'R':
   Dee_XDECREF(DEE_VA_ARG(*args,DeeObject *));
   goto end;
  case 'k': case 'r': case 'o': GOTO_X(DEE_TYPES_SIZEOF_POINTER);
  default: goto end;
 }
end: *fmt_begin = fmt;
}


static DEE_A_RET_EXCEPT_REF DeeObject *_Dee_VBuildValueOne(
 char const **fmt_begin, va_list *args) {
 DeeObject *result,*temp; int error;
 char const *fmt;
 DEE_ASSERT(fmt_begin && *fmt_begin);
 fmt = *fmt_begin;
again: switch (*fmt++) {
  case 0: result = DeeNone_New(); break;
  case '%': goto again; // ignored
  case '(':
   Dee_INCREF(result = Dee_EmptyTuple);
   while (*fmt != ')') {
    DEE_ASSERT(*fmt);
    if ((temp = _Dee_VBuildValueOne(&fmt,args)) == NULL) {
err_r: Dee_DECREF(result); goto err;
    }
    error = DeeTuple_Append(&result,temp);
    Dee_DECREF(temp);
    if (error == -1) goto err_r;
   }
   ++fmt;
   break;
  case '[':
   if DEE_UNLIKELY((result = DeeList_New()) == NULL) goto err;
   while (*fmt != ']') {
    DEE_ASSERT(*fmt);
    if DEE_UNLIKELY((temp = _Dee_VBuildValueOne(&fmt,args)) == NULL) goto err_r;
    error = DeeList_Append(result,temp);
    Dee_DECREF(temp);
    if DEE_UNLIKELY(error != 0) goto err_r;
   }
   ++fmt;
   break;
  case '<':
   if DEE_UNLIKELY((result = DeeSet_New()) == NULL) goto err;
   while (*fmt != '>') {
    DEE_ASSERT(*fmt);
    if DEE_UNLIKELY((temp = _Dee_VBuildValueOne(&fmt,args)) == NULL) goto err_r;
    error = DeeSet_Insert(result,temp);
    Dee_DECREF(temp);
    if DEE_UNLIKELY(error != 0) goto err_r;
   }
   ++fmt;
   break;
  case '{':
   if DEE_UNLIKELY((result = DeeDict_New()) == NULL) goto err;
   while (*fmt != '}') {
    DeeObject *key,*item;
    DEE_ASSERT(*fmt);
    if DEE_UNLIKELY((key = _Dee_VBuildValueOne(&fmt,args)) == NULL) goto err_r;
    DEE_ASSERT(*fmt && *fmt != '}' && "Incorrect about of elements in dict (requires 2*x)");
    if DEE_UNLIKELY((item = _Dee_VBuildValueOne(&fmt,args)) == NULL) { Dee_DECREF(key); goto err_r; }
    error = DeeDict_SetItem(result,key,item);
    Dee_DECREF(item);
    Dee_DECREF(key);
    if DEE_UNLIKELY(error != 0) goto err_r;
   }
   ++fmt;
   break;
  case 'c':
#if 1
   result = DeeString_Character(DEE_VA_ARG(*args,int));
#else
   result = DeeString_Character(DEE_VA_ARG(*args,char));
#endif
   Dee_INCREF(result);
   break;
  case 'p':
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   result = DeeVoidPointer_New(DEE_VA_ARG(*args,void *));
   goto check_res;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   DeeError_NotImplemented_str("DEE_CONFIG_RUNTIME_HAVE_POINTERS");
   goto err;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#define GOTO_BOOL_X(sizeof)  goto DEE_PP_CAT_2(b_,DEE_PP_MUL8(sizeof))
#define GOTO_INT_X(sizeof)   goto DEE_PP_CAT_2(o_i,DEE_PP_MUL8(sizeof))
#define GOTO_INT_UX(sizeof)  goto DEE_PP_CAT_2(o_ui,DEE_PP_MUL8(sizeof))
  case 'f':
#if 1
   result = DeeObject_New(float,(float)DEE_VA_ARG(*args,double));
#else
   result = DeeObject_New(float,DEE_VA_ARG(*args,float));
#endif
check_res: if DEE_UNLIKELY(!result) goto err;
   break;
  case 'D':
   result = DeeObject_New(double,DEE_VA_ARG(*args,double));
   goto check_res;
  case 'L': switch (*fmt++) {
   case 'D':
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
    result = DeeObject_New(ldouble,DEE_VA_ARG(*args,long double));
#else
    result = DeeObject_New(double,DEE_VA_ARG(*args,double));
#endif
    goto check_res;
   default: DEE_BUILDVAL_INVALID_FORMAT();
  } break;
  case 'b': GOTO_BOOL_X(DEE_TYPES_SIZEOF_INT);
  case 'd': case 'i': GOTO_INT_X(DEE_TYPES_SIZEOF_INT);
  case 'u': case 'x': case 'X': GOTO_INT_UX(DEE_TYPES_SIZEOF_INT);
  case 'j': switch (*fmt++) {
   case 'b': GOTO_BOOL_X(DEE_TYPES_SIZEOF_INTMAX_T);
   case 'd': case 'i': GOTO_INT_X(DEE_TYPES_SIZEOF_INTMAX_T);
   case 'u': case 'x': case 'X': GOTO_INT_UX(DEE_TYPES_SIZEOF_INTMAX_T);
   default: DEE_BUILDVAL_INVALID_FORMAT();
  } break;
  case 'z': switch (*fmt++) {
   case 'b': GOTO_BOOL_X(DEE_TYPES_SIZEOF_SIZE_T);
   case 'd': case 'i': case 'u': case 'x': case 'X': GOTO_INT_UX(DEE_TYPES_SIZEOF_SIZE_T);
   default: DEE_BUILDVAL_INVALID_FORMAT();
  } break;
  case 'I': switch (*fmt++) {
   case 'b': GOTO_BOOL_X(DEE_TYPES_SIZEOF_SIZE_T);
   case 'd': case 'i': case 'u': case 'x': case 'X': GOTO_INT_X(DEE_TYPES_SIZEOF_SIZE_T);
   case '8': switch (*fmt++) {
    case 'b': b_8: result = DeeBool_New(DEE_VA_ARG(*args,DEE_MINUINT_T(1))); goto check_res;
    case 'd': case 'i': o_i8: result = DeeObject_New(Dee_int8_t,(Dee_int8_t)DEE_VA_ARG(*args,DEE_MININT_T(1))); goto check_res;
    case 'u': case 'x': case 'X': o_ui8: result = DeeObject_New(Dee_uint8_t,(Dee_uint8_t)DEE_VA_ARG(*args,DEE_MINUINT_T(1))); goto check_res;
    default: DEE_BUILDVAL_INVALID_FORMAT();
   } break;
   case '1': switch (*fmt++) {
    case '6': switch (*fmt++) {
     case 'b': b_16: result = DeeBool_New((Dee_uint16_t)DEE_VA_ARG(*args,DEE_MINUINT_T(2))); goto check_res;
     case 'd': case 'i': o_i16: result = DeeObject_New(Dee_int16_t,(Dee_int16_t)DEE_VA_ARG(*args,DEE_MININT_T(2))); goto check_res;
     case 'u': case 'x': case 'X': o_ui16: result = DeeObject_New(Dee_uint16_t,(Dee_uint16_t)DEE_VA_ARG(*args,DEE_MINUINT_T(2))); goto check_res;
     default: DEE_BUILDVAL_INVALID_FORMAT();
    } break;
    default: DEE_BUILDVAL_INVALID_FORMAT();
   } break;
   case '3': switch (*fmt++) {
    case '2': switch (*fmt++) {
     case 'b': b_32: result = DeeBool_New(DEE_VA_ARG(*args,DEE_MINUINT_T(4))); goto check_res;
     case 'd': case 'i': o_i32: result = DeeObject_New(Dee_int32_t,(Dee_int32_t)DEE_VA_ARG(*args,DEE_MININT_T(4))); goto check_res;
     case 'u': case 'x': case 'X': o_ui32: result = DeeObject_New(Dee_uint32_t,(Dee_uint32_t)DEE_VA_ARG(*args,DEE_MINUINT_T(4))); goto check_res;
     default: DEE_BUILDVAL_INVALID_FORMAT();
    } break;
    default: DEE_BUILDVAL_INVALID_FORMAT();
   } break;
   case '6': switch (*fmt++) {
    case '4': switch (*fmt++) {
     case 'b': b_64: result = DeeBool_New(DEE_VA_ARG(*args,DEE_MINUINT_T(8))); goto check_res;
     case 'd': case 'i': o_i64: result = DeeObject_New(Dee_int64_t,(Dee_int64_t)DEE_VA_ARG(*args,DEE_MININT_T(8))); goto check_res;
     case 'u': case 'x': case 'X': o_ui64: result = DeeObject_New(Dee_uint64_t,(Dee_uint64_t)DEE_VA_ARG(*args,DEE_MINUINT_T(8))); goto check_res;
     default: DEE_BUILDVAL_INVALID_FORMAT();
    } break;
    default: DEE_BUILDVAL_INVALID_FORMAT();
   } break;
   default: DEE_BUILDVAL_INVALID_FORMAT();
  } break;
  case 't': switch (*fmt++) {
   case 'b': GOTO_BOOL_X(DEE_TYPES_SIZEOF_SIZE_T);
   case 'd': case 'i': case 'u': case 'x': case 'X': GOTO_INT_X(DEE_TYPES_SIZEOF_SIZE_T);
   default: DEE_BUILDVAL_INVALID_FORMAT();
  } break;
  case 'l': switch (*fmt++) {
   case 's':
    if DEE_UNLIKELY((result = DeeWideString_New(DEE_VA_ARG(*args,Dee_WideChar *))) == NULL) goto err;
    break;
   case 'c':
    if DEE_UNLIKELY((result = DeeWideString_NewSized(1)) == NULL) goto err;
#if DEE_TYPES_WCHAR_T_SIGNED
    DeeWideString_STR(result)[0] = (Dee_WideChar)DEE_VA_ARG(*args,DEE_MININT_T(DEE_TYPES_SIZEOF_WCHAR_T));
#else
    DeeWideString_STR(result)[0] = (Dee_WideChar)DEE_VA_ARG(*args,DEE_MINUINT_T(DEE_TYPES_SIZEOF_WCHAR_T));
#endif
    break;
   case 'b': GOTO_BOOL_X(DEE_TYPES_SIZEOF_LONG);
   case 'd': case 'i': GOTO_INT_X(DEE_TYPES_SIZEOF_LONG);
   case 'u': case 'x': case 'X': GOTO_INT_UX(DEE_TYPES_SIZEOF_LONG);
   case 'l': switch (*fmt++) {
    case 'd': case 'i': GOTO_INT_X(DEE_TYPES_SIZEOF_LLONG);
    case 'u': case 'b': case 'x': case 'X': GOTO_INT_UX(DEE_TYPES_SIZEOF_LLONG);
    default: DEE_BUILDVAL_INVALID_FORMAT();
   } break;
   default: DEE_BUILDVAL_INVALID_FORMAT();
  } break;
  case 'h': switch (*fmt++) {
   case 'b': GOTO_BOOL_X(DEE_TYPES_SIZEOF_SHORT);
   case 'd': case 'i': GOTO_INT_X(DEE_TYPES_SIZEOF_SHORT);
   case 'u': case 'x': case 'X': GOTO_INT_UX(DEE_TYPES_SIZEOF_SHORT);
   case 'h': switch (*fmt++) {
    case 'b': GOTO_BOOL_X(DEE_TYPES_SIZEOF_CHAR);
    case 'd': case 'i': GOTO_INT_X(DEE_TYPES_SIZEOF_CHAR);
    case 'u': case 'x': case 'X': GOTO_INT_UX(DEE_TYPES_SIZEOF_CHAR);
    default: DEE_BUILDVAL_INVALID_FORMAT();
   } break;
   default: DEE_BUILDVAL_INVALID_FORMAT();
  } break;
  case '.': switch (*fmt++) {
   case '*': switch (*fmt++) {
    case 'l':
#ifdef DEE_DEBUG
     if (*fmt != 's') DEE_BUILDVAL_INVALID_FORMAT();
#endif
     ++fmt;
    {
     unsigned int string_len = DEE_VA_ARG(*args,unsigned int);
     Dee_WideChar const *string = DEE_VA_ARG(*args,Dee_WideChar const *);
     result = DeeWideString_NewWithLength(Dee_WideStrNLen(string,(Dee_size_t)string_len),string);
    } goto check_res;
    case 's': {
     unsigned int string_len = DEE_VA_ARG(*args,unsigned int);
     char const *string = DEE_VA_ARG(*args,char const *);
     result = DeeString_NewWithLength(Dee_StrNLen(string,(Dee_size_t)string_len),string);
    } goto check_res;
    default: DEE_BUILDVAL_INVALID_FORMAT();
   } break;
   default: DEE_BUILDVAL_INVALID_FORMAT();
  } break;
  case 's': {
   char const *string = DEE_VA_ARG(*args,char const *);
   result = DeeString_New(string ? string : "(null)");
  } goto check_res;
  case 'O':
   result = DEE_VA_ARG(*args,DeeObject *);
   goto check_res;
  case 'K':
   if ((temp = DEE_VA_ARG(*args,DeeObject *)) == NULL) goto err;
   result = DeeObject_Str(temp);
   Dee_DECREF(temp);
   goto check_res;
  case 'R':
   if ((temp = DEE_VA_ARG(*args,DeeObject *)) == NULL) goto err;
   result = DeeObject_Repr(temp);
   Dee_DECREF(temp);
   goto check_res;
  case 'k':
   if ((temp = DEE_VA_ARG(*args,DeeObject *)) == NULL) goto err;
   result = DeeObject_Str(temp);
   goto check_res;
  case 'r':
   if ((temp = DEE_VA_ARG(*args,DeeObject *)) == NULL) goto err;
   result = DeeObject_Repr(temp);
   goto check_res;
  case 'o':
   if ((result = DEE_VA_ARG(*args,DeeObject *)) == NULL) goto err;
   Dee_INCREF(result);
   break;

  default:
   DEE_BUILDVAL_INVALID_FORMAT();
 }
 *fmt_begin = fmt;
 return result;
err: _Dee_VBuildValue_RunRemainder(&fmt,args); return NULL;
}


DEE_A_RET_EXCEPT_REF DeeObject *Dee_BuildValue(
 DEE_A_IN_BUILDVALUEF char const *fmt, ...) {
 DeeObject *result; va_list args;
 DEE_VA_START(args,fmt);
 result = _Dee_VBuildValueOne(&fmt,&args);
 DEE_ASSERTF(!*fmt,"Expected end of BuildValue format at %q",fmt);
 DEE_VA_END(args);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeObject *Dee_VBuildValue(
 DEE_A_IN_BUILDVALUEF char const *fmt, DEE_A_IN va_list args) {
#ifdef DEE_DEBUG
 DeeObject *result;
 result = _Dee_VBuildValueOne(&fmt,DEE_VA_ARGPTR(args));
 DEE_ASSERTF(!*fmt,"Expected end of BuildValue format at %q",fmt);
 return result;
#else
 return _Dee_VBuildValueOne(&fmt,DEE_VA_ARGPTR(args));
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *
Dee_BuildTuple(DEE_A_IN_BUILDTUPLEF char const *fmt, ...) {
 DeeObject *result; va_list args;
 DEE_VA_START(args,fmt);
 result = Dee_VBuildTuple(fmt,args);
 DEE_VA_END(args);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *
Dee_VBuildTuple(DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) {
 DeeObject *result,*temp; int error;
 Dee_INCREF(result = Dee_EmptyTuple);
 while (*fmt) {
  if DEE_UNLIKELY((temp = _Dee_VBuildValueOne(&fmt,DEE_VA_ARGPTR(args))) == NULL) {
err_r: Dee_DECREF(result);
   _Dee_VBuildValue_RunRemainder(&fmt,DEE_VA_ARGPTR(args));
   return NULL;
  }
  error = DeeTuple_Append(&result,temp);
  Dee_DECREF(temp);
  if DEE_UNLIKELY(error != 0) goto err_r;
 }
 DEE_ASSERT(DeeObject_Check(result) && DeeTuple_Check(result));
 return result;
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_VALUE_C */
