/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
* disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:ErtMain.c
********************************************************************
* Project/Product:t3622
* Title :
* Author:yhd
*********************************************************************
* Description:
* 1.初始化硬件
* 2.初始化软件
* 3.轮询执行周期性任务
*
* (Requirements, pseudo code and etc.)
*********************************************************************
* Limitations:
* 1.
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
#include "ErtMain.h"
#include "Common.h"
#include "DTC.h"
#include "FctTest.h"
#include "GlobalVars.h"
#include "MotorAPI.h"
#include "Sdk.h"
#include "TerminalTest.h"
#include "UartDebug.h"

/* Simulink model's  header file */
#include "AppMain.h"
#include "rtwtypes.h"

/* Defines*/
/******************************************************************/
#define ERT_MAIN_TIMER_BASE (5u)
#define ERT_MAIN_CNT_5MS (5u / (ERT_MAIN_TIMER_BASE))
#define ERT_MAIN_CNT_10MS (10u / (ERT_MAIN_TIMER_BASE))
#define ERT_MAIN_CNT_50MS (50u / (ERT_MAIN_TIMER_BASE))
#define ERT_MAIN_CNT_100MS (100u / (ERT_MAIN_TIMER_BASE))

/* Variables */
/******************************************************************/
/* Time count,unit:1ms、10ms、50ms、100ms.Increase Ms1 in 1ms isr */
vu16 Ms1 = 0u;
static u16 Ms5 = 0u;
static u16 Ms10 = 0u;
static u16 Ms50 = 0u;
static u16 Ms100 = 0u;
static u8 IsInTestMode = 0u;

/* Function declaration */
/******************************************************************/
static void initSw(void);
static void calIShuntZero(void);
static void updateAppMainInput(void);
static void updateFeedBackSts(void);

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:ErtMain
* Description:
* 1.初始化硬件
* 2.初始化软件
* 3.轮询执行周期性任务
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
void ErtMain(void)
{
  Sdk_initHw();
  initSw();

  while (1)
  {
    if (Ms1 >= ERT_MAIN_TIMER_BASE)
    {
      Ms1 = 0u;

      if (Ms5 == 0)
      {
        Ms5 = ERT_MAIN_CNT_5MS;

        if (IsInFctTestMode != 0u)
        {
          FctTest_samplePhaseVoltage();
          FctTest_genPwmOut100Hz50Duty();
        }
      }

      if (Ms10 == 0u)
      {
        Ms10 = ERT_MAIN_CNT_10MS;

        DrvPwmIn_step();
        DrvAdc_step();
        Sdk_diagEcuFault();

        updateAppMainInput();
        AppMain_step();

        if ((AdcResults.ErrFlags.IShuntZero != 0u) || (EcuFaults.bit.Ram != 0u) || (EcuFaults.bit.CFlash != 0u))
        {
          Y.ChipReset = 1u;
          Y.TargetSpeedLimited = 0u;
        }

        MotorAPI.RestartEn = Y.MotorRestartEnable;
        MotorAPI_step();

        MotorAPI_setSpeed(Y.TargetSpeedLimited);

        if (IsInFctTestMode != 0u)
        {
          FctTest_genPwmOut50Hz50Duty();
        }
      }

      if (Ms50 == 0u)
      {
        Ms50 = ERT_MAIN_CNT_50MS;

        Sdk_feedWDog();

        FctTest_step();
        TerminalTest_step();
        IsInTestMode = (IsInFctTestMode != 0u) || (IsInTerminalTestMode != 0u);

        UartDebug_step();
      }

      if (Ms100 == 0u)
      {
        Ms100 = ERT_MAIN_CNT_100MS;

        if (IsInTestMode == 0u)
        {
          updateFeedBackSts();
        }

        DTC_step();

        /* Fct测试模式下，执行到休眠步骤后立即进入休眠状态，22.04.01 by yhd */
        if (((IsInTestMode == 0u) && (Y.ChipSleep != 0u)) || ((IsInFctTestMode != 0u) && (FctTestSleepEn != 0u)) ||
            ((IsInTerminalTestMode != 0u) && (TerminalTestSleepEn != 0u) && (Y.ChipSleep != 0u)))
        {
          Sdk_enterSleep();
        }

        if (Y.ChipReset != 0u)
        {
          Sdk_rstChip();
        }

        GlobalVars_updatePowerUpTimeCnt();
      }

      Ms5--;
      Ms10--;
      Ms50--;
      Ms100--;
    }
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:initSw
* Description:
*	1.检查芯片存储是否正常
* 2.初始化DTC
* 3.初始化ADC采样
* 4.初始化电机控制
* 5.初始化simulink模型
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
* 1.在硬件初始化完成后执行
********************************************************************
END_FUNCTION_HDR*/
static void initSw(void)
{
  Sdk_enterCritical();

  DTC_init();
  DrvAdc_init();
  calIShuntZero();
  MotorAPI_init();
  AppMain_initialize();

  Sdk_exitCritical();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:calcIShuntZero
* Description:
* 1.采集母线电流采样运放零点值
* 2.如果电源电压、环境温度在设置的范围内，则进行母线电流采样运放零点标定
* 3.母线电流采样运放零点无法标定或标定错误时，使用DTC中的存储值
*
* Inputs:
*
*
* Outputs:
* 1.AdcResults.RawAD.IShuntZero：电流零点AD值
* 2.AdcResults.ErrFlags.IShuntZero：电流零点值错误标志，1：错误，0：正常
*
* Limitations:
* 1.该函数应在DTC初始化后调用
********************************************************************
END_FUNCTION_HDR*/
static void calIShuntZero(void)
{
  u8 Err = 0u;

  if ((AdcResults.Voltage.Bat > DRV_ADC_I_SHUNT_ZERO_CALIBRATE_BAT_MIN) &&
      (AdcResults.Voltage.Bat < DRV_ADC_I_SHUNT_ZERO_CALIBRATE_BAT_MAX) &&
      (AdcResults.Temperature.Ambient > DRV_ADC_I_SHUNT_ZERO_CALIBRATE_AMBT_MIN) &&
      (AdcResults.Temperature.Ambient < DRV_ADC_I_SHUNT_ZERO_CALIBRATE_AMBT_MAX))
  {
    Err = DrvAdc_calcIShuntZero();
  }
  else
  {
    Err = 1u;
  }

  if (Err != 0u)
  {
    AdcResults.RawAD.IShuntZero = DtcData.IShuntZero;
  }

  AdcResults.ErrFlags.IShuntZero =
    (AdcResults.RawAD.IShuntZero > DRV_ADC_ISHUNT_ZERO_MAX) || (AdcResults.RawAD.IShuntZero < DRV_ADC_ISHUNT_ZERO_MIN);

  if (AdcResults.ErrFlags.IShuntZero == 0u)
  {
    DtcData.IShuntZero = AdcResults.RawAD.IShuntZero;
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:updateAppMainInput
* Description:
* 更新simulink模型输入
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
* 1.在每次调用simulink模型*step函数前执行
********************************************************************
END_FUNCTION_HDR*/
static void updateAppMainInput(void)
{
  u16 Duty = 0u;
  /* 低占空比有效，22/03/23,by,jxj */
  Duty = (PwmInAPI.LowDuty + 5u) / 10u * 10u;
  U.PwmInFreq = PwmInAPI.Freq;
  U.PwmInDuty = Duty;
  U.PwmInIdleTime = PwmInAPI.BusIdleTime;
  U.PwmInPinSts = PwmInAPI.BusState;

  /* SleepValid is updated in AppMain*/
  U.SleepValid = (Sdk_getIGSts() == 0u);

  U.AmbTemperature = AdcResults.Temperature.Ambient;
  U.SupVoltage = AdcResults.Voltage.Bat;
  U.FilteredVoltage = AdcResults.Voltage.BatFilt;

  U.ChipFault = EcuFaults.bit.ECUFault;

  U.IsMotorShort = MotorAPI.Faults.bit.Short;
  U.IsMotorOpenLoad = MotorAPI.Faults.bit.Open;
  U.IsMotorLock = MotorAPI.Faults.bit.Lock;
  U.IsMotorDryRunning = 0u;

  U.RestartComplete = MotorAPI.RestartComplete;
  /* MotorAPI.RestartComplete 必须清零 */
  MotorAPI.RestartComplete = 0u;

  U.TestModeSpeedCtrlEnable = IsInTestMode;
  if (IsInTerminalTestMode != 0u)
  {
    U.TestModeSpeed = TernimalTestSpeed;
  }
  else
  {
    U.TestModeSpeed = 0;
  }

  if (IsInFctTestMode != 0u)
  {
    U.TestModeSleepEn = (FctTestSleepEn != 0u);
  }
  else if (IsInTerminalTestMode != 0u)
  {
    U.TestModeSleepEn = (TerminalTestSleepEn != 0u);
  }
  else
  {
    U.TestModeSleepEn = 1u;
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:updateFeedBackSts
* Description:
* 1.根据simulink母线输出的Y.FeedBackBusStatus设置反馈引脚状态
* 2.对于信号输入、输出为1根信号线的情况，设置PwmInAPI.CaptureEn控制
*  PwmIn模块是否检测输入信号
*
* Inputs:
* 1.Y.FeedBackBusStatus：反馈信号状态
*
* Outputs:
* 1.PwmInAPI.CaptureEn：是否允许检测Pwm输入信号标志，1：允许，0：禁止
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static void updateFeedBackSts(void)
{
  static u8 FeedBackBusStatusLast = 1u;

  if ((Y.FeedBackBusStatus != 0u) && (FeedBackBusStatusLast == 0u))
  {
    /* Delay to enable pwm in cature */
    Sdk_clrPwmOutPin();
  }
  else if ((Y.FeedBackBusStatus == 0u) && (FeedBackBusStatusLast != 0u))
  {
    PwmInAPI.CaptureEn = 0u;
    Sdk_setPwmOutPin();
  }
  else
  {
    PwmInAPI.CaptureEn = Y.FeedBackBusStatus;
  }

  FeedBackBusStatusLast = Y.FeedBackBusStatus;
}
