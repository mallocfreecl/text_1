/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:PwmIn.c
********************************************************************
* Project/Product:LXT9818_PMP
* Title:
* Author:yhd
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
*     1.0       2021-07-23      yhd            Original
*			1.0       2021-07-31      zyg            1.PwmIn_PeriodTask():  Define variables at the beginning of the function
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
/*修改为占空比变化超0.5%使用新的占空比，24.03.05 by WChuang*/
#define PWM_DUTY_ERROR (5u) /* Unit:0.1% */

#define PWM_BUS_IDLE_TIME_MAX (1000u)	/* ms */
#define PWM_MIN_PEROIDCNT_VALUE (((PWM_IN_CAPTURE_TIMER_FREQ) * 10u ) >> 16)

/* Max time delay between capture and overflow */
/* #define TimerIntNestMaxTime (0.0001) */  /* s */
/* #define TimerIntNestMaxCnt (0xFFFF - (PWM_IN_CAPTURE_TIMER_FREQ * (1.0 - 0.0001))) */
#define TimerIntNestMaxCnt (0xFFFFu - 600u)

#define HIGHCNT_TEN 0x19999999uL /* 0xFFFFFFFFu/10u   ,2021-08-04-by zyg */
#define HIGHCNT_HUN 0x28F5C28uL  /* 0xFFFFFFFFu/100u  ,2021-08-04-by zyg */
#define HIGHCNT_THO 0x418937uL   /* 0xFFFFFFFFu/1000u ,2021-08-04-by zyg */
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
		/*存储高占空比捕获计数，24.03.02 by WChuang*/
		Pulse_t PeriodPlusHighPulse;

    u32 HighCnt;
    u32 LowCnt;
    u32 PeriodCnt;
}Pwm_t;

typedef struct{
  u32 TimerOverflowCnt;
  u8 TimerIntNestFlag;
	u8 RisingEdgeCnt;
	/*下降沿捕获计数，24.03.02 by WChuang*/
	u8 FallingEdgeCnt;
	
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
/*高占空比计算函数，24.03.02 by WChuang*/
static u16 PwmIn_CalulatePulseHighDuty(Pwm_t *p);

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
			/*修改为上升沿触发下降沿捕获，直接计算高占空比，24.03.02 by WChuang*/
			PwmInVar.FallingEdgeCnt++;

      Pulse_t *p = (void *)0u;
      if(PwmInVar.FallingEdgeCnt == 1u)
      {
        p = &PwmInVar.PwmBuf[PwmInVar.PwmBufIdx].HighPulse;
      }
      else if(PwmInVar.FallingEdgeCnt == 2u)
      {
        TimerResetFlag = 1u;
        
        p = &PwmInVar.PwmBuf[PwmInVar.PwmBufIdx].PeriodPlusHighPulse;

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
			/*下降沿捕获，24.02.02 by WChuang*/
			SDK_PwmInSetCaptureFallingEdge();
      
      PwmInVar.TimerOverflowCnt = 0u;
	  /*捕获计数清零，24.03.02 by WChuang*/
			PwmInVar.FallingEdgeCnt = 0u;
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
    /*修改溢出中断处理逻辑 22.11.16 by lxs*/
	u16 CapCnt;
	if(SDK_PwmInIsCaptureTimerRunning() != 0)
	{
  if(PwmInVar.TimerOverflowCnt < 0xFFFFu)
  {
    PwmInVar.TimerOverflowCnt++;
  }
  else
 	{
  	
 	}
  
		if (SDK_IsPwmInCap() != 0u)
		{
			CapCnt = SDK_PwmInGetCaptureCnt();
			PwmInVar.TimerIntNestFlag = ((CapCnt > (TimerIntNestMaxCnt)) || (CapCnt == 0u));
		}
		else
		{
			PwmInVar.TimerIntNestFlag = 0u;
		}
	}
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
				
				/*获取高占空比计数和周期计数，计算频率和周期，24.03.02 by WChuang*/
				p->HighCnt = PwmIn_CalulatePulseWidth(&(p->HighPulse));
        u32 PeriodPlusHighCnt;
        PeriodPlusHighCnt = PwmIn_CalulatePulseWidth(&(p->PeriodPlusHighPulse));
        p->PeriodCnt = PeriodPlusHighCnt - p->HighCnt;

        FreqBuf[i] = PwmIn_CalulatePulseFreq(p);
        DutyBuf[i] = PwmIn_CalulatePulseHighDuty(p);
      }

      PwmIn.Freq = FreqBuf[0u];
			/*占空比变化超过0.5%使用新占空比 24.03.02 by WChuang*/
      if (((DutyBuf[0u] + PWM_DUTY_ERROR) <= PwmIn.HighDuty) ||
          (DutyBuf[0u] >= (PwmIn.HighDuty + PWM_DUTY_ERROR)))
      {
        PwmIn.HighDuty = DutyBuf[0u];
      }
			
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
				/*短电高占空比设置为100%，短地为0%，24.03.02 by WChuang*/
				/*消除polyspace警告,2021-09-22-by zyg*/
        PwmIn.HighDuty = (PwmIn.BusState != 0u) ? 1000u : 0u;
      }
    }

    
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
	u16 Freq = U16_MAX;
	if(p->PeriodCnt > PWM_MIN_PEROIDCNT_VALUE)
	{
      Freq = ((PWM_IN_CAPTURE_TIMER_FREQ) * 10u / p->PeriodCnt);
	}
	return Freq;
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
static u16 PwmIn_CalulatePulseHighDuty(Pwm_t *p)
{
	/*计算高占空比 24.03.02 by WChuang*/
  u32 High = 0;
  u32 Period = 0;
  u32 Coef = 0;
	u32 duty = U16_MAX;

  if(p->HighCnt > (HIGHCNT_TEN))
  {
    Coef = 1000u;
  }
  else if(p->HighCnt > (HIGHCNT_HUN))
  {
    Coef = 100u;
  }
  else if(p->HighCnt > (HIGHCNT_THO))
  {
    Coef = 10u;
  }
  else
  {
    Coef = 1u;
  }

  High = p->HighCnt / Coef * 1000u;
  Period = p->PeriodCnt / Coef;
	/*防止出现Period=0的情况 22.09.01 by lxs*/
  if(Period !=0)
	{
      duty = (u16)(High / Period);
	} 
	return duty;
}
