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
#ifndef GUARD_DEEMON___ATOMIC_INTRIN_NOTHREAD_INL
#define GUARD_DEEMON___ATOMIC_INTRIN_NOTHREAD_INL 1
#ifndef DEE_ATOMIC_INTRIN_INCLUDE_MAGIC
#error "#include <deemon/__atomic_intrin.inl> instead of this file"
#endif

#include <deemon/__conf.inl>

DEE_DECL_BEGIN

#define DEE_ATOMIC8_NATIVE_T  DEE_TYPES_INT8_T
#define DEE_ATOMIC16_NATIVE_T DEE_TYPES_INT16_T
#define DEE_ATOMIC32_NATIVE_T DEE_TYPES_INT32_T
#define DEE_ATOMIC64_NATIVE_T DEE_TYPES_INT64_T

#define DEE_PRIVATE_MAKE_ATOMIC_FUNCTIONS(n)\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC##n##_NATIVE_T) \
_DeeAtomic##n##_Exchange(DEE_ATOMIC##n##_NATIVE_T *p, DEE_ATOMIC##n##_NATIVE_T v) {\
 DEE_ATOMIC##n##_NATIVE_T result = *p;\
 return *p = v,result;\
}\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) \
_DeeAtomic##n##_CompareExchange(DEE_ATOMIC##n##_NATIVE_T *p, DEE_ATOMIC##n##_NATIVE_T old_v, DEE_ATOMIC##n##_NATIVE_T new_v) {\
 return *p == old_v ? (*p = new_v,1) : 0;\
}\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC##n##_NATIVE_T) \
_DeeAtomic##n##_CompareExchangeVal(DEE_ATOMIC##n##_NATIVE_T *p, DEE_ATOMIC##n##_NATIVE_T old_v, DEE_ATOMIC##n##_NATIVE_T new_v) {\
 return *p == old_v ? (*p = new_v,old_v) : *p;\
}\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC##n##_NATIVE_T) \
_DeeAtomic##n##_FetchAdd(DEE_ATOMIC##n##_NATIVE_T *p, DEE_ATOMIC##n##_NATIVE_T v) {\
 DEE_ATOMIC##n##_NATIVE_T result = *p;\
 return *p += v,result;\
}\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC##n##_NATIVE_T) \
_DeeAtomic##n##_FetchSub(DEE_ATOMIC##n##_NATIVE_T *p, DEE_ATOMIC##n##_NATIVE_T v) {\
 DEE_ATOMIC##n##_NATIVE_T result = *p;\
 return *p -= v,result;\
}\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC##n##_NATIVE_T) \
_DeeAtomic##n##_FetchAnd(DEE_ATOMIC##n##_NATIVE_T *p, DEE_ATOMIC##n##_NATIVE_T v) {\
 DEE_ATOMIC##n##_NATIVE_T result = *p;\
 return *p &= v,result;\
}\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC##n##_NATIVE_T) \
_DeeAtomic##n##_FetchOr(DEE_ATOMIC##n##_NATIVE_T *p, DEE_ATOMIC##n##_NATIVE_T v) {\
 DEE_ATOMIC##n##_NATIVE_T result = *p;\
 return *p |= v,result;\
}\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC##n##_NATIVE_T) \
_DeeAtomic##n##_FetchXor(DEE_ATOMIC##n##_NATIVE_T *p, DEE_ATOMIC##n##_NATIVE_T v) {\
 DEE_ATOMIC##n##_NATIVE_T result = *p;\
 return *p ^= v,result;\
}\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC##n##_NATIVE_T) \
_DeeAtomic##n##_NandFetch(DEE_ATOMIC##n##_NATIVE_T *p, DEE_ATOMIC##n##_NATIVE_T v) {\
 return *p = ~(*p&v);\
}\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC##n##_NATIVE_T) \
_DeeAtomic##n##_FetchNand(DEE_ATOMIC##n##_NATIVE_T *p, DEE_ATOMIC##n##_NATIVE_T v) {\
 DEE_ATOMIC##n##_NATIVE_T result = *p;\
 return *p = ~(result&v),result;\
}
DEE_PRIVATE_MAKE_ATOMIC_FUNCTIONS(8)
DEE_PRIVATE_MAKE_ATOMIC_FUNCTIONS(16)
DEE_PRIVATE_MAKE_ATOMIC_FUNCTIONS(32)
DEE_PRIVATE_MAKE_ATOMIC_FUNCTIONS(64)
#undef DEE_PRIVATE_MAKE_ATOMIC_FUNCTIONS


//////////////////////////////////////////////////////////////////////////
// DeeAtomic8 Native API
#define DeeAtomic8_Store(x,v,order)                                                    (*(Dee_int8_t*)&(x)=(Dee_int8_t)(v))
#define DeeAtomic8_Load(x,order)                                                       (*(Dee_int8_t*)&(x))
#define DeeAtomic8_Exchange(x,v,order)                                                 _DeeAtomic8_Exchange((Dee_int8_t*)&(x),(Dee_int8_t)(v))
#define DeeAtomic8_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      _DeeAtomic8_CompareExchange((Dee_int8_t*)&(x),(Dee_int8_t)(old_v),(Dee_int8_t)(new_v))
#define DeeAtomic8_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    _DeeAtomic8_CompareExchange((Dee_int8_t*)&(x),(Dee_int8_t)(old_v),(Dee_int8_t)(new_v))
#define DeeAtomic8_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   _DeeAtomic8_CompareExchangeVal((Dee_int8_t*)&(x),(Dee_int8_t)(old_v),(Dee_int8_t)(new_v))
#define DeeAtomic8_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) _DeeAtomic8_CompareExchangeVal((Dee_int8_t*)&(x),(Dee_int8_t)(old_v),(Dee_int8_t)(new_v))
#define DeeAtomic8_FetchInc(x,order)                                                   ((*(Dee_int8_t*)&(x))++)
#define DeeAtomic8_IncFetch(x,order)                                                   (++(*(Dee_int8_t*)&(x)))
#define DeeAtomic8_FetchDec(x,order)                                                   ((*(Dee_int8_t*)&(x))--)
#define DeeAtomic8_DecFetch(x,order)                                                   (--(*(Dee_int8_t*)&(x)))
#define DeeAtomic8_FetchAdd(x,v,order)                                                 _DeeAtomic8_FetchAdd((Dee_int8_t*)&(x),(Dee_int8_t)(v))
#define DeeAtomic8_AddFetch(x,v,order)                                                 ((*(Dee_int8_t*)&(x))+=(Dee_int8_t)(v))
#define DeeAtomic8_FetchSub(x,v,order)                                                 _DeeAtomic8_FetchSub((Dee_int8_t*)&(x),(Dee_int8_t)(v))
#define DeeAtomic8_SubFetch(x,v,order)                                                 ((*(Dee_int8_t*)&(x))-=(Dee_int8_t)(v))
#define DeeAtomic8_FetchAnd(x,v,order)                                                 _DeeAtomic8_FetchAnd((Dee_int8_t*)&(x),(Dee_int8_t)(v))
#define DeeAtomic8_AndFetch(x,v,order)                                                 ((*(Dee_int8_t*)&(x))&=(Dee_int8_t)(v))
#define DeeAtomic8_FetchOr(x,v,order)                                                  _DeeAtomic8_FetchOr((Dee_int8_t*)&(x),(Dee_int8_t)(v))
#define DeeAtomic8_OrFetch(x,v,order)                                                  ((*(Dee_int8_t*)&(x))|=(Dee_int8_t)(v))
#define DeeAtomic8_FetchXor(x,v,order)                                                 _DeeAtomic8_FetchXor((Dee_int8_t*)&(x),(Dee_int8_t)(v))
#define DeeAtomic8_XorFetch(x,v,order)                                                 ((*(Dee_int8_t*)&(x))^=(Dee_int8_t)(v))
#define DeeAtomic8_FetchNand(x,v,order)                                                _DeeAtomic8_FetchNand((Dee_int8_t*)&(x),(Dee_int8_t)(v))
#define DeeAtomic8_NandFetch(x,v,order)                                                _DeeAtomic8_NandFetch((Dee_int8_t*)&(x),(Dee_int8_t)(v))


//////////////////////////////////////////////////////////////////////////
// DeeAtomic16 Native API
#define DeeAtomic16_Store(x,v,order)                                                    (*(Dee_int16_t*)&(x)=(Dee_int16_t)(v))
#define DeeAtomic16_Load(x,order)                                                       (*(Dee_int16_t*)&(x))
#define DeeAtomic16_Exchange(x,v,order)                                                 _DeeAtomic16_Exchange((Dee_int16_t*)&(x),(Dee_int16_t)(v))
#define DeeAtomic16_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      _DeeAtomic16_CompareExchange((Dee_int16_t*)&(x),(Dee_int16_t)(old_v),(Dee_int16_t)(new_v))
#define DeeAtomic16_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    _DeeAtomic16_CompareExchange((Dee_int16_t*)&(x),(Dee_int16_t)(old_v),(Dee_int16_t)(new_v))
#define DeeAtomic16_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   _DeeAtomic16_CompareExchangeVal((Dee_int16_t*)&(x),(Dee_int16_t)(old_v),(Dee_int16_t)(new_v))
#define DeeAtomic16_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) _DeeAtomic16_CompareExchangeVal((Dee_int16_t*)&(x),(Dee_int16_t)(old_v),(Dee_int16_t)(new_v))
#define DeeAtomic16_FetchInc(x,order)                                                   ((*(Dee_int16_t*)&(x))++)
#define DeeAtomic16_IncFetch(x,order)                                                   (++(*(Dee_int16_t*)&(x)))
#define DeeAtomic16_FetchDec(x,order)                                                   ((*(Dee_int16_t*)&(x))--)
#define DeeAtomic16_DecFetch(x,order)                                                   (--(*(Dee_int16_t*)&(x)))
#define DeeAtomic16_FetchAdd(x,v,order)                                                 _DeeAtomic16_FetchAdd((Dee_int16_t*)&(x),(Dee_int16_t)(v))
#define DeeAtomic16_AddFetch(x,v,order)                                                 ((*(Dee_int16_t*)&(x))+=(Dee_int16_t)(v))
#define DeeAtomic16_FetchSub(x,v,order)                                                 _DeeAtomic16_FetchSub((Dee_int16_t*)&(x),(Dee_int16_t)(v))
#define DeeAtomic16_SubFetch(x,v,order)                                                 ((*(Dee_int16_t*)&(x))-=(Dee_int16_t)(v))
#define DeeAtomic16_FetchAnd(x,v,order)                                                 _DeeAtomic16_FetchAnd((Dee_int16_t*)&(x),(Dee_int16_t)(v))
#define DeeAtomic16_AndFetch(x,v,order)                                                 ((*(Dee_int16_t*)&(x))&=(Dee_int16_t)(v))
#define DeeAtomic16_FetchOr(x,v,order)                                                  _DeeAtomic16_FetchOr((Dee_int16_t*)&(x),(Dee_int16_t)(v))
#define DeeAtomic16_OrFetch(x,v,order)                                                  ((*(Dee_int16_t*)&(x))|=(Dee_int16_t)(v))
#define DeeAtomic16_FetchXor(x,v,order)                                                 _DeeAtomic16_FetchXor((Dee_int16_t*)&(x),(Dee_int16_t)(v))
#define DeeAtomic16_XorFetch(x,v,order)                                                 ((*(Dee_int16_t*)&(x))^=(Dee_int16_t)(v))
#define DeeAtomic16_FetchNand(x,v,order)                                                _DeeAtomic16_FetchNand((Dee_int16_t*)&(x),(Dee_int16_t)(v))
#define DeeAtomic16_NandFetch(x,v,order)                                                _DeeAtomic16_NandFetch((Dee_int16_t*)&(x),(Dee_int16_t)(v))

//////////////////////////////////////////////////////////////////////////
// DeeAtomic32 Native API
#define DeeAtomic32_Store(x,v,order)                                                    (*(Dee_int32_t*)&(x)=(Dee_int32_t)(v))
#define DeeAtomic32_Load(x,order)                                                       (*(Dee_int32_t*)&(x))
#define DeeAtomic32_Exchange(x,v,order)                                                 _DeeAtomic32_Exchange((Dee_int32_t*)&(x),(Dee_int32_t)(v))
#define DeeAtomic32_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      _DeeAtomic32_CompareExchange((Dee_int32_t*)&(x),(Dee_int32_t)(old_v),(Dee_int32_t)(new_v))
#define DeeAtomic32_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    _DeeAtomic32_CompareExchange((Dee_int32_t*)&(x),(Dee_int32_t)(old_v),(Dee_int32_t)(new_v))
#define DeeAtomic32_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   _DeeAtomic32_CompareExchangeVal((Dee_int32_t*)&(x),(Dee_int32_t)(old_v),(Dee_int32_t)(new_v))
#define DeeAtomic32_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) _DeeAtomic32_CompareExchangeVal((Dee_int32_t*)&(x),(Dee_int32_t)(old_v),(Dee_int32_t)(new_v))
#define DeeAtomic32_FetchInc(x,order)                                                   ((*(Dee_int32_t*)&(x))++)
#define DeeAtomic32_IncFetch(x,order)                                                   (++(*(Dee_int32_t*)&(x)))
#define DeeAtomic32_FetchDec(x,order)                                                   ((*(Dee_int32_t*)&(x))--)
#define DeeAtomic32_DecFetch(x,order)                                                   (--(*(Dee_int32_t*)&(x)))
#define DeeAtomic32_FetchAdd(x,v,order)                                                 _DeeAtomic32_FetchAdd((Dee_int32_t*)&(x),(Dee_int32_t)(v))
#define DeeAtomic32_AddFetch(x,v,order)                                                 ((*(Dee_int32_t*)&(x))+=(Dee_int32_t)(v))
#define DeeAtomic32_FetchSub(x,v,order)                                                 _DeeAtomic32_FetchSub((Dee_int32_t*)&(x),(Dee_int32_t)(v))
#define DeeAtomic32_SubFetch(x,v,order)                                                 ((*(Dee_int32_t*)&(x))-=(Dee_int32_t)(v))
#define DeeAtomic32_FetchAnd(x,v,order)                                                 _DeeAtomic32_FetchAnd((Dee_int32_t*)&(x),(Dee_int32_t)(v))
#define DeeAtomic32_AndFetch(x,v,order)                                                 ((*(Dee_int32_t*)&(x))&=(Dee_int32_t)(v))
#define DeeAtomic32_FetchOr(x,v,order)                                                  _DeeAtomic32_FetchOr((Dee_int32_t*)&(x),(Dee_int32_t)(v))
#define DeeAtomic32_OrFetch(x,v,order)                                                  ((*(Dee_int32_t*)&(x))|=(Dee_int32_t)(v))
#define DeeAtomic32_FetchXor(x,v,order)                                                 _DeeAtomic32_FetchXor((Dee_int32_t*)&(x),(Dee_int32_t)(v))
#define DeeAtomic32_XorFetch(x,v,order)                                                 ((*(Dee_int32_t*)&(x))^=(Dee_int32_t)(v))
#define DeeAtomic32_FetchNand(x,v,order)                                                _DeeAtomic32_FetchNand((Dee_int32_t*)&(x),(Dee_int32_t)(v))
#define DeeAtomic32_NandFetch(x,v,order)                                                _DeeAtomic32_NandFetch((Dee_int32_t*)&(x),(Dee_int32_t)(v))

//////////////////////////////////////////////////////////////////////////
// DeeAtomic64 Native API
#define DeeAtomic64_Store(x,v,order)                                                    (*(Dee_int64_t*)&(x)=(Dee_int64_t)(v))
#define DeeAtomic64_Load(x,order)                                                       (*(Dee_int64_t*)&(x))
#define DeeAtomic64_Exchange(x,v,order)                                                 _DeeAtomic64_Exchange((Dee_int64_t*)&(x),(Dee_int64_t)(v))
#define DeeAtomic64_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      _DeeAtomic64_CompareExchange((Dee_int64_t*)&(x),(Dee_int64_t)(old_v),(Dee_int64_t)(new_v))
#define DeeAtomic64_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    _DeeAtomic64_CompareExchange((Dee_int64_t*)&(x),(Dee_int64_t)(old_v),(Dee_int64_t)(new_v))
#define DeeAtomic64_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   _DeeAtomic64_CompareExchangeVal((Dee_int64_t*)&(x),(Dee_int64_t)(old_v),(Dee_int64_t)(new_v))
#define DeeAtomic64_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) _DeeAtomic64_CompareExchangeVal((Dee_int64_t*)&(x),(Dee_int64_t)(old_v),(Dee_int64_t)(new_v))
#define DeeAtomic64_FetchInc(x,order)                                                   ((*(Dee_int64_t*)&(x))++)
#define DeeAtomic64_IncFetch(x,order)                                                   (++(*(Dee_int64_t*)&(x)))
#define DeeAtomic64_FetchDec(x,order)                                                   ((*(Dee_int64_t*)&(x))--)
#define DeeAtomic64_DecFetch(x,order)                                                   (--(*(Dee_int64_t*)&(x)))
#define DeeAtomic64_FetchAdd(x,v,order)                                                 _DeeAtomic64_FetchAdd((Dee_int64_t*)&(x),(Dee_int64_t)(v))
#define DeeAtomic64_AddFetch(x,v,order)                                                 ((*(Dee_int64_t*)&(x))+=(Dee_int64_t)(v))
#define DeeAtomic64_FetchSub(x,v,order)                                                 _DeeAtomic64_FetchSub((Dee_int64_t*)&(x),(Dee_int64_t)(v))
#define DeeAtomic64_SubFetch(x,v,order)                                                 ((*(Dee_int64_t*)&(x))-=(Dee_int64_t)(v))
#define DeeAtomic64_FetchAnd(x,v,order)                                                 _DeeAtomic64_FetchAnd((Dee_int64_t*)&(x),(Dee_int64_t)(v))
#define DeeAtomic64_AndFetch(x,v,order)                                                 ((*(Dee_int64_t*)&(x))&=(Dee_int64_t)(v))
#define DeeAtomic64_FetchOr(x,v,order)                                                  _DeeAtomic64_FetchOr((Dee_int64_t*)&(x),(Dee_int64_t)(v))
#define DeeAtomic64_OrFetch(x,v,order)                                                  ((*(Dee_int64_t*)&(x))|=(Dee_int64_t)(v))
#define DeeAtomic64_FetchXor(x,v,order)                                                 _DeeAtomic64_FetchXor((Dee_int64_t*)&(x),(Dee_int64_t)(v))
#define DeeAtomic64_XorFetch(x,v,order)                                                 ((*(Dee_int64_t*)&(x))^=(Dee_int64_t)(v))
#define DeeAtomic64_FetchNand(x,v,order)                                                _DeeAtomic64_FetchNand((Dee_int64_t*)&(x),(Dee_int64_t)(v))
#define DeeAtomic64_NandFetch(x,v,order)                                                _DeeAtomic64_NandFetch((Dee_int64_t*)&(x),(Dee_int64_t)(v))

DEE_DECL_END

#endif /* !GUARD_DEEMON___ATOMIC_INTRIN_NOTHREAD_INL */
