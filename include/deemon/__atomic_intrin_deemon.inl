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
#ifndef GUARD_DEEMON___ATOMIC_INTRIN_DEEMON_INL
#define GUARD_DEEMON___ATOMIC_INTRIN_DEEMON_INL 1
#ifndef DEE_ATOMIC_INTRIN_INCLUDE_MAGIC
#error "#include <deemon/__atomic_intrin.inl> instead of this file"
#endif

#include <deemon/__conf.inl>
#include <atomic> /* memory_order_* */

#ifdef __INTELLISENSE__
#define _Atomic /* nothing */
#endif

DEE_DECL_BEGIN

typedef _Atomic __int8  *__patomicint8;
typedef _Atomic __int16 *__patomicint16;
typedef _Atomic __int32 *__patomicint32;
typedef _Atomic __int64 *__patomicint64;


//////////////////////////////////////////////////////////////////////////
// DeeAtomic8 Native API
#define DeeAtomic8_Store(x,v,order)                                                     __patomicint8(&(x))->store(v,order)
#define DeeAtomic8_Load(x,order)                                                        __patomicint8(&(x))->load(order)
#define DeeAtomic8_Exchange(x,v,order)                                                  __patomicint8(&(x))->exchange(v,order)
#define DeeAtomic8_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)       __patomicint8(&(x))->compare_exchange_weak(old_v,new_v,success_order,failure_order)
#define DeeAtomic8_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)     __patomicint8(&(x))->compare_exchange_strong(old_v,new_v,success_order,failure_order)
#define DeeAtomic8_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)    __patomicint8(&(x))->compare_exchange_weak_val(old_v,new_v,success_order,failure_order)
#define DeeAtomic8_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order)  __patomicint8(&(x))->compare_exchange_strong_val(old_v,new_v,success_order,failure_order)
#define DeeAtomic8_FetchInc(x,order)                                                    __patomicint8(&(x))->fetch_inc(order)
#define DeeAtomic8_IncFetch(x,order)                                                    __patomicint8(&(x))->inc_fetch(order)
#define DeeAtomic8_FetchDec(x,order)                                                    __patomicint8(&(x))->fetch_dec(order)
#define DeeAtomic8_DecFetch(x,order)                                                    __patomicint8(&(x))->dec_fetch(order)
#define DeeAtomic8_FetchAdd(x,v,order)                                                  __patomicint8(&(x))->fetch_add(v,order)
#define DeeAtomic8_AddFetch(x,v,order)                                                  __patomicint8(&(x))->add_fetch(v,order)
#define DeeAtomic8_FetchSub(x,v,order)                                                  __patomicint8(&(x))->fetch_sub(v,order)
#define DeeAtomic8_SubFetch(x,v,order)                                                  __patomicint8(&(x))->sub_fetch(v,order)
#define DeeAtomic8_FetchAnd(x,v,order)                                                  __patomicint8(&(x))->fetch_and(v,order)
#define DeeAtomic8_AndFetch(x,v,order)                                                  __patomicint8(&(x))->and_fetch(v,order)
#define DeeAtomic8_FetchOr(x,v,order)                                                   __patomicint8(&(x))->fetch_or(v,order)
#define DeeAtomic8_OrFetch(x,v,order)                                                   __patomicint8(&(x))->or_fetch(v,order)
#define DeeAtomic8_FetchXor(x,v,order)                                                  __patomicint8(&(x))->fetch_xor(v,order)
#define DeeAtomic8_XorFetch(x,v,order)                                                  __patomicint8(&(x))->xor_fetch(v,order)
#define DeeAtomic8_FetchNand(x,v,order)                                                 __patomicint8(&(x))->fetch_nand(v,order)
#define DeeAtomic8_NandFetch(x,v,order)                                                 __patomicint8(&(x))->nand_fetch(v,order)


//////////////////////////////////////////////////////////////////////////
// DeeAtomic16 Native API
#define DeeAtomic16_Store(x,v,order)                                                    __patomicint16(&(x))->store(v,order)
#define DeeAtomic16_Load(x,order)                                                       __patomicint16(&(x))->load(order)
#define DeeAtomic16_Exchange(x,v,order)                                                 __patomicint16(&(x))->exchange(v,order)
#define DeeAtomic16_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      __patomicint16(&(x))->compare_exchange_weak(old_v,new_v,success_order,failure_order)
#define DeeAtomic16_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    __patomicint16(&(x))->compare_exchange_strong(old_v,new_v,success_order,failure_order)
#define DeeAtomic16_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   __patomicint16(&(x))->compare_exchange_weak_val(old_v,new_v,success_order,failure_order)
#define DeeAtomic16_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) __patomicint16(&(x))->compare_exchange_strong_val(old_v,new_v,success_order,failure_order)
#define DeeAtomic16_FetchInc(x,order)                                                   __patomicint16(&(x))->fetch_inc(order)
#define DeeAtomic16_IncFetch(x,order)                                                   __patomicint16(&(x))->inc_fetch(order)
#define DeeAtomic16_FetchDec(x,order)                                                   __patomicint16(&(x))->fetch_dec(order)
#define DeeAtomic16_DecFetch(x,order)                                                   __patomicint16(&(x))->dec_fetch(order)
#define DeeAtomic16_FetchAdd(x,v,order)                                                 __patomicint16(&(x))->fetch_add(v,order)
#define DeeAtomic16_AddFetch(x,v,order)                                                 __patomicint16(&(x))->add_fetch(v,order)
#define DeeAtomic16_FetchSub(x,v,order)                                                 __patomicint16(&(x))->fetch_sub(v,order)
#define DeeAtomic16_SubFetch(x,v,order)                                                 __patomicint16(&(x))->sub_fetch(v,order)
#define DeeAtomic16_FetchAnd(x,v,order)                                                 __patomicint16(&(x))->fetch_and(v,order)
#define DeeAtomic16_AndFetch(x,v,order)                                                 __patomicint16(&(x))->and_fetch(v,order)
#define DeeAtomic16_FetchOr(x,v,order)                                                  __patomicint16(&(x))->fetch_or(v,order)
#define DeeAtomic16_OrFetch(x,v,order)                                                  __patomicint16(&(x))->or_fetch(v,order)
#define DeeAtomic16_FetchXor(x,v,order)                                                 __patomicint16(&(x))->fetch_xor(v,order)
#define DeeAtomic16_XorFetch(x,v,order)                                                 __patomicint16(&(x))->xor_fetch(v,order)
#define DeeAtomic16_FetchNand(x,v,order)                                                __patomicint16(&(x))->fetch_nand(v,order)
#define DeeAtomic16_NandFetch(x,v,order)                                                __patomicint16(&(x))->nand_fetch(v,order)

//////////////////////////////////////////////////////////////////////////
// DeeAtomic32 Native API
#define DeeAtomic32_Store(x,v,order)                                                    __patomicint32(&(x))->store(v,order)
#define DeeAtomic32_Load(x,order)                                                       __patomicint32(&(x))->load(order)
#define DeeAtomic32_Exchange(x,v,order)                                                 __patomicint32(&(x))->exchange(v,order)
#define DeeAtomic32_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      __patomicint32(&(x))->compare_exchange_weak(old_v,new_v,success_order,failure_order)
#define DeeAtomic32_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    __patomicint32(&(x))->compare_exchange_strong(old_v,new_v,success_order,failure_order)
#define DeeAtomic32_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   __patomicint32(&(x))->compare_exchange_weak_val(old_v,new_v,success_order,failure_order)
#define DeeAtomic32_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) __patomicint32(&(x))->compare_exchange_strong_val(old_v,new_v,success_order,failure_order)
#define DeeAtomic32_FetchInc(x,order)                                                   __patomicint32(&(x))->fetch_inc(order)
#define DeeAtomic32_IncFetch(x,order)                                                   __patomicint32(&(x))->inc_fetch(order)
#define DeeAtomic32_FetchDec(x,order)                                                   __patomicint32(&(x))->fetch_dec(order)
#define DeeAtomic32_DecFetch(x,order)                                                   __patomicint32(&(x))->dec_fetch(order)
#define DeeAtomic32_FetchAdd(x,v,order)                                                 __patomicint32(&(x))->fetch_add(v,order)
#define DeeAtomic32_AddFetch(x,v,order)                                                 __patomicint32(&(x))->add_fetch(v,order)
#define DeeAtomic32_FetchSub(x,v,order)                                                 __patomicint32(&(x))->fetch_sub(v,order)
#define DeeAtomic32_SubFetch(x,v,order)                                                 __patomicint32(&(x))->sub_fetch(v,order)
#define DeeAtomic32_FetchAnd(x,v,order)                                                 __patomicint32(&(x))->fetch_and(v,order)
#define DeeAtomic32_AndFetch(x,v,order)                                                 __patomicint32(&(x))->and_fetch(v,order)
#define DeeAtomic32_FetchOr(x,v,order)                                                  __patomicint32(&(x))->fetch_or(v,order)
#define DeeAtomic32_OrFetch(x,v,order)                                                  __patomicint32(&(x))->or_fetch(v,order)
#define DeeAtomic32_FetchXor(x,v,order)                                                 __patomicint32(&(x))->fetch_xor(v,order)
#define DeeAtomic32_XorFetch(x,v,order)                                                 __patomicint32(&(x))->xor_fetch(v,order)
#define DeeAtomic32_FetchNand(x,v,order)                                                __patomicint32(&(x))->fetch_nand(v,order)
#define DeeAtomic32_NandFetch(x,v,order)                                                __patomicint32(&(x))->nand_fetch(v,order)

//////////////////////////////////////////////////////////////////////////
// DeeAtomic64 Native API
#define DeeAtomic64_Store(x,v,order)                                                    __patomicint64(&(x))->store(v,order)
#define DeeAtomic64_Load(x,order)                                                       __patomicint64(&(x))->load(order)
#define DeeAtomic64_Exchange(x,v,order)                                                 __patomicint64(&(x))->exchange(v,order)
#define DeeAtomic64_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      __patomicint64(&(x))->compare_exchange_weak(old_v,new_v,success_order,failure_order)
#define DeeAtomic64_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    __patomicint64(&(x))->compare_exchange_strong(old_v,new_v,success_order,failure_order)
#define DeeAtomic64_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   __patomicint64(&(x))->compare_exchange_weak_val(old_v,new_v,success_order,failure_order)
#define DeeAtomic64_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) __patomicint64(&(x))->compare_exchange_strong_val(old_v,new_v,success_order,failure_order)
#define DeeAtomic64_FetchInc(x,order)                                                   __patomicint64(&(x))->fetch_inc(order)
#define DeeAtomic64_IncFetch(x,order)                                                   __patomicint64(&(x))->inc_fetch(order)
#define DeeAtomic64_FetchDec(x,order)                                                   __patomicint64(&(x))->fetch_dec(order)
#define DeeAtomic64_DecFetch(x,order)                                                   __patomicint64(&(x))->dec_fetch(order)
#define DeeAtomic64_FetchAdd(x,v,order)                                                 __patomicint64(&(x))->fetch_add(v,order)
#define DeeAtomic64_AddFetch(x,v,order)                                                 __patomicint64(&(x))->add_fetch(v,order)
#define DeeAtomic64_FetchSub(x,v,order)                                                 __patomicint64(&(x))->fetch_sub(v,order)
#define DeeAtomic64_SubFetch(x,v,order)                                                 __patomicint64(&(x))->sub_fetch(v,order)
#define DeeAtomic64_FetchAnd(x,v,order)                                                 __patomicint64(&(x))->fetch_and(v,order)
#define DeeAtomic64_AndFetch(x,v,order)                                                 __patomicint64(&(x))->and_fetch(v,order)
#define DeeAtomic64_FetchOr(x,v,order)                                                  __patomicint64(&(x))->fetch_or(v,order)
#define DeeAtomic64_OrFetch(x,v,order)                                                  __patomicint64(&(x))->or_fetch(v,order)
#define DeeAtomic64_FetchXor(x,v,order)                                                 __patomicint64(&(x))->fetch_xor(v,order)
#define DeeAtomic64_XorFetch(x,v,order)                                                 __patomicint64(&(x))->xor_fetch(v,order)
#define DeeAtomic64_FetchNand(x,v,order)                                                __patomicint64(&(x))->fetch_nand(v,order)
#define DeeAtomic64_NandFetch(x,v,order)                                                __patomicint64(&(x))->nand_fetch(v,order)

DEE_DECL_END

#endif /* !GUARD_DEEMON___ATOMIC_INTRIN_DEEMON_INL */
