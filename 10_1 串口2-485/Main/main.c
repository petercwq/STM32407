#include "led.h"
#include "beep.h" 
#include "lcd.h"
#include "key.h"
#include "rs485.h"

/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 例程10 USART2——RS485命令控制主函数main.c                             *
* 文件简述：USART2命令控制使用                                                   *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：利用串口调试助手经过USART2控制LED亮灭与蜂鸣器响闭，按KEY0串口发送数  *
            据到串口调试助手                                                     * 
* 淘宝店铺：https://shop125046348.taobao.com                                     *
**********************************************************************************
*********************************************************************************/

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();       //初始化延时函数
	
	LED_Init();					//初始化LED 
	BEEP_Init();        //初始化蜂鸣器
 	LCD_Init();					//LCD初始化 
	KEY_Init(); 				//按键初始化  
	RS485_Init(9600);		//初始化RS485串口2
 	BRUSH_COLOR=RED;    //设置字体为红色 
	LCD_DisplayString(10,10,24,"Illuminati STM32F4");	
  LCD_DisplayString(20,40,16,"Author:Clever");
	LCD_DisplayString(30,80,24,"10.USART2 TEST");	
	LCD_DisplayString(30,130,16,"KEY0:Send");    	//显示提示信息		
 									  
	while(1)
	{
		key_scan(0);
		
		if(keyup_data==KEY0_DATA)
		  {
		   RS485_Send_Data("UART2 TEST",11);
		  }
	}  
}

