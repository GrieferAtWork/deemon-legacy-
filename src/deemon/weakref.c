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
#ifndef GUARD_DEEMON_WEAKREF_C
#define GUARD_DEEMON_WEAKREF_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/bool.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/integer.h>
#include <deemon/generic_vtable.h>
#include <deemon/marshal.h>
#include <deemon/memberdef.h>
#include <deemon/none.h>
#include <deemon/tuple.h>
#include <deemon/weakref.h>
#include <deemon/optional/object_pool.h>

// /src/*
#include <deemon/__xconf.inl>
#include <deemon/marshal_data.h>


// */ (nano...)

DEE_DECL_BEGIN

DEE_OBJECTPOOL_DEFINE(weakref,DeeWeakrefObject)

DEE_A_RET_OBJECT_EXCEPT_REF(DeeWeakrefObject) *
DeeWeakref_New(DEE_A_INOUT DeeObject *value) {
 DeeWeakrefObject *result;
 if DEE_LIKELY((result = DEE_OBJECTPOOL_ALLOC(weakref)) != NULL) {
  DeeObject_INIT(result,&DeeWeakref_Type);
  Dee_WEAKINCREF(result->wr_value = value);
 }
 return (DeeObject *)result;
}

DEE_A_RET_WUNUSED int DeeWeakref_Alive(
 DEE_A_INOUT_OBJECT(DeeWeakrefObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeWeakref_Check(self));
 return DeeWeakref_ALIVE(self);
}

DEE_A_RET_NOEXCEPT_REF DeeObject *DeeWeakref_Lock(
 DEE_A_INOUT_OBJECT(DeeWeakrefObject) *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeWeakref_Check(self));
 return DeeWeakref_LOCK(self);
}

DeeError_NEW_EXTERN(_DeeError_DeadWeakReference,&DeeErrorType_ReferenceError,"Dead weak reference");
DEE_A_RET_EXCEPT_REF DeeObject *DeeWeakref_LockWithError(
 DEE_A_INOUT_OBJECT(DeeWeakrefObject) *self) {
 DeeObject *result = DeeWeakref_LOCK(self);
 if DEE_UNLIKELY(!result) DeeError_Throw((DeeObject *)&_DeeError_DeadWeakReference);
 return result;
}

Dee_size_t DeeWeakref_ClearPool(void) {
 return DEE_OBJECTPOOL_CLEAR(weakref);
}







static DeeWeakrefObject *_deeweakref_tp_alloc(DeeTypeObject *tp) {
 DeeWeakrefObject *result = DEE_OBJECTPOOL_ALLOC(weakref);
 if (result) DeeObject_INIT(result,tp);
 return result;
}
static void _deeweakref_tp_free(DeeTypeObject *DEE_UNUSED(tp), DeeWeakrefObject *ob) {
 DEE_OBJECTPOOL_FREE(weakref,ob);
}
static int _deeweakref_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeWeakrefObject *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeWeakref_Check(self));
 Dee_WEAKINCREF(self->wr_value = Dee_None);
 return 0;
}
static void _deeweakref_tp_dtor(DeeWeakrefObject *self) {
 Dee_WEAKDECREF(self->wr_value);
}
static int _deeweakref_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeWeakrefObject *self, DeeWeakrefObject *right) {
 DEE_ASSERT(DeeObject_Check(self) && DeeWeakref_Check(self));
 DEE_ASSERT(DeeObject_Check(right) && DeeWeakref_Check(right));
 Dee_WEAKINCREF(self->wr_value = right->wr_value);
 return 0;
}
DeeString_NEW_STATIC(_dee_weakref_dead_text,"<weak -> dead>");
static DeeObject *_deeweakref_tp_str(DeeWeakrefObject *self) {
 DeeObject *result,*ob;
 if DEE_LIKELY((ob = DeeWeakref_LOCK(self)) != NULL) {
  result = DeeString_Newf("<weak -> %k>",ob);
  Dee_DECREF(ob);
 } else {
  Dee_INCREF(result = (DeeObject *)&_dee_weakref_dead_text);
 }
 return result;
}
static DeeObject *_deeweakref_tp_repr(DeeWeakrefObject *self) {
 DeeObject *result,*ob;
 if DEE_LIKELY((ob = DeeWeakref_LOCK(self)) != NULL) {
  result = DeeString_Newf("<weak -> %r>",ob);
  Dee_DECREF(ob);
 } else {
  Dee_INCREF(result = (DeeObject *)&_dee_weakref_dead_text);
 }
 return result;
}
static int _deeweakref_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeWeakrefObject *self, DeeObject *args) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:weakref",&self->wr_value) != 0) return -1;
 Dee_WEAKINCREF(self->wr_value);
 return 0;
}
static int _deeweakref_tp_bool(DeeWeakrefObject *self) {
 return DeeWeakref_ALIVE(self);
}
static DeeObject *_deeweakref_tp_not(DeeWeakrefObject *self) {
 DeeReturn_Bool(!DeeWeakref_ALIVE(self));
}


static DeeObject *_deeweakref_lock(
 DeeWeakrefObject *self, DeeTupleObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result,*error_val = NULL;
 if DEE_UNLIKELY(DeeTuple_Unpack((DeeObject *)args,"|o:lock",&error_val) != 0) return NULL;
 if (error_val) {
  if ((result = DeeWeakref_Lock((DeeObject *)self)) != NULL) return result;
  DeeReturn_NEWREF(error_val);
 }
 return DeeWeakref_LockWithError((DeeObject *)self);
}
static DeeObject *_deeweakref_try_lock(
 DeeWeakrefObject *self, DeeTupleObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack((DeeObject *)args,":try_lock") != 0) return NULL;
 if DEE_UNLIKELY((result = DeeWeakref_Lock((DeeObject *)self)) == NULL) result = DeeNone_New();
 return result;
}
static DeeObject *_deeweakref_alive(
 DeeWeakrefObject *self, DeeTupleObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack((DeeObject *)args,":alive") != 0) return NULL;
 DeeReturn_Bool(DeeWeakref_ALIVE(self));
}

#if !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS
#define _deeweakref_tp_members DeeType_DEFAULT_SLOT(tp_members)
#else /* !DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */
static struct DeeMemberDef const _deeweakref_tp_members[] = {
 DEE_MEMBERDEF_NAMED_RO_v100("__wr_value",DeeWeakrefObject,wr_value,weak_object),
 DEE_MEMBERDEF_END_v100
};
#endif /* DEE_XCONFIG_HAVE_HIDDEN_MEMBERS */

static struct DeeMethodDef const _deeweakref_tp_methods[] = {
 DEE_METHODDEF_CONST_v100("lock",member(&_deeweakref_lock),"() -> object\n"
  "(object fallback) -> object\n"
  "@throws Error.ValueError.ReferenceError: The object is dead and no fallback was provided\n"
  "@return: A full reference to the stored object, or @fallback if the reference is dead.\n"
  "\tLocks the weakref and returns its value\n"
  "\tThrows an Error.ValueError.ReferenceError or returns @fallback if the object is dead"),
 DEE_METHODDEF_CONST_v100("try_lock",member(&_deeweakref_try_lock),"() -> object\n"
  "\tDEPRECATED: use 'lock(none)' instead.\n"
  "\tLocks the weakref and returns its value.\n"
  "\tReturns none if the object is dead"),
 DEE_METHODDEF_CONST_v100("alive",member(&_deeweakref_alive),"alive() -> bool\n"
  "@return: #true if the weakref is alive, #false otherwise\n"
  "\tThe returned value cannot be trusted when the object is used by multiple threads, "
    "as the alive state may have already changed by the time this function returns."),
 DEE_METHODDEF_END_v100
};



static int DEE_CALL _deeweakref_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeWeakrefObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *map) {
 Dee_uint8_t contains_object;
 DeeObject *contained_object;
 if (DeeFile_Getc(infile,&contains_object) != 0) return -1;
 if (contains_object) {
  if ((contained_object = DeeMarshal_ReadObjectWithMap(infile,map)) == NULL) return -1;
 } else {
  // A bit ugly, but we have to create some kind of dangling object here...
  if ((contained_object = DeeType_NewInstanceDefault(&DeeObject_Type)) == NULL) return -1;
 }
 Dee_WEAKINCREF(self->wr_value = contained_object);
 Dee_DECREF(contained_object);
 return 0;
}
static int DEE_CALL _deeweakref_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeWeakrefObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *map) {
 DeeObject *locked_object; int result;
 locked_object = DeeWeakref_LOCK(self);
 if (locked_object) {
  if (DeeFile_Putc(outfile,1) != 0) { Dee_DECREF(locked_object); return -1; }
  result = DeeMarshal_WriteObjectWithMap(outfile,map,locked_object);
  Dee_DECREF(locked_object);
  return result;
 } else return DeeFile_Putc(outfile,0);
}
static struct DeeTypeMarshal _deeweakref_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(weakref)),
 member(&_deeweakref_tp_marshal_ctor),member(&_deeweakref_tp_marshal_put));


DeeTypeObject DeeWeakref_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("weakref"),member(
  "(object ob) -> weakref\n"
  "\tReturns a weak reference to any object.\n"
  "\tWeak references can be used to weakly reference an object without hindering "
    "it from being destructed when no longer referenced the normal way.\n"
  "\tLocking a weak reference with #weakref.lock will yield the weakly referenced object "
    "or cause an #Error.ValueError.ReferenceError to be thrown if the object has been destructed."),
  member(DEE_TYPE_FLAG_NO_LIFETIME_EFFECT),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeWeakrefObject),
  member(&_deeweakref_tp_alloc),member(&_deeweakref_tp_ctor),
  member(&_deeweakref_tp_copy_ctor),null,member(&_deeweakref_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(
  member(&_deeweakref_tp_free),member(&_deeweakref_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deeweakref_tp_str),member(&_deeweakref_tp_repr),null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deeweakref_tp_not),member(&_deeweakref_tp_bool),
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,
  member(&_deegenericsameob_tp_hash)),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deegenericsameob_tp_cmp_lo),member(&_deegenericsameob_tp_cmp_le),
  member(&_deegenericsameob_tp_cmp_eq),member(&_deegenericsameob_tp_cmp_ne),
  member(&_deegenericsameob_tp_cmp_gr),member(&_deegenericsameob_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  member(_deeweakref_tp_members),null,
  member(_deeweakref_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deeweakref_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};

DEE_DECL_END

#endif /* !GUARD_DEEMON_WEAKREF_C */
