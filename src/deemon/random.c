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
#ifndef GUARD_DEEMON_RANDOM_C
#define GUARD_DEEMON_RANDOM_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/bool.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/float.h>
#include <deemon/integer.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/mp/thread.h>
#include <deemon/none.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/std/string.h>
#include <deemon/random.h>
#include <deemon/runtime/builtins.h>
#include <deemon/string.h>
#include <deemon/structured.h>
#include <deemon/time.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>
#include <deemon/posix_features.inl>

#ifdef DEE_PLATFORM_WINDOWS
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif

#ifndef DEE_PLATFORM_WINDOWS
#if DEE_HAVE_GETRANDOM
#include <linux/random.h>
#elif DEE_HAVE_GETENTROPY
#include <unistd.h>
#else
#include <deemon/file.h>
#endif
#endif

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeRandom_SystemURandom(
 DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s) {
#ifdef DEE_PLATFORM_WINDOWS // Win32 crypo RNG
#define DEE_HAVE_NATIVE_URANDOM 1
 HCRYPTPROV hCryptProv;
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (!CryptAcquireContext(&hCryptProv,NULL,NULL,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "CryptAcquireContext() : %K",
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
#ifdef DEE_PLATFORM_64_BIT
 while (s) {
  DWORD chunk = (DWORD)(s > 0xFFFFFFFF ? 0xFFFFFFFF : s);
  if (DeeThread_CheckInterrupt() != 0) goto err_ctx;
  if (!CryptGenRandom(hCryptProv,(DWORD)chunk,(BYTE *)p))
#else
  if (DeeThread_CheckInterrupt() != 0) goto err_ctx;
  if (!CryptGenRandom(hCryptProv,(DWORD)s,(BYTE *)p))
#endif
  {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "CryptGenRandom(%#p,%I32u,%#p) : %K",
                       hCryptProv,(DWORD)s,p,
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
err_ctx:
   CryptReleaseContext(hCryptProv,0);
   return -1;
  }
#ifdef DEE_PLATFORM_64_BIT
  *(char **)&p += chunk;
  s -= chunk;
 }
#endif
 CryptReleaseContext(hCryptProv,0);
 return 0;
#elif DEE_HAVE_GETRANDOM
#define DEE_HAVE_NATIVE_URANDOM 1
 if (DeeThread_CheckInterrupt() != 0) return -1;
 if (getrandom(p,s,0) != 0) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "getrandom(%#p,%Iu) : %K",p,s,
                      DeeSystemError_ToString(DeeSystemError_Consume()));
  return -1;
 }
 return 0;
#elif DEE_HAVE_GETENTROPY
#define DEE_HAVE_NATIVE_URANDOM 1
 Dee_size_t len;
 while (s) {
  len = s > 256 ? 256 : s;
  if (DeeThread_CheckInterrupt() != 0) return -1;
  if (getentropy(p,len) != 0) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "getentropy(%#p,%d) : %K",p,len,
                       DeeSystemError_ToString(DeeSystemError_Consume()));
   return -1;
  }
  buffer += len;
  size -= len;
 }
 return 0;
#elif 1 // Posix /dev/urandom RNG
#define DEE_HAVE_NATIVE_URANDOM 1
 DeeObject *urandom; int result;
 if ((urandom = DeeFileIO_New("/dev/urandom","r")) == NULL) return -1;
 result = DeeFile_ReadAll(urandom,p,s);
 Dee_DECREF(urandom);
 return result;
#else
 // No OS-support (use our own randomization)
 struct DeeRandomNumberGenerator rng;
 DeeRandomNumberGenerator_Randomize(&rng);
 DeeRandomNumberGenerator_NextData(&rng,p,s);
 return 0;
#endif
}


DEE_STATIC_INLINE(void)
_dee_randomize2(Dee_uint64_t *p) {
 Dee_uint8_t temp;
#define swap_slots(a,b) (temp=((Dee_uint8_t*)p)[a],((Dee_uint8_t*)p)[a]=((Dee_uint8_t*)p)[b],((Dee_uint8_t*)p)[b]=temp)
 // Put a little bit of a twist on random seeds
 // Note how all of these cases are fallthrough...
 switch (*p%8) {
  case 7: swap_slots(1,3); DEE_ATTRIBUTE_FALLTHROUGH
  case 6: swap_slots(5,2); DEE_ATTRIBUTE_FALLTHROUGH
  case 5: swap_slots(7,4); DEE_ATTRIBUTE_FALLTHROUGH
  case 4: swap_slots(0,3); DEE_ATTRIBUTE_FALLTHROUGH
  case 3: swap_slots(5,0); DEE_ATTRIBUTE_FALLTHROUGH
  case 2: swap_slots(3,5); DEE_ATTRIBUTE_FALLTHROUGH
  case 1: swap_slots(6,7); DEE_ATTRIBUTE_FALLTHROUGH
  case 0: swap_slots(2,0); DEE_ATTRIBUTE_FALLTHROUGH
  default: break;
 }
#undef swap_slots
 // Even more randomization:
 // >> Use the addresses of builtin objects (and possibly they types) to further randomize stuff
 {DeeObject *builtin_rng = DeeBuiltin_Get(*p % DeeBuiltin_Count);
  if (builtin_rng) { // Should always be the case, but better be safe
   ((Dee_uintptr_t *)p)[0] ^= (Dee_uintptr_t)builtin_rng;
#if DEE_TYPES_SIZEOF_POINTER <= 4
   // Use the object type to randomize the high dword
   // >> Not quite as random, but still better than not touching it
   ((Dee_uintptr_t *)p)[1] ^= (Dee_uintptr_t)Dee_TYPE(builtin_rng);
#endif
  }
 }
 // TODO: Maybe throw in even more entropy
 //       with other system-info based stuff?
}

// Initialize with a random seed (based on system time)
DEE_COMPILER_MSVC_WARNING_PUSH(4229)
DEE_COMPILER_PREFAST_WARNING_PUSH(6001 6102)
void DeeRandomNumberGenerator_Randomize(
 DEE_A_OUT struct DeeRandomNumberGenerator *self) {
 Dee_uint64_t new_seed;
 // Try to reverse-engineer this! (Hue hue hue...)
 if (DeeRandom_SystemURandom(&new_seed,sizeof(new_seed)) != 0)
  DeeError_Handled(); // Ignore errors & if they occurred, they're part of the entropy
 // Note how we even take advantage of possible dangling data... (twice if urandom failed) ;)
 self->rng_seed ^= new_seed;
 if ((self->rng_seed & 0x1) != 0)
  self->rng_seed ^= ~DeeTime_GetClockTick();
 // Improve the randomization a bit
 _dee_randomize2(&self->rng_seed);
#if DEE_TYPES_SIZEOF_POINTER <= 8
 // Lastly, add the address of the rng structure
 // NOTE: This only improves entropy if the rng isn't statically allocated
 if ((self->rng_seed & 0x3) != 0)
  *((Dee_uintptr_t *)&self->rng_seed) ^= (Dee_uintptr_t)self;
#endif
}
DEE_COMPILER_PREFAST_WARNING_POP
DEE_COMPILER_MSVC_WARNING_POP

// NOTE: The '>> 1' prevent the generator for freezing and guaranties positive 64-bit values
#define next_seed() \
 (self->rng_seed ^= ((DEE_UINT64_C(214013)*self->rng_seed+DEE_UINT64_C(2531011)))>>1)

DEE_A_RET_WUNUSED Dee_int8_t DeeRandomNumberGenerator_NextInt8(
 DEE_A_INOUT struct DeeRandomNumberGenerator *self) {
 return (Dee_int8_t)next_seed();
}
DEE_A_RET_WUNUSED Dee_int16_t DeeRandomNumberGenerator_NextInt16(
 DEE_A_INOUT struct DeeRandomNumberGenerator *self) {
 return (Dee_int16_t)next_seed();
}
DEE_A_RET_WUNUSED Dee_int32_t DeeRandomNumberGenerator_NextInt32(
 DEE_A_INOUT struct DeeRandomNumberGenerator *self) {
 return (Dee_int32_t)next_seed();
}
DEE_A_RET_WUNUSED Dee_int64_t DeeRandomNumberGenerator_NextInt64(
 DEE_A_INOUT struct DeeRandomNumberGenerator *self) {
 return (Dee_int64_t)next_seed();
}

DEE_A_RET_WUNUSED float DeeRandomNumberGenerator_NextFloat(
 DEE_A_INOUT struct DeeRandomNumberGenerator *self) {
 return ((float)((Dee_uint32_t)next_seed()))*((1.0f/0x10000)/0x10000)/* 2^-32 */;
}
DEE_A_RET_WUNUSED double DeeRandomNumberGenerator_NextDouble(
 DEE_A_INOUT struct DeeRandomNumberGenerator *self) {
 return ((double)((Dee_uint32_t)next_seed()))*((1.0/0x10000)/0x10000)/* 2^-32 */;
}
#if DEE_COMPILER_HAVE_LDOUBLE
DEE_A_RET_WUNUSED long double DeeRandomNumberGenerator_NextLDouble(
 DEE_A_INOUT struct DeeRandomNumberGenerator *self) {
 return ((long double)((Dee_uint32_t)next_seed()))*((1.0L/0x10000)/0x10000)/* 2^-32 */;
}
#endif

void DeeRandomNumberGenerator_NextData(
 DEE_A_INOUT struct DeeRandomNumberGenerator *self,
 DEE_A_OUT_WB(s) void *p, DEE_A_IN Dee_size_t s) {
 DEE_ASSERT(self);
 DEE_ASSERT(!s || p);
 while (s >= 8) { *(*(Dee_uint64_t **)&p)++ = next_seed(); s -= 8; }
 if (s) { Dee_uint64_t last = next_seed(); memcpy(p,&last,s); }
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeRandomNumberGenerator_Next(
 DEE_A_INOUT struct DeeRandomNumberGenerator *self,
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp) {
 DeeObject *result;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(tp) && DeeStructuredType_Check(tp) &&
            "Not a structured type");
 if ((result = (*DeeType_GET_SLOT(tp,tp_alloc))((DeeTypeObject *)tp)) != NULL) {
  // Fill with random data
  DeeRandomNumberGenerator_NextData(
   self,DeeStructured_DATA(result),
   DeeType_GET_SLOT(tp,tp_p_instance_size));
 }
 return result;
}




DEE_A_RET_OBJECT_EXCEPT_REF(DeeRandomObject) *
DeeRandom_NewWithSeed(DEE_A_IN Dee_uint64_t seed) {
 DeeRandomObject *result;
 if ((result = DeeObject_MALLOC(DeeRandomObject)) != NULL) {
  DeeObject_INIT(result,&DeeRandom_Type);
  result->r_rng.rng_seed = seed;
 }
 return (DeeObject *)result;
}
DEE_A_RET_NEVER_NULL struct DeeRandomNumberGenerator *
DeeRandom_Rng(DEE_A_IN_OBJECT(DeeRandomObject) *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeRandom_Check(self));
 return DeeRandom_RNG(self);
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeRandomObject) *DeeRandom_ThreadLocal(void) {
 DeeObject *result,*old_random; DeeThreadObject *thread_self;
 if ((thread_self = DeeThread_SELF()) == NULL)
  return NULL; // Can't raise errors without thread_self
 DeeAtomicMutex_Acquire(&thread_self->t_rt_lock);
 if ((result = (DeeObject *)thread_self->t_rt_rand) == NULL) {
  // Lazy initialization
  DeeAtomicMutex_Release(&thread_self->t_rt_lock);
  if ((result = DeeRandom_New()) == NULL) return NULL;
  DeeAtomicMutex_Acquire(&thread_self->t_rt_lock);
  if ((old_random = (DeeObject *)thread_self->t_rt_rand) != NULL) {
   Dee_INCREF(old_random);
   DeeAtomicMutex_Release(&thread_self->t_rt_lock);
   Dee_DECREF(result);
   return old_random;
  } else {
   Dee_INCREF(thread_self->t_rt_rand = (DeeRandomObject *)result);
  }
 } else Dee_INCREF(result);
 DeeAtomicMutex_Release(&thread_self->t_rt_lock);
 return result;
}



static int _deerandom_tp_ctor(DeeTypeObject *DEE_UNUSED(tp_self),DeeRandomObject *self) {
 DeeRandomNumberGenerator_Randomize(&self->r_rng);
 return 0;
}
static DeeObject *_deerandom_tp_str(DeeRandomObject *self) {
 return DeeString_Newf("<random: seed %I64u>",DeeRandom_SEED(self));
}
static int _deerandom_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),DeeRandomObject *self, DeeRandomObject *right) {
 self->r_rng = right->r_rng;
 return 0;
}
static int _deerandom_tp_copy_assign(
 DeeRandomObject *self, DeeRandomObject *right) {
 self->r_rng = right->r_rng;
 return 0;
}
static int _deerandom_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),DeeRandomObject *self, DeeObject *args) {
 if (DeeTuple_Unpack(args,"I64u:random",&self->r_rng.rng_seed) != 0) return -1;
 return 0;
}
static int _deerandom_tp_any_assign(DeeRandomObject *self, DeeObject *right) {
 if (DeeObject_Cast(Dee_uint64_t,right,&self->r_rng.rng_seed) != 0) return -1;
 return 0;
}
static DeeObject *_deerandom_tp_call(DeeRandomObject *self, DeeObject *args) {
 DeeTypeObject *ty = DeeObject_TYPE(Dee_rt_int);
 if (DeeTuple_Unpack(args,"|o:__call__",&ty) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&ty,(DeeTypeObject *)&DeeStructuredType_Type) != 0) return NULL;
 return DeeRandomNumberGenerator_Next(&self->r_rng,ty);
}
static DeeObject *_deerandom_tp_cmp_lo(DeeRandomObject *self, DeeRandomObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeRandom_Type) != 0) return NULL; DeeReturn_Bool(DeeRandom_SEED(self) < DeeRandom_SEED(right)); }
static DeeObject *_deerandom_tp_cmp_le(DeeRandomObject *self, DeeRandomObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeRandom_Type) != 0) return NULL; DeeReturn_Bool(DeeRandom_SEED(self) <= DeeRandom_SEED(right)); }
static DeeObject *_deerandom_tp_cmp_eq(DeeRandomObject *self, DeeRandomObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeRandom_Type) != 0) return NULL; DeeReturn_Bool(DeeRandom_SEED(self) == DeeRandom_SEED(right)); }
static DeeObject *_deerandom_tp_cmp_ne(DeeRandomObject *self, DeeRandomObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeRandom_Type) != 0) return NULL; DeeReturn_Bool(DeeRandom_SEED(self) != DeeRandom_SEED(right)); }
static DeeObject *_deerandom_tp_cmp_gr(DeeRandomObject *self, DeeRandomObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeRandom_Type) != 0) return NULL; DeeReturn_Bool(DeeRandom_SEED(self) > DeeRandom_SEED(right)); }
static DeeObject *_deerandom_tp_cmp_ge(DeeRandomObject *self, DeeRandomObject *right) { if (DeeObject_InplaceGetInstance(&right,&DeeRandom_Type) != 0) return NULL; DeeReturn_Bool(DeeRandom_SEED(self) >= DeeRandom_SEED(right)); }
static int _deerandom_tp_hash(DeeRandomObject *self, Dee_hash_t start, Dee_hash_t *result) {
#if DEE_CONFIG_SIZEOF_DEE_HASH_T >= 8
 *result = start ^ (Dee_hash_t)self->r_rng.rng_seed;
#elif DEE_CONFIG_SIZEOF_DEE_HASH_T*2 == 8
 *result = start ^ ((Dee_hash_t *)&self->r_rng.rng_seed)[0] ^
                   ((Dee_hash_t *)&self->r_rng.rng_seed)[1];
#elif DEE_CONFIG_SIZEOF_DEE_HASH_T*4 == 8
 *result = start ^ ((Dee_hash_t *)&self->r_rng.rng_seed)[0] ^
                   ((Dee_hash_t *)&self->r_rng.rng_seed)[1] ^
                   ((Dee_hash_t *)&self->r_rng.rng_seed)[2] ^
                   ((Dee_hash_t *)&self->r_rng.rng_seed)[3];
#else
#error "Invalid/Unsupported hash/integer size"
#endif
 return 0;
}


static DeeObject *_deerandom_next(
 DeeRandomObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeTypeObject *ty = DeeObject_TYPE(Dee_rt_int);
 if (DeeTuple_Unpack(args,"|o:next",&ty) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&ty,(DeeTypeObject *)&DeeStructuredType_Type) != 0) return NULL;
 return DeeRandomNumberGenerator_Next(&self->r_rng,ty);
}
static DeeObject *_deerandom_nextflt(
 DeeRandomObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeTypeObject *ty = DeeObject_TYPE(Dee_rt_double);
 if (DeeTuple_Unpack(args,"|o:nextflt",&ty) != 0) return NULL;
#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_FLOAT
 if (ty == (DeeTypeObject *)&DeeDouble_Type) return DeeDouble_New(DeeRandom_NextFloat((DeeObject *)self));
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_DOUBLE
 if (ty == (DeeTypeObject *)&DeeDouble_Type) return DeeDouble_New(DeeRandom_NextDouble((DeeObject *)self));
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_LDOUBLE
 if (ty == (DeeTypeObject *)&DeeDouble_Type) return DeeDouble_New(DeeRandom_NextLDouble((DeeObject *)self));
#endif
#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_FLOAT
 if (ty == (DeeTypeObject *)&DeeFloat_Type) return DeeFloat_New(DeeRandom_NextFloat((DeeObject *)self));
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_DOUBLE
 if (ty == (DeeTypeObject *)&DeeFloat_Type) return DeeFloat_New(DeeRandom_NextDouble((DeeObject *)self));
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_LDOUBLE
 if (ty == (DeeTypeObject *)&DeeFloat_Type) return DeeFloat_New(DeeRandom_NextLDouble((DeeObject *)self));
#endif
#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_FLOAT
 if (ty == (DeeTypeObject *)&DeeLDouble_Type) return DeeLDouble_New(DeeRandom_NextFloat((DeeObject *)self));
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_DOUBLE
 if (ty == (DeeTypeObject *)&DeeLDouble_Type) return DeeLDouble_New(DeeRandom_NextDouble((DeeObject *)self));
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_LDOUBLE
 if (ty == (DeeTypeObject *)&DeeLDouble_Type) return DeeLDouble_New(DeeRandom_NextLDouble((DeeObject *)self));
#endif
#if 1
 DeeError_SetStringf(&DeeErrorType_TypeError,
                     "Expected floating point type (got: '%#q')",
                     DeeType_NAME(ty));
 return NULL;
#else
 if (DeeObject_InplaceGetInstance(&ty,(DeeTypeObject *)&DeeStructuredType_Type) != 0) return NULL;
 return DeeRandomNumberGenerator_Next(&self->r_rng,ty);
#endif
}
static DeeObject *_deerandom_nextdata(
 DeeRandomObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result; Dee_size_t data_size;
 if (DeeTuple_Unpack(args,"Iu:nextdata",&data_size) != 0) return NULL;
 if ((result = DeeString_NewSized(data_size)) == NULL) return NULL;
 DeeRandom_NextData(self,DeeString_STR(result),data_size);
 return result;
}
static DeeObject *_deerandom_randomize(
 DeeRandomObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":randomize") != 0) return NULL;
 DeeRandomNumberGenerator_Randomize(&self->r_rng);
 DeeReturn_None;
}

static DeeObject *_deerandomclass_thread_local(
 DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":thread_local") != 0) return NULL;
 return DeeRandom_ThreadLocal();
}

static struct DeeMemberDef const _deerandom_tp_members[] = {
 DEE_MEMBERDEF_NAMED_v100("seed",DeeRandomObject,r_rng.rng_seed,Dee_uint64_t),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_v100("__r_rng_rng_seed",DeeRandomObject,r_rng.rng_seed,Dee_uint64_t),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};
static struct DeeMethodDef const _deerandom_tp_methods[] = {
 DEE_METHODDEF_v100("next",member(&_deerandom_next),
  "(type object T = int) -> T\n"
  "\tReturns a randomized instance of 'T', where T must be structured type"),
 DEE_METHODDEF_v100("nextflt",member(&_deerandom_nextflt),
  "(type object T = double) -> T\n"
  "\tReturns a randomized instance of 'T', where T must be structured type\n"
  "\tIf 'T' is 'float', 'double' or 'long double' the value is always in the range of 0..1"),
 DEE_METHODDEF_v100("nextdata",member(&_deerandom_nextdata),
  "(size_t data_size) -> string\n"
  "\tReturns a string of random characters with a size of 'data_size'"),
 DEE_METHODDEF_v100("randomize",member(&_deerandom_randomize),
  "() -> none\n"
  "\tGenerates a new random seed (based on the current system time)"),
 DEE_METHODDEF_END_v100
};
static struct DeeMethodDef const _deerandom_tp_class_methods[] = {
 DEE_METHODDEF_v100("thread_local",member(&_deerandomclass_thread_local),
  "() -> random\n"
  "\tReturns the randomizer local to the calling thread"),
 DEE_METHODDEF_END_v100
};


static int DEE_CALL _deerandom_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeRandomObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *DEE_UNUSED(map)) {
 return DeeFile_GetLe(infile,self->r_rng.rng_seed);
}

static int DEE_CALL _deerandom_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeRandomObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *DEE_UNUSED(map)) {
 return DeeFile_PutLe(outfile,self->r_rng.rng_seed);
}

static struct DeeTypeMarshal _deerandom_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(random)),
 member(&_deerandom_tp_marshal_ctor),member(&_deerandom_tp_marshal_put));

DeeTypeObject DeeRandom_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("random"),member(
  "() -> random\n"
  "(uint64_t seed) -> random\n"
  "\tGenerates a new random object.\n"
  "\tIf no need is given, initialize the object with a random seed.\n"),
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeRandomObject),null,
  member(&_deerandom_tp_ctor),
  member(&_deerandom_tp_copy_ctor),null,
  member(&_deerandom_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deerandom_tp_copy_assign),null,
  member(&_deerandom_tp_any_assign)),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deerandom_tp_str),null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(
  member(&_deerandom_tp_call),null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,
  member(&_deerandom_tp_hash)),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deerandom_tp_cmp_lo),member(&_deerandom_tp_cmp_le),
  member(&_deerandom_tp_cmp_eq),member(&_deerandom_tp_cmp_ne),
  member(&_deerandom_tp_cmp_gr),member(&_deerandom_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deerandom_tp_members),null,member(_deerandom_tp_methods),
  null,null,member(_deerandom_tp_class_methods)),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deerandom_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_RANDOM_C */
