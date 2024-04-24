/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:DTC.h
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
#ifndef DTC_H
#define DTC_H

/* Includes*/
/******************************************************************/
#include "AppTypes.h"

/* Defines*/
/******************************************************************/
typedef enum{
  eDTCIdxBatHigh,
  eDTCIdxBatLow,

  eDTCIdxChipTHigh,/* 2 */
  eDTCIdxChipTLow,

  eDTCIdxMotorShort,
  eDTCIdxMotorOpen,/* 5 */
  eDTCIdxMotorLock,
  eDTCIdxMotorSpeedOutRange,
  eDTCIdxMotorCurrentOutRange,/* 8 */
			
	eDTCIdxECUFaultSysOT,
	eDTCIdxECUFaultPMU_5V_OV,
	eDTCIdxECUFaultPMU_1V5_OV,/* 11 */
	eDTCIdxECUFaultSuppShort,
	eDTCIdxECUFaultSuppTimeOut,
	eDTCIdxECUFaultVDDEXT_OverLoad,/* 14 */
	eDTCIdxECUFaultVDDEXT_OverVoltage,
	eDTCIdxECUFaultVDDEXT_Short,
	eDTCIdxECUFaultRef5VOverLoad,/* 17 */
	eDTCIdxECUFaultRef5VOverVoltage,
	eDTCIdxECUFaultRef5VUnderVoltage,
	eDTCIdxECUFaultVBGError,/* 20 */
	eDTCIdxECUFaultRef5VError,
	eDTCIdxECUFaultVSDHigh,
	eDTCIdxECUFaultVSDLow,/* 23 */
	eDTCIdxECUFaultVCPHigh,
	eDTCIdxECUFaultVCPLow1,
	eDTCIdxECUFaultVCPLow2,/* 26 */
	
	eDTCIdxNVMECCFault,
	eDTCIdxRAMFault,
	eDTCIdxFlashCheckSumError,/* 29 */
	/*���ӵ������,2022-03-10-by zyg*/
	eDTCIdxIShuntZeroError,
	
  eDTCRecordsNum,
}DTCRecordIdx_t;
/*polyspace-begin  MISRA2012:6.1*/
typedef union{
  u16 reg;

	struct
	{
		struct{
			u8 RePowerCnt:7;
			u8 IsFault:1;
		}FaultRePowerInfo;

		u8 FaultTotalCnt;
	}bit;
}DTCRecord_t;
/*polyspace-end  MISRA2012:6.1*/
typedef struct{
  u16 Dummy;
  u16 PowerUpCnt;
	/*��¼�������,2022-03-09-by zyg*/
	u16 IShuntZero;

  DTCRecord_t Records[eDTCRecordsNum];

  u16 CheckSum;
}DTC_t;

/* Variables */
/******************************************************************/
extern DTC_t DTCVar;
extern u16 DTCVarBytesLen;
extern u8 DtcDataSwRstEn;
/* Function declaration */
/******************************************************************/
extern void DTC_Init(void);
extern void DTC_PeriodTask(void);
extern DTCRecord_t DTC_GetRecord(DTCRecordIdx_t Idx);

#endif
