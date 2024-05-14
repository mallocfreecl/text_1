/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
* disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:DrvAdc.h
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
#ifndef DRV_ADC_H
#define DRV_ADC_H

/* Includes*/
/******************************************************************/
#include "SdkTypes.h"

/* Defines*/
/******************************************************************/
/* Temperature uint:1 degrees centigrade */
#define DRV_ADC_I_SHUNT_ZERO_CALIBRATE_AMBT_MIN (-40)
#define DRV_ADC_I_SHUNT_ZERO_CALIBRATE_AMBT_MAX (150)
/* Voltage unit:0.1V */
#define DRV_ADC_I_SHUNT_ZERO_CALIBRATE_BAT_MIN (70u)
#define DRV_ADC_I_SHUNT_ZERO_CALIBRATE_BAT_MAX (220u)

#define DRV_ADC_ISHUNT_ZERO_DEFAULT (1660u)
#define DRV_ADC_ISHUNT_ZERO_MIN (1500u)
#define DRV_ADC_ISHUNT_ZERO_MAX (1800u)

/* 相电压历史数据个数，22.03.30 by yhd */
#define DRV_ADC_PHASE_VOLTAGE_BUF_LEN (6u)

typedef struct
{
  struct
  {
    u16 Bat;
    u16 BatFilt;
    u16 U;
    u16 V;
    u16 W;
    /* 相电压历史数据个数，22.03.30 by yhd */
    u16 UBuf[DRV_ADC_PHASE_VOLTAGE_BUF_LEN];
    u16 VBuf[DRV_ADC_PHASE_VOLTAGE_BUF_LEN];
    u16 WBuf[DRV_ADC_PHASE_VOLTAGE_BUF_LEN];
  } Voltage; // unit:0.1V

  struct
  {
    s16 U;
    s16 V;
    s16 W;

    s16 Shunt;
    /* 滤波后的母线电流 22.08.17 by jxj */
    s16 ShuntFlt;
    s16 PhasePeak;
  } Current; // unit:0.1A

  struct
  {
    s16 Ambient;
    s16 Chip;
  } Temperature; // degree centigrade

  struct
  {
    u16 Bat;
    u16 VSD;
    u16 VCP;
    u16 Mon;
    u16 VDDP;
    u16 VDDC;
    u16 VAREF;
    u16 VBG;
    u16 ChipT;
    u16 AmbT;
    u16 VoltDc;
    u16 VoltU;
    u16 VoltV;
    u16 VoltW;
    /* 相电压历史数据个数，22.03.30 by yhd */
    u16 VoltUBuf[DRV_ADC_PHASE_VOLTAGE_BUF_LEN];
    u16 VoltVBuf[DRV_ADC_PHASE_VOLTAGE_BUF_LEN];
    u16 VoltWBuf[DRV_ADC_PHASE_VOLTAGE_BUF_LEN];
    u16 IShuntZero;
    s32 IShunt;
    s16 IShuntPeak;
    s16 IShuntPeakTmp;
    s16 Iu;
    s16 Iv;
    s16 Iw;
    s16 IuAbs;
    s16 IvAbs;
    s16 IwAbs;
  } RawAD; // adc count

  /* polyspace-begin MISRA-C3:6.1 */
  struct
  {
    u8 IShuntZero : 1;
    u8 AmbTOutRange : 1;
  } ErrFlags;
  /* polyspace-end MISRA-C3:6.1 */

} AdcResults_t;

/* Variables */
/******************************************************************/
extern AdcResults_t AdcResults;

/* Function declaration */
/******************************************************************/
extern void DrvAdc_init(void);
extern void DrvAdc_step(void);
extern u8 DrvAdc_calcIShuntZero(void);
extern void DrvAdc_calPhaseVoltage(void);
/* 母线电流转AD值 22.08.17 by jxj */
extern s32 DrvAdc_convIShuntAmpToAD(s16 Amp);

#endif
