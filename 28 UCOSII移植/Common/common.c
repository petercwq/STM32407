#include "common.h"
#include "includes.h"				//ucos 使用	    

/*********************************************************************************
*************************MCU启明 STM32F407核心开发板******************************
**********************************************************************************
* 文件名称: common.c                                                             *
* 文件简述：各个工程所需调用的公共文件                                           *
* 创建日期：2017.09.30                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：包涵数据类型定义、IO口位定义、位段定义与延时函数定义                 * 
**********************************************************************************
*********************************************************************************/	  

/****************************************************************************
* 名    称: void GPIO_group_OUT(_gpio_group *group,u16 outdata)
* 功    能：使用随意16个IO口组成一个16位并行输出口
* 入口参数：*group： 任意16个IO口为元素的结构体指针
            outdata: 16位输出数值
* 返回参数：无
* 说    明：outdata从高位开始赋值
****************************************************************************/
void GPIO_group_OUT(_gpio_group *group,u16 outdata)
{
  u8 t;
	for(t=0;t<16;t++)
    {               
        if((outdata&0x8000)>>15)  
				{
						switch(t)
						{
								case 0:	   group->data15=1; break;
								case 1:	   group->data14=1; break;
								case 2:	   group->data13=1; break;
								case 3:	   group->data12=1; break;
								case 4:	   group->data11=1; break;
								case 5:	   group->data10=1; break;
								case 6:	   group->data9=1;  break;
								case 7:	   group->data8=1;  break;
								case 8:	   group->data7=1;  break;
								case 9:	   group->data6=1;  break;
								case 10:	 group->data5=1;  break;
								case 11:	 group->data4=1;  break;
								case 12:	 group->data3=1;  break;
								case 13:	 group->data2=1;  break;
								case 14:	 group->data1=1;  break;
								case 15:	 group->data0=1;  break;
						}
				}
				else
				{
				  switch(t)
						{
								case 0:	   group->data15=0; break;
								case 1:	   group->data14=0; break;
								case 2:	   group->data13=0; break;
								case 3:	   group->data12=0; break;
								case 4:	   group->data11=0; break;
								case 5:	   group->data10=0; break;
								case 6:	   group->data9=0;  break;
								case 7:	   group->data8=0;  break;
								case 8:	   group->data7=0;  break;
								case 9:	   group->data6=0;  break;
								case 10:	 group->data5=0;  break;
								case 11:	 group->data4=0;  break;
								case 12:	 group->data3=0;  break;
								case 13:	 group->data2=0;  break;
								case 14:	 group->data1=0;  break;
								case 15:	 group->data0=0;  break;
						}
				}
     outdata<<=1; 	
	  }
}
/****************************************************************************
* 名    称: void GPIO_bits_OUT(GPIO_TypeDef* GPIOx, u8 start_bit, u8 bit_size,u16 outdata)
* 功    能：位段操作实现，同一IO口的几位并行输出操作
* 入口参数：* GPIOx：  对应的IO口
*           start_bit: 并行输出的起始位
*           bit_size:  要并行输出的位数
* 返回参数：无
* 说    明：start_bit: 0~14
            bit_size:  1~16 
            bit_size<=16-start_bit
****************************************************************************/
void GPIO_bits_OUT(GPIO_TypeDef* GPIOx, u8 start_bit, u8 bit_size,u16 outdata)
{
  u8 i=0;
	u16 bu1=0;u16 middata=1;

	if( bit_size>(16-start_bit) ) 
     bit_size=16-start_bit;
	
	i=start_bit;
	if(i>0)
		 {
			 while(i--)
         { bu1+=middata; middata*=2;}
		 }
	
   assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
   
	 GPIOx->ODR&=(  ( (0xffff<<bit_size) <<start_bit ) |bu1   ); 
	 GPIOx->ODR|=(outdata<<start_bit);		 
}

/*****************************************************************************
*****************************以下代码参考网上*********************************
*****************************************************************************/
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}

//以前面不同的是，前面滴答定时器工作在查询寄存器模式
//在使用UCOS时候需要用滴答定时器提供节拍并且产生节拍
//到时中断，所以此时的滴答定时器工作在中断模式 

static u8  fac_us=0; //us延时倍乘数			   

/****************************************************************************
* 名    称: delay_init()
* 功    能：延时函数初始化
* 入口参数：无
* 返回参数：无
* 说    明：使用ucos的时候,此函数会初始化ucos的时钟节拍
****************************************************************************/			   
void delay_init(void)
{
	u32 retool;

 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us=SYSCLK/8;		//SYSTICK的时钟固定为HCLK时钟的1/8	    
	retool=SYSCLK/8;		//每秒钟的计数次数   
	
	retool*=1000000/OS_TICKS_PER_SEC;//根据OS_TICKS_PER_SEC设定溢出时间
							                     //reload为24位寄存器,最大值:16777216,在168M下,约合0.7989s左右	  
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断 （中断 中断）
	SysTick->LOAD=retool; 	//每1/OS_TICKS_PER_SEC秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK
}								    

/****************************************************************************
* 名    称: void delay_us(u32 nus)
* 功    能：延时nus
* 入口参数：要延时的微妙数
* 返回参数：无
* 说    明：ucos提供的最小延时为ms级，实际运用中很多时候用到us级，通过以下实现.
*           就是在进行us延时是关闭ucos任务调度，延时完在开启任务调度
****************************************************************************/	    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 initial,operation,count=0;
	u32 reload=SysTick->LOAD;	//LOAD的值
	
	ticks=nus*fac_us; 			  //需要的节拍数	  		 
	count=0;
	OSSchedLock();				//关闭ucos任务调度，防止打断us延时
	initial=SysTick->VAL;    //刚进入时的计数器值
	while(1)
	{
		operation=SysTick->VAL;	
		if(operation!=initial)
		{	    
			if(operation<initial)count+=initial-operation;//SYSTICK是一个递减的计数器 
			else count+=reload-operation+initial;	    
			initial=operation;
			if(count>=ticks)break; //时间超过/等于要延迟的时间,则退出.
		}  
	};
	OSSchedUnlock();			//开启ucos调度 									    
}

//ms延时用ucos提供的 OSTimeDlyHMSM(......);

//systick中断服务函数
void SysTick_Handler(void)
{				   
	OSIntEnter();		   //进入中断
  OSTimeTick();      //调用ucos的时钟服务程序               
  OSIntExit();       //触发任务切换软中断
}


			 
 
