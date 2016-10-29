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
#ifndef GUARD_DEEMON_GENERIC_VTABLE_C
#define GUARD_DEEMON_GENERIC_VTABLE_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include DEE_INCLUDE_MEMORY_API()

// /include/*
#include <deemon/bool.h>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/iterator.h>
#include <deemon/list.h>
#include <deemon/none.h>
#include <deemon/object.h>
#include <deemon/string.h>
#include <deemon/super.h>
#include <deemon/optional/hash.h>
#include <deemon/optional/std/string.h>

// /src/*
#include <deemon/runtime/builtin_functions.h>


// */ (nano...)

DEE_DECL_BEGIN


DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareVVLo(
 DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv,
 DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(lhsc) DeeObject *const *rhsv) {
 int result; do {
  if (!lhsc) return rhsc != 0;
  if (!rhsc) return 0;
  --lhsc,--rhsc;
  result = DeeObject_CompareLo(*lhsv,*rhsv);
  if (result == 0) { // *lhs < *rhs : false
   result = DeeObject_CompareLo(*rhsv,*lhsv);
   if (result == 1) return 0; // *rhs < *lhs : true
  }
  ++lhsv,++rhsv;
 } while (result == 0);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareVVLe(
 DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv,
 DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(lhsc) DeeObject *const *rhsv) {
 int result; do {
  if (!lhsc) return 1;
  if (!rhsc) return 0;
  --lhsc,--rhsc;
  result = DeeObject_CompareLo(*lhsv,*rhsv);
  if (result == 0) { // *lhs < *rhs : false
   result = DeeObject_CompareLo(*rhsv,*lhsv);
   if (result == 1) return 0; // *rhs < *lhs : true
  }
  ++lhsv,++rhsv;
 } while (result == 0);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareVVEq(
 DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv,
 DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(lhsc) DeeObject *const *rhsv) {
 int result; do {
  if (!lhsc) return rhsc == 0;
  if (!rhsc) return 0;
  --lhsc,--rhsc;
  result = DeeObject_CompareEq(*lhsv,*rhsv);
  ++lhsv,++rhsv;
 } while (result == 1);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareVVNe(
 DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv,
 DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(lhsc) DeeObject *const *rhsv) {
 int result = DeeGeneric_CompareVVEq(lhsc,lhsv,rhsc,rhsv);
 return DEE_UNLIKELY(result < 0) ? result : !result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareVVGr(
 DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv,
 DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(lhsc) DeeObject *const *rhsv) {
 int result = DeeGeneric_CompareVVLe(lhsc,lhsv,rhsc,rhsv);
 return DEE_UNLIKELY(result < 0) ? result : !result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareVVGe(
 DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv,
 DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(lhsc) DeeObject *const *rhsv) {
 int result = DeeGeneric_CompareVVLo(lhsc,lhsv,rhsc,rhsv);
 return DEE_UNLIKELY(result < 0) ? result : !result;
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareVILo(
 DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_INOUT DeeObject *rhs_iterator) {
 DeeObject *rhs_elem; int result;
 DEE_ASSERT(DeeObject_Check(rhs_iterator)); do {
  if ((result = DeeObject_IterNextEx(rhs_iterator,&rhs_elem)) != 0)
   return DEE_UNLIKELY(result < 0) ? result : 0;
  if (!lhsc--) { Dee_DECREF(rhs_elem); return 1; }
  result = DeeObject_CompareLo(*lhsv,rhs_elem);
  if (result == 0) { // *lhs < *rhs : false
   result = DeeObject_CompareLo(rhs_elem,*lhsv);
   if (result == 1) {
    Dee_DECREF(rhs_elem);
    return 0; // *rhs < *lhs : true
   }
  }
  ++lhsv; Dee_DECREF(rhs_elem);
 } while (result == 0);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareVILe(
 DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_INOUT DeeObject *rhs_iterator) {
 DeeObject *rhs_elem; int result;
 DEE_ASSERT(DeeObject_Check(rhs_iterator)); do {
  if ((result = DeeObject_IterNextEx(rhs_iterator,&rhs_elem)) != 0)
   return result < 0 ? result : lhsc == 0;
  if (!lhsc--) { Dee_DECREF(rhs_elem); return 1; }
  result = DeeObject_CompareLo(*lhsv,rhs_elem);
  if (result == 0) { // *lhs < *rhs : false
   result = DeeObject_CompareLo(rhs_elem,*lhsv);
   if (result == 1) {
    Dee_DECREF(rhs_elem);
    return 0; // *rhs < *lhs : true
   }
  }
  ++lhsv; Dee_DECREF(rhs_elem);
 } while (result == 0);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareVIEq(
 DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_INOUT DeeObject *rhs_iterator) {
 DeeObject *rhs_elem; int result;
 DEE_ASSERT(DeeObject_Check(rhs_iterator)); do {
  if ((result = DeeObject_IterNextEx(rhs_iterator,&rhs_elem)) != 0)
   return result < 0 ? result : lhsc == 0;
  if (!lhsc--) { Dee_DECREF(rhs_elem); return 0; }
  result = DeeObject_CompareEq(*lhsv,rhs_elem);
  ++lhsv; Dee_DECREF(rhs_elem);
 } while (result == 1);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareVINe(
 DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_INOUT DeeObject *rhs_iterator) {
 int result = DeeGeneric_CompareVIEq(lhsc,lhsv,rhs_iterator);
 return DEE_UNLIKELY(result < 0) ? result : !result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareVIGr(
 DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_INOUT DeeObject *rhs_iterator) {
 int result = DeeGeneric_CompareVILe(lhsc,lhsv,rhs_iterator);
 return DEE_UNLIKELY(result < 0) ? result : !result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareVIGe(
 DEE_A_IN Dee_size_t lhsc, DEE_A_IN_R(lhsc) DeeObject *const *lhsv, DEE_A_INOUT DeeObject *rhs_iterator) {
 int result = DeeGeneric_CompareVILo(lhsc,lhsv,rhs_iterator);
 return DEE_UNLIKELY(result < 0) ? result : !result;
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareIVLo(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(rhsc) DeeObject *const *rhsv) {
 DeeObject *lhs_elem; int result;
 DEE_ASSERT(DeeObject_Check(lhs_iterator)); do {
  if ((result = DeeObject_IterNextEx(lhs_iterator,&lhs_elem)) != 0)
   return result < 0 ? result : rhsc != 0;
  if (!rhsc--) { Dee_DECREF(lhs_elem); return 0; }
  result = DeeObject_CompareLo(lhs_elem,*rhsv);
  if (result == 0) { // *lhs < *rhs : false
   result = DeeObject_CompareLo(*rhsv,lhs_elem);
   if (result == 1) {
    Dee_DECREF(lhs_elem);
    return 0; // *rhs < *lhs : true
   }
  }
  Dee_DECREF(lhs_elem); ++rhsv;
 } while (result == 0);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareIVLe(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(rhsc) DeeObject *const *rhsv) {
 DeeObject *lhs_elem; int result;
 DEE_ASSERT(DeeObject_Check(lhs_iterator)); do {
  if ((result = DeeObject_IterNextEx(lhs_iterator,&lhs_elem)) != 0)
   return result < 0 ? result : 1;
  if (!rhsc--) { Dee_DECREF(lhs_elem); return 0; }
  result = DeeObject_CompareLo(lhs_elem,*rhsv);
  if (result == 0) { // *lhs < *rhs : false
   result = DeeObject_CompareLo(*rhsv,lhs_elem);
   if (result == 1) {
    Dee_DECREF(lhs_elem);
    return 0; // *rhs < *lhs : true
   }
  }
  Dee_DECREF(lhs_elem); ++rhsv;
 } while (result == 0);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareIVEq(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(rhsc) DeeObject *const *rhsv) {
 DeeObject *lhs_elem; int result;
 DEE_ASSERT(DeeObject_Check(lhs_iterator)); do {
  if ((result = DeeObject_IterNextEx(lhs_iterator,&lhs_elem)) != 0)
   return result < 0 ? result : rhsc == 0;
  if (!rhsc--) { Dee_DECREF(lhs_elem); return 0; }
  result = DeeObject_CompareEq(lhs_elem,*rhsv);
  Dee_DECREF(lhs_elem); ++rhsv;
 } while (result == 1);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareIVNe(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(rhsc) DeeObject *const *rhsv) {
 int result = DeeGeneric_CompareIVEq(lhs_iterator,rhsc,rhsv);
 return DEE_UNLIKELY(result < 0) ? result : !result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareIVGr(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(rhsc) DeeObject *const *rhsv) {
 int result = DeeGeneric_CompareIVLe(lhs_iterator,rhsc,rhsv);
 return DEE_UNLIKELY(result < 0) ? result : !result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareIVGe(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_IN Dee_size_t rhsc, DEE_A_IN_R(rhsc) DeeObject *const *rhsv) {
 int result = DeeGeneric_CompareIVLo(lhs_iterator,rhsc,rhsv);
 return DEE_UNLIKELY(result < 0) ? result : !result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareIILo(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator) {
 DeeObject *lhs_elem,*rhs_elem; int result;
 DEE_ASSERT(DeeObject_Check(lhs_iterator));
 DEE_ASSERT(DeeObject_Check(rhs_iterator));
 do {
  if ((result = DeeObject_IterNextEx(lhs_iterator,&lhs_elem)) != 0) {
   if (result < 0) return result;
   if ((result = DeeObject_IterNextEx(rhs_iterator,&rhs_elem)) != 0)
    return DEE_UNLIKELY(result < 0) ? result : 0; // size:equal
   Dee_DECREF(rhs_elem);
   return 1; // size:lower
  }
  if ((result = DeeObject_IterNextEx(rhs_iterator,&rhs_elem)) != 0) {
   Dee_DECREF(lhs_elem);
   return DEE_UNLIKELY(result < 0) ? result : 0; // size:greater
  }
  result = DeeObject_CompareLo(lhs_elem,rhs_elem);
  if (result == 0) { // *lhs < *rhs : false
   result = DeeObject_CompareLo(rhs_elem,lhs_elem);
   if (result == 1) { // *rhs < *lhs : true
    Dee_DECREF(rhs_elem);
    Dee_DECREF(lhs_elem);
    return 0; // return false
   }
  }
  Dee_DECREF(rhs_elem);
  Dee_DECREF(lhs_elem);
 } while (result == 0);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareIILe(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator) {
 DeeObject *lhs_elem,*rhs_elem; int result;
 DEE_ASSERT(DeeObject_Check(lhs_iterator));
 DEE_ASSERT(DeeObject_Check(rhs_iterator));
 do {
  if ((result = DeeObject_IterNextEx(lhs_iterator,&lhs_elem)) != 0)
   return result < 0 ? result : 1; // size:equal_or_lower
  if ((result = DeeObject_IterNextEx(rhs_iterator,&rhs_elem)) != 0) {
   Dee_DECREF(lhs_elem);
   return DEE_UNLIKELY(result < 0) ? result : 0; // size:greater
  }
  result = DeeObject_CompareLo(lhs_elem,rhs_elem);
  if (result == 0) { // *lhs < *rhs : false
   result = DeeObject_CompareLo(rhs_elem,lhs_elem);
   if (result == 1) { // *rhs < *lhs : true
    Dee_DECREF(rhs_elem);
    Dee_DECREF(lhs_elem);
    return 0; // return false
   }
  }
  Dee_DECREF(rhs_elem);
  Dee_DECREF(lhs_elem);
 } while (result == 0);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareIIEq(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator) {
 DeeObject *lhs_elem,*rhs_elem; int result;
 DEE_ASSERT(DeeObject_Check(lhs_iterator));
 DEE_ASSERT(DeeObject_Check(rhs_iterator));
 do {
  if ((result = DeeObject_IterNextEx(lhs_iterator,&lhs_elem)) != 0) {
   if (result < 0) return result;
   if ((result = DeeObject_IterNextEx(rhs_iterator,&rhs_elem)) != 0)
    return result < 0 ? result : 1; // size:equal
   Dee_DECREF(rhs_elem);
   return 0; // size:lower
  }
  if ((result = DeeObject_IterNextEx(rhs_iterator,&rhs_elem)) != 0) {
   Dee_DECREF(lhs_elem);
   return DEE_UNLIKELY(result < 0) ? result : 0; // size:greater
  }
  result = DeeObject_CompareEq(lhs_elem,rhs_elem);
  Dee_DECREF(rhs_elem);
  Dee_DECREF(lhs_elem);
 } while (result == 1);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareIINe(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator) {
 int result = DeeGeneric_CompareIIEq(lhs_iterator,rhs_iterator);
 return DEE_UNLIKELY(result < 0) ? result : !result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareIIGr(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator) {
 int result = DeeGeneric_CompareIILe(lhs_iterator,rhs_iterator);
 return DEE_UNLIKELY(result < 0) ? result : !result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_CompareIIGe(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator) {
 int result = DeeGeneric_CompareIILo(lhs_iterator,rhs_iterator);
 return DEE_UNLIKELY(result < 0) ? result : !result;
}

DEE_A_RET_EXCEPT(-1) int _DeeGeneric_MergeSort(
 DEE_A_IN Dee_size_t c,
 DEE_A_ON_ALWAYS(DEE_A_IN_R(c)) DeeObject *const *in_v,
 DEE_A_ON_ALWAYS(DEE_A_OUT_W(c)) DeeObject **out_v,
 DEE_A_IN _DeeGeneric_MergeSortPred pred_lo, void *closure) {
 int temp;
 DEE_ASSERT(in_v != out_v && pred_lo);
 switch (c) {
  case 0: break; // Shouldn't happen.
  case 1: *out_v = *in_v; break;
  case 2: // if (b < a)
   temp = (*pred_lo)(in_v[1],in_v[0],closure);
   if (temp < 0) {
    if (!DeeError_Catch(&DeeErrorType_NotImplemented)) return -1;
#if 1
    goto dont_swap;
#else
    temp = 0;
#endif
   }
   if (temp) {
    out_v[0] = in_v[1];
    out_v[1] = in_v[0];
   } else {
dont_swap:
    out_v[0] = in_v[0];
    out_v[1] = in_v[1];
   }
   break;
  default: {
   Dee_size_t s1 = c/2,s2 = c-s1;
   DeeObject **buf1,**buf2,**iter1,**iter2;
   while ((buf1 = (DeeObject **)mallocf_nz(s1*sizeof(DeeObject*),
    "mergesort - left buffer (%u objects)",(unsigned)s1)) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
no_mem: DeeError_NoMemory();
    memcpy(out_v,in_v,c*sizeof(DeeObject *));
    return -1;
   }
   while ((buf2 = (DeeObject **)mallocf_nz(s2*sizeof(DeeObject*),
    "mergesort - right buffer (%u objects)",(unsigned)s2)) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    free_nn(buf1);
    goto no_mem;
   }
   if (_DeeGeneric_MergeSort(s1,in_v,buf1,pred_lo,closure) != 0) {
    memcpy(out_v,buf1,s1*sizeof(DeeObject *));
    memcpy(out_v+s1,in_v+s1,s2*sizeof(DeeObject *));
    goto err;
   }
   if (_DeeGeneric_MergeSort(s2,in_v+s1,buf2,pred_lo,closure) != 0) {
    memcpy(out_v,buf1,s1*sizeof(DeeObject *));
    memcpy(out_v+s1,buf2,s2*sizeof(DeeObject *));
    goto err;
   }
   iter1 = buf1,iter2 = buf2;
   while (s1 && s2) {
    temp = (*pred_lo)(*iter2,*iter1,closure);
    if (temp < 0) {
     if (DeeError_Catch(&DeeErrorType_NotImplemented)) temp = 0;
     else {
      memcpy(out_v,iter1,s1*sizeof(DeeObject *));
      memcpy(out_v+s1,iter2,s2*sizeof(DeeObject *));
err:
      free_nn(buf2);
      free_nn(buf1);
      return -1;
     }
    }
    if (temp) { // if (b < a)
     *out_v++ = *iter2++,--s2;
    } else {
     *out_v++ = *iter1++,--s1;
    }
   }
   // copy remainder
   if (s1) {
    DEE_ASSERT(s2 == 0);
    memcpy(out_v,iter1,s1*sizeof(DeeObject *));
   } else if (s2) {
    memcpy(out_v,iter2,s2*sizeof(DeeObject *));
   }
   free_nn(buf2);
   free_nn(buf1);
  } break;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int _DeeGeneric_SortPredCmpLo(
 DEE_A_INOUT DeeObject *a, DEE_A_INOUT DeeObject *b, void *DEE_UNUSED(closure)) {
 DEE_ASSERT(DeeObject_Check(a));
 DEE_ASSERT(DeeObject_Check(b));
 return DeeObject_CompareLo(a,b);
}
DEE_A_RET_EXCEPT(-1) int _DeeGeneric_SortPredUserFunc(
 DEE_A_INOUT DeeObject *a, DEE_A_INOUT DeeObject *b, void *closure) {
 DeeObject *result_ob,*args; int result;
 DEE_ASSERT(DeeObject_Check(a));
 DEE_ASSERT(DeeObject_Check(b));
 DEE_ASSERT(DeeObject_Check(closure));
 if ((args = DeeTuple_Pack(2,a,b)) == NULL) return -1;
 result_ob = DeeObject_Call((DeeObject *)closure,args);
 Dee_DECREF(args);
 if (!result_ob) return -1;
 result = DeeObject_Bool(result_ob);
 Dee_DECREF(result_ob);
 return result;
}



int _deegenericsameob_tp_hash(DeeObject *self, Dee_hash_t start, Dee_hash_t *result) {
#if DEE_CONFIG_SIZEOF_DEE_HASH_T >= DEE_TYPES_SIZEOF_POINTER
 *result = start ^ (Dee_uintptr_t)self;
#elif DEE_CONFIG_SIZEOF_DEE_HASH_T == DEE_TYPES_SIZEOF_POINTER*2
 *result = start ^ ((Dee_uintptr_t *)&self)[0] ^ ((Dee_uintptr_t *)&self)[1];
#elif DEE_CONFIG_SIZEOF_DEE_HASH_T == DEE_TYPES_SIZEOF_POINTER*4
 *result = start ^ ((Dee_uintptr_t *)&self)[0] ^ ((Dee_uintptr_t *)&self)[1] ^
                 ^ ((Dee_uintptr_t *)&self)[2] ^ ((Dee_uintptr_t *)&self)[3];
#else
#error "Invalid/Unsupported relation between 'DEE_CONFIG_SIZEOF_DEE_HASH_T' and 'DEE_TYPES_SIZEOF_POINTER'"
#endif
 return 0;
}
DeeObject *_deegenericsameob_tp_cmp_lo(DeeObject *lhs, DeeObject *rhs) { while DEE_UNLIKELY(DeeSuper_Check(rhs)) rhs = DeeSuper_SELF(rhs); DeeReturn_Bool(lhs < rhs); }
DeeObject *_deegenericsameob_tp_cmp_le(DeeObject *lhs, DeeObject *rhs) { while DEE_UNLIKELY(DeeSuper_Check(rhs)) rhs = DeeSuper_SELF(rhs); DeeReturn_Bool(lhs <= rhs); }
DeeObject *_deegenericsameob_tp_cmp_eq(DeeObject *lhs, DeeObject *rhs) { while DEE_UNLIKELY(DeeSuper_Check(rhs)) rhs = DeeSuper_SELF(rhs); DeeReturn_Bool(lhs == rhs); }
DeeObject *_deegenericsameob_tp_cmp_ne(DeeObject *lhs, DeeObject *rhs) { while DEE_UNLIKELY(DeeSuper_Check(rhs)) rhs = DeeSuper_SELF(rhs); DeeReturn_Bool(lhs != rhs); }
DeeObject *_deegenericsameob_tp_cmp_gr(DeeObject *lhs, DeeObject *rhs) { while DEE_UNLIKELY(DeeSuper_Check(rhs)) rhs = DeeSuper_SELF(rhs); DeeReturn_Bool(lhs > rhs); }
DeeObject *_deegenericsameob_tp_cmp_ge(DeeObject *lhs, DeeObject *rhs) { while DEE_UNLIKELY(DeeSuper_Check(rhs)) rhs = DeeSuper_SELF(rhs); DeeReturn_Bool(lhs >= rhs); }
int _deegenericmemcmp_tp_hash(DeeObject *self, Dee_hash_t start, Dee_hash_t *result) {
 DEE_ASSERT(!DeeObject_IS_VAR(self));
 *result = DeeHash_Memory(((DeeObject *)self)+1,DeeType_GET_SLOT(
  Dee_TYPE(self),tp_instance_size)-sizeof(struct DeeObject),start);
 return 0;
}
DeeObject *_deegenericmemcmp_tp_cmp_lo(DeeObject *lhs, DeeObject *rhs) { DEE_ASSERT(!DeeObject_IS_VAR(lhs)); if DEE_UNLIKELY((rhs = DeeObject_GetInstance(rhs,Dee_TYPE(lhs))) == NULL) return NULL; DeeReturn_Bool(memcmp(((DeeObject *)lhs)+1,((DeeObject *)rhs)+1,DeeType_GET_SLOT(Dee_TYPE(lhs),tp_instance_size)-sizeof(DeeObject)) <  0); }
DeeObject *_deegenericmemcmp_tp_cmp_le(DeeObject *lhs, DeeObject *rhs) { DEE_ASSERT(!DeeObject_IS_VAR(lhs)); if DEE_UNLIKELY((rhs = DeeObject_GetInstance(rhs,Dee_TYPE(lhs))) == NULL) return NULL; DeeReturn_Bool(memcmp(((DeeObject *)lhs)+1,((DeeObject *)rhs)+1,DeeType_GET_SLOT(Dee_TYPE(lhs),tp_instance_size)-sizeof(DeeObject)) <= 0); }
DeeObject *_deegenericmemcmp_tp_cmp_eq(DeeObject *lhs, DeeObject *rhs) { DEE_ASSERT(!DeeObject_IS_VAR(lhs)); if DEE_UNLIKELY((rhs = DeeObject_GetInstance(rhs,Dee_TYPE(lhs))) == NULL) return NULL; DeeReturn_Bool(memcmp(((DeeObject *)lhs)+1,((DeeObject *)rhs)+1,DeeType_GET_SLOT(Dee_TYPE(lhs),tp_instance_size)-sizeof(DeeObject)) == 0); }
DeeObject *_deegenericmemcmp_tp_cmp_ne(DeeObject *lhs, DeeObject *rhs) { DEE_ASSERT(!DeeObject_IS_VAR(lhs)); if DEE_UNLIKELY((rhs = DeeObject_GetInstance(rhs,Dee_TYPE(lhs))) == NULL) return NULL; DeeReturn_Bool(memcmp(((DeeObject *)lhs)+1,((DeeObject *)rhs)+1,DeeType_GET_SLOT(Dee_TYPE(lhs),tp_instance_size)-sizeof(DeeObject)) != 0); }
DeeObject *_deegenericmemcmp_tp_cmp_gr(DeeObject *lhs, DeeObject *rhs) { DEE_ASSERT(!DeeObject_IS_VAR(lhs)); if DEE_UNLIKELY((rhs = DeeObject_GetInstance(rhs,Dee_TYPE(lhs))) == NULL) return NULL; DeeReturn_Bool(memcmp(((DeeObject *)lhs)+1,((DeeObject *)rhs)+1,DeeType_GET_SLOT(Dee_TYPE(lhs),tp_instance_size)-sizeof(DeeObject)) >  0); }
DeeObject *_deegenericmemcmp_tp_cmp_ge(DeeObject *lhs, DeeObject *rhs) { DEE_ASSERT(!DeeObject_IS_VAR(lhs)); if DEE_UNLIKELY((rhs = DeeObject_GetInstance(rhs,Dee_TYPE(lhs))) == NULL) return NULL; DeeReturn_Bool(memcmp(((DeeObject *)lhs)+1,((DeeObject *)rhs)+1,DeeType_GET_SLOT(Dee_TYPE(lhs),tp_instance_size)-sizeof(DeeObject)) >= 0); }

DEE_STATIC_INLINE(int) _deegenericiterable_cmp_lo(DeeObject *lhs, DeeObject *rhs) {
 DeeObject *lhs_iterator,*rhs_iterator; int result;
 DEE_ASSERT(DeeObject_Check(lhs) && DeeObject_Check(rhs));
 if DEE_UNLIKELY((lhs_iterator = DeeObject_IterSelf(lhs)) == NULL) return -1;
 if DEE_UNLIKELY((rhs_iterator = DeeObject_IterSelf(rhs)) == NULL) { Dee_DECREF(lhs_iterator); return -1; }
 result = DeeGeneric_CompareIILo(lhs_iterator,rhs_iterator);
 Dee_DECREF(rhs_iterator);
 Dee_DECREF(lhs_iterator);
 return result;
}
DEE_STATIC_INLINE(int) _deegenericiterable_cmp_le(DeeObject *lhs, DeeObject *rhs) {
 DeeObject *lhs_iterator,*rhs_iterator; int result;
 DEE_ASSERT(DeeObject_Check(lhs) && DeeObject_Check(rhs));
 if DEE_UNLIKELY((lhs_iterator = DeeObject_IterSelf(lhs)) == NULL) return -1;
 if DEE_UNLIKELY((rhs_iterator = DeeObject_IterSelf(rhs)) == NULL) { Dee_DECREF(lhs_iterator); return -1; }
 result = DeeGeneric_CompareIILe(lhs_iterator,rhs_iterator);
 Dee_DECREF(rhs_iterator);
 Dee_DECREF(lhs_iterator);
 return result;
}
DEE_STATIC_INLINE(int) _deegenericiterable_cmp_eq(DeeObject *lhs, DeeObject *rhs) {
 DeeObject *lhs_iterator,*rhs_iterator; int result;
 DEE_ASSERT(DeeObject_Check(lhs) && DeeObject_Check(rhs));
 if DEE_UNLIKELY((lhs_iterator = DeeObject_IterSelf(lhs)) == NULL) return -1;
 if DEE_UNLIKELY((rhs_iterator = DeeObject_IterSelf(rhs)) == NULL) { Dee_DECREF(lhs_iterator); return -1; }
 result = DeeGeneric_CompareIIEq(lhs_iterator,rhs_iterator);
 Dee_DECREF(rhs_iterator);
 Dee_DECREF(lhs_iterator);
 return result;
}

DeeObject *_deegenericiterable_tp_cmp_lo(DeeObject *lhs, DeeObject *rhs) { int res; if DEE_UNLIKELY((res = _deegenericiterable_cmp_lo(lhs,rhs)) < 0) return NULL; DeeReturn_Bool(res); }
DeeObject *_deegenericiterable_tp_cmp_le(DeeObject *lhs, DeeObject *rhs) { int res; if DEE_UNLIKELY((res = _deegenericiterable_cmp_le(lhs,rhs)) < 0) return NULL; DeeReturn_Bool(res); }
DeeObject *_deegenericiterable_tp_cmp_eq(DeeObject *lhs, DeeObject *rhs) { int res; if DEE_UNLIKELY((res = _deegenericiterable_cmp_eq(lhs,rhs)) < 0) return NULL; DeeReturn_Bool(res); }
DeeObject *_deegenericiterable_tp_cmp_ne(DeeObject *lhs, DeeObject *rhs) { int res; if DEE_UNLIKELY((res = _deegenericiterable_cmp_eq(lhs,rhs)) < 0) return NULL; DeeReturn_Bool(!res); }
DeeObject *_deegenericiterable_tp_cmp_gr(DeeObject *lhs, DeeObject *rhs) { int res; if DEE_UNLIKELY((res = _deegenericiterable_cmp_le(lhs,rhs)) < 0) return NULL; DeeReturn_Bool(!res); }
DeeObject *_deegenericiterable_tp_cmp_ge(DeeObject *lhs, DeeObject *rhs) { int res; if DEE_UNLIKELY((res = _deegenericiterable_cmp_lo(lhs,rhs)) < 0) return NULL; DeeReturn_Bool(!res); }
DeeObject *_deegenericiterable_tp_seq_size(DeeObject *self) {
 DeeObject *elem,*iterator; int error; Dee_size_t result = 0;
 if DEE_UNLIKELY((iterator = DeeObject_IterSelf(self)) == NULL) return NULL;
 while (1) {
  if DEE_UNLIKELY((error = DeeObject_IterNextEx(iterator,&elem)) != 0) {
   if DEE_UNLIKELY(error < 0) { Dee_DECREF(iterator); return NULL; }
   break;
  }
  ++result;
  Dee_DECREF(elem);
 }
 Dee_DECREF(iterator);
 return DeeObject_New(Dee_size_t,result);
}
DeeObject *_deegenericiterable_tp_seq_get(DeeObject *self, DeeObject *i) {
 Dee_ssize_t ii;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,i,&ii) != 0) return NULL;
 return DeeGeneric_IterableGet(self,ii);
}
DeeObject *_deegenericiterable_tp_seq_range_get(DeeObject *self, DeeObject *lo, DeeObject *hi) {
 DeeObject *result; Dee_size_t ilo,ihi;
 if (DeeNone_Check(lo)) ilo = 0;
 else if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,lo,&ilo) != 0) return NULL;
 if (DeeNone_Check(hi)) ihi = (Dee_size_t)-1;
 else if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,hi,&ihi) != 0) return NULL;
 if DEE_UNLIKELY((self = DeeObject_IterSelf(self)) == NULL) return NULL;
 result = DeeSubRangeIterator_New(self,ilo,ihi);
 Dee_DECREF(self);
 return result;
}

DeeObject *_deegenericiterable_tp_seq_contains(DeeObject *self, DeeObject *ob) {
 int result;
 if DEE_UNLIKELY((result = DeeGeneric_IterableContains(self,ob)) < 0) return NULL;
 DeeReturn_Bool(result);
}

DeeObject *_deegenericiterable_at(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t index;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu:at",&index) != 0) return NULL;
 return DeeGeneric_IterableAt(self,index);
}
DeeObject *_deegenericiterable_find(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *findob,*pred = Dee_None; Dee_ssize_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:find",&findob,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = (DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeGeneric_IterableFind(self,findob)
  : DeeGeneric_IterableFindPred(self,findob,pred))) == (Dee_ssize_t)-2) return NULL;
 return DeeObject_New(Dee_ssize_t,result);
}
DeeObject *_deegenericiterable_rfind(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *findob,*pred = Dee_None; Dee_ssize_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:rfind",&findob,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = (DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeGeneric_IterableRFind(self,findob)
  : DeeGeneric_IterableRFindPred(self,findob,pred))) == (Dee_ssize_t)-2) return NULL;
 return DeeObject_New(Dee_ssize_t,result);
}
DeeObject *_deegenericiterable_index(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *findob,*pred = Dee_None; Dee_size_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:index",&findob,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = (DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeGeneric_IterableIndex(self,findob)
  : DeeGeneric_IterableIndexPred(self,findob,pred))) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
DeeObject *_deegenericiterable_rindex(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *findob,*pred = Dee_None; Dee_size_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:rindex",&findob,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = (DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeGeneric_IterableRIndex(self,findob)
  : DeeGeneric_IterableRIndexPred(self,findob,pred))) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
DeeObject *_deegenericiterable_count(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *findob,*pred; Dee_size_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:count",&findob,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = (DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeGeneric_IterableCount(self,findob)
  : DeeGeneric_IterableCountPred(self,findob,pred))) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
DeeObject *_deegenericiterable_locate(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:locate",&pred) != 0) return NULL;
 return DeeGeneric_IterableLocate(self,pred);
}
DeeObject *_deegenericiterable_rlocate(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:rlocate",&pred) != 0) return NULL;
 return DeeGeneric_IterableRLocate(self,pred);
}
DeeObject *_deegenericiterable_locate_all(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:locate_all",&pred) != 0) return NULL;
 return DeeGeneric_IterableLocateAll(self,pred);
}
DeeObject *_deegenericiterable_transform(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:transform",&pred) != 0) return NULL;
 return DeeGeneric_IterableTransform(self,pred);
}
DeeObject *_deegenericiterable_segments(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_size_t n;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu:segments",&n) != 0) return NULL;
 return DeeGeneric_IterableSegments(self,n);
}
DeeObject *_deegenericiterable_contains(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *elem,*pred = Dee_None; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:contains",&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeGeneric_IterableContains(self,elem)
  : DeeGeneric_IterableContainsPred(self,elem,pred)) < 0) return NULL;
 DeeReturn_Bool(result);
}
DeeObject *_deegenericiterable_empty(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":empty") != 0) return NULL;
 if DEE_UNLIKELY((result = DeeGeneric_IterableEmpty(self)) < 0) return NULL;
 DeeReturn_Bool(result);
}
DeeObject *_deegenericiterable_non_empty(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":non_empty") != 0) return NULL;
 if DEE_UNLIKELY((result = DeeGeneric_IterableNonEmpty(self)) < 0) return NULL;
 DeeReturn_Bool(result);
}
DeeObject *_deegenericiterable_front(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":front") != 0) return NULL;
 return DeeGeneric_IterableFront(self);
}
DeeObject *_deegenericiterable_back(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":back") != 0) return NULL;
 return DeeGeneric_IterableBack(self);
}
DeeObject *_deegenericiterable_reversed(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":reversed") != 0) return NULL;
 return DeeGeneric_IterableReversed(self);
}
DeeObject *_deegenericiterable_sorted(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *pred = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:sorted",&pred) != 0) return NULL;
 return DEE_LIKELY(DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___lo__)
  ? DeeGeneric_IterableSorted(self)
  : DeeGeneric_IterableSortedPred(self,pred);
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeGeneric_MergeIterators(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator) {
 DeeObject *result,*lhs_elem,*rhs_elem; int temp;
 DEE_ASSERT(DeeObject_Check(lhs_iterator));
 DEE_ASSERT(DeeObject_Check(rhs_iterator));
 if DEE_UNLIKELY((temp = DeeObject_IterNextEx(lhs_iterator,&lhs_elem)) != 0)
  return DEE_UNLIKELY(temp < 0) ? NULL : DeeList_NewFromIterator(rhs_iterator);
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) return NULL;
 if DEE_UNLIKELY((temp = DeeObject_IterNextEx(rhs_iterator,&rhs_elem)) != 0) {
append_lhs_end:
  if DEE_UNLIKELY(temp < 0 || DeeList_Append(result,lhs_elem) != 0) {
err_lhs_r:
   Dee_DECREF(lhs_elem);
err_r:
   Dee_DECREF(result);
   return NULL;
  }
  Dee_DECREF(lhs_elem);
  if DEE_UNLIKELY(DeeList_AppendIterator(result,lhs_iterator) != 0) Dee_CLEAR(result);
  return result;
 }
 while (1) {
  temp = DeeObject_CompareLo(lhs_elem,rhs_elem);
  if DEE_UNLIKELY(temp < 0) {
   if DEE_UNLIKELY(DeeError_Catch(&DeeErrorType_NotImplemented)) temp = 0;
   else {err_rhs_lhs_r: Dee_DECREF(rhs_elem); goto err_lhs_r; }
  }
  if (temp) { // lhs < rhs --> append lhs
   if DEE_UNLIKELY(DeeList_Append(result,lhs_elem) != 0) goto err_rhs_lhs_r;
   Dee_DECREF(lhs_elem);
   if DEE_UNLIKELY((temp = DeeObject_IterNextEx(lhs_iterator,&lhs_elem)) != 0) {
    if DEE_UNLIKELY(temp < 0) {err_rhs_r: Dee_DECREF(rhs_elem); goto err_r; }
    if DEE_UNLIKELY(DeeList_Append(result,rhs_elem) != 0) goto err_rhs_r;
    Dee_DECREF(rhs_elem);
    if DEE_UNLIKELY(DeeList_AppendIterator(result,rhs_iterator) != 0) Dee_CLEAR(result);
    return result;
   }
  } else { // lhs > rhs --> append rhs
   if DEE_UNLIKELY(DeeList_Append(result,rhs_elem) != 0) goto err_rhs_lhs_r;
   Dee_DECREF(rhs_elem);
   if DEE_UNLIKELY((temp = DeeObject_IterNextEx(rhs_iterator,&rhs_elem)) != 0) goto append_lhs_end;
  }
 }
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeGeneric_MergeIteratorsPred(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator, DEE_A_INOUT DeeObject *pred) {
 DeeObject *result,*lhs_elem,*rhs_elem,*pred_args,*pred_result; int temp;
 DEE_ASSERT(DeeObject_Check(lhs_iterator));
 DEE_ASSERT(DeeObject_Check(rhs_iterator));
 DEE_ASSERT(DeeObject_Check(pred));
 if DEE_UNLIKELY((temp = DeeObject_IterNextEx(lhs_iterator,&lhs_elem)) != 0)
  return DEE_UNLIKELY(temp < 0) ? NULL : DeeList_NewFromIterator(rhs_iterator);
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) return NULL;
 if DEE_UNLIKELY((temp = DeeObject_IterNextEx(rhs_iterator,&rhs_elem)) != 0) {
append_lhs_end:
  if DEE_UNLIKELY(temp < 0 || DeeList_Append(result,lhs_elem) != 0) {
err_lhs_r:
   Dee_DECREF(lhs_elem);
err_r:
   Dee_DECREF(result);
   return NULL;
  }
  Dee_DECREF(lhs_elem);
  if DEE_UNLIKELY(DeeList_AppendIterator(result,lhs_iterator) != 0) Dee_CLEAR(result);
  return result;
 }
 while (1) {
  if DEE_UNLIKELY((pred_args = DeeTuple_Pack(2,lhs_elem,rhs_elem)) == NULL) goto err_rhs_lhs_r;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) goto check_ni;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if DEE_UNLIKELY(temp < 0) {
check_ni:
   if DEE_UNLIKELY(DeeError_Catch(&DeeErrorType_NotImplemented)) temp = 0;
   else {err_rhs_lhs_r: Dee_DECREF(rhs_elem); goto err_lhs_r; }
  }
  if (temp) { // lhs < rhs --> append lhs
   if DEE_UNLIKELY(DeeList_Append(result,lhs_elem) != 0) goto err_rhs_lhs_r;
   Dee_DECREF(lhs_elem);
   if DEE_UNLIKELY((temp = DeeObject_IterNextEx(lhs_iterator,&lhs_elem)) != 0) {
    if DEE_UNLIKELY(temp < 0) {err_rhs_r: Dee_DECREF(rhs_elem); goto err_r; }
    if DEE_UNLIKELY(DeeList_Append(result,rhs_elem) != 0) goto err_rhs_r;
    Dee_DECREF(rhs_elem);
    if DEE_UNLIKELY(DeeList_AppendIterator(result,rhs_iterator) != 0) Dee_CLEAR(result);
    return result;
   }
  } else { // lhs > rhs --> append rhs
   if DEE_UNLIKELY(DeeList_Append(result,rhs_elem) != 0) goto err_rhs_lhs_r;
   Dee_DECREF(rhs_elem);
   if DEE_UNLIKELY((temp = DeeObject_IterNextEx(rhs_iterator,&rhs_elem)) != 0) goto append_lhs_end;
  }
 }
}

DEE_A_RET_EXCEPT_REF DeeObject *DeeGeneric_CommonIterators(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator) {
 DeeObject *result,*rhs_list,*elem; int temp;
 DEE_ASSERT(DeeObject_Check(lhs_iterator));
 DEE_ASSERT(DeeObject_Check(rhs_iterator));
 if DEE_UNLIKELY((rhs_list = DeeList_NewFromIterator(rhs_iterator)) == NULL) return NULL;
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) {err_rhsl: Dee_DECREF(rhs_list); return NULL; }
 while (1) {
  if DEE_UNLIKELY((temp = DeeObject_IterNextEx(lhs_iterator,&elem)) != 0) {
   if DEE_UNLIKELY(temp < 0) Dee_CLEAR(result);
   break;
  }
  if ((temp = DeeList_Contains(rhs_list,elem)) != 0) {
   if DEE_UNLIKELY(temp < 0) {
    Dee_DECREF(elem);
err_rhsl_r:
    Dee_DECREF(result);
    goto err_rhsl;
   } else {
    DEE_ASSERT(temp > 0);
    temp = DeeList_Append(result,elem);
    Dee_DECREF(elem);
    if DEE_UNLIKELY(temp != 0) goto err_rhsl_r;
   }
  } else Dee_DECREF(elem);
 }
 Dee_DECREF(rhs_list);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeGeneric_CommonIteratorsPred(
 DEE_A_INOUT DeeObject *lhs_iterator, DEE_A_INOUT DeeObject *rhs_iterator, DEE_A_INOUT DeeObject *pred) {
 DeeObject *result,*rhs_list,*elem; int temp;
 DEE_ASSERT(DeeObject_Check(lhs_iterator));
 DEE_ASSERT(DeeObject_Check(rhs_iterator));
 DEE_ASSERT(DeeObject_Check(pred));
 if DEE_UNLIKELY((rhs_list = DeeList_NewFromIterator(rhs_iterator)) == NULL) return NULL;
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) {err_rhsl: Dee_DECREF(rhs_list); return NULL; }
 while (1) {
  if DEE_UNLIKELY((temp = DeeObject_IterNextEx(lhs_iterator,&elem)) != 0) {
   if DEE_UNLIKELY(temp < 0) Dee_CLEAR(result);
   break;
  }
  if ((temp = DeeList_ContainsPred(rhs_list,elem,pred)) != 0) {
   if DEE_UNLIKELY(temp < 0) {
    Dee_DECREF(elem);
err_rhsl_r:
    Dee_DECREF(result);
    goto err_rhsl;
   } else {
    DEE_ASSERT(temp > 0);
    temp = DeeList_Append(result,elem);
    Dee_DECREF(elem);
    if DEE_UNLIKELY(temp != 0) goto err_rhsl_r;
   }
  } else Dee_DECREF(elem);
 }
 Dee_DECREF(rhs_list);
 return result;
}




DEE_A_RET_EXCEPT_REF DeeObject *DeeGeneric_TIterableGet(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_ssize_t idx) {
 DeeObject *elem,*iterator; Dee_size_t used_index,size; int error;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if DEE_UNLIKELY(idx < 0) {
  if DEE_UNLIKELY((iterator = DeeList_TCast(tp_self,self)) == NULL) return NULL;
  elem = DeeList_Get(iterator,idx);
  Dee_DECREF(iterator);
  return elem;
 }
 used_index = (Dee_size_t)idx;
again: size = 0;
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return NULL;
 while (1) {
  if DEE_UNLIKELY((error = DeeObject_IterNextEx(iterator,&elem)) != 0) {
   Dee_DECREF(iterator);
   if DEE_UNLIKELY(error < 0) return NULL;
   if DEE_LIKELY(size) {
    // Wrap the index to the size and try again
    used_index = (Dee_size_t)idx % size;
    goto again;
   } else {
    // Empty iterable (can't access elements)
    DeeError_ValueError_EmptyList(tp_self);
    return NULL;
   }
  }
  if DEE_UNLIKELY(!used_index) break; // found the element at the given index
  ++size,--used_index;
  Dee_DECREF(elem);
 }
 Dee_DECREF(iterator);
 return elem;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeGeneric_TIterableAt(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t idx) {
 DeeObject *iterator,*elem; Dee_size_t iend; int error;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return NULL;
 iend = idx; while (1) {
  if DEE_UNLIKELY((error = DeeObject_IterNextEx(iterator,&elem)) != 0) {
   if DEE_UNLIKELY(error > 0) DeeError_IndexError((DeeObject *)self,idx);
   break;
  }
  if (!iend) return elem; // Inherit reference
  Dee_DECREF(elem); --iend;
 }
 return NULL;
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeGeneric_TIterableFind(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) {
 DeeObject *iterator,*elem; Dee_size_t result; int temp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return (Dee_ssize_t)-2;
 result = 0; while (1) {
  if DEE_UNLIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) != 0) {
   Dee_DECREF(iterator);
   return DEE_UNLIKELY(temp < 0) ? (Dee_ssize_t)-2 : (Dee_ssize_t)-1;
  }
  temp = DeeObject_CompareEq(elem,findob);
  Dee_DECREF(elem);
  if (temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) {
     Dee_DECREF(iterator);
     return (Dee_ssize_t)-2;
    }
   } else {
    DEE_ASSERT(temp > 0);
    Dee_DECREF(iterator);
    return (Dee_ssize_t)result;
   }
  }
  ++result;
 }
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeGeneric_TIterableFindPred(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) {
 DeeObject *iterator,*elem,*pred_result,*pred_args; Dee_size_t result; int temp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 DEE_ASSERT(DeeObject_Check(pred));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return (Dee_ssize_t)-2;
 result = 0;
 while DEE_LIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  pred_args = DeeTuple_Pack(2,elem,findob);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(!pred_args) goto err_iter;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) goto check_ni;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if (temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
check_ni:
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) {
err_iter: Dee_DECREF(iterator); return (Dee_ssize_t)-2;
    }
   } else {
    DEE_ASSERT(temp > 0);
    Dee_DECREF(iterator);
    return (Dee_ssize_t)result;
   }
  }
  ++result;
 }
 Dee_DECREF(iterator);
 return DEE_UNLIKELY(temp < 0) ? (Dee_ssize_t)-2 : (Dee_ssize_t)-1;
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeGeneric_TIterableRFind(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) {
 DeeObject *iterator,*elem; Dee_ssize_t result; Dee_size_t pos; int temp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return (Dee_ssize_t)-2;
 result = -1,pos = 0;
 while DEE_LIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  temp = DeeObject_CompareEq(elem,findob);
  Dee_DECREF(elem);
  if (temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) {
     Dee_DECREF(iterator);
     return (Dee_ssize_t)-2;
    }
   } else {
    DEE_ASSERT(temp > 0);
    result = (Dee_ssize_t)pos;
   }
  }
  ++pos;
 }
 Dee_DECREF(iterator);
 return DEE_UNLIKELY(temp < 0) ? (Dee_ssize_t)-2 : result;
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeGeneric_TIterableRFindPred(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) {
 DeeObject *iterator,*elem,*pred_result,*pred_args; Dee_ssize_t result; Dee_size_t pos; int temp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 DEE_ASSERT(DeeObject_Check(pred));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return (Dee_ssize_t)-2;
 result = -1,pos = 0;
 while DEE_LIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  pred_args = DeeTuple_Pack(2,elem,findob);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(!pred_args) goto err_iter;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) goto check_ni;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if (temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
check_ni:
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) {
err_iter: Dee_DECREF(iterator); return (Dee_ssize_t)-2;
    }
   } else {
    DEE_ASSERT(temp > 0);
    result = (Dee_ssize_t)pos;
   }
  }
  ++pos;
 }
 Dee_DECREF(iterator);
 return temp < 0 ? (Dee_ssize_t)-2 : result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeGeneric_TIterableIndex(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) {
 DeeObject *iterator,*elem; Dee_size_t result; int temp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return (Dee_size_t)-1;
 result = 0;
 while DEE_LIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  temp = DeeObject_CompareEq(elem,findob);
  Dee_DECREF(elem);
  if (temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) {
     Dee_DECREF(iterator);
     return (Dee_size_t)-1;
    }
   } else {
    DEE_ASSERT(temp > 0);
    Dee_DECREF(iterator);
    return result;
   }
  }
  ++result;
 }
 Dee_DECREF(iterator);
 if DEE_LIKELY(temp > 0) DeeError_ValueError_NotFound(self,findob);
 return (Dee_size_t)-1;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeGeneric_TIterableIndexPred(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) {
 DeeObject *iterator,*elem,*pred_args,*pred_result; Dee_size_t result; int temp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 DEE_ASSERT(DeeObject_Check(pred));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return (Dee_size_t)-1;
 result = 0;
 while DEE_LIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  pred_args = DeeTuple_Pack(2,elem,findob);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(!pred_args) goto err_iter;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) goto check_ni;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if (temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
check_ni:
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) {
err_iter: Dee_DECREF(iterator); return (Dee_size_t)-1;
    }
   } else {
    DEE_ASSERT(temp > 0);
    Dee_DECREF(iterator);
    return result;
   }
  }
  ++result;
 }
 Dee_DECREF(iterator);
 if DEE_LIKELY(temp > 0) DeeError_ValueError_NotFound(self,findob);
 return (Dee_size_t)-1;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeGeneric_TIterableRIndex(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) {
 DeeObject *iterator,*elem; Dee_size_t result,pos; int temp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return (Dee_size_t)-1;
 result = (Dee_size_t)-1,pos = 0;
 while DEE_LIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  temp = DeeObject_CompareEq(elem,findob);
  Dee_DECREF(elem);
  if (temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) {
     Dee_DECREF(iterator);
     return (Dee_size_t)-1;
    }
   } else {
    DEE_ASSERT(temp > 0);
    result = pos;
   }
  }
  ++pos;
 }
 Dee_DECREF(iterator);
 if DEE_UNLIKELY(temp < 0) return (Dee_size_t)-1;
 if DEE_UNLIKELY(result == (Dee_size_t)-1) DeeError_ValueError_NotFound(self,findob);
 return result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeGeneric_TIterableRIndexPred(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) {
 DeeObject *iterator,*elem,*pred_args,*pred_result; Dee_size_t result,pos; int temp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 DEE_ASSERT(DeeObject_Check(pred));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return (Dee_size_t)-1;
 result = (Dee_size_t)-1,pos = 0;
 while DEE_LIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  pred_args = DeeTuple_Pack(2,elem,findob);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(!pred_args) goto err_iter;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) goto check_ni;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if (temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
check_ni:
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) {
err_iter: Dee_DECREF(iterator); return (Dee_size_t)-1;
    }
   } else {
    DEE_ASSERT(temp > 0);
    result = pos;
   }
  }
  ++pos;
 }
 Dee_DECREF(iterator);
 if DEE_UNLIKELY(temp < 0) return (Dee_size_t)-1;
 if DEE_UNLIKELY(result == (Dee_size_t)-1) DeeError_ValueError_NotFound(self,findob);
 return result;
}

DEE_A_RET_EXCEPT(-1) Dee_size_t DeeGeneric_TIterableCount(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) {
 DeeObject *iterator,*elem; Dee_size_t result; int temp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return (Dee_size_t)-1;
 result = 0;
 while DEE_LIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  temp = DeeObject_CompareEq(elem,findob);
  Dee_DECREF(elem);
  if (temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) {
     Dee_DECREF(iterator);
     return (Dee_size_t)-1;
    }
   } else {
    DEE_ASSERT(temp > 0);
    ++result;
   }
  }
 }
 Dee_DECREF(iterator);
 return DEE_UNLIKELY(temp < 0) ? (Dee_size_t)-1 : result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeGeneric_TIterableCountPred(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) {
 DeeObject *iterator,*elem,*pred_args,*pred_result; Dee_size_t result; int temp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return (Dee_size_t)-1;
 result = 0;
 while DEE_LIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  pred_args = DeeTuple_Pack(2,elem,findob);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(!pred_args) goto err_iter;
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) goto check_ni;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if (temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
check_ni:
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) {
err_iter: Dee_DECREF(iterator); return (Dee_size_t)-1;
    }
   } else {
    DEE_ASSERT(temp > 0);
    ++result;
   }
  }
 }
 Dee_DECREF(iterator);
 return DEE_UNLIKELY(temp < 0) ? (Dee_size_t)-1 : result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeGeneric_TIterableLocate(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) {
 DeeObject *iterator,*elem,*callargs,*pred_result; int temp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(pred));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return NULL;
 while DEE_LIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  callargs = DeeTuple_Pack(1,elem);
  if DEE_UNLIKELY(!callargs) {
err_elem: Dee_DECREF(elem);
   Dee_DECREF(iterator);
   return NULL;
  }
  pred_result = DeeObject_Call(pred,callargs);
  Dee_DECREF(callargs);
  if DEE_UNLIKELY(!pred_result) goto err_elem;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if (temp != 0) {
   if DEE_UNLIKELY(temp < 0) goto err_elem;
   DEE_ASSERT(temp > 0);
   Dee_DECREF(iterator);
   return elem; // inherit reference
  }
  Dee_DECREF(elem);
 }
 Dee_DECREF(iterator);
 if DEE_LIKELY(temp > 0) DeeError_ValueError_NotFound_Pred(self,pred);
 return NULL;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeGeneric_TIterableRLocate(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) {
 DeeObject *iterator,*elem,*callargs,*result,*pred_result; int temp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(pred));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return NULL;
 result = NULL;
 while DEE_LIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  callargs = DeeTuple_Pack(1,elem);
  if DEE_UNLIKELY(!callargs) {
err_elem: Dee_DECREF(elem);
   Dee_DECREF(iterator);
   Dee_XDECREF(result);
   return NULL;
  }
  pred_result = DeeObject_Call(pred,callargs);
  Dee_DECREF(callargs);
  if DEE_UNLIKELY(!pred_result) goto err_elem;
  temp = DeeObject_Bool(pred_result);
  Dee_DECREF(pred_result);
  if (temp != 0) {
   if DEE_UNLIKELY(temp < 0) goto err_elem;
   DEE_ASSERT(temp > 0);
   Dee_XDECREF(result);
   Dee_INHERIT_REF(result,elem);
  } else {
   Dee_DECREF(elem);
  }
 }
 Dee_DECREF(iterator);
 if DEE_UNLIKELY(temp < 0) { Dee_XDECREF(result); return NULL; }
 if DEE_LIKELY(!result) DeeError_ValueError_NotFound_Pred(self,pred);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeGeneric_TIterableLocateAll(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) {
 DeeObject *iterator,*elem,*callargs,*result; int temp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(pred));
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) return NULL;
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) {
err_r: Dee_DECREF(result); return NULL;
 }
 while DEE_LIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  callargs = DeeTuple_Pack(1,elem);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(!callargs) {
err_iter: Dee_DECREF(iterator); goto err_r;
  }
  elem = DeeObject_Call(pred,callargs);
  Dee_DECREF(callargs);
  if DEE_UNLIKELY(!elem) goto err_iter;
  temp = DeeObject_Bool(elem);
  if (temp > 0) temp = DeeList_Append(result,elem);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(temp != 0) goto err_iter;
 }
 Dee_DECREF(iterator);
 if DEE_UNLIKELY(temp < 0) Dee_CLEAR(result);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeGeneric_TIterableTransform(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) {
 DeeObject *iterator,*elem,*args,*result; int temp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(pred));
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) return NULL;
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) {
err_r: Dee_DECREF(result); return NULL;
 }
 while DEE_LIKELY((temp = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  args = DeeTuple_Pack(1,elem);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(!args) {
err_iter: Dee_DECREF(iterator); goto err_r;
  }
  elem = DeeObject_Call(pred,args);
  Dee_DECREF(args);
  if DEE_UNLIKELY(!elem) goto err_iter;
  temp = DeeList_Append(result,elem);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(temp != 0) goto err_iter;
 }
 Dee_DECREF(iterator);
 if DEE_UNLIKELY(temp < 0) Dee_CLEAR(result);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_TIterableEmpty(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *iterator,*elem; int error;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return -1;
 error = DeeObject_IterNextEx(iterator,&elem);
 Dee_DECREF(iterator);
 if (error == 0) Dee_DECREF(elem);
 return error <= 0 ? error : 1; // error, or non-empty
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_TIterableNonEmpty(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *iterator,*elem; int error;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return -1;
 error = DeeObject_IterNextEx(iterator,&elem);
 Dee_DECREF(iterator);
 if (error == 0) Dee_DECREF(elem);
 return DEE_UNLIKELY(error < 0) ? error : error == 0 ? 1 : 0;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeGeneric_TIterableFront(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *iterator,*elem; int error;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return NULL;
 error = DeeObject_IterNextEx(iterator,&elem);
 Dee_DECREF(iterator);
 if (error != 0) {
  if DEE_LIKELY(error > 0) DeeError_ValueError_EmptyList(tp_self);
  return NULL;
 }
 return elem;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeGeneric_TIterableBack(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *iterator,*elem,*next; int error;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return NULL;
 if ((error = DeeObject_IterNextEx(iterator,&elem)) != 0) {
  Dee_DECREF(iterator);
  if DEE_LIKELY(error > 0) DeeError_ValueError_EmptyList(tp_self);
  return NULL;
 }
 while DEE_LIKELY((error = DeeObject_IterNextEx(iterator,&next)) == 0) {
  Dee_DECREF(elem);
  elem = next;
 }
 Dee_DECREF(iterator);
 if (error < 0) Dee_CLEAR(elem);
 return elem;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeGeneric_TIterableReversed(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if DEE_UNLIKELY((result = DeeList_TCast(tp_self,self)) == NULL) return NULL;
 DeeList_Reverse(result);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeGeneric_TIterableSorted(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if DEE_UNLIKELY((result = DeeList_TCast(tp_self,self)) == NULL) return NULL;
 if DEE_UNLIKELY(DeeList_Sort(result) != 0) Dee_CLEAR(result);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeGeneric_TIterableSortedPred(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if DEE_UNLIKELY((result = DeeList_TCast(tp_self,self)) == NULL) return NULL;
 if DEE_UNLIKELY(DeeList_SortPred(result,pred) != 0) Dee_CLEAR(result);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeGeneric_TIterableSegments(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t n) {
 DeeObject *temp,*result;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if DEE_UNLIKELY((temp = DeeList_TCast(tp_self,self)) == NULL) return NULL;
 result = DeeList_Segments(temp,n);
 Dee_DECREF(temp);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_TIterableContains(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *elem) {
 DeeObject *iter,*item; int temp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if DEE_UNLIKELY((iter = DeeObject_TIterSelf(tp_self,self)) == NULL) return -1;
 while DEE_LIKELY((temp = DeeObject_IterNextEx(iter,&item)) == 0) {
  temp = DeeObject_CompareEq(item,elem);
  Dee_DECREF(item);
  if (temp != 0) {
   if DEE_UNLIKELY(temp < 0) {
    if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) {
     Dee_DECREF(iter);
     return -1;
    }
   } else {
    DEE_ASSERT(temp > 0);
    Dee_DECREF(iter);
    return 1;
   }
  }
 }
 Dee_DECREF(iter);
 return temp;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeGeneric_TIterableContainsPred(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) {
 DeeObject *iter,*item,*pred_args,*pred_result; int temp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(pred));
 if DEE_UNLIKELY((iter = DeeObject_TIterSelf(tp_self,self)) == NULL) return -1;
 while DEE_LIKELY((temp = DeeObject_IterNextEx(iter,&item)) == 0) {
  pred_args = DeeTuple_Pack(2,item,elem);
  Dee_DECREF(item);
  if DEE_UNLIKELY(!pred_args) {
err_iter: Dee_DECREF(iter); return -1;
  }
  pred_result = DeeObject_Call(pred,pred_args);
  Dee_DECREF(pred_args);
  if DEE_UNLIKELY(!pred_result) {
check_ni:
   if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_NotImplemented)) goto err_iter;
  } else {
   temp = DeeObject_Bool(pred_result);
   Dee_DECREF(pred_result);
   if (temp != 0) {
    if DEE_UNLIKELY(temp < 0) goto check_ni;
    DEE_ASSERT(temp > 0);
    break;
   }
  }
 }
 Dee_DECREF(iter);
 return temp;
}




int _deegenericlist_tp_str_iterate(DeeObject *elem, DeeStringWriter *writer) {
 if DEE_UNLIKELY(DEE_LIKELY(writer->sw_len != 1) &&
                 DEE_UNLIKELY(DeeStringWriter_WriteChar(writer,',') != 0)) return -1;
 return DeeStringWriter_WriteObjectStr(writer,elem);
}
DEE_A_REF DeeObject *_deegenericlist_tp_str(DeeObject *self) {
 DeeObject *result;
 DeeStringWriter writer = DeeStringWriter_INIT();
 if DEE_UNLIKELY(DeeStringWriter_WriteChar(&writer,'[') != 0 ||
                 DeeObject_Iterate(self,(DeeObjectIterateFunc)&_deegenericlist_tp_str_iterate,&writer) != 0 ||
                 DeeStringWriter_WriteChar(&writer,']') != 0) {
  result = NULL;
  goto end;
 }
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_GENERIC_VTABLE_C */
