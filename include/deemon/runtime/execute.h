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
#ifndef GUARD_DEEMON_EXECUTE_H
#define GUARD_DEEMON_EXECUTE_H 1

#include <deemon/__conf.inl>
#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#ifndef GUARD_DEEMON_ERROR_H
#include <deemon/error.h>
#endif
#ifdef DEE_LIMITED_API
#include <deemon/optional/string_forward.h>
#include <deemon/optional/raised_exception.h>
#endif
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif

struct DeeCodeObject;
struct DeeStackFrame;

#ifdef DEE_LIMITED_DEX
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeStackFrame {
 /*readonly*/struct DeeStackFrame   *f_prev;       /*< [0..1] Prev stack frame. */
 /*readonly*/struct DeeCodeObject   *f_code;       /*< [1..1] Code being executed in this frame. */
 /*readonly*/DeeObject             **f_argv;       /*< [1..1][0..f_argc] Vector of arguments (NOTE: Elements are read/write). */
#ifdef DEE_DEBUG
 /*readonly*/Dee_size_t              f_argc;       /*< Actual amount of argument (Only available in debug builds; used in assertions). */
#endif
#ifndef DEE_WITHOUT_THREADS
             struct DeeAtomicMutex   f_lock;       /*< Lock for everything below. */
#endif
             Dee_uint8_t             f_padding;    /*< Padding data. */
#define DEE_STACKFRAME_FLAG_NONE    DEE_UINT16_C(0x0000)
#define DEE_STACKFRAME_FLAG_SANDBOX DEE_UINT16_C(0x0001) /*< Sandbox-ed stackframe. */
             Dee_uint16_t            f_flags;      /*< Frame flags. */
             Dee_uint8_t const      *f_addr;       /*< [?..?] Current code address (inside of 'DeeCode_CODE(f_code)...+DeeCode_SIZE(f_code)'). */
 /*readonly*/DeeObject             **f_constv;     /*< [1..1][0..DeeTuple_SIZE(f_code->ob_consts)] List of constants (== DeeTuple_ELEM(f_code->ob_consts)). */
 /*readonly*/DeeObject             **f_refv;       /*< [1..1][0..f_refc] List of references. */
#ifdef DEE_DEBUG
 /*readonly*/Dee_size_t              f_refc;       /*< Actual amount of references (Only available in debug builds; used in assertions). */
#endif
   DEE_A_REF DeeObject             **f_localv;     /*< [0..1][0..f_code->ob_nlocals] Vector of local variables. */
union{
   DEE_A_REF DeeObject             **f_stackv;     /*< [1..1][this..f_stackv_end] Vector of local variables. */
   char                             *f_wbuf;       /*< [1..f_code->ob_wsize] Execution buffer. */
}
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
   _f_stackv
#define f_stackv _f_stackv.f_stackv
#define f_wbuf   _f_stackv.f_wbuf
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
;
#define _DeeStackFrame_StackSize(ob) (Dee_size_t)((ob)->f_stackv_end-(ob)->f_stackv)
             DeeObject             **f_stackv_end; /*< [?..?][0..1] End of the stack (first invalid stack slot) (unrelyable; only update before 'yield'). */
   DEE_A_REF DeeObject              *f_this;       /*< [0..1][const] Instance this object (Can't be changed once frame has started). */
   DEE_A_REF DeeObject              *f_result;     /*< [0..1] Function return value. */
 struct DeeRaisedException          *f_stored_exceptions; /*< [0..1][owned] More exceptions that have been stored. */
#if DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS
 struct DeeExceptionHandlerRef      *f_exceptv;     /*< [1..1][this..f_exceptv_end] List of exception handlers. */
 struct DeeExceptionHandlerRef      *f_exceptv_end; /*< [?..?][0..1] End of the exception handler list. */
 Dee_uint8_t const **f_finally_return_address_end;
#define _DeeStackFrame_FINALLY_RETURN_ADDRESS_BASE(ob)\
 ((Dee_uint8_t const **)((ob).f_wbuf+(ob).f_code->co_woff_finally))
#define _DeeStackFrame_FINALLY_RETURN_ADDRESS_MAX_SIZE(ob)\
 ((ob).f_code->co_finallyc)
#define _DeeStackFrame_FINALLY_RETURN_ADDRESS_SIZE(ob)\
 ((Dee_size_t)((ob).f_finally_return_address_end-_DeeStackFrame_FINALLY_RETURN_ADDRESS_BASE(ob)))
#endif
};
DEE_COMPILER_MSVC_WARNING_POP

#define _DeeStackFrame_Addr(ob) DeeCode_PTR2ADDR((ob)->f_code,(ob)->f_addr)
#define _DeeStackFrame_File(ob) DeeCode_ADDR2FILE((ob)->f_code,_DeeStackFrame_Addr(ob))
#define _DeeStackFrame_Line(ob) DeeCode_ADDR2LINE((ob)->f_code,_DeeStackFrame_Addr(ob))
#define _DeeStackFrame_Func(ob) DeeCode_NAME((ob)->f_code)

#ifdef DEE_LIMITED_API
// Copies a stackframe and all its associated data
// NOTES:
//   - Throws an Error.ValueError if 'right' cannot be copied
//     NOTE: if '_DeeStackFrame_WasExecuted(right)' is false, copying is always allowed
//   - Locals and stack slots will be allocated on the heap (using 'malloc' w/ debug_new)
//   - 'self->f_prev' is initialized to NULL
//   - The following are simply initialized from right and the caller is responsible
//     for keeping them alive after this call returns: 'f_refv', 'f_argv'
#define DEE_STACKFRAME_COPY_FLAG_NONE            0x00000000
#define DEE_STACKFRAME_COPY_FLAG_LOCALS_COPY     0x00000001 // Copy locals using DeeObject_Copy
#define DEE_STACKFRAME_COPY_FLAG_LOCALS_DEEPCOPY 0x00000002 // Copy locals using DeeObject_DeepCopy
#define DEE_STACKFRAME_COPY_FLAG_STACK_COPY      0x00000004 // Copy the stack using DeeObject_Copy
#define DEE_STACKFRAME_COPY_FLAG_STACK_DEEPCOPY  0x00000008 // Copy the stack using DeeObject_DeepCopy
extern DEE_A_RET_EXCEPT(-1) int _DeeStackFrame_InitCopy(
 DEE_A_OUT struct DeeStackFrame *self,
 DEE_A_IN struct DeeStackFrame const *right,
 DEE_A_IN Dee_uint32_t copy_flags) DEE_ATTRIBUTE_NONNULL((1,2));
// De-initializes a stackframe as initialized by '_DeeStackFrame_InitCopy' / '_DeeStackFrame_InitFromMarshal'
// WARNING: Open finally blocks will not be executed; this simply undoes what the copy did previously
extern void _DeeStackFrame_Quit(DEE_A_IN struct DeeStackFrame *self) DEE_ATTRIBUTE_NONNULL((1));

struct DeeCodeObject;
struct DeeMarshalReadMap;
struct DeeMarshalWriteMap;
extern DEE_A_RET_EXCEPT(-1) int _DeeStackFrame_InitFromMarshal(
 DEE_A_OUT struct DeeStackFrame *self, DEE_A_INOUT struct DeeCodeObject *code,
#ifdef DEE_DEBUG
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeObject **argv,
 DEE_A_IN Dee_size_t refc, DEE_A_IN_R(refc) DeeObject **refv,
#else
 DEE_A_INOUT DeeObject **argv, DEE_A_INOUT DeeObject **refv,
#endif
 DEE_A_INOUT DeeObject *infile, DEE_A_INOUT struct DeeMarshalReadMap *map)
#ifdef DEE_DEBUG
 DEE_ATTRIBUTE_NONNULL((1,2,7,8))
#else
 DEE_ATTRIBUTE_NONNULL((1,2,5,6))
#endif
 ;
extern DEE_A_RET_EXCEPT(-1) int _DeeStackFrame_PutToMarshal(
 DEE_A_IN struct DeeStackFrame const *self, DEE_A_INOUT DeeObject *infile,
 DEE_A_INOUT struct DeeMarshalWriteMap *map) DEE_ATTRIBUTE_NONNULL((1,2,3));

#ifdef DEE_PRIVATE_DECL_DEE_VISIT_PROC
DEE_PRIVATE_DECL_DEE_VISIT_PROC
#undef DEE_PRIVATE_DECL_DEE_VISIT_PROC
#endif

#define _DeeStackFrame_Visit(ob,visit,closure)\
do{\
 DeeAtomicMutex_AcquireRelaxed(&(ob)->f_lock);\
 _DeeStackFrame_VisitLocked(ob,visit,closure);\
 DeeAtomicMutex_Release(&(ob)->f_lock);\
}while(0)
extern void _DeeStackFrame_VisitLocked(
 DEE_A_IN struct DeeStackFrame const *self,
 DEE_A_IN DeeVisitProc visit, void *closure) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_WUNUSED int _DeeStackFrame_WasExecuted(
 DEE_A_IN struct DeeStackFrame const *self) DEE_ATTRIBUTE_NONNULL((1));
#endif /* DEE_LIMITED_API */
#endif /* DEE_LIMITED_DEX */

#if DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS
#ifdef DEE_LIMITED_DEX
typedef int DeeExceptionHandlerKind; enum{
 DeeExceptionHandleKind_FLAG_ALL   = 0x01,
 DeeExceptionHandleKind_FLAG_TYPED = 0x02,
 DeeExceptionHandleKind_FLAG_VAR   = 0x04,
 DeeExceptionHandleKind_FINALLY    = 0x00,
 DeeExceptionHandleKind_ALL        = DeeExceptionHandleKind_FLAG_ALL,
 DeeExceptionHandleKind_ALL_VAR    = DeeExceptionHandleKind_FLAG_ALL|DeeExceptionHandleKind_FLAG_VAR,
 DeeExceptionHandleKind_TYPED      = DeeExceptionHandleKind_FLAG_TYPED,
 DeeExceptionHandleKind_TYPED_VAR  = DeeExceptionHandleKind_FLAG_TYPED|DeeExceptionHandleKind_FLAG_VAR,
};
struct DeeExceptionHandlerEntry {
 DeeExceptionHandlerKind  e_kind;    /*< The kind of handler. */
 DEE_A_REF DeeTypeObject *e_type;    /*< [0..1] The typing of objects handled by this (only used by *_TYPED* handlers). */
 Dee_size_t               e_store;   /*< Slot to store the exception in (only used by *_VAR* handlers). */
 Dee_size_t               e_stack;   /*< Before execution of the handler can be started, the stack has to be trimmed to this size. */
 Dee_size_t               e_handler; /*< Address of the handler (offset added to code base address). */
};
#define _DeeExceptionHandlerEntry_Quit(ob)\
do{ Dee_XDECREF((ob)->e_type); }while(0)
#define _DeeExceptionHandlerEntry_Visit(ob)\
do{ Dee_XVISIT((ob)->e_type); }while(0)
#define _DeeExceptionHandlerEntry_InitCopy(ob,right)\
do{ Dee_XINCREF((*(ob) = *(right)).e_type); }while(0)

struct DeeExceptionHandlerRef {
 struct DeeExceptionHandlerEntry *r_handler; /*< [1..1] The handler we are referencing. */
};
#endif /* DEE_LIMITED_DEX */
#endif /* DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS */


#ifdef DEE_LIMITED_API
//////////////////////////////////////////////////////////////////////////
// Raw, uncontrolled execution of code
#ifdef DEE_DEBUG
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeCode_RunDebug(
 DEE_A_IN_OBJECT(DeeCodeObject) *self,
 DEE_A_IN Dee_size_t refc, DEE_A_IN_OPT DeeObject **refv, // Optional depends on code in 'self'
 DEE_A_IN Dee_size_t argc, DEE_A_IN_OPT DeeObject **argv, // Optional depends on code in 'self'
 DEE_A_IN_OPT DeeObject *this_arg) // Optional depends on code in 'self'
 DEE_ATTRIBUTE_NONNULL((1));
#else
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeCode_Run(
 DEE_A_IN_OBJECT(DeeCodeObject) *self,
 DEE_A_IN_OPT DeeObject **refv,    // Optional depends on code in 'self'
 DEE_A_IN_OPT DeeObject **argv,    // Optional depends on code in 'self'
 DEE_A_IN_OPT DeeObject *this_arg) // Optional depends on code in 'self'
 DEE_ATTRIBUTE_NONNULL((1));
#endif
#endif

#ifdef DEE_LIMITED_API
struct DeeThreadObject;
extern DEE_A_RET_EXCEPT(-1) int _DeeStackFrame_RunLocked_AndUnlock(
 DEE_A_INOUT struct DeeThreadObject *thread_self,
 DEE_A_INOUT struct DeeStackFrame *_frame)
 DEE_ATTRIBUTE_NONNULL((1,2));
#endif


//////////////////////////////////////////////////////////////////////////
// Starts the interactive debugger on a given stackframe
// NOTE: '_frame' must be a thread-local stackframe
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeStackFrame_InteractiveDebugger(void);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeStackFrame_InteractiveDebuggerEx(
 DEE_A_INOUT_OBJECT(DeeThreadObject) *thread_self,
 DEE_A_INOUT struct DeeStackFrame *_frame) DEE_ATTRIBUTE_NONNULL((1,2));


// Known local variables kinds
#define DEE_STACKFRAME_VARTYPE_NONE   DEE_UINT16_C(0x0000)
#define DEE_STACKFRAME_VARTYPE_LOCALS DEE_UINT16_C(0x0001) // prefix: "L:" / "l:" / ""
#define DEE_STACKFRAME_VARTYPE_STATIC DEE_UINT16_C(0x0002) // prefix: "C:" / "c:"
#define DEE_STACKFRAME_VARTYPE_ARGS   DEE_UINT16_C(0x0004) // prefix: "A:" / "a:"
#define DEE_STACKFRAME_VARTYPE_REFS   DEE_UINT16_C(0x0008) // prefix: "R:" / "r:"
#define DEE_STACKFRAME_VARTYPE_THIS   DEE_UINT16_C(0x0010)
#define DEE_STACKFRAME_VARTYPE_ANY    DEE_UINT16_C(0x00FF)

typedef Dee_uint16_t DeeVarType;
typedef Dee_uint32_t DeeVarID;
typedef Dee_uint64_t DeeVarUID;
#define DEE_STACKFRAME_VARUID_MAKE(type,id) \
((DeeVarUID)((DeeVarID)(id))<<16|\
 (DeeVarUID)(((DeeVarType)(type))&DEE_STACKFRAME_VARTYPE_ANY))
#define DEE_STACKFRAME_VARUID_ID(uid) \
 ((DeeVarID)((((DeeVarUID)(uid))&DEE_UINT64_C(0x0000FFFFFFFF0000))>>16))
#define DEE_STACKFRAME_VARUID_TYPE(uid) \
 ((DeeVarType)(((DeeVarUID)(uid))&DEE_UINT64_C(0x00000000000000FF)))

// Iterator function for local variables
// 'uid' is the universal variable id (created with 'DEE_STACKFRAME_VARUID_MAKE')
// NOTE: 'name' might be NULL if variable names are not known
// NOTE: 'value' might be NULL if the variable isn't assigned
typedef int (*DeeStackFrame_LocalsIterFunc)(
 DEE_A_IN DeeVarUID uid, DEE_A_IN_OPT char const *name,
 DEE_A_IN_OPT DeeObject *value, void *closure);

//////////////////////////////////////////////////////////////////////////
// >> Enumerate all locals of the given set of types
// - Returns the last non-zero return value of 'func' when interrupted
// - Returns '0' if all locals were iterated
// If given, store the total amount of variables in 'total_count'
#define DeeStackFrame_EnumLocals(self,func,closure,total_count)\
 DeeStackFrame_EnumLocalsEx(self,func,closure,total_count,DEE_STACKFRAME_VARTYPE_ANY)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeStackFrame_EnumLocalsEx(
 DEE_A_INOUT struct DeeStackFrame const *self,
 DEE_A_IN DeeStackFrame_LocalsIterFunc func, void *closure,
 DEE_A_OUT_OPT Dee_size_t *total_count, DEE_A_IN Dee_uint16_t valid_types)
 DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Return the value of a given variable.
// @throw Error.ValueError:                Unknown id prefix / Ambigious variable name
// @throw Error.RuntimeError.UnboundLocal: The variable isn't assigned
// @throw Error.AttributeError:            The variable doesn't exist
// NOTE: If 'id' is a string, it may describe a number that is used as id instead
// NOTE: If it is a string, it may also be preceded by one of the prefixed as
//       listed in the comments of the 'Known local variables kinds' above.
// NOTE: This is only allowed if the associated type is part of 'valid_types'
//       e.g.: >> DeeStackFrame_GetLocal(frame,"L:foobar")
//             >> // same as:
//             >> DeeStackFrame_GetLocalEx(frame,"foobar",DEE_STACKFRAME_VARTYPE_LOCAL)
#define DeeStackFrame_GetLocal(self,name) \
 DeeStackFrame_GetLocalEx(self,name,DEE_STACKFRAME_VARTYPE_ANY)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_REF DeeObject *) DeeStackFrame_GetLocalEx(
 DEE_A_INOUT struct DeeStackFrame const *self, DEE_A_IN_Z char const *name,
 DEE_A_IN Dee_uint16_t valid_types) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_REF DeeObject *) DeeStackFrame_GetLocalID(
 DEE_A_INOUT struct DeeStackFrame const *self, DEE_A_IN DeeVarUID uid) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// >> Delete a given variable.
// @throw Error.ValueError:     Unknown id prefix / Ambigious variable name
// @throw Error.TypeError:      This type of variable can't be deleted
// @throw Error.AttributeError: The variable doesn't exist
// Returns -1 on error, 1 if the variable was already unbound deleted
// NOTE: If 'id' is a string, it may describe a number that is used as id instead
// NOTE: If it is a string, it may also be preceded by one of the prefixed as
//       listed in the comments of the 'Known local variables kinds' above.
// NOTE: This is only allowed if the associated type is part of 'valid_types'
//       e.g.: >> DeeStackFrame_DelLocal(frame,"L:foobar")
//             >> // same as:
//             >> DeeStackFrame_DelLocalEx(frame,"foobar",DEE_STACKFRAME_VARTYPE_LOCAL)
#define DeeStackFrame_DelLocal(self,name) \
 DeeStackFrame_DelLocalEx(self,name,DEE_STACKFRAME_VARTYPE_ANY)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeStackFrame_DelLocalEx(
 DEE_A_INOUT struct DeeStackFrame *self, DEE_A_IN_Z char const *name,
 DEE_A_IN Dee_uint16_t valid_types) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeStackFrame_DelLocalID(
 DEE_A_INOUT struct DeeStackFrame *self, DEE_A_IN DeeVarUID uid) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// >> Checks if a given variable exists.
// @throw Error.ValueError: Unknown id prefix / Ambigious variable name
// Returns -1 on error, 1 if the variable doesn't exist
// NOTE: If 'id' is a string, it may describe a number that is used as id instead
// NOTE: If it is a string, it may also be preceded by one of the prefixed as
//       listed in the comments of the 'Known local variables kinds' above.
// NOTE: This is only allowed if the associated type is part of 'valid_types'
//       e.g.: >> DeeStackFrame_HasLocal(frame,"L:foobar")
//             >> // same as:
//             >> DeeStackFrame_HasLocalEx(frame,"foobar",DEE_STACKFRAME_VARTYPE_LOCAL)
#define DeeStackFrame_HasLocal(self,name) \
 DeeStackFrame_HasLocalEx(self,name,DEE_STACKFRAME_VARTYPE_ANY)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeStackFrame_HasLocalEx(
 DEE_A_INOUT struct DeeStackFrame const *self, DEE_A_IN_Z char const *name,
 DEE_A_IN Dee_uint16_t valid_types) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeStackFrame_HasLocalID(
 DEE_A_INOUT struct DeeStackFrame const *self, DEE_A_IN DeeVarUID uid) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// >> Set the value of a given variable.
// @throw Error.ValueError:     Unknown id prefix / Ambigious variable name
// @throw Error.TypeError:      Can't assign to this type of variable
// @throw Error.AttributeError: The variable doesn't exist
// NOTE: If 'name' is a string, it may describe a number that is used as name instead
// NOTE: If 'id' is a string, it may describe a number that is used as id instead
// NOTE: If it is a string, it may also be preceded by one of the prefixed as
//       listed in the comments of the 'Known local variables kinds' above.
// NOTE: This is only allowed if the associated type is part of 'valid_types'
//       e.g.: >> DeeStackFrame_SetLocal(frame,"L:foobar",value)
//             >> // same as:
//             >> DeeStackFrame_SetLocalEx(frame,"foobar",value,DEE_STACKFRAME_VARTYPE_LOCAL)
#define DeeStackFrame_SetLocal(self,name,value) \
 DeeStackFrame_SetLocalEx(self,name,value,DEE_STACKFRAME_VARTYPE_ANY)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeStackFrame_SetLocalEx(
 DEE_A_INOUT struct DeeStackFrame *self, DEE_A_IN_Z char const *name,
 DEE_A_INOUT DeeObject *value, DEE_A_IN Dee_uint16_t valid_types) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeStackFrame_SetLocalID(
 DEE_A_INOUT struct DeeStackFrame *self, DEE_A_IN DeeVarUID uid,
 DEE_A_INOUT DeeObject *value) DEE_ATTRIBUTE_NONNULL((1,3));


//////////////////////////////////////////////////////////////////////////
// Returns the stack of the current thread
//  - Returns NULL if the thread could not be determined/no deemon code is running
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL struct DeeStackFrame *)
 DeeStackFrame_ThreadLocal(void);

//////////////////////////////////////////////////////////////////////////
// Returns the calling frame of a given stack frame
//  - Returns NULL if the given stack frame has no caller
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL struct DeeStackFrame *)
 DeeStackFrame_Prev(DEE_A_IN struct DeeStackFrame const *self)DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns debug information about a given stackframe.
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeStackFrame_Addr(
 DEE_A_IN struct DeeStackFrame const *self)DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_OBJECT_NOEXCEPT(DeeStringObject) *)
 DeeStackFrame_File(DEE_A_IN struct DeeStackFrame const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_int32_t) DeeStackFrame_Line(
 DEE_A_IN struct DeeStackFrame const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_OBJECT_NOEXCEPT(DeeStringObject) *)
 DeeStackFrame_Func(DEE_A_IN struct DeeStackFrame const *self) DEE_ATTRIBUTE_NONNULL((1));

DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_OBJECT(DeeCodeObject) *)
 DeeStackFrame_Code(DEE_A_IN struct DeeStackFrame const *self) DEE_ATTRIBUTE_NONNULL((1));


#ifdef DEE_LIMITED_API
extern DeeErrorObject _DeeRuntimeError_UnboundLocal;
extern DeeErrorObject _DeeRuntimeError_Unreachable;
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_EXECUTE_H */
