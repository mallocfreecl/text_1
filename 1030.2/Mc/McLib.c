/*BEGIN_FILE_HDR
********************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information
* contained in this doc should not be reproduced, or used, or disclosed
* without the written authorization from
* XiangBin Electric.
********************************************************************
* File Name:McLib.c
********************************************************************
* Project/Product:t3622
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
* Version       Date         	  Author       Descriptions
* ----------    --------------  ------------   ------------
* 1.0           2022-03-30      yhd            Original
*
*
********************************************************************
*END_FILE_HDR*/
#include "McLib.h"
#include "Common.h"
#include "DrvAdc.h"
#include "Mat.h"
#include "McCfg.h"

/* polyspace-begin MISRA-C3:17.3,17.7 */

/* Defines*/
/******************************************************************/
#define p60Up (&PwmOutBuf.Buf[0u].comp60up)
#define p60Down (&PwmOutBuf.Buf[0u].comp60down)
#define p61Up (&PwmOutBuf.Buf[0u].comp61up)
#define p61Down (&PwmOutBuf.Buf[0u].comp61down)
#define p62Up (&PwmOutBuf.Buf[0u].comp62up)
#define p62Down (&PwmOutBuf.Buf[0u].comp62down)

#define PhaseVoltReg (*pPhaseVoltageResultRegister)

/* Variables */
/******************************************************************/
PwmOutVarBuf_t PwmOutBuf;

vu16 PhaseVoltageRisingEdge = 0u;
vu16 PhaseVoltageFallingEdge = 0u;

 vu32 PhaseVoltageSum = 0u;
 vu16 PhaseVoltageCnt = 0u;
 vu16 DcVoltage = 0u;

 vu32 *pPhaseVoltageResultRegister;
/* polyspace +1 MISRA-C3:11.3 */
static vu32 *pShuntVoltage = (vu32 *)(&(ADC1->RES_OUT6.reg));

/*AdcResult[2] => 1st ADC Meas. **
**AdcResult[1] => 2nd ADC Meas. */
s16 AdcResult0 = 0;
s16 AdcResult1 = 0;

const u16 McLibVer = 4u;

static volatile PwmOutVar_t *p = &(PwmOutBuf.Buf[1u]);
static volatile PwmOutVar_t *pCmpVal = &(PwmOutBuf.Buf[1u]);
static volatile PwmOutVar_t *pCmpValBuf = &(PwmOutBuf.Buf[0u]);
static u16 *const Buf_pCmpUpVal[] = {p60Up, p61Up, p62Up};
static u16 *const Buf_pCmpDownVal[] = {p60Down, p61Down, p62Down};

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:T12_OM_Handler
* Description:
* 1.Pwm起始中断
* 2.设置T13控制死区的采样延时时间
* 3.读取上次采到的AD值
*
* 在OM中设置Pwm周期计数下降时的比较值
* 设置的比较值写入寄存器后，在比较值上升到周期值或下降到1时进行重载
* 因为要进行电流采样导致Pwm波形可能不对称，所以上升、下降的比较值都要更新
*
* ExeTime@24MHz:8.125us
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
void Mc_PwmStartHandler(void)
{
  static u8 flag = 0u;
  uint16 Time = 0u;

  /*读取死区采集到的相电压*/
  PhaseVoltageRisingEdge = PhaseVoltReg & 0x0FFFu;
  DcVoltage = (u16)((*pShuntVoltage) & 0x0FFFFuL);

  /* 设置计数上升时T13比较值，T13设为PwmChx边沿触发，用于触发
  死区采样，T13的比较值为采样延时时间 */
  CCU6_SetT13Compare(McCfg.FallingEdgeVoltSampleDelay);

  /* 设置Pwm计数下降时的比较值 */
  CCU6_LoadShadowRegister_CC60(p->comp60down);
  CCU6_LoadShadowRegister_CC61(p->comp61down);
  CCU6_LoadShadowRegister_CC62(p->comp62down);
  CCU6_EnableST_T12();

  /* 每个Pwm周期采集一次相电流电流，2个Pwm周期采集不同两相电流
   */
  if (flag == 0u)
  {
    AdcResult0 = ADC1->RES_OUT_EIM.bit.OUT_CH_EIM;

    flag = 1u;
    Time = p->IShuntTrig2;
  }
  else
  {
    AdcResult1 = ADC1->RES_OUT_EIM.bit.OUT_CH_EIM;

    flag = 0u;
    Time = p->IShuntTrig1;
  }

  /* 对电流采样时间进行补偿，设置定时器计时并重启定时器。
  Timer21设置为向上计数，发生溢出时触发ADC1的EIM采样，
  ADC1的EIM采样采集电流 */
  Time += CCU6->T12.reg;
  TIMER21->T2CON.bit.TR2 = 0u;
  TIMER21->T2H.reg = (u8)(Time >> 8u);
  TIMER21->T2L.reg = (u8)(Time & 0xFFu);
  TIMER21->T2CON.bit.TR2 = 1u;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:
* Description:
* Pwm周期中断，设置定时器比较值，读取死区采集的电压并求和
*
* ExeTime@24MHz:7.792us
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
void Mc_PwmPeriodHandler(void)
{
  PhaseVoltageFallingEdge = PhaseVoltReg & 0x0FFFu;
  DcVoltage = (u16)((*pShuntVoltage) & 0x0FFFFuL);

  CCU6_SetT13Compare(McCfg.RisingEdgeVoltSampleDelay);

  CCU6_LoadShadowRegister_CC60(p->comp60up);
  CCU6_LoadShadowRegister_CC61(p->comp61up);
  CCU6_LoadShadowRegister_CC62(p->comp62up);
  CCU6_EnableST_T12();

  PhaseVoltageSum += ((PhaseVoltageRisingEdge > DcVoltage) ? DcVoltage : PhaseVoltageRisingEdge);
  PhaseVoltageSum += ((PhaseVoltageFallingEdge > DcVoltage) ? DcVoltage : PhaseVoltageFallingEdge);
  PhaseVoltageCnt++;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Mc_exeSvm
* Description:
* 根据幅值、角度产生SVM波形
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
void Mc_exeSvm(McSvm_t *pSvm)
{
  s32 T1 = 0;
  s32 T2 = 0;
  u32 Sector = 0u;
  u32 Angle = 0u;
  u32 Index = 0u;

  if (pSvm->Amp == 0u)
  {
    pCmpValBuf->comp60up = McCfg.PwmMax >> 1u;
    pCmpValBuf->comp60down = pCmpValBuf->comp60up;
    pCmpValBuf->comp61up = pCmpValBuf->comp60up;
    pCmpValBuf->comp61down = pCmpValBuf->comp60up;
    pCmpValBuf->comp62up = pCmpValBuf->comp60up;
    pCmpValBuf->comp62down = pCmpValBuf->comp60up;
  }
  else
  {
    /* Calculate sector number 0..5 and table index 0..255 */
    Angle = ((u32)pSvm->Angle) * 6u;
    Sector = (Angle >> 16u) & 7u;
    pSvm->Sector = (u16)Sector;
    Index = (Angle >> 8u) & 0xFFu;
    /* Calculate and limit times */
    T1 = (((u32)pSvm->Amp) * Table_Sin60[255u - Index]) >> (MAT_FIX_SHIFT + 1u);
    pSvm->T1 = (s16)T1;
    /* RandVector1 = Amp * sin(gamma) */
    T2 = (((u32)pSvm->Amp) * Table_Sin60[Index]) >> (MAT_FIX_SHIFT + 1u);
    pSvm->T2 = (s16)T2;

    switch (Sector)
    {
    case 0u:
      Mc_T2C(T1, T2, 0x012u);
      break;
    case 1u:
      Mc_T2C(T2, T1, 0x102u);
      break;
    case 2u:
      Mc_T2C(T1, T2, 0x120u);
      break;
    case 3u:
      Mc_T2C(T2, T1, 0x210u);
      break;
    case 4u:
      Mc_T2C(T1, T2, 0x201u);
      break;
    default: /* case 5u: */
      Mc_T2C(T2, T1, 0x021u);
      break;
    }
  }

  /* polyspace +1 MISRA-C3:17.3 */
  s32 int_was_mask = CMSIS_Irq_Dis();
  *pCmpVal = *pCmpValBuf;
  if (int_was_mask == 0)
  {
    /* polyspace +1 MISRA-C3:17.3 */
    CMSIS_Irq_En();
  }
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Mc_T2C
* Description:
* 根据T1、T2、相序计算定时器比较值
* 根据电流采样需要对比较值进行偏移
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
void Mc_T2C(u16 T1, u16 T2, u16 Ch_Squence)
{
  static s32 ci = 0;
  static u16 *t0Up, *t0Down, *t1Up, *t1Down, *t2Up, *t2Down;
  u16 Offset0 = 0;
  u16 OffSet2 = 0;
  u8 ShiftFlag = 0u;
  u16 PulseMinCnt = 0u;

  t0Up = Buf_pCmpUpVal[(Ch_Squence >> 8u) & 0x3u];
  t0Down = Buf_pCmpDownVal[(Ch_Squence >> 8u) & 0x3u];
  t1Up = Buf_pCmpUpVal[(Ch_Squence >> 4u) & 0x3u];
  t1Down = Buf_pCmpDownVal[(Ch_Squence >> 4u) & 0x3u];
  t2Up = Buf_pCmpUpVal[(Ch_Squence >> 0u) & 0x3u];
  t2Down = Buf_pCmpDownVal[(Ch_Squence >> 0u) & 0x3u];

  /*
   t0=T/2-T1-T2
   t1=T/2+T1-T2
   t2=T/2+T1+T2
   0->t0->T1->T1->t1->T2->T2->T/2
    */
  /* polyspace +1 MISRA-C3:10.1 */
  ci = ((s32)McCfg.PwmMax >> 1u) - T1 - T2;
  if (ci < 0)
  {
    ci = 0;
  }
  *t0Up = ci;

  /* polyspace +1 MISRA-C3:10.1 */
  ci = ((s32)McCfg.PwmMax >> 1u) + T1 - T2;
  if (ci < 0)
  {
    ci = 0;
  }
  if (ci > McCfg.PwmMax)
  {
    ci = McCfg.PwmMax + 1;
  }
  *t1Up = ci;

  /* polyspace +1 MISRA-C3:10.1 */
  ci = ((s32)McCfg.PwmMax >> 1u) + T1 + T2;
  if (ci > McCfg.PwmMax)
  {
    ci = McCfg.PwmMax + 1;
  }
  *t2Up = ci;

  *t0Down = *t0Up;
  *t1Down = *t1Up;
  *t2Down = *t2Up;

  /* t1-t0 = 2T1,t2-t1=2T2 */
  pCmpValBuf->T1 = T1;
  pCmpValBuf->T2 = T2;

  PulseMinCnt = McCfg.CurrentSettleTimeCnt + McCfg.CurrentSampleTimeCnt;

  /* Smallest DutyCycle moved left */
  if ((T2 << 1u) < PulseMinCnt)
  {
    OffSet2 = PulseMinCnt - (T2 << 1u);

    ci = (*t2Up) - OffSet2;
    if (ci < 0)
    {
      ci = 0;
    }
    *t2Up = ci;

    /* Readjust for deadband voltage sample */
    if (((*t2Up) < (*t1Up)) && ((*t2Up) > (*t1Up)))
    {
      OffSet2 += ((*t2Up) - (*t1Up));
      (*t2Up) = (*t1Up);
    }

    ci = (*t2Down) + OffSet2;
    if (ci > McCfg.PwmMax)
    {
      ci = McCfg.PwmMax + 1;
    }
    *t2Down = ci;

    ShiftFlag |= 0x02u;
  }
  else
  {
  }

  /* Largest DutyCycle moved right */
  if ((T1 << 1u) < PulseMinCnt)
  {
    Offset0 = PulseMinCnt - (T1 << 1u);

    ci = (*t0Up) + Offset0;
    if (ci > McCfg.PwmMax)
    {
      ci = McCfg.PwmMax + 1;
    }
    *t0Up = ci;

    /* Readjust for deadband voltage sample */
    if (((*t0Up) < (*t1Up)) && ((*t0Up) > (*t1Up)))
    {
      Offset0 += ((*t1Up) - (*t0Up));
      (*t0Up) = (*t1Up);
    }

    ci = (*t0Down) - Offset0;
    if (ci < 0)
    {
      ci = 0;
    }
    *t0Down = ci;

    ShiftFlag |= 0x01u;
  }
  else
  {
  }

  if ((ShiftFlag & 0x1u) != 0u)
  {
    pCmpValBuf->IShuntTrig1 = U16_MAX - ((McCfg.PwmMax << 1u) - ((*t0Down) + McCfg.CurrentSampleTimeCnt));
  }
  else
  {
    pCmpValBuf->IShuntTrig1 = U16_MAX - ((McCfg.PwmMax << 1u) - (((*t0Down) + (*t1Down)) >> 1u));
  }

  if ((ShiftFlag & 0x2u) != 0u)
  {
    pCmpValBuf->IShuntTrig2 = U16_MAX - ((McCfg.PwmMax << 1u) - ((*t1Down) + McCfg.CurrentSampleTimeCnt));
  }
  else
  {
    pCmpValBuf->IShuntTrig2 = U16_MAX - ((McCfg.PwmMax << 1u) - (((*t1Down) + (*t2Down)) >> 1u));
  }
}


/* Defines*/
/******************************************************************/
#define MC_WM_ANGLE_0_TH (20000u)
#define MC_WM_ANGLE_360_TH (40000u)

#define MC_WM_VALID_CYCLE_MIN (3u)

/* 根据实际测试结果修改认为相电压达到稳定状态的电压阈值，
   实际电压大于6.4V后相电压达到稳定值，22.03.30 by yhd */
#define MC_BAT_STABLE_LOW_TH (63u)
#define MC_BAT_STABLE_HIGH_TH (68u)

#define MC_WM_PHASE_VPP_MIN (30)

typedef struct
{
  TPhaseCurr BEMF;
  TComplex StatV;

  u16 Angle;
  u16 AngleFilt;
  u16 AngleWithOffset;
  u8 AngleZeroFlag;

  u8 Cycles;
  u16 DirCnt;
  u16 ElecPeriodCnt;

  s16 U;
  s16 V;
  s16 W;
  s16 UV;
  s16 VW;
  s16 WU;
  u16 N;
  u16 EMFAmp;

  s16 UOffset;
  s16 VOffset;
  s16 WOffset;

  s16 UMax;
  s16 UMin;
  s16 VMax;
  s16 VMin;
  s16 WMax;
  s16 WMin;

  u8 IsBatStable;
} WmVar_t;

/* Variables */
/******************************************************************/
static WmVar_t WmVar;
WmAPI_t WmAPI;

/* 反电势幅值滤波，23.05.13 by yhd */
s32 BemFAmpLpA = 610;
u16 BemfAmpFlt = 0;

// static void PhaseOffsetCalibration(u8 IsWmCycleChg);

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:McWm_init
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
*
*
********************************************************************
END_FUNCTION_HDR*/
void McWm_init(void)
{
  WmVar.DirCnt = 0u;
  WmVar.Cycles = 0u;
  WmVar.AngleZeroFlag = 0u;

  WmVar.IsBatStable = 0u;

  WmAPI.Valid = 0u;
  WmAPI.Angle = McSvm.Angle;
  WmAPI.BEMFMax = 0;
  WmAPI.TimeOutCnt = 0u;

  WmAPI.Upp = 0;
  WmAPI.Vpp = 0;
  WmAPI.Wpp = 0;

  WmVar.UMin = S16_MAX;
  WmVar.UMax = S16_MIN;
  WmVar.VMin = S16_MAX;
  WmVar.VMax = S16_MIN;
  WmVar.WMin = S16_MAX;
  WmVar.WMax = S16_MIN;
}
/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:McWm_step
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
*
*
********************************************************************
END_FUNCTION_HDR*/
void McWm_step(u16 VoltU, u16 VoltV, u16 VoltW)
{
  s32 LowPassFilter_a = 0;

  if (AdcResults.Voltage.Bat < MC_BAT_STABLE_LOW_TH)
  {
    WmVar.IsBatStable = 0;
  }
  else if (AdcResults.Voltage.Bat > MC_BAT_STABLE_HIGH_TH)
  {
    WmVar.IsBatStable = 1;
  }
  else
  {
  }

  if (WmVar.IsBatStable == 0u)
  {
    WmAPI.Valid = 0u;
    WmVar.Cycles = 0u;
  }
  else
  {
    VoltU += WmVar.UOffset;
    VoltV += WmVar.VOffset;
    VoltW += WmVar.WOffset;
    WmVar.UV = (s16)((s32)VoltU - (s32)VoltV);
    WmVar.VW = (s16)((s32)VoltV - (s32)VoltW);
    WmVar.WU = (s16)((s32)VoltW - (s32)VoltU);
    WmVar.N = (WmVar.UV + WmVar.VW + WmVar.WU) / 3u;
    WmVar.U = (s16)((s32)WmVar.UV - (s32)WmVar.N);
    WmVar.V = (s16)((s32)WmVar.VW - (s32)WmVar.N);
    WmVar.W = (s16)((s32)WmVar.WU - (s32)WmVar.N);

    if (WmVar.U < WmVar.UMin)
    {
      WmVar.UMin = WmVar.U;
    }
    if (WmVar.U > WmVar.UMax)
    {
      WmVar.UMax = WmVar.U;
    }

    if (WmVar.V < WmVar.VMin)
    {
      WmVar.VMin = WmVar.V;
    }
    if (WmVar.V > WmVar.VMax)
    {
      WmVar.VMax = WmVar.V;
    }

    if (WmVar.W < WmVar.WMin)
    {
      WmVar.WMin = WmVar.W;
    }
    if (WmVar.W > WmVar.WMax)
    {
      WmVar.WMax = WmVar.W;
    }

    if (WmVar.U > WmAPI.BEMFMax)
    {
      WmAPI.BEMFMax = WmVar.U;
    }
    if (WmVar.V > WmAPI.BEMFMax)
    {
      WmAPI.BEMFMax = WmVar.V;
    }
    if (WmVar.W > WmAPI.BEMFMax)
    {
      WmAPI.BEMFMax = WmVar.W;
    }

    WmVar.BEMF.A = WmVar.U;
    WmVar.BEMF.B = WmVar.V;

    if ((WmVar.U != 0) || (WmVar.V != 0) || (WmVar.W != 0))
    {
      WmVar.StatV = Mat_Clarke(WmVar.BEMF);
      WmVar.Angle = Mat_CalcAngleAmp(WmVar.StatV, &WmVar.EMFAmp);

      if (WmVar.EMFAmp > McCfg.WmLowSpeedBEMFThreshold)
      {
        /* fc=333Hz */
        LowPassFilter_a = McCfg.WmAngleHighSpeedLpA;
      }
      else
      {
        /* fc=100Hz */
        LowPassFilter_a = McCfg.WmAngleLowSpeedLpA;
      }

      /*
      Low-pass filter:
      y(k)=a*x(k)+(1-a)*y(k-1)
      a=(2*pi*Ts*fc)/(2*pi*Ts*fc+1)

      fc is the cutoff frequency of the IIR filter.
      a is the filter coefficient in the range (0, 1].
      y(k) is the filtered output value at time k.
      y(k-1) is the filtered output value at time k−1.
      x(k) is the sampled input value at time k.
      Ts is the sample time of the IIR Filter block.
       */
      /* polyspace +2 MISRA-C3:10.1 */
      WmVar.AngleFilt =
        ((s32)WmVar.Angle * LowPassFilter_a + ((s32)32768 - LowPassFilter_a) * (s32)WmVar.AngleFilt) >> 15;
      WmVar.AngleWithOffset = WmVar.Angle + WmAPI.AngleOffset;
      // WmAPI.Angle =
      //   ((s32)WmVar.AngleWithOffset * LowPassFilter_a + ((s32)32768 - LowPassFilter_a) * (s32)WmAPI.Angle) >> 15;
      WmAPI.Angle = WmVar.AngleWithOffset;
    }
    else
    {
      WmVar.Cycles = 0u;

      WmVar.EMFAmp = 0;
    }

    /* 反电势幅值滤波，23.05.13 by yhd */
    if (BemfAmpFlt != 0)
    {
      BemfAmpFlt = ((s32)WmVar.EMFAmp * BemFAmpLpA + ((s32)32768 - (s32)BemFAmpLpA) * (s32)BemfAmpFlt) >> 15;
    }
    else
    {
      BemfAmpFlt = WmVar.EMFAmp;
    }

    /* 反电势幅值小时直接认为是静止状态，23.05.13 by yhd */
    if (BemfAmpFlt < McCfg.WmAmpThreshold)
    {
      McWm_init();

      WmAPI.TimeOutCnt = U16_MAX;

      WmAPI.ElecPeriodCnt = U16_MAX;
      WmAPI.Speed = 0;
    }
    else
    {
      if (WmVar.AngleFilt < MC_WM_ANGLE_0_TH)
      {
        if (WmVar.AngleZeroFlag == 0u)
        {
          WmVar.AngleZeroFlag = 1u;
          WmVar.DirCnt = 0u;
        }
      }

      if (WmVar.AngleFilt < MC_WM_ANGLE_360_TH)
      {
        if (WmVar.DirCnt < U16_MAX)
        {
          WmVar.DirCnt++;
        }
      }

      if ((WmVar.AngleZeroFlag != 0u) && (WmVar.AngleFilt > MC_WM_ANGLE_360_TH))
      {
        WmVar.AngleZeroFlag = 0u;

        if (WmVar.Cycles < MC_WM_VALID_CYCLE_MIN)
        {
          WmVar.Cycles++;
        }
        else
        {
          WmVar.Cycles = MC_WM_VALID_CYCLE_MIN;
        }

        if (WmVar.Cycles >= MC_WM_VALID_CYCLE_MIN)
        {
          if (WmVar.DirCnt < (WmVar.ElecPeriodCnt >> 1u))
          {
            WmAPI.Dir = -1;
          }
          else
          {
            WmAPI.Dir = 1;
          }

          WmAPI.Upp = WmVar.UMax - WmVar.UMin;
          WmAPI.Vpp = WmVar.VMax - WmVar.VMin;
          WmAPI.Wpp = WmVar.WMax - WmVar.WMin;
          WmVar.UMin = S16_MAX;
          WmVar.UMax = S16_MIN;
          WmVar.VMin = S16_MAX;
          WmVar.VMax = S16_MIN;
          WmVar.WMin = S16_MAX;
          WmVar.WMax = S16_MIN;

          WmAPI.ElecPeriodCnt = WmVar.ElecPeriodCnt;
          WmAPI.Speed = (s16)(WmAPI.ElecPeriodToSpeedCoef / (s32)WmAPI.ElecPeriodCnt * (s32)WmAPI.Dir);

          if (WmAPI.Speed != 0)
          {
            WmAPI.Ke = (s32)WmAPI.BEMFMax * MC_CFG_BEMF_FACTOR_SCALE / (ABS(WmAPI.Speed));
          }
          WmAPI.BEMFMax = 0;
        }

        WmVar.DirCnt = 0u;
        WmVar.ElecPeriodCnt = 0u;

        WmAPI.TimeOutCnt = 0u;
      }
      else
      {
        if (WmVar.ElecPeriodCnt < WmAPI.ElecPeriodCntMax)
        {
          WmVar.ElecPeriodCnt++;
        }
        else
        {
          WmAPI.ElecPeriodCnt = U16_MAX;

          WmVar.Cycles = 0u;
        }
      }
    }

    if (WmAPI.TimeOutCnt < U16_MAX)
    {
      WmAPI.TimeOutCnt++;
    }

    if (WmVar.Cycles >= MC_WM_VALID_CYCLE_MIN)
    {
      WmAPI.Valid = 1u;
    }
    else
    {
      WmAPI.Valid = 0u;
    }

    if (WmAPI.Valid != 0u)
    {
      WmAPI.Speed = (s16)(WmAPI.ElecPeriodToSpeedCoef / (s32)WmAPI.ElecPeriodCnt * (s32)WmAPI.Dir);
    }
    else
    {
      if (WmAPI.TimeOutCnt > WmAPI.TimeOutCntMax)
      {
        WmAPI.Speed = 0;
      }
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
u8 McWm_getSampleErrorFlag(void)
{
  return 0u;
}

/* polyspace-begin MISRA-C3:17.3,17.7 */
