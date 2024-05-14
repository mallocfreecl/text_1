/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
* disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:MotorAPI.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.电机控制接口
* 2.电机堵转诊断
* 3.电机故障恢复
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
#include "MotorAPI.h"
#include "Common.h"
#include "DrvAdc.h"
#include "Mat.h"
#include "MotorDiagBEMF.h"
#include "MotorDiagCurrent.h"
#include "MotorDiagPreStart.h"
#include "MotorDiagSpeed.h"
#include "Sdk.h"

#include "AppMain.h" /* Model's header file */
#include "rtwtypes.h"

/* Defines*/
/******************************************************************/
#define MOTOR_API_POS_ROTATE_DIRECTION (-1)
#if ((MOTOR_API_POS_ROTATE_DIRECTION) != 1) && ((MOTOR_API_POS_ROTATE_DIRECTION) != -1)
#error MOTOR_API_POS_ROTATE_DIRECTION should be 1 or -1
#endif

#define MOTOR_API_REF_SPEED_MIN (150)
#define MOTOR_API_MAX_NEG_START_SPEED (300)
#define MOTOR_API_REF_ACT_SPEED_DIFF_MIN (100)

#define MOTOR_API_WM_TIME_OUT_THRESHOLD (2000)

#define MOTOR_API_LOCK_CONFIRM_TIME 10000u // unit:ms

/* Variables */
/******************************************************************/
MotorAPI_t MotorAPI;
static TMat_Lp_Simple SpeedLp = {.CoefA = 2000, .CoefB = 2000};

/* Function declaration */
/******************************************************************/
static u8 MotorOffDiag(void);
static void stopMotor(void);
static void startMotor(s16 RefSpeed);
static s16 limitRefSpeed(s16 RefSpeed);
static MotorState_t getMotorState(void);
static void CloseLoopTimer(void);

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorAPI_init
* Description:
* 初始化电机控制状态
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
void MotorAPI_init(void)
{
  Mc_init();

  MotorDiagCurrent_init();
  MotorDiagSpeed_init();
  MotorDiagBEMF_init();

  MotorAPI.State = getMotorState();
  MotorAPI.StateLast = MotorAPI.State;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorAPI_setSpeed
* Description:
* 1.设置电机的目标转速
* 2.启动电机前执行故障诊断
* 3.有故障时执行故障重启
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
* 1.在MotorAPI_step之后执行
*
********************************************************************
END_FUNCTION_HDR*/
void MotorAPI_setSpeed(s16 RefSpeed)
{
  u8 DiagDone = 0u;
  /* 启动前诊断运行中标志，22.03.30 by yhd */
  static u8 IsDiagRunning = 0u;

  RefSpeed *= (MOTOR_API_POS_ROTATE_DIRECTION);
  /* 相电压变化会导致错误的检测到自转转速，所以启动前诊断过程中不再重新限制目标转速，22.03.30 by yhd */
  if (IsDiagRunning == 0u)
  {
    RefSpeed = limitRefSpeed(RefSpeed);
  }

  if (RefSpeed == 0)
  {
    stopMotor();
  }
  else
  {
    if (MotorAPI.State == eMStop)
    {
      DiagDone = MotorOffDiag();
      IsDiagRunning = 1u;

      if (DiagDone != 0u)
      {
        IsDiagRunning = 0u;

        if (MotorAPI.Faults.Reg == 0u)
        {
          startMotor(RefSpeed);
        }
        else
        {
          if ((MotorAPI.RestartEn != 0u) && (MotorAPI.RestartComplete == 0u))
          {
            startMotor(RefSpeed);
          }
        }
      }
    }
    else
    {
      startMotor(RefSpeed);
    }
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorOffDiag
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
static u8 MotorOffDiag(void)
{
  u8 IsRestart = 0u;
  u8 DiagDone = 0u;
  /* 启动前诊断运行中标志，22.03.30 by yhd */
  static u8 IsDiagRunning = 0u;

  /* 相电压变化会导致错误的检测到自转转速，所以不再判断自转转速，22.03.30 by yhd */
  if ((MotorAPI.WmSpeed != 0) && (IsDiagRunning == 0u))
  {
    MotorAPI.Faults.bit.Short = 0u;
    MotorAPI.Faults.bit.Open = (McAPI.FaultFlag.B.WmSample != 0u);

    DiagDone = 1u;
  }
  else
  {
    IsRestart = (MotorAPI.Faults.Reg != 0) && (MotorAPI.RestartEn != 0);

    if ((MotorAPI.Faults.Reg == 0u) || (IsRestart != 0u))
    {
      DiagDone = MotorDiagPreStartState();
      IsDiagRunning = 1u;

      if (DiagDone != 0u)
      {
        IsDiagRunning = 0u;

        MotorAPI.Faults.bit.Short = (BDRVDrvSts.bit.ShortGND != 0u) || (BDRVDrvSts.bit.ShortBAT != 0u);
        if (MotorAPI.Faults.bit.Short == 0u)
        {
          MotorAPI.Faults.bit.Open =
            (BDRVDrvSts.bit.OpenLoadU != 0u) || (BDRVDrvSts.bit.OpenLoadV != 0u) || (BDRVDrvSts.bit.OpenLoadW != 0u);
        }

        if ((IsRestart != 0u) && ((MotorAPI.Faults.bit.Short != 0u) || (MotorAPI.Faults.bit.Open != 0u)))
        {
          MotorAPI.RestartComplete = 1u;
        }
      }
    }
    else
    {
    }
  }

  return DiagDone;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorAPI_step
* Description:
* 1.更新转速、电流
* 2.执行故障监测流程
* 3.执行故障恢复流程
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
void MotorAPI_step(void)
{
  BdrvSts_t DrvSts = {
    0,
  };

  MotorAPI.State = getMotorState();

  MotorAPI.RefSpeed = McAPI.RefSpeed * (MOTOR_API_POS_ROTATE_DIRECTION);
  MotorAPI.ActSpeed = McAPI.ActSpeed * (MOTOR_API_POS_ROTATE_DIRECTION);
  if (MotorAPI.ActSpeed != 0)
  {
    MotorAPI.ActSpeedFilted = Mat_ExeLp_without_min_max(&SpeedLp, MotorAPI.ActSpeed);
  }
  else
  {
    MotorAPI.ActSpeedFilted = 0;
    SpeedLp.Out = 0;
  }
  MotorAPI.WmSpeed = McAPI.WmSpeed * (MOTOR_API_POS_ROTATE_DIRECTION);

  MotorAPI.BEMFAmp = McAPI.BEMFAmp;
  MotorAPI.PwmOutAmp = McAPI.PwmAmp;

  MotorAPI.IShunt = AdcResults.RawAD.IShunt;
  MotorAPI.IShuntPeak = AdcResults.RawAD.IShuntPeak;

  if (((MotorAPI.State == eMOpenLoop) || (MotorAPI.State == eMCloseLoop)) && (MotorAPI.StateLast == eMStop))
  {
    MotorDiagCurrent_init();
    MotorDiagSpeed_init();
    MotorDiagBEMF_init();

    // MotorAPI.DetailFaults.Reg = 0u;
  }

  CloseLoopTimer();

  if ((MotorAPI.State == eMOpenLoop) || (MotorAPI.State == eMCloseLoop))
  {
    (void)Sdk_diagBdrvOnShort(&DrvSts);
    MotorDiagCurrent_step();
    MotorDiagSpeed_step();
    MotorDiagBEMF_step();

    MotorAPI.Faults.bit.Short = (DrvSts.bit.ShortGND != 0u) || (DrvSts.bit.ShortBAT != 0u);

    if ((McAPI.FaultFlag.B.OverCurrent != 0u) || (MotorAPI.Faults.bit.Short != 0u) || (IsMotorOverCurrent != 0u) ||
        (IsMotorSpeedAbnormal != 0u) || (IsMotorBEMFAbnormal != 0u))
    {
      MotorAPI.DetailFaults.bit.OnShortBAT = DrvSts.bit.ShortBAT;
      MotorAPI.DetailFaults.bit.OnShortGND = DrvSts.bit.ShortGND;
      MotorAPI.DetailFaults.bit.OverCurrent = IsMotorOverCurrent;
      MotorAPI.DetailFaults.bit.SpeedOutRange = IsMotorSpeedAbnormal;
      MotorAPI.DetailFaults.bit.BEMFAmpOutRange = IsMotorBEMFAbnormal;

      stopMotor();

      if (MotorAPI.Faults.bit.Short == 0u)
      {
        MotorAPI.Faults.bit.Lock = 1u;
      }

      if (MotorAPI.RestartEn != 0u)
      {
        MotorAPI.RestartComplete = 1u;
      }
    }
    else
    {
      if ((MotorAPI.RestartEn != 0u) && (MotorAPI.Faults.bit.Lock != 0u) &&
          (MotorAPI.CloseLoopTime > MOTOR_API_LOCK_CONFIRM_TIME))
      {
        MotorAPI.Faults.bit.Lock = 0u;
        MotorAPI.RestartComplete = 1u;
      }
    }
  }

  MotorAPI.StateLast = MotorAPI.State;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:stopMotor
* Description:
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
static void stopMotor(void)
{
  Mc_stopMotor();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:startMotor
* Description:
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
static void startMotor(s16 RefSpeed)
{
  Mc_startMotor(RefSpeed);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:limitRefSpeed
* Description:
* 1.在自转转速检测未完成，并且自转检测未超时时不启动电机
* 2.如果目标转速和自转转速相反，当自转转速大于MOTOR_API_MAX_NEG_START_SPEED
*   时不启动
* 3.除1、2情况外参考转速不变
*
* Inputs:
* 1.WmAPI.Valid：0：自转检测结果无效，1：自转检测结果有效
* 2.WmAPI.TimeOutCnt：自转检测超时计数
* 3.McAPI.WmSpeed：自转转速，单位：rpm
*
*
* Outputs:
* 1.RefSpeed：参考转速
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static s16 limitRefSpeed(s16 RefSpeed)
{
  /* RefSpeed limit by motor min speed */
  if (ABS(RefSpeed) < MOTOR_API_REF_SPEED_MIN)
  {
    RefSpeed = 0;
  }

  /* RefSpeed limit by windmilling speed */
  if ((McAPI.WmValid == 0u) && (McAPI.WmDetected == 0u))
  {
    RefSpeed = 0;
  }
  else
  {
    if ((((s32)RefSpeed * (s32)McAPI.WmSpeed) < 0) && (ABS(McAPI.WmSpeed) >= MOTOR_API_MAX_NEG_START_SPEED))
    {
      /* RefSPeed limit by max reverse speed */
      RefSpeed = 0;
    }
  }

  if (McAPI.WmDetected == 0u)
  {
    RefSpeed = 0;
  }

  if ((McAPI.State == eMStop) && (ABS(RefSpeed) + MOTOR_API_REF_ACT_SPEED_DIFF_MIN) < ABS(McAPI.WmSpeed))
  {
    RefSpeed = 0;
  }

  return RefSpeed;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:getMotorState
* Description:
* 1.获取电机当前运行阶段
*
* Inputs:
*
*
* Outputs:
* 1.State电机运行状态
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static MotorState_t getMotorState(void)
{
  return McAPI.State;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:CloseLoopTimer
* Description:
* 记录闭环持续时间
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
* 1.调用周期必须为MOTOR_API_PERIOD_TASK_TIME_BASE
********************************************************************
END_FUNCTION_HDR*/
static void CloseLoopTimer(void)
{
  if (MotorAPI.State == eMCloseLoop)
  {
    if (MotorAPI.CloseLoopTime <= (U32_MAX - MOTOR_API_PERIOD_TASK_TIME_BASE))
    {
      MotorAPI.CloseLoopTime += MOTOR_API_PERIOD_TASK_TIME_BASE;
    }
    else
    {
      MotorAPI.CloseLoopTime = U32_MAX;
    }
  }
  else
  {
    MotorAPI.CloseLoopTime = 0u;
  }
}
