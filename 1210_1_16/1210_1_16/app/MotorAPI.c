/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:MotorAPI.c
********************************************************************
* Project/Product:LXT9818_PMP
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
* Version       Date         	  Author         Descriptions
* ----------    --------------  ------------   ----------------------------------------
*     1.0       2021-07-23      yhd            Original
*     1.0       2021-07-29      zyg            1.OPEN_DIAG_CURRENT_SETTLE_TIME_US ->1400us
*     1.0       2021-07-31      zyg            1.SDK_EEPROM_Read(): Define variables at the beginning of the function
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#include "MotorAPI.h"
#include "Common.h"
#include "Emo_RAM.h"
#include "SDK.h"
#include "tle_device.h"
#include "ADCSample.h"

#include "AppMain.h"                   /* Model's header file */
#include "rtwtypes.h"

/* Defines*/
/******************************************************************/
#define MOTOR_POS_DIR (1) /* Motor postive rotation direction,1 or -1 */

#define MOTOR_PERIOD_TASK_TIME_BASE 10u/* ms */

#define BDRV_SHORT_GND 1u
#define BDRV_SHORT_BAT 2u

#define MOTOR_SHORT (1u<<0u)
#define MOTOR_OPEN_LOAD (1u<<1u)

#define MOTOR_CLOSE_LOOP_SPEED_MAX 8000
#define MOTOR_CLOSE_LOOP_SPEED_MIN  600
#define MOTOR_CLOSE_LOOP_DRY_RUNNING_TEST_SPEED_MIN 2500
/*堵转诊断时间减小，解决上版堵转误诊断问题 23.1.28 by lxs*/
#define MOTOR_SPEED_OUT_RANGE_FILTER_TIME 3000u/* unit:ms */

#define MOTOR_OVER_CURRENT_THR 200 /* unit:0.1A */
#define MOTOR_OVER_CURRENT_FILTER_TIME 50u/* uint:ms */

#define MOTOR_DRY_TUNNING_TEST_SPEED  4000 /* uint:rpm */
//#define MOTOR_DRY_TUNNING_TEST_TIME 120000u

#define MOTOR_DRY_RUNNING_POWER_MAX 22 /* unit:1W */
#define MOTOR_DRY_RUNNING_CURRENT_MAX 10 /* unit:0.1A, */
/*第一次检测到干运行诊断时间为30s，重启干运行诊断时间为35s 22.11.25 by lxs*/
#define MOTOR_DRY_RUNNING_CURRENT_LOW_FILTER_TIME (30*1000u)/* uint:ms */
#define MOTOR_DRY_RUNNING_CURRENT_RESTART_FILTER_TIME (35*1000u)/* uint:ms */
#define MOTOR_DRY_RUNNING_CURRENT_HIGH_FILTER_TIME 4000u/* uint:ms */

#define MOTOR_LOCK_CONFIRM_TIME 10000u /* unit:ms */
#define MOTOR_SHORT_CONFIRM_TIME 3000u /* unit:ms */
/*清除干运行故障时间 22.11.25 by lxs*/
#define MOTOR_STOP_CLEAR_FAULT_TIME 4000u

#define MOTOR_OPEN_LOAD_DAIG_CURRENT_THR 16u /* AD */


/* Variables */
/******************************************************************/
MotorAPIVar_t MotorAPIVar;
/*消除polyspace警告,2021-09-22-by zyg*/
static TMat_Lp_Simple SpeedLp ={.CoefA = 1000,.CoefB = 1000};
volatile s32 DryRunningCurrent = 0;
s32 DryRunningCurrentTHR;

u16 MotorAPI_FeedBackCnt;
u16 MotorAPI_FeedBackClearComplete;
BDRVOffDiagResult_t BDRV_OffDiagResult;
MotorSpeedOutRangeReason_t SpeedOutRangeReason;
CSAZeroFault_t CSAZeroFault;
/* Function declaration */
/******************************************************************/
static void MotorAPI_MotorOnShortMonitor(void);
static void MotorAPI_MotorDryRunningMonitor(void);
static void MotorAPI_OverCurrentMonitor(void);
static void MotorAPI_MotorSpeedMonitor(void);

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorAPI_MotorInit
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
void MotorAPI_MotorInit(void)
{
  Emo_Init();

  MotorAPIVar.MotorOffDiagEn = 0u;
  MotorAPIVar.MotorOffDiagCompleted = 0u;

  MotorAPIVar.IsDryRunningDetected = 0u;
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorAPI_MotorSetSpeed
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
void MotorAPI_MotorSetSpeed(s16 Speed)
{
/*期望转速小于4000持续4s退出干运行 22.11.25 by lxs*/
	if(ABS(Speed) < MOTOR_DRY_TUNNING_TEST_SPEED)
	{
		if(MotorAPIVar.StopTime <= 0xFFFFu - (MOTOR_PERIOD_TASK_TIME_BASE))
    {
      MotorAPIVar.StopTime += (MOTOR_PERIOD_TASK_TIME_BASE);
    }
    else
    {
      MotorAPIVar.StopTime = 0xFFFFu;
    }
		
		if(MotorAPIVar.StopTime >= MOTOR_STOP_CLEAR_FAULT_TIME)
		{
			MotorAPIVar.DryRunningRestartFlag = 0u;
		}
		else
    {
			
		}
	}
	else
	{
		MotorAPIVar.StopTime = 0u;
	}
  if(ABS(Speed) < Emo_Foc.StartEndSpeed)
  {
    Speed = 0;
  }
  else
  {

  }
  Speed *= (MOTOR_POS_DIR);
	/*期望转速大于4000则进入干运行诊断 22.11.25 by lxs*/
	if(ABS(Speed) >= MOTOR_DRY_TUNNING_TEST_SPEED)
	{
		MotorAPIVar.IsDryRunningDetected = 1u;
	}
	else
	{
		MotorAPIVar.IsDryRunningDetected = 0u;
		if((MotorAPIVar.StopTime >= MOTOR_STOP_CLEAR_FAULT_TIME)&&(Y.DryRunningCnt < 20))
		{
			MotorAPIVar.IsDryRunning = 0u;
		}
		else
		{
			
		}
	}

  if(Speed == 0)
  {
    if(Emo_Ctrl.RefSpeed != 0)
    {
      Emo_Ctrl.RefSpeed = 0;
      Emo_StopMotor();
    }
    else
    {

    }

    return ;
  }
  else
  {

    if(Emo_Status.MotorState == (EMO_MOTOR_STATE_STOP))
    {
      if((MotorAPIVar.Faults.Reg == 0u)
      || ((MotorAPIVar.Faults.Reg != 0u) && (MotorAPIVar.RestartEn != 0u)))
      {
        MotorAPIVar.DetailFaults.Reg = 0u;
      }
      else
      {

      }

      if(MotorAPIVar.Faults.Reg == 0u)
      {
        Emo_Ctrl.RefSpeed = Speed;
        Emo_StartMotor();
      }
      else
      {
        if((MotorAPIVar.RestartEn != 0u) && (MotorAPIVar.RestartComplete == 0u))
        {
          Emo_Ctrl.RefSpeed = Speed;
          Emo_StartMotor();
        }
        else
        {

        }
      }
    }
    else
    {
      Emo_Ctrl.RefSpeed = Speed;
    }
  }
}


/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorAPI_MotorPeriodTask
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
void MotorAPI_MotorPeriodTask(void)
{
  static u8 MotorStateLast = (EMO_MOTOR_STATE_STOP);

  if((   (Emo_Status.MotorState == (EMO_MOTOR_STATE_START))
      || (Emo_Status.MotorState == (EMO_MOTOR_STATE_RUN)))
  && (MotorStateLast == (EMO_MOTOR_STATE_STOP)))
  {
    MotorAPIVar.OverCurrentTime = 0u;
    MotorAPIVar.SpeedOutRangeTime = 0u;
		MotorAPIVar.MotorOffDiagCompleted = 0u;
		MotorAPIVar.CloseLoopTime = 0u;	
    MotorAPIVar.DryRunningCurrentLowTime = 0u;
		MotorAPIVar.DryRunningCurrentHighTime = 0u;
  }
  else
  {

  }
	
  if((Emo_Status.MotorState == (EMO_MOTOR_STATE_START))
  || (Emo_Status.MotorState == (EMO_MOTOR_STATE_RUN)))
  {
    MotorAPI_MotorOnShortMonitor();
    MotorAPI_OverCurrentMonitor();

    if(Emo_Status.MotorState == (EMO_MOTOR_STATE_RUN))
    {
			if(MotorAPIVar.CloseLoopTime <= (0xFFFFFFFFu - (MOTOR_PERIOD_TASK_TIME_BASE)))
			{
				MotorAPIVar.CloseLoopTime += (MOTOR_PERIOD_TASK_TIME_BASE);
			}
			else
			{
				MotorAPIVar.CloseLoopTime = 0xFFFFFFFFu;
			}  
			
      MotorAPI_MotorSpeedMonitor();
      MotorAPI_MotorDryRunningMonitor();
    }
    else
    {

    }

   /*增加诊断开环闭环短路条件 24.02.23 by WChuang*/
    if((MotorAPIVar.DetailFaults.bit.MotorOnShortGNDOL != 0u)
    || (MotorAPIVar.DetailFaults.bit.MotorOnShortGNDCL != 0u)
		|| (MotorAPIVar.DetailFaults.bit.MotorOnShortBATOL != 0u)
		|| (MotorAPIVar.DetailFaults.bit.MotorOnShortBATCL != 0u)
		|| (MotorAPIVar.DetailFaults.bit.MotorOnOverCurrent != 0u)
		|| (MotorAPIVar.DetailFaults.bit.MotorOnSpeedOutRange != 0u)
		)
    {
			MotorAPIVar.DetailFaults1 = MotorAPIVar.DetailFaults;
			
      Emo_StopMotor();


			if((MotorAPIVar.DetailFaults.bit.MotorOnShortGNDOL != 0u)
			|| (MotorAPIVar.DetailFaults.bit.MotorOnShortGNDCL != 0u)
			|| (MotorAPIVar.DetailFaults.bit.MotorOnShortBATOL != 0u)
			|| (MotorAPIVar.DetailFaults.bit.MotorOnShortBATCL != 0u))
			{
				MotorAPIVar.Faults.bit.Short = 1u;
			}
			else 
      {
          MotorAPIVar.Faults.bit.Lock = 1u;
      }
			
      if(MotorAPIVar.RestartEn != 0u)
      {
        MotorAPIVar.RestartComplete = 1u;
      }
    }
		else
		{
			
		}

    if((MotorAPIVar.RestartEn != 0)
    && (MotorAPIVar.DetailFaults.Reg == 0u))
    {
      if((MotorAPIVar.Faults.bit.Lock != 0u)&&(MotorAPIVar.CloseLoopTime > (MOTOR_LOCK_CONFIRM_TIME)))
      {
        MotorAPIVar.Faults.bit.Lock = 0u;
        MotorAPIVar.RestartComplete = 1u;
      }
			/*短路故障恢复时间修改为3s 24.2.25 by zlh*/
      if((MotorAPIVar.Faults.bit.Short != 0u)&&(MotorAPIVar.CloseLoopTime > (MOTOR_SHORT_CONFIRM_TIME)))
      {
        MotorAPIVar.Faults.bit.Short = 0;
        MotorAPIVar.RestartComplete = 1u;
      }

    }
		else
		{
			
		}
  }
	else
	{
		SpeedLp.Out = 0;
	}

  MotorStateLast = Emo_Status.MotorState;
	
	MotorAPIVar.ActSpeed = Mat_ExeLp_without_min_max(&SpeedLp, Emo_Ctrl.ActSpeed * (MOTOR_POS_DIR));
	
	MotorAPIVar.RefSpeed = Emo_Ctrl.RefSpeed * (MOTOR_POS_DIR);
  MotorAPIVar.IShunt = Emo_Ctrl.IShunt;
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorAPI_MotorOnShortMonitor
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
static void MotorAPI_MotorOnShortMonitor(void)
{
  u8 ret;
  ret = SDK_BDRVGetShortStatus();
  if(ret != 0u)
  {  /*诊断开环闭环短路细化 23.08.07 by zjj*/
    if((ret & (BDRV_SHORT_GND)) != 0u)
    {
			if(Emo_Status.MotorState == EMO_MOTOR_STATE_RUN)
			{
				MotorAPIVar.DetailFaults.bit.MotorOnShortGNDCL = 1u;
			}
			else
			{
				MotorAPIVar.DetailFaults.bit.MotorOnShortGNDOL = 1u;
			}
    }
    else
    {

    }

    if((ret & (BDRV_SHORT_BAT)) != 0u)
    {
			if(Emo_Status.MotorState == EMO_MOTOR_STATE_RUN)
			{
				MotorAPIVar.DetailFaults.bit.MotorOnShortBATCL = 1u;
			}
			else
			{
				MotorAPIVar.DetailFaults.bit.MotorOnShortBATOL = 1u;
			}
    }
    else
    {

    }
  }
  else
  {

  }
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorAPI_MotorDryRunningMonitor
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
static void MotorAPI_MotorDryRunningMonitor(void)
{
	DryRunningCurrent = ABS(Emo_Ctrl.IShunt);

  if(MotorAPIVar.IsDryRunningDetected == 0u)/* 2021-07-29-by zyg  */
  {
    MotorAPIVar.DryRunningCurrentHighTime = 0u;
	MotorAPIVar.DryRunningCurrentLowTime = 0u;
  }
  else
  {
		s32 Current;
	
		DryRunningCurrentTHR = (MOTOR_DRY_RUNNING_POWER_MAX)*100/ADCResults.Voltage.Bat;
		Current = DryRunningCurrent;

		if(Current < DryRunningCurrentTHR)
		{
			MotorAPIVar.DryRunningCurrentHighTime = 0u;
			if(MotorAPIVar.DryRunningCurrentLowTime <= 0xFFFFFFFFu - (MOTOR_PERIOD_TASK_TIME_BASE))
			{
				MotorAPIVar.DryRunningCurrentLowTime += (MOTOR_PERIOD_TASK_TIME_BASE);
			}
			else
			{
				MotorAPIVar.DryRunningCurrentLowTime = 0xFFFFFFFFu;
			}
		}
		else
		{
			MotorAPIVar.DryRunningCurrentLowTime = 0u;
			if(MotorAPIVar.DryRunningCurrentHighTime <= 0xFFFFFFFFu - (MOTOR_PERIOD_TASK_TIME_BASE))
			{
				MotorAPIVar.DryRunningCurrentHighTime += (MOTOR_PERIOD_TASK_TIME_BASE);
			}
			else
			{
				MotorAPIVar.DryRunningCurrentHighTime = 0xFFFFFFFFu;
			}
		}

		if((MotorAPIVar.DryRunningCurrentLowTime >= (MOTOR_DRY_RUNNING_CURRENT_LOW_FILTER_TIME)) && (MotorAPIVar.DryRunningRestartFlag == 0u))
		{
			MotorAPIVar.IsDryRunning = 1u;
			MotorAPIVar.DryRunningCurrentLowTime = 0u;
			MotorAPIVar.DryRunningRestartFlag = 1u;
			MotorAPIVar.RestartComplete = 1u;
		}
		else
		{

		}
		
		if((MotorAPIVar.DryRunningRestartFlag !=0u)&&(MotorAPIVar.DryRunningCurrentLowTime >= (MOTOR_DRY_RUNNING_CURRENT_RESTART_FILTER_TIME)))
		{
			MotorAPIVar.IsDryRunning = 1u;
			MotorAPIVar.DryRunningCurrentLowTime = 0u;
			MotorAPIVar.RestartComplete = 1u;
		  if(MotorAPIVar.RestartEn != 0u)
		  {
			  MotorAPIVar.RestartComplete = 1u;
	  	}
		}
		else
    {
			
		}

		if(MotorAPIVar.DryRunningCurrentHighTime > (MOTOR_DRY_RUNNING_CURRENT_HIGH_FILTER_TIME))
		{
			MotorAPIVar.IsDryRunning = 0u;
			MotorAPIVar.DryRunningRestartFlag = 0u;
		}
		else
		{

		}
  }

 
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorAPI_OverCurrentMonitor
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
static void MotorAPI_OverCurrentMonitor(void)
{
  u16 Current;

  Current = ABS(Emo_Ctrl.IShunt);

  if(Current > (MOTOR_OVER_CURRENT_THR))
  {
    if(MotorAPIVar.OverCurrentTime <= 0xFFFFu - (MOTOR_PERIOD_TASK_TIME_BASE))
    {
      MotorAPIVar.OverCurrentTime += (MOTOR_PERIOD_TASK_TIME_BASE);
    }
    else
    {
      MotorAPIVar.OverCurrentTime = 0xFFFFu;
    }
  }
  else
  {
      MotorAPIVar.OverCurrentTime = 0u;
  }

  if(MotorAPIVar.OverCurrentTime > (MOTOR_OVER_CURRENT_FILTER_TIME))
  {
    MotorAPIVar.DetailFaults.bit.MotorOnOverCurrent = 1u;
  }
  else
  {

  }
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorAPI_MotorSpeedMonitor
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
static void MotorAPI_MotorSpeedMonitor(void)
{
  s16 ActSpeed;
  s16 RefSpeed;
  s16 ActSpeedAbs;
  s16 RefSpeedAbs;
  s16 AbsDiffSpeed;
  s16 AbsDiffPowerOutput;
	
  ActSpeed = Emo_Ctrl.ActSpeed;
  RefSpeed = Emo_Ctrl.RefSpeed;
  ActSpeedAbs = ABS(ActSpeed);
  RefSpeedAbs = ABS(RefSpeed);
  AbsDiffSpeed = ABS(RefSpeedAbs - ActSpeedAbs);
  AbsDiffPowerOutput = ABS(Emo_Ctrl.ImagCurrPi.IMax - Emo_Foc.RotVolt.Imag);
	
  u16 Current;
  Current = ABS(Emo_Ctrl.IShunt);
	/*消除polyspace警告,2021-09-27-by zyg*/
  if((ActSpeedAbs > (MOTOR_CLOSE_LOOP_SPEED_MAX)) 
	|| (ActSpeedAbs < (MOTOR_CLOSE_LOOP_SPEED_MIN))
	|| (((MotorAPIVar.IsDryRunningDetected != 0u) || (MotorAPIVar.IsDryRunning != 0u)) && (ActSpeedAbs < (MOTOR_CLOSE_LOOP_DRY_RUNNING_TEST_SPEED_MIN)))
  || (((ActSpeed >0) && (RefSpeed <0))|| ((ActSpeed < 0) && (RefSpeed > 0)))
  || ((AbsDiffSpeed > 500u) && (Current < 50u) && (AbsDiffPowerOutput > 100u))) /*在功率接近满输出时不诊断该堵转判断 2023.04.07 by zjj*/
  {
	/*堵转故障条件划分标志置位 23.05.12 by zjj*/  
    if(ActSpeedAbs > (MOTOR_CLOSE_LOOP_SPEED_MAX))
		{
			SpeedOutRangeReason.bit.MotorOnSpeedOutRange_OverSpeed = 1u;
		}
    if(ActSpeedAbs < (MOTOR_CLOSE_LOOP_SPEED_MIN))
		{
			SpeedOutRangeReason.bit.MotorOnSpeedOutRange_UnderSpeed = 1u;
		}	
    if(((MotorAPIVar.IsDryRunningDetected != 0u) || (MotorAPIVar.IsDryRunning != 0u)) && (ActSpeedAbs < (MOTOR_CLOSE_LOOP_DRY_RUNNING_TEST_SPEED_MIN)))
		{
			SpeedOutRangeReason.bit.MotorOnSpeedOutRange_DryRunning = 1u;
		}
    if(((ActSpeed >0) && (RefSpeed <0))|| ((ActSpeed < 0) && (RefSpeed > 0)))
		{
			SpeedOutRangeReason.bit.MotorOnSpeedOutRange_ReverseSpeed = 1u;
		}
    if((AbsDiffSpeed > 500u) && (Current < 50u) && (AbsDiffPowerOutput > 100u))
		{
			SpeedOutRangeReason.bit.MotorOnSpeedOutRange_DiffSpeedAndCurrent = 1u;
		}
    if(MotorAPIVar.SpeedOutRangeTime <= 0xFFFFu - (MOTOR_PERIOD_TASK_TIME_BASE))
    {
      MotorAPIVar.SpeedOutRangeTime += (MOTOR_PERIOD_TASK_TIME_BASE);
    }
    else
    {
      MotorAPIVar.SpeedOutRangeTime = 0xFFFFu;
    }
  }
  else
  {
    if(MotorAPIVar.SpeedOutRangeTime >= (MOTOR_PERIOD_TASK_TIME_BASE))
    {
      MotorAPIVar.SpeedOutRangeTime -= (MOTOR_PERIOD_TASK_TIME_BASE);
    }
    else
    {
      MotorAPIVar.SpeedOutRangeTime = 0u;
    }
  }
  if(MotorAPIVar.SpeedOutRangeTime > (MOTOR_SPEED_OUT_RANGE_FILTER_TIME))
  {
    MotorAPIVar.DetailFaults.bit.MotorOnSpeedOutRange = 1u;
	
	/*存储堵转故障划分 23.05.12 by zjj*/		
	MotorAPIVar.DetailFaults.bit.MotorOnSpeedOutRange_OverSpeed = SpeedOutRangeReason.bit.MotorOnSpeedOutRange_OverSpeed;
	MotorAPIVar.DetailFaults.bit.MotorOnSpeedOutRange_UnderSpeed = SpeedOutRangeReason.bit.MotorOnSpeedOutRange_UnderSpeed;
	MotorAPIVar.DetailFaults.bit.MotorOnSpeedOutRange_DryRunning = SpeedOutRangeReason.bit.MotorOnSpeedOutRange_DryRunning;
	MotorAPIVar.DetailFaults.bit.MotorOnSpeedOutRange_ReverseSpeed = SpeedOutRangeReason.bit.MotorOnSpeedOutRange_ReverseSpeed;
	MotorAPIVar.DetailFaults.bit.MotorOnSpeedOutRange_DiffSpeedAndCurrent = SpeedOutRangeReason.bit.MotorOnSpeedOutRange_DiffSpeedAndCurrent;
	SpeedOutRangeReason.Reg = 0u;
		
  }
}


