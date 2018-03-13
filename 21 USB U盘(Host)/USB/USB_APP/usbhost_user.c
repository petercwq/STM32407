#include "usbhost_user.h" 
#include "led.h"
#include "ff.h" 
#include "usart1.h"
#include "string.h"

/*********************************************************************************
*************************MCU启明 STM32F407核心应用板******************************
**********************************************************************************
* 文件名称: usbhost_user.c                                                       *
* 文件简述：usbhost_user驱动代码                                                 *
* 创建日期：2015.04.07                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                                                                     * 
**********************************************************************************
*********************************************************************************/  


/****************************************************************************
* 名    称: void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
* 功    能：USB OTG 底层IO初始化
* 入口参数：pdev:USB OTG内核结构体指针
* 返回参数：无	
* 说    明： 		     
****************************************************************************/
void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, ENABLE);//使能USB OTG时钟	钟
  //GPIOA11,A12设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;//PA11/12复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_OTG_FS);//PA11,AF10(USB)
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_OTG_FS);//PA12,AF10(USB)
}
//USB OTG 中断设置,开启USB FS中断
//pdev:USB OTG内核结构体指针
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能通道
  NVIC_Init(&NVIC_InitStructure);//配置 
	
}
//USB OTG 中断设置,开启USB FS中断
//pdev:USB OTG内核结构体指针
void USB_OTG_BSP_DisableInterrupt(void)
{ 
}
//USB OTG 端口供电设置(本例程未用到)
//pdev:USB OTG内核结构体指针
//state:0,断电;1,上电
void USB_OTG_BSP_DriveVBUS(USB_OTG_CORE_HANDLE *pdev, uint8_t state)
{ 
}
//USB_OTG 端口供电IO配置(本例程未用到)
//pdev:USB OTG内核结构体指针
void  USB_OTG_BSP_ConfigVBUS(USB_OTG_CORE_HANDLE *pdev)
{ 
} 
//USB_OTG us级延时函数
//官方例程采用的是定时器2来实现的.
//usec:要延时的us数.
void USB_OTG_BSP_uDelay (const uint32_t usec)
{ 
   	delay_us(usec);
}
//USB_OTG ms级延时函数
//msec:要延时的ms数.
void USB_OTG_BSP_mDelay (const uint32_t msec)
{  
	delay_ms(msec);
}
   




static u8 AppState;
extern USB_OTG_CORE_HANDLE  USB_OTG_Core;

//USB OTG 中断服务函数
//处理所有USB中断
void OTG_FS_IRQHandler(void)
{
  	USBH_OTG_ISR_Handler(&USB_OTG_Core);
} 
//USB HOST 用户回调函数.
USBH_Usr_cb_TypeDef USR_Callbacks=
{
	USBH_USR_Init,
	USBH_USR_DeInit,
	USBH_USR_DeviceAttached,
	USBH_USR_ResetDevice,
	USBH_USR_DeviceDisconnected,
	USBH_USR_OverCurrentDetected,
	USBH_USR_DeviceSpeedDetected,
	USBH_USR_Device_DescAvailable,
	USBH_USR_DeviceAddressAssigned,
	USBH_USR_Configuration_DescAvailable,
	USBH_USR_Manufacturer_String,
	USBH_USR_Product_String,
	USBH_USR_SerialNum_String,
	USBH_USR_EnumerationDone,
	USBH_USR_UserInput,
	USBH_USR_MSC_Application,
	USBH_USR_DeviceNotSupported,
	USBH_USR_UnrecoveredError
};
/////////////////////////////////////////////////////////////////////////////////
//以下为各回调函数实现.

//USB HOST 初始化 
void USBH_USR_Init(void)
{
	
}
//检测到U盘插入
void USBH_USR_DeviceAttached(void)//U盘插入
{
	LED1=1;
}
//检测到U盘拔出
void USBH_USR_DeviceDisconnected (void)//U盘移除
{
	LED1=0;
}  
//复位从机
void USBH_USR_ResetDevice(void)
{
}
//检测到从机速度
//DeviceSpeed:从机速度(0,1,2 / 其他)
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
	if(DeviceSpeed==HPRT0_PRTSPD_HIGH_SPEED)
	{
 	}  
	else if(DeviceSpeed==HPRT0_PRTSPD_FULL_SPEED)
	{
	}
	else if(DeviceSpeed==HPRT0_PRTSPD_LOW_SPEED)
	{ 
	}
	else
	{
	}
}
//检测到从机的描述符
//DeviceDesc:设备描述符指针
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{ 
	USBH_DevDesc_TypeDef *hs;
	hs=DeviceDesc;   
}
//从机地址分配成功
void USBH_USR_DeviceAddressAssigned(void)
{
}
//配置描述符获有效
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
	USBH_InterfaceDesc_TypeDef *id; 
	id = itfDesc;   
	if((*id).bInterfaceClass==0x08)
	{
	}else if((*id).bInterfaceClass==0x03)
	{
	}    
}
//获取到设备Manufacturer String
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
}
//获取到设备Product String 
void USBH_USR_Product_String(void *ProductString)
{
}
//获取到设备SerialNum String 
void USBH_USR_SerialNum_String(void *SerialNumString)
{   
} 
//设备USB枚举完成
void USBH_USR_EnumerationDone(void)
{   
} 
//无法识别的USB设备
void USBH_USR_DeviceNotSupported(void)
{    
}  
//等待用户输入按键,执行下一步操作
USBH_USR_Status USBH_USR_UserInput(void)
{ 
	return USBH_USR_RESP_OK;
} 
//USB接口电流过载
void USBH_USR_OverCurrentDetected (void)
{
} 

extern u8 USH_User_App(void);		//用户测试主程序
//USB HOST MSC类用户应用程序
int USBH_USR_MSC_Application(void)
{
	u8 res=0;
  	switch(AppState)
  	{
    	case USH_USR_FS_INIT://初始化文件系统 
			AppState=USH_USR_FS_TEST;
      		break;
    	case USH_USR_FS_TEST:	//执行USB OTG 测试主程序
			res=USH_User_App(); //用户主程序
     		res=0;
			if(res)AppState=USH_USR_FS_INIT;
      		break;
    	default:break;
  	} 
	return res;
}
//用户要求重新初始化设备
void USBH_USR_DeInit(void)
{
  	AppState=USH_USR_FS_INIT;
}
//无法恢复的错误!!  
void USBH_USR_UnrecoveredError (void)
{
}
////////////////////////////////////////////////////////////////////////////////////////
//用户定义函数,实现fatfs diskio的接口函数 
extern USBH_HOST              USB_Host;

//获取U盘状态
//返回值:0,U盘未就绪
//      1,就绪
u8 USBH_UDISK_Status(void)
{
	return HCD_IsDeviceConnected(&USB_OTG_Core);//返回U盘状态
}

/****************************************************************************
* 名    称: u8 USBH_UDISK_Read(u8* buf,u32 sector,u32 cnt)
* 功    能：读U盘 
* 入口参数：buf:读数据缓存区
            sector:扇区地址
            cnt:扇区个数		
* 返回参数：错误状态;0,正常;其他,错误代码;	
* 说    明： 		     
****************************************************************************/ 
u8 USBH_UDISK_Read(u8* buf,u32 sector,u32 cnt)
{
	u8 res=1;
	if(HCD_IsDeviceConnected(&USB_OTG_Core)&&AppState==USH_USR_FS_TEST)//连接还存在,且是APP测试状态
	{  		    
		do
		{
			res=USBH_MSC_Read10(&USB_OTG_Core,buf,sector,512*cnt);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);		      
			if(!HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				res=1;//读写错误
				break;
			};   
		}while(res==USBH_MSC_BUSY);
	}else res=1;		  
	if(res==USBH_MSC_OK)res=0;	
	return res;
}


/****************************************************************************
* 名    称: u8 USBH_UDISK_Write(u8* buf,u32 sector,u32 cnt)
* 功    能：写U盘  
* 入口参数：buf:写数据缓存区
            sector:扇区地址
            cnt:扇区个数	
* 返回参数：错误状态;0,正常;其他,错误代码;	
* 说    明： 		     
****************************************************************************/	 
u8 USBH_UDISK_Write(u8* buf,u32 sector,u32 cnt)
{
	u8 res=1;
	if(HCD_IsDeviceConnected(&USB_OTG_Core)&&AppState==USH_USR_FS_TEST)//连接还存在,且是APP测试状态
	{  		    
		do
		{
			res=USBH_MSC_Write10(&USB_OTG_Core,buf,sector,512*cnt); 
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);		      
			if(!HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				res=1;//读写错误
				break;
			};   
		}while(res==USBH_MSC_BUSY);    
	}else res=1;		  
	if(res==USBH_MSC_OK)res=0;	
	return res;
}

/****************************************************************************
* 名    称: u8 getUfree_volume(u8 *drv,u32 *total,u32 *free)
* 功    能：得到磁盘剩余容量
* 入口参数：drv:磁盘编号("0:"/"1:")
            total:总容量	 （单位KB）
            free:剩余容量	 （单位KB）
* 返回参数：错误状态;0,正常;其他,错误代码;	
* 说    明： 		     
****************************************************************************/	
u8 getUfree_volume(u8 *drv,u32 *total,u32 *free)
{
	FATFS *fs1;
	u8 res;
    u32 fre_clust=0, fre_sect=0, tot_sect=0;
    //得到磁盘信息及空闲簇数量
    res =(u32)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//得到总扇区数
	    fre_sect=fre_clust*fs1->csize;			//得到空闲扇区数	   
#if _MAX_SS!=512				  				//扇区大小不是512字节,则转换为512字节
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		*total=tot_sect>>1;	//单位为KB
		*free=fre_sect>>1;	//单位为KB 
 	}
	return res;
}	















































