#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_



/***************开发板ip及MAC定义*************************/
#define IMT407G_IP  			 	192,168,1,240   		//开发板ip 
#define IMT407G_NETMASK   	255,255,255,0   		//开发板子网掩码
#define IMT407G_WG		   	 	192,168,1,1   			//开发板子网关
#define IMT407G_MAC_ADDR    0,0,0,0,0,1				  //开发板MAC地址

#define UDP_LOCAL_PORT     	2040

void UDP_server_init(void);


#endif

