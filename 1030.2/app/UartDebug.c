/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
* disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:UartDebug.c
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 1.执行串口调试命令
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
#include "UartDebug.h"
#include "FanComTest.h"

#include "DTC.h"
#include "DrvAdc.h"
#include "DrvPwmIn.h"
#include "GlobalVars.h"
#include "MotorAPI.h"
#include "Sdk.h"

#include "AppMain.h" /* Model's header file */
#include "rtwtypes.h"

#include "Mc.h"

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:UartDebug_step
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
void UartDebug_step(void)
{
  if (FanComTest_getRxID() == (FAN_COM_TEST_RX_ID_UART_DEBUG))
  {
    u8 MainCmd = 0u;
    u8 SubCmd = 0u;
    u8 UartTxEn = 1u;
    u8 FaultIdx = 0u;
    DTCRecord_t FaultRecord = {.reg = 0u};
    u8 i = 0u;

    MainCmd = FanComTest_readU8(0u);
    switch (MainCmd)
    {
    case 0xF8u:
      SubCmd = FanComTest_readU8(1u);

      FanComTest_writeU8(0u, SubCmd);
      switch (SubCmd)
      {
      case 0u: {
        FanComTest_writeU16(2u, PwmInAPI.Freq);
        FanComTest_writeU16(3u, PwmInAPI.HighDuty);
      }
      break;
      case 1u: {
        FaultIdx = FanComTest_readU8(6u);
        FanComTest_writeU8(1u, FaultIdx);

        for (i = 0u; i < 6u; i++)
        {
          /* polyspace +1 MISRA-C3:10.8 */
          FaultRecord = DTC_getRecord((DTCRecordIdx_t)(FaultIdx + i));
          FanComTest_writeU8(2u + i, FaultRecord.bit.FaultTotalCnt);
        }
      }
      break;
      case 2u: {
        FanComTest_writeU8(1u, (ProductVersion.ProductType >> 24u) & 0xFFu);
        FanComTest_writeU8(2u, (ProductVersion.ProductType >> 16u) & 0xFFu);
        FanComTest_writeU8(3u, (ProductVersion.ProductType >> 8u) & 0xFFu);
        FanComTest_writeU8(4u, ProductVersion.ProductType & 0xFFu);
        FanComTest_writeU8(5u, (ProductVersion.InnerVersion.reg >> 8u) & 0xFFu);
        FanComTest_writeU8(6u, ProductVersion.InnerVersion.reg & 0xFFu);
        FanComTest_writeU8(7u, ProductVersion.DebugVersion);
      }
      break;
      case 3u: {
        FanComTest_writeS16(1u, Y.TargetSpeedLimited);
        FanComTest_writeS16(2u, MotorAPI.RefSpeed);
        FanComTest_writeS16(3u, MotorAPI.ActSpeed);
      }
      break;
      case 4u: {
        /* polyspace-begin MISRA-C3:10.8 */
        FanComTest_writeU8(1u, (u8)((MotorAPI.Faults.bit.Short << 0u) | (MotorAPI.Faults.bit.Short << 1u) |
                                    (MotorAPI.Faults.bit.Open << 2u) | (MotorAPI.Faults.bit.Lock << 3) |
                                    ((u8)(Y.VoltageStatus + 1) << 4u) | ((u8)(Y.AmbTStatus + 1) << 6u)));
        /* polyspace-end MISRA-C3:10.8 */
        FanComTest_writeU8(2u, (u8)((Y.PwmInError << 0u) | (Y.PwmInShortGND << 1u) | (Y.PwmInShortBat << 2u) |
                                    ((u8)((MotorAPI.State == eMOpenLoop) || (MotorAPI.State == eMCloseLoop)) << 3u)));
        FanComTest_writeU8(3u, 0u);
        FanComTest_writeU8(4u, 0u);
        FanComTest_writeU8(5u, Y.ShortCnt);
        FanComTest_writeU8(6u, Y.OpenCnt);
        FanComTest_writeU8(7u, Y.LockCnt);
      }
      break;
      case 5u: {
        FanComTest_writeS16(1u, AdcResults.Temperature.Ambient);
        FanComTest_writeS16(2u, AdcResults.Temperature.Chip);
        FanComTest_writeU16(3u, AdcResults.Voltage.BatFilt * 11u);
      }
      break;
      case 6u: {
        FanComTest_writeS16(1u, MotorAPI.WmSpeed);
      }
      break;
      case 7u: {
        FaultIdx = FanComTest_readU8(6u);
        FanComTest_writeU8(1u, FaultIdx);

        for (i = 0u; i < 6u; i++)
        {
          /* polyspace +1 MISRA-C3:10.8 */
          FaultRecord = DTC_getRecord((DTCRecordIdx_t)(FaultIdx + i));
          FanComTest_writeU8(2u + i, FaultRecord.bit.FaultRePowerInfo.RePowerCnt);
        }
      }
      break;
      case 8u:
        DtcDataSwRstEn = 1u;
        FanComTest_rstU8(1);
        break;
      case 9u:/*读取电机库版本号*/
        FanComTest_writeU8(1u, (McAPI.McLibVer >> 8u) & 0xFFu);
        FanComTest_writeU8(2u, McAPI.McLibVer & 0xFFu);
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
      FanComTest_startFrameTx(FAN_COM_TEST_TX_ID_UART_DEBUG);
    }
  }
}
