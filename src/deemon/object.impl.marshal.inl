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
#ifdef __INTELLISENSE__
#define DEE_LIMITED_API 1
#endif

// /include/*
#include <deemon/__conf.inl>
#include <deemon/optional/type_decl.h>
#include <deemon/type.h>
#include <deemon/error.h>
#include <deemon/marshal.h>

// /src/*
#include <deemon/marshal_data.h>

// */ (nano...)

DEE_DECL_BEGIN

static int DEE_CALL _deeobject_tp_marshal_ctor(
 DeeTypeObject *tp_self, DeeObject *DEE_UNUSED(self),
 DeeObject *DEE_UNUSED(infile), struct DeeMarshalReadMap *DEE_UNUSED(map)) {
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__readm__",
                     DeeType_NAME(tp_self));
 return -1;
}
static int DEE_CALL _deeobject_tp_marshal_put(
 DEE_A_INOUT DeeTypeObject *tp_self, DeeObject *DEE_UNUSED(self),
 DeeObject *DEE_UNUSED(outfile), struct DeeMarshalWriteMap *DEE_UNUSED(map)) {
 DeeError_SetStringf(&DeeErrorType_NotImplemented,
                     "Not implemented: %s.__writem__",
                     DeeType_NAME(tp_self));
 return -1;
}

static struct DeeTypeMarshal _deeobject_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(object)),
 member(&_deeobject_tp_marshal_ctor),member(&_deeobject_tp_marshal_put));


DEE_DECL_END

