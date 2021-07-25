#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>

#include "head/Calendar.h"
#include "head/LunarCalendar.h"

#define CHAR_SIZE			20			// �ַ�������ĳ���
#define BACKGROUND_COLOR	FOREGROUND_INTENSITY | BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE 
static char date[CHAR_SIZE];			// ����������ꡢ�¡���

/* ========================��������======================== */
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// ��ȡ��ǰϵͳʱ��
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
inline static struct tm GetCurrentDate()
{	
	time_t now = 0;
	struct tm* tm_now;
	time(&now);
	tm_now = gmtime(&now);		// ��ȡ��ǰϵͳʱ��
	tm_now->tm_year += 1900;	// ���ó���ȷ�����
	tm_now->tm_mon += 1;		// ���ó���ȷ���·�
	return *tm_now;
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// �ж��Ƿ������겢������Ӧ������ 
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
inline static void SetLeapYearsOfDays(int year)
{
	if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))		
		solarMonth[1] = 29;	// ����2�·���29��
	else
		solarMonth[1] = 28;	// ƽ��2�·���28��
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// ���������1900��1��1�Ŷ�����  
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
inline static int CalcFromStartDays(int year, int month)
{
	int i, sum = 0;
	/* ������ݵ�����, �����366��,ƽ���365�� */
	for (i = 1900; i < year; i++)
	{
		if ((i % 4 == 0) && (i % 100 != 0) || (i % 400 == 0))
			sum += 366;		// �����366��
		else
			sum += 365;		// ƽ���365��
	}
	/* �����·ݵ����� */
	for (i = 0; i < month - 1; i++)
		sum += solarMonth[i];

	return sum;
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// �жϽ����Ƿ��ǽڼ���,�����·ݺ����ڱȽ�
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
inline static char* IsSameMonth(Holiday *hly_temp, int month, int day)
{
	char temp1[2], temp2[2];
	for (int i = 0; i < HOLIDAYSIZE; i++)
	{
		/* ��ȡ�ڼ��յ��·� */
		temp1[0] = hly_temp[i].date[0];
		temp1[1] = hly_temp[i].date[1];
		/* ��ȡ�ڼ��յ���������һ�� */
		temp2[0] = hly_temp[i].date[2];
		temp2[1] = hly_temp[i].date[3];
		if (atoi(temp1) == month && atoi(temp2) == day)
			return hly_temp[i].holidayName;
	}
	return NULL;
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// ������ʾ˫���� 
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
		/* ����������ɫΪ����ɫ */
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR | FOREGROUND_BLUE | FOREGROUND_GREEN);
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// ��ȡ���µĽ�������
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int* GetSolarTermsDate(int year, int month)
{
	int index = 0;			// �����±�
	int temp = 0;
	char tempArr1[1];
	char tempArr2[2];
	int* date = (int*)malloc(sizeof(int) * 2);
	char* solar_terms = solarTerms[year - 1900];	// ��ȡ����Ľ�����Ϣ
	char solar_terms_month[7] = "";					// �洢�����·ݵĽ�����Ϣ
	// 97783		7f0e3		7f149		98082		b0787		b0721	7f07e 7f0e4 7f531 b0723 b0b6fb0721
	// 620419	   520419	   520521	   622722	   722823	   722721
	// 1��2��	   3��4��	   5��6��      7��8��      9��10��     11��12��
//�±�:  0		     5           10          15           20		  25 
	/* ����������±� */
	if (month == 1 || month == 2)					// �����1�¡�2�·ݣ��±�����Ϊ0
		index = 0;
	else
	{
		/* ����·���ż��, �򽫵��º����µ��º�������� */
		if (month / 2 == 0)
			index = month + (month - 1) + 2;
		/* ���������, �򽫵��º���һ���µ��º��������*/
		else
			index = month + (month + 1) + 2;
		/* ��ø��·��������е��±� */
		index = index / 5 * 5;
	}
	/* ��õ�ǰ�·ݵĽ�����Ϣ,��ŵ�solar_terms_month�� */
	strncpy(solar_terms_month, solar_terms + index, 5);
	/* ��ʮ�����ַ���ת��Ϊʮ�����ַ��� */
	sscanf(solar_terms_month, "%x", &temp);
	sprintf(solar_terms_month, "%d", temp);
	
	/* ���������µĽ������� */
	if ((month % 2) != 0)							
	{
		tempArr1[0] = solar_terms_month[0];
		date[0] = atoi(tempArr1);					// ��һ����������
		tempArr2[0] = solar_terms_month[1];
		tempArr2[1] = solar_terms_month[2];
		date[1] = atoi(tempArr2);					// �ڶ�����������
	}
	/* ����ż���µĽ������� */
	else
	{
		tempArr1[0] = solar_terms_month[3];
		date[0] = atoi(tempArr1);					// ��һ����������
		tempArr2[0] = solar_terms_month[4];
		tempArr2[1] = solar_terms_month[5];
		date[1] = atoi(tempArr2);					// �ڶ�����������
	}
	return date;
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// ��ʾ����
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
char* ShowSolarTerms(int year, int month, int day)
{
	int* date = GetSolarTermsDate(year, month);
	/* ���ظ��µ�һ������������ */
	if (day == date[0])								
	{
		free(date);
		return solarTermsName[(month - 1) * 2];
	}
	/* ���ظ��µڶ������������� */
	if (day == date[1])								
	{
		free(date);
		return solarTermsName[(month - 1) * 2 + 1];
	}
	return NULL;
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// ��ʾ�ڼ��� 
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
char *ShowHolidays(int year, int month, int day)
{
	int index = 0, i = 0;
	char temp1[2], temp2[2];
	char* result = NULL;
	Holiday *hly_temp;
	if (year < STARTHOLIADY || year > ENDHOLIDAY)	// ֻ֧��2008�굽2021��
		return NULL;
	char **holidays = holidays3[year - 2008];		// ��ȡ����Ľڼ�����Ϣ

//***********************************************************************************************
// �����Ƿ���ũ�����̶����ڵĽڼ���,���紺�ڡ�����ڡ����������
//***********************************************************************************************
	hly_temp = holidays2[year - STARTHOLIADY];		// ��ȡ��������нڼ�����Ϣ
	result = IsSameMonth(hly_temp, month, day);
	if (result != NULL)
	{
		/* ����������ɫΪ��ɫ */
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR | FOREGROUND_RED);
		return result;
	}
	
//***********************************************************************************************
// �����Ƿ���һ�������ǵĻ����ؽ�������
//***********************************************************************************************
	hly_temp = holidays1[month - 1];				// ��ȡ���µ����нڼ�����Ϣ
	result = IsSameMonth(hly_temp, month, day);
	if (result != NULL)
	{
		/* ����������ɫΪ��ɫ */
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR | FOREGROUND_RED);
		return result;
	}

//***********************************************************************************************
// �жϽ����Ƿ��ǽڼ����е�ĳһ��
//***********************************************************************************************

	for (i = 0; i < 7; i++)
	{
		temp1[0] = holidays[i][4];
		temp1[1] = holidays[i][5];
		if (month == atoi(temp1))					// �жϵ�ǰ�·��Ƿ��нڼ���
		{
			result = holidays[i];					// ��ȡ���¾���ڼ�������
			/* ��ȡ�ýڼ��յ���ʼ���� */
			temp1[0] = result[6];
			temp1[1] = result[7];
			/* ��ȡ�ýڼ��յĽ������� */
			temp2[0] = result[14];
			temp2[1] = result[15];

			if (day >= atoi(temp1) && day <= atoi(temp2))
			{
				/* ����������ɫΪ��ɫ */
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR | FOREGROUND_RED);
				return NULL;
			}
		}
	}
	return NULL;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// ��ʾÿ����һ��ũ���ļ���,������ʾ��Ӧ������ 
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
char* ShowLunarMonth(int year, int month, int day, int leap_month)
{
	int i = 0;
	int* date = GetSolarTermsDate(year, month);				// ��ȡ���µ�������������

	/* ��ȡ��һ������Ӧ������±�,�±�ʹ�����ũ���ļ��·� */
	for (i = 0; i < 48; i++)
	{
		//printf("%s\t%s\n", solarTermsName[(month - 1) * 2], solarTermsName[i]);
		if (!strcmp(solarTermsName[(month - 1) * 2], solarTermsName[i]))
			break;
	}

	i /= 2;													// ��Ϊһ��������������,���Գ���2����ũ����Ӧ�ļ��·�	
	if (i >= 11)
		i = i - 11;
	/* ��һ�ڸ���������������֮�� */
	if (day > date[1])
	{
		if (leap_month != 0 && i == leap_month)
			return lunar_calendar_month[11 + i + 1];
		else
			return lunar_calendar_month[i + 1];
	}
	/* ��һ�ڸ�����������֮ǰ */
	else
	{
		if (leap_month != 0 && i == leap_month)
			return lunar_calendar_month[11 + i];			// ��ȡ��һ��ũ���·�����
		else 
			return lunar_calendar_month[i];
	}
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// ��ӡ������Ϣ 
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int PrintSingleMonthInfo(int year, int month)
{
	if (year > 2050 || year < 1900)							// ֻ��ʾ1900-2500֮��������Ϣ
		return 1;
	int i, j, k;
	int count = 0, days;
	char temp[2];
	char* holiday = NULL;									// �洢��������
	char* solar_terms = NULL;								// �洢��������
	char* lunar_month = NULL;								// �洢ÿ�³�һ��ũ���ļ���,����������
	int freeDays = 0;										// ��ǰ�·�ͷ���������
	int firstDay = 0;										// �Ӹ��µ����ڼ���1��
	int lunarCalendarIndex = 0;								// �����ĵ�һ����ũ������
	int currentDays = solarMonth[month - 1];				// ��ȡ���µĹ�������
	int fromStartDays = CalcFromStartDays(year, month);		// ����1900���������
	int leap_month = lunarCalendarInfo[year - 1900] & 0xF;	// ������ʲô����, &0xF��ʾȡʮ���Ƹ�λ�ϵ���
	/* ������µ�1����ũ���ļ���,���lunarCalendar������±� */
	temp[0] = lunarDate[year - 1900][(month - 1) * 2];
	temp[1] = lunarDate[year - 1900][(month - 1) * 2 + 1];
	lunarCalendarIndex = atoi(temp) - 1;
	/* ��ø��µ���һ�µ�1����ũ��ʲô���� */
	if (month == 12)										// ���������12��ͻ�ȡ��һ����·�1�·�
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
	
	printf("\t%d��%d��\t\t\t\t\t      \n", year, month);
	printf("\t-----------------------------------------------------\n");
	/* ����������ɫΪ��ɫ */
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR | FOREGROUND_BLUE | FOREGROUND_RED);
	printf("\tһ\t��\t��\t��\t��\t��\t��    \n");
	/* ������ָ�Ϊ�׵׺��� */
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR);
	printf("\t-----------------------------------------------------\n\t");
	freeDays = fromStartDays % 7;
	firstDay = 7 - freeDays;
	/* �·�ͷ����������ÿո���� */
	for (i = 0; i < freeDays; i++)
		printf("\t");
	for (i = 1; i < currentDays + 1; i++)
	{
		int index = 1;
		/* ����������ɫΪ��ɫ */
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR | FOREGROUND_BLUE);
		ShowHiglightWeekend(year, month, i);
		printf("%d\t", i);
		count++;
		if (i == firstDay || (i - firstDay) % 7 == 0 || i == currentDays)
		{
			printf("\n      ");	// ����
			/* �������һЩ�ո��Ա��ڹ�����Ӧ����������ʾũ�� */
			if (i == firstDay)
			{
				for (j = 0; j < freeDays; j++)
					printf("        ");
			}
//***********************************************************************************************
// ��ӡ�ڼ�����Ϣ
//***********************************************************************************************
			/* ����������ɫΪĬ�ϵĺ�ɫ */
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR);
			for (j = 0; j < count; j++)
			{
				if ((i - (count - j)) == days)				// �ж�ũ���Ƿ���һ���·���,�������ӳ�һ��ʼ��ӡ
					lunarCalendarIndex = 0;

				/* �����ǰ�����ǽڼ��մ�ӡ�ڼ������Ʋ����������������Ϊ��ɫ */
				holiday =  ShowHolidays(year, month, i - count + 1 + j);
				if (holiday != NULL)
				{				
					
					printf("%s", holiday);					// ��ʾ��������
					if ((strlen(holiday) / 2) >= 3)
						printf(" ");
					else if (strspn(holiday, "0123456789") == strlen(holiday))		// �жϽ����Ƿ�������
						for (k = 0; k < (strlen(holiday) * 2 - 1); k++)
							printf(" ");
					else 
						for (k = 0; k < strlen(holiday); k++)
							printf(" ");
				}
//***********************************************************************************************
// ��ӡ������Ϣ
//***********************************************************************************************
				else if ((solar_terms = ShowSolarTerms(year, month, i - count + 1 + j)) != NULL)
				{
					/* ����������ɫΪ��ɫ */
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR | FOREGROUND_GREEN);
					printf(" %s", solar_terms);
					for (k = 0; k < (strlen(solar_terms) / 2); k++)
						printf(" ");
				}
//***********************************************************************************************
// ��ӡ��������������
//***********************************************************************************************
				else
				{
					if (!strcmp(lunarCalendar[lunarCalendarIndex], "��һ"))			// ��ȡ��һ��ũ���ļ���
						lunar_month = ShowLunarMonth(year, month, i - count + 1 + j, leap_month);

					if (lunar_month != NULL)
					{
						/* ����������ɫΪ��ɫ */
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR | FOREGROUND_GREEN);
						printf(" %s   ", lunar_month);								// ��ӡũ����һ�Ǽ��·�
					}
					else 
						printf(" %s   ", lunarCalendar[lunarCalendarIndex]);
					lunar_month = NULL;
				}
				lunarCalendarIndex++;						// ũ�����ڼ�һ
				/* ����������ɫΪĬ�ϵĺ�ɫ */	
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_COLOR);
			}
			count = 0;
			printf("\n\n\t");
		}
	}
	printf("\n");
}



//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// ��ʾ����
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void ShowCalendar(int year, int month)
{
	// system("cls");							// ����
	SetLeapYearsOfDays(year);				// �ж��Ƿ�������,�������������Ϊ���������
	if (month != 0)							// ��ʾ������Ϣ
	{
		PrintSingleMonthInfo(year, month);

	}
}


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// ������������
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void PerpetualCalendar()
{
	system("color F0");						// ����Ϊ�׵׺���
	struct tm tm_now = GetCurrentDate();	// ��ȡ��ǰϵͳʱ��
	ShowCalendar(2014, 9);			
	// ShowCalendar(tm_now.tm_year, tm_now.tm_mon);			// ����ϵͳʱ����ʾ��Ӧ��������Ϣ,������Ҫ���Խ�ע��ɾ��ִ�д˴���
}