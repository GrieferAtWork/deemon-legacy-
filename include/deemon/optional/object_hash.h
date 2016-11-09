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
#ifndef GUARD_DEEMON_OPTIONAL_OBJECT_HASH_H
#define GUARD_DEEMON_OPTIONAL_OBJECT_HASH_H 1

#include <deemon/__conf.inl>
#include <deemon/__ctypes.inl>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_HASH_T
DEE_PRIVATE_DECL_DEE_HASH_T
#undef DEE_PRIVATE_DECL_DEE_HASH_T
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif



//////////////////////////////////////////////////////////////////////////
// Generic object hashing interface
#define /* DEE_A_EXEC int */DeeObject_HashEx(ob,start,result) \
 DeeObject_THashEx(Dee_TYPE(ob),ob,start,result)
#define DeeObject_HashInplace(ob,hash) DeeObject_HashEx(ob,hash,&(hash))
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_THashEx(
DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
DEE_A_IN Dee_hash_t start, DEE_A_OUT Dee_hash_t *result) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PHash(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN Dee_hash_t start,
 DEE_A_OUT Dee_hash_t *result) DEE_ATTRIBUTE_NONNULL((1,2,4));

//////////////////////////////////////////////////////////////////////////
// Hash functions
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_hash_t) DeeHash_Memory(
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s,
 DEE_A_IN Dee_hash_t start) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_hash_t) DeeHash_String(
 DEE_A_IN_Z char const *p, DEE_A_IN Dee_hash_t start) DEE_ATTRIBUTE_NONNULL((1));
#define DeeHash_StringWithLength(len,s,start) DeeHash_Memory(s,len,start)


#if DEE_CONFIG_SIZEOF_DEE_HASH_T == 4
#define DeeHash_UInt64(start,value)\
 ((start)^(Dee_hash_t)( (Dee_uint64_t)(value)&DEE_UINT64_C(0x00000000FFFFFFFF))\
         ^(Dee_hash_t)(((Dee_uint64_t)(value)&DEE_UINT64_C(0xFFFFFFFF00000000)) >> 32))
#elif DEE_CONFIG_SIZEOF_DEE_HASH_T == 2
#define DeeHash_UInt64(start,value)\
 ((start)^(Dee_hash_t)( (Dee_uint64_t)(value)&DEE_UINT64_C(0x000000000000FFFF))\
         ^(Dee_hash_t)(((Dee_uint64_t)(value)&DEE_UINT64_C(0x00000000FFFF0000)) >> 16)\
         ^(Dee_hash_t)(((Dee_uint64_t)(value)&DEE_UINT64_C(0x0000FFFF00000000)) >> 32)\
         ^(Dee_hash_t)(((Dee_uint64_t)(value)&DEE_UINT64_C(0xFFFF000000000000)) >> 48))
#define DeeHash_UInt32(start,value)\
 ((start)^(Dee_hash_t)( (Dee_uint32_t)(value)&DEE_UINT32_C(0x0000FFFF))\
         ^(Dee_hash_t)(((Dee_uint32_t)(value)&DEE_UINT32_C(0xFFFF0000)) >> 16))
#elif DEE_CONFIG_SIZEOF_DEE_HASH_T == 1
#define DeeHash_UInt64(start,value)\
 ((start)^(Dee_hash_t)( (Dee_uint64_t)(value)&DEE_UINT64_C(0x00000000000000FF))\
         ^(Dee_hash_t)(((Dee_uint64_t)(value)&DEE_UINT64_C(0x000000000000FF00)) >> 8)\
         ^(Dee_hash_t)(((Dee_uint64_t)(value)&DEE_UINT64_C(0x0000000000FF0000)) >> 16)\
         ^(Dee_hash_t)(((Dee_uint64_t)(value)&DEE_UINT64_C(0x00000000FF000000)) >> 24)\
         ^(Dee_hash_t)(((Dee_uint64_t)(value)&DEE_UINT64_C(0x000000FF00000000)) >> 32)\
         ^(Dee_hash_t)(((Dee_uint64_t)(value)&DEE_UINT64_C(0x0000FF0000000000)) >> 40)\
         ^(Dee_hash_t)(((Dee_uint64_t)(value)&DEE_UINT64_C(0x00FF000000000000)) >> 48)\
         ^(Dee_hash_t)(((Dee_uint64_t)(value)&DEE_UINT64_C(0xFF00000000000000)) >> 56))
#define DeeHash_UInt32(start,value)\
 ((start)^(Dee_hash_t)( (Dee_uint32_t)(value)&DEE_UINT32_C(0x000000FF))\
         ^(Dee_hash_t)(((Dee_uint32_t)(value)&DEE_UINT32_C(0x0000FF00)) >> 8)\
         ^(Dee_hash_t)(((Dee_uint32_t)(value)&DEE_UINT32_C(0x00FF0000)) >> 16)\
         ^(Dee_hash_t)(((Dee_uint32_t)(value)&DEE_UINT32_C(0xFF000000)) >> 24))
#define DeeHash_UInt16(start,value)\
 ((start)^(Dee_hash_t)( (Dee_uint16_t)(value)&DEE_UINT16_C(0x00FF))\
         ^(Dee_hash_t)(((Dee_uint16_t)(value)&DEE_UINT16_C(0xFF00)) >> 8))
#endif

#ifndef DeeHash_UInt8
#define DeeHash_UInt8(start,value)  ((start)^(Dee_hash_t)(value))
#endif
#ifndef DeeHash_UInt16
#define DeeHash_UInt16(start,value) ((start)^(Dee_hash_t)(value))
#endif
#ifndef DeeHash_UInt32
#define DeeHash_UInt32(start,value) ((start)^(Dee_hash_t)(value))
#endif
#ifndef DeeHash_UInt64
#define DeeHash_UInt64(start,value) ((start)^(Dee_hash_t)(value))
#endif

//////////////////////////////////////////////////////////////////////////
// >> Dee_hash_t (*DeeHash_Integer_F(typename T))(Dee_hash_t start, T value);
// >> Dee_hash_t DeeHash_Integer(typename T, Dee_hash_t start, T value);
// NOTE: Unrelated to typing, any two equal integral values will generate
//       the same hash. (Assuming the given @value fits in both types,
//       which should already be implied as the values must match).
// WARNING: The @value argument may be evaluated more than once.
// WARNING: The return value may differ from what would be calculated by 'DeeHash_Memory'.
//          But that is OK, as deemon doesn't enforce same-data, same-hash, but simply
//          requires that a stable object must only generate the same hash as another
//          object when comparing equal to that other object. A rule, that is not
//          enforced, but when broken can lead to map-style containers containing
//          unaddressable objects.
#define DeeHash_Integer_F(T)           DEE_PP_CAT_2(DeeHash_UInt,DEE_PP_MUL8(DEE_TYPEDEF_SIZEOF(T)))
#define DeeHash_Integer(T,start,value) DeeHash_Integer_F(T)(start,value)


#ifdef DEE_LIMITED_API
// Make sure hashing across types generates the same results
DEE_STATIC_ASSERT(DeeHash_Integer(Dee_uint8_t, 0,0x42) == DeeHash_Integer(char,0,'\x42'));
DEE_STATIC_ASSERT(DeeHash_Integer(Dee_uint16_t,0,0x42) == DeeHash_Integer(char,0,'\x42'));
DEE_STATIC_ASSERT(DeeHash_Integer(Dee_uint32_t,0,0x42) == DeeHash_Integer(char,0,'\x42'));
DEE_STATIC_ASSERT(DeeHash_Integer(Dee_uint64_t,0,0x42) == DeeHash_Integer(char,0,'\x42'));
#endif


DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_OBJECT_HASH_H */
