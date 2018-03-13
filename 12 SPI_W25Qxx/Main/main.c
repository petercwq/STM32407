#include "led.h"
#include "lcd.h"
#include "w25qxx.h" 
#include "key.h"  
#include "usart1.h"

/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 例程11 25Q128通信主函数main.c                                        *
* 文件简述：25Q128使用                                                           *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：利用串口调试助手向单片机发送数据，并通过按KEY0写入25Q128，按KEY1读出 *
            写入的数据                                                           * 
* 淘宝店铺：https://shop125046348.taobao.com                                     *
**********************************************************************************
*********************************************************************************/

 
int main(void)
{ 
	u8 i=0;
	u8 *datatemp;	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();       //初始化延时函数
	uart1_init(9600);   //串口初始化
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化 
	KEY_Init(); 				//按键初始化  
	W25QXX_Init();			//W25QXX初始化 
	
 	BRUSH_COLOR=RED;   //设置字体为红色
	LCD_DisplayString(10,10,24,"Illuminati STM32F4");	
  LCD_DisplayString(10,40,16,"Author:Clever");
	LCD_DisplayString(30,80,24,"11.W25Q128 TEST");  
	
  LCD_DisplayString(30,110,16,"Please send data to usart1 ");   
  LCD_DisplayString(30,130,16,"Usart1 receive data is:"); 
  LCD_DisplayString(10,170,16,"KEY0:Write rec_data to 25Q128");
	LCD_DisplayString(10,190,16,"KEY1:Read before write data ");
 	BRUSH_COLOR=BLUE;  //设置字体为蓝色	 

	while(1)
	{
		key_scan(0);
		if(keydown_data==KEY0_DATA) //KEY0按下,将串口1接收到的数据写入25Q128
		{  
 			LCD_Fill_onecolor(0,210,239,249,WHITE);
			LCD_DisplayString(30,210,16,"Start write 25Q128....");
			W25QXX_SectorWrite((u8*)receive_str,0,uart_byte_count);
			LCD_DisplayString(30,210,16,"25Q128 write finished!");//写入成功
		}
		if(keydown_data==KEY1_DATA) //KEY1按下,将之前写入到25Q128中的数据读出并显示
		{
 			LCD_DisplayString(30,210,16,"Start read 25Q128.... ");
			W25QXX_Read(datatemp,0,uart_byte_count);
			LCD_DisplayString(30,210,16,"The readed data is:  "); 
			LCD_DisplayString(30,230,16,datatemp);          //显示读到的数据
		}	 
			i++;
			delay_ms(10);
			if(i==50||i==100)
			{
				if(i==50)
				 {
					LED0=0;
					LCD_DisplayString(30,110,16,"                           ");
				 }
				 if(i==100)
				 {
					LED0=1;
					LCD_DisplayString(30,110,16,"Please send data to usart1 ");   
					i=0;
				 }
				
			}		   
	} 	    
}
