#ifndef TOOLS_H
#define TOOLS_H

class CalcTools
{
public:

	INT RemainDays(SYSTEMTIME *time2);

	void IncreasedDays(const INT nIncDays, SYSTEMTIME *time2);

private:


	void FileTimeToTimeT(FILETIME *ftime, time_t *timet);

	void TimeTToFileTime(time_t *timet, FILETIME *ftime);
};




#endif