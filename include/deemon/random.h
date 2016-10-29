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
#ifndef GUARD_DEEMON_RANDOM_H
#define GUARD_DEEMON_RANDOM_H 1

#include <deemon/__conf.inl>

#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#endif

//////////////////////////////////////////////////////////////////////////
//
// WARNING:
//    -- The random API will be moved into a dex sooner or later --
//   This header will remain, and it will still be possible to use the API,
//   but additional code may be required to load the API before use,
//   as well as binary compatibility with any function/object declared
//   by this header being broken.
//
//////////////////////////////////////////////////////////////////////////


DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif


DEE_OBJECT_DEF(DeeRandomObject);

//////////////////////////////////////////////////////////////////////////
// Pseudo random number generator object
//////////////////////////////////////////////////////////////////////////

struct DeeRandomNumberGenerator { Dee_uint64_t rng_seed; };
#define DeeRandomNumberGenerator_SEED(ob) (ob)->rng_seed

// Initialize with a random seed (based on system time, while still applying a twist)
DEE_FUNC_DECL(void) DeeRandomNumberGenerator_Randomize(DEE_A_OUT struct DeeRandomNumberGenerator *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_int8_t) DeeRandomNumberGenerator_NextInt8(DEE_A_INOUT struct DeeRandomNumberGenerator *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_int16_t) DeeRandomNumberGenerator_NextInt16(DEE_A_INOUT struct DeeRandomNumberGenerator *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_int32_t) DeeRandomNumberGenerator_NextInt32(DEE_A_INOUT struct DeeRandomNumberGenerator *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_int64_t) DeeRandomNumberGenerator_NextInt64(DEE_A_INOUT struct DeeRandomNumberGenerator *self) DEE_ATTRIBUTE_NONNULL((1));
#define DeeRandomNumberGenerator_NextUInt8  (Dee_uint8_t)DeeRandomNumberGenerator_NextInt8
#define DeeRandomNumberGenerator_NextUInt16 (Dee_uint16_t)DeeRandomNumberGenerator_NextInt16
#define DeeRandomNumberGenerator_NextUInt32 (Dee_uint32_t)DeeRandomNumberGenerator_NextInt32
#define DeeRandomNumberGenerator_NextUInt64 (Dee_uint64_t)DeeRandomNumberGenerator_NextInt64
DEE_FUNC_DECL(DEE_A_RET_WUNUSED float) DeeRandomNumberGenerator_NextFloat(DEE_A_INOUT struct DeeRandomNumberGenerator *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED double) DeeRandomNumberGenerator_NextDouble(DEE_A_INOUT struct DeeRandomNumberGenerator *self) DEE_ATTRIBUTE_NONNULL((1));
#if DEE_COMPILER_HAVE_LDOUBLE
DEE_FUNC_DECL(DEE_A_RET_WUNUSED long double) DeeRandomNumberGenerator_NextLDouble(DEE_A_INOUT struct DeeRandomNumberGenerator *self) DEE_ATTRIBUTE_NONNULL((1));
#endif
DEE_FUNC_DECL(void) DeeRandomNumberGenerator_NextData(
 DEE_A_INOUT struct DeeRandomNumberGenerator *self,
 DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s) DEE_ATTRIBUTE_NONNULL((1));

// Generate a randomized instance of 'tp', where 'tp' must be structured type
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeRandomNumberGenerator_Next(
 DEE_A_INOUT struct DeeRandomNumberGenerator *self,
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp) DEE_ATTRIBUTE_NONNULL((1,2));

#ifdef DEE_LIMITED_API
struct DeeRandomObject {
 DEE_OBJECT_HEAD
 struct DeeRandomNumberGenerator r_rng; /*< Random number generator. */
};
#define DeeRandom_RNG(ob) (&((DeeRandomObject *)Dee_REQUIRES_POINTER(ob))->r_rng)
#else
#define DeeRandom_RNG(ob) DeeRandom_Rng((DeeObject *)Dee_REQUIRES_POINTER(ob))
#endif
#define DeeRandom_SEED(ob) DeeRandomNumberGenerator_SEED(DeeRandom_RNG(ob))

DEE_DATA_DECL(DeeTypeObject) DeeRandom_Type;
#define                DeeRandom_Check(ob)      DeeObject_InstanceOf(ob,&DeeRandom_Type)
#define                DeeRandom_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeRandom_Type)
#define /*DEE_A_EXEC*/ DeeRandom_New()          DeeType_NewInstanceDefault(&DeeRandom_Type)

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeRandomObject) *) DeeRandom_NewWithSeed(DEE_A_IN Dee_uint64_t seed);
DEE_FUNC_DECL(DEE_A_RET_NEVER_NULL struct DeeRandomNumberGenerator *) DeeRandom_Rng(
 DEE_A_IN_OBJECT(DeeRandomObject) *self) DEE_ATTRIBUTE_NONNULL((1));
#define DeeRandom_NextInt8(ob)   DeeRandomNumberGenerator_NextInt8(DeeRandom_RNG(ob))
#define DeeRandom_NextInt16(ob)  DeeRandomNumberGenerator_NextInt16(DeeRandom_RNG(ob))
#define DeeRandom_NextInt32(ob)  DeeRandomNumberGenerator_NextInt32(DeeRandom_RNG(ob))
#define DeeRandom_NextInt64(ob)  DeeRandomNumberGenerator_NextInt64(DeeRandom_RNG(ob))
#define DeeRandom_NextUInt8(ob)  DeeRandomNumberGenerator_NextUInt8(DeeRandom_RNG(ob))
#define DeeRandom_NextUInt16(ob) DeeRandomNumberGenerator_NextUInt16(DeeRandom_RNG(ob))
#define DeeRandom_NextUInt32(ob) DeeRandomNumberGenerator_NextUInt32(DeeRandom_RNG(ob))
#define DeeRandom_NextUInt64(ob) DeeRandomNumberGenerator_NextUInt64(DeeRandom_RNG(ob))
#define DeeRandom_NextFloat(ob)  DeeRandomNumberGenerator_NextFloat(DeeRandom_RNG(ob))
#define DeeRandom_NextDouble(ob) DeeRandomNumberGenerator_NextDouble(DeeRandom_RNG(ob))
#if DEE_COMPILER_HAVE_LDOUBLE
#define DeeRandom_NextLDouble(ob) DeeRandomNumberGenerator_NextLDouble(DeeRandom_RNG(ob))
#endif
#define DeeRandom_NextData(ob,p,s) DeeRandomNumberGenerator_NextData(DeeRandom_RNG(ob),p,s)



//////////////////////////////////////////////////////////////////////////
// Returns a thread-local random number generator
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeRandomObject) *) DeeRandom_ThreadLocal(void);

//////////////////////////////////////////////////////////////////////////
// Generate pseudo-entropy using a system-specific RNG generator
// - If the system doesn't provide a randomization implementation, use our own
// - Returns -1 if an error occurred
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeRandom_SystemURandom(
 DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s);

DEE_DECL_END

#endif /* !GUARD_DEEMON_RANDOM_H */
