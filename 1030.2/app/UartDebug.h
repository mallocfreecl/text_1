/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
* disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:UartDebug.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.执行串口调试命令
* 2.执行FCT测试用于PCBA测试
* 3.执行终端测试用于电机总成测试
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
#ifndef UART_DEBUG_H
#define UART_DEBUG_H

/* Includes*/
/******************************************************************/
#include "SdkTypes.h"

/* Function declaration */
/******************************************************************/
extern void UartDebug_step(void);

#endif
