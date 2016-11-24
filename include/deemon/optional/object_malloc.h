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
#ifndef GUARD_DEEMON_OPTIONAL_OBJECT_MALLOC_H
#define GUARD_DEEMON_OPTIONAL_OBJECT_MALLOC_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/object_decl.h>
#ifdef DEE_LIMITED_DEX
#include <deemon/optional/object_refcnt.h>
#include <deemon/optional/type_decl.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif

//////////////////////////////////////////////////////////////////////////
// v frees the object without calling the destructor
//   - required to free objects after the constructor failed
//   - NOTE: This does actually call the tp_free function of Dee_TYPE(self)
//           and decrefs the type afterwards, so DeeObject_INIT must have been called
//     NOTE: When you have to free a gc object, make sure that it is untracked before calling this
//   >> DeeObject *DeeMyOb_New(void) {
//   >>   DeeObject *allocated_object;
//   >>   allocated_object = (*DeeType_GET_SLOT(&my_tp,tp_alloc))(&my_tp);
//   >>   if DEE_UNLIKELY(!allocated_object) return NULL;
//   >>   if DEE_UNLIKELY(init_ob(allocated_object) != 0) {
//   >>     _DeeObject_DELETE(&my_tp,allocated_object);
//   >>     allocated_object = NULL;
//   >>   }
//   >>   return allocated_object;
//   >> }
DEE_FUNC_DECL(void) _DeeObject_Delete(
 DEE_A_IN DeeTypeObject const *tp_self,
 DEE_A_IN DEE_A_OUT_INVALID DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
#ifdef DEE_LIMITED_DEX
#define _DeeObject_DELETE(tp_ob,ob)\
do{\
 DEE_ASSERTF(DeeObject_Check(ob),"Invalid object");\
 DEE_ASSERTF(DeeObject_Check(tp_ob) && DeeType_Check(tp_ob),"Invalid type");\
 DEE_ASSERTF(DeeObject_IS_UNIQUE(ob),"Object isn't unique");\
 {\
  DeeTypeObject *const _stored_tp = Dee_TYPE(ob); /* transfer reference. */\
  ((DeeObject *)(ob))->__ob_type = NULL; /* Set type to NULL during calls to tp_free. */\
  DEE_ASSERT(DeeObject_Check(_stored_tp) && DeeType_Check(_stored_tp));\
  DEE_ASSERTF(DeeType_IsSameOrDerived((DeeTypeObject *)(tp_ob),_stored_tp),\
              "The object type %q isn't derived from %q",\
              DeeType_NAME(_stored_tp),DeeType_NAME(tp_ob));\
  (*DeeType_GET_SLOT(tp_ob,tp_free))((DeeTypeObject *)(tp_ob),(DeeObject *)(ob));\
  Dee_DECREF(_stored_tp); /* Drop reference to the type. */\
 }\
}while(0)
#else
#define _DeeObject_DELETE(tp_ob,ob) \
do{_DeeObject_Delete(\
 (DeeTypeObject const *)Dee_REQUIRES_POINTER(tp_ob),\
     (DeeObject       *)Dee_REQUIRES_POINTER(ob));\
}while(0)
#endif


//////////////////////////////////////////////////////////////////////////
// Simple, low-level allocators for objects
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_EXEC DEE_A_RET_NOEXCEPT(NULL) void *) DeeObject_TryMalloc(DEE_A_IN Dee_size_t s);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_EXEC DEE_A_RET_NOEXCEPT(NULL) void *) DeeObject_TryCalloc(DEE_A_IN Dee_size_t s);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_EXEC DEE_A_RET_NOEXCEPT(NULL) void *) DeeObject_TryRealloc(DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) void *) DeeObject_Malloc(DEE_A_IN Dee_size_t s);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) void *) DeeObject_Calloc(DEE_A_IN Dee_size_t s);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) void *) DeeObject_Realloc(DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_EXEC DEE_A_RET_NOEXCEPT(NULL) void *) DeeObject_TryMalloc_d(DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_EXEC DEE_A_RET_NOEXCEPT(NULL) void *) DeeObject_TryCalloc_d(DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_EXEC DEE_A_RET_NOEXCEPT(NULL) void *) DeeObject_TryRealloc_d(DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) void *) DeeObject_Malloc_d(DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) void *) DeeObject_Calloc_d(DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) void *) DeeObject_Realloc_d(DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeObject_Free(DEE_A_IN DEE_A_OUT_INVALID void *p) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeObject_Free_d(DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN_Z_OPT char const *access) DEE_ATTRIBUTE_NONNULL((1));

DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeObject_TryWeakMalloc(DEE_A_IN Dee_size_t s);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeObject_TryWeakCalloc(DEE_A_IN Dee_size_t s);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeObject_TryWeakRealloc(DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeObject_WeakMalloc(DEE_A_IN Dee_size_t s);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeObject_WeakCalloc(DEE_A_IN Dee_size_t s);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeObject_WeakRealloc(DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeObject_TryWeakMalloc_d(DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeObject_TryWeakCalloc_d(DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_NOEXCEPT(NULL) void *) DeeObject_TryWeakRealloc_d(DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeObject_WeakMalloc_d(DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeObject_WeakCalloc_d(DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access);
DEE_FUNC_DECL(DEE_ATTRIBUTE_MALLOC DEE_A_RET_EXCEPT(NULL) void *) DeeObject_WeakRealloc_d(DEE_A_IN DEE_A_OUT_INVALID void *p, DEE_A_IN Dee_size_t s, DEE_A_IN_Z_OPT char const *access) DEE_ATTRIBUTE_NONNULL((1));

#ifdef DEE_DEBUG
#ifndef __INTELLISENSE__
#define /*DEE_A_EXEC*/DeeObject_Malloc(s)           DeeObject_Malloc_d(s,DEE_DEBUG_NEW_ACCESS("DeeObject_Malloc(" #s ")"))
#define /*DEE_A_EXEC*/DeeObject_Calloc(s)           DeeObject_Calloc_d(s,DEE_DEBUG_NEW_ACCESS("DeeObject_Calloc(" #s ")"))
#define /*DEE_A_EXEC*/DeeObject_Realloc(p,s)        DeeObject_Realloc_d(p,s,DEE_DEBUG_NEW_ACCESS("DeeObject_Realloc(" #p "," #s ")"))
#define /*DEE_A_EXEC*/DeeObject_TryMalloc(s)        DeeObject_TryMalloc_d(s,DEE_DEBUG_NEW_ACCESS("DeeObject_TryMalloc(" #s ")"))
#define /*DEE_A_EXEC*/DeeObject_TryCalloc(s)        DeeObject_TryCalloc_d(s,DEE_DEBUG_NEW_ACCESS("DeeObject_TryCalloc(" #s ")"))
#define /*DEE_A_EXEC*/DeeObject_TryRealloc(p,s)     DeeObject_TryRealloc_d(p,s,DEE_DEBUG_NEW_ACCESS("DeeObject_TryRealloc(" #p "," #s ")"))
#define /*DEE_A_EXEC*/DeeObject_Free(p)             DeeObject_Free_d(p,DEE_DEBUG_NEW_ACCESS("DeeObject_Free(" #p ")"))
#define               DeeObject_WeakMalloc(s)       DeeObject_WeakMalloc_d(s,DEE_DEBUG_NEW_ACCESS("DeeObject_WeakMalloc(" #s ")"))
#define               DeeObject_WeakCalloc(s)       DeeObject_WeakCalloc_d(s,DEE_DEBUG_NEW_ACCESS("DeeObject_WeakCalloc(" #s ")"))
#define               DeeObject_WeakRealloc(p,s)    DeeObject_WeakRealloc_d(p,s,DEE_DEBUG_NEW_ACCESS("DeeObject_WeakRealloc(" #p "," #s ")"))
#define               DeeObject_TryWeakMalloc(s)    DeeObject_TryWeakMalloc_d(s,DEE_DEBUG_NEW_ACCESS("DeeObject_TryWeakMalloc(" #s ")"))
#define               DeeObject_TryWeakCalloc(s)    DeeObject_TryWeakCalloc_d(s,DEE_DEBUG_NEW_ACCESS("DeeObject_TryWeakCalloc(" #s ")"))
#define               DeeObject_TryWeakRealloc(p,s) DeeObject_TryWeakRealloc_d(p,s,DEE_DEBUG_NEW_ACCESS("DeeObject_TryWeakRealloc(" #p "," #s ")"))
#endif
#endif

#define /*DEE_A_EXEC*/DeeObject_MALLOC(T) ((T*)DeeObject_Malloc(sizeof(T)))
#define /*DEE_A_EXEC*/DeeObject_MALLOCF(T,...) ((T*)DeeObject_Mallocf(sizeof(T),__VA_ARGS__))
#define /*DEE_A_EXEC*/DeeObject_Mallocf(s,...) DeeObject_Malloc(s)
#define /*DEE_A_EXEC*/DeeObject_Reallocf(p,s,...) DeeObject_Realloc(p,s)
#define /*DEE_A_EXEC*/DeeObject_VMallocf(s,fmt,args) DeeObject_Malloc(s)
#define /*DEE_A_EXEC*/DeeObject_VReallocf(p,s,fmt,args) DeeObject_Realloc(p,s)
#define /*DEE_A_EXEC*/DeeObject_CALLOC(T) ((T*)DeeObject_Calloc(sizeof(T)))
#define /*DEE_A_EXEC*/DeeObject_CALLOCF(T,...) ((T*)DeeObject_Callocf(sizeof(T),__VA_ARGS__))
#define /*DEE_A_EXEC*/DeeObject_Callocf(s,...) DeeObject_Calloc(s)
#define /*DEE_A_EXEC*/DeeObject_VCallocf(s,fmt,args) DeeObject_Calloc(s)
#define /*DEE_A_EXEC*/DeeObject_TRYMALLOC(T) ((T*)DeeObject_TryMalloc(sizeof(T)))
#define /*DEE_A_EXEC*/DeeObject_TRYMALLOCF(T,...) ((T*)DeeObject_TryMallocf(sizeof(T),__VA_ARGS__))
#define /*DEE_A_EXEC*/DeeObject_TryMallocf(s,...) DeeObject_TryMalloc(s)
#define /*DEE_A_EXEC*/DeeObject_TryReallocf(p,s,...) DeeObject_TryRealloc(p,s)
#define /*DEE_A_EXEC*/DeeObject_TryVMallocf(s,fmt,args) DeeObject_TryMalloc(s)
#define /*DEE_A_EXEC*/DeeObject_TryVReallocf(p,s,fmt,args) DeeObject_TryRealloc(p,s)
#define /*DEE_A_EXEC*/DeeObject_TRYCALLOC(T) ((T*)DeeObject_TryCalloc(sizeof(T)))
#define /*DEE_A_EXEC*/DeeObject_TRYCALLOCF(T,...) ((T*)DeeObject_TryCallocf(sizeof(T),__VA_ARGS__))
#define /*DEE_A_EXEC*/DeeObject_TryCallocf(s,...) DeeObject_TryCalloc(s)
#define /*DEE_A_EXEC*/DeeObject_TryVCallocf(s,fmt,args) DeeObject_TryCalloc(s)

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_OBJECT_MALLOC_H */
