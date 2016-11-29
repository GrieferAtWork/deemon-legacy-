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
#include "tuple.c"
#endif
#include <deemon/integer.h>
#include <deemon/float.h>
#include <deemon/deemonrun.h>
#include <deemon/structured.h>
#include <deemon/optional/std/stdarg.h>

DEE_DECL_BEGIN

#if 1

#ifdef DEE_DEBUG
#define DEE_UTPLF_INVALID_FORMAT() \
 do{DEE_LDEBUG("%s(%u) : Invalid typef format : %s (near %.5q)\n",__FILE__,__LINE__,*fmt_begin,fmt-2);DEE_ABNORMAL_TERMINATION();}while(0)
#define DEE_UTPLF_ASSERT(x) do{if(!(x))DEE_UTPLF_INVALID_FORMAT();}while(0)
#else /* DEE_DEBUG */
#define DEE_UTPLF_INVALID_FORMAT   DEE_BUILTIN_UNREACHABLE
#define DEE_UTPLF_ASSERT(x)        do{}while(0)
#endif /* !DEE_DEBUG */


DEE_COMPILER_MSVC_WARNING_PUSH(4061)
DEE_A_RET_EXCEPT(NULL) DeeTypeObject *_DeeTuple_UnpackType_Tyname(
 DEE_A_IN_Z char const **fmt_begin, DEE_A_OUT_OPT int *do_bool) {
 DeeTypeObject *result;
 char const *fmt = *fmt_begin;
 enum {
  len_none,len_l,len_ll,len_h,len_hh,
  len_I,len_I8,len_I16,len_I32,len_I64,len_L
 } len = len_none;
len_again:
 switch (*fmt) {
  case 'a': ++fmt; goto len_again; // Ignore the atomic modifier (for now)
  case 'L': ++fmt; len = len_L; break;
  case 'l': if (*++fmt == 'l') len = len_ll,++fmt; else len = len_l; break;
  case 'h': if (*++fmt == 'h') len = len_hh,++fmt; else len = len_h; break;
  case 'I': switch (*++fmt) {
   case '8': len = len_I8; ++fmt; break;
   case '1': DEE_UTPLF_ASSERT(fmt[1] == '6'); fmt += 2; len = len_I16; break;
   case '3': DEE_UTPLF_ASSERT(fmt[1] == '2'); fmt += 2; len = len_I32; break;
   case '6': DEE_UTPLF_ASSERT(fmt[1] == '4'); fmt += 2; len = len_I64; break;
   default: len = len_I; break;
  } break;
  default: break;
 }

 switch (*fmt++) {
  case 'd': case 'i': switch (len) {
   case len_l:   result = DeeObject_TYPE(long); break;
   case len_ll:  result = DeeObject_TYPE(llong); break;
   case len_h:   result = DeeObject_TYPE(short); break;
   case len_hh:  result = DeeObject_TYPE(schar); break;
   case len_I:   result = DeeObject_TYPE(ssize_t); break;
   case len_I8:  result = DeeObject_TYPE(int8_t); break;
   case len_I16: result = DeeObject_TYPE(int16_t); break;
   case len_I32: result = DeeObject_TYPE(int32_t); break;
   case len_I64: result = DeeObject_TYPE(int64_t); break;
   default:      result = DeeObject_TYPE(int); break;
  } break;
  case 'b': if (do_bool) *do_bool = 1; DEE_ATTRIBUTE_FALLTHROUGH
  case 'u': case 'x': case 'X': switch (len) {
   case len_l:   result = DeeObject_TYPE(ulong); break;
   case len_ll:  result = DeeObject_TYPE(ullong); break;
   case len_h:   result = DeeObject_TYPE(ushort); break;
   case len_hh:  result = DeeObject_TYPE(uchar); break;
   case len_I:   result = DeeObject_TYPE(size_t); break;
   case len_I8:  result = DeeObject_TYPE(uint8_t); break;
   case len_I16: result = DeeObject_TYPE(uint16_t); break;
   case len_I32: result = DeeObject_TYPE(uint32_t); break;
   case len_I64: result = DeeObject_TYPE(uint64_t); break;
   default:      result = DeeObject_TYPE(uint); break;
  } break;
  case 'f': result = DeeObject_TYPE(float); break;
  case 'D': result = len == len_L ? DeeObject_TYPE(ldouble) : DeeObject_TYPE(double); break;
  case 'o': result = &DeeObject_Type; break;
  case 'n': case 'v': result = (DeeTypeObject *)&DeeNone_Type; break;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  case 'p': result = DeeType_Pointer((DeeTypeObject *)&DeeNone_Type); break;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  case 'p': case 's': DeeError_NotImplemented_str("DEE_CONFIG_LANGUAGE_HAVE_POINTERS"); result = NULL; break;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  case 's':
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  case 'c':
   switch (len) {
    case len_l: result = DeeObject_TYPE(wchar_t); break;
    case len_I8: result = DeeObject_TYPE(Dee_Utf8Char); break;
    case len_I16: result = DeeObject_TYPE(Dee_Utf16Char); break;
    case len_I32: result = DeeObject_TYPE(Dee_Utf32Char); break;
    default: result = DeeObject_TYPE(char); break;
   }
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   if (fmt[-1] == 's') result = DeeType_Pointer(result);
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   break;
  default: DEE_UTPLF_INVALID_FORMAT();
 }
 *fmt_begin = fmt;
 return result;
}

#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
Dee_size_t _DeeTuple_UnpackType_Intname(
 DEE_A_IN_Z char const **fmt_begin) {
 Dee_size_t result;
 char const *fmt = *fmt_begin;
 result = 0;
 DEE_UTPLF_ASSERT(*fmt >= '0' && *fmt <= '9');
 do {
  result = (Dee_size_t)(result*10+(Dee_size_t)(*fmt-'0'));
  ++fmt;
 } while (*fmt >= '0' && *fmt <= '9');
 *fmt_begin = fmt;
 return result;
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS || \
    DEE_CONFIG_RUNTIME_HAVE_ARRAYS || \
    DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
DEE_A_RET_EXCEPT(NULL) DeeTypeObject *_DeeTuple_UnpackTypeOne(
 DEE_A_IN_Z char const **fmt_begin, DEE_A_OUT_OPT int *do_bool) {
 DeeTypeObject *result;
 char const *fmt;
 if ((result = _DeeTuple_UnpackType_Tyname(fmt_begin,do_bool)) == NULL) return NULL;
 fmt = *fmt_begin;
again:
 switch (*fmt) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  case '*': if (do_bool) *do_bool = 0; ++fmt; if DEE_UNLIKELY((result = DeeType_Pointer(result)) == NULL) return NULL; goto again;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
  case '[': if (do_bool) *do_bool = 0; {
   Dee_size_t array_size; ++fmt;
   array_size = _DeeTuple_UnpackType_Intname(&fmt);
   if DEE_UNLIKELY((result = DeeType_Array(result,array_size)) == NULL) return NULL;
   DEE_UTPLF_ASSERT(*fmt == ']'); ++fmt;
   goto again;
  }
#endif /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
  case '(': if (do_bool) *do_bool = 0; {
   Dee_funflags_t flags = DEE_FUNCTIONFLAGS_NONE;
   DeeTypeObject **argty_v,**new_argty_v; Dee_size_t argty_c,argty_a;
   ++fmt;
   if (*fmt == '!') { ++fmt; } // TODO: Parse function flags
   if (*fmt == ')') {
    result = DeeType_ForeignFunction(result,flags,0,NULL);
   } else {
    DeeTypeObject *argty;
    argty_v = NULL; argty_c = 0; argty_a = 0;
    do {
     if DEE_UNLIKELY((argty = _DeeTuple_UnpackTypeOne(&fmt,NULL)) == NULL) { free(argty_v); return NULL; }
     if DEE_UNLIKELY(argty_a == argty_c) {
      if DEE_UNLIKELY(!argty_a) argty_a = 2; else argty_a *= 2;
      while DEE_UNLIKELY((new_argty_v = (DeeTypeObject **)realloc_nz(
       argty_v,argty_a*sizeof(DeeTypeObject *))) == NULL) {
       if DEE_LIKELY(Dee_CollectMemory()) continue;
       DeeError_NoMemory();
       free(argty_v);
       return NULL;
      }
      argty_v = new_argty_v;
     }
     argty_v[argty_c++] = argty;
     if (*fmt == ',') ++fmt;
    } while (*fmt != ')');
    result = DeeType_ForeignFunction(result,flags,argty_c,(DeeTypeObject const *const *)argty_v);
    free(argty_v);
    if (!result) return NULL;
   }
   DEE_UTPLF_ASSERT(*fmt == ')'); ++fmt;
  } goto again;
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
  default: break;
 }
 *fmt_begin = fmt;
 return result;
}
#else /* ... */
#define _DeeTuple_UnpackTypeOne  _DeeTuple_UnpackType_Tyname
#endif /* ... */
#undef DEE_UTPLF_ASSERT
#undef DEE_UTPLF_INVALID_FORMAT

DEE_A_RET_EXCEPT(-1) int DeeTuple_UnpackInfo(
 DEE_A_IN_UNPACKTUPLEF char const *fmt, DEE_A_OUT_OPT Dee_size_t *argc_min,
 DEE_A_OUT_OPT Dee_size_t *argc_max, DEE_A_OUT_OPT char const **func_name) {
 int opt = 0; Dee_size_t argc = 0;
 while DEE_LIKELY(*fmt) {
  if (*fmt == ':') { ++fmt; break; }
  if (*fmt == '|') { ++fmt; if (!opt) { if DEE_LIKELY(argc_min) *argc_min = argc; opt = 1; } }
  if (*fmt == '%') ++fmt;
  if DEE_UNLIKELY(!_DeeTuple_UnpackTypeOne(&fmt,NULL)) return -1;
  ++argc;
 }
 if (!opt && DEE_LIKELY(argc_min)) *argc_min = argc;
 if DEE_LIKELY(argc_max) *argc_max = argc;
 if DEE_LIKELY(func_name) *func_name = *fmt ? fmt : NULL;
 return 0;
}

DEE_A_RET_EXCEPT(-1) int _DeeTuple_VUnpack(
 DEE_A_IN_OBJECT(DeeTupleObject) *self,
 DEE_A_IN_UNPACKTUPLEF char const *fmt, DEE_A_INOUT va_list args) {
 DeeTypeObject *obtype; DeeObject *elem,**iter,**end; int opt = 0;
 Dee_size_t argi,minargc,maxargc; char const *func_name;
 char const *fmt_start = fmt; int do_bool;
 end = (iter = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
next:
 while (*fmt && *fmt != ':') {
  if (*fmt == '|') opt = 1,++fmt;
  if (*fmt == '%') ++fmt;
  if (iter == end) {
   if DEE_UNLIKELY(!opt) {
err_invargc:
    if DEE_UNLIKELY(DeeTuple_UnpackInfo(fmt_start,&minargc,&maxargc,&func_name) != 0) return -1;
    if DEE_UNLIKELY(!func_name) func_name = "?";
    DeeError_TypeError_ArgCountExpectedNamedMinMax(
     func_name,minargc,maxargc,DeeTuple_SIZE(self));
    return -1;
   }
   return 0;
  }
  elem = *iter++;
  DEE_ASSERT(DeeObject_Check(elem));
  do_bool = 0;
  if DEE_UNLIKELY((obtype = _DeeTuple_UnpackTypeOne(&fmt,&do_bool)) == NULL) return -1;
  if DEE_LIKELY(DeeType_HAS_SLOT(obtype,tp_marshal) &&
                DeeUUID_CHECK_INTERNAL(&DeeType_GET_SLOT(obtype,tp_marshal)->tp_uuid)) {
   switch (DeeUUID_GET_INTERNAL(&DeeType_GET_SLOT(obtype,tp_marshal)->tp_uuid)) {
    case DEE_MARSHALTYPEID_object:
     *DEE_VA_ARG(args,DeeObject **) = elem;
     goto next;
    case DEE_MARSHALTYPEID_int8:
    case DEE_MARSHALTYPEID_uint8:
    case DEE_MARSHALTYPEID_atomic_int8:
    case DEE_MARSHALTYPEID_atomic_uint8: {
     int8_t *resp = DEE_VA_ARG(args,int8_t *);
     if DEE_UNLIKELY(DEE_UNLIKELY(do_bool) ? ((*resp = (int8_t)DeeObject_Bool(elem)) < 0)
                                           : (DeeObject_Cast(int8_t,elem,resp) != 0)) {
err_arg_type:
      if (DeeError_Catch(&DeeErrorType_NotImplemented)) {
       argi = (Dee_size_t)(iter-DeeTuple_ELEM(self));
       if DEE_UNLIKELY(DeeTuple_UnpackInfo(fmt_start,NULL,NULL,&func_name) != 0) return -1;
       if DEE_UNLIKELY(!func_name) func_name = "?";
       DeeError_SetStringf(&DeeErrorType_TypeError,
                           "%q expects %q for argument %Iu, but got %q",
                           func_name,DeeType_NAME(obtype),argi,DeeType_NAME(Dee_TYPE(elem)));
      }
      return -1;
     }
     goto next;
    }
    case DEE_MARSHALTYPEID_int16:
    case DEE_MARSHALTYPEID_uint16:
    case DEE_MARSHALTYPEID_atomic_int16:
    case DEE_MARSHALTYPEID_atomic_uint16: {
     int16_t *resp = DEE_VA_ARG(args,int16_t *);
     if DEE_UNLIKELY(DEE_UNLIKELY(do_bool) ? ((*resp = (int16_t)DeeObject_Bool(elem)) < 0)
                                           : (DeeObject_Cast(int16_t,elem,resp) != 0)
                                           ) goto err_arg_type;
     goto next;
    }
    case DEE_MARSHALTYPEID_int32:
    case DEE_MARSHALTYPEID_uint32:
    case DEE_MARSHALTYPEID_atomic_int32:
    case DEE_MARSHALTYPEID_atomic_uint32: {
     int32_t *resp = DEE_VA_ARG(args,int32_t *);
     if DEE_UNLIKELY(DEE_UNLIKELY(do_bool) ? ((*resp = (int32_t)DeeObject_Bool(elem)) < 0)
                                           : (DeeObject_Cast(int32_t,elem,resp) != 0)
                                           ) goto err_arg_type;
     goto next;
    }
    case DEE_MARSHALTYPEID_int64:
    case DEE_MARSHALTYPEID_uint64:
    case DEE_MARSHALTYPEID_atomic_int64:
    case DEE_MARSHALTYPEID_atomic_uint64: {
     int64_t *resp = DEE_VA_ARG(args,int64_t *);
     if DEE_UNLIKELY(DEE_UNLIKELY(do_bool) ? ((*resp = (int64_t)DeeObject_Bool(elem)) < 0)
                                           : (DeeObject_Cast(int64_t,elem,resp) != 0)
                                           ) goto err_arg_type;
     goto next;
    }
    default: break;
   }
  }
  if (obtype == (DeeTypeObject *)&DeeFloat_Type) {
   Dee_rt_float *resp = DEE_VA_ARG(args,Dee_rt_float *);
   if (DeeObject_Cast(Dee_rt_float,elem,resp) != 0) goto err_arg_type;
  } else if (obtype == (DeeTypeObject *)&DeeDouble_Type) {
   Dee_rt_double *resp = DEE_VA_ARG(args,Dee_rt_double *);
   if (DeeObject_Cast(Dee_rt_double,elem,resp) != 0) goto err_arg_type;
  } else if (obtype == (DeeTypeObject *)&DeeLDouble_Type) {
   Dee_rt_ldouble *resp = DEE_VA_ARG(args,Dee_rt_ldouble *);
   if (DeeObject_Cast(Dee_rt_ldouble,elem,resp) != 0) goto err_arg_type;
  }
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  else if (DeePointerType_Check(obtype)) {
   void **resp = DEE_VA_ARG(args,void **);
   if (DeeObject_GetPointerEx(elem,DeePointerType_BASE(obtype),resp) != 0) goto err_arg_type;
  }
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  else {
   DeeError_SetStringf(&DeeErrorType_TypeError,
                       "Cannot unpack type: %q",
                       DeeType_NAME(obtype));
   return -1;
  }
 }
 if (iter != end) goto err_invargc;
 return 0;
}

#else

#ifdef DEE_DEBUG
#define DEE_TUPLE_INVALID_FORMAT() \
 do{DEE_LDEBUG("%s(%u) : Invalid Tuple unpack format : %s (near %.5q)\n",__FILE__,__LINE__,fmt_start,fmt-2);DEE_ABNORMAL_TERMINATION();}while(0)
#else /* DEE_DEBUG */
#define DEE_TUPLE_INVALID_FORMAT   DEE_BUILTIN_UNREACHABLE
#endif /* !DEE_DEBUG */

#define DEE_CONFIG_TUPLE_PERCENT_OPTIONAL 1

DEE_A_RET_EXCEPT(-1) int DeeTuple_UnpackInfo(
 DEE_A_IN_UNPACKTUPLEF char const *fmt, DEE_A_OUT_OPT Dee_size_t *argc_min,
 DEE_A_OUT_OPT Dee_size_t *argc_max, DEE_A_OUT_OPT char const **func_name) {
 Dee_size_t argc = 0; int opt = 0;
 while (1) switch (*fmt++) {
  case '%':
#if DEE_CONFIG_TUPLE_PERCENT_OPTIONAL
   break;
#else /* DEE_CONFIG_TUPLE_PERCENT_OPTIONAL */
   switch (*fmt++) {
#endif /* !DEE_CONFIG_TUPLE_PERCENT_OPTIONAL */
  case 'c': case 'p': case 'f': case 'D':
  case 'b': case 'd': case 'i': case 'u':
  case 'x': case 'X': case 's': case 'k':
  case 'K': case 'r': case 'R': case 'o':
  case 'O': ++argc; break;
  case 'L':
   if (*fmt == 'D') ++fmt;
   ++argc;
   break;

  case 'j': case 'z': case 't':
   switch (*fmt) {
    case 'b': case 'd': case 'i':
    case 'u': case 'x': case 'X':
     ++fmt;
     break;
    default:
     break;
   }
   ++argc;
   break;

  case 'I':
   switch (*fmt) {
    case 'b': case 'd': case 'i':
    case 'u': case 'x': case 'X':
     ++fmt;
     break;
    case '8':
     switch (*++fmt) {
      case 'b': case 'd': case 'i':
      case 'u': case 'x': case 'X':
       ++fmt;
       break;
      default: break;
     }
     break;
    case '1':
     if (*++fmt == '6') ++fmt;
     switch (*fmt) {
      case 'b': case 'd': case 'i':
      case 'u': case 'x': case 'X':
       ++fmt;
       break;
      default: break;
     }
     break;
    case '3':
     if (*++fmt == '2') ++fmt;
     switch (*fmt) {
      case 'b': case 'd': case 'i':
      case 'u': case 'x': case 'X':
       ++fmt;
       break;
      default: break;
     }
     break;
    case '6':
     if (*++fmt == '4') ++fmt;
     switch (*fmt) {
      case 'b': case 'd': case 'i':
      case 'u': case 'x': case 'X':
       ++fmt;
       break;
      default: break;
     }
     break;
    default: break;
   }
   ++argc;
   break;

  case 'l':
   switch (*fmt) {
    case 'c': case 'b': case 'd': case 'i':
    case 'u': case 'x': case 'X': case 's':
     ++fmt;
     break;
    case 'l':
     switch (*++fmt) {
      case 'd': case 'i':
      case 'u': case 'b': case 'x': case 'X':
       ++fmt;
       break;
      default: break;
     }
     break;
    default: break;
   }
   ++argc;
   break;

  case 'h':
   switch (*fmt) {
    case 'c': case 'b': case 'd': case 'i':
    case 'u': case 'x': case 'X':
     ++fmt;
     break;
    case 'h':
     switch (*++fmt) {
      case 'd': case 'i':
      case 'u': case 'b': case 'x': case 'X':
       ++fmt;
       break;
      default: break;
     }
     break;
    default: break;
   }
   ++argc;
   break;

  case '.':
   if (*fmt == '*') { ++fmt;
    if (*fmt == 's') ++fmt;
   }
   ++argc;
   break;

#if !DEE_CONFIG_TUPLE_PERCENT_OPTIONAL
  default: break;
  }
  break;
#endif /* !DEE_CONFIG_TUPLE_PERCENT_OPTIONAL */

  case '\0':
  case ':':
   goto end;
  case '|':
   if (!opt) {
    if (argc_min) *argc_min = argc;
    opt = 1;
   }
   break;
  default:
   break;
 }
end:
 if (!opt && argc_min) *argc_min = argc;
 if (argc_max) *argc_max = argc;
 if (func_name) *func_name = *fmt ? fmt : NULL;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int _DeeTuple_VUnpack(
 DEE_A_IN_OBJECT(DeeTupleObject) *self, DEE_A_IN_UNPACKTUPLEF char const *fmt, DEE_A_INOUT va_list args) {
 DeeObject *arg,**argv,**argv_end; char const *fmt_start = fmt; int opt = 0;
 DEE_ASSERT(DeeObject_Check(self) && DeeTuple_Check(self));
 argv_end = (argv = DeeTuple_ELEM(self))+DeeTuple_SIZE(self);
#define GET_ARG() do{if(argv==argv_end)goto inv_argc;arg=*argv++;}while(0)

 while (1) switch (*fmt++) {
  case '%':
#if DEE_CONFIG_TUPLE_PERCENT_OPTIONAL
   break;
#else /* DEE_CONFIG_TUPLE_PERCENT_OPTIONAL */
   switch (*fmt++) {
#endif /* !DEE_CONFIG_TUPLE_PERCENT_OPTIONAL */
#define SIMPLE_CAST(T,get)\
    {T temp; GET_ARG();\
     if (get(arg,&temp) != 0) return -1;\
     *DEE_VA_ARG(args,T *) = temp;}
#define BOOL_CAST(T)\
    {int temp; GET_ARG();\
     if ((temp = DeeObject_Bool(arg)) < 0) return -1;\
     *DEE_VA_ARG(args,T *) = (T)temp;}
    case 'c':
     GET_ARG();
     if (!DeeString_Check(arg)||DeeString_SIZE(arg)!=1) {
ex_chr:
      DeeError_SetStringf(&DeeErrorType_TypeError,
                          "Unexpected type (expected: str[1]; got: %s)",
                          DeeType_NAME(Dee_TYPE(arg)));
      return -1;
     }
     *DEE_VA_ARG(args,char *) = *DeeString_STR(arg);
     break;
    case 'p': {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
     void *temp;
     GET_ARG();
     if (DeeObject_GetVoidPointerEx(arg,&temp) != 0) return -1;
     *DEE_VA_ARG(args,void **) = temp;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
     GET_ARG();
#if DEE_CONFIG_HAVE_ENCODING_WIDE
     if (DeeWideString_Check(arg)) *DEE_VA_ARG(args,Dee_WideChar **) = DeeWideString_STR(arg); else
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
     if (DeeUtf8String_Check(arg)) *DEE_VA_ARG(args,Dee_Utf8Char **) = DeeUtf8String_STR(arg); else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
     if (DeeUtf16String_Check(arg)) *DEE_VA_ARG(args,Dee_Utf16Char **) = DeeUtf16String_STR(arg); else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
     if (DeeUtf32String_Check(arg)) *DEE_VA_ARG(args,Dee_Utf32Char **) = DeeUtf32String_STR(arg); else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
     {
      DeeError_TypeError_UnexpectedType(arg,&DeeString_Type);
      return -1;
     }
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
    } break;
#define GOTO_BOOL_X(sizeof)  goto DEE_PP_CAT_2(b_,DEE_PP_MUL8(sizeof))
#define GOTO_INT_X(sizeof)   goto DEE_PP_CAT_2(o_i,DEE_PP_MUL8(sizeof))
#define GOTO_INT_UX(sizeof)  goto DEE_PP_CAT_2(o_ui,DEE_PP_MUL8(sizeof))
    case 'f': SIMPLE_CAST(float,DeeObject_Cast_F(float)) break;
    case 'D': SIMPLE_CAST(double,DeeObject_Cast_F(double)) break;
    case 'L': switch (*fmt++) {
     case 'D': SIMPLE_CAST(long double,DeeObject_Cast_F(ldouble)) break;
     default: DEE_TUPLE_INVALID_FORMAT();
    } break;
    case 'b': GOTO_BOOL_X(DEE_TYPES_SIZEOF_INT);
    case 'd': case 'i': GOTO_INT_X(DEE_TYPES_SIZEOF_INT);
    case 'u': case 'x': case 'X': GOTO_INT_UX(DEE_TYPES_SIZEOF_INT);
    case 'j': switch (*fmt++) {
     case 'b': GOTO_BOOL_X(DEE_TYPES_SIZEOF_INTMAX_T);
     case 'd': case 'i': GOTO_INT_X(DEE_TYPES_SIZEOF_INTMAX_T);
     case 'u': case 'x': case 'X': GOTO_INT_UX(DEE_TYPES_SIZEOF_INTMAX_T);
     default: DEE_TUPLE_INVALID_FORMAT();
    } break;
    case 'z': switch (*fmt++) {
     case 'b': GOTO_BOOL_X(DEE_TYPES_SIZEOF_SIZE_T);
     case 'd': case 'i': case 'u': case 'x': case 'X': GOTO_INT_UX(DEE_TYPES_SIZEOF_SIZE_T);
     default: DEE_TUPLE_INVALID_FORMAT();
    } break;
    case 'I': switch (*fmt++) {
     case 'b': GOTO_BOOL_X(DEE_TYPES_SIZEOF_SIZE_T);
     case 'd': case 'i': case 'u': case 'x': case 'X': GOTO_INT_X(DEE_TYPES_SIZEOF_SIZE_T);
     case '8': switch (*fmt++) {
      case 'b': b_8: BOOL_CAST(Dee_uint8_t) break;
      case 'd': case 'i': o_i8: SIMPLE_CAST(Dee_int8_t,DeeObject_Cast_F(Dee_int8_t)) break;
      case 'u': case 'x': case 'X': o_ui8: SIMPLE_CAST(Dee_uint8_t,DeeObject_Cast_F(Dee_uint8_t)) break;
      default: DEE_TUPLE_INVALID_FORMAT();
     } break;
     case '1': switch (*fmt++) {
      case '6': switch (*fmt++) {
       case 'b': b_16: BOOL_CAST(Dee_uint16_t) break;
       case 'd': case 'i': o_i16: SIMPLE_CAST(Dee_int16_t,DeeObject_Cast_F(Dee_int16_t)) break;
       case 'u': case 'x': case 'X': o_ui16: SIMPLE_CAST(Dee_uint16_t,DeeObject_Cast_F(Dee_uint16_t)) break;
       default: DEE_TUPLE_INVALID_FORMAT();
      } break;
      default: DEE_TUPLE_INVALID_FORMAT();
     } break;
     case '3': switch (*fmt++) {
      case '2': switch (*fmt++) {
       case 'b': b_32: BOOL_CAST(Dee_uint32_t) break;
       case 'd': case 'i': o_i32: SIMPLE_CAST(Dee_int32_t,DeeObject_Cast_F(Dee_int32_t)) break;
       case 'u': case 'x': case 'X': o_ui32: SIMPLE_CAST(Dee_uint32_t,DeeObject_Cast_F(Dee_uint32_t)) break;
       default: DEE_TUPLE_INVALID_FORMAT();
      } break;
      default: DEE_TUPLE_INVALID_FORMAT();
     } break;
     case '6': switch (*fmt++) {
      case '4': switch (*fmt++) {
       case 'b': b_64: BOOL_CAST(Dee_uint64_t) break;
       case 'd': case 'i': o_i64: SIMPLE_CAST(Dee_int64_t,DeeObject_Cast_F(Dee_int64_t)) break;
       case 'u': case 'x': case 'X': o_ui64: SIMPLE_CAST(Dee_uint64_t,DeeObject_Cast_F(Dee_uint64_t)) break;
       default: DEE_TUPLE_INVALID_FORMAT();
      } break;
      default: DEE_TUPLE_INVALID_FORMAT();
     } break;
     default: DEE_TUPLE_INVALID_FORMAT();
    } break;
#undef BOOL_CAST
#undef SIMPLE_CAST
    case 't': switch (*fmt++) {
     case 'b': GOTO_BOOL_X(DEE_TYPES_SIZEOF_SIZE_T);
     case 'd': case 'i': case 'u': case 'x': case 'X': GOTO_INT_X(DEE_TYPES_SIZEOF_SIZE_T);
     default: DEE_TUPLE_INVALID_FORMAT();
    } break;
    case 'l': switch (*fmt++) {
     case 's': {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
      Dee_WideChar *temp;
      GET_ARG();
      if (DeeObject_GetPointerEx(arg,DeeObject_TYPE(Dee_WideChar),(void **)&temp) != 0) return -1;
      *DEE_VA_ARG(args,Dee_WideChar **) = temp;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
      GET_ARG();
#if DEE_CONFIG_HAVE_ENCODING_WIDE
      if (DeeWideString_Check(arg)) *DEE_VA_ARG(args,Dee_WideChar **) = DeeWideString_STR(arg); else
      { DeeError_TypeError_UnexpectedType(arg,&DeeWideString_Type); return -1; }
#else /* DEE_CONFIG_HAVE_ENCODING_WIDE */
      { DeeError_TypeError_UnexpectedType(arg,&DeeString_Type); return -1; }
#endif /* !DEE_CONFIG_HAVE_ENCODING_WIDE */
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
     } break;
     case 'c':
      GET_ARG();
      if (!DeeString_Check(arg)||DeeString_SIZE(arg)!=1) goto ex_chr;
      *DEE_VA_ARG(args,Dee_WideChar *) = (Dee_WideChar)*DeeString_STR(arg);
      break;
     case 'b': GOTO_BOOL_X(DEE_TYPES_SIZEOF_LONG);
     case 'd': case 'i': GOTO_INT_X(DEE_TYPES_SIZEOF_LONG);
     case 'u': case 'x': case 'X': GOTO_INT_UX(DEE_TYPES_SIZEOF_LONG);
     case 'l': switch (*fmt++) {
      case 'd': case 'i': GOTO_INT_X(DEE_TYPES_SIZEOF_LLONG);
      case 'u': case 'b': case 'x': case 'X': GOTO_INT_UX(DEE_TYPES_SIZEOF_LLONG);
      default: DEE_TUPLE_INVALID_FORMAT();
     } break;
     default: DEE_TUPLE_INVALID_FORMAT();
    } break;
    case 'h': switch (*fmt++) {
     case 'b': GOTO_BOOL_X(DEE_TYPES_SIZEOF_SHORT);
     case 'd': case 'i': GOTO_INT_X(DEE_TYPES_SIZEOF_SHORT);
     case 'u': case 'x': case 'X': GOTO_INT_UX(DEE_TYPES_SIZEOF_SHORT);
     case 'h': switch (*fmt++) {
      case 'b': GOTO_BOOL_X(DEE_TYPES_SIZEOF_CHAR);
      case 'd': case 'i': GOTO_INT_X(DEE_TYPES_SIZEOF_CHAR);
      case 'u': case 'x': case 'X': GOTO_INT_UX(DEE_TYPES_SIZEOF_CHAR);
      default: DEE_TUPLE_INVALID_FORMAT();
     } break;
     default: DEE_TUPLE_INVALID_FORMAT();
    } break;
    case '.': switch (*fmt++) {
     case '*': switch (*fmt++) {
      case 'l': GET_ARG();
#ifdef DEE_DEBUG
       if (*fmt != 's') DEE_TUPLE_INVALID_FORMAT();
#endif
       ++fmt;
       if (DeeError_TypeError_CheckTypeExact(arg,&DeeWideString_Type) != 0) return -1;
       *DEE_VA_ARG(args,unsigned int *) = (unsigned int)DeeWideString_SIZE(arg);
       *DEE_VA_ARG(args,Dee_WideChar **) = DeeWideString_STR(arg);
       break;
      case 's': GET_ARG();
       if (DeeError_TypeError_CheckTypeExact(arg,&DeeString_Type) != 0) return -1;
       *DEE_VA_ARG(args,unsigned int *) = (unsigned int)DeeString_SIZE(arg);
       *DEE_VA_ARG(args,char **) = DeeString_STR(arg);
       break;
      default: DEE_TUPLE_INVALID_FORMAT();
     } break;
     default: DEE_TUPLE_INVALID_FORMAT();
    } break;
    case 's': {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
     char *temp;
     GET_ARG();
     if (DeeObject_GetPointerEx(arg,DeeObject_TYPE(char),(void **)&temp) != 0) return -1;
     *DEE_VA_ARG(args,char **) = temp;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
      GET_ARG();
      if (DeeString_Check(arg)) *DEE_VA_ARG(args,char **) = DeeString_STR(arg); else
      { DeeError_TypeError_UnexpectedType(arg,&DeeString_Type); return -1; }
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
    } break;
    case 'k': case 'K':
    case 'r': case 'R':
    case 'o': case 'O':
     GET_ARG();
     *DEE_VA_ARG(args,DeeObject **) = arg;
     break;
#if !DEE_CONFIG_TUPLE_PERCENT_OPTIONAL
   default: DEE_TUPLE_INVALID_FORMAT();
  } break;
#endif /* !DEE_CONFIG_TUPLE_PERCENT_OPTIONAL */
  case '\0':
  case ':':
   goto end;
  case '|': opt = 1; break;
  default: DEE_TUPLE_INVALID_FORMAT();
  // TODO: tuple recursion
  //  >> '%d(%d)%d' // 3-elemental tuple, where the 2nd item is another 1-elemental tuple
 }
end:
 if (argv == argv_end) // Make sure all arguments were consumed
  return 0;
inv_argc2: {
 Dee_size_t minargc,maxargc; char const *func_name;
 if (DeeTuple_UnpackInfo(fmt_start,&minargc,&maxargc,&func_name) != 0) return -1;
 if (!func_name) func_name = "<unnamed>";
 DeeError_TypeError_ArgCountExpectedNamedMinMax(
  func_name,minargc,maxargc,DeeTuple_SIZE(self));
 return -1;
}
inv_argc:
 if (opt) return 0;
 goto inv_argc2;
}
#endif

DEE_A_RET_EXCEPT(-1) int DeeTuple_Unpack(
 DEE_A_IN_OBJECT(DeeTupleObject) *self, DEE_A_IN_UNPACKTUPLEF char const *fmt, ...) {
 int result; va_list args;
 DEE_VA_START(args,fmt);
 result = _DeeTuple_VUnpack(self,fmt,args);
 DEE_VA_END(args);
 return result;
}

DEE_DECL_END
