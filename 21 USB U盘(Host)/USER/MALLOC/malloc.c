#include "malloc.h"
/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: malloc.c                                                             *
* 文件简述：内存分配                                                             *
* 创建日期：2015.03.13                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                                                                     * 
**********************************************************************************
*********************************************************************************/

//内存池(4字节对齐)
__align(4) u8 inmenbase[INMEM_MAX_SIZE];													        //内部SRAM内存池
__align(4) u8 exmen2base[EXMEM_MAX_SIZE] __attribute__((at(0X68000000)));	//外部SRAM内存池
//内存管理表
u16 inmemmapbase[INMEM_ALLOC_TABLE_SIZE];													        //内部SRAM内存池MAP
u16 exmemmapbase[EXMEM_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000+EXMEM_MAX_SIZE)));	//外部SRAM内存池MAP
//内存管理参数	   
const u32 memtblsize[2]={INMEM_ALLOC_TABLE_SIZE,EXMEM_ALLOC_TABLE_SIZE};		//内存表大小
const u32 memblksize[2]={INMEM_BLOCK_SIZE,EXMEM_BLOCK_SIZE};					//内存分块大小
const u32 memsize[2]={INMEM_MAX_SIZE,EXMEM_MAX_SIZE};							//内存总大小

u8 	*membase[2]={inmenbase,exmen2base};			//内存池 管理2个区域的内存
u16 *memmap[2]={inmemmapbase,exmemmapbase}; //内存管理状态表
u8  memrdy[2]={0,0}; 						            //内存管理是否就绪

/****************************************************************************
* 名    称: void mymemcpy(void *des,void *src,u32 n)
* 功    能：复制内存
* 入口参数：*des:目的地址
            *src:源地址
            n:需要复制的内存长度(字节为单位)
* 返回参数：无
* 说    明： 		     
****************************************************************************/
void mymemcpy(void *des,void *src,u32 n)  
{  
    u8 *xdes=des;
	  u8 *xsrc=src; 
    while(n--)*xdes++=*xsrc++;  
}  

/****************************************************************************
* 名    称: void mymemset(void *s,u8 c,u32 count)  
* 功    能：设置内存
* 入口参数：*s:内存首地址
            c :要设置的值
            count:需要设置的内存大小(字节为单位)
* 返回参数：无
* 说    明： 		     
****************************************************************************/
void mymemset(void *s,u8 c,u32 count)  
{  
    u8 *xs = s;  
    while(count--)*xs++=c;  
}	 

//内存管理初始化  
//memx:所属内存块
void Memory_Init(u8 memx)  
{  
  mymemset(memmap[memx], 0,memtblsize[memx]*2);//内存状态表数据清零  
	mymemset(membase[memx], 0,memsize[memx]);	    //内存池所有数据清零  
	memrdy[memx]=1;								//内存管理初始化OK  
}  

/****************************************************************************
* 名    称: u8 Mem_perused(u8 memx)   
* 功    能：获取内存使用率
* 入口参数：memx:所属内存块
* 返回参数：使用率(0~100)
* 说    明： 		     
****************************************************************************/
u8 Mem_perused(u8 memx)  
{  
    u32 used=0;  
    u32 i;  
    for(i=0;i<memtblsize[memx];i++)  
    {  
        if(memmap[memx][i])used++; 
    } 
    return (used*100)/(memtblsize[memx]);  
}  

/****************************************************************************
* 名    称: u32 memin_malloc(u8 memx,u32 size)  
* 功    能：内存分配(内部调用)
* 入口参数：memx:所属内存块
            size:要分配的内存大小(字节)
* 返回参数：0XFFFFFFFF,代表错误;其他,内存偏移地址 
* 说    明： 		     
****************************************************************************/
u32 memin_malloc(u8 memx,u32 size)  
{  
    signed long offset=0;  
    u16 nmemb;	//需要的内存块数  
	  u16 cmemb=0;//连续空内存块数
    u32 i;  
	
    if(!memrdy[memx])Memory_Init(memx);//未初始化,先执行初始化 
    if(size==0)return 0XFFFFFFFF;//不需要分配

    nmemb=size/memblksize[memx];  	//获取需要分配的连续内存块数
    if(size%memblksize[memx])nmemb++;  
    for(offset=memtblsize[memx]-1;offset>=0;offset--)//搜索整个内存控制区  
    {     
		if(!memmap[memx][offset])cmemb++;//连续空内存块数增加
		else cmemb=0;								//连续内存块清零
		if(cmemb==nmemb)							//找到了连续nmemb个空内存块
		{
            for(i=0;i<nmemb;i++)  					//标注内存块非空 
            {  
               memmap[memx][offset+i]=nmemb;  
            }  
            return (offset*memblksize[memx]);//返回偏移地址  
		}
    }  
    return 0XFFFFFFFF;//未找到符合分配条件的内存块  
}  

/****************************************************************************
* 名    称: u8 memin_free(u8 memx,u32 offset)   
* 功    能：释放内存(内部调用) 
* 入口参数：offset:内存地址偏移
* 返回参数：0,释放成功;1,释放失败;
* 说    明： 		     
****************************************************************************/
u8 memin_free(u8 memx,u32 offset)  
{  
    int i;  
    if(!memrdy[memx])//未初始化,先执行初始化
	{
		Memory_Init(memx);    
        return 1;//未初始化  
    }  
    if(offset<memsize[memx])//偏移在内存池内. 
    {  
        int index=offset/memblksize[memx];			//偏移所在内存块号码  
        int nmemb=memmap[memx][index];	//内存块数量
        for(i=0;i<nmemb;i++)  						//内存块清零
        {  
          memmap[memx][index+i]=0;  
        }  
        return 0;  
    }else return 2;//偏移超区了.  SRAMIN
}  

/****************************************************************************
* 名    称: void Mem_free(u8 memx,void *use)
* 功    能：内存释放 
* 入口参数：memx: 内外内存选择  0：内部    1：外部
            use： 前面分配的内存首地址 
* 返回参数：无
* 说    明：
****************************************************************************/	
void Mem_free(u8 memx,void *use)  
{  
	u32 offset;  
    if(use==NULL)return;//地址为0.  
 	  offset=(u32)use-(u32)membase[memx];  
    memin_free(memx,offset);//释放内存     
}  

/****************************************************************************
* 名    称: void *Mem_malloc(u8 memx,u32 size)
* 功    能：内存分配
* 入口参数：memx: 内外内存选择  0：内部    1：外部
*           size：需要分配内存的大小
* 返回参数：分配到的内存首地址
* 说    明：比如变量ABC需要2K的内存  可这样分配 ABC=Mem_malloc(0,2048);
            注意该变量必须是指针变量，或者数组首个元素地址 因为该函数返回的是地址值
****************************************************************************/	
void *Mem_malloc(u8 memx,u32 size)  
{  
    u32 offset;  									      
	  offset=memin_malloc(memx,size);  	   				   
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((u32)membase[memx]+offset);  
}  

/****************************************************************************
* 名    称: void *Remem_malloc(u8 memx,void *old,u32 size);
* 功    能：重新内存分配
* 入口参数：memx: 内外内存选择  0：内部    1：外部
            old： 旧内存首地址
*           size：需要重新分配内存的大小
* 返回参数：分配到的内存首地址
* 说    明：
****************************************************************************/	
void *Remem_malloc(u8 memx,void *old,u32 size)  
{  
    u32 offset;  
    offset=memin_malloc(memx,size);  
    if(offset==0XFFFFFFFF)return NULL;     
    else  
    {  									   
	    mymemcpy((void*)((u32)membase[memx]+offset),old,size);	//拷贝旧内存内容到新内存   
      Mem_free(memx,old);  											  		    //释放旧内存
      return (void*)((u32)membase[memx]+offset);  				//返回新内存首地址
    }  
} 

