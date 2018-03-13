#ifndef __RTC_H
#define __RTC_H	 
#include "common.h" 

//////////////////////////////////////////////////////////////////////////////////	 

extern RTC_TimeTypeDef RTC_TimeStruct;
extern RTC_DateTypeDef RTC_DateStruct;
	
u8 RTC_InitConfig(void);						//RTC初始化

u8 RTC_GetWeek(u8 wyear,u8 wmonth,u8 wday);
ErrorStatus RTC_SetTimes(u8 year,u8 month,u8 date,u8 hour,u8 min,u8 sec);
void RTC_GetTimes(uint32_t RTC_Format);

void RTC_SetAlarmA(u8 week,u8 hour,u8 min,u8 sec);	//设置闹钟时间(按星期闹铃,24小时制)
void RTC_SetWakeUp(u32 wktime,u16 autodata);				//设置唤醒定时器的时间

#endif

















