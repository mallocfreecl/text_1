/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:TestMode.h
********************************************************************
* Project/Product:LXT9818
* Title:
* Author:yhd
*********************************************************************
* Description:
*   1.Uart debug
*   2.Fct test
*   3.Terminal test
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
#ifndef TEST_MODE_H
#define TEST_MODE_H

/* Includes*/
/******************************************************************/
#include "AppTypes.h"

/* Variables */
/******************************************************************/
extern u8 IsInTestMode;
extern u8 FctTestMOSEn;
extern u8 FctTestPwmOut5HzEn;
/*消除polyspace警告,2021-09-22-by zyg*/
extern u8 IsInFctTestMode;
extern u8 IsInTerminalTestMode;
/* Function declaration */
/******************************************************************/
extern void TestMode_UartDebugPeriodTask(void);
extern void TestMode_TestMOS(void);
extern void TestMode_FactoryTest(void);

#endif
