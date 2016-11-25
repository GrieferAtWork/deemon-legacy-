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
#include "process.c"
#define WIDE
DEE_DECL_BEGIN
#endif
#ifdef WIDE
#define DeeProcess_F(x)  DeeProcess_Wide##x
#define _DeeProcess_F(x) _DeeProcess_Wide##x
#define DeeString_F(x)             DeeWideString_##x
#define DEE_STRINGOBJECT           DeeWideStringObject
#define DEE_CHAR                   Dee_WideChar
#define DEE_STRLEN                 Dee_WideStrLen
#define DEE_STRINGWRITER           DeeWideStringWriter
#define DeeStringWriter_F(x)       DeeWideStringWriter_##x
#if DEE_COMPILER_HAVE_WIDESTRING_PREFIX
#define DEE_PRIVATE_CHAR_C(x)      L##x
#define DEE_CHAR_C(x)              DEE_PRIVATE_CHAR_C(x)
#else
#define DEE_CHAR_C(x)              (Dee_WideChar)x
#endif
#define DEE_EMPTYSTRING            Dee_EmptyWideString
#define DEERETURN_EMPTYSTRING      DeeReturn_EmptyWideString
#define DEERETURN_STATIC_STRING_EX DeeReturn_STATIC_WIDE_STRING_EX
#define DEE_PRINTF_STR             "%ls"
#define DEE_PRINTF_STRQ            "%lq"
#else
#define DeeProcess_F(x)  DeeProcess_Utf8##x
#define _DeeProcess_F(x) _DeeProcess_Utf8##x
#define WIN32_F(x)                 x##A
#define DeeString_F(x)             DeeUtf8String_##x
#define DEE_STRINGOBJECT           DeeUtf8StringObject
#define DEE_CHAR                   Dee_Utf8Char
#define DEE_STRLEN                 Dee_Utf8StrLen
#define DEE_CHAR_C(x)              x
#define DEE_STRINGWRITER           DeeUtf8StringWriter
#define DeeStringWriter_F(x)       DeeUtf8StringWriter_##x
#define DEE_EMPTYSTRING            Dee_EmptyUtf8String
#define DEERETURN_EMPTYSTRING      DeeReturn_EmptyUtf8String
#define DEERETURN_STATIC_STRING_EX DeeReturn_STATIC_UTF8_STRING_EX
#define DEE_PRINTF_STR             "%s"
#define DEE_PRINTF_STRQ            "%q"
#endif

#define DEE_STRINGWRITER_INIT    DeeStringWriter_F(INIT)
#define DEE_STRINGWRITER_Pack    DeeStringWriter_F(Pack)
#define DEE_STRINGWRITER_Quit    DeeStringWriter_F(Quit)
#define DEE_STRINGWRITER_SIZE    DeeStringWriter_F(SIZE)
#define DEE_STRINGWRITER_ELEM    DeeStringWriter_F(ELEM)
#define DEE_STRINGWRITER_STR     DeeStringWriter_F(STR)
#define DEE_STRING_Cast          DeeString_F(Cast)
#define DEE_STRING_Check         DeeString_F(Check)
#define DEE_STRING_InplaceCast   DeeString_F(InplaceCast)
#define DEE_STRING_New           DeeString_F(New)
#define DEE_STRING_NewSized      DeeString_F(NewSized)
#define DEE_STRING_NewWithLength DeeString_F(NewWithLength)
#define DEE_STRING_Resize        DeeString_F(Resize)
#define DEE_STRING_SIZE          DeeString_F(SIZE)
#define DEE_STRING_STR           DeeString_F(STR)

#ifdef DEE_PLATFORM_WINDOWS
DEE_STATIC_INLINE(DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *)
_DeeProcess_F(Win32GenerateEnvironmentString)(DEE_A_IN DeeListObject const *env) {
 DeeObject *result,**iter,**end,*name,*value;
 DEE_STRINGWRITER writer = DEE_STRINGWRITER_INIT();
 DEE_ASSERT(DeeObject_Check(env) && DeeList_Check(env));
 if ((env = (DeeListObject const *)DeeList_Copy((
  DeeObject *)env)) == NULL) { result = NULL; goto end2; }
 end = (iter = DeeList_ELEM(env))+DeeList_SIZE(env);
 while (iter != end) {
  DEE_ASSERT(DeeObject_Check(*iter) && DeeTuple_Check(*iter));
  DEE_ASSERT(DeeTuple_SIZE(*iter) == 2);
  name = DeeTuple_GET(*iter,0),value = DeeTuple_GET(*iter,1);
  DEE_ASSERT(DeeObject_Check(name));
  DEE_ASSERT(DeeObject_Check(value));
  if (DeeStringWriter_F(WriteObjectStr)(&writer,name) == -1 ||
      DeeStringWriter_F(WriteChar)(&writer,DEE_CHAR_C('=')) == -1 ||
      DeeStringWriter_F(WriteObjectStr)(&writer,value) == -1 ||
      DeeStringWriter_F(WriteChar)(&writer,DEE_CHAR_C('\0')) == -1) goto err;
  ++iter;
 }
 if (DeeStringWriter_F(WriteChar)(&writer,DEE_CHAR_C('\0')) == -1) goto err;
 result = DEE_STRINGWRITER_Pack(&writer);
end: Dee_DECREF(env); 
end2: DEE_STRINGWRITER_Quit(&writer);
 return result;
err: result = NULL; goto end;
}
#endif

DEE_STATIC_INLINE(DEE_A_RET_EXCEPT(-1) int) _DeeProcess_F(SplitCommandLine)(
 DEE_A_IN_OBJECT(DEE_STRINGOBJECT) const *cmd,
 DEE_A_OUT_OBJECT(DEE_STRINGOBJECT) const **exe,
 DEE_A_OUT_OBJECT(DEE_STRINGOBJECT) const **args) {
 DEE_CHAR *iter,*end;
 end = (iter = DEE_STRING_STR(cmd))+DEE_STRING_SIZE(cmd);
 // TODO: Windows-style splitting, where we check each possibility for existance
 // s.a.: https://msdn.microsoft.com/en-us/library/windows/desktop/ms682425(v=vs.85).aspx
 while (iter != end) {
  if (*iter == DEE_CHAR_C('"')) { while (*++iter != DEE_CHAR_C('"')) if (iter == end) goto done; }
  else if (*iter == DEE_CHAR_C('\\') && iter != end-1) iter += 2;
  else if (DEE_CH_IS_SPACE(*iter)) break;
  else ++iter;
 }
done:
 if (iter == end) {
  // Optimization: empty args
  Dee_INCREF(*exe = cmd);
  Dee_INCREF(*args = DEE_EMPTYSTRING);
 } else {
  if ((*exe = DEE_STRING_NewWithLength((Dee_size_t)(
   iter-DEE_STRING_STR(cmd)),DEE_STRING_STR(cmd))) == NULL) return -1;
  while (DEE_CH_IS_SPACE(*iter)) ++iter;
  if ((*args = DEE_STRING_NewWithLength((Dee_size_t)(end-iter),iter)) == NULL) {
   Dee_DECREF(*exe);
   return -1;
  }
 }
 return 0;
}


DEE_A_RET_OBJECT_EXCEPT_REF(DeeProcessObject) *
DeeProcess_F(New)(DEE_A_IN_Z DEE_CHAR const *cmd) {
 DeeObject *cmd_ob,*result;
 DEE_ASSERT(cmd);
 if ((cmd_ob = DEE_STRING_New(cmd)) == NULL) return NULL;
 result = DeeProcess_NewObject(cmd_ob);
 Dee_DECREF(cmd_ob);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeProcessObject) *DeeProcess_F(NewEx)(
 DEE_A_IN_Z DEE_CHAR const *exe, DEE_A_IN_Z DEE_CHAR const *args) {
 DeeObject *exe_ob,*args_ob,*result;
 if ((exe_ob = DEE_STRING_New(exe)) == NULL) return NULL;
 if ((args_ob = DEE_STRING_New(args)) == NULL) { result = NULL; goto err_0; }
 result = DeeProcess_NewExObject(exe_ob,args_ob);
 Dee_DECREF(args_ob);
err_0: Dee_DECREF(exe_ob);
 return result;
}


//////////////////////////////////////////////////////////////////////////
// Command line
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRING_OBJECT) *
DeeProcess_F(Cmd)(DEE_A_IN_OBJECT(DeeProcessObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if (!DeeProcess_IS_REF(self)) {
  DeeProcess_ACQUIRE(self);
  if (!DeeProcess_STARTED(self)) {
   DEE_STRINGWRITER writer = DEE_STRINGWRITER_INIT();
   DeeObject *result,*exe,*args;
   Dee_INCREF(exe = (DeeObject *)((DeeProcessObject *)self)->p_exe);
   Dee_INCREF(args = (DeeObject *)((DeeProcessObject *)self)->p_args);
   DeeProcess_RELEASE(self);
   if (DeeStringWriter_F(WriteChar)(&writer,DEE_CHAR_C('\"')) == -1
    || DeeStringWriter_F(WriteObjectStr)(&writer,exe) == -1
    || DeeStringWriter_F(WriteChar)(&writer,DEE_CHAR_C('\"')) == -1
    || DeeStringWriter_F(WriteObjectStr)(&writer,args) == -1
       ) result = NULL;
   else result = DEE_STRINGWRITER_Pack(&writer);
   Dee_DECREF(args);
   Dee_DECREF(exe);
   DEE_STRINGWRITER_Quit(&writer);
   return result;
  } else DeeProcess_RELEASE(self);
 }
#ifdef DEE_PLATFORM_WINDOWS
 // TODO: thread-safe
 return _DeeProcess_F(Win32Cmd)(DeeProcess_HANDLE(self),1);
#else
#ifdef WIDE
 {DeeObject *result;
  if ((result = _DeeProcess_PosixGetCmd(DeeProcess_HANDLE(self))) != NULL)
   if (DeeWideString_InplaceCast((DeeObject const **)&result) != 0)
    Dee_CLEAR(result);
  return result;
 }
#else
 return _DeeProcess_PosixGetCmd(DeeProcess_HANDLE(self));
#endif
#endif
}

//////////////////////////////////////////////////////////////////////////
// Executable
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeProcess_F(Exe)(DEE_A_IN_OBJECT(DeeProcessObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if (!DeeProcess_IS_REF(self)) {
  DeeObject *result;
  DeeProcess_ACQUIRE(self);
  Dee_INCREF(result = (DeeObject *)((DeeProcessObject *)self)->p_exe);
  DeeProcess_RELEASE(self);
  if (DEE_STRING_InplaceCast((DeeObject const **)&result) == -1)
   Dee_CLEAR(result);
  return result;
 }
#ifdef DEE_PLATFORM_WINDOWS
 // TODO: thread-safe
 return _DeeProcess_F(Win32Exe)(DeeProcess_HANDLE(self));
#elif defined(WIDE)
 DeeObject *result;
 if ((result = DeeProcess_Utf8Exe(self)) != NULL)
  if (DeeWideString_InplaceCast((DeeObject const **)&result) != 0)
   Dee_CLEAR(result);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 return DeeFS_PosixGetProcessName(DeeProcess_HANDLE(self));
#else
 return DeeString_Newf("/proc/%d/exe",DeeProcess_HANDLE(self));
#endif
}

//////////////////////////////////////////////////////////////////////////
// Argument string
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *
DeeProcess_F(Args)(DEE_A_IN_OBJECT(DeeProcessObject) const *self) {
 DeeObject *cmd,*exe,*result; int error;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if (!DeeProcess_IS_REF(self)) {
  DeeProcess_ACQUIRE(self);
  Dee_INCREF(result = (DeeObject *)((DeeProcessObject *)self)->p_args);
  DeeProcess_RELEASE(self);
  if (DEE_STRING_InplaceCast((DeeObject const **)&result) == -1)
   Dee_CLEAR(result);
  return result;
 }
 if ((cmd = DeeProcess_F(Cmd)(self)) == NULL) return NULL;
 error = _DeeProcess_F(SplitCommandLine)(
  cmd,(DeeObject const **)&exe,(DeeObject const **)&result);
 Dee_DECREF(cmd);
 if (error == -1) return NULL;
 Dee_DECREF(exe);
 return result;
}

//////////////////////////////////////////////////////////////////////////
// Argument vector / list
DEE_A_RET_OBJECT_EXCEPT_REF(DeeListObject) *
DeeProcess_F(Argv)(DEE_A_IN_OBJECT(DeeProcessObject) const *self) {
 DeeObject *args,*result,*arg; int temp;
 DEE_CHAR *begin,*iter,*end;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if ((args = DeeProcess_F(Args)(self)) == NULL) return NULL;
 if ((result = DeeList_New()) == NULL) goto end_0;
 end = (iter = DEE_STRING_STR(args))+DEE_STRING_SIZE(args);
 do {
  begin = iter;
  while (iter != end) {
   if (*iter == DEE_CHAR_C('"')) { while (*++iter != DEE_CHAR_C('"')) if (iter == end) goto done; }
   else if (*iter == DEE_CHAR_C('\\') && iter != end-1) iter += 2;
   else if (DEE_CH_IS_SPACE(*iter)) break;
   else ++iter;
  }
done:
  if ((arg = DEE_STRING_NewWithLength((
   Dee_size_t)(iter-begin),begin)) == NULL) goto err_r;
  temp = DeeList_Append(result,arg);
  Dee_DECREF(arg);
  if (temp == -1) goto err_r;
  while (DEE_CH_IS_SPACE(*iter)) ++iter;
 } while (iter != end);
end_0: Dee_DECREF(args);
 return result;
err_r: Dee_CLEAR(result); goto end_0;
}


//////////////////////////////////////////////////////////////////////////
// Current working directory
DEE_A_RET_OBJECT_EXCEPT_REF(DEE_STRINGOBJECT) *DeeProcess_F(Cwd)(
 DEE_A_IN_OBJECT(DeeProcessObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 if (DeeProcess_IS_SELF(self)) {
#ifdef WIDE
  return DeeNFS_WideGetCwd();
#else
  return DeeNFS_Utf8GetCwd();
#endif
 }
 if (!DeeProcess_IS_REF(self)) {
  DeeProcess_ACQUIRE(self);
  if (!DeeProcess_STARTED(self)) {
   DeeObject *result;
   Dee_XINCREF(result = (DeeObject *)((DeeProcessObject *)self)->p_cwd);
   DeeProcess_RELEASE(self);
#ifdef WIDE
   if (!result) result = DeeNFS_WideGetCwd(); else
#else
   if (!result) result = DeeNFS_Utf8GetCwd(); else
#endif
   if (DEE_STRING_InplaceCast((DeeObject const **)&result) == -1) Dee_CLEAR(result);
   return result;
  } else DeeProcess_RELEASE(self);
 }
#ifdef DEE_PLATFORM_WINDOWS
 // TODO: thread-safe
 return _DeeProcess_F(Win32Cwd)(DeeProcess_HANDLE(self));
#elif defined(WIDE)
 DeeObject *result;
 if ((result = DeeProcess_Utf8Cwd(self)) != NULL)
  if (DeeWideString_InplaceCast((DeeObject const **)&result) != 0)
   Dee_CLEAR(result);
 return result;
#elif defined(DEE_PLATFORM_UNIX)
 DeeObject *result,*filename;
 DEE_ASSERT(DeeObject_Check(self) && DeeProcess_Check(self));
 filename = DeeString_Newf("/proc/%d/cwd",DeeProcess_HANDLE(self));
 result = DeeNFS_ReadlinkObject(filename);
 Dee_DECREF(filename);
 return result;
#else
 return DeeString_Newf("/proc/%d/cwd",DeeProcess_HANDLE(self));
#endif
}



#undef _DeeProcess_F
#undef DeeProcess_F
#undef DeeString_F
#undef DEE_STRINGOBJECT
#undef DEE_CHAR
#undef DEE_STRLEN
#undef DEE_PRINTF_STR
#undef DEE_PRINTF_STRQ
#ifdef DEE_PRIVATE_CHAR_C
#undef DEE_PRIVATE_CHAR_C
#endif
#undef DEE_CHAR_C
#undef DEERETURN_STATIC_STRING_EX
#undef DEE_EMPTYSTRING
#undef DEERETURN_EMPTYSTRING
#undef DEE_STRINGWRITER
#undef DeeStringWriter_F
#undef DEE_STRINGWRITER_INIT
#undef DEE_STRINGWRITER_Pack
#undef DEE_STRINGWRITER_Quit
#undef DEE_STRINGWRITER_SIZE
#undef DEE_STRINGWRITER_ELEM
#undef DEE_STRINGWRITER_STR
#undef DEE_STRING_Cast
#undef DEE_STRING_Check
#undef DEE_STRING_InplaceCast
#undef DEE_STRING_New
#undef DEE_STRING_NewSized
#undef DEE_STRING_NewWithLength
#undef DEE_STRING_Resize
#undef DEE_STRING_SIZE
#undef DEE_STRING_STR
#ifdef WIDE
#undef WIDE
#endif

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
