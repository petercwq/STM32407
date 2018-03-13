/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: UDP_CLIENT.C                                                        *
* 文件简述：UDP客户端运用                                                        *
* 创建日期：2015.10.08                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                                                                     * 
**********************************************************************************
*********************************************************************************/
#include "stm32f4x7_eth.h"
#include "stm32f4x7_eth_bsp.h"
#include "netconf.h"
#include "tcp.h"
#include "udp.h"
#include "string.h"
#include "UDP_CLIENT.h"
#include "common.h"

const static unsigned char UDPData[]="UDP客户端实验例程!\r\n";

struct udp_pcb *udp_pcb;
struct ip_addr ipaddr;
struct pbuf *udp_p;

/****************************************************************************
* 名    称: void UDP_client_init(void)
* 功    能：完成udp客户端初始化
* 入口参数： 
* 返回参数： 
* 说    明：       
****************************************************************************/
void UDP_client_init(void)
{
	udp_p = pbuf_alloc(PBUF_RAW,sizeof(UDPData),PBUF_RAM);
	udp_p -> payload = (void *)UDPData;
	Set_IP4_ADDR(&ipaddr,UDP_REMOTE_IP);				    //远端IP
	udp_pcb = udp_new();
	udp_bind(udp_pcb,IP_ADDR_ANY,UDP_Client_PORT); 	// 绑定本地IP地址 
	udp_connect(udp_pcb,&ipaddr,UDP_REMOTE_PORT); 	// 连接远程主机 	
}

/****************************************************************************
* 名    称: void UDP_Send_Data(struct udp_pcb *pcb,struct pbuf *p)
* 功    能：udp客户端发送数据
* 入口参数： 
* 返回参数： 
* 说    明：       
****************************************************************************/
void UDP_Send_Data(struct udp_pcb *pcb,struct pbuf *p)
{
	udp_send(pcb,p);
	delay_ms(100);   //延时，不能发送太快	
}


