#include "usart3.h"
#include "string.h"
#include "stdlib.h"  
#include "led.h" 
#include "beep.h" 

/*********************************************************************************
***********************MCU启明 STM32F407开发板(高配版)****************************
**********************************************************************************
* 文件名称: usart3.c                                                             *
* 文件简述：USART3使用                                                           *
* 创建日期：2015.03.06                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：利用串口调试助手经过USART1控制LED亮灭与蜂鸣器响闭                    * 
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/	

u8 receive_str[USART3_REC_NUM];     //接收缓存数组,最大USART_REC_LEN个字节 
u8 uart_byte_count=0;

/****************************************************************************
* 名    称: void uart3_init(u32 bound)
* 功    能：USART1初始化
* 入口参数：bound：波特率
* 返回参数：无
* 说    明： 
****************************************************************************/
void uart3_init(u32 bound)
{   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //使能USART3时钟 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);  //GPIOB10复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11复用为USART3
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10与GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);          //初始化PB10，PB11
   //USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure); //初始化串口1	
  USART_Cmd(USART3, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART3, USART_FLAG_TC);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);         //开启相关中断
	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;      //串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		   //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	  //根据指定的参数初始化VIC寄存器、
}

//串口1发送一个字符
void uart3SendChar(u8 ch)
{      
	while((USART3->SR&0x40)==0);  
    USART3->DR = (u8) ch;      
}
/****************************************************************************
* 名    称: void uart1SendChars(u8 *str, u16 strlen)
* 功    能：串口1发送一字符串
* 入口参数：*str：发送的字符串
            strlen：字符串长度
* 返回参数：无
* 说    明： 
****************************************************************************/
void uart3SendChars(u8 *str, u16 strlen)
{ 
	  u16 k= 0 ; 
   do { uart3SendChar(*(str + k)); k++; }   //循环发送,直到发送完毕   
    while (k < strlen); 
} 

//串口1中断服务程序
void USART3_IRQHandler(void)  
{
	u8 rec_data;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断 
		{
				rec_data =(u8)USART_ReceiveData(USART3);         //(USART1->DR) 读取接收到的数据
        if(rec_data=='S')		  	                         //如果是S，表示是命令信息的起始位
				{
					uart_byte_count=0x01; 
				}

			else if(rec_data=='E')		                         //如果E，表示是命令信息传送的结束位
				{
					if(strcmp("Light_led1",(char *)receive_str)==0)        LED1=0;	//点亮LED1
					else if(strcmp("Close_led1",(char *)receive_str)==0)   LED1=1;	//关灭LED1
					else if(strcmp("Open_beep",(char *)receive_str)==0)    BEEP=1; 	//蜂鸣器响
					else if(strcmp("Close_beep",(char *)receive_str)==0)   BEEP=0; 	//蜂鸣器不响
					
					for(uart_byte_count=0;uart_byte_count<32;uart_byte_count++)receive_str[uart_byte_count]=0x00;
					uart_byte_count=0;    
				}				  
			else if((uart_byte_count>0)&&(uart_byte_count<=USART3_REC_NUM))
				{
				   receive_str[uart_byte_count-1]=rec_data;
				   uart_byte_count++;
				}                		 
   } 
} 

