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
#ifndef GUARD_DEEMON_FOREIGN_FUNCTION_H
#define GUARD_DEEMON_FOREIGN_FUNCTION_H 1

#include <deemon/__conf.inl>

#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#include <deemon/structured.h>
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#if defined(_MSC_VER) && defined(DEE_PLATFORM_WINDOWS)
DEE_COMPILER_MSVC_WARNING_PUSH(4255 4820)
#include <deemon/runtime/libffi_msvc/ffi.h>
DEE_COMPILER_MSVC_WARNING_POP
#else
// These file were previously build with "make_libffi.sh"
#include <ffi.h>
#include <ffitarget.h>
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_FUNCTION_FLAGS
DEE_PRIVATE_DECL_DEE_FUNCTION_FLAGS
#undef DEE_PRIVATE_DECL_DEE_FUNCTION_FLAGS
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif

DEE_OBJECT_DEF(DeeForeignFunctionObject);
DEE_OBJECT_DEF(DeeForeignFunctionTypeObject);
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
DEE_OBJECT_DEF(DeeForeignFunctionClosureObject);
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */

#ifdef DEE_LIMITED_API
enum DeeForeignFunctionReturnTypeKind {
 DEE_FOREIGN_RETURN_KIND_NONE                     = 0,
 DEE_FOREIGN_RETURN_KIND_STRUCTURED               = 1,
 DEE_FOREIGN_RETURN_KIND_OBJECT                   = 2,
 DEE_FOREIGN_RETURN_KIND_OBJECT_ADDREF            = 3,
 DEE_FOREIGN_RETURN_KIND_WIDESTRING_FROM_POINTER  = 4,
 DEE_FOREIGN_RETURN_KIND_UTF8STRING_FROM_POINTER  = 5,
 DEE_FOREIGN_RETURN_KIND_UTF16STRING_FROM_POINTER = 6,
 DEE_FOREIGN_RETURN_KIND_UTF32STRING_FROM_POINTER = 7,
};
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeForeignFunctionTypeObject {
 DeeStructuredTypeObject   fft_type;
 DeeFunctionFlags          fft_flags;           /*< Function flags. */
 DEE_A_REF DeeTypeObject  *fft_return_type;     /*< [1..1] Return type. */
 Dee_size_t                fft_arg_type_c;      /*< Argument count. */
 DEE_A_REF DeeTypeObject **fft_arg_type_v;      /*< [1..1][0..ob_argc][owned] Argument types. */
 ffi_type                 *fft_ffi_return_type; /*< [1..1] Raw return type. */
 ffi_type                **fft_ffi_arg_type_v;  /*< [1..1][0..ob_argc][owned] Raw argument types. */
 ffi_cif                   fft_ffi_cif;         /*< cif object to call the function. */
 // WBuffer layout:
 //  1. return value memory
 //  2. argument memory...
 //  3. argument pointers...
 Dee_size_t                fft_wsize;
 Dee_size_t                fft_woff_argmem;
union{
 Dee_size_t                fft_woff_argptr;
 Dee_size_t                fft_woff_variadic_argmem;
}
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define fft_woff_argptr          _fft_woff_argptr_data.fft_woff_argptr
#define fft_woff_variadic_argmem _fft_woff_argptr_data.fft_woff_variadic_argmem
 _fft_woff_argptr_data
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
;
 enum DeeForeignFunctionReturnTypeKind fft_return_kind;
};
#define DeeForeignFunctionType_FLAGS(ob)      ((DeeForeignFunctionTypeObject *)(ob))->fft_flags
#define DeeForeignFunctionType_RETURNTYPE(ob) ((DeeForeignFunctionTypeObject *)(ob))->fft_return_type
#define DeeForeignFunctionType_ARGTYC(ob)     ((DeeForeignFunctionTypeObject *)(ob))->fft_arg_type_c
#define DeeForeignFunctionType_ARGTYV(ob)     ((DeeForeignFunctionTypeObject *)(ob))->fft_arg_type_v
DEE_COMPILER_MSVC_WARNING_POP
#endif

DEE_COMPILER_MSVC_WARNING_PUSH(4255)
#ifdef __cplusplus
typedef int (*DeeForeignFunction)(...);
#else
typedef int (*DeeForeignFunction)();
#endif
DEE_COMPILER_MSVC_WARNING_POP

#ifdef DEE_LIMITED_API
DEE_COMPILER_STRUCTURE_PACKED_BEGIN
struct DeeForeignFunctionObject {
 DEE_OBJECT_HEAD_EX(DeeForeignFunctionTypeObject)
 // NOTE: This object cannot be instantiated
 // instructions ~would~ go here...
};
#define DeeForeignFunction_FUNC(ob)        ((DeeForeignFunction)DeeStructured_DATA(ob))


#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
// v Reuse the 'ffi_cif' from the original foreign function type
//   -> The documentation for libffi doesn't say if reusing a cif
//      for both foreign functions, as well as closure callbacks
//      is allowed. (It probably does somewhere, but I couldn't find it...)
//   -> Reusing it has been working thus far (windows|linux), but if for
//      some reason it stops working, just flip this #define and you're good to go again!
#ifndef DEE_FOREIGN_FUNCTION_CLOSURE_REUSE_CIF
#define DEE_FOREIGN_FUNCTION_CLOSURE_REUSE_CIF 1
#endif

DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeForeignFunctionClosureObject {
 DEE_OBJECT_HEAD
union{
 DeeForeignFunction                      ffc_funptr;   /*< [1..1][owned] Callback function pointer. */
 void                                   *ffc_exec;     /*< [1..1] The executable callback function pointer. */
}
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define ffc_funptr _ffc_funptr_data.ffc_funptr
#define ffc_exec   _ffc_funptr_data.ffc_exec
 _ffc_funptr_data
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
;
 DEE_A_REF DeeObject                    *ffc_callback; /*< [1..1] Object that should be called from 'ffc_funptr' (using 'tp_call'). */
 DEE_A_REF DeeForeignFunctionTypeObject *ffc_type;     /*< [1..1] The Regular foreign-function type. */
 ffi_closure                            *ffc_write;    /*< [1..1] The writable callback function pointer. */
#if !DEE_FOREIGN_FUNCTION_CLOSURE_REUSE_CIF
 ffi_cif                                 ffc_ffi_cif;
#endif
};
DEE_COMPILER_MSVC_WARNING_POP
#define DeeForeignFunctionClosure_TYPE(ob) ((DeeTypeObject *)((DeeForeignFunctionClosureObject *)Dee_REQUIRES_POINTER(ob))->ffc_type)
#define DeeForeignFunctionClosure_FUNC(ob) ((DeeForeignFunction)((DeeForeignFunctionClosureObject *)Dee_REQUIRES_POINTER(ob))->ffc_funptr)
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */
DEE_COMPILER_STRUCTURE_PACKED_END

#endif

DEE_DATA_DECL(DeeTypeObject) DeeForeignFunctionType_Type;
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
DEE_DATA_DECL(DeeTypeObject) DeeForeignFunctionClosure_Type;
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */
#define DeeForeignFunctionType_Check(ob)         DeeObject_InstanceOf(ob,&DeeForeignFunctionType_Type)
#define DeeForeignFunctionType_CheckExact(ob)    DeeObject_InstanceOfExact(ob,&DeeForeignFunctionType_Type)
#define DeeForeignFunction_Check(ob)             DeeForeignFunctionType_Check(Dee_TYPE(ob))
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
#define DeeForeignFunctionClosure_Check(ob)      DeeObject_InstanceOf(ob,&DeeForeignFunctionClosure_Type)
#define DeeForeignFunctionClosure_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeForeignFunctionClosure_Type)
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */

#ifdef DEE_LIMITED_API
// NOTE: Setting the 'DeeFunctionFlags_NOEXCEPT' flag causes os-signals to not be handled by deemon
extern DEE_A_EXEC DEE_A_RET_TYPEOBJECT_EXCEPT_REF(DeeForeignFunctionTypeObject) *_DeeForeignFunctionType_New(
 DEE_A_IN DeeFunctionFlags flags, DEE_A_IN DeeTypeObject const *return_type,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeTypeObject const *const *argv) DEE_ATTRIBUTE_NONNULL((2));
extern DEE_A_RET_EXCEPT(NULL) DeeTypeObject *_DeeForeignFunctionType_ArgTypeof(DEE_A_IN DeeTypeObject *tp) DEE_ATTRIBUTE_NONNULL((1));
#endif

#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
//////////////////////////////////////////////////////////////////////////
// Creates a new foreign-function closure object
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeForeignFunctionClosureObject) *) DeeForeignFunctionClosure_New(
 DEE_A_IN_TYPEOBJECT(DeeForeignFunctionTypeObject) const *ff_type,
 DEE_A_INOUT DeeObject *callback) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Returns the (callback) function address, which when executed is going
// to call 'callback' as defined in a previous call to 'DeeForeignFunctionClosure_New'
// NOTE: The actual function type returned by this is specified at runtime through 'ff_type'
DEE_FUNC_DECL(DEE_A_RET_NEVER_NULL DeeForeignFunction) DeeForeignFunctionClosure_Func(
 DEE_A_IN_OBJECT(DeeForeignFunctionClosureObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */

DEE_DECL_END
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */

#endif /* !GUARD_DEEMON_FOREIGN_FUNCTION_H */
