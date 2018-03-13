#ifndef  _TCP_CLIENT_H_
#define  _TCP_CLIENT_H_

#include "common.h"


/***************开发板ip及MAC定义*************************/
#define IMT407G_IP  			  192,168,1,240   //开发板ip 
#define IMT407G_NETMASK   	255,255,255,0   //开发板子网掩码
#define IMT407G_WG		   	  192,168,1,1   	//开发板子网关
#define IMT407G_MAC_ADDR    0,0,0,0,0,1			//开发板MAC地址

#define TCP_LOCAL_PORT     	2040 
#define TCP_Server_PORT    	2041
#define TCP_Server_IP   		192,168,1,68    //(电脑端)服务器IP

extern struct tcp_pcb *tcp_client_pcb;

void TCP_Client_Init(u16_t local_port,u16_t remote_port,unsigned char a,unsigned char b,unsigned char c,unsigned char d);
err_t TCP_Client_Send_Data(struct tcp_pcb *cpcb,unsigned char *buff,unsigned int length);
struct tcp_pcb *Check_TCP_Connect(void);

#endif

