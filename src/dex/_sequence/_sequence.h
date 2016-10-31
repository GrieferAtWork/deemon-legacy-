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
#ifndef GUARD_DEEMON_DEX_SEQUENCE_H
#define GUARD_DEEMON_DEX_SEQUENCE_H 1
 
#include <deemon/__conf.inl>
#include <deemon/dex.h>
#include <deemon/object.h>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/atomic_mutex.h>
#include DEE_INCLUDE_MEMORY_API()

//////////////////////////////////////////////////////////////////////////
// A dex providing additional sequence types, such as
//  - single_list // Singly-linked list (forwards-pointing nodes)
//  - double_list // Double-linked list
//  - deque
// WARNING: Currently, not all list types are (fully) implemented

DEE_DECL_BEGIN

DEE_OBJECT_DEF(DeeSingleListObject);
DEE_OBJECT_DEF(DeeSingleListIteratorObject);


struct DeeSingleList;
struct DeeSingleListNode;
struct DeeSingleListIterator;

struct DeeSingleListNode {
 /*atomic*/unsigned int    sln_refcnt; /*< Node reference counter. */
 struct DeeSingleListNode *sln_next;   /*< [0..1][lock(:sl_lock)][owned] Next node. */
 DEE_A_REF DeeObject      *sln_elem;   /*< [1..1][const] Node element. */
};
extern DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct DeeSingleListNode *
 _DeeSingleListNode_NewUnsafe(DEE_A_INOUT DeeObject *elem);
extern DEE_A_RET_NOEXCEPT(NULL) struct DeeSingleListNode *
 _DeeSingleListNode_WeakNewUnsafe(DEE_A_INOUT DeeObject *elem);
#define _DeeSingleListNode_FreeUnsafe(ob)\
do{ Dee_DECREF((ob)->sln_elem); free_nn(ob); }while(0)

#define DeeSingleListNode_INCREF(ob)\
do{ DeeAtomicInt_IncFetch((ob)->sln_refcnt,memory_order_seq_cst); }while(0)
#define DeeSingleListNode_DECREF(ob)\
do{\
 if (!DeeAtomicInt_DecFetch((ob)->sln_refcnt,memory_order_seq_cst)) {\
  /*DEE_ASSERTF((ob)->sln_next == NULL,"Dead node has successor");*/\
  Dee_DECREF((ob)->sln_elem);\
  free_nn(ob);\
 }\
}while(0)

struct DeeSingleList {
 struct DeeSingleListNode *sl_front; /*< [0..1][owned(*->sln_next,ref(sln_refcnt))] Front node. */
 struct DeeSingleListNode *sl_back;  /*< [0..1] Back node. */
 Dee_size_t                sl_size;  /*< Size of the list. */
 struct DeeAtomicMutex     sl_lock;  /*< List lock (NOTE: Also used to lock the nodes). */
};

#define DeeSingleList_INIT() {NULL,NULL,DeeAtomicMutex_INIT()}
#define _DeeSingleList_Init(ob)\
do{\
 (ob)->sl_front = NULL;\
 (ob)->sl_back = NULL;\
 DeeAtomicMutex_Init(&(ob)->sl_lock);\
}while(0)
#define _DeeSingleList_Quit(ob)\
do{\
 struct DeeSingleListNode *_sl_iter,*_sl_next;\
 _sl_iter = (ob)->sl_front;\
 while (_sl_iter) {\
  _sl_next = _sl_iter->sln_next;\
  DEE_ASSERTF(_sl_iter->sln_refcnt == 1,\
              "SingleList node has invalid refcnt during destruction (%u != 1)",\
              _sl_iter->sln_refcnt);\
  Dee_DECREF(_sl_iter->sln_elem);\
  free_nn(_sl_iter);\
  _sl_iter = _sl_next;\
 }\
}while(0)
#define _DeeSingleList_Clear(ob)\
do{\
 struct DeeSingleListNode *_sl_iter,*_sl_next;\
 DeeAtomicMutex_AcquireRelaxed(&(ob)->sl_lock);\
 _sl_iter = (ob)->sl_front;\
 (ob)->sl_front = NULL;\
 (ob)->sl_size = 0;\
 DeeAtomicMutex_Release(&(ob)->sl_lock);\
 while (_sl_iter) {\
  _sl_next = _sl_iter->sln_next;\
  DeeSingleListNode_DECREF(_sl_iter);\
  _sl_iter = _sl_next;\
 }\
}while(0)

extern DEE_A_RET_EXCEPT(-1) int DeeSingleList_InitCopy(DEE_A_OUT struct DeeSingleList *self, DEE_A_IN struct DeeSingleList const *right) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeSingleList_AssignCopy(DEE_A_INOUT struct DeeSingleList *self, DEE_A_IN struct DeeSingleList const *right) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeSingleList_InitFromIterator(DEE_A_OUT struct DeeSingleList *self, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeSingleList_AssignAllFromIterator(DEE_A_INOUT struct DeeSingleList *self, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,2));
extern void DeeSingleList_InitMove(DEE_A_OUT struct DeeSingleList *self, DEE_A_IN struct DeeSingleList *right) DEE_ATTRIBUTE_NONNULL((1,2));
extern void DeeSingleList_AssignMove(DEE_A_INOUT struct DeeSingleList *self, DEE_A_IN struct DeeSingleList *right) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(-1) int DeeSingleList_AppendAllFromIterator(
 DEE_A_INOUT struct DeeSingleList *self, DEE_A_INOUT DeeObject *iterator) DEE_ATTRIBUTE_NONNULL((1,2));


extern DEE_A_RET_NOEXCEPT(NULL) /*ref(sln_refcnt)*/struct DeeSingleListNode *DeeSingleList_Front(DEE_A_IN struct DeeSingleList const *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_NOEXCEPT(NULL) /*ref(sln_refcnt)*/struct DeeSingleListNode *DeeSingleList_Back(DEE_A_IN struct DeeSingleList const *self) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(NULL) /*ref(sln_refcnt)*/struct DeeSingleListNode *DeeSingleList_NodeGet(DEE_A_IN struct DeeSingleList const *self, DEE_A_IN Dee_ssize_t idx) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(NULL) /*ref(sln_refcnt)*/struct DeeSingleListNode *DeeSingleList_NodeAt(DEE_A_IN struct DeeSingleList const *self, DEE_A_IN Dee_size_t idx) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(NULL) /*ref(sln_refcnt)*/struct DeeSingleListNode *DeeSingleList_PushFront(DEE_A_INOUT struct DeeSingleList *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(NULL) /*ref(sln_refcnt)*/struct DeeSingleListNode *DeeSingleList_PushBack(DEE_A_INOUT struct DeeSingleList *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2));

// NOTE: DeeSingleList_Insert: 'pos == NULL' referrs to the back of the list
// NOTE: DeeSingleList_InsertAfter: 'pos_before == NULL' referrs to the front of the list (implementing a counted push_front())
extern DEE_A_RET_EXCEPT(-1) int DeeSingleList_Insert(
 DEE_A_INOUT struct DeeSingleList *self, DEE_A_IN_OPT struct DeeSingleListNode const *pos,
 DEE_A_INOUT DeeObject *elem, DEE_A_OUT /*ref(sln_refcnt)*/struct DeeSingleListNode **result_pos_after) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
extern DEE_A_RET_EXCEPT(-1) int DeeSingleList_InsertAfter(
 DEE_A_INOUT struct DeeSingleList *self, DEE_A_IN_OPT struct DeeSingleListNode const *pos_before,
 DEE_A_INOUT DeeObject *elem, DEE_A_OUT /*ref(sln_refcnt)*/struct DeeSingleListNode **result_pos_after) DEE_ATTRIBUTE_NONNULL((1,2,3,4));

// NOTE: These functions store NULL in '*result_pos_after' if no node is following
// NOTE: Returns the amount of actually erased nodes
// NOTE: DeeSingleList_Erase: 'pos == NULL' referrs to the back of the list (results in a no-op)
// NOTE: DeeSingleList_EraseAfter: 'pos_before == NULL' referrs to the front of the list (implementing a counted pop_front())
extern DEE_A_RET_EXCEPT(-1) Dee_size_t DeeSingleList_Erase(
 DEE_A_INOUT struct DeeSingleList *self, DEE_A_IN_OPT struct DeeSingleListNode const *pos,
 DEE_A_IN Dee_size_t n, DEE_A_OUT /*ref(sln_refcnt)*/struct DeeSingleListNode **result_pos_after) DEE_ATTRIBUTE_NONNULL((1,2,4));
extern DEE_A_RET_EXCEPT(-1) Dee_size_t DeeSingleList_EraseAfter(
 DEE_A_INOUT struct DeeSingleList *self, DEE_A_IN struct DeeSingleListNode const *pos_before,
 DEE_A_IN Dee_size_t n, DEE_A_OUT /*ref(sln_refcnt)*/struct DeeSingleListNode **result_pos_after) DEE_ATTRIBUTE_NONNULL((1,2,4));
extern DEE_A_RET_EXCEPT(NULL) /*ref(sln_refcnt)*/struct DeeSingleListNode *DeeSingleList_PopFront(DEE_A_INOUT struct DeeSingleList *self, DEE_A_OUT /*ref(sln_refcnt)*/struct DeeSingleListNode **result_pos_after) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(NULL) /*ref(sln_refcnt)*/struct DeeSingleListNode *DeeSingleList_PopBack(DEE_A_INOUT struct DeeSingleList *self) DEE_ATTRIBUTE_NONNULL((1));


#define DeeSingleList_TRAVERSE_SAFE(iterator,slist) \
 for ((iterator)->sli_node = DeeSingleList_Front(slist);\
      (iterator)->sli_node; DeeSingleListIterator_Next(iterator,slist))

struct DeeSingleListIterator {
 struct DeeSingleListNode *sli_node; /*< [0..1][ref(sln_refcnt)] Current node. */
};

extern void DeeSingleListIterator_Next(
 DEE_A_INOUT struct DeeSingleListIterator *self,
 DEE_A_IN struct DeeSingleList const *list)
 DEE_ATTRIBUTE_NONNULL((1,2));

#define DeeSingleListIterator_Quit(ob)\
do{\
 if ((ob)->sli_node)\
  DeeSingleListNode_DECREF((ob)->sli_node);\
}while(0)






struct DeeSingleListObject {
 DEE_OBJECT_HEAD
 struct DeeSingleList sl_list; /*< Singly-linked list. */
};
struct DeeSingleListIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeSingleListObject *sli_list; /*< [0..1][lock(sli_lock)] Keep-alive reference to the list. */
 struct DeeSingleListIterator   sli_iter; /*< [lock(sli_lock)] Internal iterator. */
 struct DeeAtomicMutex          sli_lock; /*< Lock for this iterator. */
};

extern DEE_A_RET_EXCEPT_REF DeeSingleListIteratorObject *DeeSingleListIterator_New(
 DEE_A_INOUT DeeSingleListObject *list,
 DEE_A_INOUT_OPT struct DeeSingleListNode *node) DEE_ATTRIBUTE_NONNULL((1));


extern DeeTypeObject DeeSingleList_Type;
extern DeeTypeObject DeeSingleListIterator_Type;


extern DEE_A_RET_EXCEPT(-1) int DeeSingleListNode_PtrFromObject(
 DEE_A_IN struct DeeSingleList const *list, DEE_A_INOUT DeeObject *index,
 /*ref(sln_refcnt)*/DEE_A_OUT struct DeeSingleListNode **result)
 DEE_ATTRIBUTE_NONNULL((1,2,3));


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_SEQUENCE_H */
