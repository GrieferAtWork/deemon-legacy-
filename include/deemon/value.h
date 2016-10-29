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
#ifndef GUARD_DEEMON_VALUE_H
#define GUARD_DEEMON_VALUE_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/std/stdarg.h>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif

//////////////////////////////////////////////////////////////////////////
// Builds a deemon object from a given format and arguments
//  - Format identifiers are identical to those used in 'DeeTuple_Unpack'
//  - You can use parenthesis '(...)' to create tuple objects
//  - You can use brackets '[...]' to create list objects
//  - You can use braces '<...>' to create set objects
//  - You can use braces '{...}' to create dict objects (NOTE: Every first argument is the key & every second the item)
// e.g.: >> args = Dee_BuildValue("(dds)",10,20,"foobar");
//       >> // 'args' is a 3-element tuple containing (int,int,string)
//       >> DeeObject_CallAttrString(self,"fun",args);
// 'Dee_BuildTuple' is the same as 'Dee_BuildValue' if an
// additional pair of parenthesis were added to 'fmt'
// NOTES:
//   - Uppercase object letters once again decref the associated argument
//     And just as with string formatting, it is guarantied that this is
//     always done (even after an error occurred).
//     - k/K: Run 'DeeObject_Str' on the argument
//     - r/R: Run 'DeeObject_Repr' on the argument
//     - o/O: Simply return the given 'DeeObject *' ('O' will inherit the reference)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_REF DeeObject *) Dee_BuildValue(
 DEE_A_IN_BUILDVALUEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) Dee_BuildTuple(
 DEE_A_IN_BUILDTUPLEF char const *fmt, ...) DEE_ATTRIBUTE_NONNULL((1));
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_REF DeeObject *) Dee_VBuildValue(
 DEE_A_IN_BUILDVALUEF char const *fmt, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTupleObject) *) Dee_VBuildTuple(
 DEE_A_IN_BUILDTUPLEF char const *fmt, DEE_A_IN va_list args) DEE_ATTRIBUTE_NONNULL((1));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */

DEE_DECL_END

#endif /* !GUARD_DEEMON_VALUE_H */
