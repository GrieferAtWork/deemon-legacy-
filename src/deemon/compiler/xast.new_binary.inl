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

DEE_A_RET_EXCEPT_REF DeeXAstObject *DeeXAst_NewBinary(
 DEE_A_IN DeeXAstKind kind, DEE_A_INOUT DeeTokenObject *tk,
 DEE_A_INOUT DeeLexerObject *lexer, DEE_A_IN Dee_uint32_t parser_flags,
 DEE_A_INOUT DeeXAstObject *ast_a, DEE_A_INOUT DeeXAstObject *ast_b) {
 DeeXAstObject *result; DeeObject *const_copy_a,*const_copy_b,*binary_result;
 DEE_ASSERT(DEE_XASTKIND_ISBINARY(kind));
 DEE_ASSERT(DeeObject_Check(tk) && DeeToken_Check(tk));
 DEE_ASSERT(DeeObject_Check(lexer) && DeeLexer_Check(lexer));
 DEE_ASSERT(DeeObject_Check(ast_a) && DeeXAst_Check(ast_a));
 DEE_ASSERT(DeeObject_Check(ast_b) && DeeXAst_Check(ast_b));
 switch (kind) {
  {
   DeeXAstKind iv_kind;
   if (0) { case DEE_XASTKIND_IADD: iv_kind = DEE_XASTKIND_VAR_IADD; }
   if (0) { case DEE_XASTKIND_ISUB: iv_kind = DEE_XASTKIND_VAR_ISUB; }
   if (0) { case DEE_XASTKIND_IMUL: iv_kind = DEE_XASTKIND_VAR_IMUL; }
   if (0) { case DEE_XASTKIND_IDIV: iv_kind = DEE_XASTKIND_VAR_IDIV; }
   if (0) { case DEE_XASTKIND_IMOD: iv_kind = DEE_XASTKIND_VAR_IMOD; }
   if (0) { case DEE_XASTKIND_ISHL: iv_kind = DEE_XASTKIND_VAR_ISHL; }
   if (0) { case DEE_XASTKIND_ISHR: iv_kind = DEE_XASTKIND_VAR_ISHR; }
   if (0) { case DEE_XASTKIND_IAND: iv_kind = DEE_XASTKIND_VAR_IAND; }
   if (0) { case DEE_XASTKIND_IOR : iv_kind = DEE_XASTKIND_VAR_IOR ; }
   if (0) { case DEE_XASTKIND_IXOR: iv_kind = DEE_XASTKIND_VAR_IXOR; }
   if (0) { case DEE_XASTKIND_IPOW: iv_kind = DEE_XASTKIND_VAR_IPOW; }
   // Try to create a binary inplace-var AST
   if (ast_a->ast_kind == DEE_XASTKIND_VAR &&
      (ast_a->ast_var.vs_flags&DEE_XAST_VARAST_FLAG_REF)==0) {
    return _DeeXAst_NewBinaryInplaceVar(iv_kind,tk,ast_a->ast_common.ast_token,
                                        ast_a->ast_var.vs_var,ast_b);
   }
  } break;
 
  case DEE_XASTKIND_ADD:
   if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_MERGE_OPERATORS)!=0 &&
       ast_a->ast_kind == DEE_XASTKIND_CONST &&
       DeeString_Check(ast_a->ast_const.c_const) &&
       DeeString_SIZE(ast_a->ast_const.c_const) == 0) {
    // Optimize: '""+foobar()' --> 'str foobar()';
    return DeeXAst_NewUnary(DEE_XASTKIND_STR,tk,lexer,parser_flags,ast_b);
   }
   break;

  case DEE_XASTKIND_SUPER_AT: {
   DeeTypeObject const *predicted_type_type;
   if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_MERGE_OPERATORS) != 0 &&
       ast_a->ast_kind == DEE_XASTKIND_CONST && DeeXAst_PredictType(
       ast_b) == (DeeTypeObject const *)ast_a->ast_const.c_const) {
    // Optimize '__super(int,42)' --> '42'
    VLOG_PARSER(tk,"OPTIMIZE : '__super(%r,%r)' --> '%r'",ast_a,ast_b,ast_b);
    Dee_INCREF(ast_b);
    return ast_b;
   }
   if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0 &&
       ast_a->ast_kind == DEE_XASTKIND_CONST &&
       ast_b->ast_kind == DEE_XASTKIND_CONST) {
    DeeObject *super_ob; // Create the 'super' proxy object at compile-time
    if ((super_ob = DeeSuper_New((DeeTypeObject *)ast_a->ast_const.c_const,
     ast_b->ast_const.c_const)) == NULL) return NULL;
    result = DeeXAst_NewConst(tk,super_ob);
    Dee_DECREF(super_ob);
    return result;
   }
   predicted_type_type = DeeXAst_PredictType(ast_a);
   if (predicted_type_type && !DeeType_IsSameOrDerived(predicted_type_type,&DeeType_Type) &&
       predicted_type_type != (DeeTypeObject *)&DeeNone_Type) {
    // Warn if the type argument isn't a type
    if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_TYPE_IN___SUPER,
     (DeeObject *)lexer,(DeeObject *)ast_b->ast_common.ast_token,
     "Expected 'type' as argument in '__super', but got %r",ast_b) != 0) return NULL;
   }
   goto normal_ast;
  } break;

  case DEE_XASTKIND_CALL:
   if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_PREDICATES)!=0 &&
       ast_a->ast_kind == DEE_XASTKIND_FUNCTION && ast_b->ast_kind == DEE_XASTKIND_CONST &&
       ast_b->ast_const.c_const == Dee_EmptyTuple && ast_a->ast_function.f_code) {
    // Optimize '([]{ yield x; })()' --> 'pack(x)'
    DeeSAstObject *code = ast_a->ast_function.f_code;
    if (code->ast_kind == DEE_SASTKIND_YIELD) {
     Dee_INCREF(code->ast_yield.y_value);
     return code->ast_yield.y_value;
    }
   }
   if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_INTRINSIC_CALLS)!=0 &&
       ast_a->ast_kind == DEE_XASTKIND_CONST && ast_b->ast_kind != DEE_XASTKIND_CONST) {
    // Optimize calls to intrinsic functions.
    // >> e.g.: '__inc__(x)' --> '++x'
    DeeObject *called_function;
    Dee_size_t argc,required_argc; DeeXAstObject **argv,**iter,**end;
    if (ast_b->ast_kind != DEE_XASTKIND_TUPLE) goto normal_ast;
    called_function = ast_a->ast_const.c_const;
    if (called_function == (DeeObject *)&DeeBuiltinFunction___builtin_all) return DeeXAst_NewUnary(DEE_XASTKIND_SEQ_ALL,tk,lexer,parser_flags,ast_b);
    if (called_function == (DeeObject *)&DeeBuiltinFunction___builtin_any) return DeeXAst_NewUnary(DEE_XASTKIND_SEQ_ANY,tk,lexer,parser_flags,ast_b);
    if (called_function == (DeeObject *)&DeeBuiltinFunction___builtin_sum) return DeeXAst_NewUnary(DEE_XASTKIND_SEQ_SUM,tk,lexer,parser_flags,ast_b);
    argc = ast_b->ast_tuple.s_elemc;
    argv = ast_b->ast_tuple.s_elemv;
    DEE_ASSERT(argc != 0);
    // Make sure that we can actually determine the argument count
    end = (iter = argv)+argc;
    while (iter != end) {
     if ((*iter)->ast_kind == DEE_XASTKIND_EXPAND) goto normal_ast; // NOPE! We can't
     ++iter;
    }
#define CHECK_ARGC(req_argc) do{if (argc!=(req_argc)){required_argc=(req_argc);goto intrin_invargc;}}while(0)
    if (called_function == (DeeObject *)&DeeUtf8String_Type) {
     if (argc != 1) {
      char const *func_name;
      required_argc = 1;
intrin_invargc:
      if (DeeType_Check(called_function)) func_name = DeeType_NAME(called_function);
      else if (DeeCFunction_Check(called_function)) func_name = DeeCFunction_NAME(called_function);
      else if (DeeFunction_Check(called_function)) func_name = DeeFunction_NAME(called_function)
       ? DeeString_STR(DeeFunction_NAME(called_function)) : NULL;
      else func_name = NULL;
      if (!called_function) DeeError_TypeError_ArgCountExpected(required_argc,argc);
      else DeeError_TypeError_ArgCountExpectedNamed(func_name,required_argc,argc);
      goto handle_binary_error;
     }
     return DeeXAst_NewStr(tk,lexer,parser_flags,argv[0]);
  //} else if (called_function == (DeeObject *)&DeeBool_Type) { CHECK_ARGC(1); return DeeXAst_NewUnary(DEE_XASTKIND_BOOL,tk,lexer,parser_flags,argv[0]); // Not 100% the same
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___repr__) { CHECK_ARGC(1); return DeeXAst_NewRepr(tk,lexer,parser_flags,argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___type__) { CHECK_ARGC(1); return DeeXAst_NewType(tk,lexer,parser_flags,argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___copy__) { CHECK_ARGC(1); return DeeXAst_NewCopy(tk,lexer,parser_flags,argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___move__) { CHECK_ARGC(1); return DeeXAst_NewMove(tk,lexer,parser_flags,argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___copyassign__) { CHECK_ARGC(2); return DeeXAst_NewCopyAssign(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___moveassign__) { CHECK_ARGC(2); return DeeXAst_NewMoveAssign(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___not__) { CHECK_ARGC(1); return DeeXAst_NewNot(tk,lexer,parser_flags,argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___neg__) { CHECK_ARGC(1); return DeeXAst_NewNeg(tk,lexer,parser_flags,argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___pos__) { CHECK_ARGC(1); return DeeXAst_NewPos(tk,lexer,parser_flags,argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___inv__) { CHECK_ARGC(1); return DeeXAst_NewInv(tk,lexer,parser_flags,argv[0]);
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___ref__) { CHECK_ARGC(1); return DeeXAst_NewRef(tk,lexer,parser_flags,argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___deref__) { CHECK_ARGC(1); return DeeXAst_NewDeref(tk,lexer,parser_flags,argv[0]);
#endif
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___inc__) { CHECK_ARGC(1); return DeeXAst_NewInc(tk,lexer,parser_flags,argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___dec__) { CHECK_ARGC(1); return DeeXAst_NewDec(tk,lexer,parser_flags,argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___incpost__) { CHECK_ARGC(1); return DeeXAst_NewIncPost(tk,lexer,parser_flags,argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___decpost__) { CHECK_ARGC(1); return DeeXAst_NewDecPost(tk,lexer,parser_flags,argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___lo__) { CHECK_ARGC(2); return DeeXAst_NewCompareLo(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___le__) { CHECK_ARGC(2); return DeeXAst_NewCompareLe(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___eq__) { CHECK_ARGC(2); return DeeXAst_NewCompareEq(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___ne__) { CHECK_ARGC(2); return DeeXAst_NewCompareNe(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___gr__) { CHECK_ARGC(2); return DeeXAst_NewCompareGr(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___ge__) { CHECK_ARGC(2); return DeeXAst_NewCompareGe(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___add__) { CHECK_ARGC(2); return DeeXAst_NewAdd(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___iadd__) { CHECK_ARGC(2); return DeeXAst_NewInplaceAdd(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___sub__) { CHECK_ARGC(2); return DeeXAst_NewSub(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___isub__) { CHECK_ARGC(2); return DeeXAst_NewInplaceSub(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___mul__) { CHECK_ARGC(2); return DeeXAst_NewMul(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___imul__) { CHECK_ARGC(2); return DeeXAst_NewInplaceMul(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___div__) { CHECK_ARGC(2); return DeeXAst_NewDiv(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___idiv__) { CHECK_ARGC(2); return DeeXAst_NewInplaceDiv(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___mod__) { CHECK_ARGC(2); return DeeXAst_NewMod(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___imod__) { CHECK_ARGC(2); return DeeXAst_NewInplaceMod(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___shl__) { CHECK_ARGC(2); return DeeXAst_NewShl(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___ishl__) { CHECK_ARGC(2); return DeeXAst_NewInplaceShl(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___shr__) { CHECK_ARGC(2); return DeeXAst_NewShr(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___ishr__) { CHECK_ARGC(2); return DeeXAst_NewInplaceShr(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___and__) { CHECK_ARGC(2); return DeeXAst_NewAnd(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___iand__) { CHECK_ARGC(2); return DeeXAst_NewInplaceAnd(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___or__) { CHECK_ARGC(2); return DeeXAst_NewOr(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___ior__) { CHECK_ARGC(2); return DeeXAst_NewInplaceOr(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___xor__) { CHECK_ARGC(2); return DeeXAst_NewXor(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___ixor__) { CHECK_ARGC(2); return DeeXAst_NewInplaceXor(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___pow__) { CHECK_ARGC(2); return DeeXAst_NewPow(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___ipow__) { CHECK_ARGC(2); return DeeXAst_NewInplacePow(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___call__) { CHECK_ARGC(2); return DeeXAst_NewCall(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___size__) { CHECK_ARGC(1); return DeeXAst_NewSeqSize(tk,lexer,parser_flags,argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___getitem__) { CHECK_ARGC(2); return DeeXAst_NewSeqGet(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___setitem__) { CHECK_ARGC(3); return DeeXAst_NewSeqSet(tk,lexer,parser_flags,argv[0],argv[1],argv[2]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___delitem__) { CHECK_ARGC(2); return DeeXAst_NewSeqDel(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___getrange__) { CHECK_ARGC(3); return DeeXAst_NewSeqRangeGet(tk,lexer,parser_flags,argv[0],argv[1],argv[2]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___setrange__) { CHECK_ARGC(4); return DeeXAst_NewSeqRangeSet(tk,lexer,parser_flags,argv[0],argv[1],argv[2],argv[3]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___delrange__) { CHECK_ARGC(3); return DeeXAst_NewSeqRangeDel(tk,lexer,parser_flags,argv[0],argv[1],argv[2]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___is__) { CHECK_ARGC(2); return DeeXAst_NewIs(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___contains__) { CHECK_ARGC(2); return DeeXAst_NewIn(tk,lexer,parser_flags,argv[1],argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___getattr__) { CHECK_ARGC(2); return DeeXAst_NewAttrGet(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___hasattr__) { CHECK_ARGC(2); return DeeXAst_NewAttrHas(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___delattr__) { CHECK_ARGC(2); return DeeXAst_NewAttrDel(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___setattr__) { CHECK_ARGC(3); return DeeXAst_NewAttrSet(tk,lexer,parser_flags,argv[0],argv[1],argv[2]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___iterself__) { CHECK_ARGC(1); return DeeXAst_NewIterSelf(tk,lexer,parser_flags,argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___iternext__) { CHECK_ARGC(1); return DeeXAst_NewIterNext(tk,lexer,parser_flags,argv[0]);
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___builtin_pointer_add) { CHECK_ARGC(1); return DeeXAst_NewPtrOf(tk,lexer,parser_flags,argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___builtin_lvalue_add) { CHECK_ARGC(1); return DeeXAst_NewLValOf(tk,lexer,parser_flags,argv[0]);
#endif
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___dual_posoradd__) {
     if (argc == 1) return DeeXAst_NewPos(tk,lexer,parser_flags,argv[0]);
     CHECK_ARGC(2);
     return DeeXAst_NewAdd(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___dual_negorsub__) {
     if (argc == 1) return DeeXAst_NewNeg(tk,lexer,parser_flags,argv[0]);
     CHECK_ARGC(2);
     return DeeXAst_NewSub(tk,lexer,parser_flags,argv[0],argv[1]);
#if DEE_CONFIG_LANGUAGE_HAVE_POINTERS
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___dual_dereformul__) {
     if (argc == 1) return DeeXAst_NewDeref(tk,lexer,parser_flags,argv[0]);
     CHECK_ARGC(2);
     return DeeXAst_NewMul(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___dual_reforand__) {
     if (argc == 1) return DeeXAst_NewRef(tk,lexer,parser_flags,argv[0]);
     CHECK_ARGC(2);
     return DeeXAst_NewAnd(tk,lexer,parser_flags,argv[0],argv[1]);
#endif /* DEE_CONFIG_LANGUAGE_HAVE_POINTERS */
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___dual_getitemorgetrange__) {
     if (argc == 2) return DeeXAst_NewSeqGet(tk,lexer,parser_flags,argv[0],argv[1]);
     CHECK_ARGC(3);
     return DeeXAst_NewSeqRangeGet(tk,lexer,parser_flags,argv[0],argv[1],argv[2]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___dual_delitemordelrange__) {
     if (argc == 2) return DeeXAst_NewSeqDel(tk,lexer,parser_flags,argv[0],argv[1]);
     CHECK_ARGC(3);
     return DeeXAst_NewSeqRangeDel(tk,lexer,parser_flags,argv[0],argv[1],argv[2]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___dual_setitemorsetrange__) {
     if (argc == 3) return DeeXAst_NewSeqSet(tk,lexer,parser_flags,argv[0],argv[1],argv[2]);
     CHECK_ARGC(4);
     return DeeXAst_NewSeqRangeSet(tk,lexer,parser_flags,argv[0],argv[1],argv[2],argv[3]);
    } else if (DeeNone_Check(called_function) || called_function == (DeeObject *)&DeeNone_Type) {
     DeeXAstObject *used_override_ast;
     if ((result = DeeXAst_NewConst(tk,Dee_None)) == NULL) return NULL;
     if (DeeXAst_IsNoEffect(ast_b)) return result; // Arguments are no-effect, so we can simply return 'none'
     // Repack this as a '__builtin_unused' AST
     used_override_ast = DeeXAst_NewBuiltinUnused(tk,lexer,parser_flags,ast_b,result);
     Dee_DECREF(result);
     return used_override_ast;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___read__) {
     if (argc == 2) return DeeXAst_NewIOReadnp(tk,lexer,parser_flags,argv[0],argv[1]);
     CHECK_ARGC(3);
     return DeeXAst_NewIOReadp(tk,lexer,parser_flags,argv[0],argv[1],argv[2]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___write__) {
     if (argc == 2) return DeeXAst_NewIOWritenp(tk,lexer,parser_flags,argv[0],argv[1]);
     CHECK_ARGC(3);
     return DeeXAst_NewIOWritep(tk,lexer,parser_flags,argv[0],argv[1],argv[2]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___readp__) { CHECK_ARGC(3); return DeeXAst_NewIOReadp(tk,lexer,parser_flags,argv[0],argv[1],argv[2]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___writep__) { CHECK_ARGC(3); return DeeXAst_NewIOWritep(tk,lexer,parser_flags,argv[0],argv[1],argv[2]);
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___readnp__) { CHECK_ARGC(2); return DeeXAst_NewIOReadnp(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___writenp__) { CHECK_ARGC(2); return DeeXAst_NewIOWritenp(tk,lexer,parser_flags,argv[0],argv[1]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___seek__) { CHECK_ARGC(3); return DeeXAst_NewIOSeek(tk,lexer,parser_flags,argv[0],argv[1],argv[2]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___flush__) { CHECK_ARGC(1); return DeeXAst_NewIOFlush(tk,lexer,parser_flags,argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___trunc__) { CHECK_ARGC(1); return DeeXAst_NewIOTrunc(tk,lexer,parser_flags,argv[0]);
    } else if (called_function == (DeeObject *)&DeeBuiltinFunction___close__) { CHECK_ARGC(1); return DeeXAst_NewIOClose(tk,lexer,parser_flags,argv[0]);
    } else;
    goto normal_ast;
#undef CHECK_ARGC
   } /* DEE_PARSER_FLAG_OPTIMIZE_INTRINSIC_CALLS */
   break;

  case DEE_XASTKIND_ATTR_GET:
  case DEE_XASTKIND_ATTR_HAS:
  case DEE_XASTKIND_ATTR_DEL:
   if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_INTRINSIC_CALLS)!=0) {
    // x.operator __getattr__("foobar") --> x.foobar
    if (ast_b->ast_kind != DEE_XASTKIND_CONST) {
     DeeTypeObject const *predicted_b = DeeXAst_PredictType(ast_b);
     if (predicted_b && predicted_b != &DeeString_Type) {
      // Warn is type-prediction says that 'ast_b' isn't a string
      if (DeeError_CompilerErrorf(DEE_WARNING_EXPECTED_STRING_FOR_ATTRIBUTE_OPERATOR,
       (DeeObject *)lexer,(DeeObject *)ast_a->ast_common.ast_token,
       "Expected string for argument to attribute operator, but got instance of %q",
       DeeType_NAME(predicted_b)) != 0) return NULL;
     }
     goto normal_ast;
    }
    if (DeeError_TypeError_CheckTypeExact(ast_b->ast_const.c_const,&DeeString_Type) != 0)
     goto handle_binary_error; // the second argument must be a string
    if (ast_a->ast_kind == DEE_XASTKIND_CONST) {
     int error;
     if ((const_copy_a = DeeObject_DeepCopy(ast_a->ast_const.c_const)) == NULL) goto err_copy;
     switch (kind) {
      case DEE_XASTKIND_ATTR_GET:
       binary_result = DeeObject_GetAttr(const_copy_a,ast_b->ast_const.c_const);
       break;
      case DEE_XASTKIND_ATTR_HAS:
       error = DeeObject_HasAttr(const_copy_a,ast_b->ast_const.c_const);
       binary_result = error < 0 ? NULL : DeeBool_New(error);
       break;
      case DEE_XASTKIND_ATTR_DEL:
       if (DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_ATTR_DEL,lexer,tk,Dee_TYPE(const_copy_a)) != 0) binary_result = NULL;
       else {
        error = DeeObject_DelAttr(const_copy_a,ast_b->ast_const.c_const);
        binary_result = error < 0 ? NULL : DeeBool_New(error);
       }
       break;
      default: DEE_BUILTIN_UNREACHABLE();
     }
     goto handle_binary_result_decref_a;
    }
    switch (kind) {
     // Create constant attribute lookup
     case DEE_XASTKIND_ATTR_GET: return DeeXAst_NewAttrGetC(tk,lexer,parser_flags,ast_a,ast_b->ast_const.c_const);
     case DEE_XASTKIND_ATTR_HAS: return DeeXAst_NewAttrHasC(tk,lexer,parser_flags,ast_a,ast_b->ast_const.c_const);
     case DEE_XASTKIND_ATTR_DEL: return DeeXAst_NewAttrDelC(tk,lexer,parser_flags,ast_a,ast_b->ast_const.c_const);
     default: break;
    }
    DEE_BUILTIN_UNREACHABLE();
   } /* DEE_PARSER_FLAG_OPTIMIZE_INTRINSIC_CALLS */
   break;

  case DEE_XASTKIND_BUILTIN_UNUSED:
   if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH)!=0) {
    if (DeeXAst_IsNoEffect(ast_a)) {
     VLOG_PARSER(tk,"Ignoring noeffect eval-branch in __builtin_unused(%r,%r)\n",ast_a,ast_b);
     Dee_INCREF(ast_b);
     return ast_b;
    }
   }
   goto normal_ast;

  case DEE_XASTKIND_LAND:
   if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH)!=0) {
    if (ast_a->ast_kind == DEE_XASTKIND_CONST) {
     int const_value;
     if (ast_b->ast_kind == DEE_XASTKIND_CONST) break; // this is optimized later
     // 'true && foobar()' >> '!!foobar()'
     // 'false && foobar()' >> '__builtin_unused(__builtin_dead(foobar()),false)'
     if ((const_copy_a = DeeObject_DeepCopy(ast_a->ast_const.c_const)) == NULL) goto err_copy;
     const_value = DeeObject_Bool(const_copy_a);
     Dee_DECREF(const_copy_a);
     if (const_value < 0) goto handle_binary_error;
     if (!const_value) {
      DeeXAstObject *dead_branch,*replace_branch;
      VLOG_PARSER(tk,"Detected dead branch in 'false && %r'\n",ast_b);
      if ((dead_branch = DeeXAst_NewBuiltinDead(tk,lexer,parser_flags,ast_b)) == NULL) return NULL;
      if ((replace_branch = DeeXAst_NewConst(tk,Dee_False)) == NULL) { Dee_DECREF(dead_branch); return NULL; }
      result = DeeXAst_NewBuiltinUnused(tk,lexer,parser_flags,dead_branch,replace_branch);
      Dee_DECREF(replace_branch);
      Dee_DECREF(dead_branch);
      return result;
     }
     return DeeXAst_NewUnary(DEE_XASTKIND_BOOL,tk,lexer,parser_flags,ast_b);
    } else if (ast_b->ast_kind == DEE_XASTKIND_CONST) {
     int const_value; DeeXAstObject *result2;
     // 'foobar() && true' >> '!!foobar()'
     // 'foobar() && false' >> '__builtin_unused(!!foobar(),false)'
     if ((const_copy_b = DeeObject_DeepCopy(ast_b->ast_const.c_const)) == NULL) goto err_copy;
     const_value = DeeObject_Bool(const_copy_b);
     Dee_DECREF(const_copy_b);
     if (const_value < 0) goto handle_binary_error;
     if (const_value) return DeeXAst_NewUnary(DEE_XASTKIND_BOOL,tk,lexer,parser_flags,ast_b);
     if ((result2 = DeeXAst_NewConst(tk,Dee_False)) == NULL) return NULL;
     result = DeeXAst_NewBuiltinUnused(tk,lexer,parser_flags,ast_a,result2);
     Dee_DECREF(result2);
     return result;
    } else {
     goto normal_ast;
    }
   } /* DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH */
   break;

  case DEE_XASTKIND_LOR:
   if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH)!=0) {
    if (ast_a->ast_kind == DEE_XASTKIND_CONST) {
     int const_value;
     if (ast_b->ast_kind == DEE_XASTKIND_CONST) break; // this is optimized later
     // 'true || foobar()' >> '__builtin_unused(__builtin_dead(foobar()),true)'
     // 'false || foobar()' >> '!!foobar()'
     if ((const_copy_a = DeeObject_DeepCopy(ast_a->ast_const.c_const)) == NULL) goto err_copy;
     const_value = DeeObject_Bool(const_copy_a);
     Dee_DECREF(const_copy_a);
     if (const_value < 0) goto handle_binary_error;
     if (const_value) {
      DeeXAstObject *dead_branch,*replace_branch;
      VLOG_PARSER(tk,"Detected dead branch in 'true || %r'\n",ast_b);
      if ((dead_branch = DeeXAst_NewBuiltinDead(tk,lexer,parser_flags,ast_b)) == NULL) return NULL;
      if ((replace_branch = DeeXAst_NewConst(tk,Dee_True)) == NULL) { Dee_DECREF(dead_branch); return NULL; }
      result = DeeXAst_NewBuiltinUnused(tk,lexer,parser_flags,dead_branch,replace_branch);
      Dee_DECREF(replace_branch);
      Dee_DECREF(dead_branch);
      return result;
     }
     return DeeXAst_NewUnary(DEE_XASTKIND_BOOL,tk,lexer,parser_flags,ast_b);
    } else if (ast_b->ast_kind == DEE_XASTKIND_CONST) {
     int const_value; DeeXAstObject *result2;
     // 'foobar() || true' >> '__builtin_unused(!!foobar(),true)'
     // 'foobar() || false' >> '!!foobar()'
     if ((const_copy_b = DeeObject_DeepCopy(ast_b->ast_const.c_const)) == NULL) goto err_copy;
     const_value = DeeObject_Bool(const_copy_b);
     Dee_DECREF(const_copy_b);
     if (const_value < 0) goto handle_binary_error;
     if (!const_value) return DeeXAst_NewUnary(DEE_XASTKIND_BOOL,tk,lexer,parser_flags,ast_b);
     if ((result2 = DeeXAst_NewConst(tk,Dee_False)) == NULL) return NULL;
     result = DeeXAst_NewBuiltinUnused(tk,lexer,parser_flags,ast_a,result2);
     Dee_DECREF(result2);
     return result;
    } else {
     goto normal_ast;
    }
   } /* DEE_PARSER_FLAG_OPTIMIZE_DEAD_BRANCH */
   break;

  default: break;
 }


 if ((parser_flags&DEE_PARSER_FLAG_OPTIMIZE_CONST_OPERATORS)!=0 &&
     ast_a->ast_kind == DEE_XASTKIND_CONST &&
     ast_b->ast_kind == DEE_XASTKIND_CONST) {
  // Fast-track for asts that we don't need to copy the constant for
  switch (kind) {
   case DEE_XASTKIND_CALL: {
    DeeObject *const_function = ast_a->ast_const.c_const;
    if (!_DeeBuiltin_AllowConstantCall(const_function) &&
        !DeeNone_Check(const_function) &&
        // v memberfunctions can only be retrieved if they are allowed at compile-time
        !DeeMemberFunction_Check(const_function)) {
     if (!DeeType_Check(const_function)) goto normal_ast; // This type isn't constexpr
     if ((DeeType_FLAGS(const_function)&DEE_TYPE_FLAG_CONST_CTOR) == 0
         ) goto normal_ast; // This function/type isn't constexpr
     // Pointer casts can only be evaluated at compile-time,
     // if the argument is an integral:
     // >> x = (char *)0;     // OK: Evaluate at compile-time
     // >> x = (char *)"foo"; // NOPE: Evaluate at runtime
     // This is required, because "foo" must exist as a constant
     // at runtime, so as to still be alive when working with the pointer.
     if (DeePointerType_Check(const_function)) {
      DEE_ASSERT(ast_b->ast_kind == DEE_XASTKIND_CONST);
      if (!DeeTuple_Check(ast_b->ast_const.c_const)) goto normal_ast;
      if (DeeTuple_SIZE(ast_b->ast_const.c_const) == 1) {
       struct DeeTypeMarshal *marshal;
       DeeTypeObject *single_elem_type = Dee_TYPE(DeeTuple_GET(ast_b->ast_const.c_const,0));
       marshal = DeeType_GET_SLOT(single_elem_type,tp_marshal);
       if (marshal == DeeType_DEFAULT_SLOT(tp_marshal)) goto normal_ast;
       DEE_ASSERT(marshal);
       if (!DeeUUID_CHECK_INTERNAL(&marshal->tp_uuid)) goto normal_ast;
       switch (DeeUUID_GET_INTERNAL(&marshal->tp_uuid)) {
        case DEE_MARSHALID_NONE:
        case DEE_MARSHALTYPEID_int8:   case DEE_MARSHALTYPEID_atomic_int8:
        case DEE_MARSHALTYPEID_int16:  case DEE_MARSHALTYPEID_atomic_int16:
        case DEE_MARSHALTYPEID_int32:  case DEE_MARSHALTYPEID_atomic_int32:
        case DEE_MARSHALTYPEID_int64:  case DEE_MARSHALTYPEID_atomic_int64:
        case DEE_MARSHALTYPEID_uint8:  case DEE_MARSHALTYPEID_atomic_uint8:
        case DEE_MARSHALTYPEID_uint16: case DEE_MARSHALTYPEID_atomic_uint16:
        case DEE_MARSHALTYPEID_uint32: case DEE_MARSHALTYPEID_atomic_uint32:
        case DEE_MARSHALTYPEID_uint64: case DEE_MARSHALTYPEID_atomic_uint64:
         // v these don't really make much sense, but no harm's done by allowing this...
        case DEE_MARSHALTYPEID_float4: case DEE_MARSHALTYPEID_float8:
        case DEE_MARSHALTYPEID_float12: case DEE_MARSHALTYPEID_float16:
         break;
        default:
         // We'll try to cast this one to an integer later.
         if (DeeType_IsSameOrDerived(single_elem_type,&DeeStructuredType_Type)) break;
         goto normal_ast;
       }
      }
     }
    }
   } break;
   case DEE_XASTKIND_IS:
    binary_result = DeeBool_New(DeeObject_Is(
     ast_a->ast_const.c_const,(DeeTypeObject *)
     ast_b->ast_const.c_const));
    goto return_binary_result;
   case DEE_XASTKIND_NOT_IS:
    binary_result = DeeBool_New(!DeeObject_Is(
     ast_a->ast_const.c_const,(DeeTypeObject *)
     ast_b->ast_const.c_const));
    goto return_binary_result;
   case DEE_XASTKIND_LAND:
   case DEE_XASTKIND_LOR: {
    int error; // Optimize && and ||
    if ((const_copy_a = DeeObject_DeepCopy(ast_a->ast_const.c_const)) == NULL) goto err_copy;
    error = DeeObject_Bool(const_copy_a);
    Dee_DECREF(const_copy_a);
    if (error < 0) goto handle_binary_error;
    if (kind == DEE_XASTKIND_LAND ? !error : error) {
     // The result is already determined (don't evaluate 'b', as it wouldn't be evaluated at runtime either)
     Dee_INCREF(binary_result = kind == DEE_XASTKIND_LAND ? Dee_False : Dee_True);
     goto return_binary_result;
    }
    // B will be evaluated, and the result of the expression is equal to it
    if ((const_copy_b = DeeObject_DeepCopy(ast_b->ast_const.c_const)) == NULL) goto err_copy;
    error = DeeObject_Bool(const_copy_b);
    Dee_DECREF(const_copy_b);
    if (error < 0) goto handle_binary_error;
    binary_result = DeeBool_New(error);
    goto return_binary_result;
   } break;
   default: break;
  }
  if ((const_copy_a = DeeObject_DeepCopy(ast_a->ast_const.c_const)) == NULL) {
err_copy:
   // Ignore errors in copy, as the copy-operation wouldn't be performed at runtime
   DeeError_Handled();
   goto normal_ast;
  }
  if ((const_copy_b = DeeObject_DeepCopy(ast_b->ast_const.c_const)) == NULL) {
   Dee_DECREF(const_copy_a);
   goto err_copy;
  }
  switch (kind) {
   case DEE_XASTKIND_CALL:
    if (DeeError_TypeError_CheckTypeExact(const_copy_b,&DeeTuple_Type) == 0)
     binary_result = DeeObject_Call(const_copy_a,const_copy_b);
    else binary_result = NULL;
    break;
   case DEE_XASTKIND_COMPARE_LO: binary_result = DeeObject_CompareLoObject(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_COMPARE_LE: binary_result = DeeObject_CompareLeObject(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_COMPARE_EQ: binary_result = DeeObject_CompareEqObject(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_COMPARE_NE: binary_result = DeeObject_CompareNeObject(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_COMPARE_GR: binary_result = DeeObject_CompareGrObject(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_COMPARE_GE: binary_result = DeeObject_CompareGeObject(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_ADD: binary_result = DeeObject_Add(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_SUB: binary_result = DeeObject_Sub(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_MUL: binary_result = DeeObject_Mul(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_DIV: binary_result = DeeObject_Div(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_MOD: binary_result = DeeObject_Mod(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_SHL: binary_result = DeeObject_Shl(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_SHR: binary_result = DeeObject_Shr(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_AND: binary_result = DeeObject_And(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_OR: binary_result = DeeObject_Or(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_XOR: binary_result = DeeObject_Xor(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_POW: binary_result = DeeObject_Pow(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_IADD: binary_result = DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_IADD,lexer,tk,Dee_TYPE(const_copy_a)) != 0 ? NULL : DeeObject_InplaceAdd(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_ISUB: binary_result = DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_ISUB,lexer,tk,Dee_TYPE(const_copy_a)) != 0 ? NULL : DeeObject_InplaceSub(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_IMUL: binary_result = DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_IMUL,lexer,tk,Dee_TYPE(const_copy_a)) != 0 ? NULL : DeeObject_InplaceMul(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_IDIV: binary_result = DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_IDIV,lexer,tk,Dee_TYPE(const_copy_a)) != 0 ? NULL : DeeObject_InplaceDiv(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_IMOD: binary_result = DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_IMOD,lexer,tk,Dee_TYPE(const_copy_a)) != 0 ? NULL : DeeObject_InplaceMod(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_ISHL: binary_result = DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_ISHL,lexer,tk,Dee_TYPE(const_copy_a)) != 0 ? NULL : DeeObject_InplaceShl(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_ISHR: binary_result = DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_ISHR,lexer,tk,Dee_TYPE(const_copy_a)) != 0 ? NULL : DeeObject_InplaceShr(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_IAND: binary_result = DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_IAND,lexer,tk,Dee_TYPE(const_copy_a)) != 0 ? NULL : DeeObject_InplaceAnd(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_IOR: binary_result = DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_IOR,lexer,tk,Dee_TYPE(const_copy_a)) != 0 ? NULL : DeeObject_InplaceOr(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_IXOR: binary_result = DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_IXOR,lexer,tk,Dee_TYPE(const_copy_a)) != 0 ? NULL : DeeObject_InplaceXor(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_IPOW: binary_result = DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_IPOW,lexer,tk,Dee_TYPE(const_copy_a)) != 0 ? NULL : DeeObject_InplacePow(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_SEQ_GET: binary_result = DeeObject_GetItem(const_copy_a,const_copy_b); break;
   case DEE_XASTKIND_SEQ_DEL: binary_result = binary_result = DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_SEQ_DEL,lexer,tk,Dee_TYPE(const_copy_a)) != 0 ? NULL : DeeObject_DelItem(const_copy_a,const_copy_b) != 0 ? NULL : DeeNone_New(); break;
   case DEE_XASTKIND_IN: binary_result = DeeObject_ContainsObject(const_copy_b,const_copy_a); break;
   case DEE_XASTKIND_NOT_IN: {
    int error;
    error = DeeObject_Contains(const_copy_b,const_copy_a);
    binary_result = error < 0 ? NULL : DeeBool_New(!error);
   } break;
   case DEE_XASTKIND_LXOR: {
    int bool_a,bool_b;
    if ((bool_a = DeeObject_Bool(const_copy_a)) < 0) binary_result = NULL;
    else if ((bool_b = DeeObject_Bool(const_copy_b)) < 0) binary_result = NULL;
    else binary_result = DeeBool_New(!!bool_a ^ !!bool_b);
   } break;
   case DEE_XASTKIND_MOVE_ASSIGN:
    if (DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_MOVE_ASSIGN,lexer,tk,Dee_TYPE(const_copy_a)) != 0) binary_result = NULL;
    else if (DeeObject_MoveAssign(const_copy_a,const_copy_b) != 0) binary_result = NULL;
    else Dee_INCREF(binary_result = ast_a->ast_const.c_const);
    break;
   case DEE_XASTKIND_COPY_ASSIGN:
    if (DeeXAst_WarnInplaceOnConstant(DEE_XASTKIND_COPY_ASSIGN,lexer,tk,Dee_TYPE(const_copy_a)) != 0) binary_result = NULL;
    else if (DeeObject_CopyAssign(const_copy_a,const_copy_b) != 0) binary_result = NULL;
    else Dee_INCREF(binary_result = ast_a->ast_const.c_const);
    break;
   default://normal_ast_const_copy:
    Dee_DECREF(const_copy_b);
    Dee_DECREF(const_copy_a);
    goto normal_ast;
  }
  Dee_DECREF(const_copy_b);
handle_binary_result_decref_a:
  Dee_DECREF(const_copy_a);
//handle_binary_result:
  if (!binary_result) {
   DeeTypeObject *rt_error;
handle_binary_error:
   if ((rt_error = _DeeError_HandleAndReturnType()) != NULL) {
    int temp = DeeXAst_ThrowInvalidBinaryOperatorError(kind,tk,lexer,ast_a,ast_b,rt_error);
    Dee_DECREF(rt_error);
    if (temp != 0) return NULL;
   }
  } else {
return_binary_result:
   result = DeeXAst_NewConst(tk,binary_result);
   Dee_DECREF(binary_result);
   return result;
  }
 }
normal_ast:
 if ((result = _DeeXAst_NewUnsafe(kind,tk)) != NULL) {
  Dee_INCREF(result->ast_operator.op_a = ast_a);
  Dee_INCREF(result->ast_operator.op_b = ast_b);
 }
 return result;
}

DEE_DECL_END
