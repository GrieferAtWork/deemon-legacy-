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
#include "string.c"
#include <deemon/__string_encoding.inl>
DEE_DECL_BEGIN
#define ENC Utf8
#endif

#ifndef ENC
#error "Must #define ENC before including"
#endif

#define ENC_FORMATTER           DEE_PP_CAT_3(_dee,ENC,formatter)
#define ENC_FORMATTER_GETVAL    DEE_PP_CAT_3(_dee,ENC,formatter_getval)
#define ENC_FORMATTER_GETONE    DEE_PP_CAT_3(_dee,ENC,formatter_getone)
#define ENC_FORMATTER_GETONE_EX DEE_PP_CAT_3(_dee,ENC,formatter_getone_ex)
#define ENC_STRINGWRITER DEE_PP_CAT_3(Dee,ENC,StringWriter)
#define ENC_STRINGOBJECT DEE_PP_CAT_3(Dee,ENC,StringObject)
#define ENC_CHAR         DEE_PP_CAT_3(Dee_,ENC,Char)
#if DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(Utf8)
#define ENC_CHAR_C DEE_CHAR8_C
#elif DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(Wide)
#define ENC_CHAR_C DEE_WIDECHAR_C
#elif DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(Utf16)
#define ENC_CHAR_C DEE_CHAR16_C
#elif DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(Utf32)
#define ENC_CHAR_C DEE_CHAR32_C
#else
#define ENC_CHAR_C (DEE_CHAR)
#endif

struct ENC_FORMATTER {
 ENC_STRINGWRITER        f_writer;
 ENC_STRINGOBJECT const *f_format;    /*< [1..1] Input format string. */
 ENC_CHAR const         *f_iter;      /*< [1..1] Current input format position. */
 ENC_CHAR const         *f_end;       /*< [1..1] Pointer to the first invalid character. */
 ENC_CHAR const         *f_flush;     /*< [1..1] Start of unflush input data. */
           DeeObject    *f_args;      /*< [1..1] Arguments object used as data input (usually a list or dict, but can be anything). */
 DEE_A_REF DeeObject    *f_args_iter; /*< [0..1] Iterator for traversing the arguments (NULL if not yet used). */
};

//////////////////////////////////////////////////////////////////////////
// Parse the input format after a '{' was encountered
// >> Returns the object 
#ifndef ENC_FORMATTER_MODE_DEF
#define ENC_FORMATTER_MODE_DEF  0 /*< Use the object as it is. */
#define ENC_FORMATTER_MODE_STR  1 /*< Use the object's '__str__' */
#define ENC_FORMATTER_MODE_REPR 2 /*< Use the object's '__repr__' */
#endif
static DEE_A_RET_EXCEPT_REF DeeObject *ENC_FORMATTER_GETONE(DEE_A_INOUT struct ENC_FORMATTER *self);
static DEE_A_RET_EXCEPT_REF DeeObject *ENC_FORMATTER_GETONE_EX(DEE_A_INOUT struct ENC_FORMATTER *self, DEE_A_OUT int *mode);
static DEE_A_RET_EXCEPT_REF DeeObject *ENC_FORMATTER_GETVAL(DEE_A_INOUT struct ENC_FORMATTER *self);

static DEE_A_RET_EXCEPT_REF DeeObject *ENC_FORMATTER_GETONE(DEE_A_INOUT struct ENC_FORMATTER *self) {
 DeeObject *result,*newresult; int error;
 ENC_CHAR ch = *self->f_iter;
 if (DEE_CH_IS_ALPHA(ch)) {
  // dict-style key lookup
  char *key; Dee_size_t key_len;
  ENC_CHAR const *key_begin = self->f_iter;
  do ch = *++self->f_iter; while (DEE_CH_IS_ALPHA(ch));
  key_len = (Dee_size_t)(self->f_iter-key_begin);
  while DEE_UNLIKELY((key = (char *)malloc_nz((key_len+1)*sizeof(char))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return NULL;
  }
  key[key_len] = ENC_CHAR_C('\0');
#if DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING)
  memcpy(key,key_begin,key_len*sizeof(char));
#else
  {char *dst = key;
   while (key_len--) *dst++ = (char)*key_begin++;
  }
#endif
  result = DeeObject_GetItemString(self->f_args,key);
  free_nn(key);
  if DEE_UNLIKELY(!result) return NULL;
 } else if (DEE_CH_IS_DIGIT(ch)) {
  // list-style index lookup
  Dee_size_t index;
  ENC_CHAR const *index_str_begin = self->f_iter;
  do ch = *++self->f_iter; while (DEE_CH_IS_ALNUM(ch));
  if DEE_UNLIKELY(DEE_PP_CAT_5(Dee,ENC,String_ToUInt,DEE_PP_MUL8(DEE_TYPES_SIZEOF_SIZE_T),WithLength)((
   Dee_size_t)(self->f_iter-index_str_begin),index_str_begin,&index) != 0) return NULL;
  result = DeeObject_GetIndex(self->f_args,index);
  if DEE_UNLIKELY(!result) return NULL;
 } else {
  // Fallback: Iterator element lookup
  if DEE_UNLIKELY(!self->f_args_iter && (self->f_args_iter = DeeObject_IterSelf(self->f_args)) == NULL) return NULL;
  if DEE_UNLIKELY((error = DeeObject_IterNextEx(self->f_args_iter,&result)) != 0) {
   if DEE_UNLIKELY(error < 0) return NULL;
   DEE_ASSERT(error > 0);
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Unexpected end of arguments iterator near %Iu",
                       (Dee_size_t)(self->f_iter-DEE_PP_CAT_3(Dee,ENC,String_STR)(self->f_format)));
   return NULL;
  }
 }
sfx_next:
 switch (ch) {

  case '.': { // Attribute lookup
   char *key; Dee_size_t key_len; ENC_CHAR const *key_begin;
   key_begin = ++self->f_iter;
   for (ch = *self->f_iter; DEE_CH_IS_ALNUM(ch); ch = *++self->f_iter);
   key_len = (Dee_size_t)(self->f_iter-key_begin);
   while DEE_UNLIKELY((key = (char *)malloc_nz((key_len+1)*sizeof(char))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
    return NULL;
   }
   key[key_len] = ENC_CHAR_C('\0');
#if DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING)
   memcpy(key,key_begin,key_len*sizeof(char));
#else
   {char *dst = key;
    while (key_len--) *dst++ = (char)*key_begin++;
   }
#endif
   newresult = DeeObject_GetAttrString(result,key);
   free_nn(key);
   Dee_DECREF(result);
   if DEE_UNLIKELY(!newresult) return NULL;
   result = newresult;
   goto sfx_next;
  } break;

  case '(': {
   DeeObject *args_tuple,*arg;
   Dee_INCREF(args_tuple = Dee_EmptyTuple);
   if (*++self->f_iter != ')') {
    while (1) {
     if DEE_UNLIKELY((arg = ENC_FORMATTER_GETVAL(self)) == NULL) {
err_call_args_r:
      Dee_DECREF(args_tuple);
      Dee_DECREF(result);
      return NULL;
     }
     error = DeeTuple_Append(&args_tuple,arg);
     Dee_DECREF(arg);
     if DEE_UNLIKELY(error != 0) goto err_call_args_r;
     if (*self->f_iter != ',') break;
     ++self->f_iter;
    }
    if DEE_UNLIKELY(*self->f_iter != ')') {
     DeeError_SetStringf(&DeeErrorType_ValueError,
                         "Expected ')' to end function call in string format near %Iu, but got %.1q",
                         (Dee_size_t)(self->f_iter-DEE_PP_CAT_3(Dee,ENC,String_STR)(self->f_format)),
                         self->f_iter);
     goto err_call_args_r;
    }
   }
   ch = *++self->f_iter;
   newresult = DeeObject_Call(result,args_tuple);
   Dee_DECREF(args_tuple);
   Dee_DECREF(result);
   if DEE_UNLIKELY(!newresult) return NULL;
   result = newresult;
   goto sfx_next;
  } break;
  case '[': {
   DeeObject *rbegin,*rend; // index lookup on 'result'
   if (*++self->f_iter == ':') {
    // Range w/o begin
    if (*++self->f_iter == ']') rend = DeeNone_New();
    else if DEE_UNLIKELY((rend = ENC_FORMATTER_GETVAL(self)) == NULL) {
err_r: Dee_DECREF(result); return NULL;
    }
    rbegin = DeeNone_New();
   } else {
    if DEE_UNLIKELY((rbegin = ENC_FORMATTER_GETVAL(self)) == NULL) goto err_r;
    if (*self->f_iter == ':') {
     if (*++self->f_iter == ']') rend = DeeNone_New();
     else if DEE_UNLIKELY((rend = ENC_FORMATTER_GETVAL(self)) == NULL) {
      Dee_DECREF(rbegin);
      goto err_r;
     }
    } else rend = NULL;
   }
   if (*self->f_iter != ']') {
    Dee_XDECREF(rend);
    Dee_DECREF(rbegin);
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        "Expected ']' to end attribute lookup in string format near %Iu",
                        (Dee_size_t)(self->f_iter-DEE_PP_CAT_3(Dee,ENC,String_STR)(self->f_format)));
    goto err_r;
   }
   ch = *++self->f_iter;
   DEE_ASSERT(DeeObject_Check(rbegin));
   DEE_ASSERT(!rend || DeeObject_Check(rend));
   if (rend) {
    newresult = DeeObject_GetRangeItem(result,rbegin,rend);
    Dee_DECREF(rend);
   } else {
    newresult = DeeObject_GetItem(result,rbegin);
   }
   Dee_DECREF(rbegin);
   Dee_DECREF(result);
   if DEE_UNLIKELY(!newresult) return NULL;
   result = newresult;
   goto sfx_next;
  } break;

  default: break;
 }
 return result;
}

static DEE_A_RET_EXCEPT_REF DeeObject *ENC_FORMATTER_GETONE_EX(
 DEE_A_INOUT struct ENC_FORMATTER *self, DEE_A_OUT int *mode) {
 ENC_CHAR ch; DeeObject *result,*newresult;
 if DEE_UNLIKELY((result = ENC_FORMATTER_GETONE(self)) == NULL) return NULL;
 if (*self->f_iter == '!') {
  // Explicit cast operator
  ch = *++self->f_iter;
  switch (ch) {
   {
    if (0) { case 'a': case 's': *mode = ENC_FORMATTER_MODE_STR; }
    if (0) { case 'r': *mode = ENC_FORMATTER_MODE_REPR; }
    *++self->f_iter;
   } break;
   default: {
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        "Invalid character %.1q following '!' in string format",
                        &ch);
    Dee_DECREF(result);
    return NULL;
   } break;
  }
  if (*self->f_iter == ':') {
   switch (*mode) {
    if (0) { case ENC_FORMATTER_MODE_STR: newresult = DeeObject_Str(result); }
    if (0) { case ENC_FORMATTER_MODE_REPR: newresult = DeeObject_Repr(result); }
     Dee_DECREF(result);
     if DEE_UNLIKELY(!newresult) return NULL;
     result = newresult;
     *mode = ENC_FORMATTER_MODE_DEF;
     break;
    default: break;
   }
   goto custom_formatter;
  }
 } else {
  *mode = ENC_FORMATTER_MODE_DEF;
  if (*self->f_iter == ':')
custom_formatter: {
   ENC_STRINGWRITER custom_format_writer = DEE_PP_CAT_3(Dee,ENC,StringWriter_INIT)();
   DeeObject *custom_format,*fmt_elem,*fmt_args; int mode2,error;
   ENC_CHAR const *flush_start;
   DeeString_NEW_STATIC(_format_member,"__format__");
   // Custom format string
   flush_start = self->f_iter+1;
   while DEE_LIKELY(++self->f_iter != self->f_end) {
    if DEE_UNLIKELY(*self->f_iter == '{') {
     if DEE_UNLIKELY(DEE_PP_CAT_3(Dee,ENC,StringWriter_WriteMyStringWithLength)(
      &custom_format_writer,(Dee_size_t)(self->f_iter-flush_start),flush_start) != 0) goto err_custom_format;
     ++self->f_iter;
     if DEE_UNLIKELY((fmt_elem = ENC_FORMATTER_GETONE_EX(self,&mode2)) == NULL) {
err_custom_format:
      DEE_PP_CAT_3(Dee,ENC,StringWriter_Quit)(&custom_format_writer);
err_r:
      Dee_DECREF(result);
      return NULL;
     }
     error = mode2 == ENC_FORMATTER_MODE_REPR
      ? DEE_PP_CAT_3(Dee,ENC,StringWriter_WriteObjectRepr)(&custom_format_writer,fmt_elem)
      : DEE_PP_CAT_3(Dee,ENC,StringWriter_WriteObjectStr)(&custom_format_writer,fmt_elem);
     Dee_DECREF(fmt_elem);
     if DEE_UNLIKELY(error != 0) goto err_custom_format;
     if DEE_UNLIKELY(*self->f_iter != '}') {
      DeeError_SetStringf(&DeeErrorType_ValueError,
                          "Expected end of format near %Iu, but got %.1q",
                          (Dee_size_t)(self->f_iter-DEE_PP_CAT_3(Dee,ENC,String_STR)(self->f_format)),
                          self->f_iter);
      goto err_custom_format;
     }
     flush_start = self->f_iter+1;
    } else if DEE_UNLIKELY(*self->f_iter == '}') break;
   }
   if DEE_UNLIKELY(DEE_PP_CAT_3(Dee,ENC,StringWriter_WriteMyStringWithLength)(
    &custom_format_writer,(Dee_size_t)(self->f_iter-flush_start),
    flush_start) != 0) goto err_custom_format;
   custom_format = DEE_PP_CAT_3(Dee,ENC,StringWriter_Pack)(&custom_format_writer);
   DEE_PP_CAT_3(Dee,ENC,StringWriter_Quit)(&custom_format_writer);
   if DEE_UNLIKELY(!custom_format) goto err_r;
   fmt_args = DeeTuple_Pack(1,custom_format);
   Dee_DECREF(custom_format);
   if DEE_UNLIKELY(!fmt_args) goto err_r;
   newresult = DeeObject_CallAttr(result,(DeeObject *)&_format_member,fmt_args);
   Dee_DECREF(fmt_args);
   Dee_DECREF(result);
   if DEE_UNLIKELY(!newresult) return NULL;
   result = newresult;
  }
 }
 return result;
}

static DEE_A_RET_EXCEPT_REF DeeObject *
ENC_FORMATTER_GETVAL(DEE_A_INOUT struct ENC_FORMATTER *self) {
 ENC_CHAR ch = *self->f_iter;
 if (ch == '{') {
  DeeObject *result,*newresult; int mode;
  ++self->f_iter;
  if DEE_UNLIKELY((result = ENC_FORMATTER_GETONE_EX(self,&mode)) == NULL) return NULL;
  if DEE_UNLIKELY(*self->f_iter != '}') {
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Expected end of format near %Iu, but got %.1q",
                       (Dee_size_t)(self->f_iter-DEE_PP_CAT_3(Dee,ENC,String_STR)(self->f_format)),
                       self->f_iter);
   Dee_DECREF(result);
   return NULL;
  }
  ++self->f_iter;
  switch (mode) {
   {
    if (0) { case ENC_FORMATTER_MODE_STR: newresult = DeeObject_Str(result); }
    if (0) { case ENC_FORMATTER_MODE_REPR: newresult = DeeObject_Repr(result); }
    Dee_DECREF(result);
    if (!newresult) return NULL;
    result = newresult;
    break;
   }
   default: break;
  }
  return result;
 } else if DEE_LIKELY(DEE_CH_IS_DIGIT(ch)) {
  Dee_rt_int int_val;
  ENC_CHAR const *int_begin; // Constant integral
  int_begin = self->f_iter;
  do ch = *++self->f_iter; while (DEE_CH_IS_ALNUM(ch));
  if DEE_UNLIKELY(DEE_PP_CAT_5(Dee,ENC,String_ToInt,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_INT),
   WithLength)((Dee_size_t)(self->f_iter-int_begin),int_begin,&int_val) != 0) return NULL;
  return DeeObject_New(Dee_rt_int,int_val);
 } else if DEE_LIKELY(DEE_CH_IS_ALPHA(ch)) {
  ENC_CHAR const *str_begin; // Constant string (current encoding)
  str_begin = self->f_iter;
  do ch = *++self->f_iter; while (DEE_CH_IS_ALNUM(ch));
  return DEE_PP_CAT_3(Dee,ENC,String_NewWithLength)(
   (Dee_size_t)(self->f_iter-str_begin),str_begin);
 }
 DeeError_SetStringf(&DeeErrorType_ValueError,
                     "Expected alnum or '{' near %Iu, but got %.1q",
                     (Dee_size_t)(self->f_iter-DEE_PP_CAT_3(Dee,ENC,String_STR)(self->f_format)),
                     &ch);
 return NULL;
}

DEE_A_RET_OBJECT_EXCEPT_REF(ENC_STRINGOBJECT) *DEE_PP_CAT_3(Dee,ENC,String_Format)(
 DEE_A_IN_OBJECT(ENC_STRINGOBJECT) const *self, DEE_A_INOUT DeeObject *args) {
 DeeObject *result,*fmt_elem; int error,mode; ENC_CHAR ch;
 struct ENC_FORMATTER formatter = {DEE_PP_CAT_3(Dee,ENC,StringWriter_INIT)()};
 DEE_ASSERT(DeeObject_Check(self) && DEE_PP_CAT_3(Dee,ENC,String_Check)(self));
 DEE_ASSERT(DeeObject_Check(args));
 formatter.f_iter = DEE_PP_CAT_3(Dee,ENC,String_STR)(self);
 formatter.f_end = formatter.f_iter+DEE_PP_CAT_3(Dee,ENC,String_SIZE)(self);
 formatter.f_flush = formatter.f_iter;
 formatter.f_args = args;
 formatter.f_format = (ENC_STRINGOBJECT const *)self;
 formatter.f_args_iter = NULL;
 if DEE_LIKELY(formatter.f_iter != formatter.f_end) while (1) {
  ch = *formatter.f_iter++;
  switch (ch) {
   case 0: if DEE_LIKELY(formatter.f_iter > formatter.f_end) goto done; break;
   case ENC_CHAR_C('}'):
    if (*formatter.f_iter == ENC_CHAR_C('}')) goto escape_one;
    break;
   case ENC_CHAR_C('{'): {
    if (*formatter.f_iter == ENC_CHAR_C('{')) {
escape_one:
     if (DEE_PP_CAT_3(Dee,ENC,StringWriter_WriteMyStringWithLength)(&formatter.f_writer,(
      Dee_size_t)((formatter.f_iter-formatter.f_flush)-1),formatter.f_flush) != 0) goto err;
     formatter.f_flush = formatter.f_iter++;
     break;
    }
    if (DEE_PP_CAT_3(Dee,ENC,StringWriter_WriteMyStringWithLength)(&formatter.f_writer,(
     Dee_size_t)((formatter.f_iter-formatter.f_flush)-1),formatter.f_flush) != 0) goto err;
    if DEE_UNLIKELY((fmt_elem = ENC_FORMATTER_GETONE_EX(&formatter,&mode)) == NULL) goto err;
    error = mode == ENC_FORMATTER_MODE_REPR
     ? DEE_PP_CAT_3(Dee,ENC,StringWriter_WriteObjectRepr)(&formatter.f_writer,fmt_elem)
     : DEE_PP_CAT_3(Dee,ENC,StringWriter_WriteObjectStr)(&formatter.f_writer,fmt_elem);
    Dee_DECREF(fmt_elem);
    if DEE_UNLIKELY(error != 0) goto err;
    ch = *formatter.f_iter++;
    if DEE_UNLIKELY(ch != ENC_CHAR_C('}')) {
     DeeError_SetStringf(&DeeErrorType_ValueError,
                         "Expected end of format near %Iu, but got %.1q",
                         (Dee_size_t)(formatter.f_iter-DEE_PP_CAT_3(Dee,ENC,String_STR)(self)),
                         &ch);
     goto err;
    }
    formatter.f_flush = formatter.f_iter;
   } break;
   default: break;
  }
 }
done:
 if DEE_UNLIKELY(DEE_PP_CAT_3(Dee,ENC,StringWriter_WriteMyStringWithLength)(
  &formatter.f_writer,(Dee_size_t)((formatter.f_iter-formatter.f_flush)-1),formatter.f_flush) != 0) goto err;
 result = DEE_PP_CAT_3(Dee,ENC,StringWriter_Pack)(&formatter.f_writer);
end:
 Dee_XDECREF(formatter.f_args_iter);
 DEE_PP_CAT_3(Dee,ENC,StringWriter_Quit)(&formatter.f_writer);
 return result;
err: result = NULL; goto end;
#undef FLUSH
}


#undef ENC_FORMATTER
#undef ENC_STRINGWRITER
#undef ENC_STRINGOBJECT
#undef ENC_CHAR
#undef ENC_CHAR_C
#undef ENC_CHAR_C

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif

