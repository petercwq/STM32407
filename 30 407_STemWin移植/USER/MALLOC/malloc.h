#ifndef __MALLOC_H
#define __MALLOC_H

#include "common.h"

/*********************************************************************************
*************************MCU启明 STM32F407应用开发板******************************
**********************************************************************************
* 文件名称: malloc.h                                                             *
* 文件简述：内存分配                                                             *
* 创建日期：2015.03.13                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                                                                     * 
**********************************************************************************
*********************************************************************************/

#ifndef NULL
#define NULL 0
#endif

//内存类型定义
#define INSRAM	0	  //内部内存 
#define EXSRAM  1	  //外部内存 

//内部内存  内存分配用到的管理参数  用户可根据需要自行修改
#define INMEM_BLOCK_SIZE			32  	   //内存块大小为32字节
#define INMEM_MAX_SIZE			80*1024  	 //最大管理内部内存80K
#define INMEM_ALLOC_TABLE_SIZE	 INMEM_MAX_SIZE/INMEM_BLOCK_SIZE 	//内存表大小

//外部内存  内存分配用到的管理参数  用户可根据需要自行修改
#define EXMEM_BLOCK_SIZE			32  	   //内存块大小为32字节
#define EXMEM_MAX_SIZE			800*1024   //最大管理外部内存800K
#define EXMEM_ALLOC_TABLE_SIZE	 EXMEM_MAX_SIZE/EXMEM_BLOCK_SIZE 	//内存表大小
		 
//内存分配的有关函数声明
void Memory_Init(u8 memx);//内存管理初始化函数 memx:内外内存选择 0:内部  1:外部
u8 Mem_perused(u8 memx);	//获得内存使用率
	
void *Mem_malloc(u8 memx,u32 size);			 

void *Remem_malloc(u8 memx,void *old,u32 size); 

void Mem_free(u8 memx,void *use);  	

#endif













