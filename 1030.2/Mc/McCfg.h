/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
*disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:McCfg.h
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
#ifndef MC_CFG_H
#define MC_CFG_H

/* Includes*/
/******************************************************************/
#include "Mat.h"
#include "SdkTypes.h"

/* Defines*/
/******************************************************************/
#define MC_CFG_MAX_REF_CURRENT_LIMIT_TABLE_LEN (16u)

#define MC_CFG_BEMF_FACTOR_SCALE (1000)

typedef struct
{
  /* 自传转速低通滤波 22.08.17 by jxj */
  TMat_Lp WmSpeedLp;
  /* 风扇停止时间 22.08.17 by jxj */
  u16 StopTime;

  s16 SpeedtoAngleDelta;

  u16 AlignTime1;
  u16 AlignTimeTotal;
  u16 AlignTimeCnt;

  s32 OpenLoopAcc;
  s16 OpenLoopEndSpeed;

  s32 ActSpeedMem;
  s16 OpenLoopAngleDelta;
  s16 CloseLoopAngleDelta;

  /* 母线电流 22.08.17 by jxj */
  s32 IShuntRef;
  s16 IShuntErr;
  /* 相电流 22.08.17 by jxj */
  s16 IPhaseRef;
  s16 IPhaseErr;

  s16 PhaseDiff;
  s16 PhaseOffset;

  u8 CloseLoopEn;
  u8 AutoPhaseOffsetEn;
  u8 IShuntLimitSts;

  TMat_Pi SpeedPi;
  /* 母线电流积分环 22.08.17 by jxj */
  TMat_Pi IShuntPi;
  /* 相电流积分环 22.08.17 by jxj */
  TMat_Pi IPhasePi;
  /* 相位环 22.08.17 by jxj */
  TMat_Pi PhasePi;
} McCtrl_t;

typedef struct
{
  u16 Amp;
  u16 MaxAmp;
  u16 Angle;
  u16 Sector;
  u16 StoredSector;
  u8 SectorCnt;
  u16 T1;
  u16 T2;
} McSvm_t;

typedef struct
{
  float HighTaskPeriod;
  float HighTaskFreq;
  float LowTaskPeriod;
  float LowTaskFreq;

  u16 PwmMax;
  u16 RisingEdgeVoltSampleDelay;
  u16 FallingEdgeVoltSampleDelay;
  u16 CurrentSampleTimeCnt;
  u16 CurrentSettleTimeCnt;

  u16 PolePairs;
  u16 BEMFFactor; // Phase Peak Voltage, 0.001V/rpm

  u16 AlignAngle1;
  float AlignTime1;
  u16 AlignAngle2;
  float AlignTime2;

  float OpenLoopAcc;      // rpm/s
  s16 OpenLoopStartSpeed; // rpm
  s16 OpenLoopEndSpeed;   // rpm

  s16 OpenLoopRefCurrent;
  u16 OpenLoopMinAmp;

  s16 CloseLoopSwitchRefCurr;

  u16 MaxSpeed; // rpm

  s16 SpeedErrorLimit; // rpm

  s16 SpeedKp;
  s16 SpeedKi;
  s16 SpeedPiMin;
  s16 SpeedPiMax;

  /* 母线电流环参数 22.08.17 by jxj */
  s16 IShuntAllowAmp;
  s16 IShuntErrGain;
  s16 IShuntKp;
  s16 IShuntKi;
  s16 IShuntPiMin;
  s16 IShuntPiMax;

  /* 相电流环参数 22.08.17 by jxj */
  s16 IPhaseErrGain;
  s16 IPhaseKp;
  s16 IPhaseKi;
  float IPhasePiMin;
  float IPhasePiMax;

  s16 PhaseKp;
  s16 PhaseKi;
  s16 PhasePiMin;
  s16 PhasePiMax;

  s16 PhaseLowSpeedTH;
  s16 PhaseKpLowSpeed;
  s16 PhaseKiLowSpeed;

  u16 WmAngleOffset;
  s16 BEMFMin;
  u16 WmLowSpeedBEMFThreshold;
  s16 WmAngleLowSpeedLpA;
  s16 WmAngleHighSpeedLpA;
  float WmDetectFreqMin;
  float WmTimeOutThreshold;
  /* 自传战速低通滤波 22.08.17 by jxj */
  s16 WmSpeedLpCoef;

  u16 WmAmpThreshold;

} McCfg_t;

/* Variables */
/******************************************************************/
extern McCfg_t McCfg;
extern s16 MaxRefCurrLimitTab_X[];
extern s16 MaxRefCurrLimitTab_Y[];

extern McCtrl_t McCtrl;
extern McSvm_t McSvm;

/* Function declaration */
/******************************************************************/

#endif // !MC_H
