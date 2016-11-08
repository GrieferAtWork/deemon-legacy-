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
#ifndef GUARD_DEEMON_XAST_C
#define GUARD_DEEMON_XAST_C 1
#define DEE_LIMITED_API 1

// include/*
#include <deemon/__conf.inl>
#include <deemon/array.h>
#include <deemon/bool.h>
#include <deemon/cell.h>
#include <deemon/cfunction.h>
#include <deemon/class.h>
#include <deemon/deemonrun.h>
#include <deemon/dex.h>
#include <deemon/dict.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/float.h>
#include <deemon/foreign_function.h>
#include <deemon/fs_api.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/iterator.h>
#include <deemon/list.h>
#include <deemon/memberdef.h>
#include <deemon/memberfunction.h>
#include <deemon/none.h>
#include <deemon/object.h>
#include <deemon/optional/object_doc.h>
#include <deemon/optional/object_pool.h>
#include <deemon/optional/object_visit.h>
#include <deemon/runtime/builtins.h>
#include <deemon/runtime/function.h>
#include <deemon/sequence.h>
#include <deemon/set.h>
#include <deemon/string.h>
#include <deemon/structured.h>
#include <deemon/super.h>
#include <deemon/tuple.h>
#include <deemon/weakref.h>

// src/*
#include <deemon/__alloca.inl>
#include <deemon/__xconf.inl>
#include <deemon/compiler/sast.h>
#include <deemon/compiler/xast.h>
#include <deemon/runtime/builtin_functions.h>
#include <deemon/marshal_data.h>

#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN
DEE_OBJECTPOOL_DEFINE(xast,DeeXAstObject)


#define VLOG_PARSER(tk,...) (\
 DEE_LVERBOSE1R("%s(%d) : %k : PARSER : ",\
 DeeToken_FILE(tk),DeeToken_LINE(tk)+1,tk),\
 DEE_LVERBOSE1R(__VA_ARGS__))



#define _DeeXAstCommonAst_Quit(ob)\
do{\
 Dee_DECREF((ob)->ast_token);\
}while(0)
#define _DeeXAstConstAst_Quit(ob) \
do{\
 Dee_DECREF((ob)->c_const);\
}while(0)
#define _DeeXAstVarAst_Quit(ob)\
do{\
 DeeLocalVar_REM_USE((ob)->vs_var);\
 Dee_DECREF((ob)->vs_var);\
}while(0)
#define _DeeXAstVarDeclAst_Quit(ob) \
do{\
 Dee_DECREF((ob)->vd_token);\
 DeeLocalVar_REM_INIT((ob)->vd_var);\
 Dee_DECREF((ob)->vd_var);\
 Dee_DECREF((ob)->vd_init);\
}while(0)
#define _DeeXAstMultiVarDeclAst_Quit(ob) \
do{\
 struct DeeVarDeclStorage *_mvd_iter,*_mvd_end;\
 DEE_ASSERT((ob)->mvd_varc != 0);\
 _mvd_end = (_mvd_iter = (ob)->mvd_varv)+(ob)->mvd_varc;\
 do _DeeVarDeclStorage_Quit(_mvd_iter); while DEE_LIKELY(++_mvd_iter != _mvd_end);\
 free_nn((ob)->mvd_varv);\
 Dee_DECREF((ob)->mvd_init);\
}while(0)
#define _DeeXAstStatementAst_Quit(ob)\
do{\
 Dee_DECREF((ob)->s_stmt);\
}while(0)
#define _DeeXAstIfAst_Quit(ob)\
do{\
 Dee_DECREF((ob)->if_cond);\
 Dee_XDECREF((ob)->if_succ);\
 Dee_XDECREF((ob)->if_fail);\
}while(0)
#define _DeeXAstFunctionAst_Quit(ob)\
do{\
 struct DeeXAstFunctionAstArgument *iter,*end;\
 Dee_XDECREF((ob)->f_name);\
 end = (iter = (ob)->f_argv)+(ob)->f_argc;\
 while DEE_LIKELY(iter != end) { _DeeXAstFunctionAstArgument_Quit(iter); ++iter; }\
 free((ob)->f_argv);\
 Dee_XDECREF((ob)->f_code);\
 Dee_XDECREF((ob)->f_scope);\
 \
}while(0)
#define _DeeXAstClassAst_Quit(ob)\
do{\
 struct DeeXAstClassAstEntry *iter,*end;\
 Dee_XDECREF((ob)->c_name);\
 Dee_XDECREF((ob)->c_base);\
 end = (iter = (ob)->c_entryv)+(ob)->c_entryc;\
 while DEE_LIKELY(iter != end) { _DeeXAstClassAstEntry_Quit(iter); ++iter; }\
 free((ob)->c_entryv);\
}while(0)
#define _DeeXAstSequenceAst_Quit(ob)\
do{\
 DeeXAstObject **_seq_iter,**_seq_end;\
 _seq_end = (_seq_iter = (ob)->s_elemv)+(ob)->s_elemc;\
 DEE_ASSERT((ob)->s_elemc);\
 do Dee_DECREF(*_seq_iter); while DEE_LIKELY(++_seq_iter != _seq_end);\
 free_nn((ob)->s_elemv);\
}while(0)
#define _DeeXAstDictAst_Quit(ob)\
do{\
 struct DeeXAstDictEntry *_dict_iter,*_dict_end;\
 _dict_end = (_dict_iter = (ob)->d_elemv)+(ob)->d_elemc;\
 DEE_ASSERT(_dict_iter != _dict_end);\
 do _DeeXAstDictEntry_Quit(_dict_iter);\
 while DEE_LIKELY(++_dict_iter != _dict_end); \
 free_nn((ob)->d_elemv);\
}while(0)
#define _DeeXAstSeqRangeAst_Quit(ob)\
do{\
 Dee_DECREF((ob)->sr_seq);\
 Dee_XDECREF((ob)->sr_begin);\
 Dee_XDECREF((ob)->sr_end);\
 if ((ob)->ast_kind_ == DEE_XASTKIND_SEQ_RANGE_SET)\
  Dee_DECREF((ob)->sr_value);\
}while(0)
#define _DeeXAstAttrCAst_Quit(ob)\
do{\
 Dee_DECREF((ob)->ac_object);\
 Dee_DECREF((ob)->ac_name);\
 if ((ob)->ast_kind_ == DEE_XASTKIND_ATTR_SET_C)\
  Dee_DECREF((ob)->ac_value);\
}while(0)
#define _DeeXAstModuleAst_Quit(ob)\
do{\
 Dee_DECREF((ob)->m_module);\
}while(0)
#define _DeeXAstDelVarAst_Quit(ob)\
do{\
 Dee_DECREF((ob)->d_token);\
 Dee_DECREF((ob)->d_var);\
}while(0)
#define _DeeXAstForeignFunctionAst_Quit(ob)\
do{\
 DeeXAstObject **iter,**end;\
 Dee_DECREF((ob)->ff_base);\
 end = (iter = (ob)->ff_argv)+(ob)->ff_argc;\
 while DEE_LIKELY(iter != end) Dee_DECREF(*iter++);\
 free((ob)->ff_argv);\
}while(0)
#define _DeeXAstSwitchAst_Quit(ob)\
do{\
 struct DeeXAstSwitchEntry *s_iter,*s_end;\
 Dee_DECREF((ob)->s_expr);\
 DEE_ASSERT((ob)->s_entryc);\
 s_end = (s_iter = (ob)->s_entryv)+(ob)->s_entryc;\
 do _DeeXAstSwitchEntry_Quit(s_iter);\
 while DEE_LIKELY(++s_iter != s_end);\
 free_nn((ob)->s_entryv);\
 Dee_XDECREF((ob)->s_default);\
}while(0)
#define _DeeXAstIfConstAst_Quit(ob)\
do{\
 Dee_XDECREF((ob)->ic_succ);\
 Dee_XDECREF((ob)->ic_fail);\
}while(0)
#define _DeeXAstOperatorAst_Quit(ob)\
do{\
 switch (DEE_XASTKIND_OPCOUNT((ob)->ast_kind_)) {\
  case 3: Dee_DECREF((ob)->op_c); DEE_ATTRIBUTE_FALLTHROUGH\
  case 2: Dee_DECREF((ob)->op_b); DEE_ATTRIBUTE_FALLTHROUGH\
  case 1: Dee_DECREF((ob)->op_a);\
  default: break;\
 }\
}while(0)
#define _DeeXAstUnaryInplaceVarAst_Quit(ob)\
do{\
 DeeLocalVar_REM_INIT((ob)->uiv_var);\
 DeeLocalVar_REM_USE((ob)->uiv_var);\
 Dee_DECREF((ob)->uiv_var);\
 Dee_DECREF((ob)->uiv_tok);\
}while(0)
#define _DeeXAstBinaryInplaceVarAst_Quit(ob)\
do{\
 DeeLocalVar_REM_INIT((ob)->biv_var);\
 DeeLocalVar_REM_USE((ob)->biv_var);\
 Dee_DECREF((ob)->biv_var);\
 Dee_DECREF((ob)->biv_tok);\
 Dee_DECREF((ob)->biv_arg);\
}while(0)
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
#define _DeeXAstBuiltinBoundAst_Quit(ob)\
do{\
 Dee_DECREF((ob)->b_token);\
 Dee_DECREF((ob)->b_var);\
}while(0)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */
#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
#define _DeeXAstBuiltinExternAst_Quit(ob)\
do{\
 Dee_XDECREF((ob)->ex_file);\
 Dee_DECREF((ob)->ex_name);\
 Dee_DECREF((ob)->ex_type);\
}while(0)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
#define _DeeXAstBuiltinExpectAst_Quit(ob)\
do{\
 Dee_DECREF((ob)->e_value);\
 Dee_DECREF((ob)->e_expect);\
}while(0)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */



#define _DeeXAstClassAstEntry_Visit(ob)\
do{\
 switch ((ob)->ce_kind) {\
  case DEE_XAST_CLASSENTRY_KIND_SLOT:\
   Dee_VISIT((ob)->ce_slot.cs_callback);\
   break;\
  case DEE_XAST_CLASSENTRY_KIND_METHOD:\
  case DEE_XAST_CLASSENTRY_KIND_CLASS_METHOD:\
   Dee_VISIT((ob)->ce_method.cm_name);\
   Dee_VISIT((ob)->ce_method.cm_callback);\
   break;\
  case DEE_XAST_CLASSENTRY_KIND_GETSET:\
  case DEE_XAST_CLASSENTRY_KIND_CLASS_GETSET:\
   Dee_VISIT((ob)->ce_getset.cg_name);\
   Dee_XVISIT((ob)->ce_getset.cg_get);\
   Dee_XVISIT((ob)->ce_getset.cg_del);\
   Dee_XVISIT((ob)->ce_getset.cg_set);\
   break;\
  default: break;\
 }\
}while(0)

#define _DeeXAstSwitchEntry_Visit(ob)\
do{\
 Dee_VISIT((ob)->se_key);\
 Dee_VISIT((ob)->se_value);\
}while(0)


#define _DeeXAstCommonAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->ast_token);\
}while(0)
#define _DeeXAstConstAst_Visit(ob) \
do{\
 Dee_VISIT((ob)->c_const);\
}while(0)
#define _DeeXAstVarAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->vs_var);\
}while(0)
#define _DeeXAstVarDeclAst_Visit(ob) \
do{\
 Dee_VISIT((ob)->vd_token);\
 Dee_VISIT((ob)->vd_var);\
 Dee_VISIT((ob)->vd_init);\
}while(0)
#define _DeeXAstMultiVarDeclAst_Visit(ob) \
do{\
 struct DeeVarDeclStorage *_mvd_iter,*_mvd_end;\
 DEE_ASSERT((ob)->mvd_varc >= 2);\
 _mvd_end = (_mvd_iter = (ob)->mvd_varv)+(ob)->mvd_varc;\
 do _DeeVarDeclStorage_Visit(_mvd_iter);\
 while DEE_LIKELY(++_mvd_iter != _mvd_end);\
 Dee_VISIT((ob)->mvd_init);\
}while(0)
#define _DeeXAstStatementAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->s_stmt);\
}while(0)
#define _DeeXAstIfAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->if_cond);\
 Dee_XVISIT((ob)->if_succ);\
 Dee_XVISIT((ob)->if_fail);\
}while(0)
#define _DeeXAstFunctionAst_Visit(ob)\
do{\
 struct DeeXAstFunctionAstArgument *iter,*end;\
 Dee_XVISIT((ob)->f_name);\
 end = (iter = (ob)->f_argv)+(ob)->f_argc;\
 while DEE_LIKELY(iter != end) { _DeeXAstFunctionAstArgument_Visit(iter); ++iter; }\
 Dee_XVISIT((ob)->f_code);\
 Dee_XVISIT((ob)->f_scope);\
 \
}while(0)
#define _DeeXAstClassAst_Visit(ob)\
do{\
 struct DeeXAstClassAstEntry *iter,*end;\
 Dee_XVISIT((ob)->c_name);\
 Dee_XVISIT((ob)->c_base);\
 end = (iter = (ob)->c_entryv)+(ob)->c_entryc;\
 while DEE_LIKELY(iter != end) { _DeeXAstClassAstEntry_Visit(iter); ++iter; }\
}while(0)
#define _DeeXAstSequenceAst_Visit(ob)\
do{\
 DeeXAstObject **_seq_iter,**_seq_end;\
 _seq_end = (_seq_iter = (ob)->s_elemv)+(ob)->s_elemc;\
 DEE_ASSERT(_seq_iter != _seq_end);\
 do Dee_VISIT(*_seq_iter);\
 while DEE_LIKELY(++_seq_iter != _seq_end);\
}while(0)
#define _DeeXAstDictAst_Visit(ob)\
do{\
 struct DeeXAstDictEntry *_dict_iter,*_dict_end;\
 _dict_end = (_dict_iter = (ob)->d_elemv)+(ob)->d_elemc;\
 DEE_ASSERT(_dict_iter != _dict_end);\
 do _DeeXAstDictEntry_Visit(_dict_iter);\
 while DEE_LIKELY(++_dict_iter != _dict_end);\
}while(0)
#define _DeeXAstSeqRangeAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->sr_seq);\
 Dee_XVISIT((ob)->sr_begin);\
 Dee_XVISIT((ob)->sr_end);\
 if ((ob)->ast_kind_ == DEE_XASTKIND_SEQ_RANGE_SET)\
  Dee_VISIT((ob)->sr_value);\
}while(0)
#define _DeeXAstAttrCAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->ac_object);\
 Dee_VISIT((ob)->ac_name);\
 if ((ob)->ast_kind_ == DEE_XASTKIND_ATTR_SET_C)\
  Dee_VISIT((ob)->ac_value);\
}while(0)
#define _DeeXAstModuleAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->m_module);\
}while(0)
#define _DeeXAstDelVarAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->d_token);\
 Dee_VISIT((ob)->d_var);\
}while(0)
#define _DeeXAstForeignFunctionAst_Visit(ob)\
do{\
 DeeXAstObject **iter,**end;\
 Dee_VISIT((ob)->ff_base);\
 end = (iter = (ob)->ff_argv)+(ob)->ff_argc;\
 while DEE_LIKELY(iter != end) Dee_VISIT(*iter++);\
}while(0)
#define _DeeXAstSwitchAst_Visit(ob)\
do{\
 struct DeeXAstSwitchEntry *s_iter,*s_end;\
 Dee_XVISIT((ob)->s_expr);\
 DEE_ASSERT((ob)->s_entryc);\
 s_end = (s_iter = (ob)->s_entryv)+(ob)->s_entryc;\
 do _DeeXAstSwitchEntry_Visit(s_iter);\
 while DEE_LIKELY(++s_iter != s_end);\
 Dee_XVISIT((ob)->s_default);\
}while(0)
#define _DeeXAstIfConstAst_Visit(ob)\
do{\
 Dee_XVISIT((ob)->ic_succ);\
 Dee_XVISIT((ob)->ic_fail);\
}while(0)
#define _DeeXAstOperatorAst_Visit(ob)\
do{\
 switch (DEE_XASTKIND_OPCOUNT((ob)->ast_kind_)) {\
  case 3: Dee_VISIT((ob)->op_c); DEE_ATTRIBUTE_FALLTHROUGH\
  case 2: Dee_VISIT((ob)->op_b); DEE_ATTRIBUTE_FALLTHROUGH\
  case 1: Dee_VISIT((ob)->op_a);\
  default: break;\
 }\
}while(0)
#define _DeeXAstUnaryInplaceVarAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->uiv_var);\
 Dee_VISIT((ob)->uiv_tok);\
}while(0)
#define _DeeXAstBinaryInplaceVarAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->biv_var);\
 Dee_VISIT((ob)->biv_tok);\
 Dee_VISIT((ob)->biv_arg);\
}while(0)

#define _DeeXAstSuperAtAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->sa_object);\
 Dee_VISIT((ob)->sa_type);\
}while(0)

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
#define _DeeXAstBuiltinBoundAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->b_token);\
 Dee_VISIT((ob)->b_var);\
}while(0)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */
#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
#define _DeeXAstBuiltinExternAst_Visit(ob)\
do{\
 Dee_XVISIT((ob)->ex_file);\
 Dee_VISIT((ob)->ex_name);\
 Dee_VISIT((ob)->ex_type);\
}while(0)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
#define _DeeXAstBuiltinExpectAst_Visit(ob)\
do{\
 Dee_VISIT((ob)->e_value);\
 Dee_VISIT((ob)->e_expect);\
}while(0)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */

#ifdef __INTELLISENSE__
#undef _DeeXAstSeqRangeAst_Quit
#undef _DeeXAstAttrCAst_Quit
#undef _DeeXAstOperatorAst_Quit
#undef _DeeXAstSeqRangeAst_Visit
#undef _DeeXAstAttrCAst_Visit
#undef _DeeXAstOperatorAst_Visit
#define _DeeXAstSeqRangeAst_Quit(ob) do{}while(0)
#define _DeeXAstAttrCAst_Quit(ob) do{}while(0)
#define _DeeXAstOperatorAst_Quit(ob) do{}while(0)
#define _DeeXAstSeqRangeAst_Visit(ob) do{}while(0)
#define _DeeXAstAttrCAst_Visit(ob) do{}while(0)
#define _DeeXAstOperatorAst_Visit(ob) do{}while(0)
#endif

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
DEE_A_RET_WUNUSED int DeeXAstClassAstEntry_SameDeclaration(
 DEE_A_IN struct DeeXAstClassAstEntry const *a,
 DEE_A_IN struct DeeXAstClassAstEntry const *b) {
 DeeStringObject *a_name,*b_name;
 DEE_ASSERT(a && b);
 switch (a->ce_kind) {
  case DEE_XAST_CLASSENTRY_KIND_SLOT:
   if (b->ce_kind != DEE_XAST_CLASSENTRY_KIND_SLOT) return 0;
   return a->ce_slot.cs_slotid == b->ce_slot.cs_slotid;
  case DEE_XAST_CLASSENTRY_KIND_METHOD:
  case DEE_XAST_CLASSENTRY_KIND_CLASS_METHOD:
   a_name = a->ce_method.cm_name;
   break;
  case DEE_XAST_CLASSENTRY_KIND_GETSET:
  case DEE_XAST_CLASSENTRY_KIND_CLASS_GETSET:
   a_name = a->ce_getset.cg_name;
   break;
  default: return 0;
 }
 switch (b->ce_kind) {
  case DEE_XAST_CLASSENTRY_KIND_METHOD:
  case DEE_XAST_CLASSENTRY_KIND_CLASS_METHOD:
   b_name = b->ce_method.cm_name;
   break;
  case DEE_XAST_CLASSENTRY_KIND_GETSET:
  case DEE_XAST_CLASSENTRY_KIND_CLASS_GETSET:
   b_name = b->ce_getset.cg_name;
   break;
  default: return 0;
 }
 // Compare method/getset names
 DEE_ASSERT(DeeObject_Check(a_name) && DeeString_Check(a_name));
 DEE_ASSERT(DeeObject_Check(b_name) && DeeString_Check(b_name));
 if DEE_LIKELY(DeeString_SIZE(a_name) != DeeString_SIZE(b_name)) return 0;
 return memcmp(DeeString_STR(a_name),DeeString_STR(b_name),
               DeeString_SIZE(a_name)*sizeof(Dee_Char)) == 0;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */


DEE_A_RET_EXCEPT(-1) int DeeXAst_WarnInplaceOnConstant(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_INOUT DeeTokenObject *tok, DEE_A_IN DeeTypeObject const *lhs_operator_type) {
 DeeObject *expr_repr; char const *opname,*type_name;
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(tok) && DeeToken_Check(tok));
 DEE_ASSERT(DeeObject_Check(lhs_operator_type) && DeeType_Check(lhs_operator_type));
 type_name = DeeType_NAME(lhs_operator_type);
 switch (kind) {
  {
   if (0) { case DEE_XASTKIND_INC: opname = "++"; }
   if (0) { case DEE_XASTKIND_DEC: opname = "--"; }
   expr_repr = DeeString_Newf("%s%#q",opname,type_name); 
   break;
  }
  {
   if (0) { case DEE_XASTKIND_INCPOST: opname = "++"; }
   if (0) { case DEE_XASTKIND_DECPOST: opname = "--"; }
   expr_repr = DeeString_Newf("%#q%s",type_name,opname); 
   break;
  }
  {
   if (0) { case DEE_XASTKIND_IADD: opname = "+="; }
   if (0) { case DEE_XASTKIND_ISUB: opname = "-="; }
   if (0) { case DEE_XASTKIND_IMUL: opname = "*="; }
   if (0) { case DEE_XASTKIND_IDIV: opname = "/="; }
   if (0) { case DEE_XASTKIND_IMOD: opname = "%="; }
   if (0) { case DEE_XASTKIND_ISHL: opname = "<<="; }
   if (0) { case DEE_XASTKIND_ISHR: opname = ">>="; }
   if (0) { case DEE_XASTKIND_IAND: opname = "&="; }
   if (0) { case DEE_XASTKIND_IOR:  opname = "|="; }
   if (0) { case DEE_XASTKIND_IXOR: opname = "^="; }
   if (0) { case DEE_XASTKIND_IPOW: opname = "**="; }
   expr_repr = DeeString_Newf("%#q %s ...",type_name,opname); 
   break;
  }
  {
   if (0) { case DEE_XASTKIND_SEQ_DEL:       opname = "..."; }
   if (0) { case DEE_XASTKIND_SEQ_RANGE_DEL: opname = "...:..."; }
   expr_repr = DeeString_Newf("del %#q[%s]",type_name,opname); 
   break;
  }
  {
   if (0) { case DEE_XASTKIND_MOVE_ASSIGN: opname = "move"; }
   if (0) { case DEE_XASTKIND_COPY_ASSIGN: opname = "copy"; }
   expr_repr = DeeString_Newf("%#q.operator %s = (...)",type_name,opname); 
   break;
  }
  case DEE_XASTKIND_ATTR_DEL:
  case DEE_XASTKIND_ATTR_DEL_C:
   expr_repr = DeeString_Newf("del %#q.<...>",type_name); 
   break;
  case DEE_XASTKIND_SEQ_SET:
   expr_repr = DeeString_Newf("%#q[...] = ...",type_name); 
   break;
  case DEE_XASTKIND_ATTR_SET:
  case DEE_XASTKIND_ATTR_SET_C:
   expr_repr = DeeString_Newf("%#q.<...> = ...",type_name); 
   break;
  default: expr_repr = DeeString_Newf("%#q.operator ? ()",type_name); break;
 }
 return DeeError_CompilerErrorf(DEE_WARNING_INPLACE_ON_CONSTANT,
                                (DeeObject *)lexer,(DeeObject *)tok,
                                "Inplace operation '%K' with constant has no effect",
                                expr_repr);
}




DEE_A_RET_EXCEPT(-1) int _DeePointerList_AppendImpl(
 DEE_A_INOUT struct _DeePointerList *self, DEE_A_IN void *elem) {
 void **new_list;
 Dee_size_t new_vector_size;
 DEE_ASSERT(self);
 if DEE_UNLIKELY(self->pl_data.pl_c == self->pl_data.pl_a) {
  if DEE_UNLIKELY(!self->pl_data.pl_a) {
   while DEE_UNLIKELY((new_list = (void **)malloc_nz(2*sizeof(void *))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
no_mem: DeeError_NoMemory();
    return -1;
   }
   self->pl_data.pl_a = 2;
  } else {
   new_vector_size = self->pl_data.pl_a*2;
   while DEE_UNLIKELY((new_list = (void **)realloc_nnz(
    self->pl_data.pl_v,new_vector_size*sizeof(void *))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    goto no_mem;
   }
   self->pl_data.pl_a = new_vector_size;
  }
  self->pl_data.pl_v = new_list;
 }
 self->pl_data.pl_v[self->pl_data.pl_c++] = elem;
 return 0;
}




DEE_A_RET_WUNUSED Dee_size_t DeeXAst_ClearPool(void) {
 return DEE_OBJECTPOOL_CLEAR(xast);
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *_DeeXAst_NewUnsafe(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk) {
 DeeXAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 if DEE_LIKELY((result = DEE_OBJECTPOOL_ALLOC(xast)) != NULL) {
  DeeObject_INIT(result,&DeeXAst_Type);
  result->ast_kind = kind;
  Dee_INCREF(result->ast_common.ast_token = tk);
 }
 return result;
}


DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewConst(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeObject *constant) {
 DeeXAstObject *result;
 DEE_ASSERT(DeeObject_Check(constant));
 if DEE_LIKELY((result = _DeeXAst_NewUnsafe(DEE_XASTKIND_CONST,tk)) != NULL) {
  Dee_INCREF(result->ast_const.c_const = constant);
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewVar(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLocalVarObject *localvar,
 DEE_A_INOUT DeeScopeObject *curr_scope) {
 DeeXAstObject *result; Dee_uint32_t flags;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(localvar) && DeeLocalVar_Check(localvar));
 DEE_ASSERT(DeeObject_Check(curr_scope) && DeeScope_Check(curr_scope));
 curr_scope = DeeScope_WEAK_ROOT(curr_scope);
 flags = DEE_XAST_VARAST_FLAG_NONE;
 if (DeeScope_WEAK_ROOT(localvar->lv_scope) != curr_scope) {
  curr_scope->sc_flags |= DEE_SCOPE_FLAG_FOUND_REFS; // Found a reference
  flags |= DEE_XAST_VARAST_FLAG_REF;
 }
 if DEE_UNLIKELY((result = _DeeXAst_NewUnsafe(DEE_XASTKIND_VAR,tk)) != NULL) {
  Dee_INCREF(result->ast_var.vs_var = localvar);
  DeeLocalVar_ADD_USE(localvar); 
  result->ast_var.vs_flags = flags;
 }
 return result;
}

#ifdef DEE_DEBUG
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewLocalVar(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLocalVarObject *localvar,
 DEE_A_INOUT DeeScopeObject *curr_scope)
#else
DEE_A_RET_EXCEPT_REF DeeXAstObject *_DeeXAst_NewLocalVar(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLocalVarObject *localvar)
#endif
{
 DeeXAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(localvar) && DeeLocalVar_Check(localvar));
#ifdef DEE_DEBUG
 DEE_ASSERT(DeeObject_Check(curr_scope) && DeeScope_Check(curr_scope));
 DEE_ASSERTF(DeeScope_SameWeakScope((DeeObject *)localvar->lv_scope,(DeeObject *)curr_scope),
             "non-local (aka. reference) variable given in 'DeeXAst_NewLocalVar'");
#endif
 if DEE_UNLIKELY((result = _DeeXAst_NewUnsafe(DEE_XASTKIND_VAR,tk)) != NULL) {
  Dee_INCREF(result->ast_var.vs_var = localvar);
  DeeLocalVar_ADD_USE(localvar); 
  result->ast_var.vs_flags = DEE_XAST_VARAST_FLAG_NONE;
 }
 return result;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewReferenceVar(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLocalVarObject *localvar,
 DEE_A_INOUT DeeScopeObject *curr_scope) {
 DeeXAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(localvar) && DeeLocalVar_Check(localvar));
 DEE_ASSERT(DeeObject_Check(curr_scope) && DeeScope_Check(curr_scope));
 DEE_ASSERTF(!DeeScope_SameWeakScope((DeeObject *)localvar->lv_scope,(DeeObject *)curr_scope),
             "local (aka. non-reference) variable given in 'DeeXAst_NewReferenceVar'");
 (DeeScope_WEAK_ROOT(localvar->lv_scope))->sc_flags |= DEE_SCOPE_FLAG_FOUND_REFS;
 if DEE_UNLIKELY((result = _DeeXAst_NewUnsafe(DEE_XASTKIND_VAR,tk)) != NULL) {
  Dee_INCREF(result->ast_var.vs_var = localvar);
  DeeLocalVar_ADD_USE(localvar); 
  result->ast_var.vs_flags = DEE_XAST_VARAST_FLAG_REF;
 }
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeXAst_DoThrowInvalidOperatorError(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN DeeStringObject *descr, DEE_A_IN DeeTypeObject const *rt_error) {
 return DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP,
  (DeeObject *)lexer,(DeeObject *)tk,
  "The expression %q will cause an %q at runtime",
  DeeString_STR(descr),DeeType_NAME(rt_error));
}
DEE_A_RET_EXCEPT(-1) int DeeXAst_ThrowInvalidUnaryOperatorError(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_INOUT DeeXAstObject *ast_a,
 DEE_A_IN DeeTypeObject const *rt_error) {
 DeeStringObject *descr; int result;
 if DEE_UNLIKELY((descr = (DeeStringObject *)DeeXAst_UnaryDescrStr(kind,ast_a)) == NULL) return -1;
 result = DeeXAst_DoThrowInvalidOperatorError(tk,lexer,descr,rt_error);
 Dee_DECREF(descr);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeXAst_ThrowInvalidBinaryOperatorError(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_INOUT DeeXAstObject *ast_a,
 DEE_A_INOUT DeeXAstObject *ast_b, DEE_A_IN DeeTypeObject const *rt_error) {
 DeeStringObject *descr; int result;
 if DEE_UNLIKELY((descr = (DeeStringObject *)DeeXAst_BinaryDescrStr(kind,ast_a,ast_b)) == NULL) return -1;
 result = DeeXAst_DoThrowInvalidOperatorError(tk,lexer,descr,rt_error);
 Dee_DECREF(descr);
 return result;
}
DEE_A_RET_EXCEPT(-1) int DeeXAst_ThrowInvalidTrinaryOperatorError(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_INOUT DeeXAstObject *ast_a,
 DEE_A_INOUT DeeXAstObject *ast_b, DEE_A_INOUT DeeXAstObject *ast_c,
 DEE_A_IN DeeTypeObject const *rt_error) {
 DeeStringObject *descr; int result;
 if DEE_UNLIKELY((descr = (DeeStringObject *)DeeXAst_TrinaryDescrStr(kind,ast_a,ast_b,ast_c)) == NULL) return -1;
 result = DeeXAst_DoThrowInvalidOperatorError(tk,lexer,descr,rt_error);
 Dee_DECREF(descr);
 return result;
}


DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewCast(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *cast_ob,
 DEE_A_INOUT DeeXAstObject *result_type) {
 DeeXAstObject *result,*args_ast;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(cast_ob) && DeeXAst_Check(cast_ob));
 DEE_ASSERT(DeeObject_Check(result_type) && DeeXAst_Check(result_type));
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_MERGE_OPERATORS)!=0 &&
     result_type->ast_kind == DEE_XASTKIND_CONST &&
     DeeType_Check(result_type->ast_const.c_const) &&
     DeeXAst_PredictType(cast_ob) == (DeeTypeObject const *)
     result_type->ast_const.c_const) { Dee_INCREF(cast_ob); return cast_ob; }
 if DEE_UNLIKELY((args_ast = DeeXAst_NewTuple(tk,lexer,parser_flags,1,&cast_ob)) == NULL) return NULL;
 result = DeeXAst_NewCall(tk,lexer,parser_flags,result_type,args_ast);
 Dee_DECREF(args_ast);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewCastC(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *cast_ob,
 DEE_A_INOUT DeeTypeObject const *result_type) {
 DeeXAstObject *result,*args_ast,*type_ast;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(cast_ob) && DeeXAst_Check(cast_ob));
 DEE_ASSERT(DeeObject_Check(result_type) && DeeType_Check(result_type));
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_MERGE_OPERATORS)!=0 &&
     DeeXAst_PredictType(cast_ob) == result_type)
 { Dee_INCREF(cast_ob); return cast_ob; }
 if DEE_UNLIKELY((args_ast = DeeXAst_NewTuple(tk,lexer,parser_flags,1,&cast_ob)) == NULL) return NULL;
 if DEE_UNLIKELY((type_ast = DeeXAst_NewConst(tk,(DeeObject *)result_type)) == NULL) { Dee_DECREF(args_ast); return NULL; }
 result = DeeXAst_NewCall(tk,lexer,parser_flags,type_ast,args_ast);
 Dee_DECREF(type_ast);
 Dee_DECREF(args_ast);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *_DeeXAst_NewUnaryInplaceVar(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeTokenObject *var_tk, DEE_A_INOUT DeeLocalVarObject *local_var) {
 DeeXAstObject *result;
 DEE_ASSERT(DEE_XASTKIND_ISUNARYVAR(kind));
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(var_tk) && DeeToken_Check(var_tk));
 DEE_ASSERT(DeeObject_Check(local_var) && DeeLocalVar_Check(local_var));
 if DEE_LIKELY((result = _DeeXAst_NewUnsafe(kind,tk)) != NULL) {
  Dee_INCREF(result->ast_unary_var.uiv_var = local_var);
  Dee_INCREF(result->ast_unary_var.uiv_tok = var_tk);
  DeeLocalVar_ADD_USE(local_var);
  DeeLocalVar_ADD_INIT(local_var);
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *_DeeXAst_NewBinaryInplaceVar(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeTokenObject *var_tk, DEE_A_INOUT DeeLocalVarObject *local_var,
 DEE_A_INOUT DeeXAstObject *ast_arg) {
 DeeXAstObject *result;
 DEE_ASSERT(DEE_XASTKIND_ISBINARYVAR(kind));
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(var_tk) && DeeToken_Check(var_tk));
 DEE_ASSERT(DeeObject_Check(local_var) && DeeLocalVar_Check(local_var));
 DEE_ASSERT(DeeObject_Check(ast_arg) && DeeXAst_Check(ast_arg));
 if DEE_LIKELY((result = _DeeXAst_NewUnsafe(kind,tk)) != NULL) {
  Dee_INCREF(result->ast_binary_var.biv_var = local_var);
  Dee_INCREF(result->ast_binary_var.biv_tok = var_tk);
  Dee_INCREF(result->ast_binary_var.biv_arg = ast_arg);
  DeeLocalVar_ADD_USE(local_var);
  DeeLocalVar_ADD_INIT(local_var);
 }
 return result;
}







DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewSequenceFromInheritedAstv(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_IN size_t astc, DEE_A_IN_R(astc) DeeXAstObject **astv) {
 DeeXAstObject *result,*optimized_result;
 DEE_ASSERTF(kind == DEE_XASTKIND_TUPLE || kind == DEE_XASTKIND_LIST ||
             kind == DEE_XASTKIND_SET,"Given xast kind isn't a sequence");
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(astc != 0);
 DEE_ASSERT(astv != NULL);
#ifdef DEE_DEBUG
 {DeeXAstObject **iter,**end;
  end = (iter = astv)+astc;
  while (iter != end) { DEE_ASSERT(DeeObject_Check(*iter) && DeeXAst_Check(*iter)); ++iter; }}
#endif
 if DEE_UNLIKELY((result = _DeeXAst_NewUnsafe(kind,tk)) == NULL) return NULL;
 result->ast_sequence.s_elemv = astv;
 result->ast_sequence.s_elemc = astc;
 optimized_result = DeeXAst_OptimizeSequence(result,lexer,parser_flags);
 if (!optimized_result) { // Don't inherit on failure
  result->ast_kind = DEE_XASTKIND_CONST;
  result->ast_const.c_const = DeeNone_New();
 }
 Dee_DECREF(result);
 return optimized_result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewEmptySequence(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk) {
 DeeObject *const_value; DeeXAstObject *result;
 DEE_ASSERTF(kind == DEE_XASTKIND_TUPLE || kind == DEE_XASTKIND_LIST ||
             kind == DEE_XASTKIND_SET,"Given xast kind isn't a sequence");
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 switch (kind) {
  case DEE_XASTKIND_TUPLE: Dee_INCREF(const_value = Dee_EmptyTuple); break;
  case DEE_XASTKIND_LIST: if DEE_UNLIKELY((const_value = DeeList_NewEmpty()) == NULL) return NULL; break;
  case DEE_XASTKIND_SET: if DEE_UNLIKELY((const_value = DeeSet_New()) == NULL) return NULL; break;
  default: DEE_BUILTIN_UNREACHABLE();
 }
 result = DeeXAst_NewConst(tk,const_value);
 Dee_DECREF(const_value);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewSequence(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_IN size_t astc, DEE_A_IN_R(astc) DeeXAstObject *const *astv) {
 DeeXAstObject *result;
 DeeXAstObject **my_astv,**iter,**end;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
#ifdef DEE_DEBUG
 end = (iter = (DeeXAstObject **)astv)+astc;
 while (iter != end) { DEE_ASSERT(DeeObject_Check(*iter) && DeeXAst_Check(*iter)); ++iter; }
#endif
 if (!astc) return DeeXAst_NewEmptySequence(kind,tk);
 while DEE_UNLIKELY((my_astv = (DeeXAstObject **)malloc_nz(astc*sizeof(DeeXAstObject *))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 end = (iter = my_astv)+astc;
 do Dee_INCREF(*iter++ = *astv++);
 while DEE_UNLIKELY(iter != end);
 result = DeeXAst_NewSequenceFromInheritedAstv(kind,tk,lexer,parser_flags,
                                               astc,my_astv);
 if DEE_UNLIKELY(!result) {
  end = (iter = my_astv)+astc;
  while DEE_LIKELY(iter != end) Dee_DECREF(*iter++);
  free(my_astv);
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewDictFromInheritedEntryv(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_IN size_t entryc,
 DEE_A_IN_R(entryc) struct DeeXAstDictEntry *entryv) {
 DeeObject *const_result;
 DeeXAstObject *result; struct DeeXAstDictEntry *iter,*end;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(entryc != 0);
 DEE_ASSERT(entryv != NULL);
#ifdef DEE_DEBUG
 end = (iter = (struct DeeXAstDictEntry *)entryv)+entryc;
 while DEE_LIKELY(iter != end) {
  DEE_ASSERT(DeeObject_Check(iter->de_key) && DeeXAst_Check(iter->de_key));
  DEE_ASSERT(DeeObject_Check(iter->de_item) && DeeXAst_Check(iter->de_item));
  ++iter;
 }
#endif
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_SEQUENCE)!=0) {
  end = (iter = (struct DeeXAstDictEntry *)entryv)+entryc;
  // Make sure all dict elements are constant
  while DEE_LIKELY(iter != end) {
   if (iter->de_key->ast_kind != DEE_XASTKIND_CONST ||
       iter->de_item->ast_kind != DEE_XASTKIND_CONST) goto normal_ast;
   ++iter;
  }
  // Yes! We can turn this one into a constant dict
  if DEE_UNLIKELY((const_result = DeeDict_New()) == NULL) return NULL;
  iter = (struct DeeXAstDictEntry *)entryv;
  while DEE_LIKELY(iter != end) {
   if DEE_UNLIKELY(DeeDict_SetItem(const_result,
    iter->de_key->ast_const.c_const,
    iter->de_item->ast_const.c_const) != 0) {
    Dee_DECREF(const_result);
    return NULL;
   }
   ++iter;
  }
  result = DeeXAst_NewConst(tk,const_result);
  Dee_DECREF(const_result);
  if DEE_UNLIKELY(!result) return NULL;
  goto unused_entries;
 }
normal_ast:
 if DEE_UNLIKELY((result = _DeeXAst_NewUnsafe(DEE_XASTKIND_DICT,tk)) != NULL) {
  result->ast_dict.d_elemc = entryc;
  result->ast_dict.d_elemv = entryv;
 }
 return result;
unused_entries:
 end = (iter = (struct DeeXAstDictEntry *)entryv)+entryc;
 while DEE_LIKELY(iter != end) { Dee_DECREF(iter->de_key); Dee_DECREF(iter->de_item); ++iter; }
 free(entryv);
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewDict(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_IN size_t entryc,
 DEE_A_IN_R(entryc) struct DeeXAstDictEntry const *entryv) {
 struct DeeXAstDictEntry *my_entryv;
 DeeXAstObject *result; struct DeeXAstDictEntry *iter,*end;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(!entryc || entryv);
 if (!entryc) { // Empty dicts are represented by constants
  DeeObject *const_dict;
  if DEE_UNLIKELY((const_dict = DeeDict_New()) == NULL) return NULL;
  result = DeeXAst_NewConst(tk,const_dict);
  Dee_DECREF(const_dict);
  return result;
 }
 while DEE_UNLIKELY((my_entryv = (struct DeeXAstDictEntry *)malloc_nz(
  entryc*sizeof(struct DeeXAstDictEntry))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 end = (iter = my_entryv)+entryc;
 do {
  DEE_ASSERT(DeeObject_Check(entryv->de_key) && DeeXAst_Check(entryv->de_key));
  DEE_ASSERT(DeeObject_Check(entryv->de_item) && DeeXAst_Check(entryv->de_item));
  Dee_INCREF(iter->de_key = entryv->de_key);
  Dee_INCREF(iter->de_item = entryv->de_item);
  ++iter,++entryv;
 } while DEE_LIKELY(iter != end);
 result = DeeXAst_NewDictFromInheritedEntryv(tk,lexer,parser_flags,entryc,my_entryv);
 if DEE_UNLIKELY(!result) {
  end = (iter = my_entryv)+entryc;
  while (iter != end) { Dee_DECREF(iter->de_key); Dee_DECREF(iter->de_item); ++iter; }
  free(my_entryv);
 }
 return result;
}



DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewIfFromIfSAst(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT struct DeeSAstObject *if_sast,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags) {
 DeeSAstObject *block,*sast;
 DeeXAstObject *result,*succ_expr,*fail_expr;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(if_sast) && DeeSAst_Check(if_sast));
 DEE_ASSERT(if_sast->ast_kind == DEE_SASTKIND_IF);
 DEE_ASSERT((if_sast->ast_if.if_succ_block != NULL) == (if_sast->ast_if.if_succ_scope != NULL));
 DEE_ASSERT((if_sast->ast_if.if_fail_block != NULL) == (if_sast->ast_if.if_fail_scope != NULL));
 // Convert the success branch
 if (if_sast->ast_if.if_succ_block) {
  block = DeeSAst_NewBlock(if_sast->ast_common.ast_token,
                           DeeParserLabelRefList_Empty,lexer,parser_flags,
                           if_sast->ast_if.if_succ_scope,1,
                           &if_sast->ast_if.if_succ_block);
  if DEE_UNLIKELY(!block) return NULL;
  succ_expr = DeeXAst_NewStatement(block->ast_common.ast_token,lexer,parser_flags,block);
  Dee_DECREF(block);
  if DEE_UNLIKELY(!succ_expr) return NULL;
 } else {
  if DEE_UNLIKELY((succ_expr = DeeXAst_NewConst(if_sast->ast_common.ast_token,Dee_None)) == NULL) return NULL;
 }
 // Convert the failure branch
 if (if_sast->ast_if.if_fail_block) {
  block = DeeSAst_NewBlock(if_sast->ast_common.ast_token,
                           DeeParserLabelRefList_Empty,lexer,parser_flags,
                           if_sast->ast_if.if_fail_scope,1,
                           &if_sast->ast_if.if_fail_block);
  if DEE_UNLIKELY(!block) {err_if_succ_expr: Dee_DECREF(succ_expr); return NULL; }
  fail_expr = DeeXAst_NewStatement(block->ast_common.ast_token,lexer,parser_flags,block);
  Dee_DECREF(block);
  if DEE_UNLIKELY(!fail_expr) goto err_if_succ_expr;
 } else fail_expr = NULL;

 result = DeeXAst_NewIf(tk,lexer,parser_flags,if_sast->ast_if.if_cond,succ_expr,fail_expr);
 Dee_XDECREF(fail_expr);
 Dee_DECREF(succ_expr);
 if DEE_UNLIKELY(!result) return NULL;
 // Now wrap everything into the cond-scope
 sast = DeeSAst_NewExpression(if_sast->ast_common.ast_token,DeeParserLabelRefList_Empty,
                              lexer,parser_flags,result);
 Dee_DECREF(result);
 if DEE_UNLIKELY(!sast) return NULL;
 block = DeeSAst_NewBlock(if_sast->ast_common.ast_token,&if_sast->ast_common.ast_labels,
                          lexer,parser_flags,if_sast->ast_if.if_cond_scope,1,&sast);
 Dee_DECREF(sast);
 if DEE_UNLIKELY(!block) return NULL;
 result = DeeXAst_NewStatement(tk,lexer,parser_flags,block);
 Dee_DECREF(block);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewIfConstFromIfConstSAst(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT_OPT DeeSAstObject *succ_block, DEE_A_INOUT_OPT DeeScopeObject *succ_scope,
 DEE_A_INOUT_OPT DeeSAstObject *fail_block, DEE_A_INOUT_OPT DeeScopeObject *fail_scope,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags) {
 DeeXAstObject *result,*wrapped_succ,*wrapped_fail; DeeSAstObject *temp_ast;
 DEE_ASSERTF(kind == DEE_XASTKIND_IFTRUE || kind == DEE_XASTKIND_IFFALSE,"Invalid ast kind");
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(!succ_block || (DeeObject_Check(succ_block) && DeeSAst_Check(succ_block)));
 DEE_ASSERT(!fail_block || (DeeObject_Check(fail_block) && DeeSAst_Check(fail_block)));
 DEE_ASSERT(!succ_scope || (DeeObject_Check(succ_scope) && DeeScope_Check(succ_scope)));
 DEE_ASSERT(!fail_scope || (DeeObject_Check(fail_scope) && DeeScope_Check(fail_scope)));
 DEE_ASSERT((succ_block != NULL) == (succ_scope != NULL));
 DEE_ASSERT((fail_block != NULL) == (fail_scope != NULL));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 if (succ_block) {
  temp_ast = DeeSAst_NewBlock(succ_block->ast_common.ast_token,
                              DeeParserLabelRefList_Empty,lexer,parser_flags,
                              succ_scope,1,&succ_block);
  if DEE_UNLIKELY(!temp_ast) return NULL;
  wrapped_succ = DeeXAst_NewStatement(temp_ast->ast_common.ast_token,
                                      lexer,parser_flags,temp_ast);
  Dee_DECREF(temp_ast);
  if DEE_UNLIKELY(!wrapped_succ) return NULL;
 } else wrapped_succ = NULL;
 if (fail_block) {
  temp_ast = DeeSAst_NewBlock(fail_block->ast_common.ast_token,
                              DeeParserLabelRefList_Empty,lexer,parser_flags,
                              fail_scope,1,&fail_block);
  if DEE_UNLIKELY(!temp_ast) {err_wrapped_succ: Dee_XDECREF(wrapped_succ); return NULL; }
  wrapped_fail = DeeXAst_NewStatement(fail_block->ast_common.ast_token,
                                      lexer,parser_flags,temp_ast);
  Dee_DECREF(temp_ast);
  if DEE_UNLIKELY(!wrapped_fail) goto err_wrapped_succ;
 } else wrapped_fail = NULL;
 result = DeeXAst_NewIfConst(kind,tk,lexer,parser_flags,wrapped_succ,wrapped_fail);
 Dee_XDECREF(wrapped_fail);
 Dee_XDECREF(wrapped_succ);
 return result;
}


DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewStatement(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT struct DeeSAstObject *statement) {
 DeeXAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(statement) && DeeSAst_Check(statement));
 (void)lexer,parser_flags;
 // NOTE: These optimizations assume that 'statement' has already been
 //       block-optimized (s.a.: 'DeeSAst_NewBlockFromInheritedStatements')
 // Simplify some statements
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_MERGE_OPERATORS)!=0 &&
     DeeParserLabelRefList_EMPTY(&statement->ast_common.ast_labels)) {
  switch (statement->ast_kind) {

   case DEE_SASTKIND_EMPTY: // Empty statement --> none constant
    return DeeXAst_NewConst(tk,Dee_None);

   case DEE_SASTKIND_IF:
    // Optimize: '({ if (x) { y; } else { z; } })' --> 'x ? y : z'
    return DeeXAst_NewIfFromIfSAst(tk,statement,lexer,parser_flags);

   case DEE_SASTKIND_EXPRESSION:
/*return_statement_expression:*/
    // Optimize '({ 42; })' --> '42'
    Dee_INCREF(statement->ast_expression.x_ast);
    return statement->ast_expression.x_ast;

   default: break;
  }
 }

 if DEE_LIKELY((result = _DeeXAst_NewUnsafe(DEE_XASTKIND_STATEMENT,tk)) != NULL) {
  Dee_INCREF(result->ast_statement.s_stmt = statement);
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewModule(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeScopeObject *module_) {
 DeeXAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(module_) && DeeScope_Check(module_));
 if DEE_LIKELY((result = _DeeXAst_NewUnsafe(DEE_XASTKIND_MODULE,tk)) != NULL) {
  Dee_INCREF(result->ast_module.m_module = module_);
 }
 return result;
}
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewBuiltinBound(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeTokenObject *var_token,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeLocalVarObject *var_object, DEE_A_INOUT DeeScopeObject *curr_scope) {
 DeeXAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(var_token) && DeeToken_Check(var_token));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(var_object) && DeeLocalVar_Check(var_object));
 DEE_ASSERT(DeeObject_Check(curr_scope) && DeeScope_Check(curr_scope));
 (void)parser_flags;
 if DEE_UNLIKELY(!DeeScope_SameWeakScope((DeeObject *)var_object->lv_scope,(DeeObject *)curr_scope)) {
  if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_BUILTIN_BOUND_FROM_DIFFERENT_STRONG_SCOPE,
   (DeeObject *)lexer,(DeeObject *)var_token,
   "The given variable in '__builtin_bound' is from another strong scope, "
   "thus preventing detecting at runtime") != 0) return NULL;
  return DeeXAst_NewConst(tk,DeeBool_Const(var_object->lv_init != 0));
 }
 switch (DeeLocalVar_KIND(var_object)) {
  case DEE_LOCALVAR_KIND_STATIC:
   if DEE_UNLIKELY(DeeError_CompilerError(
    DEE_WARNING_EXPECTED_NONSTATIC_AFTER_BUILTIN_BOUND,
    (DeeObject *)lexer,(DeeObject *)var_token,
    "The given variable in '__builtin_bound' is static and therefor always found"
    ) != 0) return NULL;
   return DeeXAst_NewConst(tk,Dee_True);
  case DEE_LOCALVAR_KIND_PARAM:
   if DEE_UNLIKELY(DeeError_CompilerError(
    DEE_WARNING_EXPECTED_NONARGUMENT_AFTER_BUILTIN_BOUND,
    (DeeObject *)lexer,(DeeObject *)var_token,
    "The given variable in '__builtin_bound' is an argument and therefor always found"
    ) != 0) return NULL;
   return DeeXAst_NewConst(tk,Dee_True);
  case DEE_LOCALVAR_KIND_THIS:
   if DEE_UNLIKELY(DeeError_CompilerError(
    DEE_WARNING_EXPECTED_NONTHIS_AFTER_BUILTIN_BOUND,
    (DeeObject *)lexer,(DeeObject *)var_token,
    "The given variable in '__builtin_bound' is the this-variable "
    "and therefor always found in a [[thiscall]]"
    ) != 0) return NULL;
   return DeeXAst_NewConst(tk,DeeBool_Const((
    DeeScope_WEAK_ROOT(var_object->lv_scope)->sc_flags&
    DEE_SCOPE_FLAG_FOUND_THIS)!=0));
  default: break;
 }
 if DEE_UNLIKELY((result = _DeeXAst_NewUnsafe(DEE_XASTKIND_BUILTIN_BOUND,tk)) != NULL) {
  Dee_INCREF(result->ast_builtin_bound.b_token = var_token);
  Dee_INCREF(result->ast_builtin_bound.b_var = var_object);
 }
 return result;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewBuiltinExpect(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeXAstObject *ast_value,
 DEE_A_INOUT DeeObject *expected) {
 DeeXAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(ast_value) && DeeXAst_Check(ast_value));
 DEE_ASSERT(DeeObject_Check(expected));
 if DEE_UNLIKELY((result = _DeeXAst_NewUnsafe(DEE_XASTKIND_BUILTIN_EXPECT,tk)) != NULL) {
  Dee_INCREF(result->ast_builtin_expect.e_value = ast_value);
  Dee_INCREF(result->ast_builtin_expect.e_expect = expected);
 }
 return result;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */


DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewDelVar(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeTokenObject *var_token,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeLocalVarObject *del_var, DEE_A_INOUT DeeScopeObject *curr_scope) {
 DeeXAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(var_token) && DeeToken_Check(var_token));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(del_var) && DeeLocalVar_Check(del_var));
 DEE_ASSERT(DeeObject_Check(curr_scope) && DeeScope_Check(curr_scope));
 (void)lexer,parser_flags;
 if DEE_UNLIKELY(!DeeScope_SameWeakScope((DeeObject *)del_var->lv_scope,(DeeObject *)curr_scope)) {
  if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_CANT_DEL_VARIABLE_FROM_DIFFERENT_STRONG_SCOPE,
   (DeeObject *)lexer,(DeeObject *)var_token,
   "Can't del variable from different strong scope") != 0) return NULL;
  return DeeXAst_NewConst(tk,Dee_None);
 }
 if (del_var->lv_scope == curr_scope) {
  _DeeScope_DelName(del_var->lv_scope,del_var->lv_name);
 }
 switch (DeeLocalVar_KIND(del_var)) {
  case DEE_LOCALVAR_KIND_STACK:
   if DEE_UNLIKELY(del_var->lv_scope != curr_scope) {
    if DEE_UNLIKELY(DeeError_CompilerError(
     DEE_WARNING_CANT_DEL_STACK_VARIABLE_FROM_DIFFERENT_STRONG_SCOPE,
     (DeeObject *)lexer,(DeeObject *)var_token,
     "Can't del __stack variable from different scope") != 0) return NULL;
    break;
   }
   DEE_ATTRIBUTE_FALLTHROUGH
  case DEE_LOCALVAR_KIND_LOCAL:
  case DEE_LOCALVAR_KIND_RETVAL:
   if DEE_LIKELY((result = _DeeXAst_NewUnsafe(DEE_XASTKIND_DEL_VAR,tk)) != NULL) {
    Dee_INCREF(result->ast_delvar.d_token = var_token);
    Dee_INCREF(result->ast_delvar.d_var = del_var);
   }
   return result;
  default: break;
 }
 return DeeXAst_NewConst(tk,Dee_None);
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewDel(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *del_ast,
 DEE_A_INOUT DeeScopeObject *curr_scope) {
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(del_ast) && DeeXAst_Check(del_ast));
 DEE_ASSERT(DeeObject_Check(curr_scope) && DeeScope_Check(curr_scope));
 switch (del_ast->ast_kind) {
  case DEE_XASTKIND_VAR:
   return DeeXAst_NewDelVar(del_ast->ast_common.ast_token,del_ast->ast_common.ast_token,
                            lexer,parser_flags,del_ast->ast_var.vs_var,curr_scope);
  case DEE_XASTKIND_ATTR_GET:
   return DeeXAst_NewAttrDel(del_ast->ast_common.ast_token,
                             lexer,parser_flags,del_ast->ast_operator.op_a,
                             del_ast->ast_operator.op_b);
  case DEE_XASTKIND_ATTR_GET_C:
   return DeeXAst_NewAttrDelC(del_ast->ast_common.ast_token,
                              lexer,parser_flags,del_ast->ast_attr_get_c.ac_object,
                              (DeeObject *)del_ast->ast_attr_get_c.ac_name);
  case DEE_XASTKIND_SEQ_GET:
   return DeeXAst_NewSeqDel(del_ast->ast_common.ast_token,
                            lexer,parser_flags,del_ast->ast_operator.op_a,
                            del_ast->ast_operator.op_b);
  case DEE_XASTKIND_SEQ_RANGE_GET:
   return DeeXAst_NewSeqRangeDel(del_ast->ast_common.ast_token,lexer,parser_flags,
                                 del_ast->ast_seq_range_get.sr_seq,
                                 del_ast->ast_seq_range_get.sr_begin,
                                 del_ast->ast_seq_range_get.sr_end);
  default:
   if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_INVALID_EXPRSSION_FOR_DEL,
    (DeeObject *)lexer,(DeeObject *)del_ast->ast_common.ast_token,
    "'del' can't be used with %r",del_ast) != 0) return NULL;
   break;
 }
 {
  DeeXAstObject *result,*none_result;
  if DEE_UNLIKELY((none_result = DeeXAst_NewConst(tk,Dee_None)) == NULL) return NULL;
  result = DeeXAst_NewBuiltinUnused(tk,lexer,parser_flags,del_ast,none_result);
  Dee_DECREF(none_result);
  return result;
 }
}


DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewIf(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *ast_cond,
 DEE_A_INOUT_OPT DeeXAstObject *ast_succ, DEE_A_INOUT_OPT DeeXAstObject *ast_fail) {
 DeeObject *cond_copy; int temp;
 DeeXAstObject *result; DeeTypeObject *rt_error;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(ast_cond) && DeeXAst_Check(ast_cond));
 DEE_ASSERT(!ast_succ || (DeeObject_Check(ast_succ) && DeeXAst_Check(ast_succ)));
 DEE_ASSERT(!ast_fail || (DeeObject_Check(ast_fail) && DeeXAst_Check(ast_fail)));
 // Optimize away constant none-expressions (will come back during compilation)
 // NOTE: Can't remove 'succ', as a NULL success-condition has special meaning
 //if (ast_succ && ast_succ->ast_kind == DEE_XASTKIND_CONST &&
 //    DeeNone_Check(ast_succ->ast_const.c_const)) ast_succ = NULL;
 if (ast_fail && ast_fail->ast_kind == DEE_XASTKIND_CONST &&
     DeeNone_Check(ast_fail->ast_const.c_const)) ast_fail = NULL;
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH) != 0 &&
     ast_cond->ast_kind == DEE_XASTKIND_CONST) {
  if DEE_UNLIKELY((cond_copy = DeeObject_DeepCopy(ast_cond->ast_const.c_const)) == NULL) {
   // Ignore errors in copy, as the copy-operation wouldn't be performed at runtime
   DeeError_Handled();
   goto normal_ast;
  }
  temp = DeeObject_Bool(cond_copy);
  Dee_DECREF(cond_copy);
  if DEE_UNLIKELY(temp < 0) {
   if DEE_UNLIKELY((rt_error = _DeeError_HandleAndReturnType()) != NULL) {
    int temp = DeeXAst_ThrowInvalidUnaryOperatorError(
     DEE_XASTKIND_BOOL,tk,lexer,ast_cond,rt_error);
    Dee_DECREF(rt_error);
    if DEE_UNLIKELY(temp != 0) return NULL;
   }
   goto normal_ast;
  }
  if (temp) {
   // 'true ? foo() : bar()' --> '/* constant */true ? foo() : bar()'
   VLOG_PARSER(tk,"Detected dead fail branch (compiling as 'constant true')\n");
   if (!ast_succ) ast_succ = ast_cond;
   if (!ast_fail || ast_fail->ast_kind == DEE_XASTKIND_CONST) { Dee_INCREF(ast_succ); return ast_succ; }
   return DeeXAst_NewIfTrue(tk,lexer,parser_flags,ast_succ,ast_fail);
  }
  // 'false ? foo() : bar()' --> '/* constant */false ? foo() : bar()'
  VLOG_PARSER(tk,"Detected dead succ branch in (compiling as 'constant false')\n");
  if (!ast_succ || ast_succ->ast_kind == DEE_XASTKIND_CONST) {
   if (!ast_fail) return DeeXAst_NewConst(tk,Dee_None);
   Dee_INCREF(ast_fail);
   return ast_fail;
  }
  return DeeXAst_NewIfFalse(tk,lexer,parser_flags,ast_succ,ast_fail);
 }
normal_ast:
 if DEE_LIKELY((result = _DeeXAst_NewUnsafe(DEE_XASTKIND_IF,tk)) != NULL) {
  Dee_INCREF(result->ast_if.if_cond = ast_cond);
  Dee_XINCREF(result->ast_if.if_succ = ast_succ);
  Dee_XINCREF(result->ast_if.if_fail = ast_fail);
 }
 return result;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewIfConst(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT_OPT DeeXAstObject *ast_succ, DEE_A_INOUT_OPT DeeXAstObject *ast_fail) {
 DeeXAstObject *result;
 DEE_ASSERTF(kind == DEE_XASTKIND_IFTRUE || kind == DEE_XASTKIND_IFFALSE,
             "Given xast kind isn't allowed");
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(!ast_succ || (DeeObject_Check(ast_succ) && DeeXAst_Check(ast_succ)));
 DEE_ASSERT(!ast_fail || (DeeObject_Check(ast_fail) && DeeXAst_Check(ast_fail)));
 (void)lexer,parser_flags;
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH)!=0) {
  if (!ast_succ && !ast_fail) return DeeXAst_NewConst(tk,Dee_None);
  if (kind == DEE_XASTKIND_IFTRUE && (!ast_fail || ast_fail->ast_kind == DEE_XASTKIND_CONST)) { Dee_INCREF(ast_succ); return ast_succ; }
  if (kind == DEE_XASTKIND_IFFALSE && (!ast_succ || ast_succ->ast_kind == DEE_XASTKIND_CONST)) { Dee_INCREF(ast_fail); return ast_fail; }
 }
 if DEE_LIKELY((result = _DeeXAst_NewUnsafe(kind,tk)) != NULL) {
  Dee_XINCREF(result->ast_ifconst.ic_succ = ast_succ);
  Dee_XINCREF(result->ast_ifconst.ic_fail = ast_fail);
 }
 return result;
}



DEE_A_RET_EXCEPT_REF DeeXAstObject *_DeeXAst_NewAttrGetHasOrDelC(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeXAstObject *ast_object, DEE_A_INOUT_OBJECT(DeeStringObject) *attr_name) {
 DeeXAstObject *result; DeeObject *const_copy,*attr_result; int temp;
 DEE_ASSERTF(kind == DEE_XASTKIND_ATTR_GET_C || kind == DEE_XASTKIND_ATTR_HAS_C ||
             kind == DEE_XASTKIND_ATTR_DEL_C,"Given xast kind isn't allowed");
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(ast_object) && DeeXAst_Check(ast_object));
 DEE_ASSERT(DeeObject_Check(attr_name) && DeeString_Check(attr_name));
 if ((parser_flags&(
  DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS|
  DEE_PARSER_FLAG_NO_OPTIMIZE_SUBSCRIPT)
  ) == DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS &&
  ast_object->ast_kind == DEE_XASTKIND_CONST) {
  if (kind == DEE_XASTKIND_ATTR_DEL_C) goto normal_ast;
  if DEE_UNLIKELY((const_copy = DeeObject_DeepCopy(ast_object->ast_const.c_const)) == NULL) {
   // Ignore errors in copy, as the copy-operation wouldn't be performed at runtime
   DeeError_Handled();
   goto normal_ast;
  }
  switch (kind) {
   case DEE_XASTKIND_ATTR_GET_C:
    attr_result = DeeObject_GetAttrConst(const_copy,attr_name);
    if DEE_UNLIKELY(!attr_result) {
     temp = DeeObject_HasAttr(const_copy,attr_name);
     Dee_DECREF(const_copy);
     if DEE_UNLIKELY(temp < 0) { DeeError_Handled(); goto handle_attr_error; }
     if DEE_UNLIKELY(!temp) goto handle_attr_error; // If this getattr failed too, display a warning
     DeeError_Handled(); // The attribute only exists as non-constant
     goto normal_ast;
    }
    break;
   case DEE_XASTKIND_ATTR_HAS_C:
    if DEE_UNLIKELY((temp = DeeObject_HasAttr(const_copy,attr_name)) < 0) {
     attr_result = NULL;
    } else attr_result = DeeBool_New(temp);
    break;
#if 0 /*< NO! */
   case DEE_XASTKIND_ATTR_DEL_C:
    // todo: Warn about use on constant
    if DEE_UNLIKELY(DeeObject_DelAttr(const_copy,attr_name) != 0) {
     attr_result = NULL;
    } else attr_result = DeeNone_New();
    break;
#endif
   default:
#if 1
    DEE_BUILTIN_UNREACHABLE();
#else
    Dee_DECREF(const_copy);
    goto normal_ast;
#endif
  }
  Dee_DECREF(const_copy);
//handle_attr_result:
  if DEE_UNLIKELY(!attr_result) {
   DeeTypeObject *rt_error;
handle_attr_error:
   rt_error = _DeeError_HandleAndReturnType();
   if DEE_LIKELY(rt_error) switch (kind) {
    case DEE_XASTKIND_ATTR_GET_C:
     if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP,
      (DeeObject *)lexer,(DeeObject *)tk,"The expression '%r.%s' will cause an %K at runtime",
      ast_object,DeeString_STR(attr_name),rt_error) != 0) return NULL;
     break;
    case DEE_XASTKIND_ATTR_HAS_C:
     if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP,
      (DeeObject *)lexer,(DeeObject *)tk,"The expression '(%r).operator __hasattr__(%q)' will cause an %K at runtime",
      ast_object,DeeString_STR(attr_name),rt_error) != 0) return NULL;
     break;
    case DEE_XASTKIND_ATTR_DEL_C:
     if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP,
      (DeeObject *)lexer,(DeeObject *)tk,"The expression 'del %r.%q;' will cause an %K at runtime",
      ast_object,DeeString_STR(attr_name),rt_error) != 0) return NULL;
     break;
    default:
#if 1
     DEE_BUILTIN_UNREACHABLE();
#else
     Dee_DECREF(rt_error);
     break;
#endif
   }
  } else {
//return_attr_result:
   result = DeeXAst_NewConst(tk,attr_result);
   Dee_DECREF(attr_result);
   return result;
  }
 }
normal_ast:
 if DEE_LIKELY((result = _DeeXAst_NewUnsafe(kind,tk)) != NULL) {
  Dee_INCREF(result->ast_attr_c.ac_object = ast_object);
  Dee_INCREF(result->ast_attr_c.ac_name = (DeeStringObject *)attr_name);
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewAttrSetC(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *ast_object,
 DEE_A_INOUT_OBJECT(DeeStringObject) *attr_name, DEE_A_INOUT DeeXAstObject *ast_value) {
 DeeXAstObject *result; DeeObject *object_copy,*value_copy; int temp;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(ast_object) && DeeXAst_Check(ast_object));
 DEE_ASSERT(DeeObject_Check(attr_name) && DeeString_Check(attr_name));
 DEE_ASSERT(DeeObject_Check(ast_value) && DeeXAst_Check(ast_value));
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
     ast_object->ast_kind == DEE_XASTKIND_CONST &&
     ast_value->ast_kind == DEE_XASTKIND_CONST) {
  if DEE_UNLIKELY((object_copy = DeeObject_DeepCopy(ast_object->ast_const.c_const)) == NULL) {
err_copy: // Ignore errors in copy, as the copy-operation wouldn't be performed at runtime
   DeeError_Handled();
   goto normal_ast;
  }
  if DEE_UNLIKELY((value_copy = DeeObject_DeepCopy(ast_value->ast_const.c_const)) == NULL) {
   Dee_DECREF(object_copy); goto err_copy;
  }
  temp = DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_ATTR_SET_C,lexer,tk,Dee_TYPE(object_copy));
  if DEE_LIKELY(temp == 0) temp = DeeObject_SetAttr(object_copy,attr_name,value_copy);
  Dee_DECREF(value_copy);
  Dee_DECREF(object_copy);
  if DEE_UNLIKELY(temp != 0) {
   DeeTypeObject *rt_error;
   rt_error = _DeeError_HandleAndReturnType();
   if DEE_UNLIKELY(DEE_LIKELY(rt_error) && DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP,
    (DeeObject *)lexer,(DeeObject *)tk,"The expression '%r.%s = %r' will cause an %K at runtime",
    ast_object,DeeString_STR(attr_name),ast_value,rt_error) != 0)) return NULL;
  } else {
   // Return the value AST
   Dee_INCREF(ast_value);
   return ast_value;
  }
 }
normal_ast:
 if DEE_LIKELY((result = _DeeXAst_NewUnsafe(DEE_XASTKIND_ATTR_SET_C,tk)) != NULL) {
  Dee_INCREF(result->ast_attr_set_c.ac_object = ast_object);
  Dee_INCREF(result->ast_attr_set_c.ac_name = (DeeStringObject *)attr_name);
  Dee_INCREF(result->ast_attr_set_c.ac_value = ast_value);
 }
 return result;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *_DeeXAst_NewSeqGetOrDelRange(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeXAstObject *ast_object, DEE_A_INOUT_OPT DeeXAstObject *ast_begin,
 DEE_A_INOUT_OPT DeeXAstObject *ast_end) {
 DeeXAstObject *result; DeeObject *const_object,*const_begin,*const_end,*ast_result;
 DEE_ASSERTF(kind == DEE_XASTKIND_SEQ_RANGE_GET ||
             kind == DEE_XASTKIND_SEQ_RANGE_DEL,
             "Given xast kind isn't allowed");
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(ast_object) && DeeXAst_Check(ast_object));
 DEE_ASSERT(!ast_begin || (DeeObject_Check(ast_begin) && DeeXAst_Check(ast_begin)));
 DEE_ASSERT(!ast_end || (DeeObject_Check(ast_end) && DeeXAst_Check(ast_end)));
 if (ast_begin && ast_begin->ast_kind == DEE_XASTKIND_CONST &&
     DeeNone_Check(ast_begin->ast_const.c_const)) ast_begin = NULL;
 if (ast_end && ast_end->ast_kind == DEE_XASTKIND_CONST &&
     DeeNone_Check(ast_end->ast_const.c_const)) ast_end = NULL;
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
     ast_object->ast_kind == DEE_XASTKIND_CONST &&
     (!ast_begin || ast_begin->ast_kind == DEE_XASTKIND_CONST) &&
     (!ast_end || ast_end->ast_kind == DEE_XASTKIND_CONST)) {
  if DEE_UNLIKELY((const_object = DeeObject_DeepCopy(ast_object->ast_const.c_const)) == NULL) {
err_copy: // Ignore errors in copy, as the copy-operation wouldn't be performed at runtime
   DeeError_Handled();
   goto normal_ast;
  }
  if (ast_begin) {
   if DEE_UNLIKELY((const_begin = DeeObject_DeepCopy(ast_begin->ast_const.c_const)) == NULL) {
err_copy2: Dee_DECREF(const_object); goto err_copy;
   }
  } else const_begin = DeeNone_New();
  if (ast_end) {
   if DEE_UNLIKELY((const_end = DeeObject_DeepCopy(ast_end->ast_const.c_const)) == NULL) {
/*err_copy3:*/ Dee_DECREF(const_begin); goto err_copy2;
   }
  } else const_end = DeeNone_New();
  switch (kind) {
   case DEE_XASTKIND_SEQ_RANGE_GET:
    ast_result = DeeObject_GetRangeItem(const_object,const_begin,const_end);
    break;
   case DEE_XASTKIND_SEQ_RANGE_DEL:
    if DEE_UNLIKELY(DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_SEQ_RANGE_DEL,lexer,tk,Dee_TYPE(const_object)) != 0) ast_result = NULL;
    else if DEE_UNLIKELY(DeeObject_DelRangeItem(const_object,const_begin,const_end) != 0) ast_result = NULL;
    else ast_result = DeeNone_New();
    break;
   default:
#if 1
    DEE_BUILTIN_UNREACHABLE();
#else
    Dee_DECREF(const_end);
    Dee_DECREF(const_begin);
    Dee_DECREF(const_object);
    goto normal_ast;
#endif
  }
  Dee_DECREF(const_end);
  Dee_DECREF(const_begin);
  Dee_DECREF(const_object);
  if DEE_UNLIKELY(!ast_result) {
   DeeTypeObject *rt_error;
//handle_range_error:
   rt_error = _DeeError_HandleAndReturnType();
   if DEE_LIKELY(rt_error) switch (kind) {
    case DEE_XASTKIND_SEQ_RANGE_GET:
     if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP,
      (DeeObject *)lexer,(DeeObject *)tk,"The expression '%r[%r:%r]' will cause an %K at runtime",
      ast_object,ast_begin ? (DeeObject *)ast_begin : Dee_None,
      ast_end ? (DeeObject *)ast_end : Dee_None,rt_error) != 0) return NULL;
     break;
    case DEE_XASTKIND_SEQ_RANGE_DEL:
     if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP,
      (DeeObject *)lexer,(DeeObject *)tk,"The expression 'del %r[%r:%r];' will cause an %K at runtime",
      ast_object,ast_begin ? (DeeObject *)ast_begin : Dee_None,
      ast_end ? (DeeObject *)ast_end : Dee_None,rt_error) != 0) return NULL;
     break;
    default:
#if 1
     DEE_BUILTIN_UNREACHABLE();
#else
     Dee_DECREF(rt_error);
     break;
#endif
   }
  } else {
//return_range_result:
   result = DeeXAst_NewConst(tk,ast_result);
   Dee_DECREF(ast_result);
   return result;
  }
 }
normal_ast:
 if DEE_LIKELY((result = _DeeXAst_NewUnsafe(kind,tk)) != NULL) {
  Dee_INCREF(result->ast_seq_range.sr_seq = ast_object);
  Dee_XINCREF(result->ast_seq_range.sr_begin = ast_begin);
  Dee_XINCREF(result->ast_seq_range.sr_end = ast_end);
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewSeqRangeSet(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *ast_object,
 DEE_A_INOUT_OPT DeeXAstObject *ast_begin, DEE_A_INOUT_OPT DeeXAstObject *ast_end,
 DEE_A_INOUT DeeXAstObject *ast_value) {
 DeeXAstObject *result; DeeObject *const_object,*const_begin,*const_end,*const_value,*ast_result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(ast_object) && DeeXAst_Check(ast_object));
 DEE_ASSERT(!ast_begin || (DeeObject_Check(ast_begin) && DeeXAst_Check(ast_begin)));
 DEE_ASSERT(!ast_end || (DeeObject_Check(ast_end) && DeeXAst_Check(ast_end)));
 DEE_ASSERT(DeeObject_Check(ast_value) && DeeXAst_Check(ast_value));
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH)!=0) {
  if (ast_begin && ast_begin->ast_kind == DEE_XASTKIND_CONST &&
      DeeNone_Check(ast_begin->ast_const.c_const)) {
   VLOG_PARSER(ast_begin->ast_common.ast_token,"Removing constant none range begin\n");
   ast_begin = NULL;
  }
  if (ast_end && ast_end->ast_kind == DEE_XASTKIND_CONST &&
      DeeNone_Check(ast_end->ast_const.c_const)) {
   VLOG_PARSER(ast_end->ast_common.ast_token,"Removing constant none range end\n");
   ast_end = NULL;
  }
 }
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS) != 0 &&
     ast_object->ast_kind == DEE_XASTKIND_CONST &&
     (!ast_begin || ast_begin->ast_kind == DEE_XASTKIND_CONST) &&
     (!ast_end || ast_end->ast_kind == DEE_XASTKIND_CONST) &&
     ast_value->ast_kind == DEE_XASTKIND_CONST) {
  if DEE_UNLIKELY((const_object = DeeObject_DeepCopy(ast_object->ast_const.c_const)) == NULL) {
err_copy: // Ignore errors in copy, as the copy-operation wouldn't be performed at runtime
   DeeError_Handled();
   goto normal_ast;
  }
  if (ast_begin) {
   if DEE_UNLIKELY((const_begin = DeeObject_DeepCopy(ast_begin->ast_const.c_const)) == NULL) {
err_copy2: Dee_DECREF(const_object); goto err_copy;
   }
  } else const_begin = DeeNone_New();
  if (ast_end) {
   if DEE_UNLIKELY((const_end = DeeObject_DeepCopy(ast_end->ast_const.c_const)) == NULL) {
err_copy3: Dee_DECREF(const_begin); goto err_copy2;
   }
  } else const_end = DeeNone_New();
  if DEE_UNLIKELY((const_value = DeeObject_DeepCopy(ast_value->ast_const.c_const)) == NULL) {
/*err_copy4:*/ Dee_DECREF(const_end); goto err_copy3;
  }
  if DEE_UNLIKELY(DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_SEQ_RANGE_SET,lexer,tk,Dee_TYPE(const_object)) != 0) ast_result = NULL;
  else if DEE_UNLIKELY(DeeObject_SetRangeItem(const_object,const_begin,const_end,const_value) != 0) ast_result = NULL;
  else Dee_INCREF(ast_result = const_value);
  Dee_DECREF(const_end);
  Dee_DECREF(const_begin);
  Dee_DECREF(const_object);
  if DEE_UNLIKELY(!ast_result) {
   DeeTypeObject *rt_error;
/*handle_range_error:*/
   rt_error = _DeeError_HandleAndReturnType();
   if DEE_LIKELY(rt_error) {
    if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP,
     (DeeObject *)lexer,(DeeObject *)tk,"The expression '%r[%r:%r] = %r' will cause an %K at runtime",
     ast_object,ast_begin ? (DeeObject *)ast_begin : Dee_None,
     ast_end ? (DeeObject *)ast_end : Dee_None,ast_value,rt_error) != 0) return NULL;
   }
  } else {
/*return_range_result:*/
   result = DeeXAst_NewConst(tk,ast_result);
   Dee_DECREF(ast_result);
   return result;
  }
 }
normal_ast:
 if DEE_LIKELY((result = _DeeXAst_NewUnsafe(DEE_XASTKIND_SEQ_RANGE_SET,tk)) != NULL) {
  Dee_INCREF(result->ast_seq_range_set.sr_seq = ast_object);
  Dee_XINCREF(result->ast_seq_range_set.sr_begin = ast_begin);
  Dee_XINCREF(result->ast_seq_range_set.sr_end = ast_end);
  Dee_INCREF(result->ast_seq_range_set.sr_value = ast_value);
 }
 return result;
}


#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewForeignFunctionFromInheritedArgv(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *base,
 DEE_A_IN DeeFunctionFlags flags, DEE_A_IN Dee_size_t argc,
 DEE_A_IN_R(argc) DeeXAstObject **argv) {
 DeeXAstObject *result,**iter,**end;
 DeeTypeObject const *predicted_type;
 int is_constant;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(base) && DeeXAst_Check(base));
 DEE_ASSERT((argc != 0) == (argv != NULL));
 predicted_type = DeeXAst_PredictType(base);
 if DEE_UNLIKELY(predicted_type && !DeeType_IsSameOrDerived(predicted_type,&DeeType_Type) &&
                 predicted_type != (DeeTypeObject *)&DeeNone_Type) {
  if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_TYPE_AS_BASE_IN_FOREIGN_FUNCTION,
   (DeeObject *)lexer,(DeeObject *)base->ast_common.ast_token,
   "Expected type as base in foreign function type, but got expression "
   "evaluating to instance of %q:\n\t%>r",DeeType_NAME(predicted_type),base) != 0) return NULL;
 }
 is_constant = ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0 &&
                base->ast_kind == DEE_XASTKIND_CONST);
 end = (iter = argv)+argc;
 while DEE_LIKELY(iter != end) {
  DEE_ASSERT(DeeObject_Check(*iter) && DeeXAst_Check(*iter));
  predicted_type = DeeXAst_PredictType(*iter);
  if DEE_UNLIKELY(predicted_type && !DeeType_IsSameOrDerived(predicted_type,&DeeType_Type)) {
   if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_TYPE_AS_ARG_IN_FOREIGN_FUNCTION,
    (DeeObject *)lexer,(DeeObject *)(*iter)->ast_common.ast_token,
    "Expected type as argument in foreign function type, but got expression "
    "evaluating to instance of %q:\n\t%>r",DeeType_NAME(predicted_type),*iter) != 0) return NULL;
   is_constant = 0; // Not a constant
  } else if (is_constant && (*iter)->ast_kind != DEE_XASTKIND_CONST) is_constant = 0;
  ++iter;
 }
 if (is_constant) {
  DeeTypeObject const **arg_typev,**arg_iter,**arg_end;
  DeeTypeObject *ff_type; DeeXAstObject **src;
  DEE_ASSERT(base->ast_kind == DEE_XASTKIND_CONST);
  if (argc) {
   while DEE_UNLIKELY((arg_typev = (DeeTypeObject const **)malloc_nz(
    argc*sizeof(DeeTypeObject const *))) == NULL) {
    if DEE_LIKELY(Dee_CollectMemory()) continue;
    DeeError_NoMemory();
    return NULL;
   }
   src = argv;
   arg_end = (arg_iter = arg_typev)+argc;
   do {
    DEE_ASSERT((*src)->ast_kind == DEE_XASTKIND_CONST);
    DEE_ASSERT(DeeObject_Check((*src)->ast_const.c_const) && DeeType_Check((*src)->ast_const.c_const));
    *arg_iter = (DeeTypeObject *)(*src)->ast_const.c_const;
    ++src,++arg_iter;
   } while DEE_LIKELY(arg_iter != arg_end);
  } else arg_typev = NULL;
  ff_type = DeeType_ForeignFunction((DeeTypeObject *)base->ast_const.c_const,
                                    flags,argc,arg_typev);
  free((void *)arg_typev);
  if DEE_UNLIKELY(!ff_type) {
   DeeObject *error;
   if DEE_UNLIKELY(!DeeError_GetOccurred(&error,NULL)) return NULL;
   if DEE_UNLIKELY(!error) return NULL;
   Dee_INCREF(error);
   DeeError_Handled();
   if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_INVALID_CONSTOP,
    (DeeObject *)lexer,(DeeObject *)tk,
    "Creating this foreign function will cause an %q at runtime:\n"
    "\t%>K",DeeType_NAME(Dee_TYPE(error)),error) != 0) return NULL;
   goto normal_ast;
  }
  if DEE_UNLIKELY((result = DeeXAst_NewConst(tk,(DeeObject *)ff_type)) == NULL) return NULL;
  // consume the given arguments
  end = (iter = argv)+argc;
  while DEE_LIKELY(iter != end) Dee_DECREF(*iter++);
  free(argv);
  return result;
 }
normal_ast:
 if ((result = _DeeXAst_NewUnsafe(DEE_XASTKIND_FOREIGN_FUNCTION,tk)) != NULL) {
  Dee_INCREF(result->ast_foreignfunction.ff_base = base);
  result->ast_foreignfunction.ff_flags = flags;
  result->ast_foreignfunction.ff_argc = argc;
  result->ast_foreignfunction.ff_argv = argv;
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewForeignFunction(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *base,
 DEE_A_IN DeeFunctionFlags flags, DEE_A_IN Dee_size_t argc,
 DEE_A_IN_R(argc) DeeXAstObject *const *argv) {
 DeeXAstObject *result,**my_argv,**iter,**end;
 if (argc) {
  while DEE_UNLIKELY((my_argv = (DeeXAstObject **)malloc_nz(
   argc*sizeof(DeeXAstObject *))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return NULL;
  }
  end = (iter = my_argv)+argc;
  while DEE_LIKELY(iter != end) Dee_INCREF(*iter++ = *argv++);
 } else my_argv = NULL;
 result = DeeXAst_NewForeignFunctionFromInheritedArgv(tk,lexer,parser_flags,base,
                                                      flags,argc,my_argv);
 if DEE_UNLIKELY(!result) {
  end = (iter = my_argv)+argc;
  while DEE_LIKELY(iter != end) Dee_DECREF(*iter++);
  free(my_argv);
 }
 return result;
}
#endif



DEE_STATIC_INLINE(DeeObject *) _DeeLocalVar_ReprWithToken(
 DeeLocalVarObject *local_var, DeeTokenObject *tk);
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewStoreVar(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeLocalVarObject *dest_var,
 DEE_A_INOUT DeeTokenObject *dest_var_token, DEE_A_INOUT DeeXAstObject *ast_init,
 DEE_A_INOUT DeeScopeObject *curr_scope) {
 DeeXAstObject *result,*dest_var_ast;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(dest_var) && DeeLocalVar_Check(dest_var));
 DEE_ASSERT(DeeObject_Check(dest_var_token) && DeeToken_Check(dest_var_token));
 DEE_ASSERT(DeeObject_Check(ast_init) && DeeXAst_Check(ast_init));
 if DEE_LIKELY(DeeScope_SameWeakScope((DeeObject *)curr_scope,(DeeObject *)dest_var->lv_scope)) {
  switch (DeeLocalVar_KIND(dest_var)) {
   case DEE_LOCALVAR_KIND_THIS:
    if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_CANT_STORE_THIS,
     (DeeObject *)lexer,(DeeObject *)dest_var_token,
     "Can't store a value in a this-variable") != 0) return NULL;
store_asssign:
    if DEE_UNLIKELY((dest_var_ast = DeeXAst_NewLocalVar(dest_var_token,dest_var,curr_scope)) == NULL) return NULL;
    goto store_default_with_dest_var_ast;
   case DEE_LOCALVAR_KIND_STATIC:
    if (DeeLocalVar_IS_INIT(dest_var)) {
     // Warn about secondary static initializers
     if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_STATIC_VARIABLE_ALREADY_INITIALIZED,
      (DeeObject *)lexer,(DeeObject *)dest_var_token,
      "The static variable was already initialized. "
      "Note that secondary initializer will only be executed once.") != 0) return NULL;
    }
    break;
   case DEE_LOCALVAR_KIND_STACK:
    if (!dest_var->lv_init) {
     // First initialization
     if DEE_UNLIKELY(dest_var->lv_scope != curr_scope) {
      if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_STACK_VARIABLE_FIRST_INIT_NOT_DECL_SCOPE,
       (DeeObject *)lexer,(DeeObject *)dest_var_token,
       "The first initialization of a stack variable must happen inside its declaration scope") != 0) return NULL;
      goto store_asssign;
     } else if DEE_UNLIKELY(DeeScope_IS_STRONG(curr_scope)) {
      if DEE_UNLIKELY(DeeError_CompilerError(DEE_WARNING_STACK_VARIABLE_IN_ROOT_SCOPE,
       (DeeObject *)lexer,(DeeObject *)dest_var_token,
       "Declaration of stack variables inside a root/strong scope is not recommended") != 0) return NULL;
     }
    }
    break;
   default: break;
  }
  if DEE_LIKELY((result = _DeeXAst_NewUnsafe(DEE_XASTKIND_VARDECL,tk)) != NULL) {
   Dee_INCREF(result->ast_vardecl.vd_token = dest_var_token);
   Dee_INCREF(result->ast_vardecl.vd_var = dest_var);
   Dee_INCREF(result->ast_vardecl.vd_init = ast_init);
   DeeLocalVar_ADD_INIT(dest_var);
  }
 } else {
  if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_CANT_STORE_VAR_FROM_DIFFERENT_SCOPE,
   (DeeObject *)lexer,(DeeObject *)dest_var_token,
   "Store to variable from different strong scope will be compiled as assign:\n"
   "\t%>K := %>r",_DeeLocalVar_ReprWithToken(dest_var,dest_var_token),ast_init) != 0) return NULL;
  if DEE_UNLIKELY((dest_var_ast = DeeXAst_NewReferenceVar(dest_var_token,dest_var,curr_scope)) == NULL) return NULL;
store_default_with_dest_var_ast:
  result = DeeXAst_NewBinary(DEE_XASTKIND_COPY_ASSIGN,tk,lexer,parser_flags,dest_var_ast,ast_init);
  Dee_DECREF(dest_var_ast);
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewStore(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *ast_dest,
 DEE_A_INOUT DeeXAstObject *ast_init, DEE_A_INOUT DeeScopeObject *curr_scope) {
 DeeXAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(ast_dest) && DeeXAst_Check(ast_dest));
 DEE_ASSERT(DeeObject_Check(ast_init) && DeeXAst_Check(ast_init));
 switch (ast_dest->ast_kind) {
  case DEE_XASTKIND_CONST:
   if (DeeNone_Check(ast_dest->ast_const.c_const)) { Dee_INCREF(ast_init); return ast_init; }
   goto store_default;
  case DEE_XASTKIND_VAR:
   return DeeXAst_NewStoreVar(tk,lexer,parser_flags,ast_dest->ast_var.vs_var,
                              ast_dest->ast_common.ast_token,ast_init,curr_scope);
  case DEE_XASTKIND_SEQ_RANGE_GET:
   return DeeXAst_NewSeqRangeSet(tk,lexer,parser_flags,
                                 ast_dest->ast_seq_range_get.sr_seq,
                                 ast_dest->ast_seq_range_get.sr_begin,
                                 ast_dest->ast_seq_range_get.sr_end,ast_init);
  case DEE_XASTKIND_SEQ_GET:
   return DeeXAst_NewTrinary(DEE_XASTKIND_SEQ_SET,tk,lexer,parser_flags,
                             ast_dest->ast_operator.op_a,
                             ast_dest->ast_operator.op_b,ast_init);
  case DEE_XASTKIND_ATTR_GET:
   return DeeXAst_NewTrinary(DEE_XASTKIND_ATTR_SET,tk,lexer,parser_flags,
                             ast_dest->ast_operator.op_a,
                             ast_dest->ast_operator.op_b,ast_init);
  case DEE_XASTKIND_ATTR_GET_C:
   return DeeXAst_NewAttrSetC(tk,lexer,parser_flags,ast_dest->ast_attr_get_c.ac_object,
                              (DeeObject *)ast_dest->ast_attr_get_c.ac_name,ast_init);
  default: // Fallback: Create an assign operation
store_default:
   if DEE_UNLIKELY(DeeError_CompilerErrorf(DEE_WARNING_STORE_IS_ASSIGN,
    (DeeObject *)lexer,(DeeObject *)tk,
    "Store operation will be compiled as assign:\n"
    "\t%>r := %>r",ast_dest,ast_init) != 0) return NULL;
   result = DeeXAst_NewBinary(DEE_XASTKIND_COPY_ASSIGN,tk,lexer,parser_flags,ast_dest,ast_init);
   break;
 }
 return result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewStoreVardeclStorage(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_IN struct DeeVarDeclStorage const *storage,
 DEE_A_INOUT DeeXAstObject *ast_init, DEE_A_INOUT DeeScopeObject *curr_scope) {
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(storage);
 DEE_ASSERT(DeeObject_Check(ast_init) && DeeXAst_Check(ast_init));
 DEE_ASSERT(DeeObject_Check(curr_scope) && DeeScope_Check(curr_scope));
 switch (storage->vds_kind) {
  case DEE_VARDECLSTORAGEKIND_VAR:
   return DeeXAst_NewStoreVar(tk,lexer,parser_flags,storage->vds_var.vs_var,
                              storage->vds_var.vs_token,ast_init,curr_scope);
  case DEE_VARDECLSTORAGEKIND_ATTR:
   return DeeXAst_NewAttrSet(tk,lexer,parser_flags,storage->vds_attr.as_object,
                             storage->vds_attr.as_attr,ast_init);
  case DEE_VARDECLSTORAGEKIND_ATTR_C:
   return DeeXAst_NewAttrSetC(tk,lexer,parser_flags,storage->vds_attr_c.acs_object,
                              (DeeObject *)storage->vds_attr_c.acs_attr,ast_init);
  case DEE_VARDECLSTORAGEKIND_ITEM:
   return DeeXAst_NewSeqSet(tk,lexer,parser_flags,storage->vds_item.is_object,
                            storage->vds_item.is_key,ast_init);
  case DEE_VARDECLSTORAGEKIND_RANGE:
   return DeeXAst_NewSeqRangeSet(tk,lexer,parser_flags,storage->vds_range.rs_object,
                                 storage->vds_range.rs_begin,storage->vds_range.rs_end,ast_init);
  case DEE_VARDECLSTORAGEKIND_EXPAND:
   return DeeXAst_NewMultiStore(tk,lexer,parser_flags,storage->vds_expand.es_declc,
                                storage->vds_expand.es_declv,ast_init,
                                DEE_XASTKIND_TUPLE,curr_scope);
  case DEE_VARDECLSTORAGEKIND_ASSIGN:
   return DeeXAst_NewCopyAssign(tk,lexer,parser_flags,storage->vds_assign.as_ast,ast_init);
  default: DEE_BUILTIN_UNREACHABLE();
 }
}


DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewMultiStoreFromInheritedStorage(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_IN Dee_size_t storage_c,
 DEE_A_IN_R(storage_c) struct DeeVarDeclStorage *storage_v,
 DEE_A_INOUT DeeXAstObject *ast_init, DEE_A_IN DeeXAstKind return_seq_kind,
 DEE_A_INOUT DeeScopeObject *curr_scope) {
 DeeXAstObject *result,*mvd_ast;
 DEE_ASSERT(return_seq_kind == DEE_XASTKIND_TUPLE ||
            return_seq_kind == DEE_XASTKIND_LIST ||
            return_seq_kind == DEE_XASTKIND_SET);
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(storage_c != 0); DEE_ASSERT(storage_v != NULL);
 DEE_ASSERT(DeeObject_Check(ast_init) && DeeXAst_Check(ast_init));
 DEE_ASSERT(DeeObject_Check(curr_scope) && DeeScope_Check(curr_scope));
 if DEE_UNLIKELY((mvd_ast = _DeeXAst_NewUnsafe(DEE_XASTKIND_MULTIVARDECL,tk)) == NULL) return NULL;
 mvd_ast->ast_multivardecl.mvd_type = return_seq_kind;
 mvd_ast->ast_multivardecl.mvd_varc = storage_c;
 mvd_ast->ast_multivardecl.mvd_varv = storage_v;
 Dee_INCREF(mvd_ast->ast_multivardecl.mvd_init = ast_init);
 if DEE_UNLIKELY((result = DeeXAst_OptimizeMultiVarDecl(mvd_ast,lexer,parser_flags,curr_scope)) == NULL) {
  Dee_DECREF(ast_init);
  _DeeXAst_FreeUnsafe(mvd_ast);
  return NULL;
 }
 Dee_DECREF(mvd_ast);
 return result;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewMultiStore(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_IN Dee_size_t storage_c,
 DEE_A_IN_R(storage_c) struct DeeVarDeclStorage const *storage_v,
 DEE_A_INOUT DeeXAstObject *ast_init, DEE_A_IN DeeXAstKind return_seq_kind,
 DEE_A_INOUT DeeScopeObject *curr_scope) {
 DeeXAstObject *result; struct DeeVarDeclStorage *storage_copy,*iter,*end;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(storage_c != 0); DEE_ASSERT(storage_v != NULL);
 DEE_ASSERT(DeeObject_Check(ast_init) && DeeXAst_Check(ast_init));
 DEE_ASSERT(DeeObject_Check(curr_scope) && DeeScope_Check(curr_scope));
 while DEE_UNLIKELY((storage_copy = (struct DeeVarDeclStorage *)malloc_nz(
  storage_c*sizeof(struct DeeVarDeclStorage))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 end = (iter = storage_copy)+storage_c;
 do {
  if (DeeVarDeclStorage_InitCopy(iter,storage_v) != 0) {
err_iter:
   while (iter != storage_copy) { --iter; DeeVarDeclStorage_Quit(iter); }
   free_nn(storage_copy);
   return NULL;
  }
  ++iter,++storage_v;
 } while (iter != end);
 result = DeeXAst_NewMultiStoreFromInheritedStorage(tk,lexer,parser_flags,storage_c,
                                                    storage_copy,ast_init,
                                                    return_seq_kind,curr_scope);
 if (!result) { iter = storage_copy+storage_c; goto err_iter; }
 return result;
}



DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewFunctionFromInheritedArgv(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_IN struct DeeCompilerConfig const *config,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *name, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) struct DeeXAstFunctionAstArgument *argv,
 DEE_A_INOUT_OPT struct DeeSAstObject *code, DEE_A_INOUT_OPT DeeScopeObject *scope) {
 DeeXAstObject *ast_result;
 DEE_ASSERT(config);
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(!name || (DeeObject_Check(name) && DeeString_Check(name)));
 DEE_ASSERT(!code || (DeeObject_Check(code) && DeeSAst_Check(code)));
 DEE_ASSERT(!scope || (DeeObject_Check(scope) && DeeScope_Check(scope)));
 DEE_ASSERT(!scope || DeeScope_IS_STRONG(scope));
 DEE_ASSERT((argc != 0) == (argv != NULL));
 DEE_ASSERT((code != NULL) == (scope != NULL));
 DEE_ASSERT(!scope || ((scope->sc_flags&DEE_SCOPE_FLAG_FOUND_THIS)!=0) == ((flags&DEE_FUNCTION_FLAG_THIS)!=0));
 DEE_ASSERT(!scope || ((scope->sc_flags&DEE_SCOPE_FLAG_FOUND_YIELD)!=0) == ((flags&DEE_FUNCTION_FLAG_YILD)!=0));
#ifdef DEE_DEBUG
 {struct DeeXAstFunctionAstArgument *iter,*end;
  end = (iter = argv)+argc;
  while (iter != end) {
   DEE_ASSERT(DeeObject_Check(iter->a_token) && DeeToken_Check(iter->a_token));
   DEE_ASSERT(DeeObject_Check(iter->a_var) && DeeLocalVar_Check(iter->a_var));
   DEE_ASSERT(DeeLocalVar_KIND(iter->a_var) == DEE_LOCALVAR_KIND_PARAM);
   ++iter;
  }
 }
#endif
#define RETURN(x) do{if((ast_result=(x))==NULL)return NULL;goto return_with_unused_args;}while(0)
#define AST_IS_ARG(ast,i) \
 ((ast)->ast_kind == DEE_XASTKIND_VAR && \
 ((ast)->ast_var.vs_flags&DEE_XAST_VARAST_FLAG_REF)==0 && \
  (DeeLocalVar_KIND((ast)->ast_var.vs_var) == DEE_LOCALVAR_KIND_PARAM) && \
  (ast)->ast_var.vs_var->lv_loc_id == (i))
#define XAST_IS_ARG(ast,i) ((ast)&&AST_IS_ARG(ast,i))
 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_PREDICATES) != 0 && code) {
  DeeObject *known_predicate; // Detect known predicates: 'function(x,y) -> x+y' --> '__add__'
  DeeSAstObject *inner_block;
  DeeXAstObject *return_expression;
  inner_block = code;
  // We must still walk one block, as the optimizer can't remove
  // the top one since it is the weak root of the function.
  if (inner_block->ast_kind == DEE_SASTKIND_BLOCK &&
      inner_block->ast_block.b_astc == 1
      ) inner_block = inner_block->ast_block.b_astv[0];
  if (inner_block->ast_kind == DEE_SASTKIND_EMPTY) goto ret_none;
  if (inner_block->ast_kind != DEE_SASTKIND_RETURN) goto after_pred_optimization;
  return_expression = inner_block->ast_return.r_value;
  if DEE_UNLIKELY(!return_expression) {ret_none: RETURN(DeeXAst_NewConst(tk,Dee_None)); }
  DEE_ASSERT(DeeObject_Check(return_expression) && DeeXAst_Check(return_expression));
  switch (return_expression->ast_kind) {
   case DEE_XASTKIND_STR:      known_predicate = (DeeObject *)&DeeString_Type; goto check_unary_predicate;
   case DEE_XASTKIND_REPR:     known_predicate = (DeeObject *)&DeeBuiltinFunction___repr__; goto check_unary_predicate;
   case DEE_XASTKIND_COPY:     known_predicate = (DeeObject *)&DeeBuiltinFunction___copy__; goto check_unary_predicate;
   case DEE_XASTKIND_MOVE:     known_predicate = (DeeObject *)&DeeBuiltinFunction___move__; goto check_unary_predicate;
   case DEE_XASTKIND_TYPE:     known_predicate = (DeeObject *)&DeeBuiltinFunction___type__; goto check_unary_predicate;
   case DEE_XASTKIND_WEAK:     known_predicate = (DeeObject *)&DeeWeakref_Type; goto check_unary_predicate;
   case DEE_XASTKIND_BOOL:     known_predicate = (DeeObject *)&DeeBuiltinFunction___bool__; goto check_unary_predicate; // Not 100% the same
   case DEE_XASTKIND_SUPEROF:  known_predicate = (DeeObject *)&DeeSuper_Type; goto check_unary_predicate;
 //case DEE_XASTKIND_CLASSOF:  known_predicate = (DeeObject *)&DeeBuiltinFunction___class__; goto check_unary_predicate;
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
   case DEE_XASTKIND_REF:      known_predicate = (DeeObject *)&DeeBuiltinFunction___ref__; goto check_unary_predicate;
   case DEE_XASTKIND_DEREF:    known_predicate = (DeeObject *)&DeeBuiltinFunction___deref__; goto check_unary_predicate;
   case DEE_XASTKIND_PTROF:    known_predicate = (DeeObject *)&DeeBuiltinFunction___builtin_pointer_add; goto check_unary_predicate;
   case DEE_XASTKIND_LVALOF:   known_predicate = (DeeObject *)&DeeBuiltinFunction___builtin_lvalue_add; goto check_unary_predicate;
#endif
   case DEE_XASTKIND_NOT:      known_predicate = (DeeObject *)&DeeBuiltinFunction___not__; goto check_unary_predicate;
   case DEE_XASTKIND_NEG:      known_predicate = (DeeObject *)&DeeBuiltinFunction___neg__; goto check_unary_predicate;
   case DEE_XASTKIND_POS:      known_predicate = (DeeObject *)&DeeBuiltinFunction___pos__; goto check_unary_predicate;
   case DEE_XASTKIND_INV:      known_predicate = (DeeObject *)&DeeBuiltinFunction___inv__; goto check_unary_predicate;
   case DEE_XASTKIND_INC:      known_predicate = (DeeObject *)&DeeBuiltinFunction___inc__; goto check_unary_predicate;
   case DEE_XASTKIND_DEC:      known_predicate = (DeeObject *)&DeeBuiltinFunction___dec__; goto check_unary_predicate;
   case DEE_XASTKIND_INCPOST:  known_predicate = (DeeObject *)&DeeBuiltinFunction___incpost__; goto check_unary_predicate;
   case DEE_XASTKIND_DECPOST:  known_predicate = (DeeObject *)&DeeBuiltinFunction___decpost__; goto check_unary_predicate;
 //case DEE_XASTKIND_EXPAND:   known_predicate = (DeeObject *)&DeeBuiltinFunction___expand__; goto check_unary_predicate; // 
   case DEE_XASTKIND_SEQ_SIZE: known_predicate = (DeeObject *)&DeeBuiltinFunction___size__; goto check_unary_predicate;
 //case DEE_XASTKIND_NEW:      known_predicate = (DeeObject *)&DeeBuiltinFunction___new__; goto check_unary_predicate;
 //case DEE_XASTKIND_DELETE:   known_predicate = (DeeObject *)&DeeBuiltinFunction___delete__; goto check_unary_predicate;
   case DEE_XASTKIND_ITERSELF: known_predicate = (DeeObject *)&DeeBuiltinFunction___iterself__; goto check_unary_predicate;
   case DEE_XASTKIND_ITERNEXT: known_predicate = (DeeObject *)&DeeBuiltinFunction___iternext__; goto check_unary_predicate;
   case DEE_XASTKIND_CELL:     known_predicate = (DeeObject *)&DeeCell_Type;
check_unary_predicate:
    if (argc == 1 && (flags&DEE_FUNCTION_FLAG_VARG)==0 &&
        AST_IS_ARG(return_expression->ast_operator.op_a,0)
        ) RETURN(DeeXAst_NewConst(tk,known_predicate));
    break;
    if (0) {case DEE_XASTKIND_SEQ_RANGE_GET: known_predicate = (DeeObject *)&DeeBuiltinFunction___getrange__;}
    if (0) {case DEE_XASTKIND_SEQ_RANGE_DEL: known_predicate = (DeeObject *)&DeeBuiltinFunction___delrange__;}
    if (argc == 3 && (flags&DEE_FUNCTION_FLAG_VARG)==0 &&
        AST_IS_ARG(return_expression->ast_seq_range.sr_seq,0) &&
       XAST_IS_ARG(return_expression->ast_seq_range.sr_begin,1) &&
       XAST_IS_ARG(return_expression->ast_seq_range.sr_end,2)
        ) RETURN(DeeXAst_NewConst(tk,known_predicate));
    break;
   case DEE_XASTKIND_SEQ_RANGE_SET:
    if (argc == 4 && (flags&DEE_FUNCTION_FLAG_VARG)==0 &&
        AST_IS_ARG(return_expression->ast_seq_range_set.sr_seq,0) &&
       XAST_IS_ARG(return_expression->ast_seq_range_set.sr_begin,1) &&
       XAST_IS_ARG(return_expression->ast_seq_range_set.sr_end,2) &&
       XAST_IS_ARG(return_expression->ast_seq_range_set.sr_value,3)
        ) RETURN(DeeXAst_NewConst(tk,(DeeObject *)&DeeBuiltinFunction___setrange__));
    break;

   case DEE_XASTKIND_CALL:
    // Optimize '[](x,y) -> x(y...);' --> '__call__'
    // Optimize '[](x,y...) -> x(y...);' --> '__callv__'
    if (argc == 2 &&
        AST_IS_ARG(return_expression->ast_operator.op_a,0) &&
        return_expression->ast_operator.op_b->ast_kind == DEE_XASTKIND_TUPLE &&
        return_expression->ast_operator.op_b->ast_sequence.s_elemc == 1) {
     DeeXAstObject *args_elem0 = return_expression->ast_operator.op_b->ast_sequence.s_elemv[0];
     if (args_elem0->ast_kind == DEE_XASTKIND_EXPAND &&
         AST_IS_ARG(args_elem0->ast_operator.op_a,1)
         ) RETURN(DeeXAst_NewConst(tk,((flags&DEE_FUNCTION_FLAG_VARG)!=0
         ? (DeeObject *)&DeeBuiltinFunction___callv__
         : (DeeObject *)&DeeBuiltinFunction___call__)));
    }
    break;

    { // Unary inplace-var
     if (0) { case DEE_XASTKIND_VAR_INC:     known_predicate = (DeeObject *)&DeeBuiltinFunction___inc__; }
     if (0) { case DEE_XASTKIND_VAR_DEC:     known_predicate = (DeeObject *)&DeeBuiltinFunction___dec__; }
     if (0) { case DEE_XASTKIND_VAR_INCPOST: known_predicate = (DeeObject *)&DeeBuiltinFunction___incpost__; }
     if (0) { case DEE_XASTKIND_VAR_DECPOST: known_predicate = (DeeObject *)&DeeBuiltinFunction___decpost__; }
     if (argc == 1 && (flags&DEE_FUNCTION_FLAG_VARG)==0 &&
        (DeeLocalVar_KIND(return_expression->ast_unary_var.uiv_var) == DEE_LOCALVAR_KIND_PARAM) &&
         return_expression->ast_unary_var.uiv_var->lv_loc_id == 0
         ) RETURN(DeeXAst_NewConst(tk,known_predicate));
     break;
    }

    { // Binary inplace-var
     if (0) { case DEE_XASTKIND_VAR_IADD: known_predicate = (DeeObject *)&DeeBuiltinFunction___iadd__; }
     if (0) { case DEE_XASTKIND_VAR_ISUB: known_predicate = (DeeObject *)&DeeBuiltinFunction___isub__; }
     if (0) { case DEE_XASTKIND_VAR_IMUL: known_predicate = (DeeObject *)&DeeBuiltinFunction___imul__; }
     if (0) { case DEE_XASTKIND_VAR_IDIV: known_predicate = (DeeObject *)&DeeBuiltinFunction___idiv__; }
     if (0) { case DEE_XASTKIND_VAR_IMOD: known_predicate = (DeeObject *)&DeeBuiltinFunction___imod__; }
     if (0) { case DEE_XASTKIND_VAR_ISHL: known_predicate = (DeeObject *)&DeeBuiltinFunction___ishl__; }
     if (0) { case DEE_XASTKIND_VAR_ISHR: known_predicate = (DeeObject *)&DeeBuiltinFunction___ishr__; }
     if (0) { case DEE_XASTKIND_VAR_IAND: known_predicate = (DeeObject *)&DeeBuiltinFunction___iand__; }
     if (0) { case DEE_XASTKIND_VAR_IOR:  known_predicate = (DeeObject *)&DeeBuiltinFunction___ior__;  }
     if (0) { case DEE_XASTKIND_VAR_IXOR: known_predicate = (DeeObject *)&DeeBuiltinFunction___ixor__; }
     if (0) { case DEE_XASTKIND_VAR_IPOW: known_predicate = (DeeObject *)&DeeBuiltinFunction___ipow__; }
     if (argc == 2 && (flags&DEE_FUNCTION_FLAG_VARG)==0 &&
        (DeeLocalVar_KIND(return_expression->ast_binary_var.biv_var) == DEE_LOCALVAR_KIND_PARAM) &&
         return_expression->ast_binary_var.biv_var->lv_loc_id == 0 &&
         AST_IS_ARG(return_expression->ast_binary_var.biv_arg,1)
         ) RETURN(DeeXAst_NewConst(tk,known_predicate));
     break;
    }

   case DEE_XASTKIND_COMPARE_LO:  known_predicate = (DeeObject *)&DeeBuiltinFunction___lo__; goto check_binary_predicate;
   case DEE_XASTKIND_COMPARE_LE:  known_predicate = (DeeObject *)&DeeBuiltinFunction___le__; goto check_binary_predicate;
   case DEE_XASTKIND_COMPARE_EQ:  known_predicate = (DeeObject *)&DeeBuiltinFunction___eq__; goto check_binary_predicate;
   case DEE_XASTKIND_COMPARE_NE:  known_predicate = (DeeObject *)&DeeBuiltinFunction___ne__; goto check_binary_predicate;
   case DEE_XASTKIND_COMPARE_GR:  known_predicate = (DeeObject *)&DeeBuiltinFunction___gr__; goto check_binary_predicate;
   case DEE_XASTKIND_COMPARE_GE:  known_predicate = (DeeObject *)&DeeBuiltinFunction___ge__; goto check_binary_predicate;
   case DEE_XASTKIND_ADD:         known_predicate = (DeeObject *)&DeeBuiltinFunction___add__; goto check_binary_predicate;
   case DEE_XASTKIND_SUB:         known_predicate = (DeeObject *)&DeeBuiltinFunction___sub__; goto check_binary_predicate;
   case DEE_XASTKIND_MUL:         known_predicate = (DeeObject *)&DeeBuiltinFunction___mul__; goto check_binary_predicate;
   case DEE_XASTKIND_DIV:         known_predicate = (DeeObject *)&DeeBuiltinFunction___div__; goto check_binary_predicate;
   case DEE_XASTKIND_MOD:         known_predicate = (DeeObject *)&DeeBuiltinFunction___mod__; goto check_binary_predicate;
   case DEE_XASTKIND_SHL:         known_predicate = (DeeObject *)&DeeBuiltinFunction___shl__; goto check_binary_predicate;
   case DEE_XASTKIND_SHR:         known_predicate = (DeeObject *)&DeeBuiltinFunction___shr__; goto check_binary_predicate;
   case DEE_XASTKIND_AND:         known_predicate = (DeeObject *)&DeeBuiltinFunction___and__; goto check_binary_predicate;
   case DEE_XASTKIND_OR:          known_predicate = (DeeObject *)&DeeBuiltinFunction___or__; goto check_binary_predicate;
   case DEE_XASTKIND_XOR:         known_predicate = (DeeObject *)&DeeBuiltinFunction___xor__; goto check_binary_predicate;
   case DEE_XASTKIND_POW:         known_predicate = (DeeObject *)&DeeBuiltinFunction___pow__; goto check_binary_predicate;
   case DEE_XASTKIND_IADD:        known_predicate = (DeeObject *)&DeeBuiltinFunction___iadd__; goto check_binary_predicate;
   case DEE_XASTKIND_ISUB:        known_predicate = (DeeObject *)&DeeBuiltinFunction___isub__; goto check_binary_predicate;
   case DEE_XASTKIND_IMUL:        known_predicate = (DeeObject *)&DeeBuiltinFunction___imul__; goto check_binary_predicate;
   case DEE_XASTKIND_IDIV:        known_predicate = (DeeObject *)&DeeBuiltinFunction___idiv__; goto check_binary_predicate;
   case DEE_XASTKIND_IMOD:        known_predicate = (DeeObject *)&DeeBuiltinFunction___imod__; goto check_binary_predicate;
   case DEE_XASTKIND_ISHL:        known_predicate = (DeeObject *)&DeeBuiltinFunction___ishl__; goto check_binary_predicate;
   case DEE_XASTKIND_ISHR:        known_predicate = (DeeObject *)&DeeBuiltinFunction___ishr__; goto check_binary_predicate;
   case DEE_XASTKIND_IAND:        known_predicate = (DeeObject *)&DeeBuiltinFunction___iand__; goto check_binary_predicate;
   case DEE_XASTKIND_IOR:         known_predicate = (DeeObject *)&DeeBuiltinFunction___ior__; goto check_binary_predicate;
   case DEE_XASTKIND_IXOR:        known_predicate = (DeeObject *)&DeeBuiltinFunction___ixor__; goto check_binary_predicate;
   case DEE_XASTKIND_IPOW:        known_predicate = (DeeObject *)&DeeBuiltinFunction___ipow__; goto check_binary_predicate;
   case DEE_XASTKIND_SEQ_GET:     known_predicate = (DeeObject *)&DeeBuiltinFunction___getitem__; goto check_binary_predicate;
   case DEE_XASTKIND_SEQ_DEL:     known_predicate = (DeeObject *)&DeeBuiltinFunction___delitem__; goto check_binary_predicate;
   case DEE_XASTKIND_ATTR_GET:    known_predicate = (DeeObject *)&DeeBuiltinFunction___getattr__; goto check_binary_predicate;
   case DEE_XASTKIND_ATTR_HAS:    known_predicate = (DeeObject *)&DeeBuiltinFunction___hasattr__; goto check_binary_predicate;
   case DEE_XASTKIND_ATTR_DEL:    known_predicate = (DeeObject *)&DeeBuiltinFunction___delattr__; goto check_binary_predicate;
   case DEE_XASTKIND_IS:          known_predicate = (DeeObject *)&DeeBuiltinFunction___is__; goto check_binary_predicate;
 //case DEE_XASTKIND_NOT_IS:      known_predicate = (DeeObject *)&DeeBuiltinFunction___notis__; goto check_binary_predicate;
 //case DEE_XASTKIND_NOT_IN:      known_predicate = (DeeObject *)&DeeBuiltinFunction___notin__; goto check_binary_predicate;
 //case DEE_XASTKIND_LAND:        known_predicate = (DeeObject *)&DeeBuiltinFunction___land__; goto check_binary_predicate;
 //case DEE_XASTKIND_LOR:         known_predicate = (DeeObject *)&DeeBuiltinFunction___lor__; goto check_binary_predicate;
 //case DEE_XASTKIND_LXOR:        known_predicate = (DeeObject *)&DeeBuiltinFunction___lxor__; goto check_binary_predicate;
   case DEE_XASTKIND_MOVE_ASSIGN: known_predicate = (DeeObject *)&DeeBuiltinFunction___moveassign__; goto check_binary_predicate;
   case DEE_XASTKIND_COPY_ASSIGN: known_predicate = (DeeObject *)&DeeBuiltinFunction___copyassign__;
check_binary_predicate:
    if (argc == 2 && (flags&DEE_FUNCTION_FLAG_VARG)==0 &&
        AST_IS_ARG(return_expression->ast_operator.op_a,0) &&
        AST_IS_ARG(return_expression->ast_operator.op_b,1)
        ) RETURN(DeeXAst_NewConst(tk,known_predicate));
    break;
   case DEE_XASTKIND_IN: 
    if (argc == 2 && (flags&DEE_FUNCTION_FLAG_VARG)==0 &&
        AST_IS_ARG(return_expression->ast_operator.op_a,1) &&
        AST_IS_ARG(return_expression->ast_operator.op_b,0)
        ) RETURN(DeeXAst_NewConst(tk,(DeeObject *)&DeeBuiltinFunction___contains__));
    break;
   case DEE_XASTKIND_SEQ_SET:  known_predicate = (DeeObject *)&DeeBuiltinFunction___setitem__; goto check_trinary_predicate;
   case DEE_XASTKIND_ATTR_SET: known_predicate = (DeeObject *)&DeeBuiltinFunction___setattr__;
check_trinary_predicate:
    if (argc == 3 && (flags&DEE_FUNCTION_FLAG_VARG)==0 &&
        AST_IS_ARG(return_expression->ast_operator.op_a,0) &&
        AST_IS_ARG(return_expression->ast_operator.op_b,1) &&
        AST_IS_ARG(return_expression->ast_operator.op_c,2)
        ) RETURN(DeeXAst_NewConst(tk,known_predicate));
    break;
   default: break;
  }
 }
after_pred_optimization:
#undef XAST_IS_ARG
#undef AST_IS_ARG
#undef RETURN
 if ((parser_flags&DEE_PARSER_FLAG_CONST_FUNCTION) != 0) {
  DeeObject *function_object,*function_code_ob;
  if (!scope) {
   // Without a scope, there is no code
   Dee_INCREF(function_code_ob = Dee_EmptyCode);
   goto do_create_const_function;
  } else if ((scope->sc_flags&DEE_SCOPE_FLAG_FOUND_REFS)==0) {
   if (config->optimize_flags != DEE_OPTIMIZE_FLAG_NONE) {
    Dee_size_t n_performed; // Optimize until there's nothing left to optimize
    do {
     struct DeeOptimizerAssumptions assumptions; int error;
     n_performed = 0;
     _DeeOptimizerAssumptions_InitRoot(&assumptions,scope);
     error = DeeSAst_Optimize(code,&n_performed,lexer,scope,config,config->optimize_flags
                              &~(DEE_OPTIMIZE_FLAG_USED),&assumptions);
     _DeeOptimizerAssumptions_Quit(&assumptions);
     if DEE_UNLIKELY(error != 0) return NULL;
    } while (n_performed);
   }
   {// Compile the code into actual bitcode
    Dee_uint32_t compiler_flags = config->compiler_flags&~(DEE_COMPILER_FLAG_USED);
    struct DeeCodeWriter writer = DeeCodeWriter_INIT();
    writer.cw_root_scope = scope;
    if ((scope->sc_flags&DEE_SCOPE_FLAG_CODE_COPYABLE)!=0)
     writer.cw_flags |= DEE_CODE_FLAG_COPY; // Enable the copyable bit
    if DEE_UNLIKELY(DeeSAst_CompileStrongScopeRoot(code,&writer,lexer,scope,config,compiler_flags) != 0) {
err_writer: DeeCodeWriter_Quit(&writer); return NULL;
    }
    if (writer.cw_refc) { DeeCodeWriter_Quit(&writer); goto normal_ast; } // Not a constant function
    if DEE_UNLIKELY(_DeeXAst_CompileFunctionGenerateReturnCode(
     code,scope,flags,scope->sc_flags,compiler_flags,&writer) != 0) goto err_writer;
    if DEE_UNLIKELY(DeeCodeWriter_Optimize(&writer,compiler_flags) != 0) goto err_writer;
    if DEE_UNLIKELY((function_code_ob = DeeCodeWriter_Pack(&writer,(DeeObject *)(
     (compiler_flags&DEE_COMPILER_FLAG_DEBUG_FUNC)!=0 ? name : NULL)
     )) == NULL) goto err_writer; // Pack the code
    DeeCodeWriter_Quit(&writer);
   }
do_create_const_function:
   function_object = DeeFunction_New(function_code_ob,Dee_EmptyTuple,argc,flags);
   Dee_DECREF(function_code_ob);
   if DEE_UNLIKELY(!function_object) return NULL;
   ast_result = DeeXAst_NewConst(tk,function_object);
   Dee_DECREF(function_object);
   if DEE_UNLIKELY(!ast_result) return NULL;
   goto return_with_unused_args;
  }
 }
normal_ast:
 if ((ast_result = _DeeXAst_NewUnsafe(DEE_XASTKIND_FUNCTION,tk)) != NULL) {
  ast_result->ast_function.f_argc = argc;
  ast_result->ast_function.f_argv = argv;
  ast_result->ast_function.f_flags = flags;
  Dee_XINCREF(ast_result->ast_function.f_code = code);
  Dee_XINCREF(ast_result->ast_function.f_scope = scope);
  Dee_XINCREF(ast_result->ast_function.f_name = (DeeStringObject *)name);
 }
 return ast_result;
return_with_unused_args: {
  struct DeeXAstFunctionAstArgument *iter,*end;
  end = (iter = argv)+argc;
  while (iter != end) { _DeeXAstFunctionAstArgument_Quit(iter); ++iter; }
  free(argv);
 }
 return ast_result;
}
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewFunction(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_IN struct DeeCompilerConfig const *config,
 DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *name, DEE_A_IN Dee_uint32_t flags,
 DEE_A_IN Dee_size_t argc, DEE_A_IN_R(argc) struct DeeXAstFunctionAstArgument const *argv,
 DEE_A_INOUT_OPT struct DeeSAstObject *code, DEE_A_INOUT_OPT DeeScopeObject *scope) {
 DeeXAstObject *ast_result; struct DeeXAstFunctionAstArgument *used_argv,*iter,*end;
 DEE_ASSERT((argc != 0) == (argv != NULL));
 if (argc) {
  while DEE_UNLIKELY((used_argv = (struct DeeXAstFunctionAstArgument *)malloc_nz(
   argc*sizeof(struct DeeXAstFunctionAstArgument))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return NULL;
  }
  end = (iter = used_argv)+argc;
  do {
   DEE_ASSERT(DeeObject_Check(iter->a_token) && DeeToken_Check(iter->a_token));
   DEE_ASSERT(DeeObject_Check(iter->a_var) && DeeLocalVar_Check(iter->a_var));
   DEE_ASSERT(DeeLocalVar_KIND(iter->a_var) == DEE_LOCALVAR_KIND_PARAM);
   Dee_INCREF(iter->a_token = argv->a_token);
   Dee_INCREF(iter->a_var = argv->a_var);
   Dee_XINCREF(iter->a_default = argv->a_default);
   ++iter,++argv;
  } while (iter != end);
 } else used_argv = NULL;
 ast_result = DeeXAst_NewFunctionFromInheritedArgv(tk,lexer,parser_flags,config,
                                                   name,flags,argc,used_argv,code,scope);
 if (DEE_UNLIKELY(!ast_result) && used_argv) {
  end = (iter = used_argv)+argc;
  do _DeeXAstFunctionAstArgument_Quit(iter);
  while (++iter != end);
  free_nn(used_argv);
 }
 return ast_result;
}
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewClassFromInheritedEntries(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *name,
 DEE_A_INOUT_OPT DeeXAstObject *base, DEE_A_IN Dee_uint32_t more_tpflags,
 DEE_A_IN Dee_size_t entryc, DEE_A_IN_R(entryc) struct DeeXAstClassAstEntry *entryv,
 DEE_A_IN_OPT struct DeeUUID const *uuid) {
 DeeXAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(!name || (DeeObject_Check(name) && DeeString_Check(name)));
 DEE_ASSERT((entryc != 0) == (entryv != NULL));
 (void)lexer,parser_flags; // TODO: Optimizations (constant class and such)
 if DEE_LIKELY((result = _DeeXAst_NewUnsafe(DEE_XASTKIND_CLASS,tk)) != NULL) {
  Dee_XINCREF(result->ast_class.c_base = base);
  Dee_XINCREF(result->ast_class.c_name = (DeeStringObject *)name);
  result->ast_class.c_entryc = entryc;
  result->ast_class.c_entryv = entryv;
  result->ast_class.c_tpflags = more_tpflags;
  if (uuid) {
   DeeUUID_SET(&result->ast_class.c_uuid,uuid);
   result->ast_class.c_tpflags |= DEE_XAST_CLASSAST_FLAG_HAS_UUID;
  } else {
   result->ast_class.c_tpflags &= ~DEE_XAST_CLASSAST_FLAG_HAS_UUID;
  }
 }
 return result;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewClass(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT_OBJECT_OPT(DeeStringObject) *name,
 DEE_A_INOUT_OPT DeeXAstObject *base, DEE_A_IN Dee_uint32_t more_tpflags,
 DEE_A_IN Dee_size_t entryc, DEE_A_IN_R(entryc) struct DeeXAstClassAstEntry const *entryv,
 DEE_A_IN_OPT struct DeeUUID const *uuid) {
 DeeXAstObject *ast_result; struct DeeXAstClassAstEntry *used_entryv,*iter,*end;
 DEE_ASSERT((entryc != 0) == (entryv != NULL));
 if (entryc) {
  while DEE_UNLIKELY((used_entryv = (struct DeeXAstClassAstEntry *)malloc_nz(
   entryc*sizeof(struct DeeXAstClassAstEntry))) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   DeeError_NoMemory();
   return NULL;
  }
  end = (iter = used_entryv)+entryc;
  do {
   _DeeXAstClassAstEntry_InitCopy(iter,entryv);
   ++iter,++entryv;
  } while (iter != end);
 } else used_entryv = NULL;
 ast_result = DeeXAst_NewClassFromInheritedEntries(tk,lexer,parser_flags,name,base,
                                                   more_tpflags,entryc,used_entryv,uuid);
 if (DEE_UNLIKELY(!ast_result) && used_entryv) {
  end = (iter = used_entryv)+entryc;
  do _DeeXAstClassAstEntry_Quit(iter);
  while (++iter != end);
  free_nn(used_entryv);
 }
 return ast_result;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */


#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewBuiltinExtern(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT_OPT DeeXAstObject *object_file,
 DEE_A_INOUT DeeXAstObject *object_name, DEE_A_INOUT DeeXAstObject *object_type) {
 DeeXAstObject *ast_result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(!object_file || (DeeObject_Check(object_file) && DeeXAst_Check(object_file)));
 DEE_ASSERT(DeeObject_Check(object_name) && DeeXAst_Check(object_name));
 DEE_ASSERT(DeeObject_Check(object_type) && DeeXAst_Check(object_type));
 (void)lexer,parser_flags; // TODO: Optimizations
 if DEE_LIKELY((ast_result = _DeeXAst_NewUnsafe(DEE_XASTKIND_BUILTIN_EXTERN,tk)) != NULL) {
  Dee_XINCREF(ast_result->ast_builtin_extern.ex_file = object_file);
  Dee_INCREF(ast_result->ast_builtin_extern.ex_name = object_name);
  Dee_INCREF(ast_result->ast_builtin_extern.ex_type = object_type);
 }
 return ast_result;
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */



DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewSwitchFromInheritedSwitches(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *switch_expr,
 DEE_A_IN Dee_size_t switch_c, DEE_A_IN_R(switch_c) struct DeeXAstSwitchEntry *switch_v,
 DEE_A_INOUT_OPT DeeXAstObject *switch_default) {
 DeeXAstObject *result;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(switch_expr) && DeeXAst_Check(switch_expr));
 DEE_ASSERT(switch_v != NULL); DEE_ASSERT(switch_c != 0);
 DEE_ASSERT(!switch_default || (DeeObject_Check(switch_default) && DeeXAst_Check(switch_default)));
 (void)lexer,parser_flags; // TODO: Optimizations
 if DEE_LIKELY((result = _DeeXAst_NewUnsafe(DEE_XASTKIND_SWITCH,tk)) != NULL) {
  Dee_INCREF(result->ast_switch.s_expr = switch_expr);
  result->ast_switch.s_entryc = switch_c;
  result->ast_switch.s_entryv = switch_v;
  Dee_XINCREF(result->ast_switch.s_default = switch_default);
 }
 return result;
}

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewSwitch(
 DEE_A_INOUT DeeTokenObject *tk, DEE_A_INOUT DeeLexerObject *lexer,
 DEE_A_IN Dee_uint32_t parser_flags, DEE_A_INOUT DeeXAstObject *switch_expr,
 DEE_A_IN Dee_size_t switch_c, DEE_A_IN_R(switch_c) struct DeeXAstSwitchEntry const *switch_v,
 DEE_A_INOUT_OPT DeeXAstObject *switch_default) {
 DeeXAstObject *result; struct DeeXAstSwitchEntry *new_switchv,*iter,*end;
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(switch_expr) && DeeXAst_Check(switch_expr));
 DEE_ASSERT(!switch_c || switch_v);
 DEE_ASSERT(!switch_default || (DeeObject_Check(switch_default) && DeeXAst_Check(switch_default)));
 if (!switch_c) {
  // TODO: Warning: Switch without branches
  if (switch_default) Dee_INCREF(switch_default);
  else if DEE_UNLIKELY((switch_default = DeeXAst_NewConst(tk,Dee_None)) == NULL) return NULL;
  result = DeeXAst_NewBuiltinUnused(tk,lexer,parser_flags,switch_expr,switch_default);
  Dee_DECREF(switch_default);
  return result;
 }
 while DEE_UNLIKELY((new_switchv = (struct DeeXAstSwitchEntry *)malloc_nz(
  switch_c*sizeof(struct DeeXAstSwitchEntry))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 end = (iter = new_switchv)+switch_c;
 do {
  Dee_INCREF(iter->se_key = switch_v->se_key);
  Dee_INCREF(iter->se_value = switch_v->se_value);
  ++iter,++switch_v;
 } while (iter != end);
 result = DeeXAst_NewSwitchFromInheritedSwitches(tk,lexer,parser_flags,switch_expr,
                                                 switch_c,new_switchv,switch_default);
 if DEE_UNLIKELY(!result) {
  iter = new_switchv+switch_c;
  while (iter != new_switchv) { --iter; _DeeXAstSwitchEntry_Quit(iter); }
  free_nn(new_switchv);
 }
 return result;
}












DEE_A_RET_EXCEPT(-1) int DeeVarDeclStorage_InitCopy(
 DEE_A_OUT struct DeeVarDeclStorage *self,
 DEE_A_IN struct DeeVarDeclStorage const *right) {
 DEE_ASSERT(self);
 DEE_ASSERT(right);
 _DeeVarDeclStorage_InitCopy(self,right,{ return -1; });
 return 0;
}
void DeeVarDeclStorage_Quit(DEE_A_IN struct DeeVarDeclStorage *self) {
 DEE_ASSERT(self);
 _DeeVarDeclStorage_Quit(self);
}
void DeeVarDeclStorage_Visit(
 DEE_A_IN struct DeeVarDeclStorage *self,
 DEE_A_IN DeeVisitProc visit, void *closure) {
 DEE_ASSERT(self);
 _DeeVarDeclStorage_Visit(self);
}
















static int _deexast_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeXAstObject *self, DeeXAstObject *right) {
 return DeeXAst_InitCopy(self,right);
}
static int _deexast_tp_move_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeXAstObject *self, DeeXAstObject *right) {
 DeeXAst_InitMove(self,right);
 return 0;
}
static int _deexast_tp_copy_assign(DeeXAstObject *self, DeeXAstObject *right) {
 return DeeXAst_AssignCopy(self,right);
}
static int _deexast_tp_move_assign(DeeXAstObject *self, DeeXAstObject *right) {
 DeeXAst_AssignMove(self,right);
 return 0;
}
DEE_COMPILER_MSVC_WARNING_PUSH(4061 4062)
static void _DeeXAst_DestroyUncommon(DeeXAstObject *self) {
 switch (self->ast_kind) {
  case DEE_XASTKIND_CONST:            _DeeXAstConstAst_Quit(&self->ast_const); break;
  case DEE_XASTKIND_VAR:              _DeeXAstVarAst_Quit(&self->ast_var); break;
  case DEE_XASTKIND_VARDECL:          _DeeXAstVarDeclAst_Quit(&self->ast_vardecl); break;
  case DEE_XASTKIND_MULTIVARDECL:     _DeeXAstMultiVarDeclAst_Quit(&self->ast_multivardecl); break;
  case DEE_XASTKIND_STATEMENT:        _DeeXAstStatementAst_Quit(&self->ast_statement); break;
  case DEE_XASTKIND_IF:               _DeeXAstIfAst_Quit(&self->ast_if); break;
  case DEE_XASTKIND_FUNCTION:         _DeeXAstFunctionAst_Quit(&self->ast_function); break;
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case DEE_XASTKIND_CLASS:            _DeeXAstClassAst_Quit(&self->ast_class); break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
  case DEE_XASTKIND_TUPLE:
  case DEE_XASTKIND_LIST:
  case DEE_XASTKIND_SET:              _DeeXAstSequenceAst_Quit(&self->ast_sequence); break;
  case DEE_XASTKIND_DICT:             _DeeXAstDictAst_Quit(&self->ast_dict); break;
  case DEE_XASTKIND_SEQ_RANGE_GET:
  case DEE_XASTKIND_SEQ_RANGE_DEL:
  case DEE_XASTKIND_SEQ_RANGE_SET:    _DeeXAstSeqRangeAst_Quit(&self->ast_seq_range); break;
  case DEE_XASTKIND_ATTR_GET_C:
  case DEE_XASTKIND_ATTR_HAS_C:
  case DEE_XASTKIND_ATTR_DEL_C:
  case DEE_XASTKIND_ATTR_SET_C:       _DeeXAstAttrCAst_Quit(&self->ast_attr_c); break;
  case DEE_XASTKIND_MODULE:           _DeeXAstModuleAst_Quit(&self->ast_module); break;
  case DEE_XASTKIND_DEL_VAR:          _DeeXAstDelVarAst_Quit(&self->ast_delvar); break;
#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
  case DEE_XASTKIND_FOREIGN_FUNCTION: _DeeXAstForeignFunctionAst_Quit(&self->ast_foreignfunction); break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */
  case DEE_XASTKIND_SWITCH:           _DeeXAstSwitchAst_Quit(&self->ast_switch); break;
  case DEE_XASTKIND_IFTRUE:
  case DEE_XASTKIND_IFFALSE:          _DeeXAstIfConstAst_Quit(&self->ast_ifconst); break;
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case DEE_XASTKIND_BUILTIN_BOUND:    _DeeXAstBuiltinBoundAst_Quit(&self->ast_builtin_bound); break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */
#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
  case DEE_XASTKIND_BUILTIN_EXTERN:   _DeeXAstBuiltinExternAst_Quit(&self->ast_builtin_extern); break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
  case DEE_XASTKIND_BUILTIN_EXPECT:   _DeeXAstBuiltinExpectAst_Quit(&self->ast_builtin_expect); break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */
  DEE_XASTKIND_CASE_INPLACE_VAR_UNARY _DeeXAstUnaryInplaceVarAst_Quit(&self->ast_unary_var); break;
  DEE_XASTKIND_CASE_INPLACE_VAR_BINARY _DeeXAstBinaryInplaceVarAst_Quit(&self->ast_binary_var); break;
  default:                            _DeeXAstOperatorAst_Quit(&self->ast_operator); break;
 }
}
static void _deexast_tp_dtor(DeeXAstObject *self) {
 _DeeXAstCommonAst_Quit(&self->ast_common);
 _DeeXAst_DestroyUncommon(self);
}
DEE_VISIT_PROC(_deexast_tp_visit,DeeXAstObject *self) {
 _DeeXAstCommonAst_Visit(&self->ast_common);
 switch (self->ast_kind) {
  case DEE_XASTKIND_CONST:            _DeeXAstConstAst_Visit(&self->ast_const); break;
  case DEE_XASTKIND_VAR:              _DeeXAstVarAst_Visit(&self->ast_var); break;
  case DEE_XASTKIND_VARDECL:          _DeeXAstVarDeclAst_Visit(&self->ast_vardecl); break;
  case DEE_XASTKIND_MULTIVARDECL:     _DeeXAstMultiVarDeclAst_Visit(&self->ast_multivardecl); break;
  case DEE_XASTKIND_STATEMENT:        _DeeXAstStatementAst_Visit(&self->ast_statement); break;
  case DEE_XASTKIND_IF:               _DeeXAstIfAst_Visit(&self->ast_if); break;
  case DEE_XASTKIND_FUNCTION:         _DeeXAstFunctionAst_Visit(&self->ast_function); break;
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case DEE_XASTKIND_CLASS:            _DeeXAstClassAst_Visit(&self->ast_class); break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
  case DEE_XASTKIND_TUPLE:
  case DEE_XASTKIND_LIST:
  case DEE_XASTKIND_SET:              _DeeXAstSequenceAst_Visit(&self->ast_sequence); break;
  case DEE_XASTKIND_DICT:             _DeeXAstDictAst_Visit(&self->ast_dict); break;
  case DEE_XASTKIND_SEQ_RANGE_GET:
  case DEE_XASTKIND_SEQ_RANGE_DEL:
  case DEE_XASTKIND_SEQ_RANGE_SET:    _DeeXAstSeqRangeAst_Visit(&self->ast_seq_range); break;
  case DEE_XASTKIND_ATTR_GET_C:
  case DEE_XASTKIND_ATTR_HAS_C:
  case DEE_XASTKIND_ATTR_DEL_C:
  case DEE_XASTKIND_ATTR_SET_C:       _DeeXAstAttrCAst_Visit(&self->ast_attr_c); break;
  case DEE_XASTKIND_MODULE:           _DeeXAstModuleAst_Visit(&self->ast_module); break;
  case DEE_XASTKIND_DEL_VAR:          _DeeXAstDelVarAst_Visit(&self->ast_delvar); break;
#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
  case DEE_XASTKIND_FOREIGN_FUNCTION: _DeeXAstForeignFunctionAst_Visit(&self->ast_foreignfunction); break;
#endif
  case DEE_XASTKIND_SWITCH:           _DeeXAstSwitchAst_Visit(&self->ast_switch); break;
  case DEE_XASTKIND_IFTRUE:
  case DEE_XASTKIND_IFFALSE:          _DeeXAstIfConstAst_Visit(&self->ast_ifconst); break;
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case DEE_XASTKIND_BUILTIN_BOUND:    _DeeXAstBuiltinBoundAst_Visit(&self->ast_builtin_bound); break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */
#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
  case DEE_XASTKIND_BUILTIN_EXTERN:   _DeeXAstBuiltinExternAst_Visit(&self->ast_builtin_extern); break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
  case DEE_XASTKIND_BUILTIN_EXPECT:   _DeeXAstBuiltinExpectAst_Visit(&self->ast_builtin_expect); break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */
  DEE_XASTKIND_CASE_INPLACE_VAR_UNARY _DeeXAstUnaryInplaceVarAst_Visit(&self->ast_unary_var); break;
  DEE_XASTKIND_CASE_INPLACE_VAR_BINARY _DeeXAstBinaryInplaceVarAst_Visit(&self->ast_binary_var); break;
  default:                            _DeeXAstOperatorAst_Visit(&self->ast_operator); break;
 }
}
DEE_COMPILER_MSVC_WARNING_POP


static DeeXAstObject *_deexast_tp_alloc(DeeTypeObject *tp_self) {
 DeeXAstObject *result = DEE_OBJECTPOOL_ALLOC(xast);
 if (result) DeeObject_INIT(result,tp_self);
 return result;
}
static void _deexast_tp_free(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeXAstObject *ob) {
 DEE_OBJECTPOOL_FREE(xast,ob);
}

static DeeObject *_deexast_tp_cmp_eq(
 DeeXAstObject *self, DeeXAstObject *right) {
 int result;
 if DEE_UNLIKELY((right = (DeeXAstObject *)DeeObject_GetInstance((
  DeeObject *)right,&DeeXAst_Type)) == NULL) return NULL;
 if DEE_UNLIKELY((result = DeeXAst_Equals(self,right)) < 0) return NULL;
 DeeReturn_Bool(result);
}
static DeeObject *_deexast_tp_cmp_ne(
 DeeXAstObject *self, DeeXAstObject *right) {
 int result;
 if DEE_UNLIKELY((right = (DeeXAstObject *)DeeObject_GetInstance((
  DeeObject *)right,&DeeXAst_Type)) == NULL) return NULL;
 if DEE_UNLIKELY((result = DeeXAst_Equals(self,right)) < 0) return NULL;
 DeeReturn_Bool(!result);
}

// v implemented later in "xast_repr.inl"
static DeeObject *_deexast_tp_repr(DeeXAstObject *self);


static DeeObject *_deexast_attr(
 DeeXAstObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint32_t flags = DEE_AST_ATTRIBUTE_FLAG_NONE;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|I32u:attr",&flags) != 0) return NULL;
 return DeeObject_New(DeeAstAttribute,DeeXAst_Attr(self,flags));
}
static DeeObject *_deexast_is_noreturn(
 DeeXAstObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 Dee_uint32_t flags = DEE_AST_ATTRIBUTE_FLAG_NONE;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|I32u:is_noreturn",&flags) != 0) return NULL;
 DeeReturn_Bool(DeeXAst_IsNoReturn(self,flags));
}
static DeeObject *_deexast_is_noeffect(
 DeeXAstObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":is_noeffect") != 0) return NULL;
 DeeReturn_Bool(DeeXAst_IsNoEffect(self));
}
static DeeObject *_deexast_is_noexcept(
 DeeXAstObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":is_noexcept") != 0) return NULL;
 DeeReturn_Bool(DeeXAst_IsNoExcept(self));
}
static DeeObject *_deexast_is_tuple(
 DeeXAstObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":is_tuple") != 0) return NULL;
 DeeReturn_Bool(DeeXAst_IsTuple(self));
}
static DeeObject *_deexast_is_list(
 DeeXAstObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":is_list") != 0) return NULL;
 DeeReturn_Bool(DeeXAst_IsList(self));
}
static DeeObject *_deexast_is_set(
 DeeXAstObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":is_set") != 0) return NULL;
 DeeReturn_Bool(DeeXAst_IsSet(self));
}
static DeeObject *_deexast_is_dict(
 DeeXAstObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":is_dict") != 0) return NULL;
 DeeReturn_Bool(DeeXAst_IsDict(self));
}
static DeeObject *_deexast_tuple_size(
 DeeXAstObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":tuple_size") != 0) return NULL;
 return DeeObject_New(Dee_size_t,DeeXAst_TupleSize(self));
}
static DeeObject *_deexast_list_size(
 DeeXAstObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":list_size") != 0) return NULL;
 return DeeObject_New(Dee_size_t,DeeXAst_ListSize(self));
}
static DeeObject *_deexast_set_size(
 DeeXAstObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":set_size") != 0) return NULL;
 return DeeObject_New(Dee_size_t,DeeXAst_SetSize(self));
}

struct _DeeXAst_VisitClosureData {
 DeeObject *visit_sast;
 DeeObject *visit_xast;
};
static int _DeeXAst_VisitXClosure(DeeXAstObject *self, struct _DeeXAst_VisitClosureData *data) {
 DeeObject *temp;
 if DEE_UNLIKELY((temp = DeeObject_Callf(data->visit_xast,"o",self)) == NULL) return -1;
 Dee_DECREF(temp);
 return 0;
}
static int _DeeXAst_VisitSClosure(DeeXAstObject *self, struct _DeeXAst_VisitClosureData *data) {
 DeeObject *temp;
 if DEE_UNLIKELY((temp = DeeObject_Callf(data->visit_sast,"o",self)) == NULL) return -1;
 Dee_DECREF(temp);
 return 0;
}
static DeeObject *_deexast_visit_all(
 DeeXAstObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 struct _DeeXAst_VisitClosureData data = {Dee_None,Dee_None};
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|oo:visit_all",&data.visit_sast,&data.visit_xast) != 0) return NULL;
 if DEE_UNLIKELY(DeeXAst_VisitAll(self,(DeeXAstVisitProc)&_DeeXAst_VisitXClosure,(DeeSAstVisitProc)&_DeeXAst_VisitSClosure,&data) != 0) return NULL;
 DeeReturn_None;
}
static DeeXAstObject *_deexast_copy_with_scope(
 DeeXAstObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeScopeObject *scope; DeeLexerObject *lexer;
 if (DeeTuple_Unpack(args,"oo:scope_with_scope",&scope,&lexer) != 0) return NULL;
 if DEE_UNLIKELY((scope = (DeeScopeObject *)DeeObject_GetInstance((DeeObject *)scope,&DeeScope_Type)) == NULL) return NULL;
 if DEE_UNLIKELY((lexer = (DeeLexerObject *)DeeObject_GetInstance((DeeObject *)lexer,&DeeLexer_Type)) == NULL) return NULL;
 return DeeXAst_CopyWithScope(self,scope,lexer);
}
static DeeObject *_deexast_uses_variable(
 DeeXAstObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeLocalVarObject *check_var;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:variable_usage",&check_var) != 0) return NULL;
 if DEE_UNLIKELY((check_var = (DeeLocalVarObject *)DeeObject_GetInstance((
  DeeObject *)check_var,&DeeLocalVar_Type)) == NULL) return NULL;
 DeeReturn_Bool(DeeXAst_LoadsVariable(self,check_var));
}
static DeeObject *_deexast_count_variable_uses(
 DeeXAstObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeLocalVarObject *check_var;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:count_variable_uses",&check_var) != 0) return NULL;
 if DEE_UNLIKELY((check_var = (DeeLocalVarObject *)DeeObject_GetInstance((
  DeeObject *)check_var,&DeeLocalVar_Type)) == NULL) return NULL;
 return DeeObject_New(Dee_size_t,DeeXAst_CountVariableLoads(self,check_var));
}
static DeeObject *_deexast_has_labels(
 DeeXAstObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":has_labels") != 0) return NULL;
 DeeReturn_Bool(DeeXAst_HasLabels(self));
}
static DeeObject *_deexast_assign_const(
 DeeXAstObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *const_value = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|o:assign_const") != 0) return NULL;
 DeeXAst_AssignConst(self,const_value);
 DeeReturn_None;
}

static struct DeeMethodDef const _deexast_tp_methods[] = {
 DEE_METHODDEF_v100("attr",member(&_deexast_attr),"attr(uint32_t flags = " DEE_PP_STR(DEE_AST_ATTRIBUTE_NONE) ") -> DeeAstAttribute"),
 DEE_METHODDEF_v100("is_noreturn",member(&_deexast_is_noreturn),"is_noreturn(uint32_t flags = " DEE_PP_STR(DEE_AST_ATTRIBUTE_NONE) ") -> bool"),
 DEE_METHODDEF_v100("is_noeffect",member(&_deexast_is_noeffect),"is_noeffect() -> bool"),
 DEE_METHODDEF_v100("is_noexcept",member(&_deexast_is_noexcept),"is_noexcept() -> bool"),
 DEE_METHODDEF_v100("is_tuple",member(&_deexast_is_tuple),"is_tuple() -> bool"),
 DEE_METHODDEF_v100("is_list",member(&_deexast_is_list),"is_list() -> bool"),
 DEE_METHODDEF_v100("is_set",member(&_deexast_is_set),"is_set() -> bool"),
 DEE_METHODDEF_v100("is_dict",member(&_deexast_is_dict),"is_dict() -> bool"),
 DEE_METHODDEF_v100("tuple_size",member(&_deexast_tuple_size),"tuple_size() -> size_t"),
 DEE_METHODDEF_v100("list_size",member(&_deexast_list_size),"list_size() -> size_t"),
 DEE_METHODDEF_v100("set_size",member(&_deexast_set_size),"set_size() -> size_t"),
 DEE_METHODDEF_v100("visit_all",member(&_deexast_visit_all),"visit_all(callable visit_sast = none, callable visit_xast = none) -> none"),
 DEE_METHODDEF_v100("copy_with_scope",member(&_deexast_copy_with_scope),"copy_with_scope(scope new_scope, lexer lex) -> xast"),
 DEE_METHODDEF_v100("variable_usage",member(&_deexast_uses_variable),"variable_usage(local_var var) -> bool"),
 DEE_METHODDEF_v100("count_variable_uses",member(&_deexast_count_variable_uses),"count_variable_uses(local_var var) -> size_t"),
 DEE_METHODDEF_v100("assign_const",member(&_deexast_assign_const),"assign_const(object const_value) -> none"),
 DEE_METHODDEF_END_v100
};

static DeeXAstObject *_deexastclass_new_const(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeTokenObject *tok; DeeObject *const_value = Dee_None;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:new_const",&tok,&const_value) != 0) return NULL;
 if DEE_UNLIKELY((tok = (DeeTokenObject *)DeeObject_GetInstance((
  DeeObject *)tok,&DeeToken_Type)) == NULL) return NULL;
 return DeeXAst_NewConst(tok,const_value);
}
static DeeXAstObject *_deexastclass_new_var(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeTokenObject *tok; DeeLocalVarObject *local_var; DeeScopeObject *curr_scope;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"ooo:new_new",&tok,&local_var,&curr_scope) != 0) return NULL;
 if DEE_UNLIKELY((tok = (DeeTokenObject *)DeeObject_GetInstance((DeeObject *)tok,&DeeToken_Type)) == NULL) return NULL;
 if DEE_UNLIKELY((local_var = (DeeLocalVarObject *)DeeObject_GetInstance((DeeObject *)local_var,&DeeLocalVar_Type)) == NULL) return NULL;
 if DEE_UNLIKELY((curr_scope = (DeeScopeObject *)DeeObject_GetInstance((DeeObject *)curr_scope,&DeeScope_Type)) == NULL) return NULL;
 return DeeXAst_NewVar(tok,local_var,curr_scope);
}
static DeeXAstObject *_deexastclass_new_zeroop(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeXAstKind kind; DeeTokenObject *tok;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"uo:new_zeroop",&kind,&tok) != 0) return NULL;
 if DEE_UNLIKELY((tok = (DeeTokenObject *)DeeObject_GetInstance((DeeObject *)tok,&DeeToken_Type)) == NULL) return NULL;
 if DEE_UNLIKELY(!DEE_XASTKIND_ISZEROOP(kind)) { DeeError_SetStringf(&DeeErrorType_ValueError,"Invalid zeroop xast kind: %d",kind); return NULL; }
 return DeeXAst_NewZeroOp(kind,tok);
}
static DeeXAstObject *_deexastclass_new_unary(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeXAstKind kind; DeeTokenObject *tok; DeeLexerObject *lexer;
 Dee_uint32_t parser_flags; DeeXAstObject *ast_a;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"uooI32uo:new_unary",&kind,&tok,&lexer,&parser_flags,&ast_a) != 0) return NULL;
 if DEE_UNLIKELY((tok = (DeeTokenObject *)DeeObject_GetInstance((DeeObject *)tok,&DeeToken_Type)) == NULL) return NULL;
 if DEE_UNLIKELY((lexer = (DeeLexerObject *)DeeObject_GetInstance((DeeObject *)lexer,&DeeLexer_Type)) == NULL) return NULL;
 if DEE_UNLIKELY((ast_a = (DeeXAstObject *)DeeObject_GetInstance((DeeObject *)ast_a,&DeeXAst_Type)) == NULL) return NULL;
 if DEE_UNLIKELY(!DEE_XASTKIND_ISUNARY(kind)) { DeeError_SetStringf(&DeeErrorType_ValueError,"Invalid unary xast kind: %d",kind); return NULL; }
 return DeeXAst_NewUnary(kind,tok,lexer,parser_flags,ast_a);
}
static DeeXAstObject *_deexastclass_new_binary(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeXAstKind kind; DeeTokenObject *tok; DeeLexerObject *lexer;
 Dee_uint32_t parser_flags; DeeXAstObject *ast_a,*ast_b;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"uooI32uoo:new_binary",&kind,&tok,&lexer,&parser_flags,&ast_a,&ast_b) != 0) return NULL;
 if DEE_UNLIKELY((tok = (DeeTokenObject *)DeeObject_GetInstance((DeeObject *)tok,&DeeToken_Type)) == NULL) return NULL;
 if DEE_UNLIKELY((lexer = (DeeLexerObject *)DeeObject_GetInstance((DeeObject *)lexer,&DeeLexer_Type)) == NULL) return NULL;
 if DEE_UNLIKELY((ast_a = (DeeXAstObject *)DeeObject_GetInstance((DeeObject *)ast_a,&DeeXAst_Type)) == NULL) return NULL;
 if DEE_UNLIKELY((ast_b = (DeeXAstObject *)DeeObject_GetInstance((DeeObject *)ast_b,&DeeXAst_Type)) == NULL) return NULL;
 if DEE_UNLIKELY(!DEE_XASTKIND_ISBINARY(kind)) { DeeError_SetStringf(&DeeErrorType_ValueError,"Invalid binary xast kind: %d",kind); return NULL; }
 return DeeXAst_NewBinary(kind,tok,lexer,parser_flags,ast_a,ast_b);
}
static DeeXAstObject *_deexastclass_new_trinary(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeXAstKind kind; DeeTokenObject *tok; DeeLexerObject *lexer;
 Dee_uint32_t parser_flags; DeeXAstObject *ast_a,*ast_b,*ast_c;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"uooI32uooo:new_unary",&kind,&tok,&lexer,&parser_flags,&ast_a,&ast_b,&ast_c) != 0) return NULL;
 if DEE_UNLIKELY((tok = (DeeTokenObject *)DeeObject_GetInstance((DeeObject *)tok,&DeeToken_Type)) == NULL) return NULL;
 if DEE_UNLIKELY((lexer = (DeeLexerObject *)DeeObject_GetInstance((DeeObject *)lexer,&DeeLexer_Type)) == NULL) return NULL;
 if DEE_UNLIKELY((ast_a = (DeeXAstObject *)DeeObject_GetInstance((DeeObject *)ast_a,&DeeXAst_Type)) == NULL) return NULL;
 if DEE_UNLIKELY((ast_b = (DeeXAstObject *)DeeObject_GetInstance((DeeObject *)ast_b,&DeeXAst_Type)) == NULL) return NULL;
 if DEE_UNLIKELY((ast_c = (DeeXAstObject *)DeeObject_GetInstance((DeeObject *)ast_c,&DeeXAst_Type)) == NULL) return NULL;
 if DEE_UNLIKELY(!DEE_XASTKIND_ISTRINARY(kind)) { DeeError_SetStringf(&DeeErrorType_ValueError,"Invalid trinary xast kind: %d",kind); return NULL; }
 return DeeXAst_NewTrinary(kind,tok,lexer,parser_flags,ast_a,ast_b,ast_c);
}

static struct DeeMethodDef const _deexast_tp_class_methods[] = {
 DEE_METHODDEF_v100("new_const",member(&_deexastclass_new_const),"(object const_value) -> xast"),
 DEE_METHODDEF_v100("new_var",member(&_deexastclass_new_var),"(local_var var, scope curr_scope) -> xast"),
 DEE_METHODDEF_v100("new_zeroop",member(&_deexastclass_new_zeroop),"(unsigned int kind, token tk) -> xast"),
 DEE_METHODDEF_v100("new_unary",member(&_deexastclass_new_unary),"(unsigned int kind, token tk, lexer lex, uint32_t parser_flags, xast ast_a) -> xast"),
 DEE_METHODDEF_v100("new_binary",member(&_deexastclass_new_binary),"(unsigned int kind, token tk, lexer lex, uint32_t parser_flags, xast ast_a, xast ast_b) -> xast"),
 DEE_METHODDEF_v100("new_trinary",member(&_deexastclass_new_trinary),"(unsigned int kind, token tk, lexer lex, uint32_t parser_flags, xast ast_a, xast ast_b, xast ast_c) -> xast"),
 DEE_METHODDEF_END_v100
};

DEE_COMPILER_MSVC_WARNING_PUSH(4574)
static struct DeeMemberDef const _deexast_tp_class_members[] = {
/*[[[deemon
#include <file>
const prefix = "#define DEE_XASTKIND_";
const max_line_length = 100;
for (local name: file.io("xast.h")) {
  name = name.strip();
  if (name.startswith(prefix)) {
    name = name[#prefix:];
    if (name.startswith("CASE")) continue;
    name = name.partition(" ")[0];
    if ("(" in name) continue;
    print "#ifdef DEE_XASTKIND_"+name;
    print " DEE_MEMBERDEF_CONST_v100("+repr("KIND_"+name)+",DeeXAstKind,DEE_XASTKIND_"+name+"),";
    print "#endif";
  }
}
]]]*/
#ifdef DEE_XASTKIND_NONE
 DEE_MEMBERDEF_CONST_v100("KIND_NONE",DeeXAstKind,DEE_XASTKIND_NONE),
#endif
#ifdef DEE_XASTKIND_CONST
 DEE_MEMBERDEF_CONST_v100("KIND_CONST",DeeXAstKind,DEE_XASTKIND_CONST),
#endif
#ifdef DEE_XASTKIND_VAR
 DEE_MEMBERDEF_CONST_v100("KIND_VAR",DeeXAstKind,DEE_XASTKIND_VAR),
#endif
#ifdef DEE_XASTKIND_VARDECL
 DEE_MEMBERDEF_CONST_v100("KIND_VARDECL",DeeXAstKind,DEE_XASTKIND_VARDECL),
#endif
#ifdef DEE_XASTKIND_MULTIVARDECL
 DEE_MEMBERDEF_CONST_v100("KIND_MULTIVARDECL",DeeXAstKind,DEE_XASTKIND_MULTIVARDECL),
#endif
#ifdef DEE_XASTKIND_STATEMENT
 DEE_MEMBERDEF_CONST_v100("KIND_STATEMENT",DeeXAstKind,DEE_XASTKIND_STATEMENT),
#endif
#ifdef DEE_XASTKIND_IF
 DEE_MEMBERDEF_CONST_v100("KIND_IF",DeeXAstKind,DEE_XASTKIND_IF),
#endif
#ifdef DEE_XASTKIND_FUNCTION
 DEE_MEMBERDEF_CONST_v100("KIND_FUNCTION",DeeXAstKind,DEE_XASTKIND_FUNCTION),
#endif
#ifdef DEE_XASTKIND_CLASS
 DEE_MEMBERDEF_CONST_v100("KIND_CLASS",DeeXAstKind,DEE_XASTKIND_CLASS),
#endif
#ifdef DEE_XASTKIND_TUPLE
 DEE_MEMBERDEF_CONST_v100("KIND_TUPLE",DeeXAstKind,DEE_XASTKIND_TUPLE),
#endif
#ifdef DEE_XASTKIND_LIST
 DEE_MEMBERDEF_CONST_v100("KIND_LIST",DeeXAstKind,DEE_XASTKIND_LIST),
#endif
#ifdef DEE_XASTKIND_SET
 DEE_MEMBERDEF_CONST_v100("KIND_SET",DeeXAstKind,DEE_XASTKIND_SET),
#endif
#ifdef DEE_XASTKIND_DICT
 DEE_MEMBERDEF_CONST_v100("KIND_DICT",DeeXAstKind,DEE_XASTKIND_DICT),
#endif
#ifdef DEE_XASTKIND_SEQ_RANGE_GET
 DEE_MEMBERDEF_CONST_v100("KIND_SEQ_RANGE_GET",DeeXAstKind,DEE_XASTKIND_SEQ_RANGE_GET),
#endif
#ifdef DEE_XASTKIND_SEQ_RANGE_DEL
 DEE_MEMBERDEF_CONST_v100("KIND_SEQ_RANGE_DEL",DeeXAstKind,DEE_XASTKIND_SEQ_RANGE_DEL),
#endif
#ifdef DEE_XASTKIND_SEQ_RANGE_SET
 DEE_MEMBERDEF_CONST_v100("KIND_SEQ_RANGE_SET",DeeXAstKind,DEE_XASTKIND_SEQ_RANGE_SET),
#endif
#ifdef DEE_XASTKIND_ATTR_GET_C
 DEE_MEMBERDEF_CONST_v100("KIND_ATTR_GET_C",DeeXAstKind,DEE_XASTKIND_ATTR_GET_C),
#endif
#ifdef DEE_XASTKIND_ATTR_HAS_C
 DEE_MEMBERDEF_CONST_v100("KIND_ATTR_HAS_C",DeeXAstKind,DEE_XASTKIND_ATTR_HAS_C),
#endif
#ifdef DEE_XASTKIND_ATTR_DEL_C
 DEE_MEMBERDEF_CONST_v100("KIND_ATTR_DEL_C",DeeXAstKind,DEE_XASTKIND_ATTR_DEL_C),
#endif
#ifdef DEE_XASTKIND_ATTR_SET_C
 DEE_MEMBERDEF_CONST_v100("KIND_ATTR_SET_C",DeeXAstKind,DEE_XASTKIND_ATTR_SET_C),
#endif
#ifdef DEE_XASTKIND_MODULE
 DEE_MEMBERDEF_CONST_v100("KIND_MODULE",DeeXAstKind,DEE_XASTKIND_MODULE),
#endif
#ifdef DEE_XASTKIND_DEL_VAR
 DEE_MEMBERDEF_CONST_v100("KIND_DEL_VAR",DeeXAstKind,DEE_XASTKIND_DEL_VAR),
#endif
#ifdef DEE_XASTKIND_FOREIGN_FUNCTION
 DEE_MEMBERDEF_CONST_v100("KIND_FOREIGN_FUNCTION",DeeXAstKind,DEE_XASTKIND_FOREIGN_FUNCTION),
#endif
#ifdef DEE_XASTKIND_SWITCH
 DEE_MEMBERDEF_CONST_v100("KIND_SWITCH",DeeXAstKind,DEE_XASTKIND_SWITCH),
#endif
#ifdef DEE_XASTKIND_IFTRUE
 DEE_MEMBERDEF_CONST_v100("KIND_IFTRUE",DeeXAstKind,DEE_XASTKIND_IFTRUE),
#endif
#ifdef DEE_XASTKIND_IFFALSE
 DEE_MEMBERDEF_CONST_v100("KIND_IFFALSE",DeeXAstKind,DEE_XASTKIND_IFFALSE),
#endif
#ifdef DEE_XASTKIND_STR
 DEE_MEMBERDEF_CONST_v100("KIND_STR",DeeXAstKind,DEE_XASTKIND_STR),
#endif
#ifdef DEE_XASTKIND_REPR
 DEE_MEMBERDEF_CONST_v100("KIND_REPR",DeeXAstKind,DEE_XASTKIND_REPR),
#endif
#ifdef DEE_XASTKIND_COPY
 DEE_MEMBERDEF_CONST_v100("KIND_COPY",DeeXAstKind,DEE_XASTKIND_COPY),
#endif
#ifdef DEE_XASTKIND_MOVE
 DEE_MEMBERDEF_CONST_v100("KIND_MOVE",DeeXAstKind,DEE_XASTKIND_MOVE),
#endif
#ifdef DEE_XASTKIND_TYPE
 DEE_MEMBERDEF_CONST_v100("KIND_TYPE",DeeXAstKind,DEE_XASTKIND_TYPE),
#endif
#ifdef DEE_XASTKIND_WEAK
 DEE_MEMBERDEF_CONST_v100("KIND_WEAK",DeeXAstKind,DEE_XASTKIND_WEAK),
#endif
#ifdef DEE_XASTKIND_BOOL
 DEE_MEMBERDEF_CONST_v100("KIND_BOOL",DeeXAstKind,DEE_XASTKIND_BOOL),
#endif
#ifdef DEE_XASTKIND_SUPEROF
 DEE_MEMBERDEF_CONST_v100("KIND_SUPEROF",DeeXAstKind,DEE_XASTKIND_SUPEROF),
#endif
#ifdef DEE_XASTKIND_CLASSOF
 DEE_MEMBERDEF_CONST_v100("KIND_CLASSOF",DeeXAstKind,DEE_XASTKIND_CLASSOF),
#endif
#ifdef DEE_XASTKIND_REF
 DEE_MEMBERDEF_CONST_v100("KIND_REF",DeeXAstKind,DEE_XASTKIND_REF),
#endif
#ifdef DEE_XASTKIND_DEREF
 DEE_MEMBERDEF_CONST_v100("KIND_DEREF",DeeXAstKind,DEE_XASTKIND_DEREF),
#endif
#ifdef DEE_XASTKIND_PTROF
 DEE_MEMBERDEF_CONST_v100("KIND_PTROF",DeeXAstKind,DEE_XASTKIND_PTROF),
#endif
#ifdef DEE_XASTKIND_LVALOF
 DEE_MEMBERDEF_CONST_v100("KIND_LVALOF",DeeXAstKind,DEE_XASTKIND_LVALOF),
#endif
#ifdef DEE_XASTKIND_NOT
 DEE_MEMBERDEF_CONST_v100("KIND_NOT",DeeXAstKind,DEE_XASTKIND_NOT),
#endif
#ifdef DEE_XASTKIND_NEG
 DEE_MEMBERDEF_CONST_v100("KIND_NEG",DeeXAstKind,DEE_XASTKIND_NEG),
#endif
#ifdef DEE_XASTKIND_POS
 DEE_MEMBERDEF_CONST_v100("KIND_POS",DeeXAstKind,DEE_XASTKIND_POS),
#endif
#ifdef DEE_XASTKIND_INV
 DEE_MEMBERDEF_CONST_v100("KIND_INV",DeeXAstKind,DEE_XASTKIND_INV),
#endif
#ifdef DEE_XASTKIND_INC
 DEE_MEMBERDEF_CONST_v100("KIND_INC",DeeXAstKind,DEE_XASTKIND_INC),
#endif
#ifdef DEE_XASTKIND_DEC
 DEE_MEMBERDEF_CONST_v100("KIND_DEC",DeeXAstKind,DEE_XASTKIND_DEC),
#endif
#ifdef DEE_XASTKIND_INCPOST
 DEE_MEMBERDEF_CONST_v100("KIND_INCPOST",DeeXAstKind,DEE_XASTKIND_INCPOST),
#endif
#ifdef DEE_XASTKIND_DECPOST
 DEE_MEMBERDEF_CONST_v100("KIND_DECPOST",DeeXAstKind,DEE_XASTKIND_DECPOST),
#endif
#ifdef DEE_XASTKIND_EXPAND
 DEE_MEMBERDEF_CONST_v100("KIND_EXPAND",DeeXAstKind,DEE_XASTKIND_EXPAND),
#endif
#ifdef DEE_XASTKIND_SEQ_SIZE
 DEE_MEMBERDEF_CONST_v100("KIND_SEQ_SIZE",DeeXAstKind,DEE_XASTKIND_SEQ_SIZE),
#endif
#ifdef DEE_XASTKIND_NEW
 DEE_MEMBERDEF_CONST_v100("KIND_NEW",DeeXAstKind,DEE_XASTKIND_NEW),
#endif
#ifdef DEE_XASTKIND_DELETE
 DEE_MEMBERDEF_CONST_v100("KIND_DELETE",DeeXAstKind,DEE_XASTKIND_DELETE),
#endif
#ifdef DEE_XASTKIND_ITERSELF
 DEE_MEMBERDEF_CONST_v100("KIND_ITERSELF",DeeXAstKind,DEE_XASTKIND_ITERSELF),
#endif
#ifdef DEE_XASTKIND_ITERNEXT
 DEE_MEMBERDEF_CONST_v100("KIND_ITERNEXT",DeeXAstKind,DEE_XASTKIND_ITERNEXT),
#endif
#ifdef DEE_XASTKIND_CELL
 DEE_MEMBERDEF_CONST_v100("KIND_CELL",DeeXAstKind,DEE_XASTKIND_CELL),
#endif
#ifdef DEE_XASTKIND_VARRAYOF
 DEE_MEMBERDEF_CONST_v100("KIND_VARRAYOF",DeeXAstKind,DEE_XASTKIND_VARRAYOF),
#endif
#ifdef DEE_XASTKIND_CALL
 DEE_MEMBERDEF_CONST_v100("KIND_CALL",DeeXAstKind,DEE_XASTKIND_CALL),
#endif
#ifdef DEE_XASTKIND_COMPARE_LO
 DEE_MEMBERDEF_CONST_v100("KIND_COMPARE_LO",DeeXAstKind,DEE_XASTKIND_COMPARE_LO),
#endif
#ifdef DEE_XASTKIND_COMPARE_LE
 DEE_MEMBERDEF_CONST_v100("KIND_COMPARE_LE",DeeXAstKind,DEE_XASTKIND_COMPARE_LE),
#endif
#ifdef DEE_XASTKIND_COMPARE_EQ
 DEE_MEMBERDEF_CONST_v100("KIND_COMPARE_EQ",DeeXAstKind,DEE_XASTKIND_COMPARE_EQ),
#endif
#ifdef DEE_XASTKIND_COMPARE_NE
 DEE_MEMBERDEF_CONST_v100("KIND_COMPARE_NE",DeeXAstKind,DEE_XASTKIND_COMPARE_NE),
#endif
#ifdef DEE_XASTKIND_COMPARE_GR
 DEE_MEMBERDEF_CONST_v100("KIND_COMPARE_GR",DeeXAstKind,DEE_XASTKIND_COMPARE_GR),
#endif
#ifdef DEE_XASTKIND_COMPARE_GE
 DEE_MEMBERDEF_CONST_v100("KIND_COMPARE_GE",DeeXAstKind,DEE_XASTKIND_COMPARE_GE),
#endif
#ifdef DEE_XASTKIND_ADD
 DEE_MEMBERDEF_CONST_v100("KIND_ADD",DeeXAstKind,DEE_XASTKIND_ADD),
#endif
#ifdef DEE_XASTKIND_SUB
 DEE_MEMBERDEF_CONST_v100("KIND_SUB",DeeXAstKind,DEE_XASTKIND_SUB),
#endif
#ifdef DEE_XASTKIND_MUL
 DEE_MEMBERDEF_CONST_v100("KIND_MUL",DeeXAstKind,DEE_XASTKIND_MUL),
#endif
#ifdef DEE_XASTKIND_DIV
 DEE_MEMBERDEF_CONST_v100("KIND_DIV",DeeXAstKind,DEE_XASTKIND_DIV),
#endif
#ifdef DEE_XASTKIND_MOD
 DEE_MEMBERDEF_CONST_v100("KIND_MOD",DeeXAstKind,DEE_XASTKIND_MOD),
#endif
#ifdef DEE_XASTKIND_SHL
 DEE_MEMBERDEF_CONST_v100("KIND_SHL",DeeXAstKind,DEE_XASTKIND_SHL),
#endif
#ifdef DEE_XASTKIND_SHR
 DEE_MEMBERDEF_CONST_v100("KIND_SHR",DeeXAstKind,DEE_XASTKIND_SHR),
#endif
#ifdef DEE_XASTKIND_AND
 DEE_MEMBERDEF_CONST_v100("KIND_AND",DeeXAstKind,DEE_XASTKIND_AND),
#endif
#ifdef DEE_XASTKIND_OR
 DEE_MEMBERDEF_CONST_v100("KIND_OR",DeeXAstKind,DEE_XASTKIND_OR),
#endif
#ifdef DEE_XASTKIND_XOR
 DEE_MEMBERDEF_CONST_v100("KIND_XOR",DeeXAstKind,DEE_XASTKIND_XOR),
#endif
#ifdef DEE_XASTKIND_POW
 DEE_MEMBERDEF_CONST_v100("KIND_POW",DeeXAstKind,DEE_XASTKIND_POW),
#endif
#ifdef DEE_XASTKIND_IADD
 DEE_MEMBERDEF_CONST_v100("KIND_IADD",DeeXAstKind,DEE_XASTKIND_IADD),
#endif
#ifdef DEE_XASTKIND_ISUB
 DEE_MEMBERDEF_CONST_v100("KIND_ISUB",DeeXAstKind,DEE_XASTKIND_ISUB),
#endif
#ifdef DEE_XASTKIND_IMUL
 DEE_MEMBERDEF_CONST_v100("KIND_IMUL",DeeXAstKind,DEE_XASTKIND_IMUL),
#endif
#ifdef DEE_XASTKIND_IDIV
 DEE_MEMBERDEF_CONST_v100("KIND_IDIV",DeeXAstKind,DEE_XASTKIND_IDIV),
#endif
#ifdef DEE_XASTKIND_IMOD
 DEE_MEMBERDEF_CONST_v100("KIND_IMOD",DeeXAstKind,DEE_XASTKIND_IMOD),
#endif
#ifdef DEE_XASTKIND_ISHL
 DEE_MEMBERDEF_CONST_v100("KIND_ISHL",DeeXAstKind,DEE_XASTKIND_ISHL),
#endif
#ifdef DEE_XASTKIND_ISHR
 DEE_MEMBERDEF_CONST_v100("KIND_ISHR",DeeXAstKind,DEE_XASTKIND_ISHR),
#endif
#ifdef DEE_XASTKIND_IAND
 DEE_MEMBERDEF_CONST_v100("KIND_IAND",DeeXAstKind,DEE_XASTKIND_IAND),
#endif
#ifdef DEE_XASTKIND_IOR
 DEE_MEMBERDEF_CONST_v100("KIND_IOR",DeeXAstKind,DEE_XASTKIND_IOR),
#endif
#ifdef DEE_XASTKIND_IXOR
 DEE_MEMBERDEF_CONST_v100("KIND_IXOR",DeeXAstKind,DEE_XASTKIND_IXOR),
#endif
#ifdef DEE_XASTKIND_IPOW
 DEE_MEMBERDEF_CONST_v100("KIND_IPOW",DeeXAstKind,DEE_XASTKIND_IPOW),
#endif
#ifdef DEE_XASTKIND_SEQ_GET
 DEE_MEMBERDEF_CONST_v100("KIND_SEQ_GET",DeeXAstKind,DEE_XASTKIND_SEQ_GET),
#endif
#ifdef DEE_XASTKIND_SEQ_DEL
 DEE_MEMBERDEF_CONST_v100("KIND_SEQ_DEL",DeeXAstKind,DEE_XASTKIND_SEQ_DEL),
#endif
#ifdef DEE_XASTKIND_ATTR_GET
 DEE_MEMBERDEF_CONST_v100("KIND_ATTR_GET",DeeXAstKind,DEE_XASTKIND_ATTR_GET),
#endif
#ifdef DEE_XASTKIND_ATTR_HAS
 DEE_MEMBERDEF_CONST_v100("KIND_ATTR_HAS",DeeXAstKind,DEE_XASTKIND_ATTR_HAS),
#endif
#ifdef DEE_XASTKIND_ATTR_DEL
 DEE_MEMBERDEF_CONST_v100("KIND_ATTR_DEL",DeeXAstKind,DEE_XASTKIND_ATTR_DEL),
#endif
#ifdef DEE_XASTKIND_IS
 DEE_MEMBERDEF_CONST_v100("KIND_IS",DeeXAstKind,DEE_XASTKIND_IS),
#endif
#ifdef DEE_XASTKIND_IN
 DEE_MEMBERDEF_CONST_v100("KIND_IN",DeeXAstKind,DEE_XASTKIND_IN),
#endif
#ifdef DEE_XASTKIND_NOT_IS
 DEE_MEMBERDEF_CONST_v100("KIND_NOT_IS",DeeXAstKind,DEE_XASTKIND_NOT_IS),
#endif
#ifdef DEE_XASTKIND_NOT_IN
 DEE_MEMBERDEF_CONST_v100("KIND_NOT_IN",DeeXAstKind,DEE_XASTKIND_NOT_IN),
#endif
#ifdef DEE_XASTKIND_LAND
 DEE_MEMBERDEF_CONST_v100("KIND_LAND",DeeXAstKind,DEE_XASTKIND_LAND),
#endif
#ifdef DEE_XASTKIND_LOR
 DEE_MEMBERDEF_CONST_v100("KIND_LOR",DeeXAstKind,DEE_XASTKIND_LOR),
#endif
#ifdef DEE_XASTKIND_LXOR
 DEE_MEMBERDEF_CONST_v100("KIND_LXOR",DeeXAstKind,DEE_XASTKIND_LXOR),
#endif
#ifdef DEE_XASTKIND_MOVE_ASSIGN
 DEE_MEMBERDEF_CONST_v100("KIND_MOVE_ASSIGN",DeeXAstKind,DEE_XASTKIND_MOVE_ASSIGN),
#endif
#ifdef DEE_XASTKIND_COPY_ASSIGN
 DEE_MEMBERDEF_CONST_v100("KIND_COPY_ASSIGN",DeeXAstKind,DEE_XASTKIND_COPY_ASSIGN),
#endif
#ifdef DEE_XASTKIND_SEQ_SET
 DEE_MEMBERDEF_CONST_v100("KIND_SEQ_SET",DeeXAstKind,DEE_XASTKIND_SEQ_SET),
#endif
#ifdef DEE_XASTKIND_ATTR_SET
 DEE_MEMBERDEF_CONST_v100("KIND_ATTR_SET",DeeXAstKind,DEE_XASTKIND_ATTR_SET),
#endif
#ifdef DEE_XASTKIND_IO_READNP
 DEE_MEMBERDEF_CONST_v100("KIND_IO_READNP",DeeXAstKind,DEE_XASTKIND_IO_READNP),
#endif
#ifdef DEE_XASTKIND_IO_WRITENP
 DEE_MEMBERDEF_CONST_v100("KIND_IO_WRITENP",DeeXAstKind,DEE_XASTKIND_IO_WRITENP),
#endif
#ifdef DEE_XASTKIND_IO_SEEK
 DEE_MEMBERDEF_CONST_v100("KIND_IO_SEEK",DeeXAstKind,DEE_XASTKIND_IO_SEEK),
#endif
#ifdef DEE_XASTKIND_IO_FLUSH
 DEE_MEMBERDEF_CONST_v100("KIND_IO_FLUSH",DeeXAstKind,DEE_XASTKIND_IO_FLUSH),
#endif
#ifdef DEE_XASTKIND_IO_TRUNC
 DEE_MEMBERDEF_CONST_v100("KIND_IO_TRUNC",DeeXAstKind,DEE_XASTKIND_IO_TRUNC),
#endif
#ifdef DEE_XASTKIND_IO_CLOSE
 DEE_MEMBERDEF_CONST_v100("KIND_IO_CLOSE",DeeXAstKind,DEE_XASTKIND_IO_CLOSE),
#endif
#ifdef DEE_XASTKIND_IO_READP
 DEE_MEMBERDEF_CONST_v100("KIND_IO_READP",DeeXAstKind,DEE_XASTKIND_IO_READP),
#endif
#ifdef DEE_XASTKIND_IO_WRITEP
 DEE_MEMBERDEF_CONST_v100("KIND_IO_WRITEP",DeeXAstKind,DEE_XASTKIND_IO_WRITEP),
#endif
#ifdef DEE_XASTKIND_VAR_INC
 DEE_MEMBERDEF_CONST_v100("KIND_VAR_INC",DeeXAstKind,DEE_XASTKIND_VAR_INC),
#endif
#ifdef DEE_XASTKIND_VAR_DEC
 DEE_MEMBERDEF_CONST_v100("KIND_VAR_DEC",DeeXAstKind,DEE_XASTKIND_VAR_DEC),
#endif
#ifdef DEE_XASTKIND_VAR_INCPOST
 DEE_MEMBERDEF_CONST_v100("KIND_VAR_INCPOST",DeeXAstKind,DEE_XASTKIND_VAR_INCPOST),
#endif
#ifdef DEE_XASTKIND_VAR_DECPOST
 DEE_MEMBERDEF_CONST_v100("KIND_VAR_DECPOST",DeeXAstKind,DEE_XASTKIND_VAR_DECPOST),
#endif
#ifdef DEE_XASTKIND_VAR_IADD
 DEE_MEMBERDEF_CONST_v100("KIND_VAR_IADD",DeeXAstKind,DEE_XASTKIND_VAR_IADD),
#endif
#ifdef DEE_XASTKIND_VAR_ISUB
 DEE_MEMBERDEF_CONST_v100("KIND_VAR_ISUB",DeeXAstKind,DEE_XASTKIND_VAR_ISUB),
#endif
#ifdef DEE_XASTKIND_VAR_IMUL
 DEE_MEMBERDEF_CONST_v100("KIND_VAR_IMUL",DeeXAstKind,DEE_XASTKIND_VAR_IMUL),
#endif
#ifdef DEE_XASTKIND_VAR_IDIV
 DEE_MEMBERDEF_CONST_v100("KIND_VAR_IDIV",DeeXAstKind,DEE_XASTKIND_VAR_IDIV),
#endif
#ifdef DEE_XASTKIND_VAR_IMOD
 DEE_MEMBERDEF_CONST_v100("KIND_VAR_IMOD",DeeXAstKind,DEE_XASTKIND_VAR_IMOD),
#endif
#ifdef DEE_XASTKIND_VAR_ISHL
 DEE_MEMBERDEF_CONST_v100("KIND_VAR_ISHL",DeeXAstKind,DEE_XASTKIND_VAR_ISHL),
#endif
#ifdef DEE_XASTKIND_VAR_ISHR
 DEE_MEMBERDEF_CONST_v100("KIND_VAR_ISHR",DeeXAstKind,DEE_XASTKIND_VAR_ISHR),
#endif
#ifdef DEE_XASTKIND_VAR_IAND
 DEE_MEMBERDEF_CONST_v100("KIND_VAR_IAND",DeeXAstKind,DEE_XASTKIND_VAR_IAND),
#endif
#ifdef DEE_XASTKIND_VAR_IOR
 DEE_MEMBERDEF_CONST_v100("KIND_VAR_IOR",DeeXAstKind,DEE_XASTKIND_VAR_IOR),
#endif
#ifdef DEE_XASTKIND_VAR_IXOR
 DEE_MEMBERDEF_CONST_v100("KIND_VAR_IXOR",DeeXAstKind,DEE_XASTKIND_VAR_IXOR),
#endif
#ifdef DEE_XASTKIND_VAR_IPOW
 DEE_MEMBERDEF_CONST_v100("KIND_VAR_IPOW",DeeXAstKind,DEE_XASTKIND_VAR_IPOW),
#endif
#ifdef DEE_XASTKIND_BUILTIN_ALLOCA
 DEE_MEMBERDEF_CONST_v100("KIND_BUILTIN_ALLOCA",DeeXAstKind,DEE_XASTKIND_BUILTIN_ALLOCA),
#endif
#ifdef DEE_XASTKIND_BUILTIN_HELP
 DEE_MEMBERDEF_CONST_v100("KIND_BUILTIN_HELP",DeeXAstKind,DEE_XASTKIND_BUILTIN_HELP),
#endif
#ifdef DEE_XASTKIND_BUILTIN_BREAKPOINT
 DEE_MEMBERDEF_CONST_v100("KIND_BUILTIN_BREAKPOINT",DeeXAstKind,DEE_XASTKIND_BUILTIN_BREAKPOINT),
#endif
#ifdef DEE_XASTKIND_BUILTIN_UNREACHABLE
 DEE_MEMBERDEF_CONST_v100("KIND_BUILTIN_UNREACHABLE",DeeXAstKind,DEE_XASTKIND_BUILTIN_UNREACHABLE),
#endif
#ifdef DEE_XASTKIND_BUILTIN_BOUND
 DEE_MEMBERDEF_CONST_v100("KIND_BUILTIN_BOUND",DeeXAstKind,DEE_XASTKIND_BUILTIN_BOUND),
#endif
#ifdef DEE_XASTKIND_BUILTIN_DEX
 DEE_MEMBERDEF_CONST_v100("KIND_BUILTIN_DEX",DeeXAstKind,DEE_XASTKIND_BUILTIN_DEX),
#endif
#ifdef DEE_XASTKIND_BUILTIN_EXTERN
 DEE_MEMBERDEF_CONST_v100("KIND_BUILTIN_EXTERN",DeeXAstKind,DEE_XASTKIND_BUILTIN_EXTERN),
#endif
#ifdef DEE_XASTKIND_BUILTIN_FF_CLOSURE
 DEE_MEMBERDEF_CONST_v100("KIND_BUILTIN_FF_CLOSURE",DeeXAstKind,DEE_XASTKIND_BUILTIN_FF_CLOSURE),
#endif
#ifdef DEE_XASTKIND_SUPER_AT
 DEE_MEMBERDEF_CONST_v100("KIND_SUPER_AT",DeeXAstKind,DEE_XASTKIND_SUPER_AT),
#endif
#ifdef DEE_XASTKIND_BUILTIN_UNUSED
 DEE_MEMBERDEF_CONST_v100("KIND_BUILTIN_UNUSED",DeeXAstKind,DEE_XASTKIND_BUILTIN_UNUSED),
#endif
#ifdef DEE_XASTKIND_BUILTIN_EXPECT
 DEE_MEMBERDEF_CONST_v100("KIND_BUILTIN_EXPECT",DeeXAstKind,DEE_XASTKIND_BUILTIN_EXPECT),
#endif
#ifdef DEE_XASTKIND_SEQ_ANY
 DEE_MEMBERDEF_CONST_v100("KIND_SEQ_ANY",DeeXAstKind,DEE_XASTKIND_SEQ_ANY),
#endif
#ifdef DEE_XASTKIND_SEQ_ALL
 DEE_MEMBERDEF_CONST_v100("KIND_SEQ_ALL",DeeXAstKind,DEE_XASTKIND_SEQ_ALL),
#endif
#ifdef DEE_XASTKIND_SEQ_SUM
 DEE_MEMBERDEF_CONST_v100("KIND_SEQ_SUM",DeeXAstKind,DEE_XASTKIND_SEQ_SUM),
#endif
#ifdef DEE_XASTKIND_SEQ_MIN
 DEE_MEMBERDEF_CONST_v100("KIND_SEQ_MIN",DeeXAstKind,DEE_XASTKIND_SEQ_MIN),
#endif
#ifdef DEE_XASTKIND_SEQ_MAX
 DEE_MEMBERDEF_CONST_v100("KIND_SEQ_MAX",DeeXAstKind,DEE_XASTKIND_SEQ_MAX),
#endif
//[[[end]]]
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_CONST_v100("__VARDECLSTORAGEKIND_NONE",Dee_uint32_t,DEE_VARDECLSTORAGEKIND_NONE),
 DEE_MEMBERDEF_CONST_v100("__VARDECLSTORAGEKIND_VAR",Dee_uint32_t,DEE_VARDECLSTORAGEKIND_VAR),
 DEE_MEMBERDEF_CONST_v100("__VARDECLSTORAGEKIND_ATTR",Dee_uint32_t,DEE_VARDECLSTORAGEKIND_ATTR),
 DEE_MEMBERDEF_CONST_v100("__VARDECLSTORAGEKIND_ATTR_C",Dee_uint32_t,DEE_VARDECLSTORAGEKIND_ATTR_C),
 DEE_MEMBERDEF_CONST_v100("__VARDECLSTORAGEKIND_ITEM",Dee_uint32_t,DEE_VARDECLSTORAGEKIND_ITEM),
 DEE_MEMBERDEF_CONST_v100("__VARDECLSTORAGEKIND_RANGE",Dee_uint32_t,DEE_VARDECLSTORAGEKIND_RANGE),
 DEE_MEMBERDEF_CONST_v100("__VARDECLSTORAGEKIND_EXPAND",Dee_uint32_t,DEE_VARDECLSTORAGEKIND_EXPAND),
 DEE_MEMBERDEF_CONST_v100("__VARDECLSTORAGEKIND_ASSIGN",Dee_uint32_t,DEE_VARDECLSTORAGEKIND_ASSIGN),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
 DEE_MEMBERDEF_END_v100
};
DEE_COMPILER_MSVC_WARNING_POP

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deexast_tp_getsets DeeType_DEFAULT_SLOT(tp_getsets)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
static DeeObject *DEE_CALL _deexast_ast_class_c_uuid_get(
 DeeXAstObject *self, void *DEE_UNUSED(closure)) {
 return DeeUUID_ToString(&self->ast_class.c_uuid);
}
static int DEE_CALL _deexast_ast_class_c_uuid_set(
 DeeXAstObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 if (DeeError_TypeError_CheckTypeExact(v,&DeeString_Type) != 0) return -1;
 return DeeUUID_FromStringWithError(&self->ast_class.c_uuid,DeeString_STR(v));
}
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
static struct DeeGetSetDef const _deexast_tp_getsets[] = {
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
 DEE_GETSETDEF_v100("__ast_class_c_uuid",
  member(&_deexast_ast_class_c_uuid_get),null,
  member(&_deexast_ast_class_c_uuid_set),"-> string"),
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
 DEE_GETSETDEF_END_v100
};
#endif
static struct DeeMemberDef const _deexast_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("kind",DeeXAstObject,ast_kind,DeeXAstKind),
#if DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_kind",DeeXAstObject,ast_kind,DeeXAstKind),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_common_ast_kind",DeeXAstObject,ast_kind,DeeXAstKind),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_common_ast_token",DeeXAstObject,ast_common.ast_token,object,"-> token"),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_const_c_const",DeeXAstObject,ast_const.c_const,object),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_var_vs_var",DeeXAstObject,ast_var.vs_var,object,"-> local_var"),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_var_vs_flags",DeeXAstObject,ast_var.vs_flags,Dee_uint32_t),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_vardecl_vd_token",DeeXAstObject,ast_vardecl.vd_token,object,"-> token"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_vardecl_vd_var",DeeXAstObject,ast_vardecl.vd_var,object,"-> local_var"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_vardecl_vd_init",DeeXAstObject,ast_vardecl.vd_init,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_multivardecl_mvd_type",DeeXAstObject,ast_multivardecl.mvd_type,DeeXAstKind),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_multivardecl_mvd_varc",DeeXAstObject,ast_multivardecl.mvd_varc,Dee_size_t),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_multivardecl_mvd_varv",DeeXAstObject,ast_multivardecl.mvd_varv,p(void),"-> DeeVarDeclStorage *"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_multivardecl_mvd_init",DeeXAstObject,ast_multivardecl.mvd_init,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_statement_s_stmt",DeeXAstObject,ast_statement.s_stmt,object,"-> sast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_if",DeeXAstObject,ast_if.if_cond,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_if_if_succ",DeeXAstObject,ast_if.if_succ,object_null,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_if_if_fail",DeeXAstObject,ast_if.if_fail,object_null,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_function_f_name",DeeXAstObject,ast_function.f_name,object_null,"-> string"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_function_f_flags",DeeXAstObject,ast_function.f_flags,Dee_uint32_t,"\tSame as function.FLAG_*."),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_function_f_argc",DeeXAstObject,ast_function.f_argc,Dee_size_t),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_function_f_argv",DeeXAstObject,ast_function.f_argv,p(void),"-> DeeXAstFunctionAstArgument *"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_function_f_code",DeeXAstObject,ast_function.f_code,object_null,"-> sast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_function_f_scope",DeeXAstObject,ast_function.f_scope,object_null,"-> scope"),
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_class_c_name",DeeXAstObject,ast_class.c_name,object_null,"-> string"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_class_c_base",DeeXAstObject,ast_class.c_base,object_null,"-> xast"),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_class_c_entryc",DeeXAstObject,ast_class.c_entryc,Dee_size_t),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_class_c_entryv",DeeXAstObject,ast_class.c_entryv,p(void),"-> DeeXAstClassAstEntry *"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_class_c_tpflags",DeeXAstObject,ast_class.c_tpflags,Dee_uint32_t,"\tSet of type-flags"),
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_sequence_s_elemc",DeeXAstObject,ast_sequence.s_elemc,Dee_size_t),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_sequence_s_elemv",DeeXAstObject,ast_sequence.s_elemv,p2(void),"-> DeeXAstObject **"),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_dict_d_elemc",DeeXAstObject,ast_dict.d_elemc,Dee_size_t),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_dict_d_elemv",DeeXAstObject,ast_dict.d_elemv,p2(void),"-> DeeXAstDictEntry *"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_seq_range_sr_seq",DeeXAstObject,ast_seq_range.sr_seq,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_seq_range_sr_begin",DeeXAstObject,ast_seq_range.sr_begin,object_null,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_seq_range_sr_end",DeeXAstObject,ast_seq_range.sr_end,object_null,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_seq_range_sr_value",DeeXAstObject,ast_seq_range.sr_value,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_attr_c_ac_object",DeeXAstObject,ast_attr_c.ac_object,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_attr_c_ac_name",DeeXAstObject,ast_attr_c.ac_name,object,"-> string"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_attr_ac_value",DeeXAstObject,ast_attr_c.ac_value,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_module_m_module",DeeXAstObject,ast_module.m_module,object,"-> scope"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_delvar_d_token",DeeXAstObject,ast_delvar.d_token,object,"-> token"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_delvar_d_var",DeeXAstObject,ast_delvar.d_var,object,"-> local_var"),
#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_foreignfunction_ff_base",DeeXAstObject,ast_foreignfunction.ff_base,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_foreignfunction_ff_flags",DeeXAstObject,ast_foreignfunction.ff_flags,DeeFunctionFlags,"-> DeeFunctionFlags"),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_foreignfunction_ff_argc",DeeXAstObject,ast_foreignfunction.ff_argc,Dee_size_t),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_foreignfunction_ff_argv",DeeXAstObject,ast_foreignfunction.ff_argv,p2(void),"-> DeeXAstObject **"),
#endif /* DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION */
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_switch_s_expr",DeeXAstObject,ast_switch.s_expr,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_switch_s_entryc",DeeXAstObject,ast_switch.s_entryc,Dee_size_t),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_switch_s_entryv",DeeXAstObject,ast_switch.s_entryv,p(void),"-> DeeXAstSwitchEntry *"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_switch_s_default",DeeXAstObject,ast_switch.s_default,object_null,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_ifconst_ic_succ",DeeXAstObject,ast_ifconst.ic_succ,object_null,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_ifconst_ic_fail",DeeXAstObject,ast_ifconst.ic_fail,object_null,"-> xast"),
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_builtin_bound_b_token",DeeXAstObject,ast_builtin_bound.b_token,object,"-> token"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_builtin_bound_b_var",DeeXAstObject,ast_builtin_bound.b_var,object,"-> local_var"),
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */
#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_builtin_extern_ex_file",DeeXAstObject,ast_builtin_extern.ex_file,object_null,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_builtin_extern_ex_name",DeeXAstObject,ast_builtin_extern.ex_name,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_builtin_extern_ex_type",DeeXAstObject,ast_builtin_extern.ex_type,object,"-> xast"),
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_builtin_expect_e_value",DeeXAstObject,ast_builtin_expect.e_value,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_RO_v100("__ast_builtin_expect_e_expect",DeeXAstObject,ast_builtin_expect.e_expect,object),
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_operator_op_a",DeeXAstObject,ast_operator.op_a,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_operator_op_b",DeeXAstObject,ast_operator.op_b,object,"-> xast"),
 DEE_MEMBERDEF_NAMED_DOC_RO_v100("__ast_operator_op_c",DeeXAstObject,ast_operator.op_c,object,"-> xast"),
 DEE_MEMBERDEF_END_v100
};



DeeTypeObject DeeXAst_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("xast"),null,
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeXAstObject),
  member(&_deexast_tp_alloc),null,
  member(&_deexast_tp_copy_ctor),
  member(&_deexast_tp_move_ctor),null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(
  member(&_deexast_tp_free),
  member(&_deexast_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deexast_tp_copy_assign),
  member(&_deexast_tp_move_assign),null),
 DEE_TYPE_OBJECT_CAST_v101(null,member(&_deexast_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,member(&_deexast_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,
  member(&_deexast_tp_cmp_eq),
  member(&_deexast_tp_cmp_ne),null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deexast_tp_members),member(_deexast_tp_getsets),member(_deexast_tp_methods),
  member(_deexast_tp_class_members),null,member(_deexast_tp_class_methods)),
 DEE_TYPE_OBJECT_FOOTER_v100
};


DEE_A_RET_WUNUSED int DeeXAst_TokenIDIsExpressionBeginDecl(DEE_A_IN TPPTokenID id) {
 switch (id) {
  DEE_ATTRIBUTEDECL_TOKENS
  DEE_XAST_VARIBLE_LOOKUP_TOKENS
  DEE_XAST_UNARY_TOKENS_WITH_SYMBOLS_NOATTR
   return 1;
  default: break;
 }
 return TPPTokenID_IS_KEYWORD(id);
}
DEE_A_RET_WUNUSED int DeeXAst_TokenIDIsExpressionBegin(
 DEE_A_IN TPPTokenID id, DEE_A_IN DeeScopeObject const *scope) {
 switch (id) {
  DEE_ATTRIBUTEDECL_TOKENS
  DEE_XAST_VARIBLE_LOOKUP_TOKENS
  DEE_XAST_UNARY_TOKENS_WITH_SYMBOLS_NOATTR
   return 1;
  default:
   if (TPPTokenID_IS_KEYWORD(id)) while (1) {
    // Loop if the variable exists
    if (_DeeScope_HasName(scope,id)) return 1;
    if (scope->sc_weak_prev) scope = scope->sc_weak_prev;
    else if (scope->sc_prev) scope = scope->sc_prev;
    else break;
   }
   break;
 }
 return 0;
}
DEE_A_RET_WUNUSED int DeeXAst_TokenIDIsExpressionEnd(DEE_A_IN TPPTokenID id) {
 switch (id) {
  case '}': case ']': case ')':
  case ';': case ',': case '=':
  case TPP_TOK_AT:
  case TPP_TOK_COLLON:
   return 1;
  default: break;
 }
 return 0;
}
DEE_A_RET_WUNUSED int DeeXAst_TokenIDIsAttribute(DEE_A_IN TPPTokenID id) {
 switch (id) {
  DEE_ATTRIBUTEDECL_TOKENS
   return 1;
  default: break;
 }
 return 0;
}
DEE_A_RET_WUNUSED int DeeXAst_TokenIDIsCTypeSuffixBegin(DEE_A_IN TPPTokenID id) {
 switch (id) {
  DEE_ATTRIBUTEDECL_TOKENS
  case '*': case TPP_TOK_MUL_ASSIGN:
  case TPP_TOK_POW: case TPP_TOK_POW_ASSIGN:
  case '&': case TPP_TOK_BIN_AND_ASSIGN: case TPP_TOK_LAND:
  case '(': return 1;
  default: break;
 }
 return TPPTokenID_IS_KEYWORD(id);
}

#if DEE_CONFIG_XAST_CHECK_RESERVED_IDENTIFERS
DEE_A_RET_WUNUSED int DeeXAst_TokenIsReservedIdentifier(DEE_A_IN DeeTokenObject *tok) {
 char const *tok_begin,*tok_end;
 unsigned int uscore_count;
 DEE_ASSERT(DeeObject_Check(tok) && DeeToken_Check(tok));
 if (!TPPTokenID_IS_KEYWORD(tok->tk_token.tk_id)) return 0;
 tok_begin = tok->ob_token.tk_str_begin;
 tok_end = tok->ob_token.tk_str_end;
 uscore_count = 0;
 while (tok_begin != tok_end) {
  char ch = *tok_begin++;
  if (ch == '\\') {
   if (tok_begin == tok_end) break;
   if (*tok_begin == '\n') ++tok_begin;
   else if (*tok_begin == '\r') {
    if (*++tok_begin == '\n') ++tok_begin;
   }
   continue;
  }
  if (ch == '_') {
   if (++uscore_count == 2) return 1;
  } else break;
 }
 return 0;
}
DEE_A_RET_WUNUSED int DeeXAst_CheckReservedToken(
 DEE_A_IN DeeLexerObject *lexer, DEE_A_IN DeeTokenObject *tok) {
 if (DeeXAst_TokenIsReservedIdentifier(tok)) {
  return DeeError_CompilerError(DEE_WARNING_RESERVED_KEYWORD_NAMESPACE,
   (DeeObject *)lexer,(DeeObject *)tok,
   "Identifiers starting with '__' are reserved by the language");
 }
 return 0;
}
#endif


DEE_DECL_END

#ifndef __INTELLISENSE__
#include "xast.assign.inl"
#include "xast.attributes.inl"
#include "xast.collect_top_scope_children.inl"
#include "xast.copy_with_scope.inl"
#include "xast.equals.inl"
#include "xast.generic_sequence_size.inl"
#include "xast.intrinsic_function_slots.inl"
#include "xast.new_binary.inl"
#include "xast.new_trinary.inl"
#include "xast.new_unary.inl"
#include "xast.offsetof.inl"
#include "xast.optimize_multivardecl.inl"
#include "xast.optimize_sequence.inl"
#include "xast.optimizer_assumptions.inl"
#include "xast.predict_type.inl"
#include "xast.repr.inl"
#include "xast.typeattriutes.inl"
#include "xast.vardeclstorage_from_xast.inl"
#include "xast.visit_all.inl"
#include "xast_optimize.inl"
#define DO_STORE
#define DO_COUNT
#include "xast.variable_usage.inl"
#define DO_STORE
#include "xast.variable_usage.inl"
#define DO_LOAD
#define DO_COUNT
#include "xast.variable_usage.inl"
#define DO_LOAD
#include "xast.variable_usage.inl"
#define DO_COUNT
#include "xast.variable_usage.inl"
#include "xast.variable_usage.inl"

#define SEQ_TYPE  DEE_XASTKIND_TUPLE
#include "xast.sequence_size.inl"
#define SEQ_TYPE  DEE_XASTKIND_LIST
#include "xast.sequence_size.inl"
#define SEQ_TYPE  DEE_XASTKIND_SET
#include "xast.sequence_size.inl"
#endif

#endif /* !GUARD_DEEMON_EXPR_AST_C */
