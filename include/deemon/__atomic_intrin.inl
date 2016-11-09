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
#ifndef GUARD_DEEMON___ATOMIC_INTRIN_INL
#define GUARD_DEEMON___ATOMIC_INTRIN_INL 1

#include <deemon/__conf.inl>

#ifdef DEE_LIMITED_DEX
#ifdef __INTELLISENSE__
// Just so I get auto completion for this stuff
enum{
 memory_order_relaxed,
 memory_order_consume,
 memory_order_acquire,
 memory_order_release,
 memory_order_acq_rel,
 memory_order_seq_cst
};
#endif
#endif

// Select the appropriate implementation
#define DEE_ATOMIC_INTRIN_INCLUDE_MAGIC
#ifdef DEE_WITHOUT_THREADS
#include <deemon/__atomic_intrin_nothread.inl>
#elif defined(_MSC_VER)
#include <deemon/__atomic_intrin_msvc.inl>
#elif defined(__GNUC__)
#include <deemon/__atomic_intrin_gcc.inl>
#elif defined(__DEEMON__)
#include <deemon/__atomic_intrin_deemon.inl>
#else
#error "No atomic API function (please compile with -DDEE_WITHOUT_THREADS)"
#endif /* !DEE_WITHOUT_THREADS */
#undef DEE_ATOMIC_INTRIN_INCLUDE_MAGIC

#define DEE_PRIVATE_ATOMIC_PP_CAT_3(a,b,c) a##b##c
#define DEE_ATOMIC_PP_CAT_3(a,b,c) DEE_PRIVATE_ATOMIC_PP_CAT_3(a,b,c)

#define DeeAtomicN_Store_F(pp_sizeof)                    DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_Store)
#define DeeAtomicN_Load_F(pp_sizeof)                     DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_Load)
#define DeeAtomicN_Exchange_F(pp_sizeof)                 DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_Exchange)
#define DeeAtomicN_CompareExchangeWeak_F(pp_sizeof)      DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_CompareExchangeWeak)
#define DeeAtomicN_CompareExchangeStrong_F(pp_sizeof)    DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_CompareExchangeStrong)
#define DeeAtomicN_CompareExchangeWeakVal_F(pp_sizeof)   DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_CompareExchangeWeakVal)
#define DeeAtomicN_CompareExchangeStrongVal_F(pp_sizeof) DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_CompareExchangeStrongVal)
#define DeeAtomicN_FetchInc_F(pp_sizeof)                 DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_FetchInc)
#define DeeAtomicN_IncFetch_F(pp_sizeof)                 DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_IncFetch)
#define DeeAtomicN_FetchDec_F(pp_sizeof)                 DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_FetchDec)
#define DeeAtomicN_DecFetch_F(pp_sizeof)                 DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_DecFetch)
#define DeeAtomicN_FetchAdd_F(pp_sizeof)                 DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_FetchAdd)
#define DeeAtomicN_AddFetch_F(pp_sizeof)                 DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_AddFetch)
#define DeeAtomicN_FetchSub_F(pp_sizeof)                 DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_FetchSub)
#define DeeAtomicN_SubFetch_F(pp_sizeof)                 DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_SubFetch)
#define DeeAtomicN_FetchAnd_F(pp_sizeof)                 DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_FetchAnd)
#define DeeAtomicN_AndFetch_F(pp_sizeof)                 DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_AndFetch)
#define DeeAtomicN_FetchOr_F(pp_sizeof)                  DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_FetchOr)
#define DeeAtomicN_OrFetch_F(pp_sizeof)                  DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_OrFetch)
#define DeeAtomicN_FetchXor_F(pp_sizeof)                 DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_FetchXor)
#define DeeAtomicN_XorFetch_F(pp_sizeof)                 DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_XorFetch)
#define DeeAtomicN_FetchNand_F(pp_sizeof)                DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_FetchNand)
#define DeeAtomicN_NandFetch_F(pp_sizeof)                DEE_ATOMIC_PP_CAT_3(DeeAtomic,DEE_PP_MUL8(pp_sizeof),_NandFetch)
#define DeeAtomicN_Store(pp_sizeof,x,v,order)                                                    DeeAtomicN_Store_F(pp_sizeof)(x,v,order)
#define DeeAtomicN_Load(pp_sizeof,x,order)                                                       DeeAtomicN_Load_F(pp_sizeof)(x,order)
#define DeeAtomicN_Exchange(pp_sizeof,x,v,order)                                                 DeeAtomicN_Exchange_F(pp_sizeof)(x,v,order)
#define DeeAtomicN_CompareExchangeWeak(pp_sizeof,x,old_v,new_v,success_order,failure_order)      DeeAtomicN_CompareExchangeWeak_F(pp_sizeof)(x,old_v,new_v,success_order,failure_order)
#define DeeAtomicN_CompareExchangeStrong(pp_sizeof,x,old_v,new_v,success_order,failure_order)    DeeAtomicN_CompareExchangeStrong_F(pp_sizeof)(x,old_v,new_v,success_order,failure_order)
#define DeeAtomicN_CompareExchangeWeakVal(pp_sizeof,x,old_v,new_v,success_order,failure_order)   DeeAtomicN_CompareExchangeWeakVal_F(pp_sizeof)(x,old_v,new_v,success_order,failure_order)
#define DeeAtomicN_CompareExchangeStrongVal(pp_sizeof,x,old_v,new_v,success_order,failure_order) DeeAtomicN_CompareExchangeStrongVal_F(pp_sizeof)(x,old_v,new_v,success_order,failure_order)
#define DeeAtomicN_FetchInc(pp_sizeof,x,order)                                                   DeeAtomicN_FetchInc_F(pp_sizeof)(x,order)
#define DeeAtomicN_IncFetch(pp_sizeof,x,order)                                                   DeeAtomicN_IncFetch_F(pp_sizeof)(x,order)
#define DeeAtomicN_FetchDec(pp_sizeof,x,order)                                                   DeeAtomicN_FetchDec_F(pp_sizeof)(x,order)
#define DeeAtomicN_DecFetch(pp_sizeof,x,order)                                                   DeeAtomicN_DecFetch_F(pp_sizeof)(x,order)
#define DeeAtomicN_FetchAdd(pp_sizeof,x,v,order)                                                 DeeAtomicN_FetchAdd_F(pp_sizeof)(x,v,order)
#define DeeAtomicN_AddFetch(pp_sizeof,x,v,order)                                                 DeeAtomicN_AddFetch_F(pp_sizeof)(x,v,order)
#define DeeAtomicN_FetchSub(pp_sizeof,x,v,order)                                                 DeeAtomicN_FetchSub_F(pp_sizeof)(x,v,order)
#define DeeAtomicN_SubFetch(pp_sizeof,x,v,order)                                                 DeeAtomicN_SubFetch_F(pp_sizeof)(x,v,order)
#define DeeAtomicN_FetchAnd(pp_sizeof,x,v,order)                                                 DeeAtomicN_FetchAnd_F(pp_sizeof)(x,v,order)
#define DeeAtomicN_AndFetch(pp_sizeof,x,v,order)                                                 DeeAtomicN_AndFetch_F(pp_sizeof)(x,v,order)
#define DeeAtomicN_FetchOr(pp_sizeof,x,v,order)                                                  DeeAtomicN_FetchOr_F(pp_sizeof)(x,v,order)
#define DeeAtomicN_OrFetch(pp_sizeof,x,v,order)                                                  DeeAtomicN_OrFetch_F(pp_sizeof)(x,v,order)
#define DeeAtomicN_FetchXor(pp_sizeof,x,v,order)                                                 DeeAtomicN_FetchXor_F(pp_sizeof)(x,v,order)
#define DeeAtomicN_XorFetch(pp_sizeof,x,v,order)                                                 DeeAtomicN_XorFetch_F(pp_sizeof)(x,v,order)
#define DeeAtomicN_FetchNand(pp_sizeof,x,v,order)                                                DeeAtomicN_FetchNand_F(pp_sizeof)(x,v,order)
#define DeeAtomicN_NandFetch(pp_sizeof,x,v,order)                                                DeeAtomicN_NandFetch_F(pp_sizeof)(x,v,order)


//////////////////////////////////////////////////////////////////////////
// DeeAtomic type specific API (pointer)
#define DeeAtomicPtr_Store                    DeeAtomicN_Store_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_Load                     (void *)DeeAtomicN_Load_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_Exchange                 (void *)DeeAtomicN_Exchange_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_CompareExchangeWeak      DeeAtomicN_CompareExchangeWeak_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_CompareExchangeStrong    DeeAtomicN_CompareExchangeStrong_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_CompareExchangeWeakVal   (void *)DeeAtomicN_CompareExchangeWeakVal_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_CompareExchangeStrongVal (void *)DeeAtomicN_CompareExchangeStrongVal_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_FetchInc                 (void *)DeeAtomicN_FetchInc_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_IncFetch                 (void *)DeeAtomicN_IncFetch_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_FetchDec                 (void *)DeeAtomicN_FetchDec_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_DecFetch                 (void *)DeeAtomicN_DecFetch_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_FetchAdd                 (void *)DeeAtomicN_FetchAdd_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_AddFetch                 (void *)DeeAtomicN_AddFetch_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_FetchSub                 (void *)DeeAtomicN_FetchSub_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_SubFetch                 (void *)DeeAtomicN_SubFetch_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_FetchAnd                 (void *)DeeAtomicN_FetchAnd_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_AndFetch                 (void *)DeeAtomicN_AndFetch_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_FetchOr                  (void *)DeeAtomicN_FetchOr_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_OrFetch                  (void *)DeeAtomicN_OrFetch_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_FetchXor                 (void *)DeeAtomicN_FetchXor_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_XorFetch                 (void *)DeeAtomicN_XorFetch_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_FetchNand                (void *)DeeAtomicN_FetchNand_F(DEE_TYPES_SIZEOF_POINTER)
#define DeeAtomicPtr_NandFetch                (void *)DeeAtomicN_NandFetch_F(DEE_TYPES_SIZEOF_POINTER)

//////////////////////////////////////////////////////////////////////////
// DeeAtomic type specific API (builtin types)
#define DeeAtomicChar_Store                     DeeAtomicN_Store_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_Load                      DeeAtomicN_Load_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_Exchange                  DeeAtomicN_Exchange_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_CompareExchangeWeak       DeeAtomicN_CompareExchangeWeak_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_CompareExchangeStrong     DeeAtomicN_CompareExchangeStrong_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_CompareExchangeWeakVal    DeeAtomicN_CompareExchangeWeakVal_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_CompareExchangeStrongVal  DeeAtomicN_CompareExchangeStrongVal_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_FetchInc                  DeeAtomicN_FetchInc_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_IncFetch                  DeeAtomicN_IncFetch_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_FetchDec                  DeeAtomicN_FetchDec_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_DecFetch                  DeeAtomicN_DecFetch_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_FetchAdd                  DeeAtomicN_FetchAdd_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_AddFetch                  DeeAtomicN_AddFetch_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_FetchSub                  DeeAtomicN_FetchSub_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_SubFetch                  DeeAtomicN_SubFetch_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_FetchAnd                  DeeAtomicN_FetchAnd_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_AndFetch                  DeeAtomicN_AndFetch_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_FetchOr                   DeeAtomicN_FetchOr_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_OrFetch                   DeeAtomicN_OrFetch_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_FetchXor                  DeeAtomicN_FetchXor_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_XorFetch                  DeeAtomicN_XorFetch_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_FetchNand                 DeeAtomicN_FetchNand_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicChar_NandFetch                 DeeAtomicN_NandFetch_F(DEE_TYPES_SIZEOF_CHAR)
#define DeeAtomicShort_Store                    DeeAtomicN_Store_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_Load                     DeeAtomicN_Load_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_Exchange                 DeeAtomicN_Exchange_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_CompareExchangeWeak      DeeAtomicN_CompareExchangeWeak_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_CompareExchangeStrong    DeeAtomicN_CompareExchangeStrong_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_CompareExchangeWeakVal   DeeAtomicN_CompareExchangeWeakVal_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_CompareExchangeStrongVal DeeAtomicN_CompareExchangeStrongVal_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_FetchInc                 DeeAtomicN_FetchInc_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_IncFetch                 DeeAtomicN_IncFetch_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_FetchDec                 DeeAtomicN_FetchDec_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_DecFetch                 DeeAtomicN_DecFetch_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_FetchAdd                 DeeAtomicN_FetchAdd_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_AddFetch                 DeeAtomicN_AddFetch_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_FetchSub                 DeeAtomicN_FetchSub_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_SubFetch                 DeeAtomicN_SubFetch_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_FetchAnd                 DeeAtomicN_FetchAnd_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_AndFetch                 DeeAtomicN_AndFetch_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_FetchOr                  DeeAtomicN_FetchOr_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_OrFetch                  DeeAtomicN_OrFetch_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_FetchXor                 DeeAtomicN_FetchXor_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_XorFetch                 DeeAtomicN_XorFetch_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_FetchNand                DeeAtomicN_FetchNand_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicShort_NandFetch                DeeAtomicN_NandFetch_F(DEE_TYPES_SIZEOF_SHORT)
#define DeeAtomicInt_Store                      DeeAtomicN_Store_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_Load                       DeeAtomicN_Load_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_Exchange                   DeeAtomicN_Exchange_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_CompareExchangeWeak        DeeAtomicN_CompareExchangeWeak_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_CompareExchangeStrong      DeeAtomicN_CompareExchangeStrong_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_CompareExchangeWeakVal     DeeAtomicN_CompareExchangeWeakVal_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_CompareExchangeStrongVal   DeeAtomicN_CompareExchangeStrongVal_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_FetchInc                   DeeAtomicN_FetchInc_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_IncFetch                   DeeAtomicN_IncFetch_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_FetchDec                   DeeAtomicN_FetchDec_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_DecFetch                   DeeAtomicN_DecFetch_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_FetchAdd                   DeeAtomicN_FetchAdd_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_AddFetch                   DeeAtomicN_AddFetch_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_FetchSub                   DeeAtomicN_FetchSub_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_SubFetch                   DeeAtomicN_SubFetch_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_FetchAnd                   DeeAtomicN_FetchAnd_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_AndFetch                   DeeAtomicN_AndFetch_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_FetchOr                    DeeAtomicN_FetchOr_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_OrFetch                    DeeAtomicN_OrFetch_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_FetchXor                   DeeAtomicN_FetchXor_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_XorFetch                   DeeAtomicN_XorFetch_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_FetchNand                  DeeAtomicN_FetchNand_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicInt_NandFetch                  DeeAtomicN_NandFetch_F(DEE_TYPES_SIZEOF_INT)
#define DeeAtomicLong_Store                     DeeAtomicN_Store_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_Load                      DeeAtomicN_Load_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_Exchange                  DeeAtomicN_Exchange_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_CompareExchangeWeak       DeeAtomicN_CompareExchangeWeak_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_CompareExchangeStrong     DeeAtomicN_CompareExchangeStrong_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_CompareExchangeWeakVal    DeeAtomicN_CompareExchangeWeakVal_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_CompareExchangeStrongVal  DeeAtomicN_CompareExchangeStrongVal_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_FetchInc                  DeeAtomicN_FetchInc_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_IncFetch                  DeeAtomicN_IncFetch_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_FetchDec                  DeeAtomicN_FetchDec_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_DecFetch                  DeeAtomicN_DecFetch_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_FetchAdd                  DeeAtomicN_FetchAdd_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_AddFetch                  DeeAtomicN_AddFetch_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_FetchSub                  DeeAtomicN_FetchSub_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_SubFetch                  DeeAtomicN_SubFetch_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_FetchAnd                  DeeAtomicN_FetchAnd_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_AndFetch                  DeeAtomicN_AndFetch_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_FetchOr                   DeeAtomicN_FetchOr_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_OrFetch                   DeeAtomicN_OrFetch_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_FetchXor                  DeeAtomicN_FetchXor_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_XorFetch                  DeeAtomicN_XorFetch_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_FetchNand                 DeeAtomicN_FetchNand_F(DEE_TYPES_SIZEOF_LONG)
#define DeeAtomicLong_NandFetch                 DeeAtomicN_NandFetch_F(DEE_TYPES_SIZEOF_LONG)
#ifdef DEE_TYPES_SIZEOF_LLONG
#define DeeAtomicLLong_Store                    DeeAtomicN_Store_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_Load                     DeeAtomicN_Load_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_Exchange                 DeeAtomicN_Exchange_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_CompareExchangeWeak      DeeAtomicN_CompareExchangeWeak_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_CompareExchangeStrong    DeeAtomicN_CompareExchangeStrong_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_CompareExchangeWeakVal   DeeAtomicN_CompareExchangeWeakVal_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_CompareExchangeStrongVal DeeAtomicN_CompareExchangeStrongVal_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_FetchInc                 DeeAtomicN_FetchInc_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_IncFetch                 DeeAtomicN_IncFetch_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_FetchDec                 DeeAtomicN_FetchDec_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_DecFetch                 DeeAtomicN_DecFetch_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_FetchAdd                 DeeAtomicN_FetchAdd_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_AddFetch                 DeeAtomicN_AddFetch_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_FetchSub                 DeeAtomicN_FetchSub_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_SubFetch                 DeeAtomicN_SubFetch_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_FetchAnd                 DeeAtomicN_FetchAnd_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_AndFetch                 DeeAtomicN_AndFetch_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_FetchOr                  DeeAtomicN_FetchOr_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_OrFetch                  DeeAtomicN_OrFetch_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_FetchXor                 DeeAtomicN_FetchXor_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_XorFetch                 DeeAtomicN_XorFetch_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_FetchNand                DeeAtomicN_FetchNand_F(DEE_TYPES_SIZEOF_LLONG)
#define DeeAtomicLLong_NandFetch                DeeAtomicN_NandFetch_F(DEE_TYPES_SIZEOF_LLONG)
#endif /* DEE_TYPES_SIZEOF_LLONG */

#endif /* !GUARD_DEEMON___ATOMIC_INTRIN_INL */
