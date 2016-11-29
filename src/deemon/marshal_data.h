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
#ifndef GUARD_DEEMON_MARSHAL_DATA_H
#define GUARD_DEEMON_MARSHAL_DATA_H 1

#include <deemon/__conf.inl>
#include <deemon/type.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/__bswap_intrin.inl>
#include <deemon/optional/string_forward.h>
#include <deemon/optional/uuid.h>
#include <deemon/optional/functionflags.h>

DEE_DECL_BEGIN
DEE_COMPILER_STRUCTURE_PACKED_BEGIN

//////////////////////////////////////////////////////////////////////////
// Initializer for internal UUIDs
//  - An internal UUID has all of its bytes set to 0, except
//    for the least significant one, which is then used as id
//    for the internal object, leaving deemon enough space for
//    up to 256 objects with intrinsic support for marshaling
//  - NOTE: Only '255' of those objects can actually be used for intrinsic
//          marshallable objects, as the id 256 (0xFF) marks a non-intrinsic
//          object in raw marshal data (s.a.: layout info below).
#define DeeUUID_INIT_INTERNAL(id)  {{0,0,0,0},{0,0},{0,0},{0,0},{0,0,0,0,0,id}}
#define DeeUUID_GET_INTERNAL(uuid)  (uuid)->uuid_e[5]
#ifdef DEE_PLATFORM_LIL_ENDIAN
#define DeeUUID_CHECK_INTERNAL(uuid) (((Dee_uint64_t *)(uuid))[0] == 0 && \
 (((Dee_uint64_t *)(uuid))[1] & DEE_UINT64_C(0x00FFFFFFFFFFFFFF)) == 0)
#else
#define DeeUUID_CHECK_INTERNAL(uuid) (((Dee_uint64_t *)(uuid))[0] == 0 && \
 (((Dee_uint64_t *)(uuid))[1] & DEE_UINT64_C(0xFFFFFFFFFFFFFF00)) == 0)
#endif

//////////////////////////////////////////////////////////////////////////
// Layout of raw marshal data
//  - Every marshal object is prefixed with one identifier
//    byte describing what kind of object is being dealt with.
//  - Depending on what that byte says, any number of following
//    bytes are apart of the current object.
//  - The byte also corresponds to the least significant byte of
//    the intrinsic type's uuid (as set by 'DeeUUID_INIT_INTERNAL')
//  - EXCEPTION:
//     - If the type-byte is 256 (0xFF), a non-intrinsic object
//       will be read, described by a 'struct DeeMarshalHeaderUser'
//       that will be read immediately after.
//

struct DeeMarshalHeaderUser_ {
 struct DeeUUID mhu_uuid;   /*< uuid of the type who's instance will follow. */
#define DEE_MASHALHEADER_USER_FLAGS_NONE   DEE_UINT8_C(0x00)
#define DEE_MASHALHEADER_USER_FLAGS_IGNORE DEE_UINT8_C(0x01) /*< Ignore this block (returning 'none') if the uuid is unknown. */
 Dee_uint8_t    mhu_flags;  /*< Flags associated with the data block (set of 'DEE_MASHALHEADER_USER_FLAGS_*'). */
 Dee_uint32_t   mhu_unused; /*< Unused (set to 0) (this will probably become an optional data hash, enabled with a flag). */
//Small64       mhu_size;   /*< Size of the object's chunk (starting at the end of this structure). */
// Dee_uint8_t  mhu_data[mhu_size]; /*< Object data. */
};
struct DeeMarshalHeaderUser {
 Dee_uint8_t                  mhu_marker; /*< [0xFF] Located immediately infront of this structure. */
 struct DeeMarshalHeaderUser_ mhu_block;  /*< User block. */
};


struct DeeMarshalHeaderBuiltin_ {
 Dee_uint16_t   mhb_id;     /*< Builtin object id. */
};
struct DeeMarshalHeaderBuiltin {
 Dee_uint8_t                     mhb_marker; /*< DEE_MARSHALID_BUILTIN. */
 struct DeeMarshalHeaderBuiltin_ mhb_block; /*< Builtin block. */
};

DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeMarshalHeaderPutRef_ {
 union{
  Dee_uint8_t   mhpr_ref8;   /*< 8-bit reference name. */
  Dee_uint16_t  mhpr_ref16;  /*< 16-bit reference name. */
  Dee_uint32_t  mhpr_ref32;  /*< 32-bit reference name. */
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define mhpr_ref8  _mhpr_refdata.mhpr_ref8
#define mhpr_ref16 _mhpr_refdata.mhpr_ref16
#define mhpr_ref32 _mhpr_refdata.mhpr_ref32
 _mhpr_refdata
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
 ;
};
DEE_COMPILER_MSVC_WARNING_POP
struct DeeMarshalHeaderPutRef {
 Dee_uint8_t                    mhpr_marker; /*< One of 'DEE_MARSHALID_PUTREF[8|16|32]'. */
 struct DeeMarshalHeaderPutRef_ mhpr_block;  /*< Put-ref block. */
};
#define DeeMarshalHeaderPutRef_SIZE(ob) (Dee_size_t)(\
 (ob)->mhpr_marker == DEE_MARSHALID_PUTREF8 ? 2 : \
 (ob)->mhpr_marker == DEE_MARSHALID_PUTREF16 ? 3 : 5)

DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeMarshalHeaderGetRef_ {
 union{
  Dee_uint8_t   mhgr_ref8;   /*< 8-bit reference name. */
  Dee_uint16_t  mhgr_ref16;  /*< 16-bit reference name. */
  Dee_uint32_t  mhgr_ref32;  /*< 32-bit reference name. */
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define mhgr_ref8  _mhgr_refdata.mhgr_ref8
#define mhgr_ref16 _mhgr_refdata.mhgr_ref16
#define mhgr_ref32 _mhgr_refdata.mhgr_ref32
 _mhgr_refdata
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
 ;
};
DEE_COMPILER_MSVC_WARNING_POP
struct DeeMarshalHeaderGetRef {
 Dee_uint8_t                    mhgr_marker; /*< One of 'DEE_MARSHALID_GETREF[8|16|32]'. */
 struct DeeMarshalHeaderGetRef_ mhgr_block;  /*< Get-ref block. */
};
#define DeeMarshalHeaderGetRef_SIZE(ob) (Dee_size_t)(\
 (ob)->mhgr_marker == DEE_MARSHALID_GETREF8 ? 2 : \
 (ob)->mhgr_marker == DEE_MARSHALID_GETREF16 ? 3 : 5)


struct DeeMarshalHeaderMarshalType_ {
 struct DeeUUID mhmt_uuid;   /*< The type's uuid. */
};
struct DeeMarshalHeaderMarshalType {
 Dee_uint8_t                         mhmt_marker; /*< DEE_MARSHALID_MARSHALTYPE. */
 struct DeeMarshalHeaderMarshalType_ mhmt_block;  /*< Marshal-type block. */
};

struct DeeMarshalHeaderForeignFunctionType_ {
 Dee_funflags_t mhfft_flags; /*< Function flags. */
 //Small64        mhfft_argc;  /*< Argument count. */
};
struct DeeMarshalHeaderForeignFunctionType {
 Dee_uint8_t                                 mhfft_marker; /*< DEE_MARSHALID_FOREIGNFUNCTIONTYPE. */
 struct DeeMarshalHeaderForeignFunctionType_ mhfft_block;  /*< Foreign-function block. */
};


struct DeeMarshalHeaderClass_ {
 struct DeeUUID mhc_uuid;    /*< Class uuid (set of 'DeeUUID_INIT_INTERNAL(DEE_MARSHALID_INSTANCE)' if not given). */
 //Small64 mhc_namelen;      /*< Length of the class name (set to 0 if not given). */
 //char    mhc_name[mhc_namelen];
 //object  mhc_base;
 // ... // Data, as read by '_deeclass_tp_marshal_ctor' / written by '_deeclass_tp_marshal_put'
};
#define DeeMarshalHeaderClass_HAS_UUID(ob) \
 (!DeeUUID_CHECK_INTERNAL(&(ob)->mhc_uuid) || \
   DeeUUID_GET_INTERNAL(&(ob)->mhc_uuid) != DEE_MARSHALID_INSTANCE)
struct DeeMarshalHeaderClass {
 Dee_uint8_t                   mhc_marker; /*< DEE_MARSHALID_CLASS. */
 struct DeeMarshalHeaderClass_ mhc_block;  /*< Foreign-function block. */
};


// Special marshal protocols
#define DEE_MARSHALID_NONE                0x00 /*< [+0] Dee_None. */
#define DEE_MARSHALID_BUILTIN             0x01 /*< [+2] ???. */
// NOTE: The object described by 'DEE_MARSHALID_PUTREF[8|16|32]' may not start with another reference declaration
// >> 'DEE_MARSHALID_PUTREF8,DEE_MARSHALID_PUTREF8,<object_data>' Is illegal
#define DEE_MARSHALID_GETREF8             0x02 /*< [+1] ???. // Read an object previously declared following 'DEE_MARSHALID_PUTREF[8|16|32]'. */
#define DEE_MARSHALID_PUTREF8             0x03 /*< [+1,+object] ???. // Give the following object a name that can later be re-referenced. */
#define DEE_MARSHALID_GETREF16            0x04 /*< [+2] ???. // Read an object previously declared following 'DEE_MARSHALID_PUTREF[8|16|32]'. */
#define DEE_MARSHALID_PUTREF16            0x05 /*< [+2,+object] ???. // Give the following object a name that can later be re-referenced. */
#define DEE_MARSHALID_GETREF32            0x06 /*< [+4] ???. // Read an object previously declared following 'DEE_MARSHALID_PUTREF[8|16|32]'. */
#define DEE_MARSHALID_PUTREF32            0x07 /*< [+4,+object] ???. // Give the following object a name that can later be re-referenced. */
#define DEE_MARSHALID_MARSHALTYPE         0x08 /*< [+16] ???. // Reference a registered marshal type by uuid. */
#define DEE_MARSHALID_POINTERTYPE         0x09 /*< [+object] ???. // Create a pointer type. */
#define DEE_MARSHALID_LVALUETYPE          0x0A /*< [+object] ???. // Create an lvalue type. */
#define DEE_MARSHALID_FOREIGNFUNCTIONTYPE 0x0B /*< [+8,+4,+object,+n*object] ???. // Create a foreign-function type. */
#define DEE_MARSHALID_ARRAYTYPE           0x0C /*< [+8,+object] ???. // Create an array type. */
#define DEE_MARSHALID_VARARRAYTYPE        0x0D /*< [+object] ???. // Create an variable-size array type. */
#define DEE_MARSHALID_CLASS               0x0E /*< [DeeMarshalHeaderClass_] DeeClassType_Type. // Create a new class type */
#define DEE_MARSHALID_INSTANCE            0x0F /*< [+object,+?] ???. // Read an object (type), then read an instance of it. */
#define DEE_MARSHALID_SHAREDLIB           0x10 /*< [+object] ???. // Read a shared library name, then open it. */
#define DEE_MARSHALID_DEXMODULE           0x11 /*< [+object] ???. // Read a dex name, then open it. */
#define DEE_MARSHALID_IGNORE              0x12 /*< [+object,+object] ???. // Read two objects, ignoring the first (NOTE: The ignored object will only be dereferenced after the second object has been read). */

// Intrinsic type marshal protocols
#define DEE_MARSHALTYPEIDMIN                    0x20
// HINT: After adding new marshal types, run 'deemon -F marshal_data.c'
//       to update 'DeeMashal_IntrinsicTypes'
#define DEE_MARSHALTYPEID_object                0x20 /*< [+0]  &DeeObject_Type. */
#define DEE_MARSHALTYPEID_tuple                 0x21 /*< [...] &DeeTuple_Type. */
#define DEE_MARSHALTYPEID_list                  0x22 /*< [...] &DeeList_Type. */
#define DEE_MARSHALTYPEID_set                   0x23 /*< [...] &DeeSet_Type. */
#define DEE_MARSHALTYPEID_dict                  0x24 /*< [...] &DeeDict_Type. */
#define DEE_MARSHALTYPEID_cell                  0x25 /*< [...] &DeeCell_Type. */
#define DEE_MARSHALTYPEID_int8                  0x26 /*< [+1]  &DeeInt8_Type. */
#define DEE_MARSHALTYPEID_int16                 0x27 /*< [+2]  &DeeInt16_Type. */
#define DEE_MARSHALTYPEID_int32                 0x28 /*< [+4]  &DeeInt32_Type. */
#define DEE_MARSHALTYPEID_int64                 0x29 /*< [+8]  &DeeInt64_Type. */
#define DEE_MARSHALTYPEID_uint8                 0x2A /*< [+1]  &DeeUInt8_Type. */
#define DEE_MARSHALTYPEID_uint16                0x2B /*< [+2]  &DeeUInt16_Type. */
#define DEE_MARSHALTYPEID_uint32                0x2C /*< [+4]  &DeeUInt32_Type. */
#define DEE_MARSHALTYPEID_uint64                0x2D /*< [+8]  &DeeUInt64_Type. */
#define DEE_MARSHALTYPEID_atomic_int8           0x2E /*< [+1]  &DeeAtomicInt8_Type. */
#define DEE_MARSHALTYPEID_atomic_int16          0x2F /*< [+2]  &DeeAtomicInt16_Type. */
#define DEE_MARSHALTYPEID_atomic_int32          0x30 /*< [+4]  &DeeAtomicInt32_Type. */
#define DEE_MARSHALTYPEID_atomic_int64          0x31 /*< [+8]  &DeeAtomicInt64_Type. */
#define DEE_MARSHALTYPEID_atomic_uint8          0x32 /*< [+1]  &DeeAtomicUInt8_Type. */
#define DEE_MARSHALTYPEID_atomic_uint16         0x33 /*< [+2]  &DeeAtomicUInt16_Type. */
#define DEE_MARSHALTYPEID_atomic_uint32         0x34 /*< [+4]  &DeeAtomicUInt32_Type. */
#define DEE_MARSHALTYPEID_atomic_uint64         0x35 /*< [+8]  &DeeAtomicUInt64_Type. */
#define DEE_MARSHALTYPEID_int8_range            0x36 /*< [+3]  &DeeRangeInt8Iterator_Type. */
#define DEE_MARSHALTYPEID_int16_range           0x37 /*< [+6]  &DeeRangeInt16Iterator_Type. */
#define DEE_MARSHALTYPEID_int32_range           0x38 /*< [+12] &DeeRangeInt32Iterator_Type. */
#define DEE_MARSHALTYPEID_int64_range           0x39 /*< [+24] &DeeRangeInt64Iterator_Type. */
#define DEE_MARSHALTYPEID_uint8_range           0x3A /*< [+3]  &DeeRangeUInt8Iterator_Type. */
#define DEE_MARSHALTYPEID_uint16_range          0x3B /*< [+6]  &DeeRangeUInt16Iterator_Type. */
#define DEE_MARSHALTYPEID_uint32_range          0x3C /*< [+12] &DeeRangeUInt32Iterator_Type. */
#define DEE_MARSHALTYPEID_uint64_range          0x3D /*< [+24] &DeeRangeUInt64Iterator_Type. */
#define DEE_MARSHALTYPEID_float4                0x3E /*< [+4]  DEE_MARSHALTYPE_FLOAT4. */
#define DEE_MARSHALTYPEID_float8                0x3F /*< [+8]  DEE_MARSHALTYPE_FLOAT8. */
#define DEE_MARSHALTYPEID_float12               0x40 /*< [+12] DEE_MARSHALTYPE_FLOAT12. */
#define DEE_MARSHALTYPEID_float16               0x41 /*< [+16] DEE_MARSHALTYPE_FLOAT16. */
#define DEE_MARSHALTYPEID_list_iterator         0x42 /*< [...] &DeeListIterator_Type. */
#define DEE_MARSHALTYPEID_Wide_string           0x43 /*< [...] DEE_MARSHALTYPE_WIDESTRING. */
#define DEE_MARSHALTYPEID_Utf8_string           0x44 /*< [...] DEE_MARSHALTYPE_UTF8STRING. */
#define DEE_MARSHALTYPEID_Utf16_string          0x45 /*< [...] DEE_MARSHALTYPE_UTF16STRING. */
#define DEE_MARSHALTYPEID_Utf32_string          0x46 /*< [...] DEE_MARSHALTYPE_UTF32STRING. */
#define DEE_MARSHALTYPEID_Wide_string_iterator  0x47 /*< [...] DEE_MARSHALTYPE_WIDESTRINGITERATOR. */
#define DEE_MARSHALTYPEID_Utf8_string_iterator  0x48 /*< [...] DEE_MARSHALTYPE_UTF8STRINGITERATOR. */
#define DEE_MARSHALTYPEID_Utf16_string_iterator 0x49 /*< [...] DEE_MARSHALTYPE_UTF16STRINGITERATOR. */
#define DEE_MARSHALTYPEID_Utf32_string_iterator 0x4A /*< [...] DEE_MARSHALTYPE_UTF32STRINGITERATOR. */
#define DEE_MARSHALTYPEID_foreign_function_closure 0x4B /*< [...] DEE_MARSHALTYPE_FOREIGN_FUNCTION_CLOSURE. */
#define DEE_MARSHALTYPEID_property              0x4C /*< [...] DEE_MARSHALTYPE_PROPERTY. */
#define DEE_MARSHALTYPEID_instance_method       0x4D /*< [...] DEE_MARSHALTYPE_INSTANCE_METHOD. */
#define DEE_MARSHALTYPEID_struct_type           0x4E /*< [...] &DeeStructType_Type. */
#define DEE_MARSHALTYPEID_dict_keys             0x4F /*< [...] &DeeDictKeys_Type. */
#define DEE_MARSHALTYPEID_dict_items            0x50 /*< [...] &DeeDictItems_Type. */
#define DEE_MARSHALTYPEID_dict_values           0x51 /*< [...] &DeeDictValues_Type. */
#define DEE_MARSHALTYPEID_code_debuginfo        0x52 /*< [...] &DeeCodeDebugInfo_Type. */
#define DEE_MARSHALTYPEID_code                  0x53 /*< [...] &DeeCode_Type. */
#define DEE_MARSHALTYPEID_function              0x54 /*< [...] &DeeFunction_Type. */
#define DEE_MARSHALTYPEID_yield_function        0x55 /*< [...] &DeeYieldFunction_Type. */
#define DEE_MARSHALTYPEID_yield_function_iterator 0x56 /*< [TODO...] &DeeYieldFunctionIterator_Type. */
#define DEE_MARSHALTYPEID_tuple_iterator        0x57 /*< [...] &DeeTupleIterator_Type. */
#define DEE_MARSHALTYPEID_random                0x58 /*< [...] &DeeRandom_Type. */
#define DEE_MARSHALTYPEID_file                  0x59 /*< [...] &DeeFile_Type. */
#define DEE_MARSHALTYPEID_file_writer           0x5A /*< [...] &DeeFileWriter_Type. */
#define DEE_MARSHALTYPEID_file_reader           0x5B /*< [...] &DeeFileReader_Type. */
#define DEE_MARSHALTYPEID_file_joined           0x5C /*< [...] &DeeFileJoined_Type. */
#define DEE_MARSHALTYPEID_file_iterator         0x5D /*< [...] &DeeFileIterator_Type. */
#define DEE_MARSHALTYPEID_zip_iterator          0x5E /*< [...] &DeeZipIterator_Type. */
#define DEE_MARSHALTYPEID_weakref               0x5F /*< [...] &DeeWeakref_Type. */
#define DEE_MARSHALTYPEID_time                  0x60 /*< [...] &DeeTime_Type. */
#define DEE_MARSHALTYPEID_generic_range_iterator 0x61 /*< [...] &DeeGenericRangeIterator_Type. */
#define DEE_MARSHALTYPEID_generic_sequence_iterator 0x62 /*< [...] &DeeGenericSequenceIterator_Type. */
#define DEE_MARSHALTYPEID_subrange_iterator     0x63 /*< [...] &DeeSubRangeIterator_Type. */
#define DEE_MARSHALTYPEID_traceback             0x64 /*< [...] &DeeTraceback_Type. */
#define DEE_MARSHALTYPEID_traceback_iterator    0x65 /*< [...] &DeeTracebackIterator_Type. */
#define DEE_MARSHALTYPEID_traceback_entry       0x66 /*< [...] &DeeTracebackEntry_Type. */
#define DEE_MARSHALTYPEID_memberfunction        0x67 /*< [...] &DeeMemberFunction_Type. */
#define DEE_MARSHALTYPEID_enumiterator          0x68 /*< [...] &DeeEnumIterator_Type. */
// TODO: DeeDictIterator_Type
// TODO: DeeDictKeysIterator_Type
// TODO: DeeDictItemsIterator_Type
// TODO: DeeSetIterator_Type

#define DEE_MARSHALTYPEID2(name) DEE_MARSHALTYPEID_##name
#define DEE_MARSHALTYPEID(name) DEE_MARSHALTYPEID2(name)
#define DEE_MARSHALID_NONINTRINSIC 0xFF /*< Non-intrinsic object id. */

#if DEE_CONFIG_RT_SIZEOF_FLOAT == 4
# define DEE_MARSHALTYPE_FLOAT4    &DeeFloat_Type
# define DEE_MARSHALTYPE_FLOAT4_ID DEE_TYPES_FLOATID_FLOAT
#elif DEE_CONFIG_RT_SIZEOF_DOUBLE == 4
# define DEE_MARSHALTYPE_FLOAT4    &DeeDouble_Type
# define DEE_MARSHALTYPE_FLOAT4_ID DEE_TYPES_FLOATID_DOUBLE
#elif DEE_CONFIG_RT_SIZEOF_LDOUBLE == 4
# define DEE_MARSHALTYPE_FLOAT4    &DeeLDouble_Type
# define DEE_MARSHALTYPE_FLOAT4_ID DEE_TYPES_FLOATID_LDOUBLE
#endif

#if DEE_CONFIG_RT_SIZEOF_FLOAT == 8
# define DEE_MARSHALTYPE_FLOAT8    &DeeFloat_Type
# define DEE_MARSHALTYPE_FLOAT8_ID DEE_TYPES_FLOATID_FLOAT
#elif DEE_CONFIG_RT_SIZEOF_DOUBLE == 8
# define DEE_MARSHALTYPE_FLOAT8    &DeeDouble_Type
# define DEE_MARSHALTYPE_FLOAT8_ID DEE_TYPES_FLOATID_DOUBLE
#elif DEE_CONFIG_RT_SIZEOF_LDOUBLE == 8
# define DEE_MARSHALTYPE_FLOAT8    &DeeLDouble_Type
# define DEE_MARSHALTYPE_FLOAT8_ID DEE_TYPES_FLOATID_LDOUBLE
#endif

#if DEE_CONFIG_RT_SIZEOF_FLOAT == 12
# define DEE_MARSHALTYPE_FLOAT12    &DeeFloat_Type
# define DEE_MARSHALTYPE_FLOAT12_ID DEE_TYPES_FLOATID_FLOAT
#elif DEE_CONFIG_RT_SIZEOF_DOUBLE == 12
# define DEE_MARSHALTYPE_FLOAT12    &DeeDouble_Type
# define DEE_MARSHALTYPE_FLOAT12_ID DEE_TYPES_FLOATID_DOUBLE
#elif DEE_CONFIG_RT_SIZEOF_LDOUBLE == 12
# define DEE_MARSHALTYPE_FLOAT12    &DeeLDouble_Type
# define DEE_MARSHALTYPE_FLOAT12_ID DEE_TYPES_FLOATID_LDOUBLE
#endif

#if DEE_CONFIG_RT_SIZEOF_FLOAT == 16
# define DEE_MARSHALTYPE_FLOAT16    &DeeFloat_Type
# define DEE_MARSHALTYPE_FLOAT16_ID DEE_TYPES_FLOATID_FLOAT
#elif DEE_CONFIG_RT_SIZEOF_DOUBLE == 16
# define DEE_MARSHALTYPE_FLOAT16    &DeeDouble_Type
# define DEE_MARSHALTYPE_FLOAT16_ID DEE_TYPES_FLOATID_DOUBLE
#elif DEE_CONFIG_RT_SIZEOF_LDOUBLE == 16
# define DEE_MARSHALTYPE_FLOAT16    &DeeLDouble_Type
# define DEE_MARSHALTYPE_FLOAT16_ID DEE_TYPES_FLOATID_LDOUBLE
#endif



#if DEE_CONFIG_HAVE_ENCODING_WIDE
#define DEE_MARSHALTYPE_WIDESTRING         &DeeWideString_Type
#define DEE_MARSHALTYPE_WIDESTRINGITERATOR &DeeWideStringIterator_Type
#else
#define DEE_MARSHALTYPE_WIDESTRING         NULL
#define DEE_MARSHALTYPE_WIDESTRINGITERATOR NULL
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF8
#define DEE_MARSHALTYPE_UTF8STRING         &DeeUtf8String_Type
#define DEE_MARSHALTYPE_UTF8STRINGITERATOR &DeeUtf8StringIterator_Type
#else
#define DEE_MARSHALTYPE_UTF8STRING         NULL
#define DEE_MARSHALTYPE_UTF8STRINGITERATOR NULL
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
#define DEE_MARSHALTYPE_UTF16STRING         &DeeUtf16String_Type
#define DEE_MARSHALTYPE_UTF16STRINGITERATOR &DeeUtf16StringIterator_Type
#else
#define DEE_MARSHALTYPE_UTF16STRING         NULL
#define DEE_MARSHALTYPE_UTF16STRINGITERATOR NULL
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
#define DEE_MARSHALTYPE_UTF32STRING         &DeeUtf32String_Type
#define DEE_MARSHALTYPE_UTF32STRINGITERATOR &DeeUtf32StringIterator_Type
#else
#define DEE_MARSHALTYPE_UTF32STRING         NULL
#define DEE_MARSHALTYPE_UTF32STRINGITERATOR NULL
#endif

#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
#define DEE_MARSHALTYPE_FOREIGN_FUNCTION_CLOSURE &DeeForeignFunctionClosure_Type
#else /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */
#define DEE_MARSHALTYPE_FOREIGN_FUNCTION_CLOSURE NULL
#endif /* !DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */

#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
#define DEE_MARSHALTYPE_PROPERTY            &DeeProperty_Type
#define DEE_MARSHALTYPE_INSTANCE_METHOD     &DeeInstanceMethod_Type
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES */
#define DEE_MARSHALTYPE_PROPERTY            NULL
#define DEE_MARSHALTYPE_INSTANCE_METHOD     NULL
#endif /* DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES */


// List of all intrinsic types available through uuids.
// index == intrinsic type id (as set with 'DeeUUID_INIT_INTERNAL')
// NOTE: Ids not mapping to a specific type (such as 'DEE_MARSHALID_NONE' or 'DEE_MARSHALID_BUILTIN' as set to NULL in here)
extern DeeTypeObject *const DeeMashal_IntrinsicTypes[256];
#define DeeMarshal_LookupIntrinsicType(id) DeeMashal_IntrinsicTypes[id]


//////////////////////////////////////////////////////////////////////////
// Marshaling helper functions

//////////////////////////////////////////////////////////////////////////
// Read/write a 64-bit integer in as small memory as possible
//  - These functions will only use as much memory as required to store the value +1 control byte,
//    meaning that except for very large values, you will always conserve memory.
//  NOTE: To have the small64 actually extend into the next types, the 'DEE_FILE_SMALL64_XFLAG' flag
//        must be present in the marker (aka. the first bit must be on).
//        If it isn't, the marker itself _IS_ the value.
#define DEE_FILE_SMALL64_XFLAG    0x80
#define DEE_FILE_SMALL64_KIND_0  (DEE_FILE_SMALL64_XFLAG|0) // +0 bytes
#define DEE_FILE_SMALL64_KIND_8  (DEE_FILE_SMALL64_XFLAG|1) // +1 byte
#define DEE_FILE_SMALL64_KIND_16 (DEE_FILE_SMALL64_XFLAG|2) // +2 bytes
#define DEE_FILE_SMALL64_KIND_24 (DEE_FILE_SMALL64_XFLAG|3) // +3 bytes
#define DEE_FILE_SMALL64_KIND_32 (DEE_FILE_SMALL64_XFLAG|4) // +4 bytes
#define DEE_FILE_SMALL64_KIND_40 (DEE_FILE_SMALL64_XFLAG|5) // +5 bytes
#define DEE_FILE_SMALL64_KIND_48 (DEE_FILE_SMALL64_XFLAG|6) // +6 bytes
#define DEE_FILE_SMALL64_KIND_56 (DEE_FILE_SMALL64_XFLAG|7) // +7 bytes
#define DEE_FILE_SMALL64_KIND_64 (DEE_FILE_SMALL64_XFLAG|8) // +8 bytes
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeSmall64 {
 Dee_uint8_t   sl64_marker; /*< If if 'DEE_FILE_SMALL64_XFLAG' flag isn't set, this _IS_ the value. */
 union{
  Dee_uint64_t sl64_lil_value;
  Dee_uint8_t  sl64_lil_bytes[8];
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define sl64_lil_value  _sl64_bytedata.sl64_lil_value
#define sl64_lil_bytes  _sl64_bytedata.sl64_lil_bytes
 _sl64_bytedata
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
 ;
};
DEE_COMPILER_MSVC_WARNING_POP

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int)
DeeFile_PutLeSmall64(DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_uint64_t value) {
 struct DeeSmall64 small64;
 //if (!value) return DeeFile_Putc(self,DEE_FILE_SMALL64_KIND_0);
 small64.sl64_lil_value = DEE_BUILTIN_LESWAP64(value);
 if (value < (Dee_uint64_t)(Dee_uint8_t)(0xFF&~DEE_FILE_SMALL64_XFLAG)) {
  // Special handling for numbers that (without the extended marker) we can fit into 1 byte
  small64.sl64_marker = (Dee_uint8_t)value;
  return DeeFile_WriteAll(self,&small64,1);
 } else
 // Little endian stores its least significant bit at the front.
 // So all we have to do now, is figure out what the last non-zero slot is.
      if (small64.sl64_lil_bytes[7]) small64.sl64_marker = DEE_FILE_SMALL64_KIND_64;
 else if (small64.sl64_lil_bytes[6]) small64.sl64_marker = DEE_FILE_SMALL64_KIND_56;
 else if (small64.sl64_lil_bytes[5]) small64.sl64_marker = DEE_FILE_SMALL64_KIND_48;
 else if (small64.sl64_lil_bytes[4]) small64.sl64_marker = DEE_FILE_SMALL64_KIND_40;
 else if (small64.sl64_lil_bytes[3]) small64.sl64_marker = DEE_FILE_SMALL64_KIND_32;
 else if (small64.sl64_lil_bytes[2]) small64.sl64_marker = DEE_FILE_SMALL64_KIND_24;
 else if (small64.sl64_lil_bytes[1]) small64.sl64_marker = DEE_FILE_SMALL64_KIND_16;
 else if (small64.sl64_lil_bytes[0]) small64.sl64_marker = DEE_FILE_SMALL64_KIND_8;
 else small64.sl64_marker = DEE_FILE_SMALL64_KIND_0;
 // Currently, the markers indicate how many additional bytes should be written.
 // While not written in stone, we do take advantage of that fact.
 return DeeFile_WriteAll(self,&small64,sizeof(small64.sl64_marker)+
                        (small64.sl64_marker&~(DEE_FILE_SMALL64_XFLAG)));
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int)
DeeFile_GetLeSmall64(DEE_A_INOUT DeeObject *self, DEE_A_OUT Dee_uint64_t *value) {
 struct DeeSmall64 small64;
 if (DeeFile_Getc(self,&small64.sl64_marker) != 0) return -1;
 // If there is no extension marker, we've already got our value
 if ((small64.sl64_marker&DEE_FILE_SMALL64_XFLAG)==0) {
  *value = (Dee_uint64_t)small64.sl64_marker;
  return 0;
 }
 if (small64.sl64_marker > DEE_FILE_SMALL64_KIND_64) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Invalid marker in small64: 0x%.2I8x(%I8u)",
                      small64.sl64_marker,small64.sl64_marker);
  return -1;
 }
 // Read the marked bytes
 small64.sl64_lil_value = 0; // undefined bytes are set to '0'
 if (DeeFile_ReadAll(self,small64.sl64_lil_bytes,(
  Dee_size_t)(small64.sl64_marker&~(DEE_FILE_SMALL64_XFLAG))) != 0) return -1;
 *value = DEE_BUILTIN_LESWAP64(small64.sl64_lil_value);
 return 0;
}

DEE_COMPILER_STRUCTURE_PACKED_END


#define DEE_MARSHAL_ENDIAN_BYTECODE 1234 /*< Endian used for opargs in bytecode */


DEE_DECL_END

#endif /* !GUARD_DEEMON_MARSHAL_DATA_H */
