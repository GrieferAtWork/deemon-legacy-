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
#ifndef GUARD_DEEMON_OPTIONAL_OBJECT_POOL_H
#define GUARD_DEEMON_OPTIONAL_OBJECT_POOL_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/atomic_mutex.h>
#include <deemon/optional/object_malloc.h>

DEE_DECL_BEGIN

union _DeeObjectPool { union _DeeObjectPool *_op_next; };

#define _DEE_OBJECTPOOL_SIZE_NAME(pool_name)  _deepoolimpl_size_##pool_name
#define _DEE_OBJECTPOOL_LOCK_NAME(pool_name)  _deepoolimpl_lock_##pool_name
#define _DEE_OBJECTPOOL_POOL_NAME(pool_name)  _deepoolimpl_pool_##pool_name
#define _DEE_OBJECTPOOL_TYPE_NAME(pool_name)  _deepoolimpl_type_##pool_name
#define _DEE_OBJECTPOOL_ALLOC_NAME(pool_name) _deepoolimpl_alloc_##pool_name
#define _DEE_OBJECTPOOL_FREE_NAME(pool_name)  _deepoolimpl_free_##pool_name
#define _DEE_OBJECTPOOL_CLEAR_NAME(pool_name) _deepoolimpl_clear_##pool_name

#ifdef DEE_DEBUG
#define DEE_PRIVATE_OBJECT_POOL_FREE_DEBUG_SET_NULL(x) (x)=NULL
#else
#define DEE_PRIVATE_OBJECT_POOL_FREE_DEBUG_SET_NULL(x) (void)0
#endif

#define DEE_OBJECTPOOL_DEFINE_STATS(pool_name)\
DEE_STATIC_INLINE(DEE_ATTRIBUTE_CONST DEE_A_RET_WUNUSED Dee_size_t) \
_DEE_OBJECTPOOL_SIZE_NAME(pool_name)(void) {\
 Dee_size_t _result = 0; union _DeeObjectPool *_iter;\
 DeeAtomicMutex_Acquire(&_DEE_OBJECTPOOL_LOCK_NAME(pool_name));\
 _iter = _DEE_OBJECTPOOL_POOL_NAME(pool_name);\
 while (_iter) ++_result,_iter = _iter->_op_next;\
 DeeAtomicMutex_Release(&_DEE_OBJECTPOOL_LOCK_NAME(pool_name));\
 return _result;\
}
#define DEE_OBJECTPOOL_DEFINE_TYPE(pool_name,T)\
 typedef T _DEE_OBJECTPOOL_TYPE_NAME(pool_name);
#ifdef __INTELLISENSE__
#define DEE_OBJECTPOOL_DEFINE_GLOBALS(pool_name) enum{pool_name};\
 static struct DeeAtomicMutex _DEE_OBJECTPOOL_LOCK_NAME(pool_name) = DeeAtomicMutex_INIT();\
 static union _DeeObjectPool *_DEE_OBJECTPOOL_POOL_NAME(pool_name) = NULL;
#else
#define DEE_OBJECTPOOL_DEFINE_GLOBALS(pool_name)\
 static struct DeeAtomicMutex _DEE_OBJECTPOOL_LOCK_NAME(pool_name) = DeeAtomicMutex_INIT();\
 static union _DeeObjectPool *_DEE_OBJECTPOOL_POOL_NAME(pool_name) = NULL;
#endif
#define DEE_OBJECTPOOL_DEFINE_ALLOC(pool_name)\
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(NULL) _DEE_OBJECTPOOL_TYPE_NAME(pool_name) *) \
_DEE_OBJECTPOOL_ALLOC_NAME(pool_name)(void) {\
 _DEE_OBJECTPOOL_TYPE_NAME(pool_name) *_result;\
 DeeAtomicMutex_Acquire(&_DEE_OBJECTPOOL_LOCK_NAME(pool_name));\
 if (_DEE_OBJECTPOOL_POOL_NAME(pool_name)) {\
  _result = (_DEE_OBJECTPOOL_TYPE_NAME(pool_name) *)_DEE_OBJECTPOOL_POOL_NAME(pool_name);\
  _DEE_OBJECTPOOL_POOL_NAME(pool_name) = _DEE_OBJECTPOOL_POOL_NAME(pool_name)->_op_next;\
  DeeAtomicMutex_Release(&_DEE_OBJECTPOOL_LOCK_NAME(pool_name));\
 } else {\
  DeeAtomicMutex_Release(&_DEE_OBJECTPOOL_LOCK_NAME(pool_name));\
  _result = DeeObject_MALLOCF(_DEE_OBJECTPOOL_TYPE_NAME(pool_name),"pool object: " #pool_name);\
 }\
 return _result;\
}
#define DEE_OBJECTPOOL_DEFINE_FREE(pool_name)\
DEE_STATIC_INLINE(void) _DEE_OBJECTPOOL_FREE_NAME(pool_name)(\
 DEE_A_IN DEE_A_OUT_INVALID _DEE_OBJECTPOOL_TYPE_NAME(pool_name) *ob) {\
 DEE_ASSERTF(ob,"Can't free NULL object");\
 DEE_PRIVATE_OBJECT_POOL_FREE_DEBUG_SET_NULL(((struct DeeObject *)ob)->__ob_type);\
 DeeAtomicMutex_Acquire(&_DEE_OBJECTPOOL_LOCK_NAME(pool_name));\
 ((union _DeeObjectPool *)ob)->_op_next = _DEE_OBJECTPOOL_POOL_NAME(pool_name);\
 _DEE_OBJECTPOOL_POOL_NAME(pool_name) = (union _DeeObjectPool *)ob;\
 DeeAtomicMutex_Release(&_DEE_OBJECTPOOL_LOCK_NAME(pool_name));\
}
#define DEE_OBJECTPOOL_DEFINE_CLEAR(pool_name)\
DEE_STATIC_INLINE(Dee_size_t) _DEE_OBJECTPOOL_CLEAR_NAME(pool_name)(void) {\
 Dee_size_t _result = 0;\
 union _DeeObjectPool *_iter,*_op_next;\
 DeeAtomicMutex_Acquire(&_DEE_OBJECTPOOL_LOCK_NAME(pool_name));\
 _iter = _DEE_OBJECTPOOL_POOL_NAME(pool_name);\
 _DEE_OBJECTPOOL_POOL_NAME(pool_name) = NULL;\
 DeeAtomicMutex_Release(&_DEE_OBJECTPOOL_LOCK_NAME(pool_name));\
 if (_iter) do {\
  _op_next = _iter->_op_next;\
  DeeObject_Free(_iter);\
  ++_result;\
 } while ((_iter = _op_next) != NULL);\
 return _result;\
}

#define DEE_OBJECTPOOL_DEFINE_CORE(pool_name)\
 DEE_OBJECTPOOL_DEFINE_GLOBALS(pool_name)\
 DEE_OBJECTPOOL_DEFINE_ALLOC(pool_name)\
 DEE_OBJECTPOOL_DEFINE_FREE(pool_name)\
 DEE_OBJECTPOOL_DEFINE_CLEAR(pool_name)
#define DEE_OBJECTPOOL_DEFINE(pool_name,T)\
 DEE_OBJECTPOOL_DEFINE_TYPE(pool_name,T)\
 DEE_OBJECTPOOL_DEFINE_CORE(pool_name)


#define /* Dee_size_t  */DEE_OBJECTPOOL_STAT_SIZE(pool_name) _DEE_OBJECTPOOL_SIZE_NAME(pool_name)()
#define /* Dee_size_t  */DEE_OBJECTPOOL_CLEAR(pool_name)     _DEE_OBJECTPOOL_CLEAR_NAME(pool_name)()
#define /* DeeObject * */DEE_OBJECTPOOL_ALLOC(pool_name)     _DEE_OBJECTPOOL_ALLOC_NAME(pool_name)()
#define /* void        */DEE_OBJECTPOOL_FREE(pool_name,ob)   _DEE_OBJECTPOOL_FREE_NAME(pool_name)((_DEE_OBJECTPOOL_TYPE_NAME(pool_name) *)Dee_REQUIRES_POINTER(ob))



DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_OBJECT_POOL_H */
