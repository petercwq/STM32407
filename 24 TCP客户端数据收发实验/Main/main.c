#include "led.h"
#include "lcd.h"
#include "key.h"

#include "stm32f4x7_eth.h"
#include "stm32f4x7_eth_bsp.h"
#include "netconf.h"
#include "tcp.h"
#include "udp.h"
#include "string.h"

#include "TCP_CLIENT.h"

///*********************************************************************************
//*******************MCU启明 STM32F407应用开发板(高配版)****************************
//**********************************************************************************
//* 文件名称: 例程24 网络实验                                                      *
//* 文件简述：DP83848_LWIP实验                                                     *
//* 创建日期：2015.010.05                                                          *
//* 版    本：V1.0                                                                 *
//* 作    者：Clever                                                               *
//* 说    明：完成TCP的客户端的数据收发                                            * 
//* 淘宝店铺：https://shop125046348.taobao.com                                     *
//* 声    明：本例程代码仅用于学习参考                                             *
//**********************************************************************************
//*********************************************************************************/

#define SYSTEMTICK_PERIOD_MS  10
	
unsigned char tcp_data[] = "TCP 客户端实验!\n";
struct tcp_pcb *pcb;	
	
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
uint32_t timingdelay;

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();         //初始化延时函数
	LED_Init();					  //初始化LED
	LCD_Init();					  //初始化LCD
	KEY_Init();           //初始化KEY
	
	BRUSH_COLOR=RED;
	LCD_DisplayString(10,10,24,"Illuminati STM32F4");	
  LCD_DisplayString(10,40,16,"Author:Clever");
	LCD_DisplayString(20,70,24,"24 TCP_Client ");
	
	BRUSH_COLOR=BLUE;
	LCD_DisplayString(20,110,16,"IMT407G_IP:192.168.1.240");  //开发板作为客户端时的IP
	LCD_DisplayString(20,130,16,"TCP_Server_PORT:2040 ");     //开发板作为客户端时的端口号
	
	LCD_DisplayString(20,170,16,"COM Server:192.168.1.68");   //电脑作为服务器时的IP
	LCD_DisplayString(20,190,16,"TCP_Server_PORT:2041 ");     //电脑作为服务器时的端口号
  
	ETH_BSP_Config();     //DP83848相关IO初始化与ETH相关初始化
	LwIP_Init();          //LWIP初始化

	TCP_Client_Init(TCP_LOCAL_PORT,TCP_Server_PORT,TCP_Server_IP); //TCP客户端初始化
	/* Infinite loop */
	while (1)
	{
		pcb = Check_TCP_Connect();
		if(pcb != 0)
		{	
			TCP_Client_Send_Data(pcb,tcp_data,sizeof(tcp_data));	//该函数为主动向服务器发送函数，
		}
		delay_ms(100);    

		/* handle periodic timers for LwIP */
		LwIP_Periodic_Handle(LocalTime);
	}   	
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: number of 10ms periods to wait for.
  * @retval None
  */
void Delay(uint32_t nCount)
{
  /* Capture the current local time */
  timingdelay = LocalTime + nCount;  

  /* wait until the desired delay finish */  
  while(timingdelay > LocalTime)
  {     
  }
}

/**
  * @brief  Updates the system local time
  * @param  None
  * @retval None
  */
void Time_Update(void)
{
  LocalTime += SYSTEMTICK_PERIOD_MS;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {}
}
#endif


