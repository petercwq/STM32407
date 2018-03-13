/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "tfcard_sdio.h"
#include "w25qxx.h"
#include "malloc.h"		

//////////////////////////////////////////////////////////////////////////////////	  


#define SD_CARD	 0  //SD卡,卷标为0
#define EX_FLASH 1	//外部flash,卷标为1

#define FLASH_SECTOR_SIZE 	512			  
	 			    
u16	    FLASH_SECTOR_COUNT=2048*10;	//W25Q1218,前10M字节给FATFS占用
#define FLASH_BLOCK_SIZE   	8     	//每个BLOCK有8个扇区

//初始化磁盘
DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	u8 res=0;	    
	switch(pdrv)
	{
		case SD_CARD://SD卡
			res = SD_Init();  
  			break;
		case EX_FLASH://外部FLASH
			W25QXX_Init();
			FLASH_SECTOR_COUNT=2048*10; //W25Q1218,前10M字节给FATFS用 
 			break;
		default:
			res=1; 
	}		 
	if(res)return  STA_NOINIT;
	else return 0; //初始化成功
}  

//获得磁盘状态
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{ 
	return 0;
} 
/****************************************************************************
* 名    称: DRESULT disk_read ()
* 功    能：读扇区
* 入口参数：drv:磁盘编号0~9
            buff:数据接收缓冲首地址
            sector:扇区地址
            count:需要读取的扇区数
* 返回参数：
* 说    明：移植文件系统要调用SD卡和W25Qxx的读扇区函数 	     
****************************************************************************/
DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	u8 res=0; 
    if (!count)return RES_PARERR; 
	switch(pdrv)
	{
		case SD_CARD:  //SD卡
			res=SD_ReadSDisk(buff,sector,count);	  
			break;
		case EX_FLASH: //外部FLASH
			for(;count>0;count--)
			{
				W25QXX_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
				sector++;
				buff+=FLASH_SECTOR_SIZE;
			}
			res=0;
			break;
		default:
			res=1; 
	}
   
    if(res==0x00)return RES_OK;	 
    else return RES_ERROR;	   
}
/****************************************************************************
* 名    称: DRESULT disk_write ()
* 功    能：写扇区
* 入口参数：drv:磁盘编号0~9
            buff:数据接收缓冲首地址
            sector:扇区地址
            count:需要写入的扇区数
* 返回参数：
* 说    明：移植文件系统要调用SD卡和W25Qxx的写扇区函数 	     
****************************************************************************/
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	u8 res=0;  
    if (!count)return RES_PARERR;   
	switch(pdrv)
	{
		case SD_CARD://SD卡
			res=SD_WriteSDisk((u8*)buff,sector,count);
			break;
		case EX_FLASH://外部FLASH
			for(;count>0;count--)
			{										    
				W25QXX_SectorWrite((u8*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
				sector++;
				buff+=FLASH_SECTOR_SIZE;
			}
			res=0;
			break;
		default:
			res=1; 
	}
    
    if(res == 0x00)return RES_OK;	 
    else return RES_ERROR;	
}
#endif

/****************************************************************************
* 名    称: DRESULT disk_ioctl ()
* 功    能：其他表参数的获得
* 入口参数：drv:磁盘编号0~9
            ctrl:控制代码
            buff:发送/接收缓冲区指针
* 返回参数：
* 说    明： 
****************************************************************************/
#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;						  			     
	if(pdrv==SD_CARD)//SD卡
	{
	    switch(cmd)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
				*(DWORD*)buff = 512; 
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
				*(WORD*)buff = SDCardInfo.CardBlockSize;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = SDCardInfo.CardCapacity/512;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
	}else if(pdrv==EX_FLASH)	//外部FLASH  
	{
	    switch(cmd)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = FLASH_SECTOR_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = FLASH_BLOCK_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = FLASH_SECTOR_COUNT;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
	}else res=RES_ERROR;
    return res;
}
#endif
//获得时间
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{				 
	return 0;
}			 
//动态分配内存
void *ff_memalloc (UINT size)			
{
	return (void*)Mem_malloc(INSRAM,size);
}
//释放内存
void ff_memfree (void* mf)		 
{
	Mem_free(INSRAM,mf);
}
