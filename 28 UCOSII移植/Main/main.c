#include "led.h"
#include "includes.h"
#include "task.h"

/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 例程28 UCOSII移植                                                    *
* 文件简述：UCOSII实验                                                           *
* 创建日期：2017.09.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                                                                     * 
* 淘宝店铺：https://shop125046348.taobao.com                                     *
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/



int main(void)
{ 
  OS_CPU_SR cpu_sr=0;
	
	delay_init();		  //初始化延时函数 利用滴答定时器SysTick中断模式延时，也提供了系统节拍
	LED_Init();		    //初始化LED端口 
	
	OSInit();        //初始化UCOS系统
	
	/*建立任务*/
	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)    
 	OSTaskCreate(LED0_Task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
 	OSTaskCreate(LED1_Task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);	 				   
	OS_EXIT_CRITICAL();				 //退出临界区(可以被中断打断)
	
	OSStart();	     //系统开始运行
}









