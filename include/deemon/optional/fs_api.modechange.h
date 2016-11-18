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
#ifndef GUARD_DEEMON_OPTIONAL_FS_API_MODE_CHANGE_H
#define GUARD_DEEMON_OPTIONAL_FS_API_MODE_CHANGE_H 1

#include <deemon/__conf.inl>

DEE_DECL_BEGIN

typedef DEE_TYPES_UINT16_T Dee_modechange_t;
#define DEE_MODECHANGE(bits,kind) \
 (Dee_modechange_t)(((bits)&0777)|(((kind)&07)<<9))
#define DEE_MODECHANGE_BITS(mc)     ((mc)&00777)     /*< Mask of affected permissions/groups. */
#define DEE_MODECHANGE_KIND(mc)    (((mc)&07000)>>9) /*< The kind of modification operation. */
#define DEE_MODECHANGE_KIND_NONE   0 /*< No operation. */
#define DEE_MODECHANGE_KIND_SET    1 /*< Set new permission bits (e.g.: 'chmod =r'). */
#define DEE_MODECHANGE_KIND_ADD    2 /*< Add new permission bits, keeping bits previously enabled. (e.g.: 'chmod +r'). */
#define DEE_MODECHANGE_KIND_DEL    3 /*< Delete existing permission bits, keeping bits previously disabled. (e.g.: 'chmod -r'). */
#define DEE_MODECHANGE_KIND_TOGGLE 4 /*< Toggle existing permission bits. */

//////////////////////////////////////////////////////////////////////////
// Parses a unix-style modechange string
DEE_FUNC_DECL(DEE_A_EXEC DEE_A_RET_EXCEPT(-1) int) DeeFS_ParseModeChange(
 DEE_A_IN_Z char const *mode, DEE_A_OUT Dee_modechange_t *result);

#ifdef DEE_LIMITED_API
#ifdef DEE_PRIVATE_DECL_DEE_MODE_T
DEE_PRIVATE_DECL_DEE_MODE_T
#undef DEE_PRIVATE_DECL_DEE_MODE_T
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif

extern void _DeeFS_ExecModeChange(
 DEE_A_INOUT Dee_mode_t *mode, DEE_A_IN Dee_size_t mcc,
 DEE_A_IN_R(mmc) Dee_modechange_t const *mcv);
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_FS_API_MODE_CHANGE_H */
