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
#include "native_fs.copy.c.inl"
#define DO_TRY
#endif

DEE_DECL_BEGIN

#if defined(DeeNativeFileFD) && (defined(DO_TRY)\
 ? (defined(DeeNativeFileFD_TryRead) && defined(DeeNativeFileFD_TryWrite))\
 : (defined(DeeNativeFileFD_Read) && defined(DeeNativeFileFD_Write)))

#ifdef DO_TRY
static DEE_A_RET_NOEXCEPT(0) int DeeNativeFileFD_TryTransferFile(
 DEE_A_INOUT struct DeeNativeFileFD *src_fd,
 DEE_A_INOUT struct DeeNativeFileFD *dst_fd)
#else
static DEE_A_RET_EXCEPT(-1) int DeeNativeFileFD_TransferFile(
 DEE_A_INOUT struct DeeNativeFileFD *src_fd,
 DEE_A_INOUT struct DeeNativeFileFD *dst_fd)
#endif
{
 Dee_size_t read_size,written_size;
 Dee_uint8_t *write_pos;
#ifdef DEE_XCONFIG_FSBUFSIZE_EMULATEDCOPY_ALLOCHEAP
#define RETURN(x) do{free_nn(buffer);return(x);}while(0)
 Dee_uint8_t *buffer;
 while DEE_UNLIKELY((buffer = (Dee_uint8_t *)malloc_nz(
  DEE_XCONFIG_FSBUFSIZE_EMULATEDCOPY*sizeof(Dee_uint8_t))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
#ifdef DO_TRY
  return 0;
#else
  DeeError_NoMemory();
  return -1;
#endif
 }
#else
#define RETURN(x) return (x)
 Dee_uint8_t buffer[DEE_XCONFIG_FSBUFSIZE_EMULATEDCOPY];
#endif
 while (1) {
#ifdef DO_TRY
  if (!DeeNativeFileFD_TryRead(src_fd,buffer,DEE_XCONFIG_FSBUFSIZE_EMULATEDCOPY,&read_size)) RETURN(0);
  if (!read_size) break;
  write_pos = buffer; while (1) {
   if (!DeeNativeFileFD_TryWrite(dst_fd,write_pos,read_size,&written_size)) RETURN(0);
   if (!written_size) RETURN(0);
   if (written_size >= read_size) break;
   read_size -= written_size;
   write_pos += written_size;
  }
#else
  DeeNativeFileFD_Read(src_fd,buffer,DEE_XCONFIG_FSBUFSIZE_EMULATEDCOPY,&read_size,RETURN(-1));
  if (!read_size) break;
  write_pos = buffer; while (1) {
   DeeNativeFileFD_Write(dst_fd,write_pos,read_size,&written_size,RETURN(-1));
   if (!written_size) RETURN(0);
   if (written_size >= read_size) break;
   read_size -= written_size;
   write_pos += written_size;
  }
#endif
 }
#ifdef DO_TRY
 RETURN(1);
#else
 RETURN(0);
#endif
#undef RETURN

}
#endif

#ifdef DO_TRY
#undef DO_TRY
#endif

DEE_DECL_END
