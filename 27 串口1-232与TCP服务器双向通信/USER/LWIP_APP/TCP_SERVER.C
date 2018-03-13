#include "stm32f4x7_eth.h"
#include "stm32f4x7_eth_bsp.h"
#include "netconf.h"
#include "tcp.h"
#include "udp.h"
#include "string.h"
#include "TCP_SERVER.h"
#include "usart1.h"

/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: TCP_SERVER.c                                                         *
* 文件简述：TCP服务器运用                                                        *
* 创建日期：2015.10.08                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                                                                     * 
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/


/****************************************************************************
* 名    称: tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
* 功    能：TCP数据接收和发送
* 入口参数： 
* 返回参数： 
* 说    明：这是一个回调函数，当一个TCP段到达这个连接时会被调用       
****************************************************************************/	
static err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	struct pbuf *p_temp = p;
	
	if(p_temp != NULL)
	{	
		tcp_recved(pcb, p_temp->tot_len);    //获取数据长度 tot_len：tcp数据块的长度
		while(p_temp != NULL)	
		{				
			uart1SendChars(p_temp->payload,p_temp->len); 	//接收到的数据从串口1-232发送出去   payload为TCP数据块的起始位置       
			p_temp = p_temp->next;
		}		
	}
	else
	{
		tcp_close(pcb); 	// 作为TCP服务器不应主动关闭这个连接
	}
	pbuf_free(p); 		 // 释放该TCP段  
	err = ERR_OK;
	return err;
}

/****************************************************************************
* 名    称: tcp_server_accept(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
* 功    能：回调函数
* 入口参数： 
* 返回参数： 
* 说    明：这是一个回调函数，当一个连接已经接受时会被调用
****************************************************************************/	
static err_t tcp_server_accept(void *arg,struct tcp_pcb *pcb,err_t err)
{
	tcp_setprio(pcb, TCP_PRIO_MIN); 		/* 设置回调函数优先级，当存在几个连接时特别重要,此函数必须调用*/
	tcp_recv(pcb,tcp_server_recv); 				/* 设置TCP段到时的回调函数 */
	err = ERR_OK;
	return err;
}

/****************************************************************************
* 名    称: TCP_server_init(void)
* 功    能：完成TCP服务器的初始化，主要是使得TCP通讯快进入监听状态
* 入口参数： 
* 返回参数： 
* 说    明：
****************************************************************************/	
void TCP_server_init(void)
{
	struct tcp_pcb *pcb;

	pcb = tcp_new(); 								              // 建立通信的TCP控制块(pcb) 
	tcp_bind(pcb,IP_ADDR_ANY,TCP_Server_PORT); 	  // 绑定本地IP地址和端口号（作为tcp服务器）  
	pcb = tcp_listen(pcb); 							          // 进入监听状态  
	tcp_accept(pcb,tcp_server_accept); 			      // 设置有连接请求时的回调函数  
}


