/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:ADCSample.h
********************************************************************
* Project/Product:LXT9818
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
#ifndef ADC_SAMPLE_H
#define ADC_SAMPLE_H

/* Includes*/
/******************************************************************/
#include "AppTypes.h"

/* Defines*/
/******************************************************************/
typedef struct{
  struct{
    u16 Bat;
    u16 BatFiltered;
  }Voltage; /* unit:0.1V */

  struct{
    s16 U;
    s16 V;
    s16 W;

    s16 DC;
  }Current; /* unit:0.1A */

  struct{
    s16 Ambient;
    s16 Chip;
  }Temperature; /* degree centigrade */

  struct{
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
		

    struct{
      u16 Zero;
			u16 DC;
    }Current;
  }RawAD; /* adc count */
	
	u8 IsAmbTInvalid;
	
	/* polyspace-begin MISRA-C3:6.1 */
  struct
  {
    u8 IShuntZero : 1;
    u8 AmbTOutRange : 1;
  } ErrFlags;
  /* polyspace-end MISRA-C3:6.1 */
}ADCResults_t;

/* Variables */
/******************************************************************/
extern ADCResults_t ADCResults;
extern s16 ADCSample_Temp;
/* Function declaration */
/******************************************************************/
extern void ADCSample_PeriodTask(void);
extern u16 ADCSample_GetBat(void);
extern s16 ADCSample_GetAmbT(void);


#endif
