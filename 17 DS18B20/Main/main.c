#include "led.h"
#include "lcd.h"
#include "ds18b20.h"

/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 例程17 DS18B20显示温度                                               *
* 文件简述：DS18B20使用                                                          *
* 创建日期：2015.03.15                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                                                                     *
* 淘宝店铺：https://shop125046348.taobao.com                                     *
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/
  
int main(void)
{ 		    
	short temperature;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();       //初始化延时函数
	LED_Init();					//初始化LED
 	LCD_Init();
  BRUSH_COLOR=RED;    //设置字体为红色 
  LCD_DisplayString(10,10,24,"Illuminati STM32F4");	
  LCD_DisplayString(10,40,16,"Author:Clever");
	LCD_DisplayString(30,80,24,"17.DS18B20 TEST");
	
 	while(DS18B20_Init())	//DS18B20初始化	
	{
		LCD_DisplayString(30,130,16,"DS18B20 Error");
		delay_ms(200);
		LCD_Fill_onecolor(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}   
	LCD_DisplayString(30,130,16,"DS18B20 OK");
	temperature=DS18B20_Get_Temp();  //先读取一次温度值
	
	BRUSH_COLOR=BLUE; //设置字体为蓝色 
 	LCD_DisplayString(30,150,16,"Temp:   . C");	 
	while(1)
	{	    	    

			delay_ms(150); //每150ms读取一次	
		
			temperature=DS18B20_Get_Temp();	
			if(temperature<0)
			{
				LCD_DisplayChar(30+40,150,'-',16);			//显示负号
				temperature=-temperature;					      //转为正数
			}
			else 
			  LCD_DisplayChar(30+40,150,' ',16);			//去掉负号
			
			LCD_DisplayNum(30+40+8,150,temperature/10,2,16,0);	//显示正数部分	    
   		LCD_DisplayNum(30+40+32,150,temperature%10,1,16,0);	//显示小数部分 		   

	}
}

