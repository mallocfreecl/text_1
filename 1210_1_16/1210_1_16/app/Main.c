/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:Main.c
********************************************************************
* Project/Product:LXT9818_PMP
* Title :
* Author:yhd
*********************************************************************
* Description:
* 1.Init hardware and software
* 2.Call simulink model step function
* 3.Schedule time base task
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
* ----------    --------------    ------------   ----------------------------------------
*  1.0       2021-07-23        yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/    //9829的工程规范
#include "Main.h"
#include "SDK.h"
#include "Common.h"
#include "TestMode.h"
#include "PwmIn.h"
#include "ADCSample.h"
#include "MotorAPI.h"
#include "DTC.h"
#include "GlobalVars.h"

#include "AppMain.h"                   /* Model's header file */
#include "rtwtypes.h"

/* Defines*/
/******************************************************************/
#define STACK_OVER_FLOW_CHECK_EN 0

#define SCH_PRG_TIMER_BASE  	(5)
#define SCH_CNT_10MS_OVER     (10/SCH_PRG_TIMER_BASE)
#define SCH_CNT_50MS_OVER     (50/SCH_PRG_TIMER_BASE)
#define SCH_CNT_100MS_OVER    (100/SCH_PRG_TIMER_BASE)

#define FAUIT_FEEDBACK_CNT 1u

#define MOTOR_MAX_SPEED (6950)

/* Variables */
/******************************************************************/
volatile u16 MsCnt1;/*  1ms count */ 
static u16 MsCnt10; /* 10ms count */
static u16 MsCnt_50;/* 50ms count */
static u16 MsCnt100;/*100ms count */

s16 DebugSpeed = 0;

#if STACK_OVER_FLOW_CHECK_EN
uint32 StackIdx = 0;
uint32 *StackTop = (uint32 *)0x18000A08;
uint32 StackSize = (0x400>>2);
#endif

/* Function declaration */
/******************************************************************/
static void Main_SoftwareInit(void);
static void Main_AppMainInputUpdate(void);
static void Main_FeedBackBusOutput(void);

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:main
* Description:
* 1.Init hardware and software
* 2.Call simulink model step function
* 3.Schedule time base task
*
* Inputs:none
*
*
* Outputs:none
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
int main(void)
{
#if STACK_OVER_FLOW_CHECK_EN
	for(StackIdx = 0;StackIdx < StackSize; StackIdx++)
	{
		*(StackTop - StackIdx) = 0x5A5A5A5A;
	}
#endif
	
  SDK_HardwareInit();   //芯片外设初始化
  Main_SoftwareInit();

  while (1u)
  {
    if(MsCnt1 >= SCH_PRG_TIMER_BASE)
    {
      MsCnt1 = 0u;
			
      if(MsCnt10 == 0u)
      {
        MsCnt10 = SCH_CNT_10MS_OVER;

				PwmIn_PeriodTask();
				ADCSample_PeriodTask();
				SDK_EcuFaultDiag();

				if(IsInTestMode == 0u)/* 2021-07-28-by zyg  */
				{
					Main_AppMainInputUpdate();
					AppMain_step();
					
					/*RAM错误、校验和错误将芯片复位标志重启*/
					if ((ECUFaults.bit.Ram != 0u) || (ECUFaults.bit.CFlash != 0u))
					{
						Y.ChipReset = 1u;
						Y.TargetSpeedLimited = 0u;
					}
					
					/*Y是sumlink模型的输出,2021-09-03-by zyg*/
					MotorAPIVar.RestartEn = Y.MotorRestartEnable;
					
					s16 TargetSpeed;
					if(Y.TargetSpeedLimited < -MOTOR_MAX_SPEED)
					{
						TargetSpeed = -MOTOR_MAX_SPEED;
					}
					else if(Y.TargetSpeedLimited > MOTOR_MAX_SPEED)
					{
						TargetSpeed = MOTOR_MAX_SPEED;
					}
					else
					{
						TargetSpeed = Y.TargetSpeedLimited;
					}
					/* MotorAPI_MotorSetSpeed(DebugSpeed); */
				  MotorAPI_MotorSetSpeed(TargetSpeed);
					MotorAPI_MotorPeriodTask();
				}
				else/*  add else - 2021-07-28-by zyg  */
				{
					
				}
				//MotorAPI_BatStatus_Diag();
      }
			else/*  add else - 2021-07-28-by zyg  */
			{
				
			}

      if(MsCnt_50 == 0u)
      {
        MsCnt_50 = SCH_CNT_50MS_OVER;

				SDK_FeedWDog();
				
        TestMode_FactoryTest();
        TestMode_UartDebugPeriodTask();
      }
			else/*  add else - 2021-07-28-by zyg  */
			{
				
			}

      if(MsCnt100 == 0u)
      {
        MsCnt100 = SCH_CNT_100MS_OVER;
				if(IsInTestMode == 0u)/* 2021-07-28-by zyg  */
				{
					Main_FeedBackBusOutput();
				}
				else
				{
					if(FctTestPwmOut5HzEn == 1u)
					{
						SDK_PwmOutToggle();
					}
					else
					{

					}
				}	
				
				DTC_PeriodTask();
				
				if (Y.ChipReset != 0u)
				{
					SDK_SystemReset();
				}

        GlobalVars_PowerUpTimeCount();
      }
			else/*  add else - 2021-07-28-by zyg  */
			{
				
			}
			
      MsCnt10--;
      MsCnt_50--;
      MsCnt100--;
    }
		else/*  add else - 2021-07-28-by zyg  */
		{
				
		}
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Main_SoftwareInit
* Description:
*		Init software components
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
/*消除polyspace警告,2022-06-28-by lxs*/
static void Main_SoftwareInit(void)
{
	SDK_Mem_Check();
	DTC_Init();
  MotorAPI_MotorInit();
	AppMain_initialize();
	U.PwmInHighDuty = 100u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Main_AppMainInputUpdate
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
static void Main_AppMainInputUpdate(void)
{
	U.PwmInFreq = PwmIn.Freq;
	U.PwmInHighDuty = (PwmIn.HighDuty+5)/10*10;/*四舍五入 22.09.01 by lxs*/
	U.PwmInIdleTime = PwmIn.BusIdleTime;
	U.PwmInPinSts = PwmIn.BusState;

	
	U.SleepValid = 0u;
	
	U.AmbTemperature = ADCResults.Temperature.Chip;
	U.SupVoltage = ADCResults.Voltage.Bat;
	U.FilteredVoltage = ADCResults.Voltage.BatFiltered;

  U.ChipFault = ECUFaults.bit.ECUFault;

  U.IsMotorShort = MotorAPIVar.Faults.bit.Short;
	/*不适用，24-02-25 by wchuang*/
  U.IsMotorOpenLoad = 0;
  U.IsMotorLock = MotorAPIVar.Faults.bit.Lock;
  U.IsMotorDryRunning = MotorAPIVar.IsDryRunning;

  U.RestartComplete = MotorAPIVar.RestartComplete;
  /*加入故障反馈计数标志和故障反馈计数清除完成标志 22.11.25 by lxs*/
	U.FeedBackCnt = MotorAPI_FeedBackCnt;
	U.FeedBackClearComplete = MotorAPI_FeedBackClearComplete;
  MotorAPIVar.RestartComplete = 0u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Main_FeedBackBusOutput
* Description:
*		Set PwmOut bus state.
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
static void Main_FeedBackBusOutput(void)
{
	static u8 FeedBackBusStatusLast = 1u;
	
	if((Y.FeedBackBusStatus != 0u) && (FeedBackBusStatusLast == 0u))
	{
		/* Delay to enable pwm in cature */
		SDK_PwmOutLow();
	}
	else if((Y.FeedBackBusStatus == 0u) && (FeedBackBusStatusLast != 0u))
	{	
		PwmIn.CaptureEnable = 0u;		
		SDK_PwmOutHigh();
		/*产生新的故障则计数清0 22.11.25 by lxs*/
		if(Y.FeedBackClearEn != 0u)
		{
			MotorAPI_FeedBackCnt = 0u;
			MotorAPI_FeedBackClearComplete = 1;
		}
		else
		{
			MotorAPI_FeedBackClearComplete = 0;
		}
		/*当前有故障或发生故障后故障反馈次数和时间未达到退出要求 22.11.25 by lxs*/
		if((Y.FeedBackFlag != 0u) || ((Y.FeedBackFlagLast != 0u) && ((MotorAPI_FeedBackCnt < 5u)||(Y.FaultRecoverCntOut < 50u))))
		{
			if(MotorAPI_FeedBackCnt <= 0xFFFFu - (FAUIT_FEEDBACK_CNT))
			{
				MotorAPI_FeedBackCnt += (FAUIT_FEEDBACK_CNT);
			}
			else
			{
				MotorAPI_FeedBackCnt = 0xFFFFu;
			}
		}
		else
		{
			MotorAPI_FeedBackCnt = 0u;
		}
		
	}
	else
	{
		PwmIn.CaptureEnable = Y.FeedBackBusStatus;
	}
	
	FeedBackBusStatusLast = Y.FeedBackBusStatus;
}
