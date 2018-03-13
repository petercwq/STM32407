#ifndef __TASK_H
#define __TASK_H
#include "common.h"
#include "includes.h"

////////////////////////////////////////////////////////////////////////////////////

/***************************UCOSII任务设置**************************************/

//LED0任务

#define LED0_TASK_PRIO       			8   //设置任务优先级

#define LED0_STK_SIZE  		    		64  //设置任务堆栈大小

extern OS_STK  LED0_TASK_STK[LED0_STK_SIZE];  //任务堆栈	

void LED0_Task(void *pdata);          //任务函数


//LED1任务

#define LED1_TASK_PRIO       		5    //设置任务优先级

#define LED1_STK_SIZE  					64   //设置任务堆栈大小

extern OS_STK  LED1_TASK_STK[LED1_STK_SIZE];  //任务堆栈

void LED1_Task(void *pdata);         //任务函数




#endif



