#include "led.h"
#include "lcd.h"
#include "key.h"
#include "adc.h"	 
/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 例程8 ADC模数转化main.c                                              *
* 文件简述：ADC使用                                                              *
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
 	u8 display_str[8];
	u16 adc_data;
	float temp;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();       //初始化延时函数
	LED_Init();					//初始化LED 
 	LCD_Init();         //初始化LCD接口
	ADC1_Init();        //初始化ADC
	BRUSH_COLOR=RED; 
	LCD_DisplayString(10,10,24,"Illuminati STM32F4");	
  LCD_DisplayString(10,40,16,"Author:Clever");
	LCD_DisplayString(30,80,24,"8.ADC TEST");

	BRUSH_COLOR=BLUE;//设置字体为蓝色
    
	while(1)
	{ 
		adc_data=Get_Adc(ADC_Channel_5);             //寄存器的电压值
		sprintf((char*)display_str,"ADC1_CH5_VAL:%06d",adc_data);
		LCD_DisplayString(30,130,16,display_str);	  //显示在液晶屏
		temp=(float)adc_data*(3.3/4096);            //实际电压数值
		sprintf((char*)display_str,"ADC1_CH5_VAL:%06e",temp); //浮点型数据  e-01  就是除于10      /10
		LCD_DisplayString(30,150,16,display_str);	  //实际电压数值

		LED0=!LED0;
		delay_ms(250);	
	}
}
