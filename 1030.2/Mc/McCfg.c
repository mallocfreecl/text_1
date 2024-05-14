/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
*disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:McCfg.c
********************************************************************
* Project/Product:
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

/* Includes*/
/******************************************************************/
#include "McCfg.h"
#include "Mc.h"

/* Defines*/
/******************************************************************/
#define MC_CFG_HIGH_TASK_PERIOD (200.0)
#define MC_CFG_LOW_TASK_PERIOD (3000.0)

/* Variables */
/******************************************************************/
McCtrl_t McCtrl;
McSvm_t McSvm;
/* 该电机参数适用于 扇叶直径425mm 转速 625~2750rpm,2022年4月3日，by,jxj */
McCfg_t McCfg = {
  .HighTaskPeriod = MC_CFG_HIGH_TASK_PERIOD,
  .HighTaskFreq = 1000000.0 / MC_CFG_HIGH_TASK_PERIOD,
  .LowTaskPeriod = MC_CFG_LOW_TASK_PERIOD,
  .LowTaskFreq = 1000000.0 / MC_CFG_LOW_TASK_PERIOD,

  .PwmMax = 599u, // CCU6_T12PR
  .RisingEdgeVoltSampleDelay = 24u,
  .FallingEdgeVoltSampleDelay = 24u,
  .CurrentSampleTimeCnt = 36u,
  .CurrentSettleTimeCnt = 10u,

  .PolePairs = 4u,
  .BEMFFactor = 42u,

  .AlignTime1 = 0.5,
  .AlignAngle1 = (U16_MAX) >> 2u,
  .AlignTime2 = 1.5,
  .AlignAngle2 = 0u,

  .OpenLoopAcc = 50.0,
  .OpenLoopStartSpeed = 10,
  .OpenLoopEndSpeed = 150,

  .OpenLoopRefCurrent = 300,
  .OpenLoopMinAmp = 1000,

  .CloseLoopSwitchRefCurr = 50,

  .MaxSpeed = 5000.0,

  .SpeedErrorLimit = 150,

  .SpeedKp = 5000,
  .SpeedKi = 40,
  .SpeedPiMin = 50,
  .SpeedPiMax = 700,

  /* 使用电机库4 修改相应的配置参数 22.08.17 by jxj */
  .IShuntAllowAmp = 10,
  .IShuntErrGain = 10,
  .IShuntKp = 5000,
  .IShuntKi = 40,
  .IShuntPiMin = 100,
  .IShuntPiMax = 1000,

  .IPhaseErrGain = 10,

  .IPhaseKp = 8000,
  .IPhaseKi = 100,
  .IPhasePiMin = 0.05,
  .IPhasePiMax = 1.05,
  .PhaseKp = 250,
  .PhaseKi = 5,
  /* 修改回400 防止大负载快速降速超调造成振荡 22.06.20 by jxj */
  .PhaseLowSpeedTH = 400,
  .PhaseKpLowSpeed = 50,
  .PhaseKiLowSpeed = 5,

  .WmAngleOffset = 21872u,
  .BEMFMin = 50,
  .WmLowSpeedBEMFThreshold = 1500,
  .WmAngleLowSpeedLpA = 7827,
  .WmAngleHighSpeedLpA = 18247,
  .WmDetectFreqMin = 2.0,
  .WmTimeOutThreshold = 1.0,

  .WmSpeedLpCoef = 2000,

  .WmAmpThreshold = 100,
};

// Voltage
s16 MaxRefCurrLimitTab_X[MC_CFG_MAX_REF_CURRENT_LIMIT_TABLE_LEN] = {80,  90,  100, 110, 120, 130, 140, 150,
                                                                    160, 170, 180, 190, 200, 210, 220, 230};
// RefCurr 35A
s16 MaxRefCurrLimitTab_Y[MC_CFG_MAX_REF_CURRENT_LIMIT_TABLE_LEN] = {370, 440, 500, 650, 700, 740, 780, 750,
                                                                    740, 740, 730, 730, 730, 730, 730, 730};

/* Function declaration */
/******************************************************************/
