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
#ifndef TERMINAL_TEST_H
#define TERMINAL_TEST_H

/* Includes*/
/******************************************************************/
#include "SdkTypes.h"

/* Variables */
/******************************************************************/
extern u8 IsInTerminalTestMode;
extern s16 TernimalTestSpeed;
extern u8 TerminalTestSleepEn;

/* Function declaration */
/******************************************************************/
extern void TerminalTest_step(void);

#endif
