#include "spi.h"

/*********************************************************************************
*************************MCU启明 STM32F407核心开发板******************************
**********************************************************************************
* 文件名称: spi.c                                                                *
* 文件简述：spi1初始化                                                           *
* 创建日期：2015.03.10                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：spi初始化，与读写函数实现                                            * 
**********************************************************************************
*********************************************************************************/	 


/****************************************************************************
* 名    称: void SPI1_Init(void)
* 功    能：spi1硬件初始化
* 入口参数：无
* 返回参数：无
* 说    明：spi1初始化并且将其配置成主机模式 		     
****************************************************************************/
void SPI1_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);           //使能GPIOB时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);            //使能SPI1时钟
 
  //GPIOFB3,4,5IO口初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5; //PB3~5复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                    //复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                  //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;              //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                    //上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);                          //初始化IO口
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);           //PB3复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);           //PB4复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);           //PB5复用为 SPI1
 
	//SPI口初始化
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);             //复位SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);            //停止复位SPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                    //设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		                //设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		                      //串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                      //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                        //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                          //CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);                                 //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE);    //使能SPI外设

	SPI1_ReadWriteByte(0xff); //启动传输		 
} 
/****************************************************************************
* 名    称: void SPI1_Setclock(u8 SPI_Prescaler)
* 功    能：SPI1时钟速度设置函数
* 入口参数：SPI_Prescaler：分频系数
* 返回参数：无
* 说    明：SPI_BaudRate_Prescaler取值范围:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256 	     
****************************************************************************/
void SPI1_Setclock(u8 SPI_Prescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_Prescaler));
	SPI1->CR1&=0XFFC7;                           //先将bit3-5清零 
	SPI1->CR1|=SPI_Prescaler;	                   //设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE);                        //使能SPI1
} 
/****************************************************************************
* 名    称: u8 SPI1_ReadWriteByte(u8 writeData)
* 功    能：spi1读写函数
* 入口参数：writeData:要写入的字节
* 返回参数：读取到的字节
* 说    明： 		     
****************************************************************************/
u8 SPI1_ReadWriteByte(u8 writeData)
{		 			 
   u8 waitnum=0;
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){} //等待发送区空  
	      {  
				    waitnum++;  
				    if(waitnum>200)return 0;  
				}
	SPI_I2S_SendData(SPI1, writeData);  //通过外设SPI1发送一个字节
		
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //等待一个字节接收完 
        {  
				    waitnum++;  
				    if(waitnum>200)return 0;  
				}
	return SPI_I2S_ReceiveData(SPI1);  //返回SPI1接收的数据	
 		    
}




