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
#ifndef GUARD_DEEMON_OPTIONAL_IO_BUFFER_H
#define GUARD_DEEMON_OPTIONAL_IO_BUFFER_H 1

#include <deemon/__conf.inl>
#ifdef DEE_LIMITED_DEX
#include <deemon/mp/mutex.h>
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#undef DEE_PRIVATE_DECL_DEE_SIZE_TYPES
#endif


//////////////////////////////////////////////////////////////////////////
// An IO Buffer can be used to read/write in/out of a generic memory pool.
// e.g.: One thread is constantly writing data, while another is re-reading it
// The buffer will dynamically grow/shrink based on how much storage is required
// NOTE: Once read, data will be removed from the buffer, meaning there is no seek.

#ifdef DEE_LIMITED_DEX
struct DeeIOBuffer {
#define DEE_IOBUFFER_FLAG_CLOSED 0x10000000 /*< Buffer was closed. */
 struct DeeNativeSemaphore iob_sem;   /*< Semaphore signaled when data becomes available (access requires an 'iob_uses' ticket). */
 struct DeeAtomicMutex     iob_lock;  /*< Read/write lock for the buffer. */
 // Everything below is protected by 'iob_lock'
 Dee_uint32_t              iob_uses;  /*< Amount of threads using the semaphore. */
 Dee_uint32_t              iob_flags; /*< Buffer flags. */
 Dee_uint8_t              *iob_rpos;  /*< [0..1][iob_buf..+iob_size] Read position. */
 Dee_uint8_t              *iob_wpos;  /*< [0..1][iob_buf..+iob_size] Write position. */
 Dee_uint8_t              *iob_buf;   /*< [0..1] Start of the buffer (). */
 Dee_size_t                iob_size;  /*< Allocated buffer size. */
};
#define DeeIOBuffer_BUFEND(ob) ((ob)->iob_buf+(ob)->iob_size)

#define DeeIOBuffer_MAX_READ(ob) \
(Dee_size_t)((ob)->iob_rpos <= (ob)->iob_wpos \
 ? (Dee_size_t)((ob)->iob_wpos-(ob)->iob_rpos)\
 : ((ob)->iob_size-(Dee_size_t)((ob)->iob_rpos-(ob)->iob_wpos)))
#define DeeIOBuffer_MAX_WRITE(ob) \
(Dee_size_t)((ob)->iob_wpos < (ob)->iob_rpos \
 ? (Dee_size_t)((ob)->iob_rpos-(ob)->iob_wpos)\
 : ((ob)->iob_size-(Dee_size_t)((ob)->iob_wpos-(ob)->iob_rpos)))
#define DeeIOBuffer_Init(ob) DeeIOBuffer_InitEx(ob,0,DEE_IOBUFFER_FLAG_NONE)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeIOBuffer_InitEx(
 DEE_A_OUT struct DeeIOBuffer *self, DEE_A_IN Dee_size_t size_hint,
 DEE_A_IN Dee_uint32_t flags) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeIOBuffer_Quit(DEE_A_IN struct DeeIOBuffer *self);
#endif

#define DEE_IOBUFFER_FLAG_NONE           0x00000000
#define DEE_IOBUFFER_FLAG_MINBUF         0x00000001 /*< Always reallocate to the smallest possible buffersize. */
#define DEE_IOBUFFER_FLAG_NOEXCEPT_WRITE 0x00000002 /*< Write will never cause an exception. */

#define DeeIOBuffer_New() DeeIOBuffer_NewEx(0,DEE_IOBUFFER_FLAG_NONE)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(NULL) struct DeeIOBuffer *)
 DeeIOBuffer_NewEx(DEE_A_IN Dee_size_t size_hint, DEE_A_IN Dee_uint32_t flags);
DEE_FUNC_DECL(void) DeeIOBuffer_Delete(DEE_A_IN struct DeeIOBuffer *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeIOBuffer_Close(DEE_A_INOUT struct DeeIOBuffer *self) DEE_ATTRIBUTE_NONNULL((1));


//////////////////////////////////////////////////////////////////////////
// Read data from the buffer
// - If no data was available, wait until some is
// - If not enough data is available, only read what we can get
// - Sets '*rs' to 0 if the buffer was closed
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeIOBuffer_Read(
 DEE_A_INOUT struct DeeIOBuffer *self, DEE_A_OUT_WB(*rs) void *p,
 DEE_A_IN Dee_size_t s, DEE_A_OUT Dee_size_t *rs) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Write data into the buffer
// - Wakes the first thread that was waiting for data
// - Throws an Error.IOError if the buffer was closed
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeIOBuffer_Write(
 DEE_A_INOUT struct DeeIOBuffer *self, DEE_A_IN_RB(s) void const *p,
 DEE_A_IN Dee_size_t s) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Skips data from the buffer
// - The only valid value of 'whence' is 'DEE_SEEK_CUR'
// - 'off' must be positive
// - If no data was available, wait until some is
// - If not enough data is available, only skip what we can get
// - Sets '*pos' to the about of skipped data
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeIOBuffer_Seek(
 DEE_A_INOUT struct DeeIOBuffer *self, DEE_A_IN Dee_int64_t off,
 DEE_A_IN int whence, DEE_A_OUT_OPT Dee_uint64_t *pos) DEE_ATTRIBUTE_NONNULL((1));


DEE_DECL_END

#endif /* !GUARD_DEEMON_OPTIONAL_IO_BUFFER_H */
