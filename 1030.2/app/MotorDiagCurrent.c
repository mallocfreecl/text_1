/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or disclosed
* without the written authorization from
* XiangBin Electric.
********************************************************************
* File Name:MotorDiagCurrent.c
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.检测电机运行时电流是否超过设定阈值
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
* ----------    --------------  ------------   ---------------------
*     1.0       2021-12-10      yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#include "MotorDiagCurrent.h"
#include "MotorAPI.h"

/* Defines*/
/******************************************************************/
/* Phase Curent Limit 47A，unit:AD,ICoef=0.0610352 */
#define MOTOR_DIAG_CURRENT_THR (1000)
#define MOTOR_DIAG_CURRENT_FILTER_TIME 500u // uint:ms

/* Variables */
/******************************************************************/
u8 IsMotorOverCurrent = 0u;
static u16 OverCurrentTime = 0u;

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorDiagCurrent_init
* Description:
* 初始化电流检测变量
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
* 1.每次电机启动前调用
********************************************************************
END_FUNCTION_HDR*/
void MotorDiagCurrent_init(void)
{
  IsMotorOverCurrent = 0u;
  OverCurrentTime = 0;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorDiagCurrent_step
* Description:
* 电流超过设定阈值一段时间后设置过流标志
*
* Inputs:
* 1.MotorAPI.IShuntPeak:电机相电流峰值AD值
*
* Outputs:
* 1.IsMotorOverCurrent：0：正常，1：过流
* 2.OverCurrentTime：过流时间
*
* Limitations:
* 1.调用时间必须为MOTOR_API_PERIOD_TASK_TIME_BASE
* 2.在电机运行时调用
*
********************************************************************
END_FUNCTION_HDR*/
void MotorDiagCurrent_step(void)
{
  u16 Current = 0u;

  Current = MotorAPI.IShuntPeak;

  if (Current > MOTOR_DIAG_CURRENT_THR)
  {
    if (OverCurrentTime <= (U16_MAX - MOTOR_API_PERIOD_TASK_TIME_BASE))
    {
      OverCurrentTime += MOTOR_API_PERIOD_TASK_TIME_BASE;
    }
    else
    {
      OverCurrentTime = U16_MAX;
    }
  }
  else
  {
    if (OverCurrentTime >= MOTOR_API_PERIOD_TASK_TIME_BASE)
    {
      OverCurrentTime -= MOTOR_API_PERIOD_TASK_TIME_BASE;
    }
    else
    {
      OverCurrentTime = 0u;
    }
  }

  if (OverCurrentTime > MOTOR_DIAG_CURRENT_FILTER_TIME)
  {
    IsMotorOverCurrent = 1u;
  }
}
