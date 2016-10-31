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
#ifndef GUARD_DEEMON_DEX_SEQUENCE_C
#define GUARD_DEEMON_DEX_SEQUENCE_C 1
#define DEE_EXTENSION 1

//@ dex.name = _sequence

#include "_sequence.h"
#include <deemon/dex.h>
#include <deemon/bool.h>
#include <deemon/error.h>
#include <deemon/deemonrun.h>
#include <deemon/type.h>
#include <deemon/marshal.h>
#include <deemon/generic_vtable.h>
#include <deemon/gc.h>
#include <deemon/string.h>
#include <deemon/memberdef.h>
#include <deemon/optional/uuid.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_visit.h>
#include <deemon/optional/type_util.h>
#include <deemon/optional/type_decl.h>
#include <deemon/optional/type_slots.h>

#ifndef __INTELLISENSE__
#include "deque.c.inl"
#else
#include "deque.h.inl"
#endif

DEE_DECL_BEGIN

void _sequence_emptyerror(void) {
 DeeError_SET_STRING(&DeeErrorType_ValueError,"Empty sequence");
}
void _sequence_indexerror(DEE_A_IN Dee_size_t size, DEE_A_IN Dee_size_t index) {
 DeeError_SetStringf(&DeeErrorType_IndexError,
                     "Index %Iu is greater than or equal to %Iu",
                     index,size);
}



DEE_A_RET_EXCEPT(-1) int DeeSingleList_InitCopy(
 DEE_A_OUT struct DeeSingleList *self,
 DEE_A_IN struct DeeSingleList const *right) {
 struct DeeSingleListNode *new_nodes,*rhs_nodes,*next;
 Dee_size_t i,node_count;
again:
 DEE_ASSERT(self); DEE_ASSERT(right);
 DEE_ASSERT(self != right);
 DeeAtomicMutex_AcquireRelaxed(&right->sl_lock);
 if ((node_count = right->sl_size) != 0) {
  rhs_nodes = right->sl_front;
  DEE_ASSERT(rhs_nodes);
  if DEE_UNLIKELY((new_nodes = _DeeSingleListNode_WeakNewUnsafe(rhs_nodes->sln_elem)) == NULL) {
   DeeAtomicMutex_Release(&right->sl_lock);
   if DEE_LIKELY(Dee_CollectMemory()) goto again;
err_nomem:
   DeeError_NoMemory();
   return -1;
  }
  self->sl_front = new_nodes;
  for (i = 0; i < node_count; ++i) {
   rhs_nodes = rhs_nodes->sln_next;
   if DEE_UNLIKELY((new_nodes->sln_next = _DeeSingleListNode_WeakNewUnsafe(rhs_nodes->sln_elem)) == NULL) {
    DeeAtomicMutex_Release(&right->sl_lock);
    if DEE_LIKELY(Dee_CollectMemory()) goto again;
    // Free all previously allocated nodes
    rhs_nodes = self->sl_front;
    while (rhs_nodes != new_nodes) {
     next = rhs_nodes->sln_next;
     _DeeSingleListNode_FreeUnsafe(rhs_nodes);
     rhs_nodes = next;
    }
    goto err_nomem;
   }
   new_nodes = new_nodes->sln_next;
  }
  DeeAtomicMutex_Release(&right->sl_lock);
  new_nodes->sln_next = NULL;
  self->sl_back = new_nodes;
  self->sl_size = node_count;
 } else {
  DeeAtomicMutex_Release(&right->sl_lock);
  self->sl_front = NULL;
  self->sl_back = NULL;
  self->sl_size = 0;
 }
 DeeAtomicMutex_Init(&self->sl_lock);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeSingleList_AssignCopy(
 DEE_A_INOUT struct DeeSingleList *self,
 DEE_A_IN struct DeeSingleList const *right) {
 struct DeeSingleListNode *new_front,*new_back;
 struct DeeSingleListNode *old_nodes,*rhs_nodes,*next;
 Dee_size_t i,node_count;
again:
 DEE_ASSERT(self); DEE_ASSERT(right);
 DEE_ASSERT(self != right);
 DeeAtomicMutex_AcquireRelaxed(&right->sl_lock);
 if ((node_count = right->sl_size) != 0) {
  rhs_nodes = right->sl_front;
  DEE_ASSERT(rhs_nodes);
  if DEE_UNLIKELY((new_back = _DeeSingleListNode_WeakNewUnsafe(rhs_nodes->sln_elem)) == NULL) {
   DeeAtomicMutex_Release(&right->sl_lock);
   if DEE_LIKELY(Dee_CollectMemory()) goto again;
err_nomem:
   DeeError_NoMemory();
   return -1;
  }
  new_front = new_back;
  for (i = 0; i < node_count; ++i) {
   rhs_nodes = rhs_nodes->sln_next;
   if DEE_UNLIKELY((new_back->sln_next = _DeeSingleListNode_WeakNewUnsafe(rhs_nodes->sln_elem)) == NULL) {
    DeeAtomicMutex_Release(&right->sl_lock);
    if DEE_LIKELY(Dee_CollectMemory()) goto again;
    // Free all previously allocated nodes
    rhs_nodes = new_front;
    while (rhs_nodes != new_back) {
     next = rhs_nodes->sln_next;
     _DeeSingleListNode_FreeUnsafe(rhs_nodes);
     rhs_nodes = next;
    }
    goto err_nomem;
   }
   new_back = new_back->sln_next;
  }
  DeeAtomicMutex_Release(&right->sl_lock);
  new_back->sln_next = NULL;
 } else {
  DeeAtomicMutex_Release(&right->sl_lock);
  new_front = NULL;
  new_back = NULL;
 }
 DeeAtomicMutex_AcquireRelaxed(&self->sl_lock);
 old_nodes = self->sl_front;
 self->sl_front = new_front;
 self->sl_back = new_back;
 self->sl_size = node_count;
 DeeAtomicMutex_Release(&self->sl_lock);
 while (old_nodes) {
  next = old_nodes->sln_next;
  DeeSingleListNode_DECREF(old_nodes);
  old_nodes = next;
 }
 return 0;
}

void DeeSingleList_InitMove(
 DEE_A_OUT struct DeeSingleList *self,
 DEE_A_IN struct DeeSingleList *right) {
 DEE_ASSERT(self); DEE_ASSERT(right);
 DEE_ASSERT(self != right);
 DeeAtomicMutex_AcquireRelaxed(&right->sl_lock);
 self->sl_front = right->sl_front; right->sl_front = NULL;
 self->sl_back  = right->sl_back;  right->sl_back  = NULL;
 self->sl_size  = right->sl_size;  right->sl_size  = 0;
 DeeAtomicMutex_Release(&right->sl_lock);
 DeeAtomicMutex_Init(&self->sl_lock);
}
void DeeSingleList_AssignMove(
 DEE_A_INOUT struct DeeSingleList *self,
 DEE_A_IN struct DeeSingleList *right) {
 struct DeeSingleListNode *iter,*next,*new_front,*new_back;
 Dee_size_t new_size;
 DEE_ASSERT(self); DEE_ASSERT(right);
 DEE_ASSERT(self != right);
 DeeAtomicMutex_AcquireRelaxed(&right->sl_lock);
 new_front = right->sl_front; right->sl_front = NULL;
 new_back  = right->sl_back;  right->sl_back  = NULL;
 new_size  = right->sl_size;  right->sl_size  = 0;
 DeeAtomicMutex_Release(&right->sl_lock);
 DeeAtomicMutex_AcquireRelaxed(&self->sl_lock);
 iter = self->sl_front;
 self->sl_front = new_front;
 self->sl_back = new_back;
 self->sl_size = new_size;
 DeeAtomicMutex_Release(&self->sl_lock);
 // Delete all old nodes
 while (iter) {
  next = iter->sln_next;
  DeeSingleListNode_DECREF(iter);
  iter = iter;
 }
}

DEE_A_RET_EXCEPT(-1) int DeeSingleList_InitFromIterator(
 DEE_A_OUT struct DeeSingleList *self, DEE_A_INOUT DeeObject *iterator) {
 DEE_ASSERT(self); _DeeSingleList_Init(self);
 return DeeSingleList_AppendAllFromIterator(self,iterator);
}
DEE_A_RET_EXCEPT(-1) int DeeSingleList_AssignAllFromIterator(
 DEE_A_INOUT struct DeeSingleList *self, DEE_A_INOUT DeeObject *iterator) {
 struct DeeSingleList new_list;
 if (DeeSingleList_InitFromIterator(&new_list,iterator) != 0) return -1;
 DeeSingleList_AssignMove(self,&new_list);
 // No need to call quit after move in this case, though it technically we'd need to
 //_DeeSingleList_Quit(&new_list);
 return 0;
}


DEE_A_RET_EXCEPT(-1) int DeeSingleList_AppendAllFromIterator(
 DEE_A_INOUT struct DeeSingleList *self, DEE_A_INOUT DeeObject *iterator) {
 DeeObject *elem; int error; struct DeeSingleListNode *pos;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(iterator)); pos = NULL;
 while ((error = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  pos = DeeSingleList_PushBack(self,elem);
  Dee_DECREF(elem);
  if (!pos) return -1;
  DeeSingleListNode_DECREF(pos);
 }
 if DEE_UNLIKELY(error < 0) { _DeeSingleList_Quit(self); return error; }
 return 0;
}





DEE_A_RET_EXCEPT(NULL) struct DeeSingleListNode *
_DeeSingleListNode_NewUnsafe(DEE_A_INOUT DeeObject *elem) {
 struct DeeSingleListNode *result;
 DEE_ASSERT(DeeObject_Check(elem));
 while DEE_UNLIKELY((result = (struct DeeSingleListNode *)
  malloc_nz(sizeof(struct DeeSingleListNode))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 result->sln_refcnt = 1;
 Dee_INCREF(result->sln_elem = elem);
 return result;
}
DEE_A_RET_NOEXCEPT(NULL) struct DeeSingleListNode *
_DeeSingleListNode_WeakNewUnsafe(DEE_A_INOUT DeeObject *elem) {
 struct DeeSingleListNode *result;
 DEE_ASSERT(DeeObject_Check(elem));
 if DEE_LIKELY((result = (struct DeeSingleListNode *)
  malloc_nz(sizeof(struct DeeSingleListNode))) != NULL) {
  result->sln_refcnt = 1;
  Dee_INCREF(result->sln_elem = elem);
 }
 return result;
}


DEE_A_RET_NOEXCEPT(NULL) /*ref(sln_refcnt)*/struct DeeSingleListNode *
DeeSingleList_Front(DEE_A_IN struct DeeSingleList const *self) {
 struct DeeSingleListNode *result;
 DEE_ASSERT(self);
 DeeAtomicMutex_AcquireRelaxed(&self->sl_lock);
 if ((result = self->sl_front) != NULL)
  DeeSingleListNode_INCREF(result);
 DeeAtomicMutex_Release(&self->sl_lock);
 return result;
}
DEE_A_RET_NOEXCEPT(NULL) /*ref(sln_refcnt)*/struct DeeSingleListNode *
DeeSingleList_Back(DEE_A_IN struct DeeSingleList const *self) {
 struct DeeSingleListNode *result;
 DEE_ASSERT(self);
 DeeAtomicMutex_AcquireRelaxed(&self->sl_lock);
 if ((result = self->sl_back) != NULL)
  DeeSingleListNode_INCREF(result);
 DeeAtomicMutex_Release(&self->sl_lock);
 return result;
}

DEE_A_RET_EXCEPT(NULL) struct DeeSingleListNode *DeeSingleList_NodeGet(
 DEE_A_IN struct DeeSingleList const *self, DEE_A_IN Dee_ssize_t idx) {
 struct DeeSingleListNode *result_node; Dee_size_t seq_size;
 DEE_ASSERT(self);
 DeeAtomicMutex_AcquireRelaxed(&self->sl_lock);
 if ((seq_size = self->sl_size) == 0) {
  DeeAtomicMutex_Release(&self->sl_lock);
  DeeError_SET_STRING(&DeeErrorType_ValueError,"Empty single_list");
  return NULL;
 }
 DEE_PRIVATE_CLAMP_INDEX(idx,seq_size);
 result_node = self->sl_front;
 DEE_ASSERT(result_node);
 while (idx--) {
  DEE_ASSERT(result_node->sln_next);
  result_node = result_node->sln_next;
 }
 DeeSingleListNode_INCREF(result_node);
 DeeAtomicMutex_Release(&self->sl_lock);
 return result_node; // Inherit reference
}

DEE_A_RET_EXCEPT(NULL) struct DeeSingleListNode *DeeSingleList_NodeAt(
 DEE_A_IN struct DeeSingleList const *self, DEE_A_IN Dee_size_t idx) {
 struct DeeSingleListNode *result_node; Dee_size_t seq_size;
 DEE_ASSERT(self);
 DeeAtomicMutex_AcquireRelaxed(&self->sl_lock);
 if (idx >= (seq_size = self->sl_size)) {
  DeeAtomicMutex_Release(&self->sl_lock);
  _sequence_indexerror(seq_size,idx);
  return NULL;
 }
 result_node = self->sl_front;
 DEE_ASSERT(result_node);
 while (idx--) {
  DEE_ASSERT(result_node->sln_next);
  result_node = result_node->sln_next;
 }
 DeeSingleListNode_INCREF(result_node);
 DeeAtomicMutex_Release(&self->sl_lock);
 return result_node; // Inherit reference
}

DEE_A_RET_EXCEPT(NULL) /*ref(sln_refcnt)*/struct DeeSingleListNode *DeeSingleList_PushFront(
 DEE_A_INOUT struct DeeSingleList *self, DEE_A_INOUT DeeObject *elem) {
 struct DeeSingleListNode *new_node;
 DEE_ASSERT(self); DEE_ASSERT(DeeObject_Check(elem));
 if DEE_UNLIKELY((new_node = _DeeSingleListNode_NewUnsafe(elem)) == NULL) return NULL;
 DeeAtomicMutex_AcquireRelaxed(&self->sl_lock);
 if ((new_node->sln_next = self->sl_front) == NULL)
  self->sl_back = new_node;
 self->sl_front = new_node;
 ++self->sl_size;
 DeeSingleListNode_INCREF(new_node);
 DeeAtomicMutex_Release(&self->sl_lock);
 return new_node;
}
DEE_A_RET_EXCEPT(NULL) /*ref(sln_refcnt)*/struct DeeSingleListNode *DeeSingleList_PushBack(
 DEE_A_INOUT struct DeeSingleList *self, DEE_A_INOUT DeeObject *elem) {
 struct DeeSingleListNode *new_node;
 DEE_ASSERT(self); DEE_ASSERT(DeeObject_Check(elem));
 if DEE_UNLIKELY((new_node = _DeeSingleListNode_NewUnsafe(elem)) == NULL) return NULL;
 new_node->sln_next = NULL;
 DeeAtomicMutex_AcquireRelaxed(&self->sl_lock);
 if (self->sl_back) {
  self->sl_back->sln_next = new_node;
 } else {
  DEE_ASSERT(!self->sl_front);
  self->sl_front = new_node;
 }
 self->sl_back = new_node;
 ++self->sl_size;
 DeeSingleListNode_INCREF(new_node);
 DeeAtomicMutex_Release(&self->sl_lock);
 return new_node;
}

DEE_A_RET_EXCEPT(-1) int DeeSingleList_Insert(
 DEE_A_INOUT struct DeeSingleList *self, DEE_A_IN_OPT struct DeeSingleListNode const *pos,
 DEE_A_INOUT DeeObject *elem, DEE_A_OUT /*ref(sln_refcnt)*/struct DeeSingleListNode **result_pos_after) {
 struct DeeSingleListNode *new_node,*pos_before;
 DEE_ASSERT(result_pos_after);
 if (!pos) return DEE_LIKELY((*result_pos_after = DeeSingleList_PushBack(self,elem)) != NULL) ? 0 : -1;
 if DEE_UNLIKELY((new_node = _DeeSingleListNode_NewUnsafe(elem)) == NULL) return -1;
 DeeAtomicMutex_AcquireRelaxed(&self->sl_lock);
 pos_before = self->sl_front;
 if (pos == pos_before) {
  // Insert at the front
  new_node->sln_next = self->sl_front;
  self->sl_front = new_node;
 } else {
  if (!pos->sln_next && pos != self->sl_back) {
   // Deleted node
   DeeAtomicMutex_Release(&self->sl_lock);
   _DeeSingleListNode_FreeUnsafe(new_node);
   *result_pos_after = NULL;
   return 0;
  }
  while (1) {
   DEE_ASSERTF(pos_before,"Foreign node");
   if (pos_before->sln_next == pos) break;
   pos_before = pos_before->sln_next;
  }
  DEE_ASSERT(pos_before->sln_next == pos);
  new_node->sln_next = pos_before->sln_next;
  pos_before->sln_next = new_node;
 }
 ++self->sl_size;
 DeeSingleListNode_INCREF(new_node);
 DeeAtomicMutex_Release(&self->sl_lock);
 *result_pos_after = new_node; // Inherit reference
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeSingleList_InsertAfter(
 DEE_A_INOUT struct DeeSingleList *self, DEE_A_IN_OPT struct DeeSingleListNode const *pos_before,
 DEE_A_INOUT DeeObject *elem, DEE_A_OUT /*ref(sln_refcnt)*/struct DeeSingleListNode **result_pos_after) {
 struct DeeSingleListNode *new_node;
 DEE_ASSERT(result_pos_after);
 if (!pos_before) return DEE_LIKELY((*result_pos_after = DeeSingleList_PushFront(self,elem)) != NULL) ? 0 : -1;
 if DEE_UNLIKELY((new_node = _DeeSingleListNode_NewUnsafe(elem)) == NULL) return -1;
 DeeAtomicMutex_AcquireRelaxed(&self->sl_lock);
 if (!pos_before->sln_next) {
  // Insert at the end, or at deleted node (resulting in a no-op)
  if (pos_before == self->sl_back) {
   // Iterator at the end
   self->sl_back->sln_next = new_node;
   self->sl_back = new_node;
  } else {
   // Deleted / invalid node (NOTE The invalid case isn't allowed, but implicitly handled by this)
   DeeAtomicMutex_Release(&self->sl_lock);
   _DeeSingleListNode_FreeUnsafe(new_node);
   *result_pos_after = NULL;
   return 0;
  }
 } else {
  // Insert after 'pos_before'
#ifdef DEE_DEBUG
  { // Make sure the given 'pos_before' is really part of this list
   struct DeeSingleListNode *iter;
   iter = self->sl_front;
   while (iter && iter != pos_before) iter = iter->sln_next;
   DEE_ASSERTF(iter == pos_before,"Given node is foreign");
  }
#endif
  new_node->sln_next = pos_before->sln_next;
  ((struct DeeSingleListNode *)pos_before)->sln_next = new_node;
 }
 ++self->sl_size;
 DeeSingleListNode_INCREF(new_node);
 DeeAtomicMutex_Release(&self->sl_lock);
 *result_pos_after = new_node; // Inherit reference
 return 0;
}


DEE_A_RET_EXCEPT(-1) Dee_size_t DeeSingleList_Erase(
 DEE_A_INOUT struct DeeSingleList *self, DEE_A_IN_OPT struct DeeSingleListNode const *pos,
 DEE_A_IN Dee_size_t n, DEE_A_OUT /*ref(sln_refcnt)*/struct DeeSingleListNode **result_pos_after) {
 struct DeeSingleListNode *iter,*next;
 struct DeeSingleListNode **deleted_nodev,*pos_before;
 Dee_size_t i,delnode_c;
 DEE_ASSERT(self);
 if (!pos) return 0; // Nothing to delete here (can't delete from the front of the list)
again:
 DeeAtomicMutex_AcquireRelaxed(&self->sl_lock);
 // Find the predecessor of 'pos'
 pos_before = self->sl_front;
 if (pos == pos_before) {
  // Erase from the front
  pos_before = NULL;
 } else while (1) {
  // Make sure the given node wasn't already deleted
  if (!pos_before) {
   // Node already deleted, or no succeeding elements --> noop
   DeeAtomicMutex_Release(&self->sl_lock);
   *result_pos_after = NULL;
   return 0;
  }
  if (pos_before->sln_next == pos) break;
  pos_before = pos_before->sln_next;
 }
 DEE_ASSERT(!pos_before || pos_before->sln_next == pos);
 DEE_ASSERT(!pos_before || pos_before->sln_next != self->sl_front);
 if (!n) {
  // Nothing to delete...
  *result_pos_after = (struct DeeSingleListNode *)pos;
  DeeSingleListNode_INCREF(pos);
  DeeAtomicMutex_Release(&self->sl_lock);
  return 0;
 }
 // Count how many nodes should really be deleted
 iter = (struct DeeSingleListNode *)pos,i = n,delnode_c = 0;
 do ++delnode_c; while (--i && (iter = iter->sln_next) != NULL);
 if ((deleted_nodev = (struct DeeSingleListNode **)malloc_nz(
  delnode_c*sizeof(struct DeeSingleListNode *))) == NULL) {
  DeeAtomicMutex_Release(&self->sl_lock);
  if (Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return (Dee_size_t)-1;
 }
 // Unlink and collect the nodes to-be deleted
 iter = (struct DeeSingleListNode *)pos; i = delnode_c;
 for (i = 0; i < delnode_c; ++i) {
  DEE_ASSERT(iter);
  deleted_nodev[i] = iter;
  next = iter->sln_next;
  iter->sln_next = NULL;
  iter = next;
 }
 // Update the list front if we were erasing from the front
 if (!pos_before) {
  DEE_ASSERT(pos == self->sl_front);
  self->sl_front = iter;
 }
 // Store the following node in '*result_pos_after'
 if ((*result_pos_after = iter) != NULL) {
  DeeSingleListNode_INCREF(iter);
 } else {
  // Deleted up to the very end
  // >> Set the new end to 'pos_before'
  self->sl_back = (struct DeeSingleListNode *)pos_before;
 }
 // Update the list's size
 self->sl_size -= delnode_c;
 DeeAtomicMutex_Release(&self->sl_lock);
 // Decref all nodes that were deleted
 // NOTE: We had to wait with this until we left the lock
 //       of the list to prevent a deadlock from occurring.
 for (i = 0; i < delnode_c; ++i)
  DeeSingleListNode_DECREF(deleted_nodev[i]);
 return delnode_c;
}

DEE_A_RET_EXCEPT(-1) Dee_size_t DeeSingleList_EraseAfter(
 DEE_A_INOUT struct DeeSingleList *self, DEE_A_IN_OPT struct DeeSingleListNode const *pos_before,
 DEE_A_IN Dee_size_t n, DEE_A_OUT /*ref(sln_refcnt)*/struct DeeSingleListNode **result_pos_after) {
 struct DeeSingleListNode *iter,*next,*first_delete;
 struct DeeSingleListNode **deleted_nodev;
 Dee_size_t i,delnode_c;
 DEE_ASSERT(self);
again:
 DeeAtomicMutex_AcquireRelaxed(&self->sl_lock);
 // Make sure the given node wasn't already deleted
 if (pos_before && !pos_before->sln_next) {
  // Node already deleted, or no succeeding elements --> noop
  DeeAtomicMutex_Release(&self->sl_lock);
  *result_pos_after = NULL;
  return 0;
 }
 first_delete = pos_before ? pos_before->sln_next : self->sl_front;
 DEE_ASSERT(!pos_before || pos_before->sln_next != self->sl_front);
 if (!n) {
  // Nothing to delete...
  *result_pos_after = first_delete;
  DeeSingleListNode_INCREF(first_delete);
  DeeAtomicMutex_Release(&self->sl_lock);
  return 0;
 }
 // Count how many nodes should really be deleted
 iter = first_delete,i = n,delnode_c = 0;
 do ++delnode_c; while (--i && (iter = iter->sln_next) != NULL);
 if ((deleted_nodev = (struct DeeSingleListNode **)malloc_nz(
  delnode_c*sizeof(struct DeeSingleListNode *))) == NULL) {
  DeeAtomicMutex_Release(&self->sl_lock);
  if (Dee_CollectMemory()) goto again;
  DeeError_NoMemory();
  return (Dee_size_t)-1;
 }
 // Unlink and collect the nodes to-be deleted
 iter = first_delete; i = delnode_c;
 for (i = 0; i < delnode_c; ++i) {
  DEE_ASSERT(iter);
  deleted_nodev[i] = iter;
  next = iter->sln_next;
  iter->sln_next = NULL;
  iter = next;
 }
 // Update the list front if we were erasing from the front
 if (!pos_before) {
  DEE_ASSERT(first_delete == self->sl_front);
  self->sl_front = iter;
 }
 // Store the following node in '*result_pos_after'
 if ((*result_pos_after = iter) != NULL) {
  DeeSingleListNode_INCREF(iter);
 } else {
  // Deleted up to the very end
  // >> Set the new end to 'pos_before'
  self->sl_back = (struct DeeSingleListNode *)pos_before;
 }
 // Update the list's size
 self->sl_size -= delnode_c;
 DeeAtomicMutex_Release(&self->sl_lock);
 // Decref all nodes that were deleted
 // NOTE: We had to wait with this until we left the lock
 //       of the list to prevent a deadlock from occurring.
 for (i = 0; i < delnode_c; ++i)
  DeeSingleListNode_DECREF(deleted_nodev[i]);
 return delnode_c;
}


DEE_A_RET_EXCEPT(NULL) /*ref(sln_refcnt)*/struct DeeSingleListNode *DeeSingleList_PopFront(
 DEE_A_INOUT struct DeeSingleList *self,
 DEE_A_OUT /*ref(sln_refcnt)*/struct DeeSingleListNode **result_pos_after) {
 struct DeeSingleListNode *result;
 DEE_ASSERT(self);
 DeeAtomicMutex_AcquireRelaxed(&self->sl_lock);
 if ((result = self->sl_front) == NULL) {
  DeeAtomicMutex_Release(&self->sl_lock);
  _sequence_emptyerror(); // Empty list
  return NULL;
 }
 --self->sl_size;
 // NOTE: Implicitly transfer reference from 'self->sl_front' --> 'result'
 if ((self->sl_front = result->sln_next) == NULL) {
  DEE_ASSERT(!self->sl_size);
  DEE_ASSERT(self->sl_back == result);
  self->sl_back = NULL;
 }
 DEE_ASSERT((self->sl_size == 0) == (self->sl_front == NULL) == (self->sl_back == NULL));
 if ((*result_pos_after = self->sl_front) != NULL)
  DeeSingleListNode_INCREF(*result_pos_after);
 DeeAtomicMutex_Release(&self->sl_lock);
 return result;
}

DEE_A_RET_EXCEPT(NULL) /*ref(sln_refcnt)*/struct DeeSingleListNode *
DeeSingleList_PopBack(DEE_A_INOUT struct DeeSingleList *self) {
 struct DeeSingleListNode *result,*before_result;
 DEE_ASSERT(self);
 DeeAtomicMutex_AcquireRelaxed(&self->sl_lock);
 if ((result = self->sl_back) == NULL) {
  DeeAtomicMutex_Release(&self->sl_lock);
  _sequence_emptyerror(); // Empty list
  return NULL;
 }
 DEE_ASSERTF(result->sln_next == NULL,"Last node has successor");
 before_result = self->sl_front;
 if (before_result == result) {
  DEE_ASSERT(self->sl_size == 1);
  // Only one node --> remove it
  // NOTE: Implicitly transfer reference from 'self->sl_front' --> 'result'
  self->sl_front = NULL;
  self->sl_back = NULL;
  self->sl_size = 0;
 } else {
  // Walk up to the node before the last one
  while (before_result->sln_next != result) {
   DEE_ASSERT(before_result->sln_next);
   before_result = before_result->sln_next;
  }
  // NOTE: Implicitly transfer reference from 'before_result->sln_next' --> 'result'
  before_result->sln_next = NULL;
  self->sl_back = before_result;
  --self->sl_size;
 }
 DeeAtomicMutex_Release(&self->sl_lock);
 return result;
}


void DeeSingleListIterator_Next(
 DEE_A_INOUT struct DeeSingleListIterator *self,
 DEE_A_IN struct DeeSingleList const *list) {
 struct DeeSingleListNode *next_node,*old_node;
 DEE_ASSERT(self); DEE_ASSERT(list);
 DEE_ASSERT(self->sli_node);
 DeeAtomicMutex_AcquireRelaxed(&list->sl_lock);
 next_node = (old_node = self->sli_node)->sln_next;
 if (next_node) DeeSingleListNode_INCREF(next_node);
 self->sli_node = next_node;
 DeeAtomicMutex_Release(&list->sl_lock);
 DeeSingleListNode_DECREF(old_node);
}



DEE_A_RET_EXCEPT_REF DeeSingleListIteratorObject *DeeSingleListIterator_New(
 DEE_A_INOUT DeeSingleListObject *list, DEE_A_INOUT_OPT struct DeeSingleListNode *node) {
 DeeSingleListIteratorObject *result;
 DEE_ASSERT(DeeObject_Check(list) && DeeObject_InstanceOf(list,&DeeSingleList_Type));
 if DEE_LIKELY((result = DeeGC_MALLOC(DeeSingleListIteratorObject)) != NULL) {
  DeeObject_INIT(result,&DeeSingleListIterator_Type);
  if ((result->sli_iter.sli_node = node) != NULL) DeeSingleListNode_INCREF(node);
  Dee_INCREF(result->sli_list = list);
  DeeAtomicMutex_Init(&result->sli_lock);
  DeeGC_TrackedAdd((DeeObject *)result);
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeSingleListNode_PtrFromObject(
 DEE_A_IN struct DeeSingleList const *list, DEE_A_INOUT DeeObject *index,
 /*ref(sln_refcnt)*/DEE_A_OUT struct DeeSingleListNode **result) {
 Dee_ssize_t index_id; struct DeeSingleListNode *iter;
 if (DeeObject_InstanceOf(index,&DeeSingleListIterator_Type)) {
  if (&((DeeSingleListIteratorObject *)index)->sli_list->sl_list != list) {
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Iterator %k is from a different list",index);
   return -1;
  }
  DeeAtomicMutex_AcquireRelaxed(&((DeeSingleListIteratorObject *)index)->sli_lock);
  if ((*result = ((DeeSingleListIteratorObject *)index)->sli_iter.sli_node) != NULL)
   DeeSingleListNode_INCREF(*result);
  DeeAtomicMutex_Release(&((DeeSingleListIteratorObject *)index)->sli_lock);
  return 0;
 }
 if (DeeObject_Cast(Dee_ssize_t,index,&index_id) != 0) return -1;
 DeeAtomicMutex_AcquireRelaxed(&list->sl_lock);
 if (!list->sl_size) {
  DeeAtomicMutex_Release(&list->sl_lock);
  *result = NULL;
  return 0;
 }
 DEE_ASSERT(list->sl_front);
 DEE_ASSERT(list->sl_back);
 DEE_PRIVATE_CLAMP_INDEX(index_id,list->sl_size);
 iter = list->sl_front;
 while (index_id--) { DEE_ASSERT(iter); iter = iter->sln_next; }
 DEE_ASSERT(iter);
 DeeSingleListNode_INCREF(iter);
 DeeAtomicMutex_Release(&list->sl_lock);
 *result = iter;
 return 0;
}










//////////////////////////////////////////////////////////////////////////
// VTable: single_list
static int _deesinglelist_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSingleListObject *self) {
 _DeeSingleList_Init(&self->sl_list);
 return 0;
}
static void _deesinglelist_tp_dtor(DeeSingleListObject *self) {
 _DeeSingleList_Quit(&self->sl_list);
}
DEE_VISIT_PROC(_deesinglelist_tp_visit,DeeSingleListObject *self) {
 struct DeeSingleListNode *iter;
 DeeAtomicMutex_AcquireRelaxed(&self->sl_list.sl_lock);
 iter = self->sl_list.sl_front;
 while (iter) {
  Dee_VISIT(iter->sln_elem);
  iter = iter->sln_next;
 }
 DeeAtomicMutex_Release(&self->sl_list.sl_lock);
}
static void _deesinglelist_tp_clear(DeeSingleListObject *self) {
 _DeeSingleList_Clear(&self->sl_list);
}
static int _deesinglelist_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeSingleListObject *self, DeeSingleListObject *right) {
 return DeeSingleList_InitCopy(&self->sl_list,&right->sl_list);
}
static int _deesinglelist_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeSingleListObject *self, DeeSingleListObject *right) {
 DeeSingleList_InitMove(&self->sl_list,&right->sl_list);
 return 0;
}
static int _deesinglelist_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeSingleListObject *self, DeeObject *args) {
 DeeObject *seq,*iterator; int error;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:single_list",&seq) != 0) return -1;
 if DEE_UNLIKELY((iterator = DeeObject_IterSelf(seq)) == NULL) return -1;
 error = DeeSingleList_InitFromIterator(&self->sl_list,iterator);
 Dee_DECREF(iterator);
 return error;
}
static int _deesinglelist_tp_copy_assign(
 DeeSingleListObject *self, DeeSingleListObject *right) {
 return DEE_LIKELY(self != right) ? DeeSingleList_AssignCopy(&self->sl_list,&right->sl_list) : 0;
}
static int _deesinglelist_tp_move_assign(
 DeeSingleListObject *self, DeeSingleListObject *right) {
 if DEE_LIKELY(self != right) {
  DeeSingleList_AssignMove(&self->sl_list,&right->sl_list);
 }
 return 0;
}
static int _deesinglelist_tp_any_assign(
 DeeSingleListObject *self, DeeObject *seq) {
 DeeObject *iterator; int error;
 if DEE_UNLIKELY((iterator = DeeObject_IterSelf(seq)) == NULL) return -1;
 error = DeeSingleList_AssignAllFromIterator(&self->sl_list,iterator);
 Dee_DECREF(iterator);
 return error;
}
static DeeObject *_deesinglelist_tp_str(DeeSingleListObject *self) {
 DeeObject *result; struct DeeSingleListIterator iter; int error,first = 1;
 struct DeeStringWriter writer = DeeStringWriter_INIT();
 if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,"{ ") != 0) goto err;
 DeeSingleList_TRAVERSE_SAFE(&iter,&self->sl_list) {
  if (first) first = 0;
  else {
   if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,",") != 0) {
err_iter: DeeSingleListIterator_Quit(&iter); goto err;
   }
  }
  error = DeeStringWriter_WriteObjectStr(&writer,iter.sli_node->sln_elem);
  if DEE_UNLIKELY(error != 0) goto err_iter;
 }
 if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer," }") != 0) goto err;
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}
static DeeObject *_deesinglelist_tp_repr(DeeSingleListObject *self) {
 DeeObject *result; struct DeeSingleListIterator iter; int error,first = 1;
 struct DeeStringWriter writer = DeeStringWriter_INIT();
 if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,"single_list({ ") != 0) goto err;
 DeeSingleList_TRAVERSE_SAFE(&iter,&self->sl_list) {
  if (first) first = 0;
  else {
   if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,",") != 0) {
err_iter: DeeSingleListIterator_Quit(&iter); goto err;
   }
  }
  error = DeeStringWriter_WriteObjectRepr(&writer,iter.sli_node->sln_elem);
  if DEE_UNLIKELY(error != 0) goto err_iter;
 }
 if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer," })") != 0) goto err;
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
}

static int _deesinglelist_tp_bool(DeeSingleListObject *self) {
 return self->sl_list.sl_size != 0;
}
static DeeObject *_deesinglelist_tp_not(DeeSingleListObject *self) {
 DeeReturn_Bool(self->sl_list.sl_size == 0);
}
static DeeSingleListIteratorObject *
_deesinglelist_tp_seq_iter_self(DeeSingleListObject *self) {
 DeeSingleListIteratorObject *result;
 struct DeeSingleListNode *result_node;
 result_node = DeeSingleList_Front(&self->sl_list);
 result = DeeSingleListIterator_New(self,result_node);
 if (result_node) DeeSingleListNode_DECREF(result_node);
 return result;
}
static int DEE_CALL _deesinglelist_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSingleListObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 (void)self,infile,map; // TODO
 DeeError_TODONotImplemented();
 return -1;
}

static int DEE_CALL _deesinglelist_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSingleListObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 (void)self,outfile,map; // TODO
 DeeError_TODONotImplemented();
 return -1;
}










//////////////////////////////////////////////////////////////////////////
// VTable: single_list.iterator
static int _deesinglelistiterator_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeSingleListIteratorObject *self) {
 DeeSingleListIteratorObject_Init(self);
 return 0;
}
static int _deesinglelistiterator_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeSingleListIteratorObject *self, DeeSingleListIteratorObject *right) {
 DeeSingleListIteratorObject_InitCopy(self,right);
 return 0;
}
static int _deesinglelistiterator_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeSingleListIteratorObject *self, DeeSingleListIteratorObject *right) {
 DeeSingleListIteratorObject_InitMove(self,right);
 return 0;
}
static void _deesinglelistiterator_tp_dtor(DeeSingleListIteratorObject *self) {
 Dee_XDECREF(self->sli_list);
 DeeSingleListIterator_Quit(&self->sli_iter);
}
static void _deesinglelistiterator_tp_clear(DeeSingleListIteratorObject *self) {
 struct DeeSingleListNode *old_node;
 DeeSingleListObject *old_list;
 DeeAtomicMutex_AcquireRelaxed(&self->sli_lock);
 old_node = self->sli_iter.sli_node;
 old_list = self->sli_list;
 self->sli_iter.sli_node = NULL;
 self->sli_list = NULL;
 DeeAtomicMutex_Release(&self->sli_lock);
 if (old_node) DeeSingleListNode_DECREF(old_node);
 Dee_XDECREF(old_list);
}
DEE_VISIT_PROC(_deesinglelistiterator_tp_visit,DeeSingleListIteratorObject *self) {
 Dee_VISIT(self->sli_list);
}
static DeeObject *_deesinglelistiterator_tp_str(DeeSingleListIteratorObject *self) {
 DeeObject *elem; DeeSingleListObject *list; struct DeeSingleListNode *stored_node;
 DeeAtomicMutex_AcquireRelaxed(&self->sli_lock);
 if (!self->sli_iter.sli_node) {
  DeeAtomicMutex_Release(&self->sli_lock);
  DeeReturn_STATIC_STRING("<single_list.iterator>");
 }
 DEE_ASSERT(self->sli_list);
 stored_node = self->sli_iter.sli_node;
 DeeSingleListNode_INCREF(stored_node);
 Dee_INCREF(list = self->sli_list);
 DeeAtomicMutex_Release(&self->sli_lock);
 DeeAtomicMutex_AcquireRelaxed(&list->sl_list.sl_lock);
 Dee_INCREF(elem = stored_node->sln_elem);
 DeeAtomicMutex_Release(&list->sl_list.sl_lock);
 DeeSingleListNode_DECREF(stored_node);
 Dee_DECREF(list);
 return DeeString_Newf("<single_list.iterator -> %K>",elem);
}
static DeeObject *_deesinglelistiterator_tp_repr(DeeSingleListIteratorObject *self) {
 DeeObject *elem; DeeSingleListObject *list; struct DeeSingleListNode *stored_node;
 DeeAtomicMutex_AcquireRelaxed(&self->sli_lock);
 if (!self->sli_iter.sli_node) {
  DeeAtomicMutex_Release(&self->sli_lock);
  DeeReturn_STATIC_STRING("<single_list.iterator>");
 }
 DEE_ASSERT(self->sli_list);
 stored_node = self->sli_iter.sli_node;
 DeeSingleListNode_INCREF(stored_node);
 Dee_INCREF(list = self->sli_list);
 DeeAtomicMutex_Release(&self->sli_lock);
 DeeAtomicMutex_AcquireRelaxed(&list->sl_list.sl_lock);
 Dee_INCREF(elem = stored_node->sln_elem);
 DeeAtomicMutex_Release(&list->sl_list.sl_lock);
 DeeSingleListNode_DECREF(stored_node);
 Dee_DECREF(list);
 return DeeString_Newf("<single_list.iterator -> %R>",elem);
}
static int _deesinglelistiterator_tp_seq_iter_next(
 DeeSingleListIteratorObject *self, DeeObject **result) {
 struct DeeSingleListNode *old_node,*new_node;
 DeeSingleListObject *list;
 DEE_ASSERT(self); DEE_ASSERT(result);
again:
 DeeAtomicMutex_AcquireRelaxed(&self->sli_lock);
 if ((old_node = self->sli_iter.sli_node) == NULL) {
  DeeAtomicMutex_Release(&self->sli_lock);
  return 1; // End of iterator
 }
 DEE_ASSERT(self->sli_list);
 DeeSingleListNode_INCREF(old_node);
 Dee_INCREF(list = self->sli_list);
 DeeAtomicMutex_Release(&self->sli_lock);
 Dee_INCREF(*result = old_node->sln_elem);

 DeeAtomicMutex_AcquireRelaxed(&list->sl_list.sl_lock);
 if ((new_node = old_node->sln_next) != NULL) {
  DeeSingleListNode_INCREF(new_node);
 }
 DeeAtomicMutex_Release(&list->sl_list.sl_lock);
 DeeSingleListNode_DECREF(old_node);
 Dee_DECREF(list);

 DeeAtomicMutex_AcquireRelaxed(&self->sli_lock);
 if DEE_UNLIKELY(old_node != self->sli_iter.sli_node) {
  // Some other thread is also using this iterator
  // >> Try again
  DeeAtomicMutex_Release(&self->sli_lock);
  Dee_DECREF(*result);
  DeeSingleListNode_DECREF(new_node);
  goto again;
 }
 // Implicitly transfer a reference from 'self->sli_iter.sli_node' to 'old_node'
 self->sli_iter.sli_node = new_node; // Inherit reference
 DeeAtomicMutex_Release(&self->sli_lock);
 DeeSingleListNode_DECREF(old_node);
 return 0;
}
#if 0
static DeeSingleListIteratorObject *_deesinglelistiterator_tp_add(
 DeeSingleListIteratorObject *self, DeeObject *right) {
 DeeSingleListIteratorObject *result; Dee_ssize_t n;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_ssize_t,right,&n) != 0) return NULL;
 if DEE_UNLIKELY((result = DeeGC_MALLOC(DeeSingleListIteratorObject)) == NULL) return NULL;
 DeeSingleListIteratorObject_InitCopy(result,self);
 if DEE_LIKELY(result->sli_list != NULL) {
  if (n > 0) {
   struct DeeSingleListNode *new_node;
   DeeAtomicMutex_AcquireRelaxed(&result->sli_list->sl_list.sl_lock);
   new_node = result->sli_iter.sli_node;
   while (new_node) {
    new_node = new_node->sln_next;
    if (!--n) { DeeSingleListNode_INCREF(new_node); break; }
   }
   DeeAtomicMutex_Release(&result->sli_list->sl_list.sl_lock);
   if (result->sli_iter.sli_node)
    DeeSingleListNode_DECREF(result->sli_iter.sli_node);
   result->sli_iter.sli_node = new_node;
  } else if (n < 0) {
   // TODO...
  }
 }
 DeeGC_TrackedAdd((DeeObject *)result);
 return result;
}
#endif





static int DEE_CALL _deesinglelistiterator_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSingleListIteratorObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 (void)self,infile,map; // TODO
 DeeError_TODONotImplemented();
 return -1;
}

static int DEE_CALL _deesinglelistiterator_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeSingleListIteratorObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 (void)self,outfile,map; // TODO
 DeeError_TODONotImplemented();
 return -1;
}



static DeeSingleListIteratorObject *DEE_CALL _deesinglelist_begin(
 DeeSingleListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 struct DeeSingleListNode *result_node;
 DeeSingleListIteratorObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":begin") != 0) return NULL;
 if DEE_UNLIKELY((result_node = DeeSingleList_Front(&self->sl_list)) == NULL) return NULL;
 result = DeeSingleListIterator_New(self,result_node);
 if (result_node) DeeSingleListNode_DECREF(result_node);
 return result;
}
static DeeSingleListIteratorObject *DEE_CALL _deesinglelist_end(
 DeeSingleListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":end") != 0) return NULL;
 return DeeSingleListIterator_New(self,NULL);
}


static DeeSingleListIteratorObject *DEE_CALL _deesinglelist_insert(
 DeeSingleListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *index,*elem; struct DeeSingleListNode *pos,*result_node; int error;
 DeeSingleListIteratorObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:insert",&index,&elem) != 0) return NULL;
 if DEE_UNLIKELY(DeeSingleListNode_PtrFromObject(&self->sl_list,index,&pos) != 0) return NULL;
 error = DeeSingleList_Insert(&self->sl_list,pos,elem,&result_node);
 if (pos) DeeSingleListNode_DECREF(pos);
 if DEE_UNLIKELY(error != 0) return NULL;
 result = DeeSingleListIterator_New(self,result_node);
 if (result_node) DeeSingleListNode_DECREF(result_node);
 return result;
}
static DeeSingleListIteratorObject *DEE_CALL _deesinglelist_insert_after(
 DeeSingleListObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *index,*elem; struct DeeSingleListNode *pos,*result_node; int error;
 DeeSingleListIteratorObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo:insert_after",&index,&elem) != 0) return NULL;
 if DEE_UNLIKELY(DeeSingleListNode_PtrFromObject(&self->sl_list,index,&pos) != 0) return NULL;
 error = DeeSingleList_InsertAfter(&self->sl_list,pos,elem,&result_node);
 if (pos) DeeSingleListNode_DECREF(pos);
 if DEE_UNLIKELY(error != 0) return NULL;
 result = DeeSingleListIterator_New(self,result_node);
 if (result_node) DeeSingleListNode_DECREF(result_node);
 return result;
}



static struct DeeMethodDef const _deesinglelist_tp_methods[] = {
 // TODO: Intrinsic versions of all functions below
 DEE_METHODDEF_CONST_v100("at",member(&_deegenericiterable_at),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("back",member(&_deegenericiterable_back),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("contains",member(&_deegenericiterable_contains),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("count",member(&_deegenericiterable_count),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("empty",member(&_deegenericiterable_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("find",member(&_deegenericiterable_find),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("front",member(&_deegenericiterable_front),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("index",member(&_deegenericiterable_index),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("locate",member(&_deegenericiterable_locate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("locate_all",member(&_deegenericiterable_locate_all),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("non_empty",member(&_deegenericiterable_non_empty),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rfind",member(&_deegenericiterable_rfind),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rindex",member(&_deegenericiterable_rindex),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("rlocate",member(&_deegenericiterable_rlocate),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("segments",member(&_deegenericiterable_segments),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("sorted",member(&_deegenericiterable_sorted),DEE_DOC_AUTO),
 DEE_METHODDEF_CONST_v100("transform",member(&_deegenericiterable_transform),DEE_DOC_AUTO),
 DEE_METHODDEF_v100("begin",member(&_deesinglelist_begin),"() -> single_list.iterator"),
 DEE_METHODDEF_v100("end",member(&_deesinglelist_end),"() -> single_list.iterator"),
 DEE_METHODDEF_v100("insert",member(&_deesinglelist_insert),
                    "(ssize_t index, object elem) -> single_list.iterator\n"
                    "(single_list.iterator index, object elem) -> single_list.iterator\n"),
 DEE_METHODDEF_v100("insert_after",member(&_deesinglelist_insert_after),
                    "(ssize_t index, object elem) -> single_list.iterator\n"
                    "(single_list.iterator index, object elem) -> single_list.iterator\n"),
 //TODO: DEE_METHODDEF_v100("remove_if",member(&_deelist_remove_if),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("remove",member(&_deelist_remove),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("insert_list",member(&_deelist_insert_list),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("insert_iter",member(&_deelist_insert_iter),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("sorted_insert",member(&_deelist_sorted_insert),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("append",member(&_deelist_append),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("extend",member(&_deelist_extend),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("erase",member(&_deelist_erase),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("pop",member(&_deelist_pop),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("clear",member(&_deelist_clear),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("resize",member(&_deelist_resize),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("reserve",member(&_deelist_reserve),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("shrink_to_fit",member(&_deelist_shrink_to_fit),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("reverse",member(&_deelist_reverse),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("sort",member(&_deelist_sort),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_CONST_v100("reversed",member(&_deelist_reversed),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_CONST_v100("allocated",member(&_deelist_allocated),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("fill",member(&_deelist_fill),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("push_front",member(&_deelist_push_front),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("push_back",member(&_deelist_push_back),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("pop_front",member(&_deelist_pop_front),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("pop_back",member(&_deelist_pop_back),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_v100("unique",member(&_deelist_unique),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_CONST_v100("tounique",member(&_deelist_tounique),DEE_DOC_AUTO),
 //TODO: DEE_METHODDEF_CONST_v100("extend_unique",member(&_deelist_extend_unique),DEE_DOC_AUTO),
 DEE_METHODDEF_END_v100
};

static struct DeeMemberDef const _deesinglelist_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("iterator",object,&DeeSingleListIterator_Type),
 DEE_MEMBERDEF_END_v100
};


static struct DeeTypeMarshal _deesinglelist_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_PP_INIT((cdc4f05c,b247,44c0,82fe,9b31a7dbddb9)),
 member(&_deesinglelist_tp_marshal_ctor),
 member(&_deesinglelist_tp_marshal_put));

static struct DeeTypeMarshal _deesinglelistiterator_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_PP_INIT((14f2987a,689a,4efc,b262,94fa7821593b)),
 member(&_deesinglelistiterator_tp_marshal_ctor),
 member(&_deesinglelistiterator_tp_marshal_put));


DeeTypeObject DeeSingleList_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("single_list"),member(
  "()\n"
  "(iterable sequence)\n"
  "@param sequence: A given sequence that should be casted to a #(single_list)."
  "\tA resizeable and mutable singly-linked list of objects."),member(
  DEE_TYPE_FLAG_HAS_GC|DEE_TYPE_FLAG_NO_ITERATE_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeSingleListObject),
  member(&_DeeGC_TpAlloc),
  member(&_deesinglelist_tp_ctor),
  member(&_deesinglelist_tp_copy_ctor),
  member(&_deesinglelist_tp_move_ctor),
  member(&_deesinglelist_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(member(&_DeeGC_TpFree),
  member(&_deesinglelist_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deesinglelist_tp_copy_assign),
  member(&_deesinglelist_tp_move_assign),
  member(&_deesinglelist_tp_any_assign)),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deesinglelist_tp_str),
  member(&_deesinglelist_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deesinglelist_tp_visit),
  member(&_deesinglelist_tp_clear)),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deesinglelist_tp_not),member(&_deesinglelist_tp_bool),
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null), // TODO tp_hash
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deegenericiterable_tp_cmp_lo),
  member(&_deegenericiterable_tp_cmp_le),
  member(&_deegenericiterable_tp_cmp_eq),
  member(&_deegenericiterable_tp_cmp_ne),
  member(&_deegenericiterable_tp_cmp_gr),
  member(&_deegenericiterable_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  member(&_deegenericiterable_tp_seq_get),       // TODO: member(&_deesinglelist_tp_seq_get),
  null,                                          // TODO: member(&_deesinglelist_tp_seq_del),
  null,                                          // TODO: member(&_deesinglelist_tp_seq_set),
  member(&_deegenericiterable_tp_seq_size),      // TODO: member(&_deesinglelist_tp_seq_size),
  member(&_deegenericiterable_tp_seq_contains),  // TODO: member(&_deesinglelist_tp_seq_contains),
  member(&_deegenericiterable_tp_seq_range_get), // TODO: member(&_deesinglelist_tp_seq_range_get),
  null,                                          // TODO: member(&_deesinglelist_tp_seq_range_del),
  null,                                          // TODO: member(&_deesinglelist_tp_seq_range_set),
  member(&_deesinglelist_tp_seq_iter_self),null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,null,member(_deesinglelist_tp_methods),
  member(_deesinglelist_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deesinglelist_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};

DeeTypeObject DeeSingleListIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("single_list.iterator"),null,
  member(DEE_TYPE_FLAG_HAS_GC),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeSingleListIteratorObject),
  member(&_DeeGC_TpAlloc),null,null,null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(
  member(&_DeeGC_TpFree),
  member(&_deesinglelistiterator_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deesinglelistiterator_tp_str),
  member(&_deesinglelistiterator_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v101(null,
  member(&_deesinglelistiterator_tp_visit),
  member(&_deesinglelistiterator_tp_clear)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,
  member(&_deesinglelistiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deesinglelistiterator_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};






//////////////////////////////////////////////////////////////////////////
// Module exports
int DeeDex_Main(DEE_A_INOUT struct DeeDexContext *context) {
 switch (context->dc_kind) {
  case DEE_DEXCONTEXTKIND_INITIALIZE:
   if (DeeMarshal_RegisterType(&DeeSingleList_Type) < 0) return -1;
   if (DeeMarshal_RegisterType(&DeeSingleListIterator_Type) < 0) return -1;
   break;
  case DEE_DEXCONTEXTKIND_FINALIZE:
   DeeMarshal_UnregisterType(&DeeSingleList_Type);
   DeeMarshal_UnregisterType(&DeeSingleListIterator_Type);
   break;
  default: break;
 }
 return 0;
}


struct DeeDexExportDef DeeDex_Exports[] = {
 DeeDex_EXPORT_OBJECT("single_list",&DeeSingleList_Type),
 DeeDex_EXPORT_OBJECT("deque",&DeeDeque_Type),
 DeeDex_EXPORT_END
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_SEQUENCE_C */
