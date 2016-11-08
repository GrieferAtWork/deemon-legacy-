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
#include "xast.c"
#endif


DEE_DECL_BEGIN

#define _DeeXAstDictEntry_InitCopy(ob,right,...)\
do{\
 Dee_INCREF((ob)->de_key=(right)->de_key);\
 Dee_INCREF((ob)->de_item=(right)->de_item);\
}while(0)


#define _DeeXAstCommonAst_InitCopy(ob,right)\
do{\
 (ob)->ast_kind_ = (right)->ast_kind_;\
 Dee_INCREF((ob)->ast_token = (right)->ast_token);\
}while(0)

#define _DeeXAstConstAst_InitCopy(ob,right,...)\
do{\
 Dee_INCREF((ob)->c_const = (right)->c_const);\
}while(0)

#define _DeeXAstVarAst_InitCopy(ob,right,...)\
do{\
 Dee_INCREF((ob)->vs_var = (right)->vs_var);\
 DeeLocalVar_ADD_USE((ob)->vs_var);\
 (ob)->vs_flags = (right)->vs_flags;\
}while(0)

#define _DeeXAstVarDeclAst_InitCopy(ob,right,...)\
do{\
 Dee_INCREF((ob)->vd_token = (right)->vd_token);\
 DeeLocalVar_ADD_INIT((ob)->vd_var);\
 Dee_INCREF((ob)->vd_var = (right)->vd_var);\
 Dee_INCREF((ob)->vd_init = (right)->vd_init);\
}while(0)

#define _DeeXAstMultiVarDeclAst_InitCopy(ob,right,...)\
do{\
 struct DeeVarDeclStorage *new_varstorage,*varstorage_iter,*varstorage_end,*varstorage_src;\
 DEE_ASSERT((right)->mvd_varc != 0);\
 while ((new_varstorage = (struct DeeVarDeclStorage *)malloc_nz(\
  (right)->mvd_varc*sizeof(struct DeeVarDeclStorage))) == NULL) {\
  if DEE_LIKELY(Dee_CollectMemory()) continue;\
  DeeError_NoMemory();\
  {__VA_ARGS__;}\
 }\
 (ob)->mvd_varc = (right)->mvd_varc;\
 (ob)->mvd_varv = new_varstorage;\
 varstorage_end = (varstorage_iter = new_varstorage)+(ob)->mvd_varc;\
 varstorage_src = (right)->mvd_varv;\
 do {\
  _DeeVarDeclStorage_InitCopy(varstorage_iter,varstorage_src,{\
   while (varstorage_iter != new_varstorage) { \
    _DeeVarDeclStorage_Quit(varstorage_iter); \
    --varstorage_iter;\
   }\
   free_nn(new_varstorage);\
   {__VA_ARGS__;};\
  });\
  ++varstorage_iter,++varstorage_src;\
 } while (varstorage_iter != varstorage_end);\
 Dee_INCREF((ob)->mvd_init=(right)->mvd_init);\
}while(0)

#define _DeeXAstOperatorAst_InitCopy(ob,right,...)\
do{\
 if (DEE_XASTKIND_ISOPERATOR((right)->ast_kind_)) {\
  switch (DEE_XASTKIND_OPCOUNT((right)->ast_kind_)) {\
   case 3: Dee_INCREF((ob)->op_c = (right)->op_c); DEE_ATTRIBUTE_FALLTHROUGH\
   case 2: Dee_INCREF((ob)->op_b = (right)->op_b); DEE_ATTRIBUTE_FALLTHROUGH\
   case 1: Dee_INCREF((ob)->op_a = (right)->op_a);\
   default: break;\
  }\
 }\
}while(0)
#define _DeeXAstUnaryInplaceVarAst_InitCopy(ob,right,...)\
do{\
 Dee_INCREF((ob)->uiv_var = (right)->uiv_var);\
 Dee_INCREF((ob)->uiv_tok = (right)->uiv_tok);\
 DeeLocalVar_ADD_INIT((ob)->uiv_var);\
 DeeLocalVar_ADD_USE((ob)->uiv_var);\
}while(0)
#define _DeeXAstBinaryInplaceVarAst_InitCopy(ob,right,...)\
do{\
 Dee_INCREF((ob)->biv_var = (right)->biv_var);\
 Dee_INCREF((ob)->biv_tok = (right)->biv_tok);\
 Dee_INCREF((ob)->biv_arg = (right)->biv_arg);\
 DeeLocalVar_ADD_INIT((ob)->biv_var);\
 DeeLocalVar_ADD_USE((ob)->biv_var);\
}while(0)

#define _DeeXAstStatementAst_InitCopy(ob,right,...)\
do{\
 Dee_INCREF((ob)->s_stmt = (right)->s_stmt);\
}while(0)

#define _DeeXAstIfAst_InitCopy(ob,right,...)\
do{\
 Dee_INCREF((ob)->if_cond = (right)->if_cond);\
 Dee_XINCREF((ob)->if_succ = (right)->if_succ);\
 Dee_XINCREF((ob)->if_fail = (right)->if_fail);\
}while(0)

#define _DeeXAstFunctionAst_InitCopy(ob,right,...)\
do{\
 struct DeeXAstFunctionAstArgument *_new_argv,*_argv_iter,*_argv_end;\
 if ((right)->f_argc) {\
  while ((_new_argv = (struct DeeXAstFunctionAstArgument *)malloc_nz(\
   (right)->f_argc*sizeof(struct DeeXAstFunctionAstArgument))) == NULL) {\
   if (Dee_CollectMemory()) continue;\
   DeeError_NoMemory();\
   {__VA_ARGS__};\
  }\
  (ob)->f_argv = _new_argv;\
  (ob)->f_argc = (right)->f_argc;\
  _argv_end = (_argv_iter = _new_argv)+(right)->f_argc;\
  _new_argv = (right)->f_argv;\
  while (_argv_iter != _argv_end) {\
   _DeeXAstFunctionAstArgument_InitCopy(_argv_iter,_new_argv,{\
    while (_argv_iter != (ob)->f_argv) {\
     --_argv_iter;\
     _DeeXAstFunctionAstArgument_Quit(_argv_iter);\
    }\
    free_nn(_argv_iter);\
    {__VA_ARGS__};\
   });\
   ++_argv_iter,++_new_argv;\
  }\
 } else {\
  (ob)->f_argc = 0;\
  (ob)->f_argv = NULL;\
 }\
 Dee_XINCREF((ob)->f_name = (right)->f_name);\
 (ob)->f_flags = (right)->f_flags;\
 Dee_XINCREF((ob)->f_code = (right)->f_code);\
 Dee_XINCREF((ob)->f_scope = (right)->f_scope);\
}while(0)

#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
#define _DeeXAstClassAst_InitCopy(ob,right,...)\
do{\
 struct DeeXAstClassAstEntry *_new_entryv,*_entryv_iter,*_entryv_end;\
 if ((right)->c_entryc != 0) {\
  while ((_new_entryv = (struct DeeXAstClassAstEntry *)malloc_nz(\
   (right)->c_entryc*sizeof(struct DeeXAstClassAstEntry))) == NULL) {\
   if (Dee_CollectMemory()) continue;\
   DeeError_NoMemory();\
   {__VA_ARGS__};\
  }\
  (ob)->c_entryv = _new_entryv;\
  (ob)->c_entryc = (right)->c_entryc;\
  _entryv_end = (_entryv_iter = _new_entryv)+(right)->c_entryc;\
  _new_entryv = (right)->c_entryv;\
  while (_entryv_iter != _entryv_end) {\
   _DeeXAstClassAstEntry_InitCopy(_entryv_iter,_new_entryv,{\
    while (_entryv_iter != (ob)->c_entryv) {\
     --_entryv_iter;\
     _DeeXAstClassAstEntry_Quit(_entryv_iter);\
    }\
    free_nn(_entryv_iter);\
    {__VA_ARGS__};\
   });\
   ++_entryv_iter,++_new_entryv;\
  }\
 } else {\
  (ob)->c_entryc = 0;\
  (ob)->c_entryv = NULL;\
 }\
 Dee_XINCREF((ob)->c_name = (right)->c_name);\
 Dee_XINCREF((ob)->c_base = (right)->c_base);\
 (ob)->c_tpflags = (right)->c_tpflags;\
 DeeUUID_SET(&(ob)->c_uuid,&(right)->c_uuid);\
}while(0)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */

#define _DeeXAstSequenceAst_InitCopy(ob,right,...)\
do{\
 DeeXAstObject **_new_elemv,**_elemv_iter,**_elemv_end;\
 DEE_ASSERT((right)->s_elemc != 0);\
 while ((_new_elemv = (DeeXAstObject **)malloc_nz(\
  (right)->s_elemc*sizeof(DeeXAstObject *))) == NULL) {\
  if (Dee_CollectMemory()) continue;\
  DeeError_NoMemory();\
  {__VA_ARGS__;}\
 }\
 (ob)->s_elemc = (right)->s_elemc;\
 (ob)->s_elemv = _new_elemv;\
 _elemv_end = (_elemv_iter = _new_elemv)+(ob)->s_elemc;\
 _new_elemv = (right)->s_elemv;\
 while (_elemv_iter != _elemv_end)\
  Dee_INCREF(*_elemv_iter++ = *_new_elemv++);\
}while(0)

#define _DeeXAstDictAst_InitCopy(ob,right,...)\
do{\
 struct DeeXAstDictEntry *_new_elemv,*_elemv_iter,*_elemv_end;\
 DEE_ASSERT((right)->d_elemc != 0);\
 while ((_new_elemv = (struct DeeXAstDictEntry *)malloc_nz(\
  (right)->d_elemc*sizeof(struct DeeXAstDictEntry))) == NULL) {\
  if (Dee_CollectMemory()) continue;\
  DeeError_NoMemory();\
  {__VA_ARGS__;}\
 }\
 (ob)->d_elemc = (right)->d_elemc;\
 (ob)->d_elemv = _new_elemv;\
 _elemv_end = (_elemv_iter = _new_elemv)+(ob)->d_elemc;\
 _new_elemv = (right)->d_elemv;\
 while (_elemv_iter != _elemv_end) {\
  _DeeXAstDictEntry_InitCopy(_elemv_iter,_new_elemv,{\
   while (_elemv_iter != (ob)->d_elemv) {\
    --_elemv_iter;\
    _DeeXAstDictEntry_Quit(_elemv_iter);\
   }\
   free_nn(_elemv_iter);\
   {__VA_ARGS__;}\
  });\
  ++_elemv_iter; ++_new_elemv;\
 }\
}while(0)

#define _DeeXAstSeqRangeAst_InitCopy(ob,right,...)\
do{\
 Dee_INCREF((ob)->sr_seq = (right)->sr_seq);\
 Dee_XINCREF((ob)->sr_begin = (right)->sr_begin);\
 Dee_XINCREF((ob)->sr_end = (right)->sr_end);\
 if ((right)->ast_kind_ == DEE_XASTKIND_SEQ_RANGE_SET)\
  Dee_XINCREF((ob)->sr_value = (right)->sr_value);\
}while(0)

#define _DeeXAstAttrCAst_InitCopy(ob,right,...)\
do{\
 Dee_INCREF((ob)->ac_object = (right)->ac_object);\
 Dee_INCREF((ob)->ac_name = (right)->ac_name);\
 if ((right)->ast_kind_ == DEE_XASTKIND_ATTR_SET_C)\
  Dee_XINCREF((ob)->ac_value = (right)->ac_value);\
}while(0)

#define _DeeXAstModuleAst_InitCopy(ob,right,...)\
do{\
 Dee_INCREF((ob)->m_module = (right)->m_module);\
}while(0)

#define _DeeXAstDelVarAst_InitCopy(ob,right,...)\
do{\
 Dee_INCREF((ob)->d_token = (right)->d_token);\
 Dee_INCREF((ob)->d_var = (right)->d_var);\
}while(0)

#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
#define _DeeXAstForeignFunctionAst_InitCopy(ob,right,...)\
do{\
 DeeXAstObject **_new_argv,**_argv_iter,**_argv_end;\
 DEE_ASSERT((right)->ff_argc != 0);\
 while ((_new_argv = (DeeXAstObject **)malloc_nz(\
  (right)->ff_argc*sizeof(DeeXAstObject *))) == NULL) {\
  if (Dee_CollectMemory()) continue;\
  DeeError_NoMemory();\
  {__VA_ARGS__;}\
 }\
 (ob)->ff_argc = (right)->ff_argc;\
 (ob)->ff_argv = _new_argv;\
 _argv_end = (_argv_iter = _new_argv)+(ob)->ff_argc;\
 _new_argv = (right)->ff_argv;\
 while (_argv_iter != _argv_end)\
  Dee_INCREF(*_argv_iter++ = *_new_argv++);\
 Dee_INCREF((ob)->ff_base = (right)->ff_base);\
 (ob)->ff_flags = (right)->ff_flags;\
}while(0)
#endif

#define _DeeXAstSwitchAst_InitCopy(ob,right,...)\
do{\
 struct DeeXAstSwitchEntry *_new_entryv,*_entryv_iter,*_entryv_end;\
 DEE_ASSERT((right)->s_entryc != 0);\
 while ((_new_entryv = (struct DeeXAstSwitchEntry *)malloc_nz(\
  (right)->s_entryc*sizeof(struct DeeXAstSwitchEntry))) == NULL) {\
  if (Dee_CollectMemory()) continue;\
  DeeError_NoMemory();\
  {__VA_ARGS__};\
 }\
 (ob)->s_entryv = _new_entryv;\
 (ob)->s_entryc = (right)->s_entryc;\
 _entryv_end = (_entryv_iter = _new_entryv)+(right)->s_entryc;\
 _new_entryv = (right)->s_entryv;\
 while (_entryv_iter != _entryv_end) {\
  _DeeXAstSwitchEntry_InitCopy(_entryv_iter,_new_entryv,{\
   while (_entryv_iter != (ob)->s_entryv) {\
    --_entryv_iter;\
    _DeeXAstSwitchEntry_Quit(_entryv_iter);\
   }\
   free_nn(_entryv_iter);\
   {__VA_ARGS__};\
  });\
  ++_entryv_iter,++_new_entryv;\
 }\
 Dee_INCREF((ob)->s_expr = (right)->s_expr);\
 Dee_XINCREF((ob)->s_default = (right)->s_default);\
}while(0)

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
#define _DeeXAstBuiltinBoundAst_InitCopy(ob,right,...)\
do{\
 Dee_INCREF((ob)->b_token = (right)->b_token);\
 Dee_INCREF((ob)->b_var = (right)->b_var);\
}while(0)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */

#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
#define _DeeXAstBuiltinExternAst_InitCopy(ob,right,...)\
do{\
 Dee_XINCREF((ob)->ex_file = (right)->ex_file);\
 Dee_INCREF((ob)->ex_name = (right)->ex_name);\
 Dee_INCREF((ob)->ex_type = (right)->ex_type);\
}while(0)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */

#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
#define _DeeXAstBuiltinExpectAst_InitCopy(ob,right,...)\
do{\
 Dee_INCREF((ob)->e_value = (right)->e_value);\
 Dee_INCREF((ob)->e_expect = (right)->e_expect);\
}while(0)
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */

#ifdef __INTELLISENSE__
#undef _DeeXAstSeqRangeAst_InitCopy
#undef _DeeXAstAttrCAst_InitCopy
#undef _DeeXAstOperatorAst_InitCopy
#define _DeeXAstSeqRangeAst_InitCopy(ob,right,...) do{}while(0)
#define _DeeXAstAttrCAst_InitCopy(ob,right,...)    do{}while(0)
#define _DeeXAstOperatorAst_InitCopy(ob,right,...) do{}while(0)
#endif



DEE_A_RET_EXCEPT(-1) int DeeXAst_InitCopy(
 DEE_A_OUT DeeXAstObject *self, DEE_A_INOUT DeeXAstObject *right) {
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(right) && DeeXAst_Check(right));
#ifdef DEE_DEBUG
 memset(self,0,sizeof(*self));
#endif
 switch (self->ast_kind) {
  case DEE_XASTKIND_CONST:            _DeeXAstConstAst_InitCopy(&self->ast_const,&right->ast_const,{return -1;}); break;
  case DEE_XASTKIND_VAR:              _DeeXAstVarAst_InitCopy(&self->ast_var,&right->ast_var,{return -1;}); break;
  case DEE_XASTKIND_VARDECL:          _DeeXAstVarDeclAst_InitCopy(&self->ast_vardecl,&right->ast_vardecl,{return -1;}); break;
  case DEE_XASTKIND_MULTIVARDECL:     _DeeXAstMultiVarDeclAst_InitCopy(&self->ast_multivardecl,&right->ast_multivardecl,{return -1;}); break;
  case DEE_XASTKIND_STATEMENT:        _DeeXAstStatementAst_InitCopy(&self->ast_statement,&right->ast_statement,{return -1;}); break;
  case DEE_XASTKIND_IF:               _DeeXAstIfAst_InitCopy(&self->ast_if,&right->ast_if,{return -1;}); break;
  case DEE_XASTKIND_FUNCTION:         _DeeXAstFunctionAst_InitCopy(&self->ast_function,&right->ast_function,{return -1;}); break;
#if DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES
  case DEE_XASTKIND_CLASS:            _DeeXAstClassAst_InitCopy(&self->ast_class,&right->ast_class,{return -1;}); break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_CLASS_TYPES */
  case DEE_XASTKIND_TUPLE:
  case DEE_XASTKIND_LIST:
  case DEE_XASTKIND_SET:              _DeeXAstSequenceAst_InitCopy(&self->ast_sequence,&right->ast_sequence,{return -1;}); break;
  case DEE_XASTKIND_DICT:             _DeeXAstDictAst_InitCopy(&self->ast_dict,&right->ast_dict,{return -1;}); break;
  case DEE_XASTKIND_SEQ_RANGE_GET:
  case DEE_XASTKIND_SEQ_RANGE_DEL:
  case DEE_XASTKIND_SEQ_RANGE_SET:    _DeeXAstSeqRangeAst_InitCopy(&self->ast_seq_range,&right->ast_seq_range,{return -1;}); break;
  case DEE_XASTKIND_ATTR_GET_C:
  case DEE_XASTKIND_ATTR_HAS_C:
  case DEE_XASTKIND_ATTR_DEL_C:
  case DEE_XASTKIND_ATTR_SET_C:       _DeeXAstAttrCAst_InitCopy(&self->ast_attr_c,&right->ast_attr_c,{return -1;}); break;
  case DEE_XASTKIND_MODULE:           _DeeXAstModuleAst_InitCopy(&self->ast_module,&right->ast_module,{return -1;}); break;
  case DEE_XASTKIND_DEL_VAR:          _DeeXAstDelVarAst_InitCopy(&self->ast_delvar,&right->ast_delvar,{return -1;}); break;
#if DEE_CONFIG_LANGUAGE_HAVE_FOREIGNFUNCTION
  case DEE_XASTKIND_FOREIGN_FUNCTION: _DeeXAstForeignFunctionAst_InitCopy(&self->ast_foreignfunction,&right->ast_foreignfunction,{return -1;}); break;
#endif
  case DEE_XASTKIND_SWITCH:           _DeeXAstSwitchAst_InitCopy(&self->ast_switch,&right->ast_switch,{return -1;}); break;
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND
  case DEE_XASTKIND_BUILTIN_BOUND:    _DeeXAstBuiltinBoundAst_InitCopy(&self->ast_builtin_bound,&right->ast_builtin_bound,{return -1;}); break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_BOUND */
#if DEE_CONFIG_LANGUAGE_HAVE_EXTERN
  case DEE_XASTKIND_BUILTIN_EXTERN:   _DeeXAstBuiltinExternAst_InitCopy(&self->ast_builtin_extern,&right->ast_builtin_extern,{return -1;}); break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_EXTERN */
#if DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT
  case DEE_XASTKIND_BUILTIN_EXPECT:   _DeeXAstBuiltinExpectAst_InitCopy(&self->ast_builtin_expect,&right->ast_builtin_expect,{return -1;}); break;
#endif /* DEE_CONFIG_LANGUAGE_HAVE_BUILTIN_EXPECT */
  DEE_XASTKIND_CASE_INPLACE_VAR_UNARY _DeeXAstUnaryInplaceVarAst_InitCopy(&self->ast_unary_var,&right->ast_unary_var,{return -1;}); break;
  DEE_XASTKIND_CASE_INPLACE_VAR_BINARY _DeeXAstBinaryInplaceVarAst_InitCopy(&self->ast_binary_var,&right->ast_binary_var,{return -1;}); break;
  default:                            _DeeXAstOperatorAst_InitCopy(&self->ast_operator,&right->ast_operator,{return -1;}); break;
 }
 _DeeXAstCommonAst_InitCopy(&self->ast_common,&right->ast_common);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeXAst_AssignCopy(
 DEE_A_INOUT DeeXAstObject *self, DEE_A_INOUT DeeXAstObject *right) {
 DeeXAstObject temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeXAst_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeXAst_Check(right));
 if (self == right) return 0;
 if (DeeXAst_InitCopy(&temp,right) != 0) return -1;
 _deexast_tp_dtor(self);
 memcpy(self,&temp,sizeof(DeeXAstObject));
 return 0;
}

void DeeXAst_InitMove(
 DEE_A_OUT DeeXAstObject *self, DEE_A_INOUT DeeXAstObject *right) {
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(right) && DeeXAst_Check(right));
 _DeeXAstCommonAst_InitCopy(&self->ast_common,&right->ast_common);
 memcpy(DeeXAst_UNCOMMON_DATA(self),DeeXAst_UNCOMMON_DATA(right),DeeXAst_UNCOMMON_SIZE);
#ifdef DEE_DEBUG
 memset(DeeXAst_UNCOMMON_DATA(right),0,DeeXAst_UNCOMMON_SIZE);
#endif
 right->ast_kind = DEE_XASTKIND_NONE;
}

void DeeXAst_AssignMove(DEE_A_INOUT DeeXAstObject *self, DEE_A_INOUT DeeXAstObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeXAst_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeXAst_Check(right));
 if (self == right) return;
 Dee_INCREF(right); // If 'right' is part of the sub-set of 'self'
 _deexast_tp_dtor(self);
 DeeXAst_InitMove(self,right);
 Dee_DECREF(right);
}

void DeeXAst_AssignConst(DEE_A_INOUT DeeXAstObject *self, DEE_A_IN DeeObject *const_value) {
 DEE_ASSERT(DeeObject_Check(self) && DeeXAst_Check(self));
 DEE_ASSERT(DeeObject_Check(const_value));
 Dee_INCREF(const_value); // in case 'right' is used by 'self' (inherited later)
 _DeeXAst_DestroyUncommon(self);
 self->ast_kind = DEE_XASTKIND_CONST;
 Dee_INHERIT_REF(self->ast_const.c_const,const_value);
}


DEE_DECL_END
