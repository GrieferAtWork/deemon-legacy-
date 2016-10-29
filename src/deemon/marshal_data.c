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
#ifndef GUARD_DEEMON_MARSHAL_DATA_C
#define GUARD_DEEMON_MARSHAL_DATA_C 1
#define DEE_LIMITED_API 1
#ifdef __DEEMON__
#pragma warning(disable: 16)
#endif

// /include/*
#include <deemon/__conf.inl>
#include <deemon/cell.h>
#include <deemon/class.h>
#include <deemon/compiler/code.h>
#include <deemon/dict.h>
#include <deemon/float.h>
#include <deemon/foreign_function.h>
#include <deemon/integer.h>
#include <deemon/iterator.h>
#include <deemon/list.h>
#include <deemon/memberfunction.h>
#include <deemon/object.h>
#include <deemon/random.h>
#include <deemon/runtime/function.h>
#include <deemon/set.h>
#include <deemon/struct.h>
#include <deemon/time.h>
#include <deemon/traceback.h>
#include <deemon/tuple.h>
#include <deemon/weakref.h>

// /src/*
#include <deemon/marshal_data.h>

DEE_DECL_BEGIN

#ifndef DEE_MARSHALTYPE_FLOAT4
#define DEE_MARSHALTYPE_FLOAT4  NULL
#endif
#ifndef DEE_MARSHALTYPE_FLOAT8
#define DEE_MARSHALTYPE_FLOAT8  NULL
#endif
#ifndef DEE_MARSHALTYPE_FLOAT12
#define DEE_MARSHALTYPE_FLOAT12 NULL
#endif
#ifndef DEE_MARSHALTYPE_FLOAT16
#define DEE_MARSHALTYPE_FLOAT16 NULL
#endif

DeeTypeObject *const DeeMashal_IntrinsicTypes[256] = {
/*[[[deemon
#include <file>
#include <fs>
#include <time>
local header = file.io(fs::path::join(fs::path::head(__FILE__),"marshal_data.h"));
local typeids = ["NULL"]*256;
print " /" "* Generate at: %s *" "/" % str time.now();
for (local line: header) {
  line = line.strip();
  if (line.startswith("#define")) {
    line = line[#"#define":].strip();
    if (line.startswith("DEE_MARSHALTYPEID_")) {
      line = line[#"DEE_MARSHALTYPEID_":];
      try {
        local none,id,none,tyname = line.scanf("%[^ ] %[Xx0-9a-fA-F] /*< [ %[^\\]] ] %[^.]")...;
        //if (!tyname.startswith("&")) tyname = "&"+tyname;
        id = (int)id;
        if (typeids[id] != "NULL") {
          print file.io.stderr: "ID",id,"was already used:";
          print file.io.stderr: "\tOld:",typeids[id];
          print file.io.stderr: "\tnew:",tyname;
        }
        typeids[id] = tyname;
      } catch (e...) {
        print file.io.stderr: e;
        print file.io.stderr: repr line;
      }
    }
  }
}
for (local i = 0; i < #typeids; ++i) {
  print " /" "* 0x%.2X *" "/ (DeeTypeObject *)%s," % (i,typeids[i]);
}
]]]*/
 /* Generate at: 29.09.2016 19:04:02 */
 /* 0x00 */ (DeeTypeObject *)NULL,
 /* 0x01 */ (DeeTypeObject *)NULL,
 /* 0x02 */ (DeeTypeObject *)NULL,
 /* 0x03 */ (DeeTypeObject *)NULL,
 /* 0x04 */ (DeeTypeObject *)NULL,
 /* 0x05 */ (DeeTypeObject *)NULL,
 /* 0x06 */ (DeeTypeObject *)NULL,
 /* 0x07 */ (DeeTypeObject *)NULL,
 /* 0x08 */ (DeeTypeObject *)NULL,
 /* 0x09 */ (DeeTypeObject *)NULL,
 /* 0x0A */ (DeeTypeObject *)NULL,
 /* 0x0B */ (DeeTypeObject *)NULL,
 /* 0x0C */ (DeeTypeObject *)NULL,
 /* 0x0D */ (DeeTypeObject *)NULL,
 /* 0x0E */ (DeeTypeObject *)NULL,
 /* 0x0F */ (DeeTypeObject *)NULL,
 /* 0x10 */ (DeeTypeObject *)NULL,
 /* 0x11 */ (DeeTypeObject *)NULL,
 /* 0x12 */ (DeeTypeObject *)NULL,
 /* 0x13 */ (DeeTypeObject *)NULL,
 /* 0x14 */ (DeeTypeObject *)NULL,
 /* 0x15 */ (DeeTypeObject *)NULL,
 /* 0x16 */ (DeeTypeObject *)NULL,
 /* 0x17 */ (DeeTypeObject *)NULL,
 /* 0x18 */ (DeeTypeObject *)NULL,
 /* 0x19 */ (DeeTypeObject *)NULL,
 /* 0x1A */ (DeeTypeObject *)NULL,
 /* 0x1B */ (DeeTypeObject *)NULL,
 /* 0x1C */ (DeeTypeObject *)NULL,
 /* 0x1D */ (DeeTypeObject *)NULL,
 /* 0x1E */ (DeeTypeObject *)NULL,
 /* 0x1F */ (DeeTypeObject *)NULL,
 /* 0x20 */ (DeeTypeObject *)&DeeObject_Type,
 /* 0x21 */ (DeeTypeObject *)&DeeTuple_Type,
 /* 0x22 */ (DeeTypeObject *)&DeeList_Type,
 /* 0x23 */ (DeeTypeObject *)&DeeSet_Type,
 /* 0x24 */ (DeeTypeObject *)&DeeDict_Type,
 /* 0x25 */ (DeeTypeObject *)&DeeCell_Type,
 /* 0x26 */ (DeeTypeObject *)&DeeInt8_Type,
 /* 0x27 */ (DeeTypeObject *)&DeeInt16_Type,
 /* 0x28 */ (DeeTypeObject *)&DeeInt32_Type,
 /* 0x29 */ (DeeTypeObject *)&DeeInt64_Type,
 /* 0x2A */ (DeeTypeObject *)&DeeUInt8_Type,
 /* 0x2B */ (DeeTypeObject *)&DeeUInt16_Type,
 /* 0x2C */ (DeeTypeObject *)&DeeUInt32_Type,
 /* 0x2D */ (DeeTypeObject *)&DeeUInt64_Type,
 /* 0x2E */ (DeeTypeObject *)&DeeAtomicInt8_Type,
 /* 0x2F */ (DeeTypeObject *)&DeeAtomicInt16_Type,
 /* 0x30 */ (DeeTypeObject *)&DeeAtomicInt32_Type,
 /* 0x31 */ (DeeTypeObject *)&DeeAtomicInt64_Type,
 /* 0x32 */ (DeeTypeObject *)&DeeAtomicUInt8_Type,
 /* 0x33 */ (DeeTypeObject *)&DeeAtomicUInt16_Type,
 /* 0x34 */ (DeeTypeObject *)&DeeAtomicUInt32_Type,
 /* 0x35 */ (DeeTypeObject *)&DeeAtomicUInt64_Type,
 /* 0x36 */ (DeeTypeObject *)&DeeRangeInt8Iterator_Type,
 /* 0x37 */ (DeeTypeObject *)&DeeRangeInt16Iterator_Type,
 /* 0x38 */ (DeeTypeObject *)&DeeRangeInt32Iterator_Type,
 /* 0x39 */ (DeeTypeObject *)&DeeRangeInt64Iterator_Type,
 /* 0x3A */ (DeeTypeObject *)&DeeRangeUInt8Iterator_Type,
 /* 0x3B */ (DeeTypeObject *)&DeeRangeUInt16Iterator_Type,
 /* 0x3C */ (DeeTypeObject *)&DeeRangeUInt32Iterator_Type,
 /* 0x3D */ (DeeTypeObject *)&DeeRangeUInt64Iterator_Type,
 /* 0x3E */ (DeeTypeObject *)DEE_MARSHALTYPE_FLOAT4,
 /* 0x3F */ (DeeTypeObject *)DEE_MARSHALTYPE_FLOAT8,
 /* 0x40 */ (DeeTypeObject *)DEE_MARSHALTYPE_FLOAT12,
 /* 0x41 */ (DeeTypeObject *)DEE_MARSHALTYPE_FLOAT16,
 /* 0x42 */ (DeeTypeObject *)&DeeListIterator_Type,
 /* 0x43 */ (DeeTypeObject *)DEE_MARSHALTYPE_WIDESTRING,
 /* 0x44 */ (DeeTypeObject *)DEE_MARSHALTYPE_UTF8STRING,
 /* 0x45 */ (DeeTypeObject *)DEE_MARSHALTYPE_UTF16STRING,
 /* 0x46 */ (DeeTypeObject *)DEE_MARSHALTYPE_UTF32STRING,
 /* 0x47 */ (DeeTypeObject *)DEE_MARSHALTYPE_WIDESTRINGITERATOR,
 /* 0x48 */ (DeeTypeObject *)DEE_MARSHALTYPE_UTF8STRINGITERATOR,
 /* 0x49 */ (DeeTypeObject *)DEE_MARSHALTYPE_UTF16STRINGITERATOR,
 /* 0x4A */ (DeeTypeObject *)DEE_MARSHALTYPE_UTF32STRINGITERATOR,
 /* 0x4B */ (DeeTypeObject *)DEE_MARSHALTYPE_FOREIGN_FUNCTION_CLOSURE,
 /* 0x4C */ (DeeTypeObject *)DEE_MARSHALTYPE_PROPERTY,
 /* 0x4D */ (DeeTypeObject *)DEE_MARSHALTYPE_INSTANCE_METHOD,
 /* 0x4E */ (DeeTypeObject *)&DeeStructType_Type,
 /* 0x4F */ (DeeTypeObject *)&DeeDictKeys_Type,
 /* 0x50 */ (DeeTypeObject *)&DeeDictItems_Type,
 /* 0x51 */ (DeeTypeObject *)&DeeDictValues_Type,
 /* 0x52 */ (DeeTypeObject *)&DeeCodeDebugInfo_Type,
 /* 0x53 */ (DeeTypeObject *)&DeeCode_Type,
 /* 0x54 */ (DeeTypeObject *)&DeeFunction_Type,
 /* 0x55 */ (DeeTypeObject *)&DeeYieldFunction_Type,
 /* 0x56 */ (DeeTypeObject *)&DeeYieldFunctionIterator_Type,
 /* 0x57 */ (DeeTypeObject *)&DeeTupleIterator_Type,
 /* 0x58 */ (DeeTypeObject *)&DeeRandom_Type,
 /* 0x59 */ (DeeTypeObject *)&DeeFile_Type,
 /* 0x5A */ (DeeTypeObject *)&DeeFileWriter_Type,
 /* 0x5B */ (DeeTypeObject *)&DeeFileReader_Type,
 /* 0x5C */ (DeeTypeObject *)&DeeFileJoined_Type,
 /* 0x5D */ (DeeTypeObject *)&DeeFileIterator_Type,
 /* 0x5E */ (DeeTypeObject *)&DeeZipIterator_Type,
 /* 0x5F */ (DeeTypeObject *)&DeeWeakref_Type,
 /* 0x60 */ (DeeTypeObject *)&DeeTime_Type,
 /* 0x61 */ (DeeTypeObject *)&DeeGenericRangeIterator_Type,
 /* 0x62 */ (DeeTypeObject *)&DeeGenericSequenceIterator_Type,
 /* 0x63 */ (DeeTypeObject *)&DeeSubRangeIterator_Type,
 /* 0x64 */ (DeeTypeObject *)&DeeTraceback_Type,
 /* 0x65 */ (DeeTypeObject *)&DeeTracebackIterator_Type,
 /* 0x66 */ (DeeTypeObject *)&DeeTracebackEntry_Type,
 /* 0x67 */ (DeeTypeObject *)&DeeMemberFunction_Type,
 /* 0x68 */ (DeeTypeObject *)NULL,
 /* 0x69 */ (DeeTypeObject *)NULL,
 /* 0x6A */ (DeeTypeObject *)NULL,
 /* 0x6B */ (DeeTypeObject *)NULL,
 /* 0x6C */ (DeeTypeObject *)NULL,
 /* 0x6D */ (DeeTypeObject *)NULL,
 /* 0x6E */ (DeeTypeObject *)NULL,
 /* 0x6F */ (DeeTypeObject *)NULL,
 /* 0x70 */ (DeeTypeObject *)NULL,
 /* 0x71 */ (DeeTypeObject *)NULL,
 /* 0x72 */ (DeeTypeObject *)NULL,
 /* 0x73 */ (DeeTypeObject *)NULL,
 /* 0x74 */ (DeeTypeObject *)NULL,
 /* 0x75 */ (DeeTypeObject *)NULL,
 /* 0x76 */ (DeeTypeObject *)NULL,
 /* 0x77 */ (DeeTypeObject *)NULL,
 /* 0x78 */ (DeeTypeObject *)NULL,
 /* 0x79 */ (DeeTypeObject *)NULL,
 /* 0x7A */ (DeeTypeObject *)NULL,
 /* 0x7B */ (DeeTypeObject *)NULL,
 /* 0x7C */ (DeeTypeObject *)NULL,
 /* 0x7D */ (DeeTypeObject *)NULL,
 /* 0x7E */ (DeeTypeObject *)NULL,
 /* 0x7F */ (DeeTypeObject *)NULL,
 /* 0x80 */ (DeeTypeObject *)NULL,
 /* 0x81 */ (DeeTypeObject *)NULL,
 /* 0x82 */ (DeeTypeObject *)NULL,
 /* 0x83 */ (DeeTypeObject *)NULL,
 /* 0x84 */ (DeeTypeObject *)NULL,
 /* 0x85 */ (DeeTypeObject *)NULL,
 /* 0x86 */ (DeeTypeObject *)NULL,
 /* 0x87 */ (DeeTypeObject *)NULL,
 /* 0x88 */ (DeeTypeObject *)NULL,
 /* 0x89 */ (DeeTypeObject *)NULL,
 /* 0x8A */ (DeeTypeObject *)NULL,
 /* 0x8B */ (DeeTypeObject *)NULL,
 /* 0x8C */ (DeeTypeObject *)NULL,
 /* 0x8D */ (DeeTypeObject *)NULL,
 /* 0x8E */ (DeeTypeObject *)NULL,
 /* 0x8F */ (DeeTypeObject *)NULL,
 /* 0x90 */ (DeeTypeObject *)NULL,
 /* 0x91 */ (DeeTypeObject *)NULL,
 /* 0x92 */ (DeeTypeObject *)NULL,
 /* 0x93 */ (DeeTypeObject *)NULL,
 /* 0x94 */ (DeeTypeObject *)NULL,
 /* 0x95 */ (DeeTypeObject *)NULL,
 /* 0x96 */ (DeeTypeObject *)NULL,
 /* 0x97 */ (DeeTypeObject *)NULL,
 /* 0x98 */ (DeeTypeObject *)NULL,
 /* 0x99 */ (DeeTypeObject *)NULL,
 /* 0x9A */ (DeeTypeObject *)NULL,
 /* 0x9B */ (DeeTypeObject *)NULL,
 /* 0x9C */ (DeeTypeObject *)NULL,
 /* 0x9D */ (DeeTypeObject *)NULL,
 /* 0x9E */ (DeeTypeObject *)NULL,
 /* 0x9F */ (DeeTypeObject *)NULL,
 /* 0xA0 */ (DeeTypeObject *)NULL,
 /* 0xA1 */ (DeeTypeObject *)NULL,
 /* 0xA2 */ (DeeTypeObject *)NULL,
 /* 0xA3 */ (DeeTypeObject *)NULL,
 /* 0xA4 */ (DeeTypeObject *)NULL,
 /* 0xA5 */ (DeeTypeObject *)NULL,
 /* 0xA6 */ (DeeTypeObject *)NULL,
 /* 0xA7 */ (DeeTypeObject *)NULL,
 /* 0xA8 */ (DeeTypeObject *)NULL,
 /* 0xA9 */ (DeeTypeObject *)NULL,
 /* 0xAA */ (DeeTypeObject *)NULL,
 /* 0xAB */ (DeeTypeObject *)NULL,
 /* 0xAC */ (DeeTypeObject *)NULL,
 /* 0xAD */ (DeeTypeObject *)NULL,
 /* 0xAE */ (DeeTypeObject *)NULL,
 /* 0xAF */ (DeeTypeObject *)NULL,
 /* 0xB0 */ (DeeTypeObject *)NULL,
 /* 0xB1 */ (DeeTypeObject *)NULL,
 /* 0xB2 */ (DeeTypeObject *)NULL,
 /* 0xB3 */ (DeeTypeObject *)NULL,
 /* 0xB4 */ (DeeTypeObject *)NULL,
 /* 0xB5 */ (DeeTypeObject *)NULL,
 /* 0xB6 */ (DeeTypeObject *)NULL,
 /* 0xB7 */ (DeeTypeObject *)NULL,
 /* 0xB8 */ (DeeTypeObject *)NULL,
 /* 0xB9 */ (DeeTypeObject *)NULL,
 /* 0xBA */ (DeeTypeObject *)NULL,
 /* 0xBB */ (DeeTypeObject *)NULL,
 /* 0xBC */ (DeeTypeObject *)NULL,
 /* 0xBD */ (DeeTypeObject *)NULL,
 /* 0xBE */ (DeeTypeObject *)NULL,
 /* 0xBF */ (DeeTypeObject *)NULL,
 /* 0xC0 */ (DeeTypeObject *)NULL,
 /* 0xC1 */ (DeeTypeObject *)NULL,
 /* 0xC2 */ (DeeTypeObject *)NULL,
 /* 0xC3 */ (DeeTypeObject *)NULL,
 /* 0xC4 */ (DeeTypeObject *)NULL,
 /* 0xC5 */ (DeeTypeObject *)NULL,
 /* 0xC6 */ (DeeTypeObject *)NULL,
 /* 0xC7 */ (DeeTypeObject *)NULL,
 /* 0xC8 */ (DeeTypeObject *)NULL,
 /* 0xC9 */ (DeeTypeObject *)NULL,
 /* 0xCA */ (DeeTypeObject *)NULL,
 /* 0xCB */ (DeeTypeObject *)NULL,
 /* 0xCC */ (DeeTypeObject *)NULL,
 /* 0xCD */ (DeeTypeObject *)NULL,
 /* 0xCE */ (DeeTypeObject *)NULL,
 /* 0xCF */ (DeeTypeObject *)NULL,
 /* 0xD0 */ (DeeTypeObject *)NULL,
 /* 0xD1 */ (DeeTypeObject *)NULL,
 /* 0xD2 */ (DeeTypeObject *)NULL,
 /* 0xD3 */ (DeeTypeObject *)NULL,
 /* 0xD4 */ (DeeTypeObject *)NULL,
 /* 0xD5 */ (DeeTypeObject *)NULL,
 /* 0xD6 */ (DeeTypeObject *)NULL,
 /* 0xD7 */ (DeeTypeObject *)NULL,
 /* 0xD8 */ (DeeTypeObject *)NULL,
 /* 0xD9 */ (DeeTypeObject *)NULL,
 /* 0xDA */ (DeeTypeObject *)NULL,
 /* 0xDB */ (DeeTypeObject *)NULL,
 /* 0xDC */ (DeeTypeObject *)NULL,
 /* 0xDD */ (DeeTypeObject *)NULL,
 /* 0xDE */ (DeeTypeObject *)NULL,
 /* 0xDF */ (DeeTypeObject *)NULL,
 /* 0xE0 */ (DeeTypeObject *)NULL,
 /* 0xE1 */ (DeeTypeObject *)NULL,
 /* 0xE2 */ (DeeTypeObject *)NULL,
 /* 0xE3 */ (DeeTypeObject *)NULL,
 /* 0xE4 */ (DeeTypeObject *)NULL,
 /* 0xE5 */ (DeeTypeObject *)NULL,
 /* 0xE6 */ (DeeTypeObject *)NULL,
 /* 0xE7 */ (DeeTypeObject *)NULL,
 /* 0xE8 */ (DeeTypeObject *)NULL,
 /* 0xE9 */ (DeeTypeObject *)NULL,
 /* 0xEA */ (DeeTypeObject *)NULL,
 /* 0xEB */ (DeeTypeObject *)NULL,
 /* 0xEC */ (DeeTypeObject *)NULL,
 /* 0xED */ (DeeTypeObject *)NULL,
 /* 0xEE */ (DeeTypeObject *)NULL,
 /* 0xEF */ (DeeTypeObject *)NULL,
 /* 0xF0 */ (DeeTypeObject *)NULL,
 /* 0xF1 */ (DeeTypeObject *)NULL,
 /* 0xF2 */ (DeeTypeObject *)NULL,
 /* 0xF3 */ (DeeTypeObject *)NULL,
 /* 0xF4 */ (DeeTypeObject *)NULL,
 /* 0xF5 */ (DeeTypeObject *)NULL,
 /* 0xF6 */ (DeeTypeObject *)NULL,
 /* 0xF7 */ (DeeTypeObject *)NULL,
 /* 0xF8 */ (DeeTypeObject *)NULL,
 /* 0xF9 */ (DeeTypeObject *)NULL,
 /* 0xFA */ (DeeTypeObject *)NULL,
 /* 0xFB */ (DeeTypeObject *)NULL,
 /* 0xFC */ (DeeTypeObject *)NULL,
 /* 0xFD */ (DeeTypeObject *)NULL,
 /* 0xFE */ (DeeTypeObject *)NULL,
 /* 0xFF */ (DeeTypeObject *)NULL,
//[[[end]]]
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_MARSHAL_DATA_C */
