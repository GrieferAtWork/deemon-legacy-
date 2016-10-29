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
#ifndef GUARD_DEEMON_CODE_H
#define GUARD_DEEMON_CODE_H 1

#include <deemon/__conf.inl>
#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#include <deemon/optional/atomic_mutex.h>
#include <deemon/optional/string_forward.h>
#endif
#ifdef DEE_LIMITED_DEX
#include <deemon/tuple.h>
#ifdef DEE_DEBUG
#include <deemon/type.h>
#include <deemon/string.h>
#endif
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

DEE_OBJECT_DEF(DeeCodeObject);
DEE_OBJECT_DEF(DeeCodeDebugInfoObject);

#ifdef DEE_LIMITED_API
struct DeeTupleObject;
struct DeeExceptionHandlerEntry;
struct DeeExternFuncDecl;
#endif

#ifdef DEE_LIMITED_DEX
struct _DeeCodeLnOffList {
 Dee_uint8_t lno_addroff; /*< Address offset. */
 Dee_int8_t  lno_lineoff; /*< Line offset. */
};

#ifdef DEE_LIMITED_API
extern DEE_A_RET_WUNUSED int _DeeCodeLnOffList_CompareLo(
 DEE_A_IN_OPT struct _DeeCodeLnOffList const *lhs,
 DEE_A_IN_OPT struct _DeeCodeLnOffList const *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_WUNUSED int _DeeCodeLnOffList_CompareLe(
 DEE_A_IN_OPT struct _DeeCodeLnOffList const *lhs,
 DEE_A_IN_OPT struct _DeeCodeLnOffList const *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_WUNUSED int _DeeCodeLnOffList_CompareEq(
 DEE_A_IN_OPT struct _DeeCodeLnOffList const *lhs,
 DEE_A_IN_OPT struct _DeeCodeLnOffList const *rhs) DEE_ATTRIBUTE_NONNULL((1,2));
#define _DeeCodeLnOffList_CompareNe(lhs,rhs) (!_DeeCodeLnOffList_CompareEq(lhs,rhs))
#define _DeeCodeLnOffList_CompareGr(lhs,rhs) (!_DeeCodeLnOffList_CompareLe(lhs,rhs))
#define _DeeCodeLnOffList_CompareGe(lhs,rhs) (!_DeeCodeLnOffList_CompareLo(lhs,rhs))
extern DEE_A_RET_WUNUSED Dee_size_t _DeeCodeLnOffList_Size(
 DEE_A_IN struct _DeeCodeLnOffList const *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(NULL) struct _DeeCodeLnOffList *_DeeCodeLnOffList_Copy(
 DEE_A_IN struct _DeeCodeLnOffList const *self) DEE_ATTRIBUTE_NONNULL((1));
#define _DeeCodeLnOffList_Free free_nn
#endif /* DEE_LIMITED_API */

#define _DeeCodeLnOffList_EMPTY(ob) (*((Dee_uint16_t *)(ob))==0)
DEE_STATIC_INLINE(DEE_A_RET_WUNUSED Dee_int64_t) _DeeCodeLnOffList_Addr2Line(
 DEE_A_IN_OPT struct _DeeCodeLnOffList *off, DEE_A_IN Dee_size_t addr) {
 Dee_int64_t result; Dee_size_t coff;
 if (!off) return -1;
 result = 0,coff = 0;
 while (!_DeeCodeLnOffList_EMPTY(off)) {
  coff += off->lno_addroff;
  if (coff >= addr) break;
  result += off->lno_lineoff;
  ++off;
 }
 return result;
}
#endif /* DEE_LIMITED_DEX */

#ifdef DEE_LIMITED_DEX
struct DeeCodeDebugInfoObject {
 DEE_OBJECT_HEAD
 DEE_A_REF struct DeeStringObject *di_name;        /*< [0..1] Name of the code. */
 DEE_A_REF struct DeeTupleObject  *di_local_names; /*< [0..1] Tuple of strings containing the names of locals. */
 DEE_A_REF struct DeeTupleObject  *di_const_names; /*< [0..1] Tuple of strings containing the names of static variables/constants. */
 DEE_A_REF struct DeeTupleObject  *di_ref_names;   /*< [0..1] Tuple of strings containing the names of referenced variables. */
 DEE_A_REF struct DeeTupleObject  *di_arg_names;   /*< [0..1] Tuple of strings containing the names of argument variables. */
 struct _DeeCodeLnOffList         *di_lno;         /*< [0..1][owned(_DeeCodeLnOffList_Free)] Conversion list between addr and line. */
 struct _DeeCodeLnOffList         *di_fno;         /*< [0..1][owned(_DeeCodeLnOffList_Free)] Conversion list between addr and file. */
 DEE_A_REF struct DeeTupleObject  *di_file_names;  /*< [0..1] Tuple of strings containing the names of all known files (Index is 'file_id-1'). */
};
#define DeeCodeDebugInfo_NAME(ob) \
 ((DeeObject *)((DeeCodeDebugInfoObject *)Dee_REQUIRES_POINTER(ob))->di_name)
#define DeeCodeDebugInfo_ADDR2LINE(ob,addr) \
 _DeeCodeLnOffList_Addr2Line(((DeeCodeDebugInfoObject *)Dee_REQUIRES_POINTER(ob))->di_lno,addr)
#define DeeCodeDebugInfo_ADDR2FILE(ob,addr) \
 DeeCodeDebugInfo_Addr2File((DeeCodeDebugInfoObject *)Dee_REQUIRES_POINTER(ob),addr)
#define DeeCodeDebugInfo_ADDR2FILEID(ob,addr) \
 _DeeCodeLnOffList_Addr2Line(((DeeCodeDebugInfoObject *)Dee_REQUIRES_POINTER(ob))->di_fno,addr)

#ifdef DEE_LIMITED_API
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeCodeDebugInfoObject) *_DeeCodeDebugInfo_New(void);
#endif /* DEE_LIMITED_API */
#endif /* DEE_LIMITED_DEX */

#define DeeCodeDebugInfo_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeCodeDebugInfo_Type)
#define DeeCodeDebugInfo_Check          DeeCodeDebugInfo_CheckExact
DEE_DATA_DECL(DeeTypeObject) DeeCodeDebugInfo_Type;

#ifdef DEE_LIMITED_DEX
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_NOEXCEPT(DeeStringObject) *) DeeCodeDebugInfo_Addr2File(
 DEE_A_IN DeeCodeDebugInfoObject const *self, DEE_A_IN Dee_size_t addr) {
 Dee_int64_t fileid;
 DEE_ASSERT(DeeObject_Check(self) && DeeCodeDebugInfo_Check(self));
 fileid = DeeCodeDebugInfo_ADDR2FILEID(self,addr)-1;
 if (fileid < 0 || fileid >= DeeTuple_SIZE(self->di_file_names)) return NULL;
#ifdef DEE_DEBUG
 {
  DeeObject *result = DeeTuple_GET(self->di_file_names,(Dee_size_t)fileid);
  DEE_ASSERT(DeeObject_Check(result) && DeeString_Check(result));
  return result;
 }
#else
 return DeeTuple_GET(self->di_file_names,(Dee_size_t)fileid);
#endif
}

#endif




#ifdef DEE_LIMITED_DEX
struct DeeCodeObject {
 DEE_OBJECT_HEAD
 struct DeeCodeDebugInfoObject    *co_debug;       /*< [0..1] Debug information. */
 struct DeeAtomicMutex             co_consts_lock; /*< Lock used for static variables (not required for access to non-static constants). */
 DEE_A_REF struct DeeTupleObject  *co_consts;      /*< [1..1] Tuple of all const objects used. */
#ifdef DEE_LIMITED_API
#define DEE_CODE_FLAG_NONE         0x0000
#define DEE_CODE_FLAG_COPY         0x0001          /*< Allow stackframe entries with this code to be copied. - __attribute__((__copyable__)) */
 Dee_uint16_t                      co_flags;       /*< Code flags. */
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 struct DeeExceptionHandlerEntry  *co_exceptv;     /*< [0..ob_exceptc] List of known exception handlers. */
 Dee_size_t                        co_exceptc;     /*< Amount of known exception handlers. */
 Dee_size_t                        co_finallyc;    /*< Min amount of finally return addresses required. */
 Dee_size_t                        co_exceptsize;  /*< Min amount of exception handlers required. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
 Dee_size_t                        co_nlocals;     /*< Amount of local variables required (>= argument count). */
 Dee_size_t                        co_stacksize;   /*< Min amount of stack entries required. */
 // Work buffer layout:
 //  >> [+0]               stack
 //  >> [+co_woff_loc]     locals
 //  >> [+co_woff_except]  except handlers
 //  >> [+co_woff_finally] finally return addresses
 //  >> [+co_woff_sdata]   stack-allocated userdata
 //  >> [+co_wsize]        EOD (end-of-data / invalid memory)
 Dee_size_t                        co_wsize;        /*< Total size of the execution buffer. (Never 0, so just use malloc_nz / alloca) */
 Dee_size_t                        co_woff_loc;     /*< Offset of local variables in the execution buffer. */
#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
 Dee_size_t                        co_woff_except;  /*< Offset of exception handlers in the execution buffer. */
 Dee_size_t                        co_woff_finally; /*< Offset of finally return addresses in the execution buffer. */
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS */
#define DeeCode_SDATASIZE(ob) \
(((DeeCodeObject *)Dee_REQUIRES_POINTER(ob))->co_wsize-\
 ((DeeCodeObject *)Dee_REQUIRES_POINTER(ob))->co_woff_sdata)
 Dee_size_t                        co_woff_sdata;   /*< Offset of userdata allocated on the stack. */
 Dee_size_t                        co_size;         /*< Size of the bytecode (in bytes). */
 Dee_uint8_t                       co_code[1];      /*< [co_size] Bytecode executed by this object (uses the remainder of the object). */
#endif /* DEE_LIMITED_API */
};

#ifdef DEE_LIMITED_API
#define _DeeCode_InitCommon(ob)\
 DeeAtomicMutex_Init(&(ob)->co_consts_lock)

#if DEE_CONFIG_LANGUAGE_HAVE_EXCEPTIONS
#define DeeCode_CALCULATE_WBUFFERSIZE(ob,stackdata_size)\
do{\
 (ob)->co_woff_loc = (ob)->co_stacksize*sizeof(DeeObject *);\
 (ob)->co_woff_except = (ob)->co_woff_loc+((ob)->co_nlocals*sizeof(DeeObject *));\
 (ob)->co_woff_finally = (ob)->co_woff_except+((ob)->co_exceptsize*sizeof(struct DeeExceptionHandlerRef));\
 (ob)->co_woff_sdata = (ob)->co_woff_finally+((ob)->co_finallyc*sizeof(Dee_uint8_t const *));\
 (ob)->co_wsize = (ob)->co_woff_sdata+(stackdata_size);\
 /* Optimizations to reduce offset calculation later down the line */\
 if (!(ob)->co_nlocals) (ob)->co_woff_loc = 0;\
 if (!(ob)->co_exceptsize) (ob)->co_woff_except = 0;\
 if (!(ob)->co_finallyc) (ob)->co_woff_finally = 0;\
 /* So we can use malloc_nz and alloca without any checks */\
 if (!(ob)->co_wsize) (ob)->co_wsize = 1;\
}while(0)
#else
#define DeeCode_CALCULATE_WBUFFERSIZE(ob,stackdata_size)\
do{\
 (ob)->co_woff_loc = (ob)->co_stacksize*sizeof(DeeObject *);\
 (ob)->co_woff_sdata = (ob)->co_woff_loc+((ob)->co_nlocals*sizeof(DeeObject *));\
 (ob)->co_wsize = (ob)->co_woff_sdata+(stackdata_size);\
 /* Optimizations to reduce offset calculation later down the line */\
 if (!(ob)->co_nlocals) (ob)->co_woff_loc = 0;\
 /* So we can use malloc_nz and alloca without any checks */\
 if (!(ob)->co_wsize) (ob)->co_wsize = 1;\
}while(0)
#endif
#define DeeCode_CODE(ob)       ((Dee_uint8_t *)((DeeCodeObject *)Dee_REQUIRES_POINTER(ob))->co_code)
#define DeeCode_SIZE(ob)          ((Dee_size_t)((DeeCodeObject *)Dee_REQUIRES_POINTER(ob))->co_size)
#define DeeCode_PTR2ADDR(ob,p)     (Dee_size_t)((Dee_uintptr_t)(p)-(Dee_uintptr_t)DeeCode_CODE(ob))
#endif /* DEE_LIMITED_API */
#define DeeCode_CONSTS(ob)        ((DeeObject *)((DeeCodeObject *)Dee_REQUIRES_POINTER(ob))->co_consts)
#define DeeCode_DEBUG(ob)         ((DeeObject *)((DeeCodeObject *)Dee_REQUIRES_POINTER(ob))->co_debug)
#define DeeCode_NAME(ob)           (DeeCode_DEBUG(ob)?DeeCodeDebugInfo_NAME(DeeCode_DEBUG(ob)):NULL)
#define DeeCode_ADDR2FILE(ob,addr) (DeeCode_DEBUG(ob)?DeeCodeDebugInfo_ADDR2FILE(DeeCode_DEBUG(ob),addr):NULL)
#define DeeCode_ADDR2LINE(ob,addr) (DeeCode_DEBUG(ob)?DeeCodeDebugInfo_ADDR2LINE(DeeCode_DEBUG(ob),addr):-1)
#else
#define DeeCode_CONSTS(ob)         DeeCode_Consts((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeCode_NAME(ob)           DeeCode_Name((DeeObject *)Dee_REQUIRES_POINTER(ob))
#define DeeCode_ADDR2FILE(ob,addr) DeeCode_Addr2File((DeeObject *)Dee_REQUIRES_POINTER(ob),addr)
#define DeeCode_ADDR2LINE(ob,addr) DeeCode_Addr2Line((DeeObject *)Dee_REQUIRES_POINTER(ob),addr)
#endif

#ifndef DeeCode_SIZE
#define DeeCode_SIZE(ob)           DeeCode_Size((DeeObject *)Dee_REQUIRES_POINTER(ob))
#endif

//////////////////////////////////////////////////////////////////////////
// Returns the line of a given code address
DEE_FUNC_DECL(DEE_A_RET_OBJECT_NOEXCEPT(DeeStringObject) *) DeeCode_Addr2File(
 DEE_A_IN_OBJECT(DeeCodeObject) const *self, DEE_A_IN Dee_size_t addr) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_int64_t) DeeCode_Addr2Line(
 DEE_A_IN_OBJECT(DeeCodeObject) const *self, DEE_A_IN Dee_size_t addr) DEE_ATTRIBUTE_NONNULL((1));
#ifdef DEE_LIMITED_DEX
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeCode_Ptr2Addr(
 DEE_A_IN_OBJECT(DeeCodeObject) const *self, DEE_A_IN void const *ptr) DEE_ATTRIBUTE_NONNULL((1));
#ifndef DeeCode_PTR2ADDR
#define DeeCode_PTR2ADDR(ob,ptr) DeeCode_Ptr2Addr((DeeObject *)Dee_REQUIRES_POINTER(ob),ptr)
#endif
#endif

//////////////////////////////////////////////////////////////////////////
// Returns the total size of the bytecode
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeCode_Size(
 DEE_A_IN_OBJECT(DeeCodeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns the debug information associated with the code, or NULL if none are available
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_OBJECT_MAYBE(DeeCodeDebugInfoObject) *)
 DeeCode_Debug(DEE_A_IN_OBJECT(DeeCodeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns the total amount of instructions contained within the code
// >> 'self.count_instructions()'
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeCode_InstructionCount(
 DEE_A_IN_OBJECT(DeeCodeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns the name of the code or NULL if unknown
// NOTE: If the code was compiled without debug information, NULL will always be returned
// >> 'self.name'
DEE_FUNC_DECL(DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_OBJECT_MAYBE(DeeStringObject) *)
 DeeCode_Name(DEE_A_IN_OBJECT(DeeCodeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns a human-readable string representing the assembly instructions of the code
// >> 'self.asmrepr()'
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *) DeeCode_AsmRepr(
 DEE_A_IN_OBJECT(DeeCodeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));



#define DeeCode_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeCode_Type)
#define DeeCode_Check          DeeCode_CheckExact
DEE_DATA_DECL(DeeTypeObject) DeeCode_Type;

// Empty/no-op code (when executed, simply returns 'none')
#define Dee_EmptyCode  ((DeeObject *)&_Dee_EmptyCode)
DEE_DATA_DECL(DeeCodeObject) _Dee_EmptyCode;

DEE_DECL_END

#endif /* !GUARD_DEEMON_CODE_H */
