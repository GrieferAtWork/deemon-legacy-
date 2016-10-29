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
#ifndef GUARD_DEEMON_UUID_C
#define GUARD_DEEMON_UUID_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include <deemon/optional/uuid.h>
#include <deemon/string.h>
#include <deemon/error.h>
#include <deemon/random.h>

DEE_DECL_BEGIN


DEE_STATIC_INLINE(DEE_A_RET_NOEXCEPT(0) int) _dee_sto8(
 DEE_A_IN_R(2) char const *s, DEE_A_OUT Dee_uint8_t *d) {
 char ch;
 ch = s[1];
 if (ch >= '0' && ch <= '9') *d = (Dee_uint8_t)(ch-'0');
 else if (ch >= 'a' && ch <= 'f') *d = (Dee_uint8_t)(10+(ch-'a'));
 else if (ch >= 'A' && ch <= 'F') *d = (Dee_uint8_t)(10+(ch-'A'));
 else return 0;
 ch = s[0];
 if (ch >= '0' && ch <= '9') *d |= (Dee_uint8_t)((ch-'0')<<4);
 else if (ch >= 'a' && ch <= 'f') *d |= (Dee_uint8_t)((10+(ch-'a'))<<4);
 else if (ch >= 'A' && ch <= 'F') *d |= (Dee_uint8_t)((10+(ch-'A'))<<4);
 else return 0;

 return 1;
}
DEE_STATIC_INLINE(void) _dee_8tos(
 DEE_A_OUT_W(2) char *s, DEE_A_IN Dee_uint8_t d, char hex_start) {
 Dee_uint8_t temp;
 temp = (Dee_uint8_t)((d&0x0F));    s[1] = (temp >= 10 ? hex_start+(temp-10) : '0'+temp);
 temp = (Dee_uint8_t)((d&0xF0)>>4); s[0] = (temp >= 10 ? hex_start+(temp-10) : '0'+temp);
}



DEE_A_RET_EXCEPT(-1) int DeeUUID_FromStringWithError(
 DEE_A_OUT struct DeeUUID *uuid, DEE_A_IN_Z char const *s) {
 char const *inv_char;
 if (!DeeUUID_FromString(uuid,s,&inv_char)) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Invalid character '%#.1q' in uuid string %q at pos %Iu",
                      inv_char,s,(Dee_size_t)(inv_char-s));
  return -1;
 }
 return 0;
}

DEE_A_RET_NOEXCEPT(0) int DeeUUID_FromString(
 DEE_A_OUT struct DeeUUID *uuid, DEE_A_IN_Z char const *s,
 DEE_A_ON_FAILURE(DEE_A_OUT_OPT) char const **invalid_char) {
 int has_brace; DEE_ASSERT(uuid && s);
 if (*s == '{') has_brace = 1,++s; else has_brace = 0;
 if DEE_UNLIKELY(!_dee_sto8(s,&uuid->uuid_a[0])) goto err_hex; s += 2;
 if DEE_UNLIKELY(!_dee_sto8(s,&uuid->uuid_a[1])) goto err_hex; s += 2;
 if DEE_UNLIKELY(!_dee_sto8(s,&uuid->uuid_a[2])) goto err_hex; s += 2;
 if DEE_UNLIKELY(!_dee_sto8(s,&uuid->uuid_a[3])) goto err_hex; s += 2;
 if DEE_UNLIKELY(*s != '-') goto err; else ++s;
 if DEE_UNLIKELY(!_dee_sto8(s,&uuid->uuid_b[0])) goto err_hex; s += 2;
 if DEE_UNLIKELY(!_dee_sto8(s,&uuid->uuid_b[1])) goto err_hex; s += 2;
 if DEE_UNLIKELY(*s != '-') goto err; else ++s;
 if DEE_UNLIKELY(!_dee_sto8(s,&uuid->uuid_c[0])) goto err_hex; s += 2;
 if DEE_UNLIKELY(!_dee_sto8(s,&uuid->uuid_c[1])) goto err_hex; s += 2;
 if DEE_UNLIKELY(*s != '-') goto err; else ++s;
 if DEE_UNLIKELY(!_dee_sto8(s,&uuid->uuid_d[0])) goto err_hex; s += 2;
 if DEE_UNLIKELY(!_dee_sto8(s,&uuid->uuid_d[1])) goto err_hex; s += 2;
 if DEE_UNLIKELY(*s != '-') goto err; else ++s;
 if DEE_UNLIKELY(!_dee_sto8(s,&uuid->uuid_e[0])) goto err_hex; s += 2;
 if DEE_UNLIKELY(!_dee_sto8(s,&uuid->uuid_e[1])) goto err_hex; s += 2;
 if DEE_UNLIKELY(!_dee_sto8(s,&uuid->uuid_e[2])) goto err_hex; s += 2;
 if DEE_UNLIKELY(!_dee_sto8(s,&uuid->uuid_e[3])) goto err_hex; s += 2;
 if DEE_UNLIKELY(!_dee_sto8(s,&uuid->uuid_e[4])) goto err_hex; s += 2;
 if DEE_UNLIKELY(!_dee_sto8(s,&uuid->uuid_e[5])) goto err_hex; s += 2;
 if (has_brace) { if DEE_UNLIKELY(*s != '}') goto err; ++s; }
 if DEE_UNLIKELY(*s != '\0') goto err;
 return 1;
err_hex:
 if ((*s >= '0' && *s <= '9') ||
     (*s >= 'a' && *s <= 'f') ||
     (*s >= 'A' && *s <= 'F')
     ) ++s; // The next character must be wrong
err:
 if (invalid_char) *invalid_char = s;
 return 0;
}

DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeStringObject) *DeeUUID_ToStringEx(
 DEE_A_IN struct DeeUUID const *uuid, DEE_A_IN Dee_uint32_t flags) {
 DeeObject *result; char *dst;
 char hex_start;
 DEE_ASSERT(uuid);
 if DEE_UNLIKELY((result = DeeString_NewSized((Dee_size_t)((
  flags&DEE_UUID_TOSTRING_FLAG_NOBRACE)!=0 ? 36 : 38))) == NULL) return NULL;
 hex_start = (flags&DEE_UUID_TOSTRING_FLAG_LOWER)!=0 ? 'a' : 'A';
 dst = DeeString_STR(result);
 if ((flags&DEE_UUID_TOSTRING_FLAG_NOBRACE)==0) *dst++ = '{';
 _dee_8tos(dst,uuid->uuid_a[0],hex_start); dst += 2;
 _dee_8tos(dst,uuid->uuid_a[1],hex_start); dst += 2;
 _dee_8tos(dst,uuid->uuid_a[2],hex_start); dst += 2;
 _dee_8tos(dst,uuid->uuid_a[3],hex_start); dst += 2;
 *dst++ = '-';
 _dee_8tos(dst,uuid->uuid_b[0],hex_start); dst += 2;
 _dee_8tos(dst,uuid->uuid_b[1],hex_start); dst += 2;
 *dst++ = '-';
 _dee_8tos(dst,uuid->uuid_c[0],hex_start); dst += 2;
 _dee_8tos(dst,uuid->uuid_c[1],hex_start); dst += 2;
 *dst++ = '-';
 _dee_8tos(dst,uuid->uuid_d[0],hex_start); dst += 2;
 _dee_8tos(dst,uuid->uuid_d[1],hex_start); dst += 2;
 *dst++ = '-';
 _dee_8tos(dst,uuid->uuid_e[0],hex_start); dst += 2;
 _dee_8tos(dst,uuid->uuid_e[1],hex_start); dst += 2;
 _dee_8tos(dst,uuid->uuid_e[2],hex_start); dst += 2;
 _dee_8tos(dst,uuid->uuid_e[3],hex_start); dst += 2;
 _dee_8tos(dst,uuid->uuid_e[4],hex_start); dst += 2;
 _dee_8tos(dst,uuid->uuid_e[5],hex_start); dst += 2;
 if ((flags&DEE_UUID_TOSTRING_FLAG_NOBRACE)==0) *dst = '}';
 return result;
}

DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int DeeUUID_Generate(
 DEE_A_OUT struct DeeUUID *uuid) {
 struct DeeRandomNumberGenerator rng;
 DEE_ASSERT(uuid);
 // TODO: Make sure enough entropy was available
 DeeRandomNumberGenerator_Randomize(&rng);
 DeeRandomNumberGenerator_NextData(&rng,uuid,sizeof(struct DeeUUID));
 return 0;
}



DEE_DECL_END

#endif /* !GUARD_DEEMON_UUID_C */
