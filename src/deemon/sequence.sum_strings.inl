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
//#ifdef __INTELLISENSE__
//#include "sequence.c"
//DEE_DECL_BEGIN
//#define ENC Utf8
//#endif
#ifndef ENC
#error "Must '#define ENC' before #including this file"
#endif

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeObject *) DEE_PP_CAT_3(_DeeIteratorSum_,ENC,String)(
 DEE_A_INOUT DEE_PP_CAT_3(Dee,ENC,StringObject) *first, DEE_A_INOUT DeeObject *iterator) {
 DeeObject *result,*elem; int error;
 struct DEE_PP_CAT_3(Dee,ENC,StringWriter) writer = DEE_PP_CAT_3(Dee,ENC,StringWriter_INIT)();
 if (DEE_PP_CAT_3(Dee,ENC,StringWriter_WriteMyStringWithLength)(&writer,
  DEE_PP_CAT_3(Dee,ENC,String_SIZE)(first),
  DEE_PP_CAT_3(Dee,ENC,String_STR)(first)) != 0) goto err;
 while DEE_LIKELY((error = DeeObject_IterNextEx(iterator,&elem)) == 0) {
  error = DEE_PP_CAT_3(Dee,ENC,StringWriter_WriteObjectStr)(&writer,elem);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(error != 0) goto err;
 }
 if DEE_UNLIKELY(error < 0) goto err;
 result = DEE_PP_CAT_3(Dee,ENC,StringWriter_Pack)(&writer);
end: DEE_PP_CAT_3(Dee,ENC,StringWriter_Quit)(&writer);
 return result;
err: result = NULL; goto end;
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeObject *) DEE_PP_CAT_3(_DeeVectorSum_,ENC,String)(
 DEE_A_INOUT DEE_PP_CAT_3(Dee,ENC,StringObject) *first, DEE_A_IN Dee_size_t elemc, DEE_A_IN_R(elemc) DeeObject **elemv) {
 DeeObject **end,*result;
 struct DEE_PP_CAT_3(Dee,ENC,StringWriter) writer = DEE_PP_CAT_3(Dee,ENC,StringWriter_INIT)();
 if DEE_UNLIKELY(DEE_PP_CAT_3(Dee,ENC,StringWriter_WriteMyStringWithLength)(&writer,
  DEE_PP_CAT_3(Dee,ENC,String_SIZE)(first),
  DEE_PP_CAT_3(Dee,ENC,String_STR)(first)) != 0) goto err;
 end = elemv+elemc;
 while DEE_LIKELY(elemv != end) {
  if DEE_UNLIKELY(DEE_PP_CAT_3(Dee,ENC,
   StringWriter_WriteObjectStr)(&writer,*elemv) != 0) goto err;
  ++elemv;
 }
 result = DEE_PP_CAT_3(Dee,ENC,StringWriter_Pack)(&writer);
end: DEE_PP_CAT_3(Dee,ENC,StringWriter_Quit)(&writer);
 return result;
err: result = NULL; goto end;
}

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT_REF DeeObject *) DEE_PP_CAT_3(_DeeListSum_,ENC,String)(
 DEE_A_INOUT DEE_PP_CAT_3(Dee,ENC,StringObject) *first, DEE_A_INOUT DeeListObject *list) {
 Dee_size_t i; DeeObject *elem,*result; int temp;
 struct DEE_PP_CAT_3(Dee,ENC,StringWriter) writer = DEE_PP_CAT_3(Dee,ENC,StringWriter_INIT)();
 if DEE_UNLIKELY(DEE_PP_CAT_3(Dee,ENC,StringWriter_WriteMyStringWithLength)(&writer,
  DEE_PP_CAT_3(Dee,ENC,String_SIZE)(first),
  DEE_PP_CAT_3(Dee,ENC,String_STR)(first)) != 0) goto err;
 DeeList_ACQUIRE(list);
 for (i = 1 /*Start at one, as 'first' was the first one*/; DEE_LIKELY(i < DeeList_SIZE(list)); ++i) {
  Dee_INCREF(elem = DeeList_GET(list,i));
  DeeList_RELEASE(list);
  temp = DEE_PP_CAT_3(Dee,ENC,StringWriter_WriteObjectStr)(&writer,elem);
  Dee_DECREF(elem);
  if DEE_UNLIKELY(temp != 0) goto err;
  DeeList_ACQUIRE(list);
 }
 DeeList_RELEASE(list);
 result = DEE_PP_CAT_3(Dee,ENC,StringWriter_Pack)(&writer);
end: DEE_PP_CAT_3(Dee,ENC,StringWriter_Quit)(&writer);
 return result;
err: result = NULL; goto end;
}

#undef ENC

//#ifdef __INTELLISENSE__
//DEE_DECL_END
//#endif

