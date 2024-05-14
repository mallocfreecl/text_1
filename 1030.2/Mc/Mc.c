/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
*disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:Mc.c
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
#include "Mc.h"
#include "Common.h"
#include "DrvAdc.h"
#include "McCfg.h"
#include "McLib.h"
#include "Sdk.h"
#include "Table.h"

/* Defines*/
/******************************************************************/
/* 自传转速低通滤波限值 22.08.17 by jxj */
#define MC_WM_SPEED_LP_MAX (10000)
#define MC_WM_SPEED_LP_MIN (-10000)

/* Variables */
/******************************************************************/
McAPI_t McAPI;

/* Function declaration */
/******************************************************************/
static void Mc_initPar(void);
static void Mc_initVar(void);

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Mc_init
* Description:
*
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
void Mc_init(void)
{
  McAPI.State = eMUnInit;  //未初始化状态

  Mc_initPar();   //参数初始化

  Mc_stopMotor();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Mc_initPar
* Description:
*
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static void Mc_initPar(void)
{
  static float x = 0.0;

  McAPI.McLibVer = McLibVer;    //电机库版本号

  McSvm.MaxAmp = McCfg.PwmMax / (TABLE_CFG_PWM_OUT_AMP_TABLE_SCALE); //McCfg.PwmMax？？

  x = (McCfg.AlignTime1 + McCfg.AlignTime2) * McCfg.LowTaskFreq;     //McCfg.AlignTime是校准/对齐时间
  if (x > 32767)               //限幅
  {
    x = 32767;
  }
  else if (x < 1)
  {
    x = 1;
  }
  else
  {
  }
  McCtrl.AlignTimeTotal = x;

  x = McCfg.AlignTime1 * McCfg.LowTaskFreq;
  if (x > 32767)
  {
    x = 32767;
  }
  else if (x < 1)
  {
    x = 1;
  }
  else
  {
  }
  McCtrl.AlignTime1 = x;

  McCtrl.OpenLoopEndSpeed = McCfg.OpenLoopEndSpeed;     //开环的最终速度

  x = McCfg.OpenLoopAcc * 65536.0 / McCfg.LowTaskFreq;  
  if (x > 2147483647)
  {
    x = 2147483647;
  }
  else if (x < 1)
  {
    x = 1;
  }
  else
  {
  }
  McCtrl.OpenLoopAcc = x;

  McCtrl.SpeedtoAngleDelta = (32768.0 * (1.0 / McCfg.HighTaskFreq) * 32768.0 / 30.0) * McCfg.PolePairs;

  McCtrl.SpeedPi.Kp = McCfg.SpeedKp;
  McCtrl.SpeedPi.Ki = McCfg.SpeedKi;
  McCtrl.SpeedPi.PiMin = McCfg.SpeedPiMin;
  McCtrl.SpeedPi.PiMax = McCfg.SpeedPiMax;
  McCtrl.SpeedPi.IMin = McCtrl.SpeedPi.PiMin;
  McCtrl.SpeedPi.IMax = McCtrl.SpeedPi.PiMax;
  /* 使用电机库4 修改相应的配置参数 22。08.17 by jxj*/
  McCtrl.IShuntPi.Kp = McCfg.IShuntKp;
  McCtrl.IShuntPi.Ki = McCfg.IShuntKi;
  McCtrl.IShuntPi.PiMin = McCfg.IShuntPiMin;
  McCtrl.IShuntPi.PiMax = McCfg.IShuntPiMax;
  McCtrl.IShuntPi.IMin = McCtrl.IShuntPi.PiMin;
  McCtrl.IShuntPi.IMax = McCtrl.IShuntPi.PiMax;

  McCtrl.IPhasePi.Kp = McCfg.IPhaseKp;
  McCtrl.IPhasePi.Ki = McCfg.IPhaseKi;
  McCtrl.IPhasePi.PiMin = McCfg.IPhasePiMin * McSvm.MaxAmp;
  McCtrl.IPhasePi.PiMax = McCfg.IPhasePiMax * McSvm.MaxAmp;
  McCtrl.IPhasePi.IMin = McCtrl.IPhasePi.PiMin;
  McCtrl.IPhasePi.IMax = McCtrl.IPhasePi.PiMax;

  McCtrl.CloseLoopEn = 1u;

  McCtrl.PhasePi.Kp = McCfg.PhaseKp;
  McCtrl.PhasePi.Ki = McCfg.PhaseKi;
  /* polyspace +1 MISRA-C3:17.3 */
  McCtrl.PhasePi.IMax = __ssat(Mat_FixMulScale(McCfg.MaxSpeed, McCtrl.SpeedtoAngleDelta, 0u), MAT_FIX_SAT);
  McCtrl.PhasePi.IMin = -McCtrl.PhasePi.IMax;
  McCtrl.PhasePi.PiMax = McCtrl.PhasePi.IMax;
  McCtrl.PhasePi.PiMin = -McCtrl.PhasePi.PiMax;

  McCtrl.AutoPhaseOffsetEn = 1u;

  /* 设置自转转速滤波参数，22.08.17 by yhd */
  McCtrl.WmSpeedLp.CoefA = McCfg.WmSpeedLpCoef;
  McCtrl.WmSpeedLp.CoefB = McCtrl.WmSpeedLp.CoefA;
  McCtrl.WmSpeedLp.Min = MC_WM_SPEED_LP_MIN;
  McCtrl.WmSpeedLp.Max = MC_WM_SPEED_LP_MAX;

  WmAPI.AngleOffset = McCfg.WmAngleOffset;
  WmAPI.ElecPeriodCntMax = McCfg.HighTaskFreq / McCfg.WmDetectFreqMin;
  /* polyspace +2 MISRA-C3:10.7 */
  WmAPI.TimeOutCntMax = McCfg.HighTaskFreq / McCfg.WmDetectFreqMin * 1.5;
  WmAPI.ElecPeriodToSpeedCoef = McCfg.HighTaskFreq / McCfg.PolePairs * 60.0;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Mc_startMotor
* Description:
*
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
void Mc_startMotor(s16 RefSpeed)
{
  McAPI.RefSpeed = RefSpeed;

  if (McAPI.State == eMStop)
  {
    Mc_initVar();

    Sdk_startMotorHw();
    McAPI.State = eMOpenLoop;
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Mc_initVar
* Description:
*
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
extern u16 StartTimeOutCnt;
static void Mc_initVar(void)
{
  u32 Sector = 0u;//扇区
  u32 Angle = 0u; //角度
  s32 Amp = 0u;   

  if (ABS(McAPI.WmSpeed) < McCtrl.OpenLoopEndSpeed)
  {
    if ((StartTimeOutCnt < 1000) && (WmAPI.ElecPeriodCnt < WmAPI.ElecPeriodCntMax) &&
        (WmAPI.TimeOutCnt < WmAPI.TimeOutCntMax))
    {
      if (((s32)McAPI.RefSpeed * (s32)McAPI.WmSpeed) < 0)
      {
        /*Align+openloop*/
        McAPI.ActSpeed = 0;
        McSvm.Angle = McCfg.AlignAngle1;
        McCtrl.AlignTimeCnt = McCtrl.AlignTimeTotal;
      }
      else
      {
        /*switch to openloop derectly*/
        McAPI.ActSpeed = McAPI.WmSpeed;
        McSvm.Angle = WmAPI.Angle;
        McCtrl.AlignTimeCnt = 0;
      }
    }
    else
    {
      /*Align+openloop*/
      McAPI.ActSpeed = 0;
      McSvm.Angle = McCfg.AlignAngle1;
      McCtrl.AlignTimeCnt = McCtrl.AlignTimeTotal;
    }

    McSvm.Amp = 0u;
    McCtrl.IPhaseRef = McCfg.OpenLoopRefCurrent;
  }
  else
  {
    if (((s32)McAPI.RefSpeed * (s32)McAPI.WmSpeed) < 0)
    {
      /* reverse rotation */
      McAPI.ActSpeed = 0;
      McCtrl.AlignTimeCnt = McCtrl.AlignTimeTotal << 1;
      McSvm.Angle = McCfg.AlignAngle1;

      McSvm.Amp = 0u;
      McCtrl.IPhaseRef = McCfg.OpenLoopRefCurrent;
    }
    else
    {
      McAPI.ActSpeed = McAPI.WmSpeed;
      McCtrl.AlignTimeCnt = 0;
      McSvm.Angle = WmAPI.Angle;

      Amp = (s32)(ABS(McAPI.WmSpeed)) * (s32)McSvm.MaxAmp / (s32)AdcResults.Voltage.Bat;
      Amp = (s32)Amp * McCfg.BEMFFactor / MC_CFG_BEMF_FACTOR_SCALE;
      if (Amp > McSvm.MaxAmp)
      {
        Amp = McSvm.MaxAmp;
      }
      McSvm.Amp = Amp;

      McCtrl.IPhaseRef = 0;
    }
  }

  /* Initialize variables for SVM */
  Angle = ((u32)McSvm.Angle) * 6u;
  Sector = (Angle >> 16u) & 7u;
  McSvm.Sector = (u16)Sector;
  McSvm.StoredSector = McSvm.Sector;
  McSvm.SectorCnt = 0;
  McSvm.T1 = 0u;
  McSvm.T2 = 0u;

  /* Initialize variables for motor control */
  /* polyspace +1 MISRA-C3:10.1 */
  McCtrl.ActSpeedMem = (s32)McAPI.ActSpeed << 16u;
  /* polyspace +1 MISRA-C3:21.2,17.3 */
  McCtrl.OpenLoopAngleDelta = __ssat(Mat_FixMulScale(McAPI.ActSpeed, McCtrl.SpeedtoAngleDelta, 0u), MAT_FIX_SAT);
  McCtrl.PhaseOffset = 0;
  McCtrl.IPhasePi.IOut = McSvm.Amp << 15u;
  McAPI.FaultFlag.B.OverCurrent = 0u;

  McAPI.PwmAmp = McSvm.Amp;

  McCtrl.IShuntLimitSts = 0u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Mc_stopMotor
* Description:
*
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
extern u16 BemfAmpFlt;
void Mc_stopMotor(void)
{
  McAPI.RefSpeed = 0;

  if (McAPI.State != eMStop)
  {
    McAPI.State = eMStop;

    Sdk_stopMotorHw();

    McAPI.ActSpeed = 0;
    McAPI.WmDetected = 0u;
    McAPI.FaultFlag.B.WmSample = 0u;

    /* 清零自转转速滤波值，22.08.17 by jxj */
    McCtrl.WmSpeedLp.Out = 0;
    /* 清零停止状态计时，22.08.17 by jxj */
    McCtrl.StopTime = 0u;

    AdcResults.RawAD.Iu = 0;
    AdcResults.RawAD.Iv = 0;
    AdcResults.RawAD.Iw = 0;
    AdcResults.RawAD.IuAbs = 0;
    AdcResults.RawAD.IvAbs = 0;
    AdcResults.RawAD.IwAbs = 0;
    AdcResults.RawAD.IShunt = 0;
    AdcResults.RawAD.IShuntPeak = 0;
    AdcResults.RawAD.IShuntPeakTmp = 0;

    BemfAmpFlt = 0u;

    McWm_init();
  }
}
