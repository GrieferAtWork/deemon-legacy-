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
#include <deemon/object.h>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/type_decl.h>
#include <deemon/optional/type_flags.h>
#include <deemon/runtime/function.h>
#include <deemon/structured.h>
#include <deemon/tuple.h>
#include <deemon/type.h>
#include <deemon/value.h>


DEE_DECL_BEGIN

DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TCall(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DeeTypeObject const *tp_iter;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 tp_iter = tp_self; DeeType_FIND_SLOT(tp_iter,tp_call);
 return (*DeeType_GET_SLOT(tp_iter,tp_call))(self,args);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PCall(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DeeStructuredTypeObject *tp_iter;
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 tp_iter = (DeeStructuredTypeObject *)tp_self;
 while (1) {
  if (DeeType_HAS_SLOT(tp_iter,tp_p_call))
   return (*DeeType_GET_SLOT(tp_iter,tp_p_call))(
    (DeeStructuredTypeObject *)tp_iter,base_addr,args);
  tp_iter = (DeeStructuredTypeObject *)DeeType_BASE(tp_iter);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_iter)) break;
 }
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__call__%K",
                     DeeType_NAME(tp_self),
                     DeeTuple_Types(args));
 return NULL;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TThisCall(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *this_arg, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DeeTypeObject const *tp_iter; DeeObject *result,*new_args,**dst,**src,**end;
 DeeType_SLOT_TYPE(tp_call) func;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 tp_iter = tp_self; while ((func = DeeType_GET_SLOT(tp_iter,tp_call)
  ) == DeeType_DEFAULT_SLOT(tp_call)) tp_iter = DeeType_BASE(tp_iter);
 // Special optimizations
 if (func == (DeeObject*(*)(DeeObject*,DeeObject*))&DeeFunction_Call)
  return DeeFunction_ThisCall(self,this_arg,args);
 // Fallback: Prepend 'this_arg' to 'args' and call normally
 if DEE_UNLIKELY((new_args = _DeeTuple_NewUnsafe(1+DeeTuple_SIZE(args))) == NULL) return NULL;
 dst = DeeTuple_ELEM(new_args);
 end = (src = DeeTuple_ELEM(args))+DeeTuple_SIZE(args);
 Dee_INCREF(*dst++ = this_arg);
 while DEE_LIKELY(src != end) Dee_INCREF(*dst++ = *src++);
 result = (*func)(self,new_args);
 Dee_DECREF(new_args);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PThisCall(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *this_arg,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DeeObject *result,*new_args,**dst,**src,**end;
 if DEE_UNLIKELY((new_args = _DeeTuple_NewUnsafe(1+DeeTuple_SIZE(args))) == NULL) return NULL;
 dst = DeeTuple_ELEM(new_args);
 end = (src = DeeTuple_ELEM(args))+DeeTuple_SIZE(args);
 Dee_INCREF(*dst++ = this_arg);
 while DEE_LIKELY(src != end) Dee_INCREF(*dst++ = *src++);
 result = DeeObject_PCall(tp_self,base_addr,new_args);
 Dee_DECREF(new_args);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TCallObjArgs(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, ...) {
 DeeObject *result; va_list args;
 DEE_VA_START(args,self);
 result = DeeObject_VTCallObjArgs(tp_self,self,args);
 DEE_VA_END(args);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_VTCallObjArgs(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN va_list args) {
 DeeObject *result,*args_tuple;
 if DEE_UNLIKELY((args_tuple = DeeTuple_VNewObjArgs(args)) == NULL) return NULL;
 result = DeeObject_TCall(tp_self,self,args_tuple);
 Dee_DECREF(args_tuple);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PCallObjArgs(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, ...) {
 DeeObject *result; va_list args;
 DEE_VA_START(args,base_addr);
 result = DeeObject_VPCallObjArgs(tp_self,base_addr,args);
 DEE_VA_END(args);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_VPCallObjArgs(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN va_list args) {
 DeeObject *result,*args_tuple;
 if DEE_UNLIKELY((args_tuple = DeeTuple_VNewObjArgs(args)) == NULL) return NULL;
 result = DeeObject_PCall(tp_self,base_addr,args_tuple);
 Dee_DECREF(args_tuple);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TCallf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_BUILDTUPLEF char const *fmt, ...) {
 va_list args; DeeObject *result,*args_ob;
 DEE_VA_START(args,fmt);
 args_ob = Dee_VBuildTuple(fmt,args);
 DEE_VA_END(args);
 if DEE_UNLIKELY(!args_ob) return NULL;
 result = DeeObject_TCall(tp_self,self,args_ob);
 Dee_DECREF(args_ob);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_VTCallf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) {
 DeeObject *result,*args_ob;
 if DEE_UNLIKELY((args_ob = Dee_VBuildTuple(fmt,args)) == NULL) return NULL;
 result = DeeObject_TCall(tp_self,self,args_ob);
 Dee_DECREF(args_ob);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_PCallf(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) {
 va_list args; DeeObject *result,*args_ob;
 DEE_VA_START(args,fmt);
 args_ob = Dee_VBuildTuple(fmt,args);
 DEE_VA_END(args);
 if DEE_UNLIKELY(!args_ob) return NULL;
 result = DeeObject_PCall(tp_self,base_addr,args_ob);
 Dee_DECREF(args_ob);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_VPCallf(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT void *base_addr, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) {
 DeeObject *result,*args_ob;
 if DEE_UNLIKELY((args_ob = Dee_VBuildTuple(fmt,args)) == NULL) return NULL;
 result = DeeObject_PCall(tp_self,base_addr,args_ob);
 Dee_DECREF(args_ob);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_TThisCallf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *this_arg, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) {
 va_list args; DeeObject *result,*args_ob;
 DEE_VA_START(args,fmt);
 args_ob = Dee_VBuildTuple(fmt,args);
 DEE_VA_END(args);
 if DEE_UNLIKELY(!args_ob) return NULL;
 result = DeeObject_TThisCall(tp_self,self,this_arg,args_ob);
 Dee_DECREF(args_ob);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeObject_VTThisCallf(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *this_arg, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) {
 DeeObject *result,*args_ob;
 if DEE_UNLIKELY((args_ob = Dee_VBuildTuple(fmt,args)) == NULL) return NULL;
 result = DeeObject_TThisCall(tp_self,self,this_arg,args_ob);
 Dee_DECREF(args_ob);
 return result;
}

DEE_DECL_END

