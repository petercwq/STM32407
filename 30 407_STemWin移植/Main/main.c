#include "lcd_config.h"
#include "led.h"
#include "usart1.h"
#include "sram.h"
#include "malloc.h"
#include "touch.h"
#include "GUI.h"
#include "timer.h"
#include "GUIDemo.h"

/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 在F407上移植STemWin实验                                              *
* 文件简述：STemWin移植实验                                                      *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：移植STemWin演示其Demo                                                * 
* 淘宝店铺：https://shop125046348.taobao.com                                     *
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/

int main(void)
{	
	delay_init();       	//延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  	//中断分组配置
  uart1_init(115200);

	LED_Init(); 			      //LED初始化
  LCD_Config_Init();  		//LCD初始化
	Touch_Init();				      //触摸屏初始化
  FSMC_SRAM_Init(); 		  //SRAM初始化
	TIM3_Init(999,83); 	    //1KHZ 定时器3设置为1ms
	
	Memory_Init(INSRAM); 		//初始化内部内存池
	Memory_Init(EXSRAM);  	//初始化外部内存池
//	
	TIM4_Init(999,839);  //触摸屏扫描速度,100HZ.
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC,ENABLE);//开启CRC时钟
	WM_SetCreateFlags(WM_CF_MEMDEV);
	GUI_Init();
	GUI_DispStringAt("Hello World!", 0, 200);
	GUI_DispStringAt("Hello emWin!", 0, 216);
	
  GUIDEMO_Main();   //横屏与竖屏的切换 只需在 LCD_Config_Init() 中 修改该LCD_Display_Dir(1)的参数值就可以了  “0”为竖屏	 
	while(1)
	{
	}
}

