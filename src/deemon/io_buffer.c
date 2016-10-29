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
#ifndef GUARD_DEEMON_IO_BUFFER_C
#define GUARD_DEEMON_IO_BUFFER_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/deemonrun.h>
#include <deemon/mp/thread.h>
#include <deemon/optional/io_buffer.h>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
#ifdef DEE_PLATFORM_WINDOWS
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#else
#if DEE_ENVIRONMENT_HAVE_INCLUDE_ERRNO_H
#include <errno.h>
#endif
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(NULL) struct DeeIOBuffer *DeeIOBuffer_NewEx(
DEE_A_IN Dee_size_t size_hint, DEE_A_IN Dee_uint32_t flags) {
 struct DeeIOBuffer *result;
 while ((result = (struct DeeIOBuffer *)malloc_nz(
  sizeof(struct DeeIOBuffer))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
 if (DeeIOBuffer_InitEx(result,size_hint,flags) != 0) {
  free_nn(result);
  return NULL;
 }
 return result;
}
void DeeIOBuffer_Delete(DEE_A_IN struct DeeIOBuffer *self) {
 DEE_ASSERT(self);
 DeeIOBuffer_Quit(self);
 free_nn(self);
}



DEE_A_RET_EXCEPT(-1) int DeeIOBuffer_InitEx(
 DEE_A_OUT struct DeeIOBuffer *self,
 DEE_A_IN Dee_size_t size_hint, DEE_A_IN Dee_uint32_t flags) {
 DEE_ASSERT(self);
 // v Initialize the semaphore to 0, as no data is available by default
 if (DeeNativeSemaphore_Init(&self->iob_sem,0) != 0) return -1;
 DeeAtomicMutex_Init(&self->iob_lock);
 self->iob_rpos = NULL;
 self->iob_wpos = NULL;
 if (size_hint) {
  // Allocate an initial buffer according to the given hint
  while ((self->iob_buf = (Dee_uint8_t *)malloc_nz(size_hint)) == NULL) {
   if DEE_LIKELY(Dee_CollectMemory()) continue;
   goto no_buffer; // Silently ignore the failed allocation
  }
  self->iob_size = size_hint;
  self->iob_rpos = self->iob_buf;
  self->iob_wpos = self->iob_buf;
 } else {
no_buffer:
  self->iob_buf = NULL;
  self->iob_size = 0;
 }
 self->iob_uses = 0;
 self->iob_flags = flags&0x0FFFFFFF;
 return 0;
}
void DeeIOBuffer_Quit(DEE_A_IN struct DeeIOBuffer *self) {
 DEE_ASSERT(self);
 DeeIOBuffer_Close(self);
 free(self->iob_buf);
 // v destroyed during close
 //DeeNativeSemaphore_Quit(&self->iob_sem);
}
void DeeIOBuffer_Close(DEE_A_INOUT struct DeeIOBuffer *self) {
 DEE_ASSERT(self);
 DeeAtomicMutex_AcquireRelaxed(&self->iob_lock);
 if ((self->iob_flags&DEE_IOBUFFER_FLAG_CLOSED)==0) {
  self->iob_flags |= DEE_IOBUFFER_FLAG_CLOSED;
  while (self->iob_uses) {
   DeeAtomicMutex_Release(&self->iob_lock);
   DeeNativeSemaphore_ReleaseNoexcept(&self->iob_sem);
   DeeThread_SleepNoInterrupt(1);
   DeeAtomicMutex_AcquireRelaxed(&self->iob_lock);
  }
  DeeNativeSemaphore_Quit(&self->iob_sem);
 }
 DeeAtomicMutex_Release(&self->iob_lock);
}


DEE_A_RET_EXCEPT(-1) int DeeIOBuffer_Read(
 DEE_A_INOUT struct DeeIOBuffer *self, DEE_A_OUT_WB(*rs) void *p,
 DEE_A_IN Dee_size_t s, DEE_A_OUT Dee_size_t *rs) {
 Dee_size_t max_read,do_read,max_read_before;
 Dee_uint8_t *bufend;
 DEE_ASSERT(self); DEE_ASSERT(rs);
 if (!s) { *rs = 0; return 0; }
 DEE_ASSERT(p);
wait_for_data:
 DeeAtomicMutex_AcquireRelaxed(&self->iob_lock);
 if ((self->iob_flags&DEE_IOBUFFER_FLAG_CLOSED)!=0) goto unlock_after_closed;
 ++self->iob_uses;
 DeeAtomicMutex_Release(&self->iob_lock);
 if (DeeNativeSemaphore_Acquire(&self->iob_sem) != 0) return -1;
 DeeAtomicMutex_AcquireRelaxed(&self->iob_lock);
 DEE_ASSERT(self->iob_rpos >= self->iob_buf && (!self->iob_size || self->iob_rpos < DeeIOBuffer_BUFEND(self)));
 DEE_ASSERT(self->iob_wpos >= self->iob_buf && (!self->iob_size || self->iob_wpos < DeeIOBuffer_BUFEND(self)));
 if ((self->iob_flags&DEE_IOBUFFER_FLAG_CLOSED)!=0) {
unlock_after_closed:
  DeeAtomicMutex_Release(&self->iob_lock);
  *rs = 0; // Already closed
  return 0;
 }
 max_read = DeeIOBuffer_MAX_READ(self);
 if (!max_read) { DeeAtomicMutex_Release(&self->iob_lock); goto wait_for_data; }
 do_read = s < max_read ? s : max_read;
 bufend = DeeIOBuffer_BUFEND(self);
 max_read_before = (Dee_size_t)(bufend-self->iob_rpos);
 if (max_read_before >= do_read) {
  // Can read everything without wrapping (or only have to wrap afterwards)
  memcpy(p,self->iob_rpos,do_read);
  if (max_read_before == do_read)
   self->iob_rpos = self->iob_buf;
  else self->iob_rpos += do_read;
 } else {
  // We'll have to wrap during the read
  memcpy(p,self->iob_rpos,max_read_before);
  memcpy((void *)((Dee_uintptr_t)p+max_read_before),
         self->iob_buf,do_read-max_read_before);
  self->iob_rpos = self->iob_buf+(do_read-max_read_before);
 }
 if (max_read > do_read) {
  DeeAtomicMutex_Release(&self->iob_lock);
  // More data is available, so signal the semaphore again
  if (DeeNativeSemaphore_Release(&self->iob_sem) != 0) {
   DeeAtomicMutex_AcquireRelaxed(&self->iob_lock);
   --self->iob_uses;
   DeeAtomicMutex_Release(&self->iob_lock);
   return -1;
  }
  DeeAtomicMutex_AcquireRelaxed(&self->iob_lock);
 }
 --self->iob_uses; // We're done using this
 DeeAtomicMutex_Release(&self->iob_lock);
 *rs = do_read;
 return 0;
}


//////////////////////////////////////////////////////////////////////////
// Write data into the buffer
// - Wakes the first thread that was waiting for data
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeIOBuffer_Write(
 DEE_A_INOUT struct DeeIOBuffer *self,
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s) {
 Dee_size_t max_write,max_write_before;
 Dee_uint8_t *bufend; int result;
 DEE_ASSERT(self);
 if (!s) return 0;
 DEE_ASSERT(p);
startover:
 DeeAtomicMutex_AcquireRelaxed(&self->iob_lock);
 if ((self->iob_flags&DEE_IOBUFFER_FLAG_CLOSED)!=0) {
  // Buffer was closed: Throw an error
  DeeAtomicMutex_Release(&self->iob_lock);
  if ((self->iob_flags&DEE_IOBUFFER_FLAG_NOEXCEPT_WRITE)==0)
   DeeError_SET_STRING(&DeeErrorType_IOError,"Can't write into closed io_buffer");
  return -1;
 }
 DEE_ASSERT(self->iob_rpos >= self->iob_buf && (!self->iob_size || self->iob_rpos < DeeIOBuffer_BUFEND(self)));
 DEE_ASSERT(self->iob_wpos >= self->iob_buf && (!self->iob_size || self->iob_wpos < DeeIOBuffer_BUFEND(self)));
 max_write = DeeIOBuffer_MAX_WRITE(self);
 bufend = DeeIOBuffer_BUFEND(self);
 if (max_write < s) {
  Dee_size_t read_pos,write_pos,min_new_buffer_size,new_buffer_size;
  Dee_uint8_t *new_buffer;
  // Difficult case: We have to reallocate the buffer to be large enough
  read_pos = (Dee_size_t)(self->iob_rpos-self->iob_buf);
  write_pos = (Dee_size_t)(self->iob_wpos-self->iob_buf);
  min_new_buffer_size = self->iob_size+(s-max_write);
  if ((self->iob_flags&DEE_IOBUFFER_FLAG_MINBUF)!=0) {
   // Use the smallest possible size
   new_buffer_size = min_new_buffer_size;
  } else {
   // Round up to the nearest 2**n number
   new_buffer_size = 1;
   while (new_buffer_size < min_new_buffer_size)
    new_buffer_size = (new_buffer_size<<1)|0x1;
   ++new_buffer_size;
  }
  if ((new_buffer = (Dee_uint8_t *)realloc_nz(
   self->iob_buf,new_buffer_size)) == NULL) {
   DeeAtomicMutex_Release(&self->iob_lock);
   // Must collect memory while not locking the buffer!
   if DEE_LIKELY(Dee_CollectMemory()) goto startover;
   DeeError_NoMemory(); // out-of-memory
   return -1;
  }
  self->iob_buf = new_buffer;
  self->iob_size = new_buffer_size;
  self->iob_rpos = new_buffer+read_pos;
  self->iob_wpos = new_buffer+write_pos;
 }
 // Re-use the existing buffer
 max_write_before = (Dee_size_t)(bufend-self->iob_wpos);
 if (s <= max_write_before) {
  // Can write everything without wrapping (or only have to wrap afterwards)
  memcpy(self->iob_wpos,p,s);
  if (max_write_before == s)
   self->iob_wpos = self->iob_buf;
  else self->iob_wpos += s;
 } else {
  // We'll have to wrap during the write
  memcpy(self->iob_rpos,p,max_write_before);
  memcpy(self->iob_buf,(void *)((Dee_uintptr_t)p+max_write_before),s-max_write_before);
  self->iob_rpos = self->iob_buf+(s-max_write_before);
 }

 ++self->iob_uses;
 DeeAtomicMutex_Release(&self->iob_lock);
 // Signal other threads that data is available
 if ((self->iob_flags&DEE_IOBUFFER_FLAG_NOEXCEPT_WRITE)!=0) {
#ifdef DEE_PLATFORM_WINDOWS
  if (!ReleaseSemaphore((HANDLE)self->iob_sem.s_handle,(LONG)1,NULL)) {
   result = -1;
   SetLastError(0);
  } else result = 0;
#else
  if (sem_post(&self->iob_sem.s_handle) != 0) {
   result = -1;
   errno = 0;
  } else result = 0;
#endif
 } else {
  result = DeeNativeSemaphore_Release(&self->iob_sem);
 }
 DeeAtomicMutex_AcquireRelaxed(&self->iob_lock);
 --self->iob_uses;
 DeeAtomicMutex_Release(&self->iob_lock);
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeIOBuffer_Seek(
 DEE_A_INOUT struct DeeIOBuffer *self, DEE_A_IN Dee_int64_t off,
 DEE_A_IN int whence, DEE_A_OUT_OPT Dee_uint64_t *pos) {
 Dee_size_t max_read,do_read,max_read_before;
 Dee_uint8_t *bufend;
 DEE_ASSERT(self);
 if (whence != DEE_SEEK_CUR) {
  DeeError_SET_STRING(&DeeErrorType_NotImplemented,
                      "The only valid seek disposition for io-buffers is SEEK_CUR");
  return -1;
 }
 if (off < 0) {
  DeeError_SET_STRING(&DeeErrorType_ValueError,
                      "Expected a positive seek offset when seeking in io-buffers");
  return -1;
 }
 if (!off) { if (pos) *pos = 0; return 0; }
wait_for_data:
 DeeAtomicMutex_AcquireRelaxed(&self->iob_lock);
 if ((self->iob_flags&DEE_IOBUFFER_FLAG_CLOSED)!=0) goto unlock_after_closed;
 ++self->iob_uses;
 DeeAtomicMutex_Release(&self->iob_lock);
 if (DeeNativeSemaphore_Acquire(&self->iob_sem) != 0) return -1;
 DeeAtomicMutex_AcquireRelaxed(&self->iob_lock);
 DEE_ASSERT(self->iob_rpos >= self->iob_buf && (!self->iob_size || self->iob_rpos < DeeIOBuffer_BUFEND(self)));
 DEE_ASSERT(self->iob_wpos >= self->iob_buf && (!self->iob_size || self->iob_wpos < DeeIOBuffer_BUFEND(self)));
 if ((self->iob_flags&DEE_IOBUFFER_FLAG_CLOSED)!=0) {
unlock_after_closed:
  DeeAtomicMutex_Release(&self->iob_lock);
  if (pos) *pos = 0; // Already closed
  return 0;
 }
 max_read = DeeIOBuffer_MAX_READ(self);
 if (!max_read) { DeeAtomicMutex_Release(&self->iob_lock); goto wait_for_data; }
 do_read = (Dee_size_t)((Dee_uint64_t)off) < max_read ? (Dee_size_t)((Dee_uint64_t)off) : max_read;
 bufend = DeeIOBuffer_BUFEND(self);
 max_read_before = (Dee_size_t)(bufend-self->iob_rpos);
 if (max_read_before >= do_read) {
  // Can read everything without wrapping (or only have to wrap afterwards)
//memcpy(p,self->iob_rpos,do_read);
  if (max_read_before == do_read)
   self->iob_rpos = self->iob_buf;
  else self->iob_rpos += do_read;
 } else {
  // We'll have to wrap during the read
//memcpy(p,self->iob_rpos,max_read_before);
//memcpy((void *)((Dee_uintptr_t)p+max_read_before),
//       self->iob_buf,do_read-max_read_before);
  self->iob_rpos = self->iob_buf+(do_read-max_read_before);
 }
 if (max_read > do_read) {
  DeeAtomicMutex_Release(&self->iob_lock);
  // More data is available, so signal the semaphore again
  if (DeeNativeSemaphore_Release(&self->iob_sem) != 0) {
   DeeAtomicMutex_AcquireRelaxed(&self->iob_lock);
   --self->iob_uses;
   DeeAtomicMutex_Release(&self->iob_lock);
   return -1;
  }
  DeeAtomicMutex_AcquireRelaxed(&self->iob_lock);
 }
 --self->iob_uses; // We're done using this
 DeeAtomicMutex_Release(&self->iob_lock);
 if (pos) *pos = do_read;
 return 0;
}


DEE_DECL_END

#endif /* !GUARD_DEEMON_IO_BUFFER_C */
