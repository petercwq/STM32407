#include "led.h"
#include "lcd.h"
#include "key.h"
#include "adc.h"	
#include "dac.h"
/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 例程9 DAC实验的main.c                                                *
* 文件简述：DAC使用                                                              *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：由DAC输出模拟电压，ADC采样后在LCD上显示电压，按KEY0增加输出电压，    *
            按KEY1减少输出电压                                                   * 
* 淘宝店铺：https://shop125046348.taobao.com                                     *	
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/

int main(void)
{ 
	u16 adc_data;
	u16 dac_data;
	float temp;
 	u8 display_str[8];	 
	u16 dacval=0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();      //初始化延时函数
	
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化
	ADC1_Init(); 				//adc初始化
	KEY_Init(); 				//按键初始化
	DAC1_Init();		 		//DAC通道1初始化	
	BRUSH_COLOR=RED; 
  LCD_DisplayString(10,10,24,"Illuminati STM32F4");	
  LCD_DisplayString(10,40,16,"Author:Clever");
	LCD_DisplayString(30,80,24,"9.DAC TEST");
	LCD_DisplayString(30,110,16,"KEY0: V++  KEY1:V--");

	BRUSH_COLOR=BLUE;//设置字体为蓝色      	      
 	
  DAC_SetChannel1Data(DAC_Align_12b_R,dacval);//初始值为0	
	while(1)
	{
		key_scan(0);			  
		if(keydown_data==KEY0_DATA)
		{		 
			if(dacval<4000)dacval+=200;
			DAC_SetChannel1Data(DAC_Align_12b_R, dacval); //设置DAC值
		}
		else if(keydown_data==KEY1_DATA)	
		{
			if(dacval>200)dacval-=200;
			else dacval=0;
			DAC_SetChannel1Data(DAC_Align_12b_R, dacval); //设置DAC值
		}	 
		
		if(keydown_data==KEY1_DATA||keydown_data==KEY0_DATA) 	//KEY1按下了
		{	  
 			dac_data=DAC_GetDataOutputValue(DAC_Channel_1);  //读取前面设置DAC的值
			sprintf((char*)display_str,"DAC VAL::%06d",dac_data);
			LCD_DisplayString(30,150,16,display_str);	  //显示在液晶屏
			temp=(float)dac_data*(3.3/4096);			           //得到DAC电压值
		  sprintf((char*)display_str,"DAC VOL:%06e",temp); //浮点型数据  e-01  就是除于10      /10
		  LCD_DisplayString(30,170,16,display_str);	  //实际电压数值
			
 			adc_data=Get_Adc(ADC_Channel_6);		   //得到ADC转换值	  
			temp=(float)adc_data*(3.3/4096);			           //得到ADC电压值			
			sprintf((char*)display_str,"ADC VOL:%06e",temp); //浮点型数据  e-01  就是除于10      /10
		  LCD_DisplayString(30,190,16,display_str);	  //实际电压数值
		}	    
		delay_ms(10);	 
	}	
}
