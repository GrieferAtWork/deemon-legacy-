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
#ifndef GUARD_DEEMON_MP_TLS_H
#define GUARD_DEEMON_MP_TLS_H 1

#include <deemon/__conf.inl>

#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#include <deemon/none.h>
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

DEE_OBJECT_DEF(DeeTlsObject);

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// TLS (ThreadLocalStorage)
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#ifdef DEE_LIMITED_API
// High level (object-oriented) TLS Api
struct DeeTlsObject {
 DEE_OBJECT_HEAD
 /* v [1..1] Optional per-thread constructor returning the Initial value.
             NOTE: Set to Dee_None to disable.
             Should be a function like this: 'T tls_factory()'
             NOTE: Read-only after DeeTls_New() */
 DEE_A_REF DeeObject *tls_factory;
#ifndef DEE_WITHOUT_THREADS
 Dee_size_t           tls_handle; /*< Index in the per-thread TLS object list. */
#else /* !DEE_WITHOUT_THREADS */
 DEE_A_REF DeeObject *tls_value;  /*< [0..1] Faked TLS value (NULL -> not initialized). */
#endif /* DEE_WITHOUT_THREADS */
};
#define DeeTls_FACTORY(self) ((DeeTlsObject *)Dee_REQUIRES_POINTER(self))->tls_factory
#ifndef DEE_WITHOUT_THREADS
#define DeeTls_HANDLE(self)  ((DeeTlsObject *)Dee_REQUIRES_POINTER(self))->tls_handle
#else /* !DEE_WITHOUT_THREADS */
#define DeeTls_VALUE(self)   ((DeeTlsObject *)Dee_REQUIRES_POINTER(self))->tls_value
#endif /* DEE_WITHOUT_THREADS */
#else
#define DeeTls_FACTORY(self) DeeTls_Factory((DeeObject *)Dee_REQUIRES_POINTER(self))
#endif

#define DeeTls_Check(ob)      DeeObject_InstanceOf(ob,&DeeTls_Type)
#define DeeTls_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeTls_Type)
DEE_DATA_DECL(DeeTypeObject) DeeTls_Type;


#define DeeTls_New() DeeTls_NewWithFactory(Dee_None)
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTlsObject) *) DeeTls_NewWithFactory(DEE_A_INOUT DeeObject *factory);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DeeObject *) DeeTls_Factory(DEE_A_IN_OBJECT(DeeTlsObject) const *self);

//////////////////////////////////////////////////////////////////////////
// Resets the value of 'self' in the current thread
// Afterwards, the TLS has the same state as when the thread originally launched
// NOTE: This function doesn't raise an error
// >> Returns  0: on success
// >> Returns  1: nothing to delete
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeTls_Delete(DEE_A_INOUT_OBJECT(DeeTlsObject) *self);

//////////////////////////////////////////////////////////////////////////
// Returns this thread's value for 'self'
// If missing, it will be initialized with the stored factory
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_REF DeeObject *) DeeTls_Get(DEE_A_INOUT_OBJECT(DeeTlsObject) *self);
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT_REF DeeObject *) _DeeTls_LazyGet(DEE_A_IN_OBJECT(DeeTlsObject) const *self);
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeTls_Initialized(DEE_A_IN_OBJECT(DeeTlsObject) const *self);

//////////////////////////////////////////////////////////////////////////
// Sets this thread's value for 'self'
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeTls_Set(
 DEE_A_INOUT_OBJECT(DeeTlsObject) *self, DEE_A_INOUT DeeObject *v);

//////////////////////////////////////////////////////////////////////////
// Exchange this thread's value for 'self' with 'v'
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_REF DeeObject *) DeeTls_Exchange(
 DEE_A_INOUT_OBJECT(DeeTlsObject) *self, DEE_A_INOUT DeeObject *v);

//////////////////////////////////////////////////////////////////////////
// Returns the amount of assigned TLS handles
// NOTE: Real value may have already changed by the time this function returns!
//       >> For profiling usage only
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_size_t) DeeTls_InstanceCount(void);

DEE_DECL_END

#endif /* !GUARD_DEEMON_MP_TLS_H */
