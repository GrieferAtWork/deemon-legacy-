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
#ifndef GUARD_DEEMON_MEMBERDEF_H
#define GUARD_DEEMON_MEMBERDEF_H 1

#include <deemon/__conf.inl>

#ifdef DEE_LIMITED_DEX
#include <deemon/__typecompatible.inl>
#endif

#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDDEF_H
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#include <stddef.h>
#include DEE_INCLUDE_MEMORY_API_ENABLE()
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
#ifdef DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
#undef DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
#endif
#ifdef DEE_PRIVATE_DECL_DEE_STRUCTURED_MEMBERFUNCTION
DEE_PRIVATE_DECL_DEE_STRUCTURED_MEMBERFUNCTION
#undef DEE_PRIVATE_DECL_DEE_STRUCTURED_MEMBERFUNCTION
#endif

struct DeeMemberDef;

enum{
 DeeMemberType_none = 0, // Always return Dee_None, ignore offset
 DeeMemberType_int8_t = 1,
 DeeMemberType_int16_t = 2,
 DeeMemberType_int32_t = 3,
 DeeMemberType_int64_t = 4,
 DeeMemberType_uint8_t = 5,
 DeeMemberType_uint16_t = 6,
 DeeMemberType_uint32_t = 7,
 DeeMemberType_uint64_t = 8,
 DeeMemberType_float = 9,
 DeeMemberType_double = 10,
 DeeMemberType_ldouble = 11,
 DeeMemberType_utf8char = 12,           //                char        (Output as string)
 DeeMemberType_utf8string = 13,         //                char *      ([readonly][0..1])
 DeeMemberType_inplace_utf8string = 14, //                char[N]     ([readonly] \0 terminated string directly within the structure)
 DeeMemberType_object = 15,             // DEE_A_REF      DeeObject * (raises AttributeError if NULL)
 DeeMemberType_object_null = 16,        // DEE_A_REF      DeeObject * (return Dee_None if NULL)
 DeeMemberType_weak_object = 17,        // DEE_A_WEAK_REF DeeObject * (raises AttributeError if NULL)
 DeeMemberType_weak_object_null = 18,   // DEE_A_WEAK_REF DeeObject * (return Dee_None if NULL)

 // Misc types
 DeeMemberType_time_t_astime = 19,
 DeeMemberType_local_time_t_astime = 20,
 DeeMemberType_struct_tm = 21, // When combined with a pointer, still returns a time object
#ifdef DEE_PLATFORM_WINDOWS
 DeeMemberType__SYSTEMTIME = 22,     // When combined with a pointer, still returns a time object
 DeeMemberType__FILETIME = 23,       // When combined with a pointer, still returns a time object
 DeeMemberType_local__FILETIME = 24, // When combined with a pointer, still returns a time object
#endif

 // -- New in v101: all the new string types
 DeeMemberType_widechar = 25,             // wchar_t
 DeeMemberType_widestring = 26,           // wchar_t *   ([readonly][0..1])
 DeeMemberType_inplace_widestring = 27,   // wchar_t[N]  ([readonly] \0 terminated string directly within the structure)
 DeeMemberType_utf16char = 28,            // char16_t
 DeeMemberType_utf16string = 29,          // char16_t *  ([readonly][0..1])
 DeeMemberType_inplace_utf16string = 30,  // char16_t[N] ([readonly] \0 terminated string directly within the structure)
 DeeMemberType_utf32char = 31,            // char32_t
 DeeMemberType_utf32string = 32,          // char32_t *  ([readonly][0..1])
 DeeMemberType_inplace_utf32string = 33,  // char32_t[N] ([readonly] \0 terminated string directly within the structure)

 // Alias for c-default string/char types
 DeeMemberType_char = DeeMemberType_utf8char,
 DeeMemberType_string = DeeMemberType_utf8string,
 DeeMemberType_inplace_string = DeeMemberType_inplace_utf8string,

#define DeeMemberType_POINTER(x)\
 (DeeMemberType_POINTER_X1+(x))
 DeeMemberType_POINTER_X1     = 0x01000000,
 DeeMemberType_POINTER_X2     = 0x02000000,
 DeeMemberType_POINTER_X3     = 0x03000000,
 DeeMemberType_POINTER_X4     = 0x04000000,
 DeeMemberType_POINTER_X5     = 0x05000000,
 DeeMemberType_POINTER_X6     = 0x06000000,
 DeeMemberType_POINTER_X7     = 0x07000000,
 DeeMemberType_POINTER_X8     = 0x08000000,
 DeeMemberType_POINTER_X9     = 0x09000000,
 DeeMemberType_POINTER_MASK   = 0xFF000000,
 // ...

 // Flag: read / write operations are atomic
 // NOTE: Illegal when used with INPLACE_STRING
 // NOTE: memory_order_seq_cst is used for read / write
 DeeMemberType_ATOMIC = 0x00100000,

 // Flag: Treat the offset as a constant integral expression
 //    >> With this flag set, the offset is used as a constant
 DeeMemberType_CONST  = 0x00200000,
#define DEE_PRIVATE_MEMBERTYPE_XINT_0  DeeMemberType_UINT
#define DEE_PRIVATE_MEMBERTYPE_XINT_1  DeeMemberType_INT
#define DeeMemberType_INT(sizeof)         DEE_PP_CAT_3(DeeMemberType_int,DEE_PP_MUL8(sizeof),_t)
#define DeeMemberType_UINT(sizeof)        DEE_PP_CAT_3(DeeMemberType_uint,DEE_PP_MUL8(sizeof),_t)
#define DeeMemberType_XINT(signed,sizeof) DEE_PP_CAT_2(DEE_PRIVATE_MEMBERTYPE_XINT_,signed)(sizeof)
 DeeMemberType_void = DeeMemberType_none,
 //DeeMemberType_pointer = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_DeeObject = DeeMemberType_object,
#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_FLOAT
 DeeMemberType_Dee_rt_float = DeeMemberType_float,
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_DOUBLE
 DeeMemberType_Dee_rt_float = DeeMemberType_double,
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_FLOAT == DEE_TYPES_FLOATID_LDOUBLE
 DeeMemberType_Dee_rt_float = DeeMemberType_ldouble,
#endif
#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_FLOAT
 DeeMemberType_Dee_rt_double = DeeMemberType_float,
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_DOUBLE
 DeeMemberType_Dee_rt_double = DeeMemberType_double,
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_DOUBLE == DEE_TYPES_FLOATID_LDOUBLE
 DeeMemberType_Dee_rt_double = DeeMemberType_ldouble,
#endif
#if defined(DEE_TYPES_FLOATID_FLOAT) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_FLOAT
 DeeMemberType_Dee_rt_ldouble = DeeMemberType_float,
#elif defined(DEE_TYPES_FLOATID_DOUBLE) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_DOUBLE
 DeeMemberType_Dee_rt_ldouble = DeeMemberType_double,
#elif defined(DEE_TYPES_FLOATID_LDOUBLE) && DEE_CONFIG_RT_FLOATID_LDOUBLE == DEE_TYPES_FLOATID_LDOUBLE
 DeeMemberType_Dee_rt_ldouble = DeeMemberType_ldouble,
#endif

#include <deemon/__tg_members.inl>

 DeeMemberType_DeeAtomicMutex = DeeMemberType_ATOMIC|DeeMemberType_uint8_t,
#if DEE_PLATFORM_HAVE_IO_HANDLE
#if defined(DEE_PLATFORM_WINDOWS)
 DeeMemberType_DeeIOHandle = DeeMemberType_POINTER(DeeMemberType_void),
#elif defined(DEE_PLATFORM_UNIX)
 DeeMemberType_DeeIOHandle = DeeMemberType_int,
#endif
#endif

#ifdef DEE_PLATFORM_WINDOWS
 DeeMemberType_SOCKET          = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_DeeSocketHandle = DeeMemberType_POINTER(DeeMemberType_void),
#else
 DeeMemberType_SOCKET          = DeeMemberType_int,
 DeeMemberType_DeeSocketHandle = DeeMemberType_int,
#endif

#ifdef DEE_PLATFORM_WINDOWS
 // Patform-specific types
 DeeMemberType_PUCHAR = DeeMemberType_POINTER(DeeMemberType_uchar),
 DeeMemberType_PVOID = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_LPVOID = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_LPCVOID = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HANDLE = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HMODULE = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HINSTANCE = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HTASK = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HKEY = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HGLRC = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HDESK = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HMF = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HEMF = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HPEN = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HRSRC = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HSTR = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HWINSTA = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HKL = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HGDIOBJ = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HDWP = DeeMemberType_HANDLE,
 DeeMemberType_LPSTR = DeeMemberType_string,
 DeeMemberType_PCH = DeeMemberType_POINTER(DeeMemberType_char),
 DeeMemberType_PCHAR = DeeMemberType_POINTER(DeeMemberType_char),
 DeeMemberType_LPCH = DeeMemberType_POINTER(DeeMemberType_char),
 DeeMemberType_PSTR = DeeMemberType_string,
 DeeMemberType_LPWSTR = DeeMemberType_POINTER(DeeMemberType_wchar_t),
 DeeMemberType_PWCH = DeeMemberType_POINTER(DeeMemberType_wchar_t),
 DeeMemberType_PWCHAR = DeeMemberType_POINTER(DeeMemberType_wchar_t),
 DeeMemberType_LPWCH = DeeMemberType_POINTER(DeeMemberType_wchar_t),
 DeeMemberType_PWSTR = DeeMemberType_POINTER(DeeMemberType_wchar_t),
 DeeMemberType_HWND = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HMENU = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HACCEL = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HCURSOR = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HBRUSH = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HFONT = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HDC = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HICON = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HRGN = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HMONITOR = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HGLOBAL = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HLOCAL = DeeMemberType_HGLOBAL,
 DeeMemberType_HBITMAP = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HPALETTE = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HENHMETAFILE = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HMETAFILE = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_HMETAFILEPICT = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_FARPROC = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_NEARPROC = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_PROC = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_WNDPROC = DeeMemberType_POINTER(DeeMemberType_void),
 DeeMemberType_NPSTR = DeeMemberType_string,
 DeeMemberType_PZPSTR = DeeMemberType_POINTER(DeeMemberType_POINTER(DeeMemberType_char)),
 DeeMemberType_PCZPSTR = DeeMemberType_POINTER(DeeMemberType_POINTER(DeeMemberType_char)),
 DeeMemberType_PZPCSTR = DeeMemberType_POINTER(DeeMemberType_POINTER(DeeMemberType_char)),
 DeeMemberType_PCZPCSTR = DeeMemberType_POINTER(DeeMemberType_POINTER(DeeMemberType_char)),
 DeeMemberType_PZZSTR = DeeMemberType_string,
 DeeMemberType_PCZZSTR = DeeMemberType_string,
 DeeMemberType_PNZCH = DeeMemberType_POINTER(DeeMemberType_char), // zero-terminated
 DeeMemberType_PCNZCH = DeeMemberType_POINTER(DeeMemberType_char), // zero-terminated
 DeeMemberType_PZPWSTR = DeeMemberType_POINTER(DeeMemberType_POINTER(DeeMemberType_wchar_t)), // zero-terminated
 DeeMemberType_PCZPWSTR = DeeMemberType_POINTER(DeeMemberType_POINTER(DeeMemberType_wchar_t)), // zero-terminated
 DeeMemberType_LPUWSTR = DeeMemberType_POINTER(DeeMemberType_wchar_t), // zero-terminated
 DeeMemberType_PUWSTR = DeeMemberType_POINTER(DeeMemberType_wchar_t), // zero-terminated
 DeeMemberType_PZPCWSTR = DeeMemberType_POINTER(DeeMemberType_POINTER(DeeMemberType_wchar_t)), // zero-terminated
 DeeMemberType_PCZPCWSTR = DeeMemberType_POINTER(DeeMemberType_POINTER(DeeMemberType_wchar_t)), // zero-terminated
 DeeMemberType_LPCUWSTR = DeeMemberType_POINTER(DeeMemberType_wchar_t), // zero-terminated
 DeeMemberType_PCUWSTR = DeeMemberType_POINTER(DeeMemberType_wchar_t), // zero-terminated
 DeeMemberType_PZZWSTR = DeeMemberType_POINTER(DeeMemberType_wchar_t), // zero-terminated
 DeeMemberType_PCZZWSTR = DeeMemberType_POINTER(DeeMemberType_wchar_t), // zero-terminated
 DeeMemberType_PUZZWSTR = DeeMemberType_POINTER(DeeMemberType_wchar_t), // zero-terminated
 DeeMemberType_PCUZZWSTR = DeeMemberType_POINTER(DeeMemberType_wchar_t), // zero-terminated
 DeeMemberType_PNZWCH = DeeMemberType_POINTER(DeeMemberType_wchar_t),
 DeeMemberType_PCNZWCH = DeeMemberType_POINTER(DeeMemberType_wchar_t),
 DeeMemberType_PUNZWCH = DeeMemberType_POINTER(DeeMemberType_wchar_t),
 DeeMemberType_PCUNZWCH = DeeMemberType_POINTER(DeeMemberType_wchar_t),
 DeeMemberType_LARGE_INTEGER = DeeMemberType_POINTER(DeeMemberType_int64_t),
 DeeMemberType_ULARGE_INTEGER = DeeMemberType_POINTER(DeeMemberType_uint64_t),
 DeeMemberType_SYSTEMTIME = DeeMemberType__SYSTEMTIME,
 DeeMemberType_FILETIME = DeeMemberType__FILETIME,
 DeeMemberType_local_FILETIME = DeeMemberType_local__FILETIME,
 DeeMemberType_PSYSTEMTIME = DeeMemberType_POINTER(DeeMemberType_SYSTEMTIME),
 DeeMemberType_LPSYSTEMTIME = DeeMemberType_POINTER(DeeMemberType_SYSTEMTIME),
 DeeMemberType_PFILETIME = DeeMemberType_POINTER(DeeMemberType_FILETIME),
 DeeMemberType_LPFILETIME = DeeMemberType_POINTER(DeeMemberType_FILETIME),
 DeeMemberType_local_PFILETIME = DeeMemberType_POINTER(DeeMemberType_local_FILETIME),
 DeeMemberType_local_LPFILETIME = DeeMemberType_POINTER(DeeMemberType_local_FILETIME),
#endif
};
#define DeeMemberType_atomic(x) DeeMemberType_ATOMIC|(DeeMemberType_##x)
#define DeeMemberType_p(x)      DeeMemberType_POINTER_X1+(DeeMemberType_##x)
#define DeeMemberType_p1(x)     DeeMemberType_POINTER_X1+(DeeMemberType_##x)
#define DeeMemberType_p2(x)     DeeMemberType_POINTER_X2+(DeeMemberType_##x)
#define DeeMemberType_p3(x)     DeeMemberType_POINTER_X3+(DeeMemberType_##x)
#define DeeMemberType_p4(x)     DeeMemberType_POINTER_X4+(DeeMemberType_##x)
#define DeeMemberType_p5(x)     DeeMemberType_POINTER_X5+(DeeMemberType_##x)
#define DeeMemberType_p6(x)     DeeMemberType_POINTER_X6+(DeeMemberType_##x)
#define DeeMemberType_p7(x)     DeeMemberType_POINTER_X7+(DeeMemberType_##x)
#define DeeMemberType_p8(x)     DeeMemberType_POINTER_X8+(DeeMemberType_##x)
#define DeeMemberType_p9(x)     DeeMemberType_POINTER_X9+(DeeMemberType_##x)
#define DEE_PRIVATE_MEMBERTYPE(T) DeeMemberType_##T

#define DeeMemberAccess_MAKE(r,w) \
(((r)?DeeMemberAccess_READ:0)|((w)?DeeMemberAccess_WRITE:0))\

enum{
 DeeMemberAccess_READ  = 0x1,
 DeeMemberAccess_WRITE = 0x2,
 DeeMemberAccess_BOTH  = DeeMemberAccess_READ|DeeMemberAccess_WRITE,
};

#define DEE_DOC_AUTO            "*"
#define DEE_DOC_AUTO_OF(name)   "*:" name
#define DEE_DOC_AUTO_ARGS(args) "*!" args

//////////////////////////////////////////////////////////////////////////
// Public v100 interface for member defs
#define DEE_MEMBERDEF_v100(s,m,T)                            {  #m,NULL,DEE_PRIVATE_MEMBERTYPE(T),DeeMemberAccess_BOTH,Dee_OFFSETOF(s,m)}
#define DEE_MEMBERDEF_CONST_v100(name,T,v)                   {name,NULL,DeeMemberType_CONST|DEE_PRIVATE_MEMBERTYPE(T),DeeMemberAccess_READ,(Dee_size_t)(v)}
#define DEE_MEMBERDEF_CONST_DOC_v100(name,T,v,doc)           {name,Dee_DOCSTRING(doc),DeeMemberType_CONST|DEE_PRIVATE_MEMBERTYPE(T),DeeMemberAccess_READ,(Dee_size_t)(v)}
#define DEE_MEMBERDEF_NAMED_v100(name,s,m,T)                 {name,NULL,DEE_PRIVATE_MEMBERTYPE(T),DeeMemberAccess_BOTH,Dee_OFFSETOF(s,m)}
#define DEE_MEMBERDEF_RO_v100(s,m,T)                         {  #m,NULL,DEE_PRIVATE_MEMBERTYPE(T),DeeMemberAccess_READ,Dee_OFFSETOF(s,m)}
#define DEE_MEMBERDEF_NAMED_RO_v100(name,s,m,T)              {name,NULL,DEE_PRIVATE_MEMBERTYPE(T),DeeMemberAccess_READ,Dee_OFFSETOF(s,m)}
#define DEE_MEMBERDEF_EX_v100(s,m,T,r,w)                     {  #m,NULL,DEE_PRIVATE_MEMBERTYPE(T),DeeMemberAccess_MAKE(r,w),Dee_OFFSETOF(s,m)}
#define DEE_MEMBERDEF_NAMED_EX_v100(name,s,m,T,r,w)          {name,NULL,DEE_PRIVATE_MEMBERTYPE(T),DeeMemberAccess_MAKE(r,w),Dee_OFFSETOF(s,m)}
#define DEE_MEMBERDEF_DOC_v100(s,m,T,doc)                    {  #m,Dee_DOCSTRING(doc),DEE_PRIVATE_MEMBERTYPE(T),DeeMemberAccess_BOTH,Dee_OFFSETOF(s,m)}
#define DEE_MEMBERDEF_NAMED_DOC_v100(name,s,m,T,doc)         {name,Dee_DOCSTRING(doc),DEE_PRIVATE_MEMBERTYPE(T),DeeMemberAccess_BOTH,Dee_OFFSETOF(s,m)}
#define DEE_MEMBERDEF_NAMED_DOC_RO_v100(name,s,m,T,doc)      {name,Dee_DOCSTRING(doc),DEE_PRIVATE_MEMBERTYPE(T),DeeMemberAccess_READ,Dee_OFFSETOF(s,m)}
#define DEE_MEMBERDEF_EX_DOC_v100(s,m,T,r,w,doc)             {  #m,Dee_DOCSTRING(doc),DEE_PRIVATE_MEMBERTYPE(T),DeeMemberAccess_MAKE(r,w),Dee_OFFSETOF(s,m)}
#define DEE_MEMBERDEF_NAMED_EX_DOC_v100(name,s,m,T,r,w,doc)  {name,Dee_DOCSTRING(doc),DEE_PRIVATE_MEMBERTYPE(T),DeeMemberAccess_MAKE(r,w),Dee_OFFSETOF(s,m)}
#define DEE_MEMBERDEF_FULL_v100(name,doc,type,access,offset) {name,Dee_DOCSTRING(doc),type,access,offset}
#define DEE_MEMBERDEF_END_v100                               {NULL,NULL,0,0,0}

//////////////////////////////////////////////////////////////////////////
// Public v100 interface for getset defs
#ifdef DEE_LIMITED_DEX
#define DEE_GETSETDEF_v100(name,get,del,set,doc)                              {name,Dee_DOCSTRING(doc),NULL,DEE_GETSETDEF_FLAG_NONE,DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_Getter,DEE_TYPEMEMBER(get,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_Delter,DEE_TYPEMEMBER(del,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_Setter,DEE_TYPEMEMBER(set,NULL))}
#define DEE_GETSETDEF_CONST_v100(name,get,del,set,doc)                        {name,Dee_DOCSTRING(doc),NULL,DEE_GETSETDEF_FLAG_CONST,DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_Getter,DEE_TYPEMEMBER(get,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_Delter,DEE_TYPEMEMBER(del,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_Setter,DEE_TYPEMEMBER(set,NULL))}
#define DEE_GETSETDEF_CLOSURE_v100(name,get,del,set,closure,doc)              {name,Dee_DOCSTRING(doc),closure,DEE_GETSETDEF_FLAG_NONE,DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_Getter,DEE_TYPEMEMBER(get,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_Delter,DEE_TYPEMEMBER(del,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_Setter,DEE_TYPEMEMBER(set,NULL))}
#define DEE_GETSETDEF_CONST_CLOSURE_v100(name,get,del,set,closure,doc)        {name,Dee_DOCSTRING(doc),closure,DEE_GETSETDEF_FLAG_CONST,DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_Getter,DEE_TYPEMEMBER(get,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_Delter,DEE_TYPEMEMBER(del,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_Setter,DEE_TYPEMEMBER(set,NULL))}
#define DEE_GETSETDEF_FULL_v100(name,doc,closure,flags,get,del,set)           {name,Dee_DOCSTRING(doc),closure,flags,DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_Getter,DEE_TYPEMEMBER(get,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_Delter,DEE_TYPEMEMBER(del,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_Setter,DEE_TYPEMEMBER(set,NULL))}
#define DEE_GETSETDEF_STRUCT_v100(name,get,del,set,doc)                       {name,Dee_DOCSTRING(doc),NULL,DEE_GETSETDEF_FLAG_NONE,(_DeeGetSetDef_Getter)DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_StructuredGetter,DEE_TYPEMEMBER(get,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_StructuredDelter,DEE_TYPEMEMBER(del,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_StructuredSetter,DEE_TYPEMEMBER(set,NULL))}
#define DEE_GETSETDEF_CONST_STRUCT_v100(name,get,del,set,doc)                 {name,Dee_DOCSTRING(doc),NULL,DEE_GETSETDEF_FLAG_CONST,(_DeeGetSetDef_Getter)DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_StructuredGetter,DEE_TYPEMEMBER(get,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_StructuredDelter,DEE_TYPEMEMBER(del,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_StructuredSetter,DEE_TYPEMEMBER(set,NULL))}
#define DEE_GETSETDEF_STRUCT_CLOSURE_v100(name,get,del,set,closure,doc)       {name,Dee_DOCSTRING(doc),closure,DEE_GETSETDEF_FLAG_NONE,(_DeeGetSetDef_Getter)DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_StructuredGetter,DEE_TYPEMEMBER(get,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_StructuredDelter,DEE_TYPEMEMBER(del,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_StructuredSetter,DEE_TYPEMEMBER(set,NULL))}
#define DEE_GETSETDEF_CONST_STRUCT_CLOSURE_v100(name,get,del,set,closure,doc) {name,Dee_DOCSTRING(doc),closure,DEE_GETSETDEF_FLAG_CONST,(_DeeGetSetDef_Getter)DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_StructuredGetter,DEE_TYPEMEMBER(get,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_StructuredDelter,DEE_TYPEMEMBER(del,NULL)),DEE_REQUIRE_TYPECOMPATIBLE(_DeeGetSetDef_StructuredSetter,DEE_TYPEMEMBER(set,NULL))}
#else
#define DEE_GETSETDEF_v100(name,get,del,set,doc)                              {name,Dee_DOCSTRING(doc),NULL,DEE_GETSETDEF_FLAG_NONE,(_DeeGetSetDef_Getter)DEE_TYPEMEMBER(get,NULL),(_DeeGetSetDef_Delter)DEE_TYPEMEMBER(del,NULL),(_DeeGetSetDef_Setter)DEE_TYPEMEMBER(set,NULL)}
#define DEE_GETSETDEF_CONST_v100(name,get,del,set,doc)                        {name,Dee_DOCSTRING(doc),NULL,DEE_GETSETDEF_FLAG_CONST,(_DeeGetSetDef_Getter)DEE_TYPEMEMBER(get,NULL),(_DeeGetSetDef_Delter)DEE_TYPEMEMBER(del,NULL),(_DeeGetSetDef_Setter)DEE_TYPEMEMBER(set,NULL)}
#define DEE_GETSETDEF_CLOSURE_v100(name,get,del,set,closure,doc)              {name,Dee_DOCSTRING(doc),closure,DEE_GETSETDEF_FLAG_NONE,(_DeeGetSetDef_Getter)DEE_TYPEMEMBER(get,NULL),(_DeeGetSetDef_Delter)DEE_TYPEMEMBER(del,NULL),(_DeeGetSetDef_Setter)DEE_TYPEMEMBER(set,NULL)}
#define DEE_GETSETDEF_CONST_CLOSURE_v100(name,get,del,set,closure,doc)        {name,Dee_DOCSTRING(doc),closure,DEE_GETSETDEF_FLAG_CONST,(_DeeGetSetDef_Getter)DEE_TYPEMEMBER(get,NULL),(_DeeGetSetDef_Delter)DEE_TYPEMEMBER(del,NULL),(_DeeGetSetDef_Setter)DEE_TYPEMEMBER(set,NULL)}
#define DEE_GETSETDEF_FULL_v100(name,doc,closure,flags,get,del,set)           {name,Dee_DOCSTRING(doc),closure,flags,(_DeeGetSetDef_Getter)DEE_TYPEMEMBER(get,NULL),(_DeeGetSetDef_Delter)DEE_TYPEMEMBER(del,NULL),(_DeeGetSetDef_Setter)DEE_TYPEMEMBER(set,NULL)}
#define DEE_GETSETDEF_STRUCT_v100(name,get,del,set,doc)                       {name,Dee_DOCSTRING(doc),NULL,DEE_GETSETDEF_FLAG_NONE,(_DeeGetSetDef_Getter)DEE_TYPEMEMBER(get,NULL),(_DeeGetSetDef_Delter)DEE_TYPEMEMBER(del,NULL),(_DeeGetSetDef_Setter)DEE_TYPEMEMBER(set,NULL)}
#define DEE_GETSETDEF_CONST_STRUCT_v100(name,get,del,set,doc)                 {name,Dee_DOCSTRING(doc),NULL,DEE_GETSETDEF_FLAG_CONST,(_DeeGetSetDef_Getter)DEE_TYPEMEMBER(get,NULL),(_DeeGetSetDef_Delter)DEE_TYPEMEMBER(del,NULL),(_DeeGetSetDef_Setter)DEE_TYPEMEMBER(set,NULL)}
#define DEE_GETSETDEF_STRUCT_CLOSURE_v100(name,get,del,set,closure,doc)       {name,Dee_DOCSTRING(doc),closure,DEE_GETSETDEF_FLAG_NONE,(_DeeGetSetDef_Getter)DEE_TYPEMEMBER(get,NULL),(_DeeGetSetDef_Delter)DEE_TYPEMEMBER(del,NULL),(_DeeGetSetDef_Setter)DEE_TYPEMEMBER(set,NULL)}
#define DEE_GETSETDEF_CONST_STRUCT_CLOSURE_v100(name,get,del,set,closure,doc) {name,Dee_DOCSTRING(doc),closure,DEE_GETSETDEF_FLAG_CONST,(_DeeGetSetDef_Getter)DEE_TYPEMEMBER(get,NULL),(_DeeGetSetDef_Delter)DEE_TYPEMEMBER(del,NULL),(_DeeGetSetDef_Setter)DEE_TYPEMEMBER(set,NULL)}
#endif
#define DEE_GETSETDEF_END_v100                                         {NULL,NULL,NULL,DEE_GETSETDEF_FLAG_NONE,NULL,NULL,NULL}

//////////////////////////////////////////////////////////////////////////
// Public v100 interface for method defs
#define DEE_METHODDEF_v100(name,func,doc)                              {name,Dee_DOCSTRING(doc),NULL,DEE_METHODDEF_FLAG_NONE,DEE_REQUIRE_TYPECOMPATIBLE(DeeMemberFunction,DEE_TYPEMEMBER(func,NULL))}
#define DEE_METHODDEF_CONST_v100(name,func,doc)                        {name,Dee_DOCSTRING(doc),NULL,DEE_METHODDEF_FLAG_CONST,DEE_REQUIRE_TYPECOMPATIBLE(DeeMemberFunction,DEE_TYPEMEMBER(func,NULL))}
#define DEE_METHODDEF_CLOSURE_v100(name,func,closure,doc)              {name,Dee_DOCSTRING(doc),closure,DEE_METHODDEF_FLAG_NONE,DEE_REQUIRE_TYPECOMPATIBLE(DeeMemberFunction,DEE_TYPEMEMBER(func,NULL))}
#define DEE_METHODDEF_CONST_CLOSURE_v100(name,func,closure,doc)        {name,Dee_DOCSTRING(doc),closure,DEE_METHODDEF_FLAG_CONST,DEE_REQUIRE_TYPECOMPATIBLE(DeeMemberFunction,DEE_TYPEMEMBER(func,NULL))}
#define DEE_METHODDEF_FULL_v100(name,doc,closure,flags,func)           {name,Dee_DOCSTRING(doc),closure,flags,func}
#ifdef DEE_LIMITED_DEX
#define DEE_METHODDEF_STRUCT_v100(name,func,doc)                       {name,Dee_DOCSTRING(doc),NULL,DEE_METHODDEF_FLAG_STRUCT,(DeeMemberFunction)DEE_REQUIRE_TYPECOMPATIBLE(DeeStructuredMemberFunction,DEE_TYPEMEMBER(func,NULL))}
#define DEE_METHODDEF_CONST_STRUCT_v100(name,func,doc)                 {name,Dee_DOCSTRING(doc),NULL,DEE_METHODDEF_FLAG_CONST|DEE_METHODDEF_FLAG_STRUCT,(DeeMemberFunction)DEE_REQUIRE_TYPECOMPATIBLE(DeeStructuredMemberFunction,DEE_TYPEMEMBER(func,NULL))}
#define DEE_METHODDEF_STRUCT_CLOSURE_v100(name,func,closure,doc)       {name,Dee_DOCSTRING(doc),closure,DEE_METHODDEF_FLAG_STRUCT,(DeeMemberFunction)DEE_REQUIRE_TYPECOMPATIBLE(DeeStructuredMemberFunction,DEE_TYPEMEMBER(func,NULL))}
#define DEE_METHODDEF_CONST_STRUCT_CLOSURE_v100(name,func,closure,doc) {name,Dee_DOCSTRING(doc),closure,DEE_METHODDEF_FLAG_CONST|DEE_METHODDEF_FLAG_STRUCT,(DeeMemberFunction)DEE_REQUIRE_TYPECOMPATIBLE(DeeStructuredMemberFunction,DEE_TYPEMEMBER(func,NULL))}
#else
#define DEE_METHODDEF_STRUCT_v100(name,func,doc)                       {name,Dee_DOCSTRING(doc),NULL,DEE_METHODDEF_FLAG_STRUCT,(DeeMemberFunction)DEE_TYPEMEMBER(func,NULL)}
#define DEE_METHODDEF_CONST_STRUCT_v100(name,func,doc)                 {name,Dee_DOCSTRING(doc),NULL,DEE_METHODDEF_FLAG_CONST|DEE_METHODDEF_FLAG_STRUCT,(DeeMemberFunction)DEE_TYPEMEMBER(func,NULL)}
#define DEE_METHODDEF_STRUCT_CLOSURE_v100(name,func,closure,doc)       {name,Dee_DOCSTRING(doc),closure,DEE_METHODDEF_FLAG_STRUCT,(DeeMemberFunction)DEE_TYPEMEMBER(func,NULL)}
#define DEE_METHODDEF_CONST_STRUCT_CLOSURE_v100(name,func,closure,doc) {name,Dee_DOCSTRING(doc),closure,DEE_METHODDEF_FLAG_CONST|DEE_METHODDEF_FLAG_STRUCT,(DeeMemberFunction)DEE_TYPEMEMBER(func,NULL)}
#endif
#define DEE_METHODDEF_END_v100                                         {NULL,NULL,NULL,DEE_METHODDEF_FLAG_NONE,NULL}


DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) DeeTypeObject *)
 DeeMember_Type(DEE_A_IN struct DeeMemberDef const *def) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeMember_Get(
 DEE_A_IN DeeObject const *base_ob, DEE_A_IN struct DeeMemberDef const *def) DEE_ATTRIBUTE_NONNULL((1,2));
#define DeeMember_Set(base_ob,def,v) DeeMember_TSet(base_ob,def,Dee_TYPE(v),v)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeMember_TSet(
 DEE_A_IN DeeObject *base_ob, DEE_A_IN struct DeeMemberDef const *def,
 DEE_A_IN DeeTypeObject const *tp_v, DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3,4));


DEE_DECL_END

#endif /* !GUARD_DEEMON_STRUCTMEMBER_H */
