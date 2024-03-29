#include "led.h"
#include "beep.h"

/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 例程2 蜂鸣器使用主函数main()                                         *
* 文件简述：蜂鸣器实验                                                           *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                                                                     * 
* 淘宝店铺：https://shop125046348.taobao.com                                     *
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/

int main(void)
{ 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置系统中断优先级分组2
	delay_init();		   //初始化延时函数
	LED_Init();				 //初始化LED端口
	BEEP_Init();       //初始化蜂鸣器端口

	while(1)
	{ 
    PGout(7)=0;
//		BEEP=0;
		LED0=0;
		delay_ms(300);
		
		PGout(7)=1;
//		BEEP=1;
		LED0=1;
		delay_ms(300);
	}
}
