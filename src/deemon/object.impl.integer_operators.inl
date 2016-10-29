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
#include <deemon/optional/object_decl.h>
#include <deemon/type.h>
#include <deemon/error.h>
#include <deemon/structured.h>


DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeObject_TGetInt32(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_OUT Dee_int32_t *result) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(result);
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_int32);
 return (*DeeType_GET_SLOT(tp_iter,tp_int32))(self,result);
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TGetInt64(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_OUT Dee_int64_t *result) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(result);
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_int64);
 return (*DeeType_GET_SLOT(tp_iter,tp_int64))(self,result);
}
DEE_A_RET_EXCEPT(-1) int DeeObject_TGetDouble(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_INOUT DeeObject *self, DEE_A_OUT double *result) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(result);
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_double);
 return (*DeeType_GET_SLOT(tp_iter,tp_double))(self,result);
}

DEE_A_RET_EXCEPT(-1) int DeeObject_PGetInt32(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_OUT Dee_int32_t *result) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(result && base_addr);
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_int32))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_int32))((
   DeeStructuredTypeObject *)tp_iter,base_addr,result);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__int32__()",
                     DeeType_NAME(tp_self));
 return -1;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PGetInt64(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_OUT Dee_int64_t *result) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(result && base_addr);
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_int64))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_int64))((
   DeeStructuredTypeObject *)tp_iter,base_addr,result);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__int64__()",
                     DeeType_NAME(tp_self));
 return -1;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_PGetDouble(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_OUT double *result) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(result && base_addr);
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_double))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_double))((
    DeeStructuredTypeObject *)tp_iter,base_addr,result);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__double__()",
                     DeeType_NAME(tp_self));
 return -1;
}

DEE_DECL_END

