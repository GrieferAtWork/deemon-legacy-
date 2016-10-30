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
#include "foreign_function.c"

DEE_DECL_BEGIN
#define DO_RETURN_DEE_FOREIGN_RETURN_KIND_NONE
#define VARARGS
#endif

#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
//////////////////////////////////////////////////////////////////////////
// -- deemon -> foreign_function --
// - This file implements the callbacks required to
//   execute any foreign function from deemon.
// NOTE: This file is included multiple times with different environment macros.

#ifdef VARARGS
#ifdef NOEXCEPT
static DeeObject *_deeforeignfunction_tp_p_call_nv
#else
static DeeObject *_deeforeignfunction_tp_p_call_v
#endif
#else
#ifdef NOEXCEPT
static DeeObject *_deeforeignfunction_tp_p_call_n
#else
static DeeObject *_deeforeignfunction_tp_p_call
#endif
#endif
(DeeForeignFunctionTypeObject *tp_self,
 DeeForeignFunction self, DeeTupleObject *args) {
#ifdef VARARGS
 Dee_size_t totalargc,n_varargs;
 DeeTypeObject *dee_va_type;
 DeeObject *dee_va_arg;
 ffi_type **dee_va_ffi_types,**dee_va_ffi_types_end;
#endif
 void *wbuf,**argp_iter;
 union argument *iter,*end;
 DeeObject **argv;
#if !DEE_HAVE_ALLOCA
 DeeObject *result;
#endif
 ffi_type **ffi_argtyv;
#define ret_mem  wbuf // Return memory is located at the start of the wbuffer
#ifdef VARARGS
 totalargc = DeeTuple_SIZE(args);
 if (totalargc < tp_self->fft_arg_type_c)
#else
 if (DeeTuple_SIZE(args) != tp_self->fft_arg_type_c)
#endif
 {// Check argument count
#ifdef VARARGS
  DeeError_TypeError_ArgCountExpectedMin(tp_self->fft_arg_type_c,DeeTuple_SIZE(args));
#else
  DeeError_TypeError_ArgCountExpected(tp_self->fft_arg_type_c,DeeTuple_SIZE(args));
#endif
  return NULL;
 }
 if (!self) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Tried to call null-function-pointer to %q",
                      DeeType_NAME(tp_self));
  return NULL;
 }
#ifdef VARARGS
 n_varargs = DeeTuple_SIZE(args)-tp_self->fft_arg_type_c;
#define va_data_size (totalargc*(\
 sizeof(union argument)+ /* Storage for argument value. */\
 sizeof(void *)+         /* Pointer to argument data. */\
 sizeof(ffi_type *)      /* Pointer to argument types. */\
))
// Pointer the memory used for the argument types
#define va_argmem_mem \
 (union argument *)((Dee_uintptr_t)wbuf+tp_self->fft_woff_argmem)
#define va_argptr_mem \
 (void **)((Dee_uintptr_t)wbuf+tp_self->fft_woff_argmem+(totalargc*sizeof(union argument)))
#define va_argtypes_mem \
 (ffi_type **)((Dee_uintptr_t)wbuf+tp_self->fft_woff_argmem+(totalargc*(sizeof(union argument)+sizeof(void *))))
#endif


#if DEE_HAVE_ALLOCA
#define HANDLE_ERROR   return NULL
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6255)
#endif
#ifdef VARARGS
 wbuf = alloca(tp_self->fft_wsize+va_data_size);
#else
 wbuf = alloca(tp_self->fft_wsize);
#endif
#ifdef _PREFAST_
#pragma warning(pop)
#endif
#else
#define HANDLE_ERROR   goto err
#ifdef VARARGS
 if ((wbuf = malloc_nz(tp_self->fft_wsize+va_data_size)) == NULL)
#else
 if ((wbuf = malloc_nz(tp_self->fft_wsize)) == NULL)
#endif
 {
  DeeError_NoMemory();
  return NULL;
 }
#endif
 // Initialize arguments
 iter = (union argument *)((void *)((Dee_uintptr_t)wbuf+tp_self->fft_woff_argmem));
#ifdef VARARGS
 end  = (union argument *)((void *)((Dee_uintptr_t)wbuf+tp_self->fft_woff_variadic_argmem));
 argp_iter = (void **)((Dee_uintptr_t)iter+totalargc*sizeof(union argument));
 argv = DeeTuple_ELEM(args);
 DEE_ASSERT(iter == va_argmem_mem);
 DEE_ASSERT(argp_iter == va_argptr_mem);
#else
 end = (union argument *)((void *)((Dee_uintptr_t)wbuf+tp_self->fft_woff_argptr));
 DEE_ASSERT(end == iter+tp_self->fft_arg_type_c && "Invalid wbuf cache");
 argp_iter = (void **)end;
#endif
 if (iter != end) {
#define argt  tp_self->fft_arg_type_v[(Dee_size_t)(argv-DeeTuple_ELEM(args))]
#ifndef VARARGS
  argv = DeeTuple_ELEM(args);
#endif
  ffi_argtyv = tp_self->fft_ffi_arg_type_v;
  while (1) {
   *argp_iter = (void *)iter;
   switch ((*ffi_argtyv)->type) {
    case FFI_TYPE_VOID: break;
    case FFI_TYPE_INT:
     if (DeeObject_Cast(int,*argv,&iter->i) != 0) HANDLE_ERROR;
     break;
    case FFI_TYPE_FLOAT:
     if (DeeObject_Cast(float,*argv,&iter->f) != 0) HANDLE_ERROR;
     break;
    case FFI_TYPE_DOUBLE:
     if (DeeObject_Cast(double,*argv,&iter->d) != 0) HANDLE_ERROR;
     break;
#if FFI_TYPE_LONGDOUBLE != FFI_TYPE_DOUBLE
    case FFI_TYPE_LONGDOUBLE:
     if (DeeObject_Cast(ldouble,*argv,&iter->ld) != 0) HANDLE_ERROR;
     break;
#endif
    case FFI_TYPE_UINT8:  if (DeeObject_Cast(Dee_uint8_t,*argv,&iter->ui8) != 0) HANDLE_ERROR; break;
    case FFI_TYPE_SINT8:  if (DeeObject_Cast(Dee_int8_t,*argv,&iter->i8) != 0) HANDLE_ERROR; break;
    case FFI_TYPE_UINT16: if (DeeObject_Cast(Dee_uint16_t,*argv,&iter->ui16) != 0) HANDLE_ERROR; break;
    case FFI_TYPE_SINT16: if (DeeObject_Cast(Dee_int16_t,*argv,&iter->i16)) HANDLE_ERROR; break;
    case FFI_TYPE_UINT32: if (DeeObject_Cast(Dee_uint32_t,*argv,&iter->ui32)) HANDLE_ERROR; break;
    case FFI_TYPE_SINT32: if (DeeObject_Cast(Dee_int32_t,*argv,&iter->i32)) HANDLE_ERROR; break;
    case FFI_TYPE_UINT64: if (DeeObject_Cast(Dee_uint64_t,*argv,&iter->ui64)) HANDLE_ERROR; break;
    case FFI_TYPE_SINT64: if (DeeObject_Cast(Dee_int64_t,*argv,&iter->i64)) HANDLE_ERROR; break;
    case FFI_TYPE_STRUCT:
     // TODO: Type check
     iter->p = DeeStructured_DATA(*argv);
     break;
    case FFI_TYPE_POINTER:
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
     if (DeeLValueType_Check(argt)) {
      if (DeeObject_GetLValue(*argv,(DeeTypeObject *)
       DeeType_GET_SLOT(argt,tp_lval_lvalue_base),&iter->p) != 0) HANDLE_ERROR;
     } else if (DeePointerType_Check(argt)) {
      if (DeeObject_GetPointerEx(*argv,(DeeTypeObject *)
       DeeType_GET_SLOT(argt,tp_ptr_pointer_base),
       &iter->p) != 0) HANDLE_ERROR;
     } else
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
     {
      DEE_ASSERT(argt == &DeeObject_Type);
      iter->p = *argv;
     }
     break;
    default:
     iter->ui64 = 0;
     break;
   }
#ifdef VARARGS
   ++argv,++argp_iter;
#endif
   if (++iter == end) break;
#ifndef VARARGS
   ++argv,++argp_iter;
#endif
   ++ffi_argtyv;
  }
#undef argt
 }
#ifdef VARARGS
 if (n_varargs) {
  dee_va_ffi_types = va_argtypes_mem;
  // Copy non-vararg types
  memcpy(dee_va_ffi_types,tp_self->fft_ffi_arg_type_v,
         tp_self->fft_arg_type_c*sizeof(ffi_type *));
  dee_va_ffi_types += tp_self->fft_arg_type_c;
  dee_va_ffi_types_end = dee_va_ffi_types+n_varargs;
  DEE_ASSERT(dee_va_ffi_types != dee_va_ffi_types_end);
  while (1) {
   dee_va_arg = *argv,dee_va_type = Dee_TYPE(dee_va_arg);
   if (0
#if DEE_CONFIG_HAVE_ENCODING_UTF8
    || dee_va_type == &DeeUtf8String_Type
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_WIDE
    || dee_va_type == &DeeWideString_Type
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
    || dee_va_type == &DeeUtf16String_Type
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
    || dee_va_type == &DeeUtf32String_Type
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
    ) {
    // String argument --> cast to 'char *'
    // NOTE: We abuse the fact that all string types share the same base address.
    //       Therefor we only need one check for string types.
    *dee_va_ffi_types = &ffi_type_pointer;
    iter->p = (void *)DeeString_STR(dee_va_arg);
    *argp_iter = (void *)iter;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   } else if (DeePointerType_Check(dee_va_type)) {
    // Pointer argument --> cast to 'void *'
    *dee_va_ffi_types = &ffi_type_pointer;
    iter->p = DeePointer_ADDR(dee_va_arg);
    *argp_iter = (void *)iter;
   } else if (DeeLValueType_Check(dee_va_type)) {
    // Lvalue argument --> cast to lvalue-base
    dee_va_type = DeeLValueType_BASE(dee_va_type);
    if ((*dee_va_ffi_types = DeePointerType_Check(dee_va_type) ? &ffi_type_pointer
     : _DeeForeignFunction_GetStructuredFFIType(dee_va_type)) == NULL) HANDLE_ERROR;
    *argp_iter = DeeLValue_ADDR(dee_va_arg);
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
   } else if (DeeNone_Check(dee_va_type)) {
    // none --> NULL pointer
    *dee_va_ffi_types = &ffi_type_pointer;
    iter->p = NULL;
    *argp_iter = (void *)iter;
   } else {
    if ((*dee_va_ffi_types = _DeeForeignFunction_GetStructuredFFIType(dee_va_type)) == NULL) HANDLE_ERROR;
    // Prefer hard copies
    switch ((*dee_va_ffi_types)->size) {
     case 1:
      iter->ui8 = *(Dee_uint8_t*)DeeStructured_DATA(dee_va_arg);
def_var_data:
      *argp_iter = (void *)iter;
      break;
     case 2:
      iter->ui16 = *(Dee_uint16_t*)DeeStructured_DATA(dee_va_arg);
      goto def_var_data;
     case 4:
      iter->ui32 = *(Dee_uint32_t*)DeeStructured_DATA(dee_va_arg);
      goto def_var_data;
     case 8:
      iter->ui64 = *(Dee_uint64_t*)DeeStructured_DATA(dee_va_arg);
      goto def_var_data;

     default:
      // Inline pointer to structure data
      *argp_iter = (void *)DeeStructured_DATA(dee_va_arg);
      break;
    }
   }
   if (++dee_va_ffi_types == dee_va_ffi_types_end) break;
   ++argp_iter,++argv,++iter;
  }
 }
#endif
 DeeError_PUSH_STATE() {
#ifndef NOEXCEPT
  DEE_SIGNALHANDLER_BEGIN {
#endif
#ifdef VARARGS
   if (n_varargs) {
    ffi_cif va_cif;
    if (ffi_prep_cif_var(&va_cif,tp_self->fft_ffi_cif.abi,
     (unsigned int)tp_self->fft_arg_type_c,(unsigned int)totalargc,
     tp_self->fft_ffi_return_type,va_argtypes_mem) != FFI_OK) {
     DEE_SIGNALHANDLER_DONE;
     DeeError_RuntimeError("ffi_prep_cif_var failed");
     HANDLE_ERROR;
    }
    ffi_call(&va_cif,(void(*)(void))self,ret_mem,va_argptr_mem);
   } else
#endif
   {
    ffi_call(&tp_self->fft_ffi_cif,(void(*)(void))self,ret_mem,
            (void **)((Dee_uintptr_t)wbuf+tp_self->fft_woff_argptr));
   }
#ifndef NOEXCEPT
  } DEE_SIGNALHANDLER_END ({
   HANDLE_ERROR;
  });
#endif
#if DEE_HAVE_ALLOCA
#define RETURN    return
#else
#define RETURN(x) do{result=(x);goto end;}while(0)
#endif
#ifndef NOEXCEPT
  // Check that no deemon error was thrown
#if 1
  if DEE_UNLIKELY(_temp_state.esd_threadself->t_exception != NULL)
#else
  if DEE_UNLIKELY(DeeError_OCCURRED())
#endif
  {
   // The foreign code threw a deemon error.
   // >> Now we have to forward that error to the caller
   // NOTE: In the case of usually returning an object with inherited reference, we need to try and decref the result first
   if (tp_self->fft_return_kind == DEE_FOREIGN_RETURN_KIND_OBJECT) {
    Dee_XDECREF(*(DeeObject **)ret_mem);
   }
   DeeError_BREAK_STATE();
   RETURN(NULL);
  }
#else
  // Just dump all errors if there are any
  // >> The user said this function was noexcept, and that's what we're enforcing here!
  DeeError_Handled();
#endif
 } DeeError_POP_STATE();
 switch (tp_self->fft_return_kind) {
  case DEE_FOREIGN_RETURN_KIND_NONE:
   RETURN(DeeNone_New());
  case DEE_FOREIGN_RETURN_KIND_STRUCTURED:
   RETURN(DeeStructured_NewFromData(tp_self->fft_return_type,ret_mem));
  case DEE_FOREIGN_RETURN_KIND_OBJECT_ADDREF:
   // Make sure a valid object is returned
   if (!*(DeeObject **)ret_mem) {
    DeeError_SET_STRING(&DeeErrorType_ValueError,
                        "foreign_function_call returned an invalid object");
    RETURN(NULL);
   }
   Dee_INCREF(*(DeeObject **)ret_mem);
   RETURN(*(DeeObject **)ret_mem);
  case DEE_FOREIGN_RETURN_KIND_OBJECT:
   // *ditto*
   if (!*(DeeObject **)ret_mem) {
    DeeError_SET_STRING(&DeeErrorType_ValueError,
                        "foreign_function_call returned an invalid object");
   }
   RETURN(*(DeeObject **)ret_mem);
  case DEE_FOREIGN_RETURN_KIND_WIDESTRING_FROM_POINTER: {
   Dee_WideChar *const str_p = *(Dee_WideChar **)ret_mem;
#if DEE_CONFIG_HAVE_ENCODING_WIDE
   RETURN(str_p ? DeeWideString_New(str_p) : DeeWideString_NewEmpty());
#else
   RETURN(str_p ? DeeString_FromWideString(str_p) : DeeString_NewEmpty());
#endif
  }
  case DEE_FOREIGN_RETURN_KIND_UTF8STRING_FROM_POINTER: {
   Dee_Utf8Char *const str_p = *(Dee_Utf8Char **)ret_mem;
#if DEE_CONFIG_HAVE_ENCODING_UTF8
   RETURN(str_p ? DeeUtf8String_New(str_p) : DeeUtf8String_NewEmpty());
#else
   RETURN(str_p ? DeeString_FromUtf8String(str_p) : DeeString_NewEmpty());
#endif
  }
  case DEE_FOREIGN_RETURN_KIND_UTF16STRING_FROM_POINTER: {
   Dee_Utf16Char *const str_p = *(Dee_Utf16Char **)ret_mem;
#if DEE_CONFIG_HAVE_ENCODING_UTF16
   RETURN(str_p ? DeeUtf16String_New(str_p) : DeeUtf16String_NewEmpty());
#else
   RETURN(str_p ? DeeString_FromUtf16String(str_p) : DeeString_NewEmpty());
#endif
  }
  case DEE_FOREIGN_RETURN_KIND_UTF32STRING_FROM_POINTER: {
   Dee_Utf32Char *const str_p = *(Dee_Utf32Char **)ret_mem;
#if DEE_CONFIG_HAVE_ENCODING_UTF32
   RETURN(str_p ? DeeUtf32String_New(str_p) : DeeUtf32String_NewEmpty());
#else
   RETURN(str_p ? DeeString_FromUtf32String(str_p) : DeeString_NewEmpty());
#endif
  }
  default: DEE_BUILTIN_UNREACHABLE();
 }
#undef HANDLE_ERROR
#undef RETURN
#undef ret_mem
#if !DEE_HAVE_ALLOCA
end: free_nn(wbuf);
 return result;
err: result = NULL; goto end;
#endif
}

#ifdef VARARGS
#undef totalargc
#undef va_data_size
#endif
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
