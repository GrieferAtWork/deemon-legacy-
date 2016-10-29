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
#ifndef GUARD_DEEMON_OPTIONAL_OBJECT_VISIT_H
#define GUARD_DEEMON_OPTIONAL_OBJECT_VISIT_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/object_util.h>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/type_slots.h>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif


#if defined(__cplusplus) && !defined(__INTELLISENSE__)
// Disabled in intellisense, so we don't pollute the type namespace with all
// these forward declarations. (Only start suggesting these if I actually include the correct file!)
DEE_DECL_END
namespace deemon{namespace detail{
#ifdef _MSC_VER
template<bool> struct static_if_impl{};
template<> struct static_if_impl<true>{void is_true(void);};
#define DEE_PRIVATE_STATIC_IF(x) __if_exists(::deemon::detail::static_if_impl<((x))>::is_true)
#endif
typedef int (&visit_yes)[1];
typedef int (&visit_no)[2];
#if 1
visit_yes should_visit(void const *);
#else
visit_yes should_visit(...);
#endif
}}
#define DEE_PRIVATE_REGISTER_NEVER_VISIT(T)\
struct T;namespace deemon{namespace detail{visit_no should_visit(T const&);}}
// List of known object types that don't have to be visited by the gc
// >> Used for some compile-time optimizations in c++
// This is a light-weight optimization that only does anything if
// the type of an object is explicitly stated by the member in tp_visit
// NOTE: Don't use this to argument that you shouldn't visit a
//       specific object just because you know its typing at compile-time.
//       These optimizations are only done because tp_visit is currently
//       only used for situations where the listed objects don't count.
//       If that changes and this list disappears, your code will be
//       left no longer working...
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeAnyStringObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeWideStringObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeUtf8StringObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeUtf16StringObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeUtf32StringObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(_DeeEmptyWideStringObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(_DeeEmptyUtf8StringObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(_DeeEmptyUtf16StringObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(_DeeEmptyUtf32StringObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(_DeeUtf8StringCharacterObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeWideStringIteratorObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeUtf8StringIteratorObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeUtf16StringIteratorObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeUtf32StringIteratorObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeFloatObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeDoubleObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeLDoubleObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeBoolObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeCFunctionObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeForeignFunctionObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeFSDirObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeFSDirIteratorObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeFSQueryObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeFSQueryIteratorObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeInt8Object)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeInt16Object)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeInt32Object)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeInt64Object)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeRangeInt8IteratorObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeRangeUInt8IteratorObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeRangeInt16IteratorObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeRangeUInt16IteratorObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeRangeInt32IteratorObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeRangeUInt32IteratorObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeRangeInt64IteratorObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeRangeUInt64IteratorObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeStructuredObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeRandomObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeStructObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeePointerObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeLValueObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeTimeObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeWeakrefObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeTokenObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeLexerObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeLexerIteratorObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeLocalVarObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeSemaphoreObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeMutexObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeFileWriterObject)
DEE_PRIVATE_REGISTER_NEVER_VISIT(DeeFileReaderObject)
#undef DEE_PRIVATE_REGISTER_NEVER_VISIT

#define DEE_PRIVATE_SHOULD_VISIT(ob)\
 (sizeof(::deemon::detail::should_visit(ob))==sizeof(int))
DEE_DECL_BEGIN
#ifdef DEE_PRIVATE_STATIC_IF
#define DEE_PRIVATE_IF_SHOULD_VISIT(ob) DEE_PRIVATE_STATIC_IF(DEE_PRIVATE_SHOULD_VISIT(ob))
#else
#define DEE_PRIVATE_IF_SHOULD_VISIT(ob) if(DEE_PRIVATE_SHOULD_VISIT(ob))
#endif
#endif /* ... */


#define DEE_VISIT_PROC(name,self_decl) \
 static void DEE_CALL name(DeeTypeObject *DEE_UNUSED(tp_self), self_decl, DeeVisitProc visit, void *closure)
#define DEE_VISIT_PROC_EX(name,tp_self_decl,self_decl) \
 static void DEE_CALL name(tp_self_decl, self_decl, DeeVisitProc visit, void *closure)

#define _DeeInternalObject_VISIT(ob)\
 DeeObject *_dee_next,*_dee_iter,*_dee_ob;\
 _dee_ob = (DeeObject *)Dee_REQUIRES_POINTER(ob);\
 DEE_ASSERTF(DeeObject_Check(_dee_ob),"Invalid object: " #ob);\
 while ((*visit)(_dee_ob,closure)) {\
  DeeType_SLOT_TYPE(tp_visit) _temp_visit;\
  _dee_next = (DeeObject *)Dee_TYPE(_dee_ob);\
  if (_dee_ob == _dee_next) break;\
  _dee_iter = _dee_next;\
  do {\
   if ((_temp_visit = DeeType_GET_SLOT(_dee_iter,tp_visit)) != NULL)\
    (*_temp_visit)((DeeTypeObject *)_dee_iter,_dee_ob,visit,closure);\
   _dee_iter = (DeeObject *)DeeType_BASE(_dee_iter);\
  } while (_dee_iter);\
  _dee_ob = _dee_next;\
 }
#define _DeeInternalObject_XVISIT(ob)\
 DeeObject *_dee_next,*_dee_iter,*_dee_ob;\
 if ((_dee_ob = (DeeObject *)Dee_REQUIRES_POINTER(ob)) != NULL){\
  DEE_ASSERTF(_dee_ob->__ob_refcnt&&_dee_ob->__ob_weakcnt&&_dee_ob->__ob_type,"Invalid object: " #ob);\
  while ((*visit)(_dee_ob,closure)) {\
   DeeType_SLOT_TYPE(tp_visit) _temp_visit;\
   _dee_next = (DeeObject *)Dee_TYPE(_dee_ob);\
   if (_dee_ob == _dee_next) break;\
   _dee_iter = _dee_next;\
   do {\
    if ((_temp_visit = DeeType_GET_SLOT(_dee_iter,tp_visit)) != NULL)\
     (*_temp_visit)((DeeTypeObject *)_dee_iter,_dee_ob,visit,closure);\
    _dee_iter = (DeeObject *)DeeType_BASE(_dee_iter);\
   } while (_dee_iter);\
   _dee_ob = _dee_next;\
  }\
 }
#define _DeeObject_VISIT  _DeeInternalObject_VISIT
#define _DeeObject_XVISIT _DeeInternalObject_XVISIT

#ifdef DEE_PRIVATE_IF_SHOULD_VISIT
#define Dee_VISIT(ob)  do{DEE_PRIVATE_IF_SHOULD_VISIT(ob){_DeeObject_VISIT(ob)}}while(0)
#define Dee_XVISIT(ob) do{DEE_PRIVATE_IF_SHOULD_VISIT(ob){_DeeObject_XVISIT(ob)}}while(0)
#else
#define Dee_VISIT(ob)  do{_DeeObject_VISIT(ob)}while(0)
#define Dee_XVISIT(ob) do{_DeeObject_XVISIT(ob)}while(0)
#endif

#ifdef __INTELLISENSE__
#undef Dee_VISIT
#undef Dee_XVISIT
#define Dee_VISIT  if(0);else Dee_REQUIRES_POINTER
#define Dee_XVISIT if(0);else Dee_REQUIRES_POINTER
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_OBJECT_VISIT_H */
