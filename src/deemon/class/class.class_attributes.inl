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
#include "class.c"
DEE_DECL_BEGIN
#define STRING
#endif

#ifdef STRING
#define attr_str       attr
#define ATTR_ARG       DEE_A_IN_Z char const *attr
#define ATTR_ARG_CHECK attr
#else
#define ATTR_ARG       DEE_A_IN_OBJECT(DeeStringObject) const *attr
#define ATTR_ARG_CHECK DeeObject_Check(attr)&&DeeString_Check(attr)
#endif

#ifdef STRING
DEE_A_RET_EXCEPT_REF DeeObject *DeeClass_GetAttrString
#else
DEE_A_RET_EXCEPT_REF DeeObject *DeeClass_GetAttr
#endif
(DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, ATTR_ARG) {
 struct _DeeClassTypeMethodDef *managed_methods;
 struct _DeeClassTypeGetSetDef *managed_getsets;
 struct DeeClassSuffix *suffix; DeeObject *result;
 DeeTypeObject const *tp_iter;
 int temp; Dee_size_t attr_len;
#ifndef STRING
 char const *attr_str;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(ATTR_ARG_CHECK),tp_iter = self;
#ifndef STRING
 attr_str = DeeString_STR(attr),attr_len = DeeString_SIZE(attr);
#else
 attr_len = strlen(attr);
#endif
 do {
  suffix = DeeClass_SUFFIX(tp_iter);
  // Step #1: Check for managed class methods/getsets
  managed_methods = suffix->cs_class_methods;
  DEE_ASSERT(managed_methods);
  while (managed_methods->ctmd_name) {
   DEE_ASSERT(DeeString_Check(managed_methods->ctmd_name));
   if (DeeString_SIZE(managed_methods->ctmd_name) == attr_len &&
       memcmp(DeeString_STR(managed_methods->ctmd_name),attr_str,attr_len) == 0
       ) return DeeInstanceMethod_New((DeeObject *)self,managed_methods->ctmd_func); // Found it!
   ++managed_methods;
  }
  managed_getsets = suffix->cs_class_getsets;
  DEE_ASSERT(managed_getsets);
  while (managed_getsets->ctgd_name) {
   DEE_ASSERT(DeeString_Check(managed_getsets->ctgd_name));
   if (DeeString_SIZE(managed_getsets->ctgd_name) == attr_len &&
       memcmp(DeeString_STR(managed_getsets->ctgd_name),attr_str,attr_len) == 0) {
    if (!managed_getsets->ctgd_get) {
     DeeError_AttributeError_NoRead(Dee_TYPE(self),attr_str);
     return NULL;
    }
    return DeeObject_ThisCall(managed_getsets->ctgd_get,(DeeObject *)self,Dee_EmptyTuple);
   }
   ++managed_getsets;
  }
  // Step #2: Check for managed methods/getsets
  managed_methods = suffix->cs_methods;
  DEE_ASSERT(managed_methods);
  while (managed_methods->ctmd_name) {
   DEE_ASSERT(DeeString_Check(managed_methods->ctmd_name));
   if (DeeString_SIZE(managed_methods->ctmd_name) == attr_len &&
       memcmp(DeeString_STR(managed_methods->ctmd_name),attr_str,attr_len) == 0
       ) DeeReturn_NEWREF(managed_methods->ctmd_func); // Found it!
   ++managed_methods;
  }
  managed_getsets = suffix->cs_getsets;
  DEE_ASSERT(managed_getsets);
  while (managed_getsets->ctgd_name) {
   DEE_ASSERT(DeeString_Check(managed_getsets->ctgd_name));
   if (DeeString_SIZE(managed_getsets->ctgd_name) == attr_len &&
       memcmp(DeeString_STR(managed_getsets->ctgd_name),attr_str,attr_len) == 0
       ) return DeeProperty_New(managed_getsets->ctgd_get,managed_getsets->ctgd_del,managed_getsets->ctgd_set);
   ++managed_getsets;
  }
  // Step #3: Check for unmanaged members/getsets/methods
  if ((temp = _DeeType_GetAttrString((DeeTypeObject *)tp_iter,attr_str,&result)) != 1)
   return temp == 0 ? result : NULL;
  tp_iter = DeeType_BASE(tp_iter); // Try the next type
 } while (DeeClass_Check(tp_iter));
 return DeeType_GetAttrString((DeeTypeObject *)tp_iter,attr_str);
}



#ifdef STRING
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeClass_HasAttrString
#else
DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeClass_HasAttr
#endif
(DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, ATTR_ARG) {
 struct _DeeClassTypeMethodDef *managed_methods;
 struct _DeeClassTypeGetSetDef *managed_getsets;
 struct DeeClassSuffix *suffix;
 DeeTypeObject const *tp_iter;
 Dee_size_t attr_len;
#ifndef STRING
 char const *attr_str;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(ATTR_ARG_CHECK),tp_iter = self;
#ifndef STRING
 attr_str = DeeString_STR(attr),attr_len = DeeString_SIZE(attr);
#else
 attr_len = strlen(attr);
#endif
 do {
  suffix = DeeClass_SUFFIX(tp_iter);
  // Step #1: Check for managed class methods/getsets
  managed_methods = suffix->cs_class_methods;
  DEE_ASSERT(managed_methods);
  while (managed_methods->ctmd_name) {
   DEE_ASSERT(DeeString_Check(managed_methods->ctmd_name));
   if (DeeString_SIZE(managed_methods->ctmd_name) == attr_len &&
       memcmp(DeeString_STR(managed_methods->ctmd_name),attr_str,attr_len) == 0
       ) return 1;
   ++managed_methods;
  }
  managed_getsets = suffix->cs_class_getsets;
  DEE_ASSERT(managed_getsets);
  while (managed_getsets->ctgd_name) {
   DEE_ASSERT(DeeString_Check(managed_getsets->ctgd_name));
   if (DeeString_SIZE(managed_getsets->ctgd_name) == attr_len &&
       memcmp(DeeString_STR(managed_getsets->ctgd_name),attr_str,attr_len) == 0
       ) return 1;
   ++managed_getsets;
  }
  // Step #2: Check for unmanaged members/getsets/methods
  if (_DeeType_HasAttrString((DeeTypeObject *)tp_iter,attr_str)) return 1;
  tp_iter = DeeType_BASE(tp_iter); // Try the next type
 } while (DeeClass_Check(tp_iter));
 return _DeeType_HasAttrString((DeeTypeObject *)tp_iter,attr_str);
}



#ifdef STRING
DEE_A_RET_EXCEPT(-1) int DeeClass_DelAttrString
#else
DEE_A_RET_EXCEPT(-1) int DeeClass_DelAttr
#endif
(DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, ATTR_ARG) {
 struct _DeeClassTypeGetSetDef *managed_getsets;
 struct DeeClassSuffix *suffix; DeeObject *result;
 DeeTypeObject const *tp_iter;
 int temp; Dee_size_t attr_len;
#ifndef STRING
 char const *attr_str;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(ATTR_ARG_CHECK),tp_iter = self;
#ifndef STRING
 attr_str = DeeString_STR(attr),attr_len = DeeString_SIZE(attr);
#else
 attr_len = strlen(attr);
#endif
 do {
  suffix = DeeClass_SUFFIX(tp_iter);
  // Step #1: Check for managed class getsets
  managed_getsets = suffix->cs_class_getsets;
  DEE_ASSERT(managed_getsets);
  while (managed_getsets->ctgd_name) {
   DEE_ASSERT(DeeString_Check(managed_getsets->ctgd_name));
   if (DeeString_SIZE(managed_getsets->ctgd_name) == attr_len &&
       memcmp(DeeString_STR(managed_getsets->ctgd_name),attr_str,attr_len) == 0) {
    if (!managed_getsets->ctgd_del) {
     DeeError_AttributeError_NoDel(Dee_TYPE(self),attr_str);
     return -1;
    }
    result = DeeObject_ThisCall(managed_getsets->ctgd_del,(DeeObject *)self,Dee_EmptyTuple);
    if (!result) return -1;
    Dee_DECREF(result);
    return 0;
   }
   ++managed_getsets;
  }
  // Step #2: Check for unmanaged getsets
  if ((temp = _DeeType_DelAttrString((DeeTypeObject *)tp_iter,attr_str)) != 1) return temp;
  tp_iter = DeeType_BASE(tp_iter); // Try the next type
 } while (DeeClass_Check(tp_iter));
 return DeeType_DelAttrString((DeeTypeObject *)tp_iter,attr_str);
}



#ifdef STRING
DEE_A_RET_EXCEPT(-1) int DeeClass_SetAttrString
#else
DEE_A_RET_EXCEPT(-1) int DeeClass_SetAttr
#endif
(DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, ATTR_ARG, DEE_A_INOUT DeeObject *v) {
 struct _DeeClassTypeGetSetDef *managed_getsets;
 struct DeeClassSuffix *suffix; DeeObject *result,*args;
 DeeTypeObject const *tp_iter;
 int temp; Dee_size_t attr_len;
#ifndef STRING
 char const *attr_str;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(ATTR_ARG_CHECK);
 DEE_ASSERT(DeeObject_Check(v)),tp_iter = self;
#ifndef STRING
 attr_str = DeeString_STR(attr),attr_len = DeeString_SIZE(attr);
#else
 attr_len = strlen(attr);
#endif
 do {
  suffix = DeeClass_SUFFIX(tp_iter);
  // Step #1: Check for managed class getsets
  managed_getsets = suffix->cs_class_getsets;
  DEE_ASSERT(managed_getsets);
  while (managed_getsets->ctgd_name) {
   DEE_ASSERT(DeeString_Check(managed_getsets->ctgd_name));
   if (DeeString_SIZE(managed_getsets->ctgd_name) == attr_len &&
       memcmp(DeeString_STR(managed_getsets->ctgd_name),attr_str,attr_len) == 0) {
    if (!managed_getsets->ctgd_set) {
     DeeError_AttributeError_NoWrite(Dee_TYPE(self),attr_str);
     return -1;
    }
    if ((args = DeeTuple_Pack(1,v)) == NULL) return -1;
    result = DeeObject_ThisCall(managed_getsets->ctgd_set,(DeeObject *)self,args);
    Dee_DECREF(args);
    if (!result) return -1;
    Dee_DECREF(result);
    return 0;
   }
   ++managed_getsets;
  }
  // Step #2: Check for unmanaged members/getsets
  if ((temp = _DeeType_SetAttrString((DeeTypeObject *)tp_iter,attr_str,v)) != 1) return temp;
  tp_iter = DeeType_BASE(tp_iter); // Try the next type
 } while (DeeClass_Check(tp_iter));
 return DeeType_SetAttrString((DeeTypeObject *)tp_iter,attr_str,v);
}



#ifdef STRING
DEE_A_RET_EXCEPT_REF DeeObject *DeeClass_CallAttrString
#else
DEE_A_RET_EXCEPT_REF DeeObject *DeeClass_CallAttr
#endif
(DEE_A_INOUT_TYPEOBJECT(DeeClassObject) *self, ATTR_ARG,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 struct _DeeClassTypeMethodDef *managed_methods;
 struct _DeeClassTypeGetSetDef *managed_getsets;
 struct DeeClassSuffix *suffix; DeeObject *callback,*result;
 DeeTypeObject const *tp_iter;
 int temp; Dee_size_t attr_len;
#ifndef STRING
 char const *attr_str;
#endif
 DEE_ASSERT(DeeObject_Check(self) && DeeClass_Check(self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 DEE_ASSERT(ATTR_ARG_CHECK),tp_iter = self;
#ifndef STRING
 attr_str = DeeString_STR(attr),attr_len = DeeString_SIZE(attr);
#else
 attr_len = strlen(attr);
#endif
 do {
  suffix = DeeClass_SUFFIX(tp_iter);
  // Step #1: Check for managed class methods/getsets
  managed_methods = suffix->cs_class_methods;
  DEE_ASSERT(managed_methods);
  while (managed_methods->ctmd_name) {
   DEE_ASSERT(DeeString_Check(managed_methods->ctmd_name));
   if (DeeString_SIZE(managed_methods->ctmd_name) == attr_len &&
       memcmp(DeeString_STR(managed_methods->ctmd_name),attr_str,attr_len) == 0
       ) return DeeObject_ThisCall(managed_methods->ctmd_func,(DeeObject *)self,args); // Found it!
   ++managed_methods;
  }
  managed_getsets = suffix->cs_class_getsets;
  DEE_ASSERT(managed_getsets);
  while (managed_getsets->ctgd_name) {
   DEE_ASSERT(DeeString_Check(managed_getsets->ctgd_name));
   if (DeeString_SIZE(managed_getsets->ctgd_name) == attr_len &&
       memcmp(DeeString_STR(managed_getsets->ctgd_name),attr_str,attr_len) == 0) {
    if (!managed_getsets->ctgd_get) {
     DeeError_AttributeError_NoRead(Dee_TYPE(self),attr_str);
     return NULL;
    }
    callback = DeeObject_ThisCall(managed_getsets->ctgd_get,(DeeObject *)self,Dee_EmptyTuple);
    if (!callback) return NULL;
    result = DeeObject_Call(callback,args);
    Dee_DECREF(callback);
    return result;
   }
   ++managed_getsets;
  }
  // Step #2: Check for managed methods
  managed_methods = suffix->cs_methods;
  DEE_ASSERT(managed_methods);
  while (managed_methods->ctmd_name) {
   DEE_ASSERT(DeeString_Check(managed_methods->ctmd_name));
   if (DeeString_SIZE(managed_methods->ctmd_name) == attr_len &&
       memcmp(DeeString_STR(managed_methods->ctmd_name),attr_str,attr_len) == 0
       ) return DeeObject_Call(managed_methods->ctmd_func,args); // Found it!
   ++managed_methods;
  }
  // Step #3: Check for unmanaged members/getsets/methods
  if ((temp = _DeeType_CallAttrString((DeeTypeObject *)tp_iter,attr_str,args,&result)) != 1)
   return temp == 0 ? result : NULL;
  tp_iter = DeeType_BASE(tp_iter); // Try the next type
 } while (DeeClass_Check(tp_iter));
 return DeeType_CallAttrString((DeeTypeObject *)tp_iter,attr_str,args);
}


#ifdef STRING
#undef STRING
#endif
#undef attr_str
#undef ATTR_ARG
#undef ATTR_ARG_CHECK
#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
