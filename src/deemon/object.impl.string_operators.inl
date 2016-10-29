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

#include <deemon/__conf.inl>
#include <deemon/optional/type_decl.h>
#include <deemon/optional/type_flags.h>
#include <deemon/optional/object_decl.h>
#include <deemon/type.h>
#include <deemon/error.h>
#include <deemon/string.h>
#include <deemon/mp/thread.h>


DEE_DECL_BEGIN

DeeString_NEW_STATIC(_dee_recursive_str,"...");
DeeObject *_DeeObject_RecursiveString = (DeeObject *)&_dee_recursive_str;

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeObject_TStr(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *result;
 DeeThreadObject *thread_self = DeeThread_SELF();
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERTF(DeeObject_Check(tp_self),"%p is not an object",tp_self);
 DEE_ASSERTF(DeeType_Check(tp_self),"instance of %s is not a type object",
             DeeType_NAME(Dee_TYPE(tp_self)));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DeeType_FIND_SLOT(tp_self,tp_str);
 if DEE_UNLIKELY(!thread_self || _DeeRecursionList_Contains(&thread_self->t_recursion_str,tp_self,self))
  DeeReturn_NEWREF((DeeObject *)&_dee_recursive_str); // We use this as fallback if thread_self isn't found
 if DEE_UNLIKELY(_DeeRecursionList_Push(&thread_self->t_recursion_str,tp_self,self) != 0) return NULL;
 result = (*DeeType_GET_SLOT(tp_self,tp_str))(self);
 DEE_ASSERT(!result || (DeeObject_Check(result) && DeeString_Check(result)));
 _DeeRecursionList_Pop(&thread_self->t_recursion_str);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeObject_TRepr(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *result;
 DeeThreadObject *thread_self = DeeThread_SELF();
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DeeType_FIND_SLOT(tp_self,tp_repr);
 if DEE_UNLIKELY(!thread_self || _DeeRecursionList_Contains(&thread_self->t_recursion_repr,tp_self,self))
  DeeReturn_NEWREF((DeeObject *)&_dee_recursive_str); // We use this as fallback if thread_self isn't found
 if DEE_UNLIKELY(_DeeRecursionList_Push(&thread_self->t_recursion_repr,tp_self,self) != 0) return NULL;
 result = (*DeeType_GET_SLOT(tp_self,tp_repr))(self);
 DEE_ASSERT(!result || (DeeObject_Check(result) && DeeString_Check(result)));
 _DeeRecursionList_Pop(&thread_self->t_recursion_repr);
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *_DeeObject_TStr(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
#ifdef DEE_DEBUG
 DeeObject *result;
#endif /* DEE_DEBUG */
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DeeType_FIND_SLOT(tp_self,tp_str);
#ifdef DEE_DEBUG
 result = (*DeeType_GET_SLOT(tp_self,tp_str))(self);
 DEE_ASSERT(!result || (
  DeeObject_Check(result) &&
  DeeString_Check(result)));
 return result;
#else /* DEE_DEBUG */
 return (*DeeType_GET_SLOT(tp_self,tp_str))(self);
#endif /* DEE_DEBUG */
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *_DeeObject_TRepr(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
#ifdef DEE_DEBUG
 DeeObject *result;
#endif /* DEE_DEBUG */
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DeeType_FIND_SLOT(tp_self,tp_repr);
#ifdef DEE_DEBUG
 result = (*DeeType_GET_SLOT(tp_self,tp_repr))(self);
 DEE_ASSERT(!result || (
  DeeObject_Check(result) &&
  DeeString_Check(result)));
 return result;
#else /* DEE_DEBUG */
 return (*DeeType_GET_SLOT(tp_self,tp_repr))(self);
#endif /* DEE_DEBUG */
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeObject_PStr(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_str)) {
   return (*DeeType_GET_SLOT(tp_iter,tp_p_str))((DeeStructuredTypeObject *)tp_iter,base_addr);
  }
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 return DeeString_Newf("%s@%p",DeeType_NAME(tp_self),base_addr);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeObject_PRepr(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_repr)) {
   return (*DeeType_GET_SLOT(tp_iter,tp_p_repr))((DeeStructuredTypeObject *)tp_iter,base_addr);
  }
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 return DeeString_Newf("<%K: %K>",
                       DeeObject_PStr(tp_self,base_addr),DeeString_NewHexDump(
                       base_addr,DeeType_GET_SLOT(tp_self,tp_p_instance_size)));
}


DEE_DECL_END

