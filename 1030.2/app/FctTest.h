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
#ifndef FCT_TEST_H
#define FCT_TEST_H

/* Includes*/
/******************************************************************/
#include "SdkTypes.h"

/* Variables */
/******************************************************************/
extern u8 IsInFctTestMode;
extern u8 FctTestMOSEn;
extern u8 FctTestMOSEnd;
extern u8 FctTestSleepEn;

/* Function declaration */
/******************************************************************/
extern void FctTest_step(void);
extern void FctTest_testBDRV(void);

extern void FctTest_samplePhaseVoltage(void);
extern void FctTest_genPwmOut50Hz50Duty(void);
extern void FctTest_genPwmOut100Hz50Duty(void);

#endif
