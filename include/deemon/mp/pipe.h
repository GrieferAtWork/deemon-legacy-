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
#ifndef GUARD_DEEMON_PIPE_H
#define GUARD_DEEMON_PIPE_H 1

#include <deemon/__conf.inl>
#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#endif

#if DEE_PLATFORM_HAVE_PIPES

//////////////////////////////////////////////////////////////////////////
//
// WARNING:
//    -- The pipe API will be moved into a Dex sooner or later --
//   This header will remain, and it will still be possible to use the API,
//   but additional code may be required to load the API before use,
//   as well as binary compatibility with any function/object declared
//   by this header being broken.
//
//////////////////////////////////////////////////////////////////////////

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_FILE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_FILE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_FILE_TYPEOBJECT
#endif

DEE_OBJECT_DEF(DeePipeObject);

#ifdef DEE_LIMITED_API
#ifdef DEE_PRIVATE_DECL_DEE_IO_HANDLE
DEE_PRIVATE_DECL_DEE_IO_HANDLE
#undef DEE_PRIVATE_DECL_DEE_IO_HANDLE
#endif
struct DeePipeObject {
 DEE_FILE_OBJECT_HEAD
 DeeIOHandle p_handle;
};
#define DeePipe_HANDLE(ob)  ((DeePipeObject *)Dee_REQUIRES_POINTER(ob))->p_handle
#endif


DEE_DATA_DECL(DeeFileTypeObject) DeePipe_Type;
DEE_DATA_DECL(DeeFileTypeObject) DeePipeReader_Type;
DEE_DATA_DECL(DeeFileTypeObject) DeePipeWriter_Type;
#define DeePipe_Check(ob)            DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeePipe_Type)
#define DeePipe_CheckExact(ob)       DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeePipe_Type)
#define DeePipeReader_Check(ob)      DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeePipeReader_Type)
#define DeePipeReader_CheckExact(ob) DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeePipeReader_Type)
#define DeePipeWriter_Check(ob)      DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeePipeWriter_Type)
#define DeePipeWriter_CheckExact(ob) DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeePipeWriter_Type)

//////////////////////////////////////////////////////////////////////////
// Create a new pipe and return the read/write portion in 'read' and 'write'
DEE_FUNC_DECL(DEE_A_INTERRUPT DEE_A_RET_EXCEPT(-1) int) DeePipe_New(
 DEE_A_REF DEE_A_OUT_OBJECT(DeePipeObject) **reader,
 DEE_A_REF DEE_A_OUT_OBJECT(DeePipeObject) **writer,
 DEE_A_IN Dee_size_t size_hint) DEE_ATTRIBUTE_NONNULL((1,2));

#ifdef DEE_LIMITED_API
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeePipeObject) *_DeePipeReader_NewFromInheritedHandle(DEE_A_IN DeeIOHandle handle);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeePipeObject) *_DeePipeWriter_NewFromInheritedHandle(DEE_A_IN DeeIOHandle handle);
#endif

DEE_DECL_END
#endif /* DEE_PLATFORM_HAVE_PIPES */


#endif /* !GUARD_DEEMON_PIPE_H */
