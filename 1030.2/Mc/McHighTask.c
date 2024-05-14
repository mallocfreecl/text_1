/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or disclosed
* without the written authorization from
* XiangBin Electric.
********************************************************************
* File Name:McHighTask.c
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
* 1.0           2021-08-17      yhd            Original
* 1.1           2022-03-30      yhd            分离电机库
*
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#include "Common.h"
#include "Mc.h"
#include "McCfg.h"
#include "McLib.h"
#include "Sdk.h"

#include "ccu6.h"
#include "timer2x.h"

/* Defines*/
/******************************************************************/
#define MC_SWITCH_CLOSE_LOOP_SECTOR_CNT_MIN (2u)
#define MC_CURRENT_PI_BLANK_TIME_CNT (500u)
/* 自传转速空白检测时间 22.08.17 by jxj */
#define MC_WM_DETECT_BLANK_TIME (500u)
/* 开环切闭环积分制赋值限制 22.06.30 by.jxj */
#define MC_PHASE_LOW_SPEED_START_LIMIT_TH (50u)
/* Variables */
/******************************************************************/
static volatile PwmOutVar_t *p = &(PwmOutBuf.Buf[1u]);

/* Function declaration */
/******************************************************************/
SDK_STATIC_INLINE void Mc_calcPhaseCurrent(void);
SDK_STATIC_INLINE void Mc_switchToCloseLoop(void);

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Mc_HighTask
* Description:
*
* ExeTime@24MHz:100.375us,interrupt nested include
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
void Mc_HighTask(void)
{
  u16 SvmAmpTmp = 0u;
  static u16 RunTime = 0u;
  static u8 VoltBufIdx = 0u;

  if ((McAPI.State == eMOpenLoop) || (McAPI.State == eMCloseLoop))
  {
    if (RunTime < U16_MAX)
    {
      RunTime++;
    }

    if (McSvm.StoredSector != McSvm.Sector)
    {
      /*
       *扇区切换的时候不计算电流，避免切换时计算错误
       */
      if ((McSvm.StoredSector != 2u) && (McSvm.StoredSector != 3u))
      {
        AdcResults.RawAD.IShuntPeak = AdcResults.RawAD.IShuntPeakTmp;
        AdcResults.RawAD.IShuntPeakTmp = 0;
      }
      McSvm.StoredSector = McSvm.Sector;
    }
    else
    {
      Mc_calcPhaseCurrent();
    }

    if (McAPI.State == eMOpenLoop)
    {
      if ((ABS(McAPI.ActSpeed) >= McCtrl.OpenLoopEndSpeed) && (McSvm.SectorCnt > MC_SWITCH_CLOSE_LOOP_SECTOR_CNT_MIN) &&
          (McCtrl.CloseLoopEn != 0u))
      {
        Mc_switchToCloseLoop();
      }

      McSvm.Angle += McCtrl.OpenLoopAngleDelta;
    }
    else
    {
      McSvm.Angle += McCtrl.CloseLoopAngleDelta;
    }

    // if (McAPI.State == eMCloseLoop)
    {
      /* 修改参数名称 22.08.17 by jxj */
      /* polyspace +2 MISRA-C3:21.2,17.3 */
      McCtrl.IPhaseErr =
        __ssat(((s32)McCtrl.IPhaseRef - (s32)AdcResults.RawAD.IShuntPeak) * (s32)McCfg.IPhaseErrGain, 16u);

      if ((RunTime < MC_CURRENT_PI_BLANK_TIME_CNT) && (ABS(McAPI.WmSpeed) >= McCtrl.OpenLoopEndSpeed))
      {
        McCtrl.IPhaseErr = 0;
      }

      SvmAmpTmp = Mat_ExePi(&McCtrl.IPhasePi, McCtrl.IPhaseErr);
      if ((SvmAmpTmp < McCfg.OpenLoopMinAmp) &&
          ((McAPI.State == eMOpenLoop) || (ABS(McAPI.ActSpeed) <= McCfg.PhaseLowSpeedTH)))
      {
        SvmAmpTmp = McCfg.OpenLoopMinAmp;
        /* 防止切换时输出值出现突变 22.06.12 by jxj*/
        /* 闭环后才进行重新赋值积分值 防止干涉到开环和闭环 22.06.23 by jxj*/
        if ((McAPI.State == eMCloseLoop) &&
            (ABS(McAPI.ActSpeed) > (McCfg.PhaseLowSpeedTH - MC_PHASE_LOW_SPEED_START_LIMIT_TH)))
        {
          /* polyspace +2 MISRA-C3:10.1 */ /* polyspace +2 MISRA-C3:17.3*/
          McCtrl.IPhasePi.IOut = ((s32)SvmAmpTmp << 15u) - (__ssat(McCtrl.IPhaseErr * ((s32)McCtrl.IPhasePi.Kp), 31u));
        }
      }
      McSvm.Amp = SvmAmpTmp;
    }

    Mc_exeSvm(&McSvm);

    if (McSvm.Sector != McSvm.StoredSector)
    {
      if (McSvm.SectorCnt <= MC_SWITCH_CLOSE_LOOP_SECTOR_CNT_MIN)
      {
        McSvm.SectorCnt++;
      }

      Mc_setPhaseVoltSample();
      Mc_exePhaseCtrl(McSvm.StoredSector);
    }
  }
  else
  {
    RunTime = 0u;

    PwmOutBuf.Buf[0u].comp60up = McCfg.PwmMax >> 1u;
    PwmOutBuf.Buf[0u].comp60down = PwmOutBuf.Buf[0u].comp60up;
    PwmOutBuf.Buf[0u].comp61up = PwmOutBuf.Buf[0u].comp60up;
    PwmOutBuf.Buf[0u].comp61down = PwmOutBuf.Buf[0u].comp60up;
    PwmOutBuf.Buf[0u].comp62up = PwmOutBuf.Buf[0u].comp60up;
    PwmOutBuf.Buf[0u].comp62down = PwmOutBuf.Buf[0u].comp60up;
    PwmOutBuf.Buf[1u] = PwmOutBuf.Buf[0u];

    /*
     *
     * ExeTime@24MHz:64.54us,interrupt nested include
     */
    AdcResults.RawAD.VoltU = ADC1->RES_OUT0.bit.OUT_CH0;
    AdcResults.RawAD.VoltV = ADC1->RES_OUT2.bit.OUT_CH2;
    AdcResults.RawAD.VoltW = ADC1->RES_OUT3.bit.OUT_CH3;

    if (VoltBufIdx >= DRV_ADC_PHASE_VOLTAGE_BUF_LEN)
    {
      VoltBufIdx = 0u;
    }
    AdcResults.RawAD.VoltUBuf[VoltBufIdx] = AdcResults.RawAD.VoltU;
    AdcResults.RawAD.VoltVBuf[VoltBufIdx] = AdcResults.RawAD.VoltV;
    AdcResults.RawAD.VoltWBuf[VoltBufIdx] = AdcResults.RawAD.VoltW;
    VoltBufIdx++;

    /* 自传转速检测空白时间 22.08.17 by jxj */
    if (McCtrl.StopTime >= MC_WM_DETECT_BLANK_TIME)
    {
      McWm_step(AdcResults.RawAD.VoltU, AdcResults.RawAD.VoltV, AdcResults.RawAD.VoltW);
    }

    /* Sample IShunt Zero */
    p->IShuntTrig1 = U16_MAX - (McCfg.PwmMax + 80u);
    p->IShuntTrig2 = p->IShuntTrig1;

    /* 清除读取的电流值 22.08.17 by jxj */
    AdcResults.RawAD.IShunt = 0;
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Mc_calcPhaseCurrent
* Description:
* 1.重建三相电流
* 2.计算电流峰值
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
SDK_STATIC_INLINE void Mc_calcPhaseCurrent(void)
{
  s16 R0mioffs = 0;
  s16 OffsmiR1 = 0;
  s16 R1miR0 = 0;

  /*result 0 minus CSA Offset*/
  R0mioffs = AdcResult0 - AdcResults.RawAD.IShuntZero;
  /*CSA Offset minus result 1*/
  OffsmiR1 = AdcResults.RawAD.IShuntZero - AdcResult1;
  /*Result 1 - Result 0*/
  R1miR0 = AdcResult1 - AdcResult0;

  /* 计算母线电流 22.08.17 by jxj */
  AdcResults.RawAD.IShunt = ABS((s32)R0mioffs) * p->T1 + ABS((s32)OffsmiR1) * p->T2;

  switch (McSvm.StoredSector)
  {
  case 0u: {
    AdcResults.RawAD.Iu = R0mioffs;
    AdcResults.RawAD.Iv = R1miR0;
  }
  break;
  case 1u: {
    AdcResults.RawAD.Iu = R1miR0;
    AdcResults.RawAD.Iv = R0mioffs;
  }
  break;
  case 2u: {
    AdcResults.RawAD.Iu = OffsmiR1;
    AdcResults.RawAD.Iv = R0mioffs;
  }
  break;
  case 3u: {
    AdcResults.RawAD.Iu = OffsmiR1;
    AdcResults.RawAD.Iv = R1miR0;
  }
  break;
  case 4u: {
    AdcResults.RawAD.Iu = R1miR0;
    AdcResults.RawAD.Iv = OffsmiR1;
  }
  break;
  default: {
    AdcResults.RawAD.Iu = R0mioffs;
    AdcResults.RawAD.Iv = OffsmiR1;
  }
  break;
  }
  AdcResults.RawAD.Iw = -AdcResults.RawAD.Iu - AdcResults.RawAD.Iv;

  AdcResults.RawAD.IuAbs = ABS(AdcResults.RawAD.Iu);
  AdcResults.RawAD.IvAbs = ABS(AdcResults.RawAD.Iv);
  AdcResults.RawAD.IwAbs = ABS(AdcResults.RawAD.Iw);

  if (McCtrl.AlignTimeCnt == 0u)
  {
    if (AdcResults.RawAD.IuAbs > AdcResults.RawAD.IShuntPeakTmp)
    {
      AdcResults.RawAD.IShuntPeakTmp = AdcResults.RawAD.IuAbs;
    }

    if (AdcResults.RawAD.IvAbs > AdcResults.RawAD.IShuntPeakTmp)
    {
      AdcResults.RawAD.IShuntPeakTmp = AdcResults.RawAD.IvAbs;
    }

    if (AdcResults.RawAD.IwAbs > AdcResults.RawAD.IShuntPeakTmp)
    {
      AdcResults.RawAD.IShuntPeakTmp = AdcResults.RawAD.IwAbs;
    }

    if (AdcResults.RawAD.IShuntPeakTmp > AdcResults.RawAD.IShuntPeak)
    {
      AdcResults.RawAD.IShuntPeak = AdcResults.RawAD.IShuntPeakTmp;
    }
  }
  else
  {
    AdcResults.RawAD.IShuntPeakTmp =
      (AdcResults.RawAD.IuAbs >= AdcResults.RawAD.IvAbs) ? AdcResults.RawAD.IuAbs : AdcResults.RawAD.IvAbs;
    AdcResults.RawAD.IShuntPeakTmp = (AdcResults.RawAD.IwAbs >= AdcResults.RawAD.IShuntPeakTmp)
                                       ? AdcResults.RawAD.IwAbs
                                       : AdcResults.RawAD.IShuntPeakTmp;
    AdcResults.RawAD.IShuntPeak = AdcResults.RawAD.IShuntPeakTmp;
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Mc_switchToCloseLoop
* Description:
* 切换到闭环运行
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
SDK_STATIC_INLINE void Mc_switchToCloseLoop(void)
{
  McCtrl.CloseLoopAngleDelta = McCtrl.OpenLoopAngleDelta;
  /* polyspace +2 MISRA-C3:10.1 */
  McCtrl.PhasePi.IOut = McCtrl.CloseLoopAngleDelta << 15u;
  McAPI.ActSpeed = (McCtrl.CloseLoopAngleDelta << (MAT_FIX_SHIFT)) / McCtrl.SpeedtoAngleDelta;

  /* 修改参数名称 22.08.17 by jxj */
  McCtrl.IPhaseRef = McCfg.CloseLoopSwitchRefCurr;
  /* polyspace +1 MISRA-C3:10.1 */
  McCtrl.SpeedPi.IOut = (s32)McCtrl.IPhaseRef << 15;

  McAPI.State = eMCloseLoop;
}
