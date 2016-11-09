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
#include "fs_api.c"
DEE_DECL_BEGIN
#endif

#ifdef DEE_PLATFORM_WINDOWS
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
_DeeFS_Utf8Win32SIDName(DEE_A_IN_OPT PSID sid, DEE_A_IN int full_name) {
 DeeObject *result,*user,*domain; DWORD usersize,domainsize;
 SID_NAME_USE use; DWORD error; Dee_Utf8Char *dst;
 if DEE_UNLIKELY((user = DeeUtf8String_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32SIDNAME)) == NULL) return NULL;
 if DEE_UNLIKELY((domain = DeeUtf8String_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32SIDNAME)) == NULL) { Dee_DECREF(domain); return NULL; }
 domainsize = usersize = DEE_XCONFIG_FSBUFSIZE_WIN32SIDNAME;
 // v we must perform this check ourselves, as 'LookupAccountSid' will otherwise crash
 if DEE_UNLIKELY(!sid || !IsValidSid(sid)) { error = ERROR_INVALID_PARAMETER; goto syserr; }
 while DEE_UNLIKELY(!LookupAccountSidA(NULL,sid,
  DeeUtf8String_STR(user),&usersize,
  DeeUtf8String_STR(domain),&domainsize,&use)) {
  error = DeeSystemError_Win32Consume();
  if DEE_UNLIKELY(error != ERROR_INSUFFICIENT_BUFFER) {
syserr:
   DeeError_SetStringf(&DeeErrorType_SystemError,
                        "LookupAccountSidA(%Iu) : %K",
                        sid,DeeSystemError_Win32ToString(error));
   goto err_r;
  }
  if DEE_UNLIKELY(DeeUtf8String_Resize(&user,usersize) != 0) {
err_r: Dee_DECREF(user); Dee_DECREF(domain); return NULL;
  }
 }
 if (full_name) {
  if DEE_UNLIKELY((result = DeeUtf8String_NewSized(usersize+1+domainsize)) == NULL) goto err_r;
  dst = DeeUtf8String_STR(result);
  memcpy(dst,DeeUtf8String_STR(user),usersize*sizeof(Dee_Utf8Char));
  dst += usersize; *dst = (Dee_Utf8Char)'@';
  memcpy(dst+1,DeeUtf8String_STR(domain),domainsize*sizeof(Dee_Utf8Char));
  Dee_DECREF(user);
  Dee_DECREF(domain);
  return result;
 } else {
  if DEE_UNLIKELY(DeeUtf8String_Resize(&user,usersize) != 0) goto err_r;
  Dee_DECREF(domain);
  return user;
 }
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
_DeeFS_WideWin32SIDName(DEE_A_IN_OPT PSID sid, DEE_A_IN int full_name) {
 DeeObject *result,*user,*domain; DWORD usersize,domainsize;
 SID_NAME_USE use; DWORD error; Dee_WideChar *dst;
 if DEE_UNLIKELY((user = DeeWideString_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32SIDNAME)) == NULL) return NULL;
 if DEE_UNLIKELY((domain = DeeWideString_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32SIDNAME)) == NULL) { Dee_DECREF(domain); return NULL; }
 domainsize = usersize = DEE_XCONFIG_FSBUFSIZE_WIN32SIDNAME;
 // v we must perform this check ourselves, as 'LookupAccountSid' will otherwise crash
 if DEE_UNLIKELY(!sid || !IsValidSid(sid)) { error = ERROR_INVALID_PARAMETER; goto syserr; }
 while DEE_UNLIKELY(!LookupAccountSidW(NULL,sid,
  DeeWideString_STR(user),&usersize,
  DeeWideString_STR(domain),&domainsize,&use)) {
  error = DeeSystemError_Win32Consume();
  if DEE_UNLIKELY(error != ERROR_INSUFFICIENT_BUFFER) {
syserr:
   DeeError_SetStringf(&DeeErrorType_SystemError,
                        "LookupAccountSidW(%Iu) : %K",
                        sid,DeeSystemError_Win32ToString(error));
   goto err_r;
  }
  if DEE_UNLIKELY(DeeWideString_Resize(&user,usersize) != 0) {
err_r: Dee_DECREF(user); Dee_DECREF(domain); return NULL;
  }
 }
 if (full_name) {
  if DEE_UNLIKELY((result = DeeWideString_NewSized(usersize+1+domainsize)) == NULL) goto err_r;
  dst = DeeWideString_STR(result);
  memcpy(dst,DeeWideString_STR(user),usersize*sizeof(Dee_WideChar));
  dst += usersize; *dst = (Dee_WideChar)'@';
  memcpy(dst,DeeWideString_STR(domain),domainsize*sizeof(Dee_WideChar));
  Dee_DECREF(user);
  Dee_DECREF(domain);
  return result;
 } else {
  if DEE_UNLIKELY(DeeWideString_Resize(&user,usersize) != 0) goto err_r;
  Dee_DECREF(domain);
  return user;
 }
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeFS_Utf8UidToUser(DEE_A_IN Dee_uid_t uid, DEE_A_IN int full_name) {
 return _DeeFS_Utf8Win32SIDName((PSID)uid,full_name);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeFS_WideUidToUser(DEE_A_IN Dee_uid_t uid, DEE_A_IN int full_name) {
 return _DeeFS_WideWin32SIDName((PSID)uid,full_name);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeFS_Utf8GidToGroup(DEE_A_IN Dee_gid_t gid, DEE_A_IN int full_name) {
 return _DeeFS_Utf8Win32SIDName((PSID)gid,full_name);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) *
DeeFS_WideGidToGroup(DEE_A_IN Dee_gid_t gid, DEE_A_IN int full_name) {
 return _DeeFS_WideWin32SIDName((PSID)gid,full_name);
}


struct DeeFSWin32SIDObject {
 DEE_OBJECT_HEAD
 Dee_uint8_t wsid_data[1024];
};
#define DeeFSWin32SID_SID(ob)  ((PSID)((DeeFSWin32SIDObject *)(ob))->wsid_data)
DEE_A_RET_WUNUSED Dee_size_t DeeFSWin32SID_GetID(
 DEE_A_IN_OBJECT(DeeFSWin32SIDObject) const *self) {
 return (Dee_size_t)(Dee_uintptr_t)DeeFSWin32SID_SID(self);
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSWin32SIDObject) *DeeFS_Utf8Win32NameToSID(
 DEE_A_IN_Z Dee_Utf8Char const *name) {
 DeeObject *result,*domain,*new_result; DWORD resultsize,domainsize;
 SID_NAME_USE use; DWORD error;
 DEE_ASSERT(name);
 if DEE_UNLIKELY((result = (DeeObject *)DeeObject_Malloc(
  Dee_OFFSETOF(DeeFSWin32SIDObject,wsid_data)+
  DEE_XCONFIG_FSBUFSIZE_WIN32NAMETOSID)) == NULL) return NULL;
 if DEE_UNLIKELY((domain = DeeUtf8String_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32NAMETOSID)) == NULL) {
err_r: DeeObject_Free(result); return NULL;
 }
 resultsize = DEE_XCONFIG_FSBUFSIZE_WIN32NAMETOSID,domainsize = DEE_XCONFIG_FSBUFSIZE_WIN32NAMETOSID;
 while DEE_UNLIKELY(!LookupAccountNameA(NULL,name,
  DeeFSWin32SID_SID(result),&resultsize,
  DeeUtf8String_STR(domain),&domainsize,&use)) {
  error = DeeSystemError_Win32Consume();
  if DEE_UNLIKELY(error != ERROR_INSUFFICIENT_BUFFER) {
   if DEE_LIKELY(error == ERROR_NONE_MAPPED) {
    Dee_Utf8Char const *iter,*end;
    // This might be a 'user@machine' kind of username
    // >> Convert it into the windows 'machine\user' form
    iter = name;
    while (*iter) if (*iter++ == (Dee_Utf8Char)'@') {
     Dee_size_t domain_size; // Yes, it is!
     DeeObject *win32_username;
     DeeObject_Free(result);
     Dee_DECREF(domain);
     end = iter+Dee_Utf8StrLen(iter);
     domain_size = (Dee_size_t)(end-iter);
     if DEE_UNLIKELY((win32_username = DeeUtf8String_NewSized(
      (Dee_size_t)(end-name))) == NULL) return NULL;
     memcpy(DeeUtf8String_STR(win32_username),iter,domain_size*sizeof(Dee_Utf8Char));
     DeeUtf8String_STR(win32_username)[domain_size] = '\\';
     memcpy(DeeUtf8String_STR(win32_username)+(domain_size+1),
            name,(Dee_size_t)((iter-1)-name)*sizeof(Dee_Utf8Char));
     result = DeeFS_Utf8Win32NameToSID(DeeUtf8String_STR(win32_username));
     Dee_DECREF(win32_username);
     return result;
    }
   }
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "LookupAccountNameA(%q) : %K",
                       name,DeeSystemError_Win32ToString(error));
err_dr: Dee_DECREF(domain); goto err_r;
  }
  if DEE_UNLIKELY((new_result = (DeeObject *)DeeObject_Realloc(result,
   Dee_OFFSETOF(DeeFSWin32SIDObject,wsid_data)+resultsize)) == NULL) goto err_dr;
  if DEE_UNLIKELY(DeeWideString_Resize(&domain,domainsize) != 0) goto err_dr;
 }
 Dee_DECREF(domain);
 if DEE_UNLIKELY(resultsize != DEE_XCONFIG_FSBUFSIZE_WIN32NAMETOSID && (new_result = (DeeObject *)
  DeeObject_Realloc(result,Dee_OFFSETOF(DeeFSWin32SIDObject,wsid_data)+resultsize)) == NULL) goto err_r;
 DeeObject_INIT(result,&DeeFSWin32SID_Type);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSWin32SIDObject) *DeeFS_WideWin32NameToSID(
 DEE_A_IN_Z Dee_WideChar const *name) {
 DeeObject *result,*domain,*new_result; DWORD resultsize,domainsize;
 SID_NAME_USE use; DWORD error;
 if DEE_UNLIKELY((result = (DeeObject *)DeeObject_Malloc(
  Dee_OFFSETOF(DeeFSWin32SIDObject,wsid_data)+
  DEE_XCONFIG_FSBUFSIZE_WIN32NAMETOSID)) == NULL) return NULL;
 if DEE_UNLIKELY((domain = DeeWideString_NewSized(DEE_XCONFIG_FSBUFSIZE_WIN32NAMETOSID)) == NULL) {err_r: DeeObject_Free(result); return NULL; }
 resultsize = DEE_XCONFIG_FSBUFSIZE_WIN32NAMETOSID,domainsize = DEE_XCONFIG_FSBUFSIZE_WIN32NAMETOSID;
 while DEE_UNLIKELY(!LookupAccountNameW(NULL,name,
  DeeFSWin32SID_SID(result),&resultsize,
  DeeWideString_STR(domain),&domainsize,&use)) {
  error = DeeSystemError_Win32Consume();
  if DEE_UNLIKELY(error != ERROR_INSUFFICIENT_BUFFER) {
   if DEE_LIKELY(error == ERROR_NONE_MAPPED) {
    Dee_WideChar const *iter,*end;
    // This might be a 'user@machine' kind of username
    // >> Convert it into the windows 'machine\user' form
    iter = name;
    while (*iter) if (*iter++ == (Dee_WideChar)'@') {
     Dee_size_t domain_size; // Yes, it is!
     DeeObject *win32_username;
     DeeObject_Free(result);
     Dee_DECREF(domain);
     end = iter+Dee_WideStrLen(iter);
     domain_size = (Dee_size_t)(end-iter);
     if DEE_UNLIKELY((win32_username = DeeWideString_NewSized(
      (Dee_size_t)(end-name))) == NULL) return NULL;
     memcpy(DeeWideString_STR(win32_username),iter,domain_size*sizeof(Dee_WideChar));
     DeeWideString_STR(win32_username)[domain_size] = '\\';
     memcpy(DeeWideString_STR(win32_username)+(domain_size+1),
            name,(Dee_size_t)((iter-1)-name)*sizeof(Dee_WideChar));
     result = DeeFS_WideWin32NameToSID(DeeWideString_STR(win32_username));
     Dee_DECREF(win32_username);
     return result;
    }
   }
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "LookupAccountNameW(%lq) : %K",
                       name,DeeSystemError_Win32ToString(error));
err_dr: Dee_DECREF(domain); goto err_r;
  }
  if DEE_UNLIKELY((new_result = (DeeObject *)DeeObject_Realloc(result,
   Dee_OFFSETOF(DeeFSWin32SIDObject,wsid_data)+resultsize)) == NULL) goto err_dr;
  if DEE_UNLIKELY(DeeWideString_Resize(&domain,domainsize) != 0) goto err_dr;
 }
 Dee_DECREF(domain);
 if DEE_UNLIKELY(resultsize != DEE_XCONFIG_FSBUFSIZE_WIN32NAMETOSID && (new_result = (DeeObject *)
  DeeObject_Realloc(result,Dee_OFFSETOF(DeeFSWin32SIDObject,wsid_data)+resultsize)) == NULL) goto err_r;
 DeeObject_INIT(result,&DeeFSWin32SID_Type);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSWin32SIDObject) *DeeFS_Win32NameToSIDObject(
 DEE_A_IN_OBJECT(DeeAnyStringObject) const *name) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeUtf8String_Check(name)) return DeeFS_Utf8Win32NameToSID(DeeUtf8String_STR(name));
 if DEE_UNLIKELY((name = DeeWideString_Cast(name)) == NULL) return NULL;
 result = DeeFS_WideWin32NameToSID(DeeWideString_STR(name));
 Dee_DECREF(name);
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeFSWin32SIDObject) *
DeeFSWin32SID_NewFromSID(/*PSID*/Dee_uintptr_t sid) {
 DWORD error,sid_size; DeeObject *result;
 if DEE_UNLIKELY(!sid || !IsValidSid((PSID)sid)) {
  error = ERROR_INVALID_PARAMETER;
  goto syserr;
 }
retry:
 sid_size = GetLengthSid((PSID)sid);
 if DEE_UNLIKELY((result = (DeeObject *)DeeObject_Malloc(
  Dee_OFFSETOF(DeeFSWin32SIDObject,wsid_data)+sid_size)) == NULL) return NULL;
 if DEE_UNLIKELY(!CopySid(sid_size,DeeFSWin32SID_SID(result),(PSID)sid)) {
  error = DeeSystemError_Win32Consume();
  if DEE_LIKELY(error == ERROR_INSUFFICIENT_BUFFER) goto retry;
  DeeObject_Free(result);
syserr:
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "CopySid(%Iu:%r) : %K",
                      sid,_DeeFS_Utf8Win32SIDName((PSID)sid,1),
                      DeeSystemError_Win32ToString(error));
  return NULL;
 }
 DeeObject_INIT(result,&DeeFSWin32SID_Type);
 return result;
}

static DeeObject *_deefswin32sid_tp_any_new(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeObject *args) {
 DeeObject *result,*arg0; Dee_uintptr_t sid;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:win32_sid",&arg0) != 0) return NULL;
 if (DeeUtf8String_Check(arg0)) return DeeFS_Utf8Win32NameToSID(DeeUtf8String_STR(arg0));
 if (DeeWideString_Check(arg0)) return DeeFS_WideWin32NameToSID(DeeWideString_STR(arg0));
 if (DeeUtf16String_Check(arg0) || DeeUtf32String_Check(arg0)) {
  if DEE_UNLIKELY((arg0 = DeeWideString_Cast(arg0)) == NULL) return NULL;
  result = DeeFS_WideWin32NameToSID(DeeWideString_STR(arg0));
  Dee_DECREF(arg0);
  return result;
 }
 if DEE_UNLIKELY(DeeObject_Cast(Dee_uintptr_t,arg0,&sid) != 0) return NULL;
 return DeeFSWin32SID_NewFromSID(sid);
}
static DeeObject *_deefswin32sid_tp_str(DeeFSWin32SIDObject *self) { return _DeeFS_Utf8Win32SIDName(DeeFSWin32SID_SID(self),0); }
static DeeObject *_deefswin32sid_tp_repr(DeeFSWin32SIDObject *self) { return _DeeFS_Utf8Win32SIDName(DeeFSWin32SID_SID(self),1); }
static int _deefswin32sid_tp_int32(DeeFSWin32SIDObject *self, Dee_int32_t *result) { *result = (Dee_int32_t)(Dee_uintptr_t)DeeFSWin32SID_SID(self); return 0; }
static int _deefswin32sid_tp_int64(DeeFSWin32SIDObject *self, Dee_int64_t *result) { *result = (Dee_int64_t)(Dee_uintptr_t)DeeFSWin32SID_SID(self); return 0; }
static int _deefswin32sid_tp_double(DeeFSWin32SIDObject *self, double *result) { *result = (double)(Dee_uintptr_t)DeeFSWin32SID_SID(self); return 0; }
static DeeObject *_deefswin32sid_cmp_eq(DeeFSWin32SIDObject *self, DeeObject *right) {
 Dee_uid_t rhs_uid;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_uid_t,right,&rhs_uid) != 0) return NULL;
 if DEE_UNLIKELY(!rhs_uid || !IsValidSid((PSID)rhs_uid)) DeeReturn_False;
 DeeReturn_Bool(EqualSid(DeeFSWin32SID_SID(self),(PSID)rhs_uid));
}
static DeeObject *_deefswin32sid_cmp_ne(DeeFSWin32SIDObject *self, DeeObject *right) {
 Dee_uid_t rhs_uid;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_uid_t,right,&rhs_uid) != 0) return NULL;
 if DEE_UNLIKELY(!rhs_uid || !IsValidSid((PSID)rhs_uid)) DeeReturn_True;
 DeeReturn_Bool(!EqualSid(DeeFSWin32SID_SID(self),(PSID)rhs_uid));
}

DeeTypeObject DeeFSWin32SID_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("win32_sid"),null,member(DEE_TYPE_FLAG_VAR_OBJECT),null),
 DEE_TYPE_OBJECT_VAR_CONSTRUCTOR_v101(null,null,null,member(&_deefswin32sid_tp_any_new)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member((DeeObject*(*)(DeeObject*))&_deefswin32sid_tp_str),
  member((DeeObject*(*)(DeeObject*))&_deefswin32sid_tp_repr),
  member((int(*)(DeeObject*,Dee_int32_t*))&_deefswin32sid_tp_int32),
  member((int(*)(DeeObject*,Dee_int64_t*))&_deefswin32sid_tp_int64),
  member((int(*)(DeeObject*,double*))&_deefswin32sid_tp_double)),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,
  member((DeeObject*(*)(DeeObject*,DeeObject*))&_deefswin32sid_cmp_eq),
  member((DeeObject*(*)(DeeObject*,DeeObject*))&_deefswin32sid_cmp_ne),null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};
#endif


#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
