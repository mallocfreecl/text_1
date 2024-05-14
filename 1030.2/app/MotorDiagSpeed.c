/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or disclosed
* without the written authorization from
* XiangBin Electric.
********************************************************************
* File Name:MotorDiagSpeed.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.根据实际转速判断电机是否失步
*
* (Requirements, pseudo code and etc.)
*********************************************************************
* Limitations:
*
*
* (limitations)
*********************************************************************
*********************************************************************
* Revision History：
*
* Version       Date         	  Author         Descriptions
* ----------    --------------  ------------   ---------------------
*     1.0       2021-12-10      yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#include "MotorDiagSpeed.h"
#include "Common.h"
#include "MotorAPI.h"

/* Includes*/
/******************************************************************/
#define MOTOR_DIAG_SPEED_MIN (400)  // rpm
#define MOTOR_DIAG_SPEED_MAX (4500) // rpm

#define MOTOR_DIAG_SPEED_ACC_SAMPLE_TIME_THRESHOLD (500u) // ms

#define MOTOR_DIAG_SPEED_OUT_RANGE_FILTER_TIME (5000u) // ms

/* Variables */
/******************************************************************/
u8 IsMotorSpeedAbnormal = 0u;
MotorSpeedAbnormalFlag_t MotorSpeedAbnormalFlag;

static u16 MotorSpeedAbnormalTime = 0u;
static u16 MotorSpeedAccSampleTime = 0u;
static s16 MotorSpeedAcc = 0;
static s16 MotorSpeedLast = 0;

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorDiagSpeed_init
* Description:
* 1.初始化速度诊断变量
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
* 1.每次启动前调用
********************************************************************
END_FUNCTION_HDR*/
void MotorDiagSpeed_init(void)
{
  IsMotorSpeedAbnormal = 0u;
  MotorSpeedAbnormalFlag.reg = 0u;
  MotorSpeedAbnormalTime = 0u;
  MotorSpeedAccSampleTime = 0u;
  MotorSpeedAcc = 0;
  MotorSpeedLast = MotorAPI.WmSpeed;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorDiagSpeed_step
* Description:
* 1.判断实际转速是否正常
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
* 1.调用周期必须为MOTOR_API_PERIOD_TASK_TIME_BASE
*
********************************************************************
END_FUNCTION_HDR*/
void MotorDiagSpeed_step(void)
{
  s16 ActSpeed;
  s16 RefSpeed;
  s16 ActSpeedAbs;

  ActSpeed = MotorAPI.ActSpeed;
  RefSpeed = MotorAPI.RefSpeed;
  ActSpeedAbs = ABS(ActSpeed);

  if (MotorAPI.State == eMCloseLoop)
  {
    MotorSpeedAbnormalFlag.bits.SpeedOutRange =
      (ActSpeedAbs < MOTOR_DIAG_SPEED_MIN) || (ActSpeedAbs > MOTOR_DIAG_SPEED_MAX);

    MotorSpeedAbnormalFlag.bits.SpeedReverse = (((s32)ActSpeed * (s32)RefSpeed) < 0);

    if (MotorSpeedAccSampleTime < MOTOR_DIAG_SPEED_ACC_SAMPLE_TIME_THRESHOLD)
    {
      MotorSpeedAccSampleTime += MOTOR_API_PERIOD_TASK_TIME_BASE;
    }
    else
    {
      MotorSpeedAccSampleTime = 0u;

      MotorSpeedAcc = ActSpeedAbs - MotorSpeedLast;
      MotorSpeedLast = ActSpeedAbs;
    }

    if (MotorSpeedAbnormalFlag.reg != 0u)
    {
      if (MotorSpeedAbnormalTime <= (U16_MAX - MOTOR_API_PERIOD_TASK_TIME_BASE))
      {
        MotorSpeedAbnormalTime += MOTOR_API_PERIOD_TASK_TIME_BASE;
      }
      else
      {
        MotorSpeedAbnormalTime = U16_MAX;
      }
    }
    else
    {
      if (MotorSpeedAbnormalTime >= MOTOR_API_PERIOD_TASK_TIME_BASE)
      {
        MotorSpeedAbnormalTime -= MOTOR_API_PERIOD_TASK_TIME_BASE;
      }
      else
      {
        MotorSpeedAbnormalTime = 0u;
      }
    }

    if (((MotorSpeedAcc > 400) && (ActSpeedAbs > 1000)) || (MotorSpeedAcc < -800))
    {
      // IsMotorSpeedAbnormal = 1u;
    }

    if (((MotorSpeedAbnormalFlag.bits.SpeedOutRange != 0u) || (MotorSpeedAbnormalFlag.bits.SpeedReverse != 0u)) &&
        (MotorSpeedAbnormalTime > (MOTOR_DIAG_SPEED_OUT_RANGE_FILTER_TIME)))
    {
      IsMotorSpeedAbnormal = 1u;
    }
  }
  else
  {
    MotorSpeedLast = ActSpeedAbs;
  }
}
