#ifndef __CALENDAR_H__
#define __CALENDAR_H__


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// ��������              
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>    
/*
 *	������������
 */
void PerpetualCalendar();

/*
 * ��ʾ����
 */
void ShowCalendar(int year, int month);

/*
 *	��ʾ�ڼ���
 */
char* ShowHolidays(int year, int month, int day);

/*
 *	��ӡ������Ϣ
 */
int PrintSingleMonthInfo(int year, int month);


#endif 