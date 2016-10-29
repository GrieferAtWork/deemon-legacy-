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
#include "structured.c"
DEE_DECL_BEGIN
#define MODE2
#endif

// NOTE: 'MODE2' enables the detection of integral types evaluating to 0

#ifdef MODE2
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeObject_TTryGetPointer2
#else
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeObject_TTryGetPointer
#endif
(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_result, DEE_A_OUT void **result) {
 DEE_ASSERT(result);
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(tp_result) && DeeStructuredType_Check(tp_result));
 if (DeePointerType_Check(tp_self)) {
  // Most likely case: An actual pointer object
  if (DeePointerType_CheckExact(tp_self) || // Untyped pointer
      tp_result == (DeeTypeObject *)&DeeNone_Type || DeeType_IsSameOrDerived(
      // Pointer is 'tp' or is derived from it
      (DeeTypeObject *)((DeePointerTypeObject *)tp_self)->tp_ptr_pointer_base,tp_result)) {
   *result = DeePointer_ADDR(self);
   return 0;
  } else return 1;
 } else if (tp_self == (DeeTypeObject *)&DeeNone_Type) {
  *result = NULL; // Always the case: none == nullptr
  return 0;
 } else if (DeeLValueType_Check(tp_self)) {
  DeeStructuredTypeObject *base = DeeType_GET_SLOT(tp_self,tp_lval_lvalue_base);
  if (DeePointerType_Check(base)) {
   if (DeePointerType_CheckExact(tp_self) || // Untyped pointer
       tp_result == (DeeTypeObject *)&DeeNone_Type || DeeType_IsSameOrDerived(
       // Pointer is 'tp' or is derived from it
       (DeeTypeObject *)((DeePointerTypeObject *)base)->tp_ptr_pointer_base,tp_result)) {
    DEE_SIGNALHANDLER_BEGIN {
     *result = *((void **)DeeLValue_ADDR(self));
    } DEE_SIGNALHANDLER_END({
     return -1;
    });
    return 0;
   } else return 1;
  } else if (DeeForeignFunctionType_Check(base)) {
   // Implicit cast for lvalue-to-foreign_function to pointer
   if (tp_result == (DeeTypeObject *)&DeeNone_Type ||
       DeeType_IsSameOrDerived((DeeTypeObject *)base,tp_result)) {
    *result = (void *)DeeLValue_ADDR(self);
    return 0;
   } else return 1;
  } else if (DeeArrayType_Check(base) && (
   tp_result == (DeeTypeObject *)&DeeNone_Type ||
   DeeType_IsSameOrDerived(DeeArrayType_BASE(base),tp_result))) {
   // Implicitly cast lvalue-to-array --> pointer-to-elem
   *result = (void *)DeeLValue_ADDR(self);
   return 0;
  } else {
#ifdef MODE2
#ifdef DEE_PLATFORM_64_BIT
   Dee_int64_t temp;
   DeeType_SLOT_TYPE(tp_p_int64) func;
   while (1) {
    if ((func = DeeType_GET_SLOT(base,tp_p_int64)) != NULL) break;
    base = (DeeStructuredTypeObject *)DeeType_BASE(base);
    if (!DeeStructuredType_Check(base)) return 1;
   }
#else
   Dee_int32_t temp;
   DeeType_SLOT_TYPE(tp_p_int32) func;
   while (1) {
    if ((func = DeeType_GET_SLOT(base,tp_p_int32)) != NULL) break;
    base = (DeeStructuredTypeObject *)DeeType_BASE(base);
    if (!DeeStructuredType_Check(base)) return 1;
   }
#endif
   DEE_SIGNALHANDLER_BEGIN {
    if ((*func)(base,DeeLValue_ADDR(self),&temp) != 0) return -1;
   } DEE_SIGNALHANDLER_END({
    return -1;
   });
   if (temp != 0) return 1;
   *result = NULL;
   return 0;
#else
   return 1;
#endif
  }
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 } else if (tp_self == &DeeUtf8String_Type && (
  tp_result == (DeeTypeObject *)&DeeNone_Type ||
  DeeType_IsSameOrDerived(DeeObject_TYPE(char),tp_result))) {
  *result = (void *)DeeString_STR(self);
  return 0;
#endif
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 } else if (tp_self == &DeeWideString_Type && (
  tp_result == (DeeTypeObject *)&DeeNone_Type ||
  DeeType_IsSameOrDerived(DeeObject_TYPE(wchar_t),tp_result))) {
  *result = (void *)DeeWideString_STR(self);
  return 0;
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 } else if (tp_self == &DeeUtf16String_Type && (
  tp_result == (DeeTypeObject *)&DeeNone_Type ||
  DeeType_IsSameOrDerived(DeeObject_TYPE(char16_t),tp_result))) {
  *result = (void *)DeeUtf16String_STR(self);
  return 0;
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 } else if (tp_self == &DeeUtf32String_Type && (
  tp_result == (DeeTypeObject *)&DeeNone_Type ||
  DeeType_IsSameOrDerived(DeeObject_TYPE(char32_t),tp_result))) {
  *result = (void *)DeeUtf32String_STR(self);
  return 0;
#endif
 } else if (DeeArrayType_Check(tp_self) && (tp_result == (DeeTypeObject *)&DeeNone_Type ||
  DeeType_IsSameOrDerived(DeeArrayType_BASE(tp_self),tp_result))) {
  *result = DeeArray_DATA(self); // Implicitly cast array --> pointer-to-elem
  return 0;
 } else if (DeeVarArrayType_Check(tp_self) && (tp_result == (DeeTypeObject *)&DeeNone_Type ||
  DeeType_IsSameOrDerived(DeeVarArrayType_BASE(tp_self),tp_result))) {
  *result = DeeVarArray_DATA(self); // Implicitly cast array --> pointer-to-elem
  return 0;
 } else if (DeeForeignFunctionType_Check(tp_self)) {
  // Implicit cast for foreign_function to pointer
  if (tp_result == (DeeTypeObject *)&DeeNone_Type ||
      DeeObject_InstanceOf(self,tp_result)) {
DEE_COMPILER_MSVC_WARNING_PUSH(4054 4055)
   *result = (void *)DeeForeignFunction_FUNC(self);
DEE_COMPILER_MSVC_WARNING_POP
   return 0;
  } else return 1;
 } else if (tp_self == &DeeForeignFunctionClosure_Type) {
  // Implicit cast for foreign_function_closure to pointer
  if (tp_result == (DeeTypeObject *)&DeeNone_Type ||
      DeeType_IsSameOrDerived(DeeForeignFunctionClosure_TYPE(self),tp_result)) {
DEE_COMPILER_MSVC_WARNING_PUSH(4054 4055)
   *result = (void *)DeeForeignFunctionClosure_FUNC(self);
DEE_COMPILER_MSVC_WARNING_POP
   return 0;
  } else return 1;
#if 0 /* This is not how it should work! */
 } else if (tp_result == &DeeObject_Type) {
  *result = (void *)self; // User wants an object pointer
  return 0;
#endif
 } else if (tp_self == &DeeSuper_Type) {
#ifdef MODE2
  return DeeObject_TTryGetPointer2(DeeSuper_TYPE(self),DeeSuper_SELF(self),tp_result,result);
#else
  return DeeObject_TTryGetPointer(DeeSuper_TYPE(self),DeeSuper_SELF(self),tp_result,result);
#endif
 } else {
#ifdef MODE2
  // Least likely case: an integer constant evaluating to '(size_t)0'
  Dee_size_t temp;
  if (DeeObject_Cast(Dee_size_t,self,&temp) != 0) return -1;
  if (temp != 0) return 1;
  *result = NULL;
  return 0;
#else
  return 1;
#endif
 }
}

#ifdef MODE2
#undef MODE2
#endif

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
