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
#ifndef GUARD_DEEMON_OPTIONAL_OBJECT_DOC_H
#define GUARD_DEEMON_OPTIONAL_OBJECT_DOC_H 1

#include <deemon/__conf.inl>
#if DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP
#include <deemon/dex.h>
#include <deemon/super.h>

DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// Returns the help string associated with a given object / attribute
// Note, that calling 'DeeObject_Doc' with the result of DeeObject_GetAttr
// does not necessarily yield the same doc as calling 'DeeObject_DocAttr'
// NOTE: Object documentation has been moved into a dex, as should
//       be obvious by the these functions changing to macros.
#define /* DEE_A_EXEC DeeStringObject * */ DeeObject_Doc(self)                DeeDex_Callf("_docgen.__getdoc__(o)",self)
#define /* DEE_A_EXEC DeeStringObject * */ DeeObject_DocAttr(self,attr)       DeeDex_Callf("_docgen.__getdoc__(oo)",self,attr)
#define /* DEE_A_EXEC DeeStringObject * */ DeeObject_DocAttrString(self,attr) DeeDex_Callf("_docgen.__getdoc__(os)",self,attr)
#define /* DEE_A_EXEC DeeStringObject * */ DeeObject_TDoc(tp_self,self) \
 (Dee_TYPE(self) == tp_self ? DeeObject_Doc(self)\
  : DeeDex_Callf("_docgen.__getdoc__(O)",DeeSuper_New(tp_self,(DeeObject *)(self))))
#define /* DEE_A_EXEC DeeStringObject * */ DeeObject_TDocAttr(tp_self,self,attr) \
 (Dee_TYPE(self) == tp_self ? DeeObject_DocAttr(self,attr)\
  : DeeDex_Callf("_docgen.__getdoc__(Oo)",DeeSuper_New(tp_self,(DeeObject *)(self)),attr))
#define /* DEE_A_EXEC DeeStringObject * */ DeeObject_TDocAttrString(tp_self,self,attr) \
 (Dee_TYPE(self) == tp_self ? DeeObject_DocAttrString(self,attr)\
  : DeeDex_Callf("_docgen.__getdoc__(Os)",DeeSuper_New(tp_self,(DeeObject *)(self)),attr))

DEE_DECL_END
#endif /* !DEE_CONFIG_RUNTIME_HAVE_BUILTIN_HELP */

#endif /* !GUARD_DEEMON_OPTIONAL_OBJECT_DOC_H */
