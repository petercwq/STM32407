#include "usart3.h"
#include "led.h"
#include "beep.h"
#include "lcd.h"
#include "key.h"

/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: USART3命令控制主函数main.c                                           *
* 文件简述：USART3命令控制使用                                                   *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：利用串口调试助手经过USART3控制LED亮灭与蜂鸣器响闭，按KEY0串口发送数  *
            据到串口调试助手                                                     * 
* 淘宝店铺：https://shop125046348.taobao.com                                     *
**********************************************************************************
*********************************************************************************/

int main(void)
{   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	delay_init();		      //延时初始化 
	uart3_init(9600);	    //串口初始化波特率为9600
	KEY_Init();
	LED_Init();		  		  //初始化与LED 
	BEEP_Init();          //蜂鸣器初始化
  LCD_Init();           //初始化LCD FSMC接口和显示驱动
	BRUSH_COLOR=RED;      //设置画笔颜色为红色
	
	LCD_DisplayString(10,10,24,"Illuminati STM32F4");	
  LCD_DisplayString(20,40,16,"Author:Clever");
	LCD_DisplayString(30,80,24,"5.USART3 TEST");
	LCD_DisplayString(50,110,16,"Please send control cmd");

	while(1)
	{
		key_scan(0);
		
		if(keyup_data==KEY0_DATA)
		  {
		   uart3SendChars("UART3 TEST",11);
		  }
	}
}

