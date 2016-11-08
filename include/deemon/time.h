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
#ifndef GUARD_DEEMON_TIME_H
#define GUARD_DEEMON_TIME_H 1

#include <deemon/__conf.inl>
#include <deemon/optional/object_decl.h>
#ifdef DEE_LIMITED_API
#include <deemon/object.h>
#include <deemon/optional/string_forward.h>
#endif

#if DEE_ENVIRONMENT_HAVE_INCLUDE_TIME_H
#include DEE_INCLUDE_MEMORY_API_DISABLE()
#include <time.h>
#include DEE_INCLUDE_MEMORY_API_ENABLE()
typedef time_t            Dee_time_t;
#else
typedef DEE_TYPES_XINT(
 DEE_TYPES_TIME_T_SIGNED,
 DEE_TYPES_SIZEOF_TIME_T) Dee_time_t;
#endif


//////////////////////////////////////////////////////////////////////////
//
// WARNING:
//    -- The time API will be moved into a Dex sooner or later --
//   This header will remain, and it will still be possible to use the API,
//   but additional code may be required to load the API before use,
//   as well as binary compatibility with any function/object declared
//   by this header being broken.
//
//////////////////////////////////////////////////////////////////////////

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif
#ifdef DEE_PRIVATE_DECL_DEE_OBJECT
DEE_PRIVATE_DECL_DEE_OBJECT
#undef DEE_PRIVATE_DECL_DEE_OBJECT
#endif
#ifdef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#undef DEE_PRIVATE_DECL_DEE_TYPEOBJECT
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Time
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

DEE_OBJECT_DEF(DeeTimeObject);

enum DeeTimeDiffKind{
 DeeTimeKind_DEFAULT = 0,
 // We need a different system for months and years,
 // since those can have varying lengths.
 // - So we need this to make sure that something like
 //   >> a = time.now();
 //   >> b = time.months(1);
 //   >> c = a+b; // Today in one month
 //   Always returns the correct value,
 //   no matter how long the month is.
 DeeTimeKind_MONTHS = 1,
 DeeTimeKind_YEARS = 2,
};
DEE_COMPILER_MSVC_WARNING_PUSH(4201)
struct DeeTimeObject {
 DEE_OBJECT_HEAD
 Dee_uint16_t  tm_diff_kind;
 union{
  Dee_uint64_t tm_msecs;  /*< kind == DeeTimeKind_DEFAULT: Milliseconds since 01.01.0000. */
  Dee_uint64_t tm_months; /*< kind == DeeTimeKind_MONTHS:  Months since 01.01.0000. */
  Dee_uint64_t tm_years;  /*< kind == DeeTimeKind_YEARS:   Years since 01.01.0000. */
 }
#if !DEE_COMPILER_HAVE_UNNAMED_UNION
#define tm_msecs  _tm_timedata.tm_msecs
#define tm_months _tm_timedata.tm_months
#define tm_years  _tm_timedata.tm_years
 _tm_timedata
#endif /* !DEE_COMPILER_HAVE_UNNAMED_UNION */
 ;
 // NOTE: I purposefully didn't use 1970 as base, so
 //       the user can easily just take a time object
 //       and use another to add onto it. e.g.:
 //       >> #include <time>
 //       >> // Prints the the current time and date next week
 //       >> print time.now()+time.days(7);
};
DEE_COMPILER_MSVC_WARNING_POP

#define DeeTime_Check(ob)      DeeObject_InstanceOf(ob,&DeeTime_Type)
#define DeeTime_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeTime_Type)
DEE_DATA_DECL(DeeTypeObject) DeeTime_Type;

#define DeeTime_SetTick(self,tick) \
 (((DeeTimeObject *)Dee_REQUIRES_POINTER(self))->tm_diff_kind=DeeTimeKind_DEFAULT,\
  ((DeeTimeObject *)Dee_REQUIRES_POINTER(self))->tm_msecs=(tick))
#define DeeTime_GetTick DeeTime_GetTotalMSeconds

struct tm;
#define DeeTime_Copy(ob) DeeTime_New(DeeTime_GetTick(ob))
#define DeeTime_NewFromData(year,month,mday,hour,minute,second,msecond) \
 DeeTime_New(DeeTimeTick_FromData(year,month,mday,hour,minute,second,msecond))
#define DeeTime_AsLocalTimeT(ob,t) (*(t)=(Dee_time_t)((DeeTime_GetTotalMSeconds(ob)/DEE_UINT64_C(1000))-DEE_UINT64_C(62167132800000)),0)
#define DeeTime_NewFromLocalTimeT(t) DeeTime_New(((Dee_uint64_t)(t)*DEE_UINT64_C(1000))+DEE_UINT64_C(62167132800000))
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *) DeeTime_NewFromTimeT(DEE_A_IN Dee_time_t t);
#define DeeTime_NewFromTm(t) DeeTime_New(DeeTimeTick_FromTm(t))
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *) DeeTime_NewFromYears(DEE_A_IN Dee_uint64_t years);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *) DeeTime_NewFromMonths(DEE_A_IN Dee_uint64_t months);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *) DeeTime_New(DEE_A_IN Dee_uint64_t ticks);
#define DeeTime_NewFromWeeks(weeks)       DeeTime_New((weeks)*DEE_UINT64_C(604800000))
#define DeeTime_NewFromDays(days)         DeeTime_New((days)*DEE_UINT64_C(86400000))
#define DeeTime_NewFromHours(hours)       DeeTime_New((hours)*DEE_UINT64_C(3600000))
#define DeeTime_NewFromMinutes(minutes)   DeeTime_New((minutes)*DEE_UINT64_C(60000))
#define DeeTime_NewFromSeconds(seconds)   DeeTime_New((seconds)*DEE_UINT64_C(1000))
#define DeeTime_NewFromMSeconds           DeeTime_New


DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *) DeeTime_Now(void);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeTime_AsTimeT(DEE_A_IN_OBJECT(DeeTimeObject) const *self, DEE_A_OUT Dee_time_t *t) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeTime_AsTm(DEE_A_IN_OBJECT(DeeTimeObject) const *self, DEE_A_OUT struct tm *t) DEE_ATTRIBUTE_NONNULL((1,2));

DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint64_t) DeeTimeTick_FromData(
 DEE_A_IN unsigned int year, DEE_A_IN unsigned int month,
 DEE_A_IN unsigned int mday, DEE_A_IN unsigned int hour,
 DEE_A_IN unsigned int minute, DEE_A_IN unsigned int second,
 DEE_A_IN unsigned int msecond);
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeTimeTick_Now(DEE_A_OUT Dee_uint64_t *ticks) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeTimeTick_FromTimeT(DEE_A_IN Dee_time_t t, DEE_A_OUT Dee_uint64_t *ticks) DEE_ATTRIBUTE_NONNULL((2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint64_t) DeeTimeTick_FromTm(DEE_A_IN struct tm const *t) DEE_ATTRIBUTE_NONNULL((1));

#define /* Dee_uint64_t */DeeTime_Years2Days(/* Dee_uint64_t */n_years) (((DEE_UINT64_C(146097)*(Dee_uint64_t)(n_years))/DEE_UINT64_C(400))/*-DEE_UINT64_C(1)*/) // rounding error?
#define /* Dee_uint64_t */DeeTime_Days2Years(/* Dee_uint64_t */n_days)  ((DEE_UINT64_C(400)*((Dee_uint64_t)(n_days)+DEE_UINT64_C(1)))/DEE_UINT64_C(146097))


#ifdef DEE_PLATFORM_WINDOWS
struct _SYSTEMTIME; struct _FILETIME;
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint64_t) DeeTimeTick_FromWin32SystemTime(DEE_A_IN struct _SYSTEMTIME const *t) DEE_ATTRIBUTE_NONNULL((1));
#define DeeTimeTick_FromWin32FileTime0(/*FILETIME const **/t) \
 ((*(Dee_uint64_t *)(t))/DEE_UINT64_C(10000))
#define DeeTimeTick_FromWin32LocalFileTime(/*FILETIME const **/t) \
 (DeeTimeTick_FromWin32FileTime0(t)+DEE_UINT64_C(50522659200000))
// Source: https://msdn.microsoft.com/en-us/library/windows/desktop/ms724284%28v=vs.85%29.aspx
// According to microsoft, FILETIME has a resolution of 100 nanoseconds. - Or in other words: 10000 milliseconds.
// The time started on 01.01.1601, which are '50522659200000' milliseconds since 01.01.0000
#define DeeTime_AsWin32LocalFileTime(ob,lft) \
 (*((Dee_uint64_t *)(lft))=(DeeTime_GetTotalMSeconds(ob)-DEE_UINT64_C(50522659200000))*DEE_UINT64_C(10000),0)

#define DeeTime_NewFromWin32SystemTime(t) DeeTime_New(DeeTimeTick_FromWin32SystemTime(t))
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeTime_AsWin32FileTime(DEE_A_IN_OBJECT(DeeTimeObject) const *self, DEE_A_OUT struct _FILETIME *t) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeTime_AsWin32SystemTime(DEE_A_IN_OBJECT(DeeTimeObject) const *self, DEE_A_OUT struct _SYSTEMTIME *t) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *) DeeTime_NewFromWin32FileTime(DEE_A_IN struct _FILETIME const *t) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *) DeeTime_NewFromWin32DosDateTime(DEE_A_IN Dee_uint16_t fat_date, DEE_A_IN Dee_uint16_t fat_time);
#define DeeTime_NewFromWin32LocalFileTime(/*FILETIME const **/t)  DeeTime_New(DeeTimeTick_FromWin32LocalFileTime(t))
#define DeeTime_NewFromWin32FileTime0(/*FILETIME const **/t)  DeeTime_New(DeeTimeTick_FromWin32FileTime0(t))
#endif /* DEE_PLATFORM_WINDOWS */

DEE_FUNC_DECL(void) DeeTime_InplaceAddMSeconds(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN Dee_int64_t mseconds) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeTime_InplaceAddMonths(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN Dee_int64_t months) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeTime_InplaceAddYears(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN Dee_int64_t years) DEE_ATTRIBUTE_NONNULL((1));
#define DeeTime_InplaceAddSeconds(self,seconds) DeeTime_InplaceAddMSeconds(self,(seconds)*DEE_INT64_C(1000))
#define DeeTime_InplaceAddMinutes(self,minutes) DeeTime_InplaceAddMSeconds(self,(minutes)*DEE_INT64_C(60000))
#define DeeTime_InplaceAddHours(self,hours)     DeeTime_InplaceAddMSeconds(self,(hours)*DEE_INT64_C(3600000))
#define DeeTime_InplaceAddDays(self,days)       DeeTime_InplaceAddMSeconds(self,(days)*DEE_INT64_C(86400000))
#define DeeTime_InplaceAddWeeks(self,weeks)     DeeTime_InplaceAddMSeconds(self,(weeks)*DEE_INT64_C(604800000))

#define DeeTime_SetTotalMSeconds(ob,mseconds) (((DeeTimeObject *)Dee_REQUIRES_POINTER(ob))->tm_diff_kind=DeeTimeKind_DEFAULT,((DeeTimeObject *)(ob))->tm_msecs=(mseconds))
#define DeeTime_SetTotalSeconds(ob,seconds)   (((DeeTimeObject *)Dee_REQUIRES_POINTER(ob))->tm_diff_kind=DeeTimeKind_DEFAULT,((DeeTimeObject *)(ob))->tm_msecs=(seconds)*DEE_UINT64_C(1000))
#define DeeTime_SetTotalMinutes(ob,minutes)   (((DeeTimeObject *)Dee_REQUIRES_POINTER(ob))->tm_diff_kind=DeeTimeKind_DEFAULT,((DeeTimeObject *)(ob))->tm_msecs=(minutes)*DEE_UINT64_C(60000))
#define DeeTime_SetTotalHours(ob,hours)       (((DeeTimeObject *)Dee_REQUIRES_POINTER(ob))->tm_diff_kind=DeeTimeKind_DEFAULT,((DeeTimeObject *)(ob))->tm_msecs=(hours)*DEE_UINT64_C(3600000))
#define DeeTime_SetTotalDays(ob,days)         (((DeeTimeObject *)Dee_REQUIRES_POINTER(ob))->tm_diff_kind=DeeTimeKind_DEFAULT,((DeeTimeObject *)(ob))->tm_msecs=(days)*DEE_UINT64_C(86400000))
#define DeeTime_SetTotalWeeks(ob,weeks)       (((DeeTimeObject *)Dee_REQUIRES_POINTER(ob))->tm_diff_kind=DeeTimeKind_DEFAULT,((DeeTimeObject *)(ob))->tm_msecs=(weeks)*DEE_UINT64_C(604800000))
#define DeeTime_SetTotalMonths(ob,months)     (((DeeTimeObject *)Dee_REQUIRES_POINTER(ob))->tm_diff_kind=DeeTimeKind_MONTHS,((DeeTimeObject *)(ob))->tm_months=(months))
#define DeeTime_SetTotalYears(ob,years)       (((DeeTimeObject *)Dee_REQUIRES_POINTER(ob))->tm_diff_kind=DeeTimeKind_YEARS,((DeeTimeObject *)(ob))->tm_years=(years))

#define _DeeTime_GetAlignedMSeconds(self,alignment) ((DeeTime_GetTotalMSeconds(self)/(alignment))*(alignment))
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint64_t) DeeTime_GetTotalMSeconds(DEE_A_IN_OBJECT(DeeTimeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#define DeeTime_GetTotalSeconds(ob) (DeeTime_GetTotalMSeconds(ob)/DEE_UINT64_C(1000))
#define DeeTime_GetTotalMinutes(ob) (DeeTime_GetTotalMSeconds(ob)/DEE_UINT64_C(60000))
#define DeeTime_GetTotalHours(ob)   (DeeTime_GetTotalMSeconds(ob)/DEE_UINT64_C(3600000))
#define DeeTime_GetTotalDays(ob)    (DeeTime_GetTotalMSeconds(ob)/DEE_UINT64_C(86400000))
#define DeeTime_GetTotalWeeks(ob)   (DeeTime_GetTotalMSeconds(ob)/DEE_UINT64_C(604800000))
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint64_t) DeeTime_GetTotalMonths(DEE_A_IN_OBJECT(DeeTimeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#define DeeTime_GetTotalYears   (Dee_uint64_t)DeeTime_GetYear

#define DeeTime_Compare(a,b)   (DeeTime_GetTotalMSeconds(b)-DeeTime_GetTotalMSeconds(a))
#define DeeTime_CompareLo(a,b) (DeeTime_GetTotalMSeconds(a)< DeeTime_GetTotalMSeconds(b))
#define DeeTime_CompareLe(a,b) (DeeTime_GetTotalMSeconds(a)<=DeeTime_GetTotalMSeconds(b))
#define DeeTime_CompareEq(a,b) (DeeTime_GetTotalMSeconds(a)==DeeTime_GetTotalMSeconds(b))
#define DeeTime_CompareNe(a,b) (DeeTime_GetTotalMSeconds(a)!=DeeTime_GetTotalMSeconds(b))
#define DeeTime_CompareGr(a,b) (DeeTime_GetTotalMSeconds(a)> DeeTime_GetTotalMSeconds(b))
#define DeeTime_CompareGe(a,b) (DeeTime_GetTotalMSeconds(a)>=DeeTime_GetTotalMSeconds(b))

#define /* Dee_uint64_t */DeeTime_GetBeforeCurrentYearMSeconds(self)     (DeeTime_Years2Days(DeeTime_GetYear(self))*DEE_UINT64_C(86400000))
#define /*         void */DeeTime_SetBeforeCurrentYearMSeconds(self,mseconds)                DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)(DeeTime_GetAfterCurrentYearMSeconds(self)+(Dee_int64_t)(mseconds)))
#define /* unsigned int */DeeTime_GetAfterCurrentYearMSeconds(self)          ((unsigned int)(DeeTime_GetTotalMSeconds(self)-DeeTime_GetBeforeCurrentYearMSeconds(self)))
#define /*         void */DeeTime_SetAfterCurrentYearMSeconds(self,mseconds)                 DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)((DeeTime_Years2Days(DeeTime_GetYear(self))*DEE_UINT64_C(86400000))+(Dee_int64_t)(mseconds)))
#define /* Dee_uint64_t */DeeTime_GetBeforeCurrentDayMSeconds(self)                         _DeeTime_GetAlignedMSeconds(self,DEE_UINT64_C(86400000))
#define /*         void */DeeTime_SetBeforeCurrentDayMSeconds(self,mseconds)                 DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)((DeeTime_GetTotalMSeconds(self)%DEE_UINT64_C(86400000))+(Dee_int64_t)(mseconds)))
#define /* unsigned int */DeeTime_GetAfterCurrentDayMSeconds(self)           ((unsigned int)(DeeTime_GetTotalMSeconds(self)%DEE_UINT64_C(86400000)))
#define /*         void */DeeTime_SetAfterCurrentDayMSeconds(self,mseconds)                  DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)(_DeeTime_GetAlignedMSeconds(self,DEE_UINT64_C(86400000))+(Dee_int64_t)(mseconds)))
#define /* Dee_uint64_t */_DeeTime_GetTimeMSeconds DeeTime_GetAfterCurrentDayMSeconds
#define /*         void */_DeeTime_SetTimeMSeconds DeeTime_SetAfterCurrentDayMSeconds
#define /* Dee_uint64_t */_DeeTime_GetDateMSeconds DeeTime_GetBeforeCurrentDayMSeconds
#define /*         void */_DeeTime_SetDateMSeconds DeeTime_SetBeforeCurrentDayMSeconds
//DEE_FUNC_DECL(void) _DeeTime_SetDateMSeconds(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN Dee_uint64_t mseconds) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Get / Set the date / time portions of a time object
#define DeeTime_GetTime(ob) DeeTime_New(_DeeTime_GetTimeMSeconds(ob))
#define DeeTime_GetDate(ob) DeeTime_New(_DeeTime_GetDateMSeconds(ob))
#define DeeTime_SetTime(self,hours,minutes,seconds,mseconds) _DeeTime_SetTimeMSeconds(self,\
 ((Dee_uint64_t)(hours)*DEE_UINT64_C(3600000))+((Dee_uint64_t)(minutes)*DEE_UINT64_C(60000))+\
 ((Dee_uint64_t)(seconds)*DEE_UINT64_C(1000))+((Dee_uint64_t)(mseconds)))
DEE_FUNC_DECL(void) DeeTime_SetDate(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN unsigned int years, DEE_A_IN unsigned int months, DEE_A_IN unsigned int days) DEE_ATTRIBUTE_NONNULL((1));
#define DeeTime_SetTimeObject(ob,right) _DeeTime_SetTimeMSeconds(ob,_DeeTime_GetTimeMSeconds(right))
#define DeeTime_SetDateObject(ob,right) _DeeTime_SetDateMSeconds(ob,_DeeTime_GetDateMSeconds(right))


//////////////////////////////////////////////////////////////////////////
// NOTE: When setting the time, you can put a value lower
//       than min or larger than max, to affect additional time channels. e.g.: 
//       >> #include <time>
//       >> time = time.now();
//       >> time.wday = -2;
//       >> print time; // Last friday at the current time
#define /* [0..999] unsigned int */DeeTime_GetMSecond(ob) (unsigned int)(DeeTime_GetTotalMSeconds(ob)%DEE_UINT64_C(1000))
#define /*                  void */DeeTime_SetMSecond(self,/*0..999*/v) DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)(_DeeTime_GetAlignedMSeconds(self,DEE_UINT64_C(1000)+(Dee_int64_t)(v))))
#define /* [0..59]  unsigned int */DeeTime_GetSecond(ob) (unsigned int)(DeeTime_GetTotalSeconds(ob)%DEE_UINT64_C(60))
#define /*                  void */DeeTime_SetSecond(self,/*0..59*/v)   DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)(_DeeTime_GetAlignedMSeconds(self,DEE_UINT64_C(60000))+((Dee_int64_t)(v)*DEE_INT64_C(1000))))
#define /* [0..59]  unsigned int */DeeTime_GetMinute(ob) (unsigned int)(DeeTime_GetTotalMinutes(ob)%DEE_UINT64_C(60))
#define /*                  void */DeeTime_SetMinute(self,/*0..59*/v)   DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)(_DeeTime_GetAlignedMSeconds(self,DEE_UINT64_C(3600000))+((Dee_int64_t)(v)*DEE_INT64_C(60000))))
#define /* [0..23]  unsigned int */DeeTime_GetHour(ob)   (unsigned int)(DeeTime_GetTotalHours(ob)%DEE_UINT64_C(24))
#define /*                  void */DeeTime_SetHour(self,/*0..23*/v)     DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)(_DeeTime_GetAlignedMSeconds(self,DEE_UINT64_C(86400000))+((Dee_int64_t)(v)*DEE_INT64_C(3600000))))
#define /* [0..6]   unsigned int */DeeTime_GetWDay(ob)   (unsigned int)(DeeTime_GetTotalDays(ob)%DEE_UINT64_C(7)) // NOTE: 0 == Sunday
#define /*                  void */DeeTime_SetWDay(self,/*0..6*/v)      DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)(_DeeTime_GetAlignedMSeconds(self,DEE_UINT64_C(604800000))+((Dee_int64_t)(v)*DEE_INT64_C(86400000))))
DEE_FUNC_DECL(DEE_A_RET_WUNUSED /*0..[27|28|29|30]*/unsigned int) DeeTime_GetMDay(DEE_A_IN_OBJECT(DeeTimeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#define /*                  void */DeeTime_SetMDay(self,/*0..(27|28|29|30)*/v) DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)((DeeTime_GetTotalMSeconds(self)-((Dee_uint64_t)DeeTime_GetMDay(self)*DEE_INT64_C(86400000)))+((Dee_int64_t)(v)*DEE_INT64_C(86400000))))
DEE_FUNC_DECL(DEE_A_RET_WUNUSED /*0..[365|366]*/unsigned int) DeeTime_GetYDay(DEE_A_IN_OBJECT(DeeTimeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#define /*                  void */DeeTime_SetYDay(self,/*0..[365|366]*/v) DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)((DeeTime_GetTotalMSeconds(self)-((Dee_uint64_t)DeeTime_GetYDay(self)*DEE_INT64_C(86400000)))+((Dee_int64_t)(v)*DEE_INT64_C(86400000))))
#define DeeTime_GetMWeek(ob) (DeeTime_GetMDay(ob)/7u)
#define /*                  void */DeeTime_SetMWeek(self,/*0..4*/v)     DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)((DeeTime_GetTotalMSeconds(self)-((Dee_uint64_t)DeeTime_GetMWeek(self)*DEE_INT64_C(604800000)))+((Dee_int64_t)(v)*DEE_INT64_C(604800000))))
//DEE_FUNC_DECL(void) DeeTime_SetMWeek(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN /*0..4*/int v) DEE_ATTRIBUTE_NONNULL((1));
#define DeeTime_GetYWeek(ob) (DeeTime_GetYDay(ob)/7u)
#define /*                  void */DeeTime_SetYWeek(self,/*0..55*/v)    DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)((DeeTime_GetTotalMSeconds(self)-((Dee_uint64_t)DeeTime_GetYWeek(self)*DEE_INT64_C(604800000)))+((Dee_int64_t)(v)*DEE_INT64_C(604800000))))
DEE_FUNC_DECL(DEE_A_RET_WUNUSED /*0..11*/unsigned int) DeeTime_GetMonth(DEE_A_IN_OBJECT(DeeTimeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeTime_SetMonth(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN /*0..11*/int v) DEE_ATTRIBUTE_NONNULL((1));
#define /* [0..~584542046] unsigned int */DeeTime_GetYear(self) ((unsigned int)DeeTime_Days2Years(DeeTime_GetTotalDays(self)))
#define /*                         void */DeeTime_SetYear(self,/*0..~584542046*/v) DeeTime_SetTotalMSeconds(self,(DeeTime_Years2Days(v)*DEE_UINT64_C(86400000))+DeeTime_GetBeforeCurrentYearMSeconds(self))

//DEE_FUNC_DECL(void) DeeTime_SetYear(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN /*0..~584542046*/unsigned int v) DEE_ATTRIBUTE_NONNULL((1));

#define DeeTime_FORMAT(ob,fmt)  DeeTime_Format(ob,fmt,(sizeof(fmt)/sizeof(char))-1)
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeStringObject) *) DeeTime_Format(
 DEE_A_IN_OBJECT(DeeTimeObject) const *self, DEE_A_IN_R(fmt_len) char const *fmt, DEE_A_IN Dee_size_t fmt_len) DEE_ATTRIBUTE_NONNULL((1,2));
#define DeeTime_FormatObject(ob,fmt) DeeTime_Format(ob,DeeString_STR(fmt),DeeString_SIZE(fmt))

//////////////////////////////////////////////////////////////////////////
// System clock access
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint64_t) DeeTime_GetClock1000(void); // Time in '1000/sec' (aka. milliseconds)
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint64_t) DeeTime_GetClockTick(void); // Time in 'DeeTime_GetClockFreq()/sec'
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint64_t) DeeTime_GetClockFreq(void); // Frequency for 'DeeTime_GetClockTick'

DEE_DECL_END

#endif /* !GUARD_DEEMON_TIME_H */
