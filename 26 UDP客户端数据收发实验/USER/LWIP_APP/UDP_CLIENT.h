#ifndef _UDP_CLIENT_H_
#define _UDP_CLIENT_H_


/***************开发板ip及MAC定义*************************/
#define IMT407G_IP  			 	192,168,1,240   	//开发板ip 
#define IMT407G_NETMASK   	255,255,255,0     //开发板子网掩码
#define IMT407G_WG		   	 	192,168,1,1  			//开发板子网关
#define IMT407G_MAC_ADDR    0,0,0,0,0,1				//开发板MAC地址

#define UDP_Client_PORT			2040
#define UDP_REMOTE_PORT			2041
#define UDP_REMOTE_IP		    192,168,1,68


extern struct udp_pcb *udp_pcb;
extern struct ip_addr ipaddr;
extern struct pbuf *udp_p;

void UDP_client_init(void);
void UDP_Send_Data(struct udp_pcb *pcb,struct pbuf *p);
void Set_IP4_ADDR(struct ip_addr *ipaddr,unsigned char a,unsigned char b,unsigned char c,unsigned char d);

#endif

