#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>

#include "head/Calendar.h"
#include "head/LunarCalendar.h"

#define CHAR_SIZE			20			// 字符串数组的长度
#define BACKGROUND_COLOR	FOREGROUND_INTENSITY | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE 
static char date[CHAR_SIZE];			// 接受输入的年、月、日

/* ========================函数定义======================== */
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 获取当前系统时间
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
inline static struct tm GetCurrentDate()
{	
	time_t now = 0;
	struct tm* tm_now;
	time(&now);
	tm_now = gmtime(&now);		// 获取当前系统时间
	tm_now->tm_year += 1900;	// 设置成正确的年份
	tm_now->tm_mon += 1;		// 设置成正确的月份
	return *tm_now;
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 判断是否是闰年并设置相应的天数 
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
inline static void SetLeapYearsOfDays(int year)
{
	if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))		
		solarMonth[1] = 29;	// 闰年2月份有29天
	else
		solarMonth[1] = 28;	// 平年2月份有28天
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 计算该年离1900年1月1号多少天  
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
inline static int CalcFromStartDays(int year, int month)
{
	int i, sum = 0;
	/* 计算年份的天数, 闰年加366天,平年加365天 */
	for (i = 1900; i < year; i++)
	{
		if ((i % 4 == 0) && (i % 100 != 0) || (i % 400 == 0))
			sum += 366;		// 闰年加366天
		else
			sum += 365;		// 平年加365天
	}
	/* 计算月份的天数 */
	for (i = 0; i < month - 1; i++)
		sum += solarMonth[i];

	return sum;
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 判断今天是否是节假日,进行月份和日期比较
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
inline static char* IsSameMonth(Holiday *hly_temp, int month, int day)
{
	char temp1[2], temp2[2];
	for (int i = 0; i < HOLIDAYSIZE; i++)
	{
		/* 获取节假日的月份 */
		temp1[0] = hly_temp[i].date[0];
		temp1[1] = hly_temp[i].date[1];
		/* 获取节假日的日期是哪一天 */
		temp2[0] = hly_temp[i].date[2];
		temp2[1] = hly_temp[i].date[3];
		if (atoi(temp1) == month && atoi(temp2) == day)
			return hly_temp[i].holidayName;
	}
	return NULL;
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 高亮显示双休日 
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void ShowHiglightWeekend(int year, int month, int day)
{
	if (month == 1 || month == 2)
	{
		month += 12;
		day -= 1;
	}
	int week = (day + 2 * month + 3 * (month + 1) / 5 + year + year / 4 - year / 100 + year / 400) % 7 + 1;
	if (week == 6 || week == 7)
		/* 设置字体颜色为淡蓝色 */
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR | FOREGROUND_BLUE | FOREGROUND_GREEN);
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 获取该月的节气日期
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int* GetSolarTermsDate(int year, int month)
{
	int index = 0;			// 数组下标
	int temp = 0;
	char tempArr1[1];
	char tempArr2[2];
	int* date = (int*)malloc(sizeof(int) * 2);
	char* solar_terms = solarTerms[year - 1900];	// 获取该年的节气信息
	char solar_terms_month[7] = "";					// 存储两个月份的节气信息
	// 97783		7f0e3		7f149		98082		b0787		b0721	7f07e 7f0e4 7f531 b0723 b0b6fb0721
	// 620419	   520419	   520521	   622722	   722823	   722721
	// 1月2月	   3月4月	   5月6月      7月8月      9月10月     11月12月
//下标:  0		     5           10          15           20		  25 
	/* 设置数组的下标 */
	if (month == 1 || month == 2)					// 如果是1月、2月份，下标设置为0
		index = 0;
	else
	{
		/* 如果月份是偶数, 则将当月和上月的月号相加起来 */
		if (month / 2 == 0)
			index = month + (month - 1) + 2;
		/* 如果是奇数, 则将当月和下一个月的月号相加起来*/
		else
			index = month + (month + 1) + 2;
		/* 获得该月份在数组中的下标 */
		index = index / 5 * 5;
	}
	/* 获得当前月份的节气信息,存放到solar_terms_month中 */
	strncpy(solar_terms_month, solar_terms + index, 5);
	/* 将十六进字符串转换为十进制字符串 */
	sscanf(solar_terms_month, "%x", &temp);
	sprintf(solar_terms_month, "%d", temp);
	
	/* 返回奇数月的节气日期 */
	if ((month % 2) != 0)							
	{
		tempArr1[0] = solar_terms_month[0];
		date[0] = atoi(tempArr1);					// 第一个节气日期
		tempArr2[0] = solar_terms_month[1];
		tempArr2[1] = solar_terms_month[2];
		date[1] = atoi(tempArr2);					// 第二个节气日期
	}
	/* 返回偶数月的节气日期 */
	else
	{
		tempArr1[0] = solar_terms_month[3];
		date[0] = atoi(tempArr1);					// 第一个节气日期
		tempArr2[0] = solar_terms_month[4];
		tempArr2[1] = solar_terms_month[5];
		date[1] = atoi(tempArr2);					// 第二个节气日期
	}
	return date;
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 显示节气
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
char* ShowSolarTerms(int year, int month, int day)
{
	int* date = GetSolarTermsDate(year, month);
	/* 返回该月第一个节气的名称 */
	if (day == date[0])								
	{
		free(date);
		return solarTermsName[(month - 1) * 2];
	}
	/* 返回该月第二个节气的名称 */
	if (day == date[1])								
	{
		free(date);
		return solarTermsName[(month - 1) * 2 + 1];
	}
	return NULL;
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 显示节假日 
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
char *ShowHolidays(int year, int month, int day)
{
	int index = 0, i = 0;
	char temp1[2], temp2[2];
	char* result = NULL;
	Holiday *hly_temp;
	if (year < STARTHOLIADY || year > ENDHOLIDAY)	// 只支持2008年到2021年
		return NULL;
	char **holidays = holidays3[year - 2008];		// 获取该年的节假日信息

//***********************************************************************************************
// 今天是否是农历不固定日期的节假日,比如春节、端午节、中秋节这种
//***********************************************************************************************
	hly_temp = holidays2[year - STARTHOLIADY];		// 获取该年的所有节假日信息
	result = IsSameMonth(hly_temp, month, day);
	if (result != NULL)
	{
		/* 设置字体颜色为红色 */
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR | FOREGROUND_RED);
		return result;
	}
	
//***********************************************************************************************
// 今天是否是一个节日是的话返回节日名称
//***********************************************************************************************
	hly_temp = holidays1[month - 1];				// 获取该月的所有节假日信息
	result = IsSameMonth(hly_temp, month, day);
	if (result != NULL)
	{
		/* 设置字体颜色为红色 */
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR | FOREGROUND_RED);
		return result;
	}

//***********************************************************************************************
// 判断今天是否是节假日中的某一天
//***********************************************************************************************

	for (i = 0; i < 7; i++)
	{
		temp1[0] = holidays[i][4];
		temp1[1] = holidays[i][5];
		if (month == atoi(temp1))					// 判断当前月份是否有节假日
		{
			result = holidays[i];					// 获取该月具体节假日日期
			/* 获取该节假日的起始日期 */
			temp1[0] = result[6];
			temp1[1] = result[7];
			/* 获取该节假日的截至日期 */
			temp2[0] = result[14];
			temp2[1] = result[15];

			if (day >= atoi(temp1) && day <= atoi(temp2))
			{
				/* 设置字体颜色为红色 */
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR | FOREGROUND_RED);
				return NULL;
			}
		}
	}
	return NULL;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 显示每个初一是农历的几月,或者显示对应的闰几月 
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
char* ShowLunarMonth(int year, int month, int day, int leap_month)
{
	int i = 0;
	int* date = GetSolarTermsDate(year, month);				// 获取该月的两个节气日期

	/* 获取第一节气对应数组的下标,下标就代表着农历的几月份 */
	for (i = 0; i < 48; i++)
	{
		//printf("%s\t%s\n", solarTermsName[(month - 1) * 2], solarTermsName[i]);
		if (!strcmp(solarTermsName[(month - 1) * 2], solarTermsName[i]))
			break;
	}

	i /= 2;													// 因为一个月有两个节气,所以除以2才是农历对应的几月份	
	if (i >= 11)
		i = i - 11;
	/* 初一在该月两个节气日期之后 */
	if (day > date[1])
	{
		if (leap_month != 0 && i == leap_month)
			return lunar_calendar_month[11 + i + 1];
		else
			return lunar_calendar_month[i + 1];
	}
	/* 初一在该月两个节气之前 */
	else
	{
		if (leap_month != 0 && i == leap_month)
			return lunar_calendar_month[11 + i];			// 获取下一个农历月份名称
		else 
			return lunar_calendar_month[i];
	}
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 打印单月信息 
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int PrintSingleMonthInfo(int year, int month)
{
	if (year > 2050 || year < 1900)							// 只显示1900-2500之间的年份信息
		return 1;
	int i, j, k;
	int count = 0, days;
	char temp[2];
	char* holiday = NULL;									// 存储节日名称
	char* solar_terms = NULL;								// 存储节气名称
	char* lunar_month = NULL;								// 存储每月初一是农历的几月,或者是闰几月
	int freeDays = 0;										// 当前月份头空余的天数
	int firstDay = 0;										// 从该月的星期几是1号
	int lunarCalendarIndex = 0;								// 公历的第一天是农历几号
	int currentDays = solarMonth[month - 1];				// 获取该月的公历天数
	int fromStartDays = CalcFromStartDays(year, month);		// 距离1900年的总天数
	int leap_month = lunarCalendarInfo[year - 1900] & 0xF;	// 该年有什么闰月, &0xF表示取十进制个位上的数
	/* 计算该月的1号是农历的几号,获得lunarCalendar数组的下标 */
	temp[0] = lunarDate[year - 1900][(month - 1) * 2];
	temp[1] = lunarDate[year - 1900][(month - 1) * 2 + 1];
	lunarCalendarIndex = atoi(temp) - 1;
	/* 获得该月的下一月的1号是农历什么日子 */
	if (month == 12)										// 如果该月是12年就获取下一年的月份1月份
	{
		i = year - 1900 + 1;
		j = 0;
	}
	else
	{
		i = year - 1900;
		j = month * 2;
	}
	temp[0] = lunarDate[i][j];	
	temp[1] = lunarDate[i][j + 1];	
	days = currentDays - atoi(temp) + 1;
	
	printf("\t%d年%d月\t\t\t\t\t      \n", year, month);
	printf("\t-----------------------------------------------------\n");
	/* 设置字体颜色为橙色 */
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR | FOREGROUND_BLUE | FOREGROUND_RED);
	printf("\t一\t二\t三\t四\t五\t六\t日    \n");
	/* 将字体恢复为白底黑字 */
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR);
	printf("\t-----------------------------------------------------\n\t");
	freeDays = fromStartDays % 7;
	firstDay = 7 - freeDays;
	/* 月份头空余的天数用空格代替 */
	for (i = 0; i < freeDays; i++)
		printf("\t");
	for (i = 1; i < currentDays + 1; i++)
	{
		int index = 1;
		/* 设置字体颜色为蓝色 */
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR | FOREGROUND_BLUE);
		ShowHiglightWeekend(year, month, i);
		printf("%d\t", i);
		count++;
		if (i == firstDay || (i - firstDay) % 7 == 0 || i == currentDays)
		{
			printf("\n      ");	// 换行
			/* 首先输出一些空格以便在公历对应的日期下显示农历 */
			if (i == firstDay)
			{
				for (j = 0; j < freeDays; j++)
					printf("        ");
			}
//***********************************************************************************************
// 打印节假日信息
//***********************************************************************************************
			/* 设置字体颜色为默认的黑色 */
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR);
			for (j = 0; j < count; j++)
			{
				if ((i - (count - j)) == days)				// 判断农历是否到下一个月份了,如果是则从初一开始打印
					lunarCalendarIndex = 0;

				/* 如果当前日期是节假日打印节假日名称并将假期日期字体变为红色 */
				holiday =  ShowHolidays(year, month, i - count + 1 + j);
				if (holiday != NULL)
				{				
					
					printf("%s", holiday);					// 显示节日名称
					if ((strlen(holiday) / 2) >= 3)
						printf(" ");
					else if (strspn(holiday, "0123456789") == strlen(holiday))		// 判断节日是否是数字
						for (k = 0; k < (strlen(holiday) * 2 - 1); k++)
							printf(" ");
					else 
						for (k = 0; k < strlen(holiday); k++)
							printf(" ");
				}
//***********************************************************************************************
// 打印节气信息
//***********************************************************************************************
				else if ((solar_terms = ShowSolarTerms(year, month, i - count + 1 + j)) != NULL)
				{
					/* 设置字体颜色为绿色 */
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR | FOREGROUND_GREEN);
					printf(" %s", solar_terms);
					for (k = 0; k < (strlen(solar_terms) / 2); k++)
						printf(" ");
				}
//***********************************************************************************************
// 打印正常的日历日期
//***********************************************************************************************
				else
				{
					if (!strcmp(lunarCalendar[lunarCalendarIndex], "初一"))			// 获取初一是农历的几月
						lunar_month = ShowLunarMonth(year, month, i - count + 1 + j, leap_month);

					if (lunar_month != NULL)
					{
						/* 设置字体颜色为绿色 */
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR | FOREGROUND_GREEN);
						printf(" %s   ", lunar_month);								// 打印农历初一是几月份
					}
					else 
						printf(" %s   ", lunarCalendar[lunarCalendarIndex]);
					lunar_month = NULL;
				}
				lunarCalendarIndex++;						// 农历日期加一
				/* 设置字体颜色为默认的黑色 */	
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR);
			}
			count = 0;
			printf("\n\n\t");
		}
	}
	printf("\n");
}



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 显示日历
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void ShowCalendar(int year, int month)
{
	// system("cls");							// 清屏
	SetLeapYearsOfDays(year);				// 判断是否是闰年,如果是闰年设置为闰年的天数
	if (month != 0)							// 显示单月信息
	{
		PrintSingleMonthInfo(year, month);

	}
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 万年历主函数
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void PerpetualCalendar()
{
	system("color F0");						// 设置为白底黑字
	struct tm tm_now = GetCurrentDate();	// 获取当前系统时间
	ShowCalendar(2014, 9);			
	// ShowCalendar(tm_now.tm_year, tm_now.tm_mon);			// 根据系统时间显示对应的日历信息,根据需要可以将注释删掉执行此代码
}