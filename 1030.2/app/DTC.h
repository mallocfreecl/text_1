/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
*disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:DTC.h
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.在Data Flash中记录故障信息
* 2.在Data Flash中记录电流零点
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
#ifndef DTC_H
#define DTC_H

/* Includes*/
/******************************************************************/
#include "SdkTypes.h"

/* Defines*/
/******************************************************************/
typedef enum
{
  eDTCIdxBatHigh,
  eDTCIdxBatLow,
  eDTCIdxChipTHigh,
  eDTCIdxChipTLow,
  eDTCIdxMotorShort,
  eDTCIdxMotorOpen,
  eDTCIdxMotorLock,
  eDTCIdxMotorSpeedOutRange, // 7

  eDTCIdxMotorCurrentOutRange,
  eDTCIdxECUFaultSysOT,
  eDTCIdxECUFaultPMU_5V_OV,
  eDTCIdxECUFaultPMU_1V5_OV,
  eDTCIdxECUFaultSuppShort,
  eDTCIdxECUFaultSuppTimeOut,
  eDTCIdxECUFaultVDDEXT_OverLoad,
  eDTCIdxECUFaultVDDEXT_OverVoltage, // 15

  eDTCIdxECUFaultVDDEXT_Short,
  eDTCIdxECUFaultRef5VOverLoad,
  eDTCIdxECUFaultRef5VOverVoltage,
  eDTCIdxECUFaultRef5VUnderVoltage,
  eDTCIdxECUFaultVBGError,
  eDTCIdxECUFaultRef5VError,
  eDTCIdxECUFaultVSDHigh,
  eDTCIdxECUFaultVSDLow, // 23

  eDTCIdxECUFaultVCPHigh,
  eDTCIdxECUFaultVCPLow1,
  eDTCIdxECUFaultVCPLow2,
  eDTCIdxNVMECCFault,
  eDTCIdxRAMFault,
  eDTCIdxFlashCheckSumError,
  eDTCIdxIShuntZeroError,
  // eDTCIdxI, //31

  eDTCRecordsNum,
} DTCRecordIdx_t;

typedef union {
  u16 reg;

  /* polyspace-begin MISRA-C3:6.1 */
  struct
  {
    struct
    {
      u8 RePowerCnt : 7;
      u8 IsFault : 1;
    } FaultRePowerInfo;

    u8 FaultTotalCnt;
  } bit;
  /* polyspace-end MISRA-C3:6.1 */

} DTCRecord_t;

typedef struct
{
  u16 Dummy;
  u16 PowerUpCnt;
  u16 IShuntZero;

  DTCRecord_t Records[eDTCRecordsNum];

  u16 CheckSum;
} DtcData_t;

/* Variables */
/******************************************************************/
extern DtcData_t DtcData;
extern u8 DtcDataSwRstEn;

/* Function declaration */
/******************************************************************/
extern void DTC_init(void);
extern void DTC_step(void);
extern DTCRecord_t DTC_getRecord(DTCRecordIdx_t Idx);

#endif
