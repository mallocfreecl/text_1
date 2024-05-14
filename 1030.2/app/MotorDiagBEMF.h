/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or disclosed
* without the written authorization from
* XiangBin Electric.
********************************************************************
* File Name:MotorDiagBEMF.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.根据反电势判断电机是否失步
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
#ifndef MOTOR_DIAG_BEMF_H
#define MOTOR_DIAG_BEMF_H

/* Includes*/
/******************************************************************/
#include "SdkTypes.h"

/* Variables */
/******************************************************************/
extern u8 IsMotorBEMFAbnormal;

/* Function declaration */
/******************************************************************/
extern void MotorDiagBEMF_init(void);
extern void MotorDiagBEMF_step(void);

#endif // !MOTOR_DIAG_BEMF_H
