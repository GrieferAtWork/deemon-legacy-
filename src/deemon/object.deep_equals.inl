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
#ifdef __INTELLISENSE__
#include "object.c"
DEE_DECL_BEGIN
#endif

DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeObject_TDeepEquals(
 DEE_A_IN DeeTypeObject const *tp_lhs, DEE_A_INOUT DeeObject *lhs,
 DEE_A_INOUT DeeObject *rhs) {
 int temp,likely_equal; DeeObject *lhs_iterator,*rhs_iterator;
 DeeObject *lhs_elem,*rhs_elem;
 DEE_ASSERT(DeeObject_Check(lhs));
 DEE_ASSERT(DeeObject_Check(tp_lhs) && DeeType_Check(tp_lhs));
 DEE_ASSERT(DeeObject_InstanceOf(lhs,tp_lhs));
 if DEE_LIKELY(Dee_TYPE(rhs) != tp_lhs) return DEE_OBJECT_DEEPEQUALS_FALSE; // Different types
 if DEE_UNLIKELY(lhs == rhs) return DEE_OBJECT_DEEPEQUALS_TRUE; // Simple case: it's the same object
 if (DeeStructuredType_Check(tp_lhs)) {
  // For structured types, we can simple compare the memory
  return memcmp(DeeStructured_DATA(lhs),DeeStructured_DATA(rhs),
                DeeType_GET_SLOT(tp_lhs,tp_p_instance_size)) == 0
#if DEE_OBJECT_DEEPEQUALS_TRUE != 1 || DEE_OBJECT_DEEPEQUALS_FALSE != 0
                ? DEE_OBJECT_DEEPEQUALS_TRUE : DEE_OBJECT_DEEPEQUALS_FALSE
#endif
   ;
 }
 if DEE_UNLIKELY((temp = DeeObject_TCompareEq(tp_lhs,lhs,rhs)) <= 0) return temp;
 if (DeeType_HAS_SLOT(tp_lhs,tp_marshal) &&
     DeeUUID_CHECK_INTERNAL(&DeeType_GET_SLOT(tp_lhs,tp_marshal)->tp_uuid)) {
  // (ab)use the marshal to detect some intrinsic types where we already know that they're equal.
  switch (DeeUUID_GET_INTERNAL(&DeeType_GET_SLOT(tp_lhs,tp_marshal)->tp_uuid)) {
   case DEE_MARSHALTYPEID(object):
   case DEE_MARSHALTYPEID(int8):
   case DEE_MARSHALTYPEID(int16):
   case DEE_MARSHALTYPEID(int32):
   case DEE_MARSHALTYPEID(int64):
   case DEE_MARSHALTYPEID(uint8):
   case DEE_MARSHALTYPEID(uint16):
   case DEE_MARSHALTYPEID(uint32):
   case DEE_MARSHALTYPEID(uint64):
   case DEE_MARSHALTYPEID(atomic_int8):
   case DEE_MARSHALTYPEID(atomic_int16):
   case DEE_MARSHALTYPEID(atomic_int32):
   case DEE_MARSHALTYPEID(atomic_int64):
   case DEE_MARSHALTYPEID(atomic_uint8):
   case DEE_MARSHALTYPEID(atomic_uint16):
   case DEE_MARSHALTYPEID(atomic_uint32):
   case DEE_MARSHALTYPEID(atomic_uint64):
   case DEE_MARSHALTYPEID(int8_range):
   case DEE_MARSHALTYPEID(int16_range):
   case DEE_MARSHALTYPEID(int32_range):
   case DEE_MARSHALTYPEID(int64_range):
   case DEE_MARSHALTYPEID(uint8_range):
   case DEE_MARSHALTYPEID(uint16_range):
   case DEE_MARSHALTYPEID(uint32_range):
   case DEE_MARSHALTYPEID(uint64_range):
   case DEE_MARSHALTYPEID(float4):
   case DEE_MARSHALTYPEID(float8):
   case DEE_MARSHALTYPEID(float12):
   case DEE_MARSHALTYPEID(float16):
   case DEE_MARSHALTYPEID(Wide_string):
   case DEE_MARSHALTYPEID(Utf8_string):
   case DEE_MARSHALTYPEID(Utf16_string):
   case DEE_MARSHALTYPEID(Utf32_string):
   case DEE_MARSHALTYPEID(Wide_string_iterator):
   case DEE_MARSHALTYPEID(Utf8_string_iterator):
   case DEE_MARSHALTYPEID(Utf16_string_iterator):
   case DEE_MARSHALTYPEID(Utf32_string_iterator):
   case DEE_MARSHALTYPEID(code_debuginfo):
   case DEE_MARSHALTYPEID(random):
   case DEE_MARSHALTYPEID(file):
   case DEE_MARSHALTYPEID(file_writer):
   case DEE_MARSHALTYPEID(file_reader):
   case DEE_MARSHALTYPEID(time):
   case DEE_MARSHALTYPEID(traceback):
   case DEE_MARSHALTYPEID(traceback_iterator):
   case DEE_MARSHALTYPEID(traceback_entry):
    return DEE_OBJECT_DEEPEQUALS_TRUE;
   case DEE_MARSHALTYPEID(cell): {
    lhs = DeeCell_TryGet(lhs);
    rhs = DeeCell_TryGet(rhs);
    if (!lhs) { if (rhs) { Dee_DECREF(rhs); return DEE_OBJECT_DEEPEQUALS_FALSE; } }
    if (!rhs) { Dee_DECREF(lhs); return DEE_OBJECT_DEEPEQUALS_FALSE; }
    temp = DeeObject_DeepEquals(lhs,rhs);
    Dee_DECREF(rhs);
    Dee_DECREF(lhs);
    return temp;
   } break;
   case DEE_MARSHALTYPEID(tuple): {
    DeeObject **lhs_iter,**lhs_end,**rhs_iter;
    if (DeeTuple_SIZE(lhs) != DeeTuple_SIZE(rhs)) return 0;
    lhs_end = (lhs_iter = DeeTuple_ELEM(lhs))+DeeTuple_SIZE(lhs);
    rhs_iter = DeeTuple_ELEM(rhs);
    likely_equal = 0;
    while (lhs_iter != lhs_end) {
     if ((temp = DeeObject_DeepEquals(*lhs_iter,*rhs_iter)) != DEE_OBJECT_DEEPEQUALS_TRUE) {
      if (temp <= 0) return temp;
      if (temp == DEE_OBJECT_DEEPEQUALS_LIKELY) likely_equal = 1; // Likely-equal was signaled (inherit that fact)
     }
     ++lhs_iter,++rhs_iter;
    }
    return likely_equal ? DEE_OBJECT_DEEPEQUALS_LIKELY : DEE_OBJECT_DEEPEQUALS_TRUE;
   } break;
   case DEE_MARSHALTYPEID(list): {
    Dee_size_t i,lhs_size; DeeObject *lhs_elem,*rhs_elem;
    likely_equal = 0;
    DeeList_ACQUIRE(lhs);
    for (i = 0; i < (lhs_size = DeeList_SIZE(lhs)); ++i) {
     Dee_INCREF(lhs_elem = DeeList_GET(lhs,i));
     DeeList_RELEASE(lhs);
     DeeList_ACQUIRE(rhs);
     if (lhs_size != DeeList_SIZE(rhs)) {
      DeeList_RELEASE(rhs);
      Dee_DECREF(lhs_elem);
      return DEE_OBJECT_DEEPEQUALS_FALSE;
     }
     Dee_INCREF(rhs_elem = DeeList_GET(lhs,i));
     DeeList_RELEASE(rhs);
     temp = DeeObject_DeepEquals(lhs_elem,rhs_elem);
     Dee_DECREF(rhs_elem);
     Dee_DECREF(lhs_elem);
     if (temp <= 0) return temp;
     if (temp == DEE_OBJECT_DEEPEQUALS_LIKELY) likely_equal = 1; // Likely-equal was signaled (inherit that fact)
     DeeList_ACQUIRE(lhs);
    }
    DeeList_RELEASE(lhs);
    return likely_equal ? DEE_OBJECT_DEEPEQUALS_LIKELY : DEE_OBJECT_DEEPEQUALS_TRUE;
   } break;
   //case DEE_MARSHALTYPEID(set):
   //case DEE_MARSHALTYPEID(dict):
   // todo: Special optimizations (no need to use a real iterator for these...)
   default: break;
  }
 }
 // 'tp_cmp_eq' says they're equal. - but so is '(pack true) == (pack 42)' (because 42 is != 0)
 if DEE_UNLIKELY((lhs_iterator = DeeObject_TIterSelf(tp_lhs,lhs)) == NULL) {
  // Not a sequence --> So as far as I can tell (and am supposed to tell) they're equal.
  // Note though, that this isn't guarantied, as the actual implementation of tp_cmp_eq
  // has thus far been the only thing that promised equality.
  // -> And there isn't really any rule that would prevent tp_cmp_eq from lying...
  // -> So we simply return 'DEE_OBJECT_DEEPEQUALS_LIKELY',
  //    indicating that the object are ~likely~ equal.
  return DeeError_Catch(&DeeErrorType_NotImplemented) ? DEE_OBJECT_DEEPEQUALS_LIKELY : -1;
 }
 // If this fails, we already know it's a real error (because we already generated an iterator of the same type)
 if DEE_UNLIKELY((rhs_iterator = DeeObject_IterSelf(rhs)) == NULL) {
/*err_lhs_iterator:*/ Dee_DECREF(lhs_iterator); return -1;
 }
 if DEE_UNLIKELY(Dee_TYPE(lhs_iterator) != Dee_TYPE(rhs_iterator)) {
  // There is type that does this, but it would be a clear indicator!
  Dee_DECREF(rhs_iterator);
  Dee_DECREF(lhs_iterator);
  return 0;
 }
 likely_equal = 0;
 while (1) {
  if DEE_UNLIKELY((temp = DeeObject_IterNextEx(lhs_iterator,&lhs_elem)) < 0) break;
  if DEE_UNLIKELY(temp > 0) {
   // End of left iterator (make sure the right one is done, too)
   if DEE_UNLIKELY((temp = DeeObject_IterNextEx(rhs_iterator,&rhs_elem)) < 0) break;
   if DEE_UNLIKELY(temp > 0) break; // Yes, he is (done, that is...)
   Dee_DECREF(rhs_elem);
   // no, he isn't (stop, and return false)
   DEE_ASSERT(temp == 0);
   break;
  }
  if DEE_UNLIKELY((temp = DeeObject_IterNextEx(rhs_iterator,&rhs_elem)) != 0) {
   Dee_DECREF(lhs_elem);
   if DEE_UNLIKELY(temp > 0) temp = 0; // turn end-of-iterator into not-equal
   break; // error, or premature end in rhs_iterator
  }
  temp = DeeObject_DeepEquals(lhs_elem,rhs_elem);
  Dee_DECREF(rhs_elem);
  Dee_DECREF(lhs_elem);
#if DEE_OBJECT_DEEPEQUALS_FALSE == 0
  if (temp <= 0) break;
#else /* non-equal elements, or error */
  if (temp < 0 || temp == DEE_OBJECT_DEEPEQUALS_FALSE) break;
#endif
  if (temp == DEE_OBJECT_DEEPEQUALS_LIKELY)
   likely_equal = 1; // Likely-equal was signaled (inherit that fact)
 }
 Dee_DECREF(rhs_iterator);
 Dee_DECREF(lhs_iterator);
#if DEE_OBJECT_DEEPEQUALS_FALSE == 0
 return temp <= 0 ? temp : likely_equal
#else
 return (temp < 0 || temp == DEE_OBJECT_DEEPEQUALS_FALSE) ? temp : likely_equal
#endif
  ? DEE_OBJECT_DEEPEQUALS_LIKELY
  : DEE_OBJECT_DEEPEQUALS_TRUE;
}


#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
