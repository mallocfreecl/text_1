/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
*disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:DrvIsr.c
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
*   Provide isr interface here.
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
#include "DrvAdc.h"
#include "DrvPwmIn.h"
#include "ErtMain.h"
#include "FanComTest.h"
#include "FctTest.h"
#include "GlobalVars.h"
#include "McCfg.h"
#include "MotorAPI.h"
#include "Sdk.h"
#include "SdkTypes.h"
#include "UartDebug.h"

#include "ccu6_defines.h"
#include "gpt12e_defines.h"
#include "timer2x_defines.h"
#include "tle_device.h"

/* Defines*/
/******************************************************************/
#define ISR_HANDLER_TIME_US_BASE (150u) // us

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:BDRV_Diag_Supply
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
void BDRV_Diag_Supply(void)
{
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:BDRV_Diag
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
void BDRV_Diag(void)
{
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:BDRV_Diag
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
void ADC1_EIM_Handler(void)
{
  //  PORT->P1_DATA.bit.P4 = ~PORT->P1_DATA.bit.P4;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Uart2RxHandler
* Description:
*   Uart2 is used for debuging and fct testing.
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
void Uart2RxHandler(void)
{
  FanComTest_rxHandler();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Uart2TxHandler
* Description:
*   Uart2 is used for debuging and fct testing.
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
void Uart2TxHandler(void)
{
  FanComTest_txHandler();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Timer2CaptureHandler
* Description:
*   Timer2 is used for PwmInAPI capture.
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
void Timer2CaptureHandler(void)
{
  DrvPwmIn_capHandler();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Timer2OverflowHandler
* Description:
*   Timer2 is used for PwmInAPI capture.
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
void Timer2OverflowHandler(void)
{
  DrvPwmIn_capTmrOverFlowHandler();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:GPT1_T2_Handler
* Description:
*   T2 is used for motor high priority task
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
void GPT1_T2_Handler(void)
{
  /* T2 overflow: */
  /* Set T2 to reload value */

  if (IsInFctTestMode == 0u)
  {
    GPT12E->T2.reg += GPT12E_T2;

    Mc_HighTask();
  }
  else
  {
    /* Fct测试时将中断时间设为100us，和旧的测试程序兼容 */
    GPT12E->T2.reg += (U16_MAX - ((U16_MAX - GPT12E_T2) >> 1u));

    if (FctTestMOSEnd == 0u)
    {
      FctTest_testBDRV();
    }

    AdcResults.RawAD.VoltU = ADC1->RES_OUT0.bit.OUT_CH0;
    AdcResults.RawAD.VoltV = ADC1->RES_OUT2.bit.OUT_CH2;
    AdcResults.RawAD.VoltW = ADC1->RES_OUT3.bit.OUT_CH3;
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:AppTimeBaseHandler
* Description:
*   SysTick interrupt is used for motor low priority task and time
*   base generate.
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
void AppTimeBaseHandler(void)
{
  static u8 McLowTaskTimeCnt = 0u;

  McLowTaskTimeCnt += 1u;
  if (McLowTaskTimeCnt >= (McCfg.LowTaskPeriod / 1000u))
  {
    McLowTaskTimeCnt = 0u;

    Mc_LowTask();
  }

  Ms1 += 1u;
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:GPT1_T4_Handler
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
void GPT1_T4_Handler(void)
{
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:GPT1_T4_Handler
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
void ADC1_ESM_Handler(void)
{
  // PORT->P1_DATA.bit.P4 = ~PORT->P1_DATA.bit.P4;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:GPT1_T4_Handler
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
void ADC1_P023_Handler(void)
{
  // PORT->P1_DATA.bit.P4 = ~PORT->P1_DATA.bit.P4;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:T12_OM_Handler
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
void T12_OM_Handler(void)
{
  if (IsInFctTestMode == 0u)
  {
    Mc_PwmStartHandler();
  }
  else
  {
    AdcResults.RawAD.IShunt = ADC1->RES_OUT_EIM.bit.OUT_CH_EIM >> 2;

    uint16 Time = 0u;

    Time = U16_MAX - (CCU6_T12PR + 80);
    TIMER21->T2CON.bit.TR2 = 0u;
    TIMER21->T2H.reg = (u8)(Time >> 8u);
    TIMER21->T2L.reg = (u8)(Time & 0xFFu);
    TIMER21->T2CON.bit.TR2 = 1u;
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:T12_PM_Handler
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
void T12_PM_Handler(void)
{
  if (IsInFctTestMode == 0u)
  {
    Mc_PwmPeriodHandler();
  }
}
