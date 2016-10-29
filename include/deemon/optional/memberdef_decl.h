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
#ifndef GUARD_DEEMON_OPTIONAL_MEMBERDEF_DECL_H
#define GUARD_DEEMON_OPTIONAL_MEMBERDEF_DECL_H 1

#include <deemon/__conf.inl>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
#undef DEE_PRIVATE_DECL_DEE_MEMBERFUNCTION
#endif
#ifdef DEE_PRIVATE_DECL_DEE_STRUCTURED_MEMBERFUNCTION
DEE_PRIVATE_DECL_DEE_STRUCTURED_MEMBERFUNCTION
#undef DEE_PRIVATE_DECL_DEE_STRUCTURED_MEMBERFUNCTION
#endif


struct DeeObject;
struct DeeStructuredTypeObject;

//////////////////////////////////////////////////////////////////////////
// Type member / getset / method definitions
typedef Dee_uint32_t DeeMemberType;
typedef Dee_uint32_t DeeMemberAccess;
struct DeeMemberDef {
 char const     *d_name;   /*< [1..1] (NULL indicates end of list). */
 char const     *d_doc;    /*< [0..1] Doc. */
 DeeMemberType   d_type;   /*< Member type (see above). */
 DeeMemberAccess d_access; /*< Allowed access to member type. */
 Dee_size_t      d_offset; /*< Offset from DeeObject base address. */
};
typedef DEE_A_RET_EXCEPT_REF struct DeeObject *(DEE_CALL *_DeeGetSetDef_Getter)(DEE_A_INOUT struct DeeObject *self, void *closure);
typedef DEE_A_RET_EXCEPT(-1) int               (DEE_CALL *_DeeGetSetDef_Delter)(DEE_A_INOUT struct DeeObject *self, void *closure);
typedef DEE_A_RET_EXCEPT(-1) int               (DEE_CALL *_DeeGetSetDef_Setter)(DEE_A_INOUT struct DeeObject *self, DEE_A_INOUT struct DeeObject *v, void *closure);
typedef DEE_A_RET_EXCEPT_REF struct DeeObject *(DEE_CALL *_DeeGetSetDef_StructuredGetter)(DEE_A_INOUT struct DeeStructuredTypeObject *tp_self, DEE_A_INOUT void *self, void *closure);
typedef DEE_A_RET_EXCEPT(-1) int               (DEE_CALL *_DeeGetSetDef_StructuredDelter)(DEE_A_INOUT struct DeeStructuredTypeObject *tp_self, DEE_A_INOUT void *self, void *closure);
typedef DEE_A_RET_EXCEPT(-1) int               (DEE_CALL *_DeeGetSetDef_StructuredSetter)(DEE_A_INOUT struct DeeStructuredTypeObject *tp_self, DEE_A_INOUT void *self, DEE_A_INOUT struct DeeObject *v, void *closure);
struct DeeGetSetDef {
 char const          *d_name;    /*< [1..1] (NULL indicates end of list) */
 char const          *d_doc;     /*< [0..1] Doc. */
 void                *d_closure; /*< [0..1] Closure for functions. */
#define DEE_GETSETDEF_FLAG_NONE   DEE_UINT32_C(0x00000000)
#define DEE_GETSETDEF_FLAG_CONST  DEE_UINT32_C(0x00000001) /*< Allow the use of this getset in constant expressions. */
#define DEE_GETSETDEF_FLAG_STRUCT DEE_UINT32_C(0x00000002) /*< Structured getset (Turns the below functions into the structured variations). */
 Dee_uint32_t         d_flags;
 _DeeGetSetDef_Getter d_get;     /*< [0..1] Get function (NULL if can't read). */
 _DeeGetSetDef_Delter d_del;     /*< [0..1] Del function (NULL if can't delete). */
 _DeeGetSetDef_Setter d_set;     /*< [0..1] Set function (NULL if can't write). */
};
#define DeeGetSetDef_OGET(ob) (ob)->d_get
#define DeeGetSetDef_ODEL(ob) (ob)->d_del
#define DeeGetSetDef_OSET(ob) (ob)->d_set
#define DeeGetSetDef_SGET(ob) ((_DeeGetSetDef_StructuredGetter)(ob)->d_get)
#define DeeGetSetDef_SDEL(ob) ((_DeeGetSetDef_StructuredDelter)(ob)->d_del)
#define DeeGetSetDef_SSET(ob) ((_DeeGetSetDef_StructuredSetter)(ob)->d_set)

struct DeeMethodDef {
 char const       *d_name;    /*< [1..1] (NULL indicates end of list). */
 char const       *d_doc;     /*< [0..1] Doc. */
 void             *d_closure; /*< [0..1] Closure for the  function. */
#define DEE_METHODDEF_FLAG_NONE   DEE_UINT32_C(0x00000000)
#define DEE_METHODDEF_FLAG_CONST  DEE_UINT32_C(0x00000001) /*< Don't Allow the use of this method in constant expressions. */
#define DEE_METHODDEF_FLAG_STRUCT DEE_UINT32_C(0x00000002) /*< Structured method (Turns d_func into a 'DeeStructuredMemberFunction'). */
 Dee_uint32_t      d_flags;
 DeeMemberFunction d_func;  /*< [1..1] Function (NOTE: May be a DeeStructuredMemberFunction). */
};
#define DeeMethodDef_OFUNC(ob) (ob)->d_func
#define DeeMethodDef_SFUNC(ob) ((DeeStructuredMemberFunction)(ob)->d_func)

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_MEMBERDEF_DECL_H */
