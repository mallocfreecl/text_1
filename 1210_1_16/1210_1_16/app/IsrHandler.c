/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:IsrHandler.c
********************************************************************
* Project/Product:LXT9818_PMP
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
* Version       Date         	  Author         Descriptions
* ----------    --------------  ------------   ----------------------------------------
*     1.0       2021-07-23      yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#include "Emo_RAM.h"
#include "TestMode.h"
#include "Main.h"
#include "SDK.h"
#include "FanComTest.h"
#include "PwmIn.h"
#include "MotorAPI.h"
#include "tle_device.h"
#include "timer2x_defines.h"
#include "GlobalVars.h"

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
  FanComTest_RxHandler();
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
  FanComTest_TxHandler();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Timer2CaptureHandler
* Description:
*   Timer2 is used for PwmIn capture.
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
  PwmIn_CaptureHandler();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Timer2OverflowHandler
* Description:
*   Timer2 is used for PwmIn capture.
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
  PwmIn_CaptureTimerOverFlowHandler();
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Gpt2_T6_Handler
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
void Gpt2_T6_Handler(void)
{

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
	//TimeMeasureStart(); 
		
  /* T2 overflow: */
  /* Set T2 to reload value */
  GPT12E->T2.reg += GPT12E_T2;
  
  if(Emo_Status.MotorState != EMO_MOTOR_STATE_STOP)
  {
    Emo_RAM_MHighTask();
  }
  else
  {

  }
	
	//TimeMeasureEnd(0); 
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
	//TimeMeasureStart();
	
  Emo_Status.LowTaskTime += 1u;
  if(Emo_Status.LowTaskTime >= (M_LOW_TASK_TIME_NUM))/* (M_Low_Task_Time)/1000u) -2021-08-04-by zyg*/
  {
    Emo_Status.LowTaskTime = 0u;
		
    Emo_MLowTask();
  }
  else
  {

  }
	
	MsCnt1 += 1u;
	
	//TimeMeasureEnd(0); 
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

