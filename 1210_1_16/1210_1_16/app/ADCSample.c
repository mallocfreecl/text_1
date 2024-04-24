/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:ADCSample.c
********************************************************************
* Project/Product:LXT9818_PMP
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
* Version       Date         	  Author         Descriptions
* ----------    --------------  ------------   ----------------------------------------
*     1.0       2021-07-23      yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#include "ADCSample.h"
#include "common.h"
#include "SDK.h"
#include "Mat.h" 

/* Defines*/
/******************************************************************/
#define BAT_SCALE (1260u)	 /* 1/256*1.23/0.039*1024 */
#define BAT_OFFSET (6u)    /* Unit:0.1V */

#define USE_AMB_T 1u

#define CHIP_T_K1 (480)
#define CHIP_T_K2 (66600)
#define CHIP_T_K3 (231)

#define BAT_FILTER_K (24u) /* BAT_FILTER_K must > 0 */

/*NTC范围立即数改宏定义，24-02-22 by WCchuang*/
#define DRV_ADC_AMB_T_AD_MAX (1003u)
#define DRV_ADC_AMB_T_AD_MIN (10u)
/* Variables */
/******************************************************************/
#if (USE_AMB_T) == 1u
/*
NTC resistor loocup table
ADCSample_NTCTableX:0~1024,ADC count
ADCSample_NTCTableY:-45~150,degree centigrade
*/
#define NTC_TABLE_LEN (40u)
/*消除polyspace警告,2021-09-18-by zyg*/
static const s16 ADCSample_NTCTableX[NTC_TABLE_LEN] =
{
  20, 23, 26, 29, 32, 35, 39, 44, 49, 55,
  62, 71, 80, 90, 103,116,132,151,171,195,
  222,253,287,324,365,410,457,506,558,609,
  660,710,757,800,839,874,904,930,951,967
};
static const s16 ADCSample_NTCTableY[NTC_TABLE_LEN] =
{
  150, 145, 140, 135, 130, 125, 120, 115, 110, 105,
  100, 95,  90,  85,  80,  75,  70,  65,  60,  55,
  50,  45,  40,  35,  30,  25,  20,  15,  10,  5,
  0,   -5,  -10, -15, -20, -25, -30, -35, -40, -45
};
#endif

ADCResults_t ADCResults = {0};
/* Function declaration */
/******************************************************************/
static s16 ADCSample_GetChipT(void);
static void ADCSample_BatVoltageFilter(void);
static TMat_Lp_Simple ADCSample_TempLp = {.CoefA = 100,.CoefB = 100};
s16 ADCSample_Temp = 25;
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:ADCSample_PeriodTask
* Description:
* 1.Read ADC values and converte them to physical value.
* 2.Uptate ADCResults
* 3.Filter battery voltage
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
void ADCSample_PeriodTask(void)
{
  SDK_ADCGetChResult(ADC_ChVSD  , &ADCResults.RawAD.VSD);
  SDK_ADCGetChResult(ADC_ChVCP  , &ADCResults.RawAD.VCP);
  SDK_ADCGetChResult(ADC_ChMON  , &ADCResults.RawAD.Mon);
  SDK_ADCGetChResult(ADC_ChVDDP , &ADCResults.RawAD.VDDP);
  SDK_ADCGetChResult(ADC_ChVDDC , &ADCResults.RawAD.VDDC);
  SDK_ADCGetChResult(ADC_ChVAREF, &ADCResults.RawAD.VAREF);
  SDK_ADCGetChResult(ADC_ChVBG  , &ADCResults.RawAD.VBG);
	
	/*增加温度滤波,减弱温度抖动带来的影响,2022-01-13-by zyg*/
	ADCSample_Temp = ADCSample_GetChipT();
	ADCResults.Temperature.Chip = Mat_ExeLp_without_min_max(&ADCSample_TempLp, ADCSample_Temp);
  ADCResults.Temperature.Ambient = ADCSample_GetAmbT();

  ADCResults.Voltage.Bat = ADCSample_GetBat();
  ADCSample_BatVoltageFilter();
	
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:ADCSample_GetAmbT
* Description:
* 1.Get ambient temperature
*
* Inputs:none
*
*
* Outputs:degree centigrade
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
s16 ADCSample_GetAmbT(void)
{
	s16 AmbT = 0;/*初始化变量,2021-09-16-by zyg*/
  SDK_ADCGetChResult(ADC_ChAmbT, &ADCResults.RawAD.AmbT);
#if USE_AMB_T == 1
  u16 NTC_X;

  NTC_X = ADCResults.RawAD.AmbT;
	/*NTC范围立即数改宏定义，24-02-22 by WCchuang*/
	if((NTC_X >= DRV_ADC_AMB_T_AD_MIN)&&(NTC_X <= DRV_ADC_AMB_T_AD_MAX))/* 0.1v - 4.9v */
	{
		ADCResults.IsAmbTInvalid = 0u;
		
		AmbT = Common_ExeLookUpTable(NTC_X,
										((const s16 *)&(ADCSample_NTCTableX[0u])),
										((const s16 *)&(ADCSample_NTCTableY[0u])),
										NTC_TABLE_LEN - 1u);
	}
	else
	{
		ADCResults.IsAmbTInvalid = 1u;
		/*NTC错误时使用芯片温度,2022-02-22-by WChuang*/
		AmbT = ADCResults.Temperature.Chip;
		
	}
#else
  AmbT = ADCResults.Temperature.Chip;
#endif
									
  return AmbT;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:ADCSample_GetChipT
* Description:
* 1.Get chip temperature
*
* Inputs:none
*
*
* Outputs:degree centigrade
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static s16 ADCSample_GetChipT(void)
{
  s16 T_Degree;

	SDK_ADCGetChResult(ADC_ChChipT, &ADCResults.RawAD.ChipT);
  T_Degree = ((s32)ADCResults.RawAD.ChipT*(CHIP_T_K1)-(CHIP_T_K2))/(CHIP_T_K3);

  return T_Degree;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:ADCSample_BatVoltageFilter
* Description:
* 1.Filter battery voltage
*
* Inputs:
* 1.ADCResults.Voltage.Bat
*
* Outputs:
* 1.ADCResults.Voltage.BatFiltered
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static void ADCSample_BatVoltageFilter(void)
{
  static s16 FilterDiff = 0;
  static u16 Time = 0u;
  static u8 IsBatFilteredLow = 1u;
  u16 FilterDiffAbs = 0;
  
  FilterDiffAbs = ABS(FilterDiff);

  if(ADCResults.Voltage.BatFiltered == 0xFFFFu)
  {
    ADCResults.Voltage.BatFiltered = ADCResults.Voltage.Bat;
  }
  else
  {
    ADCResults.Voltage.BatFiltered = (ADCResults.Voltage.Bat + ADCResults.Voltage.BatFiltered * (BAT_FILTER_K - 1))/(BAT_FILTER_K);
  }

  FilterDiff = (s16)ADCResults.Voltage.Bat - (s16)ADCResults.Voltage.BatFiltered;
  if(FilterDiff <= 0)
  {
    if(IsBatFilteredLow == 0u)
    {
      Time = 0u;
    }
    else
    {
     
    }

    IsBatFilteredLow = 1u;
  }
  else
  {
    if(IsBatFilteredLow == 1u)
    {
      Time = 0u;
    }
    else
    {
      
    }
    
    IsBatFilteredLow = 0u;
  }

  
  if(FilterDiffAbs < 5u)
  {
    Time = 0u;
  }
  else if(FilterDiffAbs > 30u)
  {
    Time = 0u;
    ADCResults.Voltage.BatFiltered = ADCResults.Voltage.Bat;
  }
  else
  {
    
  }

  if(Time < 400u)
  {
    Time++;
  }
  else
  {
    Time = 0u;
    ADCResults.Voltage.BatFiltered += FilterDiff;
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:ADCSample_GetBat
* Description:
* 1.Get battery voltage
*
* Inputs:none
*
*
* Outputs:voltage
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
u16 ADCSample_GetBat(void)
{
  u16 Voltage;

	SDK_ADCGetChResult(ADC_ChBat, &ADCResults.RawAD.Bat);
	
  Voltage = (((u32)ADCResults.RawAD.Bat*BAT_SCALE)>>10u) + BAT_OFFSET;

  return Voltage;
}
