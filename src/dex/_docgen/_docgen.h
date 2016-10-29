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
#ifndef GUARD_DEEMON_DEX_DOCGEN_H
#define GUARD_DEEMON_DEX_DOCGEN_H 1
 
#include <deemon/__conf.inl>
#include <deemon/string.h>
#include <deemon/memberdef.h>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/memberdef_decl.h>
#include <deemon/optional/object_visit.h>

DEE_DECL_BEGIN

struct DeeDoc;
DEE_OBJECT_DEF(DeeDocObject);
DEE_OBJECT_DEF(DeeGenericDocRefObject);
DEE_OBJECT_DEF(DeeMethodDocRefObject);
DEE_OBJECT_DEF(DeeOverloadDocsRefObject);
DEE_OBJECT_DEF(DeeOverloadDocsRefIteratorObject);
DEE_OBJECT_DEF(DeeOverloadDocRefObject);
DEE_OBJECT_DEF(DeeParamDocsRefObject);
DEE_OBJECT_DEF(DeeParamDocsRefIteratorObject);
DEE_OBJECT_DEF(DeeParamDocRefObject);
DEE_OBJECT_DEF(DeeThrowDocsRefObject);
DEE_OBJECT_DEF(DeeThrowDocsRefIteratorObject);
DEE_OBJECT_DEF(DeeThrowDocRefObject);
DEE_OBJECT_DEF(DeeMemberDocRefObject);
DEE_OBJECT_DEF(DeeGetSetDocRefObject);
DEE_OBJECT_DEF(DeeTypeDocRefObject);
DEE_OBJECT_DEF(DeeDocsRefObject);
DEE_OBJECT_DEF(DeeDocsRefIteratorObject);

// Hide names starting with two underscores
// NOTE: Explicitly requesting the doc of such a member will yields the expected result.
// >> doc(type object); // Won't list '__set_name' (in devel builds)
// >> doc(type object,"__set_name"); // Still available like this
// NOTE: Though we still show names ending with two underscores as well
#define DEE_DOC_IS_HIDDEN_NAME DEE_DOC_IS_HIDDEN_NAME
DEE_STATIC_INLINE(int) DEE_DOC_IS_HIDDEN_NAME(char const *name) {
 Dee_size_t name_len;
 if (name[0] != '_' || name[1] != '_') return 0;
 name_len = strlen(name);
 if (name_len >= 4 && name[name_len-1] == '_' &&
     name[name_len-2] == '_') return 0; // Ends with two '_' as well
 return 1;
}
 


#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif

#define DEE_DOC_DATADECL(T) extern DEE_ATTRIBUTE_DLLEXPORT T
#define DEE_DOC_FUNCDECL(T) extern DEE_ATTRIBUTE_DLLEXPORT T DEE_CALL

struct DeeDocs {
 Dee_size_t     d_docc; /*< Amount of documentations. */
 struct DeeDoc *d_docv; /*< [0..d_docc][owned] Vector of documentations. */
};
#define _DeeDocs_Init(ob)\
do{ (ob)->d_docc = 0; (ob)->d_docv = NULL; }while(0)
#define _DeeDocs_Quit(ob)\
do{\
 struct DeeDoc *_d_iter,*_d_end;\
 _d_end = (_d_iter = (ob)->d_docv)+(ob)->d_docc;\
 while (_d_iter != _d_end) { DeeDoc_Quit(_d_iter); ++_d_iter; }\
 free((ob)->d_docv);\
}while(0)
#define _DeeDocs_Visit(ob)\
do{\
 struct DeeDoc *_d_iter,*_d_end;\
 _d_end = (_d_iter = (ob)->d_docv)+(ob)->d_docc;\
 while (_d_iter != _d_end) { _DeeDoc_Visit(_d_iter); ++_d_iter; }\
}while(0)


//////////////////////////////////////////////////////////////////////////
// Returns a pointer to a newly allocated generic doc at the end of 'self's list
// The caller is responsible for properly initializing that pointer,
// and if initialization fails, 'DeeDocs_UnsafeRemove' must be called
extern struct DeeDoc *DeeDocs_UnsafeAppend(DEE_A_INOUT struct DeeDocs *self);
#define DeeDocs_UnsafeRemove(ob) (void)--(ob)->d_docc
extern DEE_A_RET_EXCEPT(-1) int DeeDocs_AppendMemberDef(DEE_A_INOUT struct DeeDocs *self, DEE_A_IN Dee_uint32_t flags, DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeMemberDef const *def);
extern DEE_A_RET_EXCEPT(-1) int DeeDocs_AppendMethodDef(DEE_A_INOUT struct DeeDocs *self, DEE_A_IN Dee_uint32_t flags, DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeMethodDef const *def);
extern DEE_A_RET_EXCEPT(-1) int DeeDocs_AppendGetSetDef(DEE_A_INOUT struct DeeDocs *self, DEE_A_IN Dee_uint32_t flags, DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeGetSetDef const *def);
extern DEE_A_RET_EXCEPT(-1) int DeeDocs_AppendMemberDefs(DEE_A_INOUT struct DeeDocs *self, DEE_A_IN Dee_uint32_t flags, DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeMemberDef const *defs);
extern DEE_A_RET_EXCEPT(-1) int DeeDocs_AppendMethodDefs(DEE_A_INOUT struct DeeDocs *self, DEE_A_IN Dee_uint32_t flags, DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeMethodDef const *defs);
extern DEE_A_RET_EXCEPT(-1) int DeeDocs_AppendGetSetDefs(DEE_A_INOUT struct DeeDocs *self, DEE_A_IN Dee_uint32_t flags, DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeGetSetDef const *defs);

extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
 DeeDocs_Repr(DEE_A_IN struct DeeDocs const *self);
extern DEE_A_RET_EXCEPT(-1) int DeeDocs_ReprToWriter(
 DEE_A_IN struct DeeDocs const *self, DEE_A_INOUT struct DeeStringWriter *writer);





struct DeeThrowDoc {
 // Declaration: '@throws <td_name> [(',' <td_name>)...]: <td_doc>'
 // NOTE: '@throws *' -> '@throws object'
 DEE_A_REF DeeStringObject *td_name; /*< [1..1] Name of the error that is thrown. */
 DEE_A_REF DeeStringObject *td_doc;  /*< [0..1] Text explaining when the error is thrown (NULL if not given). */
};

//////////////////////////////////////////////////////////////////////////
// Initializer a throw doc from a given declaration string:
//           <--------------------------> decl_len
// >> @throws Error: Something went wrong
//           ^ <decl_str
extern DEE_A_RET_EXCEPT(-1) int DeeThrowDoc_InitFromDoc(
 DEE_A_OUT struct DeeThrowDoc *self, DEE_A_IN Dee_size_t decl_len,
 DEE_A_IN_R(decl_len) char const *decl_str);
#define _DeeThrowDoc_Quit(ob)\
do{\
 Dee_DECREF((ob)->td_name);\
 Dee_XDECREF((ob)->td_doc);\
}while(0)
#define _DeeThrowDoc_Visit(ob)\
do{\
 Dee_VISIT((ob)->td_name);\
 Dee_XVISIT((ob)->td_doc);\
}while(0)

extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeThrowDoc_Repr(
 DEE_A_IN struct DeeThrowDoc const *self);
extern DEE_A_RET_EXCEPT(-1) int DeeThrowDoc_ReprToWriter(
 DEE_A_IN struct DeeThrowDoc const *self,
 DEE_A_INOUT struct DeeStringWriter *writer);





struct DeeThrowDocs {
 Dee_size_t          tds_throwc; /*< Amount of throw docs. */
 struct DeeThrowDoc *tds_throwv; /*< [0..1][owned] Documentation on thrown exceptions. */
};
#define _DeeThrowDocs_Init(ob)\
do{ (ob)->tds_throwc = 0; (ob)->tds_throwv = NULL; }while(0)
#define _DeeThrowDocs_Quit(ob)\
do{\
 struct DeeThrowDoc *_td_iter,*_td_end;\
 _td_end = (_td_iter = (ob)->tds_throwv)+(ob)->tds_throwc;\
 while (_td_iter != _td_end) { _DeeThrowDoc_Quit(_td_iter); ++_td_iter; }\
 free((ob)->tds_throwv);\
}while(0)
#define _DeeThrowDocs_Visit(ob)\
do{\
 struct DeeThrowDoc *_td_iter,*_td_end;\
 _td_end = (_td_iter = (ob)->tds_throwv)+(ob)->tds_throwc;\
 while (_td_iter != _td_end) { _DeeThrowDoc_Visit(_td_iter); ++_td_iter; }\
}while(0)

//////////////////////////////////////////////////////////////////////////
// Returns a pointer to a newly allocated throw doc at the end of 'self's list
// The caller is responsible for properly initializing that pointer,
// and if initialization fails, 'DeeThrowDocs_UnsafeRemove' must be called
extern struct DeeThrowDoc *DeeThrowDocs_UnsafeAppend(DEE_A_INOUT struct DeeThrowDocs *self);
#define DeeThrowDocs_UnsafeRemove(ob) (void)--(ob)->tds_throwc

//////////////////////////////////////////////////////////////////////////
// Appends a new doc and tries to initialize it by calling 'DeeThrowDoc_InitFromDoc'
// NOTE: An additional check is performed to ensure that
//       the exception name isn't already documented/used
//       If it is, an Error.ValueError is thrown.
extern DEE_A_RET_EXCEPT(-1) int DeeThrowDocs_AddDoc(
 DEE_A_INOUT struct DeeThrowDocs *self, DEE_A_IN Dee_size_t decl_len,
 DEE_A_IN_R(decl_len) char const *decl_str);

extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeThrowDocs_Repr(
 DEE_A_IN struct DeeThrowDocs const *self);
extern DEE_A_RET_EXCEPT(-1) int DeeThrowDocs_ReprToWriter(
 DEE_A_IN struct DeeThrowDocs const *self,
 DEE_A_INOUT struct DeeStringWriter *writer);









struct DeeParamDoc {
 // Declaration: '@param <pd_name> [(',' <pd_name>)...]: <pd_doc>'
 // Declaration: '@return: <pd_doc>'
 // NOTE: 'pd_name' must also appear in the argument list
#define DEE_PARAMDOC_FLAG_NONE    DEE_UINT32_C(0x00000000)
#define DEE_PARAMDOC_FLAG_VARG    DEE_UINT32_C(0x00000001) /*< Var-arg parameter. */
#define DEE_PARAMDOC_FLAG_RETURN  DEE_UINT32_C(0x00000002) /*< Return parameter. */
 Dee_uint32_t               pd_flags; /*< Parameter flags. */
 DEE_A_REF DeeStringObject *pd_type;  /*< [1..1] Type of the argument ('object' if not known). */
 DEE_A_REF DeeStringObject *pd_name;  /*< [0..1] Name of the argument (NULL if not known or return argument). */
 DEE_A_REF DeeStringObject *pd_doc;   /*< [0..1] Text explaining what is returned. */
 DEE_A_REF DeeStringObject *pd_def;   /*< [0..1] Default parameter value. */
};
#define _DeeParamDoc_Quit(ob)\
do{\
 Dee_DECREF((ob)->pd_type);\
 Dee_XDECREF((ob)->pd_name);\
 Dee_XDECREF((ob)->pd_doc);\
 Dee_XDECREF((ob)->pd_def);\
}while(0)
#define _DeeParamDoc_Visit(ob)\
do{\
 Dee_VISIT((ob)->pd_type);\
 Dee_XVISIT((ob)->pd_name);\
 Dee_XVISIT((ob)->pd_doc);\
 Dee_XVISIT((ob)->pd_def);\
}while(0)

//////////////////////////////////////////////////////////////////////////
// Initializer a param doc from a given declaration string:
//                 <---> decl_len
// >> function add(int a, int b) -> int;
//                 ^ <decl_str
extern DEE_A_RET_EXCEPT(-1) int DeeParamDoc_InitFromArgDecl(
 DEE_A_OUT struct DeeParamDoc *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN Dee_size_t decl_len, DEE_A_IN_R(decl_len) char const *decl_str);

//////////////////////////////////////////////////////////////////////////
// Add a documentation string to a param decl:
//             <-----------------------> doc_len
// >> @param a: The first input paramter
//             ^ <doc_str
extern DEE_A_RET_EXCEPT(-1) int DeeParamDoc_AddDoc(
 DEE_A_INOUT struct DeeParamDoc *self,
 DEE_A_IN Dee_size_t doc_len, DEE_A_IN_R(doc_len) char const *doc_str);

extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeParamDoc_Repr(
 DEE_A_IN struct DeeParamDoc const *self);
extern DEE_A_RET_EXCEPT(-1) int DeeParamDoc_ReprToWriter(
 DEE_A_IN struct DeeParamDoc const *self,
 DEE_A_INOUT struct DeeStringWriter *writer);




struct DeeParamDocs {
 Dee_size_t          pds_argc; /*< Amount of argument docs. */
 struct DeeParamDoc *pds_argv; /*< [0..1][owned] Documentation of each argument (Index is argument id). */
};
#define _DeeParamDocs_Init(ob)\
do{ (ob)->pds_argc = 0; (ob)->pds_argv = NULL; }while(0)
#define _DeeParamDocs_Quit(ob)\
do{\
 struct DeeParamDoc *_pd_iter,*_pd_end;\
 _pd_end = (_pd_iter = (ob)->pds_argv)+(ob)->pds_argc;\
 while (_pd_iter != _pd_end) { _DeeParamDoc_Quit(_pd_iter); ++_pd_iter; }\
 free((ob)->pds_argv);\
}while(0)
#define _DeeParamDocs_Visit(ob)\
do{\
 struct DeeParamDoc *_pd_iter,*_pd_end;\
 _pd_end = (_pd_iter = (ob)->pds_argv)+(ob)->pds_argc;\
 while (_pd_iter != _pd_end) { _DeeParamDoc_Visit(_pd_iter); ++_pd_iter; }\
}while(0)
//////////////////////////////////////////////////////////////////////////
// Returns a pointer to a newly allocated param doc at the end of 'self's list
// The caller is responsible for properly initializing that pointer,
// and if initialization fails, 'DeeParamDocs_UnsafeRemove' must be called
extern struct DeeParamDoc *DeeParamDocs_UnsafeAppend(DEE_A_INOUT struct DeeParamDocs *self);
#define DeeParamDocs_UnsafeRemove(ob) (void)--(ob)->pds_argc

//////////////////////////////////////////////////////////////////////////
// Appends a new doc and tries to initialize it by calling 'DeeParamDoc_InitFromArgDecl'
// NOTE: An additional check is performed to ensure that
//       the parameter name isn't already documented/used
//       If it is, an Error.ValueError is thrown.
extern DEE_A_RET_EXCEPT(-1) int DeeParamDocs_AppendParam(
 DEE_A_INOUT struct DeeParamDocs *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN Dee_size_t decl_len, DEE_A_IN_R(decl_len) char const *decl_str);

//////////////////////////////////////////////////////////////////////////
// Append a list of arguments from a given declaration string:
//                 <----------> decl_len
// >> function add(int a, int b) -> int;
//                 ^ <decl_str
//    -> Would append 'int a' and 'int b'
extern DEE_A_RET_EXCEPT(-1) int DeeParamDocs_AppendParams(
 DEE_A_INOUT struct DeeParamDocs *self, DEE_A_IN Dee_size_t decl_len,
 DEE_A_IN_R(decl_len) char const *decl_str);

//////////////////////////////////////////////////////////////////////////
// Add a documentation string to a param declaration:
//          <--------------------------> doc_len
// >> @param a: The first input paramter
//          ^ <doc_str
extern DEE_A_RET_EXCEPT(-1) int DeeParamDocs_AddDoc(
 DEE_A_INOUT struct DeeParamDocs *self,
 DEE_A_IN Dee_size_t doc_len, DEE_A_IN_R(doc_len) char const *doc_str);

extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeParamDocs_Repr(
 DEE_A_IN struct DeeParamDocs const *self);
extern DEE_A_RET_EXCEPT(-1) int DeeParamDocs_ReprToWriter(
 DEE_A_IN struct DeeParamDocs const *self,
 DEE_A_INOUT struct DeeStringWriter *writer);











struct DeeOverloadDoc {
 DEE_A_REF DeeStringObject *od_doc;    /*< [0..1] Human-readable, generic documentation. */
 struct DeeParamDoc         od_return; /*< Documentation for the return value. */
 struct DeeParamDocs        od_params; /*< Argument documentation. */
 struct DeeThrowDocs        od_throws; /*< Exception documentation. */
};
#define _DeeOverloadDoc_Quit(ob)\
do{\
 Dee_XDECREF((ob)->od_doc);\
 _DeeParamDoc_Quit(&(ob)->od_return);\
 _DeeParamDocs_Quit(&(ob)->od_params);\
 _DeeThrowDocs_Quit(&(ob)->od_throws);\
}while(0)
#define _DeeOverloadDoc_Visit(ob)\
do{\
 Dee_XVISIT((ob)->od_doc);\
 _DeeParamDoc_Visit(&(ob)->od_return);\
 _DeeParamDocs_Visit(&(ob)->od_params);\
 _DeeThrowDocs_Visit(&(ob)->od_throws);\
}while(0)

//////////////////////////////////////////////////////////////////////////
// Initialize an overload from a given doc string:
//                <-------------------> decl_len
// >> function add(int a, int b) -> int;
//                ^ <decl_str
//                <-----> decl_len
// >> function add -> int;
//                ^ <decl_str
extern DEE_A_RET_EXCEPT(-1) int DeeOverloadDoc_InitFromDoc(
 DEE_A_OUT struct DeeOverloadDoc *self, DEE_A_IN Dee_size_t decl_len,
 DEE_A_IN_R(decl_len) char const *decl_str);

//////////////////////////////////////////////////////////////////////////
// Adds additional documentation to the overload:
// >> DeeOverloadDoc_AddDoc("\tSome random function\n@throws Error: Something went wrong\n@param x: The x argument");
// NOTES:
//   - Generic documentation lines are prefixed with '\t'
//     Each line should be treated as its own section of
//     text, meaning linefeeds should not be placed manually to wrap text.
//   - Additional docs can be provided for '@throws', '@param' and '@return'
//   - Unknown documentation 
extern DEE_A_RET_EXCEPT(-1) int DeeOverloadDoc_AddDoc(
 DEE_A_INOUT struct DeeOverloadDoc *self, DEE_A_IN Dee_size_t doc_len,
 DEE_A_IN_R(doc_len) char const *doc_str);

//////////////////////////////////////////////////////////////////////////
// Add generic documentation text, as written after a '\t'
extern DEE_A_RET_EXCEPT(-1) int DeeOverloadDoc_AddText(
 DEE_A_INOUT struct DeeOverloadDoc *self, DEE_A_IN Dee_size_t text_len,
 DEE_A_IN_R(text_len) char const *text_str);

extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeOverloadDoc_Repr(
 DEE_A_IN struct DeeOverloadDoc const *self, DEE_A_IN Dee_uint32_t function_flags,
 DEE_A_IN_Z_OPT char const *function_name, DEE_A_IN_Z_OPT char const *prefix);
extern DEE_A_RET_EXCEPT(-1) int DeeOverloadDoc_ReprToWriter(
 DEE_A_IN struct DeeOverloadDoc const *self,
 DEE_A_INOUT struct DeeStringWriter *writer, DEE_A_IN Dee_uint32_t function_flags,
 DEE_A_IN_Z_OPT char const *function_name, DEE_A_IN_Z_OPT char const *prefix);



struct DeeOverloadDocs {
 Dee_size_t             ods_docc; /*< Amount of overload docs. */
 struct DeeOverloadDoc *ods_docv; /*< [0..1][owned] Documentation of each overload. */
};
#define _DeeOverloadDocs_Init(ob)\
do{ (ob)->ods_docc = 0; (ob)->ods_docv = NULL; }while(0)
#define _DeeOverloadDocs_Quit(ob)\
do{\
 struct DeeOverloadDoc *_od_iter,*_od_end;\
 _od_end = (_od_iter = (ob)->ods_docv)+(ob)->ods_docc;\
 while (_od_iter != _od_end) { _DeeOverloadDoc_Quit(_od_iter); ++_od_iter; }\
 free((ob)->ods_docv);\
}while(0)
#define _DeeOverloadDocs_Visit(ob)\
do{\
 struct DeeOverloadDoc *_od_iter,*_od_end;\
 _od_end = (_od_iter = (ob)->ods_docv)+(ob)->ods_docc;\
 while (_od_iter != _od_end) { _DeeOverloadDoc_Visit(_od_iter); ++_od_iter; }\
}while(0)

//////////////////////////////////////////////////////////////////////////
// Returns a pointer to a newly allocated overload doc at the end of 'self's list
// The caller is responsible for properly initializing that pointer,
// and if initialization fails, 'DeeParamDocs_UnsafeRemove' must be called
extern struct DeeOverloadDoc *DeeOverloadDocs_UnsafeAppend(DEE_A_INOUT struct DeeOverloadDocs *self);
#define DeeOverloadDocs_UnsafeRemove(ob) (void)--(ob)->ods_docc

//////////////////////////////////////////////////////////////////////////
// Adds a new overload based on given doc strings
//                <-------------------> decl
// >> function add(int a, int b) -> int;
//    <---------------------------...
// >> @param a: The first argument
// >> @param b: The second argument
// >> @throws Error: Something went wrong
// >>      Returns the sum of a and b
// ...------------------------------> text
extern DEE_A_RET_EXCEPT(-1) int DeeOverloadDocs_AddDocEx(
 DEE_A_INOUT struct DeeOverloadDocs *self,
 DEE_A_IN Dee_size_t decl_len, DEE_A_IN_R(decl_len) char const *decl_str,
 DEE_A_IN Dee_size_t  doc_len, DEE_A_IN_R( doc_len) char const  *doc_str);


//////////////////////////////////////////////////////////////////////////
// Adds a new overload based on given doc string
//                <---------------------...
// >> function add(int a, int b) -> int;
// >> @param a: The first argument
// >> @param b: The second argument
// >> @throws Error: Something went wrong
// >>      Returns the sum of a and b
// ...------------------------------> decl
extern DEE_A_RET_EXCEPT(-1) int DeeOverloadDocs_AddDoc(
 DEE_A_INOUT struct DeeOverloadDocs *self,
 DEE_A_IN Dee_size_t decl_len, DEE_A_IN_R(decl_len) char const *decl_str);
extern DEE_A_RET_EXCEPT(-1) int DeeOverloadDocs_AddDocs(
 DEE_A_INOUT struct DeeOverloadDocs *self,
 DEE_A_IN Dee_size_t decl_len, DEE_A_IN_R(decl_len) char const *decl_str);

extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeOverloadDocs_Repr(
 DEE_A_IN struct DeeOverloadDocs const *self, DEE_A_IN Dee_uint32_t function_flags,
 DEE_A_IN_Z_OPT char const *function_name, DEE_A_IN_Z_OPT char const *prefix);
extern DEE_A_RET_EXCEPT(-1) int DeeOverloadDocs_ReprToWriter(
 DEE_A_IN struct DeeOverloadDocs const *self,
 DEE_A_INOUT struct DeeStringWriter *writer, DEE_A_IN Dee_uint32_t function_flags,
 DEE_A_IN_Z_OPT char const *function_name, DEE_A_IN_Z_OPT char const *prefix);





struct DeeGenericDoc {
 DEE_A_REF DeeStringObject *gd_name; /*< [0..1] Object name. */
 DEE_A_REF DeeStringObject *gd_doc;  /*< [0..1] Object doc. */
};
#define _DeeGenericDoc_Quit(ob)\
do{\
 Dee_XDECREF((ob)->gd_name);\
 Dee_XDECREF((ob)->gd_doc);\
}while(0)
#define _DeeGenericDoc_Visit(ob)\
do{\
 Dee_XVISIT((ob)->gd_name);\
 Dee_XVISIT((ob)->gd_doc);\
}while(0)
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeGenericDoc_Repr(
 DEE_A_IN struct DeeGenericDoc const *self);
extern DEE_A_RET_EXCEPT(-1) int DeeGenericDoc_ReprToWriter(
 DEE_A_IN struct DeeGenericDoc const *self, DEE_A_INOUT struct DeeStringWriter *writer);




struct DeeMethodDoc {
 DEE_A_REF DeeStringObject *md_name;  /*< [1..1] Method name. e.g.: 'add'. */
 struct DeeOverloadDocs     md_over;  /*< Documentations of the various overloads. */
 DEE_A_REF DeeStringObject *md_this;  /*< [0..1] Name of the type in a this-call. */
#define DEE_METHODDOC_FLAG_NONE      DEE_UINT32_C(0x00000000)
#define DEE_METHODDOC_FLAG_CLASS     DEE_UINT32_C(0x00000001)
#define DEE_METHODDOC_FLAG_CONSTEXPR DEE_UINT32_C(0x00000002) /*< The function can be executed by the compiler. */
#define DEE_METHODDOC_FLAG_OPERATOR  DEE_UINT32_C(0x00000004) /*< The function is an operator. */
 Dee_uint32_t               md_flags; /*< Function flags. */
};
#define _DeeMethodDoc_Quit(ob)\
do{\
 Dee_DECREF((ob)->md_name);\
 Dee_XDECREF((ob)->md_this);\
 _DeeOverloadDocs_Quit(&(ob)->md_over);\
}while(0)
#define _DeeMethodDoc_Visit(ob)\
do{\
 Dee_VISIT((ob)->md_name);\
 Dee_XVISIT((ob)->md_this);\
 _DeeOverloadDocs_Visit(&(ob)->md_over);\
}while(0)

#define DeeMethodDoc_InitFromNameAndDoc(ob,flags,this_type,name,doc) \
 DeeMethodDoc_InitFromNameAndDocWithLength(ob,flags,this_type,\
  strlen(name),name,(doc) ? strlen(doc) : 0,doc)
extern DEE_A_RET_EXCEPT(-1) int DeeMethodDoc_InitFromNameAndDocWithLength(
 DEE_A_OUT struct DeeMethodDoc *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT_OPT DeeStringObject *this_type,
 Dee_size_t name_len, DEE_A_IN_R(name_len) char const *name_str,
 Dee_size_t doc_len, DEE_A_IN_R(doc_len) char const *doc_str);
extern DEE_A_RET_EXCEPT(-1) int DeeMethodDoc_InitFromMethodDef(
 DEE_A_OUT struct DeeMethodDoc *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeMethodDef const *def);
#define DeeMethodDoc_Repr(ob,prefix) \
 DeeOverloadDocs_Repr(&(ob)->md_over,(ob)->md_flags,DeeString_STR((ob)->md_name),prefix)
#define DeeMethodDoc_ReprToWriter(ob,writer,prefix) \
 DeeOverloadDocs_ReprToWriter(&(ob)->md_over,writer,(ob)->md_flags,DeeString_STR((ob)->md_name),prefix)





struct DeeMemberDoc {
 DEE_A_REF DeeStringObject *mbd_name;   /*< [1..1] Member name. */
 DEE_A_REF DeeStringObject *mbd_doc;    /*< [0..1] Documentation text. */
 struct DeeParamDoc         mbd_return; /*< Return parameter documentation. */
 DEE_A_REF DeeObject       *mbd_value;  /*< [0..1] Member value (for constants). */
 DEE_A_REF DeeStringObject *mbd_this;   /*< [1..1] Name of the type implementing this member. */
#define DEE_MEMBERDOC_FLAG_NONE   0x00000000
#define DEE_MEMBERDOC_FLAG_CLASS  0x00000001
#define DEE_MEMBERDOC_FLAG_READ   0x00000002
#define DEE_MEMBERDOC_FLAG_WRITE  0x00000004
#define DEE_MEMBERDOC_FLAG_ATOMIC 0x00000008
 Dee_uint32_t               mbd_flags; /*< Member flags. */
};
#define _DeeMemberDoc_Quit(ob)\
do{\
 Dee_DECREF((ob)->mbd_name);\
 Dee_XDECREF((ob)->mbd_doc);\
 _DeeParamDoc_Quit(&(ob)->mbd_return);\
 Dee_XDECREF((ob)->mbd_value);\
 Dee_DECREF((ob)->mbd_this);\
}while(0)
#define _DeeMemberDoc_Visit(ob)\
do{\
 Dee_VISIT((ob)->mbd_name);\
 Dee_XVISIT((ob)->mbd_doc);\
 _DeeParamDoc_Visit(&(ob)->mbd_return);\
 Dee_XVISIT((ob)->mbd_value);\
 Dee_VISIT((ob)->mbd_this);\
}while(0)

#define DeeMemberDoc_InitFromNameAndDoc(ob,flags,this_type,return_type,name,doc) \
 DeeMemberDoc_InitFromNameAndDocWithLength(ob,flags,this_type,return_type,\
  strlen(name),name,(doc) ? strlen(doc) : 0,doc)
extern DEE_A_RET_EXCEPT(-1) int DeeMemberDoc_InitFromNameAndDocWithLength(
 DEE_A_OUT struct DeeMemberDoc *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN DeeTypeObject const *return_type,
 Dee_size_t name_len, DEE_A_IN_R(name_len) char const *name_str,
 Dee_size_t doc_len, DEE_A_IN_R(doc_len) char const *doc_str);
extern DEE_A_RET_EXCEPT(-1) int DeeMemberDoc_InitFromMemberDef(
 DEE_A_OUT struct DeeMemberDoc *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeMemberDef const *def);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeMemberDoc_Repr(
 DEE_A_IN struct DeeMemberDoc const *self);
extern DEE_A_RET_EXCEPT(-1) int DeeMemberDoc_ReprToWriter(
 DEE_A_IN struct DeeMemberDoc const *self, DEE_A_INOUT struct DeeStringWriter *writer);







struct DeeGetSetDoc {
 DEE_A_REF DeeStringObject *gd_name;   /*< [1..1] GetSet name. */
 DEE_A_REF DeeStringObject *gd_doc;    /*< [0..1] Documentation text. */
 DEE_A_REF DeeStringObject *gd_this;   /*< [0..1] Name of the type in a this-call. */
#define DEE_GETSETDOC_FLAG_NONE      0x00000000
#define DEE_GETSETDOC_FLAG_CLASS     0x00000001 /*< The GetSet is part of a tp_class_members list. */
#define DEE_GETSETDOC_FLAG_CONSTEXPR 0x00000002 /*< The GetSet can be executed by the compiler. */
#define DEE_GETSETDOC_FLAG_GET       0x00000004 /*< A getter is available. */
#define DEE_GETSETDOC_FLAG_DEL       0x00000008 /*< A delete is available. */
#define DEE_GETSETDOC_FLAG_SET       0x00000010 /*< A setter is available. */
 Dee_uint32_t               gd_flags;  /*< GetSet flags. */
 struct DeeParamDoc         gd_return; /*< The getset return argument. */
 struct DeeThrowDocs        gd_throw;  /*< Exception documentation. */
};
#define _DeeGetSetDoc_Quit(ob)\
do{\
 Dee_DECREF((ob)->gd_name);\
 Dee_XDECREF((ob)->gd_doc);\
 Dee_XDECREF((ob)->gd_this);\
 _DeeParamDoc_Quit(&(ob)->gd_return);\
 _DeeThrowDocs_Quit(&(ob)->gd_throw);\
}while(0)
#define _DeeGetSetDoc_Visit(ob)\
do{\
 Dee_VISIT((ob)->gd_name);\
 Dee_XVISIT((ob)->gd_doc);\
 Dee_XVISIT((ob)->gd_this);\
 _DeeParamDoc_Visit(&(ob)->gd_return);\
 _DeeThrowDocs_Visit(&(ob)->gd_throw);\
}while(0)

#define DeeGetSetDoc_InitFromNameAndDoc(ob,flags,this_type,name,doc) \
 DeeGetSetDoc_InitFromNameAndDocWithLength(ob,flags,this_type,strlen(name),name,(doc) ? strlen(doc) : 0,doc)
extern DEE_A_RET_EXCEPT(-1) int DeeGetSetDoc_InitFromNameAndDocWithLength(
 DEE_A_OUT struct DeeGetSetDoc *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT_OPT DeeStringObject *this_type,
 Dee_size_t name_len, DEE_A_IN_R(name_len) char const *name_str,
 Dee_size_t doc_len, DEE_A_IN_R(doc_len) char const *doc_str);
extern DEE_A_RET_EXCEPT(-1) int DeeGetSetDoc_InitFromGetSetDef(
 DEE_A_OUT struct DeeGetSetDoc *self, DEE_A_IN Dee_uint32_t flags,
 DEE_A_INOUT DeeStringObject *this_type, DEE_A_IN struct DeeGetSetDef const *def);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeGetSetDoc_Repr(
 DEE_A_IN struct DeeGetSetDoc const *self);
extern DEE_A_RET_EXCEPT(-1) int DeeGetSetDoc_ReprToWriter(
 DEE_A_IN struct DeeGetSetDoc const *self, DEE_A_INOUT struct DeeStringWriter *writer);















struct DeeTypeDoc {
 DEE_A_REF DeeStringObject *td_base;   /*< [0..1] Name of the type's base. */
 DEE_A_REF DeeStringObject *td_descr;  /*< [0..1] General type doc. */
 struct DeeMethodDoc        td_ctor;   /*< Documentation for the type's constructor (also contains the type's name). */
 struct DeeDocs             td_attrib; /*< Additional documentation on members, getsets and methods. */
};
#define _DeeTypeDoc_Quit(ob)\
do{\
 Dee_XDECREF((ob)->td_base);\
 Dee_XDECREF((ob)->td_descr);\
 _DeeMethodDoc_Quit(&(ob)->td_ctor);\
 _DeeDocs_Quit(&(ob)->td_attrib);\
}while(0)
#define _DeeTypeDoc_Visit(ob)\
do{\
 Dee_XVISIT((ob)->td_base);\
 Dee_XVISIT((ob)->td_descr);\
 _DeeMethodDoc_Visit(&(ob)->td_ctor);\
 _DeeDocs_Visit(&(ob)->td_attrib);\
}while(0)

//////////////////////////////////////////////////////////////////////////
// Initializer a type doc from a given type
extern DEE_A_RET_EXCEPT(-1) int DeeTypeDoc_InitFromType(
 DEE_A_OUT struct DeeTypeDoc *self, DEE_A_IN DeeTypeObject const *ty);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
 DeeTypeDoc_Repr(DEE_A_IN struct DeeTypeDoc const *self);
extern DEE_A_RET_EXCEPT(-1) int DeeTypeDoc_ReprToWriter(
 DEE_A_IN struct DeeTypeDoc const *self, DEE_A_INOUT struct DeeStringWriter *writer);






struct DeeDoc {
#define DEE_DOCKIND_NONE    0x00000000
#define DEE_DOCKIND_GENERIC 0x00000001 /*< Generic documentation. */
#define DEE_DOCKIND_METHOD  0x00000002 /*< Method documentation. */
#define DEE_DOCKIND_MEMBER  0x00000003 /*< Member documentation. */
#define DEE_DOCKIND_GETSET  0x00000004 /*< Getset documentation. */
#define DEE_DOCKIND_TYPE    0x00000005 /*< Type documentation. */
 Dee_uint32_t          doc_kind;   /*< The kind of doc. */
 union{
  struct DeeGenericDoc doc_generic; /*< DEE_DOCKIND_GENERIC. */
  struct DeeMethodDoc  doc_method;  /*< DEE_DOCKIND_METHOD. */
  struct DeeMemberDoc  doc_member;  /*< DEE_DOCKIND_MEMBER. */
  struct DeeGetSetDoc  doc_getset;  /*< DEE_DOCKIND_GETSET. */
  struct DeeTypeDoc    doc_type;    /*< DEE_DOCKIND_TYPE. */
 };
};
extern void DeeDoc_Quit(DEE_A_IN struct DeeDoc *self);
#define _DeeDoc_Visit(ob) _DeeDoc_DoVisit(ob,visit,closure)
extern void _DeeDoc_DoVisit(DEE_A_IN struct DeeDoc *self, DEE_A_IN DeeVisitProc visit, void *closure);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
 DeeDoc_Repr(DEE_A_IN struct DeeDoc const *self);
extern DEE_A_RET_EXCEPT(-1) int DeeDoc_ReprToWriter(
 DEE_A_IN struct DeeDoc const *self, DEE_A_INOUT struct DeeStringWriter *writer);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeDoc_GetName(
 DEE_A_IN struct DeeDoc const *self);


//////////////////////////////////////////////////////////////////////////
// Initialize a documentation from any given object
#define DeeDoc_InitFromObject(self,ob) DeeDoc_TInitFromObject(self,Dee_TYPE(ob),ob)
#define DeeDoc_InitFromObjectAttr(self,ob,attr) DeeDoc_TInitFromObjectAttr(self,Dee_TYPE(ob),ob,attr)
#define DeeDoc_InitFromObjectAttrString(self,ob,attr) DeeDoc_TInitFromObjectAttrString(self,Dee_TYPE(ob),ob,attr)
#define DeeDoc_TInitFromObjectAttr(self,tp_ob,ob,attr) DeeDoc_TInitFromObjectAttrString(self,tp_ob,ob,DeeString_STR(attr))
extern DEE_A_RET_EXCEPT(-1) int DeeDoc_TInitFromObject(
 DEE_A_OUT struct DeeDoc *self, DEE_A_IN DeeTypeObject const *tp_ob,
 DEE_A_INOUT DeeObject *ob);
extern DEE_A_RET_EXCEPT(-1) int DeeDoc_TInitFromObjectAttrString(
 DEE_A_OUT struct DeeDoc *self, DEE_A_IN DeeTypeObject const *tp_ob,
 DEE_A_INOUT DeeObject *ob, DEE_A_IN_Z char const *attr);









struct DeeDocRef {
 DEE_A_REF DeeDocObject *dr_keepalive; /*< [1..1] Keep-alive reference. */
 struct DeeDoc          *dr_doc;       /*< [1..1] Referenced doc object. */
};

struct DeeDocObject {
 DEE_OBJECT_HEAD
#define DEE_DOCOBJECT_KIND_OBJ 0 /*< Real doc object. */
#define DEE_DOCOBJECT_KIND_REF 1 /*< Reference to another doc object. */
 Dee_uint32_t  d_kind;
 union{
  struct DeeDocRef d_ref; /*< DEE_DOCOBJECT_KIND_REF. */
  struct DeeDoc    d_doc; /*< DEE_DOCOBJECT_KIND_OBJ. */
 };
};
#define DeeDoc_DOC(ob) \
(((DeeDocObject *)Dee_REQUIRES_POINTER(ob))->d_kind == DEE_DOCOBJECT_KIND_OBJ\
 ? &((DeeDocObject *)Dee_REQUIRES_POINTER(ob))->d_doc\
 :  ((DeeDocObject *)Dee_REQUIRES_POINTER(ob))->d_ref.dr_doc)

struct DeeGenericDocRefObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDocObject *dr_keepalive; /*< [1..1] Keep-alive reference. */
 struct DeeGenericDoc   *dr_doc;       /*< [1..1] Referenced doc object. */
};
struct DeeMethodDocRefObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDocObject *dr_keepalive; /*< [1..1] Keep-alive reference. */
 struct DeeMethodDoc    *dr_doc;       /*< [1..1] Referenced doc object. */
};
struct DeeOverloadDocsRefObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDocObject *dr_keepalive; /*< [1..1] Keep-alive reference. */
 struct DeeOverloadDocs *dr_doc;       /*< [1..1] Referenced doc object. */
};
struct DeeOverloadDocsRefIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDocObject          *dri_keepalive; /*< [1..1] Keep-alive reference. */
 /*atomic*/struct DeeOverloadDoc *dri_iter;      /*< [1..1] Current iterator position. */
 struct DeeOverloadDoc           *dri_end;       /*< [1..1][const] Iterator end position. */
};
struct DeeOverloadDocRefObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDocObject *dr_keepalive; /*< [1..1] Keep-alive reference. */
 struct DeeOverloadDoc  *dr_doc;       /*< [1..1] Referenced doc object. */
};
struct DeeParamDocsRefObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDocObject *dr_keepalive; /*< [1..1] Keep-alive reference. */
 struct DeeParamDocs    *dr_doc;       /*< [1..1] Referenced doc object. */
};
struct DeeParamDocsRefIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDocObject       *dri_keepalive; /*< [1..1] Keep-alive reference. */
 /*atomic*/struct DeeParamDoc *dri_iter;      /*< [1..1] Current iterator position. */
 struct DeeParamDoc           *dri_end;       /*< [1..1][const] Iterator end position. */
};
struct DeeParamDocRefObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDocObject *dr_keepalive; /*< [1..1] Keep-alive reference. */
 struct DeeParamDoc     *dr_doc;       /*< [1..1] Referenced doc object. */
};

struct DeeThrowDocsRefObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDocObject *dr_keepalive; /*< [1..1] Keep-alive reference. */
 struct DeeThrowDocs    *dr_doc;       /*< [1..1] Referenced doc object. */
};
struct DeeThrowDocsRefIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDocObject       *dri_keepalive; /*< [1..1] Keep-alive reference. */
 /*atomic*/struct DeeThrowDoc *dri_iter;      /*< [1..1] Current iterator position. */
 struct DeeThrowDoc           *dri_end;       /*< [1..1][const] Iterator end position. */
};
struct DeeThrowDocRefObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDocObject *dr_keepalive; /*< [1..1] Keep-alive reference. */
 struct DeeThrowDoc     *dr_doc;       /*< [1..1] Referenced doc object. */
};

struct DeeMemberDocRefObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDocObject *dr_keepalive; /*< [1..1] Keep-alive reference. */
 struct DeeMemberDoc    *dr_doc;       /*< [1..1] Referenced doc object. */
};

struct DeeGetSetDocRefObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDocObject *dr_keepalive; /*< [1..1] Keep-alive reference. */
 struct DeeGetSetDoc    *dr_doc;       /*< [1..1] Referenced doc object. */
};

struct DeeTypeDocRefObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDocObject *dr_keepalive; /*< [1..1] Keep-alive reference. */
 struct DeeTypeDoc      *dr_doc;       /*< [1..1] Referenced doc object. */
};

struct DeeDocsRefObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDocObject *dr_keepalive; /*< [1..1] Keep-alive reference. */
 struct DeeDocs         *dr_doc;       /*< [1..1] Referenced doc object. */
};
struct DeeDocsRefIteratorObject {
 DEE_OBJECT_HEAD
 DEE_A_REF DeeDocObject  *dri_keepalive; /*< [1..1] Keep-alive reference. */
 /*atomic*/struct DeeDoc *dri_iter;      /*< [1..1] Current iterator position. */
 struct DeeDoc           *dri_end;       /*< [1..1][const] Iterator end position. */
};


extern DeeTypeObject DeeDoc_Type;                     // doc
extern DeeTypeObject DeeGenericDocRef_Type;           // doc.generic
extern DeeTypeObject DeeMethodDocRef_Type;            // doc.method
extern DeeTypeObject DeeOverloadDocsRef_Type;         // doc.method.overloads
extern DeeTypeObject DeeOverloadDocsRefIterator_Type; // doc.method.overloads.iterator
extern DeeTypeObject DeeOverloadDocRef_Type;          // doc.method.overload
extern DeeTypeObject DeeParamDocsRef_Type;            // doc.method.overload.params
extern DeeTypeObject DeeParamDocsRefIterator_Type;    // doc.method.overload.params.iterator
extern DeeTypeObject DeeParamDocRef_Type;             // doc.method.overload.param
extern DeeTypeObject DeeThrowDocsRef_Type;            // doc.method.overload.throws
extern DeeTypeObject DeeThrowDocsRefIterator_Type;    // doc.method.overload.throws.iterator
extern DeeTypeObject DeeThrowDocRef_Type;             // doc.method.overload.throw
extern DeeTypeObject DeeMemberDocRef_Type;            // doc.member
extern DeeTypeObject DeeGetSetDocRef_Type;            // doc.getset
extern DeeTypeObject DeeTypeDocRef_Type;              // doc.type
extern DeeTypeObject DeeDocsRef_Type;                 // docs
extern DeeTypeObject DeeDocsRefIterator_Type;         // docs.iterator



//////////////////////////////////////////////////////////////////////////
// API Exports
#define DeeDoc_StringFromObject(ob) DeeDoc_TStringFromObject(Dee_TYPE(ob),ob)
#define DeeDoc_StringFromObjectAttr(ob,attr) DeeDoc_TStringFromObjectAttr(Dee_TYPE(ob),ob,attr)
#define DeeDoc_StringFromObjectAttrString(ob,attr) DeeDoc_TStringFromObjectAttrString(Dee_TYPE(ob),ob,attr)
#define DeeDoc_TStringFromObjectAttr(tp_ob,ob,attr) DeeDoc_TStringFromObjectAttrString(tp_ob,ob,DeeString_STR(attr))
extern DEE_ATTRIBUTE_DLLEXPORT DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
 DEE_CALL DeeDoc_TStringFromObject(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self);
extern DEE_ATTRIBUTE_DLLEXPORT DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *
 DEE_CALL DeeDoc_TStringFromObjectAttrString(DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self, DEE_A_IN_Z char const *attr);


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DOCGEN_H */
