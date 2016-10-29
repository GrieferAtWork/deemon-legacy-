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
#ifndef GUARD_DEEMON_FUNCTION_H
#define GUARD_DEEMON_FUNCTION_H 1

#include <deemon/__conf.inl>
#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#include <deemon/runtime/execute.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif

DEE_OBJECT_DEF(DeeFunctionObject);
DEE_OBJECT_DEF(DeeYieldFunctionObject);
DEE_OBJECT_DEF(DeeYieldFunctionIteratorObject);

//////////////////////////////////////////////////////////////////////////
// Function object
#define DEE_FUNCTION_FLAG_NONE  DEE_UINT32_C(0x00000000)
#define DEE_FUNCTION_FLAG_VARG  DEE_UINT32_C(0x00000001)  /*< var-arg function (last argument is optional and consumes remainder). */
#define DEE_FUNCTION_FLAG_YILD  DEE_UINT32_C(0x00000002)  /*< yield-style function (calling this creates a yield_function.iterator object). */
#define DEE_FUNCTION_FLAG_THIS  DEE_UINT32_C(0x00000004)  /*< Function supports the 'this' argument. */

// Mask of all currently supported function flags (used by the marshal)
#define DEE_FUNCTION_FLAG_MASK  (\
 DEE_FUNCTION_FLAG_VARG|DEE_FUNCTION_FLAG_YILD|DEE_FUNCTION_FLAG_THIS)

struct DeeCodeObject;
struct DeeTupleObject;
#ifdef DEE_LIMITED_API
struct DeeFunctionObject {
 DEE_OBJECT_HEAD
 DEE_A_REF struct DeeCodeObject  *fn_code;  /*< [1..1] Code executed by this function. */
 DEE_A_REF struct DeeTupleObject *fn_refs;  /*< [1..1] References to vars in other scopes. */
 struct DeeAtomicMutex            fn_lock;  /*< Lock for this function. */
 // The following two are not protected by the lock
 Dee_uint32_t                     fn_flags; /*< Function flags. */
 Dee_size_t                       fn_argc;  /*< Argument count required to properly execute the code. */
 // TODO: add support for 'DeeFunctionFlags_NOEXCEPT' / '__attribute__((__nothrow__))'.
 //       Should work by simply dumping unhandled exceptions at the end of a call.
 // TODO: defaults
};
#define DeeFunction_LOCK(ob)    (&((DeeFunctionObject *)Dee_REQUIRES_POINTER(ob))->fn_lock)
#define DeeFunction_ACQUIRE(ob) DeeAtomicMutex_Acquire(DeeFunction_LOCK(ob))
#define DeeFunction_RELEASE(ob) DeeAtomicMutex_Release(DeeFunction_LOCK(ob))
#define DeeFunction_CODE(ob)    ((DeeObject *)((DeeFunctionObject *)Dee_REQUIRES_POINTER(ob))->fn_code)
#define DeeFunction_REFS(ob)    ((DeeObject *)((DeeFunctionObject *)Dee_REQUIRES_POINTER(ob))->fn_refs)
#define DeeFunction_ARGC(ob)                  ((DeeFunctionObject *)Dee_REQUIRES_POINTER(ob))->fn_argc
#define DeeFunction_FLAGS(ob)                 ((DeeFunctionObject *)Dee_REQUIRES_POINTER(ob))->fn_flags
#else
#define DeeFunction_ARGC(ob)    DeeFunction_Argc((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeFunction_FLAGS(ob)   DeeFunction_Flags((DeeObject *)Dee_REQUIRES_POINTER(ob))
#endif
#define DeeFunction_IS_VARG(ob) ((DeeFunction_FLAGS(ob)&DEE_FUNCTION_FLAG_VARG)!=0)
#define DeeFunction_IS_YILD(ob) ((DeeFunction_FLAGS(ob)&DEE_FUNCTION_FLAG_YILD)!=0)

#define DeeFunction_NAME(ob)    DeeCode_NAME(DeeFunction_CODE(ob))
DEE_FUNC_DECL(DEE_A_RET_OBJECT_REF(DeeCodeObject) *) DeeFunction_Code(
 DEE_A_IN_OBJECT(DeeFunctionObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_REF(DeeTupleObject) *) DeeFunction_Refs(
 DEE_A_IN_OBJECT(DeeFunctionObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeFunction_Argc(
 DEE_A_IN_OBJECT(DeeFunctionObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint32_t) DeeFunction_Flags(
 DEE_A_IN_OBJECT(DeeFunctionObject) const *self) DEE_ATTRIBUTE_NONNULL((1));


#ifdef DEE_LIMITED_API
struct DeeYieldFunctionObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeFunctionObject     *yf_func; /*< [1..1] The function we are derived from. */
 DEE_A_REF struct DeeTupleObject *yf_args; /*< [1..1] Arguments that we are called with. */
 DEE_A_REF DeeObject             *yf_this; /*< [0..1] 'this' object during callback. */
};
struct DeeYieldFunctionIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeFunctionObject     *yi_func;  /*< [1..1] The function we are derived from. */
 DEE_A_REF struct DeeTupleObject *yi_args;  /*< [1..1] Arguments that we are called with. */
 // NOTE: 'yi_frame.f_code' is a reference.
 //        >> That is required to still know the rules concerning local count
 //           and more, in case the associated function is cleared before knowledge
 //           about those things would becode relevant.
 struct DeeStackFrame             yi_frame; /*< The stackframe we are using for execution. */
#ifndef DEE_WITHOUT_THREADS
 struct DeeAtomicMutex            yi_lock;  /*< Lock for the iterator. */
#endif
};
#endif


#define DeeFunction_CheckExact(ob)              DeeObject_InstanceOfExact(ob,&DeeFunction_Type)
#define DeeFunction_Check                       DeeFunction_CheckExact
#define DeeYieldFunction_CheckExact(ob)         DeeObject_InstanceOfExact(ob,&DeeYieldFunction_Type)
#define DeeYieldFunction_Check                  DeeYieldFunction_CheckExact
#define DeeYieldFunctionIterator_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeYieldFunctionIterator_Type)
#define DeeYieldFunctionIterator_Check          DeeYieldFunctionIterator_CheckExact
DEE_DATA_DECL(DeeTypeObject) DeeFunction_Type;
DEE_DATA_DECL(DeeTypeObject) DeeYieldFunction_Type;
DEE_DATA_DECL(DeeTypeObject) DeeYieldFunctionIterator_Type;

DEE_FUNC_DECL(DEE_A_RET_EXCEPT_REF DeeObject *) DeeFunction_New(
 DEE_A_INOUT_OBJECT(DeeCodeObject) *code, DEE_A_INOUT_OBJECT(DeeTupleObject) *refs,
 DEE_A_IN Dee_size_t argc, DEE_A_IN Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1,2));
#ifdef DEE_LIMITED_API
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeFunction_Call(
 DEE_A_IN_OBJECT(DeeFunctionObject) const *self,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2));
#else
#define DeeFunction_Call \
 (*(DeeObject*(DEE_CALL *)(DeeObject const*,DeeObject*))\
  DeeType_GET_SLOT(&DeeFunction_Type,tp_call))
#endif
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_REF DeeObject *) DeeFunction_ThisCall(
 DEE_A_IN_OBJECT(DeeFunctionObject) const *self, DEE_A_INOUT DeeObject *this_arg,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) DEE_ATTRIBUTE_NONNULL((1,2,3));

DEE_DECL_END

#endif /* !GUARD_DEEMON_FUNCTION_H */
