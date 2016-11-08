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
#ifndef GUARD_DEEMON_STRUCTURED_TYPEF_INL
#define GUARD_DEEMON_STRUCTURED_TYPEF_INL 1

#ifdef __INTELLISENSE__
#include "structured.c"
#endif

#include <deemon/integer.h>
#include <deemon/float.h>
#include <deemon/__min_int_t.inl>

DEE_DECL_BEGIN


#ifdef DEE_DEBUG
#define DEE_TYPEF_INVALID_FORMAT() \
 do{DEE_LDEBUG("%s(%u) : Invalid typef format : %s (near %.5q)\n",__FILE__,__LINE__,*fmt_begin,fmt-2);DEE_ABNORMAL_TERMINATION();}while(0)
#define DEE_TYPEF_ASSERT(x) do{if(!(x))DEE_TYPEF_INVALID_FORMAT();}while(0)
#else /* DEE_DEBUG */
#define DEE_TYPEF_INVALID_FORMAT   DEE_BUILTIN_UNREACHABLE
#define DEE_TYPEF_ASSERT(x)        do{}while(0)
#endif /* !DEE_DEBUG */


DEE_COMPILER_MSVC_WARNING_PUSH(4061)
DEE_A_RET_EXCEPT(NULL) DeeTypeObject *_DeeType_VStructf_Tyname(
 DEE_A_IN_Z char const **fmt_begin, DEE_A_IN va_list *pargs) {
 DeeTypeObject *result;
 char const *fmt = *fmt_begin;
 int do_atomic = 0;
 enum {
  len_none,len_l,len_ll,len_h,len_hh,
  len_I,len_I8,len_I16,len_I32,len_I64,len_L
 } len = len_none;
len_again:
 switch (*fmt) {
  case '%': DEE_TYPEF_ASSERT(fmt[1] == 'o'); result = DEE_VA_ARG(*pargs,DeeTypeObject *); break;
  case 'a': DEE_TYPEF_ASSERT(!do_atomic); do_atomic = 1,++fmt; goto len_again;
  case 'L': ++fmt; len = len_L; break;
  case 'l': if (*++fmt == 'l') len = len_ll,++fmt; else len = len_l; break;
  case 'h': if (*++fmt == 'h') len = len_hh,++fmt; else len = len_h; break;
  case 'I': switch (*++fmt) {
   case '8': len = len_I8; ++fmt; break;
   case '1': DEE_TYPEF_ASSERT(fmt[1] == '6'); fmt += 2; len = len_I16; break;
   case '3': DEE_TYPEF_ASSERT(fmt[1] == '2'); fmt += 2; len = len_I32; break;
   case '6': DEE_TYPEF_ASSERT(fmt[1] == '4'); fmt += 2; len = len_I64; break;
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
   case len_I8:  result = DeeObject_TYPE(int8); break;
   case len_I16: result = DeeObject_TYPE(int16); break;
   case len_I32: result = DeeObject_TYPE(int32); break;
   case len_I64: result = DeeObject_TYPE(int64); break;
   default:      result = DeeObject_TYPE(int); break;
  } break;
  case 'u': case 'x': case 'X': switch (len) {
   case len_l:   result = DeeObject_TYPE(ulong); break;
   case len_ll:  result = DeeObject_TYPE(ullong); break;
   case len_h:   result = DeeObject_TYPE(ushort); break;
   case len_hh:  result = DeeObject_TYPE(uchar); break;
   case len_I:   result = DeeObject_TYPE(size_t); break;
   case len_I8:  result = DeeObject_TYPE(uint8); break;
   case len_I16: result = DeeObject_TYPE(uint16); break;
   case len_I32: result = DeeObject_TYPE(uint32); break;
   case len_I64: result = DeeObject_TYPE(uint64); break;
   default:      result = DeeObject_TYPE(uint); break;
  } break;
  case 'f': result = DeeObject_TYPE(float); break;
  case 'D': result = len == len_L ? DeeObject_TYPE(ldouble) : DeeObject_TYPE(double); break;
  case 'o': result = &DeeObject_Type; break;
  case 'n': case 'v': result = (DeeTypeObject *)&DeeNone_Type; break;
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case 'p': result = DeeType_Pointer((DeeTypeObject *)&DeeNone_Type); break;
#else /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
  case 'p': case 's': DeeError_NotImplemented_str("DEE_CONFIG_LANGUAGE_HAVE_POINTERS"); result = NULL; break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
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
  default: DEE_TYPEF_INVALID_FORMAT();
 }
 *fmt_begin = fmt;
 return result;
}

Dee_size_t _DeeType_VStructf_Intname(
 DEE_A_IN_Z char const **fmt_begin, DEE_A_IN va_list *pargs) {
 Dee_size_t result;
 char const *fmt = *fmt_begin;
 if (*fmt == '%') {
  enum {
   len_none,len_l,len_ll,len_h,len_hh,
   len_I,len_I8,len_I16,len_I32,len_I64,len_L
  } len = len_none;
  switch (*fmt) {
   case 'L': ++fmt; len = len_L; break;
   case 'l': if (*++fmt == 'l') len = len_ll,++fmt; else len = len_l; break;
   case 'h': if (*++fmt == 'h') len = len_hh,++fmt; else len = len_h; break;
   case 'I': switch (*++fmt) {
    case '8': len = len_I8; ++fmt; break;
    case '1': DEE_TYPEF_ASSERT(fmt[1] == '6'); fmt += 2; len = len_I16; break;
    case '3': DEE_TYPEF_ASSERT(fmt[1] == '2'); fmt += 2; len = len_I32; break;
    case '6': DEE_TYPEF_ASSERT(fmt[1] == '4'); fmt += 2; len = len_I64; break;
    default: len = len_I; break;
   } break;
   default: break;
  }
  switch (*fmt++) {
   case 'd': case 'i': switch (len) {
    case len_l:   goto DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_TYPES_SIZEOF_LONG));
    case len_ll:  goto DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_TYPES_SIZEOF_LLONG));
    case len_h:   goto DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_TYPES_SIZEOF_SHORT));
    case len_hh:  goto DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_TYPES_SIZEOF_CHAR));
    case len_I:   goto DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_TYPES_SIZEOF_SIZE_T));
    case len_I8:  i8:result = (Dee_size_t)DEE_VA_ARG(*pargs,DEE_MININT_T(1)); break;
    case len_I16: i16:result = (Dee_size_t)DEE_VA_ARG(*pargs,DEE_MININT_T(2)); break;
    case len_I32: i32:result = (Dee_size_t)DEE_VA_ARG(*pargs,DEE_MININT_T(4)); break;
    case len_I64: i64:result = (Dee_size_t)DEE_VA_ARG(*pargs,DEE_MININT_T(8)); break;
    default:      goto DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_TYPES_SIZEOF_INT));
   } break;
   case 'u': case 'x': case 'X': switch (len) {
    case len_l:   goto DEE_PP_CAT_2(ui,DEE_PP_MUL8(DEE_TYPES_SIZEOF_LONG));
    case len_ll:  goto DEE_PP_CAT_2(ui,DEE_PP_MUL8(DEE_TYPES_SIZEOF_LLONG));
    case len_h:   goto DEE_PP_CAT_2(ui,DEE_PP_MUL8(DEE_TYPES_SIZEOF_SHORT));
    case len_hh:  goto DEE_PP_CAT_2(ui,DEE_PP_MUL8(DEE_TYPES_SIZEOF_CHAR));
    case len_I:   goto DEE_PP_CAT_2(ui,DEE_PP_MUL8(DEE_TYPES_SIZEOF_SIZE_T));
    case len_I8:  ui8:result = (Dee_size_t)DEE_VA_ARG(*pargs,DEE_MINUINT_T(1)); break;
    case len_I16: ui16:result = (Dee_size_t)DEE_VA_ARG(*pargs,DEE_MINUINT_T(2)); break;
    case len_I32: ui32:result = (Dee_size_t)DEE_VA_ARG(*pargs,DEE_MINUINT_T(4)); break;
    case len_I64: ui64:result = (Dee_size_t)DEE_VA_ARG(*pargs,DEE_MINUINT_T(8)); break;
    default:      goto DEE_PP_CAT_2(ui,DEE_PP_MUL8(DEE_TYPES_SIZEOF_INT));
   } break;
   default: DEE_TYPEF_INVALID_FORMAT();
  }
 } else {
  result = 0;
  DEE_TYPEF_ASSERT(*fmt >= '0' && *fmt <= '9');
  do {
   result = (Dee_size_t)(result*10+(Dee_size_t)(*fmt-'0'));
   ++fmt;
  } while (*fmt >= '0' && *fmt <= '9');
 }
 *fmt_begin = fmt;
 return result;
}

extern DEE_A_RET_EXCEPT(NULL) DeeTypeObject *_DeeType_VStructOnef(
 DEE_A_IN_Z char const **fmt_begin, DEE_A_IN va_list *pargs);
DEE_A_RET_EXCEPT(NULL) DeeTypeObject *_DeeType_VStructOnef(
 DEE_A_IN_Z char const **fmt_begin, DEE_A_IN va_list *pargs) {
 DeeTypeObject *result;
 char const *fmt;
 if ((result = _DeeType_VStructf_Tyname(fmt_begin,pargs)) == NULL) return NULL;
 fmt = *fmt_begin;
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS || \
    DEE_CONFIG_LANGUAGE_HAVE_ARRAYS || \
    DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
again:
 switch (*fmt) {
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
  case '*': ++fmt; if ((result = DeeType_Pointer(result)) == NULL) return NULL; goto again;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
#if DEE_CONFIG_LANGUAGE_HAVE_ARRAYS
  case '[': {
   Dee_size_t array_size; ++fmt;
   array_size = _DeeType_VStructf_Intname(&fmt,pargs);
   if ((result = DeeType_Array(result,array_size)) == NULL) return NULL;
   DEE_TYPEF_ASSERT(*fmt == ']'); ++fmt;
   goto again;
  }
#endif /* DEE_CONFIG_LANGUAGE_HAVE_ARRAYS */
#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
  case '(': {
   DeeFunctionFlags flags = DeeFunctionFlags_NONE;
   DeeTypeObject **argty_v,**new_argty_v; Dee_size_t argty_c,argty_a;
   ++fmt;
   if (*fmt == '!') { ++fmt; } // TODO: Parse function flags
   if (*fmt == ')') {
    result = DeeType_ForeignFunction(result,flags,0,NULL);
   } else {
    DeeTypeObject *argty;
    argty_v = NULL; argty_c = 0; argty_a = 0;
    do {
     if ((argty = _DeeType_VStructOnef(&fmt,pargs)) == NULL) { free(argty_v); return NULL; }
     if (argty_a == argty_c) {
      if (!argty_a) argty_a = 2; else argty_a *= 2;
      while ((new_argty_v = (DeeTypeObject **)realloc_nz(
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
   DEE_TYPEF_ASSERT(*fmt == ')'); ++fmt;
  } goto again;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */
  default: break;
 }
#endif /* ... */
 *fmt_begin = fmt;
 return result;
}
#undef DEE_TYPEF_ASSERT
#undef DEE_TYPEF_INVALID_FORMAT
DEE_COMPILER_MSVC_WARNING_POP

#ifdef DEE_DEBUG
#define _DeeType_VStructf(fmt,args) DeeType_VStructf(fmt,args)
#else
#define _DeeType_VStructf(fmt,args) _DeeType_VStructOnef(&(fmt),DEE_VA_ARGPTR(args))
#endif
DEE_A_RET_EXCEPT(NULL) DeeTypeObject *DeeType_VStructf(
 DEE_A_IN_Z char const *fmt, DEE_A_IN va_list args) {
#ifdef DEE_DEBUG
 DeeTypeObject *result;
 result = _DeeType_VStructOnef(&fmt,DEE_VA_ARGPTR(args));
 DEE_ASSERTF(!*fmt,"Expected end of typename format at %q",fmt);
 return result;
#else
 return _DeeType_VStructOnef(&fmt,DEE_VA_ARGPTR(args));
#endif
}


DEE_A_RET_EXCEPT(NULL) DeeTypeObject *DeeType_Structf(
 DEE_A_IN_Z char const *fmt, ...) {
 va_list args; DeeTypeObject *result;
 DEE_VA_START(args,fmt);
 result = _DeeType_VStructf(fmt,args);
 DEE_VA_END(args);
 return result;
}



DEE_A_RET_OBJECT_EXCEPT_REF(DeeStructuredObject) *DeeStructured_VNewFromDataf(
 DEE_A_IN void const *self, DEE_A_IN_Z char const *ty_fmt, DEE_A_IN va_list args) {
 DeeTypeObject *struct_ty;
 if ((struct_ty = _DeeType_VStructf(ty_fmt,args)) == NULL) return NULL;
 DEE_ASSERT(DeeObject_Check(struct_ty) && DeeStructuredType_Check(struct_ty));
 return DeeStructured_NewFromData(struct_ty,self);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStructuredObject) *DeeStructured_NewFromDataf(
 DEE_A_IN void const *self, DEE_A_IN_Z char const *ty_fmt, ...) {
 va_list args; DeeObject *result;
 DEE_VA_START(args,ty_fmt);
 result = DeeStructured_VNewFromDataf(self,ty_fmt,args);
 DEE_VA_END(args);
 return result;
}

#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
DEE_A_RET_OBJECT_EXCEPT_REF(DeePointerObject) *DeePointer_VNewf(
 void *p, DEE_A_IN_Z char const *ty_fmt, DEE_A_IN va_list args) {
 DeeTypeObject *struct_ty;
 if ((struct_ty = _DeeType_VStructf(ty_fmt,args)) == NULL) return NULL;
 if ((struct_ty = DeeType_Pointer(struct_ty)) == NULL) return NULL;
 DEE_ASSERT(DeeObject_Check(struct_ty) && DeePointerType_Check(struct_ty));
 return DeePointer_New(struct_ty,p);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeePointerObject) *DeePointer_Newf(
 void *p, DEE_A_IN_Z char const *ty_fmt, ...) {
 va_list args; DeeObject *result;
 DEE_VA_START(args,ty_fmt);
 result = DeePointer_VNewf(p,ty_fmt,args);
 DEE_VA_END(args);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeLValueObject) *DeeLValue_VNewf(
 DEE_A_IN void *p, DEE_A_IN_Z char const *ty_fmt, DEE_A_IN va_list args) {
 DeeTypeObject *struct_ty;
 if ((struct_ty = _DeeType_VStructf(ty_fmt,args)) == NULL) return NULL;
 if ((struct_ty = DeeType_LValue(struct_ty)) == NULL) return NULL;
 DEE_ASSERT(DeeObject_Check(struct_ty) && DeeLValueType_Check(struct_ty));
 return DeeLValue_New(struct_ty,p);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeLValueObject) *DeeLValue_Newf(
 DEE_A_IN void *p, DEE_A_IN_Z char const *ty_fmt, ...) {
 va_list args; DeeObject *result;
 DEE_VA_START(args,ty_fmt);
 result = DeeLValue_VNewf(p,ty_fmt,args);
 DEE_VA_END(args);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeObject_VTGetPointerExf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_OUT void **result, DEE_A_IN_Z char const *ty_fmt, DEE_A_IN va_list args) {
 DeeTypeObject *ptr_base;
 if ((ptr_base = _DeeType_VStructf(ty_fmt,args)) == NULL) return -1;
 return DeeObject_TGetPointerEx(tp_self,self,ptr_base,result);
}

DEE_A_RET_EXCEPT(-1) int DeeObject_TGetPointerExf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_OUT void **result, DEE_A_IN_Z char const *ty_fmt, ...) {
 va_list args; int retval;
 DEE_VA_START(args,ty_fmt);
 retval = DeeObject_VTGetPointerExf(tp_self,self,result,ty_fmt,args);
 DEE_VA_END(args);
 return retval;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
#undef _DeeType_VStructf




DEE_DECL_END

#endif /* !GUARD_DEEMON_STRUCTURED_TYPEF_INL */
