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
#ifndef GUARD_DEEMON_OPTIONAL_OBJECT_REFCNT_H
#define GUARD_DEEMON_OPTIONAL_OBJECT_REFCNT_H 1

#include <deemon/__conf.inl>
#include <deemon/__atomic_intrin.inl>
#include <deemon/optional/object_util.h>
#include <deemon/optional/object_decl.h>

// Define as 0: disabled
// Define as 1: check on decref
// Define as 2: check on incref and decref
#define DEE_CONFIG_HAVE_MSVC_MEMORY_DEBUG 0

#ifndef _MSC_VER
#undef DEE_CONFIG_HAVE_MSVC_MEMORY_DEBUG
#define DEE_CONFIG_HAVE_MSVC_MEMORY_DEBUG 0
#endif

#if DEE_CONFIG_HAVE_MSVC_MEMORY_DEBUG
#include DEE_INCLUDE_MEMORY_API_DISABLE()
DEE_COMPILER_MSVC_WARNING_PUSH(4820 4565)
#include <crtdbg.h>
DEE_COMPILER_MSVC_WARNING_POP
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#endif /* DEE_CONFIG_HAVE_MSVC_MEMORY_DEBUG */

DEE_DECL_BEGIN

#if DEE_TYPES_SIZEOF_INT == 4
#define _DeeRefCounter_Fetch(x)     ((unsigned int)DeeAtomic32_Load(x,memory_order_seq_cst))
#define _DeeRefCounter_FetchInc(x)  ((unsigned int)DeeAtomic32_FetchInc(x,memory_order_seq_cst))
#define _DeeRefCounter_FetchDec(x)  ((unsigned int)DeeAtomic32_FetchDec(x,memory_order_seq_cst))
#define _DeeRefCounter_IncFetch(x)  ((unsigned int)DeeAtomic32_IncFetch(x,memory_order_seq_cst))
#define _DeeRefCounter_DecFetch(x)  ((unsigned int)DeeAtomic32_DecFetch(x,memory_order_seq_cst))
#else
#define _DeeRefCounter_Fetch(x)     ((unsigned int)DeeAtomicN_Load(DEE_TYPES_SIZEOF_INT,x,memory_order_seq_cst))
#define _DeeRefCounter_FetchInc(x)  ((unsigned int)DeeAtomicN_FetchInc(DEE_TYPES_SIZEOF_INT,x,memory_order_seq_cst))
#define _DeeRefCounter_FetchDec(x)  ((unsigned int)DeeAtomicN_FetchDec(DEE_TYPES_SIZEOF_INT,x,memory_order_seq_cst))
#define _DeeRefCounter_IncFetch(x)  ((unsigned int)DeeAtomicN_IncFetch(DEE_TYPES_SIZEOF_INT,x,memory_order_seq_cst))
#define _DeeRefCounter_DecFetch(x)  ((unsigned int)DeeAtomicN_DecFetch(DEE_TYPES_SIZEOF_INT,x,memory_order_seq_cst))
#endif


//////////////////////////////////////////////////////////////////////////
// 'DeeObject_IS_UNIQUE' can actually be used when multi-threading, because:
// - The check absolute against '1'
// - The caller will hold some kind of reference to the object
// - The call fails if someone else holds a reference as well
// - Essentially: To get a reference you must already have one, or be given one
//   So if the caller owns the object (refcnt == 1), then nobody else
//   will be able to get a new reference, meaning that the caller is the absolute owner
#if 1
// We use this version, to guaranty that the object can't just randomly
// not be unique any longer. But as a consequence, the object could
// just be referenced through a weak reference, meaning that this version
// doesn't imply that 'ob' will live on if it's decrefed.
#define DeeObject_IS_UNIQUE(ob) \
((Dee_uint64_t)DeeAtomic64_Load(((struct DeeObject*)Dee_REQUIRES_POINTER(ob))->__ob_refcnt,memory_order_seq_cst)==\
  DEE_UINT64_C(0x0000000100000001))
#else
//   EXCEPTION: Someone else holds a weak reference and locks it
//   NOTE: We could theoretically work around this restriction by
//         loading both refcnt & weakrefcnt with 'DeeAtomic64_Load'
//         and check if its equal to '0x0000000100000001ui64'.
//      >> But that would kind of defeat the purpose of weak references
//         only being meant to sleep in the background and not
//         interfere with the regular lifetime of any given object...
// >> However itn't relyable if 'ob' is held through a weak_reference,
//    but why would you check if the object is unique before locking it?
#define DeeObject_IS_UNIQUE(ob) (_DeeRefCounter_Fetch(((struct DeeObject*)Dee_REQUIRES_POINTER(ob))->__ob_refcnt)==1)
#endif

//////////////////////////////////////////////////////////////////////////
// Not reliable when multi-threading (object may have already been revived upon return of this call)
#define DeeObject_IS_ALIVE(ob)  (_DeeRefCounter_Fetch((ob)->__ob_refcnt)!=0)
#define DeeObject_INIT(ob,ob_type) (void)(\
                                              ((struct DeeObject*)Dee_REQUIRES_POINTER(ob))->__ob_refcnt=1,\
                                              ((struct DeeObject*)(ob))->__ob_weakcnt=1,\
 _DeeRefCounter_IncFetch(((struct DeeObject*)(((struct DeeObject*)(ob))->__ob_type=(ob_type)))->__ob_refcnt))

#if defined(_MSC_VER) && DEE_CONFIG_HAVE_MSVC_MEMORY_DEBUG >= 1
#define DEE_DEBUG_CHECK_MEMORY _CrtCheckMemory
#elif 1
//#define DEE_DEBUG_CHECK_MEMORY ...
#endif

#if !defined(DEE_DEBUG_CHECK_MEMORY) && \
    DEE_CONFIG_HAVE_MSVC_MEMORY_DEBUG
#undef DEE_CONFIG_HAVE_MSVC_MEMORY_DEBUG
#define DEE_CONFIG_HAVE_MSVC_MEMORY_DEBUG 0
#endif

#ifdef DEE_DEBUG
#define /*DEE_A_EXEC*/_DeeInternalObject_DECREF(ob) \
{unsigned int const _temp = _DeeRefCounter_DecFetch(ob->__ob_refcnt);\
 if(!_temp)_DeeObject_InternalDestroy_d(ob,__FILE__,__LINE__);\
 else DEE_ASSERTF(_temp!=0xFFFFFFFF,"Dee_DECREF(...) : Dead object");}
#else /* DEE_DEBUG */
#define /*DEE_A_EXEC*/_DeeInternalObject_DECREF(ob) \
 do{_DeeRefCounter_DecFetch(ob->__ob_refcnt)?(void)0:_DeeObject_InternalDestroy(ob);}while(0)
#endif /* !DEE_DEBUG */
#ifdef DEE_DEBUG
#define               _DeeInternalObject_INCREF(ob) (_DeeFlag_NoAssert\
 ?(void)_DeeRefCounter_IncFetch(ob->__ob_refcnt):(_DeeRefCounter_FetchInc(ob->__ob_refcnt)==0\
 ?_Dee_AssertionFailed("Dee_INCREF(...) : Dead object",__FILE__,__LINE__):(void)0))
#else
#define               _DeeInternalObject_INCREF(ob) _DeeRefCounter_IncFetch(ob->__ob_refcnt)
#endif
#define /*DEE_A_EXEC*/_DeeObject_DECREF      _DeeInternalObject_DECREF
#define               _DeeObject_INCREF      _DeeInternalObject_INCREF
#if DEE_CONFIG_HAVE_MSVC_MEMORY_DEBUG >= 2
#ifdef DEE_DEBUG
#define Dee_INCREF(ob)  do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);DEE_ASSERT(_dee_ob);_DeeObject_INCREF(_dee_ob);DEE_DEBUG_CHECK_MEMORY();}while(0)
#else
#define Dee_INCREF(ob)  do{_DeeObject_INCREF(((struct DeeObject*)Dee_REQUIRES_POINTER(ob)));DEE_DEBUG_CHECK_MEMORY();}while(0)
#endif
#define Dee_XINCREF(ob) do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);if(_dee_ob){_DeeObject_INCREF(_dee_ob);DEE_DEBUG_CHECK_MEMORY();}}while(0)
#else
#ifdef DEE_DEBUG
#define Dee_INCREF(ob)  do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);DEE_ASSERT(_dee_ob);_DeeObject_INCREF(_dee_ob);}while(0)
#else
#define Dee_INCREF(ob)  do{_DeeObject_INCREF(((struct DeeObject*)Dee_REQUIRES_POINTER(ob)));}while(0)
#endif
#define Dee_XINCREF(ob) do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);if(_dee_ob)_DeeObject_INCREF(_dee_ob);}while(0)
#endif
#ifdef DEE_DEBUG_CHECK_MEMORY
#define /*DEE_A_EXEC*/Dee_DECREF(ob)  do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);_DeeObject_DECREF(_dee_ob);DEE_DEBUG_CHECK_MEMORY();}while(0)
#define /*DEE_A_EXEC*/Dee_XDECREF(ob) do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);if(_dee_ob){_DeeObject_DECREF(_dee_ob);DEE_DEBUG_CHECK_MEMORY();}}while(0)
#define /*DEE_A_EXEC*/Dee_CLEAR(ob)   do{DEE_ASSERT(ob);_DeeObject_DECREF(((struct DeeObject*)Dee_REQUIRES_POINTER(ob)));DEE_DEBUG_CHECK_MEMORY();(ob)=NULL;}while(0)
#define /*DEE_A_EXEC*/Dee_XCLEAR(ob)  do{if(ob){_DeeObject_DECREF(((struct DeeObject*)Dee_REQUIRES_POINTER(ob)));DEE_DEBUG_CHECK_MEMORY();(ob)=NULL;}}while(0)
#else
#define /*DEE_A_EXEC*/Dee_DECREF(ob)  do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);DEE_ASSERT(_dee_ob);_DeeObject_DECREF(_dee_ob);}while(0)
#define /*DEE_A_EXEC*/Dee_XDECREF(ob) do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);if(_dee_ob)_DeeObject_DECREF(_dee_ob);}while(0)
#define /*DEE_A_EXEC*/Dee_CLEAR(ob)   do{DEE_ASSERT(ob);_DeeObject_DECREF(((struct DeeObject*)Dee_REQUIRES_POINTER(ob)));(ob)=NULL;}while(0)
#define /*DEE_A_EXEC*/Dee_XCLEAR(ob)  do{if(ob){_DeeObject_DECREF(((struct DeeObject*)Dee_REQUIRES_POINTER(ob)));(ob)=NULL;}}while(0)
#endif
#if DEE_CONFIG_HAVE_MSVC_MEMORY_DEBUG >= 2
#define DeeReturn_NEWREF(ob)  do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);DEE_ASSERT(_dee_ob);_DeeObject_INCREF(_dee_ob);DEE_DEBUG_CHECK_MEMORY();return _dee_ob;}while(0)
#define DeeReturn_XNEWREF(ob) do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);if(_dee_ob){_DeeObject_INCREF(_dee_ob);DEE_DEBUG_CHECK_MEMORY();}return _dee_ob;}while(0)
#else
#define DeeReturn_NEWREF(ob)  do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);DEE_ASSERT(_dee_ob);_DeeObject_INCREF(_dee_ob);return _dee_ob;}while(0)
#define DeeReturn_XNEWREF(ob) do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);if(_dee_ob)_DeeObject_INCREF(_dee_ob);return _dee_ob;}while(0)
#endif
DEE_FUNC_DECL(           void) Dee_IncRef(DEE_A_INOUT struct DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC void) Dee_DecRef(DEE_A_INOUT struct DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(           void) Dee_XIncRef(DEE_A_INOUT_OPT struct DeeObject *ob);
DEE_FUNC_DECL(DEE_A_EXEC void) Dee_XDecRef(DEE_A_INOUT_OPT struct DeeObject *ob);

DEE_FUNC_DECL(DEE_A_EXEC void) _DeeObject_InternalDestroy(
 DEE_A_INOUT DEE_A_OUT_INVALID struct DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC void) _DeeObject_InternalDestroy_d(
 DEE_A_INOUT DEE_A_OUT_INVALID struct DeeObject *ob, DEE_A_IN_Z char const *file, DEE_A_IN int line) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Weak reference counting API
DEE_FUNC_DECL(void) _DeeObject_InternalDestroyWeak(
 DEE_A_INOUT DEE_A_OUT_INVALID struct DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) _DeeObject_InternalDestroyWeak_d(
 DEE_A_INOUT DEE_A_OUT_INVALID struct DeeObject *ob, DEE_A_IN_Z char const *file, DEE_A_IN int line) DEE_ATTRIBUTE_NONNULL((1,2));

#ifdef DEE_DEBUG
#define _DeeeInternalObject_WEAKDECREF(ob) (_DeeRefCounter_DecFetch(ob->__ob_weakcnt)?(void)0:_DeeObject_InternalDestroyWeak_d(ob,__FILE__,__LINE__))
#else /* DEE_DEBUG */
#define _DeeeInternalObject_WEAKDECREF(ob) (_DeeRefCounter_DecFetch(ob->__ob_weakcnt)?(void)0:_DeeObject_InternalDestroyWeak(ob))
#endif /* !DEE_DEBUG */
#define _DeeeInternalObject_WEAKINCREF(ob)  _DeeRefCounter_IncFetch(ob->__ob_weakcnt)
#define _DeeObject_WEAKINCREF _DeeeInternalObject_WEAKINCREF
#define _DeeObject_WEAKDECREF _DeeeInternalObject_WEAKDECREF
#if DEE_CONFIG_HAVE_MSVC_MEMORY_DEBUG >= 2
#ifdef DEE_DEBUG
#define Dee_WEAKINCREF(ob)  do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);DEE_ASSERT(_dee_ob);_DeeObject_WEAKINCREF(_dee_ob);_CrtCheckMemory();}while(0)
#else /* DEE_DEBUG */
#define Dee_WEAKINCREF(ob)  do{_DeeObject_WEAKINCREF(((struct DeeObject*)Dee_REQUIRES_POINTER(ob)));_CrtCheckMemory();}while(0)
#endif /* !DEE_DEBUG */
#define Dee_WEAKXINCREF(ob) do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);if(_dee_ob){_DeeObject_WEAKINCREF(_dee_ob);_CrtCheckMemory();}}while(0)
#else
#ifdef DEE_DEBUG
#define Dee_WEAKINCREF(ob)  do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);DEE_ASSERT(_dee_ob);_DeeObject_WEAKINCREF(_dee_ob);}while(0)
#else /* DEE_DEBUG */
#define Dee_WEAKINCREF(ob)  do{_DeeObject_WEAKINCREF(((struct DeeObject*)Dee_REQUIRES_POINTER(ob)));}while(0)
#endif /* !DEE_DEBUG */
#define Dee_WEAKXINCREF(ob) do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);if(_dee_ob)_DeeObject_WEAKINCREF(_dee_ob);}while(0)
#endif
#if DEE_CONFIG_HAVE_MSVC_MEMORY_DEBUG
#define Dee_WEAKDECREF(ob)  do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);DEE_ASSERT(_dee_ob);_DeeObject_WEAKDECREF(_dee_ob);_CrtCheckMemory();}while(0)
#define Dee_WEAKXDECREF(ob) do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);if(_dee_ob){_DeeObject_WEAKDECREF(_dee_ob);_CrtCheckMemory();}}while(0)
#else
#define Dee_WEAKDECREF(ob)  do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);DEE_ASSERT(_dee_ob);_DeeObject_WEAKDECREF(_dee_ob);}while(0)
#define Dee_WEAKXDECREF(ob) do{struct DeeObject*const _dee_ob=(struct DeeObject*)Dee_REQUIRES_POINTER(ob);if(_dee_ob)_DeeObject_WEAKDECREF(_dee_ob);}while(0)
#endif

DEE_FUNC_DECL(void) Dee_WeakIncRef(DEE_A_INOUT struct DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) Dee_WeakDecRef(DEE_A_INOUT struct DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) Dee_WeakXIncRef(DEE_A_INOUT_OPT struct DeeObject *ob);
DEE_FUNC_DECL(void) Dee_WeakXDecRef(DEE_A_INOUT_OPT struct DeeObject *ob);

#ifdef __INTELLISENSE__
// Lighten the load on Intellisense
#undef Dee_INCREF
#undef Dee_DECREF
#undef Dee_XINCREF
#undef Dee_XDECREF
#undef Dee_WEAKINCREF
#undef Dee_WEAKDECREF
#undef Dee_WEAKXINCREF
#undef Dee_WEAKXDECREF
#define Dee_INCREF      if(0);else Dee_REQUIRES_POINTER
#define Dee_DECREF      if(0);else Dee_REQUIRES_POINTER
#define Dee_XINCREF     if(0);else Dee_REQUIRES_POINTER
#define Dee_XDECREF     if(0);else Dee_REQUIRES_POINTER
#define Dee_WEAKINCREF  if(0);else Dee_REQUIRES_POINTER
#define Dee_WEAKDECREF  if(0);else Dee_REQUIRES_POINTER
#define Dee_WEAKXINCREF if(0);else Dee_REQUIRES_POINTER
#define Dee_WEAKXDECREF if(0);else Dee_REQUIRES_POINTER
#endif

//////////////////////////////////////////////////////////////////////////
// Just a tiny helper macro to annotate reference transfer in the api implementation
#ifndef Dee_INHERIT_REF
#ifdef DEE_DEBUG
#define Dee_INHERIT_REF(dst,ob) ((dst)=Dee_REQUIRES_POINTER(ob),(ob)=NULL)
#else
#define Dee_INHERIT_REF(dst,ob) ((dst)=Dee_REQUIRES_POINTER(ob))
#endif
#endif

//////////////////////////////////////////////////////////////////////////
// Lock a weak reference to "self"
// >> Returns NULL, if the object is dead
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT_REF struct DeeObject *) DeeObject_LockWeakref(
 DEE_A_INOUT struct DeeObject *self) DEE_ATTRIBUTE_NONNULL((1));


#if DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS && defined(DEE_LIMITED_DEX)
#define Dee_IncRef(ob)      DEE_GCC_EXTENSION({ Dee_INCREF(ob); })
#define Dee_DecRef(ob)      DEE_GCC_EXTENSION({ Dee_DECREF(ob); })
#define Dee_XIncRef(ob)     DEE_GCC_EXTENSION({ Dee_XINCREF(ob); })
#define Dee_XDecRef(ob)     DEE_GCC_EXTENSION({ Dee_XDECREF(ob); })
#define Dee_WeakIncRef(ob)  DEE_GCC_EXTENSION({ Dee_WEAKINCREF(ob); })
#define Dee_WeakDecRef(ob)  DEE_GCC_EXTENSION({ Dee_WEAKDECREF(ob); })
#define Dee_WeakXIncRef(ob) DEE_GCC_EXTENSION({ Dee_WEAKXINCREF(ob); })
#define Dee_WeakXDecRef(ob) DEE_GCC_EXTENSION({ Dee_WEAKXDECREF(ob); })
#endif /* DEE_COMPILER_HAVE_GCC_STATEMENT_EXPRESSIONS */

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_OBJECT_REFCNT_H */
