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
#include "dex.c"
#define NATIVE
#endif

DEE_DECL_BEGIN

#ifdef NATIVE
DEE_A_EXEC DEE_A_RET_EXCEPT(NULL) void *
DeeDex_ImportNative(DEE_A_IN_Z char const *name)
#else
DEE_A_EXEC DEE_A_RET_EXCEPT_REF DeeObject *
DeeDex_Import(DEE_A_IN_Z char const *name)
#endif
{
 char const *module_begin;
 char const *module_end;
 char *module_copy; Dee_size_t module_size;
#if !DEE_HAVE_ALLOCA
#ifdef NATIVE
 void *result;
#else
 DeeObject *result;
#endif
#endif
 DEE_ASSERT(name);
 module_begin = name;
 while (1) {
  if (*name == '.') { module_end = name++; break; }
  if (*name == ':' && name[1] == ':') { module_end = name; name += 2; break; }
  if (!*name) {
#ifdef NATIVE
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Expected '.' or '::' after module name %q in native import",
                       module_begin);
   return NULL;
#else
   return DeeDex_Open(module_begin);
#endif
  }
  ++name;
 }
 module_size = (Dee_size_t)(module_end-module_begin);
#if DEE_HAVE_ALLOCA
 module_copy = (char *)alloca((module_size+1)*sizeof(char));
#else
 while ((module_copy = (char *)malloc_nz((module_size+1)*sizeof(char))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return NULL;
 }
#endif
 memcpy(module_copy,module_begin,module_size*sizeof(char));
 module_copy[module_size] = 0;
#if DEE_HAVE_ALLOCA
#ifdef NATIVE
 return DeeDex_ImportNativeEx(module_copy,name);
#else
 return DeeDex_ImportEx(module_copy,name);
#endif
#else
#ifdef NATIVE
 result = DeeDex_ImportNativeEx(module_copy,name);
#else
 result = DeeDex_ImportEx(module_copy,name);
#endif
 free_nn(module_copy);
 return result;
#endif
}

DEE_DECL_END

#ifdef NATIVE
#undef NATIVE
#endif

