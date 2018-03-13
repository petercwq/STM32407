#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "malloc.h" 
#include "tfcard_sdio.h"    
#include "malloc.h" 
#include "w25qxx.h"    
#include "ff.h"     
#include "updatefont.h"  //调用HanZiUse.lib中更新字库函数
#include "showhz.h"	     //调用HanZiUse.lib中显示汉字函数

#include "rtc.h"
#include "lucalendar.h"   
/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 例程16 RTC农历显示                                                   *
* 文件简述：RTC使用                                                              *
* 创建日期：2015.03.15                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：长按按KEY3进行时间调整，按KEY0进行数值的加1，按KEY1数值进行减1，     *
            按KEY2右移选择要设置的选项，在调整完后短按KEY3，确认调整时间         *
* 淘宝店铺：https://shop125046348.taobao.com                                     *		
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/

u8 const *weekdate[7]={"一","二","三","四","五","六","日"};
u8 const *set_option[6]={"Year","Month","Day","Hours","Minutes","Seconds"};

//lcd上显示时间
void Time_Display()
{
	
 LCD_DisplayHZstr(30,210,16,"日期:20  -  -  ");
 LCD_DisplayNum(86,210,RTC_DateStruct.RTC_Year,2,16,1);
 LCD_DisplayNum(110,210,RTC_DateStruct.RTC_Month,2,16,1);
 LCD_DisplayNum(134,210,RTC_DateStruct.RTC_Date,2,16,1);
 LCD_DisplayHZstr(30,230,16,"时间:  :  :  ");
 LCD_DisplayNum(70,230,RTC_TimeStruct.RTC_Hours,2,16,1);	
 LCD_DisplayNum(94,230,RTC_TimeStruct.RTC_Minutes,2,16,1);
 LCD_DisplayNum(118,230,RTC_TimeStruct.RTC_Seconds,2,16,1);
 LCD_DisplayHZstr(30,250,16,"星期:        ");
 LCD_DisplayHZstr(70,250,16,(u8 *)weekdate[RTC_DateStruct.RTC_WeekDay-1]);
}
void Adjust_Time(u8 option,u8 shanshuo)
{
		if(shanshuo%50==0)  //提示现在要设置的选项
	  {
			LCD_DisplayString_color(20,180,24,"Please Set          ",BLUE,WHITE);
		  LCD_DisplayString_color(152,180,24,(u8 *)set_option[option],BLUE,WHITE);
		}
		
		//设置时数值的加减1  Calendar
		if( (keydown_data==KEY0_DATA)||(keydown_data==KEY1_DATA))
		{
		  if(keydown_data==KEY0_DATA)
			{
			  switch(option)
				{
				  case 0: RTC_DateStruct.RTC_Year+=1;   break;
					case 1: RTC_DateStruct.RTC_Month+=1;  break;
					case 2: RTC_DateStruct.RTC_Date+=1;   break;
					case 3: RTC_TimeStruct.RTC_Hours+=1;  break;
					case 4: RTC_TimeStruct.RTC_Minutes+=1;break;
					case 5: RTC_TimeStruct.RTC_Seconds+=1;break;
				}	
			}
			if(keydown_data==KEY1_DATA)
			{
			  switch(option)
				{
				  case 0: RTC_DateStruct.RTC_Year-=1;   break;
					case 1: RTC_DateStruct.RTC_Month-=1;  break;
					case 2: RTC_DateStruct.RTC_Date-=1;   break;
					case 3: RTC_TimeStruct.RTC_Hours-=1;  break;
					case 4: RTC_TimeStruct.RTC_Minutes-=1;break;
					case 5: RTC_TimeStruct.RTC_Seconds-=1;break;
				}	
			}
		}
		//闪烁显示：显示
	if(shanshuo==150)
		{
			Time_Display();
//			LCD_DisplayNum(86,220,RTC_DateStruct.RTC_Year,2,16,1);
//			LCD_DisplayNum(110,220,RTC_DateStruct.RTC_Month,2,16,1);
//			LCD_DisplayNum(134,220,RTC_DateStruct.RTC_Date,2,16,1);
//			LCD_DisplayNum(70,240,RTC_TimeStruct.RTC_Hours,2,16,1);
//			LCD_DisplayNum(94,240,RTC_TimeStruct.RTC_Minutes,2,16,1);
//			LCD_DisplayNum(118,240,RTC_TimeStruct.RTC_Seconds,2,16,1);
		}	
		//闪烁显示：不显示
	switch(option)
	  {  
			case 0: {  if(shanshuo==49)  LCD_DisplayString(86,210,16,"  ");	 break;  }
			case 1: {  if(shanshuo==49)  LCD_DisplayString(110,210,16,"  "); break;  }
			case 2: {  if(shanshuo==49)  LCD_DisplayString(134,210,16,"  "); break;  }
			case 3: {  if(shanshuo==49)  LCD_DisplayString(70,230,16,"  ");  break;  }
			case 4: {	 if(shanshuo==49)  LCD_DisplayString(94,230,16,"  ");	 break;	 }
			case 5: {	 if(shanshuo==49)  LCD_DisplayString(118,230,16,"  "); break;  }		
	  }
}	
int main(void)
{   
	u8 process=0;  //程序流程走向
  u8 option;     //调整时间选项
	u8 t=0,j;      //计时参数
	u8 *nonglibuf;
  u8 *jieqibuf;
	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();         //初始化延时函数
	LED_Init();					  //初始化LED
 	LCD_Init();					  //初始化LCD
	KEY_Init();           //初始化KEY
	RTC_InitConfig();		 	//初始化RTC
	W25QXX_Init();         
	
	font_init();   //字库初始化 之后才能显示汉字  必须确保W25Q128里面已经有字库
	
	Memory_Init(INSRAM);	
	nonglibuf=(u8 *)Mem_malloc(INSRAM,50);  //必须给指针变量赋地址值，不然第一个指针变量能获得编译器给了地址值而第二个就没值(经试验所得)
	jieqibuf=(u8 *)Mem_malloc(INSRAM,50);
	
	BRUSH_COLOR=RED;
	LCD_DisplayString(10,10,24,"Illuminati STM32F4");	
  LCD_DisplayString(10,40,16,"Author:Clever");
	LCD_DisplayHZstr(30,70,24,"14.RTC农历显示");
	LCD_DisplayHZstr(30,100,16,"KEY3 长按 :调整时间");
	LCD_DisplayHZstr(30,120,16,"     短按 :确认调整");
	LCD_DisplayHZstr(30,140,16,"KEY0:加1  KEY1: 减1");
	LCD_DisplayHZstr(30,160,16,"KEY2:右移一位");
	
	RTC_GetTimes(RTC_Format_BIN);
	Time_Display();
  	while(1) 
	{		
		key_scan(0);	
		t++;

		switch(process)
		{
			case 0:   // 流程0：时钟显示
			     {
						 if(key_tem==KEY3_DATA&&key_time>250)
							{
								process=1;   //长按KEY3 进入调整时间流程
								break;
							}
						 if((t%50)==0)	//每250ms更新显示
							{
								RTC_GetTimes(RTC_Format_BIN);
								Time_Display();
								GetLunarCalendarStr(RTC_DateStruct.RTC_Year+2000,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date,nonglibuf);
                LCD_DisplayHZstr(30,270,16,nonglibuf);
	
                GetJieQiStr(RTC_DateStruct.RTC_Year+2000,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date,jieqibuf);
                LCD_DisplayHZstr(30,290,16,jieqibuf);
							} 
						break;
			     }
			case 1:    // 流程1：调整时间
					{
					  if(keydown_data==KEY2_DATA) //按KEY2选项右移
						  {
						   option++;
							 if(option>5)   //循环
								 option=0;
						  }					
						Adjust_Time(option,t);//调整时间函数
						 if(keydown_data==KEY3_DATA)
						 {
						   Time_Display();// 防止在闪烁不显示的时候，短按KEY3确认调整后 显示空
							 RTC_SetTimes(RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date,RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
							 //设置时间需要5秒钟左右，有时会久一点
							 LCD_DisplayString_color(20,180,24,"Adjust OK          ",BLUE,WHITE);
							 for(j=0;j<100;j++) delay_ms(10);  // Adjust OK 调整OK显示1秒
							 LCD_DisplayString(20,180,24,"                    ");
							 option=0;    //选项从头来
							 process=0;   //短按KEY3时间设置完成 返回到时间显示
							 break;
						 }
 						break;
					}
	  }
		delay_ms(5); //系统延时，也算是系统计时
	}	
}





