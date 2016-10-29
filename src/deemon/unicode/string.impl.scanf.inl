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
#include <deemon/__conf.inl>
#include <deemon/__string_encoding.inl>
#include <deemon/unicode/string.cast.escapecode.previewsize.inl>
#include <deemon/unicode/string.c>
#include "char_traits.inl"
DEE_DECL_BEGIN
#define ENC Utf8
#endif

#ifndef ENC
#error "Must #define ENC before including"
#endif

#define ENC_CHAR            DEE_PP_CAT_3(Dee_,ENC,Char)


DEE_COMPILER_MSVC_WARNING_PUSH(4061)
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *DEE_PP_CAT_3(Dee,ENC,String_Scanf)(
 DEE_A_IN_OBJECT(DEE_PP_CAT_3(Dee,ENC,StringObject)) const *data,
 DEE_A_IN_OBJECT(DEE_PP_CAT_3(Dee,ENC,StringObject)) const *format) {
 DeeObject *result;
 ENC_CHAR const *fmt_iter,*fmt_end,*dat_iter,*dat_end; ENC_CHAR ch;
 DEE_ASSERT(DeeObject_Check(data) && DEE_PP_CAT_3(Dee,ENC,String_Check)(data));
 DEE_ASSERT(DeeObject_Check(format) && DEE_PP_CAT_3(Dee,ENC,String_Check)(format));
 fmt_end = (fmt_iter = DEE_PP_CAT_3(Dee,ENC,String_STR)(
  format))+DEE_PP_CAT_3(Dee,ENC,String_SIZE)(format);
 dat_end = (dat_iter = DEE_PP_CAT_3(Dee,ENC,String_STR)(
  data))+DEE_PP_CAT_3(Dee,ENC,String_SIZE)(data);
 Dee_INCREF(result = Dee_EmptyTuple);
 while (1) {
  if ((ch = *fmt_iter++) == '\0' && fmt_iter >= fmt_end) break;
  switch (ch) {
   case ' ': case '\t': case '\n':
    while (dat_iter != dat_end && DEE_CH_IS_SPACE(*dat_iter)) ++dat_iter;
    break;
   case '%': {
    DeeObject *append_object;
    int ignore_data;
    Dee_size_t width;
    ENC_CHAR const *dat_begin;
    enum{
     len_none,len_hh,len_h,len_l,len_ll,len_j,len_z,len_t,len_L,
     len_I,len_I8,len_I16,len_I32,len_I64
    }length;
    union {
     Dee_uint8_t u8; Dee_uint16_t u16; Dee_uint32_t u32; Dee_uint64_t u64;
      Dee_int8_t i8;  Dee_int16_t i16;  Dee_int32_t i32;  Dee_int64_t i64;
     Dee_rt_float flt; Dee_rt_double dbl; Dee_rt_ldouble ldbl;
    } out_data;
    ch = *fmt_iter++;
    if (ch != '*') ignore_data = 0;
    else ignore_data = 1,ch = *fmt_iter++;
    if (DEE_CH_IS_DIGIT(ch)) {
     width = (Dee_size_t)(ch-'0');
     while (1) {
      ch = *fmt_iter++;
      if (!DEE_CH_IS_DIGIT(ch)) break;
      width = width*10+(Dee_size_t)(ch-'0');
     }
    } else width = (Dee_size_t)-1;
    // length
    switch (ch) {
     case 'h':
      ch = *fmt_iter++;
      if (ch != 'h') length = len_h;
      else ch = *fmt_iter++,length = len_hh;
      break;
     case 'l':
      ch = *fmt_iter++;
      if (ch != 'l') length = len_l;
      else ch = *fmt_iter++,length = len_ll;
      break;
     case 'j': ch = *fmt_iter++,length = len_j; break;
     case 'z': ch = *fmt_iter++,length = len_z; break;
     case 't': ch = *fmt_iter++,length = len_t; break;
     case 'L': ch = *fmt_iter++,length = len_L; break;
     case 'I':
      ch = *fmt_iter++;
      if (ch == '8') length = len_I8,ch = *fmt_iter++;
      else if (ch == '1' && *fmt_iter == '6') length = len_I16,ch = (fmt_iter += 2)[-1];
      else if (ch == '3' && *fmt_iter == '2') length = len_I32,ch = (fmt_iter += 2)[-1];
      else if (ch == '6' && *fmt_iter == '4') length = len_I64,ch = (fmt_iter += 2)[-1];
      else length = len_I;
      break;
     default: length = len_none; break;
    }
    dat_begin = dat_iter;
    // TODO: skip creation of 'append_object' if 'ignore_data' is true
    switch (ch) {
     case 'i':
      if ((*dat_iter == '+' || *dat_iter == '-') && width) ++dat_iter,--width;
      while (DEE_CH_IS_ALNUM(*dat_iter) && width) ++dat_iter,--width;
make_signed:
      switch (length) {
       case len_I8:i8:
        if (DEE_PP_CAT_3(Dee,ENC,String_ToInt8WithLength)(
         (Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.i8) == -1) goto err;
        append_object = DeeInt8_New(out_data.i8);
check_append_ob:
        if (!append_object) goto err;
        break;
       case len_I16:i16:
        if (DEE_PP_CAT_3(Dee,ENC,String_ToInt16WithLength)(
         (Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.i16) == -1) goto err;
        append_object = DeeInt16_New(out_data.i16);
        goto check_append_ob;
       case len_I32:i32:
        if (DEE_PP_CAT_3(Dee,ENC,String_ToInt32WithLength)(
         (Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.i32) == -1) goto err;
        append_object = DeeInt32_New(out_data.i32);
        goto check_append_ob;
       case len_I64:i64:
        if (DEE_PP_CAT_3(Dee,ENC,String_ToInt64WithLength)(
         (Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.i64) == -1) goto err;
        append_object = DeeInt64_New(out_data.i64);
        goto check_append_ob;
       case len_hh:  goto DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_CHAR));
       case len_h:   goto DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_SHORT));
       case len_l:   goto DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_LONG));
       case len_ll:  goto DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_LLONG));
       case len_j:   goto DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_INTMAX_T));
       case len_I:   goto DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_POINTER));
       case len_z:   goto DEE_PP_CAT_2(ui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_SIZE_T));
       case len_t:   goto DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T));
       default:      goto DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_TYPES_SIZEOF_INT));
      }
      break;

     case 'p':
      if ((*dat_iter == '+' || *dat_iter == '-') && width) ++dat_iter,--width;
      while (DEE_CH_IS_ALNUM(*dat_iter) && width) ++dat_iter,--width;
      if (DEE_PP_CAT_5(Dee,ENC,String_ToUInt,DEE_PP_MUL8(DEE_TYPES_SIZEOF_POINTER),WithLength)(
       (Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.DEE_PP_CAT_2(u,DEE_PP_MUL8(DEE_TYPES_SIZEOF_POINTER))) == -1) goto err;
      append_object = DEE_PP_CAT_3(DeeUInt,DEE_PP_MUL8(DEE_TYPES_SIZEOF_POINTER),_New)(
       out_data.DEE_PP_CAT_2(u,DEE_PP_MUL8(DEE_TYPES_SIZEOF_POINTER)));
      goto check_append_ob;

     case 'd':
      if ((*dat_iter == '+' || *dat_iter == '-') && width) ++dat_iter,--width;
      while (DEE_CH_IS_DIGIT(*dat_iter) && width) ++dat_iter,--width;
      // v prevent detection of octal numbers
      while (dat_begin < dat_iter-1 && *dat_begin == '0') ++dat_begin;
      goto make_signed;

     case 'u':
      if ((*dat_iter == '+' || *dat_iter == '-') && width) ++dat_iter,--width;
      while (DEE_CH_IS_DIGIT(*dat_iter) && width) ++dat_iter,--width;
      switch (length) {
       case len_I8:ui8:
        if (DEE_PP_CAT_3(Dee,ENC,String_ToUInt8WithLength)(
         (Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.u8) == -1) goto err;
gen_ui8:
        append_object = DeeUInt8_New(out_data.u8);
        goto check_append_ob;
       case len_I16:ui16:
        if (DEE_PP_CAT_3(Dee,ENC,String_ToUInt16WithLength)(
         (Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.u16) == -1) goto err;
gen_ui16:
        append_object = DeeUInt16_New(out_data.u16);
        goto check_append_ob;
       case len_I32:ui32:
        if (DEE_PP_CAT_3(Dee,ENC,String_ToUInt32WithLength)(
         (Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.u32) == -1) goto err;
gen_ui32:
        append_object = DeeUInt32_New(out_data.u32);
        goto check_append_ob;
       case len_I64:ui64:
        if (DEE_PP_CAT_3(Dee,ENC,String_ToUInt64WithLength)(
         (Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.u64) == -1) goto err;
gen_ui64:
        append_object = DeeUInt64_New(out_data.u64);
        goto check_append_ob;
       case len_hh:  goto DEE_PP_CAT_2(ui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_CHAR));
       case len_h:   goto DEE_PP_CAT_2(ui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_SHORT));
       case len_l:   goto DEE_PP_CAT_2(ui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_LONG));
       case len_ll:  goto DEE_PP_CAT_2(ui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_LLONG));
       case len_j:   goto DEE_PP_CAT_2(ui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_INTMAX_T));
       case len_I:   goto DEE_PP_CAT_2(ui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_POINTER));
       case len_z:   goto DEE_PP_CAT_2(ui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_SIZE_T));
       case len_t:   goto DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T));
       default:      goto DEE_PP_CAT_2(ui,DEE_PP_MUL8(DEE_TYPES_SIZEOF_INT));
      }
      break;

     case 'o': {
      int is_unsigned = 0;
      if (*dat_iter == '+' && width) ++dat_iter,--width;
      else if (*dat_iter == '-' && width) is_unsigned = 1,++dat_iter,--width;
      switch (length) {
       case len_I8:oui8: out_data.u8 = 0;
        while (*dat_iter >= '0' && *dat_iter <= '7' && width)
         out_data.u8 = (Dee_uint8_t)(out_data.u8*8+(Dee_uint8_t)(*dat_iter-'0')),++dat_iter,--width;
        if (is_unsigned) out_data.i8 = -out_data.i8;
        goto gen_ui8;
       case len_I16:oui16: out_data.u16 = 0;
        while (*dat_iter >= '0' && *dat_iter <= '7' && width)
         out_data.u16 = (Dee_uint16_t)(out_data.u16*8+(Dee_uint16_t)(*dat_iter-'0')),++dat_iter,--width;
        if (is_unsigned) out_data.i16 = -out_data.i16;
        goto gen_ui16;
       case len_I32:oui32: out_data.u32 = 0;
        while (*dat_iter >= '0' && *dat_iter <= '7' && width)
         out_data.u32 = (Dee_uint32_t)(out_data.u32*8+(Dee_uint32_t)(*dat_iter-'0')),++dat_iter,--width;
        if (is_unsigned) out_data.i32 = -out_data.i32;
        goto gen_ui32;
       case len_I64:oui64: out_data.u64 = 0;
        while (*dat_iter >= '0' && *dat_iter <= '7' && width)
         out_data.u64 = (Dee_uint64_t)(out_data.u64*8+(Dee_uint64_t)(*dat_iter-'0')),++dat_iter,--width;
        if (is_unsigned) out_data.i64 = -out_data.i64;
        goto gen_ui64;
       case len_hh: goto DEE_PP_CAT_2(oui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_CHAR));
       case len_h:  goto DEE_PP_CAT_2(oui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_SHORT));
       case len_l:  goto DEE_PP_CAT_2(oui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_LONG));
       case len_ll: goto DEE_PP_CAT_2(oui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_LLONG));
       case len_j:  goto DEE_PP_CAT_2(oui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_INTMAX_T));
       case len_I:  goto DEE_PP_CAT_2(oui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_POINTER));
       case len_z:  goto DEE_PP_CAT_2(oui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_SIZE_T));
       case len_t:
        out_data.DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T)) = 0;
        while (*dat_iter >= '0' && *dat_iter <= '7' && width)
         out_data.DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T)) =
         out_data.DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T))*8+
         (DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T))(*dat_iter-'0'),
         ++dat_iter,--width;
        if (is_unsigned)
          out_data.DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T)) =
         -out_data.DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T));
        append_object = DEE_PP_CAT_3(DeeInt,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T),_New)
         (out_data.DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T)));
        goto check_append_ob;
       default: goto DEE_PP_CAT_2(oui,DEE_PP_MUL8(DEE_TYPES_SIZEOF_INT));
      }
     } break;

     case 'x': {
      int is_unsigned = 0;
      if (*dat_iter == '+' && width) ++dat_iter,--width;
      else if (*dat_iter == '-' && width) is_unsigned = 1,++dat_iter,--width;
      if (*dat_iter == '0' && (dat_iter[1] == 'x' || dat_iter[1] == 'X') && width >= 2) dat_iter += 2,width -= 2;
      switch (length) {
       case len_I8:xui8: out_data.u8 = 0;
        while (width) {
         if (*dat_iter >= '0' && *dat_iter <= '9') out_data.u8 = (Dee_uint8_t)(out_data.u8*16+(Dee_uint8_t)(*dat_iter-'0'));
         else if (*dat_iter >= 'a' && *dat_iter <= 'f') out_data.u8 = (Dee_uint8_t)(out_data.u8*16+(10+(Dee_uint8_t)(*dat_iter-'a')));
         else if (*dat_iter >= 'A' && *dat_iter <= 'F') out_data.u8 = (Dee_uint8_t)(out_data.u8*16+(10+(Dee_uint8_t)(*dat_iter-'A')));
         else break; ++dat_iter,--width;
        }
        if (is_unsigned) out_data.i8 = -out_data.i8;
        goto gen_ui8;
       case len_I16:xui16: out_data.u16 = 0;
        while (width) {
         if (*dat_iter >= '0' && *dat_iter <= '9') out_data.u16 = (Dee_uint16_t)(out_data.u16*16+(Dee_uint16_t)(*dat_iter-'0'));
         else if (*dat_iter >= 'a' && *dat_iter <= 'f') out_data.u16 = (Dee_uint16_t)(out_data.u16*16+(10+(Dee_uint16_t)(*dat_iter-'a')));
         else if (*dat_iter >= 'A' && *dat_iter <= 'F') out_data.u16 = (Dee_uint16_t)(out_data.u16*16+(10+(Dee_uint16_t)(*dat_iter-'A')));
         else break; ++dat_iter,--width;
        }
        if (is_unsigned) out_data.i16 = -out_data.i16;
        goto gen_ui16;
       case len_I32:xui32: out_data.u32 = 0;
        while (width) {
         if (*dat_iter >= '0' && *dat_iter <= '9') out_data.u32 = (Dee_uint32_t)(out_data.u32*16+(Dee_uint32_t)(*dat_iter-'0'));
         else if (*dat_iter >= 'a' && *dat_iter <= 'f') out_data.u32 = (Dee_uint32_t)(out_data.u32*16+(10+(Dee_uint32_t)(*dat_iter-'a')));
         else if (*dat_iter >= 'A' && *dat_iter <= 'F') out_data.u32 = (Dee_uint32_t)(out_data.u32*16+(10+(Dee_uint32_t)(*dat_iter-'A')));
         else break; ++dat_iter,--width;
        }
        if (is_unsigned) out_data.i32 = -out_data.i32;
        goto gen_ui32;
       case len_I64:xui64: out_data.u64 = 0;
        while (width) {
         if (*dat_iter >= '0' && *dat_iter <= '9') out_data.u64 = (Dee_uint64_t)(out_data.u64*16+(Dee_uint64_t)(*dat_iter-'0'));
         else if (*dat_iter >= 'a' && *dat_iter <= 'f') out_data.u64 = (Dee_uint64_t)(out_data.u64*16+(10+(Dee_uint64_t)(*dat_iter-'a')));
         else if (*dat_iter >= 'A' && *dat_iter <= 'F') out_data.u64 = (Dee_uint64_t)(out_data.u64*16+(10+(Dee_uint64_t)(*dat_iter-'A')));
         else break; ++dat_iter,--width;
        }
        if (is_unsigned) out_data.i64 = -out_data.i64;
        goto gen_ui64;
       case len_hh: goto DEE_PP_CAT_2(xui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_CHAR));
       case len_h:  goto DEE_PP_CAT_2(xui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_SHORT));
       case len_l:  goto DEE_PP_CAT_2(xui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_LONG));
       case len_ll: goto DEE_PP_CAT_2(xui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_LLONG));
       case len_j:  goto DEE_PP_CAT_2(xui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_INTMAX_T));
       case len_I:  goto DEE_PP_CAT_2(xui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_POINTER));
       case len_z:  goto DEE_PP_CAT_2(xui,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_SIZE_T));
       case len_t:
        out_data.DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T)) = 0;
        while (1) {
         if (*dat_iter >= '0' && *dat_iter <= '9')
          out_data.DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T)) =
          out_data.DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T))*16+
          (DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T))(*dat_iter-'0');
         else if (*dat_iter >= 'a' && *dat_iter <= 'f')
          out_data.DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T)) =
          out_data.DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T))*16+
          (DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T))(10+(*dat_iter-'a'));
         else if (*dat_iter >= 'A' && *dat_iter <= 'F')
          out_data.DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T)) =
          out_data.DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T))*16+
          (DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T))(10+(*dat_iter-'A'));
         else break; ++dat_iter;
        }
        if (is_unsigned)
          out_data.DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T)) =
         -out_data.DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T));
        append_object = DEE_PP_CAT_3(DeeInt,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T),_New)
         (out_data.DEE_PP_CAT_2(i,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T)));
        goto check_append_ob;
       default: goto DEE_PP_CAT_2(xui,DEE_PP_MUL8(DEE_TYPES_SIZEOF_INT));
      }
     } break;

     case 'f': case 'e': case 'g': case 'a': {
      if ((*dat_iter == '+' || *dat_iter == '-') && width) ++dat_iter,--width;
      while (DEE_CH_IS_ALNUM(*dat_iter) && width) ++dat_iter,--width;
      if (*dat_iter == '.' && width) { ++dat_iter,--width;
       while (DEE_CH_IS_ALNUM(*dat_iter) && width) ++dat_iter,--width;
      }
      if ((*dat_iter == 'E' || *dat_iter == 'e') && width) { ++dat_iter,--width;
       if ((*dat_iter == '+' || *dat_iter == '-') && width) ++dat_iter,--width;
       while (DEE_CH_IS_DIGIT(*dat_iter) && width) ++dat_iter,--width;
      }
      switch (length) {
       case len_l:
#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_FLOAT
        if (DEE_PP_CAT_3(Dee,ENC,String_ToFloatWithLength)((Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.dbl) == -1) goto err;
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_DOUBLE
        if (DEE_PP_CAT_3(Dee,ENC,String_ToDoubleWithLength)((Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.dbl) == -1) goto err;
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_LDOUBLE
        if (DEE_PP_CAT_3(Dee,ENC,String_ToLDoubleWithLength)((Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.dbl) == -1) goto err;
#endif
        append_object = DeeDouble_New(out_data.dbl);
        goto check_append_ob;
       case len_L:
#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_FLOAT
        if (DEE_PP_CAT_3(Dee,ENC,String_ToFloatWithLength)((Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.ldbl) == -1) goto err;
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_DOUBLE
        if (DEE_PP_CAT_3(Dee,ENC,String_ToDoubleWithLength)((Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.ldbl) == -1) goto err;
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_LDOUBLE
        if (DEE_PP_CAT_3(Dee,ENC,String_ToLDoubleWithLength)((Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.ldbl) == -1) goto err;
#endif
        append_object = DeeLDouble_New(out_data.dbl);
        goto check_append_ob;
       default:
#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_FLOAT
        if (DEE_PP_CAT_3(Dee,ENC,String_ToFloatWithLength)((Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.flt) == -1) goto err;
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_DOUBLE
        if (DEE_PP_CAT_3(Dee,ENC,String_ToDoubleWithLength)((Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.flt) == -1) goto err;
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_LDOUBLE
        if (DEE_PP_CAT_3(Dee,ENC,String_ToLDoubleWithLength)((Dee_size_t)(dat_iter-dat_begin),dat_begin,&out_data.flt) == -1) goto err;
#endif
        append_object = DeeFloat_New(out_data.flt);
        goto check_append_ob;
      }
     } break;

     case 'c':
      // NOTE: We ignore the length here on purpose
      if (width == (Dee_size_t)-1) width = 1;
      if (width > (Dee_size_t)(dat_end-dat_iter))
       width = (Dee_size_t)(dat_end-dat_iter);
      append_object = DEE_PP_CAT_3(Dee,ENC,String_NewWithLength)(width,dat_iter);
      dat_iter += width;
      goto check_append_ob;

     case 's':
      // NOTE: We ignore the length here on purpose
      while (!DEE_CH_IS_SPACE(*dat_iter) && width) ++dat_iter,--width;
make_string:
      append_object = DEE_PP_CAT_3(Dee,ENC,String_NewWithLength)(
       (Dee_size_t)(dat_iter-dat_begin),dat_begin);
      goto check_append_ob;

     case 'n':
      switch (length) {
       case len_I8:ni8:   append_object = DeeInt8_New((Dee_int8_t)(dat_iter-DEE_PP_CAT_3(Dee,ENC,String_STR)(data))); goto check_append_ob;
       case len_I16:ni16: append_object = DeeInt16_New((Dee_int16_t)(dat_iter-DEE_PP_CAT_3(Dee,ENC,String_STR)(data))); goto check_append_ob;
       case len_I32:ni32: append_object = DeeInt32_New((Dee_int32_t)(dat_iter-DEE_PP_CAT_3(Dee,ENC,String_STR)(data))); goto check_append_ob;
       case len_I64:ni64: append_object = DeeInt64_New((Dee_int64_t)(dat_iter-DEE_PP_CAT_3(Dee,ENC,String_STR)(data))); goto check_append_ob;
       case len_hh: goto DEE_PP_CAT_2(ni,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_CHAR));
       case len_h:  goto DEE_PP_CAT_2(ni,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_SHORT));
       case len_l:  goto DEE_PP_CAT_2(ni,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_LONG));
       case len_ll: goto DEE_PP_CAT_2(ni,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_LLONG));
       case len_j:  goto DEE_PP_CAT_2(ni,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_INTMAX_T));
       case len_I:  goto DEE_PP_CAT_2(ni,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_POINTER));
       case len_t:  goto DEE_PP_CAT_2(ni,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T));
       case len_z:
        append_object = DEE_PP_CAT_3(DeeInt,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_SIZE_T),_New)((
         DEE_TYPES_INT(DEE_CONFIG_RT_SIZEOF_SIZE_T))(dat_iter-
         DEE_PP_CAT_3(Dee,ENC,String_STR)(data)));
        goto check_append_ob;
       default: goto DEE_PP_CAT_2(ni,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_INT));
      } break;

     case '%': goto do_match;

     case '[': {
      int inverse_selection = 0,found;
      ENC_CHAR const *sel_begin,*sel_iter,*sel_end;
      if (*fmt_iter == '^') inverse_selection = 1,++fmt_iter;
      sel_begin = fmt_iter;
      while (fmt_iter != fmt_end) {
       if (*fmt_iter == '\\') fmt_iter += 2; // escape the following character
       else if (*fmt_iter == ']') break;
       else ++fmt_iter;
      }
      sel_end = fmt_iter;
      // Skip the closing ']' character
      if (fmt_iter != fmt_end) ++fmt_iter;
      while (dat_iter != dat_end) {
       ch = *dat_iter;
       sel_iter = sel_begin;
       found = 0;
       while (sel_iter != sel_end) {
        if (sel_iter[1] == '-') { // range
         if (ch >= sel_iter[0] && ch <= sel_iter[2]) { found = 1; break; }
         sel_iter += 3;
        } else if (*sel_iter == '\\') {
         if (ch == sel_iter[1]) { found = 1; break; }
         sel_iter += 2;
        } else {
         if (ch == *sel_iter) { found = 1; break; }
         ++sel_iter;
        }
       }
       if ((found ^ inverse_selection) == 0) break;
       ++dat_iter;
      }
      goto make_string;
     } break;


     default: Dee_INCREF(append_object = Dee_None); break;
    }
    if (!ignore_data && DeeTuple_Append(
     &result,append_object) == -1) {
     Dee_DECREF(append_object);
     goto err;
    }
    Dee_DECREF(append_object);
    if (fmt_iter >= fmt_end) break;
   } break;

   default:do_match:
    if (*dat_iter == ch && dat_iter != dat_end) ++dat_iter; else {
     DeeError_SetStringf(&DeeErrorType_ValueError,
                         "Unexpected character in scanf: Expected '%#.1q', got: '%#.1q'",
                         &ch,dat_iter);
     goto err;
    }
    break;
  }
 }
end: return result;
err: Dee_CLEAR(result); goto end;
}
DEE_COMPILER_MSVC_WARNING_POP



#undef ENC_CHAR
#ifdef __INTELLISENSE__
DEE_DECL_END
#endif

