#include "usart1.h"
#include "string.h"
#include "stdlib.h"  
#include "led.h" 
#include "lcd.h"

/*********************************************************************************
*************************MCU启明 STM32F407核心开发板******************************
**********************************************************************************
* 文件名称: usart1.c                                                             *
* 文件简述：USART1使用                                                           *
* 创建日期：2015.03.06                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：利用串口调试助手经过USART1控制LED亮灭与蜂鸣器响闭                    * 
**********************************************************************************
*********************************************************************************/	

u8 receive_str[USART1_REC_NUM];     //接收缓存数组,最大USART_REC_LEN个字节 
u8 uart_byte_count=0;

/****************************************************************************
* 名    称: void uart1_init(u32 bound)
* 功    能：USART1初始化
* 入口参数：bound：波特率
* 返回参数：无
* 说    明： 
****************************************************************************/
void uart1_init(u32 bound)
{   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);  //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);          //初始化PA9，PA10
   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1	
  USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);         //开启相关中断
	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;      //串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		   //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	  //根据指定的参数初始化VIC寄存器、
}

//串口1发送一个字符
void uart1SendChar(u8 ch)
{      
	while((USART1->SR&0x40)==0);   //循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
}
/****************************************************************************
* 名    称: void uart1SendChars(u8 *str, u8 strlen)
* 功    能：串口1发送一字符串
* 入口参数：*str：发送的字符串
            strlen：字符串长度
* 返回参数：无
* 说    明： 
****************************************************************************/
void uart1SendChars(u8 *str, u8 strlen)
{ 
	  u8 k= 0 ; 
   do { uart1SendChar(*(str + k)); k++; }
    while (k < strlen); 
} 

//串口1中断服务程序
void USART1_IRQHandler(void)  
{
	u8 rec_data;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断 
		{
				rec_data =(u8)USART_ReceiveData(USART1);         //(USART1->DR) 读取接收到的数据
					 if(rec_data=='S')		  	                         //如果是S，表示是命令信息的起始位
						{
							uart_byte_count=0x01; 
						}

					else if(rec_data=='E')		                         //如果E，表示是命令信息传送的结束位
						{
				      LCD_Fill_onecolor(0,150,239,169,WHITE); 
							LCD_DisplayString(30,150,16,receive_str);							
//							for(uart_byte_count=0;uart_byte_count<32;uart_byte_count++)receive_str[uart_byte_count]=0x00;
// 				      uart_byte_count=0;  
						}	
  			  
		      else if((uart_byte_count>0)&&(uart_byte_count<32))
						{
							 receive_str[uart_byte_count-1]=rec_data;
							 uart_byte_count++;
						}                		 
   } 
} 

