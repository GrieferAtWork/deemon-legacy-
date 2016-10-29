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
#include "function.c"
#define FLAGS  (DEE_FUNCTION_FLAG_VARG|DEE_FUNCTION_FLAG_YILD/*|DEE_FUNCTION_FLAG_THIS*/)
DEE_DECL_BEGIN
#endif
#ifndef FLAGS
#error Must #define FLAGS before #including this file
#endif

#if (FLAGS&DEE_FUNCTION_FLAG_VARG)!=0
#define DEE_FUNC_MOD_V V
#else
#define DEE_FUNC_MOD_V
#endif
#if (FLAGS&DEE_FUNCTION_FLAG_YILD)!=0
#define DEE_FUNC_MOD_Y Y
#else
#define DEE_FUNC_MOD_Y
#endif
#if (FLAGS&DEE_FUNCTION_FLAG_THIS)!=0
#define DEE_FUNC_MOD_T T
#else
#define DEE_FUNC_MOD_T
#endif
#define DEE_FUNC_MODS  DEE_PP_CAT_3(DEE_FUNC_MOD_V,DEE_FUNC_MOD_Y,DEE_FUNC_MOD_T)

#if (FLAGS&DEE_FUNCTION_FLAG_VARG)!=0
#ifndef DEE_FUNCTION_FIXARGS_V_DEFINED
#define DEE_FUNCTION_FIXARGS_V_DEFINED 1
#define DEE_FUNCTION_HAVE_FIXARGS
#endif
#else
#ifndef DEE_FUNCTION_FIXARGS_DEFINED
#define DEE_FUNCTION_FIXARGS_DEFINED 1
#define DEE_FUNCTION_HAVE_FIXARGS
#endif
#endif

#ifdef DEE_FUNCTION_HAVE_FIXARGS
#undef DEE_FUNCTION_HAVE_FIXARGS
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *)
#if (FLAGS&DEE_FUNCTION_FLAG_VARG)!=0
DeeFunction_FixArgsImplV
#else
DeeFunction_FixArgsImpl
#endif
(DEE_A_IN_OBJECT(DeeFunctionObject) const *self,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 Dee_size_t argc = DeeFunction_ARGC(self);
 {
#if (FLAGS&DEE_FUNCTION_FLAG_VARG)!=0
  Dee_size_t given_argc;
  DeeObject *vargs,*result,**iter,**end,**src_iter;
  DEE_ASSERT(argc != 0 && "Vararg function must have at least 1 argument");
  // Simple case (use args directly as argument)
  if (argc == 1) return DeeTuple_NewFromVector(1,&args);
  // Complicated case (must split arguments into static and variadic portion)
  given_argc = DeeTuple_SIZE(args);
  if DEE_UNLIKELY(given_argc < argc-1) {
   DeeObject *function_code = DeeFunction_Code(self);
   // The variadic argument is optional, but the rest is required
   if (DeeCode_NAME(function_code))
    DeeError_TypeError_ArgCountExpectedNamedMin(
     DeeString_STR(DeeCode_NAME(function_code)),argc-1,given_argc);
   else DeeError_TypeError_ArgCountExpectedMin(argc-1,given_argc);
   Dee_DECREF(function_code);
   return NULL;
  }
  if DEE_UNLIKELY((vargs = DeeTuple_NewFromVector(
   given_argc-(argc-1),DeeTuple_ELEM(args)+(argc-1))) == NULL) return NULL;
  if DEE_UNLIKELY((result = _DeeTuple_NewUnsafe(argc)) == NULL) {
   Dee_DECREF(vargs);
   return NULL;
  }
  src_iter = DeeTuple_ELEM(args);
  end = (iter = DeeTuple_ELEM(result))+(argc-1);
  *end = vargs; // assign the variadic portion
  // Fill the absolute portion
  while (iter != end) Dee_INCREF(*iter++ = *src_iter++);
  return result;
#else
  if (DeeTuple_SIZE(args) != argc) {
   DeeObject *function_code = DeeFunction_Code(self);
   if (DeeCode_NAME(function_code))
    DeeError_TypeError_ArgCountExpectedNamed(
     DeeString_STR(DeeCode_NAME(function_code)),argc,DeeTuple_SIZE(args));
   else DeeError_TypeError_ArgCountExpected(argc,DeeTuple_SIZE(args));
   Dee_DECREF(function_code);
   return NULL;
  }
  // Need a real new tuple here, because executing the code may modify it
  return DeeTuple_NewFromVector(argc,DeeTuple_ELEM(args));
#endif
 }
}
#endif


DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeObject *)
 DEE_PP_CAT_2(DeeFunction_CallImpl,DEE_FUNC_MODS)(
 DEE_A_IN_OBJECT(DeeFunctionObject) const *self,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
#if (FLAGS&DEE_FUNCTION_FLAG_YILD)!=0
 DeeYieldFunctionObject *result;
#else
 DeeObject *result;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeFunction_Check(self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 {
#if (FLAGS&DEE_FUNCTION_FLAG_THIS)!=0
  Dee_size_t argc,given_argc;
  DeeObject *this_arg;
  argc = DeeFunction_ARGC(self);
  {
#if (FLAGS&DEE_FUNCTION_FLAG_VARG)!=0
   DeeObject *vargs,*new_args,**iter,**end,**src_iter;
   DEE_ASSERT(argc != 0 && "Vararg function must have at least 1 argument");
   if ((given_argc = DeeTuple_SIZE(args)) < argc) {
    DeeObject *function_code;
    // The variadic argument is optional, but the rest is required
    function_code = DeeFunction_Code(self);
    if (DeeCode_NAME(function_code))
     DeeError_TypeError_ArgCountExpectedNamedMin(
      DeeString_STR(DeeCode_NAME(function_code)),argc,given_argc);
    else DeeError_TypeError_ArgCountExpectedMin(argc,given_argc);
    Dee_DECREF(function_code);
    return NULL;
   }
   if ((vargs = DeeTuple_NewFromVector(given_argc-(argc-2),DeeTuple_ELEM(args)+argc)) == NULL) return NULL;
   if ((new_args = _DeeTuple_NewUnsafe(argc)) == NULL) {
    Dee_DECREF(vargs);
    return NULL;
   }
   src_iter = DeeTuple_ELEM(args);
   end = (iter = DeeTuple_ELEM(new_args))+(argc-1);
   *end = vargs; // assign the variadic portion
   this_arg = *src_iter++; // Pull out the this-argument
   // Fill the absolute portion
   while (iter != end) Dee_INCREF(*iter++ = *src_iter++);
   args = new_args;
#else
   DeeObject *new_args,**dst,**src,**end;
   if ((given_argc = DeeTuple_SIZE(args)) != (argc+1)) {
    DeeObject *function_code;
    DeeFunction_ACQUIRE(self);
    Dee_INCREF(function_code = DeeFunction_CODE(self));
    DeeFunction_RELEASE(self);
    DEE_ASSERT(DeeObject_Check(function_code) && DeeCode_Check(function_code));
    if (DeeCode_NAME(function_code)) {
     DeeError_TypeError_ArgCountExpectedNamed(
      DeeString_STR(DeeCode_NAME(function_code)),argc+1,DeeTuple_SIZE(args));
    } else {
     DeeError_TypeError_ArgCountExpected(argc+1,DeeTuple_SIZE(args));
    }
    Dee_DECREF(function_code);
    return NULL;
   }
   if ((new_args = _DeeTuple_NewUnsafe(given_argc-1)) == NULL) return NULL;
   end = (src = DeeTuple_ELEM(args))+DeeTuple_SIZE(args);
   DEE_ASSERT(src != end);
   this_arg = *src++;
   dst = DeeTuple_ELEM(new_args);
   while (src != end) Dee_INCREF(*dst++ = *src++);
   args = new_args;
#endif
  }
#if (FLAGS&DEE_FUNCTION_FLAG_YILD)!=0
  if DEE_UNLIKELY((result = DeeObject_MALLOCF(
   DeeYieldFunctionObject,"Yield function %p(%p)",self,args)) == NULL) {
   Dee_DECREF(args);
   return NULL;
  }
  DeeObject_INIT(result,&DeeYieldFunction_Type);
  Dee_INHERIT_REF(result->yf_args,*(DeeTupleObject **)&args);
  Dee_INCREF(result->yf_func = (DeeFunctionObject *)self);
  Dee_INCREF(result->yf_this = this_arg);
#else
  {
   DeeObject *code,*refs;
   DeeFunction_ACQUIRE(self);
   Dee_INCREF(code = DeeFunction_CODE(self));
   Dee_INCREF(refs = DeeFunction_REFS(self));
   DeeFunction_RELEASE(self);
#ifdef DEE_DEBUG
   result = DeeCode_RunDebug(code,
    DeeTuple_SIZE(refs),DeeTuple_ELEM(refs),
    DeeTuple_SIZE(args),DeeTuple_ELEM(args),this_arg);
#else
   result = DeeCode_Run(code,
    DeeTuple_ELEM(refs),DeeTuple_ELEM(args),this_arg);
#endif
   Dee_DECREF(refs);
   Dee_DECREF(code);
   Dee_DECREF(args);
  }
#endif
#else
#if (FLAGS&DEE_FUNCTION_FLAG_VARG)!=0
  if ((args = DeeFunction_FixArgsImplV(self,args)) == NULL) return NULL;
#else
  if ((args = DeeFunction_FixArgsImpl(self,args)) == NULL) return NULL;
#endif
#if (FLAGS&DEE_FUNCTION_FLAG_YILD)!=0
  if DEE_UNLIKELY((result = DeeObject_MALLOCF(
   DeeYieldFunctionObject,"Yield function %p(%p)",self,args)) == NULL) {
   Dee_DECREF(args);
   return NULL;
  }
  DeeObject_INIT(result,&DeeYieldFunction_Type);
  Dee_INHERIT_REF(result->yf_args,*(DeeTupleObject **)&args);
  Dee_INCREF(result->yf_func = (DeeFunctionObject *)self);
  result->yf_this = NULL;
#else
  {
   DeeObject *code,*refs;
   DeeFunction_ACQUIRE(self);
   Dee_INCREF(code = DeeFunction_CODE(self));
   Dee_INCREF(refs = DeeFunction_REFS(self));
   DeeFunction_RELEASE(self);
#ifdef DEE_DEBUG
   result = DeeCode_RunDebug(code,
    DeeTuple_SIZE(refs),DeeTuple_ELEM(refs),
    DeeTuple_SIZE(args),DeeTuple_ELEM(args),NULL);
#else
   result = DeeCode_Run(code,
    DeeTuple_ELEM(refs),DeeTuple_ELEM(args),NULL);
#endif
   Dee_DECREF(refs);
   Dee_DECREF(code);
   Dee_DECREF(args);
  }
#endif
#endif
 }
 return (DeeObject *)result;
}


DEE_A_RET_EXCEPT_REF DeeObject *DEE_PP_CAT_2(DeeFunction_ThisCallImpl,DEE_FUNC_MODS)(
 DEE_A_IN_OBJECT(DeeFunctionObject) const *self, DEE_A_INOUT DeeObject *this_arg,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
#if (FLAGS&DEE_FUNCTION_FLAG_YILD)!=0
 DeeYieldFunctionObject *result;
#else
 DeeObject *result;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeFunction_Check(self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 DEE_ASSERT(DeeObject_Check(this_arg));
#if (FLAGS&DEE_FUNCTION_FLAG_THIS)!=0
#if (FLAGS&DEE_FUNCTION_FLAG_VARG)!=0
 if ((args = DeeFunction_FixArgsImplV(self,args)) == NULL) return NULL;
#else
 if ((args = DeeFunction_FixArgsImpl(self,args)) == NULL) return NULL;
#endif
#if (FLAGS&DEE_FUNCTION_FLAG_YILD)!=0
 if DEE_UNLIKELY((result = DeeObject_MALLOCF(
  DeeYieldFunctionObject,"Yield function %p(%p)",self,args)) == NULL) {
  Dee_DECREF(args);
  return NULL;
 }
 DeeObject_INIT(result,&DeeYieldFunction_Type);
 Dee_INHERIT_REF(result->yf_args,*(DeeTupleObject **)&args);
 Dee_INCREF(result->yf_func = (DeeFunctionObject *)self);
 Dee_INCREF(result->yf_this = this_arg);
#else
 {
  DeeObject *code,*refs;
  DeeFunction_ACQUIRE(self);
  Dee_INCREF(code = DeeFunction_CODE(self));
  Dee_INCREF(refs = DeeFunction_REFS(self));
  DeeFunction_RELEASE(self);
#ifdef DEE_DEBUG
  result = DeeCode_RunDebug(code,
   DeeTuple_SIZE(refs),DeeTuple_ELEM(refs),
   DeeTuple_SIZE(args),DeeTuple_ELEM(args),this_arg);
#else
  result = DeeCode_Run(code,
   DeeTuple_ELEM(refs),DeeTuple_ELEM(args),this_arg);
#endif
  Dee_DECREF(refs);
  Dee_DECREF(code);
 }
 Dee_DECREF(args);
#endif
#else
 {
  Dee_size_t argc = DeeFunction_ARGC(self);
#if (FLAGS&DEE_FUNCTION_FLAG_VARG)!=0
  Dee_size_t given_argc;
  DEE_ASSERT(argc != 0 && "Vararg function must have at least 1 argument");
  if (argc == 1) { // Simple case: Prepend this_arg and return a tuple-of-tuple
   DeeObject *inner_tuple,**dst,**iter,**end;
   if ((inner_tuple = _DeeTuple_NewUnsafe(DeeTuple_SIZE(args)+1)) == NULL) return NULL;
   dst = DeeTuple_ELEM(inner_tuple);
   Dee_INCREF(*dst++ = this_arg);
   end = (iter = DeeTuple_ELEM(args))+DeeTuple_SIZE(args);
   while (iter != end) Dee_INCREF(*dst++ = *iter++);
   args = DeeTuple_Pack(1,inner_tuple);
   Dee_DECREF(inner_tuple);
   if (!args) return NULL;
  } else {
   // Complicated case (must split arguments into static and variadic portion)
   DeeObject *vargs,*new_args,**iter,**end,**src_iter;
   given_argc = DeeTuple_SIZE(args)+1;
   if (given_argc < argc-1) {
    // The variadic argument is optional, but the rest is required
    DeeError_TypeError_ArgCountExpectedMin(argc-1,given_argc);
    return NULL;
   }
   if ((vargs = DeeTuple_NewFromVector(given_argc-(
    argc-1),DeeTuple_ELEM(args)+(argc-1))) == NULL) return NULL;
   if ((new_args = _DeeTuple_NewUnsafe(argc)) == NULL) {
    Dee_DECREF(vargs);
    return NULL;
   }
   src_iter = DeeTuple_ELEM(args);
   end = (iter = DeeTuple_ELEM(new_args))+(argc-1);
   Dee_INCREF(*iter++ = this_arg);
   *end = vargs; // assign the variadic portion
   // Fill the absolute portion
   while (iter != end) Dee_INCREF(*iter++ = *src_iter++);
   args = new_args;
  }
#else
  DeeObject *new_args,**dst,**iter,**end;
  if (DeeTuple_SIZE(args) != (argc-1)) {
   DeeError_TypeError_ArgCountExpected(argc,DeeTuple_SIZE(args));
   return NULL;
  }
  // Need a real new tuple here, because executing the code may modify it
  if ((new_args = _DeeTuple_NewUnsafe(DeeTuple_SIZE(args)+1)) == NULL) return NULL;
  dst = DeeTuple_ELEM(new_args);
  Dee_INCREF(*dst++ = this_arg);
  end = (iter = DeeTuple_ELEM(args))+DeeTuple_SIZE(args);
  while (iter != end) Dee_INCREF(*dst++ = *iter++);
  args = new_args;
#endif
 }
#if (FLAGS&DEE_FUNCTION_FLAG_YILD)!=0
 if DEE_UNLIKELY((result = DeeObject_MALLOCF(DeeYieldFunctionObject,
  "Yield function %p(%p)",self,args)) == NULL) {
  Dee_DECREF(args);
  return NULL;
 }
 DeeObject_INIT(result,&DeeYieldFunction_Type);
 Dee_INHERIT_REF(result->yf_args,*(DeeTupleObject **)&args);
 Dee_INCREF(result->yf_func = (DeeFunctionObject *)self);
 result->yf_this = NULL;
#else
 {
  DeeObject *code,*refs;
  DeeFunction_ACQUIRE(self);
  Dee_INCREF(code = DeeFunction_CODE(self));
  Dee_INCREF(refs = DeeFunction_REFS(self));
  DeeFunction_RELEASE(self);
#ifdef DEE_DEBUG
  result = DeeCode_RunDebug(code,
   DeeTuple_SIZE(refs),DeeTuple_ELEM(refs),
   DeeTuple_SIZE(args),DeeTuple_ELEM(args),NULL);
#else
  result = DeeCode_Run(code,
   DeeTuple_ELEM(refs),DeeTuple_ELEM(args),NULL);
#endif
  Dee_DECREF(refs);
  Dee_DECREF(code);
  Dee_DECREF(args);
 }
#endif
#endif
 return (DeeObject *)result;
}


#undef DEE_FUNC_MODS
#undef DEE_FUNC_MOD_T
#undef DEE_FUNC_MOD_Y
#undef DEE_FUNC_MOD_V
#undef FLAGS
#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
