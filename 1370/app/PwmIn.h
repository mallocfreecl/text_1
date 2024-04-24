/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:PwmIn.h
********************************************************************
* Project/Product:LXT9820
* Title:
* Author:zyg
*********************************************************************
* Description:
*
* (Requirements, pseudo code and etc.)
*********************************************************************
* Limitations:
*
* (limitations)
*********************************************************************
*********************************************************************
* Revision History：
*
* Version       Date         	  Author         Descriptions
* ----------    --------------  ------------   ----------------------------------------
*     1.0       2021-09-06        zyg            Original
*
*
********************************************************************
*END_FILE_HDR*/
#ifndef PWMIN_H
#define PWMIN_H

/* Includes*/
/******************************************************************/
#include "AppTypes.h"

/* Defines*/
/******************************************************************/
typedef struct {
  u16 Freq;
  u16 LowDuty;
  u16 HighDuty;
  u16 BusIdleTime;
  u16 BusState;
  u8 CaptureEnable;
}PwmIn_t;

/* Variables */
/******************************************************************/
extern PwmIn_t PwmIn;

/* Function declaration */
/******************************************************************/
extern void PwmIn_CaptureHandler(void);
extern void PwmIn_CaptureTimerOverFlowHandler(void);

extern void PwmIn_PeriodTask(void);

#endif
