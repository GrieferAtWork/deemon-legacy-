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
#ifndef GUARD_DEEMON_MARSHAL_H
#define GUARD_DEEMON_MARSHAL_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/uuid.h>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif

struct DeeMarshalReadMap;
struct DeeMarshalWriteMap;

#define DEE_MARSHAL_WRITEFLAG_NONE           0x00000000

// If set, all gc object written can be referenced
//  - Might reduce binary size for very big objects
//  - Increases binary size for smaller objects
#define DEE_MARSHAL_WRITEFLAG_REFERENCE_ALL  0x00000001

// Write type names
#define DEE_MARSHAL_WRITEFLAG_TYPE_NAMES     0x00000002

// Allow uuid references
// WARNING: Will cause unknown uuid errors if the
//          target doesn't recognize a given uuid.
// NOTE: This option is disabled for compiled deemon code
#define DEE_MARSHAL_WRITEFLAG_UUID_REFS      0x00000004

// Default set of flags
#define DEE_MARSHAL_WRITEFLAG_DEFAULT  \
 (DEE_MARSHAL_WRITEFLAG_TYPE_NAMES|DEE_MARSHAL_WRITEFLAG_UUID_REFS)

typedef Dee_uint16_t DeeMarshalVersion;
#define DEE_MARSHAL_V100 0 // v100 (UNSUPPORTED; original version).
#define DEE_MARSHAL_V101 1 // v101 (new in deemon 101:1).
#define DEE_MARSHAL_V102 2 // v102 (new in deemon 102:0).

// Current marshal version
//   - Used as default when reading marshaled objects
//   - Version that objects are written in by 'DeeMarshal_WriteObject'
//   - Reading an object with a different version than it was
//     written in may result in invalid data being read.
#define DEE_MARSHAL_VERSION  DEE_MARSHAL_V102

#ifdef DEE_LIMITED_DEX
struct _DeeMarshalWriteRef {
 Dee_uint32_t         mwr_id; /*< Id of this object. */
 DeeTypeObject const *mwr_tp; /*< [1..1] Type of the object (not a reference; kept alive through 'ref_ob'). */
 DEE_A_REF DeeObject *mwr_ob; /*< [1..1] Object stored in this slot. */
};
#define _DeeMarshalWriteRef_Quit(ob)\
do{ Dee_DECREF((ob)->mwr_ob); }while(0)
#define _DeeMarshalWriteRef_Visit(ob)\
do{ Dee_VISIT((ob)->mwr_ob); }while(0)
struct DeeMarshalWriteMap {
 Dee_uint32_t                mwm_flags;    /*< Flags describing how objects are written. */
 Dee_uint32_t                mwm_ref_next; /*< Next reference id. */
 Dee_size_t                  mwm_refc;     /*< Amount of known references. */
 struct _DeeMarshalWriteRef *mwm_refv;     /*< [0..ob_refc][owned] List of known references. */
};
#define DeeMarshalWriteMap_INIT() {DEE_MARSHAL_WRITEFLAG_DEFAULT,0,0,NULL}
#define _DeeMarshalWriteMap_Init(ob,flags) \
do{\
 (ob)->mwm_flags = (flags);\
 (ob)->mwm_ref_next = 0;\
 (ob)->mwm_refc = 0;\
 (ob)->mwm_refv = NULL;\
}while(0)
#define _DeeMarshalWriteMap_Quit(ob)\
do{\
 struct _DeeMarshalWriteRef *_wm_iter,*_wm_end;\
 _wm_end = (_wm_iter = (ob)->mwm_refv)+(ob)->mwm_refc;\
 while (_wm_iter != _wm_end) { _DeeMarshalWriteRef_Quit(_wm_iter); ++_wm_iter; }\
 free((ob)->mwm_refv);\
}while(0)
#define _DeeMarshalWriteMap_Visit(ob)\
do{\
 struct _DeeMarshalWriteRef *_wm_iter,*_wm_end;\
 _wm_end = (_wm_iter = (ob)->mwm_refv)+(ob)->mwm_refc;\
 while (_wm_iter != _wm_end) { _DeeMarshalWriteRef_Visit(_wm_iter); ++_wm_iter; }\
}while(0)

#ifdef DEE_LIMITED_API
extern DEE_A_RET_EXCEPT(-1) Dee_uint32_t _DeeMarshalWriteMap_RegisterID(
 DEE_A_IN struct DeeMarshalWriteMap *self, DEE_A_IN DeeTypeObject const *tp_ob,
 DEE_A_INOUT DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1,2,3));
extern DEE_A_RET_NOEXCEPT(-1) Dee_uint32_t _DeeMarshalWriteMap_FindObject(
 DEE_A_IN struct DeeMarshalWriteMap const *self, DEE_A_IN DeeTypeObject const *tp_ob,
 DEE_A_INOUT DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1,2,3));
#define _DeeMarshalWriteMap_FindStringWithLength _DeeMarshalWriteMap_FindUtf8StringWithLength
extern DEE_A_RET_NOEXCEPT(-1) Dee_uint32_t _DeeMarshalWriteMap_FindWideStringWithLength(
 DEE_A_IN struct DeeMarshalWriteMap const *self, DEE_A_IN Dee_size_t len,
 DEE_A_IN_R(len) Dee_WideChar const *s) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_NOEXCEPT(-1) Dee_uint32_t _DeeMarshalWriteMap_FindUtf8StringWithLength(
 DEE_A_IN struct DeeMarshalWriteMap const *self, DEE_A_IN Dee_size_t len,
 DEE_A_IN_R(len) Dee_Utf8Char const *s) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_NOEXCEPT(-1) Dee_uint32_t _DeeMarshalWriteMap_FindUtf16StringWithLength(
 DEE_A_IN struct DeeMarshalWriteMap const *self, DEE_A_IN Dee_size_t len,
 DEE_A_IN_R(len) Dee_Utf16Char const *s) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_NOEXCEPT(-1) Dee_uint32_t _DeeMarshalWriteMap_FindUtf32StringWithLength(
 DEE_A_IN struct DeeMarshalWriteMap const *self, DEE_A_IN Dee_size_t len,
 DEE_A_IN_R(len) Dee_Utf32Char const *s) DEE_ATTRIBUTE_NONNULL((1));
#endif

struct _DeeMarshalReadRef {
 Dee_uint32_t         ref_id; /*< Object id (for lookup). */
 DEE_A_REF DeeObject *ref_ob; /*< [1..1] The that is being referenced. */
};
#define _DeeMarshalReadRef_Quit(ob)\
do{ Dee_DECREF((ob)->ref_ob); }while(0)
#define _DeeMarshalReadRef_Visit(ob)\
do{ Dee_VISIT((ob)->ref_ob); }while(0)

struct DeeMarshalReadMap {
 DeeMarshalVersion          mrm_version; /*< Marshal version used when the object was written. */
 Dee_size_t                 mrm_refc;    /*< Amount of defined object references defined. */
 struct _DeeMarshalReadRef *mrm_refv;    /*< [0..ob_refc][owned] List of defined object references. */
 struct DeeUUID            *mrm_uuid;    /*< [0..1] UUID of the type currently being read (NULL if not a user-type; could be used for version-control). */
};
#define DeeMarshalReadMap_INIT(ver)  {ver,0,NULL}
#define _DeeMarshalReadMap_Init(ob,version)\
do{\
 DEE_ASSERT(DeeMarshal_IsSupported(version));\
 (ob)->mrm_version = (version);\
 (ob)->mrm_refc = 0;\
 (ob)->mrm_refv = NULL;\
 (ob)->mrm_uuid = NULL;\
}while(0)
#define _DeeMarshalReadMap_Quit(ob)\
do{\
 struct _DeeMarshalReadRef *_rm_iter,*_rm_end;\
 DEE_ASSERT((ob));\
 _rm_end = (_rm_iter = (ob)->mrm_refv)+(ob)->mrm_refc;\
 while (_rm_iter != _rm_end) { _DeeMarshalReadRef_Quit(_rm_iter); ++_rm_iter; }\
 free((ob)->mrm_refv);\
}while(0)
#define _DeeMarshalReadMap_Visit(ob)\
do{\
 struct _DeeMarshalReadRef *_rm_iter,*_rm_end;\
 DEE_ASSERT((ob));\
 _rm_end = (_rm_iter = (ob)->mrm_refv)+(ob)->mrm_refc;\
 while (_rm_iter != _rm_end) { _DeeMarshalReadRef_Visit(_rm_iter); ++_rm_iter; }\
}while(0)

#ifdef DEE_LIMITED_API
extern DEE_A_RET_EXCEPT(-1) int _DeeMarshalReadMap_CheckVersion(DEE_A_IN DeeMarshalVersion version);
extern DEE_A_RET_EXCEPT(-1) int _DeeMarshalReadMap_TryPushRefWithoutIncref(
 DEE_A_INOUT struct DeeMarshalReadMap *self, DEE_A_IN Dee_uint32_t id, DEE_A_IN DeeObject *ob);
extern void _DeeMarshalReadMap_PopRefWithoutDecref(DEE_A_INOUT struct DeeMarshalReadMap *self, DEE_A_IN DeeObject *ob);
extern DEE_A_RET_NOEXCEPT(NULL) DeeObject *_DeeMarshalReadMap_GetRef(
 DEE_A_INOUT struct DeeMarshalReadMap *self, DEE_A_IN Dee_uint32_t id);
#endif

#define DeeMarshalWriteMap_GET_FLAGS(ob)          (ob)->mwm_flags
#define DeeMarshalWriteMap_SET_FLAGS(ob,v) (void)((ob)->mwm_flags=(v))
#else
#define DeeMarshalWriteMap_GET_FLAGS       DeeMarshalWriteMap_GetFlags
#define DeeMarshalWriteMap_SET_FLAGS       DeeMarshalWriteMap_SetFlags
#endif





DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint32_t) DeeMarshalWriteMap_GetFlags(
 DEE_A_IN struct DeeMarshalWriteMap const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeMarshalWriteMap_SetFlags(
 DEE_A_INOUT struct DeeMarshalWriteMap *self, DEE_A_IN Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1));

DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct DeeMarshalWriteMap *) DeeMarshalWriteMap_New(void);
DEE_FUNC_DECL(DEE_A_EXEC void) DeeMarshalWriteMap_Delete(DEE_A_IN DEE_A_OUT_INVALID struct DeeMarshalWriteMap *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) struct DeeMarshalReadMap *) DeeMarshalReadMap_New(DEE_A_IN DeeMarshalVersion version);
DEE_FUNC_DECL(DEE_A_EXEC void) DeeMarshalReadMap_Delete(DEE_A_IN DEE_A_OUT_INVALID struct DeeMarshalReadMap *self) DEE_ATTRIBUTE_NONNULL((1));


//////////////////////////////////////////////////////////////////////////
// Returns the UUID of the type currently being read, or NULL if unknown.
// NOTE: When called with the given map from the marshal constructor
//       of a user object, this function will never return NULL.
DEE_FUNC_DECL(DEE_A_RET_MAYBE_NULL struct DeeUUID const *) DeeMarshalReadMap_GetUUID(
 DEE_A_IN struct DeeMarshalReadMap const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED DeeMarshalVersion) DeeMarshalReadMap_Version(
 DEE_A_IN struct DeeMarshalReadMap const *self) DEE_ATTRIBUTE_NONNULL((1));
#ifdef DEE_LIMITED_DEX
#define DeeMarshalReadMap_UUID(ob)    ((struct DeeUUID const *)(ob)->mrm_uuid)
#define DeeMarshalReadMap_VERSION(ob) ((DeeMarshalVersion)(ob)->mrm_version)
#else
#define DeeMarshalReadMap_UUID    DeeMarshalReadMap_GetUUID
#define DeeMarshalReadMap_VERSION DeeMarshalReadMap_Version
#endif


//////////////////////////////////////////////////////////////////////////
// Returns true (non-zero) if objects of the given 'version' can be read.
// >> If this function returns false (zero), atempting to read objects will cause an error to be thrown
DEE_FUNC_DECL(DEE_A_RET_WUNUSED int) DeeMarshal_IsSupported(DEE_A_IN DeeMarshalVersion version);

//////////////////////////////////////////////////////////////////////////
// -- Read/Write an object to a file --
// Every object implementing the v101 marshal interface can be written this way.
// NOTES:
//   - User-classes can be marshaled by default
//     - If the actual class type should not be set
//       the 'DEE_MARSHAL_WRITEFLAG_UUID_REFS' flag and
//       make sure that the class has the uuid attribute set:
//       >> class [[uuid("{A1726FAD-102A-4F8A-993F-2675A0947BC7}")]] my_class { ... };
//       >> // doesn't store 'my_class', only writing an instance of it
//       >> // s.a.: "test/test_serialized_uuid.dee"
//       >> file.writem(my_class());
//   - None of these function perform any cleanup in 'file' after
//     an error occurred. If you want to keep using 'file',
//     you should store the old file position before calling any
//     of these functions.
//   - DeeMarshal_WriteObjectWithMap may raise a ValueError if the object
//     is greater than the maximum supported object size.
//   - All data written is relative to itself, meaning
//     you can safely write an object into one stream, then
//     copy the stream data somewhere else.
//   - Containers, such as set, list, tuple or dict are written recursively,
//     meaning that anything they contain must implement the marshal as well.
//   - Written object sharing the same write-map
//     must also be read using the same read-map.
//   - After being marshaled and turned back into an object, 'util::id()' will
//     not return the same id as it did for the original object.
//     (Not counting the very slim chance there is for this to happen when the
//      original object gets destroyed in the mean time)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeMarshal_ReadObject(DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_IN DeeMarshalVersion version) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeMarshal_ReadObjects(DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_IN Dee_size_t resultc, DEE_A_OUT_W(resultc) DeeObject **resultv, DEE_A_IN DeeMarshalVersion version) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeMarshal_WriteObject(DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_IN DeeObject *ob) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeMarshal_WriteObjects(DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_IN Dee_size_t objectc, DEE_A_IN_R(objectc) DeeObject *const *objectv) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeMarshal_WriteObjectEx(DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_IN DeeObject *ob, DEE_A_IN Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeMarshal_WriteObjectsEx(DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_IN Dee_size_t objectc, DEE_A_IN_R(objectc) DeeObject *const *objectv, DEE_A_IN Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Advanced read/write (using a custom map)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *) DeeMarshal_ReadObjectWithMap(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalReadMap *file_map) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeMarshal_ReadObjectsWithMap(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalReadMap *file_map,
 DEE_A_IN Dee_size_t resultc, DEE_A_OUT_W(resultc) DeeObject **resultv) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeMarshal_WriteObjectsWithMap(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalWriteMap *file_map,
 DEE_A_IN Dee_size_t objectc, DEE_A_IN_R(objectc) DeeObject *const *objectv) DEE_ATTRIBUTE_NONNULL((1,2));

#ifdef DEE_LIMITED_API
extern DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeMarshal_TWriteNewObjectWithMap(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalWriteMap *file_map,
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
#endif
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeMarshal_TWriteObjectWithMap(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalWriteMap *file_map,
 DEE_A_IN DeeTypeObject const *tp_self, DEE_A_INOUT DeeObject *self) DEE_ATTRIBUTE_NONNULL((1,2,3,4));
DEE_STATIC_INLINE(DEE_ATTRIBUTE_NONNULL((1,2,3)) DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeMarshal_WriteObjectWithMap(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalWriteMap *file_map, DEE_A_IN DeeObject *self)
 { DEE_ASSERT(DeeObject_Check(self)); return DeeMarshal_TWriteObjectWithMap(file,file_map,Dee_TYPE(self),self); }

#define DeeMarshal_WriteStringWithMapAndLength DeeMarshal_WriteUtf8StringWithMapAndLength
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeMarshal_WriteWideStringWithMapAndLength(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalWriteMap *file_map,
 DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_WideChar const *s) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeMarshal_WriteUtf8StringWithMapAndLength(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalWriteMap *file_map,
 DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf8Char const *s) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeMarshal_WriteUtf16StringWithMapAndLength(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalWriteMap *file_map,
 DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf16Char const *s) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeMarshal_WriteUtf32StringWithMapAndLength(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_INOUT struct DeeMarshalWriteMap *file_map,
 DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) Dee_Utf32Char const *s) DEE_ATTRIBUTE_NONNULL((1,2));


//////////////////////////////////////////////////////////////////////////
// Writes the header, indicating that a file is a compiled deemon file
// @param file: The file to write the header to
// @return: <0 on error, 0 on success
// - The written header always refers to the current deemon version (DEE_VERSION_FULL)
//   as well as the current marshal version (DEE_MARSHAL_VERSION)
// - The header can later be re-read using 'DeeMarshal_IsCompiledFile'
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeMarshal_WriteCompiledFileHeader(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Checks for the compiled file header and skips it if it was found
// @param file: The file to read the header from
// @param deemon_version: The deemon version that write the header
// @param marshal_version: The marshal version that write the header
// @return: <0 on error, 0 if no header was found, >0 if a header was found
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT_FAIL(-1,0) int) DeeMarshal_IsCompiledFile(
 DEE_A_INOUT_OBJECT(DeeFileObject) *file, DEE_A_OUT Dee_uint32_t *deemon_version,
 DEE_A_OUT DeeMarshalVersion *marshal_version) DEE_ATTRIBUTE_NONNULL((1,2,3));




//////////////////////////////////////////////////////////////////////////
// Register a type for use in marshaling
//  - Returns -1 on error
//  - Returns 0 on success
//  - Returns 1 and does nothing if the type doesn't support marshaling
//  - Returns 2 and does nothing if the type was already registered
//  - Returns 3 and does nothing if the type has intrinsic marshaling support (not needed to be registered)
// NOTE: This function will not keep a reference to the type for book-keeping,
//       and the type will automatically be unregistered from within its destructor.
// NOTE: When registering a static type from a dex module initializer,
//       'DeeMarshal_UnregisterType' must be called during finalization.
// HINT: If you need a way to generate UUIDs, simply google "uuid generator".
//       Alternatively, my Visual Studio has a ~Generate-GUID~ option under ~tools~.
// 
// === NEW IN V102 ===
// A type may specify a list of alias UUIDs, that will be registered as well.
// If any of the alias UUIDs was already in use, an error is thrown too.
//  - Using alias UUIDs function, you can implement version-control simply
//    by generating a new UUID and registering the old one as an alias.
//  - The list of aliases is terminated by an empty uuid
//  - When your type is actually read, you can retrieve
//    the used uuid using 'DeeMarshalReadMap_UUID'.
//  - To specify a list of alias UUIDs, use 'DEE_TYPE_MARSHAL_v101'
//  - NOTE: Don't use the UUIDs below, because some other,
//          lazy ~genius~ will have probably used them.
// >> static struct DeeUUID const _my_type_alias_uuids[] = {
// >>   DeeUUID_PP_INIT((3dc59962,89bf,47ba,8215,989731f767b4)),
// >>   DeeUUID_PP_INIT((bdd1b33b,e0d3,4a23,9838,d8b080b39f6e)),
// >>   DeeUUID_PP_INIT((2e66015e,0e66,48ef,99ec,d8b252da9868)),
// >>   DeeUUID_EMPTY()
// >> };
// >> static struct DeeTypeMarshal _my_type_tp_marshal = DEE_TYPE_MARSHAL_v101(
// >>  DeeUUID_PP_INIT((5a655441,53c4,471a,ba35,29bb9d2627c0)),
// >>  member(&_my_type_tp_marshal_ctor),member(&_my_type_tp_marshal_put),
// >>  member(_my_type_alias_uuids));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT_FAIL(-1,1) int) DeeMarshal_RegisterType(
 DEE_A_INOUT DeeTypeObject *marshal_type) DEE_ATTRIBUTE_NONNULL((1));

#ifdef DEE_LIMITED_DEX
// Unregister a marshaled type (called from DeeType_Type.tp_dtor for marshal-enabled types)
// NOTE: If you register a marshal type from a dex during initialization,
//       you must unregister it during finalization using this function.
DEE_FUNC_DECL(void) DeeMarshal_UnregisterType(
 DEE_A_INOUT DeeTypeObject *marshal_type) DEE_ATTRIBUTE_NONNULL((1));
#endif

#ifdef DEE_LIMITED_API
//////////////////////////////////////////////////////////////////////////
// Returns the type associated with a given uuid, or NULL if the uuid is unknown
// NOTE: The returned type is guarantied to implement 'tp_marshal' immediately.
extern DEE_A_RET_NOEXCEPT_REF DeeTypeObject *DeeMarshal_LookupType(
 DEE_A_IN struct DeeUUID const *uuid) DEE_ATTRIBUTE_NONNULL((1));
extern DEE_A_RET_EXCEPT(-1) int _DeeMarshal_DoRegisterType(
 DEE_A_INOUT DeeTypeObject *marshal_type) DEE_ATTRIBUTE_NONNULL((1));
// v should always be a no-op, but some types might forget to unregister themselves
extern void _DeeMarshal_ClearRegisteredTypes(void);
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_MARSHAL_H */
