/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
* disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:Sdk.c
********************************************************************
* Project/Product:t3622
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
* Version       Date         	  Author       Descriptions
* ----------    --------------  ------------   ------------
* 1.0           2021-12-09      yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#include "Sdk.h"
#include "Common.h"
#include "DrvAdc.h"
#include "SdkCfg.h"

#include "adc1.h"
#include "bdrv.h"
#include "ccu6.h"
#include "tle_device.h"

/* polyspace-begin MISRA-C3:17.3 */

/* Defines*/
/******************************************************************/
#define SDK_VDDEXT_TOGGLE_TIME (8u)

#define PWM_OUT_PIN (PORT->P1_DATA.bit.P4)

#define SDK_IG_VALID_TH (29u)

#define SDK_BDRV_DUTY_MAX (10000u)

#define SDK_RAM_TEST_NUM1 (0x55555555uL)
#define SDK_RAM_TEST_NUM2 (~(SDK_RAM_TEST_NUM1))

#define SDK_REF5V_RST_DELAY_TH (2u)
/* 对电压策略误差进行补偿，22.03.20 by yhd */
#define SDK_REF5V_RST_VOLT_TH (75u - 2u)

#define SDK_VSD_HIGH_TH (5u)
#define SDK_VSD_LOW_TH (5u)
#define SDK_VCP_HIGH_TH (5u)
#define SDK_VCP_LOW1_TH (0u)
#define SDK_VCP_LOW2_TH (0u)

/* Variables */
/******************************************************************/
#if SDK_EXE_TIME_MEASURE_EN
IsrTime_t IsrTime;
#endif
ECUFaults_t EcuFaults;
u32 CFlashChkSumCalc = 0u;

static s32 int_was_mask = 0;

/* Function declaration */
/******************************************************************/
static void Sdk_switchOnVDDEXT(void);
static void Sdk_setWakeUpSrc(void);
static void Sdk_diagMem(void);
static u8 Sdk_chkDFlash(void);
static u8 Sdk_chkRam(void);
static u8 Sdk_chkCFlash(void);

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_initHw
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
void Sdk_initHw(void)
{
  /* 执行RAM校验时需关闭全局中断 22.11.14 by jxj*/
  Sdk_enterCritical();
  Sdk_diagMem();
  Sdk_exitCritical();

  TLE_Init();

  Sdk_enterCritical();

  Sdk_initPwmOut();
  Sdk_setWakeUpSrc();
  Sdk_switchOnVDDEXT();
  CSA_Set_Gain(SdkCfg.IShuntGain);

  GPT12E_T2_Start();
  CCU6_StartTmr_T12();

  Sdk_exitCritical();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_switchOnVDDEXT
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
static void Sdk_switchOnVDDEXT(void)
{
  u8 i = 0u;
  for (i = 0u; i < SDK_VDDEXT_TOGGLE_TIME; i++)
  {
    PMU->VDDEXT_CTRL.reg = 0u;
    PMU->VDDEXT_CTRL.reg = 1u;
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_setWakeUpSrc
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
static void Sdk_setWakeUpSrc(void)
{
  /* polyspace-begin DEFECT:DEAD_CODE [No action planned:Unset] "正确的用法"*/
  /* polyspace-begin MISRA-C3:2.1 */
  switch (SdkCfg.WakeUpSrc)
  {
  case eWakeUpSrcPwmIn:
    PMU->LIN_WAKE_EN.reg = 0x80u;
    MON->CNF.bit.EN = 0;
    MON->CNF.bit.FALL = 0;
    MON->CNF.bit.RISE = 0;
    break;
  case eWakeUpSrcIG:
    PMU->LIN_WAKE_EN.reg = 0x00u;
    MON->CNF.bit.EN = 1;
    MON->CNF.bit.FALL = 0;
    MON->CNF.bit.RISE = 1;
    break;
  default:
    PMU->LIN_WAKE_EN.reg = 0x80u;
    MON->CNF.bit.EN = 1;
    MON->CNF.bit.FALL = 0;
    MON->CNF.bit.RISE = 1;
    break;
  }
  /* polyspace-end MISRA-C3:2.1 */
  /* polyspace-end DEFECT:DEAD_CODE [No action planned:Unset] "正确的用法"*/
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_feedWDog
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
void Sdk_feedWDog(void)
{
  WDT1_Service();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_enterSleep
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
void Sdk_enterSleep(void)
{
  SCU_EnterSleepMode();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_rstChip
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
void Sdk_rstChip(void)
{
  /* triger the wdt1 reset */
  SCUPM->WDT1_TRIG.reg = (u8)0x00000100u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:
* Description:Sdk_enterCritical
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
void Sdk_enterCritical(void)
{
  int_was_mask = CMSIS_Irq_Dis();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:
* Description:Sdk_exitCritical
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
void Sdk_exitCritical(void)
{
  if (int_was_mask == 0)
  {
    CMSIS_Irq_En();
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_setPwmOutPin
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
void Sdk_setPwmOutPin(void)
{
  PWM_OUT_PIN = 1u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_clrPwmOutPin
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
void Sdk_clrPwmOutPin(void)
{
  PWM_OUT_PIN = 0u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_togglePwmOutPin
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
void Sdk_togglePwmOutPin(void)
{
  /* polyspace +1 MISRA-C3:10.1 */
  PWM_OUT_PIN = !PWM_OUT_PIN;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:
* Description:Sdk_initPwmOut
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
void Sdk_initPwmOut(void)
{
  /* polyspace-begin MISRA-C3:2.1,14.3 */
  /* polyspace-begin DEFECT:DEAD_CODE [No action planned:Unset] "正确的用法"*/
  if (SdkCfg.PwmOutDefaultSts != 0u)
  {
    Sdk_setPwmOut();
  }
  else
  {
    Sdk_clrPwmOut();
  }
  /* polyspace-end DEFECT:DEAD_CODE [No action planned:Unset] "正确的用法"*/
  /* polyspace-end MISRA-C3:2.1,14.3 */
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_setPwmOut
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
void Sdk_setPwmOut(void)
{
  PWM_OUT_PIN = 0u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:
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
void Sdk_clrPwmOut(void)
{
  PWM_OUT_PIN = 1u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_writeUartU8
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
u8 Sdk_readUartU8(void)
{
  return (UART2->SBUF.reg);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_writeUartU8
* Description:
*
*
* Inputs:
*
*
* Outputs:
* 0:Fail,1:Pass
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
u8 Sdk_writeUartU8(u8 Data)
{
  u8 ret = 0u;

  UART2_Send_Byte(Data);
  ret = 1u;

  return ret;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_getAdcRawData
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
void Sdk_getAdcRawData(AdcCh_t Ch, u16 *pData)
{
  u16 ADVal = 0u;
  u8 Valid = 1u;

  /* polyspace +1 MISRA-C3:10.1 */
  switch (Ch >> 4u)
  {
  case 1u:
    /* polyspace +1 MISRA-C3:10.1 */
    Valid = (u8)ADC1_GetChResult(&ADVal, Ch & 0xFu);
    break;
  case 2u:
    Valid = 1u;
    /* polyspace +1 MISRA-C3:10.1 */
    ADVal = ADC2_GetChResult(Ch & 0xFu) >> 2u;
    break;
  default:
    Valid = 1u;
    switch (Ch)
    {
    case eAdcVoltDc:
      ADVal = ADC1->RES_OUT6.bit.OUT_CH6;
      break;
    case eAdcVoltU:
      ADVal = ADC1->RES_OUT0.bit.OUT_CH0;
      break;
    case eAdcVoltV:
      ADVal = ADC1->RES_OUT2.bit.OUT_CH2;
      break;
    case eAdcVoltW:
      ADVal = ADC1->RES_OUT3.bit.OUT_CH3;
      break;
    case eAdcIShunt:
      ADVal = ADC1->RES_OUT1.bit.OUT_CH1;
      break;
    default:
      Valid = 0u;
      break;
    }
    break;
  }

  if (Valid != 0u)
  {
    *pData = ADVal;
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_getIGSts
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
u8 Sdk_getIGSts(void)
{
  return (AdcResults.RawAD.Mon >= SDK_IG_VALID_TH);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_getPwmInCapVal
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
u8 Sdk_isPwmInCap(void)
{
  return TIMER2->T2CON.bit.EXF2;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_getPwmInCapVal
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
u16 Sdk_getPwmInCapVal(void)
{
  return TIMER2_Capture_Value_Get();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_setPwmInCapRisingEdge
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
void Sdk_setPwmInCapRisingEdge(void)
{
  TIMER2->T2MOD.bit.EDGESEL = 1u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_setPwmInCaptureFallingEdge
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
void Sdk_setPwmInCaptureFallingEdge(void)
{
  TIMER2->T2MOD.bit.EDGESEL = 0u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_setPwmInCaptureFallingEdge
* Description:
*
*
* Inputs:
*
*
* Outputs:
* 0u:Falling, 1u:Rising
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
u8 Sdk_getPwmInCapEdge(void)
{
  return TIMER2->T2MOD.bit.EDGESEL;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_isPwmInIsCapTmrRunning
* Description:
*
*
* Inputs:
*
*
* Outputs:
* 0u:stopped, 1u:running
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
u8 Sdk_isPwmInIsCapTmrRunning(void)
{
  return TIMER2->T2CON.bit.TR2;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_rstPwmInCapTmr
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
void Sdk_rstPwmInCapTmr(void)
{
  TIMER2_Stop();
  TIMER2_Clear_Count();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:
* Description:
*
*
* Inputs:
*
*
* Outputs:
* 0u:Low, 1u:High
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
u8 Sdk_getPwmInBusSts(void)
{
  return LIN->CTRL_STS.bit.RXD;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_openBdrv
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
void Sdk_openBdrv(void)
{
  BDRV_Set_Bridge(Ch_PWM, Ch_PWM, Ch_PWM, Ch_PWM, Ch_PWM, Ch_PWM);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_closeBdrv
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
void Sdk_closeBdrv(void)
{
  BDRV_Set_Bridge(Ch_Off, Ch_Off, Ch_Off, Ch_Off, Ch_Off, Ch_Off);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_floatBdrv
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
void Sdk_floatBdrv(void)
{
  BDRV_Set_Bridge(Ch_En, Ch_En, Ch_En, Ch_En, Ch_En, Ch_En);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_clrBdrvError
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
void Sdk_clrBdrvError(void)
{
  /* Disable output pattern */
  CCU6_Multi_Ch_PWM_Shadow_Reg_Load(0x0000u);
  /* Enable Shadow Transfer Request for MCMPS */
  CCU6_MCM_PWM_Str_SW_En();
  /* Clear all Error and Warning Flags */
  BDRV_Clr_Sts(0x1F1FFC00u);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_setBdrvDuty
* Description:
*
*
* Inputs:
* Pwm compare val
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
void Sdk_setBdrvDuty(u16 DutyU, u16 DutyV, u16 DutyW)
{
  if (DutyU > SDK_BDRV_DUTY_MAX)
  {
    DutyU = SDK_BDRV_DUTY_MAX;
  }
  DutyU = (u32)DutyU * CCU6_T12PR / SDK_BDRV_DUTY_MAX;

  if (DutyV > SDK_BDRV_DUTY_MAX)
  {
    DutyV = SDK_BDRV_DUTY_MAX;
  }
  DutyV = (u32)DutyV * CCU6_T12PR / SDK_BDRV_DUTY_MAX;

  if (DutyW > SDK_BDRV_DUTY_MAX)
  {
    DutyW = SDK_BDRV_DUTY_MAX;
  }
  DutyW = (u32)DutyW * CCU6_T12PR / SDK_BDRV_DUTY_MAX;

  CCU6_LoadShadowRegister_CC60(DutyU);
  CCU6_LoadShadowRegister_CC61(DutyV);
  CCU6_LoadShadowRegister_CC62(DutyW);
  CCU6_EnableST_T12();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_startBdrvTest
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
void Sdk_startBdrvTest(void)
{
  CSA->CTRL.bit.GAIN = 3U;
  CCU6->T12DTC.bit.DTM = 30u;
  BDRV->CTRL3.bit.DRV_CCP_TIMSEL = 2u; // 800ns

  CCU6_StopTmr_T12();
  Sdk_setBdrvDuty(SDK_BDRV_DUTY_MAX, SDK_BDRV_DUTY_MAX, SDK_BDRV_DUTY_MAX);
  CCU6_StartTmr_T12();

  Sdk_clrBdrvError();
  Sdk_openBdrv();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_openBdrvUV
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
void Sdk_openBdrvUV(void)
{
  CCU6->MODCTR.bit.MCMEN = 1u;
  CCU6->MODCTR.bit.T12MODEN = 0x03U;
  CCU6->MCMOUTS.reg = (uint16)(0x0BU | CCU6_MASK_MCMOUTS_SHADOW_OUT);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_openBdrvVW
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
void Sdk_openBdrvVW(void)
{
  CCU6->MODCTR.bit.MCMEN = 1u;
  CCU6->MODCTR.bit.T12MODEN = 0x30U;
  CCU6->MCMOUTS.reg = (uint16)(0x32U | CCU6_MASK_MCMOUTS_SHADOW_OUT);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_openBdrvVW
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
void Sdk_openBdrvWU(void)
{
  CCU6->MODCTR.bit.MCMEN = 1u;
  CCU6->MODCTR.bit.T12MODEN = 0x0CU;
  CCU6->MCMOUTS.reg = (uint16)(0x2CU | CCU6_MASK_MCMOUTS_SHADOW_OUT);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_stopBdrvTest
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
void SDK_stopBdrvTest(void)
{
  Sdk_closeBdrv();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_setULow
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
void Sdk_setULow(void)
{
  Sdk_clrBdrvError();

  BDRV_Set_Bridge(Ch_On, Ch_En, Ch_En, Ch_En, Ch_En, Ch_En);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_setULow
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
void Sdk_setUHigh(void)
{
  Sdk_clrBdrvError();

  BDRV_Set_Bridge(Ch_En, Ch_On, Ch_En, Ch_En, Ch_En, Ch_En);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_diagBdrvOnShort
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
u8 Sdk_diagBdrvOnShort(BdrvSts_t *DrvSts)
{
  u8 IsFinished = 0u;

  if (((BDRV->CTRL1.reg & 0x40400000u) > 0u) || ((BDRV->CTRL2.reg & 0x00004000u) > 0u))
  {
    DrvSts->bit.ShortGND = 1u;
  }
  else
  {
    DrvSts->bit.ShortGND = 0u;
  }

  if (((BDRV->CTRL1.reg & 0x00004040u) > 0u) || ((BDRV->CTRL2.reg & 0x00000040u) > 0u))
  {
    DrvSts->bit.ShortBAT = 1u;
  }
  else
  {
    DrvSts->bit.ShortBAT = 0u;
  }

  IsFinished = 1u;

  return IsFinished;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_startMotorHw
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
void Sdk_startMotorHw(void)
{
  u16 Duty = SDK_BDRV_DUTY_MAX >> 1u;

  CCU6_StopTmr_T12();
  Sdk_setBdrvDuty(Duty, Duty, Duty);
  CCU6_StartTmr_T12();

  Sdk_clrBdrvError();
  Sdk_openBdrv();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_stopMotorHw
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
void Sdk_stopMotorHw(void)
{
  Sdk_floatBdrv();

  /* ADC1 ch0、2、3 , T12 Period-Match (T12_PM)*/
  ADC1->CHx_ESM.bit.ESM_0 = 0x0Du;
  CCU6->TCTR2.bit.T13TEC = 0x5u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_getDFlashStartAddr
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
Addr_t Sdk_getDFlashStartAddr(void)
{
  return (Addr_t)(SdkCfg.DFlashStartAddr);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_getDFlashBackUpAddr
* Description:获取DFlash备份数据地址地址
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
Addr_t Sdk_getDFlashBackUpAddr(void)
{
  return (Addr_t)(SdkCfg.DFlashBackUpAddr);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_writeDFlash
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
void Sdk_writeDFlash(Addr_t Addr, u8 *pData, u16 DataLen)
{
  Addr_t OffSet = 0u;

  while (DataLen != 0u)
  {
    GPT12E_T2_Stop();
    ProgramPage(Addr + OffSet, pData, 0u, 1u, 0u);
    GPT12E_T2_Start();

    if (DataLen >= 128u)
    {
      DataLen -= 128u;
      OffSet += 128u;
    }
    else
    {
      DataLen = 0u;
    }
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_readDFlash
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
void Sdk_readDFlash(Addr_t Addr, u8 *pData, u16 DataLen)
{
  for (u16 i = 0u; i < DataLen; i++)
  {
    pData[i] = *((u8 *)(Addr + i));
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_eraseDFlash
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
void Sdk_eraseDFlash(Addr_t Addr, u16 DataLen)
{
  /* TLE987x只有一个DFlash Sector,所以只能擦除这个Sector，22.04.16 by yhd */
  /* 增加关闭T2中断  22.06.08 by jxj*/
  GPT12E_T2_Stop();
  USER_ERASE_SECTOR(SdkCfg.DFlashStartAddr);
  GPT12E_T2_Start();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_eraseDFlash
* Description:ERROM出错时擦除整个sector
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
void Sdk_clrDFlashError(Addr_t Addr, u16 DataLen)
{
  Sdk_eraseDFlash(Addr, DataLen);
  EcuFaults.bit.DFlash = Sdk_chkDFlash();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_diagMem
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
static void Sdk_diagMem(void)
{
  EcuFaults.bit.Ram = Sdk_chkRam();
  EcuFaults.bit.CFlash = Sdk_chkCFlash();
  EcuFaults.bit.DFlash = Sdk_chkDFlash();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_chkRam
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
static u8 Sdk_chkRam(void)
{
  u32 ret = SDK_RAM_TEST_NUM1;
  static vu32 *RamTestAddr;
  static vu32 RamTestAddrVal;

  /* polyspace-begin MISRA-C3:2.1,14.3 */
  /* polyspace +1 DEFECT:DEAD_CODE [No action planned:Unset] "正确的用法"*/
  if (ret != SDK_RAM_TEST_NUM1)
  {
    return 1u;
  }
  ret = SDK_RAM_TEST_NUM2;
  /* polyspace +1 DEFECT:DEAD_CODE [No action planned:Unset] "正确的用法"*/
  if (ret != SDK_RAM_TEST_NUM2)
  {
    return 1u;
  }
  ret = 0u;
  /* polyspace-end MISRA-C3:2.1,14.3 */

  RamTestAddrVal = SDK_RAM_TEST_NUM1;
  ret = (ret != 0u) || (RamTestAddrVal != SDK_RAM_TEST_NUM1);
  RamTestAddrVal = SDK_RAM_TEST_NUM2;
  ret = (ret != 0u) || (RamTestAddrVal != SDK_RAM_TEST_NUM2);

  RamTestAddr = (vu32 *)SDK_RAM_TEST_NUM1;
  ret = (ret != 0u) || (RamTestAddr != (vu32 *)SDK_RAM_TEST_NUM1);
  RamTestAddr = (vu32 *)SDK_RAM_TEST_NUM2;
  ret = (ret != 0u) || (RamTestAddr != (vu32 *)SDK_RAM_TEST_NUM2);

  if (ret == 0u)
  {
    for (RamTestAddr = ((vu32 *)SdkCfg.RamStartAddr); (RamTestAddr < ((vu32 *)SdkCfg.RamEndAddr)) && (ret == 0u);
         RamTestAddr++)
    {
      /* polyspace +2 MISRA-C3:11.3 */
      if ((RamTestAddr != ((vu32 *)(&RamTestAddr))) && (RamTestAddr != (vu32 *)(&RamTestAddrVal)) &&
          (RamTestAddr != (vu32 *)(&ret)))
      {
        RamTestAddrVal = (*RamTestAddr);
        (*RamTestAddr) = SDK_RAM_TEST_NUM1;
        ret = (ret != 0u) || ((*RamTestAddr) != SDK_RAM_TEST_NUM1);
        (*RamTestAddr) = SDK_RAM_TEST_NUM2;
        ret = (ret != 0u) || ((*RamTestAddr) != SDK_RAM_TEST_NUM2);
        (*RamTestAddr) = RamTestAddrVal;

        Sdk_feedWDog();
      }
    }
  }

  return ret;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_chkDFlash
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
static u8 Sdk_chkDFlash(void)
{
  /*增加EEROM故障判断条件 22.04.23 by jxj*/
  return (USER_NVM_ECC_CHECK() != 0u) || (USER_MAPRAM_INIT() != 0u);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_chkCFlash
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
static u8 Sdk_chkCFlash(void)
{
  u8 ret = 0u;

  CFlashChkSumCalc = Common_calcCrc16Ccitt((const u8 *)SdkCfg.CFlashStartAddr, (const u8 *)SdkCfg.CFlashEndAddr);

  if (CFlashChkSumCalc != CFlashChkSumStored)
  {
    ret = 1u;
  }

  return ret;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_diagEcuFault
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
void Sdk_diagEcuFault(void)
{
  static u16 VsdHighCnt = 0u;
  static u16 VsdLowCnt = 0u;
  static u16 VcpHighCnt = 0u;
  static u16 VcpLow1Cnt = 0u;
  static u16 VcpLow2Cnt = 0u;
  static u16 Ref5VRstDelayCnt = 0u;

  EcuFaults.bit.SysOT = PMU->SYS_FAIL_STS.bit.SYS_OT;
  EcuFaults.bit.PMU_5V_OV = PMU->SYS_FAIL_STS.bit.PMU_5V_OVL;
  EcuFaults.bit.PMU_1V5_OV = PMU->SYS_FAIL_STS.bit.PMU_1V5_OVL;
  EcuFaults.bit.SuppShort = PMU->SYS_FAIL_STS.bit.SUPP_SHORT;
  EcuFaults.bit.SuppTimeOut = PMU->SYS_FAIL_STS.bit.SUPP_TMOUT;

  EcuFaults.bit.VDDEXT_OverLoad = PMU->VDDEXT_CTRL.bit.OVERLOAD;
  EcuFaults.bit.VDDEXT_OverVoltage = PMU->VDDEXT_CTRL.bit.OVERVOLT;
  EcuFaults.bit.VDDEXT_Short = PMU->VDDEXT_CTRL.bit.SHORT;

  if ((EcuFaults.bit.VDDEXT_OverLoad != 0u) || (EcuFaults.bit.VDDEXT_OverVoltage != 0u) ||
      (EcuFaults.bit.VDDEXT_Short != 0u))
  {
    PMU->VDDEXT_CTRL.bit.SHORT = 0u;
    PMU->VDDEXT_CTRL.bit.ENABLE = 1u;
  }

  EcuFaults.bit.Ref5VOverLoad = MF->REF2_CTRL.bit.VREF5V_OVL_STS;
  EcuFaults.bit.Ref5VOverVoltage = MF->REF2_CTRL.bit.VREF5V_OV_STS;
  EcuFaults.bit.Ref5VUnderVoltage = MF->REF2_CTRL.bit.VREF5V_UV_STS;

  if (EcuFaults.bit.Ref5VUnderVoltage != 0u)
  {
    /* 改正判断条件错误，22.03.30 by yhd */
    if (Ref5VRstDelayCnt > SDK_REF5V_RST_DELAY_TH)
    {
      if (AdcResults.Voltage.Bat > SDK_REF5V_RST_VOLT_TH)
      {
        Sdk_rstChip();
      }
    }
    else
    {
      Ref5VRstDelayCnt++;
    }
  }
  else
  {
    Ref5VRstDelayCnt = 0u;
  }

  EcuFaults.bit.VBGError = (MF->REF1_STS.bit.REFBG_LOTHWARN_STS != 0u) || (MF->REF1_STS.bit.REFBG_UPTHWARN_STS != 0u);

  EcuFaults.bit.VSDHigh = Common_setStatusDelay(SCUPM->BDRV_IS.bit.VSD_UPTH_STS, SDK_VSD_HIGH_TH, &VsdHighCnt);
  if (SCUPM->BDRV_IS.bit.VSD_UPTH_STS == 1u)
  {
    SCUPM->BDRV_ISCLR.bit.VSD_UPTH_SCLR = 1u;
    SCUPM->BDRV_ISCLR.bit.VSD_UPTH_ICLR = 1u;
  }

  EcuFaults.bit.VSDLow = Common_setStatusDelay(SCUPM->BDRV_IS.bit.VSD_LOWTH_STS, SDK_VSD_LOW_TH, &VsdLowCnt);
  if (SCUPM->BDRV_IS.bit.VSD_LOWTH_STS == 1u)
  {
    SCUPM->BDRV_ISCLR.bit.VSD_LOWTH_SCLR = 1u;
    SCUPM->BDRV_ISCLR.bit.VSD_LOWTH_ICLR = 1u;
  }

  EcuFaults.bit.VCPHigh = Common_setStatusDelay(SCUPM->BDRV_IS.bit.VCP_UPTH_STS, SDK_VCP_HIGH_TH, &VcpHighCnt);
  if (SCUPM->BDRV_IS.bit.VCP_UPTH_STS == 1u)
  {
    SCUPM->BDRV_ISCLR.bit.VCP_UPTH_SCLR = 1u;
    SCUPM->BDRV_ISCLR.bit.VCP_UPTH_ICLR = 1u;
  }

  EcuFaults.bit.VCPLow1 = Common_setStatusDelay(SCUPM->BDRV_IS.bit.VCP_LOWTH1_STS, SDK_VCP_LOW1_TH, &VcpLow1Cnt);
  if (SCUPM->BDRV_IS.bit.VCP_LOWTH1_STS == 1u)
  {
    SCUPM->BDRV_ISCLR.bit.VCP_LOWTH1_SCLR = 1u;
    SCUPM->BDRV_ISCLR.bit.VCP_LOWTH1_ICLR = 1u;
  }

  EcuFaults.bit.VCPLow2 = Common_setStatusDelay(SCUPM->BDRV_IS.bit.VCP_LOWTH2_STS, SDK_VCP_LOW2_TH, &VcpLow2Cnt);
  if (SCUPM->BDRV_IS.bit.VCP_LOWTH2_STS == 1u)
  {
    SCUPM->BDRV_ISCLR.bit.VCP_LOWTH2_SCLR = 1u;
    SCUPM->BDRV_ISCLR.bit.VCP_LOWTH2_ICLR = 1u;
  }

  EcuFaults.bit.ECUFault = (EcuFaults.bit.Ref5VOverLoad != 0u) || (EcuFaults.bit.Ref5VOverVoltage != 0u) ||
                           (EcuFaults.bit.Ref5VUnderVoltage != 0u) || (EcuFaults.bit.VSDHigh != 0u) ||
                           (EcuFaults.bit.VSDLow != 0u) || (EcuFaults.bit.VCPHigh != 0u) ||
                           (EcuFaults.bit.VCPLow1 != 0u) || (EcuFaults.bit.VCPLow2 != 0u) ||
                           ((AdcResults.ErrFlags.AmbTOutRange != 0u) &&
                            ((PMU->VDDEXT_CTRL.bit.STABLE != 0u) || (AdcResults.Temperature.Chip > 150))) ||
                           (EcuFaults.bit.SysOT != 0u);
}

/* polyspace-end MISRA-C3:17.3 */
