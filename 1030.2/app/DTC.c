/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
*disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:DTC.c
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

/* Includes*/
/******************************************************************/
#include "DTC.h"
#include "GlobalVars.h"
#include "MotorAPI.h"
#include "Sdk.h"

#include "AppMain.h" /* Model's header file */
#include "rtwtypes.h"

/* Defines*/
/******************************************************************/
#define DTC_DUMMY_DATA (0x55AAu)
#define DTC_REPOWER_UP_CNT_MAX (0x7Fu)

#define DTC_DEFAULT_I_SHUNT_ZERO (1660u)
/*修改EEROM写入判断条件 22.06.08 by jxj*/
#define DTC_DFLASH_WRITE_VOLTAGE_MIN (65u)
#define DTC_DFLASH_WRITE_VOLTAGE_MAX (270u)
#define DTC_DFLASH_WRITE_CHIP_T_MIN (-35)
#define DTC_DFLASH_WRITE_CHIP_T_MAX (150)

/* Variables */
/******************************************************************/
DtcData_t DtcData;
u8 DtcDataSwRstEn = 0u;

static Addr_t DFlashStartAddr;
/*增加EEROM数据备份 22.04.23 by jxj*/
static Addr_t DFlashBackUpAddr;
/* polyspace +1 MISRA-C3:8.4 */
u16 DTCVarLen;
static u8 IsDTCDataChanged = 0u;
/* polyspace-begin MISRA-C3:6.1 */
static struct
{
  u16 FlagNew : 1;
  u16 FlagOld : 1;
  u16 CntNew : 7;
  u16 CntOld : 7;
} FaultFlagAndCnt[eDTCRecordsNum];
/* polyspace-end MISRA-C3:6.1 */

/* Function declaration */
/******************************************************************/
static u8 isDtcDataValid(void);
static u16 calDtcDataCheckSum(void);
static void dtcDataReset(void);
static void powerUpCntInc(void);
static void updateFaultTotalCnt(DTCRecordIdx_t Idx, u8 FaultCntOrFlag, u8 FaultCntFlagOld);
static void updateFaultRePowerInfo(DTCRecordIdx_t Idx, u8 IsFault);
static void writeDtcDataToDFlash(void);

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:DTC_init
* Description:
* 1.获取Data Flash首地址
* 2.计算DtcData数据长度
* 3.从Data Flash中读取数据到DtcData中
* 4.判断DtcData是否有效，若无效则重置为初始值，有效则将上电次数增加
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
void DTC_init(void)
{
  u8 Valid = 0u;

  DFlashStartAddr = Sdk_getDFlashStartAddr();
  /*读取备份数据地址 22.04.23 by,jxj*/
  DFlashBackUpAddr = Sdk_getDFlashBackUpAddr();
  DTCVarLen = sizeof(DtcData);
  Sdk_readDFlash(DFlashStartAddr, (u8 *)(&DtcData), DTCVarLen);
  Valid = isDtcDataValid();
  if (Valid == 0u)
  {
    Sdk_readDFlash(DFlashBackUpAddr, (u8 *)(&DtcData), DTCVarLen);
    Valid = isDtcDataValid();
  }

  if (Valid == 0u)
  {
    /* 数据错误对EEROM进行重置 */
    dtcDataReset();
  }
  else
  {
    powerUpCntInc();
  }

  IsDTCDataChanged = 1u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:isDtcDataValid
* Description:
* 计算DtcData数据的校验和并和DtcData中的校验和比较，验证数据是否有效
*
* Inputs:
*
*
* Outputs:
* 1：有效
* 0：无效
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static u8 isDtcDataValid(void)
{
  u16 ChkSum = 0u;

  ChkSum = calDtcDataCheckSum();
  return ((DtcData.Dummy == DTC_DUMMY_DATA) && (DtcData.CheckSum == ChkSum)) ? 1u : 0u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:calDtcDataCheckSum
* Description:
* 将DtcData中的数据累加求和作为校验
*
* Inputs:DtcData
*
*
* Outputs:CheckSum
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static u16 calDtcDataCheckSum(void)
{
  u16 Sum = 0u;

  Sum += DtcData.Dummy;
  Sum += DtcData.PowerUpCnt;
  Sum += DtcData.IShuntZero;

  for (u16 i = 0u; i < eDTCRecordsNum; i++)
  {
    Sum += DtcData.Records[i].reg;
  }

  return Sum;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:dtcDataReset
* Description:
*   Set Dummy = DTC_DUMMY_DATA, PowerUpCnt = 1, Records = 0.
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static void dtcDataReset(void)
{
  DtcData.Dummy = DTC_DUMMY_DATA;
  DtcData.PowerUpCnt = 1u;
  DtcData.IShuntZero = DTC_DEFAULT_I_SHUNT_ZERO;

  for (u16 i = 0u; i < eDTCRecordsNum; i++)
  {
    DtcData.Records[i].reg = 0u;
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:powerUpCntInc
* Description:
* 1.PowerUpCnt: PowerUp count from first flashed
* 2.IsFault: 1:falut occord, 0:falut not occord
* 2.RePowerCnt:repower count after last fault occord
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static void powerUpCntInc(void)
{
  if (DtcData.PowerUpCnt < U16_MAX)
  {
    DtcData.PowerUpCnt++;
  }
  else
  {
  }

  for (u16 i = 0u; i < eDTCRecordsNum; i++)
  {
    if (DtcData.Records[i].bit.FaultRePowerInfo.IsFault != 0u)
    {
      if (DtcData.Records[i].bit.FaultRePowerInfo.RePowerCnt < DTC_REPOWER_UP_CNT_MAX)
      {
        DtcData.Records[i].bit.FaultRePowerInfo.RePowerCnt++;
      }
      else
      {
        DtcData.Records[i].bit.FaultRePowerInfo.IsFault = 0u;
        DtcData.Records[i].bit.FaultRePowerInfo.RePowerCnt = 0u;
      }
    }
    else
    {
    }
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name: DTC_periodTask
* Description:
* 1.Update fault flags
* 2.Record fault counts
* 3.Write DTC data to EEPROM before power down
*
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
void DTC_step(void)
{
  FaultFlagAndCnt[eDTCIdxBatHigh].FlagNew = Y.VoltageStatus > 0;
  FaultFlagAndCnt[eDTCIdxBatLow].FlagNew = Y.VoltageStatus < 0;

  FaultFlagAndCnt[eDTCIdxChipTHigh].FlagNew = Y.AmbTStatus > 0;
  FaultFlagAndCnt[eDTCIdxChipTLow].FlagNew = Y.AmbTStatus < 0;

  FaultFlagAndCnt[eDTCIdxMotorShort].FlagNew = MotorAPI.Faults.bit.Short;
  FaultFlagAndCnt[eDTCIdxMotorOpen].FlagNew = MotorAPI.Faults.bit.Open;
  FaultFlagAndCnt[eDTCIdxMotorLock].FlagNew = MotorAPI.Faults.bit.Lock;
  FaultFlagAndCnt[eDTCIdxMotorShort].CntNew = Y.ShortCnt;
  FaultFlagAndCnt[eDTCIdxMotorOpen].CntNew = Y.OpenCnt;
  FaultFlagAndCnt[eDTCIdxMotorLock].CntNew = Y.LockCnt;

  FaultFlagAndCnt[eDTCIdxECUFaultSysOT].FlagNew = EcuFaults.bit.SysOT;
  FaultFlagAndCnt[eDTCIdxECUFaultPMU_5V_OV].FlagNew = EcuFaults.bit.PMU_5V_OV;
  FaultFlagAndCnt[eDTCIdxECUFaultPMU_1V5_OV].FlagNew = EcuFaults.bit.PMU_1V5_OV;
  FaultFlagAndCnt[eDTCIdxECUFaultSuppShort].FlagNew = EcuFaults.bit.SuppShort;
  FaultFlagAndCnt[eDTCIdxECUFaultSuppTimeOut].FlagNew = EcuFaults.bit.SuppTimeOut;
  FaultFlagAndCnt[eDTCIdxECUFaultVDDEXT_OverLoad].FlagNew = EcuFaults.bit.VDDEXT_OverLoad;
  FaultFlagAndCnt[eDTCIdxECUFaultVDDEXT_OverVoltage].FlagNew = EcuFaults.bit.VDDEXT_OverVoltage;
  FaultFlagAndCnt[eDTCIdxECUFaultVDDEXT_Short].FlagNew = EcuFaults.bit.VDDEXT_Short;
  FaultFlagAndCnt[eDTCIdxECUFaultRef5VOverLoad].FlagNew = EcuFaults.bit.Ref5VOverLoad;
  FaultFlagAndCnt[eDTCIdxECUFaultRef5VOverVoltage].FlagNew = EcuFaults.bit.Ref5VOverVoltage;
  FaultFlagAndCnt[eDTCIdxECUFaultRef5VUnderVoltage].FlagNew = EcuFaults.bit.Ref5VUnderVoltage;
  FaultFlagAndCnt[eDTCIdxECUFaultVBGError].FlagNew = EcuFaults.bit.VBGError;
  FaultFlagAndCnt[eDTCIdxECUFaultVSDHigh].FlagNew = EcuFaults.bit.VSDHigh;
  FaultFlagAndCnt[eDTCIdxECUFaultVSDLow].FlagNew = EcuFaults.bit.VSDLow;
  FaultFlagAndCnt[eDTCIdxECUFaultVCPHigh].FlagNew = EcuFaults.bit.VCPHigh;
  FaultFlagAndCnt[eDTCIdxECUFaultVCPLow1].FlagNew = EcuFaults.bit.VCPLow1;
  FaultFlagAndCnt[eDTCIdxECUFaultVCPLow2].FlagNew = EcuFaults.bit.VCPLow2;
  FaultFlagAndCnt[eDTCIdxNVMECCFault].FlagNew = EcuFaults.bit.DFlash;
  FaultFlagAndCnt[eDTCIdxRAMFault].FlagNew = EcuFaults.bit.Ram;
  FaultFlagAndCnt[eDTCIdxFlashCheckSumError].FlagNew = EcuFaults.bit.CFlash;

  for (u8 i = 0u; i < eDTCRecordsNum; i++)
  {
    updateFaultRePowerInfo((DTCRecordIdx_t)i, FaultFlagAndCnt[i].FlagNew);

    if ((i == eDTCIdxMotorShort) || (i == eDTCIdxMotorOpen) || (i == eDTCIdxMotorLock))
    {
      updateFaultTotalCnt((DTCRecordIdx_t)i, FaultFlagAndCnt[i].CntNew, FaultFlagAndCnt[i].CntOld);
      FaultFlagAndCnt[i].CntOld = FaultFlagAndCnt[i].CntNew;
    }
    else
    {
      updateFaultTotalCnt((DTCRecordIdx_t)i, FaultFlagAndCnt[i].FlagNew, FaultFlagAndCnt[i].FlagOld);
    }

    FaultFlagAndCnt[i].FlagOld = FaultFlagAndCnt[i].FlagNew;
  }

  if (DtcDataSwRstEn != 0u)
  {
    DtcDataSwRstEn = 0u;

    dtcDataReset();
  }

  /* 在DTC数据变化并且电机停止、低压时或休眠前将数据写入Data Flash */
  /*增加EEROM写入判断条件 22.04.23 by jxj*/
  if ((IsDTCDataChanged != 0u) && (AdcResults.Voltage.Bat > DTC_DFLASH_WRITE_VOLTAGE_MIN) &&
      (AdcResults.Voltage.Bat < DTC_DFLASH_WRITE_VOLTAGE_MAX) &&
      (AdcResults.Temperature.Chip > DTC_DFLASH_WRITE_CHIP_T_MIN) &&
      (AdcResults.Temperature.Chip < DTC_DFLASH_WRITE_CHIP_T_MAX) && (MotorAPI.ActSpeed == 0) &&
      ((Y.VoltageStatus == -1) || (EcuFaults.bit.VSDLow != 0u) || (Y.ChipSleep != 0u) || (Y.ChipReset != 0u)))
  {
    if (EcuFaults.bit.DFlash != 0u)
    {
      Sdk_clrDFlashError(DFlashStartAddr, DTCVarLen);
    }

    if (EcuFaults.bit.DFlash == 0u)
    {
      writeDtcDataToDFlash();
    }
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:updateFaultTotalCnt
* Description:
*
*
* Inputs:
* 1.Idx:fault type index
* 2.FaultCntOrFlag:Current fault count or falut flag
* 3.FaultCntFlagOld:Last fault count or fault flag
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static void updateFaultTotalCnt(DTCRecordIdx_t Idx, u8 FaultCntOrFlag, u8 FaultCntFlagOld)
{
  if ((FaultCntOrFlag > FaultCntFlagOld) && (Idx < eDTCRecordsNum))
  {
    if (DtcData.Records[Idx].bit.FaultTotalCnt < U8_MAX)
    {
      DtcData.Records[Idx].bit.FaultTotalCnt++;

      IsDTCDataChanged = 1u;
    }
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:updateFaultRePowerInfo
* Description:
*   Update fault flag and falut repower count.
*
* Inputs:
* 1.Idx:fault type index
* 2.IsFault:true or false
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static void updateFaultRePowerInfo(DTCRecordIdx_t Idx, u8 IsFault)
{
  if ((IsFault != 0u) && (Idx < eDTCRecordsNum))
  {
    if (DtcData.Records[Idx].bit.FaultRePowerInfo.IsFault != 1u)
    {
      DtcData.Records[Idx].bit.FaultRePowerInfo.IsFault = 1u;
      DtcData.Records[Idx].bit.FaultRePowerInfo.RePowerCnt = 0u;

      IsDTCDataChanged = 1u;
    }
    else
    {
      if (DtcData.Records[Idx].bit.FaultRePowerInfo.RePowerCnt != 0)
      {
        DtcData.Records[Idx].bit.FaultRePowerInfo.RePowerCnt = 0u;

        IsDTCDataChanged = 1u;
      }
    }
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:DTC_getRecord
* Description:
*   Read fault information by fault index
*
* Inputs:
* 1.Idx:fault type index
*
* Outputs:
* Fault record type
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
DTCRecord_t DTC_getRecord(DTCRecordIdx_t Idx)
{
  return (Idx < eDTCRecordsNum) ? (DtcData.Records[Idx])
                                : ((DTCRecord_t){
                                    0u,
                                  });
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:writeDtcDataToDFlash
* Description:
*   Write DTC data to EEPROM
*   Write DTC will pause flash code executing about 7ms.
* Inputs:
*
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static void writeDtcDataToDFlash(void)
{
  IsDTCDataChanged = 0u;
  DtcData.CheckSum = calDtcDataCheckSum();
  Sdk_writeDFlash(DFlashStartAddr, (u8 *)(&DtcData), DTCVarLen);
  /*备用数据存储 22.04.23 by jxj*/
  Sdk_writeDFlash(DFlashBackUpAddr, (u8 *)(&DtcData), DTCVarLen);
}
