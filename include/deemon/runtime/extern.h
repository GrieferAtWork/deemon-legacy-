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
#ifndef GUARD_DEEMON_EXTERN_H
#define GUARD_DEEMON_EXTERN_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/string_forward.h>
#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#include <deemon/string.h>
#include <deemon/foreign_function.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif


DEE_OBJECT_DEF(DeeSharedLibObject);
DEE_OBJECT_DEF(DeeExternFunctionObject);

#ifdef DEE_LIMITED_API
struct DeeSharedLibObject {
 DEE_OBJECT_HEAD
#ifndef DEE_PLATFORM_WINDOWS
 // On windows we can use 'DeeFS_Win32GetModuleName' instead of this
 DEE_A_REF DeeAnyStringObject *sl_name;   /*< [1..1] Name of the shared library. */
#endif
 void                         *sl_handle; /*< [?..?] System-specific shared library handle. */
};
#define DeeSharedLib_HANDLE(ob)   ((DeeSharedLibObject *)Dee_REQUIRES_POINTER(ob))->sl_handle
#else
#define DeeSharedLib_HANDLE(ob)     DeeSharedLib_Handle((DeeObject *)Dee_REQUIRES_POINTER(ob))
#endif

#define DeeSharedLib_Check(ob)      DeeObject_InstanceOf(ob,&DeeSharedLib_Type)
#define DeeSharedLib_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeSharedLib_Type)
DEE_DATA_DECL(DeeTypeObject) DeeSharedLib_Type;
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeSharedLibObject) *) DeeSharedLib_Utf8New(DEE_A_IN_Z Dee_Utf8Char const *filename) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeSharedLibObject) *) DeeSharedLib_WideNew(DEE_A_IN_Z Dee_WideChar const *filename) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeSharedLibObject) *) DeeSharedLib_NewObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(NULL) void *) DeeSharedLib_Import(DEE_A_IN_OBJECT(DeeSharedLibObject) const *self, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeSharedLib_Name(DEE_A_IN_OBJECT(DeeSharedLibObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *) DeeSharedLib_Utf8Name(DEE_A_IN_OBJECT(DeeSharedLibObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *) DeeSharedLib_WideName(DEE_A_IN_OBJECT(DeeSharedLibObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(NULL) void *) DeeSharedLib_TryImport(DEE_A_IN_OBJECT(DeeSharedLibObject) const *self, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED void *) DeeSharedLib_Handle(DEE_A_IN_OBJECT(DeeSharedLibObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#define DeeSharedLib_New DeeSharedLib_Utf8New


DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(NULL) void *) DeeSharedLib_Utf8PoolTryImport(DEE_A_IN_Z Dee_Utf8Char const *filename, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(NULL) void *) DeeSharedLib_WidePoolTryImport(DEE_A_IN_Z Dee_WideChar const *filename, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(NULL) void *) DeeSharedLib_PoolTryImportObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(NULL) void *) DeeSharedLib_Utf8PoolImport(DEE_A_IN_Z Dee_Utf8Char const *filename, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(NULL) void *) DeeSharedLib_WidePoolImport(DEE_A_IN_Z Dee_WideChar const *filename, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(NULL) void *) DeeSharedLib_PoolImportObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT(DeeSharedLibObject) *) DeeSharedLib_Utf8PoolLoad(DEE_A_IN_Z Dee_Utf8Char const *filename) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT(DeeSharedLibObject) *) DeeSharedLib_WidePoolLoad(DEE_A_IN_Z Dee_WideChar const *filename) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT(DeeSharedLibObject) *) DeeSharedLib_PoolLoadObject(DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename) DEE_ATTRIBUTE_NONNULL((1));
#define DeeSharedLib_PoolTryImport DeeSharedLib_Utf8PoolTryImport
#define DeeSharedLib_PoolImport    DeeSharedLib_Utf8PoolImport
#define DeeSharedLib_PoolLoad      DeeSharedLib_Utf8PoolLoad

#ifdef DEE_LIMITED_API
extern void DeeSharedLib_Shutdown(void);
#endif

#if DEE_CONFIG_RUNTIME_HAVE_EXTERN
//////////////////////////////////////////////////////////////////////////
// Loads an external object from a dll/so
//  - The returned object has a typing of 'DeeType_LValue(object_type)'
//  - 'filename', 'object_name' and 'object_type' can have any typing
//    (invalid types are handled by this function)
// NOTE: This is the same as using '__builtin_extern' in usercode
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeLValueObject) *) DeeSharedLib_LoadExtern(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *filename,
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *object_name,
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *object_type)
 DEE_ATTRIBUTE_NONNULL((1,2,3));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeLValueObject) *) DeeSharedLib_LoadAutoExtern(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *object_name,
 DEE_A_IN_TYPEOBJECT(DeeStructuredTypeObject) const *object_type)
 DEE_ATTRIBUTE_NONNULL((1,2));
#endif /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */

DEE_DECL_END

#endif /* !GUARD_DEEMON_EXTERN_H */
