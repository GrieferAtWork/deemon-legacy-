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
#ifndef GUARD_DEEMON_DEX_H
#define GUARD_DEEMON_DEX_H 1

#include <deemon/__conf.inl>
#include <deemon/__ctypes.inl>
#include <deemon/optional/integer_decl.h>
#include <deemon/optional/string_forward.h>
#include <deemon/optional/cfunction_decl.h>
#include <deemon/optional/std/stdarg.h>

#ifdef DEE_LIMITED_DEX
#include <deemon/__typecompatible.inl>
#endif

//////////////////////////////////////////////////////////////////////////
// Deemon extensions API
//////////////////////////////////////////////////////////////////////////
// Namespacing:
//  - The name of a deemon extension (short 'dex') module only
//    depends on the '.dll'/'.so' filename in the '/lib/dex' folder.
//    Note though, that names starting with an underscore '_'
//    are reserved for internal use (aka. for me ^w^)
//////////////////////////////////////////////////////////////////////////

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_CFUNCTION
DEE_PRIVATE_DECL_DEE_CFUNCTION
#undef DEE_PRIVATE_DECL_DEE_CFUNCTION
#endif

DEE_OBJECT_DEF(DeeDexModuleObject);



typedef DEE_A_RET_EXCEPT_REF DeeObject *(DEE_CALL *DeeDexConstructor)(void);
// 'ob' is the value returned by 'DeeDexConstructor'
typedef void (DEE_CALL *DeeDexDestructor)(DEE_A_IN DeeObject *ob);

typedef DEE_A_RET_EXCEPT_REF DeeObject *(DEE_CALL *DeeDexGetter)(void);
typedef DEE_A_RET_EXCEPT(-1) int (DEE_CALL *DeeDexDelete)(void);
typedef DEE_A_RET_EXCEPT(-1) int (DEE_CALL *DeeDexSetter)(DEE_A_IN DeeObject *ob);

DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeDexExportDef {
 char const         *dxe_name;   /*< [0..1] Name of the export. (NULL for end of export list) */
#define DEE_DEX_EXPORT_TYPE_OBJECT          DEE_UINT32_C(0x00000000)
#define DEE_DEX_EXPORT_TYPE_GENERATOR       DEE_UINT32_C(0x00000001)
#define DEE_DEX_EXPORT_TYPE_GETSET          DEE_UINT32_C(0x00000002)
#define DEE_DEX_EXPORT_TYPE_OBJECTINL       DEE_UINT32_C(0x00000003)
#define DEE_DEX_EXPORT_TYPE_FUNCTION        DEE_UINT32_C(0x00000003)
#define DEE_DEX_EXPORT_TYPE_NATIVE_FUNCTION DEE_UINT32_C(0x00000004) /*< Native function. */
#define DEE_DEX_EXPORT_TYPE_INT8            DEE_UINT32_C(0x00000005)
#define DEE_DEX_EXPORT_TYPE_INT16           DEE_UINT32_C(0x00000006)
#define DEE_DEX_EXPORT_TYPE_INT32           DEE_UINT32_C(0x00000007)
#define DEE_DEX_EXPORT_TYPE_INT64           DEE_UINT32_C(0x00000008)
#define DEE_DEX_EXPORT_TYPE_UINT8           DEE_UINT32_C(0x00000009)
#define DEE_DEX_EXPORT_TYPE_UINT16          DEE_UINT32_C(0x0000000A)
#define DEE_DEX_EXPORT_TYPE_UINT32          DEE_UINT32_C(0x0000000B)
#define DEE_DEX_EXPORT_TYPE_UINT64          DEE_UINT32_C(0x0000000C)
#define DEE_DEX_EXPORT_TYPE_MASK            DEE_UINT32_C(0x000000FF)
 Dee_uint32_t        dxe_kind;   /*< Export type/flags. */
 union{
  void                     *dxe_data[8];
  struct DeeCFunctionObject dxe_cfunction; /*< Inline cfunction object. */
  DeeObject                 dxe_objectinl; /*< Inline object. */
  DeeObject                *dxe_object;    /*< [1..1] Object being exports. */
  struct {
   union{
    Dee_int8_t  dxe_c_i8;  Dee_uint8_t  dxe_c_ui8;
    Dee_int16_t dxe_c_i16; Dee_uint16_t dxe_c_ui16;
    Dee_int32_t dxe_c_i32; Dee_uint32_t dxe_c_ui32;
    Dee_int64_t dxe_c_i64; Dee_uint64_t dxe_c_ui64;
   }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define dxe_c_i8   _dxe_const_data.dxe_c_i8
#define dxe_c_i16  _dxe_const_data.dxe_c_i16
#define dxe_c_i32  _dxe_const_data.dxe_c_i32
#define dxe_c_i64  _dxe_const_data.dxe_c_i64
#define dxe_c_ui8  _dxe_const_data.dxe_c_ui8
#define dxe_c_ui16 _dxe_const_data.dxe_c_ui16
#define dxe_c_ui32 _dxe_const_data.dxe_c_ui32
#define dxe_c_ui64 _dxe_const_data.dxe_c_ui64
   _dxe_const_data
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
   ;
   char const *dxe_c_doc; /*< [0..1] Documentation string. */
  } dxe_const;
  struct {
   char const *dxe_nf_type;     /*< [1..1] Structf-style function type descriptor (NOTE: Not the function pointer type, but the foreign_function_type). */
   void       *dxe_nf_function; /*< [1..1] Function pointer. */
  } dxe_native_function;
  struct {
   DeeObject        *dxe_gn_cache; /*< [0..1] Cache generator value (read-only; managed by the dex). */
   DeeDexConstructor dxe_gn_ctor;  /*< [1..1] Constructor callback. */
   DeeDexDestructor  dxe_gn_dtor;  /*< [0..1] Destructor callback. (If NULL, Dee_DECREF() the object) */
  } dxe_generator;
  struct {
   DeeDexGetter dxe_gs_get; /*< [0..1] Getter callback. */
   DeeDexDelete dxe_gs_del; /*< [0..1] Delete callback. */
   DeeDexSetter dxe_gs_set; /*< [0..1] Setter callback. */
  } dxe_getset;
 };
};
DEE_COMPILER_MSVC_WARNING_POP

#ifdef DEE_REQUIRE_TYPECOMPATIBLE
#define DEE_DEX_TYPECOMPATIBLE       DEE_REQUIRE_TYPECOMPATIBLE
#else
#define DEE_DEX_TYPECOMPATIBLE(T,ob) (T)(ob)
#endif


#if DEE_TYPES_SIZEOF_REFCNT == DEE_TYPES_SIZEOF_POINTER\
 && DEE_TYPES_SIZEOF_WEAKCNT == DEE_TYPES_SIZEOF_POINTER
#define DEE_DEX_PRIVATE_EXPORT_OBJECTINL_PREFIX(refcnt,weakcnt)\
 (void *)(uintptr_t)(refcnt),(void *)(uintptr_t)(weakcnt)
#elif DEE_TYPES_SIZEOF_REFCNT+DEE_TYPES_SIZEOF_WEAKCNT == DEE_TYPES_SIZEOF_POINTER
#if DEE_PLATFORM_ENDIAN == 1234
#define DEE_DEX_PRIVATE_EXPORT_OBJECTINL_PREFIX(refcnt,weakcnt)\
 (void *)((Dee_uintptr_t)(weakcnt)|((uintptr_t)(refcnt)<<(DEE_TYPES_SIZEOF_WEAKCNT*8)))
#else
#define DEE_DEX_PRIVATE_EXPORT_OBJECTINL_PREFIX(refcnt,weakcnt)\
 (void *)((Dee_uintptr_t)(refcnt)|((uintptr_t)(weakcnt)<<(DEE_TYPES_SIZEOF_REFCNT*8)))
#endif
#else
// ((DEE_TYPES_SIZEOF_REFCNT+DEE_TYPES_SIZEOF_WEAKCNT)%DEE_TYPES_SIZEOF_POINTER) != 0
#error "Cannot properly align inline-object reference counters"
#endif



#define DEE_DEX_PRIVATE_EXPORT_OBJECTINL(name,type,...)\
 {name,DEE_DEX_EXPORT_TYPE_OBJECTINL,{\
 {DEE_DEX_PRIVATE_EXPORT_OBJECTINL_PREFIX(1,1),type,__VA_ARGS__}}}

#if defined(DEE_DEX_PRIVATE_EXPORT_OBJECTINL) && !defined(__GNUC__)
#define DeeDex_EXPORT_FUNCTION(name,func,doc) \
 DEE_DEX_PRIVATE_EXPORT_OBJECTINL(name,&DeeCFunction_Type,name,doc,\
 DEE_DEX_TYPECOMPATIBLE(DeeCFunction,func))
#else
#define DeeDex_EXPORT_FUNCTION(name,func,doc)\
 {name,DEE_DEX_EXPORT_TYPE_FUNCTION,{.dxe_cfunction=\
 {1,1,&DeeCFunction_Type,name,doc,\
  DEE_DEX_TYPECOMPATIBLE(DeeCFunction,func)}}}
#endif


#define DeeDex_EXPORT_OBJECT(name,object) \
 {name,DEE_DEX_EXPORT_TYPE_OBJECT,{{object}}}
#define DeeDex_EXPORT_GENERATOR(name,ctor,dtor) \
 {name,DEE_DEX_EXPORT_TYPE_GENERATOR,{{NULL,\
  DEE_DEX_TYPECOMPATIBLE(DeeDexConstructor,ctor),\
  DEE_DEX_TYPECOMPATIBLE(DeeDexDestructor,dtor)}}}
#define DeeDex_EXPORT_GETSET(name,get,del,set) \
 {name,DEE_DEX_EXPORT_TYPE_GETSET,{{\
  DEE_DEX_TYPECOMPATIBLE(DeeDexGetter,get),\
  DEE_DEX_TYPECOMPATIBLE(DeeDexDelete,del),\
  DEE_DEX_TYPECOMPATIBLE(DeeDexSetter,set)}}}
#define DeeDex_EXPORT_END {NULL,0,{{0}}}
#define DeeDex_EXPORT_NATIVE_FUNCTION(name,structF,fun) \
 {name,DEE_DEX_EXPORT_TYPE_NATIVE_FUNCTION,{{structF,fun}}}



#if DEE_PLATFORM_ENDIAN == 1234
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S1_1(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_INT8,{{(void *)(Dee_uintptr_t)(v),doc,NULL,}}}
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S1_2(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_INT16,{{(void *)(Dee_uintptr_t)(v),doc,NULL,}}}
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S1_4(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_INT32,{{(void *)(Dee_uintptr_t)(v),doc,NULL,}}}
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S0_1(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_UINT8,{{(void *)(Dee_uintptr_t)(v),doc,NULL,}}}
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S0_2(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_UINT16,{{(void *)(Dee_uintptr_t)(v),doc,NULL,}}}
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S0_4(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_UINT32,{{(void *)(Dee_uintptr_t)(v),doc,NULL,}}}
#if DEE_TYPES_SIZEOF_POINTER == 8
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S1_8(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_INT64,{{(void *)(Dee_uintptr_t)(v),doc,NULL,}}}
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S0_8(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_UINT64,{{(void *)(Dee_uintptr_t)(v),doc,NULL,}}}
#else
// Do a little bit of hacking, to place all the bytes at their correct addresses
// ... jeez. And all just because VC doesn't support C99's struct/union initializer syntax ...
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S1_8(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_INT64,{{\
 (void *)((Dee_uintptr_t)(((Dee_uint64_t)(v)&DEE_UINT64_C(0xFFFFFFFF00000000))>>32)),\
 (void *)((Dee_uintptr_t)((Dee_uint64_t)(v)&DEE_UINT64_C(0x00000000FFFFFFFF))),doc,NULL,}}}
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S0_8(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_UINT64,{{\
 (void *)((Dee_uintptr_t)(((Dee_uint64_t)(v)&DEE_UINT64_C(0xFFFFFFFF00000000))>>32)),\
 (void *)((Dee_uintptr_t)((Dee_uint64_t)(v)&DEE_UINT64_C(0x00000000FFFFFFFF))),doc,NULL,}}}
#endif
#else
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S1_1(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_INT8,{{(void *)(((Dee_uintptr_t)(v))<<((DEE_TYPES_SIZEOF_POINTER-1)*8)),doc,NULL,}}}
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S1_2(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_INT16,{{(void *)(((Dee_uintptr_t)(v))<<((DEE_TYPES_SIZEOF_POINTER-2)*8)),doc,NULL,}}}
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S1_4(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_INT32,{{(void *)(((Dee_uintptr_t)(v))<<((DEE_TYPES_SIZEOF_POINTER-4)*8)),doc,NULL,}}}
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S0_1(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_UINT8,{{(void *)(((Dee_uintptr_t)(v))<<((DEE_TYPES_SIZEOF_POINTER-1)*8)),doc,NULL,}}}
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S0_2(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_UINT16,{{(void *)(((Dee_uintptr_t)(v))<<((DEE_TYPES_SIZEOF_POINTER-2)*8)),doc,NULL,}}}
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S0_4(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_UINT32,{{(void *)(((Dee_uintptr_t)(v))<<((DEE_TYPES_SIZEOF_POINTER-4)*8)),doc,NULL,}}}
#if DEE_TYPES_SIZEOF_POINTER == 8
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S1_8(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_INT64,{{(void *)((Dee_uintptr_t)(v)),doc,NULL,}}}
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S0_8(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_UINT64,{{(void *)((Dee_uintptr_t)(v)),doc,NULL,}}}
#else
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S1_8(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_INT64,{{\
 (void *)((Dee_uintptr_t)((Dee_uint64_t)(v)&DEE_UINT64_C(0x00000000FFFFFFFF))),\
 (void *)((Dee_uintptr_t)(((Dee_uint64_t)(v)&DEE_UINT64_C(0xFFFFFFFF00000000))>>32)),doc,NULL,}}}
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S0_8(name,v,doc) {name,DEE_DEX_EXPORT_TYPE_UINT64,{{\
 (void *)((Dee_uintptr_t)((Dee_uint64_t)(v)&DEE_UINT64_C(0x00000000FFFFFFFF))),\
 (void *)((Dee_uintptr_t)(((Dee_uint64_t)(v)&DEE_UINT64_C(0xFFFFFFFF00000000))>>32)),doc,NULL,}}}
#endif
#endif
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S0(N) DEE_PRIVATE_DEX_EXPORT_CONST_EX_S0_##N
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S1(N) DEE_PRIVATE_DEX_EXPORT_CONST_EX_S1_##N
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX_S(signed) DEE_PRIVATE_DEX_EXPORT_CONST_EX_S##signed
#define DEE_PRIVATE_DEX_EXPORT_CONST_EX(signed,N) DEE_PRIVATE_DEX_EXPORT_CONST_EX_S(signed)(N)
#define DEE_PRIVATE_DEX_EXPORT_CONST_F(T) DEE_PRIVATE_DEX_EXPORT_CONST_EX(DEE_TYPEDEF_SIGNED(T),DEE_TYPEDEF_SIZEOF(T))

#define DeeDex_EXPORT_CONST_F(T) DEE_PRIVATE_DEX_EXPORT_CONST_F(T)
#define DeeDex_EXPORT_CONST_DOC(name,T,value,doc) DeeDex_EXPORT_CONST_F(T)(name,value,doc)
#define DeeDex_EXPORT_CONST(name,T,value)         DeeDex_EXPORT_CONST_F(T)(name,value,NULL)


#ifdef DEE_LIMITED_API
extern DEE_A_RET_EXCEPT(-1) int _DeeDex_InitApi(void);
extern void _DeeDex_ShutdownApi(void);
extern void _DeeDex_QuitApi(void);
#endif


DEE_DATA_DECL(DeeTypeObject) DeeDexModule_Type;
#define DeeDexModule_Check(ob)      DeeObject_InstanceOf(ob,&DeeDexModule_Type)
#define DeeDexModule_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeDexModule_Type)

struct DeeListObject;
//////////////////////////////////////////////////////////////////////////
// Get/Set the search path for extension modules.
// Modifications to this list are mirrored immediately and system-wide.
//  -> With lists already doing their own synchronization, you can simply
//     use functions like DeeList_Append to add a new search path.
//  -> Assign the return value of 'DeeDex_GetDefaultSearchPath' to restore the default.
#define DeeDex_SearchPath  ((DeeObject *)&_DeeDex_SearchPath)
DEE_DATA_DECL(struct DeeListObject) _DeeDex_SearchPath;

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *) DeeDex_GetDefaultSearchPath(void);

#define DEEDEX_SEARCHORDER_VERPATH "V" /*< Search the version-dependent dex path ("/usr/lib/deemon/dex.%d" % DEE_VERSION_API). Ignored on windows. */
#define DEEDEX_SEARCHORDER_DEXPATH "D" /*< Search the dex search path (DeeDex_SearchPath). */
#define DEEDEX_SEARCHORDER_CWDPATH "C" /*< Search the current working directory (fs::getcwd()). */
#define DEEDEX_SEARCHORDER_EXEPATH "X" /*< Search the hosting exe's directory (fs::path::head(fs::readlink("/proc/self/exe"))). */
#define DEEDEX_SEARCHORDER_SYSPATH "P" /*< Search the system's $PATH for extension files (fs::getenv("PATH").split(fs::path::DELIM)). */

//////////////////////////////////////////////////////////////////////////
// Default dex search configuration
// >> Following window's style of using /proc/self/cwd and $PATH
//    when searching for dlls, deemon's dex does the same by default.
#ifdef DEE_PLATFORM_WINDOWS
#define DEEDEX_SEARCHORDER_DEFAULT \
 DEEDEX_SEARCHORDER_DEXPATH DEEDEX_SEARCHORDER_CWDPATH \
 DEEDEX_SEARCHORDER_EXEPATH DEEDEX_SEARCHORDER_SYSPATH
#else
#define DEEDEX_SEARCHORDER_DEFAULT \
 DEEDEX_SEARCHORDER_VERPATH DEEDEX_SEARCHORDER_DEXPATH
#endif

//////////////////////////////////////////////////////////////////////////
// Stores the current dex search order in 'order[0..s-1]' (including the terminating \0 character)
// Won't write more than 's' characters and always returns the of bytes
// that would have been required/sufficient to fully store the current order.
// - Returns an empty string if the dex isn't initialized.
DEE_FUNC_DECL(Dee_size_t) DeeDex_GetSearchMode(DEE_A_OUT_W(s) char *order, Dee_size_t s);

//////////////////////////////////////////////////////////////////////////
// Overwrites the current search order with the order specified
// - 'order' must be a string build from 'DEEDEX_SEARCHORDER_*',
//   containing any specific portion only once.
// - If an invalid order is encountered, or any given tag was
//   already used, an Error.ValueError is thrown and the call fails
//   without changing the previously configured search order.
// - If 'order' is NULL, the default search path is restored.
// - When the dex isn't initialized, this function is a no-op, returning
//   1, with the internal search order always being set to an empty string.
//  (Thus preventing new modules from being loaded when not initialized.)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDex_SetSearchOrder(DEE_A_IN_Z_OPT char const *order);



//////////////////////////////////////////////////////////////////////////
// Lookup a given dex module
// >> '__builtin_dex("_zlib")' -> 'DeeDex_Open("_zlib")'
// Throws an 'Error.AttributeError' if the dex wasn't found
// NOTE: On windows, dex names are converted to lowercase
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeDexModuleObject) *)
 DeeDex_Open(DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Lookup a dex import by name
// - 'DeeDex_ImportEx' does the same as 'DeeDex_Import':
// >> function DeeDex_ImportEx(module_name,import_name)
// >>   -> DeeDex_Import(module_name+"."+import_name)
// - 'DeeDex_Import' will return the dex itself if no '.' or '::' is contained in the name
// e.g.: 'DeeDex_Import("_debugger.breakpoint")'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDex_Import(DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDex_ImportEx(DEE_A_IN_Z char const *module_name, DEE_A_IN_Z char const *import_name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) void *) DeeDex_ImportNative(DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) void *) DeeDex_ImportNativeEx(DEE_A_IN_Z char const *module_name, DEE_A_IN_Z char const *import_name) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Call a dex import using format args
// The split between the import name and arguments is located at '('
// e.g.: >> data = DeeDex_Callf("_crypt.base64_decode(pIu)",p,s);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDex_Callf(
 DEE_A_IN_Z char const *name_and_fmt, ...) DEE_ATTRIBUTE_NONNULL((1));
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDex_VCallf(
 DEE_A_IN_Z char const *name_and_fmt, DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */

//////////////////////////////////////////////////////////////////////////
// Call a dex export and cast its return value to the
// specified result type, storing that value in '*result'
//  - The return type is specified in structf-style notation at
//    the end of 'name_and_fmt_and_result', following a '!'
//  - If no return type was specified, the call behaves just
//    'DeeDex_Callf', and nothing is written to '*result' while the return
//    value of 'DeeDex_Callf' is simply decref'ed after being checked for errors
// >> Dee_uint64_t uptime;
// >> if (DeeDex_CallAndCastf(&uptime,"_system.uptime()!I64u") != 0) { handle_error(); }
// >> Dee_StdPrintf(DEE_STDOUT,"Current uptime: %I64u\n",uptime);
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDex_CallAndCastf(
 DEE_A_OUT void *result, DEE_A_IN_Z char const *name_and_fmt_and_result, ...) DEE_ATTRIBUTE_NONNULL((1,2));
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDex_VCallAndCastf(
 DEE_A_OUT void *result, DEE_A_IN_Z char const *name_and_fmt_and_result,
 DEE_A_INOUT va_list args) DEE_ATTRIBUTE_NONNULL((1,2));
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */

//////////////////////////////////////////////////////////////////////////
// Get/Del/Set/Call an export of a dex module
// @throws Error.AttributeError: Unknown export name
// @throws ...:                  Same as export callbacks
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeDexModule_GetAttrString(
 DEE_A_INOUT_OBJECT(DeeDexModuleObject) *self, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeDexModule_HasAttrString(
 DEE_A_INOUT_OBJECT(DeeDexModuleObject) const *self, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeDexModule_DelAttrString(
 DEE_A_INOUT_OBJECT(DeeDexModuleObject) *self, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeDexModule_SetAttrString(
 DEE_A_INOUT_OBJECT(DeeDexModuleObject) *self, DEE_A_IN_Z char const *name,
 DEE_A_INOUT DeeObject *v) DEE_ATTRIBUTE_NONNULL((1,2,3));

//////////////////////////////////////////////////////////////////////////
// Returns the address of a natively exported
// variable/function from a given dex (GetProcAddress / dlsym)
// NOTE: These functions will always fail if 'DeeDexModule_Kind' isn't 'DEE_DEXMODULE_FLAGS_KIND_SO'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) void *) DeeDexModule_GetNativeAddress(
 DEE_A_INOUT_OBJECT(DeeDexModuleObject) *self, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_NOEXCEPT(NULL) void *) DeeDexModule_TryGetNativeAddress(
 DEE_A_INOUT_OBJECT(DeeDexModuleObject) *self, DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Returns the name and filename of a given dex module.
// e.g.: 'DeeDex_Open("_debugger")'
//    -> DeeDexModule_Name() -> "_debugger"
//    -> DeeDexModule_File() -> "/usr/lib/deemon/dex.102/_debugger.so"
// The name is equal to the argument used to open the dex (lower-case version on windows)
// The file is a path to the file that was opened for the dex.
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeDexModule_Name(
 DEE_A_IN_OBJECT(DeeDexModuleObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(DeeAnyStringObject) *) DeeDexModule_File(
 DEE_A_IN_OBJECT(DeeDexModuleObject) const *self) DEE_ATTRIBUTE_NONNULL((1));


#define DEE_DEXMODULE_FLAGS_NONE               DEE_UINT16_C(0x0000)
// These flags are added to the state of a dex module in the order they appear here.
// - Once set internally, a state flag can never be removed, meaning
//   that checking for the presence of one of these flags is thread-safe
//   and if the flag is present, you can be sure that it will stay that way.
#define DEE_DEXMODULE_FLAGS_STATE_INITIALIZING DEE_UINT16_C(0x0001) /*< The dex is currently initializing. */
#define DEE_DEXMODULE_FLAGS_STATE_INITIALIZED  DEE_UINT16_C(0x0002) /*< The dex was initialized. */
#define DEE_DEXMODULE_FLAGS_STATE_SHUTDOWN     DEE_UINT16_C(0x0004) /*< The dex was shut down (with this set, any non-finalizing operation will fail). */
#define DEE_DEXMODULE_FLAGS_STATE_FINALIZED    DEE_UINT16_C(0x0008) /*< The dex was finalized. */
#define DEE_DEXMODULE_FLAGS_STATE_MASK         DEE_UINT16_C(0x0FFF)
#define DEE_DEXMODULE_FLAGS_KIND_SO            DEE_UINT16_C(0x0000) /*< Regular dex loaded from an '.so' / '.dll' file. */
#define DEE_DEXMODULE_FLAGS_KIND_MASK          DEE_UINT16_C(0xF000)
#define DEE_DEXMODULE_FLAGS_STATE(x)         ((x)&DEE_DEXMODULE_FLAGS_STATE_MASK)
#define DEE_DEXMODULE_FLAGS_KIND(x)          ((x)&DEE_DEXMODULE_FLAGS_KIND_MASK)

DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint16_t) DeeDexModule_Flags(
 DEE_A_IN_OBJECT(DeeDexModuleObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#define DeeDexModule_Kind(ob)          DEE_DEXMODULE_FLAGS_KIND(DeeDexModule_Flags(ob))
#define DeeDexModule_IsInitialized(ob) ((DeeDexModule_Flags(ob)&DEE_DEXMODULE_FLAGS_STATE_INITIALIZED)!=0)
#define DeeDexModule_IsShutdown(ob)    ((DeeDexModule_Flags(ob)&DEE_DEXMODULE_FLAGS_STATE_SHUTDOWN)!=0)
#define DeeDexModule_IsFinalized(ob)   ((DeeDexModule_Flags(ob)&DEE_DEXMODULE_FLAGS_STATE_FINALIZED)!=0)


#ifdef DEE_LIMITED_API
extern DEE_A_EXEC Dee_size_t DeeDexModule_CollectMemoryEx(
 DEE_A_INOUT DeeDexModuleObject *self, DEE_A_IN Dee_uint32_t reason) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_EXEC Dee_size_t DeeDex_CollectMemoryEx(DEE_A_IN Dee_uint32_t reason);
#define DeeDexModule_CollectMemory(ob) DeeDexModule_CollectMemoryEx(ob,DEE_DEXCONTEXT_COLLECTMEMORY_REASON_NOMEM)
#define DeeDex_CollectMemory()         DeeDex_CollectMemoryEx(DEE_DEXCONTEXT_COLLECTMEMORY_REASON_NOMEM)
extern DEE_A_EXEC DEE_A_REF DeeDexModuleObject *DeeDexModule_New(
 DEE_A_IN_Z char const *name) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeDex_BroadcastEnvUpdate(
 DEE_A_IN DeeObject *key, DEE_A_IN DeeObject *value) DEE_ATTRIBUTE_NONNULL((1,2));
#endif


#ifdef DEE_LIMITED_API
extern DeeObject *DEE_CALL _deedexmodule_tp_attr_get(DeeDexModuleObject *self, DeeObject *attr);
extern int DEE_CALL _deedexmodule_tp_attr_del(DeeDexModuleObject *self, DeeObject *attr);
extern int DEE_CALL _deedexmodule_tp_attr_set(DeeDexModuleObject *self, DeeObject *attr, DeeObject *v);
#elif defined(DEE_LIMITED_DEX)
#define _deedexmodule_tp_attr_get  (*DeeType_GET_SLOT(&DeeDexModule_Type,tp_attr_get))
#define _deedexmodule_tp_attr_del  (*DeeType_GET_SLOT(&DeeDexModule_Type,tp_attr_del))
#define _deedexmodule_tp_attr_set  (*DeeType_GET_SLOT(&DeeDexModule_Type,tp_attr_set))
#endif


#ifdef DEE_LIMITED_DEX
// Initialization/Finalization reasons

#define DEE_DEX_CONTEXT_HEAD\
 Dee_uint32_t dc_kind; /*< The kind of context. */\
 DeeObject   *dc_self; /*< The dex module object. */
struct DeeDexContext_Common { DEE_DEX_CONTEXT_HEAD };
struct DeeDexContext_Initialize {
 DEE_DEX_CONTEXT_HEAD
#define DEE_DEXCONTEXT_INITIALIZE_REASON_UNSPEC     DEE_UINT32_C(0x00000000) /*< Unspecified. */
#define DEE_DEXCONTEXT_INITIALIZE_REASON_GETTER     DEE_UINT32_C(0x00000001) /*< An export is queried from '__getattr__'. */
#define DEE_DEXCONTEXT_INITIALIZE_REASON_HASXPT     DEE_UINT32_C(0x00000002) /*< An export is queried from '__hasattr__'. */
#define DEE_DEXCONTEXT_INITIALIZE_REASON_DELETE     DEE_UINT32_C(0x00000003) /*< An export is queried from '__delattr__'. */
#define DEE_DEXCONTEXT_INITIALIZE_REASON_SETTER     DEE_UINT32_C(0x00000004) /*< An export is queried from '__setattr__'. */
#define DEE_DEXCONTEXT_INITIALIZE_REASON_NATIVE     DEE_UINT32_C(0x00000005) /*< A native export is queried. */
#define DEE_DEXCONTEXT_INITIALIZE_REASON_ENUMXP     DEE_UINT32_C(0x00000006) /*< Module exports are being enumerated. */
#define DEE_DEXCONTEXT_INITIALIZE_REASON_KIND_MASK  DEE_UINT32_C(0x000000FF)
#define DEE_DEXCONTEXT_INITIALIZE_REASON_FLAGS_MASK DEE_UINT32_C(0xFFFFFF00)
#define DEE_DEXCONTEXT_INITIALIZE_REASON_KIND(reason)  ((reason)&DEE_DEXCONTEXT_INITIALIZE_REASON_KIND_MASK)
#define DEE_DEXCONTEXT_INITIALIZE_REASON_FLAGS(reason) ((reason)&DEE_DEXCONTEXT_INITIALIZE_REASON_FLAGS_MASK)
 Dee_uint32_t ci_reason; /*< Reason kind/flags for initialization (one of 'DEE_DEXCONTEXT_INITIALIZE_REASON_*'). */
};

struct DeeDexContext_Finalize {
 DEE_DEX_CONTEXT_HEAD
#define DEE_DEXCONTEXT_FINALIZE_REASON_UNSPEC     DEE_UINT32_C(0x00000000)
#define DEE_DEXCONTEXT_FINALIZE_REASON_DESTROY    DEE_UINT32_C(0x00000001)
#define DEE_DEXCONTEXT_FINALIZE_REASON_SHUTDOWN   DEE_UINT32_C(0x00000002)
#define DEE_DEXCONTEXT_FINALIZE_REASON_USERREQ    DEE_UINT32_C(0x00000003)
#define DEE_DEXCONTEXT_FINALIZE_REASON_KIND_MASK  DEE_UINT32_C(0x000000FF)
#define DEE_DEXCONTEXT_FINALIZE_REASON_FLAGS_MASK DEE_UINT32_C(0xFFFFFF00)
#define DEE_DEXCONTEXT_FINALIZE_REASON_KIND(reason)  ((reason)&DEE_DEXCONTEXT_FINALIZE_REASON_KIND_MASK)
#define DEE_DEXCONTEXT_FINALIZE_REASON_FLAGS(reason) ((reason)&DEE_DEXCONTEXT_FINALIZE_REASON_FLAGS_MASK)
 Dee_uint32_t cf_reason; /*< Reason kind/flags for finalization (one of 'DEE_DEXCONTEXT_FINALIZE_REASON_*'). */
};

struct DeeDexContext_CollectMem {
 DEE_DEX_CONTEXT_HEAD
#define DEE_DEXCONTEXT_COLLECTMEMORY_REASON_UNSPEC   DEE_UINT32_C(0x00000000)
#define DEE_DEXCONTEXT_COLLECTMEMORY_REASON_NOMEM    DEE_UINT32_C(0x00000001) /*< The system has run out of memory. */
#define DEE_DEXCONTEXT_COLLECTMEMORY_REASON_USERREQ  DEE_UINT32_C(0x00000002) /*< User-requested, non-critical collection. */
#define DEE_DEXCONTEXT_COLLECTMEMORY_REASON_FLAGCRIT DEE_UINT32_C(0x00010000) /*< The request is critical for preventing a crash. */
 Dee_uint32_t ccm_reason; /*< Reason why memory collection was requested. */
 Dee_size_t   ccm_total;  /*< Total amount of collected objects (increment for every collection before returning from 'DeeDex_Main'). */
};

#ifdef DEE_PRIVATE_DECL_DEE_VISIT_PROC
DEE_PRIVATE_DECL_DEE_VISIT_PROC
#undef DEE_PRIVATE_DECL_DEE_VISIT_PROC
#endif
struct DeeDexContext_Visit {
 DEE_DEX_CONTEXT_HEAD
 DeeVisitProc cv_visit;   /*< [1..1] Visit callback that should be executed on global references. */
 void        *cv_closure; /*< [?..?] Closure that should be supplied to 'cv_visit'. */
};

typedef union DeeDexContext DeeDexContext;
union DeeDexContext {
#define DEE_DEXCONTEXTKIND_NONE       0 /*< Unknown context kind. */
#define DEE_DEXCONTEXTKIND_INITIALIZE 1 /*< Module initialization event (guarantied to be the first event, if any). */
#define DEE_DEXCONTEXTKIND_FINALIZE   2 /*< Module finalization event (guarantied to be the last event, if any). */
#define DEE_DEXCONTEXTKIND_COLLECTMEM 3 /*< Memory has run low and must be collected (called from 'Dee_CollectMemory()'). */
#define DEE_DEXCONTEXTKIND_VISIT      4 /*< The module is being visited (run the given callback on all global object references you currently own). */
 Dee_uint32_t                    dc_kind;        /*< The kind of context event (one of 'DEE_DEXCONTEXTKIND_*'). */
 struct DeeDexContext_Common     dc_common;      /*< Data shared between all event types. */
 struct DeeDexContext_Initialize dc_initialize;  /*< DEE_DEXCONTEXTKIND_INITIALIZE. */
 struct DeeDexContext_Finalize   dc_finalize;    /*< DEE_DEXCONTEXTKIND_FINALIZE. */
 struct DeeDexContext_CollectMem dc_collectmem;  /*< DEE_DEXCONTEXTKIND_COLLECTMEM. */
 struct DeeDexContext_Visit      dc_visit;       /*< DEE_DEXCONTEXTKIND_VISIT. */
 Dee_uint8_t                     dc_padding[64]; /*< Force 64 bytes for future usage. */
};


#endif


#ifdef DEE_EXTENSION
// Per-extension export prototypes

//////////////////////////////////////////////////////////////////////////
// Called when a dex event takes place, such as initialization / finalization
//  - Returns 0 on success / negative on error
//  - This callback is optional
// WARNING: 'DEE_DEXCONTEXTKIND_VISIT' should be considered noexcept!
extern DEE_ATTRIBUTE_DLLEXPORT int DEE_CALL DeeDex_Main(DEE_A_INOUT DeeDexContext *context);

//////////////////////////////////////////////////////////////////////////
// List of extension exports
//  - This export is mandatory
// >> struct DeeDexExportDef DeeDex_Exports[] = {
// >>   DeeDex_EXPORT_OBJECT("my_export",&DeeMy_Export),
// >>   DeeDex_EXPORT_END
// >> };
extern DEE_ATTRIBUTE_DLLEXPORT struct DeeDexExportDef DeeDex_Exports[];


//////////////////////////////////////////////////////////////////////////
// Dex attributes (parsed by the dex compiler):
// NOTES:
//   - These attributes can only occur in the
//     main source files of dex extensions.
//   - These must all start with '//@'
//////////////////////////////////////////////////////////////////////////
// >> @dex.name = my_dex
//    Name of the dex; value a user must supply to '__builtin_dex()':
//////////////////////////////////////////////////////////////////////////
// >> @dex.version_dependent = true
//    A dex should be version-dependent if it uses some part of
//    the c-api, that is considered DEE_LIMITED_DEX.
//    When a dex is marked as version_dependent=true, it can only
//    be loaded if deemon has the same DEE_API_VERSION as was
//    listed in <deemon/version.h> when the dex was compiled.
//////////////////////////////////////////////////////////////////////////
// >> @dex.require = <stdlib.h>
// >> @dex.require = <stdlib.h> (Missing default header)
// >> @dex.require = <stdlib.h> ($ echo "Moooo!" | cowsay)
//    Specifies a header that is required for the dex to compile
//    If the header isn't available, an optional help message can be written
//    in parenthesis after the header name.
//    If the help string starts with a '$', that character is removed
//    and the remained of the help message is shown to the user, telling
//    him that executing the specified command will install additional
//    libraries that will include that header.
//////////////////////////////////////////////////////////////////////////
// >> @compiler.incpath = /my/include/path:/my/other/include/path
//    Specifies a ':'-separated list of additional include
//    paths that will be available during compilation.
//////////////////////////////////////////////////////////////////////////
// >> @linker.libpath = /my/lib/path:/my/other/lib/path
//    Specifies a ':'-separated list of additional library
//    paths that will be available during compilation.
//////////////////////////////////////////////////////////////////////////
// >> @linker.lib = my_library
//    Specifies a library that is required for properly linking the dex.
//    The library name is what would be written as '-l*' on the gcc commandline
//////////////////////////////////////////////////////////////////////////
// >> @linker.pthread
//    Link the dex against '-pthread'
//////////////////////////////////////////////////////////////////////////

#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_H */
