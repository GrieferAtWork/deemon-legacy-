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
#ifndef GUARD_DEEMON_SEQUENCE_C
#define GUARD_DEEMON_SEQUENCE_C 1
#define DEE_LIMITED_API 1

// /include/*
#include <deemon/__conf.inl>
#include <deemon/array.h>
#include <deemon/cell.h>
#include <deemon/dict.h>
#include <deemon/error.h>
#include <deemon/float.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/list.h>
#include <deemon/marshal_data.h>
#include <deemon/none.h>
#include <deemon/optional/type_util.h>
#include <deemon/sequence.h>
#include <deemon/super.h>
#include <deemon/set.h>
#include <deemon/string.h>
#include <deemon/structured.h>
#include <deemon/tuple.h>

// */ (nano...)

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeSequence_TFind(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) {
 DeeObject *args,*result_ob; Dee_ssize_t result; int error;
 DeeString_NEW_STATIC(member_name,"find");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 if (tp_self == &DeeList_Type) return DeeList_Find(self,findob);
 if (tp_self == &DeeTuple_Type) return DeeTuple_Find(self,findob);
 if (tp_self == &DeeSet_Type) return DeeSet_FindKeyIndex(self,findob);
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (tp_self == &DeeUtf8String_Type) { if DEE_UNLIKELY((findob = DeeUtf8String_Cast(findob)) == NULL) return (Dee_ssize_t)-2; result = DeeUtf8String_Find(self,findob,0,DeeUtf8String_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (tp_self == &DeeWideString_Type) { if DEE_UNLIKELY((findob = DeeWideString_Cast(findob)) == NULL) return (Dee_ssize_t)-2; result = DeeWideString_Find(self,findob,0,DeeWideString_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (tp_self == &DeeUtf16String_Type) { if DEE_UNLIKELY((findob = DeeUtf16String_Cast(findob)) == NULL) return (Dee_ssize_t)-2; result = DeeUtf16String_Find(self,findob,0,DeeUtf16String_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (tp_self == &DeeUtf32String_Type) { if DEE_UNLIKELY((findob = DeeUtf32String_Cast(findob)) == NULL) return (Dee_ssize_t)-2; result = DeeUtf32String_Find(self,findob,0,DeeUtf32String_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 if (tp_self == &DeeSuper_Type) return DeeSequence_TFind(DeeSuper_TYPE(self),DeeSuper_SELF(self),findob);
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,findob)) == NULL) return (Dee_ssize_t)-2;
 result_ob = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(!result_ob) {
  if DEE_LIKELY(DeeError_Catch(&DeeErrorType_AttributeError) ||
                DeeError_Catch(&DeeErrorType_NotImplemented)
                ) return DeeGeneric_TIterableFind(tp_self,self,findob);
  return (Dee_ssize_t)-2;
 }
 error = DeeObject_Cast(Dee_ssize_t,result_ob,&result);
 Dee_DECREF(result_ob);
 if DEE_UNLIKELY(error != 0) return (Dee_ssize_t)-2;
 if DEE_UNLIKELY(result == (Dee_ssize_t)-2) DeeError_SET_STRING(&DeeErrorType_ValueError,"Invalid index");
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeSequence_TFindPred(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) {
 DeeObject *args,*result_ob; Dee_ssize_t result; int error;
 DeeString_NEW_STATIC(member_name,"find");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 DEE_ASSERT(DeeObject_Check(pred));
 if (tp_self == &DeeList_Type) return DeeList_FindPred(self,findob,pred);
 if (tp_self == &DeeTuple_Type) return DeeTuple_FindPred(self,findob,pred);
 if (tp_self == &DeeSuper_Type) return DeeSequence_TFindPred(DeeSuper_TYPE(self),DeeSuper_SELF(self),findob,pred);
 if DEE_UNLIKELY((args = DeeTuple_Pack(2,findob,pred)) == NULL) return (Dee_ssize_t)-2;
 result_ob = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(!result_ob) {
  if DEE_LIKELY(DeeError_Catch(&DeeErrorType_AttributeError) ||
                DeeError_Catch(&DeeErrorType_NotImplemented)
                ) return DeeGeneric_TIterableFindPred(tp_self,self,findob,pred);
  return (Dee_ssize_t)-2;
 }
 error = DeeObject_Cast(Dee_ssize_t,result_ob,&result);
 Dee_DECREF(result_ob);
 if DEE_UNLIKELY(error != 0) return (Dee_ssize_t)-2;
 if DEE_UNLIKELY(result == (Dee_ssize_t)-2) DeeError_SET_STRING(&DeeErrorType_ValueError,"Invalid index");
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeSequence_TRFind(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) {
 DeeObject *args,*result_ob; Dee_ssize_t result; int error;
 DeeString_NEW_STATIC(member_name,"rfind");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 if (tp_self == &DeeList_Type) return DeeList_RFind(self,findob);
 if (tp_self == &DeeTuple_Type) return DeeTuple_RFind(self,findob);
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (tp_self == &DeeUtf8String_Type) { if DEE_UNLIKELY((findob = DeeUtf8String_Cast(findob)) == NULL) return (Dee_ssize_t)-2; result = DeeUtf8String_RFind(self,findob,0,DeeUtf8String_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (tp_self == &DeeWideString_Type) { if DEE_UNLIKELY((findob = DeeWideString_Cast(findob)) == NULL) return (Dee_ssize_t)-2; result = DeeWideString_RFind(self,findob,0,DeeWideString_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (tp_self == &DeeUtf16String_Type) { if DEE_UNLIKELY((findob = DeeUtf16String_Cast(findob)) == NULL) return (Dee_ssize_t)-2; result = DeeUtf16String_RFind(self,findob,0,DeeUtf16String_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (tp_self == &DeeUtf32String_Type) { if DEE_UNLIKELY((findob = DeeUtf32String_Cast(findob)) == NULL) return (Dee_ssize_t)-2; result = DeeUtf32String_RFind(self,findob,0,DeeUtf32String_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 if (tp_self == &DeeSuper_Type) return DeeSequence_TRFind(DeeSuper_TYPE(self),DeeSuper_SELF(self),findob);
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,findob)) == NULL) return (Dee_ssize_t)-2;
 result_ob = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(!result_ob) {
  if DEE_LIKELY(DeeError_Catch(&DeeErrorType_AttributeError) ||
                DeeError_Catch(&DeeErrorType_NotImplemented)
                ) return DeeGeneric_TIterableRFind(tp_self,self,findob);
  return (Dee_ssize_t)-2;
 }
 error = DeeObject_Cast(Dee_ssize_t,result_ob,&result);
 Dee_DECREF(result_ob);
 if DEE_UNLIKELY(error != 0) return (Dee_ssize_t)-2;
 if DEE_UNLIKELY(result == (Dee_ssize_t)-2) DeeError_SET_STRING(&DeeErrorType_ValueError,"Invalid index");
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-2,-1) Dee_ssize_t DeeSequence_TRFindPred(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) {
 DeeObject *args,*result_ob; Dee_ssize_t result; int error;
 DeeString_NEW_STATIC(member_name,"rfind");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 DEE_ASSERT(DeeObject_Check(pred));
 if (tp_self == &DeeList_Type) return DeeList_RFindPred(self,findob,pred);
 if (tp_self == &DeeTuple_Type) return DeeTuple_RFindPred(self,findob,pred);
 if (tp_self == &DeeSuper_Type) return DeeSequence_TRFindPred(DeeSuper_TYPE(self),DeeSuper_SELF(self),findob,pred);
 if DEE_UNLIKELY((args = DeeTuple_Pack(2,findob,pred)) == NULL) return (Dee_ssize_t)-2;
 result_ob = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(!result_ob) {
  if DEE_LIKELY(DeeError_Catch(&DeeErrorType_AttributeError) ||
                DeeError_Catch(&DeeErrorType_NotImplemented)
                ) return DeeGeneric_TIterableRFindPred(tp_self,self,findob,pred);
  return (Dee_ssize_t)-2;
 }
 error = DeeObject_Cast(Dee_ssize_t,result_ob,&result);
 Dee_DECREF(result_ob);
 if DEE_UNLIKELY(error != 0) return (Dee_ssize_t)-2;
 if DEE_UNLIKELY(result == (Dee_ssize_t)-2) DeeError_SET_STRING(&DeeErrorType_ValueError,"Invalid index");
 return result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeSequence_TIndex(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) {
 DeeObject *args,*result_ob; Dee_size_t result; int error;
 DeeString_NEW_STATIC(member_name,"index");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 if (tp_self == &DeeList_Type) return DeeList_Index(self,findob);
 if (tp_self == &DeeTuple_Type) return DeeTuple_Index(self,findob);
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (tp_self == &DeeUtf8String_Type) { if DEE_UNLIKELY((findob = DeeUtf8String_Cast(findob)) == NULL) return (Dee_size_t)-1; result = DeeUtf8String_Index(self,findob,0,DeeUtf8String_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (tp_self == &DeeWideString_Type) { if DEE_UNLIKELY((findob = DeeWideString_Cast(findob)) == NULL) return (Dee_size_t)-1; result = DeeWideString_Index(self,findob,0,DeeWideString_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (tp_self == &DeeUtf16String_Type) { if DEE_UNLIKELY((findob = DeeUtf16String_Cast(findob)) == NULL) return (Dee_size_t)-1; result = DeeUtf16String_Index(self,findob,0,DeeUtf16String_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (tp_self == &DeeUtf32String_Type) { if DEE_UNLIKELY((findob = DeeUtf32String_Cast(findob)) == NULL) return (Dee_size_t)-1; result = DeeUtf32String_Index(self,findob,0,DeeUtf32String_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 if (tp_self == &DeeSuper_Type) return DeeSequence_TIndex(DeeSuper_TYPE(self),DeeSuper_SELF(self),findob);
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,findob)) == NULL) return (Dee_size_t)-1;
 result_ob = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(!result_ob) {
  if DEE_LIKELY(DeeError_Catch(&DeeErrorType_AttributeError) ||
                DeeError_Catch(&DeeErrorType_NotImplemented)
                ) return DeeGeneric_TIterableIndex(tp_self,self,findob);
  return (Dee_size_t)-1;
 }
 error = DeeObject_Cast(Dee_size_t,result_ob,&result);
 Dee_DECREF(result_ob);
 if DEE_UNLIKELY(error != 0) return (Dee_size_t)-1;
 if DEE_UNLIKELY(result == (Dee_size_t)-1) DeeError_SET_STRING(&DeeErrorType_ValueError,"Invalid index");
 return result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeSequence_TIndexPred(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) {
 DeeObject *args,*result_ob; Dee_size_t result; int error;
 DeeString_NEW_STATIC(member_name,"index");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 DEE_ASSERT(DeeObject_Check(pred));
 if (tp_self == &DeeList_Type) return DeeList_IndexPred(self,findob,pred);
 if (tp_self == &DeeTuple_Type) return DeeTuple_IndexPred(self,findob,pred);
 if (tp_self == &DeeSuper_Type) return DeeSequence_TIndexPred(DeeSuper_TYPE(self),DeeSuper_SELF(self),findob,pred);
 if DEE_UNLIKELY((args = DeeTuple_Pack(2,findob,pred)) == NULL) return (Dee_size_t)-1;
 result_ob = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(!result_ob) {
  if DEE_LIKELY(DeeError_Catch(&DeeErrorType_AttributeError) ||
                DeeError_Catch(&DeeErrorType_NotImplemented)
                ) return DeeGeneric_TIterableIndexPred(tp_self,self,findob,pred);
  return (Dee_size_t)-1;
 }
 error = DeeObject_Cast(Dee_size_t,result_ob,&result);
 Dee_DECREF(result_ob);
 if DEE_UNLIKELY(error != 0) return (Dee_size_t)-1;
 if DEE_UNLIKELY(result == (Dee_size_t)-1) DeeError_SET_STRING(&DeeErrorType_ValueError,"Invalid index");
 return result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeSequence_TRIndex(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) {
 DeeObject *args,*result_ob; Dee_size_t result; int error;
 DeeString_NEW_STATIC(member_name,"rindex");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 if (tp_self == &DeeList_Type) return DeeList_RIndex(self,findob);
 if (tp_self == &DeeTuple_Type) return DeeTuple_RIndex(self,findob);
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (tp_self == &DeeUtf8String_Type) { if DEE_UNLIKELY((findob = DeeUtf8String_Cast(findob)) == NULL) return (Dee_size_t)-1; result = DeeUtf8String_RIndex(self,findob,0,DeeUtf8String_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (tp_self == &DeeWideString_Type) { if DEE_UNLIKELY((findob = DeeWideString_Cast(findob)) == NULL) return (Dee_size_t)-1; result = DeeWideString_RIndex(self,findob,0,DeeWideString_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (tp_self == &DeeUtf16String_Type) { if DEE_UNLIKELY((findob = DeeUtf16String_Cast(findob)) == NULL) return (Dee_size_t)-1; result = DeeUtf16String_RIndex(self,findob,0,DeeUtf16String_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (tp_self == &DeeUtf32String_Type) { if DEE_UNLIKELY((findob = DeeUtf32String_Cast(findob)) == NULL) return (Dee_size_t)-1; result = DeeUtf32String_RIndex(self,findob,0,DeeUtf32String_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 if (tp_self == &DeeSuper_Type) return DeeSequence_TRIndex(DeeSuper_TYPE(self),DeeSuper_SELF(self),findob);
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,findob)) == NULL) return (Dee_size_t)-1;
 result_ob = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(!result_ob) {
  if DEE_LIKELY(DeeError_Catch(&DeeErrorType_AttributeError) ||
                DeeError_Catch(&DeeErrorType_NotImplemented)
                ) return DeeGeneric_TIterableRIndex(tp_self,self,findob);
  return (Dee_size_t)-1;
 }
 error = DeeObject_Cast(Dee_size_t,result_ob,&result);
 Dee_DECREF(result_ob);
 if DEE_UNLIKELY(error != 0) return (Dee_size_t)-1;
 if DEE_UNLIKELY(result == (Dee_size_t)-1) DeeError_SET_STRING(&DeeErrorType_ValueError,"Invalid index");
 return result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeSequence_TRIndexPred(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) {
 DeeObject *args,*result_ob; Dee_size_t result; int error;
 DeeString_NEW_STATIC(member_name,"rindex");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 DEE_ASSERT(DeeObject_Check(pred));
 if (tp_self == &DeeList_Type) return DeeList_RIndexPred(self,findob,pred);
 if (tp_self == &DeeTuple_Type) return DeeTuple_RIndexPred(self,findob,pred);
 if (tp_self == &DeeSuper_Type) return DeeSequence_TRIndexPred(DeeSuper_TYPE(self),DeeSuper_SELF(self),findob,pred);
 if DEE_UNLIKELY((args = DeeTuple_Pack(2,findob,pred)) == NULL) return (Dee_size_t)-1;
 result_ob = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(!result_ob) {
  if DEE_LIKELY(DeeError_Catch(&DeeErrorType_AttributeError) ||
                DeeError_Catch(&DeeErrorType_NotImplemented)
                ) return DeeGeneric_TIterableRIndexPred(tp_self,self,findob,pred);
  return (Dee_size_t)-1;
 }
 error = DeeObject_Cast(Dee_size_t,result_ob,&result);
 Dee_DECREF(result_ob);
 if DEE_UNLIKELY(error != 0) return (Dee_size_t)-1;
 if DEE_UNLIKELY(result == (Dee_size_t)-1) DeeError_SET_STRING(&DeeErrorType_ValueError,"Invalid index");
 return result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeSequence_TCount(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *findob) {
 DeeObject *args,*result_ob; Dee_size_t result; int error;
 DeeString_NEW_STATIC(member_name,"count");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 if (tp_self == &DeeList_Type) return DeeList_Count(self,findob);
 if (tp_self == &DeeTuple_Type) return DeeTuple_Count(self,findob);
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (tp_self == &DeeUtf8String_Type) { if DEE_UNLIKELY((findob = DeeUtf8String_Cast(findob)) == NULL) return (Dee_size_t)-1; result = DeeUtf8String_Count(self,findob,0,DeeUtf8String_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (tp_self == &DeeWideString_Type) { if DEE_UNLIKELY((findob = DeeWideString_Cast(findob)) == NULL) return (Dee_size_t)-1; result = DeeWideString_Count(self,findob,0,DeeWideString_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (tp_self == &DeeUtf16String_Type) { if DEE_UNLIKELY((findob = DeeUtf16String_Cast(findob)) == NULL) return (Dee_size_t)-1; result = DeeUtf16String_Count(self,findob,0,DeeUtf16String_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (tp_self == &DeeUtf32String_Type) { if DEE_UNLIKELY((findob = DeeUtf32String_Cast(findob)) == NULL) return (Dee_size_t)-1; result = DeeUtf32String_Count(self,findob,0,DeeUtf32String_SIZE(self)); Dee_DECREF(findob); return result; }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
 if (tp_self == &DeeSuper_Type) return DeeSequence_TCount(DeeSuper_TYPE(self),DeeSuper_SELF(self),findob);
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,findob)) == NULL) return (Dee_size_t)-1;
 result_ob = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(!result_ob) {
  if DEE_LIKELY(DeeError_Catch(&DeeErrorType_AttributeError) ||
                DeeError_Catch(&DeeErrorType_NotImplemented)
                ) return DeeGeneric_TIterableCount(tp_self,self,findob);
  return (Dee_size_t)-1;
 }
 error = DeeObject_Cast(Dee_size_t,result_ob,&result);
 Dee_DECREF(result_ob);
 if DEE_UNLIKELY(error != 0) return (Dee_size_t)-1;
 if DEE_UNLIKELY(result == (Dee_size_t)-1) DeeError_SET_STRING(&DeeErrorType_ValueError,"Invalid count");
 return result;
}
DEE_A_RET_EXCEPT(-1) Dee_size_t DeeSequence_TCountPred(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *findob, DEE_A_INOUT DeeObject *pred) {
 DeeObject *args,*result_ob; Dee_size_t result; int error;
 DeeString_NEW_STATIC(member_name,"count");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(findob));
 DEE_ASSERT(DeeObject_Check(pred));
 if (tp_self == &DeeList_Type) return DeeList_CountPred(self,findob,pred);
 if (tp_self == &DeeTuple_Type) return DeeTuple_CountPred(self,findob,pred);
 if (tp_self == &DeeSuper_Type) return DeeSequence_TCountPred(DeeSuper_TYPE(self),DeeSuper_SELF(self),findob,pred);
 if DEE_UNLIKELY((args = DeeTuple_Pack(2,findob,pred)) == NULL) return (Dee_size_t)-1;
 result_ob = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(!result_ob) {
  if DEE_LIKELY(DeeError_Catch(&DeeErrorType_AttributeError) ||
                DeeError_Catch(&DeeErrorType_NotImplemented)
                ) return DeeGeneric_TIterableCountPred(tp_self,self,findob,pred);
  return (Dee_size_t)-1;
 }
 error = DeeObject_Cast(Dee_size_t,result_ob,&result);
 Dee_DECREF(result_ob);
 if DEE_UNLIKELY(error != 0) return (Dee_size_t)-1;
 if DEE_UNLIKELY(result == (Dee_size_t)-1) DeeError_SET_STRING(&DeeErrorType_ValueError,"Invalid count");
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeSequence_TLocate(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) {
 DeeObject *args,*result;
 DeeString_NEW_STATIC(member_name,"locate");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(pred));
 if (tp_self == &DeeList_Type) return DeeList_Locate(self,pred);
 if (tp_self == &DeeTuple_Type) DeeReturn_XNEWREF(DeeTuple_Locate(self,pred));
 if (tp_self == &DeeSuper_Type) return DeeSequence_TLocate(DeeSuper_TYPE(self),DeeSuper_SELF(self),pred);
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,pred)) == NULL) return NULL;
 result = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(DEE_UNLIKELY(!result) && DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  )) return DeeGeneric_TIterableLocate(tp_self,self,pred);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeSequence_TRLocate(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) {
 DeeObject *args,*result;
 DeeString_NEW_STATIC(member_name,"rlocate");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(pred));
 if (tp_self == &DeeList_Type) return DeeList_RLocate(self,pred);
 if (tp_self == &DeeTuple_Type) DeeReturn_XNEWREF(DeeTuple_RLocate(self,pred));
 if (tp_self == &DeeSuper_Type) return DeeSequence_TRLocate(DeeSuper_TYPE(self),DeeSuper_SELF(self),pred);
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,pred)) == NULL) return NULL;
 result = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(DEE_UNLIKELY(!result) && DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  )) return DeeGeneric_TIterableRLocate(tp_self,self,pred);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeSequence_TLocateAll(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) {
 DeeObject *args,*result;
 DeeString_NEW_STATIC(member_name,"locate_all");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(pred));
 if (tp_self == &DeeList_Type) return DeeList_LocateAll(self,pred);
 if (tp_self == &DeeTuple_Type) return DeeTuple_LocateAll(self,pred);
 if (tp_self == &DeeSuper_Type) return DeeSequence_TLocateAll(DeeSuper_TYPE(self),DeeSuper_SELF(self),pred);
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,pred)) == NULL) return NULL;
 result = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(DEE_UNLIKELY(!result) && DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  )) return DeeGeneric_TIterableLocateAll(tp_self,self,pred);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeSequence_TTransform(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) {
 DeeObject *args,*result;
 DeeString_NEW_STATIC(member_name,"transform");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(pred));
 if (tp_self == &DeeList_Type) return DeeList_Transform(self,pred);
 if (tp_self == &DeeTuple_Type) return DeeTuple_Transform(self,pred);
 if (tp_self == &DeeSuper_Type) return DeeSequence_TTransform(DeeSuper_TYPE(self),DeeSuper_SELF(self),pred);
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,pred)) == NULL) return NULL;
 result = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(DEE_UNLIKELY(!result) && DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  )) return DeeGeneric_TIterableTransform(tp_self,self,pred);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeSequence_TFront(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *result;
 DeeString_NEW_STATIC(member_name,"front");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeList_Type) return DeeList_Front(self);
 if (tp_self == &DeeTuple_Type) DeeReturn_NEWREF(DeeTuple_Front(self));
 if (tp_self == &DeeCell_Type) return DeeCell_Get(self);
 if (tp_self == &DeeSuper_Type) return DeeSequence_TFront(DeeSuper_TYPE(self),DeeSuper_SELF(self));
 result = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,Dee_EmptyTuple);
 if DEE_UNLIKELY(DEE_UNLIKELY(!result) && DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  )) return DeeGeneric_TIterableFront(tp_self,self);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeSequence_TBack(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *result;
 DeeString_NEW_STATIC(member_name,"back");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeList_Type) return DeeList_Back(self);
 if (tp_self == &DeeTuple_Type) DeeReturn_NEWREF(DeeTuple_Back(self));
 if (tp_self == &DeeCell_Type) return DeeCell_Get(self);
 if (tp_self == &DeeSuper_Type) return DeeSequence_TBack(DeeSuper_TYPE(self),DeeSuper_SELF(self));
 result = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,Dee_EmptyTuple);
 if DEE_UNLIKELY(DEE_UNLIKELY(!result) && DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  )) return DeeGeneric_TIterableBack(tp_self,self);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeSequence_TGet(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_ssize_t idx) {
 DeeObject *result,*idx_ob;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeList_Type) return DeeList_Get(self,idx);
 if (tp_self == &DeeTuple_Type) DeeReturn_XNEWREF(DeeTuple_Get(self,idx));
 if (tp_self == &DeeCell_Type) return DeeCell_Get(self);
 if (tp_self == &DeeSuper_Type) return DeeSequence_TGet(DeeSuper_TYPE(self),DeeSuper_SELF(self),idx);
 if DEE_UNLIKELY((idx_ob = DeeObject_New(Dee_ssize_t,idx)) == NULL) return NULL;
 result = DeeObject_TGetItem(tp_self,self,idx_ob);
 Dee_DECREF(idx_ob);
 if DEE_UNLIKELY(DEE_UNLIKELY(!result) && DEE_LIKELY(
//DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  )) return DeeGeneric_TIterableGet(tp_self,self,idx);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeSequence_TAt(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t idx) {
 DeeObject *args,*result,*idx_ob;
 DeeString_NEW_STATIC(member_name,"at");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeList_Type) return DeeList_At(self,idx);
 if (tp_self == &DeeTuple_Type) DeeReturn_XNEWREF(DeeTuple_At(self,idx));
 if (tp_self == &DeeCell_Type) {
  if (idx != 0) { DeeError_IndexError_T(&DeeCell_Type,idx); return NULL; }
  result = DeeCell_TryGet(self);
  if (!result) DeeError_IndexError_T(&DeeCell_Type,0);
  return result;
 }
 if (tp_self == &DeeSuper_Type) return DeeSequence_TAt(DeeSuper_TYPE(self),DeeSuper_SELF(self),idx);
 if DEE_UNLIKELY((idx_ob = DeeObject_New(Dee_size_t,idx)) == NULL) return NULL;
 args = DeeTuple_Pack(1,idx_ob);
 Dee_DECREF(idx_ob);
 if DEE_UNLIKELY(!args) return NULL;
 result = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(DEE_UNLIKELY(!result) && DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  )) return DeeGeneric_TIterableAt(tp_self,self,idx);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeSequence_TReversed(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *result;
 DeeString_NEW_STATIC(member_name,"reversed");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeList_Type) return DeeList_Reversed(self);
 if (tp_self == &DeeTuple_Type) return DeeTuple_Reversed(self);
 if (tp_self == &DeeSuper_Type) return DeeSequence_TReversed(DeeSuper_TYPE(self),DeeSuper_SELF(self));
 result = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,Dee_EmptyTuple);
 if DEE_UNLIKELY(DEE_UNLIKELY(!result) && DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  )) return DeeGeneric_TIterableReversed(tp_self,self);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeSequence_TSorted(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *result;
 DeeString_NEW_STATIC(member_name,"sorted");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeList_Type) return DeeList_Sorted(self);
 if (tp_self == &DeeTuple_Type) return DeeTuple_Sorted(self);
 if (tp_self == &DeeSuper_Type) return DeeSequence_TSorted(DeeSuper_TYPE(self),DeeSuper_SELF(self));
 result = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,Dee_EmptyTuple);
 if DEE_UNLIKELY(DEE_UNLIKELY(!result) && DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  )) return DeeGeneric_TIterableSorted(tp_self,self);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeSequence_TSortedPred(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *pred) {
 DeeObject *args,*result;
 DeeString_NEW_STATIC(member_name,"sorted");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(pred));
 if (tp_self == &DeeList_Type) return DeeList_SortedPred(self,pred);
 if (tp_self == &DeeTuple_Type) return DeeTuple_SortedPred(self,pred);
 if (tp_self == &DeeSuper_Type) return DeeSequence_TSortedPred(DeeSuper_TYPE(self),DeeSuper_SELF(self),pred);
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,pred)) == NULL) return NULL;
 result = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(DEE_UNLIKELY(!result) && DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  )) return DeeGeneric_TIterableSortedPred(tp_self,self,pred);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeSequence_TSegments(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN Dee_size_t n) {
 DeeObject *args,*result,*n_ob;
 DeeString_NEW_STATIC(member_name,"segments");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeList_Type) return DeeList_Segments(self,n);
 if (tp_self == &DeeTuple_Type) return DeeTuple_Segments(self,n);
 if (tp_self == &DeeSuper_Type) return DeeSequence_TSegments(DeeSuper_TYPE(self),DeeSuper_SELF(self),n);
 if DEE_UNLIKELY((n_ob = DeeObject_New(Dee_size_t,n)) == NULL) return NULL;
 args = DeeTuple_Pack(1,n_ob);
 Dee_DECREF(n_ob);
 if DEE_UNLIKELY(!args) return NULL;
 result = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(DEE_UNLIKELY(!result) && DEE_LIKELY(
  DeeError_Catch(&DeeErrorType_AttributeError) ||
  DeeError_Catch(&DeeErrorType_NotImplemented)
  )) return DeeGeneric_TIterableSegments(tp_self,self,n);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeSequence_TContains(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *elem) {
 DeeObject *args,*result_ob; int result;
 DeeString_NEW_STATIC(member_name,"contains");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeList_Type) return DeeList_Contains(self,elem);
 if (tp_self == &DeeTuple_Type) return DeeTuple_Contains(self,elem);
 if (tp_self == &DeeSuper_Type) return DeeSequence_TContains(DeeSuper_TYPE(self),DeeSuper_SELF(self),elem);
 if DEE_UNLIKELY((args = DeeTuple_Pack(1,elem)) == NULL) return -1;
 result_ob = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(!result_ob) {
  if DEE_LIKELY(DeeError_Catch(&DeeErrorType_AttributeError) ||
                DeeError_Catch(&DeeErrorType_NotImplemented)) {
   result = DeeGeneric_TIterableContains(tp_self,self,elem);
   // Call 'elem in self' if iterators are not implemented
   if DEE_UNLIKELY(result < 0 && DEE_LIKELY(DeeError_Catch(&DeeErrorType_NotImplemented)))
    result = DeeObject_TContains(tp_self,self,elem);
   return result;
  }
  return -1;
 }
 result = DeeObject_Bool(result_ob);
 Dee_DECREF(result_ob);
 return result;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeSequence_TContainsPred(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self,
 DEE_A_INOUT DeeObject *elem, DEE_A_INOUT DeeObject *pred) {
 DeeObject *args,*result_ob; int result;
 DeeString_NEW_STATIC(member_name,"contains");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(DeeObject_Check(pred));
 if (tp_self == &DeeList_Type) return DeeList_ContainsPred(self,elem,pred);
 if (tp_self == &DeeTuple_Type) return DeeTuple_ContainsPred(self,elem,pred);
 if (tp_self == &DeeSuper_Type) return DeeSequence_TContainsPred(DeeSuper_TYPE(self),DeeSuper_SELF(self),elem,pred);
 if DEE_UNLIKELY((args = DeeTuple_Pack(2,elem,pred)) == NULL) return -1;
 result_ob = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,args);
 Dee_DECREF(args);
 if DEE_UNLIKELY(!result_ob) {
  if DEE_LIKELY(DeeError_Catch(&DeeErrorType_AttributeError) ||
                DeeError_Catch(&DeeErrorType_NotImplemented)
                ) return DeeGeneric_TIterableContainsPred(tp_self,self,elem,pred);
  return -1;
 }
 result = DeeObject_Bool(result_ob);
 Dee_DECREF(result_ob);
 return result;
}


DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeVector_Any(
 DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(elemc) DeeObject **elemv) {
 DeeObject **end; int temp;
 end = elemv+elemc;
 while DEE_LIKELY(elemv != end) {
  if DEE_UNLIKELY((temp = DeeObject_Bool(*elemv)) != 0) return temp;
  ++elemv;
 }
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeVector_All(
 DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(elemc) DeeObject **elemv) {
 DeeObject **end; int temp;
 end = elemv+elemc;
 while DEE_LIKELY(elemv != end) {
  if DEE_UNLIKELY((temp = DeeObject_Bool(*elemv)) <= 0) return temp;
  ++elemv;
 }
 return 1;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeList_Any(
 DEE_A_INOUT DeeListObject *self) {
 Dee_size_t i; DeeObject *elem; int temp;
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  temp = DeeObject_Bool(elem);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(temp != 0) return temp; // Error, or true
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 return 0;
}
DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_FAIL(-1,0) int) _DeeList_All(
 DEE_A_INOUT DeeListObject *self) {
 Dee_size_t i; DeeObject *elem; int temp;
 DeeList_ACQUIRE(self);
 for (i = 0; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  Dee_INCREF(elem = DeeList_GET(self,i));
  DeeList_RELEASE(self);
  temp = DeeObject_Bool(elem);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(temp <= 0) return temp; // Error, or false
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 return 1;
}


DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeSequence_TAny(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *elem; int error;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeTuple_Type) return _DeeVector_Any(DeeTuple_SIZE(self),DeeTuple_ELEM(self));
 if (tp_self == &DeeList_Type) return _DeeList_Any((DeeListObject *)self);
 if (tp_self == &DeeCell_Type) {
  if ((self = DeeCell_TryGet(self)) == NULL) return 0;
  error = DeeObject_Bool(self);
  Dee_DECREF(self);
  return error;
 }
 if (tp_self == &DeeSuper_Type) return DeeSequence_TAny(DeeSuper_TYPE(self),DeeSuper_SELF(self));
 if DEE_UNLIKELY((self = DeeObject_TIterSelf(tp_self,self)) == NULL) return -1;
 while DEE_LIKELY((error = DeeObject_IterNextEx(self,&elem)) == 0) {
  error = DeeObject_Bool(elem);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(error != 0) {
   Dee_DECREF(self);
   return error; // error, or true
  }
 }
 Dee_DECREF(self);
 return DEE_UNLIKELY(error < 0) ? error : 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeSequence_TAll(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *elem; int error;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeTuple_Type) return _DeeVector_All(DeeTuple_SIZE(self),DeeTuple_ELEM(self));
 if (tp_self == &DeeList_Type) return _DeeList_Any((DeeListObject *)self);
 if (tp_self == &DeeCell_Type) {
  if ((self = DeeCell_TryGet(self)) == NULL) return 1;
  error = DeeObject_Bool(self);
  Dee_DECREF(self);
  return error;
 }
 if (tp_self == &DeeSuper_Type) return DeeSequence_TAll(DeeSuper_TYPE(self),DeeSuper_SELF(self));
 if DEE_UNLIKELY((self = DeeObject_TIterSelf(tp_self,self)) == NULL) return -1;
 while DEE_LIKELY((error = DeeObject_IterNextEx(self,&elem)) == 0) {
  error = DeeObject_Bool(elem);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(error <= 0) {
   Dee_DECREF(self);
   return error; // error, or false
  }
 }
 Dee_DECREF(self);
 return DEE_UNLIKELY(error < 0) ? error : 1;
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeSequence_TEmpty(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *elem,*result_ob; int result; Dee_size_t seqsize;
 DeeString_NEW_STATIC(member_name,"empty");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeList_Type) return DeeList_EMPTY(self);
 if (tp_self == &DeeTuple_Type) return DeeTuple_EMPTY(self);
 if (tp_self == &DeeDict_Type) return DeeDict_EMPTY(self);
 if (tp_self == &DeeSet_Type) return DeeSet_EMPTY(self);
 if (tp_self == &DeeCell_Type) return DeeCell_EMPTY(self);
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (tp_self == &DeeUtf8String_Type) return DeeUtf8String_EMPTY(self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (tp_self == &DeeWideString_Type) return DeeWideString_EMPTY(self);
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (tp_self == &DeeUtf16String_Type) return DeeUtf16String_EMPTY(self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (tp_self == &DeeUtf32String_Type) return DeeUtf32String_EMPTY(self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 if (DeeArrayType_Check(tp_self)) return DeeArrayType_EMPTY(tp_self);
#endif /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
 if (tp_self == (DeeTypeObject *)&DeeNone_Type) return 1; // Technically not, but none should be allowed to be anything!
 if (tp_self == &DeeSuper_Type) return DeeSequence_TEmpty(DeeSuper_TYPE(self),DeeSuper_SELF(self));
 if DEE_LIKELY((result_ob = DeeObject_TCallAttr(
  tp_self,self,(DeeObject *)&member_name,Dee_EmptyTuple)) != NULL) {
  result = DeeObject_Bool(result_ob);
  Dee_DECREF(result_ob);
  return result;
 }
 if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_AttributeError) &&
                 !DeeError_Catch(&DeeErrorType_NotImplemented)
                 ) return -1;
 if DEE_UNLIKELY((self = DeeObject_TIterSelf(tp_self,self)) == NULL) return -1;
 result = DeeObject_IterNextEx(self,&elem);
 Dee_DECREF(self);
 if DEE_UNLIKELY(result != 0) {
  if DEE_LIKELY(result > 0) return 1;
  if DEE_LIKELY(DeeError_Catch(&DeeErrorType_NotImplemented)) {
   // Try to implement with tp_seq_size
   if DEE_UNLIKELY((result = DeeObject_TSize(tp_self,self,&seqsize)) != 0) return -1;
   return seqsize == 0;
  }
  return result;
 }
 Dee_DECREF(elem);
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeSequence_TNonEmpty(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *elem,*result_ob; int result; Dee_size_t seqsize;
 DeeString_NEW_STATIC(member_name,"non_empty");
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeList_Type) return !DeeList_EMPTY(self);
 if (tp_self == &DeeTuple_Type) return !DeeTuple_EMPTY(self);
 if (tp_self == &DeeDict_Type) return !DeeDict_EMPTY(self);
 if (tp_self == &DeeSet_Type) return !DeeSet_EMPTY(self);
 if (tp_self == &DeeCell_Type) return !DeeCell_EMPTY(self);
#if DEE_CONFIG_HAVE_ENCODING_UTF8
 if (tp_self == &DeeUtf8String_Type) return !DeeUtf8String_EMPTY(self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_WIDE
 if (tp_self == &DeeWideString_Type) return !DeeWideString_EMPTY(self);
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
 if (tp_self == &DeeUtf16String_Type) return !DeeUtf16String_EMPTY(self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
 if (tp_self == &DeeUtf32String_Type) return !DeeUtf32String_EMPTY(self);
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 if (DeeArrayType_Check(tp_self)) return !DeeArrayType_EMPTY(self);
#endif /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
 if (tp_self == (DeeTypeObject *)&DeeNone_Type) return 1;
 if (tp_self == &DeeSuper_Type) return DeeSequence_TNonEmpty(DeeSuper_TYPE(self),DeeSuper_SELF(self));
 if DEE_LIKELY((result_ob = DeeObject_TCallAttr(tp_self,self,(DeeObject *)&member_name,Dee_EmptyTuple)) != NULL) {
  result = DeeObject_Bool(result_ob);
  Dee_DECREF(result_ob);
  return result;
 }
 if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_AttributeError) &&
                 !DeeError_Catch(&DeeErrorType_NotImplemented)
                 ) return -1;
 if DEE_UNLIKELY((self = DeeObject_TIterSelf(tp_self,self)) == NULL) return -1;
 result = DeeObject_IterNextEx(self,&elem);
 Dee_DECREF(self);
 if DEE_UNLIKELY(result != 0) {
  if DEE_LIKELY(result > 0) return 0;
  if DEE_LIKELY(DeeError_Catch(&DeeErrorType_NotImplemented)) {
   // Try to implement with tp_seq_size
   if DEE_UNLIKELY((result = DeeObject_TSize(tp_self,self,&seqsize)) != 0) return result;
   return seqsize != 0;
  }
  return result;
 }
 Dee_DECREF(elem);
 return 1;
}

DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeSequence_PEmpty(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) {
 DeeObject *elem,*iter,*result_ob; int result; Dee_size_t seqsize;
 DeeString_NEW_STATIC(member_name,"empty");
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 if (DeeArrayType_Check(tp_self)) return DeeArrayType_EMPTY(tp_self);
#endif /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
 if (tp_self == DeeObject_TYPE(none)) return 1; // Technically not, but none should be allowed to be anything!
 if DEE_LIKELY((result_ob = DeeObject_PCallAttr(
  tp_self,base_addr,(DeeObject *)&member_name,Dee_EmptyTuple)) != NULL) {
  result = DeeObject_Bool(result_ob);
  Dee_DECREF(result_ob);
  return result;
 }
 if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_AttributeError) &&
                 !DeeError_Catch(&DeeErrorType_NotImplemented)
                 ) return -1;
 if DEE_UNLIKELY((iter = DeeObject_PIterSelf(tp_self,base_addr)) == NULL) return -1;
 result = DeeObject_IterNextEx(iter,&elem);
 Dee_DECREF(iter);
 if DEE_UNLIKELY(result != 0) {
  if DEE_LIKELY(result > 0) return 1;
  if DEE_LIKELY(DeeError_Catch(&DeeErrorType_NotImplemented)) {
   // Try to implement with tp_seq_size
   if DEE_UNLIKELY((result = DeeObject_PSize(tp_self,base_addr,&seqsize)) != 0) return result;
   return seqsize == 0;
  }
  return result;
 }
 Dee_DECREF(elem);
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeSequence_PNonEmpty(
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *tp_self, DEE_A_INOUT void *base_addr) {
 DeeObject *elem,*iter,*result_ob; int result; Dee_size_t seqsize;
 DeeString_NEW_STATIC(member_name,"non_empty");
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeStructuredType_Check(tp_self));
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 if (DeeArrayType_Check(tp_self)) return !DeeArrayType_EMPTY(tp_self);
#endif /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
 if (tp_self == DeeObject_TYPE(none)) return 1;
 if DEE_LIKELY((result_ob = DeeObject_PCallAttr(
  tp_self,base_addr,(DeeObject *)&member_name,Dee_EmptyTuple)) != NULL) {
  result = DeeObject_Bool(result_ob);
  Dee_DECREF(result_ob);
  return result;
 }
 if DEE_UNLIKELY(!DeeError_Catch(&DeeErrorType_AttributeError) &&
                 !DeeError_Catch(&DeeErrorType_NotImplemented)
                 ) return -1;
 if DEE_UNLIKELY((iter = DeeObject_PIterSelf(tp_self,base_addr)) == NULL) return -1;
 result = DeeObject_IterNextEx(iter,&elem);
 Dee_DECREF(iter);
 if DEE_UNLIKELY(result != 0) {
  if DEE_LIKELY(result > 0) return 0;
  if DEE_LIKELY(DeeError_Catch(&DeeErrorType_NotImplemented)) {
   // Try to implement with tp_seq_size
   if DEE_UNLIKELY((result = DeeObject_PSize(tp_self,base_addr,&seqsize)) != 0) return result;
   return seqsize != 0;
  }
  return result;
 }
 Dee_DECREF(elem);
 return 1;
}


#if DEE_CONFIG_HAVE_ENCODING_WIDE
#define ENC Wide
#include "sequence.sum_strings.inl"
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
#define ENC Utf8
#include "sequence.sum_strings.inl"
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
#define ENC Utf16
#include "sequence.sum_strings.inl"
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
#define ENC Utf32
#include "sequence.sum_strings.inl"
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */

#define UNSIGNED
#define SIZEOF 1
#include "sequence.sum_integrals.inl"
#define SIZEOF 2
#include "sequence.sum_integrals.inl"
#define SIZEOF 4
#include "sequence.sum_integrals.inl"
#define SIZEOF 8
#include "sequence.sum_integrals.inl"
#undef UNSIGNED
#define SIZEOF 1
#include "sequence.sum_integrals.inl"
#define SIZEOF 2
#include "sequence.sum_integrals.inl"
#define SIZEOF 4
#include "sequence.sum_integrals.inl"
#define SIZEOF 8
#include "sequence.sum_integrals.inl"
#ifdef DEE_TYPES_FLOATID_FLOAT
#define FLOATID DEE_TYPES_FLOATID_FLOAT
#include "sequence.sum_integrals.inl"
#endif
#ifdef DEE_TYPES_FLOATID_DOUBLE
#define FLOATID DEE_TYPES_FLOATID_DOUBLE
#include "sequence.sum_integrals.inl"
#endif
#ifdef DEE_TYPES_FLOATID_LDOUBLE
#define FLOATID DEE_TYPES_FLOATID_LDOUBLE
#include "sequence.sum_integrals.inl"
#endif



DEE_A_RET_EXCEPT_REF DeeObject *_DeeVectorSum(
 DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(elemc) DeeObject **elemv) {
 DeeObject *new_result,*result,**end; DeeTypeObject *tp_first;
 if DEE_UNLIKELY(!elemc) DeeReturn_None;
 result = *elemv;
 if DEE_UNLIKELY(!--elemc) DeeReturn_NEWREF(result);
 ++elemv; tp_first = Dee_TYPE(result);
 if DEE_LIKELY(DeeType_HAS_SLOT(tp_first,tp_marshal) && 
               DeeUUID_CHECK_INTERNAL(&DeeType_GET_SLOT(tp_first,tp_marshal)->tp_uuid)) {
  switch (DeeUUID_GET_INTERNAL(&DeeType_GET_SLOT(tp_first,tp_marshal)->tp_uuid)) {
#if DEE_CONFIG_HAVE_ENCODING_WIDE
   case DEE_MARSHALTYPEID_Wide_string: return _DeeVectorSum_WideString((DeeWideStringObject *)result,elemc,elemv); break;
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
   case DEE_MARSHALTYPEID_Utf8_string: return _DeeVectorSum_Utf8String((DeeUtf8StringObject *)result,elemc,elemv); break;
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
   case DEE_MARSHALTYPEID_Utf16_string: return _DeeVectorSum_Utf16String((DeeUtf16StringObject *)result,elemc,elemv); break;
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
   case DEE_MARSHALTYPEID_Utf32_string: return _DeeVectorSum_Utf32String((DeeUtf32StringObject *)result,elemc,elemv); break;
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
   case DEE_MARSHALTYPEID(int8): return _DeeVectorSum_Int8(DeeInt8_VALUE(result),elemc,elemv); break;
   case DEE_MARSHALTYPEID(int16): return _DeeVectorSum_Int16(DeeInt16_VALUE(result),elemc,elemv); break;
   case DEE_MARSHALTYPEID(int32): return _DeeVectorSum_Int32(DeeInt32_VALUE(result),elemc,elemv); break;
   case DEE_MARSHALTYPEID(int64): return _DeeVectorSum_Int64(DeeInt64_VALUE(result),elemc,elemv); break;
   case DEE_MARSHALTYPEID(uint8): return _DeeVectorSum_UInt8(DeeUInt8_VALUE(result),elemc,elemv); break;
   case DEE_MARSHALTYPEID(uint16): return _DeeVectorSum_UInt16(DeeUInt16_VALUE(result),elemc,elemv); break;
   case DEE_MARSHALTYPEID(uint32): return _DeeVectorSum_UInt32(DeeUInt32_VALUE(result),elemc,elemv); break;
   case DEE_MARSHALTYPEID(uint64): return _DeeVectorSum_UInt64(DeeUInt64_VALUE(result),elemc,elemv); break;
   case DEE_MARSHALTYPEID(atomic_int8): return _DeeVectorSum_Int8((Dee_int8_t)DeeAtomic8_Load(DeeInt8_VALUE(result),memory_order_seq_cst),elemc,elemv); break;
   case DEE_MARSHALTYPEID(atomic_int16): return _DeeVectorSum_Int16((Dee_int16_t)DeeAtomic16_Load(DeeInt16_VALUE(result),memory_order_seq_cst),elemc,elemv); break;
   case DEE_MARSHALTYPEID(atomic_int32): return _DeeVectorSum_Int32((Dee_int32_t)DeeAtomic32_Load(DeeInt32_VALUE(result),memory_order_seq_cst),elemc,elemv); break;
   case DEE_MARSHALTYPEID(atomic_int64): return _DeeVectorSum_Int64((Dee_int64_t)DeeAtomic64_Load(DeeInt64_VALUE(result),memory_order_seq_cst),elemc,elemv); break;
   case DEE_MARSHALTYPEID(atomic_uint8): return _DeeVectorSum_UInt8((Dee_uint8_t)DeeAtomic8_Load(DeeUInt8_VALUE(result),memory_order_seq_cst),elemc,elemv); break;
   case DEE_MARSHALTYPEID(atomic_uint16): return _DeeVectorSum_UInt16((Dee_uint16_t)DeeAtomic16_Load(DeeUInt16_VALUE(result),memory_order_seq_cst),elemc,elemv); break;
   case DEE_MARSHALTYPEID(atomic_uint32): return _DeeVectorSum_UInt32((Dee_uint32_t)DeeAtomic32_Load(DeeUInt32_VALUE(result),memory_order_seq_cst),elemc,elemv); break;
   case DEE_MARSHALTYPEID(atomic_uint64): return _DeeVectorSum_UInt64((Dee_uint64_t)DeeAtomic64_Load(DeeUInt64_VALUE(result),memory_order_seq_cst),elemc,elemv); break;
   case DEE_MARSHALTYPEID_float4: case DEE_MARSHALTYPEID_float8:
   case DEE_MARSHALTYPEID_float12: case DEE_MARSHALTYPEID_float16: {
    if (tp_first == (DeeTypeObject *)&DeeDouble_Type)
     return _DeeVectorSum_double(DeeDouble_VALUE(result),elemc,elemv);
    else if (tp_first == (DeeTypeObject *)&DeeFloat_Type)
     return _DeeVectorSum_float(DeeFloat_VALUE(result),elemc,elemv);
    else {
     DEE_ASSERTF(tp_first == (DeeTypeObject *)&DeeLDouble_Type,
                 "Unknown floating point type %q uses internal floating uuid %K",
                 DeeType_NAME(tp_first),
                 DeeUUID_ToString(&DeeType_GET_SLOT(tp_first,tp_marshal)->tp_uuid));
     return _DeeVectorSum_ldouble(DeeLDouble_VALUE(result),elemc,elemv);
    }
   } break;
   default: break;
  }
 }
 end = elemv+elemc;
 if DEE_UNLIKELY((result = DeeObject_Add(result,*elemv)) == NULL) return NULL;
 ++elemv;
 while DEE_LIKELY(elemv != end) {
  new_result = DeeObject_Add(result,*elemv);
  ++elemv;
  Dee_DECREF(result);
  if DEE_UNLIKELY(!new_result) return NULL;
  if DEE_UNLIKELY(elemv == end) return new_result;
  result = DeeObject_Add(new_result,*elemv);
  ++elemv;
  Dee_DECREF(new_result);
  if DEE_UNLIKELY(!result) return NULL;
 }
 return result;
}

DEE_A_RET_EXCEPT_REF DeeObject *_DeeListSum(
 DEE_A_INOUT DeeListObject *self) {
 DeeObject *new_result,*result,*elem;
 Dee_size_t i; DeeTypeObject *tp_first;
 DeeList_ACQUIRE(self);
 if DEE_UNLIKELY(!DeeList_SIZE(self)) { DeeList_RELEASE(self); DeeReturn_None; }
 Dee_INCREF(elem = DeeList_GET(self,0));
 if DEE_UNLIKELY(DeeList_SIZE(self) == 1) { DeeList_RELEASE(self); return elem; }
 tp_first = Dee_TYPE(elem);
 if DEE_LIKELY(DeeType_HAS_SLOT(tp_first,tp_marshal) && 
               DeeUUID_CHECK_INTERNAL(&DeeType_GET_SLOT(tp_first,tp_marshal)->tp_uuid)) {
  switch (DeeUUID_GET_INTERNAL(&DeeType_GET_SLOT(tp_first,tp_marshal)->tp_uuid)) {
#if DEE_CONFIG_HAVE_ENCODING_WIDE
   case DEE_MARSHALTYPEID_Wide_string:
    DeeList_RELEASE(self);
    result = _DeeListSum_WideString((DeeWideStringObject *)elem,self);
    break;
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
   case DEE_MARSHALTYPEID_Utf8_string:
    DeeList_RELEASE(self);
    result = _DeeListSum_Utf8String((DeeUtf8StringObject *)elem,self);
    break;
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
   case DEE_MARSHALTYPEID_Utf16_string:
    DeeList_RELEASE(self);
    result = _DeeListSum_Utf16String((DeeUtf16StringObject *)elem,self);
    break;
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
   case DEE_MARSHALTYPEID_Utf32_string:
    DeeList_RELEASE(self);
    result = _DeeListSum_Utf32String((DeeUtf32StringObject *)elem,self);
    break;
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
   case DEE_MARSHALTYPEID(int8): result = _DeeListSum_Int8(DeeInt8_VALUE(elem),self); break;
   case DEE_MARSHALTYPEID(int16): result = _DeeListSum_Int16(DeeInt16_VALUE(elem),self); break;
   case DEE_MARSHALTYPEID(int32): result = _DeeListSum_Int32(DeeInt32_VALUE(elem),self); break;
   case DEE_MARSHALTYPEID(int64): result = _DeeListSum_Int64(DeeInt64_VALUE(elem),self); break;
   case DEE_MARSHALTYPEID(uint8): result = _DeeListSum_UInt8(DeeUInt8_VALUE(elem),self); break;
   case DEE_MARSHALTYPEID(uint16): result = _DeeListSum_UInt16(DeeUInt16_VALUE(elem),self); break;
   case DEE_MARSHALTYPEID(uint32): result = _DeeListSum_UInt32(DeeUInt32_VALUE(elem),self); break;
   case DEE_MARSHALTYPEID(uint64): result = _DeeListSum_UInt64(DeeUInt64_VALUE(elem),self); break;
   case DEE_MARSHALTYPEID(atomic_int8): result = _DeeListSum_Int8((Dee_int8_t)DeeAtomic8_Load(DeeInt8_VALUE(elem),memory_order_seq_cst),self); break;
   case DEE_MARSHALTYPEID(atomic_int16): result = _DeeListSum_Int16((Dee_int16_t)DeeAtomic16_Load(DeeInt16_VALUE(elem),memory_order_seq_cst),self); break;
   case DEE_MARSHALTYPEID(atomic_int32): result = _DeeListSum_Int32((Dee_int32_t)DeeAtomic32_Load(DeeInt32_VALUE(elem),memory_order_seq_cst),self); break;
   case DEE_MARSHALTYPEID(atomic_int64): result = _DeeListSum_Int64((Dee_int64_t)DeeAtomic64_Load(DeeInt64_VALUE(elem),memory_order_seq_cst),self); break;
   case DEE_MARSHALTYPEID(atomic_uint8): result = _DeeListSum_UInt8((Dee_uint8_t)DeeAtomic8_Load(DeeUInt8_VALUE(elem),memory_order_seq_cst),self); break;
   case DEE_MARSHALTYPEID(atomic_uint16): result = _DeeListSum_UInt16((Dee_uint16_t)DeeAtomic16_Load(DeeUInt16_VALUE(elem),memory_order_seq_cst),self); break;
   case DEE_MARSHALTYPEID(atomic_uint32): result = _DeeListSum_UInt32((Dee_uint32_t)DeeAtomic32_Load(DeeUInt32_VALUE(elem),memory_order_seq_cst),self); break;
   case DEE_MARSHALTYPEID(atomic_uint64): result = _DeeListSum_UInt64((Dee_uint64_t)DeeAtomic64_Load(DeeUInt64_VALUE(elem),memory_order_seq_cst),self); break;
   case DEE_MARSHALTYPEID_float4: case DEE_MARSHALTYPEID_float8:
   case DEE_MARSHALTYPEID_float12: case DEE_MARSHALTYPEID_float16: {
    if (tp_first == (DeeTypeObject *)&DeeDouble_Type)
     result = _DeeListSum_double(DeeDouble_VALUE(elem),self);
    else if (tp_first == (DeeTypeObject *)&DeeFloat_Type)
     result = _DeeListSum_float(DeeFloat_VALUE(elem),self);
    else {
     DEE_ASSERTF(tp_first == (DeeTypeObject *)&DeeLDouble_Type,
                 "Unknown floating point type %q uses internal floating uuid %K",
                 DeeType_NAME(tp_first),
                 DeeUUID_ToString(&DeeType_GET_SLOT(tp_first,tp_marshal)->tp_uuid));
     result = _DeeListSum_ldouble(DeeLDouble_VALUE(elem),self);
    }
   } break;
   default: goto fallback;
  }
  Dee_DECREF(elem);
  return result;
 }
fallback:
 Dee_INHERIT_REF(result,elem);
 for (i = 1; DEE_LIKELY(i < DeeList_SIZE(self)); ++i) {
  elem = DeeList_GET(self,i);
  DeeList_RELEASE(self);
  new_result = DeeObject_Add(result,elem);
  Dee_DECREF(elem);
  Dee_DECREF(result);
  if DEE_UNLIKELY(!new_result) return NULL;
  Dee_INHERIT_REF(result,new_result);
  DeeList_ACQUIRE(self);
 }
 DeeList_RELEASE(self);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeObject *_DeeIteratorSum(
 DEE_A_INOUT DeeObject *self) {
 DeeObject *result,*elem,*new_result; int error;
 DeeTypeObject *tp_first;
 if ((error = DeeObject_IterNextEx(self,&elem)) != 0) {
  if (error < 0) return NULL;
  DeeReturn_None; // '[]+...' --> 'none'
 }
 tp_first = Dee_TYPE(elem);
 if DEE_LIKELY(DeeType_HAS_SLOT(tp_first,tp_marshal) && 
               DeeUUID_CHECK_INTERNAL(&DeeType_GET_SLOT(tp_first,tp_marshal)->tp_uuid)) {
  switch (DeeUUID_GET_INTERNAL(&DeeType_GET_SLOT(tp_first,tp_marshal)->tp_uuid)) {
#if DEE_CONFIG_HAVE_ENCODING_WIDE
   case DEE_MARSHALTYPEID_Wide_string: result = _DeeIteratorSum_WideString((DeeWideStringObject *)elem,self); break;
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */
#if DEE_CONFIG_HAVE_ENCODING_UTF8
   case DEE_MARSHALTYPEID_Utf8_string: result = _DeeIteratorSum_Utf8String((DeeUtf8StringObject *)elem,self); break;
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */
#if DEE_CONFIG_HAVE_ENCODING_UTF16
   case DEE_MARSHALTYPEID_Utf16_string: result = _DeeIteratorSum_Utf16String((DeeUtf16StringObject *)elem,self); break;
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */
#if DEE_CONFIG_HAVE_ENCODING_UTF32
   case DEE_MARSHALTYPEID_Utf32_string: result = _DeeIteratorSum_Utf32String((DeeUtf32StringObject *)elem,self); break;
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */
   case DEE_MARSHALTYPEID_int8: result = _DeeIteratorSum_Int8(DeeInt8_VALUE(elem),self); break;
   case DEE_MARSHALTYPEID_int16: result = _DeeIteratorSum_Int16(DeeInt16_VALUE(elem),self); break;
   case DEE_MARSHALTYPEID_int32: result = _DeeIteratorSum_Int32(DeeInt32_VALUE(elem),self); break;
   case DEE_MARSHALTYPEID_int64: result = _DeeIteratorSum_Int64(DeeInt64_VALUE(elem),self); break;
   case DEE_MARSHALTYPEID_uint8: result = _DeeIteratorSum_UInt8(DeeUInt8_VALUE(elem),self); break;
   case DEE_MARSHALTYPEID_uint16: result = _DeeIteratorSum_UInt16(DeeUInt16_VALUE(elem),self); break;
   case DEE_MARSHALTYPEID_uint32: result = _DeeIteratorSum_UInt32(DeeUInt32_VALUE(elem),self); break;
   case DEE_MARSHALTYPEID_uint64: result = _DeeIteratorSum_UInt64(DeeUInt64_VALUE(elem),self); break;
   case DEE_MARSHALTYPEID_atomic_int8: result = _DeeIteratorSum_Int8((Dee_int8_t)DeeAtomic8_Load(DeeInt8_VALUE(elem),memory_order_seq_cst),self); break;
   case DEE_MARSHALTYPEID_atomic_int16: result = _DeeIteratorSum_Int16((Dee_int16_t)DeeAtomic16_Load(DeeInt16_VALUE(elem),memory_order_seq_cst),self); break;
   case DEE_MARSHALTYPEID_atomic_int32: result = _DeeIteratorSum_Int32((Dee_int32_t)DeeAtomic32_Load(DeeInt32_VALUE(elem),memory_order_seq_cst),self); break;
   case DEE_MARSHALTYPEID_atomic_int64: result = _DeeIteratorSum_Int64((Dee_int64_t)DeeAtomic64_Load(DeeInt64_VALUE(elem),memory_order_seq_cst),self); break;
   case DEE_MARSHALTYPEID_atomic_uint8: result = _DeeIteratorSum_UInt8((Dee_uint8_t)DeeAtomic8_Load(DeeUInt8_VALUE(elem),memory_order_seq_cst),self); break;
   case DEE_MARSHALTYPEID_atomic_uint16: result = _DeeIteratorSum_UInt16((Dee_uint16_t)DeeAtomic16_Load(DeeUInt16_VALUE(elem),memory_order_seq_cst),self); break;
   case DEE_MARSHALTYPEID_atomic_uint32: result = _DeeIteratorSum_UInt32((Dee_uint32_t)DeeAtomic32_Load(DeeUInt32_VALUE(elem),memory_order_seq_cst),self); break;
   case DEE_MARSHALTYPEID_atomic_uint64: result = _DeeIteratorSum_UInt64((Dee_uint64_t)DeeAtomic64_Load(DeeUInt64_VALUE(elem),memory_order_seq_cst),self); break;
   case DEE_MARSHALTYPEID_float4: case DEE_MARSHALTYPEID_float8:
   case DEE_MARSHALTYPEID_float12: case DEE_MARSHALTYPEID_float16: {
    if (tp_first == (DeeTypeObject *)&DeeDouble_Type)
     result = _DeeIteratorSum_double(DeeDouble_VALUE(elem),self);
    else if (tp_first == (DeeTypeObject *)&DeeFloat_Type)
     result = _DeeIteratorSum_float(DeeFloat_VALUE(elem),self);
    else {
     DEE_ASSERTF(tp_first == (DeeTypeObject *)&DeeLDouble_Type,
                 "Unknown floating point type %q uses internal floating uuid %K",
                 DeeType_NAME(tp_first),
                 DeeUUID_ToString(&DeeType_GET_SLOT(tp_first,tp_marshal)->tp_uuid));
     result = _DeeIteratorSum_ldouble(DeeLDouble_VALUE(elem),self);
    }
   } break;

   default: goto fallback;
  }
  Dee_DECREF(elem);
  return result;
 }
fallback:
 Dee_INHERIT_REF(result,elem);
 while DEE_LIKELY((error = DeeObject_IterNextEx(self,&elem)) == 0) {
  new_result = DeeObject_Add(result,elem);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(!new_result) { Dee_DECREF(result); return NULL; }
  Dee_INHERIT_REF(result,new_result);
 }
 return result;
}



DEE_A_RET_EXCEPT_REF DeeObject *DeeSequence_TSum(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (tp_self == &DeeTuple_Type) return _DeeVectorSum(DeeTuple_SIZE(self),DeeTuple_ELEM(self));
 if (tp_self == &DeeList_Type) return _DeeListSum((DeeListObject *)self);
 if (tp_self == &DeeCell_Type) return (result = DeeCell_TryGet(self)) != NULL ? result : DeeNone_New();
 if (tp_self == &DeeSuper_Type) return DeeSequence_TSum(DeeSuper_TYPE(self),DeeSuper_SELF(self));
 if DEE_UNLIKELY((self = DeeObject_TIterSelf(tp_self,self)) == NULL) return NULL;
 result = _DeeIteratorSum(self);
 Dee_DECREF(self);
 return result;
}


#ifndef __INTELLISENSE__
#define DO_MIN
#include "sequence.minmax.inl"
#include "sequence.minmax.inl"
#endif


DEE_DECL_END

#endif /* !GUARD_DEEMON_OBJECT_C */
