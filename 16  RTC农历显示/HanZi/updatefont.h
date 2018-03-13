#ifndef __UPDATEFONT_H__
#define __UPDATEFONT_H__ 
#include "common.h"

/*********************************************************************************
*************************MCU启明 STM32F407核心开发板******************************
**********************************************************************************
* 文件名称: updatefont.h                                                         *
* 文件简述：更新字库到W25Q128                                                    *
* 创建日期：2015.03.14                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                                                                     * 
**********************************************************************************
*********************************************************************************/ 

/* 字库在TF卡的路径必须如下：
UNIGBK码路径  "0:/Illuminati/HZFONT/UNIGBK.BIN";		//0:代表TF卡 
GBK12字库路径 "0:/Illuminati/HZFONT/GBK12.FON";		
GBK16字库路径 "0:/Illuminati/HZFONT/GBK16.FON";	
GBK24字库路径 "0:/Illuminati/HZFONT/GBK24.FON";		 
*/
extern u32 FONTSTARTADDR;	  //字库存放起始位置

#define FONTSTARTADDR  1024*1024*10 //字库存放位置从W25Q128的10M开始（3个字库总共3.09M）用户可根据情况自行更改
__packed typedef struct 
{
	u8  fontflag;			//字库存在标志，0X55，字库正常；其他，字库缺失 当字库缺失时字库初始化函数返回值不为“0”
	u32 ugbkaddr; 		//unigbk的地址
	u32 ugbksize;			//unigbk的大小	 
	u32 f12addr;			//gbk12地址	
	u32 gbk12size;		//gbk12的大小	 
	u32 f16addr;			//gbk16地址
	u32 gbk16size;		//gbk16的大小		 
	u32 f24addr;			//gbk24地址
	u32 gkb24size;		//gbk24的大小 
}HzfontTypedef; 

extern HzfontTypedef hzfont_info;	//字库信息结构体  字库初始化后给其赋值

//初始化字库 返回 0：字库完好  其他：字库缺失
u8 font_init(void);	

/****************************************************************************
* 名    称: void update_hzfont(u8* src);
* 功    能：更新字库到W25Q128
* 入口参数：无
* 返回参数：0:更新成功   其他:更新失败
* 说    明：		     
****************************************************************************/
void update_hzfont(void);			 

#endif





















