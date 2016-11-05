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

#ifdef __INTELLISENSE__
#include "sequence.c"
DEE_DECL_BEGIN
#define DO_MIN
#endif

#ifdef DO_MIN
#define MM_FUNC(x) x##Min
#else
#define MM_FUNC(x) x##Max
#endif

DEE_A_RET_EXCEPT_REF DeeObject *MM_FUNC(_DeeVector)(
DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(elemc) DeeObject **elemv) {
 DeeObject *result,**iter,**end; int error;
 if DEE_UNLIKELY(!elemc) DeeReturn_None;
 Dee_INCREF(result = *elemv);
 end = (iter = elemv+1)+(elemc-1);
 while (iter != end) {
#ifdef DO_MIN
  error = DeeObject_CompareLo(*iter,result);
#else
  error = DeeObject_CompareGr(*iter,result);
#endif
  if (error != 0) {
   if (error < 0) {
    if (!DeeError_Catch(&DeeErrorType_NotImplemented)) { Dee_DECREF(result); return NULL; }
   } else {
    DEE_ASSERT(error > 0);
    Dee_INCREF(*iter);
    Dee_DECREF(result);
    result = *iter;
   }
  }
  ++iter;
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *MM_FUNC(_DeeList)(DeeListObject *self) {
 DeeObject *result,*elem; Dee_size_t i; int error;
 DeeList_ACQUIRE(self);
 if DEE_UNLIKELY(DeeList_EMPTY(self)) { DeeList_RELEASE(self); DeeReturn_None; }
 Dee_INCREF(result = DeeList_GET(self,0));
 for (i = 1; i < DeeList_SIZE(self); ++i) {
  Dee_INCREF(elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
#ifdef DO_MIN
  error = DeeObject_CompareLo(elem,result);
#else
  error = DeeObject_CompareGr(elem,result);
#endif
  if (error != 0) {
   if (error < 0) {
    Dee_DECREF(elem);
    if (!DeeError_Catch(&DeeErrorType_NotImplemented)) {
     Dee_DECREF(result);
     return NULL;
    }
   } else {
    DEE_ASSERT(error > 0);
    Dee_DECREF(result);
    Dee_INHERIT_REF(result,elem);
   }
  } else {
   Dee_DECREF(elem);
  }
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeObject *MM_FUNC(_DeeIterator)(DEE_A_INOUT DeeObject *self) {
 DeeObject *result,*elem; int error;
 if ((error = DeeObject_IterNextEx(self,&result)) != 0) {
  if (error < 0) return NULL;
  DeeReturn_None; // '[]<...' --> 'none'
 }
 while ((error = DeeObject_IterNextEx(self,&elem)) == 0) {
#ifdef DO_MIN
  error = DeeObject_CompareLo(elem,result);
#else
  error = DeeObject_CompareGr(elem,result);
#endif
  if (error != 0) {
   if (error < 0) {
    Dee_DECREF(elem);
    if (!DeeError_Catch(&DeeErrorType_NotImplemented)) {
     Dee_DECREF(result);
     return NULL;
    }
   } else {
    DEE_ASSERT(error > 0);
    Dee_DECREF(result);
    Dee_INHERIT_REF(result,elem);
   }
  } else {
   Dee_DECREF(elem);
  }
 }
 if DEE_UNLIKELY(error < 0) Dee_CLEAR(result);
 return result;
}


DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *MM_FUNC(DeeSequence_T)(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *result,*iterator;
 if (tp_self == &DeeTuple_Type) return MM_FUNC(_DeeVector)(DeeTuple_SIZE(self),DeeTuple_ELEM(self));
 if (tp_self == &DeeList_Type) return MM_FUNC(_DeeList)((DeeListObject *)self);
 if (tp_self == &DeeCell_Type) return (result = DeeCell_TryGet(self)) != NULL ? result : DeeNone_New();
 if (tp_self == &DeeSuper_Type) return MM_FUNC(DeeSequence_T)(DeeSuper_TYPE(self),DeeSuper_SELF(self));
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return NULL;
 result = MM_FUNC(_DeeIterator)(iterator);
 Dee_DECREF(iterator);
 return result;
}

#undef MM_FUNC
#ifdef DO_MIN
#undef DO_MIN
#endif

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
