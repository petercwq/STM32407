/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUIConf.h
Purpose     : Configures abilities, fonts etc.
----------------------------------------------------------------------
*/


#ifndef GUICONF_H
#define GUICONF_H

#define GUI_OS                    (0)  //这里指对操作系统的支持因为我们只是单纯的移植UCGUI
																				//所以要关掉，要不然会出现很多东西无法编译
#define GUI_SUPPORT_TOUCH         (1)  //这里是对触摸屏的支持
#define GUI_SUPPORT_UNICODE       (1)  //UNICODE编码支持，如果只是单纯的想显示英文是可以关掉

#define GUI_DEFAULT_FONT          &GUI_Font6x8
#define GUI_ALLOC_SIZE         40*1024 /* Size of dynamic memory ... For WM and memory devices*/

/*********************************************************************
*
*         Configuration of available packages
*/

#define GUI_WINSUPPORT            1  //窗口支持，一般是打开的
#define GUI_SUPPORT_MEMDEV        1  //内存控制，开
#define GUI_SUPPORT_AA            1  //抗锯齿，为了性能着想，还是关了好

#endif  /* Avoid multiple inclusion */



