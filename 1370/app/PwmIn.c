/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:PwmIn.c
********************************************************************
* Project/Product:LXT9820_PMP
* Title:
* Author:zyg
*********************************************************************
* Description:
*   Detecting PwmIn frequence and duty cycles.This module is cooperated
*   with PwmOut feed back module.
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
*     1.0       2021-09-06      zyg            Original
*
*								
*
********************************************************************
*END_FILE_HDR*/

/* Includes*/
/******************************************************************/
#include "PwmIn.h"
#include "SDK.h"

/* Defines*/
/******************************************************************/
#define PWM_IN_CAPTURE_TIMER_FREQ (1500000u) /* Hz */
#define PWM_IN_PERIOD_TIME_BASE (10u) /* ms */

#define PWM_PULSE_BUF_LEN (3u)
#define PWM_VALID_CHECK_NUM (1u)	/* PWM_VALID_CHECK_NUM must little than PWM_PULSE_BUF_LEN */

#define PWM_FREQ_ERROR (10u) /* Unit:0.1Hz */
#define PWM_DUTY_ERROR (10u) /* Unit:0.1% */

#define PWM_BUS_IDLE_TIME_MAX (1000u)	/* ms */

/* Max time delay between capture and overflow */
/* #define TimerIntNestMaxTime (0.0001) */  /* s */
/* #define TimerIntNestMaxCnt (0xFFFF - (PWM_IN_CAPTURE_TIMER_FREQ * (1.0 - 0.0001))) */
#define TimerIntNestMaxCnt (0xFFFFu - 200u)

#define LOWCNT_TEN 0x19999999uL /* 0xFFFFFFFFu/10u   ,2021-08-04-by zyg */
#define LOWCNT_HUN 0x28F5C28uL  /* 0xFFFFFFFFu/100u  ,2021-08-04-by zyg */
#define LOWCNT_THO 0x418937uL   /* 0xFFFFFFFFu/1000u ,2021-08-04-by zyg */
#define MAXBUSIDLETIME (0xFFFFu - (PWM_IN_PERIOD_TIME_BASE)) /* 0xFFFFu - (PWM_IN_PERIOD_TIME_BASE) ,2021-08-04-by zyg*/

typedef struct {
  u16 CaptureCnt;
  u32 OverFlowCnt;
  u8 NestFlag;
}Pulse_t;

typedef struct {
    Pulse_t HighPulse;
    Pulse_t LowPluse;
    Pulse_t PeriodPluse;
		Pulse_t PeriodPlusLowPulse;

    u32 HighCnt;
    u32 LowCnt;
    u32 PeriodCnt;
}Pwm_t;

typedef struct{
  u32 TimerOverflowCnt;
  u8 TimerIntNestFlag;
	u8 RisingEdgeCnt;

  Pwm_t PwmBuf[PWM_PULSE_BUF_LEN];
  u8 PwmBufIdx;

  u8 PwmCnt;
	
	u8 IsCaptureTriggered;
} PwmInVar_t;

/* Variables */
/******************************************************************/
static PwmInVar_t PwmInVar;
PwmIn_t PwmIn =
{
.CaptureEnable = 1u,
};
static u16 FreqBuf[PWM_VALID_CHECK_NUM];
static u16 DutyBuf[PWM_VALID_CHECK_NUM];

/* Function declaration */
/******************************************************************/
static u32 PwmIn_CalulatePulseWidth(Pulse_t *p);
static u16 PwmIn_CalulatePulseFreq(Pwm_t *p);
static u16 PwmIn_CalulatePulseLowDuty(Pwm_t *p);

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:PwmIn_CaptureHandler
* Description:
*   PwmIn capture interrupt handler.
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
void PwmIn_CaptureHandler(void)
{	
	u32 CaptureCnt;
	CaptureCnt = SDK_PwmInGetCaptureCnt();

	if(CaptureCnt == 0u)
	{
		/* Timer is Stopping */
	}
  else
  {
    u8 TimerResetFlag = 0u;
    if(PwmIn.CaptureEnable == 0u)
    {
      TimerResetFlag = 1u;
      PwmInVar.PwmBufIdx = 0u;
      PwmInVar.PwmCnt = 0u;
    }
    else
    {
      PwmInVar.RisingEdgeCnt++;

      Pulse_t *p = (void *)0u;
      if(PwmInVar.RisingEdgeCnt == 1u)
      {
        p = &PwmInVar.PwmBuf[PwmInVar.PwmBufIdx].LowPluse;
      }
      else if(PwmInVar.RisingEdgeCnt == 2u)
      {
        TimerResetFlag = 1u;
        
        p = &PwmInVar.PwmBuf[PwmInVar.PwmBufIdx].PeriodPlusLowPulse;

        PwmInVar.PwmBufIdx++;
        if(PwmInVar.PwmBufIdx >= (PWM_PULSE_BUF_LEN))
        {
          PwmInVar.PwmBufIdx = 0u;
        }
        else
        {

        }

        if(PwmInVar.PwmCnt < 0xFFu)
        {
          PwmInVar.PwmCnt++;
        }
        else
        {

        }
      }
      else
      {
        TimerResetFlag = 1u;
        PwmInVar.PwmBufIdx = 0u;
        PwmInVar.PwmCnt = 0u;
      }

      if(p != (void *)0u)
      {
        p->CaptureCnt = CaptureCnt;
        p->OverFlowCnt = PwmInVar.TimerOverflowCnt;
        p->NestFlag = PwmInVar.TimerIntNestFlag;
      }
      else
      {

      }
      
      PwmInVar.IsCaptureTriggered = 1u;
    }

    if(TimerResetFlag != 0)
    {
      SDK_PwmInCaptureTimerReset();
      SDK_PwmInSetCaptureRisingEdge();
      
      PwmInVar.TimerOverflowCnt = 0u;
      PwmInVar.RisingEdgeCnt = 0u;
    }
    else
    {

    }

    PwmInVar.TimerIntNestFlag = 0u;
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:PwmIn_CaptureTimerOverFlowHandler
* Description:
*   PwmIn capture timer overflow handler.
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
void PwmIn_CaptureTimerOverFlowHandler(void)
{
  if(PwmInVar.TimerOverflowCnt < 0xFFFFu)
  {
    PwmInVar.TimerOverflowCnt++;
  }
  else
 	{
  	
 	}
  
  PwmInVar.TimerIntNestFlag = SDK_PwmInGetCaptureCnt() > (TimerIntNestMaxCnt);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:PwmIn_PeriodTask
* Description:
*   Caculate PwmIn frequency and duty.
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
void PwmIn_PeriodTask(void)
{
	u8 i = 0u;
	if(PwmIn.CaptureEnable== 0u)
	{
		
	}
	else/*  add else - 2021-07-21-by zyg  */
  {
    if((PwmIn.BusIdleTime < (PWM_BUS_IDLE_TIME_MAX)) && (PwmInVar.PwmCnt >= (PWM_VALID_CHECK_NUM)))
    {
      u8 tmp;
      tmp = PwmIn.CaptureEnable;
      PwmIn.CaptureEnable = 0u;

      PwmInVar.PwmCnt = 0u;

      for( i = 0u; i < (PWM_VALID_CHECK_NUM); i++)/* Define variables at the beginning of the function - 2021-07-31-by zyg  */
      {
        u8 Idx;
        if(PwmInVar.PwmBufIdx >= (i + 1u))
        {
          Idx = PwmInVar.PwmBufIdx - i - 1u;
        }
        else
        {
          Idx = PwmInVar.PwmBufIdx + (PWM_PULSE_BUF_LEN) - i - 1u;
        }

        Pwm_t *p;
        p = &PwmInVar.PwmBuf[Idx];

        p->LowCnt = PwmIn_CalulatePulseWidth(&(p->LowPluse));
        u32 PeriodPlusLowCnt;
        PeriodPlusLowCnt = PwmIn_CalulatePulseWidth(&(p->PeriodPlusLowPulse));
        p->PeriodCnt = PeriodPlusLowCnt - p->LowCnt;

        FreqBuf[i] = PwmIn_CalulatePulseFreq(p);
        DutyBuf[i] = PwmIn_CalulatePulseLowDuty(p);
      }

      PwmIn.Freq = FreqBuf[0u];
      PwmIn.LowDuty = DutyBuf[0u];
     
      PwmIn.CaptureEnable = tmp;
    }
    else
    {
      if(PwmIn.BusIdleTime > (PWM_BUS_IDLE_TIME_MAX))
      {
        PwmInVar.PwmCnt = 0u;
        PwmInVar.PwmBufIdx = 0u;
        
        PwmIn.BusState = SDK_PwmInGetBusStatus();

        PwmIn.Freq = 0u;
				/*消除polyspace警告,2021-09-22-by zyg*/
        PwmIn.LowDuty = (PwmIn.BusState != 0u) ? 0u : 1000u;
      }
    }

    PwmIn.HighDuty = 1000u - PwmIn.LowDuty;
    
    if(PwmInVar.IsCaptureTriggered != 0u)
    {
      PwmInVar.IsCaptureTriggered = 0u;
      PwmIn.BusIdleTime = 0u;
    }
    else
    {
      if(PwmIn.BusIdleTime < (MAXBUSIDLETIME))/*0xFFFFu - (PWM_IN_PERIOD_TIME_BASE) */
      {
        PwmIn.BusIdleTime += (PWM_IN_PERIOD_TIME_BASE);
      }
    }  
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:PwmIn_CalulatePulseWidth
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
static u32 PwmIn_CalulatePulseWidth(Pulse_t *p)
{
  if((p->NestFlag) != 0u)
  {
    if(p->OverFlowCnt > 1u)
    {
      p->OverFlowCnt -= 1u;
    }
    else
    {
      p->OverFlowCnt = 0u;
    }
  }
  else
  {

  }

  return (u32)((p->OverFlowCnt<<16u) + p->CaptureCnt);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:PwmIn_CalulatePulseWidth
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
static u16 PwmIn_CalulatePulseFreq(Pwm_t *p)
{
  return ((PWM_IN_CAPTURE_TIMER_FREQ) * 10u / p->PeriodCnt);
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
static u16 PwmIn_CalulatePulseLowDuty(Pwm_t *p)
{
  u32 Low;
  u32 Period;
  u32 Coef;

  if(p->LowCnt > (LOWCNT_TEN))
  {
    Coef = 1000u;
  }
  else if(p->LowCnt > (LOWCNT_HUN))
  {
    Coef = 100u;
  }
  else if(p->LowCnt > (LOWCNT_THO))
  {
    Coef = 10u;
  }
  else
  {
    Coef = 1u;
  }

  Low = p->LowCnt / Coef * 1000u;
  Period = p->PeriodCnt / Coef;

  return (u16)(Low / Period);
}
