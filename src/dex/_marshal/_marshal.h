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
#ifndef GUARD_DEEMON_DEX_MARSHAL_H
#define GUARD_DEEMON_DEX_MARSHAL_H 1
 
#include <deemon/__conf.inl>
#include <deemon/dex.h>
#include <deemon/object.h>
#include <deemon/type.h>
#include <deemon/optional/type_decl.h>
#include <deemon/marshal.h>
#include <deemon/mp/mutex.h>


//////////////////////////////////////////////////////////////////////////
// Usercode access to some of the more lower-level features of marshaling.
//   While the deemon core already provides access to very high-level
//   and streamlined marshaling and unmarshaling, the marshal is capable
//   of even more in-depth functionality, as mapped by this dex extension.
//   
//   NOTE: This dex does not add any new functionality to
//         the marshal that would not exist without it.

DEE_DECL_BEGIN

DEE_OBJECT_DEF(DeeMarshalReadMapObject);
DEE_OBJECT_DEF(DeeMarshalWriteMapObject);


struct DeeMarshalReadMapObject {
 DEE_OBJECT_HEAD
 // NOTE: We need a real mutex here, as there's no other
 //       way of us enforcing a thread-safe use of the map.
 //       Deemon's standard doesn't specify whether or not
 //       usercode can be executed from inside the marshal,
 //       meaning that it's not something illegal.
 //       Scratch that. As a matter of fact: It does say!
 //        -> Marshal implementations are allowed to run DEE_A_EXEC
 //           functions, meaning that a simply call of Dee_CollectMemory
 //           could result in the destructor of some user object
 //           being executed, which in turn is allowed to use us!
 //        -> So in the end, we need a recursive lock because
 //           the calling thread will have to hold an exclusive
 //           lock to the map whenever using it.
 struct DeeNativeMutex    mrm_lock; /*< Lock for this map. */
 struct DeeMarshalReadMap mrm_map;  /*< Stored read-map. */
};

struct DeeMarshalWriteMapObject {
 DEE_OBJECT_HEAD
 // Same reason for 'DeeNativeMutex' as in 'DeeMarshalReadMapObject'
 struct DeeNativeMutex     mwm_lock; /*< Lock for this map. */
 struct DeeMarshalWriteMap mwm_map;  /*< Stored write-map. */
};


extern DeeTypeObject DeeMarshalReadMap_Type;
extern DeeTypeObject DeeMarshalWriteMap_Type;


#ifndef DEE_MARSHAL_WRITEFLAG_MASK
#define DEE_MARSHAL_WRITEFLAG_MASK \
 (DEE_MARSHAL_WRITEFLAG_NONE\
 |DEE_MARSHAL_WRITEFLAG_REFERENCE_ALL\
 |DEE_MARSHAL_WRITEFLAG_TYPE_NAMES\
 |DEE_MARSHAL_WRITEFLAG_UUID_REFS)
#endif


extern DEE_A_RET_EXCEPT(-1) int DeeMarshal_ConfirmVersion(DEE_A_IN DeeMarshalVersion version);
extern DEE_A_RET_EXCEPT(-1) int DeeMarshal_ConfirmWriteFlags(DEE_A_IN Dee_uint32_t flags);



DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_MARSHAL_H */
