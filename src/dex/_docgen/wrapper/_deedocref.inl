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
#ifndef GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEDOCREF_INL
#define GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEDOCREF_INL 1
#ifdef __INTELLISENSE__
#include "../_docgen.c"
#endif

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// DeeGenericDocRefObject
static int _deedocref_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeGenericDocRefObject *self, DeeGenericDocRefObject *right) {
 Dee_INCREF(self->dr_keepalive = right->dr_keepalive);
 self->dr_doc = right->dr_doc;
 return 0;
}
static void _deedocref_tp_dtor(DeeGenericDocRefObject *self) {
 Dee_DECREF(self->dr_keepalive);
}
DEE_VISIT_PROC(_deedocref_tp_visit,DeeGenericDocRefObject *self) {
 Dee_VISIT(self->dr_keepalive);
}

DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DOCGEN_WRAPPER_DEEDOCREF_INL */
