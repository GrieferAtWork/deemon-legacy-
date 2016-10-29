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
#ifndef GUARD_DEEMON_DEX_MICAPI_H
#define GUARD_DEEMON_DEX_MICAPI_H 1
 
#include <deemon/dex.h>
#include <deemon/file.h>
#include <deemon/mp/thread.h>
#include <deemon/optional/io_buffer.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
#ifdef DEE_PLATFORM_WINDOWS
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
#include <Mmsystem.h>
DEE_COMPILER_MSVC_WARNING_POP
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()

DEE_DECL_BEGIN

#ifdef DEE_PLATFORM_WINDOWS
DEE_OBJECT_DEF(DeeMicObject);

#define DEE_MIC_WAVE_BUFSIZE 128
struct DeeMicObject {
 DEE_FILE_OBJECT_HEAD
 struct DeeIOBuffer m_buffer; /*< r/w buffer for mic data. */
 HWAVEIN            m_hwave;
 HMIXER             m_hmixer;
 WAVEHDR            m_header;
 HANDLE             m_hthread;
 DWORD              m_threadid;
 char               m_wbuf[DEE_MIC_WAVE_BUFSIZE];
};


#define DEE_MIC_DEFAULT_CHANNELS        1
#define DEE_MIC_DEFAULT_SAMPLES_PER_SEC 44100
#define DEE_MIC_DEFAULT_BITS_PER_SAMPLE 16

extern DeeFileTypeObject DeeMic_Type;
#define DeeMic_Check(ob)      DeeObject_InstanceOf(ob,(DeeTypeObject *)&DeeMic_Type)
#define DeeMic_CheckExact(ob) DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeMic_Type)

extern DEE_A_RET_EXCEPT(-1) int DeeMic_BeginRecord(DEE_A_INOUT DeeMicObject *self);
extern DEE_A_RET_EXCEPT(-1) int DeeMic_EndRecord(DEE_A_INOUT DeeMicObject *self);
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_MICAPI_H */
