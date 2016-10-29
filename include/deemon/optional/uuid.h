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
#ifndef GUARD_DEEMON_OPTIONAL_UUID_H
#define GUARD_DEEMON_OPTIONAL_UUID_H 1

#include <deemon/__conf.inl>
#ifdef DEE_LIMITED_API
#include <deemon/optional/string_forward.h>
#endif
#include <deemon/optional/std/string.h>

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif

#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif

struct DeeUUID {
 Dee_uint8_t uuid_a[4];  //{[601F3902]- F2AD - 4854 - 92E8 - BDB4D5F6038E }
 Dee_uint8_t uuid_b[2];  //{ 601F3902 -[F2AD]- 4854 - 92E8 - BDB4D5F6038E }
 Dee_uint8_t uuid_c[2];  //{ 601F3902 - F2AD -[4854]- 92E8 - BDB4D5F6038E }
 Dee_uint8_t uuid_d[2];  //{ 601F3902 - F2AD - 4854 -[92E8]- BDB4D5F6038E }
 Dee_uint8_t uuid_e[6];  //{ 601F3902 - F2AD - 4854 - 92E8 -[BDB4D5F6038E]}
};
#define DeeUUID_INIT()        {{0,0,0,0},{0,0},{0,0},{0,0},{0,0,0,0,0,0}}
#define DeeUUID_EQUAL(a,b)    (memcmp(a,b,sizeof(struct DeeUUID))==0)
#define DeeUUID_SET(ob,right) (*(ob)=*(right))
#define DeeUUID_EMPTY DeeUUID_INIT
#define DeeUUID_IS_EMPTY(ob)  \
(((Dee_uint64_t*)(ob))[0] == 0 &&\
 ((Dee_uint64_t*)(ob))[1] == 0)

#if defined(__INTELLISENSE__)
#define DEE_PRIVATE_UUID_PP_STR(a,b,c,d,e)\
 DEE_PRIVATE_PP_STR({a-b-c-d-e})
#else
#define DEE_PRIVATE_UUID_PP_STR(a,b,c,d,e)\
 "{" #a "-" #b "-" #c "-" #d "-" #e "}"
#endif

#define DEE_PRIVATE_UUID_PP_INIT(a,b,c,d,e)\
{{\
 (DEE_UINT32_C(0x##a)&DEE_UINT32_C(0xFF000000))>>24,\
 (DEE_UINT32_C(0x##a)&DEE_UINT32_C(0x00FF0000))>>16,\
 (DEE_UINT32_C(0x##a)&DEE_UINT32_C(0x0000FF00))>>8,\
 (DEE_UINT32_C(0x##a)&DEE_UINT32_C(0x000000FF))\
},{\
 (DEE_UINT16_C(0x##b)&DEE_UINT16_C(0xFF00))>>8,\
 (DEE_UINT16_C(0x##b)&DEE_UINT16_C(0x00FF))\
},{\
 (DEE_UINT16_C(0x##c)&DEE_UINT16_C(0xFF00))>>8,\
 (DEE_UINT16_C(0x##c)&DEE_UINT16_C(0x00FF))\
},{\
 (DEE_UINT16_C(0x##d)&DEE_UINT16_C(0xFF00))>>8,\
 (DEE_UINT16_C(0x##d)&DEE_UINT16_C(0x00FF))\
},{\
 (DEE_UINT64_C(0x0000##e)&DEE_UINT64_C(0x0000FF0000000000))>>40,\
 (DEE_UINT64_C(0x0000##e)&DEE_UINT64_C(0x000000FF00000000))>>32,\
 (DEE_UINT64_C(0x0000##e)&DEE_UINT64_C(0x00000000FF000000))>>24,\
 (DEE_UINT64_C(0x0000##e)&DEE_UINT64_C(0x0000000000FF0000))>>16,\
 (DEE_UINT64_C(0x0000##e)&DEE_UINT64_C(0x000000000000FF00))>>8,\
 (DEE_UINT64_C(0x0000##e)&DEE_UINT64_C(0x00000000000000FF))\
}}

#define DeeUUID_PP_STR(uuid)      DEE_PRIVATE_UUID_PP_STR uuid
#define DeeUUID_PP_INIT(uuid)     DEE_PRIVATE_UUID_PP_INIT uuid

//////////////////////////////////////////////////////////////////////////
// -- Special UUID values --

#define DEE_UUID_Empty        (00000000,0000,0000,0000,000000000000)

#ifdef DEE_UUID_PROTOTYPES
static DEE_ATTRIBUTE_UNUSED struct DeeUUID DeeUUID_Empty     = DeeUUID_PP_INIT(DEE_UUID_Empty);
#endif

DEE_FUNC_DECL(DEE_A_RET_NOEXCEPT(0) int) DeeUUID_FromString(
 DEE_A_OUT struct DeeUUID *uuid, DEE_A_IN_Z char const *s,
 DEE_A_ON_FAILURE(DEE_A_OUT_OPT) char const **invalid_char) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeUUID_FromStringWithError(
 DEE_A_OUT struct DeeUUID *uuid, DEE_A_IN_Z char const *s) DEE_ATTRIBUTE_NONNULL((1,2));

#define DEE_UUID_TOSTRING_FLAG_NONE    DEE_UINT32_C(0x00000000)
#define DEE_UUID_TOSTRING_FLAG_NOBRACE DEE_UINT32_C(0x00000001) /*< Don't wrap the uuid with braces. */
#define DEE_UUID_TOSTRING_FLAG_LOWER   DEE_UINT32_C(0x00000002) /*< Use lowercase hex characters. */
#define DEE_UUID_TOSTRING_FLAG_DEFAULT DEE_UUID_TOSTRING_FLAG_NONE
#define DeeUUID_ToString(uuid) DeeUUID_ToStringEx(uuid,DEE_UUID_TOSTRING_FLAG_DEFAULT)
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeStringObject) *) DeeUUID_ToStringEx(
 DEE_A_IN struct DeeUUID const *uuid, DEE_A_IN Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Generates a new uuid.
// Returns -1 if the uuid could not be generated
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeUUID_Generate(
 DEE_A_OUT struct DeeUUID *uuid) DEE_ATTRIBUTE_NONNULL((1));

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_UUID_H */
