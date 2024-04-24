/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:TestMode.c
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
*     1.0       2021-07-23      yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#include "TestMode.h"
#include "FanComTest.h"

#include "SDK.h"
#include "ADCSample.h"
#include "PwmIn.h"
#include "DTC.h"
#include "userinfo.h"
#include "Emo_RAM.h"
#include "MotorAPI.h"

#include "AppMain.h"                   /* Model's header file */
#include "rtwtypes.h"

/* Defines*/
/******************************************************************/
#define FCT_TEST_TIME_BASE (50u)               /* Time base of TestMode_FctTest */
#define FCT_ENTRY_TIME_OUT_TIME (5000u)        /* Unit:ms */
#define FCT_TEST_MOS_UART_TX_DELAY_TIME (200u) /* Unit:ms */
#define FCT_TEST_TEMPERATURE_OFFSET (80)

/* Variables */
/******************************************************************/
u8 IsInTestMode = 0u;
u8 IsInFctTestMode = 0u;
u8 IsInTerminalTestMode = 0u;
u8 FctTestMOSEn = 0u;
u8 FctTestPwmOut5HzEn = 0u;

/* Function declaration */
/******************************************************************/
static void TestMode_FctTest(void);
static void TestMode_TerminalTest(void);

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:TestMode_TestMOS
* Description:
*   Switch six mos in turn.
*   Switch sequence:HS0->LS1,HS1->LS2,HS2->LS0.
*   Call this function every pwm period.
*
* Inputs:
* 1.FctTestMOSEn:
* FctTestMOSEn == 0 : Set BDRV off.
* FctTestMOSEn != 0 : Switch six mos in turn.
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
void TestMode_TestMOS(void)
{
  static u8 MOSSwitchState=0u;

  if(FctTestMOSEn == 0u)
  {
    MOSSwitchState = 0u;
  }
  else
  {
    SDK_BDRV_TestMos(MOSSwitchState);

    MOSSwitchState++;
    if(MOSSwitchState > 3u)
    {
      MOSSwitchState = 1u;
    }
		else/* add else - 2021-07-28-by zyg*/
		{
			
		}
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:TestMode_UartDebugPeriodTask
* Description:
*   Implement uart debug command
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
void TestMode_UartDebugPeriodTask(void)
{
  if (FanComTest_GetRxID() == (RX_ID_UART_DEBUG))
  {
		u8 UartDebugCmd;
    u8 UartTxEn = 1u;
		
    u8 SubCmd = 0u;
    u8 FaultIdx = 0u;
    DTCRecord_t FaultRecord = {.reg = 0u};
    u8 i = 0u;

    UartDebugCmd = FanComTest_ReadU8(0u);
    switch (UartDebugCmd)
    {
    case 0xF8u:
      SubCmd = FanComTest_ReadU8(1u);

      FanComTest_WriteU8(0u, SubCmd);
      switch (SubCmd)
      {
      case 0u: {
        FanComTest_WriteU16(2u, PwmIn.Freq);
        FanComTest_WriteU16(3u, PwmIn.HighDuty);
      }
      break;
      case 1u: {
        FaultIdx = FanComTest_ReadU8(6u);
        FanComTest_WriteU8(1u, FaultIdx);

        for (i = 0u; i < 6u; i++)
        {
          /* polyspace +1 MISRA-C3:10.8 */
          FaultRecord = DTC_GetRecord((DTCRecordIdx_t)(FaultIdx + i));
          FanComTest_WriteU8(2u + i, FaultRecord.bit.FaultTotalCnt);
        }
      }
      break;
      case 2u: {
	  /*增加FCT版本号，24-02-23，by WChuang*/
        FanComTest_WriteU8(1u, (ProductVersion.ProductType >> 8u) & 0xFFu);
        FanComTest_WriteU8(2u, ProductVersion.ProductType & 0xFFu);
        FanComTest_WriteU8(3u, (ProductVersion.InnerVersion.reg >> 8u) & 0xFFu);
        FanComTest_WriteU8(4u, ProductVersion.InnerVersion.reg & 0xFFu);
				FanComTest_WriteU8(5u, ProductVersion.FctVersion);
        FanComTest_WriteU8(6u, ProductVersion.DebugVersion);
				FanComTest_WriteU8(7u, ProductVersion.McLib);
      }
      break;
      case 3u: {
        FanComTest_WriteS16(1u, Y.TargetSpeedLimited);
        FanComTest_WriteS16(2u, MotorAPIVar.RefSpeed);
        FanComTest_WriteS16(3u, MotorAPIVar.ActSpeed);
      }
      break;
      case 4u: {
        /* polyspace-begin MISRA-C3:10.8 */
        FanComTest_WriteU8(1u, (u8)((MotorAPIVar.Faults.bit.Short << 0u) | (MotorAPIVar.Faults.bit.Short << 1u) |
                                    (MotorAPIVar.Faults.bit.Open << 2u) | (MotorAPIVar.Faults.bit.Lock << 3) |
                                    ((u8)(Y.VoltageStatus + 1) << 4u) | ((u8)(Y.AmbTStatus + 1) << 6u)));
        /* polyspace-end MISRA-C3:10.8 */
        FanComTest_WriteU8(2u, (u8)((Y.PwmInError << 0u) | (Y.PwmInShortGND << 1u) | (Y.PwmInShortBat << 2u) |
                                    ((u8)((Emo_Status.MotorState == EMO_MOTOR_STATE_START) || (Emo_Status.MotorState == EMO_MOTOR_STATE_RUN)) << 3u)));
        FanComTest_WriteU8(3u, 0u);
        FanComTest_WriteU8(4u, 0u);
        FanComTest_WriteU8(5u, Y.ShortCnt);
        FanComTest_WriteU8(6u, Y.OpenCnt);
        FanComTest_WriteU8(7u, Y.LockCnt);
      }
      break;
      case 5u: {
        FanComTest_WriteS16(1u, ADCResults.Temperature.Ambient);
        FanComTest_WriteS16(2u, ADCResults.Temperature.Chip);
        FanComTest_WriteU16(3u, ADCResults.Voltage.BatFiltered * 11u);
      }
      break;
      case 6u: {
				/*无自传转速*/
        //FanComTest_WriteS16(1u, MotorAPI.WmSpeed);
				FanComTest_WriteS16(1u, 0u);
      }
      break;
      case 7u: {
        FaultIdx = FanComTest_ReadU8(6u);
        FanComTest_WriteU8(1u, FaultIdx);

        for (i = 0u; i < 6u; i++)
        {
          /* polyspace +1 MISRA-C3:10.8 */
          FaultRecord = DTC_GetRecord((DTCRecordIdx_t)(FaultIdx + i));
          FanComTest_WriteU8(2u + i, FaultRecord.bit.FaultRePowerInfo.RePowerCnt);
        }
      }
      break;
      case 8u:
        DtcDataSwRstEn = 1u;
        FanComTest_ResetU8(1);
        break;
			/*增加效验和的检查 2024-02-22 by W*/
			case 0x0Au: {
          FanComTest_WriteU8(0u, 0);
					FanComTest_WriteU8(1u, 0u);
					FanComTest_WriteU8(2u, 0u);
					FanComTest_WriteU8(3u, 0u);
					FanComTest_WriteU8(4u, (FlashCheckSumCal>>24u)&0xFFu);
					FanComTest_WriteU8(5u, (FlashCheckSumCal>>16u)&0xFFu);
					FanComTest_WriteU8(6u, (FlashCheckSumCal>>8u)&0xFFu);
					FanComTest_WriteU8(7u,  FlashCheckSumCal & 0xFFu);
      }
			break;
      default: /* 不做任何处理 */
        break;
      }
      break;
    default:
      UartTxEn = 0u;
      break;
    }

    if (UartTxEn != 0u)
    {
      FanComTest_StartFrameTransmission(TX_ID_UART_DEBUG);
    }
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:TestMode_FctTest
* Description:
*   1.Fct test.
*   1.1 Test Mos and current sample
*   1.2 Feed Back ECU status
*
* Inputs:
*   Uart command
*
* Outputs:
*
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static void TestMode_FctTest(void)
{
  static u8 FctEntryTimeOutCnt = (FCT_ENTRY_TIME_OUT_TIME)/(FCT_TEST_TIME_BASE);
  static u8 FctCmd = 0u;
  static u8 FctCmdLast = 0u;
  static u8 FctTxEn = 0u;
  static u8 FctTxID = 0u;
  static u8 Fct_CalIShuntZeroComplete = 0u;
  u8 FctTxBuf[8u] = {0};/*消除polyspace错误,2021-09-17-by zyg*/
	u8 i = 0u;
	u8 ErrorCode = 0u;/* 根据故障状态返回故障码，24.02.24 by WChuang */

  /* after power up,there is 5S get shakehand command from  Super_Computer */
  if((IsInFctTestMode == 0u) && (FctEntryTimeOutCnt > 0u))
  {
    /*5S cnt time up then stop get shakehand command from  Super_Computer*/
    FctEntryTimeOutCnt--;

    FctCmd = FanComTest_GetRxID();
    /*get shakehand command then jump into test mode*/
    if((FctCmd == RX_ID_ENTRY_FCT_TEST_MODE) 
    && (FanComTest_ReadU8(0u) == 0x01u) && (FanComTest_ReadU8(4u) == 0x05u) && (FanComTest_ReadU8(7u) == 0x08u))
    {
      IsInFctTestMode = 1u;
      FctEntryTimeOutCnt = 0u;
			
			MotorAPI_MotorSetSpeed(0);
    }
    else
    {

    }
  }
  else
  {
    FctEntryTimeOutCnt = 0u;
  }

  /* jump into test mode if ture */
  if(IsInFctTestMode == 1u)
  {
    FctCmd = FanComTest_GetRxID();

    if(FctCmd != RX_ID_TEST_MOS)
    {
      FctTestMOSEn = 0u;
      SDK_BDRV_Off();
    }
		else/* add else - 2021-07-28-by zyg*/
		{
			
		}

    if(FctCmd != RX_ID_TEST_PWM_OUT)
    {
      FctTestPwmOut5HzEn = 0u;
    }
		else/* add else - 2021-07-28-by zyg*/
		{
			
		}
    switch(FctCmd)
    {
      case RX_ID_ENTRY_FCT_TEST_MODE:
      {
        FctTxEn = 1u;
        FctTxID = TX_ID_ENTRY_FCT_TEST_MODE;

        FctTxBuf[0u] = 0x01u;
        FctTxBuf[1u] = 0x02u;
        FctTxBuf[2u] = 0x03u;
        FctTxBuf[3u] = 0x04u;
        FctTxBuf[4u] = 0x05u;
        FctTxBuf[5u] = 0x06u;
        FctTxBuf[6u] = 0x07u;
        FctTxBuf[7u] = 0x08u;
		/*FCT进行零点标定，且只标定一次 23.05.12 by zjj*/
		if(Fct_CalIShuntZeroComplete == 0u)
		{
			FctEmo_calIShuntZero();			
			Fct_CalIShuntZeroComplete = 1u;		
		}	
      }
      break;
      case RX_ID_TEST_MOS:
      {
        static u8 ThisStateTimeCnt = 0u;
        static u8 IsFrame3Send = 0u;
        static u8 FrameCnt = 0u;
				static u8 BdrvStatus = 0u;

				/* Fct测试时如果诊断短路需要进行确认,2022-12-15-by zjj */
				BdrvStatus = SDK_BDRVGetShortStatus();	
        if(FctCmdLast != RX_ID_TEST_MOS)
        {
          ThisStateTimeCnt = 0u;
          IsFrame3Send = 0u;
          FrameCnt = 0u;
          FctTestMOSEn = 1u;
					TestMode_TestMOS();/* 打开定时器,2022-12-15-by zjj */
        }
        else
        {

        }

        if(ThisStateTimeCnt < 0xFFu)
        {
          ThisStateTimeCnt++;
        }
        else
        {
        }

        if(ThisStateTimeCnt > ((FCT_TEST_MOS_UART_TX_DELAY_TIME)/(FCT_TEST_TIME_BASE)))
        {
          FctTxEn = 1u;
          FctTxID = TX_ID_TEST_MOS;

          FctTxBuf[0u] = FrameCnt;
          switch(FrameCnt)
          { 
            case 0u:
            {
              FrameCnt = 1u;
							/*消除polyspace警告,2021-09-23-by zyg*/
              FctTxBuf[1u] = (FlashECUInfo[0u] & 0xF0u) | (u8)((((ProductVersion.ProductType) & 0xFFu)>>4u) & 0x0Fu);
              FctTxBuf[2u] = (FlashECUInfo[1u] & 0x0Fu) | (u8)((((ProductVersion.ProductType) & 0xFFu) & 0x0Fu)<<4u);
              FctTxBuf[3u] = FlashECUInfo[2u];
              FctTxBuf[4u] = FlashECUInfo[3u];
              FctTxBuf[5u] = FlashECUInfo[4u];
              FctTxBuf[6u] = FlashECUInfo[5u];
              FctTxBuf[7u] = FlashECUInfo[6u];
            }
            break;
            case 1u:
            {
              FrameCnt = 2u;

              FctTxBuf[1u] = FlashECUInfo[7u];
              FctTxBuf[2u] = FlashECUInfo[8u];
              FctTxBuf[3u] = FlashECUInfo[9u];
              FctTxBuf[4u] = FlashECUInfo[10u];
              FctTxBuf[5u] = FlashECUInfo[11u];
              FctTxBuf[6u] = ADCResults.RawAD.Bat;
              FctTxBuf[7u] = ADCResults.RawAD.VSD;
            }
            break;
            case 2u:
            {
              FrameCnt = 3u;

              FctTxBuf[1u] = ADCResults.RawAD.VCP;
              FctTxBuf[2u] = ADCResults.RawAD.Mon;
              FctTxBuf[3u] = ADCResults.RawAD.VDDP;
              FctTxBuf[4u] = ADCResults.RawAD.VAREF;
              FctTxBuf[5u] = ADCResults.RawAD.VBG;
              FctTxBuf[6u] = ADCResults.RawAD.VDDC;
              FctTxBuf[7u] = (ADCResults.Temperature.Chip + (FCT_TEST_TEMPERATURE_OFFSET));
            }
            break;
            case 3u:
            {
							FrameCnt = 4u;
							
                FctTxBuf[1u] = (ADCResults.Temperature.Ambient + (FCT_TEST_TEMPERATURE_OFFSET));
                FctTxBuf[2u] = (ADCResults.RawAD.Current.DC>>8u);
                FctTxBuf[3u] = ADCResults.RawAD.Current.DC;
                FctTxBuf[4u] = (PwmIn.HighDuty>>8u);
                FctTxBuf[5u] = PwmIn.HighDuty & 0xFFu;
                FctTxBuf[6u] = (((u8)((ECUFaults.bit.Ram != 0u) || (ECUFaults.bit.DFlash != 0u)))<<7u) 
                             | (((u8)((ECUFaults.bit.SysOT != 0u )|| (ECUFaults.bit.SuppShort != 0u) || (ECUFaults.bit.SuppTimeOut != 0u)))<<6u)
                             | (((u8)((ECUFaults.bit.VDDEXT_OverLoad != 0u) || (ECUFaults.bit.VDDEXT_OverVoltage != 0u) || (ECUFaults.bit.VDDEXT_Short != 0u) 
                              || (ECUFaults.bit.PMU_5V_OV != 0u) || (ECUFaults.bit.PMU_1V5_OV != 0u)))<<5u)
                             | ((u8)(ECUFaults.bit.Ref5VError != 0u)<<4u)
                             | ((u8)(ECUFaults.bit.ADC1ResultInvalid != 0u)<<3u)
							               | ((u8)(BdrvStatus != 0u)<<2u)
							               | ((u8)(CSAZeroFault.bit.CSAZeroCalibrationFault != 0u)<<1u);
                FctTxBuf[7u] = ProductVersion.FctVersion;
						}
						break;
						case 4u:
						{
							if(IsFrame3Send == 0u)     /* 增加输入占空比反馈, 2022-12-15-by zjj  */
							{
                FctTxBuf[1u] = (PwmIn.Freq >> 8u);
                FctTxBuf[2u] = PwmIn.Freq & 0xFFu;
                FctTxBuf[3u] = 0;
                FctTxBuf[4u] = 0;
                FctTxBuf[5u] = 0;
                FctTxBuf[6u] = 0;
                FctTxBuf[7u] = 0;
                IsFrame3Send = 1u;
              }
              else
              {
                FctTxEn = 0u;
                FrameCnt = 0u;
                IsFrame3Send = 0u;
              }
            }
            break;
            default:
            {
              FctTxEn = 0u;
              FrameCnt = 0u;
            }
            break;
          }
		/* 根据故障状态返回故障码，24.02.24 by WChuang */
		if (ADCResults.ErrFlags.IShuntZero != 0u)
		{
			ErrorCode |= (1u << 0);				
		}
		/* 如果电流零点没有成功存入DFlash,则反馈错误。24.02.24 by WChuang */
					if (DtcIShuntWriteEn != 0u)
					{
						ErrorCode |= (1u << 1);	
					}

					if (ErrorCode != 0u)
					{
						for (i = 1u; i < 8u; i++)
						{
							FctTxBuf[i] = ErrorCode;
						}
          }
        }
				else/* add else - 2021-07-28-by zyg*/
				{
			
				}
      }
      break;
      case RX_ID_TEST_PWM_OUT:
			{
				FctTestPwmOut5HzEn = 1u;	
			}
			break;
      default:
        FctTxEn = 0u;
        break;
    }
  }
  else
  {

  }

  if(FctTxEn == 1u)
  {
    FctTxEn = 0u;

    for( i = 0u; i < 8u; i++)/* Define variables at the beginning of the function - 2021-07-31-by zyg  */
    {
			FanComTest_WriteU8(i, FctTxBuf[i]);
    }
    FanComTest_StartFrameTransmission(FctTxID);
  }
  else
  {

  }

  FctCmdLast = FctCmd;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:TestMode_TerminalTest
* Description:
*   Implement this in the futrue.
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
static void TestMode_TerminalTest(void)
{
  IsInTerminalTestMode = 0u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:TestMode_FactoryTest
* Description:
*   Call this function every 50ms
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
void TestMode_FactoryTest(void)
{
  TestMode_FctTest();
  TestMode_TerminalTest();
	/*消除polyspace警告,2021-09-23-by zyg*/
  IsInTestMode = (IsInFctTestMode != 0u) || (IsInTerminalTestMode != 0u);
}

