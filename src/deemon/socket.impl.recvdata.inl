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
#include "socket.c"
DEE_DECL_BEGIN
#endif

#ifdef ALL
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeSocket_RecvAllDataEx
#else
DEE_A_RET_OBJECT_EXCEPT_REF(DeeStringObject) *DeeSocket_RecvDataEx
#endif
(DEE_A_INOUT_OBJECT(DeeSocketObject) *self, DEE_A_IN Dee_size_t max_bytes, DEE_A_IN int flags) {
 Dee_size_t do_read,did_read,total_read = 0; char *buf; DeeObject *result;
 DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(DeeObject_Check(self) && DeeSocket_Check(self));
 while (1) {
  do_read = DEE_XCONFIG_SOCKET_RECVDATA_BUFSIZE < max_bytes ? DEE_XCONFIG_SOCKET_RECVDATA_BUFSIZE : max_bytes;
  if ((buf = DeeStringWriter_Require(&writer,do_read)) == NULL) goto err;
  if (DeeSocket_RecvEx((DeeObject *)self,buf,do_read*sizeof(Dee_Char),&did_read,flags) == -1) goto err;
  writer.sw_len = (total_read += did_read/sizeof(Dee_Char)); // Trim the writer to the total amount read
#ifdef ALL
  if (!did_read) break; // End of file (no more data)
#else
  if (did_read/sizeof(Dee_Char) != do_read) break; // End of file (not enough data)
#endif
  if ((max_bytes -= did_read/sizeof(Dee_Char)) == 0) break;
 }
 result = DeeStringWriter_Pack(&writer);
end:
 DeeStringWriter_Quit(&writer);
 return result;
err:
 result = NULL;
 goto end;
}

#ifdef ALL
#undef ALL
#endif

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
