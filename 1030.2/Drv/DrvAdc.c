/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
* disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:DrvAdc.c
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 采集ADC信号并转换为对应的物理值。对ADC的硬件操作在SDK.c文件中实现，该文件
* 只实现采集的控制、采集结果的计算
*
* (Requirements, pseudo code and etc.)
*********************************************************************
* Limitations:
* 1.该文件内的函数必须在完成ADC硬件初始化后进行调用
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
#include "DrvAdc.h"
#include "Common.h"
#include "Mat.h"
#include "Sdk.h"
#include "SdkCfg.h"

#include "adc1.h"
#include "tle_device.h"

/* Defines*/
/******************************************************************/
#define DRV_ADC_USE_NTC (1u)
#define DRV_ADC_NTC_TAB_LEN (40u)

#define DRV_ADC_BAT_LP_COEF (2000)

#define DRV_ADC_ISHUNT_ZERO_WAIT_CNT_TH (200u)

#define DRV_ADC_CHIP_T_AD_TO_DEGREE_CENTIGRADE(AD) (((s32)(AD)*480 - 66600L) / 231)

#define DRV_ADC_AMB_T_AD_MIN (10u)
#define DRV_ADC_AMB_T_AD_MAX (1003u)

#define DRV_ADC_PHASE_AD_TO_VOLTAGE(AD) ((u16)(((u32)(AD)*5 * 6 * 10) >> 12))

#define DRV_ADC_BAT_SCALE (1260u) // 1/256*1.23/0.039*1024
#define DRV_ADC_BAT_OFFSET (6u)   // Unit:0.1V
#define DRV_ADC_BAT_AD_TO_VOLTAGE(AD) ((((u32)(AD) * (DRV_ADC_BAT_SCALE)) >> 10u) + (DRV_ADC_BAT_OFFSET))

/* 母线电流和AD值互相转换 22.08.17 by jxj */
/* Gain = 10, RShunt=0.001Ohm */
#define DRV_ADC_ISHUNT_AD_TO_AMP(AD) ((AD) / 1000 * 4076 / 1000)
#define DRV_ADC_ISHUNT_AMP_TO_AD(Amp) ((s32)(Amp)*24534 / 100)

/* 母线电流低通滤波参数 22.08.17 by jxj */
#define DRV_ADC_ISHUNT_LP_COEF (2000)
#define DRV_ADC_ISHUNT_LP_MAX (2000)
#define DRV_ADC_ISHUNT_LP_MIN (-2000)

/* Variables */
/******************************************************************/
#if (DRV_ADC_USE_NTC) == 1u
/*
NTC resistor lookup table
NTCTabX:ADC count
NTCTabY:degree centigrade
*/
static const s16 NTCTabX[DRV_ADC_NTC_TAB_LEN] = {20,  23,  26,  29,  32,  35,  39,  44,  49,  55,  62,  71,  80,  90,
                                                 103, 116, 132, 151, 171, 195, 222, 253, 287, 324, 365, 410, 457, 506,
                                                 558, 609, 660, 710, 757, 800, 839, 874, 904, 930, 951, 967};
static const s16 NTCTabY[DRV_ADC_NTC_TAB_LEN] = {150, 145, 140, 135, 130, 125, 120, 115, 110, 105, 100, 95, 90, 85,
                                                 80,  75,  70,  65,  60,  55,  50,  45,  40,  35,  30,  25, 20, 15,
                                                 10,  5,   0,   -5,  -10, -15, -20, -25, -30, -35, -40, -45};
#endif

AdcResults_t AdcResults;
static TMat_Lp BatLp;
/* 母线电流滤波 22。08.17 by jxj */
static TMat_Lp ShuntLp;

/* Function declaration */
/******************************************************************/
static s16 DrvAdc_getChipT(void);
static s16 DrvAdc_getAmbT(void);
static u16 DrvAdc_getBat(void);
/* ADC转母线电流 22.08.17 by jxj */
static void DrvAdc_calIShunt(void);
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:ADCSample_Init
* Description:
* 1.设置Bat电压滤波参数，滤波输出初值
* 2.执行ADCSample_periodTask采集所有的ADC
* 3.标定母线电流采样运放零点
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
* 1.在初始化时调用该函数
* 2.调用该函数前必须完成DTC初始化
********************************************************************
END_FUNCTION_HDR*/
void DrvAdc_init(void)
{
  /* Bat voltage low pass filter */
  BatLp.CoefA = DRV_ADC_BAT_LP_COEF;
  BatLp.CoefB = BatLp.CoefA;
  BatLp.Min = 0;
  BatLp.Max = S16_MAX;

  DrvAdc_step();

  AdcResults.Voltage.BatFilt = AdcResults.Voltage.Bat;  //??
  /* polyspace +1 MISRA-C3:10.1 */
  BatLp.Out = (s32)AdcResults.Voltage.BatFilt << 15u;
  /*母线电流低通滤波参数 22.08.17 by jxj*/
  ShuntLp.CoefA = DRV_ADC_ISHUNT_LP_COEF;
  ShuntLp.CoefB = ShuntLp.CoefA;
  ShuntLp.Min = DRV_ADC_ISHUNT_LP_MIN;
  ShuntLp.Max = DRV_ADC_ISHUNT_LP_MAX;
  ShuntLp.Out = 0;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:DrvAdc_calcIShuntZero
* Description:
* 1.采集母线电流采样运放零点值
*
* Inputs:
*
*
* Outputs:
* 1.AdcResults.RawAD.IShuntZero：电流零点AD值
* 2.返回值：1：错误，0：正常
*
* Limitations:
* 1.该函数应在DTC初始化后调用
********************************************************************
END_FUNCTION_HDR*/
u8 DrvAdc_calcIShuntZero(void)
{
  u8 ret = 0u;
  u8 WaitCnt = 0u;

  ADC1_Software_Mode_Sel();

  /* wait in case ADC1 is still busy */
  while ((ADC1_Busy() == true) && (ret == 0u))
  {
    WaitCnt++;
    ret = (WaitCnt > DRV_ADC_ISHUNT_ZERO_WAIT_CNT_TH);
  }
  WaitCnt = 0u;

  if (ret == 0u)
  {
    /* set Ch1 (CSA) for software conversion mode */    //CSA采用软件模式采样
    ADC1_SW_Ch_Sel(ADC1_CSA);
    /* start conversion */
    ADC1_SOC_Set();

    /* wait until conversion is done */
    while ((ADC1_isEndOfConversion() == false) && (ret == 0u))
    {
      WaitCnt++;
      ret = (WaitCnt > DRV_ADC_ISHUNT_ZERO_WAIT_CNT_TH);
    }

    if (ret == 0u)
    {
      Sdk_getAdcRawData(eAdcIShunt, &AdcResults.RawAD.IShuntZero);
    }
  }

  ADC1_Sequencer_Mode_Sel();

  return ret;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:ADCSample_periodTask
* Description:
* 1.采集芯片内部信号AD值
* 2.采集片内、片外温度
* 3.采集供电电压
* 4.计算供电电压的滤波值
* 5.计算相电压
*
* Inputs:none
*
*
* Outputs:AdcResults
*
*
* Limitations:
* 1.周期性调用该函数进行ADC采集
********************************************************************
END_FUNCTION_HDR*/
void DrvAdc_step(void)
{
  Sdk_getAdcRawData(eAdcVSD, &AdcResults.RawAD.VSD);
  Sdk_getAdcRawData(eAdcVCP, &AdcResults.RawAD.VCP);
  Sdk_getAdcRawData(eAdcMON, &AdcResults.RawAD.Mon);
  Sdk_getAdcRawData(eAdcVDDP, &AdcResults.RawAD.VDDP);
  Sdk_getAdcRawData(eAdcVDDC, &AdcResults.RawAD.VDDC);
  Sdk_getAdcRawData(eAdcVAREF, &AdcResults.RawAD.VAREF);
  Sdk_getAdcRawData(eAdcVBG, &AdcResults.RawAD.VBG);

  AdcResults.Temperature.Chip = DrvAdc_getChipT();
  AdcResults.Temperature.Ambient = DrvAdc_getAmbT();

  AdcResults.Voltage.Bat = DrvAdc_getBat();
  AdcResults.Voltage.BatFilt = Mat_ExeLp(&BatLp, AdcResults.Voltage.Bat);

  DrvAdc_calPhaseVoltage();
  /*母线电流读取+低通滤波 22.08.17 by jxj*/
  DrvAdc_calIShunt();
  if (AdcResults.Current.Shunt == 0)
  {
    AdcResults.Current.ShuntFlt = 0;
    ShuntLp.Out = 0;
  }
  else
  {
    AdcResults.Current.ShuntFlt = Mat_ExeLp(&ShuntLp, AdcResults.Current.Shunt);
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:DrvAdc_getChipT
* Description:
* 获取芯片内部温度AD值并转换为实际温度
*
* Inputs:none
*
*
* Outputs:
* 1.AdcResults.RawAD.ChipT：片内温度AD值
* 2.T_Degree：片内温度，单位：摄氏度
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static s16 DrvAdc_getChipT(void)
{
  s16 T_Degree = 0;

  Sdk_getAdcRawData(eAdcChipT, &AdcResults.RawAD.ChipT);
  T_Degree = DRV_ADC_CHIP_T_AD_TO_DEGREE_CENTIGRADE(AdcResults.RawAD.ChipT);

  return T_Degree;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:DrvAdc_getAmbT
* Description:
* 1.采集外部NTC电阻上电压的AD值，并转换为实际温度
* 2.如果不使用外部NTC电阻，则使用芯片内部温度作为环境温度
*
* Inputs:none
*
*
* Outputs:
* 1.AdcResults.RawAD.AmbT：片外温度AD值
* 2.AmbT：片外温度，单位：摄氏度
* 3.AdcResults.ErrFlags.AmbTOutRange：片外温度采样值错误标志
*   1：错误，0：正常
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static s16 DrvAdc_getAmbT(void)
{
  s16 AmbT = 0;
#if DRV_ADC_USE_NTC == 1
  u16 NTC_X = 0;

  Sdk_getAdcRawData(eAdcAmbT, &AdcResults.RawAD.AmbT);

  NTC_X = AdcResults.RawAD.AmbT;

  if ((NTC_X >= DRV_ADC_AMB_T_AD_MIN) && (NTC_X <= DRV_ADC_AMB_T_AD_MAX))
  {
    AdcResults.ErrFlags.AmbTOutRange = 0u;

    AmbT = Common_exeLookUpTable(NTC_X, NTCTabX, NTCTabY, DRV_ADC_NTC_TAB_LEN - 1u);
  }
  else
  {
    AdcResults.ErrFlags.AmbTOutRange = 1u;
  }
#else
  AmbT = AdcResults.Temperature.Chip;
#endif

  return AmbT;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:DrvAdc_getBat
* Description:
* 采集电源电压AD并转换为电压值
*
* Inputs:none
*
*
* Outputs:
* 1.AdcResults.RawAD.Bat：电压电压AD值
* 2.Voltage：电源电压，单位：0.1V
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static u16 DrvAdc_getBat(void)
{
  u16 Voltage = 0u;

  Sdk_getAdcRawData(eAdcBat, &AdcResults.RawAD.Bat);

  Voltage = DRV_ADC_BAT_AD_TO_VOLTAGE(AdcResults.RawAD.Bat);

  return Voltage;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:ADCSample_GetPhaseVoltage
* Description:
* 获取相电压AD值并转换为实际电压值
*
* Inputs:none
*
*
* Outputs:
* 1.AdcResults.RawAD.VoltU、V、W:相电压AD值
* 1.AdcResults.Voltage.U、V、W：相电压，单位：0.1V
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
void DrvAdc_calPhaseVoltage(void)
{
  /* 计算相电压的多次历史电压值，22.03.30 by yhd */
  u8 i = 0u;

  AdcResults.Voltage.U = DRV_ADC_PHASE_AD_TO_VOLTAGE(AdcResults.RawAD.VoltU);
  AdcResults.Voltage.V = DRV_ADC_PHASE_AD_TO_VOLTAGE(AdcResults.RawAD.VoltV);
  AdcResults.Voltage.W = DRV_ADC_PHASE_AD_TO_VOLTAGE(AdcResults.RawAD.VoltW);

  for (i = 0u; i < DRV_ADC_PHASE_VOLTAGE_BUF_LEN; i++)
  {
    AdcResults.Voltage.UBuf[i] = DRV_ADC_PHASE_AD_TO_VOLTAGE(AdcResults.RawAD.VoltUBuf[i]);
    AdcResults.Voltage.VBuf[i] = DRV_ADC_PHASE_AD_TO_VOLTAGE(AdcResults.RawAD.VoltVBuf[i]);
    AdcResults.Voltage.WBuf[i] = DRV_ADC_PHASE_AD_TO_VOLTAGE(AdcResults.RawAD.VoltWBuf[i]);
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:DrvAdc_calIShunt
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
static void DrvAdc_calIShunt(void)
{
  s32 IShunt = 0;

  IShunt = DRV_ADC_ISHUNT_AD_TO_AMP(AdcResults.RawAD.IShunt);
  /* polyspace-begin DEFECT:DEAD_CODE */ /* polyspace-begin MISRA-C3:10.1*/
  switch (SdkCfg.IShuntGain)  //电流增益
  {
  case eIShuntGain20:
    IShunt >>= 1u;
    break;
  case eIShuntGain40:
    IShunt >>= 2u;
    break;
  case eIShuntGain60:
    IShunt /= 6;
    break;
  default:
    /* Gain=10 */
    break;
  }
  /* polyspace-end DEFECT:DEAD_CODE */ /* polyspace-end MISRA-C3:10.1*/

  AdcResults.Current.Shunt = IShunt;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:DrvAdc_convIShuntAmpToAD
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
s32 DrvAdc_convIShuntAmpToAD(s16 Amp)
{
  s32 Ad = 0;

  Ad = DRV_ADC_ISHUNT_AMP_TO_AD(Amp);
  /* polyspace-begin DEFECT:DEAD_CODE */ /* polyspace-begin MISRA-C3:10.1*/
  switch (SdkCfg.IShuntGain)
  {
  case eIShuntGain20:
    Ad <<= 1u;
    break;
  case eIShuntGain40:
    Ad <<= 2u;
    break;
  case eIShuntGain60:
    Ad *= 6;
    break;
  default:
    /* Gain=10 */
    break;
  }
  /* polyspace-end DEFECT:DEAD_CODE */ /* polyspace-end MISRA-C3:10.1*/
  return Ad;
}
