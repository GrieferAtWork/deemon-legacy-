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
#ifndef GUARD_DEEMON_CXX_TIME_H
#define GUARD_DEEMON_CXX_TIME_H 1

#include <deemon/__conf.inl>
#include <deemon/cxx/__features.inl>
#include <deemon/time.h>
#include <deemon/cxx/object.h>
#include <deemon/cxx/string.h>

#if DEE_HAVE_CXX_API
DEE_DECL_CXX_BEGIN
namespace deemon {

struct time: object {
 public: /* public methods */
  static DEE_A_RET_WUNUSED time now() { return time(::DeeTime_Now(),detail::tag_ref_or_err()); }
  static DEE_A_RET_WUNUSED Dee_uint64_t tick() DEE_CXX11_NOEXCEPT { return ::DeeTime_GetClockTick(); }
  static DEE_A_RET_WUNUSED Dee_uint64_t freq() DEE_CXX11_NOEXCEPT { return ::DeeTime_GetClockFreq(); }
  static DEE_A_RET_WUNUSED time from_time_t(DEE_A_IN Dee_time_t tmt) { return time(::DeeTime_NewFromTimeT(tmt),detail::tag_ref_or_err()); }
  static DEE_A_RET_WUNUSED time _time(DEE_A_IN unsigned int hour) { return time(::DeeTime_NewFromData(0,0,0,hour,0,0,0),detail::tag_ref_or_err()); }
  static DEE_A_RET_WUNUSED time _time(DEE_A_IN unsigned int hour, DEE_A_IN unsigned int minute) { return time(::DeeTime_NewFromData(0,0,0,hour,minute,0,0),detail::tag_ref_or_err()); }
  static DEE_A_RET_WUNUSED time _time(DEE_A_IN unsigned int hour, DEE_A_IN unsigned int minute, DEE_A_IN unsigned int second) { return time(::DeeTime_NewFromData(0,0,0,hour,minute,second,0),detail::tag_ref_or_err()); }
  static DEE_A_RET_WUNUSED time _time(DEE_A_IN unsigned int hour, DEE_A_IN unsigned int minute, DEE_A_IN unsigned int second, DEE_A_IN unsigned int msecond) { return time(::DeeTime_NewFromData(0,0,0,hour,minute,second,msecond),detail::tag_ref_or_err()); }
  static DEE_A_RET_WUNUSED time _date(DEE_A_IN unsigned int year) { return time(::DeeTime_NewFromData(year,0,0,0,0,0,0),detail::tag_ref_or_err()); }
  static DEE_A_RET_WUNUSED time _date(DEE_A_IN unsigned int year, DEE_A_IN unsigned int month) { return time(::DeeTime_NewFromData(year,month,0,0,0,0,0),detail::tag_ref_or_err()); }
  static DEE_A_RET_WUNUSED time _date(DEE_A_IN unsigned int year, DEE_A_IN unsigned int month, DEE_A_IN unsigned int day) { return time(::DeeTime_NewFromData(year,month,day,0,0,0,0),detail::tag_ref_or_err()); }
  static DEE_A_RET_WUNUSED time _years(DEE_A_IN Dee_uint64_t years_) { return time(::DeeTime_NewFromYears(years_),detail::tag_ref_or_err()); }
  static DEE_A_RET_WUNUSED time _months(DEE_A_IN Dee_uint64_t months_) { return time(::DeeTime_NewFromMonths(months_),detail::tag_ref_or_err()); }
  static DEE_A_RET_WUNUSED time _weeks(DEE_A_IN Dee_uint64_t weeks_) { return time(::DeeTime_NewFromWeeks(weeks_),detail::tag_ref_or_err()); }
  static DEE_A_RET_WUNUSED time _days(DEE_A_IN Dee_uint64_t days_) { return time(::DeeTime_NewFromDays(days_),detail::tag_ref_or_err()); }
  static DEE_A_RET_WUNUSED time _hours(DEE_A_IN Dee_uint64_t hours_) { return time(::DeeTime_NewFromHours(hours_),detail::tag_ref_or_err()); }
  static DEE_A_RET_WUNUSED time _minutes(DEE_A_IN Dee_uint64_t minutes_) { return time(::DeeTime_NewFromMinutes(minutes_),detail::tag_ref_or_err()); }
  static DEE_A_RET_WUNUSED time _seconds(DEE_A_IN Dee_uint64_t seconds_) { return time(::DeeTime_NewFromSeconds(seconds_),detail::tag_ref_or_err()); }
  static DEE_A_RET_WUNUSED time _mseconds(DEE_A_IN Dee_uint64_t mseconds_) { return time(::DeeTime_NewFromMSeconds(mseconds_),detail::tag_ref_or_err()); }
 public: /* public methods */
  inline string format(DEE_A_IN_Z char const *fmt) { return string(::DeeTime_Format(this->ob_ptr,fmt),detail::tag_ref_or_err()); }
 private:
  struct _proxy_year; struct _proxy_years;
  struct _proxy_month; struct _proxy_months;
  struct _proxy_mweek; struct _proxy_yweek; struct _proxy_weeks;
  struct _proxy_wday; struct _proxy_mday; struct _proxy_yday; struct _proxy_days;
  struct _proxy_hour; struct _proxy_hours;
  struct _proxy_minute; struct _proxy_minutes;
  struct _proxy_second; struct _proxy_seconds;
  struct _proxy_msecond; struct _proxy_mseconds;
  struct _proxy_time; struct _proxy_date;
 public: /* public methods */
#define DEE_TIME_DEFINE_PROXY_GETTER(name)\
  inline _proxy_##name const &name() const DEE_CXX11_NOEXCEPT { return reinterpret_cast< _proxy_##name const &>(*this); }
  DEE_TIME_DEFINE_PROXY_GETTER(year);
  DEE_TIME_DEFINE_PROXY_GETTER(years);
  DEE_TIME_DEFINE_PROXY_GETTER(month);
  DEE_TIME_DEFINE_PROXY_GETTER(months);
  DEE_TIME_DEFINE_PROXY_GETTER(mweek);
  DEE_TIME_DEFINE_PROXY_GETTER(yweek);
  DEE_TIME_DEFINE_PROXY_GETTER(weeks);
  DEE_TIME_DEFINE_PROXY_GETTER(wday);
  DEE_TIME_DEFINE_PROXY_GETTER(mday);
  DEE_TIME_DEFINE_PROXY_GETTER(yday);
  DEE_TIME_DEFINE_PROXY_GETTER(days);
  DEE_TIME_DEFINE_PROXY_GETTER(hour);
  DEE_TIME_DEFINE_PROXY_GETTER(hours);
  DEE_TIME_DEFINE_PROXY_GETTER(minute);
  DEE_TIME_DEFINE_PROXY_GETTER(minutes);
  DEE_TIME_DEFINE_PROXY_GETTER(second);
  DEE_TIME_DEFINE_PROXY_GETTER(seconds);
  DEE_TIME_DEFINE_PROXY_GETTER(msecond);
  DEE_TIME_DEFINE_PROXY_GETTER(mseconds);
#undef DEE_TIME_DEFINE_PROXY_GETTER
  inline _proxy_date const &date_() const DEE_CXX11_NOEXCEPT { return reinterpret_cast< _proxy_date const &>(*this); }
  inline _proxy_time const &time_() const DEE_CXX11_NOEXCEPT { return reinterpret_cast< _proxy_time const &>(*this); }
 public: /* public constructor / destructor */
  inline time(): object(::DeeTime_Now(),detail::tag_ref_or_err()) {}
  explicit inline time(DEE_A_OUT ::Dee_time_t *t): object(::DeeTime_Now(),detail::tag_ref_or_err()) { if (DeeTime_AsTimeT(this->ob_ptr,t) == -1) detail::throw_last_error(); }
  explicit inline time(DEE_A_IN unsigned int year): object(::DeeTime_NewFromData(year,0,0,0,0,0,0),detail::tag_ref_or_err()) {}
  explicit inline time(DEE_A_IN unsigned int year, DEE_A_IN unsigned int month): object(::DeeTime_NewFromData(year,month,0,0,0,0,0),detail::tag_ref_or_err()) {}
  explicit inline time(DEE_A_IN unsigned int year, DEE_A_IN unsigned int month, DEE_A_IN unsigned int mday): object(::DeeTime_NewFromData(year,month,mday,0,0,0,0),detail::tag_ref_or_err()) {}
  explicit inline time(DEE_A_IN unsigned int year, DEE_A_IN unsigned int month, DEE_A_IN unsigned int mday, DEE_A_IN unsigned int hour): object(::DeeTime_NewFromData(year,month,mday,hour,0,0,0),detail::tag_ref_or_err()) {}
  explicit inline time(DEE_A_IN unsigned int year, DEE_A_IN unsigned int month, DEE_A_IN unsigned int mday, DEE_A_IN unsigned int hour, DEE_A_IN unsigned int minute): object(::DeeTime_NewFromData(year,month,mday,hour,minute,0,0),detail::tag_ref_or_err()) {}
  explicit inline time(DEE_A_IN unsigned int year, DEE_A_IN unsigned int month, DEE_A_IN unsigned int mday, DEE_A_IN unsigned int hour, DEE_A_IN unsigned int minute, DEE_A_IN unsigned int second): object(::DeeTime_NewFromData(year,month,mday,hour,minute,second,0),detail::tag_ref_or_err()) {}
  explicit inline time(DEE_A_IN unsigned int year, DEE_A_IN unsigned int month, DEE_A_IN unsigned int mday, DEE_A_IN unsigned int hour, DEE_A_IN unsigned int minute, DEE_A_IN unsigned int second, DEE_A_IN unsigned int msecond): object(::DeeTime_NewFromData(year,month,mday,hour,minute,second,msecond),detail::tag_ref_or_err()) {}
 DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(time,object,DeeTime_Check)
};
#define DEE_TIME_DEFINE_PROXY(Tget,Tset,name,get,set)\
struct time::name {\
 private: DeeObject *ob;\
 DEE_CXX_DELETE_CLASSDEFAULT(name);\
 inline name const &operator = (DEE_A_IN Tset v) const { set(this->ob,v); return *this; }\
 inline operator Tget () const { return get(this->ob); }\
 inline void del() const { set(this->ob,0); }\
}
DEE_TIME_DEFINE_PROXY(unsigned int,unsigned int,_proxy_year,DeeTime_GetYear,DeeTime_SetYear);
DEE_TIME_DEFINE_PROXY(Dee_uint64_t,Dee_uint64_t,_proxy_years,DeeTime_GetTotalYears,DeeTime_SetTotalYears);
DEE_TIME_DEFINE_PROXY(unsigned int,int,_proxy_month,DeeTime_GetMonth,DeeTime_SetMonth);
DEE_TIME_DEFINE_PROXY(Dee_uint64_t,Dee_uint64_t,_proxy_months,DeeTime_GetTotalMonths,DeeTime_SetTotalMonths);
DEE_TIME_DEFINE_PROXY(unsigned int,int,_proxy_mweek,DeeTime_GetMWeek,DeeTime_SetMWeek);
DEE_TIME_DEFINE_PROXY(unsigned int,int,_proxy_yweek,DeeTime_GetYWeek,DeeTime_SetYWeek);
DEE_TIME_DEFINE_PROXY(Dee_uint64_t,Dee_uint64_t,_proxy_weeks,DeeTime_GetTotalWeeks,DeeTime_SetTotalWeeks);
DEE_TIME_DEFINE_PROXY(unsigned int,int,_proxy_wday,DeeTime_GetWDay,DeeTime_SetWDay);
DEE_TIME_DEFINE_PROXY(unsigned int,int,_proxy_mday,DeeTime_GetMDay,DeeTime_SetMDay);
DEE_TIME_DEFINE_PROXY(unsigned int,int,_proxy_yday,DeeTime_GetYDay,DeeTime_SetYDay);
DEE_TIME_DEFINE_PROXY(Dee_uint64_t,Dee_uint64_t,_proxy_days,DeeTime_GetTotalDays,DeeTime_SetTotalDays);
DEE_TIME_DEFINE_PROXY(unsigned int,int,_proxy_hour,DeeTime_GetHour,DeeTime_SetHour);
DEE_TIME_DEFINE_PROXY(Dee_uint64_t,Dee_uint64_t,_proxy_hours,DeeTime_GetTotalHours,DeeTime_SetTotalHours);
DEE_TIME_DEFINE_PROXY(unsigned int,int,_proxy_minute,DeeTime_GetMinute,DeeTime_SetMinute);
DEE_TIME_DEFINE_PROXY(Dee_uint64_t,Dee_uint64_t,_proxy_minutes,DeeTime_GetTotalMinutes,DeeTime_SetTotalMinutes);
DEE_TIME_DEFINE_PROXY(unsigned int,int,_proxy_second,DeeTime_GetSecond,DeeTime_SetSecond);
DEE_TIME_DEFINE_PROXY(Dee_uint64_t,Dee_uint64_t,_proxy_seconds,DeeTime_GetTotalSeconds,DeeTime_SetTotalSeconds);
DEE_TIME_DEFINE_PROXY(unsigned int,int,_proxy_msecond,DeeTime_GetMSecond,DeeTime_SetMSecond);
DEE_TIME_DEFINE_PROXY(Dee_uint64_t,Dee_uint64_t,_proxy_mseconds,DeeTime_GetTotalMSeconds,DeeTime_SetTotalMSeconds);
#undef DEE_TIME_DEFINE_PROXY
struct time::_proxy_time {
 private: DeeObject *ob;
 DEE_CXX_DELETE_CLASSDEFAULT(_proxy_time);
 inline operator time () const { return time(DeeTime_GetTime(this->ob),detail::tag_ref_or_err()); }
 inline _proxy_time const &operator = (time const &tm_) const { DeeTime_SetTimeObject(this->ob,tm_.priv_get()); }
 inline void del() const { _DeeTime_SetTimeMSeconds(this->ob,0); }
};
struct time::_proxy_date {
 private: DeeObject *ob;
 DEE_CXX_DELETE_CLASSDEFAULT(_proxy_date);
 inline operator time () const { return time(DeeTime_GetDate(this->ob),detail::tag_ref_or_err()); }
 inline _proxy_time const &operator = (time const &tm_) const { DeeTime_SetDateObject(this->ob,tm_.priv_get()); }
 inline void del() const { _DeeTime_SetDateMSeconds(this->ob,0); }
};


}
DEE_DECL_CXX_END
#endif

#endif /* !GUARD_DEEMON_CXX_TIME_H */
