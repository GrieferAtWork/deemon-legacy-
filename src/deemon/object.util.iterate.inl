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
#define DEE_LIMITED_API 1
#endif

// /include/*
#include <deemon/__conf.inl>
#include <deemon/dict.h>
#include <deemon/list.h>
#include <deemon/none.h>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/type_decl.h>
#include <deemon/set.h>
#include <deemon/string.h>
#include <deemon/super.h>
#include <deemon/tuple.h>
#include <deemon/type.h>

// */ (nano...)

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeObject_TIterate(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeObjectIterateFunc func, DEE_A_IN_OPT void *closure) {
 DeeObject *iterator,*elem; int result;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(func);
 // Some optimization for known types
 if (tp_self == &DeeTuple_Type) return DeeTuple_Traverse(self,func,closure);
 if (tp_self == &DeeList_Type) return DeeList_Traverse(self,func,closure);
 if (tp_self == &DeeDict_Type) return DeeDict_TraverseValues(self,func,closure);
 if (tp_self == &DeeDictKeys_Type) return DeeDict_TraverseKeys(DeeDictProxy_DICT(self),func,closure);
 if (tp_self == &DeeDictItems_Type) return DeeDict_TraverseItems(DeeDictProxy_DICT(self),func,closure);
 if (tp_self == &DeeDictValues_Type) return DeeDict_TraverseValues(DeeDictProxy_DICT(self),func,closure);
 if (tp_self == &DeeSet_Type) return DeeSet_Traverse(self,func,closure);
 if (tp_self == (DeeTypeObject *)&DeeNone_Type) return 0;
 if (tp_self == &DeeString_Type) { // string
  char *sbegin,*send;
  send = (sbegin = DeeString_STR(self))+DeeString_SIZE(self);
  while DEE_LIKELY(sbegin != send) {
   if ((result = (*func)(DeeString_Character(*sbegin),closure)) != 0) return result;
   ++sbegin;
  }
  return 0;
 }
 if (tp_self == &DeeSuper_Type) return DeeObject_TIterate(DeeSuper_TYPE(self),DeeSuper_SELF(self),func,closure);
 // default (use generic iterators)
 if DEE_UNLIKELY((iterator = DeeObject_TIterSelf(tp_self,self)) == NULL) return -1;
 while (1) {
  if DEE_UNLIKELY((result = DeeObject_IterNextEx(iterator,&elem)) != 0) {
   if DEE_LIKELY(result > 0) result = 0; // Iterator end
   break;
  }
  DEE_ASSERT(DeeObject_Check(elem));
  result = (*func)(elem,closure);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(result != 0) break;
 }
 Dee_DECREF(iterator);
 return result;
}

DEE_DECL_END

