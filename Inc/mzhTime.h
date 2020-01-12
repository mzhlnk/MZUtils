#pragma once

#ifndef _MZH_TIME
#define _MZH_TIME

#include <time.h>
#include <WinBase.h>
#include <mzhDef.h>

_MZH_BEGIN

const LONGLONG FT1970 = 116444736000000000LL;

inline void TimeToFileTime(const time_t tt, FILETIME& ft)
{
	PLARGE_INTEGER pli = (PLARGE_INTEGER)&ft;
	pli->QuadPart = FT1970 + (LONGLONG)tt * 10000000;
}

inline void FileTimeToTime(const FILETIME& ft, time_t& tt)
{
	const LARGE_INTEGER* pli = (const LARGE_INTEGER*)&ft;
	tt = (time_t)((pli->QuadPart - FT1970) / 10000000);
}

inline void TimeToSysTime(const tm& tm, SYSTEMTIME& st)
{
	st.wYear = tm.tm_year + 1900;
	st.wMonth = tm.tm_mon + 1;
	st.wDay = tm.tm_mday;
	st.wDayOfWeek = tm.tm_wday;
	st.wHour = tm.tm_hour;
	st.wMinute = tm.tm_min;
	st.wSecond = tm.tm_sec;
	st.wMilliseconds = 0;
}

inline void TimeToSysTime(const time_t tt, SYSTEMTIME& st)
{
	FILETIME ft;
	TimeToFileTime(tt, ft);
	FileTimeToSystemTime(&ft, &st);
}

inline void SysTimeToTime(const SYSTEMTIME& st, tm& tm)
{
	tm.tm_year = st.wYear - 1900;
	tm.tm_mon = st.wMonth - 1;
	tm.tm_mday = st.wDay;
	tm.tm_hour = st.wHour;
	tm.tm_min = st.wMinute;
	tm.tm_sec = st.wSecond;
	tm.tm_isdst = -1;
}

_MZH_END

#endif
