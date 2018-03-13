#include "24c02.h"  				 

/*********************************************************************************
*************************MCU启明 STM32F407核心开发板******************************
**********************************************************************************
* 文件名称: 24C02.c                                                              *
* 文件简述：24C02驱动程序                                                        *
* 创建日期：2017.08.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：IO口模拟IIC协议与24c02读写操作                                       * 
**********************************************************************************
*********************************************************************************/	

//IIC对应IO口的初始化
void IIC_Init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //使能GPIOB时钟
  //GPIOB8,B9初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);                 //初始化IO
	IIC_SCL=1;
	IIC_SDAOUT=1;
}
/*******************************************************************************
*************************以下为IO口模拟IIC通信**********************************
*******************************************************************************/
//IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //配置SDA线为输出
	IIC_SDAOUT=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDAOUT=0;
	delay_us(4);
	IIC_SCL=0;     //准备发送或接收数据 
}	  

//IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();    //配置SDA线为输出
	IIC_SCL=0;
	IIC_SDAOUT=0; 
 	delay_us(4);
	IIC_SCL=1;
  delay_us(4);				
	IIC_SDAOUT=1; //发送I2C总线结束信号				   	
}

/****************************************************************************
* 名    称: u8 MCU_Wait_Ack(void)
* 功    能：MCU等待从设备应答信号到来
* 入口参数：无
* 返回参数：1:接收应答失败  0:接收应答成功
* 说    明：  
****************************************************************************/
u8 MCU_Wait_Ack(void)
{
	u8 WaitTime=0;
	SDA_IN();      //配置SDA线为输入  
	IIC_SDAOUT=1;
	delay_us(1);	   
	IIC_SCL=1;
	delay_us(1);	 
	while(IIC_SDAIN)
	{
		WaitTime++;
		if(WaitTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0; 
	return 0;  
}

/****************************************************************************
* 名    称: u8 void MCU_Send_Ack(void)
* 功    能：MCU产生ACK应答,告知24cxx
* 入口参数：无
* 返回参数：
* 说    明：  
****************************************************************************/
void MCU_Send_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDAOUT=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

/****************************************************************************
* 名    称: u8 void MCU_Send_Ack(void)
* 功    能：MCU不产生ACK应答	
* 入口参数：无
* 返回参数：
* 说    明：  
****************************************************************************/  
void MCU_NOAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDAOUT=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}	

/****************************************************************************
* 名    称: void IIC_write_OneByte(u8 Senddata)
* 功    能：IIC写一个字节到EEPROM	
* 入口参数：Senddata:写入的8位数据
* 返回参数：
* 说    明：  
****************************************************************************/	  
void IIC_write_OneByte(u8 Senddata)
{                        
    u8 t;   
	  SDA_OUT(); 	    
    IIC_SCL=0;    //拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDAOUT=(Senddata&0x80)>>7;
        Senddata<<=1; 	  
		delay_us(2);   
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 

/****************************************************************************
* 名    称: void IIC_Read_OneByte(u8 Senddata)
* 功    能：IIC读取一个字节
* 入口参数：ack=1，发送ACK，ack=0，发送nACK 
* 返回参数：读到的8位数据
* 说    明：  
****************************************************************************/	  
u8 IIC_Read_OneByte(u8 ack)
{
	u8 i,receivedata=0;
	SDA_IN();       //配置SDA线为输入  
    for(i=0;i<8;i++ )
	  {
        IIC_SCL=0; 
        delay_us(2);
		    IIC_SCL=1;
        receivedata<<=1;
        if(IIC_SDAIN)receivedata++;   
		delay_us(1); 
    }					 
    if (!ack)
        MCU_NOAck();//发送nACK
    else
        MCU_Send_Ack(); //发送ACK   
    return receivedata;
}
/*******************************IO口模拟IIC*************************************
*******************************************************************************/


/*******************************************************************************
*************************以下为EEPROM24C02读写操作******************************
*******************************************************************************/
//初始化24c02的IIC接口
void AT24C02_Init(void)
{
	IIC_Init();  //IIC初始化
}

/****************************************************************************
* 名    称: u8 AT24C02_ReadByte(u8 ReadAddr)
* 功    能：在AT24C02指定地址读出一个数据
* 入口参数：ReadAddr：要读取数据所在的地址
* 返回参数：读到的8位数据
* 说    明：  
****************************************************************************/
u8 AT24C02_ReadByte(u8 ReadAddr)
{				  
	u8 receivedata=0;		  	    																 
  
	IIC_Start();  
	IIC_write_OneByte(0XA0);           //发送器件地址0XA0
	MCU_Wait_Ack();
  IIC_write_OneByte(ReadAddr);       //发送低地址
	MCU_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_write_OneByte(0XA1);           //进入接收模式			   
	MCU_Wait_Ack();	 
  receivedata=IIC_Read_OneByte(0);		   
  IIC_Stop();                    //产生一个停止条件	    
	
	return receivedata;
}

/****************************************************************************
* 名    称: u8 AT24C02_WriteByte(u8 WriteAddr,u8 WriteData)
* 功    能：在AT24C02指定地址写入一个数据
* 入口参数：WriteAddr：要写入数据所在的地址
            WriteData: 要写入的数据
* 返回参数： 
* 说    明：  
****************************************************************************/
void AT24C02_WriteByte(u8 WriteAddr,u8 WriteData)
{				   	  	    																 
  IIC_Start();  
	IIC_write_OneByte(0XA0);       //发送0XA0,写数据 	 
	MCU_Wait_Ack();	   
  IIC_write_OneByte(WriteAddr);  //发送低地址
	MCU_Wait_Ack(); 	 										  		   
	IIC_write_OneByte(WriteData);  //发送字节							   
	MCU_Wait_Ack();  		    	   
  IIC_Stop();                    //产生一个停止条件 
	delay_ms(10);	 
}

/****************************************************************************
* 名    称: u8 AT24C02_Test(void)
* 功    能：测试AT24C02是否正常
* 入口参数：无
* 返回参数：返回1:检测失败
            返回0:检测成功 
* 说    明：  
****************************************************************************/
u8 AT24C02_Test(void)
{
	u8 Testdata;
	Testdata=AT24C02_ReadByte(255); //如果开机测试，已有值无需再次写入	   
	if(Testdata==0XAB)return 0;		   
	else                             
	{
		AT24C02_WriteByte(255,0XAB);
	  Testdata=AT24C02_ReadByte(255);	  
		if(Testdata==0XAB)return 0;
	}
	return 1;											  
}

/****************************************************************************
* 名    称: u32 Buf_4Byte(u8 *pBuffer,u32 Date_4Byte,u8 Byte_num,u8 mode)
* 功    能：多位数与字节互转
* 入口参数：mode：1:多位数转分成字节   0:字节合并成一个多位数
            Byte_num：需要转化的字节数
            *pBuffer：字节接收数组或字节所在数组
            Date_4Byte：多位数数
* 返回参数：mode为0时，返回多位数
* 说    明：Byte_num最大为4个字节，该函数在后面的触摸屏校正时存取校正值所用到
****************************************************************************/
u32 Buf_4Byte(u8 *pBuffer,u32 Date_4Byte,u8 Byte_num,u8 mode)
{
   u8 i; u32 middata=0;
	if(mode)    //多位数转分成字节
	 {
	   for(i=0;i<Byte_num;i++)
	     {
	       *pBuffer++ =(Date_4Byte>>(8*i))&0xff;
	     }
			return 0; 
	 } 
	 else       //字节合并成一个多位数
	 {
	    Date_4Byte=0;
		  pBuffer+=(Byte_num-1);
		  for(i=0;i<Byte_num;i++)
	      { 		
		      middata<<=8;
		      middata+= *pBuffer--;			   
	      }
			return middata;	
	 }
}
/****************************************************************************
* 名    称: void AT24C02_Read(u8 ReadAddr,u8 *pBuffer,u8 ReadNum)
* 功    能：从AT24C02里面的指定地址开始读出指定个数的数据
* 入口参数：ReadAddr :开始读出的地址  0~255
            pBuffer  :数据数组首地址
            ReadNum:要读出数据的个数
* 返回参数：
* 说    明：  
****************************************************************************/
void AT24C02_Read(u8 ReadAddr,u8 *pBuffer,u8 ReadNum)
{
	while(ReadNum--)
	{
		*pBuffer++=AT24C02_ReadByte(ReadAddr++);	
	}
} 

/****************************************************************************
* 名    称: void AT24C02_Write(u8 WriteAddr,u8 *pBuffer,u8 WriteNum)
* 功    能：从AT24C02里面的指定地址开始写入指定个数的数据
* 入口参数：WriteAddr :开始写入的地址  0~255
            pBuffer  :数据数组首地址
            WriteNum:要写入数据的个数
* 返回参数：
* 说    明：  
****************************************************************************/
void AT24C02_Write(u8 WriteAddr,u8 *pBuffer,u8 WriteNum)
{
	while(WriteNum--)
	{
		AT24C02_WriteByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}








