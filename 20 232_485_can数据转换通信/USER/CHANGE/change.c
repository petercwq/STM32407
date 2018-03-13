#include "change.h"
#include "can1.h"
#include "can2.h"
#include "rs485.h"
#include "usart1.h"




void Change_232TO485(u8 numcnt)
{
  RS485_Send_Data(receive_str_232,numcnt);
}


void Change_485TO232(u8 numcnt)
{
 uart1SendChars(receive_str_485,numcnt);
}


void Change_232TOcan1(u8 numcnt)
{
 CAN1_Send_Msg(receive_str_232,numcnt);	
}	


void Change_can1TO232(u8 numcnt)
{
 uart1SendChars(receive_str_can1,numcnt);
}


void Change_485TOcan1(u8 numcnt)
{
 CAN1_Send_Msg(receive_str_485,numcnt);
}	


void Change_can1TO485(u8 numcnt)
{	
  RS485_Send_Data(receive_str_can1,numcnt);
}	

