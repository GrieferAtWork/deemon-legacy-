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
#ifndef GUARD_DEEMON_DEX_HASHLIB_H
#define GUARD_DEEMON_DEX_HASHLIB_H 1
 
#include <deemon/__conf.inl>
#include <deemon/optional/object_decl.h>

DEE_DECL_BEGIN

DEE_OBJECT_DEF(DeeCRCAlgorithmIteratorObject);
DEE_OBJECT_DEF(DeeCRCAlgorithmObject);
DEE_OBJECT_DEF(DeeCRCHasherObject);

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif

struct DeeCRCAlgorithm;
typedef Dee_uint8_t  (DEE_CALL *DeeCRCHashFunc8 )(DEE_A_IN struct DeeCRCAlgorithm const *self, DEE_A_IN Dee_uint8_t  start, DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s);
typedef Dee_uint16_t (DEE_CALL *DeeCRCHashFunc16)(DEE_A_IN struct DeeCRCAlgorithm const *self, DEE_A_IN Dee_uint16_t start, DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s);
typedef Dee_uint32_t (DEE_CALL *DeeCRCHashFunc32)(DEE_A_IN struct DeeCRCAlgorithm const *self, DEE_A_IN Dee_uint32_t start, DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s);
typedef Dee_uint64_t (DEE_CALL *DeeCRCHashFunc64)(DEE_A_IN struct DeeCRCAlgorithm const *self, DEE_A_IN Dee_uint64_t start, DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s);

struct DeeCRCAlgorithm {
 char const        *crc_name;  /*< [1..1] Name of the algorithm. */
 char const *const *crc_alias; /*< [1..1][0..1] NULL-terminated list of alias names. */
 Dee_uint8_t        crc_width; /*< Crc bit-width (<= crc_size*8). */
#define DeeCRCAlgorithm_Size(ob) (ob)->crc_size
 Dee_uint8_t        crc_size;  /*< Size of a hash value in bytes (1|2|4|8). */
#define DEE_CRCALGORITHM_FLAG_NONE     DEE_UINT16_C(0x0000)
#define DEE_CRCALGORITHM_FLAG_REVIN    DEE_UINT16_C(0x0001) /*< The input polynom is reversed. */
#define DEE_CRCALGORITHM_FLAG_REVOUT   DEE_UINT16_C(0x0002) /*< The output polynom is reversed. */
 Dee_uint16_t       crc_flags; /*< Additional crc flags. */
 union{ /*< [1..1] */
  // Internal hash algorithms (these don't perform input/output modulation)
  DeeCRCHashFunc8  crc_hash8;
  DeeCRCHashFunc16 crc_hash16;
  DeeCRCHashFunc32 crc_hash32;
  DeeCRCHashFunc64 crc_hash64;
 };
};



//////////////////////////////////////////////////////////////////////////
// Returns the crc algorithm associated with a given name
extern DEE_A_RET_NOEXCEPT(NULL) struct DeeCRCAlgorithm const *
 DeeCRCAlgorithm_Open(DEE_A_IN_Z char const *name);
extern DEE_A_RET_EXCEPT(NULL) struct DeeCRCAlgorithm const *
 DeeCRCAlgorithm_OpenWithError(DEE_A_IN_Z char const *name);

//////////////////////////////////////////////////////////////////////////
// Enumerate all known crc algorithm algorithms
#define DEE_CRCALGORITHM_ENUM_FLAG_NONE  DEE_UINT32_C(0x00000000)
#define DEE_CRCALGORITHM_ENUM_FLAG_ALIAS DEE_UINT32_C(0x00000001) /*< Enumerating/Should be enumerating alias names. */
typedef DEE_A_RET_EXCEPT(-1) int (DEE_CALL *DeeEnumCrcAlgorithmsFunc)(
 DEE_A_IN struct DeeCRCAlgorithm const *algo, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN_Z char const *name, void *closure);
extern DEE_A_RET_EXCEPT(-1) int DeeCRCAlgorithm_Enum(
 DEE_A_IN Dee_uint32_t flags, DEE_A_IN DeeEnumCrcAlgorithmsFunc func, void *closure);

#define DEE_CRCHASHER_SIZE_8  1
#define DEE_CRCHASHER_SIZE_16 2
#define DEE_CRCHASHER_SIZE_32 4
#define DEE_CRCHASHER_SIZE_64 8


struct DeeCRCHasher {
 struct DeeCRCAlgorithm const *crc_algo; /*< [1..1] Algorithm used for hashing. */
 union{ /*< Intermediate crc result. */
  Dee_uint8_t  crc_result8;
  Dee_uint16_t crc_result16;
  Dee_uint32_t crc_result32;
  Dee_uint64_t crc_result64;
 };
};

#define DeeCRCHasher_InitWithAlgorithm(ob,algo)\
do{\
 DEE_ASSERT(ob); DEE_ASSERT(algo);\
 (ob)->crc_algo = (algo);\
 switch (DeeCRCAlgorithm_Size(algo)) {\
  case DEE_CRCHASHER_SIZE_8:  (ob)->crc_result8  = ((Dee_uint8_t  *)((algo)+1))[0]; break;\
  case DEE_CRCHASHER_SIZE_16: (ob)->crc_result16 = ((Dee_uint16_t *)((algo)+1))[0]; break;\
  case DEE_CRCHASHER_SIZE_32: (ob)->crc_result32 = ((Dee_uint32_t *)((algo)+1))[0]; break;\
  case DEE_CRCHASHER_SIZE_64: (ob)->crc_result64 = ((Dee_uint64_t *)((algo)+1))[0]; break;\
  default: DEE_BUILTIN_UNREACHABLE();\
 }\
}while(0)

extern DEE_A_RET_NOEXCEPT(-1) int DeeCRCHasher_InitWithName(
 DEE_A_OUT struct DeeCRCHasher *self, DEE_A_IN_Z char const *name);
extern DEE_A_RET_EXCEPT(-1) int DeeCRCHasher_InitWithNameAndError(
 DEE_A_OUT struct DeeCRCHasher *self, DEE_A_IN_Z char const *name);
extern void DeeCRCHasher_AddData(DEE_A_INOUT struct DeeCRCHasher *self, DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s);
extern void DeeCRCHasher_AddString(DEE_A_INOUT struct DeeCRCHasher *self, DEE_A_IN_Z char const *s);
#define DeeCRCHasher_Size(ob)     DeeCRCAlgorithm_Size((ob)->crc_algo)
#if 1
#define DeeCRCHasher_Result8(ob)  ((Dee_uint8_t )((ob)->crc_result8  ^ ((Dee_uint8_t  *)(((ob)->crc_algo)+1))[1]))
#define DeeCRCHasher_Result16(ob) ((Dee_uint16_t)((ob)->crc_result16 ^ ((Dee_uint16_t *)(((ob)->crc_algo)+1))[1]))
#define DeeCRCHasher_Result32(ob) ((Dee_uint32_t)((ob)->crc_result32 ^ ((Dee_uint32_t *)(((ob)->crc_algo)+1))[1]))
#define DeeCRCHasher_Result64(ob) ((Dee_uint64_t)((ob)->crc_result64 ^ ((Dee_uint64_t *)(((ob)->crc_algo)+1))[1]))
#else
#define DeeCRCHasher_Result8(ob)  ((Dee_uint8_t )(((ob)->crc_result8  ^ ((Dee_uint8_t  *)(((ob)->crc_algo)+1))[1]) & ((Dee_uint8_t  *)(((ob)->crc_algo)+1))[2]))
#define DeeCRCHasher_Result16(ob) ((Dee_uint16_t)(((ob)->crc_result16 ^ ((Dee_uint16_t *)(((ob)->crc_algo)+1))[1]) & ((Dee_uint16_t *)(((ob)->crc_algo)+1))[2]))
#define DeeCRCHasher_Result32(ob) ((Dee_uint32_t)(((ob)->crc_result32 ^ ((Dee_uint32_t *)(((ob)->crc_algo)+1))[1]) & ((Dee_uint32_t *)(((ob)->crc_algo)+1))[2]))
#define DeeCRCHasher_Result64(ob) ((Dee_uint64_t)(((ob)->crc_result64 ^ ((Dee_uint64_t *)(((ob)->crc_algo)+1))[1]) & ((Dee_uint64_t *)(((ob)->crc_algo)+1))[2]))
#endif



//////////////////////////////////////////////////////////////////////////
// Deemon wrapper objects
struct DeeCRCAlgorithmObject {
 DEE_OBJECT_HEAD
 struct DeeCRCAlgorithm const *ca_algo; /*< [1..1] CRC Algorithm. */
};
#define DeeCRCAlgorithm_Algo(ob) ((DeeCRCAlgorithmObject *)Dee_REQUIRES_POINTER(ob))->ca_algo

extern DeeTypeObject DeeCRCAlgorithm_Type;
#define DeeCRCAlgorithm_Check(ob)              DeeObject_InstanceOf(ob,&DeeCRCAlgorithm_Type)
#define DeeCRCAlgorithm_CheckExact(ob)         DeeObject_InstanceOfExact(ob,&DeeCRCAlgorithm_Type)

struct DeeCRCAlgorithmIteratorObject {
 DEE_OBJECT_HEAD
 /*atomic*/struct DeeCRCAlgorithm const *const *cai_pos; /*< [1..1] Current iterator position. */
           struct DeeCRCAlgorithm const *const *cai_end; /*< [1..1][const] Iterator end position. */
};
extern DeeTypeObject DeeCRCAlgorithmIterator_Type;
#define DeeCRCAlgorithmIterator_Check(ob)      DeeObject_InstanceOf(ob,&DeeCRCAlgorithmIterator_Type)
#define DeeCRCAlgorithmIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeCRCAlgorithmIterator_Type)


struct DeeCRCHasherObject {
 DEE_OBJECT_HEAD
 struct DeeCRCHasher ch_hasher;
};
extern DeeTypeObject DeeCRCHasher_Type;
#define DeeCRCHasher_Check(ob)         DeeObject_InstanceOf(ob,&DeeCRCHasher_Type)
#define DeeCRCHasher_CheckExact(ob)    DeeObject_InstanceOfExact(ob,&DeeCRCHasher_Type)




DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_HASHLIB_H */
