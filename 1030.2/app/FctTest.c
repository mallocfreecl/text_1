/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
* disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:FctTest.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.执行FCT测试用于PCBA测试
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
#include "FctTest.h"
#include "DrvAdc.h"
#include "DrvPwmIn.h"
#include "FanComTest.h"
#include "GlobalVars.h"
#include "MotorAPI.h"
#include "Sdk.h"

/* Defines*/
/******************************************************************/
#define FCT_TEST_TIME_BASE (50u) // Unit:ms

#define FCT_TEST_ENTRY_TIME_OUT_TIME (5000u) // Unit:ms

#define FCT_TEST_ENTRY_DATA0 (0x01u)
#define FCT_TEST_ENTRY_DATA4 (0x05u)
#define FCT_TEST_ENTRY_DATA7 (0x08u)

#define FCT_TEST_STEP1_VOLTAGE_SAMPLE_NUM1 (8u)
#define FCT_TEST_PHASE_VOLTAGE_MAX (983u)
#define FCT_TEST_PHASE_VOLTAGE_MIN (173u)

#define FCT_TEST_MOS_UART_TX_DELAY_TIME (200u) // Unit:ms
#define FCT_TEST_TEMPERATURE_OFFSET (80)

#define FCT_TEST_MOS_NORMAL_DUTY (5000u)
#define FCT_TEST_MOS_DUTY_RAMP_STEP (200u)

#define FCT_TEST_STEP1_VOLTAGE_SAMPLE_NUM2 (4u)
#define FCT_TEST_PHASE_VOLTAGE_MAX2 (16u)

/* Variables */
/******************************************************************/
u8 IsInFctTestMode = 0u;
u8 FctTestMOSEn = 0u;
u8 FctTestMOSEnd = 1u;
u8 FctTestSleepEn = 0u;

static u8 FctCmd = 0u;

static u8 FctTestPwmOut50HzEn = 0u;
static u8 FctTestPwmOut100HzEn = 0u;
static u8 FctTestPwmOut100HzEnTmp = 0u;

static u8 IsPhaseVoltageAbnormal = 0u;

static u16 PhaseVoltageUSum = 0;
static u16 PhaseVoltageVSum = 0;
static u16 PhaseVoltageWSum = 0;
static u8 PhaseVoltageUCnt = 0;
static u8 PhaseVoltageVCnt = 0;
static u8 PhaseVoltageWCnt = 0;

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FctTest_step
* Description:
* 1.上电后5s内检测是否进入FCT测试模式，超时后仍未进入FCT测试模式则不再进行
*   检测
*
* Inputs:
*   Uart command
*
* Outputs:
*
*
* Limitations:
* 1.调用周期必须为FCT_TEST_TIME_BASE
********************************************************************
END_FUNCTION_HDR*/
void FctTest_step(void)
{
  static u8 FctEntryTimeOutCnt = (FCT_TEST_ENTRY_TIME_OUT_TIME) / (FCT_TEST_TIME_BASE);
  static u8 FctCmdLast = 0u;
  static u8 FctTxEn = 0u;
  static u8 FctTxID = 0u;
  u8 FctTxBuf[8u] = {0u};
  static u8 StateTimeCnt = 0u;
  static u8 IsFrame3Send = 0u;
  static u8 FrameCnt = 0u;
  u8 i = 0u;

  if ((IsInFctTestMode == 0u) && (FctEntryTimeOutCnt > 0u))
  {
    FctEntryTimeOutCnt--;

    FctCmd = FanComTest_getRxID();
    if ((FctCmd == FAN_COM_TEST_RX_ID_ENTRY_FCT_TEST_MODE) && (FanComTest_readU8(0u) == FCT_TEST_ENTRY_DATA0) &&
        (FanComTest_readU8(4u) == FCT_TEST_ENTRY_DATA4) && (FanComTest_readU8(7u) == FCT_TEST_ENTRY_DATA7))
    {
      IsInFctTestMode = 1u;
      FctEntryTimeOutCnt = 0u;

      MotorAPI_setSpeed(0);
    }
  }
  else
  {
    FctEntryTimeOutCnt = 0u;
  }

  if (IsInFctTestMode != 0u)
  {
    FctCmd = FanComTest_getRxID();

    if ((FctCmd != FAN_COM_TEST_RX_ID_TEST_MOS))
    {
      FctTestMOSEn = 0u;
    }

    if (FctCmd != FAN_COM_TEST_RX_ID_TEST_PWM_OUT)
    {
      FctTestPwmOut50HzEn = 0;
      FctTestPwmOut100HzEn = 0;
      FctTestPwmOut100HzEnTmp = 0;
    }

    switch (FctCmd)
    {
      /*
      Step 1:进入测试模式后反馈数据1~8
      */
    case FAN_COM_TEST_RX_ID_ENTRY_FCT_TEST_MODE: {
      FctTxEn = 1u;
      FctTxID = FAN_COM_TEST_TX_ID_ENTRY_FCT_TEST_MODE;

      FctTxBuf[0u] = 0x01u;
      FctTxBuf[1u] = 0x02u;
      FctTxBuf[2u] = 0x03u;
      FctTxBuf[3u] = 0x04u;
      FctTxBuf[4u] = 0x05u;
      FctTxBuf[5u] = 0x06u;
      FctTxBuf[6u] = 0x07u;
      FctTxBuf[7u] = 0x08u;
    }
    break;
    case FAN_COM_TEST_RX_ID_TEST_MOS: {
      if (FctCmdLast != FAN_COM_TEST_RX_ID_TEST_MOS)
      {
        StateTimeCnt = 0u;
        IsFrame3Send = 0u;
        FrameCnt = 0u;

        if ((PhaseVoltageUCnt != 0u) && (PhaseVoltageVCnt != 0u) && (PhaseVoltageWCnt != 0u))
        {
          u16 VoltU = 0u;
          u16 VoltV = 0u;
          u16 VoltW = 0u;

          VoltU = PhaseVoltageUSum / PhaseVoltageUCnt;
          VoltV = PhaseVoltageVSum / PhaseVoltageVCnt;
          VoltW = PhaseVoltageWSum / PhaseVoltageWCnt;

          if ((VoltU > FCT_TEST_PHASE_VOLTAGE_MAX) || (VoltU < FCT_TEST_PHASE_VOLTAGE_MIN) ||
              (VoltV > FCT_TEST_PHASE_VOLTAGE_MAX) || (VoltV < FCT_TEST_PHASE_VOLTAGE_MIN) ||
              (VoltW > FCT_TEST_PHASE_VOLTAGE_MAX) || (VoltW < FCT_TEST_PHASE_VOLTAGE_MIN))
          {
            IsPhaseVoltageAbnormal = 1u;
          }
          else
          {
            IsPhaseVoltageAbnormal = 0u;
          }
        }
        else
        {
          IsPhaseVoltageAbnormal = 1u;
        }

        PhaseVoltageUSum = 0u;
        PhaseVoltageVSum = 0u;
        PhaseVoltageWSum = 0u;
        PhaseVoltageUCnt = 0u;
        PhaseVoltageVCnt = 0u;
        PhaseVoltageWCnt = 0u;

        if (IsPhaseVoltageAbnormal == 0u)
        {
          FctTestMOSEn = 1u;
          FctTestMOSEnd = 0u;
        }
      }

      if (StateTimeCnt < U8_MAX)
      {
        StateTimeCnt++;
      }

      if (StateTimeCnt > ((FCT_TEST_MOS_UART_TX_DELAY_TIME) / (FCT_TEST_TIME_BASE)))
      {
        FctTxEn = 1u;
        FctTxID = FAN_COM_TEST_TX_ID_TEST_MOS;

        FctTxBuf[0u] = FrameCnt;
        switch (FrameCnt)
        {
        case 0u: {
          FrameCnt = 1u;

          FctTxBuf[1u] = (BarCode[0u] & 0xF0u) | (u8)((((ProductVersion.ProductType >> 16u) & U8_MAX) >> 4u) & 0x0Fu);
          FctTxBuf[2u] = (BarCode[1u] & 0x0Fu) | (u8)((((ProductVersion.ProductType >> 16u) & U8_MAX) & 0x0Fu) << 4u);
          FctTxBuf[3u] = BarCode[2u];
          FctTxBuf[4u] = BarCode[3u];
          FctTxBuf[5u] = BarCode[4u];
          FctTxBuf[6u] = BarCode[5u];
          FctTxBuf[7u] = BarCode[6u];
        }
        break;
        case 1u: {
          FrameCnt = 2u;

          FctTxBuf[1u] = BarCode[7u];
          FctTxBuf[2u] = BarCode[8u];
          FctTxBuf[3u] = BarCode[9u];
          FctTxBuf[4u] = BarCode[10u];
          FctTxBuf[5u] = BarCode[11u];
          /* 旧程序中VS衰减系数为0.055，新程序中为0.039，为了和fct测试兼容进行缩放 */
          FctTxBuf[6u] = (u8)(((u32)AdcResults.RawAD.Bat * 361uL) >> 8u);
          FctTxBuf[7u] = AdcResults.RawAD.VSD;
        }
        break;
        case 2u: {
          FrameCnt = 3u;

          FctTxBuf[1u] = AdcResults.RawAD.VCP;
          FctTxBuf[2u] = AdcResults.RawAD.Mon;
          FctTxBuf[3u] = AdcResults.RawAD.VDDP;
          FctTxBuf[4u] = AdcResults.RawAD.VAREF;
          FctTxBuf[5u] = AdcResults.RawAD.VBG;
          FctTxBuf[6u] = AdcResults.RawAD.VDDC;
          FctTxBuf[7u] = (AdcResults.Temperature.Chip + (FCT_TEST_TEMPERATURE_OFFSET));
        }
        break;
        case 3u: {
          if (IsFrame3Send == 0u)
          {
            FctTxBuf[1u] = (AdcResults.Temperature.Ambient + (FCT_TEST_TEMPERATURE_OFFSET));
            /* polyspace-begin MISRA-C3:10.1 */
            FctTxBuf[2u] = (AdcResults.RawAD.IShunt >> 8u);
            FctTxBuf[3u] = AdcResults.RawAD.IShunt;
            FctTxBuf[4u] = (PwmInAPI.HighDuty >> 8u);
            FctTxBuf[5u] = PwmInAPI.HighDuty & U8_MAX;
            FctTxBuf[6u] =
              ((u8)((EcuFaults.bit.Ram != 0u) || (EcuFaults.bit.DFlash != 0u)) << 7u) |
              ((u8)((EcuFaults.bit.SysOT != 0u) || (EcuFaults.bit.SuppShort != 0u) || (EcuFaults.bit.SuppTimeOut != 0u))
               << 6u) |
              ((u8)((EcuFaults.bit.VDDEXT_OverLoad != 0u) || (EcuFaults.bit.VDDEXT_OverVoltage != 0u) ||
                    (EcuFaults.bit.VDDEXT_Short != 0u) || (EcuFaults.bit.PMU_5V_OV != 0u) ||
                    (EcuFaults.bit.PMU_1V5_OV != 0u))
               << 5u) |
              (((EcuFaults.bit.Ref5VOverLoad != 0u) || (EcuFaults.bit.Ref5VOverVoltage != 0u) ||
                (EcuFaults.bit.Ref5VUnderVoltage != 0u))
               << 4u);
            /* polyspace-end MISRA-C3:10.1 */
            FctTxBuf[7u] = ProductVersion.InnerVersion.bits.InnerVersion;

            IsFrame3Send = 1u;
          }
          else
          {
            FctTxEn = 0u;
            FrameCnt = 0u;
            IsFrame3Send = 0u;
          }
        }
        break;
        default: {
          FctTxEn = 0u;
          FrameCnt = 0u;
        }
        break;
        }

        if (IsPhaseVoltageAbnormal != 0u)
        {
          FctTxBuf[1u] = 0u;
          FctTxBuf[2u] = 0u;
          FctTxBuf[3u] = 0u;
          FctTxBuf[4u] = 0u;
          FctTxBuf[5u] = 0u;
          FctTxBuf[6u] = 0u;
          FctTxBuf[7u] = 0u;
        }
      }
    }
    break;
    case FAN_COM_TEST_RX_ID_TEST_PWM_OUT:
      if (FctCmdLast != FAN_COM_TEST_RX_ID_TEST_PWM_OUT)
      {
        IsPhaseVoltageAbnormal = 0u;
      }

      FctTxEn = 0u;
      if (FctTestPwmOut100HzEn == 0u)
      {
        FctTestPwmOut50HzEn = 1u;
      }
      break;
    case FAN_COM_TEST_RX_ID_TEST_SLEEP:
      FctTxEn = 0u;
      FctTestSleepEn = 1u;
      break;
    default:
      FctTxEn = 0u;
      break;
    }
  }

  if (FctTxEn == 1u)
  {
    FctTxEn = 0u;

    for (i = 0u; i < 8u; i++)
    {
      FanComTest_writeU8(i, FctTxBuf[i]);
    }
    FanComTest_startFrameTx(FctTxID);
  }

  FctCmdLast = FctCmd;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FctTest_testBDRV
* Description:
*   Switch six mos in turn.
*   Switch sequence:HS0->LS1,HS1->LS2,HS2->LS0.
*   Call this function every pwm period.
*
* Inputs:
* 1.FctTestMOSEn:
* FctTestMOSEn == 0 : Set BDRV off.
* FctTestMOSEn != 0 : Switch six mos in turn.
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
void FctTest_testBDRV(void)
{
  static u8 Step = 0u;
  static u16 Duty = 0u;
  u16 i = 0u;

  /* Duty Ramp */
  if (FctTestMOSEn != 0u)
  {
    if (Duty < FCT_TEST_MOS_NORMAL_DUTY)
    {
      Duty += FCT_TEST_MOS_DUTY_RAMP_STEP;
      if (Duty > FCT_TEST_MOS_NORMAL_DUTY)
      {
        Duty = FCT_TEST_MOS_NORMAL_DUTY;
      }
    }
    FctTestMOSEnd = 0u;
  }
  else
  {
    if (Duty > FCT_TEST_MOS_DUTY_RAMP_STEP)
    {
      Duty -= FCT_TEST_MOS_DUTY_RAMP_STEP;
      FctTestMOSEnd = 0u;
    }
    else
    {
      Duty = 0u;
      FctTestMOSEnd = 1u;
    }
  }

  if (FctTestMOSEnd != 0u)
  {
    Step = 0u;
    Duty = 0u;

    SDK_stopBdrvTest();
  }
  else
  {
    switch (Step)
    {
    case 0u:
      Step = 1u;
      Sdk_startBdrvTest();
      Sdk_openBdrvUV();
      break;
    case 1u:
      Sdk_openBdrvUV();
      Step = 2u;
      break;
    case 2u:
      Sdk_openBdrvVW();
      Step = 3u;
      break;
    default:
      Sdk_openBdrvWU();
      Step = 1u;
      break;
    }

    // i = 10000u - Duty;
    i = Duty;
    Sdk_setBdrvDuty(i, i, i);
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FctTest_samplePhaseVoltage
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
void FctTest_samplePhaseVoltage(void)
{
  switch (FctCmd)
  {
  case FAN_COM_TEST_RX_ID_ENTRY_FCT_TEST_MODE: {
    if (PhaseVoltageUCnt < FCT_TEST_STEP1_VOLTAGE_SAMPLE_NUM1)
    {
      PhaseVoltageUSum += AdcResults.RawAD.VoltU;
      PhaseVoltageUCnt++;
    }

    if (PhaseVoltageVCnt < FCT_TEST_STEP1_VOLTAGE_SAMPLE_NUM1)
    {
      PhaseVoltageVSum += AdcResults.RawAD.VoltV;
      PhaseVoltageVCnt++;
    }

    if (PhaseVoltageWCnt < FCT_TEST_STEP1_VOLTAGE_SAMPLE_NUM1)
    {
      PhaseVoltageWSum += AdcResults.RawAD.VoltW;
      PhaseVoltageWCnt++;
    }
  }
  break;
  case FAN_COM_TEST_RX_ID_TEST_PWM_OUT: {
    /* 在MOS停止输出后进行采样，MOS输出关闭存在渐变过程 */
    if (FctTestMOSEnd != 0u)
    {
      if (PhaseVoltageUCnt <= FCT_TEST_STEP1_VOLTAGE_SAMPLE_NUM2)
      {
        PhaseVoltageUSum += AdcResults.RawAD.VoltU;
        PhaseVoltageUCnt++;
      }
      else
      {
        if ((PhaseVoltageUSum / FCT_TEST_STEP1_VOLTAGE_SAMPLE_NUM2) > FCT_TEST_PHASE_VOLTAGE_MAX2)
        {
          IsPhaseVoltageAbnormal = 1u;
          FctTestPwmOut100HzEnTmp = 1u;
        }
      }

      if (PhaseVoltageVCnt <= FCT_TEST_STEP1_VOLTAGE_SAMPLE_NUM2)
      {
        PhaseVoltageVSum += AdcResults.RawAD.VoltV;
        PhaseVoltageVCnt++;
      }
      else
      {
        if ((PhaseVoltageVSum / FCT_TEST_STEP1_VOLTAGE_SAMPLE_NUM2) > FCT_TEST_PHASE_VOLTAGE_MAX2)
        {
          IsPhaseVoltageAbnormal = 1u;
          FctTestPwmOut100HzEnTmp = 1u;
        }
      }

      if (PhaseVoltageWCnt <= FCT_TEST_STEP1_VOLTAGE_SAMPLE_NUM2)
      {
        PhaseVoltageWSum += AdcResults.RawAD.VoltW;
        PhaseVoltageWCnt++;
      }
      else
      {
        if ((PhaseVoltageWSum / FCT_TEST_STEP1_VOLTAGE_SAMPLE_NUM2) > FCT_TEST_PHASE_VOLTAGE_MAX2)
        {
          IsPhaseVoltageAbnormal = 1u;
          FctTestPwmOut100HzEnTmp = 1u;
        }
      }
    }
  }
  break;
  default:
    /* 无动作 */
    break;
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FctTest_genPwmOut50Hz50Duty
* Description:
* 输出50Hz50%波形
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:10ms调用
********************************************************************
END_FUNCTION_HDR*/
void FctTest_genPwmOut50Hz50Duty(void)
{
  static u8 BusSts = 1u;

  if (FctTestPwmOut50HzEn != 0u)
  {
    if (BusSts != 0u)
    {
      BusSts = 0u;
      Sdk_setPwmOutPin();
    }
    else
    {
      BusSts = 1u;
      Sdk_clrPwmOutPin();

      if (FctTestPwmOut100HzEnTmp != 0u)
      {
        FctTestPwmOut50HzEn = 0u;
        FctTestPwmOut100HzEn = 1u;
      }
    }
  }
  else if (FctTestPwmOut100HzEn == 0u)
  {
    Sdk_clrPwmOutPin();
  }
  else
  {
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:FctTest_genPwmOut100Hz50Duty
* Description:
* 输出100Hz50%波形
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:5ms调用
********************************************************************
END_FUNCTION_HDR*/
void FctTest_genPwmOut100Hz50Duty(void)
{
  static u8 BusSts = 1u;

  if (FctTestPwmOut100HzEn == 1u)
  {
    if (BusSts != 0u)
    {
      BusSts = 0u;
      Sdk_setPwmOutPin();
    }
    else
    {
      BusSts = 1u;
      Sdk_clrPwmOutPin();
    }
  }
  else if (FctTestPwmOut50HzEn == 0u)
  {
    Sdk_clrPwmOutPin();
  }
  else
  {
  }
}
