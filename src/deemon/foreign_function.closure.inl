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
#ifndef GUARD_DEEMON_FOREIGN_FUNCTION_CLOSURE_INL
#define GUARD_DEEMON_FOREIGN_FUNCTION_CLOSURE_INL 1

#ifdef __INTELLISENSE__
#include "foreign_function.c"
#endif

#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
#include <deemon/file.h>
#include <deemon/marshal.h>
#include <deemon/marshal_data.h>

DEE_DECL_BEGIN

DEE_COMPILER_MSVC_WARNING_PUSH(4061)
static void raw_closure_callback(
 ffi_cif *cif, void *resp, union argument **args,
 DeeForeignFunctionClosureObject *self) {
#define returnp(T)  ((T*)resp)
#define returnl(T) (*(T*)resp)
 DeeObject *deemon_result,*deemon_args;
 DeeObject **arg_iter,**arg_end; union argument *arg;
 DeeTypeObject **argty_iter;
 ffi_type **ffi_argty_iter;
 DeeForeignFunctionTypeObject *ff_type = self->ffc_type;
 (void)cif,resp,args;

 // Create the call arguments
 if ((deemon_args = _DeeTuple_NewUnsafe(ff_type->fft_arg_type_c)) == NULL) goto on_err;
 arg_end = (arg_iter = DeeTuple_ELEM(deemon_args))+ff_type->fft_arg_type_c;
 argty_iter = ff_type->fft_arg_type_v;
 ffi_argty_iter = ff_type->fft_ffi_arg_type_v;
 while (arg_iter != arg_end) {
  arg = *args;
  switch ((*ffi_argty_iter)->type) {
   case FFI_TYPE_INT: *arg_iter = DeeObject_New(int,arg->i); break;
   case FFI_TYPE_FLOAT: *arg_iter = DeeObject_New(float,arg->f); break;
   case FFI_TYPE_DOUBLE: *arg_iter = DeeObject_New(double,arg->d); break;
#if FFI_TYPE_LONGDOUBLE != FFI_TYPE_DOUBLE
   case FFI_TYPE_LONGDOUBLE: *arg_iter = DeeObject_New(ldouble,arg->ld); break;
#endif
   case FFI_TYPE_UINT8:  *arg_iter = DeeObject_New(Dee_uint8_t,arg->ui8); break;
   case FFI_TYPE_SINT8:  *arg_iter = DeeObject_New(Dee_int8_t,arg->i8); break;
   case FFI_TYPE_UINT16: *arg_iter = DeeObject_New(Dee_uint16_t,arg->ui16); break;
   case FFI_TYPE_SINT16: *arg_iter = DeeObject_New(Dee_int16_t,arg->i16); break;
   case FFI_TYPE_UINT32: *arg_iter = DeeObject_New(Dee_uint32_t,arg->ui32); break;
   case FFI_TYPE_SINT32: *arg_iter = DeeObject_New(Dee_int32_t,arg->i32); break;
   case FFI_TYPE_UINT64: *arg_iter = DeeObject_New(Dee_uint64_t,arg->ui64); break;
   case FFI_TYPE_SINT64: *arg_iter = DeeObject_New(Dee_int64_t,arg->i64); break;
   case FFI_TYPE_STRUCT: *arg_iter = DeeStructured_NewFromData(*argty_iter,(void *)arg); break;
   case FFI_TYPE_POINTER:
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
    if (DeeLValueType_Check(*argty_iter)) *arg_iter = DeeLValue_NewWithError(*argty_iter,arg->p); else
    if (DeePointerType_Check(*argty_iter)) *arg_iter = DeePointer_New(*argty_iter,arg->p); else
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#if DEE_CONFIG_HAVE_ENCODING_WIDE
    if (*argty_iter == &DeeWideString_Type) *arg_iter = DeeWideString_New((Dee_WideChar *)arg->p); else
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
    if (*argty_iter == &DeeUtf8String_Type) *arg_iter = DeeUtf8String_New((Dee_Utf8Char *)arg->p); else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
    if (*argty_iter == &DeeUtf16String_Type) *arg_iter = DeeUtf16String_New((Dee_Utf16Char *)arg->p); else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
    if (*argty_iter == &DeeUtf32String_Type) *arg_iter = DeeUtf32String_New((Dee_Utf32Char *)arg->p); else
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
    if (arg->p) {
     Dee_INCREF(*arg_iter = (DeeObject *)arg->p);
     goto argdone_nocheck;
    } else {
     *arg_iter = DeeNone_New();
     goto argdone_nocheck;
    }
    break;
   default: *arg_iter = DeeNone_New(); goto argdone_nocheck;
  }
  if (!*arg_iter) {
   while (arg_iter != DeeTuple_ELEM(deemon_args)) Dee_DECREF(*--arg_iter);
   _DeeTuple_FreeUnsafe(deemon_args);
   goto on_err;
  }
argdone_nocheck:
  ++arg_iter,++argty_iter,++ffi_argty_iter,++args;
 }

 // TODO: Actually convert arguments
 deemon_result = DeeObject_Call(self->ffc_callback,deemon_args);
 Dee_DECREF(deemon_args);
 if (deemon_result) {
  // Really hacky way of getting c-style return behavior:
  // >> return 10,20,30; // In c, this statement returns '30'
  while DEE_UNLIKELY(DeeTuple_Check(deemon_result) && !DeeTuple_EMPTY(deemon_result))
   deemon_result = DeeTuple_GET(deemon_result,DeeTuple_SIZE(deemon_result)-1);
  switch (ff_type->fft_ffi_return_type->type) {
   case FFI_TYPE_INT: if DEE_UNLIKELY(DeeObject_Cast(int,deemon_result,returnp(int)) != 0) goto on_err; break;
   case FFI_TYPE_FLOAT: if DEE_UNLIKELY(DeeObject_Cast(float,deemon_result,returnp(float)) != 0) goto on_err; break;
   case FFI_TYPE_DOUBLE: if DEE_UNLIKELY(DeeObject_Cast(double,deemon_result,returnp(double)) != 0) goto on_err; break;
#if FFI_TYPE_LONGDOUBLE != FFI_TYPE_DOUBLE
   case FFI_TYPE_LONGDOUBLE: if DEE_UNLIKELY(DeeObject_Cast(ldouble,deemon_result,returnp(long double)) != 0) goto on_err; break;
#endif
   case FFI_TYPE_UINT8:  case FFI_TYPE_SINT8:  if DEE_UNLIKELY(DeeObject_Cast(int8_t,deemon_result,returnp(int8_t)) != 0) goto on_err; break;
   case FFI_TYPE_UINT16: case FFI_TYPE_SINT16: if DEE_UNLIKELY(DeeObject_Cast(int16_t,deemon_result,returnp(int16_t)) != 0) goto on_err; break;
   case FFI_TYPE_UINT32: case FFI_TYPE_SINT32: if DEE_UNLIKELY(DeeObject_Cast(int32_t,deemon_result,returnp(int32_t)) != 0) goto on_err; break;
   case FFI_TYPE_UINT64: case FFI_TYPE_SINT64: if DEE_UNLIKELY(DeeObject_Cast(int64_t,deemon_result,returnp(int64_t)) != 0) goto on_err; break;
   case FFI_TYPE_STRUCT:set_structured_result:
    if DEE_UNLIKELY(DeeStructured_SetData(ff_type->fft_return_type,returnp(void),deemon_result) != 0) goto on_err;
    break;
   case FFI_TYPE_POINTER:
    switch (ff_type->fft_return_kind) {
     case DEE_FOREIGN_RETURN_KIND_STRUCTURED: goto set_structured_result;
     case DEE_FOREIGN_RETURN_KIND_OBJECT:
      Dee_INHERIT_REF(returnl(DeeObject *),deemon_result);
      return;
     case DEE_FOREIGN_RETURN_KIND_OBJECT_ADDREF:
      // Make sure we are not returning a unique object
      // >> Because if we are, then the caller would receive a broken/dead object
      if (DeeObject_IS_UNIQUE(deemon_result)) {
       DeeError_RuntimeErrorf("Foreign function closure %q with [[add_result_reference]] "
                              "cannot return a unique instance of %q",
                              DeeType_NAME(ff_type),DeeType_NAME(Dee_TYPE(deemon_result)));
       goto on_err;
      }
      returnl(DeeObject *) = deemon_result;
      break;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
     case DEE_FOREIGN_RETURN_KIND_WIDESTRING_FROM_POINTER:  if DEE_UNLIKELY(DeeObject_GetPointerEx(deemon_result,DeeObject_TYPE(Dee_rt_wchar_t),returnp(void *)) != 0) goto on_err; break;
     case DEE_FOREIGN_RETURN_KIND_UTF8STRING_FROM_POINTER:  if DEE_UNLIKELY(DeeObject_GetPointerEx(deemon_result,DeeObject_TYPE(Dee_rt_char),returnp(void *)) != 0) goto on_err; break;
     case DEE_FOREIGN_RETURN_KIND_UTF16STRING_FROM_POINTER: if DEE_UNLIKELY(DeeObject_GetPointerEx(deemon_result,DeeObject_TYPE(Dee_rt_char16_t),returnp(void *)) != 0) goto on_err; break;
     case DEE_FOREIGN_RETURN_KIND_UTF32STRING_FROM_POINTER: if DEE_UNLIKELY(DeeObject_GetPointerEx(deemon_result,DeeObject_TYPE(Dee_rt_char32_t),returnp(void *)) != 0) goto on_err; break;
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
     case DEE_FOREIGN_RETURN_KIND_WIDESTRING_FROM_POINTER:
#if DEE_CONFIG_HAVE_ENCODING_WIDE
      if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(deemon_result,&DeeWideString_Type) != 0) goto on_err;
      returnl(Dee_WideChar *) = DeeWideString_STR(deemon_result);
#else /* DEE_CONFIG_HAVE_ENCODING_WIDE */
      DeeError_NotImplemented_str("DEE_CONFIG_HAVE_ENCODING_WIDE");
      goto on_err;
#endif /* !DEE_CONFIG_HAVE_ENCODING_WIDE */
      break;
     case DEE_FOREIGN_RETURN_KIND_UTF8STRING_FROM_POINTER:
#if DEE_CONFIG_HAVE_ENCODING_UTF8
      if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(deemon_result,&DeeUtf8String_Type) != 0) goto on_err;
      returnl(Dee_Utf8Char *) = DeeUtf8String_STR(deemon_result);
#else /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
      DeeError_NotImplemented_str("DEE_CONFIG_HAVE_ENCODING_UTF8");
      goto on_err;
#endif /* !DEE_CONFIG_HAVE_ENCODING_UTF8 */
      break;
     case DEE_FOREIGN_RETURN_KIND_UTF16STRING_FROM_POINTER:
#if DEE_CONFIG_HAVE_ENCODING_UTF16
      if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(deemon_result,&DeeUtf16String_Type) != 0) goto on_err;
      returnl(Dee_Utf16Char *) = DeeUtf16String_STR(deemon_result);
#else /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
      DeeError_NotImplemented_str("DEE_CONFIG_HAVE_ENCODING_UTF16");
      goto on_err;
#endif /* !DEE_CONFIG_HAVE_ENCODING_UTF16 */
      break;
     case DEE_FOREIGN_RETURN_KIND_UTF32STRING_FROM_POINTER:
#if DEE_CONFIG_HAVE_ENCODING_UTF32
      if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(deemon_result,&DeeUtf32String_Type) != 0) goto on_err;
      returnl(Dee_Utf32Char *) = DeeUtf32String_STR(deemon_result);
#else /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
      DeeError_NotImplemented_str("DEE_CONFIG_HAVE_ENCODING_UTF32");
      goto on_err;
#endif /* !DEE_CONFIG_HAVE_ENCODING_UTF32 */
      break;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
     default:ret_zero: {
      memset(returnp(void),0,ff_type->fft_ffi_return_type->size);
     } break;
    } break;
   default: goto ret_zero;
  }
  Dee_DECREF(deemon_result);
 } else {
on_err:
  if ((ff_type->fft_return_kind != DEE_FOREIGN_RETURN_KIND_OBJECT &&
       ff_type->fft_return_kind != DEE_FOREIGN_RETURN_KIND_OBJECT_ADDREF)) {
   // Return zero-initialized memory on error
   memset(returnp(void),0,ff_type->fft_ffi_return_type->size);
ignore_error:
   // Ignore errors that occurred inside the closure callback (since we have no way of reporting them)
   DeeError_Print("[Ignored] Unhandled exception in foreign function closure callback",1);
  } else if ((ff_type->fft_flags&DEE_FUNCTIONFLAGS_FLAG_NOEXCEPT)!=0) {
   // Return none on error
   returnl(DeeObject *) = Dee_None;
   if (ff_type->fft_return_kind == DEE_FOREIGN_RETURN_KIND_OBJECT)
    Dee_INCREF(Dee_None); // Reference inherited by the caller
   goto ignore_error;
  } else {
   // The caller is responsible for handling this deemon error
   // (This assumes that the is going to handle this error)
   returnl(DeeObject *) = NULL;
  }
 }
#undef returnl
#undef returnp
}
DEE_COMPILER_MSVC_WARNING_POP

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) DeeForeignFunctionClosure_Init(
 DEE_A_INOUT DeeForeignFunctionClosureObject *self,
 DEE_A_IN DeeForeignFunctionTypeObject *ff_type, DEE_A_INOUT DeeObject *callback) {
#if !FFI_CLOSURES
 (void)self,ff_type,callback;
 DeeError_NotImplemented_str("libffi does not implement closures on this platform");
 return -1;
#else
 ffi_status error;
 DEE_ASSERT(DeeObject_Check(self) && DeeForeignFunctionClosure_Check(self));
 DEE_ASSERT(DeeObject_Check(ff_type) && DeeForeignFunctionType_Check(ff_type));
 DEE_ASSERT(DeeObject_Check(callback));
 self->ffc_write = (ffi_closure *)ffi_closure_alloc(sizeof(ffi_closure),&self->ffc_exec);
 if (!self->ffc_write) { DeeError_NoMemory(); return -1; }
 // TODO: Can't we just re-use the cif from 'ff_type'?
#if !DEE_FOREIGN_FUNCTION_CLOSURE_REUSE_CIF
 {
  ffi_abi abi = _DeeForeignFunction_GetFFIAbiFromFlags(ff_type->fft_flags);
  error = ffi_prep_cif(&self->ffc_ffi_cif,abi,(unsigned int)ff_type->fft_arg_type_c,
                       ff_type->fft_ffi_return_type,ff_type->fft_ffi_arg_type_v);
 }
 if (error != FFI_OK) {
  DeeError_RuntimeErrorf("ffi_prep_cif(%q) failed: %d",DeeType_NAME(ff_type),(int)error);
  goto err_write;
 }
#endif
#if defined(X86_DARWIN) || defined(POWERPC_DARWIN)
#if DEE_FOREIGN_FUNCTION_CLOSURE_REUSE_CIF
 error = ffi_prep_closure(self->ffc_write,&ff_type->fft_ffi_cif,
                          (void (*)(ffi_cif*,void*,void**,void*))&raw_closure_callback,
                          self);
#else
 error = ffi_prep_closure(self->ffc_write,&self->ffc_ffi_cif,
                          (void (*)(ffi_cif*,void*,void**,void*))&raw_closure_callback,
                          self);
#endif
#else
#if DEE_FOREIGN_FUNCTION_CLOSURE_REUSE_CIF
 error = ffi_prep_closure_loc(self->ffc_write,&ff_type->fft_ffi_cif,
                              (void (*)(ffi_cif*,void*,void**,void*))&raw_closure_callback,
                              self,self->ffc_exec);
#else
 error = ffi_prep_closure_loc(self->ffc_write,&self->ffc_ffi_cif,
                              (void (*)(ffi_cif*,void*,void**,void*))&raw_closure_callback,
                              self,self->ffc_exec);
#endif
#endif
 if (error != FFI_OK) {
  DeeError_RuntimeErrorf("ffi_prep_closure(%q) failed: %d",
                         DeeType_NAME(ff_type),(int)error);
#if !DEE_FOREIGN_FUNCTION_CLOSURE_REUSE_CIF
err_write:
#endif
  ffi_closure_free(self->ffc_write);
  return -1;
 }
 Dee_INCREF(self->ffc_type = ff_type);
 Dee_INCREF(self->ffc_callback = callback);
 return 0;
#endif
}
static void _deeforeignfunctionclosure_tp_dtor(
 DeeForeignFunctionClosureObject *self) {
 ffi_closure_free(self->ffc_write);
 Dee_DECREF(self->ffc_callback);
 Dee_DECREF(self->ffc_type);
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeForeignFunctionClosureObject) *DeeForeignFunctionClosure_New(
 DEE_A_IN_TYPEOBJECT(DeeForeignFunctionTypeObject) const *ff_type, DEE_A_INOUT DeeObject *callback) {
 DeeForeignFunctionClosureObject *result;
 DEE_ASSERT(DeeObject_Check(ff_type) && DeeForeignFunctionType_Check(ff_type));
 DEE_ASSERT(DeeObject_Check(callback));
 if ((result = DeeObject_MALLOC(DeeForeignFunctionClosureObject)) != NULL) {
  DeeObject_INIT(result,&DeeForeignFunctionClosure_Type);
  if (DeeForeignFunctionClosure_Init(result,(
   DeeForeignFunctionTypeObject *)ff_type,callback) != 0) {
   _DeeObject_DELETE(&DeeForeignFunctionClosure_Type,result);
   return NULL;
  }
 }
 return (DeeObject *)result;
}




DEE_A_RET_NEVER_NULL DeeForeignFunction DeeForeignFunctionClosure_Func(
 DEE_A_IN_OBJECT(DeeForeignFunctionClosureObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeForeignFunctionClosure_Check(self));
 return DeeForeignFunctionClosure_FUNC(self);
}



static int _deeforeignfunctionclosure_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeForeignFunctionClosureObject *self, DeeObject *args) {
 DeeForeignFunctionTypeObject *ff_type; DeeObject *callback;
 if (DeeTuple_Unpack(args,"oo:foreign_function_closure",&ff_type,&callback) != 0) return -1;
 if (DeeObject_InplaceGetInstance(&ff_type,&DeeForeignFunctionType_Type) != 0) return -1;
 return DeeForeignFunctionClosure_Init(self,ff_type,callback);
}

DEE_VISIT_PROC(_deeforeignfunctionclosure_tp_visit,DeeForeignFunctionClosureObject *self) {
 Dee_VISIT(self->ffc_callback);
 Dee_VISIT(self->ffc_type);
}
static DeeObject *_deeforeignfunctionclosure_tp_call(
 DeeForeignFunctionClosureObject *self, DeeObject *args) {
#if 1
DEE_COMPILER_MSVC_WARNING_PUSH(4054 4055)
 return (*DeeType_GET_SLOT(self->ffc_type,tp_p_call))(
  (DeeStructuredTypeObject *)self->ffc_type,(void *)self->ffc_funptr,args);
DEE_COMPILER_MSVC_WARNING_POP
#else
 return DeeObject_Call(self->ffc_callback,args);
#endif
}

static struct DeeMemberDef const _deeforeignfunctionclosure_tp_members[] = {
 DEE_MEMBERDEF_RO_v100(DeeForeignFunctionClosureObject,ffc_funptr,p(void)),
 DEE_MEMBERDEF_RO_v100(DeeForeignFunctionClosureObject,ffc_callback,object),
 DEE_MEMBERDEF_RO_v100(DeeForeignFunctionClosureObject,ffc_type,object),
 DEE_MEMBERDEF_END_v100
};


static int DEE_CALL _deeforeignfunctionclosure_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeForeignFunctionClosureObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 DeeForeignFunctionTypeObject *ff_type; DeeObject *callback; int result;
 if ((ff_type = (DeeForeignFunctionTypeObject *)DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if (DeeError_TypeError_CheckType((DeeObject *)ff_type,&DeeForeignFunctionType_Type) != 0) {err_ff_type: Dee_DECREF(ff_type); return -1; }
 if ((callback = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) goto err_ff_type;
 result = DeeForeignFunctionClosure_Init(self,ff_type,callback);
 Dee_DECREF(callback);
 Dee_DECREF(ff_type);
 return result;
}

static int DEE_CALL _deeforeignfunctionclosure_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeForeignFunctionClosureObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 if (DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)self->ffc_type) != 0) return -1;
 return DeeMarshal_WriteObjectWithMap(outfile,map,(DeeObject *)self->ffc_callback);
}

static struct DeeTypeMarshal _deeforeignfunctionclosure_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(foreign_function_closure)),
 member(&_deeforeignfunctionclosure_tp_marshal_ctor),
 member(&_deeforeignfunctionclosure_tp_marshal_put));


DeeTypeObject DeeForeignFunctionClosure_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("foreign_function_closure"),null,member(
  // 'DEE_TYPE_FLAG_NO_LIFETIME_EFFECT' is required for unused variable optimization
  DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|
  DEE_TYPE_FLAG_DONT_COPY),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeForeignFunctionClosureObject),null,null,null,null,
  member(&_deeforeignfunctionclosure_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deeforeignfunctionclosure_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(
  member(&_deeforeignfunctionclosure_tp_call),
  member(&_deeforeignfunctionclosure_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deeforeignfunctionclosure_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deeforeignfunctionclosure_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};

DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */

#endif /* !GUARD_DEEMON_FOREIGN_FUNCTION_CLOSURE_INL */
