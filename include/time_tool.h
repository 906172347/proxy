#ifndef _TIME_TOOL_H
#define _TIME_TOOL_H
void format_date( uchar *Ptd, uchar *Pts );
void vGetHostTime( char *pszTime );
void Tm2Str(struct tm *tmval,char *sDate);
void Str2Tm(char *sDate,struct tm *tmval);
int Second2Day(double dSecond);
int DiffDay(char *sDate1,char *sDate2);
void GetNextDay(char *sToday,char *sNextDay);
int IsWeekEnd(char *sDate);
int IsMonEnd(char *sDate);
int IsYearEnd(char *sDate);
void AddDay(char *sOutDate,char *sInDate,int iAddDay);
void  AddMonth(char *sOutDate,char *sInDate,int iAddMon);
void  AddYear(char *sOutDate,char *sInDate,int iAddYear);
void GetMonEnd(char *MonEndDay,char *sDate);
void vGetHostTimeFor7Field( char *pszTime );
void vGetHostTimeFor11Field( char *pszTime );

#endif
