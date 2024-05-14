/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or disclosed
* without the written authorization from
* XiangBin Electric.
********************************************************************
* File Name:Mc_LowTask.c
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.电机控制低优先级任务
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
*
*
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#include "Common.h"
#include "DrvAdc.h"
#include "Mc.h"
#include "McCfg.h"
#include "McLib.h"

#include "MotorAPI.h"
/* Simulink model's  header file */
#include "AppMain.h"
#include "rtwtypes.h"

/* 电机转速请求延时计数，23.05.13 by yhd */
u16 StartTimeOutCnt = 0u;

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Mc_LowTask
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
void Mc_LowTask(void)
{
  s16 SpeedError = 0;
  s32 i = 0;

  switch (McAPI.State)
  {
  case eMOpenLoop: {
    if (McCtrl.AlignTimeCnt == 0u)
    {
      McAPI.ActSpeed = Mat_Ramp((McAPI.RefSpeed > 0) ? McCtrl.OpenLoopEndSpeed : (-McCtrl.OpenLoopEndSpeed),
                                McCtrl.OpenLoopAcc, &McCtrl.ActSpeedMem);
    }
    else
    {
      McCtrl.AlignTimeCnt--;

      if ((McCtrl.AlignTimeTotal - McCtrl.AlignTimeCnt) > McCfg.AlignTime1)
      {
        McSvm.Angle = McCfg.AlignAngle2;
      }

      if (McCtrl.AlignTimeCnt == 0u)
      {
        McAPI.ActSpeed = (McAPI.RefSpeed > 0) ? McCfg.OpenLoopStartSpeed : (-McCfg.OpenLoopStartSpeed);
        /* polyspace +1 MISRA-C3:10.1 */
        McCtrl.ActSpeedMem = (s32)McAPI.ActSpeed << 16u;
      }
    }

    /* polyspace +1 MISRA-C3:21.2,17.3 */
    McCtrl.OpenLoopAngleDelta = __ssat(Mat_FixMulScale(McAPI.ActSpeed, McCtrl.SpeedtoAngleDelta, 0u), MAT_FIX_SAT);

    i = (s32)(ABS(McAPI.ActSpeed)) * (s32)McSvm.MaxAmp / (s32)AdcResults.Voltage.Bat;
    McAPI.BEMFAmp = i * (s32)McCfg.BEMFFactor / MC_CFG_BEMF_FACTOR_SCALE;

    McAPI.PwmAmp = McSvm.Amp;

    StartTimeOutCnt = 0u;
  }
  break;
  case eMCloseLoop: {
    /* Limit phase peak current by voltage */
    McCtrl.SpeedPi.PiMax = Common_exeLookUpTable(AdcResults.Voltage.BatFilt, MaxRefCurrLimitTab_X, MaxRefCurrLimitTab_Y,
                                                 MC_CFG_MAX_REF_CURRENT_LIMIT_TABLE_LEN - 1);
    McCtrl.SpeedPi.IMax = McCtrl.SpeedPi.PiMax;

    SpeedError = (McAPI.RefSpeed > 0) ? (McAPI.RefSpeed - McAPI.ActSpeed) : (McAPI.ActSpeed - McAPI.RefSpeed);

    if (SpeedError > McCfg.SpeedErrorLimit)
    {
      SpeedError = McCfg.SpeedErrorLimit;
    }
    else if (SpeedError < -McCfg.SpeedErrorLimit * 3)
    {
      SpeedError = -McCfg.SpeedErrorLimit * 3;
    }
    else
    {
    }

    /* 修改参数名称 22.08.17 by jxj */
    McCtrl.IPhaseRef = Mat_ExePi(&McCtrl.SpeedPi, SpeedError);

    i = (s32)(ABS(McAPI.ActSpeed)) * (s32)McSvm.MaxAmp / (s32)AdcResults.Voltage.Bat;
    McAPI.BEMFAmp = i * (s32)McCfg.BEMFFactor / MC_CFG_BEMF_FACTOR_SCALE;

    McAPI.PwmAmp = McSvm.Amp;

    StartTimeOutCnt = 0u;
  }
  break;
  default:
    Mc_stopMotor();

    /* 自传转速低通滤波 22.08.17 by jxj */
    if (WmAPI.Valid != 0u)
    {

      McAPI.WmDetected = 1u;

      /* 对自转转速进行滤波，22.08.17 by yhd */
      if (McCtrl.WmSpeedLp.Out == 0)
      {
        /* polyspace +1 MISRA-C3:10.1 */
        McCtrl.WmSpeedLp.Out = (s32)WmAPI.Speed << 15u;
        McAPI.WmSpeedFlt = WmAPI.Speed;
      }
      else
      {
        McAPI.WmSpeedFlt = Mat_ExeLp(&McCtrl.WmSpeedLp, WmAPI.Speed);
      }
      McAPI.WmSpeed = McAPI.WmSpeedFlt;
    }
    else
    {
      if (WmAPI.TimeOutCnt > WmAPI.TimeOutCntMax)
      {
        McAPI.WmSpeed = 0;
        McAPI.WmSpeedFlt = 0;
        McCtrl.WmSpeedLp.Out = 0;
        McAPI.WmDetected = 1u;
      }
      else
      {
        /*有启动请求就延时，延时超时3s后就认为静止，按静止进行启动。23.05.13 by yhd*/
        if ((Y.TargetSpeedLimited != 0) &&
            (((MotorAPI.Faults.Reg == 0) || ((MotorAPI.Faults.Reg != 0) && (MotorAPI.RestartEn != 0)))))
        {
          if (StartTimeOutCnt < 1000)
          {
            StartTimeOutCnt++;
          }
          else
          {
            McAPI.WmSpeed = 0;
            McAPI.WmSpeedFlt = 0;
            McCtrl.WmSpeedLp.Out = 0;
            McAPI.WmDetected = 1u;
          }
        }
        else
        {
          StartTimeOutCnt = 0u;
        }
      }
    }

    McAPI.WmValid = WmAPI.Valid;

    /* 计算停止状态的时间，22.05.16 by yhd */
    if (McCtrl.StopTime < U16_MAX)
    {
      McCtrl.StopTime += ((u16)McCfg.LowTaskPeriod) / 1000u;
    }

    break;
  }
}
