/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
* disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:TerminalTest.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.执行终端测试用于测试电机总成
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
#include "TerminalTest.h"
#include "DrvAdc.h"
#include "DrvPwmIn.h"
#include "MotorAPI.h"
#include "Sdk.h"

#include "AppMain.h" /* Model's header file */
#include "rtwtypes.h"

/* Defines*/
/******************************************************************/
#define TERMINAL_TEST_TIME_BASE (50u)             // Unit:ms
#define TERMINAL_TEST_ENTRY_TIME_OUT_TIME (2000u) // Unit:ms

#define TERMINAL_TEST_ENTRY_CONFIRM_TIME_CNT1 (20u)
#define TERMINAL_TEST_ENTRY_CONFIRM_TIME_CNT2 (10u)

#define TERNIMAL_TEST_PWM_IN_BUS_IDLE_TIME_MAX (1000u) // ms

#define TERMINAL_TEST_PWM_IN_FREQ_MIN (3800u) // 0.1Hz
#define TERMINAL_TEST_PWM_IN_FREQ_MAX (4200u) // 0.1Hz

#define TERMINAL_TEST_PWM_IN_DUTY_ERROR (20u) // 0.1%

#define TERMINAL_TEST_ENTRY_DUTY (300u) // 0.1%
#define TERMINAL_TEST_ENTRY_DUTY_MIN ((TERMINAL_TEST_ENTRY_DUTY) - (TERMINAL_TEST_PWM_IN_DUTY_ERROR))
#define TERMINAL_TEST_ENTRY_DUTY_MAX ((TERMINAL_TEST_ENTRY_DUTY) + (TERMINAL_TEST_PWM_IN_DUTY_ERROR))

#define TERMINAL_TEST_STEP1_DUTY 400 // 0.1%
#define TERMINAL_TEST_STEP1_DUTY_MIN ((TERMINAL_TEST_STEP1_DUTY) - (TERMINAL_TEST_PWM_IN_DUTY_ERROR))
#define TERMINAL_TEST_STEP1_DUTY_MAX ((TERMINAL_TEST_STEP1_DUTY) + (TERMINAL_TEST_PWM_IN_DUTY_ERROR))

#define TERMINAL_TEST_STEP2_DUTY_MIN (500u - (TERMINAL_TEST_PWM_IN_DUTY_ERROR))
#define TERMINAL_TEST_STEP2_DUTY_MAX (800u + (TERMINAL_TEST_PWM_IN_DUTY_ERROR))

#define TERMINAL_TEST_STEP3_DUTY (900u) // 0.1%
#define TERMINAL_TEST_STEP3_DUTY_MIN ((TERMINAL_TEST_STEP3_DUTY) - (TERMINAL_TEST_PWM_IN_DUTY_ERROR))
#define TERMINAL_TEST_STEP3_DUTY_MAX ((TERMINAL_TEST_STEP3_DUTY) + (TERMINAL_TEST_PWM_IN_DUTY_ERROR))

#define TERMINAL_TEST_AMB_T_MIN (0)
#define TERMINAL_TEST_AMB_T_MAX (140)

#define TERMINAL_TEST_BAT_MIN (120u)
#define TERMINAL_TEST_BAT_MAX (150u)

#define TERMINAL_TEST_MON_MIN (110u)
#define TERMINAL_TEST_MON_MAX (120u)

#define TERMINAL_TEST_VSD_MIN (105u)
#define TERMINAL_TEST_VSD_MAX (125u)

#define TERMINAL_TEST_VCP_MIN (125u)
#define TERMINAL_TEST_VCP_MAX (145u)

#define TERMINAL_TEST_VDDP_MIN (160u)
#define TERMINAL_TEST_VDDP_MAX (181u)

#define TERMINAL_TEST_VBG_MIN (145u)
#define TERMINAL_TEST_VBG_MAX (183u)

#define TERMINAL_TEST_VAREF_MIN (215u)
#define TERMINAL_TEST_VAREF_MAX (240u)

#define TERMINAL_TEST_VDDC_MIN (215u)
#define TERMINAL_TEST_VDDC_MAX (253u)

#define TERNIMAL_TEST_SPEED_CURVE_SPEED_MIN (1700)
#define TERNIMAL_TEST_SPEED_CURVE_SPEED_MAX (2100)
#define TERNIMAL_TEST_SPEED_CURVE_DUTY_MIN (500)
#define TERNIMAL_TEST_SPEED_CURVE_DUTY_MAX (800)

#define TERNIMAL_TEST_SPEED_MIN (800)

/* Variables */
/******************************************************************/
u8 IsInTerminalTestMode = 0u;
s16 TernimalTestSpeed = 0;
u8 TerminalTestSleepEn = 0u;

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:TerminalTest_step
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
void TerminalTest_step(void)
{
  static u8 TernimalEntryTimeOutCnt = (TERMINAL_TEST_ENTRY_TIME_OUT_TIME) / (TERMINAL_TEST_TIME_BASE);
  static u8 ConfirmTimeCnt = TERMINAL_TEST_ENTRY_CONFIRM_TIME_CNT1;
  static u8 TestStep = 0u;
  static u8 FeedBackEn = 0u;
  static u8 LastFeedBackEn = 0u;
  static u8 PulseCnt = 0u;
  static u8 BusSts = 0u;
  static u16 BusLowTime = 0u;
  static u16 BusHighTime = 0u;
  static u16 BusLowTimeTmp = 0u;
  static u16 BusHighTimeTmp = 0u;

  if ((IsInTerminalTestMode == 0u) && (TernimalEntryTimeOutCnt > 0u))
  {
    TernimalEntryTimeOutCnt--;

    if ((PwmInAPI.BusIdleTime < TERNIMAL_TEST_PWM_IN_BUS_IDLE_TIME_MAX) &&
        (PwmInAPI.Freq > TERMINAL_TEST_PWM_IN_FREQ_MIN) && (PwmInAPI.Freq < TERMINAL_TEST_PWM_IN_FREQ_MAX) &&
        (PwmInAPI.HighDuty < TERMINAL_TEST_ENTRY_DUTY_MAX) && (PwmInAPI.HighDuty > TERMINAL_TEST_ENTRY_DUTY_MIN))
    {
      if (ConfirmTimeCnt > 0u)
      {
        ConfirmTimeCnt--;
      }
      else
      {
        IsInTerminalTestMode = 1u;
        TernimalTestSpeed = 0;
      }
    }
    else
    {
      ConfirmTimeCnt = TERMINAL_TEST_ENTRY_CONFIRM_TIME_CNT2;
    }
  }
  else
  {
    TernimalEntryTimeOutCnt = 0u;
  }

  if (IsInTerminalTestMode != 0u)
  {
    if ((PwmInAPI.BusIdleTime < TERNIMAL_TEST_PWM_IN_BUS_IDLE_TIME_MAX) &&
        (PwmInAPI.Freq > TERMINAL_TEST_PWM_IN_FREQ_MIN) && (PwmInAPI.Freq < TERMINAL_TEST_PWM_IN_FREQ_MAX))
    {
      if ((PwmInAPI.HighDuty < TERMINAL_TEST_ENTRY_DUTY_MAX) && (PwmInAPI.HighDuty > TERMINAL_TEST_ENTRY_DUTY_MIN))
      {
        if (TestStep != 0u)
        {
          PulseCnt = 0u;
          FeedBackEn = 0u;
        }

        TestStep = 0u;
      }
      else if ((PwmInAPI.HighDuty < TERMINAL_TEST_STEP1_DUTY_MAX) && (PwmInAPI.HighDuty > TERMINAL_TEST_STEP1_DUTY_MIN))
      {
        if (TestStep != 1u)
        {
          PulseCnt = 0u;
          FeedBackEn = 0u;
        }
        TestStep = 1u;
      }
      else if ((PwmInAPI.HighDuty < TERMINAL_TEST_STEP2_DUTY_MAX) && (PwmInAPI.HighDuty > TERMINAL_TEST_STEP2_DUTY_MIN))
      {
        if (TestStep != 2u)
        {
          PulseCnt = 0u;
          FeedBackEn = 0u;
        }
        TestStep = 2u;
      }
      else if ((PwmInAPI.HighDuty < TERMINAL_TEST_STEP3_DUTY_MAX) && (PwmInAPI.HighDuty > TERMINAL_TEST_STEP3_DUTY_MIN))
      {
        if (TestStep != 3u)
        {
          PulseCnt = 0u;
          FeedBackEn = 0u;
        }
        TestStep = 3u;
      }
      else
      {
      }
    }

    if (MotorAPI.Faults.bit.Lock != 0u)
    {
      BusLowTime = 13u;
      BusHighTime = 1u;
    }
    else if (MotorAPI.Faults.bit.Open != 0u)
    {
      BusLowTime = 11u;
      BusHighTime = 1u;
    }
    else if (MotorAPI.Faults.bit.Short != 0u)
    {
      BusLowTime = 9u;
      BusHighTime = 1u;
    }
    else if ((AdcResults.Temperature.Ambient > TERMINAL_TEST_AMB_T_MAX) ||
             (AdcResults.Temperature.Ambient < TERMINAL_TEST_AMB_T_MIN) ||
             (AdcResults.Temperature.Chip > TERMINAL_TEST_AMB_T_MAX) ||
             (AdcResults.Temperature.Chip < TERMINAL_TEST_AMB_T_MIN) || (EcuFaults.bit.SysOT != 0u))
    {
      BusLowTime = 7u;
      BusHighTime = 1u;
    }
    else if (((MotorAPI.RefSpeed == 0) &&
              ((AdcResults.Voltage.Bat > TERMINAL_TEST_BAT_MAX) ||
               (AdcResults.Voltage.Bat < TERMINAL_TEST_BAT_MIN)
               //||(AdcResults.CH_MON > TERMINAL_TEST_MON_MAX)
               //||(AdcResults.CH_MON < TERMINAL_TEST_MON_MIN)
               || (AdcResults.RawAD.VSD > TERMINAL_TEST_VSD_MAX) || (AdcResults.RawAD.VSD < TERMINAL_TEST_VSD_MIN) ||
               (AdcResults.RawAD.VCP > TERMINAL_TEST_VCP_MAX) || (AdcResults.RawAD.VCP < TERMINAL_TEST_VCP_MIN) ||
               (AdcResults.RawAD.VDDP > TERMINAL_TEST_VDDP_MAX) || (AdcResults.RawAD.VDDP < TERMINAL_TEST_VDDP_MIN) ||
               (AdcResults.RawAD.VBG > TERMINAL_TEST_VBG_MAX) || (AdcResults.RawAD.VBG < TERMINAL_TEST_VBG_MIN) ||
               (AdcResults.RawAD.VAREF > TERMINAL_TEST_VAREF_MAX) ||
               (AdcResults.RawAD.VAREF < TERMINAL_TEST_VAREF_MIN) || (AdcResults.RawAD.VDDC > TERMINAL_TEST_VDDC_MAX) ||
               (AdcResults.RawAD.VDDC < TERMINAL_TEST_VDDC_MIN))) ||
             (EcuFaults.bit.PMU_5V_OV != 0u) || (EcuFaults.bit.PMU_1V5_OV != 0u) || (EcuFaults.bit.SuppShort != 0u) ||
             (EcuFaults.bit.SuppTimeOut != 0u) || (EcuFaults.bit.VDDEXT_OverLoad != 0u) ||
             (EcuFaults.bit.VDDEXT_OverVoltage != 0u) || (EcuFaults.bit.VDDEXT_Short != 0u) ||
             (((EcuFaults.bit.Ref5VOverLoad != 0u) || (EcuFaults.bit.Ref5VOverVoltage != 0u) ||
               (EcuFaults.bit.Ref5VUnderVoltage != 0u)) != 0u)
             //||(glADC2VBGFault != 0)
             || (EcuFaults.bit.VSDHigh != 0u) || (EcuFaults.bit.VSDLow != 0u) || (EcuFaults.bit.VCPHigh != 0u) ||
             (EcuFaults.bit.VCPLow1 != 0u) || (EcuFaults.bit.VCPLow2 != 0u))
    {
      BusLowTime = 5u;
      BusHighTime = 1u;
    }
    else
    {
      BusLowTime = 0u;
      BusHighTime = 0u;
    }

    switch (TestStep)
    {
    case 0x00u: {
      if (FeedBackEn == 0u)
      {
        if (PulseCnt > 1u)
        {
          FeedBackEn = 0u;
        }
        else
        {
          PulseCnt++;

          FeedBackEn = 1u;
          BusLowTime = 1u;
          BusHighTime = 1u;
        }
      }
      TernimalTestSpeed = 0;
    }
    break;
    case 0x01u: {
      if (FeedBackEn == 0u)
      {
        if (PulseCnt > 1u)
        {
          FeedBackEn = 0u;
        }
        else
        {
          PulseCnt++;

          FeedBackEn = 1u;
          if ((BusHighTime == 0u) && (BusLowTime == 0u))
          {
            BusLowTime = 3u;
            BusHighTime = 1u;
          }
        }
      }
      TernimalTestSpeed = 0;
    }
    break;
    case 0x02u: {
      if (FeedBackEn == 0u)
      {
        if (PulseCnt > 1u)
        {
          FeedBackEn = 0u;
        }
        else
        {
          if ((BusHighTime == 0u) && (BusLowTime == 0u))
          {
            FeedBackEn = 0u;
          }
          else
          {
            PulseCnt++;
            FeedBackEn = 1u;
          }
        }
      }

      if (PwmInAPI.HighDuty < TERNIMAL_TEST_SPEED_CURVE_DUTY_MIN)
      {
        TernimalTestSpeed = TERNIMAL_TEST_SPEED_CURVE_SPEED_MIN;
      }
      else if (PwmInAPI.HighDuty > TERNIMAL_TEST_SPEED_CURVE_DUTY_MAX)
      {
        TernimalTestSpeed = TERNIMAL_TEST_SPEED_CURVE_SPEED_MAX;
      }
      else
      {
        TernimalTestSpeed = TERNIMAL_TEST_SPEED_CURVE_SPEED_MIN +
                            (PwmInAPI.HighDuty - TERNIMAL_TEST_SPEED_CURVE_DUTY_MIN) *
                              (TERNIMAL_TEST_SPEED_CURVE_SPEED_MAX - TERNIMAL_TEST_SPEED_CURVE_SPEED_MIN) /
                              (TERNIMAL_TEST_SPEED_CURVE_DUTY_MAX - TERNIMAL_TEST_SPEED_CURVE_DUTY_MIN);
      }
    }
    break;
    case 0x03u: {
      if (FeedBackEn == 0u)
      {
        if (PulseCnt > 1u)
        {
          FeedBackEn = 0u;
        }
        else
        {
          if ((BusHighTime == 0u) && (BusLowTime == 0u))
          {
            FeedBackEn = 0u;
          }
          else
          {
            PulseCnt++;
            FeedBackEn = 1u;
          }
        }
      }

      TernimalTestSpeed = TERNIMAL_TEST_SPEED_MIN;

      TerminalTestSleepEn = 1u;
    }
    break;
    default:
      TernimalTestSpeed = 0;
      break;
    }

    if (FeedBackEn != 0u)
    {
      if ((FeedBackEn != 0u) && (LastFeedBackEn == 0u))
      {
        BusSts = 0u;
        BusLowTimeTmp = BusLowTime;
        BusHighTimeTmp = BusHighTime;
      }

      if (BusSts == 0u)
      {
        PwmInAPI.CaptureEn = 0u;

        Sdk_setPwmOutPin();

        if (BusLowTimeTmp > 0u)
        {
          BusLowTimeTmp--;
        }
        else
        {
          BusSts = 1u;
          BusLowTimeTmp = BusLowTime;
        }
      }
      else
      {
        Sdk_clrPwmOutPin();

        PwmInAPI.CaptureEn = 1u;

        if (BusHighTimeTmp > 0u)
        {
          BusHighTimeTmp--;
        }
        else
        {
          BusSts = 0u;
          FeedBackEn = 0u;
        }
      }
    }

    LastFeedBackEn = FeedBackEn;
  }
}
