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
File        : GUI_TOUCH_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "GUI_X.h"
#include "touch.h" 

void GUI_TOUCH_X_ActivateX(void) {
}

void GUI_TOUCH_X_ActivateY(void) {
}


/*****************************************************
*
*     获取X点坐标
*/
int  GUI_TOUCH_X_MeasureX(void) {
  return RTouch_Read_XorY(0xD0);  //CMD_RDX=0XD0
}


/*****************************************************
*
*     获取Y点坐标
*/
int  GUI_TOUCH_X_MeasureY(void) {
  return RTouch_Read_XorY(0x90);  //CMD_RDY=0XD0
}


