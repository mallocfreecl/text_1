/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
*disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:Mc.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
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
* Version       Date         	  Author       Descriptions
* ----------    --------------  ------------   ------------
* 1.0           2021-12-09      yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/
#ifndef MC_H
#define MC_H

/* Includes*/
/******************************************************************/
#include "SdkTypes.h"

/* Defines*/
/******************************************************************/
typedef enum
{
  eMUnInit = 0,
  eMStop,
  eMOpenLoop,
  eMCloseLoop,
  eMFault,
} MotorState_t;

typedef struct
{
  s16 RefSpeed;
  s16 ActSpeed;
  s16 WmSpeed;
  /* 滤波后的自转转速，22.08.17 by jxj */
  s16 WmSpeedFlt;

  u8 WmValid;
  u8 WmDetected;

  u16 PwmAmp;
  u16 BEMFAmp;

  MotorState_t State;

  /* polyspace-begin MISRA-C3:6.1 */
  union {
    u8 R;

    struct
    {
      u8 OverCurrent : 1;
      u8 WmSample : 1;
    } B;
  } FaultFlag;
  /* polyspace-end MISRA-C3:6.1 */

  u16 McLibVer;
} McAPI_t;

/* Variables */
/******************************************************************/
extern McAPI_t McAPI;

/* Function declaration */
/******************************************************************/
extern void Mc_init(void);
extern void Mc_startMotor(s16 RefSpeed);
extern void Mc_stopMotor(void);

extern void Mc_LowTask(void);
extern void Mc_HighTask(void);
extern void Mc_PwmStartHandler(void);
extern void Mc_PwmPeriodHandler(void);

#endif // !MC_H
