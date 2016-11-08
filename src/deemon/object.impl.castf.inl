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
#include <deemon/error.h>
#include <deemon/marshal_data.h>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/std/stdarg.h>
#include <deemon/optional/type_decl.h>
#include <deemon/optional/uuid.h>
#include <deemon/structured.h>
#include <deemon/float.h>
#include <deemon/type.h>


DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
#undef DEE_PRIVATE_DECL_DEE_RUNTIME_TYPES
#endif

DEE_A_RET_EXCEPT(-1) int DeeObject_TCastf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_OUT_OPT void *result, DEE_A_IN_Z char const *fmt, ...) {
 va_list args; int error;
 DEE_VA_START(args,fmt);
 error = DeeObject_VTCastf(tp_self,self,result,fmt,args);
 DEE_VA_END(args);
 return error;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_VTCastf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_OUT_OPT void *result, DEE_A_IN_Z char const *fmt, DEE_A_IN va_list args) {
 DeeTypeObject *casted_type;
 struct DeeTypeMarshal const *marshal;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(result);
 DEE_ASSERT(fmt);
 if DEE_UNLIKELY((casted_type = DeeType_VStructf(fmt,args)) == NULL) return -1;
 DEE_ASSERT(DeeObject_Check(casted_type) && DeeType_Check(casted_type));
 marshal = DeeType_GET_SLOT(casted_type,tp_marshal);
 if DEE_UNLIKELY(!marshal || !DeeUUID_CHECK_INTERNAL(&marshal->tp_uuid)) {
err_nocast:
  if (DeePointerType_Check(casted_type)) { // Cast to pointer
   return DeeObject_TGetPointerEx(tp_self,self,DeePointerType_BASE(casted_type),(void **)result);
  }
  DeeError_SetStringf(&DeeErrorType_TypeError,
                      "Can't cast instance of %q to %q",
                      DeeType_NAME(tp_self),DeeType_NAME(casted_type));
  return -1;
 }
 switch (DeeUUID_GET_INTERNAL(&marshal->tp_uuid)) {
  case DEE_MARSHALID_NONE: return 0;
  case DEE_MARSHALTYPEID_int8         :
  case DEE_MARSHALTYPEID_atomic_int8  : return DeeObject_TCast(Dee_int8_t,tp_self,self,(Dee_int8_t *)result);
  case DEE_MARSHALTYPEID_int16        :
  case DEE_MARSHALTYPEID_atomic_int16 : return DeeObject_TCast(Dee_int16_t,tp_self,self,(Dee_int16_t *)result);
  case DEE_MARSHALTYPEID_int32        :
  case DEE_MARSHALTYPEID_atomic_int32 : return DeeObject_TCast(Dee_int32_t,tp_self,self,(Dee_int32_t *)result);
  case DEE_MARSHALTYPEID_int64        :
  case DEE_MARSHALTYPEID_atomic_int64 : return DeeObject_TCast(Dee_int64_t,tp_self,self,(Dee_int64_t *)result);
  case DEE_MARSHALTYPEID_uint8        :
  case DEE_MARSHALTYPEID_atomic_uint8 : return DeeObject_TCast(Dee_uint8_t,tp_self,self,(Dee_uint8_t *)result);
  case DEE_MARSHALTYPEID_uint16       :
  case DEE_MARSHALTYPEID_atomic_uint16: return DeeObject_TCast(Dee_uint16_t,tp_self,self,(Dee_uint16_t *)result);
  case DEE_MARSHALTYPEID_uint32       :
  case DEE_MARSHALTYPEID_atomic_uint32: return DeeObject_TCast(Dee_uint32_t,tp_self,self,(Dee_uint32_t *)result);
  case DEE_MARSHALTYPEID_uint64       :
  case DEE_MARSHALTYPEID_atomic_uint64: return DeeObject_TCast(Dee_uint64_t,tp_self,self,(Dee_uint64_t *)result);
  case DEE_MARSHALTYPEID_float4: case DEE_MARSHALTYPEID_float8:
  case DEE_MARSHALTYPEID_float12: case DEE_MARSHALTYPEID_float16:
   if (casted_type == (DeeTypeObject *)&DeeDouble_Type) return DeeObject_TCast(Dee_rt_double,tp_self,self,(Dee_rt_double *)result);
   if (casted_type == (DeeTypeObject *)&DeeFloat_Type) return DeeObject_TCast(Dee_rt_float,tp_self,self,(Dee_rt_float *)result);
   DEE_ASSERT(casted_type == (DeeTypeObject *)&DeeLDouble_Type);
   return DeeObject_TCast(Dee_rt_ldouble,tp_self,self,(Dee_rt_ldouble *)result);
  default: goto err_nocast;
 }
}




DEE_DECL_END

