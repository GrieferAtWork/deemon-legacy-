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
#ifndef GUARD_DEEMON_DEX_MICAPI_C
#define GUARD_DEEMON_DEX_MICAPI_C 1
#define DEE_EXTENSION 1

//@ dex.name = _micapi

#include "_micapi.h"
#include <deemon/none.h>
#include <deemon/mp/thread.h>
#include <deemon/error.h>
#include <deemon/tuple.h>
#include <deemon/type.h>
#include <deemon/memberdef.h>
#include <deemon/cfunction.h>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN


#ifdef DEE_PLATFORM_WINDOWS
static DWORD WINAPI DeeMic_ThreadMain(DeeMicObject *self) {
 MSG msg;
 while (1) {
  if (GetMessage(&msg,0,0,0) != 1) break;
  switch (msg.message) {
   case MM_WIM_DATA: {
    WAVEHDR *data = (WAVEHDR *)msg.lParam;
    if (data && data->dwBytesRecorded) {
     if (DeeIOBuffer_Write(&self->m_buffer,data->lpData,data->dwBytesRecorded) != 0) return 1;
    }
    self->m_header.lpData = (LPSTR)self->m_wbuf;
    self->m_header.dwBufferLength = sizeof(self->m_wbuf);
    self->m_header.dwBytesRecorded = 0;
    self->m_header.dwUser = 0L;
    self->m_header.dwFlags = 0L;
    self->m_header.dwLoops = 0L;
    waveInPrepareHeader(self->m_hwave,&self->m_header,sizeof(WAVEHDR));
    waveInAddBuffer(self->m_hwave,&self->m_header,sizeof(WAVEHDR));
   } break;
   case MM_WIM_CLOSE: return 0;
   default: break;
  }
 }
 return 0;
}


DEE_A_RET_EXCEPT(-1) int DeeMic_Init(
 DeeMicObject *self, unsigned int channels,
 unsigned int samples_per_sec, unsigned int bits_per_sample) {
 WAVEFORMATEX	waveFormat; MMRESULT error;
 _DeeFile_Init(self);
 if (DeeIOBuffer_InitEx(&self->m_buffer,0,DEE_IOBUFFER_FLAG_NOEXCEPT_WRITE) != 0) return -1;
 self->m_hthread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)&DeeMic_ThreadMain,
                                self,CREATE_SUSPENDED,&self->m_threadid);
 if (!self->m_hthread) {
  DeeError_SystemError("CreateThread");
err_buffer:
  DeeIOBuffer_Quit(&self->m_buffer);
  return -1;
 }
 waveFormat.wFormatTag = WAVE_FORMAT_PCM;
 waveFormat.nChannels = (WORD)channels;
 waveFormat.nSamplesPerSec = (DWORD)samples_per_sec;
 waveFormat.wBitsPerSample = (WORD)bits_per_sample;
 waveFormat.nBlockAlign = (WORD)(channels*(bits_per_sample/8));
 waveFormat.nAvgBytesPerSec = samples_per_sec*waveFormat.nBlockAlign;
 waveFormat.cbSize = 0;
 error = waveInOpen(&self->m_hwave,WAVE_MAPPER,&waveFormat,self->m_threadid,0,CALLBACK_THREAD);
 if (error) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "waveInOpen() : %#.8x : %K",
                      error,DeeSystemError_ToString((Dee_errno_t)error));
err_thread_stop:
  if (!CloseHandle(self->m_hthread)) SetLastError(0);
  goto err_buffer;
 }

 error = mixerOpen(&self->m_hmixer,(DWORD)self->m_hwave,0,0,MIXER_OBJECTF_HWAVEIN);
 if (error) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "mixerOpen(%lu) : %#.8x : No Mixer support : %K",
                      (DWORD)self->m_hwave,
                      error,DeeSystemError_ToString((Dee_errno_t)error));
err_wave_close:
  if (waveInClose(self->m_hwave)) SetLastError(0);
  goto err_thread_stop;
 }

 // Setup and prepare the header
 self->m_header.lpData = (LPSTR)self->m_wbuf;
 self->m_header.dwBufferLength = sizeof(self->m_wbuf);
 self->m_header.dwBytesRecorded = 0;
 self->m_header.dwUser = 0L;
 self->m_header.dwFlags = 0L;
 self->m_header.dwLoops = 0L;
 error = waveInPrepareHeader(self->m_hwave,&self->m_header,sizeof(WAVEHDR));
 if (error) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "waveInPrepareHeader() : %#.8x : %K",
                      error,DeeSystemError_ToString((Dee_errno_t)error));
  goto err_wave_close;
 }
 error = waveInAddBuffer(self->m_hwave,&self->m_header,sizeof(WAVEHDR));
 if (error) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "waveInAddBuffer() : %#.8x : %K",
                      error,DeeSystemError_ToString((Dee_errno_t)error));
  goto err_wave_close;
 }
 if (!ResumeThread(self->m_hthread)) {
  DeeError_SystemError("ResumeThread");
  goto err_wave_close;
 }
 return 0;
}

DeeError_NEW_STATIC(_deemic_closed_error,&DeeErrorType_ValueError,"Mic already closed");
DEE_A_RET_EXCEPT(-1) int DeeMic_BeginRecord(DEE_A_INOUT DeeMicObject *self) {
 MMRESULT error;
 DEE_ASSERT(DeeObject_Check(self) && DeeMic_Check(self));
 DeeFile_ACQUIRE(self);
 if (!self->m_hwave) { DeeFile_RELEASE(self); DeeError_Throw((DeeObject *)&_deemic_closed_error); return -1; }
 error = waveInStart(self->m_hwave);
 DeeFile_RELEASE(self);
 if (error) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "waveInStart(%lu) : %#.8x : %K",(DWORD)self->m_hwave,
                      error,DeeSystemError_ToString((Dee_errno_t)error));
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeMic_EndRecord(DEE_A_INOUT DeeMicObject *self) {
 MMRESULT error;
 DEE_ASSERT(DeeObject_Check(self) && DeeMic_Check(self));
 DeeFile_ACQUIRE(self);
 if (!self->m_hwave) { DeeFile_RELEASE(self); DeeError_Throw((DeeObject *)&_deemic_closed_error); return -1; }
 error = waveInStop(self->m_hwave);
 DeeFile_RELEASE(self);
 if (error) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "waveInStop(%lu) : %#.8x : %K",(DWORD)self->m_hwave,
                      error,DeeSystemError_ToString((Dee_errno_t)error));
  return -1;
 }
 return 0;
}


static void _deemic_tp_io_close(DeeMicObject *self) {
 HANDLE old_thread; DWORD old_threadid;
 HWAVEIN old_hwave; HMIXER old_hmixer;
 // Close the IO Buffer
 DeeIOBuffer_Close(&self->m_buffer);
 DeeFile_ACQUIRE(self);
 old_thread = self->m_hthread;
 old_threadid = self->m_threadid;
 self->m_hthread = 0;
 self->m_threadid = 0;
 old_hwave = self->m_hwave;
 old_hmixer = self->m_hmixer;
 self->m_hwave = 0;
 self->m_hmixer = 0;
 DeeFile_RELEASE(self);
 if (old_hmixer && !mixerClose(old_hmixer)) SetLastError(0);
 if (old_hwave && !waveInClose(old_hwave)) SetLastError(0);
 if (old_thread) {
post_again:
  if (!PostThreadMessage(old_threadid,MM_WIM_CLOSE,0,0)) {
   DWORD err = DeeSystemError_Win32Consume();
   if (err == ERROR_INVALID_THREAD_ID) { Sleep(1); goto post_again; }
   DEE_LDEBUG("Failed to stop mic thread: %K\n",
              DeeSystemError_Win32ToString(err));
   SetLastError(0);
  } else WaitForSingleObject(old_thread,INFINITE);
  if (!CloseHandle(old_thread)) SetLastError(0);
 }
}
static void _deemic_tp_dtor(DeeMicObject *self) {
 _deemic_tp_io_close(self);
 DeeIOBuffer_Quit(&self->m_buffer);
}
static int _deemic_tp_io_read(
 DeeMicObject *self, void *p, Dee_size_t s, Dee_size_t *rs) {
 // Read from the buffer
 return DeeIOBuffer_Read(&self->m_buffer,p,s,rs);
}
static int _deemic_tp_io_seek(
 DeeMicObject *self, Dee_int64_t off, int whence, Dee_uint64_t *pos) {
 // Seek inside the buffer
 return DeeIOBuffer_Seek(&self->m_buffer,off,whence,pos);
}

static DeeObject *_deemic_begin_record(
 DeeMicObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":begin_record") != 0) return NULL;
 if (DeeMic_BeginRecord(self) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *_deemic_end_record(
 DeeMicObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if (DeeTuple_Unpack(args,":end_record") != 0) return NULL;
 if (DeeMic_EndRecord(self) != 0) return NULL;
 DeeReturn_None;
}

static struct DeeMethodDef _deemic_tp_methods[] = {
 DEE_METHODDEF_v100("begin_record",member(&_deemic_begin_record),"() -> none\n"
  "@throws Error.SystemError: Failed to start recording\n"
  "\tStarts recording raw audio data, that can then be read from @this using regular file read operations, such as #(file.read).\n"
  "\tWARNING: Don't forget to turn off recording once you're done using #(mic.end_record), as the internal file buffer will otherwise grow to infinite size, eventually taking up all available memory.\n"
  "\tNote that once data is read from @this file, that memory can be re-used for storing more raw audio data, meaning that reading "
    "data automatically releases that memory, in consequence allowing you to keep the mic on, as long as you keep reading from this file."),
 DEE_METHODDEF_v100("end_record",member(&_deemic_end_record),"() -> none\n"
  "@throws Error.SystemError: Failed to stop recording\n"
  "\tStops recording audio after a previous call to #(mic.begin_record)."),
 DEE_METHODDEF_END_v100
};


static int _deemic_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeMicObject *self, DeeObject *args) {
 unsigned int channels = DEE_MIC_DEFAULT_CHANNELS,
  samples_per_sec = DEE_MIC_DEFAULT_SAMPLES_PER_SEC,
  bits_per_sample = DEE_MIC_DEFAULT_BITS_PER_SAMPLE;
 if (DeeTuple_Unpack(args,"|uuu:mic",&channels,&samples_per_sec,&bits_per_sample) != 0) return -1;
 return DeeMic_Init(self,channels,samples_per_sec,bits_per_sample);
}


DeeFileTypeObject DeeMic_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(member(&DeeFileType_Type),
   member("mic"),member("("
   "unsigned int channels = " DEE_PP_STR(DEE_MIC_DEFAULT_CHANNELS) ", "
   "unsigned int samples_per_sec = " DEE_PP_STR(DEE_MIC_DEFAULT_SAMPLES_PER_SEC) ", "
   "unsigned int bits_per_sample = " DEE_PP_STR(DEE_MIC_DEFAULT_BITS_PER_SAMPLE) ") -> mic\n"
   "@throws Error.SystemError: Couldn't create microphone #file\n"
   "\tCreates a new microphone #(file).\n"
   "\tNOTE: The read operation of @this #file object blocks until data is available.\n"),null,
   member((DeeTypeObject *)&DeeFile_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeMicObject),null,
   null,null,null,member(&_deemic_tp_any_ctor)),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,
   member(&_deemic_tp_dtor)),
  DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
  DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
  DEE_TYPE_OBJECT_OBJECT_v100(null,null),
  DEE_TYPE_OBJECT_MATH_v101(
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null,null,
   null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
  DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
  DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
   null,null,member(_deemic_tp_methods),null,null,null),
  DEE_TYPE_OBJECT_FOOTER_v100
 },
 DEE_FILE_TYPE_OBJECT_IO_v100(
  member(&_deemic_tp_io_read),null,
  member(&_deemic_tp_io_seek),null,null,
  member(&_deemic_tp_io_close))
 DEE_FILE_TYPE_OBJECT_FOOTER_v100
};
#endif

//////////////////////////////////////////////////////////////////////////
// Module exports
struct DeeDexExportDef DeeDex_Exports[] = {
#ifdef DEE_PLATFORM_WINDOWS
 DeeDex_EXPORT_OBJECT("mic",(DeeObject *)&DeeMic_Type),
#endif
 DeeDex_EXPORT_END
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_MICAPI_C */
