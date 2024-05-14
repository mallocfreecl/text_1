/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or disclosed
* without the written authorization from
* XiangBin Electric.
********************************************************************
* File Name:MotorDiagBEMF.c
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.根据反电势判断电机是否失步
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
#include "MotorDiagBEMF.h"
#include "MotorAPI.h"

/* Defines*/
/******************************************************************/
#define MOTOR_DIAG_BEMF_MAX_DIFF (MotorAPI.BEMFAmp >> 3u)
#define MOTOR_DIAG_BEMF_FILTER_TIME_THRESHOLD (3000u) // ms

/* Variables */
/******************************************************************/
u8 IsMotorBEMFAbnormal = 0u;
static u16 MotorBEMFOutRangeTime = 0u;

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorDiagBEMF_init
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
* 1.电机每次启动前调用
********************************************************************
END_FUNCTION_HDR*/
void MotorDiagBEMF_init(void)
{
  IsMotorBEMFAbnormal = 0u;
  MotorBEMFOutRangeTime = 0u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorDiagBEMF_step
* Description:
* 1.根据反电势幅值判断电机是否失步
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
* 1.调用周期必须为MotorDiagBEMF_step
********************************************************************
END_FUNCTION_HDR*/
void MotorDiagBEMF_step(void)
{
  if (MotorAPI.BEMFAmp > (MotorAPI.PwmOutAmp + MOTOR_DIAG_BEMF_MAX_DIFF))
  {
    if (MotorBEMFOutRangeTime <= (U16_MAX - MOTOR_API_PERIOD_TASK_TIME_BASE))
    {
      MotorBEMFOutRangeTime += MOTOR_API_PERIOD_TASK_TIME_BASE;
    }
    else
    {
      MotorBEMFOutRangeTime = U16_MAX;
    }
  }
  else if (MotorAPI.BEMFAmp < MotorAPI.PwmOutAmp)
  {
    MotorBEMFOutRangeTime = 0u;
  }
  else
  {
  }

  if (MotorBEMFOutRangeTime > MOTOR_DIAG_BEMF_FILTER_TIME_THRESHOLD)
  {
    IsMotorBEMFAbnormal = 1u;
  }
}
