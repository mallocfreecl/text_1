/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:MotorAPI.c
********************************************************************
* Project/Product:LXT9820_PMP
* Title:
* Author:zyg
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
* ----------    --------------  ------------   --------------------
* 1.0           2021-09-06      zyg            Original
* 
*
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
#include "LINDataPro.h"

/* Defines*/
/******************************************************************/
#define MOTOR_POS_DIR (1) /* Motor postive rotation direction,1 or -1 */

#define MOTOR_PERIOD_TASK_TIME_BASE 10u/* ms */

#define BDRV_SHORT_GND 1u
#define BDRV_SHORT_BAT 2u

#define MOTOR_SHORT (1u<<0u)
#define MOTOR_OPEN_LOAD (1u<<1u)

#define MOTOR_CLOSE_LOOP_SPEED_MAX 7000
/*进入闭环最小速度原为600，蔚来要求改为500 23.03.26 by lxs*/
#define MOTOR_CLOSE_LOOP_SPEED_MIN  500
#define MOTOR_CLOSE_LOOP_DRY_RUNNING_TEST_SPEED_MIN 2500
#define MOTOR_SPEED_OUT_RANGE_FILTER_TIME 3000u/* unit:ms */

#define MOTOR_OVER_CURRENT_THR 200 /* unit:0.1A */
#define MOTOR_OVER_CURRENT_FILTER_TIME 50u/* uint:ms */

/*4000为干运行上电检检测转速，5800为判断为干运行后的转速 22.09.25 by lxs*/
#define MOTOR_DRY_TUNNING_TEST_SPEED  4000 /* uint:rpm */
#define MOTOR_DRY_TUNNING_SPEED  5800 /* uint:rpm */
#define MOTOR_DRY_TUNNING_TEST_TIME (30*60*1000) /* uint:ms */
/*干运行过程检速度划分 22.09.25 by lxs*/
#define MOTOR_DRY_RUNNING_PROCESS_CHECK_LOW_SPEED 4000u
#define MOTOR_DRY_RUNNING_PROCESS_CHECK_HIGH_SPEED 5000u

/*转速高于4000电流阈值为0.9A，转速高于5000电流阈值为1A 22.09.25 by lxs*/
#define MOTOR_DRY_RUNNING_CURRENT_MIN 9 /* unit:0.1A, */
#define MOTOR_DRY_RUNNING_CURRENT_MAX 10 /* unit:0.1A, */
/*根据功能规范修改,22.09.25 by lxs*/
#define MOTOR_DRY_RUNNING_CURRENT_LOW_FILTER_TIME (4000u)/* uint:ms */
#define MOTOR_DRY_RUNNING_CURRENT_HIGH_FILTER_TIME 4000u/* uint:ms */

#define MOTOR_LOCK_CONFIRM_TIME 10000u /* unit:ms */

#define MOTOR_OPEN_LOAD_DAIG_CURRENT_THR 16u /* AD */
/*电机期望转速为0时清除故障的时间 22.09.12 lxs*/
#define MOTOR_STOP_CLEAR_FAULT_TIME 4000u/* uint:ms */



/*polyspace-begin  MISRA2012:6.1*/

/*polyspace-end  MISRA2012:6.1*/
/* Variables */
/******************************************************************/
MotorAPIVar_t MotorAPIVar;
/*消除polyspace警告,2021-09-22-by zyg*/
static TMat_Lp_Simple SpeedLp ={.CoefA = 1000,.CoefB = 1000};
volatile s32 DryRunningCurrent = 0;
s32 DryRunningCurrentTHR;
u8 Lin_Error_State;
BDRVOffDiagResult_t BDRV_OffDiagResult;
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
	MotorAPIVar.IsDryRunningProcessCheck = 0u;
	MotorAPIVar.IsEnterProcessCheck = 0u;
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
    /*停止时间计数，超过4s则清除相关故障 22.09.25 by lxs*/
	if(LinRx.SpdSetLast == 0u)
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
			MotorAPIVar.Faults.bit.Lock = 0u;
			MotorAPIVar.DetailFaults.bit.MotorOnOverCurrent = 0u;
			MotorAPIVar.OverCurrentTime = 0u;
			MotorAPIVar.Faults.bit.Short = 0u;
			MotorAPIVar.Faults.bit.Open = 0u;
			MotorAPIVar.DryRunningCurrentHighTime = 0u;
			MotorAPIVar.DryRunningCurrentLowTime = 0u;
	
			if((MotorAPIVar.IsDryRunning != 0u) && (MotorAPIVar.IsEnterProcessCheck == 0u))
			{
			  MotorAPIVar.IsDryRunningDetected = 0u;
			}
			else
			{
			  
			}
			MotorAPIVar.IsDryRunning = 0u;
		}
		else
		{		
			
		}
	}
	else
  {
		MotorAPIVar.StopTime = 0;
	}
	
	if((Lin_Error_State != 0u)&&(MotorAPIVar.IsEnterProcessCheck == 0u)&&(MotorAPIVar.IsDryRunning != 0u)&&(MotorAPIVar.IsDryRunningDetected != 0u))
	{
		MotorAPIVar.IsDryRunningDetected = 0u;
	}
	else
	{
		
	}
	
  if(ABS(Speed) < Emo_Foc.StartEndSpeed)
  {
    Speed = 0;
  }
  else
  {

  }
  /*依据规范进行干运行电流阈值判断 22.09.25 by lxs*/
	if((ABS(Speed) >= MOTOR_DRY_RUNNING_PROCESS_CHECK_LOW_SPEED) && (MotorAPIVar.IsEnterProcessCheck != 0u))
  {
    MotorAPIVar.IsDryRunningProcessCheck = 1u;
		if(ABS(Speed) >= MOTOR_DRY_RUNNING_PROCESS_CHECK_HIGH_SPEED)
		{
			DryRunningCurrentTHR = MOTOR_DRY_RUNNING_CURRENT_MAX;
		}
		else
		{
			DryRunningCurrentTHR = MOTOR_DRY_RUNNING_CURRENT_MIN;
		}
  }
  else
  {
    MotorAPIVar.IsDryRunningProcessCheck = 0u;
		/*上电检的干运行电流阈值判断 22.09.29 by lxs*/
		if(MotorAPIVar.IsDryRunningDetected == 0u)
		{
			DryRunningCurrentTHR = MOTOR_DRY_RUNNING_CURRENT_MIN;
		}
		else
		{
			/*剩下的情况是过程检且转速未达到4000rpm，无需电流阈值判断干运行 22.09.29 by lxs*/
		}
  }
	
  Speed *= (MOTOR_POS_DIR);

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
		static s16 tmp = 0u;
    if(MotorAPIVar.IsDryRunningDetected == 0u)
    {
	    /*干运行转速4000到5800切换判断 22.09.25 by lxs*/
			if(MotorAPIVar.DryRunningCurrentLowTime > MOTOR_DRY_RUNNING_CURRENT_LOW_FILTER_TIME)
			{
				tmp = (MOTOR_DRY_TUNNING_SPEED) * (MOTOR_POS_DIR);
			}
			else
			{
					tmp = (MOTOR_DRY_TUNNING_TEST_SPEED) * (MOTOR_POS_DIR);
			}	
			Speed = tmp;
    }
    else
    {
      	/*过程检干运行转速判断 22.09.25 by lxs*/
			if(MotorAPIVar.IsDryRunningProcessCheckFlag != 0)
			{
				tmp = (MOTOR_DRY_TUNNING_SPEED) * (MOTOR_POS_DIR);
				Speed = tmp;
			}
			else
			{

			}
    }

    if(Emo_Status.MotorState == (EMO_MOTOR_STATE_STOP))
    {
      if((MotorAPIVar.Faults.Reg == 0u)
      || ((MotorAPIVar.Faults.Reg != 0u) && (MotorAPIVar.RestartEn != 0u)))
      {
        MotorAPIVar.DetailFaults.Reg = 0u;

				MotorAPI_MotorOffDiag();
        if(MotorAPIVar.MotorOffDiagResult != 0u)
        {
          MotorAPIVar.Faults.bit.Short = (MotorAPIVar.MotorOffDiagResult & (MOTOR_SHORT)) != 0u;
          MotorAPIVar.Faults.bit.Open = (MotorAPIVar.MotorOffDiagResult & (MOTOR_OPEN_LOAD)) != 0u;

          if(MotorAPIVar.RestartEn != 0u)
          {
            MotorAPIVar.RestartComplete = 1u;
          }
          else
          {

          }
        }
        else
        {
          MotorAPIVar.Faults.bit.Short = 0u;
          MotorAPIVar.Faults.bit.Open = 0u;
        }
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
* Function Name:MotorAPI_MotorOffDiag
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
void MotorAPI_MotorOffDiag(void)
{	
	u8 MotorShort = 0,Open = 0;
	BDRV_OffDiagResult.reg = 0u;
		
	SDK_BDRVShortDiag(&BDRV_OffDiagResult);
	if(BDRV_OffDiagResult.reg == 0u)
	{
		SDK_BDRVOpenLoadDiag(&BDRV_OffDiagResult);
	}
  else
  {

  }	
	/*消除polyspace警告,2021-09-27-by zyg*/
	MotorAPIVar.DetailFaults.bit.MotorOffShortGND = BDRV_OffDiagResult.bit.ShortGND;
	MotorAPIVar.DetailFaults.bit.MotorOffShortBAT = BDRV_OffDiagResult.bit.ShortBAT;		
	MotorAPIVar.DetailFaults.bit.MotorOffOpenLoad = (BDRV_OffDiagResult.bit.OpenLoadCh0 != 0u)
																							 || (BDRV_OffDiagResult.bit.OpenLoadCh1 != 0u)
																							 || (BDRV_OffDiagResult.bit.OpenLoadCh2 != 0u);
		
  /*根据MISRA C规则修改,2021-09-17-by zyg*/
  MotorShort = (MotorAPIVar.DetailFaults.bit.MotorOffShortGND != 0u)
        || (MotorAPIVar.DetailFaults.bit.MotorOffShortBAT != 0u);
  Open = MotorAPIVar.DetailFaults.bit.MotorOffOpenLoad;
    
  MotorAPIVar.MotorOffDiagResult = (Open<<1u) | (MotorShort<<0u);
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
    /*重启时将干运行时间计数清除 22.09.25 by lxs*/
    MotorAPIVar.DryRunningCurrentHighTime = 0u;
	  MotorAPIVar.DryRunningCurrentLowTime = 0u;		
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

    if((MotorAPIVar.DetailFaults.bit.MotorOnShortGND != 0u)
    || (MotorAPIVar.DetailFaults.bit.MotorOnShortBAT != 0u)
		|| (MotorAPIVar.DetailFaults.bit.MotorOnOverCurrent != 0u)
		|| (MotorAPIVar.DetailFaults.bit.MotorOnSpeedOutRange != 0u)
		)
    {
			MotorAPIVar.DetailFaults1 = MotorAPIVar.DetailFaults;
			
      Emo_StopMotor();

			MotorAPI_MotorOffDiag();

			if((MotorAPIVar.DetailFaults.bit.MotorOnShortGND != 0u)
			|| (MotorAPIVar.DetailFaults.bit.MotorOnShortBAT != 0u))
			{
				MotorAPIVar.Faults.bit.Short = 1u;
			}
			else 
      {
        if(MotorAPIVar.DetailFaults.bit.MotorOffOpenLoad != 0u)
        {
          MotorAPIVar.Faults.bit.Open = 1u;
        }
        else
        {
          MotorAPIVar.Faults.bit.Lock = 1u;
        }
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
    && (MotorAPIVar.DetailFaults.Reg == 0u)
    && (MotorAPIVar.Faults.bit.Lock != 0u)
    && (MotorAPIVar.CloseLoopTime > (MOTOR_LOCK_CONFIRM_TIME)))
    {
			MotorAPIVar.Faults.bit.Lock = 0u;
      MotorAPIVar.RestartComplete = 1u;
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
	/*电机停止时,不再进行滤波操作 2022-06-10-by lxs*/
	if(Emo_Ctrl.ActSpeed == 0u)
	{
		MotorAPIVar.ActSpeed = 0;
		SpeedLp.Out = 0;;
	}
	else
	{
	MotorAPIVar.ActSpeed = Mat_ExeLp_without_min_max(&SpeedLp, Emo_Ctrl.ActSpeed * (MOTOR_POS_DIR));
	}
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
  {
    if((ret & (BDRV_SHORT_GND)) != 0u)
    {
      MotorAPIVar.DetailFaults.bit.MotorOnShortGND = 1u;
    }
    else
    {

    }

    if((ret & (BDRV_SHORT_BAT)) != 0u)
    {
      MotorAPIVar.DetailFaults.bit.MotorOnShortBAT = 1u;
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
   /*增加过程检判断 22.09.25 by lxs*/
  if((MotorAPIVar.IsDryRunningDetected != 0u) && (MotorAPIVar.IsDryRunningProcessCheck == 0u))
  {
    
  }
  else
  {
		s32 Current;
	
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
	/*上电检和过程检判断 22.09.25 by lxs*/
    if(MotorAPIVar.IsDryRunningDetected == 0u)
		{
		  if(MotorAPIVar.DryRunningCurrentHighTime > (MOTOR_DRY_RUNNING_CURRENT_HIGH_FILTER_TIME))
		  {
			  MotorAPIVar.IsDryRunning = 0u;
			  MotorAPIVar.IsDryRunningDetected = 1u;
				MotorAPIVar.IsEnterProcessCheck = 1u;
				MotorAPIVar.DryRunningCurrentLowTime = 0u;
				MotorAPIVar.DryRunningCurrentHighTime = 0u;
		  }
			else
			{

			}
			
			if(MotorAPIVar.DryRunningCurrentLowTime > (MOTOR_DRY_TUNNING_TEST_TIME))
		  {
				MotorAPIVar.IsDryRunning = 1u;
				MotorAPIVar.IsDryRunningDetected = 1u;
		  }
			else
			{

			}
	  }
		else
		{			
			if(MotorAPIVar.DryRunningCurrentHighTime > (MOTOR_DRY_RUNNING_CURRENT_HIGH_FILTER_TIME))
		  {
				MotorAPIVar.IsDryRunning = 0u;
			  MotorAPIVar.IsDryRunningProcessCheckFlag = 0u;
				MotorAPIVar.DryRunningCurrentLowTime = 0u;
		  }
			else
			{

			}
			
			if(MotorAPIVar.DryRunningCurrentLowTime > (MOTOR_DRY_RUNNING_CURRENT_LOW_FILTER_TIME))
		  {
				/*过程检检测到干运行，转速升到5800rpm，过程检干运行超过30分钟，干运行停止 22.09.29 by lxs*/
			  MotorAPIVar.IsDryRunningProcessCheckFlag = 1u;
				
        if(MotorAPIVar.DryRunningCurrentLowTime > (MOTOR_DRY_TUNNING_TEST_TIME))
				{
					MotorAPIVar.IsDryRunning = 1u;
				}
				else
				{

				}
		  }
			else
			{
			
			}
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
    if(MotorAPIVar.OverCurrentTime >= (MOTOR_PERIOD_TASK_TIME_BASE))
    {
      MotorAPIVar.OverCurrentTime -= (MOTOR_PERIOD_TASK_TIME_BASE);
    }
    else
    {
      MotorAPIVar.OverCurrentTime = 0u;
    }
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
  
  ActSpeed = Emo_Ctrl.ActSpeed;
  RefSpeed = Emo_Ctrl.RefSpeed;
  ActSpeedAbs = ABS(ActSpeed);
  RefSpeedAbs = ABS(RefSpeed);
  AbsDiffSpeed = ABS(RefSpeedAbs - ActSpeedAbs);
  
  u16 Current;
  Current = ABS(Emo_Ctrl.IShunt);
	/*消除polyspace警告,2021-09-27-by zyg*/
  if((ActSpeedAbs > (MOTOR_CLOSE_LOOP_SPEED_MAX)) 
	|| (ActSpeedAbs < (MOTOR_CLOSE_LOOP_SPEED_MIN))
	|| (((MotorAPIVar.IsDryRunningDetected == 0u) || (MotorAPIVar.IsDryRunning != 0u)) && (ActSpeedAbs < (MOTOR_CLOSE_LOOP_DRY_RUNNING_TEST_SPEED_MIN)))
  || (((ActSpeed >0) && (RefSpeed <0))|| ((ActSpeed < 0) && (RefSpeed > 0)))
  || ((AbsDiffSpeed > 500u) && (Current < 50u))
	|| ((ActSpeedAbs < (MOTOR_CLOSE_LOOP_SPEED_MIN)) && (Current > 55u))
	/*实际转速小于500且电流大于5.5A为蔚来要求诊断条件，实际被转速小于500包含，在程序中为废代码 23.03.26 by lxs*/
	)
  {
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
  }
}


