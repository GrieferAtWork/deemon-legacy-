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
#define ENC Utf8
#endif

#ifndef ENC
#error Must #define ENC as 'Wide' / 'Utf8' / 'Utf16' / 'Utf32' before including this file
#endif

#include <deemon/__min_int_t.inl>
#include <deemon/__string_encoding.inl>

#ifndef DEE_STRING_ENC_CAT_4
#define DEE_PRIVATE_STRING_ENC_CAT_3(a,b,c)   a##b##c
#define DEE_PRIVATE_STRING_ENC_CAT_4(a,b,c,d) a##b##c##d
#define DEE_STRING_ENC_CAT_3(a,b,c)   DEE_PRIVATE_STRING_ENC_CAT_3(a,b,c)
#define DEE_STRING_ENC_CAT_4(a,b,c,d) DEE_PRIVATE_STRING_ENC_CAT_4(a,b,c,d)
#endif

#define IS_ENCODING(enc) (DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(enc))
#define Dee_F(x)                 DEE_STRING_ENC_CAT_3(Dee_,ENC,x)
#define _deestring_F(x)          DEE_STRING_ENC_CAT_4(_dee,ENC,string_,x)
#define _deestringclass_F(x)     DEE_STRING_ENC_CAT_4(_dee,ENC,stringclass_,x)
#define _deestringiterator_F(x)  DEE_STRING_ENC_CAT_4(_dee,ENC,stringiterator_,x)
#define DeeString_F(x)           DEE_STRING_ENC_CAT_4(Dee,ENC,String_,x)
#define DeeStringIterator_F(x)   DEE_STRING_ENC_CAT_4(Dee,ENC,StringIterator_,x)
#define DeeStringWriter_F(x)     DEE_STRING_ENC_CAT_4(Dee,ENC,StringWriter_,x)
#define _Dee_F(x)                DEE_STRING_ENC_CAT_3(_Dee_,ENC,x)
#define _DeeString_F(x)          DEE_STRING_ENC_CAT_4(_Dee,ENC,String_,x)
#define _DeeStringIterator_F(x)  DEE_STRING_ENC_CAT_4(_Dee,ENC,StringIterator_,x)
#define _DeeStringWriter_F(x)    DEE_STRING_ENC_CAT_4(_Dee,ENC,StringWriter_,x)
#define DEE_CHAR                 DEE_STRING_ENC_CAT_3(Dee_,ENC,Char)
#define DEE_STRINGOBJECT         DEE_STRING_ENC_CAT_3(Dee,ENC,StringObject)
#define DEE_STRINGITERATOROBJECT DEE_STRING_ENC_CAT_3(Dee,ENC,StringIteratorObject)
#define DEE_STRINGWRITER         DEE_STRING_ENC_CAT_3(Dee,ENC,StringWriter)
#define DEE_EMPTYSTRING          DEE_STRING_ENC_CAT_3(Dee_Empty,ENC,String)
#define DEERETURN_EMPTYSTRING    DEE_STRING_ENC_CAT_3(DeeReturn_Empty,ENC,String)

#define DEE_STRING_Type          DeeString_F(Type)
#define DEE_STRING_Check         DeeString_F(Check)
#define DEE_STRING_STR           DeeString_F(STR)
#define DEE_STRING_SIZE          DeeString_F(SIZE)
#define DEE_STRINGWRITER_INIT    DeeStringWriter_F(INIT)
#define DEE_STRINGWRITER_Pack    DeeStringWriter_F(Pack)
#define DEE_STRINGWRITER_Quit    DeeStringWriter_F(Quit)

#if DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(Utf8)
#define DEE_CHAR_C DEE_CHAR8_C
#elif DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(Wide)
#define DEE_CHAR_C DEE_WIDECHAR_C
#elif DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(Utf16)
#define DEE_CHAR_C DEE_CHAR16_C
#elif DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(Utf32)
#define DEE_CHAR_C DEE_CHAR32_C
#else
#define DEE_CHAR_C (DEE_CHAR)
#endif


#if DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING)
#define DEE_STRING_TYPE_NAME "string"
#elif IS_ENCODING(Wide)
#define DEE_STRING_TYPE_NAME "string.wide"
#elif IS_ENCODING(Utf8)
#define DEE_STRING_TYPE_NAME "string.utf8"
#elif IS_ENCODING(Utf16)
#define DEE_STRING_TYPE_NAME "string.utf16"
#elif IS_ENCODING(Utf32)
#define DEE_STRING_TYPE_NAME "string.utf32"
#else
#error "Cannot deduce string encoding name"
#endif

#if IS_ENCODING(Wide)
#define DEE_CHAR_USERPREFIX  "L"
#define DEE_CHAR_TYPE_NAME   "wchar_t"
#elif IS_ENCODING(Utf8)
#define DEE_CHAR_USERPREFIX  ""
#define DEE_CHAR_TYPE_NAME   "char"
#elif IS_ENCODING(Utf16)
#define DEE_CHAR_USERPREFIX  "u"
#define DEE_CHAR_TYPE_NAME   "char16_t"
#elif IS_ENCODING(Utf32)
#define DEE_CHAR_USERPREFIX  "U"
#define DEE_CHAR_TYPE_NAME   "char32_t"
#else
#error "Cannot deduce string encoding name"
#endif

#if IS_ENCODING(Utf8)
#define DEE_SIZEOF_ENC  DEE_TYPES_SIZEOF_CHAR
#elif IS_ENCODING(Utf16)
#define DEE_SIZEOF_ENC  DEE_TYPES_SIZEOF_CHAR16_T
#elif IS_ENCODING(Utf32)
#define DEE_SIZEOF_ENC  DEE_TYPES_SIZEOF_CHAR32_T
#elif IS_ENCODING(Wide)
#define DEE_SIZEOF_ENC  DEE_TYPES_SIZEOF_WCHAR_T
#endif

DEE_DECL_BEGIN

#ifndef DEE_PRIVATE_DEE_WINT_T_DEFINED
#define DEE_PRIVATE_DEE_WINT_T_DEFINED
#if defined(_MSC_VER)
typedef wint_t       Dee_wint_t;
#else
typedef unsigned int Dee_wint_t;
#endif
#endif

struct DEE_PP_CAT_3(_DeeEmpty,ENC,StringObject) DEE_PP_CAT_3(_Dee_Empty,ENC,String) = {
 DEE_OBJECT_HEAD_INIT(&DEE_STRING_Type),0,{0}};

#if IS_ENCODING(Utf8)
struct _DeeUtf8StringCharacterObject _DeeUtf8String_Characters[256] = {
#define C(c) {DEE_OBJECT_HEAD_INIT(&DEE_STRING_Type),1,c}
 C("\x00"),C("\x01"),C("\x02"),C("\x03"),C("\x04"),C("\x05"),C("\x06"),C("\x07"),C("\x08"),C("\t"),C("\n"),C("\x0b"),C("\x0c"),C("\r"),C("\x0e"),C("\x0f"),
 C("\x10"),C("\x11"),C("\x12"),C("\x13"),C("\x14"),C("\x15"),C("\x16"),C("\x17"),C("\x18"),C("\x19"),C("\x1a"),C("\x1b"),C("\x1c"),C("\x1d"),C("\x1e"),C("\x1f"),
 C(" "),C("!"),C("\""),C("#"),C("$"),C("%"),C("&"),C("'"),C("("),C(")"),C("*"),C("+"),C(","),C("-"),C("."),C("/"),
 C("0"),C("1"),C("2"),C("3"),C("4"),C("5"),C("6"),C("7"),C("8"),C("9"),C(":"),C(";"),C("<"),C("="),C(">"),C("?"),
 C("@"),C("A"),C("B"),C("C"),C("D"),C("E"),C("F"),C("G"),C("H"),C("I"),C("J"),C("K"),C("L"),C("M"),C("N"),C("O"),
 C("P"),C("Q"),C("R"),C("S"),C("T"),C("U"),C("V"),C("W"),C("X"),C("Y"),C("Z"),C("["),C("\\"),C("]"),C("^"),C("_"),
 C("`"),C("a"),C("b"),C("c"),C("d"),C("e"),C("f"),C("g"),C("h"),C("i"),C("j"),C("k"),C("l"),C("m"),C("n"),C("o"),
 C("p"),C("q"),C("r"),C("s"),C("t"),C("u"),C("v"),C("w"),C("x"),C("y"),C("z"),C("{"),C("|"),C("}"),C("~"),C("\x7f"),
 C("\x80"),C("\x81"),C("\x82"),C("\x83"),C("\x84"),C("\x85"),C("\x86"),C("\x87"),C("\x88"),C("\x89"),C("\x8a"),C("\x8b"),C("\x8c"),C("\x8d"),C("\x8e"),C("\x8f"),
 C("\x90"),C("\x91"),C("\x92"),C("\x93"),C("\x94"),C("\x95"),C("\x96"),C("\x97"),C("\x98"),C("\x99"),C("\x9a"),C("\x9b"),C("\x9c"),C("\x9d"),C("\x9e"),C("\x9f"),
 C("\xa0"),C("\xa1"),C("\xa2"),C("\xa3"),C("\xa4"),C("\xa5"),C("\xa6"),C("\xa7"),C("\xa8"),C("\xa9"),C("\xaa"),C("\xab"),C("\xac"),C("\xad"),C("\xae"),C("\xaf"),
 C("\xb0"),C("\xb1"),C("\xb2"),C("\xb3"),C("\xb4"),C("\xb5"),C("\xb6"),C("\xb7"),C("\xb8"),C("\xb9"),C("\xba"),C("\xbb"),C("\xbc"),C("\xbd"),C("\xbe"),C("\xbf"),
 C("\xc0"),C("\xc1"),C("\xc2"),C("\xc3"),C("\xc4"),C("\xc5"),C("\xc6"),C("\xc7"),C("\xc8"),C("\xc9"),C("\xca"),C("\xcb"),C("\xcc"),C("\xcd"),C("\xce"),C("\xcf"),
 C("\xd0"),C("\xd1"),C("\xd2"),C("\xd3"),C("\xd4"),C("\xd5"),C("\xd6"),C("\xd7"),C("\xd8"),C("\xd9"),C("\xda"),C("\xdb"),C("\xdc"),C("\xdd"),C("\xde"),C("\xdf"),
 C("\xe0"),C("\xe1"),C("\xe2"),C("\xe3"),C("\xe4"),C("\xe5"),C("\xe6"),C("\xe7"),C("\xe8"),C("\xe9"),C("\xea"),C("\xeb"),C("\xec"),C("\xed"),C("\xee"),C("\xef"),
 C("\xf0"),C("\xf1"),C("\xf2"),C("\xf3"),C("\xf4"),C("\xf5"),C("\xf6"),C("\xf7"),C("\xf8"),C("\xf9"),C("\xfa"),C("\xfb"),C("\xfc"),C("\xfd"),C("\xfe"),C("\xff"),
#undef C
};
#endif

#ifndef DEE_PRIVATE_DEE_DECIMAL_DEFAULLT_SPEC_DEFINED
#define DEE_PRIVATE_DEE_DECIMAL_DEFAULLT_SPEC_DEFINED
DeeStringWriterFormatSpec const dee_decimal_default_spec = 
 DeeStringWriterFormatSpec_INIT_BASIC(10);
#endif


#define DEE_PRIVATE_MAKE_STRING_FROM_INTEGER(name,write_fun,T)\
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *name(DEE_A_IN T v) {\
 DeeObject *result;\
 DEE_STRINGWRITER writer = DEE_STRINGWRITER_INIT();\
 if (write_fun(&writer,v,&dee_decimal_default_spec) != -1)\
  result = DEE_STRINGWRITER_Pack(&writer);\
 else result = NULL;\
 DEE_STRINGWRITER_Quit(&writer);\
 return result;\
}
DEE_PRIVATE_MAKE_STRING_FROM_INTEGER(DeeString_F(FromInt8),DeeStringWriter_F(SpecWriteInt8),Dee_int8_t)
DEE_PRIVATE_MAKE_STRING_FROM_INTEGER(DeeString_F(FromInt16),DeeStringWriter_F(SpecWriteInt16),Dee_int16_t)
DEE_PRIVATE_MAKE_STRING_FROM_INTEGER(DeeString_F(FromInt32),DeeStringWriter_F(SpecWriteInt32),Dee_int32_t)
DEE_PRIVATE_MAKE_STRING_FROM_INTEGER(DeeString_F(FromInt64),DeeStringWriter_F(SpecWriteInt64),Dee_int64_t)
DEE_PRIVATE_MAKE_STRING_FROM_INTEGER(DeeString_F(FromUInt8),DeeStringWriter_F(SpecWriteUInt8),Dee_uint8_t)
DEE_PRIVATE_MAKE_STRING_FROM_INTEGER(DeeString_F(FromUInt16),DeeStringWriter_F(SpecWriteUInt16),Dee_uint16_t)
DEE_PRIVATE_MAKE_STRING_FROM_INTEGER(DeeString_F(FromUInt32),DeeStringWriter_F(SpecWriteUInt32),Dee_uint32_t)
DEE_PRIVATE_MAKE_STRING_FROM_INTEGER(DeeString_F(FromUInt64),DeeStringWriter_F(SpecWriteUInt64),Dee_uint64_t)
#ifdef DEE_TYPES_SIZEOF_FLOAT
DEE_PRIVATE_MAKE_STRING_FROM_INTEGER(DeeString_F(FromFloat),DeeStringWriter_F(SpecWriteFloat),float)
#endif
#ifdef DEE_TYPES_SIZEOF_DOUBLE
DEE_PRIVATE_MAKE_STRING_FROM_INTEGER(DeeString_F(FromDouble),DeeStringWriter_F(SpecWriteDouble),double)
#endif
#ifdef DEE_TYPES_SIZEOF_LDOUBLE
DEE_PRIVATE_MAKE_STRING_FROM_INTEGER(DeeString_F(FromLDouble),DeeStringWriter_F(SpecWriteLDouble),long double)
#endif
#undef DEE_PRIVATE_MAKE_STRING_FROM_INTEGER


#ifndef __INTELLISENSE__
#define UNSIGNED 0
#define BITS 8
#include "string.impl.stoi.inl"
#define BITS 16
#include "string.impl.stoi.inl"
#define BITS 32
#include "string.impl.stoi.inl"
#define BITS 64
#include "string.impl.stoi.inl"
#undef UNSIGNED
#define UNSIGNED 1
#define BITS 8
#include "string.impl.stoi.inl"
#define BITS 16
#include "string.impl.stoi.inl"
#define BITS 32
#include "string.impl.stoi.inl"
#define BITS 64
#include "string.impl.stoi.inl"
#undef UNSIGNED
#define KIND 0
#include "string.impl.stof.inl"
#define KIND 1
#include "string.impl.stof.inl"
#define KIND 2
#include "string.impl.stof.inl"
#endif

#ifndef __INTELLISENSE__
#include "string.impl.format.inl"
#include "string.impl.scanf.inl"
#endif



DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DEE_PP_CAT_3(DeeObject_T,ENC,Str)(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
#if DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING)
 return DeeObject_TStr(tp_self,self);
#else
 DeeObject *str_,*result;
 if ((str_ = DeeObject_TStr(tp_self,self)) == NULL) return NULL;
 result = DeeString_F(DEE_PP_CAT_3(From,DEE_CONFIG_DEFAULT_ENCODING,StringWithLength))(
  DeeString_SIZE(str_),DeeString_STR(str_));
 Dee_DECREF(str_);
 return result;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DEE_PP_CAT_3(DeeObject_T,ENC,Repr)(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
#if DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING)
 return DeeObject_TRepr(tp_self,self);
#else
 DeeObject *str_,*result;
 if ((str_ = DeeObject_TRepr(tp_self,self)) == NULL) return NULL;
 result = DeeString_F(DEE_PP_CAT_3(From,DEE_CONFIG_DEFAULT_ENCODING,StringWithLength))(
  DeeString_SIZE(str_),DeeString_STR(str_));
 Dee_DECREF(str_);
 return result;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DEE_PP_CAT_3(DeeObject_P,ENC,Str)(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) {
#if DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING)
 return DeeObject_PStr(tp_self,base_addr);
#else
 DeeObject *str_,*result;
 if ((str_ = DeeObject_PStr(tp_self,base_addr)) == NULL) return NULL;
 result = DeeString_F(DEE_PP_CAT_3(From,DEE_CONFIG_DEFAULT_ENCODING,StringWithLength))(
  DeeString_SIZE(str_),DeeString_STR(str_));
 Dee_DECREF(str_);
 return result;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DEE_PP_CAT_3(DeeObject_P,ENC,Repr)(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) {
#if DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING)
 return DeeObject_PRepr(tp_self,base_addr);
#else
 DeeObject *str_,*result;
 if ((str_ = DeeObject_PRepr(tp_self,base_addr)) == NULL) return NULL;
 result = DeeString_F(DEE_PP_CAT_3(From,DEE_CONFIG_DEFAULT_ENCODING,StringWithLength))(
  DeeString_SIZE(str_),DeeString_STR(str_));
 Dee_DECREF(str_);
 return result;
#endif
}

DEE_A_RET_Z DEE_CHAR const *DeeString_F(Str)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 return DEE_STRING_STR(self);
}
DEE_A_RET_WUNUSED Dee_size_t DeeString_F(Size)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 return DEE_STRING_SIZE(self);
}


DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeString_F(CastEx)(DEE_A_IN DeeObject const *ob, DEE_A_IN Dee_uint32_t flags) {
 DeeTypeObject *ob_type;
 DEE_ASSERT(DeeObject_Check(ob));
 ob_type = Dee_TYPE(ob);
 if (ob_type == &DEE_STRING_Type) DeeReturn_NEWREF(ob);
#if DEE_CONFIG_HAVE_ENCODING_UTF8 && \
    DEE_STRING_ENCODING(ENC) != DEE_STRING_ENCODING(Utf8)
 if (ob_type == &DeeUtf8String_Type) return DeeString_F(
  FromUtf8StringWithLengthEx)(DeeUtf8String_SIZE(ob),DeeUtf8String_STR(ob),flags);
#endif
#if DEE_CONFIG_HAVE_ENCODING_WIDE && \
    DEE_STRING_ENCODING(ENC) != DEE_STRING_ENCODING(Wide)
 if (ob_type == &DeeWideString_Type) return DeeString_F(
  FromWideStringWithLengthEx)(DeeWideString_SIZE(ob),DeeWideString_STR(ob),flags);
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16 && \
    DEE_STRING_ENCODING(ENC) != DEE_STRING_ENCODING(Utf16)
 if (ob_type == &DeeUtf16String_Type) return DeeString_F(
  FromUtf16StringWithLengthEx)(DeeUtf16String_SIZE(ob),DeeUtf16String_STR(ob),flags);
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32 && \
    DEE_STRING_ENCODING(ENC) != DEE_STRING_ENCODING(Utf32)
 if (ob_type == &DeeUtf32String_Type) return DeeString_F(
  FromUtf32StringWithLengthEx)(DeeUtf32String_SIZE(ob),DeeUtf32String_STR(ob),flags);
#endif
 if (ob_type == DeeObject_TYPE(none)) DEERETURN_EMPTYSTRING;
 DeeError_TypeError_UnexpectedType((DeeObject *)ob,&DEE_STRING_Type);
 return NULL;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeString_F(NewSized)(DEE_A_IN Dee_size_t len) {
 DEE_STRINGOBJECT *result;
 if DEE_LIKELY((result = (DEE_STRINGOBJECT *)DeeObject_Mallocf(
  Dee_OFFSETOF(DEE_STRINGOBJECT,s_str)+(len+1)*sizeof(DEE_CHAR),
  DEE_PP_STR(DEE_STRINGOBJECT) " (len = %u)",(unsigned)len)) != NULL) {
  DeeObject_INIT(result,&DEE_STRING_Type);
  result->s_len = len;
  result->s_str[len] = 0;
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeString_F(Newf)(DEE_A_IN_PRINTF DEE_CHAR const *fmt, ...) {
 DeeObject *result;
 va_list args;
 DEE_VA_START(args,fmt);
 result = DeeString_F(VNewf)(fmt,args);
 DEE_VA_END(args);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(VNewf)(
 DEE_A_IN_PRINTF DEE_CHAR const *fmt, DEE_A_IN va_list args) {
 DeeObject *result;
 DEE_STRINGWRITER writer = DEE_STRINGWRITER_INIT();
 if DEE_UNLIKELY(DeeStringWriter_F(VWritef)(&writer,fmt,args) == -1) {
  result = NULL;
  goto end;
 }
 result = DEE_STRINGWRITER_Pack(&writer);
end: DEE_STRINGWRITER_Quit(&writer);
 return result;
}


#if DEE_CONFIG_HAVE_ENCODING_WIDE
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeString_F(WideRepr)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DeeObject *result;
 DeeWideStringWriter writer = DeeWideStringWriter_INIT();
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 if DEE_UNLIKELY(DEE_PP_CAT_3(DeeWideStringWriter_SpecWriteQuoted,ENC,StringWithLength)(
  &writer,DEE_STRING_SIZE(self),DEE_STRING_STR(self),
  &dee_decimal_default_spec) == -1) {
  result = NULL;
  goto end;
 }
 result = DeeWideStringWriter_Pack(&writer);
end: DeeWideStringWriter_Quit(&writer);
 return result;
}
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF8
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeString_F(Utf8Repr)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DeeObject *result;
 DeeUtf8StringWriter writer = DeeUtf8StringWriter_INIT();
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 if DEE_UNLIKELY(DEE_PP_CAT_3(DeeUtf8StringWriter_SpecWriteQuoted,ENC,StringWithLength)(
  &writer,DEE_STRING_SIZE(self),DEE_STRING_STR(self),
  &dee_decimal_default_spec) == -1) {
  result = NULL;
  goto end;
 }
 result = DeeUtf8StringWriter_Pack(&writer);
end: DeeUtf8StringWriter_Quit(&writer);
 return result;
}
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf16StringObject) *
DeeString_F(Utf16Repr)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DeeObject *result;
 DeeUtf16StringWriter writer = DeeUtf16StringWriter_INIT();
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 if DEE_UNLIKELY(DEE_PP_CAT_3(DeeUtf16StringWriter_SpecWriteQuoted,ENC,StringWithLength)(
  &writer,DEE_STRING_SIZE(self),DEE_STRING_STR(self),
  &dee_decimal_default_spec) == -1) {
  result = NULL;
  goto end;
 }
 result = DeeUtf16StringWriter_Pack(&writer);
end: DeeUtf16StringWriter_Quit(&writer);
 return result;
}
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf32StringObject) *
DeeString_F(Utf32Repr)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DeeObject *result;
 DeeUtf32StringWriter writer = DeeUtf32StringWriter_INIT();
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 if DEE_UNLIKELY(DEE_PP_CAT_3(DeeUtf32StringWriter_SpecWriteQuoted,ENC,StringWithLength)(
  &writer,DEE_STRING_SIZE(self),DEE_STRING_STR(self),
  &dee_decimal_default_spec) == -1) {
  result = NULL;
  goto end;
 }
 result = DeeUtf32StringWriter_Pack(&writer);
end: DeeUtf32StringWriter_Quit(&writer);
 return result;
}
#endif

DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(NewHexDump)(
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s) {
 if (!s) DEERETURN_EMPTYSTRING;
 {DeeObject *result;
  DEE_STRINGWRITER writer = DEE_STRINGWRITER_INIT();
  if DEE_UNLIKELY(DeeStringWriter_F(WriteHexDump)(&writer,p,s) == -1)
   result = NULL;
  else result = DEE_STRINGWRITER_Pack(&writer);
  DEE_STRINGWRITER_Quit(&writer);
  return result;
 }
}


DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGITERATOROBJECT) *
DeeString_F(IterSelf)(DEE_A_INOUT_OBJECT(DEE_STRINGOBJECT) const *self) {
 DEE_STRINGITERATOROBJECT *result;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 if DEE_LIKELY((result = DeeObject_MALLOCF(DEE_STRINGITERATOROBJECT,"string.iterator for %p",self)) != NULL) {
  DeeObject_INIT(result,&DeeStringIterator_F(Type));
  Dee_INCREF(result->si_string = (DEE_STRINGOBJECT *)self);
  result->si_end = (result->si_pos = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 }
 return (DeeObject *)result;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeStringIterator_F(IterNext)(
 DEE_A_INOUT_OBJECT(DEE_STRINGITERATOROBJECT) *self,
 DEE_A_REF DEE_A_OUT_OBJECT(DEE_STRINGOBJECT) **result) {
 DEE_CHAR *itercharpos;
 DEE_ASSERT(DeeObject_Check(self) && DeeStringIterator_F(Check)(self));
 itercharpos = (DEE_CHAR *)DeeAtomicPtr_FetchAdd(
  ((DEE_STRINGITERATOROBJECT *)self)->si_pos,
  sizeof(DEE_CHAR),memory_order_seq_cst);
 if DEE_UNLIKELY(itercharpos >= ((DEE_STRINGITERATOROBJECT *)self)->si_end) return 1; // Iterator end
#if DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(Utf8)
 Dee_INCREF(*result = DeeUtf8String_Character(*itercharpos));
#else
 if DEE_UNLIKELY((*result = DeeString_F(NewSized)(1)) == NULL) return -1;
 DEE_STRING_STR(*result)[0] = *itercharpos;
#endif
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeString_F(Resize)(
 DEE_A_INOUT_OBJECT(DEE_STRINGOBJECT) **self, DEE_A_IN Dee_size_t new_size) {
 DEE_STRINGOBJECT *new_str;
 DEE_ASSERT(self);
 if DEE_UNLIKELY(!*self) return ((*self = DeeString_F(NewSized)(new_size)) != NULL) ? 0 : -1;
 if DEE_UNLIKELY(!new_size) {
  Dee_DECREF(*self);
  Dee_INCREF(*self = DEE_EMPTYSTRING);
  return 0;
 }
 if DEE_LIKELY(DeeObject_IS_UNIQUE(*self)) {
  if ((new_str = (DEE_STRINGOBJECT *)DeeObject_Realloc(*self,
   Dee_OFFSETOF(DEE_STRINGOBJECT,s_str)+(new_size+1)*sizeof(DEE_CHAR))) == NULL) return -1;
  new_str->s_len = new_size;
  new_str->s_str[new_size] = 0;
 } else {
  Dee_size_t old_size = DEE_STRING_SIZE(*self);
  if DEE_UNLIKELY((new_str = (DEE_STRINGOBJECT *)DeeString_F(NewSized)(new_size)) == NULL) return -1;
  memcpy(DEE_STRING_STR(new_str),DEE_STRING_STR(*self),
         (old_size < new_size ? old_size : new_size)*sizeof(DEE_CHAR));
  Dee_DECREF(*self);
 }
 Dee_INHERIT_REF(*self,*(DeeObject **)&new_str);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeString_F(AppendWithLength)(
 DEE_A_INOUT_OBJECT(DEE_STRINGOBJECT) **self,
 DEE_A_IN Dee_size_t rhs_size, DEE_A_IN_R(rhs_size) DEE_CHAR const *rhs) {
 Dee_size_t old_len;
 DEE_ASSERT(self);
 DEE_ASSERT(!*self || (DeeObject_Check(*self) && DEE_STRING_Check(*self)));
 if DEE_UNLIKELY(!rhs_size) return 0;
 if DEE_UNLIKELY((DeeString_F(Resize)(self,(old_len = (
  *self ? DEE_STRING_SIZE(*self) : 0))+rhs_size)) == -1) return -1;
 memcpy(DEE_STRING_STR(*self)+old_len,rhs,rhs_size);
 return 0;
}






DEE_A_RET_MAYBE_NULL DEE_A_RET_WUNUSED DEE_CHAR const *
DeeStringWriter_F(Str)(DEE_A_IN DEE_STRINGWRITER const *self) {
 DEE_ASSERT(self);
 return DeeStringWriter_F(STR)(self);
}
DEE_A_RET_NEVER_NULL DEE_A_RET_WUNUSED DEE_CHAR const *
DeeStringWriter_F(Elem)(DEE_A_IN DEE_STRINGWRITER const *self) {
 DEE_ASSERT(self);
 DEE_ASSERT(DeeStringWriter_F(ALLOCATED)(self) &&
            "Undefined behavior: '" DEE_PP_STR(DeeStringWriter_F(Elem)) "' called "
            "on string writer without allocated memory");
 return DeeStringWriter_F(ELEM)(self);
}
DEE_A_RET_WUNUSED int DeeStringWriter_F(Empty)(
 DEE_A_IN DEE_STRINGWRITER const *self) {
 DEE_ASSERT(self);
 return DeeStringWriter_F(EMPTY)(self);
}
DEE_A_RET_WUNUSED Dee_size_t DeeStringWriter_F(Size)(
 DEE_A_IN DEE_STRINGWRITER const *self) {
 DEE_ASSERT(self);
 return DeeStringWriter_F(SIZE)(self);
}
DEE_A_RET_WUNUSED Dee_size_t DeeStringWriter_F(Allocated)(
 DEE_A_IN DEE_STRINGWRITER const *self) {
 DEE_ASSERT(self);
 return DeeStringWriter_F(ALLOCATED)(self);
}

DEE_A_RET_EXCEPT(NULL) DEE_STRINGWRITER *DeeStringWriter_F(Alloc)(void) {
 DEE_STRINGWRITER *result;
 while DEE_UNLIKELY((result = (DEE_STRINGWRITER *)
  malloc_nz(sizeof(DEE_STRINGWRITER))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 return result;
}
DEE_A_RET_EXCEPT(NULL) DEE_STRINGWRITER *DeeStringWriter_F(New)(void) {
 DEE_STRINGWRITER *result;
 while DEE_UNLIKELY((result = (DEE_STRINGWRITER *)
  malloc_nz(sizeof(DEE_STRINGWRITER))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 result->sw_len = 0;
 result->sw_str = NULL;
 return result;
}
void DeeStringWriter_F(Delete)(
 DEE_A_IN DEE_A_OUT_INVALID DEE_STRINGWRITER *self) {
 DEE_ASSERT(self);
 DEE_STRINGWRITER_Quit(self);
 free_nn(self);
}
void DeeStringWriter_F(Free)(
 DEE_A_IN DEE_A_OUT_INVALID DEE_STRINGWRITER *self) {
 DEE_ASSERT(self);
 free_nn(self);
}
void DeeStringWriter_F(Init)(
 DEE_A_OUT DEE_STRINGWRITER *self) {
 DEE_ASSERT(self);
 self->sw_len = 0;
 self->sw_str = NULL;
}
void DeeStringWriter_F(InitWithSizeHint)(
 DEE_A_OUT DEE_STRINGWRITER *self, DEE_A_IN Dee_size_t size_hint) {
 DEE_ASSERT(self);
 if (size_hint) {
  if DEE_UNLIKELY((self->sw_str = (DEE_STRINGOBJECT *)DeeObject_TryMallocf(
   Dee_OFFSETOF(DEE_STRINGOBJECT,s_str)+((size_hint+1)*sizeof(DEE_CHAR)),
   "StringWriter string (sizehint: %lu)",(unsigned long)size_hint)) == NULL) goto def;
  DeeObject_INIT(self->sw_str,&DEE_STRING_Type);
  self->sw_str->s_len = size_hint;
  self->sw_len = 0;
 } else {
def: DeeStringWriter_F(Init)(self);
 }
}
void DeeStringWriter_F(InitMove)(
 DEE_A_OUT DEE_STRINGWRITER *self, DEE_A_INOUT DEE_STRINGWRITER *right) {
 DEE_ASSERT(self && right && self != right);
 self->sw_str = right->sw_str;
 self->sw_len = right->sw_len;
 right->sw_str = NULL;
 right->sw_len = 0;
}
DEE_A_RET_EXCEPT(-1) int DeeStringWriter_F(InitCopy)(
 DEE_A_OUT DEE_STRINGWRITER *self, DEE_A_IN DEE_STRINGWRITER const *right) {
 DEE_ASSERT(self && right && self != right);
 if DEE_LIKELY((self->sw_len = right->sw_len) != 0) {
  if DEE_UNLIKELY((self->sw_str = (DEE_STRINGOBJECT *)DeeObject_Malloc(
   Dee_OFFSETOF(DEE_STRINGOBJECT,s_str)+((self->sw_len+1)*sizeof(DEE_CHAR)))) == NULL) return -1;
  DeeObject_INIT(self->sw_str,&DEE_STRING_Type);
  self->sw_str->s_len = self->sw_len;
 } else self->sw_str = NULL;
 return 0;
}
void DeeStringWriter_F(AssignMove)(
 DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_INOUT DEE_STRINGWRITER *right) {
 DEE_ASSERT(self && right && self != right);
 DEE_STRINGWRITER_Quit(self);
 DeeStringWriter_F(InitMove)(self,right);
}
DEE_A_RET_EXCEPT(-1) int DeeStringWriter_F(AssignCopy)(
 DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_IN DEE_STRINGWRITER const *right) {
 DEE_ASSERT(self && right && self != right);
 if DEE_LIKELY(DeeStringWriter_F(SIZE)(right)) {
  return DeeStringWriter_F(AssignWithLength)(self,
   DeeStringWriter_F(SIZE)(right),
   DeeStringWriter_F(ELEM)(right));
 } else self->sw_len = 0;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeStringWriter_F(AssignWithLength)(
 DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_IN Dee_size_t l, DEE_A_IN_R(l) DEE_CHAR const *s) {
 DEE_STRINGOBJECT *new_str;
 DEE_ASSERT(self);
 if DEE_LIKELY(self->sw_str) {
  DEE_ASSERT(DeeObject_Check(self->sw_str) && DEE_STRING_Check(self->sw_str));
  if (l > DEE_STRING_SIZE(self->sw_str)) {
   // Must reallocate string
   if ((new_str = (DEE_STRINGOBJECT *)DeeObject_Realloc(
    self->sw_str,Dee_OFFSETOF(DEE_STRINGOBJECT,s_str)+((l+1)*sizeof(DEE_CHAR)))) == NULL) return -1;
   new_str->s_len = l;
   self->sw_str = new_str;
  }
 } else {
  DEE_ASSERT(!self->sw_len && "No string allocated, but size is set");
  if DEE_LIKELY(l) {
   // Must allocate first string
   if ((new_str = (DEE_STRINGOBJECT *)DeeObject_Malloc(
    Dee_OFFSETOF(DEE_STRINGOBJECT,s_str)+((l+1)*sizeof(DEE_CHAR)))) == NULL) return -1;
   DeeObject_INIT(new_str,&DEE_STRING_Type);
   new_str->s_len = l;
   self->sw_str = new_str;
  }
 }
 if DEE_LIKELY((self->sw_len = l) != 0)
  memcpy(DeeStringWriter_F(ELEM)(self),s,l*sizeof(DEE_CHAR));
 return 0;
}
void DEE_STRINGWRITER_Quit(DEE_A_IN DEE_STRINGWRITER *self) {
 DEE_ASSERT(self);
 if DEE_UNLIKELY(self->sw_str) {
  DEE_ASSERT(DeeObject_Check(self->sw_str) && DEE_STRING_Check(self->sw_str));
  Dee_DECREF(Dee_TYPE(self->sw_str));
  DeeObject_Free(self->sw_str);
 } else DEE_ASSERT(!self->sw_len && "No string allocated, but size is set");
}
void DeeStringWriter_F(Clear)(DEE_A_INOUT DEE_STRINGWRITER *self) {
 DEE_ASSERT(self);
 self->sw_len = 0;
}
void DeeStringWriter_F(ClearAndFlush)(DEE_A_INOUT DEE_STRINGWRITER *self) {
 DEE_ASSERT(self);
 if DEE_LIKELY(self->sw_str) {
  DEE_ASSERT(DeeObject_Check(self->sw_str) && DEE_STRING_Check(self->sw_str));
  Dee_DECREF(Dee_TYPE(self->sw_str));
  DeeObject_Free(self->sw_str);
  self->sw_str = NULL;
  self->sw_len = 0;
 } else {
  DEE_ASSERTF(!self->sw_len,"No string allocated, but size is set");
 }
}
DEE_A_RET_EXCEPT(-1) int DeeStringWriter_F(Flush)(
 DEE_A_INOUT DEE_STRINGWRITER *self) {
 DEE_STRINGOBJECT *new_str;
 DEE_ASSERT(DeeStringWriter_F(SIZE)(self) <= DeeStringWriter_F(ALLOCATED)(self));
 if DEE_UNLIKELY(!self->sw_str) {
  DEE_ASSERTF(!self->sw_len,"No string allocated, but size is set");
  return 0;
 }
 if DEE_LIKELY(DEE_STRING_SIZE(self->sw_str) != self->sw_len) {
  if DEE_LIKELY(self->sw_len) {
   if ((new_str = (DEE_STRINGOBJECT *)DeeObject_Realloc(self->sw_str,
    Dee_OFFSETOF(DEE_STRINGOBJECT,s_str)+((self->sw_len+1)*sizeof(DEE_CHAR)))) == NULL) return -1;
   new_str->s_len = self->sw_len;
   Dee_INHERIT_REF(self->sw_str,new_str);
  } else {
   DEE_ASSERT(DeeObject_Check(Dee_TYPE(self->sw_str)) &&
              DeeType_Check(Dee_TYPE(self->sw_str)));
   DEE_ASSERT(Dee_TYPE(self->sw_str) == &DEE_STRING_Type);
   Dee_DECREF(Dee_TYPE(self->sw_str));
   DeeObject_Free(self->sw_str);
   self->sw_str = NULL;
  }
 }
 return 0;
}
#undef DeeWideStringWriter_FlushAndPack
#undef DeeUtf8StringWriter_FlushAndPack
#undef DeeUtf16StringWriter_FlushAndPack
#undef DeeUtf32StringWriter_FlushAndPack
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeStringWriter_F(FlushAndPack)(DEE_A_INOUT DEE_STRINGWRITER *self) {
 return DEE_STRINGWRITER_Pack(self);
}
DEE_A_RET_EXCEPT(NULL) DEE_A_RET_Z_OPT DEE_CHAR *_DeeStringWriter_F(PackCString)(
 DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_OUT_OPT Dee_size_t *str_size) {
 Dee_size_t size;
 DEE_CHAR *result;
 size = DeeStringWriter_F(SIZE)(self);
 if (str_size) *str_size = size;
 while DEE_UNLIKELY((result = (DEE_CHAR *)malloc_nz(
  (size+1)*sizeof(DEE_CHAR))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 if DEE_LIKELY(size) memcpy(result,DeeStringWriter_F(ELEM)(self),size*sizeof(DEE_CHAR));
 result[size] = DEE_CHAR_C('\0');
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DEE_STRINGWRITER_Pack(DEE_A_INOUT DEE_STRINGWRITER *self) {
 DEE_STRINGOBJECT *result;
 DEE_ASSERT(self);
 if DEE_UNLIKELY(!self->sw_len) DEERETURN_EMPTYSTRING;
#if 0
 if DEE_LIKELY(DEE_LIKELY(self->sw_len != DEE_STRING_SIZE(result))) {
  // We could technically free up some memory by reallocating
  // at this point, but there's no real reason to it...
  if (DEE_UNLIKELY((result = (DEE_STRINGOBJECT *)DeeObject_TryRealloc(
   self->sw_str,Dee_OFFSETOF(DEE_STRINGOBJECT,s_str)+(self->sw_len+1)*sizeof(DEE_CHAR))) == NULL)
   ) result = self->sw_str;
 } else
#endif
 {
  result = self->sw_str;
 }
 DEE_ASSERT(result && "No string allocated, but size is set");
 result->s_len = self->sw_len;
 result->s_str[self->sw_len] = DEE_CHAR_C('\0');
 self->sw_str = NULL;
 self->sw_len = 0;
 return (DeeObject *)result;
}

DEE_A_RET_EXCEPT(-1) int DeeStringWriter_F(IncreaseCapacity)(DEE_A_INOUT DEE_STRINGWRITER *self) {
 Dee_size_t new_size;
 enum{min_size=8};
 DEE_ASSERT(self);
 if DEE_LIKELY(self->sw_str) {
  DEE_STRINGOBJECT *new_str;
  DEE_ASSERT(DeeObject_Check(self->sw_str) && DEE_STRING_Check(self->sw_str));
  new_size = self->sw_str->s_len*2;
  if DEE_UNLIKELY((new_str = (DEE_STRINGOBJECT *)DeeObject_Realloc(
   self->sw_str,Dee_OFFSETOF(DEE_STRINGOBJECT,s_str)+(new_size+1)*sizeof(DEE_CHAR))) == NULL) return -1;
  (self->sw_str = new_str)->s_len = new_size;
 } else {
  DEE_ASSERTF(!self->sw_len,"No string allocated, but size is set");
  if DEE_UNLIKELY((self->sw_str = (DEE_STRINGOBJECT *)DeeObject_Mallocf(
   Dee_OFFSETOF(DEE_STRINGOBJECT,s_str)+(min_size+1)*sizeof(DEE_CHAR),
   "StringWriter: initial capacity string")) == NULL) return -1;
  DeeObject_INIT(self->sw_str,&DEE_STRING_Type);
  self->sw_str->s_len = min_size;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeStringWriter_F(Resize)(
 DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_IN Dee_size_t s) {
 DEE_ASSERT(self);
 if DEE_LIKELY(self->sw_str) {
  DEE_ASSERT(DeeObject_Check(self->sw_str) && DEE_STRING_Check(self->sw_str));
  if (DEE_STRING_SIZE(self->sw_str) >= s) {
   self->sw_len = s; // Enough buffer as it is (just override the size)
  } else { // Must reallocate the buffer
   DEE_STRINGOBJECT *new_str;
   if ((new_str = (DEE_STRINGOBJECT *)DeeObject_Realloc(self->sw_str,
    Dee_OFFSETOF(DEE_STRINGOBJECT,s_str)+(s+1)*sizeof(DEE_CHAR))) == NULL) return -1;
   new_str->s_len = s;
   self->sw_len = s;
   self->sw_str = new_str;
  }
 } else { // Must allocate a new buffer
  DEE_STRINGOBJECT *new_str;
  DEE_ASSERTF(!self->sw_len,"No string allocated, but size is set");
  if DEE_UNLIKELY((new_str = (DEE_STRINGOBJECT *)DeeObject_Malloc(
   Dee_OFFSETOF(DEE_STRINGOBJECT,s_str)+(s+1)*sizeof(DEE_CHAR))) == NULL) return -1;
  DeeObject_INIT(new_str,&DEE_STRING_Type);
  new_str->s_len = s;
  self->sw_len = s;
  self->sw_str = new_str;
 }
 return 0;
}

DEE_A_RET_EXCEPT(NULL) DEE_CHAR *DeeStringWriter_F(Require)(
 DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_IN Dee_size_t s) {
 DEE_CHAR *result;
 Dee_size_t min_len;
 DEE_ASSERT(self);
 min_len = self->sw_len+s;
 if DEE_UNLIKELY(DEE_UNLIKELY(!self->sw_str && DeeStringWriter_F(IncreaseCapacity)(self) == -1)) return NULL;
 while (DEE_STRING_SIZE(self->sw_str) < min_len)
  if DEE_UNLIKELY(DeeStringWriter_F(IncreaseCapacity)(self) == -1) return NULL;
 result = DeeStringWriter_F(ELEM)(self)+self->sw_len;
 self->sw_len = min_len;
 return result;
}

#ifndef __INTELLISENSE__
#define DST_ENCODING ENC
#define SRC_ENCODING Wide
#include "string.cast.unicode.inl"
#include "string.cast.escapecode.inl"
#undef SRC_ENCODING
#define SRC_ENCODING Utf8
#include "string.cast.unicode.inl"
#include "string.cast.escapecode.inl"
#undef SRC_ENCODING
#define SRC_ENCODING Utf16
#include "string.cast.unicode.inl"
#include "string.cast.escapecode.inl"
#undef SRC_ENCODING
#define SRC_ENCODING Utf32
#include "string.cast.unicode.inl"
#include "string.cast.escapecode.inl"
#undef SRC_ENCODING
#undef DST_ENCODING
#endif

DEE_A_RET_EXCEPT(-1) int DeeStringWriter_F(RepeatChar)(
 DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_IN DEE_CHAR c, DEE_A_IN Dee_size_t n) {
 DEE_CHAR *buffer;
 if DEE_UNLIKELY(!n) return 0;
 if DEE_UNLIKELY((buffer = DeeStringWriter_F(Require)(self,n)) == NULL) return -1;
 while DEE_UNLIKELY(n--) *buffer++ = c;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeStringWriter_F(CopyFromFile)(
 DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_INOUT_OBJECT(DeeFileObject) *fp, DEE_A_IN Dee_size_t s) {
 DEE_CHAR *buffer;
 if DEE_UNLIKELY(!s) return 0;
 if DEE_UNLIKELY((buffer = DeeStringWriter_F(Require)(self,s)) == NULL) return -1;
 return DeeFile_ReadAll(fp,buffer,s);
}
DEE_A_RET_EXCEPT(-1) int DeeStringWriter_F(WriteObjectStr)(
 DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_INOUT DeeObject *ob) {
 int result;
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(ob)) return DeeStringWriter_F(WriteWideStringWithLength)(self,DeeWideString_SIZE(ob),DeeWideString_STR(ob));
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(ob)) return DeeStringWriter_F(WriteUtf8StringWithLength)(self,DeeUtf8String_SIZE(ob),DeeUtf8String_STR(ob));
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(ob)) return DeeStringWriter_F(WriteUtf16StringWithLength)(self,DeeUtf16String_SIZE(ob),DeeUtf16String_STR(ob));
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(ob)) return DeeStringWriter_F(WriteUtf32StringWithLength)(self,DeeUtf32String_SIZE(ob),DeeUtf32String_STR(ob));
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 if DEE_UNLIKELY((ob = DeeObject_Str(ob)) == NULL) return -1;
 result = DeeStringWriter_F(WriteStringWithLength)(self,DeeString_SIZE(ob),DeeString_STR(ob));
 Dee_DECREF(ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeStringWriter_F(WriteObjectRepr)(
 DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_INOUT DeeObject *ob) {
 int result;
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(ob)) return DeeStringWriter_F(WriteQuotedWideStringWithLength)(self,DeeWideString_SIZE(ob),DeeWideString_STR(ob));
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(ob)) return DeeStringWriter_F(WriteQuotedUtf8StringWithLength)(self,DeeUtf8String_SIZE(ob),DeeUtf8String_STR(ob));
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(ob)) return DeeStringWriter_F(WriteQuotedUtf16StringWithLength)(self,DeeUtf16String_SIZE(ob),DeeUtf16String_STR(ob));
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(ob)) return DeeStringWriter_F(WriteQuotedUtf32StringWithLength)(self,DeeUtf32String_SIZE(ob),DeeUtf32String_STR(ob));
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 if DEE_UNLIKELY((ob = DeeObject_Repr(ob)) == NULL) return -1;
 result = DeeStringWriter_F(WriteStringWithLength)(self,DeeString_SIZE(ob),DeeString_STR(ob));
 Dee_DECREF(ob);
 return result;
}


DEE_A_RET_EXCEPT(-1) int DeeStringWriter_F(Writef)(
 DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_IN_PRINTF DEE_CHAR const *fmt, ...) {
 int result;
 va_list args;
 DEE_VA_START(args,fmt);
 result = DeeStringWriter_F(VWritef)(self,fmt,args);
 DEE_VA_END(args);
 return result;
}

DEE_COMPILER_MSVC_WARNING_PUSH(4061)
DEE_A_RET_EXCEPT(-1) int DeeStringWriter_F(VWritef)(
 DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_IN_PRINTF DEE_CHAR const *fmt, DEE_A_IN va_list args) {
 DeeStringWriterFormatSpec spec;
 DEE_CHAR ch;
 DEE_CHAR const *flush_start = fmt;
 enum{
  len_none,len_hh,len_h,len_l,len_ll,len_j,len_z,
  len_t,len_L,len_I,len_I8,len_I16,len_I32,len_I64
 }length;
 DeeObject *ob,*ob2;
 int result = 0;
 DEE_ASSERT(self);
 while ((ch = *fmt++) != 0) {
  if (ch == DEE_CHAR_C('%')) {
   if (DeeStringWriter_F(DEE_PP_CAT_3(Write,ENC,StringWithLength))(
    self,(Dee_size_t)((fmt-flush_start)-1),flush_start) == -1) result = -1;
   ch = *fmt++;
   memset(&spec,0,sizeof(spec));
   length = len_none;
p_flag: // flags
   switch (ch) {
    case DEE_CHAR_C('-'): spec.left_just = 1; ch = *fmt++; goto p_flag;
    case DEE_CHAR_C('+'): spec.sign_pos = 1; ch = *fmt++; goto p_flag;
    case DEE_CHAR_C(' '): spec.sign_space = 1; ch = *fmt++; goto p_flag;
    case DEE_CHAR_C('#'): spec.prefix = 1; ch = *fmt++; goto p_flag;
    case DEE_CHAR_C('0'): spec.pad_zero = 1; ch = *fmt++; goto p_flag;
    case DEE_CHAR_C('>'): ++spec.instab; ch = *fmt++; goto p_flag;
    default: break;
   }
   // width
   switch (ch) {
    case DEE_CHAR_C('1'): case DEE_CHAR_C('2'): case DEE_CHAR_C('3'):
    case DEE_CHAR_C('4'): case DEE_CHAR_C('5'): case DEE_CHAR_C('6'):
    case DEE_CHAR_C('7'): case DEE_CHAR_C('8'): case DEE_CHAR_C('9'):
     spec.has_width = 1;
     spec.width = (unsigned int)(ch-DEE_CHAR_C('0'));
     while (1) {
      ch = *fmt++;
      if (ch < DEE_CHAR_C('0') || ch > DEE_CHAR_C('9')) break;
      spec.width *= 10;
      spec.width += (ch-DEE_CHAR_C('0'));
     }
     break;
    case DEE_CHAR_C('*'):
     spec.has_width = 1;
     spec.width = (unsigned int)DEE_VA_ARG(args,int);
     ch = *fmt++;
     break;
   }
   // Precision
   if (ch == DEE_CHAR_C('.')) {
    ch = *fmt++;
    spec.has_precision = 1;
    switch (ch) {
     case DEE_CHAR_C('1'): case DEE_CHAR_C('2'): case DEE_CHAR_C('3'):
     case DEE_CHAR_C('4'): case DEE_CHAR_C('5'): case DEE_CHAR_C('6'):
     case DEE_CHAR_C('7'): case DEE_CHAR_C('8'): case DEE_CHAR_C('9'):
      spec.has_precision = 1;
      spec.precision = (unsigned int)(ch-DEE_CHAR_C('0'));
      while (1) {
       ch = *fmt++;
       if (ch < DEE_CHAR_C('0') || ch > DEE_CHAR_C('9')) break;
       spec.precision *= 10;
       spec.precision += (ch-DEE_CHAR_C('0'));
      }
      break;
     case DEE_CHAR_C('*'):
      spec.has_precision = 1;
      spec.precision = (unsigned int)DEE_VA_ARG(args,int);
      ch = *fmt++;
      break;
     default: --fmt; ch = DEE_CHAR_C('.'); spec.has_precision = 0; break;
    }
   }
   // Length specifier
   switch (ch) {
    case DEE_CHAR_C('h'): ch = *fmt++; if (ch == DEE_CHAR_C('h')) { ch = *fmt++; length = len_hh; } else length = len_h; break;
    case DEE_CHAR_C('l'): ch = *fmt++; if (ch == DEE_CHAR_C('l')) { ch = *fmt++; length = len_ll; } else length = len_l; break;
    case DEE_CHAR_C('j'): ch = *fmt++; length = len_j; break;
    case DEE_CHAR_C('z'): ch = *fmt++; length = len_z; break;
    case DEE_CHAR_C('t'): ch = *fmt++; length = len_t; break;
    case DEE_CHAR_C('L'): ch = *fmt++; length = len_L; break;
    case DEE_CHAR_C('I'): switch ((ch = *fmt++)) {
     case DEE_CHAR_C('8'): ch = *fmt++; length = len_I8; break;
     case DEE_CHAR_C('1'): if ((ch = *fmt++) == DEE_CHAR_C('6')) { ch = *fmt++; length = len_I16; } else { ch = *--fmt; goto def_lI; } break;
     case DEE_CHAR_C('3'): if ((ch = *fmt++) == DEE_CHAR_C('2')) { ch = *fmt++; length = len_I32; } else { ch = *--fmt; goto def_lI; } break;
     case DEE_CHAR_C('6'): if ((ch = *fmt++) == DEE_CHAR_C('4')) { ch = *fmt++; length = len_I64; } else { ch = *--fmt; goto def_lI; } break;
     default: def_lI: length = len_I; break;
    } break;
    default: break;
   }
   // format specifier
   switch (ch) {
    case DEE_CHAR_C('%'):
     if (DeeStringWriter_F(DEE_PP_CAT_3(Write,ENC,Char))(self,DEE_CHAR_C('%')) == -1) result = -1;
     break;
    case DEE_CHAR_C('d'):
    case DEE_CHAR_C('i'):
     spec.numsys = 10;
     switch (length) {
#define LABEL_I(sizeof)  DEE_PP_CAT_2(i,DEE_PP_MUL8(sizeof))
#define LABEL_UI(sizeof) DEE_PP_CAT_2(ui,DEE_PP_MUL8(sizeof))
      case len_I8:  i8:  if (DeeStringWriter_F(SpecWriteInt8)(self,(Dee_int8_t)DEE_VA_ARG(args,DEE_MININT_T(1)),&spec) == -1) result = -1; break;
      case len_I16: i16: if (DeeStringWriter_F(SpecWriteInt16)(self,(Dee_int16_t)DEE_VA_ARG(args,DEE_MININT_T(2)),&spec) == -1) result = -1; break;
      case len_I32: i32: if (DeeStringWriter_F(SpecWriteInt32)(self,(Dee_int32_t)DEE_VA_ARG(args,DEE_MININT_T(4)),&spec) == -1) result = -1; break;
      case len_I64: i64: if (DeeStringWriter_F(SpecWriteInt64)(self,(Dee_int64_t)DEE_VA_ARG(args,DEE_MININT_T(8)),&spec) == -1) result = -1; break;
      case len_hh: goto LABEL_I(DEE_TYPES_SIZEOF_CHAR);
      case len_h: goto LABEL_I(DEE_TYPES_SIZEOF_SHORT);
      case len_l: goto LABEL_I(DEE_TYPES_SIZEOF_LONG);
#if DEE_COMPILER_HAVE_LONG_LONG
      case len_ll: goto LABEL_I(DEE_TYPES_SIZEOF_LLONG);
#endif /* DEE_COMPILER_HAVE_LONG_LONG */
      case len_j: goto LABEL_I(DEE_TYPES_SIZEOF_INTMAX_T);
      case len_z: goto LABEL_UI(DEE_TYPES_SIZEOF_SIZE_T);
      case len_I: goto LABEL_I(DEE_TYPES_SIZEOF_SIZE_T);
      case len_t: goto LABEL_I(DEE_TYPES_SIZEOF_PTRDIFF_T);
      default: goto LABEL_I(DEE_TYPES_SIZEOF_INT);
     }
     break;
    case DEE_CHAR_C('b'): spec.numsys = 2; goto wu;
    case DEE_CHAR_C('o'): spec.numsys = 8; goto wu;
    case DEE_CHAR_C('X'): spec.upper_hex = 1; DEE_ATTRIBUTE_FALLTHROUGH
    case DEE_CHAR_C('x'): spec.numsys = 16; goto wu;
    case DEE_CHAR_C('u'): spec.numsys = 10;
wu:  switch (length) {
      case len_I8:  ui8:  if (DeeStringWriter_F(SpecWriteUInt8)(self,(Dee_uint8_t)DEE_VA_ARG(args,DEE_MINUINT_T(1)),&spec) == -1) result = -1; break;
      case len_I16: ui16: if (DeeStringWriter_F(SpecWriteUInt16)(self,(Dee_uint16_t)DEE_VA_ARG(args,DEE_MINUINT_T(2)),&spec) == -1) result = -1; break;
      case len_I32: ui32: if (DeeStringWriter_F(SpecWriteUInt32)(self,(Dee_uint32_t)DEE_VA_ARG(args,DEE_MINUINT_T(4)),&spec) == -1) result = -1; break;
      case len_I64: ui64: if (DeeStringWriter_F(SpecWriteUInt64)(self,(Dee_uint64_t)DEE_VA_ARG(args,DEE_MINUINT_T(8)),&spec) == -1) result = -1; break;
      case len_hh: goto LABEL_UI(DEE_TYPES_SIZEOF_CHAR);
      case len_h:  goto LABEL_UI(DEE_TYPES_SIZEOF_SHORT);
      case len_l:  goto LABEL_UI(DEE_TYPES_SIZEOF_LONG);
#if DEE_COMPILER_HAVE_LONG_LONG
      case len_ll: goto LABEL_UI(DEE_TYPES_SIZEOF_LLONG);
#endif /* DEE_COMPILER_HAVE_LONG_LONG */
      case len_j: goto LABEL_UI(DEE_TYPES_SIZEOF_INTMAX_T);
      case len_z: goto LABEL_UI(DEE_TYPES_SIZEOF_SIZE_T);
      case len_I: goto LABEL_UI(DEE_TYPES_SIZEOF_SIZE_T);
      case len_t: goto LABEL_I(DEE_TYPES_SIZEOF_SIZE_T);
      default: goto LABEL_UI(DEE_TYPES_SIZEOF_INT);
     }
     break;
    case DEE_CHAR_C('f'): case DEE_CHAR_C('F'):
    case DEE_CHAR_C('e'): case DEE_CHAR_C('E'):
    case DEE_CHAR_C('g'): case DEE_CHAR_C('G'):
    case DEE_CHAR_C('a'): case DEE_CHAR_C('A'):
     if (length == len_L) {
      if (DeeStringWriter_F(SpecWriteLDouble)(self,DEE_VA_ARG(args,long double),&spec) == -1) result = -1;
     } else {
      if (DeeStringWriter_F(SpecWriteDouble)(self,DEE_VA_ARG(args,double),&spec) == -1) result = -1;
     }
     break;
    case DEE_CHAR_C('c'): switch (length) {
     case len_l:   if (DeeStringWriter_F(SpecWriteWideChar)(self,(Dee_WideChar)DEE_VA_ARG(args,DEE_MINUINT_T(DEE_TYPES_SIZEOF_WCHAR_T)),&spec) != 0) result = -1; break;
     case len_I8:  if (DeeStringWriter_F(SpecWriteUtf8Char)(self,(Dee_Utf8Char)DEE_VA_ARG(args,DEE_MINUINT_T(1)),&spec) != 0) result = -1; break;
     case len_I16: if (DeeStringWriter_F(SpecWriteUtf16Char)(self,(Dee_Utf16Char)DEE_VA_ARG(args,DEE_MINUINT_T(2)),&spec) != 0) result = -1; break;
     case len_I32: if (DeeStringWriter_F(SpecWriteUtf32Char)(self,(Dee_Utf32Char)DEE_VA_ARG(args,DEE_MINUINT_T(4)),&spec) != 0) result = -1; break;
     default:      if (DeeStringWriter_F(SpecWriteChar)(self,(char)DEE_VA_ARG(args,DEE_MINUINT_T(DEE_TYPES_SIZEOF_CHAR)),&spec) != 0) result = -1; break;
    } break;
    case DEE_CHAR_C('s'):
    case DEE_CHAR_C('q'): {
     union{
      char const          *str_;
      Dee_WideChar const  *strw;
      Dee_Utf8Char const  *str8;
      Dee_Utf16Char const *str16;
      Dee_Utf32Char const *str32;
     } str_;
     str_.str_ = DEE_VA_ARG(args,char const *);
#define DEE_STRING_NULL_STR  {'(','n','u','l','l',')',0}
     switch (length) {
      case len_l: {
       if (!str_.strw) { static Dee_WideChar const null_strw[] = DEE_STRING_NULL_STR; str_.strw = null_strw; }
       if ((ch != DEE_CHAR_C('q') ? DeeStringWriter_F(SpecWriteWideStringWithLength)(self,spec.has_precision ? Dee_WideStrNLen(str_.strw,spec.precision) : Dee_WideStrLen(str_.strw),str_.strw,&spec)
                            : DeeStringWriter_F(SpecWriteQuotedWideStringWithLength)(self,spec.has_precision ? Dee_WideStrNLen(str_.strw,spec.precision) : Dee_WideStrLen(str_.strw),str_.strw,&spec)
          ) != 0) result = -1;
      } break;
      case len_I8: {
       if (!str_.str8) { static Dee_Utf8Char const null_str8[] = DEE_STRING_NULL_STR; str_.str8 = null_str8; }
       if ((ch != DEE_CHAR_C('q') ? DeeStringWriter_F(SpecWriteUtf8StringWithLength)(self,spec.has_precision ? Dee_Utf8StrNLen(str_.str8,spec.precision) : Dee_Utf8StrLen(str_.str8),str_.str8,&spec)
                            : DeeStringWriter_F(SpecWriteQuotedUtf8StringWithLength)(self,spec.has_precision ? Dee_Utf8StrNLen(str_.str8,spec.precision) : Dee_Utf8StrLen(str_.str8),str_.str8,&spec)
          ) != 0) result = -1;
      } break;
      case len_I16: {
       if (!str_.str16) { static Dee_Utf16Char const null_str16[] = DEE_STRING_NULL_STR; str_.str16 = null_str16; }
       if ((ch != DEE_CHAR_C('q') ? DeeStringWriter_F(SpecWriteUtf16StringWithLength)(self,spec.has_precision ? Dee_Utf16StrNLen(str_.str16,spec.precision) : Dee_Utf16StrLen(str_.str16),str_.str16,&spec)
                            : DeeStringWriter_F(SpecWriteQuotedUtf16StringWithLength)(self,spec.has_precision ? Dee_Utf16StrNLen(str_.str16,spec.precision) : Dee_Utf16StrLen(str_.str16),str_.str16,&spec)
          ) != 0) result = -1;
      } break;
      case len_I32: {
       if (!str_.str32) { static Dee_Utf32Char const null_str32[] = DEE_STRING_NULL_STR; str_.str32 = null_str32; }
       if ((ch != DEE_CHAR_C('q') ? DeeStringWriter_F(SpecWriteUtf32StringWithLength)(self,spec.has_precision ? Dee_Utf32StrNLen(str_.str32,spec.precision) : Dee_Utf32StrLen(str_.str32),str_.str32,&spec)
                            : DeeStringWriter_F(SpecWriteQuotedUtf32StringWithLength)(self,spec.has_precision ? Dee_Utf32StrNLen(str_.str32,spec.precision) : Dee_Utf32StrLen(str_.str32),str_.str32,&spec)
          ) != 0) result = -1;
      } break;
      default: {
       if (!str_.str_) { static char const null_str[] = DEE_STRING_NULL_STR; str_.str_ = null_str; }
       if ((ch != DEE_CHAR_C('q') ? DeeStringWriter_F(SpecWriteStringWithLength)(self,spec.has_precision ? Dee_StrNLen(str_.str_,spec.precision) : Dee_StrLen(str_.str_),str_.str_,&spec)
                            : DeeStringWriter_F(SpecWriteQuotedStringWithLength)(self,spec.has_precision ? Dee_StrNLen(str_.str_,spec.precision) : Dee_StrLen(str_.str_),str_.str_,&spec)
          ) != 0) result = -1;
      } break;
     }
#undef DEE_STRING_NULL_STR
    } break;
    case DEE_CHAR_C('p'):
     spec.numsys = 16;
     spec.has_precision = 1;
     if (spec.precision < DEE_TYPES_SIZEOF_POINTER*2)
      spec.precision = DEE_TYPES_SIZEOF_POINTER*2;
     goto LABEL_UI(DEE_TYPES_SIZEOF_POINTER);
    case DEE_CHAR_C('O'):
    case DEE_CHAR_C('k'): case DEE_CHAR_C('r'):
    case DEE_CHAR_C('K'): case DEE_CHAR_C('R'):
     if ((ob = DEE_VA_ARG(args,DeeObject *)) == NULL) {
      ob = Dee_None;
      if (ch == DEE_CHAR_C('K') || ch == DEE_CHAR_C('R')) Dee_INCREF(Dee_None);
      if (ch != DEE_CHAR_C('O')) result = -1;
     }
     ob2 = (ch == DEE_CHAR_C('r') || ch == DEE_CHAR_C('R')) ? DeeObject_Repr(ob) : DeeObject_Str(ob);
     if (ch == DEE_CHAR_C('K') || ch == DEE_CHAR_C('R')) Dee_DECREF(ob);
     if (ob2) {
      if (DeeStringWriter_F(DEE_PP_CAT_3(SpecWrite,DEE_CONFIG_DEFAULT_ENCODING,StringWithLength))(
       self,DeeString_SIZE(ob2),DeeString_STR(ob2),&spec) == -1) result = -1;
      Dee_DECREF(ob2);
     } else result = -1;
     break;
    default: break;
   }
#undef LABEL_UI
#undef LABEL_I
//end_fmt:
   flush_start = fmt;
  }
 }
 // Flush remainder
 if (DeeStringWriter_F(DEE_PP_CAT_3(Write,ENC,StringWithLength))(
  self,(Dee_size_t)((fmt-flush_start)-1),flush_start) == -1) result = -1;
 return result;
}
DEE_COMPILER_MSVC_WARNING_POP




#ifndef DEE_PRIVATE_HEX_BYTE_SPEC_DEFINED
#define DEE_PRIVATE_HEX_BYTE_SPEC_DEFINED 1
static DeeStringWriterFormatSpec const hex_byte_spec = {
 16, // numsys
 0,  // width
 2,  // precision
 0,  // has_width
 1,  // has_precision
 0,  // left_just
 0,  // sign_pos
 0,  // sign_space
 1,  // prefix
 0,  // pad_zero
 0,  // upper_hex
 0,  // instab
 0,  // padding
};
#endif

DEE_A_RET_EXCEPT(-1) int DeeStringWriter_F(WriteHexDump)(
 DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s) {
 Dee_uint8_t line[16];
 Dee_size_t i,line_size;
 DEE_CHAR *ascii_mem;
 DEE_ASSERT(p);
 while (1) {
  if (s >= sizeof(line))
   memcpy(line,p,line_size = sizeof(line));
  else memcpy(line,p,line_size = s);
  for (i = 0; i < line_size; ++i) {
   if (DeeStringWriter_F(SpecWriteUInt8)(self,line[i],&hex_byte_spec) == -1 ||
       DeeStringWriter_F(DEE_PP_CAT_3(Write,ENC,Char))(self,DEE_CHAR_C(' ')) == -1) return -1;
  }
  // Fill unused space
  if (DeeStringWriter_F(RepeatChar)(self,DEE_CHAR_C(' '),5*(sizeof(line)-line_size)) == -1) return -1;
  if ((ascii_mem = DeeStringWriter_F(Require)(self,sizeof(line)+1)) == NULL) return -1;
  for (i = 0; i < line_size; ++i) *ascii_mem++ = (DEE_CHAR)(
   DEE_CH_IS_GRAPH(line[i]) ? (DEE_CHAR)line[i] : DEE_CHAR_C('.'));
  line_size = (sizeof(line)-line_size);
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6305)
#endif
#if DEE_STRING_ENCODING_SIZE(ENC) == 1
  memset(ascii_mem,' ',line_size*sizeof(DEE_CHAR));
#else
 {DEE_CHAR *dst,*end;
  end = (dst = ascii_mem)+line_size;
  while (dst != end) *dst++ = DEE_CHAR_C(' ');
 }
#endif
  ascii_mem[line_size] = DEE_CHAR_C('\n');
#ifdef _PREFAST_
#pragma warning(pop)
#endif
  if (s <= sizeof(line)) break;
  s -= sizeof(line);
  *((DEE_CHAR const **)&p) += sizeof(line);
 }
 return 0;
}

#ifndef __INTELLISENSE__
#define UNSIGNED 1
#define BITS 8
#include "string.impl.itos.inl"
#define BITS 16
#include "string.impl.itos.inl"
#define BITS 32
#include "string.impl.itos.inl"
#define BITS 64
#include "string.impl.itos.inl"
#undef UNSIGNED
#define UNSIGNED 0
#define BITS 8
#include "string.impl.itos.inl"
#define BITS 16
#include "string.impl.itos.inl"
#define BITS 32
#include "string.impl.itos.inl"
#define BITS 64
#include "string.impl.itos.inl"
#undef UNSIGNED
#endif

DEE_A_RET_EXCEPT(-1) int DeeStringWriter_F(SpecWriteFloat)(
 DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_IN float v,
 DEE_A_IN DeeStringWriterFormatSpec const *spec) {
 char buffer[64];
 DEE_ASSERT(self),DEE_ASSERT(spec);
 sprintf(buffer,"%f",v); // TODO!
 return DeeStringWriter_F(SpecWriteUtf8String)(self,buffer,spec);
}
DEE_A_RET_EXCEPT(-1) int DeeStringWriter_F(SpecWriteDouble)(
 DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_IN double v,
 DEE_A_IN DeeStringWriterFormatSpec const *spec) {
 char buffer[64];
 DEE_ASSERT(self),DEE_ASSERT(spec);
 sprintf(buffer,"%f",v); // TODO!
 return DeeStringWriter_F(SpecWriteUtf8String)(self,buffer,spec);
}
DEE_A_RET_EXCEPT(-1) int DeeStringWriter_F(SpecWriteLDouble)(
 DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_IN long double v,
 DEE_A_IN DeeStringWriterFormatSpec const *spec) {
 char buffer[64];
 DEE_ASSERT(self),DEE_ASSERT(spec);
 sprintf(buffer,"%Lf",v); // TODO!
 return DeeStringWriter_F(SpecWriteUtf8String)(self,buffer,spec);
}
DEE_A_RET_EXCEPT(-1) int DeeStringWriter_F(SpecWriteObject)(
 DEE_A_INOUT DEE_STRINGWRITER *self, DEE_A_INOUT DeeObject *ob,
 DEE_A_IN DeeStringWriterFormatSpec const *spec) {
 DeeObject *sw_str; int result;
 DEE_ASSERT(self),DEE_ASSERT(spec);
 DEE_ASSERT(DeeObject_Check(ob));
 if DEE_UNLIKELY((sw_str = DeeObject_Str(ob)) == NULL) return -1;
 result = DeeStringWriter_F(DEE_PP_CAT_3(SpecWrite,DEE_CONFIG_DEFAULT_ENCODING,
  StringWithLength))(self,DeeString_SIZE(sw_str),DeeString_STR(sw_str),spec);
 Dee_DECREF(sw_str);
 return result;
}








//////////////////////////////////////////////////////////////////////////
// String Utility functions

DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(ConcatWithLength)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *lhs,
 DEE_A_IN Dee_size_t rhs_size, DEE_A_IN_R(rhs_size) DEE_CHAR const *rhs) {
 DeeObject *result; Dee_size_t lhs_size; DEE_CHAR *buf;
 DEE_ASSERT(DeeObject_Check(lhs) && DEE_STRING_Check(lhs));
 DEE_ASSERT(!rhs_size || rhs);
 lhs_size = DEE_STRING_SIZE(lhs);
 if DEE_LIKELY((result = DeeString_F(NewSized)(lhs_size+rhs_size)) != NULL) {
  buf = DEE_STRING_STR(result);
  memcpy(buf,DEE_STRING_STR(lhs),lhs_size*sizeof(DEE_CHAR));
  memcpy(buf+lhs_size,rhs,rhs_size*sizeof(DEE_CHAR));
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(Repeat)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self, DEE_A_IN Dee_size_t n) {
 DeeObject *result; Dee_size_t i,self_size; DEE_CHAR *buf,*src;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 self_size = DEE_STRING_SIZE(self);
 if DEE_UNLIKELY(!self_size || !n) DEERETURN_EMPTYSTRING;
 if DEE_LIKELY((result = DeeString_F(NewSized)(self_size*n)) != NULL) {
  src = DEE_STRING_STR(self);
  buf = DEE_STRING_STR(result);
  for (i = 0; i < n; ++i) {
   memcpy(buf,src,self_size);
   buf += self_size;
  }
 }
 return result;
}

static DeeObject *DeeString_F(join_vector)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sep,
 DEE_A_IN Dee_size_t s, DEE_A_IN_R(s) DeeObject **v) {
 DeeObject *result;
 DEE_STRINGWRITER writer = DEE_STRINGWRITER_INIT();
 if DEE_LIKELY(s) {
  if DEE_UNLIKELY(DeeStringWriter_F(WriteObjectStr)(&writer,*v) != 0) goto err;
  while DEE_LIKELY(--s) { ++v;
   if DEE_UNLIKELY(DeeStringWriter_F(DEE_PP_CAT_3(Write,ENC,StringWithLength))(
    &writer,DEE_STRING_SIZE(sep),DEE_STRING_STR(sep)) != 0 ||
    DeeStringWriter_F(WriteObjectStr)(&writer,*v) != 0) goto err;
  }
 }
 result = DEE_STRINGWRITER_Pack(&writer);
end: DEE_STRINGWRITER_Quit(&writer);
 return result;
err: result = NULL; goto end;
}
static DeeObject *DeeString_F(join_list)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sep,
 DEE_A_IN_OBJECT(DeeListObject) *lst) {
 DeeObject *result,*elem; Dee_size_t i;
 DEE_STRINGWRITER writer = DEE_STRINGWRITER_INIT();
 DeeList_ACQUIRE(lst);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(lst)); ++i) {
  Dee_INCREF(elem = DeeList_GET(lst,i));
  DeeList_RELEASE(lst);
  if DEE_UNLIKELY(DEE_UNLIKELY(DEE_UNLIKELY(i) && DeeStringWriter_F(DEE_PP_CAT_3(
   Write,ENC,StringWithLength))(&writer,DEE_STRING_SIZE(sep),DEE_STRING_STR(sep)) != 0) ||
   DEE_UNLIKELY(DeeStringWriter_F(WriteObjectStr)(&writer,elem) != 0)) { Dee_DECREF(elem); goto err; }
  Dee_DECREF(elem);
  DeeList_ACQUIRE(lst);
 }
 DeeList_RELEASE(lst);
 result = DEE_STRINGWRITER_Pack(&writer);
end: DEE_STRINGWRITER_Quit(&writer);
 return result;
err: result = NULL; goto end;
}
static DeeObject *DeeString_F(join_iterator)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sep, DEE_A_INOUT DeeObject *iter) {
 DeeObject *result,*elem; int temp;
 DEE_STRINGWRITER writer = DEE_STRINGWRITER_INIT();
 if DEE_LIKELY((temp = DeeObject_IterNextEx(iter,&elem)) == 0) {
  temp = DeeStringWriter_F(WriteObjectStr)(&writer,elem);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(temp != 0) goto err;
  while DEE_LIKELY((temp = DeeObject_IterNextEx(iter,&elem)) == 0) {
   temp = DeeStringWriter_F(DEE_PP_CAT_3(Write,ENC,StringWithLength))(
    &writer,DEE_STRING_SIZE(sep),DEE_STRING_STR(sep));
   if DEE_LIKELY(temp == 0) temp = DeeStringWriter_F(WriteObjectStr)(&writer,elem);
   Dee_DECREF(elem);
   if DEE_UNLIKELY(temp != 0) goto err;
  }
 }
 if DEE_UNLIKELY(temp < 0) goto err;
 result = DEE_STRINGWRITER_Pack(&writer);
end: DEE_STRINGWRITER_Quit(&writer);
 return result;
err: result = NULL; goto end;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(TJoin)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sep,
 DEE_A_IN DeeTypeObject const *tp_sequence, DEE_A_INOUT DeeObject *sequence) {
 DeeObject *iterator,*result;
 DEE_ASSERT(DeeObject_Check(sep) && DEE_STRING_Check(sep));
 DEE_ASSERT(DeeObject_Check(sequence));
 DEE_ASSERT(DeeObject_Check(tp_sequence) && DeeType_Check(tp_sequence));
 DEE_ASSERT(DeeObject_InstanceOf(sequence,tp_sequence));
 if (tp_sequence == &DeeTuple_Type) return DeeString_F(join_vector)(sep,DeeTuple_SIZE(sequence),DeeTuple_ELEM(sequence));
 if (tp_sequence == &DeeList_Type) return DeeString_F(join_list)(sep,sequence);
 if (tp_sequence == &DeeCell_Type) {
  if ((sequence = DeeCell_TryGet(sequence)) == NULL) DEERETURN_EMPTYSTRING;
  result = DEE_PP_CAT_3(DeeObject_T,ENC,Str)(tp_sequence,sequence);
  Dee_DECREF(sequence);
  return result;
 }
 if (tp_sequence == &DeeSuper_Type) return DeeString_F(TJoin)(sep,DeeSuper_TYPE(sequence),DeeSuper_SELF(sequence));
 if ((iterator = DeeObject_TIterSelf(tp_sequence,sequence)) == NULL) return NULL;
 result = DeeString_F(join_iterator)(sep,iterator);
 Dee_DECREF(iterator);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeString_F(Split)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self, 
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sep) {
 DeeObject *result,*elem; Dee_size_t self_size,sep_size;
 DEE_CHAR const *elem_begin,*begin,*end,*sep_begin;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 DEE_ASSERT(DeeObject_Check(sep) && DEE_STRING_Check(sep));
 self_size = DEE_STRING_SIZE(self),sep_size = DEE_STRING_SIZE(sep);
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) return NULL;
 if DEE_LIKELY(sep_size && sep_size <= self_size) {
  sep_begin = DEE_STRING_STR(sep);
  end = (begin = DEE_STRING_STR(self))+(self_size-sep_size);
  elem_begin = begin;
  while (1) {
   if DEE_UNLIKELY(memcmp(begin,sep_begin,sep_size*sizeof(DEE_CHAR)) == 0) { // Found one
    if DEE_UNLIKELY((elem = DeeString_F(NewWithLength)((Dee_size_t)(begin-elem_begin),elem_begin)) == NULL) goto err;
    if DEE_UNLIKELY(DeeList_Append(result,elem) != 0) {err_elem: Dee_DECREF(elem); goto err; }
    Dee_DECREF(elem);
    elem_begin = (begin += sep_size);
    if DEE_UNLIKELY(begin > end) break;
   } else if DEE_UNLIKELY(begin++ == end) break;
  }
  if DEE_UNLIKELY((elem = DeeString_F(NewWithLength)((Dee_size_t)(begin-elem_begin),elem_begin)) == NULL) goto err;
  if DEE_UNLIKELY(DeeList_Append(result,elem) != 0) goto err_elem;
  Dee_DECREF(elem);
 } else {
  if DEE_UNLIKELY(DeeList_Append(result,(DeeObject *)self) != 0) goto err;
 }
 return result;
err:
 Dee_DECREF(result);
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *DeeString_F(RSplit)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sep) {
 DeeObject *result,*elem; Dee_size_t self_size,sep_size;
 DEE_CHAR const *elem_end,*begin,*end,*sep_begin;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 DEE_ASSERT(DeeObject_Check(sep) && DEE_STRING_Check(sep));
 self_size = DEE_STRING_SIZE(self),sep_size = DEE_STRING_SIZE(sep);
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) return NULL;
 if DEE_LIKELY(sep_size && sep_size <= self_size) {
  sep_begin = DEE_STRING_STR(sep);
  elem_end = (begin = DEE_STRING_STR(self))+self_size;
  end = elem_end-sep_size;
  while (1) {
   if DEE_UNLIKELY(memcmp(end,sep_begin,sep_size*sizeof(DEE_CHAR)) == 0) { // Found one
    if DEE_UNLIKELY((elem = DeeString_F(NewWithLength)((Dee_size_t)(elem_end-(end+sep_size)),end+sep_size)) == NULL) goto err;
    if DEE_UNLIKELY(DeeList_Append(result,elem) != 0) {err_elem: Dee_DECREF(elem); goto err; }
    Dee_DECREF(elem);
    elem_end = end;
    if DEE_UNLIKELY((end -= sep_size) < begin) break;
   } else if DEE_UNLIKELY(end-- == begin) break;
  }
  if DEE_UNLIKELY((elem = DeeString_F(NewWithLength)((Dee_size_t)(elem_end-(end+sep_size)),end+sep_size)) == NULL) goto err;
  if DEE_UNLIKELY(DeeList_Append(result,elem) != 0) goto err_elem;
  Dee_DECREF(elem);
 } else {
  if DEE_UNLIKELY(DeeList_Append(result,(DeeObject *)self) != 0) goto err;
 }
 return result;
err:
 Dee_DECREF(result);
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(Capitalize)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DeeObject *result; DEE_CHAR *dst,ch; DEE_CHAR const *begin,*end; int was_space;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 if ((result = DeeString_F(NewSized)(DEE_STRING_SIZE(self))) == NULL) return NULL;
 end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 dst = DEE_STRING_STR(result);
 was_space = 1;
 while (begin != end) {
  ch = *begin++;
  if (DEE_CH_IS_SPACE(ch)) {
   was_space = 1;
   *dst++ = ch;
  } else {
   *dst++ = (DEE_CHAR)(was_space ? DEE_CH_TO_UPPER((int)ch) : DEE_CH_TO_LOWER((int)ch));
   was_space = 0;
  }
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(Center)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN Dee_size_t width, DEE_A_IN DEE_CHAR fill_char) {
 DeeObject *result; Dee_size_t size;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 size = DEE_STRING_SIZE(self);
 if (width <= size) DeeReturn_NEWREF(self);
 if DEE_UNLIKELY((result = DeeString_F(NewSized)(width)) == NULL) return NULL;
#if DEE_STRING_ENCODING_SIZE(ENC) == 1
 memset(DEE_STRING_STR(result),fill_char,width);
#else
 {DEE_CHAR *dst,*end;
  end = (dst = DEE_STRING_STR(result))+width;
  while (dst != end) *dst++ = fill_char;
 }
#endif
 memcpy(DEE_STRING_STR(result)+((width-size)/2),DEE_STRING_STR(self),size);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(LJust)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN Dee_size_t width, DEE_A_IN DEE_CHAR fill_char) {
 DeeObject *result; Dee_size_t size;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 size = DEE_STRING_SIZE(self);
 if (width <= size) DeeReturn_NEWREF(self);
 if DEE_UNLIKELY((result = DeeString_F(NewSized)(width)) == NULL) return NULL;
 memcpy(DEE_STRING_STR(result),DEE_STRING_STR(self),size);
#if DEE_STRING_ENCODING_SIZE(ENC) == 1
 memset(DEE_STRING_STR(result)+size,fill_char,(width-size));
#else
 {DEE_CHAR *dst,*end;
  end = (dst = DEE_STRING_STR(result)+size)+(width-size);
  while (dst != end) *dst++ = fill_char;
 }
#endif
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(RJust)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN Dee_size_t width, DEE_A_IN DEE_CHAR fill_char) {
 DeeObject *result; Dee_size_t size;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 size = DEE_STRING_SIZE(self);
 if (width <= size) DeeReturn_NEWREF(self);
 if DEE_UNLIKELY((result = DeeString_F(NewSized)(width)) == NULL) return NULL;
 width -= size; // Filler space
#if DEE_STRING_ENCODING_SIZE(ENC) == 1
 memset(DEE_STRING_STR(result),fill_char,width);
#else
 {DEE_CHAR *dst,*end;
  end = (dst = DEE_STRING_STR(result))+width;
  while (dst != end) *dst++ = fill_char;
 }
#endif
 memcpy(DEE_STRING_STR(result)+width,DEE_STRING_STR(self),size);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(Strip)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *chars) {
 DEE_CHAR const *new_begin,*new_end,*list_begin,*list_end,*list_iter; DEE_CHAR ch;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 new_end = (new_begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 list_end = (list_begin = DEE_STRING_STR(chars))+DEE_STRING_SIZE(chars);
 while (new_begin != new_end) {
  ch = *new_begin;
  list_iter = list_begin;
  while (list_iter != list_end) 
   if (ch == *list_iter++) goto more;
  break;
more: ++new_begin;
 }
 while (new_begin != new_end) {
  ch = new_end[-1];
  list_iter = list_begin;
  while (list_iter != list_end) 
   if (ch == *list_iter++) goto more2;
  break;
more2: --new_end;
 }
 if (new_begin == DEE_STRING_STR(self) &&
     new_end == new_begin+DEE_STRING_SIZE(self)
     ) DeeReturn_NEWREF(self);
 return DeeString_F(NewWithLength)((Dee_size_t)(new_end-new_begin),new_begin);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(LStrip)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *chars) {
 DEE_CHAR const *new_begin,*end,*list_begin,*list_end,*list_iter; DEE_CHAR ch;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 end = (new_begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 list_end = (list_begin = DEE_STRING_STR(chars))+DEE_STRING_SIZE(chars);
 while (new_begin != end) {
  ch = *new_begin;
  list_iter = list_begin;
  while (list_iter != list_end) 
   if (ch == *list_iter++) goto more;
  break;
more: ++new_begin;
 }
 if (new_begin == DEE_STRING_STR(self))
  DeeReturn_NEWREF(self);
 return DeeString_F(NewWithLength)((Dee_size_t)(end-new_begin),new_begin);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(RStrip)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *chars) {
 DEE_CHAR const *begin,*new_end,*list_begin,*list_end,*list_iter; DEE_CHAR ch;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 new_end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 list_end = (list_begin = DEE_STRING_STR(chars))+DEE_STRING_SIZE(chars);
 while (new_end != begin) {
  ch = new_end[-1];
  list_iter = list_begin;
  while (list_iter != list_end) 
   if (ch == *list_iter++) goto more;
  break;
more: --new_end;
 }
 if (new_end == begin+DEE_STRING_SIZE(self))
  DeeReturn_NEWREF(self);
 return DeeString_F(NewWithLength)((Dee_size_t)(new_end-begin),begin);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(ExpandTabs)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self, DEE_A_IN Dee_size_t tabsize) {
 DeeObject *result;
 DEE_CHAR const *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 if (begin == end) DEERETURN_EMPTYSTRING;
 if (tabsize == 1) {
  DEE_CHAR *dst;
  if DEE_LIKELY((result = DeeString_F(NewSized)(DEE_STRING_SIZE(self))) != NULL) {
   dst = DEE_STRING_STR(result);
   while (begin != end) *dst++ = (DEE_CHAR)(
    *begin == DEE_CHAR_C('\t') ? DEE_CHAR_C(' ') : *begin),++begin;
  }
 } else {
  DEE_STRINGWRITER writer = DEE_STRINGWRITER_INIT();
  DEE_CHAR const *write_begin = begin;
  do if (*begin == DEE_CHAR_C('\t')) {
   if DEE_UNLIKELY(DeeStringWriter_F(DEE_PP_CAT_3(Write,ENC,StringWithLength))(
    &writer,(Dee_size_t)(begin-write_begin),write_begin) != 0||
    DeeStringWriter_F(RepeatChar)(&writer,DEE_CHAR_C(' '),tabsize) != 0) goto err;
   write_begin = begin+1;
  } while (++begin != end);
  if DEE_UNLIKELY(DeeStringWriter_F(DEE_PP_CAT_3(Write,ENC,StringWithLength))(
   &writer,(Dee_size_t)(begin-write_begin),write_begin) != 0) {
err: result = NULL; goto lend;
  }
  result = DEE_STRINGWRITER_Pack(&writer);;
lend: DEE_STRINGWRITER_Quit(&writer);
 }
 return result;
}
DEE_A_RET_WUNUSED /*noexcept*/int DeeString_F(Contains)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN_R(s_size) DEE_CHAR const *s, DEE_A_IN Dee_size_t s_size) {
 DEE_CHAR const *sbegin,*send; Dee_size_t size;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 size = DEE_STRING_SIZE(self);
 if (s_size > size) return 0;
 send = (sbegin = DEE_STRING_STR(self))+(size-s_size);
 do if (memcmp(sbegin,s,s_size) == 0) return 1;
 while (sbegin++ != send);
 return 0;
}
DEE_A_RET_WUNUSED /*noexcept*/Dee_ssize_t DeeString_F(Find)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sub,
 DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) {
 Dee_size_t size,sub_len; DEE_CHAR const *substr,*sbegin,*send;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 DEE_ASSERT(DeeObject_Check(sub) && DEE_STRING_Check(sub));
 DEE_ASSERT(begin <= end && end <= DEE_STRING_SIZE(self));
 size = (Dee_size_t)(end-begin),sub_len = DEE_STRING_SIZE(sub);
 if (sub_len > size) return -1;
 substr = DEE_STRING_STR(sub);
 send = (sbegin = DEE_STRING_STR(self)+begin)+(size-sub_len);
 do if (memcmp(sbegin,substr,sub_len) == 0) return sbegin-DEE_STRING_STR(self);
 while (sbegin++ != send);
 return -1; // Not found
}
DEE_A_RET_WUNUSED /*noexcept*/Dee_ssize_t DeeString_F(RFind)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sub,
 DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) {
 Dee_size_t size,sub_len; DEE_CHAR const *substr,*sbegin,*send;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 DEE_ASSERT(DeeObject_Check(sub) && DEE_STRING_Check(sub));
 DEE_ASSERT(begin <= end && end <= DEE_STRING_SIZE(self));
 size = (Dee_size_t)(end-begin),sub_len = DEE_STRING_SIZE(sub);
 if (sub_len > size) return -1;
 substr = DEE_STRING_STR(sub);
 send = (sbegin = DEE_STRING_STR(self)+begin)+(size-sub_len);
 do if (memcmp(send,substr,sub_len) == 0) return send-DEE_STRING_STR(self);
 while (send-- != sbegin);
 return -1; // Not found
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeString_F(Index)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sub,
 DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) {
 Dee_ssize_t result;
 if DEE_UNLIKELY((result = DeeString_F(Find)(self,sub,begin,end)) == -1) {
  DeeError_ValueError_NotFound((DeeObject *)self,(DeeObject *)sub);
  return (Dee_size_t)-1;
 }
 return (Dee_size_t)result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeString_F(RIndex)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sub,
 DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) {
 Dee_ssize_t result;
 if DEE_UNLIKELY((result = DeeString_F(RFind)(self,sub,begin,end)) == -1) {
  DeeError_ValueError_NotFound((DeeObject *)self,(DeeObject *)sub);
  return (Dee_size_t)-1;
 }
 return (Dee_size_t)result;
}
DEE_A_RET_WUNUSED /*noexcept*/Dee_size_t DeeString_F(Count)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sub,
 DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) {
 Dee_size_t result,size,sub_len; DEE_CHAR const *substr,*sbegin,*send;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 DEE_ASSERT(DeeObject_Check(sub) && DEE_STRING_Check(sub));
 DEE_ASSERT(begin <= end && end <= DEE_STRING_SIZE(self));
 size = DEE_STRING_SIZE(self),sub_len = DEE_STRING_SIZE(sub);
 if DEE_UNLIKELY(sub_len > size) return 0;
 substr = DEE_STRING_STR(sub);
 send = (sbegin = DEE_STRING_STR(self))+(size-sub_len);
 result = 0;
 do if (memcmp(sbegin,substr,sub_len) == 0) ++result;
 while (sbegin++ != send);
 return result;
}
DEE_A_RET_WUNUSED /*noexcept*/int DeeString_F(StartsWith)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sub,
 DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) {
 Dee_size_t sub_size;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 DEE_ASSERT(DeeObject_Check(sub) && DEE_STRING_Check(sub));
 DEE_ASSERT(begin <= end && end <= DEE_STRING_SIZE(self));
 sub_size = DEE_STRING_SIZE(sub);
 if DEE_UNLIKELY(sub_size > end-begin) return 0; // substring too long
 return memcmp(DEE_STRING_STR(self)+begin,
               DEE_STRING_STR(sub),sub_size*sizeof(DEE_CHAR)) == 0;
}
DEE_A_RET_WUNUSED /*noexcept*/int DeeString_F(EndsWith)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sub,
 DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) {
 Dee_size_t sub_size;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 DEE_ASSERT(DeeObject_Check(sub) && DEE_STRING_Check(sub));
 DEE_ASSERT(begin <= end && end <= DEE_STRING_SIZE(self));
 sub_size = DEE_STRING_SIZE(sub);
 if DEE_UNLIKELY(sub_size > end-begin) return 0; // substring too long
 return memcmp(DEE_STRING_STR(self)+(end-sub_size),
               DEE_STRING_STR(sub),sub_size*sizeof(DEE_CHAR)) == 0;
}
DEE_A_RET_WUNUSED /*noexcept*/int DeeString_F(IsAlnum)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DEE_CHAR *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 while (begin != end) if (!DEE_CH_IS_ALNUM(*begin++)) return 0;
 return 1;
}
DEE_A_RET_WUNUSED /*noexcept*/int DeeString_F(IsAlpha)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DEE_CHAR *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 while (begin != end) if (!DEE_CH_IS_ALPHA(*begin++)) return 0;
 return 1;
}
DEE_A_RET_WUNUSED /*noexcept*/int DeeString_F(IsBlank)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DEE_CHAR *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 while (begin != end) if (!DEE_CH_IS_BLANK((int)*begin++)) return 0;
 return 1;
}
DEE_A_RET_WUNUSED /*noexcept*/int DeeString_F(IsCntrl)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DEE_CHAR *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 while (begin != end) if (!DEE_CH_IS_CNTRL(*begin++)) return 0;
 return 1;
}
DEE_A_RET_WUNUSED /*noexcept*/int DeeString_F(IsDigit)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DEE_CHAR *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 while (begin != end) if (!DEE_CH_IS_DIGIT(*begin++)) return 0;
 return 1;
}
DEE_A_RET_WUNUSED /*noexcept*/int DeeString_F(IsGraph)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DEE_CHAR *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 while (begin != end) if (!DEE_CH_IS_GRAPH(*begin++)) return 0;
 return 1;
}
DEE_A_RET_WUNUSED /*noexcept*/int DeeString_F(IsLower)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DEE_CHAR *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 while (begin != end) if (DEE_CH_IS_UPPER(*begin++)) return 0;
 return 1;
}
DEE_A_RET_WUNUSED /*noexcept*/int DeeString_F(IsPrint)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DEE_CHAR *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 while (begin != end) if (!DEE_CH_IS_PRINT(*begin++)) return 0;
 return 1;
}
DEE_A_RET_WUNUSED /*noexcept*/int DeeString_F(IsPunct)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DEE_CHAR *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 while (begin != end) if (!DEE_CH_IS_PUNCT(*begin++)) return 0;
 return 1;
}
DEE_A_RET_WUNUSED /*noexcept*/int DeeString_F(IsSpace)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DEE_CHAR *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 while (begin != end) if (!DEE_CH_IS_SPACE(*begin++)) return 0;
 return 1;
}
DEE_A_RET_WUNUSED /*noexcept*/int DeeString_F(IsUpper)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DEE_CHAR *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 while (begin != end) if (DEE_CH_IS_LOWER(*begin++)) return 0;
 return 1;
}
DEE_A_RET_WUNUSED /*noexcept*/int DeeString_F(IsXChar)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DEE_CHAR *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 while (begin != end) if (!DEE_CH_IS_XCHAR((int)*begin++)) return 0;
 return 1;
}
DEE_A_RET_WUNUSED /*noexcept*/int DeeString_F(IsXDigit)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DEE_CHAR *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 while (begin != end) if (!DEE_CH_IS_DIGIT(*begin++)) return 0;
 return 1;
}
DEE_A_RET_WUNUSED /*noexcept*/int DeeString_F(IsTitle)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DEE_CHAR *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 if DEE_UNLIKELY(begin == end) return 1;
 if (DEE_CH_IS_LOWER(*begin++)) return 0;
 while (begin != end) if (DEE_CH_IS_UPPER(*begin++)) return 0;
 return 1;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeString_F(Reverse)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DeeObject *result; DEE_CHAR *dst,*src,*src_end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 if DEE_LIKELY((result = DeeString_F(NewSized)(DEE_STRING_SIZE(self))) != NULL) {
  dst = DEE_STRING_STR(result);
  src_end = (src = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
  while (src_end != src) *dst++ = *--src_end;
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeString_F(Lower)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DeeObject *result; DEE_CHAR *dst; DEE_CHAR const *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 if DEE_LIKELY((result = DeeString_F(NewSized)(DEE_STRING_SIZE(self))) != NULL) {
  end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
  dst = DEE_STRING_STR(result);
  while (begin != end) *dst++ = (DEE_CHAR)DEE_CH_TO_LOWER((int)*begin++);
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeString_F(Upper)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DeeObject *result; DEE_CHAR *dst; DEE_CHAR const *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 if DEE_LIKELY((result = DeeString_F(NewSized)(DEE_STRING_SIZE(self))) != NULL) {
  end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
  dst = DEE_STRING_STR(result);
  while (begin != end) *dst++ = (DEE_CHAR)DEE_CH_TO_UPPER((int)*begin++);
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeString_F(Title)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DeeObject *result; DEE_CHAR *dst; DEE_CHAR const *begin,*end; Dee_size_t s;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 if ((s = DEE_STRING_SIZE(self)) == 0) DEERETURN_EMPTYSTRING;
 if DEE_LIKELY((result = DeeString_F(NewSized)(s)) != NULL) {
  end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
  dst = DEE_STRING_STR(result);
  *dst++ = (DEE_CHAR)DEE_CH_TO_UPPER((int)*begin++);
  while (begin != end) *dst++ = (DEE_CHAR)DEE_CH_TO_LOWER((int)*begin++);
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeString_F(SwapCase)(DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self) {
 DeeObject *result; DEE_CHAR *dst,ch; DEE_CHAR const *begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 if DEE_LIKELY((result = DeeString_F(NewSized)(DEE_STRING_SIZE(self))) != NULL) {
  end = (begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
  dst = DEE_STRING_STR(result);
  while DEE_LIKELY(begin != end) {
   ch = *begin++;
   switch (DEE_CH_FLAGS(ch)&(_DEE_CHAR_FLAG_IS_LOWER|_DEE_CHAR_FLAG_IS_UPPER)) {
    case _DEE_CHAR_FLAG_IS_LOWER: ch = (DEE_CHAR)DEE_CH_TO_UPPER((int)ch); break;
    case _DEE_CHAR_FLAG_IS_UPPER: ch = (DEE_CHAR)DEE_CH_TO_LOWER((int)ch); break;
    default: break;
   }
   *dst++ = ch;
  }
 }
 return result;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) /*tuple(str_,str_,str_)*/ *
_DeeString_F(Partition_impl)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sep,
 DEE_A_IN Dee_ssize_t index) {
 DeeObject *result,*str1,*str3;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 DEE_ASSERT(DeeObject_Check(sep) && DEE_STRING_Check(sep));
 if DEE_UNLIKELY((result = _DeeTuple_NewUnsafe(3)) == NULL) return NULL;
 if (index == -1) {
  Dee_INCREF(str1 = (DeeObject *)self);
  Dee_INCREF(str3 = DEE_EMPTYSTRING);
 } else {
  if DEE_UNLIKELY((str1 = DeeString_F(NewWithLength)((
   Dee_size_t)index,DEE_STRING_STR(self))) == NULL) goto err;
  index += DEE_STRING_SIZE(sep);
  if DEE_UNLIKELY((str3 = DeeString_F(NewWithLength)(
   DEE_STRING_SIZE(self)-index,DEE_STRING_STR(self)+index)) == NULL) {
   Dee_DECREF(str1);
   goto err;
  }
 }
 DeeTuple_SET(result,0,str1); // Consume reference
 Dee_INCREF(sep);
 DeeTuple_SET(result,1,(DeeObject *)sep);
 DeeTuple_SET(result,2,str3); // Consume reference
 return result;
err: _DeeTuple_FreeUnsafe(result);
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) /*tuple(str_,str_,str_)*/ *DeeString_F(Partition)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self, DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sep) {
 return _DeeString_F(Partition_impl)(self,sep,DeeUtf8String_Find(self,sep,0,DEE_STRING_SIZE(self)));
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeTupleObject) /*tuple(str_,str_,str_)*/ *DeeString_F(RPartition)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self, DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sep) {
 return _DeeString_F(Partition_impl)(self,sep,DeeUtf8String_RFind(self,sep,0,DEE_STRING_SIZE(self)));
}

DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(Replace)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *find,
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *replace) {
 DeeObject *result; DEE_CHAR const *block_begin,*begin,*end,*sfind,*sreplace;
 Dee_size_t find_size,replace_size;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 DEE_ASSERT(DeeObject_Check(find) && DEE_STRING_Check(find));
 DEE_ASSERT(DeeObject_Check(replace) && DEE_STRING_Check(replace));
 if DEE_UNLIKELY((find_size = DEE_STRING_SIZE(find)) > DEE_STRING_SIZE(self)) DeeReturn_NEWREF(self);
 if DEE_UNLIKELY(!find_size) DeeReturn_NEWREF(DEE_STRING_SIZE(self) ? (DeeObject *)self : (DeeObject *)replace);
 {DEE_STRINGWRITER writer = DEE_STRINGWRITER_INIT();
  replace_size = DEE_STRING_SIZE(replace);
  end = (begin = DEE_STRING_STR(self))+(DEE_STRING_SIZE(self)-(find_size-1));
  sfind = DEE_STRING_STR(find),sreplace = DEE_STRING_STR(replace);
  block_begin = begin;
  while (begin <= end) {
   if (memcmp(begin,sfind,find_size*sizeof(DEE_CHAR)) == 0) { // Found one
    if DEE_UNLIKELY(DeeStringWriter_F(DEE_PP_CAT_3(Write,ENC,StringWithLength))(&writer,(Dee_size_t)(begin-block_begin),block_begin) != 0||
                    DeeStringWriter_F(DEE_PP_CAT_3(Write,ENC,StringWithLength))(&writer,replace_size,sreplace) != 0) goto err;
    begin += find_size;
    block_begin = begin;
    if (begin >= end) break;
    continue;
   }
   ++begin;
  }
  // If we never found 'find', our writer will still be empty.
  // >> In that case we don't need to write the entire string to it,
  //    but can simply return a reference to the original string,
  //    saving on memory and speeding up the function by a lot.
  if (DeeStringWriter_F(SIZE)(&writer) == 0 &&
      block_begin == DEE_STRING_STR(self)) DeeReturn_NEWREF(self);
  if DEE_UNLIKELY(DeeStringWriter_F(DEE_PP_CAT_3(Write,ENC,StringWithLength))(
   &writer,(Dee_size_t)((end-block_begin)+(find_size-1)),block_begin) != 0) goto err;
  result = DEE_STRINGWRITER_Pack(&writer);
end: DEE_STRINGWRITER_Quit(&writer);
  return result;
err: result = NULL; goto end;
 }
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeListObject) /*list(str_)*/ *DeeString_F(SplitLines)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self, DEE_A_IN int keepends) {
 DeeObject *result,*elem; DEE_CHAR const *line_begin,*begin,*end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) return NULL;
 end = (line_begin = begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 while (begin != end) switch (*begin++) {
  case DEE_CHAR_C('\n'):
adde1:
   elem = DeeString_F(NewWithLength)((
   Dee_size_t)(begin-line_begin)-(keepends ? 0 : 1),line_begin);
adde:
   if DEE_UNLIKELY(!elem) goto err;
   if DEE_UNLIKELY(DeeList_Append(result,elem) != 0) goto err2;
   Dee_DECREF(elem);
   line_begin = begin;
   break;
  case DEE_CHAR_C('\r'):
   if (*begin == DEE_CHAR_C('\n')) {
    elem = DeeString_F(NewWithLength)((
     Dee_size_t)(++begin-line_begin)-(keepends ? 0 : 2),line_begin);
    goto adde;
   } else goto adde1;
  default: break;
 }
 if DEE_UNLIKELY((elem = DeeString_F(NewWithLength)((
  Dee_size_t)(begin-line_begin),line_begin)) == NULL) goto err;
 if DEE_UNLIKELY(DeeList_Append(result,elem) != 0) goto err2;
 Dee_DECREF(elem);
 return result;
err2: Dee_DECREF(elem);
err: Dee_DECREF(result);
 return NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(SStrip)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self, DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sub) {
 DEE_CHAR *new_begin,*new_end,*substr; Dee_size_t sublen;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 DEE_ASSERT(DeeObject_Check(sub) && DEE_STRING_Check(sub));
 sublen = DEE_STRING_SIZE(sub);
 if (sublen > DEE_STRING_SIZE(self) || !sublen) DeeReturn_NEWREF(self);
 new_end = (new_begin = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 substr = DEE_STRING_STR(sub);
 if (memcmp(new_begin,substr,sublen) == 0) new_begin += sublen;
 if (memcmp(new_end-sublen,substr,sublen*sizeof(DEE_CHAR)) == 0) new_end -= sublen;
 if (new_end >= new_begin) DEERETURN_EMPTYSTRING;
 return DeeString_F(NewWithLength)((Dee_size_t)(new_end-new_begin),new_begin);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(LSStrip)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self, DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sub) {
 Dee_size_t sublen;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 DEE_ASSERT(DeeObject_Check(sub) && DEE_STRING_Check(sub));
 sublen = DEE_STRING_SIZE(sub);
 if (sublen > DEE_STRING_SIZE(self) || !sublen ||
     memcmp(DEE_STRING_STR(self),DEE_STRING_STR(sub),
     sublen*sizeof(DEE_CHAR)) != 0) DeeReturn_NEWREF(self);
 return DeeString_F(NewWithLength)(
  DEE_STRING_SIZE(self)-sublen,DEE_STRING_STR(self)+sublen);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(RSStrip)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self, DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *sub) {
 Dee_size_t sublen;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 DEE_ASSERT(DeeObject_Check(sub) && DEE_STRING_Check(sub));
 sublen = DEE_STRING_SIZE(sub);
 if (sublen > DEE_STRING_SIZE(self) || !sublen ||
     memcmp(DEE_STRING_STR(self)+(DEE_STRING_SIZE(self)-sublen),DEE_STRING_STR(sub),
     sublen*sizeof(DEE_CHAR)) != 0) DeeReturn_NEWREF(self);
 return DeeString_F(NewWithLength)(
  DEE_STRING_SIZE(self)-sublen,DEE_STRING_STR(self));
}
DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeListObject) *DeeString_F(Segments)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self, DEE_A_IN Dee_size_t n) {
 DeeObject **dst_iter,**dst_end,*result; DEE_CHAR const *src_iter;
 Dee_size_t my_segment_size,src_size,segment_size;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 if DEE_UNLIKELY(!n) return DeeList_NewEmpty();
 if DEE_UNLIKELY((result = _DeeList_NewUnsafe(n)) == NULL) return NULL;
 src_size = DEE_STRING_SIZE(self),segment_size = src_size/n;
 if (segment_size*n != src_size) ++segment_size;
 dst_end = (dst_iter = DeeList_ELEM(result))+n;
 src_iter = DEE_STRING_STR(self);
 while (dst_iter != dst_end) {
  my_segment_size = segment_size <= src_size ? segment_size : src_size;
  if DEE_UNLIKELY((*dst_iter = DeeString_F(NewWithLength)(segment_size,src_iter)) == NULL) {
   while (dst_iter != DeeList_ELEM(result)) Dee_DECREF(*--dst_iter);
   _DeeList_FreeUnsafe(result);
   return NULL;
  }
  ++dst_iter;
  src_iter += my_segment_size;
  src_size -= my_segment_size;
 }
 return result;
}



DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_F(StrWMatch)(
 DEE_A_IN_Z DEE_CHAR const *str_, DEE_A_IN_Z DEE_CHAR const *pat_) {
 DEE_CHAR card_post;
 while (1) {
  if (!*str_) {
   // End of string (if the patter is empty, or only contains '*', we have a match)
   while (*pat_ == DEE_CHAR_C('*')) ++pat_;
   return *pat_ ? 0 : 1;
  }
  if (!*pat_) return 0; // Patter end doesn't match
  if (*pat_ == DEE_CHAR_C('*')) {
   // Skip starts
   do ++pat_; while (*pat_ == DEE_CHAR_C('*'));
   card_post = *pat_++;
   if (!card_post) return 1; // Pattern ends with '*' (matches everything)
   if (card_post == DEE_CHAR_C('?')) goto next; // Match any --> already found
   while (1) {
    if (*str_++ == card_post) {
     // Recursively check if the rest of the string and pattern match
     if (Dee_F(StrWMatch)(str_,pat_)) return 1;
    } else if (!*str_) return 0; // Wildcard suffix not found
   }
  }
  if (*pat_ == *str_ || *pat_ == DEE_CHAR_C('?')) {
next: ++str_,++pat_;
   continue; // single character match
  }
  break; // missmatch
 }
 return 0;
}
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_F(StrWMatchLen)(
 DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) DEE_CHAR const *str_,
 DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) DEE_CHAR const *pat_) {
 DEE_CHAR card_post;
 while (1) {
  if (!str_size) {
   // End of string (if the patter is empty, or only contains '*', we have a match)
   while (*pat_ == DEE_CHAR_C('*') && pat_size) ++pat_,--pat_size;
   return pat_size ? 0 : 1;
  }
  if (!pat_size) return 0; // Patter end doesn't match
  if (*pat_ == DEE_CHAR_C('*')) {
   // Skip starts
   do ++pat_,--pat_size; while (pat_size && *pat_ == DEE_CHAR_C('*'));
   if (!pat_size) return 1; // Pattern ends with '*' (matches everything)
   card_post = *pat_++; --pat_size;
   if (card_post == DEE_CHAR_C('?')) goto next; // Match any --> already found
   while (1) {
    --str_size;
    if (*str_++ == card_post) {
     // Recursively check if the rest of the string and pattern match
     if (Dee_F(StrWMatchLen)(str_size,str_,pat_size,pat_)) return 1;
    }
    if (!str_size) return 0; // Wildcard suffix not found
   }
  }
  if (*pat_ == *str_ || *pat_ == DEE_CHAR_C('?')) {
next: --str_size,++str_,--pat_size,++pat_;
   continue; // single character match
  }
  break; // missmatch
 }
 return 0;
}

DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_F(StrCaseWMatch)(
 DEE_A_IN_Z DEE_CHAR const *str_, DEE_A_IN_Z DEE_CHAR const *pat_) {
 DEE_CHAR card_post;
 while (1) {
  if (!*str_) {
   // End of string (if the patter is empty, or only contains '*', we have a match)
   while (*pat_ == DEE_CHAR_C('*')) ++pat_;
   return *pat_ ? 0 : 1;
  }
  if (!*pat_) return 0; // Patter end doesn't match
  if (*pat_ == DEE_CHAR_C('*')) {
   // Skip starts
   do ++pat_; while (*pat_ == DEE_CHAR_C('*'));
   card_post = *pat_++;
   if (!card_post) return 1; // Pattern ends with '*' (matches everything)
   if (card_post == DEE_CHAR_C('?')) goto next; // Match any --> already found
   while (1) {
    if (*str_++ == card_post) {
     // Recursively check if the rest of the string and pattern match
     if (Dee_F(StrWMatch)(str_,pat_)) return 1;
    } else if (!*str_) return 0; // Wildcard suffix not found
   }
  }
  if ((DEE_CHAR)DEE_CH_TO_LOWER((int)*pat_) ==
      (DEE_CHAR)DEE_CH_TO_LOWER((int)*str_) || *pat_ == DEE_CHAR_C('?')) {
next: ++str_,++pat_;
   continue; // single character match
  }
  break; // missmatch
 }
 return 0;
}
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) Dee_F(StrCaseWMatchLen)(
 DEE_A_IN Dee_size_t str_size, DEE_A_IN_R(str_size) DEE_CHAR const *str_,
 DEE_A_IN Dee_size_t pat_size, DEE_A_IN_R(pat_size) DEE_CHAR const *pat_) {
 DEE_CHAR card_post;
 while (1) {
  if (!str_size) {
   // End of string (if the patter is empty, or only contains '*', we have a match)
   while (*pat_ == DEE_CHAR_C('*') && pat_size) ++pat_,--pat_size;
   return pat_size ? 0 : 1;
  }
  if (!pat_size) return 0; // Patter end doesn't match
  if (*pat_ == DEE_CHAR_C('*')) {
   // Skip starts
   do ++pat_,--pat_size; while (pat_size && *pat_ == DEE_CHAR_C('*'));
   if (!pat_size) return 1; // Pattern ends with '*' (matches everything)
   card_post = *pat_++; --pat_size;
   if (card_post == DEE_CHAR_C('?')) goto next; // Match any --> already found
   while (1) {
    --str_size;
    if (*str_++ == card_post) {
     // Recursively check if the rest of the string and pattern match
     if (Dee_F(StrWMatchLen)(str_size,str_,pat_size,pat_)) return 1;
    }
    if (!str_size) return 0; // Wildcard suffix not found
   }
  }
  if ((DEE_CHAR)DEE_CH_TO_LOWER((int)*pat_) ==
      (DEE_CHAR)DEE_CH_TO_LOWER((int)*str_) || *pat_ == DEE_CHAR_C('?')) {
next: --str_size,++str_,--pat_size,++pat_;
   continue; // single character match
  }
  break; // missmatch
 }
 return 0;
}





DEE_COMPILER_MSVC_WARNING_PUSH(4061)
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeString_F(CFormat)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *self,
 DEE_A_IN_OBJECT(DeeTupleObject) const *args) {
 DeeObject *result;
 DEE_STRINGWRITER writer = DEE_STRINGWRITER_INIT();
 DeeStringWriterFormatSpec spec;
 DEE_CHAR ch;
 DEE_CHAR const *flush_start,*fmt,*fmt_end;
 enum{
  len_none,len_hh,len_h,len_l,len_ll,len_j,len_z,len_t,len_L,
  len_I,len_I8,len_I16,len_I32,len_I64
 }length;
 DeeObject *arg,**argv,**argv_end;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 fmt_end = (fmt = DEE_STRING_STR(self))+DEE_STRING_SIZE(self);
 argv_end = (argv = DeeTuple_ELEM(args))+DeeTuple_SIZE(args);
#define GET_ARG() do{if(argv==argv_end)goto no_args;arg=*argv++;}while(0)
 flush_start = fmt;
 while (1) {
  if ((ch = *fmt++) == DEE_CHAR_C('\0') && fmt >= fmt_end) break;
  if (ch == DEE_CHAR_C('%')) {
   if DEE_UNLIKELY(DeeStringWriter_F(DEE_PP_CAT_3(Write,ENC,StringWithLength))(
    &writer,(Dee_size_t)((fmt-flush_start)-1),flush_start) != 0) goto err;
   ch = *fmt++;
   memset(&spec,0,sizeof(spec));
   length = len_none;
p_flag: // flags
   switch (ch) {
    case DEE_CHAR_C('-'): spec.left_just = 1; ch = *fmt++; goto p_flag;
    case DEE_CHAR_C('+'): spec.sign_pos = 1; ch = *fmt++; goto p_flag;
    case DEE_CHAR_C(' '): spec.sign_space = 1; ch = *fmt++; goto p_flag;
    case DEE_CHAR_C('#'): spec.prefix = 1; ch = *fmt++; goto p_flag;
    case DEE_CHAR_C('0'): spec.pad_zero = 1; ch = *fmt++; goto p_flag;
    case DEE_CHAR_C('>'): ++spec.instab; ch = *fmt++; goto p_flag;
    default: break;
   }
   // width
   switch (ch) {
    case DEE_CHAR_C('1'): case DEE_CHAR_C('2'): case DEE_CHAR_C('3'):
    case DEE_CHAR_C('4'): case DEE_CHAR_C('5'): case DEE_CHAR_C('6'):
    case DEE_CHAR_C('7'): case DEE_CHAR_C('8'): case DEE_CHAR_C('9'):
     spec.has_width = 1;
     spec.width = (unsigned int)(ch-DEE_CHAR_C('0'));
     while (1) {
      ch = *fmt++;
      if (ch < DEE_CHAR_C('0') || ch > DEE_CHAR_C('9')) break;
      spec.width *= 10;
      spec.width += (ch-DEE_CHAR_C('0'));
     }
     break;
    case DEE_CHAR_C('*'):
     spec.has_width = 1;
     GET_ARG();
     if (DeeObject_Cast(uint,arg,&spec.width) != 0) goto err;
     ch = *fmt++;
     break;
   }
   // Precision
   if (ch == DEE_CHAR_C('.')) {
    ch = *fmt++;
    spec.has_precision = 1;
    switch (ch) {
     case DEE_CHAR_C('1'): case DEE_CHAR_C('2'): case DEE_CHAR_C('3'):
     case DEE_CHAR_C('4'): case DEE_CHAR_C('5'): case DEE_CHAR_C('6'):
     case DEE_CHAR_C('7'): case DEE_CHAR_C('8'): case DEE_CHAR_C('9'):
      spec.has_precision = 1;
      spec.precision = (unsigned int)(ch-DEE_CHAR_C('0'));
      while (1) {
       ch = *fmt++;
       if (ch < DEE_CHAR_C('0') || ch > DEE_CHAR_C('9')) break;
       spec.precision *= 10;
       spec.precision += (ch-DEE_CHAR_C('0'));
      }
      break;
     case DEE_CHAR_C('*'):
      spec.has_precision = 1;
      GET_ARG();
      if DEE_UNLIKELY(DeeObject_Cast(uint,arg,&spec.precision) != 0) goto err;
      ch = *fmt++;
      break;
     default: --fmt; ch = DEE_CHAR_C('.'); spec.has_precision = 0; break;
    }
   }
   // Length specifier
   switch (ch) {
    case DEE_CHAR_C('h'): ch = *fmt++; if (ch == DEE_CHAR_C('h')) { ch = *fmt++; length = len_hh; } else length = len_h; break;
    case DEE_CHAR_C('l'): ch = *fmt++; if (ch == DEE_CHAR_C('l')) { ch = *fmt++; length = len_ll; } else length = len_l; break;
    case DEE_CHAR_C('j'): ch = *fmt++; length = len_j; break;
    case DEE_CHAR_C('z'): ch = *fmt++; length = len_z; break;
    case DEE_CHAR_C('t'): ch = *fmt++; length = len_t; break;
    case DEE_CHAR_C('L'): ch = *fmt++; length = len_L; break;
    case DEE_CHAR_C('I'):
     switch (ch = *fmt++) {
      case DEE_CHAR_C('8'): ch = *fmt++; length = len_I8; break;
      case DEE_CHAR_C('1'): if DEE_LIKELY((ch = *fmt++) == DEE_CHAR_C('6')) { ch = *fmt++; length = len_I16; } else { ch = *--fmt; goto def_lI; } break;
      case DEE_CHAR_C('3'): if DEE_LIKELY((ch = *fmt++) == DEE_CHAR_C('2')) { ch = *fmt++; length = len_I32; } else { ch = *--fmt; goto def_lI; } break;
      case DEE_CHAR_C('6'): if DEE_LIKELY((ch = *fmt++) == DEE_CHAR_C('4')) { ch = *fmt++; length = len_I64; } else { ch = *--fmt; goto def_lI; } break;
      default: def_lI: length = len_I; break;
     }
     break;
    default: break;
   }
   // format specifier
   switch (ch) {
    case DEE_CHAR_C('%'):
     if DEE_UNLIKELY(DeeStringWriter_F(WriteUtf8Char)(&writer,DEE_CHAR_C('%')) != 0) goto err;
     break;
    case DEE_CHAR_C('d'): case DEE_CHAR_C('i'):
     spec.numsys = 10;
     switch (length) {
#define WRITE_INT(T,spec_write,ob2i)\
 { T temp; GET_ARG();\
  if DEE_UNLIKELY(ob2i(Dee_TYPE(arg),arg,&temp) != 0) goto err; \
  if DEE_UNLIKELY(spec_write(&writer,temp,&spec) != 0) goto err;\
 }
#define LABEL_WRITE_INT(sizeof)  DEE_PP_CAT_2(wi,DEE_PP_MUL8(sizeof))
#define LABEL_WRITE_UINT(sizeof) DEE_PP_CAT_2(wui,DEE_PP_MUL8(sizeof))
      case len_I8:  wi8: WRITE_INT(Dee_int8_t, DeeStringWriter_F(SpecWriteInt8), DeeObject_Cast_F(Dee_int8_t)) break;
      case len_I16: wi16:WRITE_INT(Dee_int16_t,DeeStringWriter_F(SpecWriteInt16),DeeObject_Cast_F(Dee_int16_t)) break;
      case len_I32: wi32:WRITE_INT(Dee_int32_t,DeeStringWriter_F(SpecWriteInt32),DeeObject_Cast_F(Dee_int32_t)) break;
      case len_I64: wi64:WRITE_INT(Dee_int64_t,DeeStringWriter_F(SpecWriteInt64),DeeObject_Cast_F(Dee_int64_t)) break;
      case len_hh:  goto LABEL_WRITE_INT(DEE_CONFIG_RT_SIZEOF_CHAR);
      case len_h:   goto LABEL_WRITE_INT(DEE_CONFIG_RT_SIZEOF_SHORT);
      case len_l:   goto LABEL_WRITE_INT(DEE_CONFIG_RT_SIZEOF_LONG);
      case len_ll:  goto LABEL_WRITE_INT(DEE_CONFIG_RT_SIZEOF_LLONG);
      case len_j:   goto LABEL_WRITE_INT(DEE_CONFIG_RT_SIZEOF_INTMAX_T);
      case len_I:   goto LABEL_WRITE_INT(DEE_CONFIG_RT_SIZEOF_POINTER);
      case len_z:   goto LABEL_WRITE_UINT(DEE_CONFIG_RT_SIZEOF_SIZE_T);
      case len_t:   goto LABEL_WRITE_INT(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T);
      default:      goto LABEL_WRITE_INT(DEE_CONFIG_RT_SIZEOF_INT);
     }
     break;
    case DEE_CHAR_C('b'): spec.numsys = 2; goto wu;
    case DEE_CHAR_C('o'): spec.numsys = 8; goto wu;
    case DEE_CHAR_C('X'): spec.upper_hex = 1; DEE_ATTRIBUTE_FALLTHROUGH
    case DEE_CHAR_C('x'): spec.numsys = 16; goto wu;
    case DEE_CHAR_C('u'): spec.numsys = 10;
wu:  switch (length) {
      case len_I8:  wui8: WRITE_INT(Dee_uint8_t, DeeStringWriter_F(SpecWriteUInt8), DeeObject_Cast_F(Dee_uint8_t)) break;
      case len_I16: wui16:WRITE_INT(Dee_uint16_t,DeeStringWriter_F(SpecWriteUInt16),DeeObject_Cast_F(Dee_uint16_t)) break;
      case len_I32: wui32:WRITE_INT(Dee_uint32_t,DeeStringWriter_F(SpecWriteUInt32),DeeObject_Cast_F(Dee_uint32_t)) break;
      case len_I64: wui64:WRITE_INT(Dee_uint64_t,DeeStringWriter_F(SpecWriteUInt64),DeeObject_Cast_F(Dee_uint64_t)) break;
      case len_hh:  goto LABEL_WRITE_UINT(DEE_CONFIG_RT_SIZEOF_CHAR);
      case len_h:   goto LABEL_WRITE_UINT(DEE_CONFIG_RT_SIZEOF_SHORT);
      case len_l:   goto LABEL_WRITE_UINT(DEE_CONFIG_RT_SIZEOF_LONG);
      case len_ll:  goto LABEL_WRITE_UINT(DEE_CONFIG_RT_SIZEOF_LLONG);
      case len_j:   goto LABEL_WRITE_UINT(DEE_CONFIG_RT_SIZEOF_INTMAX_T);
      case len_I:   goto LABEL_WRITE_UINT(DEE_CONFIG_RT_SIZEOF_POINTER);
      case len_z:   goto LABEL_WRITE_UINT(DEE_CONFIG_RT_SIZEOF_SIZE_T);
      case len_t:   goto LABEL_WRITE_INT(DEE_CONFIG_RT_SIZEOF_PTRDIFF_T);
      default:      goto LABEL_WRITE_UINT(DEE_CONFIG_RT_SIZEOF_INT);
     }
     break;
#undef WRITE_INT
    case DEE_CHAR_C('p'):
     spec.numsys = 16;
     spec.has_precision = 1;
     if (spec.precision < DEE_CONFIG_RT_SIZEOF_POINTER*2)
      spec.precision = DEE_CONFIG_RT_SIZEOF_POINTER*2;
     goto LABEL_WRITE_UINT(DEE_CONFIG_RT_SIZEOF_POINTER);
#undef LABEL_WRITE_UINT
#undef LABEL_WRITE_INT
    case DEE_CHAR_C('c'):
     GET_ARG();
     switch (length) {
      case len_l: {
       Dee_WideChar temp;
       if DEE_UNLIKELY(DeeObject_Cast(Dee_WideChar,arg,&temp) != 0) goto err;
       if DEE_UNLIKELY(DeeStringWriter_F(SpecWriteWideChar)(&writer,temp,&spec) != 0) goto err;
      } break;
      case len_I8: {
       Dee_Utf8Char temp;
       if DEE_UNLIKELY(DeeObject_Cast(Dee_Utf8Char,arg,&temp) != 0) goto err;
       if DEE_UNLIKELY(DeeStringWriter_F(SpecWriteUtf8Char)(&writer,temp,&spec) != 0) goto err;
      } break;
      case len_I16: {
       Dee_Utf16Char temp;
       if DEE_UNLIKELY(DeeObject_Cast(Dee_Utf16Char,arg,&temp) != 0) goto err;
       if DEE_UNLIKELY(DeeStringWriter_F(SpecWriteUtf16Char)(&writer,temp,&spec) != 0) goto err;
      } break;
      case len_I32: {
       Dee_Utf32Char temp;
       if DEE_UNLIKELY(DeeObject_Cast(Dee_Utf32Char,arg,&temp) != 0) goto err;
       if DEE_UNLIKELY(DeeStringWriter_F(SpecWriteUtf32Char)(&writer,temp,&spec) != 0) goto err;
      } break;
      default: {
       char temp;
       if DEE_UNLIKELY(DeeObject_Cast(char,arg,&temp) != 0) goto err;
       if DEE_UNLIKELY(DeeStringWriter_F(SpecWriteChar)(&writer,temp,&spec) != 0) goto err;
      } break;
     }
     break;
    case DEE_CHAR_C('f'): case DEE_CHAR_C('F'):
    case DEE_CHAR_C('e'): case DEE_CHAR_C('E'):
    case DEE_CHAR_C('g'): case DEE_CHAR_C('G'):
    case DEE_CHAR_C('a'): case DEE_CHAR_C('A'):
     GET_ARG();
     if (length == len_L) {
      long double temp;
      if DEE_UNLIKELY(DeeObject_Cast(ldouble,arg,&temp) != 0) goto err;
      if DEE_UNLIKELY(DeeStringWriter_F(SpecWriteLDouble)(&writer,temp,&spec) != 0) goto err;
     } else {
      double temp;
      if DEE_UNLIKELY(DeeObject_Cast(double,arg,&temp) != 0) goto err;
      if DEE_UNLIKELY(DeeStringWriter_F(SpecWriteDouble)(&writer,temp,&spec) != 0) goto err;
     }
     break;
    case DEE_CHAR_C('s'):
    case DEE_CHAR_C('q'):
     GET_ARG();
#if DEE_CONFIG_HAVE_ENCODING_UTF8
     if (DeeUtf8String_Check(arg)) {
      if DEE_UNLIKELY((ch != DEE_CHAR_C('q') ? DeeStringWriter_F(SpecWriteUtf8StringWithLength)(&writer,(spec.has_precision && spec.precision < DeeUtf8String_SIZE(arg)) ? spec.precision : DeeUtf8String_SIZE(arg),DeeUtf8String_STR(arg),&spec)
                                       : DeeStringWriter_F(SpecWriteQuotedUtf8StringWithLength)(&writer,(spec.has_precision && spec.precision < DeeUtf8String_SIZE(arg)) ? spec.precision : DeeUtf8String_SIZE(arg),DeeUtf8String_STR(arg),&spec)
         ) != 0) goto err;
     } else
#endif
#if DEE_CONFIG_HAVE_ENCODING_WIDE
     if (DeeWideString_Check(arg)) {
      if DEE_UNLIKELY((ch != DEE_CHAR_C('q') ? DeeStringWriter_F(SpecWriteWideStringWithLength)(&writer,(spec.has_precision && spec.precision < DeeWideString_SIZE(arg)) ? spec.precision : DeeWideString_SIZE(arg),DeeWideString_STR(arg),&spec)
                                       : DeeStringWriter_F(SpecWriteQuotedWideStringWithLength)(&writer,(spec.has_precision && spec.precision < DeeWideString_SIZE(arg)) ? spec.precision : DeeWideString_SIZE(arg),DeeWideString_STR(arg),&spec)
         ) != 0) goto err;
     } else
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
     if (DeeUtf16String_Check(arg)) {
      if DEE_UNLIKELY((ch != DEE_CHAR_C('q') ? DeeStringWriter_F(SpecWriteUtf16StringWithLength)(&writer,(spec.has_precision && spec.precision < DeeUtf16String_SIZE(arg)) ? spec.precision : DeeUtf16String_SIZE(arg),DeeUtf16String_STR(arg),&spec)
                                       : DeeStringWriter_F(SpecWriteQuotedUtf16StringWithLength)(&writer,(spec.has_precision && spec.precision < DeeUtf16String_SIZE(arg)) ? spec.precision : DeeUtf16String_SIZE(arg),DeeUtf16String_STR(arg),&spec)
         ) != 0) goto err;
     } else
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
     if (DeeUtf32String_Check(arg)) {
      if DEE_UNLIKELY((ch != DEE_CHAR_C('q') ? DeeStringWriter_F(SpecWriteUtf32StringWithLength)(&writer,(spec.has_precision && spec.precision < DeeUtf32String_SIZE(arg)) ? spec.precision : DeeUtf32String_SIZE(arg),DeeUtf32String_STR(arg),&spec)
                                             : DeeStringWriter_F(SpecWriteQuotedUtf32StringWithLength)(&writer,(spec.has_precision && spec.precision < DeeUtf32String_SIZE(arg)) ? spec.precision : DeeUtf32String_SIZE(arg),DeeUtf32String_STR(arg),&spec)
         ) != 0) goto err;
     } else
#endif
     if (DeeNone_Check(arg)) {
#if 1
      static DEE_CHAR const null_str[] = {
       DEE_CHAR_C('('),DEE_CHAR_C('n'),
       DEE_CHAR_C('u'),DEE_CHAR_C('l'),
       DEE_CHAR_C('l'),DEE_CHAR_C(')'),0};
#else
      static DEE_CHAR const null_str[] = {0};
#endif
#define NULLSTR_LEN  ((sizeof(null_str)/sizeof(*null_str))-1)
      if DEE_UNLIKELY((ch != DEE_CHAR_C('q') ? DeeStringWriter_F(DEE_PP_CAT_3(SpecWrite,ENC,StringWithLength))(&writer,(spec.has_precision && spec.precision < NULLSTR_LEN) ? spec.precision : NULLSTR_LEN,null_str,&spec)
                                             : DeeStringWriter_F(DEE_PP_CAT_3(SpecWriteQuoted,ENC,StringWithLength))(&writer,(spec.has_precision && spec.precision < NULLSTR_LEN) ? spec.precision : NULLSTR_LEN,null_str,&spec)
         ) != 0) goto err;
#undef NULLSTR_LEN
     } else
     {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
      void *argpval;
      switch (length) {
       case len_l:
        if (DeeObject_GetPointerEx(arg,DeeObject_TYPE(Dee_WideChar),&argpval) != 0) goto err;
        if DEE_UNLIKELY((ch != DEE_CHAR_C('q') ? DeeStringWriter_F(SpecWriteWideStringWithLength)(&writer,spec.has_precision ? Dee_WideStrNLen((Dee_WideChar *)argpval,spec.precision) : Dee_WideStrLen((Dee_WideChar *)argpval),(Dee_WideChar *)argpval,&spec)
                                               : DeeStringWriter_F(SpecWriteQuotedWideStringWithLength)(&writer,spec.has_precision ? Dee_WideStrNLen((Dee_WideChar *)argpval,spec.precision) : Dee_WideStrLen((Dee_WideChar *)argpval),(Dee_WideChar *)argpval,&spec)
           ) != 0) goto err;
        break;
       case len_I8:
        if (DeeObject_GetPointerEx(arg,DeeObject_TYPE(Dee_Utf8Char),&argpval) != 0) goto err;
        if DEE_UNLIKELY((ch != DEE_CHAR_C('q') ? DeeStringWriter_F(SpecWriteUtf8StringWithLength)(&writer,spec.has_precision ? Dee_Utf8StrNLen((Dee_Utf8Char *)argpval,spec.precision) : Dee_Utf8StrLen((Dee_Utf8Char *)argpval),(Dee_Utf8Char *)argpval,&spec)
                                               : DeeStringWriter_F(SpecWriteQuotedUtf8StringWithLength)(&writer,spec.has_precision ? Dee_Utf8StrNLen((Dee_Utf8Char *)argpval,spec.precision) : Dee_Utf8StrLen((Dee_Utf8Char *)argpval),(Dee_Utf8Char *)argpval,&spec)
           ) != 0) goto err;
        break;
       case len_I16:
        if (DeeObject_GetPointerEx(arg,DeeObject_TYPE(Dee_Utf16Char),&argpval) != 0) goto err;
        if DEE_UNLIKELY((ch != DEE_CHAR_C('q') ? DeeStringWriter_F(SpecWriteUtf16StringWithLength)(&writer,spec.has_precision ? Dee_Utf16StrNLen((Dee_Utf16Char *)argpval,spec.precision) : Dee_Utf16StrLen((Dee_Utf16Char *)argpval),(Dee_Utf16Char *)argpval,&spec)
                                               : DeeStringWriter_F(SpecWriteQuotedUtf16StringWithLength)(&writer,spec.has_precision ? Dee_Utf16StrNLen((Dee_Utf16Char *)argpval,spec.precision) : Dee_Utf16StrLen((Dee_Utf16Char *)argpval),(Dee_Utf16Char *)argpval,&spec)
           ) != 0) goto err;
        break;
       case len_I32:
        if (DeeObject_GetPointerEx(arg,DeeObject_TYPE(Dee_Utf32Char),&argpval) != 0) goto err;
        if DEE_UNLIKELY((ch != DEE_CHAR_C('q') ? DeeStringWriter_F(SpecWriteUtf32StringWithLength)(&writer,spec.has_precision ? Dee_Utf32StrNLen((Dee_Utf32Char *)argpval,spec.precision) : Dee_Utf32StrLen((Dee_Utf32Char *)argpval),(Dee_Utf32Char *)argpval,&spec)
                                               : DeeStringWriter_F(SpecWriteQuotedUtf32StringWithLength)(&writer,spec.has_precision ? Dee_Utf32StrNLen((Dee_Utf32Char *)argpval,spec.precision) : Dee_Utf32StrLen((Dee_Utf32Char *)argpval),(Dee_Utf32Char *)argpval,&spec)
           ) != 0) goto err;
        break;
       default:
        if (DeeObject_GetPointerEx(arg,DeeObject_TYPE(char),&argpval) != 0) goto err;
        if DEE_UNLIKELY((ch != DEE_CHAR_C('q') ? DeeStringWriter_F(SpecWriteStringWithLength)(&writer,spec.has_precision ? Dee_StrNLen((char *)argpval,spec.precision) : Dee_StrLen((char *)argpval),(char *)argpval,&spec)
                                               : DeeStringWriter_F(SpecWriteQuotedStringWithLength)(&writer,spec.has_precision ? Dee_StrNLen((char *)argpval,spec.precision) : Dee_StrLen((char *)argpval),(char *)argpval,&spec)
           ) != 0) goto err;
        break;
      }
#else
      DeeError_TypeError_UnexpectedType(arg,&DEE_STRING_Type);
      goto err;
#endif
     }
     break;
    case DEE_CHAR_C('O'): case DEE_CHAR_C('k'): case DEE_CHAR_C('K'):
    case DEE_CHAR_C('r'): case DEE_CHAR_C('R'): {
     int temp; DeeObject *ob;
     GET_ARG();
     if DEE_UNLIKELY((ob = (ch == DEE_CHAR_C('r') || ch == DEE_CHAR_C('R'))
      ? DeeObject_Repr(arg) : DeeObject_Str(arg)) == NULL) goto err;
     temp = DeeStringWriter_F(DEE_PP_CAT_3(SpecWrite,DEE_CONFIG_DEFAULT_ENCODING,StringWithLength))(
      &writer,DeeString_SIZE(ob),DeeString_STR(ob),&spec);
     Dee_DECREF(ob);
     if DEE_UNLIKELY(temp != 0) goto err;
    } break;
    default: break;
   }
//end_fmt:
   flush_start = fmt;
   if (fmt >= fmt_end) break;
  }
 }
 // Flush remainder
 if DEE_UNLIKELY(flush_start < fmt && DeeStringWriter_F(DEE_PP_CAT_3(Write,ENC,StringWithLength))(
  &writer,(Dee_size_t)((fmt-flush_start)-1),flush_start) != 0) goto err;
 result = DEE_STRINGWRITER_Pack(&writer);
end:
 DEE_STRINGWRITER_Quit(&writer);
 return result;
err:
 result = NULL;
 goto end;
#undef GET_ARG
no_args:
 DeeError_SET_STRING(&DeeErrorType_ValueError,
                     "Not enough arguments");
 goto err;
}
DEE_COMPILER_MSVC_WARNING_POP


















//////////////////////////////////////////////////////////////////////////
// string operators

static DeeObject *_deestring_F(tp_new)(DeeTypeObject *DEE_UNUSED(tp)) { DEERETURN_EMPTYSTRING; }
static DeeObject *_deestring_F(tp_copy_new)(DeeTypeObject *DEE_UNUSED(tp), DeeObject *self) { DeeReturn_NEWREF(self); }
static DeeObject *_deestring_F(tp_any_new)(
 DeeTypeObject *DEE_UNUSED(tp), DeeTupleObject *args) {
 DeeTypeObject *ob_type; DeeObject *ob;
 if DEE_UNLIKELY(DeeTuple_Unpack((DeeObject *)args,"o:" DEE_STRING_TYPE_NAME,&ob) != 0) return NULL;
 DEE_ASSERT(DeeObject_Check(ob));
 ob_type = Dee_TYPE(ob);
 if (ob_type == &DEE_STRING_Type) DeeReturn_NEWREF(ob);
#if DEE_CONFIG_HAVE_ENCODING_UTF8 && \
    DEE_STRING_ENCODING(ENC) != DEE_STRING_ENCODING(Utf8)
 if (ob_type == &DeeUtf8String_Type) return DeeString_F(
  FromUtf8StringWithLength)(DeeUtf8String_SIZE(ob),DeeUtf8String_STR(ob));
#endif
#if DEE_CONFIG_HAVE_ENCODING_WIDE && \
    DEE_STRING_ENCODING(ENC) != DEE_STRING_ENCODING(Wide)
 if (ob_type == &DeeWideString_Type) return DeeString_F(
  FromWideStringWithLength)(DeeWideString_SIZE(ob),DeeWideString_STR(ob));
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16 && \
    DEE_STRING_ENCODING(ENC) != DEE_STRING_ENCODING(Utf16)
 if (ob_type == &DeeUtf16String_Type) return DeeString_F(
  FromUtf16StringWithLength)(DeeUtf16String_SIZE(ob),DeeUtf16String_STR(ob));
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32 && \
    DEE_STRING_ENCODING(ENC) != DEE_STRING_ENCODING(Utf32)
 if (ob_type == &DeeUtf32String_Type) return DeeString_F(
  FromUtf32StringWithLength)(DeeUtf32String_SIZE(ob),DeeUtf32String_STR(ob));
#endif
 return DEE_PP_CAT_3(DeeObject_,ENC,Str)(ob);
}
DeeObject *_deestring_F(tp_mul)(DEE_STRINGOBJECT *self, DeeObject *i) {
 Dee_ssize_t ii;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self) && DeeObject_Check(i));
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return NULL;
 if DEE_UNLIKELY(ii <= 0) DEERETURN_EMPTYSTRING;
 return DeeString_F(Repeat)((DeeObject *)self,(Dee_size_t)ii);
}

static DeeObject *_deestring_F(tp_cmp_lo)(DEE_STRINGOBJECT *lhs, DeeObject *rhs) {
 DEE_ASSERT(DeeObject_Check(lhs) && DEE_STRING_Check(lhs));
 DEE_ASSERT(DeeObject_Check(rhs));
 if DEE_LIKELY(DEE_STRING_Check(rhs)) {
  DeeReturn_Bool(Dee_F(StrCmpLen)(DEE_STRING_SIZE(lhs),DEE_STRING_STR(lhs),
                                  DEE_STRING_SIZE(rhs),DEE_STRING_STR(rhs)
                                  ) < 0);
 } else if DEE_LIKELY(DEE_STRING_SIZE(lhs) == 1) {
  DEE_CHAR temp;
  if DEE_UNLIKELY(DeeObject_Cast(DEE_CHAR,rhs,&temp) != 0) return NULL;
  DeeReturn_Bool(*DEE_STRING_STR(lhs) < temp);
 } else {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Not implemented: string.__lo__(%s)",
                      DeeType_NAME(Dee_TYPE(rhs)));
  return NULL;
 }
}
static DeeObject *_deestring_F(tp_cmp_le)(DEE_STRINGOBJECT *lhs, DeeObject *rhs) {
 DEE_ASSERT(DeeObject_Check(lhs) && DEE_STRING_Check(lhs));
 DEE_ASSERT(DeeObject_Check(rhs));
 if DEE_LIKELY(DEE_STRING_Check(rhs)) {
  DeeReturn_Bool(Dee_F(StrCmpLen)(DEE_STRING_SIZE(lhs),DEE_STRING_STR(lhs),
                                  DEE_STRING_SIZE(rhs),DEE_STRING_STR(rhs)
                                  ) <= 0);
 } else if DEE_LIKELY(DEE_STRING_SIZE(lhs) == 1) {
  DEE_CHAR temp;
  if DEE_UNLIKELY(DeeObject_Cast(DEE_CHAR,rhs,&temp) != 0) return NULL;
  DeeReturn_Bool(*DEE_STRING_STR(lhs) <= temp);
 } else {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Not implemented: string.__le__(%s)",
                      DeeType_NAME(Dee_TYPE(rhs)));
  return NULL;
 }
}
static DeeObject *_deestring_F(tp_cmp_eq)(DEE_STRINGOBJECT *lhs, DeeObject *rhs) {
 DEE_ASSERT(DeeObject_Check(lhs) && DEE_STRING_Check(lhs));
 DEE_ASSERT(DeeObject_Check(rhs));
 if DEE_LIKELY(DEE_STRING_Check(rhs)) {
  DeeReturn_Bool(Dee_F(StrCmpLen)(DEE_STRING_SIZE(lhs),DEE_STRING_STR(lhs),
                                  DEE_STRING_SIZE(rhs),DEE_STRING_STR(rhs)
                                  ) == 0);
 } else if DEE_LIKELY(DEE_STRING_SIZE(lhs) == 1) {
  DEE_CHAR temp;
  if DEE_UNLIKELY(DeeObject_Cast(DEE_CHAR,rhs,&temp) != 0) return NULL;
  DeeReturn_Bool(*DEE_STRING_STR(lhs) == temp);
 } else {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Not implemented: string.__eq__(%s)",
                      DeeType_NAME(Dee_TYPE(rhs)));
  return NULL;
 }
}
static DeeObject *_deestring_F(tp_cmp_ne)(DEE_STRINGOBJECT *lhs, DeeObject *rhs) {
 DEE_ASSERT(DeeObject_Check(lhs) && DEE_STRING_Check(lhs));
 DEE_ASSERT(DeeObject_Check(rhs));
 if DEE_LIKELY(DEE_STRING_Check(rhs)) {
  DeeReturn_Bool(Dee_F(StrCmpLen)(DEE_STRING_SIZE(lhs),DEE_STRING_STR(lhs),
                                  DEE_STRING_SIZE(rhs),DEE_STRING_STR(rhs)
                                  ) != 0);
 } else if DEE_LIKELY(DEE_STRING_SIZE(lhs) == 1) {
  DEE_CHAR temp;
  if DEE_UNLIKELY(DeeObject_Cast(DEE_CHAR,rhs,&temp) != 0) return NULL;
  DeeReturn_Bool(*DEE_STRING_STR(lhs) != temp);
 } else {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Not implemented: string.__ne__(%s)",
                      DeeType_NAME(Dee_TYPE(rhs)));
  return NULL;
 }
}
static DeeObject *_deestring_F(tp_cmp_gr)(DEE_STRINGOBJECT *lhs, DeeObject *rhs) {
 DEE_ASSERT(DeeObject_Check(lhs) && DEE_STRING_Check(lhs));
 DEE_ASSERT(DeeObject_Check(rhs));
 if DEE_LIKELY(DEE_STRING_Check(rhs)) {
  DeeReturn_Bool(Dee_F(StrCmpLen)(DEE_STRING_SIZE(lhs),DEE_STRING_STR(lhs),
                                  DEE_STRING_SIZE(rhs),DEE_STRING_STR(rhs)
                                  ) > 0);
 } else if DEE_LIKELY(DEE_STRING_SIZE(lhs) == 1) {
  DEE_CHAR temp;
  if DEE_UNLIKELY(DeeObject_Cast(DEE_CHAR,rhs,&temp) != 0) return NULL;
  DeeReturn_Bool(*DEE_STRING_STR(lhs) > temp);
 } else {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Not implemented: string.__gr__(%s)",
                      DeeType_NAME(Dee_TYPE(rhs)));
  return NULL;
 }
}
static DeeObject *_deestring_F(tp_cmp_ge)(DEE_STRINGOBJECT *lhs, DeeObject *rhs) {
 DEE_ASSERT(DeeObject_Check(lhs) && DEE_STRING_Check(lhs));
 DEE_ASSERT(DeeObject_Check(rhs));
 if DEE_LIKELY(DEE_STRING_Check(rhs)) {
  DeeReturn_Bool(Dee_F(StrCmpLen)(DEE_STRING_SIZE(lhs),DEE_STRING_STR(lhs),
                                  DEE_STRING_SIZE(rhs),DEE_STRING_STR(rhs)
                                  ) >= 0);
 } else if DEE_LIKELY(DEE_STRING_SIZE(lhs) == 1) {
  DEE_CHAR temp;
  if DEE_UNLIKELY(DeeObject_Cast(DEE_CHAR,rhs,&temp) != 0) return NULL;
  DeeReturn_Bool(*DEE_STRING_STR(lhs) >= temp);
 } else {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Not implemented: string.__ge__(%s)",
                      DeeType_NAME(Dee_TYPE(rhs)));
  return NULL;
 }
}
static DeeObject *_deestring_F(tp_add)(DEE_STRINGOBJECT *lhs, DeeObject *rhs) {
 DeeObject *result,*right;
 if DEE_UNLIKELY((right = DEE_PP_CAT_3(DeeObject_,ENC,Str)(rhs)) == NULL) return NULL;
 result = DeeString_F(ConcatWithLength)((DeeObject *)lhs,DEE_STRING_SIZE(right),DEE_STRING_STR(right));
 Dee_DECREF(right);
 return result;
}
static DeeObject *_deestring_F(tp_mod)(DEE_STRINGOBJECT *self, DeeObject *args) {
 // NOTE: We accept non-tuples in args, because the compiler
 //       would not create a tuple in cases like: '"%x" % (42)'
 if DEE_LIKELY(DeeTuple_Check(args)) {
  return DeeString_F(CFormat)((DeeObject *)self,args);
 } else {
  // A little bit of hacking so we don't have to allocate a real tuple
  struct {
   DEE_OBJECT_HEAD
   Dee_size_t elemc;
   DeeObject *arg0;
  } fake_tuple = {DEE_OBJECT_HEAD_INIT(&DeeTuple_Type),1};
  fake_tuple.arg0 = args;
  return DeeString_F(CFormat)((DeeObject *)self,(DeeObject *)&fake_tuple);
 }
}
static DeeObject *_deestring_F(tp_seq_get)(DEE_STRINGOBJECT *self, DeeObject *i) {
 Dee_ssize_t ii; Dee_size_t size;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self) && DeeObject_Check(i));
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return NULL;
 if DEE_UNLIKELY((size = DEE_STRING_SIZE(self)) == 0) {
  DeeError_ValueError_EmptyList(&DEE_STRING_Type);
  return NULL;
 }
 DEE_PRIVATE_CLAMP_INDEX(ii,size);
#if DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(Utf8)
 DeeReturn_Utf8Character(DEE_STRING_STR(self)[ii]);
#else
 return DeeString_F(NewWithLength)(1,DEE_STRING_STR(self)+ii);
#endif
}
static DeeObject *_deestring_F(tp_seq_size)(DEE_STRINGOBJECT *self) {
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 return DeeObject_New(Dee_size_t,DEE_STRING_SIZE(self));
}
static DeeObject *_deestring_F(tp_seq_contains)(DEE_STRINGOBJECT *self, DeeObject *substr) {
 int result;
 if DEE_UNLIKELY((substr = DeeString_F(Cast)(substr)) == NULL) return NULL;
 result = DeeString_F(Contains)((DeeObject *)self,
  DEE_STRING_STR(substr),DEE_STRING_SIZE(substr));
 Dee_DECREF(substr);
 DeeReturn_Bool(result);
}
static DeeObject *_deestring_F(tp_seq_range_get)(DEE_STRINGOBJECT *self, DeeObject *lo, DeeObject *hi) {
 Dee_ssize_t loi,hii; Dee_size_t size;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 DEE_ASSERT(DeeObject_Check(lo) && DeeObject_Check(hi));
 size = DEE_STRING_SIZE(self);
 if (lo != Dee_None) { if (DeeObject_Cast(Dee_ssize_t,lo,&loi) != 0) return NULL; } else loi = 0;
 if (hi != Dee_None) { if (DeeObject_Cast(Dee_ssize_t,hi,&hii) != 0) return NULL; } else hii = (Dee_ssize_t)size;
 if DEE_UNLIKELY(!size) DEERETURN_EMPTYSTRING;
 DEE_PRIVATE_CLAMP_RANGE(loi,hii,size);
 if DEE_UNLIKELY(hii <= loi) DEERETURN_EMPTYSTRING;
 return DeeString_F(NewWithLength)((Dee_size_t)(hii-loi),DEE_STRING_STR(self)+loi);
}
static int _deestring_F(tp_bool)(DEE_STRINGOBJECT *self) {
 return DEE_STRING_SIZE(self) != 0;
}
static DeeObject *_deestring_F(tp_not)(DEE_STRINGOBJECT *self) {
 DeeReturn_Bool(DEE_STRING_SIZE(self) == 0);
}
static int _deestring_F(tp_hash)(DEE_STRINGOBJECT *self, Dee_hash_t start, Dee_hash_t *result) {
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 *result = DeeHash_Memory(DEE_STRING_STR(self),DEE_STRING_SIZE(self)*sizeof(DEE_CHAR),start);
 return 0;
}
#if DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING) && \
    DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(Utf8)
#define _deeUtf8string_tp_str _DeeGeneric_ReturnSelf
#elif DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING) && \
      DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(Wide)
#define _deeWidestring_tp_str _DeeGeneric_ReturnSelf
#elif DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING) && \
      DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(Utf16)
#define _deeUtf16string_tp_str _DeeGeneric_ReturnSelf
#elif DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING) && \
      DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(Utf32)
#define _deeUtf32string_tp_str _DeeGeneric_ReturnSelf
#else
static DeeObject *_deestring_F(tp_str)(DEE_STRINGOBJECT *self) {
 return DEE_PP_CAT_3(DeeUtf8String_From,ENC,StringWithLength)(
  DEE_STRING_SIZE(self),DEE_STRING_STR(self));
}
#endif
static int _deestring_F(tp_int32)(DEE_STRINGOBJECT *self, Dee_int32_t *result) {
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 if DEE_UNLIKELY(DEE_STRING_SIZE(self) != 1) return (*DeeType_GET_SLOT(&DeeObject_Type,tp_int32))((DeeObject *)self,result);
 *result = (Dee_int32_t)*DEE_STRING_STR(self);
 return 0;
}
static int _deestring_F(tp_int64)(DEE_STRINGOBJECT *self, Dee_int64_t *result) {
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 if DEE_UNLIKELY(DEE_STRING_SIZE(self) != 1) return (*DeeType_GET_SLOT(&DeeObject_Type,tp_int64))((DeeObject *)self,result);
 *result = (Dee_int64_t)*DEE_STRING_STR(self);
 return 0;
}
static int _deestring_F(tp_double)(DEE_STRINGOBJECT *self, double *result) {
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 if DEE_UNLIKELY(DEE_STRING_SIZE(self) != 1) return (*DeeType_GET_SLOT(&DeeObject_Type,tp_double))((DeeObject *)self,result);
 *result = (double)*DEE_STRING_STR(self);
 return 0;
}

//////////////////////////////////////////////////////////////////////////
// String Iterator
static void _deestringiterator_F(tp_dtor)(DEE_STRINGITERATOROBJECT *self) {
 Dee_DECREF(self->si_string);
}
static int _deestringiterator_F(tp_ctor)(
 DeeTypeObject *DEE_UNUSED(tp_self), DEE_STRINGITERATOROBJECT *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStringIterator_F(Check)(self));
 Dee_INCREF(self->si_string = (DEE_STRINGOBJECT *)DEE_EMPTYSTRING);
 self->si_end = self->si_pos = DEE_STRING_STR(DEE_EMPTYSTRING);
 return 0;
}
static int _deestringiterator_F(tp_any_ctor)(
 DeeTypeObject *DEE_UNUSED(tp_self), DEE_STRINGITERATOROBJECT *self, DeeObject *args) {
 DeeObject *str_;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:string.iterator",&str_) != 0) return -1;
 if DEE_UNLIKELY((self->si_string = (DEE_STRINGOBJECT *)DeeString_F(Cast)(str_)) == NULL) return -1;
 self->si_end = (self->si_pos = DEE_STRING_STR(str_))+DEE_STRING_SIZE(str_);
 return 0;
}
static int _deestringiterator_F(tp_copy_ctor)(
 DeeTypeObject *DEE_UNUSED(tp_self), DEE_STRINGITERATOROBJECT *self, DEE_STRINGITERATOROBJECT *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStringIterator_F(Check)(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeStringIterator_F(Check)(right));
 Dee_INCREF(self->si_string = right->si_string);
 self->si_pos = right->si_pos;
 self->si_end = right->si_end;
 return 0;
}

#define DEE_PRIVATE_MAKE_STRINGITERATOR_COMPARE(name,op)\
static DeeObject *name(DEE_STRINGITERATOROBJECT *self, DEE_STRINGITERATOROBJECT *right) {\
 DEE_ASSERT(DeeObject_Check(self) && DeeStringIterator_F(Check)(self));\
 DEE_ASSERT(DeeObject_Check(right));\
 if DEE_UNLIKELY((right = (DEE_STRINGITERATOROBJECT *)DeeObject_GetInstance((\
  DeeObject *)right,&DeeStringIterator_F(Type))) == NULL) return NULL;\
 DeeReturn_Bool(DeeAtomicPtr_Load(self->si_pos,memory_order_seq_cst) op \
                DeeAtomicPtr_Load(right->si_pos,memory_order_seq_cst));\
}
DEE_PRIVATE_MAKE_STRINGITERATOR_COMPARE(_deestringiterator_F(tp_cmp_lo),<);
DEE_PRIVATE_MAKE_STRINGITERATOR_COMPARE(_deestringiterator_F(tp_cmp_le),<=);
DEE_PRIVATE_MAKE_STRINGITERATOR_COMPARE(_deestringiterator_F(tp_cmp_eq),==);
DEE_PRIVATE_MAKE_STRINGITERATOR_COMPARE(_deestringiterator_F(tp_cmp_ne),!=);
DEE_PRIVATE_MAKE_STRINGITERATOR_COMPARE(_deestringiterator_F(tp_cmp_gr),>);
DEE_PRIVATE_MAKE_STRINGITERATOR_COMPARE(_deestringiterator_F(tp_cmp_ge),>=);
#undef DEE_PRIVATE_MAKE_STRINGITERATOR_COMPARE







//////////////////////////////////////////////////////////////////////////
// string member functions

// Characters from "DEE_CH_IS_SPACE"
static struct {
 DEE_OBJECT_HEAD
 Dee_size_t s_len;
 DEE_CHAR   s_str[sizeof(DEE_EMPTY_CHARS_STR)];
} _deestring_F(emptychars) = {DEE_OBJECT_HEAD_INIT(&DEE_STRING_Type),(
 sizeof(DEE_EMPTY_CHARS_STR)/sizeof(DEE_CHAR))-1,DEE_EMPTY_CHARS};

static DeeObject *_deestring_F(substr)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t size; Dee_ssize_t pos,len;
 DEE_ASSERT(DeeObject_Check(self) && DEE_STRING_Check(self));
 if DEE_UNLIKELY((size = DEE_STRING_SIZE(self)) == 0) DEERETURN_EMPTYSTRING;
 pos = 0,len = (Dee_ssize_t)size;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|IdId:substr",&pos,&len) != 0) return NULL;
 DEE_PRIVATE_CLAMP_INDEX(pos,size);
 if (len > (Dee_ssize_t)(size-(Dee_size_t)pos)) len = (Dee_ssize_t)(size-(Dee_size_t)pos);
 if (len <= 0) DEERETURN_EMPTYSTRING;
 return DeeString_F(NewWithLength)((Dee_size_t)len,DEE_STRING_STR(self)+pos);
}
static DeeObject *_deestring_F(compare)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *temp; int result; Dee_size_t size;
 Dee_ssize_t pos1,pos2,len1,len2;
 switch (DeeTuple_SIZE(args)) {
  case 1:
   //int compare (const string& str_) const;
   if DEE_UNLIKELY((temp = DeeString_F(Cast)(DeeTuple_GET(args,0))) == NULL) return NULL;
   result = Dee_F(StrCmpLen)(DEE_STRING_SIZE(self),DEE_STRING_STR(self),
                             DEE_STRING_SIZE(temp),DEE_STRING_STR(temp));
   Dee_DECREF(temp);
   break;
  case 3:
   if DEE_UNLIKELY(DeeTuple_Unpack(args,"IdIdo:compare",&pos1,&len1,&temp) != 0) return NULL;
   if DEE_UNLIKELY((temp = DeeString_F(Cast)(temp)) == NULL) return NULL;
   size = DEE_STRING_SIZE(self);
   DEE_PRIVATE_CLAMP_INDEX(pos1,size);
   if (len1 > (Dee_ssize_t)(size-(Dee_size_t)pos1)) len1 = (Dee_ssize_t)(size-(Dee_size_t)pos1);
   result = Dee_F(StrCmpLen)((Dee_size_t)(len1 <= 0 ? 0 : len1),
                             DEE_STRING_STR(self)+pos1,
                             DEE_STRING_SIZE(temp),
                             DEE_STRING_STR(temp));
   Dee_DECREF(temp);
   break;
  case 5:
   if DEE_UNLIKELY(DeeTuple_Unpack(args,"IdIdoIdId:compare",&pos1,&len1,&temp,&pos2,&len2) != 0) return NULL;
   if DEE_UNLIKELY((temp = DeeString_F(Cast)(temp)) == NULL) return NULL;
   size = DEE_STRING_SIZE(self);
   DEE_PRIVATE_CLAMP_INDEX(pos1,size);
   if (len1 > (Dee_ssize_t)(size-(Dee_size_t)pos1)) len1 = (Dee_ssize_t)(size-(Dee_size_t)pos1);
   size = DEE_STRING_SIZE(temp);
   DEE_PRIVATE_CLAMP_INDEX(pos2,size);
   if (len2 > (Dee_ssize_t)(size-(Dee_size_t)pos2)) len2 = (Dee_ssize_t)(size-(Dee_size_t)pos2);
   result = Dee_F(StrCmpLen)(
    (Dee_size_t)(len1 <= 0 ? 0 : len1),DEE_STRING_STR(self)+pos1,
    (Dee_size_t)(len2 <= 0 ? 0 : len2),DEE_STRING_STR(temp)+pos2);
   Dee_DECREF(temp);
   break;
  default:
   DeeError_SetStringf(&DeeErrorType_TypeError,
                       "\"compare\" requires 1,3 or 5 argument(s) (%Iu given)",
                       DeeTuple_SIZE(args));
   return NULL;
 }
 return DeeObject_New(Dee_rt_int,(Dee_rt_int)result);
}
static DeeObject *_deestring_F(ord)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":ord") != 0) return NULL;
 if DEE_UNLIKELY(DEE_STRING_SIZE(self) != 1) {
  DeeError_SET_STRING(&DeeErrorType_ValueError,"Expected a one-character string");
  return NULL;
 }
 return DeeObject_New(DEE_CHAR,*DEE_STRING_STR(self));
}
static DeeObject *_deestring_F(capitalize)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":capitalize") != 0) return NULL;
 return DeeString_F(Capitalize)(self);
}
static DeeObject *_deestring_F(center)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t width; DEE_CHAR fill_char = ' ';
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu|c:center",&width,&fill_char) != 0) return NULL;
 return DeeString_F(Center)(self,width,fill_char);
}
static DeeObject *_deestring_F(count)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t result,size = DEE_STRING_SIZE(self);
 DeeObject *sub; Dee_ssize_t begin = 0,end = (Dee_ssize_t)size;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|IdId:count",&sub,&begin,&end) != 0) return NULL;
 if DEE_UNLIKELY((sub = DeeString_F(Cast)(sub)) == NULL) return NULL;
 if DEE_UNLIKELY(!size) {st0: result = 0; goto end; }
 DEE_PRIVATE_CLAMP_RANGE(begin,end,size);
 if DEE_UNLIKELY(begin >= end) goto st0;
 result = DeeString_F(Count)(self,sub,(Dee_size_t)begin,(Dee_size_t)end);
end: Dee_DECREF(sub);
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *_deestring_F(endswith)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t size = DEE_STRING_SIZE(self);
 DeeObject *sub; int result; Dee_ssize_t begin = 0,end = (Dee_ssize_t)size;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|IdId:endswith",&sub,&begin,&end) != 0) return NULL;
 if DEE_UNLIKELY((sub = DeeString_F(Cast)(sub)) == NULL) return NULL;
 if DEE_UNLIKELY(!size) { result = 0; goto end_sub; }
 DEE_PRIVATE_CLAMP_RANGE(begin,end,size);
 if DEE_UNLIKELY(begin >= end) { result = 0; goto end_sub; }
 result = DeeString_F(EndsWith)(self,sub,(Dee_size_t)begin,(Dee_size_t)end);
end_sub: Dee_DECREF(sub);
 DeeReturn_Bool(result);
}
static DeeObject *_deestring_F(expandtabs)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t tabsize = 8;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|Iu:expandtabs",&tabsize) != 0) return NULL;
 return DeeString_F(ExpandTabs)(self,tabsize);
}
static DeeObject *_deestring_F(empty)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":empty") != 0) return NULL;
 DeeReturn_Bool(DEE_STRING_SIZE(self) == 0);
}
static DeeObject *_deestring_F(non_empty)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":non_empty") != 0) return NULL;
 DeeReturn_Bool(DEE_STRING_SIZE(self) != 0);
}
static DeeObject *_deestring_F(find)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t size = DEE_STRING_SIZE(self);
 DeeObject *sub; Dee_ssize_t result,begin = 0,end = (Dee_ssize_t)size;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|IdId:find",&sub,&begin,&end) != 0) return NULL;
 if DEE_UNLIKELY((sub = DeeString_F(Cast)(sub)) == NULL) return NULL;
 if DEE_UNLIKELY(!size) goto ret_m1;
 DEE_PRIVATE_CLAMP_RANGE(begin,end,size);
 if DEE_UNLIKELY(begin >= end) {ret_m1: Dee_DECREF(sub); return DeeObject_New(Dee_ssize_t,-1); }
 result = DeeString_F(Find)(self,sub,(Dee_size_t)begin,(Dee_size_t)end);
 Dee_DECREF(sub);
 return DeeObject_New(Dee_ssize_t,result);
}
static DeeObject *_deestring_F(format)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *data;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:format",&data) != 0) return NULL;
 return DeeString_F(Format)(self,data);
}
static DeeObject *_deestring_F(index)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t result,size = DEE_STRING_SIZE(self);
 DeeObject *sub; Dee_ssize_t begin = 0,end = (Dee_ssize_t)size;
 if (DeeTuple_Unpack(args,"o|IdId:index",&sub,&begin,&end) != 0) return NULL;
 if DEE_UNLIKELY((sub = DeeString_F(Cast)(sub)) == NULL) return NULL;
 if DEE_UNLIKELY(!size) goto not_found;
 DEE_PRIVATE_CLAMP_RANGE(begin,end,size);
 if DEE_UNLIKELY(begin >= end) {
not_found: Dee_DECREF(sub);
  DeeError_ValueError_NotFound((DeeObject *)self,(DeeObject *)sub);
  return NULL;
 }
 result = DeeString_F(Index)(self,sub,(Dee_size_t)begin,(Dee_size_t)end);
 Dee_DECREF(sub); 
 if DEE_UNLIKELY(result == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
static DeeObject *_deestring_F(isalnum)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":isalnum") != 0) return NULL;
 DeeReturn_Bool(DeeString_F(IsAlnum)(self));
}
static DeeObject *_deestring_F(isalpha)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":isalpha") != 0) return NULL;
 DeeReturn_Bool(DeeString_F(IsAlpha)(self));
}
static DeeObject *_deestring_F(isdigit)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":isdigit") != 0) return NULL;
 DeeReturn_Bool(DeeString_F(IsDigit)(self));
}
static DeeObject *_deestring_F(islower)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":islower") != 0) return NULL;
 DeeReturn_Bool(DeeString_F(IsLower)(self));
}
static DeeObject *_deestring_F(isspace)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":isspace") != 0) return NULL;
 DeeReturn_Bool(DeeString_F(IsSpace)(self));
}
static DeeObject *_deestring_F(istitle)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":istitle") != 0) return NULL;
 DeeReturn_Bool(DeeString_F(IsTitle)(self));
}
static DeeObject *_deestring_F(isupper)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":isupper") != 0) return NULL;
 DeeReturn_Bool(DeeString_F(IsUpper)(self));
}
static DeeObject *_deestring_F(join)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *seq;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:join",&seq) != 0) return NULL;
 return DeeString_F(Join)(self,seq);
}
static DeeObject *_deestring_F(ljust)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t width; DEE_CHAR fill_char = ' ';
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu|c:ljust",&width,&fill_char) != 0) return NULL;
 return DeeString_F(LJust)(self,width,fill_char);
}
static DeeObject *_deestring_F(reversed)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":reversed") != 0) return NULL;
 return DeeString_F(Reverse)(self);
}
static DeeObject *_deestring_F(lower)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":lower") != 0) return NULL;
 return DeeString_F(Lower)(self);
}
static DeeObject *_deestring_F(lstrip)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*chars = (DeeObject *)&_deestring_F(emptychars);
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:lstrip",&chars) != 0) return NULL;
 if DEE_UNLIKELY((chars = DeeString_F(Cast)(chars)) == NULL) return NULL;
 result = DeeString_F(LStrip)(self,chars);
 Dee_DECREF(chars);
 return result;
}
static DeeObject *_deestring_F(partition)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*sep;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:partition",&sep) != 0) return NULL;
 if DEE_UNLIKELY((sep = DeeString_F(Cast)(sep)) == NULL) return NULL;
 result = DeeString_F(Partition)(self,sep);
 Dee_DECREF(sep);
 return result;
}
static DeeObject *_deestring_F(replace)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *find,*replace,*result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:replace",&find,&replace) != 0) return NULL;
 if DEE_UNLIKELY((find = DeeString_F(Cast)(find)) == NULL) return NULL;
 if DEE_UNLIKELY((replace = DeeString_F(Cast)(replace)) == NULL) { result = NULL; goto end_0; }
 result = DeeString_F(Replace)(self,find,replace);
 Dee_DECREF(replace);
end_0: Dee_DECREF(find);
 return result;
}
static DeeObject *_deestring_F(rfind)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t size = DEE_STRING_SIZE(self);
 DeeObject *sub; Dee_ssize_t result,begin = 0,end = (Dee_ssize_t)size;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|IdId:rfind",&sub,&begin,&end) != 0) return NULL;
 if DEE_UNLIKELY((sub = DeeString_F(Cast)(sub)) == NULL) return NULL;
 if DEE_UNLIKELY(!size) goto ret_m1;
 DEE_PRIVATE_CLAMP_RANGE(begin,end,size);
 if DEE_UNLIKELY(begin >= end) {ret_m1: Dee_DECREF(sub); return DeeObject_New(Dee_ssize_t,-1); }
 result = DeeString_F(RFind)(self,sub,(Dee_size_t)begin,(Dee_size_t)end);
 Dee_DECREF(sub);
 return DeeObject_New(Dee_ssize_t,result);
}
static DeeObject *_deestring_F(rindex)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t result,size = DEE_STRING_SIZE(self);
 DeeObject *sub; Dee_ssize_t begin = 0,end = (Dee_ssize_t)size;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|IdId:rindex",&sub,&begin,&end) != 0) return NULL;
 if DEE_UNLIKELY((sub = DeeString_F(Cast)(sub)) == NULL) return NULL;
 if DEE_UNLIKELY(!size) goto not_found;
 DEE_PRIVATE_CLAMP_RANGE(begin,end,size);
 if DEE_UNLIKELY(begin >= end) {
not_found: Dee_DECREF(sub);
  DeeError_ValueError_NotFound((DeeObject *)self,(DeeObject *)sub);
  return NULL;
 }
 result = DeeString_F(RIndex)(self,sub,(Dee_size_t)begin,(Dee_size_t)end);
 Dee_DECREF(sub); 
 return result == (Dee_size_t)-1 ? NULL : DeeObject_New(Dee_size_t,(Dee_size_t)result);
}
static DeeObject *_deestring_F(rjust)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t width; DEE_CHAR fill_char = ' ';
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu|c:rjust",&width,&fill_char) != 0) return NULL;
 return DeeString_F(RJust)(self,width,fill_char);
}
static DeeObject *_deestring_F(rpartition)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*sep;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:rpartition",&sep) != 0) return NULL;
 if DEE_UNLIKELY((sep = DeeString_F(Cast)(sep)) == NULL) return NULL;
 result = DeeString_F(RPartition)(self,sep);
 Dee_DECREF(sep);
 return result;
}
static DeeObject *_deestring_F(rsplit)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *list;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:rsplit",&list) != 0) return NULL;
 return DeeString_F(RSplit)(self,list);
}
static DeeObject *_deestring_F(rstrip)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*chars = (DeeObject *)&_deestring_F(emptychars);
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:rstrip",&chars) != 0) return NULL;
 if DEE_UNLIKELY((chars = DeeString_F(Cast)(chars)) == NULL) return NULL;
 result = DeeString_F(RStrip)(self,chars);
 Dee_DECREF(chars);
 return result;
}
static DeeObject *_deestring_F(split)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *sep,*result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:split",&sep) != 0) return NULL;
 if DEE_UNLIKELY((sep = DeeString_F(Cast)(sep)) == NULL) return NULL;
 result = DeeString_F(Split)(self,sep);
 Dee_DECREF(sep);
 return result;
}
static DeeObject *_deestring_F(splitlines)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int keepends = 0;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|b:splitlines",&keepends) != 0) return NULL;
 return DeeString_F(SplitLines)(self,keepends);
}
static DeeObject *_deestring_F(startswith)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t size = DEE_STRING_SIZE(self);
 DeeObject *sub; int result; Dee_ssize_t begin = 0,end = (Dee_ssize_t)size;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|IdId:startswith",&sub,&begin,&end) != 0) return NULL;
 if DEE_UNLIKELY((sub = DeeString_F(Cast)(sub)) == NULL) return NULL;
 if DEE_UNLIKELY(!size) { result = 0; goto end_sub; }
 DEE_PRIVATE_CLAMP_RANGE(begin,end,size);
 if DEE_UNLIKELY(begin >= end) { result = 0; goto end_sub; }
 result = DeeString_F(StartsWith)(self,sub,(Dee_size_t)begin,(Dee_size_t)end);
end_sub: Dee_DECREF(sub);
 DeeReturn_Bool(result);
}
static DeeObject *_deestring_F(strip)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*chars = (DeeObject *)&_deestring_F(emptychars);
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:strip",&chars) != 0) return NULL;
 if DEE_UNLIKELY((chars = DeeString_F(Cast)(chars)) == NULL) return NULL;
 result = DeeString_F(Strip)(self,chars);
 Dee_DECREF(chars);
 return result;
}
static DeeObject *_deestring_F(swapcase)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":swapcase") != 0) return NULL;
 return DeeString_F(SwapCase)(self);
}
static DeeObject *_deestring_F(title)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":title") != 0) return NULL;
 return DeeString_F(Title)(self);
}
static DeeObject *_deestring_F(upper)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":upper") != 0) return NULL;
 return DeeString_F(Upper)(self);
}
static DeeObject *_deestring_F(zfill)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t width;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu:zfill",&width) != 0) return NULL;
 return DeeString_F(ZFill)(self,width);
}
static DeeObject *_deestring_F(sstrip)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *sub,*result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:sstrip",&sub) != 0) return NULL;
 if DEE_UNLIKELY((sub = DeeString_F(Cast)(sub)) == NULL) return NULL;
 result = DeeString_F(SStrip)(self,sub);
 Dee_DECREF(sub);
 return result;
}
static DeeObject *_deestring_F(lsstrip)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *sub,*result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:lsstrip",&sub) != 0) return NULL;
 if DEE_UNLIKELY((sub = DeeString_F(Cast)(sub)) == NULL) return NULL;
 result = DeeString_F(LSStrip)(self,sub);
 Dee_DECREF(sub);
 return result;
}
static DeeObject *_deestring_F(rsstrip)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *sub,*result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:rsstrip",&sub) != 0) return NULL;
 if DEE_UNLIKELY((sub = DeeString_F(Cast)(sub)) == NULL) return NULL;
 result = DeeString_F(RSStrip)(self,sub);
 Dee_DECREF(sub);
 return result;
}
static DeeObject *_deestring_F(wmatch)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pattern; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:wmatch",&pattern) != 0) return NULL;
 if DEE_UNLIKELY((pattern = DeeString_F(Cast)(pattern)) == NULL) return NULL;
 result = Dee_F(StrWMatchLen)(
  DEE_STRING_SIZE(self),DEE_STRING_STR(self),
  DEE_STRING_SIZE(pattern),DEE_STRING_STR(pattern));
 Dee_DECREF(pattern);
 DeeReturn_Bool(result);
}
static DeeObject *_deestring_F(casewmatch)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pattern; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:casewmatch",&pattern) != 0) return NULL;
 if DEE_UNLIKELY((pattern = DeeString_F(Cast)(pattern)) == NULL) return NULL;
 result = Dee_F(StrCaseWMatchLen)(
  DEE_STRING_SIZE(self),DEE_STRING_STR(self),
  DEE_STRING_SIZE(pattern),DEE_STRING_STR(pattern));
 Dee_DECREF(pattern);
 DeeReturn_Bool(result);
}
static DeeObject *_deestring_F(segments)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t n;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu:segments",&n) != 0) return NULL;
 return DeeString_F(Segments)(self,n);
}
static DeeObject *_deestring_F(scanf)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *format,*result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:scanf",&format) != 0) return NULL;
 if DEE_UNLIKELY((format = DeeString_F(Cast)(format)) == NULL) return NULL;
 result = DeeString_F(Scanf)(self,format);
 Dee_DECREF(format);
 return result;
}
static DeeObject *_deestring_F(front)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t n_chars = 1;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|Iu:front",&n_chars) != 0) return NULL;
 if DEE_UNLIKELY(n_chars > DEE_STRING_SIZE(self)) n_chars = DEE_STRING_SIZE(self);
 return DeeString_F(NewWithLength)(n_chars,DEE_STRING_STR(self));
}
static DeeObject *_deestring_F(back)(
 DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t n_chars = 1;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|Iu:back",&n_chars) != 0) return NULL;
 if DEE_UNLIKELY(n_chars > DEE_STRING_SIZE(self)) n_chars = DEE_STRING_SIZE(self);
 return DeeString_F(NewWithLength)(n_chars,DEE_STRING_STR(self)+(DEE_STRING_SIZE(self)-n_chars));
}

#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
static struct DeeMemberDef const _deestring_F(tp_members)[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__s_str",DEE_STRINGOBJECT,s_str,DEE_CHAR),
 DEE_MEMBERDEF_NAMED_RO_v100("__s_len",DEE_STRINGOBJECT,s_len,Dee_size_t),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMethodDef const _deestring_F(tp_methods)[] = {
 DEE_METHODDEF_CONST_v100("ord",member(&_deestring_F(ord)),DEE_DOC_AUTO_ARGS("T=" DEE_STRING_TYPE_NAME)),
 DEE_METHODDEF_CONST_v100("capitalize",member(&_deestring_F(capitalize)),DEE_DOC_AUTO_ARGS("T=" DEE_STRING_TYPE_NAME)),
 DEE_METHODDEF_CONST_v100("center",member(&_deestring_F(center)),
  "(size_t width, " DEE_CHAR_TYPE_NAME " fill_char = " DEE_CHAR_USERPREFIX "' ') -> " DEE_STRING_TYPE_NAME),
 DEE_METHODDEF_CONST_v100("count",member(&_deestring_F(count)),
  "(" DEE_STRING_TYPE_NAME " sub, size_t begin = 0, size_t end = #this) -> size_t"),
 DEE_METHODDEF_CONST_v100("endswith",member(&_deestring_F(endswith)),
  "(" DEE_STRING_TYPE_NAME " sub, size_t begin = 0, size_t end = #this) -> bool"),
 DEE_METHODDEF_CONST_v100("expandtabs",member(&_deestring_F(expandtabs)),
  "(size_t tabsize = 8) -> " DEE_STRING_TYPE_NAME),
 DEE_METHODDEF_CONST_v100("empty",member(&_deestring_F(empty)),"() -> bool"),
 DEE_METHODDEF_CONST_v100("non_empty",member(&_deestring_F(non_empty)),"() -> bool"),
 DEE_METHODDEF_CONST_v100("find",member(&_deestring_F(find)),
  "(" DEE_STRING_TYPE_NAME " sub, size_t begin = 0, size_t end = #this) -> ssize_t"),
 DEE_METHODDEF_CONST_v100("format",member(&_deestring_F(format)),DEE_DOC_AUTO_ARGS("T=" DEE_STRING_TYPE_NAME)),
 DEE_METHODDEF_CONST_v100("index",member(&_deestring_F(index)),
  "(" DEE_STRING_TYPE_NAME " sub, size_t begin = 0, size_t end = #this) -> size_t"),
 DEE_METHODDEF_CONST_v100("isalnum",member(&_deestring_F(isalnum)),"() -> bool"),
 DEE_METHODDEF_CONST_v100("isalpha",member(&_deestring_F(isalpha)),"() -> bool"),
 DEE_METHODDEF_CONST_v100("isdigit",member(&_deestring_F(isdigit)),"() -> bool"),
 DEE_METHODDEF_CONST_v100("islower",member(&_deestring_F(islower)),"() -> bool"),
 DEE_METHODDEF_CONST_v100("isspace",member(&_deestring_F(isspace)),"() -> bool"),
 DEE_METHODDEF_CONST_v100("istitle",member(&_deestring_F(istitle)),"() -> bool"),
 DEE_METHODDEF_CONST_v100("isupper",member(&_deestring_F(isupper)),"() -> bool"),
 DEE_METHODDEF_CONST_v100("join",member(&_deestring_F(join)),
  "(iterable seq) -> " DEE_STRING_TYPE_NAME "\n\tJoins 'seq' with 'this'"),
 DEE_METHODDEF_CONST_v100("ljust",member(&_deestring_F(ljust)),
  "(size_t width, " DEE_CHAR_TYPE_NAME " fill_char = " DEE_CHAR_USERPREFIX "' ') -> " DEE_STRING_TYPE_NAME),
 DEE_METHODDEF_CONST_v100("lower",member(&_deestring_F(lower)),DEE_DOC_AUTO_ARGS("T=" DEE_STRING_TYPE_NAME)),
 DEE_METHODDEF_CONST_v100("lstrip",member(&_deestring_F(lstrip)),
  "(" DEE_STRING_TYPE_NAME " chars = " DEE_CHAR_USERPREFIX DEE_PP_STR(DEE_EMPTY_CHARS_STR) ") -> " DEE_STRING_TYPE_NAME),
 DEE_METHODDEF_CONST_v100("partition",member(&_deestring_F(partition)),
  "(" DEE_STRING_TYPE_NAME " sep) -> (" DEE_STRING_TYPE_NAME "," DEE_STRING_TYPE_NAME "," DEE_STRING_TYPE_NAME ")"),
 DEE_METHODDEF_CONST_v100("replace",member(&_deestring_F(replace)),
  "(" DEE_STRING_TYPE_NAME " find, string " DEE_STRING_TYPE_NAME ") -> " DEE_STRING_TYPE_NAME),
 DEE_METHODDEF_CONST_v100("rfind",member(&_deestring_F(rfind)),
  "(" DEE_STRING_TYPE_NAME " sub, size_t begin = 0, size_t end = #this) -> ssize_t"),
 DEE_METHODDEF_CONST_v100("rindex",member(&_deestring_F(rindex)),
  "(" DEE_STRING_TYPE_NAME " sub, size_t begin = 0, size_t end = #this) -> size_t"),
 DEE_METHODDEF_CONST_v100("rjust",member(&_deestring_F(rjust)),
  "(size_t width, " DEE_CHAR_TYPE_NAME " fill_char = " DEE_CHAR_USERPREFIX "' ') -> " DEE_STRING_TYPE_NAME),
 DEE_METHODDEF_CONST_v100("rpartition",member(&_deestring_F(rpartition)),
  "(" DEE_STRING_TYPE_NAME " sep) -> (" DEE_STRING_TYPE_NAME "," DEE_STRING_TYPE_NAME "," DEE_STRING_TYPE_NAME ")"),
 DEE_METHODDEF_CONST_v100("rsplit",member(&_deestring_F(rsplit)),DEE_DOC_AUTO_ARGS("T=" DEE_STRING_TYPE_NAME)),
 DEE_METHODDEF_CONST_v100("rstrip",member(&_deestring_F(rstrip)),
  "(" DEE_STRING_TYPE_NAME " chars = " DEE_CHAR_USERPREFIX DEE_PP_STR(DEE_EMPTY_CHARS_STR) ") -> " DEE_STRING_TYPE_NAME),
 DEE_METHODDEF_CONST_v100("split",member(&_deestring_F(split)),DEE_DOC_AUTO_ARGS("T=" DEE_STRING_TYPE_NAME)),
 DEE_METHODDEF_CONST_v100("splitlines",member(&_deestring_F(splitlines)),DEE_DOC_AUTO_ARGS("T=" DEE_STRING_TYPE_NAME)),
 DEE_METHODDEF_CONST_v100("startswith",member(&_deestring_F(startswith)),
  "(" DEE_STRING_TYPE_NAME " sub, size_t begin = 0, size_t end = #this) -> bool"),
 DEE_METHODDEF_CONST_v100("strip",member(&_deestring_F(strip)),
  "(" DEE_STRING_TYPE_NAME " chars = " DEE_CHAR_USERPREFIX DEE_PP_STR(DEE_EMPTY_CHARS_STR) ") -> string"),
 DEE_METHODDEF_CONST_v100("swapcase",member(&_deestring_F(swapcase)),
  "() -> " DEE_STRING_TYPE_NAME),
 DEE_METHODDEF_CONST_v100("title",member(&_deestring_F(title)),DEE_DOC_AUTO_ARGS("T=" DEE_STRING_TYPE_NAME)),
 DEE_METHODDEF_CONST_v100("upper",member(&_deestring_F(upper)),DEE_DOC_AUTO_ARGS("T=" DEE_STRING_TYPE_NAME)),
 DEE_METHODDEF_CONST_v100("zfill",member(&_deestring_F(zfill)),
  "(size_t width) -> " DEE_STRING_TYPE_NAME),
 DEE_METHODDEF_CONST_v100("wmatch",member(&_deestring_F(wmatch)),
  "(" DEE_STRING_TYPE_NAME " pattern) -> bool\n"
  "\tPerforms a wildcard match against @pattern"),
 DEE_METHODDEF_CONST_v100("casewmatch",member(&_deestring_F(casewmatch)),
  "(" DEE_STRING_TYPE_NAME " pattern) -> bool\n"
  "\tPerforms a wildcard match against @pattern, ignoring casing"),
 DEE_METHODDEF_CONST_v100("sstrip",member(&_deestring_F(sstrip)),DEE_DOC_AUTO_ARGS("T=" DEE_STRING_TYPE_NAME)),
 DEE_METHODDEF_CONST_v100("lsstrip",member(&_deestring_F(lsstrip)),DEE_DOC_AUTO_ARGS("T=" DEE_STRING_TYPE_NAME)),
 DEE_METHODDEF_CONST_v100("rsstrip",member(&_deestring_F(rsstrip)),DEE_DOC_AUTO_ARGS("T=" DEE_STRING_TYPE_NAME)),
 DEE_METHODDEF_CONST_v100("substr",member(&_deestring_F(substr)),
  "(size_t pos = 0, size_t len = #this) -> " DEE_STRING_TYPE_NAME),
 DEE_METHODDEF_CONST_v100("compare",member(&_deestring_F(compare)),
  "(" DEE_STRING_TYPE_NAME " s) -> int\n"
  "(size_t pos, size_t len, " DEE_STRING_TYPE_NAME " s) -> int\n"
  "(size_t pos, size_t len, " DEE_STRING_TYPE_NAME " s, size_t sub_pos, size_t sub_len) -> int\n"
  "@return: <0 if @this string is lower than @s; 0 if they're equal; >0 if @this string is greater"),
 DEE_METHODDEF_CONST_v100("reverse",member(&_deestring_F(reversed)),"() -> string\n"
  "@return: A new string with the same length as this, but all characters being reversed"),
 DEE_METHODDEF_CONST_v100("reversed",member(&_deestring_F(reversed)),"() -> string\n"
  "@return: A new string with the same length as this, but all characters being reversed"),
 DEE_METHODDEF_CONST_v100("segments",member(&_deestring_F(segments)),"(size_t n) -> iterable\n"
  "@return: An iterable sequence of @n evenly long strings, based on @this"),
 DEE_METHODDEF_CONST_v100("scanf",member(&_deestring_F(scanf)),"(" DEE_STRING_TYPE_NAME " fmt) -> tuple\n"
  "\tScans @this string using @fmt to create a tuple of parsed values"),
 DEE_METHODDEF_CONST_v100("front",member(&_deestring_F(front)),"(size_t n = 1) -> " DEE_STRING_TYPE_NAME "\n"
  "@return: The first @n characters of the string"),
 DEE_METHODDEF_CONST_v100("back",member(&_deestring_F(back)),"(size_t n = 1) -> " DEE_STRING_TYPE_NAME "\n"
  "@return: The last @n characters of the string"),
 // TODO: Intrinsics for these
 DEE_METHODDEF_CONST_v100("locate",member(&_deegenericiterable_locate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("locate_all",member(&_deegenericiterable_locate_all),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rlocate",member(&_deegenericiterable_rlocate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("sorted",member(&_deegenericiterable_sorted),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("transform",member(&_deegenericiterable_transform),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("contains",member(&_deegenericiterable_contains),DEE_DOC_AUTO),
 DEE_METHODDEF_END_v100
};


#if 0
#define DEE_PRIVATE_STRING_CLASS_CONST_STRING(name,value)\
DeeUtf8String_NEW_STATIC(_deestringclass_val_##name,value);\
static DeeObject *_deestringclass_get_##name(\
 DeeObject *DEE_UNUSED(self), void *DEE_UNUSED(closure)) {\
 DeeReturn_NEWREF(_deestringclass_val_##name);\
}

DEE_PRIVATE_STRING_CLASS_CONST_STRING(ascii_letters,"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
DEE_PRIVATE_STRING_CLASS_CONST_STRING(ascii_lowercase,"abcdefghijklmnopqrstuvwxyz")
DEE_PRIVATE_STRING_CLASS_CONST_STRING(ascii_uppercase,"ABCDEFGHIJKLMNOPQRSTUVWXYZ")
DEE_PRIVATE_STRING_CLASS_CONST_STRING(digits,"0123456789")
DEE_PRIVATE_STRING_CLASS_CONST_STRING(hexdigits,"0123456789abcdefABCDEF")
DEE_PRIVATE_STRING_CLASS_CONST_STRING(octdigits,"01234567")
//string.letters
//string.lowercase
//string.punctuation
//string.printable
//string.uppercase
//string.whitespace
#endif





static DeeObject *_deestringclass_F(chr)(
 DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DEE_CHAR ch;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,DEE_TUPLE_UNPACKFMT_UINT(
  DEE_STRING_ENCODING_SIZE(ENC)) ":chr",&ch) != 0) return NULL;
#if DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(Utf8)
 DeeReturn_Utf8Character(ch);
#else
 return DeeString_F(DEE_PP_CAT_3(From,ENC,StringWithLength))(1,&ch);
#endif
}
static DeeObject *_deestringclass_F(hexdump)(
 DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 // TODO: Signal handler
 DeeObject *str_ob; Dee_size_t buffer_size = (Dee_size_t)-1;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|Iu:hexdump",&str_ob,&buffer_size) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (!DEE_STRING_Check(str_ob)) {
  DEE_CHAR *p;
  if (DeeObject_GetPointerEx(str_ob,DeeObject_TYPE(DEE_CHAR),(void **)&p) != 0) return NULL;
  return (buffer_size == (Dee_size_t)-1)
   ? DeeString_F(NewHexDump)(p,Dee_F(StrLen)(p))
   : DeeString_F(NewHexDump)(p,buffer_size);
 } else
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (DeeError_TypeError_CheckTypeExact(str_ob,&DEE_STRING_Type) != 0) return NULL;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 {
  if (buffer_size == (Dee_size_t)-1) buffer_size = DEE_STRING_SIZE(str_ob)*sizeof(DEE_CHAR);
  return DeeString_F(NewHexDump)(DEE_STRING_STR(str_ob),buffer_size);
 }
}
static DeeObject *_deestringclass_F(from_data)(
 DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 // TODO: Signal handler
 DeeObject *str_ob; Dee_size_t buffer_size = (Dee_size_t)-1;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|Iu:from_data",&str_ob,&buffer_size) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (!DEE_STRING_Check(str_ob)) {
  DEE_CHAR *p;
  if (DeeObject_GetPointerEx(str_ob,DeeObject_TYPE(DEE_CHAR),(void **)&p) != 0) return NULL;
  return (buffer_size == (Dee_size_t)-1)
   ? DeeString_F(New)(p)
   : DeeString_F(NewWithLength)(buffer_size,p);
 } else
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (DeeError_TypeError_CheckTypeExact(str_ob,&DEE_STRING_Type) != 0) return NULL;
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 {
  if (buffer_size == (Dee_size_t)-1 || buffer_size == DEE_STRING_SIZE(str_ob))
   DeeReturn_NEWREF(str_ob);
  return DeeString_F(NewWithLength)(buffer_size,DEE_STRING_STR(str_ob));
 }
}
static DeeObject *_deestringclass_F(cast)(
 DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *data; int ignore_errors = 0;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|b:cast",&data,&ignore_errors) != 0) return NULL;
 return DeeString_F(CastEx)(data,(Dee_uint32_t)(ignore_errors
  ? DEE_STRING_DECODE_FLAG_RELAXED : DEE_STRING_DECODE_FLAG_NONE));
}

static struct DeeMethodDef const _deestring_F(tp_class_methods)[] = {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DEE_METHODDEF_CONST_v100("from_data",member(&_deestringclass_F(from_data)),
  "(" DEE_STRING_TYPE_NAME " s) -> " DEE_STRING_TYPE_NAME "\n"
  "(" DEE_STRING_TYPE_NAME " s, size_t size) -> " DEE_STRING_TYPE_NAME "\n"
  "@return: A hexdump for up to @size bytes starting at @s\n"
  "(" DEE_CHAR_TYPE_NAME " const *p) -> " DEE_STRING_TYPE_NAME "\n"
  "(" DEE_CHAR_TYPE_NAME " const *p, size_t size) -> " DEE_STRING_TYPE_NAME "\n"
  "@return: A string containing @size characters starting at @p"),
 DEE_METHODDEF_CONST_v100("hexdump",member(&_deestringclass_F(hexdump)),
  "(" DEE_STRING_TYPE_NAME " s) -> " DEE_STRING_TYPE_NAME "\n"
  "(" DEE_STRING_TYPE_NAME " s, size_t size) -> " DEE_STRING_TYPE_NAME "\n"
  "@return: A string containing up to @size characters starting at @s\n"
  "(" DEE_CHAR_TYPE_NAME " const *p) -> " DEE_STRING_TYPE_NAME "\n"
  "(" DEE_CHAR_TYPE_NAME " const *p, size_t size) -> " DEE_STRING_TYPE_NAME "\n"
  "@return: A hexdump for @size bytes starting at @p"),
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DEE_METHODDEF_CONST_v100("from_data",member(&_deestringclass_F(from_data)),
  "(" DEE_STRING_TYPE_NAME " s) -> " DEE_STRING_TYPE_NAME "\n"
  "(" DEE_STRING_TYPE_NAME " s, size_t size) -> " DEE_STRING_TYPE_NAME "\n"
  "@return: A string containing up to @size characters starting at @s"),
 DEE_METHODDEF_CONST_v100("hexdump",member(&_deestringclass_F(hexdump)),
  "(" DEE_STRING_TYPE_NAME " s) -> " DEE_STRING_TYPE_NAME "\n"
  "(" DEE_STRING_TYPE_NAME " s, size_t size) -> " DEE_STRING_TYPE_NAME "\n"
  "@return: A hexdump for up to @size bytes starting at @s"),
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DEE_METHODDEF_CONST_v100("chr",member(&_deestringclass_F(chr)),
  "(" DEE_CHAR_TYPE_NAME " ch) -> " DEE_STRING_TYPE_NAME "\n"
  "@return: A 1-character long string containing the character 'ch'"),
#if DEE_CONFIG_HAVE_ENCODING_UTF8
#define LINE_UTF8 "(string.utf8 s, bool ignore_errors = false) -> " DEE_STRING_TYPE_NAME "\n"
#else
#define LINE_UTF8 ""
#endif
#if DEE_CONFIG_HAVE_ENCODING_WIDE
#define LINE_WIDE "(string.wide s, bool ignore_errors = false) -> " DEE_STRING_TYPE_NAME "\n"
#else
#define LINE_WIDE ""
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
#define LINE_UTF16 "(string.utf16 s, bool ignore_errors = false) -> " DEE_STRING_TYPE_NAME "\n"
#else
#define LINE_UTF16 ""
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
#define LINE_UTF32 "(string.utf32 s, bool ignore_errors = false) -> " DEE_STRING_TYPE_NAME "\n"
#else
#define LINE_UTF32 ""
#endif
 DEE_METHODDEF_CONST_v100("cast",member(&_deestringclass_F(cast)),
  LINE_UTF8 LINE_WIDE LINE_UTF16 LINE_UTF32
  "@return: The casted string\n"
  "@throws Error.ValueError.UnicodeError: Error in the encoded string, and 'ignore_errors' was false\n"
  "\tCasts a given string to the encoding of this string type.\n"
  "\tNo 'Error.ValueError.UnicodeError' exceptions will be thrown if 'ignore_errors' is true.\n"
  "\tWhen 'ignore_errors' is false, this function is equivalent to casting 's' to '" DEE_STRING_TYPE_NAME "'"),
#undef LINE_UTF8
#undef LINE_WIDE 
#undef LINE_UTF16 
#undef LINE_UTF32
 DEE_METHODDEF_END_v100
};
static struct DeeMemberDef const _deestring_F(tp_class_members)[] = {
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeStringIterator_F(Type)),
 DEE_MEMBERDEF_CONST_v100("char",object,DeeObject_TYPE(DEE_CHAR)),
#if DEE_STRING_ENCODING(ENC) == DEE_STRING_ENCODING(DEE_CONFIG_DEFAULT_ENCODING)
 // Add access to the explicit string types when querying from the default type
 // NOTE: The default type must represent the hosting platform's
 //       'char *'-style string, which usually evaluates to 'string.utf8'.
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 DEE_MEMBERDEF_CONST_v100("wide",object,&DeeWideString_Type),
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 DEE_MEMBERDEF_CONST_v100("utf8",object,&DeeUtf8String_Type),
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 DEE_MEMBERDEF_CONST_v100("utf16",object,&DeeUtf16String_Type),
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 DEE_MEMBERDEF_CONST_v100("utf32",object,&DeeUtf32String_Type),
#endif
#endif
 DEE_MEMBERDEF_END_v100
};


#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
static struct DeeMemberDef const _deestringiterator_F(tp_members)[] =  {
 DEE_MEMBERDEF_NAMED_RO_v100("__si_string",DEE_STRINGITERATOROBJECT,si_string,object),
 DEE_MEMBERDEF_NAMED_RO_v100("__si_pos",DEE_STRINGITERATOROBJECT,si_pos,atomic(p1(DEE_CHAR))),
 DEE_MEMBERDEF_NAMED_RO_v100("__si_end",DEE_STRINGITERATOROBJECT,si_end,p1(DEE_CHAR)),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */


static DeeObject *DEE_CALL _deestring_F(tp_marshal_new)(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *infile, struct DeeMarshalReadMap *DEE_UNUSED(map),
 DEE_A_IN PDeeMashalNewAfterAlloc after_alloc, void *after_alloc_data) {
 DeeObject *result; Dee_size_t used_size; Dee_uint64_t read_size;
 if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&read_size) != 0) return NULL;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_size > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "String is too long for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_size,(Dee_size_t)-1);
  return NULL;
 }
#endif
 used_size = (Dee_size_t)read_size;
 if DEE_LIKELY((result = DeeString_F(NewSized)(used_size)) != NULL) {
  (*after_alloc)(result,after_alloc_data);
  if DEE_UNLIKELY(DeeFile_ReadAll(infile,DEE_STRING_STR(result),used_size) != 0)
   Dee_CLEAR(result);
 }
 return result;
}

static int DEE_CALL _deestring_F(tp_marshal_put)(
 DeeTypeObject *DEE_UNUSED(tp_self), DEE_STRINGOBJECT *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *DEE_UNUSED(map)) {
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)DEE_STRING_SIZE(self)) != 0) return -1;
 return DeeFile_WriteAll(outfile,DEE_STRING_STR(self),DEE_STRING_SIZE(self));
}

static struct DeeTypeMarshal _deestring_F(tp_marshal) = DEE_TYPE_MARSHAL_VAR_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(DEE_PP_CAT_2(ENC,_string))),
 member(&_deestring_F(tp_marshal_new)),member(&_deestring_F(tp_marshal_put)));



DeeTypeObject DEE_STRING_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(
  member(DEE_STRING_TYPE_NAME),member(
   "() -> " DEE_STRING_TYPE_NAME "\n"
   "(object ob) -> " DEE_STRING_TYPE_NAME "\n"
   "@throws object: Same as calling #(ob.operator __str__())\n"
   "@param ob: an object meant to be concerted into a string\n"
   "@return: An empty string, or the result of #(ob.operator __str__())\n"
   "\tThe builtin #string object, used to represent sequences of characters or bytes in memory.\n"
   "\tNote that strings are immutable, meaning that they cannot be modify once created."),
   member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_VAR_OBJECT|
          DEE_TYPE_FLAG_CONST_CTOR|DEE_TYPE_FLAG_NO_ITERATE_EFFECT|
          DEE_TYPE_FLAG_NO_SUBCLASS|DEE_TYPE_FLAG_FUNDAMENTAL),null),
 DEE_TYPE_OBJECT_VAR_CONSTRUCTOR_EX_v101(
  sizeof(DEE_CHAR), // Store our character size in here.
  member(&_deestring_F(tp_new)),
  member(&_deestring_F(tp_copy_new)),null,
  member(&_deestring_F(tp_any_new))),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deestring_F(tp_str)),
  member(&DeeString_F(Utf8Repr)),
  member(&_deestring_F(tp_int32)),
  member(&_deestring_F(tp_int64)),
  member(&_deestring_F(tp_double))),
 DEE_TYPE_OBJECT_OBJECT_v101(null,null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deestring_F(tp_not)),
  member(&_deestring_F(tp_bool)),null,null,null,null,null,null,null,
  member(&_deestring_F(tp_add)),null,null,null,
  member(&_deestring_F(tp_mul)),null,null,null,
  member(&_deestring_F(tp_mod)),null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  member(&_deestring_F(tp_hash))),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deestring_F(tp_cmp_lo)),member(&_deestring_F(tp_cmp_le)),
  member(&_deestring_F(tp_cmp_eq)),member(&_deestring_F(tp_cmp_ne)),
  member(&_deestring_F(tp_cmp_gr)),member(&_deestring_F(tp_cmp_ge))),
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&_deestring_F(tp_seq_get)),null,null,
  member(&_deestring_F(tp_seq_size)),member(&_deestring_F(tp_seq_contains)),
  member(&_deestring_F(tp_seq_range_get)),null,null,member(&DeeString_F(IterSelf)),null),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deestring_F(tp_members)),null,
  member(_deestring_F(tp_methods)),
  member(_deestring_F(tp_class_members)),null,
  member(_deestring_F(tp_class_methods))),
#else
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,null,member(_deestring_F(tp_methods)),
  member(_deestring_F(tp_class_members)),null,
  member(_deestring_F(tp_class_methods))),
#endif
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deestring_F(tp_marshal))),
 DEE_TYPE_OBJECT_FOOTER_v101
};

static int DEE_CALL _deestringiterator_F(tp_marshal_ctor)(
 DeeTypeObject *DEE_UNUSED(tp_self), DEE_STRINGITERATOROBJECT *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_size_t used_offset; Dee_uint64_t read_offset;
 if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&read_offset) != 0) return -1;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
 if DEE_UNLIKELY(read_offset > (Dee_uint64_t)((Dee_size_t)-1)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "String iterator offset is too large for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                      read_offset,(Dee_size_t)-1);
  return -1;
 }
#endif
 used_offset = (Dee_size_t)read_offset;
 if DEE_UNLIKELY((self->si_string = (DEE_STRINGOBJECT *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact((DeeObject *)self->si_string,&DeeString_Type) != 0) { Dee_DECREF(self->si_string); return -1; }
 self->si_pos = DEE_STRING_STR(self->si_string)+used_offset;
 self->si_end = DEE_STRING_STR(self->si_string)+DEE_STRING_SIZE(self->si_string);
 return 0;
}

static int DEE_CALL _deestringiterator_F(tp_marshal_put)(
 DeeTypeObject *DEE_UNUSED(tp_self), DEE_STRINGITERATOROBJECT *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 DEE_CHAR *charpos; Dee_uint64_t written_pos;
 charpos = (DEE_CHAR *)DeeAtomicPtr_Load(self->si_pos,memory_order_seq_cst);
 written_pos = (Dee_uint64_t)(charpos-DEE_STRING_STR(self->si_string));
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,written_pos) != 0) return -1;
 return DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)self->si_string);
}

static struct DeeTypeMarshal _deestringiterator_F(tp_marshal) = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(DEE_PP_CAT_2(ENC,_string_iterator))),
 member(&_deestringiterator_F(tp_marshal_ctor)),member(&_deestringiterator_F(tp_marshal_put)));

DeeTypeObject DeeStringIterator_F(Type) = {
 DEE_TYPE_OBJECT_HEAD_v100(
  member(DEE_STRING_TYPE_NAME ".iterator"),
  member("(" DEE_STRING_TYPE_NAME " s = " DEE_CHAR_USERPREFIX "\"\") -> " DEE_STRING_TYPE_NAME ".iterator\n"
  "\tIterator type for strings. This iterator traverses a string character-wise, yielding "
    "a new one-character string for every character in the original string."),
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DEE_STRINGITERATOROBJECT),null,
  member(&_deestringiterator_F(tp_ctor)),
  member(&_deestringiterator_F(tp_copy_ctor)),null,
  member(&_deestringiterator_F(tp_any_ctor))),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deestringiterator_F(tp_dtor))),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deestringiterator_F(tp_cmp_lo)),
  member(&_deestringiterator_F(tp_cmp_le)),
  member(&_deestringiterator_F(tp_cmp_eq)),
  member(&_deestringiterator_F(tp_cmp_ne)),
  member(&_deestringiterator_F(tp_cmp_gr)),
  member(&_deestringiterator_F(tp_cmp_ge))),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,
  member(&DeeStringIterator_F(IterNext))),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deestringiterator_F(tp_members)),null,null,null,null,null),
#else
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
#endif
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deestringiterator_F(tp_marshal))),
 DEE_TYPE_OBJECT_FOOTER_v101
};
#undef DEE_CHAR_USERPREFIX
#undef DEE_CHAR_TYPE_NAME
#undef DEE_STRING_TYPE_NAME

#undef DEE_SIZEOF_ENC
#undef Dee_F
#undef _deestring_F
#undef _deestringclass_F
#undef _deestringiterator_F
#undef DeeString_F
#undef DeeStringIterator_F
#undef DeeStringWriter_F
#undef _Dee_F
#undef _DeeString_F
#undef _DeeStringIterator_F
#undef _DeeStringWriter_F
#undef DEE_CHAR
#undef DEE_STRINGOBJECT
#undef DEE_STRINGITERATOROBJECT
#undef DEE_STRINGWRITER
#undef DEE_EMPTYSTRING
#undef DEERETURN_EMPTYSTRING
#undef DEE_CHAR_C

#undef DEE_STRING_Type
#undef DEE_STRING_Check
#undef DEE_STRING_STR
#undef DEE_STRING_SIZE
#undef DEE_STRINGWRITER_INIT
#undef DEE_STRINGWRITER_Pack
#undef DEE_STRINGWRITER_Quit

#undef IS_ENCODING
#undef ENC

DEE_DECL_END

