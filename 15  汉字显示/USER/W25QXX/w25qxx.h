#ifndef __W25QXX_H
#define __W25QXX_H			    
#include "common.h"  

//////////////////////////////////////////////////////////////////////////////////	 

//W25Q系列芯片ID列表	   
#define W25Q08 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17

//指令表
#define W25X_CMD_WriteEnable		  0x06 
#define W25X_CMD_WriteDisable		  0x04 
#define W25X_CMD_ReadStatusReg		0x05 
#define W25X_CMD_WriteStatusReg		0x01 
#define W25X_CMD_ReadData			    0x03 
#define W25X_CMD_FastReadData		  0x0B 
#define W25X_CMD_FastReadDual		  0x3B 
#define W25X_CMD_PageProgram		  0x02 
#define W25X_CMD_BlockErase			  0xD8 
#define W25X_CMD_SectorErase		  0x20 
#define W25X_CMD_ChipErase			  0xC7 
#define W25X_CMD_PowerDown			  0xB9 
#define W25X_CMD_ReleasePowerDown	0xAB 
#define W25X_CMD_DeviceID			    0xAB 
#define W25X_CMD_ManufactDeviceID	0x90 
#define W25X_CMD_JedecDeviceID		0x9F 

extern u16 W25QXX_ID;			   //记录W25QXX芯片型号		   

#define	W25QXX_CS PGout(8)   //W25QXX的片选信号

void W25QXX_Init(void);
u16  W25QXX_ReadID(void);  	    		//读取FLASH ID
u8	 W25QXX_ReadSR(void);        		//读取状态寄存器 
void W25QXX_Write_SR(u8 sr);  			//写状态寄存器
void W25QXX_Write_Enable(void);  		//写使能 
void W25QXX_Write_Disable(void);		//写保护

void W25QXX_Erase_Chip(void);    	  	  //整片擦除
void W25QXX_Erase_Sector(u32 First_Addr);	//扇区擦除
void W25QXX_Wait_Busy(void);           	//等待空闲
void W25QXX_PowerDown(void);        	  //进入掉电模式
void W25QXX_WAKEUP(void);				        //唤醒

void W25QXX_Read(u8* DataBuffer,u32 StartAddress,u16 ByteCount);      //从指定地址读出指定字节数
void W25QXX_Write_Page(u8* DataBuffer,u32 StartAddress,u16 ByteCount);

void W25QXX_PageWrite(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);  //按页写入Flash
void W25QXX_SectorWrite(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//按扇区写入Flash

#endif
















