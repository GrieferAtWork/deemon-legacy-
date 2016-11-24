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
#ifndef GUARD_DEEMON_FS_NATIVE_VIEW_H
#define GUARD_DEEMON_FS_NATIVE_VIEW_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/object_decl.h>
#include <deemon/optional/string_forward.h>
#include <deemon/type.h>
#include <deemon/fs/native_view.utf8.h>
#include <deemon/fs/native_view.wide.h>

DEE_DECL_BEGIN

/*[[[deemon
#include <file>
const begin_tag = "@DeeSysFSWideView:begin";
const end_tag   = "@DeeSysFSWideView:end";
text = file.open(__FILE__,"r").read();
begin = text.rindex(begin_tag)+#begin_tag;
end = text.rindex(end_tag,begin);
text = text[begin:end];

text = text.replace("wide","utf8");
text = text.replace("Wide","Utf8");
text = text.replace("WIDE","UTF8");
text = text.strip().rsstrip("//").strip();
print text;
]]]*/
#ifdef DeeNFSUtf8View
DEE_OBJECT_DEF(DeeNFSUtf8ViewObject);
struct DeeNFSUtf8ViewObject {
 DEE_OBJECT_HEAD
 struct DeeNFSUtf8View nwv_view;
};
extern DeeTypeObject DeeNFSUtf8View_Type;

extern DEE_A_RET_EXCEPT_REF DeeNFSUtf8ViewObject *
 DeeNFSUtf8View_OpenPath(DEE_A_IN_Z Dee_Utf8Char const *path);
extern DEE_A_RET_EXCEPT_REF DeeNFSUtf8ViewObject *
 DeeNFSUtf8View_OpenPathObject(DEE_A_IN_OBJECT(DeeUtf8StringObject) const *path);
#endif
//[[[end]]]

//@DeeSysFSWideView:begin
#ifdef DeeNFSWideView
DEE_OBJECT_DEF(DeeNFSWideViewObject);
struct DeeNFSWideViewObject {
 DEE_OBJECT_HEAD
 struct DeeNFSWideView nwv_view;
};
extern DeeTypeObject DeeNFSWideView_Type;

extern DEE_A_RET_EXCEPT_REF DeeNFSWideViewObject *
 DeeNFSWideView_OpenPath(DEE_A_IN_Z Dee_WideChar const *path);
extern DEE_A_RET_EXCEPT_REF DeeNFSWideViewObject *
 DeeNFSWideView_OpenPathObject(DEE_A_IN_OBJECT(DeeWideStringObject) const *path);
#endif
//@DeeSysFSWideView:end

DEE_DECL_END

#endif /* !GUARD_DEEMON_FS_NATIVE_VIEW_H */
