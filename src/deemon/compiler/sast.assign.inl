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
#include "sast.c"
#endif

DEE_DECL_BEGIN

#define _DeeSAstCommonAst_InitMove(ob,right)\
do{\
 (ob)->ast_kind_ = (right)->ast_kind_;\
 Dee_INCREF((ob)->ast_token = (right)->ast_token);\
 (ob)->ast_labels = (right)->ast_labels;\
 _DeeParserLabelRefList_Init(&(right)->ast_labels);\
}while(0)



void DeeSAst_AssignEmpty(DEE_A_INOUT DeeSAstObject *self) {
 _DeeSAst_DestroyUncommon(self);
 self->ast_kind = DEE_SASTKIND_EMPTY;
}
void DeeSAst_InitMove(DEE_A_OUT DeeSAstObject *self, DEE_A_INOUT DeeSAstObject *right) {
 _DeeSAstCommonAst_InitMove(&self->ast_common,&right->ast_common);
 memcpy(DeeSAst_UNCOMMON_DATA(self),DeeSAst_UNCOMMON_DATA(right),DeeSAst_UNCOMMON_SIZE);
#ifdef DEE_DEBUG
 memset(DeeSAst_UNCOMMON_DATA(right),0,DeeSAst_UNCOMMON_SIZE);
#endif
 right->ast_kind = DEE_SASTKIND_EMPTY;
}
void DeeSAst_AssignMove(DEE_A_INOUT DeeSAstObject *self, DEE_A_INOUT DeeSAstObject *right) {
 if (self == right) return;
 Dee_INCREF(right); // If 'right' is part of the sub-set of 'self'
 _deesast_tp_dtor(self);
 DeeSAst_InitMove(self,right);
 Dee_DECREF(right);
}

DEE_DECL_END
