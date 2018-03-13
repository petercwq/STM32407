#include "led.h"
#include "lcd_config.h"
#include "key.h"
#include "timer.h"
#include "gui.h"
#include "touch.h" 
#include "GUIDEMO.h"
/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 在F407上移植ucGUI实验                                                *
* 文件简述：ucGUI移植实验                                                        *
* 创建日期：2017.08.05                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：移植ucGUI演示其Demo                                                  * 
* 淘宝店铺：https://shop125046348.taobao.com                                     *
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/


int main(void)
{ 
	delay_init();         //初始化延时函数	
	LED_Init();					  //初始化LED
	KEY_Init();
	TIM3_Init(999,83); 	//1KHZ 定时器3设置为1ms
	TIM4_Init(999,839);  //触摸屏扫描速度,100HZ.
	Touch_Init();
	GUI_Init();
//	GUI_SetFont(&GUI_Font8x16);
//	GUI_SetColor(GUI_WHITE);   
//	
//	GUI_SetColor(GUI_WHITE);
//	GUI_SetBkColor(GUI_BLACK);
//	GUI_Clear();
//  GUI_SetFont(&GUI_Font24_ASCII);
//  GUI_DispStringHCenterAt("COLOR_ShowColorBar - Sample", 160, 4);
	while(1) 
	{		 
	  GUIDEMO_main();  //运行GUIDEMO	
	} 
}
