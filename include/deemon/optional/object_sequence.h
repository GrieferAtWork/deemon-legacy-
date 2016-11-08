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
#ifndef GUARD_DEEMON_OPTIONAL_OBJECT_SEQUENCE_H
#define GUARD_DEEMON_OPTIONAL_OBJECT_SEQUENCE_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/object_decl.h>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_HASH_T
DEE_PRIVATE_DECL_DEE_HASH_T
#undef DEE_PRIVATE_DECL_DEE_HASH_T
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_STRUCTURED_TYPEOBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT_ITERATE_FUNC
DEE_PRIVATE_DECL_DEE_OBJECT_ITERATE_FUNC
#undef DEE_PRIVATE_DECL_DEE_OBJECT_ITERATE_FUNC
#endif


//////////////////////////////////////////////////////////////////////////
// Generic sequence interface
#define /* DEE_A_EXEC DeeObject * */DeeObject_IterSelf(ob) DeeObject_TIterSelf(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC int */DeeObject_IterNextEx(ob,elem) DeeObject_TIterNextEx(Dee_TYPE(ob),ob,elem)
#define /* DEE_A_EXEC int */DeeObject_Size(ob,result) DeeObject_TSize(Dee_TYPE(ob),ob,result)
#define /* DEE_A_EXEC DeeObject * */DeeObject_SizeObject(ob) DeeObject_TSizeObject(Dee_TYPE(ob),ob)
#define /* DEE_A_EXEC DeeObject * */DeeObject_GetIndex(ob,i) DeeObject_TGetIndex(Dee_TYPE(ob),ob,i)
#define /* DEE_A_EXEC int */DeeObject_DelIndex(ob,i) DeeObject_TDelIndex(Dee_TYPE(ob),ob,i)
#define /* DEE_A_EXEC int */DeeObject_SetIndex(ob,i,v) DeeObject_TSetIndex(Dee_TYPE(ob),ob,i,v)
#define /* DEE_A_EXEC DeeObject * */DeeObject_GetRangeIndex(ob,begin,end) DeeObject_TGetRangeIndex(Dee_TYPE(ob),ob,begin,end)
#define /* DEE_A_EXEC int */DeeObject_DelRangeIndex(ob,begin,end) DeeObject_TDelRangeIndex(Dee_TYPE(ob),ob,begin,end)
#define /* DEE_A_EXEC int */DeeObject_SetRangeIndex(ob,begin,end,v) DeeObject_TSetRangeIndex(Dee_TYPE(ob),ob,begin,end,v)
#define /* DEE_A_EXEC int */DeeObject_Contains(ob,elem) DeeObject_TContains(Dee_TYPE(ob),ob,elem)
#define /* DEE_A_EXEC DeeObject * */DeeObject_ContainsObject(ob,elem) DeeObject_TContainsObject(Dee_TYPE(ob),ob,elem)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TIterSelf(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeObject_TIterNextEx(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_REF DEE_A_OUT DeeObject **elem) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TSize(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_OUT Dee_size_t *result) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TSizeObject(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TGetIndex(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t i) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TDelIndex(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t i) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TSetIndex(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t i, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TGetRangeIndex(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TDelRangeIndex(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TSetRangeIndex(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,5));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeObject_TContains(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TContainsObject(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2,3));

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PIterSelf(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PSize(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_OUT Dee_size_t *result) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PSizeObject(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PGetIndex(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN Dee_size_t i) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PDelIndex(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN Dee_size_t i) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PSetIndex(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN Dee_size_t i, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PGetRangeIndex(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PDelRangeIndex(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PSetRangeIndex(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,5));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeObject_PContains(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PContainsObject(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *elem) DEE_ATTRIBUTE_NONNULL((1,2,3));

//////////////////////////////////////////////////////////////////////////
// Generic mapping interface
#define /* DEE_A_EXEC DeeObject * */DeeObject_GetItem(ob,key) DeeObject_TGetItem(Dee_TYPE(ob),ob,key)
#define /* DEE_A_EXEC int */DeeObject_HasItem(ob,key) DeeObject_THasItem(Dee_TYPE(ob),ob,key)
#define /* DEE_A_EXEC int */DeeObject_DelItem(ob,key) DeeObject_TDelItem(Dee_TYPE(ob),ob,key)
#define /* DEE_A_EXEC int */DeeObject_SetItem(ob,key,item) DeeObject_TSetItem(Dee_TYPE(ob),ob,key,item)
#define /* DEE_A_EXEC DeeObject * */DeeObject_GetItemString(ob,key) DeeObject_TGetItemString(Dee_TYPE(ob),ob,key)
#define /* DEE_A_EXEC int */DeeObject_HasItemString(ob,key) DeeObject_THasItemString(Dee_TYPE(ob),ob,key)
#define /* DEE_A_EXEC int */DeeObject_DelItemString(ob,key) DeeObject_TDelItemString(Dee_TYPE(ob),ob,key)
#define /* DEE_A_EXEC int */DeeObject_SetItemString(ob,key,item) DeeObject_TSetItemString(Dee_TYPE(ob),ob,key,item)
#define /* DEE_A_EXEC DeeObject * */DeeObject_GetRangeItem(ob,begin,end) DeeObject_TGetRangeItem(Dee_TYPE(ob),ob,begin,end)
#define /* DEE_A_EXEC int */DeeObject_DelRangeItem(ob,begin,end) DeeObject_TDelRangeItem(Dee_TYPE(ob),ob,begin,end)
#define /* DEE_A_EXEC int */DeeObject_SetRangeItem(ob,begin,end,v) DeeObject_TSetRangeItem(Dee_TYPE(ob),ob,begin,end,v)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TGetItem(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TDelItem(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TSetItem(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeObject_THasItem(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeObject_THasItemString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TGetItemString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TDelItemString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TSetItemString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_TGetRangeItem(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *begin, DEE_A_INOUT DeeObject *end) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TDelRangeItem(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *begin, DEE_A_INOUT DeeObject *end) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TSetRangeItem(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *begin, DEE_A_INOUT DeeObject *end, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3,4,5));

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PGetItem(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PDelItem(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PSetItem(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeObject_PHasItem(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *key) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeObject_PHasItemString(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PGetItemString(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PDelItemString(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *key) DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PSetItemString(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_IN_Z char const *key, DEE_A_INOUT DeeObject *item) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeObject_PGetRangeItem(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *begin, DEE_A_INOUT DeeObject *end) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PDelRangeItem(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *begin, DEE_A_INOUT DeeObject *end) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_PSetRangeItem(DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr, DEE_A_INOUT DeeObject *begin, DEE_A_INOUT DeeObject *end, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3,4,5));


//////////////////////////////////////////////////////////////////////////
// Iterate an object using its iterator interface
//  - returns   -1: if other errors caused problems
//  - returns    0: on success
//  - returns != 0: "func" returned a != 0 value, that aborted the interation
#define DeeObject_Iterate(ob,func,closure) DeeObject_TIterate(Dee_TYPE(ob),ob,func,closure)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeObject_TIterate(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_IN DeeObjectIterateFunc func, DEE_A_IN_OPT void *closure) DEE_ATTRIBUTE_NONNULL((1,2,3));

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_OBJECT_SEQUENCE_H */
