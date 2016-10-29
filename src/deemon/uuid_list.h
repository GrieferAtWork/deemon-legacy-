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
#ifndef GUARD_DEEMON_UUID_LIST_H
#define GUARD_DEEMON_UUID_LIST_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/uuid.h>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif

struct DeeUUIDListEntry {
 struct DeeUUIDListEntry *ule_next; /*< [0..1] Next entry. */
 struct DeeUUID           ule_uuid; /*< uuid associated with this entry. */
 DeeObject               *ule_elem; /*< [1..1] Element associated with the uuid. */
};
struct DeeUUIDList {
#ifndef DeeUUIDList_MAPSIZE
#define DeeUUIDList_MAPSIZE 0xFF
#endif
 struct DeeUUIDListEntry *ul_map[DeeUUIDList_MAPSIZE];
};
#define DeeUUIDList_INIT() {{NULL},}


#define DeeUUIDList_UUID_INDEX(uuid)\
(Dee_uint8_t)(\
 (Dee_uint8_t)(\
  (Dee_uint8_t)(\
   (Dee_uint8_t)(\
    (uuid)->uuid_a[0]+(uuid)->uuid_a[1]\
   )+(Dee_uint8_t)(\
    (uuid)->uuid_a[2]+(uuid)->uuid_a[3]\
   )\
  )+(Dee_uint8_t)(\
   (Dee_uint8_t)(\
    (uuid)->uuid_b[0]+(uuid)->uuid_b[1]\
   )+(Dee_uint8_t)(\
    (uuid)->uuid_c[0]+(uuid)->uuid_c[1]\
   )\
  )\
 )\
+(Dee_uint8_t)(\
  (Dee_uint8_t)(\
   (Dee_uint8_t)(\
    (uuid)->uuid_d[0]+(uuid)->uuid_d[1]\
   )+(Dee_uint8_t)(\
    (uuid)->uuid_e[0]+(uuid)->uuid_e[1]\
   )\
  )+(Dee_uint8_t)(\
   (Dee_uint8_t)(\
    (uuid)->uuid_e[2]+(uuid)->uuid_e[3]\
   )+(Dee_uint8_t)(\
    (uuid)->uuid_e[4]+(uuid)->uuid_e[5]\
   )\
  )\
 )\
)


#define _DeeUUIDList_Init(ob) (void)memset(ob,0,sizeof(struct DeeUUIDList))

#define DEE_UUIDLIST_ADD_ERROR_SUCCESS     0  /*< Object successfully added. */
#define DEE_UUIDLIST_ADD_ERROR_NOMEM     (-1) /*< Not enough memory (free something and try again). */
#define DEE_UUIDLIST_ADD_ERROR_DUPLICATE (-2) /*< uuid was already in use. */
DEE_STATIC_INLINE(int) DeeUUIDList_Add(DEE_A_INOUT struct DeeUUIDList *self,
                                       DEE_A_IN struct DeeUUID const *uuid,
                                       DEE_A_IN DeeObject *elem);
// Delete the object matching 'uuid' and returns its previously stored reference
// NOTE: This function isn't DEE_EXEC and is safe to call from inside an atomic lock
DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT_REF DeeObject *) DeeUUIDList_Del(
 DEE_A_INOUT struct DeeUUIDList *self, DEE_A_IN struct DeeUUID const *uuid);
DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT_REF DeeObject *) DeeUUIDList_PopOne(
 DEE_A_INOUT struct DeeUUIDList *self, DEE_A_OUT_OPT struct DeeUUID *uuid);
DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(NULL) DeeObject *) DeeUUIDList_Lookup(
 DEE_A_IN struct DeeUUIDList const *self, DEE_A_IN struct DeeUUID const *uuid);





DEE_STATIC_INLINE(int) DeeUUIDList_Add(
 DEE_A_INOUT struct DeeUUIDList *self,
 DEE_A_IN struct DeeUUID const *uuid, DEE_A_IN DeeObject *elem) {
 struct DeeUUIDListEntry *chain; Dee_uint8_t idx;
 DEE_ASSERT(self);
 DEE_ASSERT(uuid);
 DEE_ASSERT(DeeObject_Check(elem));
 idx = DeeUUIDList_UUID_INDEX(uuid);
 chain = self->ul_map[idx];
 while (chain) {
  if (DeeUUID_EQUAL(&chain->ule_uuid,uuid))
   return DEE_UUIDLIST_ADD_ERROR_DUPLICATE;
  chain = chain->ule_next;
 }
 if ((chain = (struct DeeUUIDListEntry *)
  malloc_nz(sizeof(struct DeeUUIDListEntry))) == NULL)
  return DEE_UUIDLIST_ADD_ERROR_NOMEM;
 chain->ule_next = self->ul_map[idx];
 self->ul_map[idx] = chain;
 DeeUUID_SET(&chain->ule_uuid,uuid);
 chain->ule_elem = elem;
 return DEE_UUIDLIST_ADD_ERROR_SUCCESS;
}
DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(NULL) DeeObject *) DeeUUIDList_Del(
 DEE_A_INOUT struct DeeUUIDList *self, DEE_A_IN struct DeeUUID const *uuid) {
 struct DeeUUIDListEntry *chain,*prev; DeeObject *result; Dee_uint8_t idx;
 DEE_ASSERT(self); DEE_ASSERT(uuid);
 idx = DeeUUIDList_UUID_INDEX(uuid);
 chain = self->ul_map[idx]; prev = NULL;
 while (chain) {
  if (DeeUUID_EQUAL(&chain->ule_uuid,uuid)) {
   if (prev) {
    DEE_ASSERT(prev->ule_next == chain);
    prev->ule_next = chain->ule_next;
   } else {
    DEE_ASSERT(self->ul_map[idx] == chain);
    self->ul_map[idx] = chain->ule_next;
   }
   result = chain->ule_elem;
   free_nn(chain);
   return result;
  }
  chain = (prev = chain)->ule_next;
 }
 return NULL;
}
DEE_A_RET_NOEXCEPT(NULL) DeeObject *DeeUUIDList_PopOne(
 DEE_A_INOUT struct DeeUUIDList *self, DEE_A_OUT_OPT struct DeeUUID *uuid) {
 struct DeeUUIDListEntry **iter,**end,*chain; DeeObject *result;
 DEE_ASSERT(self);
 end = (iter = self->ul_map)+DeeUUIDList_MAPSIZE;
 while (iter != end) {
  if (*iter) {
   chain = *iter;
   result = chain->ule_elem;
   *iter = chain->ule_next;
   if (uuid) DeeUUID_SET(uuid,&chain->ule_uuid);
   free_nn(chain);
   return result;
  }
  ++iter;
 }
 return NULL;
}

DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(NULL) DeeObject *) DeeUUIDList_Lookup(
 DEE_A_IN struct DeeUUIDList const *self, DEE_A_IN struct DeeUUID const *uuid) {
 struct DeeUUIDListEntry *chain; Dee_uint8_t idx;
 DEE_ASSERT(self); DEE_ASSERT(uuid);
 idx = DeeUUIDList_UUID_INDEX(uuid);
 chain = self->ul_map[idx];
 while (chain) {
  if (DeeUUID_EQUAL(&chain->ule_uuid,uuid))
   return chain->ule_elem;
  chain = chain->ule_next;
 }
 return NULL;
}


DEE_DECL_END

#endif /* !GUARD_DEEMON_UUID_LIST_H */
