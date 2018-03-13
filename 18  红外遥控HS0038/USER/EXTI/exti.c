#include "exti.h"
#include "hs0038.h"
#include "lcd.h"

/*********************************************************************************
*************************MCU启明 STM32F407应用开发板******************************
**********************************************************************************
* 文件名称: exti.c                                                               *
* 文件简述：外部中断初始化                                                       *
* 创建日期：2015.03.18                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：中断服务函数处理红外接收管接收到的各电平的时间                       * 
**********************************************************************************
*********************************************************************************/	 


//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
	static u8  i=0;             //接收红外信号处理
	static u8 Start_flag;       //是否开始处理标志位
	
  TIM_Cmd(TIM2,ENABLE);   
	
	if(Start_flag)                         
	{
		if(Ir_Time >34)//引导码 TC9012的头码，9ms+4.5ms
		{		
			i = 0;	
		}
		
		Ir_TimeData[i] = Ir_Time;//存储每个电平的持续时间，用于以后判断是0还是1
		Ir_Time = 0;
		i++;
		
		if(i ==33)
		{
			 Recive_OK = 1;
			 TIM_Cmd(TIM2,DISABLE);    
			 i = 0;
			 IR_flag++;
		}
		if(IR_flag==2)     //开机第一次按红外按键不处理，第二开始处理按键值
			IR_OK = 1; 
	}
	else
	{
		TIM_Cmd(TIM2,DISABLE);    
		IR_OK = 0;
		Ir_Time = 0;
		Start_flag = 1;
	}
	
	EXTI_ClearITPendingBit(EXTI_Line2);  //清除LINE2上的中断标志位 
}

   
//外部中断初始化程序
void EXTIX_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 连接到中断线2
	
	/* 配置EXTI_Line2 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	   
}












