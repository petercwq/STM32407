#include "led.h"
#include "timer.h"
#include "lcd.h"
#include "beep.h"
#include "exti.h"
#include "hs0038.h"

/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 例程18 HS0038使用主函数main.c                                        *
* 文件简述：HS0038使用                                                           *
* 创建日期：2015.03.06                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：利用定时器中断使用控制LED1亮灭与蜂鸣器响闭                           * 
* 淘宝店铺：https://shop125046348.taobao.com                                     *
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();          //初始化延时函数
	LED_Init();				     //初始化LED端口
	BEEP_Init();
	LCD_Init();
  BRUSH_COLOR=RED;      //设置画笔颜色为红色

 	TIM2_Init(255,83);	//定时器时钟84M，分频系数84，计数频率1M所以计数255次为255us 
	
	EXTIX_Init();

  LCD_DisplayString(10,10,24,"Illuminati STM32F4");	
  LCD_DisplayString(10,40,16,"Author:Clever");
	LCD_DisplayString(30,80,24,"6.TIMER TEST");	
	
	LCD_DisplayString(40,120,24,"IR_DATA:0");	
	
	while(1)
	{
		while(IR_OK)  //上电初始化后 第一次随意按键值后 IR_OK就一直为“1”，就是说上电后第一次按键无效 
		{		    	                                 
			if(Recive_OK)             //如果接收好了进行红外处理
			  {   
			   Ir_Decode();
		 	   Recive_OK = 0;
			  }
		    if(Decode_OK)          //如果处理好后进行工作处理，如按对应的按键后显示对应的数字等
			  {
					LCD_DisplayNum(136,120,Ir_Record[2],2,24,0);   //十进制显示客户码
		  	}
		}
  }
}


