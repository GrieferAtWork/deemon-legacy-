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
#ifndef GUARD_DEEMON___ATOMIC_INTRIN_MSVC_INL
#define GUARD_DEEMON___ATOMIC_INTRIN_MSVC_INL 1
#ifndef DEE_ATOMIC_INTRIN_INCLUDE_MAGIC
#error "#include <deemon/__atomic_intrin.inl> instead of this file"
#endif

#include <deemon/__conf.inl>

//////////////////////////////////////////////////////////////////////////
// NOTE: A portion of the stuff in this file is derived
//       from the msvc standard header: <xatomic.h>
//    -> We can't use that one though, because its c++ only...


#define _DeeAtomic_MemoryOrderUpperBound_relaxed_relaxed relaxed
#define _DeeAtomic_MemoryOrderUpperBound_relaxed_consume consume
#define _DeeAtomic_MemoryOrderUpperBound_relaxed_acquire acquire
#define _DeeAtomic_MemoryOrderUpperBound_relaxed_release release
#define _DeeAtomic_MemoryOrderUpperBound_relaxed_acq_rel acq_rel
#define _DeeAtomic_MemoryOrderUpperBound_relaxed_seq_cst seq_cst
#define _DeeAtomic_MemoryOrderUpperBound_consume_relaxed consume
#define _DeeAtomic_MemoryOrderUpperBound_consume_consume consume
#define _DeeAtomic_MemoryOrderUpperBound_consume_acquire acquire
#define _DeeAtomic_MemoryOrderUpperBound_consume_release release
#define _DeeAtomic_MemoryOrderUpperBound_consume_acq_rel acq_rel
#define _DeeAtomic_MemoryOrderUpperBound_consume_seq_cst seq_cst
#define _DeeAtomic_MemoryOrderUpperBound_acquire_relaxed acquire
#define _DeeAtomic_MemoryOrderUpperBound_acquire_consume acquire
#define _DeeAtomic_MemoryOrderUpperBound_acquire_acquire acquire
#define _DeeAtomic_MemoryOrderUpperBound_acquire_release release
#define _DeeAtomic_MemoryOrderUpperBound_acquire_acq_rel acq_rel
#define _DeeAtomic_MemoryOrderUpperBound_acquire_seq_cst seq_cst
#define _DeeAtomic_MemoryOrderUpperBound_release_relaxed release
#define _DeeAtomic_MemoryOrderUpperBound_release_consume release
#define _DeeAtomic_MemoryOrderUpperBound_release_acquire release
#define _DeeAtomic_MemoryOrderUpperBound_release_release release
#define _DeeAtomic_MemoryOrderUpperBound_release_acq_rel acq_rel
#define _DeeAtomic_MemoryOrderUpperBound_release_seq_cst seq_cst
#define _DeeAtomic_MemoryOrderUpperBound_acq_rel_relaxed acq_rel
#define _DeeAtomic_MemoryOrderUpperBound_acq_rel_consume acq_rel
#define _DeeAtomic_MemoryOrderUpperBound_acq_rel_acquire acq_rel
#define _DeeAtomic_MemoryOrderUpperBound_acq_rel_release acq_rel
#define _DeeAtomic_MemoryOrderUpperBound_acq_rel_acq_rel acq_rel
#define _DeeAtomic_MemoryOrderUpperBound_acq_rel_seq_cst seq_cst
#define _DeeAtomic_MemoryOrderUpperBound_seq_cst_relaxed seq_cst
#define _DeeAtomic_MemoryOrderUpperBound_seq_cst_consume seq_cst
#define _DeeAtomic_MemoryOrderUpperBound_seq_cst_acquire seq_cst
#define _DeeAtomic_MemoryOrderUpperBound_seq_cst_release seq_cst
#define _DeeAtomic_MemoryOrderUpperBound_seq_cst_acq_rel seq_cst
#define _DeeAtomic_MemoryOrderUpperBound_seq_cst_seq_cst seq_cst
#define _DeeAtomic_MemoryOrderUpperBound3(a,b) _DeeAtomic_MemoryOrderUpperBound_##a##_##b
#define _DeeAtomic_MemoryOrderUpperBound2(a,b) _DeeAtomic_MemoryOrderUpperBound3(a,b)
#define _DeeAtomic_MemoryOrderUpperBound(a,b)  _DeeAtomic_MemoryOrderUpperBound2(DEE_ATOMIC_ORDER(a),DEE_ATOMIC_ORDER(b))

#if 0
#define DEE_ATOMIC_ORDER_relaxed              relaxed
#define DEE_ATOMIC_ORDER_consume              consume
#define DEE_ATOMIC_ORDER_acquire              acquire
#define DEE_ATOMIC_ORDER_release              release
#define DEE_ATOMIC_ORDER_acq_rel              acq_rel
#define DEE_ATOMIC_ORDER_seq_cst              seq_cst
#endif
#define DEE_ATOMIC_ORDER_memory_order_relaxed relaxed
#define DEE_ATOMIC_ORDER_memory_order_consume consume
#define DEE_ATOMIC_ORDER_memory_order_acquire acquire
#define DEE_ATOMIC_ORDER_memory_order_release release
#define DEE_ATOMIC_ORDER_memory_order_acq_rel acq_rel
#define DEE_ATOMIC_ORDER_memory_order_seq_cst seq_cst
#define DEE_ATOMIC_ORDER(x) DEE_ATOMIC_ORDER_##x

#define DEE_ATOMIC8_NATIVE_T  char
#define DEE_ATOMIC16_NATIVE_T short
#define DEE_ATOMIC32_NATIVE_T long
#define DEE_ATOMIC64_NATIVE_T __int64



//////////////////////////////////////////////////////////////////////////
// DeeAtomic8 Native API
#define DeeAtomic8_Store(x,v,order)                                                    DEE_PP_CAT_2(_DeeAtomic8_Store_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(v))
#define DeeAtomic8_Load(x,order)                                                       DEE_PP_CAT_2(_DeeAtomic8_Load_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x))
#define DeeAtomic8_Exchange(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic8_Exchange_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(v))
#define DeeAtomic8_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      DEE_PP_CAT_2(_DeeAtomic8_CompareExchange_,_DeeAtomic_MemoryOrderUpperBound(success_order,failure_order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(old_v),(DEE_ATOMIC8_NATIVE_T)(new_v))
#define DeeAtomic8_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    DEE_PP_CAT_2(_DeeAtomic8_CompareExchange_,_DeeAtomic_MemoryOrderUpperBound(success_order,failure_order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(old_v),(DEE_ATOMIC8_NATIVE_T)(new_v))
#define DeeAtomic8_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   DEE_PP_CAT_2(_DeeAtomic8_CompareExchangeVal_,_DeeAtomic_MemoryOrderUpperBound(success_order,failure_order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(old_v),(DEE_ATOMIC8_NATIVE_T)(new_v))
#define DeeAtomic8_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) DEE_PP_CAT_2(_DeeAtomic8_CompareExchangeVal_,_DeeAtomic_MemoryOrderUpperBound(success_order,failure_order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(old_v),(DEE_ATOMIC8_NATIVE_T)(new_v))
#define DeeAtomic8_FetchInc(x,order)                                                   DEE_PP_CAT_2(_DeeAtomic8_FetchInc_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x))
#define DeeAtomic8_IncFetch(x,order)                                                   DEE_PP_CAT_2(_DeeAtomic8_IncFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x))
#define DeeAtomic8_FetchDec(x,order)                                                   DEE_PP_CAT_2(_DeeAtomic8_FetchDec_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x))
#define DeeAtomic8_DecFetch(x,order)                                                   DEE_PP_CAT_2(_DeeAtomic8_DecFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x))
#define DeeAtomic8_FetchAdd(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic8_FetchAdd_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(v))
#define DeeAtomic8_AddFetch(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic8_AddFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(v))
#define DeeAtomic8_FetchSub(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic8_FetchAdd_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),0-(DEE_ATOMIC8_NATIVE_T)(v))
#define DeeAtomic8_SubFetch(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic8_AddFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),0-(DEE_ATOMIC8_NATIVE_T)(v))
#define DeeAtomic8_FetchAnd(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic8_FetchAnd_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(v))
#define DeeAtomic8_AndFetch(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic8_AndFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(v))
#define DeeAtomic8_FetchOr(x,v,order)                                                  DEE_PP_CAT_2(_DeeAtomic8_FetchOr_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(v))
#define DeeAtomic8_OrFetch(x,v,order)                                                  DEE_PP_CAT_2(_DeeAtomic8_OrFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(v))
#define DeeAtomic8_FetchXor(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic8_FetchXor_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(v))
#define DeeAtomic8_XorFetch(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic8_XorFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(v))
#define DeeAtomic8_FetchNand(x,v,order)                                                DEE_PP_CAT_2(_DeeAtomic8_FetchNand_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(v))
#define DeeAtomic8_NandFetch(x,v,order)                                                DEE_PP_CAT_2(_DeeAtomic8_NandFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(v))


//////////////////////////////////////////////////////////////////////////
// DeeAtomic16 Native API
#define DeeAtomic16_Store(x,v,order)                                                    DEE_PP_CAT_2(_DeeAtomic16_Store_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(v))
#define DeeAtomic16_Load(x,order)                                                       DEE_PP_CAT_2(_DeeAtomic16_Load_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x))
#define DeeAtomic16_Exchange(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic16_Exchange_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(v))
#define DeeAtomic16_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      DEE_PP_CAT_2(_DeeAtomic16_CompareExchange_,_DeeAtomic_MemoryOrderUpperBound(success_order,failure_order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(old_v),(DEE_ATOMIC16_NATIVE_T)(new_v))
#define DeeAtomic16_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    DEE_PP_CAT_2(_DeeAtomic16_CompareExchange_,_DeeAtomic_MemoryOrderUpperBound(success_order,failure_order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(old_v),(DEE_ATOMIC16_NATIVE_T)(new_v))
#define DeeAtomic16_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   DEE_PP_CAT_2(_DeeAtomic16_CompareExchangeVal_,_DeeAtomic_MemoryOrderUpperBound(success_order,failure_order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(old_v),(DEE_ATOMIC16_NATIVE_T)(new_v))
#define DeeAtomic16_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) DEE_PP_CAT_2(_DeeAtomic16_CompareExchangeVal_,_DeeAtomic_MemoryOrderUpperBound(success_order,failure_order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(old_v),(DEE_ATOMIC16_NATIVE_T)(new_v))
#define DeeAtomic16_FetchInc(x,order)                                                   DEE_PP_CAT_2(_DeeAtomic16_FetchInc_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x))
#define DeeAtomic16_IncFetch(x,order)                                                   DEE_PP_CAT_2(_DeeAtomic16_IncFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x))
#define DeeAtomic16_FetchDec(x,order)                                                   DEE_PP_CAT_2(_DeeAtomic16_FetchDec_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x))
#define DeeAtomic16_DecFetch(x,order)                                                   DEE_PP_CAT_2(_DeeAtomic16_DecFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x))
#define DeeAtomic16_FetchAdd(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic16_FetchAdd_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(v))
#define DeeAtomic16_AddFetch(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic16_AddFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(v))
#define DeeAtomic16_FetchSub(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic16_FetchAdd_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),0-(DEE_ATOMIC16_NATIVE_T)(v))
#define DeeAtomic16_SubFetch(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic16_AddFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),0-(DEE_ATOMIC16_NATIVE_T)(v))
#define DeeAtomic16_FetchAnd(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic16_FetchAnd_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(v))
#define DeeAtomic16_AndFetch(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic16_AndFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(v))
#define DeeAtomic16_FetchOr(x,v,order)                                                  DEE_PP_CAT_2(_DeeAtomic16_FetchOr_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(v))
#define DeeAtomic16_OrFetch(x,v,order)                                                  DEE_PP_CAT_2(_DeeAtomic16_OrFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(v))
#define DeeAtomic16_FetchXor(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic16_FetchXor_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(v))
#define DeeAtomic16_XorFetch(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic16_XorFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(v))
#define DeeAtomic16_FetchNand(x,v,order)                                                DEE_PP_CAT_2(_DeeAtomic16_FetchNand_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(v))
#define DeeAtomic16_NandFetch(x,v,order)                                                DEE_PP_CAT_2(_DeeAtomic16_NandFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(v))

//////////////////////////////////////////////////////////////////////////
// DeeAtomic32 Native API
#define DeeAtomic32_Store(x,v,order)                                                    DEE_PP_CAT_2(_DeeAtomic32_Store_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(v))
#define DeeAtomic32_Load(x,order)                                                       DEE_PP_CAT_2(_DeeAtomic32_Load_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x))
#define DeeAtomic32_Exchange(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic32_Exchange_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(v))
#define DeeAtomic32_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      DEE_PP_CAT_2(_DeeAtomic32_CompareExchange_,_DeeAtomic_MemoryOrderUpperBound(success_order,failure_order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(old_v),(DEE_ATOMIC32_NATIVE_T)(new_v))
#define DeeAtomic32_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    DEE_PP_CAT_2(_DeeAtomic32_CompareExchange_,_DeeAtomic_MemoryOrderUpperBound(success_order,failure_order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(old_v),(DEE_ATOMIC32_NATIVE_T)(new_v))
#define DeeAtomic32_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   DEE_PP_CAT_2(_DeeAtomic32_CompareExchangeVal_,_DeeAtomic_MemoryOrderUpperBound(success_order,failure_order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(old_v),(DEE_ATOMIC32_NATIVE_T)(new_v))
#define DeeAtomic32_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) DEE_PP_CAT_2(_DeeAtomic32_CompareExchangeVal_,_DeeAtomic_MemoryOrderUpperBound(success_order,failure_order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(old_v),(DEE_ATOMIC32_NATIVE_T)(new_v))
#define DeeAtomic32_FetchInc(x,order)                                                   DEE_PP_CAT_2(_DeeAtomic32_FetchInc_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x))
#define DeeAtomic32_IncFetch(x,order)                                                   DEE_PP_CAT_2(_DeeAtomic32_IncFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x))
#define DeeAtomic32_FetchDec(x,order)                                                   DEE_PP_CAT_2(_DeeAtomic32_FetchDec_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x))
#define DeeAtomic32_DecFetch(x,order)                                                   DEE_PP_CAT_2(_DeeAtomic32_DecFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x))
#define DeeAtomic32_FetchAdd(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic32_FetchAdd_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(v))
#define DeeAtomic32_AddFetch(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic32_AddFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(v))
#define DeeAtomic32_FetchSub(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic32_FetchAdd_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),0-(DEE_ATOMIC32_NATIVE_T)(v))
#define DeeAtomic32_SubFetch(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic32_AddFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),0-(DEE_ATOMIC32_NATIVE_T)(v))
#define DeeAtomic32_FetchAnd(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic32_FetchAnd_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(v))
#define DeeAtomic32_AndFetch(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic32_AndFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(v))
#define DeeAtomic32_FetchOr(x,v,order)                                                  DEE_PP_CAT_2(_DeeAtomic32_FetchOr_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(v))
#define DeeAtomic32_OrFetch(x,v,order)                                                  DEE_PP_CAT_2(_DeeAtomic32_OrFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(v))
#define DeeAtomic32_FetchXor(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic32_FetchXor_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(v))
#define DeeAtomic32_XorFetch(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic32_XorFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(v))
#define DeeAtomic32_FetchNand(x,v,order)                                                DEE_PP_CAT_2(_DeeAtomic32_FetchNand_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(v))
#define DeeAtomic32_NandFetch(x,v,order)                                                DEE_PP_CAT_2(_DeeAtomic32_NandFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(v))

//////////////////////////////////////////////////////////////////////////
// DeeAtomic64 Native API
#define DeeAtomic64_Store(x,v,order)                                                    DEE_PP_CAT_2(_DeeAtomic64_Store_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(v))
#define DeeAtomic64_Load(x,order)                                                       DEE_PP_CAT_2(_DeeAtomic64_Load_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x))
#define DeeAtomic64_Exchange(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic64_Exchange_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(v))
#define DeeAtomic64_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      DEE_PP_CAT_2(_DeeAtomic64_CompareExchange_,_DeeAtomic_MemoryOrderUpperBound(success_order,failure_order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(old_v),(DEE_ATOMIC64_NATIVE_T)(new_v))
#define DeeAtomic64_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    DEE_PP_CAT_2(_DeeAtomic64_CompareExchange_,_DeeAtomic_MemoryOrderUpperBound(success_order,failure_order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(old_v),(DEE_ATOMIC64_NATIVE_T)(new_v))
#define DeeAtomic64_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   DEE_PP_CAT_2(_DeeAtomic64_CompareExchangeVal_,_DeeAtomic_MemoryOrderUpperBound(success_order,failure_order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(old_v),(DEE_ATOMIC64_NATIVE_T)(new_v))
#define DeeAtomic64_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) DEE_PP_CAT_2(_DeeAtomic64_CompareExchangeVal_,_DeeAtomic_MemoryOrderUpperBound(success_order,failure_order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(old_v),(DEE_ATOMIC64_NATIVE_T)(new_v))
#define DeeAtomic64_FetchInc(x,order)                                                   DEE_PP_CAT_2(_DeeAtomic64_FetchInc_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x))
#define DeeAtomic64_IncFetch(x,order)                                                   DEE_PP_CAT_2(_DeeAtomic64_IncFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x))
#define DeeAtomic64_FetchDec(x,order)                                                   DEE_PP_CAT_2(_DeeAtomic64_FetchDec_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x))
#define DeeAtomic64_DecFetch(x,order)                                                   DEE_PP_CAT_2(_DeeAtomic64_DecFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x))
#define DeeAtomic64_FetchAdd(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic64_FetchAdd_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(v))
#define DeeAtomic64_AddFetch(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic64_AddFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(v))
#define DeeAtomic64_FetchSub(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic64_FetchAdd_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),0-(DEE_ATOMIC64_NATIVE_T)(v))
#define DeeAtomic64_SubFetch(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic64_AddFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),0-(DEE_ATOMIC64_NATIVE_T)(v))
#define DeeAtomic64_FetchAnd(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic64_FetchAnd_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(v))
#define DeeAtomic64_AndFetch(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic64_AndFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(v))
#define DeeAtomic64_FetchOr(x,v,order)                                                  DEE_PP_CAT_2(_DeeAtomic64_FetchOr_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(v))
#define DeeAtomic64_OrFetch(x,v,order)                                                  DEE_PP_CAT_2(_DeeAtomic64_OrFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(v))
#define DeeAtomic64_FetchXor(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic64_FetchXor_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(v))
#define DeeAtomic64_XorFetch(x,v,order)                                                 DEE_PP_CAT_2(_DeeAtomic64_XorFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(v))
#define DeeAtomic64_FetchNand(x,v,order)                                                DEE_PP_CAT_2(_DeeAtomic64_FetchNand_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(v))
#define DeeAtomic64_NandFetch(x,v,order)                                                DEE_PP_CAT_2(_DeeAtomic64_NandFetch_,DEE_ATOMIC_ORDER(order))((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(v))


#ifdef DEE_PLATFORM_WINDOWS
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4820 4668 4255)
#include <intrin.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#pragma intrinsic(_ReadWriteBarrier)
#if defined(_M_ARM)
#pragma intrinsic(__dmb)
#define _DEE_MSVC_MEMORY_BARRIER() __dmb(_ARM_BARRIER_ISH)
#endif
#define _DEE_MSVC_COMPILER_BARRIER _ReadWriteBarrier
#if defined(_M_ARM)
#define _DEE_ATOMIC_INTRIN4(x)\
 __pragma(intrinsic(x##_nf))\
 __pragma(intrinsic(x##_acq))\
 __pragma(intrinsic(x##_rel))\
 __pragma(intrinsic(x))
#define _DEE_ATOMIC_INTRIN_RELAXED(x)	x##_nf
#define _DEE_ATOMIC_INTRIN_ACQUIRE(x)	x##_acq
#define _DEE_ATOMIC_INTRIN_RELEASE(x)	x##_rel
#define _DEE_ATOMIC_INTRIN_SEQ_CST(x)	x
#else
#define _DEE_ATOMIC_INTRIN4(x)	__pragma(intrinsic(x))
#define _DEE_ATOMIC_INTRIN_RELAXED(x)	x
#define _DEE_ATOMIC_INTRIN_ACQUIRE(x)	x
#define _DEE_ATOMIC_INTRIN_RELEASE(x)	x
#define _DEE_ATOMIC_INTRIN_SEQ_CST(x)	x
#endif

DEE_DECL_BEGIN


#define _DeeAtomic8_Load_acquire _DeeAtomic8_Load_seq_cst
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T)
_DeeAtomic8_Load_seq_cst(DEE_ATOMIC8_NATIVE_T volatile *x) {
 DEE_ATOMIC8_NATIVE_T result;
#if defined(_M_ARM)
#pragma intrinsic(__iso_volatile_store8)
 result = __iso_volatile_load8((char volatile *)x);
 _DEE_MSVC_MEMORY_BARRIER();
#else
 result = *x;
 _DEE_MSVC_COMPILER_BARRIER();
#endif
 return result;
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST void)
_DeeAtomic8_Store_release(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) {
#if defined(_M_ARM)
 _DEE_MSVC_MEMORY_BARRIER();
 __iso_volatile_store8((char volatile *)x,v);
#else
 _DEE_MSVC_COMPILER_BARRIER();
 *x = v;
#endif
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST void)
_DeeAtomic8_Store_seq_cst(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) {
#if defined(_M_ARM)
 _DEE_MSVC_MEMORY_BARRIER();
 __iso_volatile_store8((char volatile *)x,v);
 _DEE_MSVC_MEMORY_BARRIER();
#else
 _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedExchange8)((char volatile *)x,v);
#endif
 }
#if defined(_M_ARM)
#define _DeeAtomic8_Store_relaxed      __iso_volatile_store8
#define _DeeAtomic8_Load_relaxed       __iso_volatile_load8
#else
#define _DeeAtomic8_Store_relaxed(x,v) (void)(*(x)=(v))
#define _DeeAtomic8_Load_relaxed(x)    *(x)
#endif
_DEE_ATOMIC_INTRIN4(_InterlockedExchange8)
#define _DeeAtomic8_Exchange_seq_cst _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedExchange8)
#define _DeeAtomic8_Exchange_relaxed _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedExchange8)
#define _DeeAtomic8_Exchange_acquire _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedExchange8)
#define _DeeAtomic8_Exchange_release _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedExchange8)
_DEE_ATOMIC_INTRIN4(_InterlockedCompareExchange8)
#define _DeeAtomic8_CompareExchangeVal_seq_cst(x,old_v,new_v) _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedCompareExchange8)(x,new_v,old_v)
#define _DeeAtomic8_CompareExchangeVal_relaxed(x,old_v,new_v) _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedCompareExchange8)(x,new_v,old_v)
#define _DeeAtomic8_CompareExchangeVal_acquire(x,old_v,new_v) _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedCompareExchange8)(x,new_v,old_v)
#define _DeeAtomic8_CompareExchangeVal_release(x,old_v,new_v) _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedCompareExchange8)(x,new_v,old_v)
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic8_CompareExchange_seq_cst(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T old_v, DEE_ATOMIC8_NATIVE_T new_v) { return _DeeAtomic8_CompareExchangeVal_seq_cst(x,old_v,new_v) == old_v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic8_CompareExchange_relaxed(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T old_v, DEE_ATOMIC8_NATIVE_T new_v) { return _DeeAtomic8_CompareExchangeVal_relaxed(x,old_v,new_v) == old_v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic8_CompareExchange_acquire(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T old_v, DEE_ATOMIC8_NATIVE_T new_v) { return _DeeAtomic8_CompareExchangeVal_acquire(x,old_v,new_v) == old_v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic8_CompareExchange_release(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T old_v, DEE_ATOMIC8_NATIVE_T new_v) { return _DeeAtomic8_CompareExchangeVal_release(x,old_v,new_v) == old_v; }

#define _DeeAtomic8_IncFetch_seq_cst(x)  _DeeAtomic8_FetchAdd_seq_cst(x,1)
#define _DeeAtomic8_IncFetch_relaxed(x)  _DeeAtomic8_FetchAdd_relaxed(x,1)
#define _DeeAtomic8_IncFetch_acquire(x)  _DeeAtomic8_FetchAdd_acquire(x,1)
#define _DeeAtomic8_IncFetch_release(x)  _DeeAtomic8_FetchAdd_release(x,1)
#define _DeeAtomic8_DecFetch_seq_cst(x)  _DeeAtomic8_FetchAdd_seq_cst(x,-1)
#define _DeeAtomic8_DecFetch_relaxed(x)  _DeeAtomic8_FetchAdd_relaxed(x,-1)
#define _DeeAtomic8_DecFetch_acquire(x)  _DeeAtomic8_FetchAdd_acquire(x,-1)
#define _DeeAtomic8_DecFetch_release(x)  _DeeAtomic8_FetchAdd_release(x,-1)
#define _DeeAtomic8_FetchInc_seq_cst(x) (_DeeAtomic8_IncFetch_seq_cst(x)-1)
#define _DeeAtomic8_FetchInc_relaxed(x) (_DeeAtomic8_IncFetch_relaxed(x)-1)
#define _DeeAtomic8_FetchInc_acquire(x) (_DeeAtomic8_IncFetch_acquire(x)-1)
#define _DeeAtomic8_FetchInc_release(x) (_DeeAtomic8_IncFetch_release(x)-1)
#define _DeeAtomic8_FetchDec_seq_cst(x) (_DeeAtomic8_DecFetch_seq_cst(x)+1)
#define _DeeAtomic8_FetchDec_relaxed(x) (_DeeAtomic8_DecFetch_relaxed(x)+1)
#define _DeeAtomic8_FetchDec_acquire(x) (_DeeAtomic8_DecFetch_acquire(x)+1)
#define _DeeAtomic8_FetchDec_release(x) (_DeeAtomic8_DecFetch_release(x)+1)

_DEE_ATOMIC_INTRIN4(_InterlockedExchangeAdd8)
#define _DeeAtomic8_FetchAdd_seq_cst _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedExchangeAdd8)
#define _DeeAtomic8_FetchAdd_relaxed _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedExchangeAdd8)
#define _DeeAtomic8_FetchAdd_acquire _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedExchangeAdd8)
#define _DeeAtomic8_FetchAdd_release _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedExchangeAdd8)
_DEE_ATOMIC_INTRIN4(_InterlockedAnd8)
#define _DeeAtomic8_FetchAnd_seq_cst _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedAnd8)
#define _DeeAtomic8_FetchAnd_relaxed _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedAnd8)
#define _DeeAtomic8_FetchAnd_acquire _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedAnd8)
#define _DeeAtomic8_FetchAnd_release _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedAnd8)
_DEE_ATOMIC_INTRIN4(_InterlockedOr8)
#define _DeeAtomic8_FetchOr_seq_cst  _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedOr8)
#define _DeeAtomic8_FetchOr_relaxed  _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedOr8)
#define _DeeAtomic8_FetchOr_acquire  _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedOr8)
#define _DeeAtomic8_FetchOr_release  _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedOr8)
_DEE_ATOMIC_INTRIN4(_InterlockedXor8)
#define _DeeAtomic8_FetchXor_seq_cst _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedXor8)
#define _DeeAtomic8_FetchXor_relaxed _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedXor8)
#define _DeeAtomic8_FetchXor_acquire _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedXor8)
#define _DeeAtomic8_FetchXor_release _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedXor8)


#if defined(_M_ARM)
#pragma intrinsic(__iso_volatile_store16)
#define _DeeAtomic16_Store_relaxed      __iso_volatile_store16
#else
#define _DeeAtomic16_Store_relaxed(x,v) (void)(*(x)=(v))
#endif
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST void)
_DeeAtomic16_Store_release(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) {
#if defined(_M_ARM)
 _DEE_MSVC_MEMORY_BARRIER();
 __iso_volatile_store16((short volatile *)x,v);
#else
 _DEE_MSVC_COMPILER_BARRIER();
 *x = v;
#endif
}
#if defined(_M_ARM)
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST void)
_DeeAtomic16_Store_seq_cst(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) {
 _DEE_MSVC_MEMORY_BARRIER();
 __iso_volatile_store16((short volatile *)x, v);
 _DEE_MSVC_MEMORY_BARRIER();
}
#else
#define _DeeAtomic16_Store_seq_cst (void)_DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedExchange16)
#endif
#define _DeeAtomic16_Load_acquire _DeeAtomic16_Load_seq_cst
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T)
_DeeAtomic16_Load_seq_cst(DEE_ATOMIC16_NATIVE_T volatile *x) {
 DEE_ATOMIC16_NATIVE_T result;
#if defined(_M_ARM)
 result = __iso_volatile_load16((short volatile *)x);
 _DEE_MSVC_MEMORY_BARRIER();
#else
 result = *x;
 _DEE_MSVC_COMPILER_BARRIER();
#endif
 return result;
}
#if defined(_M_ARM)
#define _DeeAtomic16_Load_relaxed    __iso_volatile_load16
#else
#define _DeeAtomic16_Load_relaxed(x) *(x)
#endif
_DEE_ATOMIC_INTRIN4(_InterlockedExchange16)
#define _DeeAtomic16_Exchange_seq_cst _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedExchange16)
#define _DeeAtomic16_Exchange_relaxed _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedExchange16)
#define _DeeAtomic16_Exchange_acquire _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedExchange16)
#define _DeeAtomic16_Exchange_release _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedExchange16)
_DEE_ATOMIC_INTRIN4(_InterlockedCompareExchange16)
#define _DeeAtomic16_CompareExchangeVal_seq_cst(x,old_v,new_v) _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedCompareExchange16)(x,old_v,new_v)
#define _DeeAtomic16_CompareExchangeVal_relaxed(x,old_v,new_v) _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedCompareExchange16)(x,old_v,new_v)
#define _DeeAtomic16_CompareExchangeVal_acquire(x,old_v,new_v) _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedCompareExchange16)(x,old_v,new_v)
#define _DeeAtomic16_CompareExchangeVal_release(x,old_v,new_v) _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedCompareExchange16)(x,old_v,new_v)
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic16_CompareExchange_seq_cst(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T old_v, DEE_ATOMIC16_NATIVE_T new_v) { return _DeeAtomic16_CompareExchangeVal_seq_cst(x,old_v,new_v) == old_v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic16_CompareExchange_relaxed(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T old_v, DEE_ATOMIC16_NATIVE_T new_v) { return _DeeAtomic16_CompareExchangeVal_relaxed(x,old_v,new_v) == old_v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic16_CompareExchange_acquire(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T old_v, DEE_ATOMIC16_NATIVE_T new_v) { return _DeeAtomic16_CompareExchangeVal_acquire(x,old_v,new_v) == old_v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic16_CompareExchange_release(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T old_v, DEE_ATOMIC16_NATIVE_T new_v) { return _DeeAtomic16_CompareExchangeVal_release(x,old_v,new_v) == old_v; }
_DEE_ATOMIC_INTRIN4(_InterlockedIncrement16)
#define _DeeAtomic16_IncFetch_seq_cst     _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedIncrement16)
#define _DeeAtomic16_IncFetch_relaxed     _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedIncrement16)
#define _DeeAtomic16_IncFetch_acquire     _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedIncrement16)
#define _DeeAtomic16_IncFetch_release     _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedIncrement16)
_DEE_ATOMIC_INTRIN4(_InterlockedDecrement16)
#define _DeeAtomic16_DecFetch_seq_cst     _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedDecrement16)
#define _DeeAtomic16_DecFetch_relaxed     _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedDecrement16)
#define _DeeAtomic16_DecFetch_acquire     _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedDecrement16)
#define _DeeAtomic16_DecFetch_release     _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedDecrement16)
#define _DeeAtomic16_FetchInc_seq_cst(x) (_DeeAtomic16_IncFetch_seq_cst(x)-1)
#define _DeeAtomic16_FetchInc_relaxed(x) (_DeeAtomic16_IncFetch_relaxed(x)-1)
#define _DeeAtomic16_FetchInc_acquire(x) (_DeeAtomic16_IncFetch_acquire(x)-1)
#define _DeeAtomic16_FetchInc_release(x) (_DeeAtomic16_IncFetch_release(x)-1)
#define _DeeAtomic16_FetchDec_seq_cst(x) (_DeeAtomic16_DecFetch_seq_cst(x)+1)
#define _DeeAtomic16_FetchDec_relaxed(x) (_DeeAtomic16_DecFetch_relaxed(x)+1)
#define _DeeAtomic16_FetchDec_acquire(x) (_DeeAtomic16_DecFetch_acquire(x)+1)
#define _DeeAtomic16_FetchDec_release(x) (_DeeAtomic16_DecFetch_release(x)+1)
_DEE_ATOMIC_INTRIN4(_InterlockedExchangeAdd16)
#define _DeeAtomic16_FetchAdd_seq_cst _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedExchangeAdd16)
#define _DeeAtomic16_FetchAdd_relaxed _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedExchangeAdd16)
#define _DeeAtomic16_FetchAdd_acquire _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedExchangeAdd16)
#define _DeeAtomic16_FetchAdd_release _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedExchangeAdd16)
_DEE_ATOMIC_INTRIN4(_InterlockedAnd16)
#define _DeeAtomic16_FetchAnd_seq_cst _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedAnd16)
#define _DeeAtomic16_FetchAnd_relaxed _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedAnd16)
#define _DeeAtomic16_FetchAnd_acquire _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedAnd16)
#define _DeeAtomic16_FetchAnd_release _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedAnd16)
_DEE_ATOMIC_INTRIN4(_InterlockedOr16)
#define _DeeAtomic16_FetchOr_seq_cst  _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedOr16)
#define _DeeAtomic16_FetchOr_relaxed  _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedOr16)
#define _DeeAtomic16_FetchOr_acquire  _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedOr16)
#define _DeeAtomic16_FetchOr_release  _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedOr16)
_DEE_ATOMIC_INTRIN4(_InterlockedXor16)
#define _DeeAtomic16_FetchXor_seq_cst _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedXor16)
#define _DeeAtomic16_FetchXor_relaxed _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedXor16)
#define _DeeAtomic16_FetchXor_acquire _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedXor16)
#define _DeeAtomic16_FetchXor_release _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedXor16)



#if defined(_M_ARM)
#pragma intrinsic(__iso_volatile_store32)
#define _DeeAtomic32_Store_relaxed      __iso_volatile_store32
#else
#define _DeeAtomic32_Store_relaxed(x,v) (void)(*(x)=(v))
#endif
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST void)
_DeeAtomic32_Store_release(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) {
#if defined(_M_ARM)
 _DEE_MSVC_MEMORY_BARRIER();
 __iso_volatile_store32((volatile long *)x,v);
#else
 _DEE_MSVC_COMPILER_BARRIER();
 *x = v;
#endif
}
#if defined(_M_ARM)
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST void)
_DeeAtomic32_Store_seq_cst(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) {
 _DEE_MSVC_MEMORY_BARRIER();
 __iso_volatile_store32((volatile long *)x, v);
 _DEE_MSVC_MEMORY_BARRIER();
}
#else
#define _DeeAtomic32_Store_seq_cst (void)_DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedExchange)
#endif
#define _DeeAtomic32_Load_acquire _DeeAtomic32_Load_seq_cst
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T)
_DeeAtomic32_Load_seq_cst(DEE_ATOMIC32_NATIVE_T volatile *x) {
 DEE_ATOMIC32_NATIVE_T result;
#if defined(_M_ARM)
 result = __iso_volatile_load32((volatile long *)x);
 _DEE_MSVC_MEMORY_BARRIER();
#else
 result = *x;
 _DEE_MSVC_COMPILER_BARRIER();
#endif
 return result;
}
#if defined(_M_ARM)
#define _DeeAtomic32_Load_relaxed    __iso_volatile_load32
#else
#define _DeeAtomic32_Load_relaxed(x) *(x)
#endif
_DEE_ATOMIC_INTRIN4(_InterlockedExchange)
#define _DeeAtomic32_Exchange_seq_cst  _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedExchange)
#define _DeeAtomic32_Exchange_relaxed  _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedExchange)
#define _DeeAtomic32_Exchange_acquire  _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedExchange)
#define _DeeAtomic32_Exchange_release  _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedExchange)
_DEE_ATOMIC_INTRIN4(_InterlockedCompareExchange)
#define _DeeAtomic32_CompareExchangeVal_seq_cst(x,old_v,new_v) _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedCompareExchange)(x,new_v,old_v)
#define _DeeAtomic32_CompareExchangeVal_relaxed(x,old_v,new_v) _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedCompareExchange)(x,new_v,old_v)
#define _DeeAtomic32_CompareExchangeVal_acquire(x,old_v,new_v) _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedCompareExchange)(x,new_v,old_v)
#define _DeeAtomic32_CompareExchangeVal_release(x,old_v,new_v) _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedCompareExchange)(x,new_v,old_v)
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic32_CompareExchange_seq_cst(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T old_v, DEE_ATOMIC32_NATIVE_T new_v) { return _DeeAtomic32_CompareExchangeVal_seq_cst(x,old_v,new_v) == old_v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic32_CompareExchange_relaxed(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T old_v, DEE_ATOMIC32_NATIVE_T new_v) { return _DeeAtomic32_CompareExchangeVal_relaxed(x,old_v,new_v) == old_v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic32_CompareExchange_acquire(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T old_v, DEE_ATOMIC32_NATIVE_T new_v) { return _DeeAtomic32_CompareExchangeVal_acquire(x,old_v,new_v) == old_v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic32_CompareExchange_release(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T old_v, DEE_ATOMIC32_NATIVE_T new_v) { return _DeeAtomic32_CompareExchangeVal_release(x,old_v,new_v) == old_v; }
_DEE_ATOMIC_INTRIN4(_InterlockedIncrement)
#define _DeeAtomic32_IncFetch_seq_cst     _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedIncrement)
#define _DeeAtomic32_IncFetch_relaxed     _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedIncrement)
#define _DeeAtomic32_IncFetch_acquire     _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedIncrement)
#define _DeeAtomic32_IncFetch_release     _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedIncrement)
_DEE_ATOMIC_INTRIN4(_InterlockedDecrement)
#define _DeeAtomic32_DecFetch_seq_cst     _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedDecrement)
#define _DeeAtomic32_DecFetch_relaxed     _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedDecrement)
#define _DeeAtomic32_DecFetch_acquire     _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedDecrement)
#define _DeeAtomic32_DecFetch_release     _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedDecrement)
#define _DeeAtomic32_FetchInc_seq_cst(x) (_DeeAtomic32_IncFetch_seq_cst(x)-1)
#define _DeeAtomic32_FetchInc_relaxed(x) (_DeeAtomic32_IncFetch_relaxed(x)-1)
#define _DeeAtomic32_FetchInc_acquire(x) (_DeeAtomic32_IncFetch_acquire(x)-1)
#define _DeeAtomic32_FetchInc_release(x) (_DeeAtomic32_IncFetch_release(x)-1)
#define _DeeAtomic32_FetchDec_seq_cst(x) (_DeeAtomic32_DecFetch_seq_cst(x)+1)
#define _DeeAtomic32_FetchDec_relaxed(x) (_DeeAtomic32_DecFetch_relaxed(x)+1)
#define _DeeAtomic32_FetchDec_acquire(x) (_DeeAtomic32_DecFetch_acquire(x)+1)
#define _DeeAtomic32_FetchDec_release(x) (_DeeAtomic32_DecFetch_release(x)+1)
_DEE_ATOMIC_INTRIN4(_InterlockedExchangeAdd)
#define _DeeAtomic32_FetchAdd_seq_cst _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedExchangeAdd)
#define _DeeAtomic32_FetchAdd_relaxed _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedExchangeAdd)
#define _DeeAtomic32_FetchAdd_acquire _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedExchangeAdd)
#define _DeeAtomic32_FetchAdd_release _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedExchangeAdd)
_DEE_ATOMIC_INTRIN4(_InterlockedAnd)
#define _DeeAtomic32_FetchAnd_seq_cst _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedAnd)
#define _DeeAtomic32_FetchAnd_relaxed _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedAnd)
#define _DeeAtomic32_FetchAnd_acquire _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedAnd)
#define _DeeAtomic32_FetchAnd_release _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedAnd)
_DEE_ATOMIC_INTRIN4(_InterlockedOr)
#define _DeeAtomic32_FetchOr_seq_cst  _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedOr)
#define _DeeAtomic32_FetchOr_relaxed  _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedOr)
#define _DeeAtomic32_FetchOr_acquire  _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedOr)
#define _DeeAtomic32_FetchOr_release  _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedOr)
_DEE_ATOMIC_INTRIN4(_InterlockedXor)
#define _DeeAtomic32_FetchXor_seq_cst _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedXor)
#define _DeeAtomic32_FetchXor_relaxed _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedXor)
#define _DeeAtomic32_FetchXor_acquire _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedXor)
#define _DeeAtomic32_FetchXor_release _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedXor)


#if defined(_M_IX86)
#define _DeeAtomic64_Exchange_relaxed _DeeAtomic64_Exchange_seq_cst
#define _DeeAtomic64_Exchange_acquire _DeeAtomic64_Exchange_seq_cst
#define _DeeAtomic64_Exchange_release _DeeAtomic64_Exchange_seq_cst
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T)
_DeeAtomic64_Exchange_seq_cst(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) {
 _DEE_MSVC_COMPILER_BARRIER();
 __asm {
  mov esi, x;
  mov ecx, dword ptr v[4];
  mov ebx, dword ptr v;
again:
  lock cmpxchg8b [esi];
  jnz again;
  mov dword ptr v[4], edx;
  mov dword ptr v, eax;
 }
 _DEE_MSVC_COMPILER_BARRIER();
 return v;
}

#define _DeeAtomic64_FetchAdd_relaxed _DeeAtomic64_FetchAdd_seq_cst
#define _DeeAtomic64_FetchAdd_acquire _DeeAtomic64_FetchAdd_seq_cst
#define _DeeAtomic64_FetchAdd_release _DeeAtomic64_FetchAdd_seq_cst
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T)
_DeeAtomic64_FetchAdd_seq_cst(volatile DEE_ATOMIC64_NATIVE_T * x, DEE_ATOMIC64_NATIVE_T v) {
 _DEE_MSVC_COMPILER_BARRIER();
 __asm {
  mov esi, x;
  mov edx, 4[esi];
  mov eax, [esi];
 again:
  mov ecx, edx;
  mov ebx, eax;
  add ebx, dword ptr v;
  adc ecx, dword ptr v[4];
  lock cmpxchg8b [esi];
  jnz again;
  mov dword ptr v, eax;
  mov dword ptr v[4], edx;
 }
 _DEE_MSVC_COMPILER_BARRIER();
 return v;
}
#define _DeeAtomic64_FetchAnd_relaxed _DeeAtomic64_FetchAnd_seq_cst
#define _DeeAtomic64_FetchAnd_acquire _DeeAtomic64_FetchAnd_seq_cst
#define _DeeAtomic64_FetchAnd_release _DeeAtomic64_FetchAnd_seq_cst
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T)
_DeeAtomic64_FetchAnd_seq_cst(volatile DEE_ATOMIC64_NATIVE_T *x, DEE_ATOMIC64_NATIVE_T v) {
 _DEE_MSVC_COMPILER_BARRIER();
 __asm {
  mov esi, x;
  mov eax, [esi];
  mov edx, 4[esi];
again:
  mov ecx, edx;
  mov ebx, eax;
  and ebx, dword ptr v;
  and ecx, dword ptr v[4];
  lock cmpxchg8b [esi];
  jnz again;
  mov dword ptr v, eax;
  mov dword ptr v[4], edx;
 }
 _DEE_MSVC_COMPILER_BARRIER();
 return v;
}
#define _DeeAtomic64_FetchOr_relaxed _DeeAtomic64_FetchOr_seq_cst
#define _DeeAtomic64_FetchOr_acquire _DeeAtomic64_FetchOr_seq_cst
#define _DeeAtomic64_FetchOr_release _DeeAtomic64_FetchOr_seq_cst
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T)
_DeeAtomic64_FetchOr_seq_cst(volatile DEE_ATOMIC64_NATIVE_T *x, DEE_ATOMIC64_NATIVE_T v) {
 _DEE_MSVC_COMPILER_BARRIER();
 __asm {
  mov esi, x;
  mov eax, [esi];
  mov edx, 4[esi];
again:
  mov ecx, edx;
  mov ebx, eax;
  or ebx, dword ptr v;
  or ecx, dword ptr v[4];
  lock cmpxchg8b [esi];
  jnz again;
  mov dword ptr v, eax;
  mov dword ptr v[4], edx;
 }
 _DEE_MSVC_COMPILER_BARRIER();
 return v;
}

#define _DeeAtomic64_FetchXor_relaxed _DeeAtomic64_FetchXor_seq_cst
#define _DeeAtomic64_FetchXor_acquire _DeeAtomic64_FetchXor_seq_cst
#define _DeeAtomic64_FetchXor_release _DeeAtomic64_FetchXor_seq_cst
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T)
_DeeAtomic64_FetchXor_seq_cst(volatile DEE_ATOMIC64_NATIVE_T *x, DEE_ATOMIC64_NATIVE_T v) {
 _DEE_MSVC_COMPILER_BARRIER();
 __asm {
  mov esi, x;
  mov eax, [esi];
  mov edx, 4[esi];
again:
  mov ecx, edx;
  mov ebx, eax;
  xor ebx, dword ptr v;
  xor ecx, dword ptr v[4];
  lock cmpxchg8b [esi];
  jnz again;
  mov dword ptr v, eax;
  mov dword ptr v[4], edx;
 }
 _DEE_MSVC_COMPILER_BARRIER();
 return v;
}
#else
_DEE_ATOMIC_INTRIN4(_InterlockedExchange64)
#define _DeeAtomic64_Exchange_seq_cst _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedExchange64)
#define _DeeAtomic64_Exchange_relaxed _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedExchange64)
#define _DeeAtomic64_Exchange_acquire _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedExchange64)
#define _DeeAtomic64_Exchange_release _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedExchange64)
_DEE_ATOMIC_INTRIN4(_InterlockedExchangeAdd64)
#define _DeeAtomic64_FetchAdd_seq_cst _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedExchangeAdd64)
#define _DeeAtomic64_FetchAdd_relaxed _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedExchangeAdd64)
#define _DeeAtomic64_FetchAdd_acquire _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedExchangeAdd64)
#define _DeeAtomic64_FetchAdd_release _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedExchangeAdd64)
_DEE_ATOMIC_INTRIN4(_InterlockedAnd64)
#define _DeeAtomic64_FetchAnd_seq_cst _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedAnd64)
#define _DeeAtomic64_FetchAnd_relaxed _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedAnd64)
#define _DeeAtomic64_FetchAnd_acquire _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedAnd64)
#define _DeeAtomic64_FetchAnd_release _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedAnd64)
_DEE_ATOMIC_INTRIN4(_InterlockedOr64)
#define _DeeAtomic64_FetchOr_seq_cst  _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedOr64)
#define _DeeAtomic64_FetchOr_relaxed  _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedOr64)
#define _DeeAtomic64_FetchOr_acquire  _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedOr64)
#define _DeeAtomic64_FetchOr_release  _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedOr64)
_DEE_ATOMIC_INTRIN4(_InterlockedXor64)
#define _DeeAtomic64_FetchXor_seq_cst _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedXor64)
#define _DeeAtomic64_FetchXor_relaxed _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedXor64)
#define _DeeAtomic64_FetchXor_acquire _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedXor64)
#define _DeeAtomic64_FetchXor_release _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedXor64)
#endif

#if defined(_M_X64) || defined(_LP64) || defined(__x86_64)
#if !defined(_GENERIC_ATOMICS) || !_GENERIC_ATOMICS
#define DEE_ATOMIC_MS_64	1
#endif /* !_GENERIC_ATOMICS */
#endif
#ifndef DEE_ATOMIC_MS_64
#define DEE_ATOMIC_MS_64 0
#endif

#if DEE_ATOMIC_MS_64
#define _DeeAtomic64_Store_relaxed(x,v) (void)(*(x)=(v))
#else
#define _DeeAtomic64_Store_relaxed      (void)_DeeAtomic64_Exchange_relaxed
#endif
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST void)
_DeeAtomic64_Store_release(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) {
#if DEE_ATOMIC_MS_64
 _DEE_MSVC_COMPILER_BARRIER();
 *x = v;
#else
 _DeeAtomic64_Exchange_release(x,v);
#endif
}
#define _DeeAtomic64_Store_seq_cst (void)_DeeAtomic64_Exchange_seq_cst
#define _DeeAtomic64_Load_acquire        _DeeAtomic64_Load_seq_cst
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T)
_DeeAtomic64_Load_seq_cst(DEE_ATOMIC64_NATIVE_T volatile *x) {
 DEE_ATOMIC64_NATIVE_T result;
#if DEE_ATOMIC_MS_64
 result = *x;
 _DEE_MSVC_COMPILER_BARRIER();
#elif defined(_M_ARM)
 result = __ldrexd((long volatile long *)x);
 _DEE_MSVC_MEMORY_BARRIER();
#else
 result = _DeeAtomic64_FetchOr_seq_cst(x,0);
#endif
 return result;
}
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T)
_DeeAtomic64_Load_relaxed(DEE_ATOMIC64_NATIVE_T volatile *x) {
 DEE_ATOMIC64_NATIVE_T result;
#if DEE_ATOMIC_MS_64
 result = *x;
#elif defined(_M_ARM)
 result = __ldrexd((volatile long long *)_Tgt);
#else
 result = _DeeAtomic64_FetchOr_relaxed(x,0);
#endif
 return result;
}
#undef DEE_ATOMIC_MS_64
_DEE_ATOMIC_INTRIN4(_InterlockedCompareExchange64)
#define _DeeAtomic64_CompareExchangeVal_seq_cst(x,old_v,new_v) _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedCompareExchange64)(x,new_v,old_v)
#define _DeeAtomic64_CompareExchangeVal_relaxed(x,old_v,new_v) _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedCompareExchange64)(x,new_v,old_v)
#define _DeeAtomic64_CompareExchangeVal_acquire(x,old_v,new_v) _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedCompareExchange64)(x,new_v,old_v)
#define _DeeAtomic64_CompareExchangeVal_release(x,old_v,new_v) _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedCompareExchange64)(x,new_v,old_v)
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic64_CompareExchange_seq_cst(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T old_v, DEE_ATOMIC64_NATIVE_T new_v) { return _DeeAtomic64_CompareExchangeVal_seq_cst(x,old_v,new_v) == old_v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic64_CompareExchange_relaxed(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T old_v, DEE_ATOMIC64_NATIVE_T new_v) { return _DeeAtomic64_CompareExchangeVal_relaxed(x,old_v,new_v) == old_v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic64_CompareExchange_acquire(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T old_v, DEE_ATOMIC64_NATIVE_T new_v) { return _DeeAtomic64_CompareExchangeVal_acquire(x,old_v,new_v) == old_v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic64_CompareExchange_release(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T old_v, DEE_ATOMIC64_NATIVE_T new_v) { return _DeeAtomic64_CompareExchangeVal_release(x,old_v,new_v) == old_v; }
#if defined(_M_IX86)
#define _DeeAtomic64_IncFetch_seq_cst(x)  (_DeeAtomic64_FetchAdd_seq_cst(x,1)+1)
#define _DeeAtomic64_IncFetch_relaxed(x)  (_DeeAtomic64_FetchAdd_relaxed(x,1)+1)
#define _DeeAtomic64_IncFetch_acquire(x)  (_DeeAtomic64_FetchAdd_acquire(x,1)+1)
#define _DeeAtomic64_IncFetch_release(x)  (_DeeAtomic64_FetchAdd_release(x,1)+1)
#define _DeeAtomic64_DecFetch_seq_cst(x)  (_DeeAtomic64_FetchAdd_seq_cst(x,-1)-1)
#define _DeeAtomic64_DecFetch_relaxed(x)  (_DeeAtomic64_FetchAdd_relaxed(x,-1)-1)
#define _DeeAtomic64_DecFetch_acquire(x)  (_DeeAtomic64_FetchAdd_acquire(x,-1)-1)
#define _DeeAtomic64_DecFetch_release(x)  (_DeeAtomic64_FetchAdd_release(x,-1)-1)
#else
#define _DeeAtomic64_IncFetch_seq_cst     _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedIncrement64)
#define _DeeAtomic64_IncFetch_relaxed     _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedIncrement64)
#define _DeeAtomic64_IncFetch_acquire     _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedIncrement64)
#define _DeeAtomic64_IncFetch_release     _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedIncrement64)
#define _DeeAtomic64_DecFetch_seq_cst     _DEE_ATOMIC_INTRIN_SEQ_CST(_InterlockedDecrement64)
#define _DeeAtomic64_DecFetch_relaxed     _DEE_ATOMIC_INTRIN_RELAXED(_InterlockedDecrement64)
#define _DeeAtomic64_DecFetch_acquire     _DEE_ATOMIC_INTRIN_ACQUIRE(_InterlockedDecrement64)
#define _DeeAtomic64_DecFetch_release     _DEE_ATOMIC_INTRIN_RELEASE(_InterlockedDecrement64)
#endif
#define _DeeAtomic64_FetchInc_seq_cst(x) (_DeeAtomic64_IncFetch_seq_cst(x)-1)
#define _DeeAtomic64_FetchInc_relaxed(x) (_DeeAtomic64_IncFetch_relaxed(x)-1)
#define _DeeAtomic64_FetchInc_acquire(x) (_DeeAtomic64_IncFetch_acquire(x)-1)
#define _DeeAtomic64_FetchInc_release(x) (_DeeAtomic64_IncFetch_release(x)-1)
#define _DeeAtomic64_FetchDec_seq_cst(x) (_DeeAtomic64_DecFetch_seq_cst(x)+1)
#define _DeeAtomic64_FetchDec_relaxed(x) (_DeeAtomic64_DecFetch_relaxed(x)+1)
#define _DeeAtomic64_FetchDec_acquire(x) (_DeeAtomic64_DecFetch_acquire(x)+1)
#define _DeeAtomic64_FetchDec_release(x) (_DeeAtomic64_DecFetch_release(x)+1)

#define _DeeAtomic8_Load_consume               _DeeAtomic8_Load_seq_cst
#define _DeeAtomic8_Exchange_acq_rel           _DeeAtomic8_Exchange_seq_cst
//#define _DeeAtomic8_Exchange_consume         _DeeAtomic8_Exchange_acquire
#define _DeeAtomic8_CompareExchangeVal_acq_rel _DeeAtomic8_CompareExchangeVal_seq_cst
#define _DeeAtomic8_CompareExchangeVal_consume _DeeAtomic8_CompareExchangeVal_acquire
#define _DeeAtomic8_CompareExchange_acq_rel    _DeeAtomic8_CompareExchange_seq_cst
#define _DeeAtomic8_CompareExchange_consume    _DeeAtomic8_CompareExchange_acquire
#define _DeeAtomic8_FetchAdd_acq_rel           _DeeAtomic8_FetchAdd_seq_cst
#define _DeeAtomic8_FetchAdd_consume           _DeeAtomic8_FetchAdd_acquire
#define _DeeAtomic8_FetchAnd_acq_rel           _DeeAtomic8_FetchAnd_seq_cst
#define _DeeAtomic8_FetchAnd_consume           _DeeAtomic8_FetchAnd_acquire
#define _DeeAtomic8_FetchOr_acq_rel            _DeeAtomic8_FetchOr_seq_cst
#define _DeeAtomic8_FetchOr_consume            _DeeAtomic8_FetchOr_acquire
#define _DeeAtomic8_FetchXor_acq_rel           _DeeAtomic8_FetchXor_seq_cst
#define _DeeAtomic8_FetchXor_consume           _DeeAtomic8_FetchXor_acquire

#define _DeeAtomic16_Load_consume               _DeeAtomic16_Load_seq_cst
#define _DeeAtomic16_Exchange_acq_rel           _DeeAtomic16_Exchange_seq_cst
//#define _DeeAtomic16_Exchange_consume         _DeeAtomic16_Exchange_acquire
#define _DeeAtomic16_CompareExchangeVal_acq_rel _DeeAtomic16_CompareExchangeVal_seq_cst
#define _DeeAtomic16_CompareExchangeVal_consume _DeeAtomic16_CompareExchangeVal_acquire
#define _DeeAtomic16_CompareExchange_acq_rel    _DeeAtomic16_CompareExchange_seq_cst
#define _DeeAtomic16_CompareExchange_consume    _DeeAtomic16_CompareExchange_acquire
#define _DeeAtomic16_FetchAdd_acq_rel           _DeeAtomic16_FetchAdd_seq_cst
#define _DeeAtomic16_FetchAdd_consume           _DeeAtomic16_FetchAdd_acquire
#define _DeeAtomic16_FetchAnd_acq_rel           _DeeAtomic16_FetchAnd_seq_cst
#define _DeeAtomic16_FetchAnd_consume           _DeeAtomic16_FetchAnd_acquire
#define _DeeAtomic16_FetchOr_acq_rel            _DeeAtomic16_FetchOr_seq_cst
#define _DeeAtomic16_FetchOr_consume            _DeeAtomic16_FetchOr_acquire
#define _DeeAtomic16_FetchXor_acq_rel           _DeeAtomic16_FetchXor_seq_cst
#define _DeeAtomic16_FetchXor_consume           _DeeAtomic16_FetchXor_acquire

#define _DeeAtomic32_Load_consume               _DeeAtomic32_Load_seq_cst
#define _DeeAtomic32_Exchange_acq_rel           _DeeAtomic32_Exchange_seq_cst
//#define _DeeAtomic32_Exchange_consume         _DeeAtomic32_Exchange_acquire
#define _DeeAtomic32_CompareExchangeVal_acq_rel _DeeAtomic32_CompareExchangeVal_seq_cst
#define _DeeAtomic32_CompareExchangeVal_consume _DeeAtomic32_CompareExchangeVal_acquire
#define _DeeAtomic32_CompareExchange_acq_rel    _DeeAtomic32_CompareExchange_seq_cst
#define _DeeAtomic32_CompareExchange_consume    _DeeAtomic32_CompareExchange_acquire
#define _DeeAtomic32_FetchAdd_acq_rel           _DeeAtomic32_FetchAdd_seq_cst
#define _DeeAtomic32_FetchAdd_consume           _DeeAtomic32_FetchAdd_acquire
#define _DeeAtomic32_FetchAnd_acq_rel           _DeeAtomic32_FetchAnd_seq_cst
#define _DeeAtomic32_FetchAnd_consume           _DeeAtomic32_FetchAnd_acquire
#define _DeeAtomic32_FetchOr_acq_rel            _DeeAtomic32_FetchOr_seq_cst
#define _DeeAtomic32_FetchOr_consume            _DeeAtomic32_FetchOr_acquire
#define _DeeAtomic32_FetchXor_acq_rel           _DeeAtomic32_FetchXor_seq_cst
#define _DeeAtomic32_FetchXor_consume           _DeeAtomic32_FetchXor_acquire

#define _DeeAtomic64_Load_consume               _DeeAtomic64_Load_seq_cst
#define _DeeAtomic64_Exchange_acq_rel           _DeeAtomic64_Exchange_seq_cst
//#define _DeeAtomic64_Exchange_consume         _DeeAtomic64_Exchange_acquire
#define _DeeAtomic64_CompareExchangeVal_acq_rel _DeeAtomic64_CompareExchangeVal_seq_cst
#define _DeeAtomic64_CompareExchangeVal_consume _DeeAtomic64_CompareExchangeVal_acquire
#define _DeeAtomic64_CompareExchange_acq_rel    _DeeAtomic64_CompareExchange_seq_cst
#define _DeeAtomic64_CompareExchange_consume    _DeeAtomic64_CompareExchange_acquire
#define _DeeAtomic64_FetchAdd_acq_rel           _DeeAtomic64_FetchAdd_seq_cst
#define _DeeAtomic64_FetchAdd_consume           _DeeAtomic64_FetchAdd_acquire
#define _DeeAtomic64_FetchAnd_acq_rel           _DeeAtomic64_FetchAnd_seq_cst
#define _DeeAtomic64_FetchAnd_consume           _DeeAtomic64_FetchAnd_acquire
#define _DeeAtomic64_FetchOr_acq_rel            _DeeAtomic64_FetchOr_seq_cst
#define _DeeAtomic64_FetchOr_consume            _DeeAtomic64_FetchOr_acquire
#define _DeeAtomic64_FetchXor_acq_rel           _DeeAtomic64_FetchXor_seq_cst
#define _DeeAtomic64_FetchXor_consume           _DeeAtomic64_FetchXor_acquire

DEE_DECL_END

#undef _DEE_ATOMIC_INTRIN4
#undef _DEE_MSVC_COMPILER_BARRIER
#ifdef _DEE_MSVC_MEMORY_BARRIER
#undef _DEE_MSVC_MEMORY_BARRIER
#endif /* _DEE_MSVC_MEMORY_BARRIER */
#endif /* DEE_PLATFORM_WINDOWS */

DEE_DECL_BEGIN

#define _DeeAtomic8_FetchNand_acq_rel   _DeeAtomic8_FetchNand_seq_cst
#define _DeeAtomic8_FetchNand_consume   _DeeAtomic8_FetchNand_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_FetchNand_seq_cst(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { DEE_ATOMIC8_NATIVE_T temp;do{temp=*x;}while(!_DeeAtomic8_CompareExchange_seq_cst(x,temp,~(temp&v))); return temp; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_FetchNand_relaxed(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { DEE_ATOMIC8_NATIVE_T temp;do{temp=*x;}while(!_DeeAtomic8_CompareExchange_relaxed(x,temp,~(temp&v))); return temp; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_FetchNand_acquire(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { DEE_ATOMIC8_NATIVE_T temp;do{temp=*x;}while(!_DeeAtomic8_CompareExchange_acquire(x,temp,~(temp&v))); return temp; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_FetchNand_release(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { DEE_ATOMIC8_NATIVE_T temp;do{temp=*x;}while(!_DeeAtomic8_CompareExchange_release(x,temp,~(temp&v))); return temp; }
#define _DeeAtomic16_FetchNand_acq_rel   _DeeAtomic16_FetchNand_seq_cst
#define _DeeAtomic16_FetchNand_consume   _DeeAtomic16_FetchNand_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_FetchNand_seq_cst(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { DEE_ATOMIC16_NATIVE_T temp;do{temp=*x;}while(!_DeeAtomic16_CompareExchange_seq_cst(x,temp,~(temp&v))); return temp; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_FetchNand_relaxed(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { DEE_ATOMIC16_NATIVE_T temp;do{temp=*x;}while(!_DeeAtomic16_CompareExchange_relaxed(x,temp,~(temp&v))); return temp; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_FetchNand_acquire(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { DEE_ATOMIC16_NATIVE_T temp;do{temp=*x;}while(!_DeeAtomic16_CompareExchange_acquire(x,temp,~(temp&v))); return temp; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_FetchNand_release(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { DEE_ATOMIC16_NATIVE_T temp;do{temp=*x;}while(!_DeeAtomic16_CompareExchange_release(x,temp,~(temp&v))); return temp; }
#define _DeeAtomic32_FetchNand_acq_rel   _DeeAtomic32_FetchNand_seq_cst
#define _DeeAtomic32_FetchNand_consume   _DeeAtomic32_FetchNand_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_FetchNand_seq_cst(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { DEE_ATOMIC32_NATIVE_T temp;do{temp=*x;}while(!_DeeAtomic32_CompareExchange_seq_cst(x,temp,~(temp&v))); return temp; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_FetchNand_relaxed(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { DEE_ATOMIC32_NATIVE_T temp;do{temp=*x;}while(!_DeeAtomic32_CompareExchange_relaxed(x,temp,~(temp&v))); return temp; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_FetchNand_acquire(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { DEE_ATOMIC32_NATIVE_T temp;do{temp=*x;}while(!_DeeAtomic32_CompareExchange_acquire(x,temp,~(temp&v))); return temp; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_FetchNand_release(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { DEE_ATOMIC32_NATIVE_T temp;do{temp=*x;}while(!_DeeAtomic32_CompareExchange_release(x,temp,~(temp&v))); return temp; }
#define _DeeAtomic64_FetchNand_acq_rel   _DeeAtomic64_FetchNand_seq_cst
#define _DeeAtomic64_FetchNand_consume   _DeeAtomic64_FetchNand_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_FetchNand_seq_cst(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { DEE_ATOMIC64_NATIVE_T temp;do{temp=*x;}while(!_DeeAtomic64_CompareExchange_seq_cst(x,temp,~(temp&v))); return temp; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_FetchNand_relaxed(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { DEE_ATOMIC64_NATIVE_T temp;do{temp=*x;}while(!_DeeAtomic64_CompareExchange_relaxed(x,temp,~(temp&v))); return temp; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_FetchNand_acquire(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { DEE_ATOMIC64_NATIVE_T temp;do{temp=*x;}while(!_DeeAtomic64_CompareExchange_acquire(x,temp,~(temp&v))); return temp; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_FetchNand_release(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { DEE_ATOMIC64_NATIVE_T temp;do{temp=*x;}while(!_DeeAtomic64_CompareExchange_release(x,temp,~(temp&v))); return temp; }


#define _DeeAtomic8_AddFetch_acq_rel    _DeeAtomic8_AddFetch_seq_cst
#define _DeeAtomic8_AddFetch_consume    _DeeAtomic8_AddFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_AddFetch_seq_cst(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return _DeeAtomic8_FetchAdd_seq_cst(x,v)+v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_AddFetch_relaxed(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return _DeeAtomic8_FetchAdd_relaxed(x,v)+v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_AddFetch_acquire(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return _DeeAtomic8_FetchAdd_acquire(x,v)+v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_AddFetch_release(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return _DeeAtomic8_FetchAdd_release(x,v)+v; }
#define _DeeAtomic8_AndFetch_acq_rel    _DeeAtomic8_AndFetch_seq_cst
#define _DeeAtomic8_AndFetch_consume    _DeeAtomic8_AndFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_AndFetch_seq_cst(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return _DeeAtomic8_FetchAnd_seq_cst(x,v)&v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_AndFetch_relaxed(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return _DeeAtomic8_FetchAnd_relaxed(x,v)&v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_AndFetch_acquire(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return _DeeAtomic8_FetchAnd_acquire(x,v)&v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_AndFetch_release(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return _DeeAtomic8_FetchAnd_release(x,v)&v; }
#define _DeeAtomic8_OrFetch_acq_rel     _DeeAtomic8_OrFetch_seq_cst
#define _DeeAtomic8_OrFetch_consume     _DeeAtomic8_OrFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_OrFetch_seq_cst(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return _DeeAtomic8_FetchOr_seq_cst(x,v)|v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_OrFetch_relaxed(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return _DeeAtomic8_FetchOr_relaxed(x,v)|v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_OrFetch_acquire(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return _DeeAtomic8_FetchOr_acquire(x,v)|v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_OrFetch_release(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return _DeeAtomic8_FetchOr_release(x,v)|v; }
#define _DeeAtomic8_XorFetch_acq_rel    _DeeAtomic8_XorFetch_seq_cst
#define _DeeAtomic8_XorFetch_consume    _DeeAtomic8_XorFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_XorFetch_seq_cst(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return _DeeAtomic8_FetchXor_seq_cst(x,v)^v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_XorFetch_relaxed(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return _DeeAtomic8_FetchXor_relaxed(x,v)^v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_XorFetch_acquire(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return _DeeAtomic8_FetchXor_acquire(x,v)^v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_XorFetch_release(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return _DeeAtomic8_FetchXor_release(x,v)^v; }
#define _DeeAtomic8_NandFetch_acq_rel   _DeeAtomic8_NandFetch_seq_cst
#define _DeeAtomic8_NandFetch_consume   _DeeAtomic8_NandFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_NandFetch_seq_cst(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return ~(_DeeAtomic8_FetchNand_seq_cst(x,v)&v); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_NandFetch_relaxed(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return ~(_DeeAtomic8_FetchNand_relaxed(x,v)&v); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_NandFetch_acquire(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return ~(_DeeAtomic8_FetchNand_acquire(x,v)&v); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC8_NATIVE_T) _DeeAtomic8_NandFetch_release(DEE_ATOMIC8_NATIVE_T volatile *x, DEE_ATOMIC8_NATIVE_T v) { return ~(_DeeAtomic8_FetchNand_release(x,v)&v); }

#define _DeeAtomic16_AddFetch_acq_rel    _DeeAtomic16_AddFetch_seq_cst
#define _DeeAtomic16_AddFetch_consume    _DeeAtomic16_AddFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_AddFetch_seq_cst(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return _DeeAtomic16_FetchAdd_seq_cst(x,v)+v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_AddFetch_relaxed(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return _DeeAtomic16_FetchAdd_relaxed(x,v)+v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_AddFetch_acquire(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return _DeeAtomic16_FetchAdd_acquire(x,v)+v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_AddFetch_release(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return _DeeAtomic16_FetchAdd_release(x,v)+v; }
#define _DeeAtomic16_AndFetch_acq_rel    _DeeAtomic16_AndFetch_seq_cst
#define _DeeAtomic16_AndFetch_consume    _DeeAtomic16_AndFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_AndFetch_seq_cst(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return _DeeAtomic16_FetchAnd_seq_cst(x,v)&v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_AndFetch_relaxed(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return _DeeAtomic16_FetchAnd_relaxed(x,v)&v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_AndFetch_acquire(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return _DeeAtomic16_FetchAnd_acquire(x,v)&v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_AndFetch_release(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return _DeeAtomic16_FetchAnd_release(x,v)&v; }
#define _DeeAtomic16_OrFetch_acq_rel     _DeeAtomic16_OrFetch_seq_cst
#define _DeeAtomic16_OrFetch_consume     _DeeAtomic16_OrFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_OrFetch_seq_cst(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return _DeeAtomic16_FetchOr_seq_cst(x,v)|v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_OrFetch_relaxed(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return _DeeAtomic16_FetchOr_relaxed(x,v)|v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_OrFetch_acquire(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return _DeeAtomic16_FetchOr_acquire(x,v)|v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_OrFetch_release(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return _DeeAtomic16_FetchOr_release(x,v)|v; }
#define _DeeAtomic16_XorFetch_acq_rel    _DeeAtomic16_XorFetch_seq_cst
#define _DeeAtomic16_XorFetch_consume    _DeeAtomic16_XorFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_XorFetch_seq_cst(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return _DeeAtomic16_FetchXor_seq_cst(x,v)^v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_XorFetch_relaxed(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return _DeeAtomic16_FetchXor_relaxed(x,v)^v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_XorFetch_acquire(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return _DeeAtomic16_FetchXor_acquire(x,v)^v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_XorFetch_release(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return _DeeAtomic16_FetchXor_release(x,v)^v; }
#define _DeeAtomic16_NandFetch_acq_rel   _DeeAtomic16_NandFetch_seq_cst
#define _DeeAtomic16_NandFetch_consume   _DeeAtomic16_NandFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_NandFetch_seq_cst(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return ~(_DeeAtomic16_FetchNand_seq_cst(x,v)&v); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_NandFetch_relaxed(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return ~(_DeeAtomic16_FetchNand_relaxed(x,v)&v); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_NandFetch_acquire(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return ~(_DeeAtomic16_FetchNand_acquire(x,v)&v); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC16_NATIVE_T) _DeeAtomic16_NandFetch_release(DEE_ATOMIC16_NATIVE_T volatile *x, DEE_ATOMIC16_NATIVE_T v) { return ~(_DeeAtomic16_FetchNand_release(x,v)&v); }

#define _DeeAtomic32_AddFetch_acq_rel    _DeeAtomic32_AddFetch_seq_cst
#define _DeeAtomic32_AddFetch_consume    _DeeAtomic32_AddFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_AddFetch_seq_cst(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return _DeeAtomic32_FetchAdd_seq_cst(x,v)+v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_AddFetch_relaxed(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return _DeeAtomic32_FetchAdd_relaxed(x,v)+v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_AddFetch_acquire(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return _DeeAtomic32_FetchAdd_acquire(x,v)+v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_AddFetch_release(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return _DeeAtomic32_FetchAdd_release(x,v)+v; }
#define _DeeAtomic32_AndFetch_acq_rel    _DeeAtomic32_AndFetch_seq_cst
#define _DeeAtomic32_AndFetch_consume    _DeeAtomic32_AndFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_AndFetch_seq_cst(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return _DeeAtomic32_FetchAnd_seq_cst(x,v)&v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_AndFetch_relaxed(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return _DeeAtomic32_FetchAnd_relaxed(x,v)&v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_AndFetch_acquire(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return _DeeAtomic32_FetchAnd_acquire(x,v)&v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_AndFetch_release(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return _DeeAtomic32_FetchAnd_release(x,v)&v; }
#define _DeeAtomic32_OrFetch_acq_rel     _DeeAtomic32_OrFetch_seq_cst
#define _DeeAtomic32_OrFetch_consume     _DeeAtomic32_OrFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_OrFetch_seq_cst(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return _DeeAtomic32_FetchOr_seq_cst(x,v)|v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_OrFetch_relaxed(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return _DeeAtomic32_FetchOr_relaxed(x,v)|v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_OrFetch_acquire(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return _DeeAtomic32_FetchOr_acquire(x,v)|v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_OrFetch_release(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return _DeeAtomic32_FetchOr_release(x,v)|v; }
#define _DeeAtomic32_XorFetch_acq_rel    _DeeAtomic32_XorFetch_seq_cst
#define _DeeAtomic32_XorFetch_consume    _DeeAtomic32_XorFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_XorFetch_seq_cst(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return _DeeAtomic32_FetchXor_seq_cst(x,v)^v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_XorFetch_relaxed(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return _DeeAtomic32_FetchXor_relaxed(x,v)^v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_XorFetch_acquire(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return _DeeAtomic32_FetchXor_acquire(x,v)^v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_XorFetch_release(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return _DeeAtomic32_FetchXor_release(x,v)^v; }
#define _DeeAtomic32_NandFetch_acq_rel   _DeeAtomic32_NandFetch_seq_cst
#define _DeeAtomic32_NandFetch_consume   _DeeAtomic32_NandFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_NandFetch_seq_cst(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return ~(_DeeAtomic32_FetchNand_seq_cst(x,v)&v); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_NandFetch_relaxed(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return ~(_DeeAtomic32_FetchNand_relaxed(x,v)&v); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_NandFetch_acquire(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return ~(_DeeAtomic32_FetchNand_acquire(x,v)&v); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC32_NATIVE_T) _DeeAtomic32_NandFetch_release(DEE_ATOMIC32_NATIVE_T volatile *x, DEE_ATOMIC32_NATIVE_T v) { return ~(_DeeAtomic32_FetchNand_release(x,v)&v); }

#define _DeeAtomic64_AddFetch_acq_rel    _DeeAtomic64_AddFetch_seq_cst
#define _DeeAtomic64_AddFetch_consume    _DeeAtomic64_AddFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_AddFetch_seq_cst(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return _DeeAtomic64_FetchAdd_seq_cst(x,v)+v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_AddFetch_relaxed(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return _DeeAtomic64_FetchAdd_relaxed(x,v)+v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_AddFetch_acquire(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return _DeeAtomic64_FetchAdd_acquire(x,v)+v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_AddFetch_release(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return _DeeAtomic64_FetchAdd_release(x,v)+v; }
#define _DeeAtomic64_AndFetch_acq_rel    _DeeAtomic64_AndFetch_seq_cst
#define _DeeAtomic64_AndFetch_consume    _DeeAtomic64_AndFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_AndFetch_seq_cst(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return _DeeAtomic64_FetchAnd_seq_cst(x,v)&v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_AndFetch_relaxed(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return _DeeAtomic64_FetchAnd_relaxed(x,v)&v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_AndFetch_acquire(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return _DeeAtomic64_FetchAnd_acquire(x,v)&v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_AndFetch_release(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return _DeeAtomic64_FetchAnd_release(x,v)&v; }
#define _DeeAtomic64_OrFetch_acq_rel     _DeeAtomic64_OrFetch_seq_cst
#define _DeeAtomic64_OrFetch_consume     _DeeAtomic64_OrFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_OrFetch_seq_cst(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return _DeeAtomic64_FetchOr_seq_cst(x,v)|v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_OrFetch_relaxed(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return _DeeAtomic64_FetchOr_relaxed(x,v)|v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_OrFetch_acquire(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return _DeeAtomic64_FetchOr_acquire(x,v)|v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_OrFetch_release(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return _DeeAtomic64_FetchOr_release(x,v)|v; }
#define _DeeAtomic64_XorFetch_acq_rel    _DeeAtomic64_XorFetch_seq_cst
#define _DeeAtomic64_XorFetch_consume    _DeeAtomic64_XorFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_XorFetch_seq_cst(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return _DeeAtomic64_FetchXor_seq_cst(x,v)^v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_XorFetch_relaxed(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return _DeeAtomic64_FetchXor_relaxed(x,v)^v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_XorFetch_acquire(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return _DeeAtomic64_FetchXor_acquire(x,v)^v; }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_XorFetch_release(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return _DeeAtomic64_FetchXor_release(x,v)^v; }
#define _DeeAtomic64_NandFetch_acq_rel   _DeeAtomic64_NandFetch_seq_cst
#define _DeeAtomic64_NandFetch_consume   _DeeAtomic64_NandFetch_acquire
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_NandFetch_seq_cst(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return ~(_DeeAtomic64_FetchNand_seq_cst(x,v)&v); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_NandFetch_relaxed(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return ~(_DeeAtomic64_FetchNand_relaxed(x,v)&v); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_NandFetch_acquire(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return ~(_DeeAtomic64_FetchNand_acquire(x,v)&v); }
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC64_NATIVE_T) _DeeAtomic64_NandFetch_release(DEE_ATOMIC64_NATIVE_T volatile *x, DEE_ATOMIC64_NATIVE_T v) { return ~(_DeeAtomic64_FetchNand_release(x,v)&v); }

DEE_DECL_END

#endif /* !GUARD_DEEMON___ATOMIC_INTRIN_MSVC_INL */
