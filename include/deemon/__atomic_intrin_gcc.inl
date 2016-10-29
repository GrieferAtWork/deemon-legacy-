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
#ifndef GUARD_DEEMON___ATOMIC_INTRIN_GCC_INL
#define GUARD_DEEMON___ATOMIC_INTRIN_GCC_INL 1
#ifndef DEE_ATOMIC_INTRIN_INCLUDE_MAGIC
#error "#include <deemon/__atomic_intrin.inl> instead of this file"
#endif

#include <deemon/__conf.inl>

#define DEE_CONFIG_COMPILER_HAVE_GCC_SYNC_INTRIN    1
#if (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7)))
# define DEE_CONFIG_COMPILER_HAVE_GCC_ATOMIC_INTRIN 1
#endif


#if (__cplusplus) && defined(__INTELLISENSE__)
// Just to get syntax highlighting working in intellisense...
#if DEE_CONFIG_COMPILER_HAVE_GCC_ATOMIC_INTRIN
template<class type> type __atomic_load_n(type volatile *ptr, int memorder);
template<class type> void __atomic_store_n(type volatile *ptr, type val, int memorder);
template<class type> type __atomic_exchange_n(type volatile *ptr, type val, int memorder);
template<class type> bool __atomic_compare_exchange_n(type volatile *ptr, type *expected, type desired, bool weak, int success_memorder, int failure_memorder);
template<class type> type __atomic_add_fetch(type volatile *ptr, type val, int memorder);
template<class type> type __atomic_sub_fetch(type volatile *ptr, type val, int memorder);
template<class type> type __atomic_and_fetch(type volatile *ptr, type val, int memorder);
template<class type> type __atomic_xor_fetch(type volatile *ptr, type val, int memorder);
template<class type> type __atomic_or_fetch(type volatile *ptr, type val, int memorder);
template<class type> type __atomic_nand_fetch(type volatile *ptr, type val, int memorder);
template<class type> type __atomic_fetch_add(type volatile *ptr, type val, int memorder);
template<class type> type __atomic_fetch_sub(type volatile *ptr, type val, int memorder);
template<class type> type __atomic_fetch_and(type volatile *ptr, type val, int memorder);
template<class type> type __atomic_fetch_xor(type volatile *ptr, type val, int memorder);
template<class type> type __atomic_fetch_or(type volatile *ptr, type val, int memorder);
template<class type> type __atomic_fetch_nand(type volatile *ptr, type val, int memorder);
#endif

#if DEE_CONFIG_COMPILER_HAVE_GCC_SYNC_INTRIN
template<class type> type __sync_fetch_and_add(type *ptr, type value, ...);
template<class type> type __sync_fetch_and_sub(type *ptr, type value, ...);
template<class type> type __sync_fetch_and_or(type *ptr, type value, ...);
template<class type> type __sync_fetch_and_and(type *ptr, type value, ...);
template<class type> type __sync_fetch_and_xor(type *ptr, type value, ...);
template<class type> type __sync_fetch_and_nand(type *ptr, type value, ...);
template<class type> type __sync_add_and_fetch(type *ptr, type value, ...);
template<class type> type __sync_sub_and_fetch(type *ptr, type value, ...);
template<class type> type __sync_or_and_fetch(type *ptr, type value, ...);
template<class type> type __sync_and_and_fetch(type *ptr, type value, ...);
template<class type> type __sync_xor_and_fetch(type *ptr, type value, ...);
template<class type> type __sync_nand_and_fetch(type *ptr, type value, ...);
template<class type> bool __sync_bool_compare_and_swap(type *ptr, type oldval, type newval, ...);
template<class type> type __sync_val_compare_and_swap(type *ptr, type oldval, type newval, ...);
void __sync_synchronize(...);
template<class type> type __sync_lock_test_and_set(type *ptr, type value, ...);
template<class type> void __sync_lock_release(type *ptr, ...);
#endif

#endif

#if DEE_CONFIG_COMPILER_HAVE_GCC_ATOMIC_INTRIN
#define DEE_ATOMIC_ORDER_memory_order_relaxed __ATOMIC_RELAXED
#define DEE_ATOMIC_ORDER_memory_order_consume __ATOMIC_CONSUME
#define DEE_ATOMIC_ORDER_memory_order_acquire __ATOMIC_ACQUIRE
#define DEE_ATOMIC_ORDER_memory_order_release __ATOMIC_RELEASE
#define DEE_ATOMIC_ORDER_memory_order_acq_rel __ATOMIC_ACQ_REL
#define DEE_ATOMIC_ORDER_memory_order_seq_cst __ATOMIC_SEQ_CST
#define DEE_ATOMIC_ORDER(x) DEE_ATOMIC_ORDER_##x
#endif

#define DEE_ATOMIC8_NATIVE_T  DEE_TYPES_INT8_T
#define DEE_ATOMIC16_NATIVE_T DEE_TYPES_INT16_T
#define DEE_ATOMIC32_NATIVE_T DEE_TYPES_INT32_T
#define DEE_ATOMIC64_NATIVE_T DEE_TYPES_INT64_T

#if DEE_CONFIG_COMPILER_HAVE_GCC_ATOMIC_INTRIN
#if DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS
#define _DeeAtomicGCC_CompareExchange(T,weak,p,old_val,new_val,success_memorder,failure_memorder)\
 DEE_GCC_EXTENSION({T _temp_oldval=(old_val);__atomic_compare_exchange_n(p,&_temp_oldval,new_val,weak,success_memorder,failure_memorder);})
#define _DeeAtomicGCC_CompareExchangeVal(T,weak,p,old_val,new_val,success_memorder,failure_memorder)\
 DEE_GCC_EXTENSION({T _temp_oldval=(old_val);T const _temp_newval=(new_val);__atomic_compare_exchange_n(p,&_temp_oldval,_temp_newval,weak,success_memorder,failure_memorder)?_temp_newval:_temp_oldval;})
#define _DeeAtomic8_CompareExchangeWeak(p,old_val,new_val,success_memorder,failure_memorder)  _DeeAtomicGCC_CompareExchange(DEE_ATOMIC8_NATIVE_T,1,p,old_val,new_val,success_memorder,failure_memorder)
#define _DeeAtomic16_CompareExchangeWeak(p,old_val,new_val,success_memorder,failure_memorder) _DeeAtomicGCC_CompareExchange(DEE_ATOMIC16_NATIVE_T,1,p,old_val,new_val,success_memorder,failure_memorder)
#define _DeeAtomic32_CompareExchangeWeak(p,old_val,new_val,success_memorder,failure_memorder) _DeeAtomicGCC_CompareExchange(DEE_ATOMIC32_NATIVE_T,1,p,old_val,new_val,success_memorder,failure_memorder)
#define _DeeAtomic64_CompareExchangeWeak(p,old_val,new_val,success_memorder,failure_memorder) _DeeAtomicGCC_CompareExchange(DEE_ATOMIC64_NATIVE_T,1,p,old_val,new_val,success_memorder,failure_memorder)
#define _DeeAtomic8_CompareExchangeStrong(p,old_val,new_val,success_memorder,failure_memorder)  _DeeAtomicGCC_CompareExchange(DEE_ATOMIC8_NATIVE_T,0,p,old_val,new_val,success_memorder,failure_memorder)
#define _DeeAtomic16_CompareExchangeStrong(p,old_val,new_val,success_memorder,failure_memorder) _DeeAtomicGCC_CompareExchange(DEE_ATOMIC16_NATIVE_T,0,p,old_val,new_val,success_memorder,failure_memorder)
#define _DeeAtomic32_CompareExchangeStrong(p,old_val,new_val,success_memorder,failure_memorder) _DeeAtomicGCC_CompareExchange(DEE_ATOMIC32_NATIVE_T,0,p,old_val,new_val,success_memorder,failure_memorder)
#define _DeeAtomic64_CompareExchangeStrong(p,old_val,new_val,success_memorder,failure_memorder) _DeeAtomicGCC_CompareExchange(DEE_ATOMIC64_NATIVE_T,0,p,old_val,new_val,success_memorder,failure_memorder)
#define _DeeAtomic8_CompareExchangeWeakVal(p,old_val,new_val,success_memorder,failure_memorder)  _DeeAtomicGCC_CompareExchangeVal(DEE_ATOMIC8_NATIVE_T,1,p,old_val,new_val,success_memorder,failure_memorder)
#define _DeeAtomic16_CompareExchangeWeakVal(p,old_val,new_val,success_memorder,failure_memorder) _DeeAtomicGCC_CompareExchangeVal(DEE_ATOMIC16_NATIVE_T,1,p,old_val,new_val,success_memorder,failure_memorder)
#define _DeeAtomic32_CompareExchangeWeakVal(p,old_val,new_val,success_memorder,failure_memorder) _DeeAtomicGCC_CompareExchangeVal(DEE_ATOMIC32_NATIVE_T,1,p,old_val,new_val,success_memorder,failure_memorder)
#define _DeeAtomic64_CompareExchangeWeakVal(p,old_val,new_val,success_memorder,failure_memorder) _DeeAtomicGCC_CompareExchangeVal(DEE_ATOMIC64_NATIVE_T,1,p,old_val,new_val,success_memorder,failure_memorder)
#define _DeeAtomic8_CompareExchangeStrongVal(p,old_val,new_val,success_memorder,failure_memorder)  _DeeAtomicGCC_CompareExchangeVal(DEE_ATOMIC8_NATIVE_T,0,p,old_val,new_val,success_memorder,failure_memorder)
#define _DeeAtomic16_CompareExchangeStrongVal(p,old_val,new_val,success_memorder,failure_memorder) _DeeAtomicGCC_CompareExchangeVal(DEE_ATOMIC16_NATIVE_T,0,p,old_val,new_val,success_memorder,failure_memorder)
#define _DeeAtomic32_CompareExchangeStrongVal(p,old_val,new_val,success_memorder,failure_memorder) _DeeAtomicGCC_CompareExchangeVal(DEE_ATOMIC32_NATIVE_T,0,p,old_val,new_val,success_memorder,failure_memorder)
#define _DeeAtomic64_CompareExchangeStrongVal(p,old_val,new_val,success_memorder,failure_memorder) _DeeAtomicGCC_CompareExchangeVal(DEE_ATOMIC64_NATIVE_T,0,p,old_val,new_val,success_memorder,failure_memorder)
#else /* DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */
#define DEE_PRIVATE_GCC_MAKE_COMPARE_EXPCHANGE(n)\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic##n##_CompareExchangeWeak(\
 DEE_ATOMIC##n##_NATIVE_T volatile *p, DEE_ATOMIC##n##_NATIVE_T old_val, DEE_ATOMIC##n##_NATIVE_T new_val, int success_memorder, int failure_memorder) {\
 return __atomic_compare_exchange_n(p,&old_val,new_val,1,success_memorder,failure_memorder);\
}\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST int) _DeeAtomic##n##_CompareExchangeStrong(\
 DEE_ATOMIC##n##_NATIVE_T volatile *p, DEE_ATOMIC##n##_NATIVE_T old_val, DEE_ATOMIC##n##_NATIVE_T new_val, int success_memorder, int failure_memorder) {\
 return __atomic_compare_exchange_n(p,&old_val,new_val,0,success_memorder,failure_memorder);\
}\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC##n##_NATIVE_T) _DeeAtomic##n##_CompareExchangeWeakVal(\
 DEE_ATOMIC##n##_NATIVE_T volatile *p, DEE_ATOMIC##n##_NATIVE_T old_val, DEE_ATOMIC##n##_NATIVE_T new_val, int success_memorder, int failure_memorder) {\
 return __atomic_compare_exchange_n(p,&old_val,new_val,1,success_memorder,failure_memorder) ? new_val : old_val;\
}\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC##n##_NATIVE_T) _DeeAtomic##n##_CompareExchangeStrongVal(\
 DEE_ATOMIC##n##_NATIVE_T volatile *p, DEE_ATOMIC##n##_NATIVE_T old_val, DEE_ATOMIC##n##_NATIVE_T new_val, int success_memorder, int failure_memorder) {\
 return __atomic_compare_exchange_n(p,&old_val,new_val,0,success_memorder,failure_memorder) ? new_val : old_val;\
}
DEE_PRIVATE_GCC_MAKE_COMPARE_EXPCHANGE(8)
DEE_PRIVATE_GCC_MAKE_COMPARE_EXPCHANGE(16)
DEE_PRIVATE_GCC_MAKE_COMPARE_EXPCHANGE(32)
DEE_PRIVATE_GCC_MAKE_COMPARE_EXPCHANGE(64)
#undef DEE_PRIVATE_GCC_MAKE_COMPARE_EXPCHANGE
#endif /* !DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */

#define DeeAtomicGCC_Store(T,x,v,order)     __atomic_store_n((T volatile *)&(x),(T)(v),DEE_ATOMIC_ORDER(order))
#define DeeAtomicGCC_Load(T,x,order)        __atomic_load_n((T volatile *)&(x),DEE_ATOMIC_ORDER(order))
#define DeeAtomicGCC_Exchange(T,x,v,order)  __atomic_exchange_n((T volatile *)&(x),(T)(v),DEE_ATOMIC_ORDER(order))
#define DeeAtomicGCC_FetchAdd(T,x,v,order)  __atomic_fetch_add((T volatile *)&(x),(T)(v),DEE_ATOMIC_ORDER(order))
#define DeeAtomicGCC_AddFetch(T,x,v,order)  __atomic_add_fetch((T volatile *)&(x),(T)(v),DEE_ATOMIC_ORDER(order))
#define DeeAtomicGCC_FetchSub(T,x,v,order)  __atomic_fetch_sub((T volatile *)&(x),(T)(v),DEE_ATOMIC_ORDER(order))
#define DeeAtomicGCC_SubFetch(T,x,v,order)  __atomic_sub_fetch((T volatile *)&(x),(T)(v),DEE_ATOMIC_ORDER(order))
#define DeeAtomicGCC_FetchAnd(T,x,v,order)  __atomic_fetch_and((T volatile *)&(x),(T)(v),DEE_ATOMIC_ORDER(order))
#define DeeAtomicGCC_AndFetch(T,x,v,order)  __atomic_and_fetch((T volatile *)&(x),(T)(v),DEE_ATOMIC_ORDER(order))
#define DeeAtomicGCC_FetchOr(T,x,v,order)   __atomic_fetch_or((T volatile *)&(x),(T)(v),DEE_ATOMIC_ORDER(order))
#define DeeAtomicGCC_OrFetch(T,x,v,order)   __atomic_or_fetch((T volatile *)&(x),(T)(v),DEE_ATOMIC_ORDER(order))
#define DeeAtomicGCC_FetchXor(T,x,v,order)  __atomic_fetch_xor((T volatile *)&(x),(T)(v),DEE_ATOMIC_ORDER(order))
#define DeeAtomicGCC_XorFetch(T,x,v,order)  __atomic_xor_fetch((T volatile *)&(x),(T)(v),DEE_ATOMIC_ORDER(order))
#define DeeAtomicGCC_FetchNand(T,x,v,order) __atomic_fetch_nand((T volatile *)&(x),(T)(v),DEE_ATOMIC_ORDER(order))
#define DeeAtomicGCC_NandFetch(T,x,v,order) __atomic_nand_fetch((T volatile *)&(x),(T)(v),DEE_ATOMIC_ORDER(order))
#define DeeAtomicGCC_FetchInc(T,x,order)    DeeAtomicGCC_FetchAdd(T,x,1,order)
#define DeeAtomicGCC_IncFetch(T,x,order)    DeeAtomicGCC_AddFetch(T,x,1,order)
#define DeeAtomicGCC_FetchDec(T,x,order)    DeeAtomicGCC_FetchSub(T,x,1,order)
#define DeeAtomicGCC_DecFetch(T,x,order)    DeeAtomicGCC_SubFetch(T,x,1,order)
#elif DEE_CONFIG_COMPILER_HAVE_GCC_SYNC_INTRIN

#if DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS
#define _DeeAtomicGCC_Load(T,p)       DEE_GCC_EXTENSION({T *_temp_p=(p),_temp_old;do _temp_old=*_temp_p;while(!__sync_bool_compare_and_swap(_temp_p,_temp_old,_temp_old));_temp_old;})
#define _DeeAtomicGCC_Store(T,p,v)    DEE_GCC_EXTENSION({T *_temp_p=(p),_temp_old,_temp_new=(v);do _temp_old=*_temp_p;while(!__sync_bool_compare_and_swap(_temp_p,_temp_old,_temp_new));(void)0;})
#define _DeeAtomicGCC_Exchange(T,p,v) DEE_GCC_EXTENSION({T *_temp_p=(p),_temp_old,_temp_new=(v);do _temp_old=*_temp_p;while(!__sync_bool_compare_and_swap(_temp_p,_temp_old,_temp_new));_temp_old;})
#define _DeeAtomic8_Load(p)        _DeeAtomicGCC_Load(DEE_ATOMIC8_NATIVE_T,p)
#define _DeeAtomic8_Store(p,v)     _DeeAtomicGCC_Store(DEE_ATOMIC8_NATIVE_T,p,v)
#define _DeeAtomic8_Exchange(p,v)  _DeeAtomicGCC_Exchange(DEE_ATOMIC8_NATIVE_T,p,v)
#define _DeeAtomic16_Load(p)       _DeeAtomicGCC_Load(DEE_ATOMIC16_NATIVE_T,p)
#define _DeeAtomic16_Store(p,v)    _DeeAtomicGCC_Store(DEE_ATOMIC16_NATIVE_T,p,v)
#define _DeeAtomic16_Exchange(p,v) _DeeAtomicGCC_Exchange(DEE_ATOMIC16_NATIVE_T,p,v)
#define _DeeAtomic32_Load(p)       _DeeAtomicGCC_Load(DEE_ATOMIC32_NATIVE_T,p)
#define _DeeAtomic32_Store(p,v)    _DeeAtomicGCC_Store(DEE_ATOMIC32_NATIVE_T,p,v)
#define _DeeAtomic32_Exchange(p,v) _DeeAtomicGCC_Exchange(DEE_ATOMIC32_NATIVE_T,p,v)
#define _DeeAtomic64_Load(p)       _DeeAtomicGCC_Load(DEE_ATOMIC64_NATIVE_T,p)
#define _DeeAtomic64_Store(p,v)    _DeeAtomicGCC_Store(DEE_ATOMIC64_NATIVE_T,p,v)
#define _DeeAtomic64_Exchange(p,v) _DeeAtomicGCC_Exchange(DEE_ATOMIC64_NATIVE_T,p,v)
#else
#define DEE_PRIVATE_GCC_MAKE_LOAD_STORE(n)\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC##n##_NATIVE_T) \
_DeeAtomic##n##_Load(DEE_ATOMIC##n##_NATIVE_T *p) {\
 DEE_ATOMIC##n##_NATIVE_T result;\
 do result = *p; while(!__sync_bool_compare_and_swap(p,result,result));\
 return result;\
}\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST void) \
_DeeAtomic##n##_Store(DEE_ATOMIC##n##_NATIVE_T *p, DEE_ATOMIC##n##_NATIVE_T v) {\
 DEE_ATOMIC##n##_NATIVE_T oldval;\
 do oldval = *p; while (!__sync_bool_compare_and_swap(p,oldval,v));\
}\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_ATOMIC##n##_NATIVE_T) \
_DeeAtomic##n##_Exchange(DEE_ATOMIC##n##_NATIVE_T *p, DEE_ATOMIC##n##_NATIVE_T v) {\
 DEE_ATOMIC##n##_NATIVE_T oldval;\
 do oldval = *p; while (!__sync_bool_compare_and_swap(p,oldval,v));\
 return oldval;\
}
DEE_PRIVATE_GCC_MAKE_LOAD_STORE(8)
DEE_PRIVATE_GCC_MAKE_LOAD_STORE(16)
DEE_PRIVATE_GCC_MAKE_LOAD_STORE(32)
DEE_PRIVATE_GCC_MAKE_LOAD_STORE(64)
#undef DEE_PRIVATE_GCC_MAKE_LOAD_STORE
#endif
#define DeeAtomicGCC_CompareExchangeWeak(T,x,old_v,new_v,success_order,failure_order)      __sync_bool_compare_and_swap((T *)&(x),(T)(old_v),(T)(new_v))
#define DeeAtomicGCC_CompareExchangeStrong(T,x,old_v,new_v,success_order,failure_order)    __sync_bool_compare_and_swap((T *)&(x),(T)(old_v),(T)(new_v))
#define DeeAtomicGCC_CompareExchangeWeakVal(T,x,old_v,new_v,success_order,failure_order)   __sync_val_compare_and_swap((T *)&(x),(T)(old_v),(T)(new_v))
#define DeeAtomicGCC_CompareExchangeStrongVal(T,x,old_v,new_v,success_order,failure_order) __sync_val_compare_and_swap((T *)&(x),(T)(old_v),(T)(new_v))
#define DeeAtomicGCC_FetchAdd(T,x,v,order)  __sync_fetch_and_add((T *)&(x),(T)(v))
#define DeeAtomicGCC_AddFetch(T,x,v,order)  __sync_add_and_fetch((T *)&(x),(T)(v))
#define DeeAtomicGCC_FetchSub(T,x,v,order)  __sync_fetch_and_sub((T *)&(x),(T)(v))
#define DeeAtomicGCC_SubFetch(T,x,v,order)  __sync_sub_and_fetch((T *)&(x),(T)(v))
#define DeeAtomicGCC_FetchAnd(T,x,v,order)  __sync_fetch_and_and((T *)&(x),(T)(v))
#define DeeAtomicGCC_AndFetch(T,x,v,order)  __sync_and_and_fetch((T *)&(x),(T)(v))
#define DeeAtomicGCC_FetchOr(T,x,v,order)   __sync_fetch_and_or((T *)&(x),(T)(v))
#define DeeAtomicGCC_OrFetch(T,x,v,order)   __sync_or_and_fetch((T *)&(x),(T)(v))
#define DeeAtomicGCC_FetchXor(T,x,v,order)  __sync_fetch_and_xor((T *)&(x),(T)(v))
#define DeeAtomicGCC_XorFetch(T,x,v,order)  __sync_xor_and_fetch((T *)&(x),(T)(v))
#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 4))
#define DeeAtomicGCC_FetchNand(T,x,v,order) __sync_fetch_and_nand((T *)&(x),(T)(v))
#define DeeAtomicGCC_NandFetch(T,x,v,order) __sync_nand_and_fetch((T *)&(x),(T)(v))
#else
// GCC before 4.4 implemented this as: "*ptr = ~*ptr & value"
//                         instead of: "*ptr = ~(*ptr & value)";
// >> So we need to fix that by implementing it ourselves
#define DeeAtomicGCC_FetchNand(T,x,v,order) DEE_GCC_EXTENSION({ T _temp,*const _x=(T*)&(x); T const _v=(T)(v); do _temp = *_x; while (!__sync_bool_compare_and_swap(_x,_temp,(T)~(*_x&_v))); _temp; })
#define DeeAtomicGCC_NandFetch(T,x,v,order) DEE_GCC_EXTENSION({ T _temp,*const _x=(T*)&(x); T const _v=(T)(v); do _temp = *_x; while (!__sync_bool_compare_and_swap(_x,_temp,(T)~(*_x&_v))); (T)~(_temp&_v); })
#endif
#define DeeAtomicGCC_FetchInc(T,x,order)    DeeAtomicGCC_FetchAdd(T,x,1,order)
#define DeeAtomicGCC_IncFetch(T,x,order)    DeeAtomicGCC_AddFetch(T,x,1,order)
#define DeeAtomicGCC_FetchDec(T,x,order)    DeeAtomicGCC_FetchSub(T,x,1,order)
#define DeeAtomicGCC_DecFetch(T,x,order)    DeeAtomicGCC_SubFetch(T,x,1,order)
#endif /* ... */


//////////////////////////////////////////////////////////////////////////
// DeeAtomic8 Native API
#if DEE_CONFIG_COMPILER_HAVE_GCC_ATOMIC_INTRIN
#define DeeAtomic8_Store(x,v,order)                                                    DeeAtomicGCC_Store(DEE_ATOMIC8_NATIVE_T,x,v,order)
#define DeeAtomic8_Load(x,order)                                                       DeeAtomicGCC_Load(DEE_ATOMIC8_NATIVE_T,x,order)
#define DeeAtomic8_Exchange(x,v,order)                                                 DeeAtomicGCC_Exchange(DEE_ATOMIC8_NATIVE_T,x,v,order)
#define DeeAtomic8_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      _DeeAtomic8_CompareExchangeWeak((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(old_v),(DEE_ATOMIC8_NATIVE_T)(new_v),DEE_ATOMIC_ORDER(success_order),DEE_ATOMIC_ORDER(failure_order))
#define DeeAtomic8_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    _DeeAtomic8_CompareExchangeStrong((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(old_v),(DEE_ATOMIC8_NATIVE_T)(new_v),DEE_ATOMIC_ORDER(success_order),DEE_ATOMIC_ORDER(failure_order))
#define DeeAtomic8_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   _DeeAtomic8_CompareExchangeWeakVal((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(old_v),(DEE_ATOMIC8_NATIVE_T)(new_v),DEE_ATOMIC_ORDER(success_order),DEE_ATOMIC_ORDER(failure_order))
#define DeeAtomic8_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) _DeeAtomic8_CompareExchangeStrongVal((DEE_ATOMIC8_NATIVE_T volatile *)&(x),(DEE_ATOMIC8_NATIVE_T)(old_v),(DEE_ATOMIC8_NATIVE_T)(new_v),DEE_ATOMIC_ORDER(success_order),DEE_ATOMIC_ORDER(failure_order))
#elif DEE_CONFIG_COMPILER_HAVE_GCC_SYNC_INTRIN
#define DeeAtomic8_Store(x,v,order)                                                    _DeeAtomic8_Store((DEE_ATOMIC8_NATIVE_T *)&(x),(DEE_ATOMIC8_NATIVE_T)(v))
#define DeeAtomic8_Load(x,order)                                                       _DeeAtomic8_Load((DEE_ATOMIC8_NATIVE_T *)&(x))
#define DeeAtomic8_Exchange(x,v,order)                                                 _DeeAtomic8_Exchange((DEE_ATOMIC8_NATIVE_T *)&(x),(DEE_ATOMIC8_NATIVE_T)(v))
#define DeeAtomic8_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      DeeAtomicGCC_CompareExchangeWeak(DEE_ATOMIC8_NATIVE_T,x,old_v,new_v,success_order,failure_order)
#define DeeAtomic8_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    DeeAtomicGCC_CompareExchangeStrong(DEE_ATOMIC8_NATIVE_T,x,old_v,new_v,success_order,failure_order)
#define DeeAtomic8_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   DeeAtomicGCC_CompareExchangeWeakVal(DEE_ATOMIC8_NATIVE_T,x,old_v,new_v,success_order,failure_order)
#define DeeAtomic8_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) DeeAtomicGCC_CompareExchangeStrongVal(DEE_ATOMIC8_NATIVE_T,x,old_v,new_v,success_order,failure_order)
#endif
#define DeeAtomic8_FetchInc(x,order)                                                   DeeAtomicGCC_FetchInc(DEE_ATOMIC8_NATIVE_T,x,order)
#define DeeAtomic8_IncFetch(x,order)                                                   DeeAtomicGCC_IncFetch(DEE_ATOMIC8_NATIVE_T,x,order)
#define DeeAtomic8_FetchDec(x,order)                                                   DeeAtomicGCC_FetchDec(DEE_ATOMIC8_NATIVE_T,x,order)
#define DeeAtomic8_DecFetch(x,order)                                                   DeeAtomicGCC_DecFetch(DEE_ATOMIC8_NATIVE_T,x,order)
#define DeeAtomic8_FetchAdd(x,v,order)                                                 DeeAtomicGCC_FetchAdd(DEE_ATOMIC8_NATIVE_T,x,v,order)
#define DeeAtomic8_AddFetch(x,v,order)                                                 DeeAtomicGCC_AddFetch(DEE_ATOMIC8_NATIVE_T,x,v,order)
#define DeeAtomic8_FetchSub(x,v,order)                                                 DeeAtomicGCC_FetchSub(DEE_ATOMIC8_NATIVE_T,x,v,order)
#define DeeAtomic8_SubFetch(x,v,order)                                                 DeeAtomicGCC_SubFetch(DEE_ATOMIC8_NATIVE_T,x,v,order)
#define DeeAtomic8_FetchAnd(x,v,order)                                                 DeeAtomicGCC_FetchAnd(DEE_ATOMIC8_NATIVE_T,x,v,order)
#define DeeAtomic8_AndFetch(x,v,order)                                                 DeeAtomicGCC_AndFetch(DEE_ATOMIC8_NATIVE_T,x,v,order)
#define DeeAtomic8_FetchOr(x,v,order)                                                  DeeAtomicGCC_FetchOr(DEE_ATOMIC8_NATIVE_T,x,v,order)
#define DeeAtomic8_OrFetch(x,v,order)                                                  DeeAtomicGCC_OrFetch(DEE_ATOMIC8_NATIVE_T,x,v,order)
#define DeeAtomic8_FetchXor(x,v,order)                                                 DeeAtomicGCC_FetchXor(DEE_ATOMIC8_NATIVE_T,x,v,order)
#define DeeAtomic8_XorFetch(x,v,order)                                                 DeeAtomicGCC_XorFetch(DEE_ATOMIC8_NATIVE_T,x,v,order)
#define DeeAtomic8_FetchNand(x,v,order)                                                DeeAtomicGCC_FetchNand(DEE_ATOMIC8_NATIVE_T,x,v,order)
#define DeeAtomic8_NandFetch(x,v,order)                                                DeeAtomicGCC_NandFetch(DEE_ATOMIC8_NATIVE_T,x,v,order)


//////////////////////////////////////////////////////////////////////////
// DeeAtomic16 Native API
#if DEE_CONFIG_COMPILER_HAVE_GCC_ATOMIC_INTRIN
#define DeeAtomic16_Store(x,v,order)                                                    DeeAtomicGCC_Store(DEE_ATOMIC16_NATIVE_T,x,v,order)
#define DeeAtomic16_Load(x,order)                                                       DeeAtomicGCC_Load(DEE_ATOMIC16_NATIVE_T,x,order)
#define DeeAtomic16_Exchange(x,v,order)                                                 DeeAtomicGCC_Exchange(DEE_ATOMIC16_NATIVE_T,x,v,order)
#define DeeAtomic16_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      _DeeAtomic16_CompareExchangeWeak((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(old_v),(DEE_ATOMIC16_NATIVE_T)(new_v),DEE_ATOMIC_ORDER(success_order),DEE_ATOMIC_ORDER(failure_order))
#define DeeAtomic16_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    _DeeAtomic16_CompareExchangeStrong((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(old_v),(DEE_ATOMIC16_NATIVE_T)(new_v),DEE_ATOMIC_ORDER(success_order),DEE_ATOMIC_ORDER(failure_order))
#define DeeAtomic16_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   _DeeAtomic16_CompareExchangeWeakVal((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(old_v),(DEE_ATOMIC16_NATIVE_T)(new_v),DEE_ATOMIC_ORDER(success_order),DEE_ATOMIC_ORDER(failure_order))
#define DeeAtomic16_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) _DeeAtomic16_CompareExchangeStrongVal((DEE_ATOMIC16_NATIVE_T volatile *)&(x),(DEE_ATOMIC16_NATIVE_T)(old_v),(DEE_ATOMIC16_NATIVE_T)(new_v),DEE_ATOMIC_ORDER(success_order),DEE_ATOMIC_ORDER(failure_order))
#elif DEE_CONFIG_COMPILER_HAVE_GCC_SYNC_INTRIN
#define DeeAtomic16_Store(x,v,order)                                                    _DeeAtomic16_Store((DEE_ATOMIC16_NATIVE_T *)&(x),(DEE_ATOMIC16_NATIVE_T)(v))
#define DeeAtomic16_Load(x,order)                                                       _DeeAtomic16_Load((DEE_ATOMIC16_NATIVE_T *)&(x))
#define DeeAtomic16_Exchange(x,v,order)                                                 _DeeAtomic16_Exchange((DEE_ATOMIC16_NATIVE_T *)&(x),(DEE_ATOMIC16_NATIVE_T)(v))
#define DeeAtomic16_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      DeeAtomicGCC_CompareExchangeWeak(DEE_ATOMIC16_NATIVE_T,x,old_v,new_v,success_order,failure_order)
#define DeeAtomic16_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    DeeAtomicGCC_CompareExchangeStrong(DEE_ATOMIC16_NATIVE_T,x,old_v,new_v,success_order,failure_order)
#define DeeAtomic16_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   DeeAtomicGCC_CompareExchangeWeakVal(DEE_ATOMIC16_NATIVE_T,x,old_v,new_v,success_order,failure_order)
#define DeeAtomic16_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) DeeAtomicGCC_CompareExchangeStrongVal(DEE_ATOMIC16_NATIVE_T,x,old_v,new_v,success_order,failure_order)
#endif
#define DeeAtomic16_FetchInc(x,order)                                                   DeeAtomicGCC_FetchInc(DEE_ATOMIC16_NATIVE_T,x,order)
#define DeeAtomic16_IncFetch(x,order)                                                   DeeAtomicGCC_IncFetch(DEE_ATOMIC16_NATIVE_T,x,order)
#define DeeAtomic16_FetchDec(x,order)                                                   DeeAtomicGCC_FetchDec(DEE_ATOMIC16_NATIVE_T,x,order)
#define DeeAtomic16_DecFetch(x,order)                                                   DeeAtomicGCC_DecFetch(DEE_ATOMIC16_NATIVE_T,x,order)
#define DeeAtomic16_FetchAdd(x,v,order)                                                 DeeAtomicGCC_FetchAdd(DEE_ATOMIC16_NATIVE_T,x,v,order)
#define DeeAtomic16_AddFetch(x,v,order)                                                 DeeAtomicGCC_AddFetch(DEE_ATOMIC16_NATIVE_T,x,v,order)
#define DeeAtomic16_FetchSub(x,v,order)                                                 DeeAtomicGCC_FetchSub(DEE_ATOMIC16_NATIVE_T,x,v,order)
#define DeeAtomic16_SubFetch(x,v,order)                                                 DeeAtomicGCC_SubFetch(DEE_ATOMIC16_NATIVE_T,x,v,order)
#define DeeAtomic16_FetchAnd(x,v,order)                                                 DeeAtomicGCC_FetchAnd(DEE_ATOMIC16_NATIVE_T,x,v,order)
#define DeeAtomic16_AndFetch(x,v,order)                                                 DeeAtomicGCC_AndFetch(DEE_ATOMIC16_NATIVE_T,x,v,order)
#define DeeAtomic16_FetchOr(x,v,order)                                                  DeeAtomicGCC_FetchOr(DEE_ATOMIC16_NATIVE_T,x,v,order)
#define DeeAtomic16_OrFetch(x,v,order)                                                  DeeAtomicGCC_OrFetch(DEE_ATOMIC16_NATIVE_T,x,v,order)
#define DeeAtomic16_FetchXor(x,v,order)                                                 DeeAtomicGCC_FetchXor(DEE_ATOMIC16_NATIVE_T,x,v,order)
#define DeeAtomic16_XorFetch(x,v,order)                                                 DeeAtomicGCC_XorFetch(DEE_ATOMIC16_NATIVE_T,x,v,order)
#define DeeAtomic16_FetchNand(x,v,order)                                                DeeAtomicGCC_FetchNand(DEE_ATOMIC16_NATIVE_T,x,v,order)
#define DeeAtomic16_NandFetch(x,v,order)                                                DeeAtomicGCC_NandFetch(DEE_ATOMIC16_NATIVE_T,x,v,order)

//////////////////////////////////////////////////////////////////////////
// DeeAtomic32 Native API
#if DEE_CONFIG_COMPILER_HAVE_GCC_ATOMIC_INTRIN
#define DeeAtomic32_Store(x,v,order)                                                    DeeAtomicGCC_Store(DEE_ATOMIC32_NATIVE_T,x,v,order)
#define DeeAtomic32_Load(x,order)                                                       DeeAtomicGCC_Load(DEE_ATOMIC32_NATIVE_T,x,order)
#define DeeAtomic32_Exchange(x,v,order)                                                 DeeAtomicGCC_Exchange(DEE_ATOMIC32_NATIVE_T,x,v,order)
#define DeeAtomic32_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      _DeeAtomic32_CompareExchangeWeak((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(old_v),(DEE_ATOMIC32_NATIVE_T)(new_v),DEE_ATOMIC_ORDER(success_order),DEE_ATOMIC_ORDER(failure_order))
#define DeeAtomic32_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    _DeeAtomic32_CompareExchangeStrong((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(old_v),(DEE_ATOMIC32_NATIVE_T)(new_v),DEE_ATOMIC_ORDER(success_order),DEE_ATOMIC_ORDER(failure_order))
#define DeeAtomic32_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   _DeeAtomic32_CompareExchangeWeakVal((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(old_v),(DEE_ATOMIC32_NATIVE_T)(new_v),DEE_ATOMIC_ORDER(success_order),DEE_ATOMIC_ORDER(failure_order))
#define DeeAtomic32_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) _DeeAtomic32_CompareExchangeStrongVal((DEE_ATOMIC32_NATIVE_T volatile *)&(x),(DEE_ATOMIC32_NATIVE_T)(old_v),(DEE_ATOMIC32_NATIVE_T)(new_v),DEE_ATOMIC_ORDER(success_order),DEE_ATOMIC_ORDER(failure_order))
#elif DEE_CONFIG_COMPILER_HAVE_GCC_SYNC_INTRIN
#define DeeAtomic32_Store(x,v,order)                                                    _DeeAtomic32_Store((DEE_ATOMIC32_NATIVE_T *)&(x),(DEE_ATOMIC32_NATIVE_T)(v))
#define DeeAtomic32_Load(x,order)                                                       _DeeAtomic32_Load((DEE_ATOMIC32_NATIVE_T *)&(x))
#define DeeAtomic32_Exchange(x,v,order)                                                 _DeeAtomic32_Exchange((DEE_ATOMIC32_NATIVE_T *)&(x),(DEE_ATOMIC32_NATIVE_T)(v))
#define DeeAtomic32_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      DeeAtomicGCC_CompareExchangeWeak(DEE_ATOMIC32_NATIVE_T,x,old_v,new_v,success_order,failure_order)
#define DeeAtomic32_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    DeeAtomicGCC_CompareExchangeStrong(DEE_ATOMIC32_NATIVE_T,x,old_v,new_v,success_order,failure_order)
#define DeeAtomic32_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   DeeAtomicGCC_CompareExchangeWeakVal(DEE_ATOMIC32_NATIVE_T,x,old_v,new_v,success_order,failure_order)
#define DeeAtomic32_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) DeeAtomicGCC_CompareExchangeStrongVal(DEE_ATOMIC32_NATIVE_T,x,old_v,new_v,success_order,failure_order)
#endif
#define DeeAtomic32_FetchInc(x,order)                                                   DeeAtomicGCC_FetchInc(DEE_ATOMIC32_NATIVE_T,x,order)
#define DeeAtomic32_IncFetch(x,order)                                                   DeeAtomicGCC_IncFetch(DEE_ATOMIC32_NATIVE_T,x,order)
#define DeeAtomic32_FetchDec(x,order)                                                   DeeAtomicGCC_FetchDec(DEE_ATOMIC32_NATIVE_T,x,order)
#define DeeAtomic32_DecFetch(x,order)                                                   DeeAtomicGCC_DecFetch(DEE_ATOMIC32_NATIVE_T,x,order)
#define DeeAtomic32_FetchAdd(x,v,order)                                                 DeeAtomicGCC_FetchAdd(DEE_ATOMIC32_NATIVE_T,x,v,order)
#define DeeAtomic32_AddFetch(x,v,order)                                                 DeeAtomicGCC_AddFetch(DEE_ATOMIC32_NATIVE_T,x,v,order)
#define DeeAtomic32_FetchSub(x,v,order)                                                 DeeAtomicGCC_FetchSub(DEE_ATOMIC32_NATIVE_T,x,v,order)
#define DeeAtomic32_SubFetch(x,v,order)                                                 DeeAtomicGCC_SubFetch(DEE_ATOMIC32_NATIVE_T,x,v,order)
#define DeeAtomic32_FetchAnd(x,v,order)                                                 DeeAtomicGCC_FetchAnd(DEE_ATOMIC32_NATIVE_T,x,v,order)
#define DeeAtomic32_AndFetch(x,v,order)                                                 DeeAtomicGCC_AndFetch(DEE_ATOMIC32_NATIVE_T,x,v,order)
#define DeeAtomic32_FetchOr(x,v,order)                                                  DeeAtomicGCC_FetchOr(DEE_ATOMIC32_NATIVE_T,x,v,order)
#define DeeAtomic32_OrFetch(x,v,order)                                                  DeeAtomicGCC_OrFetch(DEE_ATOMIC32_NATIVE_T,x,v,order)
#define DeeAtomic32_FetchXor(x,v,order)                                                 DeeAtomicGCC_FetchXor(DEE_ATOMIC32_NATIVE_T,x,v,order)
#define DeeAtomic32_XorFetch(x,v,order)                                                 DeeAtomicGCC_XorFetch(DEE_ATOMIC32_NATIVE_T,x,v,order)
#define DeeAtomic32_FetchNand(x,v,order)                                                DeeAtomicGCC_FetchNand(DEE_ATOMIC32_NATIVE_T,x,v,order)
#define DeeAtomic32_NandFetch(x,v,order)                                                DeeAtomicGCC_NandFetch(DEE_ATOMIC32_NATIVE_T,x,v,order)

//////////////////////////////////////////////////////////////////////////
// DeeAtomic64 Native API
#if DEE_CONFIG_COMPILER_HAVE_GCC_ATOMIC_INTRIN
#define DeeAtomic64_Store(x,v,order)                                                    DeeAtomicGCC_Store(DEE_ATOMIC64_NATIVE_T,x,v,order)
#define DeeAtomic64_Load(x,order)                                                       DeeAtomicGCC_Load(DEE_ATOMIC64_NATIVE_T,x,order)
#define DeeAtomic64_Exchange(x,v,order)                                                 DeeAtomicGCC_Exchange(DEE_ATOMIC64_NATIVE_T,x,v,order)
#define DeeAtomic64_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      _DeeAtomic64_CompareExchangeWeak((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(old_v),(DEE_ATOMIC64_NATIVE_T)(new_v),DEE_ATOMIC_ORDER(success_order),DEE_ATOMIC_ORDER(failure_order))
#define DeeAtomic64_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    _DeeAtomic64_CompareExchangeStrong((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(old_v),(DEE_ATOMIC64_NATIVE_T)(new_v),DEE_ATOMIC_ORDER(success_order),DEE_ATOMIC_ORDER(failure_order))
#define DeeAtomic64_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   _DeeAtomic64_CompareExchangeWeakVal((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(old_v),(DEE_ATOMIC64_NATIVE_T)(new_v),DEE_ATOMIC_ORDER(success_order),DEE_ATOMIC_ORDER(failure_order))
#define DeeAtomic64_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) _DeeAtomic64_CompareExchangeStrongVal((DEE_ATOMIC64_NATIVE_T volatile *)&(x),(DEE_ATOMIC64_NATIVE_T)(old_v),(DEE_ATOMIC64_NATIVE_T)(new_v),DEE_ATOMIC_ORDER(success_order),DEE_ATOMIC_ORDER(failure_order))
#elif DEE_CONFIG_COMPILER_HAVE_GCC_SYNC_INTRIN
#define DeeAtomic64_Store(x,v,order)                                                    _DeeAtomic64_Store((DEE_ATOMIC64_NATIVE_T *)&(x),(DEE_ATOMIC64_NATIVE_T)(v))
#define DeeAtomic64_Load(x,order)                                                       _DeeAtomic64_Load((DEE_ATOMIC64_NATIVE_T *)&(x))
#define DeeAtomic64_Exchange(x,v,order)                                                 _DeeAtomic64_Exchange((DEE_ATOMIC64_NATIVE_T *)&(x),(DEE_ATOMIC64_NATIVE_T)(v))
#define DeeAtomic64_CompareExchangeWeak(x,old_v,new_v,success_order,failure_order)      DeeAtomicGCC_CompareExchangeWeak(DEE_ATOMIC64_NATIVE_T,x,old_v,new_v,success_order,failure_order)
#define DeeAtomic64_CompareExchangeStrong(x,old_v,new_v,success_order,failure_order)    DeeAtomicGCC_CompareExchangeStrong(DEE_ATOMIC64_NATIVE_T,x,old_v,new_v,success_order,failure_order)
#define DeeAtomic64_CompareExchangeWeakVal(x,old_v,new_v,success_order,failure_order)   DeeAtomicGCC_CompareExchangeWeakVal(DEE_ATOMIC64_NATIVE_T,x,old_v,new_v,success_order,failure_order)
#define DeeAtomic64_CompareExchangeStrongVal(x,old_v,new_v,success_order,failure_order) DeeAtomicGCC_CompareExchangeStrongVal(DEE_ATOMIC64_NATIVE_T,x,old_v,new_v,success_order,failure_order)
#endif
#define DeeAtomic64_FetchInc(x,order)                                                   DeeAtomicGCC_FetchInc(DEE_ATOMIC64_NATIVE_T,x,order)
#define DeeAtomic64_IncFetch(x,order)                                                   DeeAtomicGCC_IncFetch(DEE_ATOMIC64_NATIVE_T,x,order)
#define DeeAtomic64_FetchDec(x,order)                                                   DeeAtomicGCC_FetchDec(DEE_ATOMIC64_NATIVE_T,x,order)
#define DeeAtomic64_DecFetch(x,order)                                                   DeeAtomicGCC_DecFetch(DEE_ATOMIC64_NATIVE_T,x,order)
#define DeeAtomic64_FetchAdd(x,v,order)                                                 DeeAtomicGCC_FetchAdd(DEE_ATOMIC64_NATIVE_T,x,v,order)
#define DeeAtomic64_AddFetch(x,v,order)                                                 DeeAtomicGCC_AddFetch(DEE_ATOMIC64_NATIVE_T,x,v,order)
#define DeeAtomic64_FetchSub(x,v,order)                                                 DeeAtomicGCC_FetchSub(DEE_ATOMIC64_NATIVE_T,x,v,order)
#define DeeAtomic64_SubFetch(x,v,order)                                                 DeeAtomicGCC_SubFetch(DEE_ATOMIC64_NATIVE_T,x,v,order)
#define DeeAtomic64_FetchAnd(x,v,order)                                                 DeeAtomicGCC_FetchAnd(DEE_ATOMIC64_NATIVE_T,x,v,order)
#define DeeAtomic64_AndFetch(x,v,order)                                                 DeeAtomicGCC_AndFetch(DEE_ATOMIC64_NATIVE_T,x,v,order)
#define DeeAtomic64_FetchOr(x,v,order)                                                  DeeAtomicGCC_FetchOr(DEE_ATOMIC64_NATIVE_T,x,v,order)
#define DeeAtomic64_OrFetch(x,v,order)                                                  DeeAtomicGCC_OrFetch(DEE_ATOMIC64_NATIVE_T,x,v,order)
#define DeeAtomic64_FetchXor(x,v,order)                                                 DeeAtomicGCC_FetchXor(DEE_ATOMIC64_NATIVE_T,x,v,order)
#define DeeAtomic64_XorFetch(x,v,order)                                                 DeeAtomicGCC_XorFetch(DEE_ATOMIC64_NATIVE_T,x,v,order)
#define DeeAtomic64_FetchNand(x,v,order)                                                DeeAtomicGCC_FetchNand(DEE_ATOMIC64_NATIVE_T,x,v,order)
#define DeeAtomic64_NandFetch(x,v,order)                                                DeeAtomicGCC_NandFetch(DEE_ATOMIC64_NATIVE_T,x,v,order)

#endif /* !GUARD_DEEMON___ATOMIC_INTRIN_GCC_INL */
