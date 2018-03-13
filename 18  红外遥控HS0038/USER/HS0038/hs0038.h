#ifndef __HS0038_H
#define __HS0038_H

#include	"common.h"
	
//////////////////////////////////////////////////////////////////////////////
extern u8  Ir_Time,IR_flag;//红外用全局变量

extern u8 Decode_OK,Recive_OK,IR_OK;//定义接收状态、解码状态
extern u8 Ir_Record[4];    //处理后的红外码，分别是 客户码，客户码，数据码，数据码反码
extern u8 Ir_TimeData[33]; //33个高低电平的时间数据gfaaeg awgGww

void Ir_Decode(void);
#endif

