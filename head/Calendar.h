#ifndef __CALENDAR_H__
#define __CALENDAR_H__


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 函数声明              
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>    
/*
 *	万年历主函数
 */
void PerpetualCalendar();

/*
 * 显示日历
 */
void ShowCalendar(int year, int month);

/*
 *	显示节假日
 */
char* ShowHolidays(int year, int month, int day);

/*
 *	打印单月信息
 */
int PrintSingleMonthInfo(int year, int month);


#endif 