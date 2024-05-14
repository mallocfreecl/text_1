/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or disclosed
* without the written authorization from
* XiangBin Electric.
********************************************************************
* File Name:MotorDiagPreStartState.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 电机启动前短路、开路诊断
*
* (Requirements, pseudo code and etc.)
*********************************************************************
* Limitations:
*
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
#ifndef MOTOR_DIAG_PRESTART_H
#define MOTOR_DIAG_PRESTART_H

/* Includes*/
/******************************************************************/
#include "Sdk.h"
#include "SdkTypes.h"

/* Variables */
/******************************************************************/
extern BdrvSts_t BDRVDrvSts;

/* Function declaration */
/******************************************************************/
extern u8 MotorDiagPreStartState(void);

#endif // !MOTOR_DIAG_PRESTART_H
