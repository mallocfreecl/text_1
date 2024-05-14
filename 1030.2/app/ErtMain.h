/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
*disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:ErtMain.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.初始化硬件
* 2.初始化软件
* 3.轮询执行周期性任务
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
#ifndef ERT_MAIN_H
#define ERT_MAIN_H

/* Includes*/
/******************************************************************/
#include "SdkTypes.h"

/* Defines*/
/******************************************************************/
/* 1ms计数，在1ms中断中增加该计数 */
extern vu16 Ms1;

/* Function declaration */
/******************************************************************/
extern void ErtMain(void);

#endif
