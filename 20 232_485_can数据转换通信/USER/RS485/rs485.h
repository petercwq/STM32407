#ifndef __RS485_H
#define __RS485_H			 
#include "common.h"	 								  

	  	
extern u8 receive_str_485[128]; 	//接收缓冲,最大512个字节
extern u8 send_str_485[128]; 	//接收缓冲,最大512个字节
extern u8 byte_count_485;   			  //接收到的数据长度

//模式控制
#define RS485_TX_EN		PGout(6)	    //485模式控制.0,接收;1,发送.

														 
void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
 
#endif	   
















