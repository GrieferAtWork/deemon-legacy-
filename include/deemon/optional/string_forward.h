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
#ifndef GUARD_DEEMON_OPTIONAL_STRING_FORWARD_H
#define GUARD_DEEMON_OPTIONAL_STRING_FORWARD_H 1

#include <deemon/__conf.inl>

DEE_DECL_BEGIN

typedef char           Dee_Utf8Char;
#ifndef DEE_CHAR8_C
#define DEE_CHAR8_C(c) c
#endif

#if (defined(_CHAR16_T_DEFINED) || \
     defined(_NATIVE_CHAR16_T_DEFINED)) && \
    !DEE_TYPES_CHAR16_T_SIGNED && \
    (DEE_TYPES_SIZEOF_CHAR16_T == 2)
typedef char16_t          Dee_Utf16Char;
#if !defined(DEE_CHAR16_C) && \
    DEE_COMPILER_HAVE_UTF16STRING_PREFIX
#define DEE_CHAR16_C(c)   u##c
#endif
#else
typedef DEE_TYPES_UINT16_T Dee_Utf16Char;
#endif
#ifndef DEE_CHAR16_C
#define DEE_CHAR16_C(c)  (Dee_Utf16Char)c
#endif

#if (defined(_CHAR32_T_DEFINED) || \
     defined(_NATIVE_CHAR32_T_DEFINED)) && \
    !DEE_TYPES_CHAR32_T_SIGNED && \
    (DEE_TYPES_SIZEOF_CHAR32_T == 4)
typedef char32_t          Dee_Utf32Char;
#if !defined(DEE_CHAR32_C) && \
    DEE_COMPILER_HAVE_UTF32STRING_PREFIX
#define DEE_CHAR32_C(c)   U##c
#endif
#else
typedef DEE_TYPES_UINT32_T Dee_Utf32Char;
#endif
#ifndef DEE_CHAR32_C
#define DEE_CHAR32_C(c)  (Dee_Utf32Char)c
#endif

#if (defined(_WCHAR_T_DEFINED) || \
     defined(_NATIVE_WCHAR_T_DEFINED)) && \
    !DEE_TYPES_WCHAR_T_SIGNED
typedef wchar_t           Dee_WideChar;
#if !defined(DEE_WIDECHAR_C) && \
    DEE_COMPILER_HAVE_WIDESTRING_PREFIX
#define DEE_WIDECHAR_C(c) L##c
#endif
#elif defined(DEE_PLATFORM_WINDOWS)
typedef DEE_TYPES_UINT16_T Dee_WideChar;
#else
typedef DEE_TYPES_UINT32_T Dee_WideChar;
#endif
#ifndef DEE_WIDECHAR_C
#define DEE_WIDECHAR_C(c) (Dee_WideChar)c
#endif

#define DEE_CONFIG_HAVE_ENCODING_WIDE  1
#define DEE_CONFIG_HAVE_ENCODING_UTF8  1
#define DEE_CONFIG_HAVE_ENCODING_UTF16 1
#define DEE_CONFIG_HAVE_ENCODING_UTF32 1
#define DEE_CONFIG_ENCODING_COUNT \
(DEE_CONFIG_HAVE_ENCODING_WIDE+\
 DEE_CONFIG_HAVE_ENCODING_UTF8+\
 DEE_CONFIG_HAVE_ENCODING_UTF16+\
 DEE_CONFIG_HAVE_ENCODING_UTF32)

// Must be the type corresponding to 'char cosnt *'-style strings (aka. 'Utf8')
#define DEE_CONFIG_DEFAULT_ENCODING    Utf8

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// String
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

DEE_OBJECT_DEF(DeeAnyStringObject);
#if DEE_CONFIG_HAVE_ENCODING_WIDE
DEE_OBJECT_DEF(DeeWideStringObject);
DEE_OBJECT_DEF(DeeWideStringIteratorObject);
DEE_STRUCT_DEF(DeeWideStringWriter);
struct _DeeEmptyWideStringObject;
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF8
DEE_OBJECT_DEF(DeeUtf8StringObject);
DEE_OBJECT_DEF(DeeUtf8StringIteratorObject);
DEE_STRUCT_DEF(DeeUtf8StringWriter);
struct _DeeEmptyUtf8StringObject;
struct _DeeUtf8StringCharacterObject;
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
DEE_OBJECT_DEF(DeeUtf16StringObject);
DEE_OBJECT_DEF(DeeUtf16StringIteratorObject);
DEE_STRUCT_DEF(DeeUtf16StringWriter);
struct _DeeEmptyUtf16StringObject;
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
DEE_OBJECT_DEF(DeeUtf32StringObject);
DEE_OBJECT_DEF(DeeUtf32StringIteratorObject);
DEE_STRUCT_DEF(DeeUtf32StringWriter);
struct _DeeEmptyUtf32StringObject;
#endif

#define _DeeEmptyStringObject                                 _DeeEmptyUtf8StringObject
#define _DeeStringCharacterObject                             _DeeUtf8StringCharacterObject
#define Dee_Char                                              Dee_Utf8Char
#define DeeStringObject                                       DeeUtf8StringObject
#define DeeStringIteratorObject                               DeeUtf8StringIteratorObject
#define DeeStringWriter                                       DeeUtf8StringWriter
#define DeeString_FromString                                  DeeUtf8String_FromString
#define DeeString_FromMyString                                DeeUtf8String_FromMyString
#define DeeString_FromWideString                              DeeUtf8String_FromWideString
#define DeeString_FromUtf8String                              DeeUtf8String_FromUtf8String
#define DeeString_FromUtf16String                             DeeUtf8String_FromUtf16String
#define DeeString_FromUtf32String                             DeeUtf8String_FromUtf32String
#define DeeString_FromStringWithLength                        DeeUtf8String_FromStringWithLength
#define DeeString_FromMyStringWithLength                      DeeUtf8String_FromMyStringWithLength
#define DeeString_FromWideStringWithLength                    DeeUtf8String_FromWideStringWithLength
#define DeeString_FromUtf8StringWithLength                    DeeUtf8String_FromUtf8StringWithLength
#define DeeString_FromUtf16StringWithLength                   DeeUtf8String_FromUtf16StringWithLength
#define DeeString_FromUtf32StringWithLength                   DeeUtf8String_FromUtf32StringWithLength
#define DeeString_FromStringEx                                DeeUtf8String_FromStringEx
#define DeeString_FromMyStringEx                              DeeUtf8String_FromMyStringEx
#define DeeString_FromWideStringEx                            DeeUtf8String_FromWideStringEx
#define DeeString_FromUtf8StringEx                            DeeUtf8String_FromUtf8StringEx
#define DeeString_FromUtf16StringEx                           DeeUtf8String_FromUtf16StringEx
#define DeeString_FromUtf32StringEx                           DeeUtf8String_FromUtf32StringEx
#define DeeString_FromStringWithLengthEx                      DeeUtf8String_FromStringWithLengthEx
#define DeeString_FromMyStringWithLengthEx                    DeeUtf8String_FromMyStringWithLengthEx
#define DeeString_FromWideStringWithLengthEx                  DeeUtf8String_FromWideStringWithLengthEx
#define DeeString_FromUtf8StringWithLengthEx                  DeeUtf8String_FromUtf8StringWithLengthEx
#define DeeString_FromUtf16StringWithLengthEx                 DeeUtf8String_FromUtf16StringWithLengthEx
#define DeeString_FromUtf32StringWithLengthEx                 DeeUtf8String_FromUtf32StringWithLengthEx
#define DeeString_FromQuotedString                            DeeUtf8String_FromQuotedString
#define DeeString_FromQuotedMyString                          DeeUtf8String_FromQuotedMyString
#define DeeString_FromQuotedWideString                        DeeUtf8String_FromQuotedWideString
#define DeeString_FromQuotedUtf8String                        DeeUtf8String_FromQuotedUtf8String
#define DeeString_FromQuotedUtf16String                       DeeUtf8String_FromQuotedUtf16String
#define DeeString_FromQuotedUtf32String                       DeeUtf8String_FromQuotedUtf32String
#define DeeString_FromQuotedStringWithLength                  DeeUtf8String_FromQuotedStringWithLength
#define DeeString_FromQuotedMyStringWithLength                DeeUtf8String_FromQuotedMyStringWithLength
#define DeeString_FromQuotedWideStringWithLength              DeeUtf8String_FromQuotedWideStringWithLength
#define DeeString_FromQuotedUtf8StringWithLength              DeeUtf8String_FromQuotedUtf8StringWithLength
#define DeeString_FromQuotedUtf16StringWithLength             DeeUtf8String_FromQuotedUtf16StringWithLength
#define DeeString_FromQuotedUtf32StringWithLength             DeeUtf8String_FromQuotedUtf32StringWithLength
#define DeeString_Str                                         DeeUtf8String_Str
#define DeeString_Size                                        DeeUtf8String_Size
#define DeeString_STR                                         DeeUtf8String_STR
#define DeeString_SIZE                                        DeeUtf8String_SIZE
#define DeeString_EQUALS                                      DeeUtf8String_EQUALS
#define DeeString_ToInt8                                      DeeUtf8String_ToInt8
#define DeeString_ToInt16                                     DeeUtf8String_ToInt16
#define DeeString_ToInt32                                     DeeUtf8String_ToInt32
#define DeeString_ToInt64                                     DeeUtf8String_ToInt64
#define DeeString_ToUInt8                                     DeeUtf8String_ToUInt8
#define DeeString_ToUInt16                                    DeeUtf8String_ToUInt16
#define DeeString_ToUInt32                                    DeeUtf8String_ToUInt32
#define DeeString_ToUInt64                                    DeeUtf8String_ToUInt64
#define DeeString_ToFloat                                     DeeUtf8String_ToFloat
#define DeeString_ToDouble                                    DeeUtf8String_ToDouble
#define DeeString_ToLDouble                                   DeeUtf8String_ToLDouble
#define DeeString_ToInt8WithLength                            DeeUtf8String_ToInt8WithLength
#define DeeString_ToInt16WithLength                           DeeUtf8String_ToInt16WithLength
#define DeeString_ToInt32WithLength                           DeeUtf8String_ToInt32WithLength
#define DeeString_ToInt64WithLength                           DeeUtf8String_ToInt64WithLength
#define DeeString_ToUInt8WithLength                           DeeUtf8String_ToUInt8WithLength
#define DeeString_ToUInt16WithLength                          DeeUtf8String_ToUInt16WithLength
#define DeeString_ToUInt32WithLength                          DeeUtf8String_ToUInt32WithLength
#define DeeString_ToUInt64WithLength                          DeeUtf8String_ToUInt64WithLength
#define DeeString_ToFloatWithLength                           DeeUtf8String_ToFloatWithLength
#define DeeString_ToDoubleWithLength                          DeeUtf8String_ToDoubleWithLength
#define DeeString_ToLDoubleWithLength                         DeeUtf8String_ToLDoubleWithLength
#define DeeString_ToNumber_F                                  DeeUtf8String_ToNumber_F
#define DeeString_ToNumber                                    DeeUtf8String_ToNumber
#define DeeString_ToNumberWithLength_F                        DeeUtf8String_ToNumberWithLength_F
#define DeeString_ToNumberWithLength                          DeeUtf8String_ToNumberWithLength
#ifdef DEE_LIMITED_DEX
#define DeeStringIterator_STRING                              DeeUtf8StringIterator_STRING
#define DeeStringIterator_POS                                 DeeUtf8StringIterator_POS
#define DeeStringIterator_END                                 DeeUtf8StringIterator_END
#endif
#ifdef DEE_LIMITED_DEX
#define DeeString_NEW_STATIC                                  DeeUtf8String_NEW_STATIC
#define DeeString_NEW_STATIC_EX                               DeeUtf8String_NEW_STATIC_EX
#endif
#define _Dee_EmptyString                                      _Dee_EmptyUtf8String
#define Dee_EmptyString                                       Dee_EmptyUtf8String
#define DeeString_NewEmpty                                    DeeUtf8String_NewEmpty
#define DeeString_Character                                   DeeUtf8String_Character
#define DeeReturn_Character                                   DeeReturn_Utf8Character
#define DeeReturn_EmptyString                                 DeeReturn_EmptyUtf8String
#define DeeReturn_STATIC_STRING                               DeeReturn_STATIC_UTF8_STRING
#define DeeReturn_STATIC_STRING_EX                            DeeReturn_STATIC_UTF8_STRING_EX
#define DeeReturn_STATIC_STRING_NOREF                         DeeReturn_STATIC_UTF8_STRING_NOREF
#define DeeString_Check                                       DeeUtf8String_Check
#define DeeString_CheckExact                                  DeeUtf8String_CheckExact
#define DeeString_CheckEmpty                                  DeeUtf8String_CheckEmpty
#define DeeStringIterator_Check                               DeeUtf8StringIterator_Check
#define DeeString_Type                                        DeeUtf8String_Type
#define DeeStringIterator_Type                                DeeUtf8StringIterator_Type
#define DeeString_FromInt8                                    DeeUtf8String_FromInt8
#define DeeString_FromInt16                                   DeeUtf8String_FromInt16
#define DeeString_FromInt32                                   DeeUtf8String_FromInt32
#define DeeString_FromInt64                                   DeeUtf8String_FromInt64
#define DeeString_FromUInt8                                   DeeUtf8String_FromUInt8
#define DeeString_FromUInt16                                  DeeUtf8String_FromUInt16
#define DeeString_FromUInt32                                  DeeUtf8String_FromUInt32
#define DeeString_FromUInt64                                  DeeUtf8String_FromUInt64
#define DeeString_FromFloat                                   DeeUtf8String_FromFloat
#define DeeString_FromDouble                                  DeeUtf8String_FromDouble
#define DeeString_FromLDouble                                 DeeUtf8String_FromLDouble
#define DeeString_New                                         DeeUtf8String_New
#define DeeString_NewWithLength                               DeeUtf8String_NewWithLength
#define DeeString_NewSized                                    DeeUtf8String_NewSized
#define DeeString_NewQuoted                                   DeeUtf8String_NewQuoted
#define DeeString_NewQuotedWithLength                         DeeUtf8String_NewQuotedWithLength
#define DeeString_NEW                                         DeeUtf8String_NEW
#define DeeString_Cast                                        DeeUtf8String_Cast
#define DeeString_CastEx                                      DeeUtf8String_CastEx
#define DeeString_InplaceCast                                 DeeUtf8String_InplaceCast
#define DeeString_InplaceCastEx                               DeeUtf8String_InplaceCastEx
#define DeeString_Resize                                      DeeUtf8String_Resize
#define DeeString_Append                                      DeeUtf8String_Append
#define DeeString_AppendWithLength                            DeeUtf8String_AppendWithLength
#define DeeString_Newf                                        DeeUtf8String_Newf
#define DeeString_VNewf                                       DeeUtf8String_VNewf
#define DeeString_Repr                                        DeeUtf8String_Repr
#define DeeString_WideRepr                                    DeeUtf8String_WideRepr
#define DeeString_Utf8Repr                                    DeeUtf8String_Utf8Repr
#define DeeString_Utf16Repr                                   DeeUtf8String_Utf16Repr
#define DeeString_Utf32Repr                                   DeeUtf8String_Utf32Repr
#define DeeString_Concat                                      DeeUtf8String_Concat
#define DeeString_Repeat                                      DeeUtf8String_Repeat
#define DeeString_Join                                        DeeUtf8String_Join
#define DeeString_Split                                       DeeUtf8String_Split
#define DeeString_RSplit                                      DeeUtf8String_RSplit
#define DeeString_Capitalize                                  DeeUtf8String_Capitalize
#define DeeString_Center                                      DeeUtf8String_Center
#define DeeString_ZFill                                       DeeUtf8String_ZFill
#define DeeString_LJust                                       DeeUtf8String_LJust
#define DeeString_RJust                                       DeeUtf8String_RJust
#define DeeString_Strip                                       DeeUtf8String_Strip
#define DeeString_LStrip                                      DeeUtf8String_LStrip
#define DeeString_RStrip                                      DeeUtf8String_RStrip
#define DeeString_ExpandTabs                                  DeeUtf8String_ExpandTabs
#define DeeString_Format                                      DeeUtf8String_Format
#define DeeString_CFormat                                     DeeUtf8String_CFormat
#define DeeString_Contains                                    DeeUtf8String_Contains
#define DeeString_Find                                        DeeUtf8String_Find
#define DeeString_RFind                                       DeeUtf8String_RFind
#define DeeString_Index                                       DeeUtf8String_Index
#define DeeString_RIndex                                      DeeUtf8String_RIndex
#define DeeString_Count                                       DeeUtf8String_Count
#define DeeString_StartsWith                                  DeeUtf8String_StartsWith
#define DeeString_EndsWith                                    DeeUtf8String_EndsWith
#define DeeString_IsAlnum                                     DeeUtf8String_IsAlnum
#define DeeString_IsAlpha                                     DeeUtf8String_IsAlpha
#define DeeString_IsBlank                                     DeeUtf8String_IsBlank
#define DeeString_IsCntrl                                     DeeUtf8String_IsCntrl
#define DeeString_IsDigit                                     DeeUtf8String_IsDigit
#define DeeString_IsGraph                                     DeeUtf8String_IsGraph
#define DeeString_IsLower                                     DeeUtf8String_IsLower
#define DeeString_IsPrint                                     DeeUtf8String_IsPrint
#define DeeString_IsPunct                                     DeeUtf8String_IsPunct
#define DeeString_IsSpace                                     DeeUtf8String_IsSpace
#define DeeString_IsUpper                                     DeeUtf8String_IsUpper
#define DeeString_IsXChar                                     DeeUtf8String_IsXChar
#define DeeString_IsXDigit                                    DeeUtf8String_IsXDigit
#define DeeString_IsTitle                                     DeeUtf8String_IsTitle
#define DeeString_Reverse                                     DeeUtf8String_Reverse
#define DeeString_Lower                                       DeeUtf8String_Lower
#define DeeString_Upper                                       DeeUtf8String_Upper
#define DeeString_Title                                       DeeUtf8String_Title
#define DeeString_SwapCase                                    DeeUtf8String_SwapCase
#define DeeString_Replace                                     DeeUtf8String_Replace
#define DeeString_Partition                                   DeeUtf8String_Partition
#define DeeString_RPartition                                  DeeUtf8String_RPartition
#define DeeString_SplitLines                                  DeeUtf8String_SplitLines
#define DeeString_SStrip                                      DeeUtf8String_SStrip
#define DeeString_LSStrip                                     DeeUtf8String_LSStrip
#define DeeString_RSStrip                                     DeeUtf8String_RSStrip
#define DeeString_WMatch                                      DeeUtf8String_WMatch
#define DeeString_NewHexDump                                  DeeUtf8String_NewHexDump
#define DeeString_Hash                                        DeeUtf8String_Hash
#ifdef DEE_LIMITED_DEX
#define DeeStringWriter_INIT                                  DeeUtf8StringWriter_INIT
#endif
#define DeeStringWriter_STR                                   DeeUtf8StringWriter_STR
#define DeeStringWriter_EMPTY                                 DeeUtf8StringWriter_EMPTY
#define DeeStringWriter_ELEM                                  DeeUtf8StringWriter_ELEM
#define DeeStringWriter_SIZE                                  DeeUtf8StringWriter_SIZE
#define DeeStringWriter_ALLOCATED                             DeeUtf8StringWriter_ALLOCATED
#define DeeStringWriter_Str                                   DeeUtf8StringWriter_Str
#define DeeStringWriter_Elem                                  DeeUtf8StringWriter_Elem
#define DeeStringWriter_Empty                                 DeeUtf8StringWriter_Empty
#define DeeStringWriter_Size                                  DeeUtf8StringWriter_Size
#define DeeStringWriter_Allocated                             DeeUtf8StringWriter_Allocated
#define DeeStringWriter_Alloc                                 DeeUtf8StringWriter_Alloc
#define DeeStringWriter_New                                   DeeUtf8StringWriter_New
#define DeeStringWriter_Delete                                DeeUtf8StringWriter_Delete
#define DeeStringWriter_Free                                  DeeUtf8StringWriter_Free
#define DeeStringWriter_Init                                  DeeUtf8StringWriter_Init
#define DeeStringWriter_InitWithSizeHint                      DeeUtf8StringWriter_InitWithSizeHint
#define DeeStringWriter_InitMove                              DeeUtf8StringWriter_InitMove
#define DeeStringWriter_InitCopy                              DeeUtf8StringWriter_InitCopy
#define DeeStringWriter_AssignMove                            DeeUtf8StringWriter_AssignMove
#define DeeStringWriter_AssignCopy                            DeeUtf8StringWriter_AssignCopy
#define DeeStringWriter_Quit                                  DeeUtf8StringWriter_Quit
#define DeeStringWriter_Assign                                DeeUtf8StringWriter_Assign
#define DeeStringWriter_Clear                                 DeeUtf8StringWriter_Clear
#define DeeStringWriter_ClearAndFlush                         DeeUtf8StringWriter_ClearAndFlush
#define DeeStringWriter_Pack                                  DeeUtf8StringWriter_Pack
#define DeeStringWriter_Flush                                 DeeUtf8StringWriter_Flush
#define DeeStringWriter_FlushAndPack                          DeeUtf8StringWriter_FlushAndPack
#ifdef DEE_LIMITED_API
#define _DeeStringWriter_PackCString                          _DeeUtf8StringWriter_PackCString
#endif
#define DeeStringWriter_IncreaseCapacity                      DeeUtf8StringWriter_IncreaseCapacity
#define DeeStringWriter_Resize                                DeeUtf8StringWriter_Resize
#define DeeStringWriter_Require                               DeeUtf8StringWriter_Require
#define DeeStringWriter_Writef                                DeeUtf8StringWriter_Writef
#define DeeStringWriter_VWritef                               DeeUtf8StringWriter_VWritef
#define DeeStringWriter_WRITE_STRING                          DeeUtf8StringWriter_WRITE_STRING
#define DeeStringWriter_WRITE_MY_STRING                       DeeUtf8StringWriter_WRITE_MY_STRING
#define DeeStringWriter_WRITE_WIDE_STRING                     DeeUtf8StringWriter_WRITE_WIDE_STRING
#define DeeStringWriter_WRITE_UTF8_STRING                     DeeUtf8StringWriter_WRITE_UTF8_STRING
#define DeeStringWriter_WRITE_UTF16_STRING                    DeeUtf8StringWriter_WRITE_UTF16_STRING
#define DeeStringWriter_WRITE_UTF32_STRING                    DeeUtf8StringWriter_WRITE_UTF32_STRING
#define DeeStringWriter_WriteString                           DeeUtf8StringWriter_WriteString
#define DeeStringWriter_WriteMyString                         DeeUtf8StringWriter_WriteMyString
#define DeeStringWriter_WriteWideString                       DeeUtf8StringWriter_WriteWideString
#define DeeStringWriter_WriteUtf8String                       DeeUtf8StringWriter_WriteUtf8String
#define DeeStringWriter_WriteUtf16String                      DeeUtf8StringWriter_WriteUtf16String
#define DeeStringWriter_WriteUtf32String                      DeeUtf8StringWriter_WriteUtf32String
#define DeeStringWriter_WriteStringWithLength                 DeeUtf8StringWriter_WriteStringWithLength
#define DeeStringWriter_WriteMyStringWithLength               DeeUtf8StringWriter_WriteMyStringWithLength
#define DeeStringWriter_WriteWideStringWithLength             DeeUtf8StringWriter_WriteWideStringWithLength
#define DeeStringWriter_WriteUtf8StringWithLength             DeeUtf8StringWriter_WriteUtf8StringWithLength
#define DeeStringWriter_WriteUtf16StringWithLength            DeeUtf8StringWriter_WriteUtf16StringWithLength
#define DeeStringWriter_WriteUtf32StringWithLength            DeeUtf8StringWriter_WriteUtf32StringWithLength
#define DeeStringWriter_WriteStringEx                         DeeUtf8StringWriter_WriteStringEx
#define DeeStringWriter_WriteMyStringEx                       DeeUtf8StringWriter_WriteMyStringEx
#define DeeStringWriter_WriteWideStringEx                     DeeUtf8StringWriter_WriteWideStringEx
#define DeeStringWriter_WriteUtf8StringEx                     DeeUtf8StringWriter_WriteUtf8StringEx
#define DeeStringWriter_WriteUtf16StringEx                    DeeUtf8StringWriter_WriteUtf16StringEx
#define DeeStringWriter_WriteUtf32StringEx                    DeeUtf8StringWriter_WriteUtf32StringEx
#define DeeStringWriter_WriteStringWithLengthEx               DeeUtf8StringWriter_WriteStringWithLengthEx
#define DeeStringWriter_WriteMyStringWithLengthEx             DeeUtf8StringWriter_WriteMyStringWithLengthEx
#define DeeStringWriter_WriteWideStringWithLengthEx           DeeUtf8StringWriter_WriteWideStringWithLengthEx
#define DeeStringWriter_WriteUtf8StringWithLengthEx           DeeUtf8StringWriter_WriteUtf8StringWithLengthEx
#define DeeStringWriter_WriteUtf16StringWithLengthEx          DeeUtf8StringWriter_WriteUtf16StringWithLengthEx
#define DeeStringWriter_WriteUtf32StringWithLengthEx          DeeUtf8StringWriter_WriteUtf32StringWithLengthEx
#define DeeStringWriter_WriteChar                             DeeUtf8StringWriter_WriteChar
#define DeeStringWriter_WriteMyChar                           DeeUtf8StringWriter_WriteMyChar
#define DeeStringWriter_WriteWideChar                         DeeUtf8StringWriter_WriteWideChar
#define DeeStringWriter_WriteUtf8Char                         DeeUtf8StringWriter_WriteUtf8Char
#define DeeStringWriter_WriteUtf16Char                        DeeUtf8StringWriter_WriteUtf16Char
#define DeeStringWriter_WriteUtf32Char                        DeeUtf8StringWriter_WriteUtf32Char
#define DeeStringWriter_WriteObjectStr                        DeeUtf8StringWriter_WriteObjectStr
#define DeeStringWriter_WriteObjectRepr                       DeeUtf8StringWriter_WriteObjectRepr
#define DeeStringWriter_RepeatChar                            DeeUtf8StringWriter_RepeatChar
#define DeeStringWriter_CopyFromFile                          DeeUtf8StringWriter_CopyFromFile
#define DeeStringWriter_WriteQuotedString                     DeeUtf8StringWriter_WriteQuotedString
#define DeeStringWriter_WriteQuotedMyString                   DeeUtf8StringWriter_WriteQuotedMyString
#define DeeStringWriter_WriteQuotedWideString                 DeeUtf8StringWriter_WriteQuotedWideString
#define DeeStringWriter_WriteQuotedUtf8String                 DeeUtf8StringWriter_WriteQuotedUtf8String
#define DeeStringWriter_WriteQuotedUtf16String                DeeUtf8StringWriter_WriteQuotedUtf16String
#define DeeStringWriter_WriteQuotedUtf32String                DeeUtf8StringWriter_WriteQuotedUtf32String
#define DeeStringWriter_WriteQuotedStringWithLength           DeeUtf8StringWriter_WriteQuotedStringWithLength
#define DeeStringWriter_WriteQuotedMyStringWithLength         DeeUtf8StringWriter_WriteQuotedMyStringWithLength
#define DeeStringWriter_WriteQuotedWideStringWithLength       DeeUtf8StringWriter_WriteQuotedWideStringWithLength
#define DeeStringWriter_WriteQuotedUtf8StringWithLength       DeeUtf8StringWriter_WriteQuotedUtf8StringWithLength
#define DeeStringWriter_WriteQuotedUtf16StringWithLength      DeeUtf8StringWriter_WriteQuotedUtf16StringWithLength
#define DeeStringWriter_WriteQuotedUtf32StringWithLength      DeeUtf8StringWriter_WriteQuotedUtf32StringWithLength
#define DeeStringWriter_SpecWriteString                       DeeUtf8StringWriter_SpecWriteString
#define DeeStringWriter_SpecWriteMyString                     DeeUtf8StringWriter_SpecWriteMyString
#define DeeStringWriter_SpecWriteWideString                   DeeUtf8StringWriter_SpecWriteWideString
#define DeeStringWriter_SpecWriteUtf8String                   DeeUtf8StringWriter_SpecWriteUtf8String
#define DeeStringWriter_SpecWriteUtf16String                  DeeUtf8StringWriter_SpecWriteUtf16String
#define DeeStringWriter_SpecWriteUtf32String                  DeeUtf8StringWriter_SpecWriteUtf32String
#define DeeStringWriter_SpecWriteStringWithLength             DeeUtf8StringWriter_SpecWriteStringWithLength
#define DeeStringWriter_SpecWriteMyStringWithLength           DeeUtf8StringWriter_SpecWriteMyStringWithLength
#define DeeStringWriter_SpecWriteWideStringWithLength         DeeUtf8StringWriter_SpecWriteWideStringWithLength
#define DeeStringWriter_SpecWriteUtf8StringWithLength         DeeUtf8StringWriter_SpecWriteUtf8StringWithLength
#define DeeStringWriter_SpecWriteUtf16StringWithLength        DeeUtf8StringWriter_SpecWriteUtf16StringWithLength
#define DeeStringWriter_SpecWriteUtf32StringWithLength        DeeUtf8StringWriter_SpecWriteUtf32StringWithLength
#define DeeStringWriter_SpecWriteChar                         DeeUtf8StringWriter_SpecWriteChar
#define DeeStringWriter_SpecWriteMyChar                       DeeUtf8StringWriter_SpecWriteMyChar
#define DeeStringWriter_SpecWriteWideChar                     DeeUtf8StringWriter_SpecWriteWideChar
#define DeeStringWriter_SpecWriteUtf8Char                     DeeUtf8StringWriter_SpecWriteUtf8Char
#define DeeStringWriter_SpecWriteUtf16Char                    DeeUtf8StringWriter_SpecWriteUtf16Char
#define DeeStringWriter_SpecWriteUtf32Char                    DeeUtf8StringWriter_SpecWriteUtf32Char
#define DeeStringWriter_SpecWriteStringEx                     DeeUtf8StringWriter_SpecWriteStringEx
#define DeeStringWriter_SpecWriteMyStringEx                   DeeUtf8StringWriter_SpecWriteMyStringEx
#define DeeStringWriter_SpecWriteWideStringEx                 DeeUtf8StringWriter_SpecWriteWideStringEx
#define DeeStringWriter_SpecWriteUtf8StringEx                 DeeUtf8StringWriter_SpecWriteUtf8StringEx
#define DeeStringWriter_SpecWriteUtf16StringEx                DeeUtf8StringWriter_SpecWriteUtf16StringEx
#define DeeStringWriter_SpecWriteUtf32StringEx                DeeUtf8StringWriter_SpecWriteUtf32StringEx
#define DeeStringWriter_SpecWriteStringWithLengthEx           DeeUtf8StringWriter_SpecWriteStringWithLengthEx
#define DeeStringWriter_SpecWriteMyStringWithLengthEx         DeeUtf8StringWriter_SpecWriteMyStringWithLengthEx
#define DeeStringWriter_SpecWriteWideStringWithLengthEx       DeeUtf8StringWriter_SpecWriteWideStringWithLengthEx
#define DeeStringWriter_SpecWriteUtf8StringWithLengthEx       DeeUtf8StringWriter_SpecWriteUtf8StringWithLengthEx
#define DeeStringWriter_SpecWriteUtf16StringWithLengthEx      DeeUtf8StringWriter_SpecWriteUtf16StringWithLengthEx
#define DeeStringWriter_SpecWriteUtf32StringWithLengthEx      DeeUtf8StringWriter_SpecWriteUtf32StringWithLengthEx
#define DeeStringWriter_SpecWriteCharEx                       DeeUtf8StringWriter_SpecWriteCharEx
#define DeeStringWriter_SpecWriteMyCharEx                     DeeUtf8StringWriter_SpecWriteMyCharEx
#define DeeStringWriter_SpecWriteWideCharEx                   DeeUtf8StringWriter_SpecWriteWideCharEx
#define DeeStringWriter_SpecWriteUtf8CharEx                   DeeUtf8StringWriter_SpecWriteUtf8CharEx
#define DeeStringWriter_SpecWriteUtf16CharEx                  DeeUtf8StringWriter_SpecWriteUtf16CharEx
#define DeeStringWriter_SpecWriteUtf32CharEx                  DeeUtf8StringWriter_SpecWriteUtf32CharEx
#define DeeStringWriter_SpecWriteQuotedString                 DeeUtf8StringWriter_SpecWriteQuotedString
#define DeeStringWriter_SpecWriteQuotedMyString               DeeUtf8StringWriter_SpecWriteQuotedMyString
#define DeeStringWriter_SpecWriteQuotedWideString             DeeUtf8StringWriter_SpecWriteQuotedWideString
#define DeeStringWriter_SpecWriteQuotedUtf8String             DeeUtf8StringWriter_SpecWriteQuotedUtf8String
#define DeeStringWriter_SpecWriteQuotedUtf16String            DeeUtf8StringWriter_SpecWriteQuotedUtf16String
#define DeeStringWriter_SpecWriteQuotedUtf32String            DeeUtf8StringWriter_SpecWriteQuotedUtf32String
#define DeeStringWriter_SpecWriteQuotedStringWithLength       DeeUtf8StringWriter_SpecWriteQuotedStringWithLength
#define DeeStringWriter_SpecWriteQuotedMyStringWithLength     DeeUtf8StringWriter_SpecWriteQuotedMyStringWithLength
#define DeeStringWriter_SpecWriteQuotedWideStringWithLength   DeeUtf8StringWriter_SpecWriteQuotedWideStringWithLength
#define DeeStringWriter_SpecWriteQuotedUtf8StringWithLength   DeeUtf8StringWriter_SpecWriteQuotedUtf8StringWithLength
#define DeeStringWriter_SpecWriteQuotedUtf16StringWithLength  DeeUtf8StringWriter_SpecWriteQuotedUtf16StringWithLength
#define DeeStringWriter_SpecWriteQuotedUtf32StringWithLength  DeeUtf8StringWriter_SpecWriteQuotedUtf32StringWithLength
#define DeeStringWriter_SpecWriteDouble                       DeeUtf8StringWriter_SpecWriteDouble
#define DeeStringWriter_SpecWriteObject                       DeeUtf8StringWriter_SpecWriteObject
#define DeeStringWriter_SpecWriteFloat                        DeeUtf8StringWriter_SpecWriteFloat
#define DeeStringWriter_SpecWriteLDouble                      DeeUtf8StringWriter_SpecWriteLDouble
#define DeeStringWriter_SpecWriteInt8                         DeeUtf8StringWriter_SpecWriteInt8
#define DeeStringWriter_SpecWriteInt16                        DeeUtf8StringWriter_SpecWriteInt16
#define DeeStringWriter_SpecWriteInt32                        DeeUtf8StringWriter_SpecWriteInt32
#define DeeStringWriter_SpecWriteInt64                        DeeUtf8StringWriter_SpecWriteInt64
#define DeeStringWriter_SpecWriteUInt8                        DeeUtf8StringWriter_SpecWriteUInt8
#define DeeStringWriter_SpecWriteUInt16                       DeeUtf8StringWriter_SpecWriteUInt16
#define DeeStringWriter_SpecWriteUInt32                       DeeUtf8StringWriter_SpecWriteUInt32
#define DeeStringWriter_SpecWriteUInt64                       DeeUtf8StringWriter_SpecWriteUInt64
#define DeeStringWriter_WriteHexDump                          DeeUtf8StringWriter_WriteHexDump

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_STRING_FORWARD_H */
