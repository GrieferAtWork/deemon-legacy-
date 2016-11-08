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
#ifndef GUARD_DEEMON_BUILTIN_FUNCTIONS_C
#define GUARD_DEEMON_BUILTIN_FUNCTIONS_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include DEE_INCLUDE_MEMORY_API()

#include <deemon/__bswap_intrin.inl>
#include <deemon/bool.h>
#include <deemon/class.h>
#include <deemon/compiler/code.h>
#include <deemon/deemonrun.h>
#include <deemon/dex.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/float.h>
#include <deemon/fs_api.h>
#include <deemon/gc.h>
#include <deemon/integer.h>
#include <deemon/iterator.h>
#include <deemon/list.h>
#include <deemon/math.h>
#include <deemon/mp/thread.h>
#include <deemon/none.h>
#include <deemon/marshal_data.h>
#include <deemon/optional/enum_attributes.h>
#include <deemon/optional/atomic_once.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_io.h>
#include <deemon/optional/std/string.h>
#include <deemon/runtime/builtin_functions.h>
#include <deemon/runtime/builtins.h>
#include <deemon/runtime/execute.h>
#include <deemon/runtime/extern.h>
#include <deemon/runtime_flags.h>
#include <deemon/sequence.h>
#include <deemon/signalhandler.h>
#include <deemon/string.h>
#include <deemon/structured.h>
#include <deemon/tuple.h>
#include <deemon/unicode/char_traits.inl>

DEE_DECL_BEGIN

// NOTE: Functions in here are documented in
//      'autodoc.c.inl', and available through '__builtin_dex("_docgen")'

// Include the names of the builtin functions in debug builds.
// That way, we can always and easily identify them in the debugger
// NOTE: The names of these functions will get added later by the _docgen dex
#ifdef DEE_DEBUG
#define DEE_PRIVATE_DEF_BUILTIN_FUNCTION(name)\
 DeeCFunctionObject DeeBuiltinFunction_##name = DeeCFunction_INIT(#name,NULL,&DeeBuiltinFunction_##name##_F);\
 DEE_A_RET_EXCEPT_REF DeeObject *DEE_CALL DeeBuiltinFunction_##name##_F(DEE_A_INOUT_OBJECT(DeeTupleObject) *args)
#else
#define DEE_PRIVATE_DEF_BUILTIN_FUNCTION(name)\
 DeeCFunctionObject DeeBuiltinFunction_##name = DeeCFunction_INIT(NULL,NULL,&DeeBuiltinFunction_##name##_F);\
 DEE_A_RET_EXCEPT_REF DeeObject *DEE_CALL DeeBuiltinFunction_##name##_F(DEE_A_INOUT_OBJECT(DeeTupleObject) *args)
#endif

DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__bool__) {
 DeeObject *ob; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__bool__",&ob) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeObject_Bool(ob)) < 0) return NULL;
 DeeReturn_Bool(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__repr__) {
 DeeObject *ob;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__repr__",&ob) != 0) return NULL;
 return DeeObject_Repr(ob);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__type__) {
 DeeObject *ob;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__type__",&ob) != 0) return NULL;
 DeeReturn_NEWREF(Dee_TYPE(ob));
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__copy__) {
 DeeObject *ob;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__copy__",&ob) != 0) return NULL;
 DeeReturn_XNEWREF(DeeObject_Copy(ob));
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__move__) {
 DeeObject *ob;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__move__",&ob) != 0) return NULL;
 DeeReturn_XNEWREF(DeeObject_Move(ob));
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__copyassign__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__copyassign__",&a,&b) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_CopyAssign(a,b) != 0) return NULL;
 DeeReturn_NEWREF(a);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__moveassign__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__moveassign__",&a,&b) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_MoveAssign(a,b) != 0) return NULL;
 DeeReturn_NEWREF(a);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__not__) {
 DeeObject *a;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__not__",&a) != 0) return NULL;
 return DeeObject_Not(a);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__neg__) {
 DeeObject *a;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__neg__",&a) != 0) return NULL;
 return DeeObject_Neg(a);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__pos__) {
 DeeObject *a;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__pos__",&a) != 0) return NULL;
 return DeeObject_Pos(a);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__inv__) {
 DeeObject *a;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__inv__",&a) != 0) return NULL;
 return DeeObject_Inv(a);
}
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__ref__) {
 DeeObject *a;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__ref__",&a) != 0) return NULL;
 return DeeObject_Ref(a);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__deref__) {
 DeeObject *a;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__deref__",&a) != 0) return NULL;
 return DeeObject_Deref(a);
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__inc__) {
 DeeObject *a;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__inc__",&a) != 0) return NULL;
 return DeeObject_Inc(a);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__dec__) {
 DeeObject *a;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__dec__",&a) != 0) return NULL;
 return DeeObject_Dec(a);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__incpost__) {
 DeeObject *a;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__incpost__",&a) != 0) return NULL;
 return DeeObject_IncPost(a);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__decpost__) {
 DeeObject *a;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__decpost__",&a) != 0) return NULL;
 return DeeObject_DecPost(a);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__lo__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__lo__",&a,&b) != 0) return NULL;
 return DeeObject_CompareLoObject(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__le__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__le__",&a,&b) != 0) return NULL;
 return DeeObject_CompareLeObject(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__eq__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__eq__",&a,&b) != 0) return NULL;
 return DeeObject_CompareEqObject(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__ne__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__ne__",&a,&b) != 0) return NULL;
 return DeeObject_CompareNeObject(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__gr__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__gr__",&a,&b) != 0) return NULL;
 return DeeObject_CompareGrObject(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__ge__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__ge__",&a,&b) != 0) return NULL;
 return DeeObject_CompareGeObject(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__add__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__add__",&a,&b) != 0) return NULL;
 return DeeObject_Add(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__iadd__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__iadd__",&a,&b) != 0) return NULL;
 return DeeObject_InplaceAdd(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__sub__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__sub__",&a,&b) != 0) return NULL;
 return DeeObject_Sub(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__isub__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__iadd__",&a,&b) != 0) return NULL;
 return DeeObject_InplaceAdd(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__mul__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__mul__",&a,&b) != 0) return NULL;
 return DeeObject_Mul(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__imul__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__imul__",&a,&b) != 0) return NULL;
 return DeeObject_InplaceMul(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__div__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__div__",&a,&b) != 0) return NULL;
 return DeeObject_Div(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__idiv__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__idiv__",&a,&b) != 0) return NULL;
 return DeeObject_InplaceDiv(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__mod__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__mod__",&a,&b) != 0) return NULL;
 return DeeObject_Mod(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__imod__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__imod__",&a,&b) != 0) return NULL;
 return DeeObject_InplaceMod(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__shl__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__shl__",&a,&b) != 0) return NULL;
 return DeeObject_Shl(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__ishl__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__ishl__",&a,&b) != 0) return NULL;
 return DeeObject_InplaceShl(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__shr__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__shr__",&a,&b) != 0) return NULL;
 return DeeObject_Shr(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__ishr__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__ishr__",&a,&b) != 0) return NULL;
 return DeeObject_InplaceShr(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__and__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__and__",&a,&b) != 0) return NULL;
 return DeeObject_And(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__iand__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__iand__",&a,&b) != 0) return NULL;
 return DeeObject_InplaceAnd(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__or__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__or__",&a,&b) != 0) return NULL;
 return DeeObject_Or(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__ior__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__ior__",&a,&b) != 0) return NULL;
 return DeeObject_InplaceOr(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__xor__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__xor__",&a,&b) != 0) return NULL;
 return DeeObject_Xor(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__ixor__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__ixor__",&a,&b) != 0) return NULL;
 return DeeObject_InplaceXor(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__pow__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__pow__",&a,&b) != 0) return NULL;
 return DeeObject_Pow(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__ipow__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__ipow__",&a,&b) != 0) return NULL;
 return DeeObject_InplacePow(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__call__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__call__",&a,&b) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(b,&DeeTuple_Type) != 0) return NULL;
 return DeeObject_Call(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__callv__) {
 DeeObject *vargs,*result;
 if DEE_UNLIKELY(DeeTuple_SIZE(args) < 1) {
  DeeError_TypeError_ArgCountExpectedNamedMin("__callv__",1,DeeTuple_SIZE(args));
  return NULL;
 }
 if DEE_UNLIKELY((vargs = DeeTuple_NewFromVector(DeeTuple_SIZE(args)-1,DeeTuple_ELEM(args)+1)) == NULL) return NULL;
 result = DeeObject_Call(DeeTuple_GET(args,0),vargs);
 Dee_DECREF(vargs);
 return result;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__size__) {
 DeeObject *a;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__size__",&a) != 0) return NULL;
 return DeeObject_SizeObject(a);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__getitem__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__getitem__",&a,&b) != 0) return NULL;
 return DeeObject_GetItem(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__setitem__) {
 DeeObject *a,*b,*c;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ooo:__setitem__",&a,&b,&c) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_SetItem(a,b,c) != 0) return NULL;
 DeeReturn_NEWREF(c);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__delitem__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__delitem__",&a,&b) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_DelItem(a,b) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__getrange__) {
 DeeObject *a,*b,*c;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ooo:__getrange__",&a,&b,&c) != 0) return NULL;
 return DeeObject_GetRangeItem(a,b,c);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__setrange__) {
 DeeObject *a,*b,*c,*d;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oooo:__setrange__",&a,&b,&c,&d) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_SetRangeItem(a,b,c,d) != 0) return NULL;
 DeeReturn_NEWREF(d);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__delrange__) {
 DeeObject *a,*b,*c;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ooo:__delrange__",&a,&b,&c) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_DelRangeItem(a,b,c) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__is__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__is__",&a,&b) != 0) return NULL;
 DeeReturn_Bool(DeeObject_InstanceOf(a,(DeeTypeObject *)b));
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__contains__) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__contains__",&a,&b) != 0) return NULL;
 return DeeObject_ContainsObject(a,b);
}

DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__getattr__) {
 DeeObject *ob,*attr;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__getattr__",&ob,&attr) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(attr,&DeeString_Type) != 0) return NULL;
 return DeeObject_GetAttr(ob,attr);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__hasattr__) {
 DeeObject *ob,*attr; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__hasattr__",&ob,&attr) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(attr,&DeeString_Type) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeObject_HasAttr(ob,attr)) < 0) return NULL;
 DeeReturn_Bool(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__delattr__) {
 DeeObject *ob,*attr;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__delattr__",&ob,&attr) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(attr,&DeeString_Type) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_DelAttr(ob,attr) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__setattr__) {
 DeeObject *ob,*attr,*v;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ooo:__setattr__",&ob,&attr,&v) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(attr,&DeeString_Type) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_SetAttr(ob,attr,v) != 0) return NULL;
 DeeReturn_NEWREF(v);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__iterself__) {
 DeeObject *ob;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__iterself__",&ob) != 0) return NULL;
 return DeeObject_IterSelf(ob);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__iternext__) {
 DeeObject *ob,*result; int error;
 if (DeeTuple_Unpack(args,"o:__iternext__",&ob) != 0) return NULL;
 if DEE_UNLIKELY((error = DeeObject_IterNextEx(ob,&result)) != 0) {
  if DEE_UNLIKELY(error > 0) DeeError_Throw(DeeErrorInstance_IterDone);
  return NULL;
 }
 return result;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__hash__) {
 DeeObject *ob; Dee_hash_t result,start = DEE_HASH_START;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|" DEE_TUPLE_UNPACKFMT_UINT(DEE_CONFIG_SIZEOF_DEE_HASH_T) ":__hash__",&ob) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_HashEx(ob,start,&result) != 0) return NULL;
 return DeeObject_New(Dee_hash_t,result);
}

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__read__) {
 DeeObject *file,*result,*p,*s = NULL; void *_p; Dee_size_t _s,rs;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo|o:__read__",&file,&p,&s) != 0) return NULL;
 if (s) {
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(p,&_p) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,s,&_s) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_IORead(file,_p,_s,&rs) != 0) return NULL;
  return DeeObject_New(Dee_size_t,rs);
 }
 if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,p,&_s) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeString_NewSized(_s/sizeof(char))) == NULL) return NULL;
 if DEE_UNLIKELY(DeeObject_IORead(file,DeeString_STR(result),_s,&rs) != 0) {err_r: Dee_DECREF(result); return NULL; }
 if DEE_UNLIKELY(_s > rs && DeeString_Resize(&result,rs) != 0) goto err_r;
 return result;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__write__) {
 DeeObject *file,*p,*s = NULL; void *_p; Dee_size_t _s,ws;
 if (DeeTuple_Unpack(args,"oo|o:__write__",&file,&p,&s) != 0) return NULL;
 if (s) {
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(p,&_p) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,s,&_s) != 0) return NULL;
 } else {
  if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(p,&DeeString_Type) != 0) return NULL;
  _p = DeeString_STR(p),_s = DeeString_SIZE(p)*sizeof(char);
 }
 if (DeeObject_IOWrite(file,_p,_s,&ws) != 0) return NULL;
 return DeeObject_New(Dee_size_t,ws);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__readp__) {
 DeeObject *file; void *p; Dee_size_t s,rs;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"opIu:__readp__",&file,&p,&s) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_IORead(file,p,s,&rs) != 0) return NULL;
 return DeeObject_New(Dee_size_t,rs);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__writep__) {
 DeeObject *file; void *p; Dee_size_t s,ws;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"opIu:__writep__",&file,&p,&s) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_IOWrite(file,p,s,&ws) != 0) return NULL;
 return DeeObject_New(Dee_size_t,ws);
}
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__readnp__) {
 DeeObject *file,*result; Dee_size_t s,rs;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oIu:__readnp__",&file,&s) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeString_NewSized(s/sizeof(char))) == NULL) return NULL;
 if DEE_UNLIKELY(DeeObject_IORead(file,DeeString_STR(result),s,&rs) != 0) {err_r: Dee_DECREF(result); return NULL; }
 if DEE_UNLIKELY(s > rs && DeeString_Resize(&result,rs) != 0) goto err_r;
 return result;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__writenp__) {
 DeeObject *file,*data; Dee_size_t ws;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__writenp__",&file,&data) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(data,&DeeString_Type) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_IOWrite(file,DeeString_STR(data),DeeString_SIZE(data)*sizeof(char),&ws) != 0) return NULL;
 return DeeObject_New(Dee_size_t,ws);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__seek__) {
 DeeObject *file; Dee_int64_t off; int whence; Dee_uint64_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oI64dd:__seek__",&file,&off,&whence) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_IOSeek(file,off,whence,&result) != 0) return NULL;
 return DeeObject_New(Dee_uint64_t,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__flush__) {
 DeeObject *file;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__flush__",&file) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_IOFlush(file) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__trunc__) {
 DeeObject *file;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__trunc__",&file) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_IOTrunc(file) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__close__) {
 DeeObject *file;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__close__",&file) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_IOClose(file) != 0) return NULL;
 DeeReturn_None;
}


DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__dual_posoradd__) {
 DeeObject *a,*b = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:",&a,&b) != 0) return NULL;
 return b ? DeeObject_Add(a,b) : DeeObject_Pos(a);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__dual_negorsub__) {
 DeeObject *a,*b = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:__dual_negorsub__",&a,&b) != 0) return NULL;
 return b ? DeeObject_Sub(a,b) : DeeObject_Neg(a);
}
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__dual_dereformul__) {
 DeeObject *a,*b = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:__dual_dereformul__",&a,&b) != 0) return NULL;
 return b ? DeeObject_Mul(a,b) : DeeObject_Deref(a);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__dual_reforand__) {
 DeeObject *a,*b = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:__dual_reforand__",&a,&b) != 0) return NULL;
 return b ? DeeObject_And(a,b) : DeeObject_Ref(a);
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__dual_getitemorgetrange__) {
 DeeObject *a,*b,*c = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo|o:__dual_getitemorgetrange__",&a,&b,&c) != 0) return NULL;
 return c ? DeeObject_GetRangeItem(a,b,c) : DeeObject_GetItem(a,b);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__dual_delitemordelrange__) {
 DeeObject *a,*b,*c = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo|o:__dual_getitemorgetrange__",&a,&b,&c) != 0) return NULL;
 if DEE_UNLIKELY((c ? DeeObject_DelRangeItem(a,b,c) : DeeObject_DelItem(a,b)) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__dual_setitemorsetrange__) {
 DeeObject *a,*b,*c,*d = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ooo|o:__dual_setitemorsetrange__",&a,&b,&c,&d) != 0) return NULL;
 if DEE_UNLIKELY((d ? DeeObject_SetRangeItem(a,b,c,d) : DeeObject_SetItem(a,b,c)) != 0) return NULL;
 DeeReturn_None;
}



DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_dex) {
 DeeObject *name;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_dex",&name) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(name,&DeeString_Type) != 0) return NULL;
 return DeeDex_Open(DeeString_STR(name));
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_object) {
 Dee_uint16_t id; DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I16u:__builtin_object",&id) != 0) return NULL;
 if DEE_LIKELY((result = DeeBuiltin_Get(id)) != NULL) Dee_INCREF(result);
 else {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Unknown/Invalid builtin object object: %I16u",
                      id);
 }
 return result;
}
#if DEE_CONFIG_RUNTIME_HAVE_EXTERN
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_extern) {
 DeeObject *filename,*object_name,*object_type = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo|o:__builtin_extern",&filename,&object_name,&object_type) != 0) return NULL;
 return object_type
  ? DeeSharedLib_LoadExtern(filename,object_name,(DeeTypeObject *)object_type)
  : DeeSharedLib_LoadAutoExtern(filename,(DeeTypeObject *)object_name);
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_ff_closure) {
 DeeTypeObject *ff_type; DeeObject *callback;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__builtin_ff_closure",&ff_type,&callback) != 0) return NULL;
 if DEE_UNLIKELY((ff_type = (DeeTypeObject *)DeeObject_GetInstance((
  DeeObject *)ff_type,&DeeForeignFunctionType_Type)) == NULL) return NULL;
 return DeeForeignFunctionClosure_New(ff_type,callback);
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_pointer_add) {
 DeeTypeObject *temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_pointer_add",&temp) != 0) return NULL;
 Dee_XINCREF(temp = DeeType_Pointer(temp));
 return (DeeObject *)temp;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_pointer_rem) {
 DeeTypeObject *temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_pointer_rem",&temp) != 0) return NULL;
 Dee_XINCREF(temp = DeeType_WithoutPointer(temp));
 return (DeeObject *)temp;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_lvalue_add) {
 DeeTypeObject *temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_lvalue_add",&temp) != 0) return NULL;
 Dee_XINCREF(temp = DeeType_LValue(temp));
 return (DeeObject *)temp;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_lvalue_rem) {
 DeeTypeObject *temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_lvalue_rem",&temp) != 0) return NULL;
 Dee_XINCREF(temp = DeeType_WithoutLValue(temp));
 return (DeeObject *)temp;
}
#endif
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_assertion_failed) {
 DeeThreadObject *thread_self; char const *file; Dee_int32_t line;
 DeeObject *expr = Dee_None,*msg = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|oo:__builtin_assertion_failed",&expr,&msg) != 0) return NULL;
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL) return NULL;
 if (thread_self->t_frame_last) { // Should always be the case!
  file = _DeeStackFrame_File(thread_self->t_frame_last)
   ? DeeString_STR(_DeeStackFrame_File(thread_self->t_frame_last)) : NULL;
  line = _DeeStackFrame_Line(thread_self->t_frame_last);
 } else {
  file = NULL;
  line = 0;
 }
 if (DeeNone_Check(msg)) {
  DeeError_SetStringf(&DeeErrorType_AssertionError,
                      "%s(%I32d) : Assertion failed : %s:%k",
                      file,line+1,DeeType_NAME(Dee_TYPE(expr)),expr);
 } else {
  DeeError_SetStringf(&DeeErrorType_AssertionError,
                      "%s(%I32d) : Assertion failed : %s:%k : %k",
                      file,line+1,DeeType_NAME(Dee_TYPE(expr)),expr,msg);
 }
 return NULL;
}









//////////////////////////////////////////////////////////////////////////
// Exceptions related
#if DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_last_exception) {
 DeeObject *thread = NULL; DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:__builtin_last_exception",&thread) != 0) return NULL;
#ifndef DEE_WITHOUT_THREADS
 if DEE_LIKELY(!thread) {
#endif
  if DEE_UNLIKELY(!DeeError_GetHandling(&result,NULL)) {
#ifndef DEE_WITHOUT_THREADS
no_exc:
#endif
   DeeError_Throw((DeeObject *)&_DeeError_RuntimeError_NoActiveException);
   return NULL;
  }
#ifndef DEE_WITHOUT_THREADS
 } else {
  int temp;
  if DEE_UNLIKELY((thread = DeeObject_GetInstance(thread,&DeeThread_Type)) == NULL) return NULL;
  if DEE_UNLIKELY((temp = DeeThread_GetError(thread,&result,NULL)) < 0) return NULL;
  if DEE_UNLIKELY(!temp) goto no_exc;
 }
#endif
 DeeReturn_NEWREF(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_last_traceback) {
 DeeObject *thread = NULL; DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:__builtin_last_traceback",&thread) != 0) return NULL;
#ifndef DEE_WITHOUT_THREADS
 if DEE_LIKELY(!thread) {
#endif
  if DEE_UNLIKELY(!DeeError_GetHandling(NULL,&result)) {
#ifndef DEE_WITHOUT_THREADS
no_exc:
#endif
   DeeError_Throw((DeeObject *)&_DeeError_RuntimeError_NoActiveException);
   return NULL;
  }
#ifndef DEE_WITHOUT_THREADS
 } else {
  int temp;
  if DEE_UNLIKELY((thread = DeeObject_GetInstance(thread,&DeeThread_Type)) == NULL) return NULL;
  if DEE_UNLIKELY((temp = DeeThread_GetError(thread,NULL,&result)) < 0) return NULL;
  if DEE_UNLIKELY(!temp) goto no_exc;
 }
#endif
 DeeReturn_NEWREF(result);
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS */









//////////////////////////////////////////////////////////////////////////
// Misc utility functions
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_min) {
 return DeeSequence_Min(DeeTuple_SIZE(args) == 1 ? DeeTuple_GET(args,0) : args);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_max) {
 return DeeSequence_Max(DeeTuple_SIZE(args) == 1 ? DeeTuple_GET(args,0) : args);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_sum) {
 return DeeSequence_Sum(DeeTuple_SIZE(args) == 1 ? DeeTuple_GET(args,0) : args);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_avg) {
 double temp,total; Dee_size_t count;
 DeeObject **begin,**end;
 // TODO: Sequence overload
 if DEE_UNLIKELY((count = DeeTuple_SIZE(args)) == 0) {
  DeeError_SET_STRING(&DeeErrorType_ValueError,"__builtin_avg(): No arguments given");
  return NULL;
 }
 total = 0.0;
 end = (begin = DeeTuple_ELEM(args))+count;
 while (begin != end) {
  if DEE_UNLIKELY(DeeObject_Cast(double,*begin,&temp) != 0) return NULL;
  total += temp;
  ++begin;
 }
 return DeeDouble_New(total/(double)count);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_all) {
 int result;
 if DEE_UNLIKELY((result = DeeSequence_All(
  DeeTuple_SIZE(args) == 1 ? DeeTuple_GET(args,0) : args)) < 0) return NULL;
 DeeReturn_Bool(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_any) {
 int result;
 if DEE_UNLIKELY((result = DeeSequence_Any(
  DeeTuple_SIZE(args) == 1 ? DeeTuple_GET(args,0) : args)) < 0) return NULL;
 DeeReturn_Bool(result);
}

DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_ord) {
 DeeObject *ob;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_ord",&ob) != 0) return NULL;
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (DeeUtf8String_Check(ob)) {
  if DEE_UNLIKELY(DeeUtf8String_SIZE(ob) != 1) goto inv_length;
  return DeeObject_New(Dee_Utf8Char,*DeeUtf8String_STR(ob));
 }
#endif
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (DeeWideString_Check(ob)) {
  if DEE_UNLIKELY(DeeWideString_SIZE(ob) != 1) goto inv_length;
  return DeeObject_New(Dee_WideChar,*DeeWideString_STR(ob));
 }
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (DeeUtf16String_Check(ob)) {
  if DEE_UNLIKELY(DeeUtf16String_SIZE(ob) != 1) goto inv_length;
  return DeeObject_New(Dee_Utf16Char,*DeeUtf16String_STR(ob));
 }
#endif
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (DeeUtf32String_Check(ob)) {
  if DEE_UNLIKELY(DeeUtf32String_SIZE(ob) != 1) goto inv_length;
  return DeeObject_New(Dee_Utf32Char,*DeeUtf32String_STR(ob));
 }
#endif
 DeeError_TypeError_UnexpectedType(ob,&DeeString_Type);
 return NULL;
inv_length:
 DeeError_SET_STRING(&DeeErrorType_ValueError,"Expected a one-character string");
 return NULL;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_abs) {
 DeeObject *ob,*temp; DeeTypeObject const *tp_ob; int itemp;
 struct DeeTypeMarshal const *marshal;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_abs",&ob) != 0) return NULL;
 tp_ob = Dee_TYPE(ob);
 marshal = DeeType_GET_SLOT(tp_ob,tp_marshal);
 if DEE_LIKELY(marshal && DeeUUID_CHECK_INTERNAL(&marshal->tp_uuid)) {
  switch (DeeUUID_GET_INTERNAL(&marshal->tp_uuid)) {
   case DEE_MARSHALTYPEID_int32:
   case DEE_MARSHALTYPEID_atomic_int32:
    if (DeeInt32_VALUE(ob) >= 0) goto ret_ob;
    return DeeInt32_New(-DeeInt32_VALUE(ob));
   case DEE_MARSHALTYPEID_int8:
   case DEE_MARSHALTYPEID_atomic_int8:
    if (DeeInt8_VALUE(ob) >= 0) goto ret_ob;
    return DeeInt8_New(-DeeInt8_VALUE(ob));
   case DEE_MARSHALTYPEID_int64:
   case DEE_MARSHALTYPEID_atomic_int64:
    if (DeeInt64_VALUE(ob) >= 0) goto ret_ob;
    return DeeInt64_New(-DeeInt64_VALUE(ob));
   case DEE_MARSHALTYPEID_int16:
   case DEE_MARSHALTYPEID_atomic_int16:
    if (DeeInt16_VALUE(ob) >= 0) goto ret_ob;
    return DeeInt16_New(-DeeInt16_VALUE(ob));
   case DEE_MARSHALTYPEID_uint8:
   case DEE_MARSHALTYPEID_uint16:
   case DEE_MARSHALTYPEID_uint32:
   case DEE_MARSHALTYPEID_uint64:
   case DEE_MARSHALTYPEID_atomic_uint8:
   case DEE_MARSHALTYPEID_atomic_uint16: 
   case DEE_MARSHALTYPEID_atomic_uint32: 
   case DEE_MARSHALTYPEID_atomic_uint64: 
ret_ob: DeeReturn_NEWREF(ob);
   case DEE_MARSHALTYPEID_float4:
   case DEE_MARSHALTYPEID_float8:
   case DEE_MARSHALTYPEID_float12:
   case DEE_MARSHALTYPEID_float16:
    if (tp_ob == (DeeTypeObject *)&DeeDouble_Type) {
     if (DeeDouble_VALUE(ob) >= 0.0)  goto ret_ob;
     return DeeDouble_New(-DeeDouble_VALUE(ob));
    }
    if (tp_ob == (DeeTypeObject *)&DeeFloat_Type) {
     if (DeeFloat_VALUE(ob) >= 0.0f) goto ret_ob;
     return DeeFloat_New(-DeeFloat_VALUE(ob));
    }
    DEE_ASSERT(tp_ob == (DeeTypeObject *)&DeeLDouble_Type);
    if (DeeLDouble_VALUE(ob) >= 0.0L) goto ret_ob;
    return DeeLDouble_New(-DeeLDouble_VALUE(ob));
    break;
   case DEE_MARSHALID_NONE: DeeReturn_None;
   default: break;
  }
 }
 // Fallback...
 if DEE_UNLIKELY((temp = DeeObject_New(Dee_rt_int,0)) == NULL) return NULL;
 itemp = DeeObject_CompareLo(ob,temp);
 Dee_DECREF(temp);
 if DEE_UNLIKELY(itemp < 0) return NULL;
 if DEE_UNLIKELY(!itemp) goto ret_ob;
 return DeeObject_Neg(ob);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_ceil) {
 double v; Dee_rt_int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"D:__builtin_ceil",&v) != 0) return NULL;
 if ((result = (Dee_rt_int)v) < v) ++result;
 return DeeObject_New(Dee_rt_int,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_floor) {
 double v; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"D:__builtin_floor",&v) != 0) return NULL;
 if ((result = (int)v) > v) --result;
 return DeeObject_New(int,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_round) {
 double v; Dee_rt_int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"D:__builtin_round",&v) != 0) return NULL;
 if ((result = (Dee_rt_int)v) < v) {
  if ((v-result) >= 0.5) ++result;
 } else {
  if ((result-v) >= 0.5) --result;
 }
 return DeeObject_New(Dee_rt_int,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_range) {
 DeeTypeObject *tp_begin; DeeObject *begin,*end = NULL,*step = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|oo:__builtin_range",&begin,&end,&step) != 0) return NULL;
#if DEE_VERSION_API < 104
 if DEE_UNLIKELY(DeeNone_Check(step)) step = NULL; // Backwards compatibility until v104
#endif
 tp_begin = Dee_TYPE(begin);
#define DO_INT(n)\
do{\
 int##n##_t i_end,i_step;\
 if (end) {\
  if DEE_UNLIKELY(DeeObject_Cast(int##n##_t,end,&i_end) != 0) return NULL;\
  if (step) {\
   if DEE_UNLIKELY(DeeObject_Cast(int##n##_t,step,&i_step) != 0) return NULL;\
   if DEE_UNLIKELY(!i_step) return DeeError_Throw((DeeObject *)&_DeeError_ValueError_RangeZeroStep),NULL;\
  } else i_step = 1;\
  return DeeRangeInt##n##Iterator_New(DeeInt##n##_VALUE(begin),i_end,i_step);\
 } else {\
  return DeeRangeInt##n##Iterator_New((uint##n##_t)0,DeeInt##n##_VALUE(begin),(uint##n##_t)1);\
 }\
}while(0)
#define DO_UINT(n)\
do{\
 if (end) {\
  uint##n##_t i_end,i_step;\
  if DEE_UNLIKELY(DeeObject_Cast(uint##n##_t,end,&i_end) != 0) return NULL;\
  if (step) {\
   if DEE_UNLIKELY(DeeObject_Cast(uint##n##_t,step,&i_step) != 0) return NULL;\
   if (!i_step) return DeeError_Throw((DeeObject *)&_DeeError_ValueError_RangeZeroStep),NULL;\
  } else i_step = 1;\
  return DeeRangeUInt##n##Iterator_New(DeeUInt##n##_VALUE(begin),i_end,i_step);\
 } else {\
  return DeeRangeUInt##n##Iterator_New((uint##n##_t)0,DeeUInt##n##_VALUE(begin),(uint##n##_t)1);\
 }\
}while(0)

 if (DeeType_HAS_SLOT(tp_begin,tp_marshal) &&
     DeeUUID_CHECK_INTERNAL(&DeeType_GET_SLOT(tp_begin,tp_marshal)->tp_uuid)) {
  switch (DeeUUID_GET_INTERNAL(&DeeType_GET_SLOT(tp_begin,tp_marshal)->tp_uuid)) {
   case DEE_MARSHALTYPEID_int8:   case DEE_MARSHALTYPEID_atomic_int8:   DO_INT(8);
   case DEE_MARSHALTYPEID_int16:  case DEE_MARSHALTYPEID_atomic_int16:  DO_INT(16);
   case DEE_MARSHALTYPEID_int32:  case DEE_MARSHALTYPEID_atomic_int32:  DO_INT(32);
   case DEE_MARSHALTYPEID_int64:  case DEE_MARSHALTYPEID_atomic_int64:  DO_INT(64);
   case DEE_MARSHALTYPEID_uint8:  case DEE_MARSHALTYPEID_atomic_uint8:  DO_UINT(8);
   case DEE_MARSHALTYPEID_uint16: case DEE_MARSHALTYPEID_atomic_uint16: DO_UINT(16);
   case DEE_MARSHALTYPEID_uint32: case DEE_MARSHALTYPEID_atomic_uint32: DO_UINT(32);
   case DEE_MARSHALTYPEID_uint64: case DEE_MARSHALTYPEID_atomic_uint64: DO_UINT(64);
   default: break;
  }
 }
#undef DO_UINT
#undef DO_INT
 if (!end) {
  DEE_ASSERT(!step);
  // We try to use a default-constructed 'type begin' as actual begin if no end is given
  // NOTE: don't get confused by the variables I use here (don't wanna allocate more)
  if DEE_UNLIKELY((end = DeeType_NewInstanceDefault(tp_begin)) == NULL) return NULL;
  if (DeeStructuredType_Check(tp_begin)) // Manually fill a structured object with zeros
   memset(DeeStructured_DATA(end),0,DeeType_GET_SLOT(tp_begin,tp_p_instance_size));
  step = DeeGenericRangeIterator_New(end,begin,NULL);
  Dee_DECREF(end);
  return step;
 } else {
  return DeeGenericRangeIterator_New(begin,end,step);
 }
}

#if !DEE_DEVEL_BUILD
static Dee_uintptr_t _dee_get_random_id_mask(void) {
 struct DeeRandomNumberGenerator rng; Dee_uintptr_t result;
 DeeRandomNumberGenerator_Randomize(&rng);
 DeeRandomNumberGenerator_NextData(&rng,&result,sizeof(result));
 return result;
}
#endif /* !DEE_DEVEL_BUILD */
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_id) {
 DeeObject *ob;
#if !DEE_DEVEL_BUILD
 static Dee_uintptr_t random_seed;
 Dee_uintptr_t result,i;
 DEE_ATOMIC_ONCE({ random_seed = _dee_get_random_id_mask(); });
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_id",&ob) != 0) return NULL;
 result = (Dee_uintptr_t)((void *)ob) ^ random_seed;
 // Make this just a tiny bit more complicated (hehehe...)
 for (i = 0; i < ((result&0xF)+(result%0xF)); ++i) {
  if ((result&(0x1 << i)) != 0) result ^= random_seed;
 }
 return DeeObject_New(Dee_uintptr_t,result);
#else /* !DEE_DEVEL_BUILD */
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_id",&ob) != 0) return NULL;
 // To ease debugging, still have this be the pointer in developer builds
 return DeeObject_New(Dee_uintptr_t,(Dee_uintptr_t)((void *)ob));
#endif /* DEE_DEVEL_BUILD */
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_argv) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_argv") != 0) return NULL;
 if DEE_UNLIKELY((result = Dee_GetArgv()) == NULL) result = DeeList_New();
 return result;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_swap) {
 DeeObject *a,*b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__builtin_swap",&a,&b) != 0) return NULL;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(b,Dee_TYPE(a)) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_Swap(a,b) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_sqr) {
 DeeObject *x;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_sqr",&x) != 0) return NULL;
 return DeeObject_Mul(x,x);
}

static DEE_A_RET_EXCEPT(-1) int DEE_CALL _dee_enumattr_callback(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_Z char const *name, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN_OPT DeeTypeObject const *attr_type, DeeObject *callback) {
 DeeObject *callback_return;
 callback_return = DeeObject_Callf(callback,"oosI32uo",tp_self,self,name,flags,
                                   attr_type ? (DeeObject *)attr_type : Dee_None);
 if DEE_UNLIKELY(!callback_return) return -1;
 Dee_DECREF(callback_return);
 return 0;
}

DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_enumattr) {
 DeeObject *self,*callback;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__builtin_enumattr",&self,&callback) != 0) return NULL;
 if DEE_UNLIKELY(DeeObject_EnumAttr(self,(DeeEnumAttrFunc)&_dee_enumattr_callback,callback) != 0) return NULL;
 DeeReturn_None;
}

DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_enumerate) {
 DeeObject *result,*start_index,*iterable = NULL; Dee_size_t used_start_index;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:__builtin_enumerate",&start_index,&iterable) != 0) return NULL;
 if (iterable) {
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,start_index,&used_start_index) != 0) return NULL;
  iterable = DeeObject_IterSelf(iterable);
 } else {
  iterable = DeeObject_IterSelf(start_index);
  used_start_index = 0;
 }
 if DEE_UNLIKELY(!iterable) return NULL;
 result = DeeEnumIterator_NewEx(used_start_index,iterable);
 Dee_DECREF(iterable);
 return result;
}

DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_find) {
 DeeObject *seq,*ob,*pred = Dee_None; Dee_ssize_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo|o:__builtin_seq_find",&seq,&ob,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = ((DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeSequence_Find(seq,ob)
  : DeeSequence_FindPred(seq,ob,pred)
  )) == (Dee_ssize_t)-2) return NULL;
 return DeeObject_New(Dee_ssize_t,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_rfind) {
 DeeObject *seq,*ob,*pred = Dee_None; Dee_ssize_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo|o:__builtin_seq_rfind",&seq,&ob,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = ((DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeSequence_RFind(seq,ob)
  : DeeSequence_RFindPred(seq,ob,pred)
  )) == (Dee_ssize_t)-2) return NULL;
 return DeeObject_New(Dee_ssize_t,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_index) {
 DeeObject *seq,*ob,*pred = Dee_None; Dee_size_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo|o:__builtin_seq_index",&seq,&ob,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = ((DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeSequence_Index(seq,ob)
  : DeeSequence_IndexPred(seq,ob,pred)
  )) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_rindex) {
 DeeObject *seq,*elem,*pred = Dee_None; Dee_size_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo|o:__builtin_seq_rindex",&seq,&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = ((DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeSequence_RIndex(seq,elem)
  : DeeSequence_RIndexPred(seq,elem,pred)
  )) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_contains) {
 DeeObject *seq,*elem,*pred = Dee_None; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo|o:__builtin_seq_contains",&seq,&elem,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = (DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeSequence_Contains(seq,elem)
  : DeeSequence_ContainsPred(seq,elem,pred)
  ) < 0) return NULL;
 DeeReturn_Bool(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_count) {
 DeeObject *seq,*ob,*pred = Dee_None; Dee_size_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo|o:__builtin_seq_count",&seq,&ob,&pred) != 0) return NULL;
 if DEE_UNLIKELY((result = ((DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___eq__)
  ? DeeSequence_Count(seq,ob)
  : DeeSequence_CountPred(seq,ob,pred)
  )) == (Dee_size_t)-1) return NULL;
 return DeeObject_New(Dee_size_t,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_locate) {
 DeeObject *seq,*pred;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__builtin_seq_locate",&seq,&pred) != 0) return NULL;
 return DeeSequence_Locate(seq,pred);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_rlocate) {
 DeeObject *seq,*pred;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__builtin_seq_rlocate",&seq,&pred) != 0) return NULL;
 return DeeSequence_RLocate(seq,pred);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_locate_all) {
 DeeObject *seq,*pred;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__builtin_seq_locate_all",&seq,&pred) != 0) return NULL;
 return DeeSequence_LocateAll(seq,pred);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_transform) {
 DeeObject *seq,*pred;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__builtin_seq_transform",&seq,&pred) != 0) return NULL;
 return DeeSequence_Transform(seq,pred);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_empty) {
 DeeObject *seq; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_seq_empty",&seq) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeSequence_Empty(seq)) < 0) return NULL;
 DeeReturn_Bool(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_non_empty) {
 DeeObject *seq; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_seq_non_empty",&seq) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeSequence_NonEmpty(seq)) < 0) return NULL;
 DeeReturn_Bool(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_front) {
 DeeObject *seq;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_seq_front",&seq) != 0) return NULL;
 return DeeSequence_Front(seq);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_back) {
 DeeObject *seq;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_seq_back",&seq) != 0) return NULL;
 return DeeSequence_Back(seq);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_get) {
 DeeObject *seq; Dee_ssize_t idx;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oId:__builtin_seq_get",&seq,&idx) != 0) return NULL;
 return DeeSequence_Get(seq,idx);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_at) {
 DeeObject *seq; Dee_size_t idx;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oIu:__builtin_seq_at",&seq,&idx) != 0) return NULL;
 return DeeSequence_At(seq,idx);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_reversed) {
 DeeObject *seq;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_seq_reversed",&seq) != 0) return NULL;
 return DeeSequence_Reversed(seq);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_sorted) {
 DeeObject *seq,*pred = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:__builtin_seq_sorted",&seq,&pred) != 0) return NULL;
 return (DeeNone_Check(pred) || pred == (DeeObject *)&DeeBuiltinFunction___lo__)
  ? DeeSequence_Sorted(seq) : DeeSequence_SortedPred(seq,pred);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_seq_segments) {
 DeeObject *seq; Dee_size_t n;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oIu:__builtin_seq_segments",&seq,&n) != 0) return NULL;
 return DeeSequence_Segments(seq,n);
}








//////////////////////////////////////////////////////////////////////////
// GC related
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_gc_collect_now) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_gc_collect_now") != 0) return NULL;
 return DeeObject_New(Dee_size_t,DeeGC_CollectNow());
}


#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
//////////////////////////////////////////////////////////////////////////
// Class related
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_class) {
 DeeTypeObject *result,*base = &DeeObject_Type;
 DeeObject *name = Dee_EmptyString,*uuid_str = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|ooo:__builtin_class",&base,&name,&uuid_str) != 0) return NULL;
 if DEE_UNLIKELY((base = (DeeTypeObject *)DeeObject_GetInstance((DeeObject *)base,&DeeType_Type)) == NULL) return NULL;
 if DEE_UNLIKELY((name = DeeString_Cast(name)) == NULL) return NULL;
 if (!DeeNone_Check(uuid_str)) {
  struct DeeUUID uuid;
  if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(uuid_str,&DeeString_Type) != 0) result = NULL;
  else {
   if DEE_UNLIKELY(DeeUUID_FromStringWithError(&uuid,DeeString_STR(uuid_str)) != 0) result = NULL;
   else result = DeeClass_NewObjectEx(base,DeeString_SIZE(name) ? name : NULL,&uuid);
  }
 } else {
  result = DeeClass_NewObject(base,DeeString_SIZE(name) ? name : NULL);
 }
 Dee_DECREF(name);
 return (DeeObject *)result;
}
#endif








#if DEE_CONFIG_RUNTIME_HAVE_EXIT
//////////////////////////////////////////////////////////////////////////
// Exit related
DEE_COMPILER_MSVC_WARNING_PUSH(4702)
static void _dee_rterror_exit_returned(void) {
 DeeError_RuntimeError("'Dee_Exit' returned");
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_exit) {
 int exit_code = 0;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|%d:__builtin_exit",&exit_code) != 0) return NULL;
 Dee_Exit(exit_code);
 _dee_rterror_exit_returned();
 return NULL;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin__exit) {
 int exit_code = 0;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|%d:__builtin__exit",&exit_code) != 0) return NULL;
 Dee__Exit(exit_code);
 _dee_rterror_exit_returned();
 return NULL;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fatal) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fatal") != 0) return NULL;
 Dee_Fatal();
 _dee_rterror_exit_returned();
 return NULL;
}
DEE_COMPILER_MSVC_WARNING_POP
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_atexit) {
 DeeObject *callback_func,*callback_args; int temp;
 if (DeeTuple_EMPTY(args)) {
  DeeError_TypeError_ArgCountExpectedNamedMin("__builtin_atexit",1,0);
  return NULL;
 }
 callback_func = DeeTuple_GET(args,0);
 callback_args = DeeTuple_NewFromVector(DeeTuple_SIZE(args)-1,DeeTuple_ELEM(args)+1);
 if DEE_UNLIKELY(!callback_args) return NULL;
 temp = Dee_AtExit(callback_func,callback_args);
 Dee_DECREF(callback_args);
 if DEE_UNLIKELY(temp != 0) return NULL;
 DeeReturn_None;
}
#endif












//////////////////////////////////////////////////////////////////////////
// Stuff from the fs api
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_user) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_user") != 0) return NULL;
 return DeeFS_Utf8GetUserName();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wuser) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_wuser") != 0) return NULL;
 return DeeFS_WideGetUserName();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_machine) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_machine") != 0) return NULL;
 return DeeFS_Utf8GetMachineName();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wmachine) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_wmachine") != 0) return NULL;
 return DeeFS_WideGetMachineName();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_getcwd) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_getcwd") != 0) return NULL;
 return DeeFS_Utf8GetCwd();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wgetcwd) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_wgetcwd") != 0) return NULL;
 return DeeFS_WideGetCwd();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_chdir) {
 DeeObject *cwd;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"%o:__builtin_fs_chdir",&cwd) != 0) return NULL;
 if DEE_UNLIKELY(DeeFS_ChDirObject(cwd) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_gettmp) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_gettmp") != 0) return NULL;
 return DeeFS_Utf8GetTmp();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wgettmp) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_wgettmp") != 0) return NULL;
 return DeeFS_WideGetTmp();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_gettmpname) {
 DeeUtf8String_NEW_STATIC_EX(_unnamed,9,{'t','e','m','p','o','r','a','r','y'});
 DeeObject *result,*path = Dee_None,*prefix = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|oo:__builtin_fs_gettmpname",&path,&prefix) != 0) return NULL;
 if (DeeNone_Check(path)) {
  if DEE_UNLIKELY((path = DeeFS_Utf8GetTmp()) == NULL) return NULL;
 } else Dee_INCREF(path);
 if (DeeNone_Check(prefix)) prefix = (DeeObject *)&_unnamed;
 result = DeeFS_GetTmpNameObject(path,prefix);
 Dee_DECREF(path);
 return result;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wgettmpname) {
 DeeWideString_NEW_STATIC_EX(_unnamed,9,{'t','e','m','p','o','r','a','r','y'});
 DeeObject *result,*path = Dee_None,*prefix = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|oo:__builtin_fs_wgettmpname",&path,&prefix) != 0) return NULL;
 if (DeeNone_Check(path)) {
  if DEE_UNLIKELY((path = DeeFS_WideGetTmp()) == NULL) return NULL;
 } else Dee_INCREF(path);
 if (DeeNone_Check(prefix)) prefix = (DeeObject *)&_unnamed;
 result = DeeFS_GetTmpNameObject(path,prefix);
 Dee_DECREF(path);
 return result;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_gethome) {
 DeeObject *user = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:__builtin_fs_gethome",&user) != 0) return NULL;
 return DeeNone_Check(user) ? DeeFS_Utf8GetHome() : DeeFS_GetHomeUserObject(user);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wgethome) {
 DeeObject *user = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:__builtin_fs_wgethome",&user) != 0) return NULL;
 return DeeNone_Check(user) ? DeeFS_WideGetHome() : DeeFS_GetHomeUserObject(user);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_listenv) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_listenv") != 0) return NULL;
 return DeeFS_Utf8ListEnv();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wlistenv) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_wlistenv") != 0) return NULL;
 return DeeFS_WideListEnv();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_hasenv) {
 DeeObject *name; int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_hasenv",&name) != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFS_HasEnvObject(name)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_getenv) {
 DeeObject *name;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_getenv",&name) != 0) return NULL;
 return DeeFS_GetEnvObject(name);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_delenv) {
 DeeObject *name;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_delenv",&name) != 0) return NULL;
 if DEE_UNLIKELY(DeeFS_DelEnvObject(name) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_setenv) {
 DeeObject *name,*value;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__builtin_fs_setenv",&name,&value) != 0) return NULL;
 if DEE_UNLIKELY(DeeFS_SetEnvObject(name,value) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_head) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_path_head",&path) != 0) return NULL;
 return DeeFS_PathHeadObject(path);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_tail) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_path_tail",&path) != 0) return NULL;
 return DeeFS_PathTailObject(path);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_file) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_path_file",&path) != 0) return NULL;
 return DeeFS_PathFileObject(path);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_ext) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_path_ext",&path) != 0) return NULL;
 return DeeFS_PathExtObject(path);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_drive) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_path_drive",&path) != 0) return NULL;
 return DeeFS_PathDriveObject(path);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_inctrail) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_path_inctrail",&path) != 0) return NULL;
 return DeeFS_PathIncTrailObject(path);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_exctrail) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_path_exctrail",&path) != 0) return NULL;
 return DeeFS_PathExcTrailObject(path);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_abs) {
 DeeObject *path,*cwd=NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:__builtin_fs_path_abs",&path,&cwd) != 0) return NULL;
 if (cwd) return DeeFS_PathAbsObject(path,cwd);
 if DEE_UNLIKELY((cwd = DeeWideString_Check(path)
  ? DeeFS_WideGetCwd() : DeeFS_Utf8GetCwd()) == NULL) return NULL;
 path = DeeFS_PathAbsObject(path,cwd);
 Dee_DECREF(cwd);
 return path;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_rel) {
 DeeObject *path,*cwd = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:__builtin_fs_path_rel",&path,&cwd) != 0) return NULL;
 if (cwd) return DeeFS_PathRelObject(path,cwd);
 if DEE_UNLIKELY((cwd = DeeWideString_Check(path)
  ? DeeFS_WideGetCwd() : DeeFS_Utf8GetCwd()) == NULL) return NULL;
 path = DeeFS_PathRelObject(path,cwd);
 Dee_DECREF(cwd);
 return path;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_join) {
 return DeeFS_PathJoinObject(DeeTuple_SIZE(args),(DeeObject const*const*)DeeTuple_ELEM(args));
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_expanduser) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_path_expanduser",&path) != 0) return NULL;
 return DeeFS_PathExpandUserObject(path);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_path_expandvars) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_path_expandvars",&path) != 0) return NULL;
 return DeeFS_PathExpandVarsObject(path);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_getatime) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_getatime",&path) != 0) return NULL;
 if DEE_UNLIKELY(DeeFileIO_Check(path)) return DeeFileIO_GetATime(path);
 return DeeFS_GetATimeObject(path);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_getctime) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_getctime",&path) != 0) return NULL;
 if DEE_UNLIKELY(DeeFileIO_Check(path)) return DeeFileIO_GetCTime(path);
 return DeeFS_GetCTimeObject(path);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_getmtime) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_getmtime",&path) != 0) return NULL;
 return DeeFileIO_Check(path) ? DeeFileIO_GetMTime(path) : DeeFS_GetMTimeObject(path);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_gettimes) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_gettimes",&path) != 0) return NULL;
 return DeeFileIO_Check(path) ? DeeFileIO_GetTimes(path) : DeeFS_GetTimesObject(path);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_setatime) {
 DeeObject *path,*tm_access;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:setatime",&path,&tm_access) != 0) return NULL;
 if DEE_UNLIKELY((DeeFileIO_Check(path)
  ? DeeFileIO_SetATime(path,tm_access)
  : DeeFS_SetATimeObject(path,tm_access)) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_setctime) {
 DeeObject *path,*tm_creation;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:setctime",&path,&tm_creation) != 0) return NULL;
 if DEE_UNLIKELY((DeeFileIO_Check(path)
  ? DeeFileIO_SetCTime(path,tm_creation)
  : DeeFS_SetCTimeObject(path,tm_creation)) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_setmtime) {
 DeeObject *path,*tm_modification;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:setmtime",&path,&tm_modification) != 0) return NULL;
 if DEE_UNLIKELY((DeeFileIO_Check(path)
  ? DeeFileIO_SetMTime(path,tm_modification)
  : DeeFS_SetMTimeObject(path,tm_modification)
  ) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_settimes) {
 DeeObject *path,*tm_access = Dee_None,*tm_creation = Dee_None,*tm_modification = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|ooo:settimes",&path,
  &tm_access,&tm_creation,&tm_modification) != 0) return NULL;
 if (DeeFileIO_Check(path)) {
  if DEE_UNLIKELY(DeeFileIO_SetTimes2(path,
   DeeNone_Check(tm_access) ? NULL : tm_access,
   DeeNone_Check(tm_creation) ? NULL : tm_creation,
   DeeNone_Check(tm_modification) ? NULL : tm_modification) != 0) return NULL;
 } else {
  if DEE_UNLIKELY(DeeFS_SetTimes2Object(path,
   DeeNone_Check(tm_access) ? NULL : tm_access,
   DeeNone_Check(tm_creation) ? NULL : tm_creation,
   DeeNone_Check(tm_modification) ? NULL : tm_modification) != 0) return NULL;
 }
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_isabs) {
 DeeObject *path; int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_isabs",&path) != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFS_IsAbsObject(path)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_isfile) {
 DeeObject *path; int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_isfile",&path) != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_Check(path)
  ? DeeFileIO_IsFile(path)
  : DeeFS_IsFileObject(path)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_isdir) {
 DeeObject *path; int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_isdir",&path) != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_Check(path)
  ? DeeFileIO_IsDir(path)
  : DeeFS_IsDirObject(path)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_islink) {
 DeeObject *path; int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_islink",&path) != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_Check(path)
  ? DeeFileIO_IsLink(path)
  : DeeFS_IsLinkObject(path)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_isdrive) {
 DeeObject *path; int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_isdrive",&path) != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_Check(path)
  ? DeeFileIO_IsDrive(path)
  : DeeFS_IsDriveObject(path)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_ismount) {
 DeeObject *path; int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_ismount",&path) != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_Check(path)
  ? DeeFileIO_IsMount(path)
  : DeeFS_IsMountObject(path)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_ishidden) {
 DeeObject *path; int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_ishidden",&path) != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_Check(path)
  ? DeeFileIO_IsHidden(path)
  : DeeFS_IsHiddenObject(path)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_isxfile) {
 DeeObject *path; int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_isxfile",&path) != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_Check(path)
  ? DeeFileIO_IsExecutable(path)
  : DeeFS_IsExecutableObject(path)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_ischrdev) {
 DeeObject *path; int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_ischrdev",&path) != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_Check(path)
  ? DeeFileIO_IsCharDev(path)
  : DeeFS_IsCharDevObject(path)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_isblkdev) {
 DeeObject *path; int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_isblkdev",&path) != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_Check(path)
  ? DeeFileIO_IsBlockDev(path)
  : DeeFS_IsBlockDevObject(path)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_isfifo) {
 DeeObject *path; int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_isfifo",&path) != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_Check(path)
  ? DeeFileIO_IsFiFo(path)
  : DeeFS_IsFiFoObject(path)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_issocket) {
 DeeObject *path; int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_issocket",&path) != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFileIO_Check(path)
  ? DeeFileIO_IsSocket(path)
  : DeeFS_IsSocketObject(path)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_exists) {
 DeeObject *path; int temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_exists",&path) != 0) return NULL;
 if DEE_UNLIKELY((temp = DeeFS_ExistsObject(path)) < 0) return NULL;
 DeeReturn_Bool(temp);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_remove) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_remove",&path) != 0) return NULL;
 if DEE_UNLIKELY(DeeFS_RemoveObject(path) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_rmfile) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_rmfile",&path) != 0) return NULL;
 if DEE_UNLIKELY(DeeFS_RmFileObject(path) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_mkdir) {
 DeeObject *path; Dee_uint32_t mode = 0755;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|I32u:__builtin_fs_mkdir",&path,&mode) != 0) return NULL;
 if DEE_UNLIKELY(DeeFS_MkDirObject(path,mode) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_rmdir) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_rmdir",&path) != 0) return NULL;
 if DEE_UNLIKELY(DeeFS_RmDirObject(path) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_copy) {
 DeeObject *src,*dst,*callback = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo|o:__builtin_fs_copy",&src,&dst,&callback) != 0) return NULL;
 if DEE_UNLIKELY((DeeNone_Check(callback)
  ? DeeFS_CopyObject(src,dst)
  : DeeFS_CopyObjectWithProgressCallable(src,dst,callback)
  ) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_move) {
 DeeObject *src,*dst;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__builtin_fs_move",&src,&dst) != 0) return NULL;
 if DEE_UNLIKELY(DeeFS_MoveObject(src,dst) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_link) {
 DeeObject *src,*dst;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__builtin_fs_link",&src,&dst) != 0) return NULL;
 if DEE_UNLIKELY(DeeFS_LinkObject(src,dst) != 0) return NULL;
 DeeReturn_None;
}
#ifdef DEE_PLATFORM_WINDOWS
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32hardlink) {
 DeeObject *src,*dst;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__builtin_fs_win32hardlink",&src,&dst) != 0) return NULL;
 if DEE_UNLIKELY(DeeFS_Win32HardLinkObject(src,dst) != 0) return NULL;
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32modulename) {
 void *module;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"p:__builtin_fs_win32modulename",&module) != 0) return NULL;
 return DeeFS_Utf8Win32GetModuleName(module);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32wmodulename) {
 void *module;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"p:__builtin_fs_win32wmodulename",&module) != 0) return NULL;
 return DeeFS_WideWin32GetModuleName(module);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32dlldirectory) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_win32dlldirectory") != 0) return NULL;
 return DeeFS_Utf8Win32GetDllDirectory();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32wdlldirectory) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_win32wdlldirectory") != 0) return NULL;
 return DeeFS_WideWin32GetDllDirectory();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32systemdirectory) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_win32systemdirectory") != 0) return NULL;
 return DeeFS_Utf8Win32GetSystemDirectory();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32wsystemdirectory) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_win32wsystemdirectory") != 0) return NULL;
 return DeeFS_WideWin32GetSystemDirectory();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32windowsdirectory) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_win32windowsdirectory") != 0) return NULL;
 return DeeFS_Utf8Win32GetWindowsDirectory();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32wwindowsdirectory) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_win32wwindowsdirectory") != 0) return NULL;
 return DeeFS_WideWin32GetWindowsDirectory();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32systemwindowsdirectory) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_win32systemwindowsdirectory") != 0) return NULL;
 return DeeFS_Utf8Win32GetSystemWindowsDirectory();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32wsystemwindowsdirectory) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_win32wsystemwindowsdirectory") != 0) return NULL;
 return DeeFS_WideWin32GetSystemWindowsDirectory();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32syswow64directory) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_win32syswow64directory") != 0) return NULL;
 return DeeFS_Utf8Win32GetSysWow64Directory();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32wsyswow64directory) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_win32wsyswow64directory") != 0) return NULL;
 return DeeFS_WideWin32GetSysWow64Directory();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32drives) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_win32drives") != 0) return NULL;
 return DeeFS_Utf8Win32Drives();
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_win32wdrives) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_fs_win32wdrives") != 0) return NULL;
 return DeeFS_WideWin32Drives();
}
#endif
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_readlink) {
 DeeObject *path;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_readlink",&path) != 0) return NULL;
 return DeeFS_ReadLinkObject(path);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_chmod) {
 DeeObject *path,*mode; Dee_mode_t modeid;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__builtin_fs_chmod",&path,&mode) != 0) return NULL;
 if (DeeFileIO_Check(path)) {
  if (DeeString_Check(mode)) {
   if DEE_UNLIKELY(DeeFileIO_ChmodString(path,DeeString_STR(mode)) != 0) return NULL;
  } else {
   if DEE_UNLIKELY(DeeObject_Cast(Dee_mode_t,mode,&modeid) != 0) return NULL;
   if DEE_UNLIKELY(DeeFileIO_Chmod(path,modeid) != 0) return NULL;
  }
 } else {
  if (DeeString_Check(mode)) {
   if DEE_UNLIKELY(DeeFS_ChmodStringObject(path,DeeString_STR(mode)) != 0) return NULL;
  } else {
   if DEE_UNLIKELY(DeeObject_Cast(Dee_mode_t,mode,&modeid) != 0) return NULL;
   if DEE_UNLIKELY(DeeFS_ChmodObject(path,modeid) != 0) return NULL;
  }
 }
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_getmod) {
 DeeObject *path; Dee_mode_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_getmod",&path) != 0) return NULL;
 if DEE_UNLIKELY((DeeFileIO_Check(path)
  ? DeeFileIO_GetMod(path,&result)
  : DeeFS_GetModObject(path,&result)) != 0) return NULL;
 return DeeObject_New(Dee_mode_t,result);
}

DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_chown) {
 DeeObject *path,*arg1,*arg2 = NULL;
 Dee_uid_t uid; Dee_gid_t gid;
#ifdef DEE_PLATFORM_WINDOWS
 int error; DeeObject *buf1,*buf2;
#endif
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo|o:__builtin_fs_chown",&path,&arg1,&arg2) != 0) return NULL;
#ifdef DEE_PLATFORM_WINDOWS
 if DEE_UNLIKELY(_DeeFS_AutoGetUserAndGroup(arg1,arg2,&uid,&gid,&buf1,&buf2) != 0) return NULL;
 error = DeeFileIO_Check(path) ? DeeFileIO_Chown(path,uid,gid) : DeeFS_ChownObject(path,uid,gid);
 Dee_XDECREF(buf1);
 Dee_XDECREF(buf2);
 if (error == -1) return NULL;
#else
 if DEE_UNLIKELY(_DeeFS_AutoGetUserAndGroup(arg1,arg2,&uid,&gid) != 0) return NULL;
 if DEE_UNLIKELY((DeeFileIO_Check(path)
  ? DeeFileIO_Chown(path,uid,gid)
  : DeeFS_ChownObject(path,uid,gid)) != 0) return NULL;
#endif
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_getown) {
 DeeObject *path,*result; Dee_uid_t uid; Dee_gid_t gid;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_getown",&path) != 0) return NULL;
 if DEE_UNLIKELY((DeeFileIO_Check(path)
  ? DeeFileIO_GetOwn(path,&uid,&gid)
  : DeeFS_GetOwnObject(path,&uid,&gid)) != 0) return NULL;
 if DEE_UNLIKELY((result = _DeeTuple_NewUnsafe(2)) == NULL) return NULL;
 if DEE_UNLIKELY((DeeTuple_ELEM(result)[0] = DeeObject_New(Dee_uid_t,uid)) == NULL) {
err_0: _DeeTuple_FreeUnsafe(result); return NULL;
 }
 if DEE_UNLIKELY((DeeTuple_ELEM(result)[1] = DeeObject_New(Dee_gid_t,gid)) == NULL) {
  Dee_DECREF(DeeTuple_ELEM(result)[0]); goto err_0;
 }
 return result;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_uidof) {
 DeeObject *user;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_uidof",&user) != 0) return NULL;
#ifdef DEE_PLATFORM_WINDOWS
 return DeeFS_Win32NameToSIDObject(user);
#else
 {Dee_uid_t result;
  if DEE_UNLIKELY(DeeFS_PosixUserToUidObject(user,&result) != 0) return NULL;
  return DeeObject_New(Dee_uid_t,result);
 }
#endif
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_gidof) {
 DeeObject *user;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_fs_gidof",&user) != 0) return NULL;
#ifdef DEE_PLATFORM_WINDOWS
 return DeeFS_Win32NameToSIDObject(user);
#else
 {Dee_gid_t result;
  if DEE_UNLIKELY(DeeFS_PosixGroupToGidObject(user,&result) != 0) return NULL;
  return DeeObject_New(Dee_gid_t,result);
 }
#endif
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_uidname) {
 Dee_uid_t uid; int full_name = 0;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,DEE_TUPLE_UNPACKFMT_UINT(DEE_TYPES_SIZEOF_UID_T)
  "|b:__builtin_fs_uidname",&uid,&full_name) != 0) return NULL;
 return DeeFS_Utf8UidToUser(uid,full_name);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_gidname) {
 Dee_gid_t gid; int full_name = 0;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,DEE_TUPLE_UNPACKFMT_UINT(DEE_TYPES_SIZEOF_UID_T)
  "|b:__builtin_fs_gidname",&gid,&full_name) != 0) return NULL;
 return DeeFS_Utf8GidToGroup(gid,full_name);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wuidname) {
 Dee_uid_t uid; int full_name = 0;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,DEE_TUPLE_UNPACKFMT_UINT(DEE_TYPES_SIZEOF_UID_T)
  "|b:__builtin_fs_wuidname",&uid,&full_name) != 0) return NULL;
 return DeeFS_WideUidToUser(uid,full_name);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_fs_wgidname) {
 Dee_gid_t gid; int full_name = 0;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,DEE_TUPLE_UNPACKFMT_UINT(DEE_TYPES_SIZEOF_UID_T)
  "|b:__builtin_fs_wgidname",&gid,&full_name) != 0) return NULL;
 return DeeFS_WideGidToGroup(gid,full_name);
}












#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
//////////////////////////////////////////////////////////////////////////
// Raw memory / Pointer related
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_memcpy) {
 void *dst,*src,*result; Dee_size_t s;
 if (DeeTuple_Unpack(args,"ppIu:__builtin_memcpy",&dst,&src,&s) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = memcpy(dst,src,s);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeVoidPointer_New(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_mempcpy) {
 void *dst,*src,*result; Dee_size_t s;
 if (DeeTuple_Unpack(args,"ppIu:__builtin_mempcpy",&dst,&src,&s) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = memcpy(dst,src,s);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeVoidPointer_New((void *)((Dee_uintptr_t)result+s));
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_memccpy) {
 char *dst,*src; int c; Dee_size_t s;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ppdIu:__builtin_memccpy",&dst,&src,&c,&s) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  while (1) {
   if (!s--) { dst = NULL; break; }
   if ((*dst++ = *src++) == c) break;
  }
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeVoidPointer_New(dst);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_rawmemchr) {
 char *p; int c;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"pd:__builtin_rawmemchr",&p,&c) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  while (1) { if (*p == c) break; ++p; }
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeVoidPointer_New(p);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_memmove) {
 void *dst,*src,*result; Dee_size_t s;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ppIu:__builtin_memmove",&dst,&src,&s) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = memmove(dst,src,s);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeVoidPointer_New(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_memcmp) {
 void *a,*b; Dee_size_t s; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ppIu:__builtin_memcmp",&a,&b,&s) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = memcmp(a,b,s);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeObject_New(int,result);
}
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 6054)
#endif
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strcoll) {
 char *s1,*s2; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ss:__builtin_strcoll",&s1,&s2) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strcoll(s1,s2);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeObject_New(int,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strcmp) {
 char *s1,*s2; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ss:__builtin_strcmp",&s1,&s2) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strcmp(s1,s2);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeObject_New(int,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strncmp) {
 char *s1,*s2; Dee_size_t n; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ssIu:__builtin_strncmp",&s1,&s2,&n) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strncmp(s1,s2,n);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeObject_New(int,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strxfrm) {
 char *dst,*src; Dee_size_t n,result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ssIu:__builtin_strxfrm",&dst,&src,&n) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strxfrm(dst,src,n);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeObject_New(Dee_size_t,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strcpy) {
 char *dst,*src,*result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ss:__builtin_strcpy",&dst,&src) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strcpy(dst,src);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeCharPointer_New(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strncpy) {
 char *dst,*src,*result; Dee_size_t c;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ssIu:__builtin_strcpy",&dst,&src,&c) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strncpy(dst,src,c);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeCharPointer_New(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_stpcpy) {
 char *dst,*src;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ss:__builtin_stpcpy",&dst,&src) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  do *dst++ = *src++; while (*src);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeCharPointer_New(dst);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_stpncpy) {
 char *dst,*src; Dee_size_t n;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ssIu:__builtin_stpncpy",&dst,&src,&n) != 0) return NULL;
 if (n) DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  do *dst++ = *src++; while (*src && --n);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeCharPointer_New(dst);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strcat) {
 char *dst,*src,*result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ss:__builtin_strcat",&dst,&src) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strcat(dst,src);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeCharPointer_New(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strncat) {
 char *dst,*src,*result; Dee_size_t s;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ssIu:__builtin_strncat",&dst,&src,&s) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strncat(dst,src,s);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeCharPointer_New(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_memchr) {
 void *p,*result; int c; Dee_size_t s;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"pds:__builtin_memchr",&p,&c,&s) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = memchr(p,c,s);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeVoidPointer_New(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_memrchr) {
 char *p; int c; Dee_size_t s;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"pds:__builtin_memrchr",&p,&c,&s) != 0) return NULL;
 p += s;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  while (1) {
   if (!s--) { p = NULL; break; }
   if (*--p == c) break;
  }
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeVoidPointer_New(p);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strchr) {
 char *p,*result; int c;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"sd:__builtin_strchr",&p,&c) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strchr(p,c);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeCharPointer_New(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strchrnul) {
 char *p; int c;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"sd:__builtin_strchrnul",&p,&c) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  while (*p && *p != c) ++p;
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeCharPointer_New(p);
}
DEE_STATIC_INLINE(int) _dee_strcase_cmp(char const *a, char const *b) {
 int temp;
 while (*a && *b) if ((temp = (DEE_CH_TO_UPPER(*b++)-DEE_CH_TO_UPPER(*a++))) != 0) return temp;
 return DEE_CH_TO_UPPER(*b++)-DEE_CH_TO_UPPER(*a++);
}
DEE_STATIC_INLINE(int) _dee_strncase_cmp(char const *a, char const *b, Dee_size_t n) {
 int temp;
 while (*a && *b && n--) if ((temp = (DEE_CH_TO_UPPER(*b++)-DEE_CH_TO_UPPER(*a++))) != 0) return temp;
 return DEE_CH_TO_UPPER(*b++)-DEE_CH_TO_UPPER(*a++);
}
DEE_STATIC_INLINE(int) _dee_strcase_cmpr(char const *a, char const *b) {
 int temp;
 while (*a && *b) if ((temp = (DEE_CH_TO_UPPER(*b++)-DEE_CH_TO_UPPER(*a++))) != 0) return temp;
 if (!*b) return 0; // b is contained in a
 return DEE_CH_TO_UPPER(*b++)-DEE_CH_TO_UPPER(*a++);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strcasestr) {
 char *haystack,*needle;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ss:__builtin_strcasestr",&haystack,&needle) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  while (1) {
   if (!*haystack) { haystack = NULL; break; }
   if (_dee_strcase_cmpr(haystack,needle) == 0) break;
   ++haystack;
  }
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeCharPointer_New(haystack);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strcasecmp) {
 char *a,*b; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ss:__builtin_strcasecmp",&a,&b) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = _dee_strcase_cmp(a,b);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeObject_New(int,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strncasecmp) {
 char *a,*b; Dee_size_t n; int result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ssIu:__builtin_strncasecmp",&a,&b,&n) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = _dee_strncase_cmp(a,b,n);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeObject_New(int,result);
}

DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_memmem) {
 char *haystack,*needle; Dee_size_t haystack_len,needle_len;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"pIupIu:__builtin_memmem",
  &haystack,&haystack_len,&needle,&needle_len) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  while (1) {
   if (haystack_len < needle_len) { haystack = NULL; break; }
   if (memcmp(haystack,needle,needle_len) == 0) break;
   ++haystack,--haystack_len;
  }
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeVoidPointer_New(haystack);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strcspn) {
 char *p,*c; Dee_size_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ss:__builtin_strcspn",&p,&c) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strcspn(p,c);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeObject_New(Dee_size_t,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strpbrk) {
 char *p,*c,*result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ss:__builtin_strpbrk",&p,&c) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strpbrk(p,c);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeCharPointer_New(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strrchr) {
 char *p,*result; int c;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"sd:__builtin_strrchr",&p,&c) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strrchr(p,c);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeCharPointer_New(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strspn) {
 char *p,*c; Dee_size_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ss:__builtin_strspn",&p,&c) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strspn(p,c);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeObject_New(Dee_size_t,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strstr) {
 char *p,*c,*result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ss:__builtin_strstr",&p,&c) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strstr(p,c);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeCharPointer_New(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strtok) {
 char *p,*c,*result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ss:__builtin_strtok",&p,&c) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strtok(p,c);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeCharPointer_New(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strerror) {
 int err; char const *temp;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ss:__builtin_strerror",&err) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  temp = strerror(err);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 if (!temp) DeeReturn_EmptyString;
 return DeeString_New(temp);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strlen) {
 char *str; Dee_size_t result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"s:__builtin_strlen",&str) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strlen(str);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeObject_New(Dee_size_t,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strnlen) {
 char *str; Dee_size_t n,result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"sIu:__builtin_strlen",&str,&n) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strnlen(str,n);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeObject_New(Dee_size_t,result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strdup) {
 char *str,*result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"s:__builtin_strdup",&str) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strdup(str);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeCharPointer_New(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_strndup) {
 char *str,*result; Dee_size_t n;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"sIu:__builtin_strndup",&str,&n) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = strndup(str,n);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeCharPointer_New(result);
}
#ifdef _PREFAST_
#pragma warning(pop)
#endif
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_memset) {
 void *p,*result; int v; Dee_size_t n;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"pdIu:__builtin_memset",&p,&v,&n) != 0) return NULL;
 DEE_BUILTINMEM_SIGNALHANDLER_BEGIN {
  result = memset(p,v,n);
 } DEE_BUILTINMEM_SIGNALHANDLER_END({
  return NULL;
 });
 return DeeVoidPointer_New(result);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_malloc) {
 Dee_size_t s; void *result_p; DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu:__builtin_malloc",&s) != 0) return NULL;
 if DEE_UNLIKELY(!s) DeeReturn_None;
 if DEE_UNLIKELY((result_p = malloc_nz(s)) == NULL) { DeeError_NoMemory(); return NULL; }
 if DEE_UNLIKELY((result = DeeVoidPointer_New(result_p)) == NULL) free_nn(result_p);
 return result;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_calloc) {
 Dee_size_t n,s=1; void *result_p; DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu|Iu:__builtin_calloc",&n,&s) != 0) return NULL;
 if DEE_UNLIKELY((n*s) == 0) DeeReturn_None;
 if DEE_UNLIKELY((result_p = calloc_nz(n,s)) == NULL) { DeeError_NoMemory(); return NULL; }
 if DEE_UNLIKELY((result = DeeVoidPointer_New(result_p)) == NULL) free_nn(result_p);
 return result;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_realloc) {
 void *p; Dee_size_t s; DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"pIu:__builtin_realloc",&p,&s) != 0) return NULL;
 if DEE_UNLIKELY(!s) { free(p); DeeReturn_None; }
 if DEE_UNLIKELY((p = realloc_nz(p,s)) == NULL) { DeeError_NoMemory(); return NULL; }
 if DEE_UNLIKELY((result = DeeVoidPointer_New(p)) == NULL) free_nn(p);
 return result;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_free) {
 void *p;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"p:__builtin_free",&p) != 0) return NULL;
 free(p);
 DeeReturn_None;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_aligned_malloc) {
 Dee_size_t s,a; void *result_p; DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"IuIu:__builtin_aligned_malloc",&s,&a) != 0) return NULL;
 if DEE_UNLIKELY(!s) DeeReturn_None;
 if DEE_UNLIKELY((result_p = aligned_malloc_nz(s,a)) == NULL) { DeeError_NoMemory(); return NULL; }
 if DEE_UNLIKELY((result = DeeVoidPointer_New(result_p)) == NULL) free_nn(result_p);
 return result;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_aligned_calloc) {
 Dee_size_t s,a; void *result_p; DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"IuIu:__builtin_aligned_calloc",&s,&a) != 0) return NULL;
 if DEE_UNLIKELY(!s) DeeReturn_None;
 if DEE_UNLIKELY((result_p = aligned_calloc_nz(s,a)) == NULL) { DeeError_NoMemory(); return NULL; }
 if DEE_UNLIKELY((result = DeeVoidPointer_New(result_p)) == NULL) free_nn(result_p);
 return result;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_aligned_realloc) {
 void *p; Dee_size_t s,a; DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"pIuIu:__builtin_aligned_realloc",&p,&s,&a) != 0) return NULL;
 if DEE_UNLIKELY(!s) { free(p); DeeReturn_None; }
 if DEE_UNLIKELY((p = aligned_realloc_nz(p,s,a)) == NULL) { DeeError_NoMemory(); return NULL; }
 if DEE_UNLIKELY((result = DeeVoidPointer_New(p)) == NULL) free_nn(p);
 return result;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_aligned_free) {
 void *p;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"p:__builtin_aligned_free",&p) != 0) return NULL;
 aligned_free(p);
 DeeReturn_None;
}
#endif

//////////////////////////////////////////////////////////////////////////
// Builtin math API
#if DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_FLOAT
#define DEE_M_RT_F(x) x##f
#elif DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_DOUBLE
#define DEE_M_RT_F(x) x
#elif DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_LDOUBLE
#define DEE_M_RT_F(x) x##l
#endif

#if DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_FLOAT
#define DEE_M_RT_D(x) x##f
#elif DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_DOUBLE
#define DEE_M_RT_D(x) x
#elif DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_LDOUBLE
#define DEE_M_RT_D(x) x##l
#endif

#if DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_FLOAT
#define DEE_M_RT_L(x) x##f
#elif DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_DOUBLE
#define DEE_M_RT_L(x) x
#elif DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_LDOUBLE
#define DEE_M_RT_L(x) x##l
#endif

#define DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(name,m_name)\
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(name)\
{\
 DeeObject *ob; double temp;\
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:" #name,&ob) != 0) return NULL;\
 if (DeeDouble_Check(ob)) return DeeDouble_New(DEE_M_RT_D(Dee_##m_name)(DeeDouble_VALUE(ob)));\
 if (DeeFloat_Check(ob)) return DeeFloat_New(DEE_M_RT_F(Dee_##m_name)(DeeFloat_VALUE(ob)));\
 if (DeeLDouble_Check(ob)) return DeeLDouble_New(DEE_M_RT_L(Dee_##m_name)(DeeLDouble_VALUE(ob)));\
 if (DeeObject_Cast(double,ob,&temp) != 0) return NULL;\
 return DeeObject_New(double,Dee_##m_name(temp));\
}
#define DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY(name,m_name)\
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(name)\
{\
 DeeObject *x,*y;\
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:" #name,&x,&y) != 0) return NULL;\
 if (DeeDouble_Check(x)) { Dee_rt_double temp2;\
  if DEE_UNLIKELY(DeeObject_Cast(Dee_rt_double,y,&temp2) != 0) return NULL;\
  return DeeDouble_New(DEE_M_RT_D(Dee_##m_name)(DeeDouble_VALUE(x),temp2));\
 } else if (DeeFloat_Check(x)) { Dee_rt_float temp2;\
  if DEE_UNLIKELY(DeeObject_Cast(Dee_rt_float,y,&temp2) != 0) return NULL;\
  return DeeFloat_New(DEE_M_RT_F(Dee_##m_name)(DeeFloat_VALUE(x),temp2));\
 } else if (DeeLDouble_Check(x)) { Dee_rt_ldouble temp2; \
  if DEE_UNLIKELY(DeeObject_Cast(Dee_rt_ldouble,y,&temp2) != 0) return NULL;\
  return DeeLDouble_New(DEE_M_RT_L(Dee_##m_name)(DeeLDouble_VALUE(x),temp2));\
 } else { double fx,fy;\
  if DEE_UNLIKELY(DeeObject_Cast(double,x,&fx) != 0 || \
                  DeeObject_Cast(double,y,&fy) != 0) return NULL;\
  return DeeObject_New(double,Dee_##m_name(fx,fy));\
 }\
}
#define DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY_RET2(name,m_name)\
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(name)\
{\
 DeeObject *result,*ob;\
 Dee_rt_double def_buf[2];\
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:" #name,&ob) != 0) return NULL;\
 if DEE_UNLIKELY((result = _DeeTuple_NewUnsafe(2)) == NULL) return NULL;\
 if (DeeDouble_Check(ob)) {\
  DEE_M_RT_D(Dee_##m_name)(DeeDouble_VALUE(ob),def_buf);\
fill_def:\
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[0] = DeeDouble_New(def_buf[0])) == NULL) goto err_0;\
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[1] = DeeDouble_New(def_buf[1])) == NULL) goto err_1;\
 } else if (DeeFloat_Check(ob)) {\
  Dee_rt_float buf[2];\
  DEE_M_RT_F(Dee_##m_name)(DeeFloat_VALUE(ob),buf);\
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[0] = DeeFloat_New(buf[0])) == NULL) goto err_0;\
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[1] = DeeFloat_New(buf[1])) == NULL) goto err_1;\
 } else if (DeeLDouble_Check(ob)) {\
  Dee_rt_ldouble buf[2];\
  DEE_M_RT_L(Dee_##m_name)(DeeLDouble_VALUE(ob),buf);\
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[0] = DeeLDouble_New(buf[0])) == NULL) goto err_0;\
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[1] = DeeLDouble_New(buf[1])) == NULL) goto err_1;\
 } else { double temp;\
  if DEE_UNLIKELY(DeeObject_Cast(double,ob,&temp) != 0) goto err_0;\
  Dee_##m_name(temp,def_buf);\
  goto fill_def;\
 }\
 return result;\
err_1: Dee_DECREF(DeeTuple_GET(result,0));\
err_0: _DeeTuple_FreeUnsafe(result);\
 return NULL;\
}
#define DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY_BOOL(name,m_name)\
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(name)\
{\
 DeeObject *ob; double temp;\
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:" #name,&ob) != 0) return NULL;\
 if (DeeDouble_Check(ob)) DeeReturn_Bool(DEE_M_RT_D(Dee_##m_name)(DeeDouble_VALUE(ob)));\
 if (DeeFloat_Check(ob)) DeeReturn_Bool(DEE_M_RT_F(Dee_##m_name)(DeeFloat_VALUE(ob)));\
 if (DeeLDouble_Check(ob)) DeeReturn_Bool(DEE_M_RT_L(Dee_##m_name)(DeeLDouble_VALUE(ob)));\
 if (DeeObject_Cast(double,ob,&temp) != 0) return NULL;\
 DeeReturn_Bool(Dee_##m_name(temp));\
}
#define DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY_BOOL(name,m_name)\
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(name)\
{\
 DeeObject *x,*y;\
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:" #name,&x,&y) != 0) return NULL;\
 if (DeeDouble_Check(x)) { Dee_rt_double temp;\
  if DEE_UNLIKELY(DeeObject_Cast(Dee_rt_double,y,&temp) != 0) return NULL;\
  DeeReturn_Bool(DEE_M_RT_D(Dee_##m_name)(DeeDouble_VALUE(x),temp));\
 } else if (DeeFloat_Check(x)) { Dee_rt_float temp;\
  if DEE_UNLIKELY(DeeObject_Cast(Dee_rt_float,y,&temp) != 0) return NULL;\
  DeeReturn_Bool(DEE_M_RT_F(Dee_##m_name)(DeeFloat_VALUE(x),temp));\
 } else if (DeeFloat_Check(x)) { Dee_rt_ldouble temp;\
  if DEE_UNLIKELY(DeeObject_Cast(Dee_rt_ldouble,y,&temp) != 0) return NULL;\
  DeeReturn_Bool(DEE_M_RT_L(Dee_##m_name)(DeeLDouble_VALUE(x),temp));\
 } else { double fx,fy;\
  if DEE_UNLIKELY(DeeObject_Cast(double,x,&fx) != 0 ||\
                  DeeObject_Cast(double,y,&fy) != 0) return NULL;\
  DeeReturn_Bool(Dee_##m_name(fx,fy));\
 }\
}
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_sin,m_sin)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_cos,m_cos)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_tan,m_tan)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY_RET2(__builtin_sincos,m_sincos)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_asin,m_asin)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_acos,m_acos)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_atan,m_atan)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY(__builtin_atan2,m_atan2)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY_RET2(__builtin_asincos,m_asincos)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_sinh,m_sinh)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_cosh,m_cosh)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_tanh,m_tanh)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY_RET2(__builtin_sincosh,m_sincosh)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_asinh,m_asinh)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_acosh,m_acosh)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_atanh,m_atanh)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY_RET2(__builtin_asincosh,m_asincosh)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_exp,m_exp);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_frexp) {
 DeeObject *ob,*result; int y;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_frexp",&ob) != 0) return NULL;
 if DEE_UNLIKELY((result = _DeeTuple_NewUnsafe(2)) == NULL) return NULL;
 if (DeeDouble_Check(ob)) {
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[0] = DeeDouble_New(DEE_M_RT_D(Dee_m_frexp)(DeeDouble_VALUE(ob),&y))) == NULL) goto err_0;
 } else if (DeeFloat_Check(ob)) {
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[0] = DeeFloat_New(DEE_M_RT_F(Dee_m_frexp)(DeeFloat_VALUE(ob),&y))) == NULL) goto err_0;
 } else if (DeeLDouble_Check(ob)) {
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[0] = DeeLDouble_New(DEE_M_RT_L(Dee_m_frexp)(DeeLDouble_VALUE(ob),&y))) == NULL) goto err_0;
 } else { double x;
  if DEE_UNLIKELY(DeeObject_Cast(double,ob,&x) != 0) goto err_0;
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[0] = DeeObject_New(double,Dee_m_frexp(x,&y))) == NULL) goto err_0;
 }
 if DEE_UNLIKELY((DeeTuple_ELEM(result)[1] = DeeObject_New(int,y)) == NULL) goto err_1;
 return result;
err_1: Dee_DECREF(DeeTuple_GET(result,0));
err_0: _DeeTuple_FreeUnsafe(result);
 return NULL;
}
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_log,m_log);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_log10,m_log10);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_modf) {
 DeeObject *ob,*result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_modf",&ob) != 0) return NULL;
 if DEE_UNLIKELY((result = _DeeTuple_NewUnsafe(2)) == NULL) return NULL;
 if (DeeDouble_Check(ob)) { Dee_rt_double temp;
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[0] = DeeDouble_New(DEE_M_RT_D(Dee_m_modf)(DeeDouble_VALUE(ob),&temp))) == NULL) goto err_0;
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[1] = DeeDouble_New(temp)) == NULL) goto err_1;
 } else if (DeeFloat_Check(ob)) { Dee_rt_float temp;
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[0] = DeeFloat_New(DEE_M_RT_F(Dee_m_modf)(DeeFloat_VALUE(ob),&temp))) == NULL) goto err_0;
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[1] = DeeFloat_New(temp)) == NULL) goto err_1;
 } else if (DeeLDouble_Check(ob)) { Dee_rt_ldouble temp;
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[0] = DeeLDouble_New(DEE_M_RT_L(Dee_m_modf)(DeeLDouble_VALUE(ob),&temp))) == NULL) goto err_0;
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[1] = DeeLDouble_New(temp)) == NULL) goto err_1;
 } else { double temp1,temp2;
  if DEE_UNLIKELY(DeeObject_Cast(double,ob,&temp1) != 0) goto err_0;
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[0] = DeeObject_New(double,Dee_m_modf(DeeDouble_VALUE(ob),&temp2))) == NULL) goto err_0;
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[1] = DeeObject_New(double,temp2)) == NULL) goto err_1;
 }
 return result;
err_1: Dee_DECREF(DeeTuple_GET(result,0));
err_0: _DeeTuple_FreeUnsafe(result);
 return NULL;
}
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_exp2,m_exp2);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_expm1,m_expm1);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_ilogb) {
 DeeObject *ob; double x;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_ilogb",&ob) != 0) return NULL;
 if (DeeDouble_Check(ob)) return DeeObject_New(int,DEE_M_RT_D(Dee_m_ilogb)(DeeDouble_VALUE(ob)));
 if (DeeFloat_Check(ob)) return DeeObject_New(int,DEE_M_RT_F(Dee_m_ilogb)(DeeFloat_VALUE(ob)));
 if (DeeLDouble_Check(ob)) return DeeObject_New(int,DEE_M_RT_L(Dee_m_ilogb)(DeeLDouble_VALUE(ob)));
 if (DeeObject_Cast(double,ob,&x) != 0) return NULL;
 return DeeObject_New(int,Dee_m_ilogb(x));
}
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_log1p,m_log1p);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_log2,m_log2);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_logb,m_logb);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_scalbn) {
 DeeObject *ob; int y; double x;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"od:__builtin_scalbn",&ob,&y) != 0) return NULL;
 if (DeeDouble_Check(ob)) return DeeDouble_New(DEE_M_RT_D(Dee_m_scalbn)(DeeDouble_VALUE(ob),y));
 if (DeeFloat_Check(ob)) return DeeFloat_New(DEE_M_RT_F(Dee_m_scalbn)(DeeFloat_VALUE(ob),y));
 if (DeeLDouble_Check(ob)) return DeeLDouble_New(DEE_M_RT_L(Dee_m_scalbn)(DeeLDouble_VALUE(ob),y));
 if (DeeObject_Cast(double,ob,&x) != 0) return NULL;
 return DeeObject_New(double,Dee_m_scalbn(x,y));
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_scalbi32n) {
 DeeObject *ob; Dee_int32_t y; double x;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oI32d:__builtin_scalbi32n",&ob,&y) != 0) return NULL;
 if (DeeDouble_Check(ob)) return DeeDouble_New(DEE_M_RT_D(Dee_m_scalbi32n)(DeeDouble_VALUE(ob),y));
 if (DeeFloat_Check(ob)) return DeeFloat_New(DEE_M_RT_F(Dee_m_scalbi32n)(DeeFloat_VALUE(ob),y));
 if (DeeLDouble_Check(ob)) return DeeLDouble_New(DEE_M_RT_L(Dee_m_scalbi32n)(DeeLDouble_VALUE(ob),y));
 if (DeeObject_Cast(double,ob,&x) != 0) return NULL;
 return DeeObject_New(double,Dee_m_scalbi32n(x,y));
}
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_sqrt,m_sqrt);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_cbrt,m_cbrt);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY(__builtin_hypot,m_hypot);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_erf,m_erf);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_tgamma,m_tgamma);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_lgamma,m_lgamma);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_ceilf,m_ceil);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_floorf,m_floor);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_truncf,m_trunc);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_roundf,m_round);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_i32round) {
 DeeObject *ob; double x;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_i32round",&ob) != 0) return NULL;
 if DEE_UNLIKELY(DeeDouble_Check(ob)) return DeeInt32_New(DEE_M_RT_D(Dee_m_i32round)(DeeDouble_VALUE(ob)));
 if DEE_UNLIKELY(DeeFloat_Check(ob)) return DeeInt32_New(DEE_M_RT_F(Dee_m_i32round)(DeeFloat_VALUE(ob)));
 if DEE_UNLIKELY(DeeLDouble_Check(ob)) return DeeInt32_New(DEE_M_RT_L(Dee_m_i32round)(DeeLDouble_VALUE(ob)));
 if DEE_UNLIKELY(DeeObject_Cast(double,ob,&x) != 0) return NULL;
 return DeeInt32_New(Dee_m_i32round(x));
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_i64round) {
 DeeObject *ob; double x;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_i64round",&ob) != 0) return NULL;
 if DEE_UNLIKELY(DeeDouble_Check(ob)) return DeeInt64_New(DEE_M_RT_D(Dee_m_i64round)(DeeDouble_VALUE(ob)));
 if DEE_UNLIKELY(DeeFloat_Check(ob)) return DeeInt64_New(DEE_M_RT_F(Dee_m_i64round)(DeeFloat_VALUE(ob)));
 if DEE_UNLIKELY(DeeLDouble_Check(ob)) return DeeInt64_New(DEE_M_RT_L(Dee_m_i64round)(DeeLDouble_VALUE(ob)));
 if DEE_UNLIKELY(DeeObject_Cast(double,ob,&x) != 0) return NULL;
 return DeeInt64_New(Dee_m_i64round(x));
}
DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY(__builtin_fmod,m_fmod);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_rint,m_rint);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_i32rint) {
 DeeObject *ob; double x;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_i32rint",&ob) != 0) return NULL;
 if DEE_UNLIKELY(DeeDouble_Check(ob)) return DeeInt32_New(DEE_M_RT_D(Dee_m_i32rint)(DeeDouble_VALUE(ob)));
 if DEE_UNLIKELY(DeeFloat_Check(ob)) return DeeInt32_New(DEE_M_RT_F(Dee_m_i32rint)(DeeFloat_VALUE(ob)));
 if DEE_UNLIKELY(DeeLDouble_Check(ob)) return DeeInt32_New(DEE_M_RT_L(Dee_m_i32rint)(DeeLDouble_VALUE(ob)));
 if DEE_UNLIKELY(DeeObject_Cast(double,ob,&x) != 0) return NULL;
 return DeeInt32_New(Dee_m_i32rint(x));
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_i64rint) {
 DeeObject *ob; double x;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:__builtin_i64rint",&ob) != 0) return NULL;
 if DEE_UNLIKELY(DeeDouble_Check(ob)) return DeeInt64_New(DEE_M_RT_D(Dee_m_i64rint)(DeeDouble_VALUE(ob)));
 if DEE_UNLIKELY(DeeFloat_Check(ob)) return DeeInt64_New(DEE_M_RT_F(Dee_m_i64rint)(DeeFloat_VALUE(ob)));
 if DEE_UNLIKELY(DeeLDouble_Check(ob)) return DeeInt64_New(DEE_M_RT_L(Dee_m_i64rint)(DeeLDouble_VALUE(ob)));
 if DEE_UNLIKELY(DeeObject_Cast(double,ob,&x) != 0) return NULL;
 return DeeInt64_New(Dee_m_i64rint(x));
}
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY(__builtin_nearbyint,m_nearbyint);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY(__builtin_remainder,m_remainder);
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_remquo) {
 DeeObject *result,*x,*y; double fx,fy; int z;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:__builtin_remquo",&x,&y) != 0) return NULL;
 if DEE_UNLIKELY((result = _DeeTuple_NewUnsafe(2)) == NULL) return NULL;
 if (DeeDouble_Check(x)) { Dee_rt_double temp;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_rt_double,y,&temp) != 0) goto err_0;
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[0] = DeeDouble_New(DEE_M_RT_D(Dee_m_remquo)(DeeDouble_VALUE(x),temp,&z))) == NULL) goto err_0;
 } else if (DeeFloat_Check(x)) { Dee_rt_float temp;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_rt_float,y,&temp) != 0) goto err_0;
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[0] = DeeFloat_New(DEE_M_RT_F(Dee_m_remquo)(DeeFloat_VALUE(x),temp,&z))) == NULL) goto err_0;
 } else if (DeeLDouble_Check(x)) { Dee_rt_ldouble temp;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_rt_ldouble,y,&temp) != 0) goto err_0;
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[0] = DeeLDouble_New(DEE_M_RT_L(Dee_m_remquo)(DeeLDouble_VALUE(x),temp,&z))) == NULL) goto err_0;
 } else {
  if DEE_UNLIKELY(DeeObject_Cast(double,x,&fx) != 0 ||
                  DeeObject_Cast(double,y,&fy) != 0) goto err_0;
  if DEE_UNLIKELY((DeeTuple_ELEM(result)[0] = DeeObject_New(double,Dee_m_remquo(fx,fy,&z))) == NULL) goto err_0;
 }
 if DEE_UNLIKELY((DeeTuple_ELEM(result)[1] = DeeObject_New(int,z)) == NULL) goto err_0;
 return result;
err_0: _DeeTuple_FreeUnsafe(result);
 return NULL;
}
DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY(__builtin_copysign,m_copysign);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY(__builtin_nextafter,m_nextafter);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY(__builtin_nexttoward,m_nexttoward);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY(__builtin_fdim,m_fdim);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY_BOOL(__builtin_isfinite,m_isfinite)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY_BOOL(__builtin_isinf,m_isinf);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY_BOOL(__builtin_isnan,m_isnan);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY_BOOL(__builtin_isnormal,m_isnormal);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY_BOOL(__builtin_signbit,m_signbit);
DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY_BOOL(__builtin_isgreater,m_isgreater)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY_BOOL(__builtin_isgreaterequal,m_isgreaterequal)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY_BOOL(__builtin_isless,m_isless)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY_BOOL(__builtin_islessequal,m_islessequal)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY_BOOL(__builtin_islessgreater,m_islessgreater)
DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY_BOOL(__builtin_isunordered,m_isunordered)
#undef DEE_M_RT_F
#undef DEE_M_RT_D
#undef DEE_M_RT_L
#undef DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY
#undef DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY
#undef DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY_RET2
#undef DEE_PRIVATE_DECLARE_BUILTIN_MATH_UNARY_BOOL
#undef DEE_PRIVATE_DECLARE_BUILTIN_MATH_BINARY_BOOL

DeeError_NEW_STATIC(_dee_stack_noframe,&DeeErrorType_RuntimeError,"No active stack frame");
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_code) {
 DeeThreadObject *thread_self;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_code") != 0) return NULL;
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL) return NULL;
 if DEE_UNLIKELY(!thread_self->t_frame_last) {
  DeeError_Throw((DeeObject *)&_dee_stack_noframe);
  return NULL;
 }
 DeeReturn_NEWREF(thread_self->t_frame_last->f_code);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_get_stacklimit) {
 if (DeeTuple_Unpack(args,":__builtin_get_stacklimit") != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
 return DeeObject_New(Dee_size_t,Dee_GetStackLimit());
#else
 DeeError_NotImplemented_str("__builtin_get_stacklimit");
 return NULL;
#endif
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_set_stacklimit) {
 Dee_size_t new_limit;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"Iu:__builtin_set_stacklimit",&new_limit) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_STACKLIMIT_CHECKS
 Dee_SetStackLimit(new_limit);
 DeeReturn_None;
#else
 DeeError_NotImplemented_str("__builtin_set_stacklimit");
 return NULL;
#endif
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_FILE) {
 DeeThreadObject *thread_self; DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_FILE") != 0) return NULL;
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL) return NULL;
 if DEE_LIKELY(thread_self->t_frame_last) {
  result = _DeeStackFrame_File(thread_self->t_frame_last);
  if (result) DeeReturn_NEWREF(result);
 }
 DeeReturn_EmptyString;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_LINE) {
 DeeThreadObject *thread_self;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_LINE") != 0) return NULL;
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL) return NULL;
 if DEE_UNLIKELY(!thread_self->t_frame_last) return DeeObject_New(int,-1);
 return DeeObject_New(Dee_int32_t,_DeeStackFrame_Line(thread_self->t_frame_last)+1);
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_FUNCTION) {
 DeeThreadObject *thread_self; DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":__builtin_FUNCTION") != 0) return NULL;
 if DEE_UNLIKELY((thread_self = DeeThread_SELF()) == NULL) return NULL;
 if DEE_LIKELY(thread_self->t_frame_last) {
  result = _DeeStackFrame_Func(thread_self->t_frame_last);
  if (result) DeeReturn_NEWREF(result);
 }
 DeeReturn_EmptyString;
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_bswap16) {
 Dee_uint16_t v;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I16u:__builtin_bswap16",&v) != 0) return NULL;
 return DeeObject_New(Dee_uint16_t,DEE_BUILTIN_BESWAP16(v));
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_bswap32) {
 Dee_uint32_t v;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I32u:__builtin_bswap32",&v) != 0) return NULL;
 return DeeObject_New(Dee_uint32_t,DEE_BUILTIN_BESWAP32(v));
}
DEE_PRIVATE_DEF_BUILTIN_FUNCTION(__builtin_bswap64) {
 Dee_uint64_t v;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64u:__builtin_bswap64",&v) != 0) return NULL;
 return DeeObject_New(Dee_uint64_t,DEE_BUILTIN_BESWAP64(v));
}

#undef DEE_PRIVATE_DEF_BUILTIN_FUNCTION

DEE_DECL_END

#endif /* !GUARD_DEEMON_BUILTIN_FUNCTIONS_C */
