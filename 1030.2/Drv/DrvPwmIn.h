/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
* disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:DrvPwmIn.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 检测Pwm输入信号的频率、占空比，信号接地、接电源状态
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
* Version       Date         	  Author       Descriptions
* ----------    --------------  ------------   ------------
* 1.0           2021-12-09      yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/
#ifndef DRV_PWM_IN_H
#define DRV_PWM_IN_H

/* Includes*/
/******************************************************************/
#include "SdkTypes.h"

/* Defines*/
/******************************************************************/
#define DRV_PWM_IN_PERIOD_TASK_TIME_BASE (10u) // ms

typedef struct
{
  u16 Freq;
  u16 LowDuty;
  u16 HighDuty;
  u16 BusIdleTime;
  u16 BusState;
  u8 CaptureEn;
} PwmInAPI_t;

/* Variables */
/******************************************************************/
extern PwmInAPI_t PwmInAPI;

/* Function declaration */
/******************************************************************/
extern void DrvPwmIn_capHandler(void);
extern void DrvPwmIn_capTmrOverFlowHandler(void);

extern void DrvPwmIn_step(void);

#endif
