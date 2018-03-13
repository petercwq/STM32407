#include "can1.h"
#include "led.h"


/*********************************************************************************
**********************MCUÆôÃ÷ STM32F407Ó¦ÓÃ¿ª·¢°å(¸ßÅä°æ)*************************
**********************************************************************************
* ÎÄ¼şÃû³Æ: can1.c                                                               *
* ÎÄ¼ş¼òÊö£ºcanÅäÖÃÎÄ¼ş                                                          *
* ´´½¨ÈÕÆÚ£º2017.08.30                                                           *
* °æ    ±¾£ºV1.0                                                                 *
* ×÷    Õß£ºClever                                                               *
* Ëµ    Ã÷£º                                                                     * 
**********************************************************************************£B
*********************************************************************************/

u8 receive_str_can1[128];   //½ÓÊÕ»º³å,×î´ó128¸ö×Ö½Ú.
u8 send_str_can1[128];

/****************************************************************************
* Ãû    ³Æ: u8 CAN1_Mode_Init(u8 mode)
* ¹¦    ÄÜ£ºCAN³õÊ¼»¯
* Èë¿Ú²ÎÊı£ºmode:CAN¹¤×÷Ä£Ê½;0,ÆÕÍ¨Ä£Ê½;1,»·»ØÄ£Ê½
* ·µ»Ø²ÎÊı£º0,³É¹¦;
           	ÆäËû,Ê§°Ü;
* Ëµ    Ã÷£º       
****************************************************************************/	
u8 CAN1_Mode_Init(u8 mode)
{
  	GPIO_InitTypeDef GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN1_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif
    //Ê¹ÄÜÏà¹ØÊ±ÖÓ
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Ê¹ÄÜPORTAÊ±ÖÓ	                   											 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//Ê¹ÄÜCAN1Ê±ÖÓ	
	
    //³õÊ¼»¯GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//¸´ÓÃ¹¦ÄÜ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
    GPIO_Init(GPIOA, &GPIO_InitStructure);//³õÊ¼»¯PA11,PA12
	
	  //Òı½Å¸´ÓÃÓ³ÉäÅäÖÃ
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11¸´ÓÃÎªCAN1
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12¸´ÓÃÎªCAN1
	  
  	//CANµ¥ÔªÉèÖÃ
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//·ÇÊ±¼ä´¥·¢Í¨ĞÅÄ£Ê½   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//Èí¼ş×Ô¶¯ÀëÏß¹ÜÀí	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//Ë¯ÃßÄ£Ê½Í¨¹ıÈí¼ş»½ĞÑ(Çå³ıCAN->MCRµÄSLEEPÎ»)
  	CAN_InitStructure.CAN_NART=ENABLE;	//½ûÖ¹±¨ÎÄ×Ô¶¯´«ËÍ 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//±¨ÎÄ²»Ëø¶¨,ĞÂµÄ¸²¸Ç¾ÉµÄ  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//ÓÅÏÈ¼¶ÓÉ±¨ÎÄ±êÊ¶·û¾ö¶
	 
/***************************************************************************************/		
    //ÅäÖÃcan¹¤×÷Ä£Ê½	
  	CAN_InitStructure.CAN_Mode= mode;	 //Ä£Ê½ÉèÖÃ 
  	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;	//ÖØĞÂÍ¬²½ÌøÔ¾¿í¶È(Tsjw)Îªtsjw+1¸öÊ±¼äµ¥Î» CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=CAN_BS1_7tq; //Ê±¼ä¶Î1µÄÊ±¼äµ¥Ôª.  Tbs1·¶Î§CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=CAN_BS2_6tq; //Ê±¼ä¶Î2µÄÊ±¼äµ¥Ôª.  Tbs2·¶Î§CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=6;  //·ÖÆµÏµÊı(Fdiv)Îªbrp+1	
/***************************************************************************************/			
  	CAN_Init(CAN1, &CAN_InitStructure);   // ³õÊ¼»¯CAN1 
    
		//ÅäÖÃ¹ıÂËÆ÷
 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  //¹ıÂËÆ÷0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32Î» 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32Î»ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32Î»MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//¹ıÂËÆ÷0¹ØÁªµ½FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //¼¤»î¹ıÂËÆ÷0
  	CAN_FilterInit(&CAN_FilterInitStructure);//ÂË²¨Æ÷³õÊ¼»¯
		
#if CAN1_RX0_INT_ENABLE
	
	  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0ÏûÏ¢¹ÒºÅÖĞ¶ÏÔÊĞí.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // Ö÷ÓÅÏÈ¼¶Îª1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // ´ÎÓÅÏÈ¼¶Îª0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}   
 
#if CAN1_RX0_INT_ENABLE	//Ê¹ÄÜRX0ÖĞ¶Ï
//ÖĞ¶Ï·şÎñº¯Êı			    
void CAN1_RX0_IRQHandler(void)
{
  	CanRxMsg RxMessage;
    CAN_Receive(CAN1, 0, &RxMessage);
}
#endif

/****************************************************************************
* Ãû    ³Æ: u8 CAN1_Send_Msg(u8* msg,u8 len)
* ¹¦    ÄÜ£ºcan·¢ËÍÒ»×éÊı¾İ(¹Ì¶¨¸ñÊ½:IDÎª0X12,±ê×¼Ö¡,Êı¾İÖ¡)
* Èë¿Ú²ÎÊı£ºlen:Êı¾İ³¤¶È(×î´óÎª8)				     
            msg:Êı¾İÖ¸Õë,×î´óÎª8¸ö×Ö½Ú.
* ·µ»Ø²ÎÊı£º0,³É¹¦;
           	ÆäËû,Ê§°Ü;
* Ëµ    Ã÷£º       
****************************************************************************/		
u8 CAN1_Send_Msg(u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=0x12;	 // ±ê×¼±êÊ¶·ûÎª0
  TxMessage.ExtId=0x12;	 // ÉèÖÃÀ©Õ¹±êÊ¾·û£¨29Î»£©
  TxMessage.IDE=0;		   // Ê¹ÓÃÀ©Õ¹±êÊ¶·û
  TxMessage.RTR=0;		   // ÏûÏ¢ÀàĞÍÎªÊı¾İÖ¡£¬Ò»Ö¡8Î»
  TxMessage.DLC=len;	   // ·¢ËÍÁ½Ö¡ĞÅÏ¢
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];				 // µÚÒ»Ö¡ĞÅÏ¢          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//µÈ´ı·¢ËÍ½áÊø
  if(i>=0XFFF)return 1;
  return 0;		
}

/****************************************************************************
* Ãû    ³Æ: u8 CAN1_Receive_Msg(u8 *buf)
* ¹¦    ÄÜ£ºcan¿Ú½ÓÊÕÊı¾İ²éÑ¯
* Èë¿Ú²ÎÊı£ºbuf:Êı¾İ»º´æÇø;	 			     
* ·µ»Ø²ÎÊı£º0,ÎŞÊı¾İ±»ÊÕµ½;
    		    ÆäËû,½ÓÊÕµÄÊı¾İ³¤¶È;
* Ëµ    Ã÷£º       
****************************************************************************/	
u8 CAN1_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//Ã»ÓĞ½ÓÊÕµ½Êı¾İ,Ö±½ÓÍË³ö 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//¶ÁÈ¡Êı¾İ	
    for(i=0;i<RxMessage.DLC;i++)
    buf[i]=RxMessage.Data[i]; 	
	return RxMessage.DLC;	
}














