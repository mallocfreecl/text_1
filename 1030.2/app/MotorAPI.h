/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
* disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:MotorAPI.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.电机控制接口
* 2.电机堵转诊断
* 3.电机故障恢复
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
#ifndef MOTOR_API_H
#define MOTOR_API_H

/* Includes*/
/******************************************************************/
#include "SdkTypes.h"
#include "Mc.h"

/* Defines*/
/******************************************************************/
#define MOTOR_API_PERIOD_TASK_TIME_BASE (10u) // ms

//typedef enum
//{
//  eMUnInit = 0,
//  eMStop,
//  eMOpenLoop,
//  eMCloseLoop,
//  eMFault,
//} MotorState_t;

/* polyspace-begin MISRA-C3:6.1 */
typedef union {
  u16 Reg;

  struct
  {
    u16 Short : 1;
    u16 Open : 1;
    u16 Lock : 1;
  } bit;
} MotorFaults_t;
/* polyspace-end MISRA-C3:6.1 */

/* polyspace-begin MISRA-C3:6.1 */
typedef union {
  u32 Reg;

  struct
  {
    u32 OffShortGND : 1;
    u32 OffShortBAT : 1;
    u32 OffOpenLoad : 1;

    u32 OnShortGND : 1;
    u32 OnShortBAT : 1;
    u32 SpeedOutRange : 1;
    u32 OverCurrent : 1;
    u32 BEMFAmpOutRange : 1;
  } bit;
} MotorDetailFaults_t;
/* polyspace-end MISRA-C3:6.1 */

typedef struct
{
  s16 RefSpeed;
  s16 ActSpeed;
  s16 ActSpeedFilted;
  s16 WmSpeed;

  MotorState_t State;
  MotorState_t StateLast;

  u8 RestartEn;
  u8 RestartComplete;

  volatile MotorFaults_t Faults;
  volatile MotorDetailFaults_t DetailFaults;

  u16 PwmOutAmp;
  u16 BEMFAmp;

  u32 CloseLoopTime;

  s32 IShunt;
  u16 IShuntPeak;
} MotorAPI_t;

/* Variables */
/******************************************************************/
extern MotorAPI_t MotorAPI;

/* Function declaration */
/******************************************************************/
extern void MotorAPI_init(void);
extern void MotorAPI_setSpeed(s16 RefSpeed);
extern void MotorAPI_step(void);

#endif // !MOTOR_API_H
