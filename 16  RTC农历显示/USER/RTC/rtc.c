#include "rtc.h"
#include "led.h"

/*********************************************************************************
*************************MCU启明 STM32F407核心开发板******************************
**********************************************************************************
* 文件名称: rtc.c                                                                *
* 文件简述：RTC初始化与使用                                                      *
* 创建日期：2015.03.11                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：RTC初始化与使用                                                      * 
**********************************************************************************
*********************************************************************************/

//RTC时钟数据结构体，时间跟日期分开
RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;

NVIC_InitTypeDef   NVIC_InitStructure;

									 
u8 const month_amendBuf[12]={0,3,3,6,1,4,6,2,5,0,3,5}; 
/****************************************************************************
* 名    称: u8 RTC_GetWeek(u16 wyear,u8 wmonth,u8 wday)
* 功    能：获得某天是星期几
* 入口参数：wyear：年(最大99)  wmonth：月  wday：日
* 返回参数：星期几
* 说    明：从2000~2099有效
            返回值1-7依次对应星期一到星期天  
            该函数用于设置时间时，无需输入星期几，取时间星期几时无需使用该函数，
            因直接读取日期寄存器的星期值就行
            注意形参“年”是8位，即要输入2位的年的参数  15：合法    2015：不合法
****************************************************************************/																						 
u8 RTC_GetWeek(u8 wyear,u8 wmonth,u8 wday)
{	
	u16 middata;
	u8 yearL;
	
	yearL=wyear+100; 	 //从2000年开始，加100

	middata=yearL+yearL/4;
	middata=middata%7; 
	middata=middata+wday+month_amendBuf[wmonth-1];
	if (yearL%4==0&&wmonth<3)middata--;
	
	if((middata%7==0)) return 7;       //与STM32F103的rtc例程不同，该函数星期天返回值为7
	return(middata%7);
}	
/****************************************************************************
* 名    称: ErrorStatus RTC_SetTimes(u8 year,u8 month,u8 date,u8 hour,u8 min,u8 sec)
* 功    能：设置RTC时间
* 入口参数：年月日时分秒
* 返回参数：成功与否  1：成功   0：失败
* 说    明：注意形参“年”是16位，即要输入4位的年的参数  15：合法    2015：不合法    
****************************************************************************/	
ErrorStatus RTC_SetTimes(u8 year,u8 month,u8 date,u8 hour,u8 min,u8 sec)
{
  RTC_DateTypeDef RTC_DateTypeInitStructure;
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	
	RTC_DateTypeInitStructure.RTC_Date=date;
	RTC_DateTypeInitStructure.RTC_Month=month;
	RTC_DateTypeInitStructure.RTC_WeekDay=RTC_GetWeek(year,month,date);
	//RTC_DateTypeInitStructure.RTC_WeekDay=week;
	RTC_DateTypeInitStructure.RTC_Year=year;

	
	RTC_TimeTypeInitStructure.RTC_Hours=hour;
	RTC_TimeTypeInitStructure.RTC_Minutes=min;
	RTC_TimeTypeInitStructure.RTC_Seconds=sec;
	
	if(hour>12)
	     RTC_TimeTypeInitStructure.RTC_H12=RTC_H12_PM;
	else RTC_TimeTypeInitStructure.RTC_H12=RTC_H12_AM;
	
	return ( RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure) && RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure) );
}
/****************************************************************************
* 名    称: void RTC_GetTimes(uint32_t RTC_Format)
* 功    能：读取RTC时间
* 入口参数：RTC_Format：读取时间数据的数据类型  RTC_Format_BIN：字节型  RTC_Format_BCD：BCD码数据类型
* 返回参数：无
* 说    明：     
****************************************************************************/
void RTC_GetTimes(uint32_t RTC_Format)
{
		RTC_GetDate(RTC_Format,&RTC_DateStruct);
	  RTC_GetTime(RTC_Format,&RTC_TimeStruct);
}

//第一次需要配置RTC时调用该函数
u8 RTC_Configuration(void)
{
  u16 wait=0;  
	RTC_InitTypeDef RTC_InitStructure;
	
	  RCC_LSEConfig(RCC_LSE_ON);   //开启LSE   
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
			{
			wait++;
				delay_ms(5);
				if(wait>=500)return 1;      //如超过2.5s晶振还没就绪,晶振有问题	 
			}
			
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟 

    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC异步分频系数(1~0X7F)
    RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//RTC同步分频系数(0~7FFF)
    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//设置时间格式为：24小时制
    RTC_Init(&RTC_InitStructure);
			
		RTC_SetTimes(15,3,11,13,02,30);	
		return 0;
}
/****************************************************************************
* 名    称: u8 RTC_InitConfig(void)
* 功    能：RTC初始化设置
* 入口参数：无
* 返回参数：成功与否 0：成功  1：失败
* 说    明： 
****************************************************************************/
u8 RTC_InitConfig(void)
{
  u8 i=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟
	PWR_BackupAccessCmd(ENABLE);	 //使能后备寄存器 
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5555)		//是否第一次配置
	{
		i=RTC_Configuration();
		if(i==1) return 1;  //RTC配置失败
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x5555);	  //标记初始化过了
	} 
  RTC_SetWakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);	//配置WAKE UP中断,1秒钟中断一次
	return 0;
}
/****************************************************************************
* 名    称: void RTC_SetWakeUp(u32 wksel,u16 cnt)
* 功    能：设置唤醒定时器的时间
* 入口参数：wktime：唤醒时间   autodata：自动重装载值
* 返回参数：无
* 说    明：设置该函数的唤醒时间为1S中断一次，相当于STM32F103例程中的秒中断     
****************************************************************************/
void RTC_SetWakeUp(u32 wktime,u16 autodata)
{ 
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RTC_WakeUpCmd(DISABLE);        //关闭WAKE UP
	
	RTC_WakeUpClockConfig(wktime); //唤醒时钟选择
	
	RTC_SetWakeUpCounter(autodata);//设置WAKE UP自动重装载寄存器
		
	RTC_ClearITPendingBit(RTC_IT_WUT);  //清除RTC WAKE UP的标志
  EXTI_ClearITPendingBit(EXTI_Line22);//清除LINE22上的中断标志位 
	 
	RTC_ITConfig(RTC_IT_WUT,ENABLE); //开启WAKE UP定时器中断
	RTC_WakeUpCmd( ENABLE);          //开启WAKE UP 定时器　
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;            //LINE22
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    //中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;              //使能LINE22
  EXTI_Init(&EXTI_InitStructure);                        //配置
 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;        //子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);                              //配置
}
/****************************************************************************
* 名    称: void RTC_WKUP_IRQHandler(void)
* 功    能：RTC WAKE UP中断服务函数
* 入口参数：无
* 返回参数：无
* 说    明：相当于秒中断
****************************************************************************/
void RTC_WKUP_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_WUTF)==SET) //WK_UP中断
	{ 
		RTC_ClearFlag(RTC_FLAG_WUTF);	          //清除中断标志
		LED0=!LED0; 
	}   
	EXTI_ClearITPendingBit(EXTI_Line22);      //清除中断线22的中断标志 								
}

/****************************************************************************
* 名    称: void RTC_SetAlarmA(u8 week,u8 hour,u8 min,u8 sec)
* 功    能：设置闹钟时间(按星期闹铃,24小时制)
* 入口参数：week:星期几(1~7) 时分秒
* 返回参数：无
* 说    明： 
****************************************************************************/
void RTC_SetAlarmA(u8 week,u8 hour,u8 min,u8 sec)
{ 
	EXTI_InitTypeDef   EXTI_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmTypeInitStructure;
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);  //先关闭闹钟A 
	
  RTC_TimeTypeInitStructure.RTC_Hours=hour; 
	RTC_TimeTypeInitStructure.RTC_Minutes=min; 
	RTC_TimeTypeInitStructure.RTC_Seconds=sec; 
	RTC_TimeTypeInitStructure.RTC_H12=RTC_H12_AM;
  
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDay=week;                               //星期
	RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_WeekDay; //按星期闹
	RTC_AlarmTypeInitStructure.RTC_AlarmMask=RTC_AlarmMask_None;                        //匹配星期，时分秒
	RTC_AlarmTypeInitStructure.RTC_AlarmTime=RTC_TimeTypeInitStructure;
  RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmTypeInitStructure);
 
	RTC_ClearITPendingBit(RTC_IT_ALRA);  //清除RTC闹钟A的标志
  EXTI_ClearITPendingBit(EXTI_Line17); //清除LINE17上的中断标志位 
	
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);    //开启闹钟A中断
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);    //开启闹钟A 
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;            //LINE17
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;    //中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;              //使能LINE17
  EXTI_Init(&EXTI_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; //抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;        //子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;              //使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);                              //配置
}

//RTC闹钟中断服务函数
void RTC_Alarm_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET) //ALARM A中断  
	{
		RTC_ClearFlag(RTC_FLAG_ALRAF);           //清除中断标志
	}   
	EXTI_ClearITPendingBit(EXTI_Line17);	     //清除中断线17的中断标志 											 
}



