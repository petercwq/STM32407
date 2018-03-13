#include "led.h"
#include "lcd.h"
#include "key.h"
#include "pwm.h"

/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 例程7 PWM输出主函数main.c                                            *
* 文件简述：PWM输出                                                              *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：按键控制输出不同占空比的PWM波,需示波器观察                           *
* 淘宝店铺：https://shop125046348.taobao.com                                     *
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/

int main(void)
{ 
  u32 pwmzkb;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();		      //初始化延时函数
	LED_Init();		        //初始化LED 
	KEY_Init();
	LCD_Init();
	BRUSH_COLOR=RED;      //设置画笔颜色为红色
	
	LCD_DisplayString(10,10,24,"Illuminati STM32F4");	
  LCD_DisplayString(10,40,16,"Author:Clever");
	LCD_DisplayString(30,80,24,"7.PWM TEST");	
	LCD_DisplayString(30,110,16,"KEY0:dutyfactor-- ");
	LCD_DisplayString(30,130,16,"KEY1:dutyfactor++ ");
	LCD_DisplayString(30,150,16,"Now dutyfactor is 50%");
	
	TIM5_PWM_Init(499,83);	//84M/84=1Mhz的计数频率,重装载值500，所以PWM频率为 1M/500=2Khz.  
  pwmzkb=250;
  TIM_SetCompare1(TIM5,pwmzkb);		
	while(1)
	{
		key_scan(0);
    if(keydown_data==KEY0_DATA) 
		  {
		   pwmzkb-=50;
				if(pwmzkb<50)pwmzkb=50;
			 TIM_SetCompare1(TIM5,pwmzkb);	 // 修改占空比        
			}
		if (keydown_data==KEY1_DATA) 
			{
				pwmzkb+=50;
				if(pwmzkb>450)pwmzkb=450;
				TIM_SetCompare1(TIM5,pwmzkb);  // 修改占空比  
			}  
    delay_ms(10); 			
	}
}


