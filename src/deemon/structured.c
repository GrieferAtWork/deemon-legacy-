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
#ifndef GUARD_DEEMON_STRUCTURED_C
#define GUARD_DEEMON_STRUCTURED_C 1
#define DEE_LIMITED_API 1

// /include/*
#include <deemon/__atomic_intrin.inl>
#include <deemon/__conf.inl>
#include <deemon/array.h>
#include <deemon/bool.h>
#include <deemon/cfunction.h>
#include <deemon/deemonrun.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/float.h>
#include <deemon/foreign_function.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/memberdef.h>
#include <deemon/mp/thread.h>
#include <deemon/none.h>
#include <deemon/optional/object_hash.h>
#include <deemon/optional/object_pool.h>
#include <deemon/optional/object_visit.h>
#include <deemon/optional/std/string.h>
#include <deemon/optional/uuid.h>
#include <deemon/struct.h>
#include <deemon/structured.h>
#include <deemon/super.h>
#include <deemon/tuple.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>
#include <deemon/signalhandler.h>
#include DEE_INCLUDE_MEMORY_API()

// */ (nano...)

DEE_DECL_BEGIN

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
DEE_OBJECTPOOL_DEFINE(pointer,DeePointerObject)
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */


#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
DEE_A_RET_EXCEPT(-1) int DeeObject_TGetLValue(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) *tp_result, DEE_A_OUT void **result) {
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(tp_result) && DeeStructuredType_Check(tp_result));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if DEE_LIKELY(DeeLValueType_Check(tp_self)) {
  if DEE_UNLIKELY(!DeeType_IsSameOrDerived(tp_result,DeeLValueType_BASE(tp_self))) {
err:
   DeeError_SetStringf(&DeeErrorType_TypeError,
                       "Expected lvalue compatible with lvalue-to-%#q, but got %q",
                       DeeType_NAME(tp_result),DeeType_NAME(tp_self));
   return -1;
  }
  *result = DeeLValue_ADDR(self);
  return 0;
 } else if DEE_LIKELY(DeeType_IsSameOrDerived(tp_result,tp_self)) {
  // Implicitly cast a structured object to an lvalue of the same type (or basetype)
  *result = DeeStructured_DATA(self);
  return 0;
 } else {
  goto err;
 }
}

#ifndef __INTELLISENSE__
#include "structured.get_pointer_impl.inl"
#define MODE2
#include "structured.get_pointer_impl.inl"
#endif

DEE_COMPILER_MSVC_WARNING_PUSH(4701 4703)
DEE_A_RET_EXCEPT(-1) int DeeObject_TGetPointerEx(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_result, DEE_A_OUT void **result) {
 int temp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(tp_result) && DeeStructuredType_Check(tp_result));
 if DEE_UNLIKELY((temp = DeeObject_TTryGetPointer2(tp_self,self,tp_result,result)) < 0) return -1;
 if DEE_UNLIKELY(temp > 0) {
  DeeError_SetStringf(&DeeErrorType_TypeError,
                      "Expected pointer compatible with pointer-to-%#q, but got: %q",
                      DeeType_NAME(tp_result),DeeType_NAME(tp_self));
  return -1;
 }
 return 0;
}
DEE_COMPILER_MSVC_WARNING_POP
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */



DEE_STATIC_ASSERT(sizeof(DeeLValueObject) == sizeof(DeePointerObject));
#if defined(_MSC_VER) && !defined(__INTELLISENSE__)
// Make sure that the linked list of semi-structured types
// has binary compatibility with that of actually structured types.
DEE_STATIC_ASSERT(
 Dee_OFFSETOF(DeeLValueTypeObject,ltp_prev_structured_object_type) ==
 Dee_OFFSETOF(DeeStructuredTypeObject,stp_prev_structured_object_type));
DEE_STATIC_ASSERT(
 Dee_OFFSETOF(DeeVarArrayTypeObject,vat_prev_structured_object_type) ==
 Dee_OFFSETOF(DeeStructuredTypeObject,stp_prev_structured_object_type));
#endif

static DeePointerObject *DEE_CALL _deepointer_tp_alloc(DeePointerTypeObject *tp) {
 DeePointerObject *result = DEE_OBJECTPOOL_ALLOC(pointer);
 if (result) DeeObject_INIT(result,(DeeTypeObject *)tp);
 return result;
}
static void DEE_CALL _deepointer_tp_free(
 DeePointerTypeObject *DEE_UNUSED(tp), DeePointerObject *ob) {
 DEE_OBJECTPOOL_FREE(pointer,ob);
}
static int _deepointer_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeePointerObject *self) {
 DeePointer_ADDR(self) = NULL;
 return 0;
}
static int _deepointer_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeePointerObject *self, DeeObject *right) {
 DeePointer_ADDR(self) = DeePointer_ADDR(right);
 return 0;
}
static int _deepointer_tp_copy_assign(
 DeePointerObject *self, DeeObject *right) {
 DeePointer_ADDR(self) = DeePointer_ADDR(right);
 return 0;
}
static int _deepointer_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeePointerObject *self, DeeObject *args) {
 DeeObject *arg0; int temp;
 if (DeeTuple_Unpack(args,"o:pointer",&arg0) == -1) return -1;
 if ((temp = DeeObject_TryGetPointer(arg0,(DeeTypeObject *)DeeType_GET_SLOT(
  Dee_TYPE(self),tp_ptr_pointer_base),&DeePointer_ADDR(self))) != 1) return temp;
 return DeeObject_Cast(Dee_uintptr_t,arg0,(Dee_uintptr_t *)&DeePointer_ADDR(self));
}
static int _deepointer_tp_any_assign(
 DeePointerObject *self, DeeObject *right) {
 return DeeObject_GetPointerEx(right,(DeeTypeObject *)DeeType_GET_SLOT(
  Dee_TYPE(self),tp_ptr_pointer_base),&DeePointer_ADDR(self));
}
static int _deepointer_tp_p_any_ctor(
 DeePointerTypeObject *tp, void **self, DeeObject *args) {
 DeeObject *arg0; int temp;
 if (DeeTuple_Unpack(args,"o:pointer",&arg0) == -1) return -1;
 if ((temp = DeeObject_TryGetPointer(arg0,(
  DeeTypeObject *)tp->tp_ptr_pointer_base,self)) != 1) return temp;
 return DeeObject_Cast(Dee_uintptr_t,arg0,(Dee_uintptr_t *)&DeePointer_ADDR(self));
}
static int _deepointer_tp_p_any_assign(
 DeePointerTypeObject *tp, Dee_uintptr_t *self, DeeObject *right) {
 return DeeObject_GetPointerEx(right,(
  DeeTypeObject *)DeeType_GET_SLOT(
  tp,tp_ptr_pointer_base),(void **)self);
}
static int _deelvalue_tp_ctor(DeeLValueTypeObject *tp_self, DeeLValueObject *self);
static int _deelvalue_tp_any_ctor(DeeLValueTypeObject *tp_self, DeeLValueObject *self, DeeObject *args);
static int _deelvalue_tp_any_assign(DeeLValueObject *self, DeeObject *right);

Dee_size_t DeePointer_ClearPool(void) {
 return DEE_OBJECTPOOL_CLEAR(pointer);
}
#endif







static struct DeeAtomicMutex _structuretype_poollock = DeeAtomicMutex_INIT();
// NOTE: This pool also contains 'DeeLValueTypeObject' and 'DeeVarArrayTypeObject', whose
//       data structure is incompatible with 'DeeStructuredTypeObject'
//       >> So if you want to modify anything beyond the linked
//          list pointers, you have to check your types!
static DeeStructuredTypeObject *_structuretype_pool;

void _DeeStructureType_AddToTypePool_AlreadyLocked(DeeTypeObject *tp) {
 DEE_ASSERT(DeeObject_Check(tp));
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS && DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 DEE_ASSERTF(DeeStructuredType_Check(tp) || DeeLValueType_Check(tp) || DeeVarArrayType_Check(tp),
             "tp must be a structured_object_type, lvalue_type or vararray_type");
#elif DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DEE_ASSERTF(DeeStructuredType_Check(tp) || DeeLValueType_Check(tp),
             "tp must be a structured_object_type or lvalue_type");
#elif DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 DEE_ASSERTF(DeeStructuredType_Check(tp) || DeeVarArrayType_Check(tp),
             "tp must be a structured_object_type or vararray_type");
#else
 DEE_ASSERTF(DeeStructuredType_Check(tp),
             "tp must be a structured_object_type");
#endif
 ((DeeStructuredTypeObject *)tp)->stp_prev_structured_object_type = _structuretype_pool;
 _structuretype_pool = ((DeeStructuredTypeObject *)tp);
}

#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
void DeeForeignFunctionTypeList_Init(
 DEE_A_OUT struct DeeForeignFunctionTypeList *self) {
 DeeAtomicMutex_Init(&self->fftl_lock);
 self->fftl_typec = 0;
 self->fftl_typev = NULL;
}
void DeeForeignFunctionTypeList_Quit(
 DEE_A_IN struct DeeForeignFunctionTypeList *self) {
 free(self->fftl_typev);
}
void DeeForeignFunctionTypeList_Clear(
 DEE_A_IN struct DeeForeignFunctionTypeList *self) {
 if (self->fftl_typec) {
  free_nn(self->fftl_typev);
  self->fftl_typec = 0;
  self->fftl_typev = NULL;
 }
}
DEE_STATIC_INLINE(DeeForeignFunctionTypeObject *) _DeeForeignFunctionTypeList_Find_AlreadyLocked(
 struct DeeForeignFunctionTypeList *self, DEE_A_IN Dee_funflags_t flags,
#ifdef DEE_DEBUG
 DeeTypeObject const *return_type,
#endif
 Dee_size_t argc, DeeTypeObject const *const *argv) {
 DeeForeignFunctionTypeObject *result,**iter,**end;
 DeeTypeObject const *const *argv_end;
 DEE_ASSERT(self);
 argv_end = argv+argc;
 end = (iter = self->fftl_typev)+self->fftl_typec;
 while (iter != end) {
  result = *iter++;
  DEE_ASSERT(DeeObject_Check(result) && DeeForeignFunctionType_Check(result));
#ifdef DEE_DEBUG
  DEE_ASSERT(result->fft_return_type == return_type && "Invalid type in foreign_function list");
#endif
  if (result->fft_arg_type_c == argc && result->fft_flags == flags) {
   DeeTypeObject const *const *arg_iter,*const *argv_iter; // Candidate!
   argv_iter = argv,arg_iter = (DeeTypeObject const *const *)result->fft_arg_type_v;
   // NOTE: Make sure to compare against the actually used argument type,
   //       thus correcting type substitutions and variable array types
   while (argv_iter != argv_end) {
    DeeTypeObject *temp_type;
    temp_type = _DeeForeignFunctionType_ArgTypeof((DeeTypeObject *)*argv_iter);
    if (!temp_type) DeeError_Handled(); // No way to report...
    if (temp_type != *arg_iter) goto no_match;
    ++argv_iter,++arg_iter;
   }
   return result; // We've got a match!
  }
no_match:;
 }
 return NULL;
}

DEE_A_RET_EXCEPT(NULL) struct DeeForeignFunctionTypeObject *
DeeForeignFunctionTypeList_FindOrAddToGlobalCache(
 DEE_A_INOUT struct DeeForeignFunctionTypeList *self,
 DEE_A_IN Dee_funflags_t flags, DEE_A_IN DeeTypeObject const *return_type,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeTypeObject const *const *argv) {
 DeeForeignFunctionTypeObject *result,*temp;
 DEE_ASSERT(self);
 DeeAtomicMutex_AcquireRelaxed(&self->fftl_lock);
#ifdef DEE_DEBUG
 result = _DeeForeignFunctionTypeList_Find_AlreadyLocked(self,flags,return_type,argc,argv);
#else
 result = _DeeForeignFunctionTypeList_Find_AlreadyLocked(self,flags,argc,argv);
#endif
 DeeAtomicMutex_Release(&self->fftl_lock);
 if (DEE_UNLIKELY(!result)) { // Arguably unlikely...
  if ((result = (DeeForeignFunctionTypeObject *)_DeeForeignFunctionType_New(
   flags,return_type,argc,argv)) == NULL) return NULL; // Error
add_new_ff:
  DeeAtomicMutex_AcquireRelaxed(&self->fftl_lock);
#ifdef DEE_DEBUG
  temp = _DeeForeignFunctionTypeList_Find_AlreadyLocked(self,flags,return_type,argc,argv);
#else
  temp = _DeeForeignFunctionTypeList_Find_AlreadyLocked(self,flags,argc,argv);
#endif
  if (DEE_LIKELY(!temp)) {
   DeeForeignFunctionTypeObject **new_list;
   // Likely case: Insert our new type into the list
   if ((new_list = (DeeForeignFunctionTypeObject **)realloc_nz(
    self->fftl_typev,(self->fftl_typec+1)*sizeof(DeeForeignFunctionTypeObject *))
    ) == NULL) { // Failed to reallocate list
    DeeAtomicMutex_Release(&self->fftl_lock);
    if DEE_LIKELY(Dee_CollectMemory()) goto add_new_ff;
    DeeError_NoMemory();
    Dee_DECREF(result);
    return NULL;
   }
   // Register a reference to the new type
   (self->fftl_typev = new_list)[self->fftl_typec++] = result;
   DeeAtomicMutex_Release(&self->fftl_lock);
   // At this point, we can be sure that nobody will ever try to create
   // this exact same type again. So we can safely take our time and
   // only have to add the type to the global cache to tell it that it's
   // responsible for cleaning up the reference later.
   DeeAtomicMutex_AcquireRelaxed(&_structuretype_poollock);
   _DeeStructureType_AddToTypePool_AlreadyLocked((DeeTypeObject *)result);
   DeeAtomicMutex_Release(&_structuretype_poollock);
  } else {
   // Unlikely case: Someone else created and inserted the same type while we were creating
   // >> Solution: Destroy what we just created and simply return what the other guy did
   DeeAtomicMutex_Release(&self->fftl_lock);
   Dee_DECREF(result);
   result = temp;
  }
 }
 return result;
}
#endif


#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
void DeeArrayTypeList_Init(DEE_A_OUT struct DeeArrayTypeList *self) {
 DeeAtomicMutex_Init(&self->atl_lock);
 self->atl_typec = 0;
 self->atl_typev = NULL;
}
void DeeArrayTypeList_Quit(DEE_A_IN struct DeeArrayTypeList *self) {
 free(self->atl_typev);
}
void DeeArrayTypeList_Clear(DEE_A_IN struct DeeArrayTypeList *self) {
 if (self->atl_typec) {
  free_nn(self->atl_typev);
  self->atl_typec = 0;
  self->atl_typev = NULL;
 }
}

DEE_STATIC_INLINE(DeeArrayTypeObject const *)
_DeeArrayTypeList_Find_AlreadyLocked(
 struct DeeArrayTypeList *self,
#ifdef DEE_DEBUG
 DeeStructuredTypeObject const *elem_type,
#endif
 Dee_size_t n_elem) {
 DeeArrayTypeObject const *result,*const *iter,*const *end;
 DEE_ASSERT(self);
 end = (iter = (DeeArrayTypeObject const *const *)self->atl_typev)+self->atl_typec;
 while (iter != end) {
  result = *iter++;
  DEE_ASSERT(DeeObject_Check(result) && DeeArrayType_Check(result));
#ifdef DEE_DEBUG
  DEE_ASSERT(result->at_array_base == elem_type && "Invalid type in array_type list");
#endif
  if (result->at_array_elem_count == n_elem) return result; // We've got a match!
 }
 return NULL;
}
DEE_A_RET_EXCEPT(NULL) struct DeeArrayTypeObject *DeeArrayTypeList_FindOrAddToCache(
 DEE_A_INOUT struct DeeArrayTypeList *self,
 DEE_A_IN DeeStructuredTypeObject const *elem_type, DEE_A_IN Dee_size_t n_elem) {
 DeeArrayTypeObject *result,*temp;
 DEE_ASSERT(self);
 DeeAtomicMutex_AcquireRelaxed(&self->atl_lock);
#ifdef DEE_DEBUG
 result = (DeeArrayTypeObject *)_DeeArrayTypeList_Find_AlreadyLocked(self,elem_type,n_elem);
#else
 result = (DeeArrayTypeObject *)_DeeArrayTypeList_Find_AlreadyLocked(self,n_elem);
#endif
 DeeAtomicMutex_Release(&self->atl_lock);
 if (DEE_UNLIKELY(!result)) { // Arguably unlikely...
  result = (DeeArrayTypeObject *)_DeeArrayType_New((DeeStructuredTypeObject *)elem_type,n_elem);
  if (!result) return NULL; // Error
add_new_at:
  DeeAtomicMutex_AcquireRelaxed(&self->atl_lock);
#ifdef DEE_DEBUG
  temp = (DeeArrayTypeObject *)_DeeArrayTypeList_Find_AlreadyLocked(self,elem_type,n_elem);
#else
  temp = (DeeArrayTypeObject *)_DeeArrayTypeList_Find_AlreadyLocked(self,n_elem);
#endif
  if (DEE_LIKELY(!temp)) {
   DeeArrayTypeObject **new_list;
   // Likely case: Insert our new type into the list
   if ((new_list = (DeeArrayTypeObject **)realloc_nz(
    self->atl_typev,(self->atl_typec+1)*sizeof(DeeArrayTypeObject *))
    ) == NULL) { // Failed to reallocate list
    DeeAtomicMutex_Release(&self->atl_lock);
    if DEE_LIKELY(Dee_CollectMemory()) goto add_new_at;
    DeeError_NoMemory();
    Dee_DECREF(result);
    return NULL;
   }
   // Register a reference to the new type
   (self->atl_typev = new_list)[self->atl_typec++] = result;
   DeeAtomicMutex_Release(&self->atl_lock);
   // At this point, we can be sure that nobody will ever try to create
   // this exact same type again. So we can safely take our time and
   // only have to add the type to the global cache to tell it that it's
   // responsible for cleaning up the reference later.
   DeeAtomicMutex_AcquireRelaxed(&_structuretype_poollock);
   _DeeStructureType_AddToTypePool_AlreadyLocked((DeeTypeObject *)result);
   DeeAtomicMutex_Release(&_structuretype_poollock);
  } else {
   // Unlikely case: Someone else created and inserted the same type while we were creating
   // >> Solution: Destroy what we just created and simply return what the other guy did
   DeeAtomicMutex_Release(&self->atl_lock);
   Dee_DECREF(result);
   result = temp;
  }
 }
 return result;
}
#endif



#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
struct DeeForeignFunctionTypeList _generic_foreign_function_list = DeeForeignFunctionTypeList_INIT();
DEE_STATIC_INLINE(DeeForeignFunctionTypeObject *) _DeeForeignFunctionTypeList_FindDefault_AlreadyLocked(
 DEE_A_IN Dee_funflags_t flags, DeeTypeObject const *return_type, Dee_size_t argc, DeeTypeObject const *const *argv) {
 DeeForeignFunctionTypeObject *result,**iter,**end;
 DeeTypeObject const *const *argv_end;
 argv_end = argv+argc;
 end = (iter = _generic_foreign_function_list.fftl_typev)+_generic_foreign_function_list.fftl_typec;
 while (iter != end) {
  result = *iter++;
  DEE_ASSERT(DeeObject_Check(result) && DeeForeignFunctionType_Check(result));
  if (result->fft_return_type == return_type &&
      result->fft_arg_type_c == argc && result->fft_flags == flags) {
   DeeTypeObject const *const *arg_iter,*const *argv_iter; // Candidate!
   argv_iter = argv,arg_iter = (DeeTypeObject const *const *)result->fft_arg_type_v;
   while (argv_iter != argv_end) {
    DeeTypeObject *temp_type;
    temp_type = _DeeForeignFunctionType_ArgTypeof((DeeTypeObject *)*argv_iter);
    if (!temp_type) DeeError_Handled(); // No way to report...
    if (temp_type != *arg_iter) goto no_match;
    ++argv_iter,++arg_iter;
   }
   return result; // We've got a match!
  }
no_match:;
 }
 return NULL;
}

DEE_A_RET_EXCEPT(NULL) struct DeeForeignFunctionTypeObject *
DeeForeignFunctionTypeList_FindOrAddToGlobalCacheDefault(
 DEE_A_IN Dee_funflags_t flags, DEE_A_IN DeeTypeObject const *return_type,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeTypeObject const *const *argv) {
 DeeForeignFunctionTypeObject *result,*temp;
 DeeAtomicMutex_AcquireRelaxed(&_generic_foreign_function_list.fftl_lock);
 result = _DeeForeignFunctionTypeList_FindDefault_AlreadyLocked(flags,return_type,argc,argv);
 DeeAtomicMutex_Release(&_generic_foreign_function_list.fftl_lock);
 if DEE_UNLIKELY(!result) { // Arguably unlikely...
  if DEE_UNLIKELY((result = (DeeForeignFunctionTypeObject *)_DeeForeignFunctionType_New(
   flags,return_type,argc,argv)) == NULL) return NULL; // Error
add_new_ff:
  DeeAtomicMutex_AcquireRelaxed(&_generic_foreign_function_list.fftl_lock);
  temp = _DeeForeignFunctionTypeList_FindDefault_AlreadyLocked(flags,return_type,argc,argv);
  if DEE_LIKELY(!temp) {
   DeeForeignFunctionTypeObject **new_list;
   // Likely case: Insert our new type into the list
   if DEE_UNLIKELY((new_list = (DeeForeignFunctionTypeObject **)realloc_nz(
    _generic_foreign_function_list.fftl_typev,
    (_generic_foreign_function_list.fftl_typec+1)*sizeof(DeeForeignFunctionTypeObject *))
    ) == NULL) { // Failed to reallocate list
    DeeAtomicMutex_Release(&_generic_foreign_function_list.fftl_lock);
    if DEE_LIKELY(Dee_CollectMemory()) goto add_new_ff;
    DeeError_NoMemory();
    Dee_DECREF(result);
    return NULL;
   }
   // Register a reference to the new type
   (_generic_foreign_function_list.fftl_typev = new_list)[
    _generic_foreign_function_list.fftl_typec++] = result;
   DeeAtomicMutex_Release(&_generic_foreign_function_list.fftl_lock);
   // At this point, we can be sure that nobody will ever try to create
   // this exact same type again. So we can safely take our time and
   // only have to add the type to the global cache to tell it that it's
   // responsible for cleaning up the reference later.
   DeeAtomicMutex_AcquireRelaxed(&_structuretype_poollock);
   _DeeStructureType_AddToTypePool_AlreadyLocked((DeeTypeObject *)result);
   DeeAtomicMutex_Release(&_structuretype_poollock);
  } else {
   // Unlikely case: Someone else created and inserted the same type while we were creating
   // >> Solution: Destroy what we just created and simply return what the other guy did
   DeeAtomicMutex_Release(&_generic_foreign_function_list.fftl_lock);
   Dee_DECREF(result);
   result = temp;
  }
 }
 return result;
}
#endif




//////////////////////////////////////////////////////////////////////////
// List of all static instances of structured types (none, bool, int, ...)
// >> Required to clear all foreign_function caches
// NOTE: Technically we'd have to keep a list similar to this for all static
//       lvalue_type objects, since DeeLValueTypeObject has an tp_ffunctions member as well.
//       But there are no such objects, so we don't need it.
#define _deestructuredtype_static_instance_list_end \
 _deestructuredtype_static_instance_list+(\
 sizeof(_deestructuredtype_static_instance_list)/\
 sizeof(*_deestructuredtype_static_instance_list))
static DeeStructuredTypeObject *const _deestructuredtype_static_instance_list[] = {
 &DeeNone_Type,
 //&DeeBool_Type,
 &DeeFloat_Type,
 &DeeDouble_Type,
 &DeeLDouble_Type,
 &DeeInt8_Type,
 &DeeInt16_Type,
 &DeeInt32_Type,
 &DeeInt64_Type,
 &DeeUInt8_Type,
 &DeeUInt16_Type,
 &DeeUInt32_Type,
 &DeeUInt64_Type,
 &DeeAtomicInt8_Type,
 &DeeAtomicInt16_Type,
 &DeeAtomicInt32_Type,
 &DeeAtomicInt64_Type,
 &DeeAtomicUInt8_Type,
 &DeeAtomicUInt16_Type,
 &DeeAtomicUInt32_Type,
 &DeeAtomicUInt64_Type,
};

void DeeStructuredType_Shutdown(void) {
 DeeStructuredTypeObject *iter,*prev;
 //DeeAtomicMutex_Acquire(&_structuretype_poollock);
 if ((iter = _structuretype_pool) != NULL) {
  _structuretype_pool = NULL;
  while (1) {
   prev = iter->stp_prev_structured_object_type;
   Dee_DECREF(iter);
   if (!prev) break;
   iter = prev;
  }
 }
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
 // Re-initialize all static foreign_function lists
 DeeForeignFunctionTypeList_Clear(&_generic_foreign_function_list);
#endif
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS || \
    DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION || \
    DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 {
  DeeStructuredTypeObject *const *iter2,*elem;
  iter2 = _deestructuredtype_static_instance_list;
  while (iter2 != _deestructuredtype_static_instance_list_end) {
   elem = *iter2++;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   elem->stp_pointer = NULL;
   elem->stp_lvalue = NULL;
#endif
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
   DeeForeignFunctionTypeList_Clear(&elem->stp_ffunctions);
#endif
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
   DeeArrayTypeList_Clear(&elem->stp_arrays);
   elem->stp_varray = NULL;
#endif
  }
 }
#endif
}



#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
DeeError_NEW_STATIC(_deecan_only_ref_structured,&DeeErrorType_TypeError,
                   "Can only reference structured objects");
DEE_A_RET_OBJECT_EXCEPT_REF(DeePointerObject) *DeeObject_TRef(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeePointerObject *result;
 DeePointerTypeObject *ty;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (DeeStructuredType_Check(tp_self)) {
  // Reference an structured object
  if DEE_UNLIKELY((ty = (DeePointerTypeObject *)DeeStructuredType_Pointer(tp_self)) == NULL) return NULL;
  if DEE_LIKELY((result = DEE_OBJECTPOOL_ALLOC(pointer)) != NULL) {
   DeeObject_INIT(result,(DeeTypeObject *)ty);
   result->po_p_ptr = DeeStructured_ADDR(self);
  }
 } else if (DeeLValueType_Check(tp_self)) {
  // Reference an lvalue
  if DEE_UNLIKELY((ty = (DeePointerTypeObject *)DeeLValueType_Pointer(tp_self)) == NULL) return NULL;
  if DEE_LIKELY((result = DEE_OBJECTPOOL_ALLOC(pointer)) != NULL) {
   DeeObject_INIT(result,(DeeTypeObject *)ty);
   result->po_p_ptr = DeeLValue_ADDR(self);
  }
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
 } else if (tp_self == &DeeForeignFunctionClosure_Type) {
  if DEE_UNLIKELY((ty = (DeePointerTypeObject *)DeeStructuredType_Pointer(
   DeeForeignFunctionClosure_TYPE(self))) == NULL) return NULL;
  if DEE_LIKELY((result = DEE_OBJECTPOOL_ALLOC(pointer)) != NULL) {
   DeeObject_INIT(result,(DeeTypeObject *)ty);
DEE_COMPILER_MSVC_WARNING_PUSH(4054)
   result->po_p_ptr = (void *)DeeForeignFunctionClosure_FUNC(self);
DEE_COMPILER_MSVC_WARNING_POP
  }
#endif
 } else {
  DeeError_Throw((DeeObject *)&_deecan_only_ref_structured);
  return NULL;
 }
 return (DeeObject *)result;
}

DeeError_NEW_STATIC(_deecan_only_deref_pointers,&DeeErrorType_TypeError,
                   "Can only dereference pointer objects");
DEE_A_RET_OBJECT_EXCEPT_REF(DeeLValueObject) *DeeObject_TDeref(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeLValueTypeObject *ty;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if DEE_LIKELY(DeePointerType_Check(tp_self)) {
  // Dereference a pointer
  if DEE_UNLIKELY((ty = (DeeLValueTypeObject *)DeeStructuredType_LValue((DeeTypeObject *)
   ((DeePointerTypeObject *)tp_self)->tp_ptr_pointer_base)) == NULL) return NULL;
  return DeeLValue_NewWithError((DeeTypeObject *)ty,DeePointer_ADDR(self));
 }
 if DEE_LIKELY(DeeLValueType_Check(tp_self)) {
  // Dereference an lvaue to a pointer
  DeeStructuredTypeObject *lvalue_base = (
   (DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;
  if DEE_UNLIKELY(!DeePointerType_Check(lvalue_base)) goto err; // Not a pointer lvalue
  if DEE_UNLIKELY((ty = (DeeLValueTypeObject *)DeeStructuredType_LValue((DeeTypeObject *)
   ((DeePointerTypeObject *)lvalue_base)->tp_ptr_pointer_base)) == NULL) return NULL;
  // Dereference once lvalue(void *) --> internally: void **
  //              >> *lvalue(void *) --> internally: *(void **)
  return DeeLValue_NewWithError((DeeTypeObject *)ty,*(void **)DeeLValue_ADDR(self));
 }
err:
 DeeError_Throw((DeeObject *)&_deecan_only_deref_pointers);
 return NULL;
}

DeeError_NEW_STATIC(_deecan_only_lvalue_structured,&DeeErrorType_TypeError,
                   "Can only create lvalue for structured objects");
DEE_A_RET_OBJECT_EXCEPT_REF(DeeLValueObject) *DeeObject_TLValue(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeLValueObject *result; DeeLValueTypeObject *ty;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 // Generate an lvalue for a structured object
 if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self)) {
  DeeError_Throw((DeeObject *)&_deecan_only_lvalue_structured);
  return NULL;
 }
 if DEE_UNLIKELY((ty = (DeeLValueTypeObject *)
  DeeStructuredType_LValue(tp_self)) == NULL) return NULL;
 if DEE_LIKELY((result = (DeeLValueObject *)DEE_OBJECTPOOL_ALLOC(pointer)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)ty);
  result->lv_p_ptr = DeeStructured_ADDR(self);
 }
 return (DeeObject *)result;
}



static char *_DeePointer_AllocName(char const *base_name, char symb) {
 char *new_name; char const *iter; Dee_size_t old_name_len,new_name_len;
 if (!base_name) base_name = "unnamed_structured_object";
 old_name_len = strlen(base_name);
 iter = base_name;
 while (*iter) {
  if (*iter == '(') {
   char *temp;
   if (iter[1] == '*' || iter[1] == '&') {
    iter += 2;
    while (*iter == '*' || *iter == '&') ++iter;
    while ((new_name = (char *)malloc_nz((old_name_len+2)*sizeof(char))) == NULL) {
     if DEE_LIKELY(Dee_CollectMemory()) continue;
     goto no_mem;
    }
    new_name[old_name_len+1] = '\0';
    memcpy(new_name,base_name,(Dee_size_t)(iter-base_name)*sizeof(char));
    temp = new_name+(Dee_size_t)(iter-base_name);
    *temp++ = symb;
    memcpy(temp,iter,(old_name_len-(Dee_size_t)(iter-base_name))*sizeof(char));
   } else {
    if ((new_name = (char *)malloc_nz((old_name_len+4)*sizeof(char))) == NULL) {
     if DEE_LIKELY(Dee_CollectMemory()) continue;
     goto no_mem;
    }
    new_name[old_name_len+3] = '\0';
    memcpy(new_name,base_name,(Dee_size_t)(iter-base_name)*sizeof(char));
    temp = new_name+(Dee_size_t)(iter-base_name);
    *temp++ = '(';
    *temp++ = symb;
    *temp++ = ')';
    memcpy(temp,iter,(old_name_len-(Dee_size_t)(iter-base_name))*sizeof(char));
   }
   return new_name;
  }
  ++iter;
 }
 new_name_len = old_name_len+1;
 if (base_name[old_name_len-1] != '*') ++new_name_len;
 while ((new_name = (char *)malloc_nz((new_name_len+1)*sizeof(char))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
no_mem: DeeError_NoMemory(); return NULL;
 }
 new_name[new_name_len] = '\0';
 memcpy(new_name,base_name,old_name_len);
 if (base_name[old_name_len-1] != '*')
  new_name[old_name_len++] = ' ';
 new_name[old_name_len] = symb;
 return new_name;
}



static DeeLValueTypeObject *_DeeLValueType_New(
 DeeStructuredTypeObject *lvalue_base) {
 DeeLValueTypeObject *result;
 DEE_ASSERT(DeeObject_Check(lvalue_base) && DeeStructuredType_Check(lvalue_base));
 if ((result = DeeObject_CALLOCF(DeeLValueTypeObject,
  "type:lvalue_of(%s)",DeeType_NAME(lvalue_base))) != NULL) {
  char *name;
  DeeObject_INIT(result,&DeeLValueType_Type);
  name = _DeePointer_AllocName(DeeType_NAME(lvalue_base),'&');
  if (!name) { _DeeObject_DELETE(&DeeLValueType_Type,result); return NULL; }
  DeeType_GET_SLOT(result,tp_name) = name;
  DeeType_GET_SLOT(result,tp_flags) =
   DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|
   DEE_TYPE_FLAG_STATIC_DOC|DEE_TYPE_FLAG_HEAP_TYPE|
   DEE_TYPE_FLAG_MUST_COPY|DEE_TYPE_FLAG_CONST_CTOR|
   DEE_TYPE_FLAG_NO_SUBCLASS;
  // Set the base to the common pointer base class
  // >> With this simple call, we inherit all the pointer arithmetic we need
  Dee_INCREF(DeeType_GET_SLOT(result,tp_base) = (DeeTypeObject *)&DeeLValue_Type);
  Dee_INCREF(DeeType_GET_SLOT(result,tp_lval_lvalue_base) = lvalue_base);
  DeeType_GET_SLOT(result,tp_instance_size) = sizeof(DeeLValueObject);
  DeeType_GET_SLOT(result,tp_members) = DeeType_DEFAULT_SLOT(tp_members);
  DeeType_GET_SLOT(result,tp_getsets) = DeeType_DEFAULT_SLOT(tp_getsets);
  DeeType_GET_SLOT(result,tp_methods) = DeeType_DEFAULT_SLOT(tp_methods);
  DeeType_GET_SLOT(result,tp_class_members) = DeeType_DEFAULT_SLOT(tp_class_members);
  DeeType_GET_SLOT(result,tp_class_getsets) = DeeType_DEFAULT_SLOT(tp_class_getsets);
  DeeType_GET_SLOT(result,tp_class_methods) = DeeType_DEFAULT_SLOT(tp_class_methods);
  // NOTE: We can use the pointer pool, since
  //       lvalues have the same size as pointers
  DeeType_GET_SLOT(result,tp_alloc) = (DeeObject*(DEE_CALL*)(DeeTypeObject*))&_deepointer_tp_alloc;
  DeeType_GET_SLOT(result,tp_free) = (void(DEE_CALL*)(DeeTypeObject*,DeeObject*))&_deepointer_tp_free;
  DeeType_GET_SLOT(result,tp_ctor) = (int(*)(DeeTypeObject*,DeeObject*))&_deelvalue_tp_ctor;
  DeeType_GET_SLOT(result,tp_copy_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deepointer_tp_copy_ctor;
  DeeType_GET_SLOT(result,tp_move_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deepointer_tp_copy_ctor;
  DeeType_GET_SLOT(result,tp_any_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deelvalue_tp_any_ctor;
  DeeType_GET_SLOT(result,tp_copy_assign) = (int(*)(DeeObject*,DeeObject*))&_deelvalue_tp_any_assign;
  DeeType_GET_SLOT(result,tp_move_assign) = (int(*)(DeeObject*,DeeObject*))&_deelvalue_tp_any_assign;
  // v Must be set manually, since assign isn't an operator that can be inherited
  DeeType_GET_SLOT(result,tp_any_assign) = (int(*)(DeeObject*,DeeObject*))&_deelvalue_tp_any_assign;
 }
 return result;
}

static DeePointerTypeObject *_DeePointerType_New(
 DeeStructuredTypeObject *pointer_base) {
 DeePointerTypeObject *result;
 // v The calloc initializes most members
 if ((result = DeeObject_CALLOCF(DeePointerTypeObject,
  "type:pointer_to(%s)",DeeType_NAME(pointer_base))) != NULL) {
  char *name;
  DeeObject_INIT(result,&DeePointerType_Type);
  name = _DeePointer_AllocName(DeeType_NAME(pointer_base),'*');
  if (!name) { _DeeObject_DELETE(&DeePointerType_Type,result); return NULL; }
  DeeType_GET_SLOT(result,tp_name) = name;
  DeeType_GET_SLOT(result,tp_flags) =
   DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|
   DEE_TYPE_FLAG_STATIC_DOC|DEE_TYPE_FLAG_HEAP_TYPE|
   DEE_TYPE_FLAG_MUST_COPY|DEE_TYPE_FLAG_CONST_CTOR|
   DEE_TYPE_FLAG_NO_SUBCLASS;
  // Set the base to the common pointer base class
  // >> With this simple call, we inherit all the pointer arithmetic we need
  Dee_INCREF(DeeType_GET_SLOT(result,tp_base) = (DeeTypeObject *)&DeePointer_Type);
  Dee_INCREF(DeeType_GET_SLOT(result,tp_ptr_pointer_base) = pointer_base);
  DeeType_GET_SLOT(result,tp_ptr_elem_size) = DeeType_GET_SLOT(pointer_base,tp_p_instance_size);
  DeeType_GET_SLOT(result,tp_instance_size) = sizeof(DeePointerObject);
  DeeType_GET_SLOT(result,tp_p_instance_size) = sizeof(void *);
  DeeType_GET_SLOT(result,tp_members) = DeeType_DEFAULT_SLOT(tp_members);
  DeeType_GET_SLOT(result,tp_getsets) = DeeType_DEFAULT_SLOT(tp_getsets);
  DeeType_GET_SLOT(result,tp_methods) = DeeType_DEFAULT_SLOT(tp_methods);
  DeeType_GET_SLOT(result,tp_class_members) = DeeType_DEFAULT_SLOT(tp_class_members);
  DeeType_GET_SLOT(result,tp_class_getsets) = DeeType_DEFAULT_SLOT(tp_class_getsets);
  DeeType_GET_SLOT(result,tp_class_methods) = DeeType_DEFAULT_SLOT(tp_class_methods);
  DeeType_GET_SLOT(result,tp_alloc) = (DeeObject*(DEE_CALL*)(DeeTypeObject*))&_deepointer_tp_alloc;
  DeeType_GET_SLOT(result,tp_free) = (void(DEE_CALL*)(DeeTypeObject*,DeeObject*))&_deepointer_tp_free;
  DeeType_GET_SLOT(result,tp_ctor) = (int(*)(DeeTypeObject*,DeeObject*))&_deepointer_tp_ctor;
  DeeType_GET_SLOT(result,tp_copy_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deepointer_tp_copy_ctor;
  DeeType_GET_SLOT(result,tp_move_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deepointer_tp_copy_ctor;
  DeeType_GET_SLOT(result,tp_any_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deepointer_tp_any_ctor;
  DeeType_GET_SLOT(result,tp_copy_assign) = (int(*)(DeeObject*,DeeObject*))&_deepointer_tp_copy_assign;
  DeeType_GET_SLOT(result,tp_move_assign) = (int(*)(DeeObject*,DeeObject*))&_deepointer_tp_copy_assign;
  DeeType_GET_SLOT(result,tp_any_assign) = (int(*)(DeeObject*,DeeObject*))&_deepointer_tp_any_assign;
  DeeType_GET_SLOT(result,tp_p_any_ctor) = (int(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deepointer_tp_p_any_ctor;
  DeeType_GET_SLOT(result,tp_p_any_assign) = (int(*)(DeeStructuredTypeObject*,void*,DeeObject*))&_deepointer_tp_p_any_assign;
  // Just copy all the other 'tp_p_*' operators from the pointer base class
  // >> This has to be done manually, as structured types don't support inheritance
  memcpy(&result->ptp_type.tp_p_assign,&DeePointer_Type.ptp_type.tp_p_assign,
         sizeof(DeeStructuredTypeObject)-Dee_OFFSETOF(DeeStructuredTypeObject,tp_p_assign));
 }
 return result;
}



DEE_A_RET_TYPEOBJECT_EXCEPT(DeeLValueTypeObject) *DeeStructuredType_LValue(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *self) {
 DeeLValueTypeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructuredType_Check(self));
 if DEE_UNLIKELY((result = (DeeLValueTypeObject *)DeeAtomicPtr_Load(
  ((DeeStructuredTypeObject *)self)->stp_lvalue,memory_order_seq_cst)) == NULL) {
  // Must create a new lvalue type
  result = _DeeLValueType_New((DeeStructuredTypeObject *)self);
  DeeAtomicMutex_Acquire(&_structuretype_poollock);
  if DEE_LIKELY(result) {
   DeeLValueTypeObject *temp;
   // Reload the slot to see if someone else already create this type
   if DEE_LIKELY((temp = (DeeLValueTypeObject *)DeeAtomicPtr_Load(
    ((DeeStructuredTypeObject *)self)->stp_lvalue,memory_order_seq_cst)) == NULL) {
    // Store the pointer type in its rightful place
    DeeAtomicPtr_Store(((DeeStructuredTypeObject *)self)->stp_lvalue,result,memory_order_seq_cst);
    // Add the pointer to the global pool (NOTE: consumes reference created by '_DeeLValueType_New')
    _DeeStructureType_AddToTypePool_AlreadyLocked((DeeTypeObject *)result);
    DeeAtomicMutex_Release(&_structuretype_poollock);
   } else {
    DeeAtomicMutex_Release(&_structuretype_poollock);
    Dee_DECREF(result);
    result = temp;
   }
  } else {
   DeeAtomicMutex_Release(&_structuretype_poollock);
  }
 }
 return (DeeTypeObject *)result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeePointerObject) *DeePointer_New(
 DEE_A_IN_TYPEOBJECT(DeePointerTypeObject) const *tp, DEE_A_IN_OPT void *p) {
 DeePointerObject *result;
 DEE_ASSERT(DeeObject_Check(tp) && DeePointerType_Check(tp));
 if DEE_LIKELY((result = DEE_OBJECTPOOL_ALLOC(pointer)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)tp);
  result->po_p_ptr = p;
 }
 return (DeeObject *)result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeePointerObject) *DeeBasePointer_New(
 DEE_A_IN DeeTypeObject const *ptr_base, DEE_A_IN_OPT void *p) {
 return (ptr_base = DeeType_Pointer(ptr_base)) != NULL ? DeePointer_New(ptr_base,p) : NULL;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeLValueObject) *DeeLValue_New(
 DEE_A_IN_TYPEOBJECT(DeeLValueTypeObject) const *tp, DEE_A_IN void *p) {
 DeeLValueObject *result;
 DEE_ASSERT(DeeObject_Check(tp) && DeeLValueType_Check(tp));
 if DEE_LIKELY((result = (DeeLValueObject *)DEE_OBJECTPOOL_ALLOC(pointer)) != NULL) {
  DeeObject_INIT(result,(DeeTypeObject *)tp);
  result->lv_p_ptr = p;
 }
 return (DeeObject *)result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStructuredObject) *
DeeLValue_Value(DEE_A_IN_OBJECT(DeeLValueObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 return DeeStructured_NewFromData(DeeLValueType_BASE(Dee_TYPE(self)),
                                  DeeLValue_ADDR(self));
}



DEE_A_RET_TYPEOBJECT_EXCEPT(DeePointerTypeObject) *DeeStructuredType_Pointer(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *self) {
 DeePointerTypeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructuredType_Check(self));
 if DEE_UNLIKELY((result = (DeePointerTypeObject *)DeeAtomicPtr_Load(
  ((DeeStructuredTypeObject *)self)->stp_pointer,memory_order_seq_cst)) == NULL) {
  // Must create a new pointer type
  result = _DeePointerType_New((DeeStructuredTypeObject *)self);
  DeeAtomicMutex_Acquire(&_structuretype_poollock);
  if DEE_LIKELY(result) {
   DeePointerTypeObject *temp;
   // Reload the slot to see if someone else already create this type
   if DEE_LIKELY((temp = (DeePointerTypeObject *)DeeAtomicPtr_Load(
    ((DeeStructuredTypeObject *)self)->stp_pointer,memory_order_seq_cst)) == NULL) {
    // Store the pointer type in its rightful place
    DeeAtomicPtr_Store(((DeeStructuredTypeObject *)self)->stp_pointer,result,memory_order_seq_cst);
    // Add the pointer to the global pool (NOTE: consumes reference created by '_DeePointerType_New')
    _DeeStructureType_AddToTypePool_AlreadyLocked((DeeTypeObject *)result);
    DeeAtomicMutex_Release(&_structuretype_poollock);
   } else {
    DeeAtomicMutex_Release(&_structuretype_poollock);
    Dee_DECREF(result);
    result = temp;
   }
  } else {
   DeeAtomicMutex_Release(&_structuretype_poollock);
  }
 }
 return (DeeTypeObject *)result;
}
DEE_A_RET_TYPEOBJECT_EXCEPT(DeePointerTypeObject) *DeeLValueType_Pointer(
 DEE_A_IN_TYPEOBJECT(DeeLValueTypeObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeLValueType_Check(self));
 DEE_ASSERT(DeeObject_Check(((DeeLValueTypeObject *)self)->tp_lval_lvalue_base) &&
            DeeStructuredType_Check(((DeeLValueTypeObject *)self)->tp_lval_lvalue_base));
 return DeeStructuredType_Pointer(
  (DeeTypeObject *)((DeeLValueTypeObject *)self)->tp_lval_lvalue_base);
}
#endif

#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
DEE_A_RET_TYPEOBJECT_EXCEPT(DeeForeignFunctionTypeObject) *DeeStructuredType_ForeignFunction(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *self,
 DEE_A_IN Dee_funflags_t flags, DEE_A_IN Dee_size_t argc,
 DEE_A_IN_R(argc) DeeTypeObject const *const *argv) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStructuredType_Check(self));
 return (DeeTypeObject *)DeeForeignFunctionTypeList_FindOrAddToGlobalCache(
  &((DeeStructuredTypeObject *)self)->stp_ffunctions,flags,self,argc,argv);
}
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
DEE_A_RET_TYPEOBJECT_EXCEPT(DeeForeignFunctionTypeObject) *DeeLValueType_ForeignFunction(
 DEE_A_IN_TYPEOBJECT(DeeLValueTypeObject) const *self,
 DEE_A_IN Dee_funflags_t flags, DEE_A_IN Dee_size_t argc,
 DEE_A_IN_R(argc) DeeTypeObject const *const *argv) {
 DEE_ASSERT(DeeObject_Check(self) && DeeLValueType_Check(self));
 return (DeeTypeObject *)DeeForeignFunctionTypeList_FindOrAddToGlobalCache(
  &((DeeLValueTypeObject *)self)->ltp_ffunctions,flags,self,argc,argv);
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
DEE_A_RET_TYPEOBJECT_EXCEPT(DeeForeignFunctionTypeObject) *_DeeType_ForeignFunctionDefault(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_funflags_t flags,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeTypeObject const *const *argv) {
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERTF(!DeeStructuredType_Check(self),"Use 'DeeStructuredType_ForeignFunction(...)'");
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DEE_ASSERTF(!DeeLValueType_Check(self),"Use 'DeeLValueType_ForeignFunction(...)'");
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 return (DeeTypeObject *)DeeForeignFunctionTypeList_FindOrAddToGlobalCacheDefault(flags,self,argc,argv);
}

DEE_A_RET_TYPEOBJECT_EXCEPT(DeeForeignFunctionTypeObject) *DeeType_TForeignFunction(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_funflags_t flags,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) DeeTypeObject const *const *argv) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (DeeType_IsSameOrDerived(tp_self,&DeeStructuredType_Type))
  return DeeStructuredType_ForeignFunction(self,flags,argc,argv);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (tp_self == &DeeLValueType_Type) return DeeLValueType_ForeignFunction(self,flags,argc,argv);
#endif
 if (tp_self == (DeeTypeObject *)&DeeNone_Type)
  return DeeStructuredType_ForeignFunction((DeeTypeObject *)&DeeNone_Type,flags,argc,argv);
 if (tp_self == &DeeSuper_Type) return DeeType_TForeignFunction(
  DeeSuper_TYPE(self),(DeeTypeObject *)DeeSuper_SELF(self),flags,argc,argv);
 return _DeeType_ForeignFunctionDefault(self,flags,argc,argv);
}
#endif

#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
DEE_A_RET_TYPEOBJECT_EXCEPT(DeePointerTypeObject) *DeeType_TPointer(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeTypeObject const *self) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (DeeType_IsSameOrDerived(tp_self,&DeeStructuredType_Type)) return DeeStructuredType_Pointer(self);
 if (tp_self == &DeeLValueType_Type) return DeeLValueType_Pointer(self);
 if (tp_self == &DeeSuper_Type) return DeeType_TPointer(DeeSuper_TYPE(self),(DeeTypeObject *)DeeSuper_SELF(self));
 if (tp_self == (DeeTypeObject *)&DeeNone_Type) return DeeStructuredType_Pointer((DeeTypeObject *)&DeeNone_Type);
 DeeError_SetStringf(&DeeErrorType_TypeError,"Cannot create pointer type from %s%q",
                     DeeType_IsSameOrDerived(tp_self,&DeeType_Type) ? "" : "instance of ",
                     DeeType_IsSameOrDerived(tp_self,&DeeType_Type) ? DeeType_NAME(self) : DeeType_NAME(tp_self));
 return NULL;
}

DEE_A_RET_TYPEOBJECT_EXCEPT(DeeLValueTypeObject) *DeeType_TLValue(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeTypeObject const *self) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (DeeType_IsSameOrDerived(tp_self,&DeeStructuredType_Type)) return DeeStructuredType_LValue(self);
 if (tp_self == &DeeSuper_Type) return DeeType_TLValue(DeeSuper_TYPE(self),(DeeTypeObject *)DeeSuper_SELF(self));
 if (tp_self == (DeeTypeObject *)&DeeNone_Type) return DeeStructuredType_LValue((DeeTypeObject *)&DeeNone_Type);
 DeeError_SetStringf(&DeeErrorType_TypeError,"Cannot create lvalue type from %s%q",
                     DeeType_IsSameOrDerived(tp_self,&DeeType_Type) ? "" : "instance of ",
                     DeeType_IsSameOrDerived(tp_self,&DeeType_Type) ? DeeType_NAME(self) : DeeType_NAME(tp_self));
 return NULL;
}

DEE_A_RET_TYPEOBJECT_EXCEPT(DeeStructuredTypeObject) *DeeType_TWithoutPointer(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeTypeObject const *self) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeePointerType_Type) {
  DEE_ASSERT(DeeObject_Check(((DeePointerTypeObject *)self)->tp_ptr_pointer_base) &&
             DeeStructuredType_Check(((DeePointerTypeObject *)self)->tp_ptr_pointer_base));
  return (DeeTypeObject *)((DeePointerTypeObject *)self)->tp_ptr_pointer_base;
 }
 if (tp_self == &DeeSuper_Type) return DeeType_TWithoutPointer(DeeSuper_TYPE(self),(DeeTypeObject *)DeeSuper_SELF(self));
 if (tp_self == (DeeTypeObject *)&DeeNone_Type || self == (DeeTypeObject *)&DeeNone_Type) return (DeeTypeObject *)&DeeNone_Type;
 DeeError_SetStringf(&DeeErrorType_TypeError,
                     "Expected pointer type in __builtin_pointer_rem(), but got %s%s",
                     DeeType_IsSameOrDerived(tp_self,&DeeType_Type) ? "" : "instance of ",
                     DeeType_IsSameOrDerived(tp_self,&DeeType_Type) ? DeeType_NAME(self) : DeeType_NAME(tp_self));
 return NULL;
}

DEE_A_RET_TYPEOBJECT_EXCEPT(DeeStructuredTypeObject) *DeeType_TWithoutLValue(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeTypeObject const *self) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeLValueType_Type) {
  DEE_ASSERT(DeeObject_Check(((DeeLValueTypeObject *)self)->tp_lval_lvalue_base) &&
             DeeStructuredType_Check(((DeeLValueTypeObject *)self)->tp_lval_lvalue_base));
  return (DeeTypeObject *)((DeeLValueTypeObject *)self)->tp_lval_lvalue_base;
 }
 if (tp_self == &DeeSuper_Type) return DeeType_TWithoutLValue(DeeSuper_TYPE(self),(DeeTypeObject *)DeeSuper_SELF(self));
 if (tp_self == (DeeTypeObject *)&DeeNone_Type || self == (DeeTypeObject *)&DeeNone_Type) return (DeeTypeObject *)&DeeNone_Type;
 DeeError_SetStringf(&DeeErrorType_TypeError,
                     "Expected lvalue type in __builtin_lvalue_rem(), but got %s%s",
                     DeeType_IsSameOrDerived(tp_self,&DeeType_Type) ? "" : "instance of ",
                     DeeType_IsSameOrDerived(tp_self,&DeeType_Type) ? DeeType_NAME(self) : DeeType_NAME(tp_self));
 return NULL;
}
#endif

#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
DEE_A_RET_TYPEOBJECT_EXCEPT(DeeArrayTypeObject) *DeeType_TArray(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_size_t array_size) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == (DeeTypeObject *)&DeeNone_Type) self = (DeeTypeObject *)&DeeNone_Type;
 else if (tp_self == &DeeSuper_Type) return DeeType_TArray(DeeSuper_TYPE(self),(DeeTypeObject *)DeeSuper_SELF(self),array_size);
 else if DEE_UNLIKELY(!DeeType_IsSameOrDerived(tp_self,&DeeStructuredType_Type)) {
  DeeError_SetStringf(&DeeErrorType_TypeError,
                      "Expected structured type for array but (size: %Iu), but got %s%s",array_size,
                      DeeType_IsSameOrDerived(tp_self,&DeeType_Type) ? "" : "instance of ",
                      DeeType_IsSameOrDerived(tp_self,&DeeType_Type) ? DeeType_NAME(self) : DeeType_NAME(tp_self));
  return NULL;
 }
 DEE_ASSERTF(DeeType_IsSameOrDerived(tp_self,&DeeType_Type),
             "Being a structured type should have implied being a type...");
 if DEE_UNLIKELY(!array_size) {
  DeeError_SetStringf(&DeeErrorType_TypeError,
                      "Invalid type: \"%#q[0]\"",
                      DeeType_NAME(self));
  return NULL;
 }
 return (DeeTypeObject *)DeeArrayTypeList_FindOrAddToCache(
  &((DeeStructuredTypeObject *)self)->stp_arrays,
  (DeeStructuredTypeObject *)self,array_size);
}
DEE_A_RET_TYPEOBJECT_EXCEPT(DeeVarArrayTypeObject) *DeeType_TVArray(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_IN DeeTypeObject const *self) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (DeeType_IsSameOrDerived(tp_self,&DeeStructuredType_Type)) return DeeStructuredType_VArray(self);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (tp_self == &DeeLValueType_Type) return DeeLValueType_VArray(self);
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (tp_self == (DeeTypeObject *)&DeeNone_Type) return DeeStructuredType_VArray((DeeTypeObject *)&DeeNone_Type);
 if (tp_self == &DeeSuper_Type) return DeeType_TVArray(DeeSuper_TYPE(self),(DeeTypeObject *)DeeSuper_SELF(self));
 DeeError_SetStringf(&DeeErrorType_TypeError,"Cannot create variable-array type from %s%q",
                     DeeType_IsSameOrDerived(tp_self,&DeeType_Type) ? "" : "instance of ",
                     DeeType_IsSameOrDerived(tp_self,&DeeType_Type) ? DeeType_NAME(self) : DeeType_NAME(tp_self));
 return NULL;
}
DEE_A_RET_TYPEOBJECT_EXCEPT(DeeVarArrayTypeObject) *DeeStructuredType_VArray(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *self) {
 DeeVarArrayTypeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructuredType_Check(self));
 if DEE_UNLIKELY((result = (DeeVarArrayTypeObject *)DeeAtomicPtr_Load(
  ((DeeStructuredTypeObject *)self)->stp_varray,memory_order_seq_cst)) == NULL) {
  // Must create a new pointer type
  result = _DeeVarArrayType_New((DeeStructuredTypeObject *)self);
  DeeAtomicMutex_Acquire(&_structuretype_poollock);
  if DEE_LIKELY(result) {
   DeeVarArrayTypeObject *temp;
   // Reload the slot to see if someone else already create this type
   if DEE_LIKELY((temp = (DeeVarArrayTypeObject *)DeeAtomicPtr_Load(
    ((DeeStructuredTypeObject *)self)->stp_varray,memory_order_seq_cst)) == NULL) {
    // Store the pointer type in its rightful place
    DeeAtomicPtr_Store(((DeeStructuredTypeObject *)self)->stp_varray,result,memory_order_seq_cst);
    // Add the pointer to the global pool (NOTE: consumes reference created by '_DeePointerType_New')
    _DeeStructureType_AddToTypePool_AlreadyLocked((DeeTypeObject *)result);
    DeeAtomicMutex_Release(&_structuretype_poollock);
   } else {
    DeeAtomicMutex_Release(&_structuretype_poollock);
    Dee_DECREF(result);
    result = temp;
   }
  } else {
   DeeAtomicMutex_Release(&_structuretype_poollock);
  }
 }
 return (DeeTypeObject *)result;
}
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
DEE_A_RET_TYPEOBJECT_EXCEPT(DeeVarArrayTypeObject) *DeeLValueType_VArray(
 DEE_A_IN_TYPEOBJECT(DeeLValueTypeObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeLValueType_Check(self));
 DEE_ASSERT(DeeObject_Check(((DeeLValueTypeObject *)self)->tp_lval_lvalue_base) &&
            DeeStructuredType_Check(((DeeLValueTypeObject *)self)->tp_lval_lvalue_base));
 return DeeStructuredType_VArray(
  (DeeTypeObject *)((DeeLValueTypeObject *)self)->tp_lval_lvalue_base);
}
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#endif




DEE_A_RET_OBJECT_EXCEPT_REF(DeeStructuredObject) *DeeStructured_NewFromData(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *ty, DEE_A_IN void const *data) {
 DeeStructuredObject *result;
 DEE_ASSERT(DeeObject_Check(ty) && DeeStructuredType_Check(ty));
 DEE_ASSERT((DeeType_FLAGS(ty)&DEE_TYPE_FLAG_VAR_OBJECT)==0);
 // Allocate a new instance of the type
 result = (DeeStructuredObject *)(*DeeType_GET_SLOT(ty,tp_alloc))((DeeTypeObject *)ty);
 // Copy memory from the input data into the newly allocated type
 if DEE_LIKELY(result) memcpy(DeeStructured_DATA(result),data,DeeType_GET_SLOT(ty,tp_p_instance_size));
 return (DeeObject *)result;
}
DEE_A_RET_EXCEPT(-1) int DeeStructured_TSetData(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *self,
 DEE_A_IN DeeTypeObject const *tp_right, DEE_A_INOUT DeeObject *right) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 DEE_ASSERT(DeeObject_Check(right));
 DEE_ASSERT(DeeObject_Check(tp_right) && DeeType_Check(tp_right));
 DEE_ASSERT(DeeObject_InstanceOf(right,tp_right));
 if (DeeType_IsSameOrDerived(tp_right,(DeeTypeObject *)tp_self)) {
  // Same/derived object --> memcpy
  memcpy(self,DeeStructured_DATA(right),DeeType_GET_SLOT(tp_self,tp_p_instance_size));
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 } else if (DeeLValueType_Check(tp_right) && DeeType_IsSameOrDerived(
  DeeLValueType_BASE(tp_right),(DeeTypeObject *)tp_self)) {
  DEE_LVALUE_SIGNALHANDLER_BEGIN { // Lvalue to same/derived object --> memcpy
   memcpy(self,DeeLValue_ADDR(right),DeeType_GET_SLOT(tp_self,tp_p_instance_size));
  } DEE_LVALUE_SIGNALHANDLER_END(return -1);
#endif
 } else if (tp_self == &DeeSuper_Type) {
  return DeeStructured_TSetData(tp_self,self,DeeSuper_TYPE(right),DeeSuper_SELF(right));
 } else if (DeeType_HAS_SLOT(tp_self,tp_p_any_assign)) {
  // Different type --> use 'tp_p_any_assign' operator
  return (*DeeType_GET_SLOT(tp_self,tp_p_any_assign))((
   DeeStructuredTypeObject *)tp_self,self,right);
 } else {
  // Operator not set --> Not implemented error
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Not implemented: %s.__assign__(%s)",
                      DeeType_NAME(tp_self),
                      DeeType_NAME(tp_right));
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeStructured_DelData(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *self) {
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 DEE_LVALUE_SIGNALHANDLER_BEGIN { // Fill memory with zero
  memset(self,0,DeeType_GET_SLOT(tp_self,tp_p_instance_size));
 } DEE_LVALUE_SIGNALHANDLER_END(return -1);
 return 0;
}



#if DEE_CONFIG_RUNTIME_HAVE_NEWDELETE
DEE_A_RET_OBJECT_EXCEPT_REF(DeePointerObject) *DeePointer_NewInstance(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *ty) {
 DeeTypeObject *pointer_type; void *instance_memory; Dee_size_t instance_size;
 DEE_ASSERT(DeeObject_Check(ty) && DeeStructuredType_Check(ty));
 if ((pointer_type = DeeStructuredType_Pointer(ty)) == NULL) return NULL;
 if ((instance_size = DeeType_GET_SLOT(ty,tp_p_instance_size)) != 0) {
  // TODO: Debug informations from deemon traceback
  // TODO: This should have its own heap
  if ((instance_memory = malloc_nz(instance_size)) == NULL) {
   DeeError_NoMemory();
   return NULL;
  }
 } else instance_memory = NULL;
 return DeePointer_New(pointer_type,instance_memory);
}
DEE_A_RET_EXCEPT(-1) int DeePointer_DeleteInstance(DEE_A_INOUT DeeObject *self) {
 void *p;
 DEE_ASSERT(DeeObject_Check(self));
 if (DeeObject_GetVoidPointerEx(self,&p) != 0) return -1;
 free(p); // no 'free_nn', as this might be a NULL pointer
 return 0;
}
#endif











//////////////////////////////////////////////////////////////////////////
// Structured attribute operators (and optimized versions)
DEE_A_RET_EXCEPT_REF DeeObject *DeeStructured_TGetAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructuredObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DeeType_SLOT_TYPE(tp_p_attr_get) func;
 DeeTypeObject const *tp_first;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_first = tp_self;
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_get)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return DeeObject_TGenericGetAttr(tp_first,self,attr);
 }
 return (*func)((DeeStructuredTypeObject *)tp_first,DeeStructured_ADDR(self),(DeeObject *)attr);
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeStructured_GetAttr(
 DEE_A_INOUT_OBJECT(DeeStructuredObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DeeType_SLOT_TYPE(tp_p_attr_get) func;
 DeeTypeObject *tp_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 tp_self = Dee_TYPE(self);
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_get)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return DeeObject_GenericGetAttr(self,attr);
 }
 return (*func)((DeeStructuredTypeObject *)Dee_TYPE(self),DeeStructured_ADDR(self),(DeeObject *)attr);
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeStructured_THasAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructuredObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DeeType_SLOT_TYPE(tp_p_attr_get) func; DeeObject *temp;
 DeeTypeObject const *tp_first;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 tp_first = tp_self;
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_get)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return _DeeObject_TGenericHasAttrString(tp_first,self,DeeString_STR(attr));
 }
 if (func == (DeeType_SLOT_TYPE(tp_p_attr_get))&DeeStruct_PGetAttr)
  return DeeStruct_THasAttr(tp_first,self,attr);
 // fallback: Actually call the getter and see what happens
 temp = (*func)((DeeStructuredTypeObject *)tp_first,DeeStructured_ADDR(self),(DeeObject *)attr);
 if DEE_UNLIKELY(!temp) return DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  ) ? 0 : -1;
 Dee_DECREF(temp);
 return 1;
}
DEE_A_RET_EXCEPT(-1) int DeeStructured_TDelAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructuredObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DeeType_SLOT_TYPE(tp_p_attr_del) func;
 DeeTypeObject const *tp_first;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self)); tp_first = tp_self;
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_del)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return DeeObject_TGenericDelAttr(tp_first,self,attr);
 }
 return (*func)((DeeStructuredTypeObject *)tp_first,DeeStructured_ADDR(self),(DeeObject *)attr);
}
DEE_A_RET_EXCEPT(-1) int DeeStructured_DelAttr(
 DEE_A_INOUT_OBJECT(DeeStructuredObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DeeType_SLOT_TYPE(tp_p_attr_del) func;
 DeeTypeObject *tp_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 tp_self = Dee_TYPE(self);
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_del)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return DeeObject_GenericDelAttr(self,attr);
 }
 return (*func)((DeeStructuredTypeObject *)Dee_TYPE(self),DeeStructured_ADDR(self),(DeeObject *)attr);
}
DEE_A_RET_EXCEPT(-1) int DeeStructured_TSetAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructuredObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr,
 DEE_A_INOUT DeeObject *v) {
 DeeType_SLOT_TYPE(tp_p_attr_set) func;
 DeeTypeObject const *tp_first;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(v)); tp_first = tp_self;
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_set)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return DeeObject_TGenericSetAttr(tp_first,self,attr,v);
 }
 return (*func)((DeeStructuredTypeObject *)tp_first,DeeStructured_ADDR(self),(DeeObject *)attr,v);
}
DEE_A_RET_EXCEPT(-1) int DeeStructured_SetAttr(
 DEE_A_INOUT_OBJECT(DeeStructuredObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr,
 DEE_A_INOUT DeeObject *v) {
 DeeType_SLOT_TYPE(tp_p_attr_set) func;
 DeeTypeObject *tp_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(v));
 tp_self = Dee_TYPE(self);
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_set)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return DeeObject_GenericSetAttr(self,attr,v);
 }
 return (*func)((DeeStructuredTypeObject *)Dee_TYPE(self),DeeStructured_ADDR(self),(DeeObject *)attr,v);
}

DEE_A_RET_EXCEPT_REF DeeObject *DeeStructured_TGetAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructuredObject) *self, DEE_A_IN_Z char const *attr) {
 DeeType_SLOT_TYPE(tp_p_attr_get) func;
 DeeTypeObject const *tp_first; DeeObject *attr_ob,*result;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(attr); tp_first = tp_self;
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_get)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return DeeObject_TGenericGetAttrString(tp_first,self,attr);
 }
 if (func == (DeeType_SLOT_TYPE(tp_p_attr_get))&DeeStruct_PGetAttr)
  return DeeStruct_PGetAttrString(tp_first,DeeStructured_ADDR(self),attr);
 if DEE_UNLIKELY((attr_ob = DeeString_New(attr)) == NULL) return NULL;
 result = (*func)((DeeStructuredTypeObject *)tp_first,DeeStructured_ADDR(self),attr_ob);
 Dee_DECREF(attr_ob);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeStructured_THasAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructuredObject) *self, DEE_A_IN_Z char const *attr) {
 DeeType_SLOT_TYPE(tp_p_attr_get) func;
 DeeTypeObject const *tp_first; DeeObject *temp,*attr_ob;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(attr); tp_first = tp_self;
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_get)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return _DeeObject_TGenericHasAttrString(tp_first,self,attr);
 }
 if (func == (DeeType_SLOT_TYPE(tp_p_attr_get))&DeeStruct_PGetAttr)
  return DeeStruct_THasAttrString(tp_first,self,attr);
 // fallback: Actually call the getter and see what happens
 if DEE_UNLIKELY((attr_ob = DeeString_New(attr)) == NULL) return -1;
 temp = (*func)((DeeStructuredTypeObject *)tp_first,DeeStructured_ADDR(self),attr_ob);
 Dee_DECREF(attr_ob);
 if DEE_UNLIKELY(!temp) return DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  ) ? 0 : -1;
 Dee_DECREF(temp);
 return 1;
}
DEE_A_RET_EXCEPT(-1) int DeeStructured_TDelAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructuredObject) *self, DEE_A_IN_Z char const *attr) {
 DeeType_SLOT_TYPE(tp_p_attr_del) func;
 DeeTypeObject const *tp_first; DeeObject *attr_ob; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(attr); tp_first = tp_self;
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_del)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return DeeObject_TGenericDelAttrString(tp_first,self,attr);
 }
 if (func == (DeeType_SLOT_TYPE(tp_p_attr_del))&DeeStruct_PDelAttr)
  return DeeStruct_TDelAttrString(tp_first,self,attr);
 if DEE_UNLIKELY((attr_ob = DeeString_New(attr)) == NULL) return -1;
 result = (*func)((DeeStructuredTypeObject *)tp_first,DeeStructured_ADDR(self),attr_ob);
 Dee_DECREF(attr_ob);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeStructured_TSetAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructuredObject) *self,
 DEE_A_IN_Z char const *attr, DEE_A_INOUT DeeObject *v) {
 DeeType_SLOT_TYPE(tp_p_attr_set) func;
 DeeTypeObject const *tp_first; DeeObject *attr_ob; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 DEE_ASSERT(DeeObject_Check(v));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(attr); tp_first = tp_self;
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_set)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return DeeObject_TGenericSetAttrString(tp_first,self,attr,v);
 }
 if (func == (DeeType_SLOT_TYPE(tp_p_attr_set))&DeeStruct_PSetAttr)
  return DeeStruct_TSetAttrString(tp_first,self,attr,v);
 if DEE_UNLIKELY((attr_ob = DeeString_New(attr)) == NULL) return -1;
 result = (*func)((DeeStructuredTypeObject *)tp_first,DeeStructured_ADDR(self),attr_ob,v);
 Dee_DECREF(attr_ob);
 return result;
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeStructured_TCallAttrString(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructuredObject) *self,
 DEE_A_IN_Z char const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DeeType_SLOT_TYPE(tp_p_attr_get) func;
 DeeTypeObject const *tp_first; DeeObject *callback,*result,*attr_ob;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 DEE_ASSERT(attr); tp_first = tp_self;
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_get)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return DeeObject_TGenericCallAttrString(tp_first,self,attr,args);
 }
 if (func == (DeeType_SLOT_TYPE(tp_p_attr_get))&DeeStruct_PGetAttr)
  return DeeStruct_TCallAttrString(tp_first,self,attr,args);
 // Fallback: Call the getter, then call what it returns
 if DEE_UNLIKELY((attr_ob = DeeString_New(attr)) == NULL) return NULL;
 callback = (*func)((DeeStructuredTypeObject *)tp_first,DeeStructured_ADDR(self),attr_ob);
 Dee_DECREF(attr_ob);
 if DEE_UNLIKELY(!callback) return NULL;
 result = DeeObject_Call(callback,args);
 Dee_DECREF(callback);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeStructured_TCallAttr(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self,
 DEE_A_INOUT_OBJECT(DeeStructuredObject) *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DeeType_SLOT_TYPE(tp_p_attr_get) func;
 DeeTypeObject const *tp_first; DeeObject *callback,*result;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 tp_first = tp_self;
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_attr_get)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return DeeObject_TGenericCallAttr(tp_first,self,attr,(DeeObject *)args);
 }
 if (func == (DeeType_SLOT_TYPE(tp_p_attr_get))&DeeStruct_PGetAttr)
  return DeeStruct_TCallAttr(tp_first,self,attr,args);
 // Fallback: Call the getter, then call what it returns
 if DEE_UNLIKELY((callback = (*func)((DeeStructuredTypeObject *)tp_first,
  DeeStructured_ADDR(self),(DeeObject *)attr)) == NULL) return NULL;
 result = DeeObject_Call(callback,args);
 Dee_DECREF(callback);
 return result;
}







#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
//////////////////////////////////////////////////////////////////////////
// LValue VTable
static int _deelvalue_tp_ctor(
 DeeLValueTypeObject *tp_self, DeeLValueObject *DEE_UNUSED(self)) {
 DeeError_SetStringf(&DeeErrorType_TypeError,
                     "Cannot default-initialize instance of lvalue-type %q",
                     DeeType_NAME(tp_self));
 return -1;
}
static int _deelvalue_tp_any_ctor(
 DeeLValueTypeObject *tp_self, DeeLValueObject *self, DeeObject *args) {
 DeeObject *arg0;
 if (DeeTuple_Unpack(args,"o:lvalue",&arg0) != 0) return -1;
 if (DeeStructured_Check(arg0)) {
  DeeLValue_ADDR(self) = DeeStructured_DATA(arg0);
 } else if (DeeLValue_Check(arg0)) {
  DeeLValue_ADDR(self) = DeeLValue_ADDR(arg0);
 } else {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Not implemented: %s.__constructor__(%s)",
                      DeeType_NAME(tp_self),
                      DeeType_NAME(Dee_TYPE(arg0)));
  return -1;
 }
 return 0;
}
static int _deelvalue_tp_any_assign(
 DeeLValueObject *self, DeeObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 return DeeStructured_SetData((DeeTypeObject *)((
  DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base,
  DeeLValue_ADDR(self),right);
}
static DeeObject *_deelvalue_tp_str(DeeObject *self) {
 DeeStructuredTypeObject *tp; DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;
 if (DeeType_GET_SLOT(tp,tp_p_str)) { /* Use operator */
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   result = (*DeeType_GET_SLOT(tp,tp_p_str))(tp,DeeLValue_ADDR(self));
  } DEE_LVALUE_SIGNALHANDLER_END({ result = NULL; });
  return result;
 } else { /* Fallback implementation; same format as DeeObject_Type::tp_str */
  return DeeString_Newf("%s@%p",DeeType_NAME(tp),DeeLValue_ADDR(self));
 }
}
static DeeObject *_deelvalue_tp_repr(DeeObject *self) {
 DeeStructuredTypeObject *tp; DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;
 if (DeeType_GET_SLOT(tp,tp_p_repr)) { /* Use operator */
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   result = (*DeeType_GET_SLOT(tp,tp_p_repr))(tp,DeeLValue_ADDR(self));
  } DEE_LVALUE_SIGNALHANDLER_END({ result = NULL; });
  return result;
 } else { /* Fallback implementation; same format as DeeObject_Type::tp_str */
  return DeeString_Newf(">> %k\n%K",self,
                        DeeString_NewHexDump(DeeLValue_ADDR(self),
                        DeeType_GET_SLOT(tp,tp_p_instance_size)));
 }
}

#define NOTIMPLEMENTED_OPERATOR(name) \
 DeeError_SetStringf(&DeeErrorType_NotImplemented,\
  "Not implemented: %s." name "()",DeeType_NAME(tp))
#define NOTIMPLEMENTED_OPERATOR_2(name) \
 DeeError_SetStringf(&DeeErrorType_NotImplemented,\
  "Not implemented: %s." name "(%s)",\
  DeeType_NAME(tp),\
  DeeType_NAME(Dee_TYPE(right)))
#define NOTIMPLEMENTED_OPERATOR_3(name) \
 DeeError_SetStringf(&DeeErrorType_NotImplemented,\
  "Not implemented: %s." name "(%s,%s)",\
  DeeType_NAME(tp),\
  DeeType_NAME(Dee_TYPE(a)),\
  DeeType_NAME(Dee_TYPE(b)))
static int _deelvalue_tp_int32(DeeObject *self, Dee_int32_t *result) {
 DeeStructuredTypeObject *tp; Dee_int32_t error;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;
 if DEE_LIKELY(DeeType_GET_SLOT(tp,tp_p_int32)) { /* Use operator */
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   error = DeeType_GET_SLOT(tp,tp_p_int32)(tp,DeeLValue_ADDR(self),result);
  } DEE_LVALUE_SIGNALHANDLER_END({ error = -1; });
  return error;
 } else {
  NOTIMPLEMENTED_OPERATOR("__int32__");
  return -1;
 }
}
static int _deelvalue_tp_int64(DeeObject *self, Dee_int64_t *result) {
 DeeStructuredTypeObject *tp; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;
 if DEE_LIKELY(DeeType_GET_SLOT(tp,tp_p_int64)) { /* Use operator */
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   error = DeeType_GET_SLOT(tp,tp_p_int64)(tp,DeeLValue_ADDR(self),result);
  } DEE_LVALUE_SIGNALHANDLER_END({ error = -1; });
  return error;
 } else {
  NOTIMPLEMENTED_OPERATOR("__int64__");
  return -1;
 }
}
static int _deelvalue_tp_double(DeeObject *self, double *result) {
 DeeStructuredTypeObject *tp; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;
 if DEE_LIKELY(DeeType_GET_SLOT(tp,tp_p_double)) { /* Use operator */
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   error = DeeType_GET_SLOT(tp,tp_p_double)(tp,DeeLValue_ADDR(self),result);
  } DEE_LVALUE_SIGNALHANDLER_END({ error = -1; });
  return error;
 } else {
  NOTIMPLEMENTED_OPERATOR("__double__");
  return -1;
 }
}
static DeeObject *_deelvalue_tp_call(DeeObject *self, DeeObject *args) {
 DeeStructuredTypeObject *tp; DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;
 if DEE_LIKELY(DeeType_GET_SLOT(tp,tp_p_call)) { /* Use operator */
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   result = DeeType_GET_SLOT(tp,tp_p_call)(tp,DeeLValue_ADDR(self),args);
  } DEE_LVALUE_SIGNALHANDLER_END({ result = NULL; });
  return result;
 } else {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Not implemented: %s.__call__%K",
                      DeeType_NAME(tp),DeeTuple_Types(args));
  return NULL;
 }
}
static DeeObject *_deelvalue_tp_not(DeeObject *self) {
 DeeStructuredTypeObject *tp; DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;
 if DEE_LIKELY(DeeType_GET_SLOT(tp,tp_p_not)) { /* Use operator */
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   result = DeeType_GET_SLOT(tp,tp_p_not)(tp,DeeLValue_ADDR(self));
  } DEE_LVALUE_SIGNALHANDLER_END({ result = NULL; });
  return result;
 } else {
  NOTIMPLEMENTED_OPERATOR("__not__");
  return NULL;
 }
}
static int _deelvalue_tp_bool(DeeObject *self) {
 DeeStructuredTypeObject *tp; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;
 if DEE_LIKELY(DeeType_GET_SLOT(tp,tp_p_bool)) { /* Use operator */
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   result = DeeType_GET_SLOT(tp,tp_p_bool)(tp,DeeLValue_ADDR(self));
  } DEE_LVALUE_SIGNALHANDLER_END({ result = -1; });
  return result;
 } else {
  NOTIMPLEMENTED_OPERATOR("__bool__");
  return -1;
 }
}
static DeeObject *_deelvalue_tp_inc(DeeObject *self) {
 DeeStructuredTypeObject *tp; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;
 if DEE_LIKELY(DeeType_GET_SLOT(tp,tp_p_inc)) { /* Use operator */
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   error = DeeType_GET_SLOT(tp,tp_p_inc)(tp,DeeLValue_ADDR(self));
  } DEE_LVALUE_SIGNALHANDLER_END({ return NULL; });
  if DEE_UNLIKELY(error != 0) return NULL;
  DeeReturn_NEWREF(self);
 } else {
  NOTIMPLEMENTED_OPERATOR("__inc__");
  return NULL;
 }
}
static DeeObject *_deelvalue_tp_dec(DeeObject *self) {
 DeeStructuredTypeObject *tp; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;
 if DEE_LIKELY(DeeType_GET_SLOT(tp,tp_p_dec)) { /* Use operator */
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   error = DeeType_GET_SLOT(tp,tp_p_dec)(tp,DeeLValue_ADDR(self));
  } DEE_LVALUE_SIGNALHANDLER_END({ return NULL; });
  if DEE_UNLIKELY(error != 0) return NULL;
  DeeReturn_NEWREF(self);
 } else {
  NOTIMPLEMENTED_OPERATOR("__dec__");
  return NULL;
 }
}
#define DEE_PRIVATE_LVALUE_MAKE_UNARY_PROXY(tp_name,tp_p_name,name)\
static DeeObject *_deelvalue_##tp_name(DeeObject *self) {\
 DeeStructuredTypeObject *tp; DeeObject *result;\
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));\
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;\
 if DEE_LIKELY(DeeType_GET_SLOT(tp,tp_p_name)) { /* Use operator */\
  DEE_LVALUE_SIGNALHANDLER_BEGIN {\
   result = DeeType_GET_SLOT(tp,tp_p_name)(tp,DeeLValue_ADDR(self));\
  } DEE_LVALUE_SIGNALHANDLER_END({ result = NULL; });\
  return result;\
 } else {\
  NOTIMPLEMENTED_OPERATOR(name);\
  return NULL;\
 }\
}
#define DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_name,tp_p_name,name)\
static DeeObject *_deelvalue_##tp_name(DeeObject *self, DeeObject *right) {\
 DeeStructuredTypeObject *tp; DeeObject *result;\
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));\
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;\
 if DEE_LIKELY(DeeType_GET_SLOT(tp,tp_p_name)) { /* Use operator */\
  DEE_LVALUE_SIGNALHANDLER_BEGIN {\
   result = DeeType_GET_SLOT(tp,tp_p_name)(tp,DeeLValue_ADDR(self),right);\
  } DEE_LVALUE_SIGNALHANDLER_END({ result = NULL; });\
  return result;\
 } else {\
  NOTIMPLEMENTED_OPERATOR_2(name);\
  return NULL;\
 }\
}
#define DEE_PRIVATE_LVALUE_MAKE_BINARY_INPLACE_PROXY(tp_name,tp_p_name,name)\
static DeeObject *_deelvalue_##tp_name(DeeObject *self, DeeObject *right) {\
 DeeStructuredTypeObject *tp; int error;\
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));\
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;\
 if DEE_LIKELY(DeeType_GET_SLOT(tp,tp_p_name)) { /* Use operator */\
  DEE_LVALUE_SIGNALHANDLER_BEGIN {\
   error = DeeType_GET_SLOT(tp,tp_p_name)(tp,DeeLValue_ADDR(self),right);\
  } DEE_LVALUE_SIGNALHANDLER_END({ return NULL; });\
  if DEE_UNLIKELY(error != 0) return NULL;\
  DeeReturn_NEWREF(self);\
 } else {\
  NOTIMPLEMENTED_OPERATOR_2(name);\
  return NULL;\
 }\
}
DEE_PRIVATE_LVALUE_MAKE_UNARY_PROXY(tp_inv,tp_p_inv,"__inv__")
DEE_PRIVATE_LVALUE_MAKE_UNARY_PROXY(tp_pos,tp_p_pos,"__pos__")
DEE_PRIVATE_LVALUE_MAKE_UNARY_PROXY(tp_neg,tp_p_neg,"__neg__")
DEE_PRIVATE_LVALUE_MAKE_UNARY_PROXY(tp_incpost,tp_p_incpost,"__incpost__")
DEE_PRIVATE_LVALUE_MAKE_UNARY_PROXY(tp_decpost,tp_p_decpost,"__decpost__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_add,tp_p_add,"__add__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_INPLACE_PROXY(tp_iadd,tp_p_iadd,"__iadd__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_sub,tp_p_sub,"__sub__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_INPLACE_PROXY(tp_isub,tp_p_isub,"__isub__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_mul,tp_p_mul,"__mul__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_INPLACE_PROXY(tp_imul,tp_p_imul,"__imul__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_div,tp_p_div,"__div__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_INPLACE_PROXY(tp_idiv,tp_p_idiv,"__idiv__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_mod,tp_p_mod,"__mod__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_INPLACE_PROXY(tp_imod,tp_p_imod,"__imod__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_shl,tp_p_shl,"__shl__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_INPLACE_PROXY(tp_ishl,tp_p_ishl,"__ishl__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_shr,tp_p_shr,"__shr__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_INPLACE_PROXY(tp_ishr,tp_p_ishr,"__ishr__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_and,tp_p_and,"__and__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_INPLACE_PROXY(tp_iand,tp_p_iand,"__iand__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_or,tp_p_or,"__or__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_INPLACE_PROXY(tp_ior,tp_p_ior,"__ior__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_xor,tp_p_xor,"__xor__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_INPLACE_PROXY(tp_ixor,tp_p_ixor,"__ixor__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_pow,tp_p_pow,"__pow__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_INPLACE_PROXY(tp_ipow,tp_p_ipow,"__ipow__")
static int _deelvalue_tp_hash(DeeObject *self, Dee_hash_t start, Dee_hash_t *result) {
 DeeStructuredTypeObject *tp; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;
 DEE_LVALUE_SIGNALHANDLER_BEGIN {
  if (DeeType_GET_SLOT(tp,tp_p_hash)) { /* Use operator */
   error = DeeType_GET_SLOT(tp,tp_p_hash)(tp,DeeLValue_ADDR(self),start,result);
  } else {
   *result = DeeHash_Memory(DeeLValue_ADDR(self),DeeType_GET_SLOT(tp,tp_p_instance_size),start);
   error = 0;
  }
 } DEE_LVALUE_SIGNALHANDLER_END({ error = -1; });
 return error;
}
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_cmp_lo,tp_p_cmp_lo,"__lo__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_cmp_le,tp_p_cmp_le,"__le__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_cmp_eq,tp_p_cmp_eq,"__eq__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_cmp_ne,tp_p_cmp_ne,"__ne__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_cmp_gr,tp_p_cmp_gr,"__gr__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_cmp_ge,tp_p_cmp_ge,"__ge__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_seq_get,tp_p_seq_get,"__getitem__")
static int _deelvalue_tp_seq_del(DeeObject *self, DeeObject *i) {
 DeeStructuredTypeObject *tp; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;
 if DEE_LIKELY(DeeType_GET_SLOT(tp,tp_p_seq_del)) { /* Use operator */
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   result = DeeType_GET_SLOT(tp,tp_p_seq_del)(tp,DeeLValue_ADDR(self),i);
  } DEE_LVALUE_SIGNALHANDLER_END({ result = -1; });
  return result;
 } else {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Not implemented: %s.__delitem__(%s)",
                      DeeType_NAME(tp),
                      DeeType_NAME(Dee_TYPE(i)));
  return -1;
 }
}
static int _deelvalue_tp_seq_set(DeeObject *self, DeeObject *i, DeeObject *v) {
 DeeStructuredTypeObject *tp; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;
 if DEE_LIKELY(DeeType_GET_SLOT(tp,tp_p_seq_set)) { /* Use operator */
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   result = DeeType_GET_SLOT(tp,tp_p_seq_set)(tp,DeeLValue_ADDR(self),i,v);
  } DEE_LVALUE_SIGNALHANDLER_END({ result = -1; });
  return result;
 } else {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Not implemented: %s.__setitem__(%s,%s)",
                      DeeType_NAME(tp),
                      DeeType_NAME(Dee_TYPE(i)),
                      DeeType_NAME(Dee_TYPE(v)));
  return -1;
 }
}
DEE_PRIVATE_LVALUE_MAKE_UNARY_PROXY(tp_seq_size,tp_p_seq_size,"__size__")
DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY(tp_seq_contains,tp_p_seq_contains,"__contains__")
static DeeObject *_deelvalue_tp_seq_range_get(DeeObject *self, DeeObject *lo, DeeObject *hi) {
 DeeStructuredTypeObject *tp; DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;
 if DEE_LIKELY(DeeType_GET_SLOT(tp,tp_p_seq_range_get)) { /* Use operator */
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   result = DeeType_GET_SLOT(tp,tp_p_seq_range_get)(tp,DeeLValue_ADDR(self),lo,hi);
  } DEE_LVALUE_SIGNALHANDLER_END({ result = NULL; });
  return result;
 } else {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Not implemented: %s.__getrange__(%s,%s)",
                      DeeType_NAME(tp),
                      DeeType_NAME(Dee_TYPE(lo)),
                      DeeType_NAME(Dee_TYPE(hi)));
  return NULL;
 }
}
static int _deelvalue_tp_seq_range_del(DeeObject *self, DeeObject *lo, DeeObject *hi) {
 DeeStructuredTypeObject *tp; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;
 if DEE_LIKELY(DeeType_GET_SLOT(tp,tp_p_seq_range_del)) { /* Use operator */
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   result = DeeType_GET_SLOT(tp,tp_p_seq_range_del)(tp,DeeLValue_ADDR(self),lo,hi);
  } DEE_LVALUE_SIGNALHANDLER_END({ result = -1; });
  return result;
 } else {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Not implemented: %s.__delrange__(%s,%s)",
                      DeeType_NAME(tp),
                      DeeType_NAME(Dee_TYPE(lo)),
                      DeeType_NAME(Dee_TYPE(hi)));
  return -1;
 }
}
static int _deelvalue_tp_seq_range_set(DeeObject *self, DeeObject *lo, DeeObject *hi, DeeObject *v) {
 DeeStructuredTypeObject *tp; int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 tp = ((DeeLValueTypeObject *)Dee_TYPE(self))->tp_lval_lvalue_base;
 if DEE_LIKELY(DeeType_GET_SLOT(tp,tp_p_seq_range_set)) { /* Use operator */
  DEE_LVALUE_SIGNALHANDLER_BEGIN {
   result = DeeType_GET_SLOT(tp,tp_p_seq_range_set)(tp,DeeLValue_ADDR(self),lo,hi,v);
  } DEE_LVALUE_SIGNALHANDLER_END({ result = -1; });
  return result;
 } else {
  DeeError_SetStringf(&DeeErrorType_NotImplemented,
                      "Not implemented: %s.__setrange__(%s,%s,%s)",
                      DeeType_NAME(tp),
                      DeeType_NAME(Dee_TYPE(lo)),
                      DeeType_NAME(Dee_TYPE(hi)),
                      DeeType_NAME(Dee_TYPE(v)));
  return -1;
 }
}
DEE_PRIVATE_LVALUE_MAKE_UNARY_PROXY(tp_seq_iter_self,tp_p_seq_iter_self,"__iterself__")
DeeObject *_deelvalue_tp_attr_get(DeeLValueObject *self, DeeObject *attr) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_LVALUE_SIGNALHANDLER_BEGIN {
  result = DeeObject_PGetAttr(DeeLValueType_BASE(Dee_TYPE(self)),DeeLValue_ADDR(self),attr);
 } DEE_LVALUE_SIGNALHANDLER_END({ result = NULL; });
 return result;
}
int _deelvalue_tp_attr_del(DeeLValueObject *self, DeeObject *attr) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_LVALUE_SIGNALHANDLER_BEGIN {
  result = DeeObject_PDelAttr(DeeLValueType_BASE(Dee_TYPE(self)),DeeLValue_ADDR(self),attr);
 } DEE_LVALUE_SIGNALHANDLER_END({ result = -1; });
 return result;
}
int _deelvalue_tp_attr_set(DeeLValueObject *self, DeeObject *attr, DeeObject *v) {
 int result;
 DEE_ASSERT(DeeObject_Check(self) && DeeLValue_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(v));
 DEE_LVALUE_SIGNALHANDLER_BEGIN {
  result = DeeObject_PSetAttr(DeeLValueType_BASE(Dee_TYPE(self)),DeeLValue_ADDR(self),attr,v);
 } DEE_LVALUE_SIGNALHANDLER_END({ result = -1; });
 return result;
}

#undef DEE_PRIVATE_LVALUE_MAKE_BINARY_INPLACE_PROXY
#undef DEE_PRIVATE_LVALUE_MAKE_BINARY_PROXY
#undef DEE_PRIVATE_LVALUE_MAKE_UNARY_PROXY

#undef NOTIMPLEMENTED_OPERATOR_3
#undef NOTIMPLEMENTED_OPERATOR_2
#undef NOTIMPLEMENTED_OPERATOR

//////////////////////////////////////////////////////////////////////////
// Pointer VTable
static DeeObject *_deepointer_tp_p_str(
 DeeStructuredTypeObject *DEE_UNUSED(tp_self), Dee_uintptr_t *self) {
#if DEE_TYPES_SIZEOF_POINTER == 8
 return DeeString_Newf("%#0.16Ix",*self);
#else
 return DeeString_Newf("%#0.8Ix",*self);
#endif
}
static DeeObject *_deepointer_tp_p_repr(DeeStructuredTypeObject *tp_self, Dee_uintptr_t *self) {
#if DEE_TYPES_SIZEOF_POINTER == 8
 return DeeString_Newf("%s:%#0.16Ix",DeeType_NAME(tp_self),*self);
#else
 return DeeString_Newf("%s:%#0.8Ix",DeeType_NAME(tp_self),*self);
#endif
}
static int _deepointer_tp_p_int32(DeePointerTypeObject *DEE_UNUSED(tp_self), Dee_uintptr_t *self, Dee_int32_t *result) { *result = (Dee_int32_t)*self; return 0; }
static int _deepointer_tp_p_int64(DeePointerTypeObject *DEE_UNUSED(tp_self), Dee_uintptr_t *self, Dee_int64_t *result) { *result = (Dee_int64_t)*self; return 0; }
static int _deepointer_tp_p_double(DeePointerTypeObject *DEE_UNUSED(tp_self), Dee_uintptr_t *self, double *result) { *result = (double)*self; return 0; }
static DeeObject *_deepointer_tp_p_not(DeePointerTypeObject *DEE_UNUSED(tp_self), Dee_uintptr_t *self) { DeeReturn_Bool(*self == 0); }
static int _deepointer_tp_p_bool(DeePointerTypeObject *DEE_UNUSED(tp_self), Dee_uintptr_t *self) { return *self != 0; }
static int _deepointer_tp_p_inc(DeePointerTypeObject *tp_self, Dee_uintptr_t *self) { *self += DeePointerType_ELEMSIZ(tp_self); return 0; }
static int _deepointer_tp_p_dec(DeePointerTypeObject *tp_self, Dee_uintptr_t *self) { *self -= DeePointerType_ELEMSIZ(tp_self); return 0; }
static DeeObject *_deepointer_tp_p_incpost(
 DeePointerTypeObject *tp_self, Dee_uintptr_t *self) {
 Dee_uintptr_t result = *self;
 *self += DeePointerType_ELEMSIZ(tp_self);
 return DeePointer_New((DeeTypeObject *)tp_self,(void *)result);
}
static DeeObject *_deepointer_tp_p_decpost(
 DeePointerTypeObject *tp_self, Dee_uintptr_t *self) {
 Dee_uintptr_t result = *self;
 *self += DeePointerType_ELEMSIZ(tp_self);
 return DeePointer_New((DeeTypeObject *)tp_self,(void *)result);
}
static DeeObject *_deepointer_tp_p_add(
 DeePointerTypeObject *tp_self, Dee_ssize_t *self, DeeObject *right) {
 Dee_ssize_t ri;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,right,&ri) != 0) return NULL;
 return DeePointer_New((DeeTypeObject *)tp_self,(
  void *)((Dee_size_t)(*self+ri*tp_self->tp_ptr_elem_size)));
}
static int _deepointer_tp_p_iadd(
 DeePointerTypeObject *tp_self, Dee_uintptr_t *self, DeeObject *right) {
 Dee_ssize_t ri;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,right,&ri) != 0) return -1;
 *self += ri*tp_self->tp_ptr_elem_size;
 return 0;
}
static DeeObject *_deepointer_tp_p_sub(
 DeePointerTypeObject *tp_self, Dee_ssize_t *self, DeeObject *right) {
 void *ptr_right; int temp; Dee_ssize_t ri;
 if DEE_UNLIKELY((temp = DeeObject_TryGetPointer(right,(DeeTypeObject *)
  tp_self->tp_ptr_pointer_base,&ptr_right)) < 0) return NULL;
 if (temp == 0) { // Pointer diff
  return DeeObject_New(Dee_rt_ptrdiff_t,(Dee_rt_ptrdiff_t)(
   (*self-(Dee_ssize_t)ptr_right)/(Dee_ssize_t)tp_self->tp_ptr_elem_size));
 }
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,right,&ri) != 0) return NULL;
 return DeePointer_New((DeeTypeObject *)tp_self,(
  void *)((Dee_size_t)(*(Dee_size_t *)self-ri*tp_self->tp_ptr_elem_size)));
}
static int _deepointer_tp_p_isub(
 DeePointerTypeObject *tp_self, Dee_uintptr_t *self, DeeObject *right) {
 Dee_ssize_t ri;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,right,&ri) != 0) return -1;
 *self -= ri*tp_self->tp_ptr_elem_size;
 return 0;
}
#define DEE_PRIVATE_MAKE_POINTER_COMPARE_OPERATOR(name,op)\
static DeeObject *name(DeePointerTypeObject *tp_self, void *self, DeeObject *right) {\
 void *right_addr;\
 if DEE_UNLIKELY(DeeObject_GetPointerEx(right,(DeeTypeObject *)tp_self->tp_ptr_pointer_base,&right_addr) != 0) return NULL;\
 DeeReturn_Bool(*(void **)self op right_addr);\
}
DEE_PRIVATE_MAKE_POINTER_COMPARE_OPERATOR(_deepointer_tp_p_cmp_lo,<)
DEE_PRIVATE_MAKE_POINTER_COMPARE_OPERATOR(_deepointer_tp_p_cmp_le,<=)
DEE_PRIVATE_MAKE_POINTER_COMPARE_OPERATOR(_deepointer_tp_p_cmp_eq,==)
DEE_PRIVATE_MAKE_POINTER_COMPARE_OPERATOR(_deepointer_tp_p_cmp_ne,!=)
DEE_PRIVATE_MAKE_POINTER_COMPARE_OPERATOR(_deepointer_tp_p_cmp_gr,>)
DEE_PRIVATE_MAKE_POINTER_COMPARE_OPERATOR(_deepointer_tp_p_cmp_ge,>=)
#undef DEE_PRIVATE_MAKE_POINTER_COMPARE_OPERATOR
static DeeObject *_deepointer_tp_p_seq_get(
 DeePointerTypeObject *tp_self, Dee_uintptr_t *self, DeeObject *i) {
 DeeTypeObject *lval_tp;
 Dee_ssize_t ri;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,i,&ri) != 0) return NULL;
 if DEE_UNLIKELY((lval_tp = DeeType_LValue((DeeTypeObject *)((
  DeePointerTypeObject *)tp_self)->tp_ptr_pointer_base)) == NULL) return NULL;
 return DeeLValue_NewWithError(lval_tp,(
  void *)((Dee_size_t)(*self+ri*tp_self->tp_ptr_elem_size)));
}
static int _deepointer_tp_p_seq_set(
 DeePointerTypeObject *tp_self, Dee_uintptr_t *self, DeeObject *i, DeeObject *v) {
 Dee_ssize_t ri;
 DEE_ASSERT(DeeObject_Check(v));
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,i,&ri) != 0) return -1;
 return DeeStructured_SetData((DeeTypeObject *)tp_self->tp_ptr_pointer_base,
                              (void *)((Dee_size_t)(*self+ri*tp_self->tp_ptr_elem_size)),v);
}
#endif



//////////////////////////////////////////////////////////////////////////
// Structured Object VTable
static int _deestructuredobject_tp_copy_ctor(
 DeeTypeObject *tp_self, DeeObject *self, DeeObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeStructured_Check(right));
 memcpy(DeeStructured_DATA(self),DeeStructured_DATA(right),
        DeeType_GET_SLOT(tp_self,tp_p_instance_size));
 return 0;
}
static int _deestructuredobject_tp_copy_assign(DeeObject *self, DeeObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeStructured_Check(right));
 if DEE_LIKELY(self != right) {
  memcpy(DeeStructured_DATA(self),DeeStructured_DATA(right),DeeStructured_SIZE(self));
 }
 return 0;
}
static int _deestructuredobject_tp_any_ctor(
 DeeStructuredTypeObject *tp_self, DeeObject *self, DeeObject *args) {
 DeeType_SLOT_TYPE(tp_p_any_ctor) func;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 if ((func = DeeType_GET_SLOT(tp_self,tp_p_any_ctor)) != NULL)
  return (*func)(tp_self,DeeStructured_ADDR(self),args);
 if (DeeTuple_SIZE(args) == 1) {
  return DeeStructured_SetData((DeeTypeObject *)tp_self,
                               DeeStructured_DATA(self),DeeTuple_GET(args,0));
 }
 return (*DeeType_GET_SLOT(&DeeObject_Type,tp_any_ctor))(&DeeObject_Type,self,args);
}
static int _deestructuredobject_tp_any_assign(DeeObject *self, DeeObject *right) {
 return DeeStructured_SetData(Dee_TYPE(self),DeeStructured_DATA(self),right);
}
#define DEE_PRIVATE_STRUCTURED_OBJECT_UNARY_OPERATOR_PROXY(name,pname)\
static DeeObject *_deestructuredobject_##name(DeeObject *self)\
{\
 DeeType_SLOT_TYPE(pname) func;\
 DeeTypeObject *tp_self;\
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));\
 tp_self = Dee_TYPE(self);\
 while ((func = DeeType_GET_SLOT(tp_self,pname)) == NULL) {\
  tp_self = DeeType_BASE(tp_self);\
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))\
   return (*DeeType_GET_SLOT(&DeeObject_Type,name))(self);\
 }\
 return (*func)((DeeStructuredTypeObject *)tp_self,DeeStructured_ADDR(self));\
}
#define DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(name,pname)\
static DeeObject *_deestructuredobject_##name(DeeObject *self, DeeObject *right)\
{\
 DeeType_SLOT_TYPE(pname) func;\
 DeeTypeObject *tp_self;\
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));\
 DEE_ASSERT(DeeObject_Check(right));\
 tp_self = Dee_TYPE(self);\
 while ((func = DeeType_GET_SLOT(tp_self,pname)) == NULL) {\
  tp_self = DeeType_BASE(tp_self);\
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))\
   return (*DeeType_GET_SLOT(&DeeObject_Type,name))(self,right);\
 }\
 return (*func)((DeeStructuredTypeObject *)tp_self,DeeStructured_ADDR(self),right);\
}
#define DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_INPLACE_OPERATOR_PROXY(name,pname)\
static DeeObject *_deestructuredobject_##name(DeeObject *self, DeeObject *right)\
{\
 DeeType_SLOT_TYPE(pname) func;\
 DeeTypeObject *tp_self;\
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));\
 tp_self = Dee_TYPE(self);\
 while ((func = DeeType_GET_SLOT(tp_self,pname)) == NULL) {\
  tp_self = DeeType_BASE(tp_self);\
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))\
   return (*DeeType_GET_SLOT(&DeeObject_Type,name))(self,right);\
 }\
 if ((*func)((DeeStructuredTypeObject *)tp_self,DeeStructured_ADDR(self),right) == -1) return NULL;\
 DeeReturn_NEWREF(self);\
}

DEE_PRIVATE_STRUCTURED_OBJECT_UNARY_OPERATOR_PROXY(tp_str,tp_p_str)
DEE_PRIVATE_STRUCTURED_OBJECT_UNARY_OPERATOR_PROXY(tp_repr,tp_p_repr)

static int _deestructuredobject_tp_int32(DeeObject *self, Dee_int32_t *result) {
 DeeType_SLOT_TYPE(tp_p_int32) func; DeeTypeObject *tp_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 tp_self = Dee_TYPE(self);
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_int32)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return (*DeeType_GET_SLOT(&DeeObject_Type,tp_int32))(self,result);
 }
 return (*func)((DeeStructuredTypeObject *)tp_self,DeeStructured_ADDR(self),result);
}
static int _deestructuredobject_tp_int64(DeeObject *self, Dee_int64_t *result) {
 DeeType_SLOT_TYPE(tp_p_int64) func;
 DeeTypeObject *tp_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 tp_self = Dee_TYPE(self);
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_int64)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return (*DeeType_GET_SLOT(&DeeObject_Type,tp_int64))(self,result);
 }
 return (*func)((DeeStructuredTypeObject *)tp_self,DeeStructured_ADDR(self),result);
}
static int _deestructuredobject_tp_double(DeeObject *self, double *result) {
 DeeType_SLOT_TYPE(tp_p_double) func;
 DeeTypeObject *tp_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 tp_self = Dee_TYPE(self);
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_double)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return (*DeeType_GET_SLOT(&DeeObject_Type,tp_double))(self,result);
 }
 return (*func)((DeeStructuredTypeObject *)tp_self,DeeStructured_ADDR(self),result);
}
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_call,tp_p_call)
DEE_PRIVATE_STRUCTURED_OBJECT_UNARY_OPERATOR_PROXY(tp_not,tp_p_not)
static int _deestructuredobject_tp_bool(DeeObject *self) {
 DeeType_SLOT_TYPE(tp_p_bool) func;
 DeeTypeObject *tp_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 tp_self = Dee_TYPE(self);
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_bool)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return (*DeeType_GET_SLOT(&DeeObject_Type,tp_bool))(self);
 }
 return (*func)((DeeStructuredTypeObject *)tp_self,DeeStructured_ADDR(self));
}
DEE_PRIVATE_STRUCTURED_OBJECT_UNARY_OPERATOR_PROXY(tp_inv,tp_p_inv)
DEE_PRIVATE_STRUCTURED_OBJECT_UNARY_OPERATOR_PROXY(tp_pos,tp_p_pos)
DEE_PRIVATE_STRUCTURED_OBJECT_UNARY_OPERATOR_PROXY(tp_neg,tp_p_neg)

static DeeObject *_deestructuredobject_tp_inc(DeeObject *self) {
 DeeType_SLOT_TYPE(tp_p_inc) func;
 DeeTypeObject *tp_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 tp_self = Dee_TYPE(self);
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_inc)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return (*DeeType_GET_SLOT(&DeeObject_Type,tp_inc))(self);
 }
 if DEE_UNLIKELY((*func)((DeeStructuredTypeObject *)tp_self,DeeStructured_ADDR(self)) != 0) return NULL;
 DeeReturn_NEWREF(self);
}
static DeeObject *_deestructuredobject_tp_dec(DeeObject *self) {
 DeeType_SLOT_TYPE(tp_p_dec) func;
 DeeTypeObject *tp_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 tp_self = Dee_TYPE(self);
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_dec)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return (*DeeType_GET_SLOT(&DeeObject_Type,tp_dec))(self);
 }
 if DEE_UNLIKELY((*func)((DeeStructuredTypeObject *)tp_self,DeeStructured_ADDR(self)) != 0) return NULL;
 DeeReturn_NEWREF(self);
}
static DeeObject *_deestructuredobject_tp_incpost(DeeObject *self) {
 DeeType_SLOT_TYPE(tp_p_incpost) func;
 DeeTypeObject *tp_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 tp_self = Dee_TYPE(self);
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_incpost)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return (*DeeType_GET_SLOT(&DeeObject_Type,tp_incpost))(self);
 }
 return (*func)((DeeStructuredTypeObject *)tp_self,DeeStructured_ADDR(self));
}
static DeeObject *_deestructuredobject_tp_decpost(DeeObject *self) {
 DeeType_SLOT_TYPE(tp_p_decpost) func;
 DeeTypeObject *tp_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 tp_self = Dee_TYPE(self);
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_decpost)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return (*DeeType_GET_SLOT(&DeeObject_Type,tp_decpost))(self);
 }
 return (*func)((DeeStructuredTypeObject *)tp_self,DeeStructured_ADDR(self));
}

DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_add,tp_p_add)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_sub,tp_p_sub)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_mul,tp_p_mul)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_div,tp_p_div)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_mod,tp_p_mod)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_shl,tp_p_shl)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_shr,tp_p_shr)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_and,tp_p_and)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_or,tp_p_or)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_xor,tp_p_xor)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_pow,tp_p_pow)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_INPLACE_OPERATOR_PROXY(tp_iadd,tp_p_iadd)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_INPLACE_OPERATOR_PROXY(tp_isub,tp_p_isub)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_INPLACE_OPERATOR_PROXY(tp_imul,tp_p_imul)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_INPLACE_OPERATOR_PROXY(tp_idiv,tp_p_idiv)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_INPLACE_OPERATOR_PROXY(tp_imod,tp_p_imod)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_INPLACE_OPERATOR_PROXY(tp_ishl,tp_p_ishl)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_INPLACE_OPERATOR_PROXY(tp_ishr,tp_p_ishr)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_INPLACE_OPERATOR_PROXY(tp_iand,tp_p_iand)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_INPLACE_OPERATOR_PROXY(tp_ior,tp_p_ior)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_INPLACE_OPERATOR_PROXY(tp_ixor,tp_p_ixor)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_INPLACE_OPERATOR_PROXY(tp_ipow,tp_p_ipow)
static int _deestructuredobject_tp_hash(DeeObject *self, Dee_hash_t start, Dee_hash_t *result) {
 DeeType_SLOT_TYPE(tp_p_hash) func;
 DeeTypeObject *tp_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 tp_self = Dee_TYPE(self);
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_hash)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return (*DeeType_GET_SLOT(&DeeObject_Type,tp_hash))(self,start,result);
 }
 return (*func)((DeeStructuredTypeObject *)tp_self,DeeStructured_ADDR(self),start,result);
}
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_cmp_lo,tp_p_cmp_lo)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_cmp_le,tp_p_cmp_le)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_cmp_eq,tp_p_cmp_eq)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_cmp_ne,tp_p_cmp_ne)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_cmp_gr,tp_p_cmp_gr)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_cmp_ge,tp_p_cmp_ge)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_seq_get,tp_p_seq_get)
static int _deestructuredobject_tp_seq_del(DeeObject *self, DeeObject *i) {
 DeeType_SLOT_TYPE(tp_p_seq_del) func;
 DeeTypeObject *tp_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 tp_self = Dee_TYPE(self);
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_seq_del)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return (*DeeType_GET_SLOT(&DeeObject_Type,tp_seq_del))(self,i);
 }
 return (*func)((DeeStructuredTypeObject *)tp_self,DeeStructured_ADDR(self),i);
}
static int _deestructuredobject_tp_seq_set(DeeObject *self, DeeObject *i, DeeObject *v) {
 DeeType_SLOT_TYPE(tp_p_seq_set) func;
 DeeTypeObject *tp_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 tp_self = Dee_TYPE(self);
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_seq_set)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return (*DeeType_GET_SLOT(&DeeObject_Type,tp_seq_set))(self,i,v);
 }
 return (*func)((DeeStructuredTypeObject *)tp_self,DeeStructured_ADDR(self),i,v);
}
DEE_PRIVATE_STRUCTURED_OBJECT_UNARY_OPERATOR_PROXY(tp_seq_size,tp_p_seq_size)
DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY(tp_seq_contains,tp_p_seq_contains)
static DeeObject *_deestructuredobject_tp_seq_range_get(DeeObject *self, DeeObject *lo, DeeObject *hi) {
 DeeType_SLOT_TYPE(tp_p_seq_range_get) func;
 DeeTypeObject *tp_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 tp_self = Dee_TYPE(self);
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_seq_range_get)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return (*DeeType_GET_SLOT(&DeeObject_Type,tp_seq_range_get))(self,lo,hi);
 }
 return (*func)((DeeStructuredTypeObject *)tp_self,DeeStructured_ADDR(self),lo,hi);
}
static int _deestructuredobject_tp_seq_range_del(DeeObject *self, DeeObject *lo, DeeObject *hi) {
 DeeType_SLOT_TYPE(tp_p_seq_range_del) func;
 DeeTypeObject *tp_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 tp_self = Dee_TYPE(self);
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_seq_range_del)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return (*DeeType_GET_SLOT(&DeeObject_Type,tp_seq_range_del))(self,lo,hi);
 }
 return (*func)((DeeStructuredTypeObject *)tp_self,DeeStructured_ADDR(self),lo,hi);
}
static int _deestructuredobject_tp_seq_range_set(DeeObject *self, DeeObject *lo, DeeObject *hi, DeeObject *v) {
 DeeType_SLOT_TYPE(tp_p_seq_range_set) func;
 DeeTypeObject *tp_self;
 DEE_ASSERT(DeeObject_Check(self) && DeeStructured_Check(self));
 tp_self = Dee_TYPE(self);
 while ((func = DeeType_GET_SLOT(tp_self,tp_p_seq_range_set)) == NULL) {
  tp_self = DeeType_BASE(tp_self);
  if DEE_UNLIKELY(!DeeStructuredType_Check(tp_self))
   return (*DeeType_GET_SLOT(&DeeObject_Type,tp_seq_range_set))(self,lo,hi,v);
 }
 return (*func)((DeeStructuredTypeObject *)tp_self,DeeStructured_ADDR(self),lo,hi,v);
}
DEE_PRIVATE_STRUCTURED_OBJECT_UNARY_OPERATOR_PROXY(tp_seq_iter_self,tp_p_seq_iter_self)
#undef DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_INPLACE_OPERATOR_PROXY
#undef DEE_PRIVATE_STRUCTURED_OBJECT_BINARY_OPERATOR_PROXY
#undef DEE_PRIVATE_STRUCTURED_OBJECT_UNARY_OPERATOR_PROXY





#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
static DeeObject *_deestructuredtype_tp_seq_get(
 DeeStructuredTypeObject *self, DeeObject *array_size_ob) {
 Dee_size_t array_size; DeeTypeObject *result;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,array_size_ob,&array_size) != 0) return NULL;
 result = DeeType_Array((DeeTypeObject *)self,array_size);
 Dee_XINCREF(result);
 return (DeeObject *)result;
}
#endif

#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION || DEE_CONFIG_RUNTIME_HAVE_ARRAYS
static void _deestructuredtype_tp_dtor(DeeStructuredTypeObject *self) {
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
 DeeForeignFunctionTypeList_Quit(&self->stp_ffunctions);
#endif
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 DeeArrayTypeList_Quit(&self->stp_arrays);
#endif
}
#endif /* ... */

static void _DeeStructuredType_InitCommon(DeeStructuredTypeObject *self) {
 self->stp_prev_structured_object_type = NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 self->stp_pointer = NULL;
 self->stp_lvalue = NULL;
#endif
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
 DeeForeignFunctionTypeList_Init(&self->stp_ffunctions);
#endif
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 DeeArrayTypeList_Init(&self->stp_arrays);
 self->stp_varray = NULL;
#endif
 DeeType_GET_SLOT(self,tp_flags) |= DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_MUST_COPY|
                                    DEE_TYPE_FLAG_CONST_CTOR|DEE_TYPE_FLAG_FUNDAMENTAL;
 DeeType_GET_SLOT(self,tp_ctor) = &_DeeGeneric_Noop;
 DeeType_GET_SLOT(self,tp_copy_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deestructuredobject_tp_copy_ctor;
 DeeType_GET_SLOT(self,tp_move_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deestructuredobject_tp_copy_ctor;
 DeeType_GET_SLOT(self,tp_any_ctor) = (int(*)(DeeTypeObject*,DeeObject*,DeeObject*))&_deestructuredobject_tp_any_ctor;
 DeeType_GET_SLOT(self,tp_copy_assign) = &_deestructuredobject_tp_copy_assign;
 DeeType_GET_SLOT(self,tp_move_assign) = &_deestructuredobject_tp_copy_assign;
 DeeType_GET_SLOT(self,tp_any_assign) = &_deestructuredobject_tp_any_assign;
 DeeType_GET_SLOT(self,tp_p_instance_size) = DeeType_DEFAULT_SLOT(tp_p_instance_size);
 DeeType_GET_SLOT(self,tp_p_any_ctor) = DeeType_DEFAULT_SLOT(tp_p_any_ctor);
 DeeType_GET_SLOT(self,tp_p_any_assign) = DeeType_DEFAULT_SLOT(tp_p_any_assign);
 DeeType_GET_SLOT(self,tp_p_str) = DeeType_DEFAULT_SLOT(tp_p_str);
 DeeType_GET_SLOT(self,tp_p_repr) = DeeType_DEFAULT_SLOT(tp_p_repr);
 DeeType_GET_SLOT(self,tp_p_int32) = DeeType_DEFAULT_SLOT(tp_p_int32);
 DeeType_GET_SLOT(self,tp_p_int64) = DeeType_DEFAULT_SLOT(tp_p_int64);
 DeeType_GET_SLOT(self,tp_p_double) = DeeType_DEFAULT_SLOT(tp_p_double);
 DeeType_GET_SLOT(self,tp_p_call) = DeeType_DEFAULT_SLOT(tp_p_call);
 DeeType_GET_SLOT(self,tp_p_not) = DeeType_DEFAULT_SLOT(tp_p_not);
 DeeType_GET_SLOT(self,tp_p_bool) = DeeType_DEFAULT_SLOT(tp_p_bool);
 DeeType_GET_SLOT(self,tp_p_inv) = DeeType_DEFAULT_SLOT(tp_p_inv);
 DeeType_GET_SLOT(self,tp_p_pos) = DeeType_DEFAULT_SLOT(tp_p_pos);
 DeeType_GET_SLOT(self,tp_p_neg) = DeeType_DEFAULT_SLOT(tp_p_neg);
 DeeType_GET_SLOT(self,tp_p_inc) = DeeType_DEFAULT_SLOT(tp_p_inc);
 DeeType_GET_SLOT(self,tp_p_incpost) = DeeType_DEFAULT_SLOT(tp_p_incpost);
 DeeType_GET_SLOT(self,tp_p_dec) = DeeType_DEFAULT_SLOT(tp_p_dec);
 DeeType_GET_SLOT(self,tp_p_decpost) = DeeType_DEFAULT_SLOT(tp_p_decpost);
 DeeType_GET_SLOT(self,tp_p_add) = DeeType_DEFAULT_SLOT(tp_p_add);
 DeeType_GET_SLOT(self,tp_p_iadd) = DeeType_DEFAULT_SLOT(tp_p_iadd);
 DeeType_GET_SLOT(self,tp_p_sub) = DeeType_DEFAULT_SLOT(tp_p_sub);
 DeeType_GET_SLOT(self,tp_p_isub) = DeeType_DEFAULT_SLOT(tp_p_isub);
 DeeType_GET_SLOT(self,tp_p_mul) = DeeType_DEFAULT_SLOT(tp_p_mul);
 DeeType_GET_SLOT(self,tp_p_imul) = DeeType_DEFAULT_SLOT(tp_p_imul);
 DeeType_GET_SLOT(self,tp_p_div) = DeeType_DEFAULT_SLOT(tp_p_div);
 DeeType_GET_SLOT(self,tp_p_idiv) = DeeType_DEFAULT_SLOT(tp_p_idiv);
 DeeType_GET_SLOT(self,tp_p_mod) = DeeType_DEFAULT_SLOT(tp_p_mod);
 DeeType_GET_SLOT(self,tp_p_imod) = DeeType_DEFAULT_SLOT(tp_p_imod);
 DeeType_GET_SLOT(self,tp_p_shl) = DeeType_DEFAULT_SLOT(tp_p_shl);
 DeeType_GET_SLOT(self,tp_p_ishl) = DeeType_DEFAULT_SLOT(tp_p_ishl);
 DeeType_GET_SLOT(self,tp_p_shr) = DeeType_DEFAULT_SLOT(tp_p_shr);
 DeeType_GET_SLOT(self,tp_p_ishr) = DeeType_DEFAULT_SLOT(tp_p_ishr);
 DeeType_GET_SLOT(self,tp_p_and) = DeeType_DEFAULT_SLOT(tp_p_and);
 DeeType_GET_SLOT(self,tp_p_iand) = DeeType_DEFAULT_SLOT(tp_p_iand);
 DeeType_GET_SLOT(self,tp_p_or) = DeeType_DEFAULT_SLOT(tp_p_or);
 DeeType_GET_SLOT(self,tp_p_ior) = DeeType_DEFAULT_SLOT(tp_p_ior);
 DeeType_GET_SLOT(self,tp_p_xor) = DeeType_DEFAULT_SLOT(tp_p_xor);
 DeeType_GET_SLOT(self,tp_p_ixor) = DeeType_DEFAULT_SLOT(tp_p_ixor);
 DeeType_GET_SLOT(self,tp_p_pow) = DeeType_DEFAULT_SLOT(tp_p_pow);
 DeeType_GET_SLOT(self,tp_p_ipow) = DeeType_DEFAULT_SLOT(tp_p_ipow);
 DeeType_GET_SLOT(self,tp_p_hash) = DeeType_DEFAULT_SLOT(tp_p_hash);
 DeeType_GET_SLOT(self,tp_p_cmp_lo) = DeeType_DEFAULT_SLOT(tp_p_cmp_lo);
 DeeType_GET_SLOT(self,tp_p_cmp_le) = DeeType_DEFAULT_SLOT(tp_p_cmp_le);
 DeeType_GET_SLOT(self,tp_p_cmp_eq) = DeeType_DEFAULT_SLOT(tp_p_cmp_eq);
 DeeType_GET_SLOT(self,tp_p_cmp_ne) = DeeType_DEFAULT_SLOT(tp_p_cmp_ne);
 DeeType_GET_SLOT(self,tp_p_cmp_gr) = DeeType_DEFAULT_SLOT(tp_p_cmp_gr);
 DeeType_GET_SLOT(self,tp_p_cmp_ge) = DeeType_DEFAULT_SLOT(tp_p_cmp_ge);
 DeeType_GET_SLOT(self,tp_p_seq_get) = DeeType_DEFAULT_SLOT(tp_p_seq_get);
 DeeType_GET_SLOT(self,tp_p_seq_del) = DeeType_DEFAULT_SLOT(tp_p_seq_del);
 DeeType_GET_SLOT(self,tp_p_seq_set) = DeeType_DEFAULT_SLOT(tp_p_seq_set);
 DeeType_GET_SLOT(self,tp_p_seq_size) = DeeType_DEFAULT_SLOT(tp_p_seq_size);
 DeeType_GET_SLOT(self,tp_p_seq_contains) = DeeType_DEFAULT_SLOT(tp_p_seq_contains);
 DeeType_GET_SLOT(self,tp_p_seq_range_get) = DeeType_DEFAULT_SLOT(tp_p_seq_range_get);
 DeeType_GET_SLOT(self,tp_p_seq_range_del) = DeeType_DEFAULT_SLOT(tp_p_seq_range_del);
 DeeType_GET_SLOT(self,tp_p_seq_range_set) = DeeType_DEFAULT_SLOT(tp_p_seq_range_set);
 DeeType_GET_SLOT(self,tp_p_seq_iter_self) = DeeType_DEFAULT_SLOT(tp_p_seq_iter_self);
 DeeType_GET_SLOT(self,tp_p_attr_get) = DeeType_DEFAULT_SLOT(tp_p_attr_get);
 DeeType_GET_SLOT(self,tp_p_attr_del) = DeeType_DEFAULT_SLOT(tp_p_attr_del);
 DeeType_GET_SLOT(self,tp_p_attr_set) = DeeType_DEFAULT_SLOT(tp_p_attr_set);
}

static int _deestructuredtype_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeStructuredTypeObject *self) {
 if DEE_UNLIKELY((*DeeType_GET_SLOT(&DeeType_Type,tp_ctor))(
  &DeeType_Type,(DeeObject *)self) != 0) return -1;
 DEE_ASSERT(DeeType_GET_SLOT(self,tp_base) && "Should be initialized");
 Dee_DECREF(DeeType_GET_SLOT(self,tp_base));
 Dee_INCREF(DeeType_GET_SLOT(self,tp_base) = (DeeTypeObject *)&DeeStructured_Type);
 _DeeStructuredType_InitCommon(self);
 return 0;
}
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
static void _deepointertype_tp_dtor(DeePointerTypeObject *self) {
 Dee_DECREF(self->tp_ptr_pointer_base);
}
DEE_VISIT_PROC(_deepointertype_tp_visit,DeePointerTypeObject *self) {
 Dee_VISIT(self->tp_ptr_pointer_base);
}
static void _deelvaluetype_tp_dtor(DeeLValueTypeObject *self) {
 Dee_DECREF(self->tp_lval_lvalue_base);
}
DEE_VISIT_PROC(_deelvaluetype_tp_visit,DeeLValueTypeObject *self) {
 DEE_ASSERT(!DeeLValueType_Check(self->tp_lval_lvalue_base));
 Dee_VISIT(self->tp_lval_lvalue_base);
}
// Forward any member access on lvalue-types to the underlying type
// >> This enables stuff like: '((int &)).tp_p_instance_size' to corrently yield 'sizeof(int)'
static DeeObject *_deelvaluetype_tp_attr_get(
 DeeLValueTypeObject *self, DeeObject *attr) {
 DeeObject *result; int temp;
 if DEE_UNLIKELY((temp = _DeeType_GetAttrString((
  DeeTypeObject *)self,DeeString_STR(attr),&result)) < 0) return NULL;
 if (temp == 0) return result;
 return DeeObject_GetAttr((DeeObject *)self->tp_lval_lvalue_base,attr);
}
static int _deelvaluetype_tp_attr_del(
 DeeLValueTypeObject *self, DeeObject *attr) {
 int temp;
 if DEE_UNLIKELY((temp = _DeeType_DelAttrString((
  DeeTypeObject *)self,DeeString_STR(attr))) <= 0) return temp;
 return DeeObject_DelAttr((DeeObject *)self->tp_lval_lvalue_base,attr);
}
static int _deelvaluetype_tp_attr_set(
 DeeLValueTypeObject *self, DeeObject *attr, DeeObject *v) {
 int temp;
 if DEE_UNLIKELY((temp = _DeeType_SetAttrString(
  (DeeTypeObject *)self,DeeString_STR(attr),v)) <= 0) return temp;
 return DeeObject_SetAttr((DeeObject *)self->tp_lval_lvalue_base,attr,v);
}
#endif


#if !DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define _deestructuredtype_tp_class_members DeeType_DEFAULT_SLOT(tp_class_members)
#else /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
static struct DeeMemberDef const _deestructuredtype_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("pointer_type",object,&DeePointerType_Type),
 DEE_MEMBERDEF_CONST_v100("lvalue_type",object,&DeeLValueType_Type),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
static struct DeeMemberDef const _deestructuredtype_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__p_instance_size__",DeeStructuredTypeObject,tp_p_constructor.tp_p_instance_size,Dee_size_t),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__tp_p_instance_size",DeeStructuredTypeObject,tp_p_constructor.tp_p_instance_size,Dee_size_t),
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DEE_MEMBERDEF_NAMED_RO_v100("__stp_prev_structured_object_type",DeeStructuredTypeObject,stp_prev_structured_object_type,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__stp_pointer",DeeStructuredTypeObject,stp_pointer,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__stp_lvalue",DeeStructuredTypeObject,stp_lvalue,object_null),
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
 DEE_MEMBERDEF_NAMED_RO_v100("__stp_ffunctions_fftl_lock",DeeStructuredTypeObject,stp_ffunctions.fftl_lock,DeeAtomicMutex),
 DEE_MEMBERDEF_NAMED_RO_v100("__stp_ffunctions_fftl_typec",DeeStructuredTypeObject,stp_ffunctions.fftl_lock,Dee_size_t),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__stp_ffunctions_fftl_typev",DeeStructuredTypeObject,stp_ffunctions.fftl_typev,p2(void),"-> DeeForeignFunctionTypeObject **"),
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 DEE_MEMBERDEF_NAMED_RO_v100("__stp_arrays_atl_lock",DeeStructuredTypeObject,stp_arrays.atl_lock,DeeAtomicMutex),
 DEE_MEMBERDEF_NAMED_RO_v100("__stp_arrays_atl_typec",DeeStructuredTypeObject,stp_arrays.atl_typec,Dee_size_t),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__stp_arrays_atl_typev",DeeStructuredTypeObject,stp_arrays.atl_typev,p2(void),"-> DeeArrayTypeObject **"),
 DEE_MEMBERDEF_NAMED_RO_v100("__stp_varray",DeeStructuredTypeObject,stp_varray,object_null),
#endif /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};

#if DEE_XCONFIG_HAVE_STRUCTURED_TYPE_SLOT_MEMBERS
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
// Make the actual type operators available in developer builds
#define DEE_STRUCTUREDTYPESLOT_GETSET(name,tystring) \
static DeeObject *_deestructuredtype_##name##_get(DeeStructuredTypeObject *self, void *DEE_UNUSED(closure))\
{\
 return DeePointer_Newf((void *)DeeType_GET_SLOT(self,name),tystring);\
}\
static int _deestructuredtype_##name##_del(DeeStructuredTypeObject *self, void *DEE_UNUSED(closure))\
{\
 DeeType_DEL_SLOT(self,name);\
 return 0;\
}\
static int _deestructuredtype_##name##_set(DeeStructuredTypeObject *self, DeeObject *v, void *DEE_UNUSED(closure))\
{\
 DeeType_SLOT_TYPE(name) new_v;\
 if DEE_UNLIKELY(DeeObject_GetPointerExf(v,(void **)&new_v,tystring) != 0) return -1;\
 DeeType_GET_SLOT(self,name) = new_v;\
 return 0;\
}
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
DEE_COMPILER_MSVC_WARNING_PUSH(4052 4054)
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_any_ctor,"d(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_any_assign,"d(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_str,"o(op)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_repr,"o(op)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_int32,"d(opI32d*)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_int64,"d(opI64d*)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_double,"d(opD*)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_call,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_not,"o(op)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_bool,"d(op)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_inv,"o(op)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_pos,"o(op)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_neg,"o(op)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_inc,"d(op)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_incpost,"o(op)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_dec,"d(op)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_decpost,"o(op)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_add,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_iadd,"d(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_sub,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_isub,"d(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_mul,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_imul,"d(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_div,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_idiv,"d(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_mod,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_imod,"d(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_shl,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_ishl,"d(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_shr,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_ishr,"d(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_and,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_iand,"d(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_or,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_ior,"d(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_xor,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_ixor,"d(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_pow,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_ipow,"d(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_hash,"d(opI"
 DEE_PP_STR(DEE_PP_MUL8(DEE_CONFIG_SIZEOF_DEE_HASH_T)) "uI"
 DEE_PP_STR(DEE_PP_MUL8(DEE_CONFIG_SIZEOF_DEE_HASH_T)) "u*)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_cmp_lo,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_cmp_le,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_cmp_eq,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_cmp_ne,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_cmp_gr,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_cmp_ge,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_seq_get,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_seq_del,"d(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_seq_set,"d(opoo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_seq_size,"o(op)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_seq_contains,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_seq_range_get,"o(opoo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_seq_range_del,"d(opoo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_seq_range_set,"d(opooo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_seq_iter_self,"o(op)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_attr_get,"o(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_attr_del,"d(opo)")
DEE_STRUCTUREDTYPESLOT_GETSET(tp_p_attr_set,"d(opoo)")
DEE_COMPILER_MSVC_WARNING_POP
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#undef DEE_STRUCTUREDTYPESLOT_GETSET
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#endif /* DEE_XCONFIG_HAVE_STRUCTURED_TYPE_SLOT_MEMBERS */

#if DEE_XCONFIG_HAVE_STRUCTURED_TYPE_SLOT_MEMBERS
static struct DeeGetSetDef _deestructuredtype_tp_getsets[] = {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DEE_STRUCTUREDTYPESLOT_GETSET_REF(name,tydoc)\
 DEE_GETSETDEF_v100("__" #name,\
  member(&_deestructuredtype_##name##_get),\
  member(&_deestructuredtype_##name##_del),\
  member(&_deestructuredtype_##name##_set),\
  "-> " tydoc)
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_any_ctor,"int(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_any_assign,"int(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_str,"object(*)(object,none *)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_repr,"object(*)(object,none *)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_int32,"int(*)(object,none *,int32_t *)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_int64,"int(*)(object,none *,int64_t *)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_double,"int(*)(object,none *,double *)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_call,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_not,"object(*)(object,none *)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_bool,"int(*)(object,none *)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_inv,"object(*)(object,none *)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_pos,"object(*)(object,none *)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_neg,"object(*)(object,none *)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_inc,"int(*)(object,none *)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_incpost,"object(*)(object,none *)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_dec,"int(*)(object,none *)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_decpost,"object(*)(object,none *)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_add,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_iadd,"int(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_sub,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_isub,"int(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_mul,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_imul,"int(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_div,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_idiv,"int(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_mod,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_imod,"int(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_shl,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_ishl,"int(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_shr,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_ishr,"int(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_and,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_iand,"int(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_or,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_ior,"int(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_xor,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_ixor,"int(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_pow,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_ipow,"int(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_hash,"int(*)(object,none *,hash_t,hash_t*)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_cmp_lo,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_cmp_le,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_cmp_eq,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_cmp_ne,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_cmp_gr,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_cmp_ge,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_seq_get,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_seq_del,"int(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_seq_set,"int(*)(object,none *,object,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_seq_size,"object(*)(object,none *)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_seq_contains,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_seq_range_get,"object(*)(object,none *,object,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_seq_range_del,"int(*)(object,none *,object,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_seq_range_set,"int(*)(object,none *,object,object,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_seq_iter_self,"object(*)(object,none *)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_attr_get,"object(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_attr_del,"int(*)(object,none *,object)"),
 DEE_STRUCTUREDTYPESLOT_GETSET_REF(tp_p_attr_set,"int(*)(object,none *,object,object)"),
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#undef DEE_STRUCTUREDTYPESLOT_GETSET_REF
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DEE_GETSETDEF_END_v100
};
#else /* DEE_XCONFIG_HAVE_STRUCTURED_TYPE_SLOT_MEMBERS */
#define _deestructuredtype_tp_getsets DeeType_DEFAULT_SLOT(tp_getsets)
#endif /* !DEE_XCONFIG_HAVE_STRUCTURED_TYPE_SLOT_MEMBERS */


extern int DEE_CALL _deestructuredtype_tp_marshal_ctor(
 DeeTypeObject *tp_self, DeeStructuredTypeObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 if DEE_UNLIKELY(_deetype_tp_marshal_ctor(tp_self,(DeeTypeObject *)self,infile,map) != 0) return -1;
 _DeeStructuredType_InitCommon(self);
 return 0;
}

static struct DeeTypeMarshal _deestructuredtype_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALID_NONE),
 member(&_deestructuredtype_tp_marshal_ctor),member(&_deetype_tp_marshal_put));


DeeTypeObject DeeStructuredType_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("structured_type"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_DONT_COPY),member(&DeeType_Type)),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeStructuredTypeObject),null,
  member(&_deestructuredtype_tp_ctor),null,null,null),
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION || DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deestructuredtype_tp_dtor)),
#else /* ... */
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
#endif /* ... */
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&_deestructuredtype_tp_seq_get),
  null,null,null,null,null,null,null,null,null),
#else /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
#endif /* !DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deestructuredtype_tp_members),member(_deestructuredtype_tp_getsets),null,
  member(_deestructuredtype_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deestructuredtype_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};

#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
static struct DeeMemberDef const _deepointertype_tp_members[] = {
 DEE_MEMBERDEF_RO_v100(DeePointerTypeObject,tp_ptr_elem_size,Dee_size_t),
 DEE_MEMBERDEF_RO_v100(DeePointerTypeObject,tp_ptr_pointer_base,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("pointer_base",DeePointerTypeObject,tp_ptr_pointer_base,object_null),
 DEE_MEMBERDEF_END_v100
};
static struct DeeMemberDef const _deelvaluetype_tp_members[] = {
 DEE_MEMBERDEF_RO_v100(DeeLValueTypeObject,tp_lval_lvalue_base,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("lvalue_base",DeeLValueTypeObject,tp_lval_lvalue_base,object_null),
 DEE_MEMBERDEF_END_v100
};
static struct DeeTypeMarshal _deepointertype_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALID_POINTERTYPE),member(NULL),member(NULL)); // Special marshaling
static struct DeeTypeMarshal _deelvaluetype_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALID_LVALUETYPE),member(NULL),member(NULL)); // Special marshaling
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */

#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
DeeTypeObject DeePointerType_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("pointer_type"),null,member(
  DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_DONT_COPY|
  DEE_TYPE_FLAG_NO_SUBCLASS),member(&DeeStructuredType_Type)),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeePointerTypeObject),null,null,null,null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deepointertype_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deepointertype_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deepointertype_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deepointertype_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};
DeeTypeObject DeeLValueType_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("lvalue_type"),null,member(
  DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_DONT_COPY|
  DEE_TYPE_FLAG_NO_SUBCLASS),member(&DeeType_Type)),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeLValueTypeObject),null,null,null,null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deelvaluetype_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,
  member(&_deelvaluetype_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(
  member(&_deelvaluetype_tp_attr_get),
  member(&_deelvaluetype_tp_attr_del),
  member(&_deelvaluetype_tp_attr_set),
  member(_deelvaluetype_tp_members),null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deelvaluetype_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */

int DEE_CALL _deestructured_tp_marshal_ctor(
 DeeTypeObject *tp_self, DeeStructuredObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *DEE_UNUSED(map)) {
 Dee_uint64_t read_size; Dee_size_t correct_size;
 if DEE_UNLIKELY(DeeFile_GetLeSmall64(infile,&read_size) != 0) return -1;
 correct_size = DeeType_GET_SLOT(tp_self,tp_p_instance_size);
 if DEE_UNLIKELY(
#if DEE_TYPES_SIZEOF_SIZE_T < 8
  read_size > (Dee_uint64_t)((Dee_size_t)-1) ||
#endif
  (Dee_size_t)read_size != correct_size) {
  // This can happen if you try to stream something with varying size,
  // such as a pointer, from a 64-bit platform to a 32-bit platform.
  DeeError_SetStringf(&DeeErrorType_TypeError,
                      "Serialized size of structured object %q (%I64u bytes) "
                      "doesn't match actual size (%Iu bytes)",
                      DeeType_NAME(tp_self),read_size,correct_size);
  return -1;
 }
 return DeeFile_ReadAll(infile,DeeStructured_DATA(self),correct_size);
}
int DEE_CALL _deestructured_tp_marshal_put(
 DeeTypeObject *tp_self, DeeStructuredObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *DEE_UNUSED(map)) {
 Dee_size_t structured_size; Dee_uint64_t written_size;
 written_size = (Dee_uint64_t)(structured_size = DeeType_GET_SLOT(tp_self,tp_p_instance_size));
 if DEE_UNLIKELY(DeeFile_PutLeSmall64(outfile,written_size) != 0) return -1;
 return DeeFile_WriteAll(outfile,DeeStructured_DATA(self),structured_size);
}

static struct DeeTypeMarshal _deestructured_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALID_INSTANCE),
 member(&_deestructured_tp_marshal_ctor),member(&_deestructured_tp_marshal_put));

DeeStructuredTypeObject DeeStructured_Type = {
 { // tp_type
  DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeStructuredType_Type),
   member("structured_object"),null,
   member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeStructuredObject),null,
   member(&_DeeGeneric_Noop), // no-op constructor
   member(&_deestructuredobject_tp_copy_ctor),
   member(&_deestructuredobject_tp_copy_ctor),
   member(&_deestructuredobject_tp_any_ctor)),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(&_deestructuredobject_tp_copy_assign),
   member(&_deestructuredobject_tp_copy_assign),
   member(&_deestructuredobject_tp_any_assign)),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&_deestructuredobject_tp_str),
   member(&_deestructuredobject_tp_repr),
   member(&_deestructuredobject_tp_int32),
   member(&_deestructuredobject_tp_int64),
   member(&_deestructuredobject_tp_double)),
  DEE_TYPE_OBJECT_OBJECT_v100(
   member(&_deestructuredobject_tp_call),null),
  DEE_TYPE_OBJECT_MATH_v101(
   member(&_deestructuredobject_tp_not),member(&_deestructuredobject_tp_bool),
   member(&_deestructuredobject_tp_inv),
   member(&_deestructuredobject_tp_pos),member(&_deestructuredobject_tp_neg),
   member(&_deestructuredobject_tp_inc),member(&_deestructuredobject_tp_incpost),
   member(&_deestructuredobject_tp_dec),member(&_deestructuredobject_tp_decpost),
   member(&_deestructuredobject_tp_add),member(&_deestructuredobject_tp_iadd),
   member(&_deestructuredobject_tp_sub),member(&_deestructuredobject_tp_isub),
   member(&_deestructuredobject_tp_mul),member(&_deestructuredobject_tp_imul),
   member(&_deestructuredobject_tp_div),member(&_deestructuredobject_tp_idiv),
   member(&_deestructuredobject_tp_mod),member(&_deestructuredobject_tp_imod),
   member(&_deestructuredobject_tp_shl),member(&_deestructuredobject_tp_ishl),
   member(&_deestructuredobject_tp_shr),member(&_deestructuredobject_tp_ishr),
   member(&_deestructuredobject_tp_and),member(&_deestructuredobject_tp_iand),
   member(&_deestructuredobject_tp_or),member(&_deestructuredobject_tp_ior),
   member(&_deestructuredobject_tp_xor),member(&_deestructuredobject_tp_ixor),
   member(&_deestructuredobject_tp_pow),member(&_deestructuredobject_tp_ipow),
   member(&_deestructuredobject_tp_hash)),
  DEE_TYPE_OBJECT_COMPARE_v100(
   member(&_deestructuredobject_tp_cmp_lo),
   member(&_deestructuredobject_tp_cmp_le),
   member(&_deestructuredobject_tp_cmp_eq),
   member(&_deestructuredobject_tp_cmp_ne),
   member(&_deestructuredobject_tp_cmp_gr),
   member(&_deestructuredobject_tp_cmp_ge)),
  DEE_TYPE_OBJECT_SEQ_v101(
   member(&_deestructuredobject_tp_seq_get),
   member(&_deestructuredobject_tp_seq_del),
   member(&_deestructuredobject_tp_seq_set),
   member(&_deestructuredobject_tp_seq_size),
   member(&_deestructuredobject_tp_seq_contains),
   member(&_deestructuredobject_tp_seq_range_get),
   member(&_deestructuredobject_tp_seq_range_del),
   member(&_deestructuredobject_tp_seq_range_set),
   member(&_deestructuredobject_tp_seq_iter_self),null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(
   member(&DeeStructured_GetAttr),
   member(&DeeStructured_DelAttr),
   member(&DeeStructured_SetAttr),
   null,null,null,null,null,null),
  DEE_TYPE_OBJECT_MARSHAL_v101(
   member(&_deestructured_tp_marshal)),
  DEE_TYPE_OBJECT_FOOTER_v101
 },{NULL},
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
 &DeePointer_Type, // tp_pointer
 &DeeLValue_Type,  // tp_lvalue
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
 DEE_PRIVATE_STRUCTURED_TYPE_OBJECT_HEADER_FOREIGNFUNCTION_v100
 DEE_PRIVATE_STRUCTURED_TYPE_OBJECT_HEADER_ARRAY_v100
 DEE_STRUCTURED_TYPE_OBJECT_CONSTRUCTOR_v100(
  member(sizeof(DeeStructuredObject)-sizeof(DeeObject)),null),
 DEE_STRUCTURED_TYPE_OBJECT_ASSIGN_v100(null),
 DEE_STRUCTURED_TYPE_OBJECT_CAST_v100(null,null,null,null,null),
 DEE_STRUCTURED_TYPE_OBJECT_OBJECT_v100(null),
 DEE_STRUCTURED_TYPE_OBJECT_MATH_v100(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_STRUCTURED_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_STRUCTURED_TYPE_OBJECT_SEQ_v100(null,null,null,null,null,null,null,null,null),
 DEE_STRUCTURED_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null),
 DEE_STRUCTURED_TYPE_OBJECT_FOOTER_v100
};

#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
DeePointerTypeObject DeePointer_Type = {
 { // tp_type
  { // tp_type.tp_type
   DEE_TYPE_OBJECT_HEAD_EX_v100(
    member(&DeePointerType_Type),member("pointer"),null,
    member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),
    member((DeeTypeObject *)&DeeStructured_Type)),
   DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeePointerObject),
    member(&_deepointer_tp_alloc),null,null,null,null),
   DEE_TYPE_OBJECT_DESTRUCTOR_v100(
    member(&_deepointer_tp_free),null),
   DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
   DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
   DEE_TYPE_OBJECT_OBJECT_v100(null,null),
   DEE_TYPE_OBJECT_MATH_v101(
    null,null,null,null,null,null,null,null,null,null,null,
    null,null,null,null,null,null,null,null,null,null,null,
    null,null,null,null,null,null,null,null,null,null),
   DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
   DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
   DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
   DEE_TYPE_OBJECT_FOOTER_v100
  },{NULL},
  &DeePointer_Type, // tp_type.tp_pointer
  &DeeLValue_Type,  // tp_type.tp_lvalue
  DEE_PRIVATE_STRUCTURED_TYPE_OBJECT_HEADER_FOREIGNFUNCTION_v100
  DEE_PRIVATE_STRUCTURED_TYPE_OBJECT_HEADER_ARRAY_v100
  DEE_STRUCTURED_TYPE_OBJECT_CONSTRUCTOR_v100(
   member(sizeof(DeePointerObject)-sizeof(DeeObject)),
   member((DeeType_SLOT_TYPE(tp_p_any_ctor))&_deepointer_tp_p_any_ctor)),
  DEE_STRUCTURED_TYPE_OBJECT_ASSIGN_v100(
   member(&_deepointer_tp_p_any_assign)),
  DEE_STRUCTURED_TYPE_OBJECT_CAST_v100(
   member(&_deepointer_tp_p_str),
   member(&_deepointer_tp_p_repr),
   member(&_deepointer_tp_p_int32),
   member(&_deepointer_tp_p_int64),
   member(&_deepointer_tp_p_double)),
  DEE_STRUCTURED_TYPE_OBJECT_OBJECT_v100(null),
  DEE_STRUCTURED_TYPE_OBJECT_MATH_v100(
   member(&_deepointer_tp_p_not),
   member(&_deepointer_tp_p_bool),null,null,null,
   member(&_deepointer_tp_p_inc),
   member(&_deepointer_tp_p_incpost),
   member(&_deepointer_tp_p_dec),
   member(&_deepointer_tp_p_decpost),
   member(&_deepointer_tp_p_add),
   member(&_deepointer_tp_p_iadd),
   member(&_deepointer_tp_p_sub),
   member(&_deepointer_tp_p_isub),
   null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_STRUCTURED_TYPE_OBJECT_COMPARE_v100(
   member(&_deepointer_tp_p_cmp_lo),
   member(&_deepointer_tp_p_cmp_le),
   member(&_deepointer_tp_p_cmp_eq),
   member(&_deepointer_tp_p_cmp_ne),
   member(&_deepointer_tp_p_cmp_gr),
   member(&_deepointer_tp_p_cmp_ge)),
  DEE_STRUCTURED_TYPE_OBJECT_SEQ_v100(
   member((DeeType_SLOT_TYPE(tp_p_seq_get))&_deepointer_tp_p_seq_get),null,
   member((DeeType_SLOT_TYPE(tp_p_seq_set))&_deepointer_tp_p_seq_set),null,
   // TODO: range get/set (could be used for memcpy / from iterable)
   null,null,null,null,null),
  DEE_STRUCTURED_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null),
  DEE_STRUCTURED_TYPE_OBJECT_FOOTER_v100
 },
 sizeof(DeeStructuredObject)-sizeof(DeeObject), // tp_ptr_elem_size
 &DeeStructured_Type,                           // tp_ptr_pointer_base
};

DeeLValueTypeObject DeeLValue_Type = {
 { // tp_type
  DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeLValueType_Type),member("lvalue"),null,
   member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeePointerObject),
   member(&_deepointer_tp_alloc),null,null,null,null),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(
   member(&_deepointer_tp_free),null),
  DEE_TYPE_OBJECT_ASSIGN_v100(
   member(&_deelvalue_tp_any_assign),
   member(&_deelvalue_tp_any_assign),
   member(&_deelvalue_tp_any_assign)),
  DEE_TYPE_OBJECT_CAST_v101(
   member(&_deelvalue_tp_str),member(&_deelvalue_tp_repr),
   member(&_deelvalue_tp_int32),member(&_deelvalue_tp_int64),
   member(&_deelvalue_tp_double)),
  DEE_TYPE_OBJECT_OBJECT_v100(
   member(&_deelvalue_tp_call),null),
  DEE_TYPE_OBJECT_MATH_v101(
   member(&_deelvalue_tp_not),member(&_deelvalue_tp_bool),member(&_deelvalue_tp_inv),
   member(&_deelvalue_tp_pos),member(&_deelvalue_tp_neg),
   member(&_deelvalue_tp_inc),member(&_deelvalue_tp_incpost),
   member(&_deelvalue_tp_dec),member(&_deelvalue_tp_decpost),
   member(&_deelvalue_tp_add),member(&_deelvalue_tp_iadd),
   member(&_deelvalue_tp_sub),member(&_deelvalue_tp_isub),
   member(&_deelvalue_tp_mul),member(&_deelvalue_tp_imul),
   member(&_deelvalue_tp_div),member(&_deelvalue_tp_idiv),
   member(&_deelvalue_tp_mod),member(&_deelvalue_tp_imod),
   member(&_deelvalue_tp_shl),member(&_deelvalue_tp_ishl),
   member(&_deelvalue_tp_shr),member(&_deelvalue_tp_ishr),
   member(&_deelvalue_tp_and),member(&_deelvalue_tp_iand),
   member(&_deelvalue_tp_or),member(&_deelvalue_tp_ior),
   member(&_deelvalue_tp_xor),member(&_deelvalue_tp_ixor),
   member(&_deelvalue_tp_pow),member(&_deelvalue_tp_ipow),
   member(&_deelvalue_tp_hash)),
  DEE_TYPE_OBJECT_COMPARE_v100(
   member(&_deelvalue_tp_cmp_lo),member(&_deelvalue_tp_cmp_le),
   member(&_deelvalue_tp_cmp_eq),member(&_deelvalue_tp_cmp_ne),
   member(&_deelvalue_tp_cmp_gr),member(&_deelvalue_tp_cmp_ge)),
  DEE_TYPE_OBJECT_SEQ_v101(
   member(&_deelvalue_tp_seq_get),member(&_deelvalue_tp_seq_del),
   member(&_deelvalue_tp_seq_set),member(&_deelvalue_tp_seq_size),
   member(&_deelvalue_tp_seq_contains),member(&_deelvalue_tp_seq_range_get),
   member(&_deelvalue_tp_seq_range_del),member(&_deelvalue_tp_seq_range_set),
   member(&_deelvalue_tp_seq_iter_self),null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(
   member(&_deelvalue_tp_attr_get),
   member(&_deelvalue_tp_attr_del),
   member(&_deelvalue_tp_attr_set),
   null,null,null,null,null,null),
  DEE_TYPE_OBJECT_FOOTER_v100
 },
 {NULL},                            // tp_prev_xxx_type
 &DeeStructured_Type,               // tp_lval_lvalue_base
 DeeForeignFunctionTypeList_INIT(), // tp_ffunctions
};
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */

DEE_DECL_END

#ifndef __INTELLISENSE__
#include "structured.typef.inl"
#endif

#endif /* !GUARD_DEEMON_STRUCTURED_C */
