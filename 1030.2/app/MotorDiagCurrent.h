/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or disclosed
* without the written authorization from
* XiangBin Electric.
********************************************************************
* File Name:MotorDiagCurrent.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.检测电机运行时电流是否超过设定阈值
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
* ----------    --------------  ------------   ---------------------
*     1.0       2021-12-10      yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/
#ifndef MOTOR_DIAG_CURRENT_H
#define MOTOR_DIAG_CURRENT_H

/* Includes*/
/******************************************************************/
#include "SdkTypes.h"

/* Variables */
/******************************************************************/
extern u8 IsMotorOverCurrent;

/* Function declaration */
/******************************************************************/
extern void MotorDiagCurrent_init(void);
extern void MotorDiagCurrent_step(void);

#endif // !MOTOR_DIAG_CURRENT_H
