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

#ifdef DEE_PRIVATE_DECL_DEE_TIMETICK_T
DEE_PRIVATE_DECL_DEE_TIMETICK_T /*< TODO: Fully integrate. */
#undef DEE_PRIVATE_DECL_DEE_TIMETICK_T
#endif

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
  Dee_timetick_t tm_msecs;  /*< kind == DeeTimeKind_DEFAULT: Milliseconds since 01.01.0000. */
  Dee_timetick_t tm_months; /*< kind == DeeTimeKind_MONTHS:  Months since 01.01.0000. */
  Dee_timetick_t tm_years;  /*< kind == DeeTimeKind_YEARS:   Years since 01.01.0000. */
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


//////////////////////////////////////////////////////////////////////////
// Convert between any two given time units

#define /* Dee_uint64_t */DeeTime_Mseconds2Seconds(/* Dee_uint64_t */n_mseconds) ((n_mseconds)/DEE_UINT64_C(1000))
#define /* Dee_uint64_t */DeeTime_Mseconds2Minutes(/* Dee_uint64_t */n_mseconds) ((n_mseconds)/DEE_UINT64_C(60000))
#define /* Dee_uint64_t */DeeTime_Mseconds2Hours(/* Dee_uint64_t */n_mseconds) ((n_mseconds)/DEE_UINT64_C(3600000))
#define /* Dee_uint64_t */DeeTime_Mseconds2Days(/* Dee_uint64_t */n_mseconds) ((n_mseconds)/DEE_UINT64_C(86400000))
#define /* Dee_uint64_t */DeeTime_Mseconds2Weeks(/* Dee_uint64_t */n_mseconds) ((n_mseconds)/DEE_UINT64_C(604800000))
#define /* Dee_uint64_t */DeeTime_Mseconds2Years(/* Dee_uint64_t */n_mseconds) DeeTime_Days2Years(DeeTime_Mseconds2Days(n_mseconds))

#define /* Dee_uint64_t */DeeTime_Seconds2Mseconds(/* Dee_uint64_t */n_seconds) ((n_seconds)*DEE_UINT64_C(1000))
#define /* Dee_uint64_t */DeeTime_Seconds2Minutes(/* Dee_uint64_t */n_seconds) ((n_seconds)/DEE_UINT64_C(60))
#define /* Dee_uint64_t */DeeTime_Seconds2Hours(/* Dee_uint64_t */n_seconds) ((n_seconds)/DEE_UINT64_C(3600))
#define /* Dee_uint64_t */DeeTime_Seconds2Days(/* Dee_uint64_t */n_seconds) ((n_seconds)/DEE_UINT64_C(86400))
#define /* Dee_uint64_t */DeeTime_Seconds2Weeks(/* Dee_uint64_t */n_seconds) ((n_seconds)/DEE_UINT64_C(604800))
#define /* Dee_uint64_t */DeeTime_Seconds2Years(/* Dee_uint64_t */n_seconds) DeeTime_Days2Years(DeeTime_Seconds2Days(n_seconds))

#define /* Dee_uint64_t */DeeTime_Minutes2Mseconds(/* Dee_uint64_t */n_minutes) ((n_minutes)*DEE_UINT64_C(60000))
#define /* Dee_uint64_t */DeeTime_Minutes2Seconds(/* Dee_uint64_t */n_minutes) ((n_minutes)*DEE_UINT64_C(60000))
#define /* Dee_uint64_t */DeeTime_Minutes2Hours(/* Dee_uint64_t */n_minutes) ((n_minutes)/DEE_UINT64_C(60))
#define /* Dee_uint64_t */DeeTime_Minutes2Days(/* Dee_uint64_t */n_minutes) ((n_minutes)/DEE_UINT64_C(1440))
#define /* Dee_uint64_t */DeeTime_Minutes2Weeks(/* Dee_uint64_t */n_minutes) ((n_minutes)/DEE_UINT64_C(10080))
#define /* Dee_uint64_t */DeeTime_Minutes2Years(/* Dee_uint64_t */n_minutes) DeeTime_Days2Years(DeeTime_Minutes2Days(n_minutes))

#define /* Dee_uint64_t */DeeTime_Hours2Mseconds(/* Dee_uint64_t */n_hours) ((n_hours)*DEE_UINT64_C(3600000))
#define /* Dee_uint64_t */DeeTime_Hours2Seconds(/* Dee_uint64_t */n_hours) ((n_hours)*DEE_UINT64_C(3600))
#define /* Dee_uint64_t */DeeTime_Hours2Minutes(/* Dee_uint64_t */n_hours) ((n_hours)*DEE_UINT64_C(60))
#define /* Dee_uint64_t */DeeTime_Hours2Days(/* Dee_uint64_t */n_hours) ((n_hours)/DEE_UINT64_C(24))
#define /* Dee_uint64_t */DeeTime_Hours2Weeks(/* Dee_uint64_t */n_hours) ((n_hours)/DEE_UINT64_C(168))
#define /* Dee_uint64_t */DeeTime_Hours2Years(/* Dee_uint64_t */n_hours) DeeTime_Days2Years(DeeTime_Hours2Days(n_minutes))

#define /* Dee_uint64_t */DeeTime_Days2Mseconds(/* Dee_uint64_t */n_days) ((n_days)*DEE_UINT64_C(86400000))
#define /* Dee_uint64_t */DeeTime_Days2Seconds(/* Dee_uint64_t */n_days) ((n_days)*DEE_UINT64_C(86400))
#define /* Dee_uint64_t */DeeTime_Days2Minutes(/* Dee_uint64_t */n_days) ((n_days)*DEE_UINT64_C(1440))
#define /* Dee_uint64_t */DeeTime_Days2Hours(/* Dee_uint64_t */n_days) ((n_days)*DEE_UINT64_C(24))
#define /* Dee_uint64_t */DeeTime_Days2Weeks(/* Dee_uint64_t */n_days) ((n_days)/DEE_UINT64_C(7))
#define /* Dee_uint64_t */DeeTime_Days2Years(/* Dee_uint64_t */n_days)  ((DEE_UINT64_C(400)*(/*(Dee_uint64_t)*/(n_days)+DEE_UINT64_C(1)))/DEE_UINT64_C(146097))

#define /* Dee_uint64_t */DeeTime_Weeks2Mseconds(/* Dee_uint64_t */n_weeks) ((n_weeks)*DEE_UINT64_C(604800000))
#define /* Dee_uint64_t */DeeTime_Weeks2Seconds(/* Dee_uint64_t */n_weeks) ((n_weeks)*DEE_UINT64_C(604800))
#define /* Dee_uint64_t */DeeTime_Weeks2Minutes(/* Dee_uint64_t */n_weeks) ((n_weeks)*DEE_UINT64_C(10080))
#define /* Dee_uint64_t */DeeTime_Weeks2Hours(/* Dee_uint64_t */n_weeks) ((n_weeks)*DEE_UINT64_C(168))
#define /* Dee_uint64_t */DeeTime_Weeks2Days(/* Dee_uint64_t */n_weeks) ((n_weeks)*DEE_UINT64_C(7))
#define /* Dee_uint64_t */DeeTime_Weeks2Years(/* Dee_uint64_t */n_weeks)  DeeTime_Days2Years(DeeTime_Weeks2Days(n_weeks))

#define /* Dee_uint64_t */DeeTime_Years2Mseconds(/* Dee_uint64_t */n_years) DeeTime_Days2Mseconds(DeeTime_Years2Days(n_years))
#define /* Dee_uint64_t */DeeTime_Years2Seconds(/* Dee_uint64_t */n_years) DeeTime_Days2Seconds(DeeTime_Years2Days(n_years))
#define /* Dee_uint64_t */DeeTime_Years2Minutes(/* Dee_uint64_t */n_years) DeeTime_Days2Minutes(DeeTime_Years2Days(n_years))
#define /* Dee_uint64_t */DeeTime_Years2Hours(/* Dee_uint64_t */n_years) DeeTime_Days2Hours(DeeTime_Years2Days(n_years))
#define /* Dee_uint64_t */DeeTime_Years2Weeks(/* Dee_uint64_t */n_years) DeeTime_Days2Weeks(DeeTime_Years2Days(n_years))
#define /* Dee_uint64_t */DeeTime_Years2Days(/* Dee_uint64_t */n_years) (((DEE_UINT64_C(146097)*/*(Dee_uint64_t)*/(n_years))/DEE_UINT64_C(400))/*-DEE_UINT64_C(1)*/) // rounding error?

#ifdef DEE_LIMITED_API /* v Just so you can recognize this constant as what it really is. */
DEE_STATIC_ASSERT(DEE_UINT64_C(62167132800000) == DeeTime_Years2Mseconds(1970));
#endif

#define /* Dee_time_t */DeeTime_Mseconds2TimeT(/* Dee_uint64_t */n_mseconds) ((Dee_time_t)(((n_mseconds)-DEE_UINT64_C(62167132800000))/DEE_UINT64_C(1000)))
#define /* Dee_uint64_t */DeeTime_TimeT2Mseconds(/* Dee_time_t */tmt)        (((Dee_uint64_t)(tmt)*DEE_UINT64_C(1000))+DEE_UINT64_C(62167132800000))

//////////////////////////////////////////////////////////////////////////
// Returns true if a given year is a leap year
// >> if (year%400 == 0) return 1; else
// >> if (year%100 == 0) return 0; else
// >> if (year%4 == 0) return 1;
// >> return 0;
#define DeeTime_IsLeapYear(year)\
 ((year)%400 == 0 || ((year)%100 != 0 && ((year)%4) == 0))

//////////////////////////////////////////////////////////////////////////
// Returns the total number of leap years between 0 and 'total_years'
#define DeeTime_CountLeapYears(total_years)\
 ((((total_years)/4)-(((total_years)/100)-((total_years)/400)))+1)



#define DeeTime_SetTick(self,tick) \
 (((DeeTimeObject *)Dee_REQUIRES_POINTER(self))->tm_diff_kind=DeeTimeKind_DEFAULT,\
  ((DeeTimeObject *)Dee_REQUIRES_POINTER(self))->tm_msecs=(tick))
#define DeeTime_GetTick DeeTime_GetTotalMSeconds

struct tm;
#define DeeTime_Copy(ob) DeeTime_New(DeeTime_GetTick(ob))
#define DeeTime_NewFromData(year,month,mday,hour,minute,second,msecond) \
 DeeTime_New(DeeTimeTick_FromData(year,month,mday,hour,minute,second,msecond))

#define DeeTime_AsLocalTimeT(ob,t) (*(t)=DeeTime_Mseconds2TimeT(DeeTime_GetTotalMSeconds(ob)),0)
#define DeeTime_NewFromLocalTimeT(t) DeeTime_New(DeeTime_TimeT2Mseconds(t))
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *) DeeTime_NewFromTimeT(DEE_A_IN Dee_time_t t);
#define DeeTime_NewFromTm(t) DeeTime_New(DeeTimeTick_FromTm(t))
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *) DeeTime_NewFromYears(DEE_A_IN Dee_timetick_t years);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *) DeeTime_NewFromMonths(DEE_A_IN Dee_timetick_t months);
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *) DeeTime_New(DEE_A_IN Dee_timetick_t ticks);
#define DeeTime_NewFromWeeks(n_weeks)       DeeTime_New((Dee_timetick_t)DeeTime_Weeks2Mseconds(n_weeks))
#define DeeTime_NewFromDays(n_days)         DeeTime_New((Dee_timetick_t)DeeTime_Days2Mseconds(n_days))
#define DeeTime_NewFromHours(n_hours)       DeeTime_New((Dee_timetick_t)DeeTime_Hours2Mseconds(n_hours))
#define DeeTime_NewFromMinutes(n_minutes)   DeeTime_New((Dee_timetick_t)DeeTime_Minutes2Mseconds(n_minutes))
#define DeeTime_NewFromSeconds(n_seconds)   DeeTime_New((Dee_timetick_t)DeeTime_Seconds2Mseconds(n_seconds))
#define DeeTime_NewFromMSeconds(m_mseconds) DeeTime_New((Dee_timetick_t)(n_mseconds))


//////////////////////////////////////////////////////////////////////////
// Returns the current time
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *) DeeTime_Now(void);

//////////////////////////////////////////////////////////////////////////
// Returns the current time in internal time ticks (mseconds since 01.01.0000)
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeTimeTick_Now(DEE_A_OUT Dee_uint64_t *ticks) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Create a time object from explicitly stated data
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint64_t) DeeTimeTick_FromData(
 DEE_A_IN unsigned int year, DEE_A_IN unsigned int month,
 DEE_A_IN unsigned int mday, DEE_A_IN unsigned int hour,
 DEE_A_IN unsigned int minute, DEE_A_IN unsigned int second,
 DEE_A_IN unsigned int msecond);

//////////////////////////////////////////////////////////////////////////
// Create/Cast a time object from time_t / struct tm
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeTimeTick_FromTimeT(DEE_A_IN Dee_time_t t, DEE_A_OUT Dee_uint64_t *ticks) DEE_ATTRIBUTE_NONNULL((2));
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint64_t) DeeTimeTick_FromTm(DEE_A_IN struct tm const *t) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeTime_AsTimeT(DEE_A_IN_OBJECT(DeeTimeObject) const *self, DEE_A_OUT Dee_time_t *t) DEE_ATTRIBUTE_NONNULL((1,2));
DEE_FUNC_DECL(DEE_A_RET_EXCEPT(-1) int) DeeTime_AsTm(DEE_A_IN_OBJECT(DeeTimeObject) const *self, DEE_A_OUT struct tm *t) DEE_ATTRIBUTE_NONNULL((1,2));

//////////////////////////////////////////////////////////////////////////
// Create/Cast a time object from proprietary win32 time formats
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
#ifdef DEE_LIMITED_API
DEE_STATIC_ASSERT(DEE_UINT64_C(50522659200000) == DeeTime_Years2Mseconds(1601));
#endif
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

//////////////////////////////////////////////////////////////////////////
// Inplace add more time
DEE_FUNC_DECL(void) DeeTime_InplaceAddMSeconds(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN Dee_int64_t n_mseconds) DEE_ATTRIBUTE_NONNULL((1));
#define DeeTime_InplaceAddSeconds(self,n_seconds) DeeTime_InplaceAddMSeconds(self,DeeTime_Seconds2Mseconds(n_seconds))
#define DeeTime_InplaceAddMinutes(self,n_minutes) DeeTime_InplaceAddMSeconds(self,DeeTime_Minutes2Mseconds(n_minutes))
#define DeeTime_InplaceAddHours(self,n_hours)     DeeTime_InplaceAddMSeconds(self,DeeTime_Hours2Mseconds(n_hours))
#define DeeTime_InplaceAddDays(self,n_days)       DeeTime_InplaceAddMSeconds(self,DeeTime_Days2Mseconds(n_days))
#define DeeTime_InplaceAddWeeks(self,n_weeks)     DeeTime_InplaceAddMSeconds(self,DeeTime_Weeks2Mseconds(n_weeks))
DEE_FUNC_DECL(void) DeeTime_InplaceAddMonths(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN Dee_int64_t n_months) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeTime_InplaceAddYears(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN Dee_int64_t n_years) DEE_ATTRIBUTE_NONNULL((1));

//////////////////////////////////////////////////////////////////////////
// Inplace subtract time
#define DeeTime_InplaceSubMSeconds(self,n_mseconds) DeeTime_InplaceAddMSeconds(self,-(Dee_int64_t)(n_mseconds))
#define DeeTime_InplaceSubSeconds(self,n_seconds)   DeeTime_InplaceAddSeconds(self,-(Dee_int64_t)(n_seconds))
#define DeeTime_InplaceSubMinutes(self,n_minutes)   DeeTime_InplaceAddMinutes(self,-(Dee_int64_t)(n_minutes))
#define DeeTime_InplaceSubHours(self,n_hours)       DeeTime_InplaceAddHours(self,-(Dee_int64_t)(n_hours))
#define DeeTime_InplaceSubDays(self,n_days)         DeeTime_InplaceAddDays(self,-(Dee_int64_t)(n_days))
#define DeeTime_InplaceSubWeeks(self,n_weeks)       DeeTime_InplaceAddWeeks(self,-(Dee_int64_t)(n_weeks))
#define DeeTime_InplaceSubMonths(self,n_months)     DeeTime_InplaceAddMonths(self,-(Dee_int64_t)(n_months))
#define DeeTime_InplaceSubYears(self,n_years)       DeeTime_InplaceAddYears(self,-(Dee_int64_t)(n_years))

//////////////////////////////////////////////////////////////////////////
// Get the total amount of time
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint64_t) DeeTime_GetTotalMSeconds(DEE_A_IN_OBJECT(DeeTimeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#define DeeTime_GetTotalSeconds(ob) (DeeTime_GetTotalMSeconds(ob)/DEE_UINT64_C(1000))
#define DeeTime_GetTotalMinutes(ob) (DeeTime_GetTotalMSeconds(ob)/DEE_UINT64_C(60000))
#define DeeTime_GetTotalHours(ob)   (DeeTime_GetTotalMSeconds(ob)/DEE_UINT64_C(3600000))
#define DeeTime_GetTotalDays(ob)    (DeeTime_GetTotalMSeconds(ob)/DEE_UINT64_C(86400000))
#define DeeTime_GetTotalWeeks(ob)   (DeeTime_GetTotalMSeconds(ob)/DEE_UINT64_C(604800000))
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint64_t) DeeTime_GetTotalMonths(DEE_A_IN_OBJECT(DeeTimeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#define DeeTime_GetTotalYears   (Dee_uint64_t)DeeTime_GetYear

//////////////////////////////////////////////////////////////////////////
// Set the total amount of time
#define DeeTime_SetTotalMSeconds(ob,mseconds) (((DeeTimeObject *)Dee_REQUIRES_POINTER(ob))->tm_diff_kind=DeeTimeKind_DEFAULT,((DeeTimeObject *)(ob))->tm_msecs=(mseconds))
#define DeeTime_SetTotalSeconds(ob,seconds)   (((DeeTimeObject *)Dee_REQUIRES_POINTER(ob))->tm_diff_kind=DeeTimeKind_DEFAULT,((DeeTimeObject *)(ob))->tm_msecs=DeeTime_Seconds2Mseconds(seconds))
#define DeeTime_SetTotalMinutes(ob,minutes)   (((DeeTimeObject *)Dee_REQUIRES_POINTER(ob))->tm_diff_kind=DeeTimeKind_DEFAULT,((DeeTimeObject *)(ob))->tm_msecs=DeeTime_Minutes2Mseconds(minutes))
#define DeeTime_SetTotalHours(ob,hours)       (((DeeTimeObject *)Dee_REQUIRES_POINTER(ob))->tm_diff_kind=DeeTimeKind_DEFAULT,((DeeTimeObject *)(ob))->tm_msecs=DeeTime_Hours2Mseconds(hours))
#define DeeTime_SetTotalDays(ob,days)         (((DeeTimeObject *)Dee_REQUIRES_POINTER(ob))->tm_diff_kind=DeeTimeKind_DEFAULT,((DeeTimeObject *)(ob))->tm_msecs=DeeTime_Days2Mseconds(days))
#define DeeTime_SetTotalWeeks(ob,weeks)       (((DeeTimeObject *)Dee_REQUIRES_POINTER(ob))->tm_diff_kind=DeeTimeKind_DEFAULT,((DeeTimeObject *)(ob))->tm_msecs=DeeTime_Weeks2Mseconds(weeks))
#define DeeTime_SetTotalMonths(ob,months)     (((DeeTimeObject *)Dee_REQUIRES_POINTER(ob))->tm_diff_kind=DeeTimeKind_MONTHS,((DeeTimeObject *)(ob))->tm_months=(months))
#define DeeTime_SetTotalYears(ob,years)       (((DeeTimeObject *)Dee_REQUIRES_POINTER(ob))->tm_diff_kind=DeeTimeKind_YEARS,((DeeTimeObject *)(ob))->tm_years=(years))

#define _DeeTime_GetAlignedMSeconds(self,alignment) ((DeeTime_GetTotalMSeconds(self)/(alignment))*(alignment))

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
#define /*                  void */DeeTime_SetMSecond(self,/*0..999*/v)  DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)(_DeeTime_GetAlignedMSeconds(self,DEE_UINT64_C(1000)+(Dee_int64_t)(v))))
#define /* [0..59]  unsigned int */DeeTime_GetSecond(ob)  (unsigned int)(DeeTime_GetTotalSeconds(ob)%DEE_UINT64_C(60))
#define /*                  void */DeeTime_SetSecond(self,/*0..59*/v)    DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)(_DeeTime_GetAlignedMSeconds(self,DEE_UINT64_C(60000))+((Dee_int64_t)(v)*DEE_INT64_C(1000))))
#define /* [0..59]  unsigned int */DeeTime_GetMinute(ob)  (unsigned int)(DeeTime_GetTotalMinutes(ob)%DEE_UINT64_C(60))
#define /*                  void */DeeTime_SetMinute(self,/*0..59*/v)    DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)(_DeeTime_GetAlignedMSeconds(self,DEE_UINT64_C(3600000))+((Dee_int64_t)(v)*DEE_INT64_C(60000))))
#define /* [0..23]  unsigned int */DeeTime_GetHour(ob)    (unsigned int)(DeeTime_GetTotalHours(ob)%DEE_UINT64_C(24))
#define /*                  void */DeeTime_SetHour(self,/*0..23*/v)      DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)(_DeeTime_GetAlignedMSeconds(self,DEE_UINT64_C(86400000))+((Dee_int64_t)(v)*DEE_INT64_C(3600000))))
#define /* [0..6]   unsigned int */DeeTime_GetWDay(ob)    (unsigned int)(DeeTime_GetTotalDays(ob)%DEE_UINT64_C(7)) // NOTE: 0 == Sunday
#define /*                  void */DeeTime_SetWDay(self,/*0..6*/v)       DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)(_DeeTime_GetAlignedMSeconds(self,DEE_UINT64_C(604800000))+((Dee_int64_t)(v)*DEE_INT64_C(86400000))))
DEE_FUNC_DECL(DEE_A_RET_WUNUSED /*0..[27|28|29|30]*/unsigned int) DeeTime_GetMDay(DEE_A_IN_OBJECT(DeeTimeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#define /*                  void */DeeTime_SetMDay(self,/*0..(27|28|29|30)*/v) DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)((DeeTime_GetTotalMSeconds(self)-((Dee_uint64_t)DeeTime_GetMDay(self)*DEE_INT64_C(86400000)))+((Dee_int64_t)(v)*DEE_INT64_C(86400000))))
DEE_FUNC_DECL(DEE_A_RET_WUNUSED /*0..[365|366]*/unsigned int) DeeTime_GetYDay(DEE_A_IN_OBJECT(DeeTimeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
#define /*                  void */DeeTime_SetYDay(self,/*0..[365|366]*/v) DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)((DeeTime_GetTotalMSeconds(self)-((Dee_uint64_t)DeeTime_GetYDay(self)*DEE_INT64_C(86400000)))+((Dee_int64_t)(v)*DEE_INT64_C(86400000))))
#define DeeTime_GetMWeek(ob) (DeeTime_GetMDay(ob)/7u)
#define /*                  void */DeeTime_SetMWeek(self,/*0..4*/v)      DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)((DeeTime_GetTotalMSeconds(self)-((Dee_uint64_t)DeeTime_GetMWeek(self)*DEE_INT64_C(604800000)))+((Dee_int64_t)(v)*DEE_INT64_C(604800000))))
//DEE_FUNC_DECL(void) DeeTime_SetMWeek(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN /*0..4*/int v) DEE_ATTRIBUTE_NONNULL((1));
#define DeeTime_GetYWeek(ob) (DeeTime_GetYDay(ob)/7u)
#define /*                  void */DeeTime_SetYWeek(self,/*0..55*/v)     DeeTime_SetTotalMSeconds(self,(Dee_uint64_t)((DeeTime_GetTotalMSeconds(self)-((Dee_uint64_t)DeeTime_GetYWeek(self)*DEE_INT64_C(604800000)))+((Dee_int64_t)(v)*DEE_INT64_C(604800000))))
DEE_FUNC_DECL(DEE_A_RET_WUNUSED /*0..11*/unsigned int) DeeTime_GetMonth(DEE_A_IN_OBJECT(DeeTimeObject) const *self) DEE_ATTRIBUTE_NONNULL((1));
DEE_FUNC_DECL(void) DeeTime_SetMonth(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN /*0..11*/int v) DEE_ATTRIBUTE_NONNULL((1));
#define /* [0..~584542046] unsigned int */DeeTime_GetYear(self) ((unsigned int)DeeTime_Days2Years(DeeTime_GetTotalDays(self)))
#define /*                         void */DeeTime_SetYear(self,/*0..~584542046*/v) DeeTime_SetTotalMSeconds(self,(DeeTime_Years2Days(v)*DEE_UINT64_C(86400000))+DeeTime_GetBeforeCurrentYearMSeconds(self))


//////////////////////////////////////////////////////////////////////////
// Format a given time using a strftime-style format string
// NOTE: Since I really think that the format modifiers for strftime are just total garbage,
//       and only designed to be short, but not readable, I've added a new, extended modifier
//       that is meant to fix this:
//       >> print time.now().format("%[S:WD], the %[n:D] of %[S:M] %[Y], %[2:H]:%[2:MI]:%[2:S]");
//       -> Tuesday, the 8th of November 2016, 17:45:38
//       attribute ::= ('year'|'years'|'month'|'months'|'mweek'|'yweek'|
//                      'wday'|'mday'|'yday'|'days'|'hour'|'hours'|'minute'|
//                      'minutes'|'second'|'seconds'|'msecond'|'mseconds'|
//                      'Y'|'M'|'MW'|'YW'|'WD'|'MD'|'D'|'YD'|'H'|'MI'|'I'|'S'|'MS');
//       exftime ::= '%[' ## [('S'|'s'|(['n'|' '] ## ('0'..'9')##...)] ## ':')] ## attribute ## ']';
//       - The format modifier always starts with '%[' and ends with ']', with the actual text in-between
//       - Optional representation prefix (before ':'):
//         - 's': Short representation of the attribute (only allowed for 'wday' and 'month')
//         - 'S': Long representation of the attribute (only allowed for 'wday' and 'month')
//         - 'n': nth representation of the attribute (can be prefixed infront of width modifier; (1st, 2nd, 3rd, 4th, 5th, ...))
//         - ' ': Fill empty space created by a large width modifier with ' ' instead of '0'
//       - Optional width prefix (before ':'):
//         - Only allowed without a representation prefix, the 'n' or ' ' prefix.
//       - Attribute name:
//         - One of the names listed above, this part describes which attribute to referr to.
//         - The attributes match the member names of the time object, with the following aliases provided:
//           - 'Y'      --> 'year'
//           - 'M'      --> 'month'
//           - 'MW'     --> 'mweek'
//           - 'YW'     --> 'yweek'
//           - 'WD'     --> 'wday'
//           - 'MD'|'D' --> 'mday'
//           - 'YD'     --> 'yday'
//           - 'H'      --> 'hour'
//           - 'MI'|'I' --> 'minute'
//           - 'S'      --> 'second'
//           - 'MS'     --> 'msecond'
//       - If the format-string is malformed, such as referring to an unknown
//         attribute, an Error.ValueError is thrown and the function fails.
DEE_FUNC_DECL(DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeStringObject) *) DeeTime_Format(
 DEE_A_IN_OBJECT(DeeTimeObject) const *self, DEE_A_IN_Z char const *fmt) DEE_ATTRIBUTE_NONNULL((1,2));
#define DeeTime_FormatObject(self,fmt) DeeTime_Format(self,DeeString_STR(fmt))

//////////////////////////////////////////////////////////////////////////
// System clock access
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint64_t) DeeTime_GetClock1000(void); // Time in '1000/sec' (aka. milliseconds)
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint64_t) DeeTime_GetClockTick(void); // Time in 'DeeTime_GetClockFreq()/sec'
DEE_FUNC_DECL(DEE_A_RET_WUNUSED Dee_uint64_t) DeeTime_GetClockFreq(void); // Frequency for 'DeeTime_GetClockTick'

DEE_DECL_END

#endif /* !GUARD_DEEMON_TIME_H */
