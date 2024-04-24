/*BEGIN_FILE_HDR
**************************************************************************
* NOTICE
* This software is the property of XiangBin Electric. Any information contained in this
* doc should not be reproduced, or used, or disclosed without the written authorization from
* XiangBin Electric.
**************************************************************************
* File Name:Emo_RAM.c
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
#include "Emo_RAM.h"
#include "SDK.h"
#include "TestMode.h"
#include "ADCSample.h"

#include "gpt12e.h"
#include "ccu6.h"
#include "timer2x.h"
#include "timer2x_defines.h"

/* Defines*/
/******************************************************************/
#define TIMER21_DELAY_CNT (0u)

#define p60Up (&CompareVal.EdgeSelect[0].comp60up)
#define p60Down (&CompareVal.EdgeSelect[0].comp60down)
#define p61Up (&CompareVal.EdgeSelect[0].comp61up)
#define p61Down (&CompareVal.EdgeSelect[0].comp61down)
#define p62Up (&CompareVal.EdgeSelect[0].comp62up)
#define p62Down (&CompareVal.EdgeSelect[0].comp62down)



#define DEBUG_CODE (0u) 

#if DEBUG_CODE   
struct {
	uint16 StartAngle;
	uint16 FluxAngle;
	uint16 SvmAngle;
	
	sint16 Va;
	sint16 Vb;
	sint16 Vab;
	
	sint16 Valpha;
	sint16 Vbeta;
	
	sint16 Ia;
	sint16 Ib;
	sint16 Iab;
	
	sint16 Id;
	sint16 Iq;
	
	sint16 UmiRI;
	sint16 IntegralUmiRI;
	sint16 FaiAlpha;
	sint16 FaiAmp;
	sint16 FaiAmpLp;
	
	sint16 FaiAlphaAngle0;
}EmoWatch;	/* Disable debug code - 2021-07-29-by zyg  */
#endif

/* Variables */
/******************************************************************/

TEmo_Ctrl Emo_Ctrl;
TEmo_Foc Emo_Foc;
TEmo_Svm Emo_Svm;

CompareVal_t CompareVal;
static volatile CCU6Compares_t *p = &(CompareVal.EdgeSelect[1]);
static volatile CCU6Compares_t *pCmpVal = &(CompareVal.EdgeSelect[1]);
static volatile CCU6Compares_t *pCmpValBuf = &(CompareVal.EdgeSelect[0]);
static uint16 * const Buf_pCmpUpVal[] = {p60Up, p61Up, p62Up};
static uint16 * const Buf_pCmpDownVal[] = {p60Down, p61Down, p62Down};

uint32 Emo_AdcResult[5u];
sint16 AdcResult0;
sint16 AdcResult1;
sint16 R0mioffs;
sint16 OffsmiR1;
sint16 R1miR0;

static sint32 ci;
static sint32 ci1;
static uint16 *t0Up, *t0Down, *t1Up, *t1Down, *t2Up, *t2Down;

TComplex Flux;
uint16 Tempu;
uint16 AngleOffset = 0;

sint16 deltaphi;
sint16 domega;

/* Function declaration */
/******************************************************************/
__STATIC_INLINE void Emo_RAM_lEstFlux(void);
__STATIC_INLINE void Emo_RAM_FluxAnglePll(void);
__STATIC_INLINE void Emo_RAM_lExeSvm(TEmo_Svm *pSvm);

#if (EMO_DECOUPLING==1)
    __STATIC_INLINE TComplex Emo_CurrentDecoupling(void);
#endif

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Emo_RAM_CurrAdc1
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
void Emo_RAM_CurrAdc1(void)
{
	uint16 sector;

  /*AdcResult[2] => 1st ADC Meas. **
  **AdcResult[1] => 2nd ADC Meas. */
  AdcResult0 = Emo_AdcResult[2u] & 0x0FFFu;
  AdcResult1 = Emo_AdcResult[1u] & 0x0FFFu;
  Emo_AdcResult[3u]=AdcResult0+AdcResult1;
  /*result 0 minus CSA Offset*/
  R0mioffs = AdcResult0 - Emo_Svm.CsaOffsetAdw;
  /*CSA Offset minus result 1*/
  OffsmiR1 =Emo_Svm.CsaOffsetAdw - AdcResult1;
  /*Result 1 - Result 0*/
  R1miR0 = AdcResult1 - AdcResult0;
	
	/* Caculate shunt currrent */
	Emo_Ctrl.IShuntBufIdx = (++Emo_Ctrl.IShuntBufIdx) & 0x1u;
	IShunt_t *pIShunt = &Emo_Ctrl.IShuntBuf[Emo_Ctrl.IShuntBufIdx];
	pIShunt->I1 = (R0mioffs>=0) ? R0mioffs : (-R0mioffs);
	pIShunt->I2 = (OffsmiR1>=0) ? OffsmiR1 : (-OffsmiR1);
	pIShunt->T1 = p->T1;
	pIShunt->T2 = p->T2;
	
  /* Calculate currents according to sector number */
  sector = Emo_Svm.StoredSector1;

  switch (sector)
  {
    case 0u:
    {
      Emo_Svm.PhaseCurr.A = R0mioffs;
      Emo_Svm.PhaseCurr.B = R1miR0;
    }
    break;

    case 1u:
    {
      Emo_Svm.PhaseCurr.A = R1miR0;
      Emo_Svm.PhaseCurr.B = R0mioffs;
    }
    break;

    case 2u:
    {
      Emo_Svm.PhaseCurr.A = OffsmiR1;
      Emo_Svm.PhaseCurr.B = R0mioffs;
    }
    break;

    case 3u:
    {
      Emo_Svm.PhaseCurr.A = OffsmiR1;
      Emo_Svm.PhaseCurr.B = R1miR0;
    }
    break;

    case 4u:
    {
      Emo_Svm.PhaseCurr.A = R1miR0;
      Emo_Svm.PhaseCurr.B = OffsmiR1;
    }
    break;

    case 5u:
    {
      Emo_Svm.PhaseCurr.A = R0mioffs;
      Emo_Svm.PhaseCurr.B = OffsmiR1;
    }
    break;
    default:
    {
      /* unexpected error => halt motor, reset device */
      Emo_StopMotor();
    }
    break;
  }
	
	#if DEBUG_CODE /* Disable debug code - 2021-07-29-by zyg  */
	EmoWatch.Ia = -Emo_Svm.PhaseCurr.A;
	EmoWatch.Ib = -Emo_Svm.PhaseCurr.B;
	EmoWatch.Iab = EmoWatch.Ia - EmoWatch.Ib;
	#endif 
	
  /*prepare sector number for next period*/
  Emo_Svm.StoredSector1 = Emo_Svm.Sector;	
} /* End of Emo_HandleAdc1 */

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:T12_OM_Handler
* Description:
*  ESM is triggered by CCU6 COUT63 while T12 countting up
*  EIM is triggered by Timer21 while T12 countting down
*  IsrTime: 7.1us
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
  /* TimeMeasureStart(); */

	if(FctTestMOSEn)
	{
		ADCResults.RawAD.Current.DC = ADC1->RES_OUT1.reg & 0x0FFFu;
	}
	else
	{
		/*set PWM compare values for T12 down-counting part*/
		CCU6_LoadShadowRegister_CC60(p->comp60down);
		CCU6_LoadShadowRegister_CC61(p->comp61down);
		CCU6_LoadShadowRegister_CC62(p->comp62down);
		CCU6_EnableST_T12();

    Emo_AdcResult[1u] = ADC1->RES_OUT_EIM.reg;  /* I2 */
		Emo_AdcResult[2u] = ADC1->RES_OUT1.reg;     /* I1 */

		TIMER21->T2CON.bit.TR2 = 0u;
		uint16 Time = 0xFFFFu - (p->CompT13ValueDown - CCU6->T12.reg + (TIMER21_DELAY_CNT)) + (TIMER21_DELAY_CNT);
		TIMER21->T2H.reg = (uint8) (Time>>8u);
		TIMER21->T2L.reg = (uint8) (Time&0xFFu);
		TIMER21->T2CON.bit.TR2 = 1u;
	}

  /* TimeMeasureEnd(0); */
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:
* Description:
*   IsrTime: 5.9us
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
	/* TimeMeasureStart(); */
	
	if(FctTestMOSEn)
  {
    TestMode_TestMOS();
  }
	else
	{
		CCU6_SetT13Compare(p->CompT13ValueUp);

		CCU6_LoadShadowRegister_CC60(p->comp60up);
		CCU6_LoadShadowRegister_CC61(p->comp61up);
		CCU6_LoadShadowRegister_CC62(p->comp62up);
		CCU6_EnableST_T12();
	}
	
	/* TimeMeasureEnd(0); */
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Emo_RAM_MHighTask
* Description:
*   IsrTime: 140.1us
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
void Emo_RAM_MHighTask(void)
{
  uint16 angle = 0u;
  uint16 ampl;
  TComplex Vect1 = {0, 0};
  TComplex Vect2;

	Emo_Foc.Angle2 = Emo_Foc.Angle1;
	Emo_Foc.Angle1 = Emo_Foc.Angle;
	
  Emo_RAM_CurrAdc1();
  /* Perform Clarke transformation to stationary 2-phase system */
  /* PhaseCurr*4 -> StatCurr*/
  Emo_Foc.StatCurr = Mat_Clarke(Emo_Svm.PhaseCurr);
  /* Perform Park transformation to rotating 2-phase system */
  Emo_Foc.RotCurr = Mat_Park(Emo_Foc.StatCurr, Emo_Foc.Angle);
	
	
  #if DEBUG_CODE /* Disable debug code - 2021-07-29-by zyg  */
	EmoWatch.Id = Emo_Foc.RotCurr.Real;
	EmoWatch.Iq = Emo_Foc.RotCurr.Imag;
	#endif
	
  /* Estimate flux and calculate rotor angle */
  Emo_RAM_lEstFlux();

	/* Caculte angle speed */
  Emo_Ctrl.PtrAngle = (Emo_Ctrl.PtrAngle + 1u) & 0x1fu;
  if(Emo_Ctrl.Anglersptr == 32u)
  {
    angle = Emo_Ctrl.AngleBuffer[Emo_Ctrl.PtrAngle];
    Emo_Ctrl.AngleBuffer[Emo_Ctrl.PtrAngle] = Emo_Foc.FluxAngle;
    Emo_Ctrl.Speedest = Emo_Foc.FluxAngle - angle;
  }
  else
  {
    Emo_Ctrl.AngleBuffer[Emo_Ctrl.PtrAngle] = Emo_Foc.FluxAngle;
    Emo_Ctrl.Speedest = Emo_Foc.FluxAngle - Emo_Ctrl.AngleBuffer[(Emo_Ctrl.PtrAngle - Emo_Ctrl.Anglersptr) & 0x1fu];
  }
  Emo_Ctrl.Speedpll = Emo_Ctrl.Speedest >> Emo_Ctrl.Exppllhigh;

  Emo_RAM_FluxAnglePll();

  if(Emo_Status.MotorState == (EMO_MOTOR_STATE_RUN))
  {
    Emo_Foc.Angle = Emo_Ctrl.FluxAnglePll;
		
#if (EMO_DECOUPLING==0)
		/* Current Regulator: Execute PI algorithm for rotating voltage */
		/* id */
		Emo_Foc.RotVolt.Real = Mat_ExePi(&Emo_Ctrl.RealCurrPi, Emo_Ctrl.DemagCurr - Emo_Foc.RotCurr.Real);

		/* iq */
		Emo_Foc.RotVolt.Imag = Mat_ExePi(&Emo_Ctrl.ImagCurrPi, Emo_Ctrl.RefCurr - Emo_Foc.RotCurr.Imag);
#else
		Emo_Foc.RotVoltCurrentcontrol.Real = Mat_ExePi(&Emo_Ctrl.RealCurrPi, Emo_Ctrl.DemagCurr - Emo_Foc.RotCurrFiltered.Real);
		Emo_Foc.RotVoltCurrentcontrol.Imag = Mat_ExePi(&Emo_Ctrl.ImagCurrPi, Emo_Ctrl.RefCurr - Emo_Foc.RotCurrFiltered.Imag);
		/* Calculate Decoupling */
		Emo_Foc.RotVolt = Emo_CurrentDecoupling();
#endif
  }
  else
  {
    /* Open loop: */
    /* Increment angle */
    Emo_Foc.StartAngle += Emo_Foc.StartFrequencySlope;
    Emo_Foc.Angle = Emo_Foc.StartAngle;
		
		/* Current Regulator: Execute PI algorithm for rotating voltage */
		/* id */
		Emo_Foc.RotVolt.Real = Mat_ExePi(&Emo_Ctrl.RealCurrPi, Emo_Ctrl.RefCurr - Emo_Foc.RotCurr.Real);

		/* iq */
		Emo_Foc.RotVolt.Imag = Mat_ExePi(&Emo_Ctrl.ImagCurrPi, 0 - Emo_Foc.RotCurr.Imag);
  }
	

  /* DC-link voltage correction */
  Vect1.Real = __ssat(Mat_FixMulScale(Emo_Foc.RotVolt.Real, Emo_Foc.Dcfactor1, 1), MAT_FIX_SAT);
  Vect1.Imag = __ssat(Mat_FixMulScale(Emo_Foc.RotVolt.Imag, Emo_Foc.Dcfactor1, 1), MAT_FIX_SAT);
  /* Limitation Algorithm */
  Vect2 = Limitsvektor(&Vect1, &Emo_Svm);
  /* Cartesian to Polar Transformation **
  ** outputs; angle, ampl              */
  angle = Mat_CalcAngleAmp(Vect2, &ampl);

  /* limitation of max. amplitude */
  if (ampl > Emo_Svm.MaxAmp)
  {
    ampl = Emo_Svm.MaxAmp;
  }
  else
  {

  }
		
  if (Emo_Svm.CounterOffsetAdw > 127u)
  {
    Emo_Svm.Amp = ampl;
  }
  else
  {
    Emo_Svm.Amp    = 0u;
    Emo_Svm.CsaOffsetAdwSumme = Emo_Svm.CsaOffsetAdwSumme + Emo_AdcResult[3u];
    Emo_Svm.CounterOffsetAdw++;
    if (Emo_Svm.CounterOffsetAdw == 128u)
    {
	  /*启动前使用之前的零点数据 20230517 by zjj*/
      Emo_Svm.CsaOffsetAdw = Emo_Svm.CsaOffset;
    }
    else
    {

    }
  }

  /* sum of regulated angle out of Current Regulation + PLL-Angle */
  Emo_Svm.Angle = angle + Emo_Foc.Angle;
	
	
#if DEBUG_CODE /* Disable debug code - 2021-07-29-by zyg  */
	EmoWatch.StartAngle = Emo_Foc.Angle;
	EmoWatch.FluxAngle = Emo_Foc.FluxAngle;
	EmoWatch.SvmAngle = Emo_Svm.Angle;
#endif
	
  /* Perform Polar-2-Cartesian transformation      **
  ** preparation of input values for FluxEstimator */
  Emo_Foc.StatVoltAmpM = __ssat(Mat_FixMulScale(Emo_Svm.Amp, Emo_Foc.Dcfactor2, 3u), MAT_FIX_SAT);
  Emo_Foc.StatVolt = Mat_PolarKartesisch(Emo_Foc.StatVoltAmpM, Emo_Svm.Angle);

  /* Perform space vector modulation */
  Emo_RAM_lExeSvm(&Emo_Svm);
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Emo_RAM_T2C
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
__STATIC_INLINE void Emo_RAM_T2C(uint16 T1, uint16 T2, uint16 Ch_Squence)
{  
  t0Up = Buf_pCmpUpVal[(Ch_Squence >> 8u) & 0x3u];
  t0Down = Buf_pCmpDownVal[(Ch_Squence >> 8u) & 0x3u];
  t1Up = Buf_pCmpUpVal[(Ch_Squence >> 4u) & 0x3u];
  t1Down = Buf_pCmpDownVal[(Ch_Squence >> 4u) & 0x3u];
  t2Up = Buf_pCmpUpVal[(Ch_Squence >> 0u) & 0x3u];
  t2Down = Buf_pCmpDownVal[(Ch_Squence >> 0u) & 0x3u];

  ci = ((sint32)(CCU6_T12PR) / 2u) - T1 - T2;
  if (ci < ((EMO_SVM_DEADTIME))) {ci = 0;};
  *t0Up = ci;

  ci = ((sint32)(CCU6_T12PR) / 2u) + T1 - T2;
  if (ci < ((EMO_SVM_DEADTIME))) {ci = 0;};
  if (ci > ((CCU6_T12PR) - ((EMO_SVM_DEADTIME)))) {ci = (CCU6_T12PR) + 1;};
  *t1Up = ci;

  ci = ((CCU6_T12PR) / 2u) + T1 + T2;
  if (ci > ((CCU6_T12PR) - ((EMO_SVM_DEADTIME)))) {ci = (CCU6_T12PR) + 1;};
  *t2Up = ci;

  *t0Down = *t0Up;
  *t1Down = *t1Up;
  *t2Down = *t2Up;

	ci = *t1Up - *t0Up;
  if(ci < (EMO_SVM_MINTIME))
  {
    ci = (EMO_SVM_MINTIME) - ci;
    ci1 =  *t0Up + ci;
    if (ci1 > ((CCU6_T12PR) - (EMO_SVM_DEADTIME))) {*t0Up = (CCU6_T12PR) + 1;}else{*t0Up = ci1;};
    ci1 = *t0Down - ci;
    if (ci1 < (EMO_SVM_DEADTIME)) {*t0Down = 0;}else{*t0Down = ci1;};
  }
	else/* add else - 2021-07-28-by zyg*/
	{
		
	}
	
  ci = *t2Up - *t1Up;
  if(ci < (EMO_SVM_MINTIME))
  {
    ci = (EMO_SVM_MINTIME) - ci;
    ci1 = *t2Down + ci;
    if (ci1 > ((CCU6_T12PR) - (EMO_SVM_DEADTIME))) {*t2Down = (CCU6_T12PR) + 1;}else{*t2Down = ci1;};
    ci1 = *t2Up - ci;
    if (ci1 < (EMO_SVM_DEADTIME)) {*t2Up = 0;}else{*t2Up = ci1;};
  }
	else/* add else - 2021-07-28-by zyg*/
	{
		
	}

  pCmpValBuf->CompT13ValueUp = ((CCU6_T12PR)<<1u) - ((*t0Down + *t1Down)>>1u);
  pCmpValBuf->CompT13ValueDown = ((CCU6_T12PR)<<1u) - ((*t2Down + *t1Down)>>1u);
	
	pCmpValBuf->T1 = T1;
	pCmpValBuf->T2 = T2;
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Emo_RAM_lExeSvm
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
__STATIC_INLINE void Emo_RAM_lExeSvm(TEmo_Svm *pSvm)
{
  sint32 T1;
  sint32 T2;
  uint32 Sector;
  uint32 Angle;
  uint32 Index;

  if(pSvm->Amp == 0u)
  {
    pCmpValBuf->comp60up = (CCU6_T12PR)/2u;
    pCmpValBuf->comp60down = (CCU6_T12PR)/2u;
    pCmpValBuf->comp61up = (CCU6_T12PR)/2u;
    pCmpValBuf->comp61down = (CCU6_T12PR)/2u;
    pCmpValBuf->comp62up = (CCU6_T12PR)/2u;
    pCmpValBuf->comp62down = (CCU6_T12PR)/2u;
    pCmpValBuf->CompT13ValueUp = (CCU6_T12PR);
    pCmpValBuf->CompT13ValueDown = (CCU6_T12PR);
  }
  else
  {
    /* Calculate sector number 0..5 and table index 0..255 */
    Angle = ((uint32)pSvm->Angle) * 6u;
    Sector = (Angle >> 16u) & 7u;
    pSvm->Sector = (uint16)Sector;
    Index = (Angle >> 8u) & 0xFFu;
    /* Calculate and limit times */
    T1 = (((uint32)pSvm->Amp) * Table_Sin60[255u - Index]) >> (MAT_FIX_SHIFT + 1u);
    pSvm->T1 = (sint16)T1;
    /* RandVector1 = Amp * sin(gamma) */
    T2 = (((uint32)pSvm->Amp) * Table_Sin60[Index]) >> (MAT_FIX_SHIFT + 1u);
    pSvm->T2 = (sint16)T2;

    /* Set compare values according to sector number */
    switch (Sector)
    {
      case 0u:
        Emo_RAM_T2C(T1, T2, 0x012u);
        break;
      case 1u:
        Emo_RAM_T2C(T2, T1, 0x102u);
        break;
      case 2u:
        Emo_RAM_T2C(T1, T2, 0x120u);
        break;
      case 3u:
        Emo_RAM_T2C(T2, T1, 0x210u);
        break;
      case 4u:
        Emo_RAM_T2C(T1, T2, 0x201u);
        break;
      default: /* case 5u: */
        Emo_RAM_T2C(T2, T1, 0x021u);
        break;
    }
  }

#if DEBUG_CODE /* Disable debug code - 2021-07-29-by zyg  */
	EmoWatch.Va = pCmpValBuf->comp60up + pCmpValBuf->comp60down - (CCU6_T12PR);
	EmoWatch.Vb = pCmpValBuf->comp61up + pCmpValBuf->comp61down - (CCU6_T12PR);
	EmoWatch.Vab = EmoWatch.Va - EmoWatch.Vb;
#endif
	
  sint32 int_was_mask = CMSIS_Irq_Dis();
	*pCmpVal = *pCmpValBuf;
  if (int_was_mask == 0)
  {
    CMSIS_Irq_En();
  }
  else
  {

  }
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Emo_RAM_lEstFlux
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
__STATIC_INLINE void Emo_RAM_lEstFlux(void)
{
  sint16 Temp;
  /* uint16 Tempu; */
  uint16 FluxAbsValue;
  TComplex fluxh = {0, 0};
  static TComplex Fluxrf;
  /* static TComplex Flux; */
  /* Get stator flux in real axis */
  Temp = __ssat(Fluxrf.Real + Emo_Foc.StatVolt.Real - Mat_FixMul(Emo_Foc.StatCurr.Real, Emo_Foc.PhaseRes), MAT_FIX_SAT);
  fluxh.Real = Mat_ExeLp_without_min_max(&Emo_Foc.RealFluxLp, Temp);
  Flux.Real = __ssat(fluxh.Real - Mat_FixMulScale(Emo_Foc.StatCurr.Real, Emo_Foc.PhaseInd, 0), MAT_FIX_SAT);
	
#if DEBUG_CODE /* Disable debug code - 2021-07-29-by zyg  */	
	EmoWatch.UmiRI = Temp;
#endif	
	
  /* Get stator flux in imaginary axis */
  Temp = __ssat(Fluxrf.Imag + Emo_Foc.StatVolt.Imag - Mat_FixMul(Emo_Foc.StatCurr.Imag, Emo_Foc.PhaseRes), MAT_FIX_SAT);
  fluxh.Imag = Mat_ExeLp_without_min_max(&Emo_Foc.ImagFluxLp, Temp);
  Flux.Imag = __ssat(fluxh.Imag - Mat_FixMulScale(Emo_Foc.StatCurr.Imag, Emo_Foc.PhaseInd, 0), MAT_FIX_SAT);
	
  /* Calculate flux angle */
  /* Tempu => FluxAmplitude */
  Emo_Foc.FluxAngle =  Mat_CalcAngleAmp(Flux, &Tempu) + AngleOffset;
	if(Emo_Foc.FluxAngle == 0u)
	{
		
#if DEBUG_CODE /* Disable debug code - 2021-07-29-by zyg  */	
		EmoWatch.FaiAlphaAngle0 = Flux.Real;
#endif
		
	}
  else
  {

  }
	
  /*Tempu = (Tempu * 32000) / 32768 => ensure that FluxAmplitude will not clamp*/
  FluxAbsValue = __ssat(Mat_FixMul(Tempu, 32000u), MAT_FIX_SAT + 1);
  /*filtered FluxBetrag as reference for checkings below*/
  Temp = Mat_ExeLp_without_min_max(&Emo_Ctrl.FluxbtrLp, FluxAbsValue);

#if DEBUG_CODE /* Disable debug code - 2021-07-29-by zyg  */	
	EmoWatch.Valpha = Emo_Foc.StatVolt.Real;
	EmoWatch.Vbeta = Emo_Foc.StatVolt.Imag;
	
	EmoWatch.IntegralUmiRI = fluxh.Real;
	EmoWatch.FaiAlpha = Flux.Real;
	
	EmoWatch.FaiAmp = FluxAbsValue;
	EmoWatch.FaiAmpLp = Temp;
#endif

  if(Flux.Real > Temp)
  {
    Fluxrf.Real = -400;
  }
  else
  {
    if(Flux.Real < -Temp)
    {
      Fluxrf.Real = 400;
    }
    else
    {
      Fluxrf.Real = 0;
    }
  }

  if(Flux.Imag > Temp)
  {
    Fluxrf.Imag = -400;
  }
  else
  {
    if (Flux.Imag < -Temp)
    {
      Fluxrf.Imag = 400;
    }
    else
    {
      Fluxrf.Imag = 0;
    }
  }
}

/* BEGIN_FUNCTION_HDR
********************************************************************
* Function Name:Emo_RAM_FluxAnglePll
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
__STATIC_INLINE void Emo_RAM_FluxAnglePll(void)
{
  /* sint16 deltaphi; */
  /* sint16 domega; */
  deltaphi = Emo_Foc.FluxAngle - Emo_Ctrl.FluxAnglePll;
  domega = __ssat(Mat_FixMulScale(deltaphi, Emo_Ctrl.Pllkp, 0) + Emo_Ctrl.Speedpll, MAT_FIX_SAT);
  Emo_Ctrl.FluxAnglePll = Emo_Ctrl.FluxAnglePll + domega;
}

#if (EMO_DECOUPLING==1)
sint16 Kentk = 0;
__STATIC_INLINE TComplex Emo_CurrentDecoupling(void)
{
  TComplex StatOut = {0, 0};
  /* sint16 Kentk = 10547; */
  sint16 Oml;
  Oml = Mat_FixMulScale(Kentk, Emo_Ctrl.SpeedLpdisplay.Out/4, 0);
  StatOut.Real = __ssat(Emo_Foc.RotVoltCurrentcontrol.Real - Mat_FixMulScale(Emo_Foc.RotCurr.Imag, Oml , 1), MAT_FIX_SAT);
  StatOut.Imag = __ssat(Emo_Foc.RotVoltCurrentcontrol.Imag + Mat_FixMulScale(Emo_Foc.RotCurr.Real, Oml , 1), MAT_FIX_SAT);
  return StatOut;
}
#endif
