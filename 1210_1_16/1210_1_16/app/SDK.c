/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:SDK.c
********************************************************************
* Project/Product:LXT9818_PMP
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
*     1.0      2021-07-23      yhd             Original
*     
*
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#include "SDK.h"
#include "tle_device.h"
#include "ADCSample.h"
#include "Common.h"
#include "UserInfo.h"

/* Defines*/
/******************************************************************/
#define PWM_OUT_PIN (PORT->P0_DATA.bit.P2)

/* FCT测试需求50%占空比 2022-12-21-by zjj*/
#define PWN_OUT                                 (CCU6_T12PR/2)

/* ECU Self Error */
#define AD_ERR_DIAG_NUM                         10u
#define VSDHIGH_ERR_DIAG_NUM                    30u /*EMC_5B实验，端电压输入400ms的23V脉冲，VSD测量超出阈值的时间最大可达240ms，23-04-21 by zjj*/
#define VSLOW_ERR_DIAG_NUM                      5u
#define VCPHIGH_ERR_DIAG_NUM                    25u /*EMC_5B实验，端电压输入400ms的23V脉冲，VSP测量超出阈值的时间最大可达170ms，23-04-21 by zjj*/
#define VCPLOW1_ERR_DIAG_NUM                    0u
#define VCPLOW2_ERR_DIAG_NUM                    0u

#define SHORT_DIAG_CURRENT_SETTLE_TIME_US (500u)
#define OPEN_DIAG_CURRENT_SETTLE_TIME_US  (1400u)/*  increase 200us - 2021-07-29-by zyg  */

#define RAM_TEST_NUM1 (0x55555555uL)
#define RAM_TEST_NUM2 (~RAM_TEST_NUM1)

#define RAMTestU32(Var) \
do {\
	tmp = (Var);\
	(Var) = RAM_TEST_NUM1;\
	if((Var) != RAM_TEST_NUM1)\
	{\
		return true;\
	}\
\
	else\
	{\
	}\
	(Var) = RAM_TEST_NUM2;\
	if ((Var) != RAM_TEST_NUM2)\
	{\
		return true;\
	}\
	else\
	{\
	}\
	(Var) = tmp;\
} while(0u)

#define FLASH_CHECK_SUM_AND_BAR_CODE_SIZE (0x18u)
#define FLASH_ADDR_BGN            (u32 *)(ProgFlashStart)   
#define FLASH_ADDR_END            (u32 *)((DataFlashStart) - (FLASH_CHECK_SUM_AND_BAR_CODE_SIZE))
#define RAM_START_ADDR (volatile u32 *)(RAMStart)
#define RAM_END_ADDR   (volatile u32 *)(RAMStart + RAMSize)

/* Variables */
/******************************************************************/
u16 CSAZero;
ECUFaults_t ECUFaults;

#if ISR_TIME_MEASURE_EN
IsrTime_t IsrTime;
#endif

u32 FlashCheckSumCal = 0u;




/* Function declaration */
/******************************************************************/
static void SDK_PMU_VDDEXT_ForceOn(void);
static void SDK_eraseDFlash(Addr_t Addr, u16 DataLen);
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_HardwareInit
* Description:
*
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
void SDK_HardwareInit(void)
{
  TLE_Init();
  SDK_PMU_VDDEXT_ForceOn();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_PMU_VDDEXT_ForceOn
* Description:
*
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
static void SDK_PMU_VDDEXT_ForceOn(void)
{
  Sdk_enterCritical();         //关中断
  PMU->VDDEXT_CTRL.reg = 1u;   //开启芯片外部电源供电
  PMU->VDDEXT_CTRL.reg = 0u;   //关闭芯片外部电源供电 
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 1u;
  PMU->VDDEXT_CTRL.reg = 0u;
  PMU->VDDEXT_CTRL.reg = 1u;
  Sdk_exitCritical();         //开中断
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:
* Description:Sdk_enterCritical
*
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
void Sdk_enterCritical(void)
	{	/*封装开关全局中断，24-02-25,by WChuang*/
	__disable_irq();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:
* Description:Sdk_exitCritical
*
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
void Sdk_exitCritical(void)
{	/*封装开关全局中断，24-02-25,by WChuang*/
	__enable_irq();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_UartWriteU8
* Description:
*
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
u8 SDK_UartReadU8(void)
{
  return(UART2->SBUF.reg);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_UartWriteU8
* Description:
*
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
void SDK_UartWriteU8(u8 Data)
{
  UART2_Send_Byte(Data);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_FeedWDog
* Description:
*
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
void SDK_FeedWDog(void)
{
  WDT1_Service();
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_GoToSleepMode
* Description:
*
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
void SDK_GoToSleepMode(void)
{
  SCU_EnterSleepMode();
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_SystemReset
* Description:
*
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
void SDK_SystemReset(void)
{
  /* triger the wdt1 reset */
  SCUPM->WDT1_TRIG.reg = (u8) 0x00000100u;
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:
* Description:SDK_PwmOutInit
*
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
void SDK_PwmOutInit(void)
{

}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_PwmOutHigh
* Description:
*
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
void SDK_PwmOutHigh(void)
{
  PWM_OUT_PIN = 1u;
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_PwmOutLow
* Description:
*
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
void SDK_PwmOutLow(void)
{
  PWM_OUT_PIN = 0u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_PwmOutToggle
* Description:
*
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
void SDK_PwmOutToggle(void)
{
  PWM_OUT_PIN = !PWM_OUT_PIN;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_IsPwmInCap
* Description:
*
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
u8 SDK_IsPwmInCap(void)
{
  return TIMER2->T2CON.bit.EXF2;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_PwmInGetCaptureCnt
* Description:
*
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
u16 SDK_PwmInGetCaptureCnt(void)
{
  return TIMER2_Capture_Value_Get();
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_PwmInSetCaptureRisingEdge
* Description:
*
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
void SDK_PwmInSetCaptureRisingEdge(void)
{
  TIMER2->T2MOD.bit.EDGESEL = 1u;
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_PwmInSetCaptureFallingEdge
* Description:
*
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
void SDK_PwmInSetCaptureFallingEdge(void)
{
  TIMER2->T2MOD.bit.EDGESEL = 0u;
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_PwmInSetCaptureFallingEdge
* Description:
*
*
* Inputs:
*
*
* Outputs:
* 0u:Falling, 1u:Rising
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
u8 SDK_PwmInGetCaptureEdge(void)
{
  return TIMER2->T2MOD.bit.EDGESEL;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_PwmInIsCaptureTimerRunning
* Description:
*
*
* Inputs:
*
*
* Outputs:
* 0u:stopped, 1u:running
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
u8 SDK_PwmInIsCaptureTimerRunning(void)
{
  return TIMER2->T2CON.bit.TR2;
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_PwmInCaptureTimerReset
* Description:
*
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
void SDK_PwmInCaptureTimerReset(void)
{
  TIMER2_Stop();
  TIMER2_Clear_Count();
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:
* Description:
*
*
* Inputs:
*
*
* Outputs:
* 0u:Low, 1u:High
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
u8 SDK_PwmInGetBusStatus(void)
{
  return LIN->CTRL_STS.bit.RXD;
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_ADCGetChResult
* Description:
*
*
* Inputs:
* Ch:defined in SDK.h
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
void SDK_ADCGetChResult(u8 Ch, u16* pResult)
{
  u16 ADVal = 0xFFFFu;

  switch(Ch>>4u)
  {
    case 1u:
      ECUFaults.bit.ADC1ResultInvalid = ADC1_GetChResult(&ADVal, Ch&0xFu);
      break;
    case 2u:
      ADVal = ADC2_GetChResult(Ch&0xFu) >> 2u;
      break;
    default:/*加注释消除polyspace警告,2021-09-22-by zyg*/
    break;
  }

  if(ADVal != 0xFFFFu)
  {
    *pResult = ADVal;
  }
  else
  {

  }
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_EcuFaultDiag
* Description:
*
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
void SDK_EcuFaultDiag(void)
{
  static u16 ad_errcnt = 0u;
  static u16 vsdhigh_errcnt = 0u;
  static u16 vsdlow_errcnt = 0u;
  static u16 vcphigh_errcnt = 0u;
  static u16 vcplow1_errcnt = 0u;
  static u16 vcplow2_errcnt = 0u;
  static u16 ADC1VREFFaultRstDelay = 0u;
	static u16 Ref5VUnderVoltageCnt = 0u;

	/*增加lin相关诊断,2021-08-20-by zyg*/
	if(LIN_Get_Mode() != LIN_GET_MODE_NORMAL)
  {
    LIN_Set_Mode(LIN_MODE_SLEEP);
    LIN->CTRL_STS.bit.HV_MODE = 0;
    LIN->CTRL_STS.bit.M_SM_ERR_CLR = 1;
		SCUPM->SYS_SUPPLY_IRQ_CLR.bit.VS_UV_ICLR = 1;
		SCUPM->SYS_SUPPLY_IRQ_CLR.bit.VS_UV_SCLR = 1;
		SCUPM->SYS_ISCLR.bit.LIN_OC_ICLR = 1;
		SCUPM->SYS_ISCLR.bit.LIN_OT_ICLR = 1;
		SCUPM->SYS_ISCLR.bit.LIN_TMOUT_ICLR = 1;
    LIN_Set_Mode(LIN_MODE_NORMAL);
  }	
  ECUFaults.bit.SysOT  = PMU->SYS_FAIL_STS.bit.SYS_OT;
  ECUFaults.bit.PMU_5V_OV = PMU->SYS_FAIL_STS.bit.PMU_5V_OVL;
  ECUFaults.bit.PMU_1V5_OV = PMU->SYS_FAIL_STS.bit.PMU_1V5_OVL;
  ECUFaults.bit.SuppShort  = PMU->SYS_FAIL_STS.bit.SUPP_SHORT;
  ECUFaults.bit.SuppTimeOut  = PMU->SYS_FAIL_STS.bit.SUPP_TMOUT;
	
  ECUFaults.bit.VDDEXT_OverLoad = PMU->VDDEXT_CTRL.bit.OVERLOAD;
  ECUFaults.bit.VDDEXT_OverVoltage  = PMU->VDDEXT_CTRL.bit.OVERVOLT;
  ECUFaults.bit.VDDEXT_Short = PMU->VDDEXT_CTRL.bit.SHORT;
	
  if(ECUFaults.bit.VDDEXT_OverLoad || ECUFaults.bit.VDDEXT_OverVoltage || ECUFaults.bit.VDDEXT_Short)
  {
    PMU->VDDEXT_CTRL.bit.SHORT = 0u;
    PMU->VDDEXT_CTRL.bit.ENABLE = 1u;
  }
  else
  {

  }
	
	if(Ref5VUnderVoltageCnt < 50u)	
	{	
		if(MF->REF2_CTRL.bit.VREF5V_UV_STS)
		{
					SCUPM->SYS_ISCLR.bit.VREF5V_LOWTH_ICLR = 1u;	/*瞬间电压恢复清除 MF->REF2_CTRL.bit.VREF5V_UV_STS 标志，2023.04.07 by zjj*/			
					Ref5VUnderVoltageCnt++;				
		}
		else
	  {
		}
	}
	else
	{
		Ref5VUnderVoltageCnt = 0u;
	}
  ECUFaults.bit.VDDEXT_On = ((PMU->VDDEXT_CTRL.reg & PMU_VDDEXT_STABLE) == PMU_VDDEXT_STABLE);

  ECUFaults.bit.Ref5VOverLoad = MF->REF2_CTRL.bit.VREF5V_OVL_STS;
  ECUFaults.bit.Ref5VOverVoltage  = MF->REF2_CTRL.bit.VREF5V_OV_STS;
	ECUFaults.bit.Ref5VUnderVoltage = MF->REF2_CTRL.bit.VREF5V_UV_STS;

  ECUFaults.bit.Ref5VError = ECUFaults.bit.Ref5VOverLoad || ECUFaults.bit.Ref5VOverVoltage || ECUFaults.bit.Ref5VUnderVoltage;

  if(ECUFaults.bit.Ref5VUnderVoltage)
  {
    if(ADC1VREFFaultRstDelay > 2u)
    {
      if(ADCResults.Voltage.Bat > 75u)
      {
				SCUPM->SYS_ISCLR.bit.VREF5V_LOWTH_ICLR = 1u;	/*瞬间电压恢复清除 MF->REF2_CTRL.bit.VREF5V_UV_STS 标志，2023.04.07 by zjj*/
        SDK_SystemReset();
				
      }
      else
      {

      }
    }
    else
    {
      ADC1VREFFaultRstDelay++;
    }
  }
  else
  {
    ADC1VREFFaultRstDelay = 0u;
  }

  ECUFaults.bit.VBGError = (MF->REF1_STS.bit.REFBG_LOTHWARN_STS) || (MF->REF1_STS.bit.REFBG_UPTHWARN_STS);
  ECUFaults.bit.ADC1ResultInvalidFiltered = Common_SetStatusDelay(ECUFaults.bit.ADC1ResultInvalid,AD_ERR_DIAG_NUM,&ad_errcnt);
	
  ECUFaults.bit.VSDHigh = Common_SetStatusDelay(SCUPM->BDRV_IS.bit.VSD_UPTH_STS,VSDHIGH_ERR_DIAG_NUM,&vsdhigh_errcnt);
  if(SCUPM->BDRV_IS.bit.VSD_UPTH_STS == 1u)
  {
    SCUPM->BDRV_ISCLR.bit.VSD_UPTH_SCLR = 1u;
    SCUPM->BDRV_ISCLR.bit.VSD_UPTH_ICLR = 1u;
  }
  else
  {

  }
  ECUFaults.bit.VSDLow = Common_SetStatusDelay(SCUPM->BDRV_IS.bit.VSD_LOWTH_STS,VSLOW_ERR_DIAG_NUM,&vsdlow_errcnt);
  if(SCUPM->BDRV_IS.bit.VSD_LOWTH_STS == 1u)
  {
    SCUPM->BDRV_ISCLR.bit.VSD_LOWTH_SCLR = 1u;
    SCUPM->BDRV_ISCLR.bit.VSD_LOWTH_ICLR = 1u;
  }
  else
  {

  }
  ECUFaults.bit.VCPHigh = Common_SetStatusDelay(SCUPM->BDRV_IS.bit.VCP_UPTH_STS,VCPHIGH_ERR_DIAG_NUM,&vcphigh_errcnt);
  if(SCUPM->BDRV_IS.bit.VCP_UPTH_STS == 1u)
  {
    SCUPM->BDRV_ISCLR.bit.VCP_UPTH_SCLR = 1u;
    SCUPM->BDRV_ISCLR.bit.VCP_UPTH_ICLR = 1u;
  }
  else
  {

  }
  ECUFaults.bit.VCPLow1 = Common_SetStatusDelay(SCUPM->BDRV_IS.bit.VCP_LOWTH1_STS,VCPLOW1_ERR_DIAG_NUM,&vcplow1_errcnt);
  if(SCUPM->BDRV_IS.bit.VCP_LOWTH1_STS == 1u)
  {
    SCUPM->BDRV_ISCLR.bit.VCP_LOWTH1_SCLR = 1u;
    SCUPM->BDRV_ISCLR.bit.VCP_LOWTH1_ICLR = 1u;
  }
  else
  {

  }
  ECUFaults.bit.VCPLow2 = Common_SetStatusDelay(SCUPM->BDRV_IS.bit.VCP_LOWTH2_STS,VCPLOW2_ERR_DIAG_NUM,&vcplow2_errcnt);
  if(SCUPM->BDRV_IS.bit.VCP_LOWTH2_STS == 1u)
  {
    SCUPM->BDRV_ISCLR.bit.VCP_LOWTH2_SCLR = 1u;
    SCUPM->BDRV_ISCLR.bit.VCP_LOWTH2_ICLR = 1u;
  }
  else
  {

  }
	
  static u16 BlankTime = 0u;
  if(BlankTime < 1000u)
  {
    BlankTime += 10u;
  }
  else
  {
    u8 BDRVLow;
    static u8 BDRVLowLast = 0u;

    BDRVLow = ECUFaults.bit.VSDLow || ECUFaults.bit.VCPLow1 || ECUFaults.bit.VCPLow2;
    if((BDRVLow == 1u) && (BDRVLowLast == 0u))
    {
     /* VsJudgeSetState(GFJ_IN_UnderVol); */
    }
    else
    {

    }

    BDRVLowLast = BDRVLow;
  }
	
	ECUFaults.bit.ECUFault =  ECUFaults.bit.Ref5VError
         || ECUFaults.bit.VSDHigh || ECUFaults.bit.VSDLow
         || ECUFaults.bit.VCPHigh || ECUFaults.bit.VCPLow1 || ECUFaults.bit.VCPLow2
         || (ADCResults.IsAmbTInvalid && (ECUFaults.bit.VDDEXT_On || (ADCResults.Temperature.Chip > 150)))
         || ECUFaults.bit.SysOT;	
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:
* Description:
*
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
void SDK_EEPROM_Write(Addr_t Addr, u8 * pData, u16 DataLen)
{
	Addr_t OffSet = 0u;
	/*消除polyspace警告,2021-09-22-by zyg*/
	while(DataLen > 0u)
	{
		GPT12E_T2_Stop();
		ProgramPage(Addr + OffSet, pData, 0u, 1u, 0u);
		GPT12E_T2_Start();
		
		if(DataLen >= 128u)
		{
			DataLen -= 128u;
			OffSet += 128u;
		}
		else
		{
			DataLen = 0u;
		}
	}
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:
* Description:
*
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
void SDK_EEPROM_Read(Addr_t Addr, u8 *pData, u16 DataLen)
{
	u16 i = 0u;
 
	for( i = 0u; i < DataLen; i++)/* Define variables at the beginning of the function - 2021-07-31-by zyg  */
  {
    pData[i] = *((u8 *)(Addr + i));
  }
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:
* Description:
*
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
void SDK_EEPROM_Erase(Addr_t Addr, u16 DataLen)
{
  USER_ERASE_SECTOR(Addr);
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:
* Description:
*
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
static u8 SDK_NVM_ECC_Check(void)
{
	return ((USER_NVM_ECC_CHECK() != 0u) || (USER_MAPRAM_INIT() != 0u));
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_RAM_Check
* Description:
*		RAM校验
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
static u8 SDK_RAM_Check(void)
{
	static volatile u32 *p;
	static volatile u32  tmp;
	
	RAMTestU32(tmp);

	p = (volatile u32 *)0u;
	/* polyspace-begin DEFECT:DEAD_CODE */
	if (p != 0u)
	{
		return true;           
	}
	/* polyspace-end DEFECT:DEAD_CODE */
  else
  {

  }
	
	p = (volatile u32 *)(-1);
	if (p != (volatile u32 *)(-1))
	{
		return true;            
	}
  else
  {

  }
	
	for(p = (RAM_START_ADDR); p < (RAM_END_ADDR); p++)
	{
		/*消除polyspace警告,2021-09-22-by zyg*/
		if((p != (volatile u32 *)(&p)) && (p != &tmp))/*polyspace  MISRA2012:11.3*/
		{
		  WDT1_Service();			
			RAMTestU32(*p);	 
		}
    else
    {
      
    }
	}
	
	return false;
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_FLASH_Check
* Description:
*
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
static u8 SDK_FLASH_Check(void)
{
	u32 *Addr = 0u;
	
	for(Addr = FLASH_ADDR_BGN; Addr < FLASH_ADDR_END; Addr ++)
	{
	  FlashCheckSumCal += (*Addr);
		WDT1_Service();
	}
	
	if(FlashCheckSumCal != FlashCheckSum)
	{
		return true;  
	}
  else
  {

  }
	
	return false;	
}


/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_eraseDFlash
* Description:
*
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
void SDK_eraseDFlash(Addr_t Addr, u16 DataLen)
{
  /* TLE987x只有一个DFlash Sector,所以只能擦除这个Sector，22.04.16 by yhd */
	GPT12E_T2_Stop();
  USER_ERASE_SECTOR(DataFlashStart);
	GPT12E_T2_Start();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_eraseDFlash
* Description:
*
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
void SDK_clrDFlashError(Addr_t Addr, u16 DataLen)
{
  SDK_eraseDFlash(Addr, DataLen);
  ECUFaults.bit.DFlash = SDK_NVM_ECC_Check();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_Mem_Check
* Description:
*
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
void SDK_Mem_Check(void)
{
	/*RAM校验时开关全局中断 24-01-05 by WChuang*/
  	Sdk_enterCritical();        //管中断
	ECUFaults.bit.Ram = SDK_RAM_Check();
	 Sdk_exitCritical();          //开中断
	ECUFaults.bit.CFlash = SDK_FLASH_Check();
  ECUFaults.bit.DFlash = SDK_NVM_ECC_Check();
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Sdk_writeDFlash
* Description:
*
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
void SDK_writeDFlash(Addr_t Addr, u8 *pData, u16 DataLen)
{
  Addr_t OffSet = 0u;

  while (DataLen != 0u)
  {
    GPT12E_T2_Stop();
    ProgramPage(Addr + OffSet, pData, 0u, 1u, 0u);
    GPT12E_T2_Start();

    if (DataLen >= 128u)
    {
      DataLen -= 128u;
      OffSet += 128u;
    }
    else
    {
      DataLen = 0u;
    }
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_BDRV_Off
* Description:
*
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
void SDK_BDRV_Off(void)
{
  BDRV_Set_Bridge(Ch_Off, Ch_Off, Ch_Off, Ch_Off, Ch_Off, Ch_Off);
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:SDK_BDRV_TestMos
* Description:
*
*
* Inputs:
* state:0~3
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
void SDK_BDRV_TestMos(u8 state)
{
    switch(state)
    {
      case 0u:
      {
        CCU6_LoadShadowRegister_CC60(PWN_OUT);
        CCU6_LoadShadowRegister_CC61(PWN_OUT);
        CCU6_LoadShadowRegister_CC62(PWN_OUT);
        CCU6->PSLR.reg = 0u;
        CCU6_EnableST_T12();

        CCU6->T12MSEL.bit.HSYNC = 3u;
        CCU6->MCMCTR.bit.SWSEL = 0u;
        CCU6->IEN.reg = CCU6->IEN.reg & (~(0x1000u | 0x2000u));

        CCU6->MODCTR.reg = (u16)(0x80u | 0x03);
        CCU6->MCMOUTS.reg = (u16)(0x0Bu | CCU6_MASK_MCMOUTS_SHADOW_OUT);
      
        CCU6_SetT13Trigger(0x7au);
        CCU6_SetT13Compare(CCU6_T12PR>>1u);

        CCU6_StartTmr_T12();
        /* Enable bridge (after starting PWM according to UM) */
        BDRV_Set_Bridge(Ch_PWM, Ch_PWM, Ch_PWM, Ch_PWM, Ch_PWM, Ch_PWM);
      }
      break;
      case 1u:
      {
        CCU6->MODCTR.reg = (u16)(0x80u | 0x03u);//调制cc60  cout60  
        CCU6->MCMOUTS.reg = (u16)(0x0Bu | CCU6_MASK_MCMOUTS_SHADOW_OUT);//000111
      }  
      break;
      case 2u:
      {
        CCU6->MODCTR.reg = (u16)(0x80u | 0x30u);//调制cc62  cout62 
        CCU6->MCMOUTS.reg = (u16)(0x32u | CCU6_MASK_MCMOUTS_SHADOW_OUT);//110010
      }
      break;
      case 3:
      {
        CCU6->MODCTR.reg = (u16)(0x80u | 0x0Cu);//调制cc61  cout61
        CCU6->MCMOUTS.reg = (u16)(0x2Cu | CCU6_MASK_MCMOUTS_SHADOW_OUT);//101100
      }
      break;
      default:
        BDRV_Set_Bridge(Ch_Off, Ch_Off, Ch_Off, Ch_Off, Ch_Off, Ch_Off);/* polyspace MISRA-C3:8.5 */
      break;
    }
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:MotorAPI_BDRVGetShortStatus
* Description:
*			运行中短路诊断
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
u8 SDK_BDRVGetShortStatus(void)
{
  u8 ret = 0u;

  if(((BDRV->CTRL1.reg & 0x40400000u) > 0u) || ((BDRV->CTRL2.reg & 0x00004000u) > 0u))
  {
    ret |= (1u<<0u);
  }
  else
  {

  }

  if(((BDRV->CTRL1.reg & 0x00004040u) > 0u) || ((BDRV->CTRL2.reg & 0x00000040u) > 0u))
  {
    ret |= (1u<<1u);
  }
  else
  {

  }

  return ret;
}
