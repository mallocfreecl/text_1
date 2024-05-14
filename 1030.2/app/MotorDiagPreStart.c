
/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or disclosed
* without the written authorization from
* XiangBin Electric.
********************************************************************
* File Name:MotorDiagPreStartState.c
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 电机启动前短路、开路诊断
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
#include "MotorDiagPreStart.h"
#include "DrvAdc.h"
#include "GlobalVars.h"
#include "MotorAPI.h"

/* Defines*/
/******************************************************************/
/* 修改短路、开路诊断阈值，避免误诊断，22.03.30 by yhd */
#define MOTOR_DIAG_PRE_START_PHASE_VOLTAGE_MIN (1u)                          // 0.1V
#define MOTOR_DIAG_PRE_START_PHASE_VOLTAGE_MAX (AdcResults.Voltage.Bat - 3u) // 0.1V

#define MOTOR_DIAG_PRE_START_OPEN_LOAD_VOLTAGE_MIN1 (74u)                          // 0.1V
#define MOTOR_DIAG_PRE_START_OPEN_LOAD_VOLTAGE_MIN2 (AdcResults.Voltage.Bat - 30u) // 0.1V

/* 检测到故障后确认的次数，22.03.30 by yhd */
#define MOTOR_DIAG_PRE_START_FAULT_CONFIRM_CNT_TH (10u)

/* Variables */
/******************************************************************/
BdrvSts_t BDRVDrvSts;
static u8 ShortDiagDone = 0u, OpenDiagDone = 0u;
static u8 FaultConfirmCnt = 0u;

/* Function declaration */
/******************************************************************/
static u8 MotorDiagPreStartShort(void);
static u8 MotorDiagPreStartOpenLoad(void);

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorDiagPreStartState
* Description:
* 1.检测相是否短地、短电源
* 2.在没有相短地、短电源的情况下检测相是否开路
*
* Inputs:
*
*
* Outputs:
* 1.BDRVDrvSts:启动前诊断结果
*
* Limitations:
* 1.在电机启动前调用
*
********************************************************************
END_FUNCTION_HDR*/
u8 MotorDiagPreStartState(void)
{
  u8 DiagDone = 0u;

  BDRVDrvSts.reg = 0u;

  if (ShortDiagDone == 0u)
  {
    ShortDiagDone = MotorDiagPreStartShort();
  }
  /* polyspace +1 DEFECT:USELESS_IF */ /* polyspace +1 MISRA-C3:14.3 */
  if (ShortDiagDone != 0u)
  {
    if (BDRVDrvSts.reg == 0u)
    {
      OpenDiagDone = MotorDiagPreStartOpenLoad();
    }
    else
    {
      OpenDiagDone = 1u;
    }
  }

  DiagDone = (ShortDiagDone != 0u) && (OpenDiagDone != 0u);
  if (DiagDone != 0u)
  {
    ShortDiagDone = 0u;
    OpenDiagDone = 0u;

    /* 检测到故障后多次确认，避免误诊断，22.03.30 by yhd */
    if (BDRVDrvSts.reg != 0u)
    {
      if (FaultConfirmCnt < MOTOR_DIAG_PRE_START_FAULT_CONFIRM_CNT_TH)
      {
        FaultConfirmCnt++;
        DiagDone = 0u;
      }
      else
      {
        FaultConfirmCnt = 0u;
      }
    }
    else
    {
      FaultConfirmCnt = 0u;
    }
  }

  return DiagDone;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorDiagPreStartShort
* Description:
* 检测启动前是否发生相短地、相短电源
*
* Inputs:
*
*
* Outputs:
* 1.BDRVDrvSts.bit.ShortGND：0：正常，1：短路
* 2.BDRVDrvSts.bit.ShortBAT：0：正常，1：短路
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static u8 MotorDiagPreStartShort(void)
{
  u16 PhaseVoltageMin = MOTOR_DIAG_PRE_START_PHASE_VOLTAGE_MIN;
  u16 PhaseVoltageMax = MOTOR_DIAG_PRE_START_PHASE_VOLTAGE_MAX;
  u8 DiagDone = 0u;

  /* 取多次采样结果进行判断，22.03.30 by yhd */
  u8 i = 0u;
  u8 ShortGNDCnt = 0u;
  u8 ShortBATCnt = 0u;

  for (i = 0u; i < DRV_ADC_PHASE_VOLTAGE_BUF_LEN; i++)
  {
    if ((AdcResults.Voltage.UBuf[i] < PhaseVoltageMin) || (AdcResults.Voltage.VBuf[i] < PhaseVoltageMin) ||
        (AdcResults.Voltage.WBuf[i] < PhaseVoltageMin))
    {
      ShortGNDCnt++;
    }

    if ((AdcResults.Voltage.UBuf[i] > PhaseVoltageMax) || (AdcResults.Voltage.VBuf[i] > PhaseVoltageMax) ||
        (AdcResults.Voltage.WBuf[i] > PhaseVoltageMax))
    {
      ShortBATCnt++;
    }
  }
  BDRVDrvSts.bit.ShortGND = (ShortGNDCnt >= DRV_ADC_PHASE_VOLTAGE_BUF_LEN);
  BDRVDrvSts.bit.ShortBAT = (ShortBATCnt >= DRV_ADC_PHASE_VOLTAGE_BUF_LEN);

  DiagDone = 1u;

  return DiagDone;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorDiagPreStartOpenLoad
* Description:
* 启动前开路诊断
*
* Inputs:
*
*
* Outputs:
* 1.BDRVDrvSts.bit.OpenLoadU:0:正常，1：开路
* 2.BDRVDrvSts.bit.OpenLoadV:0:正常，1：开路
* 3.BDRVDrvSts.bit.OpenLoadW:0:正常，1：开路
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static u8 MotorDiagPreStartOpenLoad(void)
{
  u16 PhaseVoltageMin = 0u;
  static u8 Step = 0u;
  u8 DiagDone = 0u;

  /* 取多次采样结果进行判断，22.03.30 by yhd */
  u8 i = 0u;
  u8 OpenLoadUCnt = 0u;
  u8 OpenLoadVCnt = 0u;
  u8 OpenLoadWCnt = 0u;

  if (Step == 0u)
  {
    Step = 1u;

    Sdk_setUHigh();
  }
  else
  {
    Step = 0u;

    Sdk_floatBdrv();

    PhaseVoltageMin = (MOTOR_DIAG_PRE_START_OPEN_LOAD_VOLTAGE_MIN1 < MOTOR_DIAG_PRE_START_OPEN_LOAD_VOLTAGE_MIN2)
                        ? MOTOR_DIAG_PRE_START_OPEN_LOAD_VOLTAGE_MIN1
                        : MOTOR_DIAG_PRE_START_OPEN_LOAD_VOLTAGE_MIN2;

    for (i = 0u; i < DRV_ADC_PHASE_VOLTAGE_BUF_LEN; i++)
    {
      if (AdcResults.Voltage.UBuf[i] < PhaseVoltageMin)
      {
        OpenLoadUCnt++;
      }
      else
      {
        if (AdcResults.Voltage.VBuf[i] < PhaseVoltageMin)
        {
          OpenLoadVCnt++;
        }

        if (AdcResults.Voltage.WBuf[i] < PhaseVoltageMin)
        {
          OpenLoadWCnt++;
        }
      }
    }
    BDRVDrvSts.bit.OpenLoadU = (OpenLoadUCnt >= DRV_ADC_PHASE_VOLTAGE_BUF_LEN);
    BDRVDrvSts.bit.OpenLoadV = (OpenLoadVCnt >= DRV_ADC_PHASE_VOLTAGE_BUF_LEN);
    BDRVDrvSts.bit.OpenLoadW = (OpenLoadWCnt >= DRV_ADC_PHASE_VOLTAGE_BUF_LEN);

    DiagDone = 1u;
  }

  return DiagDone;
}
