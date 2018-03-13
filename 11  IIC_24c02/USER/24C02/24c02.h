#ifndef __24C02_H
#define __24C02_H
#include "common.h"  

////////////////////////////////////////////////////////////////////////////////// 	

//IIC_SDA线IO方向配置
#define SDA_IN()  {GPIOB->MODER&=~(3<<18);GPIOB->MODER|=0<<18;}	//PB9输入模式
#define SDA_OUT() {GPIOB->MODER&=~(3<<18);GPIOB->MODER|=1<<18;} //PB9输出模式
//IO操作函数	 
#define IIC_SCL      PBout(8) //SCL
#define IIC_SDAOUT   PBout(9) //输出SDA	 
#define IIC_SDAIN    PBin(9)  //输入SDA 

//IIC相关函数
void IIC_Init(void);          //初始化IIC的IO口				 
void IIC_Start(void);				  //发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
u8 MCU_Wait_Ack(void); 				//IIC等待ACK信号
void MCU_Send_Ack(void);					  //IIC发送ACK信号
void MCU_NOAck(void);				  //IIC不发送ACK信号
void IIC_write_OneByte(u8 Senddata);
u8 IIC_Read_OneByte(u8 ack);
	
//EEPROM24c02相关函数
u8 AT24C02_ReadByte(u8 ReadAddr);							     //指定地址读取一个字节
void AT24C02_WriteByte(u8 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节

u32 Buf_4Byte(u8 *pBuffer,u32 Date_4Byte,u8 Byte_num,u8 mode);

void AT24C02_Write(u8 WriteAddr,u8 *pBuffer,u8 WriteNum);	//从指定地址开始写入指定长度的数据
void AT24C02_Read(u8 ReadAddr,u8 *pBuffer,u8 ReadNum);   	//从指定地址开始读出指定长度的数据

u8 AT24C02_Test(void);  //检查器件
void AT24C02_Init(void); //初始化IIC

#endif
















