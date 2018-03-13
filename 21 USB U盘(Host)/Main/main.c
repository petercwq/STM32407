#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "sram.h"   
#include "malloc.h" 
#include "w25qxx.h"  
#include "usart1.h"
#include "tfcard_sdio.h"
#include "ff.h"    
#include "updatefont.h"
#include "showhz.h"	
#include "usbhost_user.h" 


/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 例程21 U盘OTG main.c                                                 *
* 文件简述：U盘OTG                                                               *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：读取U盘总容量                                                        * 
* 淘宝店铺：https://shop125046348.taobao.com                                     *
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/  


USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;


//用户测试主程序
//返回值:0,正常
//       1,有问题
u8 USH_User_App(void)
{ 
	u32 total,free;
	u8 res=0;
	LCD_DisplayHZstr(30,140,16,"设备连接成功!.");	 
	res=getUfree_volume("2:",&total,&free);
	if(res==0)
	{
		BRUSH_COLOR=BLUE;//设置字体为蓝色	   
		LCD_DisplayString(30,160,16,"FATFS OK!");	
		LCD_DisplayString(30,180,16,"U Disk Total Size:     MB");	 
		LCD_DisplayString(30,200,16,"U Disk  Free Size:     MB"); 	    
		LCD_DisplayNum(174,180,total>>10,5,16,0); //显示U盘总容量 MB
		LCD_DisplayNum(174,200,free>>10,5,16,0);	
	} 
 
	while(HCD_IsDeviceConnected(&USB_OTG_Core))//设备连接成功
	{	
		LED1=!LED1;
		delay_ms(200);
	}
	BRUSH_COLOR=RED;//设置字体为红色	   
	LCD_DisplayHZstr(30,140,16,"设备连接中...");
	LCD_Fill_onecolor(30,160,239,220,WHITE);
	return res;
} 


int main(void)
{        
	u8 t;
	FATFS *fsUP;  //U盘文件系统	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
 	delay_init();        //初始化延时函数
	uart1_init(9600);	 	 //初始化串口波特率为9600   
	LED_Init();				   //初始化与LED连接的硬件接口
	KEY_Init();				   //按键
  LCD_Init();				   //初始化LCD 
	W25QXX_Init();			 //SPI FLASH初始化
	Memory_Init(INSRAM); //初始化内部内存池	

	fsUP=(FATFS*)Mem_malloc(INSRAM,sizeof(FATFS));	

  f_mount(fsUP,"2:",1); 	//挂载U盘
	BRUSH_COLOR=RED;   

 	while(font_init()) 				//检查字库
	{	    
		LCD_DisplayString(60,50,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill_onecolor(60,50,240,66,WHITE);//清除显示	     
		delay_ms(200);				  
	}
  LCD_DisplayString(10,10,24,"Illuminati STM32F4");	
  LCD_DisplayString(10,40,16,"Author:Clever");			    	 
	LCD_DisplayHZstr(30,80,24,"21 USB U盘实验");					    	 
	LCD_DisplayHZstr(30,110,16,"2015年3月26日");	    	 
	LCD_DisplayHZstr(30,140,16,"设备连接中...");			 		
	//初始化USB主机
 	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks);  
	while(1)
	{
		USBH_Process(&USB_OTG_Core, &USB_Host);
		delay_ms(1);
		t++;
		if(t==200)
		{
			LED0=!LED0;
			t=0;
		}
	}	
}










