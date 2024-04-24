/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:SDK.h
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
#ifndef SDK_H
#define SDK_H

/* Includes*/
/******************************************************************/
#include "AppTypes.h"
#include "MotorAPI.h"

/* Defines*/
/******************************************************************/
#define ISR_TIME_MEASURE_EN (1u)

enum{
  ADC_ChBat 	= 0x21u,
  ADC_ChVSD 	= 0x22u,
  ADC_ChVCP 	= 0x23u,
  ADC_ChMON 	= 0x24u,
  ADC_ChVDDP 	= 0x25u,
  ADC_ChVAREF = 0x26u,
  ADC_ChVBG 	= 0x27u,
  ADC_ChVDDC 	= 0x28u,
  ADC_ChChipT = 0x29u,
  ADC_ChAmbT 	= 0x14u,
};
/*polyspace-begin  MISRA2012:6.1*/
typedef union{
	u32 reg;
	
	struct
	{
		u32 SysOT:1;
		u32 PMU_5V_OV:1;
		u32 PMU_1V5_OV:1;/* 2 */
		u32 SuppShort:1;
		u32 SuppTimeOut:1;
		
		u32 VDDEXT_OverLoad:1;/* 5 */
		u32 VDDEXT_OverVoltage:1;
		u32 VDDEXT_Short:1;
		u32 VDDEXT_On:1;//8
		
		u32 Ref5VOverLoad:1;
		u32 Ref5VOverVoltage:1;
		u32 Ref5VUnderVoltage:1;/* 11 */
		u32 Ref5VError:1;
		
		u32 VBGError:1;/* 13 */
		
		
		u32 ADC1ResultInvalid:1;/* 17 */
		u32 ADC1ResultInvalidFiltered:1;
		
		u32 VSDHigh:1;
		u32 VSDLow:1;/* 20 */
		u32 VCPHigh:1;
		u32 VCPLow1:1;
		u32 VCPLow2:1;/* 23 */
		
		u32 ECUFault:1;
		
		u32 DFlash:1;
		u32 Ram:1;/* 26 */
		u32 CFlash:1;
	}bit;
}ECUFaults_t;
/*polyspace-end  MISRA2012:6.1*/
typedef struct{
  u32 StartCnt;
  u32 EndCnt;
  u32 DiffCnt[3];
  u32 MaxDiffCnt[3];
	float Time_us[3];
} IsrTime_t;

typedef struct{
	u32 * start_addr;
	u32 * end_addr;
}ADDRESS_t; 

/* Variables */
/******************************************************************/
extern u16 CSAZero;
extern ECUFaults_t ECUFaults;/*polyspace  MISRA2012:8.6*/
/*消除polyspace警告,2021-09-22-by zyg*/
extern u32 FlashCheckSumCal;

#if ISR_TIME_MEASURE_EN
extern IsrTime_t IsrTime;
#endif


/* Function declaration */
/******************************************************************/
extern void SDK_HardwareInit(void);

extern u8 SDK_UartReadU8(void);
extern void SDK_UartWriteU8(u8 Data);

extern void SDK_FeedWDog(void);
extern void SDK_GoToSleepMode(void);
extern void SDK_SystemReset(void);

extern void SDK_PwmOutInit(void);
extern void SDK_PwmOutHigh(void);
extern void SDK_PwmOutLow(void);
extern void SDK_PwmOutToggle(void);

extern u8 SDK_IsPwmInCap(void);
extern u16 SDK_PwmInGetCaptureCnt(void);
extern void SDK_PwmInSetCaptureRisingEdge(void);
extern void SDK_PwmInSetCaptureFallingEdge(void);
extern u8 SDK_PwmInGetCaptureEdge(void);
extern u8 SDK_PwmInIsCaptureTimerRunning(void);
extern void SDK_PwmInCaptureTimerReset(void);
extern u8 SDK_PwmInGetBusStatus(void);

extern void SDK_ADCGetChResult(u8 Ch, u16* pResult);

extern void SDK_EcuFaultDiag(void);

extern void SDK_EEPROM_Write(Addr_t Addr, u8 *pData, u16 DataLen);
extern void SDK_EEPROM_Read(Addr_t Addr, u8 *pData, u16 DataLen);
extern void SDK_EEPROM_Erase(Addr_t Addr, u16 DataLen);

extern void SDK_Mem_Check(void);

extern void SDK_BDRV_Off(void);
extern void SDK_BDRV_TestMos(u8 state);


extern void BDRV_Diag_Supply(void);
extern void AppTimeBaseHandler(void);
extern void Uart2TxHandler(void);
extern void Timer2CaptureHandler(void);
extern void Timer2OverflowHandler(void);
extern void Gpt2_T6_Handler(void);
extern void Uart2RxHandler(void);
extern void GPT1_T2_Handler(void);
extern void GPT1_T4_Handler(void);
extern void BDRV_Diag(void);

extern void SDK_BDRVOpenLoadDiag(BDRVOffDiagResult_t * DiagResult);
extern u8 SDK_BDRVGetShortStatus(void);

/* 恢复DFalsh故障，Addr:故障区域起始地址 ；DataLen故障区域长度，单位：Bytes*/
extern void SDK_clrDFlashError(Addr_t Addr, u16 DataLen);
/* 向数据flash写入数据，DataLen单位：Byte */
extern void SDK_writeDFlash(Addr_t Addr, u8 *pData, u16 DataLen);
/*封装开关全局中断，24-02-25,by WChuang*/
extern void Sdk_enterCritical(void);
extern void Sdk_exitCritical(void);

#if ISR_TIME_MEASURE_EN
#include "tle_device.h"

#define TimeMeasureStart() IsrTime.StartCnt = CPU->SYSTICK_CUR.reg
#define TimeMeasureEnd(idx) \
{\
	IsrTime.EndCnt = CPU->SYSTICK_CUR.reg;\
  IsrTime.DiffCnt[idx] =((IsrTime.EndCnt < IsrTime.StartCnt) ? 0u : CPU->SYSTICK_RL.reg) + IsrTime.StartCnt - IsrTime.EndCnt;\
  if(IsrTime.DiffCnt[idx] > IsrTime.MaxDiffCnt[idx])\
  {\
    IsrTime.MaxDiffCnt[idx] = IsrTime.DiffCnt[idx];\
		IsrTime.Time_us[idx] = (float)IsrTime.MaxDiffCnt[idx]/((SCU_FSYS)/1000000u);\
  }\
}
#endif

#endif /* !SDK_H */
