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
#ifndef GUARD_DEEMON___ANNOTATIONS_INL
#define GUARD_DEEMON___ANNOTATIONS_INL 1

#ifndef GUARD_DEEMON___CONF_INL
#error "Don't #include this file directly (#include <deemon/__conf.inl>)"
#endif

#ifdef __INTELLISENSE__
#include <sal.h>
#endif

#ifndef DEE_NO_ANNOTATION
#if defined(_PREFAST_)
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#include <sal.h>
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#define DEE_HAS_ANNOTATIONS 1
#endif
#endif /* !DEE_NO_ANNOTATION */

#ifdef _On_failure_
# define DEE_A_ON_FAILURE _On_failure_
#else
# define DEE_A_ON_FAILURE(x)
#endif
#ifdef _Always_
# define DEE_A_ON_ALWAYS _Always_
#else
# define DEE_A_ON_ALWAYS(x)
#endif
#ifdef _In_
# define DEE_A_IN _In_
#else
# define DEE_A_IN
#endif
#ifdef _In_opt_
# define DEE_A_IN_OPT _In_opt_
#else
# define DEE_A_IN_OPT
#endif
#ifdef _In_z_
# define DEE_A_IN_Z _In_z_
#else
# define DEE_A_IN_Z
#endif
#ifdef _In_opt_z_
# define DEE_A_IN_Z_OPT _In_opt_z_
#else
# define DEE_A_IN_Z_OPT
#endif

#ifdef _Out_
# define DEE_A_OUT _Out_
#else
# define DEE_A_OUT
#endif
#ifdef _Out_opt_
# define DEE_A_OUT_OPT _Out_opt_
#else
# define DEE_A_OUT_OPT
#endif

#ifdef _Inout_
# define DEE_A_INOUT _Inout_
#else
# define DEE_A_INOUT
#endif
#ifdef _Inout_opt_
# define DEE_A_INOUT_OPT _Inout_opt_
#else
# define DEE_A_INOUT_OPT
#endif



#ifdef _In_reads_
# define DEE_A_IN_R _In_reads_
#else
# define DEE_A_IN_R(s)
#endif
#ifdef _In_reads_opt_
# define DEE_A_IN_R_OPT _In_reads_opt_
#else
# define DEE_A_IN_R_OPT(s)
#endif
#ifdef _In_reads_bytes_
# define DEE_A_IN_RB _In_reads_bytes_
#else
# define DEE_A_IN_RB(s)
#endif
#ifdef _In_reads_bytes_opt_
# define DEE_A_IN_RB_OPT _In_reads_bytes_opt_
#else
# define DEE_A_IN_RB_OPT(s)
#endif

#ifdef _Out_writes_
# define DEE_A_OUT_W _Out_writes_
#else
# define DEE_A_OUT_W(s)
#endif
#ifdef _Out_writes_opt_
# define DEE_A_OUT_W_OPT _Out_writes_opt_
#else
# define DEE_A_OUT_W_OPT(s)
#endif
#ifdef _Out_writes_bytes_
# define DEE_A_OUT_WB _Out_writes_bytes_
#else
# define DEE_A_OUT_WB(s)
#endif
#ifdef _Out_writes_bytes_opt_
# define DEE_A_OUT_WB_OPT _Out_writes_bytes_opt_
#else
# define DEE_A_OUT_WB_OPT(s)
#endif

#ifdef _Post_invalid_
# define DEE_A_OUT_INVALID _Post_invalid_
#else
# define DEE_A_OUT_INVALID
#endif


#ifdef _Ret_z_
# define DEE_A_RET_Z _Ret_z_
#else
# define DEE_A_RET_Z
#endif
#ifdef _Ret_opt_z_
# define DEE_A_RET_Z_OPT _Ret_opt_z_
#else
# define DEE_A_RET_Z_OPT
#endif
#ifdef _Ret_null_
# define DEE_A_RET_EQUAL_NULL _Ret_null_
#else
# define DEE_A_RET_EQUAL_NULL
#endif
#ifdef _Ret_maybenull_
# define DEE_A_RET_MAYBE_NULL _Ret_maybenull_
#else
# define DEE_A_RET_MAYBE_NULL
#endif
#ifdef _Ret_notnull_
# define DEE_A_RET_NEVER_NULL _Ret_notnull_
#else
# define DEE_A_RET_NEVER_NULL
#endif

#ifdef _Check_return_
# define DEE_A_RET_WUNUSED _Check_return_
#elif __has_attribute(warn_unused_result) || defined(__GNUC__)
# define DEE_A_RET_WUNUSED __attribute__((__warn_unused_result__))
#else
# define DEE_A_RET_WUNUSED
#endif


#ifdef _Success_
# define DEE_A_SUCCESS        _Success_
#else
# define DEE_A_SUCCESS(x)
#endif

#define DEE_A_RET_SUCCESS(x) DEE_A_SUCCESS(return == x)


#define DEE_A_IN_PRINTF       DEE_A_IN_Z
#define DEE_A_IN_BUILDVALUEF  DEE_A_IN_Z
#define DEE_A_IN_BUILDTUPLEF  DEE_A_IN_Z
#define DEE_A_IN_UNPACKTUPLEF DEE_A_IN_Z


//////////////////////////////////////////////////////////////////////////
// The function raised an exception if "x" is true
#define DEE_A_EXCEPTION(x)		     DEE_A_SUCCESS(!(x))

//////////////////////////////////////////////////////////////////////////
// Indicates a transfer of a reference
// - Usually used with function return values
// >> // v if you call this function, you must either
// >> //   Dee_DECREF() its value or forward it into another DEE_A_REF value
// >> DEE_A_REF DeeObject *DeeObject_Copy(DEE_A_INOUT DeeObject *self);
#define DEE_A_REF  /* Object tag for new reference */

//////////////////////////////////////////////////////////////////////////
// Indicates a transfer of a weak reference
// - Usage is the same as 'DEE_A_REF', but Dee_WEAKDECREF() must be called instead
//   and DeeObject_LockWeakref() must be called before the object can be used safely
#define DEE_A_WEAK_REF /* Object tag for new weak reference */

//////////////////////////////////////////////////////////////////////////
// Input object, that must be an instance of "T"
// >> Unless otherwise states, the function will assert that this argument
//    is really an instance of 'T', but will only do so in debug mode.
#define DEE_A_IN_OBJECT(T)        DEE_A_IN DeeObject
#define DEE_A_INOUT_OBJECT(T)     DEE_A_INOUT DeeObject
#define DEE_A_OUT_OBJECT(T)       DEE_A_OUT DeeObject
#define DEE_A_IN_OBJECT_OPT(T)    DEE_A_IN_OPT DeeObject
#define DEE_A_INOUT_OBJECT_OPT(T) DEE_A_INOUT_OPT DeeObject
#define DEE_A_OUT_OBJECT_OPT(T)   DEE_A_OUT_OPT DeeObject
#define DEE_A_RET_OBJECT(T)       DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeObject
#define DEE_A_RET_OBJECT_MAYBE(T) DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL DeeObject
#define DEE_A_RET_OBJECT_REF(T)   DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeObject

#define DEE_A_IN_TYPEOBJECT(T)        DEE_A_IN DeeTypeObject
#define DEE_A_INOUT_TYPEOBJECT(T)     DEE_A_INOUT DeeTypeObject
#define DEE_A_OUT_TYPEOBJECT(T)       DEE_A_OUT DeeTypeObject
#define DEE_A_IN_TYPEOBJECT_OPT(T)    DEE_A_IN_OPT DeeTypeObject
#define DEE_A_INOUT_TYPEOBJECT_OPT(T) DEE_A_INOUT_OPT DeeTypeObject
#define DEE_A_OUT_TYPEOBJECT_OPT(T)   DEE_A_OUT_OPT DeeTypeObject
#define DEE_A_RET_TYPEOBJECT(T)       DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeTypeObject
#define DEE_A_RET_TYPEOBJECT_MAYBE(T) DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL DeeTypeObject
#define DEE_A_RET_TYPEOBJECT_REF(T)   DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeTypeObject


#define DEE_A_RET_HELP_STRING     DEE_A_RET_Z_OPT DEE_A_RET_WUNUSED

//////////////////////////////////////////////////////////////////////////
// Used to annotate that a function behaves the
// same as executing 'code' as deemon code
#define DEE_A_DEEMON(code)

//////////////////////////////////////////////////////////////////////////
// Function returns a new reference or throws an exception and return NULL
#define DEE_A_RET_REF \
 DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL

#define DEE_A_RET_EXCEPT_REF \
 DEE_A_RET_WUNUSED \
 DEE_A_RET_MAYBE_NULL \
 DEE_A_EXCEPTION(return == NULL) \
 DEE_A_REF
#define DEE_A_RET_OBJECT_EXCEPT_REF(T)     DEE_A_RET_EXCEPT_REF DeeObject
#define DEE_A_RET_OBJECT_EXCEPT(T)         DEE_A_RET_EXCEPT(NULL) DeeObject
#define DEE_A_RET_TYPEOBJECT_EXCEPT_REF(T) DEE_A_RET_EXCEPT_REF DeeTypeObject
#define DEE_A_RET_TYPEOBJECT_EXCEPT(T)     DEE_A_RET_EXCEPT(NULL) DeeTypeObject

//////////////////////////////////////////////////////////////////////////
// Function doesn't raise any exceptions, but if "x" is returned, the call has failed
#define DEE_A_RET_NOEXCEPT(x) \
 DEE_A_RET_WUNUSED \
 DEE_A_SUCCESS(return != x)

//////////////////////////////////////////////////////////////////////////
// Function doesn't raise any exceptions, but if "NULL" is returned, the call has failed
// >> Returns a new refernece otherwise
#define DEE_A_RET_NOEXCEPT_REF \
 DEE_A_RET_WUNUSED \
 DEE_A_RET_MAYBE_NULL \
 DEE_A_REF
#define DEE_A_RET_OBJECT_NOEXCEPT(T) \
 DEE_A_RET_NOEXCEPT(return != NULL) DeeObject
#define DEE_A_RET_OBJECT_NOEXCEPT_REF(T) \
 DEE_A_RET_NOEXCEPT_REF DeeObject
#define DEE_A_RET_TYPEOBJECT_NOEXCEPT(T) \
 DEE_A_RET_NOEXCEPT(return != NULL) DeeTypeObject
#define DEE_A_RET_TYPEOBJECT_NOEXCEPT_REF(T) \
 DEE_A_RET_NOEXCEPT_REF DeeTypeObject

#define DEE_A_RET_EXCEPT(except_val)               DEE_A_RET_WUNUSED DEE_A_EXCEPTION(return == except_val)
#define DEE_A_RET_EXCEPT_FAIL(except_val,fail_val) DEE_A_RET_WUNUSED DEE_A_SUCCESS(return != except_val && return != fail_val)

#define DEE_A_INTERRUPT \
 /* tag: this function checks the calling thread's interrupt flag. */



#define DEE_A_IN_LOCKED    DEE_A_INOUT /* Mutex that is locked on enter. */
#define DEE_A_OUT_LOCKED   DEE_A_INOUT /* Mutex that is locked on exit. */
#define DEE_A_INOUT_LOCKED DEE_A_INOUT /* Mutex that is locked on enter and exit. */


#define DEE_A_REQ_INIT \
 /* Function requires the deemon library to be initialized with 'Dee_Initialize' / 'Dee_InitializeEx' */

#define DEE_A_EXEC DEE_A_REQ_INIT \
 /* Function can execute arbitrary code not predictable at compile time. */\
 /* NOTE: Must only be present in headers. */

#define DEE_A_EXEC_IFB(x) /* if (DEE_A_EXEC_OF(x)) DEE_A_EXEC; */
#define DEE_A_EXEC_OF(x) 0 /* true/false if 'x' has the DEE_A_EXEC tag. */
#define DEE_A_EXEC_IF(x)  DEE_A_EXEC_IFB(DEE_A_EXEC_OF(x))



#endif /* !GUARD_DEEMON___ANNOTATIONS_INL */
