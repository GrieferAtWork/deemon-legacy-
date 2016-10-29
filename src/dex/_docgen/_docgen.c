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
#ifndef GUARD_DEEMON_DEX_DOCGEN_C
#define GUARD_DEEMON_DEX_DOCGEN_C 1
#define DEE_EXTENSION 1

//@ dex.name = _docgen

#include "_docgen.h"
#include <deemon/dex.h>
#include <deemon/super.h>
#include <deemon/deemonrun.h>
#include <deemon/generic_vtable.h>
#include <deemon/string.h>
#include <deemon/integer.h>
#include <deemon/bool.h>
#include <deemon/memberfunction.h>
#include <deemon/type.h>
#include <deemon/error.h>
#include <deemon/__conf.inl>
#include <deemon/class.h>
#include <deemon/struct.h>
#include <deemon/optional/object_visit.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/unicode/char_traits.inl>
#include <deemon/runtime/builtins.h>
#include DEE_INCLUDE_MEMORY_API()

#include "autodoc.h.inl"

DEE_DECL_BEGIN

DeeString_NEW_STATIC(_object_name,"object");
DeeString_NEW_STATIC(_unnamed_name,"<unnamed>");

struct DeeDoc *DeeDocs_UnsafeAppend(DEE_A_INOUT struct DeeDocs *self) {
 struct DeeDoc *new_docv;
 DEE_ASSERT(self);
 while DEE_UNLIKELY((new_docv = (struct DeeDoc *)realloc_nz(
  self->d_docv,(self->d_docc+1)*sizeof(struct DeeDoc))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 return (self->d_docv = new_docv)+self->d_docc++;
}

DEE_A_RET_EXCEPT(-1) int DeeDocs_AppendMethodDef(
 DEE_A_INOUT struct DeeDocs *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeMethodDef const *def) {
 struct DeeDoc *doc; int result;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(this_type) && DeeString_Check(this_type));
 DEE_ASSERT(def);
 if ((doc = DeeDocs_UnsafeAppend(self)) == NULL) return -1;
 doc->doc_kind = DEE_DOCKIND_METHOD;
 result = DeeMethodDoc_InitFromMethodDef(&doc->doc_method,flags,this_type,def);
 if (result != 0) DeeDocs_UnsafeRemove(self);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeDocs_AppendMemberDef(
 DEE_A_INOUT struct DeeDocs *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeMemberDef const *def) {
 struct DeeDoc *doc; int result;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(this_type) && DeeString_Check(this_type));
 DEE_ASSERT(def);
 if ((doc = DeeDocs_UnsafeAppend(self)) == NULL) return -1;
 doc->doc_kind = DEE_DOCKIND_MEMBER;
 result = DeeMemberDoc_InitFromMemberDef(&doc->doc_member,flags,this_type,def);
 if (result != 0) DeeDocs_UnsafeRemove(self);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeDocs_AppendGetSetDef(
 DEE_A_INOUT struct DeeDocs *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeGetSetDef const *def) {
 struct DeeDoc *doc; int result;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(this_type) && DeeString_Check(this_type));
 DEE_ASSERT(def);
 if ((doc = DeeDocs_UnsafeAppend(self)) == NULL) return -1;
 doc->doc_kind = DEE_DOCKIND_GETSET;
 result = DeeGetSetDoc_InitFromGetSetDef(&doc->doc_getset,flags,this_type,def);
 if (result != 0) DeeDocs_UnsafeRemove(self);
 return result;
}


DEE_A_RET_EXCEPT(-1) int DeeDocs_AppendMethodDefs(
 DEE_A_INOUT struct DeeDocs *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeMethodDef const *defs) {
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(this_type) && DeeString_Check(this_type));
 DEE_ASSERT(defs);
 while (defs->d_name) {
#ifdef DEE_DOC_IS_HIDDEN_NAME
  if (!DEE_DOC_IS_HIDDEN_NAME(defs->d_name))
#endif
  {
   if (DeeDocs_AppendMethodDef(self,flags,this_type,defs) != 0) return -1;
  }
  ++defs;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeDocs_AppendMemberDefs(
 DEE_A_INOUT struct DeeDocs *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeMemberDef const *defs) {
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(this_type) && DeeString_Check(this_type));
 DEE_ASSERT(defs);
 while (defs->d_name) {
#ifdef DEE_DOC_IS_HIDDEN_NAME
  if (!DEE_DOC_IS_HIDDEN_NAME(defs->d_name))
#endif
  {
   if (DeeDocs_AppendMemberDef(self,flags,this_type,defs) != 0) return -1;
  }
  ++defs;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeDocs_AppendGetSetDefs(
 DEE_A_INOUT struct DeeDocs *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeGetSetDef const *defs) {
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(this_type) && DeeString_Check(this_type));
 DEE_ASSERT(defs);
 while (defs->d_name) {
#ifdef DEE_DOC_IS_HIDDEN_NAME
  if (!DEE_DOC_IS_HIDDEN_NAME(defs->d_name))
#endif
  {
   if (DeeDocs_AppendGetSetDef(self,flags,this_type,defs) != 0) return -1;
  }
  ++defs;
 }
 return 0;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeDocs_Repr(DEE_A_IN struct DeeDocs const *self) {
 DeeObject *result; struct DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(self);
 if (DeeDocs_ReprToWriter(self,&writer) != 0)
  result = NULL;
 else result = DeeStringWriter_Pack(&writer);
 DeeStringWriter_Quit(&writer);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeDocs_ReprToWriter(
 DEE_A_IN struct DeeDocs const *self,
 DEE_A_INOUT struct DeeStringWriter *writer) {
 struct DeeDoc *iter,*end;
 DEE_ASSERT(self);
 DEE_ASSERT(writer);
 end = (iter = self->d_docv)+self->d_docc;
 while (iter != end) {
  if (DeeDoc_ReprToWriter(iter,writer) != 0) return -1;
  ++iter;
 }
 return 0;
}





//////////////////////////////////////////////////////////////////////////
// Initializer a throw doc from a given declaration string:
//           <--------------------------> decl_len
// >> @throws Error: Something went wrong
//           ^ <decl_str
DEE_A_RET_EXCEPT(-1) int DeeThrowDoc_InitFromDoc(
 DEE_A_OUT struct DeeThrowDoc *self, DEE_A_IN Dee_size_t decl_len,
 DEE_A_IN_R(decl_len) char const *decl_str) {
 char const *name_begin,*name_end,*text_begin,*text_end;
 DEE_ASSERT(self);
 DEE_ASSERT(!decl_len || decl_str);
 while (decl_len && DEE_CH_IS_SPACE(*decl_str)) ++decl_str,--decl_len;
 name_begin = decl_str;
 while (decl_len && *decl_str != ':') ++decl_str,--decl_len;
 name_end = decl_str;
 while (name_end != name_begin && DEE_CH_IS_SPACE(name_end[-1])) --name_end;
 while (decl_len && *decl_str == ':') ++decl_str,--decl_len;
 text_begin = decl_str;
 text_end = decl_str+decl_len;
 while (text_end != text_begin && DEE_CH_IS_SPACE(*text_begin)) ++text_begin;
 while (text_end != text_begin && DEE_CH_IS_SPACE(text_end[-1])) --text_end;
 if (name_begin == name_end) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Invalid @throws declaration: No exception name defined");
  return -1;
 }
 if ((self->td_name = (DeeStringObject *)DeeString_NewWithLength((
  Dee_size_t)(name_end-name_begin),name_begin)) == NULL) return -1;
 if (text_begin != text_end) {
  if ((self->td_doc = (DeeStringObject *)DeeString_NewWithLength((
   Dee_size_t)(text_end-text_begin),text_begin)) == NULL) {
   Dee_DECREF(self->td_name);
   return -1;
  }
 } else self->td_doc = NULL;
 return 0;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeThrowDoc_Repr(
 DEE_A_IN struct DeeThrowDoc const *self) {
 DeeObject *result; struct DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(self);
 if (DeeThrowDoc_ReprToWriter(self,&writer) != 0)
  result = NULL;
 else result = DeeStringWriter_Pack(&writer);
 DeeStringWriter_Quit(&writer);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeThrowDoc_ReprToWriter(
 DEE_A_IN struct DeeThrowDoc const *self,
 DEE_A_INOUT struct DeeStringWriter *writer) {
 DEE_ASSERT(self);
 DEE_ASSERT(writer);
 return self->td_doc
  ? DeeStringWriter_Writef(writer,"@throws %k: %k\n",self->td_name,self->td_doc)
  : DeeStringWriter_Writef(writer,"@throws %k\n",self->td_name);
}







struct DeeThrowDoc *DeeThrowDocs_UnsafeAppend(DEE_A_INOUT struct DeeThrowDocs *self) {
 struct DeeThrowDoc *new_throwv;
 DEE_ASSERT(self);
 while DEE_UNLIKELY((new_throwv = (struct DeeThrowDoc *)realloc_nz(
  self->tds_throwv,(self->tds_throwc+1)*sizeof(struct DeeThrowDoc))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 return (self->tds_throwv = new_throwv)+self->tds_throwc++;
}

DEE_A_RET_EXCEPT(-1) int DeeThrowDocs_AddDoc(
 DEE_A_INOUT struct DeeThrowDocs *self, DEE_A_IN Dee_size_t decl_len,
 DEE_A_IN_R(decl_len) char const *decl_str) {
 struct DeeThrowDoc *doc; int result;
 DEE_ASSERT(self);
 DEE_ASSERT(!decl_len || decl_str);
 if ((doc = DeeThrowDocs_UnsafeAppend(self)) == NULL) return -1;
 result = DeeThrowDoc_InitFromDoc(doc,decl_len,decl_str);
 if (result != 0) DeeThrowDocs_UnsafeRemove(self);
 else {
  struct DeeThrowDoc *iter = self->tds_throwv;
  while (iter != doc) {
   if (DeeString_SIZE(iter->td_name) == DeeString_SIZE(doc->td_name) && memcmp(
    DeeString_STR(iter->td_name),DeeString_STR(doc->td_name),
    DeeString_SIZE(doc->td_name)*sizeof(char)) == 0) {
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        "Error %r is already documented",
                        doc->td_name);
    _DeeThrowDoc_Quit(doc);
    DeeThrowDocs_UnsafeRemove(self);
    return -1;
   }
   ++iter;
  }
 }
 return result;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeThrowDocs_Repr(
 DEE_A_IN struct DeeThrowDocs const *self) {
 DeeObject *result; struct DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(self);
 if (DeeThrowDocs_ReprToWriter(self,&writer) != 0)
  result = NULL;
 else result = DeeStringWriter_Pack(&writer);
 DeeStringWriter_Quit(&writer);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeThrowDocs_ReprToWriter(
 DEE_A_IN struct DeeThrowDocs const *self,
 DEE_A_INOUT struct DeeStringWriter *writer) {
 struct DeeThrowDoc *iter,*end;
 DEE_ASSERT(self);
 DEE_ASSERT(writer);
 end = (iter = self->tds_throwv)+self->tds_throwc;
 while (iter != end) {
  if (DeeThrowDoc_ReprToWriter(iter,writer) != 0) return -1;
  ++iter;
 }
 return 0;
}











//////////////////////////////////////////////////////////////////////////
// Initializer a param doc from a given declaration string:
//                 <---> decl_len
// >> function add(int a, int b) -> int;
//                 ^ <decl_str
DEE_A_RET_EXCEPT(-1) int DeeParamDoc_InitFromArgDecl(
 DEE_A_OUT struct DeeParamDoc *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN Dee_size_t decl_len, DEE_A_IN_R(decl_len) char const *decl_str) {
 char const *type_begin,*type_end,*name_begin,*name_end,*temp,*def_begin,*def_end;
 DEE_ASSERT(self);
 DEE_ASSERT(!decl_len || decl_str);
 while (decl_len && DEE_CH_IS_SPACE(*decl_str)) ++decl_str,--decl_len;
 type_begin = decl_str;
 if (decl_len && *decl_str == '.') {
  // Var-args type name
  while (decl_len && *decl_str == '.') ++decl_str,--decl_len;
 } else {
  while (decl_len && !DEE_CH_IS_SPACE(*decl_str)) ++decl_str,--decl_len;
 }
 while (1) {
  type_end = decl_str;
  while (decl_len && DEE_CH_IS_SPACE(*decl_str)) ++decl_str,--decl_len;
#define SKIP_STRING(str)\
  if (decl_len >= (sizeof(str)/sizeof(char))-1 &&\
      memcmp(decl_str,str,(sizeof(str)/sizeof(char))-1) == 0 && \
     (decl_len == (sizeof(str)/sizeof(char))-1 || \
     !DEE_CH_IS_ALNUM(decl_str[(sizeof(str)/sizeof(char))-1]))) {\
   decl_str += (sizeof(str)/sizeof(char))-1;\
   decl_len -= (sizeof(str)/sizeof(char))-1;\
   continue;\
  }
  SKIP_STRING("unsigned")
  SKIP_STRING("volatile")
  SKIP_STRING("signed")
  SKIP_STRING("object")
  SKIP_STRING("double")
  SKIP_STRING("short")
  SKIP_STRING("const")
  SKIP_STRING("float")
  SKIP_STRING("long")
  SKIP_STRING("char")
  SKIP_STRING("int")
#undef SKIP_STRING
  if (decl_len >= 1 && (*decl_str == '*' || *decl_str == '&')) { ++decl_str,--decl_len; continue; }
  break;
 }

 name_begin = decl_str;
 while (decl_len && (DEE_CH_IS_ALNUM(*decl_str) ||
  *decl_str == '.' || *decl_str == ':')) ++decl_str,--decl_len;
 name_end = decl_str;
 while (decl_len && DEE_CH_IS_SPACE(*decl_str)) ++decl_str,--decl_len;
 if (*decl_str == '.') {
  while (decl_len && *decl_str == '.') ++decl_str,--decl_len;
  name_end = decl_str;
  while (decl_len && DEE_CH_IS_SPACE(*decl_str)) ++decl_str,--decl_len;
 }
 if (*decl_str == '=') {
  do ++decl_str,--decl_len; while (decl_len && *decl_str == '=');
  while (decl_len && DEE_CH_IS_SPACE(*decl_str)) ++decl_str,--decl_len;
  def_begin = decl_str;
  def_end = decl_str+decl_len;
 } else {
  if (decl_len) {
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Expected end of parameter, but got %.*q",
                       (unsigned)decl_len,decl_str);
   return -1;
  }
  def_begin = NULL;
  def_end = NULL;
 }
 if (name_end == name_begin || *name_begin == '.') {
  // Swap the name and type
  temp = type_begin; type_begin = name_begin; name_begin = temp;
  temp = type_end;   type_end   = name_end;   name_end   = temp;
 }
 if (name_end != name_begin && name_end[-1] == '.') {
  flags |= DEE_PARAMDOC_FLAG_VARG;
  do --name_end; while (name_end != name_begin && name_end[-1] == '.');
 }
 if (type_end != type_begin && type_end[-1] == '.') {
  flags |= DEE_PARAMDOC_FLAG_VARG;
  do --type_end; while (type_end != type_begin && type_end[-1] == '.');
 }
 while (type_begin != type_end && DEE_CH_IS_SPACE(type_end[-1])) --type_end;
 while (name_begin != name_end && DEE_CH_IS_SPACE(name_end[-1])) --name_end;

 if (type_end == type_begin && (flags&DEE_PARAMDOC_FLAG_RETURN)!=0) {
  // Swap the name and type (again)
  temp = type_begin; type_begin = name_begin; name_begin = temp;
  temp = type_end;   type_end   = name_end;   name_end   = temp;
 }

 self->pd_flags = flags;
 if (name_begin == name_end) self->pd_name = NULL;
 else if ((self->pd_name = (DeeStringObject *)DeeString_NewWithLength(
  (Dee_size_t)(name_end-name_begin),name_begin)) == NULL) return -1;
 if (type_begin == type_end)
  Dee_INCREF(self->pd_type = (DeeStringObject *)&_object_name);
 else if ((self->pd_type = (DeeStringObject *)DeeString_NewWithLength(
  (Dee_size_t)(type_end-type_begin),type_begin)) == NULL)
 {err_name: Dee_XDECREF(self->pd_name); return -1; }
 if (def_begin == def_end)
  self->pd_def = NULL;
 else if ((self->pd_def = (DeeStringObject *)DeeString_NewWithLength(
  (Dee_size_t)(def_end-def_begin),def_begin)) == NULL)
 { Dee_DECREF(self->pd_type); goto err_name; }
 self->pd_doc = NULL;
 return 0;
}


/////////////////////////////////////////////////////////////////////////
// Add a documentation string to a param decl:
//             <-----------------------> doc_len
// >> @param a: The first input paramter
//             ^ <doc_str
DEE_A_RET_EXCEPT(-1) int DeeParamDoc_AddDoc(
 DEE_A_INOUT struct DeeParamDoc *self,
 DEE_A_IN Dee_size_t doc_len, DEE_A_IN_R(doc_len) char const *doc_str) {
 DEE_ASSERT(self);
 DEE_ASSERT(!doc_len || doc_str);
 if (self->pd_doc) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Parameter %q was already has a doc string set",
                      self->pd_name ? DeeString_STR(self->pd_name) : "<unnamed>");
  return -1;
 }
 while (doc_len && DEE_CH_IS_SPACE(*doc_str)) ++doc_str,--doc_len;
 while (doc_len && DEE_CH_IS_SPACE(doc_str[doc_len-1])) --doc_len;
 if ((self->pd_doc = (DeeStringObject *)DeeString_NewWithLength(doc_len,doc_str)) == NULL) return -1;
 if (doc_len && doc_str[doc_len-1] != '.') {
  // Append a '.' If the doc text doesn't end with one
  if (DeeString_Append((DeeObject **)&self->pd_doc,".") != 0) return -1;
 }
 return 0;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeParamDoc_Repr(
 DEE_A_IN struct DeeParamDoc const *self) {
 DeeObject *result; struct DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(self);
 if (DeeParamDoc_ReprToWriter(self,&writer) != 0)
  result = NULL;
 else result = DeeStringWriter_Pack(&writer);
 DeeStringWriter_Quit(&writer);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeParamDoc_ReprToWriter(
 DEE_A_IN struct DeeParamDoc const *self,
 DEE_A_INOUT struct DeeStringWriter *writer) {
 DEE_ASSERT(self);
 DEE_ASSERT(writer);
 if (self->pd_doc) {
  if ((self->pd_flags&DEE_PARAMDOC_FLAG_RETURN)!=0) {
   if (DeeStringWriter_Writef(writer,"@return: %k\n",self->pd_doc) != 0) return -1;
  } else {
   if (DeeStringWriter_Writef(writer,"@param %s: %k\n",
    self->pd_name ? DeeString_STR(self->pd_name) : "<unnamed>",self->pd_doc) != 0) return -1;
  }
 }
 return 0;
}














struct DeeParamDoc *DeeParamDocs_UnsafeAppend(DEE_A_INOUT struct DeeParamDocs *self) {
 struct DeeParamDoc *new_paramv;
 DEE_ASSERT(self);
 while DEE_UNLIKELY((new_paramv = (struct DeeParamDoc *)realloc_nz(
  self->pds_argv,(self->pds_argc+1)*sizeof(struct DeeParamDoc))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 return (self->pds_argv = new_paramv)+self->pds_argc++;
}
DEE_A_RET_EXCEPT(-1) int DeeParamDocs_AppendParam(
 DEE_A_INOUT struct DeeParamDocs *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN Dee_size_t decl_len, DEE_A_IN_R(decl_len) char const *decl_str) {
 struct DeeParamDoc *doc; int result;
 DEE_ASSERT(self);
 DEE_ASSERT(!decl_len || decl_str);
 if ((doc = DeeParamDocs_UnsafeAppend(self)) == NULL) return -1;
 result = DeeParamDoc_InitFromArgDecl(doc,flags,decl_len,decl_str);
 if (result != 0) DeeParamDocs_UnsafeRemove(self);
 else if (doc->pd_name != NULL) {
  struct DeeParamDoc *iter = self->pds_argv;
  while (iter != doc) {
   if (iter->pd_name &&
       DeeString_SIZE(iter->pd_name) == DeeString_SIZE(doc->pd_name) &&
       memcmp(DeeString_STR(iter->pd_name),DeeString_STR(doc->pd_name),
              DeeString_SIZE(doc->pd_name)*sizeof(char)) == 0) {
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        "Argument name %r is already used",
                        doc->pd_name);
    _DeeParamDoc_Quit(doc);
    DeeParamDocs_UnsafeRemove(self);
    return -1;
   }
   ++iter;
  }
 }
 return result;
}


DEE_A_RET_EXCEPT(-1) int DeeParamDocs_AppendParams(
 DEE_A_INOUT struct DeeParamDocs *self, DEE_A_IN Dee_size_t decl_len,
 DEE_A_IN_R(decl_len) char const *decl_str) {
 char const *arg_begin,*iter;
 int paren_recursion,bracket_recursion;
 int brace_recursion,angle_recursion;
 DEE_ASSERT(self);
 DEE_ASSERT(!decl_len || decl_str);
 if (!decl_len) return 0;
 paren_recursion = 0; bracket_recursion = 0;
 brace_recursion = 0; angle_recursion = 0;
 arg_begin = iter = decl_str;
 while (1) {
next:
  switch (*iter) {
   case ',':
flush_comma:
    if (!paren_recursion && !bracket_recursion &&
        !brace_recursion && !angle_recursion) {
     if (DeeParamDocs_AppendParam(self,DEE_PARAMDOC_FLAG_NONE,
      (Dee_size_t)(iter-arg_begin),arg_begin) != 0) return -1;
     arg_begin = iter+1;
    }
    if (!decl_len || !--decl_len) goto end;
    ++iter;
    goto next;
   case '(': ++paren_recursion; break;
   case ')': --paren_recursion; break;
   case '[': if (!paren_recursion) ++bracket_recursion; break;
   case ']': if (!paren_recursion) --bracket_recursion; break;
   case '{': if (!bracket_recursion) ++brace_recursion; break;
   case '}': if (!bracket_recursion) --brace_recursion; break;
   case '<': if (!brace_recursion) ++angle_recursion; break;
   case '>': if (!brace_recursion) --angle_recursion; break;
   default: break;
  }
  ++iter;
  if (!--decl_len) break;
 }
 goto flush_comma;
end:
 if (paren_recursion || bracket_recursion ||
     brace_recursion || angle_recursion) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Unterminated parenthesis in argument string %.*q",
                      (unsigned)(decl_len+(iter-decl_str)),decl_str);
  return -1;
 }
 return 0;
}

//////////////////////////////////////////////////////////////////////////
// Add a documentation string to a param declaration:
//          <--------------------------> doc_len
// >> @param a: The first input paramter
//          ^ <doc_str
DEE_A_RET_EXCEPT(-1) int DeeParamDocs_AddDoc(
 DEE_A_INOUT struct DeeParamDocs *self,
 DEE_A_IN Dee_size_t doc_len, DEE_A_IN_R(doc_len) char const *doc_str) {
 struct DeeParamDoc *doc_iter,*doc_end; Dee_size_t name_size;
 char const *name_begin,*name_end,*iter = doc_str;
 DEE_ASSERT(self);
 DEE_ASSERT(!doc_len || doc_str);
 while (doc_len && DEE_CH_IS_SPACE(*iter)) ++iter,--doc_len;
 name_begin = iter;
 while (doc_len && *iter != ':') ++iter,--doc_len;
 if (!doc_len) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Failed to find ':' in parameter documentation string %.*q",
                      (unsigned)(iter-name_begin),name_begin);
  return -1;
 }
 name_end = iter;
 while (name_begin != name_end && DEE_CH_IS_SPACE(name_end[-1])) --name_end;
 if (name_begin == name_end) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "No name specified in parameter documentation string %.*q",
                      (unsigned)(doc_len+(iter-doc_str)),doc_str);
  return -1;
 }
 while (doc_len && *iter == ':') ++iter,--doc_len;
 doc_end = (doc_iter = self->pds_argv)+self->pds_argc;
 name_size = (Dee_size_t)(name_end-name_begin);
 while (doc_iter != doc_end) {
  if (doc_iter->pd_name &&
      DeeString_SIZE(doc_iter->pd_name) == name_size &&
      memcmp(name_begin,DeeString_STR(doc_iter->pd_name),name_size) == 0) {
   return DeeParamDoc_AddDoc(doc_iter,doc_len,iter); // Found it!
  }
  ++doc_iter;
 }
 DeeError_SetStringf(&DeeErrorType_ValueError,
                     "Can't document unknown argument %.*q",
                     (unsigned)name_size,name_begin);
 return -1;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeParamDocs_Repr(
 DEE_A_IN struct DeeParamDocs const *self) {
 DeeObject *result; struct DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(self);
 if (DeeParamDocs_ReprToWriter(self,&writer) != 0)
  result = NULL;
 else result = DeeStringWriter_Pack(&writer);
 DeeStringWriter_Quit(&writer);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeParamDocs_ReprToWriter(
 DEE_A_IN struct DeeParamDocs const *self,
 DEE_A_INOUT struct DeeStringWriter *writer) {
 struct DeeParamDoc *arg_iter,*arg_end;
 DEE_ASSERT(self);
 DEE_ASSERT(writer);
 arg_end = (arg_iter = self->pds_argv)+self->pds_argc;
 while (arg_iter != arg_end) {
  if (DeeParamDoc_ReprToWriter(arg_iter,writer) != 0) return -1;
  ++arg_iter;
 }
 return 0;
}












//////////////////////////////////////////////////////////////////////////
// Initialize an overload from a given doc string:
//                <-------------------> decl_len
// >> function add(int a, int b) -> int;
//                ^ <decl_str
//                <-----> decl_len
// >> function add -> int;
//                ^ <decl_str
DEE_A_RET_EXCEPT(-1) int DeeOverloadDoc_InitFromDoc(
 DEE_A_OUT struct DeeOverloadDoc *self, DEE_A_IN Dee_size_t decl_len,
 DEE_A_IN_R(decl_len) char const *decl_str) {
 DEE_ASSERT(self);
 DEE_ASSERT(!decl_len || decl_str);
 while (decl_len && DEE_CH_IS_SPACE(*decl_str)) ++decl_str,--decl_len;
 while (decl_len && (DEE_CH_IS_ALNUM(*decl_str) ||
  *decl_str == '.' || *decl_str == ':')) ++decl_str,--decl_len;
 while (decl_len && DEE_CH_IS_SPACE(*decl_str)) ++decl_str,--decl_len;
 if (decl_len && *decl_str == '(') {
  char const *args_begin;
  int paren_recursion = 1;
  ++decl_str,--decl_len;
  args_begin = decl_str;
  while (decl_len) {
   if (*decl_str == ')' && !--paren_recursion) break;
   else if (*decl_str == '(') ++paren_recursion;
   ++decl_str,--decl_len;
  }
  if (!decl_len) {
   DeeError_SET_STRING(&DeeErrorType_ValueError,
                       "Expected ')' to terminate argument list");
   return -1;
  }
  _DeeParamDocs_Init(&self->od_params);
  if (DeeParamDocs_AppendParams(&self->od_params,
   (Dee_size_t)(decl_str-args_begin),args_begin) != 0) {
err_args:
   _DeeParamDocs_Quit(&self->od_params);
   return -1;
  }
  ++decl_str,--decl_len;
 } else {
  _DeeParamDocs_Init(&self->od_params);
 }
 while (decl_len && DEE_CH_IS_SPACE(*decl_str)) ++decl_str,--decl_len;
 if (decl_len >= 2 && decl_str[0] == '-' && decl_str[1] == '>') {
  decl_len -= 2,decl_str += 2; // Return type
  while (decl_len && DEE_CH_IS_SPACE(*decl_str)) ++decl_str,--decl_len;
  if ((self->od_return.pd_type = (DeeStringObject *)
   DeeString_NewWithLength(decl_len,decl_str)) == NULL) goto err_args;
 } else {
  if (decl_len) {
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Expected '(' to begin argument list, but got %.*q",
                       decl_len,decl_str);
   goto err_args;
  }
  Dee_INCREF(self->od_return.pd_type = (DeeStringObject *)&_object_name);
 }
 self->od_return.pd_name = NULL;
 self->od_return.pd_flags = DEE_PARAMDOC_FLAG_RETURN;
 self->od_return.pd_doc = NULL;
 self->od_return.pd_def = NULL;
 _DeeThrowDocs_Init(&self->od_throws);
 self->od_doc = NULL;
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeOverloadDoc_AddText(
 DEE_A_INOUT struct DeeOverloadDoc *self, DEE_A_IN Dee_size_t text_len,
 DEE_A_IN_R(text_len) char const *text_str) {
 DEE_ASSERT(self);
 DEE_ASSERT(!text_len || text_str);
 while (text_len && (DEE_CH_IS_SPACE(text_str[text_len-1]) || text_str[text_len-1] == '\n')) --text_len;
 if (!text_len) return 0;
 if (!self->od_doc) Dee_INCREF(self->od_doc = (DeeStringObject *)Dee_EmptyString);
 else if (DeeString_AppendWithLength((DeeObject **)&self->od_doc,1,"\n") != 0) return -1;
 return DeeString_AppendWithLength((DeeObject **)&self->od_doc,text_len,text_str);
}



DEE_A_RET_EXCEPT(-1) int DeeOverloadDoc_AddDoc(
 DEE_A_INOUT struct DeeOverloadDoc *self, DEE_A_IN Dee_size_t doc_len,
 DEE_A_IN_R(doc_len) char const *doc_str) {
 char const *line_start;
 DEE_ASSERT(self);
 DEE_ASSERT(!doc_len || doc_str);
 if (doc_len) while (1) {
  line_start = doc_str;
  // Seek to the end of the line/doc
  while (doc_len && *doc_str != '\n') ++doc_str,--doc_len;
  if (line_start != doc_str) {
   if (*line_start == '\t') {
    // Generic documentation text after a tab
    if (DeeOverloadDoc_AddText(self,(Dee_size_t)(
     doc_str-(line_start+1)),line_start+1) != 0) return -1;
   } else if (*line_start == ' ') {
    do ++line_start; // Skip whitespace
    while (line_start != doc_str && DEE_CH_IS_SPACE(*line_start));
    // Generic documentation text after space
    if (DeeOverloadDoc_AddText(self,(Dee_size_t)(doc_str-line_start),line_start) != 0) return -1;
   } else if (*line_start == '@') {
    char const *tag_end; Dee_size_t tag_size;
    do ++line_start; // Skip whitespace
    while (line_start != doc_str && DEE_CH_IS_SPACE(*line_start));
    tag_end = line_start;
    while (tag_end != doc_str && (DEE_CH_IS_ALNUM(*tag_end) || *tag_end == '.')) ++tag_end;
    tag_size = (Dee_size_t)(tag_end-line_start);
#define check_tag(tag) \
 (tag_size == (sizeof(tag)/sizeof(char))-1 &&\
  memcmp(line_start,tag,sizeof(tag)-sizeof(char)) == 0)
    if (check_tag("throws")) {
     if (DeeThrowDocs_AddDoc(&self->od_throws,(Dee_size_t)(doc_str-tag_end),tag_end) != 0) return -1;
    } else if (check_tag("param")) {
     if (DeeParamDocs_AddDoc(&self->od_params,(Dee_size_t)(doc_str-tag_end),tag_end) != 0) return -1;
    } else if (check_tag("return")) {
     while (tag_end != doc_str && DEE_CH_IS_SPACE(*tag_end)) ++tag_end;
     if (*tag_end != ':') {
      DeeError_SetStringf(&DeeErrorType_ValueError,
                          "Failed to find ':' after @return tag in %.*q",
                          (unsigned)(doc_str-line_start),line_start);
      return -1;
     }
     ++tag_end;
     if (DeeParamDoc_AddDoc(&self->od_return,(Dee_size_t)(doc_str-tag_end),tag_end) != 0) return -1;
    } else {
     DeeError_SetStringf(&DeeErrorType_ValueError,
                         "Unknown documentation tag: %.*q",
                         (unsigned)tag_size,line_start);
     return -1;
    }
#undef check_tag
   } else {
    DeeError_SetStringf(&DeeErrorType_ValueError,
                        "Invalid documentation line: %.*q",
                        (unsigned)(doc_str-line_start),line_start);
    return -1;
   }
  }
  if (!doc_len) break;
  ++doc_str;
  --doc_len;
 }
 return 0;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeOverloadDoc_Repr(
 DEE_A_IN struct DeeOverloadDoc const *self, DEE_A_IN Dee_uint32_t function_flags,
 DEE_A_IN_Z_OPT char const *function_name, DEE_A_IN_Z_OPT char const *prefix) {
 DeeObject *result; struct DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(self);
 if (DeeOverloadDoc_ReprToWriter(self,&writer,function_flags,function_name,prefix) != 0)
  result = NULL;
 else result = DeeStringWriter_Pack(&writer);
 DeeStringWriter_Quit(&writer);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeOverloadDoc_ReprToWriter(
 DEE_A_IN struct DeeOverloadDoc const *self,
 DEE_A_INOUT struct DeeStringWriter *writer, DEE_A_IN Dee_uint32_t function_flags,
 DEE_A_IN_Z_OPT char const *function_name, DEE_A_IN_Z_OPT char const *prefix) {
 struct DeeParamDoc *arg_iter,*arg_end;
 DEE_ASSERT(self);
 DEE_ASSERT(writer);
 if ((function_flags&DEE_METHODDOC_FLAG_CLASS)!=0 && DeeStringWriter_WRITE_STRING(writer,"class ") != 0) return -1;
 if ((function_flags&DEE_METHODDOC_FLAG_CONSTEXPR)!=0 && DeeStringWriter_WRITE_STRING(writer,"constexpr ") != 0) return -1;
 if (prefix && DeeStringWriter_Writef(writer,"%s ",prefix) != 0) return -1;
 if (DeeStringWriter_Writef(writer,"%s(",function_name ? function_name : "") != 0) return -1;
 arg_end = (arg_iter = self->od_params.pds_argv)+self->od_params.pds_argc;
 while (arg_iter != arg_end) {
  if (arg_iter != self->od_params.pds_argv && DeeStringWriter_WRITE_STRING(writer,", ") != 0) return -1;
  if (arg_iter->pd_name != NULL) {
   if (DeeStringWriter_Writef(writer,"%k %k",arg_iter->pd_type,arg_iter->pd_name) != 0) return -1;
  } else {
   if (DeeStringWriter_WriteObjectStr(writer,(DeeObject *)arg_iter->pd_type) != 0) return -1;
  }
  if ((arg_iter->pd_flags&DEE_PARAMDOC_FLAG_VARG)!=0 &&
      DeeStringWriter_WRITE_STRING(writer,"...") != 0) return -1;
  if (arg_iter->pd_def != NULL) {
   if (DeeStringWriter_Writef(writer," = %k",arg_iter->pd_def) != 0) return -1;
  }
  ++arg_iter;
 }
 if (DeeStringWriter_Writef(writer,") -> %k;\n",self->od_return.pd_type) != 0) return -1;
 if (DeeParamDocs_ReprToWriter(&self->od_params,writer) != 0) return -1;
 if (DeeParamDoc_ReprToWriter(&self->od_return,writer) != 0) return -1;
 if (DeeThrowDocs_ReprToWriter(&self->od_throws,writer) != 0) return -1;
 if (self->od_doc) {
  if (DeeStringWriter_Writef(writer,"\t%>k\n",self->od_doc) != 0) return -1;
 }
 return 0;
}






struct DeeOverloadDoc *DeeOverloadDocs_UnsafeAppend(
 DEE_A_INOUT struct DeeOverloadDocs *self) {
 struct DeeOverloadDoc *new_overloadv;
 DEE_ASSERT(self);
 while DEE_UNLIKELY((new_overloadv = (struct DeeOverloadDoc *)realloc_nz(
  self->ods_docv,(self->ods_docc+1)*sizeof(struct DeeOverloadDoc))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 return (self->ods_docv = new_overloadv)+self->ods_docc++;
}


DEE_A_RET_EXCEPT(-1) int DeeOverloadDocs_AddDocEx(
 DEE_A_INOUT struct DeeOverloadDocs *self,
 DEE_A_IN Dee_size_t decl_len, DEE_A_IN_R(decl_len) char const *decl_str,
 DEE_A_IN Dee_size_t  doc_len, DEE_A_IN_R( doc_len) char const  *doc_str) {
 struct DeeOverloadDoc *doc;
 DEE_ASSERT(self);
 DEE_ASSERT(!decl_len || decl_str);
 DEE_ASSERT(!doc_len || doc_str);
 if ((doc = DeeOverloadDocs_UnsafeAppend(self)) == NULL) return -1;
 if (DeeOverloadDoc_InitFromDoc(doc,decl_len,decl_str) != 0) {
  DeeOverloadDocs_UnsafeRemove(self);
  return -1;
 }
 if (DeeOverloadDoc_AddDoc(doc,doc_len,doc_str) != 0) {
  _DeeOverloadDoc_Quit(doc);
  DeeOverloadDocs_UnsafeRemove(self);
  return -1;
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeOverloadDocs_AddDoc(
 DEE_A_INOUT struct DeeOverloadDocs *self,
 DEE_A_IN Dee_size_t decl_len, DEE_A_IN_R(decl_len) char const *decl_str) {
 char const *head_end,*head_start;
 DEE_ASSERT(self);
 DEE_ASSERT(!decl_len || decl_str);
 head_start = decl_str;
 if (*decl_str != '\t') while (decl_len && *decl_str != '\n') ++decl_str,--decl_len;
 head_end = decl_str;
 while (head_end != head_start && ( // Remove trailing ';'
  DEE_CH_IS_SPACE(head_end[-1]) || head_end[-1] == ';')) --head_end;
 return DeeOverloadDocs_AddDocEx(self,(Dee_size_t)(head_end-head_start),
                                 head_start,decl_len,decl_str);
}

DEE_A_RET_EXCEPT(-1) int DeeOverloadDocs_AddDocs(
 DEE_A_INOUT struct DeeOverloadDocs *self,
 DEE_A_IN Dee_size_t decl_len, DEE_A_IN_R(decl_len) char const *decl_str) {
 char const *decl_start;
 DEE_ASSERT(self);
 DEE_ASSERT(!decl_len || decl_str);
 while (decl_len) {
  decl_start = decl_str;
  do {
   while (decl_len && *decl_str != '\n') ++decl_str,--decl_len;
   if (decl_len) ++decl_str,--decl_len;
  } while (decl_len && (DEE_CH_IS_SPACE(*decl_str) || *decl_str == '@'));
  if (DeeOverloadDocs_AddDoc(self,(Dee_size_t)(decl_str-decl_start),decl_start) != 0) return -1;
 }
 return 0;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeOverloadDocs_Repr(
 DEE_A_IN struct DeeOverloadDocs const *self, DEE_A_IN Dee_uint32_t function_flags,
 DEE_A_IN_Z_OPT char const *function_name, DEE_A_IN_Z_OPT char const *prefix) {
 DeeObject *result; struct DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(self);
 if (DeeOverloadDocs_ReprToWriter(self,&writer,function_flags,function_name,prefix) != 0)
  result = NULL;
 else result = DeeStringWriter_Pack(&writer);
 DeeStringWriter_Quit(&writer);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeOverloadDocs_ReprToWriter(
 DEE_A_IN struct DeeOverloadDocs const *self,
 DEE_A_INOUT struct DeeStringWriter *writer, DEE_A_IN Dee_uint32_t function_flags,
 DEE_A_IN_Z_OPT char const *function_name, DEE_A_IN_Z_OPT char const *prefix) {
 struct DeeOverloadDoc *iter,*end;
 DEE_ASSERT(self);
 DEE_ASSERT(writer);
 end = (iter = self->ods_docv)+self->ods_docc;
 while (iter != end) {
  if (DeeOverloadDoc_ReprToWriter(iter,writer,
   function_flags,function_name,prefix) != 0) return -1;
  ++iter;
 }
 return 0;
}




















DEE_A_RET_EXCEPT(-1) int DeeMethodDoc_InitFromNameAndDocWithLength(
 DEE_A_OUT struct DeeMethodDoc *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT_OPT DeeStringObject *this_type,
 Dee_size_t name_len, DEE_A_IN_R(name_len) char const *name_str,
 Dee_size_t doc_len, DEE_A_IN_R(doc_len) char const *doc_str) {
 DEE_ASSERT(self);
 DEE_ASSERT(!name_len || name_str);
 DEE_ASSERT(!doc_len || doc_str);
 DEE_ASSERT(!this_type || (DeeObject_Check(this_type) && DeeString_Check(this_type)));
 if (!doc_str) doc_str = "(...)",doc_len = 5; // Fallback overload
 _DeeOverloadDocs_Init(&self->md_over);
 if (DeeOverloadDocs_AddDocs(&self->md_over,doc_len,doc_str) != 0) {
err_over:
  _DeeOverloadDocs_Quit(&self->md_over);
  return -1;
 }
 if ((self->md_name = (DeeStringObject *)
  DeeString_NewWithLength(name_len,name_str)) == NULL) goto err_over;
 Dee_XINCREF(self->md_this = this_type);
 self->md_flags = flags;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeMethodDoc_InitFromMethodDef(
 DEE_A_OUT struct DeeMethodDoc *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeMethodDef const *def) {
 char const *used_doc;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(this_type) && DeeString_Check(this_type));
 DEE_ASSERT(def);
 if ((def->d_flags&DEE_METHODDEF_FLAG_CONST)!=0) flags |= DEE_METHODDOC_FLAG_CONSTEXPR;
 if ((used_doc = def->d_doc) == NULL) {
  used_doc = _DeeMemberFunction_GetDoc(def->d_func); // Deduce from known function
 } else if (used_doc[0] == '*') {
  if (!used_doc[1]) {
   used_doc = _DeeMemberName_GetDoc(def->d_name); // Deduce from name
  } else if (used_doc[1] == '!') {
   DeeObject *obj_doc,*doc_args; int error;
   if ((doc_args = _DeeMemberName_GetDocArgs(used_doc+2)) == NULL) return -1;
   error = _DeeMemberName_GetDocWithArgs(def->d_name,doc_args,&obj_doc);
   Dee_DECREF(doc_args);
   if (error < 0) return error;
   if (error == 0) {
    DEE_ASSERT(DeeObject_Check(obj_doc) && DeeString_Check(obj_doc));
    error = DeeMethodDoc_InitFromNameAndDoc(self,flags,this_type,def->d_name,DeeString_STR(obj_doc));
    Dee_DECREF(obj_doc);
    return error;
   }
  } else if (used_doc[1] == ':') {
   used_doc = _DeeMemberName_GetDoc(used_doc+2);
  }
 }
 if (DeeMethodDoc_InitFromNameAndDoc(self,flags,this_type,def->d_name,used_doc) != 0) return -1;
 return 0;
}



DEE_A_RET_EXCEPT(-1) int DeeMemberDoc_InitFromNameAndDocWithLength(
 DEE_A_OUT struct DeeMemberDoc *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN DeeTypeObject const *return_type,
 Dee_size_t name_len, DEE_A_IN_R(name_len) char const *name_str,
 Dee_size_t doc_len, DEE_A_IN_R(doc_len) char const *doc_str) {
 DEE_ASSERT(self);
 DEE_ASSERT(!name_len || name_str);
 DEE_ASSERT(!doc_len || doc_str);
 DEE_ASSERT(DeeObject_Check(this_type) && DeeString_Check(this_type));
 DEE_ASSERT(DeeObject_Check(return_type) && DeeType_Check(return_type));
 if ((self->mbd_name = (DeeStringObject *)DeeString_NewWithLength(name_len,name_str)) == NULL) return -1;
 self->mbd_return.pd_def = NULL;
 self->mbd_return.pd_doc = NULL;
 self->mbd_return.pd_flags = DEE_PARAMDOC_FLAG_RETURN;
 self->mbd_return.pd_name = NULL;
 self->mbd_return.pd_type = NULL;
 if (doc_len) {
  char const *line_start;
  Dee_INCREF(self->mbd_doc = (DeeStringObject *)Dee_EmptyString);
  Dee_INCREF(self->mbd_return.pd_doc = (DeeStringObject *)Dee_EmptyString);
  while (1) {
   line_start = doc_str;
   while (doc_len && *doc_str != '\n') --doc_len,++doc_str;
   if (line_start != doc_str) {
    if (*line_start == '-' && line_start+1 != doc_str && line_start[1] == '>' && !self->mbd_return.pd_type) {
     line_start += 2;
     while (line_start != doc_str && DEE_CH_IS_SPACE(*line_start)) ++line_start;
     if ((self->mbd_return.pd_type = (DeeStringObject *)DeeString_NewWithLength(
      (Dee_size_t)(doc_str-line_start),line_start)) == NULL) goto err_doc_name_type;
    } else if (*line_start == '\t') {
     // Generic documentation text after a tab
     if (DeeString_AppendWithLength((DeeObject **)&self->mbd_doc,
      (Dee_size_t)(doc_str-(line_start+1)),line_start+1) != 0) goto err_doc_name_type;
    } else if (*line_start == ' ') {
     do ++line_start; // Skip whitespace
     while (line_start != doc_str && DEE_CH_IS_SPACE(*line_start));
     // Generic documentation text after space
     if (DeeString_AppendWithLength((DeeObject **)&self->mbd_doc,
      (Dee_size_t)(doc_str-line_start),line_start) != 0) goto err_doc_name_type;
    } else if (*line_start == '@') {
     char const *tag_end; Dee_size_t tag_size;
     do ++line_start; // Skip whitespace
     while (line_start != doc_str && DEE_CH_IS_SPACE(*line_start));
     tag_end = line_start;
     while (tag_end != doc_str && (DEE_CH_IS_ALNUM(*tag_end) || *tag_end == '.')) ++tag_end;
     tag_size = (Dee_size_t)(tag_end-line_start);
#define check_tag(tag) \
  (tag_size == (sizeof(tag)/sizeof(char))-1 &&\
   memcmp(line_start,tag,sizeof(tag)-sizeof(char)) == 0)
     if (check_tag("return")) {
      while (tag_end != doc_str && DEE_CH_IS_SPACE(*tag_end)) ++tag_end;
      if (*tag_end != ':') {
       DeeError_SetStringf(&DeeErrorType_ValueError,
                           "Failed to find ':' after @return tag in %.*q",
                           (unsigned)(doc_str-line_start),line_start);
       goto err_doc_name_type;
      }
      ++tag_end;
      if (DeeString_AppendWithLength((DeeObject **)&self->mbd_return.pd_doc,
       (Dee_size_t)(doc_str-tag_end),tag_end) != 0) goto err_doc_name_type;
     } else {
      DeeError_SetStringf(&DeeErrorType_ValueError,
                          "Unknown documentation tag: %.*q",
                          (unsigned)tag_size,line_start);
      goto err_doc_name_type;
     }
#undef check_tag
    } else {
     DeeError_SetStringf(&DeeErrorType_ValueError,
                         "Invalid documentation line: %.*q",
                         (unsigned)(doc_str-line_start),line_start);
     goto err_doc_name_type;
    }
   }
   if (!doc_len) break;
   ++doc_str;
   --doc_len;
  }
  if (DeeString_CheckEmpty(self->mbd_doc)) Dee_CLEAR(self->mbd_doc);
  if (DeeString_CheckEmpty(self->mbd_return.pd_doc)) Dee_CLEAR(self->mbd_return.pd_doc);
 } else self->mbd_doc = NULL;
 if (!self->mbd_return.pd_type) {
  char const *type_name = DeeType_NAME(return_type);
  if (!type_name) Dee_INCREF(self->mbd_return.pd_type = (DeeStringObject *)&_object_name);
  else if ((self->mbd_return.pd_type = (DeeStringObject *)DeeString_New(type_name)) == NULL) {
err_doc_name_type:
   _DeeParamDoc_Quit(&self->mbd_return);
   Dee_XDECREF(self->mbd_doc);
   Dee_DECREF(self->mbd_name);
   return -1;
  }
 }
 Dee_INCREF(self->mbd_this = this_type);
 self->mbd_flags = flags;
 self->mbd_value = NULL;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeMemberDoc_InitFromMemberDef(
 DEE_A_OUT struct DeeMemberDoc *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeMemberDef const *def) {
 DeeTypeObject *member_type; DeeObject *value;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(this_type) && DeeString_Check(this_type));
 DEE_ASSERT(def);
 if ((def->d_type&DeeMemberType_CONST)!=0) {
  if ((value = DeeMember_Get(((DeeObject *)NULL)+1,def)) == NULL) return -1;
  member_type = Dee_TYPE(value);
 } else {
  if ((member_type = DeeMember_Type(def)) == NULL) return -1;
  value = NULL;
 }
 if ((def->d_access&DeeMemberAccess_READ)!=0) flags |= DEE_MEMBERDOC_FLAG_READ;
 if ((def->d_access&DeeMemberAccess_WRITE)!=0) flags |= DEE_MEMBERDOC_FLAG_WRITE;
 if ((def->d_type&DeeMemberType_ATOMIC)!=0) flags |= DEE_MEMBERDOC_FLAG_ATOMIC;
 if (DeeMemberDoc_InitFromNameAndDoc(self,flags,this_type,member_type,def->d_name,def->d_doc) != 0) { Dee_XDECREF(value); return -1; }
 Dee_INHERIT_REF(self->mbd_value,value);
 return 0;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeMemberDoc_Repr(
 DEE_A_IN struct DeeMemberDoc const *self) {
 DeeObject *result; struct DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(self);
 if (DeeMemberDoc_ReprToWriter(self,&writer) != 0)
  result = NULL;
 else result = DeeStringWriter_Pack(&writer);
 DeeStringWriter_Quit(&writer);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeMemberDoc_ReprToWriter(
 DEE_A_IN struct DeeMemberDoc const *self,
 DEE_A_INOUT struct DeeStringWriter *writer) {
 char const *rights;
 DEE_ASSERT(self);
 DEE_ASSERT(writer);
 if ((self->mbd_flags&DEE_MEMBERDOC_FLAG_CLASS)!=0 &&
     DeeStringWriter_WRITE_STRING(writer,"class ") != 0) return -1;
 if (self->mbd_value) {
  if (DeeStringWriter_Writef(writer,"const %k -> %r;\n",
   self->mbd_name,self->mbd_value) != 0) return -1;
 } else {
  switch (self->mbd_flags&(DEE_MEMBERDOC_FLAG_READ|DEE_MEMBERDOC_FLAG_WRITE)) {
   case DEE_MEMBERDOC_FLAG_READ|DEE_MEMBERDOC_FLAG_WRITE: rights = "get, set"; break;
   case DEE_MEMBERDOC_FLAG_READ: rights = "get"; break;
   case DEE_MEMBERDOC_FLAG_WRITE: rights = "set"; break;
   default: rights = ""; break;
  }
  if (DeeStringWriter_Writef(writer,"constexpr property %k -> %k = { %s }\n",
   self->mbd_name,self->mbd_return.pd_type,rights) != 0) return -1;
 }
 if (self->mbd_return.pd_doc && DeeStringWriter_Writef(writer,"@return: %k\n",self->mbd_return.pd_doc) != 0) return -1;
 if (self->mbd_doc && DeeStringWriter_Writef(writer,"\t%>k\n",self->mbd_doc) != 0) return -1;
 return 0;
}















DEE_A_RET_EXCEPT(-1) int DeeGetSetDoc_InitFromNameAndDocWithLength(
 DEE_A_OUT struct DeeGetSetDoc *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT_OPT DeeStringObject *this_type,
 Dee_size_t name_len, DEE_A_IN_R(name_len) char const *name_str,
 Dee_size_t doc_len, DEE_A_IN_R(doc_len) char const *doc_str) {
 DEE_ASSERT(self);
 DEE_ASSERT(!name_len || name_str);
 DEE_ASSERT(!doc_len || doc_str);
 DEE_ASSERT(!this_type || (DeeObject_Check(this_type) && DeeString_Check(this_type)));
 if ((self->gd_name = (DeeStringObject *)DeeString_NewWithLength(name_len,name_str)) == NULL) return -1;
 _DeeThrowDocs_Init(&self->gd_throw);
 self->gd_return.pd_doc = NULL;
 self->gd_return.pd_flags = DEE_PARAMDOC_FLAG_RETURN;
 self->gd_return.pd_def = NULL;
 self->gd_return.pd_name = NULL;
 Dee_INCREF(self->gd_return.pd_type = (DeeStringObject *)&_object_name);
 if (doc_len) {
  char const *line_start;
  Dee_INCREF(self->gd_doc = (DeeStringObject *)Dee_EmptyString);
  while (1) {
   line_start = doc_str;
   while (doc_len && *doc_str != '\n') --doc_len,++doc_str;
   if (line_start != doc_str) {
    if (*line_start == '-' && line_start+1 != doc_str && line_start[1] == '>' &&
        self->gd_return.pd_type == (DeeStringObject *)&_object_name) {
     DeeStringObject *old_doc; int error; line_start += 2;
     while (line_start != doc_str && DEE_CH_IS_SPACE(*line_start)) ++line_start;
     old_doc = self->gd_return.pd_doc;
     Dee_DECREF(self->gd_return.pd_type);
     error = DeeParamDoc_InitFromArgDecl(&self->gd_return,DEE_PARAMDOC_FLAG_RETURN,
                                         (Dee_size_t)(doc_str-line_start),line_start);
     DEE_ASSERT(!self->gd_return.pd_doc);
     self->gd_return.pd_doc = old_doc;
     if (error != 0) goto err_name_throws;
    } else if (*line_start == '\t') {
     // Generic documentation text after a tab
     if (DeeString_AppendWithLength((DeeObject **)&self->gd_doc,
      (Dee_size_t)(doc_str-(line_start+1)),line_start+1) != 0) {
err_name_throws:
      Dee_DECREF(self->gd_doc);
      Dee_DECREF(self->gd_name);
      _DeeThrowDocs_Quit(&self->gd_throw);
      _DeeParamDoc_Quit(&self->gd_return);
      return -1;
     }
    } else if (*line_start == ' ') {
     do ++line_start; // Skip whitespace
     while (line_start != doc_str && DEE_CH_IS_SPACE(*line_start));
     // Generic documentation text after space
     if (DeeString_AppendWithLength((DeeObject **)&self->gd_doc,
      (Dee_size_t)(doc_str-line_start),line_start) != 0) goto err_name_throws;
    } else if (*line_start == '@') {
     char const *tag_end; Dee_size_t tag_size;
     do ++line_start; // Skip whitespace
     while (line_start != doc_str && DEE_CH_IS_SPACE(*line_start));
     tag_end = line_start;
     while (tag_end != doc_str && (DEE_CH_IS_ALNUM(*tag_end) || *tag_end == '.')) ++tag_end;
     tag_size = (Dee_size_t)(tag_end-line_start);
#define check_tag(tag) \
  (tag_size == (sizeof(tag)/sizeof(char))-1 &&\
   memcmp(line_start,tag,sizeof(tag)-sizeof(char)) == 0)
     if (check_tag("throws")) {
      if (DeeThrowDocs_AddDoc(&self->gd_throw,(Dee_size_t)(doc_str-tag_end),tag_end) != 0) goto err_name_throws;
     } else if (check_tag("return")) {
      while (tag_end != doc_str && DEE_CH_IS_SPACE(*tag_end)) ++tag_end;
      if (*tag_end != ':') {
       DeeError_SetStringf(&DeeErrorType_ValueError,
                           "Failed to find ':' after @return tag in %.*q",
                           (unsigned)(doc_str-line_start),line_start);
       goto err_name_throws;
      }
      ++tag_end;
      if (DeeParamDoc_AddDoc(&self->gd_return,(Dee_size_t)(doc_str-tag_end),tag_end) != 0) goto err_name_throws;
     } else {
      DeeError_SetStringf(&DeeErrorType_ValueError,
                          "Unknown documentation tag: %.*q",
                          (unsigned)tag_size,line_start);
      goto err_name_throws;
     }
#undef check_tag
    } else {
     DeeError_SetStringf(&DeeErrorType_ValueError,
                         "Invalid documentation line: %.*q",
                         (unsigned)(doc_str-line_start),line_start);
     goto err_name_throws;
    }
   }
   if (!doc_len) break;
   ++doc_str;
   --doc_len;
  }
  if (DeeString_CheckEmpty(self->gd_doc)) Dee_CLEAR(self->gd_doc);
 } else {
  self->gd_doc = NULL;
 }
 Dee_XINCREF(self->gd_this = this_type);
 self->gd_flags = flags;
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeGetSetDoc_InitFromGetSetDef(
 DEE_A_OUT struct DeeGetSetDoc *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeGetSetDef const *def) {
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(this_type) && DeeString_Check(this_type));
 DEE_ASSERT(def);
 if ((def->d_flags&DEE_GETSETDEF_FLAG_CONST)!=0) flags |= DEE_GETSETDOC_FLAG_CONSTEXPR;
 if (def->d_get) flags |= DEE_GETSETDOC_FLAG_GET;
 if (def->d_del) flags |= DEE_GETSETDOC_FLAG_DEL;
 if (def->d_set) flags |= DEE_GETSETDOC_FLAG_SET;
 return DeeGetSetDoc_InitFromNameAndDoc(self,flags,this_type,def->d_name,def->d_doc);
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeGetSetDoc_Repr(
 DEE_A_IN struct DeeGetSetDoc const *self) {
 DeeObject *result; struct DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(self);
 if (DeeGetSetDoc_ReprToWriter(self,&writer) != 0)
  result = NULL;
 else result = DeeStringWriter_Pack(&writer);
 DeeStringWriter_Quit(&writer);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeGetSetDoc_ReprToWriter(
 DEE_A_IN struct DeeGetSetDoc const *self,
 DEE_A_INOUT struct DeeStringWriter *writer) {
 char const *rights;
 DEE_ASSERT(self);
 DEE_ASSERT(writer);
 if ((self->gd_flags&DEE_GETSETDOC_FLAG_CLASS)!=0 &&
     DeeStringWriter_WRITE_STRING(writer,"class ") != 0) return -1;
 if ((self->gd_flags&DEE_GETSETDOC_FLAG_CONSTEXPR)!=0 &&
     DeeStringWriter_WRITE_STRING(writer,"constexpr ") != 0) return -1;
 switch (self->gd_flags&(DEE_GETSETDOC_FLAG_GET|DEE_GETSETDOC_FLAG_DEL|DEE_GETSETDOC_FLAG_SET)) {
  case DEE_GETSETDOC_FLAG_GET|DEE_GETSETDOC_FLAG_DEL|DEE_GETSETDOC_FLAG_SET: rights = "get, del, set"; break;
  case DEE_GETSETDOC_FLAG_DEL|DEE_GETSETDOC_FLAG_SET: rights = "del, set"; break;
  case DEE_GETSETDOC_FLAG_GET|DEE_GETSETDOC_FLAG_SET: rights = "get, set"; break;
  case DEE_GETSETDOC_FLAG_GET|DEE_GETSETDOC_FLAG_DEL: rights = "get, del"; break;
  case DEE_GETSETDOC_FLAG_SET: rights = "set"; break;
  case DEE_GETSETDOC_FLAG_DEL: rights = "del"; break;
  case DEE_GETSETDOC_FLAG_GET: rights = "get"; break;
  default: rights = ""; break;
 }
 if (DeeStringWriter_Writef(writer,"property %k -> %k = { %s }\n",
  self->gd_name,self->gd_return.pd_type,rights) != 0) return -1;
 if (DeeParamDoc_ReprToWriter(&self->gd_return,writer) != 0) return -1;
 if (DeeThrowDocs_ReprToWriter(&self->gd_throw,writer) != 0) return -1;
 if (self->gd_doc && DeeStringWriter_Writef(writer,"\t%>k\n",self->gd_doc) != 0) return -1;
 return 0;
}







//////////////////////////////////////////////////////////////////////////
// Initializer a type doc from a given type
DEE_A_RET_EXCEPT(-1) int DeeTypeDoc_InitFromType(
 DEE_A_OUT struct DeeTypeDoc *self, DEE_A_IN DeeTypeObject const *ty) {
 char const *type_name,*type_doc,*basename; Dee_uint32_t ctor_flags;
 DeeTypeObject *ty_base; Dee_size_t type_doc_len; DeeStringObject *type_name_str;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(ty) && DeeType_Check(ty));
 type_name = DeeType_NAME(ty),type_doc = DeeType_GET_SLOT(ty,tp_doc);
 ctor_flags = DEE_METHODDOC_FLAG_CLASS;
 if ((DeeType_FLAGS(ty)&DEE_TYPE_FLAG_CONST_CTOR)!=0) ctor_flags = DEE_METHODDOC_FLAG_CONSTEXPR;
 if (!type_doc) {
  Dee_uint16_t builtin_id;
  if ((builtin_id = DeeBuiltin_Id((DeeObject *)ty)) != (Dee_uint16_t)-1) {
   type_doc = _DeeBuiltin_GetDoc(builtin_id,type_name ? NULL : &type_name);
  }
  if (!type_doc) {
   if (DeeType_HAS_SLOT(ty,tp_any_ctor)) type_doc = "(...)";
   else if (DeeType_HAS_SLOT(ty,tp_ctor)) type_doc = "()";
   else type_doc = "";
  }
 }
 if (!type_name) type_name = "";
 if (*type_name) {
  if ((type_name_str = (DeeStringObject *)DeeString_New(type_name)) == NULL) return -1;
 } else Dee_INCREF(type_name_str = (DeeStringObject *)&_unnamed_name);
 Dee_INCREF(self->td_descr = (DeeStringObject *)Dee_EmptyString);
 // Parse a type descriptor
 // >> That is a block of text at the very start of the doc
 // NOTE: This block does not necessarily have to be indented,
 //       though the character after the first keyword can't be a '(',
 //       and the block itself can't start with '(' either.
 _DeeDocs_Init(&self->td_attrib);
 if (type_doc) {
  char const *descr_name_begin,*descr_name_end,*descr_start,*doc_iter;
more_descr:
  while (DEE_CH_IS_SPACE(*type_doc)) ++type_doc;
  descr_start = type_doc;
  while (DEE_CH_IS_ALNUM(*type_doc)) ++type_doc;
  while (DEE_CH_IS_SPACE(*type_doc)) ++type_doc;
  if (*type_doc && *type_doc != '(') {
   // Type descriptor string
   while (*type_doc && *type_doc != '\n') ++type_doc;
   if (!DeeString_CheckEmpty(self->td_descr) &&
       DeeString_STR(self->td_descr)[DeeString_SIZE(self->td_descr)-1] != '\n' &&
       DeeString_AppendWithLength((DeeObject **)&self->td_descr,1,"\n") != 0) goto err_this_descr;
   if (DeeString_AppendWithLength((DeeObject **)&self->td_descr,
    (Dee_size_t)(type_doc-descr_start),descr_start) != 0) goto err_this_descr;
   goto more_descr;
  }
  doc_iter = descr_start;
  type_doc_len = 0; type_doc = NULL;
  // Check for operator documentations
more_blocks:
  descr_name_begin = doc_iter;
  while (DEE_CH_IS_ALNUM(*doc_iter)) ++doc_iter;
  descr_name_end = doc_iter;
  while (1) { // Scan until the end of this block
   while (*doc_iter && *doc_iter != '\n') ++doc_iter;
   if (*doc_iter) ++doc_iter;
   if (*doc_iter != '@' && *doc_iter != '\t') {
    char const *after_keyword = doc_iter;
    while (DEE_CH_IS_ALNUM(*after_keyword)) ++after_keyword;
    while (DEE_CH_IS_SPACE(*after_keyword)) ++after_keyword;
    if (*after_keyword != '(') break;
   }
  }
#define check_desc_name(name) \
 ((descr_name_end-descr_name_begin)==((sizeof(name)/sizeof(char))-1) &&\
   memcmp(descr_name_begin,name,sizeof(name)-sizeof(char)) == 0)
  if (check_desc_name("operator")) {
   // Operator declaration
   char const *operator_name_begin,*operator_name_end;
   char const *operator_text_begin,*operator_text_end;
   operator_name_begin = descr_name_end;
   while (operator_name_begin != doc_iter &&
          DEE_CH_IS_SPACE(*operator_name_begin)) ++operator_name_begin;
#define SET_CONSTANT_OPERATOR_NAME(name) \
 (operator_name_end = (operator_name_begin = (name))+((sizeof(name)/sizeof(char))-1))
   switch (operator_name_begin[0]) {
#define DEFINE_INPLACE_OPERATOR_PAIR(ch,name,iname)\
    if (0) { case (ch): operator_text_begin = operator_name_begin+2; if (operator_name_begin[1] == '=') SET_CONSTANT_OPERATOR_NAME(iname); else SET_CONSTANT_OPERATOR_NAME(name); break; }
    DEFINE_INPLACE_OPERATOR_PAIR('-',"__sub__","__isub__");
    DEFINE_INPLACE_OPERATOR_PAIR('+',"__add__","__iadd__");
    DEFINE_INPLACE_OPERATOR_PAIR('/',"__div__","__idiv__");
    DEFINE_INPLACE_OPERATOR_PAIR('%',"__mod__","__imod__");
    DEFINE_INPLACE_OPERATOR_PAIR('&',"__and__","__iand__");
    DEFINE_INPLACE_OPERATOR_PAIR('|',"__or__","__ior__");
    DEFINE_INPLACE_OPERATOR_PAIR('^',"__xor__","__ixor__");
#undef DEFINE_INPLACE_OPERATOR_PAIR
    if (0) { case '\"': if (doc_iter[1] == '\"') { SET_CONSTANT_OPERATOR_NAME("__str__"); operator_text_begin = operator_name_begin+2; break; } }
    default:
     operator_name_end = operator_name_begin;
     while (operator_name_end != doc_iter &&
            DEE_CH_IS_ALNUM(*operator_name_end)) ++operator_name_end;
     operator_text_begin = operator_name_end;
     break;
   }
#define CHECK_OPERATOR_NAME(name)\
    ((Dee_size_t)(operator_name_end-operator_name_begin)==((sizeof(name)/sizeof(char))-1) &&\
      memcmp(operator_name_begin,name,sizeof(name)-sizeof(char)) == 0)
#define REPLACE_OPERATOR_NAME(old,new)\
   if (CHECK_OPERATOR_NAME(old)) SET_CONSTANT_OPERATOR_NAME(new); else
   REPLACE_OPERATOR_NAME("str","__str__")
   REPLACE_OPERATOR_NAME("repr","__repr__")
   ;
#undef REPLACE_OPERATOR_NAME
#undef SET_CONSTANT_OPERATOR_NAME
   while (operator_text_begin != doc_iter &&
          DEE_CH_IS_SPACE(*operator_text_begin)) ++operator_text_begin;
   operator_text_end = doc_iter;
   {
    struct DeeDoc *operator_doc; int error;
    // Declare a new operator method documentation
    if ((operator_doc = DeeDocs_UnsafeAppend(&self->td_attrib)) == NULL) goto err_this_descr;
    operator_doc->doc_kind = DEE_DOCKIND_METHOD;
    error = DeeMethodDoc_InitFromNameAndDocWithLength(
     &operator_doc->doc_method,DEE_METHODDOC_FLAG_OPERATOR|(ctor_flags&~(DEE_METHODDOC_FLAG_CLASS)),type_name_str,
     (Dee_size_t)(operator_name_end-operator_name_begin),operator_name_begin,
     (Dee_size_t)(operator_text_end-operator_text_begin),operator_text_begin);
    if (error != 0) { DeeDocs_UnsafeRemove(&self->td_attrib); goto err_this_descr; }
   }
#undef CHECK_OPERATOR_NAME
  } else if (!type_doc) {
   // Store the constructor documentation for later...
   type_doc = descr_name_begin;
   type_doc_len = (Dee_size_t)(doc_iter-descr_name_begin);
  } else {
   // Type constructor documentation has already been tagged
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Second constructor documentation found: %q\nFirst: %.*q",
                       descr_name_begin,(unsigned)type_doc_len,type_doc);
   goto err_this_descr;
  }
#undef check_decl
  if (*doc_iter) {
   // Parse more blocks
   goto more_blocks;
  }
 } else {
  DEE_ASSERT(type_doc == NULL); // Reminder...
  type_doc_len = 0;
 }
 if (DeeString_CheckEmpty(self->td_descr)) Dee_CLEAR(self->td_descr);
 if (DeeMethodDoc_InitFromNameAndDocWithLength(&self->td_ctor,ctor_flags,
  type_name_str,type_name ? strlen(type_name) : 0,type_name,type_doc_len,type_doc) != 0) {
err_this_descr:
  Dee_XDECREF(self->td_descr);
  _DeeDocs_Quit(&self->td_attrib);
  Dee_DECREF(type_name_str);
  return -1;
 }
 {
  struct DeeOverloadDoc *over_iter,*over_end;
  over_end = (over_iter = self->td_ctor.md_over.ods_docv)+self->td_ctor.md_over.ods_docc;
  while (over_iter != over_end) {
   if (over_iter->od_return.pd_type == (DeeStringObject *)&_object_name) {
    Dee_DECREF(over_iter->od_return.pd_type);
    Dee_INCREF(over_iter->od_return.pd_type = self->td_ctor.md_name);
   }
   ++over_iter;
  }
 }
 if (DeeDocs_AppendMethodDefs(&self->td_attrib,DEE_METHODDOC_FLAG_CLASS,type_name_str,DeeType_GET_SLOT(ty,tp_class_methods)) != 0) {
err_docs: _DeeMethodDoc_Quit(&self->td_ctor); goto err_this_descr;
 }
 if (DeeDocs_AppendGetSetDefs(&self->td_attrib,DEE_GETSETDOC_FLAG_CLASS,type_name_str,DeeType_GET_SLOT(ty,tp_class_getsets)) != 0) goto err_docs;
 if (DeeDocs_AppendMemberDefs(&self->td_attrib,DEE_MEMBERDOC_FLAG_CLASS,type_name_str,DeeType_GET_SLOT(ty,tp_class_members)) != 0) goto err_docs;
 if (DeeDocs_AppendMethodDefs(&self->td_attrib,DEE_METHODDOC_FLAG_NONE,type_name_str,DeeType_GET_SLOT(ty,tp_methods)) != 0) goto err_docs;
 if (DeeDocs_AppendGetSetDefs(&self->td_attrib,DEE_GETSETDOC_FLAG_NONE,type_name_str,DeeType_GET_SLOT(ty,tp_getsets)) != 0) goto err_docs;
 if (DeeDocs_AppendMemberDefs(&self->td_attrib,DEE_MEMBERDOC_FLAG_NONE,type_name_str,DeeType_GET_SLOT(ty,tp_members)) != 0) goto err_docs;
 ty_base = DeeType_BASE(ty);
 basename = ty_base ? DeeType_NAME(ty_base) : NULL;
 if (basename) {
  if ((self->td_base = (DeeStringObject *)DeeString_New(basename)) == NULL) goto err_docs;
 } else self->td_base = NULL;
 Dee_DECREF(type_name_str);
 return 0;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeTypeDoc_Repr(DEE_A_IN struct DeeTypeDoc const *self) {
 DeeObject *result; struct DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(self);
 if (DeeTypeDoc_ReprToWriter(self,&writer) != 0)
  result = NULL;
 else result = DeeStringWriter_Pack(&writer);
 DeeStringWriter_Quit(&writer);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeTypeDoc_ReprToWriter(
 DEE_A_IN struct DeeTypeDoc const *self,
 DEE_A_INOUT struct DeeStringWriter *writer) {
 struct DeeDoc *iter,*end;
 DEE_ASSERT(self);
 DEE_ASSERT(writer);
 if (self->td_base) {
  if (DeeStringWriter_Writef(writer,"class %k: %k {\n",
   self->td_ctor.md_name,self->td_base) != 0) return -1;
 } else {
  if (DeeStringWriter_Writef(writer,"class %k {\n",self->td_ctor.md_name) != 0) return -1;
 }
 if (DeeStringWriter_Writef(writer,"\t%>K\n",DeeMethodDoc_Repr(&self->td_ctor,NULL)) != 0) return -1;
 end = (iter = self->td_attrib.d_docv)+self->td_attrib.d_docc;
 while (iter != end) {
  if (DeeStringWriter_Writef(writer,"\t%>K\n",DeeDoc_Repr(iter)) != 0) return -1;
  ++iter;
 }
 if (DeeStringWriter_WRITE_STRING(writer,"}\n") != 0) return -1;
 return 0;
}










DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeGenericDoc_Repr(
 DEE_A_IN struct DeeGenericDoc const *self) {
 DeeObject *result; struct DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(self);
 if (DeeGenericDoc_ReprToWriter(self,&writer) != 0)
  result = NULL;
 else result = DeeStringWriter_Pack(&writer);
 DeeStringWriter_Quit(&writer);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeGenericDoc_ReprToWriter(
 DEE_A_IN struct DeeGenericDoc const *self,
 DEE_A_INOUT struct DeeStringWriter *writer) {
 DEE_ASSERT(self);
 DEE_ASSERT(writer);
 return DeeStringWriter_Writef(writer,"%k\n\t%>k\n",self->gd_name,self->gd_doc);
}










//////////////////////////////////////////////////////////////////////////
// Initialize a documentation from any given object
void DeeDoc_Quit(DEE_A_IN struct DeeDoc *self) {
 DEE_ASSERT(self);
 switch (self->doc_kind) {
  case DEE_DOCKIND_GENERIC: _DeeGenericDoc_Quit(&self->doc_generic); break;
  case DEE_DOCKIND_METHOD:  _DeeMethodDoc_Quit(&self->doc_method); break;
  case DEE_DOCKIND_MEMBER:  _DeeMemberDoc_Quit(&self->doc_member); break;
  case DEE_DOCKIND_GETSET:  _DeeGetSetDoc_Quit(&self->doc_getset); break;
  case DEE_DOCKIND_TYPE:    _DeeTypeDoc_Quit(&self->doc_type); break;
  default: break;
 }
}
void _DeeDoc_DoVisit(DEE_A_IN struct DeeDoc *self, DEE_A_IN DeeVisitProc visit, void *closure) {
 DEE_ASSERT(self);
 DEE_ASSERT(visit);
 switch (self->doc_kind) {
  case DEE_DOCKIND_GENERIC: _DeeGenericDoc_Visit(&self->doc_generic); break;
  case DEE_DOCKIND_METHOD:  _DeeMethodDoc_Visit(&self->doc_method); break;
  case DEE_DOCKIND_MEMBER:  _DeeMemberDoc_Visit(&self->doc_member); break;
  case DEE_DOCKIND_GETSET:  _DeeGetSetDoc_Visit(&self->doc_getset); break;
  case DEE_DOCKIND_TYPE:    _DeeTypeDoc_Visit(&self->doc_type); break;
  default: break;
 }
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
DeeDoc_Repr(DEE_A_IN struct DeeDoc const *self) {
 DeeObject *result; struct DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(self);
 if (DeeDoc_ReprToWriter(self,&writer) != 0)
  result = NULL;
 else result = DeeStringWriter_Pack(&writer);
 DeeStringWriter_Quit(&writer);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeDoc_ReprToWriter(
 DEE_A_IN struct DeeDoc const *self,
 DEE_A_INOUT struct DeeStringWriter *writer) {
 DEE_ASSERT(self);
 DEE_ASSERT(writer);
 switch (self->doc_kind) {
  case DEE_DOCKIND_GENERIC: return DeeGenericDoc_ReprToWriter(&self->doc_generic,writer); break;
  case DEE_DOCKIND_METHOD:  return DeeMethodDoc_ReprToWriter(&self->doc_method,writer,"function"); break;
  case DEE_DOCKIND_MEMBER:  return DeeMemberDoc_ReprToWriter(&self->doc_member,writer); break;
  case DEE_DOCKIND_GETSET:  return DeeGetSetDoc_ReprToWriter(&self->doc_getset,writer); break;
  case DEE_DOCKIND_TYPE:    return DeeTypeDoc_ReprToWriter(&self->doc_type,writer); break;
  default: break;
 }
 return DeeStringWriter_WRITE_STRING(writer,"<no documentation available>");
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeDoc_GetName(DEE_A_IN struct DeeDoc const *self) {
 DEE_ASSERT(self);
 switch (self->doc_kind) {
  case DEE_DOCKIND_GENERIC:
   if (self->doc_generic.gd_name) DeeReturn_NEWREF((DeeObject *)self->doc_generic.gd_name);
   DeeReturn_NEWREF((DeeObject *)&_unnamed_name);
   break;
  case DEE_DOCKIND_METHOD:
   DeeReturn_NEWREF((DeeObject *)self->doc_method.md_name);
  case DEE_DOCKIND_MEMBER:
   DeeReturn_NEWREF((DeeObject *)self->doc_member.mbd_name);
  case DEE_DOCKIND_GETSET:
   DeeReturn_NEWREF((DeeObject *)self->doc_getset.gd_name);
  case DEE_DOCKIND_TYPE:
   if (DeeString_CheckEmpty(self->doc_type.td_ctor.md_name))
    DeeReturn_NEWREF((DeeObject *)&_unnamed_name);
   DeeReturn_NEWREF((DeeObject *)self->doc_type.td_ctor.md_name);
  default: break;
 }
 return DeeDoc_Repr(self); // <no documentation available>
}




DEE_A_RET_EXCEPT(-1) int DeeDoc_TInitFromObject(
 DEE_A_OUT struct DeeDoc *self, DEE_A_IN DeeTypeObject const *tp_ob,
 DEE_A_INOUT DeeObject *ob) {
 DeeString_NEW_STATIC(attr___name__,"__name__");
 DeeString_NEW_STATIC(attr___doc__,"__doc__");
 DeeObject *doc_attr,*name_attr;
 Dee_uint16_t builtin_id; char const *name,*doc;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(ob));
 DEE_ASSERT(DeeObject_Check(tp_ob) && DeeType_Check(tp_ob));
 DEE_ASSERT(DeeObject_InstanceOf(ob,tp_ob));
 if (DeeType_IsSameOrDerived(tp_ob,&DeeType_Type)) {
  self->doc_kind = DEE_DOCKIND_TYPE;
  return DeeTypeDoc_InitFromType(&self->doc_type,(DeeTypeObject *)ob);
 }
 if (tp_ob == &DeeMemberFunction_Type) {
  DeeObject *memberf_self; int error;
  if ((((DeeMemberFunctionObject *)ob)->mf_flags&DEE_MEMBERFUNCTION_FLAG_STRUCTURED_CALL)==0) {
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   Dee_INCREF(memberf_self = ((DeeMemberFunctionObject *)ob)->mf_object.mfo_self);
#else
   memberf_self = ((DeeMemberFunctionObject *)ob)->mf_object.mfo_self;
#endif
  }
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  else {
   memberf_self = DeeLValue_New(
    (DeeTypeObject *)((DeeMemberFunctionObject *)ob)->mf_struct.mfs_type,
    ((DeeMemberFunctionObject *)ob)->mf_struct.mfs_self);
   if (!memberf_self) return -1;
  }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  {
   error = DeeDoc_TInitFromObjectAttrString(self,Dee_TYPE(memberf_self),memberf_self,
    (((DeeMemberFunctionObject *)ob)->mf_flags&DEE_MEMBERFUNCTION_FLAG_MEMBERNAME_OB)!=0
    ? DeeString_STR(((DeeMemberFunctionObject *)ob)->mf_member_o)
    : ((DeeMemberFunctionObject *)ob)->mf_member_s);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
   Dee_DECREF(memberf_self);
#endif
   return error;
  }
 }
 if (tp_ob == &DeeCFunction_Type) {
  Dee_uint32_t cfunction_flags = DEE_METHODDOC_FLAG_NONE;
  self->doc_kind = DEE_DOCKIND_METHOD;
  if (_DeeBuiltin_AllowConstantCall(ob)) cfunction_flags |= DEE_METHODDOC_FLAG_CONSTEXPR;
  if ((builtin_id = DeeBuiltin_Id(ob)) != (Dee_uint16_t)-1) {
   doc = _DeeBuiltin_GetDoc(builtin_id,&name);
  } else doc = NULL,name = NULL;
  if (!name) name = DeeCFunction_NAME(ob);
  if (!doc) doc = DeeCFunction_DOC(ob);
  return DeeMethodDoc_InitFromNameAndDoc(&self->doc_method,cfunction_flags,NULL,name,doc);
 }
 if (tp_ob == &DeeSuper_Type) return DeeDoc_TInitFromObject(self,DeeSuper_TYPE(ob),DeeSuper_SELF(ob));
 // Fallback: Search for a __doc__ and __name__ attribute
 if ((doc_attr = DeeObject_TGetAttr(tp_ob,ob,(DeeObject *)&attr___doc__)) == NULL) {
  if (!DeeError_Catch(&DeeErrorType_AttributeError) &&
      !DeeError_Catch(&DeeErrorType_NotImplemented)) return -1;
 } else {
  if (DeeString_InplaceCast((DeeObject const **)&doc_attr) != 0) goto err_doc_attr;
  if ((name_attr = DeeObject_TGetAttr(tp_ob,ob,(DeeObject *)&attr___name__)) == NULL) {
   if (!DeeError_Catch(&DeeErrorType_AttributeError) &&
       !DeeError_Catch(&DeeErrorType_NotImplemented))
   {err_doc_attr: Dee_DECREF(doc_attr); return -1; }
  } else {
   if (DeeString_InplaceCast((DeeObject const **)&name_attr) != 0)
   {/*err_name_attr:*/ Dee_DECREF(name_attr); goto err_doc_attr; }
  }
  self->doc_kind = DEE_DOCKIND_GENERIC;
  Dee_INHERIT_REF(self->doc_generic.gd_doc,*(DeeStringObject **)&doc_attr);
  Dee_INHERIT_REF(self->doc_generic.gd_name,*(DeeStringObject **)&name_attr);
  return 0;
 }
 // Last chance: Is this one of those really obscure builtins?
 if (tp_ob == Dee_TYPE(ob)) {
  if ((builtin_id = DeeBuiltin_Id(ob)) != (Dee_uint16_t)-1) {
   char const *name,*doc;
   if ((doc = _DeeBuiltin_GetDoc(builtin_id,&name)) != NULL) {
    // NOTE: It can't be a cfunction, because that was hadled earlier
    self->doc_kind = DEE_DOCKIND_GENERIC;
    if (!name)self->doc_generic.gd_name = NULL;
    else if ((self->doc_generic.gd_name = (DeeStringObject *)DeeString_New(name)) == NULL) return -1;
    if (!doc) self->doc_generic.gd_doc = NULL;
    else if ((self->doc_generic.gd_doc = (DeeStringObject *)DeeString_New(doc)) == NULL) {
     Dee_DECREF(self->doc_generic.gd_name);
     return  -1;
    }
    // Done!
    return 0;
   }
  }
 }

 // No documentation available :(
 self->doc_kind = DEE_DOCKIND_NONE;
 return 0;
}


static void _error_doc_attr_not_found(DeeTypeObject const *ty, char const *attr) {
 DEE_ASSERT(attr);
 DEE_ASSERT(DeeObject_Check(ty) && DeeType_Check(ty));
 DeeError_SetStringf(&DeeErrorType_AttributeError,
                     "Failed to find attribute %q in %q",
                     attr,DeeType_NAME(ty));
}

DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDoc_TInitFromGenericObjectAttrString(
 DEE_A_OUT struct DeeDoc *self, DEE_A_IN DeeTypeObject const *tp_ob,
 DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN_Z char const *attr) {
 DeeTypeObject const *tp_iter;
 struct DeeMemberDef const *members;
 struct DeeMethodDef const *methods;
 struct DeeGetSetDef const *getsets;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(tp_ob) && DeeType_Check(tp_ob));
 DEE_ASSERT(DeeObject_Check(this_type) && DeeString_Check(this_type));
 DEE_ASSERT(attr);
 tp_iter = tp_ob;
 while (1) {
  members = DeeType_GET_SLOT(tp_iter,tp_members); DEE_ASSERT(members);
  while (members->d_name) {
   if (strcmp(members->d_name,attr) == 0) {
    self->doc_kind = DEE_DOCKIND_MEMBER; // Found it
    return DeeMemberDoc_InitFromMemberDef(&self->doc_member,DEE_MEMBERDOC_FLAG_NONE,this_type,members);
   }
   ++members;
  }
  methods = DeeType_GET_SLOT(tp_iter,tp_methods); DEE_ASSERT(methods);
  while (methods->d_name) {
   if (strcmp(methods->d_name,attr) == 0) {
    self->doc_kind = DEE_DOCKIND_METHOD; // Found it
    return DeeMethodDoc_InitFromMethodDef(&self->doc_method,DEE_METHODDOC_FLAG_NONE,this_type,methods);
   }
   ++methods;
  }
  getsets = DeeType_GET_SLOT(tp_iter,tp_getsets); DEE_ASSERT(getsets);
  while (getsets->d_name) {
   if (strcmp(getsets->d_name,attr) == 0) {
    self->doc_kind = DEE_DOCKIND_GETSET; // Found it
    return DeeGetSetDoc_InitFromGetSetDef(&self->doc_getset,DEE_GETSETDOC_FLAG_NONE,this_type,getsets);
   }
   ++getsets;
  }
  if ((tp_iter = DeeType_BASE(tp_iter)) == NULL) break;
 }
 return 1;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeDoc_TInitFromTypeAttrString(
 DEE_A_OUT struct DeeDoc *self, DEE_A_INOUT DeeTypeObject const *ob,
 DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN_Z char const *attr) {
 DeeTypeObject const *tp_iter; int error;
 struct DeeMemberDef const *members;
 struct DeeMethodDef const *methods;
 struct DeeGetSetDef const *getsets;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(ob) && DeeType_Check(ob));
 DEE_ASSERT(DeeObject_Check(this_type) && DeeString_Check(this_type));
 DEE_ASSERT(attr);
 tp_iter = ob;
 while (1) {
  if ((error = DeeDoc_TInitFromGenericObjectAttrString(self,Dee_TYPE(tp_iter),this_type,attr)) <= 0) return error;
  members = DeeType_GET_SLOT(tp_iter,tp_class_members); DEE_ASSERT(members);
  while (members->d_name) {
   if (strcmp(members->d_name,attr) == 0) {
found_member:
    self->doc_kind = DEE_DOCKIND_MEMBER; // Found it
    return DeeMemberDoc_InitFromMemberDef(&self->doc_member,DEE_MEMBERDOC_FLAG_NONE,this_type,members);
   }
   ++members;
  }
  methods = DeeType_GET_SLOT(tp_iter,tp_class_methods); DEE_ASSERT(methods);
  while (methods->d_name) {
   if (strcmp(methods->d_name,attr) == 0) {
found_method:
    self->doc_kind = DEE_DOCKIND_METHOD; // Found it
    return DeeMethodDoc_InitFromMethodDef(&self->doc_method,DEE_METHODDOC_FLAG_NONE,this_type,methods);
   }
   ++methods;
  }
  getsets = DeeType_GET_SLOT(tp_iter,tp_class_getsets); DEE_ASSERT(getsets);
  while (getsets->d_name) {
   if (strcmp(getsets->d_name,attr) == 0) {
found_getset:
    self->doc_kind = DEE_DOCKIND_GETSET; // Found it
    return DeeGetSetDoc_InitFromGetSetDef(&self->doc_getset,DEE_GETSETDOC_FLAG_NONE,this_type,getsets);
   }
   ++getsets;
  }
  members = DeeType_GET_SLOT(tp_iter,tp_members); DEE_ASSERT(members);
  while (members->d_name) { if (strcmp(members->d_name,attr) == 0) goto found_member; ++members; }
  methods = DeeType_GET_SLOT(tp_iter,tp_methods); DEE_ASSERT(methods);
  while (methods->d_name) { if (strcmp(methods->d_name,attr) == 0) goto found_method; ++methods; }
  getsets = DeeType_GET_SLOT(tp_iter,tp_getsets); DEE_ASSERT(getsets);
  while (getsets->d_name) { if (strcmp(getsets->d_name,attr) == 0) goto found_getset; ++getsets; }
  if ((tp_iter = DeeType_BASE(tp_iter)) == NULL) break;
 }
 return 1;
}


DEE_A_RET_EXCEPT(-1) int DeeDoc_TInitFromObjectAttrString(
 DEE_A_OUT struct DeeDoc *self, DEE_A_IN DeeTypeObject const *tp_ob,
 DEE_A_INOUT DeeObject *ob, DEE_A_IN_Z char const *attr) {
 DeeTypeObject const *tp_iter; DeeObject *attr_value;
 DeeStringObject *this_type;
 DeeType_SLOT_TYPE(tp_attr_get) protocol; int error;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(ob));
 DEE_ASSERT(DeeObject_Check(tp_ob) && DeeType_Check(tp_ob));
 DEE_ASSERT(DeeObject_InstanceOf(ob,tp_ob));
 DEE_ASSERT(attr);
 tp_iter = tp_ob; DeeType_FIND_SLOT(tp_iter,tp_attr_get);
 protocol = DeeType_GET_SLOT(tp_iter,tp_attr_get);
 if (protocol == (DeeType_SLOT_TYPE(tp_attr_get))&DeeObject_GenericGetAttr) {
  if (DeeType_NAME(tp_ob)) {
   if ((this_type = (DeeStringObject *)DeeString_New(DeeType_NAME(tp_ob))) == NULL) return -1;
  } else Dee_INCREF(this_type = (DeeStringObject *)&_unnamed_name);
  error = DeeDoc_TInitFromGenericObjectAttrString(self,tp_ob,this_type,attr);
  Dee_DECREF(this_type);
  if (error <= 0) return error;
  _error_doc_attr_not_found(tp_ob,attr);
  return -1;
 }
 if (protocol == (DeeType_SLOT_TYPE(tp_attr_get))&DeeType_GetAttr) {
  DEE_ASSERT(DeeType_Check(ob));
  if (DeeType_NAME(ob)) {
   if ((this_type = (DeeStringObject *)DeeString_New(DeeType_NAME(ob))) == NULL) return -1;
  } else Dee_INCREF(this_type = (DeeStringObject *)&_unnamed_name);
  error = DeeDoc_TInitFromTypeAttrString(self,(DeeTypeObject *)ob,this_type,attr);
  Dee_DECREF(this_type);
  if (error <= 0) return error;
  _error_doc_attr_not_found((DeeTypeObject *)ob,attr);
  return -1;
 }
 if (protocol == (DeeType_SLOT_TYPE(tp_attr_get))&_deesuper_tp_attr_get) {
  return DeeDoc_TInitFromObjectAttrString(self,DeeSuper_TYPE(ob),DeeSuper_SELF(ob),attr);
 }
 if (DeeClass_Check(tp_ob)) {
  // TODO: protocol == (DeeType_SLOT_TYPE(tp_attr_get))&DeeInstance_GetAttr
 }
 if (DeeClassType_Check(tp_ob)) {
  // TODO: protocol == (DeeType_SLOT_TYPE(tp_attr_get))&DeeClass_GetAttr
 }
 if (protocol == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStructured_GetAttr) {
  // TODO
 }
 if (DeeStructType_Check(tp_ob)) {
  // TODO: protocol == (DeeType_SLOT_TYPE(tp_attr_get))&DeeStruct_GetAttr
 }
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (protocol == (DeeType_SLOT_TYPE(tp_attr_get))&_deelvalue_tp_attr_get) {
  // TODO
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if (protocol == (DeeType_SLOT_TYPE(tp_attr_get))&_deedexmodule_tp_attr_get) {
  // TODO
 }

 // Fallback: Initialize the doc with the actual attribute
 if ((attr_value = DeeObject_TGetAttrString(tp_ob,ob,attr)) == NULL) return -1;
 if (DeeMemberFunction_Check(attr_value)) {
  // Don't cause a stack overflow
  self->doc_kind = DEE_DOCKIND_NONE;
  error = 0;
 } else {
  error = DeeDoc_TInitFromObject(self,Dee_TYPE(attr_value),attr_value);
 }
 Dee_DECREF(attr_value);
 return error;
}







DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeDoc_TStringFromObject(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) {
 struct DeeDoc doc; DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 if (DeeDoc_TInitFromObject(&doc,tp_self,self) != 0) return NULL;
 result = DeeDoc_Repr(&doc);
 DeeDoc_Quit(&doc);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeDoc_TStringFromObjectAttrString(
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr) {
 struct DeeDoc doc; DeeObject *result;
 DEE_ASSERT(DeeObject_Check(self));
 DEE_ASSERT(DeeObject_Check(tp_self) && DeeType_Check(tp_self));
 DEE_ASSERT(DeeObject_InstanceOf(self,tp_self));
 DEE_ASSERT(attr);
 if (DeeDoc_TInitFromObjectAttrString(&doc,tp_self,self,attr) != 0) return NULL;
 result = DeeDoc_Repr(&doc);
 DeeDoc_Quit(&doc);
 return result;
}

static DeeObject *DEE_CALL _deedoc___getdoc__(DeeObject *args) {
 DeeObject *ob,*attr = NULL;
 if (DeeTuple_Unpack(args,"o|o:__getdoc__",&ob,&attr) != 0) return NULL;
 if (attr) {
  if (DeeError_TypeError_CheckTypeExact(attr,&DeeString_Type) != 0) return NULL;
  return DeeDoc_StringFromObjectAttr(ob,attr);
 }
 return DeeDoc_TStringFromObject(Dee_TYPE(ob),ob);
}





//////////////////////////////////////////////////////////////////////////
// Module exports
struct DeeDexExportDef DeeDex_Exports[] = {
 DeeDex_EXPORT_OBJECT("doc",&DeeDoc_Type),
 DeeDex_EXPORT_OBJECT("docs",&DeeDocsRef_Type),
 DeeDex_EXPORT_FUNCTION("__getdoc__",&_deedoc___getdoc__,
  "(object ob) -> string\n"
  "(object ob, string attr) -> string\n"
  "@param ob: The object to query the doc of\n"
  "@param attr: An optional attribute describing the name of a specific attribute to query for\n"
  "@return: The doc associated with a given object/attribute\n"
  "\tReturns a human-readable documentation string for a given object/attribute"),
 DeeDex_EXPORT_END
};

DEE_DECL_END

#include "autodoc.c.inl"
#ifndef __INTELLISENSE__
#include "wrapper/DeeDoc_Type.inl"
#include "wrapper/DeeDocsRef_Type.inl"
#include "wrapper/DeeDocsRefIterator_Type.inl"
#include "wrapper/DeeGenericDocRef_Type.inl"
#include "wrapper/DeeGetSetDocRef_Type.inl"
#include "wrapper/DeeMemberDocRef_Type.inl"
#include "wrapper/DeeMethodDocRef_Type.inl"
#include "wrapper/DeeOverloadDocRef_Type.inl"
#include "wrapper/DeeOverloadDocsRefIterator_Type.inl"
#include "wrapper/DeeOverloadDocsRef_Type.inl"
#include "wrapper/DeeParamDocRef_Type.inl"
#include "wrapper/DeeParamDocsRefIterator_Type.inl"
#include "wrapper/DeeParamDocsRef_Type.inl"
#include "wrapper/DeeThrowDocRef_Type.inl"
#include "wrapper/DeeThrowDocsRefIterator_Type.inl"
#include "wrapper/DeeThrowDocsRef_Type.inl"
#include "wrapper/DeeTypeDocRef_Type.inl"
#endif

#endif /* !GUARD_DEEMON_DEX_DOCGEN_C */
