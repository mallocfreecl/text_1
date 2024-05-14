/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or disclosed
* without the written authorization from
* XiangBin Electric.
********************************************************************
* File Name:MotorDiagSpeed.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.根据实际转速判断电机是否失步
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
#ifndef MOTOR_DIAG_SPEED_H
#define MOTOR_DIAG_SPEED_H

/* Includes*/
/******************************************************************/
#include "SdkTypes.h"

/* Defines*/
/******************************************************************/
/* polyspace-begin MISRA-C3:6.1 */
typedef union {
  u8 reg;

  struct
  {
    u8 SpeedOutRange : 1;
    u8 SpeedReverse : 1;
    u8 SpeedAccOutRange : 1;
  } bits;
} MotorSpeedAbnormalFlag_t;
/* polyspace-end MISRA-C3:6.1 */

/* Variables */
/******************************************************************/
extern u8 IsMotorSpeedAbnormal;
extern MotorSpeedAbnormalFlag_t MotorSpeedAbnormalFlag;

/* Function declaration */
/******************************************************************/
extern void MotorDiagSpeed_init(void);
extern void MotorDiagSpeed_step(void);

#endif // !MOTOR_DIAG_SPEED_H
