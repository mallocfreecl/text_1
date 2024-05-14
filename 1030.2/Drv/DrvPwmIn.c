/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or
* disclosed without the written authorization from XiangBin Electric.
********************************************************************
* File Name:DrvPwmIn.c
********************************************************************
* Project/Product:t3622
* Title:
* Author:yhd
*********************************************************************
* Description:
* 检测Pwm输入信号的频率、占空比，信号接地、接电源状态
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
#include "DrvPwmIn.h"
#include "Sdk.h"

/* Defines*/
/******************************************************************/
#define DRV_PWM_IN_CAPTURE_TIMER_FREQ (1500000uL) // Hz

#define DRV_PWM_IN_PWM_PULSE_BUF_LEN (3u)
#define DRV_PWM_IN_PWM_VALID_CHECK_NUM (1u)

#if (DRV_PWM_IN_PWM_VALID_CHECK_NUM) > (DRV_PWM_IN_PWM_PULSE_BUF_LEN)
#error WM_IN_PWM_VALID_CHECK_NUM must little than DRV_PWM_IN_PWM_PULSE_BUF_LEN
#endif
/*修改遗留的宏定义名称问题 22.04.06 by jxj*/
#define DRV_PWM_IN_PWM_FREQ_ERROR (10u) // Unit:0.1Hz
#define DRV_PWM_IN_PWM_DUTY_ERROR (5u)  // Unit:0.1%

#define DRV_PWM_IN_PWM_BUS_IDLE_TIME_MAX (1000u) // ms

/*
Max time delay between capture and overflow
*/
//#define TimerIntNestMaxTime (0.0001)  //s
//#define DRV_PWM_IN_TIMER_INT_NEST_MAX_CNT (0xFFFF - (DRV_PWM_IN_CAPTURE_TIMER_FREQ *
//(1.0 - 0.0001)))
#define DRV_PWM_IN_TIMER_INT_NEST_MAX_CNT ((U16_MAX)-1000u)

typedef struct
{
  u16 CaptureCnt;
  u32 OverFlowCnt;
  u8 IsCaptureBeforeOverFlow;
} Pulse_t;

typedef struct
{
  Pulse_t HighPulse;
  Pulse_t LowPluse;
  Pulse_t PeriodPluse;
  Pulse_t PeriodPlusLowPulse;

  u32 HighCnt;
  u32 LowCnt;
  u32 PeriodCnt;
} Pwm_t;

typedef struct
{
  u32 TimerOverflowCnt;
  u8 IsCaptureBeforeOverFlow;
  u8 RisingEdgeCnt;

  Pwm_t PwmBuf[DRV_PWM_IN_PWM_PULSE_BUF_LEN];
  u8 PwmBufIdx;

  u8 PwmCnt;

  u8 IsCaptureTriggered;
} PwmInVar_t;

/* Variables */
/******************************************************************/
PwmInAPI_t PwmInAPI = {
  .CaptureEn = 1u,
};
static PwmInVar_t PwmInVar;

static u16 FreqBuf[DRV_PWM_IN_PWM_VALID_CHECK_NUM];
static u16 DutyBuf[DRV_PWM_IN_PWM_VALID_CHECK_NUM];

/* Function declaration */
/******************************************************************/
static u32 DrvPwmIn_calPulseWidth(Pulse_t *p);
static u16 DrvPwmIn_calPulseFreq(Pwm_t *p);
static u16 DrvPwmIn_calPulseLowDuty(Pwm_t *p);

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:DrvPwmIn_capHandler
* Description:
* 1.控制定时器触发时序
* 2.计算Pwm信号周期、低电平长度
*
* Inputs:
* 1.PwmInAPI.CaptureEn：0：禁止捕获输入信号，1：允许捕获输入信号
* 2.PwmInVar.TimerOverflowCnt:捕获定时器溢出计数
* 3.PwmInVar.IsCaptureBeforeOverFlow：捕获中断和溢出中断发生嵌套标志   谁嵌套谁？谁的中断优先级比较高？
*
* Outputs:
* 1.PwmInVar.PwmBuf[].LowPluse:低电平长度结构体
* 2.PwmInVar.PwmBuf[].PeriodPlusLowPulse：周期长度结构体
* 3.PwmInVar.PwmCnt：捕获到的完整Pwm周期个数
* 4.PwmInVar.IsCaptureTriggered:0：未捕获到Pwm输入信号变化，1：捕获到
*   Pwm输入信号边沿变化
*
* Limitations:
* 1.必须放在定时器捕获中断中执行
********************************************************************
END_FUNCTION_HDR*/
void DrvPwmIn_capHandler(void)
{
  u32 CaptureCnt = 0u;
  u8 TimerResetFlag = 0u;
  Pulse_t *p = (void *)0u;

  CaptureCnt = Sdk_getPwmInCapVal();
  if (CaptureCnt != 0u)
  {
    if (PwmInAPI.CaptureEn == 0u)
    {
      TimerResetFlag = 1u;
      PwmInVar.PwmBufIdx = 0u;
      PwmInVar.PwmCnt = 0u;
    }
    else
    {
      PwmInVar.RisingEdgeCnt++;

      if (PwmInVar.RisingEdgeCnt == 1u)
      {
        p = &PwmInVar.PwmBuf[PwmInVar.PwmBufIdx].LowPluse;
      }
      else if (PwmInVar.RisingEdgeCnt == 2u)
      {
        TimerResetFlag = 1u;

        p = &PwmInVar.PwmBuf[PwmInVar.PwmBufIdx].PeriodPlusLowPulse;

        PwmInVar.PwmBufIdx++;
        if (PwmInVar.PwmBufIdx >= (DRV_PWM_IN_PWM_PULSE_BUF_LEN))
        {
          PwmInVar.PwmBufIdx = 0u;
        }

        if (PwmInVar.PwmCnt < U8_MAX)
        {
          PwmInVar.PwmCnt++;
        }
      }
      else
      {
        /* Error Handler */
        TimerResetFlag = 1u;
        PwmInVar.PwmBufIdx = 0u;
        PwmInVar.PwmCnt = 0u;
      }

      if (p != (void *)0u)
      {
        p->CaptureCnt = CaptureCnt;
        p->OverFlowCnt = PwmInVar.TimerOverflowCnt;
        p->IsCaptureBeforeOverFlow = PwmInVar.IsCaptureBeforeOverFlow;
      }

      PwmInVar.IsCaptureTriggered = 1u;
    }

    if (TimerResetFlag != 0u)
    {
      Sdk_rstPwmInCapTmr();
      Sdk_setPwmInCapRisingEdge();

      PwmInVar.TimerOverflowCnt = 0u;
      PwmInVar.RisingEdgeCnt = 0u;
    }

    PwmInVar.IsCaptureBeforeOverFlow = 0u;
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:DrvPwmIn_capTmrOverFlowHandler
* Description:
*   PwmInAPI capture timer overflow handler.
*
* Inputs:
*
*
* Outputs:
* 1.PwmInVar.TimerOverflowCnt：捕获定时器溢出中断计数
* 2.PwmInVar.IsCaptureBeforeOverFlow：捕获中断和溢出中断嵌套标志。
*   0：捕获中断发生在溢出中断之后，1：捕获中断发生在溢出中断之前
*
* Limitations:
* 1.必须在捕获定时器溢出中断中执行
********************************************************************
END_FUNCTION_HDR*/
void DrvPwmIn_capTmrOverFlowHandler(void)
{
  u16 CapCnt = 0u;

  if (Sdk_isPwmInIsCapTmrRunning() != 0u)
  {
    if (PwmInVar.TimerOverflowCnt < U16_MAX)
    {
      PwmInVar.TimerOverflowCnt++;
    }

    if (Sdk_isPwmInCap() != 0u)
    {
      CapCnt = Sdk_getPwmInCapVal();
      PwmInVar.IsCaptureBeforeOverFlow = (CapCnt > (DRV_PWM_IN_TIMER_INT_NEST_MAX_CNT)) || (CapCnt == 0u);
    }
    else
    {
      PwmInVar.IsCaptureBeforeOverFlow = 0u;
    }
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:DrvPwmIn_step
* Description:
*   Caculate PwmInAPI frequency and duty.
*
* Inputs:
* 1.PwmInAPI.CaptureEn：0：禁止捕获输入信号，1：允许捕获输入信号
* 2.PwmInVar.IsCaptureTriggered:0：未捕获到Pwm输入信号变化，1：捕获到
*   Pwm输入信号边沿变化
* 3.PwmInVar.PwmCnt：捕获到的完整Pwm周期个数
* 4.PwmInVar.PwmBuf:Pwm输入信号捕获数据
* 5.PwmInVar.PwmBufIdx：Pwm输入信号捕获数据当前索引
*
* Outputs:
* 1.PwmInAPI.BusIdleTime：Pwm输入信号状态不变的持续时间，单位：ms
* 2.PwmInAPI.BusState：Pwm输入信号电平状态。0：低电平，1：高电平
* 3.PwmInAPI.Freq：Pwm输入信号频率，单位：0.1Hz
* 4.PwmInAPI.LowDuty：Pwm输入信号低电平占空比，单位：0.1%
* 5.PwmInAPI.HighDuty：Pwm输入信号高电平占空比，单位：0.1%
*
* Limitations:
* 1.执行周期必须为DRV_PWM_IN_PERIOD_TASK_TIME_BASE
* 2.
********************************************************************
END_FUNCTION_HDR*/
void DrvPwmIn_step(void)
{
  u8 tmp = 0u;
  Pwm_t *p = (void *)0;
  u8 i = 0u;
  u8 Idx = 0u;
  u8 PwmBufIdx = 0u;
  u32 PeriodPlusLowCnt = 0u;

  if (PwmInAPI.CaptureEn != 0u)
  {
    /* 短电短地后未能及时更新状态，修改为实时更新，22.04.11 by jxj */
    PwmInAPI.BusState = Sdk_getPwmInBusSts();

    if ((PwmInAPI.BusIdleTime < (DRV_PWM_IN_PWM_BUS_IDLE_TIME_MAX)) &&
        (PwmInVar.PwmCnt >= (DRV_PWM_IN_PWM_VALID_CHECK_NUM)))
    {
      PwmBufIdx = PwmInVar.PwmBufIdx;
      tmp = PwmInAPI.CaptureEn;
      PwmInAPI.CaptureEn = 0u;

      for (i = 0u; i < (DRV_PWM_IN_PWM_VALID_CHECK_NUM); i++)//DRV_PWM_IN_PWM_VALID_CHECK_NUM:1 --> i = 0
      {
        if (PwmBufIdx >= (i + 1u))   //PwmBufIdx >= 1 -->  Idx = PwmBufIdx - 1    PwmBufIdx变化范围：0  1  2
        {
          Idx = PwmBufIdx - i - 1u;
        }
        else                         //PwmBufIdx < 1 即 PwmBufIdx = 0 -->  Idx = PwmBufIdx + 3 - 1 --> Idx = PwmBufIdx + 2
        {
          Idx = PwmBufIdx + (DRV_PWM_IN_PWM_PULSE_BUF_LEN)-i - 1u;
        }                            //PwmBufIdx：0 1 2 --> Idx: 2 0 1

        if (Idx < DRV_PWM_IN_PWM_PULSE_BUF_LEN) //DRV_PWM_IN_PWM_PULSE_BUF_LEN:3
        {
          p = &PwmInVar.PwmBuf[Idx];

          p->LowCnt = DrvPwmIn_calPulseWidth(&(p->LowPluse));
          PeriodPlusLowCnt = DrvPwmIn_calPulseWidth(&(p->PeriodPlusLowPulse));
          p->PeriodCnt = PeriodPlusLowCnt - p->LowCnt;

          FreqBuf[i] = DrvPwmIn_calPulseFreq(p);
          DutyBuf[i] = DrvPwmIn_calPulseLowDuty(p);
        }
      }

      PwmInAPI.Freq = FreqBuf[0u];
      /*修改遗留的宏定义名称问题 22.04.06 by jxj*/
      if (((DutyBuf[0u] + DRV_PWM_IN_PWM_DUTY_ERROR) <= PwmInAPI.LowDuty) ||
          (DutyBuf[0u] >= (PwmInAPI.LowDuty + DRV_PWM_IN_PWM_DUTY_ERROR)))
      {
        PwmInAPI.LowDuty = DutyBuf[0u];
      }

      PwmInAPI.CaptureEn = tmp;
    }
    else
    {
      if (PwmInAPI.BusIdleTime > (DRV_PWM_IN_PWM_BUS_IDLE_TIME_MAX))
      {
        PwmInVar.PwmCnt = 0u;
        PwmInVar.PwmBufIdx = 0u;

        PwmInAPI.Freq = 0u;
        PwmInAPI.LowDuty = (PwmInAPI.BusState != 0u) ? 0u : 1000u;
      }
    }

    PwmInAPI.HighDuty = 1000u - PwmInAPI.LowDuty;

    if (PwmInVar.IsCaptureTriggered != 0u)  //与BusIdleTime有关
    {
      PwmInVar.IsCaptureTriggered = 0u;
      PwmInAPI.BusIdleTime = 0u;
    }
    else
    {
      if (PwmInAPI.BusIdleTime < (U16_MAX - (DRV_PWM_IN_PERIOD_TASK_TIME_BASE)))
      {
        PwmInAPI.BusIdleTime += (DRV_PWM_IN_PERIOD_TASK_TIME_BASE);
      }
    }
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:DrvPwmIn_calPulseWidth
* Description:
* 1.根据定时器溢出计数和捕获值计算脉冲宽度
* 2.发生中断嵌套时，如果捕获中断在溢出中断之前发生，则将溢出计数减一
*
* Inputs:
* 1.IsCaptureBeforeOverFlow:0:捕获发生在溢出后，1：捕获在溢出前
* 2.OverFlowCnt：溢出计数
* 3.CaptureCnt：捕获值
*
* Outputs:
* 1.脉冲宽度
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static u32 DrvPwmIn_calPulseWidth(Pulse_t *p)
{
  u32 OvCnt = p->OverFlowCnt;

  if (p->IsCaptureBeforeOverFlow != 0u)
  {
    if (OvCnt > 1u)
    {
      OvCnt -= 1u;
    }
    else
    {
      OvCnt = 0u;
    }
  }

  return (u32)(((u32)OvCnt << 16u) + (u32)p->CaptureCnt);
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:DrvPwmIn_calPulseWidth
* Description:
* 计算Pwm信号频率
*
* Inputs:
* 1.DRV_PWM_IN_CAPTURE_TIMER_FREQ：计时器频率
* 2.PeriodCnt：周期值
*
* Outputs:
* 1.Freq：频率，单位：0.1Hz
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static u16 DrvPwmIn_calPulseFreq(Pwm_t *p)
{
  u16 Freq = U16_MAX;

  if (p->PeriodCnt != 0u)
  {
    Freq = ((u32)(DRV_PWM_IN_CAPTURE_TIMER_FREQ)*10u / p->PeriodCnt);
  }
  return Freq;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:DrvPwmIn_calPulseLowDuty
* Description:
* 计算低电平占空比
*
* Inputs:
* 1.Low：低电平宽度
* 2.Period：周期宽度
*
* Outputs:
* 1.Duty：低电平占空比，单位：0.1%
*
* Limitations:
********************************************************************
END_FUNCTION_HDR*/
static u16 DrvPwmIn_calPulseLowDuty(Pwm_t *p)
{
  u32 Low = 0u;
  u32 Period = 0u;
  u32 Coef = 0u;
  u16 Duty = U16_MAX;

  if (p->LowCnt > (U32_MAX / 10u))
  {
    Coef = 1000u;
  }
  else if (p->LowCnt > (U32_MAX / 100u))
  {
    Coef = 100u;
  }
  else if (p->LowCnt > (U32_MAX / 1000u))
  {
    Coef = 10u;
  }
  else
  {
    Coef = 1u;
  }

  Low = p->LowCnt / Coef * 1000u;
  Period = p->PeriodCnt / Coef;

  if (Period != 0u)
  {
    Duty = (u16)(Low / Period);
  }
  return Duty;
}
