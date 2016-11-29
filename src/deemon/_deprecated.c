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
#ifndef GUARD_DEEMON_DEPRECATED_C
#define GUARD_DEEMON_DEPRECATED_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>

// /include/*
#include <deemon/class.h>
#include <deemon/dict.h>
#include <deemon/deemonrun.h>
#include <deemon/file.h>
#include <deemon/list.h>
#include <deemon/compiler/code.h>
#include <deemon/optional/std/stdarg.h>
#include <deemon/optional/object_sequence.h>
#include <deemon/optional/object_doc.h>
#include <deemon/set.h>
#include <deemon/string.h>
#include <deemon/memberdef.h>
#include <deemon/structured.h>
#include <deemon/super.h>
#include <deemon/time.h>

// */ (nano...)

// To ensure backwards-compatibility with previous releases
// with the same API version, we manually re-export removed functions.
// NOTE: By default, export-compatibility is provided for 2 API versions

// Version that API backwards compatibility should be provided for
#define DEE_API_BACKWARDS_VERSION  (DEE_VERSION_API-2)

#define DEE_API_BACKWARDS_IMPLEMENTED(version) \
 (DEE_API_BACKWARDS_VERSION <= (version))

DEE_DECL_BEGIN

#define DEPRECATED_EXPORT(return,name,args)\
 DEE_FUNC_DECL(return) name args; return name args

#if DEE_API_BACKWARDS_IMPLEMENTED(102)
DEPRECATED_EXPORT(int,DeeClass_AddGetSet,(DeeTypeObject *self, DeeObject const *name, DeeObject *getter_, DeeObject *delete_, DeeObject *setter_)) { return DeeClass_AddProperty(self,name,getter_,delete_,setter_); }
DEPRECATED_EXPORT(int,DeeClass_AddClassGetSet,(DeeTypeObject *self, DeeObject const *name, DeeObject *getter_, DeeObject *delete_, DeeObject *setter_)) { return DeeClass_AddClassProperty(self,name,getter_,delete_,setter_); }
DEPRECATED_EXPORT(int,DeeClass_HasGetSet,(DeeTypeObject *self, DeeObject const *name)) { return DeeClass_HasProperty(self,name); }
DEPRECATED_EXPORT(int,DeeClass_HasClassGetSet,(DeeTypeObject *self, DeeObject const *name)) { return DeeClass_HasClassProperty(self,name); }
DEPRECATED_EXPORT(int,DeeClass_GetGetSet,(DeeTypeObject *self, DeeObject const *name, DeeObject **getter_, DeeObject **delete_, DeeObject **setter_)) { return DeeClass_GetProperty(self,name,getter_,delete_,setter_); }
DEPRECATED_EXPORT(int,DeeClass_GetClassGetSet,(DeeTypeObject *self, DeeObject const *name, DeeObject **getter_, DeeObject **delete_, DeeObject **setter_)) { return DeeClass_GetClassProperty(self,name,getter_,delete_,setter_); }
DEPRECATED_EXPORT(int,DeeClass_AddGetSetString,(DeeTypeObject *self, char const *name, DeeObject *getter_, DeeObject *delete_, DeeObject *setter_)) { return DeeClass_AddPropertyString(self,name,getter_,delete_,setter_); }
DEPRECATED_EXPORT(int,DeeClass_AddClassGetSetString,(DeeTypeObject *self, char const *name, DeeObject *getter_, DeeObject *delete_, DeeObject *setter_)) { return DeeClass_AddClassPropertyString(self,name,getter_,delete_,setter_); }
DEPRECATED_EXPORT(int,DeeClass_HasGetSetString,(DeeTypeObject *self, char const *name)) { return DeeClass_HasPropertyString(self,name); }
DEPRECATED_EXPORT(int,DeeClass_HasClassGetSetString,(DeeTypeObject *self, char const *name)) { return DeeClass_HasClassPropertyString(self,name); }
DEPRECATED_EXPORT(int,DeeClass_GetGetSetString,(DeeTypeObject *self, char const *name, DeeObject **getter_, DeeObject **delete_, DeeObject **setter_)) { return DeeClass_GetPropertyString(self,name,getter_,delete_,setter_); }
DEPRECATED_EXPORT(int,DeeClass_GetClassGetSetString,(DeeTypeObject *self, char const *name, DeeObject **getter_, DeeObject **delete_, DeeObject **setter_)) { return DeeClass_GetClassPropertyString(self,name,getter_,delete_,setter_); }

#undef DeeFile_OpenObject
#undef DeeFile_Utf8Open
#undef DeeFile_WideOpen
#undef _DeeFile_OpenObject
#undef _DeeFile_Utf8Open
#undef _DeeFile_WideOpen
DEPRECATED_EXPORT(DeeObject *,DeeFile_Utf8Open,(Dee_Utf8Char const *file, char const *mode)) { return DeeFile_Utf8OpenEx(file,DeeFile_TryParseMode(mode),DEE_FILEIO_DEFAULT_PERMISSIONS); }
DEPRECATED_EXPORT(DeeObject *,DeeFile_WideOpen,(Dee_WideChar const *file, char const *mode)) { return DeeFile_WideOpenEx(file,DeeFile_TryParseMode(mode),DEE_FILEIO_DEFAULT_PERMISSIONS); }
DEPRECATED_EXPORT(DeeObject *,DeeFile_OpenObject,(DeeObject const *file, char const *mode)) { return DeeFile_OpenObjectEx(file,DeeFile_TryParseMode(mode),DEE_FILEIO_DEFAULT_PERMISSIONS); }
DEPRECATED_EXPORT(DeeObject *,_DeeFile_Utf8Open,(Dee_Utf8Char const *file, char const *mode)) { return _DeeFile_Utf8OpenEx(file,DeeFile_TryParseMode(mode),DEE_FILEIO_DEFAULT_PERMISSIONS); }
DEPRECATED_EXPORT(DeeObject *,_DeeFile_WideOpen,(Dee_WideChar const *file, char const *mode)) { return _DeeFile_WideOpenEx(file,DeeFile_TryParseMode(mode),DEE_FILEIO_DEFAULT_PERMISSIONS); }
DEPRECATED_EXPORT(DeeObject *,_DeeFile_OpenObject,(DeeObject const *file, char const *mode)) { return _DeeFile_OpenObjectEx(file,DeeFile_TryParseMode(mode),DEE_FILEIO_DEFAULT_PERMISSIONS); }
#undef DeeFileWriter_New
DEPRECATED_EXPORT(DeeObject *,DeeFileWriter_New,(void)) { return DeeFileWriter_NewWithSizeHint(0); }
#undef DeeFile_Read
#undef DeeFile_Write
#undef DeeFile_ReadAll
#undef DeeFile_WriteAll
#undef DeeFile_Seek
#undef DeeFile_Flush
#undef DeeFile_Trunc
#undef DeeFile_Close
DEPRECATED_EXPORT(int,DeeFile_Read,(DeeObject *self, void *p, Dee_size_t s, Dee_size_t *rs)) { return DeeFile_TRead(Dee_TYPE(self),self,p,s,rs); }
DEPRECATED_EXPORT(int,DeeFile_Write,(DeeObject *self, void const *p, Dee_size_t s, DEE_A_OUT Dee_size_t *ws)) { return DeeFile_TWrite(Dee_TYPE(self),self,p,s,ws); }
DEPRECATED_EXPORT(int,DeeFile_ReadAll,(DeeObject *self, void *p, Dee_size_t s)) { return DeeFile_TReadAll(Dee_TYPE(self),self,p,s); }
DEPRECATED_EXPORT(int,DeeFile_WriteAll,(DeeObject *self, void const *p, Dee_size_t s)) { return DeeFile_TWriteAll(Dee_TYPE(self),self,p,s); }
DEPRECATED_EXPORT(int,DeeFile_Seek,(DeeObject *self, Dee_int64_t off, int whence, Dee_uint64_t *pos)) { return DeeFile_TSeek(Dee_TYPE(self),self,off,whence,pos); }
DEPRECATED_EXPORT(int,DeeFile_Flush,(DeeObject *self)) { return DeeFile_TFlush(Dee_TYPE(self),self); }
DEPRECATED_EXPORT(int,DeeFile_Trunc,(DeeObject *self)) { return DeeFile_TTrunc(Dee_TYPE(self),self); }
DEPRECATED_EXPORT(void,DeeFile_Close,(DeeObject *self)) { DeeFile_TClose(Dee_TYPE(self),self); }
#undef DeeFile_Getc
#undef DeeFile_Putc
#undef DeeFile_Puts
#undef DeeFile_Print
#undef DeeFile_Printf
#undef DeeFile_VPrintf
DEPRECATED_EXPORT(int,DeeFile_Getc,(DeeObject *self, Dee_uint8_t *c)) { return DeeFile_TGetc(Dee_TYPE(self),self,c); }
DEPRECATED_EXPORT(int,DeeFile_Putc,(DeeObject *self, Dee_uint8_t c)) { return DeeFile_TPutc(Dee_TYPE(self),self,c); }
DEPRECATED_EXPORT(int,DeeFile_Puts,(DeeObject *self, char const *msg)) { return DeeFile_TPuts(Dee_TYPE(self),self,msg); }
DEPRECATED_EXPORT(int,DeeFile_Print,(DeeObject *self, char const *msg)) { return DeeFile_TPrint(Dee_TYPE(self),self,msg); }
DEPRECATED_EXPORT(int,DeeFile_Printf,(DeeObject *self, char const *fmt, ...)) { int result; va_list args; DEE_VA_START(args,fmt); result = DeeFile_VTPrintf(Dee_TYPE(self),self,fmt,args); DEE_VA_END(args); return result; }
DEPRECATED_EXPORT(int,DeeFile_VPrintf,(DeeObject *self, char const *fmt, va_list args)) { return DeeFile_VTPrintf(Dee_TYPE(self),self,fmt,args); }
#undef DeeFile_ReadLine
#undef DeeFile_ReadData
#undef DeeFile_ReadAllData
DEPRECATED_EXPORT(DeeObject *,DeeFile_ReadLine,(DeeObject *self, Dee_size_t max_bytes)) { return DeeFile_TReadLine(Dee_TYPE(self),self,max_bytes); }
DEPRECATED_EXPORT(DeeObject *,DeeFile_ReadData,(DeeObject *self, Dee_size_t max_bytes)) { return DeeFile_TReadData(Dee_TYPE(self),self,max_bytes); }
DEPRECATED_EXPORT(DeeObject *,DeeFile_ReadAllData,(DeeObject *self, Dee_size_t max_bytes)) { return DeeFile_TReadAllData(Dee_TYPE(self),self,max_bytes); }
#undef DeeFile_PrintOne
#undef DeeFile_PrintAll
DEPRECATED_EXPORT(int,DeeFile_PrintOne,(DeeObject *self, DeeObject *v)) { return DeeFile_TPrintOne(Dee_TYPE(self),self,Dee_TYPE(v),v); }
DEPRECATED_EXPORT(int,DeeFile_PrintAll,(DeeObject *self, DeeObject *v)) { return DeeFile_TPrintAll(Dee_TYPE(self),self,Dee_TYPE(v),v); }
#undef DeeFile_ReadObject
#undef DeeFile_WriteObject
DEPRECATED_EXPORT(DeeObject *,DeeFile_ReadObject,(DeeObject *self, DeeTypeObject *tp)) { return DeeFile_TReadStructuredObject(Dee_TYPE(self),self,tp); }
DEPRECATED_EXPORT(int,DeeFile_WriteObject,(DeeObject *self, DeeObject *v)) { return DeeFile_TWriteStructuredObject(Dee_TYPE(self),self,v); }
#undef DeeFileIO_Size
DEPRECATED_EXPORT(int,DeeFileIO_Size,(DeeObject *self, Dee_uint64_t *v)) { return DeeFile_Size(self,v); }


#undef DeeList_NewFromSequence
#undef DeeList_AssignSequence
#undef DeeList_InsertSequence
#undef DeeList_AppendSequence
#undef DeeList_SetRange
#undef DeeList_SetRangeIteartor
#undef DeeList_Concat
DEPRECATED_EXPORT(DeeObject *,DeeList_NewFromSequence,(DeeObject *sequence)) { return DeeList_TNewFromSequence(Dee_TYPE(sequence),sequence); }
DEPRECATED_EXPORT(int,DeeList_AssignSequence,(DeeObject *self, DeeObject *sequence)) { return DeeList_TAssignSequence(self,Dee_TYPE(sequence),sequence); }
DEPRECATED_EXPORT(int,DeeList_InsertSequence,(DeeObject *self, Dee_size_t i, DeeObject *sequence)) { return DeeList_TInsertSequence(self,i,Dee_TYPE(sequence),sequence); }
DEPRECATED_EXPORT(int,DeeList_AppendSequence,(DeeObject *self, DeeObject *sequence)) { return DeeList_TAppendSequence(self,Dee_TYPE(sequence),sequence); }
DEPRECATED_EXPORT(int,DeeList_SetRange,(DeeObject *self, Dee_size_t begin, Dee_size_t end, DeeObject *sequence)) { return DeeList_TSetRange(self,begin,end,Dee_TYPE(sequence),sequence); }
DEPRECATED_EXPORT(int,DeeList_SetRangeIteartor,(DeeObject *self, Dee_size_t begin, Dee_size_t end, DeeObject *iterator)) { return DeeList_SetRangeIterator(self,begin,end,iterator); }
DEPRECATED_EXPORT(DeeObject *,DeeList_Concat,(DeeObject const *lhs, DeeObject *rhs)) { return DeeList_TConcat(lhs,Dee_TYPE(rhs),rhs); }

#undef DeeTuple_AppendIterator
#undef DeeTuple_AppendSequence
#undef DeeTuple_Concat
#undef DeeTuple_GetIndex
DEPRECATED_EXPORT(int,DeeTuple_AppendIterator,(DeeObject **self, DeeObject *iterator)) { return DeeTuple_TAppendIterator(self,Dee_TYPE(iterator),iterator); }
DEPRECATED_EXPORT(int,DeeTuple_AppendSequence,(DeeObject **self, DeeObject *sequence)) { return DeeTuple_TAppendSequence(self,Dee_TYPE(sequence),sequence); }
DEPRECATED_EXPORT(DeeObject *,DeeTuple_Concat,(DeeObject const *lhs, DeeObject *rhs))  { return DeeTuple_TConcat(lhs,Dee_TYPE(rhs),rhs); }
DEPRECATED_EXPORT(DeeObject *,DeeTuple_GetIndex,(DeeObject const *self, Dee_size_t index)) { return DeeTuple_Get(self,(Dee_ssize_t)index); }
DEPRECATED_EXPORT(void,DeeTuple_SetElem,(DeeObject const *self, Dee_size_t index, DeeObject *v)) { DeeObject *old_elem; DEE_LVERBOSE1("Using unsafe function: DeeTuple_SetElem\n"); old_elem = DeeTuple_GET(self,index); Dee_INCREF(v); DeeTuple_SET(self,index,v); Dee_DECREF(old_elem); }

#undef DeeObject_CallAttrObjArgs
#undef DeeObject_CallAttrStringObjArgs
#undef DeeObject_CallAttrf
#undef DeeObject_CallAttrStringf
DEPRECATED_EXPORT(DeeObject *,DeeObject_CallAttrObjArgs,(DeeObject *self, DeeObject const *attr, ...)) { DeeObject *result; va_list args; DEE_VA_START(args,attr); result = DeeObject_VTCallAttrObjArgs(Dee_TYPE(self),self,attr,args); DEE_VA_END(args); return result; }
DEPRECATED_EXPORT(DeeObject *,DeeObject_CallAttrStringObjArgs,(DeeObject *self, char const *attr, ...)) { DeeObject *result; va_list args; DEE_VA_START(args,attr); result = DeeObject_VTCallAttrStringObjArgs(Dee_TYPE(self),self,attr,args); DEE_VA_END(args); return result; }
DEPRECATED_EXPORT(DeeObject *,DeeObject_CallAttrf,(DeeObject *self, DeeObject const *attr, char const *fmt, ...)) { DeeObject *result; va_list args; DEE_VA_START(args,fmt); result = DeeObject_VTCallAttrf(Dee_TYPE(self),self,attr,fmt,args); DEE_VA_END(args); return result; }
DEPRECATED_EXPORT(DeeObject *,DeeObject_CallAttrStringf,(DeeObject *self, char const *attr, char const *fmt, ...)) { DeeObject *result; va_list args; DEE_VA_START(args,fmt); result = DeeObject_VTCallAttrStringf(Dee_TYPE(self),self,attr,fmt,args); DEE_VA_END(args); return result; }

#undef DeeObject_CallObjArgs
#undef DeeObject_Callf
#undef DeeObject_ThisCallf
#undef DeeObject_VCallf
#undef DeeObject_VThisCallf
DEPRECATED_EXPORT(DeeObject *,DeeObject_CallObjArgs,(DeeObject *self, ...)) { DeeObject *result; va_list args; DEE_VA_START(args,self); result = DeeObject_VTCallObjArgs(Dee_TYPE(self),self,args); DEE_VA_END(args); return result; }
DEPRECATED_EXPORT(DeeObject *,DeeObject_Callf,(DeeObject *self, char const *fmt, ...)) { DeeObject *result; va_list args; DEE_VA_START(args,fmt); result = DeeObject_VTCallf(Dee_TYPE(self),self,fmt,args); DEE_VA_END(args); return result; }
DEPRECATED_EXPORT(DeeObject *,DeeObject_ThisCallf,(DeeObject *self, DeeObject *this_arg, char const *fmt, ...)) { DeeObject *result; va_list args; DEE_VA_START(args,fmt); result = DeeObject_VTThisCallf(Dee_TYPE(self),self,this_arg,fmt,args); DEE_VA_END(args); return result; }
DEPRECATED_EXPORT(DeeObject *,DeeObject_VCallf,(DeeObject *self, char const *fmt, va_list args)) { return DeeObject_VTCallf(Dee_TYPE(self),self,fmt,args); }
DEPRECATED_EXPORT(DeeObject *,DeeObject_VThisCallf,(DeeObject *self, DeeObject *this_arg, char const *fmt, va_list args)) { return DeeObject_VTThisCallf(Dee_TYPE(self),self,this_arg,fmt,args); }

#undef  DeeObject_Iterate
DEPRECATED_EXPORT(int,DeeObject_Iterate,(DeeObject *self, DeeObjectIterateFunc func, void *closure)) { return DeeObject_TIterate(Dee_TYPE(self),self,func,closure); }

#undef DeeDict_NewFromSeq
DEPRECATED_EXPORT(DeeObject *,DeeDict_NewFromSeq,(DeeObject *sequence)) { return DeeDict_NewFromSequence(sequence); }

#undef DeeObject_Ref
#undef DeeObject_Deref
#undef DeeObject_LValue
DEPRECATED_EXPORT(DeeObject *,DeeObject_Ref,(DeeObject *ob)) { return DeeObject_TRef(Dee_TYPE(ob),ob); }
DEPRECATED_EXPORT(DeeObject *,DeeObject_Deref,(DeeObject *ob)) { return DeeObject_TDeref(Dee_TYPE(ob),ob); }
DEPRECATED_EXPORT(DeeObject *,DeeObject_LValue,(DeeObject *ob)) { return DeeObject_TLValue(Dee_TYPE(ob),ob); }

#undef DeeObject_GetPointerEx
DEPRECATED_EXPORT(int,DeeObject_GetPointerEx,(DeeObject *ob, DeeTypeObject *tp, void **result)) { return DeeObject_TGetPointerEx(Dee_TYPE(ob),ob,tp,result); }

#undef DeeObject_GetPointerExf
#undef DeeObject_VGetPointerExf
DEPRECATED_EXPORT(int,DeeObject_GetPointerExf,(DeeObject *ob, void **result, char const *ty_fmt, ...)) { va_list args; int ires; DEE_VA_START(args,ty_fmt); ires = DeeObject_VTGetPointerExf(Dee_TYPE(ob),ob,result,ty_fmt,args); DEE_VA_END(args); return ires; }
DEPRECATED_EXPORT(int,DeeObject_VGetPointerExf,(DeeObject *ob, void **result, char const *ty_fmt, va_list args)) { return DeeObject_VTGetPointerExf(Dee_TYPE(ob),ob,result,ty_fmt,args); }

#undef DeeObject_TryGetPointer
#undef DeeObject_TryGetPointer2
DEPRECATED_EXPORT(int,DeeObject_TryGetPointer,(DeeObject *ob, DeeTypeObject *tp, void **p)) { return DeeObject_TTryGetPointer(Dee_TYPE(ob),ob,tp,p); }
DEPRECATED_EXPORT(int,DeeObject_TryGetPointer2,(DeeObject *ob, DeeTypeObject *tp, void **p)) { return DeeObject_TTryGetPointer2(Dee_TYPE(ob),ob,tp,p); }

#undef DeeObject_GetLValue
DEPRECATED_EXPORT(void *,DeeObject_GetLValue,(DeeObject *ob, DeeTypeObject *tp)) {
 void *result; // This version is at least safe (unlike the one shipped with v101/101:1)
 if (DeeObject_TGetLValue(Dee_TYPE(ob),ob,tp,&result) != 0) result = NULL;
 else if (!result) {
  DeeError_SET_STRING(&DeeErrorType_RuntimeError,
                      "NULL-lvalues actually are a thing now; "
                      "Update your headers and recompile your app");
 }
 return result;
}

#undef DeeStructured_Copy
DEPRECATED_EXPORT(DeeObject *,DeeStructured_Copy,(DeeObject *ob)) { return DeeObject_TCopy(Dee_TYPE(ob),ob); }

#undef DeeStructured_SetData
DEPRECATED_EXPORT(int,DeeStructured_SetData,(DeeTypeObject const *tp_self, void *self, DeeObject *right)) { return DeeStructured_TSetData(tp_self,self,Dee_TYPE(right),right); }

#undef DeeType_Pointer
#undef DeeType_LValue
#undef DeeType_WithoutPointer
#undef DeeType_WithoutLValue
#undef DeeType_ForeignFunction
#undef DeeType_Array
#undef DeeType_VArray
DEPRECATED_EXPORT(DeeTypeObject *,DeeType_Pointer,(DeeTypeObject const *ob)) { return DeeType_TPointer(Dee_TYPE(ob),ob); }
DEPRECATED_EXPORT(DeeTypeObject *,DeeType_LValue,(DeeTypeObject const *ob)) { return DeeType_TLValue(Dee_TYPE(ob),ob); }
DEPRECATED_EXPORT(DeeTypeObject *,DeeType_WithoutPointer,(DeeTypeObject const *ob)) { return DeeType_TWithoutLValue(Dee_TYPE(ob),ob); }
DEPRECATED_EXPORT(DeeTypeObject *,DeeType_WithoutLValue,(DeeTypeObject const *ob)) { return DeeType_TWithoutPointer(Dee_TYPE(ob),ob); }
DEPRECATED_EXPORT(DeeTypeObject *,DeeType_ForeignFunction,(DeeTypeObject const *self, Dee_funflags_t flags, Dee_size_t argc, DeeTypeObject const *const *argv)) { return DeeType_TForeignFunction(Dee_TYPE(self),self,flags,argc,argv); }
DEPRECATED_EXPORT(DeeTypeObject *,DeeType_Array,(DeeTypeObject const *self, Dee_size_t array_size)) { return DeeType_TArray(Dee_TYPE(self),self,array_size); }
DEPRECATED_EXPORT(DeeTypeObject *,DeeType_VArray,(DeeTypeObject const *self)) { return DeeType_TVArray(Dee_TYPE(self),self); }

#undef DeeTime_NewFromWeeks
#undef DeeTime_NewFromDays
#undef DeeTime_NewFromHours
#undef DeeTime_NewFromMinutes
#undef DeeTime_NewFromSeconds
#undef DeeTime_FormatObject
#undef DeeTime_SetTime
#undef DeeTime_SetMSecond
#undef DeeTime_SetSecond
#undef DeeTime_SetMinute
#undef DeeTime_SetHour
#undef DeeTime_SetWDay
#undef DeeTime_GetYear
#undef DeeTime_SetMDay
#undef DeeTime_SetYDay
#undef DeeTime_SetMWeek
#undef DeeTime_SetYWeek
#undef _DeeTime_SetTimeMSeconds
#undef _DeeTime_SetDateMSeconds
DEPRECATED_EXPORT(DeeObject *,DeeTime_NewFromWeeks,(Dee_uint64_t weeks)) { return DeeTime_New(weeks*DEE_UINT64_C(604800000)); }
DEPRECATED_EXPORT(DeeObject *,DeeTime_NewFromDays,(Dee_uint64_t days)) { return DeeTime_New(days*DEE_UINT64_C(86400000)); }
DEPRECATED_EXPORT(DeeObject *,DeeTime_NewFromHours,(Dee_uint64_t hours)) { return DeeTime_New(hours*DEE_UINT64_C(3600000)); }
DEPRECATED_EXPORT(DeeObject *,DeeTime_NewFromMinutes,(Dee_uint64_t minutes)) { return DeeTime_New(minutes*DEE_UINT64_C(60000)); }
DEPRECATED_EXPORT(DeeObject *,DeeTime_NewFromSeconds,(Dee_uint64_t seconds)) { return DeeTime_New(seconds*DEE_UINT64_C(1000)); }
DEPRECATED_EXPORT(DeeObject *,DeeTime_FormatObject,(DeeObject *self, DeeObject const *fmt)) { DEE_ASSERT(DeeObject_Check(fmt) && DeeString_Check(fmt)); return DeeTime_Format(self,DeeString_STR(fmt)); }
DEPRECATED_EXPORT(void,DeeTime_SetTime,(DeeObject *self, unsigned int hours, unsigned int minutes, unsigned int seconds, unsigned int mseconds)) { DeeTime_SetAfterCurrentDayMSeconds(self,((Dee_uint64_t)hours*DEE_UINT64_C(3600000))+((Dee_uint64_t)minutes*DEE_UINT64_C(60000))+((Dee_uint64_t)seconds*DEE_UINT64_C(1000))+((Dee_uint64_t)mseconds)); }
DEPRECATED_EXPORT(void,DeeTime_SetMSecond,(DeeObject *self, int v)) { DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)(_DeeTime_GetAlignedMSeconds(self,DEE_UINT64_C(1000)+(Dee_int64_t)(v)))); }
DEPRECATED_EXPORT(void,DeeTime_SetSecond,(DeeObject *self, int v)) { DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)(_DeeTime_GetAlignedMSeconds(self,DEE_UINT64_C(60000))+((Dee_int64_t)(v)*DEE_INT64_C(1000)))); }
DEPRECATED_EXPORT(void,DeeTime_SetMinute,(DeeObject *self, int v)) { DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)(_DeeTime_GetAlignedMSeconds(self,DEE_UINT64_C(3600000))+((Dee_int64_t)(v)*DEE_INT64_C(60000)))); }
DEPRECATED_EXPORT(void,DeeTime_SetHour,(DeeObject *self, int v)) { DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)(_DeeTime_GetAlignedMSeconds(self,DEE_UINT64_C(86400000))+((Dee_int64_t)(v)*DEE_INT64_C(3600000)))); }
DEPRECATED_EXPORT(void,DeeTime_SetWDay,(DeeObject *self, int v)) { DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)(_DeeTime_GetAlignedMSeconds(self,DEE_UINT64_C(604800000))+((Dee_int64_t)(v)*DEE_INT64_C(86400000)))); }
DEPRECATED_EXPORT(unsigned int,DeeTime_GetYear,(DeeObject *self)) { return (unsigned int)DeeTime_Days2Years(DeeTime_GetTotalDays(self)); }
DEPRECATED_EXPORT(void,DeeTime_SetMDay,(DeeObject *self, int v)) { DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)((DeeTime_GetTotalMSeconds(self)-((Dee_uint64_t)DeeTime_GetMDay(self)*DEE_INT64_C(86400000)))+((Dee_int64_t)(v)*DEE_INT64_C(86400000)))); }
DEPRECATED_EXPORT(void,DeeTime_SetYDay,(DeeObject *self, int v)) { DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)((DeeTime_GetTotalMSeconds(self)-((Dee_uint64_t)DeeTime_GetYDay(self)*DEE_INT64_C(86400000)))+((Dee_int64_t)(v)*DEE_INT64_C(86400000)))); }
DEPRECATED_EXPORT(void,DeeTime_SetMWeek,(DeeObject *self, int v)) { DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)((DeeTime_GetTotalMSeconds(self)-((Dee_uint64_t)DeeTime_GetMWeek(self)*DEE_INT64_C(604800000)))+((Dee_int64_t)(v)*DEE_INT64_C(604800000)))); }
DEPRECATED_EXPORT(void,DeeTime_SetYWeek,(DeeObject *self, int v)) { DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)((DeeTime_GetTotalMSeconds(self)-((Dee_uint64_t)DeeTime_GetYWeek(self)*DEE_INT64_C(604800000)))+((Dee_int64_t)(v)*DEE_INT64_C(604800000)))); }
DEPRECATED_EXPORT(void,_DeeTime_SetTimeMSeconds,(DeeObject *self, Dee_uint64_t v)) { DeeTime_SetAfterCurrentDayMSeconds(self,v); }
DEPRECATED_EXPORT(void,_DeeTime_SetDateMSeconds,(DeeObject *self, Dee_uint64_t v)) { DeeTime_SetBeforeCurrentDayMSeconds(self,v); }

#undef DeeSuper_Of
DEPRECATED_EXPORT(DeeObject *,DeeSuper_Of,(DeeObject *ob)) { return DeeSuper_TOf(Dee_TYPE(ob),ob); }

#undef DeeSet_NewFromSequence
#undef DeeSet_InsertAllFromSeq
#undef DeeSet_InsertAllFromIterator
DEPRECATED_EXPORT(DeeObject *,DeeSet_NewFromSequence,(DeeObject *sequence)) { return DeeSet_TNewFromSequence(Dee_TYPE(sequence),sequence); }
DEPRECATED_EXPORT(int,DeeSet_InsertAllFromSeq,(DeeObject *self, DeeObject *sequence)) { return DeeSet_TInsertSequence(self,Dee_TYPE(sequence),sequence); }
DEPRECATED_EXPORT(int,DeeSet_InsertAllFromIterator,(DeeObject *self, DeeObject *iterator)) { return DeeSet_InsertIterator(self,iterator); }

#undef DeeObject_ClassOf
DEPRECATED_EXPORT(DeeTypeObject *,DeeObject_ClassOf,(DeeObject const *self)) { return DeeObject_TClassOf(Dee_TYPE(self),self); }

#if DEE_CONFIG_HAVE_ENCODING_WIDE
#undef DeeWideString_Join
#undef DeeObject_WideStr
#undef DeeObject_WideRepr
DEPRECATED_EXPORT(DeeObject *,DeeWideString_Join,(DeeObject const *sep, DeeObject *seq)) { return DeeWideString_TJoin(sep,Dee_TYPE(seq),seq); }
DEPRECATED_EXPORT(DeeObject *,DeeObject_WideStr,(DeeObject *self)) { return DeeObject_TWideStr(Dee_TYPE(self),self); }
DEPRECATED_EXPORT(DeeObject *,DeeObject_WideRepr,(DeeObject *self)) { return DeeObject_TWideRepr(Dee_TYPE(self),self); }
#endif /* DEE_CONFIG_HAVE_ENCODING_WIDE */

#if DEE_CONFIG_HAVE_ENCODING_UTF8
#undef DeeUtf8String_Join
#undef DeeObject_Utf8Str
#undef DeeObject_Utf8Repr
DEPRECATED_EXPORT(DeeObject *,DeeUtf8String_Join,(DeeObject const *sep, DeeObject *seq)) { return DeeUtf8String_TJoin(sep,Dee_TYPE(seq),seq); }
DEPRECATED_EXPORT(DeeObject *,DeeObject_Utf8Str,(DeeObject *self)) { return DeeObject_TUtf8Str(Dee_TYPE(self),self); }
DEPRECATED_EXPORT(DeeObject *,DeeObject_Utf8Repr,(DeeObject *self)) { return DeeObject_TUtf8Repr(Dee_TYPE(self),self); }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF8 */

#if DEE_CONFIG_HAVE_ENCODING_UTF16
#undef DeeUtf16String_Join
#undef DeeObject_Utf16Str
#undef DeeObject_Utf16Repr
DEPRECATED_EXPORT(DeeObject *,DeeUtf16String_Join,(DeeObject const *sep, DeeObject *seq)) { return DeeUtf16String_TJoin(sep,Dee_TYPE(seq),seq); }
DEPRECATED_EXPORT(DeeObject *,DeeObject_Utf16Str,(DeeObject *self)) { return DeeObject_TUtf16Str(Dee_TYPE(self),self); }
DEPRECATED_EXPORT(DeeObject *,DeeObject_Utf16Repr,(DeeObject *self)) { return DeeObject_TUtf16Repr(Dee_TYPE(self),self); }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF16 */

#if DEE_CONFIG_HAVE_ENCODING_UTF32
#undef DeeUtf32String_Join
#undef DeeObject_Utf32Str
#undef DeeObject_Utf32Repr
DEPRECATED_EXPORT(DeeObject *,DeeUtf32String_Join,(DeeObject const *sep, DeeObject *seq)) { return DeeUtf32String_TJoin(sep,Dee_TYPE(seq),seq); }
DEPRECATED_EXPORT(DeeObject *,DeeObject_Utf32Str,(DeeObject *self)) { return DeeObject_TUtf32Str(Dee_TYPE(self),self); }
DEPRECATED_EXPORT(DeeObject *,DeeObject_Utf32Repr,(DeeObject *self)) { return DeeObject_TUtf32Repr(Dee_TYPE(self),self); }
#endif /* DEE_CONFIG_HAVE_ENCODING_UTF32 */

#undef DeeObject_TDoc
#undef DeeObject_TDocAttr
#undef DeeObject_TDocAttrString
DEPRECATED_EXPORT(DeeObject *,DeeObject_TDoc,(DeeTypeObject const *tp_self, DeeObject const *self)) { return Dee_TYPE(self) == tp_self ? DeeObject_Doc(self) : DeeDex_Callf("_docgen.__getdoc__(O)",DeeSuper_New(tp_self,(DeeObject *)self)); }
DEPRECATED_EXPORT(DeeObject *,DeeObject_TDocAttr,(DeeTypeObject const *tp_self, DeeObject const *self, DeeObject const *attr)) { return Dee_TYPE(self) == tp_self ? DeeObject_DocAttr(self,attr) : DeeDex_Callf("_docgen.__getdoc__(Oo)",DeeSuper_New(tp_self,(DeeObject *)self),attr); }
DEPRECATED_EXPORT(DeeObject *,DeeObject_TDocAttrString,(DeeTypeObject const *tp_self, DeeObject const *self, char const *attr)) { return Dee_TYPE(self) == tp_self ? DeeObject_DocAttrString(self,attr) : DeeDex_Callf("_docgen.__getdoc__(Os)",DeeSuper_New(tp_self,(DeeObject *)self),attr); }


#undef DeeSystem_GetUptime
#undef DeeSystem_GetCPUCount
DEPRECATED_EXPORT(int,DeeSystem_GetUptime,(Dee_uint64_t *msecs)) { return DeeDex_CallAndCastf(msecs,"_system.uptime()!I64u"); }
DEPRECATED_EXPORT(int,DeeSystem_GetCPUCount,(Dee_uint64_t *msecs)) { return DeeDex_CallAndCastf(msecs,"_system.cpucount()!I64u"); }

#undef DeeMember_Set
DEPRECATED_EXPORT(int,DeeMember_Set,(DeeObject *base_ob, struct DeeMemberDef const *def, DeeObject *v)) { return DeeMember_TSet(base_ob,def,Dee_TYPE(v),v); }

#undef Dee_GetArgv
#undef Dee_SetArgv
DEPRECATED_EXPORT(DeeObject *,Dee_GetArgv,(void)) { Dee_INCREF(Dee_Argv); return Dee_Argv; }
DEPRECATED_EXPORT(void,Dee_SetArgv,(DeeObject *ob)) {
 if (DeeList_CopyAssign(Dee_Argv,ob) != 0) {
  while (!DeeError_Print("Failed to assign sequence to argv",1));
 }
}


#endif /* 102 */

DEE_DECL_END

#endif /* !GUARD_DEEMON_DEPRECATED_C */
