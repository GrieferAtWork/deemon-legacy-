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
#ifndef GUARD_DEEMON_OBJECT_C
#define GUARD_DEEMON_OBJECT_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/array.h>
#include <deemon/bool.h>
#include <deemon/cell.h>
#include <deemon/class.h>
#include <deemon/dict.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/gc.h>
#include <deemon/deemonrun.h>
#include <deemon/generic_vtable.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_visit.h>
#include <deemon/list.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/set.h>
#include <deemon/struct.h>
#include <deemon/structured.h>
#include <deemon/tuple.h>
#include <deemon/type.h>
#include <deemon/value.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int _DeeObject_DestructChain(
 DEE_A_IN DeeTypeObject const *start,
 DEE_A_INOUT DeeObject *self) {
 DEE_ASSERT(DeeObject_Check(start) && DeeType_Check(start));
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_InstanceOf(self,start));
 DEE_ASSERTF(DeeObject_IS_UNIQUE(self),
             "Cannot destroy non-unique object");
 DEE_ASSERTF(DeeError_OCCURRED(),
             "_DeeObject_DestructChain() may only be called "
             "after an error has occurred and has been set.");
 do {
  // Overwrite the typing of 'self' with the one we are trying to destruct
  DEE_ASSERTF(DeeObject_IS_UNIQUE(self),"Cannot destroy non-unique object");
  Dee_INCREF(start);
  Dee_DECREF(self->__ob_type);
  self->__ob_type = (DeeTypeObject *)start;
  if ((DeeType_FLAGS(start)&DEE_TYPE_FLAG_ALWAYS_CLEAR)!=0) {
   DEE_ASSERT(DeeType_HAS_SLOT(start,tp_clear));
   DEE_ASSERT(self->__ob_refcnt == 1);
   (*DeeType_GET_SLOT(start,tp_clear))(self);
   if (_DeeRefCounter_Fetch(self->__ob_refcnt) != 1) {
    DEE_ASSERTF(_DeeRefCounter_Fetch(self->__ob_refcnt) > 1,
                "Unexpected reference count after 'tp_clear'");
    // If a reference was added, the tp_clear has revived the object
    // The object is now back in a consistent state, and is being used somewhere
    // >> cl = <>;
    // >> class a {
    // >>   this() { print __FUNCTION__; }
    // >>   ~this() { print __FUNCTION__; cl.set(this); } // Reviving destructor
    // >> };
    // >> class b: a {
    // >>   this() { print __FUNCTION__; throw "Oh No!"; } // Failed constructor
    // >>   ~this() { print __FUNCTION__; }
    // >> };
    // >> 
    // >> b(); // Actually returns an instance of 'a'
    // >> print cl.get();
    DeeError_Print("[ignored] A destructor revived an object after the "
                   "constructor of one of its sub-types threw an error\n",1);
    return 0; // Object is now in a consistent state
   }
  }
  if (DeeType_HAS_SLOT(start,tp_dtor))
   (*DeeType_GET_SLOT(start,tp_dtor))(self);
 } while ((start = DeeType_BASE(start)) != NULL);
 return -1; // The error set by the caller
}


DEE_A_RET_NOEXCEPT(0) int _DeeType_HasAttrString(
 DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z char const *attr) {
 struct DeeMemberDef const *members;
 struct DeeGetSetDef const *getsets;
 struct DeeMethodDef const *methods;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT(attr);
 do {
  // v default type attributes ("__name__", ...)
  //   >> s.a.: "_deetype_tp_members"
  if (_DeeObject_GenericHasAttrString((DeeObject *)self,attr)) return 1;
  members = DeeType_GET_SLOT(self,tp_class_members),DEE_ASSERT(members);
  while (members->d_name) { if (strcmp(members->d_name,attr) == 0) return 1; ++members; }
  getsets = DeeType_GET_SLOT(self,tp_class_getsets),DEE_ASSERT(getsets);
  while (getsets->d_name) { if (strcmp(getsets->d_name,attr) == 0) return 1; ++getsets; }
  methods = DeeType_GET_SLOT(self,tp_class_methods),DEE_ASSERT(methods);
  while (methods->d_name) { if (strcmp(methods->d_name,attr) == 0) return 1; ++methods; }
 } while ((self = DeeType_BASE(self)) != NULL);
 return 0;
}

DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeType_DelAttrString(
 DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z char const *attr) {
 struct DeeGetSetDef const *getsets; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self)); do {
  if ((temp = _DeeObject_GenericDelAttrString((DeeObject *)self,attr)
   ) != 1) return temp; // Return value if anything buf not-found
  getsets = DeeType_GET_SLOT(self,tp_class_getsets);
  DEE_ASSERT(getsets);
  while (getsets->d_name) {
   if (strcmp(getsets->d_name,attr) == 0) {
    if (getsets->d_del) {
     DEE_ASSERT((getsets->d_flags&DEE_GETSETDEF_FLAG_STRUCT)==0 &&
                "Type with structured getset");
     return (*DeeGetSetDef_ODEL(getsets))((DeeObject *)self,getsets->d_closure);
    }
    DeeError_AttributeError_NoDel(Dee_TYPE(self),attr);
    return -1;
   }
   ++getsets;
  }
 } while ((self = DeeType_BASE(self)) != NULL);
 return 1;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeType_SetAttrString(
 DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z char const *attr, DEE_A_INOUT DeeObject *v) {
 struct DeeMemberDef const *members; int temp;
 struct DeeGetSetDef const *getsets;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self)); do {
  if ((temp = _DeeObject_GenericSetAttrString((DeeObject *)self,attr,v)
   ) != 1) return temp; // Return value if anything buf not_found
  members = DeeType_GET_SLOT(self,tp_class_members);
  DEE_ASSERT(members);
  while (members->d_name) {
   if (strcmp(members->d_name,attr) == 0) {
#if !DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS
    if ((members->d_access&DeeMemberAccess_WRITE) != 0)
#endif /* !DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS */
    {
     return DeeMember_Set((DeeObject *)self,members,v);
    }
#if !DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS
    goto no_w;
#endif /* !DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS */
   }
   ++members;
  }
  getsets = DeeType_GET_SLOT(self,tp_class_getsets);
  DEE_ASSERT(getsets);
  while (getsets->d_name) {
   if (strcmp(getsets->d_name,attr) == 0) {
    if (getsets->d_set) {
     DEE_ASSERT((getsets->d_flags&DEE_GETSETDEF_FLAG_STRUCT)==0 &&
                "Type with structured getset");
     return (*DeeGetSetDef_OSET(getsets))((DeeObject *)self,v,getsets->d_closure);
    }
#if !DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS
no_w: // no_write
#endif /* !DEE_XCONFIG_FORCE_MEMBER_WRITE_ACCESS */
    DeeError_AttributeError_NoWrite(Dee_TYPE(self),attr);
    return -1;
   }
   ++getsets;
  }
 } while ((self = DeeType_BASE(self)) != NULL);
 return 1;
}

DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeType_CallAttrString(
 DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z char const *attr,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args, DEE_A_REF DEE_A_OUT DeeObject **result) {
 DeeTypeObject *iter = self,*tp;
 DeeObject *attribute;
 struct DeeMethodDef const *methods;
 struct DeeMemberDef const *members;
 struct DeeGetSetDef const *getsets;
 do { // Call object methods
  tp = Dee_TYPE(iter); do {
   methods = DeeType_GET_SLOT(tp,tp_methods);
   DEE_ASSERT(methods);
   while (methods->d_name) {
    if (strcmp(methods->d_name,attr) == 0) {
call_meth:
     DEE_ASSERT(methods->d_func);
     DEE_ASSERT((methods->d_flags&DEE_METHODDEF_FLAG_STRUCT)==0 &&
                "Type with structured method");
     if ((*result = (*DeeMethodDef_OFUNC(methods))(
      (DeeObject *)self,args,methods->d_closure)) == NULL) return -1;
     return 0;
    }
    ++methods;
   }
   members = DeeType_GET_SLOT(tp,tp_members);
   DEE_ASSERT(members);
   while (members->d_name) {
    if (strcmp(members->d_name,attr) == 0) {
call_member:
     if ((attribute = DeeMember_Get((DeeObject *)self,members)) == NULL) return -1;
     *result = DeeObject_Call(attribute,(DeeObject *)args);
     Dee_DECREF(attribute);
     return *result ? 0 : -1;
    }
    ++members;
   }
   getsets = DeeType_GET_SLOT(tp,tp_getsets);
   DEE_ASSERT(members);
   while (getsets->d_name) {
    if (strcmp(getsets->d_name,attr) == 0) {
call_getset:
     if (!getsets->d_get) {
      DeeError_AttributeError_NoRead(Dee_TYPE(self),attr);
      return -1;
     }
     DEE_ASSERT((getsets->d_flags&DEE_GETSETDEF_FLAG_STRUCT)==0 &&
                "Type with structured getset");
     if ((attribute = (*DeeGetSetDef_OGET(getsets))(
      (DeeObject *)self,getsets->d_closure)) == NULL) return -1;
     *result = DeeObject_Call(attribute,(DeeObject *)args);
     Dee_DECREF(attribute);
     return *result ? 0 : -1;
    }
    ++getsets;
   }
  } while ((tp = DeeType_BASE(tp)) != NULL);
  methods = DeeType_GET_SLOT(iter,tp_class_methods),DEE_ASSERT(methods);
  while (methods->d_name) if (strcmp(methods->d_name,attr) == 0) goto call_meth; else ++methods;
  members = DeeType_GET_SLOT(iter,tp_class_members),DEE_ASSERT(members);
  while (members->d_name) if (strcmp(members->d_name,attr) == 0) goto call_member; else ++members;
  getsets = DeeType_GET_SLOT(iter,tp_class_getsets),DEE_ASSERT(members);
  while (getsets->d_name) if (strcmp(getsets->d_name,attr) == 0) goto call_getset; else ++getsets;
 } while ((iter = DeeType_BASE(iter)) != NULL);
 // Method not found
 return 1;
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeType_GetAttr(
 DEE_A_INOUT DeeTypeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 DeeObject *result; int temp; char const *attr_str;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 attr_str = DeeString_STR(attr);
 if ((temp = _DeeType_GetAttrString(self,attr_str,&result)) != 0) {
  if (temp == 1) DeeError_AttributeError_NotFound_InType(self,attr_str);
  result = NULL;
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeType_GetAttrString(
 DEE_A_INOUT DeeTypeObject *self, DEE_A_IN_Z char const *attr) {
 DeeObject *result; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 if ((temp = _DeeType_GetAttrString(self,attr,&result)) != 0) {
  if (temp == 1) DeeError_AttributeError_NotFound_InType(self,attr);
  result = NULL;
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeType_GetConstAttrString(
 DEE_A_INOUT DeeTypeObject *self, DEE_A_IN_Z char const *attr) {
 DeeObject *result; int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 if ((temp = _DeeType_GetConstAttrString(self,attr,&result)) != 0) {
  if (temp == 1) DeeError_AttributeError_NotFound_InType(self,attr);
  result = NULL;
 }
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeType_DelAttr(
 DEE_A_INOUT DeeTypeObject *self, DEE_A_IN_OBJECT(DeeStringObject) const *attr) {
 int temp; char const *attr_str;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 attr_str = DeeString_STR(attr);
 if ((temp = _DeeType_DelAttrString(self,attr_str)) != 0) {
  if (temp == 1) DeeError_AttributeError_NotFound_InType(self,attr_str);
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeType_DelAttrString(
 DEE_A_INOUT DeeTypeObject *self, DEE_A_IN_Z char const *attr) {
 int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 if ((temp = _DeeType_DelAttrString(self,attr)) != 0) {
  if (temp == 1) DeeError_AttributeError_NotFound_InType(self,attr);
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeType_SetAttr(DEE_A_INOUT DeeTypeObject *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_INOUT DeeObject *v) {
 int temp; char const *attr_str;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(v));
 attr_str = DeeString_STR(attr);
 if ((temp = _DeeType_SetAttrString(self,attr_str,v)) != 0) {
  if (temp == 1) DeeError_AttributeError_NotFound_InType(self,attr_str);
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeType_SetAttrString(
 DEE_A_INOUT DeeTypeObject *self, DEE_A_IN_Z char const *attr, DEE_A_INOUT DeeObject *v) {
 int temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT(DeeObject_Check(v));
 if ((temp = _DeeType_SetAttrString(self,attr,v)) != 0) {
  if (temp == 1) DeeError_AttributeError_NotFound_InType(self,attr);
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeType_CallAttrString(
 DEE_A_INOUT DeeTypeObject *self, DEE_A_IN_Z char const *attr,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DeeObject *result; int temp;
 if ((temp = _DeeType_CallAttrString(self,attr,args,&result)) < 0) result = NULL;
 else if (temp == 1) { DeeError_AttributeError_NotFound_InType(self,attr); result = NULL; }
 return result;
}




DEE_A_RET_WUNUSED int DeeType_IsSameOrDerived(
 DEE_A_IN DeeTypeObject const *subclass,
 DEE_A_IN DeeTypeObject const *base) {
 DEE_ASSERT(DeeObject_Check(subclass)/* && DeeType_Check(subclass)*/);
 DEE_ASSERT(DeeObject_Check(base)/* && DeeType_Check(base)*/);
 while (1) {
  if (base == subclass) return 1;
  if ((subclass = DeeType_BASE(subclass)) == NULL) break;
  //DEE_ASSERT(DeeObject_Check(subclass) && DeeType_Check(subclass));
 }
 return 0;
}
DEE_A_RET_WUNUSED int DeeType_IsDerived(
 DEE_A_IN DeeTypeObject const *subclass,
 DEE_A_IN DeeTypeObject const *base) {
 DEE_ASSERT(DeeObject_Check(subclass)/* && DeeType_Check(subclass)*/);
 DEE_ASSERT(DeeObject_Check(base)/* && DeeType_Check(base)*/);
 while (1) {
  if ((subclass = DeeType_BASE(subclass)) == NULL) break;
  //DEE_ASSERT(DeeObject_Check(subclass) && DeeType_Check(subclass));
  if (base == subclass) return 1;
 }
 return 0;
}


DEE_A_RET_WUNUSED int DeeType_MustDeepCopy(DEE_A_IN DeeTypeObject const *tp) {
 return tp == &DeeTuple_Type || tp == &DeeList_Type || 
        tp == &DeeDict_Type || tp == &DeeSet_Type ||
        tp == &DeeCell_Type;
}
DEE_A_RET_EXCEPT(-1) int DeeType_SetName(
 DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z_OPT char const *name) {
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT((DeeType_FLAGS(self)&DEE_TYPE_FLAG_HEAP_TYPE)!=0 &&
            "Cannot set name of non-heap type");
 if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_STATIC_NAME)==0)
  free_nn((char *)DeeType_GET_SLOT(self,tp_name));
 else DeeType_GET_SLOT(self,tp_flags) &= ~DEE_TYPE_FLAG_STATIC_NAME;
 if (name && *name) {
  if ((name = strdup_nz(name)) == NULL) { DeeError_NoMemory(); return -1; }
  DeeType_GET_SLOT(self,tp_name) = name;
 } else {
  DeeType_GET_SLOT(self,tp_flags) |= DEE_TYPE_FLAG_STATIC_NAME;
  DeeType_GET_SLOT(self,tp_name) = NULL;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeType_SetDoc(
 DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z_OPT char const *doc) {
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT((DeeType_FLAGS(self)&DEE_TYPE_FLAG_HEAP_TYPE)!=0 &&
            "Cannot set doc of non-heap type");
 if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_STATIC_DOC)==0)
  free_nn((char *)DeeType_GET_SLOT(self,tp_doc));
 else DeeType_GET_SLOT(self,tp_flags) &= ~DEE_TYPE_FLAG_STATIC_DOC;
 if (doc && *doc) {
  if ((doc = strdup_nz(doc)) == NULL) { DeeError_NoMemory(); return -1; }
  DeeType_GET_SLOT(self,tp_doc) = doc;
 } else {
  DeeType_GET_SLOT(self,tp_flags) |= DEE_TYPE_FLAG_STATIC_DOC;
  DeeType_GET_SLOT(self,tp_doc) = NULL;
 }
 return 0;
}
void DeeType_SetStaticName(
 DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z_OPT char const *name) {
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT((DeeType_FLAGS(self)&DEE_TYPE_FLAG_HEAP_TYPE)!=0 &&
            "Cannot set name of non-heap type");
 if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_STATIC_NAME)==0) {
  free_nn((char *)DeeType_GET_SLOT(self,tp_name));
  DeeType_GET_SLOT(self,tp_flags) |= DEE_TYPE_FLAG_STATIC_NAME;
 }
 DeeType_GET_SLOT(self,tp_name) = name && *name ? name : NULL;
}
void DeeType_SetStaticDoc(
 DEE_A_IN DeeTypeObject *self, DEE_A_IN_Z_OPT char const *doc) {
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT((DeeType_FLAGS(self)&DEE_TYPE_FLAG_HEAP_TYPE)!=0 &&
            "Cannot set doc of non-heap type");
 if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_STATIC_DOC)==0) {
  free_nn((char *)DeeType_GET_SLOT(self,tp_doc));
  DeeType_GET_SLOT(self,tp_flags) |= DEE_TYPE_FLAG_STATIC_DOC;
 }
 DeeType_GET_SLOT(self,tp_doc) = doc && *doc ? doc : NULL;
}



DEE_A_RET_EXCEPT_REF DeeObject *
DeeType_NewInstanceDefault(DEE_A_IN DeeTypeObject const *self) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 if ((DeeType_FLAGS(self) & DEE_TYPE_FLAG_VAR_OBJECT) != 0)
  return (*DeeType_GET_SLOT(self,tp_new))((DeeTypeObject *)self);
 if ((result = (*DeeType_GET_SLOT(self,tp_alloc))((DeeTypeObject *)self)) != NULL) {
  if (DeeObject_ConstructDef(result) != 0) { _DeeObject_DELETE(self,result); return NULL; }
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeType_NewInstance(
 DEE_A_IN DeeTypeObject const *self,
 DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 if ((DeeType_FLAGS(self) & DEE_TYPE_FLAG_VAR_OBJECT) != 0) {
  switch (DeeTuple_SIZE(args)) {
   case 0: return (*DeeType_GET_SLOT(self,tp_new))((DeeTypeObject *)self);
   case 1:
    result = DeeTuple_GET(args,0);
    if (DeeObject_InstanceOf(result,self)) {
     return (*DeeType_GET_SLOT(self,tp_copy_new))((DeeTypeObject *)self,result);
    }
    DEE_ATTRIBUTE_FALLTHROUGH
   default: break;
  }
  return (*DeeType_GET_SLOT(self,tp_any_new))((DeeTypeObject *)self,args);
 }
 if ((result = (*DeeType_GET_SLOT(self,tp_alloc))((DeeTypeObject *)self)) != NULL) {
  DEE_ASSERT(DeeObject_Check(result));
  DEE_ASSERT(DeeObject_InstanceOf(result,self));
  if (DeeObject_Construct(result,args) != 0) {
   _DeeObject_DELETE(self,result);
   return NULL;
  }
 }
 return result;
}


DEE_A_RET_EXCEPT_REF DeeObject *DeeType_VNewInstancef(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_INOUT va_list args) {
 DeeObject *result,*args_ob;
 if ((args_ob = Dee_VBuildTuple(fmt,args)) != NULL) {
  result = DeeType_NewInstance(self,args_ob);
  Dee_DECREF(args_ob);
 } else result = NULL;
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeType_VNewInstanceObjArgs(
 DEE_A_IN DeeTypeObject const *self, DEE_A_INOUT va_list args) {
 DeeObject *result,*args_ob;
 if ((args_ob = DeeTuple_VNewObjArgs(args)) != NULL) {
  result = DeeType_NewInstance(self,args_ob);
  Dee_DECREF(args_ob);
 } else result = NULL;
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeType_NewInstancef(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN_BUILDTUPLEF char const *fmt, ...) {
 va_list args; DeeObject *result,*args_ob;
 DEE_VA_START(args,fmt);
 if ((args_ob = Dee_VBuildTuple(fmt,args)) != NULL) {
  result = DeeType_NewInstance(self,args_ob);
  Dee_DECREF(args_ob);
 } else result = NULL;
 DEE_VA_END(args);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeObject *DeeType_NewInstanceObjArgs(
 DEE_A_IN DeeTypeObject const *self, ...) {
 va_list args; DeeObject *result,*args_ob;
 DEE_VA_START(args,self);
 if ((args_ob = DeeTuple_VNewObjArgs(args)) != NULL) {
  result = DeeType_NewInstance(self,args_ob);
  Dee_DECREF(args_ob);
 } else result = NULL;
 DEE_VA_END(args);
 return result;
}




DEE_A_RET_EXCEPT(-1) int DeeObject_Construct(
 DEE_A_INOUT DeeObject *self, DEE_A_INOUT_OBJECT(DeeTupleObject) *args) {
 DeeObject *arg0; DeeTypeObject *tp;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(args) && DeeTuple_Check(args));
 tp = Dee_TYPE(self);
 switch (DeeTuple_SIZE(args)) {
  case 0:
   if ((*DeeType_GET_SLOT(tp,tp_ctor))(tp,self) != 0) return -1;
   break;
  case 1:
   arg0 = DeeTuple_GET(args,0);
   if (DeeObject_InstanceOf(arg0,tp)) {
    if ((*DeeType_GET_SLOT(tp,tp_copy_ctor))(tp,self,arg0) != 0) return -1;
    break;
   }
   DEE_ATTRIBUTE_FALLTHROUGH
  default:
   if ((*DeeType_GET_SLOT(tp,tp_any_ctor))(tp,self,args) != 0) return -1;
   break;
 }
 if ((DeeType_FLAGS(tp)&DEE_TYPE_FLAG_HAS_GC)!=0) DeeGC_TrackedAdd(self);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_ConstructDef(DEE_A_INOUT DeeObject *self) {
 DeeTypeObject *tp;
 DEE_ASSERT(DeeObject_Check(self)); tp = Dee_TYPE(self);
 if ((*DeeType_GET_SLOT(tp,tp_ctor))(tp,self) != 0) return -1;
 if ((DeeType_FLAGS(tp)&DEE_TYPE_FLAG_HAS_GC)!=0) DeeGC_TrackedAdd(self);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_CopyConstruct(
 DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *right) {
 DeeTypeObject *tp; int result;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(right));
 tp = Dee_TYPE(self);
 if (DeeType_IsSameOrDerived(Dee_TYPE(right),tp)) {
  // Right is derived from left
  result = (*DeeType_GET_SLOT(tp,tp_copy_ctor))(tp,self,right);
 } else { // Fallback constructor
  DeeObject *args;
  if ((args = DeeTuple_Pack(1,right)) == NULL) return -1;
  result = (*DeeType_GET_SLOT(tp,tp_any_ctor))(tp,self,args);
  Dee_DECREF(args);
 }
 if (result == 0 && (DeeType_FLAGS(tp)&DEE_TYPE_FLAG_HAS_GC)!=0)
  DeeGC_TrackedAdd(self);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeObject_MoveConstruct(
 DEE_A_INOUT DeeObject *self, DEE_A_INOUT DeeObject *right) {
 DeeTypeObject *tp; int result;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(right));
 tp = Dee_TYPE(self);
 if (DeeType_IsSameOrDerived(Dee_TYPE(right),tp)) {
  // Right is derived from left
  result = (*DeeType_GET_SLOT(tp,tp_move_ctor))(tp,self,right);
 } else { // Fallback constructor
  DeeObject *args;
  if ((args = DeeTuple_Pack(1,right)) == NULL) return -1;
  result = (*DeeType_GET_SLOT(tp,tp_any_ctor))(tp,self,args);
  Dee_DECREF(args);
 }
 if (result == 0 && (DeeType_FLAGS(tp)&DEE_TYPE_FLAG_HAS_GC)!=0)
  DeeGC_TrackedAdd(self);
 return result;
}



DEE_A_RET_WUNUSED DEE_A_RET_MAYBE_NULL void *DeeType_GetSlot(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN int slot) {
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
#define S(name)\
 case DeeType_SLOT_ID(name):\
  return (void *)&DeeType_GET_SLOT(self,name);
 switch (slot) {
#include "__typeslots.inl"
#undef S
  default: break;
 }
 return NULL;
}
DEE_A_RET_WUNUSED int DeeType_HasSlot(DEE_A_IN DeeTypeObject const *self, DEE_A_IN int slot) {
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
#define S(name)\
 case DeeType_SLOT_ID(name):\
  return DeeType_HAS_SLOT(self,name);
 switch (slot) {
#include "__typeslots.inl"
#undef S
  default: break;
 }
 return 0;
}
void DeeType_DelSlot(DEE_A_IN DeeTypeObject *self, DEE_A_IN int slot) {
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
#define S(name)\
 case DeeType_SLOT_ID(name):\
  DeeType_DEL_SLOT(self,name);\
  break;
 switch (slot) {
#include "__typeslots.inl"
#undef S
  default: break;
 }
}
DEE_A_RET_Z_OPT char const *DeeType_Name(
 DEE_A_IN DeeTypeObject const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 return DeeType_NAME(self);
}
DEE_A_RET_MAYBE_NULL DEE_A_RET_WUNUSED DeeTypeObject *
DeeType_Base(DEE_A_IN DeeTypeObject const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 return DeeType_BASE(self);
}






DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(NULL) DeeTypeObject *) _DeeType_GenericOffsetOfString(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN_Z char const *attr, DEE_A_OUT_OPT Dee_size_t *offset) {
 DeeTypeObject const *tp_next,*tp_iter;
 tp_iter = self; while (1) {
  struct DeeMemberDef const *members;
  members = DeeType_GET_SLOT(self,tp_members);
  DEE_ASSERT(members);
  while (members->d_name) {
   if (strcmp(members->d_name,attr) == 0) {
    if (offset) *offset = members->d_offset;
    return DeeMember_Type(members);
   }
   ++members;
  }
  tp_next = DeeType_BASE(self);
  if (tp_iter == tp_next) break;
  tp_iter = tp_next;
 }
 DeeError_AttributeError_NotFound(self,attr);
 return NULL;
}
DEE_A_RET_EXCEPT(NULL) DeeTypeObject *DeeType_OffsetofAttrString(
 DEE_A_IN DeeTypeObject const *self,
 DEE_A_IN_Z char const *attr, DEE_A_OUT_OPT Dee_size_t *offset) {
 DeeTypeObject *result;
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 DEE_ASSERT(attr);
 if (DeeStructType_Check(self)) {
  if ((result = DeeStructType_GetFieldString(self,attr,offset)) != NULL) return result;
 }
 return _DeeType_GenericOffsetOfString(self,attr,offset);
}
DEE_A_RET_EXCEPT(NULL) DeeTypeObject *DeeType_OffsetofAttr(
 DEE_A_IN DeeTypeObject const *self,
 DEE_A_IN_OBJECT(DeeStringObject) const *attr, DEE_A_OUT_OPT Dee_size_t *offset) {
 DeeTypeObject *result;
 DEE_ASSERT(DeeObject_Check(attr) && DeeString_Check(attr));
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(self));
 if (DeeStructType_Check(self)) {
  if ((result = DeeStructType_GetField(self,attr,offset)) != NULL) return result;
 }
 return _DeeType_GenericOffsetOfString(self,DeeString_STR(attr),offset);
}
DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_EXCEPT(NULL) DeeTypeObject *DeeType_OffsetofItemIndex(
 DEE_A_IN DeeTypeObject const *self, DEE_A_IN Dee_size_t index, DEE_A_OUT_OPT Dee_size_t *offset) {
 if (DeeArrayType_Check(self)) {
  // Array type
  *offset = index*DeeArrayType_SELEM(self);
  return DeeArrayType_BASE(self);
 }
 DeeError_IndexError_T(self,index);
 return NULL;
}
DEE_ATTRIBUTE_ADD_RESULT_REFERENCE DEE_A_RET_EXCEPT(NULL) DeeTypeObject *DeeType_OffsetofItem(
 DEE_A_IN DeeTypeObject const *self, DEE_A_INOUT DeeObject *index, DEE_A_OUT_OPT Dee_size_t *offset) {
 Dee_size_t used_index;
 if (DeeObject_Cast(Dee_size_t,index,&used_index) != 0) return NULL;
 return DeeType_OffsetofItemIndex(self,used_index,offset);
}




//////////////////////////////////////////////////////////////////////////
// Virtual type functions
static DeeObject *_deetype_tp_str(DeeTypeObject *self) {
 if (!DeeType_NAME(self)) DeeReturn_STATIC_STRING("<unnamed>");
 return DeeString_New(DeeType_NAME(self));
}
static DeeObject *_deetype_tp_repr(DeeTypeObject *self) {
 // TODO: Add more detail here
 return _deetype_tp_str(self);
}
DEE_VISIT_PROC(_deetype_tp_visit,DeeTypeObject *self) {
 // v small optimization: No need to visit the base it we're not a heap type.
 if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_HEAP_TYPE)!=0) {
  Dee_XVISIT(DeeType_BASE(self));
 }
}
DEE_STATIC_INLINE(void) _DeeType_InitCommon(DeeTypeObject *self) {
 DeeType_GET_SLOT(self,tp_instance_size) = DeeType_DEFAULT_SLOT(tp_instance_size);
 DeeType_GET_SLOT(self,tp_alloc) = DeeType_DEFAULT_SLOT(tp_alloc);
 DeeType_GET_SLOT(self,tp_ctor) = DeeType_DEFAULT_SLOT(tp_ctor);
 DeeType_GET_SLOT(self,tp_copy_ctor) = DeeType_DEFAULT_SLOT(tp_copy_ctor);
 DeeType_GET_SLOT(self,tp_move_ctor) = DeeType_DEFAULT_SLOT(tp_move_ctor);
 DeeType_GET_SLOT(self,tp_any_ctor) = DeeType_DEFAULT_SLOT(tp_any_ctor);
 DeeType_GET_SLOT(self,tp_free) = DeeType_DEFAULT_SLOT(tp_free);
 DeeType_GET_SLOT(self,tp_dtor) = DeeType_DEFAULT_SLOT(tp_dtor);
 DeeType_GET_SLOT(self,tp_copy_assign) = DeeType_DEFAULT_SLOT(tp_copy_assign);
 DeeType_GET_SLOT(self,tp_move_assign) = DeeType_DEFAULT_SLOT(tp_move_assign);
 DeeType_GET_SLOT(self,tp_any_assign) = DeeType_DEFAULT_SLOT(tp_any_assign);
 DeeType_GET_SLOT(self,tp_str) = DeeType_DEFAULT_SLOT(tp_str);
 DeeType_GET_SLOT(self,tp_repr) = DeeType_DEFAULT_SLOT(tp_repr);
 DeeType_GET_SLOT(self,tp_int32) = DeeType_DEFAULT_SLOT(tp_int32);
 DeeType_GET_SLOT(self,tp_int64) = DeeType_DEFAULT_SLOT(tp_int64);
 DeeType_GET_SLOT(self,tp_double) = DeeType_DEFAULT_SLOT(tp_double);
 DeeType_GET_SLOT(self,tp_call) = DeeType_DEFAULT_SLOT(tp_call);
 DeeType_GET_SLOT(self,tp_visit) = DeeType_DEFAULT_SLOT(tp_visit);
 DeeType_GET_SLOT(self,tp_clear) = DeeType_DEFAULT_SLOT(tp_clear);
 DeeType_GET_SLOT(self,tp_not) = DeeType_DEFAULT_SLOT(tp_not);
 DeeType_GET_SLOT(self,tp_bool) = DeeType_DEFAULT_SLOT(tp_bool);
 DeeType_GET_SLOT(self,tp_inv) = DeeType_DEFAULT_SLOT(tp_inv);
 DeeType_GET_SLOT(self,tp_pos) = DeeType_DEFAULT_SLOT(tp_pos);
 DeeType_GET_SLOT(self,tp_neg) = DeeType_DEFAULT_SLOT(tp_neg);
 DeeType_GET_SLOT(self,tp_inc) = DeeType_DEFAULT_SLOT(tp_inc);
 DeeType_GET_SLOT(self,tp_incpost) = DeeType_DEFAULT_SLOT(tp_incpost);
 DeeType_GET_SLOT(self,tp_dec) = DeeType_DEFAULT_SLOT(tp_dec);
 DeeType_GET_SLOT(self,tp_decpost) = DeeType_DEFAULT_SLOT(tp_decpost);
 DeeType_GET_SLOT(self,tp_add) = DeeType_DEFAULT_SLOT(tp_add);
 DeeType_GET_SLOT(self,tp_iadd) = DeeType_DEFAULT_SLOT(tp_iadd);
 DeeType_GET_SLOT(self,tp_sub) = DeeType_DEFAULT_SLOT(tp_sub);
 DeeType_GET_SLOT(self,tp_isub) = DeeType_DEFAULT_SLOT(tp_isub);
 DeeType_GET_SLOT(self,tp_mul) = DeeType_DEFAULT_SLOT(tp_mul);
 DeeType_GET_SLOT(self,tp_imul) = DeeType_DEFAULT_SLOT(tp_imul);
 DeeType_GET_SLOT(self,tp_div) = DeeType_DEFAULT_SLOT(tp_div);
 DeeType_GET_SLOT(self,tp_idiv) = DeeType_DEFAULT_SLOT(tp_idiv);
 DeeType_GET_SLOT(self,tp_mod) = DeeType_DEFAULT_SLOT(tp_mod);
 DeeType_GET_SLOT(self,tp_imod) = DeeType_DEFAULT_SLOT(tp_imod);
 DeeType_GET_SLOT(self,tp_shl) = DeeType_DEFAULT_SLOT(tp_shl);
 DeeType_GET_SLOT(self,tp_ishl) = DeeType_DEFAULT_SLOT(tp_ishl);
 DeeType_GET_SLOT(self,tp_shr) = DeeType_DEFAULT_SLOT(tp_shr);
 DeeType_GET_SLOT(self,tp_ishr) = DeeType_DEFAULT_SLOT(tp_ishr);
 DeeType_GET_SLOT(self,tp_and) = DeeType_DEFAULT_SLOT(tp_and);
 DeeType_GET_SLOT(self,tp_iand) = DeeType_DEFAULT_SLOT(tp_iand);
 DeeType_GET_SLOT(self,tp_or) = DeeType_DEFAULT_SLOT(tp_or);
 DeeType_GET_SLOT(self,tp_ior) = DeeType_DEFAULT_SLOT(tp_ior);
 DeeType_GET_SLOT(self,tp_xor) = DeeType_DEFAULT_SLOT(tp_xor);
 DeeType_GET_SLOT(self,tp_ixor) = DeeType_DEFAULT_SLOT(tp_ixor);
 DeeType_GET_SLOT(self,tp_pow) = DeeType_DEFAULT_SLOT(tp_pow);
 DeeType_GET_SLOT(self,tp_ipow) = DeeType_DEFAULT_SLOT(tp_ipow);
 DeeType_GET_SLOT(self,tp_hash) = DeeType_DEFAULT_SLOT(tp_hash);
 DeeType_GET_SLOT(self,tp_cmp_lo) = DeeType_DEFAULT_SLOT(tp_cmp_lo);
 DeeType_GET_SLOT(self,tp_cmp_le) = DeeType_DEFAULT_SLOT(tp_cmp_le);
 DeeType_GET_SLOT(self,tp_cmp_eq) = DeeType_DEFAULT_SLOT(tp_cmp_eq);
 DeeType_GET_SLOT(self,tp_cmp_ne) = DeeType_DEFAULT_SLOT(tp_cmp_ne);
 DeeType_GET_SLOT(self,tp_cmp_gr) = DeeType_DEFAULT_SLOT(tp_cmp_gr);
 DeeType_GET_SLOT(self,tp_cmp_ge) = DeeType_DEFAULT_SLOT(tp_cmp_ge);
 DeeType_GET_SLOT(self,tp_seq_get) = DeeType_DEFAULT_SLOT(tp_seq_get);
 DeeType_GET_SLOT(self,tp_seq_del) = DeeType_DEFAULT_SLOT(tp_seq_del);
 DeeType_GET_SLOT(self,tp_seq_set) = DeeType_DEFAULT_SLOT(tp_seq_set);
 DeeType_GET_SLOT(self,tp_seq_size) = DeeType_DEFAULT_SLOT(tp_seq_size);
 DeeType_GET_SLOT(self,tp_seq_contains) = DeeType_DEFAULT_SLOT(tp_seq_contains);
 DeeType_GET_SLOT(self,tp_seq_range_get) = DeeType_DEFAULT_SLOT(tp_seq_range_get);
 DeeType_GET_SLOT(self,tp_seq_range_del) = DeeType_DEFAULT_SLOT(tp_seq_range_del);
 DeeType_GET_SLOT(self,tp_seq_range_set) = DeeType_DEFAULT_SLOT(tp_seq_range_set);
 DeeType_GET_SLOT(self,tp_seq_iter_self) = DeeType_DEFAULT_SLOT(tp_seq_iter_self);
 DeeType_GET_SLOT(self,tp_seq_iter_next) = DeeType_DEFAULT_SLOT(tp_seq_iter_next);
 DeeType_GET_SLOT(self,tp_attr_get) = DeeType_DEFAULT_SLOT(tp_attr_get);
 DeeType_GET_SLOT(self,tp_attr_del) = DeeType_DEFAULT_SLOT(tp_attr_del);
 DeeType_GET_SLOT(self,tp_attr_set) = DeeType_DEFAULT_SLOT(tp_attr_set);
 DeeType_GET_SLOT(self,tp_members) = DeeType_DEFAULT_SLOT(tp_members);
 DeeType_GET_SLOT(self,tp_getsets) = DeeType_DEFAULT_SLOT(tp_getsets);
 DeeType_GET_SLOT(self,tp_methods) = DeeType_DEFAULT_SLOT(tp_methods);
 DeeType_GET_SLOT(self,tp_class_members) = DeeType_DEFAULT_SLOT(tp_class_members);
 DeeType_GET_SLOT(self,tp_class_getsets) = DeeType_DEFAULT_SLOT(tp_class_getsets);
 DeeType_GET_SLOT(self,tp_class_methods) = DeeType_DEFAULT_SLOT(tp_class_methods);
 DeeType_GET_SLOT(self,tp_marshal) = DeeType_DEFAULT_SLOT(tp_marshal);
}
static int _deetype_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTypeObject *self) {
 DeeType_GET_SLOT(self,tp_name) = DeeType_DEFAULT_SLOT(tp_name);
 DeeType_GET_SLOT(self,tp_doc) = DeeType_DEFAULT_SLOT(tp_doc);
 DeeType_GET_SLOT(self,tp_flags) = 
  DEE_TYPE_FLAG_STATIC_NAME|DEE_TYPE_FLAG_STATIC_DOC|DEE_TYPE_FLAG_HEAP_TYPE;
 Dee_INCREF(DeeType_GET_SLOT(self,tp_base) = DeeType_DEFAULT_SLOT(tp_base));
 _DeeType_InitCommon(self);
 return 0;
}
static int _deetype_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTypeObject *self, DeeTypeObject *right) {
 // Start by copying everything
 memcpy(((DeeObject *)self)+1,((DeeObject *)right)+1,
        sizeof(DeeTypeObject)-sizeof(DeeObject));
 DeeType_GET_SLOT(self,tp_flags) |= DEE_TYPE_FLAG_HEAP_TYPE; // This is a heap type
 if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_STATIC_NAME)==0) { // Must copy the name
  if ((DeeType_GET_SLOT(self,tp_name) = strdup_nz(DeeType_GET_SLOT(self,tp_name))) == NULL) return -1;
 }
 if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_STATIC_DOC)==0) { // Must copy the doc
  if ((DeeType_GET_SLOT(self,tp_doc) = strdup_nz(DeeType_GET_SLOT(self,tp_doc))) == NULL) {
   if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_STATIC_NAME)==0)
    free_nn((void *)DeeType_GET_SLOT(self,tp_name));
   return -1;
  }
 }
 // v really, really likely to be non-NULL
 Dee_XINCREF(DeeType_GET_SLOT(self,tp_base));
 return 0;
}
static int _deetype_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTypeObject *self, DeeTypeObject *right) {
 // Start by copying everything
 memcpy(((DeeObject *)self)+1,((DeeObject *)right)+1,
        sizeof(DeeTypeObject)-sizeof(DeeObject));
 DeeType_GET_SLOT(self,tp_flags) |= DEE_TYPE_FLAG_HEAP_TYPE; // This is a heap type
 if ((DeeType_FLAGS(right)&DEE_TYPE_FLAG_STATIC_NAME)==0) {
  DeeType_FLAGS(right) |= DEE_TYPE_FLAG_STATIC_NAME;
  DeeType_DEL_SLOT(right,tp_name); // Steal the name
 }
 if ((DeeType_FLAGS(right)&DEE_TYPE_FLAG_STATIC_DOC)==0) {
  DeeType_FLAGS(right) |= DEE_TYPE_FLAG_STATIC_DOC;
  DeeType_DEL_SLOT(right,tp_doc); // Steal the doc
 }
 // v really, really likely to be non-NULL
 Dee_XINCREF(DeeType_GET_SLOT(self,tp_base));
 return 0;
}
static int _deetype_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTypeObject *self, DeeObject *args) {
 DeeObject *name = Dee_None,*doc = Dee_None;
 DeeTypeObject *base = &DeeObject_Type;
 if (DeeTuple_Unpack(args,"|ooo:type",&name,&base,&doc) != 0) return -1;
 if (DeeNone_Check(base)) base = &DeeObject_Type; else
 if (DeeObject_InplaceGetInstance(&base,&DeeType_Type) != 0) return -1;
 DeeType_GET_SLOT(self,tp_flags) = DEE_TYPE_FLAG_HEAP_TYPE; // This is a heap type
 if (!DeeNone_Check(name)) {
  if (DeeError_TypeError_CheckTypeExact((DeeObject *)name,&DeeString_Type) != 0) return -1;
  if (!*DeeString_STR(name)) goto def_name;
  if ((DeeType_GET_SLOT(self,tp_name) = strdup_nz(DeeString_STR(name))) == NULL) goto err_name;
 } else {
def_name:
  DeeType_GET_SLOT(self,tp_name) = DeeType_DEFAULT_SLOT(tp_name);
  DeeType_GET_SLOT(self,tp_flags) |= DEE_TYPE_FLAG_STATIC_NAME;
 }
 if (!DeeNone_Check(doc)) {
  if (DeeError_TypeError_CheckTypeExact((DeeObject *)name,&DeeString_Type) != 0) {
err_name:
   if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_STATIC_NAME)==0)
    free_nn((void *)DeeType_GET_SLOT(self,tp_name));
   return -1;
  }
  if (!*DeeString_STR(doc)) goto def_doc;
  if ((DeeType_GET_SLOT(self,tp_doc) = strdup_nz(DeeString_STR(doc))) == NULL) goto err_name;
 } else {
def_doc:
  DeeType_GET_SLOT(self,tp_doc) = DeeType_DEFAULT_SLOT(tp_doc);
  DeeType_GET_SLOT(self,tp_flags) |= DEE_TYPE_FLAG_STATIC_DOC;
 }
 Dee_INCREF(DeeType_GET_SLOT(self,tp_base) = base);
 _DeeType_InitCommon(self);
 return 0;
}
static void _deetype_tp_dtor(DeeTypeObject *self) {
 DEE_ASSERT((DeeType_FLAGS(self)&DEE_TYPE_FLAG_HEAP_TYPE)!=0 &&
            "Destroying non-heap type");
 if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_REGISTERED_MARSHAL)!=0)
  DeeMarshal_UnregisterType(self);
 if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_STATIC_NAME)==0)
  free_nn((void *)DeeType_GET_SLOT(self,tp_name));
 if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_STATIC_DOC)==0)
  free_nn((void *)DeeType_GET_SLOT(self,tp_doc));
 Dee_XDECREF(DeeType_BASE(self));
}
#if DEE_DEVEL_BUILD
static DeeObject *_deetype___set_name(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *name;
 if (DeeTuple_Unpack(args,"o:__set_name",&name) != 0) return NULL;
 if (DeeError_TypeError_CheckTypeExact(name,&DeeString_Type) != 0) return NULL;
 if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_HEAP_TYPE)==0) {
  DeeError_SetStringf(&DeeErrorType_ValueError,"Cannot rename non-heap type %k",self);
  return NULL;
 }
 if (DeeType_SetName(self,DeeString_STR(name)) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deetype___set_doc(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *doc;
 if (DeeTuple_Unpack(args,"o:__set_doc",&doc) != 0) return NULL;
 if (DeeError_TypeError_CheckTypeExact(doc,&DeeString_Type) != 0) return NULL;
 if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_HEAP_TYPE)==0) {
  DeeError_SetStringf(&DeeErrorType_ValueError,"Cannot set doc of non-heap type %k",self);
  return NULL;
 }
 if (DeeType_SetDoc(self,DeeString_STR(doc)) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deetype___set_base(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeTypeObject *base;
 if (DeeTuple_Unpack(args,"o:__set_base",&base) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&base,&DeeType_Type) != 0) return NULL;
 if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_HEAP_TYPE)==0) {
  DeeError_SetStringf(&DeeErrorType_ValueError,"Cannot set base of non-heap type %k",self);
  return NULL;
 }
 Dee_DECREF(DeeType_GET_SLOT(self,tp_base));
 Dee_INCREF(DeeType_GET_SLOT(self,tp_base) = base);
 DeeReturn_None;
}
#endif

static DeeObject *_deetype_derived_from(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeTypeObject *base;
 if (DeeTuple_Unpack(args,"o:derived_from",&base) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&base,&DeeType_Type) != 0) return NULL;
 DeeReturn_Bool(DeeType_IsDerived(self,base));
}
static DeeObject *_deetype_same_or_derived_from(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeTypeObject *base;
 if (DeeTuple_Unpack(args,"o:same_or_derived_from",&base) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&base,&DeeType_Type) != 0) return NULL;
 DeeReturn_Bool(DeeType_IsSameOrDerived(self,base));
}
static DeeObject *_deetype_is_vartype(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_vartype") != 0) return NULL;
 DeeReturn_Bool(DeeType_IsVarType(self));
}
static DeeObject *_deetype_is_heaptype(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_heaptype") != 0) return NULL;
 DeeReturn_Bool(DeeType_IsHeapType(self));
}
static DeeObject *_deetype_is_gctype(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_gctype") != 0) return NULL;
 DeeReturn_Bool(DeeType_IsGCType(self));
}
static DeeObject *_deetype_is_final(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_final") != 0) return NULL;
 DeeReturn_Bool(DeeType_IsFinal(self));
}
static DeeObject *_deetype_is_class(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_class") != 0) return NULL;
 DeeReturn_Bool(DeeType_IsClass(self));
}
static DeeObject *_deetype_is_complete(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_complete") != 0) return NULL;
 DeeReturn_Bool(DeeType_IsComplete(self));
}
static DeeObject *_deetype_is_classtype(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_classtype") != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES
 DeeReturn_Bool(DeeClassType_Check(self));
#else /* DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES */
 (void)self; DeeReturn_False;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_CLASS_TYPES */
}
static DeeObject *_deetype_is_pointer(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_pointer") != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeReturn_Bool(DeePointerType_Check(self));
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 (void)self; DeeReturn_False;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
}
static DeeObject *_deetype_is_lvalue(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_lvalue") != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 DeeReturn_Bool(DeeLValueType_Check(self));
#else /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 (void)self; DeeReturn_False;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
}
static DeeObject *_deetype_is_structured(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_structured") != 0) return NULL;
 DeeReturn_Bool(DeeStructuredType_Check(self));
}
static DeeObject *_deetype_is_struct(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_struct") != 0) return NULL;
 DeeReturn_Bool(DeeStructType_Check(self));
}
static DeeObject *_deetype_is_array(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_array") != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_ARRAYS
 DeeReturn_Bool(DeeArrayType_Check(self));
#else /* DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
 (void)self; DeeReturn_False;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_ARRAYS */
}
static DeeObject *_deetype_is_foreign_function(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_foreign_function") != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
 DeeReturn_Bool(DeeForeignFunctionType_Check(self));
#else /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
 (void)self; DeeReturn_False;
#endif /* !DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
}
static DeeObject *_deetype_is_file(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_file") != 0) return NULL;
 DeeReturn_Bool(DeeFileType_Check(self));
}
static DeeObject *_deetype_is_super_base(
 DeeTypeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":is_super_base") != 0) return NULL;
 DeeReturn_Bool(DeeType_IsSuperBase(self));
}

static struct DeeMemberDef const _deetype_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__name__",DeeTypeObject,tp_name,string),
 DEE_MEMBERDEF_NAMED_RO_v100("__doc__",DeeTypeObject,tp_doc,string),
 DEE_MEMBERDEF_NAMED_RO_v100("__base__",DeeTypeObject,tp_base,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__instance_size__",DeeTypeObject,tp_constructor.tp_instance_size,Dee_size_t),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__tp_name",DeeTypeObject,tp_name,string),
 DEE_MEMBERDEF_NAMED_RO_v100("__tp_doc",DeeTypeObject,tp_doc,string),
 DEE_MEMBERDEF_NAMED_RO_v100("__tp_flags",DeeTypeObject,tp_flags,Dee_uint32_t),
 DEE_MEMBERDEF_NAMED_RO_v100("__tp_base",DeeTypeObject,tp_base,object_null),
 DEE_MEMBERDEF_NAMED_RO_v100("__tp_instance_size",DeeTypeObject,tp_constructor.tp_instance_size,Dee_size_t),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};

static struct DeeMemberDef const _deetype_tp_class_members[] = {
 DEE_MEMBERDEF_CONST_v100("FLAG_VAR_OBJECT",Dee_uint32_t,DEE_TYPE_FLAG_VAR_OBJECT),
 DEE_MEMBERDEF_CONST_v100("FLAG_STATIC_NAME",Dee_uint32_t,DEE_TYPE_FLAG_STATIC_NAME),
 DEE_MEMBERDEF_CONST_v100("FLAG_STATIC_DOC",Dee_uint32_t,DEE_TYPE_FLAG_STATIC_DOC),
 DEE_MEMBERDEF_CONST_v100("FLAG_HEAP_TYPE",Dee_uint32_t,DEE_TYPE_FLAG_HEAP_TYPE),
 DEE_MEMBERDEF_CONST_v100("FLAG_MUST_COPY",Dee_uint32_t,DEE_TYPE_FLAG_MUST_COPY),
 DEE_MEMBERDEF_CONST_v100("FLAG_HAS_GC",Dee_uint32_t,DEE_TYPE_FLAG_HAS_GC),
 DEE_MEMBERDEF_CONST_v100("FLAG_CONST_CTOR",Dee_uint32_t,DEE_TYPE_FLAG_CONST_CTOR),
 DEE_MEMBERDEF_CONST_v100("FLAG_NO_SUBCLASS",Dee_uint32_t,DEE_TYPE_FLAG_NO_SUBCLASS),
 DEE_MEMBERDEF_CONST_v100("FLAG_ALWAYS_CLEAR",Dee_uint32_t,DEE_TYPE_FLAG_ALWAYS_CLEAR),
 DEE_MEMBERDEF_CONST_v100("FLAG_DONT_COPY",Dee_uint32_t,DEE_TYPE_FLAG_DONT_COPY),
 DEE_MEMBERDEF_CONST_v100("FLAG_NO_LIFETIME_EFFECT",Dee_uint32_t,DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),
 DEE_MEMBERDEF_CONST_v100("FLAG_CLASS_TYPE",Dee_uint32_t,DEE_TYPE_FLAG_CLASS_TYPE),
 DEE_MEMBERDEF_CONST_v100("FLAG_NO_ITERATE_EFFECT",Dee_uint32_t,DEE_TYPE_FLAG_NO_ITERATE_EFFECT),
 DEE_MEMBERDEF_CONST_v100("FLAG_INCOMPLETE",Dee_uint32_t,DEE_TYPE_FLAG_INCOMPLETE),
 DEE_MEMBERDEF_END_v100
};

static struct DeeMethodDef const _deetype_tp_methods[] = {
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_METHODDEF_v100("__set_name",member(&_deetype___set_name),"(string name) -> none"),
 DEE_METHODDEF_v100("__set_doc",member(&_deetype___set_doc),"(string name) -> none"),
 DEE_METHODDEF_v100("__set_base",member(&_deetype___set_base),"(type object tp) -> none"),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_METHODDEF_v100("derived_from",member(&_deetype_derived_from),"(type object tp) -> bool\n@return: True if @this is derived from @tp"),
 DEE_METHODDEF_v100("same_or_derived_from",member(&_deetype_same_or_derived_from),"(type object tp) -> bool\n@return: True if @this equals @tp, or is derived from it"),
 DEE_METHODDEF_v100("is_vartype",member(&_deetype_is_vartype),"() -> bool\n@return: True if instance of @this have variable size (e.g.: #tuple)"),
 DEE_METHODDEF_v100("is_heaptype",member(&_deetype_is_heaptype),"() -> bool\n@return: True if @this is allocated on the heap (aka. a runtime/dynamic type)"),
 DEE_METHODDEF_v100("is_gctype",member(&_deetype_is_gctype),"() -> bool\n@return: True if @this is a gc-type"),
 DEE_METHODDEF_v100("is_final",member(&_deetype_is_final),"() -> bool\n@return: True if @this cannot be used as base for other types"),
 DEE_METHODDEF_v100("is_class",member(&_deetype_is_class),"() -> bool\n@return: True if @this is a class"),
 DEE_METHODDEF_v100("is_complete",member(&_deetype_is_complete),"() -> bool\n@return: True if @this is a complete type"),
 DEE_METHODDEF_v100("is_classtype",member(&_deetype_is_classtype),"() -> bool\n@return: True if @this is a #class_type"),
 DEE_METHODDEF_v100("is_pointer",member(&_deetype_is_pointer),"() -> bool\n@return: True if @this is a #pointer_type"),
 DEE_METHODDEF_v100("is_lvalue",member(&_deetype_is_lvalue),"() -> bool\n@return: True if @this is an #lvalue_type"),
 DEE_METHODDEF_v100("is_structured",member(&_deetype_is_structured),"() -> bool\n@return: True if @this is a #structured_type"),
 DEE_METHODDEF_v100("is_struct",member(&_deetype_is_struct),"() -> bool\n@return: True if @this is a #struct_type"),
 DEE_METHODDEF_v100("is_array",member(&_deetype_is_array),"() -> bool\n@return: True if @this is an #array_type"),
 DEE_METHODDEF_v100("is_foreign_function",member(&_deetype_is_foreign_function),"() -> bool\n@return: True if @this is a #foreign_function_type"),
 DEE_METHODDEF_v100("is_file",member(&_deetype_is_file),"() -> bool\n@return: True if @this is a #file_type"),
 DEE_METHODDEF_v100("is_super_base",member(&_deetype_is_super_base),
  "() -> bool\n"
  "\tA super-base type is a type that doesn't have a base itself.\n"
  "\tSuch a type must implement all operators described by #(type object) "
    "and must allow them to be called with any instance of any of its "
    "sub-classes, yielding meaningful results (such as throwing a Not-Implemented error).\n"),
 DEE_METHODDEF_END_v100
};


#if DEE_XCONFIG_HAVE_TYPE_SLOT_MEMBERS
// Make the actual type operators available in developer builds
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DEE_TYPESLOT_GETSET(name,tystring) \
static DeeObject *_deetype_##name##_get(DeeTypeObject *self, void *DEE_UNUSED(closure))\
{\
 return DeePointer_Newf((void *)DeeType_GET_SLOT(self,name),tystring);\
}\
static int _deetype_##name##_del(DeeTypeObject *self, void *DEE_UNUSED(closure))\
{\
 DeeType_DEL_SLOT(self,name);\
 return 0;\
}\
static int _deetype_##name##_set(DeeTypeObject *self, DeeObject *v, void *DEE_UNUSED(closure))\
{\
 DeeType_SLOT_TYPE(name) new_v;\
 if (DeeObject_GetPointerExf(v,(void **)&new_v,tystring) != 0) return -1;\
 DeeType_GET_SLOT(self,name) = new_v;\
 return 0;\
}
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
DEE_COMPILER_MSVC_WARNING_PUSH(4052 4054)
DEE_TYPESLOT_GETSET(tp_alloc,"o(o)")
DEE_TYPESLOT_GETSET(tp_free,"v(oo)")
DEE_TYPESLOT_GETSET(tp_ctor,"i(oo)")
DEE_TYPESLOT_GETSET(tp_copy_ctor,"i(ooo)")
DEE_TYPESLOT_GETSET(tp_move_ctor,"i(ooo)")
DEE_TYPESLOT_GETSET(tp_any_ctor,"i(ooo)")
DEE_TYPESLOT_GETSET(tp_new,"o(o)")
DEE_TYPESLOT_GETSET(tp_copy_new,"o(oo)")
DEE_TYPESLOT_GETSET(tp_move_new,"o(oo)")
DEE_TYPESLOT_GETSET(tp_any_new,"o(oo)")
DEE_TYPESLOT_GETSET(tp_dtor,"v(o)")
DEE_TYPESLOT_GETSET(tp_copy_assign,"i(oo)")
DEE_TYPESLOT_GETSET(tp_move_assign,"i(oo)")
DEE_TYPESLOT_GETSET(tp_any_assign,"i(oo)")
DEE_TYPESLOT_GETSET(tp_str,"o(o)")
DEE_TYPESLOT_GETSET(tp_repr,"o(o)")
DEE_TYPESLOT_GETSET(tp_int32,"i(oI32d*)")
DEE_TYPESLOT_GETSET(tp_int64,"i(oI64d*)")
DEE_TYPESLOT_GETSET(tp_double,"i(oD*)")
DEE_TYPESLOT_GETSET(tp_call,"i(oo)")
DEE_TYPESLOT_GETSET(tp_visit,"i(ood(op)*p)")
DEE_TYPESLOT_GETSET(tp_clear,"v(o)")
DEE_TYPESLOT_GETSET(tp_not,"o(o)")
DEE_TYPESLOT_GETSET(tp_bool,"i(o)")
DEE_TYPESLOT_GETSET(tp_inv,"o(o)")
DEE_TYPESLOT_GETSET(tp_pos,"o(o)")
DEE_TYPESLOT_GETSET(tp_neg,"o(o)")
DEE_TYPESLOT_GETSET(tp_inc,"o(o)")
DEE_TYPESLOT_GETSET(tp_incpost,"o(o)")
DEE_TYPESLOT_GETSET(tp_dec,"o(o)")
DEE_TYPESLOT_GETSET(tp_decpost,"o(o)")
DEE_TYPESLOT_GETSET(tp_add,"o(oo)")
DEE_TYPESLOT_GETSET(tp_iadd,"o(oo)")
DEE_TYPESLOT_GETSET(tp_sub,"o(oo)")
DEE_TYPESLOT_GETSET(tp_isub,"o(oo)")
DEE_TYPESLOT_GETSET(tp_mul,"o(oo)")
DEE_TYPESLOT_GETSET(tp_imul,"o(oo)")
DEE_TYPESLOT_GETSET(tp_div,"o(oo)")
DEE_TYPESLOT_GETSET(tp_idiv,"o(oo)")
DEE_TYPESLOT_GETSET(tp_mod,"o(oo)")
DEE_TYPESLOT_GETSET(tp_imod,"o(oo)")
DEE_TYPESLOT_GETSET(tp_shl,"o(oo)")
DEE_TYPESLOT_GETSET(tp_ishl,"o(oo)")
DEE_TYPESLOT_GETSET(tp_shr,"o(oo)")
DEE_TYPESLOT_GETSET(tp_ishr,"o(oo)")
DEE_TYPESLOT_GETSET(tp_and,"o(oo)")
DEE_TYPESLOT_GETSET(tp_iand,"o(oo)")
DEE_TYPESLOT_GETSET(tp_or,"o(oo)")
DEE_TYPESLOT_GETSET(tp_ior,"o(oo)")
DEE_TYPESLOT_GETSET(tp_xor,"o(oo)")
DEE_TYPESLOT_GETSET(tp_ixor,"o(oo)")
DEE_TYPESLOT_GETSET(tp_pow,"o(oo)")
DEE_TYPESLOT_GETSET(tp_ipow,"o(oo)")
DEE_TYPESLOT_GETSET(tp_hash,"i(oI"
 DEE_PP_STR(DEE_PP_MUL8(DEE_CONFIG_SIZEOF_DEE_HASH_T)) "uI"
 DEE_PP_STR(DEE_PP_MUL8(DEE_CONFIG_SIZEOF_DEE_HASH_T)) "u*)")
DEE_TYPESLOT_GETSET(tp_cmp_lo,"o(oo)")
DEE_TYPESLOT_GETSET(tp_cmp_le,"o(oo)")
DEE_TYPESLOT_GETSET(tp_cmp_eq,"o(oo)")
DEE_TYPESLOT_GETSET(tp_cmp_ne,"o(oo)")
DEE_TYPESLOT_GETSET(tp_cmp_gr,"o(oo)")
DEE_TYPESLOT_GETSET(tp_cmp_ge,"o(oo)")
DEE_TYPESLOT_GETSET(tp_seq_get,"o(oo)")
DEE_TYPESLOT_GETSET(tp_seq_del,"i(oo)")
DEE_TYPESLOT_GETSET(tp_seq_set,"i(ooo)")
DEE_TYPESLOT_GETSET(tp_seq_size,"o(o)")
DEE_TYPESLOT_GETSET(tp_seq_contains,"o(oo)")
DEE_TYPESLOT_GETSET(tp_seq_range_get,"o(ooo)")
DEE_TYPESLOT_GETSET(tp_seq_range_del,"o(ooo)")
DEE_TYPESLOT_GETSET(tp_seq_range_set,"o(oooo)")
DEE_TYPESLOT_GETSET(tp_seq_iter_self,"o(o)")
DEE_TYPESLOT_GETSET(tp_seq_iter_next,"i(op*)") // 'p*' is actually a pointer to an object*, but there's no way of representing that
DEE_TYPESLOT_GETSET(tp_attr_get,"o(oo)")
DEE_TYPESLOT_GETSET(tp_attr_del,"i(oo)")
DEE_TYPESLOT_GETSET(tp_attr_set,"i(ooo)")
DEE_COMPILER_MSVC_WARNING_POP
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#undef DEE_TYPESLOT_GETSET
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
#endif /* DEE_XCONFIG_HAVE_TYPE_SLOT_MEMBERS */

#if DEE_XCONFIG_HAVE_TYPE_SLOT_MEMBERS
static struct DeeGetSetDef const _deetype_tp_getsets[] = {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
#define DEE_TYPESLOT_GETSET_REF(name,tydoc)\
 DEE_GETSETDEF_v100("__" #name,\
  member(&_deetype_##name##_get),\
  member(&_deetype_##name##_del),\
  member(&_deetype_##name##_set),\
  "-> " tydoc)
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION
 DEE_TYPESLOT_GETSET_REF(tp_alloc,"object(*)(object)"),
 DEE_TYPESLOT_GETSET_REF(tp_free,"none(*)(objec)"),
 DEE_TYPESLOT_GETSET_REF(tp_ctor,"int(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_copy_ctor,"int(*)(object,object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_move_ctor,"int(*)(object,object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_any_ctor,"int(*)(object,object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_new,"object(*)(object)"),
 DEE_TYPESLOT_GETSET_REF(tp_copy_new,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_move_new,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_any_new,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_dtor,"none(*)(object)"),
 DEE_TYPESLOT_GETSET_REF(tp_copy_assign,"int(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_move_assign,"int(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_any_assign,"int(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_str,"object(*)(object)"),
 DEE_TYPESLOT_GETSET_REF(tp_repr,"object(*)(object)"),
 DEE_TYPESLOT_GETSET_REF(tp_int32,"int(*)(object,int32_t *)"),
 DEE_TYPESLOT_GETSET_REF(tp_int64,"int(*)(object,int64_t *)"),
 DEE_TYPESLOT_GETSET_REF(tp_double,"int(*)(object,double *)"),
 DEE_TYPESLOT_GETSET_REF(tp_call,"int(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_visit,"int(*)(object,object,int(*)(object,none *),none *)"),
 DEE_TYPESLOT_GETSET_REF(tp_clear,"none(*)(object)"),
 DEE_TYPESLOT_GETSET_REF(tp_not,"object(*)(object)"),
 DEE_TYPESLOT_GETSET_REF(tp_bool,"int(*)(object)"),
 DEE_TYPESLOT_GETSET_REF(tp_inv,"object(*)(object)"),
 DEE_TYPESLOT_GETSET_REF(tp_pos,"object(*)(object)"),
 DEE_TYPESLOT_GETSET_REF(tp_neg,"object(*)(object)"),
 DEE_TYPESLOT_GETSET_REF(tp_inc,"object(*)(object)"),
 DEE_TYPESLOT_GETSET_REF(tp_incpost,"object(*)(object)"),
 DEE_TYPESLOT_GETSET_REF(tp_dec,"object(*)(object)"),
 DEE_TYPESLOT_GETSET_REF(tp_decpost,"object(*)(object)"),
 DEE_TYPESLOT_GETSET_REF(tp_add,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_iadd,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_sub,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_isub,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_mul,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_imul,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_div,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_idiv,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_mod,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_imod,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_shl,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_ishl,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_shr,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_ishr,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_and,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_iand,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_or,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_ior,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_xor,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_ixor,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_pow,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_ipow,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_hash,"int(*)(object,hash_t,hash_t *)"),
 DEE_TYPESLOT_GETSET_REF(tp_cmp_lo,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_cmp_le,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_cmp_eq,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_cmp_ne,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_cmp_gr,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_cmp_ge,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_seq_get,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_seq_del,"int(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_seq_set,"int(*)(object,object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_seq_size,"object(*)(object)"),
 DEE_TYPESLOT_GETSET_REF(tp_seq_contains,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_seq_range_get,"object(*)(object,object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_seq_range_del,"object(*)(object,object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_seq_range_set,"object(*)(object,object,object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_seq_iter_self,"object(*)(object)"),
 DEE_TYPESLOT_GETSET_REF(tp_seq_iter_next,"int(*)(object,none **)"),
 DEE_TYPESLOT_GETSET_REF(tp_attr_get,"object(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_attr_del,"int(*)(object,object)"),
 DEE_TYPESLOT_GETSET_REF(tp_attr_set,"int(*)(object,object,object)"),
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTION */
#undef DEE_TYPESLOT_GETSET_REF
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 DEE_GETSETDEF_END_v100
};
#else /* DEE_XCONFIG_HAVE_TYPE_SLOT_MEMBERS */
#define _deetype_tp_getsets DeeType_DEFAULT_SLOT(tp_getsets)
#endif /* !DEE_XCONFIG_HAVE_TYPE_SLOT_MEMBERS */


int DEE_CALL _deetype_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTypeObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_uint64_t read_size; Dee_size_t size;
 if (DeeFile_GetLe(infile,DeeType_GET_SLOT(self,tp_flags)) != 0) return -1;
 DeeType_GET_SLOT(self,tp_flags) |= DEE_TYPE_FLAG_HEAP_TYPE;
 if ((DeeType_GET_SLOT(self,tp_base) = (DeeTypeObject *)
  DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 if (DeeNone_Check(DeeType_GET_SLOT(self,tp_base))) Dee_CLEAR(DeeType_GET_SLOT(self,tp_base));
 else if (DeeError_TypeError_CheckType((DeeObject *)DeeType_GET_SLOT(self,tp_base),&DeeType_Type) != 0) {
err_base: Dee_DECREF(DeeType_GET_SLOT(self,tp_base)); return -1;
 }
 if ((DeeType_GET_SLOT(self,tp_flags)&DEE_TYPE_FLAG_STATIC_NAME)==0) {
  if (DeeFile_GetLeSmall64(infile,&read_size) != 0) goto err_base;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
  if DEE_UNLIKELY(read_size > (Dee_uint64_t)((Dee_size_t)-1)) {
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Type name is too long for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                       read_size,(Dee_size_t)-1);
   return -1;
  }
#endif
  if ((size = (Dee_size_t)read_size) != 0) {
   char *name;
   while ((name = (char *)malloc_nz((size+1)*sizeof(char))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
    goto err_base;
   }
   if (DeeFile_ReadAll(infile,name,size*sizeof(char)) != 0) { free_nn(name); goto err_base; }
   name[size] = 0;
   DeeType_GET_SLOT(self,tp_name) = name;
  } else {
   DeeType_GET_SLOT(self,tp_name) = DeeType_DEFAULT_SLOT(tp_name);
   DeeType_GET_SLOT(self,tp_flags) |= DEE_TYPE_FLAG_STATIC_NAME;
  }
 } else DeeType_GET_SLOT(self,tp_name) = DeeType_DEFAULT_SLOT(tp_name);
 if ((DeeType_GET_SLOT(self,tp_flags)&DEE_TYPE_FLAG_STATIC_DOC)==0) {
  if (DeeFile_GetLeSmall64(infile,&read_size) != 0) goto err_base;
#if DEE_TYPES_SIZEOF_SIZE_T < 8
  if DEE_UNLIKELY(read_size > (Dee_uint64_t)((Dee_size_t)-1)) {
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Type doc is too long for 32-bit CPU (%I64u exceeds maximum of %Iu)",
                       read_size,(Dee_size_t)-1);
   return -1;
  }
#endif
  if ((size = (Dee_size_t)read_size) != 0) {
   char *doc;
   while ((doc = (char *)malloc_nz((size+1)*sizeof(char))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
err_name:
    if ((DeeType_GET_SLOT(self,tp_flags)&DEE_TYPE_FLAG_STATIC_NAME)==0)
     free_nn((void *)DeeType_GET_SLOT(self,tp_name));
    goto err_base;
   }
   if (DeeFile_ReadAll(infile,doc,size*sizeof(char)) != 0) { free_nn(doc); goto err_name; }
   doc[size] = 0;
   DeeType_GET_SLOT(self,tp_doc) = doc;
  } else {
   DeeType_GET_SLOT(self,tp_doc) = DeeType_DEFAULT_SLOT(tp_doc);
   DeeType_GET_SLOT(self,tp_flags) |= DEE_TYPE_FLAG_STATIC_DOC;
  }
 } else {
  DeeType_GET_SLOT(self,tp_doc) = DeeType_DEFAULT_SLOT(tp_doc);
 }
 _DeeType_InitCommon(self);
 return 0;
}

int DEE_CALL _deetype_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTypeObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 Dee_size_t size; char const *name; Dee_uint32_t written_flags;
 written_flags = DeeType_FLAGS(self)&~(DEE_TYPE_FLAG_HEAP_TYPE);
 if (DeeFile_PutLe(outfile,written_flags) != 0) return -1;
 if (DeeMarshal_WriteObjectWithMap(outfile,map,DeeType_GET_SLOT(self,tp_base)
  ? (DeeObject *)DeeType_GET_SLOT(self,tp_base) : Dee_None) != 0) return -1;
 if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_STATIC_NAME)==0) {
  size = strlen(name = DeeType_GET_SLOT(self,tp_name));
  if (DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)size) != 0) return -1;
  if (DeeFile_WriteAll(outfile,name,size) != 0) return -1;
 }
 if ((DeeType_FLAGS(self)&DEE_TYPE_FLAG_STATIC_DOC)==0) {
  size = strlen(name = DeeType_GET_SLOT(self,tp_doc));
  if (DeeFile_PutLeSmall64(outfile,(Dee_uint64_t)size) != 0) return -1;
  if (DeeFile_WriteAll(outfile,name,size) != 0) return -1;
 }
 return 0;
}

static struct DeeTypeMarshal _deetype_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALID_NONE),
 member(&_deetype_tp_marshal_ctor),member(&_deetype_tp_marshal_put));

DeeTypeObject DeeType_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("type"),null,member(
  DEE_TYPE_FLAG_DONT_COPY|DEE_TYPE_FLAG_CONST_CTOR|
  DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_FUNDAMENTAL),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeTypeObject),null,
  member(&_deetype_tp_ctor),member(&_deetype_tp_copy_ctor),
  member(&_deetype_tp_move_ctor),member(&_deetype_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
  member(&_deetype_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deetype_tp_str),member(&_deetype_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(
  member(&DeeType_NewInstance),member(&_deetype_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,
  member(&_deegenericsameob_tp_hash)),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deegenericsameob_tp_cmp_lo),
  member(&_deegenericsameob_tp_cmp_le),
  member(&_deegenericsameob_tp_cmp_eq),
  member(&_deegenericsameob_tp_cmp_ne),
  member(&_deegenericsameob_tp_cmp_gr),
  member(&_deegenericsameob_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(
  member(&DeeType_GetAttr),member(&DeeType_DelAttr),member(&DeeType_SetAttr),
  member(_deetype_tp_members),member(_deetype_tp_getsets),member(_deetype_tp_methods),
  member(_deetype_tp_class_members),null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deetype_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};


#undef DeeType_Flags
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DeeType_SLOT_TYPE(tp_flags))
DeeType_Flags(DEE_A_IN DeeTypeObject const *self) {
 return DeeType_FLAGS(self);
}


DEE_DECL_END

#endif /* !GUARD_DEEMON_OBJECT_C */
