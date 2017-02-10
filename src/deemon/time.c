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
#ifndef GUARD_DEEMON_TIME_C
#define GUARD_DEEMON_TIME_C 1
#define DEE_LIMITED_API 1

#include <deemon/__conf.inl>
#include DEE_INCLUDE_MEMORY_API()

// include/*
#include <deemon/__bswap_intrin.inl>
#include <deemon/bool.h>
#include <deemon/error.h>
#include <deemon/file.h>
#include <deemon/integer.h>
#include <deemon/memberdef.h>
#include <deemon/marshal_data.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/atomic_once.h>
#include <deemon/string.h>
#include <deemon/structured.h>
#include <deemon/time.h>
#include <deemon/tuple.h>

// src/*
#include <deemon/posix_features.inl>

#include DEE_INCLUDE_MEMORY_API_DISABLE()
#ifdef DEE_PLATFORM_WINDOWS
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#elif DEE_ENVIRONMENT_HAVE_INCLUDE_UNISTD_H
#include <unistd.h>
#endif
#include DEE_INCLUDE_MEMORY_API_ENABLE()

// */ (nano...)

DEE_DECL_BEGIN
DEE_COMPILER_MSVC_WARNING_PUSH(4061)
DEE_STATIC_ASSERT(sizeof(Dee_time_t) == DEE_TYPES_SIZEOF_TIME_T);


#ifdef _MSC_VER
#define localtime_r(t,tt) ((localtime_s(tt,t)==0)?(tt):NULL)
#elif !DEE_HAVE_LOCALTIME_R
static struct tm *localtime_r(time_t const *t, struct tm *tt) {
 struct tm *tt2 = localtime(t);
 if DEE_UNLIKELY(!tt2) return NULL;
 *tt = *tt2;
 return tt;
}
#endif



static char const abbr_month_names[12][4] = {
 "Jan","Feb","Mar","Apr","May","Jun",
 "Jul","Aug","Sep","Oct","Nov","Dec"};
static char const *full_month_names[12] = {
 "January","February","March","April","May","June",
 "July","August","September","October","November","December"};
static char const abbr_wday_names[7][4] = {
 "Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
static char const *full_wday_names[7] = {
 "Sunday","Monday","Tuesday","Wednesday",
 "Thursday","Friday","Saturday"};
static char const am_pm[2][3] = {"AM","PM"};
static char const am_pm_lower[2][3] = {"am","pm"};


static DeeStringWriterFormatSpec const two_digits_zero_spec = {
 10, // numsys
 0,  // width
 2,  // precision
 0,  // has_width
 1,  // has_precision
 0,  // left_just
 0,  // sign_pos
 0,  // sign_space
 0,  // prefix
 1,  // pad_zero
 0,  // upper_hex
};
static DeeStringWriterFormatSpec const three_digits_zero_spec = {
 10, // numsys
 0,  // width
 3,  // precision
 0,  // has_width
 1,  // has_precision
 0,  // left_just
 0,  // sign_pos
 0,  // sign_space
 0,  // prefix
 1,  // pad_zero
 0,  // upper_hex
};
static DeeStringWriterFormatSpec const two_digits_space_spec = {
 10, // numsys
 0,  // width
 2,  // precision
 0,  // has_width
 1,  // has_precision
 0,  // left_just
 0,  // sign_pos
 0,  // sign_space
 0,  // prefix
 0,  // pad_zero
 0,  // upper_hex
};

#define DEE_TIMEATTRID_NONE     0
#define DEE_TIMEATTRID_YEAR     1
#define DEE_TIMEATTRID_YEARS    DEE_TIMEATTRID_YEAR
#define DEE_TIMEATTRID_MONTH    2
#define DEE_TIMEATTRID_MONTHS   3
#define DEE_TIMEATTRID_MWEEK    4
#define DEE_TIMEATTRID_YWEEK    5
#define DEE_TIMEATTRID_WDAY     6
#define DEE_TIMEATTRID_MDAY     7
#define DEE_TIMEATTRID_YDAY     8
#define DEE_TIMEATTRID_DAYS     9
#define DEE_TIMEATTRID_HOUR     10
#define DEE_TIMEATTRID_HOURS    11
#define DEE_TIMEATTRID_MINUTE   12
#define DEE_TIMEATTRID_MINUTES  13
#define DEE_TIMEATTRID_SECOND   14
#define DEE_TIMEATTRID_SECONDS  15
#define DEE_TIMEATTRID_MSECOND  16
#define DEE_TIMEATTRID_MSECONDS 17

//////////////////////////////////////////////////////////////////////////
// Returns the mode id of a given format attribute name
// >> Returns 'DEE_TIMEATTRID_NONE' if the given name is unknown
DEE_STATIC_INLINE(int) DeeTime_GetAttributeID(
 char const *name_begin, Dee_size_t name_len) {
 DEE_ASSERT(!name_len || name_begin);
 switch (name_len) {
  case 1:
   switch (*name_begin) {
    case 'Y': return DEE_TIMEATTRID_YEAR;
    case 'M': return DEE_TIMEATTRID_MONTH;
    case 'D': return DEE_TIMEATTRID_MDAY;
    case 'H': return DEE_TIMEATTRID_HOUR;
    case 'I': return DEE_TIMEATTRID_MINUTE;
    case 'S': return DEE_TIMEATTRID_SECOND;
    default: break;
   }
   break;
  case 2:
   if (*name_begin == 'M') {
    switch (name_begin[1]) {
     case 'W': return DEE_TIMEATTRID_MWEEK;
     case 'D': return DEE_TIMEATTRID_MDAY;
     case 'I': return DEE_TIMEATTRID_MINUTE;
     case 'S': return DEE_TIMEATTRID_MSECOND;
     default: break;
    }
   } else if (*name_begin == 'Y') {
    if (name_begin[1] == 'W') return DEE_TIMEATTRID_YWEEK;
    else if (name_begin[1] == 'D') return DEE_TIMEATTRID_YDAY;
   } else if (*name_begin == 'W' && name_begin[1] == 'D') {
    return DEE_TIMEATTRID_WDAY;
   }
   break;

#define CHECK(n,name,mode) if (memcmp(name_begin,name,(n)*sizeof(char))==0) return mode
  case 4:
   CHECK(4,"year",DEE_TIMEATTRID_YEAR);
   CHECK(4,"wday",DEE_TIMEATTRID_WDAY);
   CHECK(4,"mday",DEE_TIMEATTRID_MDAY);
   CHECK(4,"yday",DEE_TIMEATTRID_YDAY);
   CHECK(4,"days",DEE_TIMEATTRID_DAYS);
   CHECK(4,"hour",DEE_TIMEATTRID_HOUR);
   break;
  case 5:
   CHECK(5,"years",DEE_TIMEATTRID_YEARS);
   CHECK(5,"month",DEE_TIMEATTRID_MONTH);
   CHECK(5,"mweek",DEE_TIMEATTRID_MWEEK);
   CHECK(5,"yweek",DEE_TIMEATTRID_YWEEK);
   CHECK(5,"hours",DEE_TIMEATTRID_HOURS);
   break;
  case 6:
   CHECK(6,"months",DEE_TIMEATTRID_MONTHS);
   CHECK(6,"minute",DEE_TIMEATTRID_MINUTE);
   CHECK(6,"second",DEE_TIMEATTRID_SECOND);
   break;
  case 7:
   CHECK(7,"minutes",DEE_TIMEATTRID_MINUTES);
   CHECK(7,"seconds",DEE_TIMEATTRID_SECONDS);
   CHECK(7,"msecond",DEE_TIMEATTRID_MSECOND);
   break;
  case 8:
   CHECK(8,"mseconds",DEE_TIMEATTRID_MSECONDS);
   break;
#undef CHECK
  default: break;
 }
 return DEE_TIMEATTRID_NONE;
}

DEE_STATIC_INLINE(DEE_A_RET_WUNUSED Dee_uint64_t) DeeTime_GetAttributeValue(
 DEE_A_IN_OBJECT(DeeTimeObject) const *self, DEE_A_IN int attrib) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTime_Check(self));
 switch (attrib) {
  case DEE_TIMEATTRID_YEAR     : return DeeTime_GetYear(self);
  case DEE_TIMEATTRID_MONTH    : return DeeTime_GetMonth(self);
  case DEE_TIMEATTRID_MONTHS   : return DeeTime_GetTotalMonths(self);
  case DEE_TIMEATTRID_MWEEK    : return DeeTime_GetMWeek(self);
  case DEE_TIMEATTRID_YWEEK    : return DeeTime_GetYWeek(self);
  case DEE_TIMEATTRID_WDAY     : return DeeTime_GetWDay(self);
  case DEE_TIMEATTRID_MDAY     : return DeeTime_GetMDay(self);
  case DEE_TIMEATTRID_YDAY     : return DeeTime_GetYDay(self);
  case DEE_TIMEATTRID_DAYS     : return DeeTime_GetTotalDays(self);
  case DEE_TIMEATTRID_HOUR     : return DeeTime_GetHour(self);
  case DEE_TIMEATTRID_HOURS    : return DeeTime_GetTotalHours(self);
  case DEE_TIMEATTRID_MINUTE   : return DeeTime_GetMinute(self);
  case DEE_TIMEATTRID_MINUTES  : return DeeTime_GetTotalMinutes(self);
  case DEE_TIMEATTRID_SECOND   : return DeeTime_GetSecond(self);
  case DEE_TIMEATTRID_SECONDS  : return DeeTime_GetTotalSeconds(self);
  case DEE_TIMEATTRID_MSECOND  : return DeeTime_GetMSecond(self);
  case DEE_TIMEATTRID_MSECONDS : return DeeTime_GetTotalMSeconds(self);
  default: DEE_BUILTIN_UNREACHABLE();
 }
}



DEE_A_RET_OBJECT_EXCEPT_REF(struct DeeStringObject) *DeeTime_Format(
 DEE_A_IN_OBJECT(DeeTimeObject) const *self, DEE_A_IN_Z char const *fmt) {
#if 1
 DeeObject *result; char ch;
 DeeStringWriter writer = DeeStringWriter_INIT();
 DEE_ASSERT(DeeObject_Check(self) && DeeTime_Check(self));
 while (1) {
  switch ((ch = *fmt++)) {
   case '\0': goto done;
   case '%':
    switch ((ch = *fmt++)) {
//TODO: @begin locale_dependent
     case 'a': if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING      (&writer,abbr_wday_names[DeeTime_GetWDay(self)]) != 0) goto err; break;
     case 'A': if DEE_UNLIKELY(DeeStringWriter_WriteString(&writer,full_wday_names[DeeTime_GetWDay(self)]) != 0) goto err; break;
     case 'h':
     case 'b': if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING      (&writer,abbr_month_names[DeeTime_GetMonth(self)]) != 0) goto err; break;
     case 'B': if DEE_UNLIKELY(DeeStringWriter_WriteString(&writer,full_month_names[DeeTime_GetMonth(self)]) != 0) goto err; break;
     case 'c': if DEE_UNLIKELY(DeeStringWriter_Writef(&writer,"%s %s %0.2u %0.2u:%0.2u:%0.2u %u",
      abbr_wday_names[DeeTime_GetWDay(self)],abbr_month_names[DeeTime_GetMonth(self)],DeeTime_GetMDay(self)+1,
      DeeTime_GetHour(self),DeeTime_GetMinute(self),DeeTime_GetSecond(self),DeeTime_GetYear(self)
      ) != 0) goto err; break;
     case 'x': if DEE_UNLIKELY(DeeStringWriter_Writef(&writer,"%0.2u/%0.2u/%0.2u",
      DeeTime_GetMonth(self)+1,DeeTime_GetMDay(self)+1,DeeTime_GetYear(self)
      ) != 0) goto err; break;
     case 'X': if DEE_UNLIKELY(DeeStringWriter_Writef(&writer,"%0.2u:%0.2u:%0.2u",
      DeeTime_GetHour(self),DeeTime_GetMinute(self),DeeTime_GetSecond(self)
      ) != 0) goto err; break;
     case 'z': break; // TODO: ISO 8601 offset from UTC in timezone (1 minute=1, 1 hour=100) | If timezone cannot be determined, no characters	+100
     case 'Z': break; // TODO: Timezone name or abbreviation * | If timezone cannot be determined, no characters	CDT
//TODO: @end locale_dependent
#define DeeStringWriter_SPEC_WRITE_UINT \
 DEE_PP_CAT_2(DeeStringWriter_SpecWriteUInt,DEE_PP_MUL8(DEE_TYPES_SIZEOF_INT))
     case 'C': if DEE_UNLIKELY(DeeStringWriter_SPEC_WRITE_UINT(&writer,(DeeTime_GetYear(self)/100)%100,&two_digits_zero_spec) != 0) goto err; break;
     case 'd': if DEE_UNLIKELY(DeeStringWriter_SPEC_WRITE_UINT(&writer,DeeTime_GetMDay(self)+1,&two_digits_zero_spec) != 0) goto err; break;
     case 'D': if DEE_UNLIKELY(DeeStringWriter_Writef(&writer,"%0.2u/%0.2u/%0.2u",
      DeeTime_GetMonth(self)+1,DeeTime_GetMDay(self)+1,
      (unsigned int)(DeeTime_GetYear(self)%100)
      ) != 0) goto err; break;
     case 'e': if DEE_UNLIKELY(DeeStringWriter_SPEC_WRITE_UINT(&writer,DeeTime_GetMDay(self)+1,&two_digits_space_spec) != 0) goto err; break;
     case 'F': if DEE_UNLIKELY(DeeStringWriter_Writef(&writer,"%0.4u-%0.2u-%0.2u",
      DeeTime_GetYear(self),DeeTime_GetMonth(self)+1,DeeTime_GetMDay(self)+1
      ) != 0) goto err; break;
     case 'H': if DEE_UNLIKELY(DeeStringWriter_SPEC_WRITE_UINT(&writer,DeeTime_GetHour(self),&two_digits_zero_spec) != 0) goto err; break;
     case 'I': if DEE_UNLIKELY(DeeStringWriter_SPEC_WRITE_UINT(&writer,DeeTime_GetHour(self)%12,&two_digits_zero_spec) != 0) goto err; break;
     case 'j': if DEE_UNLIKELY(DeeStringWriter_SPEC_WRITE_UINT(&writer,DeeTime_GetYDay(self)+1,&three_digits_zero_spec) != 0) goto err; break;
     case 'm': if DEE_UNLIKELY(DeeStringWriter_SPEC_WRITE_UINT(&writer,DeeTime_GetMonth(self)+1,&two_digits_zero_spec) != 0) goto err; break;
     case 'M': if DEE_UNLIKELY(DeeStringWriter_SPEC_WRITE_UINT(&writer,DeeTime_GetMinute(self),&two_digits_zero_spec) != 0) goto err; break;
     case 'p': if DEE_UNLIKELY(DeeStringWriter_WRITE_STRING(&writer,am_pm[DeeTime_GetHour(self)/12]) != 0) goto err; break;
     case 'r': if DEE_UNLIKELY(DeeStringWriter_Writef(&writer,"%0.2u:%0.2u:%0.2u %s",
      DeeTime_GetHour(self)%12,DeeTime_GetMinute(self),DeeTime_GetSecond(self),
      am_pm_lower[DeeTime_GetHour(self)/12]
      ) != 0) goto err; break;
     case 'R': if DEE_UNLIKELY(DeeStringWriter_Writef(&writer,"%0.2u:%0.2u",
      DeeTime_GetHour(self),DeeTime_GetMinute(self)
      ) != 0) goto err; break;
     case 'S': if DEE_UNLIKELY(DeeStringWriter_SPEC_WRITE_UINT(&writer,DeeTime_GetSecond(self),&two_digits_zero_spec) != 0) goto err; break;
     case 'T': if DEE_UNLIKELY(DeeStringWriter_Writef(&writer,"%0.2u:%0.2u:%0.2u",
      DeeTime_GetHour(self),DeeTime_GetMinute(self),DeeTime_GetSecond(self)
      ) != 0) goto err; break;
     case 'u': if DEE_UNLIKELY(DeeStringWriter_SPEC_WRITE_UINT(&writer,1u+((DeeTime_GetWDay(self)+6u)%7u),&two_digits_zero_spec) != 0) goto err; break;
     case 'w': if DEE_UNLIKELY(DeeStringWriter_SPEC_WRITE_UINT(&writer,DeeTime_GetWDay(self),&two_digits_zero_spec) != 0) goto err; break;
     case 'y': if DEE_UNLIKELY(DeeStringWriter_SPEC_WRITE_UINT(&writer,DeeTime_GetYear(self)%100,&two_digits_zero_spec) != 0) goto err; break;
     case 'Y': if DEE_UNLIKELY(DeeStringWriter_SPEC_WRITE_UINT(&writer,DeeTime_GetYear(self),&dee_decimal_default_spec) != 0) goto err; break;
#undef DeeStringWriter_SPEC_WRITE_UINT

// I don't understand this week-based stuff.
// I read the wikipedia article, but I still don't really get it.
// >> So this might be supported in the future when I understand it...
// %g	Week-based year, last two digits (00-99)	01
// %G	Week-based year	2001
// %U	Week number with the first Sunday as the first day of week one (00-53)	33
// %V	ISO 8601 week number (00-53)	34
// %W	Week number with the first Monday as the first day of week one (00-53)	34

     case 'n': if DEE_UNLIKELY(DeeStringWriter_WriteChar(&writer,'\n') != 0) goto err; break;
     case 't': if DEE_UNLIKELY(DeeStringWriter_WriteChar(&writer,'\t') != 0) goto err; break;
     case '%': case '\0':
      if DEE_UNLIKELY(DeeStringWriter_WriteChar(&writer,'%') != 0) goto err;
      if (!ch) goto done;
      break;

     case '[': {
      char const *tag_begin,*tag_end,*mode_begin,*mode_end;
      unsigned int bracket_recursion = 1; Dee_uint64_t attribval;
      int repr_mode,attribute_id,width = 0;
      // Extended formatting
      mode_end = mode_begin = tag_begin = fmt;
      while (1) {
       ch = *fmt++;
       if (ch == ']') { if (!--bracket_recursion) { tag_end = fmt-1; break; } }
       else if (ch == '[') ++bracket_recursion;
       else if (ch == ':' && bracket_recursion == 1)
        mode_end = fmt-1,tag_begin = fmt;
       else if (!ch) { tag_end = fmt; break; }
      }
      if (mode_begin != mode_end) {
       if (*mode_begin == 'n' || *mode_begin == 's' ||
           *mode_begin == 'S' || *mode_begin == ' ')
        repr_mode = *mode_begin++;
       else repr_mode = 0;
       // Parse the width modifier
       while (mode_begin != mode_end) {
        if (*mode_begin >= '0' && *mode_begin <= '9') {
         width = width*10+(*mode_begin-'0');
        } else {
         DeeError_SetStringf(&DeeErrorType_ValueError,
                             "Expected digits, or end of repr-mode, but got %.1q in '%%[%#.*q]'",
                             mode_begin,(unsigned)(tag_end-mode_begin),mode_begin);
         goto err;
        }
        ++mode_begin;
       }
      } else repr_mode = 0;
      if DEE_UNLIKELY((attribute_id = DeeTime_GetAttributeID(
       tag_begin,(Dee_size_t)(tag_end-tag_begin))) == DEE_TIMEATTRID_NONE) {
       DeeError_SetStringf(&DeeErrorType_ValueError,
                           "Unknown/Invalid time attribute: %.*q",
                           (unsigned)(tag_end-tag_begin),tag_begin);
       goto err;
      }
      attribval = DeeTime_GetAttributeValue(self,attribute_id);
      switch (repr_mode) {
       case 's':
       case 'S': {
        char const *repr_value;
        if (attribute_id == DEE_TIMEATTRID_MONTH) {
         DEE_ASSERT(attribval < 12);
         repr_value = repr_mode == 'S' ? full_month_names[attribval] : abbr_month_names[attribval];
        } else if (attribute_id == DEE_TIMEATTRID_WDAY) {
         DEE_ASSERT(attribval < 7);
         repr_value = repr_mode == 'S' ? full_wday_names[attribval] : abbr_wday_names[attribval];
        } else {
         DeeError_SetStringf(&DeeErrorType_ValueError,
                             "Cannot use attribute %.*q with 's'/'S' in '%%[%#.*q]'",
                             (unsigned)(tag_end-tag_begin),tag_begin,
                             (unsigned)(tag_end-mode_begin),mode_begin);
         goto err;
        }
        if (DeeStringWriter_WriteString(&writer,repr_value) != 0) goto err;
       } break;
       default: {
        static char const _suffix_values[] = "st" "nd" "rd" "th";
        struct DeeStringWriterFormatSpec fmt_spec = DeeStringWriterFormatSpec_INIT_BASIC(10);
        if (width) {
         fmt_spec.has_width = 1;
         fmt_spec.width = (unsigned int)width;
         if (repr_mode != ' ') fmt_spec.pad_zero = 1;
        }
        if (DeeStringWriter_SpecWriteUInt64(&writer,attribval,&fmt_spec) != 0) goto err;
        if (repr_mode == 'n') {
         Dee_size_t suffix_offset = (attribval >= 3 ? (Dee_size_t)3 : (Dee_size_t)attribval)*2;
         if (DeeStringWriter_WriteStringWithLength(
          &writer,2,_suffix_values+suffix_offset) != 0) goto err;
        }
       } break;
      }
     } break;
     default:
      if DEE_UNLIKELY(DeeStringWriter_WriteStringWithLength(&writer,2,fmt-2) != 0) goto err;
      break;
    }
    break;
   default:/*def:*/
    if DEE_UNLIKELY(DeeStringWriter_WriteChar(&writer,ch) != 0) goto err;
    break;
  }
 }
done:
 result = DeeStringWriter_Pack(&writer);
end: DeeStringWriter_Quit(&writer);
 return result;
err: result = NULL; goto end;
#else
 (void)fmt,fmt_len;
 return DeeString_Newf("%0.2u/%0.2u/%0.4u - %0.2u:%0.2u:%0.2u:%0.3u",
                       DeeTime_GetMDay(self)+1,DeeTime_GetMonth(self)+1,DeeTime_GetYear(self),
                       DeeTime_GetHour(self),DeeTime_GetMinute(self),DeeTime_GetSecond(self),
                       DeeTime_GetMSecond(self));
#endif
}






#define year_month(y,m) month_lengths[DeeTime_IsLeapYear(y)]
static unsigned int const month_lengths[2][12] = {
 {31,28,31,30,31,30,31,31,30,31,30,31},
 {31,29,31,30,31,30,31,31,30,31,30,31}};
static unsigned int const year_day_total[2][12] = {
 {31,59,90,120,151,181,212,243,273,304,334,365},
 {31,60,91,121,152,182,213,244,274,305,335,366}};
static unsigned int const month_start_yday[2][12] = {
 {0,31,59,90,120,151,181,212,243,273,304,334},
 {0,31,60,91,121,152,182,213,244,274,305,335}};

#define msecs_per_day   DEE_UINT64_C(86400000)

#define _DeeTime_YearsToMseconds(years) \
 (DeeTime_Years2Days(years)*msecs_per_day)
static Dee_uint64_t _DeeTime_MonthsToMseconds(Dee_uint64_t months) {
 Dee_uint64_t years = months/12;
 Dee_uint64_t days = DeeTime_Years2Days(years);
 days += month_start_yday[DeeTime_IsLeapYear(years)][months%12];
 return days*msecs_per_day;
}

static void _DeeTime_InplaceDefault(DeeTimeObject *self) {
 switch (self->tm_diff_kind) {
  case DeeTimeKind_MONTHS:
   self->tm_msecs = _DeeTime_MonthsToMseconds(self->tm_months);
   self->tm_diff_kind = DeeTimeKind_DEFAULT;
   break;
  case DeeTimeKind_YEARS:
   self->tm_years = _DeeTime_YearsToMseconds(self->tm_years);
   self->tm_diff_kind = DeeTimeKind_DEFAULT;
   break;
  default: break;
 }
}


#define DeeTime_NEW(mseconds) _DeeTime_Alloc(DeeTimeKind_DEFAULT,mseconds)
DEE_STATIC_INLINE(DeeObject *) _DeeTime_Alloc(Dee_uint16_t kind, Dee_uint64_t data) {
 DeeTimeObject *result;
 if DEE_LIKELY((result = DeeObject_MALLOCF(
  DeeTimeObject,"time object %d",kind)) != NULL) {
  DeeObject_INIT(result,&DeeTime_Type);
  result->tm_diff_kind = kind;
  result->tm_msecs = data;
 }
 return (DeeObject *)result;
}

DEE_A_RET_EXCEPT(-1) int DeeTime_AsTimeT(
 DEE_A_IN_OBJECT(DeeTimeObject) const *self, DEE_A_OUT Dee_time_t *t) {
 struct tm tmtime; DEE_ASSERT(t);
 if DEE_UNLIKELY(DeeTime_AsTm(self,&tmtime) != 0) return -1;
 *t = mktime(&tmtime);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeTime_AsTm(
 DEE_A_IN_OBJECT(DeeTimeObject) const *self, DEE_A_OUT struct tm *t) {
 DEE_ASSERT(t);
 DEE_ASSERT(DeeObject_Check(self) && DeeTime_Check(self));
 t->tm_sec = (int)DeeTime_GetSecond(self);
 t->tm_min = (int)DeeTime_GetMinute(self);
 t->tm_hour = (int)DeeTime_GetHour(self);
 t->tm_mday = (int)DeeTime_GetMDay(self);
 t->tm_mon = (int)DeeTime_GetMonth(self);
 t->tm_year = (int)DeeTime_GetYear(self)-1900;
 t->tm_wday = (int)DeeTime_GetWDay(self);
 t->tm_yday = (int)DeeTime_GetYDay(self);
 //t->tm_isdst; // shouldn't matter
 return 0;
}

DEE_A_RET_WUNUSED Dee_uint64_t DeeTimeTick_FromData(
 DEE_A_IN unsigned int year, DEE_A_IN unsigned int month,
 DEE_A_IN unsigned int mday, DEE_A_IN unsigned int hour,
 DEE_A_IN unsigned int minute, DEE_A_IN unsigned int second,
 DEE_A_IN unsigned int msecond) {
 unsigned int used_year = year+month/12;
 return ((DeeTime_Years2Days(used_year)+
  month_start_yday[DeeTime_IsLeapYear(used_year)][month%12]+mday)*msecs_per_day)+
 ((Dee_uint64_t)hour*DEE_UINT64_C(3600000))+
 ((Dee_uint64_t)minute*DEE_UINT64_C(60000))+
 ((Dee_uint64_t)second*DEE_UINT64_C(1000))+
 ((Dee_uint64_t)msecond);
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *
DeeTime_NewFromTimeT(DEE_A_IN Dee_time_t t) {
 Dee_uint64_t ticks;
 if DEE_UNLIKELY(DeeTimeTick_FromTimeT(t,&ticks) != 0) return NULL;
 return DeeTime_New(ticks);
}
DEE_A_RET_EXCEPT(-1) int DeeTimeTick_FromTimeT(
 DEE_A_IN Dee_time_t t, DEE_A_OUT Dee_uint64_t *ticks) {
 struct tm tmtime;
 if DEE_UNLIKELY(localtime_r((time_t*)&t,&tmtime) == NULL) {
  DeeError_SystemError("localtime_r");
  return -1;
 }
 *ticks = DeeTimeTick_FromTm(&tmtime);
 return 0;
}
DEE_A_RET_WUNUSED Dee_uint64_t DeeTimeTick_FromTm(DEE_A_IN struct tm const *t) {
 return DeeTimeTick_FromData((unsigned int)(t->tm_year+1900),
                             (unsigned int)t->tm_mon,
                             (unsigned int)(t->tm_mday-1),
                             (unsigned int)t->tm_hour,
                             (unsigned int)t->tm_min,
                             (unsigned int)t->tm_sec,
                             (unsigned int)0);
}

DEE_A_RET_EXCEPT(-1) int DeeTimeTick_Now(DEE_A_OUT Dee_uint64_t *ticks) {
#if defined(DEE_PLATFORM_WINDOWS)
 SYSTEMTIME cur_time;
 DEE_ASSERT(ticks);
 GetLocalTime(&cur_time);
 *ticks = DeeTimeTick_FromWin32SystemTime(&cur_time);
 return 0;
#else
 struct tm ltm;
 time_t t;
 DEE_ASSERT(ticks);
 time(&t);
 if (localtime_r(&t,&ltm) == NULL) {
  DeeError_SystemError("localtime_r");
  return -1;
 }
 *ticks = DeeTimeTick_FromTm(&ltm);
 return 0;
#endif
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *DeeTime_Now(void) {
 Dee_uint64_t ticks;
 if (DeeTimeTick_Now(&ticks) != 0) return NULL;
 return DeeTime_New(ticks);
}
#ifdef DEE_PLATFORM_WINDOWS
DEE_A_RET_EXCEPT(-1) int DeeTime_AsWin32FileTime(
 DEE_A_IN_OBJECT(DeeTimeObject) const *self, DEE_A_OUT struct _FILETIME *t) {
 FILETIME temp;
 DEE_ASSERT(DeeObject_Check(self) && DeeTime_Check(self));
 DEE_ASSERT(t);
 DeeTime_AsWin32LocalFileTime(self,&temp);
 if DEE_UNLIKELY(!LocalFileTimeToFileTime(&temp,t)) {
  DeeError_SystemError("LocalFileTimeToFileTime");
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeTime_AsWin32SystemTime(
 DEE_A_IN_OBJECT(DeeTimeObject) const *self, DEE_A_OUT struct _SYSTEMTIME *t) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTime_Check(self));
 DEE_ASSERT(t);
 t->wYear = (WORD)DeeTime_GetYear(self);
 t->wMonth = (WORD)(1+DeeTime_GetMonth(self));
 t->wDay = (WORD)(1+DeeTime_GetMDay(self));
 t->wHour = (WORD)DeeTime_GetHour(self);
 t->wMinute = (WORD)DeeTime_GetMinute(self);
 t->wSecond = (WORD)DeeTime_GetSecond(self);
 t->wMilliseconds = (WORD)DeeTime_GetMSecond(self);
 t->wDayOfWeek = (WORD)DeeTime_GetWDay(self);
 return 0;
}
DEE_A_RET_WUNUSED Dee_uint64_t DeeTimeTick_FromWin32SystemTime(
 DEE_A_IN struct _SYSTEMTIME const *t) {
 DEE_ASSERT(t);
 return DeeTimeTick_FromData((unsigned int)t->wYear,
                             (unsigned int)(t->wMonth-1),
                             (unsigned int)(t->wDay-1),
                             (unsigned int)t->wHour,
                             (unsigned int)t->wMinute,
                             (unsigned int)t->wSecond,
                             (unsigned int)t->wMilliseconds);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *DeeTime_NewFromWin32DosDateTime(
 DEE_A_IN Dee_uint16_t fat_date, DEE_A_IN Dee_uint16_t fat_time) {
 FILETIME f_time;
 if DEE_UNLIKELY(!DosDateTimeToFileTime(fat_date,fat_time,&f_time)) {
  DeeError_SystemError("DosDateTimeToFileTime");
  return NULL;
 }
 return DeeTime_NewFromWin32FileTime(&f_time);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *
DeeTime_NewFromWin32FileTime(DEE_A_IN struct _FILETIME const *t) {
 FILETIME local_t;
 if DEE_UNLIKELY(!FileTimeToLocalFileTime(t,&local_t)) {
  DeeError_SystemError("FileTimeToLocalFileTime");
  return NULL;
 }
 return DeeTime_NewFromWin32LocalFileTime(&local_t);
}
#endif /* DEE_PLATFORM_WINDOWS */




DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *
DeeTime_NewFromYears(DEE_A_IN Dee_timetick_t years) {
 return _DeeTime_Alloc(DeeTimeKind_YEARS,years);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *
DeeTime_NewFromMonths(DEE_A_IN Dee_timetick_t months) {
 return _DeeTime_Alloc(DeeTimeKind_MONTHS,months);
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeTimeObject) *
DeeTime_New(DEE_A_IN Dee_timetick_t ticks) {
 return DeeTime_NEW(ticks);
}


DEE_A_RET_WUNUSED Dee_timetick_t DeeTime_GetTotalMonths(
 DEE_A_IN_OBJECT(DeeTimeObject) const *self) {
 // TODO: What about the current year?
 return DeeTime_Days2Years(DeeTime_GetTotalDays(self))*12;
}
DEE_A_RET_WUNUSED unsigned int DeeTime_GetMDay(
 DEE_A_IN_OBJECT(DeeTimeObject) const *self) {
 unsigned int yday = DeeTime_GetYDay(self);
 unsigned int month = DeeTime_GetMonth(self);
 unsigned int year = DeeTime_GetYear(self);
 return (unsigned int)(yday-month_start_yday[DeeTime_IsLeapYear(year)][month]);
}
DEE_A_RET_WUNUSED unsigned int DeeTime_GetYDay(
 DEE_A_IN_OBJECT(DeeTimeObject) const *self) {
 Dee_uint64_t total_days = DeeTime_GetTotalDays(self);
 Dee_uint64_t years = DeeTime_Days2Years(total_days);
 Dee_uint64_t year_days = DeeTime_Years2Days(years);
 return (unsigned int)(total_days-year_days);
}

void DeeTime_SetMonth(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN /*0..11*/int v) {
 unsigned int year; int mmonth;
 _DeeTime_InplaceDefault((DeeTimeObject *)self);
 if ((mmonth = v % 12) < 0) mmonth += 12,v += mmonth; else v -= mmonth;
 year = DeeTime_GetYear(self);
 DeeTime_SetYDay(self,(int)DeeTime_GetMDay(self)+
                 (int)month_start_yday[DeeTime_IsLeapYear(year)][mmonth]);
 if (v) DeeTime_SetYear(self,year+(v/12));
}
DEE_A_RET_WUNUSED unsigned int DeeTime_GetMonth(
 DEE_A_IN_OBJECT(DeeTimeObject) const *self) {
 unsigned int result = 0;
 unsigned int yday = DeeTime_GetYDay(self);
 unsigned int year = DeeTime_GetYear(self);
 unsigned int const *day_total = year_day_total[DeeTime_IsLeapYear(year)];
 // Search for the first month that includes yday
 while (yday >= day_total[result]) ++result;
 return result;
}


DEE_A_RET_WUNUSED Dee_uint64_t DeeTime_GetTotalMSeconds(
 DEE_A_IN_OBJECT(DeeTimeObject) const *self) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTime_Check(self));
 switch (((DeeTimeObject *)self)->tm_diff_kind) {
  case DeeTimeKind_MONTHS: return _DeeTime_MonthsToMseconds(((DeeTimeObject *)self)->tm_months);
  case DeeTimeKind_YEARS: return _DeeTime_YearsToMseconds(((DeeTimeObject *)self)->tm_years);
  default: break;
 }
 return ((DeeTimeObject *)self)->tm_msecs;
}


/*
void DeeTime_SetMSecond(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN / *0..999* /int v) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTime_Check(self));
 _DeeTime_InplaceDefault((DeeTimeObject *)self);
 msecs = (msecs-(msecs % DEE_UINT64_C(1000)))+(Dee_int64_t)v;
}
void DeeTime_SetSecond(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN / *0..59* /int v) {
 _DeeTime_InplaceDefault((DeeTimeObject *)self);
 msecs = (msecs-(msecs % DEE_UINT64_C(60000)))+((Dee_int64_t)v*DEE_INT64_C(1000));
}
void DeeTime_SetMinute(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN / *0..59* /int v) {
 _DeeTime_InplaceDefault((DeeTimeObject *)self);
 msecs = (msecs-(msecs % DEE_UINT64_C(3600000)))+((Dee_int64_t)v*DEE_INT64_C(60000));
}
void DeeTime_SetHour(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN / *0..23* /int v) {
 _DeeTime_InplaceDefault((DeeTimeObject *)self);
 msecs = (msecs-(msecs % DEE_UINT64_C(86400000)))+((Dee_int64_t)v*DEE_INT64_C(3600000));
}
void DeeTime_SetWDay(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN / *0..6* /int v) {
 _DeeTime_InplaceDefault((DeeTimeObject *)self);
 msecs = (msecs-(msecs % DEE_UINT64_C(604800000)))+((Dee_int64_t)v*DEE_INT64_C(86400000));
}
void DeeTime_SetMDay(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN / *0..[27,28,29,30]* /int v) {
 _DeeTime_InplaceDefault((DeeTimeObject *)self);
 msecs = (msecs-(DeeTime_GetMDay(self)*DEE_INT64_C(86400000)))+((Dee_int64_t)v*DEE_INT64_C(86400000));
}
void DeeTime_SetYDay(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN / *0..[365,366]* /int v) {
 _DeeTime_InplaceDefault((DeeTimeObject *)self);
 msecs = (msecs-(DeeTime_GetYDay(self)*DEE_INT64_C(86400000)))+((Dee_int64_t)v*DEE_INT64_C(86400000));
}
void DeeTime_SetMWeek(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN / *0..4* /int v) {
 _DeeTime_InplaceDefault((DeeTimeObject *)self);
 msecs = (msecs-(DeeTime_GetMWeek(self)*DEE_INT64_C(604800000)))+((Dee_int64_t)v*DEE_INT64_C(604800000));
}
void DeeTime_SetYWeek(DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN / *0..55* /int v) {
 _DeeTime_InplaceDefault((DeeTimeObject *)self);
 msecs = (msecs-(DeeTime_GetYWeek(self)*DEE_INT64_C(604800000)))+((Dee_int64_t)v*DEE_INT64_C(604800000));
}
void _DeeTime_SetTimeMSeconds(
 DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN Dee_uint64_t mseconds) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTime_Check(self));
 _DeeTime_InplaceDefault((DeeTimeObject *)self);
 msecs = ((msecs/msecs_per_day)*msecs_per_day)+mseconds;
}
void _DeeTime_SetDateMSeconds(
 DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN Dee_uint64_t mseconds) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTime_Check(self));
 _DeeTime_InplaceDefault((DeeTimeObject *)self);
 msecs = (msecs%msecs_per_day)+mseconds;
}
*/


#define msecs   ((DeeTimeObject *)self)->tm_msecs
void DeeTime_SetDate(
 DEE_A_INOUT_OBJECT(DeeTimeObject) *self,
 DEE_A_IN unsigned int years,
 DEE_A_IN unsigned int months,
 DEE_A_IN unsigned int days) {
 Dee_uint64_t n_days;
 years += months/12,months %= 12;
 n_days = DeeTime_Years2Days(years);
 n_days += month_start_yday[DeeTime_IsLeapYear(years)][months];
 n_days += days;
 _DeeTime_SetDateMSeconds(self,n_days*msecs_per_day);
}


DEE_STATIC_INLINE(void) _DeeTime_InplaceAddYears(
 DeeTimeObject *self, DEE_A_IN Dee_int64_t years) {
 Dee_uint64_t old_years,old_days,new_days;
 DEE_ASSERT(DeeObject_Check(self) && DeeTime_Check(self));
 old_days = msecs/msecs_per_day;
 old_years = DeeTime_Days2Years(old_days);
 new_days = DeeTime_Years2Days((Dee_uint64_t)(old_years+years))+(old_days-DeeTime_Years2Days(old_years));
 msecs += ((Dee_int64_t)new_days-(Dee_int64_t)old_days)*msecs_per_day;
}

void DeeTime_InplaceAddMSeconds(
 DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN Dee_int64_t n_mseconds) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTime_Check(self));
 _DeeTime_InplaceDefault((DeeTimeObject *)self);
 ((DeeTimeObject *)self)->tm_msecs += n_mseconds;
}
void DeeTime_InplaceAddMonths(
 DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN Dee_int64_t n_months) {
 Dee_int64_t more_years,rem_months; unsigned int curr_year;
 DEE_ASSERT(DeeObject_Check(self) && DeeTime_Check(self));
 _DeeTime_InplaceDefault((DeeTimeObject *)self);
 n_months += DeeTime_GetMonth(self);
 DeeTime_SetMonth(self,0);
 more_years = n_months / DEE_INT64_C(12);
 rem_months = n_months % DEE_INT64_C(12);
 if (rem_months < 0) --more_years,rem_months += DEE_INT64_C(12);
 _DeeTime_InplaceAddYears((DeeTimeObject *)self,more_years);
 curr_year = DeeTime_GetYear(self);
 msecs += month_start_yday[DeeTime_IsLeapYear(curr_year)][rem_months]*msecs_per_day;
}
#undef msecs

void DeeTime_InplaceAddYears(
 DEE_A_INOUT_OBJECT(DeeTimeObject) *self, DEE_A_IN Dee_int64_t n_years) {
 DEE_ASSERT(DeeObject_Check(self) && DeeTime_Check(self));
 _DeeTime_InplaceDefault((DeeTimeObject *)self);
 _DeeTime_InplaceAddYears((DeeTimeObject *)self,n_years);
}

DEE_COMPILER_PREFAST_WARNING_PUSH(6387)
DEE_A_RET_WUNUSED Dee_uint64_t DeeTime_GetClock1000(void) {
#ifdef DEE_PLATFORM_WINDOWS
 typedef Dee_uint64_t /*ULONGLONG*/ (WINAPI *LPGETTICKCOUNT64)(VOID);
 static LPGETTICKCOUNT64 p_GetTickCount64 = NULL;
 DEE_ATOMIC_ONCE({
  p_GetTickCount64 = (LPGETTICKCOUNT64)GetProcAddress(
   GetModuleHandle(TEXT("KERNEL32")),"GetTickCount64");
 });
 if DEE_LIKELY(p_GetTickCount64) return (*p_GetTickCount64)();
#ifdef _PREFAST_
#pragma warning(push)
#pragma warning(disable: 28159)
#endif
 return (Dee_uint64_t)GetTickCount();
#ifdef _PREFAST_
#pragma warning(pop)
#endif
#elif DEE_HAVE_CLOCK_GETTIME && (\
  DEE_HAVE_CLOCK_PROCESS_CPUTIME_ID || \
  DEE_HAVE_CLOCK_THREAD_CPUTIME_ID)
 struct timespec result;
#if DEE_HAVE_CLOCK_PROCESS_CPUTIME_ID
 clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&result);
#else
 clock_gettime(CLOCK_THREAD_CPUTIME_ID,&result);
#endif
 return ((Dee_uint64_t)result.tv_sec*DEE_UINT64_C(1000))+
        ((Dee_uint64_t)result.tv_nsec/UINT64_C(1000000));
#else
 return 0;
#endif
}
DEE_COMPILER_PREFAST_WARNING_POP

DEE_A_RET_WUNUSED Dee_uint64_t DeeTime_GetClockTick(void) {
#ifdef DEE_PLATFORM_WINDOWS
 Dee_uint64_t result;
 if DEE_LIKELY(QueryPerformanceCounter((LARGE_INTEGER *)&result)) return result;
 SetLastError(0);
 return DeeTime_GetClock1000();
#elif DEE_HAVE_CLOCK_GETTIME && (\
  DEE_HAVE_CLOCK_PROCESS_CPUTIME_ID || \
  DEE_HAVE_CLOCK_THREAD_CPUTIME_ID)
 struct timespec result;
#if DEE_HAVE_CLOCK_PROCESS_CPUTIME_ID
 clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&result);
#else
 clock_gettime(CLOCK_THREAD_CPUTIME_ID,&result);
#endif
 return (result.tv_sec*DEE_UINT64_C(1000000000))+result.tv_nsec;
#else
 return 0;
#endif
}
DEE_A_RET_WUNUSED Dee_uint64_t DeeTime_GetClockFreq(void) {
#ifdef DEE_PLATFORM_WINDOWS
 static Dee_uint64_t result = 0;
 DEE_ATOMIC_ONCE({ // Only needs to be done once
  if DEE_UNLIKELY(!QueryPerformanceFrequency((LARGE_INTEGER *)&result)) {
   SetLastError(0);
   result = 1000;
  }
 });
 return result;
#elif DEE_HAVE_CLOCK_GETTIME && (\
  DEE_HAVE_CLOCK_PROCESS_CPUTIME_ID || \
  DEE_HAVE_CLOCK_THREAD_CPUTIME_ID)
 return DEE_UINT64_C(1000000000);
#else
 return 1000;
#endif
}



static int DEE_CALL _deetime_tp_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTimeObject *self) {
 self->tm_diff_kind = DeeTimeKind_DEFAULT;
 return DeeTimeTick_Now(&self->tm_msecs);
}
static int DEE_CALL _deetime_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTimeObject *self, DeeTimeObject *right) {
 self->tm_diff_kind = right->tm_diff_kind;
 self->tm_msecs = right->tm_msecs;
 return 0;
}
static int DEE_CALL _deetime_tp_copy_assign(DeeTimeObject *self, DeeTimeObject *right) {
 self->tm_diff_kind = right->tm_diff_kind;
 self->tm_msecs = right->tm_msecs;
 return 0;
}
static int DEE_CALL _deetime_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTimeObject *self, DeeTupleObject *args) {
 unsigned int year,month = 0,mday = 0,hour = 0,minute = 0,second = 0,msecond = 0;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (DeeTuple_SIZE(args) == 1) {
  // Required for simulating 'time_t time(time_t *)' behavior
  DeeTypeObject *time_t_pointer;
  if DEE_UNLIKELY((time_t_pointer = DeeType_Pointer(DeeObject_TYPE(Dee_time_t))) == NULL) return -1;
  if (DeeObject_InstanceOf(DeeTuple_GET(args,0),time_t_pointer)) {
   if DEE_UNLIKELY(DeeTimeTick_Now(&self->tm_msecs) != 0) return -1;
   self->tm_diff_kind = DeeTimeKind_DEFAULT;
   return DeePointer_ADDR(DeeTuple_GET(args,0))
    ? DeeTime_AsTimeT((DeeObject *)self,(Dee_time_t *)DeePointer_ADDR(DeeTuple_GET(args,0)))
    : 0;
  }
 }
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 if DEE_UNLIKELY(DeeTuple_Unpack((DeeObject *)args,"u|uuuuuu:time",
  &year,&month,&mday,&hour,&minute,&second,&msecond) != 0) return -1;
 DeeTime_SetTick(self,DeeTimeTick_FromData(year,month,mday,hour,minute,second,msecond));
 return 0;
}
static DeeObject *DEE_CALL _deetime_tp_str(DeeTimeObject *self) {
 return DeeTime_Format((DeeObject *)self,"%d.%m.%Y %H:%M:%S");
}
static DeeObject *DEE_CALL _deetime_tp_repr(DeeTimeObject *self) {
 // TODO: Extended format (e.g.: 'Saturday, the 13th of August 2016, 20:51:18')
 //return DeeTime_Format((DeeObject *)self,"%d.%m.%Y %H:%M:%S");
 return DeeTime_Format((DeeObject *)self,"%A, the %[n:mday] of %B %Y, %H:%M:%S");
}
static int DEE_CALL _deetime_tp_bool(DeeTimeObject *self) { return DeeTime_GetTotalMSeconds((DeeObject *)self) != 0; }
static DeeObject *DEE_CALL _deetime_tp_not(DeeTimeObject *self) { DeeReturn_Bool(DeeTime_GetTotalMSeconds((DeeObject *)self) == 0); }
static DeeObject *DEE_CALL _deetime_tp_inc(DeeTimeObject *self) { _DeeTime_InplaceDefault(self); ++self->tm_msecs; DeeReturn_NEWREF(self); }
static DeeObject *DEE_CALL _deetime_tp_dec(DeeTimeObject *self) { _DeeTime_InplaceDefault(self); --self->tm_msecs; DeeReturn_NEWREF(self); }
static DeeObject *DEE_CALL _deetime_tp_incpost(DeeTimeObject *self) { _DeeTime_InplaceDefault(self); return DeeTime_NEW(self->tm_msecs++); }
static DeeObject *DEE_CALL _deetime_tp_decpost(DeeTimeObject *self) { _DeeTime_InplaceDefault(self); return DeeTime_NEW(self->tm_msecs--); }
static DeeObject *DEE_CALL _deetime_tp_add(DeeTimeObject *self, DeeTimeObject *right) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeTime_Type) != 0) return NULL;
 if DEE_LIKELY((result = DeeTime_Copy((DeeObject *)self)) != NULL) switch (right->tm_diff_kind) {
  case DeeTimeKind_YEARS: DeeTime_InplaceAddYears(result,(Dee_int64_t)right->tm_years); break;
  case DeeTimeKind_MONTHS: DeeTime_InplaceAddMonths(result,(Dee_int64_t)right->tm_months); break;
  default: DeeTime_InplaceAddMSeconds(result,(Dee_int64_t)right->tm_msecs); break;
 }
 return result;
}
static DeeObject *DEE_CALL _deetime_tp_iadd(DeeTimeObject *self, DeeTimeObject *right) {
 if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeTime_Type) != 0) return NULL;
 switch (right->tm_diff_kind) {
  case DeeTimeKind_YEARS: DeeTime_InplaceAddYears((DeeObject *)self,(Dee_int64_t)right->tm_years); break;
  case DeeTimeKind_MONTHS: DeeTime_InplaceAddMonths((DeeObject *)self,(Dee_int64_t)right->tm_months); break;
  default: DeeTime_InplaceAddMSeconds((DeeObject *)self,(Dee_int64_t)right->tm_msecs); break;
 }
 DeeReturn_NEWREF(self);
}
static DeeObject *DEE_CALL _deetime_tp_sub(DeeTimeObject *self, DeeTimeObject *right) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeTime_Type) != 0) return NULL;
 if DEE_LIKELY((result = DeeTime_Copy((DeeObject *)self)) != NULL) switch (right->tm_diff_kind) {
  case DeeTimeKind_YEARS: DeeTime_InplaceAddYears(result,-(Dee_int64_t)right->tm_years); break;
  case DeeTimeKind_MONTHS: DeeTime_InplaceAddMonths(result,-(Dee_int64_t)right->tm_months); break;
  default: DeeTime_InplaceAddMSeconds(result,-(Dee_int64_t)right->tm_msecs); break;
 }
 return result;
}
static DeeObject *DEE_CALL _deetime_tp_isub(DeeTimeObject *self, DeeTimeObject *right) {
 if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeTime_Type) != 0) return NULL;
 switch (right->tm_diff_kind) {
  case DeeTimeKind_YEARS: DeeTime_InplaceAddYears((DeeObject *)self,-(Dee_int64_t)right->tm_years); break;
  case DeeTimeKind_MONTHS: DeeTime_InplaceAddMonths((DeeObject *)self,-(Dee_int64_t)right->tm_months); break;
  default: DeeTime_InplaceAddMSeconds((DeeObject *)self,-(Dee_int64_t)right->tm_msecs); break;
 }
 DeeReturn_NEWREF(self);
}
static DeeObject *DEE_CALL _deetime_tp_mod(DeeTimeObject *self, DeeTimeObject *right) {
 if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeTime_Type) != 0) return NULL;
 return DeeTime_NEW(DeeTime_GetTotalMSeconds((DeeObject *)self) %
                    DeeTime_GetTotalMSeconds((DeeObject *)right));
}
static DeeObject *DEE_CALL _deetime_tp_imod(DeeTimeObject *self, DeeTimeObject *right) {
 if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeTime_Type) != 0) return NULL;
 _DeeTime_InplaceDefault(self);
 self->tm_msecs %= DeeTime_GetTotalMSeconds((DeeObject *)right);
 DeeReturn_NEWREF(self);
}
static DeeObject *DEE_CALL _deetime_tp_mul(DeeTimeObject *self, DeeObject *right) {
 Dee_uint64_t n;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_uint64_t,right,&n) != 0) return NULL;
 return DeeTime_NEW(DeeTime_GetTotalMSeconds((DeeObject *)self)*n);
}
static DeeObject *DEE_CALL _deetime_tp_imul(DeeTimeObject *self, DeeObject *right) {
 Dee_uint64_t n;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_uint64_t,right,&n) != 0) return NULL;
 _DeeTime_InplaceDefault(self);
 self->tm_msecs *= n;
 DeeReturn_NEWREF(self);
}
static DeeObject *DEE_CALL _deetime_tp_div(DeeTimeObject *self, DeeObject *right) {
 Dee_uint64_t n;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_uint64_t,right,&n) != 0) return NULL;
 return DeeTime_NEW(DeeTime_GetTotalMSeconds((DeeObject *)self)/n);
}
static DeeObject *DEE_CALL _deetime_tp_idiv(DeeTimeObject *self, DeeObject *right) {
 Dee_uint64_t n;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_uint64_t,right,&n) != 0) return NULL;
 _DeeTime_InplaceDefault(self);
 self->tm_msecs /= n;
 DeeReturn_NEWREF(self);
}

static DeeObject *DEE_CALL _deetime_tp_cmp_lo(DeeTimeObject *self, DeeTimeObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeTime_Type) != 0) return NULL; DeeReturn_Bool(DeeTime_CompareLo((DeeObject *)self,(DeeObject *)right)); }
static DeeObject *DEE_CALL _deetime_tp_cmp_le(DeeTimeObject *self, DeeTimeObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeTime_Type) != 0) return NULL; DeeReturn_Bool(DeeTime_CompareLe((DeeObject *)self,(DeeObject *)right)); }
static DeeObject *DEE_CALL _deetime_tp_cmp_eq(DeeTimeObject *self, DeeTimeObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeTime_Type) != 0) return NULL; DeeReturn_Bool(DeeTime_CompareEq((DeeObject *)self,(DeeObject *)right)); }
static DeeObject *DEE_CALL _deetime_tp_cmp_ne(DeeTimeObject *self, DeeTimeObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeTime_Type) != 0) return NULL; DeeReturn_Bool(DeeTime_CompareNe((DeeObject *)self,(DeeObject *)right)); }
static DeeObject *DEE_CALL _deetime_tp_cmp_gr(DeeTimeObject *self, DeeTimeObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeTime_Type) != 0) return NULL; DeeReturn_Bool(DeeTime_CompareGr((DeeObject *)self,(DeeObject *)right)); }
static DeeObject *DEE_CALL _deetime_tp_cmp_ge(DeeTimeObject *self, DeeTimeObject *right) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&right,&DeeTime_Type) != 0) return NULL; DeeReturn_Bool(DeeTime_CompareGe((DeeObject *)self,(DeeObject *)right)); }

static DeeObject *DEE_CALL _deetime_format(DeeObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *fmt,*result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:format",&fmt) != 0) return NULL;
 if DEE_UNLIKELY((fmt = DeeString_Cast(fmt)) == NULL) return NULL;
 result = DeeTime_FormatObject(self,fmt);
 Dee_DECREF(fmt);
 return result;
}

static int DEE_CALL _deetime_int32(DeeObject *self, Dee_int32_t *result) { Dee_time_t resval; if DEE_UNLIKELY(DeeTime_AsTimeT(self,&resval) != 0) return -1; *result = (Dee_int32_t)resval; return 0; }
static int DEE_CALL _deetime_int64(DeeObject *self, Dee_int64_t *result) { Dee_time_t resval; if DEE_UNLIKELY(DeeTime_AsTimeT(self,&resval) != 0) return -1; *result = (Dee_int64_t)resval; return 0; }
static int DEE_CALL _deetime_double(DeeObject *self, double *result) { Dee_time_t resval; if DEE_UNLIKELY(DeeTime_AsTimeT(self,&resval) != 0) return -1; *result = (double)resval; return 0; }
static DeeObject *DEE_CALL _deetime_time_t_get(DeeObject *self, void *DEE_UNUSED(closure)) {
 Dee_time_t result;
 return DeeTime_AsTimeT(self,&result) != 0 ? NULL : DeeObject_New(Dee_time_t,result);
}
static int DEE_CALL _deetime_time_t_del(DeeObject *self, void *DEE_UNUSED(closure)) { DeeTime_SetTick(self,0); return 0; }
static int DEE_CALL _deetime_time_t_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 Dee_uint64_t ticks; Dee_time_t iv;
 if DEE_UNLIKELY(DeeObject_Cast(Dee_time_t,v,&iv) != 0) return -1;
 if DEE_UNLIKELY(DeeTimeTick_FromTimeT(iv,&ticks) != 0) return -1;
 DeeTime_SetTick(self,ticks);
 return 0;
}
static DeeObject *DEE_CALL _deetime_year_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeObject_New(uint,DeeTime_GetYear(self)); }
static int DEE_CALL _deetime_year_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { unsigned int iv; if DEE_UNLIKELY(DeeObject_Cast(uint,v,&iv) != 0) return -1; DeeTime_SetYear(self,iv); return 0; }
static int DEE_CALL _deetime_year_del(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeTime_SetYear(self,0),0; }
static DeeObject *DEE_CALL _deetime_years_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeUInt64_New(DeeTime_GetTotalYears(self)); }
static DeeObject *DEE_CALL _deetime_month_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeObject_New(uint,DeeTime_GetMonth(self)); }
static DeeObject *DEE_CALL _deetime_months_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeUInt64_New(DeeTime_GetTotalMonths(self)); }
static int DEE_CALL _deetime_month_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { int iv; if DEE_UNLIKELY(DeeObject_Cast(int,v,&iv) != 0) return -1; DeeTime_SetMonth(self,iv); return 0; }
static int DEE_CALL _deetime_months_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { Dee_uint64_t iv; if DEE_UNLIKELY(DeeObject_Cast(Dee_uint64_t,v,&iv) != 0) return -1; DeeTime_SetTotalMonths(self,iv); return 0; }
static int DEE_CALL _deetime_month_del(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeTime_SetMonth(self,0),0; }
static DeeObject *DEE_CALL _deetime_mweek_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeObject_New(uint,DeeTime_GetMWeek(self)); }
static DeeObject *DEE_CALL _deetime_yweek_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeObject_New(uint,DeeTime_GetYWeek(self)); }
static DeeObject *DEE_CALL _deetime_weeks_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeUInt64_New(DeeTime_GetTotalWeeks(self)); }
static int DEE_CALL _deetime_mweek_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { int iv; if DEE_UNLIKELY(DeeObject_Cast(int,v,&iv) != 0) return -1; DeeTime_SetMWeek(self,iv); return 0; }
static int DEE_CALL _deetime_yweek_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { int iv; if DEE_UNLIKELY(DeeObject_Cast(int,v,&iv) != 0) return -1; DeeTime_SetYWeek(self,iv); return 0; }
static int DEE_CALL _deetime_weeks_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { Dee_uint64_t iv; if DEE_UNLIKELY(DeeObject_Cast(Dee_uint64_t,v,&iv) != 0) return -1; DeeTime_SetTotalWeeks(self,iv); return 0; }
static int DEE_CALL _deetime_mweek_del(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeTime_SetMWeek(self,0),0; }
static int DEE_CALL _deetime_yweek_del(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeTime_SetYWeek(self,0),0; }
static DeeObject *DEE_CALL _deetime_wday_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeObject_New(uint,DeeTime_GetWDay(self)); }
static DeeObject *DEE_CALL _deetime_mday_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeObject_New(uint,DeeTime_GetMDay(self)); }
static DeeObject *DEE_CALL _deetime_yday_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeObject_New(uint,DeeTime_GetYDay(self)); }
static DeeObject *DEE_CALL _deetime_days_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeUInt64_New(DeeTime_GetTotalDays(self)); }
static int DEE_CALL _deetime_wday_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { int iv; if DEE_UNLIKELY(DeeObject_Cast(int,v,&iv) != 0) return -1; DeeTime_SetWDay(self,iv); return 0; }
static int DEE_CALL _deetime_mday_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { int iv; if DEE_UNLIKELY(DeeObject_Cast(int,v,&iv) != 0) return -1; DeeTime_SetMDay(self,iv); return 0; }
static int DEE_CALL _deetime_yday_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { int iv; if DEE_UNLIKELY(DeeObject_Cast(int,v,&iv) != 0) return -1; DeeTime_SetYDay(self,iv); return 0; }
static int DEE_CALL _deetime_days_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { Dee_uint64_t iv; if DEE_UNLIKELY(DeeObject_Cast(Dee_uint64_t,v,&iv) != 0) return -1; DeeTime_SetTotalDays(self,iv); return 0; }
static int DEE_CALL _deetime_wday_del(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeTime_SetWDay(self,0),0; }
static int DEE_CALL _deetime_mday_del(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeTime_SetMDay(self,0),0; }
static int DEE_CALL _deetime_yday_del(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeTime_SetYDay(self,0),0; }
static DeeObject *DEE_CALL _deetime_hour_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeObject_New(uint,DeeTime_GetHour(self)); }
static DeeObject *DEE_CALL _deetime_hours_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeUInt64_New(DeeTime_GetTotalHours(self)); }
static int DEE_CALL _deetime_hour_del(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeTime_SetHour(self,0),0; }
static int DEE_CALL _deetime_hour_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { int iv; if DEE_UNLIKELY(DeeObject_Cast(int,v,&iv) != 0) return -1; DeeTime_SetHour(self,iv); return 0; }
static int DEE_CALL _deetime_hours_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { Dee_uint64_t iv; if DEE_UNLIKELY(DeeObject_Cast(Dee_uint64_t,v,&iv) != 0) return -1; DeeTime_SetTotalHours(self,iv); return 0; }
static DeeObject *DEE_CALL _deetime_minute_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeObject_New(uint,DeeTime_GetMinute(self)); }
static DeeObject *DEE_CALL _deetime_minutes_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeUInt64_New(DeeTime_GetTotalMinutes(self)); }
static int DEE_CALL _deetime_minute_del(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeTime_SetMinute(self,0),0; }
static int DEE_CALL _deetime_minute_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { int iv; if DEE_UNLIKELY(DeeObject_Cast(int,v,&iv) != 0) return -1; DeeTime_SetMinute(self,iv); return 0; }
static int DEE_CALL _deetime_minutes_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { Dee_uint64_t iv; if DEE_UNLIKELY(DeeObject_Cast(Dee_uint64_t,v,&iv) != 0) return -1; DeeTime_SetTotalMinutes(self,iv); return 0; }
static DeeObject *DEE_CALL _deetime_second_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeObject_New(uint,DeeTime_GetSecond(self)); }
static DeeObject *DEE_CALL _deetime_seconds_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeUInt64_New(DeeTime_GetTotalSeconds(self)); }
static int DEE_CALL _deetime_second_del(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeTime_SetSecond(self,0),0; }
static int DEE_CALL _deetime_second_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { int iv; if DEE_UNLIKELY(DeeObject_Cast(int,v,&iv) != 0) return -1; DeeTime_SetSecond(self,iv); return 0; }
static int DEE_CALL _deetime_seconds_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { Dee_uint64_t iv; if DEE_UNLIKELY(DeeObject_Cast(Dee_uint64_t,v,&iv) != 0) return -1; DeeTime_SetTotalSeconds(self,iv); return 0; }
static DeeObject *DEE_CALL _deetime_msecond_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeObject_New(uint,DeeTime_GetMSecond(self)); }
static DeeObject *DEE_CALL _deetime_mseconds_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeUInt64_New(DeeTime_GetTotalMSeconds(self)); }
static int DEE_CALL _deetime_msecond_del(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeTime_SetMSecond(self,0),0; }
static int DEE_CALL _deetime_msecond_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { int iv; if DEE_UNLIKELY(DeeObject_Cast(int,v,&iv) != 0) return -1; DeeTime_SetMSecond(self,iv); return 0; }
static int DEE_CALL _deetime_mseconds_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { Dee_uint64_t iv; if DEE_UNLIKELY(DeeObject_Cast(Dee_uint64_t,v,&iv) != 0) return -1; DeeTime_SetTotalMSeconds(self,iv); return 0; }
static DeeObject *DEE_CALL _deetime_time_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeTime_GetTime(self); }
static DeeObject *DEE_CALL _deetime_date_get(DeeObject *self, void *DEE_UNUSED(closure)) { return DeeTime_GetDate(self); }
static int DEE_CALL _deetime_time_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&v,&DeeTime_Type) != 0) return -1;
 return DeeTime_SetTimeObject(self,v),0;
}
static int DEE_CALL _deetime_date_set(DeeObject *self, DeeObject *v, void *DEE_UNUSED(closure)) { if DEE_UNLIKELY(DeeObject_InplaceGetInstance(&v,&DeeTime_Type) != 0) return -1; DeeTime_SetDateObject(self,v); return 0; }
static int DEE_CALL _deetime_time_del(DeeObject *self, void *DEE_UNUSED(closure)) { return _DeeTime_SetTimeMSeconds(self,0),0; }
static int DEE_CALL _deetime_date_del(DeeObject *self, void *DEE_UNUSED(closure)) { return _DeeTime_SetDateMSeconds(self,0),0; }

static struct DeeMethodDef const _deetime_tp_methods[] = {
 DEE_METHODDEF_v100("format",member(&_deetime_format),"(string fmt) -> string\n"
  "@return: The formated time string\n"
  "\tReturns the representation of this time object as a string, using fmt as date/time format"),
 DEE_METHODDEF_v100("__format__",member(&_deetime_format),"(string fmt) -> string\n"
  "@return: The formated time string\n"
  "\tReturns the representation of this time object as a string, using fmt as date/time format\n"
  "\tAlias for 'format', and used for internal string formating"),
 DEE_METHODDEF_END_v100
};
static struct DeeGetSetDef const _deetime_tp_getsets[] = {
 DEE_GETSETDEF_v100("year",member(&_deetime_year_get),member(&_deetime_year_del),member(&_deetime_year_set),"-> unsigned int\n\tYear"),
 DEE_GETSETDEF_v100("years",member(&_deetime_years_get),null,member(&_deetime_year_set),"-> uint64_t\n\tYear (alias for 'year')"),
 DEE_GETSETDEF_v100("month",member(&_deetime_month_get),member(&_deetime_month_del),member(&_deetime_month_set),"-> [unsigned] int\n\tMonth (zero-based)"),
 DEE_GETSETDEF_v100("months",member(&_deetime_months_get),null,member(&_deetime_months_set),"-> uint64_t\n\tTotal amount of months since time(0)"),
 DEE_GETSETDEF_v100("mweek",member(&_deetime_mweek_get),member(&_deetime_mweek_del),member(&_deetime_mweek_set),"-> [unsigned] int\n\tWeek number of the month (zero-based)"),
 DEE_GETSETDEF_v100("yweek",member(&_deetime_yweek_get),member(&_deetime_yweek_del),member(&_deetime_yweek_set),"-> [unsigned] int\n\tWeek number of the year (zero-based)"),
 DEE_GETSETDEF_v100("weeks",member(&_deetime_weeks_get),null,member(&_deetime_weeks_set),"-> uint64_t\n\tTotal amount of weeks since time(0)"),
 DEE_GETSETDEF_v100("wday",member(&_deetime_wday_get),member(&_deetime_wday_del),member(&_deetime_wday_set),"-> [unsigned] int\n\tDay of the week (zero-based)"),
 DEE_GETSETDEF_v100("mday",member(&_deetime_mday_get),member(&_deetime_mday_del),member(&_deetime_mday_set),"-> [unsigned] int\n\tDay of the month (zero-based)"),
 DEE_GETSETDEF_v100("yday",member(&_deetime_yday_get),member(&_deetime_yday_del),member(&_deetime_yday_set),"-> [unsigned] int\n\tDay of the year (zero-based)"),
 DEE_GETSETDEF_v100("days",member(&_deetime_days_get),null,member(&_deetime_days_set),"-> uint64_t\n\tTotal amount of days since time(0)"),
 DEE_GETSETDEF_v100("hour",member(&_deetime_hour_get),member(&_deetime_hour_del),member(&_deetime_hour_set),"-> [unsigned] int\n\tHour (zero-based)"),
 DEE_GETSETDEF_v100("hours",member(&_deetime_hours_get),null,member(&_deetime_hours_set),"-> uint64_t\n\tTotal amount of hours since time(0)"),
 DEE_GETSETDEF_v100("minute",member(&_deetime_minute_get),member(&_deetime_minute_del),member(&_deetime_minute_set),"-> [unsigned] int\n\tMinute (zero-based)"),
 DEE_GETSETDEF_v100("minutes",member(&_deetime_minutes_get),null,member(&_deetime_minutes_set),"-> uint64_t\n\tTotal amount of minutes since time(0)"),
 DEE_GETSETDEF_v100("second",member(&_deetime_second_get),member(&_deetime_second_del),member(&_deetime_second_set),"-> [unsigned] int\n\tSecond (zero-based)"),
 DEE_GETSETDEF_v100("seconds",member(&_deetime_seconds_get),null,member(&_deetime_seconds_set),"-> uint64_t\n\tTotal amount of seconds since time(0)"),
 DEE_GETSETDEF_v100("msecond",member(&_deetime_msecond_get),member(&_deetime_msecond_del),member(&_deetime_msecond_set),"-> [unsigned] int\n\tMilli-second (zero-based)"),
 DEE_GETSETDEF_v100("mseconds",member(&_deetime_mseconds_get),null,member(&_deetime_mseconds_set),"-> uint64_t\n\tTotal amount of milli-seconds since time(0)"),
 DEE_GETSETDEF_v100("time",member(&_deetime_time_get),member(&_deetime_time_del),member(&_deetime_time_set),"-> time\n\tDescribes the time (hour, minute, second, msecond) portion of a time object"),
 DEE_GETSETDEF_v100("date",member(&_deetime_date_get),member(&_deetime_date_del),member(&_deetime_date_set),"-> time\n\tDescribes the date (year, month, day) portion of a time object"),
 DEE_GETSETDEF_v100("time_t",member(&_deetime_time_t_get),member(&_deetime_time_t_del),member(&_deetime_time_t_set),"-> time_t\n\tDescribes the time/date with the standard time_t format"),
 DEE_GETSETDEF_END_v100
};


static DeeObject *DEE_CALL _deetimeclass_tick(
 DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":tick") != 0) return NULL;
 return DeeUInt64_New(DeeTime_GetClockTick());
}
static DeeObject *DEE_CALL _deetimeclass_freq(
 DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":freq") != 0) return NULL;
 return DeeUInt64_New(DeeTime_GetClockFreq());
}
static DeeObject *DEE_CALL _deetimeclass_from_time_t(
 DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *arg0; Dee_time_t v;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:from_time_t",&arg0) != 0) return NULL;
 if (DeeTime_Check(arg0)) DeeReturn_NEWREF(arg0);
 if DEE_UNLIKELY(DeeObject_Cast(Dee_time_t,arg0,&v) != 0) return NULL;
 return DeeTime_NewFromTimeT(v);
}
static DeeObject *DEE_CALL _deetimeclass_now(DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":now") != 0) return NULL;
 return DeeTime_Now();
}
static DeeObject *DEE_CALL _deetimeclass_time(DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 unsigned int hour = 0,minute = 0,second = 0,msecond = 0;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|uuuu:time",&hour,&minute,&second,&msecond) != 0) return NULL;
 return DeeTime_NewFromData(0,0,0,hour,minute,second,msecond);
}
static DeeObject *DEE_CALL _deetimeclass_date(DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 unsigned int year = 0,month = 0,mday = 0;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|uuu:date",&year,&month,&mday) != 0) return NULL;
 return DeeTime_NewFromData(year,month,mday,0,0,0,0);
}
static DeeObject *DEE_CALL _deetimeclass_years(DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) { Dee_uint64_t year; if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64u:years",&year) != 0) return NULL; return DeeTime_NewFromYears(year); }
static DeeObject *DEE_CALL _deetimeclass_months(DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) { Dee_uint64_t months; if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64u:months",&months) != 0) return NULL; return DeeTime_NewFromMonths(months); }
static DeeObject *DEE_CALL _deetimeclass_weeks(DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) { Dee_uint64_t weeks; if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64u:weeks",&weeks) != 0) return NULL; return DeeTime_NewFromWeeks(weeks); }
static DeeObject *DEE_CALL _deetimeclass_days(DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) { Dee_uint64_t days; if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64u:days",&days) != 0) return NULL; return DeeTime_NewFromDays(days); }
static DeeObject *DEE_CALL _deetimeclass_hours(DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) { Dee_uint64_t hours; if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64u:hours",&hours) != 0) return NULL; return DeeTime_NewFromHours(hours); }
static DeeObject *DEE_CALL _deetimeclass_minutes(DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) { Dee_uint64_t minutes; if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64u:minutes",&minutes) != 0) return NULL; return DeeTime_NewFromMinutes(minutes); }
static DeeObject *DEE_CALL _deetimeclass_seconds(DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) { Dee_uint64_t seconds; if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64u:seconds",&seconds) != 0) return NULL; return DeeTime_NewFromSeconds(seconds); }
static DeeObject *DEE_CALL _deetimeclass_mseconds(DeeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) { Dee_uint64_t mseconds; if DEE_UNLIKELY(DeeTuple_Unpack(args,"I64u:mseconds",&mseconds) != 0) return NULL; return DeeTime_NEW(mseconds); }
static struct DeeMethodDef const _deetime_tp_class_methods[] = {
 DEE_METHODDEF_v100("now",member(&_deetimeclass_now),"() -> time\n"
  "@return: The current time"),
 DEE_METHODDEF_v100("tick",member(&_deetimeclass_tick),"() -> uint64_t\n"
  "@return: The time since some platform-specific, undefined event in #time.freq / second"),
 DEE_METHODDEF_v100("freq",member(&_deetimeclass_freq),"() -> uint64_t\n"
  "@return: The amount of ticks passing in #time.tick every second"),
 DEE_METHODDEF_v100("from_time_t",member(&_deetimeclass_from_time_t),"(time_t tmt) -> time\n"
  "@return: A time object initialized from a #time_t integral value"),
 DEE_METHODDEF_CONST_v100("time",member(&_deetimeclass_time),"(unsigned int hour = 0, unsigned int minute = 0, unsigned int second = 0, unsigned int msecond = 0) -> time\n"
  "@return: A time object with it's time filled in"),
 DEE_METHODDEF_CONST_v100("date",member(&_deetimeclass_date),"(unsigned int year = 0, unsigned int month = 0, unsigned int day = 0) -> time\n"
  "@return: A time object with it's date filled in"),
 DEE_METHODDEF_CONST_v100("years",member(&_deetimeclass_years),"(uint64_t n_years) -> time\n"
  "@return: A time difference of @n_years years"),
 DEE_METHODDEF_CONST_v100("months",member(&_deetimeclass_months),"(uint64_t n_months) -> time\n"
  "@return: A time difference of @n_months months"),
 DEE_METHODDEF_CONST_v100("weeks",member(&_deetimeclass_weeks),"(uint64_t n_weeks) -> time\n"
  "@return: A time difference of @n_weeks weeks"),
 DEE_METHODDEF_CONST_v100("days",member(&_deetimeclass_days),"(uint64_t n_days) -> time\n"
  "@return: A time difference of @n_days days"),
 DEE_METHODDEF_CONST_v100("hours",member(&_deetimeclass_hours),"(uint64_t n_hours) -> time\n"
  "@return: A time difference of @n_hours hours"),
 DEE_METHODDEF_CONST_v100("minutes",member(&_deetimeclass_minutes),"(uint64_t n_minutes) -> time\n"
  "@return: A time difference of @n_minutes minutes"),
 DEE_METHODDEF_CONST_v100("seconds",member(&_deetimeclass_seconds),"(uint64_t n_seconds) -> time\n"
  "@return: A time difference of @n_seconds seconds"),
 DEE_METHODDEF_CONST_v100("mseconds",member(&_deetimeclass_mseconds),"(uint64_t n_mseconds) -> time\n"
  "@return: A time difference of @n_mseconds milli-seconds"),
 DEE_METHODDEF_END_v100
};



static int DEE_CALL _deetime_tp_marshal_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTimeObject *self,
 DeeObject *infile, struct DeeMarshalReadMap *DEE_UNUSED(map)) {
 if DEE_UNLIKELY(DeeFile_GetLe(infile,self->tm_diff_kind) != 0) return -1;
 return DeeFile_GetLe(infile,self->tm_msecs);
}
static int DEE_CALL _deetime_tp_marshal_put(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeTimeObject *self,
 DeeObject *outfile, struct DeeMarshalWriteMap *DEE_UNUSED(map)) {
 if DEE_UNLIKELY(DeeFile_PutLe(outfile,self->tm_diff_kind) != 0) return -1;
 return DeeFile_PutLe(outfile,self->tm_msecs);
}

static struct DeeTypeMarshal _deetime_tp_marshal = DEE_TYPE_MARSHAL_v100(
 DeeUUID_INIT_INTERNAL(DEE_MARSHALTYPEID(time)),
 member(&_deetime_tp_marshal_ctor),member(&_deetime_tp_marshal_put));

DeeTypeObject DeeTime_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("time"),member(
  "(unsigned int year = 0, unsigned int month = 0, unsigned int day = 0, unsigned int hour = 0, unsigned int minute = 0, unsigned int second = 0, unsigned int msecond = 0)\n"
  "@return: A new time object\n"),member(
  DEE_TYPE_FLAG_NO_LIFETIME_EFFECT|DEE_TYPE_FLAG_CONST_CTOR|
  DEE_TYPE_FLAG_MUST_COPY|DEE_TYPE_FLAG_FUNDAMENTAL),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeTimeObject),null,
  member(&_deetime_tp_ctor),
  member(&_deetime_tp_copy_ctor),null,
  member(&_deetime_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(
  member(&_deetime_tp_copy_assign),null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deetime_tp_str),member(&_deetime_tp_repr),
  member(&_deetime_int32),member(&_deetime_int64),
  member(&_deetime_double)),
 DEE_TYPE_OBJECT_OBJECT_v101(null,null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  member(&_deetime_tp_not),member(&_deetime_tp_bool),null,null,null,
  member(&_deetime_tp_inc),member(&_deetime_tp_incpost),
  member(&_deetime_tp_dec),member(&_deetime_tp_decpost),
  member(&_deetime_tp_add),member(&_deetime_tp_iadd),
  member(&_deetime_tp_sub),member(&_deetime_tp_isub),
  member(&_deetime_tp_mul),member(&_deetime_tp_imul),
  member(&_deetime_tp_div),member(&_deetime_tp_idiv),
  member(&_deetime_tp_mod),member(&_deetime_tp_imod),
  null,null,null,null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(
  member(&_deetime_tp_cmp_lo),member(&_deetime_tp_cmp_le),
  member(&_deetime_tp_cmp_eq),member(&_deetime_tp_cmp_ne),
  member(&_deetime_tp_cmp_gr),member(&_deetime_tp_cmp_ge)),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,member(_deetime_tp_getsets),member(_deetime_tp_methods),
  null,null,member(_deetime_tp_class_methods)),
 DEE_TYPE_OBJECT_MARSHAL_v101(member(&_deetime_tp_marshal)),
 DEE_TYPE_OBJECT_FOOTER_v101
};


DEE_COMPILER_MSVC_WARNING_POP
DEE_DECL_END

#endif /* !GUARD_DEEMON_TIME_C */
