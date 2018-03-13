#ifndef __SDIO_SDCARD_H
#define __SDIO_SDCARD_H		

#include "common.h" 

//////////////////////////////////////////////////////////////////////////////////	 

//用户配置区			  
//SDIO时钟计算公式:SDIO_CK时钟=SDIOCLK/[clkdiv+2];其中,SDIOCLK固定为48Mhz
#define SDIO_INIT_CLK        0x76 	//SDIO初始化频率，最大400Kh  
#define SDIO_TRANSFER_CLK    0x00		//SDIO传输频率,该值太小可能会导致读写文件出错 

//SDIO工作模式定义
#define SD_WORK_MODE  0     // 0:查询模式   1:DMA模式
		  
//SD卡信息
typedef struct
{
  SD_CSD SD_csd;
  SD_CID SD_cid;
  long long CardCapacity;  	//SD卡容量 单位为“字节”   所以SDCardInfo.CardCapacity>>20右移20位得到卡的容量的单位为 M  
  u32 CardBlockSize; 		    //SD卡块大小	
  u16 Rlative_ADD;					//卡相对地址
  u8 CardType;				      //卡类型
} SD_CardInfo;	

extern SD_CardInfo SDCardInfo;//SD卡信息	sd卡初始化之后给其赋值		 

//相关函数声明

//SD卡初始化
SD_Error SD_Init(void);

/****************************************************************************
* 名    称: u8 SD_ReadSDisk(u8*buf,u32 sector,u8 cnt)
* 功    能：读SD卡
* 入口参数：buf:读数据缓存区
            sector:扇区地址
            cnt:扇区个数
* 返回参数：0,正常;其他,出错 
* 说    明：       
****************************************************************************/	
u8 SD_ReadSDisk(u8*buf,u32 sector,u8 cnt); 	//读SD卡,fatfs调用

/****************************************************************************
* 名    称: u8 SD_WriteSDisk(u8*buf,u32 sector,u8 cnt)
* 功    能：写SD卡
* 入口参数：buf:写数据缓存区
            sector:扇区地址
            cnt:扇区个数
* 返回参数：0,正常;其他,出错 
* 说    明：       
****************************************************************************/	
u8 SD_WriteSDisk(u8*buf,u32 sector,u8 cnt);	//写SD卡,fatfs调用

#endif 




