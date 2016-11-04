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
#ifndef GUARD_DEEMON_SCOPE_H
#define GUARD_DEEMON_SCOPE_H 1
#ifdef __INTELLISENSE__
#define DEE_LIMITED_API 1
#endif

#ifndef DEE_LIMITED_API
#error "Limited-only header"
#endif

#include <deemon/__conf.inl>
#include <deemon/object.h>
#include <deemon/type.h>
#include <deemon/compiler/lexer.h>
#include <deemon/compiler/code.h>
#include <deemon/compiler/codewriter.h>
#include <deemon/optional/string_forward.h>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif


DEE_OBJECT_DEF(DeeScopeObject);
DEE_OBJECT_DEF(DeeModuleObject);
DEE_OBJECT_DEF(DeeLocalVarObject);



struct DeeXAstObject;
struct DeeParserCaseLabelCase {
 DEE_A_REF struct DeeXAstObject *clc_case; /*< [1..1] The case constant of this label. */
};
struct DeeParserCaseLabelRange {
 DEE_A_REF struct DeeXAstObject *clr_begin; /*< [1..1] The lower bound of this label. */
 DEE_A_REF struct DeeXAstObject *clr_end;   /*< [1..1] The upper bound of this label. */
};
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeParserLabel {
 struct DeeParserLabel          *pl_next;       /*< [0..1] Next parser label. */
 TPPTokenID                      pl_name;       /*< Name of the label (0 for case labels). */
 DEE_A_REF DeeTokenObject       *pl_dest_token; /*< [0..1] Token of the destination of the label. (NULL if forward) */
 struct DeeCodeLabel             pl_code_label; /*< Only used during compilation: the code label. */
 unsigned int                    pl_refs;       /*< Amount of times that a goto to this label exists. */
#define DEE_PARSERLABEL_KIND_LABEL     0        /*< Regular label. */
#define DEE_PARSERLABEL_KIND_C_DEFAULT 1        /*< Default case label. */
#define DEE_PARSERLABEL_KIND_C_CASE    2        /*< Normal case label. */
#define DEE_PARSERLABEL_KIND_C_RANGE   3        /*< Range case label. */
 Dee_uint32_t                    pl_kind;       /*< The kind of case label. */
 union{
  struct DeeParserCaseLabelCase  pl_c_case;  /*< DEE_PARSERLABEL_KIND_C_CASE. */
  struct DeeParserCaseLabelRange pl_c_range; /*< DEE_PARSERLABEL_KIND_C_RANGE. */
 };
};
#define DeeParserLabel_ADD_GOTO(ob) ++(ob)->pl_refs
#define DeeParserLabel_REM_GOTO(ob) (DEE_ASSERTF((ob)->pl_refs,"Underflow in label goto counter"),--(ob)->pl_refs)

DEE_COMPILER_MSVC_WARNING_POP
#define DeeParserLabel_IS_FORWARD(ob) ((ob)->pl_dest_token==NULL)
#define DeeParserLabel_DECLARE(ob,dest_token)\
do{\
 DEE_ASSERT(DeeParserLabel_IS_FORWARD(ob));\
 Dee_INCREF((ob)->pl_dest_token = (dest_token));\
}while(0)

#define _DeeParserLabel_Quit(ob)\
do{\
 Dee_XDECREF((ob)->pl_dest_token);\
 _DeeCodeLabel_Quit(&(ob)->pl_code_label);\
 switch ((ob)->pl_kind) {\
  case DEE_PARSERLABEL_KIND_C_CASE:\
   Dee_DECREF((ob)->pl_c_case.clc_case);\
   break;\
  case DEE_PARSERLABEL_KIND_C_RANGE:\
   Dee_DECREF((ob)->pl_c_range.clr_begin);\
   Dee_DECREF((ob)->pl_c_range.clr_end);\
   break;\
  default: break;\
 }\
}while(0)
#define _DeeParserLabel_Visit(ob)\
do{\
 Dee_XVISIT((ob)->pl_dest_token);\
 switch ((ob)->pl_kind) {\
  case DEE_PARSERLABEL_KIND_C_CASE:\
   Dee_VISIT((ob)->pl_c_case.clc_case);\
   break;\
  case DEE_PARSERLABEL_KIND_C_RANGE:\
   Dee_VISIT((ob)->pl_c_range.clr_begin);\
   Dee_VISIT((ob)->pl_c_range.clr_end);\
   break;\
  default: break;\
 }\
}while(0)


//////////////////////////////////////////////////////////////////////////
// Label list (Used in scopes to track labels and switch statements to track cases)
struct DeeParserLabelList {
 struct DeeParserLabel *ll_front; /*< [0..1][owned] First known label (also knows all 'pl_next' recursively). */
};
#define DeeParserLabelList_INIT() {NULL}
#define DeeParserLabelList_EMPTY(ob) ((ob)->ll_front==NULL)
#define _DeeParserLabelList_Init(ob) \
do{ (ob)->ll_front = NULL; }while(0)
#define _DeeParserLabelList_InitMove(ob,right) \
do{ (ob)->ll_front = (right)->ll_front; (right)->ll_front = NULL; }while(0)
#define _DeeParserLabelList_Quit(ob) \
do{\
 struct DeeParserLabel *ll_next,*ll_elem = (ob)->ll_front;\
 while (ll_elem) {\
  ll_next = ll_elem->pl_next;\
  _DeeParserLabel_Quit(ll_elem);\
  free_nn(ll_elem);\
  ll_elem = ll_next;\
 }\
}while(0)
#define _DeeParserLabelList_Visit(ob) \
do{\
 struct DeeParserLabel *ll_elem = (ob)->ll_front;\
 while (ll_elem) {\
  _DeeParserLabel_Visit(ll_elem);\
  ll_elem = ll_elem->pl_next;\
 }\
}while(0)

//////////////////////////////////////////////////////////////////////////
// Returns the regular label associated with a given name (or NULL if it doesn't exist)
extern DEE_A_RET_MAYBE_NULL struct DeeParserLabel *DeeParserLabelList_GetLabel(
 DEE_A_IN struct DeeParserLabelList const *self, DEE_A_IN TPPTokenID name) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Returns the first label that covers the given case range (NULL if the range isn't covered)
extern DEE_A_RET_MAYBE_NULL struct DeeParserLabel *DeeParserLabelList_GetCase(
 DEE_A_IN struct DeeParserLabelList const *self, DEE_A_IN struct DeeXAstObject *case_value) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_MAYBE_NULL struct DeeParserLabel *DeeParserLabelList_GetCaseRange(
 DEE_A_IN struct DeeParserLabelList const *self, DEE_A_IN struct DeeXAstObject *case_begin,
 DEE_A_IN struct DeeXAstObject *case_end) DEE_ATTRIBUTE_NONNULL((1,2,3));

//////////////////////////////////////////////////////////////////////////
// Returns the default case label
extern DEE_A_RET_MAYBE_NULL struct DeeParserLabel *DeeParserLabelList_GetDefaultCase(
 DEE_A_IN struct DeeParserLabelList const *self) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Adds various new cases to the list
// NOTE: These functions assert that the given label/case[-range] doesn't exist/isn't covered
// All functions return the new label
extern DEE_A_RET_EXCEPT(NULL) struct DeeParserLabel *DeeParserLabelList_AddLabel(
 DEE_A_INOUT struct DeeParserLabelList *self,DEE_A_INOUT DeeTokenObject *dest_token,
 DEE_A_IN TPPTokenID name) DEE_ATTRIBUTE_NONNULL((1,2));
extern DEE_A_RET_EXCEPT(NULL) struct DeeParserLabel *DeeParserLabelList_AddCase(
 DEE_A_INOUT struct DeeParserLabelList *self, DEE_A_INOUT DeeTokenObject *dest_token, 
 DEE_A_INOUT struct DeeXAstObject *case_value) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_EXCEPT(NULL) struct DeeParserLabel *DeeParserLabelList_AddCaseRange(
 DEE_A_INOUT struct DeeParserLabelList *self, DEE_A_INOUT DeeTokenObject *dest_token,
 DEE_A_INOUT struct DeeXAstObject *case_begin,
 DEE_A_INOUT struct DeeXAstObject *case_end) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
extern DEE_A_RET_EXCEPT(NULL) struct DeeParserLabel *DeeParserLabelList_AddDefaultCase(
 DEE_A_INOUT struct DeeParserLabelList *self, DEE_A_INOUT DeeTokenObject *dest_token) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Gets or adds a new forward label for 'name' (used by the goto statement)
extern DEE_A_RET_EXCEPT(NULL) struct DeeParserLabel *DeeParserLabelList_GetOrAddForwardLabel(
 DEE_A_INOUT struct DeeParserLabelList *self, DEE_A_IN TPPTokenID name) DEE_ATTRIBUTE_NONNULL((1));



//////////////////////////////////////////////////////////////////////////
// Label reference list (used in SAst objects to track label endpoints)
struct DeeParserLabelRefList {
 Dee_size_t              lrl_c; /*< Amount of known label references. */
 struct DeeParserLabel **lrl_v; /*< [1..1][0..lrl_c][owned] List of label references. */
};
extern struct DeeParserLabelRefList _DeeParserLabelRefList_Empty;
#define DeeParserLabelRefList_Empty (&_DeeParserLabelRefList_Empty)
#define DeeParserLabelRefList_INIT()    {0,NULL}
#define _DeeParserLabelRefList_Init(ob) do{ (ob)->lrl_c = 0; (ob)->lrl_v = NULL; }while(0)
#define _DeeParserLabelRefList_InitMove(ob,right)\
do{\
 (ob)->lrl_c = (right)->lrl_c,(right)->lrl_c = 0;\
 (ob)->lrl_v = (right)->lrl_v,(right)->lrl_v = NULL;\
}while(0)
#define _DeeParserLabelRefList_Quit(ob) do{ free((ob)->lrl_v); }while(0)
#define DeeParserLabelRefList_ELEM(ob)  (ob)->lrl_v
#define DeeParserLabelRefList_SIZE(ob)  (ob)->lrl_c
#define DeeParserLabelRefList_EMPTY(ob) ((ob)->lrl_c==0)

//////////////////////////////////////////////////////////////////////////
// Appends a given label to 'self'
extern DEE_A_RET_EXCEPT(-1) int DeeParserLabelRefList_Append(
 DEE_A_INOUT struct DeeParserLabelRefList *self, DEE_A_IN struct DeeParserLabel *lbl);

//////////////////////////////////////////////////////////////////////////
// Moves all labels references from 'right' and appends them to 'self'
// If 'self' is empty, this is identical to calling '_DeeParserLabelRefList_InitMove(self,right)'
extern DEE_A_RET_EXCEPT(-1) int DeeParserLabelRefList_AppendMoveAll(
 DEE_A_INOUT struct DeeParserLabelRefList *self, DEE_A_INOUT struct DeeParserLabelRefList *right);
























//////////////////////////////////////////////////////////////////////////
// Local var object

struct DeeLocalVarObject {
#define DeeLocalVar_ApplyAttributes(ob,attr)\
do{\
 if (((attr)->a_flags&DEE_ATTRIBUTE_DECL_FLAG_USED)!=0) (ob)->lv_flags |= DEE_LOCALVAR_FLAG_USED;\
 if (((attr)->a_flags&DEE_ATTRIBUTE_DECL_FLAG_UNUSED)!=0) (ob)->lv_flags |= DEE_LOCALVAR_FLAG_UNUSED;\
 if (((attr)->a_flags&DEE_ATTRIBUTE_DECL_FLAG_INSTBOUNDC)!=0) (ob)->lv_flags |= DEE_LOCALVAR_FLAG_INSTBOUNDC;\
 if (((attr)->a_flags&DEE_ATTRIBUTE_DECL_FLAG_INSTBOUNDM)!=0) (ob)->lv_flags |= DEE_LOCALVAR_FLAG_INSTBOUNDM;\
}while(0)

 DEE_OBJECT_HEAD
 TPPTokenID                             lv_name;        /*< Name of this local variable. */
 DEE_A_WEAK_REF struct DeeScopeObject  *lv_scope;       /*< [1..1] Scope of this local variable. */
#define DEE_LOCAL_VAR_FLAG_NONE         0x00000000
  // v Set once the variable as been assigned a value
  //   - Usually instantly set in code like: 'a = 21'
  //     but not set if forward variable to specify its scope:
  //     >> a;
  //     >> {
  //     >>   a = 21; // This sets the initialized flag
  //     >> }
  //   NOTE: This flag is set during the compilation phase; not the AST generation, or optimization phase
#define DEE_LOCALVAR_FLAG_INITIALIZED   0x00000100
#define DEE_LOCALVAR_FLAG_INSTBOUNDC    0x00000200 /*< This variable is instance bound (only the first declaration is allowed to store to it; all other stores are compiled as copy-assign). */
#define DEE_LOCALVAR_FLAG_INSTBOUNDM    0x00000400 /*< This variable is instance bound (only the first declaration is allowed to store to it; all other stores are compiled as move-assign). */
#define DEE_LOCALVAR_FLAG_UNUSED        0x00000800 /*< Don't store a value in this variable if it is never used (even if its initializer is unknown/has a life-time effect). */
#define DEE_LOCALVAR_FLAG_USED          0x00001000 /*< Always store a value in this variable (even if it is never used). */
#define DEE_LOCALVAR_FLAG_VOLATILE      0x00002000 /*< This variable is volatile, meaning we're not allowed to try and optimize it. */
#define DEE_LOCALVAR_FLAG_MASK          0xFFFFFF00
#define DEE_LOCALVAR_KIND_LOCAL         0x00000000 /*< local variable. */
#define DEE_LOCALVAR_KIND_PARAM         0x00000001 /*< parameter variable. */
#define DEE_LOCALVAR_KIND_RETVAL        0x00000002 /*< return value variable. */
#define DEE_LOCALVAR_KIND_STATIC        0x00000003 /*< static variable (is exists as a constant). */
#define DEE_LOCALVAR_KIND_THIS          0x00000004 /*< this-variable in [[this-call function. */
#define DEE_LOCALVAR_KIND_STACK         0x00000005 /*< Variable exists on the stack. */
#define DEE_LOCALVAR_KIND_MASK          0x000000FF
#define DEE_LOCALVARFLAGS_KIND(kind)  ((kind)&DEE_LOCALVAR_KIND_MASK)
#define DEE_LOCALVARFLAGS_FLAGS(kind) ((kind)&DEE_LOCALVAR_FLAG_MASK)
 Dee_uint32_t                           lv_flags;       /*< Flags. */
 Dee_size_t                             lv_loc_id;      /*< Arg/local id in the associated code-frame / strong-scope. */
 unsigned int                           lv_init;        /*< Amount of times this variable has been initialized (used in v102+). */
 unsigned int                           lv_uses;        /*< Amount of times a value is loaded from this variable. */
 unsigned int                           lv_module_refs; /*< Amount of times this variable shows up in different modules.
                                                            NOTE: This controls if the variable is actually deleted at scope end / in a del statement */
};

#define DeeLocalVar_ADD_INIT(ob) (++((DeeLocalVarObject *)Dee_REQUIRES_POINTER(ob))->lv_init)
#define DeeLocalVar_REM_INIT(ob) (\
 DEE_ASSERT(((DeeLocalVarObject *)Dee_REQUIRES_POINTER(ob))->lv_init),\
 --((DeeLocalVarObject *)Dee_REQUIRES_POINTER(ob))->lv_init)
#define DeeLocalVar_ADD_USE(ob) (++((DeeLocalVarObject *)Dee_REQUIRES_POINTER(ob))->lv_uses)
#define DeeLocalVar_REM_USE(ob) (\
 DEE_ASSERT(((DeeLocalVarObject *)Dee_REQUIRES_POINTER(ob))->lv_uses),\
 --((DeeLocalVarObject *)Dee_REQUIRES_POINTER(ob))->lv_uses)

#define DeeLocalVar_KIND(ob)             DEE_LOCALVARFLAGS_KIND(((DeeLocalVarObject *)Dee_REQUIRES_POINTER(ob))->lv_flags)
#define DeeLocalVar_FLAGS(ob)            DEE_LOCALVARFLAGS_FLAGS(((DeeLocalVarObject *)Dee_REQUIRES_POINTER(ob))->lv_flags)
#define DeeLocalVar_IS_INIT(ob)          (((DeeLocalVarObject *)Dee_REQUIRES_POINTER(ob))->lv_init!=0)
#define DeeLocalVar_IS_USED(ob)          (((DeeLocalVarObject *)Dee_REQUIRES_POINTER(ob))->lv_uses!=0)
#define DeeLocalVar_IS_COMPILERINIT(ob) ((((DeeLocalVarObject *)Dee_REQUIRES_POINTER(ob))->lv_flags&DEE_LOCALVAR_FLAG_INITIALIZED)!=0)
#define DeeLocalVar_GET_USES(ob)         ((unsigned int)((DeeLocalVarObject *)Dee_REQUIRES_POINTER(ob))->lv_uses)
#define DeeLocalVar_GET_INIT(ob)         ((unsigned int)((DeeLocalVarObject *)Dee_REQUIRES_POINTER(ob))->lv_init)

#define DEE_TOKENID_IS_VARNAME(id) ((id)==TPP_TOK_DOTS||TPPTokenID_IS_KEYWORD(id))

extern DeeTypeObject DeeLocalVar_Type;
#define DeeLocalVar_Check(ob)      DeeObject_InstanceOf(ob,&DeeLocalVar_Type)
#define DeeLocalVar_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeLocalVar_Type)

extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeLocalVarObject) *DeeLocalVar_New(
 DEE_A_IN TPPTokenID name, DEE_A_IN_OBJECT(DeeScopeObject) *scope);
#define DeeLocalVar_SCOPE(ob) DeeObject_LockWeakref((DeeObject *)((DeeLocalVarObject *)Dee_REQUIRES_POINTER(ob))->ob_scope)
#define DeeLocalVar_NAME(ob)  ((DeeLocalVarObject *)Dee_REQUIRES_POINTER(ob))->lv_name

struct DeeXAstObject;
//////////////////////////////////////////////////////////////////////////
// Returns false (0) if initialization of 'self' should be skipped
//  - This function takes 'lv_uses', 'lv_flags' and 'DeeXAst_PredictType(initializer)' into
//    account in order to figure out whether or not the variable can be optimized away.
// in: "xast_compile.c"
extern DEE_A_RET_WUNUSED int DeeLocalVar_ShouldInitialize(
 DEE_A_IN DeeLocalVarObject const *self,
 DEE_A_IN struct DeeXAstObject *initializer);
extern DEE_A_RET_WUNUSED int DeeLocalVar_ShouldInitializeWithUnknownInitializer(
 DEE_A_IN DeeLocalVarObject const *self);

//////////////////////////////////////////////////////////////////////////
// Returns the name of a given local variable kind
// NOTE: Returns "???" if the kind is unknown
extern DEE_A_RET_Z char const *DeeLocalVarKind_Name(DEE_A_IN Dee_uint32_t kind);







//////////////////////////////////////////////////////////////////////////
// Scope object
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct _DeeScopeEntry {
 TPPTokenID                           e_name;
 DEE_A_REF struct DeeStringObject    *e_depr;   /*< [0..1] Set if this entry is deprecated (string is the reason). */
 union{
  DEE_A_REF DeeObject                *e_const;  /*< [1..1] Local const. */
  DEE_A_REF struct DeeLocalVarObject *e_local;  /*< [1..1] Local var. */
  DEE_A_REF struct DeeModuleObject   *e_module; /*< [1..1] Local module. */
 };
};
#define _DeeScopeEntry_Quit(ob)\
do{\
 Dee_XDECREF((ob)->e_depr);\
 Dee_DECREF((ob)->e_const);\
}while(0)

DEE_COMPILER_MSVC_WARNING_POP
struct DeeScopeObject {
 DEE_OBJECT_HEAD
 struct DeeScopeObject              *sc_prev;      /*< [0..1] Parenting scope (NULL if global scope). */
 struct DeeScopeObject              *sc_weak_prev; /*< [0..1] Weak parenting scope (used for '{}' recursion).
  NOTE: If this is != NULL, 'sc_prev' must be NULL and the other way around as well. */
 Dee_size_t                          sc_namesc;    /*< Used size of the names list. */
 Dee_size_t                          sc_namesa;    /*< Allocated size of the names list. */
 struct _DeeScopeEntry              *sc_namesv;    /*< [0..sc_namesc] List of local variables / consts. */
#define DEE_SCOPE_FLAG_NONE          0x00000000
#define DEE_SCOPE_FLAG_FOUND_RETURN  0x00000001
#define DEE_SCOPE_FLAG_FOUND_YIELD   0x00000002
#define DEE_SCOPE_FLAG_FOUND_RETVAR  0x00000004
#define DEE_SCOPE_FLAG_FOUND_THIS    0x00000008 /*< Set in __attribute__((__thiscall__)) functions. */
#define DEE_SCOPE_FLAG_CODE_COPYABLE 0x00000010 /*< Set in __attribute__((__copyable__)) functions. */
#define DEE_SCOPE_FLAG_MODULE_SCOPE  0x00000020 /*< Module-style scope (don't cleanup local variables in here). */
#define DEE_SCOPE_FLAG_FOUND_REFS    0x00000040 /*< Set if reference variables to other scopes were found (NOTE: even if this is set, the ast using the reference may have been optimized away). */
#if 0 // The following does work, but isn't used (define to enable)
#define DEE_SCOPE_FLAG_USING_LOCALS  0x00000080 /*< Code relies on local variable names being available at runtime. */
#endif
 Dee_uint32_t                        sc_flags;   /*< Scope flags. */
 // The following is only available for strong scopes
 DEE_A_REF struct DeeXAstObject     *sc_supertp; /*< [0..1] Super class type. */
 struct DeeParserLabelList           sc_labels;  /*< List of labels inside this scope. */
 struct DeeParserLabelList          *sc_cases;   /*< [0..1] List of known case label (only non-NULL while compiling inside of a switch statement). */
};

#define DeeScope_PREV(ob)      ((DeeObject *)((DeeScopeObject *)Dee_REQUIRES_POINTER(ob))->sc_prev)
#define DeeScope_WEAK_PREV(ob) ((DeeObject *)((DeeScopeObject *)Dee_REQUIRES_POINTER(ob))->sc_weak_prev)
#define DeeScope_IS_STRONG(ob) (DeeScope_WEAK_PREV(ob)==NULL)

extern DeeTypeObject DeeScope_Type;
#define DeeScope_Check(ob)      DeeObject_InstanceOf(ob,&DeeScope_Type)
#define DeeScope_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeScope_Type)

#define DeeScope_NewRoot() DeeScope_New(NULL,0)

//////////////////////////////////////////////////////////////////////////
// NOTE: The caller is responsible for keeping 'prev' alive longer than the returned scope
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeScopeObject) *DeeScope_New(
 DEE_A_INOUT_OBJECT_OPT(DeeScopeObject) *prev, DEE_A_IN int weak);

#ifdef DEE_LIMITED_API
extern DEE_A_RET_EXCEPT(-1) int _DeeScope_AddName(DEE_A_INOUT DeeScopeObject *self, DEE_A_IN TPPTokenID name, DEE_A_INOUT DeeObject *v);
extern DEE_A_RET_EXCEPT(-1) int _DeeScope_AddDeprecatedName(DEE_A_INOUT DeeScopeObject *self, DEE_A_IN TPPTokenID name, DEE_A_INOUT DeeObject *v, DEE_A_IN_OPT DeeObject *depr);
extern void _DeeScope_DelName(DEE_A_INOUT DeeScopeObject *self, DEE_A_IN TPPTokenID name);
extern /*noexcept*/int _DeeScope_HasName(DEE_A_IN DeeScopeObject const *self, DEE_A_IN TPPTokenID name);
extern DEE_A_RET_NOEXCEPT(NULL) DeeObject *_DeeScope_GetName(DEE_A_IN DeeScopeObject const *self, DEE_A_IN TPPTokenID name);
extern DEE_A_RET_NOEXCEPT(NULL) DeeObject *_DeeScope_GetNameEx(DEE_A_IN DeeScopeObject const *self, DEE_A_IN TPPTokenID name, DEE_A_OUT_OPT DeeObject **is_deprecated);
#endif /* DEE_LIMITED_API */


//////////////////////////////////////////////////////////////////////////
// Returns the global scope of "self"
extern DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeObject *DeeScope_Global(DEE_A_IN_OBJECT(DeeScopeObject) *self);
extern DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeObject *DeeScope_WeakRoot(DEE_A_IN_OBJECT(DeeScopeObject) *self);
extern DEE_A_RET_WUNUSED DEE_A_RET_NEVER_NULL DeeObject *DeeScope_ModuleRoot(DEE_A_IN_OBJECT(DeeScopeObject) *self);

#ifdef DEE_LIMITED_API
#define DeeScope_GLOBAL(ob)      ((DeeScopeObject *)DeeScope_Global((DeeObject *)Dee_REQUIRES_POINTER(ob)))
#define DeeScope_WEAK_ROOT(ob)   ((DeeScopeObject *)DeeScope_WeakRoot((DeeObject *)Dee_REQUIRES_POINTER(ob)))
#define DeeScope_MODULE_ROOT(ob) ((DeeScopeObject *)DeeScope_ModuleRoot((DeeObject *)Dee_REQUIRES_POINTER(ob)))
#endif

//////////////////////////////////////////////////////////////////////////
// Returns true if 'parent' is a parent of 'child'.
//  - 'DeeScope_IsWeakParent' only returns true if the link is weak
// NOTE: Returns false if 'parent == child'
extern DEE_A_RET_WUNUSED int DeeScope_IsParent(DEE_A_IN_OBJECT(DeeScopeObject) const *parent, DEE_A_IN_OBJECT(DeeScopeObject) const *child);
extern DEE_A_RET_WUNUSED int DeeScope_IsWeakParent(DEE_A_IN_OBJECT(DeeScopeObject) const *parent, DEE_A_IN_OBJECT(DeeScopeObject) const *child);
extern DEE_A_RET_WUNUSED int DeeScope_IsSameOrParent(DEE_A_IN_OBJECT(DeeScopeObject) const *parent, DEE_A_IN_OBJECT(DeeScopeObject) const *child);
extern DEE_A_RET_WUNUSED int DeeScope_IsSameOrWeakParent(DEE_A_IN_OBJECT(DeeScopeObject) const *parent, DEE_A_IN_OBJECT(DeeScopeObject) const *child);

//////////////////////////////////////////////////////////////////////////
// Import everything from 'src' in 'self'
// - Causes a compiler error if the import causes a name collision.
//   (That error is the only reason we need the lexer and and a token)
extern DEE_A_RET_EXCEPT(-1) int DeeScope_ImportScope(
 DEE_A_INOUT_OBJECT(DeeScopeObject) *self, DEE_A_IN_OBJECT(DeeScopeObject) const *src,
 DEE_A_INOUT_OBJECT(DeeLexerObject) *lexer, DEE_A_INOUT_OBJECT(DeeTokenObject) *token,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *depr);
extern DEE_A_RET_EXCEPT(-1) int DeeScope_ImportSingle(
 DEE_A_INOUT_OBJECT(DeeScopeObject) *self, DEE_A_IN TPPTokenID name, DEE_A_INOUT DeeObject *ob,
 DEE_A_INOUT_OBJECT(DeeLexerObject) *lexer, DEE_A_INOUT_OBJECT(DeeTokenObject) *token,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *depr);


//////////////////////////////////////////////////////////////////////////
// Returns 1, if "self" and "right" have a common weak parent
extern DEE_A_RET_WUNUSED int DeeScope_SameWeakScope(
 DEE_A_IN_OBJECT(DeeScopeObject) const *self,
 DEE_A_IN_OBJECT(DeeScopeObject) const *right);












struct DeeVarDeclStorage;
struct DeeLocalVarObject;
struct DeeStringObject;
struct DeeXAstObject;
struct DeeTokenObject;
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeVarDeclVarStorage {
 DEE_A_REF struct DeeTokenObject    *vs_token; /*< [1..1] Token used during error/debug output. */
 DEE_A_REF struct DeeLocalVarObject *vs_var;   /*< [1..1] Variable to assign to. (NOTE: This ast also owns +1 in 'vs_var->lv_init'). */
};
#define _DeeVarDeclVarStorage_Quit(ob)               do{Dee_DECREF((ob)->vs_token);Dee_DECREF((ob)->vs_var);}while(0)
#define _DeeVarDeclVarStorage_Visit(ob)              do{Dee_VISIT((ob)->vs_token);Dee_VISIT((ob)->vs_var);}while(0)
#define _DeeVarDeclVarStorage_InitCopy(ob,right,...) do{Dee_INCREF((ob)->vs_token=(right)->vs_token);Dee_INCREF((ob)->vs_var=(right)->vs_var);}while(0)
struct DeeVarDeclAttrStorage {
 DEE_A_REF struct DeeXAstObject *as_object; /*< [1..1] Base object. */
 DEE_A_REF struct DeeXAstObject *as_attr;   /*< [1..1] Attribute name. */
};
#define _DeeVarDeclAttrStorage_Quit(ob)               do{Dee_DECREF((ob)->as_object);Dee_DECREF((ob)->as_attr);}while(0)
#define _DeeVarDeclAttrStorage_Visit(ob)              do{Dee_VISIT((ob)->as_object);Dee_VISIT((ob)->as_attr);}while(0)
#define _DeeVarDeclAttrStorage_InitCopy(ob,right,...) do{Dee_INCREF((ob)->as_object=(right)->as_object);Dee_INCREF((ob)->as_attr=(right)->as_attr);}while(0)
struct DeeVarDeclAttrCStorage {
 DEE_A_REF struct DeeXAstObject   *acs_object; /*< [1..1] Base object. */
 DEE_A_REF struct DeeStringObject *acs_attr;   /*< [1..1] Attribute name. */
};
#define _DeeVarDeclAttrCStorage_Quit(ob)               do{Dee_DECREF((ob)->acs_object);Dee_DECREF((ob)->acs_attr);}while(0)
#define _DeeVarDeclAttrCStorage_Visit(ob)              do{Dee_VISIT((ob)->acs_object);Dee_VISIT((ob)->acs_attr);}while(0)
#define _DeeVarDeclAttrCStorage_InitCopy(ob,right,...) do{Dee_INCREF((ob)->acs_object=(right)->acs_object);Dee_INCREF((ob)->acs_attr=(right)->acs_attr);}while(0)
struct DeeVarDeclItemStorage {
 DEE_A_REF struct DeeXAstObject *is_object; /*< [1..1] Base object. */
 DEE_A_REF struct DeeXAstObject *is_key;    /*< [1..1] Item key. */
};
#define _DeeVarDeclItemStorage_Quit(ob)               do{Dee_DECREF((ob)->is_object);Dee_DECREF((ob)->is_key);}while(0)
#define _DeeVarDeclItemStorage_Visit(ob)              do{Dee_VISIT((ob)->is_object);Dee_VISIT((ob)->is_key);}while(0)
#define _DeeVarDeclItemStorage_InitCopy(ob,right,...) do{Dee_INCREF((ob)->is_object=(right)->is_object);Dee_INCREF((ob)->is_key=(right)->is_key);}while(0)
struct DeeVarDeclRangeStorage {
 DEE_A_REF struct DeeXAstObject *rs_object; /*< [1..1] Base object. */
 DEE_A_REF struct DeeXAstObject *rs_begin;  /*< [0..1] Lower range bound. */
 DEE_A_REF struct DeeXAstObject *rs_end;    /*< [0..1] Upper range bound. */
};
#define _DeeVarDeclRangeStorage_Quit(ob)               do{Dee_DECREF((ob)->rs_object);Dee_XDECREF((ob)->rs_begin);Dee_XDECREF((ob)->rs_end);}while(0)
#define _DeeVarDeclRangeStorage_Visit(ob)              do{Dee_VISIT((ob)->rs_object);Dee_XVISIT((ob)->rs_begin);Dee_XVISIT((ob)->rs_end);}while(0)
#define _DeeVarDeclRangeStorage_InitCopy(ob,right,...) do{Dee_INCREF((ob)->rs_object=(right)->rs_object);Dee_XINCREF((ob)->rs_begin=(right)->rs_begin);Dee_XINCREF((ob)->rs_end=(right)->rs_end);}while(0)
struct DeeVarDeclExpandStorage {
 Dee_size_t                es_declc; /*< Amount of expanded variable storage entries. */
 struct DeeVarDeclStorage *es_declv; /*< [1..es_declc] Vector of expanded variable storage. */
};
#define _DeeVarDeclExpandStorage_Quit(ob)\
do{\
 struct DeeVarDeclStorage *_exq_iter,*_exq_end;\
 _exq_end = (_exq_iter = (ob)->es_declv)+(ob)->es_declc;\
 DEE_ASSERT(_exq_iter != _exq_end);\
 do DeeVarDeclStorage_Quit(_exq_iter);\
 while (++_exq_iter != _exq_end);\
 free_nn((ob)->es_declv);\
}while(0)
#define _DeeVarDeclExpandStorage_Visit(ob)\
do{\
 struct DeeVarDeclStorage *_exq_iter,*_exq_end;\
 _exq_end = (_exq_iter = (ob)->es_declv)+(ob)->es_declc;\
 DEE_ASSERT(_exq_iter != _exq_end);\
 do DeeVarDeclStorage_Visit(_exq_iter,visit,closure);\
 while (++_exq_iter != _exq_end);\
}while(0)
#define _DeeVarDeclExpandStorage_InitCopy(ob,right,...)\
do{\
 struct DeeVarDeclStorage *_exq_new,*_exq_iter,*_exq_end;\
 (ob)->es_declc = (right)->es_declc;\
 DEE_ASSERT((ob)->es_declc != 0);\
 while ((_exq_new = (struct DeeVarDeclStorage *)malloc_nz(\
  (ob)->es_declc*sizeof(struct DeeVarDeclStorage))) == NULL) {\
  if DEE_LIKELY(Dee_CollectMemory()) continue;\
  DeeError_NoMemory();\
  {__VA_ARGS__;}\
 }\
 (ob)->es_declv = _exq_new;\
 _exq_end = (_exq_iter = _exq_new)+(ob)->es_declc;\
 _exq_new = (right)->es_declv;\
 do {\
  if (DeeVarDeclStorage_InitCopy(_exq_iter,_exq_new) != 0) {\
   free_nn((ob)->es_declv);\
   {__VA_ARGS__;}\
  }\
  ++_exq_iter,++_exq_new;\
 } while (_exq_iter != _exq_end);\
}while(0)

struct DeeVarDeclAssignStorage {
 DEE_A_REF struct DeeXAstObject *as_ast; /*< [1..1] Expression to assign to. */
};
#define _DeeVarDeclAssignStorage_Quit(ob)               do{Dee_DECREF((ob)->as_ast);}while(0)
#define _DeeVarDeclAssignStorage_Visit(ob)              do{Dee_VISIT((ob)->as_ast);}while(0)
#define _DeeVarDeclAssignStorage_InitCopy(ob,right,...) do{Dee_INCREF((ob)->as_ast=(right)->as_ast);}while(0)

struct DeeVarDeclStorage {
#define DEE_VARDECLSTORAGEKIND_NONE   0 /*< none; (Drop value) */
#define DEE_VARDECLSTORAGEKIND_VAR    1 /*< x */
#define DEE_VARDECLSTORAGEKIND_ATTR   2 /*< __getattr__(x,"foobar") */
#define DEE_VARDECLSTORAGEKIND_ATTR_C 3 /*< x.foobar */
#define DEE_VARDECLSTORAGEKIND_ITEM   4 /*< x[42] */
#define DEE_VARDECLSTORAGEKIND_RANGE  5 /*< x[10:20] */
#define DEE_VARDECLSTORAGEKIND_EXPAND 6 /*< x,y (expand value to fix storage) */
#define DEE_VARDECLSTORAGEKIND_ASSIGN 7 /*< -everything else- (compile as :=) */
 Dee_uint32_t                    vds_kind;  /*< The kind of variable declaration. */
 union{
  struct DeeVarDeclVarStorage    vds_var;    /*< DEE_VARDECLSTORAGEKIND_VAR. */
  struct DeeVarDeclAttrStorage   vds_attr;   /*< DEE_VARDECLSTORAGEKIND_ATTR. */
  struct DeeVarDeclAttrCStorage  vds_attr_c; /*< DEE_VARDECLSTORAGEKIND_ATTR_C. */
  struct DeeVarDeclItemStorage   vds_item;   /*< DEE_VARDECLSTORAGEKIND_ITEM. */
  struct DeeVarDeclRangeStorage  vds_range;  /*< DEE_VARDECLSTORAGEKIND_RANGE. */
  struct DeeVarDeclExpandStorage vds_expand; /*< DEE_VARDECLSTORAGEKIND_EXPAND. */
  struct DeeVarDeclAssignStorage vds_assign; /*< DEE_VARDECLSTORAGEKIND_ASSIGN. */
 };
};
DEE_COMPILER_MSVC_WARNING_POP

extern DEE_A_RET_EXCEPT(-1) int DeeVarDeclStorage_InitCopy(
 DEE_A_OUT struct DeeVarDeclStorage *self,
 DEE_A_IN struct DeeVarDeclStorage const *right);
extern void DeeVarDeclStorage_Quit(DEE_A_IN struct DeeVarDeclStorage *self);
extern void DeeVarDeclStorage_Visit(
 DEE_A_IN struct DeeVarDeclStorage *self,
 DEE_A_IN DeeVisitProc visit, void *closure);

//////////////////////////////////////////////////////////////////////////
// Returns true/false if 'self' uses 'variable' in some way, shape or form
// in "xast.variable_usage.inl"
extern DEE_A_RET_WUNUSED int DeeVarDeclStorage_UsesVariable(
 DEE_A_IN struct DeeVarDeclStorage const *self, DEE_A_IN DeeLocalVarObject const *variable);
extern DEE_A_RET_WUNUSED int DeeVarDeclStorage_LoadsVariable(
 DEE_A_IN struct DeeVarDeclStorage const *self, DEE_A_IN DeeLocalVarObject const *variable);
extern DEE_A_RET_WUNUSED int DeeVarDeclStorage_StoresVariable(
 DEE_A_IN struct DeeVarDeclStorage const *self, DEE_A_IN DeeLocalVarObject const *variable);
extern DEE_A_RET_WUNUSED Dee_size_t DeeVarDeclStorage_CountVariableUses(
 DEE_A_IN struct DeeVarDeclStorage const *self, DEE_A_IN DeeLocalVarObject const *variable);
extern DEE_A_RET_WUNUSED Dee_size_t DeeVarDeclStorage_CountVariableLoads(
 DEE_A_IN struct DeeVarDeclStorage const *self, DEE_A_IN DeeLocalVarObject const *variable);
extern DEE_A_RET_WUNUSED Dee_size_t DeeVarDeclStorage_CountVariableStores(
 DEE_A_IN struct DeeVarDeclStorage const *self, DEE_A_IN DeeLocalVarObject const *variable);


extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) * // in "xast.repr.inl"
 DeeVarDeclStorage_Repr(DEE_A_IN struct DeeVarDeclStorage const *self);

//////////////////////////////////////////////////////////////////////////
// Check two vardecl storage for equality (returns <0 on error)
// in "xast.equals.inl"
extern DEE_A_RET_EXCEPT_FAIL(-1,0) int DeeVarDeclStorage_Equals(
 DEE_A_IN struct DeeVarDeclStorage const *self,
 DEE_A_IN struct DeeVarDeclStorage const *right);

//////////////////////////////////////////////////////////////////////////
// in "xast.collect_top_scope_children.inl"
struct DeeScopeList;
extern DEE_A_RET_EXCEPT(-1) int DeeVarDeclStorage_CollectTopScopeChildren(
 DEE_A_IN struct DeeVarDeclStorage const *self, DEE_A_INOUT struct DeeScopeList *scopes);

struct DeeLexerObject;
// NOTE: Returns '1' if 'self' was initialized as a direct reference-copy of 'right'
//       (as in: all pointers are just new references to the same objects as in 'right')
// in "xast.copy_with_scope.inl"
extern DEE_A_RET_EXCEPT(-1) int DeeVarDeclStorage_InitCopyWithScope(
 DEE_A_OUT struct DeeVarDeclStorage *self, DEE_A_IN struct DeeVarDeclStorage const *right,
 DEE_A_INOUT DeeScopeObject *new_scope, DEE_A_INOUT struct DeeLexerObject *lexer);



#define _DeeVarDeclStorage_InitCopy(ob,right,...)\
do{\
 switch (((ob)->vds_kind = (right)->vds_kind)) {\
  case DEE_VARDECLSTORAGEKIND_VAR:    _DeeVarDeclVarStorage_InitCopy(&(ob)->vds_var,&(right)->vds_var,__VA_ARGS__); break;\
  case DEE_VARDECLSTORAGEKIND_ATTR:   _DeeVarDeclAttrStorage_InitCopy(&(ob)->vds_attr,&(right)->vds_attr,__VA_ARGS__); break;\
  case DEE_VARDECLSTORAGEKIND_ATTR_C: _DeeVarDeclAttrCStorage_InitCopy(&(ob)->vds_attr_c,&(right)->vds_attr_c,__VA_ARGS__); break;\
  case DEE_VARDECLSTORAGEKIND_ITEM:   _DeeVarDeclItemStorage_InitCopy(&(ob)->vds_item,&(right)->vds_item,__VA_ARGS__); break;\
  case DEE_VARDECLSTORAGEKIND_RANGE:  _DeeVarDeclRangeStorage_InitCopy(&(ob)->vds_range,&(right)->vds_range,__VA_ARGS__); break;\
  case DEE_VARDECLSTORAGEKIND_EXPAND: _DeeVarDeclExpandStorage_InitCopy(&(ob)->vds_expand,&(right)->vds_expand,__VA_ARGS__); break;\
  case DEE_VARDECLSTORAGEKIND_ASSIGN: _DeeVarDeclAssignStorage_InitCopy(&(ob)->vds_assign,&(right)->vds_assign,__VA_ARGS__); break;\
  default: break;\
 }\
}while(0)

#define _DeeVarDeclStorage_Quit(ob)\
do{\
 switch ((ob)->vds_kind) {\
  case DEE_VARDECLSTORAGEKIND_VAR:    _DeeVarDeclVarStorage_Quit(&(ob)->vds_var); break;\
  case DEE_VARDECLSTORAGEKIND_ATTR:   _DeeVarDeclAttrStorage_Quit(&(ob)->vds_attr); break;\
  case DEE_VARDECLSTORAGEKIND_ATTR_C: _DeeVarDeclAttrCStorage_Quit(&(ob)->vds_attr_c); break;\
  case DEE_VARDECLSTORAGEKIND_ITEM:   _DeeVarDeclItemStorage_Quit(&(ob)->vds_item); break;\
  case DEE_VARDECLSTORAGEKIND_RANGE:  _DeeVarDeclRangeStorage_Quit(&(ob)->vds_range); break;\
  case DEE_VARDECLSTORAGEKIND_EXPAND: _DeeVarDeclExpandStorage_Quit(&(ob)->vds_expand); break;\
  case DEE_VARDECLSTORAGEKIND_ASSIGN: _DeeVarDeclAssignStorage_Quit(&(ob)->vds_assign); break;\
  default: break;\
 }\
}while(0)
#define _DeeVarDeclStorage_Visit(ob)\
do{\
 switch ((ob)->vds_kind) {\
  case DEE_VARDECLSTORAGEKIND_VAR:    _DeeVarDeclVarStorage_Visit(&(ob)->vds_var); break;\
  case DEE_VARDECLSTORAGEKIND_ATTR:   _DeeVarDeclAttrStorage_Visit(&(ob)->vds_attr); break;\
  case DEE_VARDECLSTORAGEKIND_ATTR_C: _DeeVarDeclAttrCStorage_Visit(&(ob)->vds_attr_c); break;\
  case DEE_VARDECLSTORAGEKIND_ITEM:   _DeeVarDeclItemStorage_Visit(&(ob)->vds_item); break;\
  case DEE_VARDECLSTORAGEKIND_RANGE:  _DeeVarDeclRangeStorage_Visit(&(ob)->vds_range); break;\
  case DEE_VARDECLSTORAGEKIND_EXPAND: _DeeVarDeclExpandStorage_Visit(&(ob)->vds_expand); break;\
  case DEE_VARDECLSTORAGEKIND_ASSIGN: _DeeVarDeclAssignStorage_Visit(&(ob)->vds_assign); break;\
  default: break;\
 }\
}while(0)


DEE_DECL_END

#endif /* !GUARD_DEEMON_STMT_AST_H */
