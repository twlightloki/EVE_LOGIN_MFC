#include "stdafx.h"
#include "Tools.h"

INT CalcTools::RemainDays(SYSTEMTIME *time2)
{
	SYSTEMTIME time1;
	GetSystemTime(&time1);
	FILETIME ftime1, ftime2;
	time_t i64t1, i64t2;
	SystemTimeToFileTime(&time1, &ftime1);
	SystemTimeToFileTime(time2, &ftime2);
	FileTimeToTimeT(&ftime1, &i64t1);
	FileTimeToTimeT(&ftime2, &i64t2);
	return(ceil(DOUBLE(i64t2 - i64t1) / (24 * 3600)));
}

void CalcTools::FileTimeToTimeT(FILETIME *ftime, time_t *timet)
{
	ULARGE_INTEGER uiTmp;
	uiTmp.LowPart = ftime->dwLowDateTime;
	uiTmp.HighPart = ftime->dwHighDateTime;
	*timet = ((LONGLONG)(uiTmp.QuadPart - 116444736000000000) / 10000000);
}

void CalcTools::IncreasedDays(const INT nIncDays, SYSTEMTIME *time2)
{
	SYSTEMTIME time1;
	GetSystemTime(&time1);
	time_t i64t1,i64t2 = (time_t)nIncDays * 24 * 3600;
	FILETIME ftime1, ftime2;
	SystemTimeToFileTime(&time1, &ftime1);
	FileTimeToTimeT(&ftime1, &i64t1);
	i64t1 += i64t2;
	TimeTToFileTime(&i64t1, &ftime2);
	FileTimeToSystemTime(&ftime2, time2);
}

void CalcTools::TimeTToFileTime(time_t *timet, FILETIME *ftime)
{
	ULARGE_INTEGER uiTmp;
	
	uiTmp.QuadPart = (ULONGLONG)(*timet) * 10000000 + 116444736000000000;
	ftime->dwHighDateTime = uiTmp.HighPart;
	ftime->dwLowDateTime = uiTmp.LowPart;

}