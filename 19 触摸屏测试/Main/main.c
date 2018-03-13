 #include "usart1.h"   
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "touch.h" 


/*********************************************************************************
*************************MCU启明 STM32F407核心应用板******************************
**********************************************************************************
* 文件名称: 例程19 触摸屏main.c                                                  *
* 文件简述：触摸屏实现一个画图板                                                 *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：触摸屏实现一个画图板                                                 *    
* 淘宝店铺：https://shop125046348.taobao.com                                     *
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/	


//清空屏幕
void Clear_Screen(void)
{
	LCD_Clear(WHITE);//清屏   
 	BRUSH_COLOR=BLUE;//设置字体为蓝色 
	LCD_DisplayString(lcd_width-40,lcd_height-18,16,"Clear");//显示清屏区域
  BRUSH_COLOR=RED;//设置画笔蓝色 
}

//电阻触摸屏测试函数
void R_Touch_test(void)
{
	u8 i=0;	  
	while(1)
	{
	 	key_scan(0);
		RTouch_Scan(0); 		 
	
		 	if(Xdown<lcd_width&&Ydown<lcd_height)
			{	
				if(Xdown>(lcd_width-40)&&Ydown>lcd_height-18)Clear_Screen();  //清空屏幕
				else Draw_Point(Xdown,Ydown,RED);		//画图	  			   
			}   
		i++;
		if(i%20==0)LED0=!LED0;
	}
}

int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();  //初始化延时函数
	uart1_init(115200);		//初始化串口波特率为115200
	
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化 
	KEY_Init(); 				//按键初始化  
	Touch_Init();				//触摸屏初始化
 	BRUSH_COLOR=RED;    //设置字体为红色 
	LCD_DisplayString(10,10,24,"Illuminati STM32F4");	
  LCD_DisplayString(20,40,16,"Author:Clever");
	LCD_DisplayString(30,80,24,"19.TOUCH TEST");

	delay_ms(1000);
	
 	Clear_Screen();	 	
  R_Touch_test(); 					  //电阻屏画板测试测试
}
