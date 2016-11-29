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
#ifndef GUARD_DEEMON_FOREIGN_FUNCTION_C
#define GUARD_DEEMON_FOREIGN_FUNCTION_C 1
#define DEE_LIMITED_API 1
#define DEE_UUID_PROTOTYPES 1

#include <deemon/__conf.inl>
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
#include DEE_INCLUDE_MEMORY_API()

// include/*
#include <deemon/array.h>
#include <deemon/bool.h>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/float.h>
#include <deemon/foreign_function.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_visit.h>
#include <deemon/integer.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/optional/uuid.h>
#include <deemon/signalhandler.h>
#include <deemon/struct.h>
#include <deemon/tuple.h>

// src/*
#include <deemon/marshal_data.h>
#include <deemon/__alloca.inl>

#if defined(X86_ANY)
#ifdef X86_WIN32
#define DEE_HAVE_FFI_SYSV     1
#define DEE_HAVE_FFI_STDCALL  1
#define DEE_HAVE_FFI_THISCALL 1
#define DEE_HAVE_FFI_FASTCALL 1
#define DEE_HAVE_FFI_MS_CDECL 1
#define DEE_HAVE_FFI_PASCAL   1
#define DEE_HAVE_FFI_REGISTER 1
#elif defined(X86_WIN64)
#define DEE_HAVE_FFI_WIN64    1
#else
#define DEE_HAVE_FFI_SYSV     1
#define DEE_HAVE_FFI_UNIX64   1
#define DEE_HAVE_FFI_THISCALL 1
#define DEE_HAVE_FFI_FASTCALL 1
#define DEE_HAVE_FFI_STDCALL  1
#define DEE_HAVE_FFI_PASCAL   1
#define DEE_HAVE_FFI_REGISTER 1
#endif
#elif 0
// TODO ...
#endif

#ifndef DEE_HAVE_FFI_SYSV
#define DEE_HAVE_FFI_SYSV     0
#endif
#ifndef DEE_HAVE_FFI_STDCALL
#define DEE_HAVE_FFI_STDCALL  0
#endif
#ifndef DEE_HAVE_FFI_THISCALL
#define DEE_HAVE_FFI_THISCALL 0
#endif
#ifndef DEE_HAVE_FFI_FASTCALL
#define DEE_HAVE_FFI_FASTCALL 0
#endif
#ifndef DEE_HAVE_FFI_MS_CDECL
#define DEE_HAVE_FFI_MS_CDECL 0
#endif
#ifndef DEE_HAVE_FFI_PASCAL
#define DEE_HAVE_FFI_PASCAL   0
#endif
#ifndef DEE_HAVE_FFI_REGISTER
#define DEE_HAVE_FFI_REGISTER 0
#endif
#ifndef DEE_HAVE_FFI_WIN64
#define DEE_HAVE_FFI_WIN64    0
#endif
#ifndef DEE_HAVE_FFI_UNIX64
#define DEE_HAVE_FFI_UNIX64   0
#endif


DEE_DECL_BEGIN


DEE_STATIC_INLINE(int) DeeForeignFunctionType_Init(
 DeeForeignFunctionTypeObject *self,
 Dee_funflags_t flags, DeeTypeObject const *return_type,
 Dee_size_t arg_types_c, DeeTypeObject const *const *arg_types_v);
DEE_A_RET_TYPEOBJECT_EXCEPT_REF(DeeForeignFunctionTypeObject) *_DeeForeignFunctionType_New(
 DEE_A_IN Dee_funflags_t flags, DEE_A_IN DeeTypeObject const *return_type,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeTypeObject const *const *argv) {
 DeeForeignFunctionTypeObject *result;
 if ((result = DeeObject_MALLOC(DeeForeignFunctionTypeObject)) != NULL) {
  DeeObject_INIT(result,&DeeForeignFunctionType_Type);
  if (DeeForeignFunctionType_Init(result,flags,
   (DeeTypeObject *)return_type,argc,argv) != 0) {
   _DeeObject_DELETE(&DeeForeignFunctionType_Type,result);
   result = NULL;
  }
 }
 return (DeeTypeObject *)result;
}


#define DEE_FFI_STRUCTURED_TYPE_CASES(dee_type)\
 if ((dee_type) == (DeeTypeObject *)&DeeBool_Type) return &DEE_PP_CAT_3(ffi_type_,uint,DEE_PP_MUL8(DEE_CONFIG_RT_SIZEOF_BOOL));\
 if ((dee_type) == (DeeTypeObject *)&DeeNone_Type) return &ffi_type_void;\
 if ((dee_type) == (DeeTypeObject *)&DeeInt8_Type || (dee_type) == (DeeTypeObject *)&DeeAtomicInt8_Type) return &ffi_type_sint8;\
 if ((dee_type) == (DeeTypeObject *)&DeeInt16_Type || (dee_type) == (DeeTypeObject *)&DeeAtomicInt16_Type) return &ffi_type_sint16;\
 if ((dee_type) == (DeeTypeObject *)&DeeInt32_Type || (dee_type) == (DeeTypeObject *)&DeeAtomicInt32_Type) return &ffi_type_sint32;\
 if ((dee_type) == (DeeTypeObject *)&DeeInt64_Type || (dee_type) == (DeeTypeObject *)&DeeAtomicInt64_Type) return &ffi_type_sint64;\
 if ((dee_type) == (DeeTypeObject *)&DeeUInt8_Type || (dee_type) == (DeeTypeObject *)&DeeAtomicUInt8_Type) return &ffi_type_uint8;\
 if ((dee_type) == (DeeTypeObject *)&DeeUInt16_Type || (dee_type) == (DeeTypeObject *)&DeeAtomicUInt16_Type) return &ffi_type_uint16;\
 if ((dee_type) == (DeeTypeObject *)&DeeUInt32_Type || (dee_type) == (DeeTypeObject *)&DeeAtomicUInt32_Type) return &ffi_type_uint32;\
 if ((dee_type) == (DeeTypeObject *)&DeeUInt64_Type || (dee_type) == (DeeTypeObject *)&DeeAtomicUInt64_Type) return &ffi_type_uint64;\
 if ((dee_type) == (DeeTypeObject *)&DeeFloat_Type) return &ffi_type_float;\
 if ((dee_type) == (DeeTypeObject *)&DeeDouble_Type) return &ffi_type_double;\
 if ((dee_type) == (DeeTypeObject *)&DeeLDouble_Type) return &ffi_type_longdouble;\
 if (DeeStructType_Check(dee_type)) return DeeStructType_FFITYPE(dee_type);

DEE_STATIC_INLINE(ffi_type *) _DeeForeignFunction_GetStructuredFFIType(DeeTypeObject *dee_type) {
 DEE_FFI_STRUCTURED_TYPE_CASES(dee_type)
 // TODO: Subclasses of DeeStructuredType are structs: FFI_TYPE_STRUCT
 DeeError_SetStringf(&DeeErrorType_TypeError,"Invalid type in native function: %k",dee_type);
 return NULL;
}
DEE_STATIC_INLINE(ffi_type *) _DeeForeignFunction_GetFFIType(
 DEE_A_IN DeeTypeObject const *dee_type) {
 DEE_FFI_STRUCTURED_TYPE_CASES(dee_type)
 if (dee_type == &DeeObject_Type
#if DEE_CONFIG_HAVE_ENCODING_UTF8
  || dee_type == &DeeUtf8String_Type
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_WIDE
  || dee_type == &DeeWideString_Type
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
  || dee_type == &DeeUtf16String_Type
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
  || dee_type == &DeeUtf32String_Type
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  || DeePointerType_Check(dee_type)
  || DeeLValueType_Check(dee_type)
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  ) return &ffi_type_pointer;
 // TODO: Subclasses of DeeStructuredType are structs: FFI_TYPE_STRUCT
 DeeError_SetStringf(&DeeErrorType_TypeError,"Invalid type in native function: %k",dee_type);
 return NULL;
}
DEE_STATIC_INLINE(ffi_type **) _DeeForeignFunction_GetFFITypes(
 Dee_size_t dee_type_c, DeeTypeObject const *const *dee_type_v) {
 ffi_type **result,**iter;
 DEE_ASSERT(dee_type_c);
 while ((result = (ffi_type **)malloc_nz(
  dee_type_c*sizeof(ffi_type *))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 iter = result;
 while (dee_type_c--) if ((*iter++ = _DeeForeignFunction_GetFFIType(
  *dee_type_v++)) == NULL) { free_nn(result); return NULL; }
 return result;
}

DEE_STATIC_INLINE(char *) _DeeForeignFunctionType_GenerateName(
 Dee_funflags_t flags, DeeTypeObject const *return_type,
 Dee_size_t arg_types_c, DeeTypeObject const *const *arg_types_v) {
 char *result;
 DeeStringWriter writer = DeeStringWriter_INIT();
#if 1
 if DEE_UNLIKELY(DeeStringWriter_WriteObjectRepr(&writer,(DeeObject *)return_type) != 0) goto err;
#else
 if DEE_UNLIKELY(DeeStringWriter_WriteString(&writer,DeeType_NAME(return_type)) != 0) goto err;
#endif
 if (DEE_FUNCTIONFLAGS_ISNOEXCEPT(flags) && DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer," [[noexcept]]") != 0)) goto err;
 if (DEE_FUNCTIONFLAGS_ISNORETURN(flags) && DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer," [[noreturn]]") != 0)) goto err;
 switch (DEE_FUNCTIONFLAGS_GETCC(flags)) {
  case DEE_FUNCTIONFLAGS_FLAG_SYSV:     if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer," __sysv") != 0) goto err; break;
  case DEE_FUNCTIONFLAGS_FLAG_STDCALL:  if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer," __stdcall") != 0) goto err; break;
  case DEE_FUNCTIONFLAGS_FLAG_THISCALL: if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer," __thiscall") != 0) goto err; break;
  case DEE_FUNCTIONFLAGS_FLAG_FASTCALL: if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer," __fastcall") != 0) goto err; break;
  case DEE_FUNCTIONFLAGS_FLAG_MS_CDECL: if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer," __cdecl") != 0) goto err; break;
  case DEE_FUNCTIONFLAGS_FLAG_PASCAL:   if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer," __pascal") != 0) goto err; break;
  case DEE_FUNCTIONFLAGS_FLAG_REGISTER: if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer," __register") != 0) goto err; break;
  case DEE_FUNCTIONFLAGS_FLAG_WIN64:    if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer," __win64") != 0) goto err; break;
  case DEE_FUNCTIONFLAGS_FLAG_UNIX64:   if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer," __unix64") != 0) goto err; break;
  default: break;
 }
 if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,"(") != 0) goto err;
 if (arg_types_c) {
  while (1) {
#if 1
   if DEE_UNLIKELY(DeeStringWriter_WriteObjectRepr(&writer,(DeeObject *)*arg_types_v) != 0) goto err;
#else
   if DEE_UNLIKELY(DeeStringWriter_WriteString(&writer,DeeType_NAME(*arg_types_v)) != 0) goto err;
#endif
   if (!--arg_types_c) break; ++arg_types_v;
   if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,", ") != 0) goto err;
  }
  if ((flags&DEE_FUNCTIONFLAGS_FLAG_VARARGS)!=0 &&
      DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,", ...") != 0)) goto err;
 } else {
  if ((flags&DEE_FUNCTIONFLAGS_FLAG_VARARGS)!=0 &&
      DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,"...") != 0)) goto err;
 }
 if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,")") != 0) goto err;
 result = _DeeStringWriter_PackCString(&writer,NULL);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}


static void _dee_noinstance_error(DeeForeignFunctionTypeObject *tp) {
 DeeError_SetStringf(&DeeErrorType_TypeError,
                     "Can't create instances of foreign function %q",
                     DeeType_NAME(tp));
}

static DeeObject *_deeforeignfunction_tp_alloc(DeeForeignFunctionTypeObject *tp) {
 _dee_noinstance_error(tp);
 return NULL;
}
static void _deeforeignfunction_tp_free(
 DeeTypeObject *tp, DeeObject *ob) {
 DEE_ASSERT(
  !"What the hell are you doing?"
   "Why did you think you could free something that can't even be allocated?"
   " - You Idiot!");
 (void)tp,ob;
}
static int _deeforeignfunction_tp_ctor(
 DeeForeignFunctionTypeObject *tp_self,
 DeeForeignFunctionObject *DEE_UNUSED(self)) {
 _dee_noinstance_error(tp_self);
 return -1;
}

//////////////////////////////////////////////////////////////////////////
// Call operator optimizations / Ability to call pointers
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
static DeeObject *_deeforeignfunctionlvalue_tp_call(DeeLValueObject *self, DeeObject *args) {
 DeeStructuredTypeObject *base = DeeType_GET_SLOT(Dee_TYPE(self),tp_lval_lvalue_base);
 return (*DeeType_GET_SLOT(base,tp_p_call))(base,DeeLValue_ADDR(self),args);
}
static DeeObject *_deeforeignfunctionpointer_tp_call(DeePointerObject *self, DeeObject *args) {
 DeeStructuredTypeObject *base = DeeType_GET_SLOT(Dee_TYPE(self),tp_ptr_pointer_base);
 return (*DeeType_GET_SLOT(base,tp_p_call))(base,DeePointer_ADDR(self),args);
}
static DeeObject *_deeforeignfunctionpointer_tp_p_call(
 DeePointerTypeObject *tp_self, DeeForeignFunction *self, DeeObject *args) {
 DeeStructuredTypeObject *base = DeeType_GET_SLOT(tp_self,tp_ptr_pointer_base);
 return (*DeeType_GET_SLOT(base,tp_p_call))(base,*(void **)self,args);
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */

DEE_COMPILER_MSVC_WARNING_PUSH(4054 4055)
static DeeObject *_deeforeignfunction_tp_call(DeeForeignFunctionObject *self, DeeObject *args) {
 DeeStructuredTypeObject *base = (DeeStructuredTypeObject *)Dee_TYPE(self);
 return (*DeeType_GET_SLOT(base,tp_p_call))(base,(void *)DeeForeignFunction_FUNC(self),args);
}
DEE_COMPILER_MSVC_WARNING_POP


union argument {
 int i; float f; double d; long double ld;
 Dee_int8_t  i8;  Dee_uint8_t  ui8;
 Dee_int16_t i16; Dee_uint16_t ui16;
 Dee_int32_t i32; Dee_uint32_t ui32;
 Dee_int64_t i64; Dee_uint64_t ui64;
 void *p;
};
#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif
#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef __INTELLISENSE__
#include "foreign_function.callback_impl.inl"
#define VARARGS
#include "foreign_function.callback_impl.inl"
#undef VARARGS
#define NOEXCEPT
#include "foreign_function.callback_impl.inl"
#define VARARGS
#include "foreign_function.callback_impl.inl"
#undef VARARGS
#undef NOEXCEPT
#else
static DeeObject *_deeforeignfunction_tp_p_call(DeeForeignFunctionTypeObject *tp_self, DeeForeignFunction self, DeeTupleObject *args);
static DeeObject *_deeforeignfunction_tp_p_call_n(DeeForeignFunctionTypeObject *tp_self, DeeForeignFunction self, DeeTupleObject *args);
static DeeObject *_deeforeignfunction_tp_p_call_v(DeeForeignFunctionTypeObject *tp_self, DeeForeignFunction self, DeeTupleObject *args);
static DeeObject *_deeforeignfunction_tp_p_call_nv(DeeForeignFunctionTypeObject *tp_self, DeeForeignFunction self, DeeTupleObject *args);
#endif

DEE_STATIC_INLINE(DEE_A_RET_WUNUSED ffi_abi)
_DeeForeignFunction_GetFFIAbiFromFlags(DEE_A_IN Dee_funflags_t flags) {
 switch (DEE_FUNCTIONFLAGS_GETCC(flags)) {
#if DEE_HAVE_FFI_SYSV
  case DEE_FUNCTIONFLAGS_FLAG_SYSV: return FFI_SYSV;
#endif
#if DEE_HAVE_FFI_STDCALL
  case DEE_FUNCTIONFLAGS_FLAG_STDCALL: return FFI_STDCALL;
#endif
#if DEE_HAVE_FFI_THISCALL
  case DEE_FUNCTIONFLAGS_FLAG_THISCALL: return FFI_THISCALL;
#endif
#if DEE_HAVE_FFI_FASTCALL
  case DEE_FUNCTIONFLAGS_FLAG_FASTCALL: return FFI_FASTCALL;
#endif
#if DEE_HAVE_FFI_MS_CDECL
  case DEE_FUNCTIONFLAGS_FLAG_MS_CDECL: return FFI_MS_CDECL;
#endif
#if DEE_HAVE_FFI_PASCAL
  case DEE_FUNCTIONFLAGS_FLAG_PASCAL: return FFI_PASCAL;
#endif
#if DEE_HAVE_FFI_REGISTER
  case DEE_FUNCTIONFLAGS_FLAG_REGISTER: return FFI_REGISTER;
#endif
#if DEE_HAVE_FFI_WIN64
  case DEE_FUNCTIONFLAGS_FLAG_WIN64: return FFI_WIN64;
#endif
#if DEE_HAVE_FFI_UNIX64
  case DEE_FUNCTIONFLAGS_FLAG_UNIX64: return FFI_UNIX64;
#endif
  default: break;
 }
 return FFI_DEFAULT_ABI;
}


DEE_A_RET_EXCEPT(NULL) DeeTypeObject *
_DeeForeignFunctionType_ArgTypeof(DEE_A_IN DeeTypeObject *tp) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (tp == &DeeWideString_Type) return DeeType_Pointer(DeeObject_TYPE(Dee_WideChar));
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (tp == &DeeUtf8String_Type) return DeeType_Pointer(DeeObject_TYPE(Dee_Utf8Char));
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (tp == &DeeUtf16String_Type) return DeeType_Pointer(DeeObject_TYPE(Dee_Utf16Char));
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (tp == &DeeUtf32String_Type) return DeeType_Pointer(DeeObject_TYPE(Dee_Utf32Char));
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 if (DeeVarArrayType_Check(tp)) return DeeType_Pointer(DeeVarArrayType_BASE(tp));
#endif /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 return tp;
}

DEE_STATIC_INLINE(int) DeeForeignFunctionType_Init(
 DeeForeignFunctionTypeObject *self,
 Dee_funflags_t flags, DeeTypeObject const *return_type,
 Dee_size_t arg_types_c, DeeTypeObject const *const *arg_types_v) {
 DeeTypeObject **arg_copy; ffi_abi abi;
 DeeTypeObject const *const *iter,*const *end;
 char *new_name; ffi_status error;
#define return_kind    self->fft_return_kind
#define is_varargs   ((flags&DEE_FUNCTIONFLAGS_FLAG_VARARGS)!=0)
#define is_noexcept  ((flags&DEE_FUNCTIONFLAGS_FLAG_NOEXCEPT)!=0)
 if ((*DeeType_GET_SLOT(&DeeStructuredType_Type,tp_ctor))(
  &DeeStructuredType_Type,(DeeObject *)self) != 0) return -1;
 self->fft_flags = flags;
 Dee_INCREF(self->fft_return_type = (DeeTypeObject *)return_type);
 if ((self->fft_arg_type_c = arg_types_c) != 0) {
  while ((arg_copy = (DeeTypeObject **)malloc_nz(
   arg_types_c*sizeof(DeeTypeObject *))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
err_0:
   Dee_DECREF(self->fft_return_type);
   (*DeeType_GET_SLOT(&DeeStructuredType_Type,tp_dtor))((DeeObject *)self);
   (*DeeType_GET_SLOT(&DeeType_Type,tp_dtor))((DeeObject *)self);
   return -1;
  }
  self->fft_arg_type_v = arg_copy;
  end = (iter = arg_types_v)+arg_types_c;
  do {
   if ((*arg_copy = _DeeForeignFunctionType_ArgTypeof((DeeTypeObject *)*iter)) == NULL) {
    while (arg_copy != self->fft_arg_type_v) Dee_DECREF(*--arg_copy);
    free_nn(self->fft_arg_type_v);
    goto err_0;
   }
   Dee_INCREF(*arg_copy);
   ++arg_copy;
  } while (++iter != end);
 } else {
  self->fft_arg_type_v = NULL;
 }
 if ((self->fft_ffi_return_type = _DeeForeignFunction_GetFFIType(return_type)) == NULL) {
err_1:
  end = (iter = (DeeTypeObject const *const*)self->fft_arg_type_v)+arg_types_c;
  while (iter != end) Dee_DECREF(*iter++);
  free(self->fft_arg_type_v);
  goto err_0;
 }
 if (arg_types_c) {
  if ((self->fft_ffi_arg_type_v = _DeeForeignFunction_GetFFITypes(
   arg_types_c,(DeeTypeObject const *const *)self->fft_arg_type_v)) == NULL) {
   goto err_1;
  }
 } else self->fft_ffi_arg_type_v = NULL;

#if 1
 if ((new_name = _DeeForeignFunctionType_GenerateName(
  flags,return_type,arg_types_c,(DeeTypeObject const *const *)self->fft_arg_type_v)) == NULL) goto err_2;
#else
 if ((new_name = _DeeForeignFunctionType_GenerateName(
  flags,return_type,arg_types_c,(DeeTypeObject const *const *)arg_types_v)) == NULL) goto err_2;
#endif

 abi = _DeeForeignFunction_GetFFIAbiFromFlags(flags);
 error = ffi_prep_cif(&self->fft_ffi_cif,abi,(unsigned int)arg_types_c,
                      self->fft_ffi_return_type,self->fft_ffi_arg_type_v);
 if (error != FFI_OK) {
  DeeError_RuntimeErrorf("ffi_prep_cif(%q) failed: %d",new_name,(int)error);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
err_3:
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  free_nn(new_name);
err_2: free(self->fft_ffi_arg_type_v);
  goto err_1;
 }
 // All internal stuff is not initialized
 // >> Time to setup operators and all that stuff
 DeeType_GET_SLOT(self,tp_name) = new_name;
 //DeeType_GET_SLOT(self,tp_flags) |= DEE_TYPE_FLAG_NO_SUBCLASS;
 DeeType_GET_SLOT(self,tp_flags) &= ~DEE_TYPE_FLAG_STATIC_NAME;
 DeeType_GET_SLOT(self,tp_instance_size) = sizeof(DeeStructuredObject)+sizeof(DeeForeignFunction);
 DeeType_GET_SLOT(self,tp_ctor) = (int(*)(DeeTypeObject*,DeeObject*))&_deeforeignfunction_tp_ctor;
 DeeType_GET_SLOT(self,tp_p_instance_size) = sizeof(DeeForeignFunction);
 DeeType_GET_SLOT(self,tp_alloc) = (DeeObject*(*)(DeeTypeObject*))&_deeforeignfunction_tp_alloc;
 DeeType_GET_SLOT(self,tp_free) = (void(*)(DeeTypeObject*,DeeObject*))&_deeforeignfunction_tp_free;

 // Calculate the wbuffer cache sizes
 self->fft_wsize = max(self->fft_ffi_return_type->size,sizeof(ffi_arg));
 self->fft_woff_argmem = self->fft_wsize;
 if (is_varargs) {
  self->fft_woff_variadic_argmem = self->fft_wsize+(sizeof(union argument)*arg_types_c);
 } else {
  self->fft_wsize += (sizeof(union argument)*arg_types_c);
  self->fft_woff_argptr = self->fft_wsize;
  self->fft_wsize += (sizeof(void *)*arg_types_c);
 }
 if (!self->fft_wsize) self->fft_wsize = 1;
 if (!arg_types_c) self->fft_woff_argmem = 0,self->fft_woff_argptr = 0;

 // Calculate the function return kind
 if (return_type == (DeeTypeObject *)&DeeNone_Type) {
  return_kind = DEE_FOREIGN_RETURN_KIND_NONE;
 } else if (return_type == &DeeUtf8String_Type) {
  return_kind = DEE_FOREIGN_RETURN_KIND_UTF8STRING_FROM_POINTER;
 } else if (return_type == &DeeWideString_Type) {
  return_kind = DEE_FOREIGN_RETURN_KIND_WIDESTRING_FROM_POINTER;
 } else if (return_type == &DeeUtf16String_Type) {
  return_kind = DEE_FOREIGN_RETURN_KIND_UTF16STRING_FROM_POINTER;
 } else if (return_type == &DeeUtf32String_Type) {
  return_kind = DEE_FOREIGN_RETURN_KIND_UTF32STRING_FROM_POINTER;
 } else if (return_type == &DeeObject_Type) {
return_object:
  return_kind = ((flags&DEE_FUNCTIONFLAGS_FLAG_ADD_RESULT_REFERENCE)!=0)
   ? DEE_FOREIGN_RETURN_KIND_OBJECT_ADDREF
   : DEE_FOREIGN_RETURN_KIND_OBJECT;
 } else if (DeeStructuredType_Check(return_type)) {
  return_kind = DEE_FOREIGN_RETURN_KIND_STRUCTURED;
 } else goto return_object;
 DeeType_GET_SLOT(self,tp_p_call) = is_varargs
  ? (is_noexcept
  ? (DeeObject*(*)(struct DeeStructuredTypeObject*,void*,DeeObject*))&_deeforeignfunction_tp_p_call_nv
  : (DeeObject*(*)(struct DeeStructuredTypeObject*,void*,DeeObject*))&_deeforeignfunction_tp_p_call_v)
  : (is_noexcept
  ? (DeeObject*(*)(struct DeeStructuredTypeObject*,void*,DeeObject*))&_deeforeignfunction_tp_p_call_n
  : (DeeObject*(*)(struct DeeStructuredTypeObject*,void*,DeeObject*))&_deeforeignfunction_tp_p_call);
 DeeType_GET_SLOT(self,tp_call) = (
  DeeObject*(*)(DeeObject*,DeeObject*))&_deeforeignfunction_tp_call;

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 { // Enable the ability of calling pointer --> foreign_function
  DeeTypeObject *pointer_type;
  if ((pointer_type = DeeType_Pointer((DeeTypeObject *)self)) == NULL) goto err_3;
  DeeType_GET_SLOT(pointer_type,tp_p_call) = (DeeObject*(*)(struct DeeStructuredTypeObject*,void*,DeeObject*))&_deeforeignfunctionpointer_tp_p_call;
  DeeType_GET_SLOT(pointer_type,tp_call) = (DeeObject*(*)(DeeObject*,DeeObject*))&_deeforeignfunctionpointer_tp_call;
  if ((pointer_type = DeeType_LValue((DeeTypeObject *)self)) == NULL) goto err_3;
  DeeType_GET_SLOT(pointer_type,tp_call) = (DeeObject*(*)(DeeObject*,DeeObject*))&_deeforeignfunctionlvalue_tp_call;
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */

#ifdef return_kind
#undef return_kind
#endif
#undef is_noexcept
#undef is_varargs
 return 0;
}


#if 0
static int _deeforeignfunctiontype_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeForeignFunctionTypeObject *self) {
 return DeeForeignFunctionType_Init(self,0,(DeeTypeObject *)&DeeNone_Type,0,NULL);
}
static int _deeforeignfunctiontype_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeForeignFunctionTypeObject *self, DeeObject *args) {
 int flags; DeeTypeObject *return_type; DeeObject *arg_types,**iter,**end;
 if (DeeTuple_Unpack(args,"doo:foreign_function_type",&flags,&return_type,&arg_types) != 0) return -1;
 if (DeeObject_InplaceGetInstance(&return_type,&DeeType_Type) != 0) return -1;
 if (DeeObject_InplaceGetInstance(&arg_types,&DeeTuple_Type) != 0) return -1;
 end = (iter = DeeTuple_ELEM(arg_types))+DeeTuple_SIZE(arg_types);
 while (iter != end) { if (DeeError_TypeError_CheckType(*iter,&DeeType_Type) != 0) return -1; ++iter; }
 return DeeForeignFunctionType_Init(self,(Dee_funflags_t)flags,return_type,
  DeeTuple_SIZE(arg_types),(DeeTypeObject const* const*)DeeTuple_ELEM(arg_types));
}
#endif
static void _deeforeignfunctiontype_tp_dtor(
 DeeForeignFunctionTypeObject *self) {
 DeeTypeObject **iter,**end;
 Dee_DECREF(self->fft_return_type);
 end = (iter = self->fft_arg_type_v)+self->fft_arg_type_c;
 while (iter != end) Dee_DECREF(*iter++);
 free(self->fft_arg_type_v);
 free(self->fft_ffi_arg_type_v);
}

DEE_VISIT_PROC(_deeforeignfunctiontype_tp_visit,DeeForeignFunctionTypeObject *self) {
 DeeTypeObject **iter,**end;
 Dee_VISIT(self->fft_return_type);
 end = (iter = self->fft_arg_type_v)+self->fft_arg_type_c;
 while (iter != end) Dee_VISIT(*iter++);
}

static struct DeeMemberDef const _deeforeignfunctiontype_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("rett",DeeForeignFunctionTypeObject,fft_return_type,object),
 DEE_MEMBERDEF_RO_v100(DeeForeignFunctionTypeObject,fft_flags,Dee_funflags_t),
 DEE_MEMBERDEF_RO_v100(DeeForeignFunctionTypeObject,fft_return_type,object),
 DEE_MEMBERDEF_RO_v100(DeeForeignFunctionTypeObject,fft_arg_type_c,Dee_size_t),
 DEE_MEMBERDEF_RO_v100(DeeForeignFunctionTypeObject,fft_arg_type_v,p2(void)),
 DEE_MEMBERDEF_RO_v100(DeeForeignFunctionTypeObject,fft_ffi_return_type,p(void)),
 DEE_MEMBERDEF_RO_v100(DeeForeignFunctionTypeObject,fft_ffi_arg_type_v,p2(void)),
 DEE_MEMBERDEF_RO_v100(DeeForeignFunctionTypeObject,fft_wsize,Dee_size_t),
 DEE_MEMBERDEF_RO_v100(DeeForeignFunctionTypeObject,fft_woff_argmem,Dee_size_t),
 DEE_MEMBERDEF_RO_v100(DeeForeignFunctionTypeObject,fft_woff_argptr,Dee_size_t),
 DEE_MEMBERDEF_RO_v100(DeeForeignFunctionTypeObject,fft_return_kind,enum),
 DEE_MEMBERDEF_END_v100
};

static DeeObject *_deeforeignfunctiontype_argv_get(
 DeeForeignFunctionTypeObject *self, void *DEE_UNUSED(closure)) {
 return DeeTuple_NewFromVector(self->fft_arg_type_c,(DeeObject *const *)self->fft_arg_type_v);
}

static struct DeeGetSetDef const _deeforeignfunctiontype_tp_getsets[] = {
 DEE_GETSETDEF_v100("argv",member(&_deeforeignfunctiontype_argv_get),null,null,"-> iterable"),
 DEE_GETSETDEF_END_v100
};

static struct DeeTypeMarshal _deeforeignfunctiontype_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALID_FOREIGNFUNCTIONTYPE),member(NULL),member(NULL)); // Special marshaling

DeeTypeObject DeeForeignFunctionType_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("foreign_function_type"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_CONST_CTOR|
         DEE_TYPE_FLAG_DONT_COPY|DEE_TYPE_FLAG_FUNDAMENTAL),
  member(&DeeStructuredType_Type)),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeForeignFunctionTypeObject),null,
  null/*member(&_deeforeignfunctiontype_tp_ctor)*/,null,null,
  null/*member(&_deeforeignfunctiontype_tp_any_ctor)*/),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deeforeignfunctiontype_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deeforeignfunctiontype_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deeforeignfunctiontype_tp_members),
  member(_deeforeignfunctiontype_tp_getsets),null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deeforeignfunctiontype_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};

DEE_DECL_END

#ifndef __INTELLISENSE__
#include "foreign_function.closure.inl"
#endif
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */

#endif /* !GUARD_DEEMON_FOREIGN_FUNCTION_C */
