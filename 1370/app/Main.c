/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:Main.c
********************************************************************
* Project/Product:LXT9820_PMP
* Title :
* Author:zyg
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
*  1.0          2021-09-06        zyg            Original
*
*
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#include "Main.h"
#include "SDK.h"
#include "Common.h"
#include "TestMode.h"
#include "PwmIn.h"
#include "ADCSample.h"
#include "MotorAPI.h"
#include "DTC.h"
#include "GlobalVars.h"
/*Lin相关的头文件，需要时放开,2021-09-03-by zyg*/
#include "lin_cfg.h"
#include "lin_lld_sci.h"
#include "LINDataPro.h"

#include "AppMain.h"                   /* Model's header file */
#include "rtwtypes.h"

/* Defines*/
/******************************************************************/
#define STACK_OVER_FLOW_CHECK_EN 0
/*控制模式：0-PWM控制；1-Lin控制,2021-09-03-by zyg*/
#define CONTROL_MODE 1

/*任务调度基数,2021-09-03-by zyg*/
#define SCH_PRG_TIMER_BASE  	(5)
#define SCH_CNT_10MS_OVER     (10/SCH_PRG_TIMER_BASE)
#define SCH_CNT_50MS_OVER     (50/SCH_PRG_TIMER_BASE)
#define SCH_CNT_100MS_OVER    (100/SCH_PRG_TIMER_BASE)

/*休眠延时，2022-06-10-by lxs*/
#define SPLEEP_DELAY (1u)

/* Variables */
/******************************************************************/
volatile u16 MsCnt1;/*  1ms count */ 
static u16 MsCnt10; /* 10ms count */
static u16 MsCnt_50;/* 50ms count */
static u16 MsCnt100;/*100ms count */
static u8 SleepDelay = SPLEEP_DELAY;

s16 DebugSpeed = 0;
/*检查任务堆栈是否溢出,2021-09-03-by zyg*/
#if STACK_OVER_FLOW_CHECK_EN
uint32 StackIdx = 0;
uint32 *StackTop = (uint32 *)0x18000A08;
uint32 StackSize = (0x400>>2);
#endif

/* Function declaration */
/******************************************************************/
static void Main_SoftwareInit(void);
static void Main_AppMainInputUpdate(void);

#if CONTROL_MODE == 0	
static void Main_FeedBackBusOutput(void);
#else

#endif
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
	
  SDK_HardwareInit();
  Main_SoftwareInit();

  while (1u)
  {
    if(MsCnt1 >= SCH_PRG_TIMER_BASE)
    {
      MsCnt1 = 0u;
			
      if(MsCnt10 == 0u)
      {
        MsCnt10 = SCH_CNT_10MS_OVER;
				/*判断控制模式,2021-09-03-by zyg*/
#if CONTROL_MODE == 0
				PwmIn_PeriodTask();
#endif
				/*ADC采样,2021-09-03-by zyg*/
				ADCSample_PeriodTask();
				/*ECU自身故障诊断,2021-09-03-by zyg*/
				SDK_EcuFaultDiag();

				if(IsInTestMode == 0u)/* 2021-07-28-by zyg  */
				{
					/*更新输入sumlink模型的参数,2021-09-03-by zyg*/
					Main_AppMainInputUpdate();
					AppMain_step();
					
					/*电流零点有误、RAM错误、校验和错误将芯片复位标志重启*/
					if ((ADCResults.ErrFlags.IShuntZero != 0u) || (ECUFaults.bit.Ram != 0u) || (ECUFaults.bit.CFlash != 0u))
					{
						Y.ChipReset = 1u;
						Y.TargetSpeedLimited = 0u;
					}
					
					/*Y是sumlink模型的输出,2021-09-03-by zyg*/
					MotorAPIVar.RestartEn = Y.MotorRestartEnable;
					
					/*限制最大的期望转速,2021-09-03-by zyg*/
					s16 TargetSpeed;
					if(LinRx.IsSleep!=0u)
					{
						/*增加休眠判断,2022-06-10-by lxs*/
						TargetSpeed = 0;
					}
					else
					{
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
					}

					
					//MotorAPI_MotorSetSpeed(DebugSpeed);
				  MotorAPI_MotorSetSpeed(TargetSpeed);
					MotorAPI_MotorPeriodTask();
					
					#if CONTROL_MODE == 0
					
#else
					/*悬空4s后状态更新为全速,通讯错误持续0.121*80ms后全速,2022-03-11-by zyg*/
					if(((IDEA_TimeOut == 1)||(Lin_Error_Flag == 1))&&(LinRx.IsSleep != 1u))
					{
						Lin_Error_State = 1u;
						/*当LinRx.SpdSet=255时，采用上次转速设置值,22.05.13 by zyg*/
						LinRx.SpdSetLast = 250;
					}	
					else
					{
						Lin_Error_State = 0u;
					}						
#endif
				}
				else/*  add else - 2021-07-28-by zyg  */
				{
					
				}
      }
			else/*  add else - 2021-07-28-by zyg  */
			{
				
			}

      if(MsCnt_50 == 0u)
      {
        MsCnt_50 = SCH_CNT_50MS_OVER;

				SDK_FeedWDog();
				/*FCT测试模式,2021-09-03-by zyg*/
        TestMode_FactoryTest();
				/*串口调试,2021-09-03-by zyg*/
        TestMode_UartDebugPeriodTask();
				
#if CONTROL_MODE == 0
					
#else
				/*Lin收发函数,2021-09-03-by zyg*/
				LIN_Rx();
				if(IsInTestMode == 0u)
				{
					LIN_Tx();
				}
				else
				{
					
				}
#endif				
      }
			else/*  add else - 2021-07-28-by zyg  */
			{
				
			}

      if(MsCnt100 == 0u)
      {
        MsCnt100 = SCH_CNT_100MS_OVER;
				
				if(IsInTestMode == 0u)/* 2021-07-28-by zyg  */
				{
#if CONTROL_MODE == 0
					/*故障反馈输出,2021-09-03-by zyg*/
					Main_FeedBackBusOutput();
#else		
					
#endif		
					/*故障更新,2021-09-03-by zyg*/
					DTC_PeriodTask();
					/*Lin休眠,2022-06-01-by zyg*/
					if(LinRx.IsSleep != 0u)
					{
						/*LIN休眠延时,2022-06-10-by lxs*/
						if(SleepDelay > 0u)
						{
							SleepDelay--;
						}
						else
						{
						SDK_GoToSleepMode();
						}
					}				
					if (Y.ChipReset != 0u)
					{
						SDK_SystemReset();
					}				
				}
				else
				{
				
				}
				/*上电时间计数,2021-09-03-by zyg*/
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
/*消除polyspace警告,2021-09-18-by zyg*/
static void Main_SoftwareInit(void)
{
#if CONTROL_MODE == 0	

#else
	l_bool ret_sys = 0,ret_ifc = 0;
#endif	
	SDK_Mem_Check();
	DTC_Init();
  MotorAPI_MotorInit();
	AppMain_initialize();
	U.PwmInHighDuty = 100u;
	
#if CONTROL_MODE == 0	

#else	
	ret_sys = l_sys_init();
	ret_sys = ret_sys;
	
  ret_ifc = l_ifc_init(LI0);
	ret_ifc = ret_ifc;
#endif
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Main_AppMainInputUpdate
* Description:
*			sumlink模型输入
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
	#if 0
	U.PwmInFreq = PwmIn.Freq;
	U.PwmInHighDuty = (PwmIn.HighDuty/10u + (((PwmIn.HighDuty%10u) > 5u) ? 1u : 0u)) * 10u;
	U.PwmInIdleTime = PwmIn.BusIdleTime;
	U.PwmInPinSts = PwmIn.BusState;
	#else
	U.PwmInFreq = 1000;
	//U.PwmInHighDuty = 100;
	//U.PwmInHighDuty = LinRx.SpdSet;
	/*当LinRx.SpdSet=255时，采用上次转速设置值,22.05.13 by zyg*/
	U.PwmInHighDuty = LinRx.SpdSetLast;
	U.PwmInIdleTime = 0;
	U.PwmInPinSts = 0;
	#endif
	U.SleepValid = 0u;
	U.AmbTemperature = ADCResults.Temperature.Chip;
	
	U.SupVoltage = ADCResults.Voltage.Bat;
	U.FilteredVoltage = ADCResults.Voltage.BatFiltered;

  U.ChipFault = ECUFaults.bit.ECUFault;

  U.IsMotorShort = MotorAPIVar.Faults.bit.Short;
  U.IsMotorOpenLoad = MotorAPIVar.Faults.bit.Open;
  U.IsMotorLock = MotorAPIVar.Faults.bit.Lock;
  U.IsMotorDryRunning = MotorAPIVar.IsDryRunning;

  U.RestartComplete = MotorAPIVar.RestartComplete;
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
#if CONTROL_MODE == 0	

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
	}
	else
	{
		PwmIn.CaptureEnable = Y.FeedBackBusStatus;
	}
	
	FeedBackBusStatusLast = Y.FeedBackBusStatus;
}
#else	

#endif
