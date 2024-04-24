/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:MotorAPI.h
********************************************************************
* Project/Product:LXT9820
* Title:
* Author:zyg
*********************************************************************
* Description:
*
* (Requirements, pseudo code and etc.)
*********************************************************************
* Limitations:
*
* (limitations)
*********************************************************************
*********************************************************************
* Revision History��
*
* Version       Date         	  Author         Descriptions
* ----------    --------------  ------------   ----------------------------------------
*     1.0       2021-09-06        zyg            Original
*
*
********************************************************************
*END_FILE_HDR*/

#ifndef MOTOR_API_H
#define MOTOR_API_H

/* Includes*/
/******************************************************************/
#include "AppTypes.h"
/* Defines*/
/******************************************************************/
/*polyspace-begin  MISRA2012:6.1*/
typedef union{
  u16 Reg;

  struct{
    u16 Short:1;
    u16 Open:1;
    u16 Lock:1;
  }bit;
}MotorFaults_t;

typedef union{
  u32 Reg;

  struct{
    u32 MotorOffShortGND:1;
    u32 MotorOffShortBAT:1;
    u32 MotorOffOpenLoad:1;

    u32 MotorOnShortGND:1;
    u32 MotorOnShortBAT:1;
    u32 MotorOnSpeedOutRange:1;
    u32 MotorOnOverCurrent:1;		
  }bit;
}MotorDetailFaults_t;

typedef union{
	u8 reg;
	
	struct{
		u8 ShortBAT:1;
		u8 ShortGND:1;
	
		u8 OpenLoadCh0:1;
		u8 OpenLoadCh1:1;
		u8 OpenLoadCh2:2;
	}bit;
}BDRVOffDiagResult_t;
/*polyspace-end  MISRA2012:6.1*/
typedef struct{
  s16 RefSpeed;
  s16 ActSpeed;

  u8 IsDryRunningDetected;
	u8 IsDryRunningProcessCheck;/*是否执行过程检标志 22.09.25 by lxs*/
	u8 IsDryRunningProcessCheckFlag;/*过程检结果是否为干运行标志 22.09.25 by lxs*/
	u8 IsEnterProcessCheck;/*进入过程检标志 22.09.25 by lxs*/
  u8 IsDryRunning;
  volatile MotorFaults_t Faults;/* 故障计数 */
  volatile MotorDetailFaults_t DetailFaults;/* 故障标志 */
	volatile MotorDetailFaults_t DetailFaults1;/* 调试 */

  volatile u8 MotorOffDiagEn;
  volatile u8 MotorOffDiagCompleted;
  u8 MotorOffDiagResult;

  volatile u16 OpenLoadDiagCurrent;
  
  u8 RestartEn;
  u8 RestartComplete;

  u16 SpeedOutRangeTime;
  u16 OverCurrentTime;
	u16 StopTime;/*LIN传过来的期望转速为0时间 22.09.25 by lxs*/
  u32 DryRunningCurrentLowTime;
  u32 DryRunningCurrentHighTime;
	
	u32 CloseLoopTime;	

  s32 IShunt; 
}MotorAPIVar_t;
/* Variables */
/******************************************************************/
extern MotorAPIVar_t MotorAPIVar;
/*����polyspace����,2021-09-22-by zyg*/
extern volatile s32 DryRunningCurrent;
extern s32 DryRunningCurrentTHR;
extern u8 Lin_Error_State;
extern BDRVOffDiagResult_t BDRV_OffDiagResult;
/* Function declaration */
/******************************************************************/
extern void MotorAPI_MotorInit(void);
extern void MotorAPI_MotorSetSpeed(s16 Speed);

extern void MotorAPI_MotorPeriodTask(void);

extern void MotorAPI_MotorOffDiag(void);

extern void MotorAPI_BatStatus_Diag(void);

#endif /* !MOTOR_API_H */
