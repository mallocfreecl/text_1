/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:MotorAPI.h
********************************************************************
* Project/Product:LXT9818
* Title:
* Author:yhd
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
* Revision History：
*
* Version       Date         	  Author         Descriptions
* ----------    --------------  ------------   ----------------------------------------
*     1.0       2021-07-23      yhd            Original
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
    u32 MotorOnShortGNDCL:1;
    u32 MotorOnShortBATCL:1;
    u32 MotorOnShortGNDOL:1;
    u32 MotorOnShortBATOL:1;
		
    u32 MotorOnSpeedOutRange:1;
	u32 MotorOnSpeedOutRange_OverSpeed:1;
	u32 MotorOnSpeedOutRange_UnderSpeed:1;
	u32 MotorOnSpeedOutRange_DryRunning:1;
	u32 MotorOnSpeedOutRange_ReverseSpeed:1;
	u32 MotorOnSpeedOutRange_DiffSpeedAndCurrent:1;
		
    u32 MotorOnOverCurrent:1;		
  }bit;
}MotorDetailFaults_t;
typedef union{
  u8 Reg;
  struct{
    u8 MotorOnSpeedOutRange_OverSpeed:1;
		u8 MotorOnSpeedOutRange_UnderSpeed:1;
		u8 MotorOnSpeedOutRange_DryRunning:1;
		u8 MotorOnSpeedOutRange_ReverseSpeed:1;
		u8 MotorOnSpeedOutRange_DiffSpeedAndCurrent:1;
  }bit;
}MotorSpeedOutRangeReason_t;

typedef union{
	u8 reg;
  struct
  {
    u8 CSAZeroCalibrationFault : 1;
    u8 CSAZeroAdcBusyError : 1;
    u8 CSAZeroAdcEocError : 1;
    u8 CSAZeroHigh : 1;
    u8 CSAZeroLow : 1;
    u8 CSAZeroSampleCntErrorr : 1;
    u8 CSAZeroVoltOrTempError : 1;
  } bit;
} CSAZeroFault_t;
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
  u8 IsDryRunning;
	u8 DryRunningRestartFlag;
  volatile MotorFaults_t Faults;
  volatile MotorDetailFaults_t DetailFaults;
	volatile MotorDetailFaults_t DetailFaults1;

  volatile u8 MotorOffDiagEn;
  volatile u8 MotorOffDiagCompleted;
  u8 MotorOffDiagResult;

  volatile u16 OpenLoadDiagCurrent;
  
  u8 RestartEn;
  u8 RestartComplete;

  u16 SpeedOutRangeTime;
  u16 OverCurrentTime;
	u16 StopTime;
  u32 DryRunningCurrentLowTime;
  u32 DryRunningCurrentHighTime;
	
	u32 CloseLoopTime;	

  s32 IShunt; 
}MotorAPIVar_t;
/* Variables */
/******************************************************************/
extern MotorAPIVar_t MotorAPIVar;
/*消除polyspace警告,2021-09-22-by zyg*/
extern volatile s32 DryRunningCurrent;
extern s32 DryRunningCurrentTHR;
extern u16 MotorAPI_FeedBackCnt;
extern u16 MotorAPI_FeedBackClearComplete;
extern BDRVOffDiagResult_t BDRV_OffDiagResult;
extern MotorSpeedOutRangeReason_t SpeedOutRangeReason;
extern CSAZeroFault_t CSAZeroFault;
/* Function declaration */
/******************************************************************/
extern void MotorAPI_MotorInit(void);
extern void MotorAPI_MotorSetSpeed(s16 Speed);

extern void MotorAPI_MotorPeriodTask(void);


extern void MotorAPI_BatStatus_Diag(void);

#endif /* !MOTOR_API_H */
