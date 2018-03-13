#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "malloc.h" 
#include "tfcard_sdio.h"    
#include "malloc.h" 
#include "w25qxx.h"    
#include "ff.h"     
#include "updatefont.h"  //调用HanZiUse.lib中更新字库函数 鼠标右键选择打开头文件可查看该头文件
#include "showhz.h"	     //调用HanZiUse.lib中显示汉字函数

/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: 例程15 汉字显示                                                      *
* 文件简述：汉字在LCD上显示                                                      *
* 创建日期：2015.03.14                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：该例程使用了三个封装库函数，请读者先行找到各个库函数对应的.h文件进行 *
            仔细，了解如何使用该库函数。使用封装库函数的用意：无须关心底层，只需 *
						了解接口函数，应用就是如此简单。不过在移植的时候要注意封装库调用到的 *
						外部函数，就是说在移植时也要把封装库所调用到函数也加到工程上来，详细 *
						查看.h说明                                                           * 
* 淘宝店铺：https://shop125046348.taobao.com                                     *
**********************************************************************************
*********************************************************************************/

 
int main(void)
{        
	u8 i;
	FATFS *fsTF;  //TF卡文件系统		
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();         //初始化延时函数
	LED_Init();					  //初始化LED  
 	LCD_Init();					  //LCD初始化  
 	KEY_Init();					  //按键初始化  
	W25QXX_Init();			  //初始化W25Q128
	
/******************HanZiUse.lib调用 更新字库要用到的**********************/
	Memory_Init(INSRAM);	//初始化内部内存池  

 	fsTF=(FATFS*)Mem_malloc(INSRAM,sizeof(FATFS));	//为文件系统分配内存 
  f_mount(fsTF,"0:",1); 		//挂载TF卡 
/***********************************************************************/	
	
	while(font_init()) 	 //开机检查字库，字库缺失更新字库
	{ 
		update_hzfont();   //更新字库	      
	}  

	BRUSH_COLOR=RED;       
  LCD_DisplayString(10,10,24,"Illuminati STM32F4");	
  LCD_DisplayString(10,40,16,"Author:Clever");
	LCD_DisplayHZstr(30,80,24,"15.汉字显示");		
  LCD_DisplayHZstr(30,110,16,"KEY0:更新字库");	
  LCD_DisplayHZstr(30,130,16,"KEY1:测试汉字");	
	 
	while(1)
	{
     key_scan(0);
		 
		 if(keydown_data==KEY0_DATA)  //按KEY0将TF卡中的字库更新到W25Q128
		 {
		   update_hzfont(); 
			 LCD_DisplayString(10,10,24,"Illuminati STM32F4");	
       LCD_DisplayString(10,40,16,"Author:Clever");
	     LCD_DisplayHZstr(30,80,24,"15.汉字显示");
			 LCD_DisplayHZstr(30,110,16,"KEY0:更新字库");	
       LCD_DisplayHZstr(30,130,16,"KEY1:测试汉字");	    			 
		 }
		 if(keydown_data==KEY1_DATA)
		 {		 
		  switch(i)
			   {
				  case 0: 
						{
							LCD_DisplayHZstr(30,160,16,"测试汉字第一句话");
					    LCD_DisplayHZstr(30,180,16,"MCU 启明STM32F4开发板");
							break;
					  }
						case 1: 
						{
							LCD_DisplayHZstr(30,160,16,"测试汉字第二句话");
					    LCD_DisplayHZstr(30,180,16,"MCU启明当你的启蒙老师");
							break;
					  }
						case 2: 
						{
							LCD_DisplayHZstr(30,160,16,"测试汉字第三句话");
					    LCD_DisplayHZstr(30,180,16,"带你开起学习F4的大门 ");
							break;
					  }
			   }
				i++;
			  if(i>2) i=0;
		 }
		delay_ms(5);
	} 
}







